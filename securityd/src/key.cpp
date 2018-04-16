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
// key - representation of SecurityServer key objects
//
#include "key.h"
#include "server.h"
#include "database.h"
#include <security_cdsa_utilities/acl_any.h>


Key::Key()
	: SecurityServerAcl(keyAcl, Allocator::standard())
{
}


Database &Key::database() const
{
	return referent<Database>();
}


//
// Form a KeySpec with checking and masking
//
Key::KeySpec::KeySpec(uint32 usage, uint32 attrs)
	: CssmClient::KeySpec(usage, (attrs & ~managedAttributes) | forcedAttributes)
{
	if (attrs & generatedAttributes)
		CssmError::throwMe(CSSMERR_CSP_INVALID_KEYATTR_MASK);
}

Key::KeySpec::KeySpec(uint32 usage, uint32 attrs, const CssmData &label)
	: CssmClient::KeySpec(usage, (attrs & ~managedAttributes) | forcedAttributes, label)
{
	if (attrs & generatedAttributes)
		CssmError::throwMe(CSSMERR_CSP_INVALID_KEYATTR_MASK);
}
