/*
 * Copyright (c) 2004 Apple Computer, Inc. All Rights Reserved.
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
// token - internal representation of a (single distinct) hardware token
//
#include "token.h"
#include "reader.h"
#include "notifications.h"


Token::Token()
{
	secdebug("token", "%p created", this);
}


Token::~Token()
{
	secdebug("token", "%p destroyed", this);
}


Reader &Token::reader() const
{
	return referent<Reader>();
}


void Token::insert(Reader &slot)
{
	referent(slot);
	mState = slot.pcscState();
	//@@@ pupulate MDS here
	notify(kNotificationCDSAInsertion);
}


void Token::remove()
{
	//@@@ clear MDS here
	notify(kNotificationCDSARemoval);
	clearReferent();
}


//
// Send CDSA-layer notifications for this token.
// These events are usually received by CDSA plugins working with securityd.
// @@@ Need to add CDSA identifier as data
//
void Token::notify(NotificationEvent event)
{
	Listener::notify(kNotificationDomainCDSA, event, CssmData());
}


//
// Debug dump support
//
#if defined(DEBUGDUMP)

void Token::dumpNode()
{
	PerGlobal::dumpNode();
}

#endif //DEBUGDUMP
