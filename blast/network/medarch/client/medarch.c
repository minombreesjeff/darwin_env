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
* File Name:  medarch.c
*
* Author:  Epstein
*
* Version Creation Date:   03/31/93
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
* 6/15/93  Epstein     Changed Message() to ErrPost() in init function
* 6/24/93  Epstein     Remember to load Pub object loaders in
*                      MedArchPubSetCreate().
* 8/04/93  Epstein     Fix Fini() logic to correctly disconnect the service,
*                      and fix erroneously-typed returned value.
* 2/28/96  Grisha      Add new function for MRI look up
* 5/08/96  Grisha      Add new functions for PubMed support
* 3/06/97  volodya     Has implemented global functions MedArchGetAccUids(),
                       MedArchGetAccPmids()
*
* ==========================================================================
*
*
* RCS Modification History:
* $Log: medarch.c,v $
* Revision 6.0  1997/08/25 18:35:40  madden
* Revision changed to 6.0
*
* Revision 5.6  1997/06/05 21:34:05  volodya
* volodya added more comments to MedArchGetAccPmids(), MedArchGetAccUids() description
*
* Revision 5.5  1997/06/03 22:07:08  volodya
* volodya has implemented MedArchGetAccPmids(), MedArchGetAccUids()
*
* Revision 5.4  1997/06/03 21:31:22  volodya
* volodyas has implemented MedArchGetAccUids(), MedArchGetAccPmids() functionality
*
* Revision 5.3  1997/04/18 18:27:19  grisha
* Fixed bug with ASN.1 I/O streams initializations
*
 * Revision 5.2  1997/02/11  22:41:47  grisha
 * Add code and prototypes for support MEDLARS entry
 *
 * Revision 5.1  1996/12/17  20:32:30  grisha
 * update interface to support PubmedEntry
 *
 * Revision 5.0  1996/05/28  14:11:11  ostell
 * Set to revision 5.0
 *
 * Revision 4.6  1996/05/10  16:17:15  grisha
 * Fixed bug with check: MlaBack_error
 *
 * Revision 4.5  1996/05/08  22:09:27  grisha
 * implement Medline Id, PubMed Id conversion functions
 *
 * Revision 4.4  1996/05/08  20:58:36  grisha
 * implement new set of functions for PubMed support
 *
 * Revision 4.3  1996/02/28  22:13:51  kans
 * trivial codewarrior warnings
 *
 * Revision 4.2  1996/02/28  21:41:46  grisha
 * implement function MedArchGetMriUids(), now this function
 * will call MedArch server for MRI lookup
 *
 * Revision 4.1  1996/01/05  21:40:44  grisha
 * Add prototypes for new functions. MedArchGetMriUids for
 * MRI lookup and MedArchGetAccUids for accession lookup
 *
 * Revision 4.0  1995/07/26  13:55:12  ostell
 * force revision to 4.0
 *
 * Revision 1.15  1995/05/17  17:53:55  epstein
 * add RCS log revision history
 *
*/
#ifndef __MEDARCHC__
#define __MEDARCHC__ medarchc

/******************************************************************************/
/* INCLUDES */
/******************************************************************************/
#include <ncbinet.h>
#include <objmedli.h>
#include <objpub.h>
#include <objall.h>
#include "mapmla.h"
#include <objmla.h>
#include <medarch.h>

/******************************************************************************/
/* DEFINES */
/******************************************************************************/

/******************************************************************************/
/* GLOBAL VARIABLES */
/******************************************************************************/
MlaRequestPtr mlarp;
MlaBackPtr mlabp;

/******************************************************************************/
/* LOCAL VARIABLES */
/******************************************************************************/
static NI_HandPtr svcp = NULL;
static AsnIoPtr   asnin = NULL;
static AsnIoPtr   asnout = NULL;
static Boolean num_attached = 0;
static Boolean reallyFinal = TRUE;
static NI_DispatcherPtr dispatcher;
static Boolean (*myNetInit) PROTO((void));

/******************************************************************************/
/* LOCAL FUNCTIONS PROTOTYPES */
/******************************************************************************/
static MlaBackPtr      NetMedArchReadAsn PROTO((
                           void
                       ));
static Boolean         ReestablishNetMedArch PROTO((
                           void
                       ));
static Boolean         GenericReestablishNet PROTO((
                           CharPtr,
                           Boolean
                       ));
static Boolean         NetInit PROTO((
                           void
                       ));
static Boolean         ForceNetInit PROTO((
                           void
                       ));
static Boolean         NetFini PROTO((
                           void
                       ));
static MedlineEntryPtr s_MedArchMedlineEntryGet PROTO((
                           Int4    /* Medline Id */
                       ));
static PubmedEntryPtr  s_MedArchPubmedEntryGetUid PROTO((
                           Int4    /* PubMed Id */
                       ));
static PubmedEntryPtr  s_MedArchPubmedEntryGetPmId PROTO((
                           Int4    /* PubMed Id */
                       ));
static MedlarsEntryPtr s_MedArchMedlarsEntryGetUid PROTO((
                           Int4    /* PubMed Id */
                       ));
static MedlarsEntryPtr s_MedArchMedlarsEntryGetPmId PROTO((
                           Int4    /* PubMed Id */
                       ));
static MedlineEntryPtr s_MedArchPubSetEntryGet PROTO((
                           Int4,            /* Medline Id */
                           MedlineEntryPtr, /* Last Medline entry */
                           AsnIoPtr,        /* Client ASN.1 stream */
                           AsnTypePtr       /* ASN.1 types */
                       ));
static PubmedEntryPtr  s_MedArchPubSetEntryGetPmId PROTO((
                           Int4,            /* PubMed Id */
                           PubmedEntryPtr,  /* Last PubMed entry */
                           AsnIoPtr,        /* Client ASN.1 stream */
                           AsnTypePtr       /* ASN.1 types */
                       ));
static PubmedEntryPtr  s_MedArchPubSetEntryGetUid PROTO((
                           Int4,            /* PubMed Id */
                           PubmedEntryPtr,  /* Last PubMed entry */
                           AsnIoPtr,        /* Client ASN.1 stream */
                           AsnTypePtr       /* ASN.1 types */
                       ));
static ValNodePtr      s_MedArchGetPub PROTO((
                           Int4    /* Medline Id */
                       ));
static ValNodePtr      s_MedArchGetPubPmId PROTO((
                           Int4    /* Medline Id */
                       ));
static Int4            s_MedArchGetTitles PROTO((
                           CharPtr PNTR,
                           Int1Ptr,
                           CharPtr,
                           Int1,
                           Int1,
                           Int4
                       ));
static Int4            s_MedArchCitMatch PROTO((
                           ValNodePtr
                       ));
static Int4            s_MedArchCitMatchPmId PROTO((
                           ValNodePtr
                       ));
static ValNodePtr      s_MedArchCitMatchList PROTO((
                           ValNodePtr,
                           Int4Ptr
                       ));
static ValNodePtr      s_MedArchGetMriUids PROTO((
                           Int4
                       ));
static ValNodePtr      s_MedArchGetMriPmids PROTO((
                           Int4
                       ));
static ValNodePtr      s_MedArchGetAccPmids PROTO((
                           Int4 iCode,
                           CharPtr lpAccNum       
                       ));
static ValNodePtr      s_MedArchGetAccUids PROTO(( 
                            Int4 iCode,
                            CharPtr lpAccNum
                       ));
static Int4            s_MedArchMu2Pm PROTO((
                           Int4
                       ));
static Int4            s_MedArchPm2Mu PROTO((
                           Int4
                       ));

/******************************************************************************/
/* LOCAL FUNCTIONS */
/******************************************************************************/

/******************************************************************************/
/*.doc NetMedArchReadAsn(internal) */
/*+
-*/
/******************************************************************************/
static MlaBackPtr
/*FCN*/NetMedArchReadAsn (void)
{
    MlaBackPtr mlabp;
    short erract;
    ErrDesc err;

    ErrGetOpts(&erract, NULL);
    ErrSetOpts(ERR_CONTINUE, 0);
    ErrFetch(&err);                  /* clear any pending error */

    mlabp = MlaBackAsnRead(asnin, NULL);

    if ( mlabp == NULL ) {
        ErrPost(CTX_UNKNOWN, 1, "Null message read from server");
    }
    ErrSetOpts(erract, 0);

    return mlabp;
}                                /* NetMedArchReadAsn() */

/******************************************************************************/
/*.doc ReestablishNetMedArch(internal) */
/*+
-*/
/******************************************************************************/
static Boolean
/*FCN*/ReestablishNetMedArch (void)
{
    return GenericReestablishNet("MedArch", TRUE);
}                                /* ReestablishNetMedArch() */

/******************************************************************************/
/*.doc GenericReestablishNet(internal) */
/*+
-*/
/******************************************************************************/
static Boolean
/*FCN*/GenericReestablishNet (
  CharPtr svcName,
  Boolean showErrs
){
    Monitor *mon = NULL;
    Boolean retval;
    CharPtr buf;

    buf = (CharPtr)MemNew(2 * StrLen(svcName) + 60);

    if ( showErrs ) {
        sprintf (buf, "Re-establishing %s Service", svcName);
        mon = MonitorStrNew(buf, 40);
        sprintf (buf, "Requesting %s service", svcName);
        MonitorStrValue(mon, buf);
    }
    NetFini();
    retval = TRUE;

    if ( !myNetInit() ) {
        sprintf (buf, "%s get failed; re-contacting dispatcher", svcName);
        MonitorStrValue(mon, buf);
        retval = FALSE;
        if ( ForceNetInit() ) {
            /* successfully established contact w/dispatcher */
            sprintf (
                buf,
                "%s get failed; re-requesting %s service",
                svcName,
                svcName
            );
            MonitorStrValue(mon, buf);
            retval = myNetInit();
        } else {
            ErrPost(CTX_UNKNOWN, 1, "Unable to re-contact dispatcher");
            if (showErrs) {
                ErrShow();
            }
        }
    }

    MonitorFree(mon);

    if ( !retval ) {
        sprintf (
            buf,
            "Unable to re-establish %s service",
            svcName
        );
        ErrPost(CTX_UNKNOWN, 1, buf);
        if ( showErrs ) {
            ErrShow();
        }
    }

    MemFree((VoidPtr)buf);
    return retval;
}                                       /* GenericReestablishNet() */

/******************************************************************************/
/*.doc (internal) */
/*+
-*/
/******************************************************************************/
static Boolean
/*FCN*/NetInit(void)
{
    if ( num_attached++ > 0 ) {
        return TRUE;
    }

    if ( (dispatcher = NI_GenericInit (
                           NULL,
                           NULL,
                           TRUE,
                           NULL,
                           0
                       )) != NULL ) {
        return TRUE;
    }

    return FALSE;
}                                        /* NetInit() */

/******************************************************************************/
/*.doc ForceNetInit(internal) */
/*+
-*/
/******************************************************************************/
static Boolean
/*FCN*/ForceNetInit (void)
{
    Boolean retval;

    reallyFinal = FALSE;
    num_attached = 0; /* force re-attempt to contact dispatcher */
    retval = NetInit();
    reallyFinal = TRUE;

    return retval;
}                                       /* ForceNetInit() */

/******************************************************************************/
/*.doc NetFini(internal) */
/*+
-*/
/******************************************************************************/
static Boolean
/*FCN*/NetFini (void)
{
    if ( num_attached > 0 ) {
        num_attached--;
    }

    if ( num_attached == 0 ) {
        NI_ServiceDisconnect (svcp);
        svcp = NULL;
        NI_EndServices (dispatcher);
        dispatcher = NULL;
    }

    return TRUE;
}                                        /* NetFini() */

/******************************************************************************/
/*.doc s_MedArchFini(internal) */
/*+
   Create FINI request and send to MedArch server, terminate
   network connection.
-*/
/******************************************************************************/
static Boolean
/*FCN*/s_MedArchFini (void)
{
    Boolean retval = TRUE;

    if ( asnout != NULL && asnin != NULL ) {
        mlarp = ValNodeNew(NULL);
        mlarp->choice = MlaRequest_fini;
        MlaRequestAsnWrite (mlarp, asnout, NULL);
        AsnIoReset(asnout);
        MlaRequestFree (mlarp);

        if ( (mlabp = NetMedArchReadAsn()) == NULL ) {
            retval = FALSE;
        } else {
            mlabp->data.ptrvalue = NULL;
            MlaBackFree (mlabp);
        }
    }

    NetFini();

    return retval;
}                          /* s_MedArchFini() */

/******************************************************************************/
/*.doc s_MedArchMedlineEntryGet(internal) */
/*+
   Generate request: Get Medline Article using Medline Id, return
   pointer to Medline entry. Return NULL if article not found or
   some errors found.
-*/
/******************************************************************************/
static MedlineEntryPtr
/*FCN*/s_MedArchMedlineEntryGet (
  Int4 uid
){
    MlaRequestPtr mlarp;
    MedlineEntryPtr mep;

    /* create request object */
    mlarp = ValNodeNew(NULL);                 /* allocate node */
    mlarp->choice = MlaRequest_getmle;        /* request type */
    mlarp->data.intvalue = uid;               /* Medline id */
    MlaRequestAsnWrite (mlarp, asnout, NULL); /* send to MedArch */
    AsnIoReset(asnout);                       /* fush output stream */
    MlaRequestFree (mlarp);                   /* free request */

    /* result processing */
    if ( (mlabp = NetMedArchReadAsn()) == NULL ) {
        return NULL;
    }

    if ( mlabp->choice != MlaBack_getmle ) {
        MlaBackFree (mlabp);
        return NULL;
    }

    mep = (MedlineEntryPtr) (mlabp->data.ptrvalue);
    mlabp->data.ptrvalue = NULL; /* for clean free */
    MlaBackFree (mlabp);

    return mep;
}                                    /* s_MedArchMedlineEntryGet() */

/******************************************************************************/
/*.doc s_MedArchPubmedEntryGetPmId(internal) */
/*+
   Generate request: Get Medline Article using PubMed Id, return
   pointer to PubMed entry. Return NULL if article not found or
   some errors found.
-*/
/******************************************************************************/
static PubmedEntryPtr
/*FCN*/s_MedArchPubmedEntryGetPmId (
  Int4 pmid
){
    MlaRequestPtr mlarp;
    PubmedEntryPtr mep;

    /* create request object */
    mlarp = ValNodeNew(NULL);                 /* allocate node */
    mlarp->choice = MlaRequest_getmlepmid;    /* request type */
    mlarp->data.intvalue = pmid;              /* PubMed id */
    MlaRequestAsnWrite (mlarp, asnout, NULL); /* send to MedArch */
    AsnIoReset(asnout);                       /* fush output stream */
    MlaRequestFree (mlarp);                   /* free request */

    /* result processing */
    if ( (mlabp = NetMedArchReadAsn()) == NULL ) {
        return NULL;
    }

    if ( mlabp->choice != MlaBack_getpme ) {
        MlaBackFree (mlabp);
        return NULL;
    }

    mep = (PubmedEntryPtr) (mlabp->data.ptrvalue);
    mlabp->data.ptrvalue = NULL; /* for clean free */
    MlaBackFree (mlabp);

    return mep;
}                                    /* s_MedArchPubmedEntryGetPmId() */

/******************************************************************************/
/*.doc s_MedArchPubmedEntryGetUid(internal) */
/*+
   Generate request: Get Medline Article using PubMed Id, return
   pointer to PubMed entry. Return NULL if article not found or
   some errors found.
-*/
/******************************************************************************/
static PubmedEntryPtr
/*FCN*/s_MedArchPubmedEntryGetUid (
  Int4 pmid
){
    MlaRequestPtr mlarp;
    PubmedEntryPtr mep;

    /* create request object */
    mlarp = ValNodeNew(NULL);                 /* allocate node */
    mlarp->choice = MlaRequest_getmleuid;     /* request type */
    mlarp->data.intvalue = pmid;              /* PubMed id */
    MlaRequestAsnWrite (mlarp, asnout, NULL); /* send to MedArch */
    AsnIoReset(asnout);                       /* fush output stream */
    MlaRequestFree (mlarp);                   /* free request */

    /* result processing */
    if ( (mlabp = NetMedArchReadAsn()) == NULL ) {
        return NULL;
    }

    if ( mlabp->choice != MlaBack_getpme ) {
        MlaBackFree (mlabp);
        return NULL;
    }

    mep = (PubmedEntryPtr) (mlabp->data.ptrvalue);
    mlabp->data.ptrvalue = NULL; /* for clean free */
    MlaBackFree (mlabp);

    return mep;
}                                    /* s_MedArchPubmedEntryGetUid() */

/******************************************************************************/
/*.doc s_MedArchMedlarsEntryGetPmId(internal) */
/*+
   Generate request: Get MEDLARS entry using PubMed Id, return
   pointer to Medlars entry. Return NULL if article not found or
   some errors found.
-*/
/******************************************************************************/
static MedlarsEntryPtr
/*FCN*/s_MedArchMedlarsEntryGetPmId (
  Int4 pmid
){
    MlaRequestPtr mlarp;
    MedlarsEntryPtr mep;

    /* create request object */
    mlarp = ValNodeNew(NULL);                 /* allocate node */
    mlarp->choice = MlaRequest_getmlrpmid;    /* request type */
    mlarp->data.intvalue = pmid;              /* PubMed id */
    MlaRequestAsnWrite (mlarp, asnout, NULL); /* send to MedArch */
    AsnIoReset(asnout);                       /* fush output stream */
    MlaRequestFree (mlarp);                   /* free request */

    /* result processing */
    if ( (mlabp = NetMedArchReadAsn()) == NULL ) {
        return NULL;
    }

    if ( mlabp->choice != MlaBack_getmlr ) {
        MlaBackFree (mlabp);
        return NULL;
    }

    mep = (MedlarsEntryPtr) (mlabp->data.ptrvalue);
    mlabp->data.ptrvalue = NULL; /* for clean free */
    MlaBackFree (mlabp);

    return mep;
}                                    /* s_MedArchMedlarsEntryGetPmId() */

/******************************************************************************/
/*.doc s_MedArchMedlarsEntryGetUid(internal) */
/*+
   Generate request: Get MEDLARS entry using PubMed Id, return
   pointer to Medlars entry. Return NULL if article not found or
   some errors found.
-*/
/******************************************************************************/
static MedlarsEntryPtr
/*FCN*/s_MedArchMedlarsEntryGetUid (
  Int4 pmid
){
    MlaRequestPtr mlarp;
    MedlarsEntryPtr mep;

    /* create request object */
    mlarp = ValNodeNew(NULL);                 /* allocate node */
    mlarp->choice = MlaRequest_getmlruid;     /* request type */
    mlarp->data.intvalue = pmid;              /* PubMed id */
    MlaRequestAsnWrite (mlarp, asnout, NULL); /* send to MedArch */
    AsnIoReset(asnout);                       /* fush output stream */
    MlaRequestFree (mlarp);                   /* free request */

    /* result processing */
    if ( (mlabp = NetMedArchReadAsn()) == NULL ) {
        return NULL;
    }

    if ( mlabp->choice != MlaBack_getmlr ) {
        MlaBackFree (mlabp);
        return NULL;
    }

    mep = (MedlarsEntryPtr) (mlabp->data.ptrvalue);
    mlabp->data.ptrvalue = NULL; /* for clean free */
    MlaBackFree (mlabp);

    return mep;
}                                    /* s_MedArchMedlarsEntryGetUid() */

/******************************************************************************/
/*.doc s_MedArchPubSetEntryGet(internal) */
/*+
   This function read Medline entry from MedArch and write
   last entry while waiting for MedArch answer.
-*/
/******************************************************************************/
static MedlineEntryPtr
/*FCN*/s_MedArchPubSetEntryGet (
  Int4 uid,
  MedlineEntryPtr lastmep,
  AsnIoPtr aip,
  AsnTypePtr atp
){
    MlaRequestPtr mlarp;
    MedlineEntryPtr mep;

    mlarp = ValNodeNew(NULL);
    mlarp->choice = MlaRequest_getmle;
    mlarp->data.intvalue = uid;
    MlaRequestAsnWrite (mlarp, asnout, NULL);
    AsnIoReset (asnout);
    MlaRequestFree (mlarp);

    /* write the caller's last entry to a file while waiting
       for the server to compute and send the answer for the
       current entry
    */
    if ( lastmep != NULL && aip != NULL ) {
        MedlineEntryAsnWrite (lastmep, aip, atp);
    }

    if ( (mlabp = NetMedArchReadAsn()) == NULL ) {
        return NULL;
    }

    if ( mlabp->choice != MlaBack_getmle ) {
        MlaBackFree (mlabp);
        return NULL;
    }
    
    mep = (MedlineEntryPtr) (mlabp->data.ptrvalue);
    mlabp->data.ptrvalue = NULL; /* for clean free */
    MlaBackFree (mlabp);

    return mep;
}                                 /* s_MedArchPubSetEntryGet() */

/******************************************************************************/
/*.doc s_MedArchPubSetEntryGetPmId(internal) */
/*+
   This function read Medline entry from MedArch and write
   last entry while waiting for MedArch answer.
-*/
/******************************************************************************/
static PubmedEntryPtr
/*FCN*/s_MedArchPubSetEntryGetPmId (
  Int4 pmid,
  PubmedEntryPtr lastmep,
  AsnIoPtr aip,
  AsnTypePtr atp
){
    MlaRequestPtr mlarp;
    PubmedEntryPtr mep;

    mlarp = ValNodeNew(NULL);
    mlarp->choice = MlaRequest_getmlepmid;
    mlarp->data.intvalue = pmid;
    MlaRequestAsnWrite (mlarp, asnout, NULL);
    AsnIoReset (asnout);
    MlaRequestFree (mlarp);

    /* write the caller's last entry to a file while waiting
       for the server to compute and send the answer for the
       current entry
    */
    if ( lastmep != NULL && aip != NULL ) {
        PubmedEntryAsnWrite (lastmep, aip, atp);
    }

    if ( (mlabp = NetMedArchReadAsn()) == NULL ) {
        return NULL;
    }

    if ( mlabp->choice != MlaBack_getpme ) {
        MlaBackFree (mlabp);
        return NULL;
    }
    
    mep = (PubmedEntryPtr) (mlabp->data.ptrvalue);
    mlabp->data.ptrvalue = NULL; /* for clean free */
    MlaBackFree (mlabp);

    return mep;
}                                /* s_MedArchPubSetEntryGetPmId() */

/******************************************************************************/
/*.doc s_MedArchPubSetEntryGetUid(internal) */
/*+
   This function read Medline entry from MedArch and write
   last entry while waiting for MedArch answer.
-*/
/******************************************************************************/
static PubmedEntryPtr
/*FCN*/s_MedArchPubSetEntryGetUid (
  Int4 pmid,
  PubmedEntryPtr lastmep,
  AsnIoPtr aip,
  AsnTypePtr atp
){
    MlaRequestPtr mlarp;
    PubmedEntryPtr mep;

    mlarp = ValNodeNew(NULL);
    mlarp->choice = MlaRequest_getmleuid;
    mlarp->data.intvalue = pmid;
    MlaRequestAsnWrite (mlarp, asnout, NULL);
    AsnIoReset (asnout);
    MlaRequestFree (mlarp);

    /* write the caller's last entry to a file while waiting
       for the server to compute and send the answer for the
       current entry
    */
    if ( lastmep != NULL && aip != NULL ) {
        PubmedEntryAsnWrite (lastmep, aip, atp);
    }

    if ( (mlabp = NetMedArchReadAsn()) == NULL ) {
        return NULL;
    }

    if ( mlabp->choice != MlaBack_getpme ) {
        MlaBackFree (mlabp);
        return NULL;
    }
    
    mep = (PubmedEntryPtr) (mlabp->data.ptrvalue);
    mlabp->data.ptrvalue = NULL; /* for clean free */
    MlaBackFree (mlabp);

    return mep;
}                                /* s_MedArchPubSetEntryGetUid() */

/******************************************************************************/
/*.doc s_MedArchGetPub(internal) */
/*+
   This function read Citation definition for selected entry using
   Medline Id.
-*/
/******************************************************************************/
static ValNodePtr
/*FCN*/s_MedArchGetPub (
  Int4 uid                       /* Medline Id */
){
    MlaRequestPtr mlarp;
    ValNodePtr pub;

    mlarp = ValNodeNew(NULL);
    mlarp->choice = MlaRequest_getpub;
    mlarp->data.intvalue = uid;
    MlaRequestAsnWrite (mlarp, asnout, NULL);
    AsnIoReset(asnout);
    MlaRequestFree (mlarp);

    if ((mlabp = NetMedArchReadAsn()) == NULL)
        return NULL;

    if (mlabp->choice != MlaBack_getpub)
    {
        MlaBackFree (mlabp);
        return NULL;
    }
    pub = (ValNodePtr)mlabp->data.ptrvalue;
    mlabp->data.ptrvalue = NULL; /* for clean free */
    MlaBackFree (mlabp);

    return pub;
}                                /* s_MedArchGetPub() */

/******************************************************************************/
/*.doc s_MedArchGetPubPmId(internal) */
/*+
   This function read Citation definition for selected entry using
   PubMed Id.
-*/
/******************************************************************************/
static ValNodePtr
/*FCN*/s_MedArchGetPubPmId (
  Int4 pmid                       /* PubMed Id */
){
    MlaRequestPtr mlarp;
    ValNodePtr pub;

    mlarp = ValNodeNew(NULL);
    mlarp->choice = MlaRequest_getpubpmid;
    mlarp->data.intvalue = pmid;
    MlaRequestAsnWrite (mlarp, asnout, NULL);
    AsnIoReset(asnout);
    MlaRequestFree (mlarp);

    if ((mlabp = NetMedArchReadAsn()) == NULL)
        return NULL;

    if (mlabp->choice != MlaBack_getpub)
    {
        MlaBackFree (mlabp);
        return NULL;
    }
    pub = (ValNodePtr)mlabp->data.ptrvalue;
    mlabp->data.ptrvalue = NULL; /* for clean free */
    MlaBackFree (mlabp);

    return pub;
}                                /* s_MedArchGetPubPmId() */

/******************************************************************************/
/*.doc s_MedArchGetTitles(internal) */
/*+
-*/
/******************************************************************************/
static Int4
/*FCN*/s_MedArchGetTitles (
  CharPtr PNTR titles_found,
  Int1Ptr title_types_found,
  CharPtr title_to_lookup,
  Int1 request_type,
  Int1 response_type,
  Int4 max_titles_to_find
){
    MlaRequestPtr mlarp;
    TitleMsgPtr tmsgp;
    TitleMsgListPtr tmlp;
    Int4 i;

    mlarp = ValNodeNew(NULL);
    mlarp->choice = MlaRequest_gettitle;
    tmsgp = TitleMsgNew();
    tmsgp->type = response_type;
    tmsgp->title = ValNodeNew(NULL);
    tmsgp->title->choice = request_type;
    tmsgp->title->data.ptrvalue = (Pointer) StringSave(title_to_lookup);
    mlarp->data.ptrvalue = (Pointer) tmsgp;
    MlaRequestAsnWrite (mlarp, asnout, NULL);
    AsnIoReset(asnout);
    MlaRequestFree (mlarp);

    if ( (mlabp = NetMedArchReadAsn()) == NULL ) {
        return 0;
    }

    if ( mlabp->choice != MlaBack_gettitle ) {
        MlaBackFree (mlabp);
        return 0;
    }

    if ( (tmlp = (TitleMsgListPtr)mlabp->data.ptrvalue) == NULL ) {
        MlaBackFree (mlabp);
        return 0;
    }

    for ( i = 0, tmsgp = tmlp->titles;
             i < max_titles_to_find
          && tmlp != NULL
          && i < tmlp->num;
          i++, tmsgp = tmsgp->next ) {
        titles_found[i] = (CharPtr) tmsgp->title->data.ptrvalue;
        tmsgp->title->data.ptrvalue = NULL; /* for clean free */
        title_types_found[i] = (Int1) tmsgp->type;
    }
    MlaBackFree (mlabp);

    return i;
}                                        /* s_MedArchGetTitles() */

/******************************************************************************/
/*.doc s_MedArchCitMatch(internal) */
/*+
-*/
/******************************************************************************/
static Int4
/*FCN*/s_MedArchCitMatch (
  ValNodePtr pub
){
    MlaRequestPtr mlarp;
    Int4 uid;

    mlarp = ValNodeNew(NULL);
    mlarp->choice = MlaRequest_citmatch;
    mlarp->data.ptrvalue = pub;
    MlaRequestAsnWrite (mlarp, asnout, NULL);
    AsnIoReset(asnout);
    mlarp->data.ptrvalue = NULL; /* for clean free */
    MlaRequestFree (mlarp);

    if ((mlabp = NetMedArchReadAsn()) == NULL)
        return 0;

    if (mlabp->choice != MlaBack_citmatch)
    {
        MlaBackFree (mlabp);
        return 0;
    }
    uid = mlabp->data.intvalue;
    MlaBackFree (mlabp);

    return uid;
}                                        /* s_MedArchCitMatch() */

/******************************************************************************/
/*.doc s_MedArchCitMatchPmId(internal) */
/*+
-*/
/******************************************************************************/
static Int4
/*FCN*/s_MedArchCitMatchPmId (
  ValNodePtr pub
){
    MlaRequestPtr mlarp;
    Int4 pmid;

    mlarp = ValNodeNew(NULL);
    mlarp->choice = MlaRequest_citmatchpmid;
    mlarp->data.ptrvalue = pub;
    MlaRequestAsnWrite (mlarp, asnout, NULL);
    AsnIoReset(asnout);
    mlarp->data.ptrvalue = NULL; /* for clean free */
    MlaRequestFree (mlarp);

    if ((mlabp = NetMedArchReadAsn()) == NULL)
        return 0;

    if (mlabp->choice != MlaBack_citmatch)
    {
        MlaBackFree (mlabp);
        return 0;
    }
    pmid = mlabp->data.intvalue;
    MlaBackFree (mlabp);

    return pmid;
}                                        /* s_MedArchCitMatchPmId() */

/******************************************************************************/
/*.doc s_MedArchCitMatchList(internal) */
/*+
   This function send request '' to MedArch server and recive result
   list. It set up error code in case some error found.
-*/
/******************************************************************************/
static ValNodePtr
/*FCN*/s_MedArchCitMatchList (
  ValNodePtr pub,
  Int4Ptr    pErrCode
){
    MlaRequestPtr mlarp;
    register ValNodePtr pPmIdList = NULL; /* result PubMed ID list */

    mlarp = ValNodeNew(NULL);
    mlarp->choice = MlaRequest_citlstpmids;
    mlarp->data.ptrvalue = pub;
    MlaRequestAsnWrite (mlarp, asnout, NULL);
    AsnIoReset(asnout);
    mlarp->data.ptrvalue = NULL; /* for clean free */
    MlaRequestFree (mlarp);

    if ((mlabp = NetMedArchReadAsn()) == NULL) {
        *pErrCode = Error_val_operational_error;
        return NULL;
    }

    if ( mlabp->choice == MlaBack_getpmids ) { /* must be PubMed ID list */
        pPmIdList = (ValNodePtr)mlabp->data.ptrvalue; /* get Medline list  */
        mlabp->data.ptrvalue = NULL;                  /* for clean free */
        *pErrCode = 0;
    } else if ( mlabp->choice == MlaBack_error ) {
        *pErrCode = mlabp->data.intvalue;
    } else {
        *pErrCode = Error_val_operational_error;
    }

    MlaBackFree (mlabp);            /* free result */

    return pPmIdList;
}                                /* s_MedArchCitMatchList() */

/******************************************************************************/
/*.doc s_MedArchGetMriUids(internal) */
/*+
   This function call MedArch server and send MRI code, it recive
   list of Medline ID. Return NULL if where are no articles in
   selected MRI or if some error found.
-*/
/******************************************************************************/
static
ValNodePtr s_MedArchGetMriUids (
  Int4 mri                             /* Journal MRI code (I) */
){
    MlaRequestPtr mlarp;               /* to build request object */
    ValNodePtr pub = NULL;             /* result */

    mlarp = ValNodeNew(NULL);                 /* create request node */
    mlarp->choice = MlaRequest_getmriuids;    /* request code MRI lookup */
    mlarp->data.intvalue = mri;               /* journal MRI code */
    MlaRequestAsnWrite (mlarp, asnout, NULL); /* send to request server */
    AsnIoReset(asnout);                       /* reset ASN out buffer */
    MlaRequestFree (mlarp);                   /* do not need request node */

    if ( (mlabp = NetMedArchReadAsn()) == NULL ) { /* try to read result */
        return NULL;                               /* error */
    }

    if ( mlabp->choice != MlaBack_getuids ) { /* must be Medline ID list */
        MlaBackFree (mlabp);                  /* free result */
        return NULL;                          /* error */
    }

    pub = (ValNodePtr)mlabp->data.ptrvalue; /* get Medline list  */
    mlabp->data.ptrvalue = NULL;            /* for clean free */
    MlaBackFree (mlabp);                    /* free result */

    return pub;
}                                   /* s_MedArchGetMriUids() */

/******************************************************************************/
/*.doc s_MedArchGetMriPmids(internal) */
/*+
   This function call MedArch server and send MRI code, it recive
   list of Medline ID. Return NULL if where are no articles in
   selected MRI or if some error found.
-*/
/******************************************************************************/
static
ValNodePtr s_MedArchGetMriPmids (
  Int4 mri                             /* Journal MRI code (I) */
){
    MlaRequestPtr mlarp;               /* to build request object */
    ValNodePtr pub = NULL;             /* result */

    mlarp = ValNodeNew(NULL);                 /* create request node */
    mlarp->choice = MlaRequest_getmripmids;   /* request code MRI lookup */
    mlarp->data.intvalue = mri;               /* journal MRI code */
    MlaRequestAsnWrite (mlarp, asnout, NULL); /* send to request server */
    AsnIoReset(asnout);                       /* reset ASN out buffer */
    MlaRequestFree (mlarp);                   /* do not need request node */

    if ( (mlabp = NetMedArchReadAsn()) == NULL ) { /* try to read result */
        return NULL;                               /* error */
    }

    if ( mlabp->choice != MlaBack_getuids ) { /* must be Medline ID list */
        MlaBackFree (mlabp);                  /* free result */
        return NULL;                          /* error */
    }

    pub = (ValNodePtr)mlabp->data.ptrvalue; /* get Medline list  */
    mlabp->data.ptrvalue = NULL;            /* for clean free */
    MlaBackFree (mlabp);                    /* free result */

    return pub;
}                                   /* s_MedArchGetMriPmids() */

/******************************************************************************/
/*.doc s_MedArchGetAccUids(internal) */
/*+
   This function call MedArch server and sends accession/xref code, it recives
   list of Medline IDs. plAccNum is an optional argument in accordance with
   Medline-si definition. Set it to NULL if accession code is unknown.
   It returns NULL if there are no entries found or if some error occured.
-*/
/******************************************************************************/
static
ValNodePtr s_MedArchGetAccUids (
  Int4 iCode,                  /* Type of xref (I) */
  CharPtr lpAccNum             /* The citation/accession number (I) */
){
    MlaRequestPtr mlarp;               /* to build request object */
    ValNodePtr pub = NULL;             /* result */

    mlarp = ValNodeNew(NULL);                 /* create request node */
    mlarp->choice = MlaRequest_getaccuids;    /* request code MRI lookup */
    mlarp->data.ptrvalue = ValNodeNew(NULL);  /* Medline-si structure */
    ((ValNodePtr)mlarp->data.ptrvalue)->choice = iCode;
    ((ValNodePtr)mlarp->data.ptrvalue)->data.ptrvalue = 
                 (Nlm_VoidPtr)(lpAccNum == NULL ? NULL : StringSave(lpAccNum));
    MlaRequestAsnWrite (mlarp, asnout, NULL); /* send to request server */
    AsnIoReset(asnout);                       /* reset ASN out buffer */
    MlaRequestFree (mlarp);                   /* do not need request node */

    if ( (mlabp = NetMedArchReadAsn()) == NULL ) { /* try to read result */
        return NULL;                               /* error */
    }

    if ( mlabp->choice != MlaBack_getuids ) { /* must be Medline ID list */
        MlaBackFree (mlabp);                  /* free result */
        return NULL;                          /* error */
    }

    pub = (ValNodePtr)mlabp->data.ptrvalue; /* get Medline list  */
    mlabp->data.ptrvalue = NULL;            /* for clean free */
    MlaBackFree (mlabp);                    /* free result */

    return pub;
}                                   /* s_MedArchGetAccUids() */

/******************************************************************************/
/*.doc s_MedArchGetAccPmids(internal) */
/*+
   This function call MedArch server and sends accession/xref code, it recives
   list of PubMed IDs. 
   plAccNum is an optional argument in accordance with
   Medline-si definition. Set it to NULL if accession code is unknown.
   It returns NULL if there are no entries found or if some error occured.
 -*/
/******************************************************************************/
static
ValNodePtr s_MedArchGetAccPmids (
  Int4 iCode,                  /* Type of xref (I) */
  CharPtr lpAccNum             /* The citation/accession number (I) */
){
    MlaRequestPtr mlarp;               /* to build request object */
    ValNodePtr pub = NULL;             /* result */

    mlarp = ValNodeNew(NULL);                 /* create request node */
    mlarp->choice = MlaRequest_getaccpmids;   /* request code PmId lookup */
    mlarp->data.ptrvalue = ValNodeNew(NULL);  /* Medline-si structure */
    ((ValNodePtr)mlarp->data.ptrvalue)->choice = iCode;
    ((ValNodePtr)mlarp->data.ptrvalue)->data.ptrvalue = 
                 (Nlm_VoidPtr)(lpAccNum == NULL ? NULL : StringSave(lpAccNum));
    MlaRequestAsnWrite (mlarp, asnout, NULL); /* send to request server */
    AsnIoReset(asnout);                       /* reset ASN out buffer */
    MlaRequestFree (mlarp);                   /* do not need request node */

    if ( (mlabp = NetMedArchReadAsn()) == NULL ) { /* try to read result */
        return NULL;                               /* error */
    }

    if ( mlabp->choice != MlaBack_getpmids ) { /* must be Pm ID list */
        MlaBackFree (mlabp);                  /* free result */
        return NULL;                          /* error */
    }

    pub = (ValNodePtr)mlabp->data.ptrvalue; /* get Medline list  */
    mlabp->data.ptrvalue = NULL;            /* for clean free */
    MlaBackFree (mlabp);                    /* free result */

    return pub;
}                                   /* s_MedArchGetAccPmids() */

/******************************************************************************/
/*.doc s_MedArchMu2Pm(internal) */
/*+
   This function generate request to MedArch server for
   convert Medline Id to PubMed Id
-*/
/******************************************************************************/
static Int4
/*FCN*/s_MedArchMu2Pm (
  Int4 uid
){
    MlaRequestPtr mlarp;
    Int4 pmid;

    /* create request object */
    mlarp = ValNodeNew(NULL);                 /* allocate node */
    mlarp->choice = MlaRequest_uidtopmid;     /* request type */
    mlarp->data.intvalue = uid;               /* Medline id */
    MlaRequestAsnWrite (mlarp, asnout, NULL); /* send to MedArch */
    AsnIoReset(asnout);                       /* fush output stream */
    MlaRequestFree (mlarp);                   /* free request */

    /* result processing */
    if ( (mlabp = NetMedArchReadAsn()) == NULL ) {
        return 0;
    }

    if ( mlabp->choice != MlaBack_outpmid ) {
        MlaBackFree (mlabp);
        return 0;
    }
    pmid = mlabp->data.intvalue;
    MlaBackFree (mlabp);

    return pmid;
}                                 /* s_MedArchMu2Pm() */

/******************************************************************************/
/*.doc s_MedArchMu2Pm(internal) */
/*+
   This function generate request to MedArch server for
   convert PubMed Id to Medline Id
-*/
/******************************************************************************/
static Int4
/*FCN*/s_MedArchPm2Mu (
  Int4 pmid
){
    MlaRequestPtr mlarp;
    Int4 uid;

    /* create request object */
    mlarp = ValNodeNew(NULL);                 /* allocate node */
    mlarp->choice = MlaRequest_pmidtouid;     /* request type */
    mlarp->data.intvalue = pmid;              /* Medline id */
    MlaRequestAsnWrite (mlarp, asnout, NULL); /* send to MedArch */
    AsnIoReset(asnout);                       /* fush output stream */
    MlaRequestFree (mlarp);                   /* free request */

    /* result processing */
    if ( (mlabp = NetMedArchReadAsn()) == NULL ) {
        return 0;
    }

    if ( mlabp->choice != MlaBack_outuid ) {
        MlaBackFree (mlabp);
        return 0;
    }
    uid = mlabp->data.intvalue;
    MlaBackFree (mlabp);

    return uid;
}                                 /* s_MedArchPm2Mu() */

/******************************************************************************/
/* GLOBAL FUNCTIONS */
/******************************************************************************/

/******************************************************************************/
/*.doc MedArchInit(external) */
/*+
   Create INIT request and send to MedArch server
-*/
/******************************************************************************/
Boolean
/*FCN*/MedArchInit (void)
{
    myNetInit = MedArchInit;

    asnin = NULL;                /* reset I/O streams */
    asnout = NULL;

    if ( !NetInit() ) {          /* try init network */
        return FALSE;
    }

    /* make connection with MedArch */
    if ( (svcp = NI_GenericGetService (
                     dispatcher,
                     NULL,
                     "MED_ARCH",
                     "MedArch",
                     TRUE
                 )) == NULL ) {
        ErrPostEx (              /* report error */
            SEV_ERROR,
            0,
            0,
            "NI_ServiceGet [%s] (%s)",
            ni_errlist[ni_errno],
            ni_errtext
        );
        MedArchFini();
        return FALSE;
    }

    asnin = svcp->raip;
    asnout = svcp->waip;

    /* generate INIT request and send to MedArch server */
    mlarp = ValNodeNew(NULL);
    mlarp->choice = MlaRequest_init;
    MlaRequestAsnWrite (mlarp, asnout, NULL);
    AsnIoReset(asnout);
    MlaRequestFree (mlarp);

    if ( (mlabp = NetMedArchReadAsn()) != NULL ) {
        mlabp->data.ptrvalue = NULL;
        MlaBackFree (mlabp);
        return TRUE;
    }

    return FALSE;
}                                    /* MedArchInit() */

/******************************************************************************/
/*.doc MedArchFini(external) */
/*+
   The only thing done here is to suppress errors, plus call
   function s_MedArchFini() to real discoonnect.
-*/
/******************************************************************************/
Boolean
/*FCN*/MedArchFini (void)
{
    short erract;
    ErrDesc err;
    Boolean retval;

    ErrGetOpts(&erract, NULL);
    ErrSetOpts(ERR_IGNORE, 0);
    ErrFetch(&err);

    retval = s_MedArchFini();

    ErrSetOpts(erract, 0);
    ErrFetch(&err);

    return retval;
}                                    /* MedArchFini() */

/******************************************************************************/
/*.doc MedArchMedlineEntryListGet(external) */
/*+
   This function generate list of Medline entries using Medline Id,
   return number of result entries.
-*/
/******************************************************************************/
Int4
/*FCN*/MedArchMedlineEntryListGet (
  MedlineEntryPtr PNTR result,
  Int4 numuid,
  Int4Ptr uids,
  Boolean mark_missing
){
    Int4 i;
    Int4 j;
    MedlineEntryPtr mep;
    short erract;
    ErrDesc err;
    Int4 retval = 0;

    for (j = 0; j < numuid; j++) {
        for ( i = 0; i < MEDARCH_SERV_RETRIES; i++ ) {
            if ( i > 0 ) {
                if ( !ReestablishNetMedArch() ) {
                    break;
                }
            }
    
            ErrGetOpts(&erract, NULL);
            ErrSetOpts(ERR_IGNORE, 0);
            ErrFetch(&err);
            mep = s_MedArchMedlineEntryGet (uids[j]);
            ErrSetOpts(erract, 0);
            if (! ErrFetch(&err))
                break; /* success */
        }
        result[j] = mep;
        if ( mep != NULL ) {
            retval++;
        } else {
            if ( mark_missing ) {
                uids[j] *= -1;
            }
        }
    }

    return retval;
}                               /* MedArchMedlineEntryListGet() */

/******************************************************************************/
/*.doc MedArchMedlineEntryGet(external) */
/*+
   This function generate single Medline entry using Medline Id,
   return pointer to Medline entry and NULL if some errors found.
-*/
/******************************************************************************/
MedlineEntryPtr
/*FCN*/MedArchMedlineEntryGet (
  Int4 uid
){
    MedlineEntryPtr mep = NULL;

    MedArchMedlineEntryListGet (&mep, 1, &uid, FALSE);
    
    return mep;
}                               /* MedArchMedlineEntryGet() */

/******************************************************************************/
/*.doc MedArchPubmedEntryListGetPmId(external) */
/*+
   This function generate list of PubMed entries using PubMed Id,
   return number of result entries.
-*/
/******************************************************************************/
Int4
/*FCN*/MedArchPubmedEntryListGetPmId (
  PubmedEntryPtr PNTR result,
  Int4 numpmid,
  Int4Ptr pmids,
  Boolean mark_missing
){
    Int4 i;
    Int4 j;
    PubmedEntryPtr mep;
    short erract;
    ErrDesc err;
    Int4 retval = 0;

    for (j = 0; j < numpmid; j++) {
        for ( i = 0; i < MEDARCH_SERV_RETRIES; i++ ) {
            if ( i > 0 ) {
                if ( !ReestablishNetMedArch() ) {
                    break;
                }
            }
    
            ErrGetOpts(&erract, NULL);
            ErrSetOpts(ERR_IGNORE, 0);
            ErrFetch(&err);
            mep = s_MedArchPubmedEntryGetPmId (pmids[j]);
            ErrSetOpts(erract, 0);
            if (! ErrFetch(&err))
                break; /* success */
        }
        result[j] = mep;
        if ( mep != NULL ) {
            retval++;
        } else {
            if ( mark_missing ) {
                pmids[j] *= -1;
            }
        }
    }

    return retval;
}                               /* MedArchPubmedEntryListGetPmId() */

/******************************************************************************/
/*.doc MedArchPubmedEntryGetPmId(external) */
/*+
   This function generate single PubMed entry using Medline Id,
   return pointer to PubMed entry and NULL if some errors found.
-*/
/******************************************************************************/
PubmedEntryPtr
/*FCN*/MedArchPubmedEntryGetPmId (
  Int4 pmid
){
    PubmedEntryPtr mep = NULL;

    MedArchPubmedEntryListGetPmId (&mep, 1, &pmid, FALSE);
    
    return mep;
}                               /* MedArchPubmedEntryGetPmId() */

/******************************************************************************/
/*.doc MedArchPubmedEntryListGetUid(external) */
/*+
   This function generate list of PubMed entries using PubMed Id,
   return number of result entries.
-*/
/******************************************************************************/
Int4
/*FCN*/MedArchPubmedEntryListGetUid (
  PubmedEntryPtr PNTR result,
  Int4 numpmid,
  Int4Ptr pmids,
  Boolean mark_missing
){
    Int4 i;
    Int4 j;
    PubmedEntryPtr mep;
    short erract;
    ErrDesc err;
    Int4 retval = 0;

    for (j = 0; j < numpmid; j++) {
        for ( i = 0; i < MEDARCH_SERV_RETRIES; i++ ) {
            if ( i > 0 ) {
                if ( !ReestablishNetMedArch() ) {
                    break;
                }
            }
    
            ErrGetOpts(&erract, NULL);
            ErrSetOpts(ERR_IGNORE, 0);
            ErrFetch(&err);
            mep = s_MedArchPubmedEntryGetUid (pmids[j]);
            ErrSetOpts(erract, 0);
            if (! ErrFetch(&err))
                break; /* success */
        }
        result[j] = mep;
        if ( mep != NULL ) {
            retval++;
        } else {
            if ( mark_missing ) {
                pmids[j] *= -1;
            }
        }
    }

    return retval;
}                               /* MedArchPubmedEntryListGetUid() */

/******************************************************************************/
/*.doc MedArchPubmedEntryGetUid(external) */
/*+
   This function generate single PubMed entry using Medline Id,
   return pointer to PubMed entry and NULL if some errors found.
-*/
/******************************************************************************/
PubmedEntryPtr
/*FCN*/MedArchPubmedEntryGetUid (
  Int4 pmid
){
    PubmedEntryPtr mep = NULL;

    MedArchPubmedEntryListGetUid (&mep, 1, &pmid, FALSE);
    
    return mep;
}                               /* MedArchPubmedEntryGetUid() */

/******************************************************************************/
/*.doc MedArchPubSetCreate(external) */
/*+
   This function generate PubSet using list of Medline Id
-*/
/******************************************************************************/
Int4
/*FCN*/MedArchPubSetCreate (
  AsnIoPtr aip,
  Int4 numuid,
  Int4Ptr uids,
  Boolean mark_missing
){
    static Boolean alreadyInited = FALSE;
    AsnTypePtr startAtp;
    AsnTypePtr pubSetAtp;
    AsnTypePtr pubSetEAtp;
    AsnModulePtr amp;
    Int4 j;
    Int4 i;
    MedlineEntryPtr mep;
    MedlineEntryPtr lastmep = NULL;
    Int4 retval = 0;
    short erract;
    ErrDesc err;

    if ( !alreadyInited ) {
        if ( !PubAsnLoad() ) {
            return 0;
        }
        amp = AsnAllModPtr();
        if ( amp == NULL ) {
            return 0;
        }
        alreadyInited = TRUE;
        startAtp = AsnTypeFind(amp, "Pub-set");
        pubSetAtp = AsnTypeFind(amp, "Pub-set.medline");
        pubSetEAtp = AsnTypeFind(amp, "Pub-set.medline.E");
        if (    startAtp == NULL
             || pubSetAtp == NULL
             || pubSetEAtp == NULL ) {
            return 0;
        }
    }

    AsnWrite (aip, startAtp, NULL);
    AsnStartStruct (aip, pubSetAtp);

    for (j = 0; j < numuid; j++) {
        for (i = 0; i < MEDARCH_SERV_RETRIES; i++) {
            if ( i > 0 ) {
                if ( !ReestablishNetMedArch() ) {
                    break;
                }
            }
    
            ErrGetOpts(&erract, NULL);
            ErrSetOpts(ERR_IGNORE, 0);
            ErrFetch(&err);
            mep = s_MedArchPubSetEntryGet (
                      uids[j],
                      lastmep,
                      aip,
                      pubSetEAtp
                  );
            MedlineEntryFree (lastmep); /* do not need any more */
            lastmep = NULL;
            ErrSetOpts(erract, 0);
            if ( !ErrFetch(&err) ) {
                break;                  /* success */
            }
        }
        lastmep = mep;
        if ( mep != NULL ) {
            retval++;
        } else {
            if ( mark_missing ) {
                uids[j] *= -1;
            }
        }
    }

    if ( lastmep != NULL ) {            /* write final entry */
        MedlineEntryAsnWrite (lastmep, aip, pubSetEAtp);
        MedlineEntryFree (lastmep);
    }

    AsnEndStruct (aip, pubSetAtp);

    return retval;
}                              /* MedArchPubSetCreate() */

/******************************************************************************/
/*.doc MedArchPubSetCreatePmId(external) */
/*+
   This function generate PubSet using list of PubMed Id
-*/
/******************************************************************************/
Int4
/*FCN*/MedArchPubSetCreatePmId (
  AsnIoPtr aip,
  Int4 numpmid,
  Int4Ptr pmids,
  Boolean mark_missing
){
    return 0;
}                              /* MedArchPubSetCreatePmId() */

/******************************************************************************/
/*.doc MedArchPubSetCreateUid(external) */
/*+
   This function generate PubSet using list of PubMed Id
-*/
/******************************************************************************/
Int4
/*FCN*/MedArchPubSetCreateUid (
  AsnIoPtr aip,
  Int4 numpmid,
  Int4Ptr pmids,
  Boolean mark_missing
){
    return 0;
}                              /* MedArchPubSetCreateUid() */

/******************************************************************************/
/*.doc MedArchGetPub(external) */
/*+
   This function generate single citation using Medline Id
-*/
/******************************************************************************/
ValNodePtr
/*FCN*/MedArchGetPub (
  Int4 uid
){
    ValNodePtr retval = NULL;
    Int4 i;
    short erract;
    ErrDesc err;

    for (i = 0; i < MEDARCH_SERV_RETRIES; i++) {
        if ( i > 0 ) {
            if ( !ReestablishNetMedArch() ) {
                break;
            }
        }

        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);
        retval = s_MedArchGetPub(uid);
        ErrSetOpts(erract, 0);
        if ( !ErrFetch(&err) ) {
            break;                   /* success */
        }
    }

    return retval;
}                                    /* MedArchGetPub() */

/******************************************************************************/
/*.doc MedArchGetPubPmId(external) */
/*+
   This function generate single citation using PubMed Id
-*/
/******************************************************************************/
ValNodePtr
/*FCN*/MedArchGetPubPmId (
  Int4 pmid
){
    ValNodePtr retval = NULL;
    Int4 i;
    short erract;
    ErrDesc err;

    for (i = 0; i < MEDARCH_SERV_RETRIES; i++) {
        if ( i > 0 ) {
            if ( !ReestablishNetMedArch() ) {
                break;
            }
        }

        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);
        retval = s_MedArchGetPubPmId(pmid);
        ErrSetOpts(erract, 0);
        if ( !ErrFetch(&err) ) {
            break;                   /* success */
        }
    }

    return retval;
}                                    /* MedArchGetPubPmId() */

/******************************************************************************/
/*.doc MedArchGetTitles(external) */
/*+
-*/
/******************************************************************************/
Int4
/*FCN*/MedArchGetTitles (
  CharPtr PNTR titles_found,
  Int1Ptr title_types_found,
  CharPtr title_to_lookup,
  Int1 request_type,
  Int1 response_type,
  Int4 max_titles_to_find
){
    Int4 retval = 0;
    Int4 i;
    short erract;
    ErrDesc err;

    for (i = 0; i < MEDARCH_SERV_RETRIES; i++) {
        if (i > 0) {
            if ( !ReestablishNetMedArch() )
                break;
        }

        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);
        retval = s_MedArchGetTitles (
                     titles_found,
                     title_types_found,
                     title_to_lookup,
                     request_type,
                     response_type,
                     max_titles_to_find
                 );
        ErrSetOpts(erract, 0);
        if ( !ErrFetch(&err) ) {
            break; /* success */
        }
    }

    return retval;
}                                     /* MedArchGetTitles() */

/******************************************************************************/
/*.doc MedArchCitMatch(external) */
/*+
-*/
/******************************************************************************/
Int4
/*FCN*/MedArchCitMatch (
  ValNodePtr pub
){
    Int4 retval = 0;
    Int4 i;
    short erract;
    ErrDesc err;

    for (i = 0; i < MEDARCH_SERV_RETRIES; i++) {
        if ( i > 0 ) {
            if ( !ReestablishNetMedArch() ) {
                break;
            }
        }
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);
        retval = s_MedArchCitMatch(pub);
        ErrSetOpts(erract, 0);
        if ( !ErrFetch(&err) ) {
            break; /* success */
        }
    }

    return retval;
}                                      /* MedArchCitMatch() */

/******************************************************************************/
/*.doc MedArchCitMatchPmId(external) */
/*+
-*/
/******************************************************************************/
Int4
/*FCN*/MedArchCitMatchPmId (
  ValNodePtr pub
){
    Int4 retval = 0;
    Int4 i;
    short erract;
    ErrDesc err;

    for (i = 0; i < MEDARCH_SERV_RETRIES; i++) {
        if ( i > 0 ) {
            if ( !ReestablishNetMedArch() ) {
                break;
            }
        }
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);
        retval = s_MedArchCitMatchPmId(pub);
        ErrSetOpts(erract, 0);
        if ( !ErrFetch(&err) ) {
            break; /* success */
        }
    }

    return retval;
}                                      /* MedArchCitMatchPmId() */


/******************************************************************************/
/*.doc MedArchCitMatchList(internal) */
/*+
   This function send request '' to MedArch server and recive result
   list. It set up error code in case some error found.
-*/
/******************************************************************************/
ValNodePtr
/*FCN*/MedArchCitMatchList (
  ValNodePtr pub,
  Int4Ptr    pErrCode
){
    register ValNodePtr pPmIdList = NULL; /* result PubMed ID list */
    Int4 i;
    short erract;
    ErrDesc err;

    for ( i = 0; i < MEDARCH_SERV_RETRIES; i++ ) {
        if ( i > 0 ) {
            if ( !ReestablishNetMedArch() ) {
                break;
            }
        }
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);
        pPmIdList = s_MedArchCitMatchList(pub,pErrCode);
        ErrSetOpts(erract, 0);
        if ( !ErrFetch(&err) ) {
            break; /* success */
        }
    }

    return pPmIdList;
}                                  /* MedArchCitMatchList() */

/******************************************************************************/
/*.doc MedArchGetMriUids(external) */
/*+
   This function call MedArch server and send MRI code. Server will return
   list of articles(MuId's) that are present in this issue. This function
   will return NULL if MRI not found or some errors found. Caller must
   free list of medline id's.
-*/
/******************************************************************************/
ValNodePtr
/*FCN*/MedArchGetMriUids (
  Int4 mri                     /* MRI code (I) */
){
    register ValNodePtr pMuIdList = NULL; /* result Medline ID list */
    Int4 i;                               /* temp count */
    short erract;                         /* values for save error code */
    ErrDesc err;

    for (i = 0; i < MEDARCH_SERV_RETRIES; i++) {
        if ( i > 0 ) {
            if ( !ReestablishNetMedArch() ) {
                break;
            }
        }
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);
        pMuIdList = s_MedArchGetMriUids (mri);
        ErrSetOpts(erract, 0);
        if ( !ErrFetch(&err) ) {
            break; /* success */
        }
    }

    return pMuIdList;
}                              /* MedArchGetMriUids() */

/******************************************************************************/
/*.doc MedArchGetMriPmids(external) */
/*+
   This function call MedArch server and send MRI code. Server will return
   list of articles(MuId's) that are present in this issue. This function
   will return NULL if MRI not found or some errors found. Caller must
   free list of PubMed id's.
-*/
/******************************************************************************/
ValNodePtr
/*FCN*/MedArchGetMriPmids (
  Int4 mri                     /* MRI code (I) */
){
    register ValNodePtr pMuIdList = NULL; /* result Medline ID list */
    Int4 i;                               /* temp count */
    short erract;                         /* values for save error code */
    ErrDesc err;

    for (i = 0; i < MEDARCH_SERV_RETRIES; i++) {
        if ( i > 0 ) {
            if ( !ReestablishNetMedArch() ) {
                break;
            }
        }
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);
        pMuIdList = s_MedArchGetMriPmids (mri);
        ErrSetOpts(erract, 0);
        if ( !ErrFetch(&err) ) {
            break; /* success */
        }
    }

    return pMuIdList;
}                              /* MedArchGetMriPmids() */

/******************************************************************************/
/*.doc MedArchGetAccUids(external) */
/*+
   This function call MedArch server and send Xref and Accession. 
   Server will return list of articles(MuId's) that are present in this issue. 
   plAccNum is an optional argument in accordance with
   Medline-si definition. Set it to NULL if accession code is unknown.
   It returns NULL if there are no entries found or if some error occured.-*/
/******************************************************************************/
ValNodePtr
/*FCN*/MedArchGetAccUids (
  Int4 iCode,                  /* Medline-si.type, */
                               /* type of xref (I) */
  CharPtr lpAccNum             /* Medline.si.cit, */
                               /* the citation/accession number (I) */
){
    register ValNodePtr pMuIdList = NULL; /* result Medline ID list */
    Int4 i;                               /* temp count */
    short erract;                         /* values for save error code */
    ErrDesc err;

    for (i = 0; i < MEDARCH_SERV_RETRIES; i++) {
        if ( i > 0 ) {
            if ( !ReestablishNetMedArch() ) {
                break;
            }
        }
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);
        pMuIdList = s_MedArchGetAccUids (iCode, lpAccNum);
        ErrSetOpts(erract, 0);
        if ( !ErrFetch(&err) ) {
            break; /* success */
        }
    }

    return pMuIdList;
}                              /* MedArchGetAccUids() */

/******************************************************************************/
/*.doc MedArchGetAccPmids(external) */
/*+
   This function call MedArch server and send Xref and Accession. 
   Server will return list of PmIds that are present in this issue. 
   plAccNum is an optional argument in accordance with
   Medline-si definition. Set it to NULL if accession code is unknown.
   It returns NULL if there are no entries found or if some error occured.-*/
/******************************************************************************/
ValNodePtr
/*FCN*/MedArchGetAccPmids (
  Int4 iCode,                  /* Medline-si.type, */
                               /* type of xref (I) */
  CharPtr lpAccNum             /* Medline.si.cit, */
                               /* the citation/accession number (I) */
){
    register ValNodePtr pmIdList = NULL; /* result Medline ID list */
    Int4 i;                               /* temp count */
    short erract;                         /* values for save error code */
    ErrDesc err;

    for (i = 0; i < MEDARCH_SERV_RETRIES; i++) {
        if ( i > 0 ) {
            if ( !ReestablishNetMedArch() ) {
                break;
            }
        }
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);
        pmIdList = s_MedArchGetAccPmids (iCode, lpAccNum);
        ErrSetOpts(erract, 0);
        if ( !ErrFetch(&err) ) {
            break; /* success */
        }
    }

    return pmIdList;
}                              /* MedArchGetAccPmids() */

/******************************************************************************/
/*.doc MedArchPm2Mu(external) */
/*+
  Convert PubMed id to Medline id
-*/
/******************************************************************************/
Int4
/*FCN*/MedArchPm2Mu (
  Int4 pmid                    /* PubMed id (I) */
){
    Int4 muid = 0;
    Int4 i;                               /* temp count */
    short erract;                         /* values for save error code */
    ErrDesc err;

    for (i = 0; i < MEDARCH_SERV_RETRIES; i++) {
        if ( i > 0 ) {
            if ( !ReestablishNetMedArch() ) {
                break;
            }
        }
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);
        muid = s_MedArchPm2Mu (pmid);
        ErrSetOpts(erract, 0);
        if ( !ErrFetch(&err) ) {
            break; /* success */
        }
    }

    return muid;
}                              /* MedArchPm2Mu() */

/******************************************************************************/
/*.doc MedArchMu2Pm(external) */
/*+
  Convert Medline id to PubMed id
-*/
/******************************************************************************/
Int4
/*FCN*/MedArchMu2Pm (
  Int4 uid                     /* Medline id (I) */
){
    Int4 pmid = 0;
    Int4 i;                               /* temp count */
    short erract;                         /* values for save error code */
    ErrDesc err;

    for (i = 0; i < MEDARCH_SERV_RETRIES; i++) {
        if ( i > 0 ) {
            if ( !ReestablishNetMedArch() ) {
                break;
            }
        }
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);
        pmid = s_MedArchMu2Pm (uid);
        ErrSetOpts(erract, 0);
        if ( !ErrFetch(&err) ) {
            break; /* success */
        }
    }

    return pmid;
}                              /* MedArchMu2Pm() */

/******************************************************************************/
/*.doc MedArchMedlarsEntryGetUid(external) */
/*+
   Retrive single MEDLARS entry from database
-*/
/******************************************************************************/
MedlarsEntryPtr
/*FCN*/MedArchMedlarsEntryGetUid (
  Int4 uid
){
    MedlarsEntryPtr retval = NULL;
    Int4 i;
    short erract;
    ErrDesc err;

    for (i = 0; i < MEDARCH_SERV_RETRIES; i++) {
        if ( i > 0 ) {
            if ( !ReestablishNetMedArch() ) {
                break;
            }
        }

        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);
        retval = s_MedArchMedlarsEntryGetUid(uid);
        ErrSetOpts(erract, 0);
        if ( !ErrFetch(&err) ) {
            break;                   /* success */
        }
    }

    return retval;
}                                    /* MedArchMedlarsEntryGetUid() */

/******************************************************************************/
/*.doc MedArchMedlarsEntryGetPmId(external) */
/*+
   Retrive single MEDLARS entry from database
-*/
/******************************************************************************/
MedlarsEntryPtr
/*FCN*/MedArchMedlarsEntryGetPmId (
  Int4 uid
){
    MedlarsEntryPtr retval = NULL;
    Int4 i;
    short erract;
    ErrDesc err;

    for (i = 0; i < MEDARCH_SERV_RETRIES; i++) {
        if ( i > 0 ) {
            if ( !ReestablishNetMedArch() ) {
                break;
            }
        }

        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);
        retval = s_MedArchMedlarsEntryGetPmId(uid);
        ErrSetOpts(erract, 0);
        if ( !ErrFetch(&err) ) {
            break;                   /* success */
        }
    }

    return retval;
}                                    /* MedArchMedlarsEntryGetPmId() */

#endif /* __MEDARCHC__ */
/*EOF*/
