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
#ifndef _H_AGENTQUERY
#define _H_AGENTQUERY

#include "securityserver.h"
#include <security_agent_client/agentclient.h>
#include <security_cdsa_utilities/AuthorizationData.h>
#include <Security/AuthorizationPlugin.h>
#include "kcdatabase.h"
#include "AuthorizationEngine.h"

using Authorization::AuthItemSet;
using Authorization::AuthValueVector;
using Security::CodeSigning::OSXCode;
//
// The common machinery of retryable SecurityAgent queries
//
class Session;

class SecurityAgentQuery : protected SecurityAgent::Client {
public:
	typedef SecurityAgent::Reason Reason;
	
	static const char defaultName[];

	SecurityAgentQuery();
	SecurityAgentQuery(uid_t clientUID, const Session &clientSession, const char *agentName = defaultName);
	
	void inferHints(Process &thisProcess);

	virtual ~SecurityAgentQuery();

	virtual void activate();
	virtual void terminate();

public:
	void readChoice();

	bool allow;
	bool remember;

protected:
	AuthItemSet mClientHints;
private:
	const Session &mClientSession;
};

//
// Specialized for "rogue app" alert queries
//
class QueryKeychainUse : public SecurityAgentQuery {
public:
    QueryKeychainUse(bool needPass, const Database *db);
    Reason queryUser (const char* database, const char *description, AclAuthorization action);

private:
    const KeychainDatabase *mPassphraseCheck; // NULL to not check passphrase
};


//
// Specialized for code signature adjustment queries
//
class QueryCodeCheck : public SecurityAgentQuery {
public:
    bool operator () (const char *aclPath);
};


//
// A query for an existing passphrase
//
class QueryUnlock : public SecurityAgentQuery {
	static const int maxTries = kMaximumAuthorizationTries;
public:
	QueryUnlock(KeychainDatabase &db) : database(db) { }
	
	KeychainDatabase &database;
	
	Reason operator () ();
	
protected:
	Reason query();
	void queryInteractive(CssmOwnedData &passphrase);
	void retryInteractive(CssmOwnedData &passphrase, Reason reason);
	Reason accept(CssmManagedData &passphrase);
};


//
// A query for a new passphrase
//
class QueryNewPassphrase : public SecurityAgentQuery {
	static const int maxTries = 7;
public:
	QueryNewPassphrase(KeychainDatabase &db, Reason reason) :
	    database(db), initialReason(reason),
	    mPassphrase(Allocator::standard(Allocator::sensitive)),
	    mPassphraseValid(false) { }

	KeychainDatabase &database;
	
	Reason operator () (CssmOwnedData &passphrase);
	
protected:
	Reason query();
	Reason accept(CssmManagedData &passphrase, CssmData *oldPassphrase);
	
private:
	Reason initialReason;
	CssmAutoData mPassphrase;
    bool mPassphraseValid;
};


//
// Generic passphrase query (not associated with a database)
//
class QueryGenericPassphrase : public SecurityAgentQuery {
public:
    QueryGenericPassphrase()    { }
    Reason operator () (const char *prompt, bool verify, 
                        string &passphrase);
    
protected:
    Reason query(const char *prompt, bool verify, string &passphrase);
};


class QueryInvokeMechanism : public RefCount, SecurityAgentQuery {
public:
    QueryInvokeMechanism();
    QueryInvokeMechanism(uid_t clientUID, const Session &session, const char *agentName = NULL);
    void initialize(const string &inPluginId, const string &inMechanismId, const SessionId inSessionId = 0);
    void run(const AuthValueVector &inArguments, AuthItemSet &inHints, AuthItemSet &inContext, AuthorizationResult *outResult);

    bool operator () (const string &inPluginId, const string &inMechanismId, const Authorization::AuthValueVector &inArguments, AuthItemSet &inHints, AuthItemSet &inContext, AuthorizationResult *outResult);
    void terminateAgent();
    //~QueryInvokeMechanism();
};

#endif //_H_AGENTQUERY
