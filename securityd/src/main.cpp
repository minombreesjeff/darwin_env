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
// securityd - Apple security services daemon.
//
#include <securityd_client/ucsp.h>

#include "securityserver.h"
#include "server.h"
#include "entropy.h"

#include <security_utilities/daemon.h>
#include <security_cdsa_client/osxsigner.h>
#include "authority.h"
#include "session.h"
#include "pcscmonitor.h"

#include <unistd.h>
#include <security_utilities/machserver.h>

#include <sys/types.h>
#include <signal.h>

#include <security_utilities/ktracecodes.h>

// #define PERFORMANCE_MEASUREMENT 1

#ifdef PERFORMANCE_MEASUREMENT
#include <mach/mach_time.h>
#endif

// ACL subject types (their makers are instantiated here)
#include <security_cdsa_utilities/acl_any.h>
#include <security_cdsa_utilities/acl_password.h>
#include <security_cdsa_utilities/acl_protectedpw.h>
#include <security_cdsa_utilities/acl_threshold.h>
#include <security_cdsa_utilities/acl_codesigning.h>
#include <security_cdsa_utilities/acl_process.h>
#include <security_cdsa_utilities/acl_comment.h>
#include <syslog.h>
#include "acl_keychain.h"


namespace Security
{

//
// Program options (set by argument scan and environment)
//
uint32 debugMode = 0;
const char *bootstrapName = NULL;

} // end namespace Security


//
// Local functions of the main program driver
//
static void usage(const char *me);
static void handleSIGCHLD(int);
static void handleSIGOther(int);
IFDEBUG(static void handleSIGdebug(int));

static Port gMainServerPort;

//
// Main driver
//
int main(int argc, char *argv[])
{
	#ifdef PERFORMANCE_MEASUREMENT
	// needed for automated timing of securityd startup
	uint64_t startTime = mach_absolute_time ();
	#endif
	
    Debug::trace (kSecTraceSecurityServerStart);
    
	// program arguments (preset to defaults)
	bool doFork = false;
	bool forceCssmInit = false;
	bool reExecute = false;
	int workerTimeout = 0;
	int maxThreads = 0;
	const char *authorizationConfig = "/etc/authorization";
    const char *entropyFile = "/var/db/SystemEntropyCache";
	const char *equivDbFile = EQUIVALENCEDBPATH;

	// parse command line arguments
	extern char *optarg;
	extern int optind;
	int arg;
	while ((arg = getopt(argc, argv, "a:de:E:fN:t:T:X")) != -1) {
		switch (arg) {
		case 'a':
			authorizationConfig = optarg;
			break;
		case 'd':
			debugMode++;
			break;
		case 'e':
			equivDbFile = optarg;
			break;
        case 'E':
            entropyFile = optarg;
            break;
        case 'f':
            forceCssmInit = true;
            break;
		case 'N':
			bootstrapName = optarg;
			break;
		case 't':
			if ((maxThreads = atoi(optarg)) < 0)
				maxThreads = 0;
			break;
		case 'T':
			if ((workerTimeout = atoi(optarg)) < 0)
				workerTimeout = 0;
			break;
		case 'X':
			doFork = true;
			reExecute = true;
			break;
		default:
			usage(argv[0]);
		}
	}
	
	// take no non-option arguments
	if (optind < argc)
		usage(argv[0]);
	
	// figure out the bootstrap name
    IFDEBUG(if (!bootstrapName) bootstrapName = getenv(SECURITYSERVER_BOOTSTRAP_ENV));

	if (!bootstrapName) {
		bootstrapName = SECURITYSERVER_BOOTSTRAP_NAME;
	}
		
	// configure logging first
	if (debugMode) {
		Syslog::open(bootstrapName, LOG_AUTHPRIV, LOG_PERROR);
		Syslog::notice("SecurityServer started in debug mode");
	} else {
		Syslog::open(bootstrapName, LOG_AUTHPRIV, LOG_CONS);
	}
    
    // if we're not running as root in production mode, fail
    // in debug mode, issue a warning
    if (uid_t uid = getuid()) {
#if defined(NDEBUG)
        Syslog::alert("Tried to run securityd as user %d: aborted", uid);
        fprintf(stderr, "You are not allowed to run securityd\n");
        exit(1);
#else
        fprintf(stderr, "securityd is unprivileged; some features may not work.\n");
        secdebug("SS", "Running as user %d (you have been warned)", uid);
#endif //NDEBUG
    }
    
    // turn into a properly diabolical daemon unless debugMode is on
    if (!debugMode) {
		if (!Daemon::incarnate(doFork))
			exit(1);	// can't daemonize
		
		if (reExecute && !Daemon::executeSelf(argv))
			exit(1);	// can't self-execute
	}
	
	// create a code signing engine
	CodeSigning::OSXSigner signer;
	
	// create an Authorization engine
	Authority authority(authorizationConfig);
	
	// establish the ACL machinery
	new AnyAclSubject::Maker();
	new PasswordAclSubject::Maker();
    new ProtectedPasswordAclSubject::Maker();
	new ThresholdAclSubject::Maker();
    new CommentAclSubject::Maker();
 	new ProcessAclSubject::Maker();
	new CodeSignatureAclSubject::Maker(signer);
    new KeychainPromptAclSubject::Maker();
	
	// add a temporary registration for a subject type that went out in 10.2 seed 1
	// this should probably be removed for the next major release >10.2
	new KeychainPromptAclSubject::Maker(CSSM_WORDID__RESERVED_1);
	
	// establish the code equivalents database
	CodeSignatures codeSignatures(equivDbFile);
	
    // create the main server object and register it
 	Server server(authority, codeSignatures, bootstrapName);

    // Remember the primary service port to send signal events to.
    gMainServerPort = server.primaryServicePort();

    // set server configuration from arguments, if specified
	if (workerTimeout)
		server.timeout(workerTimeout);
	if (maxThreads)
		server.maxThreads(maxThreads);
    
	// add the RNG seed timer to it
# if defined(NDEBUG)
    EntropyManager entropy(server, entropyFile);
# else
    if (!getuid()) new EntropyManager(server, entropyFile);
# endif

	// create smartcard monitors to manage external token devices
	PCSCMonitor secureCards(server);
    
    // create the RootSession object (if -d, give it graphics and tty attributes)
    RootSession rootSession(server.primaryServicePort(),
		debugMode ? (sessionHasGraphicAccess | sessionHasTTY) : 0);
        
    // set up signal handlers
    if (signal(SIGCHLD, handleSIGCHLD) == SIG_ERR)
        secdebug("SS", "Cannot handle SIGCHLD: errno=%d", errno);
    if (signal(SIGINT, handleSIGOther) == SIG_ERR)
        secdebug("SS", "Cannot handle SIGINT: errno=%d", errno);
    if (signal(SIGTERM, handleSIGOther) == SIG_ERR)
        secdebug("SS", "Cannot handle SIGTERM: errno=%d", errno);
#if !defined(NDEBUG)
    if (signal(SIGUSR1, handleSIGdebug) == SIG_ERR)
        secdebug("SS", "Cannot handle SIGHUP: errno=%d", errno);
#endif //NDEBUG
    
    // initialize CSSM now if requested
    if (forceCssmInit)
        server.loadCssm();
    
	Syslog::notice("Entering service");
	secdebug("SS", "%s initialized", bootstrapName);

    Debug::trace (kSecTraceSecurityServerInitialized);
    
	#ifdef PERFORMANCE_MEASUREMENT
	// needed for automated timing of securityd startup
	uint64_t endTime = mach_absolute_time ();
	
	// compute how long it took to initialize
	uint64_t elapsedTime = endTime - startTime;
	mach_timebase_info_data_t multiplier;
	mach_timebase_info (&multiplier);
	
	elapsedTime = elapsedTime * multiplier.numer / multiplier.denom;
	
	FILE* f = fopen ("/var/log/startuptime.txt", "a");
	if (f == NULL)
	{
		// probably not running as root.
		f = fopen ("/tmp/startuptime.txt", "a");
	}
	
	fprintf (f, "%lld\n", elapsedTime);
	fclose (f);
	#endif

	server.run();
	
	// fell out of runloop (should not happen)
	Syslog::alert("Aborting");
    return 1;
}


//
// Issue usage message and die
//
static void usage(const char *me)
{
	fprintf(stderr, "Usage: %s [-df] [-t maxthreads] [-T threadTimeout]"
		"\t[-N bootstrapName] [-a authConfigFile]\n", me);
	exit(2);
}


//
// Handle SIGCHLD signals to reap our children (zombie cleanup)
//
static void handleSIGCHLD(int signal_number)
{
    kern_return_t kt = ucsp_client_handleSignal(gMainServerPort, mach_task_self(), signal_number);
    if (kt)
        syslog(LOG_ERR, "ucsp_client_handleSignal returned: %d", kt);
}


//
// Handle some other signals to shut down cleanly (and with logging)
//
static void handleSIGOther(int sig)
{
    switch (sig) {
    case SIGINT:
        //secdebug("SS", "Interrupt signal; terminating");
        Syslog::notice("received interrupt signal; terminating");
        exit(0);
    case SIGTERM:
        //secdebug("SS", "Termination signal; terminating");
        Syslog::notice("received termination signal; terminating");
        exit(0);
    }
}


#if !defined(NDEBUG)

static void handleSIGdebug(int)
{
	NodeCore::dumpAll();
}

#endif //NDEBUG
