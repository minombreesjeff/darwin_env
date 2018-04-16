/*
 * Copyright (c) 2000-2001 Apple Computer, Inc. All Rights Reserved.
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
// localkey - Key objects that store a local CSSM key object
//
#ifndef _H_LOCALKEY
#define _H_LOCALKEY

#include "key.h"
#include <security_cdsa_utilities/handleobject.h>
#include <security_cdsa_client/keyclient.h>


class LocalDatabase;


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
class LocalKey : public Key {
public:
	LocalKey(Database &db, const CssmKey &newKey, uint32 moreAttributes,
		const AclEntryPrototype *owner = NULL);
	virtual ~LocalKey();
    
	LocalDatabase &database() const;
	
    // yield the decoded internal key -- internal attributes
	CssmClient::Key key()		{ return keyValue(); }
	const CssmKey &cssmKey()	{ return keyValue(); }
	operator CssmClient::Key ()	{ return keyValue(); }
	operator const CssmKey &()	{ return keyValue(); }
    operator const CSSM_KEY & () { return keyValue(); }
    
    // yield the approximate external key header -- external attributes
    void returnKey(Handle &h, CssmKey::Header &hdr);
	
	// generate the canonical key digest
	const CssmData &canonicalDigest();
    
	CSSM_KEYATTR_FLAGS attributes() { return mAttributes; }
	
private:
	void setup(const CssmKey &newKey, uint32 attrs);
	CssmClient::Key keyValue();
	
protected:
	LocalKey(Database &db);
	
	virtual void getKey();				// decode into mKey or throw
	virtual void getHeader(CssmKey::Header &hdr); // get header (only) without mKey

protected:
	bool mValidKey;			// CssmKey form is valid
	CssmClient::Key mKey;	// clear form CssmKey (attributes modified)

    CSSM_KEYATTR_FLAGS mAttributes; // full attributes (external form)
	CssmAutoData mDigest;	// computed key digest (cached)
};


#endif //_H_LOCALKEY
