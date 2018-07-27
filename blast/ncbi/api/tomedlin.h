/*  tomedlin.h
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
* File Name:  tomedlin.h
*
* Author:  Jonathan Kans
*   
* Version Creation Date: 10/15/91
*
* $Revision: 6.2 $
*
* File Description:  conversion to medlars format
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: tomedlin.h,v $
* Revision 6.2  2003/09/28 20:22:47  kans
* added PubmedEntryToXXXFile functions
*
* Revision 6.1  1997/12/22 18:51:23  grisha
* update unit-record number for PmId to 969
*
* Revision 6.0  1997/08/25 18:07:58  madden
* Revision changed to 6.0
*
* Revision 5.4  1997/06/19 18:39:30  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.3  1997/04/10 20:29:25  levitsky
* MEDLINE fields codes added
*
 * Revision 5.2  1997/02/20  21:25:17  kans
 * added MedlineEntryToAbsFile and MedlarsEntryToAbsFile
 *
 * Revision 5.1  1997/02/12  21:30:02  grisha
 * add code to support MEDLARS report generation
 * for MedlarsEntry
 *
 * Revision 5.0  1996/05/28  13:23:23  ostell
 * Set to revision 5.0
 *
 * Revision 4.0  1995/07/26  13:49:01  ostell
 * force revision to 4.0
 *
 * Revision 2.2  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

#ifndef _NCBI_Tomedlin_
#define _NCBI_Tomedlin_

#include <objmedli.h>
#include <objmdrs.h>
#include <objpubme.h>

#define MEDLINE_BADCODE 0 /* BAD                             */
#define MEDLINE_EOF -1  /* END OF ENTRY                      */
#define MEDLINE_DA 100 /* DATE OF ENTRY                     */
#define MEDLINE_RO 101 /* RECORD ORIGINATOR                 */
#define MEDLINE_LR 102 /* LAST REVISION DATE                */
#define MEDLINE_CU 103 /* CLASSUP DATE                      */
#define MEDLINE_RN 261 /* CAS REGISTRY NUMBER               */
#define MEDLINE_IS 301 /* INTL STAND SER NO                 */
#define MEDLINE_TA 305 /* TITLE ABBREVIATION                */
#define MEDLINE_LA 306 /* LANGUAGE                          */
#define MEDLINE_CY 307 /* COUNTRY NAME                      */
#define MEDLINE_ZN 308 /* MESH Z TREE NUMBER                */
#define MEDLINE_JC 320 /* JOURNAL TITLE CODE                */
#define MEDLINE_PY 323 /* INDEXING PRIORITY                 */
#define MEDLINE_SB 324 /* JOURNAL SUBSET                    */
#define MEDLINE_GS 328 /* GENE SYMBOL                       */
#define MEDLINE_LI 329 /* SPECIAL LIST IND                  */
#define MEDLINE_UI 350 /* UNIQUE IDENTIFIER                 */
#define MEDLINE_MH 351 /* MESH HEADING                      */
#define MEDLINE_TI 352 /* TITLE                             */
#define MEDLINE_PG 353 /* PAGINATION                        */
#define MEDLINE_DP 354 /* DATE OF PUBLICATION               */
#define MEDLINE_PT 360 /* PUBLICATION TYPE                  */
#define MEDLINE_MRI 368 /* MACHINE-READABLE ID               */
#define MEDLINE_NP 369 /* NOT FOR PUBLICATION               */
#define MEDLINE_AB 370 /* ABSTRACT                          */
#define MEDLINE_AA 371 /* ABSTRACT AUTHOR                   */
#define MEDLINE_AU 372 /* AUTHOR                            */
#define MEDLINE_IP 373 /* ISSUE/PART/SUPP                   */
#define MEDLINE_NI 376 /* NO-AUTHOR INDICATOR               */
#define MEDLINE_RF 377 /* NUMBER OF REFS                    */
#define MEDLINE_AD 378 /* ADDRESS                           */
#define MEDLINE_PS 379 /* PERSONAL NAME AS SUBJECT          */
#define MEDLINE_TT 380 /* TRANSLIT/VERNAC TITLE             */
#define MEDLINE_VI 381 /* VOLUME/ISSUE                      */
#define MEDLINE_EA 382 /* ENGLISH ABSTRACT INDEX            */
#define MEDLINE_EM 383 /* ENTRY MONTH                       */
#define MEDLINE_CM 440 /* COMMENTS                          */
#define MEDLINE_CA 525 /* CALL NUMBER                       */
#define MEDLINE_SI 606 /* SECONDARY SOURCE ID               */
#define MEDLINE_ID 640 /* ID NUMBER                         */
#define MEDLINE_PM 969 /* PUBMED ARTICLE ID                 */
#define MEDLINE_PMOFF   4096    /* ALL INTERNAL PUBMED IDS STARTED FROM HERE */
#define MEDLINE_PAID 4097 /* PUBLISHER ARTICLE ID              */

/* Define the codes specific to identifying Monographs in the JC/TA fields */
#define MEDLINE_MONOGRAPH_JC "IDM"
#define MEDLINE_MONOGRAPH_TA "(Monograph)"

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct medlinedata {
  CharPtr  journal;
  CharPtr  volume;
  CharPtr  pages;
  CharPtr  year;
  CharPtr  title;
  CharPtr  transl;
  CharPtr  authors;
  CharPtr  affil;
  CharPtr  abstract;
  CharPtr  mesh;
  CharPtr  gene;
  CharPtr  substance;
  CharPtr  uid;
} MedlineData, PNTR MedlinePtr;

NLM_EXTERN Boolean MedlineEntryToDataFile PROTO((MedlineEntryPtr mep, FILE *fp));
NLM_EXTERN Boolean MedlineEntryToDocFile PROTO((MedlineEntryPtr mep, FILE *fp));
NLM_EXTERN Boolean MedlineEntryToAbsFile PROTO((MedlineEntryPtr mep, FILE *fp));

NLM_EXTERN Boolean MedlarsEntryToDataFile PROTO((MedlarsEntryPtr mep, FILE *fp));
NLM_EXTERN Boolean MedlarsEntryToDocFile PROTO((MedlarsEntryPtr mep, FILE *fp));
NLM_EXTERN Boolean MedlarsEntryToAbsFile PROTO((MedlarsEntryPtr mep, FILE *fp));

NLM_EXTERN Boolean PubmedEntryToDataFile PROTO((PubmedEntryPtr pep, FILE *fp));
NLM_EXTERN Boolean PubmedEntryToDocFile PROTO((PubmedEntryPtr pep, FILE *fp));
NLM_EXTERN Boolean PubmedEntryToAbsFile PROTO((PubmedEntryPtr pep, FILE *fp));

NLM_EXTERN MedlinePtr ParseMedline PROTO((MedlineEntryPtr mep));
NLM_EXTERN MedlinePtr FreeMedline PROTO((MedlinePtr mPtr));

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif
