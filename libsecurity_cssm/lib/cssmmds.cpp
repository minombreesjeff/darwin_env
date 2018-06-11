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
// cssmmds - MDS interface for CSSM & friends  
//
#ifdef __MWERKS__
#define _CPP_CSSMMDS
#endif
#include "cssmmds.h"
#include <ctype.h>


ModuleNexus<MdsComponent::MDS> MdsComponent::mds;


MdsComponent::MdsComponent(const Guid &guid) : mMyGuid(guid)
{
    mIsValid = false;
}

MdsComponent::~MdsComponent()
{
}


void MdsComponent::getInfo() const
{
    rewind(mds().config);
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), mds().config)) {
		static const char space[] = " \t\f";
		char *p = buffer + strspn(buffer, space);
		if (*p == '#' || *p == '\0' || *p == '\n')
			continue;	// comment or blank line
			
		// field 1: guid
		const char *guid = p; p += strcspn(p, space);
		if (!*p) CssmError::throwMe(CSSM_ERRCODE_MDS_ERROR);
		*p++ = '\0';
		try {
			if (Guid(guid) != mMyGuid)
				continue;	// no match this line
		} catch (const CssmError &error) {
			if (error.error == CSSM_ERRCODE_INVALID_GUID)
				CssmError::throwMe(CSSM_ERRCODE_MDS_ERROR);	// invalid file guid => MDS error
			throw;	// pass all other errors
		}
		
		// field 2: flags/options
		p += strspn(p, space);
		mServices = 0;
		mIsThreadSafe = false;
		while (*p && !isspace(*p)) {
			switch (*p) {
			case 'a':	mServices |= CSSM_SERVICE_AC; break;
			case 'c':	mServices |= CSSM_SERVICE_CSP; break;
			case 'd':	mServices |= CSSM_SERVICE_DL; break;
			case 'C':	mServices |= CSSM_SERVICE_CL; break;
			case 't':	mServices |= CSSM_SERVICE_TP; break;
			case 'm':	mIsThreadSafe = true; break;
			default:
				CssmError::throwMe(CSSM_ERRCODE_MDS_ERROR);
			}
			p++;
		}
		if (!*p) CssmError::throwMe(CSSM_ERRCODE_MDS_ERROR);
		p += strspn(p, space);	// skip space
		p[strlen(p)-1] = '\0';	// remove trailing newline
		mPath = p;				// rest of line is pathname, uninterpreted
        mIsValid = true;
        return;
    }
    // not found in file
    CssmError::throwMe(CSSM_ERRCODE_INVALID_GUID);	// @@@ should have "guid not found" error
}


#if !defined(_MDS_PATH)
# define _MDS_PATH "/System/Library/Frameworks/Security.framework/Resources/MDS"
#endif

MdsComponent::MDS::MDS()
{
    const char *path = getenv("MDSPATH");
	if (path == NULL)
			path = getenv("MDSFILE");
    if (path == NULL)
        path = _MDS_PATH;
    if (!(config = fopen(path, "r")))
        CssmError::throwMe(CSSM_ERRCODE_MDS_ERROR);
}

MdsComponent::MDS::~MDS()
{
    fclose(config);
}
