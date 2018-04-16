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
// localdatabase - locally implemented database using internal CSP cryptography
//
#include "localdatabase.h"
#include "agentquery.h"
#include "localkey.h"
#include "server.h"
#include "session.h"
#include <security_agent_client/agentclient.h>
#include <security_cdsa_utilities/acl_any.h>	// for default owner ACLs
#include <security_cdsa_client/wrapkey.h>
#include <security_cdsa_client/genkey.h>
#include <security_cdsa_client/signclient.h>
#include <security_cdsa_client/cryptoclient.h>
#include <security_cdsa_client/macclient.h>
#include <security_utilities/endian.h>


//
// Create a Database object from initial parameters (create operation)
//
LocalDatabase::LocalDatabase(Process &proc)
	: Database(proc)
{
}


static inline LocalKey &myKey(Key &key)
{
	return safer_cast<LocalKey &>(key);
}


//
// Key inquiries
//
CSSM_KEY_SIZE LocalDatabase::queryKeySize(Key &key)
{
    CssmClient::Key theKey(Server::csp(), myKey(key));
    return theKey.sizeInBits();
}


//
// Signatures and MACs
//
void LocalDatabase::generateSignature(const Context &context, Key &key,
	CSSM_ALGORITHMS signOnlyAlgorithm, const CssmData &data, CssmData &signature)
{
	context.replace(CSSM_ATTRIBUTE_KEY, myKey(key).cssmKey());
	key.validate(CSSM_ACL_AUTHORIZATION_SIGN, context);
	CssmClient::Sign signer(Server::csp(), context.algorithm(), signOnlyAlgorithm);
	signer.override(context);
	signer.sign(data, signature);
}

void LocalDatabase::verifySignature(const Context &context, Key &key,
	CSSM_ALGORITHMS verifyOnlyAlgorithm, const CssmData &data, const CssmData &signature)
{
	context.replace(CSSM_ATTRIBUTE_KEY, myKey(key).cssmKey());
	CssmClient::Verify verifier(Server::csp(), context.algorithm(), verifyOnlyAlgorithm);
	verifier.override(context);
	verifier.verify(data, signature);
}

void LocalDatabase::generateMac(const Context &context, Key &key,
	const CssmData &data, CssmData &mac)
{
	context.replace(CSSM_ATTRIBUTE_KEY, myKey(key).cssmKey());
	key.validate(CSSM_ACL_AUTHORIZATION_MAC, context);
	CssmClient::GenerateMac signer(Server::csp(), context.algorithm());
	signer.override(context);
	signer.sign(data, mac);
}

void LocalDatabase::verifyMac(const Context &context, Key &key,
	const CssmData &data, const CssmData &mac)
{
	context.replace(CSSM_ATTRIBUTE_KEY, myKey(key).cssmKey());
	key.validate(CSSM_ACL_AUTHORIZATION_MAC, context);
	CssmClient::VerifyMac verifier(Server::csp(), context.algorithm());
	verifier.override(context);
	verifier.verify(data, mac);
}


//
// Encryption/decryption
//
void LocalDatabase::encrypt(const Context &context, Key &key,
	const CssmData &clear, CssmData &cipher)
{
	context.replace(CSSM_ATTRIBUTE_KEY, myKey(key).cssmKey());
	key.validate(CSSM_ACL_AUTHORIZATION_ENCRYPT, context);
	CssmClient::Encrypt cryptor(Server::csp(), context.algorithm());
	cryptor.override(context);
	CssmData remData;
	size_t totalLength = cryptor.encrypt(clear, cipher, remData);
	// shouldn't need remData - if an algorithm REQUIRES this, we'd have to ship it
	if (remData)
		CssmError::throwMe(CSSM_ERRCODE_INTERNAL_ERROR);
	cipher.length(totalLength);
}

void LocalDatabase::decrypt(const Context &context, Key &key,
	const CssmData &cipher, CssmData &clear)
{
	context.replace(CSSM_ATTRIBUTE_KEY, myKey(key).cssmKey());
	key.validate(CSSM_ACL_AUTHORIZATION_DECRYPT, context);
	CssmClient::Decrypt cryptor(Server::csp(), context.algorithm());
	cryptor.override(context);
	CssmData remData;
	size_t totalLength = cryptor.decrypt(cipher, clear, remData);
	// shouldn't need remData - if an algorithm REQUIRES this, we'd have to ship it
	if (remData)
		CssmError::throwMe(CSSM_ERRCODE_INTERNAL_ERROR);
	clear.length(totalLength);
}


//
// Key generation and derivation.
// Currently, we consider symmetric key generation to be fast, but
// asymmetric key generation to be (potentially) slow.
//
void LocalDatabase::generateKey(const Context &context,
		const AccessCredentials *cred, const AclEntryPrototype *owner,
		uint32 usage, uint32 attrs, RefPointer<Key> &newKey)
{
	// prepare a context
	CssmClient::GenerateKey generate(Server::csp(), context.algorithm());
	generate.override(context);
	
	// generate key
	// @@@ turn "none" return into reference if permanent (only)
	CssmKey key;
	generate(key, Key::KeySpec(usage, attrs));
		
	// register and return the generated key
    newKey = makeKey(key, attrs & Key::managedAttributes, owner);
}

void LocalDatabase::generateKey(const Context &context,
	const AccessCredentials *cred, const AclEntryPrototype *owner,
	uint32 pubUsage, uint32 pubAttrs, uint32 privUsage, uint32 privAttrs,
    RefPointer<Key> &publicKey, RefPointer<Key> &privateKey)
{
	// prepare a context
	CssmClient::GenerateKey generate(Server::csp(), context.algorithm());
	generate.override(context);
	
	// this may take a while; let our server object know
	Server::active().longTermActivity();
	
	// generate keys
	// @@@ turn "none" return into reference if permanent (only)
	CssmKey pubKey, privKey;
	generate(pubKey, Key::KeySpec(pubUsage, pubAttrs),
		privKey, Key::KeySpec(privUsage, privAttrs));
		
	// register and return the generated keys
	publicKey = makeKey(pubKey, pubAttrs & Key::managedAttributes, owner);
	privateKey = makeKey(privKey, privAttrs & Key::managedAttributes, owner);
}

RefPointer<Key> LocalDatabase::deriveKey(const Context &context, Key *baseKey,
		const AccessCredentials *cred, const AclEntryPrototype *owner,
        CssmData *param, uint32 usage, uint32 attrs)
{
	// prepare a key-derivation context
    if (baseKey) {
		baseKey->validate(CSSM_ACL_AUTHORIZATION_DERIVE, cred);
        context.replace(CSSM_ATTRIBUTE_KEY, myKey(*baseKey).cssmKey());
	}
	CssmClient::DeriveKey derive(Server::csp(), context.algorithm(), CSSM_ALGID_NONE);
	derive.override(context);
	
	// derive key
	// @@@ turn "none" return into reference if permanent (only)
	CssmKey key;
	derive(param, Key::KeySpec(usage, attrs), key);
		
	// register and return the generated key
    return makeKey(key, attrs & Key::managedAttributes, owner);
}


//
// Key wrapping and unwrapping.
// Note that the key argument (the key in the context) is optional because of the special
// case of "cleartext" (null algorithm) wrapping for import/export.
//

void LocalDatabase::wrapKey(const Context &context, Key *key,
    Key &keyToBeWrapped, const AccessCredentials *cred,
    const CssmData &descriptiveData, CssmKey &wrappedKey)
{
    keyToBeWrapped.validate(context.algorithm() == CSSM_ALGID_NONE ?
            CSSM_ACL_AUTHORIZATION_EXPORT_CLEAR : CSSM_ACL_AUTHORIZATION_EXPORT_WRAPPED,
        cred);
	if(!(keyToBeWrapped.attributes() & CSSM_KEYATTR_EXTRACTABLE)) {
		CssmError::throwMe(CSSMERR_CSP_INVALID_KEYATTR_MASK);
	}
    if (key) {
        context.replace(CSSM_ATTRIBUTE_KEY, myKey(*key).cssmKey());
		key->validate(CSSM_ACL_AUTHORIZATION_ENCRYPT, context);
	}
    CssmClient::WrapKey wrap(Server::csp(), context.algorithm());
    wrap.override(context);
    wrap.cred(const_cast<AccessCredentials *>(cred));	//@@@ const madness - fix in client/pod
    wrap(myKey(keyToBeWrapped), wrappedKey, &descriptiveData);
}

RefPointer<Key> LocalDatabase::unwrapKey(const Context &context, Key *key,
	const AccessCredentials *cred, const AclEntryPrototype *owner,
	uint32 usage, uint32 attrs, const CssmKey wrappedKey,
    Key *publicKey, CssmData *descriptiveData)
{
    if (key) {
        context.replace(CSSM_ATTRIBUTE_KEY, myKey(*key).cssmKey());
		key->validate(CSSM_ACL_AUTHORIZATION_DECRYPT, context);
	}
    CssmClient::UnwrapKey unwrap(Server::csp(), context.algorithm());
    unwrap.override(context);
    CssmKey unwrappedKey;
    unwrap.cred(const_cast<AccessCredentials *>(cred));	//@@@ const madness - fix in client/pod
    if (owner) {
        AclEntryInput ownerInput(*owner);	//@@@ const trouble - fix in client/pod
        unwrap.aclEntry(ownerInput);
    }

    // @@@ Invoking conversion operator to CssmKey & on *publicKey and take the address of the result.
    unwrap(wrappedKey, Key::KeySpec(usage, attrs), unwrappedKey,
        descriptiveData, publicKey ? &static_cast<const CssmKey &>(myKey(*publicKey)) : NULL);

    return makeKey(unwrappedKey, attrs & Key::managedAttributes, owner);
}


//
// Miscellaneous CSSM functions
//
uint32 LocalDatabase::getOutputSize(const Context &context, Key &key, uint32 inputSize, bool encrypt)
{
    // We're fudging here somewhat, since the context can be any type.
    // ctx.override will fix the type, and no-one's the wiser.
	context.replace(CSSM_ATTRIBUTE_KEY, myKey(key).cssmKey());
    CssmClient::Digest ctx(Server::csp(), context.algorithm());
    ctx.override(context);
    return ctx.getOutputSize(inputSize, encrypt);
}


//
// (Re-)Authenticate the database. This changes the stored credentials.
//
void LocalDatabase::authenticate(const AccessCredentials *cred)
{
	StLock<Mutex> _(common());
	AccessCredentials *newCred = DataWalkers::copy(cred, Allocator::standard());
    Allocator::standard().free(mCred);
    mCred = newCred;
}
