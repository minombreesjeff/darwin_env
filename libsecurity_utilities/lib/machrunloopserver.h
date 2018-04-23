/*
 * Copyright (c) 2000-2001,2003-2004 Apple Computer, Inc. All Rights Reserved.
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
// machrunloopserver - C++ shell for writing Mach 3 servers called by CFRunLoop.
//
// Note that this is a subclass of MachServer and tries to preserve its interface,
// so you can switch back-and-forth between them with a minimum of fuss.
// Timers are not currently implemented; they're not that hard to add if you need them.
//
#ifndef _H_MACHRUNLOOPSERVER
#define _H_MACHRUNLOOPSERVER

#include <security_utilities/machserver.h>
#include <CoreFoundation/CFRunLoop.h>
#include <CoreFoundation/CFMachPort.h>


namespace Security
{

namespace MachPlusPlus
{

//
// Mach server object
//
class MachRunLoopServer : public MachServer {
public:
	MachRunLoopServer(const char *name);
	MachRunLoopServer(const char *name, const Bootstrap &boot);
	virtual ~MachRunLoopServer();

	void run(size_t maxSize = 4096, mach_msg_options_t options = 0);

	static MachRunLoopServer &active()
	{ return safer_cast<MachRunLoopServer &>(MachServer::active()); }
	
	void notifyIfDead(Port port) const;
	
	void blockNewRequests(bool block = true);

	void alsoListenOn(Port port);
	void stopListenOn(Port port);

protected:
    void setup(const char *name, size_t bufferSize);
	static void cfCallback(CFMachPortRef port, void *msg, CFIndex size, void *info);
	static void cfInvalidateCallback(CFMachPortRef port, void *info);
	void oneRequest(mach_msg_header_t *request);
	
private:
	CFRunLoopRef runLoop;
	CFRunLoopSourceRef runLoopSource;
	
	mach_msg_header_t *replyBuffer;
};


} // end namespace MachPlusPlus

} // end namespace Security

#endif //_H_MACHRUNLOOPSERVER
