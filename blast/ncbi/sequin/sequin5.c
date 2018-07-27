/*   sequin5.c
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
* File Name:  sequin5.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   8/26/97
*
* $Revision: 6.169 $
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

#define USE_BLAST3

#include "sequin.h"
#include <document.h>
#include <biosrc.h>
#include <jzcoll.h>
#include <objsub.h>
#ifdef USE_BLAST3
#include <netblap3.h>
#else
#include <netblap2.h>
#include <blast2.h>
#endif
#include <dust.h>
#include <pobutil.h>
#include <saledit.h>
#include <salstruc.h>
#include <salfiles.h>
#include <salign.h>
#include <salsap.h>
#include <accutils.h>
#include <edutil.h>
#include <bspview.h>
#include <gbfeat.h>
#include <gbftdef.h>
#include <blast.h>
#include <blastdef.h>
#include <satutil.h>
#include <salpedit.h>
#include <salptool.h>
#include <rpsutil.h>
#include <subutil.h>
#include <explore.h>
#include <import.h>
#include <salutil.h>

static void CommonLaunchBioseqViewer (SeqEntryPtr sep, CharPtr path, Boolean directToEditor)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Pointer       dataptr;
  Uint2         datatype;
  Uint2         entityID;
  Int2          handled;

  if (sep != NULL) {
    if (IS_Bioseq (sep)) {
      datatype = OBJ_BIOSEQ;
    } else if (IS_Bioseq_set (sep)) {
      datatype = OBJ_BIOSEQSET;
    } else {
      sep = SeqEntryFree (sep);
      return;
    }
    dataptr = (Pointer) sep->data.ptrvalue;
    entityID = ObjMgrRegister (datatype, dataptr);
    if (dataptr != NULL && entityID > 0) {
      if (datatype == OBJ_SEQSUB || datatype == OBJ_SEQENTRY ||
          datatype == OBJ_BIOSEQ || datatype == OBJ_BIOSEQSET) {
        WatchCursor ();
        sep = GetTopSeqEntryForEntityID (entityID);
        if (sep == NULL) {
          sep = SeqEntryNew ();
          if (sep != NULL) {
            if (datatype == OBJ_BIOSEQ) {
              bsp = (BioseqPtr) dataptr;
              sep->choice = 1;
              sep->data.ptrvalue = bsp;
              SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) bsp, sep);
            } else if (datatype == OBJ_BIOSEQSET) {
              bssp = (BioseqSetPtr) dataptr;
              sep->choice = 2;
              sep->data.ptrvalue = bssp;
              SeqMgrSeqEntry (SM_BIOSEQSET, (Pointer) bssp, sep);
            } else {
              sep = SeqEntryFree (sep);
            }
          }
          sep = GetTopSeqEntryForEntityID (entityID);
        }
        if (sep != NULL) {
          SeqEntryPack (sep);
        }
        if (directToEditor) {
          handled = GatherProcLaunch (OMPROC_EDIT, FALSE, entityID, 1,
                                      OBJ_BIOSEQ, 0, 0, OBJ_BIOSEQ, 0);
        } else {
          if (sep != NULL) {
            if (! leaveAsOldAsn) {
              MySeqEntryToAsn3 (sep, TRUE, FALSE, FALSE);
            }
          }
          seqviewprocs.filepath = path;
          seqviewprocs.forceSeparateViewer = TRUE;
          handled = GatherProcLaunch (OMPROC_VIEW, FALSE, entityID, 1,
                                      OBJ_BIOSEQ, 0, 0, OBJ_BIOSEQ, 0);
          seqviewprocs.filepath = NULL;
        }
        ArrowCursor ();
        if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
          Message (MSG_FATAL, "Unable to launch viewer.");
          SeqEntryFree (sep);
          return;
        } else {
          SendHelpScrollMessage (helpForm, "Editing the Record", NULL);
        }
        ObjMgrSetOptions (OM_OPT_FREE_IF_NO_VIEW, entityID);
        ObjMgrSetDirtyFlag (entityID, TRUE);
      } else {
        Message (MSG_ERROR, "Unable to process object type %d.", (int) datatype);
        ObjMgrDelete (datatype, dataptr);
      }
    }
  }
}

extern void FastaNucDirectToSeqEdProc (IteM i)

{
  FILE         *fp;
  Char         path [PATH_MAX];
  SeqEntryPtr  sep;

  if (! GetInputFileName (path, sizeof (path), "", "TEXT")) return;
  fp = FileOpen (path, "r");
  if (fp == NULL) return;
  sep = SequinFastaToSeqEntryEx (fp, TRUE, NULL, TRUE, NULL);
  FileClose (fp);
  CommonLaunchBioseqViewer (sep, path, TRUE);
}

#define BUFSIZE 128

static CharPtr GetSequenceString (BioseqPtr bsp)

{
  Char          buf [BUFSIZE + 5];
  Int2          j;
  ByteStorePtr  raw;
  CharPtr       sequence = NULL;
  SeqPortPtr    spp;
  Uint1         u1Residue;

  if (bsp == NULL) return NULL;
  if (! ISA_na (bsp->mol)) return NULL;
  spp = SeqPortNew (bsp, 0, -1, 0, Seq_code_iupacna);
  if (spp == NULL) return NULL;
  raw = BSNew (bsp->length + 5);
  if (raw == NULL) {
    SeqPortFree (spp);
    return NULL;
  }

  j = 0;
  buf [0] = '\0';
  while ((u1Residue = SeqPortGetResidue (spp)) != SEQPORT_EOF) {
    if (IS_residue (u1Residue)) {
      u1Residue = TO_UPPER(u1Residue);
      if (u1Residue == 'U') {
        u1Residue = 'T';
      }
      buf [j] = (Char) u1Residue;
      j++;
      if (j >= BUFSIZE) {
        BSWrite (raw, buf, j * sizeof (Char));
        j = 0;
      }
    }
  }
  buf [j] = '\0';
  if (j >= 0) {
    BSWrite (raw, buf, j * sizeof (Char));
  }
  sequence = BSMerge (raw, NULL);
  BSFree (raw);
  SeqPortFree (spp);
  return sequence;
}

static Boolean LookForSearchString (CharPtr title, CharPtr str, CharPtr tmp, size_t maxsize)

{
  CharPtr  ptr;

  ptr = StringISearch (title, str);
  if (ptr != NULL) {
    StringNCpy_0 (tmp, ptr + StringLen (str), maxsize);
     ptr = StringChr (tmp, ']');
     if (ptr != NULL) {
       *ptr = '\0';
     }
    return TRUE;
  }
  return FALSE;
}

typedef struct geneextendlist {
  GeneRefPtr  grp;
  SeqLocPtr   slp;
  ObjMgrPtr   omp;
  Boolean     rsult;
  Char        label [41];
} GeneExtendList, PNTR GeneExtendPtr;

static Boolean GeneExtendFunc (GatherContextPtr gcp)

{
  BioseqPtr      bsp;
  GeneExtendPtr  gep;
  GeneRefPtr     grp;
  Boolean        hasNulls;
  ObjMgrTypePtr  omtp;
  SeqFeatPtr     sfp;
  SeqLocPtr      slp;
  Char           thislabel [41];

  if (gcp == NULL) return TRUE;

  gep = (GeneExtendPtr) gcp->userdata;
  if (gep == NULL ) return TRUE;

  thislabel [0] = '\0';

  if (gcp->thistype == OBJ_SEQFEAT) {
    sfp = (SeqFeatPtr) gcp->thisitem;
    if (sfp != NULL && sfp->data.choice == SEQFEAT_GENE && sfp->data.value.ptrvalue != NULL) {
      grp = (GeneRefPtr) sfp->data.value.ptrvalue;
      omtp = ObjMgrTypeFind (gep->omp, gcp->thistype, NULL, NULL);
      if (omtp == NULL) {
        return TRUE;
      }
      if (omtp->labelfunc != NULL) {
        (*(omtp->labelfunc)) (gcp->thisitem, thislabel, 40, OM_LABEL_CONTENT);
      }
      if (thislabel [0] != '\0') {
        if (StringICmp (thislabel, gep->label) == 0) {
          if (/* SeqLocCompare (gep->slp, sfp->location) != SLC_NO_MATCH */
              SeqLocAinB (gep->slp, sfp->location) <= 0) {
            bsp = GetBioseqGivenSeqLoc (sfp->location, gcp->entityID);
            if (bsp != NULL) {
              slp = SeqLocMerge (bsp, sfp->location, gep->slp, TRUE, FALSE, FALSE);
              if (slp != NULL) {
                sfp->location = SeqLocFree (sfp->location);
                sfp->location = slp;
                if (bsp->repr == Seq_repr_seg) {
                  slp = SegLocToPartsEx (bsp, sfp->location, TRUE);
                  sfp->location = SeqLocFree (sfp->location);
                  sfp->location = slp;
                  hasNulls = LocationHasNullsBetween (sfp->location);
                  sfp->partial = (sfp->partial || hasNulls);
                }
                FreeAllFuzz (slp);
                gep->rsult = TRUE;
              }
            }
          }
          return FALSE;
        }
      }
    }
  }
  return TRUE;
}

static Boolean OligoExtendGene (GeneRefPtr grp, SeqEntryPtr nsep, SeqLocPtr slp)

{
  GeneExtendList  gel;
  GatherScope     gs;
  ObjMgrTypePtr   omtp;
  SeqFeatPtr      sfp;

  if (grp == NULL || nsep == NULL || slp == NULL) return FALSE;
  gel.grp = grp;
  gel.slp = slp;
  gel.omp = ObjMgrGet ();
  gel.label [0] = '\0';
  gel.rsult = FALSE;
  omtp = ObjMgrTypeFind (gel.omp, OBJ_SEQFEAT, NULL, NULL);
  if (omtp != NULL && omtp->labelfunc != NULL) {
    sfp = SeqFeatNew ();
    if (sfp != NULL) {
      sfp->data.choice = SEQFEAT_GENE;
      sfp->data.value.ptrvalue = (Pointer) grp;
      (*(omtp->labelfunc)) ((Pointer) sfp, gel.label, 40, OM_LABEL_CONTENT);
      sfp->data.value.ptrvalue = NULL;
      SeqFeatFree (sfp);
    }
  }
  MemSet ((Pointer)(&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = TRUE;
  MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  GatherSeqEntry (nsep, (Pointer) &gel, GeneExtendFunc, &gs);
  return gel.rsult;
}

static void AddGBQual (SeqFeatPtr sfp, CharPtr qual, CharPtr val)

{
  GBQualPtr  gbq;
  GBQualPtr  last;

  if (sfp == NULL || StringHasNoText (qual) || StringHasNoText (val)) return;
  gbq = GBQualNew ();
  if (gbq == NULL) return;
  gbq->qual = StringSave (qual);
  gbq->val = StringSave (val);
  if (sfp->qual == NULL) {
    sfp->qual = gbq;
  } else {
    last = sfp->qual;
    while (last->next != NULL) {
      last = last->next;
    }
    last->next = gbq;
  }
}

static void ProcessOligoDefline (SeqFeatPtr sfp, CharPtr title, SeqEntryPtr nsep)

{
  BioseqPtr   bsp;
  GeneRefPtr  grp;
  SeqFeatPtr  gsfp;
  SeqLocPtr   gslp;
  Boolean     hasNulls;
  SeqIdPtr    sip;
  Char        tmp [256];

  if (sfp == NULL || StringHasNoText (title) || nsep == NULL) return;
  if (LookForSearchString (title, "[comment=", tmp, sizeof (tmp) - 1)) {
    sfp->comment = StringSave (tmp);
  }
  if (LookForSearchString (title, "[stdname=", tmp, sizeof (tmp) - 1)) {
    AddGBQual (sfp, "standard_name", tmp);
  }
  if (LookForSearchString (title, "[conditions=", tmp, sizeof (tmp) - 1)) {
    AddGBQual (sfp, "PCR_conditions", tmp);
  }
  if (LookForSearchString (title, "[gene=", tmp, sizeof (tmp) - 1)) {
    grp = CreateNewGeneRef (tmp, NULL, NULL, FALSE);
    if (grp != NULL) {
      /*
      slp = AsnIoMemCopy ((Pointer) sfp->location,
                          (AsnReadFunc) SeqLocAsnRead,
                          (AsnWriteFunc) SeqLocAsnWrite);
      if (slp != NULL || slp->choice == SEQLOC_INT) {
        sintp = (SeqIntPtr) slp->data.ptrvalue;
        if (sintp != NULL) {
          if (sintp->strand == Seq_strand_minus) {
            sintp->strand = Seq_strand_plus;
          } else if (sintp->strand == Seq_strand_plus) {
            sintp->strand = Seq_strand_minus;
          }
        }
      }
      */
      if (OligoExtendGene (grp, nsep, sfp->location) /* || OligoExtendGene (grp, nsep, slp) */) {
        grp = GeneRefFree (grp);
      } else {
        gsfp = CreateNewFeature (nsep, NULL, SEQFEAT_GENE, NULL);
        if (gsfp != NULL) {
          gsfp->data.value.ptrvalue = (Pointer) grp;
          gsfp->location = SeqLocFree (gsfp->location);
          gsfp->location = AsnIoMemCopy ((Pointer) sfp->location,
                                        (AsnReadFunc) SeqLocAsnRead,
                                        (AsnWriteFunc) SeqLocAsnWrite);
          sip = SeqLocId (gsfp->location);
          if (sip != NULL) {
            bsp = BioseqFind (sip);
          } else {
            bsp = (BioseqPtr) nsep->data.ptrvalue;
          }
          if (bsp != NULL) {
            gslp = SeqLocMerge (bsp, gsfp->location, NULL, TRUE, FALSE, FALSE);
            if (gslp != NULL) {
              gsfp->location = SeqLocFree (gsfp->location);
              gsfp->location = gslp;
              if (bsp->repr == Seq_repr_seg) {
                gslp = SegLocToPartsEx (bsp, gsfp->location, TRUE);
                gsfp->location = SeqLocFree (gsfp->location);
                gsfp->location = gslp;
                hasNulls = LocationHasNullsBetween (gsfp->location);
                gsfp->partial = (gsfp->partial || hasNulls);
              }
              FreeAllFuzz (gslp);
            }
          }
        }
      }
      /* SeqLocFree (slp); */
    }
  }
}

static void ProcessOligo (SeqEntryPtr sep, CharPtr sequence, CharPtr title)

{
  BioseqSetPtr  bssp;
  ImpFeatPtr    ifp;
  BioseqPtr     nbsp;
  SeqEntryPtr   nsep;
  SeqFeatPtr    sfp;
  SeqLocPtr     slp;

  if (sep == NULL || sequence == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 7 ||
                         (IsPopPhyEtcSet (bssp->_class)))) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        ProcessOligo (sep, sequence, title);
      }
      return;
    }
  }
  nsep = FindNucSeqEntry (sep);
  if (nsep == NULL) return;
  if (! IS_Bioseq (nsep)) return;
  nbsp = (BioseqPtr) nsep->data.ptrvalue;
  if (nbsp == NULL) return;
  slp = StringSearchInBioseq (nbsp->id, sequence);
  if (slp == NULL) return;
  ifp = ImpFeatNew ();
  if (ifp != NULL) {
    ifp->key = StringSave ("primer_bind");
    sfp = CreateNewFeature (nsep, NULL, SEQFEAT_IMP, NULL);
    if (sfp != NULL) {
      sfp->data.value.ptrvalue = (Pointer) ifp;
      sfp->location = SeqLocFree (sfp->location);
      sfp->location = slp;
      ProcessOligoDefline (sfp, title, nsep);
    }
  }
}

extern void ParseInOligoPrimers (IteM i)

{
  BaseFormPtr  bfp;
  BioseqPtr    bsp;
  CharPtr      extension;
  FILE         *fp;
  SeqEntryPtr  nextsep;
  Char         path [PATH_MAX];
  SeqEntryPtr  sep;
  CharPtr      sequence;
  CharPtr      title;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  extension = GetAppProperty ("FastaNucExtension");
  if (! GetInputFileName (path, sizeof (path), extension, "TEXT")) return;
  fp = FileOpen (path, "r");
  if (fp == NULL) return;
  WatchCursor ();
  Update ();

  nextsep = SequinFastaToSeqEntryEx (fp, TRUE, NULL, FALSE, NULL);
  while (nextsep != NULL) {
    if (IS_Bioseq (nextsep)) {
      bsp = (BioseqPtr) nextsep->data.ptrvalue;
      sequence = GetSequenceString (bsp);
      title = SeqEntryGetTitle (nextsep);
      ProcessOligo (sep, sequence, title);
      MemFree (sequence);
    }
    SeqEntryFree (nextsep);
    nextsep = SequinFastaToSeqEntryEx (fp, TRUE, NULL, FALSE, NULL);
  }

  FileClose (fp);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  ArrowCursor ();
  Update ();
}

typedef struct edlocdata {
  FORM_MESSAGE_BLOCK
  SeqIdPtr      sip;
  BioseqPtr     bsp;
  TexT          locusname;
  PrompT        accnnumber;
  Boolean       refgene;
} EdLocData, PNTR EdLocPtr;

static CharPtr AllToUpper (CharPtr str)

{
  Char          ch;
  CharPtr       ptr;

  if (str != NULL) {
    ptr = str;
    ch = *ptr;
    while (ch != '\0') {
      *ptr = TO_UPPER (ch);
      ptr++;
      ch = *ptr;
    }
  }
  return str;
}

static void EditLocusActnProc (ForM f)

{
  BioseqPtr     bsp;
  Uint1         choice = SEQID_GENBANK;
  Int2          count;
  SeqIdPtr      id;
  EdLocPtr      elp;
  Int2          len;
  Char          num [16];
  BioseqPtr     part;
  CharPtr       ptr;
  Char          str [64];
  SeqIdPtr      sip;
  SeqLocPtr     slp;
  Char          tmp [76];
  TextSeqIdPtr  tsip;
  ValNode       vn;

  elp = (EdLocPtr) GetObjectExtra (f);
  if (elp != NULL) {
    sip = elp->sip;
    bsp = elp->bsp;
    if (elp->refgene) {
      choice = SEQID_OTHER;
    }
    if (indexerVersion && sip == NULL && bsp != NULL && bsp->id != NULL) {
      id = bsp->id;
      while (id->next != NULL) {
        id = sip->next;
      }
      tsip = TextSeqIdNew ();
      if (tsip != NULL) {
        sip = ValNodeNew (NULL);
        if (sip != NULL) {
          sip->choice = choice;
          sip->data.ptrvalue = (Pointer) tsip;
        } else {
          TextSeqIdFree (tsip);
        }
        id->next = sip;
        SeqMgrReplaceInBioseqIndex (bsp);
      }
    }
    if (sip == NULL) return;
    tsip = (TextSeqIdPtr) sip->data.ptrvalue;
    if (tsip == NULL) return;
    tsip->accession = MemFree (tsip->accession);
    GetTitle (elp->accnnumber, str, sizeof (str) - 1);
    TrimSpacesAroundString (str);
    if (! StringHasNoText (str)) {
      tsip->accession = StringSave (AllToUpper (str));
    }
    tsip->name = MemFree (tsip->name);
    GetTitle (elp->locusname, str, sizeof (str) - 1);
    if (str [0] != '\0') {
      if (elp->refgene) {
        tsip->name = StringSave (str);
      } else {
        tsip->name = StringSave (AllToUpper (str));
      }
    }
    if (bsp != NULL && bsp->repr == Seq_repr_seg && bsp->seq_ext != NULL) {
      if (StringNCmp (str, "SEG_", 4) != 0) {
        sprintf (tmp, "SEG_%s", str);
        tsip->name = MemFree (tsip->name);
        tsip->name = StringSave (tmp);
        SeqMgrReplaceInBioseqIndex (bsp);
      }
      vn.choice = SEQLOC_MIX;
      vn.next = NULL;
      vn.data.ptrvalue = bsp->seq_ext;
      count = 0;
      slp = SeqLocFindNext (&vn, NULL);
      while (slp != NULL) {
        if (slp->choice != SEQLOC_NULL) {
          count++;
        }
        slp = SeqLocFindNext (&vn, slp);
      }
      if (count < 10) {
        len = 1;
      } else if (count < 100) {
        len = 2;
      } else {
        len = 3;
      }
      count = 0;
      slp = SeqLocFindNext (&vn, NULL);
      while (slp != NULL) {
        if (slp->choice != SEQLOC_NULL) {
          count++;
          sprintf (num, "%*d", (int) len, (int) count);
          for (ptr = num; *ptr != '\0'; ptr++) {
            if (*ptr == ' ') {
              *ptr = '0';
            }
          }
          sprintf (tmp, "%s%s", str, num);
          sip = SeqLocId (slp);
          if (sip != NULL) {
            part = BioseqFind (sip);
            if (part != NULL && part->id != NULL) {
              id = part->id;
              while (id->choice != choice && id->next != NULL) {
                id = id->next;
              }
              if (id->choice != choice) {
                tsip = TextSeqIdNew ();
                if (tsip != NULL) {
                  sip = ValNodeNew (NULL);
                  if (sip != NULL) {
                    sip->choice = choice;
                    sip->data.ptrvalue = (Pointer) tsip;
                  } else {
                    TextSeqIdFree (tsip);
                  }
                  id->next = sip;
                  id = sip;
                }
              }
              if (id->choice == choice) {
                tsip = (TextSeqIdPtr) id->data.ptrvalue;
                if (tsip != NULL) {
                  tsip->name = MemFree (tsip->name);
                  if (tmp [0] != '\0') {
                    if (elp->refgene) {
                      tsip->name = StringSave (tmp);
                    } else {
                      tsip->name = StringSave (AllToUpper (tmp));
                    }
                  }
                }
              }
              SeqMgrReplaceInBioseqIndex (part);
            }
          }
        }
        slp = SeqLocFindNext (&vn, slp);
      }
    }
    ObjMgrSetDirtyFlag (elp->input_entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, elp->input_entityID, 0, 0);
  }
}

static void EditLocusMessage (ForM f, Int2 mssg)

{
  EdLocPtr  elp;

  elp = (EdLocPtr) GetObjectExtra (f);
  if (elp != NULL) {
    switch (mssg) {
      case VIB_MSG_CLOSE :
        Remove (f);
        break;
      case VIB_MSG_CUT :
        StdCutTextProc (NULL);
        break;
      case VIB_MSG_COPY :
        StdCopyTextProc (NULL);
        break;
      case VIB_MSG_PASTE :
        StdPasteTextProc (NULL);
        break;
      case VIB_MSG_DELETE :
        StdDeleteTextProc (NULL);
        break;
      default :
        if (elp->appmessage != NULL) {
          elp->appmessage (f, mssg);
        }
        break;
    }
  }
}

#define NUM_ORDER 16

static SeqIdPtr SeqIdFindGenBankOrOther (SeqIdPtr sip)

{
  Uint1  order [NUM_ORDER];

  SeqIdBestRank (order, NUM_ORDER);
  order [SEQID_LOCAL] = 255;
  order [SEQID_GENBANK] = 1;
  order [SEQID_EMBL] = 2;
  order [SEQID_PIR] = 255;
  order [SEQID_SWISSPROT] = 255;
  order [SEQID_DDBJ] = 3;
  order [SEQID_PRF] = 255;
  order [SEQID_PDB] = 255;
  order [SEQID_PATENT] = 255;
  order [SEQID_OTHER] = 2;
  order [SEQID_GENERAL] = 255;
  order [SEQID_GIBBSQ] = 255;
  order [SEQID_GIBBMT] = 255;
  order [SEQID_GIIM] = 255;
  order [SEQID_GI] = 255;
  return SeqIdSelect (sip, order, NUM_ORDER);
}

void EditLocusProc (IteM i)

{
  ButtoN             b;
  BaseFormPtr        bfp;
  BioseqPtr          bsp;
  GrouP              c;
  EdLocPtr           elp;
  GrouP              g;
  StdEditorProcsPtr  sepp;
  SeqIdPtr           sip;
  TextSeqIdPtr       tsip;
  WindoW             w;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  if (bfp->input_itemtype != OBJ_BIOSEQ) return;
  bsp = GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
  if (bsp == NULL) return;
  sip = SeqIdFindGenBankOrOther (bsp->id);
  /*if (sip == NULL) return;*/
  elp = (EdLocPtr) MemNew (sizeof (EdLocData));
  if (elp == NULL) return;
  w = MovableModalWindow (-50, -33, -10, -10,
                          "Locus and Accession Number",
                          StdCloseWindowProc);
  SetObjectExtra (w, elp, StdCleanupFormProc);
  elp->form = (ForM) w;
  elp->actproc = EditLocusActnProc;
  elp->formmessage = EditLocusMessage;

  elp->input_entityID = bfp->input_entityID;
  elp->input_itemID = bfp->input_itemID;
  elp->input_itemtype = bfp->input_itemtype;

#ifndef WIN_MAC
  CreateStdEditorFormMenus (w);
#endif

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    elp->appmessage = sepp->handleMessages;
  }

  g = HiddenGroup (w, 2, 0, NULL);
  elp->sip = sip;
  elp->bsp = bsp;
  StaticPrompt (g, "Locus: ", 0, dialogTextHeight, systemFont, 'l');
  elp->locusname = DialogText (g, "", 20, NULL);
  StaticPrompt (g, "Accession: ", 0, stdLineHeight, systemFont, 'l');
  elp->accnnumber = StaticPrompt (g, "", 20 * stdCharWidth, stdLineHeight, systemFont, 'l');
  c = HiddenGroup (w, 2, 0, NULL);
  b = DefaultButton (c, "Accept", StdAcceptFormButtonProc);
  SetObjectExtra (b, elp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);
  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, NULL);
  if (sip != NULL) {
    tsip = (TextSeqIdPtr) sip->data.ptrvalue;
    if (tsip != NULL) {
      SetTitle (elp->locusname, tsip->name);
      SetTitle (elp->accnnumber, tsip->accession);
    }
    if (sip->choice == SEQID_OTHER) {
      elp->refgene = TRUE;
    }
  }
  RealizeWindow (w);
  Select (elp->locusname);
  Show (w);
  Select (w);
  Update ();
}

static void ConvertSeqIdListToLocalID (SeqIdPtr sip, SeqEntryPtr makesecondary)

{
  GBBlockPtr    gbp;
  ObjectIdPtr   oip;
  Char          str [32];
  CharPtr       text;
  TextSeqIdPtr  tsip;
  ValNodePtr    vnp;

  while (sip != NULL) {
    switch (sip->choice) {
      case SEQID_NOT_SET :
        break;
      case SEQID_LOCAL :
        break;
      case SEQID_GIBBSQ :
        break;
      case SEQID_GIBBMT :
        break;
      case SEQID_GIIM :
        break;
      case SEQID_GENBANK :
      case SEQID_EMBL :
      case SEQID_DDBJ :
        tsip = (TextSeqIdPtr) sip->data.ptrvalue;
        if (tsip != NULL) {
          text = NULL;
          str [0] = '\0';
          if (tsip->accession != NULL) {
            text = tsip->accession;
          } else if (tsip->name != NULL) {
            text = tsip->name;
          }
          if (text != NULL) {
            StringNCpy_0 (str, text, sizeof (str));
            oip = ObjectIdNew ();
            if (oip != NULL) {
              oip->str = StringSave (text);
              sip->choice = SEQID_LOCAL;
              sip->data.ptrvalue = (Pointer) oip;
              TextSeqIdFree (tsip);
            }
            if (makesecondary != NULL) {
              vnp = SeqEntryGetSeqDescr (makesecondary, Seq_descr_genbank, NULL);
              if (vnp == NULL) {
                vnp = CreateNewDescriptor (makesecondary, Seq_descr_genbank);
                if (vnp != NULL) {
                  gbp = GBBlockNew ();
                  vnp->data.ptrvalue = (Pointer) gbp;
                }
              }
              if (vnp != NULL) {
                gbp = (GBBlockPtr) vnp->data.ptrvalue;
                if (gbp != NULL) {
                  for (vnp = gbp->extra_accessions;
                       vnp != NULL && StringICmp (vnp->data.ptrvalue, str) != 0;
                       vnp = vnp->next) continue;
                  if (vnp == NULL) {
                    vnp = ValNodeCopyStr (&(gbp->extra_accessions), 0, str);
                  }
                }
              }
            }
          }
        }
        break;
      case SEQID_PIR :
        break;
      case SEQID_SWISSPROT :
        break;
      case SEQID_PATENT :
        break;
      case SEQID_OTHER :
        break;
      case SEQID_GENERAL :
        break;
      case SEQID_GI :
        break;
      case SEQID_PRF :
        break;
      case SEQID_PDB :
        break;
      default :
        break;
    }
    sip = sip->next;
  }
}

static void ConvertSeqLocListToLocalID (SeqLocPtr slp)

{
  SeqLocPtr      loc;
  PackSeqPntPtr  psp;
  SeqBondPtr     sbp;
  SeqIntPtr      sinp;
  SeqIdPtr       sip;
  SeqPntPtr      spp;

  while (slp != NULL) {
    switch (slp->choice) {
      case SEQLOC_NULL :
        break;
      case SEQLOC_EMPTY :
      case SEQLOC_WHOLE :
        sip = (SeqIdPtr) slp->data.ptrvalue;
        ConvertSeqIdListToLocalID (sip, NULL);
        break;
      case SEQLOC_INT :
        sinp = (SeqIntPtr) slp->data.ptrvalue;
        if (sinp != NULL) {
          sip = sinp->id;
          ConvertSeqIdListToLocalID (sip, NULL);
        }
        break;
      case SEQLOC_PNT :
        spp = (SeqPntPtr) slp->data.ptrvalue;
        if (spp != NULL) {
          sip = spp->id;
          ConvertSeqIdListToLocalID (sip, NULL);
        }
        break;
      case SEQLOC_PACKED_PNT :
        psp = (PackSeqPntPtr) slp->data.ptrvalue;
        if (psp != NULL) {
          sip = psp->id;
          ConvertSeqIdListToLocalID (sip, NULL);
        }
        break;
      case SEQLOC_PACKED_INT :
      case SEQLOC_MIX :
      case SEQLOC_EQUIV :
        loc = (SeqLocPtr) slp->data.ptrvalue;
        while (loc != NULL) {
          ConvertSeqLocListToLocalID (loc);
          loc = loc->next;
        }
        break;
      case SEQLOC_BOND :
        sbp = (SeqBondPtr) slp->data.ptrvalue;
        if (sbp != NULL) {
          spp = (SeqPntPtr) sbp->a;
          if (spp != NULL) {
            sip = spp->id;
            ConvertSeqIdListToLocalID (sip, NULL);
          }
          spp = (SeqPntPtr) sbp->b;
          if (spp != NULL) {
            sip = spp->id;
            ConvertSeqIdListToLocalID (sip, NULL);
          }
        }
        break;
      case SEQLOC_FEAT :
        break;
      default :
        break;
    }
    slp = slp->next;
  }
}

static Boolean ChangeAccessionToLocalID (GatherContextPtr gcp)

{
  BioseqPtr    bsp;
  BoolPtr      makesec;
  SeqEntryPtr  sep;
  SeqFeatPtr   sfp;
  SeqIdPtr     sip;
  SeqLocPtr    slp;

  if (gcp == NULL) return TRUE;
  makesec = (BoolPtr) gcp->userdata;
  if (gcp->thisitem == NULL) return TRUE;
  switch (gcp->thistype) {
    case OBJ_BIOSEQ :
      sep = NULL;
      bsp = (BioseqPtr) gcp->thisitem;
      if (makesec != NULL && *makesec) {
        sep = SeqMgrGetSeqEntryForData (bsp);
      }
      sip = bsp->id;
      ConvertSeqIdListToLocalID (sip, sep);
      SeqMgrReplaceInBioseqIndex (bsp);
      break;
    case OBJ_SEQFEAT :
      sfp = (SeqFeatPtr) gcp->thisitem;
      slp = sfp->location;
      ConvertSeqLocListToLocalID (slp);
      slp = sfp->product;
      ConvertSeqLocListToLocalID (slp);
      break;
    default :
      break;
  }
  return TRUE;
}

void ConvertToLocalProc (IteM i)

{
  MsgAnswer    ans;
  BaseFormPtr  bfp;
  GatherScope  gs;
  Boolean      makesec;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  if (bfp->input_itemtype != OBJ_BIOSEQ) return;
  ans = Message (MSG_OKC, "Are you sure you want to convert accessions to local IDs?");
  if (ans == ANS_CANCEL) return;
  ans = Message (MSG_OKC, "Are you REALLY sure you want to convert accessions to local IDs?");
  if (ans == ANS_CANCEL) return;
  ans = Message (MSG_YN, "Do you want to make the original accessions secondary?");
  makesec = (Boolean) (ans == ANS_YES);
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  MemSet((Pointer) (gs.ignore), (int) (TRUE), (size_t) (OBJ_MAX * sizeof (Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  GatherEntity (bfp->input_entityID, (Pointer) &makesec, ChangeAccessionToLocalID, &gs);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}
 
static SeqIdPtr SeqIdFindBestForPromotion (SeqIdPtr sip)

{
  return SeqIdFindBest (sip, 0);
}

static SeqIdPtr SeqIdFindWorstForPromotion (SeqIdPtr sip)

{
  return SeqIdFindWorst (sip);
}

static void PromoteSeqId (SeqIdPtr sip, Boolean alsoCheckLocalAccn, Boolean findWorst)

{
  SeqIdPtr     bestid, newid, oldid;
  BioseqPtr    bsp;
  ObjectIdPtr  oip;
  TextSeqId    tsi;
  SeqId        vn;

  bsp = BioseqFind (sip);
  if (bsp == NULL && alsoCheckLocalAccn && sip->choice == SEQID_LOCAL) {
    oip = (ObjectIdPtr) sip->data.ptrvalue;
    if (oip != NULL && (! StringHasNoText (oip->str))) {
      MemSet ((Pointer) &vn, 0, sizeof (SeqId));
      MemSet ((Pointer) &tsi, 0, sizeof (TextSeqId));
      tsi.accession = oip->str;
      vn.choice = SEQID_GENBANK;
      vn.data.ptrvalue = (Pointer) &tsi;
      bsp = BioseqFind (&vn);
    }
  }
  if (bsp == NULL) return;

  if (findWorst) {
    bestid = SeqIdFindWorstForPromotion (bsp->id);
  } else {
    bestid = SeqIdFindBestForPromotion (bsp->id);
  }
  if (bestid == NULL) return;
  newid = SeqIdDup (bestid);
  if (newid == NULL) return;

  oldid = ValNodeNew (NULL);
  if (oldid == NULL) return;

  MemCopy (oldid, sip, sizeof (ValNode));
  oldid->next = NULL;

  sip->choice = newid->choice;
  sip->data.ptrvalue = newid->data.ptrvalue;

  SeqIdFree (oldid);
  ValNodeFree (newid);

  SeqIdStripLocus (sip);
}

static void PromoteSeqIdList (SeqIdPtr sip, Boolean alsoCheckLocalAccn, Boolean findWorst)

{
  while (sip != NULL) {
    PromoteSeqId (sip, alsoCheckLocalAccn, findWorst);
    sip = sip->next;
  }
}

static void PromoteSeqLocList (SeqLocPtr slp, Boolean alsoCheckLocalAccn, Boolean findWorst)

{
  SeqLocPtr      loc;
  PackSeqPntPtr  psp;
  SeqBondPtr     sbp;
  SeqIntPtr      sinp;
  SeqIdPtr       sip;
  SeqPntPtr      spp;

  while (slp != NULL) {
    switch (slp->choice) {
      case SEQLOC_NULL :
        break;
      case SEQLOC_EMPTY :
      case SEQLOC_WHOLE :
        sip = (SeqIdPtr) slp->data.ptrvalue;
        PromoteSeqIdList (sip, alsoCheckLocalAccn, findWorst);
        break;
      case SEQLOC_INT :
        sinp = (SeqIntPtr) slp->data.ptrvalue;
        if (sinp != NULL) {
          sip = sinp->id;
          PromoteSeqIdList (sip, alsoCheckLocalAccn, findWorst);
        }
        break;
      case SEQLOC_PNT :
        spp = (SeqPntPtr) slp->data.ptrvalue;
        if (spp != NULL) {
          sip = spp->id;
          PromoteSeqIdList (sip, alsoCheckLocalAccn, findWorst);
        }
        break;
      case SEQLOC_PACKED_PNT :
        psp = (PackSeqPntPtr) slp->data.ptrvalue;
        if (psp != NULL) {
          sip = psp->id;
          PromoteSeqIdList (sip, alsoCheckLocalAccn, findWorst);
        }
        break;
      case SEQLOC_PACKED_INT :
      case SEQLOC_MIX :
      case SEQLOC_EQUIV :
        loc = (SeqLocPtr) slp->data.ptrvalue;
        while (loc != NULL) {
          PromoteSeqLocList (loc, alsoCheckLocalAccn, findWorst);
          loc = loc->next;
        }
        break;
      case SEQLOC_BOND :
        sbp = (SeqBondPtr) slp->data.ptrvalue;
        if (sbp != NULL) {
          spp = (SeqPntPtr) sbp->a;
          if (spp != NULL) {
            sip = spp->id;
            PromoteSeqIdList (sip, alsoCheckLocalAccn, findWorst);
          }
          spp = (SeqPntPtr) sbp->b;
          if (spp != NULL) {
            sip = spp->id;
            PromoteSeqIdList (sip, alsoCheckLocalAccn, findWorst);
          }
        }
        break;
      case SEQLOC_FEAT :
        break;
      default :
        break;
    }
    slp = slp->next;
  }
}

static Boolean PromoteIDsProc (GatherObjectPtr gop, Boolean findWorst)

{
  CodeBreakPtr  cbp;
  CdRegionPtr   crp;
  RnaRefPtr     rrp;
  SeqFeatPtr    sfp;
  tRNAPtr       trp;

  if (gop->itemtype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gop->dataptr;
  if (sfp == NULL) return TRUE;

  PromoteSeqLocList (sfp->location, FALSE, findWorst);

  PromoteSeqLocList (sfp->product, FALSE, findWorst);

  switch (sfp->data.choice) {
    case SEQFEAT_CDREGION :
      crp = (CdRegionPtr) sfp->data.value.ptrvalue;
      if (crp != NULL && crp->code_break != NULL) {
        for (cbp = crp->code_break; cbp != NULL; cbp = cbp->next) {
          PromoteSeqLocList (cbp->loc, FALSE, findWorst);
        }
      }
      break;
    case SEQFEAT_RNA :
      rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
      if (rrp != NULL && rrp->type == 3 && rrp->ext.choice == 2) {
        trp = rrp->ext.value.ptrvalue;
        if (trp != NULL && trp->anticodon != NULL) {
          PromoteSeqLocList (trp->anticodon, FALSE, findWorst);
        }
      }
      break;
    default :
      break;
  }

  return TRUE;
}

static Boolean PromoteBestIDsProc (GatherObjectPtr gop)

{
  return PromoteIDsProc (gop, FALSE);
}

void PromoteToBestIDProc (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  oldscope, sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  oldscope = SeqEntrySetScope (sep);

  GatherObjectsInEntity (bfp->input_entityID, 0, NULL, PromoteBestIDsProc, NULL, NULL);

  SeqEntrySetScope (oldscope);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static Boolean PromoteWorstIDsProc (GatherObjectPtr gop)

{
  return PromoteIDsProc (gop, TRUE);
}

void PromoteToWorstIDProc (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  oldscope, sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  oldscope = SeqEntrySetScope (sep);

  GatherObjectsInEntity (bfp->input_entityID, 0, NULL, PromoteWorstIDsProc, NULL, NULL);

  SeqEntrySetScope (oldscope);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static Boolean PromoteAlignIDsProc (GatherObjectPtr gop)

{
  DenseDiagPtr  ddp;
  DenseSegPtr   dsp;
  PackSegPtr    psp;
  SeqAlignPtr   sap;
  SeqLocPtr     slp;
  StdSegPtr     ssp;

  if (gop->itemtype != OBJ_SEQALIGN) return TRUE;
  sap = (SeqAlignPtr) gop->dataptr;
  if (sap == NULL) return TRUE;

  switch (sap->segtype) {
    case SAS_DENDIAG :
      for (ddp = sap->segs; ddp != NULL; ddp = ddp->next) {
        PromoteSeqIdList (ddp->id, TRUE, FALSE);
      }
      break;
    case SAS_DENSEG :
      dsp = sap->segs;
      if (dsp != NULL) {
        PromoteSeqIdList (dsp->ids, TRUE, FALSE);
      }
      break;
    case SAS_STD :
      for (ssp = sap->segs; ssp != NULL; ssp = ssp->next) {
        PromoteSeqIdList (ssp->ids, TRUE, FALSE);
        for (slp = ssp->loc; slp != NULL; slp = slp->next) {
          PromoteSeqLocList (slp, TRUE, FALSE);
        }
      }
      break;
    case SAS_PACKED :
      psp = (PackSegPtr) sap->segs;
      if (psp != NULL) {
        PromoteSeqIdList (psp->ids, TRUE, FALSE);
      }
      break;
    case SAS_COMPSEQ :
      break;
    default :
      break;
  }

  return TRUE;
}

extern void PromoteAlignsToBestIDProc (IteM i);
void PromoteAlignsToBestIDProc (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  oldscope, sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  oldscope = SeqEntrySetScope (sep);

  GatherObjectsInEntity (bfp->input_entityID, 0, NULL, PromoteAlignIDsProc, NULL, NULL);

  SeqEntrySetScope (oldscope);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static Boolean RemoveGBIDsProc (GatherObjectPtr gop)

{
  BioseqPtr      bsp;
  SeqIdPtr       nextid, sip;
  SeqIdPtr PNTR  previd;

  if (gop->itemtype != OBJ_BIOSEQ) return TRUE;
  bsp = (BioseqPtr) gop->dataptr;
  if (bsp == NULL) return TRUE;

  previd = (SeqIdPtr PNTR) &(bsp->id);
  sip = bsp->id;
  while (sip != NULL) {
    nextid = sip->next;
    if (sip->choice == SEQID_GENBANK) {
      *previd = sip->next;
      sip->next = NULL;
      SeqIdFree (sip);
    } else {
      previd = (SeqIdPtr PNTR) &(sip->next);
    }
    sip = sip->next;
  }

  return TRUE;
}

void RemoveGBIDsFromBioseqs (IteM i)

{
  MsgAnswer    ans;
  BaseFormPtr  bfp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  ans = Message (MSG_OKC, "Currently deletes all GenBank SeqIDs");
  if (ans == ANS_CANCEL) return;

  GatherObjectsInEntity (bfp->input_entityID, 0, NULL, RemoveGBIDsProc, NULL, NULL);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static Boolean RemoveGBProtIDsProc (GatherObjectPtr gop)

{
  BioseqPtr      bsp;
  SeqIdPtr       nextid, sip;
  SeqIdPtr PNTR  previd;

  if (gop->itemtype != OBJ_BIOSEQ) return TRUE;
  bsp = (BioseqPtr) gop->dataptr;
  if (bsp == NULL) return TRUE;
  if (! ISA_aa (bsp->mol)) return TRUE;

  previd = (SeqIdPtr PNTR) &(bsp->id);
  sip = bsp->id;
  while (sip != NULL) {
    nextid = sip->next;
    if (sip->choice == SEQID_GENBANK) {
      *previd = sip->next;
      sip->next = NULL;
      SeqIdFree (sip);
    } else {
      previd = (SeqIdPtr PNTR) &(sip->next);
    }
    sip = sip->next;
  }

  return TRUE;
}

void RemoveGBIDsFromProteins (IteM i)

{
  MsgAnswer    ans;
  BaseFormPtr  bfp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  ans = Message (MSG_OKC, "Currently deletes all GenBank SeqIDs");
  if (ans == ANS_CANCEL) return;

  GatherObjectsInEntity (bfp->input_entityID, 0, NULL, RemoveGBProtIDsProc, NULL, NULL);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static Boolean RemoveGIProc (GatherObjectPtr gop)

{
  BioseqPtr      bsp;
  SeqIdPtr       nextid, sip;
  SeqIdPtr PNTR  previd;

  if (gop->itemtype != OBJ_BIOSEQ) return TRUE;
  bsp = (BioseqPtr) gop->dataptr;
  if (bsp == NULL) return TRUE;

  previd = (SeqIdPtr PNTR) &(bsp->id);
  sip = bsp->id;
  while (sip != NULL) {
    nextid = sip->next;
    if (sip->choice == SEQID_GI) {
      *previd = sip->next;
      sip->next = NULL;
      SeqIdFree (sip);
    } else {
      previd = (SeqIdPtr PNTR) &(sip->next);
    }
    sip = sip->next;
  }

  return TRUE;
}

void RemoveGIsFromBioseqs (IteM i)

{
  MsgAnswer    ans;
  BaseFormPtr  bfp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  ans = Message (MSG_OKC, "Currently deletes all GI SeqIDs");
  if (ans == ANS_CANCEL) return;

  GatherObjectsInEntity (bfp->input_entityID, 0, NULL, RemoveGIProc, NULL, NULL);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

/*#ifdef USE_BLAST*/
#define POST_PROCESS_BANDALGN  1
#define POST_PROCESS_NONE      2

typedef struct vecscreendata {
  CharPtr              database;
  CharPtr              program;
  Uint1                align_type;
#ifdef USE_BLAST3
  BLAST_OptionsBlkPtr  options;
#else
  CharPtr              options;
#endif
  Boolean              nucOK;
  Boolean              protOK;
  FILE                 *fp;
  MonitorPtr           mon;
  CharPtr              organism;
  Boolean              filterAgainst;
  Boolean              filterRepeats;
  Int2                 postProcess;
  SeqEntryPtr          topsep;
#ifdef USE_BLAST3
  BlastNet3Hptr        bl3hp;
  BlastResponsePtr     response;
#endif
} VecScreenData, PNTR VecScreenPtr;

#define MAX_BLAST_SEQ_LEN 100000

static MonitorPtr mon = NULL;

#ifdef USE_BLAST3
static Boolean LIBCALLBACK VectorScreenCallback (BlastResponsePtr brp, Boolean PNTR cancel)

{
  BlastProgressPtr  bpp;
  Int4              completed = 0;
  Boolean           retval;

  *cancel=FALSE;
  switch (brp->choice) {
    case BlastResponse_start :
      if (mon != NULL) {
        MonitorFree (mon);
        mon = NULL;
      }
      bpp = (BlastProgressPtr) brp->data.ptrvalue;
      if (bpp != NULL) {
        completed = bpp->completed;
      }
      if (completed == 0) {
        completed = 100;
      }
      mon = MonitorIntNew ("BLAST", 0, completed);
      return TRUE;
    case BlastResponse_progress :
      if (mon != NULL) {
        bpp = (BlastProgressPtr) brp->data.ptrvalue;
        if (bpp != NULL) {
          completed = bpp->completed;
        }
        retval = MonitorIntValue (mon, completed);
        if (retval == FALSE) {
          *cancel = TRUE;
          MonitorFree (mon);
          mon = NULL;
          return FALSE;
        }
      }
      return TRUE;
    case BlastResponse_queued :
      return TRUE;
    case BlastResponse_done :
      if (mon != NULL) {
        MonitorFree (mon);
        mon = NULL;
      }
      return TRUE;
    default :
      break;
  }
  return FALSE;
}

void VectorScreenProc (IteM i)

{
  Message (MSG_ERROR, "Vector screen not implemented for blast3");
}

#else

static void ShowVectorBlastReport (BLAST0ResultPtr result,
                                   BLAST0ResponsePtr blastResp,
                                   CharPtr program, FILE *fp)

{
  BlastReportStructPtr blrp;

  if (result == NULL || blastResp == NULL || program == NULL || fp == NULL) return;
  if (result->hitlists == NULL) return;

  blrp = TraditionalBlastReportSetUp (result, blastResp, program, fp, TEXT_OUTPUT);
  if (blrp == NULL) return;

  if (!TraditionalHeadBlastOutput (blrp)) return;

  if (TraditionalTopBlastOutput (blrp)) {
    TraditionalBlastWarning (blrp);
    if (! TraditionalBottomBlastOutput (blrp)) return;
  }

  blrp = TraditionalBlastReportCleanUp (blrp);
}

static Boolean LIBCALLBACK VectorScreenCallback (BLAST0ResponsePtr brp, Boolean PNTR cancel)

{
  BLAST0JobDescPtr      jdescp;
  BLAST0JobProgressPtr  jprogp;
  Boolean               retval;

  *cancel=FALSE;
  switch (brp->choice) {
    case BLAST0Response_job_start :
      jdescp = (BLAST0JobDescPtr) brp->data.ptrvalue;
      if (jdescp == NULL) return TRUE;
      if (mon != NULL) {
        MonitorFree (mon);
        mon = NULL;
      }
      mon = MonitorIntNew (jdescp->desc == NULL ? "BLAST" : jdescp->desc, 0, jdescp->size);
      return TRUE;
    case BLAST0Response_job_progress :
      if (mon != NULL) {
        jprogp = (BLAST0JobProgressPtr) brp->data.ptrvalue;
        if (jprogp != NULL) {
          retval = MonitorIntValue(mon, jprogp->done);
          if (retval == FALSE) {
            *cancel = TRUE;
            MonitorFree (mon);
            mon = NULL;
            return FALSE;
          }
        }
      }
      return TRUE;
    case BLAST0Response_job_done :
      if (mon != NULL) {
        MonitorFree (mon);
        mon = NULL;
      }
      return TRUE;
    default :
      break;
  }
  return FALSE;
}

static void VecBlastProc (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)

{
  BLAST0ResponsePtr  blastResp;
  ByteStorePtr       bp;
  BLAST0ResponsePtr  brp1;
  BioseqPtr          bsp;
  Bioseq             fake;
  BioseqPtr          fakeptr;
  SeqLocPtr          mask_slp;
  Int4               offset;
  Int2               residue;
  Int4               res_index;
  BLAST0ResultPtr    result;
  SeqPortPtr         spp;
  Int4               the_len;
  VecScreenPtr       vsp;

  if (! IS_Bioseq (sep)) return;
  vsp = (VecScreenPtr) data;
  if (vsp == NULL) return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (! ISA_na (bsp->mol)) return;
  if (bsp->repr != Seq_repr_raw && bsp->repr != Seq_repr_const) return;
  blastResp = NULL;
  if (bsp->length <= MAX_BLAST_SEQ_LEN) {
    mask_slp = BioseqDust (bsp, 0, (bsp->length - 1), -1, -1, -1, -1);
    MonitorStrValue (vsp->mon, "BlastBioseq");
    Update ();
    result = BlastBioseq (bsp, vsp->program, vsp->database, vsp->options, &blastResp,
                          mask_slp, 0, VectorScreenCallback);
    MonitorStrValue (vsp->mon, "ShowVectorBlastReport");
    Update ();
    ShowVectorBlastReport (result, blastResp, vsp->program, vsp->fp);
    MonitorStrValue (vsp->mon, "BLAST0ResultFree");
    Update ();
    BLAST0ResultFree (result);
    while (blastResp != NULL) {
      brp1 = blastResp;
      blastResp = blastResp->next;
      brp1->next = NULL;
      BLAST0ResponseFree (brp1);
    }
    mask_slp = SeqLocSetFree (mask_slp);
  } else {
    the_len = MAX_BLAST_SEQ_LEN /2;
    bp = BSNew ((Uint4) the_len);
    spp = SeqPortNew (bsp, 1, -1, 0, Seq_code_iupacna);
    fakeptr = &fake;
    MemSet ((Pointer) fakeptr, 0, sizeof (Bioseq));
    fakeptr->length = (Int4) the_len;
    fakeptr->seq_data = bp;
    fakeptr->repr = Seq_repr_raw;
    fakeptr->mol = bsp->mol;
    fakeptr->seq_data_type = Seq_code_iupacna;
    SeqPortSeek (spp, 0, SEEK_SET);
    BSSeek (bp, 0, SEEK_SET);
    for (res_index = 0; res_index < the_len; res_index++) {
      residue = SeqPortGetResidue (spp);
      BSPutByte (bp, residue);
    }

    mask_slp = BioseqDust (fakeptr, 0, (fakeptr->length - 1), -1, -1, -1, -1);
    MonitorStrValue (vsp->mon, "BlastBioseq");
    Update ();
    result = BlastBioseq (fakeptr, vsp->program, vsp->database, vsp->options, &blastResp,
                          mask_slp, 0, VectorScreenCallback);
    MonitorStrValue (vsp->mon, "ShowVectorBlastReport");
    Update ();
    ShowVectorBlastReport (result, blastResp, vsp->program, vsp->fp);
    MonitorStrValue (vsp->mon, "BLAST0ResultFree");
    Update ();
    BLAST0ResultFree (result);
    while (blastResp != NULL) {
      brp1 = blastResp;
      blastResp = blastResp->next;
      brp1->next = NULL;
      BLAST0ResponseFree (brp1);
    }
    mask_slp = SeqLocSetFree (mask_slp);

    bp = fakeptr->seq_data;
    offset = bsp->length - (Int4) the_len - 1;
    SeqPortSeek (spp, offset, SEEK_SET);
    BSSeek(bp, 0, SEEK_SET);
    for (res_index = 0; res_index < the_len; res_index++) {
      residue = SeqPortGetResidue (spp);
      BSPutByte (bp, residue);
    }

    mask_slp = BioseqDust (fakeptr, 0, (fakeptr->length - 1), -1, -1, -1, -1);
    MonitorStrValue (vsp->mon, "BlastBioseq");
    Update ();
    result = BlastBioseq (fakeptr, vsp->program, vsp->database, vsp->options, &blastResp,
                          mask_slp, 0, VectorScreenCallback);
    MonitorStrValue (vsp->mon, "ShowVectorBlastReport");
    Update ();
    ShowVectorBlastReport (result, blastResp, vsp->program, vsp->fp);
    MonitorStrValue (vsp->mon, "BLAST0ResultFree");
    Update ();
    BLAST0ResultFree (result);
    while (blastResp != NULL) {
      brp1 = blastResp;
      blastResp = blastResp->next;
      brp1->next = NULL;
      BLAST0ResponseFree (brp1);
    }
    mask_slp = SeqLocSetFree (mask_slp);

    bp = fakeptr->seq_data;
    BSFree (bp);
    SeqPortFree (spp);
  }
}

static void VectorAndMitoScreenSeqEntry (SeqEntryPtr sep, MonitorPtr mon)

{
  Boolean        mito;
  Char           path [PATH_MAX];
  VecScreenData  vsd;

  if (sep == NULL) return;
  TmpNam (path);
  vsd.fp = FileOpen (path, "w");
  if (vsd.fp == NULL) return;
  vsd.mon = mon;
  vsd.database = "vector";
  vsd.program = "blastn";
  vsd.options = "M=1 N=-3 S=20 S2=20 V=10 B=10";
  vsd.nucOK = TRUE;
  vsd.protOK = FALSE;
  MonitorStrValue (mon, "Performing Vector Search");
  Update ();
  SeqEntryExplore (sep, (Pointer) (&vsd), VecBlastProc);
  SeqEntryToGeneticCode (sep, &mito, NULL, 0);
  if (! mito) { 
    vsd.database = "mito";
    MonitorStrValue (mon, "Performing Mitochondrial Search");
    Update ();
    SeqEntryExplore (sep, (Pointer) (&vsd), VecBlastProc);
  }
  FileClose (vsd.fp);
  if (FileLength (path) > 0) {
    MonitorStrValue (mon, "Displaying Report");
    Update ();
    LaunchGeneralTextViewer (path, "Sequin Vector/Mito Screen Errors");
  } else if (mito) {
    Message (MSG_OK, "No vector sequences detected.");
  } else {
    Message (MSG_OK, "No vector or mitochondrial sequences detected.");
  }
  FileRemove (path);
}

void VectorScreenProc (IteM i)

{
  BaseFormPtr  bfp;
  MonitorPtr   mon;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = (BaseFormPtr) currentFormDataPtr;
#else
  bfp = (BaseFormPtr) GetObjectExtra (i);
#endif
  if (bfp != NULL) {
    sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
    if (sep != NULL) {
      SendHelpScrollMessage (helpForm, "Search Menu", "Vector Screen");
      WatchCursor ();
      Update ();
      mon = MonitorStrNewEx ("VectorScreen", 40, FALSE);
      MonitorStrValue (mon, "Connecting to BLAST");
      Update ();
      if (! BlastInit ("Sequin", FALSE)) {
        ArrowCursor ();
        MonitorFree (mon);
        Update ();
        Message (MSG_ERROR, "Unable to initialize BLAST service.");
        Update ();
        return;
      }
      VectorAndMitoScreenSeqEntry (sep, mon);
      MonitorStrValue (mon, "Closing BLAST");
      Update ();
      BlastFini ();
      MonitorFree (mon);
      ArrowCursor ();
      Update ();
    }
  }
}
#endif /* USE_BLAST3 */

static Boolean load_blast_score_for_sim (SeqAlignPtr blast_sap, SeqAlignPtr sim_sap)
{
	Int4 b_score, s_score;
	DenseSegPtr dsp;
	DenseDiagPtr ddp;

	if(blast_sap->segtype == 1)
	{
		ddp = blast_sap->segs;
		dsp = sim_sap->segs;
		b_score = get_score_value(ddp->scores);
		s_score = get_score_value(dsp->scores);

		if(s_score == -1 || b_score > s_score)
		{
			if(dsp->scores != NULL)
				ScoreSetFree(dsp->scores);
			dsp->scores = ddp->scores;
			ddp->scores = NULL;
			return TRUE;
		}
	}
	return FALSE;
}

static Boolean replace_with_sim_sap(SeqAnnotPtr sap, SeqAnnotPtr sim_sap)
{
	SeqAlignPtr sim_align, align, prev, next;
	DenseSegPtr dsp;
	DenseDiagPtr ddp;
	Boolean found;

	if(sap == NULL || sim_sap == NULL)
		return FALSE;

	if(sim_sap->data == NULL)
	{
		SeqAnnotFree(sim_sap);
		return FALSE;
	}

	align = (SeqAlignPtr)(sap->data);
	prev = NULL;
	while(align)
	{
		next = align->next;
		found = FALSE;
		if(align->segtype == 1)
		{
			ddp = align->segs;
			for(sim_align = (SeqAlignPtr)(sim_sap->data); 
				sim_align != NULL; sim_align = sim_align->next)
			{
				dsp = sim_align->segs;
				if(SeqIdMatch(dsp->ids->next, ddp->id->next))
				{
					/* load_blast_score_for_sim (align, sim_align); */
					found = TRUE;
					break;
				}
			}
		}
		if(found)
		{
			if(prev == NULL)
				sap->data = next;
			else
				prev->next = next;
			SeqAlignFree(align);
		}
		else
			prev = align;
		align = next;
	}

	sap->data = link_align((SeqAlignPtr)(sap->data), (SeqAlignPtr)(sim_sap->data));
	sim_sap->data = NULL;
	SeqAnnotFree(sim_sap);
	return TRUE;
}

/* PowBlastProc now modified with functions from Jinghui Zhang's PowerBLAST */

#define MAX_BLASTN_LEN 8000
#define MAX_BLASTP_LEN 4000
#define MAX_BLASTX_LEN 3000
#define MAX_TBLASTN_LEN 2000
#define OVERLAP_SPACE 1000
#define MAX_GI_NUM 10000

static ByteStorePtr get_gi_list(CharPtr tax_name, Uint1 mol, Int4Ptr count)
{
	Char str[200];
	DocType db;

	db = (mol == Seq_mol_aa) ? TYP_AA : TYP_NT;
	sprintf(str, "\"%s\"[ORGN]", tax_name);
	*count = EntrezTLEvalCountString(str, db, -1, NULL, NULL);
	if(*count > 0 && *count < MAX_GI_NUM)
		return EntrezTLEvalXString(str, db, -1, NULL, NULL);
	else
		return NULL;
}

static void link_two_annot(SeqAnnotPtr PNTR head, SeqAnnotPtr new_sap)
{
	SeqAnnotPtr curr;

	if(new_sap == NULL)
		return;
	if(*head == NULL)
		*head = new_sap;
	else
	{
		curr = *head;
		curr->data = link_align((SeqAlignPtr)(new_sap->data), (SeqAlignPtr)(curr->data));
		new_sap->data = NULL;
		SeqAnnotFree(new_sap);
	}
}

static void link_annot_to_end(SeqAnnotPtr PNTR head, SeqAnnotPtr new_sap)
{
	SeqAnnotPtr sap;

	if(*head == NULL)
		*head = new_sap;
	else
	{
		sap = *head;
		while(sap->next != NULL)
			sap = sap->next;
		sap->next = new_sap;
	}
}

static Boolean bin_search_gi(Int4 low, Int4 high, Int4 gi, ByteStorePtr gi_bsp)
{
	Int4 mid;
	Int4 value;

	if(low > high)
		return FALSE;
	mid = (low + high)/2;
	BSSeek(gi_bsp, mid *4, SEEK_SET);
	BSRead(gi_bsp, &value, 4);
	if(gi == value)
		return TRUE;
	if(gi < value)
		return bin_search_gi(low, mid-1, gi, gi_bsp);
	else
		return bin_search_gi(mid+1, high, gi, gi_bsp);
}
		
static Boolean is_gi_in_list(Int4 gi, ByteStorePtr gi_bsp)
{
	Int4 size;

	if(gi_bsp == NULL)
		return TRUE;
	size = gi_bsp->totlen;
	if(size == 0)
		return FALSE;
	if(size%4 != 0)
	{
		Message(MSG_ERROR, "Incorrect size for ByteStore");
		return FALSE;
	}

	return bin_search_gi(0, (size/4-1), gi, gi_bsp);
}

static void filter_org_pass (SeqAnnotPtr sap, ByteStorePtr ngi_bsp,
                             BioseqPtr bsp, Boolean filterAgainst)

{
	SeqAlignPtr align, prev, next;
	DenseDiagPtr ddp;
	StdSegPtr ssp;
	DenseSegPtr dsp;
	SeqIdPtr sip;
	Boolean keep;

	if (sap == NULL || ngi_bsp == NULL) return;

	align = sap->data;
	prev = NULL;
	while(align)
	{
		next = align->next;
		keep = TRUE;
		sip = NULL;
		switch(align->segtype)
		{
			case 1:
				ddp = align->segs;
				sip = ddp->id->next;
				break;
			case 2:
				dsp = align->segs;
				sip = dsp->ids->next;
				break;
			case 3:
				ssp = align->segs;
				sip = SeqLocId(ssp->loc->next);
				break;
			default:
				break;
		}
		if(sip != NULL && sip->choice == SEQID_GI)
		{
			if(!BioseqMatch(bsp, sip))
			{
				keep = is_gi_in_list(sip->data.intvalue, ngi_bsp);
				if (filterAgainst)
					keep = 1 - keep;
			}
		}
		if(keep)
			prev = align;
		else
		{
			if(prev)
				prev->next = align->next;
			else
				sap->data = align->next;
			align->next = NULL;
			SeqAlignFree(align);
		}
		align = next;
	}
}

static Int4  pblst_count = 0;

static void do_filter_organism (SeqAnnotPtr blast_sap, BioseqPtr bsp, CharPtr organism,
                                Uint1 align_type, ValNodePtr head, Boolean filterAgainst)

{
  DocType       db;
  ByteStorePtr  gi_bsp;
  Int2          i;
  Char          name [201];
  Int4          num;
  SeqAnnotPtr   sap;
  Char          str [201];
  Int4Ptr       uids;
  ValNodePtr    vnp;

  if (blast_sap == NULL || bsp == NULL || StringHasNoText (organism) || head == NULL) return;
  num = 0;
  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == align_type) {
      num++;
    }
  }
  if (num > 0) {
    uids = MemNew ((size_t) (num + 1) * sizeof (DocUid));
    if (uids != NULL) {
      i = 0;
      for (vnp = head; i < num && vnp != NULL; vnp = vnp->next) {
        if (vnp->choice == align_type) {
          uids [i] = vnp->data.intvalue;
          i++;
        }
      }
    }

    switch (align_type) {
      case ALIGN_BLASTN:
        db = TYP_NT;
        break;
      case ALIGN_BLASTP:
        db = TYP_AA;
        break;
      case ALIGN_BLASTX:
        db = TYP_AA;
        break;
      case ALIGN_TBLASTN:
        db = TYP_NT;
        break;
      default:
        db = TYP_NT;
        break;
    }
    pblst_count++;
    sprintf (name, "*sqnpblast%ld", (long) pblst_count);
    EntrezCreateNamedUidList (name, db, FLD_WORD, num, uids);
    sprintf (str, "%s [WORD] & \"%s\" [ORGN]", name, organism);
    gi_bsp = EntrezTLEvalXString (str, db, -1, NULL, NULL);
    MemFree (uids);

    if (gi_bsp != NULL) {
      for (sap = blast_sap; sap != NULL; sap = sap->next) {
        if (sap->type == 2) {
          filter_org_pass (sap, gi_bsp, bsp, filterAgainst);
        }
      }
    }
    BSFree (gi_bsp);
  }
}

static void PowBlastProc (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)

{
#ifdef USE_BLAST3
  PSeqAlignInfoPtr   alip;
  ValNodePtr         error_returns;
  PSeqAlignInfoPtr   head_alip = NULL;
  PSeqAlignInfoPtr   head_t_alip = NULL;
  Int4               i;
  Int4               len1;
  Int4               num;
  ValNodePtr         other_returns;
  Int4               pstop;
  SeqIntPtr          sint;
  SeqLocPtr          slp1;
  Boolean            split = FALSE;
  PSeqAlignInfoPtr   t_alip;
#else
  BLAST0ResponsePtr  blastResp;
  BLAST0ResponsePtr  brp1;
  SeqAnnotPtr        t_sap;
#endif
  BioseqPtr          bsp;
  SeqAnnotPtr        curr;
  ValNodePtr         head;
  MonitorPtr         mon = NULL;
  SeqAlignPtr        salp;
  SeqAnnotPtr        sap;
  SeqAnnotPtr PNTR   sapp;
  SeqAnnotPtr        sim_sap;
  Char               str [256];
  VecScreenPtr       vsp;

  BioseqPtr dust_bsp;
  SeqLocPtr blastloc, cloc, filterloc, dustloc;
  SeqLocPtr slp;
  SeqAnnotPtr blast_sap, n_sap;
  Int4 max_len, overlap_len;

  if (! IS_Bioseq (sep)) return;
  vsp = (VecScreenPtr) data;
  if (vsp == NULL) return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL) return;
  if (vsp->nucOK && (! ISA_na (bsp->mol))) return;
  if (vsp->protOK && (! ISA_aa (bsp->mol))) return;
  if (bsp->repr != Seq_repr_raw && bsp->repr != Seq_repr_const) return;
  sapp = &(bsp->annot);
  sap = NULL;
#ifndef USE_BLAST3
  blastResp = NULL;
#endif
  slp = SeqLocIntNew (0, bsp->length-1, Seq_strand_both, bsp->id);

  filterloc = NULL;

  if (ISA_na (bsp->mol)) {
    blastloc = break_blast_job (slp, bsp->id, MAX_BLASTN_LEN, 100);
    for (cloc = blastloc; cloc != NULL; cloc = cloc->next) {
      dustloc = SeqLocDust (cloc, -1, 2, -1, -1);
      ValNodeLink (&filterloc, dustloc);
    }
    SeqLocSetFree (blastloc);
  }

  dust_bsp = make_dust_bsp (bsp, 0, (bsp->length-1), filterloc);
  SeqLocSetFree (filterloc);
  if (dust_bsp == NULL) return;
  if (vsp->filterRepeats) {
    mask_with_repeat_feature (bsp, vsp->topsep, dust_bsp);
  }

  overlap_len = OVERLAP_SPACE;
  switch (vsp->align_type) {
    case ALIGN_BLASTN :
      max_len = MAX_BLASTN_LEN;
      break;
    case ALIGN_BLASTP :
      max_len = MAX_BLASTP_LEN;
      break;
    case ALIGN_BLASTX :
      max_len = MAX_BLASTX_LEN;
      break;
    case ALIGN_TBLASTN :
      max_len = MAX_TBLASTN_LEN;
      break;
    default :
      max_len = 2000;
      break;
  }

  blast_sap = NULL;
  n_sap = NULL;
  sap = NULL;
#ifdef USE_BLAST3
  error_returns = NULL;
  other_returns = NULL;
  len1 = SeqLocLen(slp);
  if(len1 == 0)
     return;
  if(len1 > 10000 && len1 > max_len)
     split = TRUE;
  alip = NULL;
  if(split)
    {
       num = (len1 - overlap_len)/(max_len - overlap_len);
       if((len1 - overlap_len)%(max_len - overlap_len) > max_len/2)
          ++num;
       slp1 = SeqLocIntNew(SeqLocStart(slp), SeqLocStop(slp),
                  SeqLocStrand(slp), SeqLocId(slp));
       sint = (SeqIntPtr) slp1->data.ptrvalue;
       pstop = -1;
       for(i = 0; i<num; ++i)
          {
              if(i == 0)
              {
                  sint->from = SeqLocStart(slp);
                  sint->to = sint->from + max_len -1;
              }
              else
              {
                  sint->from = sint->to -overlap_len;
                  sint->to = MIN(sint->from + max_len -1, SeqLocStop(slp));
              }
              if(i == num -1)
                  sint->to = SeqLocStop(slp);
                  sint->strand = SeqLocStrand(slp);
                  salp = BlastSeqLocNet(vsp->bl3hp, slp1, vsp->program, vsp->database, vsp->options, &other_returns, &error_returns, VectorScreenCallback);
                  if(salp != NULL)
                  {
                     t_alip = SeqAlignToPSeqAlignInfo(salp);
                     if(alip == NULL)
                     {
                        alip = t_alip;
                        head_alip = alip;
                     }
                     else if(pstop != -1)
                     {
                        alip = head_alip;
                        head_t_alip = t_alip;
                        while (alip != NULL)
                           {
                           t_alip = head_t_alip;
                           while (t_alip != NULL)
                           {
                              if (SeqIdComp(t_alip->sip->next, alip->sip->next)
                                                  && t_alip->used == FALSE)
                              {
                                 MergeTwoAlignList(alip->sap, &t_alip->sap,
                                                        sint->from, pstop, 0);
                                 if (t_alip->sap !=NULL)
                                 {
                                     SeqAlignLink(t_alip->sap, alip->sap);
                                 }
                                 t_alip->used = TRUE;
                              }
                              t_alip = t_alip->next;
                           }
                           alip = alip->next;
                        }
                        alip = head_alip;
                        while (alip->next != NULL)
                             alip = alip->next;
                        t_alip = head_t_alip;
                        while (t_alip)
                        {
                           if (t_alip->used == FALSE)
                           {
                               alip->next = t_alip;
                               t_alip = t_alip->next;
                               alip = alip->next;
                               alip->next = NULL;
                            } else
                               t_alip = t_alip->next;
                         }
                         t_alip = head_t_alip = NULL;
                      }
                      pstop = sint->to;
                   }
               }
               SeqLocFree(slp1);
               salp = ReassembleSeqAlignFromPSeqAlignInfo(head_alip);
        } else
        {
           salp = BlastSeqLocNet(vsp->bl3hp, slp, vsp->program, vsp->database, vsp->options, &other_returns, &error_returns, VectorScreenCallback);
        }
  sap = SeqAnnotForSeqAlign (salp);
#endif
#ifndef USE_BLAST3
  blastloc = break_blast_job (slp, dust_bsp->id, max_len, overlap_len);
  for (cloc = blastloc; cloc != NULL; cloc = cloc->next) {
    blastResp = NULL;
    n_sap = NULL;
    t_sap = BlastSeqLoc2 (cloc, vsp->program, vsp->database,
                          vsp->options, &blastResp, NULL, TRUE);
    if (t_sap != NULL) {
      link_two_annot (&n_sap, t_sap);
    }
    if (n_sap != NULL) {
      restore_blast_id (n_sap, SeqLocId(slp));
      SortAlignByLocation (n_sap);
      /* clean_all_internal_repeats ((SeqAlignPtr PNTR )(&(n_sap->data))); */
      merge_blast_result (&sap, n_sap);
      /* clean_all_internal_repeats ((SeqAlignPtr PNTR )(&(sap->data))); */
      /* clean_empty_seqalign ((SeqAlignPtr PNTR )(&(sap->data))); */
    }
    while (blastResp != NULL) {
      brp1 = blastResp;
      blastResp = blastResp->next;
      brp1->next = NULL;
      BLAST0ResponseFree (brp1);
    }
  }
  SeqLocSetFree (blastloc);
#endif
  if (sap != NULL) {
    AddAlignInfoToSeqAnnot (sap, vsp->align_type);
    link_annot_to_end (&blast_sap, sap);
  }
  BioseqFree (dust_bsp);

  Update ();
  mon = MonitorStrNewEx ("PowerBLAST", 40, FALSE);

  if (sapp != NULL && blast_sap != NULL) {
    if (! StringHasNoText (vsp->organism)) {
      sprintf (str, "Filter Hits for Organism %s", vsp->organism);
      MonitorStrValue (mon, str);
      Update ();
      head = NULL;
      for (sap = blast_sap; sap != NULL; sap = sap->next) {
        GetUidsForOneSeqAnnot (sap, &head, vsp->align_type);
      }
      if (head != NULL) {
        head = SortValNode (head, SortByVnpDataIntvalue);
        do_filter_organism (blast_sap, bsp, vsp->organism, vsp->align_type, head, vsp->filterAgainst);
      }
      ValNodeFree (head);
    }
  }

  MonitorStrValue (mon, "Processing BLAST results");
  Update ();

  if (sapp != NULL && blast_sap != NULL) {
    for (sap = blast_sap; sap != NULL; sap = sap->next) {
      filter_blast_query (sap, bsp);
      if (sap != NULL && sap->data != NULL) {
        sim_sap = NULL;
        switch (vsp->postProcess) {
          case POST_PROCESS_BANDALGN :
            salp=(SeqAlignPtr)sap->data;
            sim_sap = BlastBandAlignFromBlastSeqAlign (salp, FALSE);
            if (sim_sap) {
               salp = SeqAlignSetFree (salp);
               sap->data = sim_sap->data;
               sim_sap->data = NULL;
            } 
            break;
          /*
          case POST_PROCESS_SIM :
            if (vsp->align_type == 1) {
              sim_sap = sim_for_blast (sap, SeqIdFindBest (SeqLocId(slp), 0), RUN_SIM_2);
            } else if (vsp->align_type == 2) {
              sim_sap = sim_for_blast (sap, SeqIdFindBest (SeqLocId(slp), 0), RUN_SIM_1);
            }
            if (sim_sap != NULL) {
               replace_with_sim_sap (sap, sim_sap);
            }
            break;
          */
          case POST_PROCESS_NONE :
          default :
            break;
        }
      }
      if (*sapp != NULL) {
        curr = *sapp;
        while (curr->next != NULL) {
          curr = curr->next;
        }
        curr->next = sap;
      } else {
        *sapp = sap;
      }
    }
  }

  SeqLocFree (slp);
  MonitorFree (mon);
  Update ();
}

#define NUM_DBS 20

static CharPtr nucdbs [] = {
  "nr", "est", "sts", "month", "htgs", "vector", "mito", "kabat",
  "epd", "pdb", "yeast", "gss", "alu", "thc", NULL
};

static CharPtr protdbs [] = {
  "nr", "pdb", "swissprot", "month", "kabat", "yeast", "alu", NULL
};

typedef struct powblastform {
  FORM_MESSAGE_BLOCK
  SeqEntryPtr    sep;
  ButtoN         programs [4];
#ifndef USE_BLAST3
  TexT           params [4];
#endif
  ButtoN         nucdatabases [NUM_DBS];
  ButtoN         protdatabases [NUM_DBS];
  PopuP          stringency;
  GrouP          postProcessing;
  TexT           organism;
  GrouP          filterAgainst;
  ButtoN         filterRepeats;
  ButtoN         accept;
} PowBlastForm, PNTR PowBlastFormPtr;

static void AddFilterSegToOptions (CharPtr options)

{
  if (StringStr (options, "-filter seg") != NULL) return;
  StringCat (options, " -filter seg");
}

static void PowBlastFormAcceptProc (ButtoN b)

{
#ifdef USE_BLAST3
  BlastNet3Hptr     bl3hp;
  BlastResponsePtr  response;
  Int2              stringency;
#else
  Char              options [128];
#endif
  Char              databases [128];
  Int2              i;
  MonitorPtr        mon;
  Int2              numDbs;
  PowBlastFormPtr   pfp;
  Int2              postProcess = POST_PROCESS_BANDALGN;
  VecScreenData     vsd;

  pfp = (PowBlastFormPtr) GetObjectExtra (b);
  if (pfp == NULL) return;
  WatchCursor ();
  Hide (pfp->form);
  Update ();

  mon = MonitorStrNewEx ("PowerBLAST", 40, FALSE);
  MonitorStrValue (mon, "Connecting to BLAST");
  Update ();

#ifdef USE_BLAST3
  if (! BlastInit ("Sequin", &bl3hp, &response)) {
#else
  if (! BlastInit ("Sequin", FALSE)) {
#endif
    ArrowCursor ();
    MonitorFree (mon);
    Update ();
    Message (MSG_ERROR, "Unable to initialize BLAST service.");
    Remove (pfp->form);
    Update ();
    return;
  }
  MonitorFree (mon);
  Update ();

  vsd.organism = SaveStringFromText (pfp->organism);
  vsd.filterAgainst = (Boolean) (GetValue (pfp->filterAgainst) == 2);
  vsd.filterRepeats = GetStatus (pfp->filterRepeats);
  vsd.topsep = pfp->sep;
#ifdef USE_BLAST3
  stringency = GetValue (pfp->stringency);
  vsd.bl3hp = bl3hp;
  vsd.response = response;
#endif
  if (pfp->postProcessing != NULL) {
    postProcess = GetValue (pfp->postProcessing);
  }

  if (GetStatus (pfp->programs [0])) {
#ifndef USE_BLAST3
    GetTitle (pfp->params [0], options, sizeof (options));
#endif
    databases [0] = '\0';
    numDbs = 0;
    for (i = 0; i < NUM_DBS && nucdbs [i] != NULL; i++) {
      if (GetStatus (pfp->nucdatabases [i])) {
        numDbs++;
        if (numDbs > 1) {
          StringCat (databases, " ");
        }
        StringCat (databases, nucdbs [i]);
      }
    }
    if (numDbs > 0) {
      vsd.fp = NULL;
      vsd.mon = NULL;
      vsd.database = databases;
      vsd.program = "blastn";
      vsd.align_type = ALIGN_BLASTN;
      vsd.nucOK = TRUE;
      vsd.protOK = FALSE;
#ifdef USE_BLAST3
      vsd.options = BLASTOptionNew (vsd.program, TRUE);
      if (vsd.options != NULL) {
        /* vsd.options->filter = FILTER_DUST; */
        vsd.options->filter_string = StringSave ("m D");
        switch (stringency) {
          case 1 :
            vsd.options->wordsize = 16;
            vsd.options->penalty = -5;
            vsd.options->reward = 1;
            vsd.options->expect_value = 0.000001;
            break;
          case 2 :
            vsd.options->wordsize = 11;
            vsd.options->expect_value = 1.0;
            break;
          case 3 :
            vsd.options->wordsize = 10;
            vsd.options->penalty = -2;
            vsd.options->reward = 1;
            vsd.options->expect_value = 10.0;
            break;
          default :
            vsd.options->wordsize = 11;
            break;
        }
      }
#else
      vsd.options = options;
#endif
      vsd.postProcess = postProcess;
      SeqEntryExplore (pfp->sep, (Pointer) (&vsd), PowBlastProc);
#ifdef USE_BLAST3
      vsd.options = BLASTOptionDelete (vsd.options);
#endif
    }
  }

  if (GetStatus (pfp->programs [3])) {
#ifndef USE_BLAST3
    GetTitle (pfp->params [3], options, sizeof (options));
    AddFilterSegToOptions (options);
#endif
    databases [0] = '\0';
    numDbs = 0;
    for (i = 0; i < NUM_DBS && protdbs [i] != NULL; i++) {
      if (GetStatus (pfp->protdatabases [i])) {
        numDbs++;
        if (numDbs > 1) {
          StringCat (databases, " ");
        }
        StringCat (databases, protdbs [i]);
      }
    }
    if (numDbs > 0) {
      vsd.fp = NULL;
      vsd.mon = NULL;
      vsd.database = databases;
      vsd.program = "blastp";
      vsd.align_type = ALIGN_BLASTP;
      vsd.nucOK = FALSE;
      vsd.protOK = TRUE;
#ifdef USE_BLAST3
      vsd.options = BLASTOptionNew (vsd.program, TRUE);
      if (vsd.options != NULL) {
        vsd.options->filter = FILTER_SEG;
        switch (stringency) {
          case 1 :
            vsd.options->threshold_first = 13;
            vsd.options->threshold_second = 13;
            vsd.options->expect_value = 0.0001;
            break;
          case 2 :
            vsd.options->threshold_first = 11;
            vsd.options->threshold_second = 11;
            vsd.options->expect_value = 1.0;
            break;
          case 3 :
            vsd.options->threshold_first = 10;
            vsd.options->threshold_second = 10;
            vsd.options->expect_value = 10.0;
            break;
          default :
            vsd.options->threshold_first = 11;
            vsd.options->threshold_second = 11;
            break;
        }
      }
#else
      vsd.options = options;
#endif
      vsd.postProcess = postProcess;
      SeqEntryExplore (pfp->sep, (Pointer) (&vsd), PowBlastProc);
#ifdef USE_BLAST3
      vsd.options = BLASTOptionDelete (vsd.options);
#endif
    }
  }

  if (GetStatus (pfp->programs [2])) {
#ifndef USE_BLAST3
    GetTitle (pfp->params [2], options, sizeof (options));
    AddFilterSegToOptions (options);
#endif
    databases [0] = '\0';
    numDbs = 0;
    for (i = 0; i < NUM_DBS && protdbs [i] != NULL; i++) {
      if (GetStatus (pfp->protdatabases [i])) {
        numDbs++;
        if (numDbs > 1) {
          StringCat (databases, " ");
        }
        StringCat (databases, protdbs [i]);
      }
    }
    if (numDbs > 0) {
      vsd.fp = NULL;
      vsd.mon = NULL;
      vsd.database = databases;
      vsd.program = "blastx";
      vsd.align_type = ALIGN_BLASTX;
      vsd.nucOK = TRUE;
      vsd.protOK = FALSE;
#ifdef USE_BLAST3
      vsd.options = BLASTOptionNew (vsd.program, TRUE);
      if (vsd.options != NULL) {
        vsd.options->filter = FILTER_SEG;
        switch (stringency) {
          case 1 :
            vsd.options->threshold_first = 14;
            vsd.options->threshold_second = 14;
            vsd.options->expect_value = 0.0001;
            break;
          case 2 :
            vsd.options->threshold_first = 12;
            vsd.options->threshold_second = 12;
            vsd.options->expect_value = 1.0;
            break;
          case 3 :
            vsd.options->threshold_first = 11;
            vsd.options->threshold_second = 11;
            vsd.options->expect_value = 10.0;
            break;
          default :
            vsd.options->threshold_first = 12;
            vsd.options->threshold_second = 12;
            break;
        }
      }
#else
      vsd.options = options;
#endif
      vsd.postProcess = postProcess;
      SeqEntryExplore (pfp->sep, (Pointer) (&vsd), PowBlastProc);
#ifdef USE_BLAST3
      vsd.options = BLASTOptionDelete (vsd.options);
#endif
    }
  }

  if (GetStatus (pfp->programs [1])) {
#ifndef USE_BLAST3
    GetTitle (pfp->params [1], options, sizeof (options));
    AddFilterSegToOptions (options);
#endif
    databases [0] = '\0';
    numDbs = 0;
    for (i = 0; i < NUM_DBS && nucdbs [i] != NULL; i++) {
      if (GetStatus (pfp->nucdatabases [i])) {
        numDbs++;
        if (numDbs > 1) {
          StringCat (databases, " ");
        }
        StringCat (databases, nucdbs [i]);
      }
    }
    if (numDbs > 0) {
      vsd.fp = NULL;
      vsd.mon = NULL;
      vsd.database = databases;
      vsd.program = "tblastn";
      vsd.align_type = ALIGN_TBLASTN;
      vsd.nucOK = FALSE;
      vsd.protOK = TRUE;
#ifdef USE_BLAST3
      vsd.options = BLASTOptionNew (vsd.program, TRUE);
      if (vsd.options != NULL) {
        vsd.options->filter = FILTER_SEG;
        switch (stringency) {
          case 1 :
            vsd.options->threshold_first = 15;
            vsd.options->threshold_second = 15;
            vsd.options->expect_value = 0.0001;
            break;
          case 2 :
            vsd.options->threshold_first = 13;
            vsd.options->threshold_second = 13;
            vsd.options->expect_value = 1.0;
            break;
          case 3 :
            vsd.options->threshold_first = 12;
            vsd.options->threshold_second = 12;
            vsd.options->expect_value = 10.0;
            break;
          default :
            vsd.options->threshold_first = 13;
            vsd.options->threshold_second = 13;
            break;
        }
      }
#else
      vsd.options = options;
#endif
      vsd.postProcess = postProcess;
      SeqEntryExplore (pfp->sep, (Pointer) (&vsd), PowBlastProc);
#ifdef USE_BLAST3
      vsd.options = BLASTOptionDelete (vsd.options);
#endif
    }
  }

  vsd.organism = MemFree (vsd.organism);

  mon = MonitorStrNewEx ("PowerBLAST", 40, FALSE);
  MonitorStrValue (mon, "Closing BLAST");
  Update ();
#ifdef USE_BLAST3
  BlastFini (bl3hp);
#else
  BlastFini ();
#endif
  MonitorFree (mon);

  Remove (pfp->form);
  ObjMgrSetDirtyFlag (pfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, pfp->input_entityID, 0, 0);
  ArrowCursor ();
  Update ();
}

static void PowBlastProgProc (ButtoN b)

{
  Int2             i;
  Boolean          okay;
  PowBlastFormPtr  pfp;

  pfp = (PowBlastFormPtr) GetObjectExtra (b);
  if (pfp == NULL) return;
  okay = FALSE;
  if (GetStatus (pfp->programs [0]) || GetStatus (pfp->programs [1])) {
    for (i = 0; i < NUM_DBS && nucdbs [i] != NULL; i++) {
      if (GetStatus (pfp->nucdatabases [i])) {
        okay = TRUE;
      }
    }
  }
  if (GetStatus (pfp->programs [2]) || GetStatus (pfp->programs [3])) {
    for (i = 0; i < NUM_DBS && protdbs [i] != NULL; i++) {
      if (GetStatus (pfp->protdatabases [i])) {
        okay = TRUE;
      }
    }
  }
  if (okay) {
    SafeEnable (pfp->accept);
  } else {
    SafeDisable (pfp->accept);
  }
}

static void ChangeStringency (PopuP p)

{
#ifndef USE_BLAST3
  CharPtr          options;
  PowBlastFormPtr  pfp;
  Int2             stringency;

  pfp = (PowBlastFormPtr) GetObjectExtra (p);
  if (pfp == NULL) return;
  stringency = GetValue (pfp->stringency);

  options = NULL;
  switch (stringency) {
    case 1 :
      options = "M=1 N=-3 S=40 S2=40 V=10 B=10";
      break;
    case 2 :
      options = "M=1 N=-2 E=1 V=10 B=10";
      break;
    case 3 :
      options = "M=1 N=-1 E=10 V=10 B=10";
      break;
    default :
      options = "M=1 N=-1 E=10 V=10 B=10";
      break;
  }
  SafeSetTitle (pfp->params [0], options);

  options = NULL;
  switch (stringency) {
    case 1 :
      options = "E=0.1 T=13 V=10 B=10";
      break;
    case 2 :
      options = "E=1 T=13 V=10 B=10";
      break;
    case 3 :
      options = "E=10 T=12 V=10 B=10";
      break;
    default :
      options = "E=10 T=12 V=10 B=10";
      break;
  }
  SafeSetTitle (pfp->params [1], options);

  options = NULL;
  switch (stringency) {
    case 1 :
      options = "E=0.1 T=13 V=10 B=10";
      break;
    case 2 :
      options = "E=1 T=13 V=10 B=10";
      break;
    case 3 :
      options = "E=10 T=12 V=10 B=10";
      break;
    default :
      options = "E=10 T=12 V=10 B=10";
      break;
  }
  SafeSetTitle (pfp->params [2], options);

  options = NULL;
  switch (stringency) {
    case 1 :
      options = "E=0.1 T=13 V=10 B=10";
      break;
    case 2 :
      options = "E=1 T=13 V=10 B=10";
      break;
    case 3 :
      options = "E=10 T=12 V=10 B=10";
      break;
    default :
      options = "E=10 T=12 V=10 B=10";
      break;
  }
  SafeSetTitle (pfp->params [3], options);
#endif
}

static void PowBlastMessageProc (ForM f, Int2 mssg)

{
  StdEditorProcsPtr  sepp;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    if (sepp->handleMessages != NULL) {
      sepp->handleMessages (f, mssg);
    }
  }
}

static void FindProtSeqEntryCallback (SeqEntryPtr sep, Pointer mydata,
                                      Int4 index, Int2 indent)

{
  BoolPtr    boolptr;
  BioseqPtr  bsp;

  if (sep != NULL && sep->choice == 1 && mydata != NULL) {
    boolptr = (BoolPtr) mydata;
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp != NULL && ISA_aa (bsp->mol)) {
      *boolptr = TRUE;
    }
  }
}

static ForM CreatePowBlastForm (Uint2 entityID, SeqEntryPtr sep)

{
  BioseqPtr        bsp;
  GrouP            c;
  GrouP            g;
  GrouP            h;
  Int2             i;
  GrouP            n = NULL;
  Boolean          nucOK = FALSE;
  GrouP            p = NULL;
  PowBlastFormPtr  pfp;
  PrompT           ppt1 = NULL, ppt2 = NULL, ppt3, ppt4, ppt5;
  Boolean          protOK = FALSE;
  GrouP            q;
  GrouP            r;
  GrouP            s;
  GrouP            t;
  WindoW           w;
  GrouP            x;
  GrouP            y;
  GrouP            z;

  w = NULL;
  pfp = MemNew (sizeof (PowBlastForm));
  if (pfp != NULL) {
    w = FixedWindow (-50, -33, -10, -10, "PowerBLAST", StdCloseWindowProc);
    SetObjectExtra (w, pfp, StdCleanupFormProc);
    pfp->form = (ForM) w;
    pfp->formmessage = PowBlastMessageProc;
    pfp->input_entityID = entityID;
    pfp->sep = sep;

    if (sep != NULL && sep->data.ptrvalue != NULL) {
      if (IS_Bioseq (sep)) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        nucOK = ISA_na (bsp->mol);
        protOK = ISA_aa (bsp->mol);
        if (nucOK && bsp->repr == Seq_repr_seg) {
          entityID = ObjMgrGetEntityIDForChoice (sep);
          sep = GetBestTopParentForData (entityID, bsp);
          pfp->sep = sep;
        }
      } else {
        nucOK = TRUE;
        protOK = FALSE;
        SeqEntryExplore (sep, (Pointer) (&protOK), FindProtSeqEntryCallback);
      }
    }

    h = HiddenGroup (w, -1, 0, NULL);
    SetGroupSpacing (h, 10, 10);

    if (nucOK || protOK) {

      ppt1 = StaticPrompt (h, "Nucleotide Search", 0, 0, systemFont, 'c');

      n = HiddenGroup (h, -1, 0, NULL);
      SetGroupSpacing (n, 7, 7);

#ifdef USE_BLAST3
      g = HiddenGroup (n, 1, 0, NULL);
#else
      g = HiddenGroup (n, 2, 0, NULL);
#endif
      SetGroupSpacing (g, 5, 5);
      StaticPrompt (g, "Program", 0, 0, programFont, 'c');
#ifndef USE_BLAST3
      StaticPrompt (g, "Parameters", 0, 0, programFont, 'c');
#endif
      pfp->programs [0] = CheckBox (g, "blastn", (BtnActnProc) PowBlastProgProc);
      SetObjectExtra (pfp->programs [0], pfp, NULL);
#ifndef USE_BLAST3
      pfp->params [0] = DialogText (g, "", 18, NULL);
#endif
      if (! nucOK) {
        Disable (pfp->programs [0]);
#ifndef USE_BLAST3
        Disable (pfp->params [0]);
#endif
      }
      pfp->programs [1] = CheckBox (g, "tblastn", (BtnActnProc) PowBlastProgProc);
      SetObjectExtra (pfp->programs [1], pfp, NULL);
#ifndef USE_BLAST3
      pfp->params [1] = DialogText (g, "", 18, NULL);
#endif
      if (! protOK) {
        Disable (pfp->programs [1]);
#ifndef USE_BLAST3
        Disable (pfp->params [1]);
#endif
      }

      t = HiddenGroup (n, 1, 0, NULL);
      SetGroupSpacing (t, 5, 5);
      StaticPrompt (t, "Databases", 0, 0, programFont, 'c');
      s = HiddenGroup (t, 7, 0, NULL);
      for (i = 0; i < NUM_DBS && nucdbs [i] != NULL; i++) {
        pfp->nucdatabases [i] = CheckBox (s, nucdbs [i], (BtnActnProc) PowBlastProgProc);
        SetObjectExtra (pfp->nucdatabases [i], pfp, NULL);
      }
      /* AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) s, NULL); */

    }

    if (nucOK || protOK) {

      ppt2 = StaticPrompt (h, "Protein Search", 0, 0, systemFont, 'c');

      p = HiddenGroup (h, -1, 0, NULL);
      SetGroupSpacing (p, 7, 7);

#ifdef USE_BLAST3
      g = HiddenGroup (p, 1, 0, NULL);
#else
      g = HiddenGroup (p, 2, 0, NULL);
#endif
      SetGroupSpacing (g, 5, 5);
      StaticPrompt (g, "Program", 0, 0, programFont, 'c');
#ifndef USE_BLAST3
      StaticPrompt (g, "Parameters", 0, 0, programFont, 'c');
#endif
      /* blastp [3] moved before blastx [2], array order unchanged */
      pfp->programs [3] = CheckBox (g, "blastp", (BtnActnProc) PowBlastProgProc);
      SetObjectExtra (pfp->programs [3], pfp, NULL);
#ifndef USE_BLAST3
      pfp->params [3] = DialogText (g, "", 18, NULL);
#endif
      if (! protOK) {
        Disable (pfp->programs [3]);
#ifndef USE_BLAST3
        Disable (pfp->params [3]);
#endif
      }
      pfp->programs [2] = CheckBox (g, "blastx", (BtnActnProc) PowBlastProgProc);
      SetObjectExtra (pfp->programs [2], pfp, NULL);
#ifndef USE_BLAST3
      pfp->params [2] = DialogText (g, "", 18, NULL);
#endif
      if (! nucOK) {
        Disable (pfp->programs [2]);
#ifndef USE_BLAST3
        Disable (pfp->params [2]);
#endif
      }

      t = HiddenGroup (p, 1, 0, NULL);
      SetGroupSpacing (t, 5, 5);
      StaticPrompt (t, "Databases", 0, 0, programFont, 'c');
      s = HiddenGroup (t, 7, 0, NULL);
      for (i = 0; i < NUM_DBS && protdbs [i] != NULL; i++) {
        pfp->protdatabases [i] = CheckBox (s, protdbs [i], (BtnActnProc) PowBlastProgProc);
        SetObjectExtra (pfp->protdatabases [i], pfp, NULL);
      }
      /* AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) s, NULL); */

    }

    ppt3 = StaticPrompt (h, "Options", 0, 0, systemFont, 'c');

    q = HiddenGroup (h, -1, 0, NULL);
    SetGroupSpacing (q, 5, 5);

    t = HiddenGroup (q, 4, 0, NULL);
    pfp->filterAgainst = HiddenGroup (t, 1, 0, NULL);
    /*
    RadioButton (pfp->filterAgainst, "Restrict to");
    RadioButton (pfp->filterAgainst, "Filter against");
    */
    RadioButton (pfp->filterAgainst, "Require");
    RadioButton (pfp->filterAgainst, "Exclude");
    SetValue (pfp->filterAgainst, 1);
    z = HiddenGroup (t, 2, 0, NULL);
    StaticPrompt (z, "Organism", 0, dialogTextHeight, programFont, 'l');
    pfp->organism = DialogText (z, "", 14, NULL);
    AlignObjects (ALIGN_MIDDLE, (HANDLE) pfp->filterAgainst, (HANDLE) z, NULL);

    x = HiddenGroup (h, 2, 0, NULL);
    pfp->filterRepeats = CheckBox (x, "Filter repeat feature regions", NULL);
    SetStatus (pfp->filterRepeats, TRUE);

    y = NULL;
    if (indexerVersion) {
      y = HiddenGroup (h, 2, 0, NULL);
      ppt5 = StaticPrompt (y, "Post-process merging", 0, 0, programFont, 'l');
      pfp->postProcessing = HiddenGroup (y, 3, 0, NULL);
      RadioButton (pfp->postProcessing, "BANDALIGN");
      /* RadioButton (pfp->postProcessing, "SIM"); */
      RadioButton (pfp->postProcessing, "None");
      SetValue (pfp->postProcessing, 1);
      AlignObjects (ALIGN_MIDDLE, (HANDLE) ppt5, (HANDLE) pfp->postProcessing, NULL);
    }

    r = HiddenGroup (h, 2, 0, NULL);
    ppt4 = StaticPrompt (r, "Sensitivity", 0, 0, programFont, 'l');
    pfp->stringency = PopupList (r, TRUE, ChangeStringency);
    SetObjectExtra (pfp->stringency, pfp, NULL);
    PopupItem (pfp->stringency, "Stringent");
    PopupItem (pfp->stringency, "Normal");
    PopupItem (pfp->stringency, "Relaxed");
    SetValue (pfp->stringency, 2);
    AlignObjects (ALIGN_MIDDLE, (HANDLE) ppt4, (HANDLE) pfp->stringency, NULL);

    c = HiddenGroup (w, 4, 0, NULL);
    pfp->accept = DefaultButton (c, "Accept", PowBlastFormAcceptProc);
    SetObjectExtra (pfp->accept, pfp, NULL);
    Disable (pfp->accept);
    PushButton (c, "Cancel", StdCancelButtonProc);

    AlignObjects (ALIGN_CENTER, (HANDLE) ppt1, (HANDLE) ppt2, (HANDLE) ppt3,
                  (HANDLE) r, (HANDLE) q, (HANDLE) n, (HANDLE) p,
                  (HANDLE) x, (HANDLE) c, (HANDLE) y, NULL);

    RealizeWindow (w);
    ChangeStringency (pfp->stringency);
    Select (pfp->organism);
  }
  return (ForM) w;
}

void SimplePowerBlastProc (IteM i)

{
  BaseFormPtr  bfp;
  BioseqPtr    bsp;
  ForM         f;
  SeqEntryPtr  sep = NULL;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  bsp =  GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
  if (bsp != NULL) {
    sep = SeqMgrGetSeqEntryForData (bsp);
  } else {
    sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  }
  if (sep == NULL) return;

  f = CreatePowBlastForm (bfp->input_entityID, sep);
  if (f == NULL) return;
  Show (f);
  Select (f);
}

typedef struct blastfields {
  BlastNet3Hptr        bl3hp;
  BLAST_OptionsBlkPtr  options;
} BlastFields, PNTR BlastFieldsPtr;

#define EXPECT_VALUE 0.01

static void BlastCDD (BioseqPtr bsp, Pointer userdata)

{
  BlastFieldsPtr       bfp;
  BlastNet3Hptr        bl3hp;
  BLAST_OptionsBlkPtr  options;
  ValNodePtr           error_returns = NULL;
  SeqAlignPtr          salp;

  if (! ISA_aa (bsp->mol)) return;
  bfp = (BlastFieldsPtr) userdata;
  bl3hp = bfp->bl3hp;
  options = bfp->options;
  if (bl3hp == NULL) return;

  /* do blast search */

  salp = BlastBioseqNet (bl3hp, bsp, "blastp", "oasis_sap", options,
                         NULL, &error_returns, NULL);

  /* BlastErrorPrintExtra (error_returns, TRUE, stdout); */

  /* annotation function now moved to rpsutil in toolkit for common use */

  AnnotateRegionsFromCDD (bsp, salp, EXPECT_VALUE);

  /* clean up */

  SeqAlignSetFree (salp);
}

extern void SimpleCDDBlastProc (IteM i)

{
  BaseFormPtr          bfp;
  SeqEntryPtr          sep;
  BlastFields          bf;
  BlastNet3Hptr        bl3hp = NULL;
  BLAST_OptionsBlkPtr  options = NULL;
  BlastResponsePtr     response = NULL;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  TransientSetAppParam ("NCBI", "NETBLAST", "SERVICE_NAME", "rpsblast");

  if (! BlastInit ("Sequin", &bl3hp, &response)) return;

  response = BlastResponseFree (response); 
  options = BLASTOptionNew ("blastp", TRUE);
  if (options == NULL) return;

  options->is_rps_blast = TRUE;
  options->filter = FILTER_SEG;
  options->expect_value  = (Nlm_FloatHi) EXPECT_VALUE;
  options->hitlist_size = 5000;

  /* blast fetch enable needed to retrieve by general SeqID */

  BlastNetBioseqFetchEnable (bl3hp, "oasis_sap", FALSE, TRUE);

  bf.bl3hp = bl3hp;
  bf.options = options;

  WatchCursor ();
  Update ();

  FreeCDDRegions (sep);

  VisitBioseqsInSep (sep, (Pointer) &bf, BlastCDD);

  RemoveDuplicateCDDs (sep);

  BlastFini (bl3hp);
  options = BLASTOptionDelete (options);
  BlastNetBioseqFetchDisable (bl3hp, "oasis_sap", FALSE);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  ArrowCursor ();
  Update ();
}
/*#endif*/

typedef struct autoparseform {
  FORM_MESSAGE_BLOCK
  SeqEntryPtr        sep;
  BioseqPtr          target;
  PopuP PNTR         fields;
  Int2 PNTR          tags;
  Int4Ptr            counts;
  ValNodePtr         table;
  Int2               cols;
  Int2               majority;
  GrouP              strandStyle;
  GrouP              acStyle;
  Boolean            updateExisting;
  Boolean            qualifierStyle;
  EnumFieldAssocPtr  alist;
} AutoParseForm, PNTR AutoParseFormPtr;

static void AutoParseMessageProc (ForM f, Int2 mssg)

{
  StdEditorProcsPtr  sepp;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    if (sepp->handleMessages != NULL) {
      sepp->handleMessages (f, mssg);
    }
  }
}

#define GENE_NAME_TAG           1
#define PROTEIN_NAME_TAG        2
#define MRNA_NAME_TAG           3
#define RRNA_NAME_TAG           4
#define TRNA_AA_TAG             5
#define TRNA_CODON_TAG          6
#define MISC_RNA_TAG            7
#define FEAT_TYPE_TAG           8
#define FEAT_VAL_TAG            9
#define QUAL_TYPE_TAG          10
#define QUAL_VAL_TAG           11
#define COMMENT_TAG            12
#define START_TAG              13
#define STOP_TAG               14
#define STRAND_TAG             15
#define PARTIAL_TAG            16
#define ANTICODON_START_TAG    17
#define ANTICODON_STOP_TAG     18
#define GENE_SYNONYM_TAG       19
#define PROTEIN_SYNONYM_TAG    20
#define PROTEIN_DESC_TAG       21
#define PROTEIN_ACTIVITY_TAG   22
#define PROTEIN_ECNUMBER_TAG   23
#define EXTRA_GENE_SYN_TAG     24
#define EXTRA_PROTEIN_SYN_TAG  25
#define DB_XREF_TAG            26

#define NUM_PARSE_TAGS       27

static ENUM_ALIST(autoparse_alist)
  {" ",                     0},
  {"Gene Name",             GENE_NAME_TAG},
  {"Gene Synonym",          GENE_SYNONYM_TAG},
  {"Protein Name",          PROTEIN_NAME_TAG},
  {"Protein Synonym",       PROTEIN_SYNONYM_TAG},
  {"Protein Description",   PROTEIN_DESC_TAG},
  {"Protein Activity",      PROTEIN_ACTIVITY_TAG},
  {"Protein E.C. Number",   PROTEIN_ECNUMBER_TAG},
  {"mRNA Name",             MRNA_NAME_TAG},
  {"rRNA Name",             RRNA_NAME_TAG},
  {"tRNA Amino Acid",       TRNA_AA_TAG},
  {"tRNA Codons",           TRNA_CODON_TAG},
  {"misc_RNA Name",         MISC_RNA_TAG},
  {"Feature Type",          FEAT_TYPE_TAG},
  {"Feature Value",         FEAT_VAL_TAG},
  {"Qualifier Type",        QUAL_TYPE_TAG},
  {"Qualifier Value",       QUAL_VAL_TAG},
  {"Comment",               COMMENT_TAG},
  {"Start",                 START_TAG},
  {"Stop",                  STOP_TAG},
  {"Strand",                STRAND_TAG},
  {"Partial",               PARTIAL_TAG},
  {"Database Xref",         DB_XREF_TAG},
  /*
  {"Anticodon Start",       ANTICODON_START_TAG},
  {"Anticodon Stop",        ANTICODON_STOP_TAG},
  */
END_ENUM_ALIST

static ENUM_ALIST(qualparse_alist)
  {" ",                     0},
  {"Feature Type",          FEAT_TYPE_TAG},
  {"Qualifier Type",        QUAL_TYPE_TAG},
  {"Qualifier Value",       QUAL_VAL_TAG},
  {"Start",                 START_TAG},
  {"Stop",                  STOP_TAG},
  {"Strand",                STRAND_TAG},
  {"Partial",               PARTIAL_TAG},
END_ENUM_ALIST

static SeqLocPtr MakeIntSeqLoc (BioseqPtr bsp, Int4 from, Int4 to,
                                Int2 strand, Boolean partial5, Boolean partial3)

{
  IntFuzzPtr  ifp;
  SeqIntPtr   sintp;
  SeqLocPtr   slp;

  slp = NULL;
  sintp = SeqIntNew ();
  sintp->from = from;
  sintp->to = to;
  sintp->strand = (Uint1) strand;
  sintp->id = SeqIdDup (SeqIdFindBest (bsp->id, 0));

  slp = ValNodeNew (NULL);
  slp->choice = SEQLOC_INT;
  slp->data.ptrvalue = (Pointer) sintp;

  if (partial5) {
    ifp = IntFuzzNew ();
    if (ifp != NULL) {
      ifp->choice = 4;
      if (strand == Seq_strand_minus || strand == Seq_strand_both_rev) {
        sintp->if_to = ifp;
        ifp->a = 1;
      } else {
        sintp->if_from = ifp;
        ifp->a = 2;
      }
    }
  }

  if (partial3) {
    ifp = IntFuzzNew ();
    if (ifp != NULL) {
      ifp->choice = 4;
      if (strand == Seq_strand_minus || strand == Seq_strand_both_rev) {
        sintp->if_from = ifp;
        ifp->a = 2;
      } else {
        sintp->if_to = ifp;
        ifp->a = 1;
      }
    }
  }

  return slp;
} 

static SeqFeatPtr AddGeneFeat (CharPtr geneName, CharPtr geneSyn, CharPtr xtraGeneSyn,
                               SeqEntryPtr nsep, BioseqPtr nbsp, SeqLocPtr slp,
                               Boolean partial, Boolean partial5, Boolean partial3)

{
  BioseqPtr   bsp;
  GeneRefPtr  grp;
  SeqLocPtr   gslp;
  Boolean     hasNulls;
  SeqFeatPtr  sfp;
  SeqIdPtr    sip;

  sfp = NULL;
  if ((geneName != NULL || geneSyn != NULL) && nsep != NULL && nbsp != NULL && slp != NULL) {
    grp = GeneRefNew ();
    if (grp != NULL) {
      grp->locus = StringSave (geneName);
      TrimSpacesAroundString (grp->locus);
      if (! StringHasNoText (geneSyn)) {
        ValNodeCopyStr (&(grp->syn), 0, geneSyn);
      }
      if (! StringHasNoText (xtraGeneSyn)) {
        ValNodeCopyStr (&(grp->syn), 0, xtraGeneSyn);
      }
      if (ExtendGene (grp, nsep, slp)) {
        grp = GeneRefFree (grp);
      } else {
        sfp = CreateNewFeature (nsep, NULL, SEQFEAT_GENE, NULL);
        if (sfp != NULL) {
          sfp->data.value.ptrvalue = (Pointer) grp;
          sfp->location = SeqLocFree (sfp->location);
          sfp->location = AsnIoMemCopy ((Pointer) slp,
                                        (AsnReadFunc) SeqLocAsnRead,
                                        (AsnWriteFunc) SeqLocAsnWrite);
          sfp->partial = partial;
          sip = SeqLocId (sfp->location);
          if (sip != NULL) {
            bsp = BioseqFind (sip);
          } else {
            bsp = nbsp;
          }
          if (bsp != NULL) {
            gslp = SeqLocMerge (bsp, sfp->location, NULL, TRUE, FALSE, FALSE);
            if (gslp != NULL) {
              sfp->location = SeqLocFree (sfp->location);
              sfp->location = gslp;
              if (bsp->repr == Seq_repr_seg) {
                gslp = SegLocToPartsEx (bsp, sfp->location, TRUE);
                sfp->location = SeqLocFree (sfp->location);
                sfp->location = gslp;
                hasNulls = LocationHasNullsBetween (sfp->location);
                sfp->partial = (sfp->partial || hasNulls);
              }
              FreeAllFuzz (gslp);
              hasNulls = LocationHasNullsBetween (sfp->location);
              sfp->partial = (sfp->partial || hasNulls);
              SetSeqLocPartial (sfp->location, partial5, partial3);
            }
          }
        }
      }
    }
  }
  return sfp;
}

static SeqFeatPtr AddCdsProtFeats (CharPtr protName, CharPtr protSyn, CharPtr xtraProtSyn,
                                   CharPtr protDesc, CharPtr protActiv, CharPtr protEC,
                                   SeqEntryPtr sep, SeqEntryPtr nsep,
                                   BioseqPtr nbsp, SeqLocPtr slp, CharPtr comment,
                                   Boolean partial, Boolean partial5, Boolean partial3)

{
  ByteStorePtr  bs;
  BioseqPtr     bsp;
  SeqFeatPtr    cds;
  Char          ch;
  CdRegionPtr   crp;
  ValNodePtr    descr;
  Int2          genCode;
  Int2          i;
  /*
  Uint1         frame;
  Int4          len;
  Int4          lens [4];
  Int4          max;
  */
  MolInfoPtr    mip;
  SeqEntryPtr   old;
  CharPtr       prot;
  ProtRefPtr    prp;
  SeqEntryPtr   psep;
  CharPtr       ptr;
  SeqFeatPtr    sfp;
  ValNodePtr    vnp;

  cds = NULL;
  if ((protName != NULL || protDesc != NULL) && sep != NULL &&
       nsep != NULL && nbsp != NULL && slp != NULL) {
    genCode = SeqEntryToGeneticCode (sep, NULL, NULL, 0);
    crp = CreateNewCdRgn (1, FALSE, genCode);
    if (crp != NULL) {
      cds = CreateNewFeature (nsep, NULL, SEQFEAT_CDREGION, NULL);
      if (cds != NULL) {
        cds->data.value.ptrvalue = (Pointer) crp;
        cds->location = SeqLocFree (cds->location);
        cds->location = AsnIoMemCopy ((Pointer) slp,
                                      (AsnReadFunc) SeqLocAsnRead,
                                      (AsnWriteFunc) SeqLocAsnWrite);
        cds->partial = partial;
        if (! StringHasNoText (comment)) {
          cds->comment = StringSave (comment);
        }
        /*
        max = 0;
        frame = 0;
        for (i = 1; i <= 3; i++) {
          crp->frame = (Uint1) i;
          bs = ProteinFromCdRegion (cds, FALSE);
          len = BSLen (bs);
          BSFree (bs);
          lens [i] = len;
          if (len > max) {
            max = len;
            frame = (Uint1) i;
          }
        }
        */
        crp->frame = 0;
        bs = ProteinFromCdRegionEx (cds, TRUE, FALSE);
        if (bs != NULL) {
          prot = BSMerge (bs, NULL);
          bs = BSFree (bs);
          if (prot != NULL) {
            ptr = prot;
            ch = *ptr;
            while (ch != '\0') {
              *ptr = TO_UPPER (ch);
              ptr++;
              ch = *ptr;
            }
            i = (Int2) StringLen (prot);
            if (i > 0 && prot [i - 1] == '*') {
              prot [i - 1] = '\0';
            }
            bs = BSNew (1000);
            if (bs != NULL) {
              ptr = prot;
              /*
              if (prot [0] == '-') {
                ptr++;
              }
              */
              BSWrite (bs, (VoidPtr) ptr, (Int4) StringLen (ptr));
            }
          }
          bsp = BioseqNew ();
          if (bsp != NULL) {
            bsp->repr = Seq_repr_raw;
            bsp->mol = Seq_mol_aa;
            bsp->seq_data_type = Seq_code_ncbieaa;
            bsp->seq_data = bs;
            bsp->length = BSLen (bs);
            bs = NULL;
            old = SeqEntrySetScope (sep);
            bsp->id = MakeNewProteinSeqId (cds->location, NULL);
            SeqMgrAddToBioseqIndex (bsp);
            SeqEntrySetScope (old);
            psep = SeqEntryNew ();
            if (psep != NULL) {
              psep->choice = 1;
              psep->data.ptrvalue = (Pointer) bsp;
              SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) bsp, psep);
              mip = MolInfoNew ();
              if (mip != NULL) {
                mip->biomol = 8;
                mip->tech = 8;
                if (partial5 && partial3) {
                  mip->completeness = 5;
                } else if (partial5) {
                  mip->completeness = 3;
                } else if (partial3) {
                  mip->completeness = 4;
                }
                vnp = CreateNewDescriptor (psep, Seq_descr_molinfo);
                if (vnp != NULL) {
                  vnp->data.ptrvalue = (Pointer) mip;
                }
              }
              descr = ExtractBioSourceAndPubs (sep);
              /*
              AddSeqEntryToSeqEntry (sep, psep, FALSE);
              */
              AddSeqEntryToSeqEntry (sep, psep, TRUE);
              nsep = FindNucSeqEntry (sep);
              ReplaceBioSourceAndPubs (sep, descr);
              SetSeqFeatProduct (cds, bsp);
              prp = CreateNewProtRef (protName, protDesc, NULL, NULL);
              if (prp != NULL) {
                if (! StringHasNoText (protSyn)) {
                   ValNodeCopyStr (&(prp->name), 0, protSyn);
                }
                if (! StringHasNoText (xtraProtSyn)) {
                   ValNodeCopyStr (&(prp->name), 0, xtraProtSyn);
                }
                if (! StringHasNoText (protActiv)) {
                   ValNodeCopyStr (&(prp->activity), 0, protActiv);
                }
                if (! StringHasNoText (protEC)) {
                   ValNodeCopyStr (&(prp->ec), 0, protEC);
                }
                sfp = CreateNewFeature (psep, NULL, SEQFEAT_PROT, NULL);
                if (sfp != NULL) {
                  sfp->data.value.ptrvalue = (Pointer) prp;
                }
              }
            }
          }
        }
      }
    }
  }
  return cds;
}

static Boolean IsCodon (CharPtr str, Uint1Ptr codonPtr, Boolean revcomp)

{
  Char   ch;
  Uint1  code;
  Uint1  codon [4];
  Int2   i, k, q;
  Char   tmp [4];

  if (StringLen (str) == 3) {
    k = 0;
    q = 0;
    ch = str [k];
    while (ch != '\0' && q < 3) {
      ch = TO_UPPER (ch);
      if (StringChr ("ACGTU", ch) != NULL) {
        if (ch == 'U') {
          ch = 'T';
        }
        tmp [q] = ch;
        q++;
      }
      k++;
      ch = str [k];
    }
    tmp [q] = 0;
    if (q == 3) {
      if (revcomp) {
        for (i = 0; i < 3; i++) {
          ch = tmp [2 - i];
          switch (ch) {
            case 'A' :
              ch = 'T';
              break;
            case 'C' :
              ch = 'G';
              break;
            case 'G' :
              ch = 'C';
              break;
            case 'T' :
              ch = 'A';
              break;
            default :
              break;
          }
          codon [i] = (Uint1) ch;
        }
        codon [3] = 0;
      } else {
        for (i = 0; i < 3; i++) {
          codon [i] = (Uint1) tmp [i];
        }
        codon [3] = 0;
      }
      code = IndexForCodon (codon, Seq_code_iupacna);
      if (code != INVALID_RESIDUE) {
        if (codonPtr != NULL) {
          *codonPtr = code;
        }
        return TRUE;
      }
    }
  }
  return FALSE;
}

static ValNodePtr LclTokenizeTRnaString (CharPtr strx)

{
  Char        ch;
  ValNodePtr  head;
  Int2        i, j, k;
  CharPtr     ptr;
  Char        str [256];
  CharPtr     strs;
  Char        tmp [128];

  if (StringHasNoText (strx)) return NULL;
  strs = StringSave (strx);
  head = NULL;
  ptr = strs;
  ch = *ptr;
  while (ch != '\0') {
    if (ch == '*') {  /* keep possible terminator tRNA symbol */
    } else if (! IS_ALPHA (ch)) {
     *ptr = ' ';
    }
    ptr++;
    ch = *ptr;
  }
  i = 0;
  while (StringLen (strs + i) > 0) {
    StringNCpy_0 (str, strs + i, sizeof (str));
    k = 0;
    ch = str [k];
    while (ch == ' ') {
      k++;
      ch = str [k];
    }
    j = 0;
    while (ch != '\0' && ch != ' ') {
      j++;
      ch = str [j + k];
    }
    if (ch == ' ') {
      str [j + k] = '\0';
      i += j + k + 1;
    } else {
      i += j + k;
    }
    StringNCpy_0 (tmp, str + k, sizeof (tmp));
    TrimSpacesAroundString (tmp);
    if (! StringHasNoText (tmp)) {
      ValNodeCopyStr (&head, 0, tmp);
    }
  }
  MemFree (strs);
  return head;
}

static SeqFeatPtr AddTRnaFeats (CharPtr tRnaAA, CharPtr tRnaCodon,
                                SeqEntryPtr nsep, BioseqPtr nbsp,
                                SeqLocPtr slp, CharPtr comment, Int2 acStyle,
                                SeqLocPtr acSlp, Boolean partial)

{
  Uint1       aa;
  Uint1       codon;
  Uint1       curraa;
  ValNodePtr  head;
  Int2        j;
  Boolean     revcomp;
  RnaRefPtr   rrp;
  SeqFeatPtr  sfp;
  tRNAPtr     trna;
  ValNodePtr  vnp;

  sfp = NULL;
  if (tRnaAA == NULL || nsep == NULL || nbsp == NULL || slp == NULL) return NULL;
  rrp = RnaRefNew ();
  if (rrp == NULL) return NULL;
  rrp->type = 3;
  rrp->ext.choice = 2;
  trna = (tRNAPtr) MemNew (sizeof (tRNA));
  rrp->ext.value.ptrvalue = (Pointer) trna;
  if (trna != NULL) {
    trna->aatype = 2;
    head = LclTokenizeTRnaString (tRnaAA);
    aa = 0;
    for (vnp = head; (aa == 0 || aa == 'A') && vnp != NULL; vnp = vnp->next) {
      curraa = FindTrnaAA (vnp->data.ptrvalue);
      if (curraa != 0) {
        aa = curraa;
      }
    }
    if (aa == 0) {
      aa = 'X';
    }
    trna->aa = aa;
    for (j = 0; j < 6; j++) {
      trna->codon [j] = 255;
    }
    if (acStyle > 1) {
      if (tRnaAA != tRnaCodon) {
        head = ValNodeFreeData (head);
        head = LclTokenizeTRnaString (tRnaCodon);
      }
      revcomp = (Boolean) (acStyle == 2);
      j = 0;
      for (vnp = head; vnp != NULL; vnp = vnp->next) {
        if (IsCodon (vnp->data.ptrvalue, &codon, revcomp)) {
          if (j < 6) {
            trna->codon [j] = codon;
            j++;
          }
        }
      }
    }
    ValNodeFreeData (head);
    if (acSlp != NULL) {
      trna->anticodon = AsnIoMemCopy ((Pointer) acSlp,
                                      (AsnReadFunc) SeqLocAsnRead,
                                      (AsnWriteFunc) SeqLocAsnWrite);
    }
  }
  sfp = CreateNewFeature (nsep, NULL, SEQFEAT_RNA, NULL);
  if (sfp != NULL) {
    sfp->data.value.ptrvalue = (Pointer) rrp;
    sfp->location = SeqLocFree (sfp->location);
    sfp->location = AsnIoMemCopy ((Pointer) slp,
                                  (AsnReadFunc) SeqLocAsnRead,
                                  (AsnWriteFunc) SeqLocAsnWrite);
    sfp->partial = partial;
    if (! StringHasNoText (comment)) {
      sfp->comment = StringSave (comment);
    }
  }
  return sfp;
}

static SeqFeatPtr AddNonTRnaFeats (CharPtr mRnaName, CharPtr rRnaName, CharPtr miscRnaName,
                             SeqEntryPtr nsep, BioseqPtr nbsp,
                             SeqLocPtr slp, CharPtr comment, Boolean partial)

{
  RnaRefPtr   rrp;
  SeqFeatPtr  sfp;

  sfp = NULL;
  if (mRnaName == NULL && rRnaName == NULL && miscRnaName == NULL) return NULL;
  if (nsep == NULL || nbsp == NULL || slp == NULL) return NULL;
  rrp = RnaRefNew ();
  if (rrp == NULL) return NULL;
  if (mRnaName != NULL) {
    rrp->type = 2;
    rrp->ext.choice = 1;
    rrp->ext.value.ptrvalue = StringSave (mRnaName);
  } else if (rRnaName != NULL) {
    rrp->type = 4;
    rrp->ext.choice = 1;
    rrp->ext.value.ptrvalue = StringSave (rRnaName);
  } else if (miscRnaName != NULL) {
    rrp->type = 255;
    rrp->ext.choice = 1;
    rrp->ext.value.ptrvalue = StringSave (miscRnaName);
  } else return NULL;
  sfp = CreateNewFeature (nsep, NULL, SEQFEAT_RNA, NULL);
  if (sfp != NULL) {
    sfp->data.value.ptrvalue = (Pointer) rrp;
    sfp->location = SeqLocFree (sfp->location);
    sfp->location = AsnIoMemCopy ((Pointer) slp,
                                  (AsnReadFunc) SeqLocAsnRead,
                                  (AsnWriteFunc) SeqLocAsnWrite);
    sfp->partial = partial;
    if (! StringHasNoText (comment)) {
      sfp->comment = StringSave (comment);
    }
  }
  return sfp;
}

static SeqFeatPtr AddImpFeat (CharPtr featType, CharPtr qualType, CharPtr qualVal,
                              SeqEntryPtr nsep, BioseqPtr nbsp,
                              SeqLocPtr slp, CharPtr comment, Boolean partial)

{
  FeatDefPtr  curr;
  ImpFeatPtr  ifp;
  Uint1       key;
  CharPtr     label;
  SeqFeatPtr  sfp;
  CharPtr     str;
  Uint2       subtype;

  sfp = NULL;
  if (featType == NULL || nsep == NULL || nbsp == NULL || slp == NULL) return NULL;
  str = NULL;
  label = NULL;
  curr = FeatDefFindNext (NULL, &key, &label, FEATDEF_ANY, TRUE);
  while (curr != NULL) {
    if (key != FEATDEF_BAD && curr->seqfeat_key == SEQFEAT_IMP) {
      subtype = curr->featdef_key;
      if (subtype != FEATDEF_Imp_CDS &&
          subtype != FEATDEF_misc_RNA &&
          subtype != FEATDEF_precursor_RNA &&
          /*
          subtype != FEATDEF_mat_peptide &&
          subtype != FEATDEF_sig_peptide &&
          subtype != FEATDEF_transit_peptide &&
          */
          subtype != FEATDEF_source &&
          subtype != FEATDEF_virion &&
          subtype != FEATDEF_mutation &&
          subtype != FEATDEF_allele &&
          subtype != FEATDEF_site_ref &&
          subtype != FEATDEF_gap) {
        if (StringICmp (featType, curr->typelabel) == 0) {
          str = curr->typelabel;
        }
      }
    }
    curr = FeatDefFindNext (curr, &key, &label, FEATDEF_ANY, TRUE);
  }
  if (str == NULL) return NULL;
  ifp = ImpFeatNew ();
  if (ifp == NULL) return NULL;
  ifp->key = StringSave (str);
  sfp = CreateNewFeature (nsep, NULL, SEQFEAT_IMP, NULL);
  if (sfp != NULL) {
    sfp->data.value.ptrvalue = (Pointer) ifp;
    sfp->location = SeqLocFree (sfp->location);
    sfp->location = AsnIoMemCopy ((Pointer) slp,
                                  (AsnReadFunc) SeqLocAsnRead,
                                  (AsnWriteFunc) SeqLocAsnWrite);
    sfp->partial = partial;
    if (! StringHasNoText (comment)) {
      sfp->comment = StringSave (comment);
    }
    if ((! StringHasNoText (qualType)) && (! StringHasNoText (qualVal))) {
      AddGBQual (sfp, qualType, qualVal);
    }
  }
  return sfp;
}

#define geneName field [GENE_NAME_TAG]
#define geneSyn field [GENE_SYNONYM_TAG]
#define protName field [PROTEIN_NAME_TAG]
#define protSyn field [PROTEIN_SYNONYM_TAG]
#define protDesc field [PROTEIN_DESC_TAG]
#define mRnaName field [MRNA_NAME_TAG]
#define rRnaName field [RRNA_NAME_TAG]
#define tRnaAA field [TRNA_AA_TAG]
#define tRnaCodon field [TRNA_CODON_TAG]
#define miscRnaName field [MISC_RNA_TAG]
#define featType field [FEAT_TYPE_TAG]
#define featVal field [FEAT_VAL_TAG]
#define qualType field [QUAL_TYPE_TAG]
#define qualVal field [QUAL_VAL_TAG]
#define commentStr field [COMMENT_TAG]
#define startStr field [START_TAG]
#define stopStr field [STOP_TAG]
#define strandStr field [STRAND_TAG]
#define partialStr field [PARTIAL_TAG]
#define acStartStr field [ANTICODON_START_TAG]
#define acStopStr field [ANTICODON_STOP_TAG]
#define protActiv field [PROTEIN_ACTIVITY_TAG]
#define protEC field [PROTEIN_ECNUMBER_TAG]
#define xtraGeneSyn field [EXTRA_GENE_SYN_TAG]
#define xtraProtSyn field [EXTRA_PROTEIN_SYN_TAG]
#define dbxrefStr field [DB_XREF_TAG]

static ValNodePtr ParseALine (ValNodePtr line, Int4Ptr startP, Int4Ptr stopP,
                              CharPtr PNTR featP, CharPtr PNTR qualP,
                              CharPtr PNTR valP, Int2Ptr strandP,
                              BoolPtr partialP, BoolPtr partial5P,
                              BoolPtr partial3P, Int4Ptr lineNumP,
                              AutoParseFormPtr afp)

{
  Boolean     anotherInterval;
  Boolean     badNumber;
  CharPtr     field [NUM_PARSE_TAGS];
  Int2        i;
  Boolean     partial;
  Boolean     partial5;
  Boolean     partial3;
  Int4        start;
  Int4        stop;
  Int2        strand;
  Int2        tag;
  Int4        tmp;
  ValNodePtr  vnp;

  while (line != NULL && line->data.ptrvalue == NULL) {
    line = line->next;
    (*lineNumP)++;
  }
  if (afp == NULL || line == NULL) return NULL;
  if (line->data.ptrvalue == NULL) return line;
  for (i = 0; i < NUM_PARSE_TAGS; i++) {
    field [i] = NULL;
  }
  start = -1;
  stop = -1;
  vnp = line->data.ptrvalue;
  for (i = 0; i < afp->cols && vnp != NULL; i++) {
    tag = afp->tags [i];
    if (tag > 0 && tag < NUM_PARSE_TAGS) {
      if (field [tag] == NULL) {
        if (! StringHasNoText ((CharPtr) vnp->data.ptrvalue)) {
          field [tag] = (CharPtr) vnp->data.ptrvalue;
        }
      }
    }
    vnp = vnp->next;
  }

  badNumber = FALSE;
  if (! StrToLong (startStr, &start)) {
    start = -1;
    badNumber = TRUE;
  }
  if (! StrToLong (stopStr, &stop)) {
    stop = -1;
    badNumber = TRUE;
  }

  partial = FALSE;
  partial5 = FALSE;
  partial3 = FALSE;
  if (! StringHasNoText (partialStr)) {
    partial = TRUE;
    if (StringStr (partialStr, "5")) {
      partial5 = TRUE;
    }
    if (StringStr (partialStr, "3")) {
      partial3 = TRUE;
    }
    if (StringICmp (partialStr, "false") == 0 ||
        StringICmp (partialStr, "no") == 0 ||
        StringICmp (partialStr, "not") == 0 ||
        StringICmp (partialStr, "isn't") == 0 ||
        StringICmp (partialStr, "is not") == 0 ||
        StringICmp (partialStr, "-") == 0 ||
        StringICmp (partialStr, "nyet") == 0 ||
        StringICmp (partialStr, "nein") == 0 ||
        StringICmp (partialStr, "complete") == 0) {
      partial = FALSE;
    }
    if (StringICmp (partialStr, "partial") == 0 ||
        StringICmp (partialStr, "incomplete") == 0) {
      partial = TRUE;
    }
  }

  start--;
  stop--;

  strand = Seq_strand_plus;
  if (strandStr != NULL) {
    if (StringStr (strandStr, "minus") ||
        StringChr (strandStr, '-') ||
        StringStr (strandStr, "complement")) {
      strand = Seq_strand_minus;
    }
  }
  if (start > stop) {
    tmp = start;
    start = stop;
    stop = tmp;
    strand = Seq_strand_minus;
  }
  if (strandStr != NULL) {
    if (StringStr (strandStr, "plus") || StringChr (strandStr, '+')) {
      strand = Seq_strand_plus;
    }
  }

  anotherInterval = TRUE;
  for (i = 0; i < afp->cols; i++) {
    tag = afp->tags [i];
    if (tag > 0 && tag < NUM_PARSE_TAGS) {
      if (! StringHasNoText (field [tag])) {
        if (tag != START_TAG && tag != STOP_TAG &&
            tag != STRAND_TAG && tag != PARTIAL_TAG) {
          anotherInterval = FALSE;
        }
      }
    }
  }
  if (anotherInterval) {
    if (strand == Seq_strand_minus && stop > start) {
      tmp = start;
      start = stop;
      stop = tmp;
    }
  }

  *startP = start;
  *stopP = stop;
  *featP = featType;
  *qualP = qualType;
  *valP = qualVal;
  *strandP = strand;
  *partialP = partial;
  *partial5P = partial5;
  *partial3P = partial3;
  return line;
}

static void AddQualToCDS (SeqFeatPtr sfp, SeqEntryPtr sep, BioseqPtr target, CharPtr product, CharPtr function, CharPtr ec, Uint2 entityID)

{
  ByteStorePtr  bs;
  BioseqPtr     bsp;
  Char          ch;
  CdRegionPtr   crp;
  ValNodePtr    descr;
  Int2          i;
  MolInfoPtr    mip;
  SeqEntryPtr   nsep;
  SeqEntryPtr   old;
  Boolean       partial5;
  Boolean       partial3;
  BioseqPtr     pbsp;
  SeqFeatPtr    prot;
  CharPtr       protseq;
  ProtRefPtr    prp;
  SeqEntryPtr   psep;
  CharPtr       ptr;
  ValNodePtr    vnp;

  if (sfp == NULL || entityID == 0) return;

  if (sfp->product == NULL) {
    crp = (CdRegionPtr) sfp->data.value.ptrvalue;
    if (crp == NULL) return;
    crp->frame = 0;
    bs = ProteinFromCdRegionEx (sfp, TRUE, FALSE);
    if (bs != NULL) {
      protseq = BSMerge (bs, NULL);
      bs = BSFree (bs);
      if (protseq != NULL) {
        ptr = protseq;
        ch = *ptr;
        while (ch != '\0') {
          *ptr = TO_UPPER (ch);
          ptr++;
          ch = *ptr;
        }
        i = (Int2) StringLen (protseq);
        if (i > 0 && protseq [i - 1] == '*') {
          protseq [i - 1] = '\0';
        }
        bs = BSNew (1000);
        if (bs != NULL) {
          ptr = protseq;
          /*
          if (protseq [0] == '-') {
            ptr++;
          }
          */
          BSWrite (bs, (VoidPtr) ptr, (Int4) StringLen (ptr));
        }
      }
      bsp = BioseqNew ();
      if (bsp != NULL) {
        bsp->repr = Seq_repr_raw;
        bsp->mol = Seq_mol_aa;
        bsp->seq_data_type = Seq_code_ncbieaa;
        bsp->seq_data = bs;
        bsp->length = BSLen (bs);
        bs = NULL;
        old = SeqEntrySetScope (sep);
        bsp->id = MakeNewProteinSeqId (sfp->location, NULL);
        CheckSeqLocForPartial (sfp->location, &partial5, &partial3);
        SeqMgrAddToBioseqIndex (bsp);
        SeqEntrySetScope (old);
        psep = SeqEntryNew ();
        if (psep != NULL) {
          psep->choice = 1;
          psep->data.ptrvalue = (Pointer) bsp;
          SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) bsp, psep);
          mip = MolInfoNew ();
          if (mip != NULL) {
            mip->biomol = 8;
            mip->tech = 8;
            if (partial5 && partial3) {
              mip->completeness = 5;
            } else if (partial5) {
              mip->completeness = 3;
            } else if (partial3) {
              mip->completeness = 4;
            }
            vnp = CreateNewDescriptor (psep, Seq_descr_molinfo);
            if (vnp != NULL) {
              vnp->data.ptrvalue = (Pointer) mip;
            }
          }
          descr = ExtractBioSourceAndPubs (sep);
          /*
          AddSeqEntryToSeqEntry (sep, psep, FALSE);
          */
          AddSeqEntryToSeqEntry (sep, psep, TRUE);
          nsep = SeqMgrGetSeqEntryForData (target);
          ReplaceBioSourceAndPubs (sep, descr);
          SetSeqFeatProduct (sfp, bsp);
        }
      }
    }
  }

  if (sfp->product != NULL) {
    FindGeneAndProtForCDS (entityID, sfp, NULL, &prot);
    if (prot == NULL) {
      pbsp = GetBioseqGivenSeqLoc (sfp->product, entityID);
      if (pbsp != NULL) {
        psep = SeqMgrGetSeqEntryForData (pbsp);
        if (psep != NULL) {
          prp = ProtRefNew ();
          if (prp != NULL) {
            prot = CreateNewFeature (psep, NULL, SEQFEAT_PROT, NULL);
            if (prot != NULL) {
              prot->data.value.ptrvalue = (Pointer) prp;
            }
          }
        }
      }
    }
    if (prot != NULL) {
      prp = (ProtRefPtr) prot->data.value.ptrvalue;
      if (prp != NULL) {
        if (! StringHasNoText (product)) {
           ValNodeCopyStr (&(prp->name), 0, product);
        }
        if (! StringHasNoText (function)) {
           ValNodeCopyStr (&(prp->activity), 0, function);
        }
        if (! StringHasNoText (ec)) {
           ValNodeCopyStr (&(prp->ec), 0, ec);
        }
      }
    }
  }
}

typedef struct existfeatdata {
  SeqFeatPtr     source;
  SeqFeatPtr     found;
} ExistFeatData, PNTR ExistFeatPtr;

static Boolean FindExistingFeatFunc (GatherContextPtr gcp)

{
  ExistFeatPtr  efp;
  SeqFeatPtr    sfp;

  if (gcp == NULL) return TRUE;
  efp = (ExistFeatPtr) gcp->userdata;
  if (efp == NULL || efp->source == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  if (sfp == NULL || sfp->data.value.ptrvalue == NULL) return TRUE;
  if (sfp->data.choice == efp->source->data.choice) {
    if (SeqLocCompare (sfp->location, efp->source->location) == SLC_A_EQ_B) {
      efp->found = sfp;
      return FALSE;
    }
  }
  return TRUE;
}

static SeqFeatPtr FindExistingFeature (SeqFeatPtr virtualSfp, Uint2 entityID)

{
  ExistFeatData  efd;
  GatherScope    gs;

  if (entityID == 0 || virtualSfp == NULL) return NULL;
  efd.source = virtualSfp;
  efd.found = NULL;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  GatherEntity (entityID, (Pointer) &efd, FindExistingFeatFunc, &gs);
  return efd.found;
}

static ValNodePtr QualProcessOneLine (AutoParseFormPtr afp, ValNodePtr line, Int4Ptr lineNumP)

{
  Uint1        aa;
  Boolean      clearGene;
  CdRegionPtr  crp;
  FeatDefPtr   curr;
  Uint1        curraa;
  CharPtr      feat;
  Uint1        featdef_key;
  FeatDefPtr   found;
  SeqFeatPtr   gene;
  Int2         genCode;
  GeneRefPtr   grp;
  ValNodePtr   head;
  ImpFeatPtr   ifp;
  Int2         j;
  Uint1        key;
  CharPtr      label;
  BioseqPtr    nbsp;
  SeqEntryPtr  nsep;
  Boolean      partial;
  Boolean      partial5;
  Boolean      partial3;
  SeqFeatPtr   prot;
  ProtRefPtr   prp;
  Int2         qnum;
  CharPtr      qual;
  RnaRefPtr    rrp;
  Uint1        seqfeat_key;
  SeqFeatPtr   sfp;
  SeqLocPtr    slp;
  Int4         start;
  Int4         stop;
  Int2         strand;
  Uint2        subtype;
  tRNAPtr      trna;
  CharPtr      typelabel;
  CharPtr      val;
  SeqFeatPtr   virtualSfp;
  ValNodePtr   vnp;

  if (afp == NULL || line == NULL || lineNumP == NULL) return NULL;
  sfp = NULL;
  virtualSfp = NULL;
  clearGene = FALSE;
  while ((line = ParseALine (line, &start, &stop, &feat, &qual, &val,
      &strand, &partial, &partial5, &partial3, lineNumP, afp)) != NULL) {
    if (feat != NULL && sfp != NULL) {
      return line;
    }
    nsep = SeqMgrGetSeqEntryForData (afp->target);
    if (nsep == NULL) return NULL;
    nbsp = (BioseqPtr) nsep->data.ptrvalue;
    if (nbsp == NULL) return NULL;
    if (sfp == NULL) {
      if (feat == NULL) {
        (*lineNumP)++;
        return line->next;
      }
      slp = MakeIntSeqLoc (nbsp, start, stop, strand, partial5, partial3);
      if (slp == NULL) return NULL;
      label = NULL;
      found = NULL;
      curr = FeatDefFindNext (NULL, &key, &label, FEATDEF_ANY, TRUE);
      while (curr != NULL) {
        if (key != FEATDEF_BAD) {
          subtype = curr->featdef_key;
          if (subtype != FEATDEF_source &&
              subtype != FEATDEF_virion &&
              subtype != FEATDEF_mutation &&
              subtype != FEATDEF_allele &&
              subtype != FEATDEF_site_ref &&
              subtype != FEATDEF_gap) {
            if (StringICmp (feat, curr->typelabel) == 0) {
              found = curr;
            }
          }
        }
        curr = FeatDefFindNext (curr, &key, &label, FEATDEF_ANY, TRUE);
      }
      if (found == NULL) return NULL;
      featdef_key = found->featdef_key;
      seqfeat_key = found->seqfeat_key;
      typelabel = found->typelabel;
      if (featdef_key == FEATDEF_Imp_CDS) {
        featdef_key = FEATDEF_CDS;
        seqfeat_key = SEQFEAT_CDREGION;
      } else if (featdef_key == FEATDEF_misc_RNA) {
        featdef_key = FEATDEF_otherRNA;
        seqfeat_key = SEQFEAT_RNA;
      } else if (featdef_key == FEATDEF_precursor_RNA) {
        featdef_key = FEATDEF_preRNA;
        seqfeat_key = SEQFEAT_RNA;
      }
      switch (seqfeat_key) {
        case SEQFEAT_GENE :
          grp = GeneRefNew ();
          if (grp == NULL) return NULL;
          if (afp->updateExisting) {
            sfp = SeqFeatNew ();
            if (sfp != NULL) {
              sfp->data.choice = SEQFEAT_GENE;
            }
            virtualSfp = sfp;
            clearGene = TRUE;
          } else {
            sfp = CreateNewFeature (nsep, NULL, SEQFEAT_GENE, NULL);
          }
          if (sfp != NULL) {
            sfp->data.value.ptrvalue = (Pointer) grp;
          }
          break;
        case SEQFEAT_CDREGION :
          genCode = SeqEntryToGeneticCode (afp->sep, NULL, NULL, 0);
          crp = CreateNewCdRgn (1, FALSE, genCode);
          if (crp == NULL) return NULL;
          if (afp->updateExisting) {
            sfp = SeqFeatNew ();
            if (sfp != NULL) {
              sfp->data.choice = SEQFEAT_CDREGION;
            }
            virtualSfp = sfp;
            clearGene = TRUE;
          } else {
            sfp = CreateNewFeature (nsep, NULL, SEQFEAT_CDREGION, NULL);
          }
          if (sfp != NULL) {
            sfp->data.value.ptrvalue = (Pointer) crp;
          }
          break;
        case SEQFEAT_RNA :
          rrp = RnaRefNew ();
          if (rrp == NULL) return NULL;
          switch (featdef_key) {
            case FEATDEF_preRNA :
              rrp->type = 1;
              break;
            case FEATDEF_mRNA :
              rrp->type = 2;
              break;
            case FEATDEF_tRNA :
              rrp->type = 3;
              break;
            case FEATDEF_rRNA :
              rrp->type = 4;
              break;
            case FEATDEF_otherRNA :
              rrp->type = 255;
              break;
            default :
              break;
          }
          if (afp->updateExisting) {
            sfp = SeqFeatNew ();
            if (sfp != NULL) {
              sfp->data.choice = SEQFEAT_RNA;
            }
            virtualSfp = sfp;
            clearGene = TRUE;
          } else {
            sfp = CreateNewFeature (nsep, NULL, SEQFEAT_RNA, NULL);
          }
          if (sfp != NULL) {
            sfp->data.value.ptrvalue = (Pointer) rrp;
          }
          break;
        case SEQFEAT_IMP :
          ifp = ImpFeatNew ();
          if (ifp == NULL) return NULL;
          ifp->key = StringSave (typelabel);
          if (afp->updateExisting) {
            sfp = SeqFeatNew ();
            if (sfp != NULL) {
              sfp->data.choice = SEQFEAT_IMP;
            }
            virtualSfp = sfp;
            clearGene = TRUE;
          } else {
            sfp = CreateNewFeature (nsep, NULL, SEQFEAT_IMP, NULL);
          }
          if (sfp != NULL) {
            sfp->data.value.ptrvalue = (Pointer) ifp;
          }
          break;
        default :
          sfp = NULL;
          break;
      }
      if (sfp != NULL) {
        sfp->location = SeqLocFree (sfp->location);
        sfp->location = AsnIoMemCopy ((Pointer) slp,
                                      (AsnReadFunc) SeqLocAsnRead,
                                      (AsnWriteFunc) SeqLocAsnWrite);
        sfp->partial = partial;
      }
    } else if (start >= 0 && stop >= 0 && feat == NULL && qual == NULL && val == NULL) {
      AddSeqFeatInterval (sfp, nbsp, start + 1, stop + 1, partial5, partial3);
    } else if (qual != NULL && val != NULL) {
      if (afp->updateExisting && virtualSfp != NULL) {
        sfp = FindExistingFeature (virtualSfp, afp->input_entityID);
        if (sfp != NULL) {
          virtualSfp = SeqFeatFree (virtualSfp);
          sfp->qual = GBQualFree (sfp->qual);
          sfp->xref = SeqFeatXrefFree (sfp->xref);
          sfp->comment = MemFree (sfp->comment);
          switch (sfp->data.choice) {
            case SEQFEAT_GENE :
              break;
            case SEQFEAT_CDREGION :
              if (sfp->product != NULL) {
                FindGeneAndProtForCDS (afp->input_entityID, sfp, NULL, &prot);
                if (prot != NULL) {
                  prp = (ProtRefPtr) prot->data.value.ptrvalue;
                  if (prp != NULL) {
                    prp->name = ValNodeFreeData (prp->name);
                    prp->activity = ValNodeFreeData (prp->activity);
                    prp->ec = ValNodeFreeData (prp->ec);
                    prp->desc = MemFree (prp->desc);
                  }
                }
              }
              break;
            case SEQFEAT_RNA :
              break;
            case SEQFEAT_IMP :
              break;
            default :
              break;
          }
        }
      }
      qnum = GBQualNameValid (qual);
      if (qnum <= -1) {
        if (StringNCmp (qual, "gene_syn", 8) == 0) {
          qnum = GBQUAL_gene;
        }
      }
      if (qnum > -1 && sfp != NULL) {
        if (qnum == GBQUAL_gene) {
          if (clearGene) {
            clearGene = FALSE;
            FindGeneAndProtForCDS (afp->input_entityID, sfp, &gene, NULL);
            if (gene != NULL) {
              grp = (GeneRefPtr) gene->data.value.ptrvalue;
              if (grp != NULL) {
                if (sfp->data.choice == SEQFEAT_GENE || StringCmp (grp->locus, val) != 0) {
                  grp->locus = MemFree (grp->locus);
                  grp->locus = StringSave (val);
                  grp->syn = ValNodeFreeData (grp->syn);
                }
              }
            }
          } else {
            FindGeneAndProtForCDS (afp->input_entityID, sfp, &gene, NULL);
            if (gene == NULL) {
              CheckSeqLocForPartial (sfp->location, &partial5, &partial3);
              nsep = SeqMgrGetSeqEntryForData (afp->target);
              if (nsep != NULL) {
                nbsp = (BioseqPtr) nsep->data.ptrvalue;
                if (nbsp != NULL) {
                  AddGeneFeat (val, NULL, NULL, nsep, nbsp, sfp->location,
                               sfp->partial, partial5, partial3);
                  FindGeneAndProtForCDS (afp->input_entityID, sfp, &gene, NULL);
                }
              }
            }
            if (gene != NULL) {
              grp = (GeneRefPtr) gene->data.value.ptrvalue;
              if (grp != NULL) {
                if (StringHasNoText (grp->locus)) {
                  grp->locus = StringSave (val);
                } else if (StringICmp (grp->locus, val) != 0) {
                  vnp = grp->syn;
                  while (vnp != NULL && StringICmp ((CharPtr) vnp->data.ptrvalue, val) != 0) {
                    vnp = vnp->next;
                  }
                  if (vnp == NULL) {
                    ValNodeCopyStr (&(grp->syn), 0, val);
                  }
                }
              }
            }
          }
        } else if (qnum == GBQUAL_note) {
          sfp->comment = StringSave (val);
        } else {
          switch (sfp->data.choice) {
            case SEQFEAT_CDREGION :
              switch (qnum) {
                case GBQUAL_function :
                  AddQualToCDS (sfp, afp->sep, afp->target, NULL, val, NULL, afp->input_entityID);
                  break;
                case GBQUAL_EC_number :
                  AddQualToCDS (sfp, afp->sep, afp->target, NULL, NULL, val, afp->input_entityID);
                  break;
                case GBQUAL_product :
                  AddQualToCDS (sfp, afp->sep, afp->target, val, NULL, NULL, afp->input_entityID);
                  break;
                default :
                break;
              }
              break;
            case SEQFEAT_RNA :
              rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
              if (rrp != NULL) {
                switch (qnum) {
                  case GBQUAL_product :
                    switch (rrp->type) {
                      case 3 :
                        rrp->ext.choice = 2;
                        trna = (tRNAPtr) MemNew (sizeof (tRNA));
                        rrp->ext.value.ptrvalue = (Pointer) trna;
                        if (trna != NULL) {
                          trna->aatype = 2;
                          head = LclTokenizeTRnaString (val);
                          aa = 0;
                          for (vnp = head; (aa == 0 || aa == 'A') && vnp != NULL; vnp = vnp->next) {
                            curraa = FindTrnaAA (vnp->data.ptrvalue);
                            if (curraa != 0) {
                              aa = curraa;
                            }
                          }
                          if (aa == 0) {
                            aa = 'X';
                          }
                          trna->aa = aa;
                          for (j = 0; j < 6; j++) {
                            trna->codon [j] = 255;
                          }
                        }
                        break;
                      default :
                        if (rrp->ext.choice == 1) {
                          rrp->ext.value.ptrvalue = MemFree (rrp->ext.value.ptrvalue);
                        }
                        rrp->ext.choice = 1;
                        rrp->ext.value.ptrvalue = StringSave (val);
                        break;
                    }
                    break;
                  default :
                  break;
                }
              }
              break;
            case SEQFEAT_IMP :
              AddGBQual (sfp, qual, val);
              break;
            default :
              break;
          }
        }
      }
    }
    (*lineNumP)++;
    line = line->next;
  }
  if (line == NULL) return NULL;
  (*lineNumP)++;
  return line->next;
}

static SeqFeatPtr AutoProcessOneLine (AutoParseFormPtr afp, ValNodePtr line,
                                      Int4 lineNum, SeqEntryPtr nsep,
                                      SeqFeatPtr sfp, FILE *fp)

{
  Boolean     anotherInterval;
  Boolean     badNumber;
  CharPtr     field [NUM_PARSE_TAGS];
  ValNodePtr  head;
  Int2        i;
  BioseqPtr   nbsp;
  Boolean     partial;
  Boolean     partial5;
  Boolean     partial3;
  CharPtr     ptr;
  Int4        start;
  Int4        stop;
  Int2        strand;
  Int2        tag;
  Int4        tmp;
  ValNodePtr  vnp;

  if (afp == NULL || line == NULL || nsep == NULL || fp == NULL) return sfp;
  nbsp = (BioseqPtr) nsep->data.ptrvalue;
  if (nbsp == NULL) return sfp;
  for (i = 0; i < NUM_PARSE_TAGS; i++) {
    field [i] = NULL;
  }
  start = -1;
  stop = -1;
  vnp = line->data.ptrvalue;
  if (vnp == NULL) return sfp;
  for (i = 0; i < afp->cols && vnp != NULL; i++) {
    tag = afp->tags [i];
    if (tag > 0 && tag < NUM_PARSE_TAGS) {
      if (field [tag] == NULL) {
        if (! StringHasNoText ((CharPtr) vnp->data.ptrvalue)) {
          field [tag] = (CharPtr) vnp->data.ptrvalue;
        }
      }
    }
    vnp = vnp->next;
  }

  badNumber = FALSE;
  if (! StrToLong (startStr, &start)) {
    start = -1;
    badNumber = TRUE;
  }
  if (! StrToLong (stopStr, &stop)) {
    stop = -1;
    badNumber = TRUE;
  }
  if (start < 1 || stop < 1) {
    badNumber = TRUE;
  }
  if (badNumber) {
    if (startStr == NULL) startStr = "";
    if (stopStr == NULL) stopStr = "";
    if (start < 1 && stop < 1) {
      Message (MSG_POST, "Bad number in line %ld - start '%s', stop '%s'",
               (long) lineNum, startStr, stopStr);
    } else if (start < 1) {
      Message (MSG_POST, "Bad number in line %ld - start '%s'", (long) lineNum, startStr);
    } else if (stop < 1) {
      Message (MSG_POST, "Bad number in line %ld - stop '%s'", (long) lineNum, stopStr);
    } else {
      Message (MSG_POST, "Bad number in line %ld", (long) lineNum);
    }
    return sfp;
  }

  partial = FALSE;
  partial5 = FALSE;
  partial3 = FALSE;
  if (! StringHasNoText (partialStr)) {
    partial = TRUE;
    if (StringStr (partialStr, "5")) {
      partial5 = TRUE;
    }
    if (StringStr (partialStr, "3")) {
      partial3 = TRUE;
    }
    if (StringICmp (partialStr, "false") == 0 ||
        StringICmp (partialStr, "no") == 0 ||
        StringICmp (partialStr, "not") == 0 ||
        StringICmp (partialStr, "isn't") == 0 ||
        StringICmp (partialStr, "is not") == 0 ||
        StringICmp (partialStr, "-") == 0 ||
        StringICmp (partialStr, "nyet") == 0 ||
        StringICmp (partialStr, "nein") == 0 ||
        StringICmp (partialStr, "complete") == 0) {
      partial = FALSE;
    }
    if (StringICmp (partialStr, "partial") == 0 ||
        StringICmp (partialStr, "incomplete") == 0) {
      partial = TRUE;
    }
  }

  /*
  start--;
  stop--;
  */

  strand = Seq_strand_plus;
  if (strandStr != NULL) {
    if (StringStr (strandStr, "minus") ||
        StringChr (strandStr, '-') ||
        StringStr (strandStr, "complement")) {
      if (start < stop) {
        tmp = start;
        start = stop;
        stop = tmp;
      }
    }
  }

  anotherInterval = TRUE;
  for (i = 0; i < afp->cols; i++) {
    tag = afp->tags [i];
    if (tag > 0 && tag < NUM_PARSE_TAGS) {
      if (! StringHasNoText (field [tag])) {
        if (tag != START_TAG && tag != STOP_TAG &&
            tag != STRAND_TAG && tag != PARTIAL_TAG) {
          anotherInterval = FALSE;
        }
      }
    }
  }
  if (anotherInterval /* && sfp != NULL */) {
    fprintf (fp, "%ld\t%ld\n", (long) start, (long) stop);
    /*
    AddSeqFeatInterval (sfp, nbsp, start + 1, stop + 1, partial5, partial3);
    if (sfp->data.choice == SEQFEAT_CDREGION) {
      RetranslateOneCDS (sfp, afp->input_entityID);
    }
    */
    return sfp;
  }

  /*
  slp = MakeIntSeqLoc (nbsp, start, stop, strand, partial5, partial3);
  if (slp == NULL) return sfp;
  */

  /*
  acSlp = NULL;
  acStyle = GetValue (afp->acStyle);
  if (! StrToLong (acStartStr, &acStart)) {
    acStart = -1;
  }
  if (! StrToLong (acStopStr, &acStop)) {
    acStop = -1;
  }
  if (acStart > 0 && acStop > 0) {
    acStart--;
    acStop--;
    acSlp = MakeIntSeqLoc (nbsp, acStart, acStop, strand, FALSE, FALSE);
  }
  */

  if ((! StringHasNoText (featType)) && (! StringHasNoText (featVal))) {
    if (StringStr (featType, "gene") && geneName == NULL) {
      geneName = featVal;
    } else if (StringStr (featType, "CDS") && protName == NULL) {
      protName = featVal;
    } else if (StringStr (featType, "mRNA") && mRnaName == NULL) {
      mRnaName = featVal;
    } else if (StringStr (featType, "rRNA") && rRnaName == NULL) {
      rRnaName = featVal;
    } else if (StringStr (featType, "tRNA") && tRnaAA == NULL) {
      tRnaAA = featVal;
    } else if (StringStr (featType, "misc_RNA") && tRnaAA == NULL) {
      miscRnaName = featVal;
    }
  } else if (! StringHasNoText (featType)) {
    if (StringStr (featType, "tRNA-") && featVal == NULL && tRnaAA == NULL) {
      tRnaAA = featType;
    }
  }

  if ((! StringHasNoText (geneName)) ||
      (! StringHasNoText (protName)) ||
      (! StringHasNoText (mRnaName)) ||
      (! StringHasNoText (rRnaName)) ||
      (! StringHasNoText (tRnaAA)) ||
      (! StringHasNoText (miscRnaName))) {
    featVal = NULL;
    featType = NULL;
  }

  if ((! StringHasNoText (geneName)) || (! StringHasNoText (geneSyn))) {
    fprintf (fp, "%ld\t%ld\tgene\n", (long) start, (long) stop);
    /*
    sfp = AddGeneFeat (geneName, geneSyn, xtraGeneSyn, nsep, nbsp, slp, partial, partial5, partial3);
    */
    if (! StringHasNoText (geneName)) {
      fprintf (fp, "\t\t\tgene\t%s\n", geneName);
    }
    if (! StringHasNoText (geneSyn)) {
      fprintf (fp, "\t\t\tgene_syn\t%s\n", geneSyn);
    }
    if (! StringHasNoText (xtraGeneSyn)) {
      fprintf (fp, "\t\t\tgene_syn\t%s\n", xtraGeneSyn);
    }
  }

  if ((! StringHasNoText (protName)) || (! StringHasNoText (protDesc))) {
    /*
    sfp = AddCdsProtFeats (protName, protSyn, xtraProtSyn, protDesc,
                           protActiv, protEC, afp->sep, nsep,
                           nbsp, slp, commentStr, partial, partial5, partial3);
    */
    fprintf (fp, "%ld\t%ld\tCDS\n", (long) start, (long) stop);
    if (! StringHasNoText (protName)) {
      fprintf (fp, "\t\t\tproduct\t%s\n", protName);
    }
    if (! StringHasNoText (protSyn)) {
      fprintf (fp, "\t\t\tproduct\t%s\n", protSyn);
    }
    if (! StringHasNoText (xtraProtSyn)) {
      fprintf (fp, "\t\t\tproduct\t%s\n", xtraProtSyn);
    }
    if (! StringHasNoText (protDesc)) {
      fprintf (fp, "\t\t\tproduct\t%s\n", protDesc);
    }
    if (! StringHasNoText (protActiv)) {
      fprintf (fp, "\t\t\tfunction\t%s\n", protActiv);
    }
    if (! StringHasNoText (protEC)) {
      fprintf (fp, "\t\t\tEC_number\t%s\n", protEC);
    }
  }

  if ((! StringHasNoText (mRnaName)) ||
      (! StringHasNoText (rRnaName)) ||
      (! StringHasNoText (miscRnaName))) {
    /*
    sfp = AddNonTRnaFeats (mRnaName, rRnaName, miscRnaName, nsep, nbsp, slp, commentStr, partial);
    */
    if (! StringHasNoText (mRnaName)) {
      fprintf (fp, "%ld\t%ld\tmRNA\n", (long) start, (long) stop);
      fprintf (fp, "\t\t\tproduct\t%s\n", mRnaName);
    }
    if (! StringHasNoText (rRnaName)) {
      fprintf (fp, "%ld\t%ld\trRNA\n", (long) start, (long) stop);
      fprintf (fp, "\t\t\tproduct\t%s\n", rRnaName);
    }
    if (! StringHasNoText (miscRnaName)) {
      fprintf (fp, "%ld\t%ld\tRNA\n", (long) start, (long) stop);
      fprintf (fp, "\t\t\tproduct\t%s\n", miscRnaName);
    }
  }

  if (! StringHasNoText (tRnaAA)) {
    if (StringHasNoText (tRnaCodon)) {
      tRnaCodon = tRnaAA;
    }
    /*
    sfp = AddTRnaFeats (tRnaAA, tRnaCodon, nsep, nbsp, slp, commentStr, acStyle, acSlp, partial);
    */
    fprintf (fp, "%ld\t%ld\ttRNA\n", (long) start, (long) stop);
    if (! StringHasNoText (tRnaAA)) {
      head = LclTokenizeTRnaString (tRnaCodon);
      ptr = NULL;
      for (vnp = head; ptr == NULL && vnp != NULL; vnp = vnp->next) {
        ptr = FindTrnaAAIndex (vnp->data.ptrvalue);
      }
      if (ptr == NULL) {
        ptr = "Xxx";
      }
      if (! StringHasNoText (ptr)) {
        fprintf (fp, "\t\t\tproduct\t%s\n", ptr);
      }
      head = ValNodeFreeData (head);
    }
  }

  if (! StringHasNoText (featType)) {
    if (StringCmp (featType, "source") == 0) return sfp;
    /* sfp = AddImpFeat (featType, qualType, qualVal, nsep, nbsp, slp, commentStr, partial); */
    fprintf (fp, "%ld\t%ld\t%s\n", (long) start, (long) stop, featType);
    if ((! StringHasNoText (qualType)) && (! StringHasNoText (qualVal))) {
      fprintf (fp, "\t\t\t%s\t%s\n", qualType, qualVal);
    }
    if (! StringHasNoText (commentStr)) {
      fprintf (fp, "\t\t\tnote\t%s\n", commentStr);
    }
  }

  if (! StringHasNoText (dbxrefStr)) {
    fprintf (fp, "\t\t\tdb_xref\t%s\n", dbxrefStr);
  }

  /*
  SeqLocFree (slp);
  SeqLocFree (acSlp);
  */
  return sfp;
}

static void FindBadRows (ButtoN b)

{
  AutoParseFormPtr  afp;
  Uint1             idx;
  ValNodePtr        line;
  Int4              lineNum;
  Int4              num;
  ValNodePtr        vnp;

  afp = (AutoParseFormPtr) GetObjectExtra (b);
  if (afp == NULL) return;
  num = ValNodeLen (afp->table);
  num /= 20;
  lineNum = 0;
  for (line = afp->table; line != NULL; line = line->next) {
    lineNum++;
    vnp = line->data.ptrvalue;
    if (vnp != NULL) {
      idx = vnp->choice;
      if (idx != afp->majority) {
        if (idx > 0 && idx < (Uint1) afp->cols) {
          if (afp->counts [idx] < 50 || afp->counts [idx] < num) {
            Message (MSG_POST, "There are %d columns in line %ld", (int) idx, (long) lineNum);
          }
        } else {
          Message (MSG_POST, "There are %d columns in line %ld", (int) idx, (long) lineNum);
        }
      }
    }
  }
}

static void AutoParseAcceptProc (ButtoN b)

{
  AutoParseFormPtr  afp;
  BioseqPtr         bsp;
  Uint2             entityID;
  FILE              *fp;
  Boolean           hasGeneSyn;
  Boolean           hasProtSyn;
  Boolean           hasStart;
  Boolean           hasStop;
  Int2              i;
  ValNodePtr        line;
  Int4              lineNum;
  MonitorPtr        mon;
  SeqEntryPtr       nsep;
  Int4              num;
  Char              path [PATH_MAX];
  SeqEntryPtr       sep;
  SeqFeatPtr        sfp;
  SeqIdPtr          sip;
  Char              str [42];
  UIEnum            val;

  afp = (AutoParseFormPtr) GetObjectExtra (b);
  if (afp == NULL) return;
  nsep = SeqMgrGetSeqEntryForData (afp->target);
  /* nsep = FindNucSeqEntry (afp->sep); */
  if (nsep == NULL || nsep->choice != 1 || nsep->data.ptrvalue == NULL) return;
  hasStart = FALSE;
  hasStop = FALSE;
  hasGeneSyn = FALSE;
  hasProtSyn = FALSE;
  for (i = 0; i < afp->cols; i++) {
    if (GetEnumPopup (afp->fields [i], afp->alist, &val)) {
      afp->tags [i] = (Int2) val;
      if (val == START_TAG) {
        hasStart = TRUE;
      } else if (val == STOP_TAG) {
        hasStop = TRUE;
      } else if (val == GENE_SYNONYM_TAG) {
        if (hasGeneSyn) {
          afp->tags [i] = (Int2) EXTRA_GENE_SYN_TAG;
        }
        hasGeneSyn = TRUE;
      } else if (val == PROTEIN_SYNONYM_TAG) {
        if (hasProtSyn) {
          afp->tags [i] = (Int2) EXTRA_PROTEIN_SYN_TAG;
        }
        hasProtSyn = TRUE;
      }
    }
  }
  if (hasStart && hasStop) {
  } else {
    if (hasStart) {
      Message (MSG_ERROR, "Stop field not set");
    } else if (hasStop) {
      Message (MSG_ERROR, "Start field not set");
    } else {
      Message (MSG_ERROR, "Start and stop fields not set");
    }
    return;
  }
  Hide (afp->form);
  Update ();
  num = ValNodeLen (afp->table);
  mon = MonitorIntNewEx ("Feature Table Parsing", 0, num, FALSE);
  if (afp->qualifierStyle) {
    lineNum = 1;
    line = afp->table;
    while (line != NULL) {
      MonitorIntValue (mon, lineNum);
      line = QualProcessOneLine (afp, line, &lineNum);
    }
  } else {
    lineNum = 0;
    sfp = NULL;
    if (afp->updateExisting) {
      Message (MSG_OK, "Enter the name of a new file for converting old style to new style");
      while (! GetOutputFileName (path, sizeof (path), NULL)) {
        if (Message (MSG_YN, "Do you wish to cancel this operation?") == ANS_YES) {
          MonitorFree (mon);
          Remove (afp->form);
          return;
        }
      }
    } else {
      TmpNam (path);
    }
    fp = FileOpen (path, "w");
    if (fp != NULL) {
      bsp = afp->target;
      sip = SeqIdFindBest (bsp->id, 0);
      SeqIdWrite (sip, str, PRINTID_FASTA_LONG, sizeof (str) - 1);
      if (! StringHasNoText (str)) {
        fprintf (fp, ">Feature %s\n", str);
      }
      for (line = afp->table; line != NULL; line = line->next) {
        lineNum++;
        nsep = SeqMgrGetSeqEntryForData (afp->target);
        /* nsep = FindNucSeqEntry (afp->sep); */
        MonitorIntValue (mon, lineNum);
        if (line->data.ptrvalue != NULL) {
          sfp = AutoProcessOneLine (afp, line, lineNum, nsep, sfp, fp);
        }
      }
      FileClose (fp);
      if (afp->updateExisting) {
        Message (MSG_OK, "Now process the new file through the Parse Features function");
      } else {
        SequinOpenResultFile (path);
      }
    }
    if (! afp->updateExisting) {
      FileRemove (path);
    }
  }
  MonitorFree (mon);
  Update ();
  entityID = afp->input_entityID;
  sep = GetTopSeqEntryForEntityID (entityID);
  MySeqEntryToAsn3 (sep, TRUE, FALSE, FALSE);
  Remove (afp->form);
  Update ();
  ObjMgrSetDirtyFlag (entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
}

#define ONLY_NUMBER     1
#define ONLY_FEATURE    2
#define ONLY_QUALIFIER  3
#define MAYBE_VALUE     4
#define ONLY_PARTIAL    5
#define ONLY_STRAND     6
#define JUST_A_DASH     7
#define IS_GENE         8

static CharPtr partialList [] = {
  "5", "3", "partial", "false", "no", "not", "isn't", "is not",
  "-", "nyet", "nein", "complete", "incomplete", NULL
};

static CharPtr strandList [] = {
  "plus", "minus", "+", "-", "complement", NULL
};

static Boolean ListHasString (CharPtr str, CharPtr PNTR list)

{
  Int2  i;

  if (str == NULL || list == NULL) return FALSE;
  for (i = 0; list [i] != NULL; i++) {
    if (StringICmp (str, list [i]) == 0) return TRUE;
  }
  return FALSE;
}

static void TryToPresetParserFieldPopups (AutoParseFormPtr afp)

{
  Char        ch;
  Int2        current [7];
  Boolean     hasPartial;
  Boolean     hasStart;
  Boolean     hasStrand;
  Int2        i;
  Boolean     isNumber;
  ValNodePtr  line;
  Int2        mixedCount;
  Int2        overall [7];
  CharPtr     ptr;
  CharPtr     str;
  ValNodePtr  vnp;

  for (i = 0; i < 7; i++) {
    overall [i] = 0;
  }
  for (line = afp->table; line != NULL; line = line->next) {
    for (i = 0; i < 7; i++) {
      current [i] = 0;
    }
    for (vnp = (ValNodePtr) line->data.ptrvalue, i = 0; vnp != NULL && i < 7; vnp = vnp->next, i++) {
      str = (CharPtr) vnp->data.ptrvalue;
      if (str != NULL) {
        ptr = str;
        isNumber = TRUE;
        ch = *ptr;
        while (ch != '\0') {
          if (! IS_DIGIT (ch)) {
            isNumber = FALSE;
          }
          ptr++;
          ch = *ptr;
        }
        if (StringCmp (str, "-") == 0) {
          current [i] = JUST_A_DASH;
        } else if (StringCmp (str, "gene") == 0) {
          current [i] = IS_GENE;
        } else if (isNumber) {
          current [i] = ONLY_NUMBER;
        } else if (GBFeatKeyNameValid (&str, FALSE) > -1) {
          current [i] = ONLY_FEATURE;
        } else if (GBQualNameValid (str) > -1 || StringNCmp (str, "gene_syn", 8) == 0) {
          current [i] = ONLY_QUALIFIER;
        } else if (ListHasString (str, partialList)) {
          current [i] = ONLY_PARTIAL;
        } else if (ListHasString (str, strandList)) {
          current [i] = ONLY_STRAND;
        } else {
          current [i] = MAYBE_VALUE;
        }
      }
    }
    for (i = 0; i < 7; i++) {
      if (overall [i] == 0) {
        overall [i] = current [i];
      } else if (current [i] == 0) {
      } else if (current [i] == JUST_A_DASH) {
        if (overall [i] != JUST_A_DASH &&
            overall [i] != ONLY_PARTIAL &&
            overall [i] != ONLY_STRAND) {
          overall [i] = MAYBE_VALUE;
        }
      } else if (current [i] == IS_GENE) {
        if (overall [i] != IS_GENE &&
            overall [i] != ONLY_FEATURE &&
            overall [i] != ONLY_QUALIFIER) {
          overall [i] = MAYBE_VALUE;
        }
      } else if (overall [i] != current [i]) {
        if (overall [i] == JUST_A_DASH) {
          overall [i] = current [i];
        } else {
          overall [i] = MAYBE_VALUE;
        }
      }
    }
  }
  hasStart = FALSE;
  hasPartial = FALSE;
  hasStrand = FALSE;
  mixedCount = 0;
  for (i = 0; i < 7; i++) {
    switch (overall [i]) {
      case ONLY_PARTIAL :
        hasPartial = TRUE;
        break;
      case ONLY_STRAND :
        hasStrand = TRUE;
        break;
      case MAYBE_VALUE :
        mixedCount++;
        break;
      default :
        break;
    }
  }
  for (i = 0; i < 7; i++) {
    switch (overall [i]) {
      case ONLY_NUMBER :
        if (hasStart) {
          SetEnumPopup (afp->fields [i], afp->alist, STOP_TAG);
        } else {
          SetEnumPopup (afp->fields [i], afp->alist, START_TAG);
          hasStart = TRUE;
        }
        break;
      case ONLY_FEATURE :
        SetEnumPopup (afp->fields [i], afp->alist, FEAT_TYPE_TAG);
        break;
      case ONLY_QUALIFIER :
        SetEnumPopup (afp->fields [i], afp->alist, QUAL_TYPE_TAG);
        break;
      case ONLY_PARTIAL :
        SetEnumPopup (afp->fields [i], afp->alist, PARTIAL_TAG);
        break;
      case ONLY_STRAND :
        SetEnumPopup (afp->fields [i], afp->alist, STRAND_TAG);
        break;
      case JUST_A_DASH :
        if (hasPartial && hasStrand) {
        } else if (hasStrand) {
          SetEnumPopup (afp->fields [i], afp->alist, PARTIAL_TAG);
        } else if (hasPartial) {
          SetEnumPopup (afp->fields [i], afp->alist, STRAND_TAG);
        }
        break;
      case MAYBE_VALUE :
        if (mixedCount == 1) {
          SetEnumPopup (afp->fields [i], afp->alist, QUAL_VAL_TAG);
        }
        break;
      default :
        break;
    }
  }
}

static void FreeFeatureTable (ValNodePtr head)

{
  ValNodePtr  vnp;

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    vnp->data.ptrvalue = ValNodeFreeData (vnp->data.ptrvalue);
  }
  ValNodeFreeData (head);
}

static void CleanupAutoParseForm (GraphiC g, VoidPtr data)

{
  AutoParseFormPtr  afp;

  afp = (AutoParseFormPtr) data;
  if (afp != NULL) {
    FreeFeatureTable (afp->table);
    MemFree (afp->fields);
    MemFree (afp->tags);
    MemFree (afp->counts);
  }
  StdCleanupFormProc (g, data);
}

static ForM CreateAutoParseForm (Uint2 entityID, SeqEntryPtr sep, BioseqPtr target,
                                 ValNodePtr head, Boolean updateExisting, Boolean qualifierStyle)

{
  AutoParseFormPtr   afp;
  ButtoN             b;
  GrouP              c;
  Int4Ptr            counts;
  CharPtr PNTR       examples;
  PopuP PNTR         fields;
  GrouP              g;
  GrouP              h;
  Int2               i;
  Uint1              idx;
  GrouP              k;
  ValNodePtr         line;
  Int2               majority;
  Int4               max;
  StdEditorProcsPtr  sepp;
  Char               str [64];
  Boolean            truncated;
  ValNodePtr         vnp;
  WindoW             w;
  PrompT             warn;

  w = NULL;
  afp = MemNew (sizeof (AutoParseForm));
  if (afp != NULL) {
    w = FixedWindow (-50, -33, -10, -10, "Feature Table Parser", StdCloseWindowProc);
    SetObjectExtra (w, afp, StdCleanupFormProc);
    afp->form = (ForM) w;
    afp->formmessage = AutoParseMessageProc;
    afp->input_entityID = entityID;
    afp->sep = sep;
    afp->target = target;
    afp->table = head;
    afp->updateExisting = updateExisting;
    afp->qualifierStyle = qualifierStyle;
    if (afp->qualifierStyle) {
      afp->alist = qualparse_alist;
    } else {
      afp->alist = autoparse_alist;
    }

    sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
    if (sepp != NULL) {
      SetActivate (w, sepp->activateForm);
      afp->appmessage = sepp->handleMessages;
    }

    afp->cols = head->choice;
    fields = MemNew (sizeof (PopuP) * (afp->cols + 1));
    afp->fields = fields;
    afp->tags = MemNew (sizeof (Int2) * (afp->cols + 1));
    examples = MemNew (sizeof (CharPtr) * (afp->cols + 1));
    counts = MemNew (sizeof (Int4) * (afp->cols + 1));
    afp->counts = counts;

    truncated = FALSE;
    for (line = head; line != NULL; line = line->next) {
      vnp = line->data.ptrvalue;
      if (vnp != NULL) {
        idx = vnp->choice;
        if (idx > 0 && idx <= (Uint1) afp->cols) {
          (counts [idx])++;
          if (idx < (Uint1) afp->cols) {
            truncated = TRUE;
          }
        }
        for (i = 0, vnp = line->data.ptrvalue; i < afp->cols && vnp != NULL; i++, vnp = vnp->next) {
          if (examples [i] == NULL && (! StringHasNoText ((CharPtr) vnp->data.ptrvalue))) {
            examples [i] = (CharPtr) vnp->data.ptrvalue;
          }
        }
      }
    }

    majority = 0;
    if (truncated) {
      max = 0;
      for (i = 0; i < afp->cols; i++) {
        if (counts [i + 1] > max) {
          max = counts [i + 1];
          majority = i + 1;
        }
      }
    }
    afp->majority = majority;

    h = HiddenGroup (w, -1, 0, NULL);
    SetGroupSpacing (h, 10, 10);

    warn = NULL;
    if (truncated && (! afp->qualifierStyle)) {
      warn = StaticPrompt (h, "WARNING - NOT ALL ROWS HAVE THE SAME NUMBER OF COLUMNS",
                           0, 0, systemFont, 'c');
    }

    g = HiddenGroup (h, 4, 0, NULL);
    StaticPrompt (g, "Column", 0, 0, programFont, 'c');
    StaticPrompt (g, "Field", 0, 0, programFont, 'c');
    StaticPrompt (g, "Example", 0, 0, programFont, 'c');
    if (truncated) {
      StaticPrompt (g, "Rows", 0, 0, programFont, 'c');
    } else {
      StaticPrompt (g, "", 0, 0, programFont, 'c');
    }
    for (i = 0; i < afp->cols; i++) {
      sprintf (str, "%d", (int) (i + 1));
      StaticPrompt (g, str, 0, popupMenuHeight, programFont, 'c');
      fields [i] = PopupList (g, TRUE, NULL);
      InitEnumPopup (fields [i], afp->alist, NULL);
      SetEnumPopup (fields [i], afp->alist, 0);
      if (examples [i] != NULL) {
        StringNCpy_0 (str, examples [i], sizeof (str));
        if (StringLen (str) > 40) {
          str [50] = '\0';
          StringCat (str, "...");
        }
        StaticPrompt (g, str, 0, popupMenuHeight, programFont, 'l');
      } else {
        StaticPrompt (g, "", 0, popupMenuHeight, programFont, 'l');
      }
      sprintf (str, "%d", (int) (counts [i + 1]));
      if (truncated) {
        if (counts [i + 1] == 0) {
          StaticPrompt (g, "", 0, popupMenuHeight, programFont, 'c');
        } else {
          StaticPrompt (g, str, 0, popupMenuHeight, programFont, 'c');
        }
      } else {
        StaticPrompt (g, "", 0, popupMenuHeight, programFont, 'c');
      }
    }

    MemFree (examples);

    k = HiddenGroup (h, 1, 0, NULL);
    /*
    StaticPrompt (k, "Minus strand indicator", 0, 0, programFont, 'c');
    afp->strandStyle = HiddenGroup (k, 4, 0, NULL);
    RadioButton (afp->strandStyle, "'minus' or '-'");
    RadioButton (afp->strandStyle, "start > stop");
    */
    if (! afp->qualifierStyle) {
      StaticPrompt (k, "tRNA ACGTU triplet policy", 0, 0, programFont, 'c');
      afp->acStyle = HiddenGroup (k, 4, 0, NULL);
      RadioButton (afp->acStyle, "Ignore");
      RadioButton (afp->acStyle, "Anticodon");
      RadioButton (afp->acStyle, "Codon Recognized");
      SetValue (afp->acStyle, 1);
    }

    c = HiddenGroup (w, 4, 0, NULL);
    b = DefaultButton (c, "Accept", AutoParseAcceptProc);
    SetObjectExtra (b, afp, NULL);
    PushButton (c, "Cancel", StdCancelButtonProc);

    b = NULL;
    if (truncated && (! afp->qualifierStyle)) {
      b = PushButton (w, "Find Unusual Rows", FindBadRows);
      SetObjectExtra (b, afp, NULL);
    }

    AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) k, (HANDLE) c,
                  (HANDLE) warn, (HANDLE) b, NULL);

    RealizeWindow (w);

    if (afp->qualifierStyle && head != NULL && head->choice >= 5 && head->choice <= 7) {
      TryToPresetParserFieldPopups (afp);
    }

  }
  return (ForM) w;
}

static ValNodePtr ParseFeatureLine (CharPtr line, Boolean tabDelimited)

{
  Char        ch;
  Int2        i, j, k;
  CharPtr     str;
  Char        tmp [1024];
  ValNodePtr  vnp;

  vnp = NULL;
  i = 0;
  while (StringLen (line + i) > 0) {
    /* StringNCpy_0 (str, line + i, sizeof (str)); */
    str = line + i;
    k = 0;
    ch = str [k];
    while (ch == ' ') {
      k++;
      ch = str [k];
    }
    j = 0;
    if (tabDelimited) {
      while (ch != '\0' && ch != '\t') {
        j++;
        ch = str [j + k];
      }
    } else {
      while (ch != '\0' && (! (IS_WHITESP (ch)))) {
        j++;
        ch = str [j + k];
      }
    }
    if (ch == '\0') {
      i += j + k;
    } else {
      str [j + k] = '\0';
      i += j + k + 1;
    }
    StringNCpy_0 (tmp, str + k, sizeof (tmp));
    TrimSpacesAroundString (tmp);
    if (StringHasNoText (tmp)) {
      ValNodeAdd (&vnp);
    } else {
      ValNodeCopyStr (&vnp, 0, tmp);
    }
  }
  if (vnp != NULL) {
    vnp->choice = ValNodeLen (vnp);
  }
  return vnp;
}

static ValNodePtr ParseFeatureTable (CharPtr path, Boolean tabDelimited)

{
  Char        ch;
  FILE        *fp;
  ValNodePtr  head;
  size_t      len;
  CharPtr     ptr;
  CharPtr     str;
  ValNodePtr  tmp;
  ValNodePtr  vnp;

  head = NULL;
  fp = FileOpen (path, "r");
  if (fp == NULL) return NULL;
  len = sizeof (Char) * 8192;
  str = MemNew (len);
  ReadLine (fp, str, len); /* FileGets on Mac sometimes sees '\r' instead of '\n' */
  while (Nlm_fileDone) {
    if (! StringHasNoText (str)) {
      ptr = str;
      ch = *ptr;
      while (ch != '\0') {
        if (ch == '\n' || ch == '\r') {
          *ptr = '\0';
        } else {
          ptr++;
        }
        ch = *ptr;
      }
      if (StringNICmp (str, ">Feature", 8) != 0) {
        vnp = ParseFeatureLine (str, tabDelimited);
        if (vnp != NULL) {
          ValNodeAddPointer (&head, 0, (Pointer) vnp);
        }
      }
    } else {
      ValNodeAdd (&head);
    }
    ReadLine (fp, str, len);
  }
  MemFree (str);
  FileClose (fp);
  for (tmp = head; tmp != NULL; tmp = tmp->next) {
    vnp = tmp->data.ptrvalue;
    if (vnp != NULL) {
      head->choice = MAX (head->choice, vnp->choice);
    }
  }
  return head;
}

static void CommonAutoParseProc (BaseFormPtr bfp, Boolean updateExisting,
                                 Boolean qualifierStyle, Boolean tabDelimited)

{
  BioseqPtr    bsp;
  ForM         f;
  Char         path [PATH_MAX];
  SeqEntryPtr  sep = NULL;
  ValNodePtr   vnp;

  if (bfp == NULL) return;

  bsp =  GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
  if (bsp == NULL) return;
  sep = GetBestTopParentForData (bfp->input_entityID, bsp);
  if (sep == NULL) return;

  if (! GetInputFileName (path, sizeof (path), NULL, "TEXT")) return;
  vnp = ParseFeatureTable (path, tabDelimited);
  if (vnp == NULL) return;

  f = CreateAutoParseForm (bfp->input_entityID, sep, bsp, vnp, updateExisting, qualifierStyle);
  if (f == NULL) return;
  Show (f);
  Select (f);
}

typedef struct preparsedata {
  FORM_MESSAGE_BLOCK
  BaseFormPtr    bfp;
  SeqEntryPtr    sep;
  GrouP          tabDelimited;
  GrouP          updateExisting;
  GrouP          qualifierStyle;
  Int2           upExVal;
} PreParseData, PNTR PreParsePtr;

static void PreParseAcceptProc (ButtoN b)

{
  PreParsePtr  ppp;
  Boolean      qualifierStyle;
  Boolean      tabDelimited;
  Boolean      updateExisting;

  ppp = (PreParsePtr) GetObjectExtra (b);
  if (ppp == NULL) return;
  Hide (ppp->form);
  Update ();
  qualifierStyle = (Boolean) (GetValue (ppp->qualifierStyle) == 1);
  updateExisting = (Boolean) (GetValue (ppp->updateExisting) == 2);
  tabDelimited = (Boolean) (GetValue (ppp->tabDelimited) == 1);
  CommonAutoParseProc (ppp->bfp, updateExisting, qualifierStyle, tabDelimited);
  Remove (ppp->form);
}

static void ChangeQualifierStyle (GrouP g)

{
  /*
  PreParsePtr  ppp;
  Int2         val;

  ppp = (PreParsePtr) GetObjectExtra (g);
  if (ppp == NULL) return;
  val = GetValue (g);
  if (val == 1) {
    Enable (ppp->updateExisting);
    SetValue (ppp->updateExisting, ppp->upExVal);
  } else {
    ppp->upExVal = GetValue (ppp->updateExisting);
    SetValue (ppp->updateExisting, 1);
    Disable (ppp->updateExisting);
  }
  */
}

static void PreParseMessageProc (ForM f, Int2 mssg)

{
  StdEditorProcsPtr  sepp;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    if (sepp->handleMessages != NULL) {
      sepp->handleMessages (f, mssg);
    }
  }
}

static CharPtr style1Txt = "\
1044    1142    CDS\n\
1208    1612\n\
1683    1949\n\
                        product   alcohol dehydrogenase\n\
                        gene      adh";
static CharPtr style2Txt = "\
1044    1142    adh     alcohol dehydrogenase\n\
1208    1612\n\
1683    1949\n";

static CharPtr nolongersupportedmessage =
"This function has been superseded.  Use File->Open to read the\n\
5-column feature table format described in the Sequin Quick Guide.\n\
However, if you have data in the old form, you can still use this\n\
function, but please tell submitters to use the new 5-column format\n\
in the future.  Continue?";

void AutoParseFeatureTableProc (IteM i)

{
  ButtoN       b, b1, b2;
  BaseFormPtr  bfp;
  GrouP        c;
  Int2         delta;
  GrouP        h;
  GrouP        k;
  GrouP        m;
  Int2         max;
  PreParsePtr  ppp;
  GrouP        ppt1, ppt2;
  GrouP        q;
  RecT         r;
  SeqEntryPtr  sep;
  Int2         top;
  WindoW       w;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  if (Message (MSG_YN, nolongersupportedmessage) == ANS_NO) return;

  w = NULL;
  ppp = MemNew (sizeof (PreParseData));
  if (ppp != NULL) {
    w = FixedWindow (-50, -33, -10, -10, "Feature Table Parser", StdCloseWindowProc);
    SetObjectExtra (w, ppp, StdCleanupFormProc);
    ppp->form = (ForM) w;
    ppp->formmessage = PreParseMessageProc;
    ppp->input_entityID = bfp->input_entityID;
    ppp->bfp = bfp;
    ppp->sep = sep;

    SelectFont (programFont);
    max = StringWidth ("                        product   alcohol dehydrogenase") + 2;
    SelectFont (systemFont);

    h = HiddenGroup (w, -1, 0, NULL);
    SetGroupSpacing (h, 10, 10);

    k = NormalGroup (h, 2, 0, "Table Style", programFont, NULL);
    ppp->qualifierStyle = HiddenGroup (k, 0, 2, ChangeQualifierStyle);
    SetObjectExtra (ppp->qualifierStyle, ppp, NULL);
    /*
    b1 = RadioButton (ppp->qualifierStyle, "One Qualifier Per Line");
    b2 = RadioButton (ppp->qualifierStyle, "All Fields Assigned");
    */
    b1 = RadioButton (ppp->qualifierStyle, " ");
    b2 = RadioButton (ppp->qualifierStyle, " ");
    SetValue (ppp->qualifierStyle, 1);
    m = HiddenGroup (k, 0, 2, NULL);
    SetGroupSpacing (m, 3, 10);
    ppt1 = MultiLinePromptEx (m, style1Txt, max, programFont, FALSE);
    ppt2 = MultiLinePromptEx (m, style2Txt, max, programFont, FALSE);

    q = NormalGroup (h, 2, 0, "Operation", programFont, NULL);
    ppp->updateExisting = HiddenGroup (q, 2, 0, NULL);
    RadioButton (ppp->updateExisting, "Create New Features");
    RadioButton (ppp->updateExisting, "Replace Old Features");
    SetValue (ppp->updateExisting, 1);
    ppp->upExVal = 1;

    ppp->tabDelimited = NormalGroup (h, 2, 0, "Delimiter", programFont, NULL);
    RadioButton (ppp->tabDelimited, "Tabs");
    RadioButton (ppp->tabDelimited, "White Space");
    SetValue (ppp->tabDelimited, 1);

    c = HiddenGroup (w, 4, 0, NULL);
    b = DefaultButton (c, "Accept", PreParseAcceptProc);
    SetObjectExtra (b, ppp, NULL);
    PushButton (c, "Cancel", StdCancelButtonProc);

    AlignObjects (ALIGN_CENTER, (HANDLE) k, (HANDLE) q,
                  (HANDLE) ppp->tabDelimited, (HANDLE) c, NULL);

    GetPosition (ppt2, &r);
    top = r.top;
    GetPosition (b2, &r);
    delta = top - r.top;
    OffsetRect (&r, 0, delta);
    SetPosition (b2, &r);
    AdjustPrnt (b2, &r, FALSE);

    AlignObjects (ALIGN_MIDDLE, (HANDLE) b1, (HANDLE) ppt1, NULL);
    AlignObjects (ALIGN_MIDDLE, (HANDLE) b2, (HANDLE) ppt2, NULL);

    RealizeWindow (w);
    Show (w);
    Select (w);
  }
}

extern Boolean DoBuildContig (void)

{
  MsgAnswer    ans;
  BioseqPtr    bsp;
  Pointer      dataptr;
  Uint2        datatype;
  Uint2        entityID;
  FILE         *fp;
  Int2         handled;
  Char         path [PATH_MAX];
  SeqEntryPtr  sep;

  if (! GetInputFileName (path, sizeof (path), NULL, "TEXT")) return FALSE;
  fp = FileOpen (path, "r");
  if (fp == NULL) return FALSE;
  ans = Message (MSG_YN, "Are coordinates on the master (as opposed to the individual accession)?");
  sep = ReadContigList (fp, (Boolean) (ans == ANS_YES));
  FileClose (fp);
  if (sep == NULL || sep->choice != 1) return FALSE;

  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL) return FALSE;
  datatype = OBJ_BIOSEQ;
  dataptr = (Pointer) bsp;
  entityID = ObjMgrRegister (datatype, dataptr);
  if (dataptr == NULL || entityID == 0) return FALSE;

  WatchCursor ();
  Update ();
  handled = GatherProcLaunch (OMPROC_VIEW, FALSE, entityID, 1,
                              OBJ_BIOSEQ, 0, 0, OBJ_BIOSEQ, 0);
  ArrowCursor ();
  Update ();

  if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
    Message (MSG_FATAL, "Unable to launch viewer.");
    SeqEntryFree (sep);
    return FALSE;
  }

  ObjMgrSetOptions (OM_OPT_FREE_IF_NO_VIEW, entityID);
  ObjMgrSetDirtyFlag (entityID, TRUE);
  return TRUE;
}

static void DoParseTrinomial (BioSourcePtr biop, Pointer userdata)

{
  BinomialOrgNamePtr  bonp;
  OrgModPtr           omp;
  OrgNamePtr          onp;
  OrgRefPtr           orp;
  CharPtr             str;

  if (biop == NULL) return;
  orp = biop->org;
  if (orp == NULL) return;
  onp = orp->orgname;
  if (onp == NULL) return;
  if (onp->choice != 1) return;
  bonp = (BinomialOrgNamePtr) onp->data;
  if (bonp == NULL) return;
  if (StringHasNoText (bonp->subspecies)) return;
  for (omp = onp->mod; omp != NULL; omp = omp->next) {
    if (omp->subtype == ORGMOD_sub_species) return;
  }
  str = bonp->subspecies;
  if (StringNICmp (str, "subsp. ", 7) == 0) {
    str += 7;
    if (StringHasNoText (str)) return;
  }
  omp = OrgModNew ();
  if (omp == NULL) return;
  omp->subtype = ORGMOD_sub_species;
  omp->subname = StringSave (str);
  omp->next = onp->mod;
  onp->mod = omp;
}

extern void ParseTrinomial (IteM i);
extern void ParseTrinomial (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  VisitBioSourcesInSep (sep, NULL, DoParseTrinomial);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  Update ();
}

#define REMOVE_QUAL   1
#define CONVERT_FEAT  2
#define CONVERT_QUAL  3
#define EDIT_QUAL     4
#define ADD_QUAL      5

typedef struct qualformdata {
  FEATURE_FORM_BLOCK

  Int2           type;
  LisT           fromfeatlist;
  LisT           tofeatlist;
  LisT           fromquallist;
  LisT           toquallist;
  TexT           findthis;
  TexT           replacewith;
  Uint2          itemtype;
  Uint2          subtype;
  ObjMgrPtr      omp;
  ObjMgrTypePtr  omtp;
  Boolean        stringfound;
  Boolean        abortconvert;
  CharPtr        findStr;
  CharPtr        replaceStr;
  EnumFieldAssoc PNTR realalist;
  EnumFieldAssoc PNTR alist;
  ValNodePtr     bsplist;
  ButtoN         leaveDlgUp;
} QualFormData, PNTR QualFormPtr;

static void LIBCALLBACK AsnWriteQualifierForDCallBack (AsnExpOptStructPtr pAEOS)

{
  CharPtr      pchFind;
  CharPtr      pchSource;
  QualFormPtr  qfp;

  qfp = (QualFormPtr) pAEOS->data;
  if (ISA_STRINGTYPE (AsnFindBaseIsa (pAEOS->atp))) {
	pchSource = (CharPtr) pAEOS->dvp->ptrvalue;
	pchFind = qfp->findStr;
	if (StringSearch (pchSource, pchFind) != NULL) {
	  qfp->stringfound = TRUE;
	}
  }
}

static Boolean QualifierHasSubstring (ObjMgrTypePtr omtp, AsnIoPtr aip, Pointer ptr, QualFormPtr qfp)

{
  qfp->stringfound = FALSE;
  (omtp->asnwrite) (ptr, aip, NULL);
  return qfp->stringfound;
}

static void CommentToNote (SeqFeatPtr sfp)

{
  GBQualPtr  gbqual;

  if (sfp == NULL || sfp->comment == NULL) return;
  gbqual = GBQualNew ();
  if (gbqual == NULL) return;
  gbqual->qual = StringSave ("note");
  gbqual->val = sfp->comment;
  sfp->comment = NULL;
  gbqual->next = sfp->qual;
  sfp->qual = gbqual;
}

static void NoteToComment (SeqFeatPtr sfp)

{
  GBQualPtr       gbqual;
  size_t          len;
  GBQualPtr       nextqual;
  GBQualPtr PNTR  prevqual;
  CharPtr         str;

  if (sfp == NULL) return;
  gbqual = sfp->qual;
  prevqual = (GBQualPtr PNTR) &(sfp->qual);
  while (gbqual != NULL) {
    nextqual = gbqual->next;
    if (StringICmp (gbqual->qual, "note") == 0) {
       *(prevqual) = gbqual->next;
       gbqual->next = NULL;
       if (sfp->comment == NULL) {
         sfp->comment = gbqual->val;
       } else {
         len = StringLen (sfp->comment) + StringLen (gbqual->val) + 5;
         str = MemNew (sizeof (Char) * len);
         StringCpy (str, sfp->comment);
         StringCat (str, "; ");
         StringCat (str, gbqual->val);
         sfp->comment = MemFree (sfp->comment);
         gbqual->val = MemFree (gbqual->val);
         sfp->comment = str;
       }
       gbqual->val = NULL;
       GBQualFree (gbqual);
    } else {
      prevqual = (GBQualPtr PNTR) &(gbqual->next);
    }
    gbqual = nextqual;
  }
}

static void EditQualifierString (QualFormPtr qfp, GBQualPtr gbqual, CharPtr foundit)

{
  size_t   diff;
  size_t   foundlen;
  size_t   replen;
  CharPtr  newstring;
  CharPtr  tmp;
  CharPtr  tmp2;

  if (qfp == NULL || gbqual == NULL || foundit == NULL) return;
  foundlen = StringLen (qfp->findStr);
  replen = StringLen (qfp->replaceStr);
  if (replen > foundlen) {
    diff = replen - foundlen;
  } else {
    diff = foundlen - replen;
  }
  newstring = MemNew (StringLen (gbqual->val) + diff + 1);
  tmp = gbqual->val;
  tmp2 = newstring;
  while (tmp != foundit) {
    *tmp2 = *tmp;
    tmp++;
    tmp2++;
  }
  if (qfp->replaceStr != NULL) {
    tmp2 = MemCopy (tmp2, qfp->replaceStr, replen);
  }
  tmp2 += replen;
  tmp += foundlen;
  tmp2 = StringMove (tmp2, tmp);
  gbqual->val = MemFree (gbqual->val);
  gbqual->val = newstring;
}

#define NUM_SITES 26
static CharPtr siteString [NUM_SITES] = {
  "", "active site", "binding site", "cleavage site", "inhibition site", "modified site",
  "glycosylation site", "myristoylation site", "mutagenized site", "metal binding site",
  "phosphorylation site", "acetylation site", "amidation site", "methylation site",
  "hydroxylation site", "sulfatation site", "oxidative deamination site",
  "pyrrolidone carboxylic acid site", "gamma carboxyglutamic acid site",
  "blocked site", "lipid binding site", "np binding site", "DNA binding site",
  "signal peptide", "transit peptide", "transmembrane region"
};

/*=========================================================================*/
/*                                                                         */
/* SeqLocAdjustByOffset ()                                                 */
/*                                                                         */
/*=========================================================================*/

extern void SeqLocAdjustByOffset (SeqLocPtr slp,
				  Int4      offset)
{
  SeqIntPtr sinp;

  switch (slp->choice) {
  case SEQLOC_INT :
    sinp = (SeqIntPtr) slp->data.ptrvalue;
    if (NULL == sinp)
      break;
    sinp->from += offset;
    sinp->to   += offset;
    break;
  case SEQLOC_EMPTY :
  case SEQLOC_NULL :
  case SEQLOC_WHOLE :
  case SEQLOC_PNT :
  case SEQLOC_PACKED_PNT :
  case SEQLOC_PACKED_INT :
  case SEQLOC_MIX :
  case SEQLOC_EQUIV :
  case SEQLOC_BOND :
  case SEQLOC_FEAT :
  default :
    break;
  }
}

/*=========================================================================*/
/*                                                                         */
/* MoveProteinFeatures ()                                                  */
/*                                                                         */
/*=========================================================================*/

static void MoveProteinFeatures (SeqFeatPtr destSfp,
				 SeqFeatPtr srcSfp,
				 Int4Ptr    offsetPtr)
{
  SeqFeatPtr copySfp;
  SeqIdPtr   destSip;

  while (NULL != srcSfp) {

    /* Make a copy of the source feature */
    
    copySfp = SeqFeatCopy (srcSfp);
    copySfp->next = NULL;
    
    /* Adjust the location of the source feature */
    
    destSip = SeqLocId (destSfp->location);
    SeqLocReplaceLocalID (copySfp->location, destSip);
    SeqLocAdjustByOffset (copySfp->location, *offsetPtr);

    /* Attach it to the end of the linked */
    /* list of destination features.      */

    if (NULL == destSfp)
      destSfp = copySfp;
    else {
      while (destSfp->next != NULL)
	destSfp = destSfp->next;
      destSfp->next = copySfp;
    }

    /* Mark the source feature to be deleted */
    
    srcSfp->idx.deleteme = TRUE;

    /* Go to the next source feature */

    srcSfp = srcSfp->next;
  }
    
  /* Return successfully */
  
  return;
}
      
/*=========================================================================*/
/*                                                                         */
/* MoveProteinAnnots ()                                                    */
/*                                                                         */
/*=========================================================================*/

static void MoveProteinAnnots (BioseqPtr destBsp,
			       BioseqPtr sourceBsp,
			       Int4Ptr   offsetPtr)
{
  SeqAnnotPtr sourceSap;
  SeqAnnotPtr destSap;
  SeqAnnotPtr lastDestSap;
  SeqFeatPtr  destSfp;
  SeqFeatPtr  sourceSfp;

  /* Find end of destination annotation list */

  if (NULL == destBsp->annot)
    lastDestSap = destBsp->annot;
  else
    for (lastDestSap = destBsp->annot;
	 lastDestSap->next != NULL;
	 lastDestSap = lastDestSap->next) {
    }

  /* For each source SeqAnnot ... */

  sourceSap = sourceBsp->annot;
  while (NULL != sourceSap) {

    /* ... if not feature table then */
    /*     add to end of dest annots */
    
    if (sourceBsp->annot->type != 1) {
      if (NULL == lastDestSap)
	lastDestSap = sourceBsp->annot;
      else {
	lastDestSap->next = sourceBsp->annot;
	lastDestSap = lastDestSap->next;
      }
      lastDestSap->next = NULL;
    }
    
    /* ... Else if feature table then  */
    /*     if there is a dest feature  */
    /*     table merge them, otherwise */
    /*     add to end of dest annots   */

    else {

      /* Find a destination feature table to merge with */

      destSap = destBsp->annot;
      while ((destSap != NULL) && (destSap->type != 1))
	destSap = destSap->next;

      /* If no destination feature table found */
      /* then just add to end of list.         */

      if (NULL == destSap) {
	if (NULL == lastDestSap)
	  lastDestSap = sourceBsp->annot;
	else {
	  lastDestSap->next = sourceBsp->annot;
	  lastDestSap = lastDestSap->next;
	}
	lastDestSap->next = NULL;
      }

      /* Otherwise, move all the features */
      /* from the source feature table to */
      /* the destination one.             */

      else {
	sourceSfp = (SeqFeatPtr) sourceBsp->annot->data;
	destSfp   = (SeqFeatPtr) destSap->data;
	MoveProteinFeatures (destSfp, sourceSfp, offsetPtr);
      }
    }

    sourceSap->idx.deleteme = TRUE;
    sourceSap = sourceSap->next;
  }
    
  return;
}

/*=========================================================================*/
/*                                                                         */
/* ConvertProtsToMatPeptides ()                                            */
/*                                                                         */
/*=========================================================================*/

static void ConvertProtsToMatPeptides (BioseqPtr bsp)
{
  SeqAnnotPtr sap;
  SeqFeatPtr  sfp;
  ProtRefPtr  prp;
  SeqLocPtr   slp;

  /* Make sure that it's a protein Bioseq */

  if (! ISA_aa (bsp->mol))
    return;

  /* Find all the protein features and */
  /* convert them to mat_peptides.     */

  sap = bsp->annot;
  while (NULL != sap) {
    if (1 == sap->type) {
      sfp = (SeqFeatPtr) sap->data;
      while (NULL != sfp) {
	if (sfp->data.choice == SEQFEAT_PROT) {
	  prp = (ProtRefPtr) sfp->data.value.ptrvalue;
	  prp->processed = 2;
	}
	sfp = sfp->next;
      }
    }
    sap = sap->next;
  }
  
  /* Add a new full-length protein feature */

  prp = ProtRefNew ();
  slp = SeqLocIntNew (0, bsp->length-1, 1, bsp->id);
  sfp = CreateNewFeatureOnBioseq (bsp, SEQFEAT_PROT, slp);
  sfp->data.value.ptrvalue = (Pointer) prp;
  sfp->next = NULL;

  /* Return successfully */

  return;
}

static void ConvertFirstMatProteinToPolyProtein (
  BioseqPtr nucBsp,
  SeqFeatPtr firstMatCDS
)
{

  SeqLocPtr firstMatLoc;
  BioseqPtr firstMatBsp;
  Uint1     strand;

  if (nucBsp == NULL
    || firstMatCDS == NULL)
  {
    return;
  }

  firstMatBsp = BioseqFindFromSeqLoc (firstMatCDS->product);
  if (firstMatBsp == NULL) return;

  strand = SeqLocStrand (firstMatCDS->location);
  firstMatLoc = firstMatCDS->location;
  firstMatCDS->location = SeqLocMerge (nucBsp, firstMatLoc, NULL, FALSE, TRUE, FALSE);
  expand_seq_loc (0, nucBsp->length - 1, strand, firstMatCDS->location);

  SetSeqLocPartial (firstMatCDS->location, TRUE, TRUE);
  firstMatCDS->partial = TRUE;
  SeqLocFree (firstMatLoc);

  firstMatBsp->seq_data = BSFree (firstMatBsp->seq_data);
  firstMatBsp->seq_data = ProteinFromCdRegion (firstMatCDS, FALSE);
  firstMatBsp->length = BSLen (firstMatBsp->seq_data);
  SetSeqFeatProduct (firstMatCDS, firstMatBsp);

}

/*=========================================================================*/
/*                                                                         */
/* MergeProteinBioseqs ()                                                  */
/*                                                                         */
/*=========================================================================*/

static Boolean MergeProteinBioseqs (BioseqSetPtr bssp)
{
  BioseqPtr          firstProtBsp = NULL;
  BioseqPtr          bsp;
  BioseqPtr          protBsp;
  SeqEntryPtr        sep;
  Int4               offset;
  SeqFeatPtr         cdsSfp;
  Boolean            found;
  SeqMgrFeatContext  fcontext;

  /* Find the nucleotide bioseq */

  found = FALSE;
  for (sep = bssp->seq_set; (sep != NULL) && (found != TRUE); sep = sep->next)
    {

      if (!IS_Bioseq (sep))
	continue;

      bsp = (BioseqPtr) sep->data.ptrvalue;
      if (ISA_na (bsp->mol))
	found = TRUE;
    }

  if (FALSE == found)
    return FALSE;

  /* Use the the CDS features to loop through the protein Bioseqs */

  cdsSfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_CDREGION, 0, &fcontext);
  while (NULL != cdsSfp)
    {
      protBsp = BioseqFindFromSeqLoc (cdsSfp->product);
      
      /* If it's the first Bioseq then */
      /* just save a pointer to it and */
      /* get its length to use as an   */
      /* offset for the next Bioseq.   */
      
      if (NULL == firstProtBsp)	{
        ConvertFirstMatProteinToPolyProtein (bsp, cdsSfp);
        protBsp = BioseqFindFromSeqLoc (cdsSfp->product);
	firstProtBsp = protBsp;
	offset = firstProtBsp->length;
      }
      
      /* Otherwise, move all of its features to */
      /* the first prot Bioseq and delete it.   */
      
      else {

	/* Move all features to 1st protein Bioseq */
	offset = fcontext.left / 3;

	MoveProteinAnnots (firstProtBsp, protBsp, &offset);

        /* Mark for deletion the original Bioseq */
	protBsp->idx.deleteme = TRUE;
        /* Mark for deletion the CDS */
        /* pointing to this Bioseq.  */
	cdsSfp->idx.deleteme = TRUE;

      }

    cdsSfp = SeqMgrGetNextFeature (protBsp, cdsSfp, SEQFEAT_CDREGION,
				   0, &fcontext);
  }

  /* Convert the protein features on the combined */
  /* protein bioseq to mat_peptides.              */

  ConvertProtsToMatPeptides (firstProtBsp);

  /* Return successfully */

  return TRUE;
}

/*----------------------------------------------------------------------*/
/*                                                                      */
/* MergeCDSForOneSet ()                                                 */
/*                                                                      */
/*----------------------------------------------------------------------*/

static Boolean LIBCALLBACK MergeCDSForOneSet (BioseqSetPtr bssp)
{
  SeqEntryPtr  childSep;
  BioseqSetPtr childBssp;

  /* Recurse down from the top until */
  /* we find a nuc/prot set.         */

  if (bssp->_class != BioseqseqSet_class_nuc_prot)
    {
      childSep = bssp->seq_set;
      
      while (NULL != childSep)
	{
	  if (IS_Bioseq_set (childSep))
	    {
	      childBssp = (BioseqSetPtr) childSep->data.ptrvalue;
	      MergeCDSForOneSet (childBssp);
	    }
	  childSep = childSep->next;
	}
      return FALSE;
    }

  /* Combine all of a nucleotide Bioseq's product */
  /* protein Bioseqs into one Bioseq.             */

  MergeProteinBioseqs (bssp);

  /* Return successfully */

  return TRUE;
}

/*=========================================================================*/
/*                                                                         */
/* MergeCDS ()                                                             */
/*                                                                         */
/*=========================================================================*/

extern void MergeCDS (IteM i);

extern void MergeCDS (IteM i)
{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;
  BioseqSetPtr bssp;

  /* Get the top level BioseqSet */

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif

  if (NULL == bfp)
    return;

  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (NULL == sep)
    return;

  WatchCursor ();
  Update ();

  if (IS_Bioseq (sep))
    return;
  else
    bssp = (BioseqSetPtr) sep->data.ptrvalue;

  /* Loop through all the Nuc-Prot sets */

  MergeCDSForOneSet (bssp);

  /* Do an update and return successfully */

  DeleteMarkedObjects (bfp->input_entityID, 0, NULL);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  ArrowCursor ();
  Update ();

  return;
}

static void FeatConvertImpToSpecialRNA (SeqFeatPtr sfp,
					Int2       toFeatSubType)
{
  RnaRefPtr          rrp;

  rrp = RnaRefNew ();
  if (rrp != NULL) {
    sfp->data.value.ptrvalue = ImpFeatFree ((ImpFeatPtr) sfp->data.value.ptrvalue);
    sfp->data.choice = SEQFEAT_RNA;
    sfp->data.value.ptrvalue = (Pointer) rrp;
    if (toFeatSubType == FEATDEF_precursor_RNA) {
      rrp->type = 1;
    } else {
      rrp->type = 255;
    }
  }
}

static void FeatConvertGeneToRNA (SeqFeatPtr  sfp,
				  Int2        toFeatSubType,
				  Int2        fromFeat,
				  QualFormPtr qfp)
{
  Int2               j;
  Char               label [256];
  BioseqPtr          productbsp;
  RnaRefPtr          rrp;
  SeqIdPtr           sip;
  tRNAPtr            trp;

  rrp = RnaRefNew ();
  if (NULL == rrp)
    return;

  FeatDefLabel (sfp, label, sizeof (label), OM_LABEL_CONTENT);
  if (fromFeat == SEQFEAT_GENE) {
    sfp->data.value.ptrvalue =
      GeneRefFree ((GeneRefPtr) sfp->data.value.ptrvalue);
  } else if (fromFeat == SEQFEAT_CDREGION) {
    if (sfp->product != NULL) {
      sip = SeqLocId (sfp->product);
      if (sip != NULL) {
	productbsp = BioseqFind (sip);
	if (productbsp != NULL) {
	  ValNodeAddPointer (&(qfp->bsplist), 0, (Pointer) productbsp);
	}
      }
    }
    sfp->data.value.ptrvalue =
      CdRegionFree ((CdRegionPtr) sfp->data.value.ptrvalue);
  }

  sfp->data.choice = SEQFEAT_RNA;
  sfp->data.value.ptrvalue = (Pointer) rrp;
  switch (toFeatSubType) {
  case FEATDEF_preRNA :
    rrp->type = 1;
    break;
  case FEATDEF_mRNA :
    rrp->type = 2;
    break;
  case FEATDEF_tRNA :
    rrp->type = 3;
    break;
  case FEATDEF_rRNA :
    rrp->type = 4;
    break;
  case FEATDEF_snRNA :
    rrp->type = 5;
    break;
  case FEATDEF_scRNA :
    rrp->type = 6;
    break;
  case FEATDEF_snoRNA :
    rrp->type = 7;
    break;
  case FEATDEF_otherRNA :
    rrp->type = 255;
    break;
  default :
    break;
  }

  if (toFeatSubType == FEATDEF_tRNA) {
    trp = (tRNAPtr) MemNew (sizeof (tRNA));
    rrp->ext.choice = 2;
    rrp->ext.value.ptrvalue = (Pointer) trp;
    if (trp != NULL) {
      trp->aa = ParseTRnaString (label, NULL, NULL, FALSE);
      trp->aatype = 2;
      for (j = 0; j < 6; j++) {
	trp->codon [j] = 255;
      }
    }
  } else if (! StringHasNoText (label)) {
    rrp->ext.choice = 1;
    rrp->ext.value.ptrvalue = StringSave (label);
  }
}

static void FeatConvertRegionToImp (SeqFeatPtr sfp,
				    Int2       toFeatSubType,
				    EnumFieldAssoc PNTR alist)
{
  EnumFieldAssocPtr  ap;
  GBQualPtr          gbqual;
  Int2               i;
  ImpFeatPtr         ifp;
  CharPtr            str;

  ifp = ImpFeatNew ();
  if (NULL == ifp)
    return;

  str = (CharPtr) sfp->data.value.ptrvalue;
  sfp->data.choice = SEQFEAT_IMP;
  sfp->data.value.ptrvalue = (Pointer) ifp;
  for (i = 1, ap = alist; ap->name != NULL; i++, ap++) {
    if (ap->value == toFeatSubType) {
      ifp->key = MemFree (ifp->key);
      ifp->key = StringSave (ap->name);
    }
  }

  if (ifp->key == NULL) {
    ifp->key = StringSave ("misc_feature");
  }

  if (! StringHasNoText (str)) {
    gbqual = GBQualNew ();
    if (gbqual != NULL) {
      gbqual->qual = StringSave ("note");
      gbqual->val = str;
      gbqual->next = sfp->qual;
      sfp->qual = gbqual;
    }
  }
  
}

static void FeatConvertRegionToRNA (SeqFeatPtr sfp,
				    Int2       toFeatSubType)
{
  RnaRefPtr  rrp;
  CharPtr    str;

  rrp = RnaRefNew ();
  if (NULL == rrp)
    return;

  str = (CharPtr) sfp->data.value.ptrvalue;
  sfp->data.choice = SEQFEAT_RNA;
  sfp->data.value.ptrvalue = (Pointer) rrp;

  if (toFeatSubType == FEATDEF_precursor_RNA) {
    rrp->type = 1;
  } else {
    rrp->type = 255;
  }

  if (! StringHasNoText (str)) {
    rrp->ext.choice = 1;
    rrp->ext.value.ptrvalue = str;
  }

}

static void FeatConvertImpToRNA (SeqFeatPtr sfp,
				 Int2       toFeatSubType)
{
  RnaRefPtr  rrp;

  rrp = RnaRefNew ();
  if (NULL == rrp)
    return;

  sfp->data.value.ptrvalue =
    ImpFeatFree ((ImpFeatPtr) sfp->data.value.ptrvalue);
  sfp->data.choice = SEQFEAT_RNA;
  sfp->data.value.ptrvalue = (Pointer) rrp;

  switch (toFeatSubType) {
  case FEATDEF_preRNA :
    rrp->type = 1;
    break;
  case FEATDEF_mRNA :
    rrp->type = 2;
    break;
  case FEATDEF_tRNA :
    rrp->type = 3;
    break;
  case FEATDEF_rRNA :
    rrp->type = 4;
    break;
  case FEATDEF_snRNA :
    rrp->type = 5;
    break;
  case FEATDEF_scRNA :
    rrp->type = 6;
    break;
  case FEATDEF_snoRNA :
    rrp->type = 7;
    break;
  case FEATDEF_otherRNA :
    rrp->type = 255;
    break;
  default :
    break;
  }

}

static void FeatConvertCommentToMiscFeature (SeqFeatPtr sfp)
{
  ImpFeatPtr ifp;

  if (sfp->data.value.ptrvalue == NULL) {
    ifp = ImpFeatNew ();
    if (ifp != NULL) {
      ifp->key = StringSave ("misc_feature");
      sfp->data.choice = SEQFEAT_IMP;
      sfp->data.value.ptrvalue = (Pointer) ifp;
    }
  }
}

static void FeatConvertGeneToMiscFeature (SeqFeatPtr sfp)
{
  ImpFeatPtr  ifp;
  GBQualPtr   gbqual;
  CharPtr     genelocus;
  GeneRefPtr  grp;

  ifp = ImpFeatNew ();
  if (NULL == ifp)
    return;

  genelocus = NULL;
  grp = (GeneRefPtr) sfp->data.value.ptrvalue;
  if (grp != NULL) {
    genelocus = grp->locus;
    grp->locus = NULL;
  }

  sfp->data.value.ptrvalue =
    GeneRefFree ((GeneRefPtr) sfp->data.value.ptrvalue);
  sfp->data.choice = SEQFEAT_IMP;
  sfp->data.value.ptrvalue = (Pointer) ifp;
  ifp->key = StringSave ("misc_feature");

  if (! StringHasNoText (genelocus)) {
    gbqual = GBQualNew ();
    if (gbqual != NULL) {
      gbqual->qual = StringSave ("gene");
      gbqual->val = genelocus;
      gbqual->next = sfp->qual;
      sfp->qual = gbqual;
    }
  }

}

static void FeatConvertRNAToMiscFeature (SeqFeatPtr sfp)
{
  GBQualPtr  gbqual;
  ImpFeatPtr ifp;
  CharPtr    rnaname;
  RnaRefPtr  rrp;

  ifp = ImpFeatNew ();
  if (NULL == ifp)
    return;

  rnaname = NULL;
  rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
  if (rrp != NULL) {
    if (rrp->ext.choice == 1) {
      rnaname = (CharPtr) rrp->ext.value.ptrvalue;
      rrp->ext.value.ptrvalue = NULL;
    }
  }

  sfp->data.value.ptrvalue =
    RnaRefFree ((RnaRefPtr) sfp->data.value.ptrvalue);
  sfp->data.choice = SEQFEAT_IMP;
  sfp->data.value.ptrvalue = (Pointer) ifp;
  ifp->key = StringSave ("misc_feature");

  if (! StringHasNoText (rnaname)) {
    gbqual = GBQualNew ();
    if (gbqual != NULL) {
      gbqual->qual = StringSave ("product");
      gbqual->val = rnaname;
      gbqual->next = sfp->qual;
      sfp->qual = gbqual;
    }
  }

}

static void FeatConvertSiteToMiscFeature (SeqFeatPtr sfp)
{
  GBQualPtr  gbqual;
  ImpFeatPtr ifp;
  Int2       sitetype;

  ifp = ImpFeatNew ();
  if (NULL == ifp)
    return;

  sitetype = (Int2) sfp->data.value.intvalue;
  sfp->data.choice = SEQFEAT_IMP;
  sfp->data.value.ptrvalue = (Pointer) ifp;
  ifp->key = StringSave ("misc_feature");
  if (sitetype > 0 && sitetype < NUM_SITES) {
    gbqual = GBQualNew ();
    if (gbqual != NULL) {
      gbqual->qual = StringSave ("note");
      gbqual->val = StringSave (siteString [sitetype]);
      gbqual->next = sfp->qual;
      sfp->qual = gbqual;
    }
  }

}

static void FeatConvertPeptideToRegion (SeqFeatPtr sfp)
{
  ProtRefPtr prp;
  ValNodePtr vnp;
  CharPtr    str;

  prp = (ProtRefPtr) sfp->data.value.ptrvalue;
  if (NULL == prp)
    return;

  vnp = prp->name;
  if (vnp != NULL && vnp->next == NULL) {
    str = (CharPtr) vnp->data.ptrvalue;
    if (! StringHasNoText (str)) {
      vnp->data.ptrvalue = NULL;
      sfp->data.value.ptrvalue = ProtRefFree (prp);
      sfp->data.choice = SEQFEAT_REGION;
      sfp->data.value.ptrvalue = (Pointer) str;
    }
  }

}


static void FeatConvertImpToImp (SeqFeatPtr     sfp,
				 Int2           toFeatSubType,
				 EnumFieldAssoc PNTR alist)
{
  EnumFieldAssocPtr  ap;
  Int2               i;
  ImpFeatPtr         ifp;

  ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
  if (NULL == ifp)
    return;

  for (i = 1, ap = alist; ap->name != NULL; i++, ap++) {
    if (ap->value == toFeatSubType) {
      ifp->key = MemFree (ifp->key);
      ifp->key = StringSave (ap->name);
    }
  }

}

static void FeatConvertRNAToRNA (SeqFeatPtr sfp,
				 Int2       toFeatSubType)
{
  RnaRefPtr  rrp;

  rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
  if (NULL == rrp)
    return;

  switch (toFeatSubType) {
  case FEATDEF_preRNA :
    rrp->type = 1;
    break;
  case FEATDEF_mRNA :
    rrp->type = 2;
    break;
  case FEATDEF_tRNA :
    rrp->type = 3;
    break;
  case FEATDEF_rRNA :
    rrp->type = 4;
    break;
  case FEATDEF_snRNA :
    rrp->type = 5;
    break;
  case FEATDEF_scRNA :
    rrp->type = 6;
    break;
  case FEATDEF_snoRNA :
    rrp->type = 7;
    break;
  case FEATDEF_otherRNA :
    rrp->type = 255;
    break;
  default:
    break;
  }

}

static void FeatConvertProtToProt (SeqFeatPtr sfp,
				   Int2       toFeatSubType)
{
  ProtRefPtr prp;

  prp = (ProtRefPtr) sfp->data.value.ptrvalue;
  if (NULL == prp)
    return;

  switch (toFeatSubType) {
  case FEATDEF_PROT :
    prp->processed = 0;
    break;
  case FEATDEF_preprotein :
    prp->processed = 1;
    break;
  case FEATDEF_mat_peptide_aa :
    prp->processed = 2;
    break;
  case FEATDEF_sig_peptide_aa :
    prp->processed = 3;
    break;
  case FEATDEF_transit_peptide_aa :
    prp->processed = 4;
    break;
  default :
    break;
  }

}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* FeatConvertImpToPeptide () - Convert a given import feature to a    */
/*                           protein feature.                          */
/*                                                                     */
/*    Note : Any of the Import feature's gbquals that can be converted */
/*           to protein fields are caught in the automatic cleanup     */
/*           called during reindexing, so they don't need to be        */
/*           converted here.                                           */
/*                                                                     */
/*---------------------------------------------------------------------*/

static void FeatConvertImpToPeptide (SeqFeatPtr  sfp,
				     Int2        toFeatSubType,
				     QualFormPtr qfp)
{
  ImpFeatPtr ifp;
  SeqFeatPtr cds;
  SeqLocPtr  slp;
  SeqFeatPtr newSfp;
  Int4       frame;
  ProtRefPtr prp;
  SeqIdPtr   sip;
  BioseqPtr  bsp;

  /* Get the Import Feature */

  ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
  if (NULL == ifp)
    return;

  /* Convert the location to a protein location */

  cds = FindBestCds (qfp->input_entityID, sfp->location, NULL, NULL);
  if (cds == NULL)
    return;

  slp = dnaLoc_to_aaLoc (cds, sfp->location, TRUE, &frame, FALSE);
  if (slp == NULL)
    return;

  /* Create a new generic feature */

  sip = SeqLocId (cds->product);
  if (sip == NULL)
    return;

  bsp = BioseqLockById (sip);
  if (bsp == NULL)
    return;

  newSfp = CreateNewFeatureOnBioseq (bsp, SEQFEAT_PROT, slp);
  BioseqUnlock (bsp);
  if (newSfp == NULL)
    return;

  /* Make it into a protein feature */

  prp = ProtRefNew ();
  newSfp->data.value.ptrvalue = (Pointer) prp;
  if (NULL == prp)
    return;

  switch (toFeatSubType) {
    case FEATDEF_mat_peptide_aa :
      prp->processed = 2;
      break;
    case FEATDEF_sig_peptide_aa :
      prp->processed = 3;
      break;
    case FEATDEF_transit_peptide_aa :
      prp->processed = 4;
      break;
  }

  sfp->idx.subtype = toFeatSubType;

  /* Transfer unchanged fields from old feature */

  newSfp->partial     = sfp->partial;
  newSfp->excpt       = sfp->excpt;
  newSfp->exp_ev      = sfp->exp_ev;
  newSfp->pseudo      = sfp->pseudo;
  newSfp->comment     = sfp->comment;
  newSfp->qual        = sfp->qual;
  newSfp->title       = sfp->title;
  newSfp->ext         = sfp->ext;
  newSfp->cit         = sfp->cit;
  newSfp->xref        = sfp->xref;
  newSfp->dbxref      = sfp->dbxref;
  newSfp->except_text = sfp->except_text;

  /* Null out pointers to transferred fields from old feature  */
  /* so that they don't get deleted when the feature does,     */

  sfp->comment     = NULL;
  sfp->qual        = NULL;
  sfp->title       = NULL;
  sfp->ext         = NULL;
  sfp->cit         = NULL;
  sfp->xref        = NULL;
  sfp->dbxref      = NULL;
  sfp->except_text = NULL;

  /* Mark the old feature for deletion */

  sfp->idx.deleteme = TRUE;
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* FeatConvertPeptideToImp () -                                        */
/*                                                                     */
/*---------------------------------------------------------------------*/

static void FeatConvertPeptideToImp (SeqFeatPtr  sfp,
				     QualFormPtr qfp)
{
  ProtRefPtr    prp;
  SeqFeatPtr    cds;
  SeqLocPtr     slp;
  ImpFeatPtr    ifp;
  CharPtr       name;
  CharPtr       ec;
  CharPtr       activity;
  ValNodePtr    vnp;
  GBQualPtr     gbqual;
  GBQualPtr     prevGbq;
  GBQualPtr     topOfGbqList;
  DbtagPtr      dbt;
  Char          idStr[64];
  ObjectIdPtr   oip;

  /* Make sure that we have a matching peptide feature */

  if (sfp->data.choice != SEQFEAT_PROT)
    return;

  prp = (ProtRefPtr) sfp->data.value.ptrvalue;
  if (NULL == prp)
    return;

  switch (qfp->subtype) {
    case FEATDEF_mat_peptide_aa :
      if (2 != prp->processed)
	return;
      break;
    case FEATDEF_sig_peptide_aa :
      if (3 != prp->processed)
	return;
      break;
    case FEATDEF_transit_peptide_aa :
      if (4 != prp->processed)
	return;
      break;
  }

  /* Convert the location from the protein */
  /* to the nucleotide Bioseq.             */

  cds = FindBestCds (qfp->input_entityID, NULL, sfp->location, NULL);
  if (NULL == cds)
    return;

  slp = aaLoc_to_dnaLoc (cds, sfp->location);
  if (NULL == slp)
    return;
  sfp->location = SeqLocFree (sfp->location);
  sfp->location = slp;

  /* Create a new import feature and */
  /* attach it to the feature.       */

  ifp = ImpFeatNew ();
  if (NULL == ifp)
    return;

  switch (qfp->subtype) {
    case FEATDEF_mat_peptide_aa :
      ifp->key = StringSave ("mat_peptide");
      break;
    case FEATDEF_sig_peptide_aa :
      ifp->key = StringSave ("sig_peptide");
      break;
    case FEATDEF_transit_peptide_aa :
      ifp->key = StringSave ("transit_peptide");
      break;
  }

  sfp->data.choice = SEQFEAT_IMP;
  sfp->data.value.ptrvalue = (Pointer) ifp;

  /* Store the protein fields as  */
  /* gbqual qualifier/value pairs */

  name = NULL;
  vnp = prp->name;
  if (vnp != NULL)
    name = vnp->data.ptrvalue;
  if (name == NULL) 
    name = prp->desc;

  if (name != NULL) {
    gbqual = GBQualNew ();
    if (NULL == gbqual)
      return;
    topOfGbqList = gbqual;
    gbqual->qual = StringSave ("product");
    gbqual->val = StringSave (name);
  }

  prevGbq = gbqual;

  ec = NULL;
  vnp = prp->ec;
  if (vnp != NULL)
    ec = (CharPtr) vnp->data.ptrvalue;
  
  if (ec != NULL) {
    gbqual = GBQualNew ();
    if (NULL == gbqual)
      return;
    prevGbq->next = gbqual;
    gbqual->qual = StringSave ("EC_number");
    gbqual->val = StringSave (ec);
  }

  prevGbq = gbqual;

  activity = NULL;
  vnp = prp->activity;
  if (vnp != NULL)
    activity = (CharPtr) vnp->data.ptrvalue;
  
  if (NULL != activity) {
    gbqual = GBQualNew ();
    if (NULL == gbqual)
      return;
    prevGbq->next = gbqual;
    gbqual->qual = StringSave ("function");
    gbqual->val = StringSave (activity);
  }

  prevGbq = gbqual;

  for (vnp = prp->db; vnp != NULL; vnp = vnp->next) {
    dbt = (DbtagPtr) vnp->data.ptrvalue;
    if (NULL == dbt ) 
      return;
    if (! StringHasNoText (dbt->db)) {
      gbqual = GBQualNew ();
      if (NULL == gbqual)
	return;
      prevGbq->next = gbqual;
      oip = dbt->tag;
      if (oip->str != NULL && (! StringHasNoText (oip->str))) {
	sprintf (idStr, "%s:%s", dbt->tag, oip->str);
	gbqual->qual = StringSave ("db_xref");
	gbqual->val = StringSave (idStr);
      } else {
	sprintf (idStr, "%s:%ld", dbt->tag, (long) oip->id);
	gbqual->qual = StringSave ("db_xref");
	gbqual->val = StringSave (idStr);
      }
      prevGbq = gbqual;
    }
  }

  /* Insert the new qualifiers in front of any existing ones */

  gbqual->next = sfp->qual;
  sfp->qual = topOfGbqList;

  /* Free the obsolete Protein reference */

  ProtRefFree (prp);
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* FeatConvertBioSrcToRepeatRegion ()                                  */
/*                                                                     */
/*---------------------------------------------------------------------*/

static void FeatConvertBioSrcToRepeatRegion (SeqFeatPtr sfp)
{
  BioSourcePtr  biop;
  Boolean       doConvert;
  GBQualPtr     gbqual;
  ImpFeatPtr    ifp;
  OrgModPtr     omp;
  SubSourcePtr  ssp;

  /* Only convert transposon and insertion_seq features */

  doConvert = FALSE;
  biop = (BioSourcePtr) sfp->data.value.ptrvalue;
  for (ssp = biop->subtype; ssp != NULL; ssp = ssp->next)
    if ((SUBSRC_transposon_name == ssp->subtype) ||
	(SUBSRC_insertion_seq_name == ssp->subtype))
      doConvert = TRUE;

  if (FALSE == doConvert)
    return;

  /* Create a new Import Feature */

  ifp = ImpFeatNew ();
  if (NULL == ifp)
    return;
  ifp->key = StringSave ("repeat_region");

  /* Copy relevant info from the BioSource */
  /* feature to the Import feature.        */

  for (ssp = biop->subtype; ssp != NULL; ssp = ssp->next) {

    if (biop->org != NULL)
      if (biop->org->orgname != NULL)
	for (omp = biop->org->orgname->mod; omp != NULL; omp = omp->next)
	  if (ORGMOD_other == omp->subtype)
	    if (!StringHasNoText (omp->subname)) {
	      gbqual = GBQualNew ();
	      if (gbqual != NULL) {
		gbqual->qual = StringSave ("note");
		gbqual->val = StringSave (omp->subname);
		gbqual->next = sfp->qual;
		sfp->qual = gbqual;
	      }
	    }

    if (SUBSRC_transposon_name == ssp->subtype) {
      if (! StringHasNoText (ssp->name)) {
	gbqual = GBQualNew ();
	if (gbqual != NULL) {
	  gbqual->qual = StringSave ("transposon");
	  gbqual->val = StringSave (ssp->name);
	  gbqual->next = sfp->qual;
	  sfp->qual = gbqual;
	}
      }
    }
    else if (SUBSRC_insertion_seq_name == ssp->subtype) {
      if (! StringHasNoText (ssp->name)) {
	gbqual = GBQualNew ();
	if (gbqual != NULL) {
	  gbqual->qual = StringSave ("insertion_seq");
	  gbqual->val = StringSave (ssp->name);
	  gbqual->next = sfp->qual;
	  sfp->qual = gbqual;
	}
      }
    }
    else if (SUBSRC_other == ssp->subtype) {
      if (! StringHasNoText (ssp->name)) {
	gbqual = GBQualNew ();
	if (gbqual != NULL) {
	  gbqual->qual = StringSave ("note");
	  gbqual->val = StringSave (ssp->name);
	  gbqual->next = sfp->qual;
	  sfp->qual = gbqual;
	}
      }
    }
  }
  
  /* Delete the old BioSource feature */

  sfp->data.value.ptrvalue = BioSourceFree (biop);

  /* Attach the new Import feature in its place */

  sfp->data.choice = SEQFEAT_IMP;
  sfp->data.value.ptrvalue = ifp;
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* FeatConvert () -- Convert a given feature from one type to another.     */
/*                                                                         */
/*-------------------------------------------------------------------------*/

static Boolean FeatConvert (QualFormPtr qfp, 
			    SeqFeatPtr  sfp,
			    Int2        toFeatSubType)
{
  Uint2 fromFeat;
  Uint2 toFeat;

  fromFeat = FindFeatFromFeatDefType (qfp->subtype);
  toFeat   = FindFeatFromFeatDefType (toFeatSubType);

  if (fromFeat == SEQFEAT_IMP &&
      toFeat   == SEQFEAT_PROT)
    FeatConvertImpToPeptide (sfp, toFeatSubType, qfp);
  else if (fromFeat == SEQFEAT_PROT &&
	   toFeat   == SEQFEAT_IMP &&
	   qfp->subtype == FEATDEF_mat_peptide_aa)
    FeatConvertPeptideToImp (sfp, qfp);
  else if (fromFeat == SEQFEAT_PROT &&
	   toFeat   == SEQFEAT_IMP &&
	   qfp->subtype == FEATDEF_sig_peptide_aa)
    FeatConvertPeptideToImp (sfp, qfp);
  else if (fromFeat == SEQFEAT_PROT &&
	   toFeat   == SEQFEAT_IMP &&
	   qfp->subtype == FEATDEF_transit_peptide_aa)
    FeatConvertPeptideToImp (sfp, qfp);
  else if (fromFeat == SEQFEAT_IMP &&
	   (toFeatSubType == FEATDEF_misc_RNA ||
	    toFeatSubType == FEATDEF_precursor_RNA))
    FeatConvertImpToSpecialRNA (sfp, toFeatSubType);
  else if ((fromFeat == SEQFEAT_GENE ||
	    fromFeat == SEQFEAT_CDREGION) &&
	   toFeat == SEQFEAT_RNA)
    FeatConvertGeneToRNA (sfp, toFeatSubType, fromFeat, qfp);
  else if (fromFeat == SEQFEAT_REGION &&
	   toFeat == SEQFEAT_IMP)
    FeatConvertRegionToImp (sfp, toFeatSubType, qfp->alist);
  else if (fromFeat == SEQFEAT_REGION &&
	   (toFeatSubType == FEATDEF_misc_RNA ||
	    toFeatSubType == FEATDEF_precursor_RNA))
    FeatConvertRegionToRNA (sfp, toFeatSubType);
  else if (fromFeat == SEQFEAT_IMP &&
	   toFeat == SEQFEAT_RNA)
    FeatConvertImpToRNA (sfp, toFeatSubType);
  else if (fromFeat == SEQFEAT_COMMENT &&
	   toFeatSubType == FEATDEF_misc_feature)
    FeatConvertCommentToMiscFeature (sfp);
  else if (fromFeat == SEQFEAT_GENE &&
	   toFeatSubType == FEATDEF_misc_feature)
    FeatConvertGeneToMiscFeature (sfp);
  else if (fromFeat == SEQFEAT_RNA &&
	   toFeatSubType == FEATDEF_misc_feature)
    FeatConvertRNAToMiscFeature (sfp);
  else if (fromFeat == SEQFEAT_SITE &&
	   toFeatSubType == FEATDEF_misc_feature)
    FeatConvertSiteToMiscFeature (sfp);
  else if (fromFeat == SEQFEAT_PROT &&
	   (qfp->subtype == FEATDEF_mat_peptide_aa ||
	    qfp->subtype == FEATDEF_sig_peptide_aa ||
	    qfp->subtype == FEATDEF_transit_peptide_aa) &&
	   toFeat == SEQFEAT_REGION)
    FeatConvertPeptideToRegion (sfp);
  else if (fromFeat == SEQFEAT_BIOSRC &&
	   toFeat == SEQFEAT_IMP &&
	   toFeatSubType == FEATDEF_repeat_region)
    FeatConvertBioSrcToRepeatRegion (sfp);
  else if (fromFeat != toFeat)
    {
      ArrowCursor ();
      Message (MSG_OK, "This conversion not supported - contact"
	       " sequindev for instructions.");
      qfp->abortconvert = TRUE;
      return FALSE;
    }
  else if (fromFeat == SEQFEAT_IMP)
    FeatConvertImpToImp (sfp, toFeatSubType, qfp->alist);
  else if (fromFeat == SEQFEAT_RNA)
    FeatConvertRNAToRNA (sfp, toFeatSubType);
  else if (fromFeat == SEQFEAT_PROT)
    FeatConvertProtToProt (sfp, toFeatSubType);

  return TRUE;
}

static GbFeatName AddQualifierList[] = {
 {"allele", Class_text}, {"anticodon", Class_pos_aa},
 {"bound_moiety", Class_text},
 {"chromosome", Class_text},
 {"citation", Class_bracket_int},
 {"codon", Class_seq_aa},
 {"codon_start", Class_int_or}, {"cons_splice", Class_site},
 {"db_xref", Class_text},
 {"direction", Class_L_R_B}, {"EC_number", Class_ecnum},
 {"evidence", Class_exper}, {"exception", Class_text},
 {"frequency", Class_text}, {"function", Class_text},
 {"gene", Class_text}, {"gdb_xref", Class_text},
 {"insertion_seq", Class_text},
 {"label", Class_token},
 {"map", Class_text},
 {"mod_base", Class_token}, {"note", Class_note},
 {"number", Class_number}, {"organism", Class_text},
 {"partial", Class_none}, {"PCR_conditions", Class_text},
 {"phenotype", Class_text},
 {"plasmid", Class_text}, {"product", Class_text},
 {"pseudo", Class_none},
 {"rearranged", Class_none}, { "replace", Class_text},
 {"rpt_family", Class_text}, {"rpt_type", Class_rpt},
 {"rpt_unit", Class_token},
 {"sequenced_mol", Class_text},
 {"standard_name", Class_text},
 {"translation", Class_text}, {"transl_except", Class_pos_aa},
 {"transl_table", Class_int}, {"transposon", Class_text},
 {"usedin", Class_token},
 {"focus", Class_none},
 {"protein_id", Class_text},
 {"organelle", Class_text}, {"transcript_id", Class_text},
 {"transgenic", Class_none}, {"environmental_sample", Class_none},
 {"locus_tag", Class_text}, {"mol_type", Class_text},
};

const Int4 NumAddQualifiers = sizeof (AddQualifierList) / sizeof (GbFeatName);

static Int2 AddQualifierNameValid (CharPtr qual)
{
   Int2  i;

   for (i = 0; i < NumAddQualifiers && qual != NULL; i++) {
       if (StringCmp(qual, AddQualifierList[i].name) == 0)
          return (i);
   }

   return (-1);

}

static void QualAdd (QualFormPtr qfp,
		     SeqFeatPtr  sfp,
		     Int2        fromFeatSubType)
{
  GBQualPtr  gbqual;

  if (fromFeatSubType - 1 > NumAddQualifiers || fromFeatSubType < 1) return;
  if (StringCmp (AddQualifierList[fromFeatSubType - 1].name, "partial") == 0)
  {
    sfp->partial = FALSE;
  }
  else if (StringCmp (AddQualifierList[fromFeatSubType - 1].name, "evidence") == 0)
  {
    sfp->exp_ev = 0;
  }
  else {
    gbqual = GBQualNew ();
    if (gbqual != NULL) {
      gbqual->qual = 
	StringSave (AddQualifierList[fromFeatSubType - 1].name);
      gbqual->val = StringSave (qfp->findStr);
      gbqual->next = sfp->qual;
      sfp->qual = gbqual;
      if (StringCmp (AddQualifierList[fromFeatSubType - 1].name, "note") == 0)
	NoteToComment (sfp);
    }
  }
}

static GbFeatName RemoveQualifierList[] = {
 {"allele", Class_text}, {"anticodon", Class_pos_aa},
 {"bound_moiety", Class_text}, {"citation", Class_bracket_int},
 {"codon", Class_seq_aa}, {"codon_start", Class_int_or},
 {"cons_splice", Class_site}, {"db_xref", Class_text},
 {"direction", Class_L_R_B}, {"EC_number", Class_ecnum},
 {"evidence", Class_exper}, {"exception", Class_text},
 {"frequency", Class_text}, {"function", Class_text},
 {"gene", Class_text}, {"gdb_xref", Class_text},
 {"insertion_seq", Class_text}, {"label", Class_token},
 {"map", Class_text}, {"mod_base", Class_token},
 {"note", Class_note}, {"number", Class_number}, {"organism", Class_text},
 {"partial", Class_none}, {"PCR_conditions", Class_text},
 {"phenotype", Class_text}, {"plasmid", Class_text},
 {"product", Class_text}, {"pseudo", Class_none},
 {"rearranged", Class_none}, { "replace", Class_text},
 {"rpt_family", Class_text}, {"rpt_type", Class_rpt},
 { "rpt_unit", Class_token}, {"sequenced_mol", Class_text},
 {"standard_name", Class_text}, {"translation", Class_text},
 {"transl_except", Class_pos_aa}, {"transl_table", Class_int},
 {"transposon", Class_text}, {"usedin", Class_token},
 {"focus", Class_none}, {"protein_id", Class_text},
 {"organelle", Class_text}, {"transcript_id", Class_text},
 {"transgenic", Class_none}, {"environmental_sample", Class_none},
 {"locus_tag", Class_text}, {"mol_type", Class_text},
};

const Int4 NumRemoveQualifiers = sizeof (RemoveQualifierList) / sizeof (GbFeatName);

static Int2 RemoveQualifierNameValid (CharPtr qual)
{
   Int2  i;

   for (i = 0; i < NumRemoveQualifiers && qual != NULL; i++) {
       if (StringCmp(qual, RemoveQualifierList[i].name) == 0)
          return (i);
   }

   return (-1);

}

static GbFeatName ConvertQualifierList[] = {
 {"allele", Class_text}, {"anticodon", Class_pos_aa},
 {"bound_moiety", Class_text},
 {"chromosome", Class_text},
 {"citation", Class_bracket_int},
 {"codon", Class_seq_aa},
 {"codon_start", Class_int_or}, {"cons_splice", Class_site},
 {"db_xref", Class_text},
 {"direction", Class_L_R_B}, {"EC_number", Class_ecnum},
 {"evidence", Class_exper}, {"exception", Class_text},
 {"frequency", Class_text}, {"function", Class_text},
 {"gene", Class_text}, {"gdb_xref", Class_text},
 {"insertion_seq", Class_text},
 {"label", Class_token},
 {"map", Class_text},
 {"mod_base", Class_token}, {"note", Class_note},
 {"number", Class_number}, {"organism", Class_text},
 {"partial", Class_none}, {"PCR_conditions", Class_text},
 {"phenotype", Class_text},
 {"plasmid", Class_text}, {"product", Class_text},
 {"pseudo", Class_none},
 {"rearranged", Class_none}, { "replace", Class_text},
 {"rpt_family", Class_text}, {"rpt_type", Class_rpt},
 { "rpt_unit", Class_token},
 {"sequenced_mol", Class_text},
 {"standard_name", Class_text},
 {"translation", Class_text}, {"transl_except", Class_pos_aa},
 {"transl_table", Class_int}, {"transposon", Class_text},
 {"usedin", Class_token},
 {"focus", Class_none},
 {"protein_id", Class_text},
 {"organelle", Class_text}, {"transcript_id", Class_text},
 {"transgenic", Class_none}, {"environmental_sample", Class_none},
 {"locus_tag", Class_text}, {"mol_type", Class_text},
 {"segment", Class_text}
};

const Int4 NumConvertQualifiers = sizeof (ConvertQualifierList) / sizeof (GbFeatName);

static Int2 ConvertQualifierNameValid (CharPtr qual)
{
   Int2  i;

   for (i = 0; i < NumConvertQualifiers && qual != NULL; i++) {
       if (StringCmp(qual, ConvertQualifierList[i].name) == 0)
          return (i);
   }

   return (-1);

}

static GbFeatName EditQualifierList[] = {
 {"allele", Class_text}, {"anticodon", Class_pos_aa},
 {"bound_moiety", Class_text},
 {"chromosome", Class_text},
 {"citation", Class_bracket_int},
 {"codon", Class_seq_aa},
 {"codon_start", Class_int_or}, {"cons_splice", Class_site},
 {"db_xref", Class_text},
 {"direction", Class_L_R_B}, {"EC_number", Class_ecnum},
 {"evidence", Class_exper}, {"exception", Class_text},
 {"frequency", Class_text}, {"function", Class_text},
 {"gene", Class_text}, {"gdb_xref", Class_text},
 {"insertion_seq", Class_text},
 {"label", Class_token},
 {"map", Class_text},
 {"mod_base", Class_token}, {"note", Class_note},
 {"number", Class_number}, {"organism", Class_text},
 {"partial", Class_none}, {"PCR_conditions", Class_text},
 {"phenotype", Class_text},
 {"plasmid", Class_text}, {"product", Class_text},
 {"pseudo", Class_none},
 {"rearranged", Class_none}, { "replace", Class_text},
 {"rpt_family", Class_text}, {"rpt_type", Class_rpt},
 { "rpt_unit", Class_token},
 {"sequenced_mol", Class_text},
 {"standard_name", Class_text},
 {"translation", Class_text}, {"transl_except", Class_pos_aa},
 {"transl_table", Class_int}, {"transposon", Class_text},
 {"usedin", Class_token},
 {"focus", Class_none},
 {"protein_id", Class_text},
 {"organelle", Class_text}, {"transcript_id", Class_text},
 {"transgenic", Class_none}, {"environmental_sample", Class_none},
 {"locus_tag", Class_text}, {"mol_type", Class_text},
 {"segment", Class_text}
};

const Int4 NumEditQualifiers = sizeof (EditQualifierList) / sizeof (GbFeatName);

static Int2 EditQualifierNameValid (CharPtr qual)
{
   Int2  i;

   for (i = 0; i < NumEditQualifiers && qual != NULL; i++) {
       if (StringCmp(qual, EditQualifierList[i].name) == 0)
          return (i);
   }

   return (-1);

}

static void QualConvertEditRemove (QualFormPtr qfp,
				   SeqFeatPtr  sfp,
				   Int2        fromFeatSubType,
				   Int2        toFeatSubType)
{
  CharPtr         foundit;
  GBQualPtr       gbqual;
  GBQualPtr       nextqual;
  GBQualPtr PNTR  prevqual;
  Int2            qual;

  if (qfp->type == REMOVE_QUAL)
  {
    if ( fromFeatSubType < NumRemoveQualifiers
        && StringCmp (RemoveQualifierList[fromFeatSubType - 1].name, "note") == 0)
      CommentToNote (sfp);
  }
  else if (qfp->type == ADD_QUAL)
  {
    if (fromFeatSubType < NumAddQualifiers
        && StringCmp (AddQualifierList[fromFeatSubType -1].name, "note") == 0)
      CommentToNote (sfp);
  }
  else if (qfp->type == CONVERT_QUAL)
  {
    if (fromFeatSubType < NumConvertQualifiers
        && StringCmp (ConvertQualifierList[fromFeatSubType -1].name, "note") == 0)
      CommentToNote (sfp);
  }
  else if (qfp->type == EDIT_QUAL)
  {
    if (fromFeatSubType < NumEditQualifiers
        && StringCmp (EditQualifierList[fromFeatSubType - 1].name, "note") == 0)
      CommentToNote (sfp);
  }
  else
  {
    if (fromFeatSubType - 1 == GBQUAL_note ||
        toFeatSubType - 1   == GBQUAL_note)
      CommentToNote (sfp);
  }

  if (qfp->type != CONVERT_QUAL) {
    if (qfp->type == REMOVE_QUAL)
    {
      if (fromFeatSubType < NumRemoveQualifiers)
      {
        if (StringCmp (RemoveQualifierList[fromFeatSubType - 1].name, "partial") == 0)
          sfp->partial = FALSE;
        else if (StringCmp (RemoveQualifierList[fromFeatSubType - 1].name, "evidence") == 0)
          sfp->exp_ev = 0;
      }
    }
    else if (qfp->type == ADD_QUAL)
    {
      if (fromFeatSubType < NumAddQualifiers)
      {
        if (StringCmp (AddQualifierList[fromFeatSubType - 1].name, "partial") == 0)
          sfp->partial = FALSE;
        else if (StringCmp (AddQualifierList[fromFeatSubType - 1].name, "evidence") == 0)
          sfp->exp_ev = 0;
      }
    }
    else if (qfp->type == EDIT_QUAL)
    {
      if (fromFeatSubType < NumEditQualifiers)
      {
        if (StringCmp (EditQualifierList[fromFeatSubType - 1].name, "partial") == 0)
          sfp->partial = FALSE;
        else if (StringCmp (EditQualifierList[fromFeatSubType - 1].name, "evidence") == 0)
          sfp->exp_ev = 0;
      }
    }
    else
    {
      if (fromFeatSubType - 1 == GBQUAL_partial)
        sfp->partial = FALSE;
      else if (fromFeatSubType - 1 == GBQUAL_evidence)
        sfp->exp_ev = 0;
    }
  }

  gbqual = sfp->qual;
  prevqual = (GBQualPtr PNTR) &(sfp->qual);

  while (gbqual != NULL) {
    nextqual = gbqual->next;
    if (qfp->type == REMOVE_QUAL)
      qual = RemoveQualifierNameValid (gbqual->qual);
    else if (qfp->type == ADD_QUAL)
      qual = AddQualifierNameValid (gbqual->qual);
    else if (qfp->type == CONVERT_QUAL)
      qual = ConvertQualifierNameValid (gbqual->qual);
    else if (qfp->type == EDIT_QUAL)
      qual = EditQualifierNameValid (gbqual->qual);
    else
      qual = GBQualNameValid (gbqual->qual);
    if (qual > -1 && qual == fromFeatSubType - 1) {
      if (qfp->type == CONVERT_QUAL) {
	gbqual->qual = MemFree (gbqual->qual);
	gbqual->qual = StringSave (ConvertQualifierList [toFeatSubType - 1].name);
	prevqual = (GBQualPtr PNTR) &(gbqual->next);
      } else if (qfp->type == EDIT_QUAL) {
	foundit = StringStr (gbqual->val, qfp->findStr);
	if (foundit != NULL) {
	  EditQualifierString (qfp, gbqual, foundit);
	}
	prevqual = (GBQualPtr PNTR) &(gbqual->next);
      } else {
	*(prevqual) = gbqual->next;
	gbqual->next = NULL;
	GBQualFree (gbqual);
      }
    } else {
      prevqual = (GBQualPtr PNTR) &(gbqual->next);
    }
    gbqual = nextqual;
  }

  if (qfp->type == REMOVE_QUAL)
  {
    if ( fromFeatSubType < NumRemoveQualifiers
        && StringCmp (RemoveQualifierList[fromFeatSubType - 1].name, "note") == 0)
      NoteToComment (sfp);
  }
  else if (qfp->type == ADD_QUAL)
  {
    if ( fromFeatSubType < NumAddQualifiers
        && StringCmp (AddQualifierList[fromFeatSubType - 1].name, "note") == 0)
      NoteToComment (sfp);
  }
  else if (qfp->type == CONVERT_QUAL)
  {
    if ( fromFeatSubType < NumConvertQualifiers
        && StringCmp (ConvertQualifierList[fromFeatSubType - 1].name, "note") == 0)
      NoteToComment (sfp);
  }
  else if (qfp->type == EDIT_QUAL)
  {
    if ( fromFeatSubType < NumEditQualifiers
        && StringCmp (EditQualifierList[fromFeatSubType - 1].name, "note") == 0)
      NoteToComment (sfp);
  }
  else
  {
    if (fromFeatSubType - 1 == GBQUAL_note ||
        toFeatSubType - 1   == GBQUAL_note) {
      NoteToComment (sfp);
    }
  }
}

static void CommonQualifierCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  AsnExpOptPtr       aeop;
  AsnIoPtr           aip;
  BioseqPtr          bsp;
  BioseqSetPtr       bssp;
  UIEnum             enumval;
  Int2               lookfor;
  Int2               newval;
  SeqAnnotPtr        nextsap;
  SeqFeatPtr         nextsfp;
  Boolean            notext;
  ObjMgrTypePtr      omtp;
  QualFormPtr        qfp;
  SeqAnnotPtr        sap;
  SeqFeatPtr         sfp;
  Uint2              subtype;
  Int2               val;

  if (mydata == NULL) return;
  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  qfp = (QualFormPtr) mydata;
  if (qfp == NULL) return;
  if (qfp->abortconvert) return;
  omtp = qfp->omtp;
  if (omtp == NULL || omtp->subtypefunc == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
  } else return;
  val = 0;
  newval = 0;
  switch (qfp->type) {
    case REMOVE_QUAL :
      val = GetValue (qfp->fromquallist);
      if (val == 0) return;
      break;
    case CONVERT_FEAT :
      if (GetEnumPopup ((PopuP) qfp->tofeatlist, qfp->alist, &enumval)) {
        newval = (Int2) enumval;
      }
      if (newval == 0) return;
      break;
    case CONVERT_QUAL :
      val = GetValue (qfp->fromquallist);
      if (val == 0) return;
      newval = GetValue (qfp->toquallist);
      if (newval == 0) return;
      break;
    case EDIT_QUAL :
      val = GetValue (qfp->fromquallist);
      if (val == 0) return;
      break;
    case ADD_QUAL :
      val = GetValue (qfp->fromquallist);
      if (val == 0) return;
      break;
    default :
      return;
  }
  qfp->findStr = JustSaveStringFromText (qfp->findthis);
  notext = StringHasNoText (qfp->findStr);
  qfp->replaceStr = JustSaveStringFromText (qfp->replacewith);
  if (qfp->type == EDIT_QUAL && notext) return;
  if (qfp->type == ADD_QUAL && notext) return;
  aip = AsnIoNullOpen ();
  aeop = AsnExpOptNew (aip, NULL, NULL, AsnWriteQualifierForDCallBack);
  if (aeop != NULL) {
    aeop->user_data = (Pointer) qfp;
  }
  while (sap != NULL) {
    nextsap = sap->next;
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      while (sfp != NULL) {
        nextsfp = sfp->next;
        subtype = (*(omtp->subtypefunc)) ((Pointer) sfp);
        lookfor = qfp->subtype;

	if ((SEQFEAT_IMP == sfp->data.choice) &&
	   ((subtype == FEATDEF_mat_peptide ||
	     subtype == FEATDEF_sig_peptide ||
	     subtype == FEATDEF_transit_peptide)))
	  lookfor = subtype;

        if (qfp->type != CONVERT_FEAT){
          if (qfp->subtype == 0)
            lookfor = subtype;
        }

        if (subtype == lookfor) {
          if (qfp->type == ADD_QUAL)
	    QualAdd (qfp, sfp, val);
	  else if (notext || QualifierHasSubstring (omtp, aip,
						      (Pointer) sfp, qfp)) {
            if (qfp->type == CONVERT_FEAT) {
	      if (FeatConvert (qfp, sfp, newval) == FALSE)
		return;
	    }
	    else
	      QualConvertEditRemove (qfp, sfp, val, newval);
          }
          if (qfp->type == REMOVE_QUAL
		&& val < NumRemoveQualifiers
		&& StringCmp (RemoveQualifierList[val - 1].name, "citation") == 0) {
            sfp->cit = PubSetFree (sfp->cit);
          }
        }
        sfp = nextsfp;
      }
    }
    sap = nextsap;
  }
  AsnIoClose (aip);
}

static void DoProcessQualifier (ButtoN b)

{
  BioseqPtr      bsp;
  Uint2          choice;
  UIEnum         enumval;
  Uint2          itemID;
  Uint2          newchoice;
  OMProcControl  ompc;
  QualFormPtr    qfp;
  SeqEntryPtr    sep;
  ValNodePtr     tmp;
  Int2           val;

  /* Check for valid conditions */

  qfp = (QualFormPtr) GetObjectExtra (b);
  if (qfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (qfp->input_entityID);
  if (sep == NULL) return;

  /* Hide the popup and initialize */

  Hide (qfp->form);
  WatchCursor ();
  Update ();
  qfp->itemtype = OBJ_SEQFEAT;
  qfp->subtype = 0;
  qfp->abortconvert = FALSE;
  qfp->bsplist = NULL;

  /* Get the 'from' feature type */

  val = 0;
  if (GetEnumPopup ((PopuP) qfp->fromfeatlist, qfp->alist, &enumval)) {
    val = (Int2) enumval;
  }
  if (val > 0) {
    qfp->subtype = val;
  }

  /**/

  if (qfp->subtype != 0 || qfp->type != CONVERT_FEAT) {
    qfp->omp = ObjMgrGet ();
    qfp->omtp = NULL;
    if (qfp->omp != NULL) {
      qfp->omtp = ObjMgrTypeFind (qfp->omp, OBJ_SEQFEAT, NULL, NULL);
    }
    if (qfp->type == CONVERT_FEAT) {
      val = 0;
      if (GetEnumPopup ((PopuP) qfp->tofeatlist, qfp->alist, &enumval)) {
        val = (Int2) enumval;
      }
      if (val > 0) {
        choice = FindFeatFromFeatDefType (qfp->subtype);
        newchoice = FindFeatFromFeatDefType (val);

        if (choice == SEQFEAT_IMP && newchoice == SEQFEAT_CDREGION) {
          ArrowCursor ();
          Update ();
          qfp->findStr = JustSaveStringFromText (qfp->findthis);
          PrepareToConvertToCDS (sep, qfp->input_entityID,
				 qfp->subtype, qfp->findStr);
          if (GetStatus (qfp->leaveDlgUp)) {
            Show (qfp->form);
          } else {
            Remove (qfp->form);
          }
          return;
	}
      }
    }
    if (qfp->itemtype != 0 && qfp->omtp != NULL) {
      SeqEntryExplore (sep, (Pointer) qfp, CommonQualifierCallback);
    }
  }

  /**/

  ArrowCursor ();
  Update ();
  if (qfp->bsplist != NULL) {
    if (Message (MSG_YN, "Remove protein products?") == ANS_YES) {
      for (tmp = qfp->bsplist; tmp != NULL; tmp = tmp->next) {
        bsp = (BioseqPtr) tmp->data.ptrvalue;
        itemID = GetItemIDGivenPointer (qfp->input_entityID, OBJ_BIOSEQ, (Pointer) bsp);
        if (itemID > 0) {
          MemSet ((Pointer) (&ompc), 0, sizeof (OMProcControl));
          ompc.do_not_reload_from_cache = TRUE;
          ompc.input_entityID = qfp->input_entityID;
          ompc.input_itemID = itemID;
          ompc.input_itemtype = OBJ_BIOSEQ;
          if (! DetachDataForProc (&ompc, FALSE)) {
            Message (MSG_POSTERR, "DetachDataForProc failed");
          }
        }
      }
    }
    RenormalizeNucProtSets (sep, TRUE);
  }

  /* Force an update */

  DeleteMarkedObjects (qfp->input_entityID, 0, NULL);
  ObjMgrSetDirtyFlag (qfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, qfp->input_entityID, 0, 0);
  if (GetStatus (qfp->leaveDlgUp)) {
    Show (qfp->form);
  } else {
    Remove (qfp->form);
  }
}

static void QualMessageProc (ForM f, Int2 mssg)

{
  QualFormPtr  qfp;

  qfp = (QualFormPtr) GetObjectExtra (f);
  if (qfp != NULL) {
    if (qfp->appmessage != NULL) {
      qfp->appmessage (f, mssg);
    }
  }
}

static void CleanupQualForm (GraphiC g, VoidPtr data)

{
  Int2         j;
  QualFormPtr  qfp;

  qfp = (QualFormPtr) data;
  if (qfp != NULL) {
    MemFree (qfp->findStr);
    MemFree (qfp->replaceStr);
    if (qfp->alist != NULL
      && qfp->alist != qfp->realalist
      && qfp->alist != qfp->realalist + 1)
    {
      for (j = 0; qfp->alist [j].name != NULL; j++) {
        MemFree (qfp->alist [j].name);
      }
      MemFree (qfp->alist);
    }
    if (qfp->realalist != NULL) {
      for (j = 0; qfp->realalist [j].name != NULL; j++) {
        MemFree (qfp->realalist [j].name);
      }
    }
    MemFree (qfp->realalist);
    ValNodeFree (qfp->bsplist);
  }
  StdCleanupFormProc (g, data);
}

static void ProcessQualifier (IteM i, Int2 type)

{
  EnumFieldAssocPtr  ap;
  BaseFormPtr        bfp;
  ButtoN             b;
  GrouP              c;
  GrouP              g;
  GrouP              h;
  Int2               j;
  GrouP              k;
  Int2               listHeight;
  QualFormPtr        qfp;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
  CharPtr            str;
  CharPtr            title;
  WindoW             w;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  qfp = (QualFormPtr) MemNew (sizeof (QualFormData));
  if (qfp == NULL) return;
  qfp->type = type;
  switch (type) {
    case REMOVE_QUAL :
      title = "Qualifier Removal";
      break;
    case CONVERT_FEAT :
      title = "Feature Conversion";
      break;
    case CONVERT_QUAL :
      title = "Qualifier Conversion";
      break;
    case EDIT_QUAL :
      title = "Edit Qualifier";
      break;
    case ADD_QUAL :
      title = "Add Qualifier";
      break;
    default :
      title = "?";
      break;
  }
  w = FixedWindow (-50, -33, -10, -10, title, StdCloseWindowProc);
  SetObjectExtra (w, qfp, CleanupQualForm);
  qfp->form = (ForM) w;
  qfp->formmessage = QualMessageProc;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    qfp->appmessage = sepp->handleMessages;
  }

  qfp->input_entityID = bfp->input_entityID;
  qfp->input_itemID = bfp->input_itemID;
  qfp->input_itemtype = bfp->input_itemtype;

  if (type == CONVERT_FEAT) {
    ap = import_featdef_alist (TRUE, FALSE, TRUE);
  } else {
    ap = import_featdef_alist (FALSE, FALSE, FALSE);
  }
  SortEnumFieldAssocPtrArray (ap, CompareImpFeatEnumFieldAssoc);
  qfp->realalist = ap;
  if (type == CONVERT_FEAT) {
    ap++;
  } else {
    ap->name = MemFree (ap->name);
    ap->name = StringSave ("[ALL FEATURES]");
  }
 
  if (type == CONVERT_FEAT)
  { 
    qfp->alist = InsertMostUsedFeatureEnumFieldAssoc (ap);
  }
  else
  {
    qfp->alist = ap;
  }

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  g = HiddenGroup (h, 0, 2, NULL);

  if (type == CONVERT_FEAT) {
    StaticPrompt (g, "From Feature", 0, 0, programFont, 'c');
  } else {
    StaticPrompt (g, "Feature", 0, 0, programFont, 'c');
  }
  if (indexerVersion) {
    listHeight = 16;
  } else {
    listHeight = 8;
  }
  qfp->fromfeatlist = SingleList (g, 10, listHeight, NULL);
  for (ap = qfp->alist; ap->name != NULL; ap++) {
    ListItem (qfp->fromfeatlist, ap->name);
  }

  if (type == CONVERT_FEAT) {
    StaticPrompt (g, "To Feature", 0, 0, programFont, 'c');
    qfp->tofeatlist = SingleList (g, 10, listHeight, NULL);
    for (ap = qfp->alist; ap->name != NULL; ap++) {
      ListItem (qfp->tofeatlist, ap->name);
    }
  } else {
    if (type == CONVERT_QUAL) {
      StaticPrompt (g, "From Qualifier", 0, 0, programFont, 'c');
    } else {
      StaticPrompt (g, "Qualifier", 0, 0, programFont, 'c');
    }
    qfp->fromquallist = SingleList (g, 10, listHeight, NULL);
    if (type == REMOVE_QUAL)
    {
      for (j = 0; j < NumRemoveQualifiers; j++) {
        ListItem (qfp->fromquallist, RemoveQualifierList [j].name);
      }
    }
    else if (type == ADD_QUAL)
    {
      for (j = 0; j < NumAddQualifiers; j++) {
        ListItem (qfp->fromquallist, AddQualifierList [j].name);
      }
    }
    else if (type == CONVERT_QUAL)
    {
      for (j = 0; j < NumConvertQualifiers; j++) {
        ListItem (qfp->fromquallist, ConvertQualifierList [j].name);
      }
    }
    else if (type == EDIT_QUAL)
    {
      for (j = 0; j < NumEditQualifiers; j++) {
        ListItem (qfp->fromquallist, EditQualifierList [j].name);
      }
    }
    else
    {
      for (j = 0; j < ParFlat_TOTAL_GBQUAL; j++) {
        ListItem (qfp->fromquallist, ParFlat_GBQual_names [j].name);
      }
    }
    if (type == CONVERT_QUAL) {
      StaticPrompt (g, "To Qualifier", 0, 0, programFont, 'c');
      qfp->toquallist = SingleList (g, 10, listHeight, NULL);
      for (j = 0; j < NumConvertQualifiers; j++) {
        ListItem (qfp->toquallist, ConvertQualifierList [j].name);
      }
    }
  }

  if (type == EDIT_QUAL) {
    k = HiddenGroup (h, 2, 0, NULL);
  } else {
    k = HiddenGroup (h, 0, 2, NULL);
  }
  if (type == EDIT_QUAL) {
    StaticPrompt (k, "Find", 0, dialogTextHeight, programFont, 'l');
  } else if (type == ADD_QUAL) {
    StaticPrompt (k, "Text", 0, dialogTextHeight, programFont, 'l');
  } else {
    StaticPrompt (k, "Optional string constraint", 0, dialogTextHeight, programFont, 'c');
  }
  qfp->findthis = DialogText (k, "", 14, NULL);
  if (type == EDIT_QUAL) {
    StaticPrompt (k, "Replace", 0, dialogTextHeight, programFont, 'l');
    qfp->replacewith = DialogText (k, "", 14, NULL);
  }

  for (ap = qfp->alist; ap->name != NULL; ap++) {
    str = ap->name;
    if (*str == '~') {
      *str = '-';
    }
  }

  c = HiddenGroup (h, 4, 0, NULL);
  b = DefaultButton (c, "Accept", DoProcessQualifier);
  SetObjectExtra (b, qfp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);
  qfp->leaveDlgUp = CheckBox (c, "Leave Dialog Up", NULL);

  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) k, (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Select (qfp->findthis);
  Update ();
}

extern void RemoveQualifier (IteM i);
extern void ConvertFeatures (IteM i);
extern void ConvertQualifier (IteM i);
extern void EditQualifier (IteM i);
extern void AddQualifier (IteM i);

extern void RemoveQualifier (IteM i)

{
  ProcessQualifier (i, REMOVE_QUAL);
}

extern void ConvertFeatures (IteM i)

{
  ProcessQualifier (i, CONVERT_FEAT);
}

extern void ConvertQualifier (IteM i)

{
  ProcessQualifier (i, CONVERT_QUAL);
}

extern void EditQualifier (IteM i)

{
  ProcessQualifier (i, EDIT_QUAL);
}

extern void AddQualifier (IteM i)

{
  ProcessQualifier (i, ADD_QUAL);
}

#define REMOVE_SOURCE    1
#define CONVERT_SOURCE   2
#define EDIT_SOURCE      3
#define ADD_SOURCE       4

/* Note that any changes to this alist should also be made to */
/* the (subsource_and_orgmod_subtype_remove_alistX list which */
/* follows.                                                   */

static ENUM_ALIST(subsource_and_orgmod_subtype_alistX)
  {" ",                       0},
  {"Acronym",                19},
  {"Anamorph",               29},
  {"Authority",              24},
  {"Biotype",                14},
  {"Biovar",                 13},
  {"Breed",                  31},
  {"Cell-line",             108},
  {"Cell-type",             109},
  {"Chemovar",               12},
  {"Chromosome",            101},
  {"Clone",                 103},
  {"Clone-lib",             111},
  {"Common",                 18},
  {"Country",               123},
  {"Cultivar",               10},
  {"Dev-stage",             112},
  {"Dosage",                 20},
  {"Ecotype",                27},
  {"Endogenous_virus-name", 125},
  {"Environmental-sample",  127},
  {"Forma",                  25},
  {"Forma-specialis",        26},
  {"Frequency",             113},
  {"Genotype",              106},
  {"Germline",              114},
  {"Group",                  15},
  {"Haplotype",             105},
  {"Ins-seq-name",          121},
  {"Isolate",                17},
  {"Isolation-source",      128},
  {"Lab-host",              116},
  {"Map",                   102},
  {"Note - OrgMod",          55},
  {"Note - SubSource",      155},
  {"Old Lineage",            53}, /* 253 */
  {"Old Name",               54}, /* 254 */
  {"Pathovar",               11},
  {"Plasmid-name",          119},
  {"Plastid-name",          122},
  {"Pop-variant",           117},
  {"Rearranged",            115},
  {"Segment",               124},
  {"Serogroup",               8},
  {"Serotype",                7},
  {"Serovar",                 9},
  {"Sex",                   107},
  {"Specific-host",          21},
  {"Specimen-voucher",       23},
  {"Strain",                  2},
  {"Sub-species",            22},
  {"Subclone",              104},
  {"Subgroup",               16},
  {"Substrain",               3},
  {"Subtype",                 5},
  {"Synonym",                28},
  {"Teleomorph",             30},
  {"Tissue-lib",            118},
  {"Tissue-type",           110},
  {"Transgenic",            126},
  {"Transposon-name",       120},
  {"Type",                    4},
  {"Variety",                 6},
END_ENUM_ALIST

static ENUM_ALIST(subsource_and_orgmod_subtype_remove_alistX)
  {" ",                       0},
  {"Acronym",                19},
  {"All Notes",             999},
  {"Anamorph",               29},
  {"Authority",              24},
  {"Biotype",                14},
  {"Biovar",                 13},
  {"Breed",                  31},
  {"Cell-line",             108},
  {"Cell-type",             109},
  {"Chemovar",               12},
  {"Chromosome",            101},
  {"Clone",                 103},
  {"Clone-lib",             111},
  {"Common",                 18},
  {"Country",               123},
  {"Cultivar",               10},
  {"Dev-stage",             112},
  {"Dosage",                 20},
  {"Ecotype",                27},
  {"Endogenous_virus-name", 125},
  {"Environmental-sample",  127},
  {"Forma",                  25},
  {"Forma-specialis",        26},
  {"Frequency",             113},
  {"Genotype",              106},
  {"Germline",              114},
  {"Group",                  15},
  {"Haplotype",             105},
  {"Ins-seq-name",          121},
  {"Isolate",                17},
  {"Isolation-source",      128},
  {"Lab-host",              116},
  {"Map",                   102},
  {"Note - OrgMod",          55},
  {"Note - SubSource",      155},
  {"Old Lineage",            53}, /* 253 */
  {"Old Name",               54}, /* 254 */
  {"Pathovar",               11},
  {"Plasmid-name",          119},
  {"Plastid-name",          122},
  {"Pop-variant",           117},
  {"Rearranged",            115},
  {"Segment",               124},
  {"Serogroup",               8},
  {"Serotype",                7},
  {"Serovar",                 9},
  {"Sex",                   107},
  {"Specific-host",          21},
  {"Specimen-voucher",       23},
  {"Strain",                  2},
  {"Sub-species",            22},
  {"Subclone",              104},
  {"Subgroup",               16},
  {"Substrain",               3},
  {"Subtype",                 5},
  {"Synonym",                28},
  {"Teleomorph",             30},
  {"Tissue-lib",            118},
  {"Tissue-type",           110},
  {"Transgenic",            126},
  {"Transposon-name",       120},
  {"Type",                    4},
  {"Variety",                 6},
END_ENUM_ALIST

static ENUM_ALIST(biosource_genome_alistX)
  {" ",                    0},
  {"Apicoplast",          16},
  {"Chloroplast",          2},
  {"Chromoplast",          3},
  {"Cyanelle",            12},
  {"Endogenous-virus",    19},
  {"Extrachromosomal",     8},
  {"Genomic",              1},
  {"Insertion Sequence",  11},
  {"Kinetoplast",          4},
  {"Leucoplast",          17},
  {"Macronuclear",         7},
  {"Mitochondrion",        5},
  {"Nucleomorph",         15},
  {"Plasmid",              9},
  {"Plastid",              6},
  {"Proplastid",          18},
  {"Proviral",            13},
  {"Transposon",          10},
  {"Virion",              14},
END_ENUM_ALIST

static ENUM_ALIST(orgref_textfield_alist)
  {" ",                    0},
  {"Scientific Name",      1},
  {"Common Name",          2},
  {"Lineage",              3},
  {"Division",             4},
END_ENUM_ALIST

typedef struct sourceformdata {
  FEATURE_FORM_BLOCK

  Int2           type;
  ButtoN         applyToParts;
  TexT           onlyThisPart;
  GrouP          sourceGroup;
  GrouP          modGrp;
  GrouP          applyChoiceGrp;
  GrouP          genGrp;
  GrouP          refGrp;
  GrouP          txtGrp;
  GrouP          originGrp;
  PopuP          frommod;
  PopuP          tomod;
  PopuP          fromgen;
  PopuP          togen;
  PopuP          fromref;
  PopuP          toref;
  PopuP          toorigin;
  PopuP          fromorigin;
  TexT           findthis;
  TexT           replacewith;

  Int2           choice;
  Int2           fromval;
  Int2           toval;
  Int2           onlythis;
  CharPtr        findStr;
  CharPtr        replaceStr;

  Boolean        replaceOldAsked;
  Boolean        doReplaceAll;
  Boolean        use_semicolon;
  ButtoN         leaveDlgUp;
} SourceFormData, PNTR SourceFormPtr;

extern void AppendOrReplaceString (
  CharPtr PNTR string_loc,
  CharPtr new_value,
  Boolean PNTR asked_question,
  Boolean PNTR do_replace,
  Boolean PNTR use_semicolon
)
{
  MsgAnswer ans;
  CharPtr   tmp_value, tmp_new_value;

  if (string_loc == NULL
    || new_value == NULL
    || asked_question == NULL
    || do_replace == NULL
    || use_semicolon == NULL)
  {
    return;
  }

  if (! *asked_question && !StringHasNoText (*string_loc))
  {
    *asked_question = TRUE;
    ArrowCursor ();
    ans = Message (MSG_YN, "Do you wish to overwrite existing content?");
    *do_replace = (Boolean) (ans == ANS_YES);
    if (! *use_semicolon)
    {
      if (! *do_replace)
      {
        ans = Message (MSG_YN, "Separate items with semicolon?");
        *use_semicolon = (Boolean) (ans == ANS_YES);
      }
    }
    WatchCursor ();
    Update ();
  }
  if (*do_replace || StringHasNoText (*string_loc))
  {
    MemFree (*string_loc);
    *string_loc = StringSave (new_value);
  }
  else
  {
    tmp_value = MemNew (StringLen (*string_loc) + StringLen ( new_value) + 3);
    if (tmp_value == NULL) return;
    StringCpy (tmp_value, *string_loc);
    TrimSpacesAroundString (tmp_value);
    if (*use_semicolon)
    {
      StringCat (tmp_value, "; ");
    }
    else
    {
      StringCat (tmp_value, " ");
    }
    tmp_new_value = StringSave (new_value);
    TrimSpacesAroundString (tmp_new_value);
    StringCat (tmp_value, tmp_new_value);
    MemFree (tmp_new_value);
    MemFree (*string_loc);
    *string_loc = tmp_value;
  }
}

static SubSourcePtr FindSubSource (BioSourcePtr biop, Uint1 subtype, SourceFormPtr sfp,
                                   Boolean forceRemove, Boolean convertNote, Boolean is_feat)

{
  SubSourcePtr  PNTR  prev;
  SubSourcePtr  ssp;
  SubSourcePtr  tmp;

  if (subtype == 55 || subtype == 155) {
    subtype = 255;
  }
  prev = &(biop->subtype);
  for (ssp = biop->subtype; ssp != NULL; ssp = ssp->next) {
    if (ssp->subtype == subtype) {
      if (sfp->type == REMOVE_SOURCE || forceRemove) {
        if (StringHasNoText (sfp->findStr) ||
            StringISearch (ssp->name, sfp->findStr) != NULL) {
          *prev = ssp->next;
          SubSourceFree (ssp);
          return NULL;
        }
      } else {
        return ssp;
      }
    }
    prev = &(ssp->next);
  }
  if (sfp->type == REMOVE_SOURCE || forceRemove) return NULL;
  if (sfp->type != ADD_SOURCE && (! convertNote)) return NULL;
  if (subtype == SUBSRC_transgenic && is_feat) return NULL;
  ssp = SubSourceNew ();
  if (biop->subtype == NULL) {
    biop->subtype = ssp;
  } else {
    tmp = biop->subtype;
    while (tmp->next != NULL) {
      tmp = tmp->next;
    }
    tmp->next = ssp;
  }
  if (ssp != NULL) {
    ssp->subtype = subtype;
  }
  return ssp;
}

static OrgModPtr FindOrgMod (BioSourcePtr biop, Uint1 subtype, SourceFormPtr sfp, Boolean forceRemove, Boolean convertNote)

{
  OrgModPtr   mod;
  OrgNamePtr  onp;
  OrgRefPtr   orp;
  OrgModPtr   PNTR  prev;
  OrgModPtr   tmp;

  if (subtype == 55 || subtype == 155) {
    subtype = 255;
  } else if (subtype == 53) {
    subtype = 253;
  } else if (subtype == 54) {
    subtype = 254;
  }
  mod = NULL;
  orp = biop->org;
  if (orp == NULL) {
    if (sfp->type != ADD_SOURCE) return NULL;
    orp = OrgRefNew ();
    biop->org = orp;
  }
  if (orp == NULL) return NULL;
  onp = orp->orgname;
  if (onp == NULL) {
    if ((sfp->type != ADD_SOURCE) &&
	(sfp->type != CONVERT_SOURCE))
      return NULL;
    onp = OrgNameNew ();
    orp->orgname = onp;
  }
  if (onp == NULL) return NULL;
  prev = &(onp->mod);
  for (mod = onp->mod; mod != NULL; mod = mod->next) {
    if (mod->subtype == subtype) {
      if (sfp->type == REMOVE_SOURCE || forceRemove) {
        if (StringHasNoText (sfp->findStr) ||
            StringISearch (mod->subname, sfp->findStr) != NULL) {
          *prev = mod->next;
          OrgModFree (mod);
          return NULL;
        }
      } else {
        return mod;
      }
    }
    prev = &(mod->next);
  }
  if (sfp->type == REMOVE_SOURCE || forceRemove) return NULL;
  if (sfp->type != ADD_SOURCE && (! convertNote)) return NULL;
  mod = OrgModNew ();
  if (onp->mod == NULL) {
    onp->mod = mod;
  } else {
    tmp = onp->mod;
    while (tmp->next != NULL) {
      tmp = tmp->next;
    }
    tmp->next = mod;
  }
  if (mod != NULL) {
    mod->subtype = subtype;
  }
  return mod;
}

static void ConvertSourceString (OrgRefPtr orp, Int2 fromval, Int2 toval)
{
  CharPtr     tmp;
  OrgNamePtr  onp;

  if (orp == NULL || fromval == toval) return;

  tmp = NULL;
  onp = orp->orgname;

  switch (fromval) {
    case 1 :
      tmp = orp->taxname;
      SetTaxNameAndRemoveTaxRef (orp, NULL);
      break;
    case 2 :
      tmp = orp->common; 
      orp->common = NULL;
      break;
    case 3 :
      if (onp != NULL) {
         tmp = onp->lineage ;
         onp->lineage = NULL;
      }
      break;
    case 4 :
      if (onp != NULL) {
         tmp = onp->div ;
         onp->div = NULL;
      }
      break;
    default :
      break;
  }
 
  if (tmp == NULL) return;

  switch (toval) {
    case 1 :
      SetTaxNameAndRemoveTaxRef (orp, tmp);
      break;
    case 2 :
      orp->common = tmp;
      break;
    case 3 :
      if (onp == NULL) {
         onp = OrgNameNew ();
         orp->orgname = onp;
      }
      onp->lineage = tmp;
      break;
    case 4 :
      onp = orp->orgname;
      if (onp == NULL) {
         onp = OrgNameNew ();
         orp->orgname = onp;
      }
      onp->div = tmp;
      break;
    default :
      break;
  }

  return;

} /* ConvertSourceString */

static void EditSourceString (CharPtr PNTR strptr, SourceFormPtr sfp, CharPtr foundit)

{
  size_t   diff;
  size_t   foundlen;
  size_t   replen;
  CharPtr  newstring;
  CharPtr  tmp;
  CharPtr  tmp2;

  if (sfp == NULL || strptr == NULL || foundit == NULL) return;
  foundlen = StringLen (sfp->findStr);
  replen = StringLen (sfp->replaceStr);
  if (replen > foundlen) {
    diff = replen - foundlen;
  } else {
    diff = foundlen - replen;
  }
  newstring = MemNew (StringLen (*strptr) + diff + 1);
  tmp = *strptr;
  tmp2 = newstring;
  while (tmp != foundit) {
    *tmp2 = *tmp;
    tmp++;
    tmp2++;
  }
  if (sfp->replaceStr != NULL) {
    tmp2 = MemCopy (tmp2, sfp->replaceStr, replen);
  }
  tmp2 += replen;
  tmp += foundlen;
  tmp2 = StringMove (tmp2, tmp);
  *strptr = MemFree (*strptr);
  *strptr = newstring;
}

static Uint1 AssignSubtype (Uint1 subtype)

{
  if (subtype >= 100) {
    subtype -= 100;
  }
  if (subtype == 55) {
    subtype = 255;
  }
  return subtype;
}

static void RemoveSubSourceByPtr (BioSourcePtr biop, SubSourcePtr to_remove)
{
  SubSourcePtr ssp, prev;

  if (biop == NULL || to_remove == NULL) return;
  
  prev = NULL;
  ssp = biop->subtype;
  while (ssp != NULL)
  {
    if (ssp == to_remove)
    {
      if (prev == NULL)
      {
        biop->subtype = ssp->next;
        ssp->next = NULL;
        SubSourceFree (ssp);
        ssp = biop->subtype;
      }
      else
      {
        prev->next = ssp->next;
        ssp->next = NULL;
        SubSourceFree (ssp);
        ssp = prev->next;
      }
    }
    else
    {
      prev = ssp;
      ssp = ssp->next;
    }
  }
}

static void ConvertSubSourceModifier (
  BioSourcePtr  biop,
  SourceFormPtr sfp,
  Boolean       is_feat,
  SubSourcePtr  ssp_from
)
{
  SubSourcePtr ssp_to;

  if (biop == NULL || sfp == NULL || ssp_from == NULL) return;
  ssp_to = FindSubSource (biop, sfp->toval - 100, sfp, TRUE, FALSE, is_feat);
  if (ssp_to != NULL)
  {
    AppendOrReplaceString (&(ssp_to->name), ssp_from->name,
                           &(sfp->replaceOldAsked),
                           &(sfp->doReplaceAll),
                           &(sfp->use_semicolon));
    RemoveSubSourceByPtr (biop, ssp_from);
  }
  else
  {
    ssp_from->subtype = AssignSubtype (sfp->toval);
  }
}
 
static void RemoveOrgModByPtr (BioSourcePtr biop, OrgModPtr to_remove)
{
  OrgModPtr mod, prev;
  
  if (biop == NULL
    || biop->org == NULL
    || biop->org->orgname == NULL
    || biop->org->orgname->mod == NULL
    || to_remove == NULL)
  {
    return;
  }

  prev = NULL;
  mod = biop->org->orgname->mod;
  while (mod != NULL)
  {
    if (mod == to_remove)
    {
      if (prev == NULL)
      {
        biop->org->orgname->mod = mod->next;
        mod->next = NULL;
        OrgModFree (mod);
        mod = biop->org->orgname->mod;
      }
      else
      {
        prev->next = mod->next;
        mod->next = NULL;
        OrgModFree (mod);
        mod = prev->next;
      }
    }
    else
    {
      prev = mod;
      mod = mod->next;
    }
  }
}

static void ConvertOrganismModifier (
  BioSourcePtr  biop,
  SourceFormPtr sfp,
  Boolean       is_feat,
  OrgModPtr     mod_from
)
{
  OrgModPtr mod_to;

  if (biop == NULL || sfp == NULL || mod_from == NULL) return;
  mod_to = FindOrgMod (biop, sfp->toval, sfp, FALSE, FALSE);
  if (mod_to != NULL)
  {
    AppendOrReplaceString (&(mod_to->subname), mod_from->subname,
                           &(sfp->replaceOldAsked),
                           &(sfp->doReplaceAll),
                           &(sfp->use_semicolon));
    RemoveOrgModByPtr (biop, mod_from);
  }
  else
  {
    mod_from->subtype = AssignSubtype (sfp->toval);
  }
}

static void ConvertSubSourceToOrgMod (
  BioSourcePtr biop,
  SourceFormPtr sfp,
  Boolean is_feat,
  SubSourcePtr ssp
)
{
  OrgModPtr mod;

  if (biop == NULL
    || sfp == NULL
    || ssp == NULL)
  {
    return;
  }

  if (sfp->toval == 55)
  {
    mod = FindOrgMod (biop, 255, sfp, FALSE, FALSE);
  }
  else
  {
    mod = FindOrgMod (biop, sfp->toval, sfp, FALSE, FALSE);
  }
  if (mod == NULL)
  {
    if (biop->org == NULL) 
    {
      biop->org = OrgRefNew ();
      if (biop->org == NULL) return;
    } 
    if (biop->org->orgname == NULL)
    {
      biop->org->orgname = OrgNameNew ();
      if (biop->org->orgname == NULL) return;
    }
    mod = OrgModNew ();
    if (sfp->toval == 55)
    {
      mod->subtype = 255;
    }
    else
    {
      mod->subtype = sfp->toval;
    }
    mod->subname = StringSave (ssp->name);
    mod->next = biop->org->orgname->mod;
    biop->org->orgname->mod = mod;
  }
  else
  {
    AppendOrReplaceString (&(mod->subname), ssp->name,
                           &(sfp->replaceOldAsked),
                           &(sfp->doReplaceAll),
                           &(sfp->use_semicolon));
  }
  RemoveSubSourceByPtr (biop, ssp);
}
 
static void ConvertOrgModToSubSource (
  BioSourcePtr biop,
  SourceFormPtr sfp,
  Boolean is_feat,
  OrgModPtr mod
)
{
  SubSourcePtr ssp;

  if (biop == NULL
    || sfp == NULL
    || mod == NULL)
  {
    return;
  }

  if (sfp->toval == 155)
  {
    ssp = FindSubSource (biop, 255, sfp, FALSE, FALSE, is_feat);
  }
  else
  {
    ssp = FindSubSource (biop, sfp->toval - 100, sfp, FALSE, FALSE, is_feat);
  }
  if (ssp == NULL)
  {
    ssp = SubSourceNew ();
    if (sfp->toval == 155)
    {
      ssp->subtype = 255;
    }
    else
    {
      ssp->subtype = sfp->toval - 100;
    }
    ssp->name = StringSave (mod->subname);
    ssp->next = biop->subtype;
    biop->subtype = ssp;
  }
  else
  {
    AppendOrReplaceString (&(ssp->name), mod->subname,
                           &(sfp->replaceOldAsked),
                           &(sfp->doReplaceAll),
                           &(sfp->use_semicolon));
  }
  RemoveOrgModByPtr (biop, mod);
}

static Boolean DoesBioSourceQualContainString (BioSourcePtr biop, CharPtr str)
{
  SubSourcePtr  ssp = NULL;
  OrgRefPtr     orp = NULL;
  OrgNamePtr    onp = NULL;
  OrgModPtr     mod = NULL;
  
  if (biop == NULL) 
  {
	return FALSE;
  } 
  if (str == NULL || StringHasNoText (str))
  {
	return TRUE;
  }
  for (ssp = biop->subtype; ssp != NULL; ssp = ssp->next) {
    if (ssp->name != NULL && StringStr (ssp->name, str) != NULL) 
    {
      return TRUE;
    }
  }
  orp = biop->org;
  if (orp == NULL) {
    return FALSE;
  }
  onp = orp->orgname;
  if (onp == NULL) {
    return FALSE;
  }
  for (mod = onp->mod; mod != NULL; mod = mod->next) {
	if (mod->subname != NULL && StringStr (mod->subname, str) != NULL) 
	{
	  return TRUE;
	}
  }
  return FALSE;
}

static Boolean DoesBioSourceHaveQual (BioSourcePtr biop, Uint1 subtype, Boolean is_ssp)
{
  SubSourcePtr  ssp = NULL;
  OrgRefPtr     orp = NULL;
  OrgNamePtr    onp = NULL;
  OrgModPtr     mod = NULL;
  
  if (biop == NULL) 
  {
  	return FALSE;
  }
  if (is_ssp) 
  {
    if (subtype == 55 || subtype == 155) {
      subtype = 255;
    }
    for (ssp = biop->subtype; ssp != NULL; ssp = ssp->next) {
      if (ssp->subtype == subtype) {
        return TRUE;
      }
    }
  }
  else 
  {
    if (subtype == 55 || subtype == 155) {
      subtype = 255;
    } else if (subtype == 53) {
      subtype = 253;
    } else if (subtype == 54) {
      subtype = 254;
    }

    orp = biop->org;
    if (orp == NULL) {
      return FALSE;
    }
    onp = orp->orgname;
    if (onp == NULL) {
      return FALSE;
    }
    for (mod = onp->mod; mod != NULL; mod = mod->next) {
      if (mod->subtype == subtype) {
	    return TRUE;
 	  }
    }
  }
  return FALSE;
}

static Boolean OkToApplyQual (BioSourcePtr biop, SourceFormPtr sfp)
{
  Int2           val;
  Uint1          subtype;
  Boolean        is_ssp;
  
  if (biop == NULL || sfp == NULL) {
    return FALSE;
  }
  if (sfp->choice != 1 || sfp->type != ADD_SOURCE) 
  {
  	return TRUE;
  }
  val = GetValue (sfp->applyChoiceGrp);
  switch (val) 
  {
  	case 1:
  	  return TRUE;
  	  break;
  	case 2:
  	  subtype = sfp->toval;
  	  is_ssp = FALSE;
  	  if (subtype == 999) 
  	  {
  	  	subtype = 255;
  	  	is_ssp = TRUE;
  	  	if (DoesBioSourceHaveQual (biop, subtype, TRUE))
  	  	{
  	      return TRUE;
  	  	}
  	  	else 
  	  	{
  	  	  return DoesBioSourceHaveQual (biop, subtype, FALSE);
  	  	}
  	  }
  	  else if (subtype >= 100)
  	  {
  	  	subtype = subtype - 100;
  	  	is_ssp = TRUE;
  	  }
  	  return DoesBioSourceHaveQual (biop, subtype, is_ssp);
  	  break;
  	case 4:
      return DoesBioSourceQualContainString (biop, sfp->replaceStr);
      break; 	  
  }
  return FALSE;	
}
 
static void ProcessBioSourceFunc (BioSourcePtr biop, SourceFormPtr sfp, Boolean is_feat)

{
  CharPtr       foundit;
  OrgModPtr     mod = NULL;
  OrgNamePtr    onp;
  OrgRefPtr     orp;
  SubSourcePtr  ssp = NULL;
  CharPtr       str = NULL;
  CharPtr       str1 = NULL;
  CharPtr       str2 = NULL;
  CharPtr       tmp_str;
  Int4          offset;

  if (biop == NULL || sfp == NULL) return;
  if (! OkToApplyQual (biop, sfp)) return;
  if (sfp->choice == 1) {
    if (sfp->fromval == 999) {
      ssp = FindSubSource (biop, 255, sfp, FALSE, FALSE, is_feat);
      if (ssp != NULL)
	str1 = ssp->name;

      mod = FindOrgMod (biop, 255, sfp, FALSE, FALSE);
      if (mod != NULL)
	str2 = mod->subname;
    }
    else if (sfp->fromval >= 100) {
      ssp = FindSubSource (biop, sfp->fromval - 100, sfp,
			   FALSE, FALSE, is_feat);
      if (ssp == NULL) return;
      str = ssp->name;
    } else {
      mod = FindOrgMod (biop, sfp->fromval, sfp, FALSE, FALSE);
      if (mod == NULL) return;
      str = mod->subname;
    }
    switch (sfp->type) {
      case REMOVE_SOURCE :
        break;
      case CONVERT_SOURCE :
        if (sfp->toval < 1) return;
        if (StringHasNoText (sfp->findStr) || StringISearch (str, sfp->findStr) != NULL) {
          if (sfp->toval >= 100 && ssp != NULL) {
            ConvertSubSourceModifier (biop, sfp, is_feat, ssp);
          } else if (sfp->toval < 100 && mod != NULL) {
            ConvertOrganismModifier (biop, sfp, is_feat, mod);
          } else if (sfp->toval < 100 && ssp != NULL) {
            ConvertSubSourceToOrgMod (biop, sfp, is_feat, ssp);
          } else if (sfp->toval >= 100 && mod != NULL) {
            ConvertOrgModToSubSource (biop, sfp, is_feat, mod);
          }
        }
        break;
      case EDIT_SOURCE :
        if (ssp != NULL) {
          foundit = StringISearch (ssp->name, sfp->findStr);
		  while (foundit != NULL) {
		    offset = foundit - ssp->name;
            EditSourceString (&(ssp->name), sfp, foundit);
			foundit = StringISearch (ssp->name + offset, sfp->findStr);
          }
        } else if (mod != NULL) {
          foundit = StringISearch (mod->subname, sfp->findStr);
 		  while (foundit != NULL) {
		    offset = foundit - mod->subname;
            EditSourceString (&(mod->subname), sfp, foundit);
			foundit = StringISearch (mod->subname + offset, sfp->findStr);
          }
       }
        break;
      case ADD_SOURCE :
        if (ssp != NULL) {
          AppendOrReplaceString (&(ssp->name), sfp->findStr,
                           &(sfp->replaceOldAsked),
                           &(sfp->doReplaceAll),
                           &(sfp->use_semicolon));
          if (ssp->name == NULL &&
              (ssp->subtype == SUBSRC_germline ||
               ssp->subtype == SUBSRC_rearranged ||
               ssp->subtype == SUBSRC_environmental_sample)) {
              ssp->name = StringSave ("");
          } else if (ssp->subtype == SUBSRC_transgenic && (! is_feat)) {
            ssp->name = StringSave ("");
          }
        } else if (mod != NULL) {
          AppendOrReplaceString (&(mod->subname), sfp->findStr,
                           &(sfp->replaceOldAsked),
                           &(sfp->doReplaceAll),
                           &(sfp->use_semicolon));
        }
        break;
      default :
        break;
    }
  } else if (sfp->choice == 2) {
    switch (sfp->type) {
      case REMOVE_SOURCE :
        if (sfp->fromval == 0 || biop->genome == sfp->fromval) {
          biop->genome = 0;
        }
        break;
      case CONVERT_SOURCE :
        if (biop->genome == sfp->fromval) {
          biop->genome = sfp->toval;
        }
        break;
      case EDIT_SOURCE :
      case ADD_SOURCE :
        biop->genome = sfp->fromval;
        break;
      default :
        break;
    }
  } else if (sfp->choice == 3) {
    /* do origin */
    switch (sfp->type) {
      case REMOVE_SOURCE :
        biop->origin = 0;
        break;
      case CONVERT_SOURCE :
        if (biop->origin == sfp->fromval)
        {
          biop->origin = sfp->toval;
        }
        break;
      case EDIT_SOURCE :
      case ADD_SOURCE :
        biop->origin = sfp->fromval;
        break;
      default :
        break;
    }
  } else if (sfp->choice == 4) {
    onp = NULL;
    orp = biop->org;
    if (orp != NULL) {
      onp = orp->orgname;
    }
    switch (sfp->type) {
      case REMOVE_SOURCE :
        switch (sfp->fromval) {
          case 1 :
            if (orp != NULL) {
              SetTaxNameAndRemoveTaxRef (orp, MemFree (orp->taxname));
            }
            break;
          case 2 :
            if (orp != NULL) {
              orp->common = MemFree (orp->common);
            }
            break;
          case 3 :
            if (onp != NULL) {
              onp->lineage = MemFree (onp->lineage);
            }
            break;
          case 4 :
            if (onp != NULL) {
              onp->div = MemFree (onp->div);
            }
            break;
          default :
            break;
        }
        break;
      case CONVERT_SOURCE :
        ConvertSourceString (orp, sfp->fromval, sfp->toval);
        break;
      case EDIT_SOURCE :
        switch (sfp->fromval) {
          case 1 :
            if (orp != NULL) {
              foundit = StringISearch (orp->taxname, sfp->findStr);
              if (foundit != NULL) {
                tmp_str = StringSave (orp->taxname);
                foundit = StringISearch (tmp_str, sfp->findStr);
                EditSourceString (&(tmp_str), sfp, foundit);
                SetTaxNameAndRemoveTaxRef (orp, tmp_str);
              }
            }
            break;
          case 2 :
            if (orp != NULL) {
              foundit = StringISearch (orp->common, sfp->findStr);
              if (foundit != NULL) {
                EditSourceString (&(orp->common), sfp, foundit);
              }
            }
            break;
          case 3 :
            if (onp != NULL) {
              foundit = StringISearch (onp->lineage, sfp->findStr);
              if (foundit != NULL) {
                EditSourceString (&(onp->lineage), sfp, foundit);
              }
            }
            break;
          case 4 :
            if (onp != NULL) {
              foundit = StringISearch (onp->div, sfp->findStr);
              if (foundit != NULL) {
                EditSourceString (&(onp->div), sfp, foundit);
              }
            }
            break;
          default :
            break;
        }
        break;
      case ADD_SOURCE :
        break;
      default :
        break;
    }
  }
}

static Boolean ProcessSourceGatherFunc (GatherContextPtr gcp)

{
  BioSourcePtr   biop;
  SeqFeatPtr     feat;
  Boolean        is_feat = FALSE;
  SourceFormPtr  sfp;
  ValNodePtr     vnp;

  if (gcp == NULL) return TRUE;
  sfp = (SourceFormPtr) gcp->userdata;
  if (sfp == NULL) return TRUE;
  if (gcp->thisitem == NULL) return TRUE;
  biop = NULL;
  if (gcp->thistype == OBJ_SEQFEAT) {
    feat = (SeqFeatPtr) gcp->thisitem;
    if (feat->data.choice == SEQFEAT_BIOSRC) {
      biop = (BioSourcePtr) feat->data.value.ptrvalue;
      is_feat = TRUE;
    }
  } else if (gcp->thistype == OBJ_SEQDESC) {
    vnp = (ValNodePtr) gcp->thisitem;
    if (vnp->choice == Seq_descr_source) {
      biop = (BioSourcePtr) vnp->data.ptrvalue;
    }
  }
  if (biop == NULL) return TRUE;
  ProcessBioSourceFunc (biop, sfp, is_feat);
  return TRUE;
}

static void PutBioSourceOnPartAndProcess (SeqEntryPtr sep, Int2 onlythis, SourceFormPtr sfp)

{
  BioSourcePtr       biop;
  BioseqPtr          bsp;
  BioseqSetPtr       bssp;
  Int2               count;
  SeqMgrDescContext  dcontext;
  SeqEntryPtr        nsep;
  SeqDescrPtr        sdp;
  SeqEntryPtr        tmp;

  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL) {
      if (onlythis != 0 && bssp->_class == BioseqseqSet_class_parts) {
        for (tmp = bssp->seq_set, count = 1;
             tmp != NULL && count != onlythis;
             tmp = tmp->next, count++) continue;
        if (tmp != NULL) {
          PutBioSourceOnPartAndProcess (tmp, onlythis, sfp);
        }
      } else {
        for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
          PutBioSourceOnPartAndProcess (tmp, onlythis, sfp);
        }
      }
      return;
    }
  }
  nsep = FindNucSeqEntry (sep);
  if (nsep == NULL) return;
  bsp = (BioseqPtr) nsep->data.ptrvalue;
  if (bsp == NULL) return;
  if (bsp->repr == Seq_repr_raw) {
    sdp = SeqEntryGetSeqDescr (nsep, Seq_descr_source, NULL);
    if (sdp != NULL) {
      biop = (BioSourcePtr) sdp->data.ptrvalue;
      ProcessBioSourceFunc (biop, sfp, FALSE);
      return;
    }
    sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_source, &dcontext);
    if (sdp == NULL) return;
    biop = (BioSourcePtr) sdp->data.ptrvalue;
    if (biop != NULL) {
      biop = (BioSourcePtr) AsnIoMemCopy ((Pointer) biop,
                                          (AsnReadFunc) BioSourceAsnRead,
                                          (AsnWriteFunc) BioSourceAsnWrite);
      if (biop != NULL) {
        sdp = CreateNewDescriptor (nsep, Seq_descr_source);
        if (sdp != NULL) {
          sdp->data.ptrvalue = (Pointer) biop;
          ProcessBioSourceFunc (biop, sfp, FALSE);
        }
      }
    }
  }
}

static ENUM_ALIST(origin_alist)
{" ",               ORG_DEFAULT    },
{"Natural",         ORG_NATURAL    },
{"Natural Mutant",  ORG_NATMUT     },
{"Mutant",          ORG_MUT        },
{"Artificial",      ORG_ARTIFICIAL },
{"Synthetic",       ORG_SYNTHETIC  },
{"Other",           ORG_OTHER      },
END_ENUM_ALIST

static void DoProcessSource (ButtoN b)

{
  GatherScope    gs;
  SeqEntryPtr    sep;
  SourceFormPtr  sfp;
  Char           str [32];
  UIEnum         val;

  sfp = (SourceFormPtr) GetObjectExtra (b);
  if (sfp == NULL) return;
  Hide (sfp->form);
  WatchCursor ();
  Update ();
  sfp->fromval = 0;
  sfp->toval = 0;
  sfp->choice = GetValue (sfp->sourceGroup);
  if (sfp->choice == 1) {
    if (REMOVE_SOURCE == sfp->type) {
      if (GetEnumPopup (sfp->frommod, subsource_and_orgmod_subtype_remove_alistX,
			&val))
	sfp->fromval = (Int2) val;
      if (GetEnumPopup (sfp->tomod, subsource_and_orgmod_subtype_remove_alistX,
			&val))
	sfp->toval = (Int2) val;
    }
    else {
      if (GetEnumPopup (sfp->frommod, subsource_and_orgmod_subtype_alistX,
			&val))
	sfp->fromval = (Int2) val;
      if (GetEnumPopup (sfp->tomod, subsource_and_orgmod_subtype_alistX, &val))
	sfp->toval = (Int2) val;
    }
  } else if (sfp->choice == 2) {
    if (GetEnumPopup (sfp->fromgen, biosource_genome_alistX, &val)) {
      sfp->fromval = (Int2) val;
    }
    if (GetEnumPopup (sfp->togen, biosource_genome_alistX, &val)) {
      sfp->toval = (Int2) val;
    }
  } else if (sfp->choice == 3) {
    if (GetEnumPopup (sfp->fromorigin, origin_alist, &val)) {
      sfp->fromval = (Int2) val;
    }
    if (GetEnumPopup (sfp->toorigin, origin_alist, &val)) {
      sfp->toval = (Int2) val;
    }
  } else if (sfp->choice == 4) {
    if (GetEnumPopup (sfp->fromref, orgref_textfield_alist, &val)) {
      sfp->fromval = (Int2) val;
    }
    if (GetEnumPopup (sfp->toref, orgref_textfield_alist, &val)) {
      sfp->toval = (Int2) val;
    }
  }
  if (sfp->fromval > 0 || (sfp->choice == 4 || sfp->type == REMOVE_SOURCE)) {
    sfp->findStr = JustSaveStringFromText (sfp->findthis);
    sfp->replaceStr = JustSaveStringFromText (sfp->replacewith);
    if (sfp->type != EDIT_SOURCE) {
      TrimSpacesAroundString (sfp->findStr);
      TrimSpacesAroundString (sfp->replaceStr);
    }
    if (sfp->type != EDIT_SOURCE
      || (! StringHasNoText (sfp->findStr))
      || sfp->choice == 2
      || sfp->choice == 3) {
      sfp->replaceOldAsked = FALSE;
      sfp->doReplaceAll = FALSE;
      sfp->onlythis = 0;
      if (sfp->applyToParts != NULL && GetStatus (sfp->applyToParts)) {
        GetTitle (sfp->onlyThisPart, str, sizeof (str));
        if (! StrToInt (str, &(sfp->onlythis))) {
          sfp->onlythis = 0;
        }
      }
      if (sfp->onlythis > 0) {
        sep = GetTopSeqEntryForEntityID (sfp->input_entityID);
        PutBioSourceOnPartAndProcess (sep, sfp->onlythis, sfp);
      } else {
        MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
        gs.seglevels = 1;
        MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
        gs.ignore[OBJ_BIOSEQ] = FALSE;
        gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
        gs.ignore[OBJ_SEQANNOT] = FALSE;
        gs.ignore[OBJ_SEQFEAT] = FALSE;
        gs.ignore[OBJ_SEQDESC] = FALSE;
        GatherEntity (sfp->input_entityID, (Pointer) sfp, ProcessSourceGatherFunc, &gs);
      }
    }
  }
  ArrowCursor ();
  Update ();
  if (indexerVersion && sfp->type == ADD_SOURCE) {
    sep = GetTopSeqEntryForEntityID (sfp->input_entityID);
    if (CountSeqEntryComponents (sep) == 1) {
      Message (MSG_OK, "When only one record present, edit the BioSource directly");
    }
  }
  ObjMgrSetDirtyFlag (sfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, sfp->input_entityID, 0, 0);
  if (GetStatus (sfp->leaveDlgUp)) {
    Show (sfp->form);
  } else {
    Remove (sfp->form);
  }
}


static void ChangeSourceGroup (GrouP g)

{
  SourceFormPtr  sfp;
  Int2           val;

  sfp = (SourceFormPtr) GetObjectExtra (g);
  if (sfp == NULL) return;
  val = GetValue (g);
  switch (val) {
    case 1 :
      SafeHide (sfp->genGrp);
      SafeHide (sfp->refGrp);
      SafeShow (sfp->modGrp);
      SafeShow (sfp->txtGrp);
      SafeHide (sfp->originGrp);
      break;
    case 2 :
      SafeHide (sfp->modGrp);
      SafeHide (sfp->refGrp);
      SafeHide (sfp->txtGrp);
      SafeShow (sfp->genGrp);
      SafeHide (sfp->originGrp);
      break;
    case 3 :
      SafeHide (sfp->modGrp);
      SafeHide (sfp->refGrp);
      SafeHide (sfp->txtGrp);
      SafeHide (sfp->genGrp);
      SafeShow (sfp->originGrp);
      break;
    case 4 :
      SafeHide (sfp->modGrp);
      SafeHide (sfp->genGrp);
      SafeShow (sfp->txtGrp);
      SafeShow (sfp->refGrp);
      SafeHide (sfp->originGrp);
      break;
    default :
      SafeHide (sfp->modGrp);
      SafeHide (sfp->genGrp);
      SafeHide (sfp->refGrp);
      SafeHide (sfp->txtGrp);
      break;
  }
  Update ();
}

static void SourceMessageProc (ForM f, Int2 mssg)

{
  SourceFormPtr  sfp;

  sfp = (SourceFormPtr) GetObjectExtra (f);
  if (sfp != NULL) {
    if (sfp->appmessage != NULL) {
      sfp->appmessage (f, mssg);
    }
  }
}

static void CleanupSourceForm (GraphiC g, VoidPtr data)

{
  SourceFormPtr  sfp;

  sfp = (SourceFormPtr) data;
  if (sfp != NULL) {
    MemFree (sfp->findStr);
    MemFree (sfp->replaceStr);
  }
  StdCleanupFormProc (g, data);
}

static PopuP PopupOrSingleList (GrouP prnt, Boolean macLike, PupActnProc actn, EnumFieldAssocPtr al, UIEnum val)

{
#ifdef WIN_MOTIF
  LisT  lst;

  lst = CreateEnumListDialog (prnt, 0, 16, (LstActnProc) actn, al, 0, NULL);
  SetEnumPopup ((PopuP) lst, al, val);
  return (PopuP) lst;
#else
  PopuP  pop;

  pop = CreateEnumPopupDialog (prnt, macLike, actn, al, 0, NULL);
  SetEnumPopup (pop, al, val);
  return pop;
#endif
}

extern Boolean HasPartsSet (SeqEntryPtr sep);

static void SourceApplyToPartsProc (ButtoN b)

{
  SourceFormPtr  sfp;

  sfp = (SourceFormPtr) GetObjectExtra (b);
  if (sfp == NULL) return;
  if (GetStatus (b)) {
    SafeEnable (sfp->onlyThisPart);
  } else {
    SafeDisable (sfp->onlyThisPart);
  }
}

static void ClearProcessSourceDlg (ButtoN b)
{
  SourceFormPtr  sfp;

  sfp = (SourceFormPtr) GetObjectExtra (b);
  if (sfp == NULL) return;

  if ( sfp->onlyThisPart != NULL) {
    Disable (sfp->onlyThisPart);
  }
  SetValue (sfp->sourceGroup, 1);
  Hide (sfp->modGrp);
  switch (sfp->type) {
    case REMOVE_SOURCE :
      SetEnumPopup (sfp->fromgen, biosource_genome_alistX, 0);
      SetEnumPopup (sfp->fromref, orgref_textfield_alist, 0);
      SetTitle (sfp->findthis, "");
      SetValue (sfp->frommod, 1);
      SetValue (sfp->fromorigin, 1);
      break;
    case CONVERT_SOURCE :
      SetEnumPopup (sfp->fromgen, biosource_genome_alistX, 0);
      SetEnumPopup (sfp->togen, biosource_genome_alistX, 0);
      SetEnumPopup (sfp->fromref, orgref_textfield_alist, 0);
      SetEnumPopup (sfp->toref, orgref_textfield_alist, 0);
      SetTitle (sfp->findthis, "");
      SetValue (sfp->frommod, 1);
      SetValue (sfp->tomod, 1);
      SetValue (sfp->fromorigin, 1);
      SetValue (sfp->toorigin, 1);
      break;
    case EDIT_SOURCE :
      SetEnumPopup (sfp->fromgen, biosource_genome_alistX, 0);
      SetEnumPopup (sfp->fromref, orgref_textfield_alist, 0);
      SetTitle (sfp->findthis, "");
      SetTitle (sfp->replacewith, "");
      SetValue (sfp->frommod, 1);
      SetValue (sfp->fromorigin, 1);
      break;
    case ADD_SOURCE :
      SetEnumPopup (sfp->fromgen, biosource_genome_alistX, 0);
      SetTitle (sfp->findthis, "");
      SetValue (sfp->frommod, 1);
      SetValue (sfp->fromorigin, 1);
      break;
    default :
      break;
  }
  Hide (sfp->genGrp);
  Hide (sfp->refGrp);
  Show (sfp->modGrp);
  SafeHide (sfp->originGrp);
  Select (sfp->findthis);
}

static void ChangeApplyTarget (GrouP g)
{
  SourceFormPtr  sfp;
  Int2           val;

  sfp = (SourceFormPtr) GetObjectExtra (g);
  if (sfp == NULL) return;
  val = GetValue (g);
  switch (val) {
  	case 1:
      Disable (sfp->tomod);
      Disable (sfp->replacewith);
      break;
  	case 2:
  	  Enable (sfp->tomod);
  	  Disable (sfp->replacewith);
  	  break;
  	case 4:
  	  Disable (sfp->tomod);
  	  Enable (sfp->replacewith);
  	  break;
  }
}

static void ProcessSource (IteM i, Int2 type)

{
  ButtoN             b;
  BaseFormPtr        bfp;
  GrouP              c;
  GrouP              h;
  GrouP              q;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
  SourceFormPtr      sfp;
  CharPtr            title;
  WindoW             w;
  GrouP              x;
  GrouP              z;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  sfp = (SourceFormPtr) MemNew (sizeof (SourceFormData));
  if (sfp == NULL) return;
  sfp->type = type;
  switch (type) {
    case REMOVE_SOURCE :
      title = "Source Removal";
      break;
    case CONVERT_SOURCE :
      title = "Source Conversion";
      break;
    case EDIT_SOURCE :
      title = "Edit Source";
      break;
    case ADD_SOURCE :
      title = "Add Source";
      break;
    default :
      title = "?";
      break;
  }
  w = FixedWindow (-50, -33, -10, -10, title, StdCloseWindowProc);
  SetObjectExtra (w, sfp, CleanupSourceForm);
  sfp->form = (ForM) w;
  sfp->formmessage = SourceMessageProc;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    sfp->appmessage = sepp->handleMessages;
  }

  sfp->input_entityID = bfp->input_entityID;
  sfp->input_itemID = bfp->input_itemID;
  sfp->input_itemtype = bfp->input_itemtype;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  z = HiddenGroup (h, 1, 0, NULL);
  if (HasPartsSet (sep)) {
    sfp->applyToParts = CheckBox (z, "Apply to segmented parts, not segmented sequence", SourceApplyToPartsProc);
    SetObjectExtra (sfp->applyToParts, sfp, NULL);
    x = HiddenGroup (z, 2, 0, NULL);
    StaticPrompt (x, "Apply only to particular numbered segment", 0, dialogTextHeight, programFont, 'l');
    sfp->onlyThisPart = DialogText (x, "", 4, NULL);
    Disable (sfp->onlyThisPart);
  } else {
    sfp->onlyThisPart = NULL;
  }

  sfp->sourceGroup = HiddenGroup (h, 4, 0, ChangeSourceGroup);
  SetObjectExtra (sfp->sourceGroup, sfp, NULL);
  RadioButton (sfp->sourceGroup, "Modifiers");
  RadioButton (sfp->sourceGroup, "Location");
  RadioButton (sfp->sourceGroup, "Origin");
  RadioButton (sfp->sourceGroup, "Strings");
  SetValue (sfp->sourceGroup, 1);

  q = HiddenGroup (h, 0, 0, NULL);

  sfp->modGrp = HiddenGroup (q, -4, 0, NULL);
  switch (type) {
    case REMOVE_SOURCE :
      StaticPrompt (sfp->modGrp, "Remove", 0, popupMenuHeight, programFont, 'l');
      sfp->frommod = PopupOrSingleList (sfp->modGrp, TRUE, NULL, subsource_and_orgmod_subtype_remove_alistX, 0);
      break;
    case CONVERT_SOURCE :
      StaticPrompt (sfp->modGrp, "From", 0, popupMenuHeight, programFont, 'l');
      sfp->frommod = PopupOrSingleList (sfp->modGrp, TRUE, NULL, subsource_and_orgmod_subtype_alistX, 0);

      StaticPrompt (sfp->modGrp, "To", 0, popupMenuHeight, programFont, 'l');
      sfp->tomod = PopupOrSingleList (sfp->modGrp, TRUE, NULL, subsource_and_orgmod_subtype_alistX, 0);
      break;
    case EDIT_SOURCE :
      StaticPrompt (sfp->modGrp, "Type", 0, popupMenuHeight, programFont, 'l');
      sfp->frommod = PopupOrSingleList (sfp->modGrp, TRUE, NULL, subsource_and_orgmod_subtype_alistX, 0);
      break;
    case ADD_SOURCE :
      StaticPrompt (sfp->modGrp, "Type", 0, popupMenuHeight, programFont, 'l');
      sfp->frommod = PopupOrSingleList (sfp->modGrp, TRUE, NULL, subsource_and_orgmod_subtype_alistX, 0);
      sfp->applyChoiceGrp = HiddenGroup (sfp->modGrp, 1, 3, ChangeApplyTarget);
      SetObjectExtra (sfp->applyChoiceGrp, sfp, NULL);
      RadioButton (sfp->applyChoiceGrp, "To every biosource");
      RadioButton (sfp->applyChoiceGrp, "When qualifier present");
      sfp->tomod = PopupOrSingleList (sfp->applyChoiceGrp, TRUE, NULL, subsource_and_orgmod_subtype_alistX, 0);      
      RadioButton (sfp->applyChoiceGrp, "When any qualifier contains");
      sfp->replacewith = DialogText (sfp->applyChoiceGrp, "", 14, NULL);
      SetValue (sfp->applyChoiceGrp, 1);
      Disable (sfp->tomod);
      Disable (sfp->replacewith);
      break;
    default :
      break;
  }
  Hide (sfp->modGrp);

  sfp->genGrp = HiddenGroup (q, 4, 0, NULL);
  switch (type) {
    case REMOVE_SOURCE :
      StaticPrompt (sfp->genGrp, "Remove", 0, popupMenuHeight, programFont, 'l');
      sfp->fromgen = PopupList (sfp->genGrp, TRUE, NULL);
      InitEnumPopup (sfp->fromgen, biosource_genome_alistX, NULL);
      SetEnumPopup (sfp->fromgen, biosource_genome_alistX, 0);
      break;
    case CONVERT_SOURCE :
      StaticPrompt (sfp->genGrp, "From", 0, popupMenuHeight, programFont, 'l');
      sfp->fromgen = PopupList (sfp->genGrp, TRUE, NULL);
      InitEnumPopup (sfp->fromgen, biosource_genome_alistX, NULL);
      SetEnumPopup (sfp->fromgen, biosource_genome_alistX, 0);

      StaticPrompt (sfp->genGrp, "To", 0, popupMenuHeight, programFont, 'l');
      sfp->togen = PopupList (sfp->genGrp, TRUE, NULL);
      InitEnumPopup (sfp->togen, biosource_genome_alistX, NULL);
      SetEnumPopup (sfp->togen, biosource_genome_alistX, 0);
      break;
    case EDIT_SOURCE :
      StaticPrompt (sfp->genGrp, "Type", 0, popupMenuHeight, programFont, 'l');
      sfp->fromgen = PopupList (sfp->genGrp, TRUE, NULL);
      InitEnumPopup (sfp->fromgen, biosource_genome_alistX, NULL);
      SetEnumPopup (sfp->fromgen, biosource_genome_alistX, 0);
      break;
    case ADD_SOURCE :
      StaticPrompt (sfp->genGrp, "Type", 0, popupMenuHeight, programFont, 'l');
      sfp->fromgen = PopupList (sfp->genGrp, TRUE, NULL);
      InitEnumPopup (sfp->fromgen, biosource_genome_alistX, NULL);
      SetEnumPopup (sfp->fromgen, biosource_genome_alistX, 0);
      break;
    default :
      break;
  }
  Hide (sfp->genGrp);

  sfp->refGrp = HiddenGroup (q, 4, 0, NULL);
  switch (type) {
    case REMOVE_SOURCE :
      StaticPrompt (sfp->refGrp, "Remove", 0, popupMenuHeight, programFont, 'l');
      sfp->fromref = PopupList (sfp->refGrp, TRUE, NULL);
      InitEnumPopup (sfp->fromref, orgref_textfield_alist, NULL);
      SetEnumPopup (sfp->fromref, orgref_textfield_alist, 0);
      break;
    case CONVERT_SOURCE :
      StaticPrompt (sfp->refGrp, "From", 0, popupMenuHeight, programFont, 'l');
      sfp->fromref = PopupList (sfp->refGrp, TRUE, NULL);
      InitEnumPopup (sfp->fromref, orgref_textfield_alist, NULL);
      SetEnumPopup (sfp->fromref, orgref_textfield_alist, 0);

      StaticPrompt (sfp->refGrp, "To", 0, popupMenuHeight, programFont, 'l');
      sfp->toref = PopupList (sfp->refGrp, TRUE, NULL);
      InitEnumPopup (sfp->toref, orgref_textfield_alist, NULL);
      SetEnumPopup (sfp->toref, orgref_textfield_alist, 0);
      break;
    case EDIT_SOURCE :
      StaticPrompt (sfp->refGrp, "Type", 0, popupMenuHeight, programFont, 'l');
      sfp->fromref = PopupList (sfp->refGrp, TRUE, NULL);
      InitEnumPopup (sfp->fromref, orgref_textfield_alist, NULL);
      SetEnumPopup (sfp->fromref, orgref_textfield_alist, 0);
      break;
    case ADD_SOURCE :
      break;
    default :
      break;
  }
  Hide (sfp->refGrp);

  Show (sfp->modGrp);

  sfp->txtGrp = NULL;
  switch (type) {
    case REMOVE_SOURCE :
    case CONVERT_SOURCE :
      sfp->txtGrp = HiddenGroup (h, 0, 2, NULL);
      StaticPrompt (sfp->txtGrp, "Optional string constraint", 0, dialogTextHeight, programFont, 'c');
      sfp->findthis = DialogText (sfp->txtGrp, "", 14, NULL);
      break;
    case ADD_SOURCE :
      sfp->txtGrp = HiddenGroup (h, 2, 0, NULL);
      StaticPrompt (sfp->txtGrp, "Text", 0, dialogTextHeight, programFont, 'c');
      sfp->findthis = DialogText (sfp->txtGrp, "", 14, NULL);
      break;
    case EDIT_SOURCE :
      sfp->txtGrp = HiddenGroup (h, 2, 0, NULL);
      StaticPrompt (sfp->txtGrp, "Find", 0, dialogTextHeight, programFont, 'l');
      sfp->findthis = DialogText (sfp->txtGrp, "", 14, NULL);
      StaticPrompt (sfp->txtGrp, "Replace", 0, dialogTextHeight, programFont, 'l');
      sfp->replacewith = DialogText (sfp->txtGrp, "", 14, NULL);
      break;
    default :
      break;
  }

  /* Build origin group */
  switch (type) {
    case REMOVE_SOURCE :
      sfp->originGrp = HiddenGroup (h, 2, 0, NULL);
      StaticPrompt (sfp->originGrp, "Remove", 0, dialogTextHeight, programFont, 'l');
      sfp->fromorigin = PopupList (sfp->originGrp, TRUE, NULL);
      InitEnumPopup (sfp->fromorigin, origin_alist, NULL);
      break;
    case ADD_SOURCE :
    case EDIT_SOURCE :
      sfp->originGrp = HiddenGroup (h, 2, 0, NULL);
      StaticPrompt (sfp->originGrp, "Origin", 0, dialogTextHeight, programFont, 'l');
      sfp->fromorigin = PopupList (sfp->originGrp, TRUE, NULL);
      InitEnumPopup (sfp->fromorigin, origin_alist, NULL);
      break;
    case CONVERT_SOURCE :
      sfp->originGrp = HiddenGroup (h, 4, 0, NULL);
      StaticPrompt (sfp->originGrp, "From", 0, dialogTextHeight, programFont, 'l');
      sfp->fromorigin = PopupList (sfp->originGrp, TRUE, NULL);
      InitEnumPopup (sfp->fromorigin, origin_alist, NULL);
      StaticPrompt (sfp->originGrp, "To", 0, dialogTextHeight, programFont, 'l');
      sfp->toorigin = PopupList (sfp->originGrp, TRUE, NULL);
      InitEnumPopup (sfp->toorigin, origin_alist, NULL);
      break;
  }
  SafeHide (sfp->originGrp);

  c = HiddenGroup (h, 4, 0, NULL);
  b = DefaultButton (c, "Accept", DoProcessSource);
  SetObjectExtra (b, sfp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);
  b = PushButton (c, "Clear", ClearProcessSourceDlg);
  SetObjectExtra (b, sfp, NULL);
  sfp->leaveDlgUp = CheckBox (c, "Leave Dialog Up", NULL);

  AlignObjects (ALIGN_CENTER, (HANDLE) c, (HANDLE) sfp->sourceGroup,
                (HANDLE) sfp->modGrp, (HANDLE) sfp->genGrp,
                (HANDLE) sfp->refGrp, (HANDLE) sfp->txtGrp,
                (HANDLE) sfp->originGrp, NULL);
  RealizeWindow (w);
  Show (w);
  Select (sfp->findthis);
  Update ();
}

extern void RemoveSource (IteM i);
extern void ConvertSource (IteM i);
extern void EditSource (IteM i);
extern void AddSource (IteM i);

extern void RemoveSource (IteM i)

{
  ProcessSource (i, REMOVE_SOURCE);
}

extern void ConvertSource (IteM i)

{
  ProcessSource (i, CONVERT_SOURCE);
}

extern void EditSource (IteM i)

{
  ProcessSource (i, EDIT_SOURCE);
}

extern void AddSource (IteM i)

{
  ProcessSource (i, ADD_SOURCE);
}

