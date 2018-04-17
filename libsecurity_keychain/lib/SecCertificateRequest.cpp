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

#include <Security/SecCertificateRequest.h>

#include "SecBridge.h"


CFTypeID
SecCertificateRequestGetTypeID(void)
{
	BEGIN_SECAPI

	return gTypes().CertificateRequest.typeID;

	END_SECAPI1(_kCFRuntimeNotATypeID)
}


OSStatus
SecCertificateRequestCreate(
        SecPolicyRef policy,
        CSSM_CERT_TYPE certificateType,
        CSSM_TP_AUTHORITY_REQUEST_TYPE requestType,
        SecCertificateRequestRef* certRequest)
{
	BEGIN_SECAPI

	MacOSError::throwMe(unimpErr);//%%%for now

	END_SECAPI
}


OSStatus
SecCertificateRequestSetPrivateKey(
        SecCertificateRequestRef certRequest,
        SecKeychainItemRef privateKeyItemRef)
{
	BEGIN_SECAPI

	MacOSError::throwMe(unimpErr);//%%%for now

	END_SECAPI
}


OSStatus
SecCertificateRequestSetAttribute(
        SecCertificateRequestRef certRequest,
        const CSSM_OID* oid,
        const CSSM_DATA* value)
{
	BEGIN_SECAPI

	MacOSError::throwMe(unimpErr);//%%%for now

	END_SECAPI
}


OSStatus
SecCertificateRequestSubmit(
        SecCertificateRequestRef certRequest,
        SecKeychainRef keychain,
        sint32* estimatedTime,
        SecKeychainItemRef* certRequestItemRef)
{
	BEGIN_SECAPI

	MacOSError::throwMe(unimpErr);//%%%for now

	END_SECAPI
}


OSStatus
SecCertificateRequestCreateFromItem(
        SecKeychainItemRef certRequestItemRef,
        SecCertificateRequestRef* certRequestRef)
{
	BEGIN_SECAPI

	MacOSError::throwMe(unimpErr);//%%%for now

	END_SECAPI
}


OSStatus
SecCertificateRequestGetType(
        SecCertificateRequestRef certRequestRef,
        CSSM_TP_AUTHORITY_REQUEST_TYPE* requestType)
{
	BEGIN_SECAPI

	MacOSError::throwMe(unimpErr);//%%%for now

	END_SECAPI
}


OSStatus
SecCertificateRequestGetResult(
        SecCertificateRequestRef certRequestRef,
        sint32* estimatedTime,
        SecCertificateRef* certificateRef)
{
	BEGIN_SECAPI

	MacOSError::throwMe(unimpErr);//%%%for now

	END_SECAPI
}
