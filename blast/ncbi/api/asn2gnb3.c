/*   asn2gnb3.c
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
* File Name:  asn2gnb3.c
*
* Author:  Karl Sirotkin, Tom Madden, Tatiana Tatusov, Jonathan Kans,
*          Mati Shomrat
*
* Version Creation Date:   10/21/98
*
* $Revision: 1.28 $
*
* File Description:  New GenBank flatfile generator - work in progress
*
* Modifications:
* --------------------------------------------------------------------------
* ==========================================================================
*/

#include <ncbi.h>
#include <objall.h>
#include <objsset.h>
#include <objsub.h>
#include <objfdef.h>
#include <objpubme.h>
#include <seqport.h>
#include <sequtil.h>
#include <sqnutils.h>
#include <subutil.h>
#include <tofasta.h>
#include <explore.h>
#include <gbfeat.h>
#include <gbftdef.h>
#include <edutil.h>
#include <alignmgr2.h>
#include <asn2gnbi.h>

#ifdef WIN_MAC
#if __profile__
#include <Profiler.h>
#endif
#endif

/* ********************************************************************** */

static void AddHistCommentString (
  IntAsn2gbJobPtr ajp,
  StringItemPtr ffstring,
  CharPtr prefix,
  CharPtr suffix,
  DatePtr dp,
  SeqIdPtr ids
)

{
  Int2      count = 0;
  Char      buf [256];
  Boolean   first;
  Int4      gi = 0;
  SeqIdPtr  sip;
  CharPtr   strd;
  
  if (dp == NULL || ids == NULL || prefix == NULL || suffix == NULL || ffstring == NULL) return;

  strd = asn2gb_PrintDate (dp);
  if (strd == NULL) {
    strd = StringSave ("?");
  }

  for (sip = ids; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_GI) {
      gi = (long) sip->data.intvalue;
      count++;
    }
  }

  if (count > 1) {
    sprintf (buf, "%s or before %s %s", prefix, strd, suffix);
  } else {
    sprintf (buf, "%s %s %s", prefix, strd, suffix);
  }
  FFAddOneString (ffstring, buf, FALSE, FALSE, TILDE_EXPAND);

  MemFree (strd);

  if (gi == 0) {
    FFAddOneString (ffstring, " gi:?", FALSE, FALSE, TILDE_EXPAND);
    return;
  }

  first = TRUE;
  for (sip = ids; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_GI) {
      gi = (long) sip->data.intvalue;
      if (! first) {
        FFAddOneString (ffstring, ",", FALSE, FALSE, TILDE_IGNORE);
      }
      first = FALSE;
      if ( GetWWW(ajp) ) {
        FFAddOneString (ffstring, " gi:", FALSE, FALSE, TILDE_IGNORE);
        FFAddTextToString (ffstring, "<a href=", link_seq, NULL, FALSE, FALSE, TILDE_IGNORE);
        sprintf (buf, "%ld", (long) gi);
        FFAddTextToString (ffstring, "val=", buf, ">", FALSE, FALSE, TILDE_IGNORE);
        FFAddOneString (ffstring, buf, FALSE, FALSE, TILDE_EXPAND);
        FFAddOneString (ffstring, "</a>", FALSE, FALSE, TILDE_IGNORE);
      } else {
        sprintf (buf, " gi:%ld", (long) gi);
        FFAddOneString (ffstring, buf, FALSE, FALSE, TILDE_EXPAND);
      }
    }
  }

  FFAddOneString (ffstring, ".", FALSE, FALSE, TILDE_EXPAND);
}

static void AddHTGSCommentString (
  StringItemPtr ffstring,
  BioseqPtr bsp,
  MolInfoPtr mip
)

{
  CharPtr      buf = NULL;
  Char         buffer [256];
  Int4         buflen = 0;
  DeltaSeqPtr  dsp;
  ValNodePtr   head = NULL;
  Int4         num_s = 0;
  Int4         num_g = 0;
  CharPtr      str = NULL;

  if (bsp == NULL || mip == NULL || mip->tech < 2) return;

  if (bsp->repr == Seq_repr_delta) {
    for (dsp = (DeltaSeqPtr) bsp->seq_ext, buflen = 0; dsp != NULL; dsp = dsp->next) {
      buflen += 80;
    }
    if (buflen > 0) {
      buf = MemNew ((size_t) (buflen + 1));
      if (buf == NULL) return;
      CountGapsInDeltaSeq (bsp, &num_s, &num_g, NULL, NULL, buf, buflen);
    }
  }

  if (mip->tech == MI_TECH_htgs_0) {

    if (num_s > 0) {
      sprintf (buffer, "* NOTE: This record contains %ld individual~", (long) (num_g + 1));
      ValNodeCopyStr (&head, 0, buffer);
      ValNodeCopyStr (&head, 0, "* sequencing reads that have not been assembled into~");
      ValNodeCopyStr (&head, 0, "* contigs. Runs of N are used to separate the reads~");
      ValNodeCopyStr (&head, 0, "* and the order in which they appear is completely~");
      ValNodeCopyStr (&head, 0, "* arbitrary. Low-pass sequence sampling is useful for~");
      ValNodeCopyStr (&head, 0, "* identifying clones that may be gene-rich and allows~");
      ValNodeCopyStr (&head, 0, "* overlap relationships among clones to be deduced.~");
      ValNodeCopyStr (&head, 0, "* However, it should not be assumed that this clone~");
      ValNodeCopyStr (&head, 0, "* will be sequenced to completion. In the event that~");
      ValNodeCopyStr (&head, 0, "* the record is updated, the accession number will~");
      ValNodeCopyStr (&head, 0, "* be preserved.");
    }
    ValNodeCopyStr (&head, 0, "~");
    ValNodeCopyStr (&head, 0, buf);

  } else if (mip->tech == MI_TECH_htgs_1) {

    ValNodeCopyStr (&head, 0, "* NOTE: This is a \"working draft\" sequence.");
    if (num_s > 0) {
      sprintf (buffer, " It currently~* consists of %ld contigs. The true order of the pieces~", (long) (num_g + 1));
      ValNodeCopyStr (&head, 0, buffer);
      ValNodeCopyStr (&head, 0, "* is not known and their order in this sequence record is~");
      ValNodeCopyStr (&head, 0, "* arbitrary. Gaps between the contigs are represented as~");
      ValNodeCopyStr (&head, 0, "* runs of N, but the exact sizes of the gaps are unknown.");
    }
    ValNodeCopyStr (&head, 0, "~* This record will be updated with the finished sequence~");
    ValNodeCopyStr (&head, 0, "* as soon as it is available and the accession number will~");
    ValNodeCopyStr (&head, 0, "* be preserved.");
    ValNodeCopyStr (&head, 0, "~");
    ValNodeCopyStr (&head, 0, buf);

  } else if (mip->tech == MI_TECH_htgs_2) {

    ValNodeCopyStr (&head, 0, "* NOTE: This is a \"working draft\" sequence.");
    if (num_s > 0) {
      sprintf (buffer, " It currently~* consists of %ld contigs. Gaps between the contigs~", (long) (num_g + 1));
      ValNodeCopyStr (&head, 0, buffer);
      ValNodeCopyStr (&head, 0, "* are represented as runs of N. The order of the pieces~");
      ValNodeCopyStr (&head, 0, "* is believed to be correct as given, however the sizes~");
      ValNodeCopyStr (&head, 0, "* of the gaps between them are based on estimates that have~");
      ValNodeCopyStr (&head, 0, "* provided by the submittor.");
    }
    ValNodeCopyStr (&head, 0, "~* This sequence will be replaced~");
    ValNodeCopyStr (&head, 0, "* by the finished sequence as soon as it is available and~");
    ValNodeCopyStr (&head, 0, "* the accession number will be preserved.");
    ValNodeCopyStr (&head, 0, "~");
    ValNodeCopyStr (&head, 0, buf);

  } else if ((str = StringForSeqTech (mip->tech)) != NULL) {

      sprintf (buffer, "Method: %s.", str);
      ValNodeCopyStr (&head, 0, buffer);
  }

  MemFree (buf);

  str = MergeFFValNodeStrs (head);

  FFAddOneString (ffstring, str, TRUE, TRUE, TILDE_EXPAND);

  MemFree (str);
  ValNodeFreeData (head);
}

typedef struct barcodedata {
  CharPtr  orgmods [40];
  CharPtr  subsources [40];
  CharPtr  taxname;
} BarCodeData, PNTR BarCodePtr;

static Boolean AddBarCodeCommentString (
  BioSourcePtr biop,
  BarCodePtr bcp
)

{
  OrgModPtr     omp;
  OrgNamePtr    onp;
  OrgRefPtr     orp;
  SubSourcePtr  ssp;
  Uint1         subtype;

  if (biop == NULL || bcp == NULL) return FALSE;
  orp = biop->org;
  if (orp == NULL) return FALSE;

  MemSet ((Pointer) bcp, 0, sizeof (BarCodeData));

  bcp->taxname = orp->taxname;
  for (ssp = biop->subtype; ssp != NULL; ssp = ssp->next) {
    subtype = ssp->subtype;
    if (subtype == 255) {
      subtype = 37;
    }
    if (subtype < 38) {
      bcp->subsources [subtype] = ssp->name;
    }
  }

  onp = orp->orgname;
  if (onp != NULL) {
    for (omp = onp->mod; omp != NULL; omp = omp->next) {
      subtype = omp->subtype;
      if (subtype == 253) {
        subtype = 35;
      } else if (subtype == 254) {
        subtype = 36;
      } else if (subtype == 255) {
        subtype = 37;
      }
      if (subtype < 38) {
        bcp->orgmods [subtype] = omp->subname;
      }
    }
  }

  return TRUE;
}

static void PrintOneBarCodeElement (
  StringItemPtr ffstring,
  StringItemPtr temp,
  FmtType format,
  CharPtr label,
  CharPtr spaces,
  CharPtr value
)

{
  FFStartPrint (temp, format, 16, 35, NULL, 12, 9, 28, "CC", FALSE);

  FFAddOneString (temp, label, FALSE, FALSE, TILDE_EXPAND);
  if (StringDoesHaveText (value)) {
    FFAddOneString (temp,  spaces, FALSE, FALSE, TILDE_EXPAND);
     FFAddOneString (temp, value, FALSE, FALSE, TILDE_EXPAND);
  }
  FFAddNewLine (temp);

  if (format == GENBANK_FMT || format == GENPEPT_FMT) {
    FFLineWrap(ffstring, temp, 16, 35, ASN2FF_GB_MAX, NULL);
  } else {
    FFLineWrap(ffstring, temp, 9, 28, ASN2FF_EMBL_MAX, "CC");
  }
}

static void AddWGSMasterCommentString (
  StringItemPtr ffstring,
  BioseqPtr bsp,
  CharPtr wgsaccn,
  CharPtr wgsname
)

{
  size_t             acclen;
  BioSourcePtr       biop;
  Char               buf [256];
  SeqMgrDescContext  dcontext;
  CharPtr            first = NULL;
  CharPtr            last = NULL;
  ObjectIdPtr        oip;
  OrgRefPtr          orp;
  SeqDescrPtr        sdp;
  CharPtr            taxname = NULL;
  UserFieldPtr       ufp;
  UserObjectPtr      uop;
  Char               ver [16];

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_source, &dcontext);
  if (sdp != NULL) {
    biop = (BioSourcePtr) sdp->data.ptrvalue;
    if (biop != NULL) {
      orp = biop->org;
      if (orp != NULL) {
        taxname = orp->taxname;
      }
    }
  }

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_user, &dcontext);
  while (sdp != NULL) {
    uop = (UserObjectPtr) sdp->data.ptrvalue;
    if (uop != NULL) {
      oip = uop->type;
      if (oip != NULL && StringICmp (oip->str, "WGSProjects") == 0) {
        for (ufp = uop->data; ufp != NULL; ufp = ufp->next) {
          oip = ufp->label;
          if (oip == NULL || oip->str == NULL || ufp->choice != 1) continue;
          if (StringICmp (oip->str, "WGS_accession_first") == 0) {
            first = (CharPtr) ufp->data.ptrvalue;
          } else if (StringICmp (oip->str, "WGS_accession_last") == 0) {
            last = (CharPtr) ufp->data.ptrvalue;
          }
        }
      }
    }
    sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_user, &dcontext);
  }

  if (StringHasNoText (taxname)) {
    taxname = "?";
  }
  if (StringHasNoText (first)) {
    first = "?";
  }
  if (StringHasNoText (last)) {
    last = "?";
  }
  ver [0] = '\0';
  acclen = StringLen (wgsname);
  if (acclen == 12) {
    StringCpy (ver, wgsname + 4);
    ver [2] = '\0';
  } else if (acclen == 13) {
    StringCpy (ver, wgsname + 4);
    ver [2] = '\0';
  } else if (acclen == 15) {
    StringCpy (ver, wgsname + 7);
    ver [2] = '\0';
  }

  sprintf (buf, "The %s whole genome shotgun (WGS) project has the project accession %s.", taxname, wgsaccn);
  FFAddOneString(ffstring, buf, TRUE, FALSE, TILDE_EXPAND);

  sprintf (buf, "  This version of the project (%s) has the accession number %s,", ver, wgsname);
  FFAddOneString(ffstring, buf, FALSE, FALSE, TILDE_EXPAND);

  if (StringCmp (first, last) != 0) {
    sprintf (buf, " and consists of sequences %s-%s.", first, last);
    FFAddOneString(ffstring, buf, TRUE, FALSE, TILDE_EXPAND);
  } else {
    sprintf (buf, " and consists of sequence %s.", first);
    FFAddOneString(ffstring, buf, TRUE, FALSE, TILDE_EXPAND);
  }
}


static CharPtr GetMolInfoCommentString (
  BioseqPtr bsp,
  MolInfoPtr mip
)

{
  Boolean  is_aa;
  CharPtr  str = NULL;

  if (bsp == NULL || mip == NULL) return NULL;

  is_aa = ISA_aa (bsp->mol);
  switch (mip->completeness) {
    case 1 :
      str = "COMPLETENESS: full length";
      break;
    case 2 :
      str = "COMPLETENESS: not full length";
      break;
    case 3 :
      if (is_aa) {
        str = "COMPLETENESS: incomplete on the amino end";
      } else {
        str = "COMPLETENESS: incomplete on the 5' end";
      }
      break;
    case 4 :
      if (is_aa) {
        str = "COMPLETENESS: incomplete on the carboxy end";
      } else {
        str = "COMPLETENESS: incomplete on the 3' end";
      }
      break;
    case 5 :
      str = "COMPLETENESS: incomplete on both ends";
      break;
    case 6 :
      if (is_aa) {
        str = "COMPLETENESS: complete on the amino end";
      } else {
        str = "COMPLETENESS: complete on the 5' end";
      }
      break;
    case 7 :
      if (is_aa) {
        str = "COMPLETENESS: complete on the carboxy end";
      } else {
        str = "COMPLETENESS: complete on the 3' end";
      }
      break;
    default :
      str = "COMPLETENESS: unknown";
      break;
  }

  return str;
}

static CharPtr GetStrForBankit (
  UserObjectPtr uop
)

{
  CharPtr       bic = NULL, uvc = NULL, ptr;
  ObjectIdPtr   oip;
  UserFieldPtr  ufp;

  if (uop == NULL) return NULL;
  if ((oip = uop->type) == NULL) return NULL;
  if (StringCmp (oip->str, "Submission") != 0) return NULL;

  for (ufp = uop->data; ufp != NULL; ufp = ufp->next) {
    oip = ufp->label;
    if (StringCmp(oip->str, "UniVecComment") == 0) {
      uvc = ufp->data.ptrvalue;
    } else if (StringCmp(oip->str, "AdditionalComment") == 0) {
      bic = ufp->data.ptrvalue;
    }
  }

  if (uvc == NULL && bic == NULL) return NULL;

  ptr = (CharPtr) MemNew (StringLen (uvc) + StringLen (bic) + 45);
  if (uvc != NULL && bic != NULL) {
    sprintf (ptr, "Vector Explanation: %s~Bankit Comment: %s", uvc, bic);
  } else if (uvc != NULL) {
    sprintf (ptr, "Vector Explanation: %s", uvc);
  } else if (bic != NULL) {
    sprintf (ptr, "Bankit Comment: %s", bic);
  }

  return ptr;
}

static CharPtr reftxt0 = " The reference sequence was derived from ";
static CharPtr reftxt1 = " This record is predicted by genome sequence analysis and is not yet supported by experimental evidence.";
static CharPtr reftxt2 = " This record has not yet been subject to final NCBI review.";
static CharPtr reftxt3 = " The mRNA record is supported by experimental evidence; however, the coding sequence is predicted.";
static CharPtr reftxt4 = " This record has undergone preliminary review of the sequence, but has not yet been subject to final review.";
static CharPtr reftxt5 = " This record has been curated by ";
static CharPtr reftxt6 = " This record is predicted by automated computational analysis.";
static CharPtr reftxt7 = " This record is provided to represent a collection of whole genome shotgun sequences.";
static CharPtr reftxt9 = " This record is derived from an annotated genomic sequence (";

static CharPtr GetStatusForRefTrack (
  UserObjectPtr uop
)

{
  CharPtr       st;
  ObjectIdPtr   oip;
  UserFieldPtr  ufp, urf = NULL;

  if (uop == NULL) return NULL;
  if ((oip = uop->type) == NULL) return NULL;
  if (StringCmp (oip->str, "RefGeneTracking") != 0) return NULL;
  for (ufp = uop->data; ufp != NULL; ufp = ufp->next) {
    oip = ufp->label;
    if (StringCmp(oip->str, "Assembly") == 0) {
      urf = ufp;
    }
  }
  /* if (urf == NULL || urf->choice != 11) return NULL; */
  for (ufp = uop->data; ufp != NULL; ufp = ufp->next) {
    oip = ufp->label;
    if (StringCmp (oip->str, "Status") == 0) {
      st = (CharPtr) ufp->data.ptrvalue;
      if (StringICmp (st, "Inferred") == 0) {
        return "INFERRED ";
      } else if (StringICmp (st, "Provisional") == 0) {
        return "PROVISIONAL ";
      } else if (StringICmp (st, "Predicted") == 0) {
        return "PREDICTED ";
      } else if (StringICmp (st, "Validated") == 0) {
        return "VALIDATED ";
      } else if (StringICmp (st, "Reviewed") == 0) {
        return "REVIEWED ";
      } else if (StringICmp (st, "Model") == 0) {
        return "MODEL ";
      } else if (StringICmp (st, "WGS") == 0) {
        return "WGS ";
      } else if (StringICmp (st, "Pipeline") == 0) {
        return "Pipeline ";
      }
    }
  }
  return NULL;
}


static void AddStrForRefTrack (
  IntAsn2gbJobPtr ajp,
  StringItemPtr ffstring,
  UserObjectPtr uop
)

{
  CharPtr       accn, curator = NULL, source = NULL, st;
  ObjectIdPtr   oip;
  UserFieldPtr  ufp, tmp, u, urf = NULL;
  Int2          i = 0;
  Int2          review = 0,len;
  Boolean       is_accn;

  if ( uop == NULL || ffstring == NULL ) return;
  if ((oip = uop->type) == NULL) return;
  if (StringCmp (oip->str, "RefGeneTracking") != 0) return;

  len = StringLen (reftxt0);
  for (ufp = uop->data; ufp != NULL; ufp = ufp->next) {
    oip = ufp->label;
    if (StringCmp(oip->str, "Assembly") == 0) {
      urf = ufp;
    }
    if (StringCmp (oip->str, "Status") == 0) {
      st = (CharPtr) ufp->data.ptrvalue;
      if (StringICmp (st, "Inferred") == 0) {
        review = 1;
      } else if (StringICmp (st, "Provisional") == 0) {
        review = 2;
      } else if (StringICmp (st, "Predicted") == 0) {
        review = 3;
      } else if (StringICmp (st, "Validated") == 0) {
        review = 4;
      } else if (StringICmp (st, "Reviewed") == 0) {
        review = 5;
      } else if (StringICmp (st, "Model") == 0) {
        review = 6;
      } else if (StringICmp (st, "WGS") == 0) {
        review = 7;
      } else if (StringICmp (st, "Pipeline") == 0) {
        review = 8;
      }
    } else if (StringCmp (oip->str, "Collaborator") == 0) {
      st = (CharPtr) ufp->data.ptrvalue;
      if (! StringHasNoText (st)) {
        curator = st;
      }
    } else if (StringCmp (oip->str, "GenomicSource") == 0) {
      st = (CharPtr) ufp->data.ptrvalue;
      if (! StringHasNoText (st)) {
        source = st;
      }
    }
  }
  if (urf != NULL && urf->choice == 11) {
    for (tmp = urf->data.ptrvalue; tmp != NULL; tmp = tmp->next) {
      for (u = tmp->data.ptrvalue; u != NULL; u = u->next) {
        oip = u->label;
        if (StringCmp (oip->str, "accession") == 0 ||
            StringCmp (oip->str, "name") == 0) {
          i++;
        }
      }
    }
  }
  if ( GetWWW(ajp) ) {
    FFAddTextToString(ffstring, "<a href=", ref_link, ">", FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString (ffstring, "REFSEQ", FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString (ffstring, "</a>", FALSE, FALSE, TILDE_IGNORE);
  } else {
    FFAddOneString (ffstring, "REFSEQ", FALSE, FALSE, TILDE_IGNORE);
  }
  FFAddOneString (ffstring, ":", FALSE, FALSE, TILDE_IGNORE);
  if (review == 1) {
    FFAddOneString (ffstring, reftxt1, FALSE, FALSE, TILDE_IGNORE);
  } else if (review == 2) {
    FFAddOneString (ffstring, reftxt2, FALSE, FALSE, TILDE_IGNORE);
  } else if (review == 3) {
    FFAddOneString (ffstring, reftxt3, FALSE, FALSE, TILDE_IGNORE);
  } else if (review == 4) {
    FFAddOneString (ffstring, reftxt4, FALSE, FALSE, TILDE_IGNORE);
  } else if (review == 5) {
    if (curator == NULL) {
      curator = "NCBI staff";
    }
    FFAddOneString (ffstring, reftxt5, FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString (ffstring, curator, FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString (ffstring, ".", FALSE, FALSE, TILDE_IGNORE);
  } else if (review == 6) {
    FFAddOneString (ffstring, reftxt6, FALSE, FALSE, TILDE_IGNORE);
  } else if (review == 7) {
    FFAddOneString (ffstring, reftxt7, FALSE, FALSE, TILDE_IGNORE);
  } else if (review == 8) {
  }
  if (review != 5 && curator != NULL) {
    FFAddOneString (ffstring, reftxt5, FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString (ffstring, curator, FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString (ffstring, ".", FALSE, FALSE, TILDE_IGNORE);
  }
  if (source != NULL) {
    FFAddOneString (ffstring, reftxt9, FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString (ffstring, source, FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString (ffstring, ").", FALSE, FALSE, TILDE_IGNORE);
  }
  if (i > 0) {
    FFAddOneString (ffstring, reftxt0, FALSE, FALSE, TILDE_IGNORE);

    for (tmp = urf->data.ptrvalue; tmp != NULL; tmp = tmp->next) {
      is_accn = TRUE;
      for (u = tmp->data.ptrvalue; u != NULL; u = u->next) {
        oip = u->label;
        if (StringCmp (oip->str, "accession") == 0) break;
        if (StringCmp (oip->str, "name") == 0) {
          is_accn = FALSE;
          break;
        }
      }
      if (u == NULL) continue;
      accn = (CharPtr) u->data.ptrvalue;
      if (StringHasNoText (accn)) continue;
      if (is_accn && GetWWW(ajp) ) {
        FFAddTextToString(ffstring, "<a href=", link_seq, NULL, FALSE, FALSE, TILDE_IGNORE);
        FFAddTextToString(ffstring, "val=", accn, ">", FALSE, FALSE, TILDE_IGNORE);
        FFAddOneString (ffstring, accn, FALSE, FALSE, TILDE_IGNORE);
        FFAddOneString (ffstring, "</a>", FALSE, FALSE, TILDE_IGNORE);
      } else {
        FFAddOneString (ffstring, accn, FALSE, FALSE, TILDE_IGNORE);
      }
      if (tmp->next != NULL) {
        ufp = tmp->next;
        if (ufp->next != NULL) {
          FFAddOneString (ffstring, ", ", FALSE, FALSE, TILDE_IGNORE);
        } else {
          FFAddOneString (ffstring, " and ", FALSE, FALSE, TILDE_IGNORE);
        }
      }
    }
    FFAddOneString (ffstring, ".", FALSE, FALSE, TILDE_EXPAND);
  }
}

static CharPtr GetGenomeBuildNumber (
  UserObjectPtr uop
)

{
  ObjectIdPtr   oip;
  CharPtr       str;
  UserFieldPtr  ufp;

  if (uop == NULL) return NULL;
  if ((oip = uop->type) == NULL) return NULL;
  if (StringCmp (oip->str, "GenomeBuild") != 0) return NULL;
  for (ufp = uop->data; ufp != NULL; ufp = ufp->next) {
    oip = ufp->label;
    if (StringCmp(oip->str, "NcbiAnnotation") == 0) {
      if (ufp->choice == 1) { /* string */
        str = ufp->data.ptrvalue;
        if (! StringHasNoText (str)) return str;
      }
    } else if (StringCmp (oip->str, "Annotation") == 0) {
      if (ufp->choice == 1) { /* string */
        str = ufp->data.ptrvalue;
        if (! StringHasNoText (str)) {
          if (StringNICmp (str, "NCBI build ", 11) == 0) {
            if (! StringHasNoText (str + 11)) {
              return (str + 11);
            }
          }
        }
      }
    }
  }
  return NULL;
}

static CharPtr GetGenomeVersionNumber (
  UserObjectPtr uop
)

{
  ObjectIdPtr   oip;
  CharPtr       str;
  UserFieldPtr  ufp;

  if (uop == NULL) return NULL;
  if ((oip = uop->type) == NULL) return NULL;
  if (StringCmp (oip->str, "GenomeBuild") != 0) return NULL;
  for (ufp = uop->data; ufp != NULL; ufp = ufp->next) {
    oip = ufp->label;
    if (StringCmp(oip->str, "NcbiVersion") == 0) {
      if (ufp->choice == 1) { /* string */
        str = ufp->data.ptrvalue;
        if (! StringHasNoText (str)) return str;
      }
    }
  }
  return NULL;
}


static CharPtr reftxt11 = "This record is predicted by automated computational analysis. This record is derived from an annotated genomic sequence";
static CharPtr reftxt12 = "using gene prediction method:";

static void FindModelEvidenceUop (
  UserObjectPtr uop,
  Pointer userdata
)

{
  ObjectIdPtr         oip;
  UserObjectPtr PNTR  uopp;

  if (uop == NULL || userdata == NULL) return;
  uopp = (UserObjectPtr PNTR) userdata;
  oip = uop->type;
  if (oip == NULL) return;
  if (StringCmp (oip->str, "ModelEvidence") == 0) {
    *uopp = uop;
  }
}

static Boolean DoGetAnnotationComment (
   BioseqPtr bsp,
   CharPtr PNTR namep,
   CharPtr PNTR methodp,
   BoolPtr mrnaEv,
   BoolPtr estEv
)

{
  SeqMgrDescContext  dcontext;
  CharPtr            method = NULL;
  UserObjectPtr      moduop;
  CharPtr            name = NULL;
  ObjectIdPtr        oip;
  SeqDescrPtr        sdp;
  UserFieldPtr       ufp;
  UserObjectPtr      uop;

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_user, &dcontext);
  while (sdp != NULL) {
    uop = (UserObjectPtr) sdp->data.ptrvalue;
    if (uop != NULL) {
      moduop = NULL;
      VisitUserObjectsInUop (uop, (Pointer) &moduop, FindModelEvidenceUop);
      if (moduop != NULL) {
        oip = moduop->type;
        if (oip != NULL && StringCmp(oip->str, "ModelEvidence") == 0) {
          for (ufp = uop->data; ufp != NULL; ufp = ufp->next) {
            oip = ufp->label;
            if (oip == NULL) continue;
            if (StringCmp(oip->str, "Contig Name") == 0) {
              name = (CharPtr) ufp->data.ptrvalue;
            } else if (StringCmp(oip->str, "Method") == 0) {
              method = (CharPtr) ufp->data.ptrvalue;
            } else if (StringCmp(oip->str, "mRNA") == 0) {
              *mrnaEv = TRUE;
            } else if (StringCmp(oip->str, "EST") == 0) {
              *estEv = TRUE;
            }
          }
        }
      }
    }
    sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_user, &dcontext);
  }
  if (StringHasNoText (name)) return FALSE;
  *namep = name;
  if (! StringHasNoText (method)) {
    *methodp = method;
  }
  return TRUE;
}

static Boolean GetAnnotationComment (
   BioseqPtr bsp,
   CharPtr PNTR namep,
   CharPtr PNTR methodp,
   BoolPtr mrnaEv,
   BoolPtr estEv
)

{
  SeqFeatPtr  cds;

  if (DoGetAnnotationComment (bsp, namep, methodp, mrnaEv, estEv)) return TRUE;
  if (ISA_aa (bsp->mol)) {
    cds = SeqMgrGetCDSgivenProduct (bsp, NULL);
    if (cds != NULL) {
      bsp = BioseqFindFromSeqLoc (cds->location);
      if (bsp != NULL) {
        return DoGetAnnotationComment (bsp, namep, methodp, mrnaEv, estEv);
      }
    }
  }
  return FALSE;
}

static void FindGeneFeat (
  SeqFeatPtr sfp,
  Pointer userdata
)

{
  SeqFeatPtr PNTR  sfpp;

  if (sfp->data.choice != SEQFEAT_GENE) return;
  sfpp = (SeqFeatPtr PNTR) userdata;
  *sfpp = sfp;
}

static void FindLocusId (
  ValNodePtr dbxref,
  CharPtr locusIDp
)

{
  DbtagPtr     dbt;
  ObjectIdPtr  oip;
  ValNodePtr   vnp;

  for (vnp = dbxref; vnp != NULL; vnp = vnp->next) {
    dbt = (DbtagPtr) vnp->data.ptrvalue;
    if (dbt == NULL) continue;
    if (StringICmp (dbt->db, "LocusID") != 0 && StringICmp (dbt->db, "InterimID") != 0) continue;
    oip = dbt->tag;
    if (oip == NULL) continue;
    if (oip->str != NULL) {
      StringCpy (locusIDp, oip->str);
    } else if (oip->id > 0) {
      sprintf (locusIDp, "%ld", (long) oip->id);
    }
  }
}

static Boolean GetGeneAndLocus (
  BioseqPtr bsp,
  CharPtr PNTR genep,
  CharPtr locusIDp,
  CharPtr taxIDp
)

{
  BioSourcePtr       biop;
  DbtagPtr           dbt;
  SeqMgrDescContext  dcontext;
  SeqFeatPtr         gene = NULL;
  GeneRefPtr         grp;
  ObjectIdPtr        oip;
  OrgRefPtr          orp;
  SeqDescrPtr        sdp;
  SeqEntryPtr        sep;
  CharPtr            str;
  ValNodePtr         syn;
  ValNodePtr         vnp;

  sep = GetTopSeqEntryForEntityID (bsp->idx.entityID);
  if (sep == NULL) return FALSE;
  VisitFeaturesInSep (sep, (Pointer) &gene, FindGeneFeat);
  if (gene == NULL) return FALSE;

  grp = (GeneRefPtr) gene->data.value.ptrvalue;
  if (grp == NULL) return FALSE;
  if (! StringHasNoText (grp->locus)) {
    *genep = grp->locus;
  } else {
    syn = grp->syn;
    if (syn != NULL) {
      str = (CharPtr) syn->data.ptrvalue;
      if (! StringHasNoText (str)) {
        *genep = str;
      }
    }
  }
  FindLocusId (gene->dbxref, locusIDp);
  FindLocusId (grp->db, locusIDp);

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_source, &dcontext);
  if (sdp != NULL) {
    biop = (BioSourcePtr) sdp->data.ptrvalue;
    if (biop != NULL) {
      orp = biop->org;
      if (orp != NULL) {
        for (vnp = orp->db; vnp != NULL; vnp = vnp->next) {
          dbt = (DbtagPtr) vnp->data.ptrvalue;
          if (dbt == NULL) continue;
          if (StringCmp (dbt->db, "taxon") == 0) {
            oip = dbt->tag;
            if (oip == NULL) continue;
            if (oip->str != NULL) {
              StringCpy (taxIDp, oip->str);
            } else if (oip->id > 0) {
              sprintf (taxIDp, "%ld", (long) oip->id);
            }
          }
        }
      }
    }
  }

  if (genep == NULL || StringHasNoText (locusIDp)) return FALSE;

  return TRUE;
}

static CharPtr reftxt21 = "NCBI contigs are derived from assembled genomic sequence data.";

static CharPtr reftxt22 = "Features on this sequence have been produced for build ";
static CharPtr reftxt23 = " of the NCBI's genome annotation";

static CharPtr nsAreGapsString = "The strings of n's in this record represent gaps between contigs, and the length of each string corresponds to the length of the gap.";
static CharPtr nsWGSGapsString = "The strings of n's in this record represent gaps between contigs or uncallable bases.";

static Boolean IsTpa (
  BioseqPtr bsp,
  Boolean has_tpa_assembly,
  BoolPtr isRefSeqP
)

{
  DbtagPtr  dbt;
  Boolean   has_bankit = FALSE;
  Boolean   has_genbank = FALSE;
  Boolean   has_gi = FALSE;
  Boolean   has_local = FALSE;
  Boolean   has_refseq = FALSE;
  Boolean   has_smart = FALSE;
  Boolean   has_tpa = FALSE;
  SeqIdPtr  sip;

  if (bsp == NULL || bsp->id == NULL) return FALSE;
  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    switch (sip->choice) {
      case SEQID_LOCAL :
        has_local = TRUE;
        break;
      case SEQID_GENBANK :
      case SEQID_EMBL :
      case SEQID_DDBJ :
        has_genbank = TRUE;
        break;
      case SEQID_OTHER :
        has_refseq = TRUE;
        if (isRefSeqP != NULL) {
          *isRefSeqP = TRUE;
        }
        break;
      case SEQID_GI :
        has_gi = TRUE;
        break;
      case SEQID_TPG :
      case SEQID_TPE :
      case SEQID_TPD :
        has_tpa = TRUE;
        break;
      case SEQID_GENERAL :
        dbt = (DbtagPtr) sip->data.ptrvalue;
        if (dbt != NULL) {
          if (StringICmp (dbt->db, "BankIt") == 0) {
            has_bankit = TRUE;
          }
          if (StringICmp (dbt->db, "TMSMART") == 0) {
            has_smart = TRUE;
          }
        }
        break;
      default :
        break;
    }
  }

  if (has_genbank) return FALSE;
  if (has_tpa) return TRUE;
  if (has_refseq) return FALSE;
  if (has_bankit && has_tpa_assembly) return TRUE;
  if (has_smart && has_tpa_assembly) return TRUE;
  if (has_gi) return FALSE;
  if (has_local && has_tpa_assembly) return TRUE;

  return FALSE;
}

static CharPtr GetStrForTpaOrRefSeqHist (
  BioseqPtr bsp
)

{
  Boolean      accn;
  Char         buf [64];
  DbtagPtr     dbt;
  Int4         gi;
  ValNodePtr   head = NULL;
  SeqHistPtr   hist;
  SeqIdPtr     id;
  Boolean      isRefSeq = FALSE;
  Boolean      minus1;
  Boolean      minus2;
  SeqAlignPtr  salp;
  SeqAlignPtr  salptmp;
  SeqIdPtr     sip;
  Int4         start;
  Int4         stop;
  CharPtr      str;
  Char         tmp [80];

  hist = bsp->hist;
  if (hist != NULL && hist->assembly != NULL) {
    salp = SeqAlignListDup (hist->assembly);
    AlnMgr2IndexLite (salp);
    AlnMgr2SortAlnSetByNthRowPos (salp, 1);
    salptmp = (SeqAlignPtr) (salp->segs);
    while (salptmp != NULL) {
      AlnMgr2GetNthSeqRangeInSA (salptmp, 1, &start, &stop);
      sip = AlnMgr2GetNthSeqIdPtr (salptmp, 2);
      if (sip != NULL) {
        id = NULL;
        accn = FALSE;
        if (sip->choice == SEQID_GI) {
          gi = (Int4) sip->data.intvalue;
          if (GetAccnVerFromServer (gi, buf)) {
            accn = TRUE;
          } else {
            id = GetSeqIdForGI (gi);
          }
        } else {
          id = SeqIdDup (sip);
        }
        if (id != NULL || accn) {
          if (head == NULL) {
            if (isRefSeq) {
              ValNodeCopyStr (&head, 0, "REFSEQ_SPAN         PRIMARY_IDENTIFIER PRIMARY_SPAN        COMP");
            } else {
              ValNodeCopyStr (&head, 0, "TPA_SPAN            PRIMARY_IDENTIFIER PRIMARY_SPAN        COMP");
            }
          }
          if (id != NULL) {
            SeqIdWrite (id, buf, PRINTID_TEXTID_ACC_VER, sizeof (buf) - 1);
            if (id->choice == SEQID_GENERAL) {
              dbt = (DbtagPtr) id->data.ptrvalue;
              if (dbt != NULL && StringICmp (dbt->db, "ti") == 0) {
                StringCpy (buf, "TI");
                SeqIdWrite (id, buf + 2, PRINTID_TEXTID_ACC_VER, sizeof (buf) - 3);
              }
            }
          }
          sprintf (tmp, "~%ld-%ld                                        ",
                   (long) (start + 1), (long) (stop + 1));
          /*
          i = 39 - StringLen (buf);
          if (i > 0) {
            tmp [i] = '\0';
          } else {
            tmp [21] = '\0';
          }
          */
          tmp [21] = '\0';
          StringCat (buf, "                                        ");
          buf [18] = '\0';
          StringCat (tmp, buf);
          AlnMgr2GetNthSeqRangeInSA (salptmp, 2, &start, &stop);
          sprintf (buf, " %ld-%ld                                        ",
                   (long) (start + 1), (long) (stop + 1));
          buf [21] = '\0';
          StringCat (tmp, buf);
          minus1 = (Boolean) (AlnMgr2GetNthStrand (salptmp, 1) == Seq_strand_minus);
          minus2 = (Boolean) (AlnMgr2GetNthStrand (salptmp, 2) == Seq_strand_minus);
          if (minus1 || minus2) {
            if (! (minus1 && minus2)) {
              StringCat (tmp, "c");
            }
          }
          ValNodeCopyStr (&head, 0, tmp);
        }
        SeqIdFree (id);
      }
      SeqIdFree (sip);
      salptmp = salptmp->next;
    }
    SeqAlignFree (salp);
  }

  if (head == NULL) return NULL;

  str = MergeFFValNodeStrs (head);
  ValNodeFreeData (head);

  return str;
}

static CharPtr tpaString = "THIRD PARTY ANNOTATION DATABASE: This TPA record uses data from DDBJ/EMBL/GenBank ";

static CharPtr GetStrForTPA (
  UserObjectPtr uop,
  BioseqPtr bsp
)

{
  Char          ch;
  UserFieldPtr  curr;
  SeqHistPtr    hist;
  Int2          i;
  Char          id [41];
  Boolean       isRefSeq = FALSE;
  Int2          j;
  size_t        len;
  ObjectIdPtr   oip;
  CharPtr       ptr;
  CharPtr       str;
  CharPtr       tmp;
  UserFieldPtr  ufp;

  if (uop == NULL) return NULL;
  if ((oip = uop->type) == NULL) return NULL;
  if (StringCmp (oip->str, "TpaAssembly") != 0) return NULL;
  if (bsp == NULL) return NULL;
  hist = bsp->hist;
  if (hist != NULL && hist->assembly != NULL) return NULL;
  if (! IsTpa (bsp, TRUE, &isRefSeq)) return NULL;
  if (isRefSeq) return NULL;

  len = StringLen (tpaString) + StringLen ("entries ") + StringLen ("and ") + 5;
  i = 0;
  for (curr = uop->data; curr != NULL; curr = curr->next) {
    if (curr->choice != 11) continue;
    for (ufp = curr->data.ptrvalue; ufp != NULL; ufp = ufp->next) {
      if (ufp->choice != 1) continue;
      oip = ufp->label;
      if (oip == NULL || StringICmp (oip->str, "accession") != 0) continue;
      str = (CharPtr) ufp->data.ptrvalue;
      if (StringHasNoText (str)) continue;
      len += StringLen (str) + 2;
      i++;
    }
  }
  if (i == 0) return NULL;

  ptr = (CharPtr) MemNew (len);
  if (ptr == NULL) return NULL;
  StringCpy (ptr, tpaString);
  if (i > 1) {
    StringCat (ptr, "entries ");
  } else {
    StringCat (ptr, "entry ");
  }

  j = 0;
  for (curr = uop->data; curr != NULL; curr = curr->next) {
    if (curr->choice != 11) continue;
    for (ufp = curr->data.ptrvalue; ufp != NULL; ufp = ufp->next) {
      if (ufp->choice != 1) continue;
      oip = ufp->label;
      if (oip == NULL || StringICmp (oip->str, "accession") != 0) continue;
      str = (CharPtr) ufp->data.ptrvalue;
      if (StringHasNoText (str)) continue;
      StringNCpy_0 (id, str, sizeof (id));
      tmp = id;
      ch = *tmp;
      while (ch != '\0') {
        if (IS_LOWER (ch)) {
          *tmp = TO_UPPER (ch);
        }
        tmp++;
        ch = *tmp;
      }
      if (j == i - 1 && i > 1) {
        StringCat (ptr, " and ");
      } else if (j > 0) {
        StringCat (ptr, ", ");
      }
      StringCat (ptr, id);
      j++;
    }
  }

  return ptr;
}

static CharPtr GetStrForGenome (
  UserObjectPtr uop,
  BioseqPtr bsp
)

{
  ObjectIdPtr  oip;

  if (uop == NULL) return NULL;
  if ((oip = uop->type) == NULL) return NULL;
  if (StringCmp (oip->str, "GenomeInfo") != 0) return NULL;

  /* !!! need to implement !!! */

  return NULL;
}

NLM_EXTERN void AddPrimaryBlock (
  Asn2gbWorkPtr awp
)

{
  IntAsn2gbJobPtr    ajp;
  Asn2gbSectPtr      asp;
  BaseBlockPtr       bbp = NULL;
  BioseqPtr          bsp;
  SeqMgrDescContext  dcontext;
  GBSeqPtr           gbseq;
  Boolean            has_tpa_assembly = FALSE;
  SeqHistPtr         hist;
  Boolean            isRefSeq = FALSE;
  ObjectIdPtr        oip;
  SeqDescrPtr        sdp;
  CharPtr            str;
  UserObjectPtr      uop;
  StringItemPtr      ffstring;

  if (awp == NULL) return;
  ajp = awp->ajp;
  if (ajp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;
  asp = awp->asp;
  if (asp == NULL) return;

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_user, &dcontext);
  while (sdp != NULL) {
    uop = (UserObjectPtr) sdp->data.ptrvalue;
    if (uop != NULL) {
      oip = uop->type;
      if (oip != NULL) {
        if (StringCmp (oip->str, "TpaAssembly") == 0) {
          has_tpa_assembly = TRUE;
        }
      }
    }
    if (has_tpa_assembly) {
      sdp = NULL;
    } else {
      sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_user, &dcontext);
    }
  }

  if (! IsTpa (bsp, has_tpa_assembly, &isRefSeq)) return;
  hist = bsp->hist;
  if (hist == NULL || hist->assembly == NULL) return;

  ffstring = FFGetString(ajp);
  if ( ffstring == NULL ) return;

  str = GetStrForTpaOrRefSeqHist (bsp);
  if (str != NULL) {

    bbp = (BaseBlockPtr) Asn2gbAddBlock (awp, PRIMARY_BLOCK, sizeof (BaseBlock));
    if (bbp != NULL) {

      if (has_tpa_assembly) {
        bbp->entityID = dcontext.entityID;
        bbp->itemID = dcontext.itemID;
        bbp->itemtype = OBJ_SEQDESC;
      }

      FFStartPrint (ffstring, awp->format, 0, 12, "PRIMARY", 12, 5, 5, "PR", TRUE);

      FFAddOneString (ffstring, str, FALSE, FALSE, TILDE_EXPAND);

      bbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 5, 5, "PR");

      /* optionally populate gbseq for XML-ized GenBank format */

      if (ajp->gbseq) {
        gbseq = &asp->gbseq;
      } else {
        gbseq = NULL;
      }

      if (gbseq != NULL) {
        gbseq->primary = StringSave (str);
      }

      if (awp->afp != NULL) {
        DoImmediateFormat (awp->afp, (BaseBlockPtr) bbp);
      }
    }
    MemFree (str);
  }

  FFRecycleString(ajp, ffstring);
}

static CharPtr reftxt32 = "It is defined by coordinates on the sequence of chromosome";
static CharPtr reftxt33 = "from the";
static CharPtr reftxt34 = "assembly of the human genome (NCBI build";
static CharPtr reftxt35 = ").";

static CharPtr GetEncodeString (
  UserObjectPtr uop,
  BioseqPtr bsp
)

{
  CharPtr            assembly_date = NULL;
  BioSourcePtr       biop;
  CharPtr            chromosome = NULL;
  SeqMgrDescContext  dcontext;
  size_t             len;
  CharPtr            ncbi_annotation = NULL;
  ObjectIdPtr        oip;
  SeqDescrPtr        sdp;
  SubSourcePtr       ssp;
  CharPtr            str;
  UserFieldPtr       ufp;

  if (uop == NULL || bsp == NULL) return NULL;

  for (ufp = uop->data; ufp != NULL; ufp = ufp->next) {
    oip = ufp->label;
    if (oip == NULL || oip->str == NULL || ufp->choice != 1) continue;
    if (StringICmp (oip->str, "AssemblyDate") == 0) {
      assembly_date = (CharPtr) ufp->data.ptrvalue;
    } else if (StringICmp (oip->str, "NcbiAnnotation") == 0) {
      ncbi_annotation = (CharPtr) ufp->data.ptrvalue;
    }
  }

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_source, &dcontext);
  if (sdp != NULL) {
    biop = (BioSourcePtr) sdp->data.ptrvalue;
    if (biop != NULL) {
      for (ssp = biop->subtype; ssp != NULL; ssp = ssp->next) {
        if (ssp->subtype == SUBSRC_chromosome) {
          chromosome = ssp->name;
        }
      }
    }
  }

  if (chromosome == NULL || assembly_date == NULL || ncbi_annotation == NULL) return NULL;

  if (StringHasNoText (chromosome)) {
    chromosome = "?";
  }
  if (StringHasNoText (assembly_date)) {
    assembly_date = "?";
  }
  if (StringHasNoText (ncbi_annotation)) {
    ncbi_annotation = "?";
  }

  len = StringLen (reftxt32) + StringLen (reftxt33) +
        StringLen (reftxt34) + StringLen (reftxt35) +
        StringLen (chromosome) +
        StringLen (assembly_date) +
        StringLen (ncbi_annotation);

  str = (CharPtr) MemNew (sizeof (Char) * (len + 10));
  if (str == NULL) return NULL;

  sprintf (str, "%s %s %s %s %s %s%s", reftxt32, chromosome, reftxt33,
           assembly_date, reftxt34, ncbi_annotation, reftxt35);

  return str;
}

static void GetAnnotComments (
  SeqAnnotPtr sap,
  Pointer userdata
)

{
  AnnotDescrPtr    desc;
  ValNodePtr PNTR  head;
  CharPtr          str;

  head = (ValNodePtr PNTR) userdata;
  if (sap == NULL || head == NULL) return;

  for (desc = sap->desc; desc != NULL; desc = desc->next) {
    str = NULL;
    if (desc->choice == Annot_descr_comment) {
      str = (CharPtr) desc->data.ptrvalue;
    }
    if (StringHasNoText (str)) continue;
    ValNodeCopyStr (head, 0, str);
  }
}

NLM_EXTERN void AddCommentBlock (
  Asn2gbWorkPtr awp
)

{
  size_t             acclen;
  IntAsn2gbJobPtr    ajp;
  BarCodeData        bcd;
  BioSourcePtr       biop;
  BioseqPtr          bsp;
  Char               buf [128];
  CommentBlockPtr    cbp;
  Char               ch;
  Boolean            didGenome = FALSE;
  Boolean            didRefTrack = FALSE;
  Boolean            didTPA = FALSE;
  DbtagPtr           dbt;
  SeqMgrDescContext  dcontext;
  DeltaSeqPtr        dsp;
  Boolean            estEv = FALSE;
  SeqMgrFeatContext  fcontext;
  Boolean            first = TRUE;
  GBBlockPtr         gbp;
  CharPtr            geneName = NULL;
  CharPtr            genomeBuildNumber = NULL;
  CharPtr            genomeVersionNumber = NULL;
  Int4               gi = 0;
  Int4               gsdbid = 0;
  Boolean            has_gaps = FALSE;
  Boolean            hasRefTrackStatus = FALSE;
  ValNodePtr         head;
  SeqHistPtr         hist;
  Boolean            is_collab = FALSE;
  Boolean            is_encode = FALSE;
  Boolean            is_other = FALSE;
  Boolean            is_tpa = FALSE;
  Boolean            is_wgs = FALSE;
  SeqLitPtr          litp;
  ObjectIdPtr        localID = NULL;
  Char               locusID [32];
  CharPtr            method = NULL;
  MolInfoPtr         mip;
  Boolean            mrnaEv = FALSE;
  CharPtr            name = NULL;
  ObjectIdPtr        oip;
  Boolean            okay;
  BioseqPtr          parent;
  SeqDescrPtr        sdp;
  SeqFeatPtr         sfp;
  Boolean            showGBBSource = FALSE;
  SeqIdPtr           sip;
  CharPtr            str;
  Char               taxID [32];
  TextSeqIdPtr       tsip;
  UserObjectPtr      uop;
  ValNodePtr         vnp;
  CharPtr            wgsaccn = NULL;
  CharPtr            wgsname = NULL;
  StringItemPtr      ffstring = NULL, temp = NULL;

  if (awp == NULL) return;
  ajp = awp->ajp;
  if (ajp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;

  ffstring = FFGetString(ajp);
  if ( ffstring ==  NULL ) return;

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_user, &dcontext);
  while (sdp != NULL) {
    uop = (UserObjectPtr) sdp->data.ptrvalue;
    if (uop != NULL) {
      str = GetStatusForRefTrack (uop);
      if (str != NULL) {
        hasRefTrackStatus = TRUE;
      }
      if (genomeBuildNumber == NULL) {
        genomeBuildNumber = GetGenomeBuildNumber (uop);
      }
      if (genomeVersionNumber == NULL) {
        genomeVersionNumber = GetGenomeVersionNumber (uop);
      }
      oip = uop->type;
      if (oip != NULL && StringICmp (oip->str, "ENCODE") == 0) {
        is_encode = TRUE;
      }
    }
    sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_user, &dcontext);
  }

  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_OTHER) {
      tsip = (TextSeqIdPtr) sip->data.ptrvalue;

      if (tsip != NULL) {
        is_other = TRUE;
        if (StringNCmp (tsip->accession, "NC_", 3) == 0) {
          if (! StringHasNoText (genomeBuildNumber)) {
            cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
            if (cbp != NULL) {

              cbp->entityID = awp->entityID;
              cbp->first = first;
              first = FALSE;

              if (cbp->first) {
                FFStartPrint (ffstring, awp->format, 0, 12, "COMMENT", 12, 5, 5, "CC", TRUE);
              } else {
                FFStartPrint (ffstring, awp->format, 0, 12, NULL, 12, 5, 5, "CC", FALSE);
              }

              FFAddOneString (ffstring, "GENOME ANNOTATION ", FALSE, FALSE, TILDE_IGNORE);

              if ( GetWWW(ajp) ) {
                FFAddTextToString (ffstring, "<a href=", ref_link, ">", FALSE, FALSE, TILDE_IGNORE);
              }
              FFAddOneString (ffstring, "REFSEQ", FALSE, FALSE, TILDE_IGNORE);
              if ( GetWWW(ajp) ) {
                FFAddOneString (ffstring, "</a>", FALSE, FALSE, TILDE_IGNORE);
              }
              FFAddOneString (ffstring, ":  ", FALSE, FALSE, TILDE_IGNORE);

              FFAddOneString (ffstring, reftxt22, FALSE, FALSE, TILDE_EXPAND);
              FFAddOneString (ffstring, genomeBuildNumber, FALSE, FALSE, TILDE_EXPAND);
              if (StringHasNoText (genomeVersionNumber)) {
                genomeVersionNumber = "1";
              }
              FFAddOneString (ffstring, " version ", FALSE, FALSE, TILDE_EXPAND);
              FFAddOneString (ffstring, genomeVersionNumber, FALSE, FALSE, TILDE_EXPAND);
              FFAddOneString (ffstring, reftxt23, FALSE, FALSE, TILDE_EXPAND);

              FFAddOneString (ffstring, " [see ", FALSE, FALSE, TILDE_EXPAND);

              if ( GetWWW(ajp) ) {
                FFAddTextToString (ffstring, "<a href=", doc_link, ">", FALSE, FALSE, TILDE_IGNORE);
              }
              FFAddOneString (ffstring, "documentation", FALSE, FALSE, TILDE_IGNORE);
              if ( GetWWW(ajp) ) {
                FFAddOneString (ffstring, "</a>", FALSE, FALSE, TILDE_IGNORE);
              }

              FFAddOneString (ffstring, "].", FALSE, FALSE, TILDE_EXPAND);

              cbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 5, 5, "CC");
              FFRecycleString(ajp, ffstring);
              ffstring = FFGetString(ajp);

              if (awp->afp != NULL) {
                DoImmediateFormat (awp->afp, (BaseBlockPtr) cbp);
              }
            }
          }

        } else if (StringNCmp(tsip->accession, "NT_", 3) == 0 || StringNCmp(tsip->accession, "NW_", 3) == 0) {

          if (is_encode) {
            cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
            if (cbp != NULL) {

              cbp->entityID = awp->entityID;
              cbp->first = first;
              first = FALSE;

              if (cbp->first) {
                FFStartPrint (ffstring, awp->format, 0, 12, "COMMENT", 12, 5, 5, "CC", TRUE);
              } else {
                FFStartPrint (ffstring, awp->format, 0, 12, NULL, 12, 5, 5, "CC", FALSE);
              }

              FFAddOneString (ffstring, "REFSEQ", FALSE, FALSE, TILDE_IGNORE);
              FFAddOneString (ffstring, ":  ", FALSE, FALSE, TILDE_IGNORE);

              FFAddOneString (ffstring, "This record was provided by the ", FALSE, FALSE, TILDE_EXPAND);
              if ( GetWWW(ajp) ) {
                FFAddTextToString (ffstring, "<a href=", link_encode, ">", FALSE, FALSE, TILDE_IGNORE);
              }
              FFAddOneString (ffstring, "ENCODE", FALSE, FALSE, TILDE_EXPAND);
              if ( GetWWW(ajp) ) {
                FFAddOneString (ffstring, "</a>", FALSE, FALSE, TILDE_IGNORE);
              }
              FFAddOneString (ffstring, " project.", FALSE, FALSE, TILDE_EXPAND);

              str = GetEncodeString (uop, bsp);
              if (str != NULL) {
                FFAddOneString (ffstring, "  ", FALSE, FALSE, TILDE_EXPAND);
                FFAddOneString (ffstring, str, FALSE, FALSE, TILDE_EXPAND);
              }
              MemFree (str);

              cbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 5, 5, "CC");
              FFRecycleString(ajp, ffstring);
              ffstring = FFGetString(ajp);

              if (awp->afp != NULL) {
                DoImmediateFormat (awp->afp, (BaseBlockPtr) cbp);
              }
            }

          } else if (! hasRefTrackStatus) {

            cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
            if (cbp != NULL) {

              cbp->entityID = awp->entityID;
              cbp->first = first;
              first = FALSE;

              if (cbp->first) {
                FFStartPrint (ffstring, awp->format, 0, 12, "COMMENT", 12, 5, 5, "CC", TRUE);
              } else {
                FFStartPrint (ffstring, awp->format, 0, 12, NULL, 12, 5, 5, "CC", FALSE);
              }

              FFAddOneString (ffstring, "GENOME ANNOTATION ", FALSE, FALSE, TILDE_IGNORE);

              if ( GetWWW(ajp) ) {
                FFAddTextToString (ffstring, "<a href=", ref_link, ">", FALSE, FALSE, TILDE_IGNORE);
              }
              FFAddOneString (ffstring, "REFSEQ", FALSE, FALSE, TILDE_IGNORE);
              if ( GetWWW(ajp) ) {
                FFAddOneString (ffstring, "</a>", FALSE, FALSE, TILDE_IGNORE);
              }
              FFAddOneString (ffstring, ":  ", FALSE, FALSE, TILDE_IGNORE);

              if (! StringHasNoText (genomeBuildNumber)) {
                FFAddOneString (ffstring, reftxt22, FALSE, FALSE, TILDE_EXPAND);
                FFAddOneString (ffstring, genomeBuildNumber, FALSE, FALSE, TILDE_EXPAND);
                if (StringHasNoText (genomeVersionNumber)) {
                  genomeVersionNumber = "1";
                }
                FFAddOneString (ffstring, " version ", FALSE, FALSE, TILDE_EXPAND);
                FFAddOneString (ffstring, genomeVersionNumber, FALSE, FALSE, TILDE_EXPAND);
                FFAddOneString (ffstring, reftxt23, FALSE, FALSE, TILDE_EXPAND);

                FFAddOneString (ffstring, " [see ", FALSE, FALSE, TILDE_EXPAND);

                if ( GetWWW(ajp) ) {
                  FFAddTextToString (ffstring, "<a href=", doc_link, ">", FALSE, FALSE, TILDE_IGNORE);
                }
                FFAddOneString (ffstring, "documentation", FALSE, FALSE, TILDE_IGNORE);
                if ( GetWWW(ajp) ) {
                  FFAddOneString (ffstring, "</a>", FALSE, FALSE, TILDE_IGNORE);
                }

                FFAddOneString (ffstring, "].", FALSE, FALSE, TILDE_EXPAND);
              } else {

                FFAddOneString (ffstring, reftxt21, TRUE, FALSE, TILDE_EXPAND);

                FFAddOneString (ffstring, "~Also see:~    ", FALSE, FALSE, TILDE_EXPAND);

                if ( GetWWW(ajp) ) {
                  FFAddTextToString (ffstring, "<a href=", doc_link, ">", FALSE, FALSE, TILDE_IGNORE);
                }
                FFAddOneString (ffstring, "Documentation", FALSE, FALSE, TILDE_IGNORE);
                if ( GetWWW(ajp) ) {
                  FFAddOneString (ffstring, "</a>", FALSE, FALSE, TILDE_IGNORE);
                }

                FFAddOneString (ffstring, " of NCBI's Annotation Process~    ", FALSE, FALSE, TILDE_EXPAND);
              }

              cbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 5, 5, "CC");
              FFRecycleString(ajp, ffstring);
              ffstring = FFGetString(ajp);

              if (awp->afp != NULL) {
                DoImmediateFormat (awp->afp, (BaseBlockPtr) cbp);
              }
            }
          }

        } else if (StringNCmp(tsip->accession, "XP_", 3) == 0 ||
                   StringNCmp(tsip->accession, "XM_", 3) == 0 ||
                   StringNCmp(tsip->accession, "XR_", 3) == 0 ||
                   StringNCmp(tsip->accession, "ZP_", 3) == 0) {

          name = NULL;
          method = NULL;
          mrnaEv = FALSE;
          estEv = FALSE;
          if (GetAnnotationComment (bsp, &name, &method, &mrnaEv, &estEv)) {

            cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
            if (cbp != NULL) {

              cbp->entityID = awp->entityID;
              cbp->first = first;
              first = FALSE;

              if (cbp->first) {
                FFStartPrint (ffstring, awp->format, 0, 12, "COMMENT", 12, 5, 5, "CC", TRUE);
              } else {
                FFStartPrint (ffstring, awp->format, 0, 12, NULL, 12, 5, 5, "CC", FALSE);
              }

              FFAddOneString (ffstring, "MODEL ", FALSE, FALSE, TILDE_IGNORE);

              if ( GetWWW(ajp) ) {
                FFAddTextToString (ffstring, "<a href=", ref_link, ">", FALSE, FALSE, TILDE_IGNORE);
              }
              FFAddOneString (ffstring, "REFSEQ", FALSE, FALSE, TILDE_IGNORE);
              if ( GetWWW(ajp) ) {
                FFAddOneString (ffstring, "</a>", FALSE, FALSE, TILDE_IGNORE);
              }
              FFAddOneString (ffstring, ":  ", FALSE, FALSE, TILDE_IGNORE);

              FFAddTextToString (ffstring, NULL, reftxt11, " (", FALSE, FALSE, TILDE_IGNORE);

              if ( GetWWW(ajp) ) {
                FFAddTextToString (ffstring, "<a href=", nt_link, name, FALSE, FALSE, TILDE_IGNORE);
                FFAddOneString (ffstring, ">", FALSE, FALSE, TILDE_IGNORE);
              }
              FFAddOneString (ffstring, name, FALSE, FALSE, TILDE_IGNORE);
              if ( GetWWW(ajp) ) {
                FFAddOneString (ffstring, "</a>", FALSE, FALSE, TILDE_IGNORE);
              }

              FFAddOneString (ffstring, ")", FALSE, FALSE, TILDE_IGNORE);
 
              if (method != NULL) {
                FFAddOneString (ffstring, " ", FALSE, FALSE, TILDE_IGNORE);
                FFAddOneString (ffstring, reftxt12, FALSE, FALSE, TILDE_IGNORE);
                FFAddOneString (ffstring, " ", FALSE, FALSE, TILDE_IGNORE);
                FFAddOneString (ffstring, method, FALSE, FALSE, TILDE_IGNORE);
              }

              if (mrnaEv || estEv) {
                FFAddOneString (ffstring, ", supported by ", FALSE, FALSE, TILDE_IGNORE);
                if (mrnaEv && estEv) {
                  FFAddOneString (ffstring, "mRNA and EST ", FALSE, FALSE, TILDE_IGNORE);
                } else if (mrnaEv) {
                  FFAddOneString (ffstring, "mRNA ", FALSE, FALSE, TILDE_IGNORE);
                } else {
                  FFAddOneString (ffstring, "EST ", FALSE, FALSE, TILDE_IGNORE);
                }
                geneName = NULL;
                locusID [0] = '\0';
                taxID [0] = '\0';
                if ( GetWWW(ajp) && GetGeneAndLocus (bsp, &geneName, locusID, taxID)) {
                  FFAddTextToString (ffstring, "<a href=", ev_link, NULL, FALSE, FALSE, TILDE_IGNORE);
                  FFAddTextToString (ffstring, "contig=", name, NULL, FALSE, FALSE, TILDE_IGNORE);
                  FFAddTextToString (ffstring, "&gene=", geneName, NULL, FALSE, FALSE, TILDE_IGNORE);
                  FFAddTextToString (ffstring, "&lid=", locusID, NULL, FALSE, FALSE, TILDE_IGNORE);
                  if (! StringHasNoText (taxID)) {
                    FFAddTextToString (ffstring, "&taxid=", taxID, NULL, FALSE, FALSE, TILDE_IGNORE);
                  }
                  FFAddOneString (ffstring, ">", FALSE, FALSE, TILDE_IGNORE);
                  FFAddOneString (ffstring, "evidence", FALSE, FALSE, TILDE_IGNORE);
                  FFAddOneString (ffstring, "</a>", FALSE, FALSE, TILDE_IGNORE);
                } else {
                  FFAddOneString (ffstring, "evidence", FALSE, FALSE, TILDE_IGNORE);
                }
              }

              FFAddOneString (ffstring, ".", FALSE, FALSE, TILDE_IGNORE);

              FFAddOneString (ffstring, "~Also see:~    ", FALSE, FALSE, TILDE_EXPAND);

              if ( GetWWW(ajp) ) {
                FFAddTextToString (ffstring, "<a href=", doc_link, ">", FALSE, FALSE, TILDE_IGNORE);
              }
              FFAddOneString (ffstring, "Documentation", FALSE, FALSE, TILDE_IGNORE);
              if ( GetWWW(ajp) ) {
                FFAddOneString (ffstring, "</a>", FALSE, FALSE, TILDE_IGNORE);
              }

              FFAddOneString (ffstring, " of NCBI's Annotation Process~    ", FALSE, FALSE, TILDE_EXPAND);

              cbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 5, 5, "CC");
              FFRecycleString(ajp, ffstring);
              ffstring = FFGetString(ajp);

              if (awp->afp != NULL) {
                DoImmediateFormat (awp->afp, (BaseBlockPtr) cbp);
              }
            }
          }
        } else {
          if (StringLen (tsip->accession) == 15) {
            is_wgs = TRUE;
            if (StringCmp (tsip->accession + 9, "000000") == 0) {
              wgsaccn = tsip->accession;
              wgsname = tsip->name; /* master accession has 8 zeroes, name has project version plus 6 zeroes */
            }
          }
        }
      }

    } else if (sip->choice == SEQID_TPG || sip->choice == SEQID_TPE || sip->choice == SEQID_TPD) {

      is_tpa = TRUE;

    } else if (sip->choice == SEQID_GENBANK || sip->choice == SEQID_EMBL || sip->choice == SEQID_DDBJ) {

      is_collab = TRUE;

      tsip = (TextSeqIdPtr) sip->data.ptrvalue;
      if (tsip != NULL && tsip->accession != NULL) {
        acclen = StringLen (tsip->accession);
        if (acclen == 12) {
          is_wgs = TRUE;
          if (StringCmp (tsip->accession + 6, "000000") == 0) {
            wgsaccn = tsip->accession;
            wgsname = tsip->name; /* master accession has 8 zeroes, name has project version plus 6 zeroes */
          }
        } else if (acclen == 13) {
          is_wgs = TRUE;
          if (StringCmp (tsip->accession + 6, "0000000") == 0) {
            wgsaccn = tsip->accession;
            wgsname = tsip->name; /* master accession has 9 zeroes, name has project version plus 7 zeroes */
          }
        } else if (ajp->newSourceOrg && StringLen (tsip->accession) == 6) {
          ch = tsip->accession [0];
          if (ch == 'J' || ch == 'K' || ch == 'L' || ch == 'M') {
            showGBBSource = TRUE;
          }
        }
      }

    } else if (sip->choice == SEQID_GENERAL) {
      dbt = (DbtagPtr) sip->data.ptrvalue;

      /* show GSDB sequence identifier */

      if (dbt != NULL && StringCmp (dbt->db, "GSDB") == 0 && dbt->tag != NULL) {
        cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
        if (cbp != NULL) {

          cbp->entityID = awp->entityID;
          cbp->first = first;
          first = FALSE;

          /* string will be created after we know if there are additional comments */

          gsdbid = dbt->tag->id;
          sprintf (buf, "GSDB:S:%ld.", (long) gsdbid);

          if (cbp->first) {
            FFStartPrint (ffstring, awp->format, 0, 12, "COMMENT", 12, 5, 5, "CC", TRUE);
          } else {
            FFStartPrint (ffstring, awp->format, 0, 12, NULL, 12, 5, 5, "CC", FALSE);
          }

          /* CheckEndPunctuation, ConvertDoubleQuotes, and ExpandTildes already taken into account */

          FFAddOneString (ffstring, buf, FALSE, FALSE, TILDE_IGNORE);

          cbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 5, 5, "CC");
          FFRecycleString(ajp, ffstring);
          ffstring = FFGetString(ajp);

          if (awp->afp != NULL) {
            DoImmediateFormat (awp->afp, (BaseBlockPtr) cbp);
          }
        }
      }

    } else if (sip->choice == SEQID_GI) {
      gi = (Int4) sip->data.intvalue;

    } else if (sip->choice == SEQID_LOCAL) {
      localID = (ObjectIdPtr) sip->data.ptrvalue;
    }
  }

  if (localID != NULL) {
    if (is_tpa || is_collab) {
      if (awp->mode == SEQUIN_MODE || awp->mode == DUMP_MODE) {
        buf [0] = '\0';
        if (! StringHasNoText (localID->str)) {
          if (StringLen (localID->str) < 100) {
            sprintf (buf, "LocalID: %s", localID->str);
          } else {
            sprintf (buf, "LocalID string too large");
          }
        } else {
          sprintf (buf, "LocalID: %ld", (long) localID->id);
        }

        cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
        if (cbp != NULL) {

          cbp->entityID = awp->entityID;
          cbp->first = first;
          first = FALSE;

          if (cbp->first) {
            FFStartPrint (ffstring, awp->format, 0, 12, "COMMENT", 12, 5, 5, "CC", TRUE);
          } else {
            FFStartPrint (ffstring, awp->format, 0, 12, NULL, 12, 5, 5, "CC", FALSE);
          }

          FFAddOneString (ffstring, buf, FALSE, FALSE, TILDE_EXPAND);

          cbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12,5, 5, "CC");
          FFRecycleString(ajp, ffstring);
          ffstring = FFGetString(ajp);

          if (awp->afp != NULL) {
            DoImmediateFormat (awp->afp, (BaseBlockPtr) cbp);
          }
        }
      }
    }
  }

  /* RefSeq results in allocated comment string */

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_user, &dcontext);
  while (sdp != NULL) {

    uop = (UserObjectPtr) sdp->data.ptrvalue;
    if (uop != NULL) {

      if (! didTPA) {
        str = GetStrForTPA (uop, bsp);
        if (str != NULL) {

          cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
          if (cbp != NULL) {

            cbp->entityID = dcontext.entityID;
            cbp->itemID = dcontext.itemID;
            cbp->itemtype = OBJ_SEQDESC;
            cbp->first = first;
            first = FALSE;

            if (cbp->first) {
              FFStartPrint (ffstring, awp->format, 0, 12, "COMMENT", 12, 5, 5, "CC", TRUE);
            } else {
              FFStartPrint (ffstring, awp->format, 0, 12, NULL, 12, 5, 5, "CC", FALSE);
            }

            FFAddOneString (ffstring, str, FALSE, FALSE, TILDE_EXPAND);

            cbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12,5, 5, "CC");
            FFRecycleString(ajp, ffstring);
            ffstring = FFGetString(ajp);

            if (awp->afp != NULL) {
              DoImmediateFormat (awp->afp, (BaseBlockPtr) cbp);
            }
          }
          MemFree (str);
          didTPA = TRUE;
        }
      }

      if (! ajp->flags.hideBankItComment) {
        str = GetStrForBankit (uop);
        if (str != NULL) {

          cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
          if (cbp != NULL) {

            cbp->entityID = dcontext.entityID;
            cbp->itemID = dcontext.itemID;
            cbp->itemtype = OBJ_SEQDESC;
            cbp->first = first;
            first = FALSE;

            if (cbp->first) {
              FFStartPrint (ffstring, awp->format, 0, 12, "COMMENT", 12, 5, 5, "CC", TRUE);
            } else {
              FFStartPrint (ffstring, awp->format, 0, 12, NULL, 12, 5, 5, "CC", FALSE);
            }

            FFAddOneString (ffstring, str, TRUE, FALSE, TILDE_EXPAND);

            cbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12,5, 5, "CC");
            FFRecycleString(ajp, ffstring);
            ffstring = FFGetString(ajp);

            if (awp->afp != NULL) {
              DoImmediateFormat (awp->afp, (BaseBlockPtr) cbp);
            }
          }
          MemFree (str);
        }
      }

      if (! didRefTrack) {
        str = GetStatusForRefTrack (uop);
        if (str != NULL) {

          cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
          if (cbp != NULL) {

            cbp->entityID = dcontext.entityID;
            cbp->itemID = dcontext.itemID;
            cbp->itemtype = OBJ_SEQDESC;
            cbp->first = first;
            first = FALSE;

            if (cbp->first) {
              FFStartPrint (ffstring, awp->format, 0, 12, "COMMENT", 12, 5, 5, "CC", TRUE);
            } else {
              FFStartPrint (ffstring, awp->format, 0, 12, NULL, 12, 5, 5, "CC", FALSE);
            }

            if (StringICmp (str, "Pipeline ") != 0) {
              FFAddOneString (ffstring, str, FALSE, FALSE, TILDE_EXPAND);
            }

            AddStrForRefTrack (ajp, ffstring, uop);

            cbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12,5, 5, "CC");
            FFRecycleString(ajp, ffstring);
            ffstring = FFGetString(ajp);

            if (awp->afp != NULL) {
              DoImmediateFormat (awp->afp, (BaseBlockPtr) cbp);
            }
          }
          /* do not free static str from GetStatusForRefTrack */
          didRefTrack = TRUE;
        }
      }

      if (! didGenome) {
        str = GetStrForGenome (uop, bsp);
        if (str != NULL) {

          cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
          if (cbp != NULL) {

            cbp->entityID = dcontext.entityID;
            cbp->itemID = dcontext.itemID;
            cbp->itemtype = OBJ_SEQDESC;
            cbp->first = first;
            first = FALSE;

            if (cbp->first) {
              FFStartPrint (ffstring, awp->format, 0, 12, "COMMENT", 12, 5, 5, "CC", TRUE);
            } else {
              FFStartPrint (ffstring, awp->format, 0, 12, NULL, 12, 5, 5, "CC", FALSE);
            }

            FFAddOneString (ffstring, str, FALSE, FALSE, TILDE_EXPAND);

            cbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 5, 5, "CC");
            FFRecycleString(ajp, ffstring);
            ffstring = FFGetString(ajp);

            if (awp->afp != NULL) {
              DoImmediateFormat (awp->afp, (BaseBlockPtr) cbp);
            }
          }
          MemFree (str);
          didGenome = TRUE;
        }
      }
    }
    sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_user, &dcontext);
  }

  if (bsp->repr == Seq_repr_delta && bsp->seq_ext_type == 4 && is_wgs) {
    has_gaps = FALSE;
    for (dsp = (DeltaSeqPtr) bsp->seq_ext; dsp; dsp=dsp->next) {
      if (dsp->choice == 2) {
        litp = (SeqLitPtr) dsp->data.ptrvalue;
        if (litp != NULL) {
          if (litp->seq_data == NULL && litp->length > 0) {
            has_gaps = TRUE;
          }
        }
      }
    }
    if (has_gaps) {
      cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
      if (cbp != NULL) {

        cbp->entityID = awp->entityID;
        cbp->first = first;
        first = FALSE;

        if (cbp->first) {
          FFStartPrint (ffstring, awp->format, 0, 12, "COMMENT", 12, 5, 5, "CC", TRUE);
        } else {
          FFStartPrint (ffstring, awp->format, 0, 12, NULL, 12, 5, 5, "CC", FALSE);
        }

        if (is_wgs) {
          FFAddOneString (ffstring, nsWGSGapsString, TRUE, FALSE, TILDE_EXPAND);
        } else {
          FFAddOneString (ffstring, nsAreGapsString, TRUE, FALSE, TILDE_EXPAND);
        }

        cbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 5, 5, "CC");
        FFRecycleString(ajp, ffstring);
        ffstring = FFGetString(ajp);

        if (awp->afp != NULL) {
          DoImmediateFormat (awp->afp, (BaseBlockPtr) cbp);
        }
      }
    }
  }

  /* Seq-hist results in allocated comment string */

  hist = bsp->hist;
  if (hist != NULL) {

    if (hist->replaced_by_ids != NULL && hist->replaced_by_date != NULL) {

      okay = TRUE;
      for (sip = hist->replaced_by_ids; sip != NULL; sip = sip->next) {
        if (sip->choice == SEQID_GI) {
          if (gi == (Int4) sip->data.intvalue) {
            okay = FALSE;
          }
        }
      }

      if (okay) {
        cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
        if (cbp != NULL) {

          cbp->entityID = awp->entityID;
          cbp->first = first;
          first = FALSE;

          if (cbp->first) {
            FFStartPrint (ffstring, awp->format, 0, 12, "COMMENT", 12, 5, 5, "CC", TRUE);
          } else {
            FFStartPrint (ffstring, awp->format, 0, 12, NULL, 12, 5, 5, "CC", FALSE);
          }

          AddHistCommentString (ajp, ffstring, "[WARNING] On", "this sequence was replaced by a newer version",
                                hist->replaced_by_date, hist->replaced_by_ids);

          cbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 5, 5, "CC");
          FFRecycleString(ajp, ffstring);
          ffstring = FFGetString(ajp);

            if (awp->afp != NULL) {
              DoImmediateFormat (awp->afp, (BaseBlockPtr) cbp);
            }
        }
      }
    }

    if (hist->replace_ids != NULL && hist->replace_date != NULL && awp->mode != SEQUIN_MODE) {

      okay = TRUE;
      for (sip = hist->replace_ids; sip != NULL; sip = sip->next) {
        if (sip->choice == SEQID_GI) {
          if (gi == (Int4) sip->data.intvalue) {
            okay = FALSE;
          }
        }
      }

      if (okay) {
        cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
        if (cbp != NULL) {

          cbp->entityID = awp->entityID;
          cbp->first = first;
          first = FALSE;

          if (cbp->first) {
            FFStartPrint (ffstring, awp->format, 0, 12, "COMMENT", 12, 5, 5, "CC", TRUE);
          } else {
            FFStartPrint (ffstring, awp->format, 0, 12, NULL, 12, 5, 5, "CC", FALSE);
          }

          AddHistCommentString (ajp, ffstring, "On", "this sequence version replaced",
                                hist->replace_date, hist->replace_ids);

          cbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 5, 5, "CC");
          FFRecycleString(ajp, ffstring);
          ffstring = FFGetString(ajp);

          if (awp->afp != NULL) {
            DoImmediateFormat (awp->afp, (BaseBlockPtr) cbp);
          }
        }
      }
    }

  }

  /* just save IDs for comment, maploc, and region descriptors */

  /*
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_comment, &dcontext);
  while (sdp != NULL) {
    if (sdp->data.ptrvalue != NULL) {
      cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
      if (cbp != NULL) {
        cbp->entityID = dcontext.entityID;
        cbp->itemID = dcontext.itemID;
        cbp->itemtype = OBJ_SEQDESC;
        cbp->first = first;
        first = FALSE;

        if (awp->afp != NULL) {
          DoImmediateFormat (awp->afp, (BaseBlockPtr) cbp);
        }
      }
    }
    sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_comment, &dcontext);
  }
  */

  /* WGS master comment goes before comment descriptors */

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &dcontext);
  if (sdp != NULL) {

    mip = (MolInfoPtr) sdp->data.ptrvalue;
    if (mip != NULL) {
      if (mip->tech == MI_TECH_wgs) {

        if (wgsname != NULL) {

          cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
          if (cbp != NULL) {

            /*
            cbp->entityID = dcontext.entityID;
            cbp->itemID = dcontext.itemID;
            cbp->itemtype = OBJ_SEQDESC;
            */
            cbp->entityID = awp->entityID;
            cbp->first = first;
            first = FALSE;

            if (cbp->first) {
              FFStartPrint (ffstring, awp->format, 0, 12, "COMMENT", 12, 5, 5, "CC", TRUE);
            } else {
              FFStartPrint (ffstring, awp->format, 0, 12, NULL, 12, 5, 5, "CC", FALSE);
            }

            AddWGSMasterCommentString (ffstring, bsp, wgsaccn, wgsname);

            cbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 5, 5, "CC");
            FFRecycleString(ajp, ffstring);
            ffstring = FFGetString(ajp);

            if (awp->afp != NULL) {
              DoImmediateFormat (awp->afp, (BaseBlockPtr) cbp);
            }
          }
        }
      }
    }
  }

  if (showGBBSource) {
    sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_genbank, &dcontext);
    if (sdp != NULL) {
      gbp = (GBBlockPtr) sdp->data.ptrvalue;
      if (gbp != NULL && (! StringHasNoText (gbp->source))) {
        cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
        if (cbp != NULL) {

          cbp->entityID = dcontext.entityID;
          cbp->itemID = dcontext.itemID;
          cbp->itemtype = OBJ_SEQDESC;
          cbp->first = first;
          first = FALSE;

          if (cbp->first) {
            FFStartPrint (ffstring, awp->format, 0, 12, "COMMENT", 12, 5, 5, "CC", TRUE);
          } else {
            FFStartPrint (ffstring, awp->format, 0, 12, NULL, 12, 5, 5, "CC", FALSE);
          }

          FFAddOneString (ffstring, "Original source text: ", FALSE, FALSE, TILDE_EXPAND);
          FFAddOneString (ffstring, gbp->source, TRUE, TRUE, TILDE_EXPAND);

          cbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 5, 5, "CC");
          FFRecycleString(ajp, ffstring);
          ffstring = FFGetString(ajp);

          if (awp->afp != NULL) {
            DoImmediateFormat (awp->afp, (BaseBlockPtr) cbp);
          }
        }
      }
    }
  }

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &dcontext);
  if (sdp != NULL) {

    mip = (MolInfoPtr) sdp->data.ptrvalue;
    if (mip != NULL) {
      if (mip->tech == MI_TECH_barcode) {

        sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_source, &dcontext);
        if (sdp != NULL) {
          biop = (BioSourcePtr) sdp->data.ptrvalue;

          if (AddBarCodeCommentString (biop, &bcd)) {

            temp = FFGetString(ajp);
            if ( temp != NULL ) {

              cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
              if (cbp != NULL) {

                cbp->entityID = dcontext.entityID;
                cbp->itemID = dcontext.itemID;
                cbp->itemtype = OBJ_SEQDESC;
                cbp->entityID = awp->entityID;
                cbp->first = first;
                first = FALSE;

                if (cbp->first) {
                  FFStartPrint (temp, awp->format, 0, 12, "COMMENT", 12, 5, 5, "CC", TRUE);
                } else {
                  FFStartPrint (temp, awp->format, 0, 12, NULL, 12, 5, 5, "CC", FALSE);
                }

                FFAddOneString (temp, "Barcode Consortium: Standard Data Elements", FALSE, FALSE, TILDE_EXPAND);
                FFAddNewLine (temp);
                FFAddNewLine (temp);

                if (awp->format == GENBANK_FMT || awp->format == GENPEPT_FMT) {
                  FFLineWrap(ffstring, temp, 12, 12, ASN2FF_GB_MAX, NULL);
                } else {
                  FFLineWrap(ffstring, temp, 5, 5, ASN2FF_EMBL_MAX, "CC");
                }

                FFRecycleString (ajp, temp);
                temp = FFGetString (ajp);

                PrintOneBarCodeElement (ffstring, temp, awp->format, "Organism:", "          ", bcd.taxname);

                FFRecycleString (ajp, temp);
                temp = FFGetString (ajp);

                PrintOneBarCodeElement (ffstring, temp, awp->format, "Collected By:", "      ", bcd.subsources [SUBSRC_collected_by]);

                FFRecycleString (ajp, temp);
                temp = FFGetString (ajp);

                PrintOneBarCodeElement (ffstring, temp, awp->format, "Collection Date:", "   ", bcd.subsources [SUBSRC_collection_date]);

                FFRecycleString (ajp, temp);
                temp = FFGetString (ajp);

                PrintOneBarCodeElement (ffstring, temp, awp->format, "Country:", "           ", bcd.subsources [SUBSRC_country]);

                FFRecycleString (ajp, temp);
                temp = FFGetString (ajp);

                PrintOneBarCodeElement (ffstring, temp, awp->format, "Identified By:", "     ", bcd.subsources [SUBSRC_identified_by]);

                FFRecycleString (ajp, temp);
                temp = FFGetString (ajp);

                PrintOneBarCodeElement (ffstring, temp, awp->format, "Isolate:", "           ", bcd.orgmods [ORGMOD_isolate]);

                FFRecycleString (ajp, temp);
                temp = FFGetString (ajp);

                PrintOneBarCodeElement (ffstring, temp, awp->format, "Lat-Lon:", "           ", bcd.subsources [SUBSRC_lat_lon]);

                FFRecycleString (ajp, temp);
                temp = FFGetString (ajp);

                PrintOneBarCodeElement (ffstring, temp, awp->format, "Specimen Voucher:", "  ", bcd.orgmods [ORGMOD_specimen_voucher]);

                FFRecycleString (ajp, temp);
                temp = FFGetString (ajp);

                PrintOneBarCodeElement (ffstring, temp, awp->format, "Forward Primer:", "    ", bcd.subsources [SUBSRC_fwd_primer_seq]);

                FFRecycleString (ajp, temp);
                temp = FFGetString (ajp);

                PrintOneBarCodeElement (ffstring, temp, awp->format, "Reverse Primer:", "    ", bcd.subsources [SUBSRC_rev_primer_seq]);

                FFRecycleString (ajp, temp);
                temp = FFGetString (ajp);

                PrintOneBarCodeElement (ffstring, temp, awp->format, "Fwd Primer Name:", "   ", bcd.subsources [SUBSRC_fwd_primer_name]);

                FFRecycleString (ajp, temp);
                temp = FFGetString (ajp);

                PrintOneBarCodeElement (ffstring, temp, awp->format, "Rev Primer Name:", "   ", bcd.subsources [SUBSRC_rev_primer_name]);

                FFRecycleString (ajp, temp);
                temp = FFGetString (ajp);

                if (awp->format == GENBANK_FMT || awp->format == GENPEPT_FMT) {
                  /*
                  do not do extra blank line for EMBL format,
                  since it would be followed by blank XX line,
                  and we do not expect subsequent comments in
                  barcode records
                  */
                  FFStartPrint (temp, awp->format, 0, 12, NULL, 12, 5, 5, "CC", FALSE);
                  FFAddNewLine (temp);
                  FFLineWrap(ffstring, temp, 12, 12, ASN2FF_GB_MAX, NULL);
                }

               cbp->string = FFToCharPtr(ffstring);
                FFRecycleString(ajp, ffstring);
                ffstring = FFGetString(ajp);

                if (awp->afp != NULL) {
                  DoImmediateFormat (awp->afp, (BaseBlockPtr) cbp);
                }
              }
              FFRecycleString(ajp, temp);
            }
          }
        }
      }
    }
  }

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_comment, &dcontext);
  while (sdp != NULL) {
    if (sdp->data.ptrvalue != NULL) {
      cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
      if (cbp != NULL) {

        cbp->entityID = dcontext.entityID;
        cbp->itemID = dcontext.itemID;
        cbp->itemtype = OBJ_SEQDESC;
        cbp->first = first;
        first = FALSE;

        if (awp->afp != NULL) {
          DoImmediateFormat (awp->afp, (BaseBlockPtr) cbp);
        }
      }
    }
    sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_comment, &dcontext);
  }

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_maploc, &dcontext);
  while (sdp != NULL) {
    cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
    if (cbp != NULL) {

      cbp->entityID = dcontext.entityID;
      cbp->itemID = dcontext.itemID;
      cbp->itemtype = OBJ_SEQDESC;
      cbp->first = first;
      first = FALSE;

      if (awp->afp != NULL) {
        DoImmediateFormat (awp->afp, (BaseBlockPtr) cbp);
      }
    }
    sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_maploc, &dcontext);
  }

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_region, &dcontext);
  while (sdp != NULL) {
    if (sdp->data.ptrvalue != NULL) {
      cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
      if (cbp != NULL) {

        cbp->entityID = dcontext.entityID;
        cbp->itemID = dcontext.itemID;
        cbp->itemtype = OBJ_SEQDESC;
        cbp->first = first;
        first = FALSE;

        if (awp->afp != NULL) {
          DoImmediateFormat (awp->afp, (BaseBlockPtr) cbp);
        }
      }
    }
    sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_region, &dcontext);
  }

  /* HTGS results in allocated comment string */

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &dcontext);
  if (sdp != NULL) {

    mip = (MolInfoPtr) sdp->data.ptrvalue;
    if (mip != NULL) {
      if (mip->completeness != 0 && is_other) {

        str = GetMolInfoCommentString (bsp, mip);

        if (str != NULL) {
          cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
          if (cbp != NULL) {

            cbp->entityID = dcontext.entityID;
            cbp->itemID = dcontext.itemID;
            cbp->itemtype = OBJ_SEQDESC;
            cbp->first = first;
            first = FALSE;

            if (cbp->first) {
              FFStartPrint (ffstring, awp->format, 0, 12, "COMMENT", 12, 5, 5, "CC", TRUE);
            } else {
              FFStartPrint (ffstring, awp->format, 0, 12, NULL, 12, 5, 5, "CC", FALSE);
            }

            FFAddOneString (ffstring, str, TRUE, FALSE, TILDE_EXPAND);

            cbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 5, 5, "CC");
            FFRecycleString(ajp, ffstring);
            ffstring = FFGetString(ajp);

            if (awp->afp != NULL) {
              DoImmediateFormat (awp->afp, (BaseBlockPtr) cbp);
            }
          }
        }

      }
      if (mip->tech == MI_TECH_htgs_0 ||
          mip->tech == MI_TECH_htgs_1 ||
          mip->tech == MI_TECH_htgs_2) {

        cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
        if (cbp != NULL) {

          /*
          cbp->entityID = dcontext.entityID;
          cbp->itemID = dcontext.itemID;
          cbp->itemtype = OBJ_SEQDESC;
          */
          cbp->entityID = awp->entityID;
          cbp->first = first;
          first = FALSE;

          if (cbp->first) {
            FFStartPrint (ffstring, awp->format, 0, 12, "COMMENT", 12, 5, 5, "CC", TRUE);
          } else {
            FFStartPrint (ffstring, awp->format, 0, 12, NULL, 12, 5, 5, "CC", FALSE);
          }
          
          AddHTGSCommentString (ffstring, bsp, mip);

          cbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 5, 5, "CC");
          FFRecycleString(ajp, ffstring);
          ffstring = FFGetString(ajp);

          if (awp->afp != NULL) {
            DoImmediateFormat (awp->afp, (BaseBlockPtr) cbp);
          }
        }

      } else {
        str = StringForSeqTech (mip->tech);
        if (! StringHasNoText (str)) {

          cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
          if (cbp != NULL) {

            /*
            cbp->entityID = dcontext.entityID;
            cbp->itemID = dcontext.itemID;
            cbp->itemtype = OBJ_SEQDESC;
            */
            cbp->entityID = awp->entityID;
            cbp->first = first;
            first = FALSE;

            if (cbp->first) {
              FFStartPrint (ffstring, awp->format, 0, 12, "COMMENT", 12, 5, 5, "CC", TRUE);
            } else {
              FFStartPrint (ffstring, awp->format, 0, 12, NULL, 12, 5, 5, "CC", FALSE);
            }

            FFAddTextToString (ffstring, "Method: ", str, NULL, TRUE, FALSE, TILDE_EXPAND);

            cbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 5, 5, "CC");
            FFRecycleString(ajp, ffstring);
            ffstring = FFGetString(ajp);

            if (awp->afp != NULL) {
              DoImmediateFormat (awp->afp, (BaseBlockPtr) cbp);
            }
          }
        }
      }
    }
  }

  /* add comment features that are full length on appropriate segment */

  parent = awp->parent;
  if (parent == NULL) return;

  sfp = SeqMgrGetNextFeature (parent, NULL, SEQFEAT_COMMENT, 0, &fcontext);
  while (sfp != NULL) {
    if (fcontext.left == awp->from && fcontext.right == awp->to) {
      cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
      if (cbp != NULL) {

        cbp->entityID = fcontext.entityID;
        cbp->itemID = fcontext.itemID;
        cbp->itemtype = OBJ_SEQFEAT;
        cbp->first = first;
        first = FALSE;

        if (awp->afp != NULL) {
          DoImmediateFormat (awp->afp, (BaseBlockPtr) cbp);
        }
      }
    }
    sfp = SeqMgrGetNextFeature (parent, sfp, SEQFEAT_COMMENT, 0, &fcontext);
  }

  /* look for Seq-annot.desc.comment on annots packaged on current bioseq */

  head = NULL;
  VisitAnnotsOnBsp (bsp, (Pointer) &head, GetAnnotComments);
  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    str = (CharPtr) vnp->data.ptrvalue;
    if (StringHasNoText (str)) continue;
    cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
    if (cbp != NULL) {

      cbp->entityID = awp->entityID;
      cbp->first = first;
      first = FALSE;

      if (cbp->first) {
        FFStartPrint (ffstring, awp->format, 0, 12, "COMMENT", 12, 5, 5, "CC", TRUE);
      } else {
        FFStartPrint (ffstring, awp->format, 0, 12, NULL, 12, 5, 5, "CC", FALSE);
      }

      FFAddOneString (ffstring, str, TRUE, FALSE, TILDE_EXPAND);

      cbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 5, 5, "CC");
      FFRecycleString(ajp, ffstring);
      ffstring = FFGetString(ajp);

      if (awp->afp != NULL) {
        DoImmediateFormat (awp->afp, (BaseBlockPtr) cbp);
      }
    }
  }
  ValNodeFreeData (head);

  FFRecycleString(ajp, ffstring);
}

NLM_EXTERN void AddFeatHeaderBlock (
  Asn2gbWorkPtr awp
)

{
  IntAsn2gbJobPtr ajp;
  BaseBlockPtr    bbp;
  StringItemPtr   ffstring;

  if (awp == NULL) return;
  ajp = awp->ajp;
  if (ajp == NULL) return;

  bbp = Asn2gbAddBlock (awp, FEATHEADER_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  bbp->entityID = awp->entityID;

  if (awp->format != FTABLE_FMT) {
    ffstring = FFGetString(ajp);
    if ( ffstring == NULL ) return;

    FFStartPrint (ffstring, awp->format, 0, 12, "FEATURES", 21, 5, 0, "FH", TRUE);

    if (awp->format == EMBL_FMT || awp->format == EMBLPEPT_FMT) {
      FFAddOneString (ffstring, "Key", FALSE, FALSE, TILDE_IGNORE);
      FFAddNChar(ffstring, ' ', 13 , FALSE);
    }

    FFAddOneString (ffstring, "Location/Qualifiers", FALSE, FALSE, TILDE_TO_SPACES);

    if (awp->format == EMBL_FMT || awp->format == EMBLPEPT_FMT) {
      FFAddNewLine(ffstring);
      FFAddNewLine(ffstring);
    }

    bbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 21, 5, 0, "FH");
    FFRecycleString(ajp, ffstring);
  }

  if (awp->afp != NULL) {
    DoImmediateFormat (awp->afp, bbp);
  }
}

static Uint2 ComputeSourceHash (
  CharPtr key,
  Uint2 start
)

{
  Uint4  h;
  Uint2  M;
  Uint2  S;

  if (key == NULL) return start;

  M = 101; /* prime key */
  S = 256; /* size of alphabet */

  for (h = start; *key != '\0'; key++) {
    h = (S * h + *key) % M;
  }

  return (Uint2) h;
}

static BaseBlockPtr AddSource (
  Asn2gbWorkPtr awp,
  ValNodePtr PNTR head,
  BioSourcePtr biop,
  CharPtr comment
)

{
  BaseBlockPtr    bbp;
  DbtagPtr        dbt;
  Uint2           hash;
  SourceType      idx;
  IntSrcBlockPtr  isp;
  ObjectIdPtr     oip;
  OrgModPtr       omp;
  OrgNamePtr      onp;
  OrgRefPtr       orp;
  SubSourcePtr    ssp;
  CharPtr         str;
  Uint1           subtype;
  Char            tmp [16];
  ValNodePtr      vnp;

  if (awp == NULL || head == NULL || biop == NULL) return NULL;

  bbp = (BaseBlockPtr) MemNew (sizeof (IntSrcBlock));
  if (bbp == NULL) return NULL;
  bbp->blocktype = SOURCEFEAT_BLOCK;
  bbp->section = awp->currsection;

  ValNodeAddPointer (head, 0, bbp);

  isp = (IntSrcBlockPtr) bbp;
  isp->biop = biop;
  isp->is_focus = biop->is_focus;
  if (biop->origin == 5) {
    isp->is_synthetic = TRUE;
  }

  orp = biop->org;
  if (orp == NULL) return bbp;

  isp->orghash = ComputeSourceHash (orp->taxname, 0);
  isp->taxname = orp->taxname;

  hash = 0;
  onp = orp->orgname;
  if (onp != NULL) {
    if (StringICmp (onp->div, "SYN") == 0) {
      isp->is_synthetic = TRUE;
    }
    isp->omp = onp->mod;
    for (omp = onp->mod; omp != NULL; omp = omp->next) {
      subtype = omp->subtype;
      if (subtype == 253) {
        subtype = 35;
      } else if (subtype == 254) {
        subtype = 36;
      } else if (subtype == 255) {
        subtype = 37;
      }
      if (subtype < 38) {
        idx = orgModToSourceIdx [subtype];
        if (idx > 0 && idx < ASN2GNBK_TOTAL_SOURCE) {
          str = asn2gnbk_source_quals [idx].name;
          hash = ComputeSourceHash (str, hash);
          hash = ComputeSourceHash (omp->subname, hash);
        }
      }
    }
  }
  if (comment != NULL) {
    hash = ComputeSourceHash ("note", hash);
    hash = ComputeSourceHash (comment, hash);
  }
  isp->modhash = hash;

  hash = 0;
  for (ssp = biop->subtype; ssp != NULL; ssp = ssp->next) {
    subtype = ssp->subtype;
    if (subtype == 255) {
      subtype = 29;
    }
    if (subtype < 30) {
      idx = subSourceToSourceIdx [subtype];
      if (idx > 0 && idx < ASN2GNBK_TOTAL_SOURCE) {
        str = asn2gnbk_source_quals [idx].name;
        hash = ComputeSourceHash (str, hash);
        hash = ComputeSourceHash (ssp->name, hash);
      }
    }
  }
  isp->subhash = hash;
  isp->ssp = biop->subtype;

  hash = 0;
  for (vnp = orp->db; vnp != NULL; vnp = vnp->next) {
    dbt = (DbtagPtr) vnp->data.ptrvalue;
    if (dbt != NULL) {
      hash = ComputeSourceHash (dbt->db, hash);
      oip = dbt->tag;
      if (oip != NULL) {
        if (oip->str != NULL) {
          hash = ComputeSourceHash (oip->str, hash);
        } else {
          sprintf (tmp, "%ld", (long) oip->id);
          hash = ComputeSourceHash (tmp, hash);
        }
      }
    }
  }
  isp->xrfhash = hash;
  isp->vnp = orp->db;

  return bbp;
}

static int LIBCALLBACK SortSourcesByHash (
  VoidPtr ptr1,
  VoidPtr ptr2
)

{
  Int4            diff;
  IntSrcBlockPtr  isp1;
  IntSrcBlockPtr  isp2;
  ValNodePtr      vnp1;
  ValNodePtr      vnp2;

  if (ptr1 == NULL || ptr2 == NULL) return 0;
  vnp1 = *((ValNodePtr PNTR) ptr1);
  vnp2 = *((ValNodePtr PNTR) ptr2);
  if (vnp1 == NULL || vnp2 == NULL) return 0;
  isp1 = (IntSrcBlockPtr) vnp1->data.ptrvalue;
  isp2 = (IntSrcBlockPtr) vnp2->data.ptrvalue;
  if (isp1 == NULL || isp2 == NULL) return 0;

  if (isp1->is_focus && (! isp2->is_focus)) return -1;
  if (isp2->is_focus && (! isp1->is_focus)) return 1;

  diff = isp1->orghash - isp2->orghash;
  if (diff > 0) return -1;
  if (diff < 0) return 1;

  diff = isp1->xrfhash - isp2->xrfhash;
  if (diff > 0) return -1;
  if (diff < 0) return 1;

  /* sort so that sources with modifiers come first */

  diff = isp1->modhash - isp2->modhash;
  if (diff > 0) return -1;
  if (diff < 0) return 1;

  diff = isp1->subhash - isp2->subhash;
  if (diff > 0) return -1;
  if (diff < 0) return 1;

  /* if all hashes are equal, descriptor comes first */

  if (isp1->is_descriptor && (! isp2->is_descriptor)) {
    return -1;
  } else if (isp2->is_descriptor && (! isp1->is_descriptor)) {
    return 1;
  }

  return 0;
}

static int LIBCALLBACK SortSourcesByPos (
  VoidPtr ptr1,
  VoidPtr ptr2
)

{
  IntSrcBlockPtr  isp1;
  IntSrcBlockPtr  isp2;
  ValNodePtr      vnp1;
  ValNodePtr      vnp2;

  if (ptr1 == NULL || ptr2 == NULL) return 0;
  vnp1 = *((ValNodePtr PNTR) ptr1);
  vnp2 = *((ValNodePtr PNTR) ptr2);
  if (vnp1 == NULL || vnp2 == NULL) return 0;
  isp1 = (IntSrcBlockPtr) vnp1->data.ptrvalue;
  isp2 = (IntSrcBlockPtr) vnp2->data.ptrvalue;
  if (isp1 == NULL || isp2 == NULL) return 0;

  /* descriptor always goes first */

  if (isp1->is_descriptor && (! isp2->is_descriptor)) {
    return -1;
  } else if (isp2->is_descriptor && (! isp1->is_descriptor)) {
    return 1;
  }

  /* feature with smallest left extreme is first */

  if (isp1->left > isp2->left) {
    return 1;
  } else if (isp1->left < isp2->left) {
    return -1;
  }

  /* if same left extreme, shortest source feature is first just for flatfile */

  if (isp1->right > isp2->right) {
    return 1;
  } else if (isp1->right < isp2->right) {
    return -1;
  }

  return 0;
}

/*                                                                   */
/* s_isFuzzyLoc () -- Determines is a location has fuzzy coordinates */
/*                                                                   */

static Boolean s_isFuzzyLoc ( SeqLocPtr pLocation )
{
  SeqIntPtr pIntLocation;

  if (pLocation == NULL)
    return FALSE;

  if (pLocation->choice != SEQLOC_INT)
    return FALSE;

  if (pLocation->data.ptrvalue == NULL)
    return FALSE;

  pIntLocation = (SeqIntPtr) pLocation->data.ptrvalue;

  if ((pIntLocation->if_from != NULL) && (pIntLocation->if_from->choice == 2))
    return TRUE;

  if ((pIntLocation->if_to != NULL) && (pIntLocation->if_to->choice == 2))
    return TRUE;

  return FALSE;
}

static void GetSourcesOnBioseq (
  Asn2gbWorkPtr awp,
  BioseqPtr target,
  BioseqPtr bsp,
  Int4 from,
  Int4 to
)

{
  IntAsn2gbJobPtr    ajp;
  BaseBlockPtr       bbp;
  BioSourcePtr       biop;
  SeqMgrDescContext  dcontext;
  SeqMgrFeatContext  fcontext;
  Boolean            hasNulls;
  Int4               left;
  Boolean            loop = FALSE;
  Int2               idx;
  IntSrcBlockPtr     isp;
  Int4Ptr            ivals;
  SeqLocPtr          newloc;
  Boolean            noLeft;
  Boolean            noRight;
  Int2               numivals;
  Boolean            okay;
  Int4               right;
  SeqDescrPtr        sdp;
  SeqFeatPtr         sfp;
  SeqInt             sint;
  SeqIdPtr           sip;
  Boolean            split;
  Int4               start;
  Int4               stop;
  Uint1              strand;
  ValNode            vn;
  ValNodePtr         vnp;

  if (awp == NULL || target == NULL || bsp == NULL) return;
  ajp = awp->ajp;
  if (ajp == NULL) return;

  if (awp->format != FTABLE_FMT || awp->mode == DUMP_MODE) {

	/* full length loc for descriptors */
  
	sint.from = 0;
	if (ajp->ajp.slp != NULL) {
	  sint.to = SeqLocLen (ajp->ajp.slp) - 1;
	} else {
	  sint.to = bsp->length - 1;
	}
	sint.strand = Seq_strand_plus;
	sint.id = SeqIdStripLocus (SeqIdDup (SeqIdFindBest (bsp->id, 0)));
	sint.if_from = NULL;
	sint.if_to = NULL;
  
	vn.choice = SEQLOC_INT;
	vn.data.ptrvalue = (Pointer) &sint;
	vn.next = NULL;
  
	/* if SWISS-PROT, may have multiple source descriptors */
  
	if (ISA_aa (bsp->mol)) {
	  for (sip = bsp->id; sip != NULL; sip = sip->next) {
		if (sip->choice == SEQID_SWISSPROT) {
		  loop = TRUE;
		}
	  }
	}
  
	sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_source, &dcontext);
	while (sdp != NULL) {
  
	  /* check if descriptor on part already added on segmented bioseq */
  
	  okay = TRUE;
	  for (vnp = awp->srchead; vnp != NULL && okay; vnp = vnp->next) {
		bbp = (BaseBlockPtr) vnp->data.ptrvalue;
		if (bbp != NULL) {
		  if (bbp->entityID == dcontext.entityID &&
			  bbp->itemID == dcontext.itemID &&
			  bbp->itemtype == OBJ_SEQDESC) {
			okay = FALSE;
		  }
		}
	  }
  
	  if (okay) {
		biop = (BioSourcePtr) sdp->data.ptrvalue;
		bbp = AddSource (awp, &(awp->srchead), biop, NULL);
		if (bbp != NULL) {
  
		  bbp->entityID = dcontext.entityID;
		  bbp->itemID = dcontext.itemID;
		  bbp->itemtype = OBJ_SEQDESC;
  
		  isp = (IntSrcBlockPtr) bbp;
		  isp->loc = SeqLocMerge (target, &vn, NULL, FALSE, TRUE, FALSE);
		  isp->left = 0;
		  isp->right = bsp->length - 1;
		  isp->is_descriptor = TRUE;
		}
	  }
  
	  /* if SWISS-PROT, loop through multiple source descriptors */
  
	  if (loop) {
		sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_source, &dcontext);
	  } else {
		sdp = NULL;
	  }
	}
  
	SeqIdFree (sint.id);
  }

  if ((! awp->contig) || awp->showconsource) {

    /* features are indexed on parent if segmented */

    bsp = awp->parent;

    sfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_BIOSRC, 0, &fcontext);
    while (sfp != NULL) {
      ivals = fcontext.ivals;
      numivals = fcontext.numivals;
      if (ivals != NULL && numivals > 0) {

        idx = (numivals - 1) * 2;
        start = ivals [idx];
        stop = ivals [idx + 1];
        if (stop >= from && stop <= to && (ajp->ajp.slp == NULL || SeqLocCompare (sfp->location, ajp->ajp.slp) > 0)) {

          biop = (BioSourcePtr) sfp->data.value.ptrvalue;
          bbp = AddSource (awp, &(awp->srchead), biop, sfp->comment);
          if (bbp != NULL) {

            bbp->entityID = fcontext.entityID;
            bbp->itemID = fcontext.itemID;
            bbp->itemtype = OBJ_SEQFEAT;

            isp = (IntSrcBlockPtr) bbp;
            if (sfp->location != NULL && sfp->location->choice == SEQLOC_PNT) {
              isp->loc = AsnIoMemCopy ((Pointer) sfp->location,
                                       (AsnReadFunc) SeqLocAsnRead,
                                       (AsnWriteFunc) SeqLocAsnWrite);
            } else if (s_isFuzzyLoc (sfp->location)) {
              isp->loc = AsnIoMemCopy ((Pointer) sfp->location,
                                      (AsnReadFunc) SeqLocAsnRead,
                                      (AsnWriteFunc) SeqLocAsnWrite);
            } else if (SeqLocId(sfp->location) == NULL) {
              isp->loc = AsnIoMemCopy ((Pointer) sfp->location,
                                       (AsnReadFunc) SeqLocAsnRead,
                                       (AsnWriteFunc) SeqLocAsnWrite);
            } else {
              CheckSeqLocForPartial (sfp->location, &noLeft, &noRight);
              hasNulls = LocationHasNullsBetween (sfp->location);
              isp->loc = SeqLocMerge (target, sfp->location, NULL, FALSE, TRUE, hasNulls);
              SetSeqLocPartial (isp->loc, noLeft, noRight);
            }
            isp->left = fcontext.left;
            isp->right = fcontext.right;
            isp->comment = sfp->comment;
            if (ajp->ajp.slp != NULL) {
              sip = SeqIdParse ("lcl|dummy");
              left = GetOffsetInBioseq (ajp->ajp.slp, bsp, SEQLOC_LEFT_END);
              right = GetOffsetInBioseq (ajp->ajp.slp, bsp, SEQLOC_RIGHT_END);
              strand = SeqLocStrand (ajp->ajp.slp);
              split = FALSE;
              newloc = SeqLocReMapEx (sip, ajp->ajp.slp, isp->loc, 0, FALSE, ajp->masterStyle);
              /*
              newloc = SeqLocCopyRegion (sip, isp->loc, bsp, left, right, strand, &split);
              */
              SeqIdFree (sip);
              if (newloc != NULL) {
                A2GBSeqLocReplaceID (newloc, ajp->ajp.slp);
                isp->loc = SeqLocFree (isp->loc);
                isp->loc = newloc;
                isp->left = left;
                isp->right = right;
              }
            }
          }
        }
      }

      sfp = SeqMgrGetNextFeature (bsp, sfp, SEQFEAT_BIOSRC, 0, &fcontext);
    }
  }
}

static Boolean LIBCALLBACK GetSourcesOnSeg (
  SeqLocPtr slp,
  SeqMgrSegmentContextPtr context
)

{
  Asn2gbWorkPtr  awp;
  BioseqPtr      bsp;
  Int4           from;
  SeqLocPtr      loc;
  SeqEntryPtr    oldscope;
  SeqEntryPtr    sep;
  SeqIdPtr       sip;
  Int4           to;

  if (slp == NULL || context == NULL) return FALSE;
  awp = (Asn2gbWorkPtr) context->userdata;

  from = context->cumOffset;
  to = from + context->to - context->from;

  sip = SeqLocId (slp);
  if (sip == NULL) {
    loc = SeqLocFindNext (slp, NULL);
    if (loc != NULL) {
      sip = SeqLocId (loc);
    }
  }
  if (sip == NULL) return TRUE;

  /* biosource descriptors only on parts within entity */

  sep = GetTopSeqEntryForEntityID (awp->entityID);
  oldscope = SeqEntrySetScope (sep);
  bsp = BioseqFind (sip);
  SeqEntrySetScope (oldscope);

  if (bsp != NULL) {
    GetSourcesOnBioseq (awp, awp->target, bsp, from, to);
    return TRUE;
  }

  /* if we ever want to fetch remote sources, code goes here */

#if 0
  Uint2          entityID;

  /* may remote fetch genome component if not already in memory */

  bsp = BioseqLockById (sip);

  if (bsp == NULL) return TRUE;

  entityID = ObjMgrGetEntityIDForPointer (bsp);

  if (entityID != awp->entityID) {

    /* if segment not packaged in record, may need to feature index it */

    if (SeqMgrFeaturesAreIndexed (entityID) == 0) {
      SeqMgrIndexFeatures (entityID, NULL);
    }

    /* collect features indexed on the remote bioseq */

    from = 0;
    to = bsp->length - 1;
  }

  GetSourcesOnBioseq (awp, awp->target, bsp, from, to);

  BioseqUnlock (bsp);
#endif

  return TRUE;
}

/* isIdenticalSource() -- Checks to see if two sources are identical */
/*                        by comparing the actual values in the      */
/*                        fields.  This only gets called if the two  */
/*                        sources hashed the same -- it's a double-  */
/*                        check since two non-identical things will  */
/*                        occassionally hash to the same value.      */

static Boolean isIdenticalSource (IntSrcBlockPtr isp1, IntSrcBlockPtr isp2)
{
  OrgModPtr     omp1;
  OrgModPtr     omp2;
  SubSourcePtr  ssp1;
  SubSourcePtr  ssp2;
  ValNodePtr    vnp1;
  ValNodePtr    vnp2;
  ObjectIdPtr   oip1;
  ObjectIdPtr   oip2;
  DbtagPtr      dbt1;
  DbtagPtr      dbt2;

  if (isp1->is_focus != isp2->is_focus)
    return FALSE;

  /* Compare the taxonomy names */

  if (StringICmp(isp1->taxname,isp2->taxname) != 0)
    return FALSE;

  /* Compare the comment */

  if (StringICmp(isp1->comment,isp2->comment) != 0)
    return FALSE;

  /* Compare the org mods */

  omp1 = isp1->omp;
  omp2 = isp2->omp;
  while (omp1 != NULL && omp2 != NULL)
    {
      if (omp1->subtype != omp2->subtype)
        return FALSE;
      if (StringICmp (omp1->subname, omp2->subname) != 0)
        return FALSE;
      omp1 = omp1->next;
      omp2 = omp2->next;
    }

  if (omp1 != NULL || omp2 != NULL)
    return FALSE;

  /* Compare the subtypes */

  ssp1 = isp1->ssp;
  ssp2 = isp2->ssp;

  while (ssp1 != NULL && ssp2 != NULL)
    {
      if (ssp1->subtype != ssp2->subtype)
        return FALSE;
      if (StringICmp(ssp1->name,ssp2->name) != 0)
        return FALSE;
      ssp1 = ssp1->next;
      ssp2 = ssp2->next;
    }

  if (ssp1 != NULL || ssp2 != NULL)
    return FALSE;

  /* Compare the DB tags */

  vnp1 = isp1->vnp;
  vnp2 = isp2->vnp;

  while (vnp1 != NULL && vnp2 != NULL)
    {
      dbt1 = (DbtagPtr) vnp1->data.ptrvalue;
      dbt2 = (DbtagPtr) vnp2->data.ptrvalue;

      if ((dbt1 != NULL) && (dbt2 != NULL)) {
        if (dbt1->db != dbt2->db)
          return FALSE;

        oip1 = dbt1->tag;
        oip2 = dbt2->tag;
        if ((oip1 != NULL) && (oip2 != NULL)) {
          if (oip1->str != NULL) {
            if (StringICmp(oip1->str, oip2->str) != 0)
              return FALSE;
          } else  {
            if (oip1->id != oip2->id)
              return FALSE;
          }
        }
        else if (oip1 != NULL)
          return FALSE;
        else if (oip2 != NULL)
          return FALSE;
      }
      else if (dbt1 != NULL)
        return FALSE;
      else if (dbt2 != NULL)
        return FALSE;

      vnp1 = vnp1->next;
      vnp2 = vnp2->next;
    }

  if (vnp1 != NULL || vnp2 != NULL)
    return FALSE;

  /* If it passed all checks, then they */
  /* are the same, so return true.      */

  return TRUE;
}

static void CleanupPackedSeqInt (SeqLocPtr location)

{
  SeqLocPtr  head = NULL;
  SeqIntPtr  loc;
  SeqIntPtr  sintp;
  SeqLocPtr  slp;

  if (location == NULL || location->choice != SEQLOC_PACKED_INT || location->data.ptrvalue == NULL) return;

  slp = SeqLocFindNext (location, NULL);
  while (slp != NULL) {
    if (slp->choice == SEQLOC_INT) {
      sintp = (SeqIntPtr) slp->data.ptrvalue;
      if (sintp != NULL) {
        loc = AsnIoMemCopy (sintp, (AsnReadFunc) SeqIntAsnRead,
                            (AsnWriteFunc) SeqIntAsnWrite);
        ValNodeAddPointer (&head, SEQLOC_INT, loc);
      }
    }
    slp = SeqLocFindNext (location, slp);
  }
  if (head == NULL) return;

  location->data.ptrvalue = SeqLocFree (location->data.ptrvalue);
  location->data.ptrvalue = head;

  slp = location->data.ptrvalue;
  if (slp == NULL || slp->next != NULL) return;
    /* here seqloc_packed_int points to a single location element, so no need for seqloc_packed_int parent */
    location->choice = slp->choice;
    location->data.ptrvalue = (Pointer) slp->data.ptrvalue;
    MemFree (slp);
}


NLM_EXTERN void AddSourceFeatBlock (
  Asn2gbWorkPtr awp
)

{
  IntAsn2gbJobPtr    ajp;
  Asn2gbSectPtr      asp;
  BaseBlockPtr       bbp;
  BioseqPtr          bsp;
  SeqFeatPtr         cds;
  SeqMgrFeatContext  context;
  BioseqPtr          dna;
  SeqLocPtr          duploc;
  Boolean            excise;
  GBFeaturePtr       gbfeat = NULL;
  GBSeqPtr           gbseq;
  ValNodePtr         head = NULL;
  IntSrcBlockPtr     isp;
  IntSrcBlockPtr     lastisp;
  IntSrcBlockPtr     descrIsp;
  ValNodePtr         next;
  ValNodePtr         PNTR prev;
  SeqInt             sint;
  SeqLocPtr          slp;
  CharPtr            str;
  BioseqPtr          target;
  ValNode            vn;
  ValNodePtr         vnp;
  Boolean            descHasFocus = FALSE;
  StringItemPtr      ffstring;

  if (awp == NULL) return;
  ajp = awp->ajp;
  if (ajp == NULL) return;
  asp = awp->asp;
  if (asp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;

  ffstring = FFGetString(ajp);
  if ( ffstring == NULL ) return;


  /* collect biosources on bioseq */

  awp->srchead = NULL;
  GetSourcesOnBioseq (awp, bsp, bsp, awp->from, awp->to);
  target = bsp;

  if (bsp->repr == Seq_repr_seg) {

    /* collect biosource descriptors on local parts */

    SeqMgrExploreSegments (bsp, (Pointer) awp, GetSourcesOnSeg);
    target = awp->target;
  }

  if (awp->srchead == NULL && ISA_aa (bsp->mol)) {

    /* if protein with no sources, get sources applicable to DNA location of CDS */

    cds = SeqMgrGetCDSgivenProduct (bsp, &context);
    if (cds != NULL) {
      dna = BioseqFindFromSeqLoc (cds->location);
      if (dna != NULL) {
        GetSourcesOnBioseq (awp, dna, dna, context.left, context.right);
        target = dna;
      }
    }
  }

  head = awp->srchead;
  awp->srchead = NULL;

  if (head == NULL && (awp->format != FTABLE_FMT || awp->mode == DUMP_MODE)) {

    if (ajp->gbseq) {
      gbseq = &asp->gbseq;
    } else {
      gbseq = NULL;
    }

    sint.from = 0;
    sint.to = bsp->length - 1;
    sint.strand = Seq_strand_plus;
    sint.id = SeqIdStripLocus (SeqIdDup (SeqIdFindBest (bsp->id, 0)));
    sint.if_from = NULL;
    sint.if_to = NULL;

    vn.choice = SEQLOC_INT;
    vn.data.ptrvalue = (Pointer) &sint;
    vn.next = NULL;

    FFStartPrint (ffstring, awp->format, 5, 21, NULL, 0, 5, 21, "FT", FALSE);
    FFAddOneString(ffstring, "source", FALSE, FALSE, TILDE_IGNORE);
    FFAddNChar(ffstring, ' ', 21 - 5 - StringLen("source"), FALSE);

    if (gbseq != NULL) {
      gbfeat = GBFeatureNew ();
      if (gbfeat != NULL) {
        gbfeat->key = StringSave ("source");
      }
    }

    str = FFFlatLoc (ajp, bsp, &vn, (Boolean) (awp->style == MASTER_STYLE));
    if ( GetWWW(ajp) ) {
      FF_www_featloc (ffstring, str);
    } else {
      FFAddOneString (ffstring, str, FALSE, FALSE, TILDE_IGNORE);
    }

    if (gbseq != NULL) {
      if (gbfeat != NULL) {
        if (! StringHasNoText (str)) {
          gbfeat->location = StringSave (str);
        } else {
          gbfeat->location = StringSave ("");
        }
      }
    }

    MemFree (str);

    if (ajp->flags.needOrganismQual) {
      FFAddNewLine(ffstring);
      FFAddTextToString (ffstring, "/organism=\"", "unknown", "\"", FALSE, TRUE, TILDE_TO_SPACES);
#ifdef ASN2GNBK_PRINT_UNKNOWN_ORG
    } else {
      FFAddNewLine(ffstring);
      FFAddTextToString (ffstring, "/organism=\"", "unknown", "\"", FALSE, TRUE, TILDE_TO_SPACES);
#endif
    }

    str = GetMolTypeQual (bsp);
    if (str == NULL) {
      switch (bsp->mol) {
        case Seq_mol_dna :
          str = "unassigned DNA";
          break;
        case Seq_mol_rna :
          str = "unassigned RNA";
          break;
        case Seq_mol_aa :
          break;
        default :
          str = "unassigned DNA";
          break;
      }
    }
    if (str != NULL) {
      FFAddNewLine(ffstring);
      FFAddTextToString (ffstring, "/mol_type=\"", str, "\"", FALSE, TRUE, TILDE_TO_SPACES);
    }

    str = FFEndPrint(ajp, ffstring, awp->format, 5, 21, 5, 21, "FT");

    bbp = (BaseBlockPtr) Asn2gbAddBlock (awp, SOURCEFEAT_BLOCK, sizeof (IntSrcBlock));
    if (bbp != NULL) {
      bbp->section = awp->currsection;
      bbp->string = str;
    } else {
      MemFree(str);
    }
    FFRecycleString(ajp, ffstring);

    if (awp->afp != NULL) {
      DoImmediateFormat (awp->afp, (BaseBlockPtr) bbp);
    }

    /* optionally populate gbseq for XML-ized GenBank format */

    if (gbseq != NULL) {
      if (gbfeat != NULL) {
        AddFeatureToGbseq (gbseq, gbfeat, str, NULL);
      }
    }
  }

  if (head == NULL) return;

  /* sort by hash values */

  head = SortValNode (head, SortSourcesByHash);

  /* unique sources, excise duplicates from list */

  prev = &(head);
  vnp = head;
  lastisp = NULL;
  while (vnp != NULL) {
    excise = FALSE;
    next = vnp->next;
    isp = (IntSrcBlockPtr) vnp->data.ptrvalue;
    if (isp->is_descriptor && isp->is_focus)
      descHasFocus = TRUE;
    if (lastisp != NULL) {
      if (isp != NULL) {
        if (lastisp->is_focus == isp->is_focus &&
            lastisp->orghash == isp->orghash &&
            lastisp->xrfhash == isp->xrfhash) {

          /* check for identical modifiers */

          if (lastisp->modhash == isp->modhash &&
              lastisp->subhash == isp->subhash) {

            excise = isIdenticalSource (isp, lastisp);

          /* or modifiers only in lastisp (e.g., on part bioseq) */

          } else if (isp->modhash == 0 && isp->subhash == 0) {
            excise = isIdenticalSource (isp, lastisp);
          }
        }
      }
    }
    if (excise) {
      *prev = vnp->next;
      vnp->next = NULL;

      /* combine locations of duplicate sources */

      if (lastisp != NULL) {
        slp = SeqLocMerge (target, lastisp->loc, isp->loc, FALSE, TRUE, FALSE);
        lastisp->loc = SeqLocFree (lastisp->loc);
        lastisp->loc = slp;
        lastisp->left = MIN (lastisp->left,isp->left);
        lastisp->right = MAX (lastisp->right, isp->right);
      }

      /* and remove duplicate source */

      SeqLocFree (isp->loc);
      MemFree (isp);
      ValNodeFree (vnp);

    } else {

      prev = &(vnp->next);
      lastisp = isp;
    }
    vnp = next;
  }

  /* Sort again, by location this time */

  head = SortValNode (head, SortSourcesByPos);

  /* If the descriptor has a focus, then subtract */
  /* out all the other source locations.          */

  descrIsp = (IntSrcBlockPtr) head->data.ptrvalue; /* Sorted 1st by now */

  if ((descHasFocus) && (! descrIsp->is_synthetic)) {

    vnp = head;
    duploc = AsnIoMemCopy ((Pointer) descrIsp->loc,
                           (AsnReadFunc) SeqLocAsnRead,
                           (AsnWriteFunc) SeqLocAsnWrite);
    vnp = vnp->next;
    while (vnp != NULL) {
      isp = (IntSrcBlockPtr) vnp->data.ptrvalue;
      if (SeqLocAinB (descrIsp->loc, isp->loc) >= 0) {
        vnp = NULL; /* break the chain */
        descrIsp->loc = SeqLocFree (descrIsp->loc);
        descrIsp->loc = duploc;
        duploc = NULL;
      } else {
        descrIsp->loc = SeqLocSubtract (descrIsp->loc, isp->loc);
        vnp = vnp->next;
      }
    }
    CleanupPackedSeqInt (descrIsp->loc);
    descrIsp->left  = SeqLocStart (descrIsp->loc);
    descrIsp->right = SeqLocStop (descrIsp->loc);
    SeqLocFree (duploc);
  }

  /* if features completely subtracted descriptor
     intervals, suppress in release, entrez modes */

  if (descrIsp->loc == NULL && ajp->flags.hideEmptySource && head->next != NULL) {
    vnp = head->next;
    head->next = NULL;
    ValNodeFreeData (head);
    head = vnp;
  }

  /* finally link into blocks for current section */

  ValNodeLink (&(awp->lastblock), head);
  vnp = awp->lastblock;
  if (vnp == NULL) return;
  while (vnp->next != NULL) {
    vnp = vnp->next;
  }

  awp->lastblock = vnp;
  if (awp->blockList == NULL) {
    awp->blockList = vnp;
  }
  FFRecycleString(ajp, ffstring);

  if (awp->afp != NULL) {
    for (vnp = head; vnp != NULL; vnp = vnp->next) {
      isp = (IntSrcBlockPtr) vnp->data.ptrvalue;
      if (isp == NULL) continue;
      DoImmediateFormat (awp->afp, (BaseBlockPtr) isp);
    }
  }

}

static Boolean IsCDD (
  SeqFeatPtr sfp
)

{
  DbtagPtr    dbt;
  ValNodePtr  vnp;

  for (vnp = sfp->dbxref; vnp != NULL; vnp = vnp->next) {
    dbt = (DbtagPtr) vnp->data.ptrvalue;
    if (dbt != NULL && StringCmp (dbt->db, "CDD") == 0) return TRUE;
  }

  return FALSE;
}

static void GetFeatsOnCdsProduct (
  SeqFeatPtr cds,
  BioseqPtr nbsp,
  BioseqPtr pbsp,
  IntAsn2gbJobPtr ajp,
  Asn2gbWorkPtr awp
)

{
  FeatBlockPtr       fbp;
  IntFeatBlockPtr    ifp;
  Boolean            isRefSeq;
  Int4               lastleft;
  Int4               lastright;
  SeqAnnotPtr        lastsap;
  SeqFeatPtr         lastsfp;
  SeqLocPtr          location;
  SeqLocPtr          newloc;
  SeqMgrFeatContext  pcontext;
  SeqFeatPtr         prt;
  SeqIdPtr           sip;
  SeqLocPtr          slp;
  Boolean            suppress;

  if (cds == NULL || ajp == NULL || awp == NULL) return;
  if (nbsp == NULL || pbsp == NULL || (! ISA_aa (pbsp->mol))) return;

  if (awp->hideCdsProdFeats) return;

  isRefSeq = FALSE;
  for (sip = nbsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_OTHER) {
      isRefSeq = TRUE;
    }
  }

  /* explore mat_peptides, sites, etc. */

  lastsfp = NULL;
  lastsap = NULL;
  lastleft = 0;
  lastright = 0;

  prt = SeqMgrGetNextFeature (pbsp, NULL, 0, 0, &pcontext);
  while (prt != NULL) {

    if (pcontext.featdeftype == FEATDEF_REGION ||
        pcontext.featdeftype == FEATDEF_SITE ||
        pcontext.featdeftype == FEATDEF_BOND ||
        pcontext.featdeftype == FEATDEF_mat_peptide_aa ||
        pcontext.featdeftype == FEATDEF_sig_peptide_aa ||
        pcontext.featdeftype == FEATDEF_transit_peptide_aa ||
        (pcontext.featdeftype == FEATDEF_preprotein /* && isRefSeq */)) {

      if (awp->hideCddFeats && pcontext.featdeftype == FEATDEF_REGION && IsCDD (prt)) {

        /* passing this test prevents mapping of COG CDD region features */

      } else if (pcontext.dnaStop >= awp->from && pcontext.dnaStop <= awp->to) {

        /* suppress duplicate features (on protein) */

        suppress = FALSE;
        if (lastsfp != NULL && lastsap != NULL) {
          if (lastsfp->idx.subtype == prt->idx.subtype &&
              lastleft == pcontext.left &&
              lastright == pcontext.right) {
              if (lastsap == pcontext.sap ||
                  (lastsap->desc == NULL && pcontext.sap->desc == NULL)) {
              if (AsnIoMemComp (lastsfp, prt, (AsnWriteFunc) SeqFeatAsnWrite)) {
                suppress = TRUE;
              }
            }
          }
        }

        /* make sure feature maps within nucleotide sublocation */

        if (! suppress) {
          if (ajp->ajp.slp != NULL) {
            location = aaFeatLoc_to_dnaFeatLoc (cds, prt->location);
            slp = SeqLocMerge (nbsp, location, NULL, FALSE, TRUE, FALSE);
            if (slp != NULL) {
              sip = SeqIdParse ("lcl|dummy");
              newloc = SeqLocReMapEx (sip, ajp->ajp.slp, slp, 0, FALSE, ajp->masterStyle);
              SeqIdFree (sip);
              SeqLocFree (slp);
              if (newloc == NULL) {
                suppress = TRUE;
              }
              SeqLocFree (newloc);
            } else {
              suppress = TRUE;
            }
            SeqLocFree (location);
          }
        }

        if (! suppress) {

          fbp = (FeatBlockPtr) Asn2gbAddBlock (awp, FEATURE_BLOCK, sizeof (IntFeatBlock));
          if (fbp != NULL) {

            fbp->entityID = pcontext.entityID;
            fbp->itemID = pcontext.itemID;
            fbp->itemtype = OBJ_SEQFEAT;
            fbp->featdeftype = pcontext.featdeftype;
            ifp = (IntFeatBlockPtr) fbp;
            ifp->mapToNuc = TRUE;
            ifp->mapToProt = FALSE;
            ifp->mapToGen = FALSE;
            ifp->mapToMrna = FALSE;
            ifp->mapToPep = FALSE;
            ifp->firstfeat = awp->firstfeat;
            awp->firstfeat = FALSE;

            if (awp->afp != NULL) {
              DoImmediateFormat (awp->afp, (BaseBlockPtr) fbp);
            }
          }
        }

        lastsfp = prt;
        lastsap = pcontext.sap;
        lastleft = pcontext.left;
        lastright = pcontext.right;

      }
    }
    prt = SeqMgrGetNextFeature (pbsp, prt, 0, 0, &pcontext);
  }
}

static Boolean NotEMBLorDDBJ (
  BioseqPtr bsp
)

{
  SeqIdPtr  sip;

  if (bsp == NULL) return TRUE;
  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_EMBL || sip->choice == SEQID_TPE) return FALSE;
    if (sip->choice == SEQID_DDBJ || sip->choice == SEQID_TPD) return FALSE;
  }
  return TRUE;
}

static Boolean LIBCALLBACK GetFeatsOnBioseq (
  SeqFeatPtr sfp,
  SeqMgrFeatContextPtr fcontext
)

{
  IntAsn2gbJobPtr    ajp;
  Asn2gbSectPtr      asp;
  Asn2gbWorkPtr      awp;
  BioseqPtr          bsp;
  Char               buf [41];
  SeqFeatPtr         cds;
  SeqMgrFeatContext  cdscontext;
  FeatBlockPtr       fbp;
  SeqLocPtr          firstslp;
  GBQualPtr          gbq;
  SeqFeatPtr         gene;
  Int4               gi;
  GeneRefPtr         grp;
  Boolean            juststop = FALSE;
  IntCdsBlockPtr     icp;
  Int2               idx;
  IntFeatBlockPtr    ifp;
  Int4Ptr            ivals;
  Int2               j;
  SeqAnnotPtr        lastsap;
  SeqFeatPtr         lastsfp;
  SeqLocPtr          lastslp;
  SeqLocPtr          newloc;
  Int2               numivals;
  Boolean            okay;
  SeqEntryPtr        oldscope;
  BioseqPtr          parent;
  Boolean            partial5;
  Boolean            partial3;
  ValNodePtr         ppr;
  BioseqPtr          prod;
  Boolean            pseudo = FALSE;
  SeqEntryPtr        sep;
  SeqIdPtr           sip;
  SeqLocPtr          slp;
  Int4               start;
  Int4               stop;
  TextSeqIdPtr       tsip;
  ValNodePtr         vnp;
  /*
  SeqMgrDescContext  dcontext;
  PubdescPtr         pdp;
  SeqDescrPtr        sdp;
  */

  if (sfp == NULL || fcontext == NULL) return FALSE;
  awp = (Asn2gbWorkPtr) fcontext->userdata;
  if (awp == NULL) return FALSE;
  ajp = awp->ajp;
  if (ajp == NULL) return FALSE;
  asp = awp->asp;
  if (asp == NULL) return FALSE;
  bsp = asp->bsp;
  if (bsp == NULL) return FALSE;

  if (fcontext->featdeftype == FEATDEF_PUB ||
      fcontext->featdeftype == FEATDEF_NON_STD_RESIDUE ||
      fcontext->featdeftype == FEATDEF_RSITE ||
      fcontext->featdeftype == FEATDEF_SEQ) return TRUE;

  if (fcontext->featdeftype == FEATDEF_BIOSRC) return TRUE;

  if (ajp->flags.validateFeats &&
      (fcontext->featdeftype == FEATDEF_BAD ||
       fcontext->featdeftype == FEATDEF_virion)) {
    return TRUE;
  }

  if (ISA_na (bsp->mol) && fcontext->featdeftype == FEATDEF_HET) return TRUE;

  /* check feature customization flags */

  if (awp->hideImpFeats && sfp->data.choice == SEQFEAT_IMP) return TRUE;
  if (awp->hideSnpFeats && fcontext->featdeftype == FEATDEF_variation) return TRUE;
  if (awp->hideExonFeats && fcontext->featdeftype == FEATDEF_exon) return TRUE;
  if (awp->hideIntronFeats && fcontext->featdeftype == FEATDEF_intron) return TRUE;
  if (awp->hideMiscFeats && fcontext->featdeftype == FEATDEF_misc_feature) return TRUE;
  if (awp->hideGaps && fcontext->featdeftype == FEATDEF_gap) return TRUE;
  if (awp->hideRemImpFeats && sfp->data.choice == SEQFEAT_IMP) {
    if (fcontext->featdeftype != FEATDEF_variation &&
        fcontext->featdeftype != FEATDEF_exon &&
        fcontext->featdeftype != FEATDEF_intron &&
        fcontext->featdeftype != FEATDEF_misc_feature) return TRUE;
  }
  if (ISA_aa (bsp->mol) && fcontext->featdeftype == FEATDEF_REGION &&
      awp->hideCddFeats && IsCDD (sfp)) return TRUE;

  /* DDBJ does not want to show gene features */

  if (fcontext->seqfeattype == SEQFEAT_GENE && awp->hideGeneFeats) return TRUE;

  /* suppress comment features that are full length */

  if (fcontext->seqfeattype == SEQFEAT_COMMENT &&
      fcontext->left == awp->from && fcontext->right == awp->to) return TRUE;

  ivals = fcontext->ivals;
  numivals = fcontext->numivals;

  /* check to see if last interval is on this awp->from - awp->to range */

  if (ivals != NULL && numivals > 0) {
    idx = (numivals - 1) * 2;
    start = ivals [idx];
    stop = ivals [idx + 1];
    if (stop < awp->from || stop > awp->to) {

      /* may need to map sig_peptide on a different segment */

      if (fcontext->seqfeattype == SEQFEAT_CDREGION) {
        sip = SeqLocIdForProduct (sfp->product);
        bsp = BioseqFind (sip);
        GetFeatsOnCdsProduct (sfp, asp->bsp, bsp, ajp, awp);
      }

      if (! awp->showAllFeats) return TRUE;

      /* if showing one segment, only show features covering this segment */

      if (fcontext->right < awp->from || fcontext->left > awp->to) return TRUE;

    } else if (fcontext->farloc && NotEMBLorDDBJ (awp->bsp)) {

      /* last interval may not have been mapped to bioseq if far */

      firstslp = NULL;
      lastslp = NULL;

      slp = SeqLocFindNext (sfp->location, NULL);
      while (slp != NULL) {
        if (slp->choice != SEQLOC_NULL) {
          lastslp = slp;
          if (firstslp == NULL) {
            firstslp = slp;
          }
        }
        slp = SeqLocFindNext (sfp->location, slp);
      }

      /* !!! EMBL may have different desired behavior on where to map !!! */

      if (firstslp != NULL && SeqLocStrand (firstslp) == Seq_strand_minus) {
        slp = firstslp;
      } else {
        slp = lastslp;
      }

      if (slp != NULL) {
        sip = SeqLocId (slp);
        if (sip != NULL) {
          bsp = BioseqFindCore (sip);
          if (bsp == NULL || (bsp != awp->parent && bsp != awp->bsp)) {

            return TRUE;
          }
        }
      }
    }
  }

  /* make sure feature is within sublocation */

  if (ajp->ajp.slp != NULL) {
    if (SeqLocCompare (sfp->location, ajp->ajp.slp) == SLC_NO_MATCH) {
      slp = SeqLocMerge (bsp, sfp->location, NULL, FALSE, TRUE, FALSE);
      if (slp == NULL) return TRUE;
      sip = SeqIdParse ("lcl|dummy");
      newloc = SeqLocReMapEx (sip, ajp->ajp.slp, slp, 0, FALSE, ajp->masterStyle);
      SeqIdFree (sip);
      SeqLocFree (slp);
      if (newloc == NULL) return TRUE;
      SeqLocFree (newloc);
    }
  }

  /* suppress duplicate features (on nucleotide) */

  lastsfp = awp->lastsfp;
  lastsap = awp->lastsap;
  if (lastsfp != NULL && lastsap != NULL) {
    if (lastsfp->idx.subtype == sfp->idx.subtype &&
        awp->lastleft == fcontext->left &&
        awp->lastright == fcontext->right) {
        if (lastsap == fcontext->sap ||
            (lastsap->desc == NULL && fcontext->sap->desc == NULL)) {
        if (AsnIoMemComp (lastsfp, sfp, (AsnWriteFunc) SeqFeatAsnWrite)) {
          return TRUE;
        }
      }
    }
  }

  /* if RELEASE_MODE, verify that features have all mandatory qualifiers */

  if (ajp->flags.needRequiredQuals) {
    okay = FALSE;

    switch (fcontext->featdeftype) {

    case FEATDEF_CDS:
      if (ajp->flags.checkCDSproductID) {
        /* non-pseudo CDS must have /product */
        if (sfp->pseudo) {
          pseudo = TRUE;
        }
        grp = SeqMgrGetGeneXref (sfp);
        if (grp == NULL) {
          sep = GetTopSeqEntryForEntityID (ajp->ajp.entityID);
          oldscope = SeqEntrySetScope (sep);
          gene = SeqMgrGetOverlappingGene (sfp->location, NULL);
          SeqEntrySetScope (oldscope);
          if (gene != NULL) {
            grp = (GeneRefPtr) gene->data.value.ptrvalue;
            if (gene->pseudo) {
              pseudo = TRUE;
            }
          }
        }
        if (grp != NULL && grp->pseudo) {
          pseudo = TRUE;
        }
        if (sfp->location != NULL) {
          if (CheckSeqLocForPartial (sfp->location, &partial5, &partial3)) {
            if (partial5 && (! partial3)) {
              if (SeqLocLen (sfp->location) <= 5) {
                juststop = TRUE;
              }
            }
          }
        }
        if (pseudo || juststop) {
          okay = TRUE;
        } else if (sfp->product != NULL) {
          sip = SeqLocIdForProduct (sfp->product);
          if (sip != NULL) {
            if ((sip->choice == SEQID_GI && sip->data.intvalue > 0) ||
                sip->choice == SEQID_LOCAL) {
              sep = GetTopSeqEntryForEntityID (ajp->ajp.entityID);
              oldscope = SeqEntrySetScope (sep);
              prod = BioseqFind (sip);
              SeqEntrySetScope (oldscope);
              if (prod != NULL) {
                for (sip = prod->id; sip != NULL; sip = sip->next) {
                  if (sip->choice == SEQID_GENBANK ||
                     sip->choice == SEQID_EMBL ||
                      sip->choice == SEQID_DDBJ ||
                      sip->choice == SEQID_OTHER ||
                      sip->choice == SEQID_PATENT ||
                      sip->choice == SEQID_TPG ||
                      sip->choice == SEQID_TPE ||
                      sip->choice == SEQID_TPD ||
                      sip->choice == SEQID_GPIPE) {
                    tsip = (TextSeqIdPtr) sip->data.ptrvalue;
                    if (tsip != NULL && (! StringHasNoText (tsip->accession))) {
                      if (ValidateAccn (tsip->accession) == 0)
                      okay = TRUE;
                    }
                  }
                }
              } else if (sip->choice == SEQID_GI && sip->data.intvalue > 0) {
                /* RELEASE_MODE requires that /protein_id is an accession */
                gi = sip->data.intvalue;
                if (GetAccnVerFromServer (gi, buf)) {
                  okay = TRUE;
                } else {
                  sip = GetSeqIdForGI (gi);
                  if (sip != NULL) {
                    okay = TRUE;
                  }
                }
              }
            } else if (sip->choice == SEQID_GENBANK ||
                       sip->choice == SEQID_EMBL ||
                       sip->choice == SEQID_DDBJ ||
                       sip->choice == SEQID_OTHER ||
                       sip->choice == SEQID_PATENT ||
                       sip->choice == SEQID_TPG ||
                       sip->choice == SEQID_TPE ||
                       sip->choice == SEQID_TPD ||
                       sip->choice == SEQID_GPIPE) {
              tsip = (TextSeqIdPtr) sip->data.ptrvalue;
              if (tsip != NULL && (! StringHasNoText (tsip->accession))) {
                if (ValidateAccn (tsip->accession) == 0)
                okay = TRUE;
              }
            }
          }
        } else {
          if (sfp->excpt && (! StringHasNoText (sfp->except_text))) {
            if (StringStr (sfp->except_text, "rearrangement required for product") != NULL) {
              okay = TRUE;
            }
          }
        }
      } else {
        okay = TRUE;
      }
      if (! okay) {
        ajp->relModeError = TRUE;
      }
      break;

    case FEATDEF_conflict:
      if (sfp->cit == NULL) {
        /* RefSeq allows conflict with accession in comment instead of sfp->cit */
        for (sip = bsp->id; sip != NULL; sip = sip->next) {
          if (sip->choice == SEQID_OTHER) {
            if (! StringHasNoText (sfp->comment)) {
              okay = TRUE;
            }
          }
        }
      }
      /* continue on to old_sequence */
    case FEATDEF_old_sequence:
      /* conflict and old_sequence require a publication printable on the segment */
      vnp = sfp->cit;

      if (vnp != NULL && asp->referenceArray != NULL) {
        for (ppr = vnp->data.ptrvalue; ppr != NULL; ppr = ppr->next) {
          j = MatchRef (ppr, asp->referenceArray, asp->numReferences);
          if (j > 0) {
            okay = TRUE;
            break;
          }
        }
      }
      if (! okay) {
        /* compare qualifier can now substitute for citation qualifier */
        gbq = sfp->qual;
        while (gbq != NULL) {
          if (StringICmp (gbq->qual, "compare") == 0 && (! StringHasNoText (gbq->val))) {
            okay = TRUE;
            break;
          }
          gbq = gbq->next;
        }
      }
      break;

    case FEATDEF_GENE:
      /* gene requires /gene or /locus_tag, but desc or syn can be mapped to /gene */
      grp = (GeneRefPtr) sfp->data.value.ptrvalue;
      if (grp != NULL) {
        if (! StringHasNoText (grp->locus)) {
          okay = TRUE;
        }  else if (! StringHasNoText (grp->locus_tag)) {
          okay = TRUE;
        } else if (! StringHasNoText (grp->desc)) {
          okay = TRUE;
        } else {
          vnp = grp->syn;
          if (vnp != NULL) {
            if (! StringHasNoText (vnp->data.ptrvalue)) {
              okay = TRUE;
            }
          }
        }
      }
      break;

    case FEATDEF_protein_bind:
    case FEATDEF_misc_binding:
      /* protein_bind or misc_binding require FTQUAL_bound_moiety */
      gbq = sfp->qual;
      while (gbq != NULL) {
        if (StringICmp (gbq->qual, "bound_moiety") == 0 && (! StringHasNoText (gbq->val))) {
          okay = TRUE;
          break;
        }
        gbq = gbq->next;
      }
      break;

    case FEATDEF_modified_base:
      /* modified_base requires FTQUAL_mod_base */
      gbq = sfp->qual;
      while (gbq != NULL) {
        if (StringICmp (gbq->qual, "mod_base") == 0 && (! StringHasNoText (gbq->val))) {
          okay = TRUE;
          break;
        }
        gbq = gbq->next;
      }
      break;

    case FEATDEF_gap:
      /* modified_base requires FTQUAL_estimated_length */
      gbq = sfp->qual;
      while (gbq != NULL) {
        if (StringICmp (gbq->qual, "estimated_length") == 0 && (! StringHasNoText (gbq->val))) {
          okay = TRUE;
          break;
        }
        gbq = gbq->next;
      }
      break;

    default:
      if (fcontext->featdeftype >= FEATDEF_GENE && fcontext->featdeftype < FEATDEF_MAX) {
        okay = TRUE;
      }
      break;
    }

    if (okay == FALSE) return TRUE;
  }

  /* if RELEASE_MODE, suppress features with location on segmented Bioseq */

  if (ajp->flags.suppressSegLoc) {
    bsp = awp->parent;
    if (bsp != NULL && bsp->repr == Seq_repr_seg) {
      slp = SeqLocFindNext (sfp->location, NULL);
      while (slp != NULL) {
        sip = SeqLocId (slp);
        if (sip != NULL) {
          if (SeqIdIn (sip, bsp->id)) return TRUE;
        }
        slp = SeqLocFindNext (sfp->location, slp);
      }
    }
  }

  awp->lastsfp = sfp;
  awp->lastsap = fcontext->sap;
  awp->lastleft = fcontext->left;
  awp->lastright = fcontext->right;

  if (fcontext->seqfeattype == SEQFEAT_CDREGION) {
    fbp = (FeatBlockPtr) Asn2gbAddBlock (awp, FEATURE_BLOCK, sizeof (IntCdsBlock));
  } else {
    fbp = (FeatBlockPtr) Asn2gbAddBlock (awp, FEATURE_BLOCK, sizeof (IntFeatBlock));
  }
  if (fbp == NULL) return TRUE;

  fbp->entityID = fcontext->entityID;
  fbp->itemID = fcontext->itemID;
  fbp->itemtype = OBJ_SEQFEAT;
  fbp->featdeftype = fcontext->featdeftype;
  ifp = (IntFeatBlockPtr) fbp;
  ifp->mapToNuc = FALSE;
  ifp->mapToProt = FALSE;
  ifp->mapToGen = FALSE;
  ifp->mapToMrna = FALSE;
  ifp->mapToPep = FALSE;
  ifp->firstfeat = awp->firstfeat;
  awp->firstfeat = FALSE;
  awp->featseen = TRUE;


  if (awp->afp != NULL) {
    DoImmediateFormat (awp->afp, (BaseBlockPtr) fbp);
  }

  /* optionally map CDS from cDNA onto genomic */

  if (awp->isGPS && ISA_na (bsp->mol) && awp->copyGpsCdsUp &&
      fcontext->featdeftype == FEATDEF_mRNA) {
    sip = SeqLocIdForProduct (sfp->product);
    bsp = BioseqFind (sip);
    if (bsp != NULL && ISA_na (bsp->mol)) {
      cds = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_CDREGION, 0, &cdscontext);
      if (cds != NULL) {
        fbp = (FeatBlockPtr) Asn2gbAddBlock (awp, FEATURE_BLOCK, sizeof (IntCdsBlock));
        if (fbp != NULL) {

          fbp->entityID = cdscontext.entityID;
          fbp->itemID = cdscontext.itemID;
          fbp->itemtype = OBJ_SEQFEAT;
          fbp->featdeftype = cdscontext.featdeftype;
          ifp = (IntFeatBlockPtr) fbp;
          ifp->mapToNuc = FALSE;
          ifp->mapToProt = FALSE;
          ifp->mapToGen = TRUE;
          ifp->mapToMrna = FALSE;
          ifp->mapToPep = FALSE;
          ifp->firstfeat = awp->firstfeat;
          awp->firstfeat = FALSE;

          if (awp->afp != NULL) {
            DoImmediateFormat (awp->afp, (BaseBlockPtr) fbp);
          }
        }
      }
    }
  }

  if (fcontext->seqfeattype != SEQFEAT_CDREGION) return TRUE;

  /* if feature table format, do not get features from protein product */

  if (awp->format == FTABLE_FMT) return TRUE;

  /* if CDS, collect more information from product protein bioseq - may be part */

  sip = SeqLocIdForProduct (sfp->product);
  bsp = BioseqFind (sip);
  if (bsp == NULL || (! ISA_aa (bsp->mol))) return TRUE;

  ifp->isCDS = TRUE;
  icp = (IntCdsBlockPtr) ifp;

  /* first explore pubs to pick up figure and maploc - no longer shown */

  /*
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_pub, &dcontext);
  while (sdp != NULL) {
    pdp = (PubdescPtr) sdp->data.ptrvalue;
    if (pdp != NULL) {
      if (icp->fig == NULL) {
        icp->fig = StringSaveNoNull (pdp->fig);
      }
      if (icp->maploc == NULL) {
        icp->maploc = StringSaveNoNull (pdp->maploc);
      }
    }
    sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_pub, &dcontext);
  }
  */

  /* product may be segmented part, and remaining features are indexed on parent */

  parent = SeqMgrGetParentOfPart (bsp, NULL);
  if (parent != NULL) {
    bsp = parent;
  }

  /* then explore mat_peptides, sites, etc. */

  GetFeatsOnCdsProduct (sfp, asp->bsp, bsp, ajp, awp);

  return TRUE;
}

static Boolean LIBCALLBACK GetFeatsOnSeg (
  SeqLocPtr slp,
  SeqMgrSegmentContextPtr context
)

{
  IntAsn2gbJobPtr  ajp;
  Asn2gbWorkPtr    awp;
  BioseqPtr        bsp;
  Uint2            entityID;
  Int4             from;
  Int4             left;
  SeqLocPtr        loc;
  Int4             right;
  SeqIdPtr         sip;
  Int4             to;

  if (slp == NULL || context == NULL) return FALSE;
  awp = (Asn2gbWorkPtr) context->userdata;
  if (awp == NULL) return FALSE;
  ajp = awp->ajp;
  if (ajp == NULL) return FALSE;

  /* do not fetch outside of desired component */

  if (ajp->ajp.slp != NULL) {
    left = GetOffsetInBioseq (ajp->ajp.slp, awp->parent, SEQLOC_LEFT_END);
    right = GetOffsetInBioseq (ajp->ajp.slp, awp->parent, SEQLOC_RIGHT_END);

    from = context->cumOffset;
    to = from + context->to - context->from;

    if (left > to) return TRUE;
    if (right < from) return TRUE;
  }
 
  from = awp->from;
  to = awp->to;

  sip = SeqLocId (slp);
  if (sip == NULL) {
    loc = SeqLocFindNext (slp, NULL);
    if (loc != NULL) {
      sip = SeqLocId (loc);
    }
  }
  if (sip == NULL) return TRUE;

  /* may want to remote fetch genome component if not already in memory */

  bsp = BioseqLockById (sip);

  if (bsp == NULL) return TRUE;

  entityID = ObjMgrGetEntityIDForPointer (bsp);

  if (entityID != awp->entityID) {

    /* if segment not packaged in record, may need to feature index it */

    if (SeqMgrFeaturesAreIndexed (entityID) == 0) {
      SeqMgrIndexFeatures (entityID, NULL);
    }

    /* collect features indexed on the remote bioseq */

    awp->from = 0;
    awp->to = bsp->length - 1;
  }

  awp->lastsfp = NULL;
  awp->lastsap = NULL;
  awp->lastleft = 0;
  awp->lastright = 0;

  if (context->strand == Seq_strand_minus) {
    SeqMgrExploreFeaturesRev (bsp, (Pointer) awp, GetFeatsOnBioseq, /* awp->slp */ slp, NULL, NULL);
  } else {
    SeqMgrExploreFeatures (bsp, (Pointer) awp, GetFeatsOnBioseq, /* awp->slp */ slp, NULL, NULL);
  }

  /* restore original from and to */

  awp->from = from;
  awp->to = to;

  BioseqUnlock (bsp);

  return TRUE;
}

NLM_EXTERN void AddFeatureBlock (
  Asn2gbWorkPtr awp
)

{
  IntAsn2gbJobPtr    ajp;
  BioseqPtr          bsp;
  SeqFeatPtr         cds;
  SeqMgrDescContext  dcontext;
  SeqMgrFeatContext  fcontext;
  FeatBlockPtr       fbp;
  SeqFeatPtr         gene;
  IntFeatBlockPtr    ifp;
  Boolean            is_other;
  MolInfoPtr         mip;
  SeqFeatPtr         mrna;
  SeqFeatPtr         prot;
  SeqDescrPtr        sdp;
  SeqIdPtr           sip;
  SeqLocPtr          slp;

  if (awp == NULL) return;
  ajp = awp->ajp;
  if (ajp == NULL) return;
  bsp = awp->parent;
  if (bsp == NULL) return;

  awp->lastsfp = NULL;
  awp->lastsap = NULL;
  awp->lastleft = 0;
  awp->lastright = 0;

  /* optionally map gene from genomic onto cDNA */

  if (awp->isGPS && ISA_na (bsp->mol) && awp->copyGpsGeneDown) {
    sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &dcontext);
    if (sdp != NULL && sdp->choice == Seq_descr_molinfo) {
      mip = (MolInfoPtr) sdp->data.ptrvalue;
      if (mip != NULL) {
        if (mip->biomol == MOLECULE_TYPE_MRNA) {
          mrna = SeqMgrGetRNAgivenProduct (bsp, NULL);
          if (mrna != NULL) {
            gene = SeqMgrGetOverlappingGene (mrna->location, &fcontext);
            if (gene != NULL && gene->data.choice == SEQFEAT_GENE) {

              fbp = (FeatBlockPtr) Asn2gbAddBlock (awp, FEATURE_BLOCK, sizeof (IntCdsBlock));
              if (fbp != NULL) {

                fbp->entityID = fcontext.entityID;
                fbp->itemID = fcontext.itemID;
                fbp->itemtype = OBJ_SEQFEAT;
                fbp->featdeftype = fcontext.featdeftype;
                ifp = (IntFeatBlockPtr) fbp;
                ifp->mapToNuc = FALSE;
                ifp->mapToProt = FALSE;
                ifp->mapToGen = FALSE;
                ifp->mapToMrna = TRUE;
                ifp->mapToPep = FALSE;
                ifp->isCDS = TRUE;
                ifp->firstfeat = awp->firstfeat;
                awp->firstfeat = FALSE;

                if (awp->afp != NULL) {
                  DoImmediateFormat (awp->afp, (BaseBlockPtr) fbp);
                }
              }
            }
          }
        }
      }
    }
  }

  if (! awp->onlyNearFeats) {
    if (awp->farFeatsSuppress) {

      if (bsp->repr == Seq_repr_seg || bsp->repr == Seq_repr_delta) {

        /* if farFeatsSuppress first collect features on remote segments in MASTER_STYLE */

        SeqMgrExploreSegments (bsp, (Pointer) awp, GetFeatsOnSeg);
      }
    }
  }

  if ((! awp->farFeatsSuppress) || (! awp->featseen)) {

    /* reminder - features on near parts are indexed on segmented Bioseq */

    slp = ajp->ajp.slp;
    if (slp != NULL && SeqLocStrand (slp) == Seq_strand_minus) {
      SeqMgrExploreFeaturesRev (bsp, (Pointer) awp, GetFeatsOnBioseq, awp->slp, NULL, NULL);
    } else {
      SeqMgrExploreFeatures (bsp, (Pointer) awp, GetFeatsOnBioseq, awp->slp, NULL, NULL);
    }
  }


  if (awp->format == GENPEPT_FMT && ISA_aa (bsp->mol)) {
    cds = SeqMgrGetCDSgivenProduct (bsp, &fcontext);
    if (cds != NULL && cds->data.choice == SEQFEAT_CDREGION) {
      /* if protein bioseq and cds feature but no nucleotide, cannot index cds, so skip */
      if (fcontext.entityID > 0 && fcontext.itemID > 0) {

        fbp = (FeatBlockPtr) Asn2gbAddBlock (awp, FEATURE_BLOCK, sizeof (IntCdsBlock));
        if (fbp != NULL) {

          fbp->entityID = fcontext.entityID;
          fbp->itemID = fcontext.itemID;
          fbp->itemtype = OBJ_SEQFEAT;
          fbp->featdeftype = fcontext.featdeftype;
          ifp = (IntFeatBlockPtr) fbp;
          ifp->mapToNuc = FALSE;
          ifp->mapToProt = TRUE;
          ifp->mapToGen = FALSE;
          ifp->mapToMrna = FALSE;
          ifp->mapToPep = FALSE;
          ifp->isCDS = TRUE;
          ifp->firstfeat = awp->firstfeat;
          awp->firstfeat = FALSE;

          if (awp->afp != NULL) {
            DoImmediateFormat (awp->afp, (BaseBlockPtr) fbp);
          }
        }
      }
    }
    prot = SeqMgrGetPROTgivenProduct (bsp, &fcontext);
    if (prot != NULL && prot->data.choice == SEQFEAT_PROT) {

      is_other = FALSE;
      for (sip = bsp->id; sip != NULL; sip = sip->next) {
        if (sip->choice == SEQID_OTHER) {
          is_other = TRUE;
        }
      }

      /* for RefSeq records or GenBank not release_mode */
      if (is_other || (! ajp->flags.forGbRelease)) {

        fbp = (FeatBlockPtr) Asn2gbAddBlock (awp, FEATURE_BLOCK, sizeof (IntCdsBlock));
        if (fbp != NULL) {

          fbp->entityID = fcontext.entityID;
          fbp->itemID = fcontext.itemID;
          fbp->itemtype = OBJ_SEQFEAT;
          fbp->featdeftype = fcontext.featdeftype;
          ifp = (IntFeatBlockPtr) fbp;
          ifp->mapToNuc = FALSE;
          ifp->mapToProt = FALSE;
          ifp->mapToGen = FALSE;
          ifp->mapToMrna = FALSE;
          ifp->mapToPep = TRUE;
          ifp->firstfeat = awp->firstfeat;
          awp->firstfeat = FALSE;

          if (awp->afp != NULL) {
            DoImmediateFormat (awp->afp, (BaseBlockPtr) fbp);
          }
        }
      }
    }
  }

  if (awp->onlyNearFeats) return;

  if (awp->nearFeatsSuppress && awp->featseen) return;

  if (! awp->farFeatsSuppress) {

    if (bsp->repr == Seq_repr_seg || bsp->repr == Seq_repr_delta) {

      /* if not farFeatsSuppress now collect features on remote segments in MASTER_STYLE */

      SeqMgrExploreSegments (bsp, (Pointer) awp, GetFeatsOnSeg);
    }
  }
}

