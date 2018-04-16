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
// database - abstract database management
//
// This file defines database objects that represent different
// way to implement "database with cryptographic operations on its contents".
// The objects here are abstract and need to be implemented to be useful.
//
#ifndef _H_DATABASE
#define _H_DATABASE

#include "securityserver.h"
#include "structure.h"
#include "acls.h"
#include "dbcrypto.h"
#include "notifications.h"
#include <security_utilities/utilities.h>
#include <security_cdsa_utilities/handleobject.h>
#include <security_cdsa_utilities/cssmdb.h>
#include <security_utilities/machserver.h>
#include <security_agent_client/agentclient.h>
#include <security_utilities/timeflow.h>
#include <string>
#include <map>


class Key;
class Connection;
class Process;
class Session;
using MachPlusPlus::MachServer;


//
// A Database::DbCommon is the "common core" of all Database objects that
// represent the same client database (on disk, presumably).
// NOTE: DbCommon obeys exterior locking protocol: the caller (always Database)
// must lock it before operating on its non-const members. In practice,
// most Database methods lock down their DbCommon first thing.
//
class DbCommon : public PerSession {
public:
	DbCommon(Session &ssn);
	
	Session &session() const;
	
	virtual void sleepProcessing();
};


//
// A Database object represents an Apple CSP/DL open database (DL/DB) object.
// It maintains its protected semantic state (including keys) and provides controlled
// access.
//
class Database : public PerProcess, public SecurityServerAcl {
    static const NotificationEvent lockedEvent = kNotificationEventLocked;
    static const NotificationEvent unlockedEvent = kNotificationEventUnlocked;
    static const NotificationEvent passphraseChangedEvent = kNotificationEventPassphraseChanged;

protected:
	Database(Process &proc);
	
public:
	Process& process() const;

	virtual void releaseKey(Key &key);
    virtual CSSM_KEY_SIZE queryKeySize(Key &key) = 0;
	
	// service calls
	virtual void generateSignature(const Context &context, Key &key,
		CSSM_ALGORITHMS signOnlyAlgorithm, const CssmData &data, CssmData &signature) = 0;
	virtual void verifySignature(const Context &context, Key &key,
		CSSM_ALGORITHMS verifyOnlyAlgorithm, const CssmData &data, const CssmData &signature) = 0;
	virtual void generateMac(const Context &context, Key &key,
		const CssmData &data, CssmData &mac) = 0;
	virtual void verifyMac(const Context &context, Key &key,
		const CssmData &data, const CssmData &mac) = 0;
	
	virtual void encrypt(const Context &context, Key &key, const CssmData &clear, CssmData &cipher) = 0;
	virtual void decrypt(const Context &context, Key &key, const CssmData &cipher, CssmData &clear) = 0;
	
	virtual void generateKey(const Context &context,
		const AccessCredentials *cred, const AclEntryPrototype *owner,
		uint32 usage, uint32 attrs, RefPointer<Key> &newKey) = 0;
	virtual void generateKey(const Context &context,
		const AccessCredentials *cred, const AclEntryPrototype *owner,
		uint32 pubUsage, uint32 pubAttrs, uint32 privUsage, uint32 privAttrs,
		RefPointer<Key> &publicKey, RefPointer<Key> &privateKey) = 0;
	virtual RefPointer<Key> deriveKey(const Context &context, Key *key,
		const AccessCredentials *cred, const AclEntryPrototype *owner,
		CssmData *param, uint32 usage, uint32 attrs) = 0;

    virtual void wrapKey(const Context &context, Key *key,
        Key &keyToBeWrapped, const AccessCredentials *cred,
        const CssmData &descriptiveData, CssmKey &wrappedKey) = 0;
	virtual RefPointer<Key> unwrapKey(const Context &context, Key *key,
		const AccessCredentials *cred, const AclEntryPrototype *owner,
		uint32 usage, uint32 attrs, const CssmKey wrappedKey,
        Key *publicKey, CssmData *descriptiveData) = 0;
        
    virtual uint32 getOutputSize(const Context &context, Key &key,
		uint32 inputSize, bool encrypt = true) = 0;

	virtual void authenticate(const AccessCredentials *cred) = 0;

public:
	static const int maxUnlockTryCount = 3;

public:
	DbCommon& common() const			{ return parent<DbCommon>(); }
	virtual const char *dbName() const = 0;

protected:
    AccessCredentials *mCred;		// local access credentials (always valid)
};


//
// This class implements a "system keychaiin unlock record" store
//
class SystemKeychainKey {
public:
	SystemKeychainKey(const char *path);
	~SystemKeychainKey();
	
	bool matches(const DbBlob::Signature &signature);
	CssmKey &key()		{ return mKey; }

private:
	std::string mPath;					// path to file
	CssmKey mKey;						// proper CssmKey with data in mBlob

	bool mValid;						// mBlob was validly read from mPath
	UnlockBlob mBlob;					// contents of mPath as last read
	
	Time::Absolute mCachedDate;			// modify date of file when last read
	Time::Absolute mUpdateThreshold;	// cutoff threshold for checking again
	
	static const int checkDelay = 1;	// seconds minimum delay between update checks
	
	bool update();
};

#endif //_H_DATABASE
