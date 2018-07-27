/*   asn2gnb2.c
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
* File Name:  asn2gnb2.c
*
* Author:  Karl Sirotkin, Tom Madden, Tatiana Tatusov, Jonathan Kans,
*          Mati Shomrat
*
* Version Creation Date:   10/21/98
*
* $Revision: 1.36 $
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

static Boolean DeltaLitOnly (
  BioseqPtr bsp
)

{
  ValNodePtr  vnp;

  if (bsp == NULL || bsp->repr != Seq_repr_delta) return FALSE;
  for (vnp = (ValNodePtr)(bsp->seq_ext); vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == 1) return FALSE;
  }
  return TRUE;
}

static Boolean SegHasParts (
  BioseqPtr bsp
)

{
  BioseqSetPtr  bssp;
  SeqEntryPtr   sep;

  if (bsp == NULL || bsp->repr != Seq_repr_seg) return FALSE;
  sep = bsp->seqentry;
  if (sep == NULL) return FALSE;
  sep = sep->next;
  if (sep == NULL || (! IS_Bioseq_set (sep))) return FALSE;
  bssp = (BioseqSetPtr) sep->data.ptrvalue;
  if (bssp != NULL && bssp->_class == BioseqseqSet_class_parts) return TRUE;
  return FALSE;
}

/* add functions allocate specific blocks, populate with paragraph print info */

static CharPtr strd [4] = {
  "   ", "ss-", "ds-", "ms-"
};

static CharPtr gnbk_mol [14] = {
  "    ", "DNA ", "RNA ", "mRNA", "rRNA", "tRNA", "snRNA", "scRNA", " AA ", "DNA ", "DNA ", "RNA ", "snoRNA", "RNA "
};

/* EMBL_FMT in RELEASE_MODE or ENTREZ_MODE, otherwise use gnbk_mol */

static CharPtr embl_mol [14] = {
  "xxx", "DNA", "RNA", "RNA", "RNA", "RNA", "RNA", "RNA", "AA ", "DNA", "DNA", "RNA", "RNA", "RNA"
};

static CharPtr embl_divs [18] = {
  "FUN", "INV", "MAM", "ORG", "PHG", "PLN", "PRI", "PRO", "ROD"
  "SYN", "UNA", "VRL", "VRT", "PAT", "EST", "STS", "HUM", "HTC"
};

static Uint1 imolToMoltype [14] = {
  0, 1, 2, 5, 4, 3, 6, 7, 9, 1, 1, 2, 8, 2
};

static DatePtr GetBestDate (
  DatePtr a,
  DatePtr b
)

{
  Int2  status;

  if (a == NULL) return b;
  if (b == NULL) return a;

  status = DateMatch (a, b, FALSE);
  if (status == 1) return a;

  return b;
}

/*--------------------------------------------------------*/
/*                                                        */
/*  s_IsSeperatorNeeded()                                 */
/*                                                        */
/*--------------------------------------------------------*/

static Boolean s_IsSeperatorNeeded(CharPtr baseString, Int4 baseLength, Int2 suffixLength)
{
  Char lastChar;
  Char nextToLastChar;

  lastChar = baseString[baseLength - 1];
  nextToLastChar = baseString[baseLength - 2];

  /* This first check put here to emulate what may be a  */
  /* bug in the original code (in CheckLocusLength() )   */
  /* which adds an 'S' segment seperator only if it      */
  /* DOES make the string longer than the max.           */

  if (baseLength + suffixLength < 16)
    return FALSE;

  /* If the last character is not a digit */
  /* then don't use a seperator.          */

  if (!IS_DIGIT(lastChar))
    return FALSE;

  /* If the last two characters are a non-digit   */
  /* followed by a '0', then don't use seperator. */

  if ((lastChar == '0') && (!IS_DIGIT(nextToLastChar)))
    return FALSE;

  /* If we made it to here, use a seperator */

  return TRUE;
}

/*--------------------------------------------------------*/
/*                                                        */
/*  s_LocusAddSuffix() -                                  */
/*                                                        */
/*--------------------------------------------------------*/

static Boolean s_LocusAddSuffix (CharPtr locus, Asn2gbWorkPtr awp)
{
  size_t  buflen;
  Char    ch;
  Char    segCountStr[6];
  Int2    segCountStrLen;
  Char    segSuffix[5];

  buflen = StringLen (locus);

  /* If there's one or less segments, */
  /* no suffix is needed.             */

  if (awp->numsegs <= 1)
    return FALSE;

  /* If the basestring has one or less */
  /* characters, no suffix is needed.  */

  if (buflen <=1)
    return FALSE;

  /* Add the suffix */

  ch = locus[buflen-1];
  sprintf(segCountStr,"%d",awp->numsegs);
  segCountStrLen = StringLen(segCountStr);
  segSuffix[0] = '\0';

  if (s_IsSeperatorNeeded(locus,buflen,segCountStrLen) == TRUE)
    sprintf(segSuffix,"S%0*d",segCountStrLen,awp->seg);
  else
    sprintf(segSuffix,"%0*d",segCountStrLen,awp->seg);
  StringCat(locus,segSuffix);

  /* Return successfully */

  return TRUE;
}

/*--------------------------------------------------------*/
/*                                                        */
/*  s_LocusAdjustLength() -                               */
/*                                                        */
/*--------------------------------------------------------*/

static Boolean s_LocusAdjustLength(CharPtr locus, Int2 maxLength)
{
  Int2     trimCount;
  Int2     buflen;
  CharPtr  buftmp;

  buflen = StringLen (locus);
  if (buflen <= maxLength) return FALSE;

  buftmp = MemNew(maxLength + 1);

  /* If the sequence id is an NCBI locus of the */
  /* form HSU00001, then make sure that if      */
  /* there is trimming the HS gets trimmed off  */
  /* as a unit, never just the 'H'.             */

  trimCount = buflen - maxLength;
  if (trimCount == 1)
    if (IS_ALPHA(locus[0]) != 0 &&
        IS_ALPHA(locus[1]) != 0 &&
        IS_ALPHA(locus[2]) != 0 &&
        IS_DIGIT(locus[3]) != 0 &&
        IS_DIGIT(locus[4]) != 0 &&
        IS_DIGIT(locus[5]) != 0 &&
        IS_DIGIT(locus[6]) != 0 &&
        IS_DIGIT(locus[7]) != 0 &&
        locus[8] == 'S' &&
        locus[9] == '\0')
      trimCount++;

  /* Left truncate the sequence id */

  StringCpy(buftmp, &locus[trimCount]);
  StringCpy(locus, buftmp);

  MemFree(buftmp);
  return TRUE;
}

/*--------------------------------------------------------*/
/*                                                        */
/*  AddLocusBlock() -                                     */
/*                                                        */
/*--------------------------------------------------------*/

static DatePtr GetBestDateForBsp (
  BioseqPtr bsp
)

{
  DatePtr            best_date = NULL;
  SeqMgrDescContext  dcontext;
  DatePtr            dp;
  EMBLBlockPtr       ebp;
  GBBlockPtr         gbp;
  PdbBlockPtr        pdp;
  PdbRepPtr          prp;
  SeqDescrPtr        sdp;
  SPBlockPtr         spp;

  if (bsp == NULL) return NULL;

  dp = NULL;
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_update_date, &dcontext);
  if (sdp != NULL) {
    dp = (DatePtr) sdp->data.ptrvalue;
    best_date = GetBestDate (dp, best_date);
  }

  /* !!! temporarily also look at genbank block entry date !!! */

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_genbank, &dcontext);
  if (sdp != NULL) {
    gbp = (GBBlockPtr) sdp->data.ptrvalue;
    if (gbp != NULL) {
      dp = gbp->entry_date;
      best_date = GetBestDate (dp, best_date);
    }
  }

  /* more complicated code for dates from various objects goes here */

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_embl, &dcontext);
  if (sdp != NULL) {
    ebp = (EMBLBlockPtr) sdp->data.ptrvalue;
    if (ebp != NULL) {
      dp = ebp->creation_date;
      best_date = GetBestDate (dp, best_date);
      dp = ebp->update_date;
      best_date = GetBestDate (dp, best_date);
    }
  }

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_sp, &dcontext);
  if (sdp != NULL) {
    spp = (SPBlockPtr) sdp->data.ptrvalue;
    if (spp != NULL) {
      dp = spp->created;
      if (dp != NULL && dp->data [0] == 1) {
        best_date = GetBestDate (dp, best_date);
      }
      dp = spp->sequpd;
      if (dp != NULL && dp->data [0] == 1) {
        best_date = GetBestDate (dp, best_date);
      }
      dp = spp->annotupd;
      if (dp != NULL && dp->data [0] == 1) {
        best_date = GetBestDate (dp, best_date);
      }
    }
  }

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_pdb, &dcontext);
  if (sdp != NULL) {
    pdp = (PdbBlockPtr) sdp->data.ptrvalue;
    if (pdp != NULL) {
      dp = pdp->deposition;
      if (dp != NULL && dp->data [0] == 1) {
        best_date = GetBestDate (dp, best_date);
      }
      prp = pdp->replace;
      if (prp != NULL) {
        dp = prp->date;
        if (dp != NULL && dp->data[0] == 1) {
          best_date = GetBestDate (dp, best_date);
        }
      }
    }
  }

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_create_date, &dcontext);
  if (sdp != NULL) {
    dp = (DatePtr) sdp->data.ptrvalue;
    if (dp != NULL) {
      best_date = GetBestDate (dp, best_date);
    }
  }

  return best_date;
}

static Boolean LocusHasBadChars (
  CharPtr locus
)

{
  Char     ch;
  CharPtr  ptr;

  ptr = locus;
  ch = *ptr;
  while (ch != '\0') {
    if (! IS_ALPHANUM (ch)) {
      return TRUE;
    }
    ptr++;
    ch = *ptr;
  }
  return FALSE;
}

NLM_EXTERN void AddLocusBlock (
  Asn2gbWorkPtr awp
)

{
  size_t             acclen;
  IntAsn2gbJobPtr    ajp;
  Asn2gbSectPtr      asp;
  BaseBlockPtr       bbp;
  DatePtr            best_date = NULL;
  BioSourcePtr       biop;
  Int2               bmol = 0;
  BioseqPtr          bsp;
  SeqFeatPtr         cds;
  Char               date [40];
  SeqMgrDescContext  dcontext;
  Char               div [10];
  BioseqPtr          dna;
  DatePtr            dp;
  CharPtr            ebmol;
  EMBLBlockPtr       ebp;
  SeqMgrFeatContext  fcontext;
  Boolean            first = TRUE;
  GBBlockPtr         gbp;
  Char               gene [32];
  Boolean            genome_view;
  GBSeqPtr           gbseq;
  Int4               gi = 0;
  Char               gi_buf [16];
  SeqIdPtr           gpp = NULL;
  Char               id [41];
  Int2               imol = 0;
  IndxPtr            index;
  Int2               istrand;
  Boolean            is_aa;
  Boolean            is_nm = FALSE;
  Boolean            is_np = FALSE;
  Boolean            is_nz = FALSE;
  Boolean            is_env_sample = FALSE;
  Boolean            is_transgenic = FALSE;
  Char               len [32];
  Int4               length;
  size_t             loclen;
  Char               locus [41];
  MolInfoPtr         mip;
  Char               mol [30];
  BioseqPtr          nm = NULL;
  OrgNamePtr         onp;
  Uint1              origin;
  OrgRefPtr          orp;
  BioseqPtr          parent;
  CharPtr            prefix = NULL;
  SeqDescrPtr        sdp;
  SeqFeatPtr         sfp;
  SeqIdPtr           sip;
  SubSourcePtr       ssp;
  CharPtr            str;
  CharPtr            suffix = NULL;
  Uint1              tech;
  Uint1              topology;
  TextSeqIdPtr       tsip;
  Boolean            wgsmaster = FALSE;
  StringItemPtr      ffstring;

  if (awp == NULL) return;
  ajp = awp->ajp;
  if (ajp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;
  asp = awp->asp;
  if (asp == NULL) return;

  bbp = Asn2gbAddBlock (awp, LOCUS_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  ffstring = FFGetString(ajp);
  if ( ffstring == NULL ) return;

  mol [0] = '\0';
  len [0] = '\0';
  div [0] = '\0';
  date [0] = '\0';
  gene [0] = '\0';

  genome_view = FALSE;
  if (bsp->repr == Seq_repr_seg && (! SegHasParts (bsp))) {
    genome_view = TRUE;
  }
  if (bsp->repr == Seq_repr_delta && (! DeltaLitOnly (bsp))) {
    genome_view = TRUE;
  }

  /* locus id */

  sip = NULL;
  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_OTHER) {
      tsip = (TextSeqIdPtr) sip->data.ptrvalue;
      if (tsip != NULL) {
        if (StringNCmp (tsip->accession, "NM_", 3) == 0 ||
            StringNCmp (tsip->accession, "NR_", 3) == 0 ||
            StringNCmp (tsip->accession, "XM_", 3) == 0 ||
            StringNCmp (tsip->accession, "XR_", 3) == 0) {
          is_nm = TRUE;
          nm = bsp;
        } else if (StringNCmp (tsip->accession, "NP_", 3) == 0  ||
                   StringNCmp (tsip->accession, "XP_", 3) == 0) {
          is_np = TRUE;
        } else if (StringNCmp (tsip->accession, "NZ_", 3) == 0) {
          is_nz = TRUE;
        }
      }
      break;
    }
    if (sip->choice == SEQID_GENBANK ||
        sip->choice == SEQID_EMBL ||
        sip->choice == SEQID_DDBJ) break;
    if (sip->choice == SEQID_PIR ||
        sip->choice == SEQID_SWISSPROT ||
        sip->choice == SEQID_PRF ||
        sip->choice == SEQID_PDB) break;
    if (sip->choice == SEQID_TPG ||
        sip->choice == SEQID_TPE ||
        sip->choice == SEQID_TPD) break;
    if (sip->choice == SEQID_GPIPE) {
      gpp = sip;
    }
  }
  if (sip == NULL) {
    sip = gpp;
  }
  if (sip == NULL) {
    sip = SeqIdFindBest (bsp->id, SEQID_GENBANK);
  }

  if (genome_view) {
    SeqIdWrite (sip, locus, PRINTID_TEXTID_ACCESSION, sizeof (locus) - 1);
  } else {
    SeqIdWrite (sip, locus, PRINTID_TEXTID_LOCUS, sizeof (locus) - 1);
    if (LocusHasBadChars (locus)) {
      SeqIdWrite (sip, locus, PRINTID_TEXTID_ACCESSION, sizeof (locus) - 1);
    }
  }

  if (is_np) {
    sfp = SeqMgrGetCDSgivenProduct (bsp, &fcontext);
    if (sfp != NULL && fcontext.bsp != NULL) {
      nm = fcontext.bsp;
      for (sip = nm->id; sip != NULL; sip = sip->next) {
        if (sip->choice == SEQID_OTHER) {
          tsip = (TextSeqIdPtr) sip->data.ptrvalue;
          if (tsip != NULL) {
            if (StringNCmp (tsip->accession, "NM_", 3) == 0 ||
                StringNCmp (tsip->accession, "XM_", 3) == 0) {
              is_nm = TRUE;
            }
          }
        }
      }
      if (! is_nm) {
        nm = NULL;
      }
    }
  }
  if (nm != NULL) {
    /*
    sfp = SeqMgrGetNextFeature (nm, NULL, SEQFEAT_GENE, 0, &fcontext);
    if (sfp != NULL) {
      StringNCpy_0 (gene, fcontext.label, sizeof (gene));
      if (SeqMgrGetNextFeature (nm, sfp, SEQFEAT_GENE, 0, &fcontext) != NULL) {
        gene [0] = '\0';
      }
      if (StringLen (gene) > 15) {
        gene [0] = '\0';
      }
    }
    */
  }

  /* more complicated code to get parent locus, if segmented, goes here */

  if (awp->slp != NULL) {
    length = SeqLocLen (awp->slp);
  } else {
    length = bsp->length;
  }

  mip = NULL;
  tech = MI_TECH_standard;
  origin = 0;
  bmol = bsp->mol;
  if (bmol > Seq_mol_aa) {
    bmol = 0;
  }
  istrand = bsp->strand;
  if (istrand > 3) {
    istrand = 0;
  }

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &dcontext);
  if (sdp != NULL) {
    bbp->entityID = dcontext.entityID;
    bbp->itemID = dcontext.itemID;
    bbp->itemtype = OBJ_SEQDESC;

    mip = (MolInfoPtr) sdp->data.ptrvalue;
    if (mip != NULL) {
      if (mip->biomol <= MOLECULE_TYPE_TRANSCRIBED_RNA) {
        imol = (Int2) mip->biomol;
      }
      tech = mip->tech;

      if (tech == MI_TECH_wgs && bsp->repr == Seq_repr_virtual) {

        /* check for WGS master record */

        for (sip = bsp->id; sip != NULL; sip = sip->next) {
          switch (sip->choice) {
            case SEQID_GENBANK :
            case SEQID_EMBL :
            case SEQID_DDBJ :
              tsip = (TextSeqIdPtr) sip->data.ptrvalue;
              if (tsip != NULL && tsip->accession != NULL) {
                acclen = StringLen (tsip->accession);
                if (acclen == 12) {
                  if (StringCmp (tsip->accession + 6, "000000") == 0) {
                    wgsmaster = TRUE;
                  }
                } else if (acclen == 13) {
                  if (StringCmp (tsip->accession + 6, "0000000") == 0) {
                    wgsmaster = TRUE;
                  }
                }
              }
              break;
            case SEQID_OTHER :
              tsip = (TextSeqIdPtr) sip->data.ptrvalue;
              if (tsip != NULL && tsip->accession != NULL) {
                if (StringLen (tsip->accession) == 15) {
                  if (StringCmp (tsip->accession + 9, "000000") == 0) {
                    wgsmaster = TRUE;
                  }
                }
              }
              break;
            default :
              break;
          }
        }
      }
    }
  }

  /* check inst.mol if mol-type is not-set or genomic */

  if (imol <= MOLECULE_TYPE_GENOMIC) {
    if (bmol == Seq_mol_aa) {
      imol = MOLECULE_TYPE_PEPTIDE;
    } else if (bmol == Seq_mol_na) {
      imol = 0;
    } else if (bmol == Seq_mol_rna) {
      imol = 2;
    } else {
      imol = 1;
    }
  } else if (imol == MOLECULE_TYPE_OTHER_GENETIC_MATERIAL) {
    if (bmol == Seq_mol_rna) {
      imol = 2;
    }
  }

  /* if ds-DNA don't show ds */

  if (bmol == Seq_mol_dna && istrand == 2) {
    istrand = 0;
  }

  /* ss=any RNA don't show ss */

  if ((bmol > Seq_mol_rna ||
      (imol >= MOLECULE_TYPE_MRNA && imol <= MOLECULE_TYPE_PEPTIDE)) &&
      istrand == 1) {
    istrand = 0;
  }

  topology = bsp->topology;
  if (awp->slp != NULL) {
    topology = TOPOLOGY_LINEAR;
  }

  /* length, topology, and molecule type */

  if (awp->format == GENBANK_FMT) {

    if (awp->newLocusLine) {

      if (wgsmaster && (! is_nz)) {
        sprintf (len, "%ld rc", (long) length);
      } else {
        sprintf (len, "%ld bp", (long) length);
      }
      sprintf (mol, "%s%-4s", strd [istrand], gnbk_mol [imol]);

    } else {

      if (topology == TOPOLOGY_CIRCULAR) {
        sprintf (len, "%7ld bp", (long) length);
        sprintf (mol, "%s%-4s  circular", strd [istrand], gnbk_mol [imol]);
      } else {
        sprintf (len, "%7ld bp", (long) length);
        sprintf (mol, "%s%-4s          ", strd [istrand], gnbk_mol [imol]);
      }
    }

  } else if (awp->format == GENPEPT_FMT) {

    if (awp->newLocusLine) {
      sprintf (len, "%ld aa", (long) length);
    } else {
      sprintf (len, "%7ld aa", (long) length);
    }

  } else if (awp->format == EMBL_FMT) {

    if (imol < MOLECULE_TYPE_PEPTIDE) {
      if (ajp->flags.useEmblMolType) {
        ebmol = embl_mol [imol];
      } else {
        ebmol = gnbk_mol [imol];
      }

      if (topology == TOPOLOGY_CIRCULAR) {
        sprintf (mol, "circular %s", ebmol);
        sprintf (len, "%ld BP.", (long) length);
      } else {
        sprintf (mol, "%s", ebmol);
        sprintf (len, "%ld BP.", (long) length);
      }
    }
  }

  /* division */

  biop = NULL;
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_source, &dcontext);
  if (sdp != NULL) {
    biop = (BioSourcePtr) sdp->data.ptrvalue;
  } else {
    sfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_BIOSRC, 0, &fcontext);
    if (sfp != NULL) {
      biop = (BioSourcePtr) sfp->data.value.ptrvalue;
    } else if (ISA_aa (bsp->mol)) {

      /* if protein with no sources, get sources applicable to DNA location of CDS */

      cds = SeqMgrGetCDSgivenProduct (bsp, &fcontext);
      if (cds != NULL) {
        sfp = SeqMgrGetOverlappingSource (cds->location, &fcontext);
        if (sfp != NULL) {
          biop = (BioSourcePtr) sfp->data.value.ptrvalue;
        } else {
          dna = BioseqFindFromSeqLoc (cds->location);
          if (dna != NULL) {
            sdp = SeqMgrGetNextDescriptor (dna, NULL, Seq_descr_source, &dcontext);
            if (sdp != NULL) {
              biop = (BioSourcePtr) sdp->data.ptrvalue;
            }
          }
        }
      }
    }
  }
  if (biop != NULL) {
    origin = biop->origin;
    orp = biop->org;
    if (orp != NULL) {
      onp = orp->orgname;
      if (onp != NULL) {
        StringNCpy_0 (div, onp->div, sizeof (div));
      }
    }
    for (ssp = biop->subtype; ssp != NULL; ssp = ssp->next) {
      if (ssp->subtype == SUBSRC_transgenic) {
        is_transgenic = TRUE;
      } else if (ssp->subtype == SUBSRC_environmental_sample) {
        is_env_sample = TRUE;
      }
    }
  }

  switch (tech) {
    case MI_TECH_est :
      StringCpy (div, "EST");
      break;
    case MI_TECH_sts :
      StringCpy (div, "STS");
      break;
    case MI_TECH_survey :
      StringCpy (div, "GSS");
      break;
    case MI_TECH_htgs_0 :
    case MI_TECH_htgs_1 :
    case MI_TECH_htgs_2 :
      StringCpy (div, "HTG");
      break;
    case MI_TECH_htc :
      StringCpy (div, "HTC");
      break;
    default :
      break;
  }

  if (origin == ORG_MUT || origin == ORG_ARTIFICIAL || origin == ORG_SYNTHETIC || is_transgenic) {
    StringCpy (div, "SYN");
  } else if (is_env_sample) {
    if (tech == MI_TECH_unknown || tech == MI_TECH_standard || tech == MI_TECH_other) {
      StringCpy (div, "ENV");
    }
  }

  sip = SeqIdFindBest (bsp->id, SEQID_PATENT);
  if (sip != NULL && sip->choice == SEQID_PATENT) {
    StringCpy (div, "PAT");
  }

  /* more complicated code for division, if necessary, goes here */

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_genbank, &dcontext);
  while (sdp != NULL) {
    gbp = (GBBlockPtr) sdp->data.ptrvalue;
    if (gbp != NULL) {
      if (StringHasNoText (div) && gbp->div != NULL) {
        StringCpy (div, gbp->div);
      }
      else if (StringCmp(gbp->div, "PAT") == 0 ||
               StringCmp(gbp->div, "SYN") == 0 ) {
        StringCpy (div, gbp->div);
      }
    }
    sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_genbank, &dcontext);
  }

  if (awp->format == EMBL_FMT || awp->format == EMBLPEPT_FMT) {

    sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_embl, &dcontext);
    if (sdp != NULL) {
      ebp = (EMBLBlockPtr) sdp->data.ptrvalue;
      if (ebp != NULL) {
        if (ebp->div == 255) {
          if (mip == NULL) {
            StringCpy (div, "HUM");
          }
        } else if (ebp->div < 18)  {
          StringCpy (div, embl_divs [ebp->div]);
        }
      }
    }

    if (StringHasNoText (div)) {
      StringCpy (div, "UNA");
    }
  }

  /* empty division field if unable to find anything */

  if (StringHasNoText (div)) {
    StringCpy (div, "   ");
  }

  /* contig style (old genome_view flag) forces CON division */

  if (awp->contig) {
    StringCpy (div, "CON");
  }

  if (genome_view) {
    StringCpy (div, "CON");
  }

  /* date */

  best_date = GetBestDateForBsp (bsp);

  if (best_date == NULL) {

    /* if bsp is product of CDS or mRNA feature, get date from sfp->location bsp */

    sfp = NULL;
    if (ISA_na (bsp->mol)) {
      sfp = SeqMgrGetRNAgivenProduct (bsp, NULL);
    } else if (ISA_aa (bsp->mol)) {
      sfp = SeqMgrGetCDSgivenProduct (bsp, NULL);
    }
    if (sfp != NULL) {
      parent = BioseqFindFromSeqLoc (sfp->location);
      if (parent != NULL) {
        best_date = GetBestDateForBsp (parent);
      }
    }
  }

  /* convert best date */

  if (best_date != NULL) {
    DateToFF (date, best_date, FALSE);
  }
  if (StringHasNoText (date)) {
    StringCpy (date, "01-JAN-1900");
  }

  if (awp->format == GENBANK_FMT || awp->format == GENPEPT_FMT) {

    /* Create the proper locus name */

    parent = awp->parent;
    if (parent->repr == Seq_repr_seg) {

      if (! StringHasNoText (awp->basename)) {
        StringCpy (locus, awp->basename);
        s_LocusAddSuffix (locus, awp);
      }
    }

    /* Print the "LOCUS_NEW" line, if requested */

    if (awp->newLocusLine) {

      FFStartPrint (ffstring, awp->format, 0, 0, "LOCUS", 12, 0, 0, NULL, FALSE);
      parent = awp->parent;

      if (parent->repr == Seq_repr_seg)
        s_LocusAdjustLength (locus,16);

      if (is_nm && (! StringHasNoText (gene))) {
        FFAddOneString (ffstring, gene, FALSE, FALSE, TILDE_IGNORE);
      } else {
        FFAddOneString (ffstring, locus, FALSE, FALSE, TILDE_IGNORE);
      }
      FFAddNChar(ffstring, ' ', 43 - StringLen(len)- ffstring->curr->pos, FALSE);
      FFAddOneString (ffstring, len, FALSE, FALSE, TILDE_IGNORE);
      FFAddNChar(ffstring, ' ', 44 - ffstring->curr->pos, FALSE);
      FFAddOneString (ffstring, mol, FALSE, FALSE, TILDE_IGNORE);
      FFAddNChar(ffstring, ' ', 55 - ffstring->curr->pos, FALSE);
      if (topology == TOPOLOGY_CIRCULAR) {
        FFAddOneString (ffstring, "circular", FALSE, FALSE, TILDE_IGNORE);
      } else {
        FFAddOneString (ffstring, "linear  ", FALSE, FALSE, TILDE_IGNORE);
      }
      FFAddNChar(ffstring, ' ', 64 - ffstring->curr->pos, FALSE);
      FFAddOneString (ffstring, div, FALSE, FALSE, TILDE_IGNORE);
      FFAddNChar(ffstring, ' ', 68 - ffstring->curr->pos, FALSE);
      FFAddOneString (ffstring, date, FALSE, FALSE, TILDE_IGNORE);
    }

    /* Else print the "LOCUS" line */

    else {

      FFStartPrint (ffstring, awp->format, 0, 0, "LOCUS", 12, 0, 0, NULL, FALSE);

      if (parent->repr == Seq_repr_seg)
        s_LocusAdjustLength (locus,16);

      FFAddOneString (ffstring, locus, FALSE, FALSE, TILDE_IGNORE);
      FFAddNChar(ffstring, ' ', 32 - StringLen(len) - ffstring->curr->pos, FALSE);
      FFAddOneString (ffstring, len, FALSE, FALSE, TILDE_IGNORE);
      FFAddNChar(ffstring, ' ', 33 - ffstring->curr->pos, FALSE);
      FFAddOneString (ffstring, mol, FALSE, FALSE, TILDE_IGNORE);
      FFAddNChar(ffstring, ' ', 52 - ffstring->curr->pos, FALSE);
      FFAddOneString (ffstring, div, FALSE, FALSE, TILDE_IGNORE);
      FFAddNChar(ffstring, ' ', 62 - ffstring->curr->pos, FALSE);
      FFAddOneString (ffstring, date, FALSE, FALSE, TILDE_IGNORE);
    }

  } else if (awp->format == EMBL_FMT || awp->format == EMBLPEPT_FMT) {

    FFStartPrint (ffstring, awp->format, 0, 0, NULL, 0, 5, 0, "ID", FALSE);

    FFAddOneString (ffstring, locus, FALSE, FALSE, TILDE_IGNORE);
    loclen = StringLen(locus);
    if (14 - 5 - loclen > 0) {
      FFAddNChar(ffstring, ' ', 14 - 5 - loclen, FALSE);
    }
    if (awp->hup) {
      FFAddOneString (ffstring, " confidential; ", FALSE, FALSE, TILDE_IGNORE);
    } else {
      FFAddOneString (ffstring, " standard; ", FALSE, FALSE, TILDE_IGNORE);
    }
    FFAddOneString (ffstring, mol, FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString (ffstring, "; ", FALSE, FALSE, TILDE_IGNORE);

    /* conditional code to make div "UNA" goes here */

    FFAddOneString (ffstring, div, FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString (ffstring, "; ", FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString (ffstring, len, FALSE, FALSE, TILDE_IGNORE);
  }

  /* optionally populate indexes for NCBI internal database */

  if (ajp->index) {
    index = &asp->index;
  } else {
    index = NULL;
  }

  if (index != NULL) {
    Char  tmp [20];
    index->locus = StringSave (locus);
    index->div = StringSave (div);
    sprintf (tmp, "%ld", (long) length);
    index->base_cnt = StringSave (tmp);
  }

  /* optionally populate gbseq for XML-ized GenBank format */

  if (ajp->gbseq) {
    gbseq = &asp->gbseq;
  } else {
    gbseq = NULL;
  }

  if (gbseq != NULL) {
    gbseq->locus = StringSave (locus);
    gbseq->length = length;
    gbseq->division = StringSave (div);
    gbseq->strandedness = bsp->strand;
    gbseq->moltype = imolToMoltype [imol];
    gbseq->topology = topology;

    for (sip = bsp->id; sip != NULL; sip = sip->next) {
      SeqIdWrite (sip, id, PRINTID_FASTA_SHORT, sizeof (id));
      ValNodeCopyStr (&gbseq->other_seqids, 0, id);
    }

    date [0] = '\0';
    dp = NULL;
    sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_create_date, &dcontext);
    if (sdp != NULL) {
      dp = (DatePtr) sdp->data.ptrvalue;
    }
    if (dp != NULL) {
      DateToFF (date, dp, FALSE);
    }
    if (StringHasNoText (date)) {
      StringCpy (date, "01-JAN-1900");
    }
    gbseq->create_date = StringSave (date);

    date [0] = '\0';
    dp = NULL;
    sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_update_date, &dcontext);
    if (sdp != NULL) {
      dp = (DatePtr) sdp->data.ptrvalue;
    }
    if (dp != NULL) {
      DateToFF (date, dp, FALSE);
    }
    if (StringHasNoText (date)) {
      StringCpy (date, "01-JAN-1900");
    }
    gbseq->update_date = StringSave (date);
  }

  suffix = FFEndPrint(ajp, ffstring, awp->format, 12, 0, 5, 0, "ID");
  FFRecycleString(ajp, ffstring);

  if (awp->contig && (! awp->showconfeats) && awp->smartconfeats && GetWWW (ajp) &&
      (awp->format == GENBANK_FMT || awp->format == GENPEPT_FMT)) {
    is_aa = ISA_aa (bsp->mol);
    gi = 0;
    for (sip = bsp->id; sip != NULL; sip = sip->next) {
      if (sip->choice == SEQID_GI) {
        gi = (Int4) sip->data.intvalue;
      }
    }
    if (gi > 0) {
      ffstring = FFGetString(ajp);

      sprintf(gi_buf, "%ld", (long) gi);
      FFAddOneString(ffstring, "<a href=", FALSE, FALSE, TILDE_IGNORE);
      FFAddOneString(ffstring, link_feat, FALSE, FALSE, TILDE_IGNORE);
      FFAddOneString(ffstring, "val=", FALSE, FALSE, TILDE_IGNORE);
      FFAddOneString(ffstring, gi_buf, FALSE, FALSE, TILDE_IGNORE);
      if ( is_aa ) {
        FFAddOneString(ffstring, "&view=gpwithparts>", FALSE, FALSE, TILDE_IGNORE);
      } else {
        FFAddOneString(ffstring, "&view=gbwithparts>", FALSE, FALSE, TILDE_IGNORE);
      }
      if (bsp->length > 1000000) {
        FFAddOneString(ffstring, "Click here to see all features and the sequence of this contig record.", FALSE, FALSE, TILDE_IGNORE);
      } else {
        FFAddOneString(ffstring, "Click here to see the sequence of this contig record.", FALSE, FALSE, TILDE_IGNORE);
      }
      FFAddOneString(ffstring, "</a>", FALSE, FALSE, TILDE_IGNORE);

      prefix = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 5, 5, "??");

      FFRecycleString(ajp, ffstring);
    }
  }

  if (StringDoesHaveText (prefix)) {
    loclen = StringLen (prefix) + StringLen (suffix);
    str = (CharPtr) MemNew (loclen + 10);
    if (str != NULL) {
      StringCpy (str, prefix);
      StringCat (str, "\n\n");
      StringCat (str, suffix);
    }
    bbp->string = str;
  } else {
    bbp->string = suffix;
  }

  if (awp->afp != NULL) {
    DoImmediateFormat (awp->afp, bbp);
  }
}

NLM_EXTERN void AddDeflineBlock (
  Asn2gbWorkPtr awp
)

{
  IntAsn2gbJobPtr    ajp;
  Asn2gbSectPtr      asp;
  BaseBlockPtr       bbp;
  BioseqPtr          bsp;
  Char               buf[4096]; 
  /*CharPtr          buf;
  size_t             buflen = 4096;*/ 
  SeqMgrDescContext  dcontext;
  GBSeqPtr           gbseq;
  ItemInfo           ii;
  MolInfoPtr         mip;
  SeqDescrPtr        sdp;
  Uint1              tech;
  StringItemPtr      ffstring;

  if (awp == NULL) return;
  ajp = awp->ajp;
  if (ajp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;
  asp = awp->asp;
  if (asp == NULL) return;

  bbp = Asn2gbAddBlock (awp, DEFLINE_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  tech = 0;
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &dcontext);
  if (sdp != NULL) {
    mip = (MolInfoPtr) sdp->data.ptrvalue;
    if (mip != NULL) {
      tech = mip->tech;
    }
  }

  ffstring = FFGetString(ajp);
  if ( ffstring == NULL ) return;

  /*buf = MemNew (sizeof (Char) * (buflen + 1));*/
  MemSet ((Pointer) (&ii), 0, sizeof (ItemInfo));
  MemSet ((Pointer) buf, 0, sizeof (buf));

  /* create default defline */

  if ( CreateDefLine (&ii, bsp, buf, sizeof(buf), tech, NULL, NULL)) {
    bbp->entityID = ii.entityID;
    bbp->itemID = ii.itemID;
    bbp->itemtype = ii.itemtype;

    FFStartPrint (ffstring, awp->format, 0, 12, "DEFINITION", 12, 5, 5, "DE", TRUE);

    if (StringHasNoText (buf)) {
      FFAddOneChar (ffstring, '.', FALSE);
    } else {
      FFAddOneString (ffstring, buf, TRUE, TRUE, TILDE_IGNORE);
    }

    bbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 5, 5, "DE");
  }

  /* optionally populate gbseq for XML-ized GenBank format */

  if (ajp->gbseq) {
    gbseq = &asp->gbseq;
  } else {
    gbseq = NULL;
  }

  if (gbseq != NULL) {
    gbseq->definition = StringSave (buf);
  }

  FFRecycleString(ajp, ffstring);

  if (awp->afp != NULL) {
    DoImmediateFormat (awp->afp, bbp);
  }
}

static void FF_www_accession (
  IntAsn2gbJobPtr ajp,
  StringItemPtr ffstring,
  CharPtr cstring
)
{
  if (cstring == NULL || ffstring == NULL) return;

  if ( GetWWW(ajp) ) {
    FFAddTextToString(ffstring, "<a href=", link_seq, NULL, FALSE, FALSE, TILDE_IGNORE);
    FFAddTextToString(ffstring, "val=", cstring, ">", FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString(ffstring, cstring, FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString(ffstring, "</a>", FALSE, FALSE, TILDE_IGNORE);
  } else {
    FFAddOneString(ffstring, cstring, FALSE, FALSE, TILDE_IGNORE);
  }
  return;
}

/* Check if acc directly follows prev */
static Boolean IsSuccessor(CharPtr acc, CharPtr prev)
{
  CharPtr accp, prevp;
  Int4 acc_num, prev_num;

  if (acc == NULL  ||  prev == NULL) return FALSE;

  if (StringLen(acc) != StringLen(prev)) return FALSE;

  accp = acc;
  prevp = prev;
  while (accp != '\0'  &&  prevp != '\0') {
    if (*accp != *prevp) return FALSE;
    if (IS_DIGIT(*accp)) {
      acc_num = (Int4)atol(accp);
      prev_num = (Int4)atol(prevp);
      return (acc_num == prev_num + 1);
    }
    ++accp;
    ++prevp;
  }
  return FALSE;
}


static ValNodePtr GetSecondaryAccessions(ValNodePtr extra_access)
{
#define EXTRA_ACCESSION_CUTOFF 20
#define BIN_ACCESSION_CUTOFF   5

  Int4 extra_acc_num = 0;
  ValNodePtr  bins, bin, vnp, result = NULL, temp;
  CharPtr first, last, curr, prev = NULL;
  Char  range[40];

  extra_acc_num = ValNodeLen(extra_access);
  if (extra_acc_num < EXTRA_ACCESSION_CUTOFF) {
    for (vnp = extra_access; vnp != NULL; vnp = vnp->next) {
      ValNodeCopyStr(&result, 0, (CharPtr)vnp->data.ptrvalue);
    }
    return result;
  }

  /* sort the accessions into bins of successive accessions */
  bin = bins = NULL;
  for (vnp = extra_access; vnp != NULL; vnp = vnp->next) {
    curr = (CharPtr) vnp->data.ptrvalue;
    if (ValidateAccn (curr) != 0) {
      continue;
    }
    if (!IsSuccessor(curr, prev)) {
      bin = ValNodeAdd(&bins);
    }
    temp = (ValNodePtr)bin->data.ptrvalue;
    ValNodeAddStr(&temp, 0, curr);
    bin->data.ptrvalue = temp;

    prev = curr;
  }

  for (bin = bins; bin != NULL; bin = bin->next) {
    vnp = (ValNodePtr)bin->data.ptrvalue;
    if (ValNodeLen(vnp) > BIN_ACCESSION_CUTOFF) {
      first = last = NULL;
      for ( ; vnp != NULL; vnp = vnp->next) {
        last = (CharPtr)vnp->data.ptrvalue;
        if (first == NULL) {
          first = last;
        }
      }
      range[0] = '\0';
      StringCat(range, first);
      StringCat(range, "-");
      StringCat(range, last);
      ValNodeCopyStr(&result, 0, range);
    } else {
      for ( ; vnp != NULL; vnp = vnp->next) {
        ValNodeCopyStr(&result, 0, (CharPtr)vnp->data.ptrvalue);
      }
    }
    bin->data.ptrvalue = ValNodeFree((ValNodePtr)bin->data.ptrvalue);
  }

  bins = ValNodeFreeData(bins);
  return result;
}


/* !!! this definitely needs more work to support all classes, use proper SeqId !!! */

NLM_EXTERN void AddAccessionBlock (
  Asn2gbWorkPtr awp
)

{
  size_t             acclen;
  SeqIdPtr           accn = NULL;
  IntAsn2gbJobPtr    ajp;
  Asn2gbSectPtr      asp;
  BaseBlockPtr       bbp;
  BioseqPtr          bsp;
  Char               buf [41];
  SeqMgrDescContext  dcontext;
  EMBLBlockPtr       ebp;
  ValNodePtr         extra_access;
  CharPtr            flatloc;
  GBBlockPtr         gbp;
  SeqIdPtr           gi = NULL;
  GBSeqPtr           gbseq;
  SeqIdPtr           gnl = NULL;
  SeqIdPtr           gpp = NULL;
  IndxPtr            index;
  SeqIdPtr           lcl = NULL;
  size_t             len = 0;
  MolInfoPtr         mip;
  SeqDescrPtr        sdp;
  ValNodePtr         secondary_acc;
  CharPtr            separator = " ";
  SeqIdPtr           sip;
  TextSeqIdPtr       tsip;
  ValNodePtr         vnp;
  CharPtr            wgsaccn = NULL;
  CharPtr            xtra;
  StringItemPtr      ffstring;

  if (awp == NULL) return;
  ajp = awp->ajp;
  if (ajp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;
  asp = awp->asp;
  if (asp == NULL) return;
  
  ffstring = FFGetString(ajp);
  if ( ffstring == NULL ) return;

  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    switch (sip->choice) {
      case SEQID_GI :
        gi = sip;
        break;
      case SEQID_GENBANK :
      case SEQID_EMBL :
      case SEQID_DDBJ :
        accn = sip;
        tsip = (TextSeqIdPtr) sip->data.ptrvalue;
        if (tsip != NULL) {
          acclen = StringLen (tsip->accession);
          if (acclen == 12) {
            wgsaccn = tsip->accession;
            len = 12;
          } else if (acclen == 13) {
            wgsaccn = tsip->accession;
            len = 13;
          }
        }
        break;
      case SEQID_OTHER :
        accn = sip;
        tsip = (TextSeqIdPtr) sip->data.ptrvalue;
        if (tsip != NULL) {
          if (StringLen (tsip->accession) == 15) {
            wgsaccn = tsip->accession;
            len = 15;
          }
        }
        break;
      case SEQID_PIR :
      case SEQID_SWISSPROT :
      case SEQID_PRF :
      case SEQID_PDB :
        accn = sip;
        break;
      case SEQID_TPG :
      case SEQID_TPE :
      case SEQID_TPD :
        accn = sip;
        break;
      case SEQID_GPIPE :
        /* should not override better accession */
        gpp = sip;
        break;
      case SEQID_GENERAL :
        /* should not override better accession */
        gnl = sip;
        break;
      case SEQID_LOCAL :
        lcl = sip;
        break;
      default :
        break;
    }
  }

  sip = NULL;
  if (accn == NULL) {
    accn = gpp;
  }
  if (accn != NULL) {
    sip = accn;
  } else if (gnl != NULL) {
    sip = gnl;
  } else if (lcl != NULL) {
    sip = lcl;
  } else if (gi != NULL) {
    sip = gi;
  }

  if (sip == NULL) return;

  SeqIdWrite (sip, buf, PRINTID_TEXTID_ACC_ONLY, sizeof (buf));

  bbp = Asn2gbAddBlock (awp, ACCESSION_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  bbp->entityID = awp->entityID;

  if (accn == NULL) {

    /* if no accession, do not show local or general in ACCESSION */

    if (ajp->mode == ENTREZ_MODE || ajp->mode == SEQUIN_MODE) {
      buf [0] = '\0';
    }
  }

  FFStartPrint (ffstring, awp->format, 0, 12, "ACCESSION", 12, 5, 5, "AC", TRUE);

  if (awp->hup && accn != NULL) {
    FFAddOneString (ffstring, ";", FALSE, FALSE, TILDE_TO_SPACES);

  } else if (ajp->ajp.slp != NULL) {
    FF_www_accession (ajp, ffstring, buf);
    flatloc =  FFFlatLoc (ajp, bsp, ajp->ajp.slp, ajp->masterStyle);
    FFAddTextToString (ffstring, " REGION: ", flatloc, NULL, FALSE, FALSE, TILDE_TO_SPACES);
    MemFree (flatloc);
  } else {
    FFAddOneString (ffstring, buf, FALSE, FALSE, TILDE_TO_SPACES);
    if (awp->format == EMBL_FMT || awp->format == EMBLPEPT_FMT) {
        FFAddOneChar(ffstring, ';', FALSE);
    }
  }

  /* optionally populate indexes for NCBI internal database */

  if (ajp->index) {
    index = &asp->index;
  } else {
    index = NULL;
  }

  if (index != NULL) {
    index->accession = StringSave (buf);
  }

  /* optionally populate gbseq for XML-ized GenBank format */

  if (ajp->gbseq) {
    gbseq = &asp->gbseq;
  } else {
    gbseq = NULL;
  }

  if (gbseq != NULL) {
    gbseq->primary_accession = StringSave (buf);
  }

  if (awp->format == GENBANK_FMT || awp->format == GENPEPT_FMT) {
    separator = " ";
  } else if (awp->format == EMBL_FMT || awp->format == EMBLPEPT_FMT) {
    separator = " ";
  }

  if (ajp->ajp.slp == NULL) {
    sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &dcontext);
    if (sdp != NULL && wgsaccn != NULL) {
      mip = (MolInfoPtr) sdp->data.ptrvalue;
      if (mip != NULL && mip->tech == MI_TECH_wgs) {
        StringNCpy_0 (buf, wgsaccn, sizeof (buf));
        acclen = StringLen (buf);
        if (acclen == 12 && StringCmp (buf + len - 6, "000000") != 0) {
          StringCpy (buf + len - 6, "000000");
        } else if (acclen == 13 && StringCmp (buf + len - 7, "0000000") != 0) {
          StringCpy (buf + len - 7, "0000000");
        } else if (acclen == 15 && StringCmp (buf + len - 8, "00000000") != 0) {
          StringCpy (buf + len - 8, "00000000");
        } else {
          buf [0] = '\0';
        }
        if (! StringHasNoText (buf)) {
          if ( GetWWW(ajp) ) {
            FFAddTextToString(ffstring, separator, "<a href=", link_wgs, FALSE, FALSE, TILDE_IGNORE);
            FFAddTextToString(ffstring, "db=Nucleotide&cmd=Search&term=", buf, ">", FALSE, FALSE, TILDE_IGNORE);
            FFAddOneString (ffstring, buf, FALSE, FALSE, TILDE_TO_SPACES);
            FFAddOneString (ffstring, "</a>", FALSE, FALSE, TILDE_TO_SPACES);
          } else {
            FFAddTextToString(ffstring, separator, buf, NULL, FALSE, FALSE, TILDE_TO_SPACES);
          }
          if (awp->format == EMBL_FMT || awp->format == EMBLPEPT_FMT) {
              FFAddOneChar(ffstring, ';', FALSE);
          }
        }
      }
    }

    sdp = SeqMgrGetNextDescriptor (bsp, NULL, 0, &dcontext);
    while (sdp != NULL) {

      extra_access = NULL;

      switch (dcontext.seqdesctype) {
        case Seq_descr_genbank :
          gbp = (GBBlockPtr) sdp->data.ptrvalue;
          if (gbp != NULL) {
            extra_access = gbp->extra_accessions;
          }
          break;
        case Seq_descr_embl :
          ebp = (EMBLBlockPtr) sdp->data.ptrvalue;
          if (ebp != NULL) {
            extra_access = ebp->extra_acc;
          }
          break;
        default :
          break;
      }

      if (extra_access != NULL) {
        bbp->entityID = dcontext.entityID;
        bbp->itemID = dcontext.itemID;
        bbp->itemtype = OBJ_SEQDESC;

        
        secondary_acc = GetSecondaryAccessions(extra_access);
        for (vnp = secondary_acc; vnp != NULL; vnp = vnp->next) {
          xtra = (CharPtr)vnp->data.ptrvalue;
          FFAddTextToString(ffstring, separator, xtra, NULL, FALSE, FALSE, TILDE_IGNORE);
          if (awp->format == EMBL_FMT || awp->format == EMBLPEPT_FMT) {
            FFAddOneChar(ffstring, ';', FALSE);
          }

          /* optionally populate indexes for NCBI internal database */

          if (index != NULL) {
            ValNodeCopyStrToHead (&(index->secondaries), 0, xtra);
          }

          /* optionally populate gbseq for XML-ized GenBank format */

          if (gbseq != NULL) {
              ValNodeCopyStr (&(gbseq->secondary_accessions), 0, xtra);
          }
        }
        ValNodeFreeData(secondary_acc);
      }
      
      sdp = SeqMgrGetNextDescriptor (bsp, sdp, 0, &dcontext);
    }
  }

  bbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 5, 5, "AC");
  FFRecycleString(ajp, ffstring);

  if (awp->afp != NULL) {
    DoImmediateFormat (awp->afp, bbp);
  }
}

NLM_EXTERN void AddVersionBlock (
  Asn2gbWorkPtr awp
)

{
  SeqIdPtr         accn = NULL;
  IntAsn2gbJobPtr  ajp;
  Asn2gbSectPtr    asp;
  BaseBlockPtr     bbp;
  BioseqPtr        bsp;
  Char             buf [41];
  GBSeqPtr         gbseq;
  Int4             gi = -1;
  SeqIdPtr         gpp = NULL;
  IndxPtr          index;
  CharPtr          ptr;
  SeqIdPtr         sip;
  Char             tmp [41];
  Char             version [64];
  StringItemPtr    ffstring;

  if (awp == NULL) return;
  ajp = awp->ajp;
  if (ajp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;
  asp = awp->asp;
  if (asp == NULL) return;

  ffstring = FFGetString(ajp);
  if ( ffstring == NULL ) return;

  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    switch (sip->choice) {
      case SEQID_GI :
        gi = sip->data.intvalue;
        break;
      case SEQID_GENBANK :
      case SEQID_EMBL :
      case SEQID_DDBJ :
      case SEQID_OTHER :
        accn = sip;
        break;
      case SEQID_PIR :
      case SEQID_SWISSPROT :
      case SEQID_PRF :
      case SEQID_PDB :
        accn = sip;
        break;
      case SEQID_TPG :
      case SEQID_TPE :
      case SEQID_TPD :
        accn = sip;
        break;
      case SEQID_GPIPE :
        /* should not override better accession */
        gpp = sip;
        break;
      default :
        break;
    }
  }

  if (accn == NULL) {
    accn = gpp;
  }

  /* if (gi < 1 && accn == NULL) return; */

  if (awp->format == EMBL_FMT || awp->format == EMBLPEPT_FMT) {
    if ( accn == NULL ) return;
  }

  bbp = Asn2gbAddBlock (awp, VERSION_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  bbp->entityID = awp->entityID;

  /* no longer displaying NID */

  /*
  if (gi > 0) {
    sprintf (version, "g%ld", (long) gi);

    gb_StartPrint (awp->format, needInitBuff, 0, 12, "NID", 13, 5, 5, "NI", TRUE);
    needInitBuff = FALSE;

    gb_AddString (NULL, version, NULL, FALSE, FALSE, TILDE_TO_SPACES);

    ff_EndPrint();
    needEndPrint = FALSE;
  }
  */

  version [0] = '\0';

  if (awp->format == EMBL_FMT || awp->format == EMBLPEPT_FMT) {
    SeqIdWrite (accn, version, PRINTID_TEXTID_ACC_VER, sizeof (version) - 1);

    FFStartPrint (ffstring, awp->format, 0, 12, "VERSION", 12, 5, 5, "SV", TRUE);

    FFAddOneString (ffstring, version, FALSE, FALSE, TILDE_TO_SPACES);

    FFAddOneChar(ffstring, '\n', FALSE);

    bbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 5, 5, "SV");
    FFRecycleString(ajp, ffstring);

    if (awp->afp != NULL) {
      DoImmediateFormat (awp->afp, bbp);
    }

    return;
  }

  if (accn != NULL) {

    buf [0] = '\0';
    SeqIdWrite (accn, buf, PRINTID_TEXTID_ACC_VER, sizeof (buf) - 1);

    if (gi > 0) {
      sprintf (version, "%s  GI:%ld", buf, (long) gi);
    } else {
      sprintf (version, "%s", buf);
    }

    FFStartPrint (ffstring, awp->format, 0, 12, "VERSION", 12, 5, 5, "SV", TRUE);

    FFAddTextToString (ffstring, NULL, version, "\n", FALSE, FALSE, TILDE_TO_SPACES);
    /* optionally populate indexes for NCBI internal database */

    if (ajp->index) {
      index = &asp->index;
    } else {
      index = NULL;
    }

    if (index != NULL) {
      ptr = StringChr (buf, '.');
      if (ptr != NULL) {
        ptr++;
        index->version = StringSave (ptr);
      }
      if (gi > 0) {
        sprintf (tmp, "%ld", (long) gi);
        index->gi = StringSave (tmp);
      }
    }

    /* optionally populate gbseq for XML-ized GenBank format */

    if (ajp->gbseq) {
      gbseq = &asp->gbseq;
    } else {
      gbseq = NULL;
    }

    if (gbseq != NULL) {
      ptr = StringChr (buf, '.');
      if (ptr != NULL) {
        gbseq->accession_version = StringSave (buf);
      }
    }

  } else if (gi > 0) {

    FFStartPrint (ffstring, awp->format, 0, 0, "VERSION", 12, 5, 5, "SV", TRUE);

    sprintf (version, "  GI:%ld", (long) gi);

    FFAddTextToString (ffstring, NULL, version, "\n", FALSE, FALSE, TILDE_TO_SPACES);

  } else {

    FFStartPrint (ffstring, awp->format, 0, 0, "VERSION", 0, 5, 5, "SV", TRUE);
    FFAddOneChar(ffstring, '\n', FALSE);
    bbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 5, 5, "SV");
  }
  bbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 5, 5, "SV");
  FFRecycleString(ajp, ffstring);

  if (awp->afp != NULL) {
    DoImmediateFormat (awp->afp, bbp);
  }
}

/* only displaying PID in GenPept format */

static void AddPidBlock (Asn2gbWorkPtr awp)

{
  IntAsn2gbJobPtr  ajp;
  BaseBlockPtr  bbp;
  BioseqPtr     bsp;
  Int4          gi = -1;
  SeqIdPtr      sip;
  Char          version [64];
  StringItemPtr ffstring;

  if (awp == NULL) return;
  ajp = awp->ajp;
  if (ajp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;

  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    switch (sip->choice) {
      case SEQID_GI :
        gi = sip->data.intvalue;
        break;
      default :
        break;
    }
  }

  if (gi < 1) return;

  bbp = Asn2gbAddBlock (awp, PID_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  ffstring = FFGetString(ajp);
  if ( ffstring == NULL ) return;

  FFStartPrint (ffstring, awp->format, 0, 12, "PID", 12, 5, 5, NULL, TRUE);

  sprintf (version, "g%ld", (long) gi);
  FFAddOneString (ffstring, version, FALSE, FALSE, TILDE_TO_SPACES);

  bbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 5, 5, NULL);
  FFRecycleString(ajp, ffstring);
}

static Uint1 dbsource_fasta_order [NUM_SEQID] = {
  33, /* 0 = not set */
  20, /* 1 = local Object-id */
  15, /* 2 = gibbsq */
  16, /* 3 = gibbmt */
  30, /* 4 = giim Giimport-id */
  10, /* 5 = genbank */
  10, /* 6 = embl */
  10, /* 7 = pir */
  10, /* 8 = swissprot */
  15, /* 9 = patent */
  18, /* 10 = other TextSeqId */
  20, /* 11 = general Dbtag */
  31, /* 12 = gi */
  10, /* 13 = ddbj */
  10, /* 14 = prf */
  12, /* 15 = pdb */
  10, /* 16 = tpg */
  10, /* 17 = tpe */
  10, /* 18 = tpd */
  10  /* 19 = gpp */
};

static void AddToUniqueSipList (
  ValNodePtr PNTR list,
  SeqIdPtr sip
)

{
  ValNodePtr  vnp;

  if (list == NULL || sip == NULL) return;
  for (vnp = *list; vnp != NULL; vnp = vnp->next) {
    if (SeqIdMatch (sip, (SeqIdPtr) vnp->data.ptrvalue)) return;
  }
  ValNodeAddPointer (list, 0, (Pointer) sip);
}

static Boolean WriteDbsourceID (
  SeqIdPtr sip,
  CharPtr str
)

{
  DbtagPtr      db;
  CharPtr       dt;
  Int4          gi;
  ObjectIdPtr   oip;
  CharPtr       pfx;
  PDBSeqIdPtr   psip = NULL;
  CharPtr       prefix;
  Boolean       rsult = FALSE;
  CharPtr       sfx;
  CharPtr       suffix;
  Char          tmp [32];
  TextSeqIdPtr  tsip = NULL;

  if (sip == NULL || str == NULL) return FALSE;
  *str = '\0';
  switch (sip->choice) {
    case SEQID_LOCAL :
      oip = (ObjectIdPtr) sip->data.ptrvalue;
      if (oip == NULL) return FALSE;
      if (! StringHasNoText (oip->str)) {
        StringCat (str, oip->str);
        return TRUE;
      } else if (oip->id > 0) {
        sprintf (tmp, "%ld", (long) oip->id);
        StringCat (str, tmp);
        return TRUE;
      }
      return FALSE;
    case SEQID_GI :
      gi = (Int4) sip->data.intvalue;
      if (gi == 0) return FALSE;
      sprintf (tmp, "gi: %ld", (long) gi);
      StringCat (str, tmp);
      return TRUE;
    case SEQID_GENERAL :
      db = (DbtagPtr) sip->data.ptrvalue;
      if (db == NULL) return FALSE;
      /* !!! still need to implement this !!! */
      return FALSE;
    case SEQID_GENBANK :
    case SEQID_EMBL :
    case SEQID_DDBJ :
    case SEQID_OTHER :
    case SEQID_PIR :
    case SEQID_SWISSPROT :
    case SEQID_PRF :
    case SEQID_TPG :
    case SEQID_TPE :
    case SEQID_TPD :
    case SEQID_GPIPE :
      tsip = (TextSeqIdPtr) sip->data.ptrvalue;
      if (tsip == NULL) return FALSE;
      break;
    case SEQID_PDB :
      psip = (PDBSeqIdPtr) sip->data.ptrvalue;
      if (psip == NULL) return FALSE;
      break;
    default :
      break;
  }
  prefix = " ";
  suffix = NULL;
  switch (sip->choice) {
    case SEQID_EMBL :
      StringCat (str, "embl ");
      suffix = ",";
      break;
    case SEQID_OTHER :
      StringCat (str, "REFSEQ: ");
      break;
    case SEQID_SWISSPROT :
      StringCat (str, "swissprot: ");
      suffix = ",";
      break;
    case SEQID_PIR :
      StringCat (str, "pir: ");
      break;
    case SEQID_PRF :
      StringCat (str, "prf: ");
      break;
    case SEQID_PDB :
      StringCat (str, "pdb: ");
      suffix = ",";
      break;
    default :
      break;
  }
  pfx = NULL;
  sfx = NULL;
  if (tsip != NULL) {
    if (! StringHasNoText (tsip->name)) {
      StringCat (str, sfx);
      StringCat (str, pfx);
      StringCat (str, "locus ");
      StringCat (str, tsip->name);
      sfx = suffix;
      pfx = prefix;
      rsult = TRUE;
    }
    if (! StringHasNoText (tsip->accession)) {
      StringCat (str, sfx);
      StringCat (str, pfx);
      StringCat (str, "accession ");
      StringCat (str, tsip->accession);
      sfx = suffix;
      pfx = prefix;
      rsult = TRUE;
    }
    if (tsip->version > 0) {
      sprintf (tmp, ".%d", (int) tsip->version);
      StringCat (str, tmp);
      sfx = suffix;
      pfx = prefix;
    }
    if (! StringHasNoText (tsip->release)) {
      StringCat (str, pfx);
      StringCat (str, "release ");
      StringCat (str, tsip->release);
      sfx = suffix;
      pfx = prefix;
    }
    if (sip->choice == SEQID_SWISSPROT || sip->choice == SEQID_PIR || sip->choice == SEQID_PRF) {
      StringCat (str, ";");
    }
    return rsult;
  }
  if (psip != NULL) {
    if (! StringHasNoText (psip->mol)) {
      StringCat (str, "molecule ");
      StringCat (str, psip->mol);
      sfx = suffix;
      pfx = prefix;
      rsult = TRUE;
    }
    if (psip->chain > 0) {
      StringCat (str, sfx);
      StringCat (str, pfx);
      sprintf (tmp, "chain %d", (int) psip->chain);
      StringCat (str, tmp);
      sfx = suffix;
      pfx = prefix;
      rsult = TRUE;
    }
    if (psip->rel != NULL) {
      StringCat (str, sfx);
      StringCat (str, pfx);
      StringCat (str, "release ");
      dt = asn2gb_PrintDate (psip->rel);
      StringCat (str, dt);
      MemFree (dt);
      sfx = suffix;
      pfx = prefix;
      rsult = TRUE;
    }
    StringCat (str, ";");
    return rsult;
  }
  return rsult;
}


static void AddSPBlock (
  IntAsn2gbJobPtr ajp,
  StringItemPtr ffstring,
  BioseqPtr bsp
)

{
  CharPtr            acc;
  DbtagPtr           db;
  SeqMgrDescContext  dcontext;
  Boolean            first;
  Int4               gi;
  Boolean            has_link;
  Char               id [42];
  ObjectIdPtr        oip;
  SeqDescrPtr        sdp;
  SeqIdPtr           sid;
  SeqIdPtr           sif;
  SeqIdPtr           sip;
  SPBlockPtr         spb;
  CharPtr            string;
  ValNodePtr         vnp;
  CharPtr            str;
  Char               numbuf[40];

  if (bsp == NULL) return;
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_sp, &dcontext);
  if (sdp == NULL) return;
  spb = (SPBlockPtr) sdp->data.ptrvalue;
  if (spb == NULL) return;

  if (spb->_class == 1) {
    FFAddOneString (ffstring, "class: standard.", FALSE, FALSE, TILDE_IGNORE);
    FFAddNewLine(ffstring);
  } else if (spb->_class == 2) {
    FFAddOneString (ffstring, "class: preliminary.", FALSE, FALSE, TILDE_IGNORE);
    FFAddNewLine(ffstring);
  }

  if (spb->extra_acc) {
    FFAddOneString (ffstring, "extra accessions:", FALSE, FALSE, TILDE_IGNORE);
    for (vnp = spb->extra_acc; vnp != NULL; vnp = vnp->next) {
      FFAddOneString (ffstring, (CharPtr) vnp->data.ptrvalue, FALSE, FALSE, TILDE_IGNORE);
      FFAddOneChar (ffstring, ',', FALSE );
    }
  }

  if (spb->imeth) {
    FFAddOneString (ffstring, "seq starts with Met", FALSE, FALSE, TILDE_IGNORE);
  }

  if (spb->plasnm != NULL) {
    FFAddOneString (ffstring, "plasmid:", FALSE, FALSE, TILDE_IGNORE);
    for (vnp = spb->plasnm; vnp != NULL; vnp = vnp->next) {
      FFAddOneString (ffstring, (CharPtr) vnp->data.ptrvalue, FALSE, FALSE, TILDE_IGNORE);
      FFAddOneChar (ffstring, ',', FALSE );
    }
  }

  if (spb->created) {
    string = PrintDate (spb->created);
    FFAddOneString (ffstring, "created: ", FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString (ffstring, string, FALSE, FALSE, TILDE_IGNORE);

    MemFree (string);
  }

  if (spb->sequpd) {
    string = PrintDate (spb->sequpd);
    FFAddOneString (ffstring, "sequence updated: ", FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString (ffstring, string, FALSE, FALSE, TILDE_IGNORE);
    MemFree (string);
  }

  if (spb->annotupd) {
    string = PrintDate (spb->annotupd);
    FFAddOneString (ffstring, "annotation updated: ", FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString (ffstring, string, FALSE, FALSE, TILDE_IGNORE);
    MemFree (string);
  }

  if (spb->seqref) {
    FFAddOneString (ffstring, "xrefs: ", FALSE, FALSE, TILDE_IGNORE);
    first = TRUE;
    for (sid = spb->seqref; sid != NULL; sid = sid->next) {
      acc = NULL;
      has_link = FALSE;
      if (first == FALSE) {
        FFAddOneString (ffstring, ", ", FALSE, FALSE, TILDE_IGNORE);
      }
      first = FALSE;
      sip = sid;
      sif = NULL;
      id [0] = '\0';
      if (sip->choice == SEQID_GI) {
        gi = sid->data.intvalue;
        if (! GetAccnVerFromServer (gi, id)) {
          sif = GetSeqIdForGI (gi);
          if (sif != NULL) {
            sip = sif;
          }
        }
      }
      if (id [0] == '\0') {
        SeqIdWrite (sip, id, PRINTID_TEXTID_ACC_VER, sizeof (id) - 1);
      }
      if (sid->choice == SEQID_GI) {
        has_link = TRUE;
      }
      if (StringDoesHaveText (id)) {
        acc = id;
      }
      /*
      if (acc != NULL) {
        switch (sip->choice) {
          case SEQID_GENBANK:
            FFAddOneString (ffstring, "genbank accession ", FALSE, FALSE, TILDE_IGNORE);
            break; 
          case SEQID_EMBL:
            FFAddOneString (ffstring, "embl accession ", FALSE, FALSE, TILDE_IGNORE);
            break; 
          case SEQID_PIR:
            FFAddOneString (ffstring, "pir locus ", FALSE, FALSE, TILDE_IGNORE);
            break; 
          case SEQID_SWISSPROT:
            FFAddOneString (ffstring, "swissprot accession ", FALSE, FALSE, TILDE_IGNORE);
            break; 
          case SEQID_DDBJ:
            FFAddOneString (ffstring, "ddbj accession ", FALSE, FALSE, TILDE_IGNORE);
            break; 
          case SEQID_PRF:
            FFAddOneString (ffstring, "prf accession ", FALSE, FALSE, TILDE_IGNORE);
            break;
          case SEQID_PDB:
            FFAddOneString (ffstring, "pdb accession ", FALSE, FALSE, TILDE_IGNORE);
            break;
          case SEQID_GI:
            FFAddOneString (ffstring, "gi: ", FALSE, FALSE, TILDE_IGNORE);
            break; 
          case SEQID_TPG:
            FFAddOneString (ffstring, "genbank third party accession ", FALSE, FALSE, TILDE_IGNORE);
            break; 
          case SEQID_TPE:
            FFAddOneString (ffstring, "embl third party accession ", FALSE, FALSE, TILDE_IGNORE);
            break; 
          case SEQID_TPD:
            FFAddOneString (ffstring, "ddbj third party accession ", FALSE, FALSE, TILDE_IGNORE);
            break; 
          default:
            acc = NULL;
            break; 
        }
      }
      */
      if (acc != NULL) {
        if ( GetWWW(ajp) && has_link ) {
          sprintf(numbuf, "%ld", (long) sid->data.intvalue);
          FFAddTextToString(ffstring, "<a href=", link_seq, NULL, FALSE, FALSE, TILDE_IGNORE);
          FFAddTextToString(ffstring, "val=", numbuf, ">", FALSE, FALSE, TILDE_IGNORE);
          FFAddOneString(ffstring, acc, FALSE, FALSE, TILDE_IGNORE);
          FFAddOneString(ffstring, "</a>", FALSE, FALSE, TILDE_IGNORE);
        } else {
          FFAddOneString(ffstring, acc, FALSE, FALSE, TILDE_IGNORE);
        }
      }
      if (sif != NULL) {
        SeqIdFree (sif);
      }
    }
  }

  first = TRUE;
  for (vnp = spb->dbref; vnp != NULL; vnp = vnp->next) {
    db = (DbtagPtr) vnp->data.ptrvalue;
    if (db == NULL) continue;
    oip = db->tag;
    if (oip == NULL) continue;
    has_link = FALSE;
    if (first) {
      FFAddNewLine(ffstring);
      FFAddOneString (ffstring, "xrefs (non-sequence databases): ", FALSE, FALSE, TILDE_IGNORE);
      first = FALSE;
    } else {
      FFAddOneString (ffstring, ", ", FALSE, FALSE, TILDE_IGNORE);
    }
    if (StringCmp (db->db, "MGD") == 0 || StringCmp (db->db, "MGI") == 0) {
      FFAddOneString (ffstring, "MGI", FALSE, FALSE, TILDE_IGNORE);
    } else {
      FFAddOneString (ffstring, db->db, FALSE, FALSE, TILDE_IGNORE);
    }
    if (StringCmp (db->db, "MIM") == 0) {
      has_link = TRUE;
    }

    str = NULL;
    if ( oip->str != NULL ) {
      str = oip->str;
      if (StringNCmp (str, "GO:", 3) == 0) {
        str += 3;
      } else if (StringNCmp (str, "MGI:", 4) == 0) {
        str += 4;
      }
    } else if ( oip->id > 0 ) {
      sprintf(numbuf, "%d", oip->id);
      str = numbuf;
    }

    if ( !StringHasNoText(str) ) {
      if ( GetWWW(ajp) && has_link) {
        FFAddOneChar (ffstring, ' ', FALSE);
        FFAddTextToString(ffstring, "<a href=", link_omim, str, FALSE, FALSE, TILDE_IGNORE);
        FFAddTextToString(ffstring, ">", str, "</a>", FALSE, FALSE, TILDE_IGNORE);
      } else {
        FFAddOneString(ffstring, str, FALSE, FALSE, TILDE_IGNORE);
      }
    }
  }
}

static void AddPIRBlock (
  IntAsn2gbJobPtr ajp,
  StringItemPtr ffstring,
  BioseqPtr bsp
)

{
  CharPtr            acc;
  SeqMgrDescContext  dcontext;
  Boolean            first;
  Char               id [41];
  CharPtr            prefix = NULL;
  SeqDescrPtr        sdp;
  SeqIdPtr           sid;
  PirBlockPtr        pbp;

  if (bsp == NULL) return;
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_pir, &dcontext);
  if (sdp == NULL) return;
  pbp = (PirBlockPtr) sdp->data.ptrvalue;
  if (pbp == NULL) return;

  if (pbp->host != NULL) {
    FFAddTextToString (ffstring, "host:", pbp->host, "\n", FALSE, TRUE, TILDE_IGNORE);
    prefix = ";";
  }

  if (pbp->source != NULL) {
    FFAddOneString (ffstring, prefix, FALSE, FALSE, TILDE_IGNORE);
    FFAddNewLine(ffstring);
    FFAddTextToString(ffstring, "source: ", pbp->source, "\n", FALSE, TRUE, TILDE_IGNORE);
    prefix = ";";
  }

  if (pbp->summary != NULL) {
    FFAddOneString (ffstring, prefix, FALSE, FALSE, TILDE_IGNORE);
    FFAddNewLine(ffstring);
    FFAddTextToString(ffstring, "summary: ", pbp->summary, "\n", FALSE, TRUE, TILDE_IGNORE);
    prefix = ";";
  }

  if (pbp->genetic != NULL) {
    FFAddOneString (ffstring, prefix, FALSE, FALSE, TILDE_IGNORE);
    FFAddNewLine(ffstring);
    FFAddTextToString(ffstring, "genetic: ", pbp->genetic, "\n", FALSE, TRUE, TILDE_IGNORE);
    prefix = ";";
  }

  if (pbp->includes != NULL) {
    FFAddOneString (ffstring, prefix, FALSE, FALSE, TILDE_IGNORE);
    FFAddNewLine(ffstring);
    FFAddTextToString(ffstring, "includes: ", pbp->includes, "\n", FALSE, TRUE, TILDE_IGNORE);
    prefix = ";";
  }

  if (pbp->placement != NULL) {
    FFAddOneString (ffstring, prefix, FALSE, FALSE, TILDE_IGNORE);
    FFAddNewLine(ffstring);
    FFAddTextToString(ffstring, "placement: ", pbp->placement, "\n", FALSE, TRUE, TILDE_IGNORE);
    prefix = ";";
  }

  if (pbp->superfamily != NULL) {
    FFAddOneString (ffstring, prefix, FALSE, FALSE, TILDE_IGNORE);
    FFAddNewLine(ffstring);
    FFAddTextToString(ffstring, "superfamily: ", pbp->superfamily, "\n", FALSE, TRUE, TILDE_IGNORE);
    prefix = ";";
  }

  if (pbp->cross_reference != NULL) {
    FFAddOneString (ffstring, prefix, FALSE, FALSE, TILDE_IGNORE);
    FFAddNewLine(ffstring);
    FFAddTextToString(ffstring, "xref: ", pbp->cross_reference, "\n", FALSE, TRUE, TILDE_IGNORE);
    prefix = ";";
  }

  if (pbp->date != NULL) {
    FFAddOneString (ffstring, prefix, FALSE, FALSE, TILDE_IGNORE);
    FFAddNewLine(ffstring);
    FFAddTextToString (ffstring, "PIR dates: ", pbp->date, "\n", FALSE, TRUE, TILDE_IGNORE);
    prefix = ";";
  }

  if (pbp->had_punct) {
    FFAddOneString (ffstring, prefix, FALSE, FALSE, TILDE_IGNORE);
    FFAddNewLine(ffstring);
    FFAddOneString (ffstring, "punctuation in sequence", FALSE, FALSE, TILDE_IGNORE);
    prefix = ";";
  }

  if (pbp->seqref) {
    FFAddOneString (ffstring, prefix, FALSE, FALSE, TILDE_IGNORE);
    FFAddNewLine(ffstring);
    FFAddOneString (ffstring, "xrefs: ", FALSE, FALSE, TILDE_IGNORE);
    first = TRUE;
    for (sid = pbp->seqref; sid != NULL; sid = sid->next) {
      acc = NULL;
      if (first == FALSE) {
        FFAddOneString (ffstring, ", ", FALSE, FALSE, TILDE_IGNORE);
      }
      first = FALSE;
      SeqIdWrite (sid, id, PRINTID_TEXTID_ACC_VER, sizeof (id) - 1);
      acc = id;
      if (acc != NULL) {
        switch (sid->choice) {
          case SEQID_GENBANK:
            FFAddOneString (ffstring, "genbank ", FALSE, FALSE, TILDE_IGNORE);
            break; 
          case SEQID_EMBL:
            FFAddOneString (ffstring, "embl ", FALSE, FALSE, TILDE_IGNORE);
            break; 
          case SEQID_PIR:
            FFAddOneString (ffstring, "pir ", FALSE, FALSE, TILDE_IGNORE);
            break; 
          case SEQID_SWISSPROT:
            FFAddOneString (ffstring, "swissprot ", FALSE, FALSE, TILDE_IGNORE);
            break; 
          case SEQID_DDBJ:
            FFAddOneString (ffstring, "ddbj ", FALSE, FALSE, TILDE_IGNORE);
            break; 
          case SEQID_PRF:
            FFAddOneString (ffstring, "prf ", FALSE, FALSE, TILDE_IGNORE);
            break; 
          case SEQID_GI:
            FFAddOneString (ffstring, "gi: ", FALSE, FALSE, TILDE_IGNORE);
            break; 
          default:
            acc = NULL;
            break; 
        }
      }
      if (acc != NULL) {
        FFAddOneString (ffstring, acc, FALSE, FALSE, TILDE_IGNORE);
      }
    }
  }
  FFAddOneString (ffstring, ".", FALSE, FALSE, TILDE_IGNORE);
}

static void AddPRFBlock (
  IntAsn2gbJobPtr ajp,
  StringItemPtr ffstring,
  BioseqPtr bsp
)

{
  SeqMgrDescContext  dcontext;
  PrfExtSrcPtr       extra;
  CharPtr            prefix = NULL;
  SeqDescrPtr        sdp;
  PrfBlockPtr        prf;

  if (bsp == NULL) return;
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_prf, &dcontext);
  if (sdp == NULL) return;
  prf = (PrfBlockPtr) sdp->data.ptrvalue;
  if (prf == NULL) return;
  if ( ffstring == NULL ) return;

  extra = prf->extra_src;
  if (extra != NULL) {

    if (extra->host != NULL) {
      FFAddTextToString(ffstring, "host:", extra->host, NULL, FALSE, TRUE, TILDE_IGNORE);
      prefix = ";";
    }

    if (extra->part != NULL) {
      FFAddOneString(ffstring, prefix, FALSE, FALSE, TILDE_IGNORE);
      FFAddNewLine(ffstring);
      FFAddTextToString(ffstring, "part: ", extra->part, NULL, FALSE, TRUE, TILDE_IGNORE);
      prefix = ";";
    }
    if (extra->state != NULL) {
      FFAddOneString(ffstring, prefix, FALSE, FALSE, TILDE_IGNORE);
      FFAddNewLine(ffstring);
      FFAddTextToString(ffstring, "state: ", extra->state, NULL, FALSE, TRUE, TILDE_IGNORE);
      prefix = ";";
    }
    if (extra->strain != NULL) {
      FFAddOneString(ffstring, prefix, FALSE, FALSE, TILDE_IGNORE);
      FFAddNewLine(ffstring);
      FFAddTextToString(ffstring, "strain: ", extra->strain, NULL, FALSE, TRUE, TILDE_IGNORE);
      prefix = ";";
    }
    if (extra->taxon != NULL) {
      FFAddOneString(ffstring, prefix, FALSE, FALSE, TILDE_IGNORE);
      FFAddNewLine(ffstring);
      FFAddTextToString(ffstring, "taxonomy: ", extra->taxon, NULL, FALSE, TRUE, TILDE_IGNORE);
      prefix = ";";
    }

    FFAddOneChar(ffstring, '.', FALSE);
  }
}

static void AddPDBBlock (
  IntAsn2gbJobPtr ajp,
  StringItemPtr ffstring,
  BioseqPtr bsp
)

{
  SeqMgrDescContext  dcontext;
  CharPtr            dt;
  CharPtr            prefix = NULL;
  SeqDescrPtr        sdp;
  PdbBlockPtr        pdb;
  PdbRepPtr          replace;
  CharPtr            str;
  ValNodePtr         vnp;

  if (bsp == NULL) return;
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_pdb, &dcontext);
  if (sdp == NULL) return;
  pdb = (PdbBlockPtr) sdp->data.ptrvalue;
  if (pdb == NULL) return;

  if (pdb->deposition != NULL) {
    dt = asn2gb_PrintDate (pdb->deposition);
    FFAddTextToString (ffstring, "deposition: ", dt, NULL, FALSE, TRUE, TILDE_IGNORE);
    MemFree (dt);
    prefix = ";";
  }
  if (pdb->pdbclass != NULL) {
    FFAddOneString(ffstring, prefix, FALSE, FALSE, TILDE_IGNORE);
    FFAddNewLine(ffstring);
    FFAddTextToString(ffstring, "class: ", pdb->pdbclass, NULL, FALSE, TRUE, TILDE_IGNORE);
    prefix = ";";
  }
  if (pdb->source != NULL) {
    FFAddOneString(ffstring, prefix, FALSE, FALSE, TILDE_IGNORE);
    FFAddNewLine(ffstring);
    FFAddOneString(ffstring, "source: ", FALSE, TRUE, TILDE_IGNORE);
    prefix = NULL;
    for (vnp = pdb->source; vnp != NULL; vnp = vnp->next) {
      str = (CharPtr) vnp->data.ptrvalue;
      if (StringHasNoText (str)) continue;
      FFAddTextToString (ffstring, prefix, str, NULL, FALSE, TRUE, TILDE_IGNORE);
      prefix = ", ";
    }
    prefix = ";";
  }
  if (pdb->exp_method != NULL) {
    FFAddOneString(ffstring, prefix, FALSE, FALSE, TILDE_IGNORE);
    FFAddNewLine(ffstring);
    FFAddTextToString(ffstring, "Exp. method: ", pdb->exp_method, NULL, FALSE, TRUE, TILDE_IGNORE);
    prefix = ";";
  }
  replace = pdb->replace;
  if (replace != NULL) {
    if (replace->ids != NULL) {
      FFAddOneString(ffstring, prefix, FALSE, FALSE, TILDE_IGNORE);
      FFAddNewLine(ffstring);
      FFAddOneString(ffstring, "ids replaced: ", FALSE, TRUE, TILDE_IGNORE);

      prefix = NULL;
      for (vnp = replace->ids; vnp != NULL; vnp = vnp->next) {
        str = (CharPtr) vnp->data.ptrvalue;
        if (StringHasNoText (str)) continue;
        FFAddTextToString (ffstring, prefix, str, NULL, FALSE, TRUE, TILDE_IGNORE);
        prefix = ", ";
      }
      prefix = ";";
    }
    if (replace->date != NULL) {
      FFAddOneString(ffstring, prefix, FALSE, FALSE, TILDE_IGNORE);
      FFAddNewLine(ffstring);

      dt = asn2gb_PrintDate (replace->date);
      FFAddTextToString(ffstring, "replacement date: ", dt, NULL, FALSE, TRUE, TILDE_IGNORE);
      MemFree (dt);
      prefix = ";";
    }
  }

  FFAddOneChar(ffstring, '.', FALSE);
}

static CharPtr TxtSave (CharPtr text, size_t len)

{
   CharPtr str = NULL;

   if ((text == NULL) || (len == 0))
      return str;

   str = MemNew((size_t)(len + 1));
   MemCopy(str, text, (size_t)len);

   return (str);
}

static Boolean FF_www_dbsource(
  IntAsn2gbJobPtr ajp,
  StringItemPtr ffstring,
  CharPtr str,
  Boolean first,
  Uint1 choice
)
{
  CharPtr  temp, end, text, loc, link=NULL;
  Int2 j;

  if( GetWWW(ajp) ) {
    if (choice == SEQID_PIR) {
      link = link_seq;
    } else if (choice == SEQID_SWISSPROT) {
      link = link_sp;
    } else if (choice == SEQID_PDB || choice == SEQID_PRF) {
      link = link_seq;
    } else if (choice == SEQID_EMBL || choice == SEQID_GENBANK || 
        choice == SEQID_DDBJ || choice == SEQID_GIBBSQ || 
        choice == SEQID_GIBBMT || choice == SEQID_GI || 
        choice == SEQID_GIIM || choice == SEQID_OTHER ||
        choice == SEQID_TPG || choice == SEQID_TPE || choice == SEQID_TPD ||
        choice == SEQID_GPIPE)  {
      link = link_seq;
    } else {
      AddStringWithTildes(ffstring, str);
      return TRUE;
    }
  
    if ((text = StringStr(str, "accession")) != NULL) {
      end = text + 9;
      j = 9;
      while (*end == ' ') {
        ++end;
        j++;
      }
      if (first == FALSE) {
        FFAddOneString(ffstring, ", ", FALSE, FALSE, TILDE_IGNORE);
      }
      loc = TxtSave (str, end-str - j);
      FFAddOneString(ffstring, loc, FALSE, FALSE, TILDE_IGNORE);
      MemFree (loc);
      for (; text != end; ++text ) {
        FFAddOneChar(ffstring, *text, FALSE);
      }

      temp = text;
      end += StringLen(text) - 1;
      if ( *end != ';' ) {
        ++end;
      }

      if (choice == SEQID_SWISSPROT) {
        FFAddTextToString(ffstring, "<a href=", link, NULL, FALSE, FALSE, TILDE_IGNORE);
      } else {
        FFAddTextToString(ffstring, "<a href=", link, "val=", FALSE, FALSE, TILDE_IGNORE);
      }
      for (text = temp; text != end; ++text ) {
        FFAddOneChar(ffstring, *text, FALSE);
      }
      FFAddOneString(ffstring, ">", FALSE, FALSE, TILDE_IGNORE);

      for (text = temp; text != end; ++text ) {
        FFAddOneChar(ffstring, *text, FALSE);
      }
      FFAddOneString(ffstring, "</a>", FALSE, FALSE, TILDE_IGNORE);
      if ( *end == ';' ) {
        FFAddOneChar(ffstring, ';', FALSE);
      }
    } else {
      if (first == FALSE) {
        FFAddOneString(ffstring, ", ", FALSE, FALSE, TILDE_IGNORE);
      }
      FFAddOneString(ffstring, str, FALSE, FALSE, TILDE_IGNORE);
    }
  } else {
    AddStringWithTildes(ffstring, str);
  }
  return TRUE;
}

NLM_EXTERN void AddDbsourceBlock (
  Asn2gbWorkPtr awp
)

{
  IntAsn2gbJobPtr  ajp;
  Asn2gbSectPtr    asp;
  BaseBlockPtr     bbp;
  BioseqPtr        bsp;
  Char             buf [256];
  SeqFeatPtr       cds;
  DbtagPtr         db;
  GBSeqPtr         gbseq;
  SeqIdPtr         id;
  ValNodePtr       list = NULL;
  BioseqPtr        nuc;
  SeqIdPtr         sip;
  SeqLocPtr        slp;
  CharPtr          str;
  Boolean          unknown = TRUE;
  ValNodePtr       vnp;
  StringItemPtr    ffstring;

  if (awp == NULL) return;
  ajp = awp->ajp;
  if (ajp == NULL) return;
  asp = awp->asp;
  if (asp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;

  bbp = Asn2gbAddBlock (awp, DBSOURCE_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  bbp->entityID = awp->entityID;

  ffstring = FFGetString(ajp);
  if ( ffstring == NULL ) return;

  FFStartPrint (ffstring, awp->format, 0, 12, "DBSOURCE", 12, 5, 5, NULL, TRUE);

  sip = SeqIdSelect (bsp->id, dbsource_fasta_order, NUM_SEQID);

  if (sip != NULL) {

    switch (sip->choice) {
      case SEQID_PIR :
      case SEQID_SWISSPROT :
      case SEQID_PRF :
      case SEQID_PDB :
        if (WriteDbsourceID (sip, buf)) {
          FF_www_dbsource (ajp, ffstring, buf, TRUE, sip->choice);
          FFAddNewLine(ffstring);
          unknown = FALSE;
        }
        break;
      case SEQID_GENERAL :
        db = sip->data.ptrvalue;
        if (db == NULL) {
          break;
        }
        if (StringNCmp (db->db, "PIDe", 4) != 0 &&
            StringNCmp (db->db, "PIDd", 4) != 0 &&
            StringNCmp (db->db, "PID", 3) != 0) {
          break;
        }
        /* if (ChoicePID) found, continue on to next set of cases */
      case SEQID_EMBL :
      case SEQID_GENBANK :
      case SEQID_DDBJ :
      case SEQID_GIBBSQ :
      case SEQID_GIBBMT :
      case SEQID_OTHER :
      case SEQID_TPG :
      case SEQID_TPE :
      case SEQID_TPD :
      case SEQID_GPIPE :
      case SEQID_GI :
      case SEQID_GIIM :
        cds = SeqMgrGetCDSgivenProduct (bsp, NULL);
        if (cds == NULL) {
          /* now may also be protein product of mature peptide feature */
          cds = SeqMgrGetPROTgivenProduct (bsp, NULL);
        }
        if (cds != NULL) {
          nuc = BioseqFindFromSeqLoc (cds->location);
          if (nuc != NULL) {
            slp = SeqLocFindNext (cds->location, NULL);
            while (slp != NULL) {
              sip = SeqLocId (slp);
              AddToUniqueSipList (&list, sip);
              slp = SeqLocFindNext (cds->location, slp);
            }
            for (vnp = list; vnp != NULL; vnp = vnp->next) {
              id = (SeqIdPtr) vnp->data.ptrvalue;
              nuc = BioseqFindCore (id);
              sip = NULL;
              if (nuc != NULL) {
                sip = SeqIdSelect (nuc->id, dbsource_fasta_order, NUM_SEQID);
              } else if (id != NULL && id->choice == SEQID_GI) {
                sip = GetSeqIdForGI (id->data.intvalue);
              }
              if (sip == NULL) {
                sip = id;
              }
              if (sip != NULL) {
                if (WriteDbsourceID (sip, buf)) {
                  FF_www_dbsource (ajp, ffstring, buf, TRUE, sip->choice);
                  FFAddNewLine(ffstring);
                  unknown = FALSE;
                }
              }
            }
            ValNodeFree (list);
          }
        } else {
          if (WriteDbsourceID (sip, buf)) {
            FF_www_dbsource (ajp, ffstring, buf, TRUE, sip->choice);
            FFAddNewLine(ffstring);
            unknown = FALSE;
          }
        }
        break;
      default :
        break;
    }

    switch (sip->choice) {
      case SEQID_PIR :
        AddPIRBlock (ajp, ffstring, bsp);
        break;
      case SEQID_SWISSPROT :
        AddSPBlock (ajp, ffstring, bsp);
        break;
      case SEQID_PRF :
        AddPRFBlock (ajp, ffstring, bsp);
        break;
      case SEQID_PDB :
        AddPDBBlock (ajp, ffstring, bsp);
        break;
      default :
        break;
    }
  }

  if (unknown) {
    FFAddOneString (ffstring, "UNKNOWN", FALSE, FALSE, TILDE_TO_SPACES);
  }

  str = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 5, 5, NULL);

  /* optionally populate gbseq for XML-ized GenBank format */

  if (ajp->gbseq) {
    gbseq = &asp->gbseq;
  } else {
    gbseq = NULL;
  }

  if (gbseq != NULL) {
    if (StringNCmp (str, "DBSOURCE    ", 12) == 0) {
      gbseq->source_db = StringSave (str + 12);
    } else {
      gbseq->source_db = StringSave (str);
    }
    CleanQualValue (gbseq->source_db);
    Asn2gnbkCompressSpaces (gbseq->source_db);
  }

  bbp->string = str;
  FFRecycleString(ajp, ffstring);

  if (awp->afp != NULL) {
    DoImmediateFormat (awp->afp, bbp);
  }
}

NLM_EXTERN void AddDateBlock (
  Asn2gbWorkPtr awp
)

{
  IntAsn2gbJobPtr    ajp;
  BaseBlockPtr       bbp;
  BioseqPtr          bsp;
  Char               date [40];
  SeqMgrDescContext  dcontext;
  DatePtr            dp;
  SeqDescrPtr        sdp;
  StringItemPtr      ffstring;

  if (awp == NULL) return;
  ajp = awp->ajp;
  if (ajp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;

  ffstring = FFGetString(ajp);
  if ( ffstring == NULL ) return;

  bbp = Asn2gbAddBlock (awp, DATE_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  date [0] = '\0';

  dp = NULL;
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_create_date, &dcontext);
  if (sdp != NULL) {
    dp = (DatePtr) sdp->data.ptrvalue;
  }
  if (dp != NULL) {
    DateToFF (date, dp, FALSE);
  }
  if (StringHasNoText (date)) {
    StringCpy (date, "01-JAN-1900");
  }

  FFStartPrint (ffstring, awp->format, 0, 0, NULL, 0, 5, 5, "DT", TRUE);
  FFAddOneString (ffstring, date, FALSE, FALSE, TILDE_IGNORE);

  bbp->string = FFEndPrint(ajp, ffstring, awp->format, 0, 0, 5, 5, "DT");
  FFRecycleString(ajp, ffstring);

  bbp = Asn2gbAddBlock (awp, DATE_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  ffstring = FFGetString(ajp);

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_update_date, &dcontext);
  if (sdp != NULL) {
    dp = (DatePtr) sdp->data.ptrvalue;
  }
  if (dp != NULL) {
    DateToFF (date, dp, FALSE);
  }

  FFStartPrint (ffstring, awp->format, 0, 0, NULL, 0, 5, 5, "DT", FALSE);
  FFAddOneString (ffstring, date, FALSE, FALSE, TILDE_IGNORE);

  bbp->string = FFEndPrint(ajp, ffstring, awp->format, 0, 0, 5, 5, "DT");
  FFRecycleString(ajp, ffstring);

  if (awp->afp != NULL) {
    DoImmediateFormat (awp->afp, bbp);
  }
}


#define TOTAL_ESTKW 11
#define TOTAL_STSKW 5
#define TOTAL_GSSKW 2

static CharPtr EST_kw_array[ TOTAL_ESTKW] = {
  "EST", "EST PROTO((expressed sequence tag)", "expressed sequence tag",
  "EST (expressed sequence tag)", "EST(expressed sequence tag)",
  "partial cDNA sequence", "transcribed sequence fragment", "TSR",
  "putatively transcribed partial sequence", "UK putts"
};

static CharPtr GSS_kw_array [TOTAL_GSSKW] = {
  "GSS", "trapped exon"
};
static CharPtr STS_kw_array[TOTAL_STSKW] = {
  "STS", "STS(sequence tagged site)", "STS (sequence tagged site)",
  "STS sequence", "sequence tagged site"
};

static Int2 MatchArrayString (
  CharPtr array_string [],
  Int2 totalstr,
  CharPtr text
)

{
  Int2 i;

  for (i = 0; i < totalstr && text != NULL; i++) {
    if (StringCmp (array_string [i], text) == 0) {
      return (i);
    }
  }

  return (-1);
}

static Boolean CheckSpecialKeyword (
  Boolean is_est,
  Boolean is_sts,
  Boolean is_gss,
  CharPtr kwd
)

{
  if (kwd == NULL) return FALSE;

  if (is_est) {
    if (MatchArrayString (STS_kw_array, TOTAL_STSKW, kwd) != -1) return FALSE;
    if (MatchArrayString (GSS_kw_array, TOTAL_GSSKW, kwd) != -1) return FALSE;
  }

  if (is_sts) {
    if (MatchArrayString (EST_kw_array, TOTAL_ESTKW, kwd) != -1) return FALSE;
    if (MatchArrayString (GSS_kw_array, TOTAL_GSSKW, kwd) != -1) return FALSE;
  }

  if (is_gss) {
    if (MatchArrayString (STS_kw_array, TOTAL_STSKW, kwd) != -1) return FALSE;
    if (MatchArrayString (EST_kw_array, TOTAL_ESTKW, kwd) != -1) return FALSE;
  }

  return TRUE;
}

static Boolean KeywordAlreadyInList (
  ValNodePtr head,
  CharPtr kwd
)

{
  ValNodePtr  vnp;

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    if (StringICmp ((CharPtr) vnp->data.ptrvalue, kwd) == 0) return TRUE;
  }

  return FALSE;
}


NLM_EXTERN void AddKeywordsBlock (
  Asn2gbWorkPtr awp
)

{
  Boolean            add_encode = FALSE;
  IntAsn2gbJobPtr    ajp;
  Asn2gbSectPtr      asp;
  BaseBlockPtr       bbp;
  BioseqPtr          bsp;
  BioSourcePtr       biop;
  SeqMgrDescContext  dcontext;
  EMBLBlockPtr       ebp;
  GBBlockPtr         gbp;
  GBSeqPtr           gbseq;
  ValNodePtr         head = NULL;
  IndxPtr            index;
  Boolean            is_est = FALSE;
  Boolean            is_gss = FALSE;
  Boolean            is_sts = FALSE;
  Boolean            is_env_sample = FALSE;
  ValNodePtr         keywords;
  CharPtr            kwd;
  MolInfoPtr         mip;
  ObjectIdPtr        oip;
  PirBlockPtr        pir;
  PrfBlockPtr        prf;
  SeqDescrPtr        sdp;
  SeqIdPtr           sip;
  SPBlockPtr         sp;
  SubSourcePtr       ssp;
  CharPtr            str;
  UserObjectPtr      uop;
  ValNodePtr         vnp;
  StringItemPtr      ffstring;

  if (awp == NULL) return;
  ajp = awp->ajp;
  if (ajp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;
  asp = awp->asp;
  if (asp == NULL) return;

  bbp = (BaseBlockPtr) Asn2gbAddBlock (awp, KEYWORDS_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  ffstring = FFGetString(ajp);
  if ( ffstring == NULL ) return;

  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_TPG || sip->choice == SEQID_TPE || sip->choice == SEQID_TPD) {
      ValNodeCopyStr (&head, 0, "Third Party Annotation");
      ValNodeCopyStr (&head, 0, "; ");
      ValNodeCopyStr (&head, 0, "TPA");
    }
  }

  biop = NULL;
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_source, &dcontext);
  if (sdp != NULL) {
    biop = (BioSourcePtr) sdp->data.ptrvalue;
  }
  if (biop != NULL) {
    for (ssp = biop->subtype; ssp != NULL; ssp = ssp->next) {
      if (ssp->subtype == SUBSRC_environmental_sample) {
        is_env_sample = TRUE;
      }
    }
  }

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &dcontext);
  if (sdp != NULL) {
    bbp->entityID = dcontext.entityID;
    bbp->itemID = dcontext.itemID;
    bbp->itemtype = OBJ_SEQDESC;

    mip = (MolInfoPtr) sdp->data.ptrvalue;
    if (mip != NULL) {
      switch (mip->tech) {
        case MI_TECH_htgs_1 :
          if (head != NULL) {
            ValNodeCopyStr (&head, 0, "; ");
          }
          ValNodeCopyStr (&head, 0, "HTG");
          ValNodeCopyStr (&head, 0, "; ");
          ValNodeCopyStr (&head, 0, "HTGS_PHASE1");
          break;
        case MI_TECH_htgs_2 :
          if (head != NULL) {
            ValNodeCopyStr (&head, 0, "; ");
          }
          ValNodeCopyStr (&head, 0, "HTG");
          ValNodeCopyStr (&head, 0, "; ");
          ValNodeCopyStr (&head, 0, "HTGS_PHASE2");
          break;
        case MI_TECH_htgs_3 :
          if (head != NULL) {
            ValNodeCopyStr (&head, 0, "; ");
          }
          ValNodeCopyStr (&head, 0, "HTG");
          break;
        case MI_TECH_est :
          if (head != NULL) {
            ValNodeCopyStr (&head, 0, "; ");
          }
          is_est = TRUE;
          ValNodeCopyStr (&head, 0, "EST");
          break;
        case MI_TECH_sts :
          if (head != NULL) {
            ValNodeCopyStr (&head, 0, "; ");
          }
          is_sts = TRUE;
          ValNodeCopyStr (&head, 0, "STS");
          break;
        case MI_TECH_survey :
          if (head != NULL) {
            ValNodeCopyStr (&head, 0, "; ");
          }
          is_gss = TRUE;
          ValNodeCopyStr (&head, 0, "GSS");
          break;
        case MI_TECH_fli_cdna :
          if (head != NULL) {
            ValNodeCopyStr (&head, 0, "; ");
          }
          ValNodeCopyStr (&head, 0, "FLI_CDNA");
          break;
        case MI_TECH_htgs_0 :
          if (head != NULL) {
            ValNodeCopyStr (&head, 0, "; ");
          }
          ValNodeCopyStr (&head, 0, "HTG");
          ValNodeCopyStr (&head, 0, "; ");
          ValNodeCopyStr (&head, 0, "HTGS_PHASE0");
          break;
        case MI_TECH_htc :
          if (head != NULL) {
            ValNodeCopyStr (&head, 0, "; ");
          }
          ValNodeCopyStr (&head, 0, "HTC");
          break;
        case MI_TECH_wgs :
          if (head != NULL) {
            ValNodeCopyStr (&head, 0, "; ");
          }
          ValNodeCopyStr (&head, 0, "WGS");
          break;
        case MI_TECH_barcode :
          if (head != NULL) {
            ValNodeCopyStr (&head, 0, "; ");
          }
          ValNodeCopyStr (&head, 0, "BARCODE");
          break;
        case MI_TECH_unknown :
        case MI_TECH_standard :
        case MI_TECH_other :
          if (is_env_sample) {
            if (head != NULL) {
              ValNodeCopyStr (&head, 0, "; ");
            }
            ValNodeCopyStr (&head, 0, "ENV");
          }
          break;
        default :
          break;
      }
    }
  }

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_user, &dcontext);
  while (sdp != NULL) {
    uop = (UserObjectPtr) sdp->data.ptrvalue;
    if (uop != NULL) {
      oip = uop->type;
      if (oip != NULL && StringICmp (oip->str, "ENCODE") == 0) {
        add_encode = TRUE;
      }
    }
    sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_user, &dcontext);
  }
  if (add_encode) {
    if (head != NULL) {
      ValNodeCopyStr (&head, 0, "; ");
    }
    ValNodeCopyStr (&head, 0, "ENCODE");
  }

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, 0, &dcontext);
  while (sdp != NULL) {

    keywords = NULL;

    switch (dcontext.seqdesctype) {
      case Seq_descr_genbank :
        gbp = (GBBlockPtr) sdp->data.ptrvalue;
        if (gbp != NULL) {
          keywords = gbp->keywords;
        }
        break;
      case Seq_descr_embl :
        ebp = (EMBLBlockPtr) sdp->data.ptrvalue;
        if (ebp != NULL) {
          keywords = ebp->keywords;
        }
        break;
      case Seq_descr_pir :
        pir = (PirBlockPtr) sdp->data.ptrvalue;
        if (pir != NULL) {
          keywords = pir->keywords;
        }
        break;
      case Seq_descr_prf :
        prf = (PrfBlockPtr) sdp->data.ptrvalue;
        if (prf != NULL) {
          keywords = prf->keywords;
        }
        break;
      case Seq_descr_sp :
        sp = (SPBlockPtr) sdp->data.ptrvalue;
        if (sp != NULL) {
          keywords = sp->keywords;
        }
        break;
      default :
        break;
    }

    if (keywords != NULL) {
      bbp->entityID = dcontext.entityID;
      bbp->itemID = dcontext.itemID;
      bbp->itemtype = OBJ_SEQDESC;
    }

    for (vnp = keywords; vnp != NULL; vnp = vnp->next) {
      kwd = (CharPtr) vnp->data.ptrvalue;
      if (CheckSpecialKeyword (is_est, is_sts, is_gss, kwd)) {
        if (! KeywordAlreadyInList (head, kwd)) {
          if (head != NULL) {
            ValNodeCopyStr (&head, 0, "; ");
          }
          ValNodeCopyStr (&head, 0, kwd);
        }
      }
    }

    sdp = SeqMgrGetNextDescriptor (bsp, sdp, 0, &dcontext);
  }

  FFStartPrint( ffstring, awp->format, 0, 12, "KEYWORDS", 12, 5, 5, "KW", TRUE);
  str = MergeFFValNodeStrs (head);
  
  /* if no keywords were found, period will still be added by this call */
  if ( str != NULL ) {
    FFAddOneString (ffstring, str, TRUE, FALSE, TILDE_TO_SPACES);
  } else {
    FFAddOneChar(ffstring, '.', FALSE);
  }

  MemFree (str);

  /* optionally populate indexes for NCBI internal database */

  if (ajp->index) {
    index = &asp->index;
  } else {
    index = NULL;
  }

  if (index != NULL) {
    for (vnp = head; vnp != NULL; vnp = vnp->next) {
      kwd = (CharPtr) vnp->data.ptrvalue;
      if (StringCmp (kwd, "; ") == 0) continue;
      ValNodeCopyStrToHead (&(index->keywords), 0, kwd);
    }
  }

  /* optionally populate gbseq for XML-ized GenBank format */

  if (ajp->gbseq) {
    gbseq = &asp->gbseq;
  } else {
    gbseq = NULL;
  }

  if (gbseq != NULL) {
    for (vnp = head; vnp != NULL; vnp = vnp->next) {
      kwd = (CharPtr) vnp->data.ptrvalue;
      if (StringCmp (kwd, "; ") == 0) continue;
      ValNodeCopyStr (&(gbseq->keywords), 0, kwd);
    }
  }

  ValNodeFreeData (head);

  bbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 5, 5, "KW");

  FFRecycleString(ajp, ffstring);

  if (awp->afp != NULL) {
    DoImmediateFormat (awp->afp, bbp);
  }
}

NLM_EXTERN void AddSegmentBlock (
  Asn2gbWorkPtr awp,
  Boolean onePartOfSeg
)

{
  Char             acc [41];
  IntAsn2gbJobPtr  ajp;
  Asn2gbSectPtr    asp;
  BaseBlockPtr     bbp;
  Char             buf [32];
  GBSeqPtr         gbseq;
  StringItemPtr    ffstring;

  if (awp == NULL) return;
  ajp = awp->ajp;
  if (ajp == NULL) return;
  asp = awp->asp;
  if (asp == NULL) return;

  if (awp->seg < 1 || awp->numsegs < 1) return;

  bbp = Asn2gbAddBlock (awp, SEGMENT_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  ffstring = FFGetString(ajp);
  if ( ffstring == NULL ) return;


  FFStartPrint (ffstring, awp->format, 0, 12, "SEGMENT", 12, 5, 5, "XX", FALSE);

  if ( GetWWW(ajp) && awp->parent != NULL && onePartOfSeg) {
    sprintf (buf, "%d of ", (int) awp->seg);
    FFAddOneString (ffstring, buf, FALSE, FALSE, TILDE_IGNORE);
    SeqIdWrite (awp->parent->id, acc, PRINTID_TEXTID_ACC_VER, sizeof (acc) - 1);

    FFAddTextToString(ffstring, "<a href=", link_seq, NULL, FALSE, FALSE, TILDE_IGNORE);
    FFAddTextToString(ffstring, "val=", acc, ">", FALSE, FALSE, TILDE_IGNORE);

    sprintf (buf, "%ld", (long) awp->numsegs);
    FFAddOneString (ffstring, buf, FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString (ffstring, "</a>", FALSE, FALSE, TILDE_IGNORE);
  } else {
    sprintf (buf, "%d of %ld", (int) awp->seg, (long) awp->numsegs);
    FFAddOneString (ffstring, buf, FALSE, TRUE, TILDE_TO_SPACES);
  }

  /* optionally populate gbseq for XML-ized GenBank format */

  if (ajp->gbseq) {
    gbseq = &asp->gbseq;
  } else {
    gbseq = NULL;
  }

  if (gbseq != NULL) {
    sprintf (buf, "%d of %ld", (int) awp->seg, (long) awp->numsegs);
    gbseq->segment = StringSave (buf);
  }

  bbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 5, 5, "XX");
  FFRecycleString(ajp, ffstring);

  if (awp->afp != NULL) {
    DoImmediateFormat (awp->afp, bbp);
  }
}

NLM_EXTERN void AddSourceBlock (
  Asn2gbWorkPtr awp
)

{
  IntAsn2gbJobPtr    ajp;
  BaseBlockPtr       bbp;
  BioseqPtr          bsp;
  SeqFeatPtr         cds;
  SeqMgrDescContext  dcontext;
  BioseqPtr          dna;
  SeqMgrFeatContext  fcontext;
  GBBlockPtr         gbp;
  SeqDescrPtr        sdp;
  SeqFeatPtr         sfp;

  if (awp == NULL) return;
  ajp = awp->ajp;
  if (ajp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;

  bbp = Asn2gbAddBlock (awp, SOURCE_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_genbank, &dcontext);
  if (sdp != NULL && (! ajp->newSourceOrg)) {
    gbp = (GBBlockPtr) sdp->data.ptrvalue;
    if (gbp != NULL && (! StringHasNoText (gbp->source))) {
      bbp->entityID = dcontext.entityID;
      bbp->itemID = dcontext.itemID;
      bbp->itemtype = OBJ_SEQDESC;

      if (awp->afp != NULL) {
        DoImmediateFormat (awp->afp, bbp);
      }

      return;
    }
  }

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_source, &dcontext);
  if (sdp != NULL) {
    bbp->entityID = dcontext.entityID;
    bbp->itemID = dcontext.itemID;
    bbp->itemtype = OBJ_SEQDESC;
  } else {
    sfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_BIOSRC, 0, &fcontext);
    if (sfp != NULL) {
      bbp->entityID = fcontext.entityID;
      bbp->itemID = fcontext.itemID;
      bbp->itemtype = OBJ_SEQFEAT;
    } else if (ISA_aa (bsp->mol)) {

      /* if protein with no sources, get sources applicable to DNA location of CDS */

      cds = SeqMgrGetCDSgivenProduct (bsp, &fcontext);
      if (cds != NULL) {
        sfp = SeqMgrGetOverlappingSource (cds->location, &fcontext);
        if (sfp != NULL) {
          bbp->entityID = fcontext.entityID;
          bbp->itemID = fcontext.itemID;
          bbp->itemtype = OBJ_SEQFEAT;
        } else {
          dna = BioseqFindFromSeqLoc (cds->location);
          if (dna != NULL) {
            sdp = SeqMgrGetNextDescriptor (dna, NULL, Seq_descr_source, &dcontext);
            if (sdp != NULL) {
              bbp->entityID = dcontext.entityID;
              bbp->itemID = dcontext.itemID;
              bbp->itemtype = OBJ_SEQDESC;
            }
          }
        }
      }
    }
  }

  if (awp->afp != NULL) {
    DoImmediateFormat (awp->afp, bbp);
  }
}

NLM_EXTERN void AddOrganismBlock (
  Asn2gbWorkPtr awp
)

{
  BaseBlockPtr       bbp;
  BioseqPtr          bsp;
  SeqFeatPtr         cds;
  SeqMgrDescContext  dcontext;
  BioseqPtr          dna;
  SeqMgrFeatContext  fcontext;
  SeqDescrPtr        sdp;
  SeqFeatPtr         sfp;

  if (awp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;

  bbp = Asn2gbAddBlock (awp, ORGANISM_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_source, &dcontext);
  if (sdp != NULL) {
    bbp->entityID = dcontext.entityID;
    bbp->itemID = dcontext.itemID;
    bbp->itemtype = OBJ_SEQDESC;
  } else {
    sfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_BIOSRC, 0, &fcontext);
    if (sfp != NULL) {
      bbp->entityID = fcontext.entityID;
      bbp->itemID = fcontext.itemID;
      bbp->itemtype = OBJ_SEQFEAT;
    } else if (ISA_aa (bsp->mol)) {

      /* if protein with no sources, get sources applicable to DNA location of CDS */

      cds = SeqMgrGetCDSgivenProduct (bsp, &fcontext);
      if (cds != NULL) {
        sfp = SeqMgrGetOverlappingSource (cds->location, &fcontext);
        if (sfp != NULL) {
          bbp->entityID = fcontext.entityID;
          bbp->itemID = fcontext.itemID;
          bbp->itemtype = OBJ_SEQFEAT;
        } else {
          dna = BioseqFindFromSeqLoc (cds->location);
          if (dna != NULL) {
            sdp = SeqMgrGetNextDescriptor (dna, NULL, Seq_descr_source, &dcontext);
            if (sdp != NULL) {
              bbp->entityID = dcontext.entityID;
              bbp->itemID = dcontext.itemID;
              bbp->itemtype = OBJ_SEQDESC;
            }
          }
        }
      }
    }
  }

  if (awp->afp != NULL) {
    DoImmediateFormat (awp->afp, bbp);
  }
}

static RefBlockPtr AddPub (
  Asn2gbWorkPtr awp,
  ValNodePtr PNTR head,
  PubdescPtr pdp
)

{
  Char            buf [521]; /* increased for consortium in citsub */
  CitArtPtr       cap;
  CitBookPtr      cbp;
  CitGenPtr       cgp;
  CitJourPtr      cjp;
  CitPatPtr       cpp;
  CitSubPtr       csp;
  DatePtr         dp = NULL;
  Boolean         justuids = TRUE;
  ImprintPtr      imp = NULL;
  IntRefBlockPtr  irp;
  RefBlockPtr     rbp;
  ValNodePtr      vnp;
  ArticleIdPtr	  aip;

  if (awp == NULL || head == NULL || pdp == NULL) return NULL;

  if (awp->hideGeneRIFs) {
    if (StringISearch (pdp->comment, "GeneRIF") != NULL) return NULL;
  } else if (awp->onlyGeneRIFs) {
    if (StringISearch (pdp->comment, "GeneRIF") == NULL) return NULL;
  } else if (awp->onlyReviewPubs) {
    if (StringISearch (pdp->comment, "Review Article") == NULL) return NULL;
  }

  rbp = (RefBlockPtr) MemNew (sizeof (IntRefBlock));
  if (rbp == NULL) return NULL;
  rbp->blocktype = REFERENCE_BLOCK;
  rbp->section = awp->currsection;

  rbp->serial = INT2_MAX;

  for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
    switch (vnp->choice) {
      case PUB_Gen :
        /* may be unpublished, or may be serial number of swiss-prot reference */
        cgp = (CitGenPtr) vnp->data.ptrvalue;
        if (cgp != NULL) {
          if (StringNICmp ("BackBone id_pub", cgp->cit, 15) != 0) {
            rbp->category = REF_CAT_UNP;
            if (dp == NULL) {
              dp = cgp->date;
            }
            if (cgp->serial_number > 0) {
              rbp->serial = cgp->serial_number;
            }
            if (cgp->cit != NULL) {
              if (StringNICmp ("unpublished", cgp->cit, 11) != 0 &&
                  StringNICmp ("submitted", cgp->cit, 8) != 0 &&
                  StringNICmp ("to be published", cgp->cit, 15) != 0 &&
                  StringNICmp ("in press", cgp->cit, 8) != 0 &&
                  StringStr (cgp->cit, "Journal") == NULL) {
                if (cgp->serial_number == 0) {
                  MemFree (rbp);
                  return NULL;
                }
              }
            } else if (cgp->journal == NULL || cgp->date == NULL) {
              if (cgp->serial_number == 0) {
                MemFree (rbp);
                return NULL;
              }
            }
          }
        }
        break;
      case PUB_Sub :
        rbp->category = REF_CAT_SUB;
        csp = (CitSubPtr) vnp->data.ptrvalue;
        if (csp != NULL) {
          imp = csp->imp;
          if (imp != NULL) {
            if (dp == NULL) {
              dp = imp->date;
            }
          }
          if (csp->date != NULL) {
            if (dp == NULL) {
              dp = csp->date;
            }
          }
        }
        break;
      case PUB_Article:
        cap = (CitArtPtr) vnp->data.ptrvalue;
        if (cap != NULL) {
          switch (cap->from) {
            case 1:
              cjp = (CitJourPtr) cap->fromptr;
              if (cjp != NULL) {
                imp = (ImprintPtr) cjp->imp;
                if (imp != NULL) {
                  if (dp == NULL) {
                    dp = imp->date;
                  }
                }
              }
              break;
            case 2:
              cbp = (CitBookPtr) cap->fromptr;
              if (cbp != NULL) {
                imp = (ImprintPtr) cbp->imp;
                if (imp != NULL) {
                  if (dp == NULL) {
                    dp = imp->date;
                  }
                }
              }
              break;
            case 3:
              cbp = (CitBookPtr) cap->fromptr;
              if (cbp != NULL) {
                imp = (ImprintPtr) cbp->imp;
                if (imp != NULL) {
                  if (dp == NULL) {
                    dp = imp->date;
                  }
                }
              }
              break;
            default:
              break;
          }
		  /*  look for PMID and MUID in the Cit-art article ids set */
		  if (cap->ids != NULL) {
			for (aip = cap->ids; aip != NULL; aip = aip->next) {
              if (aip->choice == ARTICLEID_PUBMED && rbp->pmid == 0) {
                rbp->pmid = aip->data.intvalue;
                rbp->category = REF_CAT_PUB;
              } else if (aip->choice == ARTICLEID_MEDLINE && rbp->muid == 0) {
                rbp->muid = aip->data.intvalue;
                rbp->category = REF_CAT_PUB;
              }
			}
		  }
        }
        break;
      case PUB_Book:
        cbp = (CitBookPtr) vnp->data.ptrvalue;
        if (cbp != NULL) {
          imp = (ImprintPtr) cbp->imp;
          if (imp != NULL) {
            if (dp == NULL) {
              dp = imp->date;
            }
          }
        }
        break;
      case PUB_Proc:
        cbp = (CitBookPtr) vnp->data.ptrvalue;
        if (cbp != NULL) {
          imp = (ImprintPtr) cbp->imp;
          if (imp != NULL) {
            if (dp == NULL) {
              dp = imp->date;
            }
          }
        }
        break;
      case PUB_Patent :
        rbp->category = REF_CAT_PUB;
        cpp = (CitPatPtr) vnp->data.ptrvalue;
        if (cpp != NULL) {
          if (cpp->date_issue != NULL) {
            if (dp == NULL) {
              dp = (DatePtr) cpp->date_issue;
            }
          } else if (cpp->app_date != NULL) {
            if (dp == NULL) {
              dp = (DatePtr) cpp->app_date;
            }
          }
        }
        break;
      case PUB_Man:
        cbp = (CitBookPtr) vnp->data.ptrvalue;
        if (cbp != NULL) {
          imp = (ImprintPtr) cbp->imp;
          if (imp != NULL) {
            if (dp == NULL) {
              dp = imp->date;
            }
          }
        }
        break;
      case PUB_Muid :
        if (rbp->muid == 0) {
          rbp->muid = vnp->data.intvalue;
          rbp->category = REF_CAT_PUB;
        }
        break;
      case PUB_PMid :
        if (rbp->pmid == 0) {
          rbp->pmid = vnp->data.intvalue;
          rbp->category = REF_CAT_PUB;
        }
        break;
      default :
        break;
    }
    if (vnp->choice != PUB_Muid && vnp->choice != PUB_PMid) {
      justuids = FALSE;
    }
  }

  /* check for submitted vs. in-press */

  if (imp != NULL) {
    rbp->category = REF_CAT_PUB;
    switch (imp->prepub) {
      case 1 :
        rbp->category = REF_CAT_UNP;
        break;
      case 2 :
        rbp->category = REF_CAT_PUB;
        break;
      default :
        break;
    }
  }

  /* check for sites reftype */

  if (pdp->reftype != 0) {
    rbp->sites = pdp->reftype;
  }

  if (rbp->muid == 0 && rbp->pmid == 0) {
    vnp = pdp->pub;

    /* skip over just serial number */

    if (vnp != NULL && vnp->choice == PUB_Gen && vnp->next != NULL) {
      cgp = (CitGenPtr) vnp->data.ptrvalue;
      if (cgp != NULL) {
        if (StringNICmp ("BackBone id_pub", cgp->cit, 15) != 0) {
          if (cgp->cit == NULL && cgp->journal == NULL && cgp->date == NULL && cgp->serial_number) {
            vnp = vnp->next;
          }
        }
      }
    }

    if (PubLabelUnique (vnp, buf, sizeof (buf) - 1, OM_LABEL_CONTENT, TRUE) > 0) {
      rbp->uniquestr = StringSaveNoNull (buf);
    }
  }

  irp = (IntRefBlockPtr) rbp;
  irp->date = DateDup (dp);
  irp->justuids = justuids;
  /* if (justuids) { */
    irp->fig = StringSaveNoNull (pdp->fig);
    irp->maploc = StringSaveNoNull (pdp->maploc);
    irp->poly_a = pdp->poly_a;
  /* } */

  /* if not rejected by now, link in */

  ValNodeAddPointer (head, 0, rbp);

  return rbp;
}

static int LIBCALLBACK SortReferences (
  VoidPtr ptr1,
  VoidPtr ptr2,
  Boolean serialFirst,
  Boolean isRefSeq
)

{
  int             compare;
  IntRefBlockPtr  irp1;
  IntRefBlockPtr  irp2;
  RefBlockPtr     rbp1;
  RefBlockPtr     rbp2;
  Int2            status;
  RefBlockPtr     temp;
  ValNodePtr      vnp1;
  ValNodePtr      vnp2;

  if (ptr1 == NULL || ptr2 == NULL) return 0;
  vnp1 = *((ValNodePtr PNTR) ptr1);
  vnp2 = *((ValNodePtr PNTR) ptr2);
  if (vnp1 == NULL || vnp2 == NULL) return 0;
  rbp1 = (RefBlockPtr) vnp1->data.ptrvalue;
  rbp2 = (RefBlockPtr) vnp2->data.ptrvalue;
  if (rbp1 == NULL || rbp2 == NULL) return 0;

  if (serialFirst) {
    if (rbp1->serial > rbp2->serial) {
      return 1;
    } else if (rbp1->serial < rbp2->serial) {
      return -1;
    }
  }

  /* usual first sort by published, unpublished, and cit-subs */

  if (rbp1->category > rbp2->category) {
    return 1;
  } else if (rbp1->category < rbp2->category) {
    return -1;
  }

  /* for RefSeq, newer publications first, so temporarily swap pointers */

  if (isRefSeq) {
    temp = rbp1;
    rbp1 = rbp2;
    rbp2 = temp;
  }

  /* within class, sort by date, older publications first (except RefSeq) */

  irp1 = (IntRefBlockPtr) rbp1;
  irp2 = (IntRefBlockPtr) rbp2;

  if ( irp1->date != 0  &&  irp2->date == 0 ) {
      return 1;
  } else if ( irp1->date == 0  &&  irp2->date != 0 ) {
      return -1;
  }

  status = DateMatch (irp1->date, irp2->date, TRUE);
  if (status == 1 || status == -1) return status;

  /* if dates (e.g., years) match, try to distinguish by uids */

  if (rbp1->pmid != 0 && rbp2->pmid != 0) {
    if (rbp1->pmid > rbp2->pmid) {
      return 1;
    } else if (rbp1->pmid < rbp2->pmid) {
      return -1;
    }
  }

  if (rbp1->muid != 0 && rbp2->muid != 0) {
    if (rbp1->muid > rbp2->muid) {
      return 1;
    } else if (rbp1->muid < rbp2->muid) {
      return -1;
    }
  }

  /* restore sort order after date and pmid/muid matching */

  if (isRefSeq) {
    temp = rbp1;
    rbp1 = rbp2;
    rbp2 = temp;

    irp1 = (IntRefBlockPtr) rbp1;
    irp2 = (IntRefBlockPtr) rbp2;
  }

  /* if same uid, one with just uids goes last to be excised but remembered */

  if ((rbp1->pmid != 0 && rbp2->pmid != 0) || (rbp1->muid != 0 && rbp2->muid != 0)) {
    if (irp1->justuids && (! irp2->justuids)) {
      return 1;
    } else if ((! irp1->justuids) && irp2->justuids) {
      return -1;
    }
  }

  /* put sites after pubs that refer to all or a range of bases */

  if (rbp1->sites > rbp2->sites) {
    return 1;
  } else if (rbp2->sites > rbp1->sites) {
    return -1;
  }

  /* for publication features, sort in explore index order */

  if (irp1->index > irp2->index) {
    return 1;
  } else if (irp1->index < irp2->index) {
    return -1;
  }

  /* next use author string */

  if (irp1->authstr != NULL && irp2->authstr != NULL) {
    compare = StringICmp (irp1->authstr, irp2->authstr);
    if (compare > 0) {
      return 1;
    } else if (compare < 0) {
      return -1;
    }
  }

  /* use unique label string to determine sort order */

  if (rbp1->uniquestr != NULL && rbp2->uniquestr != NULL) {
    compare = StringICmp (rbp1->uniquestr, rbp2->uniquestr);
    if (compare > 0) {
      return 1;
    } else if (compare < 0) {
      return -1;
    }
  }

  /* last resort for equivalent publication descriptors, sort in itemID order */

  if (rbp1->itemtype == OBJ_SEQDESC && rbp2->itemtype == OBJ_SEQDESC) {
    if (rbp1->itemID > rbp2->itemID) {
      return 1;
    } else if (rbp1->itemID < rbp2->itemID) {
      return -1;
    }
  }

  if (rbp1->itemtype == OBJ_ANNOTDESC && rbp2->itemtype == OBJ_ANNOTDESC) {
    if (rbp1->itemID > rbp2->itemID) {
      return 1;
    } else if (rbp1->itemID < rbp2->itemID) {
      return -1;
    }
  }

  if (! serialFirst) {
    if (rbp1->serial > rbp2->serial) {
      return 1;
    } else if (rbp1->serial < rbp2->serial) {
      return -1;
    }
  }

  return 0;
}

static int LIBCALLBACK SortReferencesA (
  VoidPtr ptr1,
  VoidPtr ptr2
)

{
  return SortReferences (ptr1, ptr2, FALSE, FALSE);
}

static int LIBCALLBACK SortReferencesB (
  VoidPtr ptr1,
  VoidPtr ptr2
)

{
  return SortReferences (ptr1, ptr2, TRUE, FALSE);
}

static int LIBCALLBACK SortReferencesAR (
  VoidPtr ptr1,
  VoidPtr ptr2
)

{
  return SortReferences (ptr1, ptr2, FALSE, TRUE);
}

static int LIBCALLBACK SortReferencesBR (
  VoidPtr ptr1,
  VoidPtr ptr2
)

{
  return SortReferences (ptr1, ptr2, TRUE, TRUE);
}

static CharPtr GetAuthorsPlusConsortium (
  FmtType format,
  AuthListPtr alp
)

{
  CharPtr  consortium;
  CharPtr  str;
  CharPtr  tmp;

  consortium = NULL;
  str = GetAuthorsString (format, alp, &consortium, NULL, NULL);
  if (str == NULL) return consortium;
  if (consortium == NULL) return str;
  tmp = MemNew (StringLen (str) + StringLen (consortium) + 5);
  if (tmp == NULL) return NULL;
  StringCpy (tmp, str);
  StringCat (tmp, "; ");
  StringCat (tmp, consortium);
  MemFree (str);
  MemFree (consortium);
  return tmp;
}

static Boolean HasNoPmidOrMuid (
  PubdescPtr pdp
)

{
  ValNodePtr  vnp;

  if (pdp == NULL) return TRUE;
  for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == PUB_PMid || vnp->choice == PUB_Muid) return FALSE;
  }
  return TRUE;
}

typedef struct cdspubs {
  Asn2gbWorkPtr  awp;
  BioseqPtr      target;
  ValNodePtr     vnp;
} CdsPubs, PNTR CdsPubsPtr;

static Boolean LIBCALLBACK GetRefsOnCDS (
  SeqFeatPtr sfp,
  SeqMgrFeatContextPtr context
)

{
  AuthListPtr     alp;
  Asn2gbWorkPtr   awp;
  CdsPubsPtr      cpp;
  IntRefBlockPtr  irp;
  Boolean         okay;
  PubdescPtr      pdp;
  RefBlockPtr     rbp;

  if (sfp == NULL || context == NULL) return TRUE;
  cpp = (CdsPubsPtr) context->userdata;
  awp = cpp->awp;
  if (awp == NULL) return TRUE;

  okay = TRUE;
  pdp = (PubdescPtr) sfp->data.value.ptrvalue;
  if (awp->format == FTABLE_FMT) {
    if (HasNoPmidOrMuid (pdp)) {
      okay = FALSE;
    }
  }

  if (okay) {
    rbp = AddPub (awp, &(awp->pubhead), pdp);
    if (rbp != NULL) {

      rbp->entityID = context->entityID;
      rbp->itemID = context->itemID;
      rbp->itemtype = OBJ_SEQFEAT;

      irp = (IntRefBlockPtr) rbp;
      irp->loc = SeqLocMerge (cpp->target, cpp->vnp, NULL, FALSE, TRUE, FALSE);
      alp = GetAuthListPtr (pdp, NULL);
      if (alp != NULL) {
        irp->authstr = GetAuthorsPlusConsortium (awp->format, alp);
      }
      irp->index = 0;
    }
  }

  return TRUE;
}

static void GetRefsOnBioseq (
  Asn2gbWorkPtr awp,
  BioseqPtr target,
  BioseqPtr bsp,
  Int4 from,
  Int4 to,
  SeqLocPtr cdsloc
)

{
  SeqMgrAndContext   acontext;
  AnnotDescPtr       adp;
  IntAsn2gbJobPtr    ajp;
  AuthListPtr        alp;
  CdsPubs            cp;
  SeqMgrDescContext  dcontext;
  SeqMgrFeatContext  fcontext;
  Int2               i;
  Int2               idx;
  IntRefBlockPtr     irp;
  Int4Ptr            ivals;
  Int4               left;
  SeqLocPtr          newloc;
  Int2               numivals;
  Boolean            okay;
  PubdescPtr         pdp;
  RefBlockPtr        rbp;
  Int4               right;
  SeqDescrPtr        sdp;
  SeqFeatPtr         sfp;
  SeqInt             sint;
  SeqIdPtr           sip;
  Boolean            split;
  Int4               start;
  Int4               stop;
  Uint1              strand;
  Boolean            takeIt;
  ValNode            vn;
  ValNodePtr         vnp;

  if (awp == NULL || target == NULL || bsp == NULL) return;
  ajp = awp->ajp;
  if (ajp == NULL) return;

  /* full length loc for descriptors */

  sint.from = 0;
  if (ajp->ajp.slp != NULL) {
    from = SeqLocStart (ajp->ajp.slp); /* other features use awp->slp for from and to */
  }
  if (ajp->ajp.slp != NULL) {
    sint.to = SeqLocLen (ajp->ajp.slp) - 1;
    to = SeqLocStop (ajp->ajp.slp); /* other features use awp->slp for from and to */
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

  sdp = SeqMgrGetNextDescriptor (target, NULL, Seq_descr_pub, &dcontext);
  while (sdp != NULL) {

    /* check if descriptor on part already added on segmented bioseq */

    okay = TRUE;
    for (vnp = awp->pubhead; vnp != NULL && okay; vnp = vnp->next) {
      rbp = (RefBlockPtr) vnp->data.ptrvalue;
      if (rbp != NULL) {
        if (rbp->entityID == dcontext.entityID &&
            rbp->itemID == dcontext.itemID &&
            rbp->itemtype == OBJ_SEQDESC) {
          okay = FALSE;
        }
      }
    }
    if (awp->format == FTABLE_FMT) {
      pdp = (PubdescPtr) sdp->data.ptrvalue;
      if (HasNoPmidOrMuid (pdp)) {
        okay = FALSE;
      }
    }

    if (okay) {
      pdp = (PubdescPtr) sdp->data.ptrvalue;
      rbp = AddPub (awp, &(awp->pubhead), pdp);
      if (rbp != NULL) {

        rbp->entityID = dcontext.entityID;
        rbp->itemID = dcontext.itemID;
        rbp->itemtype = OBJ_SEQDESC;

        irp = (IntRefBlockPtr) rbp;
        irp->loc = SeqLocMerge (target, &vn, NULL, FALSE, TRUE, FALSE);
        alp = GetAuthListPtr (pdp, NULL);
        if (alp != NULL) {
          irp->authstr = GetAuthorsPlusConsortium (awp->format, alp);
        }
        irp->index = 0;
      }
    }
    sdp = SeqMgrGetNextDescriptor (target, sdp, Seq_descr_pub, &dcontext);
  }

  /* if protein with no pubs, get pubs applicable to DNA location of CDS */

  if (cdsloc != NULL) {
    cp.awp = awp;
    cp.target = target;
    cp.vnp = &vn;
    SeqMgrGetAllOverlappingFeatures (cdsloc, FEATDEF_PUB, NULL, 0, LOCATION_SUBSET, (Pointer) &cp, GetRefsOnCDS);
  }

  /* also get publications from AnnotDesc on SeqAnnot */

  adp = SeqMgrGetNextAnnotDesc (target, NULL, Annot_descr_pub, &acontext);
  while (adp != NULL) {

    okay = TRUE;

    if (okay) {
      pdp = (PubdescPtr) adp->data.ptrvalue;
      rbp = AddPub (awp, &(awp->pubhead), pdp);
      if (rbp != NULL) {

        rbp->entityID = acontext.entityID;
        rbp->itemID = acontext.itemID;
        rbp->itemtype = OBJ_ANNOTDESC;

        irp = (IntRefBlockPtr) rbp;
        irp->loc = SeqLocMerge (target, &vn, NULL, FALSE, TRUE, FALSE);
        alp = GetAuthListPtr (pdp, NULL);
        if (alp != NULL) {
          irp->authstr = GetAuthorsPlusConsortium (awp->format, alp);
        }
        irp->index = 0;
      }
    }
    adp = SeqMgrGetNextAnnotDesc (target, adp, Annot_descr_pub, &acontext);
  }

  SeqIdFree (sint.id);

  /* features are indexed on parent if segmented */

  bsp = awp->parent;

  sfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_PUB, 0, &fcontext);
  while (sfp != NULL) {
    ivals = fcontext.ivals;
    numivals = fcontext.numivals;
    if (ivals != NULL && numivals > 0) {

      /*
      idx = (numivals - 1) * 2;
      start = ivals [idx];
      stop = ivals [idx + 1];
      */

      takeIt = FALSE;
      for (i = 0, idx = 0; i < numivals; i++, idx += 2) {
        start = ivals [idx];
        stop = ivals [idx + 1];
        if ((start <= from && stop > from) ||
            (start < to && stop >= to) ||
            (start >= from && stop <= to)) {
          takeIt = TRUE;
        }
      }
      if (awp->format == FTABLE_FMT) {
        pdp = (PubdescPtr) sdp->data.ptrvalue;
        if (HasNoPmidOrMuid (pdp)) {
          takeIt = FALSE;
        }
      }

      if (takeIt /* stop >= from && stop <= to */) {

        /*
        start = ivals [0] + 1;
        stop = ivals [idx + 1] + 1;
        */
        pdp = (PubdescPtr) sfp->data.value.ptrvalue;
        rbp = AddPub (awp, &(awp->pubhead), pdp);
        if (rbp != NULL) {

          rbp->entityID = fcontext.entityID;
          rbp->itemID = fcontext.itemID;
          rbp->itemtype = OBJ_SEQFEAT;

          irp = (IntRefBlockPtr) rbp;
          irp->loc = SeqLocMerge (target, sfp->location, NULL, FALSE, TRUE, FALSE);
          if (ajp->ajp.slp != NULL) {
            sip = SeqIdParse ("lcl|dummy");
            left = GetOffsetInBioseq (ajp->ajp.slp, bsp, SEQLOC_LEFT_END);
            right = GetOffsetInBioseq (ajp->ajp.slp, bsp, SEQLOC_RIGHT_END);
            strand = SeqLocStrand (ajp->ajp.slp);
            split = FALSE;
            newloc = SeqLocReMapEx (sip, ajp->ajp.slp, irp->loc, 0, FALSE, ajp->masterStyle);
            /*
            newloc = SeqLocCopyRegion (sip, irp->loc, bsp, left, right, strand, &split);
            */
            SeqIdFree (sip);
            if (newloc != NULL) {
              A2GBSeqLocReplaceID (newloc, ajp->ajp.slp);
              irp->loc = SeqLocFree (irp->loc);
              irp->loc = newloc;
            }
          }
          alp = GetAuthListPtr (pdp, NULL);
          if (alp != NULL) {
            irp->authstr = GetAuthorsPlusConsortium (awp->format, alp);
          }
          irp->index = fcontext.index;
        }
      }
    }

    sfp = SeqMgrGetNextFeature (bsp, sfp, SEQFEAT_PUB, 0, &fcontext);
  }
}

static Boolean LIBCALLBACK GetRefsOnSeg (
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

  /* reference descriptors only on parts within entity */

  sep = GetTopSeqEntryForEntityID (awp->entityID);
  oldscope = SeqEntrySetScope (sep);
  bsp = BioseqFind (sip);
  SeqEntrySetScope (oldscope);

  if (bsp != NULL) {
    GetRefsOnBioseq (awp, awp->refs, bsp, from, to, NULL);
    return TRUE;
  }

  /* if we ever want to fetch remote references, code goes here */

  return TRUE;
}

NLM_EXTERN Boolean AddReferenceBlock (
  Asn2gbWorkPtr awp,
  Boolean isRefSeq
)

{
  IntAsn2gbJobPtr    ajp;
  AuthListPtr        alp;
  Asn2gbSectPtr      asp;
  BioseqPtr          bsp;
  SeqFeatPtr         cds;
  Boolean            combine;
  SeqMgrFeatContext  context;
  CitSubPtr          csp;
  BioseqPtr          dna;
  Boolean            excise;
  Int2               firstserial;
  ValNodePtr         head = NULL;
  Int2               i;
  IntRefBlockPtr     irp;
  Boolean            is_aa;
  Boolean            is_embl = FALSE;
  Boolean            is_patent = FALSE;
  Int2               j;
  IntRefBlockPtr     lastirp;
  RefBlockPtr        lastrbp;
  ValNodePtr         next;
  Int2               numReferences;
  ValNodePtr         PNTR prev;
  RefBlockPtr        rbp;
  RefBlockPtr        PNTR referenceArray;
  BioseqPtr          refs;
  SubmitBlockPtr     sbp;
  SeqIdPtr           sip;
  SeqLocPtr          slp;
  BioseqPtr          target;
  ValNodePtr         vnp;

  if (awp == NULL) return FALSE;
  ajp = awp->ajp;
  if (ajp == NULL) return FALSE;
  asp = awp->asp;
  if (asp == NULL) return FALSE;
  bsp = awp->bsp;
  refs = awp->refs;
  if (bsp == NULL || refs == NULL) return FALSE;

  /* collect publications on bioseq */

  awp->pubhead = NULL;
  GetRefsOnBioseq (awp, bsp, refs, awp->from, awp->to, NULL);
  target = bsp;

  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_EMBL) {
      is_embl = TRUE;
    } else if (sip->choice == SEQID_PATENT) {
      is_patent = TRUE;
    }
  }

  is_aa = (Boolean) ISA_aa (bsp->mol);

  if (bsp->repr == Seq_repr_seg) {

    /* collect publication descriptors on local parts */

    SeqMgrExploreSegments (bsp, (Pointer) awp, GetRefsOnSeg);
    target = awp->refs;
  }

  if (awp->pubhead == NULL && ISA_aa (bsp->mol)) {

    /* if protein with no pubs, get pubs applicable to DNA location of CDS */

    cds = SeqMgrGetCDSgivenProduct (bsp, &context);
    if (cds != NULL) {
      dna = BioseqFindFromSeqLoc (cds->location);
      if (dna != NULL) {
        GetRefsOnBioseq (awp, dna, dna, context.left, context.right, cds->location);
        target = dna;
      }
    }
  }

  head = awp->pubhead;
  awp->pubhead = NULL;

  if (head == NULL && awp->ssp == NULL) return FALSE;

  /* sort by pub/unpub/sites/sub, then date, finally existing serial */

  if (isRefSeq) {
    head = SortValNode (head, SortReferencesAR);
  } else {
    head = SortValNode (head, SortReferencesA);
  }

  if (awp->ssp != NULL) {

    /* add seq-submit citation */

    rbp = (RefBlockPtr) MemNew (sizeof (IntRefBlock));
    if (rbp != NULL) {
      irp = (IntRefBlockPtr) rbp;

      rbp->blocktype = REFERENCE_BLOCK;
      rbp->section = awp->currsection;
      rbp->serial = INT2_MAX;
      rbp->category = REF_CAT_SUB;

      rbp->entityID = ajp->ajp.entityID;
      rbp->itemID = 1;
      rbp->itemtype = OBJ_SEQSUB_CIT;

      sbp = awp->ssp->sub;
      if (sbp != NULL) {
        csp = sbp->cit;
        if (csp != NULL) {
          alp = GetAuthListPtr (NULL, csp);
          if (alp != NULL) {
            irp->authstr = GetAuthorsPlusConsortium (awp->format, alp);
          }
          if (csp->date != NULL) {
            irp->date = DateDup (csp->date);
          }
        }
      }

      if (awp->citSubsFirst) {

        /* for DDBJ, add seq-submit citation to beginning of list */

        vnp = ValNodeNew (NULL);
        if (vnp != NULL) {
          vnp->choice = 0;
          vnp->data.ptrvalue = (VoidPtr) rbp;
          vnp->next = head;
          head = vnp;
        }

      } else {

        /* for GENBANK and EMBL add seq-submit citation to end of list */

        ValNodeAddPointer (&head, 0, rbp);
      }
    }
  }

  /* unique references, excise duplicates from list */

  prev = &(head);
  vnp = head;
  lastrbp = NULL;
  while (vnp != NULL) {
    excise = FALSE;
    combine = TRUE;
    next = vnp->next;
    rbp = (RefBlockPtr) vnp->data.ptrvalue;
    if (lastrbp != NULL) {
      lastirp = (IntRefBlockPtr) lastrbp;
      if (rbp != NULL) {
        irp = (IntRefBlockPtr) rbp;
        if (lastrbp->pmid != 0 && rbp->pmid != 0) {
          if (lastrbp->pmid == rbp->pmid) {
            excise = TRUE;
          }
        } else if (lastrbp->muid != 0 && rbp->muid != 0) {
          if (lastrbp->muid == rbp->muid) {
            excise = TRUE;
          }
        } else if (lastrbp->uniquestr != NULL && rbp->uniquestr != NULL) {
          if (StringICmp (lastrbp->uniquestr, rbp->uniquestr) == 0) {
            if (SeqLocCompare (irp->loc, lastirp->loc) == SLC_A_EQ_B) {
              if (StringICmp (irp->authstr, lastirp->authstr) == 0) {

                /* L76496.1 - removing duplicate submission pubs */
                excise = TRUE;
              }
            }
          }
        }
        if (excise && lastrbp->sites == 0 && rbp->sites > 0) {
          /* real range trumps sites */
          combine = FALSE;
        }
      }
    }
    if (rbp != NULL) {
      irp = (IntRefBlockPtr) rbp;
      if (irp->justuids) {
        if (isRefSeq && is_aa) {
          /* if allowing justuid in protein RefSeq, try to look up dynamically */
          excise = TRUE; /* Back to old behavior, do not fetch */
        } else {
          /* do not allow justuids reference to appear by itself - S79174.1 */
          excise = TRUE;
          /* justuids should still combine, even if no authors - S67070.1 */
        }
      } else if (is_embl && is_patent) {
        /* EMBL patent records do not need author or title - A29528.1 */
      } else if (StringHasNoText (irp->authstr)) {
        /* do not allow no author reference to appear by itself - U07000.1 */
        excise = TRUE;
        combine = FALSE;
      } else if (isRefSeq && is_aa && rbp->category == REF_CAT_SUB) {
        /* GenPept RefSeq suppresses cit-subs */
        excise = TRUE;
        combine = FALSE;
      }
    }
    if (excise) {
      *prev = vnp->next;
      vnp->next = NULL;

      /* combine locations of duplicate references */

      irp = (IntRefBlockPtr) rbp;
      lastirp = (IntRefBlockPtr) lastrbp;
      if (combine) {
        if (lastirp != NULL) {
          slp = SeqLocMerge (target, lastirp->loc, irp->loc, FALSE, TRUE, FALSE);
          lastirp->loc = SeqLocFree (lastirp->loc);
          lastirp->loc = slp;
        }
        if (irp != NULL && lastirp != NULL) {
          if ((rbp->muid == lastrbp->muid && rbp->muid != 0) ||
              (rbp->pmid == lastrbp->pmid && rbp->pmid != 0)) {
            if (lastirp->fig == NULL) {
              lastirp->fig = StringSaveNoNull (irp->fig);
            }
            if (lastirp->maploc == NULL) {
              lastirp->maploc = StringSaveNoNull (irp->maploc);
            }
            lastirp->poly_a = irp->poly_a;
          }
        }
      }

      /* and remove duplicate reference */

      MemFree (rbp->uniquestr);
      DateFree (irp->date);
      SeqLocFree (irp->loc);
      MemFree (irp->authstr);
      MemFree (irp->fig);
      MemFree (irp->maploc);
      MemFree (rbp);
      ValNodeFree (vnp);

    } else {

      prev = &(vnp->next);
      lastrbp = rbp;
    }
    vnp = next;
  }

  /* resort by existing serial, then pub/unpub/sites/sub, then date */

  if (isRefSeq) {
    head = SortValNode (head, SortReferencesBR);
  } else {
    head = SortValNode (head, SortReferencesB);
  }

  if (head == NULL) return FALSE;

  /* if taking newest publications, free remainder */

  if (awp->newestPubs) {
    for (vnp = head, i = 1; vnp != NULL && i < 5; vnp = vnp->next, i++) continue;
    if (vnp != NULL) {
      next = vnp->next;
      vnp->next = NULL;
      for (vnp = next; vnp != NULL; vnp = vnp->next) {
        rbp = (RefBlockPtr) vnp->data.ptrvalue;
        MemFree (rbp->uniquestr);
        irp = (IntRefBlockPtr) rbp;
        DateFree (irp->date);
        SeqLocFree (irp->loc);
        MemFree (irp->authstr);
        MemFree (irp->fig);
        MemFree (irp->maploc);
        MemFree (rbp);
      }
    }

  /* if taking oldest publications, free remainder */

  } else if (awp->oldestPubs) {
    for (vnp = head, j = 0; vnp != NULL; vnp = vnp->next, j++) continue;
    if (j > 5) {
      for (vnp = head, i = 0; vnp != NULL && i < j - 6; vnp = vnp->next, i++) continue;
      if (vnp != NULL) {
        next = vnp->next;
        vnp->next = NULL;
        for (vnp = head; vnp != NULL; vnp = vnp->next) {
          rbp = (RefBlockPtr) vnp->data.ptrvalue;
          MemFree (rbp->uniquestr);
          irp = (IntRefBlockPtr) rbp;
          DateFree (irp->date);
          SeqLocFree (irp->loc);
          MemFree (irp->authstr);
          MemFree (irp->fig);
          MemFree (irp->maploc);
          MemFree (rbp);
        }
        head = next;
      }
    }
  }

  /* assign serial numbers */

  firstserial = 1;

  /* first find highest one assigned by EMBL/SWISS-PROT */

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    rbp = (RefBlockPtr) vnp->data.ptrvalue;
    if (rbp == NULL) continue;
    if (rbp->serial > 0 && rbp->serial < INT2_MAX) {
      firstserial = rbp->serial + 1;
    }
  }

  /* then give increasing serial numbers to unassigned publications */

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    rbp = (RefBlockPtr) vnp->data.ptrvalue;
    if (rbp == NULL) continue;
    if (rbp->serial > 0 && rbp->serial < INT2_MAX) continue;
    rbp->serial = firstserial;
    firstserial++;
  }

  /* allocate reference array for this section */

  numReferences = ValNodeLen (head);
  asp->numReferences = numReferences;

  if (numReferences > 0) {
    referenceArray = (RefBlockPtr PNTR) MemNew (sizeof (RefBlockPtr) * (numReferences + 1));
    asp->referenceArray = referenceArray;

    if (referenceArray != NULL) {

      /* fill in reference array */

      for (vnp = head, i = 0; vnp != NULL && i < numReferences; vnp = vnp->next, i++) {
        referenceArray [i] = (RefBlockPtr) vnp->data.ptrvalue;
      }
    }
  }

  /* finally link into blocks for current section */

  ValNodeLink (&(awp->lastblock), head);
  vnp = awp->lastblock;
  if (vnp == NULL) return FALSE;
  while (vnp->next != NULL) {
    vnp = vnp->next;
  }

  awp->lastblock = vnp;
  if (awp->blockList == NULL) {
    awp->blockList = vnp;
  }

  if (awp->afp != NULL) {
    for (vnp = head; vnp != NULL; vnp = vnp->next) {
      rbp = (RefBlockPtr) vnp->data.ptrvalue;
      if (rbp == NULL) continue;
      DoImmediateFormat (awp->afp, (BaseBlockPtr) rbp);
    }
  }

  return TRUE;
}

NLM_EXTERN void AddWGSBlock (
  Asn2gbWorkPtr awp
)

{
  IntAsn2gbJobPtr    ajp;
  BaseBlockPtr       bbp;
  BioseqPtr          bsp;
  Char               buf [80];
  SeqMgrDescContext  dcontext;
  CharPtr            first;
  CharPtr            last;
  ObjectIdPtr        oip;
  SeqDescrPtr        sdp;
  UserFieldPtr       ufp;
  UserObjectPtr      uop;
  Int2               wgstype;
  StringItemPtr      ffstring;

  if (awp == NULL) return;
  ajp = awp->ajp;
  if ( ajp == NULL ) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;

  if (awp->format == EMBL_FMT || awp->format == EMBLPEPT_FMT) return;

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_user, &dcontext);
  while (sdp != NULL) {
    uop = (UserObjectPtr) sdp->data.ptrvalue;
    if (uop != NULL) {
      oip = uop->type;
      first = NULL;
      last = NULL;
      wgstype = 0;
      if (oip != NULL) {
        if (StringICmp (oip->str, "WGSProjects") == 0) {
          wgstype = 1;
        } else if (StringICmp (oip->str, "WGS-Scaffold-List") == 0) {
          wgstype = 2;
        } else if (StringICmp (oip->str, "WGS-Contig-List") == 0) {
          wgstype = 3;
        }
        if (wgstype != 0) {
          for (ufp = uop->data; ufp != NULL; ufp = ufp->next) {
            oip = ufp->label;
            if (oip == NULL || oip->str == NULL || ufp->choice != 1) continue;
            if (StringICmp (oip->str, "WGS_accession_first") == 0) {
              first = (CharPtr) ufp->data.ptrvalue;
            } else if (StringICmp (oip->str, "WGS_accession_last") == 0) {
              last = (CharPtr) ufp->data.ptrvalue;
            } else if (StringICmp (oip->str, "Accession_first") == 0) {
              first = (CharPtr) ufp->data.ptrvalue;
            } else if (StringICmp (oip->str, "Accession_last") == 0) {
              last = (CharPtr) ufp->data.ptrvalue;
            }
          }
          if (first != NULL && last != NULL) {
            bbp = Asn2gbAddBlock (awp, WGS_BLOCK, sizeof (BaseBlock));
            if (bbp != NULL) {
              ffstring = FFGetString (ajp);
              if (ffstring != NULL) {
                if (wgstype == 1) {
                  FFStartPrint (ffstring, awp->format, 0, 12, "WGS", 12, 0, 0, NULL, FALSE);
                } else if (wgstype == 2) {
                  FFStartPrint (ffstring, awp->format, 0, 12, "WGS_SCAFLD", 12, 0, 0, NULL, FALSE);
                } else if (wgstype == 3) {
                  FFStartPrint (ffstring, awp->format, 0, 12, "WGS_CONTIG", 12, 0, 0, NULL, FALSE);
                }
              
                if ( GetWWW(ajp) ) {
                  if (StringCmp (first, last) != 0) {
                    FFAddTextToString(ffstring, "<a href=", link_wgs, NULL, FALSE, FALSE, TILDE_IGNORE);
                    FFAddTextToString(ffstring, "db=Nucleotide&cmd=Search&term=", first, NULL, FALSE, FALSE, TILDE_IGNORE);
                    FFAddTextToString(ffstring, ":", last, "[PACC]>", FALSE, FALSE, TILDE_IGNORE);
                    sprintf (buf, "%s-%s", first, last);
                    FFAddOneString (ffstring, buf, FALSE, FALSE, TILDE_TO_SPACES);
                    FFAddOneString (ffstring, "</a>", FALSE, FALSE, TILDE_TO_SPACES);
                  } else {
                    FFAddTextToString(ffstring, "<a href=", link_seq, NULL, FALSE, FALSE, TILDE_IGNORE);
                    FFAddTextToString(ffstring, "val=", first, ">", FALSE, FALSE, TILDE_IGNORE);
                    sprintf (buf, "%s", first);
                    FFAddOneString (ffstring, buf, FALSE, FALSE, TILDE_TO_SPACES);
                    FFAddOneString (ffstring, "</a>", FALSE, FALSE, TILDE_TO_SPACES);
                  }
                } else {
                  if (StringCmp (first, last) != 0) {
                    sprintf (buf, "%s-%s", first, last);
                    FFAddOneString (ffstring, buf, FALSE, FALSE, TILDE_TO_SPACES);
                  } else {
                    sprintf (buf, "%s", first);
                    FFAddOneString (ffstring, buf, FALSE, FALSE, TILDE_TO_SPACES);
                  }
                }

                bbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 0, 0, NULL);
                FFRecycleString(ajp, ffstring);
              }

              if (awp->afp != NULL) {
                DoImmediateFormat (awp->afp, bbp);
              }
            }
          }
        }
      }
    }
    sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_user, &dcontext);
  }
}

NLM_EXTERN void AddGenomeBlock (
  Asn2gbWorkPtr awp
)

{
  CharPtr            accn;
  IntAsn2gbJobPtr    ajp;
  BaseBlockPtr       bbp;
  BioseqPtr          bsp;
  SeqMgrDescContext  dcontext;
  Boolean            first = TRUE;
  CharPtr            moltype;
  ObjectIdPtr        oip;
  SeqDescrPtr        sdp;
  UserFieldPtr       ufp;
  UserObjectPtr      uop;
  UserFieldPtr       urf;
  StringItemPtr      ffstring;

  if (awp == NULL) return;
  ajp = awp->ajp;
  if ( ajp == NULL ) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;

  if (awp->format == EMBL_FMT || awp->format == EMBLPEPT_FMT) return;

  bbp = Asn2gbAddBlock (awp, GENOME_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  ffstring = FFGetString(ajp);
  if ( ffstring == NULL ) return;

  FFStartPrint (ffstring, awp->format, 0, 12, "GENOME", 12, 0, 0, NULL, FALSE);

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_user, &dcontext);
  while (sdp != NULL) {
    uop = (UserObjectPtr) sdp->data.ptrvalue;
    if (uop != NULL) {
      oip = uop->type;
      if (oip != NULL && StringICmp (oip->str, "GenomeProject") == 0) {
        for (ufp = uop->data; ufp != NULL; ufp = ufp->next) {
          oip = ufp->label;
          if (oip == NULL || oip->str == NULL || ufp->choice != 11) continue;
          if (StringICmp (oip->str, "Chromosome") != 0) continue;
          accn = NULL;
          moltype = NULL;
          for (urf = (UserFieldPtr) ufp->data.ptrvalue; urf != NULL; urf = urf->next) {
            oip = urf->label;
            if (oip == NULL || oip->str == NULL || urf->choice != 1) continue;
            if (StringICmp (oip->str, "accession") == 0) {
              accn = (CharPtr) urf->data.ptrvalue;
            } else if (StringICmp (oip->str, "Moltype") == 0) {
              moltype = (CharPtr) urf->data.ptrvalue;
            }
          }
          if (! StringHasNoText (accn)) {
            if (! first) {
              FFAddNewLine(ffstring);
            }
            first = FALSE;
            FFAddOneString (ffstring, accn, FALSE, FALSE, TILDE_IGNORE);
            if (! StringHasNoText (moltype)) {
              FFAddTextToString (ffstring, " (", moltype, ")", FALSE, FALSE, TILDE_TO_SPACES);
            }
          }
        }
      }
    }
    sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_user, &dcontext);
  }

  bbp->string = FFEndPrint(ajp, ffstring, awp->format, 12, 12, 0, 0, NULL);
  FFRecycleString(ajp, ffstring);

  if (awp->afp != NULL) {
    DoImmediateFormat (awp->afp, bbp);
  }
}

NLM_EXTERN void AddBasecountBlock (
  Asn2gbWorkPtr awp
)

{
  IntAsn2gbJobPtr  ajp;
  BaseBlockPtr     bbp;
  BioseqPtr        bsp;
 
  if (awp == NULL) return;
  ajp = awp->ajp;
  if (ajp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;

  bbp = Asn2gbAddBlock (awp, BASECOUNT_BLOCK, sizeof (BaseBlock));

  if (awp->afp != NULL) {
    DoImmediateFormat (awp->afp, bbp);
  }
}

NLM_EXTERN void AddOriginBlock (
  Asn2gbWorkPtr awp
)

{
  IntAsn2gbJobPtr    ajp;
  BaseBlockPtr       bbp;
  BioseqPtr          bsp;
  Char               buf [67];
  SeqMgrDescContext  dcontext;
  GBBlockPtr         gbp;
  SeqDescrPtr        sdp;
  StringItemPtr      ffstring;

  if (awp == NULL) return;
  ajp = awp->ajp;
  if (ajp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;

  if (awp->format == EMBL_FMT || awp->format == EMBLPEPT_FMT) return;
  
  ffstring = FFGetString(ajp);
  if ( ffstring == NULL ) return;

  bbp = Asn2gbAddBlock (awp, ORIGIN_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  bbp->entityID = awp->entityID;

  if (awp->format == GENBANK_FMT || awp->format == GENPEPT_FMT) {

    buf [0] = '\0';

    sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_genbank, &dcontext);
    if (sdp != NULL) {
      gbp = (GBBlockPtr) sdp->data.ptrvalue;
      if (gbp != NULL && (! StringHasNoText (gbp->origin))) {
        StringNCpy_0 (buf, gbp->origin, sizeof (buf));
        bbp->entityID = dcontext.entityID;
        bbp->itemID = dcontext.itemID;
        bbp->itemtype = OBJ_SEQDESC;
      }
    }

    FFStartPrint (ffstring, awp->format, 0, 12, "ORIGIN", 12, 0, 0, NULL, FALSE);

    if (! StringHasNoText (buf)) {
      FFAddOneString (ffstring, buf, TRUE, FALSE, TILDE_TO_SPACES);
    }
  }

  bbp->string = FFEndPrint(ajp, ffstring, awp->format, 0, 12, 0, 0, NULL);
  FFRecycleString(ajp, ffstring);

  if (awp->afp != NULL) {
    DoImmediateFormat (awp->afp, bbp);
  }
}

#define BASES_PER_BLOCK 1200

NLM_EXTERN void AddSequenceBlock (
  Asn2gbWorkPtr awp
)

{
  IntAsn2gbJobPtr  ajp;
  BioseqPtr        bsp;
  Int4             len;
  SeqBlockPtr      sbp;
  Int4             start;
  Int4             stop;

  if (awp == NULL) return;
  ajp = awp->ajp;
  if (ajp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;

  if (awp->slp != NULL) {
    len = SeqLocLen (awp->slp);
  } else {
    len = bsp->length;
  }

  /* if generating GBSeq XML, populate single sequence block */

  if (ajp->gbseq) {
    sbp = (SeqBlockPtr) Asn2gbAddBlock (awp, SEQUENCE_BLOCK, sizeof (SeqBlock));
    if (sbp == NULL) return;

    sbp->entityID = bsp->idx.entityID;
    sbp->itemID = bsp->idx.itemID;
    sbp->itemtype = OBJ_BIOSEQ;

    sbp->start = 0;
    sbp->stop = len;

    if (awp->afp != NULL) {
      DoImmediateFormat (awp->afp, (BaseBlockPtr) sbp);
    }

    return;
  }

  /* otherwise populate individual sequence blocks for given range */

  for (start = 0; start < len; start += BASES_PER_BLOCK) {
    sbp = (SeqBlockPtr) Asn2gbAddBlock (awp, SEQUENCE_BLOCK, sizeof (SeqBlock));
    if (sbp == NULL) continue;

    sbp->entityID = bsp->idx.entityID;
    sbp->itemID = bsp->idx.itemID;
    sbp->itemtype = OBJ_BIOSEQ;

    stop = start + BASES_PER_BLOCK;
    if (stop >= len) {
      stop = len;
    }

    sbp->start = start;
    sbp->stop = stop;

    if (awp->afp != NULL) {
      DoImmediateFormat (awp->afp, (BaseBlockPtr) sbp);
    }
  }
}

NLM_EXTERN void AddContigBlock (
  Asn2gbWorkPtr awp
)

{
  BaseBlockPtr  bbp;

  if (awp == NULL) return;

  bbp = Asn2gbAddBlock (awp, CONTIG_BLOCK, sizeof (BaseBlock));

  if (awp->afp != NULL) {
    DoImmediateFormat (awp->afp, bbp);
  }
}

NLM_EXTERN void AddSlashBlock (
  Asn2gbWorkPtr awp
)

{
  BaseBlockPtr  bbp;
  CharPtr str;

  if (awp == NULL) return;

  bbp = Asn2gbAddBlock (awp, SLASH_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  bbp->entityID = awp->entityID;

  str = MemNew(sizeof(Char) * 4);
  StringNCpy(str, "//\n", 4);

  bbp->string = str;

  if (awp->afp != NULL) {
    DoImmediateFormat (awp->afp, bbp);
  }
}

