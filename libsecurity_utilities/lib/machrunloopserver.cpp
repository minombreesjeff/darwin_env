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
// machrunloopserver - C++ shell for writing Mach 3 servers called by CFRunLoop
//
#include "machrunloopserver.h"
#include <security_utilities/cfutilities.h>
#include <mach/mach_error.h>
#include <security_utilities/debugging.h>


namespace Security {
namespace MachPlusPlus {


//
// Generic Mach server
//
MachRunLoopServer::MachRunLoopServer(const char *name) : MachServer(name)
{
}

MachRunLoopServer::MachRunLoopServer(const char *name, const Bootstrap &boot)
: MachServer(name, boot)
{
}

void MachRunLoopServer::run(size_t bufferSize, mach_msg_options_t options)
{
	// allocate reply buffer (well, try)
	replyBuffer = Allocator::standard().malloc<mach_msg_header_t>(bufferSize);

	// Now do the CFRunLoop tango...
	runLoop = CFRunLoopGetCurrent();
	CFRef<CFMachPortRef> cfPort = CFMachPortCreateWithPort(NULL, mServerPort, cfCallback,
        NULL, NULL);
	runLoopSource =
		CFMachPortCreateRunLoopSource(NULL, cfPort, 10);	//@@@ no idea what order is good
	if (!runLoop || !runLoopSource || !cfPort)
		Error::throwMe(MIG_SERVER_DIED);	//@@@ $#!!& CF non-diagnostics!
	CFRunLoopAddSource(runLoop, runLoopSource, kCFRunLoopDefaultMode);
	
	// we are it!
	perThread().server = this;
}

MachRunLoopServer::~MachRunLoopServer()
{
	// remove our runloop source
	CFRunLoopRemoveSource(runLoop, runLoopSource, kCFRunLoopDefaultMode);
	CFRelease(runLoopSource);
	
	// delete the reply buffer
	Allocator::standard().free(replyBuffer);
    
    // no longer tagged
    perThread().server = NULL;
	
	// our MachServer parent class will clean up the ports and deregister from our bootstrap
}


//
// Block/unblock new request reception to serialize the request queue
//
void MachRunLoopServer::blockNewRequests(bool block)
{
	if (block) {
		CFRunLoopRemoveSource(runLoop, runLoopSource, kCFRunLoopDefaultMode);
		secdebug("machsrv", "disabled request reception");
	} else {
		CFRunLoopAddSource(runLoop, runLoopSource, kCFRunLoopDefaultMode);
		secdebug("machsrv", "enabled request reception");
	}
}


//
// Add secondary ports to receive on
//
void MachRunLoopServer::alsoListenOn(Port port)
{
	CFRef<CFMachPortRef> cfPort = CFMachPortCreateWithPort(NULL, port, cfCallback,
        NULL, NULL);
	CFRef<CFRunLoopSourceRef> source =
		CFMachPortCreateRunLoopSource(NULL, cfPort, 10);	//@@@ no idea what order is good
	CFRunLoopAddSource(runLoop, source, kCFRunLoopDefaultMode);
	secdebug("machsrv", "also receiving from port %d", port.port());
}

void MachRunLoopServer::stopListenOn(Port port)
{
	CFRef<CFMachPortRef> cfPort = CFMachPortCreateWithPort(NULL, port, cfCallback,
        NULL, NULL);
	CFRef<CFRunLoopSourceRef> source =
		CFMachPortCreateRunLoopSource(NULL, cfPort, 10);	//@@@ no idea what order is good
	CFRunLoopRemoveSource(runLoop, source, kCFRunLoopDefaultMode);
	secdebug("machsrv", "no longer receiving from port %d", port.port());
}


//
// Handle dead-port notifications.
// Since we don't actually run our own runloop here, we can't well use standard
// notifications to our own server port. So we use a CFMachPort facility instead.
//
void MachRunLoopServer::notifyIfDead(Port port) const
{
	//@@@ not clear how to deal with CFRetainCount of cfPort here
	//    will CF clean up the cfPort when it dies? Or do we have to keep a set?
	CFMachPortRef cfPort = CFMachPortCreateWithPort(NULL, port, NULL, NULL, NULL);
	if (cfPort != NULL) // check to make sure that we got a valid port reference back
	{
		CFMachPortSetInvalidationCallBack(cfPort, cfInvalidateCallback);
	}
}

void MachRunLoopServer::cfInvalidateCallback(CFMachPortRef cfPort, void *)
{
	active().notifyDeadName(CFMachPortGetPort(cfPort));
}


//
// The callback triggered from CFRunLoop
//
void MachRunLoopServer::cfCallback(CFMachPortRef port, void *msg, CFIndex, void *)
{
	active().oneRequest(reinterpret_cast<mach_msg_header_t *>(msg));
}

void MachRunLoopServer::oneRequest(mach_msg_header_t *request)
{
	if (!handle(request, replyBuffer)) {
		// MIG dispatch did not recognize the request. Ignore/Retry/Fail? :-)
		//@@@ Should send an error reply back here, I suppose. Later...
		secdebug("machrls", "MachRunLoopServer dispatch failed");
		return;
	}
	
	// MIG dispatch handled the call. Send reply back to caller.
	// This boilerplate stolen from mach_msg_server, since MIG can't seem to
	// generate send-only code for replies (without explicit simpleroutines).
	if (IFDEBUG(kern_return_t err =) mach_msg_overwrite(replyBuffer,
		(MACH_MSGH_BITS_REMOTE(replyBuffer->msgh_bits) == MACH_MSG_TYPE_MOVE_SEND_ONCE) ?
		MACH_SEND_MSG :	MACH_SEND_MSG|MACH_SEND_TIMEOUT,
		replyBuffer->msgh_size, 0, MACH_PORT_NULL,
		0, MACH_PORT_NULL, (mach_msg_header_t *) 0, 0)) {
		//@@@ should at least clean up resources here, I suppose.
		secdebug("machsrv", "RunloopServer cannot post reply: %s", mach_error_string(err));
		active().releaseDeferredAllocations();
		return;
	}
	active().releaseDeferredAllocations();
	return;
}


} // end namespace MachPlusPlus
} // end namespace Security
