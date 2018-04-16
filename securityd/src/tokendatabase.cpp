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
// tokendatabase - software database container implementation.
//
#include "tokendatabase.h"
#include "process.h"
#include "key.h"


class TokenKey : public Key {
public:
};


//
// Construct a TokenDatabase
//
TokenDatabase::TokenDatabase(Process &proc)
	: Database(proc)
{
	proc.addReference(*this);
}


//
// Basic Database virtual implementations
//
TokenDbCommon &TokenDatabase::common() const
{
	return parent<TokenDbCommon>();
}

const char *TokenDatabase::dbName() const
{
	return "<<whatever>>";
}


static inline TokenKey &myKey(Key &key)
{
	return safer_cast<TokenKey &>(key);
}




//
// Key inquiries
//
CSSM_KEY_SIZE TokenDatabase::queryKeySize(Key &key)
{
	CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}


//
// Signatures and MACs
//
void TokenDatabase::generateSignature(const Context &context, Key &key,
	CSSM_ALGORITHMS signOnlyAlgorithm, const CssmData &data, CssmData &signature)
{
	key.validate(CSSM_ACL_AUTHORIZATION_SIGN, context);
	CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}

void TokenDatabase::verifySignature(const Context &context, Key &key,
	CSSM_ALGORITHMS verifyOnlyAlgorithm, const CssmData &data, const CssmData &signature)
{
	CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}

void TokenDatabase::generateMac(const Context &context, Key &key,
	const CssmData &data, CssmData &mac)
{
	key.validate(CSSM_ACL_AUTHORIZATION_MAC, context);
	CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}

void TokenDatabase::verifyMac(const Context &context, Key &key,
	const CssmData &data, const CssmData &mac)
{
	key.validate(CSSM_ACL_AUTHORIZATION_MAC, context);
	CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}


//
// Encryption/decryption
//
void TokenDatabase::encrypt(const Context &context, Key &key,
	const CssmData &clear, CssmData &cipher)
{
	key.validate(CSSM_ACL_AUTHORIZATION_ENCRYPT, context);
	CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}

void TokenDatabase::decrypt(const Context &context, Key &key,
	const CssmData &cipher, CssmData &clear)
{
	key.validate(CSSM_ACL_AUTHORIZATION_DECRYPT, context);
	CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}


//
// Key generation and derivation.
// Currently, we consider symmetric key generation to be fast, but
// asymmetric key generation to be (potentially) slow.
//
void TokenDatabase::generateKey(const Context &context,
		const AccessCredentials *cred, const AclEntryPrototype *owner,
		uint32 usage, uint32 attrs, RefPointer<Key> &newKey)
{
	CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}

void TokenDatabase::generateKey(const Context &context,
	const AccessCredentials *cred, const AclEntryPrototype *owner,
	uint32 pubUsage, uint32 pubAttrs, uint32 privUsage, uint32 privAttrs,
    RefPointer<Key> &publicKey, RefPointer<Key> &privateKey)
{
	CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}

RefPointer<Key> TokenDatabase::deriveKey(const Context &context, Key *baseKey,
		const AccessCredentials *cred, const AclEntryPrototype *owner,
        CssmData *param, uint32 usage, uint32 attrs)
{
	CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}


//
// Key wrapping and unwrapping.
// Note that the key argument (the key in the context) is optional because of the special
// case of "cleartext" (null algorithm) wrapping for import/export.
//

void TokenDatabase::wrapKey(const Context &context, Key *key,
    Key &keyToBeWrapped, const AccessCredentials *cred,
    const CssmData &descriptiveData, CssmKey &wrappedKey)
{
	CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}

RefPointer<Key> TokenDatabase::unwrapKey(const Context &context, Key *key,
	const AccessCredentials *cred, const AclEntryPrototype *owner,
	uint32 usage, uint32 attrs, const CssmKey wrappedKey,
    Key *publicKey, CssmData *descriptiveData)
{
	CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}


//
// Miscellaneous CSSM functions
//
uint32 TokenDatabase::getOutputSize(const Context &context, Key &key, uint32 inputSize, bool encrypt)
{
	CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}


//
// (Re-)Authenticate the database. This changes the stored credentials.
//
void TokenDatabase::authenticate(const AccessCredentials *cred)
{
	CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}
