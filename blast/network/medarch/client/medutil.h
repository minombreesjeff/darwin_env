/*  medutil.h
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
* File Name:  medutil.h
*
* Author:  James Ostell
*   
* Version Creation Date: 8/31/93
*
* $Revision: 6.1 $
*
* File Description:  * File Description:  Medline Utilities for MedArch
*   Assumes user calls MedArchInit and Fini
*   Uses MedArch to match citations, lookup Medline uids, and ISO jta's
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* ==========================================================================
*
*
* RCS Modification History:
* $Log: medutil.h,v $
* Revision 6.1  2000/08/18 17:01:02  kans
* added FetchPubPmId, enhanced FixPubEquiv to handle records with pmid but no muid
*
* Revision 6.0  1997/08/25 18:35:50  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 14:11:11  ostell
* Set to revision 5.0
*
 * Revision 4.0  1995/07/26  13:55:12  ostell
 * force revision to 4.0
 *
 * Revision 1.6  1995/05/24  16:14:56  tatiana
 * add in_press() to avoid lookup for in press articles
 *
 * Revision 1.4  1995/05/17  17:54:08  epstein
 * add RCS log revision history
 *
*/

#ifndef _NCBI_MedUtil_
#define _NCBI_MedUtil_

#ifndef _NCBI_SeqUtil_
#include <sequtil.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define GB_REF     0
#define EMBL_REF   1
#define SP_REF     2
#define PIR_REF    3
#define PDB_REF    4
#define ML_REF     5

typedef struct findpubstr {
	Boolean always_look;    /* if TRUE, look up even if muid in Pub-equiv */
	Boolean replace_cit;    /* if TRUE, replace Cit-art w/ replace from MEDLINE */
	Int4 lookups_attempted, /* citartmatch tries */
		lookups_succeeded,  /* citartmatch worked */
		fetches_attempted,  /* FetchPubs tried */
		fetches_succeeded;  /* FetchPubs that worked */
} FindPubOption, PNTR FindPubOptionPtr;

                              /* SeqEntryExplore callback to fix all Pubs */
                              /* "data" is a FindPubOptionPtr */

void print_pub(ValNodePtr pub, Boolean found, Boolean auth, Int4 muid);

void FindPub PROTO((SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent));

                              /* Converts any Pub to ISO/GenBank style */
ValNodePtr FixPub PROTO((ValNodePtr pub, FindPubOptionPtr fpop));

							  /* Converts MedlineEntry to Pub-equiv */
ValNodePtr SplitMedlineEntry PROTO((MedlineEntryPtr mep));

                              /* Process PubEquiv to ISO/GenBank style */
ValNodePtr FixPubEquiv PROTO((ValNodePtr pube, FindPubOptionPtr fpop));

                              /* gets Cit-art in ISO/GenBank style given Muid */
ValNodePtr FetchPub PROTO((Int4 muid));

                              /* gets Cit-art in ISO/GenBank style given Pmid */
ValNodePtr FetchPubPmId PROTO((Int4 pmid));

                              /* check if a string all upper case */
Boolean AllUpperCase PROTO((CharPtr p ));

                              /* splits ML name into parts */
void SplitMlAuthorName PROTO(( CharPtr name, CharPtr last, CharPtr initials, CharPtr suffix ));

                              /* makes ML Cit-art to ISO/GenBank style */
ValNodePtr MedlineToISO PROTO((ValNodePtr tmp));

                              /* makes std Author name from string */
ValNodePtr get_std_auth PROTO((CharPtr token, Uint1 format));

Boolean in_press PROTO ((CitArtPtr cit));

#ifdef __cplusplus
}
#endif

#endif
