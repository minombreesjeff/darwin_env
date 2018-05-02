/*
 * Copyright (c) 2002-2004 Apple Computer, Inc. All Rights Reserved.
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


/*
 *  AuthorizationPriv.h -- Authorization SPIs
 *  Private APIs for implementing access control in applications and daemons.
 *  
 */

#ifndef _SECURITY_AUTHORIZATIONPRIV_H_
#define _SECURITY_AUTHORIZATIONPRIV_H_

#include <Security/Authorization.h>

#if defined(__cplusplus)
extern "C" {
#endif


/*!
	@header AuthorizationPriv
	Version 1.1 04/2003

	This header contains private APIs for authorization services.
	This is the private extension of <Security/Authorization.h>, a public header file.
*/


/*!
    @function AuthorizationBindPrivilegedPort

    @param fileDescriptor (input)
	
	@param name (input)
	
    @param authorization (input) The authorization object on which this operation is performed.
	
	@param flags (input) Bit mask of option flags to this call.

    @result errAuthorizationSuccess 0 No error.
*/
OSStatus AuthorizationBindPrivilegedPort(int fileDescriptor,
	const struct sockaddr_in *name,
	AuthorizationRef authorization,
	AuthorizationFlags flags);

int __authorization_bind(int s, const struct sockaddr_in *name);


#if defined(__cplusplus)
}
#endif

#endif /* !_SECURITY_AUTHORIZATIONPRIV_H_ */
