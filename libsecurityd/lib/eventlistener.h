/*
 * Copyright (c) 2003-2004 Apple Computer, Inc. All Rights Reserved.
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

#ifndef _H_EVENTLISTENER
#define _H_EVENTLISTENER

#include <securityd_client/ssclient.h>
#include <CoreFoundation/CFMachPort.h>



#undef verify

namespace Security {
namespace SecurityServer {


class EventListener
{
protected:
	ClientSession mClientSession;
	CFMachPortRef mMachPortRef;
	CFRunLoopSourceRef mRunLoopSourceRef;

	static void Callback (CFMachPortRef port, void *msg, CFIndex size, void *info);
	static OSStatus ProcessMessage (NotificationDomain domain, NotificationEvent event, const void *data, size_t dataLength, void *context);
	void HandleCallback (CFMachPortRef port, void *msg, CFIndex size);
	void HandleMessage ();
	void Initialize ();
	
public:
	EventListener (Allocator &standard = Allocator::standard(), Allocator &returning = Allocator::standard());
	virtual ~EventListener ();

	void RequestEvents (NotificationDomain domain, NotificationMask eventMask);
	virtual void EventReceived (NotificationDomain domain, NotificationEvent event, const void* data, size_t dataLength);
};


} // end namespace SecurityServer
} // end namespace Security


#endif
