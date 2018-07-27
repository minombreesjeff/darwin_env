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
* $Revision: 6.135 $
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
                         (bssp->_class >= 13 && bssp->_class <= 16))) {
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
  b = PushButton (c, "Accept", StdAcceptFormButtonProc);
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
  Uint1  order [NUM_ORDER];

  SeqIdBestRank (order, NUM_ORDER);
  order [SEQID_OTHER] = 52;
  return SeqIdSelect (sip, order, NUM_ORDER);
}

static void PromoteSeqId (SeqIdPtr sip, Boolean alsoCheckLocalAccn)

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

  bestid = SeqIdFindBestForPromotion (bsp->id);
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

static void PromoteSeqIdList (SeqIdPtr sip, Boolean alsoCheckLocalAccn)

{
  while (sip != NULL) {
    PromoteSeqId (sip, alsoCheckLocalAccn);
    sip = sip->next;
  }
}

static void PromoteSeqLocList (SeqLocPtr slp, Boolean alsoCheckLocalAccn)

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
        PromoteSeqIdList (sip, alsoCheckLocalAccn);
        break;
      case SEQLOC_INT :
        sinp = (SeqIntPtr) slp->data.ptrvalue;
        if (sinp != NULL) {
          sip = sinp->id;
          PromoteSeqIdList (sip, alsoCheckLocalAccn);
        }
        break;
      case SEQLOC_PNT :
        spp = (SeqPntPtr) slp->data.ptrvalue;
        if (spp != NULL) {
          sip = spp->id;
          PromoteSeqIdList (sip, alsoCheckLocalAccn);
        }
        break;
      case SEQLOC_PACKED_PNT :
        psp = (PackSeqPntPtr) slp->data.ptrvalue;
        if (psp != NULL) {
          sip = psp->id;
          PromoteSeqIdList (sip, alsoCheckLocalAccn);
        }
        break;
      case SEQLOC_PACKED_INT :
      case SEQLOC_MIX :
      case SEQLOC_EQUIV :
        loc = (SeqLocPtr) slp->data.ptrvalue;
        while (loc != NULL) {
          PromoteSeqLocList (loc, alsoCheckLocalAccn);
          loc = loc->next;
        }
        break;
      case SEQLOC_BOND :
        sbp = (SeqBondPtr) slp->data.ptrvalue;
        if (sbp != NULL) {
          spp = (SeqPntPtr) sbp->a;
          if (spp != NULL) {
            sip = spp->id;
            PromoteSeqIdList (sip, alsoCheckLocalAccn);
          }
          spp = (SeqPntPtr) sbp->b;
          if (spp != NULL) {
            sip = spp->id;
            PromoteSeqIdList (sip, alsoCheckLocalAccn);
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

static Boolean PromoteIDsProc (GatherObjectPtr gop)

{
  CodeBreakPtr  cbp;
  CdRegionPtr   crp;
  RnaRefPtr     rrp;
  SeqFeatPtr    sfp;
  tRNAPtr       trp;

  if (gop->itemtype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gop->dataptr;
  if (sfp == NULL) return TRUE;

  PromoteSeqLocList (sfp->location, FALSE);

  PromoteSeqLocList (sfp->product, FALSE);

  switch (sfp->data.choice) {
    case SEQFEAT_CDREGION :
      crp = (CdRegionPtr) sfp->data.value.ptrvalue;
      if (crp != NULL && crp->code_break != NULL) {
        for (cbp = crp->code_break; cbp != NULL; cbp = cbp->next) {
          PromoteSeqLocList (cbp->loc, FALSE);
        }
      }
      break;
    case SEQFEAT_RNA :
      rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
      if (rrp != NULL && rrp->type == 3 && rrp->ext.choice == 2) {
        trp = rrp->ext.value.ptrvalue;
        if (trp != NULL && trp->anticodon != NULL) {
          PromoteSeqLocList (trp->anticodon, FALSE);
        }
      }
      break;
    default :
      break;
  }

  return TRUE;
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

  GatherObjectsInEntity (bfp->input_entityID, 0, NULL, PromoteIDsProc, NULL, NULL);

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
        PromoteSeqIdList (ddp->id, TRUE);
      }
      break;
    case SAS_DENSEG :
      dsp = sap->segs;
      if (dsp != NULL) {
        PromoteSeqIdList (dsp->ids, TRUE);
      }
      break;
    case SAS_STD :
      for (ssp = sap->segs; ssp != NULL; ssp = ssp->next) {
        PromoteSeqIdList (ssp->ids, TRUE);
        for (slp = ssp->loc; slp != NULL; slp = slp->next) {
          PromoteSeqLocList (slp, TRUE);
        }
      }
      break;
    case SAS_PACKED :
      psp = (PackSegPtr) sap->segs;
      if (psp != NULL) {
        PromoteSeqIdList (psp->ids, TRUE);
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

static Boolean RemoveIDsProc (GatherObjectPtr gop)

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

void RemoveIDsFromBioseqs (IteM i)

{
  MsgAnswer    ans;
  BaseFormPtr  bfp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  ans = Message (MSG_OKC, "Currently deletes all GenBank/EMBL/DDBJ SeqIDs");
  if (ans == ANS_CANCEL) return;

  GatherObjectsInEntity (bfp->input_entityID, 0, NULL, RemoveIDsProc, NULL, NULL);

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
  Int2               beg = 0;
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
    pfp->accept = PushButton (c, "Accept", PowBlastFormAcceptProc);
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

static void SearchCDD (BioseqPtr bsp, Pointer userdata)

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

void SimpleCDDBlastProc (IteM i)

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

  VisitBioseqsInSep (sep, (Pointer) &bf, SearchCDD);

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
          subtype != FEATDEF_site_ref) {
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
              subtype != FEATDEF_site_ref) {
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
    b = PushButton (c, "Accept", AutoParseAcceptProc);
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
    b = PushButton (c, "Accept", PreParseAcceptProc);
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

