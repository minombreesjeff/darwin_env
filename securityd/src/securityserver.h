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
// securityserver.h - master header file for the SecurityServer.
//
#ifndef _H_SECURITYSERVER
#define _H_SECURITYSERVER

#include <securityd_client/ssblob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Security/cssm.h>
//#include <security_utilities/utilities.h>
#include <security_cdsa_utilities/context.h>
#include <securityd_client/ssclient.h>
#include <security_utilities/mach++.h>
#include <security_utilities/unix++.h>
#include <security_utilities/debugging.h>
#include <security_utilities/logging.h>

namespace Security {

using namespace SecurityServer;
using namespace UnixPlusPlus;


//
// Logging and verbosity levels
//
extern uint32 debugMode;
extern const char *bootstrapName;

} // end namespace Security

#endif //_H_SECURITYSERVER
