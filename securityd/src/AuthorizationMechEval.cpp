/*
 *  Copyright (c) 2003-2004 Apple Computer, Inc. All Rights Reserved.
 *
 *  @APPLE_LICENSE_HEADER_START@
 *  
 *  Copyright (c) 1999-2003 Apple Computer, Inc.  All Rights Reserved.
 *  
 *  This file contains Original Code and/or Modifications of Original Code
 *  as defined in and that are subject to the Apple Public Source License
 *  Version 2.0 (the 'License'). You may not use this file except in
 *  compliance with the License. Please obtain a copy of the License at
 *  http://www.opensource.apple.com/apsl/ and read it before using this
 *  file.
 *  
 *  The Original Code and all software distributed under the License are
 *  distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 *  EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 *  INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 *  Please see the License for the specific language governing rights and
 *  limitations under the License.
 *  
 *  @APPLE_LICENSE_HEADER_END@
 *
 *  AuthorizationMechEval.cpp
 *  securityd
 *
 */

#include "AuthorizationMechEval.h"

namespace Authorization {

AgentMechanismRef::AgentMechanismRef(uid_t clientUID, const Session &session) : 
    RefPointer<QueryInvokeMechanism>(new QueryInvokeMechanism(clientUID, session)) {}

AgentMechanismRef::AgentMechanismRef() : 
    RefPointer<QueryInvokeMechanism>(new QueryInvokeMechanism()) {}

// we need the vector<string> of mechanisms
AgentMechanismEvaluator::AgentMechanismEvaluator(uid_t uid, const Session& session, const vector<string>& inMechanisms) : 
    mMechanisms(inMechanisms), mClientUid(uid), mSession(session)
{
    //set up environment
}

OSStatus
AgentMechanismEvaluator::run(const AuthValueVector &inArguments, const AuthItemSet &inHints, const AuthItemSet &inContext)
{
    // add process specifics to context?

    vector<std::string>::const_iterator currentMechanism = mMechanisms.begin();
    
    AuthorizationResult result = kAuthorizationResultAllow;
    
    AuthItemSet hints = inHints;
    AuthItemSet context = inContext;
        
    while ( (result == kAuthorizationResultAllow)  &&
            (currentMechanism != mMechanisms.end()) ) // iterate mechanisms
    {
        ClientMap::iterator iter = mClients.find(*currentMechanism);
        if (iter == mClients.end())
        {
            string::size_type extPlugin = currentMechanism->find(':');
            if (extPlugin != string::npos)
            {
                // no whitespace removal
                string pluginIn(currentMechanism->substr(0, extPlugin));
                string mechanismIn(currentMechanism->substr(extPlugin + 1));
                secdebug("AuthEvalMech", "external mech %s:%s", pluginIn.c_str(), mechanismIn.c_str());
                
                AgentMechanismRef client(mClientUid, mSession);
                client->initialize(pluginIn, mechanismIn);
// XXX/cs                client->inferHints(Server::process());
                mClients[*currentMechanism] = client;
            }
            else if (*currentMechanism == "authinternal")
            {
                secdebug("AuthEvalMech", "performing authentication");
                result = authinternal(context);
            }
            else if (*currentMechanism == "push_hints_to_context")
            {
                secdebug("AuthEvalMech", "evaluate push_hints_to_context");
                result = kAuthorizationResultAllow; // snarfcredential doesn't block evaluation, ever, it may restart
                                                    // create out context from input hints, no merge
                                                    // @@@ global copy template not being invoked...
                context = hints;
            }
#if 0
            else if (*currentMechanism == "switch_to_user")
            {
                AgentMechanismRef client(mClientUid, mSession);
                client->terminate();
            }
#endif
            else
                return errAuthorizationInternal;
        }

        iter = mClients.find(*currentMechanism);

        if (iter != mClients.end())
        {
            try
            {
                iter->second->run(inArguments, hints, context, &result);
                secdebug("AuthEvalMech", "evaluate(%s) succeeded with result: %lu.", (iter->first).c_str(), result);
            }
            catch (...) {
                secdebug("AuthEvalMech", "exception from mech eval or client death");
                result = kAuthorizationResultUndefined;
            }
        }
    
        // we own outHints and outContext
        switch(result)
        {
            case kAuthorizationResultAllow:
                secdebug("AuthEvalMech", "result allow");
                currentMechanism++;
                break;
            case kAuthorizationResultDeny:
                secdebug("AuthEvalMech", "result deny");
                break;
            case kAuthorizationResultUndefined:
                secdebug("AuthEvalMech", "result undefined");
                break; // abort evaluation
            case kAuthorizationResultUserCanceled:
                secdebug("AuthEvalMech", "result canceled");
                break; // stop evaluation, return some sideband
            default:
                break; // abort evaluation
        }
    }
    
    if ((result == kAuthorizationResultUserCanceled) ||
        (result == kAuthorizationResultAllow))
    {
        mHints = hints;
        mContext = context;
    }
    
    // convert AuthorizationResult to OSStatus
    switch(result)
    {
        case kAuthorizationResultDeny:
            return errAuthorizationDenied;
        case kAuthorizationResultUserCanceled:
            return errAuthorizationCanceled;
        case kAuthorizationResultAllow:
            return errAuthorizationSuccess;
        default:
            return errAuthorizationInternal;
    }    
}

AuthorizationResult AgentMechanismEvaluator::authinternal(AuthItemSet &context)
{
    secdebug("AuthEvalMech", "evaluate authinternal");
    do {
        AuthItemSet::iterator found = find_if(context.begin(), context.end(), FindAuthItemByRightName(kAuthorizationEnvironmentUsername) );
        if (found == context.end())
            break;
        string username(static_cast<const char *>((*found)->value().data), (*found)->value().length);
        secdebug("AuthEvalMech", "found username");
        found = find_if(context.begin(), context.end(), FindAuthItemByRightName(kAuthorizationEnvironmentPassword) );
        if (found == context.end())
            break;
        string password(static_cast<const char *>((*found)->value().data), (*found)->value().length);
        secdebug("AuthEvalMech", "found password");
        Credential newCredential(username, password, true); // create a new shared credential
        
        if (newCredential->isValid())
        {
            Syslog::info("authinternal authenticated user %s (uid %lu).", newCredential->username().c_str(), newCredential->uid());
            return kAuthorizationResultAllow;
        }
        
        Syslog::error("authinternal failed to authenticate user %s.", newCredential->username().c_str());
        
    } while (0);
    
    return kAuthorizationResultDeny;
}

/*
AuthItemSet &
AgentMechanismEvaluator::commonHints(const AuthorizationToken &auth)
{
    
}
*/

} /* namespace Authorization */
