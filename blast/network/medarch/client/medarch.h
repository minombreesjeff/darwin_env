/*
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
* File Name:  medarch.h
*
* Author:  Epstein
*
* Version Creation Date:   04/12/93
*
* $Revision: 6.0 $
*
* File Description: 
*       API for Medline Archive service
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 010296   grisha      Add prototypes for MRI and Accessions lookup
* 050896   grisha      Add prototypes for PubMed support
* 021196   grisha      Add prototypes for MEDLARS entry support
*
* ==========================================================================
*
*
* RCS Modification History:
* $Log: medarch.h,v $
* Revision 6.0  1997/08/25 18:35:44  madden
* Revision changed to 6.0
*
* Revision 5.3  1997/02/11 22:41:47  grisha
* Add code and prototypes for support MEDLARS entry
*
 * Revision 5.2  1996/12/17  20:32:30  grisha
 * update interface to support PubmedEntry
 *
 * Revision 5.1  1996/07/23  19:36:18  grisha
 * Add new function to return Medline entry in new format
 *
 * Revision 5.0  1996/05/28  14:11:11  ostell
 * Set to revision 5.0
 *
 * Revision 4.3  1996/05/08  20:58:36  grisha
 * implement new set of functions for PubMed support
 *
 * Revision 4.2  1996/05/03  14:35:19  grisha
 * Add new functions and error codes for PubMed support
 *
 * Revision 4.1  1996/01/05  21:40:44  grisha
 * Add two new function for MRI and Accession lookup. Now
 * this function do not call MedArch server.
 *
 * Revision 4.0  1995/07/26  13:55:12  ostell
 * force revision to 4.0
 *
 * Revision 1.2  1995/05/17  17:53:58  epstein
 * add RCS log revision history
 *
*/
#ifndef __MEDARCHH__
#define __MEDARCHH__ medarchh

/***************************************************************************/
/* INCLUDES */
/***************************************************************************/
#include <mapmla.h>

/***************************************************************************/
/* TYPEDEFS */
/***************************************************************************/
typedef enum {
    kMedArchDdbj = 1,           /* DNA Data Bank of Japan */
    kMedArchCarbBank = 2,       /* Carbohydrate Structure Database */
    kMedArchEmbl = 3,           /* EMBL Data Library */
    kMedArchHdb = 4,            /* Hybridoma Data Bank */
    kMedArchGenBank = 5,        /* GenBank */
    kMedArchHgml = 6,           /* Human Gene Map Library */
    kMedArchMim = 7,            /* Mendelian Inheritance in Man */
    kMedArchMsd = 8,            /* Microbial Strains Database */
    kMedArchPdb = 9,            /* Protein Data Bank (Brookhaven) */
    kMedArchPir = 10,           /* Protein Identification Resource */
    kMedArchPrfseqdb = 11,      /* Protein Research Foundation (Japan) */
    kMedArchPsd = 12,           /* Protein Sequence Database (Japan) */
    kMedArchSwissprot = 13      /* SwissProt */
} EMedArchTypeOfXref;

/***************************************************************************/
/* DEFINES */
/***************************************************************************/
#define MEDARCH_SERV_RETRIES 2  /* The number of retries to get a server */

/***************************************************************************/
/* FUNCTION PROTOTYPES */
/***************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/* Initialization/Termination */
Boolean         MedArchInit PROTO((        /* Initialize MedArch interface */
                       void
                ));
Boolean         MedArchFini PROTO((        /* Close MedArch interface */
                       void
                ));

/* Generate list of articles using Medline id(uids) or PubMed id(pmid) */
Int4            MedArchPubmedEntryListGetPmId PROTO((   /* Create list of Med. Ent. */
                    PubmedEntryPtr PNTR result,          /* To store data list (O) */
                    Int4 nupmid,                         /* Number if pmids (I) */
                    Int4Ptr pmids,                       /* Array of pmids (I/O) */
                    Boolean mark_missing                 /* Mark missing flag (I) */
                ));
Int4            MedArchPubmedEntryListGetUid PROTO(( /* Create list of Med. Ent.*/
                    PubmedEntryPtr PNTR result,       /* To store data list (O) */
                    Int4 numuid,                      /* Number if ids (I) */
                    Int4Ptr uids,                     /* Array of ids (I/O) */
                    Boolean mark_missing              /* Mark missing flag (I) */
                ));
Int4            MedArchMedlineEntryListGet PROTO(( /* Create list of Med. Ent.*/
                    MedlineEntryPtr PNTR result,   /* To store data list (O) */
                    Int4 numuid,                   /* Number if ids (I) */
                    Int4Ptr uids,                  /* Array of ids (I/O) */
                    Boolean mark_missing           /* Mark missing flag (I) */
                ));

/* Generate single article entry using Medline id(uid) or PubMed id(pmid) */
PubmedEntryPtr  MedArchPubmedEntryGetPmId PROTO(( /* Get entry by PubMed id */
                    Int4 pmid                      /* PubMed unique id */
                ));
PubmedEntryPtr  MedArchPubmedEntryGetUid PROTO((  /* Get entry by Medline id */
                    Int4 uid                       /* Medline unique id */
                ));
MedlineEntryPtr MedArchMedlineEntryGet PROTO((     /* Get entry by Medline id */
                    Int4 uid                       /* Medline unique id */
                ));
MedlarsEntryPtr MedArchMedlarsEntryGetPmId PROTO(( /* Get entry by PubMed id */
                    Int4 pmid                      /* PubMed unique id */
                ));
MedlarsEntryPtr MedArchMedlarsEntryGetUid PROTO((  /* Get entry by Medline id */
                    Int4 uid                       /* Medline unique id */
                ));

/* Generate ASN.1 Publication Set using list of PubMed or Medline ids */
Int4            MedArchPubSetCreatePmId PROTO(( /* Create PubSet using PubMed ids */
                    AsnIoPtr aip,               /* Output ASN.1 stream (I/O) */
                    Int4 numpmid,               /* Number of ids (I) */
                    Int4Ptr pmids,              /* Array of pmids (I/O) */
                    Boolean mark_missing        /* Mark missing flag (I) */
                ));
Int4            MedArchPubSetCreateUid PROTO((  /* Create PubSet using Medline ids */
                    AsnIoPtr aip,               /* Output ASN.1 stream (I/O) */
                    Int4 numuid,                /* Number of ids (I) */
                    Int4Ptr uids,               /* Array of muids (I/O) */
                    Boolean mark_missing        /* Mark missing flag (I) */
                ));
Int4            MedArchPubSetCreate PROTO((     /* Create PubSet using Medline ids */
                    AsnIoPtr aip,               /* Output ASN.1 stream (I/O) */
                    Int4 numuid,                /* Number of ids (I) */
                    Int4Ptr uids,               /* Array of muids (I/O) */
                    Boolean mark_missing        /* Mark missing flag (I) */
                ));

/* Journal database titles lookup */
Int4            MedArchGetTitles PROTO((
                    CharPtr PNTR titles_found,
                    Int1Ptr title_types_found,
                    CharPtr title_to_lookup,
                    Int1 request_type,
                    Int1 response_type,
                    Int4 max_titles_to_find
                ));

/* Generate citation information for id */
ValNodePtr      MedArchGetPubPmId PROTO((       /* Create citation for PubMed id */
                    Int4 pmid                   /* Article PubMed id (I) */
                ));
ValNodePtr      MedArchGetPub PROTO((           /* Create citation for Medline id */
                    Int4 uid                    /* Article Medline id (I) */
                ));

/* Citation matching functions */
ValNodePtr      MedArchCitMatchList PROTO((     /* Find PubMed ids are match */
                    ValNodePtr pub,             /* Citation (I) */
                    Int4Ptr                     /* Error code (O) */
                ));
Int4            MedArchCitMatchPmId PROTO((     /* Find article PubMed id */
                    ValNodePtr pub              /* Citation (I) */
                ));
Int4            MedArchCitMatch PROTO((         /* Find article Medline id */
                    ValNodePtr pub              /* Citation (I) */
                ));

/* Generate list of PubMed or Medline id for selected MRI */
ValNodePtr      MedArchGetMriPmids PROTO(( /* Generate list of PubMed ids */
                    Int4 mri               /* Machine-readable id (I) */
                ));
ValNodePtr      MedArchGetMriUids PROTO((  /* Generate list of Medline ids */
                    Int4 mri               /* Machine-readable id (I) */
                ));

/* Generate list of PubMed or Medline id for selected accession */
ValNodePtr      MedArchGetAccPmids PROTO(( /* Generate list of PubMed ids */
                    Int4 iCode,            /* Type of xref (I) */
                    CharPtr lpAccNum       /* The citation/accession number (I) */
                ));
ValNodePtr      MedArchGetAccUids PROTO((  /* Generate list of Medline ids */
                    Int4 iCode,            /* Type of xref (I) */
                    CharPtr lpAccNum       /* The citation/accession number (I) */
                ));

/* Medline to PubMed identifier convertor */
Int4            MedArchPm2Mu PROTO((       /* Convert PubMed id to Medline id */
                    Int4 pmid              /* PubMed id (I) */
                ));
Int4            MedArchMu2Pm PROTO((       /* Convert Medline id to PubMed id */
                    Int4 uid               /* Medline id (I) */
                ));

#ifdef __cplusplus
}
#endif

#endif /* __MEDARCHH__ */
/*END*/
