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
// server - the actual Server object
//
#ifndef _H_SERVER
#define _H_SERVER

#include "securityserver.h"
#include "structure.h"
#include <security_utilities/machserver.h>
#include <security_utilities/powerwatch.h>
#include <security_cdsa_client/cssmclient.h>
#include <security_cdsa_client/cspclient.h>
#include <security_cdsa_client/osxsigner.h>
#include <security_utilities/devrandom.h>
#include <security_cdsa_utilities/uniformrandom.h>
#include "codesigdb.h"
#include "connection.h"
#include "key.h"
#include "database.h"
#include "localdatabase.h"
#include "kcdatabase.h"
#include "authority.h"
#include "AuthorizationEngine.h"
#include <map>

#define EQUIVALENCEDBPATH "/var/db/CodeEquivalenceDatabase"

//
// The authority itself. You will usually only have one of these.
//
class Authority : public Authorization::Engine {
public:
	Authority(const char *configFile);
	~Authority();
};

class Server : public PerGlobal,
			   public MachPlusPlus::MachServer,
               public UniformRandomBlobs<DevRandomGenerator> {
public:
	Server(Authority &myAuthority, CodeSignatures &signatures, const char *bootstrapName);
	~Server();
		
    // run the server until it shuts down
	void run();
	
    //
    // Retrieve pieces of the Server's object web.
    // These are all static methods that use the active() Server of this thread.
    //
	static Server &active() { return safer_cast<Server &>(MachServer::active()); }
	static const char *bootstrapName() { return active().mBootstrapName.c_str(); }
	
	static Connection &connection(mach_port_t replyPort);
	static Connection &connection(bool tolerant = false);
	static void requestComplete();
	
	static Process &process();
	static Session &session();
	
	static RefPointer<Key> key(KeyHandle key);
    static RefPointer<Key> optionalKey(KeyHandle k) { return (k == noKey) ? NULL : key(k); }
	static RefPointer<Database> database(DbHandle db);
	static RefPointer<KeychainDatabase> keychain(DbHandle db);
	static RefPointer<Database> optionalDatabase(DbHandle db);
    static Authority &authority() { return active().mAuthority; }
	static CodeSignatures &codeSignatures() { return active().mCodeSignatures; }
	static SecurityServerAcl &aclBearer(AclKind kind, CSSM_HANDLE handle);
	static CssmClient::CSP &csp() { return active().getCsp(); }

	void loadCssm();
	
public:
	// set up a new connection
	enum ConnectLevel {
		connectNewSession,
		connectNewProcess,
		connectNewThread
	};
	void setupConnection(ConnectLevel type, Port servicePort, Port replyPort, Port taskPort,
        const security_token_t &securityToken,
		const ClientSetupInfo *info = NULL, const char *executablePath = NULL);
		
	void endConnection(Port replyPort);
	
	static void releaseWhenDone(Allocator &alloc, void *memory)
	{ MachServer::active().releaseWhenDone(alloc, memory); }
	static void releaseWhenDone(void *memory)
	{ releaseWhenDone(Allocator::standard(), memory); }
    
protected:
    // implementation methods of MachServer
	boolean_t handle(mach_msg_header_t *in, mach_msg_header_t *out);
	void notifyDeadName(Port port);
	void notifyNoSenders(Port port, mach_port_mscount_t);
    
private:
    class SleepWatcher : public MachPlusPlus::PortPowerWatcher {
    public:
        void systemWillSleep();
    };
    SleepWatcher sleepWatcher;
	
private:
	// mach bootstrap registration name
	std::string mBootstrapName;
	
	// connection map (by client reply port)
	PortMap<Connection> mConnections;

	// process map (by process task port)
	PortMap<Process> mProcesses;
	
	// Current connection, if any (per thread).
	// Set as a side effect of calling connection(mach_port_t)
	// and returned by connection(bool).
	ThreadNexus<RefPointer<Connection> > mCurrentConnection;
	
    // CSSM components
    CssmClient::Cssm mCssm;				// CSSM instance
    CssmClient::Module mCSPModule;		// CSP module
	CssmClient::CSP mCSP;				// CSP attachment
    CssmClient::CSP &getCsp();			// lazily initialize, then return CSP attachment
    
	Authority &mAuthority;
	CodeSignatures &mCodeSignatures;
};

#endif //_H_SERVER
