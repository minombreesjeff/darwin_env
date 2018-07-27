/*   salpanel.h
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
* File Name:  salmedia.h
*
* Author:  Yanli Wang, Patrick Durand
*
* Version Creation Date:   1/20/98
*
* $Revision: 6.2 $
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

#ifndef _SALMEDIA_
#define _SALMEDIA_



typedef struct mycolor{
  Uint1 rgb[3];
} ResidueColorCell, PNTR ResidueColorCellPtr;

typedef struct mediainfo{
SeqIdPtr sip;
Int4 Gi, length;
Uint2 entityID, itemID, itemtype;
Byte bVisible;
BytePtr bAligned;
ValNodePtr seq_color; /* seq_color->data.ptrvalue is ResidueColorCellPtr */
} MediaInfo, PNTR MediaInfoPtr;

#endif
