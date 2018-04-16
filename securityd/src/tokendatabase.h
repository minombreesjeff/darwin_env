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
// A TokenDatabase is a software storage container,
// implemented in cooperation by the AppleCSLDP CDSA plugin and this daemon.
//
#ifndef _H_TOKENDATABASE
#define _H_TOKENDATABASE

#include "database.h"

class TokenDatabase;
class TokenDbCommon;
class TokenKey;


//
// TokenDatabase DbCommons
//
class TokenDbCommon : public DbCommon {
public:
	TokenDbCommon(Session &ssn);
public:
	
};


//
// A Database object represents an Apple CSP/DL open database (DL/DB) object.
// It maintains its protected semantic state (including keys) and provides controlled
// access.
//
class TokenDatabase : public Database {
	friend class TokenDbCommon;
public:
	TokenDatabase(Process &proc);

	TokenDbCommon &common() const;
	
	const char *dbName() const;

public:
    CSSM_KEY_SIZE queryKeySize(Key &key);
	
	// service calls
	void generateSignature(const Context &context, Key &key, CSSM_ALGORITHMS signOnlyAlgorithm,
		const CssmData &data, CssmData &signature);
	void verifySignature(const Context &context, Key &key, CSSM_ALGORITHMS verifyOnlyAlgorithm,
		const CssmData &data, const CssmData &signature);
	void generateMac(const Context &context, Key &key,
		const CssmData &data, CssmData &mac);
	void verifyMac(const Context &context, Key &key,
		const CssmData &data, const CssmData &mac);
	
	void encrypt(const Context &context, Key &key, const CssmData &clear, CssmData &cipher);
	void decrypt(const Context &context, Key &key, const CssmData &cipher, CssmData &clear);
	
	void generateKey(const Context &context,
		const AccessCredentials *cred, const AclEntryPrototype *owner,
		uint32 usage, uint32 attrs, RefPointer<Key> &newKey);
	void generateKey(const Context &context,
		const AccessCredentials *cred, const AclEntryPrototype *owner,
		uint32 pubUsage, uint32 pubAttrs, uint32 privUsage, uint32 privAttrs,
		RefPointer<Key> &publicKey, RefPointer<Key> &privateKey);
	RefPointer<Key> deriveKey(const Context &context, Key *key,
		const AccessCredentials *cred, const AclEntryPrototype *owner,
		CssmData *param, uint32 usage, uint32 attrs);

    void wrapKey(const Context &context, Key *key,
        Key &keyToBeWrapped, const AccessCredentials *cred,
        const CssmData &descriptiveData, CssmKey &wrappedKey);
	RefPointer<Key> unwrapKey(const Context &context, Key *key,
		const AccessCredentials *cred, const AclEntryPrototype *owner,
		uint32 usage, uint32 attrs, const CssmKey wrappedKey,
        Key *publicKey, CssmData *descriptiveData);
        
    uint32 getOutputSize(const Context &context, Key &key, uint32 inputSize, bool encrypt = true);

public:
	// encoding/decoding databases
    void authenticate(const AccessCredentials *cred);
};


#endif //_H_TOKENDATABASE
