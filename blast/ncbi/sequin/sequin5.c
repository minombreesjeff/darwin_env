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
* $Revision: 6.387 $
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
#include <gbfeat.h>
#include <gbftdef.h>
#include <satutil.h>
#include <rpsutil.h>
#include <subutil.h>
#include <explore.h>
#include <import.h>
#include <salutil.h>

#include <asn2gnbp.h> /* added for mockup parse to flatfile */
#include <seqpanel.h> /* added to include SeqEdTranslateOneCDS */


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

typedef struct accntolocal 
{
  Boolean make_secondary;
  Boolean convert_prots;
  Boolean convert_nucs;
} AccnToLocalData, PNTR AccnToLocalPtr;

static SeqIdPtr LocalSeqIdFromAccession (SeqIdPtr sip, CharPtr str, Int4 str_size)
{
  TextSeqIdPtr  tsip;
  CharPtr       text;
  SeqIdPtr      new_sip;
  ObjectIdPtr   oip;

  if (sip == NULL || str == NULL || str_size < 1)
  {
    return NULL;
  }
  if (sip->choice != SEQID_GENBANK 
      && sip->choice != SEQID_EMBL 
      && sip->choice != SEQID_DDBJ)
  {
    return NULL;
  }
  if (sip->data.ptrvalue == NULL)
  {
    return NULL;
  }
  new_sip = ValNodeNew (NULL);
  if (new_sip == NULL)
  {
    return NULL;
  }
  tsip = (TextSeqIdPtr) sip->data.ptrvalue;
  text = NULL;
  str [0] = '\0';
  if (tsip->accession != NULL) {
    text = tsip->accession;
  } else if (tsip->name != NULL) {
    text = tsip->name;
  }
  if (text != NULL) {
    StringNCpy_0 (str, text, str_size);
    oip = ObjectIdNew ();
    if (oip != NULL) {
      oip->str = StringSave (text);
      new_sip->choice = SEQID_LOCAL;
      new_sip->data.ptrvalue = (Pointer) oip;
    }
  }
  return new_sip;
}

static void SeqIdSwap (SeqIdPtr sip1, SeqIdPtr sip2)
{
  ValNode       tmp_sip;

  if (sip1 == NULL || sip2 == NULL)
  {
    return;
  }
  
  tmp_sip.choice = sip1->choice;
  tmp_sip.data.ptrvalue = sip1->data.ptrvalue;
  sip1->choice = sip2->choice;
  sip1->data.ptrvalue = sip2->data.ptrvalue;
  sip2->choice = tmp_sip.choice;
  sip2->data.ptrvalue = tmp_sip.data.ptrvalue;
}

static void ConvertSeqIdListToLocalID (SeqIdPtr sip, SeqEntryPtr sep, AccnToLocalPtr atlp)

{
  GBBlockPtr    gbp;
  Char          str [32];
  ValNodePtr    vnp;
  BioseqPtr     bsp;
  SeqIdPtr      new_sip = NULL;
  
  if (atlp == NULL) return;

  while (sip != NULL) 
  {
    bsp = BioseqFind (sip);
    if (bsp == NULL)
    {
      new_sip = LocalSeqIdFromAccession (sip, str, sizeof (str));
      if (new_sip != NULL)
      {
        bsp = BioseqFind (new_sip);
      }
    }
    if (bsp == NULL)
    {
      /* have to skip */
    }
    else if (ISA_na (bsp->mol) && !atlp->convert_nucs)
    {
      /* don't convert the nucleotide ID */
    }
    else if (ISA_aa (bsp->mol) && ! atlp->convert_prots)
    {
      /* don't convert the protein ID */
    }
    else if ((sip->choice == SEQID_GENBANK 
          || sip->choice == SEQID_EMBL 
          || sip->choice == SEQID_DDBJ)
          && sip->data.ptrvalue != NULL)
    {
      new_sip = LocalSeqIdFromAccession (sip, str, sizeof (str));
      if (new_sip != NULL)
      {
        /* replace old sip values with new  - new_sip will now be ready for freeing */
        SeqIdSwap (sip, new_sip);
                
        /* make secondary if needed */
        if (atlp->make_secondary && sep != NULL)
        {
          vnp = SeqEntryGetSeqDescr (sep, Seq_descr_genbank, NULL);
          if (vnp == NULL) {
            vnp = CreateNewDescriptor (sep, Seq_descr_genbank);
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
    new_sip = SeqIdFree (new_sip);
    sip = sip->next;
  }
}

static void ConvertSeqLocListToLocalID (SeqLocPtr slp, AccnToLocalPtr atlp)

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
        ConvertSeqIdListToLocalID (sip, NULL, atlp);
        break;
      case SEQLOC_INT :
        sinp = (SeqIntPtr) slp->data.ptrvalue;
        if (sinp != NULL) {
          sip = sinp->id;
          ConvertSeqIdListToLocalID (sip, NULL, atlp);
        }
        break;
      case SEQLOC_PNT :
        spp = (SeqPntPtr) slp->data.ptrvalue;
        if (spp != NULL) {
          sip = spp->id;
          ConvertSeqIdListToLocalID (sip, NULL, atlp);
        }
        break;
      case SEQLOC_PACKED_PNT :
        psp = (PackSeqPntPtr) slp->data.ptrvalue;
        if (psp != NULL) {
          sip = psp->id;
          ConvertSeqIdListToLocalID (sip, NULL, atlp);
        }
        break;
      case SEQLOC_PACKED_INT :
      case SEQLOC_MIX :
      case SEQLOC_EQUIV :
        loc = (SeqLocPtr) slp->data.ptrvalue;
        while (loc != NULL) {
          ConvertSeqLocListToLocalID (loc, atlp);
          loc = loc->next;
        }
        break;
      case SEQLOC_BOND :
        sbp = (SeqBondPtr) slp->data.ptrvalue;
        if (sbp != NULL) {
          spp = (SeqPntPtr) sbp->a;
          if (spp != NULL) {
            sip = spp->id;
            ConvertSeqIdListToLocalID (sip, NULL, atlp);
          }
          spp = (SeqPntPtr) sbp->b;
          if (spp != NULL) {
            sip = spp->id;
            ConvertSeqIdListToLocalID (sip, NULL, atlp);
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
  BioseqPtr      bsp;
  AccnToLocalPtr atlp;
  SeqEntryPtr    sep;
  SeqFeatPtr     sfp;
  SeqIdPtr       sip;
  SeqLocPtr      slp;

  if (gcp == NULL || gcp->userdata == NULL) return TRUE;
  atlp = (AccnToLocalPtr) gcp->userdata;
  if (gcp->thisitem == NULL) return TRUE;
  switch (gcp->thistype) {
    case OBJ_BIOSEQ :
      sep = NULL;
      bsp = (BioseqPtr) gcp->thisitem;
      if (atlp->make_secondary)
      {
        sep = SeqMgrGetSeqEntryForData (bsp);
      }
      sip = bsp->id;
      ConvertSeqIdListToLocalID (sip, sep, atlp);
      SeqMgrReplaceInBioseqIndex (bsp);
      break;
    case OBJ_SEQFEAT :
      sfp = (SeqFeatPtr) gcp->thisitem;
      slp = sfp->location;
      ConvertSeqLocListToLocalID (slp, atlp);
      slp = sfp->product;
      ConvertSeqLocListToLocalID (slp, atlp);
      break;
    default :
      break;
  }
  return TRUE;
}

static void ConvertToLocalProc (IteM i, Boolean convert_nucs, Boolean convert_prots)

{
  MsgAnswer       ans;
  BaseFormPtr     bfp;
  GatherScope     gs;
  AccnToLocalData atld;

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
  atld.make_secondary = (Boolean) (ans == ANS_YES);
  atld.convert_nucs = convert_nucs;
  atld.convert_prots = convert_prots;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  MemSet((Pointer) (gs.ignore), (int) (TRUE), (size_t) (OBJ_MAX * sizeof (Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  GatherEntity (bfp->input_entityID, (Pointer) &atld, ChangeAccessionToLocalID, &gs);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

extern void ConvertToLocalProcOnlyNucs (IteM i)
{
  ConvertToLocalProc (i, TRUE, FALSE);
}

extern void ConvertToLocalProcOnlyProts (IteM i)
{
  ConvertToLocalProc (i, FALSE, TRUE);
}

extern void ConvertToLocalProcAll (IteM i)
{
  ConvertToLocalProc (i, TRUE, TRUE);
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

  salp = BlastBioseqNet (bl3hp, bsp, "blastp", "cdd", options,
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

  BlastNetBioseqFetchEnable (bl3hp, "cdd", FALSE, TRUE);

  bf.bl3hp = bl3hp;
  bf.options = options;

  WatchCursor ();
  Update ();

  FreeCDDRegions (sep);

  VisitBioseqsInSep (sep, (Pointer) &bf, BlastCDD);

  RemoveDuplicateCDDs (sep);

  BlastFini (bl3hp);
  options = BLASTOptionDelete (options);
  BlastNetBioseqFetchDisable (bl3hp, "cdd", FALSE);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  ArrowCursor ();
  Update ();
}
/*#endif*/


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



#define NUM_SITES 27
static CharPtr siteString [NUM_SITES] = {
  "", "active site", "binding site", "cleavage site", "inhibition site", "modified site",
  "glycosylation site", "myristoylation site", "mutagenized site", "metal binding site",
  "phosphorylation site", "acetylation site", "amidation site", "methylation site",
  "hydroxylation site", "sulfatation site", "oxidative deamination site",
  "pyrrolidone carboxylic acid site", "gamma carboxyglutamic acid site",
  "blocked site", "lipid binding site", "np binding site", "DNA binding site",
  "signal peptide", "transit peptide", "transmembrane region", "nitrosylation"
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
/* MergeCDS ()                                                             */
/*                                                                         */
/*=========================================================================*/

static void MergeCDSCallback (BioseqPtr bsp, Pointer userdata)
{
  SeqFeatPtr        cds, first_cds;
  SeqMgrFeatContext fcontext;
  Int4              left = -1, right = -1;
  SeqLocPtr         cover_loc;
  Boolean           partial_left = FALSE, partial_right = FALSE;
  Uint2             strand;
  
  if (bsp == NULL)
  {
    return;
  }
  
  first_cds = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_CDREGION, 0, &fcontext);
  cds = first_cds;
  while (NULL != cds)
  {
    if (left == -1 || left > fcontext.left)
    {
      left = fcontext.left;
      partial_left = fcontext.partialL;
    }
    if (right < fcontext.right)
    {
      right = fcontext.right;
      partial_right = fcontext.partialR;
    }
    cds = SeqMgrGetNextFeature (bsp, cds, SEQFEAT_CDREGION, 0, &fcontext);
  }
  if (left < 0 || right < 0)
  {
    return;
  }
  strand = SeqLocStrand (first_cds->location);
  cover_loc = SeqLocIntNew (left, right, strand, bsp->id);
  if (strand == Seq_strand_minus)
  {
		SetSeqLocPartial (cover_loc, partial_right, partial_left);
  }
  else
  {
		SetSeqLocPartial (cover_loc, partial_left, partial_right);
  }

  cds = CreateNewFeatureOnBioseq (bsp, SEQFEAT_CDREGION, cover_loc);
  cds->partial = (partial_left | partial_right);
  cds->data.value.ptrvalue = AsnIoMemCopy (first_cds->data.value.ptrvalue,
                                              (AsnReadFunc) CdRegionAsnRead,
                                              (AsnWriteFunc) CdRegionAsnWrite);
  RetranslateOneCDS (cds, first_cds->idx.entityID, TRUE, FALSE);
}

extern void MergeCDS (IteM i)
{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;
  LogInfoPtr   lip;

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

  lip = OpenLog ("Merge CDS Mat Peptides");
  VisitBioseqsInSep (sep, NULL, MergeCDSCallback);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  VisitBioseqsInSep (sep, lip, ConvertInnerCDSsToMatPeptidesCallback);

  CloseLog (lip);
  FreeLog (lip);
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
				     Uint2       entityID)
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

  cds = FindBestCds (entityID, sfp->location, NULL, NULL);
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

static void FeatConvertPeptideToImp (SeqFeatPtr  sfp, Uint2 entityID)
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

  switch (sfp->idx.subtype) {
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

  cds = FindBestCds (entityID, NULL, sfp->location, NULL);
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

  switch (sfp->idx.subtype) {
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
	sprintf (idStr, "%s:%s", (CharPtr)dbt->tag, oip->str);
	gbqual->qual = StringSave ("db_xref");
	gbqual->val = StringSave (idStr);
      } else {
	sprintf (idStr, "%s:%ld", (CharPtr)dbt->tag, (long) oip->id);
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
/* 9/28/2004: Changed to convert all BioSource features with notes     */
/* instead of ones with transposon or insertion_seq qualifiers.        */
/*---------------------------------------------------------------------*/

static void FeatConvertBioSrcToRepeatRegion (SeqFeatPtr sfp)
{
  BioSourcePtr  biop;
  GBQualPtr     gbqual;
  ImpFeatPtr    ifp;
  OrgModPtr     omp = NULL;
  SubSourcePtr  ssp = NULL;
  CharPtr       text_to_use = NULL;
  Int4          text_len = 0;

  /* Only convert transposon and insertion_seq features */

  biop = (BioSourcePtr) sfp->data.value.ptrvalue;
  for (ssp = biop->subtype; 
       ssp != NULL && ssp->subtype != SUBSRC_other; 
       ssp = ssp->next)
  {
  }
  if (biop->org != NULL && biop->org->orgname != NULL)
  {
    for (omp = biop->org->orgname->mod; 
         omp != NULL && omp->subtype != ORGMOD_other; 
         omp = omp->next)
    {
    }
  }

  
  /* Create a new Import Feature */

  ifp = ImpFeatNew ();
  if (NULL == ifp)
    return;
  ifp->key = StringSave ("repeat_region");

  /* Copy relevant info from the BioSource */
  /* feature to the Import feature.        */

  if (ssp != NULL)
  {
    text_len += StringLen (ssp->name) + 1;
  }
  if (omp != NULL)
  {
    text_len += StringLen (omp->subname) + 1;
  }
  if (text_len > 0)
  {
    text_to_use = (CharPtr) MemNew (text_len * sizeof (Char));
    if (text_to_use != NULL)
    {
      text_to_use [0] = 0;
      if (ssp != NULL)
      {
        StringCat (text_to_use, ssp->name);
      }
      if (omp != NULL)
      {
        if (!StringHasNoText (text_to_use))
        {
          StringCat (text_to_use, ";");
        }
        StringCat (text_to_use, omp->subname);
      }
	    gbqual = GBQualNew ();
	    if (gbqual != NULL) 
	    {
		    gbqual->qual = StringSave ("note");
		    gbqual->val = text_to_use;
	      gbqual->next = sfp->qual;
		    sfp->qual = gbqual;
	    }
    }
  }
    
  /* Delete the old BioSource feature */

  sfp->data.value.ptrvalue = BioSourceFree (biop);

  /* Attach the new Import feature in its place */

  sfp->data.choice = SEQFEAT_IMP;
  sfp->data.value.ptrvalue = ifp;
}


static void FeatConvertRegionToProt (SeqFeatPtr sfp, Int2 toFeatSubType)
{
  BioseqPtr  bsp;
  ProtRefPtr prp;
  SeqFeatPtr cds;
  SeqLocPtr  location;
  
  if (sfp == NULL || sfp->data.choice != SEQFEAT_REGION)
  {
    return; 
  }
  
  /* only convert features that are on protein sequences */
  bsp = BioseqFindFromSeqLoc (sfp->location);
  if (!ISA_aa (bsp->mol))
  {
    cds = SeqMgrGetOverlappingCDS (sfp->location, NULL);
    if (cds == NULL)
    {
      return;
    }
    else
    {
      location = dnaLoc_to_aaLoc (cds, sfp->location, TRUE, NULL, FALSE);
      sfp->location = SeqLocFree (sfp->location);
      sfp->location = location;
    }
  }
  
  prp = ProtRefNew ();
  if (prp != NULL)
  {
    prp->name = ValNodeNew(NULL);
    if (prp->name != NULL)
    {
      /* use region name for protein name */
      prp->name->data.ptrvalue = sfp->data.value.ptrvalue;
      switch (toFeatSubType) 
      {
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

      sfp->data.value.ptrvalue = prp;
      sfp->data.choice = SEQFEAT_PROT;
    }
  }  
}

extern EnumFieldAssoc  enum_bond_alist [];
extern EnumFieldAssoc  enum_site_alist [];


static GbFeatName EditQualifierList[] = {
 {"allele", Class_text}, {"anticodon", Class_pos_aa},
 {"bound_moiety", Class_text},
 {"chromosome", Class_text},
 {"citation", Class_bracket_int},
 {"codon", Class_seq_aa},
 {"codon_start", Class_int_or}, 
 {"compare", Class_text },
 {"cons_splice", Class_site},
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

/* Note that any changes to this alist should also be made to */
/* the subsource_and_orgmod_subtype_remove_alistX and         */
/* subsource_and_orgmod_subtype_fasta_ alistX lists which     */
/* follow.                                                   */

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
  {"Collected-by",          131},
  {"Collection-date",       130},
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
  {"Fwd-primer-name",       135},
  {"Fwd-primer-seq",        133},
  {"Genotype",              106},
  {"Germline",              114},
  {"Group",                  15},
  {"Haplotype",             105},
  {"Identified_by",         132},
  {"Ins-seq-name",          121},
  {"Isolate",                17},
  {"Isolation-source",      128},
  {"Lab-host",              116},
  {"Lat-lon",               129},
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
  {"Rev-primer-name",       136},
  {"Rev-primer-seq",        134},
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
  {"Collected-by",          131},
  {"Collection-date",       130},
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
  {"Fwd-primer-name",       135},
  {"Fwd-primer-seq",        133},
  {"Genotype",              106},
  {"Germline",              114},
  {"Group",                  15},
  {"Haplotype",             105},
  {"Identified_by",         132},
  {"Ins-seq-name",          121},
  {"Isolate",                17},
  {"Isolation-source",      128},
  {"Lab-host",              116},
  {"Lat-lon",               129},
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
  {"Rev-primer-name",       136},
  {"Rev-primer-seq",        134},
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

static ENUM_ALIST(subsource_and_orgmod_subtype_fasta_alistX)
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
  {"Collected-by",          131},
  {"Collection-date",       130},
  {"Common",                 18},
  {"Country",               123},
  {"Cultivar",               10},
  {"Dev-stage",             112},
  {"Dosage",                 20},
  {"Ecotype",                27},
  {"Endogenous_virus-name", 125},
  {"Forma",                  25},
  {"Forma-specialis",        26},
  {"Frequency",             113},
  {"Fwd-primer-name",       135},
  {"Fwd-primer-seq",        133},
  {"Genotype",              106},
  {"Group",                  15},
  {"Haplotype",             105},
  {"Identified_by",         132},
  {"Isolate",                17},
  {"Isolation-source",      128},
  {"Lab-host",              116},
  {"Lat-lon",               129},
  {"Map",                   102},
  {"Note - OrgMod",          55},
  {"Note - SubSource",      155},
  {"Old Lineage",            53}, /* 253 */
  {"Old Name",               54}, /* 254 */
  {"Pathovar",               11},
  {"Plasmid-name",          119},
  {"Plastid-name",          122},
  {"Pop-variant",           117},
  {"Rev-primer-name",       136},
  {"Rev-primer-seq",        134},
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
  GrouP          applyChoiceGrp;  /* This is the group for the radio buttons for the
                                   * constraint on which biosources to operate on.
                                   */
  PopuP          qual_to_have;    /* This is the control that indicates which qualifier
                                   * should be present to operate on a biosource.
                                   */
  TexT           text_to_have;    /* This is the control that indicates the text that
                                   * should be present in the biosource to be operated on.
                                   */
  Int4           applyChoiceVal;  /* This is the value of the applyChoiceGrp choice. */
  CharPtr        text_to_have_str;/* This is the contents of the text_to_have box. */
  Int4           qual_to_have_val;/* This is the subtype for the qual to have. */

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

Uint2 mod_widths [] = {
  0, 25
};

Uint2 mod_types [] = {
  TAGLIST_POPUP, TAGLIST_TEXT
};

extern Uint1 FindTypeForModNameText (CharPtr cp)
{
  EnumFieldAssocPtr  ap;
  
  for (ap = subsource_and_orgmod_subtype_alistX; ap->name != NULL; ap++) 
  {
    if (StringICmp (ap->name, cp) == 0)
    {
      return ap->value;
    }
  }
  return 255;
}

static void ModListToModDialog (DialoG d, Pointer data)

{
  ValNodePtr  head;
  Int2        j;
  size_t      len;
  ListPairPtr lpp;
  CharPtr     str;
  TagListPtr  tlp;
  ValNodePtr  name_vnp;
  ValNodePtr  value_vnp;
  ValNodePtr  vnp;

  tlp = (TagListPtr) GetObjectExtra (d);
  lpp = (ListPairPtr) data;
  head = NULL;
  if (tlp != NULL) {
    name_vnp = lpp->selected_names_list;
    value_vnp = lpp->selected_values_list;
    while (name_vnp != NULL) {
      if (!StringHasNoText (name_vnp->data.ptrvalue)
        && name_vnp->choice != 114 /* germline */
        && name_vnp->choice != 126 /* transgenic */
        && name_vnp->choice != 115 /* rearranged */
        && name_vnp->choice != 127 /* environmental sample */)
      {
        vnp = ValNodeNew (head);
        if (head == NULL) {
          head = vnp;
        }
        len = StringLen (name_vnp->data.ptrvalue) + 4;
        if (value_vnp != NULL)
        {
          len += StringLen (value_vnp->data.ptrvalue);
        }
        str = MemNew (len);
        if (str != NULL)
        {
          if (value_vnp != NULL && ! StringHasNoText (value_vnp->data.ptrvalue))
          {
        	sprintf (str, "%d\t%s\n", 
        	         name_vnp->choice,
        	         (CharPtr)value_vnp->data.ptrvalue);
          }
          else
          {
          	sprintf (str, "%d\t\n", name_vnp->choice);
          }
        }
        vnp->data.ptrvalue = str;
      }
      name_vnp = name_vnp->next;
      if (value_vnp != NULL)
      {
      	value_vnp = value_vnp->next;
      }
    }
    SendMessageToDialog (tlp->dialog, VIB_MSG_RESET);
    tlp->vnp = head;
    SendMessageToDialog (tlp->dialog, VIB_MSG_REDRAW);
    for (j = 0, vnp = tlp->vnp; vnp != NULL; j++, vnp = vnp->next) {
    }
    tlp->max = MAX ((Int2) 0, (Int2) (j - tlp->rows + 1));
    CorrectBarMax (tlp->bar, tlp->max);
    CorrectBarPage (tlp->bar, tlp->rows - 1, tlp->rows - 1);
  }
}

static CharPtr GetValueNameFromEnum (Int4 val, EnumFieldAssocPtr list)
{ 
  EnumFieldAssocPtr ap;
  
  for (ap = list; ap != NULL && ap->name != NULL; ap++)
  {
  	if (ap->value == val)
  	{
  	  return ap->name;
  	}
  }
  return NULL;
}

static Pointer ModDialogToModList (DialoG d)

{
  Char        ch;
  OrgModPtr   head;
  Int2        j;
  Int2        len;
  Boolean     okay;
  CharPtr     str;
  TagListPtr  tlp;
  CharPtr     mod_name;
  CharPtr     mod_value;
  ValNodePtr  vnp, name_vnp, value_vnp;
  ListPairPtr lpp;
  CharPtr     tmp;
  int         val;

  head = NULL;
  tlp = (TagListPtr) GetObjectExtra (d);

  lpp = (ListPairPtr) MemNew (sizeof (ListPairData));
  if (lpp == NULL) return NULL;
  lpp->selected_names_list = NULL;
  lpp->selected_values_list = NULL;

  if (tlp != NULL && tlp->vnp != NULL) {
    for (vnp = tlp->vnp; vnp != NULL; vnp = vnp->next) {
      str = (CharPtr) vnp->data.ptrvalue;
      okay = FALSE;
      len = StringLen (str);
      for (j = 0; j < len; j++) {
        ch = str [j];
        if (ch != ' ' && ch != '\t' && ch != '\n') {
          okay = TRUE;
        }
      }
      if (okay) {
        tmp = ExtractTagListColumn ((CharPtr) vnp->data.ptrvalue, 0);
        if (tmp != NULL && sscanf (tmp, "%d", &val) == 1 && val != 0) {
          mod_name = GetValueNameFromEnum (val, subsource_and_orgmod_subtype_fasta_alistX);
          if (mod_name != NULL && ! StringHasNoText (mod_name)) {
            mod_value = ExtractTagListColumn ((CharPtr) vnp->data.ptrvalue, 1);
            name_vnp = ValNodeNew (lpp->selected_names_list);
            value_vnp = ValNodeNew (lpp->selected_values_list);
            if (lpp->selected_names_list == NULL)
            {
              lpp->selected_names_list = name_vnp;
            }
            if (lpp->selected_values_list == NULL)
            {
          	  lpp->selected_values_list = value_vnp;
            }
            if (name_vnp != NULL)
            {
              name_vnp->choice = val;
              name_vnp->data.ptrvalue = mod_name;
            }
            if (value_vnp != NULL)
            {
          	  value_vnp->data.ptrvalue = mod_value;
            }
            
          }
        } 
        MemFree (tmp);
      }
    }
  }
  return (Pointer) lpp;
}

static EnumFieldAssocPtr mod_lists[] = 
  { subsource_and_orgmod_subtype_fasta_alistX, NULL };

extern DialoG CreateModifierTagList (GrouP g, ListPairPtr lpp)
{ 
  DialoG d;
  
  if (lpp == NULL) return NULL;
  d = CreateTagListDialog (g, 4, 2, STD_TAG_SPACING, mod_types,
                           mod_widths, mod_lists,
                           ModListToModDialog,
                           ModDialogToModList);
  PointerToDialog (d, lpp);
  return d;
}

extern ListPairPtr GetModifierList (DialoG d)
{
  ListPairPtr lpp;
  
  lpp = DialogToPointer (d);
  return lpp;
}

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

static ENUM_ALIST(origin_alist)
{" ",               ORG_DEFAULT    },
{"Natural",         ORG_NATURAL    },
{"Natural Mutant",  ORG_NATMUT     },
{"Mutant",          ORG_MUT        },
{"Artificial",      ORG_ARTIFICIAL },
{"Synthetic",       ORG_SYNTHETIC  },
{"Other",           ORG_OTHER      },
END_ENUM_ALIST


typedef struct setsample
{
  GetFeatureFieldString    fieldstring_func;
  GetDescriptorFieldString descrstring_func;
  Uint2                    entityID;
  ValNodePtr               field_list;
  FreeValNodeProc          free_vn_proc;
  CopyValNodeDataProc      copy_vn_proc;
  MatchValNodeProc         match_vn_proc;
  NameFromValNodeProc      label_vn_proc;  
  
  FilterSetPtr             fsp;
} SetSampleData, PNTR SetSamplePtr;

static ValNodePtr IntValNodeCopy (ValNodePtr vnp)
{
  ValNodePtr vnp_new;
  
  if (vnp == NULL)
  {
    return NULL;
  }
  vnp_new = ValNodeNew (NULL);
  if (vnp_new != NULL)
  {
    vnp_new->choice = vnp->choice;
    vnp_new->data.intvalue = vnp->data.intvalue;
  }
  return vnp_new;
}

static Boolean IntValNodeMatch (ValNodePtr vnp1, ValNodePtr vnp2)
{
  if (vnp1 == NULL && vnp2 == NULL)
  {
    return TRUE;
  }
  else if (vnp1 == NULL || vnp2 == NULL)
  {
    return FALSE;
  }
  else if (vnp1->data.intvalue == vnp2->data.intvalue)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

static void LocationConstraintClearText (LocationConstraintPtr lcp)
{
  if (lcp != NULL)
  {
    lcp->left = -1;
    lcp->right = -1;
  }
}

static LocationConstraintPtr LocationConstraintFree (LocationConstraintPtr lcp)
{
  lcp = MemFree (lcp);
  return lcp;
}

static LocationConstraintPtr LocationConstraintCopy (LocationConstraintPtr lcp)
{
  LocationConstraintPtr lcp_new;
  if (lcp == NULL)
  {
    return NULL;
  }
  lcp_new = (LocationConstraintPtr) MemNew (sizeof (LocationConstraintData));
  if (lcp_new != NULL)
  {
    lcp_new->left = lcp->left;
    lcp_new->right = lcp->right;
    lcp_new->interval_end_choice = lcp->interval_end_choice;
    lcp_new->match_choice = lcp->match_choice;
    lcp_new->strand = lcp->strand;
    lcp_new->sequence_type = lcp->sequence_type;
  }
  return lcp_new;
}

static StringConstraintPtr StringConstraintFree (StringConstraintPtr scp)
{
  if (scp == NULL) return NULL;
  scp->match_text = MemFree (scp->match_text);
  scp = MemFree (scp);
  return scp;  
}

static void StringConstraintClearText (StringConstraintPtr scp)
{
  if (scp != NULL)
  {
    scp->match_text = MemFree (scp->match_text);
  }
}

static StringConstraintPtr StringConstraintCopy (StringConstraintPtr scp)
{
  StringConstraintPtr scp_new;
  if (scp == NULL)
  {
    return NULL;
  }
  
  scp_new = (StringConstraintPtr) MemNew (sizeof (StringConstraintData));
  if (scp_new != NULL)
  {
    scp_new->match_text = StringSave (scp->match_text);
    scp_new->match_location = scp->match_location;
    scp_new->insensitive = scp->insensitive;
    scp_new->whole_word = scp->whole_word;
    scp_new->not_present = scp->not_present;   
  }
  return scp_new;
}

extern ValNodePtr ValNodeFuncFree (ValNodePtr vnp, FreeValNodeProc free_vn_proc)
{
  if (vnp == NULL)
  {
    return NULL;
  }
  vnp->next = ValNodeFuncFree (vnp->next, free_vn_proc);
  if (free_vn_proc != NULL)
  {
    free_vn_proc (vnp);
  }
  ValNodeFree (vnp);
  return NULL;
}

static ChoiceConstraintPtr ChoiceConstraintFree (ChoiceConstraintPtr scp)
{
  if (scp == NULL) return NULL;
  scp->qual_choice = ValNodeFuncFree (scp->qual_choice, scp->free_vn_proc);
  scp->string_constraint = StringConstraintFree (scp->string_constraint);
  scp = MemFree (scp);
  return scp;
}

static ValNodePtr QualChoiceCopy (ValNodePtr vnp, CopyValNodeDataProc copy_vn_proc)
{
  ValNodePtr new_list = NULL;
  
  if (vnp == NULL || copy_vn_proc == NULL)
  {
    return NULL;
  }
  
  new_list = copy_vn_proc (vnp);
  new_list->next = QualChoiceCopy (vnp->next, copy_vn_proc);
  return new_list;
}

static ChoiceConstraintPtr ChoiceConstraintCopy (ChoiceConstraintPtr ccp)
{
  ChoiceConstraintPtr ccp_new;
  if (ccp == NULL || ccp->copy_vn_proc == NULL)
  {
    return NULL;
  }
  ccp_new = (ChoiceConstraintPtr) MemNew (sizeof (ChoiceConstraintData));
  if (ccp_new != NULL)
  {
    ccp_new->constraint_type = ccp->constraint_type;
    ccp_new->qual_choice = QualChoiceCopy (ccp->qual_choice, ccp->copy_vn_proc);
    ccp_new->string_constraint = StringConstraintCopy (ccp->string_constraint);
    ccp_new->copy_vn_proc = ccp->copy_vn_proc;
    ccp_new->free_vn_proc = ccp->free_vn_proc;
  }
  return ccp_new;
}

static void ChoiceConstraintClearText (ChoiceConstraintPtr scp)
{
  if (scp != NULL)
  {
    StringConstraintClearText (scp->string_constraint);
  }
}


static void FilterSetClearText (FilterSetPtr fsp)
{
  if (fsp != NULL)
  {
    StringConstraintClearText (fsp->scp);
    ChoiceConstraintClearText (fsp->ccp);
    LocationConstraintClearText (fsp->lcp);
  }
}

static void FilterSetFree (FilterSetPtr fsp)
{
  if (fsp != NULL)
  {
    fsp->scp = StringConstraintFree (fsp->scp);
    fsp->ccp = ChoiceConstraintFree (fsp->ccp);
    fsp->lcp = LocationConstraintFree (fsp->lcp);
    fsp->cgp = ChoiceConstraintFree (fsp->cgp);
  }
}

static FilterSetPtr FilterSetCopy (FilterSetPtr fsp)
{
  FilterSetPtr fsp_new;
  
  if (fsp == NULL)
  {
    return NULL;
  }
  fsp_new = (FilterSetPtr) MemNew (sizeof (FilterSetData));
  if (fsp_new != NULL)
  {
    fsp_new->scp = StringConstraintCopy (fsp->scp);
    fsp_new->ccp = ChoiceConstraintCopy (fsp->ccp);
    fsp_new->lcp = LocationConstraintCopy (fsp->lcp);
    fsp_new->cgp = ChoiceConstraintCopy (fsp->cgp);
  }
  return fsp_new;
}


static SetSamplePtr SetSampleFree (SetSamplePtr ssp)
{
  ValNodePtr vnp;
  
  if (ssp == NULL)
  {
    return NULL;
  }
  if (ssp->free_vn_proc != NULL)
  {
    for (vnp = ssp->field_list; vnp != NULL; vnp = vnp->next)
    {
      (ssp->free_vn_proc)(vnp);
    }
  }
  ssp->field_list = ValNodeFree (ssp->field_list);
   
  ssp = MemFree (ssp);
  return ssp;
}

static SetSamplePtr SetSampleCopy (SetSamplePtr ssp)
{
  SetSamplePtr ssp_new;
  ValNodePtr   vnp_old, vnp_new, vnp_last = NULL;
  
  if (ssp == NULL)
  {
    return NULL;
  }
  ssp_new = (SetSamplePtr) MemNew (sizeof (SetSampleData));
  if (ssp_new != NULL)
  {
    ssp_new->fieldstring_func = ssp->fieldstring_func;
    ssp_new->descrstring_func = ssp->descrstring_func;
    ssp_new->free_vn_proc = ssp->free_vn_proc;
    ssp_new->copy_vn_proc = ssp->copy_vn_proc;
    ssp_new->match_vn_proc = ssp->match_vn_proc;
    ssp_new->label_vn_proc = ssp->label_vn_proc;  
    
    ssp_new->entityID = ssp->entityID;
    ssp_new->field_list = NULL;
    for (vnp_old = ssp->field_list; vnp_old != NULL; vnp_old = vnp_old->next)
    {
      vnp_new = (ssp_new->copy_vn_proc)(vnp_old);
      if (vnp_last == NULL)
      {
        ssp_new->field_list = vnp_new;
      }
      else
      {
        vnp_last->next = vnp_new;
      }
      vnp_last = vnp_new;
    }
    ssp_new->fsp = FilterSetCopy (ssp->fsp);
  }
  return ssp_new;
}

typedef struct getsample
{
  GetFeatureFieldString    fieldstring_func;
  GetDescriptorFieldString descrstring_func;
  ValNodePtr               requested_field;
  FreeValNodeProc          free_vn_proc;
  CopyValNodeDataProc      copy_vn_proc;

  CharPtr                  sample_text;
  Int4                     num_found;
  Boolean                  all_same;
} GetSampleData, PNTR GetSamplePtr;

#define PARSE_FIELD_DEFLINE       1
#define PARSE_FIELD_BIOSRC_STRING 2
#define PARSE_FIELD_SOURCE_QUAL   3
#define PARSE_FIELD_GENE_FIELD    4
#define PARSE_FIELD_MRNA_FIELD    5
#define PARSE_FIELD_CDS_COMMENT   6
#define PARSE_FIELD_PROTEIN_FIELD 7
#define PARSE_FIELD_IMPORT_QUAL   8
#define PARSE_FIELD_FEATURE_NOTE  9

#define MAX_PARSE_FIELD_TYPE   9

#define PARSE_FIELD_FIRST_FEATURE 4
#define PARSE_FIELD_LAST_FEATURE  9

extern Boolean DoesLocationMatchConstraint (SeqLocPtr slp, LocationConstraintPtr lcp);
extern void 
OperateOnSeqEntryConstrainedObjects 
(SeqEntryPtr           sep,
 FilterSetPtr          fsp,
 FeatureActionProc     feature_action,
 DescriptorActionProc  descriptor_action,
 Uint1                 seqFeatChoice,
 Uint1                 featDefChoice,
 Uint1                 descriptorChoice,
 Pointer               userdata);

extern DialoG AcceptCancelDialog 
(GrouP                 parent,
 Nlm_AcceptActnProc    accept_actn,
 Nlm_CancelActnProc    cancel_actn,
 Nlm_ClearActnProc     clear_actn,
 Nlm_ClearTextActnProc clear_text_actn,
 Pointer               userdata,
 WindoW                w);
extern void EnableAcceptCancelDialogAccept (DialoG d);
extern void DisableAcceptCancelDialogAccept (DialoG d);

typedef struct acceptpolicy 
{
  Boolean leave_dlg_up;
} AcceptPolicyData, PNTR AcceptPolicyPtr;

typedef struct acceptcanceldialog 
{
  DIALOG_MESSAGE_BLOCK
  Nlm_AcceptActnProc    accept_actn;
  Nlm_CancelActnProc    cancel_actn;
  Nlm_ClearActnProc     clear_actn;
  Nlm_ClearTextActnProc clear_text_actn;
  Pointer               userdata;
  WindoW                w;
  
  ButtoN                accept_btn;
  
  ButtoN                leave_dlg_up_btn;
} AcceptCancelDialogData, PNTR AcceptCancelDialogPtr;

static void AcceptCancelDialogAccept (ButtoN b)
{
  AcceptCancelDialogPtr acdp;

  acdp = (AcceptCancelDialogPtr) GetObjectExtra (b);
  if (acdp == NULL) return;
  
  if (acdp->accept_actn != NULL)
  {
    if (!((acdp->accept_actn) (acdp->userdata)))
    {
      return;
    }
  }
  if (! GetStatus (acdp->leave_dlg_up_btn))
  {
    Remove (acdp->w);
  }
}

static void AcceptCancelDialogCancel (ButtoN b)
{
  AcceptCancelDialogPtr acdp;

  acdp = (AcceptCancelDialogPtr) GetObjectExtra (b);
  if (acdp == NULL) return;
  
  if (acdp->cancel_actn != NULL)
  {
    (acdp->cancel_actn) (acdp->userdata);    
  }
  Remove (acdp->w);
}

static void AcceptCancelDialogClear (ButtoN b)
{
  AcceptCancelDialogPtr acdp;

  acdp = (AcceptCancelDialogPtr) GetObjectExtra (b);
  if (acdp == NULL || acdp->clear_actn == NULL) return;
  
  (acdp->clear_actn) (acdp->userdata);
}

static void AcceptCancelDialogClearText (ButtoN b)
{
  AcceptCancelDialogPtr acdp;

  acdp = (AcceptCancelDialogPtr) GetObjectExtra (b);
  if (acdp == NULL || acdp->clear_text_actn == NULL) return;
  
  (acdp->clear_text_actn) (acdp->userdata);
}

static void AcceptPolicyToAcceptCancelDialog (DialoG d, Pointer data)

{
  AcceptCancelDialogPtr   acdp;
  AcceptPolicyPtr         app;

  acdp = (AcceptCancelDialogPtr) GetObjectExtra (d);
  app = (AcceptPolicyPtr) data;
  if (acdp == NULL || app == NULL)
  {
    return;
  }

  SetStatus (acdp->leave_dlg_up_btn, app->leave_dlg_up);  
}

static Pointer AcceptCancelDialogToAcceptPolicy (DialoG d)
{
  AcceptCancelDialogPtr acdp;
  AcceptPolicyPtr       app;
  
  acdp = (AcceptCancelDialogPtr) GetObjectExtra (d);
  if (acdp == NULL) 
  {
    return NULL;
  }
  
  app = (AcceptPolicyPtr) MemNew (sizeof (AcceptPolicyData));
  if (app != NULL)
  {
    app->leave_dlg_up = GetStatus (acdp->leave_dlg_up_btn);
  }
  return app;
}

static void AcceptCancelMessage (DialoG d, Int2 mssg)

{
  AcceptCancelDialogPtr acdp;

  acdp = (AcceptCancelDialogPtr) GetObjectExtra (d);
  if (acdp != NULL) {
    switch (mssg) {
      case VIB_MSG_INIT :
        /* reset accept policy */
        SetStatus (acdp->leave_dlg_up_btn, FALSE);
        break;
      case VIB_MSG_ENTER :
        Select (acdp->accept_btn);
        break;
      default :
        break;
    }
  }
}

static ValNodePtr TestAcceptCancelDialog (DialoG d)

{
  return NULL;
}

extern DialoG AcceptCancelDialog 
(GrouP                 parent,
 Nlm_AcceptActnProc    accept_actn,
 Nlm_CancelActnProc    cancel_actn,
 Nlm_ClearActnProc     clear_actn,
 Nlm_ClearTextActnProc clear_text_actn,
 Pointer               userdata,
 WindoW                w)
{
  AcceptCancelDialogPtr acdp;
  GrouP                 grp, policy_grp = NULL, other_grp;
  ButtoN                b;
  
  acdp = (AcceptCancelDialogPtr) MemNew (sizeof (AcceptCancelDialogData));
  grp = HiddenGroup (parent, -1, 0, NULL);
  SetObjectExtra (grp, acdp, StdCleanupExtraProc);
  SetGroupSpacing (grp, 10, 10);

  acdp->dialog = (DialoG) grp;
  acdp->todialog = AcceptPolicyToAcceptCancelDialog;
  acdp->fromdialog = AcceptCancelDialogToAcceptPolicy;
  acdp->dialogmessage = AcceptCancelMessage;
  acdp->testdialog = TestAcceptCancelDialog;

  acdp->accept_actn             = accept_actn;
  acdp->cancel_actn             = cancel_actn;
  acdp->clear_actn              = clear_actn;
  acdp->clear_text_actn         = clear_text_actn;
  acdp->userdata                = userdata;
  acdp->w                       = w;

  policy_grp = HiddenGroup (grp, 3, 0, NULL);
  SetGroupSpacing (policy_grp, 10, 10);
  
  b = PushButton (policy_grp, "Clear", AcceptCancelDialogClear);
  SetObjectExtra (b, acdp, NULL);
 
  b = PushButton (policy_grp, "Clear Text", AcceptCancelDialogClearText);
  SetObjectExtra (b, acdp, NULL);
  
  acdp->leave_dlg_up_btn = CheckBox (policy_grp, "Leave Dialog Up", NULL);
  
  other_grp = HiddenGroup (grp, 5, 0, NULL);
  SetGroupSpacing (other_grp, 10, 10);
  acdp->accept_btn = PushButton (other_grp, "Accept", AcceptCancelDialogAccept);
  SetObjectExtra (acdp->accept_btn, acdp, NULL);
  
  b = PushButton (other_grp, "Cancel", AcceptCancelDialogCancel);
  SetObjectExtra (b, acdp, NULL);
  
  AlignObjects (ALIGN_CENTER, (HANDLE) policy_grp, (HANDLE) other_grp, NULL);
  
  return (DialoG) grp;
}


extern void EnableAcceptCancelDialogAccept (DialoG d)
{
  AcceptCancelDialogPtr acdp;

  acdp = (AcceptCancelDialogPtr) GetObjectExtra (d);

  if (acdp == NULL) return;
  
  SafeEnable (acdp->accept_btn);
}

extern void DisableAcceptCancelDialogAccept (DialoG d)
{
  AcceptCancelDialogPtr acdp;

  acdp = (AcceptCancelDialogPtr) GetObjectExtra (d);

  if (acdp == NULL) return;
  
  SafeDisable (acdp->accept_btn);
}


typedef struct sequenceselection
{
  DIALOG_MESSAGE_BLOCK
  DialoG     sequence_list_dlg;
  ValNodePtr sequence_choice_list;
} SequenceSelectionData, PNTR SequenceSelectionPtr;

static void CleanupSequenceSelectionDialogForm (GraphiC g, VoidPtr data)

{
  SequenceSelectionPtr dlg;

  dlg = (SequenceSelectionPtr) data;
  if (dlg != NULL) {
    dlg->sequence_choice_list = ValNodeFree (dlg->sequence_choice_list);
  }
  StdCleanupExtraProc (g, data);
}

static void ResetSequenceSelectionDialog (SequenceSelectionPtr dlg)
{  
  if (dlg != NULL)
  {
    PointerToDialog (dlg->sequence_list_dlg, NULL);
  }
}

static void SequenceSelectionListToSequenceSelectionDialog (DialoG d, Pointer userdata)
{
  SequenceSelectionPtr dlg;
  ValNodePtr           sequence_list, vnp_list, vnp_sel, pos_list = NULL;
  Int4                 i;
  SeqIdPtr             sip;
  Boolean              found;

  dlg = (SequenceSelectionPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return;
  }
  
  ResetSequenceSelectionDialog (dlg);  
  sequence_list = (ValNodePtr) userdata;
  for (vnp_list = sequence_list; vnp_list != NULL; vnp_list = vnp_list->next)
  {
    sip = (SeqIdPtr) vnp_list->data.ptrvalue;
    found = FALSE;
    while (sip != NULL && ! found)
    {
      for (vnp_sel = dlg->sequence_choice_list, i = 1;
           vnp_sel != NULL && !found;
           vnp_sel = vnp_sel->next, i++)
      {
        found = SeqIdIn (sip, vnp_sel->data.ptrvalue);
        if (found)
        {
          ValNodeAddInt (&pos_list, 0, i);
        }
      }
      sip = sip->next;
    }
  }
  PointerToDialog (dlg->sequence_list_dlg, pos_list);
  ValNodeFree (pos_list);
}

static Pointer SequenceSelectionDialogToSequenceSelectionList (DialoG d)
{
  SequenceSelectionPtr dlg;
  ValNodePtr           sequence_list = NULL, vnp_list, pos_list, vnp_pos;
  Int4                 i;

  dlg = (SequenceSelectionPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return NULL;
  }
  
  pos_list = DialogToPointer (dlg->sequence_list_dlg);
  for (vnp_pos = pos_list; vnp_pos != NULL; vnp_pos = vnp_pos->next)
  {
    for (i = 1, vnp_list = dlg->sequence_choice_list;
         i < vnp_pos->data.intvalue && vnp_list != NULL;
         i++, vnp_list = vnp_list->next)
    {
    }
    if (i == vnp_pos->data.intvalue && vnp_list != NULL)
    {
      ValNodeAddPointer (&sequence_list, 0, vnp_list->data.ptrvalue);
    }
  }
  return sequence_list;
}

static void 
GetSequenceChoiceList 
(SeqEntryPtr sep,
 ValNodePtr PNTR list, 
 Boolean show_nucs, 
 Boolean show_prots)
{
  BioseqPtr                bsp;
  BioseqSetPtr             bssp;
  
  if (sep == NULL) return;
  
  if (IS_Bioseq (sep))
  {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp == NULL) return;
    if (!show_nucs && ISA_na (bsp->mol))
    {
      return;
    }
    if (!show_prots && ISA_aa (bsp->mol))
    {
      return;
    }
    ValNodeAddPointer (list, 0, bsp->id);
  }
  else
  {
  	bssp = (BioseqSetPtr) sep->data.ptrvalue;
    for (sep = bssp->seq_set; sep != NULL; sep = sep->next) 
    {
      GetSequenceChoiceList (sep, list, show_nucs, show_prots);
    }
  }
}

static void SequenceSelectionDialogMessage (DialoG d, Int2 mssg)

{
  SequenceSelectionPtr dlg;

  dlg = (SequenceSelectionPtr) GetObjectExtra (d);
  if (dlg != NULL) {
    switch (mssg) {
      case VIB_MSG_INIT :
        /* reset list */
        ResetSequenceSelectionDialog (dlg);
        break;
      case VIB_MSG_SELECT:
        Select (dlg->sequence_list_dlg);
        break;
      case VIB_MSG_ENTER :
        Select (dlg->sequence_list_dlg);
        break;
      case NUM_VIB_MSG + 1:
        SendMessageToDialog (dlg->sequence_list_dlg, NUM_VIB_MSG + 1);
        break;
      default :
        break;
    }
  }
}

static ValNodePtr TestSequenceSelectionDialog (DialoG d)

{
  SequenceSelectionPtr dlg;
  ValNodePtr           head = NULL;

  dlg = (SequenceSelectionPtr) GetObjectExtra (d);
  if (dlg != NULL) {
    head = TestDialog (dlg->sequence_list_dlg);
  }
  return head;
}


extern DialoG SequenceSelectionDialog 
(GrouP h,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata,
 Boolean                  allow_multi,
 Boolean                  show_nucs,
 Boolean                  show_prots,
 Uint2                    entityID)

{
  SequenceSelectionPtr  dlg;
  GrouP                 p;
  ValNodePtr                vnp;
  SeqEntryPtr               sep;
  SeqIdPtr                  sip;
  Char                      tmp[128];
  ValNodePtr            choice_name_list = NULL;
  
  if (!show_nucs && ! show_prots)
  {
    return NULL;
  }
  
  sep = GetTopSeqEntryForEntityID (entityID);
  if (sep == NULL)
  {
    return NULL;
  }

  dlg = (SequenceSelectionPtr) MemNew (sizeof (SequenceSelectionData));
  if (dlg == NULL)
  {
    return NULL;
  }
  
  p = HiddenGroup (h, 1, 0, NULL);
  SetObjectExtra (p, dlg, CleanupSequenceSelectionDialogForm);

  dlg->dialog = (DialoG) p;
  dlg->todialog = SequenceSelectionListToSequenceSelectionDialog;
  dlg->fromdialog = SequenceSelectionDialogToSequenceSelectionList;
  dlg->dialogmessage = SequenceSelectionDialogMessage;
  dlg->testdialog = TestSequenceSelectionDialog;

  dlg->sequence_choice_list = NULL;
  GetSequenceChoiceList (sep, &dlg->sequence_choice_list, show_nucs, show_prots);
  
  
  for (vnp = dlg->sequence_choice_list; vnp != NULL; vnp = vnp->next) {
    sip = SeqIdFindWorst ((SeqIdPtr) vnp->data.ptrvalue);
    SeqIdWrite (sip, tmp, PRINTID_REPORT, sizeof (tmp));
    ValNodeAddPointer (&choice_name_list, 0, StringSave (tmp));
  }

  dlg->sequence_list_dlg = SelectionDialog (p, change_notify, change_userdata,
                                            allow_multi, "sequence",
                                            choice_name_list, TALL_SELECTION_LIST);
  ValNodeFreeData (choice_name_list); 
  return (DialoG) p;
}


static ValNodePtr ValNodeAppend (ValNodePtr list, ValNodePtr second_list)
{
  ValNodePtr last_vnp;
  
  if (list == NULL)
  {
    list = second_list;
  }
  else if (second_list != NULL)
  {
    last_vnp = list;
    while (last_vnp != NULL && last_vnp->next != NULL)
    {
      last_vnp = last_vnp->next;
    }
    last_vnp->next = second_list;
  }
  return list;
}

static ValNodePtr BuildFeatureDialogList (Boolean list_most_used_first)
{
  ValNodePtr  feature_choice_list = NULL;
  ValNodePtr  import_feature_list = NULL;
  ValNodePtr  least_liked_feature_list = NULL;
  ValNodePtr  most_used_feature_list = NULL;
  FeatDefPtr  curr;
  Uint1       key;
  CharPtr     label = NULL;
  Char        str [256];

  curr = FeatDefFindNext (NULL, &key, &label, FEATDEF_ANY, TRUE);
  while (curr != NULL) {
    if (key == FEATDEF_BAD
        || key == FEATDEF_Imp_CDS
        || key == FEATDEF_source
        || key == FEATDEF_ORG)
    {
      curr = FeatDefFindNext (curr, &key, &label, FEATDEF_ANY, TRUE);
      continue;
    }
      
    if (curr->featdef_key == FEATDEF_PUB)
    {
      StringNCpy_0 (str, curr->typelabel, sizeof (str) - 15);
      StringCat (str, " (Publication)");
      ValNodeAddPointer (&feature_choice_list, curr->featdef_key, StringSave (str));
    }
    else if (curr->featdef_key == FEATDEF_BIOSRC)
    {
      ValNodeAddPointer (&feature_choice_list, FEATDEF_BIOSRC, StringSave ("BioSource"));
    }
    else if (curr->featdef_key == FEATDEF_otherRNA)
    {
      ValNodeAddPointer (&feature_choice_list, FEATDEF_otherRNA, StringSave ("misc_RNA"));
      if (list_most_used_first)
      {
        /* also add to most used list */
        ValNodeAddPointer (&most_used_feature_list, curr->featdef_key, 
                           StringSave ("misc_RNA"));
      }
    }
    else if (curr->featdef_key == FEATDEF_misc_RNA ||
            curr->featdef_key == FEATDEF_precursor_RNA ||
            curr->featdef_key == FEATDEF_mat_peptide ||
            curr->featdef_key == FEATDEF_sig_peptide ||
            curr->featdef_key == FEATDEF_transit_peptide ||
            curr->featdef_key == FEATDEF_Imp_CDS)
    {
      StringNCpy_0 (str, curr->typelabel, sizeof (str) - 10);
      StringCat (str, "_imp");
      ValNodeAddPointer (&import_feature_list, curr->featdef_key, StringSave (str));
    }
    else if (curr->featdef_key == FEATDEF_TXINIT)
    {
      ValNodeAddPointer (&least_liked_feature_list, FEATDEF_TXINIT, StringSave (curr->typelabel));
    }
    else
    {
      ValNodeAddPointer (&feature_choice_list, curr->featdef_key, StringSave (curr->typelabel));
    }
    
    if (list_most_used_first)
    {
      /* also build most used list */
      if (curr->featdef_key == FEATDEF_CDS
          || curr->featdef_key == FEATDEF_exon
          || curr->featdef_key == FEATDEF_GENE
          || curr->featdef_key == FEATDEF_intron
          || curr->featdef_key == FEATDEF_mRNA
          || curr->featdef_key == FEATDEF_rRNA)
      {
        ValNodeAddPointer (&most_used_feature_list, curr->featdef_key, 
                           StringSave (curr->typelabel));
      }
    }
    
    curr = FeatDefFindNext (curr, &key, &label, FEATDEF_ANY, TRUE);
  }

  most_used_feature_list = SortValNode (most_used_feature_list,
                                        CompareFeatureValNodeStrings);
  feature_choice_list = SortValNode (feature_choice_list,
                                     CompareFeatureValNodeStrings);
  import_feature_list = SortValNode (import_feature_list,
                                     CompareFeatureValNodeStrings);
  least_liked_feature_list = SortValNode (least_liked_feature_list,
                                          CompareFeatureValNodeStrings);

  feature_choice_list = ValNodeAppend (most_used_feature_list, feature_choice_list);
  feature_choice_list = ValNodeAppend (feature_choice_list, least_liked_feature_list);
  feature_choice_list = ValNodeAppend (feature_choice_list, import_feature_list);
  
  return feature_choice_list;
}

static ValNodePtr BuildImportList (void)
{
  ValNodePtr  import_feature_list = NULL;
  FeatDefPtr  curr;
  Uint1       key;
  CharPtr     label = NULL;
  Char        str [256];

  curr = FeatDefFindNext (NULL, &key, &label, FEATDEF_ANY, TRUE);
  while (curr != NULL) {
    if (FindFeatFromFeatDefType (key) == SEQFEAT_IMP)
    {
      ValNodeAddPointer (&import_feature_list, curr->featdef_key, StringSave (str));
    }
    
    curr = FeatDefFindNext (curr, &key, &label, FEATDEF_ANY, TRUE);
  } 
  return import_feature_list; 
}

static void RemoveFeatureSelectionDuplicates (ValNodePtr orig_list)
{
  ValNodePtr vnp1, vnp2, prev_vnp, next_vnp;
  
  if (orig_list == NULL || orig_list->next == NULL)
  {
    return;
  }
  
  vnp1 = orig_list;
  while (vnp1 != NULL && vnp1->next != NULL)
  {
    vnp2 = vnp1->next;
    prev_vnp = vnp1;
    while (vnp2 != NULL)
    {
      next_vnp = vnp2->next;
      if (vnp2->choice == vnp1->choice)
      {
        prev_vnp->next = next_vnp;
        vnp2->next = NULL;
        vnp2 = ValNodeFreeData (vnp2);
      }
      else
      {
        prev_vnp = vnp2;
      }
      vnp2 = next_vnp;
    }
    vnp1 = vnp1->next;
  }
}

static ValNodePtr FeatureSelectionRemap (ValNodePtr orig_list)
{
  ValNodePtr vnp, vnp_next, prev_vnp, repl_vnp = NULL;
  
  if (orig_list == NULL)
  {
    return NULL;
  }
  
  vnp = orig_list;
  prev_vnp = NULL;
  while (vnp != NULL && repl_vnp == NULL)
  {
    vnp_next = vnp->next;
    /* replace FEATDEF_IMP with list of import features */
    if (vnp->choice == FEATDEF_IMP)
    {
      /* build replacement list */
      repl_vnp = BuildImportList ();
      
      ValNodeLink (&repl_vnp, vnp->next);
      
      if (prev_vnp == NULL)
      {
        orig_list = repl_vnp;
      }
      else
      {
        prev_vnp->next = repl_vnp;
      }

      vnp->next = NULL;
      ValNodeFreeData (vnp);
    }
    else
    {
      prev_vnp = vnp;
    }
    vnp = vnp_next;
  }
  
  RemoveFeatureSelectionDuplicates (orig_list);
  
  return orig_list; 
}

static DialoG 
FeatureSelectionDialog 
(GrouP                    h,
 Boolean                  allow_multi,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata)
{
  DialoG      dlg;
  ValNodePtr  feature_choice_list, vnp, vnp_next, vnp_prev;
  Boolean     found_import = FALSE;

  feature_choice_list = BuildFeatureDialogList (TRUE);
  
  if (!allow_multi)
  {
    /* remove Import option */
    vnp_prev = NULL;
    for (vnp = feature_choice_list; vnp != NULL && !found_import; vnp = vnp_next)
    {
      vnp_next = vnp->next;
      if (vnp->choice == FEATDEF_IMP)
      {
        if (vnp_prev == NULL)
        {
          feature_choice_list = vnp_next;
        }
        else
        {
          vnp_prev->next = vnp_next;
        }
        vnp->next = NULL;
        vnp = ValNodeFreeData (vnp);
        found_import = TRUE;
      }
      else
      {
        vnp_prev = vnp;
      }
    }
  }
  
  /* note - the ValNodeSelectionDialog will free the feature_choice_list when done */
  
  dlg = ValNodeSelectionDialogEx (h, feature_choice_list, TALL_SELECTION_LIST, 
                                ValNodeStringName,
                                ValNodeSimpleDataFree, ValNodeStringCopy,
                                ValNodeChoiceMatch, "feature list", 
                                change_notify, change_userdata, allow_multi,
                                allow_multi ? FeatureSelectionRemap : NULL);
  return dlg;
}

extern CharPtr SourceQualValNodeName (ValNodePtr vnp)
{
  SourceQualDescPtr           sqdp;
  
  if (vnp == NULL || vnp->data.ptrvalue == NULL)
  {
    return NULL;
  }
  else if (vnp->choice == 0)
  {
    sqdp = (SourceQualDescPtr) vnp->data.ptrvalue;
    return StringSave (sqdp->name);
  }
  else
  {
    return StringSave (vnp->data.ptrvalue);
  }
}

extern ValNodePtr SourceQualValNodeDataCopy (ValNodePtr vnp)
{
  SourceQualDescPtr           sqdp, new_sqdp = NULL;
  ValNodePtr                  vnp_copy = NULL;
  
  if (vnp != NULL && vnp->data.ptrvalue != NULL)
  {
    if (vnp->choice == 0)
    {
      sqdp = (SourceQualDescPtr) vnp->data.ptrvalue;
      new_sqdp = (SourceQualDescPtr) MemNew (sizeof (SourceQualDescData));
      if (new_sqdp != NULL)
      {
        MemCpy (new_sqdp, sqdp, sizeof (SourceQualDescData));
      }
      ValNodeAddPointer (&vnp_copy, vnp->choice, new_sqdp);
    }
    else
    {
      ValNodeAddPointer (&vnp_copy, vnp->choice, StringSave (vnp->data.ptrvalue));
    }
  }
  return vnp_copy;
}

extern Boolean SourceQualValNodeMatch (ValNodePtr vnp1, ValNodePtr vnp2)
{
  SourceQualDescPtr  sqdp1, sqdp2;
  Boolean            rval = FALSE;
  
  if (vnp1 == NULL || vnp2 == NULL
      || vnp1->data.ptrvalue == NULL || vnp2->data.ptrvalue == NULL)
  {
    rval = FALSE;
  }
  else if (vnp1->choice != vnp2->choice)
  {
    rval = FALSE;
  }
  else if (vnp1->choice == 0)
  {
    sqdp1 = (SourceQualDescPtr) vnp1->data.ptrvalue;
    sqdp2 = (SourceQualDescPtr) vnp2->data.ptrvalue;
  
    if (sqdp1->subtype == sqdp2->subtype
        && ((sqdp1->isOrgMod && sqdp2->isOrgMod)
            || (!sqdp1->isOrgMod && ! sqdp2->isOrgMod)))
    {
      rval = TRUE;
    }
    else
    {
      rval = FALSE;
    }
  }
  else
  {
    if (StringCmp (vnp1->data.ptrvalue, vnp2->data.ptrvalue) == 0)
    {
      rval = TRUE;
    }
    else
    {
      rval = FALSE;
    }
  }
  return rval;
}

static DialoG SourceQualTypeSelectionDialogEx
(GrouP h,
 Boolean                  allow_multi,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata,
 Int2                     list_height)

{
  DialoG     dlg;
  ValNodePtr qual_choice_list;

  if (allow_multi)
  {
    qual_choice_list = ValNodeNew (NULL);
    qual_choice_list->choice = 1;
    qual_choice_list->data.ptrvalue = StringSave ("All Notes");
    qual_choice_list->next = GetSourceQualDescList (TRUE, TRUE);
  }
  else
  {
    qual_choice_list = GetSourceQualDescList (TRUE, TRUE);
  }
  /* note - the ValNodeSelectionDialog will free the qual_choice_list when done */                                            
  dlg = ValNodeSelectionDialog (h, qual_choice_list, list_height, SourceQualValNodeName,
                                ValNodeSimpleDataFree, SourceQualValNodeDataCopy,
                                SourceQualValNodeMatch, "feature list", 
                                change_notify, change_userdata, allow_multi);

  return dlg;
}

extern DialoG SourceQualTypeSelectionDialog 
(GrouP h,
 Boolean                  allow_multi,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata)
 
{
  return SourceQualTypeSelectionDialogEx (h, allow_multi, change_notify,
                                          change_userdata, 6);
}

static DialoG SourceQualTypeConstraintSelectionDialog
(GrouP h,
 Boolean                  allow_multi,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata)

{
  return SourceQualTypeSelectionDialogEx (h, allow_multi, change_notify,
                                          change_userdata, 4);
}

static DialoG SourceStringConstraintSelectionDialog
(GrouP h,
 Boolean                  allow_multi,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata)

{
  DialoG     dlg;
  ValNodePtr qual_choice_list = NULL;

  /* ignore allow_multi */
  
  ValNodeAddPointer (&qual_choice_list, 1, StringSave ("Organism or Any Qual"));
  ValNodeAddPointer (&qual_choice_list, 1, StringSave ("Organism"));
  qual_choice_list->next->next = GetSourceQualDescList (TRUE, TRUE);
  ValNodeAddPointer (&qual_choice_list, 1, StringSave ("Lineage"));

  /* note - the ValNodeSelectionDialog will free the qual_choice_list when done */                                            
  dlg = ValNodeSelectionDialog (h, qual_choice_list, 4, SourceQualValNodeName,
                                ValNodeSimpleDataFree, SourceQualValNodeDataCopy,
                                SourceQualValNodeMatch, "feature list", 
                                change_notify, change_userdata, FALSE);

  return dlg;
}

static Boolean OrgModSpecialMatch (OrgModPtr mod, FilterSetPtr fsp)
{
  SourceQualDescPtr sqdp;
  
  if (mod == NULL)
  {
    return FALSE;
  }
  if (fsp == NULL
      || fsp->ccp == NULL 
      || fsp->ccp->constraint_type != CHOICE_CONSTRAINT_STRING
      || fsp->ccp->string_constraint == NULL
      || fsp->ccp->qual_choice == NULL
      || fsp->ccp->qual_choice->data.ptrvalue == NULL)
  {
    return TRUE;
  }
  
  sqdp = (SourceQualDescPtr) fsp->ccp->qual_choice->data.ptrvalue;
  if (sqdp->subtype != mod->subtype)
  {
    return TRUE;
  }
  
  if (DoesStringMatchConstraint (mod->subname, fsp->ccp->string_constraint))
  {
    if (fsp->ccp->string_constraint->not_present)
    {
      return FALSE;
    }
    else
    {
      return TRUE;
    }
  }
  else
  {
    if (fsp->ccp->string_constraint->not_present)
    {
      return TRUE;
    }
    else
    {
      return FALSE;
    }
  }
}

static Boolean SubSrcSpecialMatch (SubSourcePtr ssp, FilterSetPtr fsp)
{
  SourceQualDescPtr sqdp;
  
  if (ssp == NULL)
  {
    return FALSE;
  }
  if (fsp == NULL
      || fsp->ccp == NULL 
      || fsp->ccp->constraint_type != CHOICE_CONSTRAINT_STRING
      || fsp->ccp->string_constraint == NULL
      || fsp->ccp->qual_choice == NULL
      || fsp->ccp->qual_choice->data.ptrvalue == NULL)
  {
    return TRUE;
  }
  
  sqdp = (SourceQualDescPtr) fsp->ccp->qual_choice->data.ptrvalue;
  if (sqdp->subtype != ssp->subtype)
  {
    return TRUE;
  }
  if (DoesStringMatchConstraint (ssp->name, fsp->ccp->string_constraint))
  {
    if (fsp->ccp->string_constraint->not_present)
    {
      return FALSE;
    }
    else
    {
      return TRUE;
    }
  }
  else
  {
    if (fsp->ccp->string_constraint->not_present)
    {
      return TRUE;
    }
    else
    {
      return FALSE;
    }
  }
}

static CharPtr 
GetSourceQualQualValueFromBiop 
(BioSourcePtr      biop,
 SourceQualDescPtr sqdp,
 FilterSetPtr      fsp)
{
  CharPtr            str = NULL;
  OrgModPtr          mod;
  SubSourcePtr       ssp;
  
  if (biop == NULL || sqdp == NULL)
  {
    return NULL;
  }

  if (sqdp->isOrgMod)
  {
    if (biop->org != NULL && biop->org->orgname != NULL)
    {
      mod = biop->org->orgname->mod;
      while (mod != NULL && str == NULL)
      {
        if (mod->subtype == sqdp->subtype
            && !StringHasNoText (mod->subname)
            && OrgModSpecialMatch (mod, fsp))
        {
          str = StringSave (mod->subname);
        }
        mod = mod->next;
      }
    }
  }
  else
  {
    ssp = biop->subtype;
    while (ssp != NULL && str == NULL)
    {
      if (ssp->subtype == sqdp->subtype
          && !StringHasNoText (ssp->name)
          && SubSrcSpecialMatch (ssp, fsp))
      {
        str = StringSave (ssp->name);
      }
      ssp = ssp->next;
    }
  }
  return str;  
}

static CharPtr GetLocationFromSource (BioSourcePtr biop, ValNodePtr vnp)
{
  Nlm_EnumFieldAssocPtr eap;
  
  if (biop == NULL)
  {
    return NULL;
  }
  
  for (eap = biosource_genome_alistX;
       eap->name != NULL && eap->value != biop->genome;
       eap++)
  {
  }
  
  if (eap != NULL && eap->name != NULL && !StringHasNoText (eap->name))
  {
    return StringSave (eap->name);
  }
  else
  {
    return NULL;
  }  
}

static CharPtr GetLocationFromSourceFeature (SeqFeatPtr sfp, ValNodePtr vnp, FilterSetPtr fsp)
{
  if (sfp != NULL && sfp->data.choice == SEQFEAT_BIOSRC)
  {
    return GetLocationFromSource (sfp->data.value.ptrvalue, vnp);
  }
  else
  {
    return NULL;
  }
}

static CharPtr 
GetLocationFromSourceDescriptor 
(SeqDescrPtr  sdp,
 ValNodePtr   vnp, 
 FilterSetPtr fsp)
{
  if (sdp != NULL && sdp->choice == Seq_descr_source)
  {
    return GetLocationFromSource (sdp->data.ptrvalue, vnp);
  }
  else
  {
    return NULL;
  }
}

static CharPtr GetOriginFromSource (BioSourcePtr biop, ValNodePtr vnp)
{
  Nlm_EnumFieldAssocPtr eap;
  
  if (biop == NULL)
  {
    return NULL;
  }
  
  for (eap = origin_alist;
       eap->name != NULL && eap->value != biop->origin;
       eap++)
  {
  }
  
  if (eap != NULL && eap->name != NULL && !StringHasNoText (eap->name))
  {
    return StringSave (eap->name);
  }
  else
  {
    return NULL;
  }  
}

static CharPtr 
GetOriginFromSourceFeature 
(SeqFeatPtr   sfp,
 ValNodePtr   vnp,
 FilterSetPtr fsp)
{
  if (sfp != NULL && sfp->data.choice == SEQFEAT_BIOSRC)
  {
    return GetOriginFromSource (sfp->data.value.ptrvalue, vnp);
  }
  else
  {
    return NULL;
  }
}

static CharPtr 
GetOriginFromSourceDescriptor 
(SeqDescrPtr  sdp,
 ValNodePtr   vnp,
 FilterSetPtr fsp)
{
  if (sdp != NULL && sdp->choice == Seq_descr_source)
  {
    return GetOriginFromSource (sdp->data.ptrvalue, vnp);
  }
  else
  {
    return NULL;
  }
}

static CharPtr GetStringFromSource (BioSourcePtr biop, ValNodePtr vnp)
{
  Nlm_EnumFieldAssocPtr eap;
  
  if (biop == NULL)
  {
    return NULL;
  }
  
  for (eap = orgref_textfield_alist;
       eap->name != NULL && StringCmp (vnp->data.ptrvalue, eap->name) != 0;
       eap++)
  {
  }
  
  if (eap != NULL && eap->name != NULL)
  {
    switch (eap->value)
    {
      case 1:
        if (biop->org != NULL && !StringHasNoText (biop->org->taxname))
        {
          return StringSave (biop->org->taxname);
        }
        break;
      case 2:
        if (biop->org != NULL && !StringHasNoText (biop->org->common))
        {
          return StringSave (biop->org->common);
        }
        break;
      case 3:
        if (biop->org != NULL && biop->org->orgname != NULL
            && !StringHasNoText (biop->org->orgname->lineage))
        {
          return StringSave (biop->org->orgname->lineage);
        }
        break;
      case 4:
        if (biop->org != NULL && biop->org->orgname != NULL
            && !StringHasNoText (biop->org->orgname->div))
        {
          return StringSave (biop->org->orgname->div);
        }
        break;
    }
  }
  return NULL;  
}

static CharPtr GetStringFromSourceFeature (SeqFeatPtr sfp, ValNodePtr vnp, FilterSetPtr fsp)
{
  if (sfp != NULL && sfp->data.choice == SEQFEAT_BIOSRC)
  {
    return GetStringFromSource (sfp->data.value.ptrvalue, vnp);
  }
  else
  {
    return NULL;
  }
}

static CharPtr StringAppend (CharPtr str1, CharPtr str2)
{
  CharPtr new_str;
  
  if (str1 == NULL && str2 == NULL)
  {
    return NULL;
  }
  else if (str1 == NULL)
  {
    return StringSave (str2);
  }
  else if (str2 == NULL)
  {
    return StringSave (str1);
  }
  else
  {
    new_str = (CharPtr) MemNew ((StringLen (str1) + StringLen (str2) + 3) * sizeof (Char));
    if (new_str != NULL)
    {
      sprintf (new_str, "%s; %s", str1, str2);  
    }
    return new_str;
  }
}

static CharPtr GetStringFromSourceDescriptor (SeqDescrPtr sdp, ValNodePtr vnp, FilterSetPtr fsp)
{
  if (sdp != NULL && sdp->choice == Seq_descr_source)
  {
    return GetStringFromSource (sdp->data.ptrvalue, vnp);
  }
  else
  {
    return NULL;
  }
}

static CharPtr 
GetSourceQualStringFromBioSource 
(BioSourcePtr biop,
 ValNodePtr   vnp,
 FilterSetPtr fsp)
{
  CharPtr            str = NULL, str1 = NULL, str2 = NULL;
  SourceQualDescData sqdd;
  
  if (biop == NULL || vnp == NULL)
  {
    return NULL;
  }
  
  while (vnp != NULL && str == NULL)
  {
    if (vnp->choice == 0)
    {
      str = GetSourceQualQualValueFromBiop (biop, vnp->data.ptrvalue, fsp);   
    }
    else if (vnp->choice == 1)
    {
      if (StringCmp (vnp->data.ptrvalue, "Location") == 0)
      {
        str = GetLocationFromSource (biop, vnp);
      }
      else if (StringCmp (vnp->data.ptrvalue, "Origin") == 0)
      {
        str = GetOriginFromSource (biop, vnp);
      }
      else if (StringCmp (vnp->data.ptrvalue, "All Notes") == 0)
      {
        sqdd.name = "note";
        sqdd.isOrgMod = TRUE;
        sqdd.subtype = 255;
        str1 = GetSourceQualQualValueFromBiop (biop, &sqdd, fsp);
        sqdd.isOrgMod = FALSE;
        str2 = GetSourceQualQualValueFromBiop (biop, &sqdd, fsp);
        str = StringAppend (str1, str2);
        str1 = MemFree (str1);
        str2 = MemFree (str2);
      }
      else
      {
        str = GetStringFromSource (biop, vnp);
      }
    }
    vnp = vnp->next;
  }
  return str;
}

static CharPtr 
GetSourceQualDescrString 
(SeqDescrPtr  sdp,
 ValNodePtr   vnp,
 FilterSetPtr fsp)
{
  if (sdp == NULL || sdp->choice != Seq_descr_source || vnp == NULL)
  {
    return NULL;
  }
  return GetSourceQualStringFromBioSource (sdp->data.ptrvalue, vnp, fsp);
}

static CharPtr 
GetSourceQualFeatureString 
(SeqFeatPtr   sfp,
 ValNodePtr   vnp,
 FilterSetPtr fsp)
{
  if (sfp == NULL || sfp->data.choice != SEQFEAT_BIOSRC || vnp == NULL)
  {
    return NULL;
  }
  
  return GetSourceQualStringFromBioSource (sfp->data.value.ptrvalue, vnp, fsp);
}

static void ApplyLocationToBiop (BioSourcePtr biop, ApplyValuePtr avp)
{
  if (biop == NULL || avp == NULL || avp->field_list == NULL)
  {
    return;
  }
  
  if (avp->etp == NULL 
      || avp->etp->existing_text_choice == EXISTING_TEXT_CHOICE_REPLACE_OLD
      || biop->genome == 0)
  {
    biop->genome = avp->field_list->choice;
  }
}

static void ApplyLocationToSourceFeature (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  if (sfp != NULL && sfp->data.choice == SEQFEAT_BIOSRC)
  {
    ApplyLocationToBiop (sfp->data.value.ptrvalue, userdata);
  }
}

static void ApplyLocationToSourceDescriptor (SeqDescrPtr sdp, Pointer userdata, FilterSetPtr fsp)
{
  if (sdp != NULL && sdp->choice == Seq_descr_source)
  {
    ApplyLocationToBiop (sdp->data.ptrvalue, userdata);
  }
}

static void RemoveLocationFromBiop (BioSourcePtr biop, ApplyValuePtr avp)
{
  if (biop == NULL || avp == NULL || avp->field_list == NULL)
  {
    return;
  }
  
  if (biop->genome == avp->field_list->choice)
  {
    biop->genome = 0;
  }
}

static void RemoveLocationFromSourceFeature (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  if (sfp != NULL && sfp->data.choice == SEQFEAT_BIOSRC)
  {
    RemoveLocationFromBiop (sfp->data.value.ptrvalue, userdata);
  }
}

static void RemoveLocationFromSourceDescriptor (SeqDescrPtr sdp, Pointer userdata, FilterSetPtr fsp)
{
  if (sdp != NULL && sdp->choice == Seq_descr_source)
  {
    RemoveLocationFromBiop (sdp->data.ptrvalue, userdata);
  }
}

static void ApplyOriginToBiop (BioSourcePtr biop, ApplyValuePtr avp)
{
  if (biop == NULL || avp == NULL || avp->field_list == NULL)
  {
    return;
  }
  
  if (avp->etp == NULL 
      || avp->etp->existing_text_choice == EXISTING_TEXT_CHOICE_REPLACE_OLD
      || biop->origin == 0)
  {
    biop->origin = avp->field_list->choice;
  }
}

static void ApplyOriginToSourceFeature (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  if (sfp != NULL && sfp->data.choice == SEQFEAT_BIOSRC)
  {
    ApplyOriginToBiop (sfp->data.value.ptrvalue, userdata);
  }
}

static void ApplyOriginToSourceDescriptor (SeqDescrPtr sdp, Pointer userdata, FilterSetPtr fsp)
{
  if (sdp != NULL && sdp->choice == Seq_descr_source)
  {
    ApplyOriginToBiop (sdp->data.ptrvalue, userdata);
  }
}

static void RemoveOriginFromBiop (BioSourcePtr biop, ApplyValuePtr avp)
{
  if (biop == NULL || avp == NULL || avp->field_list == NULL)
  {
    return;
  }
  
  if (biop->origin == avp->field_list->choice)
  {
    biop->origin = 0;
  }
}

static void RemoveOriginFromSourceFeature (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  if (sfp != NULL && sfp->data.choice == SEQFEAT_BIOSRC)
  {
    RemoveOriginFromBiop (sfp->data.value.ptrvalue, userdata);
  }
}

static void RemoveOriginFromSourceDescriptor (SeqDescrPtr sdp, Pointer userdata, FilterSetPtr fsp)
{
  if (sdp != NULL && sdp->choice == Seq_descr_source)
  {
    RemoveOriginFromBiop (sdp->data.ptrvalue, userdata);
  }
}

static void ApplyStringToSource (BioSourcePtr biop, Pointer userdata)
{
  ApplyValuePtr         avp;
  Boolean               found = FALSE;

  if (biop == NULL || userdata == NULL)
  {
    return;
  }
  
  avp = (ApplyValuePtr) userdata;
  if (avp == NULL || avp->field_list == NULL)
  {
    return;
  }

  if (StringHasNoText (avp->text_to_replace))
  {
    if (biop->org == NULL)
    {
      biop->org = OrgRefNew();
    }
    if (avp->field_list->choice == 3
        || avp->field_list->choice == 4)
    {
      biop->org->orgname = OrgNameNew ();
    }
  }
  
  switch (avp->field_list->choice)
  {
    case 1:
      biop->org->taxname = HandleApplyValue (biop->org->taxname, avp);
      break;
    case 2:
      biop->org->common = HandleApplyValue (biop->org->common, avp);
      break;
    case 3:
      biop->org->orgname->lineage = HandleApplyValue (biop->org->orgname->lineage, avp);
      break;
    case 4:
      biop->org->orgname->div = HandleApplyValue (biop->org->orgname->div, avp);
      break;
  }  
}

static void ApplyStringToSourceFeature (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  if (sfp != NULL && sfp->data.choice == SEQFEAT_BIOSRC)
  {
    ApplyStringToSource (sfp->data.value.ptrvalue, userdata);
  }
}

static void ApplyStringToSourceDescriptor (SeqDescrPtr sdp, Pointer userdata, FilterSetPtr fsp)
{
  if (sdp != NULL && sdp->choice == Seq_descr_source)
  {
    ApplyStringToSource (sdp->data.ptrvalue, userdata);
  }
}

static void RemoveStringFromSource (BioSourcePtr biop, Pointer userdata)
{
  ApplyValuePtr avp;
  
  if (biop == NULL || biop->org == NULL || userdata == NULL)
  {
    return;
  }
  
  avp = (ApplyValuePtr) userdata;
  if (avp == NULL || avp->field_list == NULL)
  {
    return;
  }
  
  switch (avp->field_list->choice)
  {
    case 1:
      biop->org->taxname = MemFree (biop->org->taxname);
      break;
    case 2:
      biop->org->common = MemFree (biop->org->common);
      break;
    case 3:
      if (biop->org->orgname != NULL)
      {
        biop->org->orgname->lineage = MemFree (biop->org->orgname->lineage);
      }
      break;
    case 4:
      if (biop->org->orgname != NULL)
      {
        biop->org->orgname->div = MemFree (biop->org->orgname->div);
      }
      break;
  }   
}

static void RemoveStringFromSourceFeature (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  if (sfp != NULL && sfp->data.choice == SEQFEAT_BIOSRC)
  {
    RemoveStringFromSource (sfp->data.value.ptrvalue, userdata);
  }
}

static void RemoveStringFromSourceDescriptor (SeqDescrPtr sdp, Pointer userdata, FilterSetPtr fsp)
{
  if (sdp != NULL && sdp->choice == Seq_descr_source)
  {
    RemoveStringFromSource (sdp->data.ptrvalue, userdata);
  }
}

static void RemoveSourceQualFromBioSource (BioSourcePtr biop, Pointer userdata, FilterSetPtr fsp);

static void ApplySourceQualBioSourceCallback (BioSourcePtr biop, Pointer userdata, FilterSetPtr fsp)
{
  ApplyValuePtr         avp;
  SourceQualDescPtr     sqdp;
  OrgModPtr             mod = NULL, last_mod = NULL;
  SubSourcePtr          ssp = NULL, last_ssp = NULL;
  Boolean               found = FALSE;
  Boolean               is_nontext;
  
  if (biop == NULL || userdata == NULL)
  {
    return;
  }
  
  avp = (ApplyValuePtr) userdata;
  if (avp->field_list == NULL || avp->field_list->data.ptrvalue == NULL)
  {
    return;
  }
  
  /* don't apply to All Notes */
  if (avp->field_list->choice != 0)
  {
    return;
  }
  
  sqdp = (SourceQualDescPtr) avp->field_list->data.ptrvalue;
  
  is_nontext = IsNonTextModifier (sqdp->name);
  if (is_nontext && StringHasNoText (avp->new_text))
  {
    RemoveSourceQualFromBioSource (biop, avp, fsp);
    return;
  }
  
  if (!StringHasNoText (avp->text_to_replace))
  {
    found = TRUE;
  }
  
  if (sqdp->isOrgMod)
  {
    if (biop->org == NULL)
    {
      biop->org = OrgRefNew ();
    }
    if (biop->org != NULL && biop->org->orgname == NULL)
    {
      biop->org->orgname = OrgNameNew ();
    }
    if (biop->org != NULL && biop->org->orgname != NULL)
    {
      mod = biop->org->orgname->mod;
      while (mod != NULL)
      {
        if (mod->subtype == sqdp->subtype)
        {
          if (!is_nontext)
          {
            mod->subname = HandleApplyValue (mod->subname, avp); 
          }
          found = TRUE;
        }
        last_mod = mod;
        mod = mod->next;
      }
      if (!found)
      {
        mod = OrgModNew ();
        mod->subtype = sqdp->subtype;
        if (is_nontext)
        {
          mod->subname = StringSave ("");
        }
        else
        {
          mod->subname = StringSave (avp->new_text);
        }
        if (last_mod == NULL)
        {
          biop->org->orgname->mod = mod;
        }
        else
        {
          last_mod->next = mod;
        }
      }
    }
  }
  else
  {
    ssp = biop->subtype;
    while (ssp != NULL)
    {
      if (ssp->subtype == sqdp->subtype)
      {
        if (! is_nontext)
        {
          ssp->name = HandleApplyValue (ssp->name, avp);
        }
        found = TRUE;
      }
      last_ssp = ssp;
      ssp = ssp->next;
    }
    if (!found)
    {
      ssp = SubSourceNew ();
      ssp->subtype = sqdp->subtype;
      if (is_nontext)
      {
        ssp->name = StringSave ("");
      }
      else
      {
        ssp->name = StringSave (avp->new_text);
      }
      if (last_ssp == NULL)
      {
        biop->subtype = ssp;
      }
      else
      {
        last_ssp->next = ssp;
      }
    }
  }
}

static void ApplySourceQualFeatureCallback (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  if (sfp == NULL || sfp->data.choice != SEQFEAT_BIOSRC || userdata == NULL)
  {
    return;
  }
  ApplySourceQualBioSourceCallback (sfp->data.value.ptrvalue, userdata, fsp);
}

static void ApplySourceQualDescriptorCallback (SeqDescrPtr sdp, Pointer userdata, FilterSetPtr fsp)
{
  if (sdp == NULL || sdp->choice != Seq_descr_source || userdata == NULL)
  {
    return;
  }
  ApplySourceQualBioSourceCallback (sdp->data.ptrvalue, userdata, fsp);
}

static void RemoveOrgModFromBioSource (BioSourcePtr biop, Uint1 subtype, FilterSetPtr fsp)
{
  OrgModPtr mod = NULL, prev_mod = NULL, next_mod;
  
  if (biop != NULL && biop->org != NULL && biop->org->orgname != NULL)
  {
    mod = biop->org->orgname->mod;
    while (mod != NULL)
    {
      next_mod = mod->next;
      if (mod->subtype == subtype
          && OrgModSpecialMatch (mod, fsp))
      {
        if (prev_mod == NULL)
        {
          biop->org->orgname->mod = mod->next;
        }
        else
        {
          prev_mod->next = mod->next;
        }
        mod->next = NULL;
        OrgModFree (mod);
      }
      else
      {
        prev_mod = mod;
      }
      mod = mod->next;
    }
  }
}

static void RemoveSubSourceFromBioSource (BioSourcePtr biop, Uint2 subtype, FilterSetPtr fsp)
{
  SubSourcePtr ssp = NULL, prev_ssp = NULL, next_ssp;

  if (biop != NULL)
  {
    ssp = biop->subtype;
    while (ssp != NULL)
    { 
      next_ssp = ssp->next;
      if (ssp->subtype == subtype
          && SubSrcSpecialMatch (ssp, fsp))
      {
        if (prev_ssp == NULL)
        {
          biop->subtype = ssp->next;
        }
        else
        {
          prev_ssp->next = ssp->next;
        }
        ssp->next = NULL;
        SubSourceFree (ssp);
      }
      else
      {
        prev_ssp = ssp;
      }
      ssp = next_ssp;
    }
  }
}

static void RemoveSourceQualFromBioSource (BioSourcePtr biop, Pointer userdata, FilterSetPtr fsp)
{
  ApplyValuePtr         avp;
  SourceQualDescPtr     sqdp;
  
  if (biop == NULL || userdata == NULL)
  {
    return;
  }
  
  avp = (ApplyValuePtr) userdata;
  if (avp->field_list == NULL || avp->field_list->data.ptrvalue == NULL)
  {
    return;
  }
  if (avp->field_list->choice == 0)
  {
    sqdp = (SourceQualDescPtr) avp->field_list->data.ptrvalue;
    if (sqdp->isOrgMod)
    {
      RemoveOrgModFromBioSource (biop, sqdp->subtype, fsp);
    }
    else
    {
      RemoveSubSourceFromBioSource (biop, sqdp->subtype, fsp);
    }
  }
  else if (avp->field_list->choice == 1 
           && StringCmp (avp->field_list->data.ptrvalue, "All Notes") == 0)
  {
    RemoveOrgModFromBioSource (biop, 255, fsp);
    RemoveSubSourceFromBioSource (biop, 255, fsp);
  }
}

static void RemoveSourceQualFromSourceFeature (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  if (sfp == NULL || sfp->data.choice != SEQFEAT_BIOSRC || userdata == NULL)
  {
    return;
  }
  RemoveSourceQualFromBioSource (sfp->data.value.ptrvalue, userdata, fsp);
}

static void RemoveSourceQualFromSourceDescriptor (SeqDescrPtr sdp, Pointer userdata, FilterSetPtr fsp)
{
  if (sdp == NULL || sdp->choice != Seq_descr_source || userdata == NULL)
  {
    return;
  }
  RemoveSourceQualFromBioSource (sdp->data.ptrvalue, userdata, fsp);
}

static DialoG SourceLocationSelectionDialog 
(GrouP h,
 Boolean                  allow_multi,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata)

{
  return EnumAssocSelectionDialog (h, biosource_genome_alistX, "Location",
                                   allow_multi, change_notify, change_userdata);
}

static DialoG SourceOriginSelectionDialog
(GrouP h,
 Boolean                  allow_multi,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata)

{
  return EnumAssocSelectionDialog (h, origin_alist, "Origin",
                                   allow_multi, change_notify, change_userdata);
}

static DialoG SourceStringSelectionDialog
(GrouP h,
 Boolean                  allow_multi,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata)

{
  return EnumAssocSelectionDialog (h, orgref_textfield_alist, "string",
                                   allow_multi, change_notify, change_userdata);
}


static CharPtr 
GetStringFromStringDescriptor 
(SeqDescrPtr  sdp,
 ValNodePtr   vnp,
 FilterSetPtr fsp)
{
  if (sdp == NULL || vnp == NULL || sdp->choice != vnp->data.intvalue)
  {
    return NULL;
  }
  return StringSave (sdp->data.ptrvalue);
}

static void SetStringInStringDescriptor (SeqDescrPtr sdp, ValNodePtr vnp, ApplyValuePtr avp)
{
  if (sdp == NULL || vnp == NULL || sdp->choice != vnp->data.intvalue)
  {
    return;
  }
  sdp->data.ptrvalue = HandleApplyValue (sdp->data.ptrvalue, avp);
}

static void ApplyTitleDescriptorCallback (SeqDescrPtr sdp, Pointer userdata, FilterSetPtr fsp)
{
  ApplyValuePtr avp;
  ValNode       vn;
  
  if (sdp == NULL || sdp->choice != Seq_descr_title || userdata == NULL)
  {
    return;
  }
  
  avp = (ApplyValuePtr) userdata;
  
  vn.data.intvalue = Seq_descr_title;
  vn.next = NULL;
  
  SetStringInStringDescriptor (sdp, &vn, avp);
  
}


static DialoG GBQualSelectionDialog 
(GrouP                    h,
 Boolean                  allow_multi,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata)

{
  DialoG                    dlg;
  ValNodePtr            choice_name_list = NULL;
  Int4                      i;
     
  for (i = 0; i < NumEditQualifiers; i++)
  {
    ValNodeAddPointer (&choice_name_list, 0, EditQualifierList[i].name);
  }

  dlg = SelectionDialog (h, change_notify, change_userdata, 
                         allow_multi, "genbank qualifier",
                         choice_name_list, TALL_SELECTION_LIST);
  ValNodeFree (choice_name_list); 
  return dlg;
}

static CharPtr GetGBQualString (SeqFeatPtr sfp, ValNodePtr vnp, FilterSetPtr fsp)
{
  GBQualPtr gbqual;
  CharPtr   str = NULL;
  Int4      field_choice;
  
  if (sfp == NULL)
  {
    return NULL;
  }
  while (vnp != NULL && str == NULL)
  {
    field_choice = vnp->data.intvalue;
    if (StringICmp (EditQualifierList[field_choice - 1].name, "note") == 0)
    {
      if (!StringHasNoText (sfp->comment))
      {
        str = StringSave (sfp->comment);
      }
    }
    else if (StringICmp (EditQualifierList[field_choice - 1].name, "exception") == 0)
    {
      if (!StringHasNoText (sfp->except_text))
      {
        str = StringSave (sfp->except_text);
      }
    }
    else
    {
      gbqual = sfp->qual;
      while (gbqual != NULL && str == NULL)
      {
        if (field_choice > 0 && field_choice < NumEditQualifiers + 1
            && StringCmp (gbqual->qual, EditQualifierList[field_choice - 1].name) == 0
            && !StringHasNoText (gbqual->val))
        {
          str = StringSave (gbqual->val);
        }
        gbqual = gbqual->next;
      }
    }
    vnp = vnp->next;
  }
  return str;
}

static void SetGBQualString (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  GBQualPtr     gbqual, gbqual_last = NULL;
  ApplyValuePtr avp;
  Boolean       found = FALSE;
  
  if (sfp == NULL || userdata == NULL)
  {
    return;
  }
  avp = (ApplyValuePtr) userdata;
  if (avp->field_list == NULL 
      || avp->field_list->data.intvalue < 1 
      || avp->field_list->data.intvalue >= NumEditQualifiers + 1)
  {
    return;
  }

  if (StringICmp (EditQualifierList[avp->field_list->data.intvalue - 1].name, "note") == 0)
  {
    sfp->comment = HandleApplyValue (sfp->comment, avp);
    return;
  }
  else if (StringICmp (EditQualifierList[avp->field_list->data.intvalue - 1].name, "exception") == 0)
  {
    sfp->except_text = HandleApplyValue (sfp->except_text, avp);
    return;
  }

  if (!StringHasNoText (avp->text_to_replace))
  {
    found = TRUE;
  }
    
  gbqual = sfp->qual;
  while (gbqual != NULL)
  {
    gbqual_last = gbqual;
    if (StringCmp (gbqual->qual, EditQualifierList[avp->field_list->data.intvalue - 1].name) == 0)
    {
      gbqual->val = HandleApplyValue (gbqual->val, avp);
      found = TRUE;
    }
    gbqual = gbqual->next;
  }
  if (!found)
  {
    gbqual = GBQualNew ();
    if (gbqual != NULL)
    {
      gbqual->qual = StringSave (EditQualifierList[avp->field_list->data.intvalue - 1].name);
      gbqual->val = StringSave (avp->new_text);
      if (gbqual_last == NULL)
      {
        sfp->qual = gbqual;
      }
      else
      {
        gbqual_last->next = gbqual;
      }
    }
  }
}

static void RemoveGBQualField (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  GBQualPtr     gbqual, gbqual_last = NULL, gbqual_next;
  ApplyValuePtr avp;
  
  if (sfp == NULL || userdata == NULL)
  {
    return;
  }
  avp = (ApplyValuePtr) userdata;
  if (avp->field_list == NULL 
      || avp->field_list->data.intvalue < 1 
      || avp->field_list->data.intvalue >= NumEditQualifiers + 1)
  {
    return;
  }
  
  if (StringICmp (EditQualifierList[avp->field_list->data.intvalue - 1].name, "note") == 0)
  {
    sfp->comment = MemFree (sfp->comment);
    return;
  }
  else if (StringICmp (EditQualifierList[avp->field_list->data.intvalue - 1].name, "exception") == 0)
  {
    sfp->except_text = MemFree (sfp->except_text);
    return;
  }

  gbqual = sfp->qual;
  while (gbqual != NULL)
  {
    gbqual_next = gbqual->next;
    if (StringCmp (gbqual->qual, EditQualifierList[avp->field_list->data.intvalue - 1].name) == 0)
    {
      if (gbqual_last == NULL)
      {
        sfp->qual = gbqual->next;
      }
      else
      {
        gbqual_last->next = gbqual->next;
      }
      gbqual->next = NULL;
      GBQualFree (gbqual);
    }
    else
    {
      gbqual_last = gbqual;
    }
    gbqual = gbqual_next;
  }
}

static void SetFeatureNote (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  ApplyValuePtr avp;
  
  if (sfp == NULL || userdata == NULL)
  {
    return;
  }
  avp = (ApplyValuePtr) userdata;
  sfp->comment = HandleApplyValue (sfp->comment, avp);
}

static CharPtr GetFeatureNote (SeqFeatPtr sfp, ValNodePtr vnp, FilterSetPtr fsp)
{
  if (sfp == NULL || StringHasNoText (sfp->comment))
  {
    return NULL;
  }
  else
  {
    return StringSave (sfp->comment);
  }
}

static void RemoveFeatureNote (SeqFeatPtr sfp, Pointer userdata)
{
  if (sfp == NULL)
  {
    return;
  }
  sfp->comment = MemFree (sfp->comment);
}

static void ConvertFeatureFieldCallback (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  ConvertFieldPtr cfp;
  CharPtr         src_text;
  CharPtr         dst_text;
  ApplyValueData  avd;
  
  if (sfp == NULL || userdata == NULL)
  {
    return;
  }
  
  cfp = (ConvertFieldPtr) userdata;
  
  if (cfp->src_field_list == NULL || cfp->dst_field_list == NULL
      || cfp->get_str_func == NULL || cfp->set_str_func == NULL
      || cfp->remove_str_func == NULL)
  {
    return;
  }
  
  avd.text_to_replace = NULL;
  src_text = (cfp->get_str_func)(sfp, cfp->src_field_list, cfp->fsp);
  if (StringHasNoText (src_text))
  {
    src_text = MemFree (src_text);
    return;
  }
  
  if (cfp->convert_type == CONVERT_TYPE_SWAP)
  {
    /* for swap, we already know what we're doing with existing text - 
     * we're putting it where the new text used to be. */
    avd.etp = NULL;
    /* and we need to get the destination text before it's replaced */
    dst_text = (cfp->get_str_func)(sfp, cfp->dst_field_list, cfp->fsp);    
  }
  else
  {
    avd.etp = cfp->etp;
  }
    
  /* put src_text in the dst_field */
  avd.field_list = cfp->dst_field_list;
  avd.new_text = src_text;
  (cfp->set_str_func) (sfp, &avd, cfp->fsp);
  
  if (cfp->convert_type == CONVERT_TYPE_SWAP)
  {
    /* put dst_text in the src_field */
    avd.field_list = cfp->src_field_list;
    avd.new_text = dst_text;
    (cfp->set_str_func) (sfp, &avd, cfp->fsp);
  }
  else if (cfp->convert_type == CONVERT_TYPE_MOVE)
  {
    /* remove old src_field */
    avd.etp = NULL;
    avd.field_list = cfp->src_field_list;
    avd.new_text = NULL;
    (cfp->remove_str_func) (sfp, &avd, cfp->fsp);
  }
}

static void ConvertDescriptorFieldCallback (SeqDescrPtr sdp, Pointer userdata, FilterSetPtr fsp)
{
  ConvertFieldPtr cfp;
  CharPtr         src_text;
  CharPtr         dst_text;
  ApplyValueData  avd;
  
  if (sdp == NULL || userdata == NULL)
  {
    return;
  }
  
  cfp = (ConvertFieldPtr) userdata;
  
  if (cfp->src_field_list == NULL || cfp->dst_field_list == NULL
      || cfp->get_d_str_func == NULL || cfp->set_d_str_func == NULL
      || cfp->remove_d_str_func == NULL)
  {
    return;
  }
  
  avd.text_to_replace = NULL;
  src_text = (cfp->get_d_str_func)(sdp, cfp->src_field_list, cfp->fsp);
  if (StringHasNoText (src_text))
  {
    src_text = MemFree (src_text);
    return;
  }
  
  if (cfp->convert_type == CONVERT_TYPE_SWAP)
  {
    /* for swap, we already know what we're doing with existing text - 
     * we're putting it where the new text used to be. */
    avd.etp = NULL;
    /* and we need to get the destination text before it's replaced */
    dst_text = (cfp->get_d_str_func)(sdp, cfp->dst_field_list, cfp->fsp);    
  }
  else
  {
    avd.etp = cfp->etp;
  }
    
  /* put src_text in the dst_field */
  avd.field_list = cfp->dst_field_list;
  avd.new_text = src_text;
  (cfp->set_d_str_func) (sdp, &avd, cfp->fsp);
  
  if (cfp->convert_type == CONVERT_TYPE_SWAP)
  {
    /* put dst_text in the src_field */
    avd.field_list = cfp->src_field_list;
    avd.new_text = dst_text;
    (cfp->set_d_str_func) (sdp, &avd, cfp->fsp);
  }
  else if (cfp->convert_type == CONVERT_TYPE_MOVE)
  {
    /* remove old src_field */
    avd.etp = NULL;
    avd.field_list = cfp->src_field_list;
    avd.new_text = NULL;
    (cfp->remove_d_str_func) (sdp, &avd, cfp->fsp);
  }
}

static void ConvertNonTextFeatureFieldCallback (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  ConvertFieldPtr cfp;
  CharPtr         curr_val = NULL;
  CharPtr         src_val;
  ApplyValueData  avd;
  
  if (sfp == NULL || userdata == NULL)
  {
    return;
  }
  
  cfp = (ConvertFieldPtr) userdata;
  
  if (cfp->src_field_list == NULL || cfp->dst_field_list == NULL
      || cfp->get_str_func == NULL || cfp->set_str_func == NULL
      || cfp->name_field_func == NULL)
  {
    return;
  }
  
  curr_val = (cfp->get_str_func)(sfp, NULL, NULL);
  src_val = (cfp->name_field_func)(cfp->src_field_list);
  if (StringCmp (curr_val, src_val) == 0)
  {
    avd.field_list = cfp->dst_field_list;
    avd.etp = NULL;
    (cfp->set_str_func) (sfp, &avd, cfp->fsp);
  }
  curr_val = MemFree (curr_val);
  src_val = MemFree (src_val);
}

static void ConvertNonTextDescriptorFieldCallback (SeqDescrPtr sdp, Pointer userdata, FilterSetPtr fsp)
{
  ConvertFieldPtr cfp;
  CharPtr         curr_val = NULL;
  CharPtr         src_val;
  ApplyValueData  avd;
  
  if (sdp == NULL || userdata == NULL)
  {
    return;
  }
  
  cfp = (ConvertFieldPtr) userdata;
  
  if (cfp->src_field_list == NULL || cfp->dst_field_list == NULL
      || cfp->get_d_str_func == NULL || cfp->set_d_str_func == NULL
      || cfp->name_field_func == NULL)
  {
    return;
  }
  
  curr_val = (cfp->get_d_str_func)(sdp, NULL, NULL);
  src_val = (cfp->name_field_func)(cfp->src_field_list);
  if (StringCmp (curr_val, src_val) == 0)
  {
    avd.field_list = cfp->dst_field_list;
    avd.etp = NULL;
    (cfp->set_d_str_func) (sdp, &avd, cfp->fsp);
  }
  curr_val = MemFree (curr_val);
  src_val = MemFree (src_val);
}

#define FEATUREFIELD_NONE        0

typedef struct featurefieldselection 
{
  DIALOG_MESSAGE_BLOCK
  PopuP                    feature_field;

  Boolean                  allow_none;
  Int4                     num_fields;
  Nlm_ChangeNotifyProc     change_notify;
  Pointer                  change_userdata;
} FeatureFieldSelectionData, PNTR FeatureFieldSelectionPtr;


static void FeatureFieldSelectionChange (PopuP p)
{
  FeatureFieldSelectionPtr  dlg;

  dlg = (FeatureFieldSelectionPtr) GetObjectExtra (p);
  if (dlg == NULL)
  {
    return;
  }
  if (dlg->change_notify != NULL)
  {
    (dlg->change_notify) (dlg->change_userdata);
  }
}

static void ResetFeatureFieldSelection (FeatureFieldSelectionPtr dlg)
{
  if (dlg == NULL)
  {
    return;
  }
  SetValue (dlg->feature_field, 1);
  FeatureFieldSelectionChange (dlg->feature_field);
}

static void FeatureFieldToDialog (DialoG d, Pointer data)
{
  FeatureFieldSelectionPtr  dlg;
  ValNodePtr             vnp;
  Int4                   feature_field;

  dlg = (FeatureFieldSelectionPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return;
  }
  
  vnp = (ValNodePtr) data;
  if (vnp == NULL)
  {
    ResetFeatureFieldSelection (dlg);
  }
  else
  {
    feature_field = vnp->data.intvalue;
    if (feature_field < FEATUREFIELD_NONE || feature_field > dlg->num_fields + 1)
    {
      feature_field = FEATUREFIELD_NONE;
    }
    if (dlg->allow_none)
    {
      feature_field ++;
    }
    SetValue (dlg->feature_field, feature_field);
  }  
}

static Pointer DialogToFeatureField (DialoG d)
{
  FeatureFieldSelectionPtr  dlg;
  ValNodePtr             vnp = NULL;
  Int4                   feature_field;

  dlg = (FeatureFieldSelectionPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return NULL;
  }
  
  feature_field = GetValue (dlg->feature_field);
  if (dlg->allow_none)
  {
    feature_field --;
  }
  if (feature_field > FEATUREFIELD_NONE)
  {
    vnp = ValNodeNew (NULL);
    if (vnp != NULL)
    {
      vnp->data.intvalue = feature_field;
    }
  }
  return vnp;
}

static void FeatureFieldMessage (DialoG d, Int2 mssg)

{
  FeatureFieldSelectionPtr  dlg;

  dlg = (FeatureFieldSelectionPtr) GetObjectExtra (d);
  if (dlg != NULL) {
    switch (mssg) {
      case VIB_MSG_INIT :
        /* reset list */
        ResetFeatureFieldSelection (dlg);
        break;
      case VIB_MSG_ENTER :
        Select (dlg->feature_field);
        break;
      default :
        break;
    }
  }
}

static ValNodePtr TestFeatureFieldDialog (DialoG d)

{
  return NULL;
}

static DialoG FeatureFieldSelectionDialogEx 
(GrouP                    h,
 Boolean                  allow_none,
 CharPtr                  none_txt,
 Int4                     num_fields,
 CharPtr PNTR             field_names,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata)
{
  FeatureFieldSelectionPtr  dlg;
  GrouP                     p;
  Int4                      k;
  
  dlg = (FeatureFieldSelectionPtr) MemNew (sizeof (FeatureFieldSelectionData));
  if (dlg == NULL)
  {
    return NULL;
  }

  p = HiddenGroup (h, 1, 0, NULL);
  SetObjectExtra (p, dlg, StdCleanupExtraProc);

  dlg->dialog = (DialoG) p;
  dlg->todialog = FeatureFieldToDialog;
  dlg->fromdialog = DialogToFeatureField;
  dlg->dialogmessage = FeatureFieldMessage;
  dlg->testdialog = TestFeatureFieldDialog;
  dlg->allow_none = allow_none;
  dlg->num_fields = num_fields;
  dlg->change_notify = change_notify;
  dlg->change_userdata = change_userdata;

  dlg->feature_field = PopupList (p, TRUE, FeatureFieldSelectionChange);
  if (dlg->allow_none)
  {
    PopupItem (dlg->feature_field, none_txt);
  }
  for (k = 0; k < dlg->num_fields; k++)
  {
    PopupItem (dlg->feature_field, field_names[k]);
  }
  SetValue (dlg->feature_field, 1);
  SetObjectExtra (dlg->feature_field, dlg, NULL);
  return (DialoG) p;  
}

extern DialoG 
FeatureFieldSelectionDialog
(GrouP                    h,
 Boolean                  allow_none,
 Int4                     num_fields,
 CharPtr PNTR             field_names,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata)
{
  return FeatureFieldSelectionDialogEx (h, allow_none, "None", 
                                        num_fields, field_names, 
                                        change_notify, change_userdata);
}

static DialoG FeatureFieldSelectionDialogAny
(GrouP                    h,
 Boolean                  allow_none,
 Int4                     num_fields,
 CharPtr PNTR             field_names,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata)
{
  return FeatureFieldSelectionDialogEx (h, allow_none, "Any Field", 
                                        num_fields, field_names, 
                                        change_notify, change_userdata);
}


static CharPtr gene_field_list [] = 
{
  "locus", "description", "comment", "allele", "maploc", "locus_tag", "synonym"  
};

#define GENEFIELD_LOCUS       1
#define GENEFIELD_DESCRIPTION 2
#define GENEFIELD_COMMENT     3
#define GENEFIELD_ALLELE      4
#define GENEFIELD_MAPLOC      5
#define GENEFIELD_LOCUS_TAG   6
#define GENEFIELD_SYNONYM     7

static num_gene_fields = sizeof (gene_field_list) / sizeof (CharPtr);

extern DialoG 
GeneFieldSelectionDialog
(GrouP                    h,
 Boolean                  allow_none,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata)
{
  return FeatureFieldSelectionDialog (h, allow_none,
                                      num_gene_fields, gene_field_list, 
                                      change_notify, change_userdata);
}

extern CharPtr 
GetGeneFieldString 
(SeqFeatPtr   sfp,
 ValNodePtr   gene_field,
 FilterSetPtr fsp)
{
  ValNodePtr vnp;
  CharPtr    str = NULL;
  GeneRefPtr grp;
  
  if (sfp == NULL || gene_field == NULL || sfp->data.choice != SEQFEAT_GENE)
  {
    return NULL;
  }
  
  vnp = gene_field;
  grp = (GeneRefPtr) sfp->data.value.ptrvalue;
  while (vnp != NULL && vnp->data.intvalue != FEATUREFIELD_NONE && StringHasNoText (str))
  {
    str = NULL;
    switch (vnp->data.intvalue)
    {
      case GENEFIELD_LOCUS:
        if (grp != NULL)
        {
          str = grp->locus;
        }
        break;
      case GENEFIELD_DESCRIPTION:
        if (grp != NULL)
        {
          str = grp->desc;
        }
        break;
      case GENEFIELD_COMMENT:
        str = sfp->comment;
        break;
      case GENEFIELD_ALLELE:
        if (grp != NULL)
        {
          str = grp->allele;
        }
        break;
      case GENEFIELD_MAPLOC:
        if (grp != NULL)
        {
          str = grp->maploc;
        }
        break;
      case GENEFIELD_SYNONYM:
        if (grp != NULL && grp->syn != NULL)
        {
          str = grp->syn->data.ptrvalue;
        }
        break;
      case GENEFIELD_LOCUS_TAG:
        if (grp != NULL)
        {
          str = grp->locus_tag;
        }
        break;
    }
    vnp = vnp->next;
  }
  if (StringHasNoText (str))
  {
    str = NULL;
  }
  else
  {
    str = StringSave (str);
  }
  return str;
}

extern void RemoveGeneFieldString (SeqFeatPtr sfp, ValNodePtr gene_field)
{
  ValNodePtr vnp;
  Boolean    found_nonempty = FALSE;
  GeneRefPtr grp;
  ValNodePtr syn_remove;
  
  if (sfp == NULL || gene_field == NULL || sfp->data.choice != SEQFEAT_GENE)
  {
    return;
  }
  
  vnp = gene_field;
  grp = (GeneRefPtr) sfp->data.value.ptrvalue;
  while (vnp != NULL && vnp->data.intvalue != FEATUREFIELD_NONE && !found_nonempty)
  {
    switch (vnp->data.intvalue)
    {
      case GENEFIELD_LOCUS:
        if (grp != NULL && !StringHasNoText (grp->locus))
        {
          found_nonempty = TRUE;
          if (vnp->choice != 0)
          {
            grp->locus = MemFree (grp->locus);
          }
        }
        break;
      case GENEFIELD_DESCRIPTION:
        if (grp != NULL && !StringHasNoText (grp->desc))
        {
          found_nonempty = TRUE;
          if (vnp->choice != 0)
          {
            grp->desc = MemFree (grp->desc);
          }
        }
        break;
      case GENEFIELD_COMMENT:
        if (!StringHasNoText (sfp->comment))
        {
          found_nonempty = TRUE;
          if (vnp->choice != 0)
          {
            sfp->comment = MemFree (sfp->comment);
          }
        }
        break;
      case GENEFIELD_ALLELE:
        if (grp != NULL && !StringHasNoText (grp->allele))
        {
          found_nonempty = TRUE;
          if (vnp->choice != 0)
          {
            grp->allele = MemFree (grp->allele);  
          }
        }
        break;
      case GENEFIELD_MAPLOC:
        if (grp != NULL && !StringHasNoText (grp->maploc))
        {
          found_nonempty = TRUE;
          if (vnp->choice != 0)
          {
            grp->maploc = MemFree (grp->maploc);            
          }
        }
        break;
      case GENEFIELD_SYNONYM:
        if (grp != NULL && grp->syn != NULL && !StringHasNoText (grp->syn->data.ptrvalue))
        {
          found_nonempty = TRUE;
          if (vnp->choice != 0)
          {
            syn_remove = grp->syn;
            grp->syn = grp->syn->next;
            syn_remove->next = NULL;
            ValNodeFreeData (syn_remove);
          }
        }
        break;
      case GENEFIELD_LOCUS_TAG:
        if (grp != NULL && !StringHasNoText (grp->locus_tag))
        {
          found_nonempty = TRUE;
          if (vnp->choice != 0)
          {
            grp->locus_tag = MemFree (grp->locus_tag);
          }
        }
        break;
    }
    vnp = vnp->next;
  }
}

static void SetGeneFieldString (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  GeneRefPtr    grp;
  ApplyValuePtr avp;
  
  if (sfp == NULL || userdata == NULL || sfp->data.choice != SEQFEAT_GENE)
  {
    return;
  }
  avp = (ApplyValuePtr) userdata;
  if (avp->field_list == NULL)
  {
    return;
  }
  
  grp = (GeneRefPtr) sfp->data.value.ptrvalue;
  if (grp == NULL)
  {
    grp = GeneRefNew ();
    sfp->data.value.ptrvalue = grp;
    if (grp == NULL)
    {
      return;
    }
  }
  switch (avp->field_list->data.intvalue)
  {
    case GENEFIELD_LOCUS:
      grp->locus = HandleApplyValue (grp->locus, avp);
      break;
    case GENEFIELD_DESCRIPTION:
      grp->desc = HandleApplyValue (grp->desc, avp);
      break;
    case GENEFIELD_COMMENT:
      sfp->comment = HandleApplyValue (sfp->comment, avp);
      break;
    case GENEFIELD_ALLELE:
      grp->allele = HandleApplyValue (grp->allele, avp);  
      break;
    case GENEFIELD_MAPLOC:
      grp->maploc = HandleApplyValue (grp->maploc, avp);            
      break;
    case GENEFIELD_SYNONYM:
      grp->syn = ApplyValueToValNodeStringList (grp->syn, 0, avp);
      break;
    case GENEFIELD_LOCUS_TAG:
      grp->locus_tag = HandleApplyValue (grp->locus_tag, avp);
      break;
  }
}

static CharPtr mrna_field_list [] = 
{
  "product", "comment"  
};

#define MRNAFIELD_PRODUCT     1
#define MRNAFIELD_COMMENT     2

static num_mrna_fields = sizeof (mrna_field_list) / sizeof (CharPtr);

extern DialoG 
MRNAFieldSelectionDialog
(GrouP                    h,
 Boolean                  allow_none,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata)
{
  return FeatureFieldSelectionDialog (h, allow_none,
                                      num_mrna_fields, mrna_field_list, 
                                      change_notify, change_userdata);
}

extern CharPtr 
GetmRNAFieldString 
(SeqFeatPtr   sfp,
 ValNodePtr   mrna_field,
 FilterSetPtr fsp)
{
  ValNodePtr vnp;
  CharPtr    str = NULL;
  RnaRefPtr  rrp;
  
  if (sfp == NULL || mrna_field == NULL || sfp->idx.subtype != FEATDEF_mRNA)
  {
    return NULL;
  }
  
  vnp = mrna_field;
  rrp = (RnaRefPtr) sfp->data.value.ptrvalue;

  while (vnp != NULL && vnp->data.intvalue != FEATUREFIELD_NONE && StringHasNoText (str))
  {
    str = NULL;
    switch (vnp->data.intvalue)
    {
      case MRNAFIELD_PRODUCT:
        if (rrp != NULL)
        {
          str = rrp->ext.value.ptrvalue;
        }
        break;
      case MRNAFIELD_COMMENT:
        str = sfp->comment;
        break;
    }
    vnp = vnp->next;
  }
  if (StringHasNoText (str))
  {
    str = NULL;
  }
  else
  {
    str = StringSave (str);
  }
  return str;
}

extern void RemovemRNAFieldString (SeqFeatPtr sfp, ValNodePtr mrna_field)
{
  ValNodePtr vnp;
  Boolean    found_nonempty = FALSE;
  RnaRefPtr  rrp;
  
  if (sfp == NULL || mrna_field == NULL || sfp->idx.subtype != FEATDEF_mRNA)
  {
    return;
  }
  
  vnp = mrna_field;
  rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
  while (vnp != NULL && vnp->data.intvalue != FEATUREFIELD_NONE && !found_nonempty)
  {
    switch (vnp->data.intvalue)
    {
      case MRNAFIELD_PRODUCT:
        if (rrp != NULL && !StringHasNoText (rrp->ext.value.ptrvalue))
        {
          found_nonempty = TRUE;
          if (vnp->choice != 0)
          {
            rrp->ext.value.ptrvalue = MemFree (rrp->ext.value.ptrvalue);
          }
        }
        break;
      case MRNAFIELD_COMMENT:
        if (!StringHasNoText (sfp->comment))
        {
          found_nonempty = TRUE;
          if (vnp->choice != 0)
          {
            sfp->comment = MemFree (sfp->comment);
          }
        }
        break;
    }
    vnp = vnp->next;
  }
}

static void SetmRNAFieldString (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  RnaRefPtr     rrp;
  ApplyValuePtr avp;
  Int4          field_choice;
  
  if (sfp == NULL || userdata == NULL || sfp->idx.subtype != FEATDEF_mRNA)
  {
    return;
  }
  
  avp = (ApplyValuePtr) userdata;
  if (avp->field_list == NULL)
  {
    return;
  }
  
  rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
  if (rrp == NULL)
  {
    rrp = RnaRefNew ();
    if (rrp == NULL)
    {
      return;
    }
    sfp->data.value.ptrvalue = rrp;
  }
  field_choice = avp->field_list->data.intvalue;
  switch (avp->field_list->data.intvalue)
  {
    case MRNAFIELD_PRODUCT:
      rrp->ext.value.ptrvalue = HandleApplyValue (rrp->ext.value.ptrvalue, avp);
      break;
    case MRNAFIELD_COMMENT:
      sfp->comment = HandleApplyValue (sfp->comment,avp);
      break;
  }
}

static CharPtr GetCDSComment (SeqFeatPtr sfp, ValNodePtr vnp, FilterSetPtr fsp)
{
  if (sfp == NULL || sfp->data.choice != SEQFEAT_CDREGION || StringHasNoText (sfp->comment))
  {
    return NULL;
  }
  else
  {
    return StringSave (sfp->comment);
  }
}

static void SetCDSComment (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  ApplyValuePtr avp;

  if (sfp == NULL || userdata == NULL || sfp->data.choice != SEQFEAT_CDREGION)
  {
    return;
  }
  
  avp = (ApplyValuePtr) userdata;

  sfp->comment = HandleApplyValue (sfp->comment, avp);  
}

static void RemoveCDSComment (SeqFeatPtr sfp, ValNodePtr vnp)
{
  if (sfp == NULL || sfp->data.choice != SEQFEAT_CDREGION)
  {
    return;
  }
  sfp->comment = MemFree (sfp->comment);
}

#define PROTEINFIELD_NAME     1
#define PROTEINFIELD_DESC     2
#define PROTEINFIELD_EC_NUM   3
#define PROTEINFIELD_ACTIVITY 4
#define PROTEINFIELD_COMMENT  5

static CharPtr protein_field_list [] =
{
  "name", "description", "E.C. number", "activity", "comment"
};

static num_protein_fields = sizeof (protein_field_list) / sizeof (CharPtr);

extern DialoG 
ProteinFieldSelectionDialog
(GrouP                    h,
 Boolean                  allow_none,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata)
{
  return FeatureFieldSelectionDialog (h, allow_none,
                                      num_protein_fields, protein_field_list, 
                                      change_notify, change_userdata);
}

extern CharPtr GetProteinFieldString (SeqFeatPtr sfp, ValNodePtr protein_field, FilterSetPtr fsp)
{
  ValNodePtr vnp, val_vnp;
  CharPtr    str = NULL;
  ProtRefPtr prp;
  Int4       field_choice;
  
  if (sfp == NULL || protein_field == NULL || sfp->idx.subtype != FEATDEF_PROT)
  {
    return NULL;
  }
  
  vnp = protein_field;
  prp = (ProtRefPtr) sfp->data.value.ptrvalue;

  while (vnp != NULL && vnp->data.intvalue != FEATUREFIELD_NONE && StringHasNoText (str))
  {
    str = NULL;
    field_choice = vnp->data.intvalue;
    switch (field_choice)
    {
      case PROTEINFIELD_NAME:
        if (prp != NULL && prp->name != NULL)
        {
          val_vnp = prp->name;
          str = val_vnp->data.ptrvalue;
        }
        break;
      case PROTEINFIELD_DESC:
        if (prp != NULL)
        {
          str = prp->desc;
        }
        break;
      case PROTEINFIELD_EC_NUM:
        if (prp != NULL && prp->ec != NULL)
        {
          val_vnp = prp->ec;
          str = val_vnp->data.ptrvalue;
        }
        break;
      case PROTEINFIELD_ACTIVITY:
        if (prp != NULL && prp->activity != NULL)
        {
          val_vnp = prp->activity;
          str = val_vnp->data.ptrvalue;
        }
        break;
      case PROTEINFIELD_COMMENT:
        str = sfp->comment;
        break;
    }
    vnp = vnp->next;
  }
  if (StringHasNoText (str))
  {
    str = NULL;
  }
  else
  {
    str = StringSave (str);
  }
  return str;
}

static void RemoveProteinFieldString (SeqFeatPtr sfp, ValNodePtr protein_field)
{
  ValNodePtr vnp;
  ProtRefPtr prp;
  Int4       field_choice;
  
  if (sfp == NULL || protein_field == NULL || sfp->idx.subtype != FEATDEF_PROT)
  {
    return;
  }
  
  vnp = protein_field;
  prp = (ProtRefPtr) sfp->data.value.ptrvalue;

  while (vnp != NULL && vnp->data.intvalue != FEATUREFIELD_NONE)
  {
    field_choice = vnp->data.intvalue;
    switch (field_choice)
    {
      case PROTEINFIELD_NAME:
        if (prp != NULL && prp->name != NULL)
        {
          prp->name = ValNodeFreeData (prp->name);
        }
        break;
      case PROTEINFIELD_DESC:
        if (prp != NULL)
        {
          prp->desc = MemFree (prp->desc);
        }
        break;
      case PROTEINFIELD_EC_NUM:
        if (prp != NULL && prp->ec != NULL)
        {
          prp->ec = ValNodeFreeData (prp->ec);
        }
        break;
      case PROTEINFIELD_ACTIVITY:
        if (prp != NULL && prp->activity != NULL)
        {
          prp->activity = ValNodeFreeData (prp->activity);
        }
        break;
      case PROTEINFIELD_COMMENT:
        sfp->comment = MemFree (sfp->comment);
        break;
    }
    vnp = vnp->next;
  }
}

static void SetProteinFieldString (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  ProtRefPtr    prp;
  ApplyValuePtr avp;
  
  if (sfp == NULL || userdata == NULL || sfp->idx.subtype != FEATDEF_PROT)
  {
    return;
  }
  
  avp = (ApplyValuePtr) userdata;
  if (avp->field_list == NULL)
  {
    return;
  }
  
  prp = (ProtRefPtr) sfp->data.value.ptrvalue;
  if (prp == NULL)
  {
    prp = ProtRefNew ();
    if (prp == NULL)
    {
      return;
    }
    sfp->data.value.ptrvalue = prp;
  }
  switch (avp->field_list->data.intvalue)
  {
    case PROTEINFIELD_NAME:
      if (prp->name == NULL)
      {
        prp->name = ApplyValueToValNodeStringList (prp->name, 0, avp);
      }
      else
      {
        prp->name->data.ptrvalue = HandleApplyValue (prp->name->data.ptrvalue, avp);
      }
      break;
    case PROTEINFIELD_DESC:
      prp->desc = HandleExistingText (prp->desc, StringSave (avp->new_text), avp->etp);
      break;
    case PROTEINFIELD_EC_NUM:
      if (prp->ec == NULL)
      {
        prp->ec = ApplyValueToValNodeStringList (prp->ec, 0, avp);
      }
      else
      {
        prp->ec->data.ptrvalue = HandleApplyValue (prp->ec->data.ptrvalue, avp);
      }
      break;
    case PROTEINFIELD_ACTIVITY:
      if (prp->activity == NULL)
      {
        prp->activity = ApplyValueToValNodeStringList (prp->activity, 0, avp);
      }
      else
      {
        prp->activity->data.ptrvalue = HandleApplyValue (prp->activity->data.ptrvalue, avp);
      }
      break;
    case PROTEINFIELD_COMMENT:
      sfp->comment = HandleExistingText (sfp->comment, StringSave (avp->new_text), avp->etp);
      break;
  }
}

static CharPtr PNTR BuildCDSGeneFieldList (Int4Ptr num_fields)
{
  CharPtr PNTR field_name_list;
  Int4         i, k;
  Char         tmp[100];

  if (num_fields == NULL) 
  {
    return NULL;
  }
  *num_fields = 1 + num_gene_fields + num_mrna_fields + num_protein_fields;
  field_name_list = (CharPtr PNTR) MemNew (*num_fields * sizeof (CharPtr));
  if (field_name_list == NULL)
  {
    return NULL;
  }
  
  field_name_list [0] = StringSave ("CDS comment");
  k = 1;
  for (i = 0; i < num_gene_fields; i++)
  {
    sprintf (tmp, "Gene %s", gene_field_list [i]);
    field_name_list [k++] = StringSave (tmp);
  }
  for (i = 0; i < num_mrna_fields; i++)
  {
    sprintf (tmp, "mRNA %s", mrna_field_list [i]);
    field_name_list [k++] = StringSave (tmp);
  }
  for (i = 0; i < num_protein_fields; i++)
  {
    sprintf (tmp, "Protein %s", protein_field_list [i]);
    field_name_list [k++] = StringSave (tmp);
  }
  return field_name_list;  
}

static void FreeCDSGeneFieldList (CharPtr PNTR field_name_list, Int4 num_fields)
{
  Int4 i;
  
  if (field_name_list == NULL || num_fields == 0)
  {
    return;
  }
  
  for (i = 0; i < num_fields; i++)
  {
    field_name_list [i] = MemFree (field_name_list [i]);
  }
  field_name_list = MemFree (field_name_list);
}

extern DialoG 
CDSGeneProtFieldSelectionDialog
(GrouP                    h,
 Boolean                  allow_none,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata)
{
  CharPtr PNTR field_name_list;
  Int4         num_fields = 0;
  DialoG       d;

  field_name_list = BuildCDSGeneFieldList (&num_fields);
  
  d = FeatureFieldSelectionDialog (h, allow_none,
                                      num_fields, field_name_list, 
                                      change_notify, change_userdata);
  FreeCDSGeneFieldList (field_name_list, num_fields);
  return d;
}

static DialoG
CDSGeneProtFieldConstraintSelectionDialog
(GrouP                    h,
 Boolean                  allow_none,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata)
{
  CharPtr PNTR field_name_list;
  Int4         num_fields = 0;
  DialoG       d;

  field_name_list = BuildCDSGeneFieldList (&num_fields);
  
  d = FeatureFieldSelectionDialogAny (h, allow_none,
                                      num_fields, field_name_list, 
                                      change_notify, change_userdata);
  FreeCDSGeneFieldList (field_name_list, num_fields);
  return d; 
}

extern CharPtr GetCDSGeneProtField (SeqFeatPtr sfp, ValNodePtr vnp, FilterSetPtr fsp)
{
  CharPtr str = NULL;
  ValNode vn;
  Int4    field_choice;
  
  if (sfp == NULL || vnp == NULL)
  {
    return NULL;
  }
  
  while (vnp != NULL && str == NULL)
  {
    vn.choice = vnp->choice;
    vn.next = NULL;
    
    field_choice = vnp->data.intvalue;
  
    if (vnp->data.intvalue == 1)
    {
      /* CDS Comment */
      str = GetCDSComment (sfp, NULL, NULL);
    }
    else if (vnp->data.intvalue <= 1 + num_gene_fields)
    {
      vn.data.intvalue = vnp->data.intvalue - 1;
      str = GetGeneFieldString (sfp, &vn, NULL);
    }
    else if (vnp->data.intvalue <= 1 + num_gene_fields + num_mrna_fields)
    {
      vn.data.intvalue = vnp->data.intvalue - num_gene_fields - 1;
      str = GetmRNAFieldString (sfp, &vn, NULL);
    }
    else if (vnp->data.intvalue <= 1 + num_gene_fields + num_mrna_fields + num_protein_fields)
    {
      vn.data.intvalue = vnp->data.intvalue - num_gene_fields - num_mrna_fields - 1;
      str = GetProteinFieldString (sfp, &vn, NULL);
    }
    vnp = vnp->next;
  }
  return str;
}

static void RemoveCDSGeneProtField (SeqFeatPtr sfp, ValNodePtr vnp)
{
  ValNode vn;
  
  if (sfp == NULL || vnp == NULL)
  {
    return;
  }
  
  while (vnp != NULL)
  {
    vn.choice = vnp->choice;
    vn.next = NULL;
  
    if (vnp->data.intvalue == 1)
    {
      /* CDS Comment */
      RemoveCDSComment (sfp, NULL);
    }
    else if (vnp->data.intvalue <= 1 + num_gene_fields)
    {
      vn.data.intvalue = vnp->data.intvalue - 1;
      RemoveGeneFieldString (sfp, &vn);
    }
    else if (vnp->data.intvalue <= 1 + num_gene_fields + num_mrna_fields)
    {
      vn.data.intvalue = vnp->data.intvalue - num_gene_fields - 1;
      RemovemRNAFieldString (sfp, &vn);
    }
    else if (vnp->data.intvalue <= 1 + num_gene_fields + num_mrna_fields + num_protein_fields)
    {
      vn.data.intvalue = vnp->data.intvalue - num_gene_fields - num_mrna_fields - 1;
      RemoveProteinFieldString (sfp, &vn);
    }
    vnp = vnp->next;
  }
}

static void 
SetCDSGeneProtField 
(SeqFeatPtr      sfp,
 ValNodePtr      vnp, 
 CharPtr         new_text, 
 ExistingTextPtr etp)
{
  ValNode vn;
  ApplyValueData avd;
  
  if (sfp == NULL || vnp == NULL)
  {
    return;
  }
  
  vn.choice = vnp->choice;
  vn.next = NULL;
  avd.field_list = &vn;
  avd.new_text = StringSave (new_text);
  avd.etp = etp;
  avd.text_to_replace = NULL;

  if (vnp->data.intvalue == 1)
  {
    /* CDS Comment */
    SetCDSComment (sfp, &avd, NULL);
  }
  else if (vnp->data.intvalue <= 1 + num_gene_fields)
  {
    vn.data.intvalue = vnp->data.intvalue - 1;
    SetGeneFieldString (sfp, &avd, NULL);
  }
  else if (vnp->data.intvalue <= 1 + num_gene_fields + num_mrna_fields)
  {
    vn.data.intvalue = vnp->data.intvalue - num_gene_fields - 1;
    SetmRNAFieldString (sfp, &avd, NULL);
  }
  else if (vnp->data.intvalue <= 1 + num_gene_fields + num_mrna_fields + num_protein_fields)
  {
    vn.data.intvalue = vnp->data.intvalue - num_gene_fields - num_mrna_fields - 1;
    SetProteinFieldString (sfp, &avd, NULL);
  }
  
  avd.new_text = MemFree (avd.new_text);
}

#define RNAFIELD_NAME              1
#define RNAFIELD_COMMENT           2
#define RNAFIELD_PRODUCT           3
#define RNAFIELD_CODONS_RECOGNIZED 4
#define RNAFIELD_ANTICODON         5
#define RNAFIELD_TRANSCRIPT_ID     6

static CharPtr rna_field_list [] = 
{
  "Name", "Comment", "Product GBQual", "Codons Recognized", "Anticodon", "Transcript ID"
};

static Int4 num_rna_fields = sizeof (rna_field_list) / sizeof (CharPtr);

extern DialoG
RNAAddFieldSelectionDialog
(GrouP                    h,
 Boolean                  allow_none,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata)
{
  return FeatureFieldSelectionDialog (h, allow_none,
                                      4, rna_field_list, 
                                      change_notify, change_userdata);

}

extern DialoG
RNARemoveFieldSelectionDialog
(GrouP                    h,
 Boolean                  allow_none,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata)
{
  return FeatureFieldSelectionDialog (h, allow_none,
                                      num_rna_fields, rna_field_list, 
                                      change_notify, change_userdata);

}

extern DialoG
RNAFieldSelectionDialog
(GrouP                    h,
 Boolean                  allow_none,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata)
{
  return FeatureFieldSelectionDialog (h, allow_none,
                                      3, rna_field_list, 
                                      change_notify, change_userdata);

}

static CharPtr rna_subtype_list [] = { "misc_RNA", "preRna", "mRNA", "tRNA",
                                        "rRNA", "snRNA", "scRNA", "snoRNA",
                                        "unknown" };
static Int2    num_rna_subtypes = sizeof (rna_subtype_list) / sizeof (CharPtr); 

static CharPtr GetRNASubtypeName (ValNodePtr vnp)
{
  CharPtr label = NULL;
  
  if (vnp != NULL && vnp->choice >= 0 && vnp->choice < num_rna_subtypes)
  {
    label = StringSave (rna_subtype_list[vnp->choice]);
  }
  return label;
}

static DialoG RNASubtypeSelectionDialog
(GrouP                    h,
 Boolean                  allow_multi,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata)
{
  ValNodePtr choice_list = NULL;
  DialoG     dlg;
  
  ValNodeAddInt (&choice_list, 0, FEATDEF_otherRNA);
  ValNodeAddInt (&choice_list, 1, FEATDEF_preRNA);
  ValNodeAddInt (&choice_list, 2, FEATDEF_mRNA);
  ValNodeAddInt (&choice_list, 3, FEATDEF_tRNA);
  ValNodeAddInt (&choice_list, 4, FEATDEF_rRNA);
  ValNodeAddInt (&choice_list, 5, FEATDEF_snRNA);
  ValNodeAddInt (&choice_list, 6, FEATDEF_scRNA);
  ValNodeAddInt (&choice_list, 7, FEATDEF_snoRNA);
  ValNodeAddInt (&choice_list, 8, FEATDEF_misc_RNA);

  dlg = ValNodeSelectionDialog (h, choice_list, TALL_SELECTION_LIST,
                                GetRNASubtypeName, NULL, 
                                IntValNodeCopy, IntValNodeMatch,
                                "RNA subtype", change_notify, change_userdata, 
                                allow_multi);
  return dlg;
}

extern CharPtr GetRNAFieldString (SeqFeatPtr sfp, ValNodePtr vnp, FilterSetPtr fsp)
{
  RnaRefPtr rrp;
  CharPtr   str = NULL;
  GBQualPtr gbqual;
  
  if (sfp == NULL || sfp->data.choice != SEQFEAT_RNA || vnp == NULL)
  {
    return NULL;
  }
  
  rrp = sfp->data.value.ptrvalue;
  
  while (vnp != NULL && str == NULL)
  {
    switch (vnp->data.intvalue)
    {
      case RNAFIELD_NAME :
        if (rrp != NULL && rrp->ext.choice == 1 && !StringHasNoText (rrp->ext.value.ptrvalue))
        {
          str = StringSave (rrp->ext.value.ptrvalue);        
        }
        break;
      case RNAFIELD_COMMENT :
        if (!StringHasNoText (sfp->comment))
        {
          str = StringSave (sfp->comment);
        }
        break;
      case RNAFIELD_PRODUCT :
        gbqual = sfp->qual;
        while (gbqual != NULL && str == NULL) {
          if (StringICmp (gbqual->qual, "product") == 0
              && !StringHasNoText (gbqual->val)) 
          {
            str = StringSave (gbqual->val);
          }
          gbqual = gbqual->next;
        }
        break;      
    }
    vnp = vnp->next;
  }
  return str;
}

static void SetRNAFieldString (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  RnaRefPtr     rrp;
  ApplyValuePtr avp;
  GBQualPtr     gbqual, gbqual_last = NULL;
  
  if (sfp == NULL || sfp->data.choice != SEQFEAT_RNA || userdata == NULL)
  {
    return;
  }
  
  avp = (ApplyValuePtr) userdata;
  if (avp->field_list == NULL)
  {
    return;
  }

  rrp = sfp->data.value.ptrvalue;
    
  switch (avp->field_list->data.intvalue)
  {
    case RNAFIELD_NAME :
      if (rrp != NULL)
      {
        if (rrp->ext.choice == 0)
        {
          rrp->ext.choice = 1;
          rrp->ext.value.ptrvalue = NULL;
        }
        if (rrp->ext.choice == 1)
        {
          rrp->ext.value.ptrvalue = HandleApplyValue (rrp->ext.value.ptrvalue, avp);        
        }
      }
      break;
    case RNAFIELD_COMMENT :
      sfp->comment = HandleApplyValue (sfp->comment, avp);
      break;
    case RNAFIELD_PRODUCT :
      gbqual = sfp->qual;
      while (gbqual != NULL && StringICmp (gbqual->qual, "product") != 0)
      {
        gbqual_last = gbqual;
        gbqual = gbqual->next;
      }
      if (gbqual == NULL)
      {
        if (StringHasNoText (avp->text_to_replace))
        {
          gbqual = GBQualNew ();
          if (gbqual == NULL)
          {
            return;
          }
          gbqual->qual = StringSave ("product");
          gbqual->val = StringSave (avp->new_text);
          if (gbqual_last == NULL)
          {
            sfp->qual = gbqual;
          }
          else
          {
            gbqual->next = gbqual_last->next;
            gbqual_last->next = gbqual;
          }
        }
        else
        {
          /* don't create new product for edit/replace */
        }
      }
      else
      {        
        gbqual->val = HandleApplyValue (gbqual->val, avp);
      }
      break;      
  }
}

static void RemoveRNAField (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  RnaRefPtr     rrp;
  GBQualPtr     gbqual, prev_qual = NULL, next_qual;
  tRNAPtr       trp;
  ApplyValuePtr avp;
  
  if (sfp == NULL || sfp->data.choice != SEQFEAT_RNA || userdata == NULL)
  {
    return;
  }
  
  avp = (ApplyValuePtr) userdata;
  if (avp->field_list == NULL)
  {
    return;
  }
  
  rrp = sfp->data.value.ptrvalue;

  switch (avp->field_list->data.intvalue)
  {
    case RNAFIELD_NAME :
      if (rrp != NULL && rrp->ext.choice == 1)
      {
        rrp->ext.value.ptrvalue = MemFree (rrp->ext.value.ptrvalue);  
      }
      break;
    case RNAFIELD_COMMENT :
      sfp->comment = MemFree (sfp->comment);
      break;
    case RNAFIELD_PRODUCT :
      gbqual = sfp->qual;
      while (gbqual != NULL) 
      {
        next_qual = gbqual->next;
        if (StringICmp (gbqual->qual, "product") == 0)
        {
          if (prev_qual == NULL)
          {
            sfp->qual = sfp->qual->next;
          }
          else
          {
            prev_qual->next = gbqual->next;
          }
          gbqual->next = NULL;
          GBQualFree (gbqual);
        }
        else
        {
          prev_qual = gbqual;
        }
        gbqual = next_qual;
      }
      break;    
    case RNAFIELD_ANTICODON :
      if (rrp->ext.choice == 2) {
        trp = (tRNAPtr) rrp->ext.value.ptrvalue;
        if (trp != NULL && trp->anticodon != NULL) {
          trp->anticodon = SeqLocFree (trp->anticodon);
        }
      }
      break;
    case RNAFIELD_CODONS_RECOGNIZED :
      if (rrp->ext.choice == 2) {
        trp = (tRNAPtr) rrp->ext.value.ptrvalue;
        if (trp != NULL) {
          trp->codon [0] = 255;
          trp->codon [1] = 255;
          trp->codon [2] = 255;
          trp->codon [3] = 255;
          trp->codon [4] = 255;
          trp->codon [5] = 255;
        }
      }
      break;
    case RNAFIELD_TRANSCRIPT_ID :
      sfp->product = SeqLocFree (sfp->product);
      break;
  }
}

typedef struct exonfieldselection 
{
  DIALOG_MESSAGE_BLOCK
  PopuP                    exon_field;

  Boolean                  allow_none;
  Nlm_ChangeNotifyProc     change_notify;
  Pointer                  change_userdata;  
} ExonFieldSelectionData, PNTR ExonFieldSelectionPtr;

#define EXONFIELD_ALLELE            1
#define EXONFIELD_COMMENT           2
#define EXONFIELD_EC_NUMBER         3
#define EXONFIELD_FUNCTION          4
#define EXONFIELD_OLD_LOCUS_TAG     5
#define EXONFIELD_NUMBER            6
#define EXONFIELD_PRODUCT           7

static CharPtr exon_field_list [] = 
{
  "allele", "comment", "EC_number", "function", "old_locus_tag", "number", "product"  
};
#define NUM_EXON_FIELDS 7

extern DialoG
ExonFieldSelectionDialog
(GrouP                    h,
 Boolean                  allow_none,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata)
{
  return FeatureFieldSelectionDialog (h, allow_none,
                                      NUM_EXON_FIELDS, exon_field_list, 
                                      change_notify, change_userdata);

}

extern CharPtr GetExonFieldString (SeqFeatPtr sfp, ValNodePtr exon_field)
{
  ValNodePtr vnp;
  CharPtr    str = NULL;
  GBQualPtr  gbqual;
  
  if (sfp == NULL || exon_field == NULL || sfp->idx.subtype != FEATDEF_exon)
  {
    return NULL;
  }
  
  vnp = exon_field;
  while (vnp != NULL && StringHasNoText (str))
  {
    str = NULL;
    if (vnp->data.intvalue == EXONFIELD_COMMENT)
    {
      str = sfp->comment;
    }
    else
    {
      gbqual = sfp->qual;
      while (gbqual != NULL && StringHasNoText (str))
      {
        if (vnp->data.intvalue < NUM_EXON_FIELDS + 2
            && vnp->data.intvalue >= 2
            && StringCmp (gbqual->qual, exon_field_list[vnp->data.intvalue - 2]) == 0)
        {
          str = gbqual->val;
        }
        gbqual = gbqual->next;
      }    
    }
    vnp = vnp->next;
  }
  if (StringHasNoText (str))
  {
    str = NULL;
  }
  else
  {
    str = StringSave (str);
  }
  return str;
}

extern void RemoveExonFieldString (SeqFeatPtr sfp, ValNodePtr exon_field)
{
  ValNodePtr vnp;
  Boolean    found_nonempty = FALSE;
  GBQualPtr  gbqual, prev_qual;
  
  if (sfp == NULL || exon_field == NULL || sfp->idx.subtype != FEATDEF_exon)
  {
    return;
  }
  
  vnp = exon_field;
  while (vnp != NULL && !found_nonempty)
  {
    if (vnp->data.intvalue == EXONFIELD_COMMENT)
    {
      if (!StringHasNoText (sfp->comment))
      {
        found_nonempty = TRUE;
        if (vnp->choice != 0)
        {
          sfp->comment = MemFree (sfp->comment);
        }
      }
    }
    else
    {     
      gbqual = sfp->qual;
      prev_qual = NULL;
      while (gbqual != NULL)
      {
        if (vnp->data.intvalue < NUM_EXON_FIELDS + 2
            && vnp->data.intvalue >= 2
            && StringCmp (gbqual->qual, exon_field_list[vnp->data.intvalue - 2]) == 0)
        {
           if (!StringHasNoText (gbqual->val))
           {
            found_nonempty = TRUE;
            if (vnp->choice != 0)
            {
              if (prev_qual == NULL)
              {
                sfp->qual = gbqual->next;
                gbqual->next = NULL;
                GBQualFree (gbqual);
                return;
              }
            }
          }
        }
        gbqual = gbqual->next;
      }
    }
    vnp = vnp->next;
  }
}

static void SetExonFieldString (SeqFeatPtr sfp, Pointer userdata)
{
  ApplyValuePtr avp;
  GBQualPtr  gbqual, prev_qual = NULL;
  Boolean       found = FALSE;
  
  if (sfp == NULL || userdata == NULL || sfp->idx.subtype != FEATDEF_exon)
  {
    return;
  }
  
  avp = (ApplyValuePtr) userdata;
  if (avp->field_list == NULL 
      || avp->field_list->data.intvalue >= NUM_EXON_FIELDS + 2
      || avp->field_list->data.intvalue < 2)
  {
    return;
  }
  
  if (!StringHasNoText (avp->text_to_replace))
  {
    found = TRUE;
  }
  
  gbqual = sfp->qual;
  while (gbqual != NULL)
  {
    if (StringCmp (gbqual->qual, 
                   exon_field_list[avp->field_list->data.intvalue - 2]) == 0)
    {
      gbqual->val = HandleApplyValue (gbqual->val, avp);
      found = TRUE;
    }
    prev_qual = gbqual;
    gbqual = gbqual->next;
  }
  if (!found)
  {
    gbqual = GBQualNew ();
    gbqual->qual = StringSave (exon_field_list[avp->field_list->data.intvalue - 2]);
    gbqual->val = StringSave (avp->new_text);
    if (prev_qual == NULL)
    {
      sfp->qual = gbqual;
    }
    else
    {
      prev_qual->next = gbqual;
    }
  }
}

#define PARSE_FIELD_BIOSRC_TAXNAME  1
#define PARSE_FIELD_BIOSRC_LINEAGE  2
#define PARSE_FIELD_BIOSRC_DIVISION 3

static CharPtr biosrc_string_list [] =
{
  "Organism Name", "Lineage", "Division"
};

static num_biosrc_strings = sizeof (biosrc_string_list) / sizeof (CharPtr);

extern DialoG BioSourceStringDialog 
(GrouP                    h,
 Boolean                  allow_multi,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata)
{
  return FeatureFieldSelectionDialog (h, allow_multi,
                                      num_biosrc_strings, biosrc_string_list, 
                                      change_notify, change_userdata);
}

static CharPtr GetSourceStringFromBioSource (BioSourcePtr biop, ValNodePtr vnp)
{
  if (biop == NULL || vnp == NULL || biop->org == NULL)
  {
    return NULL;
  }
  
  if (vnp->data.intvalue == PARSE_FIELD_BIOSRC_TAXNAME
      && !StringHasNoText (biop->org->taxname))
  {
    return StringSave (biop->org->taxname);
  }
  else if (vnp->data.intvalue == PARSE_FIELD_BIOSRC_LINEAGE
           && biop->org->orgname != NULL
           && !StringHasNoText (biop->org->orgname->lineage))
  {
    return StringSave (biop->org->orgname->lineage);
  }
  else if (vnp->data.intvalue == PARSE_FIELD_BIOSRC_DIVISION
           && biop->org->orgname != NULL
           && !StringHasNoText (biop->org->orgname->div))
  {
    return StringSave (biop->org->orgname->div);
  }
  else
  {
    return NULL;
  }
}

static CharPtr 
GetSourceFeatureString 
(SeqFeatPtr   sfp,
 ValNodePtr   vnp,
 FilterSetPtr fsp)
{
  if (sfp == NULL || sfp->data.choice != SEQFEAT_BIOSRC || vnp == NULL)
  {
    return NULL;
  }
  
  return GetSourceStringFromBioSource (sfp->data.value.ptrvalue, vnp);
}

static CharPtr 
GetSourceDescriptorString 
(SeqDescrPtr  sdp,
 ValNodePtr   vnp,
 FilterSetPtr fsp)
{
  if (sdp == NULL || sdp->choice != Seq_descr_source || vnp == NULL)
  {
    return NULL;
  }
  return GetSourceStringFromBioSource (sdp->data.ptrvalue, vnp);
}

static void ApplySourceStringBioSourceCallback (BioSourcePtr biop, Pointer userdata)
{
  ApplyValuePtr avp;
  
  if (biop == NULL || userdata == NULL)
  {
    return;
  }
  
  avp = (ApplyValuePtr) userdata;
  if (avp->field_list != NULL && !StringHasNoText (avp->new_text))
  {
    if (biop->org == NULL)
    {
      biop->org = OrgRefNew();
    }
    if (avp->field_list->data.intvalue == PARSE_FIELD_BIOSRC_TAXNAME)
    {
      biop->org->taxname = HandleApplyValue (biop->org->taxname, avp);
    }
    else if (avp->field_list->data.intvalue == PARSE_FIELD_BIOSRC_LINEAGE)
    {
      if (biop->org->orgname == NULL)
      {
        biop->org->orgname = OrgNameNew ();
      }
      biop->org->orgname->lineage = HandleApplyValue (biop->org->orgname->lineage, avp);
    }
    else if (avp->field_list->data.intvalue == PARSE_FIELD_BIOSRC_DIVISION)
    {
      if (biop->org->orgname == NULL)
      {
        biop->org->orgname = OrgNameNew ();
      }
      biop->org->orgname->div = HandleApplyValue (biop->org->orgname->div, avp);
    }
  }
  
}

static void ApplySourceStringFeatureCallback (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  if (sfp == NULL || sfp->data.choice != SEQFEAT_BIOSRC || userdata == NULL)
  {
    return;
  }
  ApplySourceStringBioSourceCallback (sfp->data.value.ptrvalue, userdata);
}

static void ApplySourceStringDescriptorCallback (SeqDescrPtr sdp, Pointer userdata, FilterSetPtr fsp)
{
  if (sdp == NULL || sdp->choice != Seq_descr_source || userdata == NULL)
  {
    return;
  }
  ApplySourceStringBioSourceCallback (sdp->data.ptrvalue, userdata);
}


#define TEXT_PORTION_START_AFTER 1
#define TEXT_PORTION_START_WITH  2
#define TEXT_PORTION_END_AFTER   1
#define TEXT_PORTION_END_WITH    2

typedef struct textportiondialog
{
  DIALOG_MESSAGE_BLOCK

  GrouP  start_choice;
  TexT   start_text;
  GrouP  end_choice;
  TexT   end_text;
  ButtoN insensitive;
  ButtoN whole_word;
} TextPortionDialogData, PNTR TextPortionDialogPtr;

static void ResetTextPortionDialog (TextPortionDialogPtr tp)
{
  if (tp == NULL)
  {
    return;
  }
  SetValue (tp->start_choice, TEXT_PORTION_START_AFTER);
  SetTitle (tp->start_text, "");
  SetValue (tp->end_choice, TEXT_PORTION_END_AFTER);
  SetTitle (tp->end_text, "");
  SetStatus (tp->insensitive, FALSE);
  SetStatus (tp->whole_word, FALSE);
}

static void TextPortionToDialog (DialoG d, Pointer data)
{
  TextPortionDialogPtr tdlg;
  TextPortionPtr       tdata;
  Int4                 start_choice, end_choice;
  
  tdlg = (TextPortionDialogPtr) GetObjectExtra (d);
  if (tdlg == NULL)
  {
    return;
  }
  tdata = (TextPortionPtr) data;
  ResetTextPortionDialog (tdlg);  
  if (tdata != NULL)
  {
    start_choice = tdata->end_choice;
    end_choice = tdata->end_choice;
    if (start_choice < TEXT_PORTION_START_AFTER || start_choice > TEXT_PORTION_START_WITH)
    {
      start_choice = TEXT_PORTION_START_AFTER;
    }
    if (end_choice < TEXT_PORTION_END_AFTER || end_choice > TEXT_PORTION_END_WITH)
    {
      end_choice = TEXT_PORTION_END_AFTER;
    }
    SetValue (tdlg->start_choice, start_choice);
    SetValue (tdlg->end_choice, end_choice);
    if (tdata->start_text != NULL)
    {
      SetTitle (tdlg->start_text, tdata->start_text);
    }
    if (tdata->end_text != NULL)
    {
      SetTitle (tdlg->end_text, tdata->end_text);
    }
    SetStatus (tdlg->insensitive, tdata->insensitive);
    SetStatus (tdlg->whole_word, tdata->whole_word);
  }
}

static Pointer DialogToTextPortion (DialoG d)
{
  TextPortionDialogPtr tdlg;
  TextPortionPtr       tdata;

  tdlg = (TextPortionDialogPtr) GetObjectExtra (d);
  if (tdlg == NULL)
  {
    return NULL;
  }

  tdata = (TextPortionPtr) MemNew (sizeof (TextPortionData));
  if (tdata != NULL)
  {
    tdata->start_choice = GetValue (tdlg->start_choice);
    tdata->end_choice = GetValue (tdlg->end_choice);
    tdata->start_text = JustSaveStringFromText (tdlg->start_text);
    tdata->end_text = JustSaveStringFromText (tdlg->end_text);
    tdata->insensitive = GetStatus (tdlg->insensitive);
    tdata->whole_word = GetStatus (tdlg->whole_word);
  }
  return tdata;
}

static void TextPortionMessage (DialoG d, Int2 mssg)

{
  TextPortionDialogPtr tp;

  tp = (TextPortionDialogPtr) GetObjectExtra (d);
  if (tp != NULL) {
    switch (mssg) {
      case VIB_MSG_INIT :
        ResetTextPortionDialog (tp);        
        break;
      case VIB_MSG_ENTER :
        Select (tp->start_text);
        break;
      default :
        break;
    }
  }
}

static ValNodePtr TestTextPortionDialog (DialoG d)

{
  return NULL;
}

extern DialoG TextPortionDialog (GrouP h)
{
  TextPortionDialogPtr tp;
  GrouP                p, g1, g2;
  
  tp = (TextPortionDialogPtr) MemNew (sizeof (TextPortionDialogData));
  if (tp == NULL)
  {
    return NULL;
  }
  
  p = HiddenGroup (h, -1, 0, NULL);
  SetObjectExtra (p, tp, StdCleanupExtraProc);

  tp->dialog = (DialoG) p;
  tp->todialog = TextPortionToDialog;
  tp->fromdialog = DialogToTextPortion;
  tp->dialogmessage = TextPortionMessage;
  tp->testdialog = TestTextPortionDialog;

  g1 = HiddenGroup (p, 2, 0, NULL);
  SetGroupSpacing (g1, 10, 10);

  tp->start_choice = HiddenGroup (g1, 2, 0, NULL);
  RadioButton (tp->start_choice, "just after");
  RadioButton (tp->start_choice, "starting at");
  SetValue (tp->start_choice, TEXT_PORTION_START_AFTER);

  tp->start_text = DialogText (g1, "", 10, NULL);
  
  tp->end_choice = HiddenGroup (g1, 2, 0, NULL);
  RadioButton (tp->end_choice, "up to");
  RadioButton (tp->end_choice, "including");
  SetValue (tp->end_choice, TEXT_PORTION_END_AFTER);
    
  tp->end_text = DialogText (g1, "", 10, NULL);
  
  g2 = HiddenGroup (p, 2, 0, NULL);
  tp->insensitive = CheckBox (g2, "Case insensitive", NULL);
  tp->whole_word = CheckBox (g2, "Whole word", NULL);
  
  AlignObjects (ALIGN_CENTER, (HANDLE) g1, (HANDLE) g2, NULL);
  
  ResetTextPortionDialog (tp);
  return (DialoG) p;
}

extern TextPortionPtr TextPortionFree (TextPortionPtr tp)
{
  if (tp == NULL) return NULL;
  tp->start_text = MemFree (tp->start_text);
  tp->end_text = MemFree (tp->end_text);
  tp = MemFree (tp->end_text);
  return tp;
}

static Boolean IsWholeWordMatch (CharPtr start, CharPtr found, Int4 match_len)
{
  Boolean rval = TRUE;
  Char    char_after;
  Char    char_before;
  
  if (match_len == 0)
  {
    rval = TRUE;
  }
  else if (start == NULL || found == NULL)
  {
    rval = FALSE;
  }
  else
  {
	  char_after = *(found + match_len);
    if (found != start)
	  {
	    char_before = *(found - 1);
	    if (isalpha ((Int4) char_before) || isdigit ((Int4) char_before))
	    {
	      rval = FALSE;
	    }
	  }
	  if (char_after != 0 && (isalpha ((Int4) char_after) || isdigit ((Int4)char_after)))
	  {
	    rval = FALSE;
	  }   
  }
  return rval;
}

extern void 
FindTextPortionInString 
(CharPtr        str, 
 TextPortionPtr tp, 
 CharPtr PNTR   ploc, 
 Int4Ptr        plen)
{
  CharPtr found_start, found_end;
  Int4    found_len;
  
  if (ploc == NULL || plen == NULL || tp == NULL)
  {
    return;
  }
  *ploc = NULL;
  *plen = 0;

  if (str == NULL)
  {
    return;
  }
  
  if (tp->start_text == NULL || tp->start_text [0] == 0)
  {
    found_start = str;
  }
  else
  {
    if (tp->insensitive)
    {
      found_start = StringISearch (str, tp->start_text);
    }
    else
    {
      found_start = StringSearch (str, tp->start_text);
    }
    
    if (tp->whole_word && ! IsWholeWordMatch (str, found_start, StringLen (tp->start_text)))
    {
      found_start = NULL;
    }
  }
  
  if (found_start == NULL)
  {
    return;
  }
  

  
  if (tp->start_choice == TEXT_PORTION_START_AFTER)
  {
    found_start += StringLen (tp->start_text);
  }
  
  if (tp->end_text == NULL || tp->end_text [0] == 0)
  {
    found_len = StringLen (found_start);
  }
  else
  {
    if (tp->insensitive)
    {
      found_end = StringISearch (found_start, tp->end_text);
    }
    else
    {
      found_end = StringSearch (found_start, tp->end_text);
    }
    if (tp->whole_word && ! IsWholeWordMatch (str, found_end, StringLen (tp->end_text)))
    {
      found_end = NULL;
    }    
    
    if (found_end == NULL)
    {
      found_len = 0;
    }
    else if (tp->end_choice == TEXT_PORTION_END_WITH)
    {
      found_len = (Int4)(found_end - found_start) + StringLen (tp->end_text);
    }
    else
    {
      found_len = found_end - found_start;
    }
  }

  if (found_len > 0)
  {
    *ploc = found_start;
    *plen = found_len;
  }
}

static void ClearDialogBtn (ButtoN b)
{
  DialoG d;
  
  d = (DialoG) GetObjectExtra (b);
  
  PointerToDialog (d, NULL);
}

typedef struct stringconstraintdialog 
{
  DIALOG_MESSAGE_BLOCK
  PopuP  match_choice;
  TexT   match_text;
  ButtoN insensitive;
  ButtoN whole_word;   
} StringConstraintDialogData, PNTR StringConstraintDialogPtr;

static void ResetStringConstraintDialog (StringConstraintDialogPtr scdp)
{
  if (scdp == NULL) return;
  
  SetValue (scdp->match_choice, 1);
  SetTitle (scdp->match_text, " ");
  SetStatus (scdp->insensitive, FALSE);
  SetStatus (scdp->whole_word, FALSE);
}

static void StringConstraintToDialog (DialoG d, Pointer data)

{
  StringConstraintDialogPtr scdp;
  StringConstraintPtr       scp;
  Int4                      match_location;

  scdp = (StringConstraintDialogPtr) GetObjectExtra (d);
  scp = (StringConstraintPtr) data;
  if (scdp == NULL)
  {
    return;
  }

  if (scp == NULL)
  {
    ResetStringConstraintDialog (scdp);
  }
  else
  {
    match_location = scp->match_location;
    if (match_location < STRING_CONSTRAINT_CONTAINS || match_location > STRING_CONSTRAINT_ENDS)
    {
      match_location = STRING_CONSTRAINT_CONTAINS;
    }
    
    if (scp->not_present)
    {
      match_location += STRING_CONSTRAINT_ENDS;
    }

    SetValue (scdp->match_choice, match_location);
    
    if (StringHasNoText (scp->match_text))
    {
      SetTitle (scdp->match_text, " ");
    }
    else
    {
      SetTitle (scdp->match_text, scp->match_text);
    }
    
    SetStatus (scdp->insensitive, scp->insensitive);
    SetStatus (scdp->whole_word, scp->whole_word);
  }
}

static Pointer DialogToStringConstraint (DialoG d)

{
  StringConstraintDialogPtr scdp;
  StringConstraintPtr       scp;
  Int4                      match_choice;

  scdp = (StringConstraintDialogPtr) GetObjectExtra (d);
  if (scdp == NULL)
  {
    return NULL;
  }
  scp = (StringConstraintPtr) MemNew (sizeof (StringConstraintData));
  if (scp != NULL)
  {
    scp->match_text = SaveStringFromText (scdp->match_text);
    scp->insensitive = GetStatus (scdp->insensitive);
    scp->whole_word = GetStatus (scdp->whole_word);
    match_choice = GetValue (scdp->match_choice);
    switch (match_choice)
    {
      case 1:
        scp->match_location = STRING_CONSTRAINT_CONTAINS;
        scp->not_present = FALSE;
        break;
      case 2:
        scp->match_location = STRING_CONSTRAINT_CONTAINS;
        scp->not_present = TRUE;
        break;
      case 3:
        scp->match_location = STRING_CONSTRAINT_STARTS;
        scp->not_present = FALSE;
        break;
      case 4:
        scp->match_location = STRING_CONSTRAINT_ENDS;
        scp->not_present = FALSE;
        break;
      case 5:
        scp->match_location = STRING_CONSTRAINT_STARTS;
        scp->not_present = TRUE;
        break;
      case 6:
        scp->match_location = STRING_CONSTRAINT_ENDS;
        scp->not_present = TRUE;
        break;
      default:
        scp->match_location = STRING_CONSTRAINT_CONTAINS;
        scp->not_present = FALSE;
        break;
    }
  }
  return scp;
}

static void StringConstraintMessage (DialoG d, Int2 mssg)

{
  StringConstraintDialogPtr scdp;

  scdp = (StringConstraintDialogPtr) GetObjectExtra (d);
  if (scdp != NULL) {
    switch (mssg) {
      case VIB_MSG_INIT :
        ResetStringConstraintDialog (scdp);        
        break;
      case VIB_MSG_ENTER :
        Select (scdp->match_text);
        break;
      default :
        break;
    }
  }
}

static ValNodePtr TestStringConstraintDialog (DialoG d)

{
  return NULL;
}

extern DialoG StringConstraintDialog (GrouP h, CharPtr label, Boolean clear_btn)

{
  StringConstraintDialogPtr scdp;
  GrouP                     p, g, k;
  ButtoN                    b = NULL;
  
  scdp = (StringConstraintDialogPtr) MemNew (sizeof (StringConstraintDialogData));
  if (scdp == NULL)
  {
    return NULL;
  }

  p = HiddenGroup (h, -1, 0, NULL);
  SetObjectExtra (p, scdp, StdCleanupExtraProc);

  scdp->dialog = (DialoG) p;
  scdp->todialog = StringConstraintToDialog;
  scdp->fromdialog = DialogToStringConstraint;
  scdp->dialogmessage = StringConstraintMessage;
  scdp->testdialog = TestStringConstraintDialog;

  g = HiddenGroup (p, 3, 0, NULL);
  SetGroupSpacing (g, 10, 10);
  
  if (!StringHasNoText (label))
  {
    StaticPrompt (g, label, 0, dialogTextHeight, systemFont, 'l');
  }
  
  scdp->match_choice = PopupList (g, TRUE, NULL);
  PopupItem (scdp->match_choice, "Contains");
  PopupItem (scdp->match_choice, "Does not contain");
  PopupItem (scdp->match_choice, "Starts with");
  PopupItem (scdp->match_choice, "Ends with");
  PopupItem (scdp->match_choice, "Does not start with");
  PopupItem (scdp->match_choice, "Does not end with");
  SetValue (scdp->match_choice, 1);
  scdp->match_text = DialogText (g, "", 15, NULL);
  
  k = HiddenGroup (p, 3, 0, NULL);
  SetGroupSpacing (k, 10, 10);
  scdp->insensitive = CheckBox (k, "Case Insensitive", NULL);
  scdp->whole_word = CheckBox (k, "Whole Word", NULL);
  
  if (clear_btn)
  {
    b = PushButton (p, "Clear Constraint", ClearDialogBtn);
    SetObjectExtra (b, p, NULL);
  }

  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) k, (HANDLE) b, NULL);
    
  return (DialoG) p;
}

typedef struct choiceconstraintdialog 
{
  DIALOG_MESSAGE_BLOCK
  GrouP               constraint_type;
  DialoG              present_qual_choice;
  DialoG              string_qual_choice;
  DialoG              string_constraint;
  FreeValNodeProc     free_vn_proc;
  CopyValNodeDataProc copy_vn_proc;
} ChoiceConstraintDialogData, PNTR ChoiceConstraintDialogPtr;

static void ChangeChoiceConstraintType (GrouP p)
{
  ChoiceConstraintDialogPtr scdp;
  Int4                      constraint_type;

  scdp = (ChoiceConstraintDialogPtr) GetObjectExtra (p);
  if (scdp == NULL)
  {
    return;
  }
  
  constraint_type = GetValue (scdp->constraint_type);
  if (constraint_type == CHOICE_CONSTRAINT_QUAL_PRESENT)
  {
    Enable (scdp->present_qual_choice);
    Disable (scdp->string_qual_choice);
    Disable (scdp->string_constraint);
  }
  else if (constraint_type == CHOICE_CONSTRAINT_STRING)
  {
    Enable (scdp->string_qual_choice);
    Enable (scdp->string_constraint);
    Disable (scdp->present_qual_choice);
  }
  else
  {
    Disable (scdp->present_qual_choice);
    Disable (scdp->string_qual_choice);
    Disable (scdp->string_constraint);
  }
}

static void ResetChoiceConstraintDialog (ChoiceConstraintDialogPtr scdp)
{
  if (scdp != NULL)  
  {
    SetValue (scdp->constraint_type, CHOICE_CONSTRAINT_ANY);
    PointerToDialog (scdp->present_qual_choice, NULL);
    PointerToDialog (scdp->string_qual_choice, NULL);
    PointerToDialog (scdp->string_constraint, NULL);
    ChangeChoiceConstraintType (scdp->constraint_type);
  }
}

static void ChoiceConstraintToDialog (DialoG d, Pointer data)
{
  ChoiceConstraintDialogPtr src_dlg;
  ChoiceConstraintPtr       src_data;
  Int4 constraint_type;
  
  src_dlg = (ChoiceConstraintDialogPtr) GetObjectExtra (d);
  if (src_dlg == NULL)
  {
    return;
  }
  src_data = (ChoiceConstraintPtr) data;
  if (src_data == NULL)
  {
    ResetChoiceConstraintDialog (src_dlg);
  }
  else
  {
    constraint_type = src_data->constraint_type;
    if (constraint_type < CHOICE_CONSTRAINT_ANY || constraint_type > CHOICE_CONSTRAINT_STRING)
    {
      constraint_type = CHOICE_CONSTRAINT_ANY;
    }
    SetValue (src_dlg->constraint_type, constraint_type);
    if (constraint_type == CHOICE_CONSTRAINT_QUAL_PRESENT)
    {
      PointerToDialog (src_dlg->present_qual_choice, src_data->qual_choice);
    }
    else if (constraint_type == CHOICE_CONSTRAINT_STRING)
    {
      PointerToDialog (src_dlg->string_qual_choice, src_data->qual_choice);
      PointerToDialog (src_dlg->string_constraint, src_data->string_constraint);
    }
    ChangeChoiceConstraintType (src_dlg->constraint_type);
  }
}

static Pointer DialogToChoiceConstraint (DialoG d)
{
  ChoiceConstraintDialogPtr src_dlg;
  ChoiceConstraintPtr       src_data;

  src_dlg = (ChoiceConstraintDialogPtr) GetObjectExtra (d);
  if (src_dlg == NULL) 
  {
    return NULL;
  }
  
  src_data = (ChoiceConstraintPtr) MemNew (sizeof (ChoiceConstraintData));
  if (src_data != NULL)
  {
    src_data->constraint_type = GetValue (src_dlg->constraint_type);
    if (src_data->constraint_type == CHOICE_CONSTRAINT_QUAL_PRESENT)
    {
      src_data->qual_choice = DialogToPointer (src_dlg->present_qual_choice);
    }
    else if (src_data->constraint_type == CHOICE_CONSTRAINT_STRING)
    {
      src_data->qual_choice = DialogToPointer (src_dlg->string_qual_choice);
      src_data->string_constraint = DialogToPointer (src_dlg->string_constraint);
    }
    src_data->free_vn_proc = src_dlg->free_vn_proc;
    src_data->copy_vn_proc = src_dlg->copy_vn_proc;
  }
  return src_data;
}

static void ChoiceConstraintMessage (DialoG d, Int2 mssg)

{
  ChoiceConstraintDialogPtr scdp;

  scdp = (ChoiceConstraintDialogPtr) GetObjectExtra (d);
  if (scdp != NULL) {
    switch (mssg) {
      case VIB_MSG_INIT :
        ResetChoiceConstraintDialog (scdp);        
        break;
      case VIB_MSG_ENTER :
        Select (scdp->constraint_type);
        break;
      default :
        break;
    }
  }
}

static ValNodePtr TestChoiceConstraintDialog (DialoG d)

{
  return NULL;
}

extern DialoG 
ConstraintChoiceDialog 
(GrouP                     h, 
 FeatureFieldSelectionProc func_present,
 FeatureFieldSelectionProc func_string,
 FreeValNodeProc           free_vn_proc,
 CopyValNodeDataProc       copy_vn_proc,
 CharPtr                   any_name,
 CharPtr                   present_name,
 Boolean                   clear_btn)
{
  ChoiceConstraintDialogPtr scdp;
  GrouP                     p, q;
  ButtoN                    b = NULL;
  
  scdp = (ChoiceConstraintDialogPtr) MemNew (sizeof (ChoiceConstraintDialogData));
  if (scdp == NULL)
  {
    return NULL;
  }

  p = HiddenGroup (h, -1, 0, NULL);
  SetObjectExtra (p, scdp, StdCleanupExtraProc);
  SetGroupSpacing (p, 10, 10);

  scdp->dialog = (DialoG) p;
  scdp->todialog = ChoiceConstraintToDialog;
  scdp->fromdialog = DialogToChoiceConstraint;
  scdp->dialogmessage = ChoiceConstraintMessage;
  scdp->testdialog = TestChoiceConstraintDialog;

  scdp->constraint_type = HiddenGroup (p, 2, 0, ChangeChoiceConstraintType);
  RadioButton (scdp->constraint_type, any_name);
  StaticPrompt (scdp->constraint_type, "", 0, dialogTextHeight, systemFont, 'l');  
  
  RadioButton (scdp->constraint_type, present_name);
  scdp->present_qual_choice = func_present (scdp->constraint_type, FALSE, NULL, NULL);
    
  RadioButton (scdp->constraint_type, "When");
  q = HiddenGroup (scdp->constraint_type, 2, 0, NULL);
  scdp->string_qual_choice = func_string (q, TRUE, NULL, NULL);
  scdp->string_constraint = StringConstraintDialog (q, NULL, FALSE);
  
  SetValue (scdp->constraint_type, CHOICE_CONSTRAINT_ANY);
  SetObjectExtra (scdp->constraint_type, scdp, NULL);

  Disable (scdp->present_qual_choice);
  Disable (scdp->string_qual_choice);
  Disable (scdp->string_constraint);
  
  scdp->free_vn_proc = free_vn_proc;
  scdp->copy_vn_proc = copy_vn_proc;
  
  if (clear_btn)
  {
    b = PushButton (p, "Clear Constraint", ClearDialogBtn);
    SetObjectExtra (b, p, NULL);    
  }
  
  AlignObjects (ALIGN_CENTER, (HANDLE) scdp->constraint_type, (HANDLE) b, NULL);
   
  return (DialoG) p;
}

extern DialoG SourceConstraintDialog (GrouP h, Boolean clear_btn)
{
  return ConstraintChoiceDialog (h, SourceQualTypeConstraintSelectionDialog, 
                                 SourceStringConstraintSelectionDialog,
                                 ValNodeSimpleDataFree,
                                 SourceQualValNodeDataCopy,
                                 "For any source", "When qualifier present:",
                                 clear_btn);
}

static ValNodePtr CDSGeneProtValNodeDataCopy (ValNodePtr vnp)
{
  ValNodePtr                  vnp_copy = NULL;
  
  if (vnp != NULL)
  {
    ValNodeAddInt (&vnp_copy, vnp->choice, vnp->data.intvalue);
  }
  return vnp_copy;
}

extern DialoG CDSGeneProtConstraintDialog (GrouP h, Boolean clear_btn)
{
  return ConstraintChoiceDialog (h, CDSGeneProtFieldSelectionDialog,
                                 CDSGeneProtFieldConstraintSelectionDialog,
                                 NULL, IntValNodeCopy,
                                 "For any CDS-Gene-Prot-mRNA set", 
                                 "When field present:", 
                                 clear_btn);
}

typedef struct locationconstraintdialog 
{
  DIALOG_MESSAGE_BLOCK
  PopuP  interval_end_choice;
  PopuP  interval_match_choice;
  PopuP  endpoint_match_choice;
  TexT   only_val;
  TexT   first_val;
  TexT   second_val;
  PrompT second_val_prompt;
  PopuP  strand;
  PopuP  sequence_type;
  Boolean show_interval_controls;
} LocationConstraintDialogData, PNTR LocationConstraintDialogPtr;

static void ShowLocationChoiceControls (PopuP p)

{
  LocationConstraintDialogPtr lcdp;
  Int4                        match_choice;

  lcdp = (LocationConstraintDialogPtr) GetObjectExtra (p);
  if (lcdp == NULL || lcdp->interval_end_choice == NULL) return;
  
  if (GetValue (lcdp->interval_end_choice) == LOCATION_CONSTRAINT_WHOLE_INTERVAL)
  {
    Show (lcdp->interval_match_choice);
    Hide (lcdp->endpoint_match_choice);
    if (p == lcdp->interval_end_choice)
    {
      SetValue (lcdp->interval_match_choice, GetValue (lcdp->endpoint_match_choice)); 
    }
    match_choice = GetValue (lcdp->interval_match_choice);
  }
  else
  {
    Hide (lcdp->interval_match_choice);
    Show (lcdp->endpoint_match_choice);
    if (p == lcdp->interval_end_choice)
    {
      SetValue (lcdp->endpoint_match_choice, GetValue (lcdp->interval_match_choice)); 
    }
    match_choice = GetValue (lcdp->endpoint_match_choice);
  }
  
  switch (match_choice)
  {
    case LOCATION_CONSTRAINT_ANY :
      Hide (lcdp->only_val);
      Hide (lcdp->first_val);
      Hide (lcdp->second_val_prompt);
      Hide (lcdp->second_val);
      break;
    case LOCATION_CONSTRAINT_UPSTREAM :
    case LOCATION_CONSTRAINT_DOWNSTREAM :
      Show (lcdp->only_val);
      Hide (lcdp->first_val);
      Hide (lcdp->second_val_prompt);
      Hide (lcdp->second_val);
      break;
    case LOCATION_CONSTRAINT_CONTAINED :
    case LOCATION_CONSTRAINT_NOT_IN :
    case LOCATION_CONSTRAINT_OVERLAP :
    case LOCATION_CONSTRAINT_EQUAL :
      Hide (lcdp->only_val);
      Show (lcdp->first_val);
      Show (lcdp->second_val_prompt);
      Show (lcdp->second_val);
      break;
  }
}

static void ResetLocationConstraintDialog (LocationConstraintDialogPtr lcdp)
{
  if (lcdp == NULL) return;
  
  SafeSetValue (lcdp->interval_end_choice, LOCATION_CONSTRAINT_WHOLE_INTERVAL);
  SafeSetValue (lcdp->interval_match_choice, LOCATION_CONSTRAINT_ANY);
  SafeSetTitle (lcdp->only_val, "");
  SafeSetTitle (lcdp->first_val, "");
  SafeSetTitle (lcdp->second_val, "");
  SetValue (lcdp->strand, LOCATION_CONSTRAINT_ANY_STRAND);
  SetValue (lcdp->sequence_type, LOCATION_CONSTRAINT_ANY_SEQ);
  ShowLocationChoiceControls (lcdp->interval_end_choice);
}

static void LocationConstraintToDialog (DialoG d, Pointer data)

{
  LocationConstraintDialogPtr lcdp;
  LocationConstraintPtr       lcp;
  Int4                        interval_end_choice;
  Int4                        match_choice;
  Char                        tmp [15];
  Int4                        strand;
  Int4                        sequence_type;

  lcdp = (LocationConstraintDialogPtr) GetObjectExtra (d);
  if (lcdp == NULL)
  {
    return;
  }
  lcp = (LocationConstraintPtr) data;
  
  ResetLocationConstraintDialog (lcdp);
  if (lcp != NULL)
  {
    interval_end_choice = lcp->interval_end_choice;
    if (interval_end_choice < LOCATION_CONSTRAINT_WHOLE_INTERVAL
        || interval_end_choice > LOCATION_CONSTRAINT_STOP_ENDPOINT)
    {
      interval_end_choice = LOCATION_CONSTRAINT_WHOLE_INTERVAL;
    }
    match_choice = lcp->match_choice;
    if (match_choice < LOCATION_CONSTRAINT_ANY)
    {
      match_choice = LOCATION_CONSTRAINT_ANY;
    }
    else if (match_choice > LOCATION_CONSTRAINT_EQUAL)
    {
      match_choice = LOCATION_CONSTRAINT_EQUAL;
    }
    if (match_choice > LOCATION_CONSTRAINT_NOT_IN && interval_end_choice != LOCATION_CONSTRAINT_WHOLE_INTERVAL)
    {
      interval_end_choice = LOCATION_CONSTRAINT_WHOLE_INTERVAL;
    }
    
    SetValue (lcdp->interval_end_choice, interval_end_choice);
    if (interval_end_choice == LOCATION_CONSTRAINT_WHOLE_INTERVAL)
    {
      SetValue (lcdp->interval_match_choice, match_choice);
    }
    else
    {
      SetValue (lcdp->endpoint_match_choice, match_choice);
    }
    switch (match_choice)
    {
      case LOCATION_CONSTRAINT_ANY :
        break;
      case LOCATION_CONSTRAINT_UPSTREAM :
      case LOCATION_CONSTRAINT_DOWNSTREAM :
        if (lcp->left >= 0)
        {
          sprintf (tmp, "%d", lcp->left + 1);
          SetTitle (lcdp->only_val, tmp);
        }
        break;
      case LOCATION_CONSTRAINT_CONTAINED :
      case LOCATION_CONSTRAINT_NOT_IN :
      case LOCATION_CONSTRAINT_OVERLAP :
      case LOCATION_CONSTRAINT_EQUAL :
        if (lcp->left >= 0)
        {
          sprintf (tmp, "%d", lcp->left + 1);
          SetTitle (lcdp->first_val, tmp);
        }
        if (lcp->right >= 0)
        {
          sprintf (tmp, "%d", lcp->right + 1);
          SetTitle (lcdp->second_val, tmp);
        }
        break;
    }
    strand = lcp->strand;
    if (strand < LOCATION_CONSTRAINT_ANY_STRAND || strand > LOCATION_CONSTRAINT_MINUS_STRAND)
    {
      strand = LOCATION_CONSTRAINT_ANY_STRAND;
    }
    SetValue (lcdp->strand, strand);
    sequence_type = lcp->sequence_type;
    if (sequence_type < LOCATION_CONSTRAINT_ANY_SEQ || sequence_type > LOCATION_CONSTRAINT_PROT_SEQ)
    {
      sequence_type = LOCATION_CONSTRAINT_ANY_SEQ;
    }
    SetValue (lcdp->sequence_type, LOCATION_CONSTRAINT_ANY_SEQ);
  }
  ShowLocationChoiceControls (lcdp->interval_match_choice);
}

static Pointer DialogToLocationConstraint (DialoG d)

{
  LocationConstraintDialogPtr lcdp;
  LocationConstraintPtr       lcp;
  CharPtr                     tmp;

  lcdp = (LocationConstraintDialogPtr) GetObjectExtra (d);
  if (lcdp == NULL)
  {
    return NULL;
  }
  lcp = (LocationConstraintPtr) MemNew (sizeof (LocationConstraintData));
  if (lcp != NULL)
  {
    if (lcdp->show_interval_controls)
    {
      lcp->interval_end_choice = GetValue (lcdp->interval_end_choice);
      if (lcp->interval_end_choice == LOCATION_CONSTRAINT_WHOLE_INTERVAL)
      {
        lcp->match_choice = GetValue (lcdp->interval_match_choice);
      }
      else
      {
        lcp->match_choice = GetValue (lcdp->endpoint_match_choice);
      }
      switch (lcp->match_choice)
      {
        case LOCATION_CONSTRAINT_ANY :
          break;
        case LOCATION_CONSTRAINT_UPSTREAM :
        case LOCATION_CONSTRAINT_DOWNSTREAM :
          tmp = SaveStringFromText (lcdp->only_val);
          if (StringHasNoText (tmp))
          {
            lcp->left = 0;
          }
          else
          {
            lcp->left = atoi (tmp) - 1;
          }
          tmp = MemFree (tmp);
          break;
        case LOCATION_CONSTRAINT_CONTAINED :
        case LOCATION_CONSTRAINT_NOT_IN :
        case LOCATION_CONSTRAINT_OVERLAP :
        case LOCATION_CONSTRAINT_EQUAL :
          tmp = SaveStringFromText (lcdp->first_val);
          if (StringHasNoText (tmp))
          {
            lcp->left = 0;
          }
          else
          {
            lcp->left = atoi (tmp) - 1;
          }
          tmp = MemFree (tmp);
          tmp = SaveStringFromText (lcdp->second_val);
          if (StringHasNoText (tmp))
          {
            lcp->right = 0;
          }
          else
          {
            lcp->right = atoi (tmp) - 1;
          }
          tmp = MemFree (tmp);
          break;
      }
    }
    else
    {
      lcp->match_choice = LOCATION_CONSTRAINT_ANY;
      lcp->left = 0;
      lcp->right = 0;
    }
    lcp->strand = GetValue (lcdp->strand);
    lcp->sequence_type = GetValue (lcdp->sequence_type);
  }
  return lcp;
}

static void LocationConstraintMessage (DialoG d, Int2 mssg)

{
  LocationConstraintDialogPtr lcdp;

  lcdp = (LocationConstraintDialogPtr) GetObjectExtra (d);
  if (lcdp != NULL) {
    switch (mssg) {
      case VIB_MSG_INIT :
        ResetLocationConstraintDialog (lcdp);        
        break;
      case VIB_MSG_ENTER :
        if (lcdp->show_interval_controls)
        {
          Select (lcdp->interval_end_choice);
        }
        else
        {
          Select (lcdp->strand);
        }
        break;
      default :
        break;
    }
  }
}

static ValNodePtr TestLocationConstraintDialog (DialoG d)

{
  return NULL;
}


extern DialoG 
LocationConstraintDialog 
(GrouP   h,
 Boolean show_interval_controls, 
 Boolean clear_btn)

{
  LocationConstraintDialogPtr lcdp;
  GrouP                       p, g, k, val_grp, strand_grp, g2, g3, g4;
  ButtoN                      b = NULL;
  
  lcdp = (LocationConstraintDialogPtr) MemNew (sizeof (LocationConstraintDialogData));
  if (lcdp == NULL)
  {
    return NULL;
  }

  p = HiddenGroup (h, -1, 0, NULL);
  SetObjectExtra (p, lcdp, StdCleanupExtraProc);

  lcdp->dialog = (DialoG) p;
  lcdp->todialog = LocationConstraintToDialog;
  lcdp->fromdialog = DialogToLocationConstraint;
  lcdp->dialogmessage = LocationConstraintMessage;
  lcdp->testdialog = TestLocationConstraintDialog;
  
  lcdp->show_interval_controls = show_interval_controls;

  if (lcdp->show_interval_controls)
  {
    g = HiddenGroup (p, 6, 0, NULL);
    SetGroupSpacing (g, 10, 10);
    lcdp->interval_end_choice = PopupList (g, TRUE, ShowLocationChoiceControls);
    PopupItem (lcdp->interval_end_choice, "Entire location");
    PopupItem (lcdp->interval_end_choice, "Start");
    PopupItem (lcdp->interval_end_choice, "Stop");
    SetValue (lcdp->interval_end_choice, 1);
    SetObjectExtra (lcdp->interval_end_choice, lcdp, NULL);
  
    StaticPrompt (g, "is", 0, dialogTextHeight, systemFont, 'l');
  
    k = HiddenGroup (g, 0, 0, NULL);
    lcdp->interval_match_choice = PopupList (k, TRUE, ShowLocationChoiceControls);
    PopupItem (lcdp->interval_match_choice, "Any location");
    PopupItem (lcdp->interval_match_choice, "Upstream from");
    PopupItem (lcdp->interval_match_choice, "Downstream from");
    PopupItem (lcdp->interval_match_choice, "Contained in");
    PopupItem (lcdp->interval_match_choice, "Not in");
    PopupItem (lcdp->interval_match_choice, "Overlaps");
    PopupItem (lcdp->interval_match_choice, "Equal to");
    SetValue (lcdp->interval_match_choice, 1);
    SetObjectExtra (lcdp->interval_match_choice, lcdp, NULL);
  
    lcdp->endpoint_match_choice = PopupList (k, TRUE, ShowLocationChoiceControls);
    PopupItem (lcdp->endpoint_match_choice, "Any location");
    PopupItem (lcdp->endpoint_match_choice, "Upstream from");
    PopupItem (lcdp->endpoint_match_choice, "Downstream from");
    PopupItem (lcdp->endpoint_match_choice, "Contained in");
    PopupItem (lcdp->endpoint_match_choice, "Not in");
    SetValue (lcdp->endpoint_match_choice, 1);
    SetObjectExtra (lcdp->endpoint_match_choice, lcdp, NULL);

    g2 = HiddenGroup (g, 2, 0, NULL);
    SetGroupSpacing (g2, 10, 10);
    val_grp = HiddenGroup (g2, 0, 0, NULL);
    lcdp->only_val = DialogText (val_grp, "", 5, NULL);
    g3 = HiddenGroup (val_grp, 3, 0, NULL);
    lcdp->first_val = DialogText (g3, "", 5, NULL);
    lcdp->second_val_prompt = StaticPrompt (g3, "to", 0, dialogTextHeight, systemFont, 'l');
    lcdp->second_val = DialogText (g3, "", 5, NULL);
    AlignObjects (ALIGN_CENTER, (HANDLE) lcdp->only_val, (HANDLE) g3, NULL);
  }
  else
  {
    g = HiddenGroup (p, 2, 0, NULL);
  }
  
  strand_grp = HiddenGroup (g, 2, 0, NULL);
  StaticPrompt (strand_grp, "on", 0, dialogTextHeight, systemFont, 'l');
  lcdp->strand = PopupList (strand_grp, TRUE, NULL);
  PopupItem (lcdp->strand, "Any strand");
  PopupItem (lcdp->strand, "Plus strand");
  PopupItem (lcdp->strand, "Minus strand");
  SetValue (lcdp->strand, LOCATION_CONSTRAINT_ANY_STRAND);
  
  g4 = HiddenGroup (g, 3, 0, NULL);
  SetGroupSpacing (g4, 10, 10);
  StaticPrompt (g4, "on", 0, dialogTextHeight, systemFont, 'l');
  lcdp->sequence_type = PopupList (g4, TRUE, NULL);
  PopupItem (lcdp->sequence_type, "nucleotide and protein sequences");
  PopupItem (lcdp->sequence_type, "nucleotide sequences only");
  PopupItem (lcdp->sequence_type, "protein sequences only");
  SetValue (lcdp->sequence_type, LOCATION_CONSTRAINT_ANY_SEQ);

  if (clear_btn)
  {
    b = PushButton (p, "Clear Constraint", ClearDialogBtn);
    SetObjectExtra (b, p, NULL);
  }

  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) b, NULL);
  
  ShowLocationChoiceControls (lcdp->interval_match_choice);
  return (DialoG) p;
}

static Boolean DoesStrandMatchConstraint (SeqLocPtr slp, LocationConstraintPtr lcp)
{
  Uint2 strand;
  Boolean rval = FALSE;
  
  if (slp == NULL)
  {
    rval = FALSE;
  }
  else if (lcp == NULL || lcp->strand == LOCATION_CONSTRAINT_ANY_STRAND)
  {
    rval = TRUE;
  }
  else
  {
    strand = SeqLocStrand (slp);
    if (strand == Seq_strand_minus)
    {
      if (lcp->strand == LOCATION_CONSTRAINT_MINUS_STRAND)
      {
        rval = TRUE;
      }
      else
      {
        rval = FALSE;
      }
    }
    else
    {
      if (lcp->strand == LOCATION_CONSTRAINT_PLUS_STRAND)
      {
        rval = TRUE;
      }
      else
      {
        rval = FALSE;
      }
    }
  }
  return rval;
}

static Boolean DoesSequenceTypeMatchContraint (SeqLocPtr slp, LocationConstraintPtr lcp)
{
  Boolean   rval = FALSE;
  BioseqPtr bsp;
  
  if (slp == NULL)
  {
    rval = FALSE;
  }
  else if (lcp == NULL || lcp->sequence_type == LOCATION_CONSTRAINT_ANY_SEQ)
  {
    rval = TRUE;
  }
  else
  {
    bsp = BioseqFindFromSeqLoc (slp);
    if (bsp != NULL)
    {
      if (ISA_na (bsp->mol) && lcp->sequence_type == LOCATION_CONSTRAINT_NUC_SEQ)
      {
        rval = TRUE;
      }
      else if (ISA_aa (bsp->mol) && lcp->sequence_type == LOCATION_CONSTRAINT_PROT_SEQ)
      {
        rval = TRUE;
      }
    }
  }
  return rval;
}

extern Boolean DoesLocationMatchConstraint (SeqLocPtr slp, LocationConstraintPtr lcp)

{
  Boolean rval = FALSE;
  Int4    loc_start, loc_stop, endpoint = 0;
  
  if (slp == NULL)
  {
    return FALSE;
  }
  
  if (lcp == NULL)
  {
    rval = TRUE;
  }
  else if (! DoesStrandMatchConstraint (slp, lcp))
  {
    rval = FALSE;
  }
  else if (! DoesSequenceTypeMatchContraint (slp, lcp))
  {
    rval = FALSE;
  }
  else
  {
    switch (lcp->match_choice)
    {
      case LOCATION_CONSTRAINT_ANY :
        rval = TRUE;
        break;
      case LOCATION_CONSTRAINT_UPSTREAM :
        if (lcp->interval_end_choice == LOCATION_CONSTRAINT_WHOLE_INTERVAL
            || lcp->interval_end_choice == LOCATION_CONSTRAINT_STOP_ENDPOINT)
        {
          endpoint = SeqLocStop (slp);
        }
        else
        {
          endpoint = SeqLocStart (slp);
        }
        if (endpoint < lcp->left)
        {
          rval = TRUE;
        }
        break;
      case LOCATION_CONSTRAINT_DOWNSTREAM :
        if (lcp->interval_end_choice == LOCATION_CONSTRAINT_WHOLE_INTERVAL
            || lcp->interval_end_choice == LOCATION_CONSTRAINT_START_ENDPOINT)
        {
          endpoint = SeqLocStart (slp);
        }
        else
        {
          endpoint = SeqLocStop (slp);
        }
        if (endpoint > lcp->left)
        {
          rval = TRUE;
        }
        break;
      case LOCATION_CONSTRAINT_CONTAINED :
        if (lcp->interval_end_choice == LOCATION_CONSTRAINT_WHOLE_INTERVAL)
        {
          loc_start = SeqLocStart (slp);
          loc_stop = SeqLocStop (slp);
        }
        else
        {
          if (lcp->interval_end_choice == LOCATION_CONSTRAINT_START_ENDPOINT)
          {
            endpoint = SeqLocStart (slp);
          }
          else
          {
            endpoint = SeqLocStop (slp);
          }
          loc_start = endpoint;
          loc_stop = endpoint;
        }
        if (loc_start >= lcp->left && loc_stop <= lcp->right)
        {
          rval = TRUE;
        }
        break;
      case LOCATION_CONSTRAINT_NOT_IN :
        if (lcp->interval_end_choice == LOCATION_CONSTRAINT_WHOLE_INTERVAL)
        {
          loc_start = SeqLocStart (slp);
          loc_stop = SeqLocStop (slp);
        }
        else
        {
          if (lcp->interval_end_choice == LOCATION_CONSTRAINT_START_ENDPOINT)
          {
            endpoint = SeqLocStart (slp);
          }
          else
          {
            endpoint = SeqLocStop (slp);
          }
          loc_start = endpoint;
          loc_stop = endpoint;
        }
        if (loc_stop <= lcp->left || loc_start >= lcp->right)
        {
          rval = TRUE;
        }
        break;
      case LOCATION_CONSTRAINT_OVERLAP :
        loc_start = SeqLocStart (slp);
        loc_stop = SeqLocStop (slp);
        if (loc_start <= lcp->left && loc_stop >= lcp->left)
        {
          rval = TRUE;
        }
        else if (loc_start <= lcp->right && loc_stop >= lcp->right)
        {
          rval = TRUE;
        }
        else if (loc_start >= lcp->left && loc_stop <= lcp->right)
        {
          rval = TRUE;
        }
        break;
      case LOCATION_CONSTRAINT_EQUAL :
        loc_start = SeqLocStart (slp);
        loc_stop = SeqLocStop (slp);
        if (loc_start == lcp->left && loc_stop == lcp->right)
        {
          rval = TRUE;
        }
        break;
    }
  }
  return rval; 
}

typedef struct filterform
{
  DIALOG_MESSAGE_BLOCK
  
  DialoG string_constraint;
  DialoG source_constraint;
  DialoG location_constraint;
  DialoG cds_gene_prot_constraint;
  
  DialoG tbs;
  DialoG pages[4];
  Int4   current_page;
  
} FilterFormData, PNTR FilterFormPtr;

static void FilterToDialog (DialoG d, Pointer userdata)
{
  FilterFormPtr dlg;
  FilterSetPtr dlg_data;
  
  dlg = (FilterFormPtr) GetObjectExtra (d);
  
  if (dlg == NULL)
  {
    return;
  }
  dlg_data = (FilterSetPtr) userdata;
  PointerToDialog (dlg->string_constraint, NULL);
  PointerToDialog (dlg->source_constraint, NULL);
  PointerToDialog (dlg->location_constraint, NULL);
  PointerToDialog (dlg->cds_gene_prot_constraint, NULL);
  
  if (dlg_data != NULL)
  {
    PointerToDialog (dlg->string_constraint, dlg_data->scp);
    PointerToDialog (dlg->source_constraint, dlg_data->ccp);
    PointerToDialog (dlg->location_constraint, dlg_data->lcp);
    PointerToDialog (dlg->cds_gene_prot_constraint, dlg_data->cgp);
  }
}

static Pointer DialogToFilter (DialoG d)
{
  FilterFormPtr dlg;
  FilterSetPtr dlg_data;
  
  dlg = (FilterFormPtr) GetObjectExtra (d);
  
  if (dlg == NULL)
  {
    return NULL;
  }
  dlg_data = (FilterSetPtr) MemNew (sizeof (FilterSetData));
  
  if (dlg_data != NULL)
  {
    dlg_data->scp = DialogToPointer (dlg->string_constraint);
    dlg_data->ccp = DialogToPointer (dlg->source_constraint);
    dlg_data->lcp = DialogToPointer (dlg->location_constraint);
    dlg_data->cgp = DialogToPointer (dlg->cds_gene_prot_constraint);
  }
  return dlg_data;
}

static void FilterFormMessage (DialoG d, Int2 mssg)

{
}

static ValNodePtr TestFilterFormDialog (DialoG d)

{
  return NULL;
}

static void ChangeFilterPage (VoidPtr data, Int2 newval, Int2 oldval)

{
  FilterFormPtr ffp;

  ffp = (FilterFormPtr) data;
  if (ffp != NULL) {
    ffp->current_page = newval;
    SafeHide (ffp->pages [oldval]);
    Update ();
    SafeShow (ffp->pages [newval]);
    Update ();
  }
}

static void ClearAllConstraints (ButtoN b)
{
  FilterFormPtr ffp;

  ffp = (FilterFormPtr) GetObjectExtra (b);
  if (ffp != NULL)
  {
    PointerToDialog (ffp->dialog, NULL);
  }
}

extern DialoG 
FilterGroup 
(GrouP h,
 Boolean has_string_constraint,
 Boolean has_source_constraint,
 Boolean has_location_constraint,
 Boolean has_cds_gene_prot_constraint,
 CharPtr string_constraint_label)
{
  GrouP         g, k;
  FilterFormPtr ffp;
  Int4          num_pages = 0;
  CharPtr       filterTabs[5];
  Int4          i;
  ButtoN        clear_constraints = NULL;
  
  if (! has_string_constraint
      && ! has_source_constraint
      && ! has_location_constraint
      && ! has_cds_gene_prot_constraint)
  {
    return NULL;
  }
  
  ffp = (FilterFormPtr) MemNew (sizeof (FilterFormData));
  if (ffp == NULL)
  {
    return NULL;
  }

  g = NormalGroup (h, -1, 0, NULL, programFont, NULL);
  SetObjectExtra (g, ffp, StdCleanupExtraProc);

  ffp->dialog = (DialoG) g;
  ffp->todialog = FilterToDialog;
  ffp->fromdialog = DialogToFilter;
  ffp->dialogmessage = FilterFormMessage;
  ffp->testdialog = TestFilterFormDialog;
  
  ffp->current_page = 0;

  if (has_source_constraint)
  {
    filterTabs [num_pages++] = "Source Constraint";
  }
  if (has_string_constraint)
  {
    filterTabs [num_pages++] = "String Constraint";
  }
  if (has_location_constraint)
  {
    filterTabs [num_pages++] = "Location Constraint";
  }
  if (has_cds_gene_prot_constraint)
  {
    filterTabs [num_pages++] = "CDS-Gene-Prot-mRNA Set Constraint";
  }
  filterTabs [num_pages] = NULL;
  
  if (num_pages > 1)
  {
  
    ffp->tbs = CreateFolderTabs (g, filterTabs, 0,
                                 0, 0, PROGRAM_FOLDER_TAB,
                                 ChangeFilterPage, (Pointer) ffp);
    k = HiddenGroup (g, 0, 0, NULL);
    num_pages = 0;
    if (has_source_constraint)
    {
      ffp->source_constraint = SourceConstraintDialog (k, FALSE);
      ffp->pages [num_pages++] = ffp->source_constraint;
    }
    if (has_string_constraint)
    {
      ffp->string_constraint = StringConstraintDialog (k, string_constraint_label, FALSE);
      ffp->pages [num_pages++] = ffp->string_constraint;
    }
    if (has_location_constraint)
    {
      ffp->location_constraint = LocationConstraintDialog (k, FALSE, FALSE);
      ffp->pages [num_pages++] = ffp->location_constraint;
    }
    if (has_cds_gene_prot_constraint)
    {
      ffp->cds_gene_prot_constraint = CDSGeneProtConstraintDialog (k, FALSE);
      ffp->pages [num_pages++] = ffp->cds_gene_prot_constraint;
    }
    for (i = 1; i < num_pages; i++)
    {
      Hide (ffp->pages [i]);
    }
    AlignObjects (ALIGN_CENTER, (HANDLE) ffp->pages [0],
                                (HANDLE) ffp->pages [1],
                                (HANDLE) ffp->pages [2],
                                NULL);
    clear_constraints = PushButton (g, "Clear Constraints", ClearAllConstraints);
    SetObjectExtra (clear_constraints, ffp, NULL);
    AlignObjects (ALIGN_CENTER, (HANDLE) ffp->tbs, (HANDLE) k, (HANDLE) clear_constraints, NULL);
  }
  else
  {
    if (has_source_constraint)
    {
      ffp->source_constraint = SourceConstraintDialog (g, TRUE);
    }
    else if (has_string_constraint)
    {
      ffp->string_constraint = StringConstraintDialog (g, string_constraint_label, TRUE);
    }
    else if (has_location_constraint)
    {
      ffp->location_constraint = LocationConstraintDialog (g, FALSE, TRUE);
    }
    else if (has_cds_gene_prot_constraint)
    {
      ffp->cds_gene_prot_constraint = CDSGeneProtConstraintDialog (g, TRUE);
    }
  }
  return (DialoG) g;
}

/* Operations on constrained features and descriptors */
typedef struct objecthasstring
{
  StringConstraintPtr scp;
  Boolean             found;
} ObjectHasStringData, PNTR ObjectHasStringPtr;

typedef struct constraintop 
{
  FilterSetPtr           fsp;
  ObjMgrPtr              omp;
  ObjMgrTypePtr          omtp;
  AsnIoPtr               aip;
  Uint2                  entityID;
  ObjectHasStringData    ohsd;
  Pointer                userdata;
  FeatureActionProc      feature_action;
  Uint1                  seqFeatChoice;
  Uint1                  featDefChoice;
  DescriptorActionProc   descriptor_action;
  Uint1                  descriptorChoice;
} ConstraintOpData, PNTR ConstraintOpPtr;

extern Boolean DoesStringMatchConstraint (CharPtr pchSource, StringConstraintPtr scp)
{
  CharPtr pFound;
  Boolean rval = FALSE;
  Char    char_after;
  
  if (pchSource == NULL) return FALSE;
  
  if (scp == NULL || StringHasNoText (scp->match_text)) return TRUE;
 
	if (scp->insensitive)
	{
	  pFound = StringISearch (pchSource, scp->match_text);
	}
	else
	{
	  pFound = StringSearch (pchSource, scp->match_text);
	}
	  
	if (pFound != NULL)
	{
	  rval = TRUE;
	  char_after = *(pFound + StringLen (scp->match_text));
	  if (scp->match_location == STRING_CONSTRAINT_STARTS && pFound != pchSource)
	  {
	    rval = FALSE;
	  }
	  else if (scp->match_location == STRING_CONSTRAINT_ENDS && char_after != 0)
	  {
	    rval = FALSE;
	  }
	  else if (scp->whole_word)
	  {
	    rval = IsWholeWordMatch (pchSource, pFound, StringLen (scp->match_text));
	  }
	}
	return rval;
}

static void LIBCALLBACK AsnWriteConstraintCallBack (AsnExpOptStructPtr pAEOS)

{
  CharPtr            pchSource;
  ObjectHasStringPtr ohsp;

  ohsp = (ObjectHasStringPtr) pAEOS->data;
  if (ISA_STRINGTYPE (AsnFindBaseIsa (pAEOS->atp))) 
  {
	  pchSource = (CharPtr) pAEOS->dvp->ptrvalue;
	  ohsp->found |= DoesStringMatchConstraint (pchSource, ohsp->scp);
  }
}

static Boolean 
DoesObjectMatchStringConstraint 
(ObjMgrTypePtr      omtp,
 AsnIoPtr           aip, 
 Pointer            ptr, 
 ObjectHasStringPtr ohsp)

{
  if (omtp == NULL || ohsp == NULL)
  {
    return FALSE;
  }
  ohsp->found = FALSE;
  (omtp->asnwrite) (ptr, aip, NULL);
  return ohsp->found;
}

static Boolean DoesSourceHaveOneQualPresent (BioSourcePtr biop, SourceQualDescPtr sqdp)
{
  OrgModPtr    mod;
  SubSourcePtr ssp;
  Boolean      is_present = FALSE;
  
  if (biop == NULL || sqdp == NULL)
  {
    return FALSE;
  }
  
  if (sqdp->isOrgMod)
  {
    if (biop->org != NULL || biop->org->orgname != NULL)
    {
      mod = biop->org->orgname->mod;
      while (mod != NULL && mod->subtype != sqdp->subtype)
      {
        mod = mod->next;
      }
      if (mod != NULL && mod->subtype == sqdp->subtype)
      {
        is_present = TRUE;
      }
    }
  }
  else
  {
    ssp = biop->subtype;
    while (ssp != NULL && ssp->subtype != sqdp->subtype)
    {
      ssp = ssp->next;
    }
    if (ssp != NULL && ssp->subtype == sqdp->subtype)
    {
      is_present = TRUE;
    }
  }
  return is_present;
}

static Boolean DoesSourceHaveQualPresent (BioSourcePtr biop, ValNodePtr qual_list)
{
  ValNodePtr        vnp;
  Boolean           qual_found = FALSE;
  
  if (biop == NULL)
  {
    return FALSE;
  }
  if (qual_list == NULL)
  {
    return TRUE;
  }
  
  for (vnp = qual_list; vnp != NULL && !qual_found; vnp = vnp->next)
  {
    qual_found = DoesSourceHaveOneQualPresent (biop, (SourceQualDescPtr) vnp->data.ptrvalue);
  }
  
  return qual_found;
}

static Boolean 
DoesOneSourceMatchConstraint 
(BioSourcePtr biop, ChoiceConstraintPtr scp)
{
  Boolean                does_match = FALSE;
  OrgModPtr              mod = NULL;
  SubSourcePtr           ssp;
  SourceQualDescPtr      sqdp;
  
  if (scp == NULL || scp->constraint_type == CHOICE_CONSTRAINT_ANY)
  {
    return TRUE;
  }
  if (biop == NULL)
  {
    return FALSE;
  }
  
  if (scp->constraint_type == CHOICE_CONSTRAINT_STRING)
  {
    if (scp->qual_choice == NULL
        || scp->qual_choice->data.ptrvalue == NULL
        || StringICmp (scp->qual_choice->data.ptrvalue, "Organism or Any Qual") == 0)
    {
      if (biop->org != NULL)
      {
        does_match = DoesStringMatchConstraint (biop->org->taxname, scp->string_constraint);
        if (biop->org->orgname != NULL)
        {
          mod = biop->org->orgname->mod;
        }
        while (! does_match && mod != NULL)
        {
          does_match = DoesStringMatchConstraint (mod->subname, scp->string_constraint);
          mod = mod->next;
        }
      }
      ssp = biop->subtype;
      while (!does_match && ssp != NULL)
      {
        does_match = DoesStringMatchConstraint (ssp->name, scp->string_constraint);
        ssp = ssp->next;
      }
    }
    else if (StringICmp (scp->qual_choice->data.ptrvalue, "Organism") == 0)
    {
      if (biop->org != NULL)
      {
        does_match = DoesStringMatchConstraint (biop->org->taxname, scp->string_constraint);
      }
    }
    else if (StringICmp (scp->qual_choice->data.ptrvalue, "Lineage") == 0)
    {
      if (biop->org != NULL && biop->org->orgname != NULL)
      {
        does_match = DoesStringMatchConstraint (biop->org->orgname->lineage,
                                                scp->string_constraint);
      }
    }
    else
    {
      sqdp = (SourceQualDescPtr) scp->qual_choice->data.ptrvalue;
      if (sqdp->isOrgMod)
      {
        if (biop->org != NULL && biop->org->orgname != NULL)
        {
          mod = biop->org->orgname->mod;
        }
        while (! does_match && mod != NULL)
        {
          if (mod->subtype == sqdp->subtype)
          {
            does_match = DoesStringMatchConstraint (mod->subname, scp->string_constraint);
          }
          mod = mod->next;
        }
      }
      else
      {
        ssp = biop->subtype;
        while (!does_match && ssp != NULL)
        {
          if (ssp->subtype == sqdp->subtype)
          {
            does_match = DoesStringMatchConstraint (ssp->name, scp->string_constraint);
          }
          ssp = ssp->next;
        }
      }
    }
  }
  else
  {
    does_match = DoesSourceHaveQualPresent (biop, scp->qual_choice);
  }
  return does_match;
}

static void SeqEntryConstrainedFeaturesCallback (SeqFeatPtr sfp, Pointer userdata)
{
  SeqMgrFeatContext fcontext;
  ConstraintOpPtr   cop;
  Boolean           feature_matches = TRUE;
  
  if (sfp == NULL || userdata == NULL) return;
  cop = (ConstraintOpPtr) userdata;
  sfp = SeqMgrGetDesiredFeature (cop->entityID, NULL, 0, 0, sfp, &fcontext);
  if (sfp == NULL) return;
  
  if (cop->seqFeatChoice != 0 && cop->seqFeatChoice != sfp->data.choice) return;
  if (cop->featDefChoice != 0 && cop->featDefChoice != sfp->idx.subtype) return;
  
  if (cop->fsp != NULL && cop->fsp->scp != NULL)
  {
    if (cop->fsp->scp->not_present)
    {
      if (DoesObjectMatchStringConstraint (cop->omtp, cop->aip, (Pointer) sfp, &(cop->ohsd))
          || DoesStringMatchConstraint (fcontext.label, cop->fsp->scp))
      {
        feature_matches = FALSE; 
      }
    }
    else
    {
      if (! DoesObjectMatchStringConstraint (cop->omtp, cop->aip, (Pointer) sfp, &(cop->ohsd))
          && ! DoesStringMatchConstraint (fcontext.label, cop->fsp->scp))
      {
        feature_matches = FALSE;
      }
    }
  }
  if (cop->fsp != NULL && cop->fsp->lcp != NULL)
  {
    if (! DoesLocationMatchConstraint (sfp->location, cop->fsp->lcp))
    {
      feature_matches = FALSE;
    }
  }
  
  if (feature_matches)
  {
    (cop->feature_action) (sfp, cop->userdata, cop->fsp);
  }
}

static void SeqEntryConstrainedDescriptorsCallback (SeqDescrPtr sdp, Pointer userdata)
{
  ConstraintOpPtr cop;
  Boolean         descriptor_matches = TRUE;
  
  if (sdp == NULL || userdata == NULL) return;
  cop = (ConstraintOpPtr) userdata;
  
  if (cop != NULL && cop->descriptorChoice != 0 && cop->descriptorChoice != sdp->choice)
  {
    return;
  }
  
  if (cop != NULL && cop->fsp != NULL && cop->fsp->scp != NULL)
  {
    if (cop->fsp->scp->not_present)
    {
      if (DoesObjectMatchStringConstraint (cop->omtp, cop->aip, (Pointer) sdp, &(cop->ohsd)))
      {
        descriptor_matches = FALSE;
      }
    }
    else
    {
      if (! DoesObjectMatchStringConstraint (cop->omtp, cop->aip, (Pointer) sdp, &(cop->ohsd)))
      {
        descriptor_matches = FALSE;
      }
    }
  }
  if (descriptor_matches)
  {
    (cop->descriptor_action) (sdp, cop->userdata, cop->fsp);
  }
}


static void ConstrainedSourceCallback (SeqEntryPtr sep, ConstraintOpPtr cop)
{
  BioseqPtr       bsp;
  BioseqSetPtr    bssp = NULL;
  SeqAnnotPtr     sap = NULL;
  SeqDescrPtr     sdp = NULL;
  SeqFeatPtr      sfp;
  Boolean         found = FALSE;
  
  if (sep == NULL || sep->data.ptrvalue == NULL || cop == NULL || cop->fsp == NULL)
  {
    return;
  }
   
  if (IS_Bioseq (sep))
  {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
    sdp = bsp->descr;
  }
  else if (IS_Bioseq_set (sep))
  {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
    sdp = bssp->descr;
  }
  while (sap != NULL && !found)
  {
    if (sap->type == 1)
    {
      sfp = (SeqFeatPtr) sap->data;
      while (sfp != NULL && ! found)
      {
        if (sfp->data.choice == SEQFEAT_BIOSRC 
            && DoesOneSourceMatchConstraint (sfp->data.value.ptrvalue, cop->fsp->ccp))
        {
          found = TRUE;
        }
        sfp = sfp->next;
      }
    }
    sap = sap->next;
  }
  
  while (sdp != NULL && !found)
  {
    if (sdp->choice == Seq_descr_source
        && DoesOneSourceMatchConstraint (sdp->data.ptrvalue, cop->fsp->ccp))
    {
      found = TRUE;
    }
    sdp = sdp->next;
  }
  if (found)
  {
    if (cop->feature_action != NULL)
    {
      cop->omtp = ObjMgrTypeFind (cop->omp, OBJ_SEQFEAT, NULL, NULL);
      if (cop->omtp != NULL)
      {
        VisitFeaturesInSep (sep, cop, SeqEntryConstrainedFeaturesCallback);
      }
    }
  
    if (cop->descriptor_action != NULL)
    {
      cop->omtp = ObjMgrTypeFind (cop->omp, OBJ_SEQDESC, NULL, NULL);
      if (cop->omtp != NULL)
      {
        VisitDescriptorsInSep (sep, cop, SeqEntryConstrainedDescriptorsCallback);
      }
    }
  }
  else if (bssp != NULL)
  {
    ConstrainedSourceCallback (bssp->seq_set, cop);
  }
  ConstrainedSourceCallback (sep->next, cop);
}

extern void 
OperateOnSeqEntryConstrainedObjects 
(SeqEntryPtr           sep,
 FilterSetPtr          fsp,
 FeatureActionProc     feature_action,
 DescriptorActionProc  descriptor_action,
 Uint1                 seqFeatChoice,
 Uint1                 featDefChoice,
 Uint1                 descriptorChoice,
 Pointer               userdata)
{
  ConstraintOpData cod;
  AsnExpOptPtr     aeop;
  
  if (sep == NULL) return;
  if (feature_action == NULL && descriptor_action == NULL) return;
  
  cod.omp = ObjMgrGet ();
  if (cod.omp == NULL) return;
  
  cod.fsp               = fsp;
  cod.feature_action    = feature_action;
  cod.descriptor_action = descriptor_action;
  cod.userdata          = userdata;
  cod.entityID          = SeqMgrGetEntityIDForSeqEntry (sep);
  cod.seqFeatChoice     = seqFeatChoice;
  cod.featDefChoice     = featDefChoice;
  cod.descriptorChoice  = descriptorChoice;

  cod.aip = AsnIoNullOpen ();
  if (fsp == NULL)
  {
    cod.ohsd.scp = NULL;
  }
  else
  {
    cod.ohsd.scp = fsp->scp;
  }
  
  aeop = AsnExpOptNew (cod.aip, NULL, NULL, AsnWriteConstraintCallBack);
  if (aeop != NULL) {
    aeop->user_data = (Pointer) &(cod.ohsd);
  }
  
  if (fsp == NULL || fsp->ccp == NULL || fsp->ccp->constraint_type == CHOICE_CONSTRAINT_ANY)
  {
    if (feature_action != NULL)
    {
      cod.omtp = ObjMgrTypeFind (cod.omp, OBJ_SEQFEAT, NULL, NULL);
      if (cod.omtp != NULL)
      {
        VisitFeaturesInSep (sep, &cod, SeqEntryConstrainedFeaturesCallback);
      }
    }
  
    if (descriptor_action != NULL)
    {
      cod.omtp = ObjMgrTypeFind (cod.omp, OBJ_SEQDESC, NULL, NULL);
      if (cod.omtp != NULL)
      {
        VisitDescriptorsInSep (sep, &cod, SeqEntryConstrainedDescriptorsCallback);
      }
    }
  }
  else
  {
    ConstrainedSourceCallback (sep, &cod);
  }

  AsnIoClose (cod.aip);
}

static GetSamplePtr GetSampleFree (GetSamplePtr gsp)
{
  if (gsp != NULL)
  {
    gsp->sample_text = MemFree (gsp->sample_text);
    if (gsp->free_vn_proc != NULL)
    {
      (gsp->free_vn_proc)(gsp->requested_field);
    }
    gsp->requested_field = ValNodeFree (gsp->requested_field);
    gsp = MemFree (gsp);
  }
  return gsp;
}

static GetSamplePtr GetSampleCopy (GetSamplePtr gsp)
{
  GetSamplePtr copy_gsp;
  
  if (gsp == NULL)
  {
    return NULL;
  }
  
  copy_gsp = (GetSamplePtr) MemNew (sizeof (GetSampleData));
  if (copy_gsp != NULL)
  {
    copy_gsp->fieldstring_func = gsp->fieldstring_func;
    copy_gsp->descrstring_func = gsp->descrstring_func;
    copy_gsp->free_vn_proc = gsp->free_vn_proc;
    copy_gsp->copy_vn_proc = gsp->copy_vn_proc;
    if (copy_gsp->copy_vn_proc != NULL)
    {
      copy_gsp->requested_field = (gsp->copy_vn_proc) (gsp->requested_field);
    }
    copy_gsp->sample_text = StringSave (gsp->sample_text);
    copy_gsp->num_found = gsp->num_found;
    copy_gsp->all_same = gsp->all_same;
  }
  return copy_gsp;
}

static GetSamplePtr GetSampleAdd (GetSamplePtr gsp1, GetSamplePtr gsp2)
{
  GetSamplePtr sum_gsp;
  
  if (gsp1 == NULL && gsp2 == NULL)
  {
    sum_gsp =  NULL;
  }
  else if (gsp1 == NULL)
  {
    sum_gsp =  GetSampleCopy (gsp2);
  }
  else if (gsp2 == NULL)
  {
    sum_gsp =  GetSampleCopy (gsp1);
  }
  else
  {
    sum_gsp = (GetSamplePtr) MemNew (sizeof (GetSampleData));
    if (sum_gsp != NULL)
    {
      sum_gsp->fieldstring_func = gsp1->fieldstring_func;
      sum_gsp->descrstring_func = gsp1->descrstring_func;
      sum_gsp->requested_field = gsp1->requested_field;
      gsp1->requested_field = NULL;
      sum_gsp->sample_text = StringSave (gsp1->sample_text);
      sum_gsp->num_found = gsp1->num_found + gsp2->num_found;
      if (gsp1->num_found == 0)
      {
        sum_gsp->all_same = gsp2->all_same;
      }
      else if (gsp2->num_found == 0)
      {
        sum_gsp->all_same = gsp1->all_same;
      }
      else if (!gsp1->all_same || ! gsp2->all_same)
      {
        sum_gsp->all_same = FALSE;
      }
      else if (StringCmp (gsp1->sample_text, gsp2->sample_text) == 0)
      {
        sum_gsp->all_same = TRUE;
      }
      else
      {
        sum_gsp->all_same = FALSE;
      }
    }
  }
  return sum_gsp;
}


static void GetSampleFeatureCallback (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  GetSamplePtr cp;
  CharPtr      str;
  
  if (sfp == NULL || userdata == NULL)
  {
    return;
  }
  
  cp = (GetSamplePtr) userdata;
  if (cp->fieldstring_func == NULL)
  {
    return;
  }
  
  str = cp->fieldstring_func (sfp, cp->requested_field, NULL);
  if (!StringHasNoText (str))
  {
    cp->num_found ++;
    if (cp->sample_text == NULL)
    {
      cp->sample_text = str;
    }
    else
    {
      if (StringCmp (str, cp->sample_text) != 0)
      {
        cp->all_same = FALSE;
      }
      str = MemFree (str);
    }
  }
}

static void GetSampleDescriptorCallback (SeqDescrPtr sdp, Pointer userdata, FilterSetPtr fsp)
{
  GetSamplePtr cp;
  CharPtr      str;
  
  if (sdp == NULL || userdata == NULL)
  {
    return;
  }
  
  cp = (GetSamplePtr) userdata;
  if (cp->descrstring_func == NULL)
  {
    return;
  }
  
  str = cp->descrstring_func (sdp, cp->requested_field, NULL);
  if (!StringHasNoText (str))
  {
    cp->num_found ++;
    if (cp->sample_text == NULL)
    {
      cp->sample_text = str;
    }
    else
    {
      if (StringCmp (str, cp->sample_text) != 0)
      {
        cp->all_same = FALSE;
      }
      str = MemFree (str);
    }
  }
}

static GetSamplePtr CheckForExistingTextInSeqEntry
(SeqEntryPtr              sep,
 ValNodePtr               requested_field,
 GetFeatureFieldString    fieldstring_func,
 GetDescriptorFieldString descrstring_func,
 FreeValNodeProc          free_vn_proc,
 CopyValNodeDataProc      copy_vn_proc,
 FilterSetPtr             fsp,
 Uint1                    seqfeat_choice,
 Uint1                    featdef_choice,
 Uint1                    descr_choice)
{
  GetSamplePtr gsp;
  
  gsp = (GetSamplePtr) MemNew (sizeof (GetSampleData));
  if (gsp != NULL)
  {
    gsp->sample_text = NULL;
    gsp->fieldstring_func = fieldstring_func;
    gsp->descrstring_func = descrstring_func;
    gsp->free_vn_proc = free_vn_proc;
    gsp->copy_vn_proc = copy_vn_proc;
    gsp->requested_field = (gsp->copy_vn_proc) (requested_field);
    gsp->num_found = 0;
    gsp->all_same = TRUE;
    OperateOnSeqEntryConstrainedObjects (sep, fsp, GetSampleFeatureCallback, 
                                         GetSampleDescriptorCallback,
                                         seqfeat_choice, featdef_choice,
                                         descr_choice, gsp);
  }
  return gsp;  
}

static GetSamplePtr CheckForExistingText
(Uint2                    entityID,
 ValNodePtr               requested_field,
 GetFeatureFieldString    fieldstring_func,
 GetDescriptorFieldString descrstring_func,
 FreeValNodeProc          free_vn_proc,
 CopyValNodeDataProc      copy_vn_proc,
 FilterSetPtr             fsp,
 Uint1                    seqfeat_choice,
 Uint1                    featdef_choice,
 Uint1                    descr_choice)
{
  SeqEntryPtr  sep;

  sep = GetTopSeqEntryForEntityID (entityID);
  return CheckForExistingTextInSeqEntry (sep, requested_field, 
                                         fieldstring_func, descrstring_func,
                                         free_vn_proc, copy_vn_proc, fsp,
                                         seqfeat_choice, featdef_choice, descr_choice);
}

typedef struct existingtextdlg 
{
  GrouP pre_app_grp;
  GrouP delim_grp;
} ExistingTextDlgData, PNTR ExistingTextDlgPtr;

static void ChangePreAppIgnoreChoice (GrouP g)
{
  ExistingTextDlgPtr etdp;
  Int4               handle_choice;
  
  etdp = (ExistingTextDlgPtr) GetObjectExtra (g);
  if (etdp == NULL)
  {
    return;
  }
  
  handle_choice = GetValue (etdp->pre_app_grp);
  if (handle_choice == 1 || handle_choice == 2)
  {
    Enable (etdp->delim_grp);
  }
  else
  {
    Disable (etdp->delim_grp);
  }
}

static ExistingTextPtr GetExistingTextHandlerInfo (GetSamplePtr gsp, Boolean non_text)
{
  WindoW                w;
  GrouP                 h, c;
  ExistingTextDlgData   etdd;
  ButtoN                b;
  ModalAcceptCancelData acd;
  ExistingTextPtr       etp;
  Char                  txt [128];
  MsgAnswer             ans;
  PrompT                ppt;
  Int4                  handle_choice;

  if (gsp == NULL || gsp->num_found == 0)
  {
    return NULL;
  }
  
  sprintf (txt, "%d affected fields already contain a value.  Do you wish to overwrite existing text?",
           gsp->num_found);
  ans = Message (MSG_YNC, txt, 0, dialogTextHeight, systemFont, 'l');
  if (ans == ANS_CANCEL)
  {
    etp = (ExistingTextPtr) MemNew (sizeof (ExistingTextData));
    etp->existing_text_choice = EXISTING_TEXT_CHOICE_CANCEL;
    return etp;
  }
  else if (ans == ANS_YES)
  {
    etp = (ExistingTextPtr) MemNew (sizeof (ExistingTextData));
    etp->existing_text_choice = EXISTING_TEXT_CHOICE_REPLACE_OLD;
    return etp;
  }
    
  w = MovableModalWindow(-20, -13, -10, -10, "How to Add New Text", NULL);
  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);
  etdd.pre_app_grp = HiddenGroup (h, 0, 3, ChangePreAppIgnoreChoice);
  SetGroupSpacing (etdd.pre_app_grp, 10, 10);
  RadioButton (etdd.pre_app_grp, "Append");
  RadioButton (etdd.pre_app_grp, "Prefix");
  RadioButton (etdd.pre_app_grp, "Ignore new text");
  SetValue (etdd.pre_app_grp, 1);
  SetObjectExtra (etdd.pre_app_grp, &etdd, NULL);
  
  ppt = StaticPrompt (h, "Separate new text and old text with", 
                      0, dialogTextHeight, programFont, 'c');
  etdd.delim_grp = HiddenGroup (h, 0, 3, NULL);
  SetGroupSpacing (etdd.delim_grp, 10, 10);
  RadioButton (etdd.delim_grp, "Semicolon");
  RadioButton (etdd.delim_grp, "Space");
  RadioButton (etdd.delim_grp, "Do not separate");
  SetValue (etdd.delim_grp, 1);
  
  c = HiddenGroup (h, 2, 0, NULL);
  SetGroupSpacing (c, 10, 10);
  b = PushButton (c, "Accept", ModalAcceptButton);
  SetObjectExtra (b, &acd, NULL);
  b = PushButton (c, "Cancel", ModalCancelButton);
  SetObjectExtra (b, &acd, NULL);
  AlignObjects (ALIGN_CENTER, (HANDLE) etdd.pre_app_grp,
                              (HANDLE) ppt, 
                              (HANDLE) etdd.delim_grp, 
                              (HANDLE) c, 
                              NULL);
  Show (w);
  Select (w);
  acd.accepted = FALSE;
  acd.cancelled = FALSE;
  while (!acd.accepted && ! acd.cancelled)
  {
    ProcessExternalEvent ();
    Update ();
  }
  ProcessAnEvent ();
  etp = (ExistingTextPtr) MemNew (sizeof (ExistingTextData));
  if (acd.cancelled)
  {
    etp->existing_text_choice = EXISTING_TEXT_CHOICE_CANCEL;
  }
  else
  {
    handle_choice = GetValue (etdd.pre_app_grp);
    if (handle_choice == 1)
    {
      switch (GetValue (etdd.delim_grp))
      {
        case 1:
          etp->existing_text_choice = EXISTING_TEXT_CHOICE_APPEND_SEMI;
          break;
        case 2:
          etp->existing_text_choice = EXISTING_TEXT_CHOICE_APPEND_SPACE;
          break;
        case 3:
          etp->existing_text_choice = EXISTING_TEXT_CHOICE_APPEND_NONE;
          break;
      }
    }
    else if (handle_choice == 2)
    {
      switch (GetValue (etdd.delim_grp))
      {
        case 1:
          etp->existing_text_choice = EXISTING_TEXT_CHOICE_PREFIX_SEMI;
          break;
        case 2:
          etp->existing_text_choice = EXISTING_TEXT_CHOICE_PREFIX_SPACE;
          break;
        case 3:
          etp->existing_text_choice = EXISTING_TEXT_CHOICE_PREFIX_NONE;
          break;
      }
    }
    else
    {
      etp->existing_text_choice = EXISTING_TEXT_CHOICE_LEAVE_OLD;
    }
  }
  Remove (w);
  return etp;
}

extern CharPtr HandleExistingText (CharPtr existing_text, CharPtr new_text, ExistingTextPtr etp)
{
  CharPtr rstring = NULL;
  Int4    len;
  
  if (StringHasNoText (existing_text) || etp == NULL)
  {
    MemFree (existing_text);
    return new_text;
  }
  switch (etp->existing_text_choice)
  {
    case EXISTING_TEXT_CHOICE_REPLACE_OLD:
      /* replace current text with new text */
      MemFree (existing_text);
      rstring = new_text;
      break;
    case EXISTING_TEXT_CHOICE_LEAVE_OLD:
      /* do not change current text */
      MemFree (new_text);
      rstring = existing_text;
      break;
    case EXISTING_TEXT_CHOICE_APPEND_SEMI:
      /* Append new text to current text, separated by semicolon */
      len = StringLen (new_text) + StringLen (existing_text) + 4;
      rstring = MemNew (len);
      if (rstring != NULL) {
        StringCpy (rstring, existing_text);
        StringCat (rstring, "; ");
        StringCat (rstring, new_text);
        MemFree (new_text);
        MemFree (existing_text);
      }
      break;
    case EXISTING_TEXT_CHOICE_APPEND_SPACE:
      /* Append new text to current text, separated by space */
      len = StringLen (new_text) + StringLen (existing_text) + 3;
      rstring = MemNew (len);
      if (rstring != NULL) {
        StringCpy (rstring, existing_text);
        StringCat (rstring, " ");
        StringCat (rstring, new_text);
        MemFree (new_text);
        MemFree (existing_text);
      }
      break;
    case EXISTING_TEXT_CHOICE_APPEND_NONE:
      /* Append new text to current text, no delimiter */
      len = StringLen (new_text) + StringLen (existing_text) + 1;
      rstring = MemNew (len);
      if (rstring != NULL) {
        StringCpy (rstring, existing_text);
        StringCat (rstring, new_text);
        MemFree (new_text);
        MemFree (existing_text);
      }
      break;
    case EXISTING_TEXT_CHOICE_PREFIX_SEMI:
      /* Prepend new text to current text, separated by semicolon */
      len = StringLen (new_text) + StringLen (existing_text) + 4;
      rstring = MemNew (len);
      if (rstring != NULL) {
        StringCpy (rstring, new_text);
        StringCat (rstring, "; ");
        StringCat (rstring, existing_text);
        MemFree (new_text);
        MemFree (existing_text);
      }
      break;
    case EXISTING_TEXT_CHOICE_PREFIX_SPACE:
      /* Prepend new text to current text, separated by semicolon */
      len = StringLen (new_text) + StringLen (existing_text) + 3;
      rstring = MemNew (len);
      if (rstring != NULL) {
        StringCpy (rstring, new_text);
        StringCat (rstring, " ");
        StringCat (rstring, existing_text);
        MemFree (new_text);
        MemFree (existing_text);
      }
      break;
    case EXISTING_TEXT_CHOICE_PREFIX_NONE:
      /* prefix current text with new text */
      len = StringLen (new_text) + StringLen (existing_text) + 1;
      rstring = MemNew (len);
      if (rstring != NULL) {
        StringCpy (rstring, new_text);
        StringCat (rstring, existing_text);
        MemFree (new_text);
        MemFree (existing_text);
      }
      break;    
  }
  return rstring;
}

extern CharPtr HandleApplyValue (CharPtr orig_text, ApplyValuePtr avp)
{
  CharPtr new_str, cp_found;
  Int4    found_len, replace_len, new_len;
  
  if (avp == NULL)
  {
    return orig_text;
  }
  else if (StringHasNoText (orig_text))
  {
    if (StringHasNoText (avp->text_to_replace))
    {
      MemFree (orig_text);
      return StringSave (avp->new_text);
    }
    else
    {
      return orig_text;
    }
  }
  else if (StringHasNoText (avp->text_to_replace))
  {
    return HandleExistingText (orig_text, StringSave (avp->new_text), avp->etp);
  }
  else
  {
    found_len = StringLen (avp->text_to_replace);
    replace_len = StringLen (avp->new_text);
    cp_found = StringISearch (orig_text, avp->text_to_replace);
    while (cp_found != NULL)
    {
      new_len = StringLen (orig_text) + 1 - found_len + replace_len;
      new_str = (CharPtr) MemNew (new_len * sizeof (Char));
      if (new_str != NULL)
      {
        if (cp_found != orig_text)
        {
          StringNCpy (new_str, orig_text, cp_found - orig_text);
        }
        StringCat (new_str, avp->new_text);
        StringCat (new_str, cp_found + found_len);
        cp_found = new_str + (cp_found - orig_text) + replace_len;
        orig_text = MemFree (orig_text);
        orig_text = new_str;
      }
      cp_found = StringISearch (cp_found, avp->text_to_replace);
    }
    return orig_text;
  }
}

extern ValNodePtr ApplyValueToValNodeStringList (ValNodePtr list, Int2 choice, ApplyValuePtr avp)
{
  ValNodePtr vnp;
  
  if (avp == NULL)
  {
    return NULL;
  }
  
  if (!StringHasNoText (avp->text_to_replace))
  {
    for (vnp = list; vnp != NULL; vnp = vnp->next)
    {
      vnp->data.ptrvalue = HandleApplyValue (vnp->data.ptrvalue, avp);
    }
  }
  else
  {
    ValNodeAddPointer (&list, choice, StringSave (avp->new_text));
  }
  return list;
}

typedef struct parsefield
{
  Int4        parse_field_type;
  ValNodePtr  feature_field;
  ValNodePtr  feature_subtype;
} ParseFieldData, PNTR ParseFieldPtr;

typedef struct parsefielddialog
{
  DIALOG_MESSAGE_BLOCK
  PopuP                    parse_field_type;
  DialoG                   biosrc_string_choice;
  DialoG                   source_qual_choice;
  DialoG                   gene_field;
  DialoG                   mrna_field;
  DialoG                   protein_field;
  DialoG                   import_feature;
  DialoG                   import_qual;
  DialoG                   feature;
  Nlm_ChangeNotifyProc     change_notify;
  Pointer                  change_userdata;
} ParseFieldDialogData, PNTR ParseFieldDialogPtr;

static void ChangeParseFieldType (PopuP p)
{
  ParseFieldDialogPtr dlg;
  Int4                parse_field_type;

  dlg = (ParseFieldDialogPtr) GetObjectExtra (p);
  if (dlg == NULL)
  {
    return;
  }
  parse_field_type = GetValue (dlg->parse_field_type);
  Hide (dlg->biosrc_string_choice);
  Hide (dlg->source_qual_choice);
  Hide (dlg->gene_field);
  Hide (dlg->mrna_field);
  Hide (dlg->protein_field);
  Hide (dlg->import_feature);
  Hide (dlg->import_qual);
  Hide (dlg->feature);
  switch (parse_field_type)
  {
    case PARSE_FIELD_BIOSRC_STRING:
      Show (dlg->biosrc_string_choice);
      break;
    case PARSE_FIELD_SOURCE_QUAL:
      Show (dlg->source_qual_choice);
      break;
    case PARSE_FIELD_GENE_FIELD:
      Show (dlg->gene_field);
      break;
    case PARSE_FIELD_MRNA_FIELD:
      Show (dlg->mrna_field);
      break;
    case PARSE_FIELD_PROTEIN_FIELD:
      Show (dlg->protein_field);
      break;
    case PARSE_FIELD_IMPORT_QUAL:
      Show (dlg->import_feature);
      Show (dlg->import_qual);
      break;
    case PARSE_FIELD_FEATURE_NOTE:
      Show (dlg->feature);
      break;
  }
  if (dlg->change_notify != NULL)
  {
    (dlg->change_notify)(dlg->change_userdata);
  }
}

static void ResetParseFieldDialog (ParseFieldDialogPtr dlg)
{
  if (dlg == NULL)
  {
    return;
  }
  SetValue (dlg->parse_field_type, PARSE_FIELD_DEFLINE);
  ChangeParseFieldType (dlg->parse_field_type);
}

static void ParseFieldToDialog (DialoG d, Pointer data)
{
  ParseFieldDialogPtr dlg;
  ParseFieldPtr       parse_field;
  Int4                parse_field_type;

  dlg = (ParseFieldDialogPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return;
  }
  ResetParseFieldDialog (dlg);
  parse_field = (ParseFieldPtr) data;
  if (parse_field != NULL)
  {
    parse_field_type = parse_field->parse_field_type;
    if (parse_field_type < PARSE_FIELD_DEFLINE || parse_field_type > MAX_PARSE_FIELD_TYPE)
    {
      parse_field_type = PARSE_FIELD_DEFLINE;
    }
    SetValue (dlg->parse_field_type, parse_field_type);
    switch (parse_field_type)
    {
      case PARSE_FIELD_BIOSRC_STRING:
        PointerToDialog (dlg->biosrc_string_choice, parse_field->feature_field);
        break;
      case PARSE_FIELD_SOURCE_QUAL:
        PointerToDialog (dlg->source_qual_choice, parse_field->feature_field);
        break;
      case PARSE_FIELD_GENE_FIELD:
        PointerToDialog (dlg->gene_field, parse_field->feature_field);
        break;
      case PARSE_FIELD_MRNA_FIELD:
        PointerToDialog (dlg->mrna_field, parse_field->feature_field);
        break;
      case PARSE_FIELD_PROTEIN_FIELD:
        PointerToDialog (dlg->protein_field, parse_field->feature_field);
        break;
      case PARSE_FIELD_IMPORT_QUAL:
        PointerToDialog (dlg->import_feature, parse_field->feature_subtype);
        PointerToDialog (dlg->import_qual, parse_field->feature_field);
        break;
      case PARSE_FIELD_FEATURE_NOTE:
        PointerToDialog (dlg->feature, parse_field->feature_field);
        break;
    }
  }
  ChangeParseFieldType (dlg->parse_field_type);
}

static Pointer DialogToParseField (DialoG d)
{
  ParseFieldDialogPtr dlg;
  ParseFieldPtr       parse_field;

  dlg = (ParseFieldDialogPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return NULL;
  }
  parse_field = (ParseFieldPtr) MemNew (sizeof (ParseFieldData));
  if (parse_field != NULL)
  {
    parse_field->parse_field_type = GetValue (dlg->parse_field_type);
    parse_field->feature_field = NULL;
    switch (parse_field->parse_field_type)
    {
      case PARSE_FIELD_BIOSRC_STRING:
        parse_field->feature_field = DialogToPointer (dlg->biosrc_string_choice);
        break;
      case PARSE_FIELD_SOURCE_QUAL:
        parse_field->feature_field = DialogToPointer (dlg->source_qual_choice);
        break;
      case PARSE_FIELD_GENE_FIELD:
        parse_field->feature_field = DialogToPointer (dlg->gene_field);
        break;
      case PARSE_FIELD_MRNA_FIELD:
        parse_field->feature_field = DialogToPointer (dlg->mrna_field);
        break;
      case PARSE_FIELD_PROTEIN_FIELD:
        parse_field->feature_field = DialogToPointer (dlg->protein_field);
        break;
      case PARSE_FIELD_IMPORT_QUAL:
        parse_field->feature_subtype = DialogToPointer (dlg->import_feature);
        parse_field->feature_field = DialogToPointer (dlg->import_qual);
        break;
      case PARSE_FIELD_FEATURE_NOTE:
        parse_field->feature_field = DialogToPointer (dlg->feature);
        break;
    }
  }
  return parse_field;
}

static void ParseFieldMessage (DialoG d, Int2 mssg)

{
  ParseFieldDialogPtr dlg;

  dlg = (ParseFieldDialogPtr) GetObjectExtra (d);
  if (dlg != NULL) {
    switch (mssg) {
      case VIB_MSG_INIT :
        ResetParseFieldDialog (dlg);        
        break;
      case VIB_MSG_ENTER :
        Select (dlg->parse_field_type);
        break;
      default :
        break;
    }
  }
}

static ValNodePtr TestParseFieldDialog (DialoG d)

{
  ValNodePtr          head = NULL;
  ParseFieldDialogPtr dlg;
  Int4                parse_field_type;
  ValNodePtr          vnp;

  dlg = (ParseFieldDialogPtr) GetObjectExtra (d);
  
  if (dlg != NULL)
  {
    parse_field_type = GetValue (dlg->parse_field_type);
    switch (parse_field_type)
    {
      case PARSE_FIELD_DEFLINE:
      case PARSE_FIELD_CDS_COMMENT:
        /* don't need any extra information */
        break;
      case PARSE_FIELD_BIOSRC_STRING:
        vnp = DialogToPointer (dlg->biosrc_string_choice);
        if (vnp == NULL)
        {
          head = AddStringToValNodeChain (head, "biosrc string type", 1);
        }
        ValNodeFree (vnp);
        break;
      case PARSE_FIELD_SOURCE_QUAL:
        vnp = DialogToPointer (dlg->source_qual_choice);
        if (vnp == NULL)
        {
          head = AddStringToValNodeChain (head, "source qual type", 1);
        }
        ValNodeFree (vnp);
        break;
      case PARSE_FIELD_GENE_FIELD:
        vnp = DialogToPointer (dlg->gene_field);
        if (vnp == NULL || vnp->data.intvalue == FEATUREFIELD_NONE)
        {
          head = AddStringToValNodeChain (head, "gene field", 1);
        }
        ValNodeFree (vnp);
        break;
      case PARSE_FIELD_MRNA_FIELD:
        vnp = DialogToPointer (dlg->mrna_field);
        if (vnp == NULL || vnp->data.intvalue == FEATUREFIELD_NONE)
        {
          head = AddStringToValNodeChain (head, "mRNA field", 1);
        }
        ValNodeFree (vnp);
        break;
      case PARSE_FIELD_PROTEIN_FIELD:
        vnp = DialogToPointer (dlg->protein_field);
        if (vnp == NULL || vnp->data.intvalue == FEATUREFIELD_NONE)
        {
          head = AddStringToValNodeChain (head, "protein field", 1);
        }
        ValNodeFree (vnp);
        break;
      case PARSE_FIELD_IMPORT_QUAL:
        vnp = DialogToPointer (dlg->import_qual);
        if (vnp == NULL)
        {
          ValNodeAddPointer (&head, 1, "import qualifier");
        }
        ValNodeFree (vnp);
        vnp = DialogToPointer (dlg->import_feature);
        if (vnp == NULL)
        {
          ValNodeAddPointer (&head, 1, "import feature");
        }
        ValNodeFree (vnp);
        break;
      case PARSE_FIELD_FEATURE_NOTE:
        vnp = DialogToPointer (dlg->feature);
        if (vnp == NULL)
        {
          ValNodeAddPointer (&head, 1, "feature");
          ValNodeFree (vnp);
        }
        break;
      default:
        head = AddStringToValNodeChain (head, "field type", 1);
        break;
    }
  }      

  return head;
}

extern DialoG ParseFieldDestDialog 
(GrouP                    h,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata)
{
  ParseFieldDialogPtr dlg;
  GrouP               p, g;
  GrouP               subgrp;
  
  dlg = (ParseFieldDialogPtr) MemNew (sizeof (ParseFieldDialogData));
  if (dlg == NULL)
  {
    return NULL;
  }
  
  p = HiddenGroup (h, 2, 0, NULL);
  SetObjectExtra (p, dlg, StdCleanupExtraProc);

  dlg->dialog = (DialoG) p;
  dlg->todialog = ParseFieldToDialog;
  dlg->fromdialog = DialogToParseField;
  dlg->dialogmessage = ParseFieldMessage;
  dlg->testdialog = TestParseFieldDialog;
  dlg->change_notify = change_notify;
  dlg->change_userdata = change_userdata;
  
  dlg->parse_field_type = PopupList (p, TRUE, ChangeParseFieldType);  
  PopupItem (dlg->parse_field_type, "Definition Line");
  PopupItem (dlg->parse_field_type, "Biosource");
  PopupItem (dlg->parse_field_type, "Source Qualifier");
  PopupItem (dlg->parse_field_type, "Gene Field");
  PopupItem (dlg->parse_field_type, "mRNA Field");
  PopupItem (dlg->parse_field_type, "CDS Comment");
  PopupItem (dlg->parse_field_type, "Protein Field");
  PopupItem (dlg->parse_field_type, "Import Feature");
  PopupItem (dlg->parse_field_type, "Feature Note");
  SetValue (dlg->parse_field_type, PARSE_FIELD_DEFLINE);
  SetObjectExtra (dlg->parse_field_type, dlg, NULL);
  
  subgrp = HiddenGroup (p, 0, 0, NULL);
  dlg->biosrc_string_choice = BioSourceStringDialog (subgrp, FALSE, change_notify, change_userdata);
  Hide (dlg->biosrc_string_choice);
  dlg->source_qual_choice = SourceQualTypeSelectionDialog (subgrp, FALSE, change_notify, change_userdata);
  Hide (dlg->source_qual_choice);
  dlg->gene_field = GeneFieldSelectionDialog (subgrp, FALSE, change_notify, change_userdata);
  Hide (dlg->gene_field);
  dlg->mrna_field = MRNAFieldSelectionDialog (subgrp, FALSE, change_notify, change_userdata);
  Hide (dlg->mrna_field);
  dlg->protein_field = ProteinFieldSelectionDialog (subgrp, FALSE, change_notify, change_userdata);
  Hide (dlg->protein_field);
  g = HiddenGroup (subgrp, 2, 0, NULL);
  dlg->import_feature = FeatureSelectionDialog (g, TRUE, change_notify, change_userdata);
  Hide (dlg->import_feature);
  dlg->import_qual = GBQualSelectionDialog (g, FALSE, change_notify, change_userdata);
  Hide (dlg->import_qual);
  dlg->feature = FeatureSelectionDialog (subgrp, TRUE, change_notify, change_userdata);
  Hide (dlg->feature);

  return (DialoG) p;  
}

#define PARSE_FIELD_SRC_DEFLINE          1
#define PARSE_FIELD_SRC_GENBANK_FLATFILE 2
#define PARSE_FIELD_SRC_LOCAL_ID         3
#define PARSE_FIELD_SRC_TAXNAME          4
#define PARSE_FIELD_SRC_COMMENT          5
#define PARSE_FIELD_BANKIT_COMMENT       6
#define MAX_PARSE_FIELD                  6

static CharPtr parse_src_field_list [] = 
{
  "Definition Line", "GenBank Flatfile", "Local ID", "Organism Name", "Comment", "Bankit Comment"  
};

static num_parse_src_fields = sizeof (parse_src_field_list) / sizeof (CharPtr);

extern DialoG ParseFieldSourceDialog
(GrouP                    h,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata)
{
  return FeatureFieldSelectionDialog (h, FALSE,
                                      num_parse_src_fields, parse_src_field_list, 
                                      change_notify, change_userdata);
}

typedef struct sampledialog
{
  DIALOG_MESSAGE_BLOCK
  DoC                doc;
  Int2               start_item;
  Int2               start_row;
  Int2               start_col;
  Int2               end_item;
  Int2               end_row;
  Int2               end_col;
  SetSamplePtr       ssp;
} SampleDialogData, PNTR SampleDialogPtr;

static void TruncateStringAfterTwoLines (CharPtr str, Int4 char_per_line)
{
  CharPtr cp;
  Int4    string_len;
  
  if (StringHasNoText (str))
  {
    return;
  }
  
  string_len = StringLen (str);
  if (string_len < char_per_line)
  {
    return;
  }
  
  cp = str + char_per_line;
  while (! isspace ((Int4)(*cp)) && cp > str)
  {
    cp--;
  }
  if (cp == str)
  {
    if (string_len > 2 * char_per_line)
    {
      str [2 * char_per_line] = 0;
    }
  }
  else
  {
    if (StringLen (cp) > char_per_line)
    {
      cp[char_per_line] = 0;
    }
  }
  
}

typedef struct exportsample
{
  ValNodePtr   row_list;
  SetSamplePtr ssp;
} ExportSampleData, PNTR ExportSamplePtr;

static void ExportSampleDialogCallback (BioseqPtr bsp, Pointer userdata)
{
  ExportSamplePtr   esp;
  SeqFeatPtr        sfp;
  SeqMgrFeatContext fcontext;
  SeqDescrPtr       sdp;
  SeqMgrDescContext dcontext;
  Char              tmp[128];
  CharPtr           val_str = NULL, new_val_str, tmp_val_str;
  ValNodePtr        sample_field, sample_field_next;
  Boolean           found_text = FALSE;
  ValNodePtr        new_row = NULL;
  SeqIdPtr          sip;
  
  if (bsp == NULL || userdata == NULL || ! ISA_na (bsp->mol))
  {
    return;
  }
  
  esp = (ExportSamplePtr) userdata;
  
  sip = SeqIdFindBest (bsp->id, 0);
  SeqIdWrite (sip, tmp, PRINTID_REPORT, sizeof (tmp));
  
  ValNodeAddPointer (&new_row, 0, StringSave (tmp));
  
  for (sample_field = esp->ssp->field_list;
       sample_field != NULL; 
       sample_field = sample_field->next)
  {
    sample_field_next = sample_field->next;
    sample_field->next = NULL;
    val_str = NULL;
  
    if (esp->ssp->descrstring_func != NULL)
    {
      sdp = SeqMgrGetNextDescriptor (bsp, NULL, 0, &dcontext);
      while (sdp != NULL)
      {
        new_val_str = (esp->ssp->descrstring_func) (sdp, sample_field, NULL);
        tmp_val_str = StringAppend (val_str, new_val_str);
        val_str = MemFree (val_str);
        new_val_str = MemFree (new_val_str);
        val_str = tmp_val_str;
        tmp_val_str = NULL;
        sdp = SeqMgrGetNextDescriptor (bsp, sdp, 0, &dcontext);
      }
    }
    if (esp->ssp->fieldstring_func != NULL)
    {
      sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &fcontext);
      while (sfp != NULL)
      {
        new_val_str = (esp->ssp->fieldstring_func)(sfp, sample_field, NULL);
        tmp_val_str = StringAppend (val_str, new_val_str);
        val_str = MemFree (val_str);
        new_val_str = MemFree (new_val_str);
        val_str = tmp_val_str;
        tmp_val_str = NULL;
        sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &fcontext);
      }
    }
    ValNodeAddPointer (&new_row, 0, val_str);
    sample_field->next = sample_field_next;
  }
  ValNodeAddPointer (&(esp->row_list), 0, new_row);
}

static void GetTableDataColumnWidths (ValNodePtr row_list)
{
  ValNodePtr row_vnp, col_vnp, header_vnp;

  if (row_list == NULL || row_list->next == NULL)
  {
    return;
  }
  
  /* only collect widths for data columns, not header, so we can detect empty rows */
  for (row_vnp = row_list->next; row_vnp != NULL; row_vnp = row_vnp->next)
  {
    for (col_vnp = row_vnp->data.ptrvalue, header_vnp = row_list->data.ptrvalue;
         col_vnp != NULL && header_vnp != NULL;
         col_vnp = col_vnp->next, header_vnp = header_vnp->next)
    {
      if (!StringHasNoText (col_vnp->data.ptrvalue))
      {
        header_vnp->choice = MAX (header_vnp->choice, StringLen (col_vnp->data.ptrvalue));
      }
    }
  }
}

static void RemoveTableEmptyColumns (ValNodePtr row_list)
{
  ValNodePtr row_vnp, col_vnp, header_vnp, col_vnp_next, col_vnp_prev;
  ValNodePtr column_list;

  if (row_list == NULL)
  {
    return;
  }
  
  /* remove data columns */
  for (row_vnp = row_list->next; row_vnp != NULL; row_vnp = row_vnp->next)
  {
    column_list = row_vnp->data.ptrvalue;
    col_vnp_prev = NULL;
    for (col_vnp = column_list, header_vnp = row_list->data.ptrvalue;
         col_vnp != NULL && header_vnp != NULL;
         col_vnp = col_vnp_next, header_vnp = header_vnp->next)
    {
      col_vnp_next = col_vnp->next;
      if (header_vnp->choice == 0)
      {
        if (col_vnp_prev == NULL)
        {
          row_vnp->data.ptrvalue = col_vnp_next;
          column_list = row_vnp->data.ptrvalue;
        }
        else
        {
          col_vnp_prev->next = col_vnp_next;
        }
        col_vnp->next = NULL;
        ValNodeFreeData (col_vnp);
      }
      else
      {
        col_vnp_prev = col_vnp;
      }
    }
  }
  
  /* remove headers for empty columns */
  col_vnp_prev = NULL;
  for (col_vnp = row_list->data.ptrvalue;
       col_vnp != NULL;
       col_vnp = col_vnp_next)
  {
    col_vnp_next = col_vnp->next;
    if (col_vnp->choice == 0)
    {
      if (col_vnp_prev == NULL)
      {
        row_list->data.ptrvalue = col_vnp_next;
      }
      else
      {
        col_vnp_prev->next = col_vnp_next;
      }
      col_vnp->next = NULL;
      ValNodeFreeData (col_vnp);
    }
    else
    {
      col_vnp_prev = col_vnp;
    }
  }
}

static void ExportSampleDialogData (SampleDialogPtr dlg)
{
  Char             path [PATH_MAX];
  ExportSampleData esd;
  SeqEntryPtr      sep;
  CharPtr          sample_label;
  ValNodePtr       sample_field;
  FILE             *fp;
  ValNodePtr       new_row = NULL;
  
  if (dlg == NULL || dlg->ssp == NULL || dlg->ssp->label_vn_proc == NULL)
  {
    return;
  }
  
  sep = GetTopSeqEntryForEntityID (dlg->ssp->entityID);
  if (sep == NULL)
  {
    return;
  }
  if (! GetOutputFileName (path, sizeof (path), NULL)) return;
  fp = FileOpen (path, "w");
  if (fp == NULL)
  {
    Message (MSG_ERROR, "Unable to open %s", path);
    return;
  }
  WatchCursor ();
  Update ();
  esd.ssp = dlg->ssp;
  ValNodeAddPointer (&new_row, 0, StringSave ("Sequence"));
    
  for (sample_field = dlg->ssp->field_list;
       sample_field != NULL; 
       sample_field = sample_field->next)
  {
    sample_label = (dlg->ssp->label_vn_proc) (sample_field);
    ValNodeAddPointer (&new_row, 0, StringSave (sample_label));
  }
  esd.row_list = NULL;
  ValNodeAddPointer (&(esd.row_list), 0, new_row);
  
  VisitBioseqsInSep (sep, &esd, ExportSampleDialogCallback);

  GetTableDataColumnWidths (esd.row_list);
  RemoveTableEmptyColumns (esd.row_list);

  PrintTableDisplayRowListToFile (esd.row_list, fp);
  esd.row_list = FreeTableDisplayRowList (esd.row_list);
  
  FileClose (fp);
  ArrowCursor();
  Update();
}

static void RefreshSampleDialog (SampleDialogPtr dlg)
{
  SeqEntryPtr     sep;
  RecT            r;
  ParData         ParFmt = {FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0};
  ColData         ColFmt[] = 
  {
    {0, 0, 80, 0, NULL, 'l', TRUE, FALSE, FALSE, FALSE, FALSE},
    {0, 0, 80, 0, NULL, 'l', TRUE, FALSE, FALSE, FALSE, FALSE},
    {0, 0, 80, 0, NULL, 'l', TRUE, FALSE, FALSE, FALSE, TRUE}
  };
  GetSampleData   gsd;
  CharPtr         new_line;
  ValNodePtr      sample_field, sample_field_next;
  CharPtr         sample_label = NULL;
  Int4            max_char_per_line;
  FonT            currentFont=NULL;

  if (dlg == NULL)
  {
    return;
  }
  Reset (dlg->doc);
  if (dlg->ssp == NULL || dlg->ssp->field_list == NULL
      || (dlg->ssp->fieldstring_func == NULL && dlg->ssp->descrstring_func == NULL))
  {
    return;
  }
  
  sep = GetTopSeqEntryForEntityID (dlg->ssp->entityID);
  if (sep == NULL)
  {
    return;
  }
  gsd.sample_text = NULL;
  gsd.fieldstring_func = dlg->ssp->fieldstring_func;
  gsd.descrstring_func = dlg->ssp->descrstring_func;
    
  ObjectRect (dlg->doc, &r);
  InsetRect (&r, 4, 4);
  ColFmt[0].pixWidth = (r.right - r.left) / 3;
  ColFmt[1].pixWidth = (r.right - r.left) / 4;
  ColFmt[2].pixWidth = (r.right - r.left)  - ColFmt[0].pixWidth - ColFmt[1].pixWidth;

  SelectFont (programFont);
  max_char_per_line = ColFmt[2].pixWidth / CharWidth ('0');
  
  
  for (sample_field = dlg->ssp->field_list; sample_field != NULL; sample_field = sample_field->next)
  {
    sample_field_next = sample_field->next;
    sample_field->next = NULL;
    gsd.requested_field = sample_field;
    gsd.num_found = 0;
    gsd.all_same = TRUE;
    OperateOnSeqEntryConstrainedObjects (sep, dlg->ssp->fsp, GetSampleFeatureCallback, 
                                         GetSampleDescriptorCallback,
                                         0, 0, 0, &gsd);
    if (gsd.num_found > 0)
    {
      sample_label = (dlg->ssp->label_vn_proc) (sample_field);
      new_line = (CharPtr) MemNew (
           (StringLen (sample_label) 
            + StringLen (gsd.sample_text) + 20)
            * sizeof (Char));
      if (new_line != NULL)
      {
        if (gsd.all_same)
        {
          sprintf (new_line, "%s\t(%d same)\t", sample_label, gsd.num_found);
        }
        else
        {
          sprintf (new_line, "%s\t(%d mixed)\t", sample_label, gsd.num_found);
        }
        if (!StringHasNoText (gsd.sample_text))
        {
          TruncateStringAfterTwoLines (gsd.sample_text, max_char_per_line);
          StringCat (new_line, gsd.sample_text);
        }
        StringCat (new_line, "\n");
      	AppendText (dlg->doc, new_line, &ParFmt, ColFmt, programFont);
      	MemFree (new_line);
      }
      sample_label = MemFree (sample_label);
    }
    gsd.sample_text = MemFree (gsd.sample_text);
    sample_field->next = sample_field_next;
  }
  InvalDocument (dlg->doc);
}

static void PopulateSampleDialog (DialoG d, Pointer userdata)
{
  SampleDialogPtr dlg;
  SetSamplePtr    ssp;
  
  dlg = (SampleDialogPtr) GetObjectExtra (d);
  if (dlg == NULL || userdata == NULL)
  {
    return;
  }
  ssp = (SetSamplePtr) userdata;
  dlg->ssp = SetSampleFree (dlg->ssp);
  dlg->ssp = SetSampleCopy (ssp);
  RefreshSampleDialog (dlg);
}

static void 
GetSampleDialogStartAndEnd 
(SampleDialogPtr dlg,
 Int2Ptr         start_item,
 Int2Ptr         start_row,
 Int2Ptr         start_col,
 Int2Ptr         end_item,
 Int2Ptr         end_row,
 Int2Ptr         end_col)
{
  if (dlg == NULL || start_item == NULL || start_row == NULL
      || start_col == NULL || end_item == NULL
      || end_row == NULL || end_col == NULL)
  {
    return;
  }
  
  if (dlg->start_item < 0)
  {
    *start_item = -1;
    *start_row = -1;
    *start_col = -1;
    *end_item = -1;
    *end_row = -1;
    *end_col = -1;
  }
  else if (dlg->end_item < 0)
  {
    *start_item = dlg->start_item;
    *start_row = dlg->start_row;
    *start_col = dlg->start_col;
    *end_item = *start_item;
    *end_row = *start_row;
    *end_col = *start_col;
  }
  else if (dlg->start_item <= dlg->end_item)
  {
    *start_item = dlg->start_item;
    *end_item = dlg->end_item;
    if (dlg->start_row <= dlg->end_row)
    {
      *start_row = dlg->start_row;
      *end_row = dlg->end_row;
      if (dlg->start_col <= dlg->end_col)
      {
        *start_col = dlg->start_col;
        *end_col = dlg->end_col;
      }
      else
      {
        *start_col = dlg->end_col;
        *end_col = dlg->start_col;
      }
    }
    else
    {
      *start_row = dlg->end_row;
      *start_col = dlg->end_col;
      *end_row = dlg->start_row;
      *end_col = dlg->start_col;
    }
  }
  else
  {
    *start_item = dlg->end_item;
    *start_row = dlg->end_row;
    *start_col = dlg->end_col;
    *end_item = dlg->start_item;
    *end_row = dlg->start_row;
    *end_col = dlg->start_col;
  }
  
}

static void SampleDialogCopy (SampleDialogPtr dlg)
{
  Int2       start_row = 0, end_row = 0, tmp_row, first_row, last_row;
  Int2       start_col = 0, end_col = 0, first_col, last_col;
  Int2       start_item = 0, end_item = 0, tmp_item;
  CharPtr    str;
  ValNodePtr strings = NULL;
  Int2       num_rows, num_cols;
  
  if (dlg->start_row < 0)
  {
    return;
  }
  
  GetSampleDialogStartAndEnd (dlg, &start_item, &start_row, &start_col,
                              &end_item, &end_row, &end_col);

  first_row = start_row;
  first_col = start_col;
  for (tmp_item = start_item; tmp_item <= end_item; tmp_item++)
  {
    GetItemParams (dlg->doc, tmp_item, NULL, &num_rows, &num_cols, NULL, NULL);
    if (tmp_item == end_item)
    {
      last_row = end_row;
    }
    else
    {
      last_row = num_rows;
    }
    for (tmp_row = first_row; tmp_row <= last_row; tmp_row++)
    {
      if (tmp_row == last_row && tmp_item == end_item)
      {
        last_col = end_col;
      }
      else
      {
        last_col = num_cols;
      }
      str = GetDocText (dlg->doc, tmp_item, tmp_row, 3);
      ValNodeAddPointer (&strings, 0, str);
    }
    first_row = 1;
  }
  str = MergeValNodeStrings (strings, FALSE);
  
  StringToClipboard (str);
  MemFree (str);
  strings = ValNodeFreeData (strings);
}

static void InvalidateSampleDialogRows (DoC d, Int2 start_item, Int2 end_item)
{
  Int2 num_rows;
  if (d == NULL)
  {
    return;
  }
  if (start_item < 1)
  {
    start_item = 1;
  }
  if (end_item < 1)
  {
    end_item = start_item;
  }
  while (start_item <= end_item)
  {
    GetItemParams (d, start_item, NULL, &num_rows, NULL, NULL, NULL);
    InvalDocRows (d, start_item, 1, num_rows); 
    start_item++;       
  }
}

static void SampleDialogOnClick (DoC d, PoinT pt)
{
  SampleDialogPtr   dlg;
  Int2              pos_item, pos_row, pos_col;
  Int2              start_item = 0, start_row, start_col;
  Int2              end_item = -1, end_row, end_col;
  
  dlg = (SampleDialogPtr) GetObjectExtra (d);
  if (dlg == NULL) return;
  
  MapDocPoint (d, pt, &(pos_item), &(pos_row), &(pos_col), NULL);
  if (dlg->start_item == pos_item
      && dlg->start_row == pos_row 
      && dlg->start_col == pos_col)
  {
    dlg->start_row = -1;
    dlg->start_col = -1;
    
  }
  else
  {
    if (dlg->start_item > -1)
    {
      GetSampleDialogStartAndEnd (dlg, &start_item, &start_row, &start_col,
                                  &end_item, &end_row, &end_col);
    }
    dlg->start_item = pos_item;
    dlg->start_row = pos_row;
    dlg->start_col = pos_col;
  }
  dlg->end_item = -1;
  dlg->end_row = -1;
  dlg->end_col = -1;
  InvalidateSampleDialogRows (d, start_item, end_item);
  InvalidateSampleDialogRows (d, pos_item, pos_item);
}

static void SampleDialogOnDrag (DoC d, PoinT pt)
{
  SampleDialogPtr   dlg;
  
  dlg = (SampleDialogPtr) GetObjectExtra (d);
  if (dlg == NULL) return;
  
  MapDocPoint (d, pt, &(dlg->end_item), &(dlg->end_row), &(dlg->end_col), NULL);
  InvalDocument (d);
}

static void SampleDialogOnRelease (DoC d, PoinT pt)
{
  SampleDialogPtr   dlg;
  
  dlg = (SampleDialogPtr) GetObjectExtra (d);
  if (dlg == NULL) return;
  
  MapDocPoint (d, pt, &(dlg->end_item), &(dlg->end_row), &(dlg->end_col), NULL);
  InvalDocument (d);
}

static Boolean SampleDialogInvert (DoC d, Int2 item, Int2 row, Int2 col)
{
  SampleDialogPtr   dlg;
  Int2              start_item = 0, start_row = 0, start_col = 0;
  Int2              end_item = 0, end_row = 0, end_col = 0;
  Boolean           rval = FALSE;
  
  dlg = (SampleDialogPtr) GetObjectExtra (d);
  if (dlg == NULL) return FALSE;
  
  if (dlg->start_row < 0)
  {
    return FALSE;
  }
  
  if (dlg->end_item == -1)
  {
    if(dlg->start_item == item
       && dlg->start_row == row
       && dlg->start_col == col)
    {
      return TRUE;
    }
    else
    {
      return FALSE;
    }
  }
  GetSampleDialogStartAndEnd (dlg, &start_item, &start_row, &start_col,
                              &end_item, &end_row, &end_col);
  if (start_item <= item && end_item >= item)
  {
    rval = TRUE;
    if (start_item == item)
    {
      if (start_row == row)
      {
        if (start_col > col)
        {
          rval = FALSE;
        }
      }
      else if (start_row > row)
      {
        rval = FALSE;
      }
    }
    
    if (end_item == item)
    {
      if (end_row == row)
      {
        if (end_col < col)
        {
          rval = FALSE;
        }
      }
      else if (end_row < row)
      {
        rval = FALSE;
      }
    }
  }
  if (col == 3)
  {
    return rval;
  }
  else
  {
    return FALSE;
  }
}

static void SampleDialogOnKey (SlatE s, Char ch)
{
  DoC             d;
  SampleDialogPtr dlg;

  d = (DoC) s;
  Select (d);
  dlg = (SampleDialogPtr) GetObjectExtra (d);
  if (dlg == NULL) return;
  CaptureSlateFocus ((SlatE) s);
  
#ifdef WIN_MSWIN
  if (ch == 3)
  {
    SampleDialogCopy (dlg);
  }
#else
  if (ctrlKey)
  {
    if (ch == 'c')
    {
      SampleDialogCopy (dlg);
    }
  }
#endif
}

static void SampleDialogMessage (DialoG d, Int2 mssg)

{
  SampleDialogPtr dlg;

  dlg = (SampleDialogPtr) GetObjectExtra (d);
  if (dlg != NULL) {
    switch (mssg) {
      case VIB_MSG_INIT :
        /* reset list */
        RefreshSampleDialog (dlg);
        break;
      default :
        break;
    }
  }
}

static void CleanupSampleDialog (GraphiC g, VoidPtr data)
{
  SampleDialogPtr dlg;

  dlg = (SampleDialogPtr) data;
  if (dlg != NULL) {
    dlg->ssp = SetSampleFree (dlg->ssp);
  }
  StdCleanupExtraProc (g, data);
}

static void RefreshSampleBtn (ButtoN b)
{
  SampleDialogPtr dlg;
  
  dlg = (SampleDialogPtr) GetObjectExtra (b);
  RefreshSampleDialog (dlg);  
}

static void ExportSampleBtn (ButtoN b)
{
  SampleDialogPtr dlg;
  
  dlg = (SampleDialogPtr) GetObjectExtra (b);
  ExportSampleDialogData (dlg);  
}

extern DialoG SampleDialog (GrouP h)
{
  SampleDialogPtr dlg;
  GrouP           p, g;
  ButtoN          b;
  
  dlg = (SampleDialogPtr) MemNew (sizeof (SampleDialogData));
  if (dlg == NULL) 
  {
    return NULL;
  }
  
  p = HiddenGroup (h, -1, 0, NULL);

  SetObjectExtra (p, dlg, CleanupSampleDialog);

  dlg->dialog = (DialoG) p;
  dlg->todialog = PopulateSampleDialog;
  dlg->fromdialog = NULL;
  dlg->dialogmessage = SampleDialogMessage;
  dlg->testdialog = NULL;

  dlg->doc = DocumentPanel (p, stdCharWidth * 27, stdLineHeight * 8);
  SetObjectExtra (dlg->doc, dlg, NULL);
  SetDocProcs (dlg->doc, SampleDialogOnClick, SampleDialogOnDrag, SampleDialogOnRelease, NULL);  
  SetDocShade (dlg->doc, NULL, NULL, SampleDialogInvert, NULL);
  SetDocAutoAdjust (dlg->doc, TRUE);
  SetSlateChar ((SlatE) dlg->doc, SampleDialogOnKey);
  
  g = HiddenGroup (p, 2, 0, NULL);
  b = PushButton (g, "Refresh Qualifiers", RefreshSampleBtn);
  SetObjectExtra (b, dlg, NULL);
  b = PushButton (g, "Export Qualifiers", ExportSampleBtn);
  SetObjectExtra (b, dlg, NULL);  
  
  AlignObjects (ALIGN_CENTER, (HANDLE) dlg->doc, (HANDLE) g, NULL);
  
  return (DialoG) p;  
}

typedef struct featurefieldchoicedialog 
{
  DIALOG_MESSAGE_BLOCK
  DialoG first_choice;
  DialoG second_choice;
  DialoG third_choice;
  ButtoN remove_first;
  ButtoN remove_second;
  ButtoN remove_third;

  Nlm_ChangeNotifyProc     change_notify;
  Pointer                  change_userdata;      
} FeatureFieldChoiceDialogData, PNTR FeatureFieldChoiceDialogPtr;

static void FeatureFieldChoiceChange (Pointer data)
{
  FeatureFieldChoiceDialogPtr dlg;
  ValNodePtr                  vnp;

  dlg = (FeatureFieldChoiceDialogPtr) data;
  if (dlg == NULL)
  {
    return;
  }
  
  Disable (dlg->remove_first);
  Disable (dlg->remove_second);
  Disable (dlg->remove_third);
  
  Disable (dlg->second_choice);
  Disable (dlg->third_choice);
  
  vnp = DialogToPointer (dlg->first_choice);
  if (vnp != NULL)
  {
    ValNodeFree (vnp);
    Enable (dlg->second_choice);
    Enable (dlg->remove_first);
    vnp = DialogToPointer (dlg->second_choice);
    if (vnp != NULL)
    {
      ValNodeFree (vnp);
      Enable (dlg->third_choice);
      Enable (dlg->remove_second);
      vnp = DialogToPointer (dlg->third_choice);
      if (vnp != NULL)
      {
        ValNodeFree (vnp);
        Enable (dlg->remove_third);
      }
    }
  }
  if (dlg->change_notify != NULL)
  {
    (dlg->change_notify)(dlg->change_userdata);
  }
}

static void ResetFeatureFieldChoiceDialog (FeatureFieldChoiceDialogPtr dlg)
{
  if (dlg == NULL) 
  {
    return;
  }
  PointerToDialog (dlg->first_choice, NULL);
  PointerToDialog (dlg->second_choice, NULL);
  PointerToDialog (dlg->third_choice, NULL);
  if (dlg->remove_first != NULL)
  {
    SetStatus (dlg->remove_first, FALSE);
  }
  if (dlg->remove_second != NULL)
  {
    SetStatus (dlg->remove_second, FALSE);
  }
  if (dlg->remove_third != NULL)
  {
    SetStatus (dlg->remove_third, FALSE);
  }
  FeatureFieldChoiceChange (dlg);
}

static void FeatureFieldChoiceToDialog (DialoG d, Pointer data)
{
  FeatureFieldChoiceDialogPtr dlg;
  ValNodePtr                  feature_field;
  
  dlg = (FeatureFieldChoiceDialogPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return;
  }
  
  ResetFeatureFieldChoiceDialog (dlg);
  feature_field = (ValNodePtr) data;
  if (feature_field != NULL)
  {
    PointerToDialog (dlg->first_choice, feature_field);
    if (feature_field->choice != 0 && dlg->remove_first != NULL)
    {
      SetStatus (dlg->remove_first, TRUE);
    }
    feature_field = feature_field->next;
    if (feature_field != NULL)
    {
      PointerToDialog (dlg->second_choice, feature_field);
      if (feature_field->choice != 0 && dlg->remove_second != NULL)
      {
        SetStatus (dlg->remove_second, TRUE);
      }
      feature_field = feature_field->next;
      if (feature_field != NULL)
      {
        if (feature_field->choice != 0 && dlg->remove_third != NULL)
        {
          SetStatus (dlg->remove_third, TRUE);
        }
        PointerToDialog (dlg->third_choice, feature_field);
      }
    }
  }
  FeatureFieldChoiceChange (dlg);
}

static Pointer DialogToFeatureFieldChoice (DialoG d)
{
  FeatureFieldChoiceDialogPtr dlg;
  ValNodePtr                  choice_list = NULL;
  ValNodePtr                  feature_field, vnp_prev;

  dlg = (FeatureFieldChoiceDialogPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return NULL;
  }
  
  feature_field = DialogToPointer (dlg->first_choice);
  if (feature_field != NULL)
  {
    if (dlg->remove_first != NULL && GetStatus (dlg->remove_first))
    {
      feature_field->choice = 1;
    }
    choice_list = feature_field;
    
    feature_field = DialogToPointer (dlg->second_choice);
    if (feature_field != NULL)
    {
      if (dlg->remove_second != NULL && GetStatus (dlg->remove_second))
      {
        feature_field->choice = 1;
      }
      choice_list->next = feature_field;
      vnp_prev = feature_field;
      
      feature_field = DialogToPointer (dlg->third_choice);
      if (feature_field != NULL)
      {
        if (dlg->remove_third != NULL && GetStatus (dlg->remove_third))
        {
          feature_field->choice = 1;
        }
        vnp_prev->next = feature_field;
      }
      else
      {
        ValNodeFree (feature_field);
      }
    }
    else
    {
      ValNodeFree (feature_field);
    }
  }
  else
  {
    ValNodeFree (feature_field);
  }
  return choice_list;
}

static void FeatureFieldChoiceMessage (DialoG d, Int2 mssg)

{
  FeatureFieldChoiceDialogPtr  dlg;

  dlg = (FeatureFieldChoiceDialogPtr) GetObjectExtra (d);
  if (dlg != NULL) {
    switch (mssg) {
      case VIB_MSG_INIT :
        /* reset list */
        ResetFeatureFieldChoiceDialog (dlg);
        break;
      case VIB_MSG_ENTER :
        Select (dlg->first_choice);
        break;
      default :
        break;
    }
  }
}

static ValNodePtr TestFeatureFieldChoice (DialoG d)

{
  FeatureFieldChoiceDialogPtr dlg;
  ValNodePtr                  head = NULL;
  ValNodePtr                  vnp;

  dlg = (FeatureFieldChoiceDialogPtr) GetObjectExtra (d);
  vnp = DialogToPointer (d);
  if (vnp == NULL)
  {
    head = AddStringToValNodeChain (head, "feature field choice", 1);
  }
  else
  {
    ValNodeFree (vnp);
  }
  return head;  
}

extern DialoG FeatureFieldChoiceDialog 
(GrouP h,
 FeatureFieldSelectionProc make_fieldlist_dlg,
 Boolean                   offer_to_remove,
 Nlm_ChangeNotifyProc      change_notify,
 Pointer                   change_userdata)
{
  FeatureFieldChoiceDialogPtr  dlg;
  GrouP                     p, g;

  if (make_fieldlist_dlg == NULL)
  {
    return NULL;
  }
  
  dlg = (FeatureFieldChoiceDialogPtr) MemNew (sizeof (FeatureFieldChoiceDialogData));
  if (dlg == NULL)
  {
    return NULL;
  }

  p = HiddenGroup (h, 0, 6, NULL);
  SetObjectExtra (p, dlg, StdCleanupExtraProc);

  dlg->dialog = (DialoG) p;
  dlg->todialog = FeatureFieldChoiceToDialog;
  dlg->fromdialog = DialogToFeatureFieldChoice;
  dlg->dialogmessage = FeatureFieldChoiceMessage;
  dlg->testdialog = TestFeatureFieldChoice;
  dlg->change_notify = change_notify;
  dlg->change_userdata = change_userdata;

  g = HiddenGroup (p, 3, 0, NULL);
  StaticPrompt (g, "1st Choice", 0, 0, programFont, 'c');
  dlg->first_choice = (make_fieldlist_dlg) (g, TRUE, FeatureFieldChoiceChange, dlg);
  if (offer_to_remove)
  {
    dlg->remove_first = CheckBox (g, "Remove if used", NULL);
    Disable (dlg->remove_first);
  }
  else
  {
    dlg->remove_first = NULL;
  }
  
  g = HiddenGroup (p, 3, 0, NULL);
  StaticPrompt (g, "2nd Choice", 0, 0, programFont, 'c');
  dlg->second_choice = (make_fieldlist_dlg) (g, TRUE, FeatureFieldChoiceChange, dlg);
  if (offer_to_remove)
  {
    dlg->remove_second = CheckBox (g, "Remove if used", NULL);
    Disable (dlg->remove_second);
  }
  else
  {
    dlg->remove_second = NULL;
  }
  Disable (dlg->second_choice);
  
  g = HiddenGroup (p, 3, 0, NULL);
  StaticPrompt (g, "3rd Choice", 0, 0, programFont, 'c');
  dlg->third_choice = (make_fieldlist_dlg) (g, TRUE, FeatureFieldChoiceChange, dlg);
  if (offer_to_remove)
  {
    dlg->remove_third = CheckBox (g, "Remove if used", NULL);
    Disable (dlg->remove_third);
  }
  else
  {
    dlg->remove_third = NULL;
  }
  Disable (dlg->third_choice);

  return (DialoG) p;
}


typedef struct parseaction
{
  TextPortionPtr tp;
  FilterSetPtr   fsp;
  Int4           src_field;

  SeqDescrPtr    sdp;
  SeqFeatPtr     sfp;
  SeqEntryPtr    biop_sep;
  SeqEntryPtr    title_sep;
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  SeqEntryPtr    sep;
} ParseActionData, PNTR ParseActionPtr;

typedef  void  (*ParseActnProc) PROTO((ParseActionPtr, Pointer, CharPtr, Int4));

typedef struct parseactionop
{
  ParseActionPtr  parse_action_data;
  
  ParseActnProc   parse_action;
  Pointer         parse_userdata;
  Boolean         parse_complete;
  
} ParseActionOpData, PNTR ParseActionOpPtr;

static void ResetParseActionVisitData (ParseActionPtr pap)
{
  if (pap != NULL)
  {
    pap->sdp = NULL;
    pap->sfp = NULL;
    pap->bsp = NULL;
    pap->bssp = NULL;
    pap->sep = NULL;
    pap->title_sep = NULL;
    pap->biop_sep = NULL;
  }
}

static void DoParseAction (CharPtr src_str, ParseActionOpPtr paop)
{
  CharPtr          found_loc = NULL;
  Int4             found_len = 0;

  if (StringHasNoText (src_str) || paop == NULL 
      || paop->parse_action_data == NULL
      || paop->parse_action == NULL)
  {
    return;
  }
  
  FindTextPortionInString (src_str, paop->parse_action_data->tp, &found_loc, &found_len);
  
  if (found_loc != NULL)
  {
    (paop->parse_action) (paop->parse_action_data, paop->parse_userdata, found_loc, found_len);
    paop->parse_complete = TRUE;
  }
  
}

static void ParseActionDeflineText (SeqDescrPtr sdp, Pointer userdata)
{
  ParseActionOpPtr paop;
  CharPtr          tmp_string;
  Int4             last_pos;
  
  paop = (ParseActionOpPtr) userdata;
  if (paop == NULL || paop->parse_action_data == NULL || paop->parse_action == NULL
      || sdp == NULL || sdp->choice != Seq_descr_title)
  {
    return;
  }
  
  tmp_string = StringSave (sdp->data.ptrvalue);
  if (tmp_string != NULL)
  {
    last_pos = StringLen (tmp_string) - 1;
    if (tmp_string [last_pos] == '.')
    {
      tmp_string [last_pos] = 0;
    }
  }
  DoParseAction (tmp_string, paop);
  tmp_string = MemFree (tmp_string);
}

static void ParseActionTaxnameText (BioSourcePtr biop, Pointer userdata)
{
  ParseActionOpPtr paop;
  
  paop = (ParseActionOpPtr) userdata;
  if (paop == NULL || paop->parse_action_data == NULL || paop->parse_action == NULL
      || biop == NULL || biop->org == NULL)
  {
    return;
  }
  
  if (paop->parse_action_data->fsp != NULL
      && ! DoesOneSourceMatchConstraint (biop, paop->parse_action_data->fsp->ccp))
  {
    return;
  }
  
  DoParseAction (biop->org->taxname, paop);
}

static void ParseActionLocalIDText (BioseqPtr bsp, Pointer userdata)
{
  ParseActionOpPtr paop;
  SeqIdPtr         sip;
  ObjectIdPtr      oip;
  Char             tmp_str [128];
  
  paop = (ParseActionOpPtr) userdata;
  if (paop == NULL || paop->parse_action_data == NULL || paop->parse_action == NULL
      || bsp == NULL || ISA_aa (bsp->mol))
  {
    return;
  }
  
  for (sip = bsp->id; sip != NULL; sip = sip->next)
  {
    if (sip->choice == SEQID_LOCAL && sip->data.ptrvalue != NULL)
    {
      oip = (ObjectIdPtr) sip->data.ptrvalue;
      if (oip->str == NULL)
      {
        sprintf (tmp_str, "%d", oip->id);
        DoParseAction (tmp_str, paop);
      }
      else
      {
        DoParseAction (oip->str, paop);
      }
    }
  }
}

static void ParseActionBankitComment (SeqDescrPtr sdp, Pointer userdata)
{
  ParseActionOpPtr paop;
  UserObjectPtr    uop;
  ObjectIdPtr      oip;
  UserFieldPtr     ufp;
  
  if (sdp == NULL || userdata == NULL || sdp->data.ptrvalue == NULL)
  {
    return;
  }
  
  paop = (ParseActionOpPtr) userdata;
  if (paop == NULL || paop->parse_action_data == NULL || paop->parse_action == NULL)
  {
    return;
  }

  if (sdp->choice == Seq_descr_user && sdp->extended != 0)
  {
    /* Bankit Comments */
    uop = (UserObjectPtr) sdp->data.ptrvalue;
    oip = uop->type;
    if (oip != NULL && StringCmp (oip->str, "Submission") == 0)
    {
      for (ufp = uop->data; ufp != NULL; ufp = ufp->next)
      {
        oip = ufp->label;
        if (oip != NULL && StringCmp (oip->str, "AdditionalComment") == 0)
        {
          DoParseAction (ufp->data.ptrvalue, paop);
        }
      }
    }
  }
}

static void ParseActionCommentDescriptor (SeqDescrPtr sdp, Pointer userdata)
{
  ParseActionOpPtr paop;
  UserObjectPtr    uop;
  ObjectIdPtr      oip;
  UserFieldPtr     ufp;
  
  if (sdp == NULL || userdata == NULL || sdp->data.ptrvalue == NULL)
  {
    return;
  }
  
  paop = (ParseActionOpPtr) userdata;
  if (paop == NULL || paop->parse_action_data == NULL || paop->parse_action == NULL)
  {
    return;
  }

  if (sdp->choice == Seq_descr_comment)
  {
    DoParseAction (sdp->data.ptrvalue, paop);
  }
  else if (sdp->choice == Seq_descr_user && sdp->extended != 0)
  {
    /* Bankit Comments */
    uop = (UserObjectPtr) sdp->data.ptrvalue;
    oip = uop->type;
    if (oip != NULL && StringCmp (oip->str, "Submission") == 0)
    {
      for (ufp = uop->data; ufp != NULL; ufp = ufp->next)
      {
        oip = ufp->label;
        if (oip != NULL && StringCmp (oip->str, "AdditionalComment") == 0)
        {
          DoParseAction (ufp->data.ptrvalue, paop);
        }
      }
    }
  }
}

static void ParseActionCommentFeature (SeqFeatPtr sfp, Pointer userdata)
{
  ParseActionOpPtr paop;

  if (sfp == NULL || sfp->data.choice != SEQFEAT_COMMENT || userdata == NULL)
  {
    return;
  }
  paop = (ParseActionOpPtr) userdata;
  DoParseAction (sfp->comment, paop);
}

static void SetSourceAndTitle (ParseActionPtr pap, SeqDescrPtr sdp, SeqAnnotPtr sap)
{
  SeqFeatPtr sfp;
  BioseqPtr  bsp;
  
  if (pap == NULL)
  {
    return;
  }
  while (sdp != NULL)
  {
    if (sdp->choice == Seq_descr_source)
    {
      pap->biop_sep = pap->sep;
    }
    else if (sdp->choice == Seq_descr_title)
    {
      if (IS_Bioseq (pap->sep))
      {
        bsp = (BioseqPtr) (pap->sep->data.ptrvalue);
        if (bsp != NULL && ! ISA_aa (bsp->mol))
        {
          pap->title_sep = pap->sep;
        }
      }
      else
      {
        pap->title_sep = pap->sep;
      }
    }
    sdp = sdp->next;
  }
  while (sap != NULL)
  {
    if (sap->type == 1)
    {
      sfp = sap->data;
      while (sfp != NULL)
      {
        if (sfp->data.choice == SEQFEAT_BIOSRC)
        {
          pap->biop_sep = pap->sep;
        }
        sfp = sfp->next;
      }
    }
    sap = sap->next;
  }
}

static void ParseFromSource (BioSourcePtr biop, ParseActionPtr pap, ParseActionOpPtr paop)
{
  if (biop == NULL || pap == NULL || paop == NULL)
  {
    return;
  }

  if (pap->src_field == PARSE_FIELD_SRC_TAXNAME)
  {
    ParseActionTaxnameText (biop, paop);
  }
}

static void 
VisitSeqEntriesForParseAction 
(SeqEntryPtr sep,
 ParseActionPtr pap,
 ParseActnProc   parse_action,
 Pointer        parse_data,
 Boolean        ok_to_parse)
{
  SeqDescrPtr       sdp, sdp_tmp;
  SeqAnnotPtr       sap, sap_tmp;
  SeqFeatPtr        sfp;
  ParseActionOpData paod;
  BioseqSetPtr      bssp = NULL;
  
  SeqEntryPtr       old_title_sep;
  SeqEntryPtr       old_biop_sep;
  
  if (sep == NULL || pap == NULL || parse_action == NULL) 
  {
    return;
  }
  
  paod.parse_action_data = pap;
  paod.parse_action = parse_action;
  paod.parse_userdata = parse_data;
  paod.parse_complete = FALSE;
  
  pap->sep = sep;
  if (IS_Bioseq (pap->sep))
  {
    pap->bsp = (BioseqPtr) pap->sep->data.ptrvalue;
    sdp = pap->bsp->descr;
    sap = pap->bsp->annot;
  }
  else if (IS_Bioseq_set (sep))
  {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    pap->bssp = bssp;
    sdp = pap->bssp->descr;
    sap = pap->bssp->annot;
  }
  
  old_title_sep = pap->title_sep;
  old_biop_sep = pap->biop_sep;
  
  if (pap->fsp != NULL
      && pap->fsp->ccp != NULL 
      && pap->fsp->ccp->constraint_type != CHOICE_CONSTRAINT_ANY)
  {
    ok_to_parse = FALSE;
    /* try to find BioSource descriptor to match source constraint */
    sdp_tmp = sdp;
    while (sdp_tmp != NULL && ! ok_to_parse)
    {
      if (sdp_tmp->choice == Seq_descr_source)
      {
        if (DoesOneSourceMatchConstraint (sdp_tmp->data.ptrvalue, pap->fsp->ccp))
        {
          ok_to_parse = TRUE;
        }
      }
      sdp_tmp = sdp_tmp->next;
    }
    /* try to find BioSource feature to match source constraint */
    sap_tmp = sap;
    while (sap_tmp != NULL && ! ok_to_parse)
    {
      if (sap_tmp->type == 1)
      {
        sfp = (SeqFeatPtr) sap_tmp->data;
        while (sfp != NULL && !ok_to_parse)
        {
          if (sfp->data.choice == SEQFEAT_BIOSRC)
          {
            if (DoesOneSourceMatchConstraint (sfp->data.value.ptrvalue, pap->fsp->ccp))
            {
              ok_to_parse = TRUE;
            }
          }
          sfp = sfp->next;
        }
      }
      sap_tmp = sap_tmp->next;
    }
  }
  else
  {
    ok_to_parse = TRUE;
  }
  if (ok_to_parse)
  {
    SetSourceAndTitle (pap, sdp, sap);
    if (pap->biop_sep != old_biop_sep)
    {
      sdp_tmp = sdp;
      while (sdp_tmp != NULL)
      {
        if (sdp_tmp->choice == Seq_descr_source)
        {
          ParseFromSource (sdp_tmp->data.ptrvalue, pap, &paod);
        }
        sdp_tmp = sdp_tmp->next;
      }
      sap_tmp = sap;
      while (sap_tmp != NULL)
      {
        if (sap_tmp->type == 1)
        {
          sfp = sap_tmp->data;
          while (sfp != NULL)
          {
            if (sfp->data.choice == SEQFEAT_BIOSRC)
            {
              ParseFromSource (sfp->data.value.ptrvalue, pap, &paod);
            }
            sfp = sfp->next;
          }
        }
        sap_tmp = sap_tmp->next;
      }
    }
    if (pap->title_sep != old_title_sep 
        && pap->src_field == PARSE_FIELD_SRC_DEFLINE)
    {
      sdp_tmp = sdp;
      while (sdp_tmp != NULL)
      {
        if (sdp_tmp->choice == Seq_descr_title)
        {
          ParseActionDeflineText (sdp_tmp, &paod);
        }
        sdp_tmp = sdp_tmp->next;
      }
    }
      
    if (pap->src_field == PARSE_FIELD_SRC_LOCAL_ID && IS_Bioseq (sep))
    {
      if (pap->biop_sep == NULL)
      {
        pap->biop_sep = sep;
      }
      if (pap->title_sep == NULL)
      {
        pap->title_sep = sep;
      }
      ParseActionLocalIDText (sep->data.ptrvalue, &paod);
    }
    if (pap->src_field == PARSE_FIELD_SRC_COMMENT)
    {
      sdp_tmp = sdp;
      while (sdp_tmp != NULL)
      {
        ParseActionCommentDescriptor (sdp_tmp, &paod);
        sdp_tmp = sdp_tmp->next;
      }
      
      sap_tmp = sap;
      while (sap_tmp != NULL)
      {
        if (sap_tmp->type == 1)
        {
          sfp = sap_tmp->data;
          while (sfp != NULL)
          {
            ParseActionCommentFeature (sfp, &paod);
            sfp = sfp->next;
          }
        }
        sap_tmp = sap_tmp->next;
      }
    }
    if (pap->src_field == PARSE_FIELD_BANKIT_COMMENT)
    {
      sdp_tmp = sdp;
      while (sdp_tmp != NULL)
      {
        ParseActionBankitComment (sdp_tmp, &paod);
        sdp_tmp = sdp_tmp->next;
      }      
    }
  }
  
  if (bssp != NULL && !paod.parse_complete)
  {
    VisitSeqEntriesForParseAction (bssp->seq_set, pap, parse_action, parse_data, ok_to_parse);
  }

  pap->title_sep = old_title_sep;
  pap->biop_sep = old_biop_sep;
  
  VisitSeqEntriesForParseAction (sep->next, pap, parse_action, parse_data, ok_to_parse);
}

typedef struct parseactionlog 
{
  FILE    *fp;
  Boolean data_in_log;
} ParseActionLogData, PNTR ParseActionLogPtr;

static void  ParseActionSample (ParseActionPtr pap, Pointer userdata, CharPtr found_text, Int4 found_len)
{
  CharPtr           log_text;
  ParseActionLogPtr palp;
  
  palp = (ParseActionLogPtr) userdata;
  if (palp == NULL || found_len == 0 || found_text == NULL || palp->fp == NULL)
  {
    return;
  }
  
  log_text = (CharPtr) MemNew ((found_len + 1) * sizeof (Char));
  if (log_text != NULL)
  {
    StringNCpy (log_text, found_text, found_len);
    log_text[found_len] = 0;
    fprintf (palp->fp, "%s\n", log_text);
    palp->data_in_log = TRUE;
    MemFree (log_text);
  }
}

typedef struct featureselremconform
{
  FORM_MESSAGE_BLOCK
  DialoG  feature_select;
  DialoG  constraints;
  DialoG  accept_cancel;
  PopuP   action_choice;
  DialoG  feature_select_to;
  PrompT  from_prompt;
  PrompT  to_prompt;
  DialoG  feature_select_from;
  GrouP   remove_grp;
  GrouP   convert_grp;
  ButtoN  clear_constraints_on_action_change;
  
  /* used for convert callback */
  Uint2      featdef_to;
  CharPtr    featname_to;
  Boolean    ask_for_transcript_id_removal;
  Boolean    do_remove_transcript_ids;
  ValNodePtr productbsp_list;
  Int4       site_or_bond_type;
  Boolean    abort_convert;
} FeatureSelRemConvFormData, PNTR FeatureSelRemConvFormPtr;

static void CleanupFeatureSelRemConvForm (GraphiC g, VoidPtr data)

{
  FeatureSelRemConvFormPtr  mrfp;

  mrfp = (FeatureSelRemConvFormPtr) data;
  if (mrfp != NULL) {
    mrfp->featname_to = MemFree (mrfp->featname_to);
  }
  StdCleanupFormProc (g, data);
}

static void FeatureRemoveClearText (Pointer data)
{
  FeatureSelRemConvFormPtr   mrfp;
  FilterSetPtr          fsp;

  mrfp = (FeatureSelRemConvFormPtr) data;
  if (mrfp == NULL) return;
 
  fsp = DialogToPointer (mrfp->constraints);
  FilterSetClearText (fsp);
  PointerToDialog (mrfp->constraints, fsp);
  FilterSetFree (fsp);
}

static void FeatureRemoveClear (Pointer data)
{
  FeatureSelRemConvFormPtr mrfp;

  mrfp = (FeatureSelRemConvFormPtr) data;
  if (mrfp == NULL) return;
 
  PointerToDialog (mrfp->feature_select, NULL);
  PointerToDialog (mrfp->constraints, NULL);
  PointerToDialog (mrfp->feature_select_to, NULL);
  PointerToDialog (mrfp->feature_select_from, NULL);
}

#define FEATURE_REMOVE   1
#define FEATURE_CONVERT  2
#define FEATURE_SELECT   3
#define FEATURE_DESELECT 4

static void FeatureRemoveChangeNotify (Pointer userdata)
{
  FeatureSelRemConvFormPtr mrfp;
  ValNodePtr          err_list;

  mrfp = (FeatureSelRemConvFormPtr) userdata;
  if (mrfp == NULL) return;

  if (mrfp->action_choice == NULL)
  {
    /* just the remove dialog */
    err_list = TestDialog (mrfp->feature_select);
  }
  else if (GetValue (mrfp->action_choice) == FEATURE_CONVERT)
  {
    /* convert dialog in choice */
    if (mrfp->feature_select_from != NULL)
    {
      err_list = TestDialog (mrfp->feature_select_from);
    }
    else
    {
      err_list = TestDialog (mrfp->feature_select);
    }
    if (err_list == NULL && mrfp->feature_select_to != NULL)
    {
      err_list = TestDialog (mrfp->feature_select_to); 
    }
  }
  else
  {
    /* single feature dialog in choice */
    if (mrfp->feature_select != NULL)
    {
      err_list = TestDialog (mrfp->feature_select);
    }
  }
  
  if (err_list == NULL)
  {
    EnableAcceptCancelDialogAccept (mrfp->accept_cancel);
  }
  else
  {
    DisableAcceptCancelDialogAccept (mrfp->accept_cancel);
  }
  ValNodeFree (err_list);
}

static void FeatureRemoveOrConvertCenterAction (PopuP p)
{
  FeatureSelRemConvFormPtr mrfp;
  
  mrfp = (FeatureSelRemConvFormPtr) GetObjectExtra (p);
  if (mrfp == NULL) return;
  
  if (GetValue (p) == 2)
  {
    Hide (mrfp->remove_grp);
    Show (mrfp->convert_grp);
  }
  else
  {
    Show (mrfp->remove_grp);
    Hide (mrfp->convert_grp);
  }
  
  if (GetStatus (mrfp->clear_constraints_on_action_change))
  {
    PointerToDialog (mrfp->feature_select, NULL);
    PointerToDialog (mrfp->feature_select_from, NULL);
    PointerToDialog (mrfp->feature_select_to, NULL);
    PointerToDialog (mrfp->constraints, NULL);
  }
  FeatureRemoveChangeNotify ((Pointer) mrfp);
}



static void SelectFeatureCallback (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)

{
  if (sfp == NULL) return;
  ObjMgrAlsoSelect (sfp->idx.entityID, sfp->idx.itemID, OBJ_SEQFEAT, 0, NULL);
}

static void DeselectFeatureCallback (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)

{
  if (sfp == NULL) return;
  ObjMgrDeSelect (sfp->idx.entityID, sfp->idx.itemID, OBJ_SEQFEAT, 0, NULL);
}

typedef struct removefeature
{
  ValNodePtr bsplist;
  ValNodePtr bssplist;  
} RemoveFeatureData, PNTR RemoveFeaturePtr;

static void RemoveFeatureCallback (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  RemoveFeaturePtr rfp;
  SeqIdPtr         sip;
  BioseqPtr        productbsp, productcdna;
  BioseqSetPtr     productnps;
  
  if (sfp == NULL || userdata == NULL) return;
  rfp = (RemoveFeaturePtr) userdata;
  
  sfp->idx.deleteme = TRUE;
  if (sfp->data.choice == SEQFEAT_CDREGION)
  {
    if (sfp->product != NULL)
    {
      sip = SeqLocId (sfp->product);
      if (sip != NULL)
      {
        productbsp = BioseqFind (sip);
        if (productbsp != NULL)
        {
          ValNodeAddPointer (&(rfp->bsplist), 0, (Pointer) productbsp);
        }
      }
    }
  }
  else if (sfp->data.choice == SEQFEAT_RNA)
  {
    if (sfp->product != NULL)
    {
      sip = SeqLocId (sfp->product);
      if (sip != NULL)
      {
        productcdna = BioseqFind (sip);
        if (productcdna != NULL && productcdna->idx.parenttype == OBJ_BIOSEQSET)
        {
          productnps = (BioseqSetPtr) productcdna->idx.parentptr;
          if (productnps != NULL && productnps->_class == BioseqseqSet_class_nuc_prot)
          {
            ValNodeAddPointer (&(rfp->bssplist), 0, (Pointer) productnps);
          }
        }
      }
    }
  }
}

static void GetImpFeatureToCDSConversionValues (FeatureSelRemConvFormPtr mrfp)
{
  WindoW w;
  GrouP  h, g;
  TexT   geneName, protName, featcomment;

  if (mrfp == NULL)
  {
    return;
  }
  
  w = MovableModalWindow (-50, -33, -10, -10,
                          "Convert to CDS",
                          StdCloseWindowProc);
  h = HiddenGroup (w, -1, 0, NULL);
   
  g = HiddenGroup (h, 2, 0, NULL);
  StaticPrompt (g, "Gene Symbol", 0, dialogTextHeight, programFont, 'l');
  geneName = DialogText (g, "", 20, NULL);
  StaticPrompt (g, "Protein Name", 0, dialogTextHeight, programFont, 'l');
  protName = DialogText (g, "", 20, NULL);
  StaticPrompt (g, "Comment", 0, 4 * Nlm_stdLineHeight, programFont, 'l');
  featcomment = ScrollText (g, 20, 4, programFont, TRUE, NULL);

}


static void ConvertRegionToImp (SeqFeatPtr sfp, CharPtr featname_to)
{
  GBQualPtr          gbqual;
  ImpFeatPtr         ifp;
  CharPtr            str;

  ifp = ImpFeatNew ();
  if (NULL == ifp)
    return;

  str = (CharPtr) sfp->data.value.ptrvalue;
  sfp->data.choice = SEQFEAT_IMP;
  sfp->data.value.ptrvalue = (Pointer) ifp;
  if (featname_to == NULL)
  {
    ifp->key = StringSave ("misc_feature");
  }
  else
  {
    ifp->key = StringSave (featname_to);
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

static void AskForTranscriptionIdRemoval (BoolPtr ask, BoolPtr do_remove)
{
  if (ask == NULL || do_remove == NULL)
  {
    return;
  }
  if (*ask)
  {
    if (Message (MSG_YN, "Remove transcript IDs?") == ANS_YES)
    {
      *do_remove = TRUE;
    }
    *ask = FALSE;
  }
}

static void 
ConvertGeneToRNA 
(SeqFeatPtr  sfp,
 Int2        toFeatSubType,
 Int2        fromFeat,
 Boolean     do_remove_transcript_ids,
 ValNodePtr PNTR productbsp_list)
{
  Int2               j;
  Char               label [256];
  BioseqPtr          productbsp;
  RnaRefPtr          rrp;
  SeqIdPtr           sip;
  tRNAPtr            trp;
  Boolean            just_trna_text = FALSE;
  Boolean            asked_for_replace = TRUE;
  Boolean            do_replace = FALSE;
  Boolean            use_semicolon = TRUE;
  GeneRefPtr         grp = NULL;

  rrp = RnaRefNew ();
  if (NULL == rrp)
    return;

  FeatDefLabel (sfp, label, sizeof (label), OM_LABEL_CONTENT);
  if (fromFeat == SEQFEAT_GENE) {
    grp = (GeneRefPtr) sfp->data.value.ptrvalue;
    sfp->data.value.ptrvalue = NULL;
  } else if (fromFeat == SEQFEAT_CDREGION) {
    if (sfp->product != NULL) {
      sip = SeqLocId (sfp->product);
      if (sip != NULL) {
        productbsp = BioseqFind (sip);
        if (productbsp != NULL) 
        {
	        ValNodeAddPointer (productbsp_list, 0, (Pointer) productbsp);
	      }
      }
      if (do_remove_transcript_ids)
      {
        sfp->product = SeqLocFree (sfp->product);
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
      trp->aa = ParseTRnaString (label, &just_trna_text, NULL, FALSE);
      trp->aatype = 2;
      for (j = 0; j < 6; j++) {
	      trp->codon [j] = 255;
      }
    }
    if (!just_trna_text)
    {
      AppendOrReplaceString (&(sfp->comment), label,
                           &asked_for_replace,
                           &do_replace,
                           &use_semicolon);
    }
  } else if (! StringHasNoText (label)) {
    rrp->ext.choice = 1;
    rrp->ext.value.ptrvalue = StringSave (label);
  }
  
  if (grp != NULL)
  {
    /* append unused gene qualifiers */
    if (StringCmp (grp->locus, label) != 0)
    {
      AppendOrReplaceString (&(sfp->comment), grp->locus,
                           &asked_for_replace,
                           &do_replace,
                           &use_semicolon);
    }
    if (StringCmp (grp->allele, label) != 0)
    {
      AppendOrReplaceString (&(sfp->comment), grp->allele,
                           &asked_for_replace,
                           &do_replace,
                           &use_semicolon);
    }
    if (StringCmp (grp->desc, label) != 0)
    {
      AppendOrReplaceString (&(sfp->comment), grp->desc,
                           &asked_for_replace,
                           &do_replace,
                           &use_semicolon);
    }
    if (StringCmp (grp->maploc, label) != 0)
    {
      AppendOrReplaceString (&(sfp->comment), grp->maploc,
                           &asked_for_replace,
                           &do_replace,
                           &use_semicolon);
    }
    if (StringCmp (grp->locus_tag, label) != 0)
    {
      AppendOrReplaceString (&(sfp->comment), grp->locus_tag,
                           &asked_for_replace,
                           &do_replace,
                           &use_semicolon);
    }

    /* free generef */
    grp = GeneRefFree (grp);      
  }
}

static void GetSiteOrBondType (FeatureSelRemConvFormPtr mrfp, Uint2 toFeat)
{
  EnumFieldAssocPtr  al;
  CharPtr            mssg;
  UIEnum             val;
  if (mrfp == NULL || mrfp->site_or_bond_type != -1 || toFeat == SEQFEAT_REGION)
  {
    return;
  }
  al = NULL;
  mssg = NULL;
  if (toFeat == SEQFEAT_BOND) {
    al = enum_bond_alist;
    mssg = "Select type of bond";
  } else if (toFeat == SEQFEAT_SITE) {
    al = enum_site_alist;
    mssg = "Select type of site";
  }
  if (al == NULL) return;
  if (! AlistMessage (al, &val, 1, mssg)) return;
  mrfp->site_or_bond_type = (Int4) val; 
}

static void 
ConvertToBondSiteOrRegion 
(SeqFeatPtr sfp, 
 Int4       site_or_bond_type,
 Uint2      toFeat)
{
  SeqFeatPtr         cds;
  BioseqPtr          bsp;
  SeqLocPtr          slp;
  SeqEntryPtr        sep;
  SeqMgrFeatContext  context, cds_context;
  SeqFeatPtr         newsfp;
  SeqIdPtr           sip;
  CharPtr            comment;
  SeqBondPtr         sbp;
  SeqPntPtr          spp;
  
  if (sfp == NULL)
  {
    return;
  }
  
  if (site_or_bond_type == -1 && toFeat != SEQFEAT_REGION)
  {
    return;
  }
  
  bsp = BioseqFindFromSeqLoc (sfp->location);
  sfp = SeqMgrGetDesiredFeature (0, bsp, 0, 0, sfp, &context);
  if (sfp == NULL) return;

  
  cds = SeqMgrGetOverlappingCDS (sfp->location, &cds_context);
    if (cds == NULL) {
    Message (MSG_ERROR, "Unable to find CDS covering %s", context.label);
    return;
  }
  bsp = GetBioseqGivenSeqLoc (cds->location, context.entityID);
  if (bsp == NULL || ISA_aa (bsp->mol)) {
    Message (MSG_ERROR, "Unable to find DNA bioseq for %s", context.label);
    return;
  }
  slp = dnaLoc_to_aaLoc (cds, sfp->location, TRUE, NULL, FALSE);
  if (slp == NULL) {
    Message (MSG_ERROR, "Unable to convert to protein coordinate for %s", context.label);
    return;
  }
  bsp = GetBioseqGivenSeqLoc (slp, context.entityID);
  if (bsp == NULL) {
    Message (MSG_ERROR, "Unable to find target protein bioseq for %s", context.label);
    return;
  }
  sep = SeqMgrGetSeqEntryForData (bsp);
  if (sep == NULL) {
    Message (MSG_ERROR, "Unable to find target protein seq-entry for %s", context.label);
    return;
  }
  
  comment = sfp->comment;

  newsfp = NULL;
  if (toFeat == SEQFEAT_BOND || toFeat == SEQFEAT_SITE) {
    newsfp = SeqFeatNew ();
    if (newsfp != NULL) {
      newsfp->data.choice = toFeat;
      newsfp->data.value.intvalue = site_or_bond_type;
	  sfp->comment = NULL;
    }
  } else if (toFeat == SEQFEAT_REGION) {
    newsfp = SeqFeatNew ();
    if (newsfp != NULL) {
      newsfp->data.choice = toFeat;
      newsfp->data.value.ptrvalue = sfp->comment;
      sfp->comment = NULL;
	  comment = NULL;
    }
  }
  if (newsfp != NULL) {
    sfp->idx.deleteme = TRUE;
    DeleteMarkedObjects (context.entityID, 0, NULL);
    CreateNewFeature (sep, NULL, toFeat, newsfp);
    newsfp->location = slp;
    newsfp->comment = comment;
    if (toFeat == SEQFEAT_BOND) {
      if (slp->choice != SEQLOC_BOND) {
        sip = SeqLocId (slp);
        if (sip != NULL) {
          sbp = SeqBondNew ();
          if (sbp != NULL) {
            slp = ValNodeNew (NULL);
            if (slp != NULL) {
              slp->choice = SEQLOC_BOND;
              slp->data.ptrvalue = (Pointer) sbp;
              spp = SeqPntNew ();
              if (spp != NULL) {
                spp->strand = SeqLocStrand (newsfp->location);
                spp->id = SeqIdStripLocus (SeqIdDup (SeqIdFindBest (sip, 0)));
                spp->point = SeqLocStart (newsfp->location);
                sbp->a = spp;
              }
              spp = SeqPntNew ();
              if (spp != NULL) {
                spp->strand = SeqLocStrand (newsfp->location);
                spp->id = SeqIdStripLocus (SeqIdDup (SeqIdFindBest (sip, 0)));
                spp->point = SeqLocStop (newsfp->location);
                sbp->b = spp;
              }
              newsfp->location = SeqLocFree (newsfp->location);
              newsfp->location = slp;
            }
          }
        }
      }
    }
  }
}


static void ConvertImpToImp (SeqFeatPtr sfp, CharPtr featname_to)
{
  ImpFeatPtr         ifp;

  ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
  if (NULL == ifp)
    return;
  
  ifp->key = MemFree (ifp->key);
  if (featname_to == NULL)
  {
    ifp->key = StringSave ("misc_feature");
  }
  else
  {
    ifp->key = StringSave (featname_to);
  }
}

static void ConvertFeatureCallback (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  FeatureSelRemConvFormPtr   mrfp;
  Uint2                 fromFeat;
  Uint2                 toFeat;

  mrfp = (FeatureSelRemConvFormPtr) userdata;
  
  if (sfp == NULL || mrfp == NULL || mrfp->abort_convert)
  {
    return;
  }
  
  fromFeat = sfp->data.choice;
  toFeat   = FindFeatFromFeatDefType (mrfp->featdef_to);

  if (fromFeat == SEQFEAT_IMP 
      && toFeat == SEQFEAT_CDREGION)
  {
    Message (MSG_ERROR, "Conversion not yet supported");
    mrfp->abort_convert = TRUE;   
  }
  else if (fromFeat == SEQFEAT_IMP &&
      toFeat   == SEQFEAT_PROT)
    FeatConvertImpToPeptide (sfp, mrfp->featdef_to, mrfp->input_entityID);
  else if (fromFeat == SEQFEAT_PROT &&
	   toFeat   == SEQFEAT_IMP &&
	   sfp->idx.subtype == FEATDEF_mat_peptide_aa)
    FeatConvertPeptideToImp (sfp, mrfp->input_entityID);
  else if (fromFeat == SEQFEAT_PROT &&
	   toFeat   == SEQFEAT_IMP &&
	   sfp->idx.subtype == FEATDEF_sig_peptide_aa)
    FeatConvertPeptideToImp (sfp, mrfp->input_entityID);
  else if (fromFeat == SEQFEAT_PROT &&
	   toFeat   == SEQFEAT_IMP &&
	   sfp->idx.subtype == FEATDEF_transit_peptide_aa)
    FeatConvertPeptideToImp (sfp, mrfp->input_entityID);
  else if (fromFeat == SEQFEAT_IMP &&
	   (mrfp->featdef_to == FEATDEF_misc_RNA ||
	    mrfp->featdef_to == FEATDEF_precursor_RNA))
    FeatConvertImpToSpecialRNA (sfp, mrfp->featdef_to);
  else if ((fromFeat == SEQFEAT_GENE ||
	    fromFeat == SEQFEAT_CDREGION) &&
	   toFeat == SEQFEAT_RNA)
  {
    AskForTranscriptionIdRemoval (&(mrfp->ask_for_transcript_id_removal),
                                  &(mrfp->do_remove_transcript_ids));
    ConvertGeneToRNA (sfp, mrfp->featdef_to, fromFeat, 
                      mrfp->do_remove_transcript_ids, &(mrfp->productbsp_list));
  }
  else if (fromFeat == SEQFEAT_REGION &&
	   toFeat == SEQFEAT_IMP)
    ConvertRegionToImp (sfp, mrfp->featname_to);
  else if (fromFeat == SEQFEAT_REGION &&
	   (mrfp->featdef_to == FEATDEF_misc_RNA ||
	    mrfp->featdef_to == FEATDEF_precursor_RNA))
    FeatConvertRegionToRNA (sfp, mrfp->featdef_to);
  else if (fromFeat == SEQFEAT_IMP &&
	   toFeat == SEQFEAT_RNA)
    FeatConvertImpToRNA (sfp, mrfp->featdef_to);
  else if (fromFeat == SEQFEAT_COMMENT &&
	   mrfp->featdef_to == FEATDEF_misc_feature)
    FeatConvertCommentToMiscFeature (sfp);
  else if (fromFeat == SEQFEAT_GENE &&
	   mrfp->featdef_to == FEATDEF_misc_feature)
    FeatConvertGeneToMiscFeature (sfp);
  else if (fromFeat == SEQFEAT_RNA &&
	   mrfp->featdef_to == FEATDEF_misc_feature)
    FeatConvertRNAToMiscFeature (sfp);
  else if (fromFeat == SEQFEAT_SITE &&
	   mrfp->featdef_to == FEATDEF_misc_feature)
    FeatConvertSiteToMiscFeature (sfp);
  else if (fromFeat == SEQFEAT_PROT &&
	   (sfp->idx.subtype == FEATDEF_mat_peptide_aa ||
	    sfp->idx.subtype == FEATDEF_sig_peptide_aa ||
	    sfp->idx.subtype == FEATDEF_transit_peptide_aa) &&
	   toFeat == SEQFEAT_REGION)
    FeatConvertPeptideToRegion (sfp);
  else if (fromFeat == SEQFEAT_BIOSRC &&
	   toFeat == SEQFEAT_IMP &&
	   mrfp->featdef_to == FEATDEF_repeat_region)
    FeatConvertBioSrcToRepeatRegion (sfp);
  else if (fromFeat == SEQFEAT_REGION
    && toFeat == SEQFEAT_PROT
    && (mrfp->featdef_to == FEATDEF_mat_peptide_aa ||
        mrfp->featdef_to == FEATDEF_sig_peptide_aa ||
        mrfp->featdef_to == FEATDEF_transit_peptide_aa ||
        mrfp->featdef_to == FEATDEF_preprotein))
  {
    FeatConvertRegionToProt (sfp, mrfp->featdef_to);
  }
  else if (toFeat == SEQFEAT_BOND || toFeat == SEQFEAT_REGION || toFeat == SEQFEAT_SITE)
  {
    GetSiteOrBondType (mrfp, toFeat);
    ConvertToBondSiteOrRegion (sfp, mrfp->site_or_bond_type, toFeat);
  }
    
  else if (fromFeat != toFeat)
  {
    ArrowCursor ();
    Message (MSG_OK, "This conversion not supported - contact"
	     " sequindev for instructions.");
    mrfp->abort_convert = TRUE;
    return;
  }
  else if (fromFeat == SEQFEAT_IMP)
    ConvertImpToImp (sfp, mrfp->featname_to);
  else if (fromFeat == SEQFEAT_RNA)
    FeatConvertRNAToRNA (sfp, mrfp->featdef_to);
  else if (fromFeat == SEQFEAT_PROT)
    FeatConvertProtToProt (sfp, mrfp->featdef_to);

}

static void RemoveProteinProducts (ValNodePtr bsplist, Uint2 entityID, SeqEntryPtr sep)
{
  MsgAnswer  ans;
  ValNodePtr tmp;
  BioseqPtr  bsp;
  Uint2      itemID;
  OMProcControl  ompc;
  
  if (bsplist == NULL) 
  {
    return;
  }
  ans = Message (MSG_YN, "Remove protein products?");
  if (ans == ANS_YES) {
    for (tmp = bsplist; tmp != NULL; tmp = tmp->next) {
      bsp = (BioseqPtr) tmp->data.ptrvalue;
      itemID = GetItemIDGivenPointer (entityID, OBJ_BIOSEQ, (Pointer) bsp);
      if (itemID > 0) {
        MemSet ((Pointer) (&ompc), 0, sizeof (OMProcControl));
        ompc.do_not_reload_from_cache = TRUE;
        ompc.input_entityID = entityID;
        ompc.input_itemID = itemID;
        ompc.input_itemtype = OBJ_BIOSEQ;
        if (! DetachDataForProc (&ompc, FALSE)) {
          Message (MSG_POSTERR, "DetachDataForProc failed");
        }
        SeqMgrDeleteFromBioseqIndex (bsp);
      }
    }
    ans = Message (MSG_YN, "Renormalize Nuc-Prot sets?");
    if (ans == ANS_YES)
    {
      RemoveOrphanProteins (entityID, sep);
      RenormalizeNucProtSets (sep, TRUE);
    }
  }
}

static void RemoveCDNANucProtProducts (ValNodePtr bssplist, Uint2 entityID)
{
  MsgAnswer     ans;
  ValNodePtr    tmp;
  BioseqSetPtr  bssp;
  Uint2         itemID;
  OMProcControl ompc;

  if (bssplist == NULL)
  {
    return;
  }
  
  ans = Message (MSG_YN, "Remove cDNA nuc-prot products?");
  if (ans != ANS_YES)
  {
    return;
  }

  for (tmp = bssplist; tmp != NULL; tmp = tmp->next) 
  {
    bssp = (BioseqSetPtr) tmp->data.ptrvalue;
    itemID = GetItemIDGivenPointer (entityID, OBJ_BIOSEQSET, (Pointer) bssp);
    if (itemID > 0) {
      MemSet ((Pointer) (&ompc), 0, sizeof (OMProcControl));
      ompc.do_not_reload_from_cache = TRUE;
      ompc.input_entityID = entityID;
      ompc.input_itemID = itemID;
      ompc.input_itemtype = OBJ_BIOSEQSET;
      if (! DetachDataForProc (&ompc, FALSE)) {
        Message (MSG_POSTERR, "DetachDataForProc failed");
      }
    }
  }
}

static Boolean FeatureRemoveOrConvertAction (Pointer userdata)
{
  FeatureSelRemConvFormPtr   mrfp;
  FilterSetPtr          fsp;
  SeqEntryPtr           sep;
  ValNodePtr            feature_type_list, vnp;
  Int4                  window_action;
  Int2                  feat_def_choice;
  RemoveFeatureData     rfd;
  
  if (userdata == NULL) return FALSE;
  
  mrfp = (FeatureSelRemConvFormPtr) userdata;
  mrfp->abort_convert = FALSE;
  window_action = GetValue (mrfp->action_choice);
  
  fsp = (FilterSetPtr) DialogToPointer (mrfp->constraints);
  
  sep = GetTopSeqEntryForEntityID (mrfp->input_entityID);
  if (sep == NULL) return FALSE;
  
  if (window_action == FEATURE_CONVERT)
  {
    vnp = (ValNodePtr) DialogToPointer (mrfp->feature_select_to);
    if (vnp == NULL) 
    {
      FilterSetFree (fsp);
      return FALSE;
    }
    mrfp->featdef_to = vnp->choice;
    mrfp->featname_to = vnp->data.ptrvalue;
    /* NOTE - I do not need to use ValNodeFreeData because I'm using
     * the data.ptrvalue in mrfp->featname_to and will free it when I free
     * mrfp.
     */
    vnp = ValNodeFree (vnp);
    feature_type_list = (ValNodePtr) DialogToPointer (mrfp->feature_select_from);
  }
  else
  {
    feature_type_list = (ValNodePtr) DialogToPointer (mrfp->feature_select);
  }
  if (feature_type_list == NULL)
  {
    FilterSetFree (fsp);
    return FALSE;
  }
    
  for (vnp = feature_type_list; vnp != NULL; vnp = vnp->next)
  {
    feat_def_choice = vnp->choice;
    if (feat_def_choice == 255)
    {
      feat_def_choice = 0;
    }
    switch (window_action)
    {
      case FEATURE_CONVERT:
        if ((feat_def_choice == FEATDEF_CDS || feat_def_choice == FEATDEF_GENE)
            && mrfp->featdef_to == SEQFEAT_RNA)
        {
          mrfp->ask_for_transcript_id_removal = TRUE;
        }
        OperateOnSeqEntryConstrainedObjects (sep, fsp, 
                                             ConvertFeatureCallback,
                                             NULL, 0, feat_def_choice, 0, mrfp);
        break;
      case FEATURE_REMOVE:
        rfd.bsplist = NULL;
        rfd.bssplist = NULL;
        OperateOnSeqEntryConstrainedObjects (sep, fsp, 
                                             RemoveFeatureCallback,
                                             NULL, 0, feat_def_choice, 0, &rfd);
        RemoveProteinProducts (rfd.bsplist, mrfp->input_entityID, sep);
        RemoveCDNANucProtProducts (rfd.bssplist, mrfp->input_entityID);
        rfd.bsplist = ValNodeFree (rfd.bsplist);
        rfd.bssplist = ValNodeFree (rfd.bssplist);
        break;
      case FEATURE_SELECT:
        OperateOnSeqEntryConstrainedObjects (sep, fsp, 
                                             SelectFeatureCallback,
                                             NULL, 0, feat_def_choice, 0, NULL);
        break;
      case FEATURE_DESELECT:
        OperateOnSeqEntryConstrainedObjects (sep, fsp, 
                                             DeselectFeatureCallback,
                                             NULL, 0, feat_def_choice, 0, NULL);
        break;
    }
  }
  
  ValNodeFree (feature_type_list);
  FilterSetFree (fsp);
  
  if (window_action == FEATURE_REMOVE || window_action == FEATURE_CONVERT)
  {
    DeleteMarkedObjects (mrfp->input_entityID, 0, NULL);
  }
  
  ObjMgrSetDirtyFlag (mrfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, mrfp->input_entityID, 0, 0);  
  Update ();
  return TRUE;
}

static void FeatureRemoveOrConvert (IteM i, Int4 first_action)
{
  BaseFormPtr         bfp;
  FeatureSelRemConvFormPtr mrfp;
  WindoW              w;
  GrouP               h, k, n;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  mrfp = (FeatureSelRemConvFormPtr) MemNew (sizeof (FeatureSelRemConvFormData));
  if (mrfp == NULL) return;
  
  w = FixedWindow (-50, -33, -10, -10, "Remove or Convert Features", StdCloseWindowProc);
  SetObjectExtra (w, mrfp, CleanupFeatureSelRemConvForm);
  mrfp->form = (ForM) w;
  mrfp->input_entityID = bfp->input_entityID;
  
  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  if (first_action < FEATURE_REMOVE || first_action > FEATURE_DESELECT)
  {
    first_action = FEATURE_REMOVE;
  }
  mrfp->action_choice = PopupList (h, TRUE, FeatureRemoveOrConvertCenterAction);
  PopupItem (mrfp->action_choice, "Remove Features");
  PopupItem (mrfp->action_choice, "Convert Features");
  SetValue (mrfp->action_choice, first_action);
  SetObjectExtra (mrfp->action_choice, mrfp, NULL);
  
  mrfp->clear_constraints_on_action_change = CheckBox (h, "Clear when changing actions", NULL);
  SetStatus (mrfp->clear_constraints_on_action_change, TRUE);

  n = HiddenGroup (h, 0, 0, NULL);
  mrfp->remove_grp = HiddenGroup (n, 0, 1, NULL);
  mrfp->feature_select =  FeatureSelectionDialog (mrfp->remove_grp, TRUE,
                                                  FeatureRemoveChangeNotify, 
                                                  mrfp);
  
  mrfp->convert_grp = HiddenGroup (n, 2, 0, NULL);
  k = HiddenGroup (mrfp->convert_grp, 0, 2, NULL);
  mrfp->from_prompt = StaticPrompt (k, "From", 0, dialogTextHeight, systemFont, 'l');
  mrfp->feature_select_from =  FeatureSelectionDialog (k, FALSE,
                                                  FeatureRemoveChangeNotify, 
                                                  mrfp);
  AlignObjects (ALIGN_CENTER, (HANDLE) mrfp->from_prompt, (HANDLE) mrfp->feature_select_from, NULL);
  k = HiddenGroup (mrfp->convert_grp, 0, 2, NULL);
  mrfp->to_prompt = StaticPrompt (k, "To", 0, dialogTextHeight, systemFont, 'l');
  mrfp->feature_select_to =  FeatureSelectionDialog (k, FALSE,
                                                  FeatureRemoveChangeNotify, 
                                                  mrfp);
  AlignObjects (ALIGN_CENTER, (HANDLE) mrfp->to_prompt, (HANDLE) mrfp->feature_select_to, NULL);
  AlignObjects (ALIGN_CENTER, (HANDLE) mrfp->remove_grp, (HANDLE) mrfp->convert_grp, NULL);
  
  mrfp->constraints = FilterGroup (h, TRUE, FALSE, TRUE, FALSE, "Where feature text");
  mrfp->accept_cancel = AcceptCancelDialog (h, FeatureRemoveOrConvertAction, NULL, FeatureRemoveClear, FeatureRemoveClearText, (Pointer)mrfp, w);
  AlignObjects (ALIGN_CENTER, (HANDLE) mrfp->action_choice,
                              (HANDLE) mrfp->clear_constraints_on_action_change,
                              (HANDLE) n,
                              (HANDLE) mrfp->constraints,
                              (HANDLE) mrfp->accept_cancel, NULL);
                              
  FeatureRemoveOrConvertCenterAction (mrfp->action_choice);
  
  Show (w);
}

extern void FeatureRemove (IteM i)
{
  FeatureRemoveOrConvert (i, FEATURE_REMOVE);
}

extern void ConvertFeatures (IteM i)
{
  FeatureRemoveOrConvert (i, FEATURE_CONVERT);
}

static Boolean SelectFeatureAction (Pointer userdata)
{
  FeatureSelRemConvFormPtr   mrfp;
  FilterSetPtr          fsp;
  SeqEntryPtr           sep;
  ValNodePtr            feature_type_list, vnp;
  Int2                  feat_def_choice;
  
  if (userdata == NULL) return FALSE;
  
  mrfp = (FeatureSelRemConvFormPtr) userdata;
  fsp = (FilterSetPtr) DialogToPointer (mrfp->constraints);
  
  sep = GetTopSeqEntryForEntityID (mrfp->input_entityID);
  if (sep == NULL) return FALSE;
  
  feature_type_list = (ValNodePtr) DialogToPointer (mrfp->feature_select);
  if (feature_type_list == NULL)
  {
    FilterSetFree (fsp);
    return FALSE;
  }
    
  for (vnp = feature_type_list; vnp != NULL; vnp = vnp->next)
  {
    feat_def_choice = vnp->choice;
    if (feat_def_choice == 255)
    {
      feat_def_choice = 0;
    }
    OperateOnSeqEntryConstrainedObjects (sep, fsp, 
                                         SelectFeatureCallback,
                                         NULL, 0, feat_def_choice, 0, NULL);
  }
  
  ValNodeFree (feature_type_list);
  FilterSetFree (fsp);
  
  ObjMgrSetDirtyFlag (mrfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, mrfp->input_entityID, 0, 0);  
  Update ();
  return TRUE;
}

extern void SelectFeatures (IteM i)
{
  BaseFormPtr              bfp;
  FeatureSelRemConvFormPtr mrfp;
  WindoW                   w;
  GrouP                    h;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  mrfp = (FeatureSelRemConvFormPtr) MemNew (sizeof (FeatureSelRemConvFormData));
  if (mrfp == NULL) return;
  
  w = FixedWindow (-50, -33, -10, -10, "Select Features", StdCloseWindowProc);
  SetObjectExtra (w, mrfp, CleanupFeatureSelRemConvForm);
  mrfp->form = (ForM) w;
  mrfp->input_entityID = bfp->input_entityID;
  
  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  mrfp->feature_select =  FeatureSelectionDialog (h, TRUE,
                                                  FeatureRemoveChangeNotify, 
                                                  mrfp);
    
  mrfp->constraints = FilterGroup (h, TRUE, FALSE, TRUE, FALSE, "Where feature text");
  mrfp->accept_cancel = AcceptCancelDialog (h, SelectFeatureAction, NULL, FeatureRemoveClear, FeatureRemoveClearText, (Pointer)mrfp, w);
  AlignObjects (ALIGN_CENTER, (HANDLE) mrfp->feature_select,
                              (HANDLE) mrfp->constraints,
                              (HANDLE) mrfp->accept_cancel, NULL);
                              
  Show (w);
}

typedef struct parseform 
{
  FEATURE_FORM_BLOCK
  DialoG  text_portion;
  DialoG  source_field;
  DialoG  destination_field;
  DialoG  accept_cancel;
  
  ParseFieldPtr   dst_field_data;
  ValNodePtr      source_qual_choice_list;
  GetSamplePtr    gsp;
  ExistingTextPtr etp;
} ParseFormData, PNTR ParseFormPtr;

static void CleanupParseForm (GraphiC g, VoidPtr data)
{
  ParseFormPtr mp;
  
  mp = (ParseFormPtr) data;
  if (mp != NULL)
  {
    mp->source_qual_choice_list = ValNodeFreeData (mp->source_qual_choice_list);
    mp->gsp = GetSampleFree (mp->gsp);
    mp->etp = MemFree (mp->etp);
  }
  StdCleanupExtraProc (g, data);
}

static void GetParseToExistingTextCallback (ParseActionPtr pap, Pointer userdata, CharPtr found_text, Int4 found_len)
{
  ParseFormPtr      mp;
  GetSamplePtr        gsp = NULL, gsp_sum, gsp_new;
  ValNodePtr          requested_field = NULL, vnp;

  mp = (ParseFormPtr) userdata;
  if (mp == NULL || mp->dst_field_data == NULL)
  {
    return;
  }

  switch (mp->dst_field_data->parse_field_type)
  {
    case PARSE_FIELD_SOURCE_QUAL :
      gsp = CheckForExistingTextInSeqEntry (pap->biop_sep, 
                                            mp->dst_field_data->feature_field, 
                                            GetSourceQualFeatureString,
                                            GetSourceQualDescrString,
                                            ValNodeSimpleDataFree,
                                            SourceQualValNodeDataCopy,
                                            NULL,
                                            SEQFEAT_BIOSRC, 0,
                                            Seq_descr_source);
      break;
    case PARSE_FIELD_DEFLINE:
      requested_field = ValNodeNew (NULL);
      requested_field->data.intvalue = Seq_descr_title;
      gsp = CheckForExistingTextInSeqEntry (pap->title_sep, requested_field, 
                                            NULL,
                                            GetStringFromStringDescriptor,
                                            NULL, IntValNodeCopy,
                                            NULL, 0, 0, Seq_descr_title);
      requested_field = ValNodeFree (requested_field);
      break;
    case PARSE_FIELD_BIOSRC_STRING:
      gsp = CheckForExistingTextInSeqEntry (pap->biop_sep, 
                                            mp->dst_field_data->feature_field, 
                                            GetSourceFeatureString,
                                            GetSourceDescriptorString,
                                            NULL, IntValNodeCopy,
                                            NULL,
                                            SEQFEAT_BIOSRC, 0,
                                            Seq_descr_source);
      break;
    case PARSE_FIELD_GENE_FIELD:
      gsp = CheckForExistingTextInSeqEntry (pap->sep, 
                                            mp->dst_field_data->feature_field, 
                                            GetGeneFieldString,
                                            NULL,
                                            NULL, IntValNodeCopy,
                                            NULL,
                                            SEQFEAT_GENE, 0, 0);
      break;
    case PARSE_FIELD_MRNA_FIELD:
      gsp = CheckForExistingTextInSeqEntry (pap->sep, 
                                            mp->dst_field_data->feature_field, 
                                            GetmRNAFieldString,
                                            NULL,
                                            NULL, IntValNodeCopy,
                                            NULL,
                                            SEQFEAT_RNA, FEATDEF_mRNA, 0);
      break;
    case PARSE_FIELD_CDS_COMMENT:
      gsp = CheckForExistingTextInSeqEntry (pap->sep, 
                                            NULL, 
                                            GetCDSComment,
                                            NULL,
                                            NULL, IntValNodeCopy,
                                            NULL,
                                            SEQFEAT_CDREGION, FEATDEF_CDS, 0);
      break;
    case PARSE_FIELD_PROTEIN_FIELD:
      gsp = CheckForExistingTextInSeqEntry (pap->sep, 
                                            mp->dst_field_data->feature_field, 
                                            GetProteinFieldString,
                                            NULL,
                                            NULL, IntValNodeCopy,
                                            NULL,
                                            SEQFEAT_PROT, 0, 0);
      break;
    case PARSE_FIELD_IMPORT_QUAL:
      gsp_sum = NULL;
      for (vnp = mp->dst_field_data->feature_subtype; vnp != NULL; vnp = vnp->next)
      {
        gsp_new = CheckForExistingTextInSeqEntry (pap->sep, mp->dst_field_data->feature_field, 
                                                  GetGBQualString,
                                                  NULL,
                                                  NULL, IntValNodeCopy,
                                                  NULL, 0, vnp->choice, 0);
        gsp = GetSampleAdd (gsp_new, gsp_sum);
        gsp_new = GetSampleFree (gsp_new);
        gsp_sum = GetSampleFree (gsp_sum);
        gsp_sum = gsp;
      }
      break;
    case PARSE_FIELD_FEATURE_NOTE:
      gsp_sum = NULL;
      for (vnp = mp->dst_field_data->feature_field; vnp != NULL; vnp = vnp->next)
      {
        gsp_new = CheckForExistingTextInSeqEntry (pap->sep, vnp, 
                                                  GetFeatureNote,
                                                  NULL,
                                                  ValNodeSimpleDataFree,
                                                  ValNodeStringCopy,
                                                  NULL, 0, vnp->choice, 0);
        gsp = GetSampleAdd (gsp_new, gsp_sum);
        gsp_new = GetSampleFree (gsp_new);
        gsp_sum = GetSampleFree (gsp_sum);
        gsp_sum = gsp;
      }
      break;
  }
  
  if (gsp == NULL)
  {
    return;
  }
  if (mp->gsp == NULL)
  {
    mp->gsp = gsp;
  }
  else
  {
    mp->gsp->num_found += gsp->num_found;
    if (StringCmp (mp->gsp->sample_text, gsp->sample_text) != 0)
    {
      mp->gsp->all_same = FALSE;
    }
    gsp = GetSampleFree (gsp);
  } 
}

static void 
ParseToFeaturesOnBioseq 
(BioseqPtr             bsp, 
 SetFeatureFieldString set_string_func,
 ApplyValuePtr         avp,
 Int4                  parse_field_type,
 Uint2                 feature_subtype)
{
  SeqFeatPtr        sfp;
  SeqMgrFeatContext fcontext;
  BioseqPtr         prot_bsp;
  ProtRefPtr        prp;
  
  if (bsp == NULL || set_string_func == NULL)
  {
    return;
  }
  
  if (ISA_na (bsp->mol) && parse_field_type == PARSE_FIELD_PROTEIN_FIELD)
  {
    sfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_CDREGION, 0, &fcontext);
    while (sfp != NULL)
    {
      prot_bsp = BioseqFindFromSeqLoc (sfp->product);
      if (prot_bsp == NULL)
      {
  	    SeqEdTranslateOneCDS (sfp, bsp, sfp->idx.entityID);
  	    SeqMgrIndexFeatures (sfp->idx.entityID, NULL);
        prot_bsp = BioseqFindFromSeqLoc (sfp->product);
      }
      ParseToFeaturesOnBioseq (prot_bsp, set_string_func, avp, 
                               parse_field_type, feature_subtype);
      sfp = SeqMgrGetNextFeature (bsp, sfp, SEQFEAT_CDREGION, 0, &fcontext);
    }
  }
  else
  {
    sfp = SeqMgrGetNextFeature (bsp, NULL, 0, feature_subtype, &fcontext);
    /* if full-length protein feature is missing, add it */
    if (ISA_aa (bsp->mol) && feature_subtype == FEATDEF_PROT && sfp == NULL)
    {
      prp = ProtRefNew ();
      sfp = CreateNewFeatureOnBioseq (bsp, SEQFEAT_PROT, NULL);
      if (sfp != NULL)
      {
        sfp->data.value.ptrvalue = prp;
  	    SeqMgrIndexFeatures (bsp->idx.entityID, NULL);
  	    set_string_func (sfp, avp, NULL);
      }
    }
    else
    {
      while (sfp != NULL)
      {
        set_string_func (sfp, avp, NULL);        
        sfp = SeqMgrGetNextFeature (bsp, sfp, 0, feature_subtype, &fcontext);
      }
    }
  }
}

typedef struct parsefeatures
{
  ApplyValuePtr         avp;
  SetFeatureFieldString set_string_func;
} ParseFeaturesData, PNTR ParseFeaturesPtr;

static void ParseToFeaturesCallback (SeqFeatPtr sfp, Pointer userdata)
{
  ParseFeaturesPtr pfp;
  
  if (sfp == NULL || userdata == NULL)
  {
    return;
  }
  
  pfp = (ParseFeaturesPtr) userdata;
  
  if (pfp->avp == NULL || pfp->set_string_func == NULL)
  {
    return;
  }
  
  (pfp->set_string_func) (sfp, pfp->avp, NULL);
}

static void 
ParseToFeatures 
(SeqEntryPtr   sep,
 ApplyValuePtr avp, 
 Int4          parse_field_type,
 Uint2         feature_subtype)
{
  SetFeatureFieldString set_string_func = NULL;
  ParseFeaturesData     pfd;
  
  if (sep == NULL || avp == NULL)
  {
    return;
  }
  
  switch (parse_field_type)
  {
    case PARSE_FIELD_GENE_FIELD :
      set_string_func = SetGeneFieldString;
      break;
    case PARSE_FIELD_MRNA_FIELD :
      set_string_func = SetmRNAFieldString;
      break;
    case PARSE_FIELD_CDS_COMMENT:
      set_string_func = SetCDSComment;
      break;
    case PARSE_FIELD_PROTEIN_FIELD:
      set_string_func = SetProteinFieldString;
      break;
    case PARSE_FIELD_IMPORT_QUAL:
      set_string_func = SetGBQualString;
      break;
    case PARSE_FIELD_FEATURE_NOTE:
      set_string_func = SetFeatureNote;
  }
  
  if (set_string_func == NULL)
  {
    return;
  }
  

  if (IS_Bioseq (sep))
  {
    ParseToFeaturesOnBioseq (sep->data.ptrvalue, set_string_func, avp,
                             parse_field_type, feature_subtype);
  }
  else
  {
    pfd.avp = avp;
    pfd.set_string_func = set_string_func;
    VisitFeaturesInSep (sep, &pfd, ParseToFeaturesCallback);
  }
}

static void ParseToDest (ParseActionPtr pap, Pointer userdata, CharPtr found_text, Int4 found_len)
{
  ParseFormPtr  mp;
  ApplyValueData  avd;
  ValNodePtr      vnp;

  mp = (ParseFormPtr) userdata;
  if (mp == NULL || mp->dst_field_data == NULL)
  {
    return;
  }
  
  avd.new_text = MemNew ((found_len + 1) * sizeof (Char));
  StringNCpy (avd.new_text, found_text, found_len);
  avd.new_text [found_len] = 0;
  avd.etp = mp->etp;
  avd.text_to_replace = NULL;
  
  switch (mp->dst_field_data->parse_field_type)
  {
    case PARSE_FIELD_SOURCE_QUAL:
      avd.field_list= mp->dst_field_data->feature_field;
      OperateOnSeqEntryConstrainedObjects (pap->biop_sep, NULL, 
                                           ApplySourceQualFeatureCallback,
                                           ApplySourceQualDescriptorCallback,
                                           0, 0, 0, &avd);
      avd.new_text = MemFree (avd.new_text);
      break;
    case PARSE_FIELD_DEFLINE :
      avd.field_list = mp->dst_field_data->feature_field;
      OperateOnSeqEntryConstrainedObjects (pap->title_sep, NULL, 
                                           NULL,
                                           ApplyTitleDescriptorCallback,
                                           0, 0, 0, &avd);
      avd.new_text = MemFree (avd.new_text);
      break;
    case PARSE_FIELD_BIOSRC_STRING :
      avd.field_list = mp->dst_field_data->feature_field;
      OperateOnSeqEntryConstrainedObjects (pap->biop_sep, NULL, 
                                           ApplySourceStringFeatureCallback,
                                           ApplySourceStringDescriptorCallback,
                                           0, 0, 0, &avd);
    case PARSE_FIELD_GENE_FIELD :
      avd.field_list = mp->dst_field_data->feature_field;
      ParseToFeatures (pap->sep, &avd, mp->dst_field_data->parse_field_type,
                       FEATDEF_GENE);
      break;
    case PARSE_FIELD_MRNA_FIELD :
      avd.field_list = mp->dst_field_data->feature_field;
      ParseToFeatures (pap->sep, &avd, mp->dst_field_data->parse_field_type,
                       FEATDEF_mRNA);
      break;
    case PARSE_FIELD_CDS_COMMENT:
      avd.field_list = NULL;
      ParseToFeatures (pap->sep, &avd, mp->dst_field_data->parse_field_type,
                       FEATDEF_CDS);
      break;
    case PARSE_FIELD_PROTEIN_FIELD:
      avd.field_list = mp->dst_field_data->feature_field;
      ParseToFeatures (pap->sep, &avd, mp->dst_field_data->parse_field_type,
                       FEATDEF_PROT);
      break;
    case PARSE_FIELD_IMPORT_QUAL :
      avd.field_list = mp->dst_field_data->feature_field;
      for (vnp = mp->dst_field_data->feature_subtype; 
           vnp != NULL;
           vnp = vnp->next)
      {
        ParseToFeatures (pap->sep, &avd, mp->dst_field_data->parse_field_type,
                         vnp->choice);
      }
      break;    
    case PARSE_FIELD_FEATURE_NOTE :
      avd.field_list = NULL;
      for (vnp = mp->dst_field_data->feature_field; 
           vnp != NULL;
           vnp = vnp->next)
      {
        ParseToFeatures (pap->sep, &avd, mp->dst_field_data->parse_field_type,
                         vnp->choice);
      }
      break;    
  }
  avd.new_text = MemFree (avd.new_text);  
}

static GetSamplePtr 
GetDestSample 
(SeqEntryPtr   sep, 
 ParseFieldPtr dst_field_data)
{
  GetSamplePtr gsp = NULL, gsp_sum, gsp_new;
  ValNodePtr   requested_field = NULL, vnp;
  
  if (sep == NULL || dst_field_data == NULL)
  {
    return NULL;
  }
  
  switch (dst_field_data->parse_field_type)
  {
    case PARSE_FIELD_SOURCE_QUAL :
      gsp = CheckForExistingTextInSeqEntry (sep, dst_field_data->feature_field, 
                                            GetSourceQualFeatureString,
                                            GetSourceQualDescrString,
                                            ValNodeSimpleDataFree,
                                            SourceQualValNodeDataCopy,
                                            NULL,
                                            SEQFEAT_BIOSRC, 0, Seq_descr_source);
      break;
    case PARSE_FIELD_DEFLINE:
      requested_field = ValNodeNew (NULL);
      requested_field->data.intvalue = Seq_descr_title;
      gsp = CheckForExistingTextInSeqEntry (sep, requested_field, 
                                            NULL,
                                            GetStringFromStringDescriptor,
                                            NULL, IntValNodeCopy,
                                            NULL, 0, 0, Seq_descr_title);
      requested_field = ValNodeFree (requested_field);
      break;
    case PARSE_FIELD_BIOSRC_STRING:
      gsp = CheckForExistingTextInSeqEntry (sep, dst_field_data->feature_field, 
                                            GetSourceFeatureString,
                                            GetSourceDescriptorString,
                                            NULL, IntValNodeCopy,
                                            NULL,
                                            SEQFEAT_BIOSRC, 0, Seq_descr_source);
      break;
    case PARSE_FIELD_GENE_FIELD:
      gsp = CheckForExistingTextInSeqEntry (sep, dst_field_data->feature_field, 
                                            GetGeneFieldString,
                                            NULL,
                                            NULL, IntValNodeCopy,
                                            NULL, SEQFEAT_GENE, 0, 0);
      break;
    case PARSE_FIELD_MRNA_FIELD:
      gsp = CheckForExistingTextInSeqEntry (sep, dst_field_data->feature_field, 
                                            GetmRNAFieldString,
                                            NULL,
                                            NULL, IntValNodeCopy,
                                            NULL, SEQFEAT_RNA, FEATDEF_mRNA, 0);
      break;
    case PARSE_FIELD_CDS_COMMENT:
      gsp = CheckForExistingTextInSeqEntry (sep, NULL, 
                                            GetCDSComment,
                                            NULL,
                                            NULL, IntValNodeCopy,
                                            NULL, SEQFEAT_CDREGION, FEATDEF_CDS, 0);
      break;
    case PARSE_FIELD_PROTEIN_FIELD:
      gsp = CheckForExistingTextInSeqEntry (sep, dst_field_data->feature_field, 
                                            GetProteinFieldString,
                                            NULL,
                                            NULL, IntValNodeCopy,
                                            NULL, SEQFEAT_PROT, FEATDEF_PROT, 0);
      break;
    case PARSE_FIELD_IMPORT_QUAL:
      gsp_sum = NULL;
      for (vnp = dst_field_data->feature_subtype; vnp != NULL; vnp = vnp->next)
      {
        gsp_new = CheckForExistingTextInSeqEntry (sep, dst_field_data->feature_field, 
                                                  GetGBQualString,
                                                  NULL,
                                                  NULL, IntValNodeCopy,
                                                  NULL, 0, vnp->choice, 0);
        gsp = GetSampleAdd (gsp_new, gsp_sum);
        gsp_new = GetSampleFree (gsp_new);
        gsp_sum = GetSampleFree (gsp_sum);
        gsp_sum = gsp;
      }
      break;
    case PARSE_FIELD_FEATURE_NOTE:
      gsp_sum = NULL;
      for (vnp = dst_field_data->feature_field; vnp != NULL; vnp = vnp->next)
      {
        gsp_new = CheckForExistingTextInSeqEntry (sep, dst_field_data->feature_field, 
                                                  GetFeatureNote,
                                                  NULL,
                                                  ValNodeSimpleDataFree,
                                                  ValNodeStringCopy,
                                                  NULL, 0, vnp->choice, 0);
        gsp = GetSampleAdd (gsp_new, gsp_sum);
        gsp_new = GetSampleFree (gsp_new);
        gsp_sum = GetSampleFree (gsp_sum);
        gsp_sum = gsp;
      }
      break;
  }
  return gsp;
}

static Boolean ParseFileToDest (ParseFormPtr mp, FilterSetPtr fsp, TextPortionPtr tp, SeqEntryPtr sep)
{
  ParseActionData     pad;
  FILE                *fp;
  Char                path [PATH_MAX];
  ReadBufferData      rbd;
  CharPtr             line;
  CharPtr             found_loc = NULL;
  Int4                found_len = 0;
  GetSamplePtr        gsp;

  if (mp == NULL || tp == NULL || sep == NULL)
  {
    return FALSE;
  }
  ResetParseActionVisitData (&pad);
  pad.fsp = fsp;
  pad.tp = tp;

  gsp = GetDestSample (sep, mp->dst_field_data);
  mp->etp = GetExistingTextHandlerInfo (gsp, FALSE);        
  gsp = GetSampleFree (gsp);
  if (mp->etp != NULL && mp->etp->existing_text_choice == EXISTING_TEXT_CHOICE_CANCEL)
  {
    mp->etp = MemFree (mp->etp);
    return FALSE;
  }
  

  if (! GetInputFileName (path, sizeof (path), "", "TEXT")) 
  {
    mp->etp = MemFree (mp->etp);
    return FALSE;
  }
  
  fp = FileOpen (path, "r");
  if (fp == NULL)
  {
    Message (MSG_ERROR, "Unable to open %s", path);
    mp->etp = MemFree (mp->etp);
    return FALSE;
  }

  ResetParseActionVisitData (&pad);
  pad.sep = sep;
  pad.title_sep = sep;
  pad.biop_sep = sep;

  rbd.fp = fp;
  rbd.current_data = NULL;
  line = AbstractReadFunction (&rbd);
  while (line != NULL)
  {
    found_loc = NULL;
    found_len = 0;
    FindTextPortionInString (line, tp, &found_loc, &found_len);
  
    if (found_loc != NULL)
    {
      ParseToDest (&pad, mp, found_loc, found_len);
    }
    line = AbstractReadFunction (&rbd);
  }

  FileClose (fp);
  mp->etp = MemFree (mp->etp);
  
  ObjMgrSetDirtyFlag (mp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, mp->input_entityID, 0, 0);
  ArrowCursor ();
  Update ();    
  return TRUE;
}

static GetSamplePtr
ParseFlatfileForSample
(SeqEntryPtr    sep,
 ParseActionPtr pap,
 ParseFormPtr mp,
 Uint1          format,
 GetSamplePtr   gsp)
{
  Asn2gbJobPtr     ajp;
  BioseqPtr        bsp = NULL;
  BioseqSetPtr     bssp = NULL;
  Int4             index;
  ErrSev           level;
  CharPtr          string;
  CharPtr          found_loc = NULL;
  Int4             found_len = 0;
  SeqDescrPtr      sdp;
  SeqAnnotPtr      sap;
  SeqEntryPtr      old_title_sep;
  SeqEntryPtr      old_biop_sep;
  GetSamplePtr     new_gsp, sum_gsp;

  if (sep == NULL || pap == NULL || mp == NULL || sep->data.ptrvalue == NULL)
  {
    return gsp;
  }
  
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sdp = bsp->descr;
    sap = bsp->annot;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sdp = bssp->descr;
    sap = bssp->annot;
  } else return gsp;
  
  old_title_sep = pap->title_sep;
  old_biop_sep = pap->biop_sep;
  pap->sep = sep;
  
  SetSourceAndTitle (pap, sdp, sap);

  if (bsp != NULL)
  {
    level = ErrSetMessageLevel (SEV_MAX);

    ajp = asn2gnbk_setup (bsp, bssp, NULL, (FmtType)format, SEQUIN_MODE, NORMAL_STYLE, 0, 0, 0, NULL);
    if (ajp != NULL) {
      for (index = 0; index < ajp->numParagraphs && found_loc == NULL; index++) {
        string = asn2gnbk_format (ajp, (Int4) index);
        if (string != NULL && *string != '\0') {
          CompressSpaces (string);
          FindTextPortionInString (string, pap->tp, &found_loc, &found_len);
  
          if (found_loc != NULL)
          {
            if (mp->dst_field_data->parse_field_type >= PARSE_FIELD_FIRST_FEATURE
                && mp->dst_field_data->parse_field_type <= PARSE_FIELD_LAST_FEATURE)
            {
              pap->sep = FindNucSeqEntry (sep);
            }
            new_gsp = GetDestSample (pap->sep, mp->dst_field_data);
            sum_gsp = GetSampleAdd (gsp, new_gsp);
            gsp = GetSampleFree (gsp);
            new_gsp = GetSampleFree (new_gsp);
            gsp = sum_gsp;
            sum_gsp = NULL;
          }
        }
        MemFree (string);
      }
      asn2gnbk_cleanup (ajp);
    }

    ErrSetMessageLevel (level);    
  }
  if (bssp != NULL)
  {
    gsp = ParseFlatfileForSample (bssp->seq_set, pap, mp, format, gsp);
  }
  pap->title_sep = old_title_sep;
  pap->biop_sep = old_biop_sep;
  
  gsp = ParseFlatfileForSample (sep->next, pap, mp, format, gsp);
  return gsp;
}

static void 
ParseFlatfileToDest 
(SeqEntryPtr    sep,
 ParseActionPtr pap,
 ParseFormPtr mp,
 Uint1          format)
{
  Asn2gbJobPtr     ajp;
  BioseqPtr        bsp = NULL;
  BioseqSetPtr     bssp = NULL;
  Int4             index;
  ErrSev           level;
  CharPtr          string;
  CharPtr          found_loc = NULL;
  Int4             found_len = 0;
  SeqDescrPtr      sdp;
  SeqAnnotPtr      sap;
  SeqEntryPtr      old_title_sep;
  SeqEntryPtr      old_biop_sep;

  if (sep == NULL || pap == NULL || mp == NULL || sep->data.ptrvalue == NULL)
  {
    return;
  }
  
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sdp = bsp->descr;
    sap = bsp->annot;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sdp = bssp->descr;
    sap = bssp->annot;
  } else return;
  
  old_title_sep = pap->title_sep;
  old_biop_sep = pap->biop_sep;
  pap->sep = sep;
  
  SetSourceAndTitle (pap, sdp, sap);

  if (bsp != NULL)
  {
    level = ErrSetMessageLevel (SEV_MAX);

    ajp = asn2gnbk_setup (bsp, bssp, NULL, (FmtType)format, SEQUIN_MODE, NORMAL_STYLE, 0, 0, 0, NULL);
    if (ajp != NULL) {
      for (index = 0; index < ajp->numParagraphs && found_loc == NULL; index++) {
        string = asn2gnbk_format (ajp, (Int4) index);
        if (string != NULL && *string != '\0') {
          CompressSpaces (string);
          FindTextPortionInString (string, pap->tp, &found_loc, &found_len);
  
          if (found_loc != NULL)
          {
            if (mp->dst_field_data->parse_field_type >= PARSE_FIELD_FIRST_FEATURE
                && mp->dst_field_data->parse_field_type <= PARSE_FIELD_LAST_FEATURE)
            {
              pap->sep = FindNucSeqEntry (sep);
            }

            ParseToDest (pap, mp, found_loc, found_len);
          }
        }
        MemFree (string);
      }
      asn2gnbk_cleanup (ajp);
    }

    ErrSetMessageLevel (level);    
  }
  if (bssp != NULL)
  {
    ParseFlatfileToDest (bssp->seq_set, pap, mp, format);
  }
  pap->title_sep = old_title_sep;
  pap->biop_sep = old_biop_sep;
  
  ParseFlatfileToDest (sep->next, pap, mp, format);
}

static Boolean ParseTextAction (Pointer data)
{
  ParseFormPtr      mp;
  SeqEntryPtr         sep;
  Char                path [PATH_MAX];
  ParseActionLogData  pald;
  ParseActionData     pad;
  ValNodePtr          list_vnp;
  Boolean             rval = FALSE;

  mp = (ParseFormPtr) data;
  if (mp == NULL)
  {
    return FALSE;
  }

  sep = GetTopSeqEntryForEntityID (mp->input_entityID);
  if (sep == NULL) 
  {
    return FALSE;
  }

  list_vnp = DialogToPointer (mp->source_field);
  if (list_vnp == NULL 
      || list_vnp->data.intvalue < PARSE_FIELD_SRC_DEFLINE 
      || list_vnp->data.intvalue > MAX_PARSE_FIELD)
  {
    ValNodeFree (list_vnp);
    return FALSE;
  }
  pad.src_field = list_vnp->data.intvalue;
  list_vnp = ValNodeFree (list_vnp);
  
  pad.tp = DialogToPointer (mp->text_portion);
  pad.fsp = NULL;
  pad.biop_sep = sep;
  pad.title_sep = sep;
  mp->dst_field_data = DialogToPointer (mp->destination_field);
  
  if (pad.src_field == PARSE_FIELD_SRC_GENBANK_FLATFILE)
  {
    WatchCursor ();
    ResetParseActionVisitData (&pad);
    
    mp->gsp = ParseFlatfileForSample (sep, &pad, mp, GENBANK_FMT, NULL);
    mp->etp = GetExistingTextHandlerInfo (mp->gsp, FALSE);        
    mp->gsp = GetSampleFree (mp->gsp);
    if (mp->etp == NULL || mp->etp->existing_text_choice != EXISTING_TEXT_CHOICE_CANCEL)
    {
      ResetParseActionVisitData (&pad);
      pad.biop_sep = sep;
      pad.title_sep = sep;
      ParseFlatfileToDest (sep, &pad, mp, GENBANK_FMT);
      ObjMgrSetDirtyFlag (mp->input_entityID, TRUE);
      ObjMgrSendMsg (OM_MSG_UPDATE, mp->input_entityID, 0, 0);
      rval = TRUE;
    }
    ArrowCursor ();
    Update ();    
  }
  else if (mp->dst_field_data != NULL)
  {
    switch (mp->dst_field_data->parse_field_type)
    {
      case PARSE_FIELD_SOURCE_QUAL:
      case PARSE_FIELD_DEFLINE:
      case PARSE_FIELD_BIOSRC_STRING:
      case PARSE_FIELD_GENE_FIELD:
      case PARSE_FIELD_MRNA_FIELD:
      case PARSE_FIELD_CDS_COMMENT:
      case PARSE_FIELD_PROTEIN_FIELD:
      case PARSE_FIELD_IMPORT_QUAL:
      case PARSE_FIELD_FEATURE_NOTE:
        WatchCursor ();
        Update ();
        
        ResetParseActionVisitData (&pad);
        VisitSeqEntriesForParseAction (sep, &pad, GetParseToExistingTextCallback, mp, FALSE);
        mp->etp = GetExistingTextHandlerInfo (mp->gsp, FALSE);        
        mp->gsp = GetSampleFree (mp->gsp);
        if (mp->etp == NULL || mp->etp->existing_text_choice != EXISTING_TEXT_CHOICE_CANCEL)
        {
          ResetParseActionVisitData (&pad);
          VisitSeqEntriesForParseAction (sep, &pad, ParseToDest, mp, FALSE);
          mp->etp = MemFree (mp->etp);
          ObjMgrSetDirtyFlag (mp->input_entityID, TRUE);
          ObjMgrSendMsg (OM_MSG_UPDATE, mp->input_entityID, 0, 0);
          rval = TRUE;
        }
        ArrowCursor ();
        Update ();    
        break;
      default:
        pald.data_in_log = FALSE;
        TmpNam (path);
        pald.fp = FileOpen (path, "wb");
        if (pald.fp != NULL)
        {
          VisitSeqEntriesForParseAction (sep, &pad, ParseActionSample, &pald, FALSE);
          FileClose (pald.fp);
          LaunchGeneralTextViewer (path, "Found Text");
          FileRemove (path);
          rval = TRUE;
        }
        break;
    }
  }
  
  pad.tp = TextPortionFree (pad.tp);
  FilterSetFree (pad.fsp);
  return rval;
}

static void ParseFormClear (Pointer data)
{
  ParseFormPtr      mp;

  mp = (ParseFormPtr) data;
  if (mp == NULL)
  {
    return;
  }
  
  PointerToDialog (mp->text_portion, NULL);
  PointerToDialog (mp->source_field, NULL);
  PointerToDialog (mp->destination_field, NULL);
}

static void ParseFormClearText (Pointer data)
{
  ParseFormPtr      mp;
  TextPortionPtr      tp;

  mp = (ParseFormPtr) data;
  if (mp == NULL)
  {
    return;
  }
  
  /* clear text portion text */
  tp = DialogToPointer (mp->text_portion);
  if (tp != NULL)
  {
    tp->start_text = MemFree (tp->start_text);
    tp->end_text = MemFree (tp->start_text);
  }
  PointerToDialog (mp->text_portion, tp);
  tp = MemFree (tp);
}

static void ParseChangeNotify (Pointer userdata)
{
  ParseFormPtr mp;
  ValNodePtr     err_list;

  mp = (ParseFormPtr) userdata;
  if (mp == NULL) return;

  err_list = TestDialog (mp->source_field);
  
  if (err_list == NULL)
  {
    err_list = TestDialog (mp->destination_field);
  }
  
  if (err_list == NULL)
  {
    EnableAcceptCancelDialogAccept (mp->accept_cancel);
  }
  else
  {
    DisableAcceptCancelDialogAccept (mp->accept_cancel);
  }
  ValNodeFree (err_list);
}

static void ParseText (Uint2 input_entityID, Int4 from_field, Int4 to_field)
{
  ParseFormPtr   mp;
  WindoW         w;
  GrouP          h, g1, g2, g3;
  ValNode        vn;
  ParseFieldData pd;

  mp = (ParseFormPtr) MemNew (sizeof (ParseFormData));
  if (mp == NULL) return;
  
  w = FixedWindow (-50, -33, -10, -10, "Parse Text", StdCloseWindowProc); 
  SetObjectExtra (w, mp, CleanupParseForm); 
  SetObjectExtra (w, mp, NULL);
  mp->form = (ForM) w;
  mp->input_entityID = input_entityID;

  mp->source_qual_choice_list = GetSourceQualDescList (TRUE, TRUE);
  mp->gsp = NULL;
  mp->etp = NULL;
  
  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);
  
  g1 = HiddenGroup (h, 2, 0, NULL);
  StaticPrompt (g1, "Select text", 0, dialogTextHeight, systemFont, 'l');
  mp->text_portion = TextPortionDialog (g1);
  
  g2 = HiddenGroup (h, 2, 0, NULL);
  StaticPrompt (g2, "From", 0, dialogTextHeight, systemFont, 'l');
  mp->source_field = ParseFieldSourceDialog (g2, ParseChangeNotify, mp);
  
  g3 = HiddenGroup (h, 2, 0, NULL);
  StaticPrompt (g3, "And place in", 0, dialogTextHeight, systemFont, 'l');
  mp->destination_field = ParseFieldDestDialog (g3, ParseChangeNotify, mp);
  
  mp->accept_cancel = AcceptCancelDialog (h, ParseTextAction, NULL, ParseFormClear, ParseFormClearText, (Pointer)mp, w);  
  AlignObjects (ALIGN_CENTER, (HANDLE) g1,
                              (HANDLE) g2,
                              (HANDLE) g3,
                              (HANDLE) mp->accept_cancel,
                              NULL);
  Show (w); 
  
  if (from_field > 0)
  {
    vn.data.intvalue = from_field;
    vn.next = NULL;
    PointerToDialog (mp->source_field, &vn);
  }
  if (to_field > 0)
  {
    pd.parse_field_type = to_field;
    pd.feature_field = NULL;
    pd.feature_subtype = NULL;
    PointerToDialog (mp->destination_field, &pd);
  }
}

static void ParseTextItem (IteM i, Int4 from_field, Int4 to_field)
{
  BaseFormPtr    bfp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  ParseText (bfp->input_entityID, from_field, to_field);  
}

extern void ParseDefLineToSourceQual (IteM i)
{
  ParseTextItem (i, PARSE_FIELD_SRC_DEFLINE, PARSE_FIELD_SOURCE_QUAL);
}

extern void ParseFlatfileToSourceQual (IteM i)
{
  ParseTextItem (i, PARSE_FIELD_SRC_GENBANK_FLATFILE, PARSE_FIELD_SOURCE_QUAL);
}

static void ParseTextToolBtn (ButtoN b, Int4 from_field, Int4 to_field)
{
  BaseFormPtr bfp;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;

  ParseText (bfp->input_entityID, from_field, to_field);
}

extern void ParseLocalIDToSourceQual (ButtoN b)
{
  ParseTextToolBtn (b, PARSE_FIELD_SRC_LOCAL_ID, PARSE_FIELD_SOURCE_QUAL);
}


#define NUM_FEAT_ED_PAGES 2

#define FEAT_ED_EVIDENCE  1
#define FEAT_ED_EXCEPTION 2
#define FEAT_ED_PARTIAL   3
#define FEAT_ED_STRAND    4
#define FEAT_ED_CITATION  5

typedef struct mockupfeated
{
  FORM_MESSAGE_BLOCK
  DialoG  feature_select;
  DialoG  constraints;
  DialoG  accept_cancel;

  /* for evidence */  
  PopuP   evidence_choice;
  Int4    evidence_val;
      
  /* for exceptions */
  PopuP   explanation_choice;
  ButtoN  move_to_comment;
  GrouP   product_choice;
  TexT    other_explanation;
  CharPtr explanation_text;
  Boolean do_move_to_comment;
  Int4    product_val;
  
  /* for partials */
  PopuP  convert_choice;
  PopuP  partial5_choice;
  ButtoN extend5_btn;
  PopuP  partial3_choice;
  ButtoN extend3_btn;
  Int4   orderjoinpolicy;
  Int4   leftpolicy;
  Int4   rightpolicy;
  Boolean extend5;
  Boolean extend3;
  
  /* for strand */
  PopuP  from_strand;
  PopuP  to_strand;
  Boolean from_strand_val;
  Boolean to_strand_val;

  GrouP  action_grps [5];
  ButtoN action_btns [5];
  GrouP  action_choice_grp;
  
  /* for citations */
  DialoG     citation_list;
  ButtoN     explanation_constraint;
  PopuP      explanation_constraint_choice;
  ButtoN     citation_translation;
  Boolean    use_explanation_constraint;
  Int4       explanation_constraint_choice_val;
  Boolean    use_product_match;
  
} MockupFeatEdData, PNTR MockupFeatEdPtr;

static void MockupFeatEdChangeNotify (Pointer userdata)
{
  MockupFeatEdPtr mp;
  ValNodePtr      err_list;
  CharPtr         str;

  mp = (MockupFeatEdPtr) userdata;
  if (mp == NULL) return;
  
  if (mp->explanation_choice != NULL)
  {
    if (GetValue (mp->explanation_choice) == 9 
        && ((mp->action_btns[1] == NULL && mp->action_choice_grp == NULL)
            || (mp->action_btns[1] != NULL && GetStatus (mp->action_btns[1]))
            || (mp->action_choice_grp != NULL && GetValue (mp->action_choice_grp) == 2)))
    {
      Enable (mp->other_explanation);
      str = SaveStringFromText (mp->other_explanation);
      if (StringHasNoText (str))
      {
        DisableAcceptCancelDialogAccept (mp->accept_cancel);
        str = MemFree (str);
        return;
      }
      str = MemFree (str);
    }
    else
    {
      Disable (mp->other_explanation);
    }
  }
  
  if (mp->action_btns[0] != NULL)
  {
    if (! GetStatus (mp->action_btns[0])
        && ! GetStatus (mp->action_btns[1])
        && ! GetStatus (mp->action_btns[2])
        && ! GetStatus (mp->action_btns[3]))
    {
      DisableAcceptCancelDialogAccept (mp->accept_cancel);
      return;
    }
  }

  err_list = TestDialog (mp->feature_select);
  
  if (err_list == NULL)
  {
    EnableAcceptCancelDialogAccept (mp->accept_cancel);
  }
  else
  {
    DisableAcceptCancelDialogAccept (mp->accept_cancel);
  }
  ValNodeFree (err_list);  
}

static void MockupFeatEdClear (Pointer data)
{
  MockupFeatEdPtr mp;

  mp = (MockupFeatEdPtr) data;
  if (mp == NULL) return;
 
  PointerToDialog (mp->feature_select, NULL);
  PointerToDialog (mp->constraints, NULL);

  if (mp->evidence_choice != NULL)
  {
    SetValue (mp->evidence_choice, 1);
  }
  if (mp->explanation_choice != NULL)
  {
    SetValue (mp->explanation_choice, 1);
  }
  if (mp->move_to_comment != NULL)
  {
    SetStatus (mp->move_to_comment, FALSE);
  }
  if (mp->product_choice != NULL)
  {
    SetValue (mp->product_choice, 3);
  }
  if (mp->other_explanation != NULL)
  {
    SetTitle (mp->other_explanation, "");
    Disable (mp->other_explanation);
  }
  
  if (mp->convert_choice != NULL)
  {
    SetValue (mp->convert_choice, 1);
  }
  if (mp->partial5_choice != NULL)
  {
    SetValue (mp->partial5_choice, 8);
  }
  if (mp->extend5_btn != NULL)
  {
    SetStatus (mp->extend5_btn, FALSE);
  }
  if (mp->partial3_choice != NULL)
  {
    SetValue (mp->partial3_choice, 7);
  }
  if (mp->extend3_btn != NULL)
  {
    SetStatus (mp->extend3_btn, FALSE);
  }

  /* for strand */
  if (mp->from_strand != NULL)
  {
    SetValue (mp->from_strand, 1);
  }
  if (mp->to_strand != NULL)
  {
    SetValue (mp->to_strand, 1);
  }
  MockupFeatEdChangeNotify (mp);
}

static void MockupFeatEdClearText (Pointer data)
{
  MockupFeatEdPtr     mp;
  FilterSetPtr        fsp;

  mp = (MockupFeatEdPtr) data;
  if (mp == NULL) return;
 
  /* clear text from constraints */
  fsp = DialogToPointer (mp->constraints);
  FilterSetClearText (fsp);
  PointerToDialog (mp->constraints, fsp);
  FilterSetFree (fsp);
  
  if (mp->other_explanation != NULL)
  {
    SetTitle (mp->other_explanation, "");
  }
  MockupFeatEdChangeNotify (mp);
}

static GrouP MockupFeatureEvidenceGroup (GrouP h, MockupFeatEdPtr mp)
{
  GrouP g;
  
  if (mp == NULL)
  {
    return NULL;
  }

  g = HiddenGroup (h, 2, 0, NULL);
  SetGroupSpacing (g, 10, 10);
  
  StaticPrompt (g, "Set Evidence to ", 0, dialogTextHeight, systemFont, 'l');
  mp->evidence_choice = PopupList (g, TRUE, NULL);
  PopupItem (mp->evidence_choice, " ");
  PopupItem (mp->evidence_choice, "Experimental");
  PopupItem (mp->evidence_choice, "Non-Experimental");
  SetValue (mp->evidence_choice, 1);
  
  return g;
}

static void MockupFeatExplanationChange (PopuP p)
{
  MockupFeatEdPtr mp;

  mp = (MockupFeatEdPtr) GetObjectExtra (p);
  if (mp == NULL) return;
  
  if (GetValue (mp->explanation_choice) == 9)
  {
    Enable (mp->other_explanation);
  }
  else
  {
    Disable (mp->other_explanation);
  }
  
  MockupFeatEdChangeNotify (mp);
}

static void MockupFeatOtherExplanationChange (TexT t)
{
  MockupFeatEdPtr mp;
 
  mp = (MockupFeatEdPtr) GetObjectExtra (t);
  MockupFeatEdChangeNotify (mp);
}

static CharPtr exception_explanations[] = 
{
  "RNA Editing", "reasons given in citation", "ribosomal slippage",
  "trans splicing", "artificial frameshift", "nonconsensus splice site",
  "rearrangement required", "other->"
};

static Int4 num_exception_explanations = sizeof (exception_explanations) / sizeof (CharPtr);

static GrouP MockupFeatureExceptionGroup (GrouP h, MockupFeatEdPtr mp)
{
  GrouP  g, k1, k2;
  Int4   i;
  
  if (mp == NULL)
  {
    return NULL;
  }
  g = HiddenGroup (h, -1, 0, NULL);
  SetGroupSpacing (g, 10, 10);
  
  k1 = HiddenGroup (g, 3, 0, NULL);
  SetGroupSpacing (k1, 10, 10);
  StaticPrompt (k1, "Set Explanation to ", 0, dialogTextHeight, systemFont, 'l');
  mp->explanation_choice = PopupList (k1, TRUE, MockupFeatExplanationChange);
  SetObjectExtra (mp->explanation_choice, mp, NULL);
  PopupItem (mp->explanation_choice, " ");
  for (i = 0; i < num_exception_explanations; i++)
  {
    PopupItem (mp->explanation_choice, exception_explanations [i]);
  }
  SetValue (mp->explanation_choice, 1);
  mp->other_explanation = DialogText (k1, "", 10, MockupFeatOtherExplanationChange);
  Disable (mp->other_explanation);
  SetObjectExtra (mp->other_explanation, mp, NULL);

  k2 = HiddenGroup (g, 2, 0, NULL);
  SetGroupSpacing (k2, 10, 10);
  StaticPrompt (k2, "Where feature product is", 0, dialogTextHeight, systemFont, 'l');
  mp->product_choice = HiddenGroup (k2, 3, 0, NULL);
  RadioButton (mp->product_choice, "Present");
  RadioButton (mp->product_choice, "Absent");
  RadioButton (mp->product_choice, "Either");
  SetValue (mp->product_choice, 3);
  
  mp->move_to_comment = CheckBox (g, "Move explanation to comment", NULL);

  AlignObjects (ALIGN_CENTER, (HANDLE) k1,
                              (HANDLE) k2,
                              (HANDLE) mp->move_to_comment,
                              NULL);
  
  return g;
}

static void ChangeExplanationConstraint (ButtoN b)
{
  MockupFeatEdPtr mp;
  
  mp = (MockupFeatEdPtr) GetObjectExtra (b);
  if (mp == NULL)
  {
    return;
  }
  if (GetStatus (mp->explanation_constraint))
  {
    Enable (mp->explanation_constraint_choice);
  }
  else
  {
    Disable (mp->explanation_constraint_choice);
  }  
}

static GrouP FeatureEditorCitationGroup (GrouP h, MockupFeatEdPtr mp)
{
  GrouP  g, k1, k2;
  Int4   i;
  
  if (mp == NULL)
  {
    return NULL;
  }
  g = HiddenGroup (h, -1, 0, NULL);
  SetGroupSpacing (g, 10, 10);
  
  /* need citation selector */
  mp->citation_list = FeatCitEditDialog (g, mp->input_entityID);
  PointerToDialog (mp->citation_list, NULL);

  k1 = HiddenGroup (g, 3, 0, NULL);
  SetGroupSpacing (k1, 10, 10);
  mp->explanation_constraint = CheckBox (k1, "Where explanation is", ChangeExplanationConstraint);
  SetObjectExtra (mp->explanation_constraint, mp, NULL);
  SetStatus (mp->explanation_constraint, TRUE);
  mp->explanation_constraint_choice = PopupList (k1, TRUE, NULL);
  PopupItem (mp->explanation_constraint_choice, "Any");
  PopupItem (mp->explanation_constraint_choice, " ");
  for (i = 0; i < num_exception_explanations; i++)
  {
    PopupItem (mp->explanation_constraint_choice, exception_explanations [i]);
  }
  SetValue (mp->explanation_constraint_choice, 4);

  k2 = HiddenGroup (g, 2, 0, NULL);
  SetGroupSpacing (k2, 10, 10);
  mp->citation_translation = CheckBox (k2, "Where translation does not match product", NULL);

  AlignObjects (ALIGN_CENTER, (HANDLE) mp->citation_list,
                              (HANDLE) k1,
                              (HANDLE) k2,
                              (HANDLE) mp->citation_translation,
                              NULL);
  
  return g;
}

static GrouP FeatureEditorPartialGroup (GrouP h, MockupFeatEdPtr mp)
{
  GrouP g, k1, k2;
  
  if (mp == NULL)
  {
    return NULL;
  }
  
  g = HiddenGroup (h, -1, 0, NULL);
  SetGroupSpacing (g, 10, 10);
  
  mp->convert_choice = PopupList (g, TRUE, NULL);
  PopupItem (mp->convert_choice, "Do not convert location");
  PopupItem (mp->convert_choice, "Convert location to join");
  PopupItem (mp->convert_choice, "Convert location to order");
  PopupItem (mp->convert_choice, "Convert location to single interval");
  SetValue (mp->convert_choice, 1);
  
  k1 = HiddenGroup (g, 2, 0, NULL);
  SetGroupSpacing (k1, 10, 10);
  StaticPrompt (k1, "5' partial:", 0, dialogTextHeight, systemFont, 'l');
  mp->partial5_choice = PopupList (k1, TRUE, NULL);
  PopupItem (mp->partial5_choice, "Set");
  PopupItem (mp->partial5_choice, "Set only if at 5' end");
  PopupItem (mp->partial5_choice, "Set if bad start codon");
  PopupItem (mp->partial5_choice, "Set if CDS frame > 1");
  PopupItem (mp->partial5_choice, "Clear");
  PopupItem (mp->partial5_choice, "Clear if not at 5' end");
  PopupItem (mp->partial5_choice, "Clear if good start codon");
  PopupItem (mp->partial5_choice, "Do not change");
  SetValue (mp->partial5_choice, 8);
  
  mp->extend5_btn = CheckBox (g, "Extend to 5' end of sequence if setting 5' partial", NULL);
  
  k2 = HiddenGroup (g, 2, 0, NULL);
  SetGroupSpacing (k2, 10, 10);
  StaticPrompt (k2, "3' partial:", 0, dialogTextHeight, systemFont, 'l');
  mp->partial3_choice = PopupList (k2, TRUE, NULL);
  PopupItem (mp->partial3_choice, "Set");
  PopupItem (mp->partial3_choice, "Set only if at 3' end");
  PopupItem (mp->partial3_choice, "Set if bad stop codon");
  PopupItem (mp->partial3_choice, "Clear");
  PopupItem (mp->partial3_choice, "Clear if not at 3' end");
  PopupItem (mp->partial3_choice, "Clear if good stop codon");
  PopupItem (mp->partial3_choice, "Do not change");
  SetValue (mp->partial3_choice, 7);
  
  mp->extend3_btn = CheckBox (g, "Extend to 3' end of sequence if setting 3' partial", NULL);
  
  AlignObjects (ALIGN_CENTER, (HANDLE) mp->convert_choice,
                              (HANDLE) k1,
                              (HANDLE) mp->extend5_btn,
                              (HANDLE) k2,
                              (HANDLE) mp->extend3_btn,
                              NULL);
  
  return g;
}

#define FEATED_STRAND_ANY_REVERSE 1
#define FEATED_STRAND_UNKNOWN     2
#define FEATED_STRAND_PLUS        3
#define FEATED_STRAND_MINUS       4
#define FEATED_STRAND_BOTH        5

static GrouP FeatureEditorStrandGroup (GrouP h, MockupFeatEdPtr mp)
{
  GrouP  g;
  
  if (mp == NULL)
  {
    return NULL;
  }
  g = HiddenGroup (h, 5, 0, NULL);
  SetGroupSpacing (g, 10, 10);
  StaticPrompt (g, "Convert location strand", 0, dialogTextHeight, systemFont, 'l');
  StaticPrompt (g, "From", 0, dialogTextHeight, systemFont, 'l');
  mp->from_strand = PopupList (g, TRUE, NULL);
  PopupItem (mp->from_strand, "Any");
  PopupItem (mp->from_strand, "Unknown");
  PopupItem (mp->from_strand, "Plus");
  PopupItem (mp->from_strand, "Minus");
  PopupItem (mp->from_strand, "Both");
  SetValue (mp->from_strand, FEATED_STRAND_PLUS); 
  StaticPrompt (g, "To", 0, dialogTextHeight, systemFont, 'l');
  mp->to_strand = PopupList (g, TRUE, NULL);
  PopupItem (mp->to_strand, "Reverse");
  PopupItem (mp->to_strand, "Unknown");
  PopupItem (mp->to_strand, "Plus");
  PopupItem (mp->to_strand, "Minus");
  PopupItem (mp->to_strand, "Both");
  SetValue (mp->to_strand, FEATED_STRAND_MINUS); 
  return g;
}

static void ChangeFeatureEditorActionButton (ButtoN b)
{
  MockupFeatEdPtr mp;
  Int4            i;
  
  mp = (MockupFeatEdPtr) GetObjectExtra (b);
  if (mp == NULL)
  {
    return;
  }
  
  for (i = 0; i < 4; i++)
  {
    if (GetStatus (mp->action_btns[i]))
    {
      Enable (mp->action_grps[i]);
    }
    else
    {
      Disable (mp->action_grps[i]);
    }
  }

  MockupFeatEdChangeNotify (mp);
  
}

static void ChangeFeatureEditorActionGroup (GrouP g)
{
  MockupFeatEdPtr mp;
  Int4            i;
  
  mp = (MockupFeatEdPtr) GetObjectExtra (g);
  if (mp == NULL)
  {
    return;
  }
  
  for (i = 0; i < 5; i++)
  {
    Hide (mp->action_grps[i]);
  }
  
  i = GetValue (mp->action_choice_grp);
  if (i > 0 && i < 6)
  {
    Show (mp->action_grps [i-1]);
  }

  MockupFeatEdChangeNotify (mp);
  
}

static GrouP FeatureEditorActionGroup (GrouP h, MockupFeatEdPtr mp, Int4 first_action)
{
  GrouP g, k, n = NULL;
  PrompT p1;
  
  if (mp == NULL)
  {
    return NULL;
  }
  g = HiddenGroup (h, -1, 0, NULL);

  p1 = StaticPrompt (g, "For", 0, dialogTextHeight, systemFont, 'l');
  mp->feature_select = FeatureSelectionDialog (g, TRUE, MockupFeatEdChangeNotify, mp);
  
  k = HiddenGroup (g, -1, 0, NULL);
  mp->action_choice_grp = HiddenGroup (g, 5, 0, ChangeFeatureEditorActionGroup);
  SetObjectExtra (mp->action_choice_grp, mp, NULL);
  RadioButton (mp->action_choice_grp, "Evidence");
  RadioButton (mp->action_choice_grp, "Exceptions");
  RadioButton (mp->action_choice_grp, "Partials");
  RadioButton (mp->action_choice_grp, "Strands");
  RadioButton (mp->action_choice_grp, "Citations");
  if (first_action >= FEAT_ED_EVIDENCE && first_action <= FEAT_ED_CITATION)
  {
    SetValue (mp->action_choice_grp, first_action);
  }
  else
  {
    SetValue (mp->action_choice_grp, FEAT_ED_EVIDENCE);
  }
    
  n = HiddenGroup (g, 0, 0, NULL);
  mp->action_grps [0] = MockupFeatureEvidenceGroup (n, mp);
  mp->action_grps[1] = MockupFeatureExceptionGroup (n, mp);
  mp->action_grps[2] = FeatureEditorPartialGroup (n, mp);
  mp->action_grps[3] = FeatureEditorStrandGroup (n, mp);
  mp->action_grps[4] = FeatureEditorCitationGroup (n, mp);
  AlignObjects (ALIGN_CENTER, (HANDLE) mp->action_grps [0],
                              (HANDLE) mp->action_grps [1],
                              (HANDLE) mp->action_grps [2],
                              (HANDLE) mp->action_grps [3],
                              (HANDLE) mp->action_grps [4], NULL);
  AlignObjects (ALIGN_CENTER, (HANDLE) mp->action_choice_grp, (HANDLE) n, NULL);
  
  AlignObjects (ALIGN_CENTER, (HANDLE) p1,
                              (HANDLE) mp->feature_select,
                              (HANDLE) k, (HANDLE) n, NULL);
      
  return g;
}

static void DoEvidenceFeatureProc (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  MockupFeatEdPtr mp;

  if (sfp == NULL || userdata == NULL)
  {
    return;
  }
  
  mp = (MockupFeatEdPtr) userdata;
  
  sfp->exp_ev = (Uint1) mp->evidence_val;
}

static void DoExceptionFeatureProc (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  MockupFeatEdPtr  mp;
  ExistingTextData etd;
  CharPtr          old_explanation = NULL;

  if (sfp == NULL || userdata == NULL)
  {
    return;
  }
  
  mp = (MockupFeatEdPtr) userdata;
  
  if ((mp->product_val == 1 && sfp->product != NULL)
      || (mp->product_val == 2 && sfp->product == NULL)
      || mp->product_val == 3)
  {
    /* first, take care of any pre-existing exception explanation */
    old_explanation = sfp->except_text;
    sfp->except_text = NULL;
    if (mp->do_move_to_comment)
    {
      etd.existing_text_choice = EXISTING_TEXT_CHOICE_APPEND_SEMI;
      sfp->comment = HandleExistingText (sfp->comment, old_explanation, &etd);
      old_explanation = NULL;
    }
    else
    {
      old_explanation = MemFree (old_explanation);
    }
    
    if (StringHasNoText (mp->explanation_text))
    {
      sfp->excpt = FALSE;
    }
    else
    {
      sfp->excpt = TRUE;
      sfp->except_text = StringSave (mp->explanation_text);
    }
  }
}

#define DO_NOT_CONVERT             1
#define CONVERT_TO_JOIN            2
#define CONVERT_TO_ORDER           3
#define CONVERT_TO_SINGLE_INTERVAL 4

static void DoPartialFeatureProc (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  MockupFeatEdPtr mp;
  Boolean      partial5, partial3;
  Boolean      dash_at_end = FALSE;
  Boolean      star_at_end = FALSE;
  ByteStorePtr bs;
  CharPtr      prot;
  CharPtr      ptr;
  Char         first_char, ch;
  Boolean      hasNulls;
  SeqLocPtr    slp, firstSlp = NULL, lastSlp = NULL;
  Uint1        strand;
  Boolean      atEnd;
  BioseqPtr    bsp;
  CdRegionPtr  crp = NULL;
  
  if (sfp == NULL || sfp->location == NULL || userdata == NULL)
  {
    return;
  }
  
  mp = (MockupFeatEdPtr) userdata;
  
  CheckSeqLocForPartial (sfp->location, &partial5, &partial3);
  bsp = BioseqFindFromSeqLoc (sfp->location);

  if (sfp->idx.subtype == FEATDEF_CDS) 
  {
    crp = (CdRegionPtr) sfp->data.value.ptrvalue;
    bs = ProteinFromCdRegionEx (sfp, TRUE, FALSE);
    if (bs != NULL) 
    {
      prot = BSMerge (bs, NULL);
      bs = BSFree (bs);
      if (prot != NULL) 
      {
        first_char = *prot;
        ptr = prot;
        ch = *ptr;
        if (ch == '-') 
        {
          dash_at_end = TRUE;
        }
        while (ch != '\0') 
        {
          /* *ptr = TO_UPPER (ch); */
          if (ch == '*') 
          {
            star_at_end = TRUE;
          } else {
            star_at_end = FALSE;
          }
          ptr++;
          ch = *ptr;
        }
      }
      prot = MemFree (prot);
    }
  }

	/* If requested, convert joins to orders */
	/* or orders to joins.                   */

	hasNulls = LocationHasNullsBetween (sfp->location);
	switch (mp->orderjoinpolicy) 
	{
	  case CONVERT_TO_JOIN :
	    if (hasNulls) 
	    {
	      slp = SeqLocMerge (bsp, sfp->location, NULL, FALSE, FALSE, FALSE);
		    sfp->location = SeqLocFree (sfp->location);
		    sfp->location = slp;
		    if (bsp->repr == Seq_repr_seg) 
		    {
		      slp = SegLocToPartsEx (bsp, sfp->location, FALSE);
		      sfp->location = SeqLocFree (sfp->location);
		      sfp->location = slp;
		      hasNulls = LocationHasNullsBetween (sfp->location);
		      sfp->partial = (sfp->partial || hasNulls);
		    }
		    FreeAllFuzz (sfp->location);
		    SetSeqLocPartial (sfp->location, partial5, partial3);
	    }
	    break;
  	case CONVERT_TO_ORDER :
	    if (!hasNulls) 
	    {
		    slp = SeqLocMerge (bsp, sfp->location, NULL, FALSE, FALSE, TRUE);
        sfp->location = SeqLocFree (sfp->location);
		    sfp->location = slp;
		    if (bsp->repr == Seq_repr_seg) 
		    {
		      slp = SegLocToPartsEx (bsp, sfp->location, TRUE);
		      sfp->location = SeqLocFree (sfp->location);
		      sfp->location = slp;
		      hasNulls = LocationHasNullsBetween (sfp->location);
		      sfp->partial = (sfp->partial || hasNulls);
		    }
		    FreeAllFuzz (sfp->location);
		    SetSeqLocPartial (sfp->location, partial5, partial3);
	    }
	    break;
	  case CONVERT_TO_SINGLE_INTERVAL :
	    slp = SeqLocMerge (bsp, sfp->location, NULL, TRUE, FALSE, FALSE);
	    sfp->location = SeqLocFree (sfp->location);
	    sfp->location = slp;
		  SetSeqLocPartial (sfp->location, partial5, partial3);
	  default:
	    break;
	}

  /* find first and last location parts in a multi-part location */
  slp = SeqLocFindNext (sfp->location, NULL);
  while (slp != NULL) 
  {
    if (firstSlp == NULL) 
    {
      firstSlp = slp;
    }
    lastSlp = slp;
    slp = SeqLocFindNext (sfp->location, slp);
  }

  /* Set the 5' partial */

  if (firstSlp != NULL) 
  {
    strand = SeqLocStrand (firstSlp);
    atEnd = FALSE;
    if (strand == Seq_strand_minus) 
    {
      if (GetOffsetInBioseq (firstSlp, bsp, SEQLOC_START) == bsp->length - 1)
		  {
        atEnd = TRUE;
      }
    } else {
      if (GetOffsetInBioseq (firstSlp, bsp, SEQLOC_START) == 0) 
      {
        atEnd = TRUE;
      }
    }
    switch (mp->leftpolicy) 
    {
      case 1 :
        partial5 = TRUE;
        if (! atEnd && mp->extend5)
        {
          ExtendSeqLocToEnd (sfp->location, bsp, TRUE);
        }
        break;
      case 2 :
        if (atEnd) 
        {
          partial5 = TRUE;
        }
        break;
      case 3 :
        if (crp != NULL && (crp->frame > 1 || first_char != 'M')) 
        {
          partial5 = TRUE;
          if (! atEnd && mp->extend5)
          {
            ExtendSeqLocToEnd (sfp->location, bsp, TRUE);
          }
        }
        break;
      case 4:
        if (crp != NULL && crp->frame > 1)
        {
          partial5 = TRUE;
          if (! atEnd && mp->extend5)
          {
            ExtendSeqLocToEnd (sfp->location, bsp, TRUE);
          }
        }
        break;
      case 5 :
        partial5 = FALSE;
        break;
      case 6 :
        if (! atEnd) 
        {
          partial5 = FALSE;
        }
        break;
      case 7 :
        if (crp != NULL && crp->frame == 1 && first_char == 'M')
        {
          partial5 = FALSE;
        }
        break;
      default :
        break;
    }
  }

  /* Set the 3' partial */

  if (lastSlp != NULL) 
  {
    strand = SeqLocStrand (firstSlp);
    atEnd = FALSE;
    if (strand == Seq_strand_minus) 
    {
      if (GetOffsetInBioseq (lastSlp, bsp, SEQLOC_STOP) == 0) 
      {
        atEnd = TRUE;
      }
    } 
    else 
    {
      if (GetOffsetInBioseq (lastSlp, bsp, SEQLOC_STOP) == bsp->length - 1) 
      {
        atEnd = TRUE;
      }
    }
    switch (mp->rightpolicy) 
    {
      case 1 :
        partial3 = TRUE;
        if (! atEnd && mp->extend3)
        {
          ExtendSeqLocToEnd (sfp->location, bsp, FALSE);
        }
        break;
      case 2 :
        if (atEnd) 
        {
          partial3 = TRUE;
        }
        break;
      case 3 :
        if ((! star_at_end) && crp != NULL) 
        {
          partial3 = TRUE;
          if (! atEnd && mp->extend3)
          {
            ExtendSeqLocToEnd (sfp->location, bsp, FALSE);
          }
        }
        break;
      case 4 :
        partial3 = FALSE;
        break;
      case 5 :
        if (! atEnd) 
        {
          partial3 = FALSE;
        }
        break;
      case 6 :
        if (star_at_end && crp != NULL) 
        {
          partial3 = FALSE;
        }
        break;
      default :
        break;
    }
  }

  SetSeqLocPartial (sfp->location, partial5, partial3);
  sfp->partial = (partial5 || partial3 ||
                  LocationHasNullsBetween (sfp->location));
  if (partial5)
  {
   	SetBestFrame (sfp);
  }
}

static Boolean DoesStrandMatch (Int4 strand_choice, Uint1 strand_val)
{
  Boolean rval = FALSE;
  
  switch (strand_choice)
  {
    case FEATED_STRAND_ANY_REVERSE:
      rval = TRUE;
      break;
    case FEATED_STRAND_UNKNOWN:
      if (strand_val == Seq_strand_unknown)
      {
        rval = TRUE;
      }
      break;
    case FEATED_STRAND_PLUS:
      if (strand_val == Seq_strand_plus)
      {
        rval = TRUE;
      }
      break;
    case FEATED_STRAND_MINUS:
      if (strand_val == Seq_strand_minus)
      {
        rval = TRUE;
      }
      break;
    case FEATED_STRAND_BOTH:
      if (strand_val == Seq_strand_both)
      {
        rval = TRUE;
      }
      break;
  }
  return rval;
}

static Uint1 GetNewStrandValue (Int4 strand_choice, Uint1 strand_val)
{
  Uint1 rval = Seq_strand_unknown;
  
  switch (strand_choice)
  {
    case FEATED_STRAND_ANY_REVERSE:
      switch (strand_val)
      {
        case Seq_strand_plus:
          rval = Seq_strand_minus;
          break;
        case Seq_strand_minus:
          rval = Seq_strand_plus;
          break;
        default:
          rval = strand_val;
          break;
      }
      break;
    case FEATED_STRAND_UNKNOWN:
      rval = Seq_strand_unknown;
      break;
    case FEATED_STRAND_PLUS:
      rval = Seq_strand_plus;
      break;
    case FEATED_STRAND_MINUS:
      rval = Seq_strand_minus;
      break;
    case FEATED_STRAND_BOTH:
      rval = Seq_strand_both;
      break;
  }  
  return rval;
}

static void ConvertLocationStrand (SeqLocPtr slp, Int4 fromStrand, Int4 toStrand)
{
  SeqLocPtr      loc;
  PackSeqPntPtr  psp;
  SeqBondPtr     sbp;
  SeqIntPtr      sinp;
  SeqPntPtr      spp;

  while (slp != NULL) {
    switch (slp->choice) {
      case SEQLOC_NULL :
        break;
      case SEQLOC_EMPTY :
      case SEQLOC_WHOLE :
        break;
      case SEQLOC_INT :
        sinp = (SeqIntPtr) slp->data.ptrvalue;
        if (sinp != NULL && DoesStrandMatch (fromStrand, sinp->strand)) 
        {
          sinp->strand = GetNewStrandValue (toStrand, sinp->strand);
        }
        break;
      case SEQLOC_PNT :
        spp = (SeqPntPtr) slp->data.ptrvalue;
        if (spp != NULL && DoesStrandMatch (fromStrand, spp->strand))
        {
          spp->strand = GetNewStrandValue (toStrand, spp->strand);
        }
        break;
      case SEQLOC_PACKED_PNT :
        psp = (PackSeqPntPtr) slp->data.ptrvalue;
        if (psp != NULL && DoesStrandMatch (fromStrand, psp->strand)) 
        {
          psp->strand = GetNewStrandValue (toStrand, psp->strand);
        }
        break;
      case SEQLOC_PACKED_INT :
      case SEQLOC_MIX :
      case SEQLOC_EQUIV :
        loc = (SeqLocPtr) slp->data.ptrvalue;
        while (loc != NULL) {
          ConvertLocationStrand (loc, fromStrand, toStrand);
          loc = loc->next;
        }
        break;
      case SEQLOC_BOND :
        sbp = (SeqBondPtr) slp->data.ptrvalue;
        if (sbp != NULL) {
          spp = (SeqPntPtr) sbp->a;
          if (spp != NULL && DoesStrandMatch (fromStrand, spp->strand)) 
          {
            spp->strand = GetNewStrandValue (toStrand, spp->strand);
          }
          spp = (SeqPntPtr) sbp->b;
          if (spp != NULL && DoesStrandMatch (fromStrand, spp->strand)) 
          {
            spp->strand = GetNewStrandValue (toStrand, spp->strand);
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

static void DoStrandFeatureProc (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  MockupFeatEdPtr mp;
  
  if (sfp == NULL || userdata == NULL)
  {
    return;
  }
  
  mp = (MockupFeatEdPtr) userdata;
  
  ConvertLocationStrand (sfp->location, mp->from_strand_val, mp->to_strand_val);
  
}

static void DoCitationFeatureProc (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  MockupFeatEdPtr mp;
  ByteStorePtr    bs;
  BioseqPtr       protbsp;
  CharPtr         seq1 = NULL, seq2 = NULL;
  Boolean         add_citations = TRUE;
  
  if (sfp == NULL || userdata == NULL)
  {
    return;
  }
  
  mp = (MockupFeatEdPtr) userdata;
  
  if (mp->use_explanation_constraint && mp->explanation_constraint_choice_val != 1)
  {
    /* do not continue unless feature matches explanation constraint */
    if (StringHasNoText (sfp->except_text))
    {
      if (mp->explanation_constraint_choice_val != 2)
      {
        add_citations = FALSE;
      }
    }
    else if (mp->explanation_constraint_choice_val > num_exception_explanations + 2)
    {
      add_citations = FALSE;
    }
    else if (StringICmp (sfp->except_text, 
             exception_explanations [mp->explanation_constraint_choice_val - 3]) != 0)
    {
      add_citations = FALSE;
    }
  }

  if (mp->use_product_match)
  {
    /* do not continue unless translation does not match product */
    if (sfp->data.choice != SEQFEAT_CDREGION)
    {
      add_citations = FALSE;
    }
    else if (sfp->product != NULL)
    {
      protbsp = BioseqFindFromSeqLoc (sfp->product);
      seq1 = GetSequenceByBsp (protbsp);
      bs = ProteinFromCdRegionEx (sfp, TRUE, FALSE);
      if (bs != NULL)
      {
        seq2 = BSMerge (bs, NULL);
      }
      bs = BSFree (bs);
      if (StringICmp (seq1, seq2) == 0)
      {
        add_citations = FALSE;
      }
      seq1 = MemFree (seq1);
      seq2 = MemFree (seq2);
    }
  }

  /* add citations to feature */
  if (add_citations)
  {
    sfp->cit = PubSetFree (sfp->cit);
    sfp->cit = DialogToPointer (mp->citation_list);
  }
  
}

static void 
FeatureEditorDoOneAction 
(SeqEntryPtr     sep, 
 ValNodePtr      feature_type_list, 
 FilterSetPtr    fsp,
 MockupFeatEdPtr mp,
 Int4            action_choice)
{
  Int4       exception_choice;
  ValNodePtr vnp;
  Uint2      feat_def_choice;
  
  for (vnp = feature_type_list; vnp != NULL; vnp = vnp->next)
  {
    feat_def_choice = vnp->choice;
    if (feat_def_choice == 255)
    {
      feat_def_choice = 0;
    }
    switch (action_choice)
    {
      case FEAT_ED_EVIDENCE:
        mp->evidence_val = GetValue (mp->evidence_choice) - 1;
        OperateOnSeqEntryConstrainedObjects (sep, fsp, 
                                             DoEvidenceFeatureProc,
                                             NULL, 0, feat_def_choice, 0, mp);
        break;
      case FEAT_ED_EXCEPTION:
        mp->do_move_to_comment = GetStatus (mp->move_to_comment);
        mp->product_val = GetValue (mp->product_choice);
        exception_choice = GetValue (mp->explanation_choice);
        if (exception_choice == 1)
        {
          mp->explanation_text = NULL;
        }
        else if (exception_choice > 1 && exception_choice < num_exception_explanations + 1)
        {
          mp->explanation_text = StringSave (exception_explanations [exception_choice - 2]);
        }
        else if (exception_choice == num_exception_explanations + 1)
        {
          mp->explanation_text = SaveStringFromText (mp->other_explanation);
        }
        OperateOnSeqEntryConstrainedObjects (sep, fsp, 
                                             DoExceptionFeatureProc,
                                             NULL, 0, feat_def_choice, 0, mp);
        mp->explanation_text = MemFree (mp->explanation_text);
        break;
      case FEAT_ED_PARTIAL:
        mp->orderjoinpolicy = GetValue (mp->convert_choice);
        mp->leftpolicy = GetValue (mp->partial5_choice);
        mp->rightpolicy = GetValue (mp->partial3_choice);
        mp->extend5 = GetStatus (mp->extend5_btn);
        mp->extend3 = GetStatus (mp->extend3_btn);

        OperateOnSeqEntryConstrainedObjects (sep, fsp, 
                                             DoPartialFeatureProc,
                                             NULL, 0, feat_def_choice, 0, mp);
        break;
      case FEAT_ED_STRAND:
        mp->from_strand_val = GetValue (mp->from_strand);
        mp->to_strand_val = GetValue (mp->to_strand);
        OperateOnSeqEntryConstrainedObjects (sep, fsp, 
                                             DoStrandFeatureProc,
                                             NULL, 0, feat_def_choice, 0, mp);
        break;
      case FEAT_ED_CITATION:
        /* do setup here */
        mp->use_explanation_constraint = GetStatus (mp->explanation_constraint);
        mp->explanation_constraint_choice_val = GetValue (mp->explanation_constraint_choice);
        mp->use_product_match = GetStatus (mp->citation_translation);
        OperateOnSeqEntryConstrainedObjects (sep, fsp, 
                                             DoCitationFeatureProc,
                                             NULL, 0, feat_def_choice, 0, mp);
        break;
    }
  }
}

static Boolean FeatureEditorAction (Pointer userdata)
{
  MockupFeatEdPtr mp;
  Int4            action_choice;
  SeqEntryPtr     sep;
  ValNodePtr      feature_type_list;
  FilterSetPtr    fsp;
  Int4            i;

  mp = (MockupFeatEdPtr) userdata;
  if (mp == NULL)
  {
    return FALSE;
  }

  sep = GetTopSeqEntryForEntityID (mp->input_entityID);
  if (sep == NULL)
  {
    return FALSE;
  }

  WatchCursor ();
  Update ();
  
  feature_type_list = (ValNodePtr) DialogToPointer (mp->feature_select);
  
  if (feature_type_list == NULL)
  {
    return FALSE;
  }
  
  fsp = (FilterSetPtr) DialogToPointer (mp->constraints);
  
  if (mp->action_choice_grp != NULL)
  {
    action_choice = GetValue (mp->action_choice_grp);
    FeatureEditorDoOneAction (sep, feature_type_list, fsp, mp, action_choice);
  }
  else
  {
    for (i = FEAT_ED_EVIDENCE; i <= FEAT_ED_STRAND; i++)
    {
      if (GetStatus (mp->action_btns[i - 1]))
      {
        FeatureEditorDoOneAction (sep, feature_type_list, fsp, mp, i);
      }
    }
  }
  ValNodeFreeData (feature_type_list);
  FilterSetFree (fsp);
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (mp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, mp->input_entityID, 0, 0);  
  return TRUE;
}

static void FeatureEditor (IteM i, Int4 first_action)
{
  BaseFormPtr     bfp;
  MockupFeatEdPtr mp;
  WindoW          w;
  GrouP           h, g;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  mp = (MockupFeatEdPtr) MemNew (sizeof (MockupFeatEdData));
  if (mp == NULL) return;
  
  w = FixedWindow (-50, -33, -10, -10, "Mockup Feature Editor", StdCloseWindowProc);
  SetObjectExtra (w, mp, StdCleanupFormProc);
  mp->form = (ForM) w;
  mp->input_entityID = bfp->input_entityID;
  
  h = HiddenGroup (w, -1, 0, NULL);
  
  g = FeatureEditorActionGroup (h, mp, first_action);
  
  mp->constraints = FilterGroup (h, TRUE, FALSE, TRUE, FALSE, "Where feature text");
                      
  mp->accept_cancel = AcceptCancelDialog (h, FeatureEditorAction, NULL, 
                                          MockupFeatEdClear, 
                                          MockupFeatEdClearText, 
                                          (Pointer)mp, w);
  AlignObjects (ALIGN_CENTER, (HANDLE) g,
                              (HANDLE) mp->constraints,
                              (HANDLE) mp->accept_cancel, NULL);
  
  ChangeFeatureEditorActionGroup (mp->action_choice_grp);
  Show (w);
}

extern void FeatureEvidenceEditor (IteM i)
{
  FeatureEditor(i, FEAT_ED_EVIDENCE);
}

extern void FeatureExceptionEditor (IteM i)
{
  FeatureEditor(i, FEAT_ED_EXCEPTION);
}

extern void FeaturePartialEditor (IteM i)
{
  FeatureEditor(i, FEAT_ED_PARTIAL);
}

extern void FeatureStrandEditor (IteM i)
{
  FeatureEditor(i, FEAT_ED_STRAND);
}

extern void FeatureCitationEditor (IteM i)
{
  FeatureEditor(i, FEAT_ED_CITATION);
}

typedef struct cdset 
{
  ValNodePtr cds_list;
  ValNodePtr gene_list;
  ValNodePtr prot_list;
  ValNodePtr mrna_list;
} CDSetData, PNTR CDSetPtr;

static ValNodePtr FreeCDSetList (ValNodePtr vnp)
{
  CDSetPtr cdsp;
  
  if (vnp != NULL)
  {
    FreeCDSetList (vnp->next);
    vnp->next = NULL;
    cdsp = (CDSetPtr) vnp->data.ptrvalue;
    if (cdsp != NULL)
    {
      cdsp->cds_list = ValNodeFree (cdsp->cds_list);
      cdsp->gene_list = ValNodeFree (cdsp->gene_list);
      cdsp->prot_list = ValNodeFree (cdsp->prot_list);
      cdsp->mrna_list = ValNodeFree (cdsp->mrna_list);
    }
    ValNodeFreeData (vnp);
  }
  return NULL;
}

static CharPtr GetCDSGeneProtFieldName (ValNodePtr vnp)
{
  CharPtr PNTR      field_name_list = NULL;
  Int4              num_fields = 0;
  CharPtr           label = NULL;

  field_name_list = BuildCDSGeneFieldList (&num_fields);
  if (field_name_list != NULL
      && vnp->data.intvalue > 0 && vnp->data.intvalue <= num_fields)
  {
    label = StringSave (field_name_list [vnp->data.intvalue - 1]);
  }
  FreeCDSGeneFieldList (field_name_list, num_fields);
  return label;
}

static DialoG GetCDSGeneProtSample (GrouP h, Uint2 entityID)
{
  CharPtr PNTR      field_name_list = NULL;
  Int4              num_fields = 0, i;
  DialoG            d = NULL;
  SetSampleData     ssd;
  
  field_name_list = BuildCDSGeneFieldList (&num_fields);
  if (field_name_list != NULL)
  {    
    ssd.entityID = entityID;
    ssd.field_list = NULL;
    for (i = 0; i < num_fields; i++)
    {
      ValNodeAddInt (&ssd.field_list, 0, i + 1);
    }
    ssd.fieldstring_func = GetCDSGeneProtField;
    ssd.descrstring_func = NULL;
    ssd.fsp = NULL;
    ssd.free_vn_proc = NULL;
    ssd.copy_vn_proc = IntValNodeCopy;
    ssd.match_vn_proc = IntValNodeMatch;
    ssd.label_vn_proc = GetCDSGeneProtFieldName;  
    
    
    d = SampleDialog (h);
    PointerToDialog (d, &ssd);
    FreeCDSGeneFieldList (field_name_list, num_fields);
  }
  return d;
}

static Boolean AlreadyInList (ValNodePtr vnp, SeqFeatPtr sfp)
{
  while (vnp != NULL && vnp->data.ptrvalue != sfp)
  {
    vnp = vnp->next;
  }
  if (vnp == NULL)
  {
    return FALSE;
  }
  else
  {
    return TRUE;
  }
}

static ValNodePtr AlreadyInCDSList (ValNodePtr cds_list, SeqFeatPtr sfp)
{
  ValNodePtr vnp;
  CDSetPtr   cdsp;
  
  if (cds_list == NULL || sfp == NULL)
  {
    return NULL;
  }
  
  vnp = cds_list;
  while (vnp != NULL)
  {
    cdsp = vnp->data.ptrvalue;
    if (cdsp != NULL)
    {
      switch (sfp->data.choice)
      {
        case SEQFEAT_CDREGION:
          if (AlreadyInList (cdsp->cds_list, sfp))
          {
            return vnp;
          }
          break;
        case SEQFEAT_PROT:
          if (AlreadyInList (cdsp->prot_list, sfp))
          {
            return vnp;
          }
          break;
        case SEQFEAT_GENE:
          if (AlreadyInList (cdsp->gene_list, sfp))
          {
            return vnp;
          }
          break;
        case SEQFEAT_RNA:
          if (sfp->idx.subtype == FEATDEF_mRNA)
          {
            if (AlreadyInList (cdsp->mrna_list, sfp))
            {
              return vnp;
            }
          }
          break;
      }
    }
    vnp = vnp->next;
  }
  return NULL;
}

typedef struct buildcdset
{
  ValNodePtr cdset_list;
  ValNodePtr lonely_gene_list;  
} BuildCDSetData, PNTR BuildCDSetPtr;

static void BuildCDSSetCallback (SeqFeatPtr sfp, Pointer userdata)
{
  BuildCDSetPtr        bp;
  CDSetPtr             cdsp = NULL;
  SeqFeatPtr           gene = NULL, mrna = NULL, cds = NULL, prot = NULL;
  ValNodePtr           vnp;
  BioseqPtr            protbsp = NULL;
  SeqMgrFeatContext    fcontext;
  ProtRefPtr           prp;

  if (sfp == NULL || userdata == NULL
      || (sfp->data.choice != SEQFEAT_CDREGION
          && sfp->data.choice != SEQFEAT_GENE
          && sfp->idx.subtype != FEATDEF_mRNA))
  {
    return;
  }
  
  bp = (BuildCDSetPtr) userdata;
  
  vnp = AlreadyInCDSList (bp->cdset_list, sfp);
  if (vnp != NULL)
  {
    return;
  }
  
  if (sfp->data.choice == SEQFEAT_GENE)
  {
    if (!AlreadyInList (bp->lonely_gene_list, sfp))
    {
      ValNodeAddPointer (&(bp->lonely_gene_list), 0, sfp);
    }
  }
  else
  {
    gene = SeqMgrGetOverlappingGene (sfp->location, &fcontext);
    
    if (sfp->idx.subtype == FEATDEF_mRNA)
    {
      mrna = sfp;
      cds = NULL;
      vnp = AlreadyInCDSList (bp->cdset_list, mrna);
      if (vnp != NULL && vnp->data.ptrvalue != NULL)
      {
        cdsp = (CDSetPtr) vnp->data.ptrvalue;
        if (AlreadyInList (cdsp->gene_list, gene))
        {
          gene = NULL;
        }
      }
    }
    else
    {
      cds = sfp;
      mrna = SeqMgrGetOverlappingmRNA (sfp->location, &fcontext);
      vnp = AlreadyInCDSList (bp->cdset_list, mrna);
      if (vnp != NULL && vnp->data.ptrvalue != NULL)
      {
        cdsp = (CDSetPtr) vnp->data.ptrvalue;
        mrna = NULL;
        if (AlreadyInList (cdsp->gene_list, gene))
        {
          gene = NULL;
        }
      }
    }
        
    if (cdsp == NULL)
    {
      cdsp = (CDSetPtr) MemNew (sizeof (CDSetData));
      if (cdsp == NULL)
      {
        return;
      }
      ValNodeAddPointer (&(bp->cdset_list), 0, cdsp);
    }
    if (gene != NULL)
    {
      ValNodeAddPointer (&(cdsp->gene_list), 0, gene);
    }
    if (mrna != NULL)
    {
      ValNodeAddPointer (&(cdsp->mrna_list), 0, mrna);
      if (mrna->product != NULL)
      {
        protbsp = BioseqFindFromSeqLoc (mrna->product);
      }
    }
    if (cds != NULL)
    {
      ValNodeAddPointer (&(cdsp->cds_list), 0, cds);
      if (cds->product != NULL)
      {
        protbsp = BioseqFindFromSeqLoc (cds->product);
      }
    }
    if (protbsp != NULL)
    {
      prot = SeqMgrGetNextFeature (protbsp, NULL, SEQFEAT_PROT, FEATDEF_PROT, &fcontext);
      /* if there is no full-length protein feature, make one */
      if (prot == NULL)
      {
        prp = ProtRefNew ();
        prot = CreateNewFeatureOnBioseq (protbsp, SEQFEAT_PROT, NULL);
        if (prot != NULL)
        {
          prot->data.value.ptrvalue = prp;
  	      SeqMgrIndexFeatures (protbsp->idx.entityID, NULL);
        }
      }
      if (prot != NULL && ! AlreadyInList (cdsp->prot_list, prot))
      {
        ValNodeAddPointer (&(cdsp->prot_list), 0, prot);
      }
    }
  }
}

static CharPtr GetCDSetField (CDSetPtr cdsp, ValNodePtr vnp)
{
  ValNodePtr sfp_vnp;
  CharPtr    str = NULL;
  
  if (cdsp == NULL || vnp == NULL)
  {
    return NULL;
  }
  
  for (sfp_vnp = cdsp->cds_list; sfp_vnp != NULL && str == NULL; sfp_vnp = sfp_vnp->next)
  {
    str = GetCDSGeneProtField (sfp_vnp->data.ptrvalue, vnp, NULL);
  }
  for (sfp_vnp = cdsp->gene_list; sfp_vnp != NULL && str == NULL; sfp_vnp = sfp_vnp->next)
  {
    str = GetCDSGeneProtField (sfp_vnp->data.ptrvalue, vnp, NULL);
  }
  for (sfp_vnp = cdsp->prot_list; sfp_vnp != NULL && str == NULL; sfp_vnp = sfp_vnp->next)
  {
    str = GetCDSGeneProtField (sfp_vnp->data.ptrvalue, vnp, NULL);
  }
  for (sfp_vnp = cdsp->mrna_list; sfp_vnp != NULL && str == NULL; sfp_vnp = sfp_vnp->next)
  {
    str = GetCDSGeneProtField (sfp_vnp->data.ptrvalue, vnp, NULL);
  }
  return str;
}

static void SetCDSetField (CDSetPtr cdsp, ValNodePtr vnp, CharPtr new_text, ExistingTextPtr etp)
{
  ValNodePtr sfp_vnp;
  CharPtr    str = NULL;
  
  if (cdsp == NULL || vnp == NULL)
  {
    return;
  }
  
  for (sfp_vnp = cdsp->cds_list; sfp_vnp != NULL && str == NULL; sfp_vnp = sfp_vnp->next)
  {
    SetCDSGeneProtField (sfp_vnp->data.ptrvalue, vnp, new_text, etp);
  }
  for (sfp_vnp = cdsp->gene_list; sfp_vnp != NULL && str == NULL; sfp_vnp = sfp_vnp->next)
  {
    SetCDSGeneProtField (sfp_vnp->data.ptrvalue, vnp, new_text, etp);
  }
  for (sfp_vnp = cdsp->prot_list; sfp_vnp != NULL && str == NULL; sfp_vnp = sfp_vnp->next)
  {
    SetCDSGeneProtField (sfp_vnp->data.ptrvalue, vnp, new_text, etp);
  }
  for (sfp_vnp = cdsp->mrna_list; sfp_vnp != NULL && str == NULL; sfp_vnp = sfp_vnp->next)
  {
    SetCDSGeneProtField (sfp_vnp->data.ptrvalue, vnp, new_text, etp);
  }
}

static void RemoveCDSetField (CDSetPtr cdsp, ValNodePtr vnp)
{
  ValNodePtr sfp_vnp;
  CharPtr    str = NULL;
  
  if (cdsp == NULL || vnp == NULL)
  {
    return;
  }
  
  for (sfp_vnp = cdsp->cds_list; sfp_vnp != NULL && str == NULL; sfp_vnp = sfp_vnp->next)
  {
    RemoveCDSGeneProtField (sfp_vnp->data.ptrvalue, vnp);
  }
  for (sfp_vnp = cdsp->gene_list; sfp_vnp != NULL && str == NULL; sfp_vnp = sfp_vnp->next)
  {
    RemoveCDSGeneProtField (sfp_vnp->data.ptrvalue, vnp);
  }
  for (sfp_vnp = cdsp->prot_list; sfp_vnp != NULL && str == NULL; sfp_vnp = sfp_vnp->next)
  {
    RemoveCDSGeneProtField (sfp_vnp->data.ptrvalue, vnp);
  }
  for (sfp_vnp = cdsp->mrna_list; sfp_vnp != NULL && str == NULL; sfp_vnp = sfp_vnp->next)
  {
    RemoveCDSGeneProtField (sfp_vnp->data.ptrvalue, vnp);
  }
}

static Boolean DoesCDSetMatchConstraint (CDSetPtr cdsp, ChoiceConstraintPtr ccp)
{
  Boolean                does_match = FALSE;
  CharPtr                str;
  Int4                   field_choice;
  ValNode                field_vn;
  
  if (cdsp == NULL)
  {
    return FALSE;
  }
  if (ccp == NULL || ccp->constraint_type == CHOICE_CONSTRAINT_ANY)
  {
    does_match = TRUE;
  }
  else if (ccp->constraint_type == CHOICE_CONSTRAINT_QUAL_PRESENT)
  {
    if (ccp->qual_choice == NULL)
    {
      does_match = TRUE;
    }
    else
    {
      str = GetCDSetField (cdsp, ccp->qual_choice);
      if (str != NULL)
      {
        MemFree (str);
        does_match = TRUE;
      }
    }
  }
  else if (ccp->constraint_type == CHOICE_CONSTRAINT_STRING)
  {
    if (ccp->qual_choice == NULL) 
    {
      field_vn.next = NULL;
      field_vn.choice = 0;
      for (field_choice = 1;
           ! does_match && field_choice < 1 + num_gene_fields + num_mrna_fields + num_protein_fields;
           field_choice++)
      {
        field_vn.data.intvalue = field_choice;
        str = GetCDSetField (cdsp, &field_vn);
        does_match = DoesStringMatchConstraint (str, ccp->string_constraint);
        MemFree (str);
      }
    }
    else
    {
      str = GetCDSetField (cdsp, ccp->qual_choice);
      does_match = DoesStringMatchConstraint (str, ccp->string_constraint);
      MemFree (str);
    }
  }
  return does_match;
}

static ValNodePtr BuildCDSetList (Uint2 entityID, ChoiceConstraintPtr  ccp)
{
  SeqEntryPtr    sep;
  BuildCDSetData bd;
  CDSetPtr       cdsp;
  ValNodePtr     vnp, vnp_next, vnp_prev;
  
  sep = GetTopSeqEntryForEntityID (entityID);
  bd.cdset_list = NULL;
  bd.lonely_gene_list = NULL;
  
  VisitFeaturesInSep (sep, &bd, BuildCDSSetCallback);
  
  for (vnp = bd.lonely_gene_list; vnp != NULL; vnp = vnp->next)
  {
    /* only create sets for lonely genes if they have not been added to other sets */
    if (AlreadyInCDSList (bd.cdset_list, vnp->data.ptrvalue))
    {
      continue;
    }
    cdsp = (CDSetPtr) MemNew (sizeof (CDSetData));
    if (cdsp != NULL)
    {
      ValNodeAddPointer (&(cdsp->gene_list), 0, vnp->data.ptrvalue);
      ValNodeAddPointer (&bd.cdset_list, 0, cdsp);
    }
  }
  bd.lonely_gene_list = ValNodeFree (bd.lonely_gene_list);

  /* now remove sets that don't match our choice constraint */
  vnp_prev = NULL;
  for (vnp = bd.cdset_list; vnp != NULL; vnp = vnp_next)
  {
    vnp_next = vnp->next;
    if (!DoesCDSetMatchConstraint (vnp->data.ptrvalue, ccp))
    {
      if (vnp_prev == NULL)
      {
        bd.cdset_list = vnp->next;
      }
      else
      {
        vnp_prev->next = vnp->next;
      }
      vnp->next = NULL;
      FreeCDSetList (vnp);     
    }
    else
    {
      vnp_prev = vnp;
    }
  }
  
  return bd.cdset_list;
}

static SeqFeatPtr BuildOneNewCDSGeneProtFeature (CDSetPtr cdsp, Uint2 choice)
{
  BioseqPtr bsp = NULL;
  SeqLocPtr slp = NULL, slp_tmp;
  ValNodePtr vnp;
  SeqFeatPtr sfp = NULL;
  RnaRefPtr  rrp;
  
  if (cdsp == NULL)
  {
    return NULL;
  }

  /* get location from CDS features */
  for (vnp = cdsp->cds_list; vnp != NULL; vnp = vnp->next)
  {
    sfp = vnp->data.ptrvalue;
    if (sfp != NULL)
    {
      if (choice == SEQFEAT_PROT)
      {
        if (bsp == NULL)
        {
          bsp = BioseqFindFromSeqLoc (sfp->product);
        }
      }
      else
      {
        if (bsp == NULL)
        {
          bsp = BioseqFindFromSeqLoc (sfp->location);
        }
        if (bsp != NULL)
        {
          slp_tmp = SeqLocMerge (bsp, sfp->location, slp, TRUE, FALSE, FALSE);
          slp = SeqLocFree (slp);
          slp = slp_tmp;
          slp_tmp = NULL;
        }
      }
    }
  }  
  
  /* get location from mRNA features */
  for (vnp = cdsp->mrna_list; vnp != NULL; vnp = vnp->next)
  {
    sfp = vnp->data.ptrvalue;
    if (sfp != NULL)
    {
      if (choice == SEQFEAT_PROT)
      {
        if (bsp == NULL)
        {
          bsp = BioseqFindFromSeqLoc (sfp->product);
        }
      }
      else
      {
        if (bsp == NULL)
        {
          bsp = BioseqFindFromSeqLoc (sfp->location);
        }
        if (bsp != NULL)
        {
          slp_tmp = SeqLocMerge (bsp, sfp->location, slp, TRUE, FALSE, FALSE);
          slp = SeqLocFree (slp);
          slp = slp_tmp;
          slp_tmp = NULL;
        }
      }
    }
  }

  if (choice == SEQFEAT_PROT)
  {
    if (bsp != NULL)
    {
      slp = SeqLocWholeNew (bsp);
    }
  }
  else
  {
    /* get location from gene features */
    for (vnp = cdsp->gene_list; vnp != NULL; vnp = vnp->next)
    {
      sfp = vnp->data.ptrvalue;
      if (sfp != NULL)
      {
        if (bsp == NULL)
        {
          bsp = BioseqFindFromSeqLoc (sfp->location);
        }
        if (bsp != NULL)
        {
          slp_tmp = SeqLocMerge (bsp, sfp->location, slp, TRUE, FALSE, FALSE);
          slp = SeqLocFree (slp);
          slp = slp_tmp;
          slp_tmp = NULL;
        }
      }
    }
  }
  
  /* for now, cover the entire sequence */
  slp = SeqLocFree (slp);
  slp = SeqLocWholeNew (bsp);
  
  if (bsp != NULL && slp != NULL)
  {
    sfp = CreateNewFeatureOnBioseq (bsp, choice, slp);
    if (choice == SEQFEAT_RNA)
    {
      rrp = RnaRefNew ();
      rrp->type = 2;
      sfp->data.value.ptrvalue = rrp;
    }
  }
  return sfp;
}

static void BuildNewCDSGeneProtFeature (CDSetPtr cdsp, ValNodePtr vnp)
{
  SeqFeatPtr new_sfp;
  
  if (cdsp == NULL || vnp == NULL)
  {
    return;
  }
  if (vnp->data.intvalue == 1)
  {
    /* for now, don't build anything but genes */
#if 0
    if (cdsp->cds_list == NULL)
    {
      /* add new CDS */
      new_sfp = BuildOneNewCDSGeneProtFeature (cdsp, SEQFEAT_CDREGION);
      if (new_sfp != NULL)
      {
        ValNodeAddPointer (&(cdsp->cds_list), 0, new_sfp);
      }
    }
#endif
  }
  else if (vnp->data.intvalue <= 1 + num_gene_fields)
  {
    if (cdsp->gene_list == NULL)
    {
      /* add new gene */
      new_sfp = BuildOneNewCDSGeneProtFeature (cdsp, SEQFEAT_GENE);
      if (new_sfp != NULL)
      {
        ValNodeAddPointer (&(cdsp->gene_list), 0, new_sfp);
      }
    }
  }
  else if (vnp->data.intvalue <= 1 + num_gene_fields + num_mrna_fields)
  {
    /* for now, don't build anything but genes */
#if 0
    if (cdsp->mrna_list == NULL)
    {
      /* add new mrna */
      new_sfp = BuildOneNewCDSGeneProtFeature (cdsp, SEQFEAT_RNA);
      if (new_sfp != NULL)
      {
        ValNodeAddPointer (&(cdsp->mrna_list), 0, new_sfp);
      }
    }
#endif
  }
  else if (vnp->data.intvalue <= 1 + num_gene_fields + num_mrna_fields + num_protein_fields)
  {
    /* for now, don't build anything but genes */
#if 0
    if (cdsp->prot_list == NULL)
    {
      /* add new prot */
      new_sfp = BuildOneNewCDSGeneProtFeature (cdsp, SEQFEAT_PROT);
      if (new_sfp != NULL)
      {
        ValNodeAddPointer (&(cdsp->prot_list), 0, new_sfp);
      }
    }
#endif
  }  
}

static void BuildGeneForSegSet (BioseqSetPtr segset)
{
  SeqEntryPtr       this_sep, master_sep = NULL;
  BioseqSetPtr      part_set = NULL;
  SeqLocPtr         slp = NULL, slp_new, slp_last = NULL;
  SeqFeatPtr        sfp;
  SeqMgrFeatContext context;
  
  if (segset == NULL || segset->seq_set == NULL)
  {
    return;
  }
  
  this_sep = segset->seq_set;
  while (this_sep != NULL && (part_set == NULL || master_sep == NULL))
  {
    if (IS_Bioseq_set (this_sep))
    {
      part_set = (BioseqSetPtr) this_sep->data.ptrvalue;
      if (part_set != NULL && part_set->_class != BioseqseqSet_class_parts)
      {
        part_set = NULL;
      }
    }
    else if (IS_Bioseq (this_sep))
    {
      master_sep = this_sep;
    }
    this_sep = this_sep->next;
  }
  if (part_set != NULL && master_sep != NULL)
  {
    this_sep = part_set->seq_set;
    sfp = SeqMgrGetNextFeature (master_sep->data.ptrvalue, NULL, 
                                SEQFEAT_GENE, FEATDEF_GENE, &context);
    
    while (this_sep != NULL && sfp == NULL)
    {
      if (IS_Bioseq (this_sep))
      {
        sfp = SeqMgrGetNextFeature (this_sep->data.ptrvalue, NULL, 
                                    SEQFEAT_GENE, FEATDEF_GENE, &context);
      }
      this_sep = this_sep->next;
    }
    if (sfp != NULL)
    {
      return;
    }
  
    this_sep = part_set->seq_set;
    while (this_sep != NULL)
    {
      if (IS_Bioseq (this_sep))
      {
        slp_new = SeqLocWholeNew (this_sep->data.ptrvalue);
        if (slp_new != NULL)
        {
          if (slp_last == NULL)
          {
            slp = slp_new;
          }
          else
          {
            slp_last = ValNodeNew (slp);
            slp_last->choice = SEQLOC_NULL;
            slp_last->next = slp_new;
          }
          slp_last = slp_new;
        }
      }
      this_sep = this_sep->next;
    }
    slp_new = ValNodeNew (NULL);
    slp_new->choice = SEQLOC_MIX;
    slp_new->data.ptrvalue = slp;
    sfp = CreateNewFeature (master_sep, NULL, SEQFEAT_GENE, NULL);
    sfp->location = SeqLocFree (sfp->location);
    sfp->location = slp_new;
    sfp->data.value.ptrvalue = GeneRefNew ();
  }
}

static void BuildNewGenes (SeqEntryPtr sep)
{
  BioseqSetPtr      bssp = NULL;
  BioseqPtr         bsp = NULL;
  SeqFeatPtr        sfp;
  SeqMgrFeatContext context;
  SeqLocPtr         slp = NULL;
  
  if (sep == NULL) 
  {
    return;
  }
  
  if (IS_Bioseq (sep))
  {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (ISA_na (bsp->mol))
    {
      sfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_GENE, FEATDEF_GENE, &context);
      if (sfp == NULL)
      {
        slp = SeqLocWholeNew (bsp);
        sfp = CreateNewFeatureOnBioseq (bsp, SEQFEAT_GENE, slp);    
        sfp->data.value.ptrvalue = GeneRefNew();  
      }
    }
  }
  else if (IS_Bioseq_set (sep))
  {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp->_class == BioseqseqSet_class_segset)
    {
      BuildGeneForSegSet (bssp);
    }
    else
    {
      BuildNewGenes (bssp->seq_set);
    }
  }
  BuildNewGenes (sep->next);
}

typedef Boolean (*Nlm_TestAECRChoiceProc) PROTO((Pointer));
typedef DialoG  (*Nlm_AECRMakeDlgProc) PROTO ((GrouP, Int4, Nlm_ChangeNotifyProc, Pointer, Uint2));
typedef DialoG  (*Nlm_SampleDialogProc) PROTO ((GrouP, Uint2));
typedef DialoG  (*Nlm_FieldListDlgProc) PROTO ((GrouP, Boolean, Nlm_ChangeNotifyProc, Pointer));
typedef DialoG  (*Nlm_SubtypeListDlgProc) PROTO ((GrouP, Boolean, Nlm_ChangeNotifyProc, Pointer));

#define AECR_APPLY   1
#define AECR_EDIT    2
#define AECR_CONVERT 3
#define AECR_SWAP    4
#define AECR_REMOVE  5
#define NUM_AECR     5

#define AECR_VIB_MSG_SET_DEFAULT (NUM_VIB_MSG + 1)
#define AECR_VIB_MSG_CLEAR_TEXT  (NUM_VIB_MSG + 2)
#define AECR_VIB_MSG_AUTOPOPULATE (NUM_VIB_MSG + 3)

typedef void (*Nlm_ChangeAECRActionProc) PROTO ((DialoG, DialoG));

typedef struct applyeditconvertremove
{
  FORM_MESSAGE_BLOCK
  DialoG                   aecr_pages [NUM_AECR];
  DialoG                   accept_cancel;
  PopuP                    action_popup;
  DialoG                   constraints;
  ButtoN                   clear_constraints_on_action_change;
  Nlm_ChangeAECRActionProc change_action;
  Int4                     prev_page;
  Boolean                  crippled;
  Int4                     crippled_action;
} ApplyEditConvertRemoveData, PNTR ApplyEditConvertRemovePtr;

static void ApplyEditConvertRemoveChangeNotify (Pointer userdata)
{
  ApplyEditConvertRemovePtr mp;
  Int4                      action_choice;
  ValNodePtr                err_list;
  
  mp = (ApplyEditConvertRemovePtr) userdata;
  if (mp == NULL)
  {
    DisableAcceptCancelDialogAccept (mp->accept_cancel);
    return;
  }
  
  if (mp->crippled)
  {
    action_choice = mp->crippled_action;
  }
  else
  {
    action_choice = GetValue (mp->action_popup);
  }
  
  if (action_choice > 0 && action_choice <= NUM_AECR)
  {
    err_list = TestDialog (mp->aecr_pages [action_choice - 1]);
    if (err_list != NULL)
    {
      err_list = ValNodeFree (err_list);
      DisableAcceptCancelDialogAccept (mp->accept_cancel);
    }
    else
    {
      EnableAcceptCancelDialogAccept (mp->accept_cancel);
    }
  }
  else
  {
    DisableAcceptCancelDialogAccept (mp->accept_cancel);
    return;
  }  
}

static void ApplyEditConvertRemoveTextChange (TexT t)
{
  ApplyEditConvertRemovePtr mp;

  mp = (ApplyEditConvertRemovePtr) GetObjectExtra (t);
  ApplyEditConvertRemoveChangeNotify (mp);
}

static void ApplyEditConvertRemoveClearText (Pointer userdata)
{
  ApplyEditConvertRemovePtr mp;
  FilterSetPtr              fsp;
  Int4                      i;

  mp = (ApplyEditConvertRemovePtr) userdata;
  if (mp != NULL)
  {
    for (i = 0; i < NUM_AECR; i++)
    {
      SendMessageToDialog (mp->aecr_pages[i], AECR_VIB_MSG_CLEAR_TEXT);
    }
    fsp = DialogToPointer (mp->constraints);
    FilterSetClearText (fsp);
    PointerToDialog (mp->constraints, fsp);
    FilterSetFree (fsp);
    ApplyEditConvertRemoveChangeNotify (mp);
  }
}

static void ApplyEditConvertRemoveClear (Pointer userdata)
{
  ApplyEditConvertRemovePtr mp;
  Int4                      i;

  mp = (ApplyEditConvertRemovePtr) userdata;
  if (mp != NULL)
  {  
    for (i = 0; i < NUM_AECR; i++)
    {
      PointerToDialog (mp->aecr_pages[i], NULL);
    }
    PointerToDialog (mp->constraints, NULL);    
    ApplyEditConvertRemoveChangeNotify (mp);
  }
}

static void 
ChangeApplyEditConvertRemoveAction 
(ApplyEditConvertRemovePtr mp,
 Int4                      action_choice)
{
  Int4 i;

  if (mp == NULL || action_choice < AECR_APPLY || action_choice > NUM_AECR)
  {
    return;
  }
    
  for (i = 0; i < NUM_AECR; i++)
  {
    if (i + 1 == action_choice)
    {
      Show (mp->aecr_pages[i]);
    }
    else
    {
      Hide (mp->aecr_pages[i]);
    }
  }
  
  if (GetStatus (mp->clear_constraints_on_action_change))
  {
    PointerToDialog (mp->constraints, NULL);
    PointerToDialog (mp->aecr_pages [action_choice - 1], NULL);
  }
  
  if (mp->change_action != NULL)
  {
    if (mp->prev_page < AECR_APPLY || mp->prev_page > NUM_AECR)
    {
      (mp->change_action) (NULL, mp->aecr_pages [action_choice - 1]);
    }
    else
    {
      (mp->change_action) (mp->aecr_pages [mp->prev_page - 1],
                           mp->aecr_pages [action_choice - 1]);
    }
  }
  
  mp->prev_page = action_choice;
  
  if (action_choice == AECR_APPLY)
  {
    SendMessageToDialog (mp->aecr_pages[AECR_APPLY - 1], AECR_VIB_MSG_AUTOPOPULATE);
  }
  else if (action_choice == AECR_EDIT)
  {
    SendMessageToDialog (mp->aecr_pages[AECR_EDIT - 1], AECR_VIB_MSG_AUTOPOPULATE);
  }

  ApplyEditConvertRemoveChangeNotify (mp);
}

static void ChangeApplyEditConvertRemoveActionPopup (PopuP p)
{
  ApplyEditConvertRemovePtr mp;
  Int4                      action_choice;

  mp = (ApplyEditConvertRemovePtr) GetObjectExtra (p);
  if (mp == NULL)
  {
    return;
  }
  
  action_choice = GetValue (mp->action_popup);
  ChangeApplyEditConvertRemoveAction (mp, action_choice);
}
typedef struct samplebtn
{
  Uint2                entityID;
  Nlm_SampleDialogProc make_sample_dlg;
  CharPtr              title;
} SampleBtnData, PNTR SampleBtnPtr;

static void AECRSampleButton (ButtoN b)
{
  SampleBtnPtr sbp;
  WindoW       w;
  
  sbp = GetObjectExtra (b);
  if (sbp != NULL && sbp->make_sample_dlg != NULL)
  {
    w = FixedWindow (-50, -33, -10, -10, sbp->title, StdCloseWindowProc); 
    (sbp->make_sample_dlg) (w, sbp->entityID);
    Show (w);
  }
}

static void 
ApplyEditConvertRemoveCombo 
(IteM                     i,
 Int4                     first_action_choice,
 Boolean                  crippled,
 CharPtr                  title,
 Nlm_AECRMakeDlgProc      make_dlg,
 Boolean                  show_sample,
 Nlm_SampleDialogProc     make_sample_dlg,
 Boolean                  string_constraint,
 Boolean                  source_constraint,
 Boolean                  location_constraint,
 Boolean                  cds_gene_prot_constraint,
 CharPtr                  string_constraint_label,
 Nlm_AcceptActnProc       accept_actn,
 Nlm_CancelActnProc       cancel_actn,
 Nlm_ChangeAECRActionProc change_action)
{
  BaseFormPtr               bfp;
  ApplyEditConvertRemovePtr mp;
  WindoW                    w;
  GrouP                     h, g1, k, n, m;
  Int4                      page_num;
  SampleBtnPtr              sbp;
  ButtoN                    b;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  mp = (ApplyEditConvertRemovePtr) MemNew (sizeof (ApplyEditConvertRemoveData));
  if (mp == NULL) return;
  
  if (first_action_choice < AECR_APPLY || first_action_choice > AECR_REMOVE)
  {
    first_action_choice = AECR_APPLY;
  }
  mp->crippled = crippled;
  mp->crippled_action = first_action_choice;
  
  w = FixedWindow (-50, -33, -10, -10, title, StdCloseWindowProc); 
  SetObjectExtra (w, mp, StdCleanupExtraProc); 
  SetObjectExtra (w, mp, NULL);
  mp->form = (ForM) w;
  mp->input_entityID = bfp->input_entityID;
  
  n = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (n, 10, 10);
  
  k = HiddenGroup (n, 2, 0, NULL);
  SetGroupSpacing (k, 10, 10);
  if (show_sample && make_sample_dlg != NULL && ! mp->crippled)
  {
    make_sample_dlg (k, mp->input_entityID);
  }
  
  h = HiddenGroup (k, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);  
    
  if (!mp->crippled)
  {
    m = HiddenGroup (h, 2, 0, NULL);
    SetGroupSpacing (m, 10, 10);
  
    if (!show_sample && make_sample_dlg != NULL)
    {
      sbp = (SampleBtnPtr) MemNew (sizeof (SampleBtnData));
      sbp->entityID = mp->input_entityID;
      sbp->title = title;
      sbp->make_sample_dlg = make_sample_dlg;
      b = PushButton (m, "Show Qualifiers", AECRSampleButton);
      SetObjectExtra (b, sbp, StdCleanupExtraProc);
    }
 
    mp->action_popup = PopupList (m, TRUE, ChangeApplyEditConvertRemoveActionPopup);
    SetObjectExtra (mp->action_popup, mp, NULL);
    PopupItem (mp->action_popup, "Apply");
    PopupItem (mp->action_popup, "Edit");
    PopupItem (mp->action_popup, "Convert");
    PopupItem (mp->action_popup, "Swap");
    PopupItem (mp->action_popup, "Remove");
    SetValue (mp->action_popup, first_action_choice);
  
    mp->change_action = change_action;
  
    mp->clear_constraints_on_action_change = CheckBox (h, "Clear when changing actions", NULL);
    SetStatus (mp->clear_constraints_on_action_change, TRUE);
  }
  
  g1 = HiddenGroup (h, 0, 0, NULL);
  for (page_num = 0; page_num < NUM_AECR; page_num++)
  {
    mp->aecr_pages [page_num] = make_dlg (g1, page_num + 1, 
                                          ApplyEditConvertRemoveChangeNotify,
                                          mp, mp->input_entityID);
  }
  AlignObjects (ALIGN_CENTER, (HANDLE) mp->aecr_pages[0],
                              (HANDLE) mp->aecr_pages[1],
                              (HANDLE) mp->aecr_pages[2],
                              (HANDLE) mp->aecr_pages[3],
                              (HANDLE) mp->aecr_pages[4],
                               NULL);

  if (! mp->crippled)
  {
    AlignObjects (ALIGN_CENTER, (HANDLE) m,
                                (HANDLE) mp->clear_constraints_on_action_change,
                                (HANDLE) g1,
                                (HANDLE) NULL);
    mp->constraints = FilterGroup (n, string_constraint, source_constraint, 
                                   location_constraint, cds_gene_prot_constraint,
                                   string_constraint_label);
  
  }
  
  mp->accept_cancel = AcceptCancelDialog (n, accept_actn, cancel_actn, 
                                          ApplyEditConvertRemoveClear, 
                                          ApplyEditConvertRemoveClearText, 
                                          (Pointer)mp, w);
                                          
  if (mp->crippled)
  {
    AlignObjects (ALIGN_CENTER, (HANDLE) k, (HANDLE) mp->accept_cancel, NULL);
  }
  else
  {
    AlignObjects (ALIGN_CENTER, (HANDLE) k,
                                (HANDLE) mp->constraints,
                                (HANDLE) mp->accept_cancel,
                                NULL);
  }
                              
  mp->prev_page = -1;                              
  ChangeApplyEditConvertRemoveAction (mp, first_action_choice);
  Show (w);   
}

typedef struct editapply
{
  CharPtr    find_txt;
  CharPtr    repl_txt;
  CharPtr    apply_txt;  
} EditApplyData, PNTR EditApplyPtr;

static EditApplyPtr EditApplyFree (EditApplyPtr eap)
{
  if (eap != NULL)
  {
    eap->find_txt = MemFree (eap->find_txt);
    eap->repl_txt = MemFree (eap->repl_txt);
    eap->apply_txt = MemFree (eap->apply_txt);
    eap = MemFree (eap);
  }
  return eap;
}

typedef struct EditApplydlg
{
  DIALOG_MESSAGE_BLOCK
  TexT           find_txt;
  TexT           repl_txt;
  TexT           apply_txt;
  Int4           action_choice;
} EditApplyDlgData, PNTR EditApplyDlgPtr;

static void ResetEditApplyDlg (EditApplyDlgPtr dlg)
{
  if (dlg != NULL)
  {
    if (dlg->find_txt != NULL)
    {
      SetTitle (dlg->find_txt, "");
    }
    if (dlg->repl_txt != NULL)
    {
      SetTitle (dlg->repl_txt, "");
    }
    if (dlg->apply_txt != NULL)
    {
      SetTitle (dlg->apply_txt, "");
    }
  }
}

static void EditApplyToDialog (DialoG d, Pointer userdata)
{
  EditApplyDlgPtr dlg;
  EditApplyPtr    data;
  
  dlg = (EditApplyDlgPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return;
  }
  
  ResetEditApplyDlg (dlg);
  data = (EditApplyPtr) userdata;
  if (data != NULL)
  {
    if (dlg->find_txt != NULL && !StringHasNoText (data->find_txt))
    {
      SetTitle (dlg->find_txt, data->find_txt);
    }
    if (dlg->repl_txt != NULL && !StringHasNoText (data->repl_txt))
    {
      SetTitle (dlg->repl_txt, data->repl_txt);
    }
    if (dlg->apply_txt != NULL && !StringHasNoText (data->apply_txt))
    {
      SetTitle (dlg->apply_txt, data->apply_txt);
    }
  }
}

static Pointer DialogToEditApply (DialoG d)
{
  EditApplyDlgPtr dlg;
  EditApplyPtr    data;
  
  dlg = (EditApplyDlgPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return NULL;
  }
  
  data = (EditApplyPtr) MemNew (sizeof (EditApplyData));
  if (data != NULL)
  {
    if (dlg->find_txt != NULL)
    {
      data->find_txt = JustSaveStringFromText (dlg->find_txt);
    }
    if (dlg->repl_txt != NULL)
    {
      data->repl_txt = JustSaveStringFromText (dlg->repl_txt);
    }
    if (dlg->apply_txt != NULL)
    {
      data->apply_txt = JustSaveStringFromText (dlg->apply_txt);
    }
  }
  return data;
}

static void EditApplyMessage (DialoG d, Int2 mssg)

{
  EditApplyDlgPtr  dlg;

  dlg = (EditApplyDlgPtr) GetObjectExtra (d);
  if (dlg != NULL) {
    switch (mssg) 
    {
      case VIB_MSG_INIT :
        /* reset list */
        ResetEditApplyDlg (dlg);
        break;
      case VIB_MSG_ENTER :
        if (dlg->find_txt != NULL)
        {
          Select (dlg->find_txt);
        }
        else if (dlg->apply_txt != NULL)
        {
          Select (dlg->apply_txt);
        }
        break;
      case AECR_VIB_MSG_SET_DEFAULT :
        ResetEditApplyDlg (dlg);
        break;
      case AECR_VIB_MSG_CLEAR_TEXT :
        ResetEditApplyDlg (dlg);
        break;
      default :
        break;
    }
  }
}

static ValNodePtr TestEditApply (DialoG d)
{
  EditApplyDlgPtr dlg;
  ValNodePtr      total_err_list = NULL;
  
  dlg = (EditApplyDlgPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return FALSE;
  }

  if (dlg->action_choice == AECR_APPLY)
  {
    if (TextHasNoText (dlg->apply_txt))
    {
      ValNodeAddPointer (&total_err_list, 0, "apply text");
    }
  }
  else if (dlg->action_choice == AECR_EDIT)
  {
    if (TextHasNoText (dlg->find_txt))
    {
      ValNodeAddPointer (&total_err_list, 0, "find text");
    }
  }
  return total_err_list;
}

static void EditApplyDialogCopy (ButtoN b)
{
  EditApplyDlgPtr dlg;
  CharPtr         str = NULL;

  dlg = (EditApplyDlgPtr) GetObjectExtra (b);
  if (dlg == NULL)
  {
    return;
  }
  str = SaveStringFromText (dlg->find_txt);
  SetTitle (dlg->repl_txt, str);
  str = MemFree (str);
}

static DialoG EditApplyDialog 
(GrouP                    h,
 Int4                     action_choice, 
 CharPtr                  apply_label,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata)
{
  EditApplyDlgPtr dlg;
  GrouP           p;
  ButtoN          b;
  
  dlg = (EditApplyDlgPtr) MemNew (sizeof (EditApplyDlgData));
  if (dlg == NULL)
  {
    return NULL;
  }

  p = HiddenGroup (h, 3, 0, NULL);
  SetGroupSpacing (p, 10, 10);
  SetObjectExtra (p, dlg, StdCleanupExtraProc);
  
  dlg->dialog = (DialoG) p;
  dlg->todialog = EditApplyToDialog;
  dlg->fromdialog = DialogToEditApply;
  dlg->dialogmessage = EditApplyMessage;
  dlg->testdialog = TestEditApply;
  dlg->action_choice = action_choice;
  
  if (action_choice == AECR_APPLY)
  {
    StaticPrompt (p, apply_label, 0, dialogTextHeight, systemFont, 'r');
    dlg->apply_txt = DialogText (p, "", 20, ApplyEditConvertRemoveTextChange);
    SetObjectExtra (dlg->apply_txt, change_userdata, NULL);
  }
  else if (action_choice == AECR_EDIT)
  {
    StaticPrompt (p, "Find", 0, dialogTextHeight, systemFont, 'r');
    dlg->find_txt = DialogText (p, "", 18, ApplyEditConvertRemoveTextChange);
    SetObjectExtra (dlg->find_txt, change_userdata, NULL);
    b = PushButton (p, "Copy", EditApplyDialogCopy);
    SetObjectExtra (b, dlg, NULL);
    Hide (b);
    StaticPrompt (p, "Replace", 0, dialogTextHeight, systemFont, 'r');
    dlg->repl_txt = DialogText (p, "", 18, ApplyEditConvertRemoveTextChange);
    SetObjectExtra (dlg->repl_txt, change_userdata, NULL);
    b = PushButton (p, "Copy", EditApplyDialogCopy);
    SetObjectExtra (b, dlg, NULL);
  }
  return (DialoG) p;
}

typedef struct conversionconflict 
{
  ConvertFieldPtr cfp;
  GetSamplePtr    gsp;
} ConversionConflictData, PNTR ConversionConflictPtr;

static void GetConversionConflictsFeatureCallback (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  ConversionConflictPtr ccp;
  CharPtr               src_str, dst_str;
  
  if (sfp == NULL || userdata == NULL)
  {
    return;
  }
  
  ccp = (ConversionConflictPtr) userdata;
  
  if (ccp->cfp == NULL
      || ccp->cfp->src_field_list == NULL 
      || ccp->cfp->dst_field_list == NULL
      || ccp->cfp->get_str_func == NULL
      || ccp->gsp == NULL)
  {
    return;
  }
  
  src_str = (ccp->cfp->get_str_func) (sfp, ccp->cfp->src_field_list, ccp->cfp->fsp);
  if (!StringHasNoText (src_str))
  {
    dst_str = (ccp->cfp->get_str_func) (sfp, ccp->cfp->dst_field_list, ccp->cfp->fsp);
    if (!StringHasNoText (dst_str))
    {
      ccp->gsp->num_found ++;
      if (ccp->gsp->sample_text == NULL)
      {
        ccp->gsp->sample_text = dst_str;
      }
      else
      {
        if (StringCmp (dst_str, ccp->gsp->sample_text) != 0)
        {
          ccp->gsp->all_same = FALSE;
        }
        dst_str = MemFree (dst_str);
      }
    }
    src_str = MemFree (src_str);   
  }
}

static void GetConversionConflictsDescriptorCallback (SeqDescPtr sdp, Pointer userdata, FilterSetPtr fsp)
{
  ConversionConflictPtr ccp;
  CharPtr               src_str, dst_str;
  
  if (sdp == NULL || userdata == NULL)
  {
    return;
  }
  
  ccp = (ConversionConflictPtr) userdata;
  
  if (ccp->cfp == NULL
      || ccp->cfp->src_field_list == NULL 
      || ccp->cfp->dst_field_list == NULL
      || ccp->cfp->get_d_str_func == NULL
      || ccp->gsp == NULL)
  {
    return;
  }
  
  src_str = (ccp->cfp->get_d_str_func) (sdp, ccp->cfp->src_field_list, ccp->cfp->fsp);
  if (!StringHasNoText (src_str))
  {
    dst_str = (ccp->cfp->get_d_str_func) (sdp, ccp->cfp->dst_field_list, ccp->cfp->fsp);
    if (!StringHasNoText (dst_str))
    {
      ccp->gsp->num_found ++;
      if (ccp->gsp->sample_text == NULL)
      {
        ccp->gsp->sample_text = dst_str;
      }
      else
      {
        if (StringCmp (dst_str, ccp->gsp->sample_text) != 0)
        {
          ccp->gsp->all_same = FALSE;
        }
        dst_str = MemFree (dst_str);
      }
    }
    src_str = MemFree (src_str);   
  }
}

static GetSamplePtr CheckForConversionExistingTextInSeqEntry
(SeqEntryPtr              sep,
 ConvertFieldPtr          cfp,
 FilterSetPtr             fsp,
 Uint1                    seqfeat_choice,
 Uint1                    featdef_choice,
 Uint1                    descr_choice)
{
  ConversionConflictData ccd;
  
  ccd.cfp = cfp;
  ccd.gsp = (GetSamplePtr) MemNew (sizeof (GetSampleData));
  if (ccd.gsp != NULL)
  {
    ccd.gsp->sample_text = NULL;
    ccd.gsp->fieldstring_func = NULL;
    ccd.gsp->descrstring_func = NULL;
    ccd.gsp->free_vn_proc = NULL;
    ccd.gsp->copy_vn_proc = NULL;
    ccd.gsp->requested_field = NULL;
    ccd.gsp->num_found = 0;
    ccd.gsp->all_same = TRUE;
    OperateOnSeqEntryConstrainedObjects (sep, fsp, GetConversionConflictsFeatureCallback, 
                                         GetConversionConflictsDescriptorCallback,
                                         seqfeat_choice, featdef_choice, descr_choice, &ccd);
  }
  return ccd.gsp;  
}

static void 
AddEditApplyDataToApplyValue 
(Int4 action_choice,
 EditApplyPtr edit_apply, 
 ApplyValuePtr avp)
{
  if (avp == NULL)
  {
    return;
  }
  
  if (edit_apply == NULL)
  {
    avp->text_to_replace = NULL;
    avp->new_text = NULL;
    return;
  }

  if (action_choice == AECR_EDIT)
  {
    avp->text_to_replace = StringSave (edit_apply->find_txt);
    avp->new_text = StringSave (edit_apply->repl_txt);
  }
  else if (action_choice == AECR_APPLY)
  {
    avp->new_text = StringSave (edit_apply->apply_txt);
    avp->text_to_replace = NULL; 
  }
  else
  {
    avp->text_to_replace = NULL;
    avp->new_text = NULL;
  } 
}

typedef struct simpleaecr
{
  ValNodePtr      field_list;
  ValNodePtr      field_list_to;
  ValNodePtr      subtype_list;
  Boolean         leave_on_original;
  EditApplyPtr    edit_apply;
  FreeValNodeProc free_field_vn_proc;
  FreeValNodeProc free_subtype_vn_proc;
} SimpleAECRData, PNTR SimpleAECRPtr;

static SimpleAECRPtr SimpleAECRFree (SimpleAECRPtr sp)
{
  if (sp != NULL)
  {
    sp->field_list = ValNodeFuncFree (sp->field_list, sp->free_field_vn_proc);
    sp->field_list_to = ValNodeFuncFree (sp->field_list_to, sp->free_field_vn_proc);
    sp->subtype_list = ValNodeFuncFree (sp->subtype_list, sp->free_subtype_vn_proc);
    sp->edit_apply = EditApplyFree (sp->edit_apply);
    sp = MemFree (sp);
  }
  return sp;
}

/* This function is used to autopopulate the new_text and find fields */
typedef CharPtr (*GetAutoPopulateTextFunc) PROTO ((ValNodePtr, ValNodePtr, Uint2));

typedef struct simpleecrdlg
{
  DIALOG_MESSAGE_BLOCK
  DialoG          field_list;
  DialoG          field_list_to;
  DialoG          subtype_list;
  ButtoN          leave_on_original;  
  DialoG          edit_apply; 
  Int4            action_choice;
  /* These are necessary to produce the SimpleAECR structure for output */
  FreeValNodeProc free_field_vn_proc;
  FreeValNodeProc free_subtype_vn_proc;
  
  /* These are used for autopopulating text fields */
  GetAutoPopulateTextFunc  get_autopopulate_text;
  Nlm_ChangeNotifyProc     change_notify;
  Pointer                  change_userdata;
  Uint2                    entityID;
} SimpleAECRDlgData, PNTR SimpleAECRDlgPtr;

static void ClearTextSimpleAECRDlg (SimpleAECRDlgPtr dlg)
{
  if (dlg != NULL)
  {
    PointerToDialog (dlg->edit_apply, NULL);
  }
}

static void ResetSimpleAECRDlg (SimpleAECRDlgPtr dlg)
{
  if (dlg != NULL)
  {
    PointerToDialog (dlg->field_list, NULL);
    PointerToDialog (dlg->field_list_to, NULL);
    PointerToDialog (dlg->subtype_list, NULL);
    SafeSetStatus (dlg->leave_on_original, FALSE);
    ClearTextSimpleAECRDlg (dlg);
  }
}

static void SimpleAECRToDialog (DialoG d, Pointer userdata)
{
  SimpleAECRDlgPtr dlg;
  SimpleAECRPtr    data;
  
  dlg = (SimpleAECRDlgPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return;
  }
  
  ResetSimpleAECRDlg (dlg);
  data = (SimpleAECRPtr) userdata;
  if (data != NULL)
  {
    PointerToDialog (dlg->field_list, data->field_list);
    PointerToDialog (dlg->field_list_to, data->field_list_to);
    PointerToDialog (dlg->subtype_list, data->subtype_list);
    PointerToDialog (dlg->edit_apply, data->edit_apply);
    SafeSetValue (dlg->leave_on_original, data->leave_on_original);
    dlg->free_field_vn_proc = data->free_field_vn_proc;
    dlg->free_subtype_vn_proc = data->free_subtype_vn_proc;
  }
}

static Pointer DialogToSimpleAECR (DialoG d)
{
  SimpleAECRDlgPtr dlg;
  SimpleAECRPtr    data;
  
  dlg = (SimpleAECRDlgPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return NULL;
  }
  
  data = (SimpleAECRPtr) MemNew (sizeof (SimpleAECRData));
  if (data != NULL)
  {
    data->field_list = DialogToPointer (dlg->field_list);
    data->field_list_to = DialogToPointer (dlg->field_list_to);
    data->subtype_list = DialogToPointer (dlg->subtype_list);
    data->edit_apply = DialogToPointer (dlg->edit_apply);
    if (dlg->leave_on_original != NULL)
    {
      data->leave_on_original = GetStatus (dlg->leave_on_original);
    }
    else
    {
      data->leave_on_original = FALSE;
    }
    data->free_field_vn_proc = dlg->free_field_vn_proc;
    data->free_subtype_vn_proc = dlg->free_subtype_vn_proc;
  }
  return data;
}

static void SimpleAECRDialogChangeNotify (Pointer userdata)
{
  SimpleAECRDlgPtr dlg;
  CharPtr          autopopulate_text;
  EditApplyPtr     eap;
  ValNodePtr       subtype_list, field_list;

  dlg = (SimpleAECRDlgPtr) userdata;
  if (dlg == NULL)
  { 
    return;
  }
  
  if (dlg->get_autopopulate_text != NULL
      && (dlg->action_choice == AECR_APPLY || dlg->action_choice == AECR_EDIT))
  {
    eap = DialogToPointer (dlg->edit_apply);
    subtype_list = DialogToPointer (dlg->subtype_list);
    field_list = DialogToPointer (dlg->field_list);
    autopopulate_text = (dlg->get_autopopulate_text) (subtype_list, field_list, dlg->entityID);
    subtype_list = ValNodeFuncFree (subtype_list, dlg->free_subtype_vn_proc);
    field_list = ValNodeFuncFree (field_list, dlg->free_field_vn_proc);
    if (dlg->action_choice == AECR_APPLY)
    {
      eap->apply_txt = MemFree (eap->find_txt);
      eap->apply_txt = autopopulate_text;
    }
    else
    {
      eap->find_txt = MemFree (eap->find_txt);
      eap->find_txt = autopopulate_text;
    }
    PointerToDialog (dlg->edit_apply, eap);
    EditApplyFree (eap);
  }
  
  if (dlg->change_notify != NULL)
  {
    (dlg->change_notify) (dlg->change_userdata);
  }
}


static void SimpleAECRMessage (DialoG d, Int2 mssg)

{
  SimpleAECRDlgPtr  dlg;

  dlg = (SimpleAECRDlgPtr) GetObjectExtra (d);
  if (dlg != NULL) {
    switch (mssg) 
    {
      case VIB_MSG_INIT :
        /* reset list */
        ResetSimpleAECRDlg (dlg);
        break;
      case VIB_MSG_ENTER :
        if (dlg->subtype_list != NULL)
        {
          Select (dlg->subtype_list);
        }
        else
        {
          Select (dlg->field_list);
        }
        break;
      case AECR_VIB_MSG_SET_DEFAULT :
        SendMessageToDialog (dlg->field_list, AECR_VIB_MSG_SET_DEFAULT);
        SendMessageToDialog (dlg->field_list_to, AECR_VIB_MSG_SET_DEFAULT);
        SendMessageToDialog (dlg->subtype_list, AECR_VIB_MSG_SET_DEFAULT);
        ClearTextSimpleAECRDlg (dlg);
        break;
      case AECR_VIB_MSG_AUTOPOPULATE:
        SimpleAECRDialogChangeNotify (dlg);
        break;
      case AECR_VIB_MSG_CLEAR_TEXT :
        ClearTextSimpleAECRDlg (dlg);
        break;
      default :
        break;
    }
  }
}

static ValNodePtr TestSimpleAECR (DialoG d)
{
  SimpleAECRDlgPtr dlg;
  ValNodePtr       err_list = NULL;
  ValNodePtr       total_err_list = NULL;
  
  dlg = (SimpleAECRDlgPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return FALSE;
  }

  total_err_list = TestDialog (dlg->field_list);
  
  if (dlg->subtype_list != NULL)
  {
    err_list = TestDialog (dlg->subtype_list);
    total_err_list = ValNodeAppend (total_err_list, err_list);
  }
    
  err_list = NULL;  
  switch (dlg->action_choice)
  {
    case AECR_CONVERT:
    case AECR_SWAP:
      err_list = TestDialog (dlg->field_list_to);
      break;
    case AECR_APPLY:
    case AECR_EDIT:
      err_list = TestDialog (dlg->edit_apply);
      break;
  }

  total_err_list = ValNodeAppend (total_err_list, err_list);
    
  return total_err_list;
}

static DialoG SimpleAECRDialog
(GrouP                    h,
 Int4                     action_choice, 
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata,
 FreeValNodeProc          free_field_vn_proc,
 FreeValNodeProc          free_subtype_vn_proc,
 Nlm_FieldListDlgProc     fieldlist_dlg_proc,
 Nlm_SubtypeListDlgProc   subtypelist_dlg_proc,
 GetAutoPopulateTextFunc  get_autopopulate_text,
 CharPtr                  field_label,
 CharPtr                  subtype_label,
 Boolean                  is_text,
 Uint2                    entityID)
{
  SimpleAECRDlgPtr dlg;
  GrouP            p, g1;
  
  dlg = (SimpleAECRDlgPtr) MemNew (sizeof (SimpleAECRDlgData));
  if (dlg == NULL)
  {
    return NULL;
  }

  p = HiddenGroup (h, -1, 0, NULL);
  SetObjectExtra (p, dlg, StdCleanupExtraProc);
  SetGroupSpacing (p, 10, 10);
  
  dlg->dialog = (DialoG) p;
  dlg->todialog = SimpleAECRToDialog;
  dlg->fromdialog = DialogToSimpleAECR;
  dlg->dialogmessage = SimpleAECRMessage;
  dlg->testdialog = TestSimpleAECR;
  dlg->action_choice = action_choice;
  dlg->free_field_vn_proc = free_field_vn_proc;
  dlg->free_subtype_vn_proc = free_subtype_vn_proc;
  dlg->change_notify = change_notify;
  dlg->change_userdata = change_userdata;
  dlg->get_autopopulate_text = get_autopopulate_text;
  dlg->entityID = entityID;
  
  dlg->subtype_list = NULL;
  
  if (action_choice == AECR_CONVERT || action_choice == AECR_SWAP)
  {
    if (subtypelist_dlg_proc == NULL)
    {
      g1 = HiddenGroup (p, 2, 0, NULL);
    }
    else
    {
      g1 = HiddenGroup (p, 3, 0, NULL);
      StaticPrompt (g1, subtype_label, 0, dialogTextHeight, systemFont, 'l');
    }
    StaticPrompt (g1, "From", 0, dialogTextHeight, systemFont, 'l');
    StaticPrompt (g1, "To", 0, dialogTextHeight, systemFont, 'l');
    
    if (subtypelist_dlg_proc != NULL)
    {
      dlg->subtype_list = (subtypelist_dlg_proc) (g1, TRUE, change_notify, change_userdata);
    }
    dlg->field_list = fieldlist_dlg_proc (g1, FALSE, 
                                          change_notify, 
                                          change_userdata);
    dlg->field_list_to = fieldlist_dlg_proc (g1, FALSE, 
                                             change_notify, 
                                             change_userdata);
    if (action_choice == AECR_CONVERT && is_text)
    {
      dlg->leave_on_original = CheckBox (p, "Leave on original", NULL);
    }
  }
  else
  {
    if (subtypelist_dlg_proc == NULL)
    {
      g1 = HiddenGroup (p, 1, 0, NULL);
      
    }
    else
    {
      g1 = HiddenGroup (p, 2, 0, NULL);
      StaticPrompt (g1, subtype_label, 0, dialogTextHeight, systemFont, 'l');
    }
    StaticPrompt (g1, field_label, 0, dialogTextHeight, systemFont, 'l');
    if (action_choice == AECR_REMOVE)
    {
      if (subtypelist_dlg_proc != NULL)
      {
        dlg->subtype_list = (subtypelist_dlg_proc) (g1, TRUE, change_notify, change_userdata);
      }
      dlg->field_list = fieldlist_dlg_proc (g1, TRUE, 
                                            change_notify, 
                                            change_userdata);
    }
    else
    {
      if (subtypelist_dlg_proc != NULL)
      {
        dlg->subtype_list = (subtypelist_dlg_proc) (g1, TRUE, 
                                                    SimpleAECRDialogChangeNotify,
                                                    dlg);
      }
      dlg->field_list = fieldlist_dlg_proc (g1, FALSE, 
                                            SimpleAECRDialogChangeNotify, 
                                            dlg);
    }
  }
  
  if (is_text && (action_choice == AECR_APPLY || action_choice == AECR_EDIT))
  {
    dlg->edit_apply = EditApplyDialog (p, action_choice, "New Text",
                                       change_notify, change_userdata);
  }
  else
  {
    dlg->edit_apply = NULL;
  }

  if (action_choice == AECR_CONVERT)
  {
    AlignObjects (ALIGN_CENTER, (HANDLE) g1, (HANDLE) dlg->leave_on_original, NULL);
  }
  else
  {
    AlignObjects (ALIGN_CENTER, (HANDLE) g1, (HANDLE) dlg->edit_apply, NULL);
  }
  return (DialoG) p;
}

static CharPtr 
CDSGeneProtAutopopulate 
(ValNodePtr subtype_list,
 ValNodePtr field_list, 
 Uint2 entityID)
{
  GetSamplePtr gsp;
  CharPtr      rval;
  
  gsp = CheckForExistingText (entityID, field_list, 
                              GetCDSGeneProtField,
                              NULL,
                              NULL,
                              IntValNodeCopy,
                              NULL,
                              0, 0, 0);
  rval = gsp->sample_text;
  gsp->sample_text = NULL;
  GetSampleFree (gsp);
  return rval;
}

static DialoG CDSGeneProtAECRDialog
(GrouP                    h,
 Int4                     action_choice, 
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata,
 Uint2                    entityID)
{
  return SimpleAECRDialog (h, action_choice, change_notify, change_userdata,
                           NULL, NULL, 
                           CDSGeneProtFieldSelectionDialog, NULL,
                           CDSGeneProtAutopopulate,
                           "Field", NULL, TRUE, entityID);
}

static GetSamplePtr 
GetCDSGeneProtAECRSample 
(ValNodePtr cdset_list,
 ValNodePtr field_list, 
 ValNodePtr field_list_to)
{
  GetSamplePtr gsp;
  ValNodePtr   cdset_vnp;
  CharPtr      str;
  
  gsp = (GetSamplePtr) MemNew (sizeof (GetSampleData));
  if (gsp == NULL)
  {
    return NULL;
  }
  gsp->free_vn_proc = NULL;
  gsp->copy_vn_proc = IntValNodeCopy;
  for (cdset_vnp = cdset_list; cdset_vnp != NULL; cdset_vnp = cdset_vnp->next)
  {
    str = GetCDSetField (cdset_vnp->data.ptrvalue, field_list);
    if (str != NULL && field_list_to != NULL)
    {
      str = MemFree (str);
      str = GetCDSetField (cdset_vnp->data.ptrvalue, field_list_to);
    }
    if (str != NULL)
    {
      gsp->num_found ++;
    }
    if (gsp->sample_text == NULL)
    {
      gsp->sample_text = str;
    }
    else
    {
      if (StringCmp (str, gsp->sample_text) != 0)
      {
        gsp->all_same = FALSE;
      }
      str = MemFree (str);
    }
  }
  return gsp;
}

static Boolean CDSGeneProtAECRAction (Pointer userdata)
{
  ApplyEditConvertRemovePtr ap;
  SimpleAECRPtr             sp;
  Int4                      action_choice;
  SeqEntryPtr               sep;
  ApplyValueData            avd;
  FilterSetPtr              fsp;
  GetSamplePtr              gsp = NULL;
  Boolean                   rval = TRUE;
  ValNodePtr                cdset_list, cdset_vnp;
  ExistingTextPtr           etp = NULL;
  CharPtr                   str_src, str_dst;

  ap = (ApplyEditConvertRemovePtr) userdata;
  if (ap == NULL)
  {
    return FALSE;
  }
  
  WatchCursor ();
  Update ();
  
  if (ap->crippled)
  {
    action_choice = ap->crippled_action;
  }
  else
  {
    action_choice = GetValue (ap->action_popup);
  }
  if (action_choice < 1 || action_choice > NUM_AECR)
  {
    return FALSE;
  }
  sp = (SimpleAECRPtr) DialogToPointer (ap->aecr_pages[action_choice - 1]);

  sep = GetTopSeqEntryForEntityID (ap->input_entityID);
  if (sp->field_list == NULL
      || ((action_choice == AECR_CONVERT || action_choice == AECR_SWAP) 
           && sp->field_list == NULL))
  {
    sp = SimpleAECRFree (sp);
    ArrowCursor ();
    Update ();
    return FALSE;
  }

  fsp = (FilterSetPtr) DialogToPointer (ap->constraints);
  
  /* need to create a list of sets */
  if (fsp == NULL)
  {
    cdset_list = BuildCDSetList (ap->input_entityID, NULL);
  }
  else
  {
    cdset_list = BuildCDSetList (ap->input_entityID, fsp->cgp);
  }
  
  /* find existing text and how to handle it */
  if (action_choice == AECR_CONVERT || action_choice == AECR_APPLY)
  {
    gsp = GetCDSGeneProtAECRSample (cdset_list, sp->field_list, sp->field_list_to);
    etp = GetExistingTextHandlerInfo (gsp, FALSE);
    gsp = GetSampleFree (gsp);
    if (etp != NULL && etp->existing_text_choice == EXISTING_TEXT_CHOICE_CANCEL)
    {
      rval = FALSE;
    }
  }
  
  
  if (rval)
  {
    /* if we are applying gene fields, we need to make sure every sequence has a gene
     * and we need to recollect the cdset_list
     */
    if (action_choice == AECR_APPLY
        && sp->field_list != NULL
        && sp->field_list->data.intvalue > 1 
        && sp->field_list->data.intvalue <= 1 + num_gene_fields)
    {
      BuildNewGenes (sep);
      FreeCDSetList (cdset_list);
      if (fsp == NULL)
      {
        cdset_list = BuildCDSetList (ap->input_entityID, NULL); 
      }
      else
      {
        cdset_list = BuildCDSetList (ap->input_entityID, fsp->cgp); 
      }
    }
    
  
    for (cdset_vnp = cdset_list; cdset_vnp != NULL; cdset_vnp = cdset_vnp->next)
    {
      if (action_choice == AECR_CONVERT)
      {
        str_src = GetCDSetField (cdset_vnp->data.ptrvalue, sp->field_list);
        if (str_src != NULL)
        {
          BuildNewCDSGeneProtFeature (cdset_vnp->data.ptrvalue, sp->field_list_to);
          SetCDSetField (cdset_vnp->data.ptrvalue, sp->field_list_to, str_src, etp);
          if (!sp->leave_on_original)
          {
            sp->field_list->choice = 1;
            RemoveCDSetField (cdset_vnp->data.ptrvalue, sp->field_list);
          }
        }   
      }
      else if (action_choice == AECR_SWAP)
      {
        str_src = GetCDSetField (cdset_vnp->data.ptrvalue, sp->field_list);
        str_dst = GetCDSetField (cdset_vnp->data.ptrvalue, sp->field_list_to);
        if (!StringHasNoText (str_src) || !StringHasNoText (str_dst))
        {
          BuildNewCDSGeneProtFeature (cdset_vnp->data.ptrvalue, sp->field_list);
          BuildNewCDSGeneProtFeature (cdset_vnp->data.ptrvalue, sp->field_list_to);
          SetCDSetField (cdset_vnp->data.ptrvalue, sp->field_list_to, str_src, NULL);
          SetCDSetField (cdset_vnp->data.ptrvalue, sp->field_list, str_dst, NULL);
        }
      }
      else if (action_choice == AECR_APPLY)
      {
        /* for now, this is taken care of by BuildNewGenes */
        /* BuildNewCDSGeneProtFeature (cdset_vnp->data.ptrvalue, sp->field_list); */
        SetCDSetField (cdset_vnp->data.ptrvalue, sp->field_list, sp->edit_apply->apply_txt, etp);
      }
      else if (action_choice == AECR_REMOVE)
      {
        sp->field_list->choice = 1;
        RemoveCDSetField (cdset_vnp->data.ptrvalue, sp->field_list);
      }
      else if (action_choice == AECR_EDIT)
      {
        str_src = GetCDSetField (cdset_vnp->data.ptrvalue, sp->field_list);
        avd.field_list = sp->field_list;
        avd.etp = NULL;
        AddEditApplyDataToApplyValue (AECR_EDIT, sp->edit_apply, &avd);
        str_src = HandleApplyValue (str_src, &avd);
        SetCDSetField (cdset_vnp->data.ptrvalue, sp->field_list, str_src, NULL);
        avd.text_to_replace = MemFree (avd.text_to_replace);
        avd.new_text = MemFree (avd.new_text);
      }
    }
  }
  FilterSetFree (fsp);
  sp = SimpleAECRFree (sp);
  FreeCDSetList (cdset_list);
  
  if (rval)
  {
    ObjMgrSetDirtyFlag (ap->input_entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, ap->input_entityID, 0, 0);
  }
  ArrowCursor ();
  Update (); 
  
  return rval;

}

static void 
CDSGeneProtApplyEditConvertRemove 
(IteM    i, 
 Int4    first_action_choice, 
 Boolean crippled)
{
  ApplyEditConvertRemoveCombo (i, first_action_choice, crippled,
                               "CDS-Gene-Prot-mRNA",
                               CDSGeneProtAECRDialog, 
                               TRUE, GetCDSGeneProtSample,
                               FALSE, FALSE, FALSE, TRUE, NULL,
                               CDSGeneProtAECRAction, NULL, NULL);
}

extern void ApplyCDSGeneProt (IteM i)
{
  CDSGeneProtApplyEditConvertRemove (i, AECR_APPLY, FALSE);
}

extern void PublicApplyCDSGeneProt (IteM i)
{
  CDSGeneProtApplyEditConvertRemove (i, AECR_APPLY, TRUE);
}

extern void EditCDSGeneProt (IteM i)
{
  CDSGeneProtApplyEditConvertRemove (i, AECR_EDIT, FALSE);
}

extern void PublicEditCDSGeneProt (IteM i)
{
  CDSGeneProtApplyEditConvertRemove (i, AECR_EDIT, TRUE);
}

extern void ConvertCDSGeneProt (IteM i)
{
  CDSGeneProtApplyEditConvertRemove (i, AECR_CONVERT, FALSE);
}

extern void SwapCDSGeneProt (IteM i)
{
  CDSGeneProtApplyEditConvertRemove (i, AECR_SWAP, FALSE);
}

extern void RemoveCDSGeneProt (IteM i)
{
  CDSGeneProtApplyEditConvertRemove (i, AECR_REMOVE, FALSE);
}

#define QUALKIND_SOURCE_QUAL 1
#define QUALKIND_STRINGS     2
#define QUALKIND_LOCATION    3
#define QUALKIND_ORIGIN      4

typedef struct sourcequalaecrdata
{
  Int4          qual_kind;
  SimpleAECRPtr source_qual;
  SimpleAECRPtr location;
  SimpleAECRPtr origin;
  SimpleAECRPtr strings;
  
} SourceQualAECRData, PNTR SourceQualAECRPtr;

static SourceQualAECRPtr SourceQualAECRFree (SourceQualAECRPtr sp)
{
  if (sp != NULL)
  {
    sp->source_qual = SimpleAECRFree (sp->source_qual);
    sp = MemFree (sp);
  }
  return sp;
}

typedef struct sourcequalaecrdlg
{
  DIALOG_MESSAGE_BLOCK
  GrouP  qual_kind;
  
  DialoG source_qual;
  DialoG location;
  DialoG origin;
  DialoG strings;
  
  Int4   action_choice;
  Nlm_ChangeNotifyProc     change_notify;
  Pointer                  change_userdata;
  
} SourceQualAECRDlgData, PNTR SourceQualAECRDlgPtr;

static void ChangeQualKind (GrouP g)
{
  SourceQualAECRDlgPtr dlg;
  Int4                 qual_kind;

  dlg = (SourceQualAECRDlgPtr) GetObjectExtra (g);
  if (dlg == NULL)
  {
    return;
  }
  
  Hide (dlg->source_qual);
  Hide (dlg->location);
  Hide (dlg->origin);
  Hide (dlg->strings);
  qual_kind = GetValue (dlg->qual_kind);
  switch (qual_kind)
  {
    case QUALKIND_SOURCE_QUAL :
      Show (dlg->source_qual);
      break;
    case QUALKIND_LOCATION :
      Show (dlg->location);
      break;
    case QUALKIND_ORIGIN :
      Show (dlg->origin);
      break;
    case QUALKIND_STRINGS :
      Show (dlg->strings);
      break;
  }
  if (dlg->change_notify != NULL)
  {
    (dlg->change_notify)(dlg->change_userdata);
  }
}

static void ResetSourceQualAECRDlg (SourceQualAECRDlgPtr dlg)
{
  if (dlg == NULL)
  {
    return;
  }
  
  PointerToDialog (dlg->source_qual, NULL);
  PointerToDialog (dlg->location, NULL);
  PointerToDialog (dlg->origin, NULL);
  PointerToDialog (dlg->strings, NULL);
  ChangeQualKind (dlg->qual_kind);
}

static void ClearTextSourceQualAECRDlg (SourceQualAECRDlgPtr dlg)
{
  if (dlg == NULL)
  {
    return;
  }
  SendMessageToDialog (dlg->source_qual, AECR_VIB_MSG_CLEAR_TEXT);
  SendMessageToDialog (dlg->location, AECR_VIB_MSG_CLEAR_TEXT);
  SendMessageToDialog (dlg->origin, AECR_VIB_MSG_CLEAR_TEXT);
  SendMessageToDialog (dlg->strings, AECR_VIB_MSG_CLEAR_TEXT);
}

static void SourceQualAECRToDialog (DialoG d, Pointer userdata)
{
  SourceQualAECRDlgPtr dlg;
  SourceQualAECRPtr    data;
  
  dlg = (SourceQualAECRDlgPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return;
  }
  
  ResetSourceQualAECRDlg (dlg);
  
  data = (SourceQualAECRPtr) userdata;
  if (data == NULL)
  {
    return;
  }
  PointerToDialog (dlg->source_qual, data->source_qual);
  PointerToDialog (dlg->location, data->location);
  PointerToDialog (dlg->origin, data->origin);
  PointerToDialog (dlg->strings, data->strings);
  
  if (data->qual_kind < QUALKIND_SOURCE_QUAL)
  {
    SetValue (dlg->qual_kind, QUALKIND_SOURCE_QUAL);
  }
  else if (data->qual_kind > QUALKIND_STRINGS 
           && (dlg->action_choice == AECR_EDIT || dlg->action_choice == AECR_SWAP))
  {
    SetValue (dlg->qual_kind, QUALKIND_STRINGS);
  }
  else if (data->qual_kind > QUALKIND_ORIGIN)
  {
    SetValue (dlg->qual_kind, QUALKIND_ORIGIN);
  }
  else
  {
    SetValue (dlg->qual_kind, data->qual_kind);
  }
  ChangeQualKind (dlg->qual_kind);  
}

static Pointer DialogToSourceQualAECR (DialoG d)
{
  SourceQualAECRDlgPtr dlg;
  SourceQualAECRPtr    data;
  
  dlg = (SourceQualAECRDlgPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return NULL;
  }
  
  data = (SourceQualAECRPtr) MemNew (sizeof (SourceQualAECRData));
  if (data != NULL)
  {
    data->qual_kind = GetValue (dlg->qual_kind);
    switch (data->qual_kind)
    {
      case QUALKIND_SOURCE_QUAL :
        data->source_qual = DialogToPointer (dlg->source_qual);
        break;
      case QUALKIND_LOCATION :
        data->location = DialogToPointer (dlg->location);
        break;
      case QUALKIND_ORIGIN :
        data->origin = DialogToPointer (dlg->origin);
        break;
      case QUALKIND_STRINGS :
        data->strings = DialogToPointer (dlg->strings);
        break;
    }
    
  }
  return data;
}

static void SourceQualAECRMessage (DialoG d, Int2 mssg)

{
  SourceQualAECRDlgPtr  dlg;

  dlg = (SourceQualAECRDlgPtr) GetObjectExtra (d);
  if (dlg != NULL) {
    switch (mssg) 
    {
      case VIB_MSG_INIT :
        /* reset list */
        ResetSourceQualAECRDlg (dlg);
        break;
      case VIB_MSG_ENTER :
        Select (dlg->qual_kind);
        break;
      case AECR_VIB_MSG_SET_DEFAULT :
        SetValue (dlg->qual_kind, QUALKIND_SOURCE_QUAL);
        SendMessageToDialog (dlg->source_qual, AECR_VIB_MSG_SET_DEFAULT);
        SendMessageToDialog (dlg->location, AECR_VIB_MSG_SET_DEFAULT);
        SendMessageToDialog (dlg->origin, AECR_VIB_MSG_SET_DEFAULT);
        SendMessageToDialog (dlg->strings, AECR_VIB_MSG_SET_DEFAULT);
        ChangeQualKind (dlg->qual_kind);
        break;
      case AECR_VIB_MSG_CLEAR_TEXT :
        ClearTextSourceQualAECRDlg (dlg);
        break;
      default :
        break;
    }
  }
}

static ValNodePtr TestSourceQualAECR (DialoG d)
{
  SourceQualAECRDlgPtr dlg;
  ValNodePtr           err_list = NULL;
  ValNodePtr           total_err_list = NULL;
  Int4                 qual_kind;
  
  dlg = (SourceQualAECRDlgPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return FALSE;
  }

  qual_kind = GetValue (dlg->qual_kind);
  switch (qual_kind)
  {
    case QUALKIND_SOURCE_QUAL :
      total_err_list = TestDialog (dlg->source_qual);
      break;
    case QUALKIND_LOCATION :
      total_err_list = TestDialog (dlg->location);
      break;
    case QUALKIND_ORIGIN :
      total_err_list = TestDialog (dlg->origin);
      break;
    case QUALKIND_STRINGS :
      total_err_list = TestDialog (dlg->strings);
      break;
    default:
      ValNodeAddPointer (&total_err_list, 0, "qual kind");
      break;
  }
  return total_err_list;
}

typedef struct applysourcequaldlg
{
  DIALOG_MESSAGE_BLOCK
  DialoG               field_list;
  DialoG               edit_apply;
  PopuP                true_false;
  Nlm_ChangeNotifyProc change_notify;
  Pointer              change_userdata;
  Uint2                entityID;
  
} ApplySourceQualDlgData, PNTR ApplySourceQualDlgPtr;

static void SourceQualChangeNotify (Pointer userdata)
{
  ApplySourceQualDlgPtr dlg;
  ValNodePtr            vnp;
  SourceQualDescPtr     sqdp;
  
  dlg = (ApplySourceQualDlgPtr) userdata;
  if (dlg == NULL)
  {
    return;
  }
  
  vnp = (ValNodePtr) DialogToPointer (dlg->field_list);
  if (vnp == NULL || vnp->data.ptrvalue == NULL)
  {
    Hide (dlg->edit_apply);
    Hide (dlg->true_false);
  }
  else 
  {
    sqdp = (SourceQualDescPtr) vnp->data.ptrvalue;
    if (IsNonTextModifier (sqdp->name))
    {
      Hide (dlg->true_false);
      Hide (dlg->edit_apply);
    }
    else
    {
      Show (dlg->edit_apply);
      Hide (dlg->true_false);
    }
  }
  vnp = ValNodeFreeData (vnp);
  
  if (dlg->change_notify != NULL)
  {
    (dlg->change_notify) (dlg->change_userdata);
  }
}

static void ClearTextApplySourceQualDialog (ApplySourceQualDlgPtr dlg)
{
  if (dlg != NULL)
  {
    PointerToDialog (dlg->edit_apply, NULL);
  }
}

static void ResetApplySourceQualDialog (ApplySourceQualDlgPtr dlg)
{
  if (dlg == NULL)
  {
    return;
  }
  PointerToDialog (dlg->field_list, NULL);
  PointerToDialog (dlg->edit_apply, NULL);
  SetValue (dlg->true_false, 1);
  SourceQualChangeNotify (dlg);
}

static void ApplySourceQualToDialog (DialoG d, Pointer data)
{
  ApplySourceQualDlgPtr dlg;
  SimpleAECRPtr         sap;
  SourceQualDescPtr     sqdp;
  
  dlg = (ApplySourceQualDlgPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return;
  }
  ResetApplySourceQualDialog (dlg);
  
  sap = (SimpleAECRPtr) data;
  if (sap != NULL)
  {
    PointerToDialog (dlg->field_list, sap->field_list);

    if (sap->field_list != NULL && sap->field_list->data.ptrvalue != NULL)
    {
      sqdp = (SourceQualDescPtr) sap->field_list->data.ptrvalue;
      if (IsNonTextModifier (sqdp->name))
      {
        if (sap->edit_apply == NULL 
            || StringHasNoText (sap->edit_apply->apply_txt))
        {
          SetValue (dlg->true_false, 2);
        }
        else
        {
          SetValue (dlg->true_false, 1);
        }
      }
      else
      {
        PointerToDialog (dlg->edit_apply, sap->edit_apply);
      }
    }
  }
  SourceQualChangeNotify (dlg);
}

static Pointer DialogToApplySourceQual (DialoG d)
{
  ApplySourceQualDlgPtr dlg;
  SimpleAECRPtr         sap;
  SourceQualDescPtr     sqdp;
  
  dlg = (ApplySourceQualDlgPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return NULL;
  }
  
  sap = (SimpleAECRPtr) MemNew (sizeof (SimpleAECRData));
  if (sap == NULL)
  {
    return NULL;
  }
  sap->field_list = DialogToPointer (dlg->field_list);
  sap->edit_apply = DialogToPointer (dlg->edit_apply);
  if (sap->field_list != NULL && sap->field_list->data.ptrvalue != NULL)
  {
    sqdp = (SourceQualDescPtr) sap->field_list->data.ptrvalue;
    if (IsNonTextModifier (sqdp->name))
    {
      sap->edit_apply->apply_txt = MemFree (sap->edit_apply->apply_txt);
      if (GetValue (dlg->true_false) == 1)
      {
        sap->edit_apply->apply_txt = StringSave ("TRUE");
      }
    }
  }
  
  sap->free_field_vn_proc = ValNodeSimpleDataFree;
  sap->free_subtype_vn_proc = NULL;
  return sap;  
}

static void ApplySourceQualMessage (DialoG d, Int2 mssg)

{
  ApplySourceQualDlgPtr  dlg;

  dlg = (ApplySourceQualDlgPtr) GetObjectExtra (d);
  if (dlg != NULL) {
    switch (mssg) 
    {
      case VIB_MSG_INIT :
        /* reset list */
        ResetApplySourceQualDialog (dlg);
        break;
      case VIB_MSG_ENTER :
        Select (dlg->field_list);
        break;
      case AECR_VIB_MSG_SET_DEFAULT :
        SendMessageToDialog (dlg->field_list, AECR_VIB_MSG_SET_DEFAULT);
        ClearTextApplySourceQualDialog (dlg);
        break;
      case AECR_VIB_MSG_CLEAR_TEXT :
        ClearTextApplySourceQualDialog (dlg);
        break;
      case AECR_VIB_MSG_AUTOPOPULATE:
        SourceQualChangeNotify (dlg);
        break; 
      default :
        break;
    }
  }
}

static ValNodePtr TestApplySourceQual (DialoG d)
{
  ApplySourceQualDlgPtr dlg;
  ValNodePtr            err_list = NULL;
  ValNodePtr            total_err_list = NULL;
  ValNodePtr            field_list;
  SourceQualDescPtr     sqdp;
   
  dlg = (ApplySourceQualDlgPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return NULL;
  }

  err_list = TestDialog (dlg->field_list);
  total_err_list = ValNodeAppend (total_err_list, err_list);
      
  field_list = DialogToPointer (dlg->field_list);
  if (field_list != NULL && field_list->data.ptrvalue != NULL)
  {
    sqdp = (SourceQualDescPtr) field_list->data.ptrvalue;
    if (IsNonTextModifier (sqdp->name))
    {
      /* any true or false value is valid */
    }
    else
    {
      err_list = TestDialog (dlg->edit_apply);
      total_err_list = ValNodeAppend (total_err_list, err_list);
    }
  }
  field_list = ValNodeFreeData (field_list);
      
  return total_err_list;
}

static DialoG ApplySourceQualDialog
(GrouP                    h,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata,
 Uint2                    entityID)
{
  ApplySourceQualDlgPtr dlg;  
  GrouP                 p, g1, g2;
  
  dlg = (ApplySourceQualDlgPtr) MemNew (sizeof (ApplySourceQualDlgData));
  if (dlg == NULL)
  {
    return NULL;
  }

  p = HiddenGroup (h, -1, 0, NULL);
  SetObjectExtra (p, dlg, StdCleanupExtraProc);
  SetGroupSpacing (p, 10, 10);
  
  dlg->dialog = (DialoG) p;
  dlg->todialog = ApplySourceQualToDialog;
  dlg->fromdialog = DialogToApplySourceQual;
  dlg->dialogmessage = ApplySourceQualMessage;
  dlg->testdialog = TestApplySourceQual;
  dlg->change_notify = change_notify;
  dlg->change_userdata = change_userdata;
  dlg->entityID = entityID;
  
  g1 = HiddenGroup (p, 1, 0, NULL);
  StaticPrompt (g1, "Qualifier", 0, dialogTextHeight, systemFont, 'l');

  dlg->field_list = SourceQualTypeSelectionDialog (g1, FALSE, 
                                                SourceQualChangeNotify,
                                                dlg);
  
  g2 = HiddenGroup (p, 0, 0, NULL);
  dlg->edit_apply = EditApplyDialog (g2, AECR_APPLY, "New Text",
                                     change_notify, change_userdata);
  
  dlg->true_false = PopupList (g2, TRUE, NULL);
  PopupItem (dlg->true_false, "TRUE");
  PopupItem (dlg->true_false, "FALSE");
  SetValue (dlg->true_false, 1);
  
  Hide (dlg->edit_apply);
  Hide (dlg->true_false);
  
  AlignObjects (ALIGN_CENTER, (HANDLE) dlg->edit_apply, (HANDLE) dlg->true_false, NULL);
  
  AlignObjects (ALIGN_CENTER, (HANDLE) g1, (HANDLE) g2, NULL);
  
  SourceQualChangeNotify (dlg);
  
  return (DialoG) p;
}

static DialoG SourceQualAECRDialog 
(GrouP                    h,
 Int4                     action_choice, 
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata,
 Uint2                    entityID)
{

  SourceQualAECRDlgPtr dlg;
  GrouP                p, g1;
  
  dlg = (SourceQualAECRDlgPtr) MemNew (sizeof (SourceQualAECRDlgData));
  if (dlg == NULL)
  {
    return NULL;
  }

  p = HiddenGroup (h, -1, 0, NULL);
  SetObjectExtra (p, dlg, StdCleanupExtraProc);
  SetGroupSpacing (p, 10, 10);
  
  dlg->dialog = (DialoG) p;
  dlg->todialog = SourceQualAECRToDialog;
  dlg->fromdialog = DialogToSourceQualAECR;
  dlg->dialogmessage = SourceQualAECRMessage;
  dlg->testdialog = TestSourceQualAECR;
  dlg->action_choice = action_choice;
  dlg->change_notify = change_notify;
  dlg->change_userdata = change_userdata;

  dlg->qual_kind = HiddenGroup (p, 4, 0, ChangeQualKind);
  SetObjectExtra (dlg->qual_kind, dlg, NULL);
  RadioButton (dlg->qual_kind, "Qualifiers");
  RadioButton (dlg->qual_kind, "Taxonomy");
  if (action_choice != AECR_EDIT && action_choice != AECR_SWAP)
  {
    RadioButton (dlg->qual_kind, "Location");
    RadioButton (dlg->qual_kind, "Origin");
  }
  SetValue (dlg->qual_kind, QUALKIND_SOURCE_QUAL);
    
  g1 = HiddenGroup (p, 0, 0, NULL);
  /* Source Qualifiers dialog */
  if (action_choice == AECR_APPLY)
  {
    dlg->source_qual = ApplySourceQualDialog (g1, 
                                              change_notify, 
                                              change_userdata,
                                              entityID);
  }
  else
  {
    dlg->source_qual = SimpleAECRDialog (g1, action_choice, 
                           change_notify, change_userdata,
                           ValNodeSimpleDataFree, NULL,
                           SourceQualTypeSelectionDialog, NULL, NULL,
                           "Qualifier", NULL, TRUE, entityID);    
  }
  
  /* Taxonomy Strings dialog */
  dlg->strings = SimpleAECRDialog (g1, action_choice, change_notify, change_userdata,
                           ValNodeSimpleDataFree, NULL,
                           SourceStringSelectionDialog, NULL, NULL,
                           "Taxonomy", NULL, TRUE, entityID);
                          
  if (action_choice != AECR_EDIT && action_choice != AECR_SWAP)
  {
    /* Location Dialog */
    dlg->location = SimpleAECRDialog (g1, action_choice, change_notify, change_userdata,
                           ValNodeSimpleDataFree, NULL,
                           SourceLocationSelectionDialog, NULL, NULL,
                           "Location", NULL, FALSE, entityID);
    /* Origin Dialog */
    dlg->origin = SimpleAECRDialog (g1, action_choice, change_notify, change_userdata,
                           ValNodeSimpleDataFree, NULL,
                           SourceOriginSelectionDialog, NULL, NULL,
                           "Origin", NULL, FALSE, entityID);
  }
  
  AlignObjects (ALIGN_CENTER, (HANDLE) dlg->source_qual,
                              (HANDLE) dlg->location,
                              (HANDLE) dlg->origin,
                              (HANDLE) dlg->strings,
                              NULL);
                              
  AlignObjects (ALIGN_CENTER, (HANDLE) dlg->qual_kind,
                              (HANDLE) g1,
                              NULL);
                           
  ChangeQualKind (dlg->qual_kind);                         
  return (DialoG) p;
}

static Boolean 
SourceQualQualApplyEditConvertRemoveAction
(ApplyEditConvertRemovePtr   ap,
 SimpleAECRPtr               sp,
 Int4                        action_choice,
 SetFeatureFieldString       feature_apply_action,
 SetDescriptorFieldString    descriptor_apply_action,
 RemoveFeatureFieldString    feature_remove_action,
 RemoveDescriptorFieldString descriptor_remove_action,
 CopyValNodeDataProc         copy_vn_proc,
 GetFeatureFieldString       fieldstring_func,
 GetDescriptorFieldString    descrstring_func,
 Boolean                     non_text,
 NameFromValNodeProc         name_field_func)

{
  SeqEntryPtr               sep;
  ApplyValueData            avd;
  ConvertFieldData          cfd;
  FilterSetPtr              fsp;
  GetSamplePtr              gsp;
  Boolean                   rval = TRUE;
  ValNodePtr                vnp;
  
  sep = GetTopSeqEntryForEntityID (ap->input_entityID);
  if (sp == NULL
      || sp->field_list == NULL
      || feature_apply_action == NULL
      || descriptor_apply_action == NULL
      || feature_remove_action == NULL
      || descriptor_remove_action == NULL
      || ((action_choice == AECR_CONVERT || action_choice == AECR_SWAP) 
           && sp->field_list == NULL))
  {
    return FALSE;
  }
  WatchCursor ();
  Update ();
  
  fsp = (FilterSetPtr) DialogToPointer (ap->constraints);
  
  if (action_choice == AECR_CONVERT || action_choice == AECR_SWAP)
  {
    cfd.src_field_list = sp->field_list;
    cfd.dst_field_list = sp->field_list_to;
    cfd.etp = NULL;
    cfd.get_str_func = fieldstring_func;
    cfd.set_str_func = feature_apply_action;
    cfd.remove_str_func = feature_remove_action;
    cfd.get_d_str_func = descrstring_func;
    cfd.set_d_str_func = descriptor_apply_action;
    cfd.remove_d_str_func = descriptor_remove_action;
    cfd.fsp = fsp;
    if (action_choice == AECR_CONVERT)
    {
      if (sp->leave_on_original)
      {
        cfd.convert_type = CONVERT_TYPE_COPY;
      }
      else
      {
        cfd.convert_type = CONVERT_TYPE_MOVE;
      }
    }
    else if (action_choice == AECR_SWAP)
    {
      cfd.convert_type = CONVERT_TYPE_SWAP;
    }
    if ((cfd.convert_type == CONVERT_TYPE_MOVE || cfd.convert_type == CONVERT_TYPE_COPY)
        && ! non_text)
    {
      gsp = CheckForConversionExistingTextInSeqEntry (sep, &cfd, fsp, 
                                                      SEQFEAT_BIOSRC, 0,
                                                      Seq_descr_source);
      cfd.etp = GetExistingTextHandlerInfo (gsp, non_text);
      gsp = GetSampleFree (gsp);
      if (cfd.etp != NULL && cfd.etp->existing_text_choice == EXISTING_TEXT_CHOICE_CANCEL)
      {
        rval = FALSE;
      }
    }
    else
    {
      cfd.etp = NULL;
    }  

    if (rval)
    {
      if (non_text)
      {
        cfd.name_field_func = name_field_func;

        OperateOnSeqEntryConstrainedObjects (sep, fsp, 
                                             ConvertNonTextFeatureFieldCallback,
                                             ConvertNonTextDescriptorFieldCallback,
                                             SEQFEAT_BIOSRC, 0, Seq_descr_source,
                                             &cfd);    
      }
      else
      {
        OperateOnSeqEntryConstrainedObjects (sep, fsp, 
                                             ConvertFeatureFieldCallback,
                                             ConvertDescriptorFieldCallback,
                                             SEQFEAT_BIOSRC, 0, Seq_descr_source,
                                             &cfd);    
      }
      
    }
    cfd.etp = MemFree (cfd.etp);
  }
  else
  {
    avd.field_list = sp->field_list;
  
    /* get handling for existing text */
    if (action_choice == AECR_APPLY)
    {
      gsp = CheckForExistingText (ap->input_entityID,
                                  avd.field_list,
                                  fieldstring_func,
                                  descrstring_func,
                                  ValNodeSimpleDataFree,
                                  copy_vn_proc,
                                  fsp, SEQFEAT_BIOSRC, 0, Seq_descr_source);
      avd.etp = GetExistingTextHandlerInfo (gsp, non_text);
      gsp = GetSampleFree (gsp);
    
      if (avd.etp != NULL 
          && avd.etp->existing_text_choice == EXISTING_TEXT_CHOICE_CANCEL)
      {
        rval = FALSE;
      }
    }
    else
    {
      avd.etp = NULL;
    }
  
    if (rval)
    {
      AddEditApplyDataToApplyValue (action_choice, sp->edit_apply, &avd);
   
      if (action_choice == AECR_EDIT || action_choice == AECR_APPLY)
      {
        OperateOnSeqEntryConstrainedObjects (sep, fsp, 
                                             feature_apply_action,
                                             descriptor_apply_action,
                                             SEQFEAT_BIOSRC, 0, Seq_descr_source, &avd);  
      }
      else if (action_choice == AECR_REMOVE)
      {
        for (vnp = sp->field_list; vnp != NULL; vnp = vnp->next)
        {
          avd.field_list = vnp;
          OperateOnSeqEntryConstrainedObjects (sep, fsp, 
                                               feature_remove_action,
                                               descriptor_remove_action,
                                               SEQFEAT_BIOSRC, 0, Seq_descr_source, &avd);
        }
      }

      avd.text_to_replace = MemFree (avd.text_to_replace);
      avd.new_text = MemFree (avd.new_text);
    }
    avd.etp = MemFree (avd.etp);
  }
  FilterSetFree (fsp);
  
  if (rval)
  {
    ObjMgrSetDirtyFlag (ap->input_entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, ap->input_entityID, 0, 0);
  }
  ArrowCursor ();
  Update (); 
  
  return rval;
}

static Boolean SourceQualApplyEditConvertRemoveAction (Pointer userdata)
{
  ApplyEditConvertRemovePtr ap;  
  SourceQualAECRPtr         sp;
  Int4                      action_choice;
  Boolean                   rval = FALSE;
  
  ap = (ApplyEditConvertRemovePtr) userdata;
  if (ap == NULL)
  {
    return FALSE;
  }
  
  if (ap->crippled)
  {
    action_choice = ap->crippled_action;
  }
  else
  {
    action_choice = GetValue (ap->action_popup);
  }
  if (action_choice < 1 || action_choice > NUM_AECR)
  {
    return FALSE;
  }
  sp = (SourceQualAECRPtr) DialogToPointer (ap->aecr_pages[action_choice - 1]);
  
  switch (sp->qual_kind)
  {
    case QUALKIND_SOURCE_QUAL :
      rval = SourceQualQualApplyEditConvertRemoveAction (ap, sp->source_qual, 
                                                         action_choice,
                                       ApplySourceQualFeatureCallback,
                                       ApplySourceQualDescriptorCallback,
                                       RemoveSourceQualFromSourceFeature,
                                       RemoveSourceQualFromSourceDescriptor,
                                       SourceQualValNodeDataCopy,
                                       GetSourceQualFeatureString,
                                       GetSourceQualDescrString,
                                       FALSE,
                                       NULL);
      break;
    case QUALKIND_STRINGS :
      rval = SourceQualQualApplyEditConvertRemoveAction (ap, sp->strings, 
                                                         action_choice,
                                       ApplyStringToSourceFeature,
                                       ApplyStringToSourceDescriptor,
                                       RemoveStringFromSourceFeature,
                                       RemoveStringFromSourceDescriptor,
                                       ValNodeStringCopy,
                                       GetStringFromSourceFeature,
                                       GetStringFromSourceDescriptor,
                                       FALSE,
                                       NULL);
      break;
    case QUALKIND_LOCATION :
      rval = SourceQualQualApplyEditConvertRemoveAction (ap, sp->location, 
                                                         action_choice,
                                       ApplyLocationToSourceFeature,
                                       ApplyLocationToSourceDescriptor,
                                       RemoveLocationFromSourceFeature,
                                       RemoveLocationFromSourceDescriptor,
                                       ValNodeStringCopy,
                                       GetLocationFromSourceFeature,
                                       GetLocationFromSourceDescriptor,
                                       TRUE,
                                       ValNodeStringName);
      break;      
    case QUALKIND_ORIGIN :
      rval = SourceQualQualApplyEditConvertRemoveAction (ap, sp->origin, 
                                                         action_choice,
                                       ApplyOriginToSourceFeature,
                                       ApplyOriginToSourceDescriptor,
                                       RemoveOriginFromSourceFeature,
                                       RemoveOriginFromSourceDescriptor,
                                       ValNodeStringCopy,
                                       GetOriginFromSourceFeature,
                                       GetOriginFromSourceDescriptor,
                                       TRUE,
                                       ValNodeStringName);
      break;      
  }
  sp = SourceQualAECRFree (sp);
  return rval;
}

static void ChangeSourceQualAction (DialoG prev_page, DialoG new_page)
{
  SourceQualAECRPtr         prev_sp, new_sp;
  
  if (prev_page == NULL || new_page == NULL)
  {
    return;
  }
  
  prev_sp = (SourceQualAECRPtr) DialogToPointer (prev_page);
  new_sp = (SourceQualAECRPtr) DialogToPointer (new_page);
  if (prev_sp != NULL && new_sp != NULL)
  {
    new_sp->qual_kind = prev_sp->qual_kind;
    PointerToDialog (new_page, new_sp);
  }
  new_sp = SourceQualAECRFree (new_sp);
  prev_sp = SourceQualAECRFree (prev_sp);
}

static DialoG GetSourceQualSample (GrouP h, Uint2 entityID)
{
  DialoG                d;
  SetSampleData         ssd;

  d = SampleDialog (h);
  ssd.fieldstring_func = GetSourceQualFeatureString;
  ssd.descrstring_func = GetSourceQualDescrString;
  ssd.entityID = entityID;
  ssd.field_list = GetSourceQualDescList (TRUE, TRUE);
  ValNodeAddPointer (&(ssd.field_list), 1, StringSave ("Scientific Name"));
  ValNodeAddPointer (&(ssd.field_list), 1, StringSave ("Location"));
  
  ssd.fsp = NULL;
  ssd.free_vn_proc = ValNodeSimpleDataFree;
  ssd.copy_vn_proc = SourceQualValNodeDataCopy;
  ssd.match_vn_proc = SourceQualValNodeMatch;
  ssd.label_vn_proc = SourceQualValNodeName;  
  PointerToDialog (d, &ssd);
  return d;
}

static void 
SourceQualApplyEditConvertRemove 
(IteM    i,
 Int4    first_action_choice,
 Boolean crippled)
{
  ApplyEditConvertRemoveCombo (i, first_action_choice, crippled,
                               "Source Qual",
                               SourceQualAECRDialog, 
                               FALSE, GetSourceQualSample,
                               FALSE, TRUE, FALSE, FALSE, NULL,
                               SourceQualApplyEditConvertRemoveAction, NULL,
                               ChangeSourceQualAction);
}

extern void ApplySourceQual (IteM i)
{
  SourceQualApplyEditConvertRemove (i, AECR_APPLY, FALSE);
}

extern void PublicApplySourceQual (IteM i)
{
  SourceQualApplyEditConvertRemove (i, AECR_APPLY, TRUE);
}

extern void EditSourceQual (IteM i)
{
  SourceQualApplyEditConvertRemove (i, AECR_EDIT, FALSE);
}

extern void PublicEditSourceQual (IteM i)
{
  SourceQualApplyEditConvertRemove (i, AECR_EDIT, TRUE);
}

extern void ConvertSourceQual (IteM i)
{
  SourceQualApplyEditConvertRemove (i, AECR_CONVERT, FALSE);
}

extern void SwapSourceQual (IteM i)
{
  SourceQualApplyEditConvertRemove (i, AECR_SWAP, FALSE);
}

extern void RemoveSourceQual (IteM i)
{
  SourceQualApplyEditConvertRemove (i, AECR_REMOVE, FALSE);
}

static CharPtr rna_sample_field_list [] = { "Name", 
                                            "Comment", 
                                            "Product GBQual" };
static Int2    num_rna_sample_fields = sizeof (rna_sample_field_list) / sizeof (CharPtr);



static CharPtr GetRNASampleFieldString (SeqFeatPtr sfp, ValNodePtr vnp, FilterSetPtr fsp)
{
  RnaRefPtr rrp;
  CharPtr   str = NULL;
  GBQualPtr gbqual;
  Int2      rna_subtype, rna_field_type;
  
  if (sfp == NULL || sfp->data.choice != SEQFEAT_RNA || vnp == NULL)
  {
    return NULL;
  }
  
  rrp = sfp->data.value.ptrvalue;
  
  while (vnp != NULL && str == NULL)
  {
    rna_subtype = (vnp->data.intvalue - 1) / num_rna_sample_fields;
    rna_field_type = (vnp->data.intvalue - 1) % num_rna_sample_fields;
    if (rna_subtype == 8)
    {
      rna_subtype = 255;
    }
    if (rna_subtype != rrp->type)
    {
      vnp = vnp->next;
      continue;
    }
    switch (rna_field_type)
    {
      case 0 :
        if (rrp != NULL && rrp->ext.choice == 1 && !StringHasNoText (rrp->ext.value.ptrvalue))
        {
          str = StringSave (rrp->ext.value.ptrvalue);        
        }
        break;
      case 1 :
        if (!StringHasNoText (sfp->comment))
        {
          str = StringSave (sfp->comment);
        }
        break;
      case 2 :
        gbqual = sfp->qual;
        while (gbqual != NULL && str == NULL) {
          if (StringICmp (gbqual->qual, "product") == 0
              && !StringHasNoText (gbqual->val)) 
          {
            str = StringSave (gbqual->val);
          }
          gbqual = gbqual->next;
        }
        break;      
    }
    vnp = vnp->next;
  }
  return str;
}

static CharPtr GetRNAQualSampleName (ValNodePtr vnp)
{
  CharPtr label = NULL;
  Int2    rna_subtype, rna_field_type;
  
  if (vnp == NULL)
  {
    return NULL;
  }
  rna_subtype = (vnp->data.intvalue - 1) / num_rna_sample_fields;
  rna_field_type = (vnp->data.intvalue - 1) % num_rna_sample_fields;
  
  label = (CharPtr) MemNew ((StringLen (rna_subtype_list [rna_subtype])
                            + StringLen (rna_sample_field_list[rna_field_type])
                            + 2) * sizeof (Char));
  if (label != NULL)
  {
    sprintf (label, "%s %s", rna_subtype_list [rna_subtype],
             rna_sample_field_list[rna_field_type]);
  }
  return label;
}

static DialoG GetRNAQualSample (GrouP h, Uint2 entityID)
{
  DialoG        d;
  SetSampleData ssd;
  Int2          rna_subtype, field_type;
  Int4          list_index;      

  d = SampleDialog (h);
  ssd.fieldstring_func = GetRNASampleFieldString;
  ssd.descrstring_func = NULL;
  ssd.entityID = entityID;
  
  /* construct RNA field list for separate types of RNAs */
  ssd.free_vn_proc = NULL;
  ssd.copy_vn_proc = IntValNodeCopy;
  ssd.match_vn_proc = IntValNodeMatch;
  ssd.label_vn_proc = GetRNAQualSampleName;
  
  ssd.field_list = NULL;
  for (rna_subtype = 0; rna_subtype < num_rna_subtypes; rna_subtype++)
  {
    for (field_type = 0; field_type < num_rna_sample_fields; field_type++)
    {
      list_index = (rna_subtype * num_rna_sample_fields) + field_type;
      ValNodeAddInt (&ssd.field_list, 0, list_index + 1);
    }
  }
  ssd.fsp = NULL;
  PointerToDialog (d, &ssd);
  
  /* now free up field list (SampleDialog maintains its own copy */
  ssd.field_list = ValNodeFree (ssd.field_list);
  
  return d;
}

typedef struct applyrnaqual
{
  ValNodePtr   field_list;
  ValNodePtr   subtype_list;
  EditApplyPtr edit_apply;
  ValNodePtr   codons;
  SeqLocPtr    anticodon;
} ApplyRNAQualData, PNTR ApplyRNAQualPtr;

static ApplyRNAQualPtr ApplyRNAQualFree (ApplyRNAQualPtr ap)
{
  if (ap != NULL)
  {
    ap->field_list = ValNodeFree (ap->field_list);
    ap->subtype_list = ValNodeFree (ap->subtype_list);
    ap->edit_apply = EditApplyFree (ap->edit_apply);
    ap->codons = ValNodeFreeData (ap->codons);
    ap = MemFree (ap);
  }
  return ap;
}

typedef struct applyrnaqualdlg
{
  DIALOG_MESSAGE_BLOCK
  DialoG               field_list;
  DialoG               subtype_list;
  DialoG               edit_apply;
  DialoG               codons;
  Nlm_ChangeNotifyProc change_notify;
  Pointer              change_userdata;
  Uint2                entityID;
  
  /* The following is used to create the anticodon location dialog */
  FeatureForm          ff;
  DialoG               anticodon;
} ApplyRNAQualDlgData, PNTR ApplyRNAQualDlgPtr;

static void ClearTextApplyRNAQualDlg (ApplyRNAQualDlgPtr dlg)
{
  if (dlg != NULL)
  {
    PointerToDialog (dlg->edit_apply, NULL);
    PointerToDialog (dlg->codons, NULL);
  }
}

static void ResetApplyRNAQualDlg (ApplyRNAQualDlgPtr dlg)
{
  if (dlg != NULL)
  {
    PointerToDialog (dlg->field_list, NULL);
    PointerToDialog (dlg->subtype_list, NULL);
    ClearTextApplyRNAQualDlg (dlg);
  }
}

static void ApplyRNAQualToDialog (DialoG d, Pointer userdata)
{
  ApplyRNAQualDlgPtr dlg;
  ApplyRNAQualPtr    data;
  
  dlg = (ApplyRNAQualDlgPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return;
  }
  
  ResetApplyRNAQualDlg (dlg);
  data = (ApplyRNAQualPtr) userdata;
  if (data != NULL)
  {
    PointerToDialog (dlg->field_list, data->field_list);
    PointerToDialog (dlg->subtype_list, data->subtype_list);
    PointerToDialog (dlg->edit_apply, data->edit_apply);
    PointerToDialog (dlg->codons, data->codons);
    PointerToDialog (dlg->anticodon, data->anticodon);
  }
}

static Pointer DialogToApplyRNAQual (DialoG d)
{
  ApplyRNAQualDlgPtr dlg;
  ApplyRNAQualPtr    data;
  
  dlg = (ApplyRNAQualDlgPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return NULL;
  }
  
  data = (ApplyRNAQualPtr) MemNew (sizeof (ApplyRNAQualData));
  if (data != NULL)
  {
    data->field_list = DialogToPointer (dlg->field_list);
    data->subtype_list = DialogToPointer (dlg->subtype_list);
    data->edit_apply = DialogToPointer (dlg->edit_apply);
    data->codons = DialogToPointer (dlg->codons);
    data->anticodon = DialogToPointer (dlg->anticodon);
  }
  return data;
}

static CharPtr 
RNAQualAutopopulate 
(ValNodePtr subtype_list,
 ValNodePtr field_list,
 Uint2      entityID)
{
  GetSamplePtr gsp;
  CharPtr      rval = NULL;
  ValNodePtr   vnp;

  for (vnp = subtype_list; vnp != NULL && rval == NULL; vnp = vnp->next)
  {
    gsp = CheckForExistingText (entityID,
                                field_list,
                                GetRNAFieldString,
                                NULL, NULL, IntValNodeCopy,
                                NULL, SEQFEAT_RNA, vnp->data.intvalue, 0);
    if (gsp != NULL && !StringHasNoText (gsp->sample_text))
    {
      rval = gsp->sample_text;
      gsp->sample_text = NULL;
    }
    GetSampleFree (gsp);
  }
  return rval;
}

static void RNAQualChangeNotify (Pointer userdata)
{
  ApplyRNAQualDlgPtr dlg;
  ValNodePtr         field_list = NULL, subtype_list = NULL;
  EditApplyPtr       eap;

  dlg = (ApplyRNAQualDlgPtr) userdata;
  if (dlg == NULL)
  {
    return;
  }
  
  field_list = DialogToPointer (dlg->field_list);
  
  if (field_list->data.intvalue == RNAFIELD_CODONS_RECOGNIZED)
  {
    Show (dlg->codons);
    Hide (dlg->edit_apply);
    Hide (dlg->anticodon);
  }
  else if (field_list->data.intvalue == RNAFIELD_ANTICODON)
  {
    Show (dlg->anticodon);
    Hide (dlg->edit_apply);
    Hide (dlg->codons);
  }
  else
  {
    Show (dlg->edit_apply);
    Hide (dlg->codons);
    Hide (dlg->anticodon);

    /* autopopulate text fields */
    subtype_list = DialogToPointer (dlg->subtype_list);
    eap = DialogToPointer (dlg->edit_apply);
    eap->apply_txt = MemFree (eap->apply_txt);
    eap->apply_txt = RNAQualAutopopulate (subtype_list, field_list, dlg->entityID);
    PointerToDialog (dlg->edit_apply, eap);
    EditApplyFree (eap);    
    subtype_list = ValNodeFree (subtype_list);
  }
  
  field_list = ValNodeFree (field_list);
  
  if (dlg->change_notify != NULL)
  {
    (dlg->change_notify)(dlg->change_userdata);
  }  
}

static void ApplyRNAQualMessage (DialoG d, Int2 mssg)

{
  ApplyRNAQualDlgPtr  dlg;

  dlg = (ApplyRNAQualDlgPtr) GetObjectExtra (d);
  if (dlg != NULL) {
    switch (mssg) 
    {
      case VIB_MSG_INIT :
        /* reset list */
        ResetApplyRNAQualDlg (dlg);
        break;
      case VIB_MSG_ENTER :
        if (dlg->subtype_list != NULL)
        {
          Select (dlg->subtype_list);
        }
        else
        {
          Select (dlg->field_list);
        }
        break;
      case AECR_VIB_MSG_SET_DEFAULT :
        SendMessageToDialog (dlg->field_list, AECR_VIB_MSG_SET_DEFAULT);
        SendMessageToDialog (dlg->subtype_list, AECR_VIB_MSG_SET_DEFAULT);
        ClearTextApplyRNAQualDlg (dlg);
        break;
      case AECR_VIB_MSG_CLEAR_TEXT :
        ClearTextApplyRNAQualDlg (dlg);
        break;
      case AECR_VIB_MSG_AUTOPOPULATE:
        RNAQualChangeNotify (dlg);
        break; 
      default :
        break;
    }
  }
}

static ValNodePtr TestApplyRNAQual (DialoG d)
{
  ApplyRNAQualDlgPtr dlg;
  ValNodePtr         err_list = NULL;
  ValNodePtr         total_err_list = NULL;
  ValNodePtr         field_list;
  
  dlg = (ApplyRNAQualDlgPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return NULL;
  }

  total_err_list = TestDialog (dlg->subtype_list);
  
  err_list = TestDialog (dlg->field_list);
  total_err_list = ValNodeAppend (total_err_list, err_list);
      
  field_list = DialogToPointer (dlg->field_list);
  if (field_list != NULL && field_list->data.intvalue != RNAFIELD_CODONS_RECOGNIZED)
  {
    err_list = TestDialog (dlg->edit_apply);
    total_err_list = ValNodeAppend (total_err_list, err_list);
  }
      
  return total_err_list;
}

static DialoG ApplyRNAQualDialog
(GrouP                    h,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata,
 Uint2                    entityID)
{
  ApplyRNAQualDlgPtr dlg;
  GrouP              p, g1, g2;
  SeqEntryPtr        sep;
  
  dlg = (ApplyRNAQualDlgPtr) MemNew (sizeof (ApplyRNAQualDlgData));
  if (dlg == NULL)
  {
    return NULL;
  }

  p = HiddenGroup (h, -1, 0, NULL);
  SetObjectExtra (p, dlg, StdCleanupExtraProc);
  SetGroupSpacing (p, 10, 10);
  
  dlg->dialog = (DialoG) p;
  dlg->todialog = ApplyRNAQualToDialog;
  dlg->fromdialog = DialogToApplyRNAQual;
  dlg->dialogmessage = ApplyRNAQualMessage;
  dlg->testdialog = TestApplyRNAQual;
  dlg->change_notify = change_notify;
  dlg->change_userdata = change_userdata;
  dlg->entityID = entityID;
  
  g1 = HiddenGroup (p, 2, 0, NULL);
  StaticPrompt (g1, "RNA Type", 0, dialogTextHeight, systemFont, 'l');
  StaticPrompt (g1, "RNA Field", 0, dialogTextHeight, systemFont, 'l');
  dlg->subtype_list = RNASubtypeSelectionDialog (g1, TRUE, 
                                                 RNAQualChangeNotify,
                                                 dlg);
  dlg->field_list = RNAAddFieldSelectionDialog (g1, FALSE, 
                                                RNAQualChangeNotify,
                                                dlg);
  
  g2 = HiddenGroup (p, 0, 0, NULL);
  dlg->edit_apply = EditApplyDialog (g2, AECR_APPLY, "New Text",
                                     change_notify, change_userdata);
  
  dlg->codons = CreateVisibleStringDialog (g2, 3, -1, 4);
  
  sep = GetTopSeqEntryForEntityID (entityID);
  
  dlg->anticodon = NULL;
/*
  dlg->anticodon = CreateIntervalEditorDialogEx (g2, NULL, 2, 2, sep, TRUE, FALSE,
                                                  TRUE, TRUE, FALSE,
                                                  &(dlg->ff),
                                                  NULL); */

  AlignObjects (ALIGN_CENTER, (HANDLE) dlg->edit_apply,
                              (HANDLE) dlg->codons,
                              (HANDLE) dlg->anticodon,
                              NULL);

  AlignObjects (ALIGN_CENTER, (HANDLE) g1, (HANDLE) g2, NULL);
  
  RNAQualChangeNotify (dlg);
  
  return (DialoG) p;
}

static DialoG RNAQualListDialog 
(GrouP                    h,
 Int4                     action_choice,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata,
 Uint2                    entityID)
{
  DialoG d = NULL;
  
  switch (action_choice)
  {
    case AECR_REMOVE:
      d = SimpleAECRDialog (h, action_choice, change_notify, change_userdata,
                            NULL, NULL,
                            RNARemoveFieldSelectionDialog,
                            RNASubtypeSelectionDialog,
                            NULL,
                            "RNA Field", "RNA type", FALSE, entityID);
      break;
    case AECR_CONVERT:
    case AECR_SWAP:
      d = SimpleAECRDialog (h, action_choice, change_notify, change_userdata,
                            NULL, NULL,
                            RNAFieldSelectionDialog,
                            RNASubtypeSelectionDialog,
                            NULL,
                            "RNA Field", "RNA type", TRUE, entityID);
      break;
      break;
    case AECR_EDIT:
      d = SimpleAECRDialog (h, action_choice, change_notify, change_userdata,
                            NULL, NULL,
                            RNAFieldSelectionDialog,
                            RNASubtypeSelectionDialog,
                            RNAQualAutopopulate,
                            "RNA Field", "RNA type", TRUE, entityID);
      break;
    case AECR_APPLY:
      d = ApplyRNAQualDialog (h, change_notify, change_userdata, entityID);
      break;
  }
  return d;
}

static void ApplyRNACodonsCallback (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  ValNodePtr codons;
  RnaRefPtr  rrp;
  
  if (sfp == NULL || sfp->data.choice != SEQFEAT_RNA)
  {
    return;
  }
  
  rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
  codons = (ValNodePtr) userdata;
  if (rrp != NULL && rrp->type == 3 && rrp->ext.choice == 2) 
  {
    AddCodonListTotRNA (rrp->ext.value.ptrvalue, codons);
  }
}

static void ApplyRNAAnticodonCallback (SeqFeatPtr sfp, Pointer userdata, FilterSetPtr fsp)
{
  RnaRefPtr  rrp;
  tRNAPtr    trp;
  SeqLocPtr  anticodon_loc, slp;
  
  if (sfp == NULL || sfp->data.choice != SEQFEAT_RNA 
      || sfp->data.value.ptrvalue == NULL)
  {
    return;
  }
  
  anticodon_loc = (SeqLocPtr) userdata;

  rrp = (RnaRefPtr) sfp->data.value.ptrvalue;

  if (rrp->ext.choice == 2) 
  {
    trp = (tRNAPtr) rrp->ext.value.ptrvalue;
    if (trp != NULL) 
    {
      trp->anticodon = SeqLocFree (trp->anticodon);
      if (anticodon_loc != NULL)
      {
        slp = (SeqLocPtr) AsnIoMemCopy (anticodon_loc,
                                        (AsnReadFunc) SeqLocAsnRead,
                                        (AsnWriteFunc) SeqLocAsnWrite);

        slp = SeqLocReplaceID (slp, SeqLocId (sfp->location));
        trp->anticodon = slp;
      }
    }
  }
}

static Boolean AcceptRNAApply (ApplyEditConvertRemovePtr ap)
{
  ApplyRNAQualPtr rp;
  FilterSetPtr    fsp;
  ApplyValueData  avd;
  Boolean         rval = TRUE;
  SeqEntryPtr     sep;
  ValNodePtr      vnp;
  GetSamplePtr    gsp, gsp_new, gsp_sum;
  
  if (ap == NULL)
  {
    return FALSE;
  }
  
  rp = DialogToPointer (ap->aecr_pages[AECR_APPLY - 1]);
  
  sep = GetTopSeqEntryForEntityID (ap->input_entityID);
  if (rp->subtype_list == NULL || rp->field_list == NULL)
  {
    rp = ApplyRNAQualFree (rp);
    return FALSE;
  }
  
  fsp = (FilterSetPtr) DialogToPointer (ap->constraints);
  
  if (rp->field_list->data.intvalue == RNAFIELD_CODONS_RECOGNIZED)
  {
    for (vnp = rp->subtype_list; vnp != NULL; vnp = vnp->next)
    {
      OperateOnSeqEntryConstrainedObjects (sep, fsp, ApplyRNACodonsCallback,
                                           NULL, SEQFEAT_RNA, FEATDEF_tRNA, 0, rp->codons);  
    }
  }
  else if (rp->field_list->data.intvalue == RNAFIELD_ANTICODON)
  {
    for (vnp = rp->subtype_list; vnp != NULL; vnp = vnp->next)
    {
      OperateOnSeqEntryConstrainedObjects (sep, fsp, ApplyRNAAnticodonCallback,
                                           NULL, SEQFEAT_RNA, FEATDEF_tRNA, 0, rp->anticodon);  
    }
  }
  else
  {
    avd.field_list = rp->field_list;
    
    /* get handling for existing text */
    gsp_sum = NULL;
    for (vnp = rp->subtype_list; vnp != NULL; vnp = vnp->next)
    {
      gsp = CheckForExistingText (ap->input_entityID,
                                  avd.field_list,
                                  GetRNAFieldString,
                                  NULL, NULL, IntValNodeCopy,
                                  fsp, SEQFEAT_RNA, vnp->data.intvalue, 0);
      gsp_new = GetSampleAdd (gsp_sum, gsp);
      gsp = GetSampleFree (gsp);
      gsp_sum = GetSampleFree (gsp_sum);
      gsp_sum = gsp_new;
    }
    avd.etp = GetExistingTextHandlerInfo (gsp_sum, FALSE);
    gsp_sum = GetSampleFree (gsp_sum);
  
    if (avd.etp != NULL 
        && avd.etp->existing_text_choice == EXISTING_TEXT_CHOICE_CANCEL)
    {
      rval = FALSE;
    }

    if (rval)
    {
      AddEditApplyDataToApplyValue (AECR_APPLY, rp->edit_apply, &avd);
     
      for (vnp = rp->subtype_list; vnp != NULL; vnp = vnp->next)
      {
        OperateOnSeqEntryConstrainedObjects (sep, fsp, SetRNAFieldString,
                                             NULL, SEQFEAT_RNA, vnp->data.intvalue, 0, &avd);  
      }
      avd.text_to_replace = MemFree (avd.text_to_replace);
      avd.new_text = MemFree (avd.new_text);
    }
    avd.etp = MemFree (avd.etp);
  }
  
  FilterSetFree (fsp);
  
  return rval;
}

static Boolean 
AcceptRNAQualEditRemoveConvert 
(ApplyEditConvertRemovePtr ap,
 Int4                      action_choice,
 SimpleAECRPtr             rp)
{
  SeqEntryPtr      sep;
  FilterSetPtr     fsp;
  ConvertFieldData cfd;
  GetSamplePtr     gsp, gsp_new, gsp_sum;
  ValNodePtr       vnp;
  ApplyValueData   avd;
  Boolean          rval = TRUE;
  
  if (ap == NULL
      || action_choice < AECR_EDIT 
      || action_choice > NUM_AECR
      || rp == NULL
      || rp->subtype_list == NULL || rp->field_list == NULL
      || ((action_choice == AECR_CONVERT || action_choice == AECR_SWAP) 
           && rp->field_list_to == NULL))
  {
    return FALSE;
  }

  sep = GetTopSeqEntryForEntityID (ap->input_entityID);

  fsp = (FilterSetPtr) DialogToPointer (ap->constraints);
  
  if (action_choice == AECR_CONVERT || action_choice == AECR_SWAP)
  {
    cfd.src_field_list = rp->field_list;
    cfd.dst_field_list = rp->field_list_to;
    cfd.etp = NULL;
    cfd.get_str_func = GetRNAFieldString;
    cfd.set_str_func = SetRNAFieldString;
    cfd.remove_str_func = RemoveRNAField;
    cfd.get_d_str_func = NULL;
    cfd.set_d_str_func = NULL;
    cfd.remove_d_str_func = NULL;
    cfd.fsp = fsp;
    if (action_choice == AECR_CONVERT)
    {
      if (rp->leave_on_original)
      {
        cfd.convert_type = CONVERT_TYPE_COPY;
      }
      else
      {
        cfd.convert_type = CONVERT_TYPE_MOVE;
      }
    }
    else if (action_choice == AECR_SWAP)
    {
      cfd.convert_type = CONVERT_TYPE_SWAP;
    }
    
    if (cfd.convert_type == CONVERT_TYPE_MOVE || cfd.convert_type == CONVERT_TYPE_COPY)
    {
      /* get existing text sample */
      gsp_sum = NULL;
      for (vnp = rp->subtype_list; vnp != NULL; vnp = vnp->next)
      {
        gsp = CheckForConversionExistingTextInSeqEntry (sep, &cfd, fsp,
                                                        SEQFEAT_RNA,
                                                        vnp->data.intvalue,
                                                        0);
        gsp_new = GetSampleAdd (gsp_sum, gsp);
        gsp = GetSampleFree (gsp);
        gsp_sum = GetSampleFree (gsp_sum);
        gsp_sum = gsp_new;
      }
      cfd.etp = GetExistingTextHandlerInfo (gsp_sum, FALSE);
      gsp_sum = GetSampleFree (gsp_sum);
      if (cfd.etp != NULL && cfd.etp->existing_text_choice == EXISTING_TEXT_CHOICE_CANCEL)
      {
        rval = FALSE;
      }
    }
    else
    {
      cfd.etp = NULL;
    }  

    if (rval)
    {
      for (vnp = rp->subtype_list; vnp != NULL; vnp = vnp->next)
      {
        OperateOnSeqEntryConstrainedObjects (sep, fsp, 
                                             ConvertFeatureFieldCallback, NULL,
                                             SEQFEAT_RNA, vnp->data.intvalue, 
                                             0, &cfd);
      }
    }
  }
  else
  {
    avd.field_list = rp->field_list;
    avd.etp = NULL;
    AddEditApplyDataToApplyValue (action_choice, rp->edit_apply, &avd);
     
    for (vnp = rp->subtype_list; vnp != NULL; vnp = vnp->next)
    {
      if (action_choice == AECR_EDIT)
      {
        OperateOnSeqEntryConstrainedObjects (sep, fsp, SetRNAFieldString,
                                             NULL, SEQFEAT_RNA, vnp->data.intvalue, 0, &avd);  
      }
      else if (action_choice == AECR_REMOVE)
      {
        OperateOnSeqEntryConstrainedObjects (sep, fsp, RemoveRNAField,
                                             NULL, SEQFEAT_RNA, vnp->data.intvalue, 0, &avd);  
      }
    }
    avd.text_to_replace = MemFree (avd.text_to_replace);
    avd.new_text = MemFree (avd.new_text);
  }

  FilterSetFree (fsp);
  return rval;

}

static Boolean AcceptRNAQualApplyEditRemoveConvert (Pointer userdata)
{
  ApplyEditConvertRemovePtr ap;
  SimpleAECRPtr             rp = NULL;
  Int4                      action_choice;
  Boolean                   rval = TRUE;

  ap = (ApplyEditConvertRemovePtr) userdata;
  if (ap == NULL)
  {
    return FALSE;
  }
  
  WatchCursor ();
  Update ();
  
  if (ap->crippled)
  {
    action_choice = ap->crippled_action;
  }
  else
  {
    action_choice = GetValue (ap->action_popup);
  }
  
  if (action_choice == AECR_APPLY)
  {
    rval = AcceptRNAApply (ap);
  }
  else if (action_choice > AECR_APPLY && action_choice <= NUM_AECR)
  {
    rp = (SimpleAECRPtr) DialogToPointer (ap->aecr_pages[action_choice - 1]);
    rval = AcceptRNAQualEditRemoveConvert (ap, action_choice, rp);
    rp = SimpleAECRFree (rp);
  }
    
  if (rval)
  {
    ObjMgrSetDirtyFlag (ap->input_entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, ap->input_entityID, 0, 0);
  }
  ArrowCursor ();
  Update (); 
  
  return rval;
}

static void 
RNAQualApplyEditRemoveConvert 
(IteM    i, 
 Int4    first_action_choice,
 Boolean crippled)
{
  ApplyEditConvertRemoveCombo (i, first_action_choice, crippled,
                               "RNA Qualifiers",
                               RNAQualListDialog, TRUE, GetRNAQualSample,
                               TRUE, FALSE, TRUE, FALSE, "Where feature text",
                               AcceptRNAQualApplyEditRemoveConvert, NULL, NULL);
}

extern void ApplyRNAQual (IteM i)
{
  RNAQualApplyEditRemoveConvert (i, AECR_APPLY, FALSE);
}

extern void PublicApplyRNAQual (IteM i)
{
  RNAQualApplyEditRemoveConvert (i, AECR_APPLY, TRUE);
}

extern void EditRNAQual (IteM i)
{
  RNAQualApplyEditRemoveConvert (i, AECR_EDIT, FALSE);
}

extern void PublicEditRNAQual (IteM i)
{
  RNAQualApplyEditRemoveConvert (i, AECR_EDIT, TRUE);
}

extern void ConvertRNAQual (IteM i)
{
  RNAQualApplyEditRemoveConvert (i, AECR_CONVERT, FALSE);
}

extern void SwapRNAQual (IteM i)
{
  RNAQualApplyEditRemoveConvert (i, AECR_SWAP, FALSE);
}

extern void RemoveRNAQual (IteM i)
{
  RNAQualApplyEditRemoveConvert (i, AECR_REMOVE, FALSE);
}

static DialoG 
GBQualListDialog
(GrouP                    h,
 Int4                     action_choice,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata,
 Uint2                    entityID)
{
  return SimpleAECRDialog (h, action_choice, change_notify, change_userdata,
                           NULL, ValNodeSimpleDataFree,
                           GBQualSelectionDialog, FeatureSelectionDialog, NULL,
                           "Qualifier", "Feature", TRUE, entityID);
}

static GetSamplePtr 
GetGBQualExistingText 
(SeqEntryPtr  sep,
 ValNodePtr   feature_type_list,
 ValNodePtr   requested_field,
 FilterSetPtr fsp)
{
  ValNodePtr   vnp;
  GetSamplePtr gsp;
  
  if (sep == NULL || feature_type_list == NULL)
  {
    return NULL;
  }
  
  gsp = (GetSamplePtr) MemNew (sizeof (GetSampleData));
  if (gsp == NULL)
  {
    return NULL;
  }
  gsp->sample_text = NULL;
  gsp->fieldstring_func = GetGBQualString;
  gsp->descrstring_func = NULL;
  gsp->free_vn_proc = NULL;
  gsp->copy_vn_proc = IntValNodeCopy;
    
  gsp->requested_field = (gsp->copy_vn_proc) (requested_field);
  gsp->num_found = 0;
  gsp->all_same = TRUE;
  for (vnp = feature_type_list; vnp != NULL; vnp = vnp->next)
  {
    OperateOnSeqEntryConstrainedObjects (sep, fsp, GetSampleFeatureCallback, 
                                         NULL,
                                         0, vnp->choice, 0, gsp);
  }
  return gsp;  
}


static Boolean AcceptGBQualApplyEditRemoveConvert (Pointer userdata)
{
  ApplyEditConvertRemovePtr ap;
  SimpleAECRPtr             gp;
  Int4                      action_choice;
  SeqEntryPtr               sep;
  ApplyValueData            avd;
  ConvertFieldData          cfd;
  FilterSetPtr              fsp;
  GetSamplePtr              gsp = NULL, gsp_new, gsp_sum;
  ValNodePtr                vnp, qual_vnp;
  Boolean                   rval = TRUE;

  ap = (ApplyEditConvertRemovePtr) userdata;
  if (ap == NULL)
  {
    return FALSE;
  }
  
  WatchCursor ();
  Update ();
  
  if (ap->crippled)
  {
    action_choice = ap->crippled_action;
  }
  else
  {
    action_choice = GetValue (ap->action_popup);
  }
  if (action_choice < 1 || action_choice > NUM_AECR)
  {
    return FALSE;
  }
  gp = (SimpleAECRPtr) DialogToPointer (ap->aecr_pages[action_choice - 1]);

  sep = GetTopSeqEntryForEntityID (ap->input_entityID);
  if (gp->subtype_list == NULL || gp->field_list == NULL
      || ((action_choice == AECR_CONVERT || action_choice == AECR_SWAP) 
           && gp->field_list_to == NULL))
  {
    gp = SimpleAECRFree (gp);
    ArrowCursor ();
    Update ();
    return FALSE;
  }

  fsp = (FilterSetPtr) DialogToPointer (ap->constraints);
  
  if (action_choice == AECR_CONVERT || action_choice == AECR_SWAP)
  {
    cfd.src_field_list = gp->field_list;
    cfd.dst_field_list = gp->field_list_to;
    cfd.etp = NULL;
    cfd.get_str_func = GetGBQualString;
    cfd.set_str_func = SetGBQualString;
    cfd.remove_str_func = RemoveGBQualField;
    cfd.get_d_str_func = NULL;
    cfd.set_d_str_func = NULL;
    cfd.remove_d_str_func = NULL;
    cfd.fsp = fsp;
    
    if (action_choice == AECR_CONVERT)
    {
      if (gp->leave_on_original)
      {
        cfd.convert_type = CONVERT_TYPE_COPY;
      }
      else
      {
        cfd.convert_type = CONVERT_TYPE_MOVE;
      }
    }
    else if (action_choice == AECR_SWAP)
    {
      cfd.convert_type = CONVERT_TYPE_SWAP;
    }
    
    if (cfd.convert_type == CONVERT_TYPE_MOVE || cfd.convert_type == CONVERT_TYPE_COPY)
    {
      /* get existing text data */
      gsp_sum = NULL;
      for (vnp = gp->subtype_list; vnp != NULL; vnp = vnp->next)
      {
        gsp = CheckForConversionExistingTextInSeqEntry (sep, &cfd, fsp,
                                                        0,
                                                        vnp->choice,
                                                        0);
        gsp_new = GetSampleAdd (gsp_sum, gsp);
        gsp = GetSampleFree (gsp);
        gsp_sum = GetSampleFree (gsp_sum);
        gsp_sum = gsp_new;
      }
      cfd.etp = GetExistingTextHandlerInfo (gsp_sum, FALSE);
      gsp_sum = GetSampleFree (gsp_sum);
      if (cfd.etp != NULL && cfd.etp->existing_text_choice == EXISTING_TEXT_CHOICE_CANCEL)
      {
        rval = FALSE;
      }
    }
    else
    {
      cfd.etp = NULL;
    }        
    
    if (rval)
    {
      for (vnp = gp->subtype_list; vnp != NULL; vnp = vnp->next)
      {
        OperateOnSeqEntryConstrainedObjects (sep, fsp, ConvertFeatureFieldCallback,
                                             NULL, 0, vnp->choice, 0, &cfd);  
      }
    }
    cfd.etp = MemFree (cfd.etp);
  }
  else
  {
    avd.field_list = gp->field_list;
  
    /* get handling for existing text */
    if (action_choice == AECR_APPLY)
    {
      gsp = GetGBQualExistingText (sep, gp->subtype_list,
                                   avd.field_list,
                                   fsp);
      avd.etp = GetExistingTextHandlerInfo (gsp, FALSE);
      gsp = GetSampleFree (gsp);
    
      if (avd.etp != NULL 
        && avd.etp->existing_text_choice == EXISTING_TEXT_CHOICE_CANCEL)
      {
        rval = FALSE;
      }
    }
    else
    {
      avd.etp = NULL;
    }
  
    AddEditApplyDataToApplyValue (action_choice, gp->edit_apply, &avd);
        
    for (vnp = gp->subtype_list; vnp != NULL; vnp = vnp->next)
    {
      if (action_choice == AECR_EDIT || action_choice == AECR_APPLY)
      {
        OperateOnSeqEntryConstrainedObjects (sep, fsp, SetGBQualString,
                                             NULL, 0, vnp->choice, 0, &avd);  
      }
      else if (action_choice == AECR_REMOVE)
      {
        for (qual_vnp = gp->field_list; qual_vnp != NULL; qual_vnp = qual_vnp->next)
        {
          avd.field_list = qual_vnp;
          OperateOnSeqEntryConstrainedObjects (sep, fsp, RemoveGBQualField,
                                               NULL, 0, vnp->choice, 0, &avd);  
        }
      }
    }

    avd.text_to_replace = MemFree (avd.text_to_replace);
    avd.new_text = MemFree (avd.new_text);
  }
  FilterSetFree (fsp);
  gp = SimpleAECRFree (gp);
  
  if (rval)
  {
    ObjMgrSetDirtyFlag (ap->input_entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, ap->input_entityID, 0, 0);
  }
  ArrowCursor ();
  Update (); 
  
  return rval;
}

static CharPtr GetGBQualName (ValNodePtr vnp)
{
  Int4 field_choice;
  if (vnp != NULL 
      && vnp->data.intvalue > 0 
      && vnp->data.intvalue <= NumEditQualifiers)
  {
    return StringSave (EditQualifierList [vnp->data.intvalue - 1].name);
  }
  else
  {
    field_choice = vnp->data.intvalue;
    return NULL;
  }
}

static CharPtr GetGBQualSampleName (ValNodePtr vnp)
{
  CharPtr    label = NULL;
  Int2       feature_subtype, gb_field_choice;
  ValNodePtr feature_list, f_vnp;
  Int4       num_feature_choices;
  CharPtr    feature_label = "";
  
  if (vnp == NULL)
  {
    return NULL;
  }
  
  feature_list = BuildFeatureDialogList (FALSE);
  num_feature_choices = ValNodeLen (feature_list);
  
  feature_subtype = (vnp->data.intvalue - 1) / NumEditQualifiers;
  gb_field_choice = (vnp->data.intvalue - 1) % NumEditQualifiers;
  
  for (f_vnp = feature_list; f_vnp != NULL; f_vnp = f_vnp->next)
  {
    if (f_vnp->choice == feature_subtype)
    {
      feature_label = f_vnp->data.ptrvalue;
    }
  }
  
  
  label = (CharPtr) MemNew ((StringLen (feature_label)
                            + StringLen (EditQualifierList[gb_field_choice].name)
                            + 2) * sizeof (Char));
  if (label != NULL)
  {
    sprintf (label, "%s %s", feature_label,
             EditQualifierList[gb_field_choice].name);
  }
  ValNodeFreeData (feature_list);
  return label;
}

static CharPtr GetGBQualSampleFieldString (SeqFeatPtr sfp, ValNodePtr vnp, FilterSetPtr fsp)
{
  CharPtr   str = NULL;
  Int2      feature_subtype, gb_field_choice;
  ValNode   vn;
  
  if (sfp == NULL || vnp == NULL)
  {
    return NULL;
  }

  feature_subtype = (vnp->data.intvalue - 1) / NumEditQualifiers;
  gb_field_choice = (vnp->data.intvalue - 1) % NumEditQualifiers;
    
  if (feature_subtype != sfp->idx.subtype)
  {
    return NULL;
  }

  vn.next = NULL;
  vn.choice = 0;
  vn.data.intvalue = gb_field_choice + 1;
  str = GetGBQualString (sfp, &vn, NULL);
  
  return str;
}

static DialoG GetGBQualSample (GrouP h, Uint2 entityID)
{
  DialoG        d;
  SetSampleData ssd;
  Int4          qual_choice, list_index;  
  ValNodePtr    feature_list, f_vnp;    

  d = SampleDialog (h);
  ssd.fieldstring_func = GetGBQualSampleFieldString;
  ssd.descrstring_func = NULL;
  ssd.entityID = entityID;
  ssd.free_vn_proc = NULL;
  ssd.copy_vn_proc = IntValNodeCopy;
  ssd.match_vn_proc = IntValNodeMatch;
  ssd.label_vn_proc = GetGBQualSampleName;
  ssd.fsp = NULL;
 
  /* construct list of Features and GBQualifiers */
  ssd.field_list = NULL;

  feature_list = BuildFeatureDialogList (FALSE);
  
  for (f_vnp = feature_list; f_vnp != NULL; f_vnp = f_vnp->next)
  {
    for (qual_choice = 0; qual_choice < NumEditQualifiers; qual_choice++)
    {
      list_index = f_vnp->choice * NumEditQualifiers
                   + qual_choice;
      ValNodeAddInt (&ssd.field_list, 0, list_index + 1);    
    }
  }

  PointerToDialog (d, &ssd);
  
  /* now free up field list (SampleDialog maintains its own copy */
  ssd.field_list = ValNodeFree (ssd.field_list);
  feature_list = ValNodeFreeData (feature_list);
  return d;
}

static void 
GBQualApplyEditRemoveConvert 
(IteM    i,
 Int4    first_action_choice,
 Boolean crippled)
{
  ApplyEditConvertRemoveCombo (i, first_action_choice, crippled,
                               "Import Qualifiers",
                               GBQualListDialog, FALSE, GetGBQualSample,
                               TRUE, FALSE, TRUE, FALSE, "Where feature text",
                               AcceptGBQualApplyEditRemoveConvert, NULL, NULL);
}

extern void ApplyGBQual (IteM i)
{
  GBQualApplyEditRemoveConvert (i, AECR_APPLY, FALSE);
}

extern void PublicApplyGBQual (IteM i)
{
  GBQualApplyEditRemoveConvert (i, AECR_APPLY, TRUE);
}

extern void EditGBQual (IteM i)
{
  GBQualApplyEditRemoveConvert (i, AECR_EDIT, FALSE);
}

extern void PublicEditGBQual (IteM i)
{
  GBQualApplyEditRemoveConvert (i, AECR_EDIT, TRUE);
}

extern void ConvertGBQual (IteM i)
{
  GBQualApplyEditRemoveConvert (i, AECR_CONVERT, FALSE);
}

extern void SwapGBQual (IteM i)
{
  GBQualApplyEditRemoveConvert (i, AECR_SWAP, FALSE);
}

extern void RemoveGBQual (IteM i)
{
  GBQualApplyEditRemoveConvert (i, AECR_REMOVE, FALSE);
}

typedef struct locustagtooldlocustag 
{
  FORM_MESSAGE_BLOCK
  DialoG         constraints;
  DialoG         accept_cancel;
  
  SeqEntryPtr    sep;
} LocusTagToOldLocusTagData, PNTR LocusTagToOldLocusTagPtr;

static void LocusTagToOldLocusTagActionClear (Pointer userdata)
{
  LocusTagToOldLocusTagPtr mp;
  
  mp = (LocusTagToOldLocusTagPtr) userdata;
  if (mp == NULL)
  {
    return;
  }
  PointerToDialog (mp->constraints, NULL);
}

static void LocusTagToOldLocusTagActionClearText (Pointer userdata)
{
  LocusTagToOldLocusTagPtr mp;
  FilterSetPtr             fsp;
  
  mp = (LocusTagToOldLocusTagPtr) userdata;
  if (mp == NULL)
  {
    return;
  }
  fsp = (FilterSetPtr) DialogToPointer (mp->constraints);
  FilterSetClearText (fsp);
  PointerToDialog (mp->constraints, fsp);
  FilterSetFree (fsp);
}


static void 
ConvertGeneLocusTagToOldLocusTagCallback 
(SeqFeatPtr   gene,
 Pointer      userdata, 
 FilterSetPtr fsp)
{
  GeneRefPtr               grp;
  CharPtr                  locus_tag;
  GBQualPtr                new_qual, prev_qual;
  LocusTagToOldLocusTagPtr mp;
  
  if (gene == NULL || gene->data.choice != SEQFEAT_GENE || userdata == NULL) return;
  
  mp = (LocusTagToOldLocusTagPtr)userdata;
  
  grp = (GeneRefPtr) gene->data.value.ptrvalue;
  if (grp == NULL || StringHasNoText (grp->locus_tag))
  {
  	return;
  }
  locus_tag = grp->locus_tag;
  grp->locus_tag = NULL;
  
  new_qual = GBQualNew ();
  if (new_qual == NULL) return;
  new_qual->qual = StringSave ("old_locus_tag");
  new_qual->val = locus_tag;
  new_qual->next = NULL;
  
  if (gene->qual == NULL)
  {
  	gene->qual = new_qual;
  }
  else
  {
  	for (prev_qual = gene->qual; prev_qual->next != NULL; prev_qual = prev_qual->next)
  	{
  	}
  	prev_qual->next = new_qual;
  }
}

static Boolean LocusTagToOldLocusTagAction (Pointer userdata)
{
  LocusTagToOldLocusTagPtr mp;
  FilterSetPtr             fsp;
  
  mp = (LocusTagToOldLocusTagPtr) userdata;
  if (mp == NULL)
  {
    return FALSE;
  }
  
  WatchCursor ();
  Update ();
  
  mp->sep = GetTopSeqEntryForEntityID (mp->input_entityID);
  fsp = (FilterSetPtr) DialogToPointer (mp->constraints);
  OperateOnSeqEntryConstrainedObjects (mp->sep, fsp, ConvertGeneLocusTagToOldLocusTagCallback, 
                                       NULL, SEQFEAT_GENE, 0, 0, mp);
  ObjMgrSetDirtyFlag (mp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, mp->input_entityID, 0, 0);
  ArrowCursor ();
  Update ();  
  return TRUE;
}

extern void ConvertLocusTagToOldLocusTag (IteM i)
{
  BaseFormPtr         bfp;
  LocusTagToOldLocusTagPtr     mp;
  WindoW              w;
  GrouP               h;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  mp = (LocusTagToOldLocusTagPtr) MemNew (sizeof (LocusTagToOldLocusTagData));
  if (mp == NULL) return;
  
  w = FixedWindow (-50, -33, -10, -10, "Convert Locus Tag to Old Locus Tag", StdCloseWindowProc); 
  SetObjectExtra (w, mp, StdCleanupExtraProc); 
  SetObjectExtra (w, mp, NULL);
  mp->form = (ForM) w;
  mp->input_entityID = bfp->input_entityID;
  
  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);
  
  mp->constraints = FilterGroup (h, TRUE, FALSE, TRUE, FALSE, "Where feature text"); 
  mp->accept_cancel = AcceptCancelDialog (h, LocusTagToOldLocusTagAction, NULL, 
                                          LocusTagToOldLocusTagActionClear,
                                          LocusTagToOldLocusTagActionClearText,
                                          (Pointer)mp, w);  
  AlignObjects (ALIGN_CENTER, (HANDLE) mp->constraints,
                              (HANDLE) mp->accept_cancel,
                              NULL);
  Show (w);   
}

extern LogInfoPtr OpenLog (CharPtr display_title)
{
  LogInfoPtr lip;
  
  lip = (LogInfoPtr) MemNew (sizeof (LogInfoData));
  if (lip == NULL)
  {
    return NULL;
  }
  TmpNam (lip->path);
  lip->fp = FileOpen (lip->path, "w");
  lip->data_in_log = FALSE;
  lip->display_title = StringSave (display_title);
  return lip;
}

extern void CloseLog (LogInfoPtr lip)
{
  if (lip == NULL || lip->fp == NULL)
  {
    return;
  }
  FileClose (lip->fp);
  lip->fp = NULL;
  if (lip->data_in_log)
  {
    LaunchGeneralTextViewer (lip->path, lip->display_title);
  }
  FileRemove (lip->path);  
}

extern LogInfoPtr FreeLog (LogInfoPtr lip)
{
  if (lip != NULL)
  {
    lip->display_title = MemFree (lip->display_title);
    if (lip->fp != NULL)
    {
      FileClose (lip->fp);
      lip->fp = NULL;
    }
    lip = MemFree (lip);
  }
  return lip;
}

static CharPtr GetLastLineageFromString (CharPtr lineage)
{
  CharPtr last_semicolon, lineage_start, penultimate_semicolon;
  
  
  if (StringHasNoText (lineage))
  {
    return lineage;
  }
  
  last_semicolon = StringRChr (lineage, ';');
  if (last_semicolon == NULL)
  {
    return lineage;
  }
  
  /* skip past semicolon */
  lineage_start = last_semicolon + 1;

  /* skip over whitespace */
  lineage_start += StringSpn (lineage_start, " \t");
  
  if (StringICmp (lineage_start, "environmental samples") == 0 
      && last_semicolon != lineage)
  {
    *last_semicolon = 0;
    penultimate_semicolon = StringRChr (lineage, ';');
    if (penultimate_semicolon == NULL)
    {
      lineage_start = lineage;
    }
    else
    {
      lineage_start = penultimate_semicolon + 1;
    }
    lineage_start += StringSpn (lineage_start, " \t");
  }
  return lineage_start;
}

static void ExportLastLineageCallback (BioseqPtr bsp, Pointer userdata)
{
  LogInfoPtr        lip;
  SeqFeatPtr        sfp;
  SeqMgrFeatContext fcontext;
  SeqDescrPtr       sdp;
  SeqMgrDescContext dcontext;
  SeqIdPtr          sip;
  Char              tmp[128];
  CharPtr           val_str = NULL;
  ValNode           lineage_field;
  CharPtr           last_lineage;
  Boolean           found_text = FALSE;
  
  if (bsp == NULL || userdata == NULL)
  {
    return;
  }
  
  lip = (LogInfoPtr) userdata;
  
  sip = SeqIdFindBest (bsp->id, 0);
  SeqIdWrite (sip, tmp, PRINTID_REPORT, sizeof (tmp));
  
  fprintf (lip->fp, "%s", tmp);
  
  lineage_field.next = NULL;
  lineage_field.choice = 1;
  lineage_field.data.ptrvalue = "Lineage";

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_source, &dcontext);
  while (sdp != NULL)
  {
    val_str = GetSourceQualDescrString (sdp, &lineage_field, NULL);
    last_lineage = GetLastLineageFromString (val_str);
    if (last_lineage != NULL)
    {
      if (found_text)
      {
        fprintf (lip->fp, ";%s", last_lineage);
      }
      else
      {
        fprintf (lip->fp, "\t%s", last_lineage);
      }
      val_str = MemFree (val_str);
      found_text = TRUE;
    }
    sdp = SeqMgrGetNextDescriptor (bsp, sdp, 0, &dcontext);
  }

  sfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_BIOSRC, 0, &fcontext);
  while (sfp != NULL)
  {
    val_str = GetSourceQualFeatureString (sfp, &lineage_field, NULL);
    last_lineage = GetLastLineageFromString (val_str);
    if (last_lineage != NULL)
    {
      if (found_text)
      {
        fprintf (lip->fp, ";%s", last_lineage);
      }
      else
      {
        fprintf (lip->fp, "\t%s", last_lineage);
      }
      val_str = MemFree (val_str);
      found_text = TRUE;
    }
    sfp = SeqMgrGetNextFeature (bsp, sfp, SEQFEAT_BIOSRC, 0, &fcontext);
  }
  fprintf (lip->fp, "\n");
  lip->data_in_log = TRUE;
}

extern void ExportLastLineage (IteM i)
{
  BaseFormPtr         bfp;
  SeqEntryPtr         sep;
  LogInfoPtr          lip;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  
  lip = OpenLog ("Lineage");
  WatchCursor ();
  Update ();
  VisitBioseqsInSep (sep, lip, ExportLastLineageCallback);
  CloseLog (lip);
  lip = FreeLog (lip);    
  ArrowCursor ();
  Update ();
}
