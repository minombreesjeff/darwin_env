/*
 * Copyright (c) 2000-2004 Apple Computer, Inc. All Rights Reserved.
 * 
 * @APPLE_LICENSE_HEADER_START@
 * 
 * Copyright (c) 1999-2003 Apple Computer, Inc.  All Rights Reserved.
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

//
// passphrases - canonical code to obtain passphrases
//
#include "agentquery.h"
#include "authority.h"
#include "server.h"
#include "session.h"

#include <Security/AuthorizationTags.h>
#include <Security/AuthorizationTagsPriv.h>

//
// NOSA support functions. This is a test mode where the SecurityAgent
// is simulated via stdio in the client. Good for running automated tests
// of client programs. Only available if -DNOSA when compiling.
//
#if defined(NOSA)

#include <cstdarg>

static void getNoSA(char *buffer, size_t bufferSize, const char *fmt, ...)
{
	// write prompt
	va_list args;
	va_start(args, fmt);
	vfprintf(stdout, fmt, args);
	va_end(args);

	// read reply
	memset(buffer, 0, bufferSize);
	const char *nosa = getenv("NOSA");
	if (!strcmp(nosa, "-")) {
		if (fgets(buffer, bufferSize-1, stdin) == NULL)
			CssmError::throwMe(CSSM_ERRCODE_NO_USER_INTERACTION);
		buffer[strlen(buffer)-1] = '\0';	// remove trailing newline
		if (!isatty(fileno(stdin)))
			printf("%s\n", buffer);			// echo to output if input not terminal
	} else {
		strncpy(buffer, nosa, bufferSize-1);
		printf("%s\n", buffer);
	}
	if (buffer[0] == '\0')				// empty input -> cancellation
		CssmError::throwMe(CSSM_ERRCODE_USER_CANCELED);
}

#endif //NOSA


//
// The default Mach service name for SecurityAgent
//
const char SecurityAgentQuery::defaultName[] = "com.apple.SecurityAgent";

using SecurityAgent::Reason;
using namespace Authorization;

//
// Construct a query object
//

SecurityAgentQuery::SecurityAgentQuery() :
    SecurityAgent::Client(Server::process().uid(),
		Server::session().bootstrapPort(),
		defaultName),
	mClientSession(Server::session())
{
    secdebug("SecurityAgentQuery", "new query");
        
	// XXX/cs set up the general settings for the client as hints
		// any invoke will merge these and whatever the client passes on invoke
	
}

SecurityAgentQuery::SecurityAgentQuery(uid_t clientUID,
                                       const Session &clientSession,
                                       const char *agentName) :
SecurityAgent::Client(clientUID, clientSession.bootstrapPort(), agentName ? agentName : defaultName),
	mClientSession(clientSession)
{
    secdebug("SecurityAgentQuery", "new query");
}

SecurityAgentQuery::~SecurityAgentQuery()
{
    secdebug("SecurityAgentQuery", "query dying");
    Server::connection().useAgent(NULL);

#if defined(NOSA)
	if (getenv("NOSA")) {
		printf(" [query done]\n");
		return;
	}
#endif		

    if (state() != dead)
        destroy(); 
}

void
SecurityAgentQuery::activate()
{
	if (isActive())
		return;

	// Before popping up an agent: is UI session allowed?
	if (!(mClientSession.attributes() & sessionHasGraphicAccess))
		CssmError::throwMe(CSSM_ERRCODE_NO_USER_INTERACTION);

	// this may take a while
	Server::active().longTermActivity();
	Server::connection().useAgent(this);

	try {
		SecurityAgent::Client::activate();
	} catch (...) {
		Server::connection().useAgent(NULL);	// guess not
		throw;
	}
	
	
}


void
SecurityAgentQuery::inferHints(Process &thisProcess)
{
	AuthItemSet processHints = clientHints(thisProcess.clientCode(), thisProcess.pid(), thisProcess.uid());
    mClientHints.insert(processHints.begin(), processHints.end());
}

void
SecurityAgentQuery::readChoice()
{
    allow = false;
    remember = false;
    
	AuthItem *allowAction = mContext.find(AGENT_CONTEXT_ALLOW);
	if (allowAction)
	{
	   string allowString;
		if (allowAction->getString(allowString) 
		      && (allowString == "YES"))
		          allow = true;
	}
	
	AuthItem *rememberAction = mContext.find(AGENT_CONTEXT_REMEMBER_ACTION);
	if (rememberAction)
	{
	   string rememberString;
	   if (rememberAction->getString(rememberString)
	           && (rememberString == "YES"))
	               remember = true;
	}
}	

void
SecurityAgentQuery::terminate()
{
	if (!isActive())
		activate();

    // @@@ This happens already in the destructor; presumably we do this to tear things down orderly
	Server::connection(true).useAgent(NULL);
    
	SecurityAgent::Client::terminate();
}


//
// Perform the "rogue app" access query dialog
//
QueryKeychainUse::QueryKeychainUse(bool needPass, const Database *db)
	: mPassphraseCheck(NULL)
{
	// if passphrase checking requested, save KeychainDatabase reference
	// (will quietly disable check if db isn't a keychain)
	if (needPass)
		mPassphraseCheck = dynamic_cast<const KeychainDatabase *>(db);
}

Reason QueryKeychainUse::queryUser (const char *database, const char *description, AclAuthorization action)
{
    Reason reason = SecurityAgent::noReason;
    int retryCount = 0;
	OSStatus status;
	AuthValueVector arguments;
	AuthItemSet hints, context;

#if defined(NOSA)
	if (getenv("NOSA")) {
		char answer[maxPassphraseLength+10];
		
        string applicationPath;
        AuthItem *applicationPathItem = mClientHints.find(AGENT_HINT_APPLICATION_PATH);
		if (applicationPathItem)
		  applicationPathItem->getString(applicationPath);

		getNoSA(answer, sizeof(answer), "Allow %s to do %d on %s in %s? [yn][g]%s ",
			applicationPath.c_str(), int(action), (description ? description : "[NULL item]"),
			(database ? database : "[NULL database]"),
			mPassphraseCheck ? ":passphrase" : "");
		// turn passphrase (no ':') into y:passphrase
		if (mPassphraseCheck && !strchr(answer, ':')) {
			memmove(answer+2, answer, strlen(answer)+1);
			memcpy(answer, "y:", 2);
		}

		allow = answer[0] == 'y';
		remember = answer[1] == 'g';
		return SecurityAgent::noReason;
	}
#endif

    activate();
    
	// prepopulate with client hints
	hints.insert(mClientHints.begin(), mClientHints.end());
	
	// put action/operation (sint32) into hints
	hints.insert(AuthItemRef(AGENT_HINT_ACL_TAG, AuthValueOverlay(sizeof(action), static_cast<SInt32*>(&action))));
	
	// item name into hints

	hints.insert(AuthItemRef(AGENT_HINT_KEYCHAIN_ITEM_NAME, AuthValueOverlay(description ? strlen(description) : 0, const_cast<char*>(description))));
	
	// keychain name into hints
	hints.insert(AuthItemRef(AGENT_HINT_KEYCHAIN_PATH, AuthValueOverlay(database ? strlen(database) : 0, const_cast<char*>(database))));
	
	
	if (mPassphraseCheck)
	{
		status = create("builtin", "confirm-access-password", NULL);
		
		CssmAutoData data(Allocator::standard(Allocator::sensitive));

		do 
		{

            AuthItemRef triesHint(AGENT_HINT_TRIES, AuthValueOverlay(sizeof(retryCount), &retryCount));
            hints.erase(triesHint); hints.insert(triesHint); // replace
            
            if (retryCount++ > kMaximumAuthorizationTries)
			{
                reason = SecurityAgent::tooManyTries;
			}
				
            AuthItemRef retryHint(AGENT_HINT_RETRY_REASON, AuthValueOverlay(sizeof(reason), &reason));
            hints.erase(retryHint); hints.insert(retryHint); // replace

			status = invoke(arguments, hints, context);

            if (retryCount > kMaximumAuthorizationTries)
			{
                return reason;
			}
			
			checkResult();
			
			AuthItem *passwordItem = mContext.find(kAuthorizationEnvironmentPassword);
			if (!passwordItem)
				continue;
						
			passwordItem->getCssmData(data);
		} 
		while (reason = (const_cast<KeychainDatabase*>(mPassphraseCheck)->decode(data) ? SecurityAgent::noReason : SecurityAgent::invalidPassphrase));
	}
	else
	{
		create("builtin", "confirm-access", NULL);
		invoke(arguments, hints, context);
	}
	
    readChoice();
    
	return reason;
}

//
// Perform code signature ACL access adjustment dialogs
//
bool QueryCodeCheck::operator () (const char *aclPath)
{
	OSStatus status;
	AuthValueVector arguments;
	AuthItemSet hints, context;
	
#if defined(NOSA)
	if (getenv("NOSA")) {
		char answer[10];
		
        string applicationPath;
        AuthItem *applicationPathItem = mClientHints.find(AGENT_HINT_APPLICATION_PATH);
		if (applicationPathItem)
		  applicationPathItem->getString(applicationPath);

		getNoSA(answer, sizeof(answer),
				"Allow %s to match an ACL for %s [yn][g]? ",
				applicationPath.c_str(), aclPath ? aclPath : "(unknown)");
		allow = answer[0] == 'y';
		remember = answer[1] == 'g';
		return;
	}
#endif

	// prepopulate with client hints
	hints.insert(mClientHints.begin(), mClientHints.end());
	
	hints.insert(AuthItemRef(AGENT_HINT_APPLICATION_PATH, AuthValueOverlay(strlen(aclPath), const_cast<char*>(aclPath))));
	
	MacOSError::check(create("builtin", "code-identity", NULL));

	status = invoke(arguments, hints, context);
		
    checkResult();

//	MacOSError::check(status);

    return kAuthorizationResultAllow == result();
}


//
// Obtain passphrases and submit them to the accept() method until it is accepted
// or we can't get another passphrase. Accept() should consume the passphrase
// if it is accepted. If no passphrase is acceptable, throw out of here.
//
Reason QueryUnlock::query()
{
	Reason reason = SecurityAgent::noReason;
	OSStatus status;
	AuthValueVector arguments;
	AuthItemSet hints, context;
	CssmAutoData passphrase(Allocator::standard(Allocator::sensitive));
	int retryCount = 0;
	
#if defined(NOSA)
    // return the passphrase
	if (getenv("NOSA")) {
        char passphrase_[maxPassphraseLength];
		getNoSA(passphrase, maxPassphraseLength, "Unlock %s [<CR> to cancel]: ", database.dbName());
	passphrase.copy(passphrase_, strlen(passphrase_));
    	return database.decode(passphrase) ? SecurityAgent::noReason : SecurityAgent::invalidPassphrase;
	}
#endif
    activate();

	
	// prepopulate with client hints

    const char *keychainPath = database.dbName();
    hints.insert(AuthItemRef(AGENT_HINT_KEYCHAIN_PATH, AuthValueOverlay(strlen(keychainPath), const_cast<char*>(keychainPath))));

	hints.insert(mClientHints.begin(), mClientHints.end());

	MacOSError::check(create("builtin", "unlock-keychain", NULL));

	do
	{
        AuthItemRef triesHint(AGENT_HINT_TRIES, AuthValueOverlay(sizeof(retryCount), &retryCount));
        hints.erase(triesHint); hints.insert(triesHint); // replace
		
        ++retryCount;
            
        if (retryCount > maxTries)
		{
			reason = SecurityAgent::tooManyTries;
        }
            
        AuthItemRef retryHint(AGENT_HINT_RETRY_REASON, AuthValueOverlay(sizeof(reason), &reason));
        hints.erase(retryHint); hints.insert(retryHint); // replace

        status = invoke(arguments, hints, context);

        if (retryCount > maxTries)
        {
			return reason;
		}

        checkResult();
		
		AuthItem *passwordItem = mContext.find(kAuthorizationEnvironmentPassword);
		if (!passwordItem)
			continue;
		
		passwordItem->getCssmData(passphrase);
	}
	while (reason = database.decode(passphrase) ? SecurityAgent::noReason : SecurityAgent::invalidPassphrase);

	return SecurityAgent::noReason;
}


//
// Get existing passphrase (unlock) Query
//
Reason QueryUnlock::operator () ()
{
	return query();
}


//
// Obtain passphrases and submit them to the accept() method until it is accepted
// or we can't get another passphrase. Accept() should consume the passphrase
// if it is accepted. If no passphrase is acceptable, throw out of here.
//
Reason QueryNewPassphrase::query()
{
	Reason reason = initialReason;
	CssmAutoData passphrase(Allocator::standard(Allocator::sensitive));
	CssmAutoData oldPassphrase(Allocator::standard(Allocator::sensitive));

    OSStatus status;
	AuthValueVector arguments;
	AuthItemSet hints, context;
    
	int retryCount = 0;

#if defined(NOSA)
	if (getenv("NOSA")) {
        char passphrase_[maxPassphraseLength];
		getNoSA(passphrase_, maxPassphraseLength,
			"New passphrase for %s (reason %d) [<CR> to cancel]: ",
			database.dbName(), reason);
		return SecurityAgent::noReason;
	}
#endif

    activate();

	// prepopulate with client hints
	hints.insert(mClientHints.begin(), mClientHints.end());

	// keychain name into hints
	hints.insert(AuthItemRef(AGENT_HINT_KEYCHAIN_PATH, AuthValueOverlay(database.dbName())));

    switch (initialReason)
    {
        case SecurityAgent::newDatabase: 
            MacOSError::check(create("builtin", "new-passphrase", NULL));
            break;
        case SecurityAgent::changePassphrase:
            MacOSError::check(create("builtin", "change-passphrase", NULL));
            break;
        default:
            assert(false);
    }

	do
	{
        AuthItemRef triesHint(AGENT_HINT_TRIES, AuthValueOverlay(sizeof(retryCount), &retryCount));
        hints.erase(triesHint); hints.insert(triesHint); // replace

		if (++retryCount > maxTries)
		{
			reason = SecurityAgent::tooManyTries;
		}

        AuthItemRef retryHint(AGENT_HINT_RETRY_REASON, AuthValueOverlay(sizeof(reason), &reason));
        hints.erase(retryHint); hints.insert(retryHint); // replace

		status = invoke(arguments, hints, context);

		if (retryCount > maxTries)
		{
            return reason;
        }
        
        checkResult();

		if (SecurityAgent::changePassphrase == initialReason)
        {
            AuthItem *oldPasswordItem = mContext.find(AGENT_PASSWORD);
            if (!oldPasswordItem)
                continue;
            
            oldPasswordItem->getCssmData(oldPassphrase);
        }
        
		AuthItem *passwordItem = mContext.find(AGENT_CONTEXT_NEW_PASSWORD);
		if (!passwordItem)
			continue;
		
		passwordItem->getCssmData(passphrase);
        
    }
	while (reason = accept(passphrase, (initialReason == SecurityAgent::changePassphrase) ? &oldPassphrase.get() : NULL));
    
	return SecurityAgent::noReason;
}


//
// Get new passphrase Query
//
Reason QueryNewPassphrase::operator () (CssmOwnedData &passphrase)
{
	if (Reason result = query())
		return result;	// failed
	passphrase = mPassphrase;
	return SecurityAgent::noReason;	// success
}

Reason QueryNewPassphrase::accept(CssmManagedData &passphrase, CssmData *oldPassphrase)
{
	//@@@ acceptance criteria are currently hardwired here
	//@@@ This validation presumes ASCII - UTF8 might be more lenient
	
	// if we have an old passphrase, check it
	if (oldPassphrase && !database.validatePassphrase(*oldPassphrase))
		return SecurityAgent::oldPassphraseWrong;
	
	// sanity check the new passphrase (but allow user override)
	if (!(mPassphraseValid && passphrase.get() == mPassphrase)) {
		mPassphrase = passphrase;
		mPassphraseValid = true;
		if (mPassphrase.length() == 0)
			return SecurityAgent::passphraseIsNull;
		if (mPassphrase.length() < 6)
			return SecurityAgent::passphraseTooSimple;
	}
	
	// accept this
	return SecurityAgent::noReason;
}

// 
// Get a passphrase for unspecified use
// 
Reason QueryGenericPassphrase::operator () (const char *prompt, bool verify, 
                                            string &passphrase)
{
    return query(prompt, verify, passphrase);
}

Reason QueryGenericPassphrase::query(const char *prompt, bool verify, 
                                     string &passphrase)
{
    Reason reason = SecurityAgent::noReason;
    OSStatus status;    // not really used; remove?  
    AuthValueVector arguments;
    AuthItemSet hints, context;
	
#if defined(NOSA)
    if (getenv("NOSA")) {
		// FIXME  3690984
		return SecurityAgent::noReason;
    }
#endif
	
    activate();
	
    hints.insert(mClientHints.begin(), mClientHints.end());
    hints.insert(AuthItemRef(AGENT_HINT_CUSTOM_PROMPT, AuthValueOverlay(prompt ? strlen(prompt) : 0, const_cast<char*>(prompt))));
    // XXX/gh  defined by dmitch but no analogous hint in
    // AuthorizationTagsPriv.h:
    // CSSM_ATTRIBUTE_ALERT_TITLE (optional alert panel title)
	
    if (false == verify) {  // import
		MacOSError::check(create("builtin", "generic-unlock", NULL));
    } else {		// verify passphrase (export)
					// new-passphrase-generic works with the pre-4 June 2004 agent; 
					// generic-new-passphrase is required for the new agent
		MacOSError::check(create("builtin", "generic-new-passphrase", NULL));
    }
    
    AuthItem *passwordItem;
    
    do {
		
		status = invoke(arguments, hints, context);
		checkResult();
		passwordItem = mContext.find(AGENT_PASSWORD);
		
    } while (!passwordItem);
	
    passwordItem->getString(passphrase);
    
    return reason;
}


QueryInvokeMechanism::QueryInvokeMechanism() :
    SecurityAgentQuery() { }

QueryInvokeMechanism::QueryInvokeMechanism(uid_t clientUID, const Session &session, const char *agentName) :
	SecurityAgentQuery(clientUID, session, agentName) 
{
}

void QueryInvokeMechanism::initialize(const string &inPluginId, const string &inMechanismId, const SessionId inSessionId)
{
    activate();

    if (init == state())
        MacOSError::check(create(inPluginId.c_str(), inMechanismId.c_str(), inSessionId));
}

// XXX/cs should return AuthorizationResult
void QueryInvokeMechanism::run(const AuthValueVector &inArguments, AuthItemSet &inHints, AuthItemSet &inContext, AuthorizationResult *outResult)
{
    // prepopulate with client hints
	inHints.insert(mClientHints.begin(), mClientHints.end());

    MacOSError::check(invoke(inArguments, inHints, inContext));
    
	if (outResult) *outResult = result();
    
    inHints = hints();
    inContext = context();
}

void QueryInvokeMechanism::terminateAgent()
{
    terminate();
}

