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
// CoreFoundation related utilities
//
#include <security_utilities/cfutilities.h>
#include <security_utilities/debugging.h>


namespace Security {


//
// Turn a CFString into a UTF8-encoded C++ string
//
string cfString(CFStringRef str)
{
	// NULL translates (cleanly) to empty
	if (str == NULL)
		return "";

	// quick path first
	if (const char *s = CFStringGetCStringPtr(str, kCFStringEncodingUTF8))
		return s;
	
	// need to extract into buffer
	string ret;
	CFIndex length = CFStringGetLength(str);	// in 16-bit character units
	char *buffer = new char[6 * length + 1];	// pessimistic
	if (CFStringGetCString(str, buffer, 6 * length + 1, kCFStringEncodingUTF8))
		ret = buffer;
	delete[] buffer;
	return ret;
}


}	// end namespace Security
