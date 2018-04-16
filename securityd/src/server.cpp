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
// server - the actual SecurityServer server object
//
#include <securityd_client/ucsp.h> // knowledge of mig message sizes
#include "server.h"
#include "session.h"
#include "acls.h"
#include "notifications.h"
#include <mach/mach_error.h>

using namespace MachPlusPlus;

//
// Construct an Authority
//
Authority::Authority(const char *configFile)
: Authorization::Engine(configFile)
{
}

Authority::~Authority()
{
}

//
// Construct the server object
//
Server::Server(Authority &authority, CodeSignatures &signatures, const char *bootstrapName)
  : MachServer(bootstrapName),
    mBootstrapName(bootstrapName),
    mCSPModule(gGuidAppleCSP, mCssm), mCSP(mCSPModule),
    mAuthority(authority),
	mCodeSignatures(signatures)
{
    // engage the subsidiary port handler for sleep notifications
    add(sleepWatcher);
}


//
// Clean up the server object
//
Server::~Server()
{
    //@@@ more later
}


//
// Locate a connection by reply port and make it the current connection
// of this thread. The connection will be marked busy, and can be accessed
// by calling Server::connection() [no argument] until it is released by
// calling Connection::endWork().
//
Connection &Server::connection(mach_port_t port)
{
	Server &server = active();
	StLock<Mutex> _(server);
	Connection *conn = server.mConnections.get(port, CSSM_ERRCODE_INVALID_CONTEXT_HANDLE);
	active().mCurrentConnection() = conn;
	conn->beginWork();
	return *conn;
}

Connection &Server::connection(bool tolerant)
{
	Connection *conn = active().mCurrentConnection();
	assert(conn);	// have to have one
	if (!tolerant)
		conn->checkWork();
	return *conn;
}

void Server::requestComplete()
{
	// note: there may not be an active connection if connection setup failed
	if (RefPointer<Connection> &conn = active().mCurrentConnection()) {
		conn->endWork();
		conn = NULL;
	}
	IFDUMPING("state", NodeCore::dumpAll());
}


//
// Shorthand for "current" process and session.
// This is the process and session for the current connection.
//
Process &Server::process()
{
	return connection().process();
}

Session &Server::session()
{
	return connection().process().session();
}

RefPointer<Key> Server::key(KeyHandle key)
{
	return HandleObject::findRef<Key>(key, CSSMERR_CSP_INVALID_KEY_REFERENCE);
}

RefPointer<Database> Server::database(DbHandle db)
{
	RefPointer<Database> database = 
		HandleObject::findRef<Database>(db, CSSMERR_DL_INVALID_DB_HANDLE);
	if (database->process() != process())
		CssmError::throwMe(CSSMERR_DL_INVALID_DB_HANDLE);
	return database;
}

RefPointer<KeychainDatabase> Server::keychain(DbHandle db)
{
	RefPointer<KeychainDatabase> keychain =
		HandleObject::findRef<KeychainDatabase>(db, CSSMERR_DL_INVALID_DB_HANDLE);
	if (keychain->process() != process())
		CssmError::throwMe(CSSMERR_DL_INVALID_DB_HANDLE);
	return keychain;
}

RefPointer<Database> Server::optionalDatabase(DbHandle db)
{
	if (db == noDb)
		return &process().localStore();
	else
		return database(db);
}


//
// Locate an ACL bearer (database or key) by handle
//
SecurityServerAcl &Server::aclBearer(AclKind kind, CSSM_HANDLE handle)
{
	SecurityServerAcl &bearer =
		HandleObject::find<SecurityServerAcl>(handle, CSSMERR_CSSM_INVALID_ADDIN_HANDLE);
	if (kind != bearer.kind())
		CssmError::throwMe(CSSMERR_CSSM_INVALID_HANDLE_USAGE);
	return bearer;
}


//
// Run the server. This will not return until the server is forced to exit.
//
void Server::run()
{
	MachServer::run(0x10000,
        MACH_RCV_TRAILER_TYPE(MACH_MSG_TRAILER_FORMAT_0) |
        MACH_RCV_TRAILER_ELEMENTS(MACH_RCV_TRAILER_SENDER));
}


//
// The primary server run-loop function.
// Invokes the MIG-generated main dispatch function (ucsp_server).
// For debug builds, look up request names in a MIG-generated table
// for better debug-log messages.
//
boolean_t ucsp_server(mach_msg_header_t *, mach_msg_header_t *);

#if defined(NDEBUG)

boolean_t Server::handle(mach_msg_header_t *in, mach_msg_header_t *out)
{
	return ucsp_server(in, out);
}

#else //NDEBUG

static const struct IPCName { const char *name; int ipc; } ipcNames[] =
    { subsystem_to_name_map_ucsp };	// macro generated by MIG, from ucsp.h

boolean_t Server::handle(mach_msg_header_t *in, mach_msg_header_t *out)
{
    const int first = ipcNames[0].ipc;
    const char *name = (in->msgh_id >= first && in->msgh_id < first + ucsp_MSG_COUNT) ?
		ipcNames[in->msgh_id - first].name : "OUT OF BOUNDS";
    secdebug("SSreq", "begin %s (%d)", name, in->msgh_id);
	boolean_t result = ucsp_server(in, out);
    secdebug("SSreq", "end %s (%d)", name, in->msgh_id);
    return result;
}

#endif //NDEBUG


//
// Set up a new Connection. This establishes the environment (process et al) as needed
// and registers a properly initialized Connection object to run with.
// Type indicates how "deep" we need to initialize (new session, process, or connection).
// Everything at and below that level is constructed. This is straight-forward except
// in the case of session re-initialization (see below).
//
void Server::setupConnection(ConnectLevel type, Port servicePort, Port replyPort, Port taskPort,
    const security_token_t &securityToken, const ClientSetupInfo *info, const char *identity)
{
	// first, make or find the process based on task port
	StLock<Mutex> _(*this);
	RefPointer<Process> &proc = mProcesses[taskPort];
	if (type == connectNewSession && proc) {
		// The client has talked to us before and now wants to create a new session.
		proc->changeSession(servicePort);
	}
	if (!proc) {
		if (type == connectNewThread)	// client error (or attack)
			CssmError::throwMe(CSSM_ERRCODE_INTERNAL_ERROR);
		assert(info && identity);
		proc = new Process(servicePort, taskPort, info, identity,
			securityToken.val[0], securityToken.val[1]);
		notifyIfDead(taskPort);
	}

	// now, establish a connection and register it in the server
	Connection *connection = new Connection(*proc, replyPort);
	if (mConnections.contains(replyPort))   // malicious re-entry attempt?
		CssmError::throwMe(CSSM_ERRCODE_INTERNAL_ERROR);	//@@@ error code? (client error)
	mConnections[replyPort] = connection;
	notifyIfDead(replyPort);
}


//
// Synchronously end a Connection.
// This is due to a request from the client, so no thread races are possible.
// In practice, this is optional since the DPN for the client thread reply port
// will destroy the connection anyway when the thread dies.
//
void Server::endConnection(Port replyPort)
{
	StLock<Mutex> _(*this);
	PortMap<Connection>::iterator it = mConnections.find(replyPort);
	assert(it != mConnections.end());
	it->second->terminate();
	mConnections.erase(it);
}


//
// Handling dead-port notifications.
// This receives DPNs for all kinds of ports we're interested in.
//
void Server::notifyDeadName(Port port)
{
	StLock<Mutex> _(*this);
	secdebug("SSports", "port %d is dead", port.port());
    
    // is it a connection?
    PortMap<Connection>::iterator conIt = mConnections.find(port);
    if (conIt != mConnections.end()) {
		conIt->second->abort();
		mConnections.erase(conIt);
        return;
    }
    
    // is it a process?
    PortMap<Process>::iterator procIt = mProcesses.find(port);
    if (procIt != mProcesses.end()) {
		procIt->second->kill();
		mProcesses.erase(procIt);
        return;
    }
    
    // is it a notification client?
    if (Listener::remove(port))
        return;
    
	secdebug("server", "spurious dead port notification for port %d", port.port());
}


//
// Handling no-senders notifications.
// This is currently only used for (subsidiary) service ports
//
void Server::notifyNoSenders(Port port, mach_port_mscount_t)
{
	secdebug("SSports", "port %d no senders", port.port());
	Session::destroy(port);
}


//
// Notifier for system sleep events
//
void Server::SleepWatcher::systemWillSleep()
{
    secdebug("SS", "sleep notification received");
    Session::processSystemSleep();
}


//
// Return the primary Cryptographic Service Provider.
// This will be lazily loaded when it is first requested.
//
CssmClient::CSP &Server::getCsp()
{
    if (!mCssm->isActive())
		loadCssm();
    return mCSP;
}


//
// Initialize the CSSM/MDS subsystem.
// This is thread-safe and can be done lazily.
//
static void initMds();

void Server::loadCssm()
{
	if (!mCssm->isActive()) {
		StLock<Mutex> _(*this);
		if (!mCssm->isActive()) {
			try {
				initMds();
			} catch (const CssmError &error) {
				switch (error.osStatus()) {
				case CSSMERR_DL_MDS_ERROR:
				case CSSMERR_DL_OS_ACCESS_DENIED:
					secdebug("SS", "MDS initialization failed; continuing");
					Syslog::warning("MDS initialization failed; continuing");
					break;
				default:
					throw;
				}
			}
			secdebug("SS", "CSSM initializing");
			mCssm->init();
			mCSP->attach();
			IFDEBUG(char guids[Guid::stringRepLength+1]);
			secdebug("SS", "CSSM ready with CSP %s", mCSP->guid().toString(guids));
		}
	}
}

#include <Security/mds.h>

static void initMds()
{
	secdebug("SS", "MDS initializing");
	CssmAllocatorMemoryFunctions memory(Allocator::standard());
	MDS_FUNCS functions;
	MDS_HANDLE handle;
	CssmError::check(MDS_Initialize(NULL, &memory, &functions, &handle));
	CssmError::check(MDS_Install(handle));
	CssmError::check(MDS_Terminate(handle));
}
