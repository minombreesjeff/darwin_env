/* $Id: repeats_filter.h,v 1.4 2005/04/06 23:27:53 dondosha Exp $
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE                          
*               National Center for Biotechnology Information
*                                                                          
*  This software/database is a "United States Government Work" under the   
*  terms of the United States Copyright Act.  It was written as part of    
*  the author's official duties as a United States Government employee and 
*  thus cannot be copyrighted.  This software/database is freely available 
*  to the public for use. The National Library of Medicine and the U.S.    
*  Government have not placed any restriction on its use or reproduction.  
*                                                                          
*  Although all reasonable efforts have been taken to ensure the accuracy  
*  and reliability of the software and data, the NLM and the U.S.          
*  Government do not and cannot warrant the performance or results that    
*  may be obtained by using this software or data. The NLM and the U.S.    
*  Government disclaim all warranties, express or implied, including       
*  warranties of performance, merchantability or fitness for any particular
*  purpose.                                                                
*                                                                          
*  Please cite the author in any work or product based on this material.   
*
* ===========================================================================
*
* Author: Ilya Dondoshansky
*
*/

/** @file repeats_filter.h
* Repeats filtering API for the C version of rewritten BLAST engine.
*/

#ifndef _REPEATS_FILTER_ 
#define _REPEATS_FILTER_ 

#ifdef __cplusplus
extern "C" {
#endif

#include <objloc.h>

/** @addtogroup CToolkitAlgoBlast
 *
 * @{
 */

/** Finds repeats filtering locations, given filter string option.
 * Does nothing if filter string does not contain a repeat option.
 * @param query_seqloc List of query locations [in]
 * @param filter_string Filtering option for a BLAST search, e.g. 
 *                      "m L;R -d rodents.lib" [in]
 * @param mask_loc List of repeat mask locations [out]
 * @return Status.
 */
Int2 Blast_FindRepeatFilterSeqLoc(SeqLoc* query_seqloc, 
                                  const char* filter_string, 
                                  SeqLoc* *mask_loc);

/* @} */

#ifdef __cplusplus
}
#endif

#endif
