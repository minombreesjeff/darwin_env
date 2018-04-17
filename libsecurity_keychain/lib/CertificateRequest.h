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

//
// CertificateRequest.h
//
#ifndef _SECURITY_CERTIFICATEREQUEST_H_
#define _SECURITY_CERTIFICATEREQUEST_H_

#include <Security/SecCertificateRequest.h>
#include <security_utilities/seccfobject.h>
#include "SecCFTypes.h"

namespace Security
{

namespace KeychainCore
{

class CertificateRequest : public SecCFObject
{
	NOCOPY(CertificateRequest)
public:
	SECCFFUNCTIONS(CertificateRequest, SecCertificateRequestRef, errSecInvalidItemRef, gTypes().CertificateRequest)

    CertificateRequest(int a);
    virtual ~CertificateRequest() throw();

private:
};

} // end namespace KeychainCore

} // end namespace Security

#endif // !_SECURITY_CERTIFICATEREQUEST_H_
