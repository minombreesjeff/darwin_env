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
#ifndef _H_KEY
#define _H_KEY

#include "securityserver.h"
#include "structure.h"
#include "acls.h"
#include <security_cdsa_utilities/handleobject.h>
#include <security_cdsa_client/keyclient.h>


class Database;


//
// A Key object represents a CSSM_KEY known to the SecurityServer.
// We give each Key a handle that allows our clients to access it, while we use
// the Key's ACL to control such accesses.
// A Key can be used by multiple Connections. Whether more than one Key can represent
// the same actual key object is up to the CSP we use, so let's be tolerant about that.
//
// A note on key attributes: We keep two sets of attribute bits. The internal bits are used
// when talking to our CSP; the external bits are used when negotiating with our client(s).
// The difference is the bits in managedAttributes, which relate to persistent key storage
// and are not digestible by our CSP. The internal attributes are kept in mKey. The external
// ones are kept in mAttributes.
//
class Key : public PerProcess, public SecurityServerAcl {
public:
	Key();
	
	Database &database() const;
	
	virtual const CssmData &canonicalDigest() = 0;
	virtual CSSM_KEYATTR_FLAGS attributes() = 0;
	
	virtual void returnKey(Handle &h, CssmKey::Header &hdr) = 0;
	
public:
    // key attributes that should not be passed on to the CSP
    static const uint32 managedAttributes = KeyBlob::managedAttributes;
	// these attributes are "forced on" in internal keys (but not always in external attributes)
	static const uint32 forcedAttributes = KeyBlob::forcedAttributes;
	// these attributes are internally generated, and invalid on input
	static const uint32 generatedAttributes =
		CSSM_KEYATTR_ALWAYS_SENSITIVE | CSSM_KEYATTR_NEVER_EXTRACTABLE;
	
	// a version of KeySpec that self-checks and masks for CSP operation
	class KeySpec : public CssmClient::KeySpec {
	public:
		KeySpec(uint32 usage, uint32 attrs);
		KeySpec(uint32 usage, uint32 attrs, const CssmData &label);
	};
};


#endif //_H_KEY
