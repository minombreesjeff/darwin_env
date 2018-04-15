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

#include "eventlistener.h"


namespace Security {
namespace SecurityServer {

EventListener::EventListener (Allocator &standard, Allocator &returning)
	: mClientSession (standard, returning),
	mMachPortRef (NULL),
	mRunLoopSourceRef (NULL)
{
	Initialize ();
}



EventListener::~EventListener ()
{
	if (mMachPortRef != NULL)
	{
		mach_port_t mp = CFMachPortGetPort (mMachPortRef);
		mClientSession.stopNotification (mp);
		CFRelease (mMachPortRef);
	}
	
	if (mRunLoopSourceRef != NULL)
	{
		CFRelease (mRunLoopSourceRef);
	}
}



void EventListener::Callback (CFMachPortRef port, void *msg, CFIndex size, void *info)
{
	reinterpret_cast<EventListener*>(info)->HandleCallback (port, msg, size);
}



void EventListener::Initialize ()
{
	// create a callback information structure
	CFMachPortContext context = {1, this, NULL, NULL, NULL};
	
	// create the CFMachPort
	mMachPortRef = CFMachPortCreate (NULL, Callback, &context, NULL);
	if (mMachPortRef == NULL)
	{
		return;
	}
	
    // set the buffer limit for the port
	mach_port_t mp = CFMachPortGetPort (mMachPortRef);

    mach_port_limits_t limits;
    limits.mpl_qlimit = MACH_PORT_QLIMIT_MAX;
    kern_return_t result =
        mach_port_set_attributes (mach_task_self (), mp, MACH_PORT_LIMITS_INFO,
                                  mach_port_info_t (&limits), MACH_PORT_LIMITS_INFO_COUNT);

    if (result != KERN_SUCCESS)
    {
        secdebug ("notify", "Got error %d when trying to maximize queue size", result);
    }
    
	// make a run loop source for this ref
	mRunLoopSourceRef = CFMachPortCreateRunLoopSource (NULL, mMachPortRef, NULL);
	if (mRunLoopSourceRef == NULL)
	{
		CFRelease (mMachPortRef);
		return;
	}
	
	// attach this run loop source to the main run loop
	CFRunLoopAddSource (CFRunLoopGetCurrent (), mRunLoopSourceRef, kCFRunLoopDefaultMode);
	
	// extract the actual port from the run loop, and request callbacks on that port
	mClientSession.requestNotification (mp, kNotificationDomainDatabase,
											kNotificationAllEvents);
}



void EventListener::HandleCallback (CFMachPortRef port, void *msg, CFIndex size)
{
	// we need to parse the message and see what happened
	mClientSession.dispatchNotification (reinterpret_cast<mach_msg_header_t *>(msg), ProcessMessage, this);
}



OSStatus EventListener::ProcessMessage (NotificationDomain domain, NotificationEvent event, const void *data, size_t dataLength, void *context)
{
	reinterpret_cast<EventListener*>(context)->EventReceived (domain, event, data, dataLength);
	return noErr;
}



void EventListener::RequestEvents (NotificationDomain whichDomain, NotificationMask whichEvents)
{
	// stop the old event request and change to the new one
	mach_port_t mp = CFMachPortGetPort (mMachPortRef);
	mClientSession.stopNotification (mp);
	mClientSession.requestNotification (mp, whichDomain, whichEvents);
}



void EventListener::EventReceived (NotificationDomain domain, NotificationEvent event, const void* data, size_t dataLength)
{
}



} // end namespace SecurityServer
} // end namespace Security
