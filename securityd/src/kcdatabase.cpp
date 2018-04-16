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
// kcdatabase - software database container implementation.
//
// General implementation notes:
// This leverages LocalDatabase/LocalKey for cryptography, and adds the
//  storage coder/decoder logic that implements "keychain" databases in their
//  intricately choreographed dance between securityd and the AppleCSPDL.
// As always, Database objects are lifetime-bound to their Process referent;
//  they can also be destroyed explicitly with a client release call.
// DbCommons are reference-held by their Databases, with one extra special
//  reference (from the Session) introduced when the database unlocks, and
//  removed when it locks again. That way, an unused DbCommon dies when it
//  is locked or when the Session dies, whichever happens earlier.
// There is (as yet) no global-scope Database object for Keychain databases.
//
#include "kcdatabase.h"
#include "agentquery.h"
#include "kckey.h"
#include "server.h"
#include "session.h"
#include "notifications.h"
#include <security_agent_client/agentclient.h>
#include <security_cdsa_utilities/acl_any.h>	// for default owner ACLs
#include <security_cdsa_client/wrapkey.h>
#include <security_cdsa_client/genkey.h>
#include <security_cdsa_client/signclient.h>
#include <security_cdsa_client/cryptoclient.h>
#include <security_cdsa_client/macclient.h>
#include <securityd_client/dictionary.h>
#include <security_utilities/endian.h>


//
// Create a Database object from initial parameters (create operation)
//
KeychainDatabase::KeychainDatabase(const DLDbIdentifier &id, const DBParameters &params, Process &proc,
            const AccessCredentials *cred, const AclEntryPrototype *owner)
    : LocalDatabase(proc), mValidData(false), version(0), mBlob(NULL)
{
    // save a copy of the credentials for later access control
    mCred = DataWalkers::copy(cred, Allocator::standard());

    // create a new random signature to complete the DLDbIdentifier
    DbBlob::Signature newSig;
    Server::active().random(newSig.bytes);
    DbIdentifier ident(id, newSig);
	
    // create common block and initialize
	RefPointer<KeychainDbCommon> newCommon = new KeychainDbCommon(proc.session(), ident);
	StLock<Mutex> _(*newCommon);
	parent(*newCommon);
	// new common is now visible (in ident-map) but we hold its lock

	// establish the new master secret
	establishNewSecrets(cred, SecurityAgent::newDatabase);
	
	// set initial database parameters
	common().mParams = params;
		
	// the common is "unlocked" now
	common().makeNewSecrets();

	// establish initial ACL
	if (owner)
		cssmSetInitial(*owner);
	else
		cssmSetInitial(new AnyAclSubject());
    mValidData = true;
    
    // for now, create the blob immediately
    encode();
    
	proc.addReference(*this);
	secdebug("SSdb", "database %s(%p) created, common at %p",
		common().dbName(), this, &common());
}


//
// Create a Database object from a database blob (decoding)
//
KeychainDatabase::KeychainDatabase(const DLDbIdentifier &id, const DbBlob *blob, Process &proc,
    const AccessCredentials *cred)
	: LocalDatabase(proc), mValidData(false), version(0)
{
    // perform basic validation on the incoming blob
	assert(blob);
    blob->validate(CSSMERR_APPLEDL_INVALID_DATABASE_BLOB);
    switch (blob->version()) {
#if defined(COMPAT_OSX_10_0)
    case blob->version_MacOS_10_0:
        break;
#endif
    case blob->version_MacOS_10_1:
        break;
    default:
        CssmError::throwMe(CSSMERR_APPLEDL_INCOMPATIBLE_DATABASE_BLOB);
    }

    // save a copy of the credentials for later access control
    mCred = DataWalkers::copy(cred, Allocator::standard());
    mBlob = blob->copy();
    
    // check to see if we already know about this database
    DbIdentifier ident(id, blob->randomSignature);
	Session &session = process().session();
	StLock<Mutex> _(session);
	if (KeychainDbCommon *dbcom =
			session.findFirst<KeychainDbCommon, const DbIdentifier &>(&KeychainDbCommon::identifier, ident)) {
		parent(*dbcom);
		//@@@ arbitrate sequence number here, perhaps update common().mParams
		secdebug("SSdb",
			"open database %s(%p) version %lx at known common %p",
			common().dbName(), this, blob->version(), &common());
	} else {
		// DbCommon not present; make a new one
		parent(*new KeychainDbCommon(proc.session(), ident));
		common().mParams = blob->params;
		secdebug("SSdb", "open database %s(%p) version %lx with new common %p",
			common().dbName(), this, blob->version(), &common());
	}
	proc.addReference(*this);
}


//
// Destroy a Database
//
KeychainDatabase::~KeychainDatabase()
{
    secdebug("SSdb", "deleting database %s(%p) common %p",
        common().dbName(), this, &common());
    Allocator::standard().free(mCred);
	Allocator::standard().free(mBlob);
}


//
// Basic Database virtual implementations
//
KeychainDbCommon &KeychainDatabase::common() const
{
	return parent<KeychainDbCommon>();
}

const char *KeychainDatabase::dbName() const
{
	return common().dbName();
}


static inline KeychainKey &myKey(Key *key)
{
	return *safe_cast<KeychainKey *>(key);
}


//
// (Re-)Authenticate the database. This changes the stored credentials.
//
void KeychainDatabase::authenticate(const AccessCredentials *cred)
{
	StLock<Mutex> _(common());
	AccessCredentials *newCred = DataWalkers::copy(cred, Allocator::standard());
    Allocator::standard().free(mCred);
    mCred = newCred;
}


//
// Make a new KeychainKey
//
RefPointer<Key> KeychainDatabase::makeKey(const CssmKey &newKey,
	uint32 moreAttributes, const AclEntryPrototype *owner)
{
	return new KeychainKey(*this, newKey, moreAttributes, owner);
}


//
// Return the database blob, recalculating it as needed.
//
DbBlob *KeychainDatabase::blob()
{
	StLock<Mutex> _(common());
    if (!validBlob()) {
        makeUnlocked();			// unlock to get master secret
		encode();				// (re)encode blob if needed
    }
    activity();					// reset timeout
	assert(validBlob());		// better have a valid blob now...
    return mBlob;
}


//
// Encode the current database as a blob.
// Note that this returns memory we own and keep.
// Caller must hold common lock.
//
void KeychainDatabase::encode()
{
	DbBlob *blob = common().encode(*this);
	Allocator::standard().free(mBlob);
	mBlob = blob;
	version = common().version;
	secdebug("SSdb", "encoded database %p common %p(%s) version %ld params=(%ld,%d)",
		this, &common(), dbName(), version,
		common().mParams.idleTimeout, common().mParams.lockOnSleep);
}


//
// Change the passphrase on a database
//
void KeychainDatabase::changePassphrase(const AccessCredentials *cred)
{
	// get and hold the common lock (don't let other threads break in here)
	StLock<Mutex> _(common());
	
	// establish OLD secret - i.e. unlock the database
	//@@@ do we want to leave the final lock state alone?
	makeUnlocked(cred);
	
    // establish NEW secret
	establishNewSecrets(cred, SecurityAgent::changePassphrase);
	common().version++;	// blob state changed
	secdebug("SSdb", "Database %s(%p) master secret changed", common().dbName(), this);
	encode();			// force rebuild of local blob
	
	// send out a notification
	notify(kNotificationEventPassphraseChanged);

    // I guess this counts as an activity
    activity();
}


//
// Extract the database master key as a proper Key object.
//
RefPointer<Key> KeychainDatabase::extractMasterKey(Database &db,
	const AccessCredentials *cred, const AclEntryPrototype *owner,
	uint32 usage, uint32 attrs)
{
	// get and hold common lock
	StLock<Mutex> _(common());
	
	// unlock to establish master secret
	makeUnlocked(cred);
	
	// extract the raw cryptographic key
	CssmClient::WrapKey wrap(Server::csp(), CSSM_ALGID_NONE);
	CssmKey key;
	wrap(common().masterKey(), key);
	
	// make the key object and return it
	return new KeychainKey(db, key, attrs & Key::managedAttributes, owner);
}


//
// Construct a binary blob of moderate size that is suitable for constructing
// an index identifying this database.
// We construct this from the database's marker blob, which is created with
// the database is made, and stays stable thereafter.
// Note: Ownership of the index blob passes to the caller.
// @@@ This means that physical copies share this index.
//
void KeychainDatabase::getDbIndex(CssmData &indexData)
{
	if (!mBlob)
		encode();	// force blob creation
	assert(mBlob);
	CssmData signature = CssmData::wrap(mBlob->randomSignature);
	indexData = CssmAutoData(Allocator::standard(), signature).release();
}


//
// Unlock this database (if needed) by obtaining the master secret in some
// suitable way and then proceeding to unlock with it.
// Does absolutely nothing if the database is already unlocked.
// The makeUnlocked forms are identical except the assume the caller already
// holds the common lock.
//
void KeychainDatabase::unlockDb()
{
	StLock<Mutex> _(common());
	makeUnlocked();
}

void KeychainDatabase::makeUnlocked()
{
	return makeUnlocked(mCred);
}

void KeychainDatabase::makeUnlocked(const AccessCredentials *cred)
{
    if (isLocked()) {
		secdebug("SSdb", "%p(%p) unlocking for makeUnlocked()", this, &common());
        assert(mBlob || (mValidData && common().hasMaster()));
		establishOldSecrets(cred);
		common().setUnlocked(); // mark unlocked
		activity();				// set timeout timer
	} else if (!mValidData)	{	// need to decode to get our ACLs, master secret available
		secdebug("SSdb", "%p(%p) is unlocked; decoding for makeUnlocked()", this, &common());
		if (!decode())
			CssmError::throwMe(CSSM_ERRCODE_OPERATION_AUTH_DENIED);
	}
	assert(!isLocked());
	assert(mValidData);
}


//
// The following unlock given an explicit passphrase, rather than using
// (special cred sample based) default procedures.
//
void KeychainDatabase::unlockDb(const CssmData &passphrase)
{
	StLock<Mutex> _(common());
	makeUnlocked(passphrase);
}

void KeychainDatabase::makeUnlocked(const CssmData &passphrase)
{
	if (isLocked()) {
		if (decode(passphrase))
			return;
		else
			CssmError::throwMe(CSSM_ERRCODE_OPERATION_AUTH_DENIED);
	} else if (!mValidData)	{	// need to decode to get our ACLs, passphrase available
		if (!decode())
			CssmError::throwMe(CSSM_ERRCODE_OPERATION_AUTH_DENIED);
	}
	assert(!isLocked());
	assert(mValidData);
}


//
// Nonthrowing passphrase-based unlock. This returns false if unlock failed.
// Note that this requires an explicitly given passphrase.
// Caller must hold common lock.
//
bool KeychainDatabase::decode(const CssmData &passphrase)
{
	assert(mBlob);
	common().setup(mBlob, passphrase);
	return decode();
}


//
// Given the established master secret, decode the working keys and other
// functional secrets for this database. Return false (do NOT throw) if
// the decode fails. Call this in low(er) level code once you established
// the master key.
//
bool KeychainDatabase::decode()
{
	assert(mBlob);
	assert(common().hasMaster());
	void *privateAclBlob;
	if (common().unlockDb(mBlob, &privateAclBlob)) {
		if (!mValidData) {
			importBlob(mBlob->publicAclBlob(), privateAclBlob);
			mValidData = true;
		}
		Allocator::standard().free(privateAclBlob);
		return true;
	}
	secdebug("SSdb", "%p decode failed", this);
	return false;
}


//
// Given an AccessCredentials for this database, wring out the existing primary
// database secret by whatever means necessary.
// On entry, caller must hold the database common lock. It will be held throughout.
// On exit, the crypto core has its master secret. If things go wrong,
// we will throw a suitable exception. Note that encountering any malformed
// credential sample will throw, but this is not guaranteed -- don't assume
// that NOT throwing means creds is entirely well-formed (it may just be good
// enough to work THIS time).
//
// How this works:
// Walk through the creds. Fish out those credentials (in order) that
// are for unlock processing (they have no ACL subject correspondents),
// and (try to) obey each in turn, until one produces a valid secret
// or you run out. If no special samples are found at all, interpret that as
// "use the system global default," which happens to be hard-coded right here.
//
void KeychainDatabase::establishOldSecrets(const AccessCredentials *creds)
{
	list<CssmSample> samples;
	if (creds && creds->samples().collect(CSSM_SAMPLE_TYPE_KEYCHAIN_LOCK, samples)) {
		for (list<CssmSample>::iterator it = samples.begin(); it != samples.end(); it++) {
			TypedList &sample = *it;
			sample.checkProper();
			switch (sample.type()) {
			// interactively prompt the user - no additional data
			case CSSM_SAMPLE_TYPE_KEYCHAIN_PROMPT:
				{
				secdebug("SSdb", "%p attempting interactive unlock", this);
				QueryUnlock query(*this);
				query.inferHints(Server::process());
				if (query() == SecurityAgent::noReason)
					return;
				}
				break;
			// try to use an explicitly given passphrase - Data:passphrase
			case CSSM_SAMPLE_TYPE_PASSWORD:
				if (sample.length() != 2)
					CssmError::throwMe(CSSM_ERRCODE_INVALID_SAMPLE_VALUE);
				secdebug("SSdb", "%p attempting passphrase unlock", this);
				if (decode(sample[1]))
					return;
				break;
			// try to open with a given master key - Data:CSP or KeyHandle, Data:CssmKey
			case CSSM_WORDID_SYMMETRIC_KEY:
				assert(mBlob);
				secdebug("SSdb", "%p attempting explicit key unlock", this);
				common().setup(mBlob, keyFromCreds(sample));
				if (decode())
					return;
				break;
			// explicitly defeat the default action but don't try anything in particular
			case CSSM_WORDID_CANCELED:
				secdebug("SSdb", "%p defeat default action", this);
				break;
			default:
				// Unknown sub-sample for unlocking.
				// If we wanted to be fascist, we could now do
				//  CssmError::throwMe(CSSM_ERRCODE_SAMPLE_VALUE_NOT_SUPPORTED);
				// But instead we try to be tolerant and continue on.
				// This DOES however count as an explicit attempt at specifying unlock,
				// so we will no longer try the default case below...
				secdebug("SSdb", "%p unknown sub-sample unlock (%ld) ignored", this, sample.type());
				break;
			}
		}
	} else {
		// default action
		assert(mBlob);
		
		// attempt system-keychain unlock
		SystemKeychainKey systemKeychain(kSystemUnlockFile);
		if (systemKeychain.matches(mBlob->randomSignature)) {
			secdebug("SSdb", "%p attempting system unlock", this);
			common().setup(mBlob, CssmClient::Key(Server::csp(), systemKeychain.key(), true));
			if (decode())
				return;
		}
		
		// attempt interactive unlock
		QueryUnlock query(*this);
		query.inferHints(Server::process());
		if (query() == SecurityAgent::noReason)
			return;
	}
	
	// out of options - no secret obtained
	CssmError::throwMe(CSSM_ERRCODE_OPERATION_AUTH_DENIED);
}


//
// Same thing, but obtain a new secret somehow and set it into the common.
//
void KeychainDatabase::establishNewSecrets(const AccessCredentials *creds, SecurityAgent::Reason reason)
{
	list<CssmSample> samples;
	if (creds && creds->samples().collect(CSSM_SAMPLE_TYPE_KEYCHAIN_CHANGE_LOCK, samples)) {
		for (list<CssmSample>::iterator it = samples.begin(); it != samples.end(); it++) {
			TypedList &sample = *it;
			sample.checkProper();
			switch (sample.type()) {
			// interactively prompt the user
			case CSSM_SAMPLE_TYPE_KEYCHAIN_PROMPT:
				{
				secdebug("SSdb", "%p specified interactive passphrase", this);
				QueryNewPassphrase query(*this, reason);
				query.inferHints(Server::process());
				CssmAutoData passphrase(Allocator::standard(Allocator::sensitive));
				if (query(passphrase) == SecurityAgent::noReason) {
					common().setup(NULL, passphrase);
					return;
				}
				}
				break;
			// try to use an explicitly given passphrase
			case CSSM_SAMPLE_TYPE_PASSWORD:
				secdebug("SSdb", "%p specified explicit passphrase", this);
				if (sample.length() != 2)
					CssmError::throwMe(CSSM_ERRCODE_INVALID_SAMPLE_VALUE);
				common().setup(NULL, sample[1]);
				return;
			// try to open with a given master key
			case CSSM_WORDID_SYMMETRIC_KEY:
				secdebug("SSdb", "%p specified explicit master key", this);
				common().setup(NULL, keyFromCreds(sample));
				return;
			// explicitly defeat the default action but don't try anything in particular
			case CSSM_WORDID_CANCELED:
				secdebug("SSdb", "%p defeat default action", this);
				break;
			default:
				// Unknown sub-sample for acquiring new secret.
				// If we wanted to be fascist, we could now do
				//  CssmError::throwMe(CSSM_ERRCODE_SAMPLE_VALUE_NOT_SUPPORTED);
				// But instead we try to be tolerant and continue on.
				// This DOES however count as an explicit attempt at specifying unlock,
				// so we will no longer try the default case below...
				secdebug("SSdb", "%p unknown sub-sample acquisition (%ld) ignored",
					this, sample.type());
				break;
			}
		}
	} else {
		// default action -- interactive (only)
		QueryNewPassphrase query(*this, reason);
        query.inferHints(Server::process());
		CssmAutoData passphrase(Allocator::standard(Allocator::sensitive));
		if (query(passphrase) == SecurityAgent::noReason) {
			common().setup(NULL, passphrase);
			return;
		}
	}
	
	// out of options - no secret obtained
	CssmError::throwMe(CSSM_ERRCODE_OPERATION_AUTH_DENIED);
}


//
// Given a (truncated) Database credentials TypedList specifying a master key,
// locate the key and return a reference to it.
//
CssmClient::Key KeychainDatabase::keyFromCreds(const TypedList &sample)
{
	// decode TypedList structure (sample type; Data:CSPHandle; Data:CSSM_KEY)
	assert(sample.type() == CSSM_WORDID_SYMMETRIC_KEY);
	if (sample.length() != 3
		|| sample[1].type() != CSSM_LIST_ELEMENT_DATUM
		|| sample[2].type() != CSSM_LIST_ELEMENT_DATUM)
			CssmError::throwMe(CSSM_ERRCODE_INVALID_SAMPLE_VALUE);
	CSSM_CSP_HANDLE &handle = *sample[1].data().interpretedAs<CSSM_CSP_HANDLE>(CSSM_ERRCODE_INVALID_SAMPLE_VALUE);
	CssmKey &key = *sample[2].data().interpretedAs<CssmKey>(CSSM_ERRCODE_INVALID_SAMPLE_VALUE);

	if (key.header().cspGuid() == gGuidAppleCSPDL) {
		// handleOrKey is a SecurityServer KeyHandle; ignore key argument
		return myKey(Server::key(handle));
	} else {
		// not a KeyHandle reference; use key as a raw key
		if (key.header().blobType() != CSSM_KEYBLOB_RAW)
			CssmError::throwMe(CSSMERR_CSP_INVALID_KEY_REFERENCE);
		if (key.header().keyClass() != CSSM_KEYCLASS_SESSION_KEY)
			CssmError::throwMe(CSSMERR_CSP_INVALID_KEY_CLASS);
		return CssmClient::Key(Server::csp(), key, true);
	}
}


//
// Verify a putative database passphrase.
// If the database is already unlocked, just check the passphrase.
// Otherwise, unlock with that passphrase and report success.
// Caller must hold the common lock.
//
bool KeychainDatabase::validatePassphrase(const CssmData &passphrase) const
{
	if (common().hasMaster()) {
		// verify against known secret
		return common().validatePassphrase(passphrase);
	} else {
		// no master secret - perform "blind" unlock to avoid actual unlock
		try {
			DatabaseCryptoCore test;
			test.setup(mBlob, passphrase);
			test.decodeCore(mBlob, NULL);
			return true;
		} catch (...) {
			return false;
		}
	}
}


//
// Lock this database
//
void KeychainDatabase::lockDb()
{
    common().lockDb();
}


//
// Given a Key for this database, encode it into a blob and return it.
//
KeyBlob *KeychainDatabase::encodeKey(const CssmKey &key, const CssmData &pubAcl, const CssmData &privAcl)
{
    unlockDb();
    
    // tell the cryptocore to form the key blob
    return common().encodeKeyCore(key, pubAcl, privAcl);
}


//
// Given a "blobbed" key for this database, decode it into its real
// key object and (re)populate its ACL.
//
void KeychainDatabase::decodeKey(KeyBlob *blob, CssmKey &key, void * &pubAcl, void * &privAcl)
{
    unlockDb();							// we need our keys

    common().decodeKeyCore(blob, key, pubAcl, privAcl);
    // memory protocol: pubAcl points into blob; privAcl was allocated
	
    activity();
}


//
// Modify database parameters
//
void KeychainDatabase::setParameters(const DBParameters &params)
{
	StLock<Mutex> _(common());
    makeUnlocked();
	common().mParams = params;
    common().version++;		// invalidate old blobs
    activity();
	secdebug("SSdb", "%p common %p(%s) set params=(%ld,%d)",
		this, &common(), dbName(), params.idleTimeout, params.lockOnSleep);
}


//
// Retrieve database parameters
//
void KeychainDatabase::getParameters(DBParameters &params)
{
	StLock<Mutex> _(common());
    makeUnlocked();
	params = common().mParams;
    //activity();		// getting parameters does not reset the idle timer
}


//
// Intercept ACL change requests and reset blob validity
//
void KeychainDatabase::instantiateAcl()
{
	StLock<Mutex> _(common());
	makeUnlocked();
}

void KeychainDatabase::changedAcl()
{
	StLock<Mutex> _(common());
	version = 0;
}

const Database *KeychainDatabase::relatedDatabase() const
{ return this; }


//
// Debugging support
//
#if defined(DEBUGDUMP)

void KeychainDbCommon::dumpNode()
{
	PerSession::dumpNode();
	uint32 sig; memcpy(&sig, &mIdentifier.signature(), sizeof(sig));
	Debug::dump(" %s[%8.8lx]", mIdentifier.dbName(), sig);
	if (isLocked()) {
		Debug::dump(" locked");
	} else {
		time_t whenTime = time_t(when());
		Debug::dump(" unlocked(%24.24s/%.2g)", ctime(&whenTime),
			(when() - Time::now()).seconds());
	}
	Debug::dump(" params=(%ld,%d)", mParams.idleTimeout, mParams.lockOnSleep);
}

void KeychainDatabase::dumpNode()
{
	PerProcess::dumpNode();
	Debug::dump(" %s vers=%ld",
		mValidData ? " data" : " nodata", version);
	if (mBlob) {
		uint32 sig; memcpy(&sig, &mBlob->randomSignature, sizeof(sig));
		Debug::dump(" blob=%p[%8.8lx]", mBlob, sig);
	} else {
		Debug::dump(" noblob");
	}
}

#endif //DEBUGDUMP


//
// DbCommon basic features
//
KeychainDbCommon::KeychainDbCommon(Session &ssn, const DbIdentifier &id)
	: DbCommon(ssn), mIdentifier(id), sequence(0), version(1),
      mIsLocked(true), mValidParams(false)
{ }

KeychainDbCommon::~KeychainDbCommon()
{
	secdebug("SSdb", "DbCommon %p destroyed", this);

	// explicitly unschedule ourselves
	Server::active().clearTimer(this);
}


void KeychainDbCommon::makeNewSecrets()
{
	// we already have a master key (right?)
	assert(hasMaster());

	// tell crypto core to generate the use keys
	DatabaseCryptoCore::generateNewSecrets();
	
	// we're now officially "unlocked"; set the timer
	setUnlocked();
	activity();			// set lock timer
}


//
// All unlocking activity ultimately funnels through this method.
// This unlocks a DbCommon using the secrets setup in its crypto core
// component, and performs all the housekeeping needed to represent
// the state change.
// Returns true if unlock was successful, false if it failed due to
// invalid/insufficient secrets. Throws on other errors.
//
bool KeychainDbCommon::unlockDb(DbBlob *blob, void **privateAclBlob)
{
	try {
		// Tell the cryptocore to (try to) decode itself. This will fail
		// in an astonishing variety of ways if the passphrase is wrong.
		assert(hasMaster());
		decodeCore(blob, privateAclBlob);
		secdebug("SSdb", "%p unlock successful", this);
	} catch (...) {
		secdebug("SSdb", "%p unlock failed", this);
		return false;
	}
	
	// get the database parameters only if we haven't got them yet
	if (!mValidParams) {
		mParams = blob->params;
		n2hi(mParams.idleTimeout);
		mValidParams = true;	// sticky
	}

	setUnlocked();		// mark unlocked
	activity();			// set lock timer
	
	// broadcast unlock notification
	notify(kNotificationEventUnlocked);
    return true;
}

void KeychainDbCommon::setUnlocked()
{
	session().addReference(*this); // active/held
	mIsLocked = false;  // mark unlocked
}


void KeychainDbCommon::lockDb(bool forSleep)
{
    StLock<Mutex> locker(*this);
    if (!isLocked()) {
        if (forSleep && !mParams.lockOnSleep)
            return;	// it doesn't want to

		DatabaseCryptoCore::invalidate();
        notify(kNotificationEventLocked);
		Server::active().clearTimer(this);

		mIsLocked = true;		// mark locked
		locker.unlock();		// release DbCommon lock now
		session().removeReference(*this); // remove active/hold
    }
}


DbBlob *KeychainDbCommon::encode(KeychainDatabase &db)
{
    assert(!isLocked());	// must have been unlocked by caller
    
    // export database ACL to blob form
    CssmData pubAcl, privAcl;
    db.exportBlob(pubAcl, privAcl);
    
    // tell the cryptocore to form the blob
    DbBlob form;
    form.randomSignature = identifier();
    form.sequence = sequence;
    form.params = mParams;
	h2ni(form.params.idleTimeout);
	
	assert(hasMaster());
    DbBlob *blob = encodeCore(form, pubAcl, privAcl);
    
    // clean up and go
    db.allocator.free(pubAcl);
    db.allocator.free(privAcl);
	return blob;
}


//
// Send a keychain-related notification event about this keychain
//
void KeychainDbCommon::notify(NotificationEvent event)
{
	// form the data (encoded DLDbIdentifier)
    NameValueDictionary nvd;
    NameValueDictionary::MakeNameValueDictionaryFromDLDbIdentifier(identifier(), nvd);
    CssmData data;
    nvd.Export(data);

	// inject notification into Security event system
    Listener::notify(kNotificationDomainDatabase, event, data);
	
	// clean up
    free (data.data());
}


//
// Perform deferred lock processing for a database.
//
void KeychainDbCommon::action()
{
	secdebug("SSdb", "common %s(%p) locked by timer", dbName(), this);
	lockDb();
}

void KeychainDbCommon::activity()
{
    if (!isLocked()) {
		secdebug("SSdb", "setting DbCommon %p timer to %d",
			this, int(mParams.idleTimeout));
		Server::active().setTimer(this, Time::Interval(int(mParams.idleTimeout)));
	}
}

void KeychainDbCommon::sleepProcessing()
{
	lockDb(true);
}
