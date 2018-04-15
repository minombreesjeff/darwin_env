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
// ssclient - SecurityServer client interface library
//
// This interface is private to the Security system. It is not a public interface,
// and it may change at any time. You have been warned.
//
#ifndef _H_SSCLIENT
#define _H_SSCLIENT


#include <Security/cssm.h>
#include <security_utilities/alloc.h>
#include <security_cdsa_utilities/cssmdata.h>
#include <security_cdsa_utilities/cssmdbname.h>
#include <security_cdsa_utilities/cssmkey.h>
#include <security_cdsa_utilities/cssmacl.h>
#include <security_cdsa_utilities/context.h>
#include <security_utilities/globalizer.h>
#include <security_utilities/unix++.h>
#include <security_utilities/mach++.h>
#include <security_cdsa_utilities/cssmdb.h>
#include <security_cdsa_client/osxsigning.h>
#include <Security/Authorization.h>
#include <Security/AuthSession.h>
#include "ssnotify.h"


namespace Security {
namespace SecurityServer {

using MachPlusPlus::Port;
using MachPlusPlus::ReceivePort;


//
// The default Mach bootstrap registration name for SecurityServer,
// and the environment variable to override it
//
#define SECURITYSERVER_BOOTSTRAP_NAME	"com.apple.SecurityServer"
#define SECURITYSERVER_BOOTSTRAP_ENV	"SECURITYSERVER"


//
// Common data types
//
typedef CSSM_HANDLE AttachmentHandle;
typedef CSSM_HANDLE DbHandle;
typedef CSSM_HANDLE KeyHandle;
typedef CSSM_HANDLE RecordHandle;
typedef CSSM_HANDLE SearchHandle;

static const CSSM_HANDLE noDb = 0;
static const CSSM_HANDLE noKey = 0;

struct KeyUID {
    uint8 signature[20];
};

struct AuthorizationBlob {
    uint32 data[2];
	
	bool operator < (const AuthorizationBlob &other) const
	{ return memcmp(data, other.data, sizeof(data)) < 0; }
	
	bool operator == (const AuthorizationBlob &other) const
	{ return memcmp(data, other.data, sizeof(data)) == 0; }
    
    size_t hash() const {	//@@@ revisit this hash
        return data[0] ^ data[1] << 3;
    }
};

struct ClientSetupInfo {
	uint32 version;
};
#define SSPROTOVERSION 5

enum AclKind { dbAcl, keyAcl, loginAcl };


//
// Database parameter structure
//
class DBParameters {
public:
	uint32 idleTimeout;				// seconds idle timout lock
	uint8 lockOnSleep;				// lock keychain when system sleeps
};


//
// A client connection (session)
//
class ClientSession {
	NOCOPY(ClientSession)
public:
	typedef void DidChangeKeyAclCallback(void *context, ClientSession &clientSession,
		KeyHandle key, CSSM_ACL_AUTHORIZATION_TAG tag);

	ClientSession(Allocator &standard, Allocator &returning);
	virtual ~ClientSession();

	void registerForAclEdits(DidChangeKeyAclCallback *callback, void *context);

	Allocator &internalAllocator;
	Allocator &returnAllocator;

public:
	typedef Security::Context Context;

public:
	void activate();
	void terminate();
	
public:
	// use this only if you know what you're doing...
	void contactName(const char *name);
	const char *contactName() const;

public:
	// DL interface
	
	// Managing attachments.
	AttachmentHandle attach(const CssmSubserviceUid &ssuid);
	void detach(AttachmentHandle attachment);
	
	// Calls that operate on an attachment.
	// CSSM_DL_DbOpen
	DbHandle openDb(AttachmentHandle attachment,
					const char *dbName,
					const CssmNetAddress *dbLocation,
					CSSM_DB_ACCESS_TYPE accessType,
					const AccessCredentials *cred,
					const CssmData &openParameters);

	// CSSM_DL_DbCreate
	DbHandle createDb(AttachmentHandle attachment,
					  const char *dbName,
					  const CssmNetAddress *dbLocation,
					  const CSSM_DBINFO &dBInfo,
					  CSSM_DB_ACCESS_TYPE accessType,
					  const AccessCredentials *cred,
					  const AclEntryInput *owner,
					  const CssmData &openParameters);

	// CSSM_DL_DbDelete
	void deleteDb(AttachmentHandle attachment,
				  const char *dbName,
				  const CssmNetAddress *dbLocation,
                  const AccessCredentials *cred);

	// CSSM_DL_GetDbNames
	CSSM_NAME_LIST_PTR getDbNames(AttachmentHandle attachment);

	// CSSM_DL_FreeNameList @@@ Not an RPC
	void freeNameList(AttachmentHandle attachment,
                      CSSM_NAME_LIST_PTR NameList);

	// Calls that operate on a db
	// CSSM_DL_GetDbNameFromHandle result is allocated using the returnAllocator.
	char *getDbNameFromHandle(DbHandle db);
	// CSSM_DL_DbClose
	void closeDb(DbHandle db);
	// CSSM_DL_Authenticate
	// @@@ cred should be non optional.
	void authenticateDb(DbHandle db, CSSM_DB_ACCESS_TYPE type, const AccessCredentials *cred);
	// CSSM_DL_CreateRelation
	void createRelation(DbHandle db,
                        CSSM_DB_RECORDTYPE recordType,
                        const char &relationName,
                        uint32 attributeCount,
                        const CSSM_DB_SCHEMA_ATTRIBUTE_INFO *attributes,
                        uint32 indexCount,
                        const CSSM_DB_SCHEMA_INDEX_INFO *indices);
	// CSSM_DL_DestroyRelation
	void destroyRelation(DbHandle db,
						 CSSM_DB_RECORDTYPE recordType);
	// CSSM_DL_DataInsert
	RecordHandle insertRecord(DbHandle db,
							  CSSM_DB_RECORDTYPE recordType,
							  const CssmDbRecordAttributeData *attributes,
							  const CssmData *data);
	// CSSM_DL_DataDelete
	void deleteRecord(RecordHandle record);
	// CSSM_DL_DataModify
	void modifyRecord(RecordHandle record,
					  CSSM_DB_RECORDTYPE recordType,
					  const CssmDbRecordAttributeData *attributesToBeModified,
					  const CssmData *dataToBeModified,
					  CSSM_DB_MODIFY_MODE modifyMode);
	// CSSM_DL_DataGetFirst
	RecordHandle getFirstRecord(DbHandle db,
								const CssmQuery &query,
								SearchHandle &outSearchHandle,
								CssmDbRecordAttributeData *inOutAttributes,
								CssmData *inOutData);
	// CSSM_DL_DataGetNext
	RecordHandle getNextRecord(SearchHandle searchHandle,
							   CssmDbRecordAttributeData *inOutAttributes,
							   CssmData *inOutData);
	// CSSM_DL_DataAbortQuery
	void abortFind(SearchHandle searchHandle);
	// CSSM_DL_DataGetFromUniqueRecordId
	void getRecordFromHandle(RecordHandle record,
							 CssmDbRecordAttributeData *inOutAttributes,
							 CssmData *inOutData);
	// CSSM_DL_FreeUniqueRecord
	void freeRecordHandle(RecordHandle record);

	// database sessions
	DbHandle createDb(const DLDbIdentifier &dbId,
        const AccessCredentials *cred, const AclEntryInput *owner,
        const DBParameters &params);
	DbHandle decodeDb(const DLDbIdentifier &dbId,
        const AccessCredentials *cred, const CssmData &blob);
	void encodeDb(DbHandle db, CssmData &blob, Allocator &alloc);
    void encodeDb(DbHandle db, CssmData &blob) { return encodeDb(db, blob, returnAllocator); }
	void releaseDb(DbHandle db);
	void setDbParameters(DbHandle db, const DBParameters &params);
	void getDbParameters(DbHandle db, DBParameters &params);
	void getDbSuggestedIndex(DbHandle db, CssmData &index, Allocator &alloc);
	void getDbSuggestedIndex(DbHandle db, CssmData &index)
	{ return getDbSuggestedIndex(db, index, returnAllocator); }
    void changePassphrase(DbHandle db, const AccessCredentials *cred);
    void lock(DbHandle db);
    void lockAll(bool forSleep);
    void unlock(DbHandle db);
    void unlock(DbHandle db, const CssmData &passPhrase);
    bool isLocked(DbHandle db);
	
	// key objects
	void encodeKey(KeyHandle key, CssmData &blob, KeyUID *uid, Allocator &alloc);
	void encodeKey(KeyHandle key, CssmData &blob, KeyUID *uid = NULL)
    { return encodeKey(key, blob, uid, returnAllocator); }
	KeyHandle decodeKey(DbHandle db, const CssmData &blob, CssmKey::Header &header);
	void releaseKey(KeyHandle key);

	CssmKeySize queryKeySizeInBits(KeyHandle key);
    uint32 getOutputSize(const Security::Context &context, KeyHandle key,
        uint32 inputSize, bool encrypt = true);
		
	void getKeyDigest(KeyHandle key, CssmData &digest, Allocator &alloc);
	void getKeyDigest(KeyHandle key, CssmData &digest)
	{ return getKeyDigest(key, digest, returnAllocator); }


public:
    // key wrapping and unwrapping
	void wrapKey(const Security::Context &context, KeyHandle key, KeyHandle keyToBeWrapped,
		const AccessCredentials *cred,
		const CssmData *descriptiveData, CssmWrappedKey &wrappedKey, Allocator &alloc);
	void wrapKey(const Security::Context &context, KeyHandle key, KeyHandle keyToBeWrapped,
		const AccessCredentials *cred,
		const CssmData *descriptiveData, CssmWrappedKey &wrappedKey)
    { return wrapKey(context, key, keyToBeWrapped, cred,
        descriptiveData, wrappedKey, returnAllocator); }
    
	void unwrapKey(DbHandle db, const Security::Context &context, KeyHandle key, KeyHandle publicKey,
		const CssmWrappedKey &wrappedKey, uint32 keyUsage, uint32 keyAttr,
		const AccessCredentials *cred, const AclEntryInput *owner,
		CssmData &data, KeyHandle &newKey, CssmKey::Header &newKeyHeader, Allocator &alloc);
	void unwrapKey(DbHandle db, const Security::Context &context, KeyHandle key, KeyHandle publicKey,
		const CssmWrappedKey &wrappedKey, uint32 keyUsage, uint32 keyAttr,
		const AccessCredentials *cred, const AclEntryInput *owner, CssmData &data,
        KeyHandle &newKey, CssmKey::Header &newKeyHeader)
    { return unwrapKey(db, context, key, publicKey, wrappedKey, keyUsage, keyAttr,
      cred, owner, data, newKey, newKeyHeader, returnAllocator); }

    // key generation and derivation
	void generateKey(DbHandle db, const Security::Context &context, uint32 keyUsage, uint32 keyAttr,
		const AccessCredentials *cred, const AclEntryInput *owner,
        KeyHandle &newKey, CssmKey::Header &newHeader);
	void generateKey(DbHandle db, const Security::Context &context,
		uint32 pubKeyUsage, uint32 pubKeyAttr,
		uint32 privKeyUsage, uint32 privKeyAttr,
		const AccessCredentials *cred, const AclEntryInput *owner,
		KeyHandle &pubKey, CssmKey::Header &pubHeader,
        KeyHandle &privKey, CssmKey::Header &privHeader);
	void deriveKey(DbHandle db, const Security::Context &context, KeyHandle baseKey,
        uint32 keyUsage, uint32 keyAttr, CssmData &param,
		const AccessCredentials *cred, const AclEntryInput *owner,
        KeyHandle &newKey, CssmKey::Header &newHeader, Allocator &alloc);
	void deriveKey(DbHandle db, const Security::Context &context, KeyHandle baseKey,
        uint32 keyUsage, uint32 keyAttr, CssmData &param,
		const AccessCredentials *cred, const AclEntryInput *owner,
        KeyHandle &newKey, CssmKey::Header &newHeader)
    { return deriveKey(db, context, baseKey, keyUsage, keyAttr, param, cred, owner, newKey, newHeader, returnAllocator); }
	//void generateAlgorithmParameters();	// not implemented

	void generateRandom(CssmData &data);
	
    // encrypt/decrypt
	void encrypt(const Security::Context &context, KeyHandle key,
        const CssmData &in, CssmData &out, Allocator &alloc);
	void encrypt(const Security::Context &context, KeyHandle key, const CssmData &in, CssmData &out)
    { return encrypt(context, key, in, out, returnAllocator); }
	void decrypt(const Security::Context &context, KeyHandle key,
        const CssmData &in, CssmData &out, Allocator &alloc);
	void decrypt(const Security::Context &context, KeyHandle key, const CssmData &in, CssmData &out)
    { return decrypt(context, key, in, out, returnAllocator); }

    // signatures
	void generateSignature(const Security::Context &context, KeyHandle key,
        const CssmData &data, CssmData &signature, Allocator &alloc,
        CSSM_ALGORITHMS signOnlyAlgorithm = CSSM_ALGID_NONE);
	void generateSignature(const Security::Context &context, KeyHandle key,
		const CssmData &data, CssmData &signature, CSSM_ALGORITHMS signOnlyAlgorithm = CSSM_ALGID_NONE)
    { return generateSignature(context, key, data, signature, returnAllocator, signOnlyAlgorithm); }
	void verifySignature(const Security::Context &context, KeyHandle key,
		const CssmData &data, const CssmData &signature,
        CSSM_ALGORITHMS verifyOnlyAlgorithm = CSSM_ALGID_NONE);
		
    // MACs
	void generateMac(const Security::Context &context, KeyHandle key,
		const CssmData &data, CssmData &mac, Allocator &alloc);
	void generateMac(const Security::Context &context, KeyHandle key,
		const CssmData &data, CssmData &mac)
    { return generateMac(context, key, data, mac, returnAllocator); }
	void verifyMac(const Security::Context &context, KeyHandle key,
		const CssmData &data, const CssmData &mac);
	
    // key ACL management
	void getKeyAcl(KeyHandle key, const char *tag,
        uint32 &count, AclEntryInfo * &info, Allocator &alloc);
	void getKeyAcl(KeyHandle key, const char *tag,
        uint32 &count, AclEntryInfo * &info)
    { return getKeyAcl(key, tag, count, info, returnAllocator); }
	void changeKeyAcl(KeyHandle key, const AccessCredentials &cred, const AclEdit &edit);
	void getKeyOwner(KeyHandle key, AclOwnerPrototype &owner, Allocator &alloc);
	void getKeyOwner(KeyHandle key, AclOwnerPrototype &owner)
    { return getKeyOwner(key, owner, returnAllocator); }
	void changeKeyOwner(KeyHandle key, const AccessCredentials &cred,
		const AclOwnerPrototype &edit);
	
    // database ACL management
	void getDbAcl(DbHandle db, const char *tag,
        uint32 &count, AclEntryInfo * &info, Allocator &alloc);
	void getDbAcl(DbHandle db, const char *tag,
        uint32 &count, AclEntryInfo * &info)
    { return getDbAcl(db, tag, count, info, returnAllocator); }
	void changeDbAcl(DbHandle db, const AccessCredentials &cred, const AclEdit &edit);
	void getDbOwner(DbHandle db, AclOwnerPrototype &owner, Allocator &alloc);
	void getDbOwner(DbHandle db, AclOwnerPrototype &owner)
    { return getDbOwner(db, owner, returnAllocator); }
    void changeDbOwner(DbHandle db, const AccessCredentials &cred,
		const AclOwnerPrototype &edit);
	
	// database key manipulations
	void extractMasterKey(DbHandle db, const Context &context, DbHandle sourceDb,
        uint32 keyUsage, uint32 keyAttr,
		const AccessCredentials *cred, const AclEntryInput *owner,
        KeyHandle &newKey, CssmKey::Header &newHeader, Allocator &alloc);
	void extractMasterKey(DbHandle db, const Context &context, DbHandle sourceDb,
        uint32 keyUsage, uint32 keyAttr,
		const AccessCredentials *cred, const AclEntryInput *owner,
        KeyHandle &newKey, CssmKey::Header &newHeader)
	{ return extractMasterKey(db, context, sourceDb, keyUsage, keyAttr, cred, owner,
		newKey, newHeader, returnAllocator); }
		
public:
	// Authorization API support
	void authCreate(const AuthorizationItemSet *rights,	const AuthorizationItemSet *environment, 
		AuthorizationFlags flags,AuthorizationBlob &result);
	void authRelease(const AuthorizationBlob &auth, AuthorizationFlags flags);
	void authCopyRights(const AuthorizationBlob &auth,
		const AuthorizationItemSet *rights, const AuthorizationItemSet *environment,
		AuthorizationFlags flags, AuthorizationItemSet **result);
	void authCopyInfo(const AuthorizationBlob &auth, const char *tag, AuthorizationItemSet * &info);
	void authExternalize(const AuthorizationBlob &auth, AuthorizationExternalForm &extForm);
	void authInternalize(const AuthorizationExternalForm &extForm, AuthorizationBlob &auth);
    
public:
    // Session API support
    void getSessionInfo(SecuritySessionId &sessionId, SessionAttributeBits &attrs);
    void setupSession(SessionCreationFlags flags, SessionAttributeBits attrs);
    
public:
    // Notification core support
    void requestNotification(Port receiver, NotificationDomain domain, NotificationMask events);
    void stopNotification(Port receiver);
    void postNotification(NotificationDomain domain, NotificationEvent event, const CssmData &data);
    
    typedef OSStatus ConsumeNotification(NotificationDomain domain, NotificationEvent event,
        const void *data, size_t dataLength, void *context);
    OSStatus dispatchNotification(const mach_msg_header_t *message,
        ConsumeNotification *consumer, void *context) throw();

public:
	// AuthorizationDB API
	void authorizationdbGet(const AuthorizationString rightname, CssmData &rightDefinition, Allocator &alloc);
	void authorizationdbSet(const AuthorizationBlob &auth, const AuthorizationString rightname, uint32_t rightdefinitionLength, const void *rightdefinition);
	void authorizationdbRemove(const AuthorizationBlob &auth, const AuthorizationString rightname);
	
public:
	// miscellaneous administrative calls
	void addCodeEquivalence(const CssmData &oldCode, const CssmData &newCode,
		const char *name, bool forSystem = false);
	void removeCodeEquivalence(const CssmData &code, const char *name, bool forSystem = false);
	void setAlternateSystemRoot(const char *path);

private:
	void getAcl(AclKind kind, KeyHandle key, const char *tag,
		uint32 &count, AclEntryInfo * &info, Allocator &alloc);
	void changeAcl(AclKind kind, KeyHandle key,
		const AccessCredentials &cred, const AclEdit &edit);
	void getOwner(AclKind kind, KeyHandle key, AclOwnerPrototype &owner, Allocator &alloc);
	void changeOwner(AclKind kind, KeyHandle key, const AccessCredentials &cred,
		const AclOwnerPrototype &edit);

	void addApplicationAclSubject(KeyHandle key, CSSM_ACL_AUTHORIZATION_TAG tag);

	void returnAttrsAndData(CssmDbRecordAttributeData *inOutAttributes,
		CssmDbRecordAttributeData *attrs, CssmDbRecordAttributeData *attrsBase, mach_msg_type_number_t attrsLength,
		CssmData *inOutData, void *dataPtr, mach_msg_type_number_t dataLength);
private:
	DidChangeKeyAclCallback *mCallback;
	void *mCallbackContext;

	static UnixPlusPlus::StaticForkMonitor mHasForked;	// global fork indicator

	struct Thread {
		Thread() : registered(false) { }
		operator bool() const { return registered; }
		
		ReceivePort replyPort;	// dedicated reply port (send right held by SecurityServer)
        bool registered;		// has been registered with SecurityServer
	};

	struct Global {
        Global();
		Port serverPort;
		RefPointer<CodeSigning::OSXCode> myself;
		ThreadNexus<Thread> thread;
	};

	static ModuleNexus<Global> mGlobal;
	static bool mSetupSession;
	static const char *mContactName;
};


} // end namespace SecurityServer
} // end namespace Security


#endif //_H_SSCLIENT
