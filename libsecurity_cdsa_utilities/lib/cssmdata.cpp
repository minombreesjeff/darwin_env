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
// cssmdata.cpp -- Manager different CssmData types
//
#include <security_cdsa_utilities/cssmdata.h>
#include <security_utilities/utilities.h>
#include <cstring>


namespace Security {


//
// Comparing raw CSSM_DATA things
//
bool operator == (const CSSM_DATA &d1, const CSSM_DATA &d2)
{
    if (&d1 == &d2)
        return true;	// identical
    if (d1.Length != d2.Length)
        return false;	// can't be
    if (d1.Data == d2.Data)
        return true;	// points to same data
    return !memcmp(d1.Data, d2.Data, d1.Length);
}


//
// CssmData out of line members
// 
string CssmData::toString() const 
{
    return data() ? 
        string(reinterpret_cast<const char *>(data()), length())
        :
        string();
}   


//
// Managed data objects
//
CssmManagedData::~CssmManagedData()
{ }


//
// CssmOwnedData
//
void CssmOwnedData::set(CssmManagedData &source)
{
    if (source.length() == 0) {					// source is empty
        reset();								// so just clear old data
    } else if (allocator == source.allocator) {	// compatible allocators
        if (referent.data() == source.data()) {	// same data *and* we own it?!
            assert(this == &source);			//  this better *be* me!
        } else {								// different data
            reset();							// give up our old data
            referent = source.release();		// take over source's data
        }
    } else {									// different allocators
        copy(source);							// make a copy with our allocator
        source.reset();							// release source's data
    }
}


//
// CssmAutoData
//
CssmData CssmAutoData::release()
{
    CssmData result = mData;
    mData.clear();
    return result;
}

void CssmAutoData::reset()
{
    allocator.free(mData);
    mData.clear();
}


//
// CssmRemoteData
//
CssmData CssmRemoteData::release()
{
    iOwnTheData = false;
    return referent;
}

void CssmRemoteData::reset()
{
    if (iOwnTheData)
        allocator.free(referent);
    referent.clear();
}


//
// Date stuff
//
CssmDateData::CssmDateData(const CSSM_DATE &date)
: CssmData(buffer, sizeof(buffer))
{
	memcpy(buffer, date.Year, 4);
	memcpy(buffer + 4, date.Month, 2);
	memcpy(buffer + 6, date.Day, 2);
}


}	// end namespace Security
