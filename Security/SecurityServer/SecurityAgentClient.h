/*
 * Copyright (c) 2000-2001 Apple Computer, Inc. All Rights Reserved.
 * 
 * The contents of this file constitute Original Code as defined in and are
 * subject to the Apple Public Source License Version 1.2 (the 'License').
 * You may not use this file except in compliance with the License. Please obtain
 * a copy of the License at http://www.apple.com/publicsource and read it before
 * using this file.
 * 
 * This Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS
 * OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT. Please see the License for the
 * specific language governing rights and limitations under the License.
 */


//
// SecurityAgentClient - client interface to SecurityAgent
//
#ifndef _H_SECURITYAGENTCLIENT
#define _H_SECURITYAGENTCLIENT

#if defined(__cplusplus)
#include <string>
#include <Security/mach++.h>
#include <Security/osxsigning.h>
#include <Security/cssmacl.h>
#include <Security/cssm.h>
#include <Security/Authorization.h>
#include <Security/AuthorizationPlugin.h>
#include <Security/AuthorizationWalkers.h>
#include <Security/AuthorizationData.h>

using Authorization::AuthItemSet;
using Authorization::AuthValueVector;

namespace Security {

using MachPlusPlus::Port;
    using MachPlusPlus::Bootstrap;
using CodeSigning::OSXCode;


namespace SecurityAgent {

#endif //C++ only

// Note: Following section also available to C code for inclusion

static const unsigned int maxPassphraseLength = 1024;
static const unsigned int maxUsernameLength = 80;

#define kMaximumAuthorizationTries 3

//
// Unified reason codes transmitted to SecurityAgent (and internationalized there)
//
enum Reason {
	noReason = 0,			// no reason (not used, used as a NULL)
	unknownReason,			// something else (catch-all internal error)
	
	// reasons for asking for a new passphrase
	newDatabase = 11,		// need passphrase for a new database
	changePassphrase,		// changing passphrase for existing database
	
	// reasons for retrying an unlock query
	invalidPassphrase = 21,	// passphrase was wrong
	
	// reasons for retrying a new passphrase query
	passphraseIsNull = 31,	// empty passphrase
	passphraseTooSimple,	// passphrase is not complex enough
	passphraseRepeated,		// passphrase was used before (must use new one)
	passphraseUnacceptable, // passphrase unacceptable for some other reason
	oldPassphraseWrong,		// the old passphrase given is wrong
	
	// reasons for retrying an authorization query
	userNotInGroup = 41,	// authenticated user not in needed group
	unacceptableUser,		// authenticated user unacceptable for some other reason
	
	// reasons for canceling a staged query
	tooManyTries = 61,		// too many failed attempts to get it right
	noLongerNeeded,			// the queried item is no longer needed
	keychainAddFailed,		// the requested itemed couldn't be added to the keychain
	generalErrorCancel		// something went wrong so we have to give up now
};

#define AGENT_HINT_SUGGESTED_USER "suggestedUser"
#define AGENT_HINT_REQUIRE_USER_IN_GROUP "requireUserInGroup"
#define AGENT_HINT_CUSTOM_PROMPT "prompt"
#define AGENT_HINT_AUTHORIZE_RIGHT "authorizeRight"
#define AGENT_HINT_CLIENT_PID "clientPid"
#define AGENT_HINT_CLIENT_UID "clientUid"
#define AGENT_HINT_CREATOR_PID "creatorPid"
#define AGENT_HINT_CLIENT_TYPE "clientType"
#define AGENT_HINT_CLIENT_PATH "clientPath"
#define AGENT_HINT_TRIES "tries"
#define AGENT_HINT_RETRY_REASON "reason"
#define AGENT_HINT_AUTHORIZE_RULE "authorizeRule"
//
// "Login Keychain Creation" Right: Hint and context keys
//
#define AGENT_HINT_ATTR_NAME "loginKCCreate:attributeName"
#define AGENT_HINT_LOGIN_KC_NAME "loginKCCreate:pathName"
#define AGENT_HINT_LOGIN_KC_EXISTS_IN_KC_FOLDER "loginKCCreate:exists"
#define AGENT_HINT_LOGIN_KC_USER_NAME "loginKCCreate:userName"
#define AGENT_HINT_LOGIN_KC_CUST_STR1 "loginKCCreate:customStr1"
#define AGENT_HINT_LOGIN_KC_CUST_STR2 "loginKCCreate:customStr2"
#define AGENT_HINT_LOGIN_KC_USER_HAS_OTHER_KCS_STR "loginKCCreate:moreThanOneKeychainExists"

#define LOGIN_KC_CREATION_RIGHT	"system.keychain.create.loginkc"

#if defined(__cplusplus)


//
// The client interface to the SecurityAgent.
//
class Client {
public:
	Client();
    Client(uid_t clientUID, Bootstrap clientBootstrap, const char *agentName);
	virtual ~Client();

	virtual void activate();
	virtual void terminate();
	bool isActive() const { return mActive; }

	bool keepAlive() const		{ return mKeepAlive; }
	void keepAlive(bool ka)		{ mKeepAlive = ka; }
	
	// common stage termination calls
	void finishStagedQuery();
	void cancelStagedQuery(Reason reason);
    
public:
    struct KeychainBox {
        bool show;				// show the "save in keychain" checkbox (in)
        bool setting;			// value of the checkbox (in/out)
    };
	
public:
	// ask to unlock an existing database. Staged protocol
	void queryUnlockDatabase(const OSXCode *requestor, pid_t requestPid,
        const char *database, char passphrase[maxPassphraseLength]);
	void retryUnlockDatabase(Reason reason, char passphrase[maxPassphraseLength]);
	
	// ask for a new passphrase for a database. Not yet staged
	void queryNewPassphrase(const OSXCode *requestor, pid_t requestPid,
        const char *database, Reason reason, char passphrase[maxPassphraseLength], char oldPassphrase[maxPassphraseLength]);
	void retryNewPassphrase(Reason reason, char passphrase[maxPassphraseLength], char oldPassphrase[maxPassphraseLength]);
	
	// ask permission to use an item in a database
    struct KeychainChoice {
        bool allowAccess;						// user said "yes"
        bool continueGrantingToCaller;			// user wants calling App added to ACL
		char passphrase[maxPassphraseLength];	// only if requested
    };
    void queryKeychainAccess(const OSXCode *requestor, pid_t requestPid,
        const char *database, const char *itemName, AclAuthorization action,
		bool needPassphrase, KeychainChoice &choice);
	void retryQueryKeychainAccess (Reason reason, KeychainChoice &choice);
    
	// one-shot code-identity confirmation query
	void queryCodeIdentity(const OSXCode *requestor, pid_t requestPid,
		const char *aclPath, KeychainChoice &choice);
        
    // generic old passphrase query
    void queryOldGenericPassphrase(const OSXCode *requestor, pid_t requestPid,
        const char *prompt,
        KeychainBox &addToKeychain, char passphrase[maxPassphraseLength]);
    void retryOldGenericPassphrase(Reason reason,
        bool &addToKeychain, char passphrase[maxPassphraseLength]);
        
    // generic new passphrase query
    void queryNewGenericPassphrase(const OSXCode *requestor, pid_t requestPid,
        const char *prompt, Reason reason,
        KeychainBox &addToKeychain, char passphrase[maxPassphraseLength]);
    void retryNewGenericPassphrase(Reason reason,
        bool &addToKeychain, char passphrase[maxPassphraseLength]);
	
	// authenticate a user for the purpose of authorization
	bool authorizationAuthenticate(const OSXCode *requestor, pid_t requestPid,
		const char *neededGroup, const char *candidateUser,
                char username[maxUsernameLength], char passphrase[maxPassphraseLength]);
	bool retryAuthorizationAuthenticate(Reason reason,
                char username[maxUsernameLength], char passphrase[maxPassphraseLength]);

    bool invokeMechanism(const string &inPluginId, const string &inMechanismId, const AuthValueVector &inArguments, AuthItemSet &inHints,  AuthItemSet &inContext, AuthorizationResult *outResult);

    void terminateAgent();
    
	// Cancel a pending client call in another thread by sending a cancel message.
	// This call (only) may be made from another thread.
	void cancel();
		
private:
	// used by client call wrappers to receive IPC return-status
	OSStatus status;

private:
    Port mServerPort;
    Port mClientPort;
	bool mActive;
    uid_t desktopUid;
    Bootstrap mClientBootstrap;
	bool mKeepAlive;

	enum Stage {
		mainStage,				// in between requests
		unlockStage,			// in unlock sub-protocol
		newPassphraseStage,		// in get-new-passphrase sub-protocol
        newGenericPassphraseStage, // in get-new-generic-passphrase sub-protocol
        oldGenericPassphraseStage, // in get-old-generic-passphrase sub-protocol
		authorizeStage,			// in authorize-by-group-membership sub-protocol
        queryKeychainAccessStage,
       invokeMechanismStage   // in invoke mechanism sub-protocol
	} stage;
	Port mStagePort;
	string mAgentName;

	void locateDesktop();
	void establishServer();
	void check(kern_return_t error);
	void unstage();
	
private:
	static const int cancelMessagePseudoID = 1200;
};

}; // end namespace SecurityAgent

} // end namespace Security

#endif //C++ only

#endif //_H_SECURITYAGENTCLIENT
