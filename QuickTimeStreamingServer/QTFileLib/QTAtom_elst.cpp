/*
 *
 * @APPLE_LICENSE_HEADER_START@
 *
 * Copyright (c) 1999-2001 Apple Computer, Inc.  All Rights Reserved. The
 * contents of this file constitute Original Code as defined in and are
 * subject to the Apple Public Source License Version 1.2 (the 'License').
 * You may not use this file except in compliance with the License.  Please
 * obtain a copy of the License at http://www.apple.com/publicsource and
 * read it before using this file.
 *
 * This Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.  Please
 * see the License for the specific language governing rights and
 * limitations under the License.
 *
 *
 * @APPLE_LICENSE_HEADER_END@
 *
 */
// $Id: QTAtom_elst.cpp,v 1.5.18.1 2002/11/27 10:14:02 murata Exp $
//
// QTAtom_elst:
//   The 'elst' QTAtom class.


// -------------------------------------
// Includes
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "QTFile.h"

#include "QTAtom.h"
#include "QTAtom_elst.h"
#include "OSMemory.h"



// -------------------------------------
// Macros
//
#define DEBUG_PRINT(s) if(fDebug) printf s
#define DEEP_DEBUG_PRINT(s) if(fDeepDebug) printf s



// -------------------------------------
// Constants
//
const int		elstPos_VersionFlags		=  0;
const int		elstPos_NumEdits			=  4;
const int		elstPos_EditList			=  8;

const int		elstEntryPos_Duration		=  0;
const int		elstEntryPos_MediaTime		=  4;
const int		elstEntryPos_MediaRate		=  8;

const int		elstEntryPosV1_Duration		=  0;
const int		elstEntryPosV1_MediaTime    =  8;
const int		elstEntryPosV1_MediaRate	=  16;



// -------------------------------------
// Constructors and destructors
//
QTAtom_elst::QTAtom_elst(QTFile * File, QTFile::AtomTOCEntry * TOCEntry, Bool16 Debug, Bool16 DeepDebug)
	: QTAtom(File, TOCEntry, Debug, DeepDebug),
	  fNumEdits(0), fEdits(NULL),
	  fFirstEditMovieTime(0)
{
}

QTAtom_elst::~QTAtom_elst(void)
{
	//
	// Free our variables.
	if( fEdits != NULL )
		delete[] fEdits;
}



// -------------------------------------
// Initialization functions
//
Bool16 QTAtom_elst::Initialize(void)
{
	// Temporary vars
	UInt32		tempInt32;


	//
	// Parse this atom's fields.
	ReadInt32(elstPos_VersionFlags, &tempInt32);
	fVersion = (UInt8)((tempInt32 >> 24) & 0x000000ff);
	fFlags = tempInt32 & 0x00ffffff;

    if (fVersion > 1)
    {
        DEEP_DEBUG_PRINT(("QTAtom_elst::Initialize failed. Version unsupported: %d\n",fVersion));
        return false;
    }

	ReadInt32(elstPos_NumEdits, &fNumEdits);

	//
	// Read in all of the edits.
	if( fNumEdits > 0 ) {
		DEEP_DEBUG_PRINT(("QTAtom_elst::Initialize ..%lu edits found.\n", fNumEdits));

		//
		// Allocate our ref table.
		fEdits = NEW EditListEntry[fNumEdits];
		if( fEdits == NULL )
			return false;

		//
		// Read them all in..
		for( UInt32 CurEdit = 0; CurEdit < fNumEdits; CurEdit++ ) 
        {
            
            if (0 == fVersion)
            {
                //
                // Get all of the data in this edit list entry.
                ReadInt32To64(elstPos_EditList + (CurEdit * 12) + elstEntryPos_Duration, &fEdits[CurEdit].EditDuration);
                ReadInt32To64(elstPos_EditList + (CurEdit * 12) + elstEntryPos_MediaTime, (UInt64*) &fEdits[CurEdit].StartingMediaTime);
    
                ReadInt32(elstPos_EditList + (CurEdit * 12) + elstEntryPos_MediaRate, &fEdits[CurEdit].EditMediaRate);
                
            }
            else if (1 == fVersion)
            {

                // Get all of the data in this edit list entry.
                ReadInt64(elstPos_EditList + (CurEdit * 20) + elstEntryPosV1_Duration, &fEdits[CurEdit].EditDuration);    
                ReadInt64(elstPos_EditList + (CurEdit * 20) + elstEntryPosV1_MediaTime, (UInt64*) &fEdits[CurEdit].StartingMediaTime );
                
                ReadInt32(elstPos_EditList + (CurEdit * 20) + elstEntryPosV1_MediaRate, &fEdits[CurEdit].EditMediaRate);
                
            }

            DEEP_DEBUG_PRINT(("QTAtom_elst::Initialize ..Edit #%lu: Duration=%"_64BITARG_"u; MediaTime=%"_64BITARG_"d\n", CurEdit, fEdits[CurEdit].EditDuration, fEdits[CurEdit].StartingMediaTime));
			//
			// Adjust our starting media time.
			if( fEdits[CurEdit].StartingMediaTime == -1 )
				fFirstEditMovieTime = fEdits[CurEdit].EditDuration;
		}
	}

	//
	// This atom has been successfully read in.
	return true;
}



// -------------------------------------
// Debugging functions
//
void QTAtom_elst::DumpAtom(void)
{
	DEBUG_PRINT(("QTAtom_elst::DumpAtom - Dumping atom.\n"));
	DEBUG_PRINT(("QTAtom_elst::DumpAtom - ..Version: %d.\n", (int) fVersion));
	DEBUG_PRINT(("QTAtom_elst::DumpAtom - ..Number of edits: %ld\n", fNumEdits));
}
