/*
 *  agentclient.h
 *  SecurityAgent
 *
 *  Copyright (c) 2002 Apple Computer, Inc.. All rights reserved.
 *
 */

#ifndef _H_AGENTCLIENT
#define _H_AGENTCLIENT

#include <Security/Authorization.h>
#include <Security/AuthorizationPlugin.h>
#include <Security/AuthorizationTags.h>
#include <Security/AuthorizationTagsPriv.h>

#include <security_agent_client/sa_types.h>

#if defined(__cplusplus)

#include <string>
#include <security_utilities/mach++.h>
#include <security_cdsa_utilities/AuthorizationData.h>
#include <security_cdsa_client/osxsigning.h>

namespace SecurityAgent {
#endif /* __cplusplus__ */

#define kMaximumAuthorizationTries 3
#define maxPassphraseLength 1024
    
//
// Unified reason codes transmitted to SecurityAgent (and internationalized there)
//
enum Reason {
    noReason = 0,                   // no reason (not used, used as a NULL)
    unknownReason,                  // something else (catch-all internal error)

    // reasons for asking for a new passphrase
    newDatabase = 11,               // need passphrase for a new database
    changePassphrase,               // changing passphrase for existing database

    // reasons for retrying an unlock query
    invalidPassphrase = 21,         // passphrase was wrong

    // reasons for retrying a new passphrase query
    passphraseIsNull = 31,          // empty passphrase
    passphraseTooSimple,            // passphrase is not complex enough
    passphraseRepeated,             // passphrase was used before (must use new one)
    passphraseUnacceptable,         // passphrase unacceptable for some other reason
    oldPassphraseWrong,             // the old passphrase given is wrong

    // reasons for retrying an authorization query
    userNotInGroup = 41,            // authenticated user not in needed group
    unacceptableUser,               // authenticated user unacceptable for some other reason

    // reasons for canceling a staged query
    tooManyTries = 61,              // too many failed attempts to get it right
    noLongerNeeded,                 // the queried item is no longer needed
    keychainAddFailed,              // the requested itemed couldn't be added to the keychain
    generalErrorCancel              // something went wrong so we have to give up now
};

typedef enum {
	tool = 'TOOL',
	bundle = 'BNDL',
	unknown = 'UNKN'
} RequestorType;

#if defined(__cplusplus)

using MachPlusPlus::Port;
using MachPlusPlus::Bootstrap;
using MachPlusPlus::ReceivePort;
using MachPlusPlus::Message;
using Authorization::AuthItemSet;

class Clients;

class Client
{
friend class Clients;

enum MessageType { requestInterruptMessage, didDeactivateMessage, reportErrorMessage };

public:
	Client();
	Client(uid_t clientUID, Bootstrap clientBootstrap, const char *name);

	virtual ~Client();

    static AuthItemSet clientHints(CodeSigning::OSXCode *clientCode, pid_t clientPid, uid_t clientUid);
	
protected:
	void establishServer();
	
public:
// client requests
	bool isActive() { return mActive; }
	OSStatus activate();

	OSStatus create(const char *pluginId, const char *mechanismId, const SessionId inSessionId);
    OSStatus invoke(const Authorization::AuthValueVector& inArguments, const Authorization::AuthItemSet& inHints, const Authorization::AuthItemSet& inContext);
	OSStatus deactivate();
	OSStatus destroy();
	OSStatus terminate();
    void receive();
	
	void didCreate(const mach_port_t inStagePort);
    void setResult(const AuthorizationResult inResult, const AuthorizationItemSet *inHints, const AuthorizationItemSet *inContext);
	void requestInterrupt(); // setMessageType(requestInterrupt);
	void didDeactivate(); // setMessageType(didDeactivate);

	void setError(const OSStatus inMechanismError); // setMessageType(reportError); setError(mechanismError);
    OSStatus getError();
    AuthorizationResult result() { return mResult; }

protected:
	void setMessageType(const MessageType inMessageType);
	// allow didCreate to set stagePort 
	void setStagePort(const mach_port_t inStagePort) { mStagePort = Port(inStagePort); }
	// allow server routines to use request port to find instance 

	// @@@ implement lessThan operator for set in terms of instance	

protected:
	void setup();
	void teardown() throw();

	// agent default port
    Port mServerPort;
	// session port on agent for one mechanism to talk to
	Port mStagePort;
	// session port on client for sending to agent
    Port mClientPort;
	bool mActive;

	MessageType mMessageType;
    //uid_t mUid;
    //gid_t mGid;
    //mach_port_t mSessionBootstrap;
	//bool mKeepAlive;
    
    OSStatus mErrorState;

    AuthorizationResult mResult;
    AuthItemSet mHints;
    AuthItemSet mContext;
	
	enum PluginState {
		init,
		created,
		current,
		deactivating,
		active,
		interrupting,
		dead
	};
	PluginState mState;
	void setState(PluginState mState);
	int state();

public:
	mach_port_t instance() const { return mClientPort; }
//	bool operator == (const Client &other) const { return this->instance() == other.instance(); }
	bool operator < (const Client &other) const { return this->instance() < other.instance(); }

    AuthItemSet &hints() { return mHints; }
    AuthItemSet &context() { return mContext; }

public:
		void check(mach_msg_return_t returnCode);
        void checkResult();

private:

	uid_t mDesktopUid;
	bool mKeepAlive;
	string mAgentName;
    Bootstrap mClientBootstrap;
	
};

class Clients
{
friend class Client;

protected:
	set<Client*> mClients;
public:
    Clients() {}
	void create(); // create an agentclient
	void insert(Client *agent) { StLock<Mutex> _(mLock); mClients.insert(agent); }
	void remove(Client *agent) { StLock<Mutex> _(mLock); mClients.erase(agent); }
	Client &find(const mach_port_t instance) const;
	bool compare(const Client * client, mach_port_t instance);

	mutable Mutex mLock;
	static ModuleNexus<Clients> gClients;
};

} // end namespace Authorization

#endif /* __cplusplus__ */

#endif /* _H_AGENTCLIENT */

