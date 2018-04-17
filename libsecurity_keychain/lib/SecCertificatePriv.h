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

#ifndef _SECURITY_SECCERTIFICATEPRIV_H_
#define _SECURITY_SECCERTIFICATEPRIV_H_

#include <Security/SecBase.h>
#include <Security/cssmtype.h>
#include <CoreFoundation/CFArray.h>

#if defined(__cplusplus)
extern "C" {
#endif

/*!
	@function SecCertificateGetPublicKey
	@abstract Retrieves the public key for a given certificate.
    @param certificate A reference to the certificate from which to retrieve the data.
    @param data On return, a pointer to the data for the certificate specified.  The caller must allocate the space for a CSSM_DATA structure before calling this function.  This data pointer is only guaranteed to remain valid as long as the certificate remains unchanged and valid.
	@result A result code.  See "Security Error Codes" (SecBase.h).
*/
OSStatus SecCertificateCopyPublicKey(SecCertificateRef certificate, SecKeyRef *key);

OSStatus SecCertificateGetAlgorithmID(SecCertificateRef certificate,const CSSM_X509_ALGORITHM_IDENTIFIER **algid);

OSStatus SecCertificateGetCommonName(SecCertificateRef certificate, CFStringRef *commonName);

/* @@@ Obsoleted by SecCertificateCopyEmailAddresses(), also really should of been named
   SecCertificateCopyEmailAddress() since the returned address is not autoreleased. */
OSStatus SecCertificateGetEmailAddress(SecCertificateRef certificate, CFStringRef *emailAddress);

OSStatus SecCertificateCopyEmailAddresses(SecCertificateRef certificate, CFArrayRef *emailAddresses);

/*
 * Private API to infer a display name for a SecCertificateRef which
 * may or may not be in a keychain.
 */
OSStatus SecCertificateInferLabel(SecCertificateRef certificate, CFStringRef *label);


/*
 * Subset of the above, useful for both certs and CRLs.
 * Infer printable label for a given an CSSM_X509_NAME. Returns NULL
 * if no appropriate printable name found.
 */
const CSSM_DATA *SecInferLabelFromX509Name(
	const CSSM_X509_NAME *x509Name);

/*	Convenience functions for searching
*/

OSStatus SecCertificateFindByIssuerAndSN(CFTypeRef keychainOrArray, const CSSM_DATA *issuer,
	const CSSM_DATA *serialNumber, 	SecCertificateRef *certificate);

OSStatus SecCertificateFindBySubjectKeyID(CFTypeRef keychainOrArray, const CSSM_DATA *subjectKeyID,
	SecCertificateRef *certificate);

OSStatus SecCertificateFindByEmail(CFTypeRef keychainOrArray, const char *emailAddress,
	SecCertificateRef *certificate);


/* These should go to SecKeychainSearchPriv.h. */
OSStatus SecKeychainSearchCreateForCertificateByIssuerAndSN(CFTypeRef keychainOrArray, const CSSM_DATA *issuer,
	const CSSM_DATA *serialNumber, SecKeychainSearchRef *searchRef);

OSStatus SecKeychainSearchCreateForCertificateBySubjectKeyID(CFTypeRef keychainOrArray, const CSSM_DATA *subjectKeyID,
	SecKeychainSearchRef *searchRef);

OSStatus SecKeychainSearchCreateForCertificateByEmail(CFTypeRef keychainOrArray, const char *emailAddress,
	SecKeychainSearchRef *searchRef);

CSSM_RETURN SecDigestGetData(CSSM_ALGORITHMS alg, CSSM_DATA* digest, const CSSM_DATA* data);

/* NOT EXPORTED YET; copied from SecurityInterface but could be useful in the future.
CSSM_CSP_HANDLE	SecGetAppleCSPHandle();
CSSM_CL_HANDLE SecGetAppleCLHandle();
*/

#if defined(__cplusplus)
}
#endif

#endif /* !_SECURITY_SECCERTIFICATEPRIV_H_ */
