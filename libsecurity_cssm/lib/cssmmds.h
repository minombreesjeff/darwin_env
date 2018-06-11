/*
 * Copyright (c) 2000-2001,2003-2004 Apple Computer, Inc. All Rights Reserved.
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
// cssmmds - MDS interface for CSSM & friends
//
#ifndef _H_CSSMMDS
#define _H_CSSMMDS

#include "cssmint.h"
#include <security_utilities/globalizer.h>
#include <security_cdsa_utilities/cssmpods.h>
#include <string>
#include <stdio.h>	// @@@ for temporary MDS interface only

#ifdef _CPP_CSSMMDS
# pragma export on
#endif


class MdsComponent {
public:
    MdsComponent(const Guid &guid);
    virtual ~MdsComponent();

    const Guid &myGuid() const { return mMyGuid; }
    
    CSSM_SERVICE_MASK services() const { update(); return mServices; }
    bool supportsService(CSSM_SERVICE_TYPE t) const { update(); return t & mServices; }
    bool isThreadSafe() const { update(); return mIsThreadSafe; }
    const char *path() const { update(); return mPath.c_str(); }

private:
    const Guid mMyGuid;					// GUID of the component

    // information obtained about the component
    mutable bool mIsValid;				// information is valid
    mutable string mPath;			// module location
    mutable CSSM_SERVICE_TYPE mServices; // service types provided
    mutable bool mIsThreadSafe;			// is the module thread safe?

    void getInfo() const;  				// retrieve MDS information
    void update() const { if (!mIsValid) getInfo(); }

private:
    struct MDS {
        MDS();
        ~MDS();
        FILE *config;
    };
    static ModuleNexus<MDS> mds;
};

#ifdef _CPP_CSSMMDS
# pragma export off
#endif


#endif //_H_CSSMMDS
