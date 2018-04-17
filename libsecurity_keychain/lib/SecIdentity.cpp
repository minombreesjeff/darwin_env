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
 */

#include <Security/SecIdentity.h>
#include <Security/SecIdentityPriv.h>

#include "SecBridge.h"
#include <security_keychain/Certificate.h>
#include <security_keychain/Identity.h>
#include <security_keychain/KeyItem.h>

CFTypeID
SecIdentityGetTypeID(void)
{
	BEGIN_SECAPI

	return gTypes().Identity.typeID;

	END_SECAPI1(_kCFRuntimeNotATypeID)
}


OSStatus
SecIdentityCopyCertificate(
            SecIdentityRef identityRef, 
            SecCertificateRef *certificateRef)
{
    BEGIN_SECAPI

	SecPointer<Certificate> certificatePtr(Identity::required(identityRef)->certificate());
	Required(certificateRef) = certificatePtr->handle();

    END_SECAPI
}


OSStatus
SecIdentityCopyPrivateKey(
            SecIdentityRef identityRef, 
            SecKeyRef *privateKeyRef)
{
    BEGIN_SECAPI

	SecPointer<KeyItem> keyItemPtr(Identity::required(identityRef)->privateKey());
	Required(privateKeyRef) = keyItemPtr->handle();

    END_SECAPI
}

OSStatus
SecIdentityCreateWithCertificate(CFTypeRef keychainOrArray, SecCertificateRef certificateRef,
            SecIdentityRef *identityRef)
{
    BEGIN_SECAPI

	SecPointer<Certificate> certificatePtr(Certificate::required(certificateRef));
	StorageManager::KeychainList keychains;
	globals().storageManager.optionalSearchList(keychainOrArray, keychains);
	SecPointer<Identity> identityPtr(new Identity(keychains, certificatePtr));
	Required(identityRef) = identityPtr->handle();

    END_SECAPI
}
