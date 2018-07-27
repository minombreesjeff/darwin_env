/*   sequin4.c
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
* File Name:  sequin4.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   6/28/96
*
* $Revision: 6.210 $
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

#include "sequin.h"
#include <ncbilang.h>
#include <seqport.h>
#include <gather.h>
#include <objall.h>
#include <objcode.h>
#include <utilpub.h>
#include <vibrant.h>
#include <document.h>
#include <toasn3.h>
#include <asn2ffp.h>
#include <salfiles.h>
#include <salsap.h>
#include <saledit.h>
#include <salign.h>
#include <salptool.h>
#include <subutil.h>
#include <pobutil.h>
#include <tfuns.h>
#include <edutil.h>
#include <biosrc.h>
#include <seqgrphx.h>
#include <seqmtrx.h>
#include <bspview.h>
#include <vsmpriv.h>
#include <explore.h>
#include <alignval.h>
#include <alignmgr.h>
#include <alignmgr2.h>
#include <aliparse.h>
#include <spidey.h>
#include <ent2api.h>

#define REGISTER_UPDATESEGSET ObjMgrProcLoadEx (OMPROC_FILTER,"Update Segmented Set","UpdateSegSet",0,0,0,0,NULL,UpdateSegSet,PROC_PRIORITY_DEFAULT, "Indexer")

#define REGISTER_ADJUSTMULTISEGSEQ ObjMgrProcLoadEx (OMPROC_FILTER,"Adjust SegSeq Length","AdjustSegLength",0,0,0,0,NULL,AdjustSegSeqLength,PROC_PRIORITY_DEFAULT, "Indexer")

#define REGISTER_UNDOSEGSET ObjMgrProcLoadEx (OMPROC_FILTER,"Undo Segmented Set","UndoSegSet",0,0,0,0,NULL,UndoSegSet,PROC_PRIORITY_DEFAULT, "Indexer")

#define REGISTER_REPACKAGE_PARTS ObjMgrProcLoadEx (OMPROC_FILTER,"Repackage Segmented Parts","RepackageParts",0,0,0,0,NULL,PackagePartsInPartsSet,PROC_PRIORITY_DEFAULT, "Indexer")

#define REGISTER_NORMALIZE_NUCPROT ObjMgrProcLoadEx (OMPROC_FILTER,"Normalize Nuc-Prot","NormalizeNucProts",0,0,0,0,NULL,NormalizeNucProts,PROC_PRIORITY_DEFAULT, "Indexer")

#define REGISTER_REMOVE_EXTRANEOUS ObjMgrProcLoadEx (OMPROC_FILTER,"Remove Extraneous Sets","RemoveExtraneousSets",0,0,0,0,NULL,RemoveExtraneousSets,PROC_PRIORITY_DEFAULT, "Indexer")

#define REGISTER_POPSET_WITHIN_GENBANK ObjMgrProcLoadEx (OMPROC_FILTER,"Add Popset Within GenBank Set","AddPopSetWithinGenBankSet",0,0,0,0,NULL,PopWithinGenBankSet,PROC_PRIORITY_DEFAULT, "Indexer")

#define REGISTER_REMOVE_MESSEDUP ObjMgrProcLoadEx (OMPROC_FILTER,"Repair Messed Up Sets","RepairMessedUpSets",0,0,0,0,NULL,RepairMessedUpRecord,PROC_PRIORITY_DEFAULT, "Indexer")

#define REGISTER_UPDATE_SEQALIGN ObjMgrProcLoadEx (OMPROC_FILTER, "Update SeqAlign","UpdateSeqAlign",OBJ_SEQALIGN,0,OBJ_SEQALIGN,0,NULL,NewUpdateSeqAlign,PROC_PRIORITY_DEFAULT, "Indexer")

#define REGISTER_DELETE_BY_TEXT ObjMgrProcLoadEx (OMPROC_FILTER, "Delete By Text","DeleteByText",0,0,0,0,NULL,CreateDeleteByTextWindow,PROC_PRIORITY_DEFAULT, "Indexer")

#define REGISTER_SEGREGATE_BY_TEXT ObjMgrProcLoadEx (OMPROC_FILTER, "Segregate By Text","SegregateByText",0,0,0,0,NULL,CreateSegregateByTextWindow,PROC_PRIORITY_DEFAULT, "Indexer")

#define REGISTER_CONVERTSEQALIGN ObjMgrProcLoadEx (OMPROC_FILTER,"Convert SeqAlign","ConvertSeqAlign",0,0,0,0,NULL,ConvertToTrueMultipleAlignment,PROC_PRIORITY_DEFAULT, "Alignment")

#define REGISTER_MAKESEQALIGN ObjMgrProcLoadEx (OMPROC_FILTER,"Make SeqAlign","CreateSeqAlign",0,0,0,0,NULL,GenerateSeqAlignFromSeqEntry,PROC_PRIORITY_DEFAULT, "Alignment")

#define REGISTER_MAKESEQALIGNP ObjMgrProcLoadEx (OMPROC_FILTER,"Make Protein SeqAlign","CreateSeqAlignProt",0,0,0,0,NULL,GenerateSeqAlignFromSeqEntryProt,PROC_PRIORITY_DEFAULT, "Alignment")

#define REGISTER_MAKESEQALIGND ObjMgrProcLoadEx (OMPROC_FILTER,"Make Discontinuous SeqAlign","CreateSeqAlignDisc",0,0,0,0,NULL,GenerateSeqAlignDiscFromSeqEntry,PROC_PRIORITY_DEFAULT, "Alignment")

#define REGISTER_NORMSEQALIGN ObjMgrProcLoadEx (OMPROC_FILTER,"Validate SeqAlign","ValidateSeqAlign",0,0,0,0,NULL,ValidateSeqAlignFromData,PROC_PRIORITY_DEFAULT, "Alignment")

#define REGISTER_NOMORESEGGAP ObjMgrProcLoadEx (OMPROC_FILTER,"Get Rid of Seg Gap","GetRidOfSegGap",0,0,0,0,NULL,NoMoreSegGap,PROC_PRIORITY_DEFAULT, "Alignment")

#define REGISTER_PARTSEQALIGNTOPARENT ObjMgrProcLoadEx (OMPROC_FILTER,"Part SeqAlign to Parent","PartSeqAlignToParent",0,0,0,0,NULL,PartSeqAlignToParent,PROC_PRIORITY_DEFAULT, "Alignment")

#define REGISTER_GROUP_EXPLODE ObjMgrProcLoadEx (OMPROC_FILTER, "Explode a group", "GroupExplode", OBJ_SEQFEAT, 0, OBJ_SEQFEAT, 0, NULL, GroupExplodeFunc, PROC_PRIORITY_DEFAULT, "Indexer")

#define REGISTER_INTERVAL_COMBINE ObjMgrProcLoadEx (OMPROC_FILTER, "Combine feature intervals", "IntervalCombine", OBJ_SEQFEAT, 0, OBJ_SEQFEAT, 0, NULL, IntervalCombineFunc, PROC_PRIORITY_DEFAULT, "Indexer")

#define REGISTER_MRNA_FROM_CDS ObjMgrProcLoadEx (OMPROC_FILTER, "mRNA from CDS", "mRNAfromCDS", OBJ_SEQFEAT, FEATDEF_CDS, OBJ_SEQFEAT, FEATDEF_CDS, NULL, MRnaFromCdsFunc, PROC_PRIORITY_DEFAULT, "Utilities")

#define REGISTER_SPLIT_BIOSEQ ObjMgrProcLoadEx (OMPROC_FILTER,"Split Bioseq Into Segments","SplitBioseqIntoSegments",0,0,0,0,NULL,SplitIntoSegmentedBioseq,PROC_PRIORITY_DEFAULT, "Indexer")

#define REGISTER_MAP_TO_PROT ObjMgrProcLoadEx (OMPROC_FILTER,"Map to Prot","MapToProt",OBJ_SEQFEAT,0,OBJ_SEQFEAT,0,NULL,MapToProtFunc,PROC_PRIORITY_DEFAULT, "Utilities")

#define REGISTER_MAP_TO_NUC ObjMgrProcLoadEx (OMPROC_FILTER,"Map to Nuc","MapToNuc",OBJ_SEQFEAT,0,OBJ_SEQFEAT,0,NULL,MapToNucFunc,PROC_PRIORITY_DEFAULT, "Utilities")

#define REGISTER_BIOSEQ_ORF ObjMgrProcLoadEx (OMPROC_FILTER, "ORF Finder", "OrfFinder", OBJ_BIOSEQ, 0, OBJ_BIOSEQ, 0, NULL, OrfFindFunc, PROC_PRIORITY_DEFAULT, "Utilities")

#define REGISTER_BIOSEQ_REVCOMP_WITHFEAT ObjMgrProcLoadEx (OMPROC_FILTER, "Bioseq and Features RevComp", "BioseqFeatsRevComp", OBJ_BIOSEQ, 0, OBJ_BIOSEQ, 0, NULL, RevCompFuncFeat, PROC_PRIORITY_DEFAULT, "Utilities")
#define REGISTER_BIOSEQ_REVCOMP_NOTFEAT ObjMgrProcLoadEx (OMPROC_FILTER, "Bioseq only RevComp", "BioseqOnlyRevComp", OBJ_BIOSEQ, 0, OBJ_BIOSEQ, 0, NULL, RevCompFunc, PROC_PRIORITY_DEFAULT, "Utilities")
#define REGISTER_BIOSEQ_REVERSE ObjMgrProcLoadEx (OMPROC_FILTER, "Bioseq Reverse", "BioseqReverse", OBJ_BIOSEQ, 0, OBJ_BIOSEQ, 0, NULL, RevFunc, PROC_PRIORITY_DEFAULT, "Utilities")
#define REGISTER_BIOSEQ_COMPLEMENT ObjMgrProcLoadEx (OMPROC_FILTER, "Bioseq Complement", "BioseqComplement", OBJ_BIOSEQ, 0, OBJ_BIOSEQ, 0, NULL, CompFunc, PROC_PRIORITY_DEFAULT, "Utilities")

#define REGISTER_BIOSEQ_SEG_REPORT ObjMgrProcLoadEx (OMPROC_FILTER, "Bioseq Seg Report", "BioseqSegReport", OBJ_BIOSEQ, 0, OBJ_BIOSEQ, 0, NULL, ReportDeltaSegments, PROC_PRIORITY_DEFAULT, "Misc")

#define REGISTER_FIXUP_RBS ObjMgrProcLoadEx (OMPROC_FILTER,"Fixup RBS","FixupRBS",0,0,0,0,NULL,FixupRBS,PROC_PRIORITY_DEFAULT, "Indexer")

#define REGISTER_BSP_INDEX ObjMgrProcLoadEx (OMPROC_FILTER,"Bioseq Index","MakeBioseqIndex",0,0,0,0,NULL,DoBioseqIndexing,PROC_PRIORITY_DEFAULT, "Indexer")

#define REGISTER_FIND_NON_ACGT ObjMgrProcLoadEx (OMPROC_FILTER,"Find Non ACGT","FindNonACGT",0,0,0,0,NULL,FindNonACGT,PROC_PRIORITY_DEFAULT, "Indexer")

#define REGISTER_TRIM_GENES ObjMgrProcLoadEx (OMPROC_FILTER,"Trim Genes","TrimGenes",0,0,0,0,NULL,TrimGenes,PROC_PRIORITY_DEFAULT, "Indexer")

#define REGISTER_OPENALED ObjMgrProcLoadEx (OMPROC_FILTER,"Open Align Editor 1","Open Contiguous Protein Alignment", 0,0,0,0,NULL,LaunchAlignEditorFromDesktop, PROC_PRIORITY_DEFAULT, "Alignment")

#define REGISTER_OPENALED2 ObjMgrProcLoadEx (OMPROC_FILTER,"Open Align Editor 2","Open Interleave Protein Alignment", 0,0,0,0,NULL,LaunchAlignEditorFromDesktop2, PROC_PRIORITY_DEFAULT, "Alignment")

#define REGISTER_OPENALED3 ObjMgrProcLoadEx (OMPROC_FILTER,"Open Align Editor 3","AA2NASeqAlign", 0,0,0,0,NULL,LaunchAlignEditorFromDesktop3, PROC_PRIORITY_DEFAULT, "Alignment")

#define REGISTER_OPENALED4 ObjMgrProcLoadEx (OMPROC_FILTER,"Open Align Editor 4","Alignment Profile", 0,0,0,0,NULL,LaunchAlignEditorFromDesktop4, PROC_PRIORITY_DEFAULT, "Alignment")

#define REGISTER_MAKEEXONINTRON ObjMgrProcLoadEx (OMPROC_FILTER,"Make Exons and Introns","MakeExonIntron",OBJ_SEQFEAT,0,OBJ_SEQFEAT,0,NULL,MakeExonIntron,PROC_PRIORITY_DEFAULT, "Misc")

#define REGISTER_PROT_IDS_TO_GENE_SYN ObjMgrProcLoadEx (OMPROC_FILTER,"Protein SeqID to Gene Synonym","ProtLocalIDtoGeneSyn",0,0,0,0,NULL,ProtLocalIDtoGeneSyn,PROC_PRIORITY_DEFAULT, "Indexer")

#define REGISTER_DESKTOP_REPORT ObjMgrProcLoadEx (OMPROC_FILTER, "Desktop Report", "DesktopReport", 0, 0, 0, 0, NULL, DesktopReportFunc, PROC_PRIORITY_DEFAULT, "Indexer")

#define REGISTER_DESCRIPTOR_PROPAGATE ObjMgrProcLoadEx (OMPROC_FILTER, "Descriptor Propagate", "DescriptorPropagate", 0, 0, 0, 0, NULL, DescriptorPropagate, PROC_PRIORITY_DEFAULT, "Indexer")

#define REGISTER_CLEAR_SEQENTRYSCOPE ObjMgrProcLoadEx (OMPROC_FILTER, "Clear SeqEntry Scope", "ClearSeqEntryScope", 0, 0, 0, 0, NULL, DoClearSeqEntryScope, PROC_PRIORITY_DEFAULT, "Indexer")

#define REGISTER_SEQUIN_PROT_TITLES ObjMgrProcLoadEx (OMPROC_FILTER,"Sequin Style Protein Titles","SequinStyleProteinTitles",0,0,0,0,NULL,MakeSequinProteinTitles,PROC_PRIORITY_DEFAULT, "Misc")
extern Int2 LIBCALLBACK MakeSequinProteinTitles (Pointer data);

#define REGISTER_SEQUIN_NUC_TITLES ObjMgrProcLoadEx (OMPROC_FILTER,"Sequin Style Nucleotide Titles","SequinStyleNucleotideTitles",0,0,0,0,NULL,MakeSequinNucleotideTitles,PROC_PRIORITY_DEFAULT, "Misc")
extern Int2 LIBCALLBACK MakeSequinNucleotideTitles (Pointer data);

#define REGISTER_SEQUIN_FEAT_TABLE ObjMgrProcLoadEx (OMPROC_FILTER,"Sequin Style Feature Table","SequinStyleFeatureTable",0,0,0,0,NULL,MakeSequinFeatureTable,PROC_PRIORITY_DEFAULT, "Misc")
extern Int2 LIBCALLBACK MakeSequinFeatureTable (Pointer data);

#define REGISTER_SEQUIN_DUMP_CONTIG ObjMgrProcLoadEx (OMPROC_FILTER,"Make Contig Build Table","MakeContigBuildTable",0,0,0,0,NULL,MakeContigBuildTable,PROC_PRIORITY_DEFAULT, "Misc")
extern Int2 LIBCALLBACK MakeContigBuildTable (Pointer data);

#define REGISTER_SEQUIN_GI_TO_ACCN ObjMgrProcLoadEx (OMPROC_FILTER,"Convert Align GI to Accession","ConvertAlignGisToAccn",0,0,0,0,NULL,AlignGiToAccnProc,PROC_PRIORITY_DEFAULT, "Misc")
static Int2 LIBCALLBACK AlignGiToAccnProc (Pointer data);

#define REGISTER_SEQUIN_ACCN_TO_GI ObjMgrProcLoadEx (OMPROC_FILTER,"Convert Align Accession To Gi","ConvertAlignAccnsToGi",0,0,0,0,NULL,AlignAccnToGiProc,PROC_PRIORITY_DEFAULT, "Misc")
static Int2 LIBCALLBACK AlignAccnToGiProc (Pointer data);

#define REGISTER_SEQUIN_CACHE_ACCN ObjMgrProcLoadEx (OMPROC_FILTER,"Cache Accessions to Disk","CacheAccnsToDisk",0,0,0,0,NULL,CacheAccnsToDisk,PROC_PRIORITY_DEFAULT, "Misc")
static Int2 LIBCALLBACK CacheAccnsToDisk (Pointer data);

#define REGISTER_SEPARATE_MRNA_ALIGNS ObjMgrProcLoadEx (OMPROC_FILTER,"Separate mRNA Alignments from NR","SeparateMrnaAlignsFromNR",0,0,0,0,NULL,SeparateMrnaFromNrProc,PROC_PRIORITY_DEFAULT, "Misc")
static Int2 LIBCALLBACK SeparateMrnaFromNrProc (Pointer data);

#define REGISTER_REFGENEUSER_DESC_EDIT ObjMgrProcLoad(OMPROC_EDIT,"Edit RefGene UserTrack Desc","RefGene Tracking",OBJ_SEQDESC,Seq_descr_user,OBJ_SEQDESC,Seq_descr_user,NULL,RefGeneUserGenFunc,PROC_PRIORITY_DEFAULT)
extern Int2 LIBCALLBACK RefGeneUserGenFunc (Pointer data);

#define REGISTER_TPAASSEMBLYUSER_DESC_EDIT ObjMgrProcLoad(OMPROC_EDIT,"Edit Assembly User Desc","TPA Assembly",OBJ_SEQDESC,Seq_descr_user,OBJ_SEQDESC,Seq_descr_user,NULL,AssemblyUserGenFunc,PROC_PRIORITY_DEFAULT)

typedef struct {
  CharPtr  oldStr;
  SeqIdPtr newSip;
} ReplaceIDStruct, PNTR ReplaceIDStructPtr;

typedef struct _explodeStruct {
  SeqEntryPtr topSep;
  SeqFeatPtr  seqFeatPtr;
  struct _explodeStruct PNTR next;
} ExplodeStruct, PNTR ExplodeStructPtr;

typedef struct {
  DESCRIPTOR_FORM_BLOCK
  PopuP        fromPopup;
  PopuP        toPopup;
  Int2         fromStrand;
  Int2         toStrand;
  ValNodePtr   featlist;
  LisT         feature;
  Int2         featSubType;
  CharPtr      findThisStr;
  TexT         findThis;
} EditStrand, PNTR EditStrandPtr;

extern Int2 LIBCALLBACK AssemblyUserGenFunc (Pointer data);

static void AddBspToSegSet (BioseqPtr segseq, BioseqPtr bsp)

{
  SeqIdPtr   sip;
  SeqLocPtr  slp;

  if (segseq == NULL) return;
  slp = ValNodeNew ((ValNodePtr) segseq->seq_ext);
  if (slp == NULL) return;
  if (segseq->seq_ext == NULL) {
    segseq->seq_ext = (Pointer) slp;
  }
  if (bsp != NULL && bsp->length > 0) {
    segseq->length += bsp->length;
    slp->choice = SEQLOC_WHOLE;
    sip = SeqIdFindBest (bsp->id, 0);
    slp->data.ptrvalue = (Pointer) SeqIdStripLocus (SeqIdDup (sip));
  } else {
    slp->choice = SEQLOC_NULL;
  }
}

static void RemoveMolInfoDescriptors (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  ValNodePtr     nextsdp;
  Pointer PNTR   prevsdp;
  ValNodePtr     sdp;

  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sdp = bsp->descr;
    prevsdp = (Pointer PNTR) &(bsp->descr);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sdp = bssp->descr;
    prevsdp = (Pointer PNTR) &(bssp->descr);
  } else return;
  while (sdp != NULL) {
    nextsdp = sdp->next;
    if (sdp->choice == Seq_descr_molinfo) {
      *(prevsdp) = sdp->next;
      sdp->next = NULL;
      SeqDescFree (sdp);
    } else {
      prevsdp = (Pointer PNTR) &(sdp->next);
    }
    sdp = nextsdp;
  }
}

static void MoveSegSetMolInfo (BioseqPtr segseq, BioseqSetPtr parts, BioseqSetPtr segset)

{
  MolInfoPtr   first;
  MolInfoPtr   mip;
  SeqEntryPtr  partssep;
  ValNodePtr   sdp;
  SeqEntryPtr  sep;
  SeqEntryPtr  tmp;

  if (segseq == NULL || parts == NULL || parts->seq_set == NULL || segset == NULL) return;
  sep = SeqMgrGetSeqEntryForData (segset);
  if (sep == NULL) return;
  sdp = SeqEntryGetSeqDescr (sep, Seq_descr_molinfo, NULL);
  if (sdp != NULL) return;
  first = NULL;
  partssep = SeqMgrGetSeqEntryForData (parts);
  if (partssep != NULL) {
    sdp = SeqEntryGetSeqDescr (partssep, Seq_descr_molinfo, NULL);
    if (sdp != NULL) {
      first = (MolInfoPtr) sdp->data.ptrvalue;
    }
  }
  for (tmp = parts->seq_set; tmp != NULL; tmp = tmp->next) {
    sdp = SeqEntryGetSeqDescr (tmp, Seq_descr_molinfo, NULL);
    if (sdp != NULL) {
      if (first == NULL) {
        first = (MolInfoPtr) sdp->data.ptrvalue;
      } else {
        mip = (MolInfoPtr) sdp->data.ptrvalue;
        if (first != NULL && mip != NULL) {
          if (mip->biomol != first->biomol) return;
        }
      }
    }
  }
  if (first == NULL) return;
  mip = MolInfoNew ();
  if (mip == NULL) return;
  sdp = CreateNewDescriptor (sep, Seq_descr_molinfo);
  if (sdp == NULL) return;
  sdp->data.ptrvalue = (Pointer) mip;
  mip->biomol = first->biomol;
  mip->tech = first->tech;
  mip->completeness = first->completeness;
  mip->techexp = StringSaveNoNull (first->techexp);
  if (partssep != NULL) {
    SeqEntryExplore (partssep, NULL, RemoveMolInfoDescriptors);
  }
  for (tmp = parts->seq_set; tmp != NULL; tmp = tmp->next) {
    SeqEntryExplore (tmp, NULL, RemoveMolInfoDescriptors);
  }
}

static void DoUpdateSegSet (BioseqPtr segseq, BioseqSetPtr parts, Boolean ask)

{
  MsgAnswer    ans;
  BioseqPtr    bsp;
  Boolean      notFirst;
  Boolean      nullsBetween;
  SeqFeatPtr   sfp;
  SeqFeatPtr   sfpnext;
  SeqLocPtr    slp;
  SeqEntryPtr  tmp;

  if (segseq == NULL || parts == NULL || parts->seq_set == NULL) return;
  tmp = parts->seq_set;
  notFirst = FALSE;
  nullsBetween = FALSE;
  segseq->length = 0;
  switch (segseq->seq_ext_type) {
    case 1:    /* seg-ext */
      slp = (ValNodePtr) segseq->seq_ext;
      while (slp != NULL) {
        if (slp->choice == SEQLOC_NULL) {
          nullsBetween = TRUE;
        }
        slp = slp->next;
      }
      SeqLocSetFree ((ValNodePtr) segseq->seq_ext);
      break;
    case 2:   /* reference */
      SeqLocFree ((ValNodePtr) segseq->seq_ext);
      break;
    case 3:   /* map */
      sfp = (SeqFeatPtr) segseq->seq_ext;
      while (sfp != NULL) {
        sfpnext = sfp->next;
        SeqFeatFree (sfp);
        sfp = sfpnext;
      }
      break;
    default:
      break;
  }
  segseq->seq_ext = NULL;
  if (ask) {
    if (nullsBetween) {
      ans = Message (MSG_YN, "Intersperse intervals with gaps (currently has gaps)?");
    } else {
      ans = Message (MSG_YN, "Intersperse intervals with gaps (currently no gaps)?");
    }
    nullsBetween = (Boolean) (ans == ANS_YES);
  }
  while (tmp != NULL) {
    if (nullsBetween && notFirst) {
      AddBspToSegSet (segseq, NULL);
    }
    bsp = (BioseqPtr) tmp->data.ptrvalue;
    if (bsp != NULL) {
      AddBspToSegSet (segseq, bsp);
    }
    notFirst = TRUE;
    tmp = tmp->next;
  }
}

typedef struct updatesegstruc {
  BioseqSetPtr      parts;
  BioseqPtr         segseq;
  BioseqSetPtr      segset;
} UpdateSegStruc, PNTR UpdateSegStrucPtr;

static void FindSegSetComponentsCallback (SeqEntryPtr sep, Pointer mydata,
                                          Int4 index, Int2 indent)

{
  BioseqPtr          bsp;
  BioseqSetPtr       bssp;
  UpdateSegStrucPtr  ussp;

  if (sep != NULL && sep->data.ptrvalue && mydata != NULL) {
    ussp = (UpdateSegStrucPtr) mydata;
    if (sep->choice == 1) {
      bsp = (BioseqPtr) sep->data.ptrvalue;
      if (ISA_na (bsp->mol) && bsp->repr == Seq_repr_seg) {
        ussp->segseq = bsp;
      }
    } else if (sep->choice == 2) {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
      if (bssp->_class == 2) {
        ussp->segset = bssp;
      } else if (bssp->_class == 4) {
        ussp->parts = bssp;
      }
    }
  }
}

static Int4 UpdateSegList (SeqEntryPtr sep, Pointer mydata,
                           SeqEntryFunc mycallback,
                           Int4 index, Int2 indent)

{
  BioseqSetPtr  bssp;

  if (sep == NULL) return index;
  if (mycallback != NULL)
    (*mycallback) (sep, mydata, index, indent);
  index++;
  if (IS_Bioseq (sep)) return index;
  if (Bioseq_set_class (sep) == 4) return index;
  bssp = (BioseqSetPtr) sep->data.ptrvalue;
  sep = bssp->seq_set;
  indent++;
  while (sep != NULL) {
    index = UpdateSegList (sep, mydata, mycallback, index, indent);
    sep = sep->next;
  }
  return index;
}

#define UpdateSegExplore(a,b,c) UpdateSegList(a, b, c, 0L, 0);

extern Int2 DoOneSegFixup (SeqEntryPtr sep, Boolean ask);
extern Int2 DoOneSegFixup (SeqEntryPtr sep, Boolean ask)

{
  BioseqSetPtr    bssp;
  Uint1           choice;
  Int2            count;
  SeqEntryPtr     insert;
  SeqEntryPtr     next;
  ObjMgrDataPtr   omdptop;
  ObjMgrData      omdata;
  Uint2           parenttype;
  Pointer         parentptr;
  UpdateSegStruc  uss;
  SeqEntryPtr     tmp;

  if (sep == NULL) return 0;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 7 ||
                         (IsPopPhyEtcSet (bssp->_class)))) {
      choice = 0;
      for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
        if (choice == 0) {
          choice = tmp->choice;
        } else if (choice != tmp->choice) {
          return 0;
        }
      }
      count = 0;
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        count += DoOneSegFixup (sep, ask);
      }
      return count;
    }
  }
  if (IS_Bioseq (sep) && sep->next != NULL) {
    count = 0;
    SaveSeqEntryObjMgrData (sep, &omdptop, &omdata);
    GetSeqEntryParent (sep, &parentptr, &parenttype);
    insert = sep->next;
    sep->next = NULL;
    while (insert != NULL) {
      next = insert->next;
      insert->next = NULL;
      AddSeqEntryToSeqEntry (sep, insert, FALSE);
      count++;
      insert = next;
    }
    SeqMgrLinkSeqEntry (sep, parenttype, parentptr);
    RestoreSeqEntryObjMgrData (sep, omdptop, &omdata);
    uss.segseq = NULL;
    uss.parts = NULL;
    uss.segset = NULL;
    UpdateSegExplore (sep, (Pointer) &uss, FindSegSetComponentsCallback);
    if (uss.segseq != NULL && uss.parts != NULL && uss.segset != NULL) {
      DoUpdateSegSet (uss.segseq, uss.parts, ask);
      MoveSegSetMolInfo (uss.segseq, uss.parts, uss.segset);
    }
    return count;
  }
  uss.segseq = NULL;
  uss.parts = NULL;
  uss.segset = NULL;
  UpdateSegExplore (sep, (Pointer) &uss, FindSegSetComponentsCallback);
  if (uss.segseq != NULL && uss.parts != NULL && uss.segset != NULL) {
    DoUpdateSegSet (uss.segseq, uss.parts, ask);
    MoveSegSetMolInfo (uss.segseq, uss.parts, uss.segset);
    return 1;
  }
  return 0;
}

extern void DoFixupLocus (SeqEntryPtr sep);
extern void DoFixupSegSet (SeqEntryPtr sep);

static Int2 LIBCALLBACK UpdateSegSet (Pointer data)

{
  OMProcControlPtr  ompcp;
  SeqEntryPtr       sep;
  ErrSev            sev;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;
  switch (ompcp->input_itemtype) {
    case OBJ_BIOSEQ :
      break;
    case OBJ_BIOSEQSET :
      break;
    case 0 :
      return OM_MSG_RET_ERROR;
    default :
      return OM_MSG_RET_ERROR;
  }
  if (ompcp->input_data == NULL) return OM_MSG_RET_ERROR;
  sep = GetTopSeqEntryForEntityID (ompcp->input_entityID);
  if (sep == NULL) return OM_MSG_RET_ERROR;
  DoOneSegFixup (sep, TRUE);
  if (Message (MSG_YN, "Do you want to Force Locus Fixup?") == ANS_YES) {
    sev = ErrSetMessageLevel (SEV_FATAL);
    DoFixupLocus (sep);
    DoFixupSegSet (sep);
    ErrSetMessageLevel (sev);
  }
  ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, 0, 0);
  Update ();
  return OM_MSG_RET_DONE;
}

static void DoAdjustSegSeqLength (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr   bsp;
  ValNode     head;
  Int4        len;
  Int4        len2;
  ValNodePtr  vnp;

  if (! IS_Bioseq (sep)) return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL || bsp->repr != Seq_repr_seg) return;
  head.choice = SEQLOC_MIX;
  head.data.ptrvalue = bsp->seq_ext;
  head.next = NULL;
  vnp = NULL;
  len = 0;
  while ((vnp = SeqLocFindNext (&head, vnp)) != NULL) {
    len2 = SeqLocLen (vnp);
    if (len2 > 0) {
      len += len2;
    }
  }
  bsp->length = len;
}

static Int2 LIBCALLBACK AdjustSegSeqLength (Pointer data)

{
  OMProcControlPtr  ompcp;
  SeqEntryPtr       sep;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;
  switch (ompcp->input_itemtype) {
    case OBJ_BIOSEQ :
      break;
    case OBJ_BIOSEQSET :
      break;
    case 0 :
      return OM_MSG_RET_ERROR;
    default :
      return OM_MSG_RET_ERROR;
  }
  if (ompcp->input_data == NULL) return OM_MSG_RET_ERROR;
  sep = GetTopSeqEntryForEntityID (ompcp->input_entityID);
  if (sep == NULL) return OM_MSG_RET_ERROR;
  SeqEntryExplore (sep, NULL, DoAdjustSegSeqLength);
  ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, 0, 0);
  return OM_MSG_RET_DONE;
}

#if 0
static SeqLocPtr FixLonelySeqLocIntervals (SeqLocPtr slp)
{
  SeqLocPtr loc;

  if (slp == NULL || (slp->choice != SEQLOC_MIX && slp->choice != SEQLOC_PACKED_INT)) {
    return slp;
  }

  loc = SeqLocFindNext (slp, NULL);
  if (loc == NULL) {
    SeqLocFree (slp);
    return NULL;
  } else if (loc->next == NULL) {
    slp->data.ptrvalue = NULL;
    SeqLocFree (slp);
    return loc;
  } else {
    return slp;
  }
}
#endif

static SeqLocPtr ReduceLocationToSingleBioseq (SeqLocPtr slp, BioseqPtr bsp)
{
  SeqLocPtr this_slp, tmp_slp;
  BioseqPtr this_bsp;

  if (slp == NULL || bsp == NULL) return NULL;

  this_slp = SeqLocFindNext (slp, NULL);

  while (slp != NULL && this_slp != NULL) {
    this_bsp = BioseqFindFromSeqLoc (this_slp);
    if (this_bsp != bsp) {
      tmp_slp = SeqLocMerge (this_bsp, this_slp, NULL, TRUE, FALSE, FALSE);
      slp = SeqLocSubtract (slp, tmp_slp);
      SeqLocFree (tmp_slp);
      this_slp = SeqLocFindNext (slp, NULL);
    } else {
      this_slp = SeqLocFindNext (slp, this_slp);
    }
  }
  return slp;
}

static SeqLocPtr RemoveBioseqFromLocation (SeqLocPtr slp, BioseqPtr bsp)
{
  SeqLocPtr this_slp, tmp_slp;
  BioseqPtr this_bsp;

  if (slp == NULL || bsp == NULL) return slp;

  this_slp = SeqLocFindNext (slp, NULL);
  while (slp != NULL && this_slp != NULL) {
    this_bsp = BioseqFindFromSeqLoc (this_slp);
    if (this_bsp == bsp) {
      tmp_slp = SeqLocMerge (bsp, this_slp, NULL, TRUE, FALSE, FALSE);
      slp = SeqLocSubtract (slp, tmp_slp);
      SeqLocFree (tmp_slp);
      this_slp = SeqLocFindNext (slp, NULL);
    } else {
      this_slp = SeqLocFindNext (slp, this_slp);
    }
  }
  return slp;
}

static void PushFeaturesDownToBioseq (SeqAnnotPtr annot, SeqEntryPtr sep)
{
  BioseqSetPtr bssp;
  BioseqPtr    bsp, this_bsp;
  SeqFeatPtr   sfp, prev_sfp, last_sfp, next_sfp;

  if (annot == NULL || annot->type != 1 || sep == NULL) return;

  if (IS_Bioseq_set (sep)) {
    bssp = sep->data.ptrvalue;
    for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
      PushFeaturesDownToBioseq (annot, sep);
    }
    return;
  }

  if (! IS_Bioseq (sep)) return;

  bsp = sep->data.ptrvalue;
  sfp = annot->data;
  prev_sfp = NULL;
  while (sfp != NULL) {
    next_sfp = sfp->next;
    while (sfp != NULL && (this_bsp = BioseqFindFromSeqLoc (sfp->location)) != bsp) {
      prev_sfp = sfp;
      sfp = sfp->next;
      if (sfp != NULL) {
        next_sfp = sfp->next;
      }
    }
    if (sfp != NULL) {  
      if (bsp->annot == NULL) {
        bsp->annot = SeqAnnotNew ();
        bsp->annot->type = 1;
        bsp->annot->data = sfp;
      } else {
        if (bsp->annot->data == NULL) {
          bsp->annot->data = sfp;
        } else {
          last_sfp = bsp->annot->data;
          while (last_sfp->next != NULL) {
            last_sfp = last_sfp->next;
          }
          last_sfp->next = sfp;
        }
      }
      if (prev_sfp == NULL) {
        annot->data = sfp->next;
      } else {
        prev_sfp->next = sfp->next;
      }
      sfp->next = NULL;
      sfp = next_sfp;
    }
  }
}

static void SplitSegmentedFeatsOnOneSet (BioseqSetPtr set)
{
  SeqAnnotPtr annot, prev_annot, next_annot;
  SeqFeatPtr  sfp, new_sfp, prev_sfp, next_sfp;
  SeqEntryPtr sep;
  BioseqPtr   bsp, last_bsp;
  SeqLocPtr   loc, slp;

  if (set == NULL || set->annot == NULL) return;
  prev_annot = NULL;
  annot = set->annot;
  next_annot = annot->next;
  while (annot != NULL) {
    next_annot = annot->next;
    if (annot->type != 1) {
      prev_annot = annot;
      annot = next_annot;
      continue;
    }
    sfp = annot->data;
    while (sfp != NULL) {
      next_sfp = sfp->next;
      prev_sfp = sfp;
      loc = SeqLocFindNext (sfp->location, NULL);
      last_bsp = BioseqFindFromSeqLoc (loc);
      slp = SeqLocFindNext (sfp->location, loc);
      while (slp != NULL) {
        bsp = BioseqFindFromSeqLoc (slp);
        if (bsp != last_bsp && bsp != NULL) {
          new_sfp = SeqFeatCopy (sfp);
          new_sfp->location = ReduceLocationToSingleBioseq (new_sfp->location, bsp);
          sfp->location = RemoveBioseqFromLocation (sfp->location, bsp);
          prev_sfp->next = new_sfp;
          new_sfp->next = next_sfp;
          prev_sfp = new_sfp;
          slp = SeqLocFindNext (sfp->location, NULL);
        } else {
          slp = SeqLocFindNext (sfp->location, slp);
        }
      }
      sfp = next_sfp;
    }

    /* push features to appropriate bioseqs */
    for (sep = set->seq_set; sep != NULL; sep = sep->next) {
      PushFeaturesDownToBioseq (annot, sep);
    }
    if (prev_annot == NULL) {
      set->annot = annot->next;
    } else {
      prev_annot->next = annot->next;
    }
    annot->next = NULL;
    SeqAnnotFree (annot);
    annot = next_annot;      
  }
}

static void SplitSegmentedFeats (SeqEntryPtr sep)
{
  BioseqSetPtr bssp;

  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    SplitSegmentedFeatsOnOneSet (bssp);
    for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
      SplitSegmentedFeats (sep);
    }
  }
}

extern void SplitSegmentedFeatsMenuItem (IteM i)
{
  BaseFormPtr   bfp;
  SeqEntryPtr   sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  SplitSegmentedFeats (sep);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);

}

static void DoSegSetUndo (BioseqPtr segseq, BioseqSetPtr parts, BioseqSetPtr segset)

{
  SeqAnnotPtr  annot;
  ValNodePtr   descr;
  SeqAnnotPtr  sap;
  SeqEntryPtr  tmp;

  if (segseq == NULL || parts == NULL || parts->seq_set == NULL || segset == NULL) return;
  segset->_class = 7;
  parts->_class = 14;
  annot = segseq->annot;
  segseq->annot = NULL;
  if (segset->annot == NULL) {
    segset->annot = annot;
    annot = NULL;
  } else {
    sap = segset->annot;
    while (sap->next != NULL) {
      sap = sap->next;
    }
    sap->next = annot;
  }
  descr = segseq->descr;
  segseq->descr = NULL;
  ValNodeLink (&(segset->descr), descr);
  tmp = segset->seq_set;
  if (tmp != NULL && IS_Bioseq (tmp)) {
    segset->seq_set = tmp->next;
    tmp->next = NULL;
    SeqEntryFree (tmp);
  }
}

static Int2 DoOneSegUndo (SeqEntryPtr sep)

{
  BioseqSetPtr    bssp;
  Int2            count = 0;
  UpdateSegStruc  uss;

  if (sep == NULL) return 0;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 7 ||
                         (IsPopPhyEtcSet (bssp->_class)))) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        count += DoOneSegUndo (sep);
      }
      return count;
    }
  }

  uss.segseq = NULL;
  uss.parts = NULL;
  uss.segset = NULL;
  UpdateSegExplore (sep, (Pointer) &uss, FindSegSetComponentsCallback);
  if (uss.segseq != NULL && uss.parts != NULL && uss.segset != NULL) {
    DoSegSetUndo (uss.segseq, uss.parts, uss.segset);
    return 1;
  }
  return 0;
}

static SeqEntryPtr BioseqExtract (SeqEntryPtr top, BioseqPtr bsp)

{
  BioseqSetPtr  bssp;
  SeqEntryPtr   next;
  SeqEntryPtr   PNTR prev;
  SeqEntryPtr   rsult = NULL;
  SeqEntryPtr   sep;

  if (top == NULL || bsp == NULL) return NULL;
  if (! IS_Bioseq_set (top)) return NULL;
  bssp = (BioseqSetPtr) top->data.ptrvalue;
  if (bssp == NULL) return NULL;
  prev = &(bssp->seq_set);
  sep = bssp->seq_set;
  while (sep != NULL) {
    next = sep->next;
    if (IS_Bioseq_set (sep)) {
      rsult = BioseqExtract (sep, bsp);
      if (rsult != NULL) {
        return rsult;
      }
      prev = &(sep->next);
      sep = next;
    } else if (IS_Bioseq (sep) && sep->data.ptrvalue == (Pointer) bsp) {
      *(prev) = next;
      sep->next = NULL;
      return sep;
    } else {
      prev = &(sep->next);
      sep = next;
    }
  }
  return NULL;
}

static void DoRepairPartsSet (SeqEntryPtr sep)

{
  BioseqPtr       bsp;
  BioseqSetPtr    bssp;
  ValNode         head;
  BioseqSetPtr    parts;
  BioseqPtr       segseq;
  SeqLocPtr       slp;
  SeqEntryPtr     tmp;
  UpdateSegStruc  uss;

  if (sep == NULL) return;
  if (IS_Bioseq (sep)) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp == NULL) return;
    if (bssp->_class == 7 ||
        (IsPopPhyEtcSet (bssp->_class))) {
      for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
        DoRepairPartsSet (tmp);
      }
      return;
    }
  }
  uss.segseq = NULL;
  uss.parts = NULL;
  uss.segset = NULL;
  UpdateSegExplore (sep, (Pointer) &uss, FindSegSetComponentsCallback);
  if (uss.segseq == NULL || uss.parts == NULL || uss.segset == NULL) return;
  segseq = uss.segseq;
  parts = uss.parts;
  if (segseq->repr != Seq_repr_seg ||
      segseq->seq_ext_type != 1 ||
      segseq->seq_ext == NULL) return;
  head.choice = SEQLOC_MIX;
  head.data.ptrvalue = segseq->seq_ext;
  head.next = NULL;
  slp = NULL;
  while ((slp = SeqLocFindNext (&head, slp)) != NULL) {
    bsp = BioseqFind (SeqLocId (slp));
    if (bsp != NULL) {
      tmp = BioseqExtract (sep, bsp);
      if (tmp != NULL) {
        ValNodeLink (&parts->seq_set, tmp);
      }
    }
  }
}

static Int2 LIBCALLBACK PackagePartsInPartsSet (Pointer data)

{
  OMProcControlPtr  ompcp;
  ObjMgrDataPtr     omdptop;
  ObjMgrData        omdata;
  Uint2             parenttype;
  Pointer           parentptr;
  SeqEntryPtr       sep;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;
  switch (ompcp->input_itemtype) {
    case OBJ_BIOSEQ :
      break;
    case OBJ_BIOSEQSET :
      break;
    case 0 :
      return OM_MSG_RET_ERROR;
    default :
      return OM_MSG_RET_ERROR;
  }
  if (ompcp->input_data == NULL) return OM_MSG_RET_ERROR;
  sep = GetTopSeqEntryForEntityID (ompcp->input_entityID);
  if (sep == NULL) return OM_MSG_RET_ERROR;
  SaveSeqEntryObjMgrData (sep, &omdptop, &omdata);
  GetSeqEntryParent (sep, &parentptr, &parenttype);
  DoRepairPartsSet (sep);
  SeqMgrLinkSeqEntry (sep, parenttype, parentptr);
  RestoreSeqEntryObjMgrData (sep, omdptop, &omdata);
  ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, 0, 0);
  return OM_MSG_RET_DONE;
}

static void RemoveDupGenBankSets (SeqEntryPtr sep)

{
  SeqAnnotPtr   annot;
  BioseqSetPtr  bssp;
  ValNodePtr    descr;
  SeqAnnotPtr   sap;
  SeqEntryPtr   tmp;
  BioseqSetPtr  tmpbssp;

  if (sep == NULL || ! IS_Bioseq_set (sep)) return;
  bssp = (BioseqSetPtr) sep->data.ptrvalue;
  if (bssp == NULL || bssp->_class != 7) return;
  tmp = bssp->seq_set;
  if (tmp == NULL || ! IS_Bioseq_set (tmp) || tmp->next != NULL) return;
  tmpbssp = (BioseqSetPtr) tmp->data.ptrvalue;
  if (tmpbssp == NULL || tmpbssp->_class != 7 || tmpbssp->seq_set == NULL) return;
  annot = tmpbssp->annot;
  tmpbssp->annot = NULL;
  if (bssp->annot == NULL) {
    bssp->annot = annot;
    annot = NULL;
  } else {
    sap = bssp->annot;
    while (sap->next != NULL) {
      sap = sap->next;
    }
    sap->next = annot;
  }
  descr = tmpbssp->descr;
  tmpbssp->descr = NULL;
  ValNodeLink (&(bssp->descr), descr);
  bssp->seq_set = tmpbssp->seq_set;
  tmpbssp->seq_set = NULL;
  SeqEntryFree (tmp);
}

static Int2 LIBCALLBACK UndoSegSet (Pointer data)

{
  Int2              count;
  ObjMgrDataPtr     omdptop;
  ObjMgrData        omdata;
  OMProcControlPtr  ompcp;
  Uint2             parenttype;
  Pointer           parentptr;
  SeqEntryPtr       sep;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;
  switch (ompcp->input_itemtype) {
    case OBJ_BIOSEQ :
      break;
    case OBJ_BIOSEQSET :
      break;
    case 0 :
      return OM_MSG_RET_ERROR;
    default :
      break;
  }
  if (ompcp->input_data == NULL) return OM_MSG_RET_ERROR;
  sep = GetTopSeqEntryForEntityID (ompcp->input_entityID);
  if (sep == NULL) return OM_MSG_RET_ERROR;
  SaveSeqEntryObjMgrData (sep, &omdptop, &omdata);
  GetSeqEntryParent (sep, &parentptr, &parenttype);
  count = DoOneSegUndo (sep);
  RemoveDupGenBankSets (sep);
  SeqMgrLinkSeqEntry (sep, parenttype, parentptr);
  RestoreSeqEntryObjMgrData (sep, omdptop, &omdata);
  if (count > 0) {
    PropagateFromGenBankBioseqSet (sep, FALSE);
    SeqMgrClearFeatureIndexes (ompcp->input_entityID, NULL);
    SeqMgrIndexFeatures (ompcp->input_entityID, NULL);
    SplitSegmentedFeats (sep);
    ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, 0, 0);
  }
  return OM_MSG_RET_DONE;
}

#if 0
typedef struct denseg {
    Int2 dim,
        numseg;
    SeqIdPtr ids;           /* dimension is dim */
    Int4Ptr starts;			/* dimension is dim * numseg */
    Int4Ptr lens;			/* dimension is numseg */
    Uint1Ptr strands;		/* dimension is dim * numseg */
    ScorePtr scores;		/* dimension is numseg */
} DenseSeg, PNTR DenseSegPtr;
#endif

static void DoPartSeqAlignToParent (DenseSegPtr dsp)

{
  BioseqPtr             bsp, part;
  SeqMgrSegmentContext  context;
  Int2                  i, j, k;
  SeqIdPtr              sip, next;
  SeqIdPtr PNTR         prev;
  Int4                  val;

  if (dsp == NULL || dsp->ids == NULL || dsp->starts == NULL) return;
  for (i = 0, sip = dsp->ids, prev = &(dsp->ids);
       i < dsp->dim && sip != NULL;
       i++, sip = next) {
    next = sip->next;
    part = BioseqFind (sip);
    if (part == NULL) continue;
    bsp = SeqMgrGetParentOfPart (part, &context);
    if (bsp == NULL) continue;
    *prev = NULL;
    sip->next = NULL;
    SeqIdFree (sip);
    sip = SeqIdDup (SeqIdFindBest (bsp->id, 0));
    *prev = sip;
    sip->next = next;
    prev = &(sip->next);
    for (j = 0; j < dsp->numseg; j++) {
      k = (dsp->dim * j) + i;
      val = dsp->starts [k];
      if (val != -1) {
        if (context.strand == Seq_strand_minus) {
          dsp->starts [k] = context.cumOffset + (context.to - val);
        } else {
          dsp->starts [k] = context.cumOffset + (val - context.from);
        }
      }
    }
  }
}

static Int2 LIBCALLBACK PartSeqAlignToParent (Pointer data)

{
  DenseSegPtr       dsp;
  OMProcControlPtr  ompcp;
  SeqAlignPtr       sap;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;
  switch (ompcp->input_itemtype) {
    case OBJ_SEQALIGN :
      sap = (SeqAlignPtr) ompcp->input_data;
      break;
    case 0 :
      return OM_MSG_RET_ERROR;
    default :
      return OM_MSG_RET_ERROR;
  }
  if (sap == NULL) return OM_MSG_RET_ERROR;

  if (sap->segtype == SAS_DENSEG) {
    dsp = (DenseSegPtr) sap->segs;
    if (dsp != NULL) {
      DoPartSeqAlignToParent (dsp);
    }
  }

  ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, 0, 0);

  return OM_MSG_RET_DONE;
}

static Int2 LIBCALLBACK ConvertToTrueMultipleAlignment (Pointer data)

{
  OMProcControlPtr  ompcp;
  SeqAlignPtr       salp, sap, next;
  SeqAnnotPtr       sanp;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;
  switch (ompcp->input_itemtype) {
    case OBJ_SEQALIGN :
      break;
    case OBJ_SEQANNOT :
      break;
    case 0 :
      return OM_MSG_RET_ERROR;
    default :
      return OM_MSG_RET_ERROR;
  }
  if (ompcp->input_data == NULL) return OM_MSG_RET_ERROR;
  sap = NULL;
  if (ompcp->input_itemtype == OBJ_SEQALIGN)
     sap = (SeqAlignPtr) ompcp->input_data;
  else
  {
     sanp = (SeqAnnotPtr) ompcp->input_data;
     if (sanp->type == 2)
        sap = (SeqAlignPtr)(sanp->data);
  }
  if (sap == NULL) return OM_MSG_RET_ERROR;
  salp = sap;
  while (salp != NULL)
  {
     if (salp->saip != NULL)
     {
        SeqAlignIndexFree(salp->saip);
        salp->saip = NULL;
     }
     salp = salp->next;
  }
  AlnMgr2IndexSeqAlign (sap);

  salp = AlnMgr2GetSubAlign (sap, 0, -1, 0, FALSE);
  if (salp == NULL) return OM_MSG_RET_ERROR;

  if (sap->idx.prevlink != NULL) {
    *(sap->idx.prevlink) = (Pointer) salp;
  }

  while (sap != NULL) {
    next = sap->next;
    sap->next = NULL;
    SeqAlignFree (sap);
    sap = next;
  }

  ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, 0, 0);

  return OM_MSG_RET_DONE;
}

static void SqnCleanUpSegGap (SeqAlignPtr sap)
{
   DenseSegPtr  dsp;
   DenseSegPtr  dsp_new;
   Boolean      found;
   Int4         i;
   Int4         j;
   Int4         k;
   Int4         numgap;

   if (sap == NULL || sap->segtype != SAS_DENSEG)
      return;
   dsp = (DenseSegPtr)(sap->segs);
   numgap = 0;
   for (i=0; i<dsp->numseg; i++)
   {
      found = FALSE;
      for (j=0; !found && j<dsp->dim; j++)
      {
         if (dsp->starts[i*dsp->dim+j] != -1)
            found = TRUE;
      }
      if (!found)
         numgap++;
   }
   if (numgap == 0)
      return;
   dsp_new = DenseSegNew();
   dsp_new->dim = dsp->dim;
   dsp_new->ids = dsp->ids;
   dsp->ids = NULL;
   dsp_new->numseg = dsp->numseg - numgap;
   dsp_new->starts = (Int4Ptr)MemNew(dsp_new->numseg*dsp_new->dim*sizeof(Int4));
   dsp_new->strands = (Uint1Ptr)MemNew(dsp_new->numseg*dsp_new->dim*sizeof(Uint1));
   dsp_new->lens = (Int4Ptr)MemNew(dsp_new->numseg*sizeof(Int4));
   k = 0;
   for (i=0; i<dsp->numseg; i++)
   {
      found = FALSE;
      for (j=0; !found && j<dsp->dim; j++)
      {
         if (dsp->starts[i*dsp->dim+j] != -1)
            found = TRUE;
      }
      if (found)
      {
         for (j=0; j<dsp_new->dim; j++)
         {
            dsp_new->starts[k*dsp_new->dim+j] = dsp->starts[i*dsp->dim+j];
            dsp_new->strands[k*dsp_new->dim+j] = dsp->strands[i*dsp->dim+j];
         }
         dsp_new->lens[k] = dsp->lens[i];
         k++;
      }
   }
   DenseSegFree(dsp);
   sap->segs = (Pointer)(dsp_new);
   return;
}

static void NoMoreSegGapForOneAlignment (SeqAlignPtr sap, Pointer userdata)
{
  SeqAlignPtr       salp;

  salp = sap;
  while (salp != NULL)
  {
    if (salp->saip != NULL)
    {
       SeqAlignIndexFree(salp->saip);
       salp->saip = NULL;
    } else {
       AlnMgr2IndexSingleChildSeqAlign(salp); /* make sure it's dense-seg */
       SeqAlignIndexFree(salp->saip);
       salp->saip = NULL;
    }
    SqnCleanUpSegGap(salp);
    AlnMgr2IndexSingleChildSeqAlign(salp);
    salp = salp->next;
  }
}


static Int2 LIBCALLBACK NoMoreSegGap (Pointer data)
{
  OMProcControlPtr  ompcp;
  SeqAlignPtr       sap;
  SeqAnnotPtr       sanp;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;
  switch (ompcp->input_itemtype) {
    case OBJ_SEQALIGN :
      sap = (SeqAlignPtr) ompcp->input_data;
      if (sap == NULL)
      {
        return OM_MSG_RET_ERROR;
      }
      else
      {
        NoMoreSegGapForOneAlignment (sap, NULL);
      }
      break;
    case OBJ_SEQANNOT :
      sanp = (SeqAnnotPtr) ompcp->input_data;
      if (sanp->type != 2)
      {
        return OM_MSG_RET_ERROR;
      }
      else if ((sap = (SeqAlignPtr)(sanp->data)) == NULL)
      {
        return OM_MSG_RET_ERROR;
      }
      else
      {
        NoMoreSegGapForOneAlignment (sap, NULL);
      }
      break;
    case OBJ_BIOSEQ :
      VisitAlignmentsOnBsp (ompcp->input_data, NULL, NoMoreSegGapForOneAlignment);
      break;
    case OBJ_BIOSEQSET :
      VisitAlignmentsInSet (ompcp->input_data, NULL, NoMoreSegGapForOneAlignment);
      break;
    case 0 :
      return OM_MSG_RET_ERROR;
      break;
    default :
      return OM_MSG_RET_ERROR;
      break;
  }
  ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, 0, 0);
  return OM_MSG_RET_DONE;
}

static void SqnSeqAlignDeleteInSeqEntryCallBack (SeqEntryPtr sep, Pointer mydata,
                                          Int4 index, Int2 indent)
{
  BioseqPtr          bsp;
  BioseqSetPtr       bssp;
  SeqAnnotPtr        sap,
                     pre;
  BoolPtr            dirtyp;
  
  if (sep != NULL && sep->data.ptrvalue && mydata != NULL) {
     dirtyp = (BoolPtr)mydata;
     if (IS_Bioseq(sep)) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        if (bsp!=NULL) {
           sap=bsp->annot;
           pre=NULL;
           while (sap) {
              if (sap->type == 2) {
                 if (pre==NULL) {
                    bsp->annot = sap->next;
                    sap->next=NULL;
                    sap = SeqAnnotFree (sap);
                    if (bsp->annot)
                       sap=bsp->annot->next;
                 }
                 else {
                    pre=sap->next;
                    sap->next=NULL;
                    sap = SeqAnnotFree (sap);
                    if (pre)
                       sap=pre->next;
                 }
                 *dirtyp=TRUE;
              }
              else {
                 pre=sap;
                 sap=sap->next;
              }
           }
        }
     }
     else if(IS_Bioseq_set(sep)) {
        bssp = (BioseqSetPtr)sep->data.ptrvalue;
        if (bssp!=NULL) {
           sap=bssp->annot;
           pre=NULL;
           while (sap) {
              if (sap->type == 2) {
                 if (pre==NULL) {
                    bssp->annot = sap->next;
                    sap->next=NULL;
                    sap = SeqAnnotFree (sap);
                    if (bssp->annot)
                       sap=bssp->annot->next;
                 }
                 else {
                    pre=sap->next;
                    sap->next=NULL;
                    sap = SeqAnnotFree (sap);
                    if (pre)
                       sap=pre->next;
                 }
                 *dirtyp=TRUE;
              }
              else {
                 pre=sap;
                 sap=sap->next;
              }
           }
        }
     }
  }
}

static Int2 LIBCALLBACK NewUpdateSeqAlign (Pointer data)

{
  AlignFileDataPtr  afdp;
  Char              path [PATH_MAX];
  FILE              *fp;
  OMProcControlPtr  ompcp;
  SeqAlignPtr       salp=NULL,
                    salpnew;
  SeqAnnotPtr       sap=NULL,
                    sapcopy;
  SeqEntryPtr       sep=NULL,
                    sepnew=NULL;
  Uint2             entityID,
                    itemID;
  MsgAnswer         ans;
  SeqSubmitPtr      ssp;
  Boolean           ok = TRUE, 
                    dirty = FALSE;
  ErrInfoPtr        eip;
  CharPtr           msg;
  Char              str [256];
  FILE              *tmpFp;
  Char              tmpPath [PATH_MAX];
  AliConfigInfo     configInfo;
   
  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;

  if (ompcp->input_data == NULL) return OM_MSG_RET_ERROR;

  switch(ompcp->input_itemtype)
    {
    case OBJ_BIOSEQ :
      sep = SeqMgrGetSeqEntryForData (ompcp->input_data);
      break;
    case OBJ_BIOSEQSET :
      sep = SeqMgrGetSeqEntryForData (ompcp->input_data);
      break;
    case OBJ_SEQENTRY :
      sep = ompcp->input_data;
      break;
    case OBJ_SEQSUB :
      ssp = ompcp->input_data;
      if(ssp->datatype==1)
         sep = (SeqEntryPtr)ssp->data;
      break;
    case 0 :
      return OM_MSG_RET_ERROR;
    default :
      return OM_MSG_RET_ERROR;
  }
  if (sep==NULL)
     return OM_MSG_RET_ERROR;
  entityID = ObjMgrGetEntityIDForChoice (sep);
  if (entityID < 1)
     return OM_MSG_RET_ERROR;

  if (newAlignReader) {
    if (GetInputFileName (path, sizeof (path), NULL, "TEXT")) {
      fp = FileOpen (path, "r");
      if (fp != NULL) {
        MemSet ((Pointer) &configInfo, 0, sizeof (AliConfigInfo));
        configInfo.gapChar = "-.";
        Ali_SetConfig (&configInfo, ALI_SET_GAP_CHAR);
        afdp = Ali_Read (fp);
        FileClose (fp);
        if (afdp != NULL) {
	  if (afdp->errors != NULL) {
	    TmpNam (tmpPath);
	    tmpFp = FileOpen (tmpPath, "w");
	    
	    for (eip = afdp->errors; eip != NULL; eip = eip->next) {
	      size_t   len;
	      if (eip->info == NULL) {
		fprintf (tmpFp, "ERROR: eip->info is NULL\n");
		continue;
	      }

	      /* Ignore "No source info" errors for updates */

	      if ((eip->errNum == ERR_DEFLINE_NODEFS) ||
		  (eip->errNum == ERR_GLOBAL_DEFLINE_NODEFS) ||
		  (eip->errNum == ERR_MULTI_DEFLINE_NODEFS))
		continue;

	      len = StringLen (eip->info) + 60;
	      msg = MemNew (len);
	      if (msg == NULL) continue;
	      if (eip->level == LEVEL_MULTI) {
		StringCpy (msg, "MULTIPLE ERRORS:");
	      } else if (eip->level == LEVEL_ERROR) {
		StringCpy (msg, "ERROR:");
	      } else if (eip->level == LEVEL_WARNING) {
		StringCpy (msg, "WARNING:");
	      } else if (eip->level == LEVEL_INFO) {
		  StringCpy (msg, "INFO:");
	      }
	      if (eip->rowNum != 0) {
		sprintf (str, " [Line %ld]", (long) eip->rowNum);
		StringCat (msg, str);
	      }
	      if (eip->info != NULL) {
		  StringCat (msg, " ");
		  StringCat (msg, eip->info);
	      }
	      fprintf (tmpFp, "%s\n\n", msg);
	      MemFree (msg);
	      }
	    FileClose (tmpFp);
	    LaunchGeneralTextViewer (tmpPath, "Sequence Alignment Update Errors");
	    FileRemove (tmpPath);
	  }
          sepnew = ALI_ConvertToNCBIData (afdp);
          Ali_Free (afdp);
          afdp = NULL;
        }
      }
    }
  } else {
    sepnew = ReadAnyAlignment (FALSE, NULL);
  }
  if (sepnew) {
     salpnew = (SeqAlignPtr) FindSeqAlignInSeqEntry (sepnew, OBJ_SEQALIGN);
     if (salpnew) {
        ok = ValidateSeqAlignandACCInSeqEntry (sepnew, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE);
        if (ok) {
           salp = (SeqAlignPtr) FindSeqAlignInSeqEntry (sep, OBJ_SEQALIGN);
           if (salp) {
              ans = Message (MSG_OKC, "Do you wish to replace (OK) or add (Cancel) the alignment in your SeqEntry?");
              if (ans == ANS_OK)
              {
                 SeqEntryExplore (sep, &dirty, SqnSeqAlignDeleteInSeqEntryCallBack);
              }
           }
           sap=SeqAnnotForSeqAlign(salpnew);
           sapcopy = (SeqAnnotPtr) AsnIoMemCopy (sap, (AsnReadFunc) SeqAnnotAsnRead, (AsnWriteFunc) SeqAnnotAsnWrite);
           SeqAlignAddInSeqEntry (sep, sapcopy);
           sap->data=NULL;
           MemFree(sap);
           ObjMgrSetDirtyFlag (entityID, TRUE);
           itemID = GetItemIDGivenPointer (entityID, OBJ_SEQENTRY, (Pointer) sep);
           ObjMgrSendMsg (OM_MSG_UPDATE, entityID, itemID, OBJ_SEQENTRY);
        }
     }
     ObjMgrFree (OBJ_SEQENTRY, (Pointer)sepnew);
     sepnew=NULL;
  }
  return OM_MSG_RET_OK;
}

typedef struct sqn_bsp {
   BioseqPtr  bsp;
   struct sqn_bsp PNTR next;
} SQNBsp, PNTR SQNBspPtr;

static void SQNGetBioseqs(SeqEntryPtr sep, Pointer userdata, Int4 index, Int2 indent)
{
   SQNBspPtr  sbp;

   if (sep == NULL || sep->data.ptrvalue == NULL || userdata == NULL)
      return;
   sbp = (SQNBspPtr)userdata;
   if (IS_Bioseq(sep))
   {
      if (sbp->bsp == NULL)
         sbp->bsp = (BioseqPtr)(sep->data.ptrvalue);
      else if (ISA_na(sbp->bsp->mol))
      {
         while (sbp->next != NULL)
         {
            sbp = sbp->next;
         }
         sbp->next = (SQNBspPtr)MemNew(sizeof(SQNBsp));
         sbp->next->bsp = (BioseqPtr)(sep->data.ptrvalue);
      }
   }
}

static void SQNGetBioseqsProt(SeqEntryPtr sep, Pointer userdata, Int4 index, Int2 indent)
{
   BioseqPtr  bsp;
   SQNBspPtr  sbp;

   if (sep == NULL || sep->data.ptrvalue == NULL || userdata == NULL)
      return;
   sbp = (SQNBspPtr)userdata;
   if (IS_Bioseq(sep))
      bsp = (BioseqPtr)(sep->data.ptrvalue);
   if (IS_Bioseq(sep) && ISA_aa(bsp->mol))
   {
      if (sbp->bsp == NULL)
         sbp->bsp = (BioseqPtr)(sep->data.ptrvalue);
      else
      {
         while (sbp->next != NULL)
         {
            sbp = sbp->next;
         }
         sbp->next = (SQNBspPtr)MemNew(sizeof(SQNBsp));
         sbp->next->bsp = (BioseqPtr)(sep->data.ptrvalue);
      }
   }
}

static Int2 LIBCALLBACK GenerateSeqAlignFromSeqEntry (Pointer data)

{
  BioseqPtr         bsp;
  BioseqSetPtr      bssp;
  SeqAnnotPtr       curr;
  DenseSegPtr       dsp;
  ObjectIdPtr       oip;
  OMProcControlPtr  ompcp;
  SeqAlignPtr       salp;
  SeqAlignPtr       salp_head;
  SeqAlignPtr       salp_mult;
  SeqAlignPtr       salp_prev;
  SeqAlignPtr       salp_tmp;
  SeqAnnotPtr       sap;
  SeqAnnotPtr PNTR  sapp;
  SQNBspPtr         sbp;
  SQNBspPtr         sbp_prev;
  SeqEntryPtr       sep;
  SeqIdPtr          sip1;
  SeqIdPtr          sip2;
  UserFieldPtr      ufp;
  UserObjectPtr     uop;
  Boolean           revcomp = FALSE;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;
  switch (ompcp->input_itemtype) {
    case OBJ_BIOSEQ :
      break;
    case OBJ_BIOSEQSET :
      break;
    case 0 :
      return OM_MSG_RET_ERROR;
    default :
      return OM_MSG_RET_ERROR;
  }
  if (ompcp->input_data == NULL) return OM_MSG_RET_ERROR;
  sep = SeqMgrGetSeqEntryForData (ompcp->input_data);
  if (sep == NULL) return OM_MSG_RET_ERROR;
  sbp = (SQNBspPtr)MemNew(sizeof(SQNBsp));
  SeqEntryExplore(sep, sbp, SQNGetBioseqs);
  bsp = sbp->bsp;
  sbp_prev = sbp;
  sbp = sbp->next;
  MemFree(sbp_prev);
  salp_head = salp_prev = NULL;
  while (sbp != NULL)
  {
    if (ISA_na(sbp->bsp->mol))
    {
       sip1 = SeqIdDup(bsp->id);
       sip2 = SeqIdDup(sbp->bsp->id);
       revcomp = FALSE;
       salp = Sqn_GlobalAlign2Seq(bsp, sbp->bsp, &revcomp);
       if (revcomp) {
         BioseqRevComp (sbp->bsp);
       }
       if (salp != NULL) {
         dsp = (DenseSegPtr)(salp->segs);
         SeqIdSetFree(dsp->ids);
         dsp->ids = sip1;
         dsp->ids->next = sip2;
         if (salp != NULL && salp_head != NULL) {
            salp_prev->next = salp;
            salp_prev = salp;
         } else if (salp != NULL) {
          salp_head = salp_prev = salp;
         }
       }
    }
    sbp_prev = sbp;
    sbp = sbp->next;
    MemFree(sbp_prev);
  }
  if (salp_head != NULL)
  {
    salp_tmp = salp_head;
    while (salp_tmp != NULL)
    {
       if (salp_tmp->saip != NULL)
       {
          SeqAlignIndexFree(salp_tmp->saip);
          salp_tmp->saip = NULL;
       }
       salp_tmp = salp_tmp->next;
    }
    AlnMgr2IndexSeqAlignEx(salp_head, FALSE);
    salp_mult = AlnMgr2GetSubAlign(salp_head, 0, -1, 0, TRUE);
    salp_mult->dim = AlnMgr2GetNumRows(salp_head);
    salp_mult->type = SAT_PARTIAL;
    SeqAlignSetFree(salp_head);
    sap = SeqAnnotForSeqAlign(salp_mult);
  } else
    sap = NULL;
  if (sap != NULL) {

    oip = ObjectIdNew ();
    oip->str = StringSave ("Hist Seqalign");
    uop = UserObjectNew ();
    uop->type = oip;

    oip = ObjectIdNew();
    oip->str = StringSave ("Hist Seqalign");
    ufp = UserFieldNew ();
    ufp->choice = 4;
    ufp->data.boolvalue = TRUE;
    ufp->label = oip;

    uop->data = ufp;
	ValNodeAddPointer (&(sap->desc), Annot_descr_user, (Pointer) uop);

    sep = GetTopSeqEntryForEntityID (ompcp->input_entityID);
    if (sep != NULL && sep->data.ptrvalue != NULL) {
      sapp = NULL;
      if (IS_Bioseq (sep)) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        sapp = &(bsp->annot);
      } else if (IS_Bioseq_set (sep)) {
        bssp = (BioseqSetPtr) sep->data.ptrvalue;
        sapp = &(bssp->annot);
      }
      if (sapp != NULL) {
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
      ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
      ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, 0, 0);
    }
  }
  return OM_MSG_RET_DONE;
}

static Int2 LIBCALLBACK GenerateSeqAlignFromSeqEntryProt (Pointer data)

{
  BioseqPtr         bsp;
  BioseqSetPtr      bssp;
  SeqAnnotPtr       curr;
  ObjectIdPtr       oip;
  OMProcControlPtr  ompcp;
  SeqAlignPtr       salp;
  SeqAlignPtr       salp_head;
  SeqAlignPtr       salp_mult;
  SeqAlignPtr       salp_prev;
  SeqAlignPtr       salp_tmp;
  SeqAnnotPtr       sap;
  SeqAnnotPtr PNTR  sapp;
  SQNBspPtr         sbp;
  SQNBspPtr         sbp_prev;
  SeqEntryPtr       sep;
  UserFieldPtr      ufp;
  UserObjectPtr     uop;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;
  switch (ompcp->input_itemtype) {
    case OBJ_BIOSEQ :
      break;
    case OBJ_BIOSEQSET :
      break;
    case 0 :
      return OM_MSG_RET_ERROR;
    default :
      return OM_MSG_RET_ERROR;
  }
  if (ompcp->input_data == NULL) return OM_MSG_RET_ERROR;
  sep = SeqMgrGetSeqEntryForData (ompcp->input_data);
  if (sep == NULL) return OM_MSG_RET_ERROR;
  sbp = (SQNBspPtr)MemNew(sizeof(SQNBsp));
  SeqEntryExplore(sep, sbp, SQNGetBioseqsProt);
  bsp = sbp->bsp;
  sbp_prev = sbp;
  sbp = sbp->next;
  MemFree(sbp_prev);
  salp_head = salp_prev = NULL;
  while (sbp != NULL)
  {
    salp = Sqn_GlobalAlign2Seq(bsp, sbp->bsp, FALSE);
    if (salp_head != NULL && salp != NULL)
    {
       salp_prev->next = salp;
       salp_prev = salp;
    } else if (salp != NULL)
       salp_head = salp_prev = salp;
    sbp_prev = sbp;
    sbp = sbp->next;
    MemFree(sbp_prev);
  }
  if (salp_head != NULL)
  {
    salp_tmp = salp_head;
    while (salp_tmp != NULL)
    {
       if (salp_tmp->saip != NULL)
       {
          SeqAlignIndexFree(salp_tmp->saip);
          salp_tmp->saip = NULL;
       }
       salp_tmp = salp_tmp->next;
    }
    AlnMgr2IndexSeqAlign(salp_head);
    salp_mult = AlnMgr2GetSubAlign(salp_head, 0, -1, 0, TRUE);
    salp_mult->dim = AlnMgr2GetNumRows(salp_head);
    salp_mult->type = SAT_PARTIAL;
    SeqAlignSetFree(salp_head);
    sap = SeqAnnotForSeqAlign(salp_mult);
  } else
    sap = NULL;
  if (sap != NULL) {

    oip = ObjectIdNew ();
    oip->str = StringSave ("Hist Seqalign");
    uop = UserObjectNew ();
    uop->type = oip;

    oip = ObjectIdNew();
    oip->str = StringSave ("Hist Seqalign");
    ufp = UserFieldNew ();
    ufp->choice = 4;
    ufp->data.boolvalue = TRUE;
    ufp->label = oip;

    uop->data = ufp;
	ValNodeAddPointer (&(sap->desc), Annot_descr_user, (Pointer) uop);

    sep = GetTopSeqEntryForEntityID (ompcp->input_entityID);
    if (sep != NULL && sep->data.ptrvalue != NULL) {
      sapp = NULL;
      if (IS_Bioseq (sep)) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        sapp = &(bsp->annot);
      } else if (IS_Bioseq_set (sep)) {
        bssp = (BioseqSetPtr) sep->data.ptrvalue;
        sapp = &(bssp->annot);
      }
      if (sapp != NULL) {
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
      ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
      ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, 0, 0);
    }
  }
  return OM_MSG_RET_DONE;
}

static Int2 LIBCALLBACK GenerateSeqAlignDiscFromSeqEntry (Pointer data)

{
  BioseqPtr            bsp;
  BioseqSetPtr         bssp;
  SeqAnnotPtr          curr;
  ObjectIdPtr          oip;
  OMProcControlPtr     ompcp;
  BLAST_OptionsBlkPtr  options;
  CharPtr              program;
  SeqAlignPtr          salp;
  SeqAlignPtr          salp_head;
  SeqAlignPtr          salp_mult;
  SeqAlignPtr          salp_prev;
  SeqAlignPtr          salp_tmp;
  SeqAnnotPtr          sap;
  SeqAnnotPtr          PNTR sapp;
  SQNBspPtr            sbp;
  SQNBspPtr            sbp_prev;
  SeqEntryPtr          sep;
  UserFieldPtr         ufp;
  UserObjectPtr        uop;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;
  switch (ompcp->input_itemtype) {
    case OBJ_BIOSEQ :
      break;
    case OBJ_BIOSEQSET :
      break;
    case 0 :
      return OM_MSG_RET_ERROR;
    default :
      return OM_MSG_RET_ERROR;
  }
  if (ompcp->input_data == NULL) return OM_MSG_RET_ERROR;
  sep = SeqMgrGetSeqEntryForData (ompcp->input_data);
  if (sep == NULL) return OM_MSG_RET_ERROR;
  sbp = (SQNBspPtr)MemNew(sizeof(SQNBsp));
  SeqEntryExplore(sep, sbp, SQNGetBioseqs);
  bsp = sbp->bsp;
  sbp_prev = sbp;
  sbp = sbp->next;
  MemFree(sbp_prev);
  salp_head = salp_prev = NULL;
  if (ISA_na(bsp->mol))
     program = StringSave("blastn");
  else
     program = StringSave("blastp");
  while (sbp != NULL)
  {
    options = BLASTOptionNew(program, TRUE);
    options->filter_string = StringSave("m L;R");
    salp = BlastTwoSequences(bsp, sbp->bsp, program, options);
    BLASTOptionDelete(options);
    if (salp != NULL)
    {
       AlnMgr2IndexLite(salp);
       SPI_RemoveInconsistentAlnsFromSet(salp, 0, 1, SPI_LEFT);
       salp_tmp = (SeqAlignPtr)(salp->segs);
       salp->segs = NULL;
       SeqAlignFree(salp);
       salp = salp_tmp;
    }
    if (salp != NULL && salp_head != NULL)
    {
       salp_prev->next = salp;
       salp_prev = salp;
    } else if (salp != NULL)
       salp_head = salp_prev = salp;
    while (salp_prev != NULL && salp_prev->next != NULL)
    {
       salp_prev = salp_prev->next;
    }
    sbp_prev = sbp;
    sbp = sbp->next;
    MemFree(sbp_prev);
  }
  if (salp_head != NULL)
  {
    salp_tmp = salp_head;
    while (salp_tmp != NULL)
    {
       if (salp_tmp->saip != NULL)
       {
          SeqAlignIndexFree(salp_tmp->saip);
          salp_tmp->saip = NULL;
       }
       salp_tmp = salp_tmp->next;
    }
    AlnMgr2IndexSeqAlign(salp_head);
    salp_mult = AlnMgr2GetSubAlign(salp_head, 0, -1, 0, FALSE);
    SeqAlignSetFree(salp_head);
    sap = SeqAnnotForSeqAlign(salp_mult);
  } else
    sap = NULL;
  if (sap != NULL) {

    oip = ObjectIdNew ();
    oip->str = StringSave ("Hist Seqalign");
    uop = UserObjectNew ();
    uop->type = oip;

    oip = ObjectIdNew();
    oip->str = StringSave ("Hist Seqalign");
    ufp = UserFieldNew ();
    ufp->choice = 4;
    ufp->data.boolvalue = TRUE;
    ufp->label = oip;

    uop->data = ufp;
        ValNodeAddPointer (&(sap->desc), Annot_descr_user, (Pointer) uop);

    sep = GetTopSeqEntryForEntityID (ompcp->input_entityID);
    if (sep != NULL && sep->data.ptrvalue != NULL) {
      sapp = NULL;
      if (IS_Bioseq (sep)) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        sapp = &(bsp->annot);
      } else if (IS_Bioseq_set (sep)) {
        bssp = (BioseqSetPtr) sep->data.ptrvalue;
        sapp = &(bssp->annot);
      }
      if (sapp != NULL) {
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
      ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
      ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, 0, 0);
    }
  }
  return OM_MSG_RET_DONE;
}

static Boolean RawSeqLaunchFunc (GatherContextPtr gcp)

{
  BioseqPtr  bsp;
  Int2       handled;

  if (gcp == NULL) return TRUE;
  bsp = (BioseqPtr) gcp->userdata;
  if (bsp == NULL) return TRUE;
  if (gcp->thistype == OBJ_BIOSEQ) {
    if (bsp == (BioseqPtr) gcp->thisitem) {
      WatchCursor ();
      handled = GatherProcLaunch (OMPROC_EDIT, FALSE, gcp->entityID, gcp->itemID,
                                  OBJ_BIOSEQ, 0, 0, OBJ_BIOSEQ, 0);
      ArrowCursor ();
      if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
        /*
        Message (MSG_ERROR, "Unable to launch editor on sequence.");
        */
      }
      return FALSE;
    }
  }
  return TRUE;
}

extern Int2 LIBCALLBACK BioseqSegEditFunc (Pointer data)

{
  BioseqPtr         bsp;
  GatherScope       gs;
  SeqIdPtr          sip;
  SeqLocPtr         slp = NULL;
  OMProcControlPtr  ompcp;

  ompcp = (OMProcControlPtr) data;
  slp = NULL;
  if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;
  switch (ompcp->input_itemtype) {
    case OBJ_BIOSEQ_SEG :
      slp = (SeqLocPtr) ompcp->input_data;
      break;
   case 0 :
      return OM_MSG_RET_ERROR;
    default :
      return OM_MSG_RET_ERROR;
  }
  if (slp == NULL) return OM_MSG_RET_ERROR;
  sip = SeqLocId (slp);
  if (sip == NULL) return OM_MSG_RET_ERROR;
  bsp = BioseqFind (sip);
  if (bsp == NULL) return OM_MSG_RET_ERROR;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = TRUE;
  MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  GatherEntity (ompcp->input_entityID, (Pointer) bsp, RawSeqLaunchFunc, &gs);

  return OM_MSG_RET_DONE;
}

static Int2 LIBCALLBACK CreateLocMsgFunc (OMMsgStructPtr ommsp)
{
  return OM_MSG_RET_OK;
}

static SeqLocPtr SeqLocCopyOne (SeqLocPtr slp)
{
  SeqLocPtr   slpnew, slptemp;

  slptemp = slp->next;
  slp->next = NULL;
  slpnew = AsnIoMemCopy ((Pointer) slp, (AsnReadFunc) SeqLocAsnRead,
                         (AsnWriteFunc) SeqLocAsnWrite);
  slp->next = slptemp;
  return slpnew;
}

extern SeqFeatPtr SeqFeatCopy (SeqFeatPtr sfp)
{
  SeqFeatPtr  sfpnew;

  sfpnew = AsnIoMemCopy ((Pointer) sfp, (AsnReadFunc) SeqFeatAsnRead,
                         (AsnWriteFunc) SeqFeatAsnWrite);
  return sfpnew;
}

static Boolean ExplodeGroup (SeqEntryPtr sep, SeqFeatPtr sfp)

{
  SeqFeatPtr     sfpnew, sfpold, sfplast;
  ImpFeatPtr     ifp;
  SeqLocPtr      slphead, slp;
  GBQualPtr      gbq;
  Int2           count;
  Char           str [16];

  if (sfp == NULL || sfp->location == NULL) return FALSE;

/* save the seqloc (chain) */
  slphead = sfp->location;
  slp = SeqLocFindNext (slphead, NULL);
  if (slp == NULL) return FALSE;

/* trash the loc info in the impfeat */
  if (sfp->data.choice == SEQFEAT_IMP) {
    ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
    if (ifp != NULL) {
      ifp->loc = MemFree (ifp->loc);
    }
  }

/* orig sfp is copied then orig sfp data is replaced */
  sfplast = sfp->next;
  sfpnew = SeqFeatCopy (sfp);

/* if exon, increment /number qualifier */
  gbq = NULL;
  count = 0;
  if (sfpnew != NULL && sfpnew->data.choice == SEQFEAT_IMP) {
    ifp = (ImpFeatPtr) sfpnew->data.value.ptrvalue;
    if (ifp != NULL) {
      if (StringICmp (ifp->key, "exon") == 0) {
        gbq = sfpnew->qual;
        while (gbq != NULL && StringICmp (gbq->qual, "number") != 0) {
          gbq = gbq->next;
        }
        if (gbq != NULL) {
          if (! StrToInt (gbq->val, &count)) {
            gbq = NULL;
          }
        }
      }
    }
  }

  sfp->location = SeqLocCopyOne (slp);
  sfpold = sfp;
  slp = SeqLocFindNext (slphead, slp);

/* clone as many more as required */
  while (slp != NULL) {
    if (slp->choice != SEQLOC_NULL) {
      if (gbq != NULL) {
        gbq->val = MemFree (gbq->val);
        count++;
        sprintf (str, "%d", (int) count);
        gbq->val = StringSave (str);
      }
      sfp = SeqFeatCopy (sfpnew);
      sfp->location = SeqLocFree (sfp->location);
      sfp->location = SeqLocCopyOne (slp);
      sfp->partial = CheckSeqLocForPartial (sfp->location, NULL, NULL);
      sfpold->next = sfp;
      sfpold = sfp;
    }
    slp = SeqLocFindNext (slphead, slp);
  }
  sfpold->next = sfplast;
  sfpnew = SeqFeatFree (sfpnew);
  slphead = SeqLocFree (slphead);
  return TRUE;
}

static Int2 LIBCALLBACK GroupExplodeFunc (Pointer data)

{
  OMProcControlPtr  ompcp;
  SelStructPtr      ssp;
  Boolean           isDirty = FALSE;
  Boolean           isFirstSsp;
  ExplodeStructPtr  esp;
  ExplodeStructPtr  firstEsp = NULL;
  ExplodeStructPtr  lastEsp;
  Boolean           isFirstEsp;

  /* Check the parameter */

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->input_itemtype == 0)
    return OM_MSG_RET_ERROR;

  /* Get the linked of list of selected items */

  ssp  = ObjMgrGetSelected();

  /* Go through the list and save pointers */
  /* to the items themselves.              */

  isFirstEsp = TRUE;
  isFirstSsp = TRUE;

  while (NULL != ssp) {

    if (!isFirstSsp) {
      ompcp->input_entityID = ssp->entityID;
      ompcp->input_itemID   = ssp->itemID;
      ompcp->input_itemtype = ssp->itemtype;
      
      GatherDataForProc (ompcp, FALSE);
    }

    switch (ssp->itemtype)
      {
      case OBJ_SEQFEAT:

	esp = (ExplodeStructPtr) MemNew (sizeof (ExplodeStruct));
	esp->seqFeatPtr = (SeqFeatPtr) ompcp->input_data;
	esp->topSep     = GetTopSeqEntryForEntityID (ssp->entityID);
	
	if (isFirstEsp) {
	  firstEsp = esp;
	  isFirstEsp = FALSE;
	}
	else
	  lastEsp->next = esp;

	lastEsp = esp;
	lastEsp->next = NULL;
	break;
      default:
	break;
      }

    isFirstSsp = FALSE;
    ssp = ssp->next;
  }  

  /* Loop through all the selected items */
  /* and explode each one.               */

  esp = firstEsp;
  while (NULL != esp) {
    if (ExplodeGroup (esp->topSep, esp->seqFeatPtr))
      isDirty = TRUE;
    esp = esp->next;
  }

  /* If any actual exploding was done then */
  /* force an update to be done.           */

  if (isDirty)
    {
      ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
      ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID,
		     ompcp->input_itemID, ompcp->input_itemtype);
      return OM_MSG_RET_DONE;
    }
  else
    return OM_MSG_RET_ERROR;
}

extern void GroupExplodeToolBtn (ButtoN b)
{
  BaseFormPtr       bfp;
  SelStructPtr      ssp;
  Boolean           isDirty = FALSE;
  SeqEntryPtr       sep;
  SeqFeatPtr        sfp;
  SeqMgrFeatContext context;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;

  ssp  = ObjMgrGetSelected();
  while (NULL != ssp) {
    if (ssp->itemtype == OBJ_SEQFEAT)
    {
      sep = GetTopSeqEntryForEntityID (ssp->entityID);
    
      sfp = SeqMgrGetDesiredFeature (ssp->entityID, NULL, ssp->itemID, 0, NULL, &context);
      if (sfp != NULL && ExplodeGroup (sep, sfp))
      {
        isDirty = TRUE;
      }
    }
    ssp = ssp->next;
  }

  /* If any actual exploding was done then */
  /* force an update to be done.           */

  if (isDirty)
  {
    ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID,
		     bfp->input_itemID, bfp->input_itemtype);
  }
}

static Boolean MakeExonsAndIntronsFromFeature (SeqEntryPtr sep, BioseqPtr bsp,
                                               SeqLocPtr location,
                                               SeqFeatPtr putafterhere,
                                               Boolean MakeIntrons,
                                               Int4 first_exon_number)

{
  SeqFeatPtr  curr;
  Boolean     first;
  Int2        fuzz_from;
  Int2        fuzz_to;
  ImpFeatPtr  ifp;
  Int4        last;
  SeqLocPtr   next;
  SeqFeatPtr  putbeforehere;
  SeqFeatPtr  sfp;
  SeqLocPtr   slp;
  Int4        start;
  Int4        stop;
  Uint1       strand;
  Int4        tmp;
  Boolean     partial5, partial3;
  GBQualPtr   gbqual;
  Int4        part_number;
  Char        number_text [256];

  if (sep == NULL || bsp == NULL || location == NULL || putafterhere == NULL) return FALSE;
  putbeforehere = putafterhere->next;
  curr = putafterhere;
  slp = SeqLocFindNext (location, NULL);
  if (slp == NULL) return FALSE;
  first = TRUE;
  last = 0;
  part_number = first_exon_number;
  while (slp != NULL) {
    CheckSeqLocForPartial (slp, &partial5, &partial3);
    next = SeqLocFindNext (location, slp);
    if (slp->choice != SEQLOC_NULL) {
      start = GetOffsetInBioseq (slp, bsp, SEQLOC_START);
      stop = GetOffsetInBioseq (slp, bsp, SEQLOC_STOP);
      strand = SeqLocStrand (slp);
      if (strand > Seq_strand_both_rev && strand != Seq_strand_other) {
        strand = Seq_strand_unknown;
      }
      fuzz_from = -1;
      fuzz_to = -1;
      if (start > stop) {
        tmp = start;
        start = stop;
        stop = tmp;
      }
      if (! first && MakeIntrons) {
        sfp = SeqFeatNew ();
        if (sfp != NULL) {
          sfp->data.choice = SEQFEAT_IMP;
          if (strand == Seq_strand_minus) {
            AddIntToSeqFeat (sfp, stop + 1, last - 1, bsp,
                             fuzz_from, fuzz_to, strand);
          } else {
            AddIntToSeqFeat (sfp, last + 1, start - 1, bsp,
                             fuzz_from, fuzz_to, strand);
          }
          ifp = ImpFeatNew ();
          if (ifp != NULL) {
            sfp->data.value.ptrvalue = (Pointer) ifp;
            ifp->key = StringSave ("intron");
          }
          gbqual = GBQualNew ();
          if (gbqual != NULL)
          {
            sprintf (number_text, "%d", part_number - 1);
            gbqual->qual = StringSave ("number");
            gbqual->val = StringSave (number_text);
            gbqual->next = sfp->qual;
            sfp->qual = gbqual;
          }
          curr->next = sfp;
          curr = sfp;
        }
      }
      first = FALSE;
      if (strand == Seq_strand_minus) {
        last = start;
      } else {
        last = stop;
      }
      sfp = SeqFeatNew ();
      if (sfp != NULL) {
        sfp->data.choice = SEQFEAT_IMP;
        AddIntToSeqFeat (sfp, start, stop, bsp,
                         fuzz_from, fuzz_to, strand);
        ifp = ImpFeatNew ();
        if (ifp != NULL) {
          sfp->data.value.ptrvalue = (Pointer) ifp;
          ifp->key = StringSave ("exon");
        }
        SetSeqLocPartial (sfp->location, partial5, partial3);
        gbqual = GBQualNew ();
        if (gbqual != NULL)
        {
          sprintf (number_text, "%d", part_number);
          gbqual->qual = StringSave ("number");
          gbqual->val = StringSave (number_text);
          gbqual->next = sfp->qual;
          sfp->qual = gbqual;
        }
        part_number ++;
        curr->next = sfp;
        curr = sfp;
      }
    }
    slp = next;
  }
  curr->next = putbeforehere;
  return TRUE;
}

typedef struct makeexondata {
  FEATURE_FORM_BLOCK

  ButtoN      make_introns_button;
  TexT        exon_number_field;
  ButtoN      accept;

  SeqEntryPtr sep;
  Boolean     make_introns;
  Int4        first_exon_number;
  Uint1       feature_type;
} MakeExonData, PNTR MakeExonPtr;

static void MakeExonsFromFeatureIntervalsVisitFunc (SeqFeatPtr sfp, Pointer userdata)
{
  MakeExonPtr mep;
  BioseqPtr   bsp;

  if (sfp == NULL || (mep = (MakeExonPtr) userdata) == NULL || sfp->idx.subtype != mep->feature_type)
  {
    return;
  }

  mep = (MakeExonPtr) userdata;
  bsp = BioseqFindFromSeqLoc (sfp->location);

  MakeExonsAndIntronsFromFeature (mep->sep, bsp, sfp->location, sfp,
      mep->make_introns, mep->first_exon_number);
  
}

static void DoMakeExonsFromFeatureIntervals (ButtoN b)
{
  MakeExonPtr mep;
  Char        exon_number_str [256];

  if (b == NULL || (mep = (MakeExonPtr) GetObjectExtra (b)) == NULL) return;

  Hide (mep->form);

  WatchCursor ();
  Update ();

  mep->sep = GetTopSeqEntryForEntityID (mep->input_entityID);
  mep->make_introns = GetStatus (mep->make_introns_button);
  GetTitle (mep->exon_number_field,
            exon_number_str,
            sizeof (exon_number_str) - 1 );
  mep->first_exon_number = atoi (exon_number_str);
  VisitFeaturesInSep (mep->sep, mep, 
                      MakeExonsFromFeatureIntervalsVisitFunc);
  ObjMgrSetDirtyFlag (mep->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, mep->input_entityID, 0, 0);
  ArrowCursor ();
  Update ();
}

static void CheckExonNumberText (TexT number_field)
{
  MakeExonPtr mep;
  Char        exon_number_str [256];
  CharPtr     cp;

  if (number_field == NULL || (mep = (MakeExonPtr)GetObjectExtra (number_field)) == NULL) return;

  GetTitle (mep->exon_number_field,
            exon_number_str,
            sizeof (exon_number_str) - 1 );
  if (exon_number_str [0] == 0) 
  {
    Disable (mep->accept);
    return;
  }

  for (cp = exon_number_str; cp != NULL && *cp != 0; cp++)
  {
    if (*cp != '0' && *cp != '1' && *cp != '2' && *cp != '3'
      && *cp != '4' && *cp != '5' && *cp != '6' && *cp != '7'
      && *cp != '8' && *cp != '9')
    {
      Disable (mep->accept);
      return;
    }
  }
  Enable (mep->accept);
  return;
}
 
static void CommonMakeExonsFromFeatureIntervals (
  IteM i,
  Boolean make_introns,
  Uint1 feature_type
)
{
  BaseFormPtr  bfp;
  MakeExonPtr  mep;
  WindoW       w;
  GrouP        h, p, c;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif

  if (bfp == NULL) return;

  mep = MemNew (sizeof (MakeExonData));
  if (mep == NULL) return;
  mep->input_entityID = bfp->input_entityID;
  mep->feature_type = feature_type;

  if (feature_type == FEATDEF_CDS)
  {
    w = FixedWindow (-50, -33, -10, -10, "Make Exons from CDS", NULL);
  }
  else if (feature_type == FEATDEF_mRNA)
  {
    w = FixedWindow (-50, -33, -10, -10, "Make Exons from mRNA", NULL);
  }
  else
  {
    w = FixedWindow (-50, -33, -10, -10, "Make Exons from Feature", NULL);
  }

  SetObjectExtra (w, mep, StdCleanupFormProc);
  mep->form = (ForM) w;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  p = HiddenGroup (h, 2, 0, NULL);
  StaticPrompt (p, "First Exon Number", 0, 0, programFont, 'c');
  mep->exon_number_field = DialogText (p, "1", 3, CheckExonNumberText);
  SetObjectExtra (mep->exon_number_field, mep, NULL);
  mep->make_introns_button = CheckBox (p, "Make Introns", NULL);

  c = HiddenGroup (h, 4, 0, NULL);
  mep->accept = DefaultButton (c, "Accept", DoMakeExonsFromFeatureIntervals);
  SetObjectExtra (mep->accept, mep, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);
  AlignObjects (ALIGN_CENTER, (HANDLE) p, (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Update ();
}

extern void MakeExonsFromCDSIntervals (IteM i)
{
  CommonMakeExonsFromFeatureIntervals (i, FALSE, FEATDEF_CDS);
}

extern void MakeExonsFromMRNAIntervals (IteM i)
{
  CommonMakeExonsFromFeatureIntervals (i, TRUE, FEATDEF_mRNA);
}

static Int2 LIBCALLBACK MakeExonIntron (Pointer data)

{
  BioseqPtr         nbsp;
  SeqEntryPtr       nsep;
  OMProcControlPtr  ompcp;
  SeqFeatPtr        sfp;
  SeqEntryPtr       sep;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->input_itemtype == 0 || ompcp->input_data == NULL)
    return OM_MSG_RET_ERROR;

  switch (ompcp->input_itemtype)
  {
    case OBJ_SEQFEAT:
      sfp = (SeqFeatPtr) ompcp->input_data;
      if (sfp->data.choice != SEQFEAT_CDREGION && sfp->data.choice != SEQFEAT_RNA) {
        return OM_MSG_RET_ERROR;
      }
      break;
    default:
      return OM_MSG_RET_ERROR;
  }

  sep = GetBestTopParentForItemID (ompcp->input_entityID,
                                   ompcp->input_itemID,
                                   ompcp->input_itemtype);
  nsep = FindNucSeqEntry (sep);
  if (nsep == NULL || nsep->choice != 1) return OM_MSG_RET_ERROR;
  nbsp = (BioseqPtr) nsep->data.ptrvalue;
  if (nbsp == NULL) return OM_MSG_RET_ERROR;

  if (MakeExonsAndIntronsFromFeature (sep, nbsp, sfp->location, sfp, TRUE, 1))
  {
    ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, ompcp->input_itemID,
                   ompcp->input_itemtype);
    return OM_MSG_RET_DONE;
  }
  else
    return OM_MSG_RET_ERROR;
}

static Int2 LIBCALLBACK ProtLocalIDtoGeneSyn (Pointer data)

{
  BioseqPtr          bsp = NULL;
  Char               buf [41];
  SeqMgrFeatContext  ccontext;
  SeqFeatPtr         cds;
  SeqMgrFeatContext  gcontext;
  SeqFeatPtr         gene;
  GeneRefPtr         grp;
  OMProcControlPtr   ompcp;
  BioseqPtr          pbsp;
  SeqEntryPtr        sep;
  SeqIdPtr           sip;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->input_itemtype == 0 || ompcp->input_data == NULL)
    return OM_MSG_RET_ERROR;

  switch (ompcp->input_itemtype)
  {
    case OBJ_BIOSEQ:
      bsp = (BioseqPtr) ompcp->input_data;
      break;
    default:
      return OM_MSG_RET_ERROR;
  }

  if (bsp == NULL) {
    return OM_MSG_RET_ERROR;
  }
  sep = SeqMgrGetSeqEntryForData (bsp);
  if (sep == NULL) {
    return OM_MSG_RET_ERROR;
  }

  cds = NULL;
  while ((cds = SeqMgrGetNextFeature (bsp, cds, SEQFEAT_CDREGION, 0, &ccontext)) != NULL) {
    sip = SeqLocId (cds->product);
    if (sip != NULL) {
      pbsp = BioseqFind (sip);
      if (pbsp != NULL) {
        sip = SeqIdFindBest (pbsp->id, SEQID_LOCAL);
        if (sip != NULL) {
          SeqIdWrite (sip, buf, PRINTID_REPORT, sizeof (buf) - 1);
          grp = SeqMgrGetGeneXref (cds);
          if (grp != NULL && SeqMgrGeneIsSuppressed (grp)) {
            continue;
          }
          if (grp == NULL) {
            gene = SeqMgrGetOverlappingGene (cds->location, &gcontext);
            if (gene == NULL) {
              gene = CreateNewFeature (sep, NULL, SEQFEAT_GENE, NULL);
              if (gene != NULL) {
                grp = GeneRefNew ();
                gene->data.value.ptrvalue = (Pointer) grp;
                gene->location = SeqLocFree (gene->location);
                gene->location = AsnIoMemCopy ((Pointer) cds->location,
                                               (AsnReadFunc) SeqLocAsnRead,
                                               (AsnWriteFunc) SeqLocAsnWrite);
              }
            }
            if (gene != NULL) {
              grp = (GeneRefPtr) gene->data.value.ptrvalue;
            }
          }
          if (grp != NULL) {
            ValNodeCopyStr (&(grp->syn), 0, buf);
          }
        }
      }
    }
  }

  BasicSeqEntryCleanup (sep);
  SeriousSeqEntryCleanup (sep, NULL, NULL);

  ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, ompcp->input_itemID,
                 ompcp->input_itemtype);
  return OM_MSG_RET_DONE;
}

#define BLACK     0
#define RED       4
#define GREEN     2
#define BLUE      1
#define CYAN      3
#define MAGENTA   5
#define YELLOW    6
#define WHITE    15
#define GRAY      8
#define LTGRAY    7

#define DKCYAN   21
#define DKGREEN  22
#define DKBLUE  23

#define ORF_LENGTH 10


typedef struct orfviewform
{
	FORM_MESSAGE_BLOCK
	IcoN 		icon;
	WindoW 		w;
	DoC 		doc;
	BioseqPtr	bsp;
	Int2		gcode;
	ValNodePtr	orfs;
	Int2 		frame, strand;
	Int4 		from, to;
	double 		dx, dy;
	RecT		mi;
	Boolean		orf_only;
	SeqLocPtr	select_orf;
	Uint2 		bsp_entityID;
	Uint2 		bsp_itemID;
	Uint2 		len;		/* minimum length of the ORF shown */
	Boolean     standAlone;
	Boolean     alt_start;
} OrfViewForm, PNTR OrfViewFormPtr;

static RecT mi0 = {  24,  56, 460, 200 };

Uint1 AAForCodon (Uint1Ptr codon, CharPtr codes); /* in seqport.c */

static void dkCyan (void)
{
	SelectColor(0, 203, 196);
}

static void dkBlue (void)
{
	SelectColor(0, 0, 196);
}

static void dkGreen (void)
{
	SelectColor(0, 203, 0);
}

static void SetIntColor (int color)
{
	switch (color) {
		case BLACK: Black(); break;
		case RED: Red(); break;
		case GREEN: Green(); break;
		case BLUE: Blue(); break;
		case CYAN: Cyan(); break;
		case MAGENTA: Magenta(); break;
		case YELLOW: Yellow(); break;
		case WHITE: White(); break;
		case GRAY: Gray(); break;
		case LTGRAY: LtGray(); break;
		case DKCYAN: dkCyan(); break;
		case DKBLUE: dkBlue(); break;
		case DKGREEN: dkGreen(); break;
	}
}

static void Frame2Rect (RecT PNTR a, int color1, int color2)
{
	MoveTo(a->left, a->top);
	SetIntColor(color1);
	LineTo(a->right, a->top);
	LineTo(a->right, a->bottom);
	SetIntColor(color2);
	LineTo(a->left, a->bottom);
	LineTo(a->left, a->top);
}

static void Frame3d (RecT PNTR a)
{
	int i, in = 3;
	
	SetIntColor(LTGRAY);
	PaintRect(a);
	for (i=0; i < in; i++) {
		if (i > 0) {
			InsetRect(a, 1, 1);
		}
		Frame2Rect(a, WHITE, GRAY);
	}
	for (i=0; i < in; i++) {
		InsetRect(a, 1, 1);
	}
	for (i=0; i < in; i++) {
		InsetRect(a, 1, 1);
		Frame2Rect(a, GRAY, WHITE);
	}
	SelectColor(0, 203, 196);
	PaintRect(a);
}

static void DrawColorLine (int x0, int y0, int x1, int y1, int color)
{
	SetIntColor(color);
	MoveTo(x0, y0);
	LineTo(x1, y1);
}

static void Rect3d (RecT PNTR a, int color)
{
	Frame2Rect(a, GRAY, WHITE);
	InsetRect(a, 1, 1);
	Frame2Rect(a, GRAY, WHITE);
	InsetRect(a, 1, 1);
	if (color != -1) {
		SetIntColor(color);
		PaintRect(a);
	}
}



static void OrfQuitProc (ButtoN b)

{
  QuitProgram ();
}

static void CloseProc (ButtoN b)
{ 
	WindoW   w;

    w = ParentWindow (b);
    Remove (w);
}

static void draw_rect(SeqPortPtr spp, Int2 ir, RecT PNTR frect, CharPtr vals, CharPtr codes, Boolean paint, OrfViewFormPtr ovp, Int2 strand)
{
	Int4 pos;
	Uint1 codon[3], aa;
	Int4 len;
	double x;
	RecT r;
	
    	SeqPortSeek(spp, ir, SEEK_SET);
		frect->bottom = frect->top + 15;
		len = spp->totlen;
		if (paint) {
			dkGreen();
		} else {
			dkCyan();
		}
		dkCyan();
		PaintRect(frect);
		Black();
		FrameRect(frect);
		for (pos=0; pos < len-2; pos += 3) {
			codon[0] = SeqPortGetResidue(spp);
			codon[1] = SeqPortGetResidue(spp);
			codon[2] = SeqPortGetResidue(spp);
			aa = AAForCodon(codon, codes);
			if (aa == '*') {
				if (strand == Seq_strand_plus) {
					x = frect->left + (pos+ir)*ovp->dx;
				} else {
					x = frect->left + (len+2-(pos+ir))*ovp->dx;
				}
				DrawColorLine(x, frect->top+1, x, frect->bottom-1, RED);
				DrawColorLine(x+1, frect->top+1, x+1, frect->bottom-1, RED);
			}
			if (ovp->alt_start == TRUE) {
				aa = AAForCodon(codon, vals);
			}
			if (aa == 'M') {
				if (strand == Seq_strand_plus) {
					x = frect->left + (pos+ir)*ovp->dx;
				} else {
					x = frect->left + (len+2-(pos+ir))*ovp->dx;
				}
				DrawColorLine(x, frect->top+1, x, frect->bottom-1, WHITE);
				DrawColorLine(x+1, frect->top+1, x+1, frect->bottom-1, WHITE);
			}
		}
		if (paint) {
			r.top = frect->top + 1;
			r.bottom = frect->bottom - 1;
			r.left = frect->left + ovp->from*ovp->dx + 2;
			r.right = frect->left + ovp->to*ovp->dx - 2;
			Magenta();
			PaintRect(&r);
		}
		frect->top = frect->bottom + 4;
}

static void draw_frame(Int2 ir, RecT PNTR frect, Boolean paint, OrfViewFormPtr ovp, Int2 strand)
{
	RecT r;  /* for ORF */
	ValNodePtr vnp;
	SeqLocPtr slp;
	SeqIntPtr sip;

	frect->bottom = frect->top + 15;
	LtGray();
	PaintRect(frect);
	Black();
	FrameRect(frect);
	for (vnp = ovp->orfs; vnp; vnp=vnp->next) {
		if (vnp->choice == ir) {
			slp = vnp->data.ptrvalue;
			if (slp == NULL) {
				continue;
			}
			sip = slp->data.ptrvalue;
			if (sip == NULL) {
				continue;
			}
			if (sip->strand == strand) {
				r.top = frect->top + 1;
				r.bottom = frect->bottom - 1;
				r.left = frect->left + sip->from*ovp->dx;
				r.right = frect->left + sip->to*ovp->dx;
				dkCyan();
				PaintRect(&r);
				Black();
				r.top = frect->top;
				r.bottom = frect->bottom;
				FrameRect(&r);
			}
			if (paint) {
				r.top = frect->top + 1;
				r.bottom = frect->bottom - 1;
				r.left = frect->left + ovp->from*ovp->dx + 2;
				r.right = frect->left + ovp->to*ovp->dx - 2;
				Magenta();
				PaintRect(&r);
			}
		}
	}
	frect->top = frect->bottom + 4;
	return;
}

static void draw_strands(OrfViewFormPtr ovp)
{
	Int2 ir, gcode;
	RecT frect;
	SeqPortPtr spp;
	GeneticCodePtr gcp;
	CharPtr vals, codes;
	ValNodePtr vnp;
	Boolean paint;
	RecT PNTR r;
	
	r = &(ovp->mi);
	gcode = ovp->gcode;
	gcp = GeneticCodeFind(gcode, NULL);   /* use universal */
	vals = NULL;
	codes = NULL;
	for (vnp = (ValNodePtr)gcp->data.ptrvalue; vnp != NULL; vnp = vnp->next)
	{
		if (vnp->choice == 6)   /* sncbieaa */
			vals = (CharPtr)vnp->data.ptrvalue;
		else if (vnp->choice == 3)  /* ncbieaa */
			codes = (CharPtr)vnp->data.ptrvalue;
	}
	if (vals == NULL) {
		vals = codes;
	}
	frect.left = r->left + 11;
	frect.right = r->right - 11;
	frect.top = r->top + 4;
	ovp->dx = (frect.right - frect.left - 1.) / (ovp->bsp)->length;
	ovp->dy = (r->bottom - r->top - 1.) / 6.;
	spp = SeqPortNew(ovp->bsp, 0, -1, Seq_strand_plus, Seq_code_ncbi4na);
	for (ir=0; ir < 3; ir++) {
		if (ovp->from == 0 && ovp->to == 0) {
			paint = FALSE;
		} else if (ovp->strand == Seq_strand_plus && ovp->frame == ir) {
			paint = TRUE;
		} else {
			paint = FALSE;
		}
		if (ovp->orf_only) {
			draw_frame(ir, &frect, paint, ovp, Seq_strand_plus);
		} else {
			draw_rect(spp, ir, &frect, vals, codes, paint, ovp, Seq_strand_plus);
		}
	}
	SeqPortFree(spp);
	spp = SeqPortNew(ovp->bsp, 0, -1, Seq_strand_minus, Seq_code_ncbi4na);
	frect.top += 7;
	for (ir=0; ir < 3; ir++) {
		if (ovp->from == 0 && ovp->to == 0) {
			paint = FALSE;
		} else if (ovp->strand == Seq_strand_minus && ovp->frame == ir) {
			paint = TRUE;
		} else {
			paint = FALSE;
		}
		if (ovp->orf_only) {
			draw_frame(ir, &frect, paint, ovp, Seq_strand_minus);
		} else {
			draw_rect(spp, ir, &frect, vals, codes, paint, ovp, Seq_strand_minus);
		}
	}
	
	SeqPortFree(spp);
}

static void DrawIcon(IcoN ic0)
{
	RecT r;
	OrfViewFormPtr ovp;

	ovp = (OrfViewFormPtr) GetObjectExtra (ic0);
	ObjectRect(ovp->icon, &r);
	ovp->mi.left = mi0.left + r.left;
	ovp->mi.right = mi0.right + r.left;
	ovp->mi.top = mi0.top + r.top;
	ovp->mi.bottom = mi0.bottom + r.top;
	Frame3d(&r);
	Rect3d(&(ovp->mi), LTGRAY);
	draw_strands(ovp);
}

static void notify( DoC d, Int2 item, Int2 raw, Int2 col, Boolean event)
{
	ValNodePtr vnp;
	SeqLocPtr slp, slptmp;
	SeqIntPtr sip;
	Int2 i;
	Int2	itemOld1;
	Int2	itemOld2;
	Boolean status;
	OrfViewFormPtr ovp;
	Int2 top, bottom;
	BaR sb;
	Int2 startsAt;
	
	if( item == 0) {
		return;
	}
	ovp = (OrfViewFormPtr) GetObjectExtra (d);
	for (vnp = ovp->orfs, i = 1; i < item && vnp; i++, vnp=vnp->next) continue;
	if (vnp == NULL) {
		return;
	}
	if (ItemIsVisible (d, item, &top, &bottom, NULL) == FALSE) {
      GetItemParams (d, item, &startsAt, NULL, NULL, NULL, NULL);
      sb = GetSlateVScrollBar ((SlatE) d);
      CorrectBarValue (sb, startsAt);
	}
    GetDocHighlight(d, &itemOld1, &itemOld2);
	SetDocHighlight(d, item, item);
	UpdateDocument(d, itemOld1, itemOld2);
	UpdateDocument(d, item, item);

	ovp->frame = vnp->choice;
	slp = vnp->data.ptrvalue;
	sip = slp->data.ptrvalue;
	ovp->strand = sip->strand;
	ovp->from = sip->from;
	ovp->to = sip->to;
	DrawIcon(ovp->icon);
	status = GetStatus(ovp->icon);
	SetStatus(ovp->icon, !status);

    if (! ovp->standAlone) {
		slptmp = AsnIoMemCopy(slp, (AsnReadFunc) SeqLocAsnRead, 
				(AsnWriteFunc) SeqLocAsnWrite);
		ovp->select_orf = slptmp;
		ObjMgrSelect (ovp->bsp_entityID, ovp->bsp_itemID, OBJ_BIOSEQ, 
				OM_REGION_SEQLOC, slptmp);
	}

	Update();
}

static void myrelease(IcoN ic, PoinT pt)
{
	RecT r;
	Int2 ir;
	Int4 pos;
	ValNodePtr vnp;
	SeqLocPtr slp;
	SeqIntPtr sip;
	Boolean status;
	OrfViewFormPtr ovp;
	Int2	item;
	Int2 top, bottom;
	BaR sb;
	Int2 startsAt;
	DoC doc;
	
	ovp = GetObjectExtra((IcoN) ic);
	ObjectRect(ic, &r);
	if (!PtInRect(pt, &(ovp->mi))) {
		return;
	}
	doc = ovp->doc;
	r.left = ovp->mi.left + 11;
	r.right = ovp->mi.right - 11;
	r.top = ovp->mi.top + 4;
	ovp->dx = (r.right - r.left - 1.) / (ovp->bsp)->length;
	ovp->dy = (ovp->mi.bottom - ovp->mi.top - 1.) / 6.;
	for (ir=0; ir < 3; ir++) {
		r.bottom = r.top + 15;
		if (pt.y < r.bottom && pt.y > r.top) {
			break;
		}
		r.top = r.bottom + 4;
	}
	if (ir < 3) {
		pos = (pt.x - r.left)/ovp->dx - ir;
		for (vnp=ovp->orfs, item=1; vnp; vnp=vnp->next, item++) {
			slp = vnp->data.ptrvalue;
			if (slp == NULL) continue;
			sip = slp->data.ptrvalue;
			if (sip == NULL) continue;
			if (vnp->choice != ir || sip->strand != Seq_strand_plus) {
				continue;
			}
			if (pos < sip->to && pos > sip->from) {
				break;
			}
		}
		if (vnp == NULL) {
			Beep();
			return;
		}
		if (slp == NULL) return;
		if (sip == NULL) return;
		ovp->frame = ir;
		ovp->strand = sip->strand;
		ovp->from = sip->from;
		ovp->to = sip->to;
		ovp->select_orf = AsnIoMemCopy(slp, (AsnReadFunc) SeqLocAsnRead, 
		(AsnWriteFunc) SeqLocAsnWrite);
		status = GetStatus(ic);
		SetStatus(ic, !status);
		Update();
		Select(doc);
		SetDocHighlight(doc, item, item);
		if (ItemIsVisible (doc, item, &top, &bottom, NULL) == FALSE) {
      	GetItemParams (doc, item, &startsAt, NULL, NULL, NULL, NULL);
      	sb = GetSlateVScrollBar ((SlatE) doc);
      	CorrectBarValue (sb, startsAt);
		}
		UpdateDocument(doc, 0, 0);

		if (! ovp->standAlone) {
			if (ovp->select_orf != NULL) {
				ObjMgrSelect (ovp->bsp_entityID, ovp->bsp_itemID, OBJ_BIOSEQ, 
					OM_REGION_SEQLOC, ovp->select_orf);
			}
		}
		Update();
		return;
	}
	r.top += 7;
	for (ir=0; ir < 3; ir++) {
		r.bottom = r.top + 15;
		if (pt.y < r.bottom && pt.y > r.top) {
			break;
		}
		r.top = r.bottom + 4;
	}
	if (ir < 3) {
		pos = (pt.x - r.left)/ovp->dx - ir;
		for (vnp=ovp->orfs, item=1; vnp; vnp=vnp->next, item++) {
			slp = vnp->data.ptrvalue;
			if (slp == NULL) continue;
			sip = slp->data.ptrvalue;
			if (sip == NULL) continue;
			if (vnp->choice != ir  || sip->strand != Seq_strand_minus) {
				continue;
			}
			if (pos < sip->to && pos > sip->from) {
				break;
			}
		}
		if (vnp == NULL) {
			Beep();
			return;
		}
		if (slp == NULL) return;
		if (sip == NULL) return;
		ovp->frame = ir;
		ovp->strand = sip->strand;
		ovp->from = sip->from;
		ovp->to = sip->to;
		ovp->select_orf = AsnIoMemCopy(slp, (AsnReadFunc) SeqLocAsnRead, 
					(AsnWriteFunc) SeqLocAsnWrite);
		status = GetStatus(ic);
		SetStatus(ic, !status);
		Update();
		Select(doc);
		SetDocHighlight(doc, item, item);
		if (ItemIsVisible (doc, item, &top, &bottom, NULL) == FALSE) {
      	GetItemParams (doc, item, &startsAt, NULL, NULL, NULL, NULL);
      	sb = GetSlateVScrollBar ((SlatE) doc);
      	CorrectBarValue (sb, startsAt);
		}
		UpdateDocument(doc, 0, 0); 
	}
	if (! ovp->standAlone) {
		if (ovp->select_orf != NULL) {
			ObjMgrSelect (ovp->bsp_entityID, ovp->bsp_itemID, OBJ_BIOSEQ, 
				OM_REGION_SEQLOC, ovp->select_orf);
		}
	}
	Update();
	return;
}

/* show all ORF from List without starts and stops */
static void ORFProc(ButtoN b)
{
	OrfViewFormPtr ovp;
	Boolean status;
	
	if ((ovp = (OrfViewFormPtr) GetObjectExtra (b)) == NULL) {
		return;
	}
	ovp->orf_only = !ovp->orf_only;
	draw_strands(ovp);
	status = GetStatus(ovp->icon);
	SetStatus(ovp->icon, !status);
	Update();
}
static void AddOrfListToDoc(DoC doc, ValNodePtr list)
{
	ParPtr	par;
	ValNodePtr vnp;
	SeqLocPtr slp, tmp;
	SeqIntPtr sip;
	Boolean minus;
	Int2 l, i;
	CharPtr buf, str;
	OrfViewFormPtr ovp;

	Reset(doc);
	ovp = (OrfViewFormPtr) GetObjectExtra(doc);
	par = (ParPtr) MemNew(sizeof (ParData));
  	par->openSpace = FALSE;
  	par->keepWithNext = 1;
  	par->keepTogether = 1;
	par->newPage  = 1;
  	par->tabStops     = 1;
	for (vnp = list, i=0; vnp; vnp=vnp->next, i++) {
		minus = FALSE;
		tmp = (SeqLocPtr) vnp->data.ptrvalue;
		slp = AsnIoMemCopy ((Pointer) tmp, (AsnReadFunc) SeqLocAsnRead,
                                        (AsnWriteFunc) SeqLocAsnWrite);
		if (slp == NULL) {
			continue;
		}                                        
        slp->next = NULL;
		sip = (SeqIntPtr) slp->data.ptrvalue;
		if (sip->strand == Seq_strand_minus) {
			sip->strand = Seq_strand_plus;
			minus = TRUE;
		}
		str = FlatLoc(ovp->bsp, slp);
		MemFree(slp);
		l = StringLen(str);
		if (minus == TRUE) {
			buf = MemNew(l+4);
			sprintf(buf, "c(%s)", str);
			AppendText(doc, buf, par, NULL, NULL); 
			MemFree(buf);
			sip->strand = Seq_strand_minus;
		} else {
			AppendText(doc, str, par, NULL, NULL); 
		}		
	}
	UpdateDocument(doc, 0, 0);
}

static void AltProc(ButtoN b)
{
	OrfViewFormPtr ovp;
	Boolean status;
	
	if ((ovp = (OrfViewFormPtr) GetObjectExtra (b)) == NULL) {
		return;
	}
	ovp->orfs =  GetAltList(ovp->orfs);
	AddOrfListToDoc(ovp->doc, ovp->orfs);
	ovp->from = 0;
	ovp->to = 0;
	draw_strands(ovp); 
	status = GetStatus(ovp->icon);
	SetStatus(ovp->icon, !status);
	if (ovp->alt_start) {
		SetTitle(b, "Alternative Initiation Codon");
		ovp->alt_start = FALSE;
	} else {
		SetTitle(b, "Standard Initiation Codon");
		ovp->alt_start = TRUE;
	}
	Update();
}

static void ChangeAAcutoff(PopuP p)
{
	OrfViewFormPtr ovp;
	Int2 i;
	Boolean status;

	ovp = (OrfViewFormPtr) GetObjectExtra(p);
	i = GetValue((PopuP) p);
	switch (i) {
		case 1:
			ovp->len = 10;
			break;
		case 2:
			ovp->len = 50;
			break;
		case 3:
			ovp->len = 100;
			break;
		default:
			ovp->len = 3;
			break;
	}
	ovp->orfs =  GetOrfList(ovp->bsp, ovp->len);
	AddOrfListToDoc(ovp->doc, ovp->orfs);
	draw_strands(ovp);
	status = GetStatus(ovp->icon);
	SetStatus(ovp->icon, !status);
	Update();
}

static Int2 LIBCALLBACK OrfViewerMsgFunc (OMMsgStructPtr ommsp)

{
  ObjMgrDataPtr   omdp;
  OMUserDataPtr   omudp;
  OrfViewFormPtr  ovp;

  omudp = (OMUserDataPtr)(ommsp->omuserdata);
  if (omudp == NULL) return OM_MSG_RET_ERROR;
  ovp = (OrfViewFormPtr) omudp->userdata.ptrvalue;
  if (ovp == NULL) return OM_MSG_RET_ERROR;
  switch (ommsp->message) {
    case OM_MSG_DEL:
      omdp = ObjMgrGetData (ommsp->entityID);
      if (omdp != NULL) {
        if (ObjMgrWholeEntity (omdp, ommsp->itemID, ommsp->itemtype)) {
          if (ovp != NULL) {
            Remove (ovp->form);
          }
          return OM_MSG_RET_OK;
        }
      }
      break;
    default :
      break;
  }
  return OM_MSG_RET_OK;
}

static void CleanupOrfViewer (GraphiC g, VoidPtr data)

{
  OrfViewFormPtr  ovp;

  ovp = (OrfViewFormPtr) data;
  if (ovp != NULL && ovp->input_entityID > 0) {
    ObjMgrFreeUserData (ovp->input_entityID, ovp->procid, ovp->proctype, ovp->userkey);
  }
  StdCleanupFormProc (g, data);
}

extern void LaunchOrfViewer (BioseqPtr bsp, Uint2 entityID, Uint2 itemID, Boolean standAlone)
{
	ButtoN qu, b1, b2;
	GrouP g;
	Int2 i, l;
	ValNodePtr vnp;
	SeqLocPtr slp, tmp;
	SeqIntPtr sip;
	CharPtr str, buf;
	DoC doc;
	OrfViewFormPtr ovp;
	WindoW w;
	IcoN ic;
	ParPtr	par;
	Boolean minus;
	PopuP pu;
	ObjMgrPtr omp;
	ObjMgrProcPtr ompp;
	OMUserDataPtr omudp;

	WatchCursor ();
	Update ();
	ovp = (OrfViewFormPtr) MemNew (sizeof (OrfViewForm));
	ovp->bsp = bsp;
	ovp->select_orf = NULL;
	ovp->input_entityID = entityID;
	ovp->bsp_entityID = entityID;
	ovp->bsp_itemID = itemID;
	ovp->orfs =  GetOrfList(bsp, ORF_LENGTH);
	ovp->orf_only = TRUE;
	ovp->gcode = 1;
	ovp->standAlone = standAlone;
	ovp->alt_start = FALSE;
	w = FixedWindow(-50, -33, -10, -10, "Orf Finder", NULL);
	SetObjectExtra (w, ovp, CleanupOrfViewer);
	ovp->form = (ForM) w;
    g = HiddenGroup (w, 5, 0, NULL);
    SetGroupSpacing (g, 6, 0);
	if (ovp->standAlone) {
		qu = PushButton(g, "Quit", OrfQuitProc);
	} else {
		qu = PushButton(g, "Close", CloseProc);
	}
	b1 = PushButton(g, "ORF", ORFProc);
	SetObjectExtra (b1, ovp, NULL);
	b2 = PushButton(g, "Alternative Initiation Codon", AltProc);
	SetObjectExtra (b2, ovp, NULL);
	StaticPrompt(g, "ORF length", 0, 16, systemFont, '1');
	pu = PopupList(g, TRUE, ChangeAAcutoff );
	PopupItem(pu, "10");
	PopupItem(pu, "50");
	PopupItem(pu, "100");
	SetValue(pu, 1);
	SetObjectExtra (pu, ovp, NULL);
	
    g = HiddenGroup (w, 2, 0, NULL);
	ic = IconButton(g, 500, 240, 
		DrawIcon, NULL, NULL, NULL, NULL, myrelease);
	if (ovp->select_orf != NULL) {
	}
	ovp->icon = ic;
    SetObjectExtra (ic, ovp, NULL);
	doc = DocumentPanel(g, 10 * stdCharWidth, 240);
    SetObjectExtra (doc, ovp, NULL);
/****/
	par = (ParPtr) MemNew(sizeof (ParData));
  	par->openSpace = FALSE;
  	par->keepWithNext = 1;
  	par->keepTogether = 1;
	par->newPage  = 1;
  	par->tabStops     = 1;
	for (vnp = ovp->orfs, i=0; vnp; vnp=vnp->next, i++) {
		minus = FALSE;
		tmp = (SeqLocPtr) vnp->data.ptrvalue;
		slp = AsnIoMemCopy ((Pointer) tmp, (AsnReadFunc) SeqLocAsnRead,
                                        (AsnWriteFunc) SeqLocAsnWrite);
        if (slp == NULL) {
        	continue;
        }
        slp->next = NULL;
		sip = (SeqIntPtr) slp->data.ptrvalue;
		if (sip->strand == Seq_strand_minus) {
			sip->strand = Seq_strand_plus;
			minus = TRUE;
		}
		str = FlatLoc(ovp->bsp, slp);
		MemFree(slp);
		l = StringLen(str);
		if (minus == TRUE) {
			buf = MemNew(l+4);
			sprintf(buf, "c(%s)", str);
			AppendText(doc, buf, par, NULL, NULL); 
			MemFree(buf);
			sip->strand = Seq_strand_minus;
		} else {
			AppendText(doc, str, par, NULL, NULL); 
		}		
	}
/****/
    ovp->doc = doc;
	SetDocNotify(doc, notify);
	/*ovp = (OrfViewFormPtr) GetObjectExtra(ic);*/
    omp = ObjMgrGet ();
    if (omp != NULL) {
		ompp = ObjMgrProcFind (omp, 0, "ORF Finder", OMPROC_FILTER);
		if (ompp != NULL) {
			ovp->procid = ompp->procid;
			ovp->proctype = OMPROC_FILTER;
			ovp->userkey = OMGetNextUserKey ();
			omudp = ObjMgrAddUserData (ovp->input_entityID, ompp->procid,
	                           OMPROC_FILTER, ovp->userkey);
			if (omudp != NULL) {
				omudp->userdata.ptrvalue = (Pointer) ovp;
				omudp->messagefunc = OrfViewerMsgFunc;
			}
		}
	}
	RealizeWindow (w);
	Show(w);
	ArrowCursor ();
	Update ();
	if (ovp->standAlone) {
		ProcessEvents();
	} else {
		return;
	}
}

static Int2 LIBCALLBACK OrfFindFunc (Pointer data)

{
  BioseqPtr         bsp;
  OMProcControlPtr  ompcp;


	ompcp = (OMProcControlPtr) data;   /* always do this cast */

	if (ompcp == NULL || ompcp->input_itemtype == 0)
		return OM_MSG_RET_ERROR;

	switch (ompcp->input_itemtype)
	{
		case OBJ_BIOSEQ:
			bsp = (BioseqPtr) ompcp->input_data;
		break;
		default:
			return OM_MSG_RET_ERROR;
	}

	LaunchOrfViewer (bsp, ompcp->input_entityID, ompcp->input_itemID, FALSE);
	return OM_MSG_RET_DONE;
}

typedef struct mergedata {
  SeqLocPtr     slp;
} MergeData, PNTR MergeDataPtr;

static Boolean AddToSeqLoc (GatherContextPtr gcp)

{
  BioseqPtr     bsp;
  MergeDataPtr  mdp;
  SeqFeatPtr    sfp;
  SeqLocPtr     slp;

  mdp = (MergeDataPtr) gcp->userdata;
  if (mdp == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  if (sfp == NULL || sfp->location == NULL) return TRUE;
  bsp = GetBioseqGivenSeqLoc (sfp->location, gcp->entityID);
  if (bsp == NULL) return TRUE;
  slp = SeqLocMerge (bsp, sfp->location, mdp->slp, FALSE, TRUE, FALSE);
  mdp->slp = SeqLocFree (mdp->slp);
  mdp->slp = slp;
  return TRUE;
}

static SeqLocPtr MergeSelectedFeatureIntervals (void)

{
  MergeData     md;
  SelStructPtr  sel;

  md.slp = NULL;
  for (sel = ObjMgrGetSelected (); sel != NULL; sel = sel->next) {
    GatherItem (sel->entityID, sel->itemID, sel->itemtype,
                (Pointer) &md, AddToSeqLoc);
  }
  return md.slp;
}

static Int2 LIBCALLBACK IntervalCombineFunc (Pointer data)

{
  OMProcControlPtr  ompcp;
  SeqLocPtr         slp;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL) return OM_MSG_RET_ERROR;
  slp = MergeSelectedFeatureIntervals ();
  if (slp == NULL) return OM_MSG_RET_ERROR;
  ObjMgrRegister (OBJ_SEQLOC, (Pointer) slp);
  return OM_MSG_RET_DONE;
}

static CharPtr objmgrtypestrs [] = {
  "OBJ_ALL", "OBJ_SEQENTRY", "OBJ_BIOSEQ", "OBJ_BIOSEQSET", "OBJ_SEQDESC",
  "OBJ_SEQANNOT", "OBJ_ANNOTDESC", "OBJ_SEQFEAT", "OBJ_SEQALIGN", "OBJ_SEQGRAPH",
  "OBJ_SEQSUB", "OBJ_SUBMIT_BLOCK", "OBJ_SEQSUB_CONTACT", "13", "OBJ_BIOSEQ_MAPFEAT",
  "OBJ_BIOSEQ_SEG", "OBJ_SEQHIST", "OBJ_SEQHIST_ALIGN", "OBJ_BIOSEQ_DELTA", "19",
  "OBJ_PUB", "OBJ_SEQFEAT_CIT", "OBJ_SEQSUB_CIT", "OBJ_MEDLINE_ENTRY", "OBJ_PUB_SET",
  "OBJ_SEQLOC", "OBJ_SEQID", "OBJ_SEQCODE", "OBJ_SEQCODE_SET", "OBJ_GENETIC_CODE",
  "OBJ_GENETIC_CODE_SET", "OBJ_TEXT_REPORT", "OBJ_FASTA", "OBJ_VIBRANT_PICTURE", "OBJ_PROJECT"
};

static CharPtr temploadstrs [] = {
  "TL_NOT_TEMP", "TL_LOADED", "TL_CACHED"
};

static CharPtr proctypestrs [] = {
  "0", "OMPROC_OPEN", "OMPROC_DELETE", "OMPROC_VIEW", "OMPROC_EDIT",
  "OMPROC_SAVE", "OMPROC_CUT", "OMPROC_COPY", "OMPROC_PASTE", "OMPROC_ANALYZE",
  "OMPROC_FIND", "OMPROC_REPLACE", "OMPROC_FILTER", "OMPROC_FETCH",
};

static void PrintABool (FILE *fp, CharPtr str, Boolean val)

{
  if (val) {
    fprintf (fp, "%s TRUE\n", str);
  } else {
    fprintf (fp, "%s FALSE\n", str);
  }
}

Int2 LIBCALLBACK VSMPictMsgFunc PROTO((OMMsgStructPtr ommsp));

static void ReportOnEntity (ObjMgrDataPtr omdp, ObjMgrPtr omp, Boolean selected, Uint2 itemID,
                            Uint2 itemtype, Int2 index, FILE *fp)

{
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  Char           buf [50];
  OMUserDataPtr  omudp;
  VSMPictPtr     vsmpp;

  if (omdp == NULL || fp == NULL) return;
  if (selected) {
    fprintf (fp, "Data Element\n\n");
    fprintf (fp, "  EntityID %d selected\n", (int) omdp->EntityID);
    fprintf (fp, "  ItemID %d, Itemtype %d\n", (int) itemID, (int) itemtype);
  } else if (omdp->parentptr == NULL) {
    fprintf (fp, "Top Data Element %d\n\n", (int) index);
    fprintf (fp, "  EntityID %d\n", (int) omdp->EntityID);
  } else {
    fprintf (fp, "Inner Data Element %d\n\n", (int) index);
    fprintf (fp, "  EntityID %d\n", (int) omdp->EntityID);
  }
  if (omdp->datatype < OBJ_MAX) {
    fprintf (fp, "  Datatype %s", objmgrtypestrs [omdp->datatype]);
    if (omdp->datatype == OBJ_BIOSEQ) {
      bsp = (BioseqPtr) omdp->dataptr;
      if (bsp != NULL) {
        SeqIdWrite (bsp->id, buf, PRINTID_FASTA_LONG, sizeof (buf) - 1);
        fprintf (fp, " %s, length %ld", buf, (long) bsp->length);
      }
    } else if (omdp->datatype == OBJ_BIOSEQSET) {
      bssp = (BioseqSetPtr) omdp->dataptr;
      if (bssp != NULL) {
        fprintf (fp, " class %d", (int) bssp->_class);
      }
    }
    fprintf (fp, "\n");
  } else {
    fprintf (fp, "  Unregistered datatype %d\n", (int) omdp->datatype);
  }
  fprintf (fp, "  Lockcnt %d\n", (int) omdp->lockcnt);
  if (omdp->tempload < 3) {
    fprintf (fp, "  Tempload %s\n", temploadstrs [omdp->tempload]);
  } else {
    fprintf (fp, "  Unrecognized tempload %d\n", (int) omdp->tempload);
  }
  PrintABool (fp, "  Clipboard", omdp->clipboard);
  PrintABool (fp, "  Dirty", omdp->dirty);
  PrintABool (fp, "  Being_freed", omdp->being_freed);
  PrintABool (fp, "  Free", omdp->free);
  fprintf (fp, "\n");
  for (omudp = omdp->userdata; omudp != NULL; omudp = omudp->next) {
    if (omudp->proctype <= OMPROC_MAX) {
      fprintf (fp, "    Proctype %s\n", proctypestrs [omudp->proctype]);
    } else {
      fprintf (fp, "    Unrecognized proctype %d\n", (int) omudp->proctype);
    }
    fprintf (fp, "    Procid %d\n", (int) omudp->procid);
    fprintf (fp, "    Userkey %d\n", (int) omudp->userkey);
    if (omudp->messagefunc == VSMPictMsgFunc) {
      vsmpp = (VSMPictPtr) omudp->userdata.ptrvalue;
      if (vsmpp != NULL) {
        if (vsmpp->s != NULL) {
          fprintf (fp, "    VSMPictPtr segment not NULL\n");
        } else {
          fprintf (fp, "    VSMPictPtr segment is NULL\n");
        }
      }
    }
    fprintf (fp, "\n");
  }
}

static Int2 LIBCALLBACK DesktopReportFunc (Pointer data)

{
  FILE           *fp;
  Int2           j;
  Int2           num;
  ObjMgrPtr      omp;
  ObjMgrDataPtr  omdp;
  ObjMgrDataPtr  PNTR omdpp;
  Char           path [PATH_MAX];
  SelStructPtr   sel;

  omp = ObjMgrGet ();
  if (omp == NULL) return OM_MSG_RET_DONE;
  TmpNam (path);
  fp = FileOpen (path, "w");
  fprintf (fp, "Object Manager\n\n");
  fprintf (fp, "  HighestEntityID %d\n", (int) omp->HighestEntityID);
  fprintf (fp, "  Totobj %d\n", (int) omp->totobj);
  fprintf (fp, "  Currobj %d\n", (int) omp->currobj);
  fprintf (fp, "  Maxtemp %d\n", (int) omp->maxtemp);
  fprintf (fp, "  Tempcnt %d\n", (int) omp->tempcnt);
  fprintf (fp, "  Hold %d\n", (int) omp->hold);
  PrintABool (fp, "  Reaping", omp->reaping);
  PrintABool (fp, "  Is_write_locked", omp->is_write_locked);
  fprintf (fp, "\n");
  sel = ObjMgrGetSelected ();
  if (sel != NULL) {
    omdp = ObjMgrGetData (sel->entityID);
    ReportOnEntity (omdp, omp, TRUE, sel->itemID, sel->itemtype, 0, fp);
  } else {
    num = omp->currobj;
    for (j = 0, omdpp = omp->datalist; j < num && omdpp != NULL; j++, omdpp++) {
      omdp = *omdpp;
      if (omdp->parentptr == NULL) {
        ReportOnEntity (omdp, omp, FALSE, 0, 0, j + 1, fp);
      }
    }
    for (j = 0, omdpp = omp->datalist; j < num && omdpp != NULL; j++, omdpp++) {
      omdp = *omdpp;
      if (omdp->parentptr != NULL) {
        ReportOnEntity (omdp, omp, FALSE, 0, 0, j + 1, fp);
      }
    }
  }
  FileClose (fp);
  LaunchGeneralTextViewer (path, "Object Manager Report");
  FileRemove (path);
  return OM_MSG_RET_DONE;
}

static void ConvertGiToAccn (SeqIdPtr sip)

{
  Int4      gi;
  SeqIdPtr  newsip;

  if (sip == NULL) return;
  if (sip->choice != SEQID_GI) return;
  gi = sip->data.intvalue;
  newsip = GetSeqIdForGI (gi);
  if (newsip == NULL) return;
  if (newsip->choice == SEQID_GIBBSQ ||
      newsip->choice == SEQID_GIBBMT ||
      newsip->choice == SEQID_GI) {
    SeqIdFree (newsip);
    return;
  }
  sip->choice = newsip->choice;
  sip->data.ptrvalue = newsip->data.ptrvalue;
  newsip->choice = SEQID_NOT_SET;
  newsip->data.ptrvalue = NULL;
  SeqIdFree (newsip);
}

static Boolean GiToAccnAlignCallback (GatherContextPtr gcp)

{
  SeqAlignPtr   align;
  DenseDiagPtr  ddp;
  DenseSegPtr   dsp;
  SeqIdPtr      sip;
  StdSegPtr     ssp;
  SeqLocPtr     tloc;

  if (gcp == NULL) return TRUE;
  switch (gcp->thistype) {
    case OBJ_SEQALIGN :
    case OBJ_SEQHIST_ALIGN :
      align = (SeqAlignPtr) gcp->thisitem;
      sip = NULL;
      if (align->segtype == 1) {
        ddp = (DenseDiagPtr) align->segs;
        if (ddp != NULL) {
          for (sip = ddp->id; sip != NULL; sip = sip->next) {
            ConvertGiToAccn (sip);
          }
        }
      } else if (align->segtype == 2) {
        dsp = (DenseSegPtr) align->segs;
        if (dsp != NULL) {
          for (sip = dsp->ids; sip != NULL; sip = sip->next) {
            ConvertGiToAccn (sip);
          }
        }
      } else if (align->segtype == 3) {
        ssp = (StdSegPtr) align->segs;
        if (ssp != NULL) {
          for (tloc = ssp->loc; tloc != NULL; tloc = tloc->next) {
            sip = SeqLocId (tloc);
            ConvertGiToAccn (sip);
          }
        }
      }
      break;
    default :
      break;
  }
  return TRUE;
}

static Int2 LIBCALLBACK AlignGiToAccnProc (Pointer data)

{
  MsgAnswer         ans;
  GatherScope       gs;
  OMProcControlPtr  ompcp;
  SeqEntryPtr       sep;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL) return OM_MSG_RET_ERROR;
  ans = Message (MSG_OKC, "Are you sure you want to convert alignment GIs to accessions?");
  if (ans == ANS_CANCEL) return OM_MSG_RET_DONE;
  sep = GetTopSeqEntryForEntityID (ompcp->input_entityID);
  LookupFarSeqIDs (sep, FALSE, FALSE, FALSE, TRUE, FALSE);
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQALIGN] = FALSE;
  gs.ignore[OBJ_SEQHIST_ALIGN] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  GatherEntity (ompcp->input_entityID, NULL, GiToAccnAlignCallback, &gs);
  return OM_MSG_RET_DONE;
}

static void ConvertAccnToGi (SeqIdPtr sip)

{
  Int4      gi;
  SeqIdPtr  newsip;
  Char      str [42];

  if (sip == NULL) return;
  if (sip->choice == SEQID_GI) return;
  gi = GetGIForSeqId (sip);
  if (gi < 1) return;
  sprintf (str, "gi|%ld", (long) gi);
  newsip = SeqIdParse (str);
  if (newsip == NULL) return;
  sip->choice = newsip->choice;
  sip->data.ptrvalue = newsip->data.ptrvalue;
  newsip->choice = SEQID_NOT_SET;
  newsip->data.ptrvalue = NULL;
  SeqIdFree (newsip);
}

static Boolean AccnToGiAlignCallback (GatherContextPtr gcp)

{
  SeqAlignPtr   align;
  DenseDiagPtr  ddp;
  DenseSegPtr   dsp;
  SeqIdPtr      sip;
  StdSegPtr     ssp;
  SeqLocPtr     tloc;

  if (gcp == NULL) return TRUE;
  switch (gcp->thistype) {
    case OBJ_SEQALIGN :
    case OBJ_SEQHIST_ALIGN :
      align = (SeqAlignPtr) gcp->thisitem;
      sip = NULL;
      if (align->segtype == 1) {
        ddp = (DenseDiagPtr) align->segs;
        if (ddp != NULL) {
          for (sip = ddp->id; sip != NULL; sip = sip->next) {
            ConvertAccnToGi (sip);
          }
        }
      } else if (align->segtype == 2) {
        dsp = (DenseSegPtr) align->segs;
        if (dsp != NULL) {
          for (sip = dsp->ids; sip != NULL; sip = sip->next) {
            ConvertAccnToGi (sip);
          }
        }
      } else if (align->segtype == 3) {
        ssp = (StdSegPtr) align->segs;
        if (ssp != NULL) {
          for (tloc = ssp->loc; tloc != NULL; tloc = tloc->next) {
            sip = SeqLocId (tloc);
            ConvertAccnToGi (sip);
          }
        }
      }
      break;
    default :
      break;
  }
  return TRUE;
}

static Int2 LIBCALLBACK AlignAccnToGiProc (Pointer data)

{
  MsgAnswer         ans;
  GatherScope       gs;
  OMProcControlPtr  ompcp;
  SeqEntryPtr       sep;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL) return OM_MSG_RET_ERROR;
  ans = Message (MSG_OKC, "Are you sure you want to convert alignment accessions to GIs?");
  if (ans == ANS_CANCEL) return OM_MSG_RET_DONE;
  sep = GetTopSeqEntryForEntityID (ompcp->input_entityID);
  LookupFarSeqIDs (sep, FALSE, FALSE, FALSE, TRUE, FALSE);
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQALIGN] = FALSE;
  gs.ignore[OBJ_SEQHIST_ALIGN] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  GatherEntity (ompcp->input_entityID, NULL, AccnToGiAlignCallback, &gs);
  return OM_MSG_RET_DONE;
}

static Boolean IsSipMrna (SeqIdPtr sip)

{
  Char                    buf [45];
  BioseqPtr               bsp;
  Int4                    count;
  Entrez2BooleanReplyPtr  e2br;
  Entrez2RequestPtr       e2rq;
  Entrez2ReplyPtr         e2ry;
  Int4                    gi;
  Char                    query [128];
  E2ReplyPtr              reply;

  if (sip == NULL) return FALSE;
  bsp = BioseqFind (sip);
  if (bsp != NULL) return FALSE;
  if (sip->choice == SEQID_GI) {
    gi = sip->data.intvalue;
    sip = GetSeqIdForGI (gi);
  }
  if (sip == NULL) return FALSE;
  SeqIdWrite (sip, buf, PRINTID_TEXTID_ACCESSION, 41);
  sprintf (query, "biomol_mrna [PROP] AND %s [ACCN]", buf);
  e2rq = EntrezCreateBooleanRequest (FALSE, FALSE, "nucleotide", query, 0, 0, NULL, 0, 0);
  e2ry = EntrezSynchronousQuery (e2rq);
  e2rq = Entrez2RequestFree (e2rq);
  if (e2ry == NULL) return FALSE;
  reply = e2ry->reply;
  if (reply == NULL || reply->choice != E2Reply_eval_boolean) return FALSE;
  e2br = EntrezExtractBooleanReply (e2ry);
  if (e2br == NULL) return FALSE;
  count = e2br->count;
  Entrez2BooleanReplyFree (e2br);
  if (count > 0) return TRUE;
  return FALSE;
}

static Boolean IsMrnaAlignment (SeqAlignPtr align)

{
  DenseDiagPtr  ddp;
  DenseSegPtr   dsp;
  SeqIdPtr      sip;
  StdSegPtr     ssp;
  SeqLocPtr     tloc;

  if (align == NULL) return FALSE;
  sip = NULL;
  if (align->segtype == 1) {
    ddp = (DenseDiagPtr) align->segs;
    if (ddp != NULL) {
      for (sip = ddp->id; sip != NULL; sip = sip->next) {
        if (IsSipMrna (sip)) return TRUE;
      }
    }
  } else if (align->segtype == 2) {
    dsp = (DenseSegPtr) align->segs;
    if (dsp != NULL) {
      for (sip = dsp->ids; sip != NULL; sip = sip->next) {
        if (IsSipMrna (sip)) return TRUE;
      }
    }
  } else if (align->segtype == 3) {
    ssp = (StdSegPtr) align->segs;
    if (ssp != NULL) {
      for (tloc = ssp->loc; tloc != NULL; tloc = tloc->next) {
        sip = SeqLocId (tloc);
        if (IsSipMrna (sip)) return TRUE;
      }
    }
  }
  return FALSE;
}

static SeqAnnotPtr ExtractBlastMrna (SeqAlignPtr sap, Pointer PNTR prevlink)

{
  AnnotDescrPtr  adp;
  SeqAnnotPtr    annot = NULL;
  SeqAlignPtr    next;
  ObjectIdPtr    oip;
  UserFieldPtr   ufp;
  UserObjectPtr  uop;

  while (sap != NULL) {
    next = sap->next;

    if (IsMrnaAlignment (sap)) {
      *prevlink = sap->next;
      sap->next = NULL;

      if (annot == NULL) {
        annot = SeqAnnotNew ();
        if (annot != NULL) {
          annot->type = 2;
          adp = ValNodeNew (NULL);
          adp->choice = Annot_descr_user;
          annot->desc = adp;
          uop = UserObjectNew ();
          adp->data.ptrvalue = uop;
          oip = ObjectIdNew ();
          oip->str = StringSave ("Blast Type");
          ufp = UserFieldNew ();
          uop->type = oip;
          uop->data = ufp;
          oip = ObjectIdNew ();
          oip->str = StringSave ("BLASTN - mrna");
          ufp->label = oip;
          ufp->choice = 2;
          ufp->data.intvalue = 1;
        }
      }
      if (annot != NULL) {
        sap->next = annot->data;
        annot->data = sap;
      }

    } else {
      sap->idx.prevlink = prevlink;
      prevlink = (Pointer PNTR) &(sap->next);
    }

    sap = next;
  }

  return annot;
}

static void FindBlastNR (SeqAnnotPtr sap, Pointer userdata)

{
  AnnotDescrPtr  adp;
  SeqAnnotPtr    annot;
  ObjectIdPtr    oip;
  UserFieldPtr   ufp;
  UserObjectPtr  uop;

  if (sap == NULL || sap->type != 2) return;
  for (adp = sap->desc; adp != NULL; adp = adp->next) {
    if (adp->choice != Annot_descr_user) continue;
    for (uop = adp->data.ptrvalue; uop != NULL; uop = uop->next) {
      oip = uop->type;
      if (oip == NULL) continue;
      if (StringCmp (oip->str, "Blast Type") == 0) {
        ufp = uop->data;
        if (ufp == NULL) continue;
        oip = ufp->label;
        if (oip == NULL) continue;
        if (StringCmp (oip->str, "BLASTN - nr") == 0) {
          annot = ExtractBlastMrna ((SeqAlignPtr) sap->data, (Pointer PNTR) &(sap->data));
          if (annot != NULL) {
            annot->next = sap->next;
            sap->next = annot;
          }
        }
      }
    }
  }
}

static Int2 LIBCALLBACK SeparateMrnaFromNrProc (Pointer data)

{
  OMProcControlPtr  ompcp;
  SeqEntryPtr       sep;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL) return OM_MSG_RET_ERROR;
  sep = GetTopSeqEntryForEntityID (ompcp->input_entityID);
  LookupFarSeqIDs (sep, FALSE, FALSE, FALSE, TRUE, FALSE);
  VisitAnnotsInSep (sep, NULL, FindBlastNR);
  DeleteMarkedObjects (ompcp->input_entityID, 0, NULL);
  ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, 0, 0);
  return OM_MSG_RET_DONE;
}

static void CacheByAccn (SeqIdPtr sip, CharPtr directory)

{
  AsnIoPtr     aip;
  BioseqPtr    bsp;
  Char         buf [45];
  Uint2        entityID;
  Int4         gi = 0;
  SeqIdPtr     newsip = NULL;
  Char         path [PATH_MAX];
  SeqEntryPtr  sep;

  if (sip == NULL || sip->choice == SEQID_LOCAL) return;
  if (sip->choice == SEQID_GI) {
    gi = sip->data.intvalue;
    newsip = GetSeqIdForGI (gi);
    sip = newsip;
  }
  if (sip == NULL) return;

  if (gi != 0) {
    sprintf (buf, "%ld", (long) gi);
  } else {
    SeqIdWrite (sip, buf, PRINTID_TEXTID_ACCESSION, 40);
  }
  StringCat (buf, ".ent");
  StringCpy (path, directory);
  FileBuildPath (path, NULL, buf);
  if (FileLength (path) > 0) return;

  bsp = BioseqLockById (sip);
  if (bsp != NULL) {
    sep = SeqMgrGetSeqEntryForData (bsp);
    entityID = ObjMgrGetEntityIDForChoice (sep);
    sep = GetTopSeqEntryForEntityID (entityID);
    if (sep != NULL) {
      aip = AsnIoOpen (path, "w");
      if (aip != NULL) {
        SeqEntryAsnWrite (sep, aip, NULL);
        AsnIoClose (aip);
      }
    }
  }
  BioseqUnlock (bsp);

  if (newsip != NULL) {
    SeqIdFree (newsip);
  }
}

static Boolean CacheAccnsCallback (GatherContextPtr gcp)

{
  SeqAlignPtr   align;
  DenseDiagPtr  ddp;
  DeltaSeqPtr   dlt;
  DenseSegPtr   dsp;
  CharPtr       path;
  SeqIdPtr      sip;
  SeqLocPtr     slp;
  StdSegPtr     ssp;
  SeqLocPtr     tloc;

  if (gcp == NULL) return TRUE;
  path = (CharPtr) gcp->userdata;
  switch (gcp->thistype) {
    case OBJ_BIOSEQ_SEG :
      slp = (SeqLocPtr) gcp->thisitem;
      if (slp != NULL) {
        sip = SeqLocId (slp);
        CacheByAccn (sip, path);
      }
      break;
    case OBJ_BIOSEQ_DELTA :
      dlt = (DeltaSeqPtr) gcp->thisitem;
      if (dlt != NULL && dlt->choice == 1) {
        slp = (SeqLocPtr) dlt->data.ptrvalue;
        if (slp != NULL) {
          sip = SeqLocId (slp);
          CacheByAccn (sip, path);
        }
      }
      break;
    case OBJ_SEQALIGN :
    case OBJ_SEQHIST_ALIGN :
      align = (SeqAlignPtr) gcp->thisitem;
      sip = NULL;
      if (align->segtype == 1) {
        ddp = (DenseDiagPtr) align->segs;
        if (ddp != NULL) {
          for (sip = ddp->id; sip != NULL; sip = sip->next) {
            CacheByAccn (sip, path);
          }
        }
      } else if (align->segtype == 2) {
        dsp = (DenseSegPtr) align->segs;
        if (dsp != NULL) {
          for (sip = dsp->ids; sip != NULL; sip = sip->next) {
            CacheByAccn (sip, path);
          }
        }
      } else if (align->segtype == 3) {
        ssp = (StdSegPtr) align->segs;
        if (ssp != NULL) {
          for (tloc = ssp->loc; tloc != NULL; tloc = tloc->next) {
            sip = SeqLocId (tloc);
            CacheByAccn (sip, path);
          }
        }
      }
      break;
    default :
      break;
  }
  return TRUE;
}

static Int2 LIBCALLBACK CacheAccnsToDisk (Pointer data)

{
  GatherScope       gs;
  OMProcControlPtr  ompcp;
  Char              path [PATH_MAX];
  CharPtr           ptr;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL) return OM_MSG_RET_ERROR;
  if (! GetOutputFileName (path, sizeof (path), NULL)) return OM_MSG_RET_DONE;
  ptr = StringRChr (path, DIRDELIMCHR);
  if (ptr != NULL) {
    *ptr = '\0';
  }
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_BIOSEQ_DELTA] = FALSE;
  gs.ignore[OBJ_SEQALIGN] = FALSE;
  gs.ignore[OBJ_SEQHIST_ALIGN] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  GatherEntity (ompcp->input_entityID, (Pointer) path, CacheAccnsCallback, &gs);
  return OM_MSG_RET_DONE;
}

static Boolean AddFeatToVnp (GatherContextPtr gcp)

{
  SeqFeatPtr       sfp;
  ValNodePtr       vnp;
  ValNodePtr PNTR  vnpp;

  vnpp = (ValNodePtr PNTR) gcp->userdata;
  if (vnpp == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  if (sfp == NULL || sfp->data.choice != SEQFEAT_CDREGION || sfp->location == NULL) return TRUE;
  vnp = ValNodeAdd (vnpp);
  if (vnp == NULL) return TRUE;
  vnp->data.ptrvalue = (Pointer) sfp;
  return TRUE;
}

typedef struct protfindlist {
  SeqLocPtr   slp;
  ProtRefPtr  prp;
  Int4        min;
} ProtFindList, PNTR ProtFindPtr;

static Boolean ProtFindFunc (GatherContextPtr gcp)

{
  Int4         diff;
  ProtFindPtr  pfp;
  SeqFeatPtr   sfp;

  if (gcp == NULL) return TRUE;

  pfp = (ProtFindPtr) gcp->userdata;
  if (pfp == NULL) return TRUE;

  if (gcp->thistype == OBJ_SEQFEAT) {
    sfp = (SeqFeatPtr) gcp->thisitem;
    if (sfp != NULL && sfp->data.choice == SEQFEAT_PROT && sfp->data.value.ptrvalue != NULL) {
      diff = SeqLocAinB (pfp->slp, sfp->location);
      if (diff >= 0) {
        if (diff < pfp->min) {
          pfp->min = diff;
          pfp->prp = (ProtRefPtr) sfp->data.value.ptrvalue;
        }
      }
    }
  }

  return TRUE;
}

static ProtRefPtr FindBestProtRef (Uint2 entityID, SeqFeatPtr cds)

{
  GatherScope   gs;
  ProtFindList  pfl;

  if (entityID == 0 || cds == NULL || cds->product == NULL) return NULL;
  pfl.slp = cds->product;
  pfl.prp = NULL;
  pfl.min = INT4_MAX;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = TRUE;
  MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  GatherEntity (entityID, (Pointer) &pfl, ProtFindFunc, &gs);
  return pfl.prp;
}

/* if gene location matches mRNA exactly, make it partial on both ends */
static void MakeMRNAGenesPartial (SeqFeatPtr sfp, Pointer userdata)
{
  SeqFeatPtr mrna;

  if (sfp == NULL || userdata == NULL) return;
  if (sfp->data.choice != SEQFEAT_GENE) return;

  mrna = (SeqFeatPtr) userdata;

  if (SeqLocAinB (mrna->location, sfp->location) != 0) return;

  SetSeqLocPartial (sfp->location, TRUE, TRUE);
}

extern void MRnaFromCdsProc (Uint2 entityID)

{
  BioseqPtr     bsp;
  SeqFeatPtr    cds;
  ValNodePtr    head;
  Char          mRnaName [128];
  ValNodePtr    name;
  /*
  Boolean       noLeft;
  Boolean       noRight;
  */
  ProtRefPtr    prp;
  SeqFeatPtr    rna;
  RnaRefPtr     rrp;
  SelStructPtr  sel;
  SeqEntryPtr   sep;
  ValNodePtr    vnp;

  if (entityID == 0) return;
  head = NULL;
  for (sel = ObjMgrGetSelected (); sel != NULL; sel = sel->next) {
    GatherItem (sel->entityID, sel->itemID, sel->itemtype,
                (Pointer) &head, AddFeatToVnp);
  }
  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    cds = (SeqFeatPtr) vnp->data.ptrvalue;
    if (cds != NULL) {
      rrp = RnaRefNew ();
      if (rrp != NULL) {
        rrp->type = 2;
        mRnaName [0] = '\0';
        prp = FindBestProtRef (entityID, cds);
        if (prp != NULL) {
          name = prp->name;
          if (name != NULL) {
            StringNCpy_0 (mRnaName, (CharPtr) name->data.ptrvalue, sizeof (mRnaName));
          }
          if (StringHasNoText (mRnaName)) {
            StringNCpy_0 (mRnaName, prp->desc, sizeof (mRnaName));
          }
        }
        if (! StringHasNoText (mRnaName)) {
          rrp->ext.choice = 1;
          rrp->ext.value.ptrvalue = StringSave (mRnaName);
        }
        rna = SeqFeatNew ();
        if (rna != NULL) {
          rna->data.choice = SEQFEAT_RNA;
          rna->data.value.ptrvalue = (Pointer) rrp;
          rna->location = AsnIoMemCopy ((Pointer) cds->location,
                                        (AsnReadFunc) SeqLocAsnRead,
                                        (AsnWriteFunc) SeqLocAsnWrite);
          /* CheckSeqLocForPartial (rna->location, &noLeft, &noRight); */
          SetSeqLocPartial (rna->location, TRUE, TRUE); /* now always set */
          /* rna->partial = (rna->partial || noLeft || noRight); */
          rna->partial = TRUE;
          bsp = GetBioseqGivenSeqLoc (rna->location, entityID);
          if (bsp != NULL) {
            sep = SeqMgrGetSeqEntryForData (bsp);
            if (sep != NULL) {
              CreateNewFeature (sep, NULL, SEQFEAT_RNA, rna);
            } else {
              rna->next = cds->next;
              cds->next = rna;
            }
            VisitFeaturesOnBsp (bsp, (Pointer) rna, MakeMRNAGenesPartial);
          } else {
            rna->next = cds->next;
            cds->next = rna;
          }
        }
      }
    }
  }
  ValNodeFree (head);
  ObjMgrSetDirtyFlag (entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
}

static Int2 LIBCALLBACK MRnaFromCdsFunc (Pointer data)

{
  OMProcControlPtr  ompcp;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL) return OM_MSG_RET_ERROR;
  MRnaFromCdsProc (ompcp->input_entityID);
  return OM_MSG_RET_DONE;
}

typedef struct cdsfindlist {
  SeqLocPtr   loc;
  SeqLocPtr   prod;
  SeqFeatPtr  cds;
  Int4        min;
} CdsFindList, PNTR CdsFindPtr;

static Boolean CdsFindFunc (GatherContextPtr gcp)

{
  CdsFindPtr      cfp;
  Int4            diff;
  SeqFeatPtr      sfp;

  if (gcp == NULL) return TRUE;

  cfp = (CdsFindPtr) gcp->userdata;
  if (cfp == NULL) return TRUE;

  if (gcp->thistype == OBJ_SEQFEAT) {
    sfp = (SeqFeatPtr) gcp->thisitem;
    if (sfp != NULL && sfp->data.choice == SEQFEAT_CDREGION && sfp->data.value.ptrvalue != NULL) {
      if (cfp->loc != NULL) {
        diff = SeqLocAinB (cfp->loc, sfp->location);
        if (diff >= 0) {
          if (diff < cfp->min) {
            cfp->min = diff;
            cfp->cds = sfp;
          }
        }
      } else if (cfp->prod != NULL) {
        diff = SeqLocAinB (cfp->prod, sfp->product);
        if (diff >= 0) {
          if (diff < cfp->min) {
            cfp->min = diff;
            cfp->cds = sfp;
          }
        }
      }
    }
  }

  return TRUE;
}

extern SeqFeatPtr FindBestCds (Uint2 entityID, SeqLocPtr loc, SeqLocPtr prod, SeqEntryPtr scope)

{
  CdsFindList  cfl;
  GatherScope  gs;

  if (entityID == 0) return NULL;
  cfl.loc = loc;
  cfl.prod = prod;
  cfl.cds = NULL;
  cfl.min = INT4_MAX;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = TRUE;
  MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  gs.scope = scope;
  GatherEntity (entityID, (Pointer) &cfl, CdsFindFunc, &gs);
  return cfl.cds;
}

static Int2 LIBCALLBACK MapToProtFunc (Pointer data)

{
  SeqFeatPtr        cds;
  OMProcControlPtr  ompcp;
  SeqEntryPtr       scope;
  SeqFeatPtr        sfp;
  SeqLocPtr         slp;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->input_itemtype == 0)
    return OM_MSG_RET_ERROR;

  switch (ompcp->input_itemtype)
  {
    case OBJ_SEQFEAT:
      sfp = (SeqFeatPtr) ompcp->input_data;
      break;
    default:
      return OM_MSG_RET_ERROR;
  }

  scope = GetBestTopParentForItemID (ompcp->input_entityID, ompcp->input_itemID,
                                     ompcp->input_itemtype);
  cds = FindBestCds (ompcp->input_entityID, sfp->location, NULL, scope);
  if (cds != NULL) {
    slp = dnaLoc_to_aaLoc (cds, sfp->location, TRUE, NULL, FALSE);
    if (slp != NULL) {
      ObjMgrRegister (OBJ_SEQLOC, (Pointer) slp);
    }
    return OM_MSG_RET_DONE;
  } else {
    Message (MSG_ERROR, "Unable to find CDS for peptide feature");
    return OM_MSG_RET_ERROR;
  }
}

static Int2 LIBCALLBACK MapToNucFunc (Pointer data)

{
  SeqFeatPtr        cds;
  OMProcControlPtr  ompcp;
  SeqEntryPtr       scope;
  SeqFeatPtr        sfp;
  SeqLocPtr         slp;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->input_itemtype == 0)
    return OM_MSG_RET_ERROR;

  switch (ompcp->input_itemtype)
  {
    case OBJ_SEQFEAT:
      sfp = (SeqFeatPtr) ompcp->input_data;
      break;
    default:
      return OM_MSG_RET_ERROR;
  }

  scope = GetBestTopParentForItemID (ompcp->input_entityID, ompcp->input_itemID,
                                     ompcp->input_itemtype);
  cds = FindBestCds (ompcp->input_entityID, NULL, sfp->location, scope);
  if (cds != NULL) {
    slp = aaLoc_to_dnaLoc (cds, sfp->location);
    if (slp != NULL) {
      ObjMgrRegister (OBJ_SEQLOC, (Pointer) slp);
    }
    return OM_MSG_RET_DONE;
  } else {
    Message (MSG_ERROR, "Unable to find CDS for peptide feature");
    return OM_MSG_RET_ERROR;
  }
}

static void RevCompFeats (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  CodeBreakPtr  cbp;
  CdRegionPtr   crp;
  RnaRefPtr     rrp;
  SeqAnnotPtr   sap;
  SeqFeatPtr    sfp;
  SeqIdPtr      sip;
  SeqLocPtr     slp;
  Boolean       split;
  tRNAPtr       trp;

  if (mydata == NULL) return;
  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
  } else return;
  bsp = (BioseqPtr) mydata;
  if (bsp == NULL) return;
  if (! ISA_na (bsp->mol)) return;
  while (sap != NULL) {
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      while (sfp != NULL) {
        sip = SeqLocId (sfp->location);
        if (sip != NULL) {
          if (SeqIdIn (sip, bsp->id)) {
            slp = SeqLocCopyRegion (sip, sfp->location, bsp, 0,
                                    bsp->length - 1, Seq_strand_minus, &split);
            sfp->location = SeqLocFree (sfp->location);
            sfp->location = slp;
            switch (sfp->data.choice) {
              case SEQFEAT_CDREGION :
                crp = (CdRegionPtr) sfp->data.value.ptrvalue;
                if (crp != NULL) {
                  for (cbp = crp->code_break; cbp != NULL; cbp = cbp->next) {
                    sip = SeqLocId (cbp->loc);
                    slp = SeqLocCopyRegion (sip, cbp->loc, bsp, 0,
                                            bsp->length - 1, Seq_strand_minus, &split);
                    cbp->loc = SeqLocFree (cbp->loc);
                    cbp->loc = slp;
                  }
                }
                break;
              case SEQFEAT_RNA :
                rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
                if (rrp != NULL && rrp->ext.choice == 2) {
                  trp = (tRNAPtr) rrp->ext.value.ptrvalue;
                  if (trp != NULL && trp->anticodon != NULL) {
                    sip = SeqLocId (trp->anticodon);
                    slp = SeqLocCopyRegion (sip, trp->anticodon, bsp, 0,
                                            bsp->length - 1, Seq_strand_minus, &split);
                    trp->anticodon = SeqLocFree (trp->anticodon);
                    trp->anticodon = slp;
                  }
                }
                break;
              default :
                break;
            }
          }
        }
        sfp = sfp->next;
      }
    }
    sap = sap->next;
  }
}

static void AddBspToVnpCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr        bsp;
  ValNodePtr PNTR  vnpp;

  vnpp = (ValNodePtr PNTR) mydata;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp != NULL && ISA_na (bsp->mol)) {
      ValNodeAddPointer (vnpp, 0, (Pointer) bsp);
    }
  }
}

static Boolean AddBspToVnp (GatherContextPtr gcp)

{
  BioseqPtr        bsp;
  BioseqSetPtr     bssp;
  SeqEntryPtr      sep;
  ValNodePtr PNTR  vnpp;

  vnpp = (ValNodePtr PNTR) gcp->userdata;
  if (vnpp == NULL) return TRUE;
  bsp = NULL;
  if (gcp->thistype == OBJ_BIOSEQ) {
    bsp = (BioseqPtr) gcp->thisitem;
  } else if (gcp->thistype == OBJ_BIOSEQSET) {
    bssp = (BioseqSetPtr) gcp->thisitem;
    if (bssp != NULL) {
      sep = SeqMgrGetSeqEntryForData (bssp);
      if (sep != NULL) {
        SeqEntryExplore (sep, (Pointer) vnpp, AddBspToVnpCallback);
      }
    }
    return TRUE;
  } else return TRUE;
  if (bsp == NULL) return TRUE;
  ValNodeAddPointer (vnpp, 0, (Pointer) bsp);
  return TRUE;
}

typedef Boolean (LIBCALL *BioseqFunc) (BioseqPtr);

static void ProcessMultipleBioseqFunctions (OMProcControlPtr ompcp, BioseqFunc func,
                                            Boolean revCompFeats)

{
  BioseqPtr     bsp;
  ValNodePtr    head;
  SelStructPtr  sel;
  SeqEntryPtr   sep;
  ValNodePtr    vnp;

  if (ompcp == NULL || ompcp->input_entityID == 0 || func == NULL) return;
  head = NULL;
  for (sel = ObjMgrGetSelected (); sel != NULL; sel = sel->next) {
    GatherItem (sel->entityID, sel->itemID, sel->itemtype,
                (Pointer) &head, AddBspToVnp);
  }
  if (head == NULL) return;
  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    bsp = (BioseqPtr) vnp->data.ptrvalue;
    func (bsp);
    if (revCompFeats) {
      if (bsp->repr == Seq_repr_raw || bsp->repr == Seq_repr_const) {
        sep = GetTopSeqEntryForEntityID (ompcp->input_entityID);
        if (sep != NULL) {
          SeqEntryExplore (sep, (Pointer) bsp, RevCompFeats);
        }
      }
    }
  }
  ValNodeFree (head);
  ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, 0, 0);
}

static Int2 LIBCALLBACK RevCompFuncFeat (Pointer data)

{
  /*
  BioseqPtr         bsp;
  OMProcControlPtr  ompcp;
  SeqEntryPtr       sep;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL) return OM_MSG_RET_ERROR;
  switch (ompcp->input_itemtype) {
    case OBJ_BIOSEQ :
      bsp = (BioseqPtr) ompcp->input_data;
      break;
    default :
      return OM_MSG_RET_ERROR;
      break;
  }
  if (bsp == NULL) return OM_MSG_RET_ERROR;
  BioseqRevComp (bsp);
  if (bsp->repr == Seq_repr_raw || bsp->repr == Seq_repr_const) {
    sep = GetTopSeqEntryForEntityID (ompcp->input_entityID);
    if (sep != NULL) {
      SeqEntryExplore (sep, (Pointer) bsp, RevCompFeats);
    }
  }
  ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, 0, 0);
  */
  ProcessMultipleBioseqFunctions ((OMProcControlPtr) data, BioseqRevComp, TRUE);
  return OM_MSG_RET_DONE;
}

static Int2 LIBCALLBACK RevCompFunc (Pointer data)

{
  /*
  BioseqPtr         bsp;
  OMProcControlPtr  ompcp;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL) return OM_MSG_RET_ERROR;
  switch (ompcp->input_itemtype) {
    case OBJ_BIOSEQ :
      bsp = (BioseqPtr) ompcp->input_data;
      break;
    default :
      return OM_MSG_RET_ERROR;
      break;
  }
  if (bsp == NULL) return OM_MSG_RET_ERROR;
  BioseqRevComp (bsp);
  ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, 0, 0);
  */
  ProcessMultipleBioseqFunctions ((OMProcControlPtr) data, BioseqRevComp, FALSE);
  return OM_MSG_RET_DONE;
}

static Int2 LIBCALLBACK RevFunc (Pointer data)

{
  /*
  BioseqPtr         bsp;
  OMProcControlPtr  ompcp;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL) return OM_MSG_RET_ERROR;
  switch (ompcp->input_itemtype) {
    case OBJ_BIOSEQ :
      bsp = (BioseqPtr) ompcp->input_data;
      break;
    default :
      return OM_MSG_RET_ERROR;
      break;
  }
  if (bsp == NULL) return OM_MSG_RET_ERROR;
  BioseqReverse (bsp);
  ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, 0, 0);
  */
  ProcessMultipleBioseqFunctions ((OMProcControlPtr) data, BioseqReverse, FALSE);
  return OM_MSG_RET_DONE;
}

static Int2 LIBCALLBACK CompFunc (Pointer data)

{
  /*
  BioseqPtr         bsp;
  OMProcControlPtr  ompcp;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL) return OM_MSG_RET_ERROR;
  switch (ompcp->input_itemtype) {
    case OBJ_BIOSEQ :
      bsp = (BioseqPtr) ompcp->input_data;
      break;
    default :
      return OM_MSG_RET_ERROR;
      break;
  }
  if (bsp == NULL) return OM_MSG_RET_ERROR;
  BioseqComplement (bsp);
  ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, 0, 0);
  */
  ProcessMultipleBioseqFunctions ((OMProcControlPtr) data, BioseqComplement, FALSE);
  return OM_MSG_RET_DONE;
}

typedef struct orphandata {
  ValNodePtr  bspfromcds;
  ValNodePtr  bspinentry;
} OrphanData, PNTR OrphanDataPtr;

static void GetCdsProducts (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  OrphanDataPtr  odp;
  SeqAnnotPtr    sap;
  SeqFeatPtr     sfp;
  SeqIdPtr       sip;

  if (mydata == NULL) return;
  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  odp = (OrphanDataPtr) mydata;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
    if (ISA_aa (bsp->mol)) {
      ValNodeAddPointer (&(odp->bspinentry), 0, (Pointer) bsp);
    }
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
  } else return;
  while (sap != NULL) {
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      while (sfp != NULL) {
        if (sfp->data.choice == SEQFEAT_CDREGION) {
          if (sfp->product != NULL) {
            sip = SeqLocId (sfp->product);
            if (sip != NULL) {
              bsp = BioseqFind (sip);
              if (bsp != NULL) {
                ValNodeAddPointer (&(odp->bspfromcds), 0, (Pointer) bsp);
              }
            }
          }
        }
        sfp = sfp->next;
      }
    }
    sap = sap->next;
  }
}

static int LIBCALLBACK SortByVnpDataPtrvalue (VoidPtr ptr1, VoidPtr ptr2)

{
  ValNodePtr   vnp1;
  ValNodePtr   vnp2;

  if (ptr1 != NULL && ptr2 != NULL) {
    vnp1 = *((ValNodePtr PNTR) ptr1);
    vnp2 = *((ValNodePtr PNTR) ptr2);
    if (vnp1 != NULL && vnp2 != NULL) {
      if (vnp1->data.ptrvalue > vnp2->data.ptrvalue) {
        return 1;
      } else if (vnp1->data.ptrvalue < vnp2->data.ptrvalue) {
        return -1;
      } else {
        return 0;
      }
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}

extern void RemoveOrphanProteins (Uint2 entityID, SeqEntryPtr sep)

{
  Int2        doit;
  OrphanData  od;
  ValNodePtr  vnp;
  ValNodePtr  vnp1;
  ValNodePtr  vnp2;
  BioseqPtr   protein_bsp;

  if (sep == NULL || entityID == 0) return;
  od.bspfromcds = NULL;
  od.bspinentry = NULL;
  /* This function collects a list of all proteins in the entry
   * and a list of all proteins that are products of a CDS
   */
  SeqEntryExplore (sep, (Pointer) &od, GetCdsProducts);

  /* If there are no proteins in the entry at all, we're done */
  if (od.bspinentry == NULL)
  {
    ValNodeFree (od.bspinentry);
    return;
  }

  /* If we found any CDS product proteins, we need to sort the two
   * lists and mark the ones in both lists as "keep".
   * If there are no CDS product proteins, then all of the proteins
   * in the entry should be removed.
   */
  if (od.bspfromcds != NULL) {
    od.bspinentry = SortValNode (od.bspinentry, SortByVnpDataPtrvalue);
    od.bspfromcds = SortValNode (od.bspfromcds, SortByVnpDataPtrvalue);
    vnp1 = od.bspfromcds;
    vnp2 = od.bspinentry;
    while (vnp1 != NULL && vnp2 != NULL) {
      if (vnp1->data.ptrvalue < vnp2->data.ptrvalue) {
        vnp1 = vnp1->next;
      } else if (vnp2->data.ptrvalue < vnp1->data.ptrvalue) {
        vnp2 = vnp2->next;
      } else {
        vnp2->data.ptrvalue = NULL;
        vnp1 = vnp1->next;
        vnp2 = vnp2->next;
      }
    }
  }

  /* Now we count how many proteins are in the entry but not a 
   * product of a CDS
   */
  doit = 0;
  for (vnp = od.bspinentry; vnp != NULL; vnp = vnp->next) {
    if (vnp->data.ptrvalue != NULL) {
      doit++;
    }
  }
  if (doit > 0) {
    if (Message (MSG_YN, "Do you want to remove %d orphaned proteins?", (int) doit) == ANS_YES) {
      vnp = od.bspinentry;
      while (vnp != NULL) {
        if (vnp->data.ptrvalue != NULL) {
          protein_bsp = vnp->data.ptrvalue;
          protein_bsp->idx.deleteme = TRUE;
        }
        vnp = vnp->next;
      }
      DeleteMarkedObjects (entityID, 0, NULL);
      ObjMgrSetDirtyFlag (entityID, TRUE);
      ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
    }
  }
  
  ValNodeFree (od.bspinentry);
  ValNodeFree (od.bspfromcds);
}

static Int2 LIBCALLBACK NormalizeNucProts (Pointer data)

{
  OMProcControlPtr  ompcp;
  SeqEntryPtr       sep;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL) return OM_MSG_RET_ERROR;
  sep = GetTopSeqEntryForEntityID (ompcp->input_entityID);
  RemoveOrphanProteins (ompcp->input_entityID, sep);
  RenormalizeNucProtSets (sep, TRUE);
  ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, 0, 0);
  return OM_MSG_RET_DONE;
}

static void RemovePopPhyMutSet (SeqEntryPtr sep, Boolean forceGBClass, Boolean forceAll)

{
  SeqAnnotPtr   annot;
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  ValNodePtr    descr;
  SeqAnnotPtr   sap;
  SeqEntryPtr   seqentry;

  if (sep == NULL || IS_Bioseq (sep)) return;
  bssp = (BioseqSetPtr) sep->data.ptrvalue;
  if (bssp == NULL) return;
  if (forceAll) {
  } else if (forceGBClass) {
    if ((bssp->_class < 13 || bssp->_class > 15) && bssp->_class != 7) return;
  } else {
    if (bssp->_class < 13 || bssp->_class > 15) return;
  }
  seqentry = bssp->seq_set;
  if (seqentry == NULL) return;
  if (! forceAll) {
    if (! forceGBClass) {
      if (seqentry->next != NULL) return;
    }
  }
  descr = bssp->descr;
  bssp->descr = NULL;
  annot = bssp->annot;
  bssp->annot = NULL;
  sep->choice = seqentry->choice;
  sep->data.ptrvalue = seqentry->data.ptrvalue;
  sep->next = seqentry->next;
  seqentry->data.ptrvalue = NULL;
  seqentry->next = NULL;
  bssp->seq_set = NULL;
  SeqEntryFree (seqentry);
  BioseqSetFree (bssp);
  sap = NULL;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    ValNodeLink (&(bsp->descr), descr);
    if (bsp->annot == NULL) {
      bsp->annot = annot;
      annot = NULL;
    } else {
      sap = bsp->annot;
    }
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    ValNodeLink (&(bssp->descr), descr);
    if (bssp->annot == NULL) {
      bssp->annot = annot;
      annot = NULL;
    } else {
      sap = bssp->annot;
    }
  }
  if (sap != NULL) {
    while (sap->next != NULL) {
      sap = sap->next;
    }
    sap->next = annot;
  }
}

static BioseqSetPtr InsetNewSet (BioseqSetPtr bssp, Uint1 _class)

{
  BioseqSetPtr  popphymut;
  SeqEntryPtr   tmp;

  if (bssp == NULL) return NULL;
  popphymut = BioseqSetNew ();
  if (popphymut == NULL) return NULL;
  popphymut->_class = _class;
  popphymut->seq_set = bssp->seq_set;
  tmp = SeqEntryNew ();
  if (tmp == NULL) return NULL;
  tmp->choice = 2;
  tmp->data.ptrvalue = (Pointer) popphymut;
  bssp->seq_set = tmp;
  return popphymut;
}

static Int2 LIBCALLBACK PopWithinGenBankSet (Pointer data)

{
  BioseqSetPtr      bssp;
  BioseqSetPtr      newbssp;
  ObjMgrDataPtr     omdptop;
  ObjMgrData        omdata;
  OMProcControlPtr  ompcp;
  Uint2             parenttype;
  Pointer           parentptr;
  SeqEntryPtr       sep;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;
  switch (ompcp->input_itemtype) {
    case OBJ_BIOSEQ :
      break;
    case OBJ_BIOSEQSET :
      break;
    case 0 :
      return OM_MSG_RET_ERROR;
    default :
      break;
  }
  if (ompcp->input_data == NULL) return OM_MSG_RET_ERROR;
  sep = GetTopSeqEntryForEntityID (ompcp->input_entityID);
  if (sep == NULL) return OM_MSG_RET_ERROR;
  SaveSeqEntryObjMgrData (sep, &omdptop, &omdata);
  GetSeqEntryParent (sep, &parentptr, &parenttype);
  RemoveDupGenBankSets (sep);
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL) {
      newbssp = InsetNewSet (bssp, BioseqseqSet_class_pop_set);
      if (newbssp != NULL) {
        newbssp->descr = bssp->descr;
        bssp->descr = NULL;
        newbssp->annot = bssp->annot;
        bssp->annot = NULL;
      }
    }
  }
  SeqMgrLinkSeqEntry (sep, parenttype, parentptr);
  RestoreSeqEntryObjMgrData (sep, omdptop, &omdata);
  ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, 0, 0);
  return OM_MSG_RET_DONE;
}

extern Int2 LIBCALLBACK RemoveExtraneousSets (Pointer data)

{
  BioseqPtr         bsp;
  BioseqSetPtr      bssp;
  Uint1             _class;
  ValNodePtr        descr1 = NULL;
  ValNodePtr        descr2 = NULL;
  BioseqSetPtr      first = NULL;
  ObjMgrDataPtr     omdptop;
  ObjMgrData        omdata;
  OMProcControlPtr  ompcp;
  Uint2             parenttype;
  Pointer           parentptr;
  SeqEntryPtr       sep;
  SeqEntryPtr       tmp;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL) return OM_MSG_RET_ERROR;
  sep = GetTopSeqEntryForEntityID (ompcp->input_entityID);
  if (sep != NULL && IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && bssp->_class == 7) {
      descr1 = bssp->descr;
      bssp->descr = NULL;
      _class = 0;
      for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
        if (IS_Bioseq (tmp)) return OM_MSG_RET_DONE;
        if (_class == 0 && IS_Bioseq_set (tmp)) {
          first = (BioseqSetPtr) tmp->data.ptrvalue;
          if (first != NULL) {
            _class = first->_class;
          }
        }
      }
      if (first != NULL && bssp->seq_set != NULL && bssp->seq_set->next == NULL) {
        descr2 = first->descr;
        first->descr = NULL;
      }
      SaveSeqEntryObjMgrData (sep, &omdptop, &omdata);
      GetSeqEntryParent (sep, &parentptr, &parenttype);
      if (_class >= 13 && _class <= 15) {
        for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
          RemovePopPhyMutSet (tmp, FALSE, FALSE);
        }
        InsetNewSet (bssp, _class);
      }
      if (_class == 7) {
        tmp = bssp->seq_set;
        if (tmp != NULL && IS_Bioseq_set (tmp)) {
          RemovePopPhyMutSet (tmp, TRUE, FALSE);
        }
      }
      if (IS_Bioseq (sep)) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        ValNodeLink (&(bsp->descr), descr1);
        ValNodeLink (&(bsp->descr), descr2);
      } else if (IS_Bioseq_set (sep)) {
        bssp = (BioseqSetPtr) sep->data.ptrvalue;
        ValNodeLink (&(bssp->descr), descr1);
        ValNodeLink (&(bssp->descr), descr2);
      }
      SeqMgrLinkSeqEntry (sep, parenttype, parentptr);
      RestoreSeqEntryObjMgrData (sep, omdptop, &omdata);
      ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
      ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, 0, 0);
    }
  }
  return OM_MSG_RET_DONE;
}

static Int2 LIBCALLBACK RepairMessedUpRecord (Pointer data)

{
  BioseqSetPtr      bssp;
  SeqEntryPtr       last;
  SeqEntryPtr       next;
  ObjMgrDataPtr     omdptop;
  ObjMgrData        omdata;
  OMProcControlPtr  ompcp;
  Uint2             parenttype;
  Pointer           parentptr;
  SeqEntryPtr       sep;
  SeqEntryPtr       tmp;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL) return OM_MSG_RET_ERROR;
  sep = GetTopSeqEntryForEntityID (ompcp->input_entityID);
  if (sep == NULL) return OM_MSG_RET_ERROR;
  if (! IS_Bioseq_set (sep)) return OM_MSG_RET_ERROR;
  bssp = (BioseqSetPtr) sep->data.ptrvalue;
  if (bssp == NULL || bssp->_class != 7) return OM_MSG_RET_ERROR;
  SaveSeqEntryObjMgrData (sep, &omdptop, &omdata);
  GetSeqEntryParent (sep, &parentptr, &parenttype);
  tmp = bssp->seq_set;
  while (tmp != NULL) {
    next = tmp->next;
    tmp->next = NULL;
    if (IS_Bioseq_set (tmp)) {
      RemovePopPhyMutSet (tmp, FALSE, TRUE);
    }
    last = bssp->seq_set;
    while (last->next != NULL) {
      last = last->next;
    }
    last->next = next;
    tmp = next;
  }
  SeqMgrLinkSeqEntry (sep, parenttype, parentptr);
  RestoreSeqEntryObjMgrData (sep, omdptop, &omdata);
  ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, 0, 0);
  return OM_MSG_RET_DONE;
}

typedef struct featsints {
  SeqFeatPtr  sfp;
  Int4        leftend;
  Int4        rightend;
  Uint1       strand;
} FeatsIntsData, PNTR FeatsIntsPtr;

static void CommonGatherRBSorGene (ValNodePtr PNTR vnpp, SeqFeatPtr sfp, Uint2 entityID)

{
  BioseqPtr     bsp;
  FeatsIntsPtr  fip;
  GeneRefPtr    grp;
  Int4          start;
  Int4          stop;
  Char          str [128];
  ValNodePtr    vnp;

  if (vnpp == NULL || sfp == NULL || entityID == 0) return;
  bsp = GetBioseqGivenSeqLoc (sfp->location, entityID);
  if (bsp == NULL) return;
  fip = MemNew (sizeof (FeatsIntsData));
  if (fip == NULL) return;
  fip->sfp = sfp;
  fip->leftend = GetOffsetInBioseq (sfp->location, bsp, SEQLOC_LEFT_END);
  fip->rightend = GetOffsetInBioseq (sfp->location, bsp, SEQLOC_RIGHT_END);
  fip->strand = SeqLocStrand (sfp->location);
  if (sfp->data.choice == SEQFEAT_GENE) {
    start = GetOffsetInBioseq (sfp->location, bsp, SEQLOC_START);
    stop = GetOffsetInBioseq (sfp->location, bsp, SEQLOC_STOP);
    if ((start > stop && fip->strand == Seq_strand_plus) ||
        (start < stop && fip->strand == Seq_strand_minus)) {
      str [0] = '\0';
      grp = (GeneRefPtr) sfp->data.value.ptrvalue;
      if (grp != NULL) {
        StringNCpy_0 (str, grp->locus, sizeof (str));
        if (StringHasNoText (str)) {
          vnp = grp->syn;
          if (vnp != NULL) {
            StringNCpy_0 (str, (CharPtr) vnp->data.ptrvalue, sizeof (str));
          }
        }
        if (StringHasNoText (str)) {
          StringNCpy_0 (str, grp->desc, sizeof (str));
        }
        if (StringHasNoText (str)) {
          StringNCpy_0 (str, "?", sizeof (str));
        }
      }
      Message (MSG_POST, "Ignoring gene '%s' going across origin, please fix manually", str);
      return;
    }
  }
  ValNodeAddPointer (vnpp, 0, (Pointer) fip);
}

static Boolean GetRBSGatherFunc (GatherContextPtr gcp)

{
  SeqFeatPtr  sfp;
  Uint1       type;
  ValNodePtr  PNTR vnpp;

  if (gcp == NULL || gcp->thisitem == NULL || gcp->userdata == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  switch (sfp->data.choice) {
    case SEQFEAT_IMP :
      type = FindFeatDefType (sfp);
      if (type != FEATDEF_RBS) return TRUE;
      vnpp = (ValNodePtr PNTR) gcp->userdata;
      CommonGatherRBSorGene (vnpp, sfp, gcp->entityID);
      break;
    default :
      break;
  }
  return TRUE;
}

static Boolean GetGeneGatherFunc (GatherContextPtr gcp)

{
  SeqFeatPtr  sfp;
  ValNodePtr  PNTR vnpp;

  if (gcp == NULL || gcp->thisitem == NULL || gcp->userdata == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  switch (sfp->data.choice) {
    case SEQFEAT_GENE :
      vnpp = (ValNodePtr PNTR) gcp->userdata;
      CommonGatherRBSorGene (vnpp, sfp, gcp->entityID);
      break;
    default :
      break;
  }
  return TRUE;
}

static Boolean GetNonGeneGatherFunc (GatherContextPtr gcp)

{
  SeqFeatPtr  sfp;
  ValNodePtr  PNTR vnpp;

  if (gcp == NULL || gcp->thisitem == NULL || gcp->userdata == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  switch (sfp->data.choice) {
    case SEQFEAT_GENE :
    case SEQFEAT_PROT :
      break;
    default :
      vnpp = (ValNodePtr PNTR) gcp->userdata;
      CommonGatherRBSorGene (vnpp, sfp, gcp->entityID);
      break;
  }
  return TRUE;
}

static int LIBCALLBACK SortFeatByLocation (VoidPtr ptr1, VoidPtr ptr2)

{
  FeatsIntsPtr  fip1;
  FeatsIntsPtr  fip2;
  ValNodePtr    vnp1;
  ValNodePtr    vnp2;

  if (ptr1 != NULL && ptr2 != NULL) {
    vnp1 = *((ValNodePtr PNTR) ptr1);
    vnp2 = *((ValNodePtr PNTR) ptr2);
    if (vnp1 != NULL && vnp2 != NULL) {
      fip1 = (FeatsIntsPtr) vnp1->data.ptrvalue;
      fip2 = (FeatsIntsPtr) vnp2->data.ptrvalue;
      if (fip1 != NULL && fip2 != NULL) {
        if (fip1->leftend > fip2->leftend) {
          return 1;
        } else if (fip1->leftend < fip2->leftend) {
          return -1;
        } else if (fip1->rightend > fip2->rightend) {
          return 1;
        } else if (fip1->rightend < fip2->rightend) {
          return -1;
        } else {
          return 0;
        }
      }
    }
  }
  return 0;
}

static void ExtendGeneByRbs (SeqFeatPtr sfpr, SeqFeatPtr sfpg, Uint2 entityID)

{
  BioseqPtr       bsp;
  SeqFeatXrefPtr  curr;
  SeqFeatXrefPtr  PNTR last;
  SeqFeatXrefPtr  next;
  SeqLocPtr       slp;

  if (sfpr == NULL || sfpg == NULL || sfpg->data.choice != SEQFEAT_GENE) return;
  bsp = GetBioseqGivenSeqLoc (sfpg->location, entityID);
  if (bsp == NULL) return;
  slp = SeqLocMerge (bsp, sfpg->location, sfpr->location, TRUE, TRUE, FALSE);
  sfpg->location = SeqLocFree (sfpg->location);
  sfpg->location = slp;
  last = (SeqFeatXrefPtr PNTR) &(sfpr->xref);
  curr = sfpr->xref;
  while (curr != NULL) {
    next = curr->next;
    if (curr->data.choice == SEQFEAT_GENE) {
      *last = next;
      curr->next = NULL;
      SeqFeatXrefFree (curr);
    } else {
      last = &(curr->next);
    }
    curr = next;
  }
}

static void MakeGeneXref (SeqFeatPtr sfpr, SeqFeatPtr sfpg)

{
  SeqFeatXrefPtr  curr;
  GeneRefPtr      grp;
  SeqFeatXrefPtr  PNTR last;
  CharPtr         locus;
  SeqFeatXrefPtr  next;
  SeqFeatXrefPtr  xref;

  if (sfpr == NULL || sfpg == NULL || sfpg->data.choice != SEQFEAT_GENE) return;
  grp = (GeneRefPtr) sfpg->data.value.ptrvalue;
  if (grp == NULL) return;
  locus = grp->locus;
  if (StringHasNoText (locus)) return;
  last = (SeqFeatXrefPtr PNTR) &(sfpr->xref);
  curr = sfpr->xref;
  while (curr != NULL) {
    next = curr->next;
    if (curr->data.choice == SEQFEAT_GENE) {
      *last = next;
      curr->next = NULL;
      SeqFeatXrefFree (curr);
    } else {
      last = &(curr->next);
    }
    curr = next;
  }
  grp = GeneRefNew ();
  if (grp == NULL) return;
  grp->locus = StringSave (locus);
  TrimSpacesAroundString (grp->locus);
  xref = SeqFeatXrefNew ();
  sfpr->xref = xref;
  if (xref != NULL) {
    xref->data.choice = SEQFEAT_GENE;
    xref->data.value.ptrvalue = (Pointer) grp;
  }
}

static Int2 FindNextGeneFeat (ValNodePtr PNTR genelist, Int4 max, FeatsIntsPtr fipr)

{
  Int4          compare;
  FeatsIntsPtr  fipg;
  Int4          left;
  Int4          mid;
  Int4          right;
  ValNodePtr    vnpg;

  if (genelist == NULL || fipr == NULL || max < 1) return 0;
  mid = 0;
  left = 1;
  right = max;
  while (left <= right) {
    mid = (left + right) / 2;
    vnpg = genelist [mid - 1];
    fipg = (FeatsIntsPtr) vnpg->data.ptrvalue;
    if (fipr->strand == Seq_strand_minus) {
      compare = fipg->rightend - fipr->leftend;
    } else {
      compare = fipr->rightend - fipg->leftend;
    }
    if (compare <= 0) {
      right = mid - 1;
    }
    if (compare >= 0) {
      left = mid + 1;
    }
  }
  if (left <= right + 1) {
    if (fipr->strand == Seq_strand_minus) {
      mid += 2;
      mid = MIN (mid, max);
      vnpg = genelist [mid - 1];
      fipg = (FeatsIntsPtr) vnpg->data.ptrvalue;
      while (fipg->rightend > fipr->leftend && mid > 1) {
        mid--;
        vnpg = genelist [mid - 1];
        fipg = (FeatsIntsPtr) vnpg->data.ptrvalue;
      }
    } else {
      mid -= 2;
      mid = MAX (mid, 1);
      vnpg = genelist [mid - 1];
      fipg = (FeatsIntsPtr) vnpg->data.ptrvalue;
      while (fipr->rightend > fipg->leftend && mid < max) {
        mid++;
        vnpg = genelist [mid - 1];
        fipg = (FeatsIntsPtr) vnpg->data.ptrvalue;
      }
    }
  }
  return mid;
}

static Boolean  rbsDirty;
static Boolean  rbsChoiceAsked;
static Boolean  rbsExtend;

static void FixupRBSGenes (Uint2 entityID, SeqEntryPtr sep)

{
  MsgAnswer     ans;
  BioseqSetPtr  bssp;
  FeatsIntsPtr  fipg;
  FeatsIntsPtr  fipr;
  ValNodePtr    PNTR genelist;
  ValNodePtr    genes;
  GatherScope   gs;
  Int2          i;
  Int4          max;
  ValNodePtr    rbss;
  ValNodePtr    tmp;
  ValNodePtr    vnpg;
  ValNodePtr    vnpr;

  if (sep == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 7 ||
                         (IsPopPhyEtcSet (bssp->_class)))) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        FixupRBSGenes (entityID, sep);
      }
      return;
    }
  }

  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet ((Pointer) (gs.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.scope = sep;
  rbss = NULL;
  GatherEntity (entityID, (Pointer) (&rbss), GetRBSGatherFunc, &gs);
  rbss = SortValNode (rbss, SortFeatByLocation);
  genes = NULL;
  GatherEntity (entityID, (Pointer) (&genes), GetGeneGatherFunc, &gs);
  genes = SortValNode (genes, SortFeatByLocation);

  for (vnpg = genes, i = 0; vnpg != NULL; vnpg = vnpg->next) {
    i++;
  }
  genelist = (ValNodePtr PNTR) MemNew (sizeof (ValNodePtr) * (i + 1));
  if (genelist != NULL) {
    for (tmp = genes, i = 0; tmp != NULL; tmp = tmp->next) {
      genelist [i] = tmp;
      i++;
    }
    max = (Int4) i;

    for (vnpr = rbss; vnpr != NULL; vnpr = vnpr->next) {
      fipr = (FeatsIntsPtr) vnpr->data.ptrvalue;
      i = FindNextGeneFeat (genelist, max, fipr);
      if (i > 0) {
        vnpg = genelist [i - 1];
        fipg = (FeatsIntsPtr) vnpg->data.ptrvalue;
        rbsDirty = TRUE;
        if (! rbsChoiceAsked) {
          rbsChoiceAsked = TRUE;
          ans = Message (MSG_YN, "Extend Gene Feature?");
          rbsExtend = (Boolean) (ans == ANS_YES);
        }
        if (rbsExtend) {
          ExtendGeneByRbs (fipr->sfp, fipg->sfp, entityID);
        } else {
          MakeGeneXref (fipr->sfp, fipg->sfp);
        }
      }
    }
  }

  MemFree (genelist);
  ValNodeFreeData (rbss);
  ValNodeFreeData (genes);
}

static Int2 LIBCALLBACK FixupRBS (Pointer data)

{
  OMProcControlPtr  ompcp;
  SeqEntryPtr       sep;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->input_entityID == 0) {
    Message (MSG_ERROR, "Please select a Bioseq or BioseqSet");
    return OM_MSG_RET_ERROR;
  }
  sep = GetTopSeqEntryForEntityID (ompcp->input_entityID);
  if (sep == NULL) {
    Message (MSG_ERROR, "Please select a Bioseq or BioseqSet");
    return OM_MSG_RET_ERROR;
  }
  rbsDirty = FALSE;
  rbsChoiceAsked = FALSE;
  rbsExtend = FALSE;
  WatchCursor ();
  Update ();
  FixupRBSGenes (ompcp->input_entityID, sep);
  ArrowCursor ();
  Update ();
  if (rbsDirty) {
    ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, 0, 0);
  }
  return OM_MSG_RET_DONE;
}

static Boolean LIBCALLBACK GetSeqs (BioseqPtr bsp, SeqMgrBioseqContextPtr context)

{
  FILE  *fp;
  Char  str [256];

  fp = (FILE *) context->userdata;
  if (BioseqLabel (bsp, str, sizeof (str) - 1, OM_LABEL_BOTH)) {
    fprintf (fp, "  Bioseq item %d %s\n", (int) context->itemID, str);
  }
  return TRUE;
}

static Boolean LIBCALLBACK GetSegs (SeqLocPtr slp, SeqMgrSegmentContextPtr context)

{
  FILE  *fp;
  Char  str [256];

  fp = (FILE *) context->userdata;
  if (SeqLocLabel (slp, str, sizeof (str) - 1, OM_LABEL_BOTH)) {
    fprintf (fp, "  SeqLoc item %d %s cumulative %ld\n", (int) context->itemID,
             str, (long) context->cumOffset);
  }
  return TRUE;
}

static Boolean LIBCALLBACK GetDescs (ValNodePtr sdp, SeqMgrDescContextPtr context)

{
  FILE  *fp;
  Char  str [256];

  fp = (FILE *) context->userdata;
  if (SeqDescLabel (sdp, str, sizeof (str) - 1, OM_LABEL_BOTH)) {
    fprintf (fp, "  Descriptor item %d index %d %s\n",
            (int) context->itemID, (int) context->index, str);
  }
  return TRUE;
}

static Boolean LIBCALLBACK GetFeats (SeqFeatPtr sfp, SeqMgrFeatContextPtr context)

{
  FILE  *fp;
  Char  str [256];

  fp = (FILE *) context->userdata;
  if (FeatDefLabel (sfp, str, sizeof (str) - 1, OM_LABEL_BOTH)) {
    fprintf (fp, "  Feature item %d index %d %s\n",
             (int) context->itemID, (int) context->index, str);
  }
  return TRUE;
}

static void DoBioseqReport (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr          bsp;
  SeqMgrDescContext  desccontext;
  SeqMgrFeatContext  featcontext;
  FILE               *fp;
  SeqFeatPtr         gene;
  Int2               i;
  Int4Ptr            ivals;
  Int2               numivals;
  ValNodePtr         sdp;
  SeqFeatPtr         sfp;
  Char               str [256];

  if (sep == NULL) return;
  if (! IS_Bioseq (sep)) return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL) return;

  fp = (FILE*) mydata;
  if (fp == NULL) return;

  if (BioseqLabel (bsp, str, sizeof (str) - 1, OM_LABEL_BOTH)) {
    fprintf (fp, "Bioseq %s\n\n", str);
  }

  if (bsp->repr == Seq_repr_seg) {
    fprintf (fp, " Exploring segments\n\n");
    SeqMgrExploreSegments (bsp, (Pointer) fp, GetSegs);
    fprintf (fp, "\n");
  }

  if (ISA_aa (bsp->mol)) {
    sfp = SeqMgrGetBestProteinFeature (bsp, NULL);
    if (FeatDefLabel (sfp, str, sizeof (str) - 1, OM_LABEL_BOTH)) {
      fprintf (fp, "  Best protein %s", str);
    }
    sfp = SeqMgrGetCDSgivenProduct (bsp, NULL);
    if (FeatDefLabel (sfp, str, sizeof (str) - 1, OM_LABEL_BOTH)) {
      fprintf (fp, ", best cds %s", str);
    }
    fprintf (fp, "\n\n");
  }


  fprintf (fp, " Exploring descriptors\n\n");
  SeqMgrExploreDescriptors (bsp, (Pointer) fp, GetDescs, NULL);
  fprintf (fp, "\n");

  fprintf (fp, " Exploring features\n\n");
  SeqMgrExploreFeatures (bsp, (Pointer) fp, GetFeats, NULL, NULL, NULL);
  fprintf (fp, "\n");


  fprintf (fp, " Collecting descriptors\n\n");
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, 0, &desccontext);
  while (sdp != NULL) {
    if (SeqDescLabel (sdp, str, sizeof (str) - 1, OM_LABEL_BOTH)) {
      fprintf (fp, "  Descriptor item %d index %d %s\n",
               (int) desccontext.itemID, (int) desccontext.index, str);
    }
    sdp = SeqMgrGetNextDescriptor (bsp, sdp, 0, &desccontext);
  }
  fprintf (fp, "\n");

  fprintf (fp, " Collecting features\n\n");
  sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &featcontext);
  while (sfp != NULL) {
    if (FeatDefLabel (sfp, str, sizeof (str) - 1, OM_LABEL_BOTH)) {
      fprintf (fp, "  Feature item %d index %d %s",
               (int) featcontext.itemID, (int) featcontext.index, str);
    }
    if (featcontext.seqfeattype != SEQFEAT_GENE) {
      gene = SeqMgrGetOverlappingGene (sfp->location, NULL);
      if (gene != NULL) {
        if (FeatDefLabel (gene, str, sizeof (str) - 1, OM_LABEL_BOTH)) {
          fprintf (fp, ", gene %s", str);
        }
      }
    }
    fprintf (fp, "\n");
    ivals = featcontext.ivals;
    numivals = featcontext.numivals * 2;
    if (ivals != NULL && numivals > 0) {
      fprintf (fp, "    (");
      for (i = 0; i < numivals; i += 2) {
        if (i > 0) {
          fprintf (fp, ", ");
        }
        fprintf (fp, "%ld-%ld", (long) ivals [i], (long) ivals [i + 1]);
      }
      fprintf (fp, ")\n");
    }
    sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &featcontext);
  }
  fprintf (fp, "\n");
}

static Int2 LIBCALLBACK DoBioseqIndexing (Pointer data)

{
  BioseqPtr         bsp = NULL;
  Uint2             entityID;
  FILE              *fp;
  OMProcControlPtr  ompcp;
  Char              path [PATH_MAX];
  SeqEntryPtr       sep;
  SeqFeatPtr        sfp = NULL;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->input_entityID == 0) {
    Message (MSG_ERROR, "Please select a Bioseq");
    return OM_MSG_RET_ERROR;
  }
  switch (ompcp->input_itemtype) {
    case OBJ_BIOSEQ :
      bsp = (BioseqPtr) ompcp->input_data;
      break;
    case OBJ_SEQFEAT:
      sfp = (SeqFeatPtr) ompcp->input_data;
      break;
    case 0 :
      return OM_MSG_RET_ERROR;
    default :
      return OM_MSG_RET_ERROR;
  }
  if (sfp != NULL) {
    bsp = BioseqFindFromSeqLoc (sfp->location);
  }
  if (bsp == NULL) {
    Message (MSG_ERROR, "Please select a Bioseq");
    return OM_MSG_RET_ERROR;
  }
  entityID = SeqMgrIndexFeatures (ompcp->input_entityID, NULL);
  if (entityID == 0) {
    Message (MSG_ERROR, "SeqMgrReindexBioseqExtraData failed");
    return OM_MSG_RET_ERROR;
  }
  sep = GetTopSeqEntryForEntityID (entityID);
  if (sep == NULL) return OM_MSG_RET_DONE;
  TmpNam (path);
  fp = FileOpen (path, "w");

  if (sfp != NULL) {
    
    fprintf (fp, " Exploring features filtered by location\n\n");
    SeqMgrExploreFeatures (bsp, (Pointer) fp, GetFeats, sfp->location, NULL, NULL);
    fprintf (fp, "\n");

  } else {

    fprintf (fp, "Exploring bioseqs\n\n");
    SeqMgrExploreBioseqs (entityID, 0, (Pointer) fp, GetSeqs, TRUE, TRUE, TRUE);
    fprintf (fp, "\n");

    SeqEntryExplore (sep, (Pointer) fp, DoBioseqReport);
  }
  FileClose (fp);
  LaunchGeneralTextViewer (path, "Bioseq Index Report");
  FileRemove (path);
  return OM_MSG_RET_DONE;
}

static Int2 LIBCALLBACK FindNonACGT (Pointer data)

{
  Byte              bases [400];
  BioseqPtr         bsp = NULL;
  Uint1             code = Seq_code_iupacna;
  Int2              ctr;
  FILE              *fp;
  Int2              i;
  Boolean           is_bad = FALSE;
  Int4              len;
  OMProcControlPtr  ompcp;
  Char              path [PATH_MAX];
  Uint1             residue;
  SeqPortPtr        spp = NULL;
  Int4              total = 0;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->input_entityID == 0) {
    Message (MSG_ERROR, "Please select a Bioseq");
    return OM_MSG_RET_ERROR;
  }
  switch (ompcp->input_itemtype) {
    case OBJ_BIOSEQ :
      bsp = (BioseqPtr) ompcp->input_data;
      break;
    default :
      break;
  }
  if (bsp == NULL) {
    Message (MSG_ERROR, "Please select a Bioseq");
    return OM_MSG_RET_ERROR;
  }
  if (ISA_aa (bsp->mol)) {
    code = Seq_code_ncbieaa;
  }

  spp = SeqPortNew (bsp, 0, -1, 0, code);
  len = bsp->length;

  if (spp == NULL) return OM_MSG_RET_ERROR;

  TmpNam (path);
  fp = FileOpen (path, "w");

  /* use SeqPortRead rather than SeqPortGetResidue for faster performance */

  ctr = SeqPortRead (spp, bases, sizeof (bases));
  i = 0;
  residue = (Uint1) bases [i];
  while (residue != SEQPORT_EOF) {
    if (IS_residue (residue)) {
      total++;
      switch (residue) {
        case 'A' :
        case 'C' :
        case 'G' :
        case 'T' :
          break;
        default :
          fprintf (fp, "Bad residue '%c' at position %ld\n", (char) residue, (long) total);
          is_bad = TRUE;
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

  SeqPortFree (spp);

  if (! is_bad) {
    fprintf (fp, "No ambiguous residues found");
  }

  FileClose (fp);
  LaunchGeneralTextViewer (path, "Bioseq Index Report");
  FileRemove (path);
  return OM_MSG_RET_DONE;
}

static void NEAR RevStringUpper (CharPtr str)
{
	CharPtr nd;
	Char tmp;

		if (str == NULL)
			return;
    nd = str;
	while (*nd != '\0')
		nd++;
	nd--;

	while (nd > str)
	{
		tmp = TO_UPPER(*nd);
		*nd = TO_UPPER(*str);
		*str = tmp;
		nd--; str++;
	}

	if (nd == str)
		*nd = TO_UPPER(*nd);
	return;
}

static Int2 LIBCALLBACK ReportDeltaSegments (Pointer data)

{
  BioseqPtr         bsp = NULL;
  BioseqExtraPtr    bspextra;
  FILE              *fp;
  Int4              i;
  Char              id [64];
  ObjMgrDataPtr     omdp;
  OMProcControlPtr  ompcp;
  SMSeqIdxPtr PNTR  partsByLoc;
  Char              path [PATH_MAX];
  SMSeqIdxPtr       sidx;
  CharPtr           strand;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->input_entityID == 0) {
    Message (MSG_ERROR, "Please select a Bioseq");
    return OM_MSG_RET_ERROR;
  }
  switch (ompcp->input_itemtype) {
    case OBJ_BIOSEQ :
      bsp = (BioseqPtr) ompcp->input_data;
      break;
    case 0 :
      return OM_MSG_RET_ERROR;
    default :
      return OM_MSG_RET_ERROR;
  }
  if (bsp == NULL) {
    Message (MSG_ERROR, "Please select a Bioseq");
    return OM_MSG_RET_ERROR;
  }
  omdp = (ObjMgrDataPtr) bsp->omdp;
  if (omdp == NULL || omdp->datatype != OBJ_BIOSEQ) return OM_MSG_RET_ERROR;
  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return OM_MSG_RET_ERROR;

  partsByLoc = bspextra->partsByLoc;
  if (partsByLoc == NULL || bspextra->numsegs < 1) return OM_MSG_RET_ERROR;

  TmpNam (path);
  fp = FileOpen (path, "w");

  for (i = 0; i < bspextra->numsegs; i++) {
    sidx = partsByLoc [i];
    if (sidx == NULL) continue;
    if (sidx->strand == Seq_strand_minus) {
      strand = "-";
    } else {
      strand = "+";
    }
    StringNCpy_0 (id, sidx->seqIdOfPart, sizeof (id) - 30);
    RevStringUpper (id);
    StringCat (id, "                    ");
    id [20] = '\0';
    fprintf (fp, "%s%9ld%9ld%9ld%9ld %s\n", id,
             (long) sidx->cumOffset, (long) (sidx->to - sidx->from + 1),
             (long) sidx->from, (long) sidx->to, strand);
  }

  FileClose (fp);
  LaunchGeneralTextViewer (path, "Bioseq Segment Report");
  FileRemove (path);
  return OM_MSG_RET_DONE;
}

static Int2 FindBestGeneFeat (ValNodePtr PNTR genelist, Int4 max, FeatsIntsPtr fipr)

{
  Int4          compare;
  FeatsIntsPtr  fipg;
  Int4          left;
  Int4          mid;
  Int4          right;
  ValNodePtr    vnpg;

  if (genelist == NULL || fipr == NULL || max < 1) return 0;
  mid = 0;
  left = 1;
  right = max;
  while (left <= right) {
    mid = (left + right) / 2;
    vnpg = genelist [mid - 1];
    fipg = (FeatsIntsPtr) vnpg->data.ptrvalue;
    if (fipg->rightend < fipr->leftend) {
      compare =  1;
    } else if (fipr->rightend < fipg->leftend) {
      compare =  -1;
    } else {
      compare =  0;
    }
    if (compare <= 0) {
      right = mid - 1;
    }
    if (compare >= 0) {
      left = mid + 1;
    }
  }
  if (left > right + 1) {
    if (fipg->strand == fipr->strand) {
      if (SeqLocAinB (fipr->sfp->location, fipg->sfp->location)) {
        return mid;
      }
    }
  }
  return 0;
}

static void DoTrimGenesGenes (Uint2 entityID, SeqEntryPtr sep)

{
  BioseqSetPtr  bssp;
  ValNodePtr    PNTR featlist;
  FeatsIntsPtr  fipf;
  FeatsIntsPtr  fipg;
  FeatsIntsPtr  fipr;
  ValNodePtr    PNTR genelist;
  ValNodePtr    genes;
  GatherScope   gs;
  Int2          i;
  Int4          max;
  ValNodePtr    rbss;
  ValNodePtr    tmp;
  ValNodePtr    vnpf;
  ValNodePtr    vnpg;
  ValNodePtr    vnpr;

  if (sep == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 7 ||
                         (IsPopPhyEtcSet (bssp->_class)))) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        DoTrimGenesGenes (entityID, sep);
      }
      return;
    }
  }

  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet ((Pointer) (gs.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.scope = sep;
  rbss = NULL;
  GatherEntity (entityID, (Pointer) (&rbss), GetNonGeneGatherFunc, &gs);
  rbss = SortValNode (rbss, SortFeatByLocation);
  genes = NULL;
  GatherEntity (entityID, (Pointer) (&genes), GetGeneGatherFunc, &gs);
  genes = SortValNode (genes, SortFeatByLocation);

  for (vnpg = genes, i = 0; vnpg != NULL; vnpg = vnpg->next) {
    i++;
  }
  genelist = (ValNodePtr PNTR) MemNew (sizeof (ValNodePtr) * (i + 1));
  featlist = (ValNodePtr PNTR) MemNew (sizeof (ValNodePtr) * (i + 1));
  if (genelist != NULL && featlist != NULL) {
    for (tmp = genes, i = 0; tmp != NULL; tmp = tmp->next) {
      genelist [i] = tmp;
      i++;
    }
    max = (Int4) i;

    for (vnpr = rbss; vnpr != NULL; vnpr = vnpr->next) {
      fipr = (FeatsIntsPtr) vnpr->data.ptrvalue;
      /*
      for (i = 0; i < (Int2) max; i++) {
        vnpg = genelist [i];
        fipg = (FeatsIntsPtr) vnpg->data.ptrvalue;
        if (SeqLocAinB (fipr->sfp->location, fipg->sfp->location)) {
          ValNodeAddPointer (&(featlist [i]), 0, (Pointer) fipr);
        }
      }
      */
      i = FindBestGeneFeat (genelist, max, fipr);
      if (i > 0) {
        ValNodeAddPointer (&(featlist [i - 1]), 0, (Pointer) fipr);
      }
    }

    for (i = 0; i < (Int2) max; i++) {
      vnpf = featlist [i];
      vnpg = genelist [i];
      if (vnpf != NULL && vnpg != NULL) {
        fipg = (FeatsIntsPtr) vnpg->data.ptrvalue;
        fipf = (FeatsIntsPtr) vnpf->data.ptrvalue;
        fipg->sfp->location = SeqLocFree (fipg->sfp->location);
        fipg->sfp->location = AsnIoMemCopy ((Pointer) (fipf->sfp->location),
                                            (AsnReadFunc) SeqLocAsnRead,
                                            (AsnWriteFunc) SeqLocAsnWrite);
        for (tmp = vnpf->next; tmp != NULL; tmp = tmp->next) {
          fipf = (FeatsIntsPtr) tmp->data.ptrvalue;
          ExtendGeneByRbs (fipf->sfp, fipg->sfp, entityID);
        }
      }
    }

    for (i = 0; i < (Int2) max; i++) {
      ValNodeFree (featlist [i]);
    }
  }

  MemFree (genelist);
  MemFree (featlist);
  ValNodeFreeData (rbss);
  ValNodeFreeData (genes);
}

static Int2 LIBCALLBACK TrimGenes (Pointer data)

{
  OMProcControlPtr  ompcp;
  SeqEntryPtr       sep;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->input_entityID == 0) {
    Message (MSG_ERROR, "Please select a Bioseq or BioseqSet");
    return OM_MSG_RET_ERROR;
  }
  sep = GetTopSeqEntryForEntityID (ompcp->input_entityID);
  if (sep == NULL) {
    Message (MSG_ERROR, "Please select a Bioseq or BioseqSet");
    return OM_MSG_RET_ERROR;
  }
  WatchCursor ();
  Update ();
  DoTrimGenesGenes (ompcp->input_entityID, sep);
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, 0, 0);
  return OM_MSG_RET_DONE;
}

static Int2 LIBCALLBACK SplitIntoSegmentedBioseq (Pointer data)

{
  BioseqPtr         bsp, bsp2;
  BioseqSetPtr      bssp;
  SeqIdPtr          id, id1, id2;
  Int4              left;
  ObjMgrDataPtr     omdptop;
  ObjMgrData        omdata;
  OMProcControlPtr  ompcp;
  Uint2             parenttype;
  Pointer           parentptr;
  BioseqPtr         part1, part2;
  Int4              right;
  SelStructPtr      sel;
  SeqEntryPtr       sep, sep1, sep2;
  SeqIdPtr          sip;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL) return OM_MSG_RET_ERROR;
  sel = ObjMgrGetSelected ();
  if (sel == NULL || sel->next != NULL) return OM_MSG_RET_ERROR;
  if (sel->entityID != ompcp->input_entityID) return OM_MSG_RET_ERROR;
  if (sel->itemtype != OBJ_BIOSEQ || sel->region == NULL) return OM_MSG_RET_ERROR;

  bsp = GetBioseqGivenSeqLoc (sel->region, ompcp->input_entityID);
  if (bsp == NULL) return OM_MSG_RET_ERROR;
  sep = SeqMgrGetSeqEntryForData (bsp);
  SaveSeqEntryObjMgrData (sep, &omdptop, &omdata);
  GetSeqEntryParent (sep, &parentptr, &parenttype);

  left = SeqLocStart (sel->region);
  right = SeqLocStop (sel->region);
  if (right != left + 1) {
    Message (MSG_OK, "Please select two adjacent bases");
    return OM_MSG_RET_ERROR;
  }
  id = SeqIdFindBest (bsp->id, 0);

  id1 = MakeNewProteinSeqId (NULL, id);
  part1 = BioseqCopy (id1, id, 0, right - 1, Seq_strand_plus, FALSE);
  sep1 = SeqEntryNew ();
  if (sep1 == NULL) return OM_MSG_RET_ERROR;
  sep1->choice = 1;
  sep1->data.ptrvalue = part1;
  SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) part1, sep1);

  id2 = MakeNewProteinSeqId (NULL, id);
  part2 = BioseqCopy (id2, id, right, bsp->length - 1, Seq_strand_plus, FALSE);
  sep2 = SeqEntryNew ();
  if (sep2 == NULL) return OM_MSG_RET_ERROR;
  sep2->choice = 1;
  sep2->data.ptrvalue = part2;
  SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) part2, sep2);

  AddSeqEntryToSeqEntry (sep1, sep2, TRUE);
  sep2 = FindNucSeqEntry (sep1);
  if (sep2 == NULL || sep2->choice != 1 || sep2->data.ptrvalue == NULL) return OM_MSG_RET_ERROR;
  bsp2 = (BioseqPtr) sep2->data.ptrvalue;

  sip = bsp->id;
  bsp->id = bsp2->id;
  bsp2->id = sip;

  if (sep1 == NULL || sep1->choice != 2 || sep1->data.ptrvalue == NULL) return OM_MSG_RET_ERROR;
  bssp = (BioseqSetPtr) sep1->data.ptrvalue;

  bssp->descr = bsp->descr;
  bsp->descr = NULL;
  bssp->annot = bsp->annot;
  bsp->annot = NULL;

  sep = SeqMgrGetSeqEntryForData (bsp);
  sep->choice = sep1->choice;
  sep->data.ptrvalue = sep1->data.ptrvalue;

  SeqMgrReplaceInBioseqIndex (bsp2);
  SeqMgrReplaceInBioseqIndex (bsp);
  BioseqFree (bsp);

  SeqMgrLinkSeqEntry (sep, parenttype, parentptr);
  RestoreSeqEntryObjMgrData (sep, omdptop, &omdata);

  ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, 0, 0);
  return OM_MSG_RET_DONE;
}


static void ReindexBioseqs (BioseqPtr bsp, Pointer userdata)

{
  SeqMgrReplaceInBioseqIndex (bsp);
}

static Int2 LIBCALLBACK DoClearSeqEntryScope (Pointer data)

{
  OMProcControlPtr  ompcp;
  SeqEntryPtr       sep;

  SeqEntrySetScope (NULL);
  ompcp = (OMProcControlPtr) data;
  if (ompcp != NULL) {
    sep = GetTopSeqEntryForEntityID (ompcp->input_entityID);
    if (sep != NULL) {
      VisitBioseqsInSep (sep, NULL, ReindexBioseqs);
    }
  }
  SeqEntrySetScope (NULL);
  return OM_MSG_RET_DONE;
}

static Int2 LIBCALLBACK FilterMolWtFunc (Pointer data)

{
  BioseqPtr         bsp = NULL;
  Char              buf [32];
  FloatHi           molwt;
  OMProcControlPtr  ompcp;
  SeqEntryPtr       sep;
  SeqLocPtr         slp;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->input_itemtype == 0) return OM_MSG_RET_ERROR;

  switch (ompcp->input_itemtype)
  {
    case OBJ_BIOSEQ:
      bsp = (BioseqPtr) ompcp->input_data;
      break;
    default:
      return OM_MSG_RET_ERROR;
  }

  if (bsp == NULL) return OM_MSG_RET_ERROR;
  if (! (ISA_aa (bsp->mol))) return OM_MSG_RET_ERROR;

  sep = bsp->seqentry;
  if (sep == NULL) return OM_MSG_RET_ERROR;

  slp = CreateWholeInterval (sep);
  if (slp == NULL) return OM_MSG_RET_ERROR;

  molwt = MolWtForLoc (slp);
  sprintf (buf, "%15.3lf", (double) molwt);
  TrimSpacesAroundString (buf);
  Message (MSG_OK, "Protein molecular weight is %s", buf);

  SeqLocFree (slp);

  return OM_MSG_RET_DONE;
}

#define REGISTER_GROUP_MOLWT ObjMgrProcLoadEx (OMPROC_FILTER, \
        "MOLWT", "MolWt", \
        OBJ_BIOSEQ, 0, OBJ_BIOSEQ, 0, \
        NULL, FilterMolWtFunc, PROC_PRIORITY_DEFAULT, "Analysis")

extern void SetupSequinFilters (void)

{
  Char  str [32];

  if (extraServices) {
    REGISTER_BIOSEQ_SEG_REPORT;
    REGISTER_SEQUIN_PROT_TITLES;
    REGISTER_SEQUIN_NUC_TITLES;
    REGISTER_SEQUIN_FEAT_TABLE;
  }

  if (genomeCenter != NULL || indexerVersion) {
    REGISTER_SEQUIN_DUMP_CONTIG;
  }

  if (indexerVersion) {
    REGISTER_CLEAR_SEQENTRYSCOPE;
    REGISTER_SEQUIN_CACHE_ACCN;
    REGISTER_SEPARATE_MRNA_ALIGNS;
    REGISTER_SEQUIN_ACCN_TO_GI;
    REGISTER_SEQUIN_GI_TO_ACCN;
    REGISTER_REFGENEUSER_DESC_EDIT;
    REGISTER_PROT_IDS_TO_GENE_SYN;
    REGISTER_DESCRIPTOR_PROPAGATE;
  }

  if (indexerVersion
    || (GetAppParam ("SEQUIN", "SETTINGS", "ALLOWAUTOINTRONEXON",
                     NULL, str, sizeof (str))
      && StringICmp (str, "TRUE") == 0))
  {
    REGISTER_MAKEEXONINTRON;
  }

  REGISTER_TPAASSEMBLYUSER_DESC_EDIT;

  REGISTER_BIOSEQ_COMPLEMENT;
  REGISTER_BIOSEQ_REVERSE;
  REGISTER_BIOSEQ_REVCOMP_WITHFEAT;
  REGISTER_BIOSEQ_REVCOMP_NOTFEAT;

  REGISTER_ALUFEAT;
  REGISTER_BIOSEQ_ORF;
  REGISTER_MRNA_FROM_CDS;
  REGISTER_MAP_TO_PROT;
  REGISTER_MAP_TO_NUC;

  REGISTER_UPDATE_SEQALIGN;
  REGISTER_PARTSEQALIGNTOPARENT;
  REGISTER_CONVERTSEQALIGN;
  REGISTER_MAKESEQALIGN;
  REGISTER_MAKESEQALIGNP;
  REGISTER_NORMSEQALIGN;
  REGISTER_NOMORESEGGAP;
  REGISTER_OPENALED;
  REGISTER_OPENALED2;
  REGISTER_OPENALED3;
  REGISTER_OPENALED4;

  if (extraServices) {
    REGISTER_TRIM_GENES;
    REGISTER_FIXUP_RBS;
    REGISTER_INTERVAL_COMBINE;
    REGISTER_REPACKAGE_PARTS;
    REGISTER_UNDOSEGSET;
    REGISTER_ADJUSTMULTISEGSEQ;
    REGISTER_UPDATESEGSET;
  }

  if (indexerVersion) {
    REGISTER_DELETE_BY_TEXT;
    REGISTER_SEGREGATE_BY_TEXT;
    REGISTER_FIND_NON_ACGT;
    REGISTER_BSP_INDEX;
    REGISTER_POPSET_WITHIN_GENBANK;
    REGISTER_NORMALIZE_NUCPROT;
    REGISTER_REMOVE_EXTRANEOUS;
    REGISTER_REMOVE_MESSEDUP;
    REGISTER_GROUP_EXPLODE;
    REGISTER_SPLIT_BIOSEQ;
    REGISTER_DESKTOP_REPORT;
  }

  REGISTER_GROUP_FILTERGC;
  REGISTER_GROUP_DUST;
  REGISTER_GROUP_PCC;
  REGISTER_GROUP_HOPP;
  REGISTER_GROUP_KYTE;
  REGISTER_GROUP_MATRIX;
  REGISTER_GROUP_MOLWT;
}

extern CharPtr MergeValNodeStrings (ValNodePtr list, Boolean useReturn)

{
  size_t      len;
  CharPtr     ptr;
  CharPtr     str;
  CharPtr     tmp;
  ValNodePtr  vnp;


  ptr = NULL;
  if (list != NULL) {
    vnp = list;
    len = 0;
    while (vnp != NULL) {
      if (vnp->data.ptrvalue != NULL) {
        len += StringLen ((CharPtr) vnp->data.ptrvalue) + 1;
      }
      vnp = vnp->next;
    }
    if (len > 0) {
      ptr = MemNew (sizeof (Char) * (len + 2));
      if (ptr != NULL) {
        vnp = list;
        tmp = NULL;
        while (vnp != NULL) {
          str = (CharPtr) vnp->data.ptrvalue;
          if (str != NULL) {
            if (tmp == NULL) {
              tmp = ptr;
            } else if (useReturn) {
              tmp = StringMove (tmp, "\n");
            } else if (IsJapanese () && (tmp - ptr > 2) &&
            		IsMBLetter (tmp - 2) && IsMBLetter (str)) {
              /* no space required between two Japanese letters. */
              tmp = tmp;
            } else if (str [0] != ',' && str [0] != ';' && str [0] != ':') {
              tmp = StringMove (tmp, " ");
            } else {
              tmp = StringMove (tmp, " ");
            }
            tmp = StringMove (tmp, str);
          }
          vnp = vnp->next;
        }
      }
    }
  }
  return ptr;
}

/* general text parsing and conversion */

static ENUM_ALIST(from_fld_alist)
  {" ",                    0},
  {"Gene-CDS-Prot-RNA",    1},
  {"Source-Modifiers",     2},
  {"Feature Qualifiers",   3},
  {"COMMENT",              4},
  {"GenBank",              5},
  {"EMBL",                 6},
  {"Def Line",             7},
  {"Local ID",             8},
END_ENUM_ALIST

static ENUM_ALIST(to_fld_alist)
  {" ",                    0},
  {"Gene-CDS-Prot-RNA",    1},
  {"Source-Modifiers",     2},
  {"Feature Qualifiers",   3},
  {"COMMENT",              4},
END_ENUM_ALIST

static ENUM_ALIST(gene_cds_prot_rna_fld_alist)
  {" ",                    0},
  {"CDS comment",          1},
  {"CDS gene xref",        2},
  {"CDS protein xref",     3},
  {"Gene locus",           4},
  {"Gene description",     5},
  {"Gene allele",          6},
  {"Gene maploc",          7},
  {"Gene synonym",         8},
  {"Gene comment",         9},
  {"Protein name",        10},
  {"Protein description", 11},
  {"Protein E.C. number", 12},
  {"Protein activity",    13},
  {"Protein comment",     14},
  {"RNA name",            15},
  {"RNA comment",         16},
  {"RNA gene xref",       17},
END_ENUM_ALIST

static ENUM_ALIST(source_modifiers_fld_alist)
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
  {"Common Name",           202},
  {"Country",               123},
  {"Cultivar",               10},
  {"Dev-stage",             112},
  {"Division",              204},
  {"Dosage",                 20},
  {"Ecotype",                27},
  {"Endogenous-virus-name", 125},
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
  {"Lineage",               203},
  {"Map",                   102},
  {"Old Lineage",            53},
  {"Old Name",               54},
  {"OrgMod Note",            55},
  {"Pathovar",               11},
  {"Plasmid-name",          119},
  {"Plastid-name",          122},
  {"Pop-variant",           117},
  {"Rearranged",            115},
  {"Scientific Name",       201},
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
  {"SubSource Note",        155},
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

static ENUM_ALIST(feature_qualifiers_fld_alist)
  {" ",               0},
  {"bound_moiety",    1},
  {"clone",           2},
  {"cons_splice",     3},
  {"direction",       4},
  {"frequency",       5},
  {"function",        6},
  {"label",           7},
  {"map",             8},
  {"mod_base",        9},
  {"note",           10},
  {"number",         11},
  {"organism",       12},
  {"PCR_conditions", 13},
  {"phenotype",      14},
  {"product",        15},
  {"replace",        16},
  {"rpt_family",     17},
  {"rpt_type",       18},
  {"rpt_unit",       19},
  {"standard_name",  20},
  {"usedin",         21},
END_ENUM_ALIST

/*
static Uint1 SourceModListToOrgModType (UIEnum val) 

{
  if (val > 0 && val < 32) return (Uint1) val;
  if (val == 55) return 255;
  if (val == 54) return 254;
  return 0;
}

static Uint1 SourceModListToSubSourceType (UIEnum val) 

{
  if (val > 100 && val < 125) return (Uint1) val - 100;
  if (val == 155) return 255;
  return 0;
}

static Uint1 SourceModListToBioSourceField (UIEnum val) 

{
  if (val > 200 && val < 205) return (Uint1) val - 200;
  return 0;
}
*/

#define NUM_SUBTARGET_POPUPS 10

typedef struct targetdata {
  PopuP              target;
  EnumFieldAssocPtr  alist;
  PopuP              subtarget [NUM_SUBTARGET_POPUPS];
  EnumFieldAssocPtr  alists [NUM_SUBTARGET_POPUPS];
  Int2               type;
  Int2               subtype;
} TargetData, PNTR TargetDataPtr;

static void ChangeTarget (PopuP p)

{
  Int2           i;
  TargetDataPtr  tdp;
  UIEnum         val;

  tdp = (TargetDataPtr) GetObjectExtra (p);
  if (tdp == NULL) return;
  if (GetEnumPopup (tdp->target, tdp->alist, &val) && val > 0) {
    for (i = 0; i < NUM_SUBTARGET_POPUPS; i++) {
      if (i != (Int2) val) {
        SafeHide (tdp->subtarget [i]);
      }
    }
    SafeShow (tdp->subtarget [(Int2) val]);
  }
}

static void CreateTransformTargetControl (GrouP h, TargetDataPtr tdp, EnumFieldAssocPtr alist)

{
  Int2   j;
  GrouP  p;
  GrouP  q;

  if (h == NULL || tdp == NULL || alist == NULL) return;

  p = HiddenGroup (h, 2, 0, NULL);

  tdp->target = PopupList (p, TRUE, ChangeTarget);
  SetObjectExtra (tdp->target, tdp, NULL);
  InitEnumPopup (tdp->target, alist, NULL);
  SetEnumPopup (tdp->target, alist, 0);

  tdp->alists [1] = gene_cds_prot_rna_fld_alist;
  tdp->alists [2] = source_modifiers_fld_alist;
  tdp->alists [3] = feature_qualifiers_fld_alist;

  q = HiddenGroup (p, 0, 0, NULL);
  for (j = 1; j < 4; j++) {
    tdp->subtarget [j] = PopupList (q, TRUE, NULL);
    SetObjectExtra (tdp->subtarget [j], tdp, NULL);
    InitEnumPopup (tdp->subtarget [j], tdp->alists [j], NULL);
    SetEnumPopup (tdp->subtarget [j], tdp->alists [j], 0);
    Hide (tdp->subtarget [j]);
  }
}

typedef struct stringdata {
  TexT     atleft;
  TexT     atright;
  GrouP    leftbehav;
  GrouP    rightbehav;
  CharPtr  leftstr;
  CharPtr  rightstr;
  Boolean  includeleft;
  Boolean  includeright;
  TexT     replaceby;
  CharPtr  replacestr;
} StringData, PNTR StringDataPtr;

static void CreateTransformStringControl (GrouP h, StringDataPtr sdp)

{
  GrouP  g;

  if (h == NULL || sdp == NULL) return;

  g = HiddenGroup (h, 3, 0, NULL);

  StaticPrompt (g, "Select text", 0, dialogTextHeight, programFont, 'l');
  sdp->leftbehav = HiddenGroup (g, 2, 0, NULL);
  RadioButton (sdp->leftbehav, "just after");
  RadioButton (sdp->leftbehav, "starting at");
  SetValue (sdp->leftbehav, 1);
  sdp->atleft = DialogText (g, "", 10, NULL);
  SetObjectExtra (sdp->atleft, sdp, NULL);

  StaticPrompt (g, "and", 0, dialogTextHeight, programFont, 'l');
  sdp->rightbehav = HiddenGroup (g, 2, 0, NULL);
  RadioButton (sdp->rightbehav, "just before");
  RadioButton (sdp->rightbehav, "ending with");
  SetValue (sdp->rightbehav, 1);
  sdp->atright = DialogText (g, "", 10, NULL);
  SetObjectExtra (sdp->atright, sdp, NULL);
}

typedef struct transformdata {
  FEATURE_FORM_BLOCK

  TargetData         fromtarget;
  TargetData         totarget;
  StringData         strings;
  ButtoN             accept;
  CharPtr            foundstr;
  Boolean            replaceOldAsked;
  Boolean            doReplaceAll;
  Int2               index;
} TransFormData, PNTR TransFormPtr;

static CharPtr SaveStringFromTextNoStripSpaces (TexT t)

{
  size_t   len;
  CharPtr  str;

  len = TextLength (t);
  if (len > 0) {
    str = (CharPtr) MemNew(len + 1);
    if (str != NULL) {
      GetTitle (t, str, len + 1);
      return str;
    } else {
      return NULL;
    }
  } else {
    return NULL;
  }
}

static void DoOneTransformText (Uint2 entityID, SeqEntryPtr sep, TransFormPtr tfp, MonitorPtr mon)

{
  /*
  BioSourcePtr  biop;
  BioseqSetPtr  bssp;
  Char          str [64];

  if (sep == NULL || tfp == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 7 || bssp->_class == 13 ||
                         bssp->_class == 14 || bssp->_class == 15)) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        DoOneTransformText (entityID, sep, tfp, mon);
      }
      return;
    }
  }
  (tfp->index)++;
  if (mon != NULL) {
    sprintf (str, "Processing component %d", (int) tfp->index);
    MonitorStrValue (mon, str);
  }
  switch (tfp->fromtarget.type) {
    case 1 :
    case 2 :
    case 3 :
    case 4 :
      SeqEntryExplore (sep, (Pointer) tfp, RemoveAFeatureText);
      break;
    case 5 :
      SeqEntryToBioSource (sep, NULL, NULL, 0, &biop);
      RemoveASourceText (biop, tfp);
      break;
    case 6 :
    case 7 :
      SeqEntryToBioSource (sep, NULL, NULL, 0, &biop);
      RemoveASourceText (biop, tfp);
      break;
    default :
      break;
  }
  */
}

static void DoTransformProc (ButtoN b)

{
  MonitorPtr    mon;
  SeqEntryPtr   sep;
  TransFormPtr  tfp;
  UIEnum        val;

  tfp = (TransFormPtr) GetObjectExtra (b);
  if (tfp == NULL || tfp->input_entityID == 0) return;
  sep = GetTopSeqEntryForEntityID (tfp->input_entityID);
  if (sep == NULL) return;
  Hide (tfp->form);
  WatchCursor ();
  Update ();
  if (GetEnumPopup (tfp->fromtarget.target, tfp->fromtarget.alist, &val) && val > 0) {
    tfp->fromtarget.type = (Int2) val;
    if (GetEnumPopup (tfp->fromtarget.subtarget [tfp->fromtarget.type],
                      tfp->fromtarget.alists [tfp->fromtarget.type], &val) && val > 0) {
      tfp->fromtarget.subtype = (Int2) val;
      tfp->strings.leftstr = SaveStringFromTextNoStripSpaces (tfp->strings.atleft);
      tfp->strings.rightstr = SaveStringFromTextNoStripSpaces (tfp->strings.atright);
      tfp->strings.includeleft = (Boolean) (GetValue (tfp->strings.leftbehav) == 2);
      tfp->strings.includeright = (Boolean) (GetValue (tfp->strings.rightbehav) == 2);
      mon = MonitorStrNewEx ("Removing Text From String", 20, FALSE);
      tfp->index = 0;
      DoOneTransformText (tfp->input_entityID, sep, tfp, mon);
      MonitorFree (mon);
      tfp->strings.leftstr = MemFree (tfp->strings.leftstr);
      tfp->strings.rightstr = MemFree (tfp->strings.rightstr);
    }
  }
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (tfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, tfp->input_entityID, 0, 0);
  Remove (tfp->form);
}

/* resolve existing colliding ids section */

typedef struct lclidlist {
  BioseqPtr  firstbsp;
  SeqIdPtr   firstsip;
  CharPtr    key;
  Int2       count;
  struct lclidlist PNTR left;
  struct lclidlist PNTR right;
} LclIdList, PNTR LclIdListPtr;

/********************************************************************
*
* SeqLocReplaceLocalID
*   replaces the Seq-Id in a Seq-Loc (slp) with a new Seq-Id (new_sip)
*   only if the Seq-Id is a local one.
*
**********************************************************************/

extern SeqLocPtr SeqLocReplaceLocalID (SeqLocPtr slp,
				       SeqIdPtr  new_sip)
{
  SeqLocPtr        curr;
  PackSeqPntPtr    pspp;
  SeqIntPtr        target_sit;
  SeqPntPtr        spp;
  SeqIdPtr         currId;

  switch (slp->choice) {
     case SEQLOC_PACKED_INT :
     case SEQLOC_MIX :
     case SEQLOC_EQUIV :
        curr = NULL;
        while ((curr = SeqLocFindNext (slp, curr)) != NULL) {
           curr = SeqLocReplaceLocalID (curr, new_sip);
        }
        break;
     case SEQLOC_PACKED_PNT :
        pspp = (PackSeqPntPtr) slp->data.ptrvalue;
        if ((pspp != NULL) && (pspp->id->choice == SEQID_LOCAL)) {
          SeqIdFree (pspp->id);
          pspp->id = SeqIdDup (new_sip);
        }
        break;
     case SEQLOC_EMPTY :
     case SEQLOC_WHOLE :
        currId = (SeqIdPtr) slp->data.ptrvalue;
	if (currId->choice == SEQID_LOCAL)
	  {
	    SeqIdFree (currId);
	    slp->data.ptrvalue = (Pointer) SeqIdDup (new_sip);
	  }
        break;
     case SEQLOC_INT :
        target_sit = (SeqIntPtr) slp->data.ptrvalue;
	if (target_sit->id->choice == SEQID_LOCAL)
	  {
	    SeqIdFree (target_sit->id);
	    target_sit->id = SeqIdDup (new_sip);
	  }
        break;
     case SEQLOC_PNT :
        spp = (SeqPntPtr)slp->data.ptrvalue;
	if (spp->id->choice == SEQID_LOCAL)
	  {
	    SeqIdFree(spp->id);
	    spp->id = SeqIdDup(new_sip);
	  }
        break;
     default :
        break;
  }
  return slp;
}

static void ReplaceLocalIdOnLoc_callback (SeqFeatPtr sfp, Pointer userdata)
{
  SeqIdPtr sip;

  sip = (SeqIdPtr) userdata;
  if (sfp->location != NULL) 
    SeqLocReplaceLocalID (sfp->location, sip);
}

static void CheckFeatForNuclID_callback (SeqFeatPtr sfp, Pointer userdata)
{
  SeqIdPtr            featSip = NULL;
  ReplaceIDStructPtr  idsPtr;
  ObjectIdPtr         oip;
  Char                tmpIdStr [128];

  if (NULL == sfp)
    return;

  if ((sfp->data.choice == SEQFEAT_CDREGION) &&
      (sfp->location != NULL)) {

    /* Get the old Seq Id and the new */
    /* one that it was changed to.    */
    
    idsPtr = (ReplaceIDStructPtr) userdata;
    if ((NULL == idsPtr)         ||
	(NULL == idsPtr->oldStr) ||
	(NULL == idsPtr->newSip))
      return;

    /* Get the location Seq ID for this CDS feature */
    
    featSip = SeqLocId (sfp->location);
    if (featSip == NULL) return;
    oip     = (ObjectIdPtr) featSip->data.ptrvalue;
    
    /* If the location Seq ID matches the old Seq Id */
    /* then change the location to point to the new. */
    
    if (NULL == oip->str) {
      sprintf (tmpIdStr, "%d", oip->id);
      if (StringCmp (tmpIdStr, idsPtr->oldStr) == 0)
	SeqLocReplaceLocalID (sfp->location, idsPtr->newSip);
    }
    else if (StringCmp (oip->str, idsPtr->oldStr) == 0)
      SeqLocReplaceLocalID (sfp->location, idsPtr->newSip);
  }
}

static void CheckFeatForProductID_callback (SeqFeatPtr sfp, Pointer userdata)
{
  SeqIdPtr            featSip = NULL;
  ReplaceIDStructPtr  idsPtr;
  ObjectIdPtr         oip;
  Char                tmpIdStr [128];

  if (NULL == sfp)
    return;

  if ((sfp->data.choice == SEQFEAT_CDREGION) &&
      (sfp->product != NULL)) {

    /* Get the old Seq Id and the new */
    /* one that it was changed to.    */
    
    idsPtr = (ReplaceIDStructPtr) userdata;
    if ((NULL == idsPtr)         ||
	(NULL == idsPtr->oldStr) ||
	(NULL == idsPtr->newSip))
      return;

    /* Get the product Seq ID for this CDS feature */
    
    featSip = SeqLocId (sfp->product);
    oip     = (ObjectIdPtr) featSip->data.ptrvalue;
    
    /* If the product Seq ID matches the old Seq Id */
    /* then change the product to point to the new. */
    
    if (NULL == oip->str) {
      sprintf (tmpIdStr, "%d", oip->id);
      if (StringCmp (tmpIdStr, idsPtr->oldStr) == 0)
	SeqLocReplaceLocalID (sfp->product, idsPtr->newSip);
    }
    if (StringCmp (oip->str, idsPtr->oldStr) == 0)
      SeqLocReplaceLocalID (sfp->product, idsPtr->newSip);
    
  }
}

static void ReplaceLocalID (BioseqPtr bsp,
			    SeqIdPtr sip,
			    CharPtr key,
			    Int2 count)

{
  ObjectIdPtr      oip;
  Char             str [64];
  Char             tmp [70];
  BioseqSetPtr     bssp;
  ReplaceIDStruct  ids;
  BioseqPtr        siblingBsp;
  SeqEntryPtr      sep;
  Int2             parentType;

  if (bsp == NULL || sip == NULL || StringHasNoText (key)) return;
  oip = (ObjectIdPtr) sip->data.ptrvalue;
  if (oip == NULL) return;

  /* Create the new ID string */

  StringNCpy_0 (str, key, sizeof (str));
  sprintf (tmp, "%s__%d", str, (int) count);

  /* Save the original SeqId for later passing */
  /* to CheckSetForNuclID_callback () and      */
  /* CheckSetForProductId_callback ().         */

  if (NULL != oip->str)
    ids.oldStr = StringSave (oip->str);
  else {
    ids.oldStr = (CharPtr) MemNew (32);
    sprintf (ids.oldStr, "%d", oip->id);
  }
    

  /* Update the Seq ID with the new string */

  oip->str = StringSave (tmp);
  ids.newSip = sip;
  SeqMgrReplaceInBioseqIndex (bsp);

  /* Replace the local ID on all the features of the bioseq */

  VisitFeaturesOnBsp (bsp, (Pointer) sip, ReplaceLocalIdOnLoc_callback);

  /* Check the parent (and grandparent, etc.) BioseqSet */
  /* for features that use the changed ID.              */

  parentType = bsp->idx.parenttype;
  if (parentType == OBJ_BIOSEQSET) 
    bssp = (BioseqSetPtr) bsp->idx.parentptr;

  while (parentType == OBJ_BIOSEQSET) {

    if ((bssp != NULL) && (bssp->_class == 1)) {
      
      /* Check features that are attached to */
      /* the parent set itself.              */
      
      if (ISA_na(bsp->mol))
	VisitFeaturesOnSet (bssp, (Pointer) &ids,
			    CheckFeatForNuclID_callback);
      else if (ISA_aa(bsp->mol))
	VisitFeaturesOnSet (bssp, (Pointer) &ids,
			    CheckFeatForProductID_callback);
      
      /* Check features that are attached to */
      /* other Bioseqs in the set.           */
      
      sep = bssp->seqentry;
      while (NULL != sep) {
	if (sep->choice == 1) { /* bioseq */
	  siblingBsp = (BioseqPtr) sep->data.ptrvalue;
	  if (ISA_na(bsp->mol))
	    VisitFeaturesOnBsp (siblingBsp, (Pointer) sip,
				CheckFeatForNuclID_callback);
	  else if (ISA_aa(bsp->mol))
	    VisitFeaturesOnBsp (siblingBsp, (Pointer) sip,
				CheckFeatForProductID_callback);
	}
	sep = sep->next;
      }
      
      sep = bssp->seq_set;
      while (NULL != sep) {
	if (sep->choice == 1) { /* bioseq */
	  siblingBsp = (BioseqPtr) sep->data.ptrvalue;
	  if (ISA_na(bsp->mol))
	    VisitFeaturesOnBsp (siblingBsp, (Pointer) sip,
				CheckFeatForNuclID_callback);
	  else if (ISA_aa(bsp->mol))
	    VisitFeaturesOnBsp (siblingBsp, (Pointer) sip,
				CheckFeatForProductID_callback);
	}
	sep = sep->next;
      }
    }
    parentType = bssp->idx.parenttype;
    bssp = (BioseqSetPtr) bssp->idx.parentptr;
  }

  /* Clean up before exiting */

  MemFree (ids.oldStr);

}

static void BuildLclTree (LclIdListPtr PNTR head, BioseqPtr bsp, CharPtr x, SeqIdPtr sip)

{
  Int2          comp;
  LclIdListPtr  idlist;

  if (*head != NULL) {
    idlist = *head;
    comp = StringICmp (idlist->key, x);
    if (comp < 0) {
      BuildLclTree (&(idlist->right), bsp, x, sip);
    } else if (comp > 0) {
      BuildLclTree (&(idlist->left), bsp, x, sip);
    } else {
      if (idlist->firstbsp != NULL && idlist->firstsip != NULL) {
        ReplaceLocalID (idlist->firstbsp, idlist->firstsip, x, 1);
        idlist->count = 2;
        idlist->firstbsp = NULL;
        idlist->firstsip = NULL;
      }
      ReplaceLocalID (bsp, sip, x, idlist->count);
      (idlist->count)++;
    }
  } else {
    idlist = MemNew (sizeof (LclIdList));
    if (idlist != NULL) {
      *head = idlist;
      idlist->firstbsp = bsp;
      idlist->firstsip = sip;
      idlist->count = 1;
      idlist->key = StringSave (x);
      idlist->left = NULL;
      idlist->right = NULL;
    }
  }
}

static void FreeLclTree (LclIdListPtr PNTR head)

{
  LclIdListPtr  idlist;

  if (head != NULL && *head != NULL) {
    idlist = *head;
    FreeLclTree (&(idlist->left));
    FreeLclTree (&(idlist->right));
    MemFree (idlist->key);
    MemFree (idlist);
  }
}

static void ResolveExistingIDsCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr          bsp;
  LclIdListPtr PNTR  head;
  SeqIdPtr           sip;
  Char               str [64];

  head = (LclIdListPtr PNTR) mydata;
  if (sep == NULL || head == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp != NULL) {
      for (sip = bsp->id; sip != NULL; sip = sip->next) {
        if (sip->choice == SEQID_LOCAL) {
          SeqIdWrite (sip, str, PRINTID_REPORT, sizeof (str));
          BuildLclTree (head, bsp, str, sip);
        }
      }
    }
  }
}

extern Int2 DoOneSegFixup (SeqEntryPtr sep, Boolean ask);

static void ResolveExistingLocalIDsBaseForm (BaseFormPtr bfp)

{
  MsgAnswer     ans;
  Boolean       doParts = FALSE;
  LclIdListPtr  head = NULL;
  SeqEntryPtr   sep;

  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  ans = Message (MSG_YN, "Do you wish to also reconstruct segmented bioseqs?");
  doParts = (Boolean) (ans == ANS_YES);
  SeqEntryExplore (sep, (Pointer) &head, ResolveExistingIDsCallback);
  FreeLclTree (&head);
  if (doParts) {
    DoOneSegFixup (sep, FALSE);
  }
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

extern void ResolveExistingLocalIDs (IteM i);
extern void ResolveExistingLocalIDs (IteM i)
{
  BaseFormPtr   bfp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif

  if (bfp == NULL) return;
  ResolveExistingLocalIDsBaseForm (bfp);
}

extern void ResolveExistingLocalIDsToolBtn (ButtoN b)
{
  BaseFormPtr   bfp;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;
  ResolveExistingLocalIDsBaseForm (bfp);
}

extern void SetSourceFocus (IteM i);
extern void ClearSourceFocus (IteM i);

static Boolean LIBCALLBACK SetDescriptorFocus (BioseqPtr bsp,
					SeqMgrBioseqContextPtr bContext)
{
  SeqMgrFeatContext fContext;
  SeqMgrDescContext dContext;
  BioSourcePtr      biop;
  Boolean           is_focus;
  SeqDescrPtr       sdp;

  /* Only set the focus when the Bioseq has */
  /* a source feature in addition to the    */
  /* source descriptor.                     */

  is_focus = (Boolean) * ((BoolPtr) bContext->userdata);

  /* don't need feature to clear existing focus on descriptor */

  if (is_focus && NULL == SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_BIOSRC, 0,
				    &fContext))
    return TRUE;

  /* Set the focus on all of the Bioseq's */
  /* source descriptors.                  */

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_source, &dContext);
  while (NULL != sdp) {
    biop = (BioSourcePtr) sdp->data.ptrvalue;
    if (biop == NULL)
      return TRUE;
    biop->is_focus = is_focus;
    sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_source, &dContext);
  }

  /* Return true to continue on to next Bioseq */
    
  return TRUE;
}

static void CommonSourceFocus (IteM i, Boolean setfocus)

{
  BaseFormPtr   bfp;
  Boolean       flag;
  SeqEntryPtr   sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  flag = setfocus;
  SeqMgrExploreBioseqs (0, sep->data.ptrvalue, (Pointer) &flag,
			SetDescriptorFocus, TRUE, TRUE, TRUE);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

extern void SetSourceFocus (IteM i)

{
  CommonSourceFocus (i, TRUE);
}

extern void ClearSourceFocus (IteM i)

{
  CommonSourceFocus (i, FALSE);
}

typedef struct acchist {
  DESCRIPTOR_FORM_BLOCK
  SeqEntryPtr     sep;
  TexT            which;
  Int2            pos;
} AccHist, PNTR AccHistPtr;

static void ExAcToHisProc (BioseqPtr bsp, Pointer userdata)

{
  CharPtr       accn;
  AccHistPtr    ahp;
  EMBLBlockPtr  ebp;
  GBBlockPtr    gbp;
  SeqHistPtr    hist;
  Int2          i;
  SeqDescrPtr   sdp;
  SeqIdPtr      sip;
  TextSeqIdPtr  tsip;
  ValNodePtr    vnp = NULL;
  Uint4         whichdb;

  ahp = (AccHistPtr) userdata;
  sdp = BioseqGetSeqDescr (bsp, Seq_descr_genbank, NULL);
  if (sdp != NULL) {
    gbp = (GBBlockPtr) sdp->data.ptrvalue;
    if (gbp == NULL) return;
    for (vnp = gbp->extra_accessions, i = 1;
         vnp != NULL && i < ahp->pos;
         vnp = vnp->next, i++) continue;
  } else {
    sdp = BioseqGetSeqDescr (bsp, Seq_descr_embl, NULL);
    if (sdp != NULL) {
      ebp = (EMBLBlockPtr) sdp->data.ptrvalue;
      if (ebp == NULL) return;
      for (vnp = ebp->extra_acc, i = 1;
           vnp != NULL && i < ahp->pos;
           vnp = vnp->next, i++) continue;
    }
  }
  if (vnp == NULL) return;
  accn = (CharPtr) vnp->data.ptrvalue;
  if (StringHasNoText (accn)) return;
  hist = bsp->hist;
  if (hist != NULL) {
    for (sip = hist->replace_ids; sip != NULL; sip = sip->next) {
      switch (sip->choice) {
        case SEQID_GENBANK :
        case SEQID_EMBL :
        case SEQID_DDBJ :
          tsip = (TextSeqIdPtr) sip->data.ptrvalue;
          if (tsip != NULL) {
            if (StringICmp (tsip->accession, accn) == 0) return;
          }
          break;
        default :
          break;
      }
    }
  }
  if (hist == NULL) {
    hist = SeqHistNew ();
    bsp->hist = hist;
  }
  if (hist == NULL) return;
  sip = ValNodeNew (hist->replace_ids);
  if (hist->replace_ids == NULL) {
     hist->replace_ids = sip;
  }
  if (sip == NULL) return;
  tsip = TextSeqIdNew ();
  if (tsip == NULL) return;
  tsip->accession = StringSave (accn);
  whichdb = WHICH_db_accession (accn);
  if (ACCN_IS_EMBL (whichdb)) {
    sip->choice = SEQID_EMBL;
  } else if (ACCN_IS_DDBJ (whichdb)) {
    sip->choice = SEQID_DDBJ;
  } else {
    sip->choice = SEQID_GENBANK;
  }
  sip->data.ptrvalue = (Pointer) tsip;
}

static void DoProcessExtraAccToHis (ButtoN b)

{
  AccHistPtr  ahp;
  Char        str [16];
  int         val;

  ahp = (AccHistPtr) GetObjectExtra (b);
  if (ahp == NULL) return;
  Hide (ahp->form);
  WatchCursor ();
  Update ();
  GetTitle (ahp->which, str, sizeof (str));
  if (sscanf (str, "%d", &val) == 1 && val > 0) {
    ahp->pos = (Int2) val;
    VisitBioseqsInSep (ahp->sep, (Pointer) ahp, ExAcToHisProc);
  }
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (ahp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, ahp->input_entityID, 0, 0);
  Remove (ahp->form);
}

extern void ExtraAccToHistByPos (IteM i);
extern void ExtraAccToHistByPos (IteM i)

{
  AccHistPtr    ahp;
  ButtoN        b;
  BaseFormPtr   bfp;
  GrouP         c, g, h;
  SeqEntryPtr   sep;
  WindoW        w;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  ahp = (AccHistPtr) MemNew (sizeof (AccHist));
  if (ahp == NULL) return;
  w = FixedWindow (-50, -33, -10, -10, "Secondary to History", StdCloseWindowProc);
  SetObjectExtra (w, ahp, StdCleanupFormProc);
  ahp->form = (ForM) w;
  ahp->formmessage = NULL;

  ahp->sep = sep;
  ahp->input_entityID = bfp->input_entityID;

  h = HiddenGroup (w, -1, 0, NULL);

  g = HiddenGroup (h, 2, 0, NULL);
  StaticPrompt (g, "Secondary Accession Position", 0, dialogTextHeight, programFont, 'l');
  ahp->which = DialogText (g, "", 5, NULL);

  c = HiddenGroup (h, 2, 0, NULL);
  b = DefaultButton (c, "Accept", DoProcessExtraAccToHis);
  SetObjectExtra (b, ahp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);
  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Update ();
}

static void ConvertLocationStrand (SeqLocPtr     slp,
				   EditStrandPtr esp)
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
        if (sinp != NULL) {
          if (sinp->strand == esp->fromStrand) {
            sinp->strand = esp->toStrand;
          }
        }
        break;
      case SEQLOC_PNT :
        spp = (SeqPntPtr) slp->data.ptrvalue;
        if (spp != NULL) {
          if (spp->strand == esp->fromStrand) {
            spp->strand = esp->toStrand;
          }
        }
        break;
      case SEQLOC_PACKED_PNT :
        psp = (PackSeqPntPtr) slp->data.ptrvalue;
        if (psp != NULL) {
          if (psp->strand == esp->fromStrand) {
            psp->strand = esp->toStrand;
          }
        }
        break;
      case SEQLOC_PACKED_INT :
      case SEQLOC_MIX :
      case SEQLOC_EQUIV :
        loc = (SeqLocPtr) slp->data.ptrvalue;
        while (loc != NULL) {
          ConvertLocationStrand (loc, esp);
          loc = loc->next;
        }
        break;
      case SEQLOC_BOND :
        sbp = (SeqBondPtr) slp->data.ptrvalue;
        if (sbp != NULL) {
          spp = (SeqPntPtr) sbp->a;
          if (spp != NULL) {
            if (spp->strand == esp->fromStrand) {
              spp->strand = esp->toStrand;
            }
          }
          spp = (SeqPntPtr) sbp->b;
          if (spp != NULL) {
            if (spp->strand == esp->fromStrand) {
              spp->strand = esp->toStrand;
            }
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

static Boolean LIBCALLBACK DoEditStrand_FeatureCallback (SeqFeatPtr sfp,
				SeqMgrFeatContextPtr fcontext)
{
  EditStrandPtr  esp;
  SeqLocPtr      slp;

  slp = sfp->location;
  esp = (EditStrandPtr) fcontext->userdata;

  if (MeetsStringConstraint (sfp, esp->findThisStr)) {
    ConvertLocationStrand (slp, esp);
  }

  return TRUE;
}

static Boolean LIBCALLBACK DoEditStrand_BioseqCallback (BioseqPtr bsp,
					 SeqMgrBioseqContextPtr bcontext)
{
  Boolean        featureFilterArray [SEQFEAT_MAX];
  EditStrandPtr  esp;

  /* Get the attached data */

  esp = (EditStrandPtr) bcontext->userdata;

  /* Filter for the requested feature type, then */
  /* explore the Bioseq's features, converting   */
  /* the strands as requested.                   */

  if (esp->featSubType == 0)
    SeqMgrExploreFeatures (bsp, (Pointer) esp, DoEditStrand_FeatureCallback,
			   NULL, NULL, NULL);
  else {
    MemSet ((Pointer) (featureFilterArray), (int) FALSE, SEQFEAT_MAX);
    featureFilterArray[esp->featSubType] = TRUE;
    SeqMgrExploreFeatures (bsp, (Pointer) esp, DoEditStrand_FeatureCallback,
			   NULL, NULL, featureFilterArray);
  }

  /* Return TRUE to continue on to the next Bioseq */

  return TRUE;
}

static void DoEditFeatureStrand (ButtoN b)

{
  EditStrandPtr   esp;
  SeqEntryPtr     sep;
  Int2            val;
  ValNodePtr      vnp;

  /* Get the associated data */

  esp = (EditStrandPtr) GetObjectExtra (b);
  if (esp == NULL)
    return;

  /* Get the top-level seqentry */

  sep = GetTopSeqEntryForEntityID (esp->input_entityID);
  if (sep == NULL)
    return;

  /* Hide the screen and show 'working' cursor */

  Hide (esp->form);
  WatchCursor ();
  Update ();

  /* Get the selected values from the lists */

  esp->fromStrand = GetValue (esp->fromPopup);
  esp->fromStrand--;
  esp->toStrand = GetValue (esp->toPopup);
  esp->toStrand--;

  val = GetValue (esp->feature);
  vnp = NULL;
  if (val > 0) {
    vnp = esp->featlist;
    while (vnp != NULL && val > 1) {
      val--;
      vnp = vnp->next;
    }
  }

  if (vnp != NULL)
    esp->featSubType = vnp->choice;
  else
    esp->featSubType = 0;

  esp->findThisStr = JustSaveStringFromText (esp->findThis);
  
  /* Explore each Bioseq */

  SeqMgrExploreBioseqs (esp->input_entityID, NULL, (Pointer) esp,
			DoEditStrand_BioseqCallback, TRUE, FALSE, TRUE);

  /* Flag as changed, cleanup, and exit */
  MemFree (esp->findThisStr);

  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (esp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, esp->input_entityID, 0, 0);
  Remove (esp->form);
}

static void CleanupEditStrandForm_Callback (GraphiC g, VoidPtr data)

{
  EditStrandPtr  esp;

  esp = (EditStrandPtr) data;

  if (esp != NULL) 
    ValNodeFree (esp->featlist);

  StdCleanupFormProc (g, data);
}

extern void EditFeatureStrand (IteM i)
{
  ButtoN         b;
  BaseFormPtr    bfp;
  GrouP          c;
  EditStrandPtr  esp;
  GrouP          g;
  GrouP          h;
  ValNodePtr     head;
  GrouP          k;
  Int2           listHeight;
  ValNodePtr     vnp;
  WindoW         w;
  GrouP          y;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL)
    return;

  /* Create the edit feature window */

  w = FixedWindow (-50, -33, -10, -10, "Feature Strand Editor",
		   StdCloseWindowProc);
  /* Create a form for passing data to the callbacks */

  esp = (EditStrandPtr) MemNew (sizeof (EditStrand));
  esp->form = (ForM) w;
  esp->formmessage = NULL;
  esp->input_entityID = bfp->input_entityID;

  SetObjectExtra (w, esp, CleanupEditStrandForm_Callback);

  /* Main overall group */

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  /* Create list box for feature types */

  k = HiddenGroup (h, 0, 2, NULL);
  StaticPrompt (k, "Feature", 0, 0, programFont, 'c');
  if (indexerVersion) {
    listHeight = 16;
  } else {
    listHeight = 8;
  }
  esp->feature = SingleList (k, 16, listHeight, NULL);

  /* Create a val node list of all feature types */

  head = BuildFeatureValNodeList (TRUE, "[ALL FEATURES]", 0, FALSE, TRUE);

  /* Use the val node list to populate the feature list box */

  if (head != NULL) {
    for (vnp = head; vnp != NULL; vnp = vnp->next)
      ListItem (esp->feature, (CharPtr) vnp->data.ptrvalue);
  }
  esp->featlist = head;

  /* Create popups for source and destination strand type */

  g = HiddenGroup (h, 5, 0, NULL);

  StaticPrompt (g, "Convert From", 0, stdLineHeight, programFont, 'l');
  esp->fromPopup = PopupList (g, TRUE, NULL);
  PopupItem (esp->fromPopup, "Unknown");
  PopupItem (esp->fromPopup, "Plus");
  PopupItem (esp->fromPopup, "Minus");
  PopupItem (esp->fromPopup, "Both");
  PopupItem (esp->fromPopup, "Reverse");
  SetValue (esp->fromPopup, 1);

  StaticPrompt (g, "To", 0, stdLineHeight, programFont, 'l');
  esp->toPopup = PopupList (g, TRUE, NULL);
  PopupItem (esp->toPopup, "Unknown");
  PopupItem (esp->toPopup, "Plus");
  PopupItem (esp->toPopup, "Minus");
  PopupItem (esp->toPopup, "Both");
  PopupItem (esp->toPopup, "Reverse");
  SetValue (esp->toPopup, 1);

  y = HiddenGroup (h, 2, 0, NULL);
  StaticPrompt (y, "Optional string constraint", 0,
                dialogTextHeight, programFont, 'c');
  esp->findThis = DialogText (y, "", 14, NULL);

  /* Create accept and cancel buttons */

  c = HiddenGroup (h, 2, 0, NULL);
  b = DefaultButton (c, "Accept", DoEditFeatureStrand);
  SetObjectExtra (b, esp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  /* Layout the objects and display the window */

  AlignObjects (ALIGN_CENTER, (HANDLE) k, (HANDLE) g, (HANDLE) y, (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Update ();
}

extern void ClearCdsProducts (IteM i);
extern void ClearMrnaProducts (IteM i);

static void ClearAProduct (SeqFeatPtr sfp, Pointer userdata)

{
  Uint2Ptr  featdefptr;
  Uint2     subtype;

  featdefptr = (Uint2Ptr) userdata;
  subtype = *featdefptr;
  if (sfp->idx.subtype != subtype) return;
  sfp->product = SeqLocFree (sfp->product);
}

static void ClearFeatProducts (IteM i, Uint2 featdeftype)

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
  VisitFeaturesInSep (sep, (Pointer) &featdeftype, ClearAProduct);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

extern void ClearCdsProducts (IteM i)

{
  ClearFeatProducts (i, FEATDEF_CDS);
}

extern void ClearMrnaProducts (IteM i)

{
  ClearFeatProducts (i, FEATDEF_mRNA);
}

static void ConsolidateOneLikeSubSourceModifier (
  SubSourcePtr match_to,
  Boolean use_semicolon
)
{
  SubSourcePtr prev, index;
  Int4         len, num_matches;
  CharPtr      new_value;

  if (match_to == NULL) return;
  len = StringLen (match_to->name) + 1;
  num_matches = 0;
  prev = match_to;
  index = match_to->next;
  while (index != NULL)
  {
    if (index->subtype == match_to->subtype && index->name != NULL)
    {
      len += StringLen (index->name) + 2;
      num_matches++;
    }
    index = index->next;
  }
  if (num_matches == 0) return;

  new_value = MemNew (len * sizeof (char));
  if (new_value == NULL) return;

  StringCpy (new_value, match_to->name);
  index = match_to->next;
  while (index != NULL)
  {
    if (index->subtype == match_to->subtype && index->name != NULL)
    {
      if (use_semicolon)
      {
        StringCat (new_value, "; ");
      }
      else
      {
        StringCat (new_value, " ");
      }
      StringCat (new_value, index->name);
      prev->next = index->next;
      index->next = NULL;
      SubSourceFree (index);
      index = prev;
    }
    prev = index;
    index = index->next;
  }
  MemFree (match_to->name);
  match_to->name = new_value; 
}
  
static void ConsolidateOneLikeOrganismModifier (
  OrgModPtr match_to,
  Boolean use_semicolon
)
{
  OrgModPtr prev, index;
  Int4      len, num_matches;
  CharPtr   new_value;

  if (match_to == NULL) return;
  len = StringLen (match_to->subname) + 1;
  num_matches = 0;
  prev = match_to;
  index = match_to->next;
  while (index != NULL)
  {
    if (index->subtype == match_to->subtype && index->subname != NULL)
    {
      len += StringLen (index->subname) + 2;
      num_matches++;
    }
    index = index->next;
  }
  if (num_matches == 0) return;

  new_value = MemNew (len * sizeof (char));
  if (new_value == NULL) return;

  StringCpy (new_value, match_to->subname);
  index = match_to->next;
  while (index != NULL)
  {
    if (index->subtype == match_to->subtype && index->subname != NULL)
    {
      if (use_semicolon)
      {
        StringCat (new_value, "; ");
      }
      else
      {
        StringCat (new_value, " ");
      }
      StringCat (new_value, index->subname);
      prev->next = index->next;
      index->next = NULL;
      OrgModFree (index);
      index = prev;
    }
    prev = index;
    index = index->next;
  }
  MemFree (match_to->subname);
  match_to->subname = new_value; 
}
  
static void ConsolidateLikeModifiersProc (BioSourcePtr biop, Pointer userdata)
{
  SubSourcePtr ssp;
  OrgModPtr    mod;
  Boolean      use_semicolon;

  if (biop == NULL || userdata == NULL) return;

  use_semicolon = *((Boolean PNTR) userdata);

  for (ssp = biop->subtype; ssp != NULL; ssp = ssp->next)
  {
    if (ssp->name != NULL)
    {
      ConsolidateOneLikeSubSourceModifier (ssp, use_semicolon);
    }
  }
    
  if (biop->org == NULL || biop->org->orgname == NULL) return;
  for (mod = biop->org->orgname->mod; mod != NULL; mod = mod->next)
  {
    if (mod->subname != NULL)
    {
      ConsolidateOneLikeOrganismModifier (mod, use_semicolon);
    }
  }
}

static void ConsolidateLikeModifiers (IteM i, Boolean use_semicolon)
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
  VisitBioSourcesInSep (sep, &use_semicolon, ConsolidateLikeModifiersProc);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

extern void ConsolidateLikeModifiersWithSemicolons (IteM i)
{
  ConsolidateLikeModifiers (i, TRUE);
}

extern void ConsolidateLikeModifiersWithoutSemicolons (IteM i)
{
  ConsolidateLikeModifiers (i, FALSE);
}

static void ConsolidateOrganismNotesProc (BioSourcePtr biop, Pointer userdata)
{
  SubSourcePtr ssp, note_ssp;
  OrgModPtr    mod, note_mod;

  if (biop == NULL) return;

  for (ssp = biop->subtype; ssp != NULL; ssp = ssp->next)
  {
    if (ssp->subtype == 255 && ssp->name != NULL)
    {
      ConsolidateOneLikeSubSourceModifier (ssp, TRUE);
      note_ssp = ssp;
    }
  }
    
  if (biop->org == NULL || biop->org->orgname == NULL) return;
  for (mod = biop->org->orgname->mod; mod != NULL; mod = mod->next)
  {
    if (mod->subtype == 255 && mod->subname != NULL)
    {
      ConsolidateOneLikeOrganismModifier (mod, TRUE);
      note_mod = mod;
    }
  }
}

extern void ConsolidateOrganismNotes (IteM i)
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
  VisitBioSourcesInSep (sep, NULL, ConsolidateOrganismNotesProc);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

