/*   apparam.h
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*            National Center for Biotechnology Information (NCBI)
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government do not place any restriction on its use or reproduction.
*  We would, however, appreciate having the NCBI and the author cited in
*  any work or product based on this material
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
* ===========================================================================
*
* File Name:  apparam.h
*
* Author:  Sergei Egorov
*
* Version Creation Date:   9/23/94
*
* $Revision: 6.0 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#ifndef _APPARAM_
#define _APPARAM_

#include <vibrant.h>

/* fuzzy logic here 8^) */
typedef UIEnum UIFuzzy, PNTR UIFuzzyPtr;
#define FUZZY_DEFAULT       -3 /* JK */
#define FUZZY_VERY_SMALL    -2
#define FUZZY_SMALL         -1
#define FUZZY_MEDIUM        0
#define FUZZY_LARGE         1
#define FUZZY_VERY_LARGE    2

#define MAX_APS_BASE  30
#define MAX_APS_SECT  60
#define MAX_APS_VALUE 120

typedef struct apsect {
  Char base [MAX_APS_BASE+1];
  Char sect [MAX_APS_SECT+1]; 
} APSect, PNTR APSectPtr;


/* basic operations */
extern Boolean APOpenSection (CharPtr file, CharPtr sect, APSectPtr sp);
extern void APForEachKey (APSectPtr sp, void (*proc) (CharPtr key));
extern Boolean APDeleteSection (APSectPtr sp); /* delete subsections first! */
extern Boolean APFindKey (APSectPtr sp, CharPtr key);
extern Boolean APDeleteKey (APSectPtr sp, CharPtr key);
extern Boolean APGetKeyValue (APSectPtr sp, CharPtr key, CharPtr PNTR pval);
extern Boolean APSetKeyValue (APSectPtr sp, CharPtr key, CharPtr val);

/* subsections */
extern Boolean APCreateSubsection (APSectPtr sp, CharPtr ss, APSectPtr ssp);
extern Boolean APLookupSubsection (APSectPtr sp, CharPtr ss, APSectPtr ssp);
extern Boolean APDeleteSubsection (APSectPtr sp, CharPtr ss);
extern void APForEachSubsection (APSectPtr sp, void (*proc) (CharPtr ss));

/* font dir hacks ??? */
extern Boolean APFindEntry (APSectPtr sp, CharPtr ss, Int2Ptr pindex);
extern Boolean APEntrySubsection (APSectPtr sp, Int2 index, APSectPtr ssp);

/* save/restore */
extern Boolean APArchiveBoolean (Boolean writep, APSectPtr sp, CharPtr key, BoolPtr pb);
extern Boolean APArchiveEnum (Boolean writep, APSectPtr sp, CharPtr key, UIEnumPtr pe);
extern Boolean APArchiveInt2 (Boolean writep, APSectPtr sp, CharPtr key, Int2Ptr pi2);
extern Boolean APArchiveUint2 (Boolean writep, APSectPtr sp, CharPtr key, Uint2Ptr pu2);
extern Boolean APArchiveInt4 (Boolean writep, APSectPtr sp, CharPtr key, Int4Ptr pi4);
extern Boolean APArchiveUint4 (Boolean writep, APSectPtr sp, CharPtr key, Uint4Ptr pu4);
extern Boolean APArchiveString (Boolean writep, APSectPtr sp, CharPtr key, CharPtr psb, Int2 bsz);

#define ARC_WRITE   TRUE
#define ARC_READ    FALSE

/* utils */
extern CharPtr APArrayIndexKey(CharPtr key, Int4 i);

#endif /* ndef _APPARAM_ */
