/*
 * Copyright (c) 2002-2004 Apple Computer, Inc. All Rights Reserved.
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
 *
 * IdentityCursor.cpp -- Working with IdentityCursor
 */

#include <security_keychain/IdentityCursor.h>
#include <security_keychain/Identity.h>
#include <security_keychain/Trust.h>
#include <security_keychain/Item.h>
#include <security_keychain/Certificate.h>
#include <security_keychain/KeyItem.h>
#include <security_cdsa_utilities/Schema.h>
#include <security_cdsa_utilities/KeySchema.h>
#include <Security/oidsalg.h>

using namespace KeychainCore;

IdentityCursorPolicyAndID::IdentityCursorPolicyAndID(const StorageManager::KeychainList &searchList, CSSM_KEYUSE keyUsage, CFStringRef idString, SecPolicyRef policy, bool returnOnlyValidIdentities) :
	IdentityCursor(searchList, keyUsage),
	mPolicy(policy),
	mIDString(idString),
	mReturnOnlyValidIdentities(returnOnlyValidIdentities)
{
}

bool
IdentityCursorPolicyAndID::next(SecPointer<Identity> &identity)
{
	SecPointer<Identity> currIdentity;
	Boolean identityOK = true;
	for (;;)
	{
		bool result = IdentityCursor::next(currIdentity);   // base class finds the next identity by keyUsage
		if ( result )
		{
			// To reduce the number of trust evaluation calls for the cert, we need
			// to do some pre-preocessing to weed-out certs that don't match the search criteria.
			//
			
			//%%%for better performance, if 'mReturnOnlyValidIdentities' is set, 
			// do some special processing with the CL to weed out invalid certs
			
			SecPointer<Certificate> certificate = currIdentity->certificate();
			CSSM_OID policyOID;
			SecPolicyGetOID(mPolicy, &policyOID);//%%%err? %%%use Policy object
			//
			// Deal with Policy to determine what the idString means for the search.
			// Check the subjectAltName field if we're dealing with the the S/MIME policy.
			// Check the 'otherName' field of the cert if we're dealing with the AppleID policy.
			//
			CFComparisonResult compareResult = kCFCompareGreaterThan;  // Assume mismatch.
			if ( 0/*CSSMOID_APPLE_TP_APPLEID ?*/ && mIDString )
			{
				// For AppleID policy, we check the otherName (already part of cStruct?)
				// portion of the certificate to look for ID string.
				// or maybe in the 'alis' attr or other as defined by Michael?
			}
			else
			{
				CssmOid oid1 = CssmOid::overlay(CSSMOID_APPLE_TP_SMIME);
				CssmOid oid2 = CssmOid::overlay(policyOID);
				if ( oid1 == oid2 )
				{
					// For S/MIME policy, we check the subjectAltName RFC 822 Name (1 or more email addresses)
					// portion of the certificate and compare against mIDString.
					//
					CFArrayRef emailAddressArray = NULL;
					emailAddressArray = certificate->copyEmailAddresses();
					if ( !emailAddressArray )
					{
						identityOK = false;
						break;
					}
					else
					{
						short arrayIndex = 0;
						CFIndex emailArraySize = CFArrayGetCount(emailAddressArray);
						for (arrayIndex = 0;arrayIndex < emailArraySize; arrayIndex++)
						{
							CFStringRef compareToStr = (CFStringRef)CFArrayGetValueAtIndex(emailAddressArray, arrayIndex);
							compareResult = CFStringCompare(mIDString, compareToStr, 0);
							if ( compareResult == kCFCompareEqualTo )
								break;  // break from array loop; we found it.
						}
						CFRelease(emailAddressArray);
					}
				}
				else
					;//%%%other policies: we do what?
			}
			if ( compareResult == kCFCompareEqualTo )
			{
				// Perform trust evaluation if they wanted and if they specified a policy.
				//
				if ( mReturnOnlyValidIdentities && mPolicy )
				{
					// Perform the trust evaluation on the cert with the specified policy.
					//
					CFMutableArrayRef certArray = CFArrayCreateMutable(NULL, 1, NULL);
					if ( !certArray )
					{
						identityOK = false; 
						break;  // can't create the cert array for evaluation.
					}
					CFArrayAppendValue(certArray, (void*)(certificate->handle()));
					Trust* trust = new Trust(certArray, mPolicy);
					trust->evaluate();
					SecTrustResultType trustResult = trust->result();
					delete trust;
					CFRelease(certArray);   // done with cert array
					if ( trustResult == kSecTrustResultInvalid )
						identityOK = false; // nope, on to the next one.
					else
						break;  // this one was OK! break from for(;;)
					//
					// Trust evaluated OK...
				}
			}
		}
		else
		{
			identityOK = false;
			break;  // no more left.
		}
	}   // for
	
	if ( identityOK )
	{
		identity = currIdentity;//%%%they release?<currIdentity is autoreleased?!>
		return true;
	}
	else
		return false;
}


IdentityCursor::IdentityCursor(const StorageManager::KeychainList &searchList, CSSM_KEYUSE keyUsage) :
	mSearchList(searchList),
	mKeyCursor(mSearchList, CSSM_DL_DB_RECORD_PRIVATE_KEY, NULL)
{
	// If keyUsage is CSSM_KEYUSE_ANY then we need a key that can do everything
	if (keyUsage & CSSM_KEYUSE_ANY)
		keyUsage = CSSM_KEYUSE_ENCRYPT | CSSM_KEYUSE_DECRYPT
						   | CSSM_KEYUSE_DERIVE | CSSM_KEYUSE_SIGN
						   | CSSM_KEYUSE_VERIFY | CSSM_KEYUSE_SIGN_RECOVER
						   | CSSM_KEYUSE_VERIFY_RECOVER | CSSM_KEYUSE_WRAP
						   | CSSM_KEYUSE_UNWRAP;

	if (keyUsage & CSSM_KEYUSE_ENCRYPT)
		mKeyCursor->add(CSSM_DB_EQUAL, KeySchema::Encrypt, true);
	if (keyUsage & CSSM_KEYUSE_DECRYPT)
		mKeyCursor->add(CSSM_DB_EQUAL, KeySchema::Decrypt, true);
	if (keyUsage & CSSM_KEYUSE_DERIVE)
		mKeyCursor->add(CSSM_DB_EQUAL, KeySchema::Derive, true);
	if (keyUsage & CSSM_KEYUSE_SIGN)
		mKeyCursor->add(CSSM_DB_EQUAL, KeySchema::Sign, true);
	if (keyUsage & CSSM_KEYUSE_VERIFY)
		mKeyCursor->add(CSSM_DB_EQUAL, KeySchema::Verify, true);
	if (keyUsage & CSSM_KEYUSE_SIGN_RECOVER)
		mKeyCursor->add(CSSM_DB_EQUAL, KeySchema::SignRecover, true);
	if (keyUsage & CSSM_KEYUSE_VERIFY_RECOVER)
		mKeyCursor->add(CSSM_DB_EQUAL, KeySchema::VerifyRecover, true);
	if (keyUsage & CSSM_KEYUSE_WRAP)
		mKeyCursor->add(CSSM_DB_EQUAL, KeySchema::Wrap, true);
	if (keyUsage & CSSM_KEYUSE_UNWRAP)
		mKeyCursor->add(CSSM_DB_EQUAL, KeySchema::Unwrap, true);
}

IdentityCursor::~IdentityCursor() throw()
{
}

bool
IdentityCursor::next(SecPointer<Identity> &identity)
{
	for (;;)
	{
		if (!mCertificateCursor)
		{
			Item key;
			if (!mKeyCursor->next(key))
				return false;
	
			mCurrentKey = static_cast<KeyItem *>(key.get());

			CssmClient::DbUniqueRecord uniqueId = mCurrentKey->dbUniqueRecord();
			CssmClient::DbAttributes dbAttributes(uniqueId->database(), 1);
			dbAttributes.add(KeySchema::Label);
			uniqueId->get(&dbAttributes, NULL);
			const CssmData &keyHash = dbAttributes[0];

			mCertificateCursor = KCCursor(mSearchList, CSSM_DL_DB_RECORD_X509_CERTIFICATE, NULL);
			mCertificateCursor->add(CSSM_DB_EQUAL, Schema::kX509CertificatePublicKeyHash, keyHash);
		}
	
		Item cert;
		if (mCertificateCursor->next(cert))
		{
			SecPointer<Certificate> certificate(static_cast<Certificate *>(cert.get()));
			identity = new Identity(mCurrentKey, certificate);
			return true;
		}
		else
			mCertificateCursor = KCCursor();
	}
}
