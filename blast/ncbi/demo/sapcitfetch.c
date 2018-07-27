/*   sapcitfetch.c
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
* File Name:  sapcitfetch.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   11/16/04
*
* $Revision: 1.4 $
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

#include <ncbi.h>
#include <objall.h>
#include <objsset.h>
#include <objsub.h>
#include <objfdef.h>
#include <objmedli.h>
#include <sqnutils.h>
#include <tomedlin.h>
#include <pmfapi.h>

static void FindSfpCitPmids (
  SeqFeatPtr sfp,
  Pointer userdata
)

{
  ValNodePtr PNTR  head;
  Int4             pmid;
  ValNodePtr       ppr;
  ValNodePtr       psp;

  if (sfp == NULL || userdata == NULL) return;
  head = (ValNodePtr PNTR) userdata;

  psp = sfp->cit;
  if (psp != NULL && psp->data.ptrvalue) {
    for (ppr = (ValNodePtr) psp->data.ptrvalue; ppr != NULL; ppr = ppr->next) {
      if (ppr->choice == PUB_PMid) {
        pmid = (Int4) ppr->data.intvalue;
        ValNodeAddInt (head, 0, pmid);
      }
    }
  }
}

static Boolean PmidAlreadyInAnnotDesc (
  Int4 pmid,
  AnnotDescrPtr desc
)

{
  PubdescPtr  pdp;
  ValNodePtr  adp;
  ValNodePtr  vnp;

  for (adp = desc; adp != NULL; adp = adp->next) {
    if (adp->choice != Annot_descr_pub) continue;
    pdp = (PubdescPtr) adp->data.ptrvalue;
    if (pdp == NULL) continue;
    for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
      if (vnp->choice != PUB_PMid) continue;
      if (pmid == vnp->data.intvalue) return TRUE;
    }
  }

  return FALSE;
}

NLM_EXTERN void AsnPrintNewLine PROTO((AsnIoPtr aip));

static void ProcessOneSeqAnnot (
  SeqAnnotPtr sap
)

{
  CitArtPtr        cap;
  ValNodePtr       head;
  MedlineEntryPtr  mep;
  PubdescPtr       pdp;
  PubmedEntryPtr   pep;
  Int4             pmid;
  ValNodePtr       vnp;

  if (sap != NULL) {
    head = NULL;
    VisitFeaturesOnSap (sap, (Pointer) &head, FindSfpCitPmids);
    if (head != NULL) {
      head = ValNodeSort (head, SortByIntvalue);
      head = UniqueIntValNode (head);
      for (vnp = head; vnp != NULL; vnp = vnp->next) {
        pmid = (Int4) vnp->data.intvalue;
        if (pmid < 1) continue;
        if (PmidAlreadyInAnnotDesc (pmid, sap->desc)) continue;
        pep = PubMedSynchronousQuery (pmid);
        if (pep == NULL) {
          ErrPostEx (SEV_ERROR, 0, 0,
                     "PubMedSynchronousQuery failed for pmid %ld",
                     (long) pmid);
          continue;
        }
        mep = (MedlineEntryPtr) pep->medent;
        if (mep != NULL && mep->cit != NULL) {
          cap = AsnIoMemCopy ((Pointer) mep->cit,
                              (AsnReadFunc) CitArtAsnRead,
                              (AsnWriteFunc) CitArtAsnWrite);
          if (cap != NULL) {
            pdp = PubdescNew ();
            if (pdp != NULL) {
              ValNodeAddPointer (&(pdp->pub), PUB_Article, cap);
              ValNodeAddInt (&(pdp->pub), PUB_PMid, pmid);
              pdp->reftype = 2;
              ValNodeAddPointer (&(sap->desc), Annot_descr_pub, pdp);
            }
          }
        }
        pep = PubmedEntryFree (pep);
      }
    }
    head = ValNodeFree (head);
  }
}

static void ProcessSapFile (
  AsnIoPtr aip,
  AsnIoPtr aop
)

{
  Int4         pos;
  SeqAnnotPtr  sap;

  if (aip == NULL || aop == NULL) return;

  sap = SeqAnnotAsnRead (aip, NULL);
  while (sap != NULL) {

    ProcessOneSeqAnnot (sap);

    SeqAnnotAsnWrite (sap, aop, NULL);
    AsnPrintNewLine (aop);
    AsnIoFlush (aop);

    SeqAnnotFree (sap);

    pos = AsnIoTell (aip);
    AsnIoReset (aip);
    AsnIoSeek (aip, pos);
    sap = SeqAnnotAsnRead (aip, NULL);
  }
}

/* Args structure contains command-line arguments */

#define i_argInputFile  0
#define o_argNtOutFile  1

Args myargs [] = {
  {"Input File Name", "stdin", NULL, NULL,
    FALSE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
  {"Output File Name", NULL, NULL, NULL,
    FALSE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},
};

Int2 Main (void)

{
  AsnIoPtr  aip, aop;
  CharPtr   infile, outfile;

  /* standard setup */

  ErrSetFatalLevel (SEV_MAX);
  ErrClearOptFlags (EO_SHOW_USERSTR);
  ErrSetLogfile ("stderr", ELOG_APPEND);
  UseLocalAsnloadDataAndErrMsg ();
  ErrPathReset ();

  if (! AllObjLoad ()) {
    Message (MSG_FATAL, "AllObjLoad failed");
    return 1;
  }
  if (! SubmitAsnLoad ()) {
    Message (MSG_FATAL, "SubmitAsnLoad failed");
    return 1;
  }
  if (! FeatDefSetLoad ()) {
    Message (MSG_FATAL, "FeatDefSetLoad failed");
    return 1;
  }
  if (! SeqCodeSetLoad ()) {
    Message (MSG_FATAL, "SeqCodeSetLoad failed");
    return 1;
  }
  if (! GeneticCodeTableLoad ()) {
    Message (MSG_FATAL, "GeneticCodeTableLoad failed");
    return 1;
  }

  /* process command line arguments */

  if (! GetArgs ("sapcitfetch", sizeof (myargs) / sizeof (Args), myargs)) {
    return 0;
  }

  infile = (CharPtr) myargs [i_argInputFile].strvalue;
  outfile = (CharPtr) myargs [o_argNtOutFile].strvalue;

  if (StringHasNoText (infile)) {
    Message (MSG_FATAL, "Input file name is required");
    return 1;
  }
  if (StringHasNoText (outfile)) {
    Message (MSG_FATAL, "Output file name is required");
    return 1;
  }

  aip = AsnIoOpen (infile, "r");
  if (aip == NULL) {
    Message (MSG_FATAL, "Unable to open input file");
    return 1;
  }

  aop = AsnIoOpen (outfile, "w");
  if (aip == NULL) {
    Message (MSG_FATAL, "Unable to open output file");
    AsnIoClose (aip);
    return 1;
  }

  ProcessSapFile (aip, aop);

  AsnIoClose (aop);
  AsnIoClose (aip);

  return 0;
}

