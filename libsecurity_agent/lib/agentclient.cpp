/*
 *  agentclient.cpp
 *  SecurityAgent
 *
 *  Copyright (c) 2002 Apple Computer, Inc.. All rights reserved.
 *
 */

#include "agentclient.h"

#include <stdio.h>

/*
For now all the calls into agentclient will be synchronous, with timeouts

On a timeout, we will return control to the client, but we really need to send the appropriate abort right there and then, otherwise they'll need to call the same method again to check that the reply still isn't there.

If we receive a reply that is not confirming attempts to abort, we'll process these and return them to the caller.

Alternatively, there can be an answer that isn't the answer we expected: setError, where the server aborts the transaction.

We can't support interrupt() with a synchronous interface unless we add some notification that let's the client know that the "server" is dead
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <grp.h>

#include <security_agent_server/sa_reply.h> // for size of replies
#include <security_agent_client/sa_request.h>


#include <security_utilities/mach++.h>
#include <security_cdsa_utilities/walkers.h>
#include <security_cdsa_utilities/cssmwalkers.h>
#include <security_cdsa_utilities/AuthorizationWalkers.h>
#include <security_cdsa_utilities/AuthorizationData.h>

using Security::MachPlusPlus::VMGuard;

using LowLevelMemoryUtilities::increment;
using LowLevelMemoryUtilities::difference;
using Security::DataWalkers::walk;

using Authorization::AuthItemSet;
using Authorization::AuthItemRef;
using Authorization::AuthValueOverlay;

namespace SecurityAgent {
    
class CheckingReconstituteWalker {
public:
    CheckingReconstituteWalker(void *ptr, void *base, size_t size)
    : mBase(base), mLimit(increment(base, size)), mOffset(difference(ptr, base)) { }
	
	template <class T>
	void operator () (T &obj, size_t size = sizeof(T))
{ }
	
    template <class T>
    void operator () (T * &addr, size_t size = sizeof(T))
{
		blob(addr, size);
}

template <class T>
void blob(T * &addr, size_t size)
{
	DEBUGWALK("checkreconst:*");
	if (addr) {
		if (addr < mBase || increment(addr, size) > mLimit)
			MacOSError::throwMe(errAuthorizationInternal);
		addr = increment<T>(addr, mOffset);
	}
}

static const bool needsRelinking = true;
static const bool needsSize = false;

private:
void *mBase;			// old base address
void *mLimit;			// old last byte address + 1
off_t mOffset;			// relocation offset
};

template <class T>
void relocate(T *obj, T *base, size_t size)
{
    if (obj) {
        CheckingReconstituteWalker w(obj, base, size);
        walk(w, base);
    }
}

void Client::check(mach_msg_return_t returnCode)
{
	// first check the Mach IPC return code
	switch (returnCode) {
		case MACH_MSG_SUCCESS:				// peachy
			break;
		case MIG_SERVER_DIED:			// explicit can't-send-it's-dead
			CssmError::throwMe(CSSM_ERRCODE_NO_USER_INTERACTION);
		default:						// some random Mach error
			MachPlusPlus::Error::throwMe(returnCode);
	}
}

void Client::checkResult()
{
        // now check the OSStatus return from the server side
        switch (result()) {
            case kAuthorizationResultAllow: return;
            case kAuthorizationResultDeny:
            case kAuthorizationResultUserCanceled: CssmError::throwMe(CSSM_ERRCODE_USER_CANCELED);
            default: MacOSError::throwMe(errAuthorizationInternal);
        }
}



extern "C" boolean_t secagentreply_server(mach_msg_header_t *, mach_msg_header_t *);
//extern "C" struct sa_reply_server_secagentreply_subsystem sa_reply_server_secagentreply_subsystem;

#pragma mark administrative operations

Client::Client() : 
	mActive(false), mState(init), mDesktopUid(0), mKeepAlive(false), mAgentName("com.apple.SecurityAgent")
{
}

Client::Client(uid_t clientUID, Bootstrap clientBootstrap, const char *name) : 
	mActive(false), mState(init), mDesktopUid(clientUID), mKeepAlive(false), mAgentName(name), 
	mClientBootstrap(clientBootstrap)
{
}

//
// Get the port for the SecurityAgent.
// Start it if necessary (and possible). Throw an exception if we can't get to it.
// Sets mServerPort on success.
//
void
Client::establishServer()
{
    // if the server is already running, we're done
	if (mServerPort = mClientBootstrap.lookupOptional(mAgentName.c_str()))
		return;
    
#if defined(AGENTNAME) && defined(AGENTPATH)
    // switch the bootstrap port to that of the logged-in user
	
    Security::MachPlusPlus::StBootstrap bootSaver(mClientBootstrap);
	
    // try to start the agent
    switch (pid_t pid = fork()) {
		case 0:		// child
		{
			// Setup the environment for the SecurityAgent
			unsetenv("USER");
			unsetenv("LOGNAME");
			unsetenv("HOME");
			
			// tell agent which name to register
			setenv("AGENTNAME", mAgentName.c_str(), 1);
			
			if (mDesktopUid) // if the user is running as root, or we're not told what uid to use, we stick with what we are
			{
				struct group *grent = getgrnam("nobody");
				gid_t desktopGid = grent ? grent->gr_gid : unsigned(-2);	//@@@ questionable
				endgrent();
				secdebug("SAclnt", "setgid(%d)", desktopGid);
				setgid(desktopGid);	// switch to login-user gid
				secdebug("SAclnt", "setuid(%d)", mDesktopUid);
				// Must be setuid and not seteuid since we do not want the agent to be able
				// to call seteuid(0) successfully.
				setuid(mDesktopUid);	// switch to login-user uid
			}
			// close down any files that might have been open at this point
			int maxDescriptors = getdtablesize ();
			int i;
			
			for (i = 3; i < maxDescriptors; ++i)
			{
				// XXX/cs should dup(2) to /dev/null
				close (i);
			}
			
			// construct path to SecurityAgent
			char agentExecutable[PATH_MAX + 1];
			const char *path = getenv("SECURITYAGENT");
			if (!path)
				path = AGENTPATH;
			snprintf(agentExecutable, sizeof(agentExecutable), "%s/Contents/MacOS/" AGENTNAME, path);
			secdebug("SAclnt", "execl(%s)", agentExecutable);
			execl(agentExecutable, agentExecutable, NULL);
			secdebug("SAclnt", "execl of SecurityAgent failed, errno=%d", errno);
			
			// Unconditional suicide follows.
			// See comments below on why we can't use abort()
#if 1
			_exit(1);
#else
			// NOTE: OS X abort() is implemented as kill(getuid()), which fails
			// for a setuid-root process that has setuid'd. Go back to root to die...
			setuid(0);
			abort();
#endif
		}
		case -1:	// error (in parent)
			UnixError::throwMe();
		default:	// parent
        {
			static const int timeout = 300;
			
            secdebug("SAclnt", "Starting security agent (%d seconds timeout)", timeout);
			struct timespec rqtp;
			memset(&rqtp, 0, sizeof(rqtp));
			rqtp.tv_nsec = 100000000; /* 10^8 nanaseconds = 1/10th of a second */
            for (int n = timeout; n > 0; nanosleep(&rqtp, NULL), n--) {
                if (mServerPort = mClientBootstrap.lookupOptional(mAgentName.c_str()))
                    break;
                int status;
                switch (IFDEBUG(pid_t rc =) waitpid(pid, &status, WNOHANG)) {
					case 0:	// child still running
						continue;
					case -1:	// error
						switch (errno) {
							case EINTR:
							case EAGAIN:	// transient
								continue;
							case ECHILD:	// no such child (dead; already reaped elsewhere)
								secdebug("SAclnt", "child is dead (reaped elsewhere)");
								CssmError::throwMe(CSSM_ERRCODE_NO_USER_INTERACTION);
							default:
								secdebug("SAclnt", "waitpid failed: errno=%d", errno);
								UnixError::throwMe();
						}
					default:
						assert(rc == pid);
						secdebug("SAclnt", "child died without claiming the SecurityAgent port");
						CssmError::throwMe(CSSM_ERRCODE_NO_USER_INTERACTION);
                }
            }
			
            if (mServerPort == 0) {		// couldn't contact Security Agent
				secdebug("SAclnt", "Autolaunch failed");
                CssmError::throwMe(CSSM_ERRCODE_NO_USER_INTERACTION);
			}
            secdebug("SAclnt", "SecurityAgent located");
            return;
        }
    }
#endif
	
    // well, this didn't work. Too bad
	secdebug("SAclnt", "Cannot contact SecurityAgent");
    CssmError::throwMe(CSSM_ERRCODE_NO_USER_INTERACTION);	//@@@ or INTERNAL_ERROR?
}



OSStatus
Client::activate()
{
	if (!mActive) 
	{
		// set mServerPort
		establishServer();
				
		if (!mServerPort)
			MacOSError::throwMe(errAuthorizationInternal);
		
		// create reply port
		mClientPort.allocate(); //implicit MACH_PORT_RIGHT_RECEIVE
		
		// we need the send right for passing it to the agent to talk back
		mClientPort.insertRight(MACH_MSG_TYPE_MAKE_SEND);
		
		// get notified if the server dies (shouldn't happen, but...)
		mServerPort.requestNotify(mClientPort, MACH_NOTIFY_DEAD_NAME, true);
		
		// register with agentclients
		Clients::gClients().insert(this);
		
		// ready
		mActive = true;
	}
	return noErr;
}


Client::~Client()
{
    teardown();
}

void Client::setState(PluginState inState)
{
	// validate state transition: might be more useful where change is requested if that implies anything to interpreting what to do.
	
{
	// Mutex
	mState = inState;
}
}

int Client::state()
{
	return mState;
}

void Client::teardown() throw()
{
	Clients::gClients().remove(this);
	if (mActive) {
		mServerPort.deallocate();
		mStagePort.deallocate();
		mClientPort.destroy();
		mActive = false;
	}
}


AuthItemSet
Client::clientHints(CodeSigning::OSXCode *clientCode, pid_t clientPid, uid_t clientUid)
{
    AuthItemSet clientHints;
	string encodedBundle = clientCode->encode();
	char bundleType = (encodedBundle.c_str())[0]; // yay, no accessor
	SecurityAgent::RequestorType requestorType;
	string bundlePath = clientCode->canonicalPath();
	
	switch(bundleType)
	{
	   case 'b': requestorType = SecurityAgent::bundle; break;
	   case 't': requestorType = SecurityAgent::tool; break;
	   default: requestorType = SecurityAgent::unknown; break;
	}
	
	clientHints.insert(AuthItemRef(AGENT_HINT_CLIENT_TYPE, AuthValueOverlay(sizeof(requestorType), &requestorType)));
	clientHints.insert(AuthItemRef(AGENT_HINT_CLIENT_PATH, AuthValueOverlay(bundlePath)));
	clientHints.insert(AuthItemRef(AGENT_HINT_CLIENT_PID, AuthValueOverlay(sizeof(clientPid), &clientPid)));
	clientHints.insert(AuthItemRef(AGENT_HINT_CLIENT_UID, AuthValueOverlay(sizeof(clientUid), &clientUid)));

    return clientHints;
}


#pragma mark request operations


OSStatus Client::create(const char *inPluginId, const char *inMechanismId, const SessionId inSessionId )
{
	activate();
	
	secdebug("agentclient", "asking server at port %d to create %s:%s; replies to %d", mServerPort.port(), inPluginId, inMechanismId, mClientPort.port()); // XXX/cs
	kern_return_t ret = sa_request_client_create(mServerPort, mClientPort, inSessionId, inPluginId, inMechanismId);

	if (ret)
	{
		// XXX/cs any conditions we could handle from here?
		
		return ret;
	}
	
	// wait for message (either didCreate, reportError)
	try {
		receive();
	}
	catch (...)
	{
		return errAuthorizationInternal;
	}
    
    // verify that we got didCreate
	if (state() == created)
		return noErr;
	
    // and not reportError
	if (state() == dead)
		return Client::getError();

	// something we don't deal with
	secdebug("agentclient", "we got an error on create"); // XXX/cs
    return errAuthorizationInternal;
}

// client maintains their own copy of the current data
OSStatus Client::invoke(
	const Authorization::AuthValueVector& inArguments, 
	const Authorization::AuthItemSet& inHints, 
	const Authorization::AuthItemSet& inContext)
{
	if ((state() != created) &&
        (state() != active))
		return errAuthorizationInternal;
	
    AuthorizationValueVector *arguments;
    AuthorizationItemSet *hints, *context;
    size_t argumentSize, hintSize, contextSize;

    inHints.copy(hints, hintSize);
    inContext.copy(context, contextSize);
    inArguments.copy(&arguments, &argumentSize);
        
    check(sa_request_client_invoke(mStagePort.port(), 
                    arguments, argumentSize, arguments, // data, size, offset
                    hints, hintSize, hints,
                    context, contextSize, context));
            
    receive();
    
    switch(state())
    {
        case active: 
            switch(result())
            {
                case kAuthorizationResultUndefined:
                    MacOSError::throwMe(errAuthorizationInternal);
                default: 
                    return noErr;
            }
        case dead: 
            return mErrorState;
    }
    return errAuthorizationInternal;
}

OSStatus
Client::deactivate()
{
	// check state is current
	if (state() != current)
		return errAuthorizationInternal;
	
	secdebug("agentclient", "deactivating mechanism at request port %d", mStagePort.port());

	// tell mechanism to deactivate
	check(sa_request_client_deactivate(mStagePort.port()));
	
	setState(deactivating);
	
	receive(); 
	// didDeactivate?
	// setResult?
	// server died?
	// reportError?
	
	// if failed destroy it
	return noErr;
}

OSStatus
Client::destroy()
{
	// check state is active or created
	if ((state() == current) && deactivate())
		return errAuthorizationInternal;
	
	if (state() == active || state() == created)
	{
		secdebug("agentclient", "destroying mechanism at request port %d", mStagePort.port());
		// tell mechanism to destroy
		check(sa_request_client_destroy(mStagePort.port()));

		setState(dead);
	
		return noErr;
	}
	
	return errAuthorizationInternal;
}

// destroy plugin and don't care for how it feels about that.
// return an error if that didn't work so the entire host can get shot down
OSStatus
Client::terminate()
{
	check(sa_request_client_terminate(mServerPort.port()));
	
    return noErr;
}

void
Client::receive()
{
	try 
	{
		// maximum known message size (variable sized elements are already forced OOL)
		Message in(sizeof(union __ReplyUnion__sa_reply_client_secagentreply_subsystem));
		Message out(sizeof(union __ReplyUnion__sa_reply_client_secagentreply_subsystem));

		in.receive(mClientPort, 0, 0);

        // got the message, now demux it; call secagentreply_server to handle any call
        // this is asynchronous, so no reply message, although not apparent
        if (!secagentreply_server(in, out))
        {
    		// @@@ port death notification? -> (can't talk to user error)

            // message not recognized, chances of the right message coming back are zero
            MacOSError::throwMe(errAuthorizationInternal);		
        }
    } 
    catch (Security::MachPlusPlus::Error &e) 
    {
        secdebug("agentclient", "interpret error %ul", e.error);
		check(e.error);
    }
    catch (...)
    {
        MacOSError::throwMe(errAuthorizationInternal);
    }
        
}

#pragma mark result operations

void Client::setResult(const AuthorizationResult inResult, const AuthorizationItemSet *inHints, const AuthorizationItemSet *inContext)
{
    // construct AuthItemSet for hints and context (deep copy - previous contents are released)
	mHints = (*inHints);
	mContext = (*inContext);
    mResult = inResult;
    setState(active);
}

void Client::setError(const OSStatus inMechanismError)
{
	setState(dead);

    mErrorState = inMechanismError; 
    // setMessageType(reportError); setError(mechanismError);
}

OSStatus Client::getError()
{
    return mErrorState;
}

void Client::requestInterrupt()
{
	// check state for active
	// change state
	setState(interrupting);
	// hint at engine that it's time to stop
    
    // XXX add a callback to report this
    
	// if successfully stopped
	setState(created);
	// if not able to restart, abort the evaluation - we'll be expecting the destroy
}

void Client::didDeactivate()
{
	// check state for deactivating
	// change state
	setState(active);
}

void Client::didCreate(const mach_port_t inStagePort)
{
	setStagePort(inStagePort);
	setState(created);
}

#pragma mark client instances

ModuleNexus<Clients> Clients::gClients;

bool
Clients::compare(const Client * client, mach_port_t instance)
{
    if (client->instance() == instance) return true;
    return false;
}

// throw so the agent client operation is aborted
Client&
Clients::find(mach_port_t instanceReplyPort) const
{
	StLock<Mutex> _(mLock);
    for (set<Client*>::const_iterator foundClient = mClients.begin(); 
        foundClient != mClients.end();
        foundClient++)
        {
            Client *client = *foundClient;
            if (client->instance() == instanceReplyPort)
                return *client;
        }
        
    // can't be receiving for a client we didn't create
	MacOSError::throwMe(errAuthorizationInternal);
}


#pragma mark demux requests replies
// external C symbols for the mig message handling code to call into
extern "C" {

#define COPY_IN(type,name)	type *name, mach_msg_type_number_t name##Length, type *name##Base
//#define COPY_OUT(type,name)	type **name, mach_msg_type_number_t *name##Length, type **name##Base

// callbacks that key off instanceReplyPort to find the right agentclient instance
// to deliver the message to.

// they make the data readable to the receiver (relocate internal references)
	
	kern_return_t sa_reply_server_didCreate(mach_port_t instanceReplyPort, mach_port_t instanceRequestPort)
	{
		secdebug("agentclient", "got didCreate at port %u; requests go to port %u", instanceReplyPort, instanceRequestPort);
		Clients::gClients().find(instanceReplyPort).didCreate(instanceRequestPort);
		return KERN_SUCCESS;
	}
	
	kern_return_t sa_reply_server_setResult(mach_port_t instanceReplyPort, AuthorizationResult result,
        COPY_IN(AuthorizationItemSet,inHints) ,
        COPY_IN(AuthorizationItemSet,inContext) )
	{

		VMGuard _(inHints, inHintsLength);
		VMGuard _2(inContext, inContextLength);

		secdebug("agentclient", "got setResult at port %u; result %ld", instanceReplyPort, result);

		// relink internal references according to current place in memory
		try { relocate(inHints, inHintsBase, inHintsLength); }
		catch (MacOSError &e) {	return e.osStatus(); }
		catch (...) { return errAuthorizationInternal; }

		try { relocate(inContext, inContextBase, inContextLength); }
		catch (MacOSError &e) {	return e.osStatus(); }
		catch (...) { return errAuthorizationInternal; }

		Clients::gClients().find(instanceReplyPort).setResult(result, inHints, inContext);
        		
		return KERN_SUCCESS;
	}
	
	kern_return_t sa_reply_server_requestInterrupt(mach_port_t instanceReplyPort)
	{
		secdebug("agentclient", "got requestInterrupt at port %u", instanceReplyPort);
		Clients::gClients().find(instanceReplyPort).requestInterrupt();
		return KERN_SUCCESS;
	}
	
	kern_return_t sa_reply_server_didDeactivate(mach_port_t instanceReplyPort)
	{
		secdebug("agentclient", "got didDeactivate at port %u", instanceReplyPort);
		Clients::gClients().find(instanceReplyPort).didDeactivate();
		return KERN_SUCCESS;
	}
	
	kern_return_t sa_reply_server_reportError(mach_port_t instanceReplyPort, OSStatus status)
	{
		secdebug("agentclient", "got reportError at port %u; error is %ld", instanceReplyPort, status);
		Clients::gClients().find(instanceReplyPort).setError(status);
		return KERN_SUCCESS;
	}

} /* end extern "C" */

} /* end namesapce Authorization */

