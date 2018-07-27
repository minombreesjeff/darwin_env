/*   idcleanscan.c
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
* File Name:  idcleanscan.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   5/26/00
*
* $Revision: 6.4 $
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

/* scans binary ASN.1 Bioseq-set release files in preparation for ID cleanup */

#include <ncbi.h>
#include <objall.h>
#include <objsset.h>
#include <objsub.h>
#include <objfdef.h>
#include <seqport.h>
#include <sequtil.h>
#include <sqnutils.h>
#include <subutil.h>
#include <tofasta.h>
#include <gather.h>
#include <toasn3.h>
#include <explore.h>

typedef struct stringset {
  Char     firstID [41];
  Char     text [100];
  Int4     count;
} StringSet, PNTR StringSetPtr;

typedef struct scandata {
  FILE        *fp;
  AsnIoPtr    aop;
  AsnTypePtr  atp_se;
  Char        buf [41];
  Boolean     bulk;
  ValNodePtr  cgList;
  ValNodePtr  lcList;
  Int4        impCdsCount;
  Int4        recordCount;
} ScanData, PNTR ScanDataPtr;

static void PrintFeatureMessage (SeqFeatPtr sfp, ScanDataPtr sdp,
                                 CharPtr prefix, CharPtr suffix)

{
  BioseqPtr  bsp;
  Char       buf [41];

  if (sfp == NULL || sdp == NULL || prefix == NULL) return;

  bsp = BioseqFindFromSeqLoc (sfp->location);
  if (bsp != NULL) {
    SeqIdWrite (bsp->id, buf, PRINTID_FASTA_LONG, sizeof (buf));
    fprintf (sdp->fp, "%s - %s", prefix, buf);
  } else {
    fprintf (sdp->fp, "%s - %s", prefix, sdp->buf);
  }
  if (! StringHasNoText (suffix)) {
    fprintf (sdp->fp, " - %s", suffix);
  }
  fprintf (sdp->fp, "\n");
}

static void PrintGraphMessage (BioseqPtr bsp, ScanDataPtr sdp,
                               CharPtr prefix, CharPtr suffix)

{
  Char  buf [41];

  if (bsp == NULL || sdp == NULL || prefix == NULL) return;

  SeqIdWrite (bsp->id, buf, PRINTID_FASTA_LONG, sizeof (buf));
  fprintf (sdp->fp, "QA - %s - %s", buf, prefix);
  if (! StringHasNoText (suffix)) {
    fprintf (sdp->fp, " - %s", suffix);
  }
  fprintf (sdp->fp, "\n");
}

typedef struct gphgetdata {
  ValNodePtr  vnp;
  SeqLocPtr   slp;
  BioseqPtr   bsp;
} GphGetData, PNTR GphGetPtr;

typedef struct gphitem {
  SeqGraphPtr  sgp;
  Int4         left;
  Int4         right;
} GphItem, PNTR GphItemPtr;

static Boolean GetGraphsProc (GatherObjectPtr gop)

{
  GphGetPtr    ggp;
  GphItemPtr   gip;
  SeqGraphPtr  sgp;

  if (gop == NULL || gop->itemtype != OBJ_SEQGRAPH) return TRUE;
  ggp = (GphGetPtr) gop->userdata;
  sgp = (SeqGraphPtr) gop->dataptr;
  if (ggp == NULL || sgp == NULL) return TRUE;
  /* only phrap or gap4 currently allowed */
  if (StringICmp (sgp->title, "Phrap Quality") == 0 ||
      StringICmp (sgp->title, "Gap4") == 0) {
    /* data type must be bytes */
    if (sgp->flags[2] == 3) {
      if (SeqIdForSameBioseq (SeqLocId (sgp->loc), SeqLocId (ggp->slp))) {
        gip = (GphItemPtr) MemNew (sizeof (GphItem));
        if (gip == NULL) return TRUE;
        gip->sgp = sgp;
        gip->left = GetOffsetInBioseq (sgp->loc, ggp->bsp, SEQLOC_LEFT_END);
        gip->right = GetOffsetInBioseq (sgp->loc, ggp->bsp, SEQLOC_RIGHT_END);
        ValNodeAddPointer (&(ggp->vnp), 0, (Pointer) gip);
      }
    }
  }
  return TRUE;
}

static int LIBCALLBACK SortSeqGraphProc (VoidPtr ptr1, VoidPtr ptr2)

{
  GphItemPtr  gip1, gip2;
  ValNodePtr  vnp1, vnp2;

  if (ptr1 == NULL || ptr2 == NULL) return 0;
  vnp1 = *((ValNodePtr PNTR) ptr1);
  vnp2 = *((ValNodePtr PNTR) ptr2);
  if (vnp1 == NULL || vnp2 == NULL) return 0;
  gip1 = (GphItemPtr) vnp1->data.ptrvalue;
  gip2 = (GphItemPtr) vnp2->data.ptrvalue;
  if (gip1 == NULL || gip2 == NULL) return 0;
  if (gip1->left > gip2->left) {
    return 1;
  } else if (gip1->left < gip2->left) {
    return -1;
  } else if (gip1->right > gip2->right) {
    return -1;
  } else if (gip2->right < gip2->right) {
    return 1;
  }
  return 0;
}

static ValNodePtr GetSeqGraphsOnBioseq (Uint2 entityID, BioseqPtr bsp)

{
  GphGetData  ggd;
  Boolean     objMgrFilter [OBJ_MAX];
  ValNode     vn;

  ggd.vnp = NULL;
  vn.choice = SEQLOC_WHOLE;
  vn.data.ptrvalue = (Pointer) bsp->id;
  ggd.slp = &vn;
  ggd.bsp = bsp;
  MemSet ((Pointer) &objMgrFilter, 0, sizeof (objMgrFilter));
  objMgrFilter [OBJ_SEQGRAPH] = TRUE;
  GatherObjectsInEntity (entityID, 0, NULL, GetGraphsProc, (Pointer) &ggd, objMgrFilter);
  ggd.vnp = ValNodeSort (ggd.vnp, SortSeqGraphProc);
  return ggd.vnp;
}

static void DoGraphs (BioseqPtr bsp, Pointer userdata)

{
  Byte          bases [400];
  ByteStorePtr  bs;
  Int2          ctr, i, j, val;
  Int4          curroffset = 0, gphlen = 0, seqlen = 0, slplen,
                bslen, min = INT4_MAX, max = INT4_MIN,
                NsWithScore, ACGTsWithoutScore;
  DeltaSeqPtr   dsp;
  Uint2         entityID, olditemid = 0, olditemtype = 0,
                numdsp = 0, numsgp = 0;
  GphItemPtr    gip;
  ValNodePtr    head, vnp;
  Boolean       noErrors = TRUE;
  Uint1         residue;
  ScanDataPtr   sdp;
  SeqGraphPtr   sgp;
  SeqIntPtr     sintp;
  SeqLocPtr     slocp;
  SeqLitPtr     slp;
  SeqPortPtr    spp;
  Char          str [128];

  if (bsp == NULL) return;
  if (! ISA_na (bsp->mol)) return;
  if (SeqMgrGetParentOfPart (bsp, NULL) != NULL) return;

  sdp = (ScanDataPtr) userdata;

  entityID = ObjMgrGetEntityIDForPointer (bsp);
  head = GetSeqGraphsOnBioseq (entityID, bsp);
  if (head == NULL) return;

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    gip = (GphItemPtr) vnp->data.ptrvalue;
    if (gip == NULL) continue;
    sgp = gip->sgp;
    min = MIN ((Int4) min, (Int4) sgp->min.intvalue);
    max = MAX ((Int4) max, (Int4) sgp->max.intvalue);

    if (sgp->min.intvalue < 0 || sgp->min.intvalue > 100) {
      sprintf (str, "Graph min (%ld) out of range", (long) sgp->min.intvalue);
      PrintGraphMessage (bsp, sdp, str, NULL);
      noErrors = FALSE;
    }

    if (sgp->max.intvalue < 0 || sgp->max.intvalue > 100) {
      sprintf (str, "Graph max (%ld) out of range", (long) sgp->max.intvalue);
      PrintGraphMessage (bsp, sdp, str, NULL);
      noErrors = FALSE;
    }

    gphlen += sgp->numval;
    bs = (ByteStorePtr) sgp->values;
    if (bs != NULL) {
      bslen = BSLen (bs);
      if (sgp->numval != bslen) {
        sprintf (str, "SeqGraph (%ld) and ByteStore (%ld) length mismatch", (long) sgp->numval, (long) bslen);
        PrintGraphMessage (bsp, sdp, str, NULL);
        noErrors = FALSE;
      }
    }
  }

  if (bsp->repr == Seq_repr_raw) {
    seqlen = bsp->length;
  } else if (bsp->repr == Seq_repr_delta) {
    for (dsp = (DeltaSeqPtr) (bsp->seq_ext); dsp != NULL; dsp = dsp->next) {
      switch (dsp->choice) {
        case 1 :
          slocp = (SeqLocPtr) dsp->data.ptrvalue;
          if (slocp == NULL) break;
          if (slocp->choice != SEQLOC_NULL) {
            seqlen += SeqLocLen (slocp);
          }
          break;
        case 2 :
          slp = (SeqLitPtr) dsp->data.ptrvalue;
          if (slp == NULL || slp->seq_data == NULL) break;
          seqlen += slp->length;
          break;
        default :
          break;
      }
    }
  }

  if (seqlen != gphlen) {
    sprintf (str, "SeqGraph (%ld) and Bioseq (%ld) length mismatch", (long) gphlen, (long) seqlen);
    PrintGraphMessage (bsp, sdp, str, NULL);
    noErrors = FALSE;
  }

  if (bsp->repr == Seq_repr_delta) {
    for (dsp = (DeltaSeqPtr) (bsp->seq_ext), vnp = head;
         dsp != NULL && vnp != NULL; dsp = dsp->next) {
      gip = (GphItemPtr) vnp->data.ptrvalue;
      if (gip == NULL) continue;
      sgp = gip->sgp;
      if (sgp == NULL) continue;
      switch (dsp->choice) {
        case 1 :
          slocp = (SeqLocPtr) dsp->data.ptrvalue;
          if (slocp != NULL && slocp->choice != SEQLOC_NULL) {
            slplen = SeqLocLen (slocp);
            curroffset += slplen;
            if (sgp->numval != slplen) {
              sprintf (str, "SeqGraph (%ld) and SeqLoc (%ld) length mismatch", (long) sgp->numval, (long) slplen);
              PrintGraphMessage (bsp, sdp, str, NULL);
              noErrors = FALSE;
            }
            numdsp++;
            if (vnp != NULL) {
              vnp = vnp->next;
              numsgp++;
            }
          }
          break;
        case 2 :
          slp = (SeqLitPtr) dsp->data.ptrvalue;
          if (slp != NULL && slp->seq_data != NULL) {
            if (sgp->numval != slp->length) {
              sprintf (str, "SeqGraph (%ld) and SeqLit (%ld) length mismatch", (long) sgp->numval, (long) slp->length);
              PrintGraphMessage (bsp, sdp, str, NULL);
              noErrors = FALSE;
            }
            slocp = sgp->loc;
            if (slocp != NULL && slocp->choice == SEQLOC_INT) {
              sintp = (SeqIntPtr) slocp->data.ptrvalue;
              if (sintp != NULL) {
                if (sintp->from != curroffset) {
                  sprintf (str, "SeqGraph (%ld) and SeqLit (%ld) start do not coincide", (long) sintp->from, (long) curroffset);
                  PrintGraphMessage (bsp, sdp, str, NULL);
                  noErrors = FALSE;
                }
                if (sintp->to != slp->length + curroffset - 1) {
                  sprintf (str, "SeqGraph (%ld) and SeqLit (%ld) stop do not coincide", (long) sintp->to, (long) (slp->length + curroffset - 1));
                  PrintGraphMessage (bsp, sdp, str, NULL);
                  noErrors = FALSE;
                }
              }
            }
            numdsp++;
            if (vnp != NULL) {
              vnp = vnp->next;
              numsgp++;
            }
          }
          if (slp != NULL) {
            curroffset += slp->length;
          }
          break;
        default :
          break;
      }
    }
    if (numdsp != numsgp) {
      sprintf (str, "Different number of SeqGraph (%d) and SeqLit (%d) components", (int) numsgp, (int) numdsp);
      PrintGraphMessage (bsp, sdp, str, NULL);
      noErrors = FALSE;
    }
  }

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    gip = (GphItemPtr) vnp->data.ptrvalue;
    if (gip == NULL) continue;
    sgp = gip->sgp;
    if (sgp == NULL) continue;
    spp = SeqPortNewByLoc (sgp->loc, Seq_code_iupacna);
    if (spp == NULL) continue;
    slplen = SeqLocLen (sgp->loc);
    if (bsp->repr == Seq_repr_delta || bsp->repr == Seq_repr_virtual) {
      SeqPortSet_do_virtual (spp, TRUE);
    }

    bs = (ByteStorePtr) sgp->values;
    BSSeek (bs, 0, SEEK_SET);
    j = 0;
    val = 0;

    ctr = SeqPortRead (spp, bases, sizeof (bases));
    i = 0;
    residue = (Uint1) bases [i];

    NsWithScore = 0;
    ACGTsWithoutScore = 0;

    while (residue != SEQPORT_EOF && j < sgp->numval) {
      if (IS_residue (residue)) {
        val = (Int2) BSGetByte (bs);
        j++;
        switch (residue) {
          case 'A' :
          case 'C' :
          case 'G' :
          case 'T' :
            if (val == 0) {
              ACGTsWithoutScore++;
            }
            break;
          case 'N' :
            if (val > 0) {
              NsWithScore++;
            }
            break;
          default :
            break;
        }
      }
      i++;
      if (i >= ctr) {
        i = 0;
        ctr = SeqPortRead (spp, bases, sizeof (bases));
        if (ctr < 0) {
          bases [0] = -ctr;
        } else if (ctr < 1) {
          bases [0] = SEQPORT_EOF;
        }
      }
      residue = (Uint1) bases [i];
    }

    if (ACGTsWithoutScore > 0) {
      sprintf (str, "%ld ACGT bases have zero score value", (long) ACGTsWithoutScore);
      PrintGraphMessage (bsp, sdp, str, NULL);
      noErrors = FALSE;
    }
    if (NsWithScore > 0) {
      sprintf (str, "%ld N bases have positive score value", (long) NsWithScore);
      PrintGraphMessage (bsp, sdp, str, NULL);
      noErrors = FALSE;
    }

    SeqPortFree (spp);
  }

  if (noErrors) {
    PrintGraphMessage (bsp, sdp, "Quality scores okay", NULL);
  }

  ValNodeFreeData (head);
}

static void DoProteins (BioseqPtr bsp, Pointer userdata)

{
  Char               buf [6];
  SeqMgrFeatContext  fcontext;
  Boolean            firstIsSig = FALSE;
  Int4               left = 0, right = 0;
  ScanDataPtr        sdp;
  SeqFeatPtr         sfp, last = NULL;
  SeqInt             sint;
  SeqPortPtr         spp;
  ValNode            vn;

  if (bsp == NULL) return;
  if (! ISA_aa (bsp->mol)) return;

  sdp = (ScanDataPtr) userdata;

  sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &fcontext);
  while (sfp != NULL) {
    if (fcontext.featdeftype == FEATDEF_mat_peptide_aa ||
        fcontext.featdeftype == FEATDEF_sig_peptide_aa ||
        fcontext.featdeftype == FEATDEF_transit_peptide_aa) {
      if (last != NULL) {
        if (fcontext.left <= right) {
          if (firstIsSig && fcontext.left == right &&
              fcontext.featdeftype != FEATDEF_sig_peptide_aa) {

            buf [0] = '\0';

            if (right >= 4) {
              MemSet ((Pointer) &vn, 0, sizeof (ValNode));
              vn.choice = SEQLOC_INT;
              vn.data.ptrvalue = &sint;

              MemSet ((Pointer) &sint, 0, sizeof (SeqInt));
              sint.id = SeqLocId (sfp->location);

              sint.from = right - 3;
              sint.to = right;
              sint.strand = Seq_strand_plus;

              spp = SeqPortNewByLoc (&vn, Seq_code_ncbieaa);
              if (spp != NULL) {
                SeqPortRead (spp, (BytePtr) buf, 4);
                SeqPortFree (spp);
              }
              buf [4] = '\0';
            }

            PrintFeatureMessage (sfp, sdp, "SP", buf);
          } else {
            PrintFeatureMessage (sfp, sdp, "OV", NULL);
          }
        }
      } else {
        last = sfp;
        left = fcontext.left;
        right = fcontext.right;
        if (fcontext.featdeftype == FEATDEF_sig_peptide_aa) {
          firstIsSig = TRUE;
        }
      }
    }
    sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &fcontext);
  }
}

static void DoPeptide (SeqFeatPtr sfp, Pointer userdata)

{
  SeqFeatPtr   cds;
  CdRegionPtr  crp;
  SeqLocPtr    first = NULL, last = NULL, slp = NULL;
  Boolean      partial5, partial3;
  Int4         pos1, pos2, adjust = 0, mod1, mod2;
  ScanDataPtr  sdp;

  if (sfp == NULL) return;
  if (sfp->idx.subtype != FEATDEF_mat_peptide &&
      sfp->idx.subtype != FEATDEF_sig_peptide &&
      sfp->idx.subtype != FEATDEF_transit_peptide) return;

  cds = SeqMgrGetOverlappingCDS (sfp->location, NULL);
  if (cds == NULL) return;
  crp = (CdRegionPtr) cds->data.value.ptrvalue;
  if (crp == NULL) return;
  if (crp->frame == 2) {
    adjust = 1;
  } else if (crp->frame == 3) {
    adjust = 2;
  }

  while ((slp = SeqLocFindNext (sfp->location, slp)) != NULL) {
    last = slp;
    if (first == NULL) {
      first = slp;
    }
  }
  if (first == NULL || last == NULL) return;

  pos1 = GetOffsetInLoc (first, cds->location, SEQLOC_START) - adjust;
  pos2 = GetOffsetInLoc (last, cds->location, SEQLOC_STOP) - adjust;
  mod1 = pos1 % 3;
  mod2 = pos2 % 3;

  CheckSeqLocForPartial (sfp->location, &partial5, &partial3);
  if (partial5) {
    mod1 = 0;
  }
  if (partial3) {
    mod2 = 2;
  }

  if (mod1 == 0 && mod2 == 2) return;

  sdp = (ScanDataPtr) userdata;
  PrintFeatureMessage (sfp, sdp, "CB", NULL);
}

static void DoPseudoCDS (SeqFeatPtr sfp, Pointer userdata)

{
  GBQualPtr    gbq;
  SeqFeatPtr   gene;
  GeneRefPtr   grp;
  Boolean      pseudo = FALSE;
  ScanDataPtr  sdp;

  if (sfp->data.choice != SEQFEAT_CDREGION) return;

  if (sfp->pseudo) {
    pseudo = TRUE;
  }
  grp = SeqMgrGetGeneXref (sfp);
  if (grp == NULL) {
    gene = SeqMgrGetOverlappingGene (sfp->location, NULL);
    if (gene != NULL) {
      if (gene->pseudo) {
        pseudo = TRUE;
      }
    }
  }
  if (grp != NULL && grp->pseudo) {
    pseudo = TRUE;
  }

  if (! pseudo) return;

  sdp = (ScanDataPtr) userdata;

  if (sfp->product != NULL) {
    PrintFeatureMessage (sfp, sdp, "PC", NULL);
  }

  for (gbq = sfp->qual; gbq != NULL; gbq = gbq->next) {
    if (StringICmp (gbq->qual, "translation") == 0) {
      PrintFeatureMessage (sfp, sdp, "TR", NULL);
      return;
    }
  }
}

static void DoImpCDSandTrna (SeqFeatPtr sfp, Pointer userdata)

{
  Uint1           from = 0;
  ImpFeatPtr      ifp;
  RnaRefPtr       rrp;
  ScanDataPtr     sdp;
  SeqMapTablePtr  smtp;
  tRNAPtr         trp;

  if (sfp->data.choice == SEQFEAT_IMP) {
    ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
    if (ifp == NULL) return;
    if (StringICmp (ifp->key, "CDS") != 0) return;

    sdp = (ScanDataPtr) userdata;
    (sdp->impCdsCount)++;
    /*
    PrintFeatureMessage (sfp, sdp, "IC", NULL);
    */
  } else if (sfp->data.choice == SEQFEAT_RNA) {
    rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
    if (rrp != NULL && rrp->ext.choice == 2) {
      trp = (tRNAPtr) rrp->ext.value.ptrvalue;
      if (trp == NULL) return;
      if (trp->aatype == 2) return;
      switch (trp->aatype) {
        case 0 :
          from = 0;
          break;
        case 1 :
          from = Seq_code_iupacaa;
          break;
        case 2 :
          from = Seq_code_ncbieaa;
          break;
        case 3 :
          from = Seq_code_ncbi8aa;
          break;
        case 4 :
          from = Seq_code_ncbistdaa;
          break;
        default:
          break;
      }
      smtp = SeqMapTableFind (Seq_code_ncbieaa, from);
      if (smtp == NULL) {
        sdp = (ScanDataPtr) userdata;
        PrintFeatureMessage (sfp, sdp, "BT", NULL);
      }
    }
  }
}

static void RecordTitle (ScanDataPtr sdp, CharPtr str)

{
  StringSetPtr  ssp;
  ValNodePtr    vnp;

  if (sdp == NULL || StringHasNoText (str)) return;

  for (vnp = sdp->cgList; vnp != NULL; vnp = vnp->next) {
    ssp = (StringSetPtr) vnp->data.ptrvalue;
    if (ssp == NULL) continue;
    if (StringCmp (ssp->text, str) == 0) {
      (ssp->count)++;
      return;
    }
  }

  ssp = MemNew (sizeof (StringSet));
  if (ssp == NULL) return;
  StringCpy (ssp->firstID, sdp->buf);
  StringNCpy_0 (ssp->text, str, sizeof (ssp->text));
  ssp->count = 1;

  ValNodeAddPointer (&(sdp->cgList), 0, (Pointer) ssp);
}

static void DoTitle (SeqDescrPtr vnp, Pointer userdata)

{
  Char         ch;
  CharPtr      ptr, str, tmp;
  ScanDataPtr  sdp;

  if (vnp->choice != Seq_descr_title) return;
  str = (CharPtr) vnp->data.ptrvalue;
  if (StringHasNoText (str)) return;

  sdp = (ScanDataPtr) userdata;

  tmp = str;
  ptr = StringStr (tmp, "complete ");
  while (ptr != NULL) {
    tmp = ptr + 9;
    ch = *tmp;
    while (ch != '\0' && (! (IS_WHITESP (ch)))) {
      tmp++;
      ch = *tmp;
    }
    if (ch == '\0') return;
    if (StringNICmp (tmp, " genome", 7) == 0) {
      tmp [7] = '\0';
      RecordTitle (sdp, ptr);
      return;
    } else if (StringNICmp (tmp, " DNA", 4) == 0) {
      tmp [4] = '\0';
      RecordTitle (sdp, ptr);
      return;
    } else if (StringNICmp (tmp, " sequence", 9) == 0) {
      tmp [9] = '\0';
      RecordTitle (sdp, ptr);
      return;
    }
    ptr = StringStr (tmp, "complete ");
  }

  if (StringStr (str, "genome DNA") != NULL) {
    RecordTitle (sdp, "genome DNA");
    return;
  }

  if (sdp->bulk) return;

  if (StringStr (str, "genomic DNA") != NULL) {
    RecordTitle (sdp, "genomic DNA");
    return;
  }
}

static void LookForBulk (SeqDescrPtr vnp, Pointer userdata)

{
  BioSourcePtr  biop;
  MolInfoPtr    mip;
  OrgNamePtr    onp;
  OrgRefPtr     orp;
  ScanDataPtr   sdp;

  if (vnp->choice == Seq_descr_molinfo) {
    mip = (MolInfoPtr) vnp->data.ptrvalue;
    if (mip == NULL) return;
    if (mip->tech == MI_TECH_est ||
        mip->tech == MI_TECH_sts ||
        mip->tech == MI_TECH_survey ||
        mip->tech == MI_TECH_htgs_1 ||
        mip->tech == MI_TECH_htgs_2 ||
        mip->tech == MI_TECH_htgs_3 ||
        mip->tech == MI_TECH_htgs_0) {
      sdp = (ScanDataPtr) userdata;
      sdp->bulk = TRUE;
    }
  }

  if (vnp->choice == Seq_descr_source) {
    biop = (BioSourcePtr) vnp->data.ptrvalue;
    if (biop == NULL) return;
    orp = biop->org;
    if (orp == NULL) return;
    onp = orp->orgname;
    if (onp == NULL) return;
    if (StringICmp (onp->div, "SYN") == 0) {
      sdp = (ScanDataPtr) userdata;
      sdp->bulk = TRUE;
    }
  }
}

static void RecordThesis (ScanDataPtr sdp, CharPtr str)

{
  StringSetPtr  ssp;
  ValNodePtr    vnp;

  if (sdp == NULL || StringHasNoText (str)) return;

  for (vnp = sdp->lcList; vnp != NULL; vnp = vnp->next) {
    ssp = (StringSetPtr) vnp->data.ptrvalue;
    if (ssp == NULL) continue;
    if (StringCmp (ssp->text, str) == 0) {
      (ssp->count)++;
      return;
    }
  }

  ssp = MemNew (sizeof (StringSet));
  if (ssp == NULL) return;
  StringCpy (ssp->firstID, sdp->buf);
  StringNCpy_0 (ssp->text, str, sizeof (ssp->text));
  ssp->count = 1;

  ValNodeAddPointer (&(sdp->lcList), 0, (Pointer) ssp);
}

static void DoThesis (PubdescPtr pdp, Pointer userdata)

{
  CitBookPtr   cbp;
  Char         ch;
  ScanDataPtr  sdp;
  CharPtr      title, tmp;
  ValNodePtr   ttl, vnp;

  if (pdp == NULL) return;

  for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == PUB_Man) {
      cbp = (CitBookPtr) vnp->data.ptrvalue;
      if (cbp != NULL) {
        ttl = cbp->title;
        if (ttl != NULL) {
          title = (CharPtr) ttl->data.ptrvalue;
          if (! StringHasNoText (title)) {
            if (StringLen (title) > 3) {
              ch = *title;
              if (IS_LOWER (ch)) {
                tmp = title;
                ch = *tmp;
                while (ch != '\0' && (! (IS_WHITESP (ch)))) {
                  tmp++;
                  ch = *tmp;
                }
                *tmp = '\0';
                sdp = (ScanDataPtr) userdata;
                RecordThesis (sdp, title);
              }
            }
          }
        }
      }
    }
  }
}

static void DoUser (SeqFeatPtr sfp, Pointer userdata)

{
  BoolPtr  hasUserP;

  hasUserP = (BoolPtr) userdata;
  if (sfp->ext != NULL) {
    *hasUserP = TRUE;
  }
}

static void DoRecord (SeqEntryPtr sep, Pointer userdata)

{
  BioseqPtr     bsp;
  SeqEntryPtr   nsep;
  ScanDataPtr   sdp;

  sdp = (ScanDataPtr) userdata;
  (sdp->recordCount)++;

  nsep = FindNthBioseq (sep, 1);
  if (nsep != NULL && IS_Bioseq (nsep)) {
    bsp = (BioseqPtr) nsep->data.ptrvalue;
    if (bsp != NULL) {
      SeqIdWrite (bsp->id, sdp->buf, PRINTID_FASTA_LONG, sizeof (sdp->buf));
    }
  }
#ifdef OS_UNIX
  /* printf ("%s\n", sdp->buf); */
#endif

  VisitPubdescsInSep (sep, (Pointer) sdp, DoThesis);

  /* check for 'genomic DNA' in DoTitle suppressed for bulk submissions */

  sdp->bulk = FALSE;
  VisitDescriptorsInSep (sep, (Pointer) sdp, LookForBulk);
  VisitDescriptorsInSep (sep, (Pointer) sdp, DoTitle);

  VisitFeaturesInSep (sep, (Pointer) sdp, DoImpCDSandTrna);

  /* index for pseudo cds, impfeat peptides codon frame */

  SeqMgrIndexFeatures (0, sep->data.ptrvalue);

  VisitFeaturesInSep (sep, (Pointer) sdp, DoPseudoCDS);

  VisitFeaturesInSep (sep, (Pointer) sdp, DoPeptide);

  /* now cleanup, index for overlapping peptides */

  SeriousSeqEntryCleanup (sep, NULL, NULL);
  SeqMgrIndexFeatures (0, sep->data.ptrvalue);

  VisitBioseqsInSep (sep, (Pointer) sdp, DoProteins);

  /*
  VisitBioseqsInSep (sep, (Pointer) sdp, DoGraphs);
  */

#if 0
  {
    Boolean  hasUser = FALSE;

    VisitFeaturesInSep (sep, (Pointer) &hasUser, DoUser);

    if (hasUser && sdp->aop != NULL && sdp->atp_se != NULL) {
      SeqEntryAsnWrite (sep, sdp->aop, sdp->atp_se);
    }
  }
#endif
}

static void DoReleaseFile (CharPtr inputFile, Boolean binary, Boolean compressed, FILE *fp, AsnIoPtr aop, AsnTypePtr atp_se)

{
  ScanData      sd;
  StringSetPtr  ssp;
  ValNodePtr    vnp;

  fprintf (fp, "\n***** %s *****\n\n", inputFile);

  sd.fp = fp;
  sd.aop = aop;
  sd.atp_se = atp_se;
  StringCpy (sd.buf, "?");
  sd.bulk = FALSE;
  sd.cgList = NULL;
  sd.lcList = NULL;
  sd.impCdsCount = 0;
  sd.recordCount = 0;

  ScanBioseqSetRelease (inputFile, binary, compressed, (Pointer) &sd, DoRecord);

  for (vnp = sd.cgList; vnp != NULL; vnp = vnp->next) {
    ssp = (StringSetPtr) vnp->data.ptrvalue;
    if (ssp == NULL) continue;
    fprintf (sd.fp, "CG - %s (%ld) - %s\n", ssp->firstID, (long) ssp->count, ssp->text);
  }

  for (vnp = sd.lcList; vnp != NULL; vnp = vnp->next) {
    ssp = (StringSetPtr) vnp->data.ptrvalue;
    if (ssp == NULL) continue;
    fprintf (sd.fp, "LC - %s (%ld) - %s\n", ssp->firstID, (long) ssp->count, ssp->text);
  }

  if (sd.impCdsCount != 0) {
    fprintf (sd.fp, "IC - (%ld)\n", (long) sd.impCdsCount);
  }

  sd.cgList = ValNodeFreeData (sd.cgList);
  sd.lcList = ValNodeFreeData (sd.lcList);

  fprintf (fp, "\n\n!!!!! %s - %ld records !!!!!\n\n", inputFile, (long) sd.recordCount);
}

#define p_argInputPath  0
#define i_argInputFile  1
#define o_argOutputFile 2
#define x_argFileSelect 3
#define b_argBinaryFile 4
#define c_argCompressed 5
#define s_argSubset     6

Args myargs [] = {
  {"Path to files", NULL, NULL, NULL,
    TRUE, 'p', ARG_STRING, 0.0, 0, NULL},
  {"Input File Name", NULL, NULL, NULL,
    TRUE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
  {"Output File Name", "stdout", NULL, NULL,
    FALSE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},
  {"File selection substring", ".bna.", NULL, NULL,
    TRUE, 'x', ARG_STRING, 0.0, 0, NULL},
  {"Binary file", "T", NULL, NULL,
    TRUE, 'b', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Compressed file", "F", NULL, NULL,
    TRUE, 'c', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Subset File Name", NULL, NULL, NULL,
    TRUE, 's', ARG_FILE_OUT, 0.0, 0, NULL},
};

Int2 Main (void)

{
  AsnIoPtr      aop = NULL;
  AsnModulePtr  amp;
  AsnTypePtr    atp_bss, atp_ss, atp_se;
  BioseqSet     bss;
  FILE          *fp;
  ValNodePtr    head, vnp;
  Char          path [PATH_MAX];
  CharPtr       progname, str, subfile;

  ErrSetFatalLevel (SEV_MAX);
  ErrClearOptFlags (EO_SHOW_USERSTR);
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
  if (! SeqCodeSetLoad ()) {
    Message (MSG_FATAL, "SeqCodeSetLoad failed");
    return 1;
  }
  if (! GeneticCodeTableLoad ()) {
    Message (MSG_FATAL, "GeneticCodeTableLoad failed");
    return 1;
  }

  MemSet ((Pointer) &bss, 0, sizeof (BioseqSet));

  amp = AsnAllModPtr ();
  if (amp == NULL) {
    Message (MSG_FATAL, "Unable to load AsnAllModPtr");
    return 1;
  }

  atp_bss = AsnFind ("Bioseq-set");
  if (atp_bss == NULL) {
    Message (MSG_FATAL, "Unable to find ASN.1 type Bioseq-set");
    return 1;
  }

  atp_ss = AsnFind ("Bioseq-set.seq-set");
  if (atp_ss == NULL) {
    Message (MSG_FATAL, "Unable to find ASN.1 type Bioseq-set.seq-set");
    return 1;
  }


  atp_se = AsnFind ("Bioseq-set.seq-set.E");
  if (atp_se == NULL) {
    Message (MSG_FATAL, "Unable to find ASN.1 type Bioseq-set.seq-set.E");
    return 1;
  }

  ProgramPath (path, sizeof (path));
  progname = StringRChr (path, DIRDELIMCHR);
  if (progname != NULL) {
    progname++;
  } else {
    progname = "idcleanscan";
  }

  if (! GetArgs (progname, sizeof (myargs) / sizeof (Args), myargs)) {
    return 0;
  }

  fp = FileOpen (myargs [o_argOutputFile].strvalue, "a");
  if (fp == NULL) {
    Message (MSG_FATAL, "FileOpen failed");
    return 1;
  }

  if (StringHasNoText (myargs [p_argInputPath].strvalue)) {

    str = myargs [i_argInputFile].strvalue;
    if (! StringHasNoText (str)) {
      DoReleaseFile (str, myargs [b_argBinaryFile].intvalue, myargs [c_argCompressed].intvalue, fp, aop, NULL);
    }

  } else {

    head = DirCatalog (myargs [p_argInputPath].strvalue);

    if (! StringHasNoText (myargs [s_argSubset].strvalue)) {
      aop = AsnIoOpen (myargs [s_argSubset].strvalue, /* "wb" */ "w");
      AsnOpenStruct (aop, atp_bss, (Pointer) &bss);
      AsnOpenStruct (aop, atp_ss, (Pointer) bss.seq_set);
      /*
      av.intvalue = BioseqseqSet_class_genbank;
      AsnWrite (aop, atp_cls, &av);
      */
    }

    for (vnp = head; vnp != NULL; vnp = vnp->next) {
      if (vnp->choice == 0) {
        str = (CharPtr) vnp->data.ptrvalue;
        if (! StringHasNoText (str)) {
          subfile = myargs [x_argFileSelect].strvalue;
          if (StringHasNoText (subfile) || StringStr (str, subfile) != NULL) {
#ifdef OS_UNIX
            /* printf ("%s\n", str); */
#endif
            DoReleaseFile (str, myargs [b_argBinaryFile].intvalue, myargs [c_argCompressed].intvalue, fp, aop, atp_se);
          }
        }
      }
    }

    if (aop != NULL) {
      AsnCloseStruct (aop, atp_ss, (Pointer) bss.seq_set);
      AsnCloseStruct (aop, atp_bss, (Pointer) &bss);
      AsnIoClose (aop);
    }

    ValNodeFreeData (head);
  }

  FileClose (fp);

  return 0;
}

