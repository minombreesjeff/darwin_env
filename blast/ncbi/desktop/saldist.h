/*   saldist.h
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
* File Name:  saldist.h
*
* Author:  Colombe Chappey
*
* Version Creation Date:   1/27/96
*
* $Revision: 6.3 $
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

#ifndef _SALDIST_
#define _SALDIST_

#include <salsa.h>
#include <vibrant.h>

/********************************************************
*** Calcul 
*********************************************************/
extern void AlignReportItem (IteM i);
extern void PwDistanceItem (IteM i);
extern void PwDistanceLongItem (IteM i);
extern void GpDistanceItem (IteM i);
extern void SortbySimItem (IteM i);
extern void SortbyLenItem (IteM i);
extern void QuorumItem (IteM i);
extern void DistPosItem (IteM i);
extern void NonsyToSynItem1 (IteM i);
extern void NonsyToSynItem2 (IteM i);
extern void NonsyToSynItem3 (IteM i);
extern void NonsyToSynItem4 (IteM i);
extern void NonsyToSynItem5 (IteM i);
extern void NonsyToSynItem6 (IteM i);

#ifdef SALSA_DEBUG
extern void TreeViewItem (IteM i);
#endif


#endif

