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
#ifndef _H_TOKEN
#define _H_TOKEN

#include "securityserver.h"
#include "structure.h"
#include "pcsc++.h"


class Reader;


//
// Token is the global-scope object representing a smartcard token
//
class Token : public PerGlobal {
public:
	Token();
	~Token();
	
	Reader &reader() const;
	
	void insert(Reader &slot);
	void remove();
	
	void notify(NotificationEvent event);
		
	IFDUMP(void dumpNode());

private:
	PCSC::ReaderState mState;
};


#endif //_H_TOKEN
