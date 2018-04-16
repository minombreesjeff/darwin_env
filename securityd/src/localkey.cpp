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
#include "localkey.h"
#include "server.h"
#include "database.h"
#include <security_cdsa_utilities/acl_any.h>


//
// Create a Key from an explicit CssmKey.
//
LocalKey::LocalKey(Database &db, const CssmKey &newKey, uint32 moreAttributes,
	const AclEntryPrototype *owner)
	: mDigest(Server::csp().allocator())
{
	referent(db);
	mValidKey = true;
	setup(newKey, moreAttributes);
	
	// establish initial ACL; reinterpret empty (null-list) owner as NULL for resilence's sake
	if (owner && !owner->subject().empty())
		cssmSetInitial(*owner);					// specified
	else
		cssmSetInitial(new AnyAclSubject());	// defaulted
    secdebug("SSkey", "%p (handle 0x%lx) created from key alg=%ld use=0x%lx attr=0x%lx db=%p",
        this, handle(), mKey.header().algorithm(), mKey.header().usage(), mAttributes, &db);
}


LocalKey::LocalKey(Database &db)
	: mValidKey(false), mAttributes(0), mDigest(Server::csp().allocator())
{
	referent(db);
}


//
// Set up the CssmKey part of this Key according to instructions.
//
void LocalKey::setup(const CssmKey &newKey, uint32 moreAttributes)
{
	mKey = CssmClient::Key(Server::csp(), newKey, false);
    CssmKey::Header &header = mKey->header();
    
	// copy key header
	header = newKey.header();
    mAttributes = (header.attributes() & ~forcedAttributes) | moreAttributes;
	
	// apply initial values of derived attributes (these are all in managedAttributes)
    if (!(mAttributes & CSSM_KEYATTR_EXTRACTABLE))
        mAttributes |= CSSM_KEYATTR_NEVER_EXTRACTABLE;
    if (mAttributes & CSSM_KEYATTR_SENSITIVE)
        mAttributes |= CSSM_KEYATTR_ALWAYS_SENSITIVE;

    // verify internal/external attribute separation
    assert((header.attributes() & managedAttributes) == forcedAttributes);
}


LocalKey::~LocalKey()
{
    secdebug("SSkey", "%p destroyed", this);
}


LocalDatabase &LocalKey::database() const
{
	return referent<LocalDatabase>();
}


//
// Retrieve the actual CssmKey value for the key object.
// This will decode its blob if needed (and appropriate).
//
CssmClient::Key LocalKey::keyValue()
{
    if (!mValidKey) {
		getKey();
		mValidKey = true;
	}
    return mKey;
}


//
// Return a key's handle and header in external form
//
void LocalKey::returnKey(Handle &h, CssmKey::Header &hdr)
{
    // return handle
    h = handle();
	
	// obtain the key header, from the valid key or the blob if no valid key
	if (mValidKey) {
		hdr = mKey.header();
	} else {
		getHeader(hdr);
	}
    
    // adjust for external attributes
	hdr.clearAttribute(forcedAttributes);
    hdr.setAttribute(mAttributes);
}


//
// Generate the canonical key digest.
// This is defined by a CSP feature that we invoke here.
//
const CssmData &LocalKey::canonicalDigest()
{
	if (!mDigest) {
		CssmClient::PassThrough ctx(Server::csp());
		ctx.key(keyValue());
		CssmData *digest = NULL;
		ctx(CSSM_APPLECSP_KEYDIGEST, (const void *)NULL, &digest);
		assert(digest);
		mDigest.set(*digest);	// takes ownership of digest data
		Server::csp().allocator().free(digest);	// the CssmData itself
	}
	return mDigest.get();
}


//
// Default getKey/getHeader calls - should never be called
//
void LocalKey::getKey()
{
	assert(false);
}

void LocalKey::getHeader(CssmKey::Header &)
{
	assert(false);
}
