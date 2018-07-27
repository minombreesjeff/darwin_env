/*   sequin6.c
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
* File Name:  sequin6.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   11/12/97
*
* $Revision: 6.58 $
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
#include <gather.h>
#include <asn2ff.h>
#include <ffprint.h>
#include <bspview.h>
#include <import.h>

static ENUM_ALIST(target_field_alist)
  {" ",                    0},
  {"Gene",                 1},
  {"CDS",                  2},
  {"Prot",                 3},
  {"RNA",                  4},
  {"BioSource",            5},
  {"OrgMod",               6},
  {"SubSource",            7},
END_ENUM_ALIST

static ENUM_ALIST(ext_target_field_alist)
  {" ",                    0},
  {"Gene",                 1},
  {"CDS",                  2},
  {"Prot",                 3},
  {"RNA",                  4},
  {"BioSource",            5},
  {"OrgMod",               6},
  {"SubSource",            7},
  {"Feature Note",         8},
END_ENUM_ALIST

static ENUM_ALIST(gene_field_alist)
  {" ",                    0},
  {"Gene locus",           1},
  {"Gene description",     2},
  {"Gene allele",          3},
  {"Gene maploc",          4},
  {"Gene synonym",         5},
  {"Gene comment",         6},
END_ENUM_ALIST

static ENUM_ALIST(cds_field_alist)
  {" ",                    0},
  {"CDS comment",          1},
  {"gene xref",            2},
  {"db_xref",              3},
END_ENUM_ALIST

static ENUM_ALIST(prot_field_alist)
  {" ",                    0},
  {"Protein name",         1},
  {"Protein description",  2},
  {"Protein E.C. number",  3},
  {"Protein activity",     4},
  {"Protein comment",      5},
END_ENUM_ALIST

static ENUM_ALIST(rnax_field_alist)
  {" ",                    0},
  {"RNA Name",             1},
  {"RNA Comment",          2},
  {"gene xref",            3},
END_ENUM_ALIST

static ENUM_ALIST(orgref_field_alist)
  {" ",                    0},
  {"Scientific Name",      1},
  {"Common Name",          2},
  {"Lineage",              3},
  {"Division",             4},
END_ENUM_ALIST

static ENUM_ALIST(orgmod_note_subtype_alist)
  {" ",                 0},
  {"Strain",            2},
  {"Substrain",         3},
  {"Type",              4},
  {"Subtype",           5},
  {"Variety",           6},
  {"Serotype",          7},
  {"Serogroup",         8},
  {"Serovar",           9},
  {"Cultivar",         10},
  {"Pathovar",         11},
  {"Chemovar",         12},
  {"Biovar",           13},
  {"Biotype",          14},
  {"Group",            15},
  {"Subgroup",         16},
  {"Isolate",          17},
  {"Common",           18},
  {"Acronym",          19},
  {"Dosage",           20},
  {"Natural-host",     21},
  {"Sub-species",      22},
  {"Specimen-voucher", 23},
  {"Authority",        24},
  {"Forma",            25},
  {"Forma-specialis",  26},
  {"Ecotype",          27},
  {"Synonym",          28},
  {"Anamorph",         29},
  {"Teleomorph",       30},
  {"Breed",            31},
  {"Old Lineage",     253},
  {"Old Name",        254},
  {"Note",            255},
END_ENUM_ALIST

static ENUM_ALIST(subsource_note_subtype_alist)
  {" ",                      0},
  {"Chromosome",             1},
  {"Map",                    2},
  {"Clone",                  3},
  {"Subclone",               4},
  {"Haplotype",              5},
  {"Genotype",               6},
  {"Sex",                    7},
  {"Cell-line",              8},
  {"Cell-type",              9},
  {"Tissue-type",           10},
  {"Clone-lib",             11},
  {"Dev-stage",             12},
  {"Frequency",             13},
  {"Germline",              14},
  {"Rearranged",            15},
  {"Lab-host",              16},
  {"Pop-variant",           17},
  {"Tissue-lib",            18},
  {"Plasmid-name",          19},
  {"Transposon-name",       20},
  {"Ins-seq-name",          21},
  {"Plastid-name",          22},
  {"Country",               23},
  {"Segment",               24},
  {"Endogenous-virus-name", 25},
  {"Transgenic",            26},
  {"Environmental-sample",  27},
  {"Isolation-source",      28},
  {"Note",                 255},
END_ENUM_ALIST

extern EnumFieldAssoc  orgmod_subtype_alist [];
extern EnumFieldAssoc  subsource_subtype_alist [];

#define NUM_SUBTARGET_POPUPS 10

typedef struct convertformdata {
  FEATURE_FORM_BLOCK

  GrouP              asnOrFlat;
  TexT               atleft;
  TexT               atright;
  PopuP              target;
  PopuP              subtarget [NUM_SUBTARGET_POPUPS];
  EnumFieldAssocPtr  alists [NUM_SUBTARGET_POPUPS];
  ButtoN             accept;
  Int2               report;
  Int2               type;
  Int2               subtype;
  CharPtr            leftstr;
  CharPtr            rightstr;
  CharPtr            foundstr;
  GrouP              leftbehav;
  GrouP              rightbehav;
  Boolean            includeleft;
  Boolean            includeright;
  Boolean            replaceOldAsked;
  Boolean            doReplaceAll;
  Int2               index;
} ConvertFormData, PNTR ConvertFormPtr;

static Boolean ShouldReplaceParsedString (ConvertFormPtr cfp)

{
  MsgAnswer  ans;

  if (cfp == NULL) return FALSE;
  if (! cfp->replaceOldAsked) {
    cfp->replaceOldAsked = TRUE;
    ArrowCursor ();
    Update ();
    ans = Message (MSG_YN, "Do you wish to overwrite existing contents?");
    WatchCursor ();
    Update ();
    cfp->doReplaceAll = (Boolean) (ans == ANS_YES);
  }
  return cfp->doReplaceAll;
}

static CharPtr SaveOrReplaceStringCopy (ConvertFormPtr cfp, CharPtr str, CharPtr current)

{
  size_t   len;
  CharPtr  tmp;

  if (cfp == NULL) return NULL;
  if (StringHasNoText (str)) return current;
  if (current != NULL) {
    if (ShouldReplaceParsedString (cfp)) {
      MemFree (current);
    } else {
      len = StringLen (str) + StringLen (current) + 4;
      tmp = MemNew (len);
      if (tmp != NULL) {
        StringCpy (tmp, current);
        StringCat (tmp, "; ");
        StringCat (tmp, str);
        /* str is within asn2ff string, so must not MemFree (str); */
        MemFree (current);
        return tmp;
      }
    }
  }
  return StringSave (str);
}

static void ConvertAFeatureField (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr       bsp;
  BioseqSetPtr    bssp;
  ConvertFormPtr  cfp;
  GBQualPtr       gbq;
  GeneRefPtr      grp;
  ProtRefPtr      prp;
  RnaRefPtr       rrp;
  SeqAnnotPtr     sap;
  SeqFeatPtr      sfp;
  ValNodePtr      vnp;
  SeqFeatXrefPtr  xref;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  cfp = (ConvertFormPtr) mydata;
  if (cfp == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
  } else return;
  while (sap != NULL) {
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      while (sfp != NULL) {
        if (sfp->data.choice == SEQFEAT_GENE && cfp->type == 1) {
          grp = (GeneRefPtr) sfp->data.value.ptrvalue;
          if (grp != NULL) {
            switch (cfp->subtype) {
              case 1 :
                grp->locus = SaveOrReplaceStringCopy (cfp, cfp->foundstr, grp->locus);
                break;
              case 2 :
                grp->desc = SaveOrReplaceStringCopy (cfp, cfp->foundstr, grp->desc);
                break;
              case 3 :
                grp->allele = SaveOrReplaceStringCopy (cfp, cfp->foundstr, grp->allele);
                break;
              case 4 :
                grp->maploc = SaveOrReplaceStringCopy (cfp, cfp->foundstr, grp->maploc);
                break;
              case 5 :
                if (! StringHasNoText (cfp->foundstr)) {
                  if (grp->syn != NULL && ShouldReplaceParsedString (cfp)) {
                    vnp = grp->syn;
                    vnp->data.ptrvalue = MemFree (vnp->data.ptrvalue);
                    vnp->data.ptrvalue = StringSave (cfp->foundstr);
                  } else {
                    vnp = ValNodeNew (NULL);
                    if (vnp != NULL) {
                      vnp->next = grp->syn;
                      grp->syn = vnp;
                      vnp->data.ptrvalue = StringSave (cfp->foundstr);;
                    }
                  }
                }
                break;
              case 6 :
                sfp->comment = SaveOrReplaceStringCopy (cfp, cfp->foundstr, sfp->comment);
                break;
              default :
                break;
            }
          }
        } else if (sfp->data.choice == SEQFEAT_CDREGION && cfp->type == 2) {
          switch (cfp->subtype) {
            case 1 :
              sfp->comment = SaveOrReplaceStringCopy (cfp, cfp->foundstr, sfp->comment);
              break;
            case 2 :
              if (! StringHasNoText (cfp->foundstr)) {
                xref = SeqFeatXrefNew ();
                if (xref != NULL) {
                  grp = CreateNewGeneRef (cfp->foundstr, NULL, NULL, FALSE);
                  if (grp != NULL) {
                    xref->data.choice = SEQFEAT_GENE;
                    xref->data.value.ptrvalue = grp;
                    xref->next = sfp->xref;
                    sfp->xref = xref;
                  }
                }
              }
              break;
            case 3 :
              gbq = GBQualNew ();
              if (gbq != NULL) {
                /* BasicSeqEntryCleanup will convert to real sfp->dbxref */
                gbq->qual = StringSave ("db_xref");
                gbq->val = StringSave (cfp->foundstr);
                gbq->next = sfp->qual;
                sfp->qual = gbq;
              }
              break;
            default :
              break;
          }
        } else if (sfp->data.choice == SEQFEAT_PROT && cfp->type == 3) {
          prp = (ProtRefPtr) sfp->data.value.ptrvalue;
          if (prp != NULL) {
            switch (cfp->subtype) {
              case 1 :
                if (! StringHasNoText (cfp->foundstr)) {
                  if (prp->name != NULL && ShouldReplaceParsedString (cfp)) {
                    vnp = prp->name;
                    vnp->data.ptrvalue = MemFree (vnp->data.ptrvalue);
                    vnp->data.ptrvalue = StringSave (cfp->foundstr);
                  } else {
                    vnp = ValNodeNew (NULL);
                    if (vnp != NULL) {
                      vnp->next = prp->name;
                      prp->name = vnp;
                      vnp->data.ptrvalue = StringSave (cfp->foundstr);
                    }
                  }
                }
                break;
              case 2 :
                prp->desc = SaveOrReplaceStringCopy (cfp, cfp->foundstr, prp->desc);
                break;
              case 3 :
                if (! StringHasNoText (cfp->foundstr)) {
                  if (prp->ec != NULL && ShouldReplaceParsedString (cfp)) {
                    vnp = prp->ec;
                    vnp->data.ptrvalue = MemFree (vnp->data.ptrvalue);
                    vnp->data.ptrvalue = StringSave (cfp->foundstr);
                  } else {
                    vnp = ValNodeNew (NULL);
                    if (vnp != NULL) {
                      vnp->next = prp->ec;
                      prp->ec = vnp;
                      vnp->data.ptrvalue = StringSave (cfp->foundstr);
                    }
                  }
                }
                break;
              case 4 :
                if (! StringHasNoText (cfp->foundstr)) {
                  if (prp->activity != NULL && ShouldReplaceParsedString (cfp)) {
                    vnp = prp->activity;
                    vnp->data.ptrvalue = MemFree (vnp->data.ptrvalue);
                    vnp->data.ptrvalue = StringSave (cfp->foundstr);
                  } else {
                    vnp = ValNodeNew (NULL);
                    if (vnp != NULL) {
                      vnp->next = prp->activity;
                      prp->activity = vnp;
                      vnp->data.ptrvalue = StringSave (cfp->foundstr);
                    }
                  }
                }
                break;
              case 5 :
                sfp->comment = SaveOrReplaceStringCopy (cfp, cfp->foundstr, sfp->comment);
                break;
              default :
                break;
            }
          }
        } else if (sfp->data.choice == SEQFEAT_RNA && cfp->type == 4) {
          rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
          if (rrp != NULL) {
            switch (cfp->subtype) {
              case 1 :
                if (rrp->ext.choice == 0 || rrp->ext.choice == 1) {
                  rrp->ext.choice = 1;
                  rrp->ext.value.ptrvalue = SaveOrReplaceStringCopy (cfp, cfp->foundstr, rrp->ext.value.ptrvalue);
                }
                break;
              case 2 :
                sfp->comment = SaveOrReplaceStringCopy (cfp, cfp->foundstr, sfp->comment);
                break;
              case 3 :
                if (! StringHasNoText (cfp->foundstr)) {
                  xref = SeqFeatXrefNew ();
                  if (xref != NULL) {
                    grp = CreateNewGeneRef (cfp->foundstr, NULL, NULL, FALSE);
                    if (grp != NULL) {
                      xref->data.choice = SEQFEAT_GENE;
                      xref->data.value.ptrvalue = grp;
                      xref->next = sfp->xref;
                      sfp->xref = xref;
                    }
                  }
                }
              default :
                break;
            }
          }
        } else if (cfp->type == 8) {
          if (cfp->subtype == sfp->idx.subtype) {
            sfp->comment = SaveOrReplaceStringCopy (cfp, cfp->foundstr, sfp->comment);
          }
        }
        sfp = sfp->next;
      }
    }
    sap = sap->next;
  }
}

static void ConvertASourceField (BioSourcePtr biop, ConvertFormPtr cfp)

{
  OrgModPtr     mod;
  OrgNamePtr    onp;
  OrgRefPtr     orp;
  SubSourcePtr  ssp;
  OrgModPtr     tmpmod;
  SubSourcePtr  tmpssp;

  if (biop == NULL || cfp == NULL) return;
  switch (cfp->type) {
    case 5 :
      orp = biop->org;
      if (orp == NULL) return;
      switch (cfp->subtype) {
        case 1 :
          orp->taxname = SaveOrReplaceStringCopy (cfp, cfp->foundstr, orp->taxname);
          break;
        case 2 :
          orp->common = SaveOrReplaceStringCopy (cfp, cfp->foundstr, orp->common);
          break;
        case 3 :
          onp = orp->orgname;
          if (onp == NULL) {
            onp = OrgNameNew ();
            orp->orgname = onp;
          }
          if (onp != NULL) {
            onp->lineage = SaveOrReplaceStringCopy (cfp, cfp->foundstr, onp->lineage);
          }
          break;
        case 4 :
          onp = orp->orgname;
          if (onp == NULL) {
            onp = OrgNameNew ();
            orp->orgname = onp;
          }
          if (onp != NULL) {
            onp->div = SaveOrReplaceStringCopy (cfp, cfp->foundstr, onp->div);
          }
          break;
        default :
          break;
      }
      break;
    case 6 :
      orp = biop->org;
      if (orp == NULL) {
        orp = OrgRefNew ();
        biop->org = orp;
      }
      if (orp != NULL) {
        onp = orp->orgname;
        if (onp == NULL) {
          onp = OrgNameNew ();
          orp->orgname = onp;
        }
        if (onp != NULL) {
          mod = onp->mod;
          while (mod != NULL && mod->subtype != cfp->subtype) {
            mod = mod->next;
          }
          if (mod == NULL) {
            mod = OrgModNew ();
            if (onp->mod == NULL) {
              onp->mod = mod;
            } else {
              tmpmod = onp->mod;
              while (tmpmod->next != NULL) {
                tmpmod = tmpmod->next;
              }
              tmpmod->next = mod;
            }
          }
          if (mod != NULL) {
            mod->subtype = cfp->subtype;
            mod->subname = SaveOrReplaceStringCopy (cfp, cfp->foundstr, mod->subname);
          }
        }
      }
      break;
    case 7 :
      ssp = biop->subtype;
      while (ssp != NULL && ssp->subtype != cfp->subtype) {
        ssp = ssp->next;
      }
      if (ssp == NULL) {
        ssp = SubSourceNew ();
        if (biop->subtype == NULL) {
          biop->subtype = ssp;
        } else {
          tmpssp = biop->subtype;
          while (tmpssp->next != NULL) {
            tmpssp = tmpssp->next;
          }
          tmpssp->next = ssp;
        }
      }
      if (ssp != NULL) {
        ssp->subtype = cfp->subtype;
        ssp->name = SaveOrReplaceStringCopy (cfp, cfp->foundstr, ssp->name);
      }
      break;
    default :
      break;
  }
}

static void FoundStringForConversion (Uint2 entityID, SeqEntryPtr sep, ConvertFormPtr cfp, CharPtr str)

{
  BioSourcePtr  biop;
  OrgRefPtr     orp;
  ValNodePtr    vnp;

  cfp->foundstr = str;
  switch (cfp->type) {
    case 1 :
    case 2 :
    case 3 :
    case 4 :
      SeqEntryExplore (sep, (Pointer) cfp, ConvertAFeatureField);
      break;
    case 5 :
      SeqEntryToBioSource (sep, NULL, NULL, 0, &biop);
      if (biop == NULL && cfp->subtype == 1) {
        biop = BioSourceNew ();
        if (biop == NULL) return;
        orp = OrgRefNew ();
        biop->org = orp;
        vnp = CreateNewDescriptor (sep, Seq_descr_source);
        if (vnp != NULL) {
          vnp->data.ptrvalue = (Pointer) biop;
        }
      }
      ConvertASourceField (biop, cfp);
      break;
    case 6 :
    case 7 :
      SeqEntryToBioSource (sep, NULL, NULL, 0, &biop);
      ConvertASourceField (biop, cfp);
      break;
    case 8 :
      SeqEntryExplore (sep, (Pointer) cfp, ConvertAFeatureField);
      break;
    default :
      break;
  }
}

static void ConvertFromFlatFile (Uint2 entityID, SeqEntryPtr sep, ConvertFormPtr cfp, Uint1 format)

{
  Asn2ffJobPtr     ajp;
  Boolean          goOn;
  Int4             index;
  ErrSev           level;
  CharPtr          lft = NULL;
  Boolean          okay;
  FFPrintArrayPtr  pap = NULL;
  Int4             pap_size;
  CharPtr          rgt = NULL;
  CharPtr          string;
  CharPtr          tmp;

  ajp = (Asn2ffJobPtr) MemNew (sizeof (Asn2ffJob));
  if (ajp == NULL) return;
  level = ErrSetMessageLevel (SEV_MAX);

  ajp->sep = sep;
  ajp->mode = SEQUIN_MODE;
  ajp->format = format;
  ajp->gb_style = TRUE;
  ajp->show_seq = TRUE;
  ajp->show_gi = TRUE;
  ajp->error_msgs = FALSE;
  ajp->non_strict = TRUE;
  ajp->Spop = spop;
  ajp->show_gene = TRUE;
  if (IsAGenomeRecord (sep) || IsSegmentedBioseqWithoutParts (sep)) {
    ajp->only_one = TRUE;
    ajp->genome_view = TRUE;
  }
  ajp->bankit = TRUE;

  pap_size = asn2ff_setup (ajp, &pap);
  if (pap_size > 0) {
    asn2ff_set_output (NULL, "\n");
    goOn = TRUE;
    for (index = 0; index < pap_size && goOn; index++) {
      string = FFPrint (pap, index, pap_size);
      if (string != NULL && *string != '\0') {
        CompressSpaces (string);
        okay = TRUE;
        tmp = string;
        if (cfp->leftstr != NULL && cfp->leftstr [0] != '\0') {
          lft = SearchForString (string, cfp->leftstr, FALSE, FALSE);
          if (lft != NULL) {
            if (! cfp->includeleft) {
              lft += StringLen (cfp->leftstr);
            }
            tmp = lft;
          } else {
            okay = FALSE;
          }
        }
        if (cfp->rightstr != NULL && cfp->rightstr [0] != '\0') {
          rgt = SearchForString (tmp, cfp->rightstr, FALSE, FALSE);
          if (rgt != NULL) {
            if (cfp->includeright) {
              rgt += StringLen (cfp->rightstr);
            }
            *rgt = '\0';
          } else {
            okay = FALSE;
          }
        }
        if (okay) {
          if (lft != NULL) {
            lft = tmp;
          } else {
            lft = string;
          }
          if (! StringHasNoText (lft)) {
            FoundStringForConversion (entityID, sep, cfp, lft);
            goOn = FALSE;
          }
        }
      }
      MemFree (string);
    }
    MemFree (pap);
  }
  asn2ff_cleanup (ajp);

  MemFree (ajp);
  ErrSetMessageLevel (level);
}

static void DoOneConvert (Uint2 entityID, SeqEntryPtr sep, ConvertFormPtr cfp, MonitorPtr mon)

{
  BioseqSetPtr  bssp;
  Char          str [64];

  if (sep == NULL || cfp == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 7 ||
                         (bssp->_class >= 13 && bssp->_class <= 16))) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        DoOneConvert (entityID, sep, cfp, mon);
      }
      return;
    }
  }
  (cfp->index)++;
  if (mon != NULL) {
    sprintf (str, "Parsing component %d", (int) cfp->index);
    MonitorStrValue (mon, str);
  }
  switch (cfp->report) {
    case 1 :
      ConvertFromFlatFile (entityID, sep, cfp, GENBANK_FMT);
      break;
    case 2 :
      ConvertFromFlatFile (entityID, sep, cfp, EMBL_FMT);
      break;
    default :
      break;
  }
}

static void SearchAndExciseText (CharPtr PNTR strptr, ConvertFormPtr cfp)

{
  Char     ch;
  CharPtr  lft = NULL;
  CharPtr  rgt = NULL;
  CharPtr  string;
  CharPtr  tmp;

  if (strptr == NULL || cfp == NULL) return;
  string = *strptr;
  if (string == NULL) return;
  tmp = string;
  if (cfp->leftstr != NULL && cfp->leftstr [0] != '\0') {
    lft = SearchForString (string, cfp->leftstr, FALSE, FALSE);
    if (lft == NULL) return;
    if (! cfp->includeleft) {
      lft += StringLen (cfp->leftstr);
    }
    tmp = lft;
  }
  if (cfp->rightstr != NULL && cfp->rightstr [0] != '\0') {
    rgt = SearchForString (tmp, cfp->rightstr, FALSE, FALSE);
    if (rgt == NULL) return;
    if (cfp->includeright) {
      rgt += StringLen (cfp->rightstr);
    }
  }
  if (lft != NULL) {
    *tmp = '\0';
    lft = tmp;
  } else {
    lft = string;
  }
  if (rgt != NULL) {
    ch = *rgt;
    while (ch != '\0') {
      *lft = ch;
      rgt++;
      lft++;
      ch = *rgt;
    }
    *lft = '\0';
  }
}

static void RemoveAFeatureText (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr       bsp;
  BioseqSetPtr    bssp;
  ConvertFormPtr  cfp;
  GeneRefPtr      grp;
  ProtRefPtr      prp;
  RnaRefPtr       rrp;
  SeqAnnotPtr     sap;
  SeqFeatPtr      sfp;
  CharPtr PNTR    strp;
  ValNodePtr      vnp;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  cfp = (ConvertFormPtr) mydata;
  if (cfp == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
  } else return;
  while (sap != NULL) {
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      while (sfp != NULL) {
        if (sfp->data.choice == SEQFEAT_GENE && cfp->type == 1) {
          grp = (GeneRefPtr) sfp->data.value.ptrvalue;
          if (grp != NULL) {
            switch (cfp->subtype) {
              case 1 :
                SearchAndExciseText (&grp->locus, cfp);
                break;
              case 2 :
                SearchAndExciseText (&grp->desc, cfp);
                break;
              case 3 :
                SearchAndExciseText (&grp->allele, cfp);
                break;
              case 4 :
                SearchAndExciseText (&grp->maploc, cfp);
                break;
              case 5 :
                for (vnp = grp->syn; vnp != NULL; vnp = vnp->next) {
                  strp = (CharPtr PNTR) &vnp->data.ptrvalue;
                  SearchAndExciseText (strp, cfp);
                }
                break;
              case 6 :
                SearchAndExciseText (&sfp->comment, cfp);
                break;
              default :
                break;
            }
          }
        } else if (sfp->data.choice == SEQFEAT_CDREGION && cfp->type == 2) {
          switch (cfp->subtype) {
            case 1 :
              SearchAndExciseText (&sfp->comment, cfp);
              break;
            case 2 :
              break;
            default :
              break;
          }
        } else if (sfp->data.choice == SEQFEAT_PROT && cfp->type == 3) {
          prp = (ProtRefPtr) sfp->data.value.ptrvalue;
          if (prp != NULL) {
            switch (cfp->subtype) {
              case 1 :
                for (vnp = prp->name; vnp != NULL; vnp = vnp->next) {
                  strp = (CharPtr PNTR) &vnp->data.ptrvalue;
                  SearchAndExciseText (strp, cfp);
                }
                break;
              case 2 :
                SearchAndExciseText (&prp->desc, cfp);
                break;
              case 3 :
                for (vnp = prp->ec; vnp != NULL; vnp = vnp->next) {
                  strp = (CharPtr PNTR) &vnp->data.ptrvalue;
                  SearchAndExciseText (strp, cfp);
                }
                break;
              case 4 :
                for (vnp = prp->activity; vnp != NULL; vnp = vnp->next) {
                  strp = (CharPtr PNTR) &vnp->data.ptrvalue;
                  SearchAndExciseText (strp, cfp);
                }
                break;
              case 5 :
                SearchAndExciseText (&sfp->comment, cfp);
                break;
              default :
                break;
            }
          }
        } else if (sfp->data.choice == SEQFEAT_RNA && cfp->type == 4) {
          rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
          if (rrp != NULL) {
            switch (cfp->subtype) {
              case 1 :
                if (rrp->ext.choice == 0 || rrp->ext.choice == 1) {
                  rrp->ext.choice = 1;
                  strp = (CharPtr PNTR) &rrp->ext.value.ptrvalue;
                  SearchAndExciseText (strp, cfp);
                }
                break;
              case 2 :
                SearchAndExciseText (&sfp->comment, cfp);
                break;
              case 3 :
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

static void RemoveASourceText (BioSourcePtr biop, ConvertFormPtr cfp)

{
  OrgModPtr     mod;
  OrgNamePtr    onp;
  OrgRefPtr     orp;
  SubSourcePtr  ssp;

  if (biop == NULL || cfp == NULL) return;
  switch (cfp->type) {
    case 5 :
      orp = biop->org;
      if (orp == NULL) return;
      switch (cfp->subtype) {
        case 1 :
          SearchAndExciseText (&orp->taxname, cfp);
          break;
        case 2 :
          SearchAndExciseText (&orp->common, cfp);
          break;
        case 3 :
          onp = orp->orgname;
          if (onp == NULL) {
            onp = OrgNameNew ();
            orp->orgname = onp;
          }
          if (onp != NULL) {
            SearchAndExciseText (&onp->lineage, cfp);
          }
          break;
        case 4 :
          onp = orp->orgname;
          if (onp == NULL) {
            onp = OrgNameNew ();
            orp->orgname = onp;
          }
          if (onp != NULL) {
            SearchAndExciseText (&onp->div, cfp);
          }
          break;
        default :
          break;
      }
      break;
    case 6 :
      orp = biop->org;
      if (orp == NULL) {
        orp = OrgRefNew ();
        biop->org = orp;
      }
      if (orp != NULL) {
        onp = orp->orgname;
        if (onp == NULL) {
          onp = OrgNameNew ();
          orp->orgname = onp;
        }
        if (onp != NULL) {
          mod = onp->mod;
          while (mod != NULL && mod->subtype != cfp->subtype) {
            mod = mod->next;
          }
          if (mod != NULL) {
            SearchAndExciseText (&mod->subname, cfp);
          }
        }
      }
      break;
    case 7 :
      ssp = biop->subtype;
      while (ssp != NULL && ssp->subtype != cfp->subtype) {
        ssp = ssp->next;
      }
      if (ssp != NULL) {
        SearchAndExciseText (&ssp->name, cfp);
      }
      break;
    default :
      break;
  }
}

static void DoOneRemoveText (Uint2 entityID, SeqEntryPtr sep, ConvertFormPtr cfp, MonitorPtr mon)

{
  BioSourcePtr  biop;
  BioseqSetPtr  bssp;
  Char          str [64];

  if (sep == NULL || cfp == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 7 ||
                         (bssp->_class >= 13 && bssp->_class <= 16))) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        DoOneRemoveText (entityID, sep, cfp, mon);
      }
      return;
    }
  }
  (cfp->index)++;
  if (mon != NULL) {
    sprintf (str, "Processing component %d", (int) cfp->index);
    MonitorStrValue (mon, str);
  }
  switch (cfp->type) {
    case 1 :
    case 2 :
    case 3 :
    case 4 :
      SeqEntryExplore (sep, (Pointer) cfp, RemoveAFeatureText);
      break;
    case 5 :
      SeqEntryToBioSource (sep, NULL, NULL, 0, &biop);
      RemoveASourceText (biop, cfp);
      break;
    case 6 :
    case 7 :
      SeqEntryToBioSource (sep, NULL, NULL, 0, &biop);
      RemoveASourceText (biop, cfp);
      break;
    default :
      break;
  }
}

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

static void DoConvertProc (ButtoN b)

{
  ConvertFormPtr  cfp;
  MonitorPtr      mon;
  SeqEntryPtr     sep;
  UIEnum          val;

  cfp = (ConvertFormPtr) GetObjectExtra (b);
  if (cfp == NULL || cfp->input_entityID == 0) return;
  sep = GetTopSeqEntryForEntityID (cfp->input_entityID);
  if (sep == NULL) return;
  Hide (cfp->form);
  WatchCursor ();
  Update ();
  if (GetEnumPopup (cfp->target, ext_target_field_alist, &val) && val > 0) {
    cfp->type = (Int2) val;
    if (GetEnumPopup (cfp->subtarget [cfp->type], cfp->alists [cfp->type], &val) && val > 0) {
      cfp->subtype = (Int2) val;
      cfp->report = GetValue (cfp->asnOrFlat);
      if (cfp->report > 0) {
        cfp->leftstr = SaveStringFromTextNoStripSpaces (cfp->atleft);
        cfp->rightstr = SaveStringFromTextNoStripSpaces (cfp->atright);
        cfp->includeleft = (Boolean) (GetValue (cfp->leftbehav) == 2);
        cfp->includeright = (Boolean) (GetValue (cfp->rightbehav) == 2);
        mon = MonitorStrNewEx ("Parsing Text From FlatFile", 20, FALSE);
        cfp->index = 0;
        DoOneConvert (cfp->input_entityID, sep, cfp, mon);
        MonitorFree (mon);
        cfp->leftstr = MemFree (cfp->leftstr);
        cfp->rightstr = MemFree (cfp->rightstr);
      }
    }
  }
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (cfp->input_entityID, TRUE);
  BasicSeqEntryCleanup (sep);
  ObjMgrSendMsg (OM_MSG_UPDATE, cfp->input_entityID, 0, 0);
  Remove (cfp->form);
}

static void DoRemoveTextProc (ButtoN b)

{
  ConvertFormPtr  cfp;
  MonitorPtr      mon;
  SeqEntryPtr     sep;
  UIEnum          val;

  cfp = (ConvertFormPtr) GetObjectExtra (b);
  if (cfp == NULL || cfp->input_entityID == 0) return;
  sep = GetTopSeqEntryForEntityID (cfp->input_entityID);
  if (sep == NULL) return;
  Hide (cfp->form);
  WatchCursor ();
  Update ();
  if (GetEnumPopup (cfp->target, ext_target_field_alist, &val) && val > 0) {
    cfp->type = (Int2) val;
    if (GetEnumPopup (cfp->subtarget [cfp->type], cfp->alists [cfp->type], &val) && val > 0) {
      cfp->subtype = (Int2) val;
      cfp->leftstr = SaveStringFromTextNoStripSpaces (cfp->atleft);
      cfp->rightstr = SaveStringFromTextNoStripSpaces (cfp->atright);
      cfp->includeleft = (Boolean) (GetValue (cfp->leftbehav) == 2);
      cfp->includeright = (Boolean) (GetValue (cfp->rightbehav) == 2);
      mon = MonitorStrNewEx ("Removing Text From String", 20, FALSE);
      cfp->index = 0;
      DoOneRemoveText (cfp->input_entityID, sep, cfp, mon);
      MonitorFree (mon);
      cfp->leftstr = MemFree (cfp->leftstr);
      cfp->rightstr = MemFree (cfp->rightstr);
    }
  }
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (cfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, cfp->input_entityID, 0, 0);
  Remove (cfp->form);
}

static void SetConvertAcceptButton (Handle a)

{
  ConvertFormPtr  cfp;
  UIEnum          val;

  cfp = (ConvertFormPtr) GetObjectExtra (a);
  if (cfp == NULL) return;
  if (GetEnumPopup (cfp->target, ext_target_field_alist, &val) && val > 0) {
    cfp->type = (Int2) val;
    if (GetEnumPopup (cfp->subtarget [cfp->type], cfp->alists [cfp->type], &val) && val > 0) {
      if (GetValue (cfp->asnOrFlat) > 0) {
        if ((! TextHasNoText (cfp->atleft)) || (! TextHasNoText (cfp->atright))) {
          SafeEnable (cfp->accept);
          return;
        }
      }
      if (cfp->asnOrFlat == NULL) {
        if ((! TextHasNoText (cfp->atleft)) || (! TextHasNoText (cfp->atright))) {
          SafeEnable (cfp->accept);
          return;
        }
      }
    }
  }
  SafeDisable (cfp->accept);
}

static void ChangeSubtarget (PopuP p)

{
  ConvertFormPtr  cfp;
  Int2            i;
  UIEnum          val;

  cfp = (ConvertFormPtr) GetObjectExtra (p);
  if (cfp == NULL) return;
  if (GetEnumPopup (cfp->target, ext_target_field_alist, &val) && val > 0) {
    for (i = 0; i < NUM_SUBTARGET_POPUPS; i++) {
      if (i != (Int2) val) {
        SafeHide (cfp->subtarget [i]);
      }
    }
    SafeShow (cfp->subtarget [(Int2) val]);
  }
  SetConvertAcceptButton ((Handle) p);
}

static void ConvertMessageProc (ForM f, Int2 mssg)

{
  StdEditorProcsPtr  sepp;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    if (sepp->handleMessages != NULL) {
      sepp->handleMessages (f, mssg);
    }
  }
}

static void CleanupParseForm (GraphiC g, VoidPtr data)

{
  ConvertFormPtr  cfp;

  cfp = (ConvertFormPtr) data;
  if (cfp != NULL) {
    if (cfp->alists [8] != NULL) {
      FreeEnumFieldAlist (cfp->alists [8]);
    }
  }
  StdCleanupFormProc (g, data);
}

extern void ParseAsnOrFlatfileToAnywhere (IteM i)

{
  EnumFieldAssocPtr  ap;
  BaseFormPtr        bfp;
  GrouP              c;
  ConvertFormPtr     cfp;
  GrouP              g;
  GrouP              h;
  Int2               j;
  GrouP              p;
  GrouP              q;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
  WindoW             w;
  GrouP              x;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  cfp = (ConvertFormPtr) MemNew (sizeof (ConvertFormData));
  if (cfp == NULL) return;
  w = FixedWindow (-50, -33, -10, -10, "General Field Conversion", StdCloseWindowProc);
  SetObjectExtra (w, cfp, CleanupParseForm);
  cfp->form = (ForM) w;
  cfp->formmessage = ConvertMessageProc;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    cfp->appmessage = sepp->handleMessages;
  }

  cfp->input_entityID = bfp->input_entityID;
  cfp->input_itemID = bfp->input_itemID;
  cfp->input_itemtype = bfp->input_itemtype;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  q = HiddenGroup (h, 4, 0, NULL);
  StaticPrompt (q, "File format to parse", 0, stdLineHeight, programFont, 'l');
  cfp->asnOrFlat = HiddenGroup (q, 3, 0, (GrpActnProc) SetConvertAcceptButton);
  SetObjectExtra (cfp->asnOrFlat, cfp, NULL);
  RadioButton (cfp->asnOrFlat, "GenBank");
  RadioButton (cfp->asnOrFlat, "EMBL");
  SetValue (cfp->asnOrFlat, 1);

  g = HiddenGroup (h, 3, 0, NULL);
  StaticPrompt (g, "Get text between", 0, dialogTextHeight, programFont, 'l');
  cfp->leftbehav = HiddenGroup (g, 2, 0, NULL);
  RadioButton (cfp->leftbehav, "just after");
  RadioButton (cfp->leftbehav, "starting at");
  SetValue (cfp->leftbehav, 1);
  cfp->atleft = DialogText (g, "", 10, (TxtActnProc) SetConvertAcceptButton);
  SetObjectExtra (cfp->atleft, cfp, NULL);
  StaticPrompt (g, "and", 0, dialogTextHeight, programFont, 'l');
  cfp->rightbehav = HiddenGroup (g, 2, 0, NULL);
  RadioButton (cfp->rightbehav, "up to");
  RadioButton (cfp->rightbehav, "including");
  SetValue (cfp->rightbehav, 1);
  cfp->atright = DialogText (g, "", 10, (TxtActnProc) SetConvertAcceptButton);
  SetObjectExtra (cfp->atright, cfp, NULL);

  p = HiddenGroup (h, 6, 0, NULL);
  StaticPrompt (p, "Place in", 0, popupMenuHeight, programFont, 'l');
  cfp->target = PopupList (p, TRUE, ChangeSubtarget);
  SetObjectExtra (cfp->target, cfp, NULL);
  InitEnumPopup (cfp->target, ext_target_field_alist, NULL);
  SetEnumPopup (cfp->target, ext_target_field_alist, 0);

  cfp->alists [1] = gene_field_alist;
  cfp->alists [2] = cds_field_alist;
  cfp->alists [3] = prot_field_alist;
  cfp->alists [4] = rnax_field_alist;
  cfp->alists [5] = orgref_field_alist;
  cfp->alists [6] = orgmod_note_subtype_alist;
  cfp->alists [7] = subsource_note_subtype_alist;
  cfp->alists [8] = import_featdef_alist (TRUE, FALSE, FALSE);

  cfp->replaceOldAsked = FALSE;
  cfp->doReplaceAll = FALSE;

  x = HiddenGroup (p, 0, 0, NULL);

  for (j = 1; j <= 7; j++) {
    cfp->subtarget [j] = PopupList (x, TRUE, (PupActnProc) SetConvertAcceptButton);
    SetObjectExtra (cfp->subtarget [j], cfp, NULL);
    InitEnumPopup (cfp->subtarget [j], cfp->alists [j], NULL);
    SetEnumPopup (cfp->subtarget [j], cfp->alists [j], 0);
    Hide (cfp->subtarget [j]);
  }
  cfp->subtarget [8] = (PopuP) SingleList (x, 10, 8, (LstActnProc) SetConvertAcceptButton);
  SetObjectExtra (cfp->subtarget [8], cfp, NULL);
  for (ap = cfp->alists [8]; ap->name != NULL; ap++) {
    ListItem ((LisT) cfp->subtarget [8], ap->name);
  }
  Hide (cfp->subtarget [8]);

  c = HiddenGroup (h, 4, 0, NULL);
  cfp->accept = PushButton (c, "Accept", DoConvertProc);
  SetObjectExtra (cfp->accept, cfp, NULL);
  Disable (cfp->accept);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) p, (HANDLE) c, (HANDLE) g, NULL);
  RealizeWindow (w);
  Show (w);
  Select (w);
  Select (cfp->atleft);
  Update ();
}

extern void RemoveTextInsideString (IteM i)

{
  BaseFormPtr        bfp;
  GrouP              c;
  ConvertFormPtr     cfp;
  GrouP              g;
  GrouP              h;
  Int2               j;
  GrouP              p;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
  WindoW             w;
  GrouP              x;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  cfp = (ConvertFormPtr) MemNew (sizeof (ConvertFormData));
  if (cfp == NULL) return;
  w = FixedWindow (-50, -33, -10, -10, "General Text Excision", StdCloseWindowProc);
  SetObjectExtra (w, cfp, StdCleanupFormProc);
  cfp->form = (ForM) w;
  cfp->formmessage = ConvertMessageProc;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    cfp->appmessage = sepp->handleMessages;
  }

  cfp->input_entityID = bfp->input_entityID;
  cfp->input_itemID = bfp->input_itemID;
  cfp->input_itemtype = bfp->input_itemtype;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  g = HiddenGroup (h, 3, 0, NULL);
  StaticPrompt (g, "Remove text between", 0, dialogTextHeight, programFont, 'l');
  cfp->leftbehav = HiddenGroup (g, 2, 0, NULL);
  RadioButton (cfp->leftbehav, "just after");
  RadioButton (cfp->leftbehav, "starting at");
  SetValue (cfp->leftbehav, 1);
  cfp->atleft = DialogText (g, "", 10, (TxtActnProc) SetConvertAcceptButton);
  SetObjectExtra (cfp->atleft, cfp, NULL);
  StaticPrompt (g, "and", 0, dialogTextHeight, programFont, 'l');
  cfp->rightbehav = HiddenGroup (g, 2, 0, NULL);
  RadioButton (cfp->rightbehav, "up to");
  RadioButton (cfp->rightbehav, "including");
  SetValue (cfp->rightbehav, 1);
  cfp->atright = DialogText (g, "", 10, (TxtActnProc) SetConvertAcceptButton);
  SetObjectExtra (cfp->atright, cfp, NULL);

  p = HiddenGroup (h, 6, 0, NULL);
  StaticPrompt (p, "Perform excision in", 0, popupMenuHeight, programFont, 'l');
  cfp->target = PopupList (p, TRUE, ChangeSubtarget);
  SetObjectExtra (cfp->target, cfp, NULL);
  InitEnumPopup (cfp->target, target_field_alist, NULL);
  SetEnumPopup (cfp->target, target_field_alist, 0);

  cfp->alists [1] = gene_field_alist;
  cfp->alists [2] = cds_field_alist;
  cfp->alists [3] = prot_field_alist;
  cfp->alists [4] = rnax_field_alist;
  cfp->alists [5] = orgref_field_alist;
  cfp->alists [6] = orgmod_note_subtype_alist;
  cfp->alists [7] = subsource_note_subtype_alist;

  cfp->replaceOldAsked = FALSE;
  cfp->doReplaceAll = FALSE;

  x = HiddenGroup (p, 0, 0, NULL);

  for (j = 1; j <= 7; j++) {
    cfp->subtarget [j] = PopupList (x, TRUE, (PupActnProc) SetConvertAcceptButton);
    SetObjectExtra (cfp->subtarget [j], cfp, NULL);
    InitEnumPopup (cfp->subtarget [j], cfp->alists [j], NULL);
    SetEnumPopup (cfp->subtarget [j], cfp->alists [j], 0);
    Hide (cfp->subtarget [j]);
  }

  c = HiddenGroup (h, 4, 0, NULL);
  cfp->accept = PushButton (c, "Accept", DoRemoveTextProc);
  SetObjectExtra (cfp->accept, cfp, NULL);
  Disable (cfp->accept);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) p, (HANDLE) c, (HANDLE) g, NULL);
  RealizeWindow (w);
  Show (w);
  Select (w);
  Select (cfp->atleft);
  Update ();
}

/* AAForCodon is extern in seqport.c */
/* NLM_EXTERN Uint1 AAForCodon (Uint1Ptr codon, CharPtr codes); */

/*
static Boolean CorrectStartCodonCallback (GatherContextPtr gcp)

{
  Uint1           aa;
  Boolean         bad_base;
  CodeBreakPtr    cbp;
  Uint1           codon [3];
  CharPtr         codes;
  CdRegionPtr     crp;
  GeneticCodePtr  gc;
  Int2            i;
  Uint1           residue;
  SeqEntryPtr     sep;
  SeqFeatPtr      sfp;
  SeqLocPtr       slp;
  SeqPntPtr       spntp;
  SeqPortPtr      spp;
  ValNodePtr      vnp;

  if (gcp == NULL) return TRUE;
  sep = (SeqEntryPtr) gcp->userdata;
  if (sep == NULL ) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  if (sfp == NULL || sfp->data.choice != SEQFEAT_CDREGION) return TRUE;
  crp = (CdRegionPtr) sfp->data.value.ptrvalue;
  if (crp == NULL) return TRUE;

  if (crp->code_break != NULL) return TRUE;

	gc = NULL;
	if (crp->genetic_code != NULL)
	{
		vnp = (ValNodePtr)(crp->genetic_code->data.ptrvalue);
		while ((vnp != NULL) && (gcp == NULL))
		{
			switch (vnp->choice)
			{
			case 1:
				gc = GeneticCodeFind(0, (CharPtr)vnp->data.ptrvalue);
				break;
			case 2:
				gc = GeneticCodeFind(vnp->data.intvalue, NULL);
				break;
			case 3:
			case 6:
			case 4:
			case 5:
			case 7:
			case 8:
			default:
				break;
			}
			vnp = vnp->next;
		}
	}
	if (gc == NULL)
		gc = GeneticCodeFind(1, NULL);
	if (gc == NULL) return TRUE;

	codes = NULL;
	for (vnp = (ValNodePtr)gc->data.ptrvalue; vnp != NULL; vnp = vnp->next)
	{
		if (vnp->choice == 3)
			codes = (CharPtr)vnp->data.ptrvalue;
	}
	if (codes == NULL) return TRUE;

  if (crp->frame == 2 || crp->frame == 3) return TRUE;

  spp = SeqPortNewByLoc (sfp->location, Seq_code_ncbi4na);
  if (spp == NULL) return TRUE;
  bad_base = FALSE;
  for (i = 0; i < 3; i++) {
    residue = SeqPortGetResidue (spp);
    if (residue == SEQPORT_EOF)
      break;
    if (residue == INVALID_RESIDUE)
      bad_base = TRUE;
    codon[i] = residue;
  }
  SeqPortFree (spp);
  if (i != 3 || bad_base) return TRUE;
  aa = AAForCodon (codon, codes);

  spp = SeqPortNewByLoc (sfp->product, Seq_code_ncbieaa);
  if (spp == NULL) return TRUE;
  residue = SeqPortGetResidue (spp);
  SeqPortFree (spp);
  if (residue == SEQPORT_EOF || residue == INVALID_RESIDUE) return TRUE;

  if (residue != aa) {
    cbp = CodeBreakNew ();
    if (cbp != NULL) {
      spntp = SeqPntNew ();
      slp = ValNodeNew (NULL);
      slp->choice = SEQLOC_PNT;
      slp->data.ptrvalue = (Pointer) spntp;
      spntp->point = (Int4) 0;
      spntp->id = SeqIdStripLocus (SeqIdDup (SeqLocId (sfp->product)));
      cbp->loc = slp;
      slp = aaLoc_to_dnaLoc (sfp, cbp->loc);
      cbp->loc = SeqLocFree (cbp->loc);
      cbp->loc = slp;
      cbp->aa.value.intvalue = aa;
      cbp->aa.choice = 1;
      cbp->next = crp->code_break;
      crp->code_break = cbp;
    }
  }

  return TRUE;
}

static void CorrectStartCodon (SeqEntryPtr sep, Uint2 entityID)

{
  BioseqSetPtr  bssp;
  GatherScope   gs;

  if (sep == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 7 || bssp->_class == 13 ||
                         bssp->_class == 14 || bssp->_class == 15)) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        CorrectStartCodon (sep, entityID);
      }
      return;
    }
  }
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  gs.scope = sep;
  GatherEntity (entityID, (Pointer) sep, CorrectStartCodonCallback, &gs);
}

extern void CorrectCDSStartCodon (IteM i);
extern void CorrectCDSStartCodon (IteM i)

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
  CorrectStartCodon (sep, bfp->input_entityID);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}
*/

static void CountComponentFunc (SeqEntryPtr sep, Int2Ptr cp)

{
  BioseqSetPtr  bssp;

  if (sep == NULL || cp == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 7 ||
                         (bssp->_class >= 13 && bssp->_class <= 16))) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        CountComponentFunc (sep, cp);
      }
      return;
    }
  }
  (*cp)++;
}

extern Int2 CountSeqEntryComponents (SeqEntryPtr sep)

{
  Int2  count;

  count = 0;
  if (sep != NULL) {
    CountComponentFunc (sep, &count);
  }
  return count;
}

static ENUM_ALIST(rnax_subtype_alist)
  {" ",            7},
  {"unknown",      0},
  {"preRna",       1},
  {"mRNA",         2},
  {"tRNA",         3},
  {"rRNA",         4},
  {"snRNA",        5},
  {"scRNA",        6},
  {"Other RNA",  255},
END_ENUM_ALIST

typedef struct applyformdata {
  FEATURE_FORM_BLOCK

  Int2           type;
  Int2           errcount;
  Boolean        noLeft;
  Boolean        noRight;
  ButtoN         applyToParts;
  ButtoN         partial5;
  ButtoN         partial3;
  TexT           onlyThisPart;
  TexT           geneName;
  TexT           protName;
  TexT           rnaName;
  TexT           featcomment;
  TexT           defline;
  LisT           key;
  PopuP          rnaSubType;
  Uint1          rnatype;
  EnumFieldAssoc PNTR realalist;
  EnumFieldAssoc PNTR alist;
} ApplyFormData, PNTR ApplyFormPtr;

typedef struct alreadyhas {
  Boolean        rsult;
  Uint1          featchoice;
  Uint1          descchoice;
  Uint1          rnatype;
} AlreadyHas, PNTR AlreadyHasPtr;

static Boolean SeeIfAlreadyHasGatherFunc (GatherContextPtr gcp)

{
  AlreadyHasPtr  ahp;
  RnaRefPtr      rrp;
  ValNodePtr     sdp;
  SeqFeatPtr     sfp;

  if (gcp == NULL) return TRUE;

  ahp = (AlreadyHasPtr) gcp->userdata;
  if (ahp == NULL ) return TRUE;

  if (gcp->thistype == OBJ_SEQFEAT && ahp->featchoice != 0) {
    sfp = (SeqFeatPtr) gcp->thisitem;
    if (sfp != NULL && sfp->data.choice == ahp->featchoice && sfp->data.value.ptrvalue != NULL) {
      if (sfp->data.choice == SEQFEAT_RNA) {
        rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
        if (rrp->type != ahp->rnatype) return TRUE;
      }
      ahp->rsult = TRUE;
      return FALSE;
    }
  } else if (gcp->thistype == OBJ_SEQDESC && ahp->descchoice != 0) {
    sdp = (ValNodePtr) gcp->thisitem;
    if (sdp != NULL && sdp->choice == ahp->descchoice && sdp->data.ptrvalue != NULL) {
      ahp->rsult = TRUE;
      return FALSE;
    }
  }
  return TRUE;
}

static Boolean AlreadyHasFeatOrDesc (SeqEntryPtr sep, Uint1 featchoice, Uint1 descchoice, Uint1 rnatype)

{
  AlreadyHas   ah;
  BioseqPtr    bsp;
  GatherScope  gs;
  SeqEntryPtr  nsep;
  SeqIdPtr     sip;
  SeqLocPtr    slp;

  ah.rsult = FALSE;
  ah.featchoice = featchoice;
  ah.descchoice = descchoice;
  ah.rnatype = rnatype;
  if (sep == NULL) return FALSE;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = TRUE;
  MemSet ((Pointer) (gs.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQDESC] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  gs.scope = sep;
  if (descchoice != 0) {
    nsep = FindNucSeqEntry (sep);
    if (nsep != NULL && IS_Bioseq (nsep)) {
      bsp = (BioseqPtr) nsep->data.ptrvalue;
      if (bsp != NULL) {
        slp = ValNodeNew (NULL);
        slp->choice = SEQLOC_WHOLE;
        sip = SeqIdStripLocus (SeqIdDup (SeqIdFindBest (bsp->id, 0)));
        slp->data.ptrvalue = sip;
        gs.target = slp;
      }
    }
  }
  GatherSeqEntry (sep, (Pointer) (&ah), SeeIfAlreadyHasGatherFunc, &gs);
  gs.target = SeqLocFree (gs.target);
  return ah.rsult;
}

static void RealApplyBioFeatToAll (Uint2 entityID, SeqEntryPtr sep, SeqEntryPtr nsep,
                                   ApplyFormPtr afp, Boolean suppressDups)

{
  EnumFieldAssocPtr  ap;
  ByteStorePtr       bs;
  BioseqPtr          bsp;
  BioseqSetPtr       bssp;
  Char               ch;
  CdRegionPtr        crp;
  ValNodePtr         descr;
  Uint1              frame;
  Int2               genCode;
  GeneRefPtr         grp;
  Int2               i;
  ImpFeatPtr         ifp;
  Int4               len;
  Int4               lens [4];
  Int4               max;
  MolInfoPtr         mip;
  SeqEntryPtr        old;
  CharPtr            prot;
  ProtRefPtr         prp;
  SeqEntryPtr        psep;
  CharPtr            ptr;
  RnaRefPtr          rrp;
  ValNodePtr         sdp;
  SeqFeatPtr         sfp;
  Char               str [128];
  Int2               val;
  ValNodePtr         vnp;

  if (sep == NULL || nsep == NULL || afp == NULL) return;
  if (afp->type == ADD_TITLE) {
    if (suppressDups && entityID > 0 && AlreadyHasFeatOrDesc (sep, 0, Seq_descr_title, 0)) return;
    if (entityID == 0 && SeqEntryGetTitle (sep) != NULL) return;
    if (! TextHasNoText (afp->defline)) {
      if (IS_Bioseq_set (sep)) {
        bssp = (BioseqSetPtr) sep->data.ptrvalue;
        if (bssp != NULL && bssp->_class == 1 && bssp->seq_set != NULL) {
          sep = bssp->seq_set;
        }
      }
      sdp = CreateNewDescriptor (sep, Seq_descr_title);
      if (sdp != NULL) {
        sdp->data.ptrvalue = (Pointer) SaveStringFromTextAndStripNewlines (afp->defline);
      }
    }
    return;
  } else if (afp->type == ADD_CDS) {
    if (suppressDups && entityID > 0 && AlreadyHasFeatOrDesc (sep, SEQFEAT_CDREGION, 0, 0)) return;
    genCode = SeqEntryToGeneticCode (sep, NULL, NULL, 0);
    crp = CreateNewCdRgn (1, FALSE, genCode);
    if (crp != NULL) {
      sfp = CreateNewFeature (nsep, NULL, SEQFEAT_CDREGION, NULL);
      if (sfp != NULL) {
        sfp->data.value.ptrvalue = (Pointer) crp;
        SetSeqLocPartial (sfp->location, afp->noLeft, afp->noRight);
        sfp->partial = (afp->noLeft || afp->noRight);
        if (! TextHasNoText (afp->featcomment)) {
          sfp->comment = SaveStringFromTextAndStripNewlines (afp->featcomment);
        }
        max = 0;
        frame = 0;
        for (i = 1; i <= 3; i++) {
          crp->frame = (Uint1) i;
          bs = ProteinFromCdRegionEx (sfp, FALSE, FALSE);
          len = BSLen (bs);
          BSFree (bs);
          lens [i] = len;
          if (len > max) {
            max = len;
            frame = (Uint1) i;
          }
        }
        for (i = 1; i <= 3; i++) {
          if (lens [i] == max && i != frame) {
            (afp->errcount)++;
          }
        }
        crp->frame = frame;
        bs = ProteinFromCdRegionEx (sfp, TRUE, FALSE);
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
            bsp->id = MakeNewProteinSeqId (sfp->location, NULL);
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
                if (afp->noLeft && afp->noRight) {
                  mip->completeness = 5;
                } else if (afp->noLeft) {
                  mip->completeness = 3;
                } else if (afp->noRight) {
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
              SetSeqFeatProduct (sfp, bsp);
              GetTitle (afp->protName, str, sizeof (str));
              if (! StringHasNoText (str)) {
                prp = CreateNewProtRef (str, NULL, NULL, NULL);
                if (prp != NULL) {
                  sfp = CreateNewFeature (psep, NULL, SEQFEAT_PROT, NULL);
                  if (sfp != NULL) {
                    sfp->data.value.ptrvalue = (Pointer) prp;
                    SetSeqLocPartial (sfp->location, afp->noLeft, afp->noRight);
                    sfp->partial = (afp->noLeft || afp->noRight);
                  }
                }
              }
            }
          }
        }
      }
    }
    GetTitle (afp->geneName, str, sizeof (str));
    if (! StringHasNoText (str)) {
      if (entityID > 0 && AlreadyHasFeatOrDesc (sep, SEQFEAT_GENE, 0, 0)) return;
      grp = CreateNewGeneRef (str, NULL, NULL, FALSE);
      if (grp != NULL) {
        sfp = CreateNewFeature (nsep, NULL, SEQFEAT_GENE, NULL);
        if (sfp != NULL) {
          sfp->data.value.ptrvalue = (Pointer) grp;
          SetSeqLocPartial (sfp->location, afp->noLeft, afp->noRight);
          sfp->partial = (afp->noLeft || afp->noRight);
        }
      }
    }
  } else if (afp->type == ADD_RRNA) {
    if (suppressDups && entityID > 0 && AlreadyHasFeatOrDesc (sep, SEQFEAT_RNA, 0, afp->rnatype)) return;
    rrp = RnaRefNew ();
    if (rrp != NULL) {
      rrp->type = afp->rnatype;
      if (! TextHasNoText (afp->rnaName)) {
        rrp->ext.choice = 1;
        rrp->ext.value.ptrvalue = SaveStringFromText (afp->rnaName);
      }
      sfp = CreateNewFeature (nsep, NULL, SEQFEAT_RNA, NULL);
      if (sfp != NULL) {
        sfp->data.value.ptrvalue = (Pointer) rrp;
        SetSeqLocPartial (sfp->location, afp->noLeft, afp->noRight);
        sfp->partial = (afp->noLeft || afp->noRight);
        if (! TextHasNoText (afp->featcomment)) {
          sfp->comment = SaveStringFromTextAndStripNewlines (afp->featcomment);
        }
      }
    }
    GetTitle (afp->geneName, str, sizeof (str));
    if (! StringHasNoText (str)) {
      if (entityID > 0 && AlreadyHasFeatOrDesc (sep, SEQFEAT_GENE, 0, 0)) return;
      grp = CreateNewGeneRef (str, NULL, NULL, FALSE);
      if (grp != NULL) {
        sfp = CreateNewFeature (nsep, NULL, SEQFEAT_GENE, NULL);
        if (sfp != NULL) {
          sfp->data.value.ptrvalue = (Pointer) grp;
          SetSeqLocPartial (sfp->location, afp->noLeft, afp->noRight);
          sfp->partial = (afp->noLeft || afp->noRight);
        }
      }
    }
  } else if (afp->type == ADD_IMP) {
    val = GetValue (afp->key);
    if (val > 0) {
      ifp = ImpFeatNew ();
      if (ifp != NULL) {
        if (val > 1) {
          for (i = 1, ap = afp->alist; ap->name != NULL; i++, ap++) {
            if (i == val) {
              ifp->key = StringSave (ap->name);
            }
          }
        } else {
          ifp->key = StringSave ("misc_feature");
        }
        sfp = CreateNewFeature (nsep, NULL, SEQFEAT_IMP, NULL);
        if (sfp != NULL) {
          sfp->data.value.ptrvalue = (Pointer) ifp;
          SetSeqLocPartial (sfp->location, afp->noLeft, afp->noRight);
          sfp->partial = (afp->noLeft || afp->noRight);
          if (! TextHasNoText (afp->featcomment)) {
            sfp->comment = SaveStringFromTextAndStripNewlines (afp->featcomment);
          }
          sfp->qual = DialogToPointer (afp->gbquals);
        }
      }
    }
    GetTitle (afp->geneName, str, sizeof (str));
    if (! StringHasNoText (str)) {
      if (entityID > 0 && AlreadyHasFeatOrDesc (sep, SEQFEAT_GENE, 0, 0)) return;
      grp = CreateNewGeneRef (str, NULL, NULL, FALSE);
      if (grp != NULL) {
        sfp = CreateNewFeature (nsep, NULL, SEQFEAT_GENE, NULL);
        if (sfp != NULL) {
          sfp->data.value.ptrvalue = (Pointer) grp;
          SetSeqLocPartial (sfp->location, afp->noLeft, afp->noRight);
          sfp->partial = (afp->noLeft || afp->noRight);
          if (val == 0 && (! TextHasNoText (afp->featcomment))) {
            sfp->comment = SaveStringFromTextAndStripNewlines (afp->featcomment);
          }
        }
      }
    }
  }
}

static void ApplyBioFeatToRaw (Uint2 entityID, SeqEntryPtr sep, SeqEntryPtr tmp, ApplyFormPtr afp, Int2 onlythis)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Int2          count;
  SeqEntryPtr   nsep;

  if (tmp == NULL || afp == NULL) return;
  if (IS_Bioseq_set (tmp)) {
    bssp = (BioseqSetPtr) tmp->data.ptrvalue;
    if (bssp != NULL) {
      if (onlythis != 0 && bssp->_class == BioseqseqSet_class_parts) {
        for (tmp = bssp->seq_set, count = 1;
             tmp != NULL && count != onlythis;
             tmp = tmp->next, count++) continue;
        if (tmp != NULL) {
          ApplyBioFeatToRaw (entityID, sep, tmp, afp, onlythis);
        }
      } else {
        for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
          ApplyBioFeatToRaw (entityID, sep, tmp, afp, onlythis);
        }
      }
      return;
    }
  }
  nsep = FindNucSeqEntry (tmp);
  if (nsep == NULL) return;
  bsp = (BioseqPtr) nsep->data.ptrvalue;
  if (bsp == NULL) return;
  if (bsp->repr == Seq_repr_raw) {
    RealApplyBioFeatToAll (entityID, nsep, nsep, afp, FALSE);
    /*
    RealApplyBioFeatToAll (entityID, sep, nsep, afp, FALSE);
    */
  }
}

static void ApplyBioFeatToAll (Uint2 entityID, SeqEntryPtr sep, ApplyFormPtr afp)

{
  BioseqSetPtr  bssp;
  SeqEntryPtr   nsep;
  Int2          onlythis;
  Char          str [32];

  if (sep == NULL || afp == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 7 ||
                         (bssp->_class >= 13 && bssp->_class <= 16))) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        ApplyBioFeatToAll (entityID, sep, afp);
      }
      return;
    }
  }
  nsep = FindNucSeqEntry (sep);
  if (nsep == NULL) return;
  if (afp->applyToParts != NULL && GetStatus (afp->applyToParts)) {
    GetTitle (afp->onlyThisPart, str, sizeof (str));
    if (! StrToInt (str, &onlythis)) {
      onlythis = 0;
    }
    ApplyBioFeatToRaw (entityID, sep, sep, afp, onlythis);
  } else {
    RealApplyBioFeatToAll (entityID, sep, nsep, afp, TRUE);
  }
}

Int2 ApplyAnnotationToAll (Int2 type, SeqEntryPtr sep,
                           ButtoN partialLft, ButtoN partialRgt,
                           TexT geneName, TexT protName, TexT rnaName,
                           TexT featcomment, TexT defline)

{
  ApplyFormData  afd;

  MemSet ((Pointer) (&afd), 0, sizeof (ApplyFormData));
  afd.type = type;
  afd.errcount = 0;
  afd.partial5 = partialLft;
  afd.partial3 = partialRgt;
  afd.noLeft = GetStatus (afd.partial5);
  afd.noRight = GetStatus (afd.partial3);
  afd.geneName = geneName;
  afd.protName = protName;
  afd.rnaName = rnaName;
  afd.featcomment = featcomment;
  afd.defline = defline;
  ApplyBioFeatToAll (0, sep, &afd);
  if (afd.type == ADD_CDS) {
    return afd.errcount;
  }
  return 0;
}

static void NowReadyToApplyToAll (ApplyFormPtr afp, DialoG gbquals)

{
  Uint2        parenttype;
  Pointer      parentptr;
  CharPtr      plural;
  SeqEntryPtr  sep;
  SeqEntryPtr  top;
  UIEnum       val;

  if (afp == NULL) return;
  afp->gbquals = gbquals;
  sep = GetTopSeqEntryForEntityID (afp->input_entityID);
  if (sep == NULL) return;
  Hide (afp->form);
  WatchCursor ();
  Update ();
  afp->noLeft = GetStatus (afp->partial5);
  afp->noRight = GetStatus (afp->partial3);
  top = sep;
  if (afp->type == ADD_CDS) {
    GetSeqEntryParent (top, &parentptr, &parenttype);
  }
  afp->rnatype = 0;
  if (afp->rnaSubType != NULL) {
    if (GetEnumPopup (afp->rnaSubType, rnax_subtype_alist, &val)) {
      afp->rnatype = val;
    }
  }
  ApplyBioFeatToAll (afp->input_entityID, sep, afp);
  ArrowCursor ();
  Update ();
  if (afp->errcount > 0 && afp->type == ADD_CDS) {
    if (afp->errcount > 1) {
      plural = "records";
    } else {
      plural = "record";
    }
    Message (MSG_ERROR, "Possible ambiguous frames detected in %d %s",
             (int) afp->errcount, plural);
  }
  if (indexerVersion) {
    if (CountSeqEntryComponents (sep) == 1) {
      switch (afp->type) {
        case ADD_TITLE :
          Message (MSG_OK, "When only one record present, add a title descriptor directly");
          break;
        case ADD_RRNA :
          Message (MSG_OK, "When only one record present, add an rRNA feature directly");
          break;
        case ADD_CDS :
          Message (MSG_OK, "When only one record present, add a CdRgn feature directly");
          break;
        case ADD_IMP :
          Message (MSG_OK, "When only one record present, add the feature directly");
          break;
        default :
          break;
      }
    }
  }
  ObjMgrSetDirtyFlag (afp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, afp->input_entityID, 0, 0);
  Remove (afp->form);
}

typedef struct qualsform {
  FEATURE_FORM_BLOCK

  ApplyFormPtr  afp;
} QualsForm, PNTR QualsFormPtr;

static void CallNowReady (ButtoN b)

{
  QualsFormPtr  qfp;

  qfp = (QualsFormPtr) GetObjectExtra (b);
  if (qfp == NULL) return;
  Hide (qfp->form);
  NowReadyToApplyToAll (qfp->afp, qfp->gbquals);
  Remove (qfp->form);
}

static void DoTheApplyToAllProc (ButtoN b)

{
  ApplyFormPtr       afp;
  EnumFieldAssocPtr  ap;
  Int2               i;
  CharPtr            name;
  QualsFormPtr       qfp;
  Int2               val;
  WindoW             w;

  afp = GetObjectExtra (b);
  if (afp == NULL) {
    Remove (ParentWindow (b));
    return;
  }
  if (afp->type == ADD_IMP) {
    val = GetValue (afp->key);
    if (val > 0) {
      qfp = (QualsFormPtr) MemNew (sizeof (QualsForm));
      if (qfp != NULL) {
        Hide (afp->form);
        Update ();
        name = NULL;
        if (val > 1) {
          for (i = 1, ap = afp->alist; ap->name != NULL; i++, ap++) {
            if (i == val) {
              name = ap->name;
            }
          }
        } else {
          name = "misc_feature";
        }
        qfp->afp = afp;
        w = FixedWindow (-50, -33, -10, -10, "Qualifiers", StdCloseWindowProc);
        SetObjectExtra (w, qfp, StdCleanupFormProc);
        qfp->form = (ForM) w;
        qfp->gbquals = CreateImportFields (w, name, NULL, FALSE);
        b = PushButton (w, "Okay", CallNowReady);
        SetObjectExtra (b, qfp, NULL);
        AlignObjects (ALIGN_CENTER, (HANDLE) qfp->gbquals, (HANDLE) b, NULL);
        RealizeWindow (w);
        Show (w);
        Select (w);
      }
    } else {
      NowReadyToApplyToAll (afp, NULL);
    }
  } else {
    NowReadyToApplyToAll (afp, NULL);
  }
}

static void ApplyToPartsProc (ButtoN b)

{
  ApplyFormPtr  afp;

  afp = (ApplyFormPtr) GetObjectExtra (b);
  if (afp == NULL) return;
  if (GetStatus (b)) {
    SafeEnable (afp->onlyThisPart);
  } else {
    SafeDisable (afp->onlyThisPart);
  }
}

static void LookForParts (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqSetPtr  bssp;
  BoolPtr       rsult;

  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && bssp->_class == BioseqseqSet_class_parts) {
      rsult = (BoolPtr) mydata;
      if (rsult != NULL) {
        *rsult = TRUE;
      }
    }
  }
}

extern Boolean HasPartsSet (SeqEntryPtr sep);
extern Boolean HasPartsSet (SeqEntryPtr sep)

{
  Boolean  rsult = FALSE;

  SeqEntryExplore (sep, (Pointer) (&rsult), LookForParts);
  return rsult;
}

static void ApplyMessageProc (ForM f, Int2 mssg)

{
  ApplyFormPtr  afp;

  afp = (ApplyFormPtr) GetObjectExtra (f);
  if (afp != NULL) {
    if (afp->appmessage != NULL) {
      afp->appmessage (f, mssg);
    }
  }
}

static void CleanupApplyToAllForm (GraphiC g, VoidPtr data)

{
  Int2          j;
  ApplyFormPtr  afp;

  afp = (ApplyFormPtr) data;
  if (afp != NULL) {
    if (afp->realalist != NULL) {
      for (j = 0; afp->realalist [j].name != NULL; j++) {
        MemFree (afp->realalist [j].name);
      }
    }
    MemFree (afp->realalist);
  }
  StdCleanupFormProc (g, data);
}

static void CommonApplyToAllProc (IteM i, Int2 type)

{
  ApplyFormPtr       afp;
  EnumFieldAssocPtr  ap;
  BaseFormPtr        bfp;
  ButtoN             b;
  GrouP              c;
  GrouP              g;
  GrouP              h;
  GrouP              p;
  GrouP              q;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
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
  afp = (ApplyFormPtr) MemNew (sizeof (ApplyFormData));
  if (afp == NULL) return;
  w = FixedWindow (-50, -33, -10, -10, "Automatic Processing", StdCloseWindowProc);
  SetObjectExtra (w, afp, CleanupApplyToAllForm);
  afp->form = (ForM) w;
  afp->formmessage = ApplyMessageProc;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    afp->appmessage = sepp->handleMessages;
  }

  afp->input_entityID = bfp->input_entityID;
  afp->input_itemID = bfp->input_itemID;
  afp->input_itemtype = bfp->input_itemtype;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  afp->type = type;
  afp->errcount = 0;

  z = HiddenGroup (h, 1, 0, NULL);
  if (HasPartsSet (sep)) {
    afp->applyToParts = CheckBox (z, "Apply to segmented parts, not segmented sequence", ApplyToPartsProc);
    SetObjectExtra (afp->applyToParts, afp, NULL);
    x = HiddenGroup (z, 2, 0, NULL);
    StaticPrompt (x, "Apply only to particular numbered segment", 0, dialogTextHeight, programFont, 'l');
    afp->onlyThisPart = DialogText (x, "", 4, NULL);
    Disable (afp->onlyThisPart);
  }

  g = HiddenGroup (h, 2, 0, NULL);
  if (type == ADD_CDS || type == ADD_RRNA || type == ADD_IMP) {
    afp->partial5 = CheckBox (g, "Incomplete at 5' end", NULL);
    afp->partial3 = CheckBox (g, "Incomplete at 3' end", NULL);
  }

  if (type == ADD_TITLE) {
    p = HiddenGroup (h, 0, 2, NULL);
    StaticPrompt (p, "Title", 0, 0, programFont, 'c');
    afp->defline = ScrollText (p, 20, 4, programFont, TRUE, NULL);
  } else {
    p = HiddenGroup (h, 2, 0, NULL);
    if (type == ADD_CDS) {
      StaticPrompt (p, "Gene Symbol", 0, dialogTextHeight, programFont, 'l');
      afp->geneName = DialogText (p, "", 20, NULL);
      StaticPrompt (p, "Protein Name", 0, dialogTextHeight, programFont, 'l');
      afp->protName = DialogText (p, "", 20, NULL);
    } else if (type == ADD_RRNA) {
      StaticPrompt (p, "RNA Name", 0, dialogTextHeight, programFont, 'l');
      afp->rnaName = DialogText (p, "", 20, NULL);
      StaticPrompt (p, "Gene Symbol", 0, dialogTextHeight, programFont, 'l');
      afp->geneName = DialogText (p, "", 20, NULL);
    } else if (type == ADD_IMP) {
      ap = import_featdef_alist (FALSE, FALSE, FALSE);
      afp->realalist = ap;
      ap++;
      afp->alist = ap;
      StaticPrompt (p, "Type", 0, 6 * Nlm_stdLineHeight, programFont, 'l');
      afp->key = SingleList (p, 12, 6, NULL);
      for (ap = afp->alist; ap->name != NULL; ap++) {
        ListItem (afp->key, ap->name);
      }
      SetValue (afp->key, 0);
      StaticPrompt (p, "Gene Symbol", 0, dialogTextHeight, programFont, 'l');
      afp->geneName = DialogText (p, "", 20, NULL);
    }
    StaticPrompt (p, "Comment", 0, 4 * Nlm_stdLineHeight, programFont, 'l');
    afp->featcomment = ScrollText (p, 20, 4, programFont, TRUE, NULL);
  }
  afp->gbquals = NULL;

  q = NULL;
  if (type == ADD_RRNA) {
    q = HiddenGroup (h, 2, 0, NULL);
    StaticPrompt (q, "RNA subtype", 0, dialogTextHeight, programFont, 'l');
    afp->rnaSubType = PopupList (q, TRUE, NULL);
    InitEnumPopup (afp->rnaSubType, rnax_subtype_alist, NULL);
    SetEnumPopup (afp->rnaSubType, rnax_subtype_alist, (UIEnum) 4);
  }

  c = HiddenGroup (h, 4, 0, NULL);
  b = PushButton (c, "Accept", DoTheApplyToAllProc);
  SetObjectExtra (b, afp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) p, (HANDLE) c, (HANDLE) z, (HANDLE) q, NULL);
  RealizeWindow (w);
  Show (w);
  if (type == ADD_TITLE) {
    Select (afp->defline);
  } else if (type == ADD_RRNA) {
    Select (afp->rnaName);
  } else {
    Select (afp->geneName);
  }
  Update ();
}

extern void ApplyTitle (IteM i)

{
  CommonApplyToAllProc (i, ADD_TITLE);
}

extern void ApplyCDS (IteM i)

{
  CommonApplyToAllProc (i, ADD_CDS);
}

extern void ApplyRRNA (IteM i)

{
  CommonApplyToAllProc (i, ADD_RRNA);
}

extern void ApplyImpFeat (IteM i)

{
  CommonApplyToAllProc (i, ADD_IMP);
}

#define SUBMISSION_PAGE   0
#define CONTACT_PAGE      1
#define AUTHOR_PAGE       2
#define AFFILIATION_PAGE  3

typedef struct submitform {
  FORM_MESSAGE_BLOCK
  GrouP           pages [4];
  Int2            currentPage;
  DialoG          tbs;

  GrouP           hup;
  GrouP           dateGrp;

  TexT            title;
  DialoG          reldate;
  TexT            firstname;
  TexT            middleinit;
  TexT            lastname;
  TexT            suffix;
  DialoG          phonefaxemail;
  DialoG          authors;
  DialoG          affil;

  Boolean         visitedContact;
  Boolean         visitedAuthor;

  ButtoN          nextBtn;
  ButtoN          prevBtn;
  BtnActnProc     goToNext;
  BtnActnProc     goToPrev;
} SubmitForm, PNTR SubmitFormPtr;

static void SequinBlockPtrToSubmitForm (ForM f, Pointer data)

{
  AuthorPtr       ap;
  DatePtr         dp;
  NameStdPtr      nsp;
  PersonIdPtr     pid;
  SubmitFormPtr   sbfp;
  SequinBlockPtr  sbp;
  CharPtr         str;
  CharPtr         txt;

  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  sbp = (SequinBlockPtr) data;
  if (sbfp != NULL) {
    if (sbp != NULL) {
      SafeSetTitle (sbfp->title, sbp->citsubtitle);
      PointerToDialog (sbfp->reldate, (Pointer) sbp->releasedate);
      ap = sbp->contactperson;
      if (ap != NULL) {
        pid = ap->name;
        if (pid != NULL && pid->choice == 2) {
          nsp = pid->data;
          if (nsp != NULL) {
            str = NameStdPtrToAuthorSpreadsheetString (nsp);
            if (str != NULL) {
              txt = ExtractTagListColumn (str, 0);
              SafeSetTitle (sbfp->firstname, txt);
              MemFree (txt);
              txt = ExtractTagListColumn (str, 1);
              SafeSetTitle (sbfp->middleinit, txt);
              MemFree (txt);
              txt = ExtractTagListColumn (str, 2);
              SafeSetTitle (sbfp->lastname, txt);
              MemFree (txt);
              txt = ExtractTagListColumn (str, 3);
              SafeSetTitle (sbfp->suffix, txt);
              MemFree (txt);
              MemFree (str);
            }
          }
        }
        PointerToDialog (sbfp->phonefaxemail, (Pointer) ap->affil);
      }
      PointerToDialog (sbfp->authors, (Pointer) sbp->citsubauthors);
      PointerToDialog (sbfp->affil, (Pointer) sbp->citsubaffil);
      if (sbp->holduntilpublished) {
        SafeSetValue (sbfp->hup, 2);
        SafeShow (sbfp->dateGrp);
      } else {
        SafeSetValue (sbfp->hup, 1);
        SafeHide (sbfp->dateGrp);
      }
      sbfp->visitedAuthor = TRUE;
      SetValue (sbfp->tbs, 0);
    } else {
      SafeSetTitle (sbfp->title, NULL);
      dp = DateCurr ();
      if (dp != NULL) {
        dp->data [3] = 0; /* force to end of month */
        dp = DateAdvance (dp, 12);
        /*
        (dp->data [1])++;
        if (dp->data [2] == 2 && dp->data [3] > 28) {
          dp->data [3] = 28;
        }
        */
        PointerToDialog (sbfp->reldate, (Pointer) dp);
      } else {
        PointerToDialog (sbfp->reldate, NULL);
      }
      DateFree (dp);
      SafeSetTitle (sbfp->firstname, "");
      SafeSetTitle (sbfp->middleinit, "");
      SafeSetTitle (sbfp->lastname, "");
      SafeSetTitle (sbfp->suffix, "");
      PointerToDialog (sbfp->phonefaxemail, NULL);
      PointerToDialog (sbfp->authors, NULL);
      PointerToDialog (sbfp->affil, NULL);
      SafeSetValue (sbfp->hup, 1);
      SafeHide (sbfp->dateGrp);
      SetValue (sbfp->tbs, 0);
    }
  }
}

static Pointer SubmitFormToSequinBlockPtr (ForM f)

{
  AffilPtr        affil;
  AuthorPtr       ap;
  NameStdPtr      nsp;
  PersonIdPtr     pid;
  SubmitFormPtr   sbfp;
  SequinBlockPtr  sbp;
  Char            str [128];
  CharPtr         txt;

  sbp = NULL;
  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    sbp = (SequinBlockPtr) MemNew (sizeof (SequinBlock));
    if (sbp != NULL) {
      sbp->citsubtitle = SaveStringFromTextAndStripNewlines (sbfp->title);
      ap = AuthorNew ();
      if (ap != NULL) {
        pid = PersonIdNew ();
        ap->name = pid;
        if (pid != NULL) {
          pid->choice = 2;
          str [0] = '\0';
          txt = SaveStringFromText (sbfp->firstname);
          StringCat (str, txt);
          StringCat (str, "\t");
          MemFree (txt);
          txt = SaveStringFromText (sbfp->middleinit);
          StringCat (str, txt);
          StringCat (str, "\t");
          MemFree (txt);
          txt = SaveStringFromText (sbfp->lastname);
          StringCat (str, txt);
          StringCat (str, "\t");
          MemFree (txt);
          txt = SaveStringFromText (sbfp->suffix);
          StringCat (str, txt);
          StringCat (str, "\n");
          MemFree (txt);
          txt = StringSave (str);
          nsp = AuthorSpreadsheetStringToNameStdPtr (txt);
          MemFree (txt);
          pid->data = nsp;
          if (nsp != NULL) {
            if (StringHasNoText (nsp->names [0])) {
              ap = AuthorFree (ap);
            }
          }
        }
        affil = (AffilPtr) DialogToPointer (sbfp->phonefaxemail);
        if (affil != NULL) {
          if (affil->choice == 2) {
            affil->affil = MemFree (affil->affil);
            affil->div = MemFree (affil->div);
            affil->city = MemFree (affil->city);
            affil->sub = MemFree (affil->sub);
            affil->country = MemFree (affil->country);
            affil->street = MemFree (affil->street);
            affil->postal_code = MemFree (affil->postal_code);
            if (affil->phone == NULL && affil->fax == NULL &&
                affil->email == NULL) {
              affil = AffilFree (affil);
            }
          } else {
            affil = AffilFree (affil);
          }
        }
        ap->affil = affil;
      }
      sbp->contactperson = ap;
      sbp->citsubauthors = (AuthListPtr) DialogToPointer (sbfp->authors);
      sbp->citsubaffil = (AffilPtr) DialogToPointer (sbfp->affil);
      if (GetValue (sbfp->hup) == 2) {
        sbp->holduntilpublished = TRUE;
        sbp->releasedate = (DatePtr) DialogToPointer (sbfp->reldate);
      }
      if (sbp->contactperson == NULL &&
          sbp->citsubauthors == NULL &&
          sbp->citsubaffil == NULL) {
        sbp = SequinBlockFree (sbp);
      }
    }
  }
  return (Pointer) sbp;
}

static void SubmitBlockPtrToSubmitForm (ForM f, Pointer data)

{
  AuthorPtr       ap;
  AuthListPtr     authors;
  ContactInfoPtr  cip;
  CitSubPtr       csp;
  DatePtr         dp;
  NameStdPtr      nsp;
  PersonIdPtr     pid;
  SubmitFormPtr   sbfp;
  SubmitBlockPtr  sbp;
  CharPtr         str;
  CharPtr         txt;

  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  sbp = (SubmitBlockPtr) data;
  if (sbfp != NULL) {
    if (sbp != NULL) {
      PointerToDialog (sbfp->reldate, (Pointer) sbp->reldate);
      cip = sbp->contact;
      if (cip != NULL) {
        ap = cip->contact;
        if (ap != NULL) {
          pid = ap->name;
          if (pid != NULL && pid->choice == 2) {
            nsp = pid->data;
            if (nsp != NULL) {
              str = NameStdPtrToAuthorSpreadsheetString (nsp);
              if (str != NULL) {
                txt = ExtractTagListColumn (str, 0);
                SafeSetTitle (sbfp->firstname, txt);
                MemFree (txt);
                txt = ExtractTagListColumn (str, 1);
                SafeSetTitle (sbfp->middleinit, txt);
                MemFree (txt);
                txt = ExtractTagListColumn (str, 2);
                SafeSetTitle (sbfp->lastname, txt);
                MemFree (txt);
                txt = ExtractTagListColumn (str, 3);
                SafeSetTitle (sbfp->suffix, txt);
                MemFree (txt);
                MemFree (str);
              }
            }
          }
          PointerToDialog (sbfp->phonefaxemail, (Pointer) ap->affil);
        }
      }
      csp = sbp->cit;
      if (csp != NULL) {
        authors = csp->authors;
        if (authors != NULL) {
          PointerToDialog (sbfp->authors, (Pointer) authors);
          PointerToDialog (sbfp->affil, (Pointer) authors->affil);
        }
      }
      if (sbp->hup) {
        SafeSetValue (sbfp->hup, 2);
        SafeShow (sbfp->dateGrp);
      } else {
        SafeSetValue (sbfp->hup, 1);
        SafeHide (sbfp->dateGrp);
      }
      sbfp->visitedAuthor = TRUE;
      SetValue (sbfp->tbs, 0);
    } else {
      SafeSetTitle (sbfp->title, NULL);
      dp = DateCurr ();
      if (dp != NULL) {
        dp->data [3] = 0; /* force to end of month */
        dp = DateAdvance (dp, 12);
        /*
        (dp->data [1])++;
        if (dp->data [2] == 2 && dp->data [3] > 28) {
          dp->data [3] = 28;
        }
        */
        PointerToDialog (sbfp->reldate, (Pointer) dp);
      } else {
        PointerToDialog (sbfp->reldate, NULL);
      }
      DateFree (dp);
      SafeSetTitle (sbfp->firstname, "");
      SafeSetTitle (sbfp->middleinit, "");
      SafeSetTitle (sbfp->lastname, "");
      SafeSetTitle (sbfp->suffix, "");
      PointerToDialog (sbfp->phonefaxemail, NULL);
      PointerToDialog (sbfp->authors, NULL);
      PointerToDialog (sbfp->affil, NULL);
      SafeSetValue (sbfp->hup, 1);
      SafeHide (sbfp->dateGrp);
      SetValue (sbfp->tbs, 0);
    }
  }
}

static ValNodePtr TestSubmitForm (ForM f)

{
  AffilPtr       affil;
  AuthListPtr    authors;
  DatePtr        dp;
  ValNodePtr     head;
  SubmitFormPtr  sbfp;

  head = NULL;

  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {

    if (TextHasNoText (sbfp->firstname)) {
      head = AddStringToValNodeChain (head, "first name", 1);
    }
    if (TextHasNoText (sbfp->lastname)) {
      head = AddStringToValNodeChain (head, "last name", 1);
    }
    affil = DialogToPointer (sbfp->phonefaxemail);
    if (affil != NULL) {
      if (StringHasNoText (affil->phone)) {
        head = AddStringToValNodeChain (head, "telephone number", 0);
      }
      if (StringHasNoText (affil->fax)) {
        head = AddStringToValNodeChain (head, "fax number", 0);
      }
      if (StringHasNoText (affil->email)) {
        head = AddStringToValNodeChain (head, "e-mail address", 0);
      }
    } else {
      head = AddStringToValNodeChain (head, "telephone number", 0);
      head = AddStringToValNodeChain (head, "fax number", 0);
      head = AddStringToValNodeChain (head, "e-mail address", 0);
    }
    affil = AffilFree (affil);

    if (GetValue (sbfp->hup) == 2) {
      dp = DialogToPointer (sbfp->reldate);
      if (dp == NULL) {
        head = AddStringToValNodeChain (head, "release date", 1);
      }
      dp = DateFree (dp);
    }
    if (TextHasNoText (sbfp->title)) {
      head = AddStringToValNodeChain (head, "manuscript title", 1);
    }

    authors = DialogToPointer (sbfp->authors);
    if (authors == NULL) {
      head = AddStringToValNodeChain (head, "author names", 1);
    }
    authors = AuthListFree (authors);
    affil = DialogToPointer (sbfp->affil);
    if (affil == NULL) {
      head = AddStringToValNodeChain (head, "affiliation", 1);
    }
    affil = AffilFree (affil);

  }

  return head;
}

/*
static Boolean ReadSubmitBlock (ForM f, CharPtr filename)

{
  AsnIoPtr        aip;
  Char            path [PATH_MAX];
  SubmitFormPtr   sbfp;
  SubmitBlockPtr  sbp;

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    if (path [0] != '\0' || GetInputFileName (path, sizeof (path), "", "TEXT")) {
      aip = AsnIoOpen (path, "r");
      if (aip != NULL) {
        sbp = SubmitBlockAsnRead (aip, NULL);
        AsnIoClose (aip);
        if (sbp != NULL) {
          SubmitBlockPtrToSubmitForm (f, (Pointer) sbp);
          sbp = SubmitBlockFree (sbp);
          Update ();
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}
*/

static Boolean ReadSubmitBlock (ForM f, CharPtr filename)

{
  Pointer         dataptr;
  Uint2           datatype;
  Uint2           entityID;
  SubmitFormPtr   sbfp;
  SubmitBlockPtr  sbp;
  SeqSubmitPtr    ssp;
  Char            path [PATH_MAX];

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    if (path [0] != '\0' || GetInputFileName (path, sizeof (path), "", "TEXT")) {
      dataptr = ObjMgrGenericAsnTextFileRead (path, &datatype, &entityID);
      if (dataptr != NULL && entityID > 0) {
        sbp = NULL;
        switch (datatype) {
          case OBJ_SUBMIT_BLOCK :
            sbp = (SubmitBlockPtr) dataptr;
            break;
          case OBJ_SEQSUB :
            ssp = (SeqSubmitPtr) dataptr;
            if (ssp != NULL) {
              sbp = ssp->sub;
            }
            break;
          default :
            break;
        }
        if (sbp != NULL) {
          SubmitBlockPtrToSubmitForm (f, sbp);
        }
        ObjMgrDelete (datatype, dataptr);
        if (sbp != NULL) {
          Update ();
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

static Boolean ReadContactPage (ForM f, CharPtr filename)

{
  AsnIoPtr        aip;
  AuthorPtr       ap;
  ContactInfoPtr  cip;
  NameStdPtr      nsp;
  Char            path [PATH_MAX];
  PersonIdPtr     pid;
  SubmitFormPtr   sbfp;
  CharPtr         str;
  CharPtr         txt;

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    if (path [0] != '\0' || GetInputFileName (path, sizeof (path), "", "TEXT")) {
      aip = AsnIoOpen (path, "r");
      if (aip != NULL) {
        cip = ContactInfoAsnRead (aip, NULL);
        AsnIoClose (aip);
        if (cip != NULL) {
          ap = cip->contact;
          if (ap != NULL) {
            pid = ap->name;
            if (pid != NULL && pid->choice == 2) {
              nsp = pid->data;
              if (nsp != NULL) {
                str = NameStdPtrToAuthorSpreadsheetString (nsp);
                if (str != NULL) {
                  txt = ExtractTagListColumn (str, 0);
                  SafeSetTitle (sbfp->firstname, txt);
                  MemFree (txt);
                  txt = ExtractTagListColumn (str, 1);
                  SafeSetTitle (sbfp->middleinit, txt);
                  MemFree (txt);
                  txt = ExtractTagListColumn (str, 2);
                  SafeSetTitle (sbfp->lastname, txt);
                  MemFree (txt);
                  txt = ExtractTagListColumn (str, 3);
                  SafeSetTitle (sbfp->suffix, txt);
                  MemFree (txt);
                  MemFree (str);
                }
              }
            }
            PointerToDialog (sbfp->phonefaxemail, (Pointer) ap->affil);
          }
          cip = ContactInfoFree (cip);
          Update ();
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

static Boolean WriteContactPage (ForM f, CharPtr filename)

{
  AffilPtr        affil;
  AsnIoPtr        aip;
  AuthorPtr       ap;
  ContactInfoPtr  cip;
  NameStdPtr      nsp;
  Char            path [PATH_MAX];
  PersonIdPtr     pid;
  SubmitFormPtr   sbfp;
  Char            str [128];
  CharPtr         txt;
#ifdef WIN_MAC
  FILE            *fp;
#endif

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    if (path [0] != '\0' || GetOutputFileName (path, sizeof (path), NULL)) {
#ifdef WIN_MAC
      fp = FileOpen (path, "r");
      if (fp != NULL) {
        FileClose (fp);
      } else {
        FileCreate (path, "TEXT", "ttxt");
      }
#endif
      aip = AsnIoOpen (path, "w");
      if (aip != NULL) {
        cip = ContactInfoNew ();
        if (cip != NULL) {
          ap = AuthorNew ();
          if (ap != NULL) {
            pid = PersonIdNew ();
            ap->name = pid;
            if (pid != NULL) {
              pid->choice = 2;
              str [0] = '\0';
              txt = SaveStringFromText (sbfp->firstname);
              StringCat (str, txt);
              StringCat (str, "\t");
              MemFree (txt);
              txt = SaveStringFromText (sbfp->middleinit);
              StringCat (str, txt);
              StringCat (str, "\t");
              MemFree (txt);
              txt = SaveStringFromText (sbfp->lastname);
              StringCat (str, txt);
              StringCat (str, "\t");
              MemFree (txt);
              txt = SaveStringFromText (sbfp->suffix);
              StringCat (str, txt);
              StringCat (str, "\n");
              MemFree (txt);
              txt = StringSave (str);
              nsp = AuthorSpreadsheetStringToNameStdPtr (txt);
              MemFree (txt);
              pid->data = nsp;
              if (nsp != NULL) {
                if (StringHasNoText (nsp->names [0])) {
                  ap = AuthorFree (ap);
                }
              }
            }
            affil = (AffilPtr) DialogToPointer (sbfp->phonefaxemail);
            if (affil != NULL) {
              if (affil->choice == 2) {
                affil->affil = MemFree (affil->affil);
                affil->div = MemFree (affil->div);
                affil->city = MemFree (affil->city);
                affil->sub = MemFree (affil->sub);
                affil->country = MemFree (affil->country);
                affil->street = MemFree (affil->street);
                affil->postal_code = MemFree (affil->postal_code);
                if (affil->phone == NULL && affil->fax == NULL &&
                    affil->email == NULL) {
                  affil = AffilFree (affil);
                }
              } else {
                affil = AffilFree (affil);
              }
            }
            ap->affil = affil;
          }
          cip->contact = ap;
          ContactInfoAsnWrite (cip, aip, NULL);
          AsnIoClose (aip);
          cip = ContactInfoFree (cip);
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

static Boolean ReadAuthListPage (ForM f, CharPtr filename)

{
  AsnIoPtr       aip;
  AuthListPtr    alp;
  Char           path [PATH_MAX];
  SubmitFormPtr  sbfp;

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    if (path [0] != '\0' || GetInputFileName (path, sizeof (path), "", "TEXT")) {
      aip = AsnIoOpen (path, "r");
      if (aip != NULL) {
        alp = AuthListAsnRead (aip, NULL);
        AsnIoClose (aip);
        if (alp != NULL) {
          PointerToDialog (sbfp->authors, alp);
          alp = AuthListFree (alp);
          Update ();
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

static Boolean WriteAuthListPage (ForM f, CharPtr filename)

{
  AsnIoPtr       aip;
  AuthListPtr    alp;
  Char           path [PATH_MAX];
  SubmitFormPtr  sbfp;
#ifdef WIN_MAC
  FILE           *fp;
#endif

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    if (path [0] != '\0' || GetOutputFileName (path, sizeof (path), NULL)) {
#ifdef WIN_MAC
      fp = FileOpen (path, "r");
      if (fp != NULL) {
        FileClose (fp);
      } else {
        FileCreate (path, "TEXT", "ttxt");
      }
#endif
      aip = AsnIoOpen (path, "w");
      if (aip != NULL) {
        alp = DialogToPointer (sbfp->authors);
        AuthListAsnWrite (alp, aip, NULL);
        AsnIoClose (aip);
        alp = AuthListFree (alp);
        return TRUE;
      }
    }
  }
  return FALSE;
}

static Boolean ReadAffilPage (ForM f, CharPtr filename)

{
  AffilPtr       affil;
  AsnIoPtr       aip;
  Char           path [PATH_MAX];
  SubmitFormPtr  sbfp;

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    if (path [0] != '\0' || GetInputFileName (path, sizeof (path), "", "TEXT")) {
      aip = AsnIoOpen (path, "r");
      if (aip != NULL) {
        affil = AffilAsnRead (aip, NULL);
        AsnIoClose (aip);
        if (affil != NULL) {
          affil->phone = MemFree (affil->phone);
          affil->fax = MemFree (affil->fax);
          affil->email = MemFree (affil->email);
          PointerToDialog (sbfp->affil, affil);
          affil = AffilFree (affil);
          Update ();
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

static Boolean WriteAffilPage (ForM f, CharPtr filename)

{
  AffilPtr       affil;
  AsnIoPtr       aip;
  Char           path [PATH_MAX];
  SubmitFormPtr  sbfp;
#ifdef WIN_MAC
  FILE           *fp;
#endif

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    if (path [0] != '\0' || GetOutputFileName (path, sizeof (path), NULL)) {
#ifdef WIN_MAC
      fp = FileOpen (path, "r");
      if (fp != NULL) {
        FileClose (fp);
      } else {
        FileCreate (path, "TEXT", "ttxt");
      }
#endif
      aip = AsnIoOpen (path, "w");
      if (aip != NULL) {
        affil = DialogToPointer (sbfp->affil);
        affil->phone = MemFree (affil->phone);
        affil->fax = MemFree (affil->fax);
        affil->email = MemFree (affil->email);
        AffilAsnWrite (affil, aip, NULL);
        AsnIoClose (aip);
        affil = AffilFree (affil);
        return TRUE;
      }
    }
  }
  return FALSE;
}

static Boolean ImportSubmitForm (ForM f, CharPtr filename)

{
  SubmitFormPtr  sbfp;

  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    switch (sbfp->currentPage) {
      case SUBMISSION_PAGE :
        return ReadSubmitBlock (f, filename);
      case CONTACT_PAGE :
        return ReadContactPage (f, filename);
      case AUTHOR_PAGE :
        return ReadAuthListPage (f, filename);
      case AFFILIATION_PAGE :
        return ReadAffilPage (f, filename);
      default :
        break;
    }
  }
  return FALSE;
}

static Boolean ExportSubmitForm (ForM f, CharPtr filename)

{
  SubmitFormPtr  sbfp;

  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    switch (sbfp->currentPage) {
      case SUBMISSION_PAGE :
        return FALSE;
      case CONTACT_PAGE :
        return WriteContactPage (f, filename);
      case AUTHOR_PAGE :
        return WriteAuthListPage (f, filename);
      case AFFILIATION_PAGE :
        return WriteAffilPage (f, filename);
      default :
        break;
    }
  }
  return FALSE;
}

static void CopyContactToAuthors (SubmitFormPtr sbfp)

{
  AuthListPtr  alp;
  AuthorPtr    ap;
  ValNodePtr   names;
  NameStdPtr   nsp;
  PersonIdPtr  pid;
  Char         str [128];
  CharPtr      txt;

  if (sbfp == NULL) return;
  ap = NULL;
  alp = AuthListNew ();
  if (alp != NULL) {
    alp->choice = 1;
    names = ValNodeNew (NULL);
    alp->choice = 1;
    alp->names = names;
    if (names != NULL) {
      ap = AuthorNew ();
      if (ap != NULL) {
        pid = PersonIdNew ();
        ap->name = pid;
        if (pid != NULL) {
          pid->choice = 2;
          str [0] = '\0';
          txt = SaveStringFromText (sbfp->firstname);
          StringCat (str, txt);
          StringCat (str, "\t");
          MemFree (txt);
          txt = SaveStringFromText (sbfp->middleinit);
          StringCat (str, txt);
          StringCat (str, "\t");
          MemFree (txt);
          txt = SaveStringFromText (sbfp->lastname);
          StringCat (str, txt);
          StringCat (str, "\t");
          MemFree (txt);
          txt = SaveStringFromText (sbfp->suffix);
          StringCat (str, txt);
          StringCat (str, "\n");
          MemFree (txt);
          txt = StringSave (str);
          nsp = AuthorSpreadsheetStringToNameStdPtr (txt);
          MemFree (txt);
          pid->data = nsp;
          if (nsp != NULL) {
            if (StringHasNoText (nsp->names [0])) {
              ap = AuthorFree (ap);
            }
          }
        }
      }
      names->choice = 1;
      names->data.ptrvalue = ap;
    }
    if (ap == NULL) {
      alp = AuthListFree (alp);
    }
    if (alp != NULL) {
       PointerToDialog (sbfp->authors, (Pointer) alp);
    }
  }
  alp = AuthListFree (alp);
}

static void SetSubmitterImportExportItems (SubmitFormPtr sbfp)

{
  IteM  exportItm;
  IteM  importItm;

  if (sbfp != NULL) {
    importItm = FindFormMenuItem ((BaseFormPtr) sbfp, VIB_MSG_IMPORT);
    exportItm = FindFormMenuItem ((BaseFormPtr) sbfp, VIB_MSG_EXPORT);
    switch (sbfp->currentPage) {
      case SUBMISSION_PAGE :
        SafeSetTitle (importItm, "Import Submitter Info...");
        SafeSetTitle (exportItm, "Export...");
        SafeEnable (importItm);
        SafeDisable (exportItm);
        break;
      case CONTACT_PAGE :
        SafeSetTitle (importItm, "Import Contact...");
        SafeSetTitle (exportItm, "Export Contact...");
        SafeEnable (importItm);
        SafeEnable (exportItm);
        break;
      case AUTHOR_PAGE :
        SafeSetTitle (importItm, "Import Authors...");
        SafeSetTitle (exportItm, "Export Authors...");
        SafeEnable (importItm);
        SafeEnable (exportItm);
        break;
      case AFFILIATION_PAGE :
        SafeSetTitle (importItm, "Import Affiliation...");
        SafeSetTitle (exportItm, "Export Affiliation...");
        SafeEnable (importItm);
        SafeEnable (exportItm);
        break;
      default :
        break;
    }
  }
}

static void EnterSubmitPage (SubmitFormPtr sbfp, Int2 page)

{
  AuthListPtr  alp;

  if (sbfp != NULL) {
    switch (page) {
      case SUBMISSION_PAGE :
        Select (sbfp->title);
        SafeSetTitle (sbfp->prevBtn, "<< Prev Form");
        SafeSetTitle (sbfp->nextBtn, "Next Page >>");
        break;
      case CONTACT_PAGE :
        Select (sbfp->firstname);
        sbfp->visitedContact = TRUE;
        SafeSetTitle (sbfp->prevBtn, "<< Prev Page");
        SafeSetTitle (sbfp->nextBtn, "Next Page >>");
        break;
      case AUTHOR_PAGE :
        alp = (AuthListPtr) DialogToPointer (sbfp->authors);
        if (sbfp->visitedContact && alp == NULL) {
          CopyContactToAuthors (sbfp);
        }
        AuthListFree (alp);
        /*
        if (sbfp->visitedContact && (! sbfp->visitedAuthor)) {
          CopyContactToAuthors (sbfp);
        }
        */
        SendMessageToDialog (sbfp->authors, VIB_MSG_ENTER);
        sbfp->visitedAuthor = TRUE;
        SafeSetTitle (sbfp->prevBtn, "<< Prev Page");
        SafeSetTitle (sbfp->nextBtn, "Next Page >>");
        break;
      case AFFILIATION_PAGE :
        SendMessageToDialog (sbfp->affil, VIB_MSG_ENTER);
        SafeSetTitle (sbfp->prevBtn, "<< Prev Page");
        SafeSetTitle (sbfp->nextBtn, "Next Form >>");
        break;
      default :
        break;
    }
  }
}

static void ChangeSubmitFormPage (VoidPtr data, Int2 newval, Int2 oldval)

{
  SubmitFormPtr  sbfp;

  sbfp = (SubmitFormPtr) data;
  if (sbfp != NULL) {
    sbfp->currentPage = newval;
    SafeHide (sbfp->pages [oldval]);
    Update ();
#ifdef WIN_MAC
    EnterSubmitPage (sbfp, newval);
#endif
    SetSubmitterImportExportItems (sbfp);
    SafeShow (sbfp->pages [newval]);
#ifndef WIN_MAC
    EnterSubmitPage (sbfp, newval);
#endif
    Update ();
    switch (newval) {
      case SUBMISSION_PAGE :
        SendHelpScrollMessage (helpForm, "Submitting Authors Form", "Submission Page");
        break;
      case CONTACT_PAGE :
        SendHelpScrollMessage (helpForm, "Submitting Authors Form", "Contact Page");
        break;
      case AUTHOR_PAGE :
        SendHelpScrollMessage (helpForm, "Submitting Authors Form", "Authors Page");
        break;
      case AFFILIATION_PAGE :
        SendHelpScrollMessage (helpForm, "Submitting Authors Form", "Affiliation Page");
        break;
      default :
        break;
    }
  }
}

static void NextSubmitFormBtn (ButtoN b)

{
  SubmitFormPtr  sbfp;

  sbfp = (SubmitFormPtr) GetObjectExtra (b);
  if (sbfp != NULL) {
    if (sbfp->currentPage < 3) {
      SetValue (sbfp->tbs, sbfp->currentPage + 1);
    } else if (sbfp->goToNext != NULL) {
      (sbfp->goToNext) (b);
    }
  }
}

static void PrevSubmitFormBtn (ButtoN b)

{
  SubmitFormPtr  sbfp;

  sbfp = (SubmitFormPtr) GetObjectExtra (b);
  if (sbfp != NULL) {
    if (sbfp->currentPage > 0) {
      SetValue (sbfp->tbs, sbfp->currentPage - 1);
    } else if (sbfp->goToPrev != NULL) {
      (sbfp->goToPrev) (b);
    }
  }
}

static void ChangeHup (GrouP g)

{
  Boolean        hup;
  SubmitFormPtr  sbfp;

  sbfp = (SubmitFormPtr) GetObjectExtra (g);
  if (sbfp != NULL) {
    hup = (Boolean) (GetValue (sbfp->hup) == 2);
    if (hup) {
      SafeShow (sbfp->dateGrp);
    } else {
      SafeHide (sbfp->dateGrp);
    }
  }
}

static CharPtr  submitFormTabs [] = {
  "Submission", "Contact", "Authors", "Affiliation", NULL
};

static void SubmitFormMessage (ForM f, Int2 mssg)

{
  SubmitFormPtr  sbfp;

  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    switch (mssg) {
      case VIB_MSG_IMPORT :
        ImportSubmitForm (f, NULL);
        break;
      case VIB_MSG_EXPORT :
        ExportSubmitForm (f, NULL);
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
        if (sbfp->appmessage != NULL) {
          sbfp->appmessage (f, mssg);
        }
        break;
    }
  }
}

static void InitSubmitterFormActivate (WindoW w)

{
  SubmitFormPtr  sbfp;

  sbfp = (SubmitFormPtr) GetObjectExtra (w);
  if (sbfp != NULL) {
    if (sbfp->activate != NULL) {
      sbfp->activate (w);
    }
    SetSubmitterImportExportItems (sbfp);
  }
}

extern ForM CreateInitSubmitterForm (Int2 left, Int2 top, CharPtr title,
                                     BtnActnProc goToNext,
                                     BtnActnProc goBack,
                                     WndActnProc activateForm)

{
  GrouP              c;
  GrouP              g;
  GrouP              h;
  GrouP              j;
  GrouP              m;
  GrouP              n;
  GrouP              q;
  SubmitFormPtr      sbfp;
  StdEditorProcsPtr  sepp;
  GrouP              t;
  WindoW             w;
  GrouP              x;

  w = NULL;
  sbfp = MemNew (sizeof (SubmitForm));
  if (sbfp != NULL) {
    w = FixedWindow (left, top, -10, -10, title, NULL);
    SetObjectExtra (w, sbfp, StdCleanupFormProc);
    sbfp->form = (ForM) w;
    sbfp->toform = SequinBlockPtrToSubmitForm;
    sbfp->fromform = SubmitFormToSequinBlockPtr;
    sbfp->testform = TestSubmitForm;
    sbfp->importform = ImportSubmitForm;
    sbfp->exportform = ExportSubmitForm;
    sbfp->formmessage = SubmitFormMessage;

#ifndef WIN_MAC
    CreateSqnInitialFormMenus (w);
#endif

    sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
    if (sepp != NULL) {
      sbfp->appmessage = sepp->handleMessages;
    }

    SetGroupSpacing (w, 10, 10);

    j = HiddenGroup (w, -1, 0, NULL);
    SetGroupSpacing (j, 10, 10);

    sbfp->tbs = CreateFolderTabs (j, submitFormTabs, 0, 0, 0,
                                  SYSTEM_FOLDER_TAB,
                                  ChangeSubmitFormPage, (Pointer) sbfp);
    sbfp->currentPage = SUBMISSION_PAGE;

    sbfp->visitedContact = FALSE;
    sbfp->visitedAuthor = FALSE;

    h = HiddenGroup (w, 0, 0, NULL);

    q = HiddenGroup (h, -1, 0, NULL);
    SetGroupSpacing (q, 10, 20);
    m = HiddenGroup (q, -1, 0, NULL);
    g = HiddenGroup (m, 0, -4, NULL);
    SetGroupSpacing (g, 3, 10);
    StaticPrompt (g, "When may we release your sequence record?",
                  0, stdLineHeight, programFont, 'l');
    sbfp->hup = HiddenGroup (g, 0, -2, ChangeHup);
    SetObjectExtra (sbfp->hup, sbfp, NULL);
    RadioButton (sbfp->hup, "Immediately After Processing");
    RadioButton (sbfp->hup, "Release Date:");
    SetValue (sbfp->hup, 1);
    sbfp->dateGrp = HiddenGroup (m, 10, 0, NULL);
    /* StaticPrompt (sbfp->dateGrp, "Release Date: ", 0, popupMenuHeight, programFont, 'l'); */
    sbfp->reldate = CreateDateDialog (sbfp->dateGrp, NULL);
    AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) sbfp->dateGrp, NULL);
    Hide (sbfp->dateGrp);
    t = HiddenGroup (q, 1, 0, NULL);
    StaticPrompt (t, "Tentative title for manuscript", 0, 0, programFont, 'c');
    sbfp->title = ScrollText (t, 25, 4, programFont, TRUE, NULL);
    AlignObjects (ALIGN_CENTER, (HANDLE) sbfp->reldate,
                  (HANDLE) sbfp->title, (HANDLE) t, NULL);
    sbfp->pages [SUBMISSION_PAGE] = q;
    Hide (sbfp->pages [SUBMISSION_PAGE]);

    q = HiddenGroup (h, -1, 0, NULL);
    SetGroupSpacing (q, 10, 20);
    n = HiddenGroup (q, 4, 0, NULL);
    SetGroupSpacing (n, -1, 2);
    StaticPrompt (n, "First Name", 0, 0, programFont, 'c');
    StaticPrompt (n, "M.I.", 0, 0, programFont, 'c');
    StaticPrompt (n, "Last Name", 0, 0, programFont, 'c');
    StaticPrompt (n, "Sfx", 0, 0, programFont, 'c');
    sbfp->firstname = DialogText (n, "", 8, NULL);
    sbfp->middleinit = DialogText (n, "", 4, NULL);
    sbfp->lastname = DialogText (n, "", 9, NULL);
    sbfp->suffix = DialogText (n, "", 3, NULL);
    sbfp->phonefaxemail = CreateExtAffilDialog (q, NULL, NULL, &x);
    Show (x);
    Show (sbfp->phonefaxemail);
    AlignObjects (ALIGN_CENTER, (HANDLE) n, (HANDLE) sbfp->phonefaxemail, NULL);
    sbfp->pages [CONTACT_PAGE] = q;
    Hide (sbfp->pages [CONTACT_PAGE]);

    q = HiddenGroup (h, -1, 0, NULL);
    SetGroupSpacing (q, 10, 20);
    sbfp->authors = CreateAuthorDialog (q, 3, -1);
    sbfp->pages [AUTHOR_PAGE] = q;
    Hide (sbfp->pages [AUTHOR_PAGE]);

    q = HiddenGroup (h, -1, 0, NULL);
    SetGroupSpacing (q, 10, 20);
    sbfp->affil = CreateExtAffilDialog (q, NULL, &g, NULL);
    Show (g);
    Show (sbfp->affil);
    sbfp->pages [AFFILIATION_PAGE] = q;
    Hide (sbfp->pages [AFFILIATION_PAGE]);

    c = HiddenGroup (w, 4, 0, NULL);
    SetGroupSpacing (c, 10, 2);
    sbfp->goToPrev = goBack;
    sbfp->prevBtn = PushButton (c, " << Prev Form ", PrevSubmitFormBtn);
    SetObjectExtra (sbfp->prevBtn, sbfp, NULL);
    sbfp->goToNext = goToNext;
    sbfp->nextBtn = PushButton (c, " Next Page >> ", NextSubmitFormBtn);
    SetObjectExtra (sbfp->nextBtn, sbfp, NULL);

    AlignObjects (ALIGN_CENTER,
                  (HANDLE) sbfp->pages [SUBMISSION_PAGE],
                  (HANDLE) sbfp->pages [CONTACT_PAGE],
                  (HANDLE) sbfp->pages [AUTHOR_PAGE],
                  (HANDLE) sbfp->pages [AFFILIATION_PAGE],
                  (HANDLE) sbfp->tbs, (HANDLE) c, NULL);

    RealizeWindow (w);

    SafeSetTitle (sbfp->prevBtn, "<< Prev Form");
    SafeSetTitle (sbfp->nextBtn, "Next Page >>");

    sbfp->activate = activateForm;
    SetActivate (w, InitSubmitterFormActivate);

    Show (sbfp->pages [sbfp->currentPage]);
    EnterSubmitPage (sbfp, sbfp->currentPage);
  }
  return (ForM) w;
}

#define REMOVE_RNA   1
#define CONVERT_RNA  2
#define ADD_RNA      3
#define EDIT_RNA     4

static ENUM_ALIST(rna_field_alist)
  {" ",                    0},
  {"Name",                 1},
  {"Comment",              2},
  {"Product GBQUAL",       3},
END_ENUM_ALIST

static ENUM_ALIST(rna_subtype_alist)
  {" ",            7},
  {"unknown",      0},
  {"preRna",       1},
  {"mRNA",         2},
  {"tRNA",         3},
  {"rRNA",         4},
  {"snRNA",        5},
  {"scRNA",        6},
  {"Other RNA",  255},
END_ENUM_ALIST

typedef struct rnaformdata {
  FEATURE_FORM_BLOCK

  Int2           type;
  PopuP          fromfield;
  PopuP          tofield;
  Int2           fromval;
  Int2           toval;

  Boolean        replaceOldAsked;
  Boolean        doReplaceAll;

  TexT           findthis;
  Char           findStr [256];
  PopuP          rnaSubType;
  PopuP          rnaDestType;
  GrouP          txtGrp;
  CharPtr        replaceStr;
  TexT           replacewith;
} RnaFormData, PNTR RnaFormPtr;

static Boolean ShouldReplaceRnaString (RnaFormPtr rfp)

{
  MsgAnswer  ans;

  if (rfp == NULL) return FALSE;
  if (! rfp->replaceOldAsked) {
    rfp->replaceOldAsked = TRUE;
    ArrowCursor ();
    ans = Message (MSG_YN, "Do you wish to overwrite existing contents?");
    WatchCursor ();
    Update ();
    rfp->doReplaceAll = (Boolean) (ans == ANS_YES);
  }
  return rfp->doReplaceAll;
}

static CharPtr SaveOrReplaceRnaString (RnaFormPtr rfp, CharPtr str, CharPtr current)

{
  size_t   len;
  CharPtr  tmp;

  if (rfp == NULL) return str;
  if (StringHasNoText (str)) return current;
  if (current != NULL) {
    if (ShouldReplaceRnaString (rfp)) {
      MemFree (current);
    } else {
      len = StringLen (str) + StringLen (current) + 4;
      tmp = MemNew (len);
      if (tmp != NULL) {
        StringCpy (tmp, current);
        StringCat (tmp, "; ");
        StringCat (tmp, str);
        MemFree (str);
        MemFree (current);
        return tmp;
      }
    }
  }
  return str;
}

static void EditRnaString (CharPtr PNTR strptr, RnaFormPtr rfp, CharPtr findStr, CharPtr foundit)

{
  size_t   diff;
  size_t   foundlen;
  size_t   replen;
  CharPtr  newstring;
  CharPtr  tmp;
  CharPtr  tmp2;

  if (rfp == NULL || strptr == NULL || findStr == NULL || foundit == NULL) return;


  foundlen = StringLen (findStr);
  replen = StringLen (rfp->replaceStr);
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
  if (rfp->replaceStr != NULL) {
    tmp2 = MemCopy (tmp2, rfp->replaceStr, replen);
  }
  tmp2 += replen;
  tmp += foundlen;
  tmp2 = StringMove (tmp2, tmp);
  *strptr = MemFree (*strptr);
  *strptr = newstring;

} /* EditRnaString */

static Boolean ProcessEachRNAFunc (GatherContextPtr gcp)

{
  GBQualPtr   gbqual;
  RnaFormPtr  rfp;
  RnaRefPtr   rrp;
  SeqFeatPtr  sfp;
  CharPtr     str, foundit;
  UIEnum      val;

  if (gcp == NULL) return TRUE;
  rfp = (RnaFormPtr) gcp->userdata;
  if (rfp == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  if (sfp == NULL || sfp->data.value.ptrvalue == NULL) return TRUE;
  rrp = NULL;
  if (sfp->data.choice == SEQFEAT_RNA) {
    rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
  } else return TRUE;
  str = NULL;
  if (rfp->rnaSubType != NULL) {
    if (GetEnumPopup (rfp->rnaSubType, rna_subtype_alist, &val)) {
      if (val != 7 && (Uint1) val != rrp->type) {
        rrp = NULL;
      }
    }
  }
  if (rrp == NULL) return TRUE;
  switch (rfp->type) {
    case REMOVE_RNA :
      break;
    case CONVERT_RNA :
      switch (rfp->fromval) {
        case 1 :
          if (rrp->ext.choice == 1 && rrp->ext.value.ptrvalue != NULL) {
            str = StringSave ((CharPtr) rrp->ext.value.ptrvalue);
          }
          break;
        case 2 :
          str = StringSave (sfp->comment);
          break;
        case 3 :
          gbqual = sfp->qual;
          while (gbqual != NULL) {
            if (StringICmp (gbqual->qual, "product") == 0) {
              str = StringSave (gbqual->val);
            }
            gbqual = gbqual->next;
          }
          break;
        default :
          break;
      }
      break;
    case ADD_RNA :
      str = StringSave (rfp->findStr);
      break;
    case EDIT_RNA :
      str = JustSaveStringFromText (rfp->findthis);
      break;
    default :
      break;
  }
  if (str == NULL) return TRUE;
  switch (rfp->type) {
    case REMOVE_RNA :
      break;
    case CONVERT_RNA :
    case ADD_RNA :
      switch (rfp->toval) {
        case 1 :
          if (rrp->ext.choice == 0 || rrp->ext.choice == 1) {
            rrp->ext.choice = 1;
            rrp->ext.value.ptrvalue = SaveOrReplaceRnaString (rfp, str, rrp->ext.value.ptrvalue);
          }
          str = NULL;
          break;
        case 2 :
          sfp->comment = SaveOrReplaceRnaString (rfp, str, sfp->comment);
          str = NULL;
          break;
        case 3 :
          gbqual = GBQualNew ();
          if (gbqual != NULL) {
            gbqual->qual = StringSave ("product");
            gbqual->val = str;
            str = NULL;
            gbqual->next = sfp->qual;
            sfp->qual = gbqual;
          }
          break;
        default :
          break;
      }
      break;
    case EDIT_RNA : 
      rfp->replaceStr = JustSaveStringFromText (rfp->replacewith);
      foundit = StringISearch (rrp->ext.value.ptrvalue, str);
      if (foundit) {
        if (rrp->ext.choice == 0 || rrp->ext.choice == 1) {
          rrp->ext.choice = 1;
          EditRnaString ((CharPtr PNTR) &(rrp->ext.value.ptrvalue), rfp, str, foundit);
        }
      }
      MemFree (str);
      break;
    default :
      break;
  }
  MemFree (str);
  return TRUE;
}

static Boolean CleanupEachRNAFunc (GatherContextPtr gcp)

{
  Uint1           desttype;
  GBQualPtr       gbqual;
  GBQualPtr       nextqual;
  GBQualPtr PNTR  prevqual;
  RnaFormPtr      rfp;
  RnaRefPtr       rrp;
  SeqFeatPtr      sfp;
  UIEnum          val;

  if (gcp == NULL) return TRUE;
  rfp = (RnaFormPtr) gcp->userdata;
  if (rfp == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  if (sfp == NULL || sfp->data.value.ptrvalue == NULL) return TRUE;
  rrp = NULL;
  if (sfp->data.choice == SEQFEAT_RNA) {
    rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
  } else return TRUE;
  if (rfp->rnaSubType != NULL) {
    if (GetEnumPopup (rfp->rnaSubType, rna_subtype_alist, &val)) {
      if (val != 7 && (Uint1) val != rrp->type) {
        rrp = NULL;
      }
    }
  }
  if (rrp == NULL) return TRUE;
  desttype = 7;
  if (rfp->rnaDestType != NULL) {
    if (GetEnumPopup (rfp->rnaDestType, rna_subtype_alist, &val)) {
      if (val != 7 && (Uint1) val != rrp->type) {
        desttype = (Uint1) val;
      }
    }
  }
  switch (rfp->type) {
    case REMOVE_RNA :
    case CONVERT_RNA :
      switch (rfp->fromval) {
        case 1 :
          if (rrp->ext.choice == 1 && rrp->ext.value.ptrvalue != NULL) {
            rrp->ext.value.ptrvalue = MemFree (rrp->ext.value.ptrvalue);
            rrp->ext.choice = 0;
          }
          break;
        case 2 :
          sfp->comment = MemFree (sfp->comment);
          break;
        case 3 :
          gbqual = sfp->qual;
          prevqual = (GBQualPtr PNTR) &(sfp->qual);
          while (gbqual != NULL) {
            nextqual = gbqual->next;
            if (StringICmp (gbqual->qual, "product") == 0) {
              *(prevqual) = gbqual->next;
              gbqual->next = NULL;
              gbqual->qual = MemFree (gbqual->qual);
              gbqual->val = MemFree (gbqual->val);
              GBQualFree (gbqual);
            } else {
              prevqual = (GBQualPtr PNTR) &(gbqual->next);
            }
            gbqual = nextqual;
          }
          break;
        default :
          break;
      }
      if (desttype != 7) {
        rrp->type = desttype;
      }
      break;
    case ADD_RNA :
      break;
    default :
      break;
  }
  return TRUE;
}

static void DoProcessRnaProc (ButtoN b)

{
  GatherScope  gs;
  RnaFormPtr   rfp;
  UIEnum       val;

  rfp = (RnaFormPtr) GetObjectExtra (b);
  if (rfp == NULL || rfp->input_entityID == 0) return;
  Hide (rfp->form);
  WatchCursor ();
  Update ();
  rfp->fromval = 0;
  rfp->toval = 0;
  if (GetEnumPopup (rfp->fromfield, rna_field_alist, &val)) {
    rfp->fromval = (Int2) val;
  }
  if (GetEnumPopup (rfp->tofield, rna_field_alist, &val)) {
    rfp->toval = (Int2) val;
  }
  if (rfp->type == ADD_RNA) {
    rfp->toval = rfp->fromval;
  }
  rfp->replaceOldAsked = FALSE;
  rfp->doReplaceAll = FALSE;
  rfp->findStr [0] = '\0';
  GetTitle (rfp->findthis, rfp->findStr, sizeof (rfp->findStr) - 1);
  TrimSpacesAroundString (rfp->findStr);
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  if (rfp->type != REMOVE_RNA) {
    GatherEntity (rfp->input_entityID, (Pointer) rfp, ProcessEachRNAFunc, &gs);
  }
  if (rfp->type == CONVERT_RNA || rfp->type == REMOVE_RNA) {
    GatherEntity (rfp->input_entityID, (Pointer) rfp, CleanupEachRNAFunc, &gs);
  }
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (rfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, rfp->input_entityID, 0, 0);
  Remove (rfp->form);
}

static void ProcessRNAMessageProc (ForM f, Int2 mssg)

{
  StdEditorProcsPtr  sepp;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    if (sepp->handleMessages != NULL) {
      sepp->handleMessages (f, mssg);
    }
  }
}

static void ProcessRNA (IteM i, Int2 type)

{
  ButtoN             b;
  BaseFormPtr        bfp;
  GrouP              c;
  GrouP              g;
  GrouP              h;
  GrouP              p;
  RnaFormPtr         rfp;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
  CharPtr            title;
  WindoW             w;
  GrouP              y;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  rfp = (RnaFormPtr) MemNew (sizeof (RnaFormData));
  if (rfp == NULL) return;
  rfp->type = type;
  switch (type) {
    case REMOVE_RNA :
      title = "RNA Removal";
      break;
    case CONVERT_RNA :
      title = "RNA Conversion";
      break;
    case ADD_RNA :
      title = "Add RNA Qualifier";
      break;
    case EDIT_RNA :
      title = "Edit RNA Qualifier";
      break;
    default :
      title = "?";
      break;
  }
  w = FixedWindow (-50, -33, -10, -10, title, StdCloseWindowProc);
  SetObjectExtra (w, rfp, StdCleanupFormProc);
  rfp->form = (ForM) w;
  rfp->formmessage = ProcessRNAMessageProc;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    rfp->appmessage = sepp->handleMessages;
  }

  rfp->input_entityID = bfp->input_entityID;
  rfp->input_itemID = bfp->input_itemID;
  rfp->input_itemtype = bfp->input_itemtype;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  g = HiddenGroup (h, 4, 0, NULL);
  switch (type) {
    case REMOVE_RNA :
      StaticPrompt (g, "Remove", 0, popupMenuHeight, programFont, 'l');
      rfp->fromfield = PopupList (g, TRUE, NULL);
      SetObjectExtra (rfp->fromfield, rfp, NULL);
      InitEnumPopup (rfp->fromfield, rna_field_alist, NULL);
      SetEnumPopup (rfp->fromfield, rna_field_alist, 0);
      break;
    case CONVERT_RNA :
      StaticPrompt (g, "From", 0, popupMenuHeight, programFont, 'l');
      rfp->fromfield = PopupList (g, TRUE, NULL);
      SetObjectExtra (rfp->fromfield, rfp, NULL);
      InitEnumPopup (rfp->fromfield, rna_field_alist, NULL);
      SetEnumPopup (rfp->fromfield, rna_field_alist, 0);

      StaticPrompt (g, "To", 0, popupMenuHeight, programFont, 'l');
      rfp->tofield = PopupList (g, TRUE, NULL);
      InitEnumPopup (rfp->tofield, rna_field_alist, NULL);
      SetEnumPopup (rfp->tofield, rna_field_alist, 0);
      break;
    case ADD_RNA :
      StaticPrompt (g, "Type", 0, popupMenuHeight, programFont, 'l');
      rfp->fromfield = PopupList (g, TRUE, NULL);
      SetObjectExtra (rfp->fromfield, rfp, NULL);
      InitEnumPopup (rfp->fromfield, rna_field_alist, NULL);
      SetEnumPopup (rfp->fromfield, rna_field_alist, 0);
      break;
    case EDIT_RNA :
      break;
    default :
      break;
  }

  y = NULL;
  switch (type) {
    case REMOVE_RNA :
      break;
    case CONVERT_RNA :
      break;
    case ADD_RNA :
      y = HiddenGroup (h, 2, 0, NULL);
      StaticPrompt (y, "Text", 0, dialogTextHeight, programFont, 'c');
      rfp->findthis = DialogText (y, "", 14, NULL);
      break;
    case EDIT_RNA :
      break;
    default :
      break;
  }

  p = HiddenGroup (h, 4, 0, NULL);
  StaticPrompt (p, "RNA subtype", 0, popupMenuHeight, programFont, 'l');
  rfp->rnaSubType = PopupList (p, TRUE, NULL);
  InitEnumPopup (rfp->rnaSubType, rna_subtype_alist, NULL);
  SetEnumPopup (rfp->rnaSubType, rna_subtype_alist, (UIEnum) 7);
  rfp->rnaDestType = NULL;
  if (type == CONVERT_RNA) {
    StaticPrompt (p, "Change to", 0, popupMenuHeight, programFont, 'l');
    rfp->rnaDestType = PopupList (p, TRUE, NULL);
    InitEnumPopup (rfp->rnaDestType, rna_subtype_alist, NULL);
    SetEnumPopup (rfp->rnaDestType, rna_subtype_alist, (UIEnum) 7);
  }

  rfp->txtGrp = NULL;
  if (type == EDIT_RNA) {
    rfp->txtGrp = HiddenGroup (h, 2, 0, NULL);
    StaticPrompt (rfp->txtGrp, "Find", 0, dialogTextHeight, programFont, 'l');
    rfp->findthis = DialogText (rfp->txtGrp, "", 14, NULL);
    StaticPrompt (rfp->txtGrp, "Replace", 0, dialogTextHeight, programFont, 'l');
    rfp->replacewith = DialogText (rfp->txtGrp, "", 14, NULL);
  }

  c = HiddenGroup (h, 4, 0, NULL);
  b = PushButton (c, "Accept", DoProcessRnaProc);
  SetObjectExtra (b, rfp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) p, (HANDLE) c, (HANDLE) y, NULL);
  RealizeWindow (w);
  Show (w);
  Update ();
}

extern void RemoveRNA (IteM i)

{
  ProcessRNA (i, REMOVE_RNA);
}

extern void ConvertRNA (IteM i)

{
  ProcessRNA (i, CONVERT_RNA);
}

extern void AddRNA (IteM i)

{
  ProcessRNA (i, ADD_RNA);
}

extern void EditRNA (IteM i)

{
  ProcessRNA (i, EDIT_RNA);
}

typedef struct parseformdata {
  FEATURE_FORM_BLOCK

  TexT           atleft;
  TexT           atright;
  PopuP          orgmod;
  PopuP          subsource;
  PopuP          taxname;
  Boolean        parsedef;
  Char           path [PATH_MAX];
  FILE           *fp;
} ParseFormData, PNTR ParseFormPtr;

static void DoOneParseItem (Uint2 entityID, SeqEntryPtr sep, ParseFormPtr pfp, BioSourcePtr topbiop, CharPtr useme, SeqEntryPtr nps)

{
  BioSourcePtr  biop;
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  OrgModPtr     mod;
  SeqEntryPtr   nsep;
  OrgNamePtr    onp;
  Uint1         orgmodtype;
  OrgRefPtr     orp;
  CharPtr       ptr;
  SeqIdPtr      sip;
  SubSourcePtr  ssp;
  Char          str [256];
  Uint1         subsourcetype;
  Uint1         subtype;
  Int2          taxnameval;
  SeqEntryPtr   tmp;
  OrgModPtr     tmpmod;
  SubSourcePtr  tmpssp;
  ValNodePtr    ttl;
  Char          txt [128];
  UIEnum        val;
  ValNodePtr    vnp;

  if (sep == NULL || pfp == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL) {
      /* if nucprot set, look for biosource here */
      biop = NULL;
      SeqEntryToBioSource (sep, NULL, NULL, 0, &biop);
      if (biop != NULL && topbiop != NULL) {
        topbiop = biop;
      }
      for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
        DoOneParseItem (entityID, tmp, pfp, topbiop, useme, sep);
      }
      return;
    }
  }
  nsep = FindNucSeqEntry (sep);
  if (nsep != NULL && nsep->data.ptrvalue != NULL) {
    str [0] = '\0';
    if (useme != NULL) {
      StringNCpy_0 (str, useme, sizeof (str));
    } else if (pfp->parsedef) {
      ttl = SeqEntryGetSeqDescr (nsep, Seq_descr_title, NULL);
      if (ttl == NULL && nps != NULL) {
        ttl = SeqEntryGetSeqDescr (nps, Seq_descr_title, NULL);
      }
      if (ttl == NULL || ttl->data.ptrvalue == NULL) return;
      StringNCpy_0 (str, (CharPtr) ttl->data.ptrvalue, sizeof (str));
      GetTitle (pfp->atleft, txt, sizeof (txt));
      if (! StringHasNoText (txt)) {
        ptr = StringISearch (str, txt);
        if (ptr != NULL) {
          StringNCpy_0 (str, ptr + StringLen (txt), sizeof (str));
        }
      }
      GetTitle (pfp->atright, txt, sizeof (txt));
      if (txt [0] != '\0' /* ! StringHasNoText (txt) */) {
        ptr = StringISearch (str, txt);
        if (ptr != NULL) {
          *ptr = '\0';
        }
      }
    } else {
      bsp = (BioseqPtr) nsep->data.ptrvalue;
      for (sip = bsp->id; sip != NULL; sip = sip->next) {
        if (sip->choice == SEQID_LOCAL) {
          SeqIdWrite (sip, str, PRINTID_REPORT, sizeof (str));
          if (StringNICmp (str, "tmpseq_", 7) == 0 ||
              StringNICmp (str, "segseq_", 7) == 0 ||
              StringNICmp (str, "SEG_dna", 7) == 0) return;
        }
      }
    }
    if (StringHasNoText (str)) return;
    biop = NULL;
    SeqEntryToBioSource (sep, NULL, NULL, 0, &biop);
    taxnameval = GetValue (pfp->taxname);
    if (taxnameval == 2) {
      if (biop == NULL) {
        biop = BioSourceNew ();
        if (biop != NULL) {
          orp = OrgRefNew ();
          biop->org = orp;
          vnp = CreateNewDescriptor (sep, Seq_descr_source);
          if (vnp != NULL) {
            vnp->data.ptrvalue = (Pointer) biop;
          }
        }
      }
      if (biop == NULL) return;
      orp = biop->org;
      if (orp == NULL) return;
      orp->taxname = MemFree (orp->taxname);
      orp->taxname = StringSave (str);
      return;
    }
    if (biop == NULL && topbiop != NULL) {
      biop = (BioSourcePtr) AsnIoMemCopy ((Pointer) topbiop,
                                          (AsnReadFunc) BioSourceAsnRead,
                                          (AsnWriteFunc) BioSourceAsnWrite);
      if (biop != NULL) {
        vnp = CreateNewDescriptor (sep, Seq_descr_source);
        if (vnp != NULL) {
          vnp->data.ptrvalue = (Pointer) biop;
        }
      }
    }
    if (biop == NULL) return;
    orgmodtype = 0;
    subsourcetype = 0;
    if (taxnameval == 3) {
      orgmodtype = 255;
    } else if (taxnameval == 4) {
      subsourcetype = 255;
    } else if (GetEnumPopup (pfp->orgmod, orgmod_subtype_alist, &val) && val > 0) {
      orgmodtype = val;
    } else if (GetEnumPopup (pfp->subsource, subsource_subtype_alist, &val) && val > 0) {
      subsourcetype = val;
    }
    if (! StringHasNoText (str)) {
      if (orgmodtype > 0) {
        subtype = orgmodtype;
        orp = biop->org;
        if (orp == NULL) {
          orp = OrgRefNew ();
          biop->org = orp;
        }
        if (orp != NULL) {
          onp = orp->orgname;
          if (onp == NULL) {
            onp = OrgNameNew ();
            orp->orgname = onp;
          }
          if (onp != NULL) {
            mod = onp->mod;
            while (mod != NULL && mod->subtype != subtype) {
              mod = mod->next;
            }
            if (mod == NULL) {
              mod = OrgModNew ();
              if (onp->mod == NULL) {
                onp->mod = mod;
              } else {
                tmpmod = onp->mod;
                while (tmpmod->next != NULL) {
                  tmpmod = tmpmod->next;
                }
                tmpmod->next = mod;
              }
            }
            if (mod != NULL) {
              mod->subtype = subtype;
              mod->subname = MemFree (mod->subname);
              mod->subname = StringSave (str);
            }
          }
        }
      } else if (subsourcetype > 0) {
        subtype = subsourcetype;
        ssp = biop->subtype;
        while (ssp != NULL && ssp->subtype != subtype) {
          ssp = ssp->next;
        }
        if (ssp == NULL) {
          ssp = SubSourceNew ();
          if (biop->subtype == NULL) {
            biop->subtype = ssp;
          } else {
            tmpssp = biop->subtype;
            while (tmpssp->next != NULL) {
              tmpssp = tmpssp->next;
            }
            tmpssp->next = ssp;
          }
        }
        if (ssp != NULL) {
          ssp->subtype = subtype;
          ssp->name = MemFree (ssp->name);
          ssp->name = StringSave (str);
        }
      }
    }
  }
}

static void DoOneParse (Uint2 entityID, SeqEntryPtr sep, ParseFormPtr pfp, BioSourcePtr topbiop)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Char          gb [256];
  CharPtr       ptr;
  SeqIdPtr      sip;
  Char          str [250];
  SeqEntryPtr   tmp;

  if (sep == NULL || pfp == NULL) return;
  if (pfp->fp != NULL) {
    ReadLine (pfp->fp, str, sizeof (str)); /* FileGets on Mac sometimes sees '\r' instead of '\n' */
    while (Nlm_fileDone) {
      if (! StringHasNoText (str)) {
        ptr = StringChr (str, '\t');
        if (ptr != NULL) {
          *ptr = '\0';
          ptr++;
          TrimSpacesAroundString (str);
          TrimSpacesAroundString (ptr);
          sip = MakeSeqID (str);
          bsp = BioseqFind (sip);
          SeqIdFree (sip);
          if (bsp == NULL) {
            sprintf (gb, "gb|%s|", str);
            sip = MakeSeqID (gb);
            bsp = BioseqFind (sip);
            SeqIdFree (sip);
          }
          if (bsp != NULL) {
            tmp = GetBestTopParentForData (entityID, bsp);
             DoOneParseItem (entityID, tmp, pfp, topbiop, ptr, NULL);
          }
        }
      }
      ReadLine (pfp->fp, str, sizeof (str));
    }
    return;
  }
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 7 ||
                         (bssp->_class >= 13 && bssp->_class <= 16))) {
      for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
        DoOneParse (entityID, tmp, pfp, topbiop);
      }
      return;
    }
  }
  DoOneParseItem (entityID, sep, pfp, topbiop, NULL, NULL);
}

static void DoParseDeflineProc (ButtoN b)

{
  BioSourcePtr  topbiop;
  ParseFormPtr  pfp;
  SeqEntryPtr   sep;

  pfp = (ParseFormPtr) GetObjectExtra (b);
  if (pfp == NULL || pfp->input_entityID == 0) return;
  sep = GetTopSeqEntryForEntityID (pfp->input_entityID);
  if (sep == NULL) return;
  Hide (pfp->form);
  WatchCursor ();
  Update ();
  /* if popset, look for top biosource */
  SeqEntryToBioSource (sep, NULL, NULL, 0, &topbiop);
  if (! StringHasNoText (pfp->path)) {
    pfp->fp = FileOpen (pfp->path, "r");
  }
  DoOneParse (pfp->input_entityID, sep, pfp, topbiop);
  FileClose (pfp->fp);
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (pfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, pfp->input_entityID, 0, 0);
  Remove (pfp->form);
}

static void ParseDeflineMessageProc (ForM f, Int2 mssg)

{
  StdEditorProcsPtr  sepp;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    if (sepp->handleMessages != NULL) {
      sepp->handleMessages (f, mssg);
    }
  }
}

static void ParseDefOrLocalIDToSource (IteM i, Boolean parsedef, CharPtr path)

{
  ButtoN             b;
  BaseFormPtr        bfp;
  GrouP              c;
  GrouP              g;
  GrouP              h;
  GrouP              p;
  ParseFormPtr       pfp;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
  WindoW             w;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  pfp = (ParseFormPtr) MemNew (sizeof (ParseFormData));
  if (pfp == NULL) return;
  w = FixedWindow (-50, -33, -10, -10, "Parse Def Line", StdCloseWindowProc);
  SetObjectExtra (w, pfp, StdCleanupFormProc);
  pfp->form = (ForM) w;
  pfp->formmessage = ParseDeflineMessageProc;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    pfp->appmessage = sepp->handleMessages;
  }

  pfp->input_entityID = bfp->input_entityID;
  pfp->input_itemID = bfp->input_itemID;
  pfp->input_itemtype = bfp->input_itemtype;

  pfp->parsedef = parsedef;
  StringNCpy_0 (pfp->path, path, sizeof (pfp->path));
  pfp->fp = NULL;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  g = NULL;
  if (parsedef) {
    g = HiddenGroup (h, 4, 0, NULL);
    StaticPrompt (g, "Get text between", 0, dialogTextHeight, programFont, 'l');
    pfp->atleft = DialogText (g, "", 10, NULL);
    StaticPrompt (g, "and", 0, dialogTextHeight, programFont, 'l');
    pfp->atright = DialogText (g, "", 10, NULL);
  }

  p = HiddenGroup (h, 6, 0, NULL);
  if (parsedef || path != NULL) {
    StaticPrompt (p, "Place in", 0, popupMenuHeight, programFont, 'l');
  } else {
    StaticPrompt (p, "Place localID in", 0, popupMenuHeight, programFont, 'l');
  }
  pfp->taxname = PopupList (p, TRUE, NULL);
  SetObjectExtra (pfp->taxname, pfp, NULL);
  PopupItem (pfp->taxname, " ");
  PopupItem (pfp->taxname, "Taxname");
  PopupItem (pfp->taxname, "OrgMod Note");
  PopupItem (pfp->taxname, "SubSource Note");
  SetValue (pfp->taxname, 1);
  StaticPrompt (p, "or", 0, popupMenuHeight, programFont, 'l');
  pfp->orgmod = PopupList (p, TRUE, NULL);
  SetObjectExtra (pfp->orgmod, pfp, NULL);
  InitEnumPopup (pfp->orgmod, orgmod_subtype_alist, NULL);
  SetEnumPopup (pfp->orgmod, orgmod_subtype_alist, 0);
  StaticPrompt (p, "or", 0, popupMenuHeight, programFont, 'l');
  pfp->subsource = PopupList (p, TRUE, NULL);
  SetObjectExtra (pfp->subsource, pfp, NULL);
  InitEnumPopup (pfp->subsource, subsource_subtype_alist, NULL);
  SetEnumPopup (pfp->subsource, subsource_subtype_alist, 0);

  c = HiddenGroup (h, 4, 0, NULL);
  b = PushButton (c, "Accept", DoParseDeflineProc);
  SetObjectExtra (b, pfp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) p, (HANDLE) c, (HANDLE) g, NULL);
  RealizeWindow (w);
  Show (w);
  Select (w);
  Select (pfp->atleft);
  Update ();
}

extern void ParseDefToSource (IteM i)

{
  ParseDefOrLocalIDToSource (i, TRUE, NULL);
}

extern void ParseLocalIDToSource (IteM i)

{
  ParseDefOrLocalIDToSource (i, FALSE, NULL);
}

extern void ParseFileToSource (IteM i)

{
  Char  path [PATH_MAX];

  if (GetInputFileName (path, sizeof (path), "", "TEXT")) {
    ParseDefOrLocalIDToSource (i, FALSE, path);
  }
}

static Boolean AddStrainGatherFunc (GatherContextPtr gcp)

{
  BioSourcePtr  biop;
  Boolean       influenza;
  size_t        len;
  OrgModPtr     mod;
  OrgNamePtr    onp;
  OrgRefPtr     orp;
  CharPtr       ptr;
  ValNodePtr    sdp;
  SeqFeatPtr    sfp;
  CharPtr       str;
  CharPtr       strain;

  if (gcp == NULL || gcp->thisitem == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT  && gcp->thistype != OBJ_SEQDESC) return TRUE;

  orp = NULL;
  biop = NULL;
  switch (gcp->thistype) {
    case OBJ_SEQFEAT :
      sfp = (SeqFeatPtr) gcp->thisitem;
      switch (sfp->data.choice) {
        case SEQFEAT_ORG :
          orp = (OrgRefPtr) sfp->data.value.ptrvalue;
          break;
        case SEQFEAT_BIOSRC :
          biop = (BioSourcePtr) sfp->data.value.ptrvalue;
          break;
        default :
          break;
      }
      break;
    case OBJ_SEQDESC :
      sdp = (ValNodePtr) gcp->thisitem;
      switch (sdp->choice) {
        case Seq_descr_org :
          orp = (OrgRefPtr) sdp->data.ptrvalue;
          break;
        case Seq_descr_source :
          biop = (BioSourcePtr) sdp->data.ptrvalue;
          break;
        default :
          break;
      }
      break;
    default :
      break;
  }
  if (orp == NULL && biop != NULL) {
    orp = biop->org;
  }
  if (orp != NULL) {
    onp = orp->orgname;
    if (onp != NULL) {
      mod = onp->mod;
      strain = NULL;
      while (mod != NULL) {
        if (mod->subtype == 2) {
          strain = mod->subname;
        }
        mod = mod->next;
      }
      if (strain != NULL) {
        ptr = StringStr (orp->taxname, " (");
        if (ptr != NULL) {
          *ptr = '\0';
        } else {
          ptr = StringStr (orp->taxname, " (");
          if (ptr != NULL) {
            *ptr = '\0';
          }
        }
        influenza = FALSE;
        if (StringICmp (orp->taxname, "Influenza A virus") == 0 ||
            StringICmp (orp->taxname, "Influenza B virus") == 0) {
          influenza = TRUE;
        }
        len = StringLen (orp->taxname) + StringLen (strain) + 6;
        if (influenza) {
          len += 2;
        }
        str = MemNew (len);
        if (str != NULL) {
          StringCpy (str, orp->taxname);
          StringCat (str, " ");
          if (influenza) {
            StringCat (str, "(");
          }
          StringCat (str, strain);
          if (influenza) {
            StringCat (str, ")");
          }
          orp->taxname = MemFree (orp->taxname);
          orp->taxname = str;
        }
      }
    }
  }
  return TRUE;
}

extern void AddStrainToOrg (IteM i)

{
  BaseFormPtr  bfp;
  GatherScope  gs;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet ((Pointer) (gs.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  gs.ignore[OBJ_SEQDESC] = FALSE;
  GatherEntity (bfp->input_entityID, (Pointer) NULL, AddStrainGatherFunc, &gs);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  Update ();
}

static Boolean AddSubspeciesGatherFunc (GatherContextPtr gcp)

{
  BioSourcePtr  biop;
  size_t        len;
  OrgModPtr     mod;
  OrgNamePtr    onp;
  OrgRefPtr     orp;
  CharPtr       ptr;
  ValNodePtr    sdp;
  SeqFeatPtr    sfp;
  CharPtr       str;
  CharPtr       subspecies;

  if (gcp == NULL || gcp->thisitem == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT  && gcp->thistype != OBJ_SEQDESC) return TRUE;

  orp = NULL;
  biop = NULL;
  switch (gcp->thistype) {
    case OBJ_SEQFEAT :
      sfp = (SeqFeatPtr) gcp->thisitem;
      switch (sfp->data.choice) {
        case SEQFEAT_ORG :
          orp = (OrgRefPtr) sfp->data.value.ptrvalue;
          break;
        case SEQFEAT_BIOSRC :
          biop = (BioSourcePtr) sfp->data.value.ptrvalue;
          break;
        default :
          break;
      }
      break;
    case OBJ_SEQDESC :
      sdp = (ValNodePtr) gcp->thisitem;
      switch (sdp->choice) {
        case Seq_descr_org :
          orp = (OrgRefPtr) sdp->data.ptrvalue;
          break;
        case Seq_descr_source :
          biop = (BioSourcePtr) sdp->data.ptrvalue;
          break;
        default :
          break;
      }
      break;
    default :
      break;
  }
  if (orp == NULL && biop != NULL) {
    orp = biop->org;
  }
  if (orp != NULL) {
    onp = orp->orgname;
    if (onp != NULL) {
      mod = onp->mod;
      subspecies = NULL;
      while (mod != NULL) {
        if (mod->subtype == 22) {
          subspecies = mod->subname;
        }
        mod = mod->next;
      }
      if (subspecies != NULL) {
        ptr = StringStr (orp->taxname, " (");
        if (ptr != NULL) {
          *ptr = '\0';
        } else {
          ptr = StringStr (orp->taxname, " (");
          if (ptr != NULL) {
            *ptr = '\0';
          }
        }
        len = StringLen (orp->taxname) + StringLen (subspecies) + 6;
        str = MemNew (len);
        if (str != NULL) {
          StringCpy (str, orp->taxname);
          StringCat (str, " ");
          StringCat (str, subspecies);
          orp->taxname = MemFree (orp->taxname);
          orp->taxname = str;
        }
      }
    }
  }
  return TRUE;
}

extern void AddSubspeciesToOrg (IteM i)

{
  BaseFormPtr  bfp;
  GatherScope  gs;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet ((Pointer) (gs.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  gs.ignore[OBJ_SEQDESC] = FALSE;
  GatherEntity (bfp->input_entityID, (Pointer) NULL, AddSubspeciesGatherFunc, &gs);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  Update ();
}

static Boolean AddCloneGatherFunc (GatherContextPtr gcp)

{
  BioSourcePtr  biop;
  CharPtr       clone;
  size_t        len;
  OrgRefPtr     orp;
  CharPtr       ptr;
  ValNodePtr    sdp;
  SeqFeatPtr    sfp;
  SubSourcePtr  ssp;
  CharPtr       str;

  if (gcp == NULL || gcp->thisitem == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT  && gcp->thistype != OBJ_SEQDESC) return TRUE;

  orp = NULL;
  biop = NULL;
  switch (gcp->thistype) {
    case OBJ_SEQFEAT :
      sfp = (SeqFeatPtr) gcp->thisitem;
      switch (sfp->data.choice) {
        case SEQFEAT_ORG :
          orp = (OrgRefPtr) sfp->data.value.ptrvalue;
          break;
        case SEQFEAT_BIOSRC :
          biop = (BioSourcePtr) sfp->data.value.ptrvalue;
          break;
        default :
          break;
      }
      break;
    case OBJ_SEQDESC :
      sdp = (ValNodePtr) gcp->thisitem;
      switch (sdp->choice) {
        case Seq_descr_org :
          orp = (OrgRefPtr) sdp->data.ptrvalue;
          break;
        case Seq_descr_source :
          biop = (BioSourcePtr) sdp->data.ptrvalue;
          break;
        default :
          break;
      }
      break;
    default :
      break;
  }
  if (orp == NULL && biop != NULL) {
    orp = biop->org;
  }
  if (orp != NULL && biop != NULL) {
    if (biop->subtype != NULL) {
      ssp = biop->subtype;
      clone = NULL;
      while (ssp != NULL) {
        if (ssp->subtype == 3) {
          clone = ssp->name;
        }
        ssp = ssp->next;
      }
      if (clone != NULL) {
        ptr = StringStr (orp->taxname, " (");
        if (ptr != NULL) {
          *ptr = '\0';
        } else {
          ptr = StringStr (orp->taxname, " (");
          if (ptr != NULL) {
            *ptr = '\0';
          }
        }
        len = StringLen (orp->taxname) + StringLen (clone) + 6;
        str = MemNew (len);
        if (str != NULL) {
          StringCpy (str, orp->taxname);
          /* StringCat (str, " ("); */
          StringCat (str, " ");
          StringCat (str, clone);
          /* StringCat (str, ")"); */
          orp->taxname = MemFree (orp->taxname);
          orp->taxname = str;
        }
      }
    }
  }
  return TRUE;
}

extern void AddCloneToOrg (IteM i)

{
  BaseFormPtr  bfp;
  GatherScope  gs;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet ((Pointer) (gs.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  gs.ignore[OBJ_SEQDESC] = FALSE;
  GatherEntity (bfp->input_entityID, (Pointer) NULL, AddCloneGatherFunc, &gs);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  Update ();
}

typedef struct descformdata {
  FEATURE_FORM_BLOCK

  Uint2          oldEntityID;
  Uint2          oldItemID;
  Uint2          oldItemtype;
  Uint2          lookfor;
  Boolean        found;
  ObjMgrProcPtr  ompp;
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  SeqEntryPtr    sep;
  Handle         target;
  Boolean        usePopupForTarget;
  Boolean        hasMutPopPhySet;
  Int2           targetScratchSpace;
  Int2           nucProtCount;
  Int2           segSetCount;
  Uint2          descsubtype;
  ButtoN         createNewBtn;
} DescFormData, PNTR DescFormPtr;

static Boolean FindDescrFunc (GatherContextPtr gcp)

{
  DescFormPtr  dfp;
  ValNodePtr   vnp;

  if (gcp == NULL) return TRUE;
  dfp = (DescFormPtr) gcp->userdata;
  if (dfp == NULL) return TRUE;
  if (gcp->thistype == OBJ_SEQDESC) {
    vnp = (ValNodePtr) gcp->thisitem;
    if (vnp != NULL && vnp->choice == dfp->lookfor) {
      dfp->oldEntityID = gcp->entityID;
      dfp->oldItemID = gcp->itemID;
      dfp->oldItemtype = gcp->thistype;
      dfp->found = TRUE;
      return FALSE;
    }
  }
  return TRUE;
}

static CharPtr segClassList [] = {
  " ", "Nucleotide-Protein Set", "Segmented Nucleotide Set",
  "conset", "parts", "gibb", "gi", "genbank", "pir", "pub-set",
  "equiv", "swissprot", "pdb-entry", "MUTATION SET",
  "POPULATION SET", "PHYLOGENETIC SET", "ENVIRONMENTAL SAMPLES",
  "genomic product set", "other", NULL
};

static Int4 AllButPartsList (SeqEntryPtr sep, Pointer mydata,
                             SeqEntryFunc mycallback,
                             Int4 index, Int2 indent)

{
  BioseqSetPtr  bssp;

  if (sep == NULL) return index;
  if (IS_Bioseq (sep)) {
    if (mycallback != NULL)
      (*mycallback) (sep, mydata, index, indent);
    return index + 1;
  }
  if (Bioseq_set_class (sep) != 4) {
    if (mycallback != NULL)
      (*mycallback) (sep, mydata, index, indent);
    index++;
  }
  bssp = (BioseqSetPtr) sep->data.ptrvalue;
  sep = bssp->seq_set;
  indent++;
  while (sep != NULL) {
    index = AllButPartsList (sep, mydata, mycallback, index, indent);
    sep = sep->next;
  }
  return index;
}

#define AllButPartsCount( a )  AllButPartsList( a ,NULL,NULL,0,0);
#define AllButPartsExplore(a,b,c) AllButPartsList(a, b, c, 0L, 0);

static void PopTargetProc (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  DescFormPtr   dfp;
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Uint1         _class;
  CharPtr       ptr;
  SeqIdPtr      sip;
  Char          str [128];

  dfp = (DescFormPtr) mydata;
  if (sep != NULL && sep->data.ptrvalue != NULL) {
    if (sep->choice == 1) {
      bsp = (BioseqPtr) sep->data.ptrvalue;
      sip = SeqIdFindWorst (bsp->id);
      SeqIdWrite (sip, str, PRINTID_REPORT, sizeof (str));
      ptr = StringChr (str, '|');
      if (ptr == NULL) {
        ptr = str;
      } else {
        ptr++;
      }
      if (dfp->usePopupForTarget) {
        PopupItem (dfp->target, ptr);
      } else {
        ListItem (dfp->target, ptr);
      }
      if (bsp == dfp->bsp) {
        dfp->targetScratchSpace = index + 1;
      }
    } else if (sep->choice == 2) {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
      _class = bssp->_class;
      if (_class > 17) {
        _class = 18;
      }
      if (_class == 7 || (_class >= 13 && _class <= 16)) {
        dfp->hasMutPopPhySet = TRUE;
      }
      if (! dfp->hasMutPopPhySet) {
        sprintf (str, "[%s]", segClassList [_class]);
      } else if (_class == 1) {
        (dfp->nucProtCount)++;
        sprintf (str, "[%s %d]", segClassList [_class], (int) dfp->nucProtCount);
        (dfp->segSetCount)++;
      } else if (_class == 2) {
        sprintf (str, "[%s %d]", segClassList [_class], (int) dfp->segSetCount);
      } else {
        sprintf (str, "[%s]", segClassList [_class]);
      }
      if (dfp->usePopupForTarget) {
        PopupItem (dfp->target, str);
      } else {
        ListItem (dfp->target, str);
      }
    }
  }
}

static Int2 PopulateTarget (DescFormPtr dfp, BioseqPtr bsp, Uint2 entityID)

{
  SeqEntryPtr  sep;
  Int2         val;

  val = 0;
  if (dfp != NULL) {
    if (bsp != NULL) {
      sep = SeqMgrGetSeqEntryForData (bsp);
      entityID = ObjMgrGetEntityIDForPointer (sep);
    }
    sep = GetTopSeqEntryForEntityID (entityID);
    dfp->sep = sep;
    if (sep != NULL) {
      dfp->hasMutPopPhySet = FALSE;
      dfp->targetScratchSpace = 1;
      dfp->nucProtCount = 0;
      dfp->segSetCount = 0;
      AllButPartsExplore (sep, (Pointer) dfp, PopTargetProc);
      val = dfp->targetScratchSpace;
    }
  }
  return val;
}

static void FindNewTargetProc (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  DescFormPtr   dfp;

  dfp = (DescFormPtr) mydata;
  if (sep != NULL && sep->data.ptrvalue != NULL) {
    if (index + 1 == dfp->targetScratchSpace) {
      if (sep->choice == 1) {
        dfp->bsp = (BioseqPtr) sep->data.ptrvalue;
      } else if (sep->choice == 2) {
        dfp->bssp = (BioseqSetPtr) sep->data.ptrvalue;
      }
    }
  }
}

static Boolean ChangeTargetItemID (GatherContextPtr gcp)

{
  DescFormPtr  dfp;

  if (gcp == NULL) return TRUE;
  dfp = (DescFormPtr) gcp->userdata;
  if (dfp == NULL) return TRUE;
  if (gcp->thistype == OBJ_BIOSEQ) {
    if (dfp->bsp == (BioseqPtr) gcp->thisitem) {
      dfp->input_entityID = gcp->entityID;
      dfp->input_itemID = gcp->itemID;
      dfp->input_itemtype = gcp->thistype;
      return FALSE;
    }
  } else if (gcp->thistype == OBJ_BIOSEQSET) {
    if (dfp->bssp == (BioseqSetPtr) gcp->thisitem) {
      dfp->input_entityID = gcp->entityID;
      dfp->input_itemID = gcp->itemID;
      dfp->input_itemtype = gcp->thistype;
      return FALSE;
    }
  }
  return TRUE;
}

static void PreventDupTitleCreation (DescFormPtr dfp)

{
  ValNodePtr  sdp;

  if (dfp == NULL) return;
  if (dfp->bsp != NULL) {
    sdp = dfp->bsp->descr;
  } else if (dfp->bssp != NULL) {
    sdp = dfp->bssp->descr;
  } else return;
  if (dfp->descsubtype != Seq_descr_title) return;
  while (sdp != NULL) {
    if (sdp->choice == Seq_descr_title) {
      SafeDisable (dfp->createNewBtn);
      return;
    }
    sdp = sdp->next;
  }
  SafeEnable (dfp->createNewBtn);
}

static void ChangeNewDescTarget (Handle obj)

{
  DescFormPtr  dfp;
  GatherScope  gs;

  dfp = (DescFormPtr) GetObjectExtra (obj);
  if (dfp != NULL) {
    dfp->bsp = NULL;
    dfp->bssp = NULL;
    dfp->targetScratchSpace = GetValue (dfp->target);
    AllButPartsExplore (dfp->sep, (Pointer) dfp, FindNewTargetProc);
    MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
    gs.seglevels = 0;
    MemSet((Pointer) (gs.ignore), (int) (TRUE), (size_t) (OBJ_MAX * sizeof (Boolean)));
    gs.ignore[OBJ_BIOSEQ] = FALSE;
    gs.ignore[OBJ_BIOSEQSET] = FALSE;
    gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
    GatherEntity (dfp->input_entityID, (Pointer) dfp, ChangeTargetItemID, &gs);
    PreventDupTitleCreation (dfp);
  }
}

static void CreateNewDescProc (ButtoN b)

{
  DescFormPtr    dfp;
  OMProcControl  ompc;
  ObjMgrProcPtr  ompp;
  Int2           retval;

  dfp = (DescFormPtr) GetObjectExtra (b);
  if (dfp != NULL) {
    Hide (dfp->form);
    ompp = dfp->ompp;
    MemSet ((Pointer) (&ompc), 0, sizeof (OMProcControl));
    ompc.input_entityID = dfp->input_entityID;
    ompc.input_itemID = dfp->input_itemID;
    ompc.input_itemtype = dfp->input_itemtype;
    GatherDataForProc (&ompc, FALSE);
    ompc.proc = ompp;
    retval = (*(ompp->func)) (&ompc);
    if (retval == OM_MSG_RET_ERROR) {
      ErrShow ();
    }
    Update ();
    Remove (dfp->form);
  }
}

static void EditOldDescProc (ButtoN b)

{
  DescFormPtr  dfp;
  Int2         handled;

  dfp = (DescFormPtr) GetObjectExtra (b);
  if (dfp != NULL) {
    Hide (dfp->form);
    if (dfp->oldEntityID > 0 && dfp->oldItemID > 0 && dfp->oldItemtype > 0) {
      WatchCursor ();
      handled = GatherProcLaunch (OMPROC_EDIT, FALSE, dfp->oldEntityID,
                                  dfp->oldItemID, dfp->oldItemtype,
                                  0, 0, dfp->oldItemtype, 0);
      ArrowCursor ();
      if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
        Message (MSG_ERROR, "Unable to edit existing descriptor.");
      }
    }
    Update ();
    Remove (dfp->form);
  }
}

static CharPtr  newDescMsg = "\
Descriptors may apply to a single sequence or to a set of \
sequences.  Please set the target control to the desired \
set or sequence.";

static CharPtr  editOldMsg = "\
A descriptor of this type already exists at or above the \
displayed target.  You may want to edit it instead of \
creating a new descriptor.";

static void NewDescriptorMessageProc (ForM f, Int2 mssg)

{
  DescFormPtr  dfp;

  dfp = (DescFormPtr) GetObjectExtra (f);
  if (dfp != NULL) {
    if (dfp->appmessage != NULL) {
      dfp->appmessage (f, mssg);
    }
  }
}

extern void NewDescriptorMenuFunc (ObjMgrProcPtr ompp, BaseFormPtr bfp, Uint2 descsubtype)

{
  ButtoN             b;
  BioseqPtr          bsp;
  GrouP              c;
  Int4               count;
  DescFormPtr        dfp;
  Uint2              entityID;
  GrouP              g;
  GatherScope        gs;
  GrouP              h;
  GrouP              p1, p2;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
  SeqIdPtr           sip;
  SeqLocPtr          slp;
  Int2               val;
  WindoW             w;

#ifdef WIN_MAC
  bfp = (BaseFormPtr) currentFormDataPtr;
#endif
  if (bfp == NULL) return;
  if (ompp == NULL || ompp->func == NULL) return;
  if (ompp->inputtype != OBJ_SEQDESC) return;
  dfp = (DescFormPtr) MemNew (sizeof (DescFormData));
  if (dfp == NULL) return;
  dfp->ompp = ompp;
  dfp->lookfor = ompp->subinputtype;
  dfp->found = FALSE;
  dfp->descsubtype = descsubtype;
  bsp =  GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 0;
  gs.get_feats_location = TRUE;
  MemSet((Pointer) (gs.ignore), (int) (TRUE), (size_t) (OBJ_MAX * sizeof (Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQSET] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQDESC] = FALSE;
  if (bsp != NULL) {
    slp = ValNodeNew (NULL);
    slp->choice = SEQLOC_WHOLE;
    sip = SeqIdStripLocus (SeqIdDup (SeqIdFindBest (bsp->id, 0)));
    slp->data.ptrvalue = sip;
    gs.target = slp;
  }
  dfp->bsp = bsp;
  GatherEntity (bfp->input_entityID, (Pointer) dfp, FindDescrFunc, &gs);
  SeqLocFree (gs.target);
  w = FixedWindow (-50, -33, -10, -10, "Descriptor Target Control", StdCloseWindowProc);
  SetObjectExtra (w, dfp, StdCleanupFormProc);
  dfp->form = (ForM) w;
  dfp->formmessage = NewDescriptorMessageProc;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    dfp->appmessage = sepp->handleMessages;
  }

  dfp->input_entityID = bfp->input_entityID;
  dfp->input_itemID = bfp->input_itemID;
  dfp->input_itemtype = bfp->input_itemtype;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  p1 = MultiLinePrompt (h, newDescMsg, 25 * stdCharWidth, programFont);

  entityID = bfp->input_entityID;
  if (bsp != NULL) {
    sep = SeqMgrGetSeqEntryForData (bsp);
    entityID = ObjMgrGetEntityIDForChoice (sep);
  }
  sep = GetTopSeqEntryForEntityID (entityID);
  count = AllButPartsCount (sep);
  if (count < 32) {
    g = HiddenGroup (h, 2, 0, NULL);
    StaticPrompt (g, "Target", 0, popupMenuHeight, programFont, 'l');
    dfp->usePopupForTarget = TRUE;
    dfp->target = PopupList (g, TRUE, (PupActnProc) ChangeNewDescTarget);
  } else {
    g = HiddenGroup (h, 0, 2, NULL);
    StaticPrompt (g, "Target", 0, 0, programFont, 'c');
    dfp->usePopupForTarget = FALSE;
    dfp->target = SingleList (g, 14, 3, (LstActnProc) ChangeNewDescTarget);
  }
  SetObjectExtra (dfp->target, dfp, NULL);
  val = PopulateTarget (dfp, bsp, bfp->input_entityID);
  SetValue (dfp->target, val);
  ChangeNewDescTarget ((Handle) dfp->target);

  p2 = NULL;
  if (dfp->found) {
    p2 = MultiLinePrompt (h, editOldMsg, 25 * stdCharWidth, programFont);
  }

  c = HiddenGroup (h, 4, 0, NULL);
  SetGroupSpacing (c, 10, 2);
  dfp->createNewBtn = PushButton (c, "Create New", CreateNewDescProc);
  SetObjectExtra (dfp->createNewBtn, dfp, NULL);
  if (dfp->found) {
    b = DefaultButton (c, "Edit Old", EditOldDescProc);
    SetObjectExtra (b, dfp, NULL);
  }
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) p1, (HANDLE) g, (HANDLE) c, (HANDLE) p2, NULL);
  RealizeWindow (w);
  PreventDupTitleCreation (dfp);
  Show (w);
  Update ();
}

static void NewDescriptorMenuProc (IteM i)

{
  NewObjectPtr  nop;

  nop = (NewObjectPtr) GetObjectExtra (i);
  if (nop == NULL) return;
  NewDescriptorMenuFunc (nop->ompp, nop->bfp, nop->descsubtype);
}

extern void SetupNewDescriptorsMenu (MenU m, BaseFormPtr bfp)

{
  Boolean        allowgenbank;
  IteM           i;
  NewObjectPtr   nop;
  ObjMgrPtr      omp;
  ObjMgrProcPtr  ompp;
  ObjMgrTypePtr  omtp;

  if (m == NULL) return;
  omp = ObjMgrGet ();
  if (omp == NULL) return;
  ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT, 0, 0, NULL);
  if (ompp == NULL) return;
  omtp = NULL;
  allowgenbank = FALSE;
/*#ifdef EXTRA_SERVICES*/
  if (extraServices) {
    allowgenbank = TRUE;
  }
/*#endif*/
  while ((omtp = ObjMgrTypeFindNext (omp, omtp)) != NULL) {
    ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT, omtp->datatype, 0, NULL);
    if (ompp != NULL) {
      switch (omtp->datatype) {
        case OBJ_SEQDESC :
          ompp = NULL;
          while ((ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT,
				  omtp->datatype, 0, ompp)) != NULL) {
            if (ompp->subinputtype != Seq_descr_pub) {
              if (allowgenbank || ompp->subinputtype != Seq_descr_genbank) {
                i = CommandItem (m, ompp->proclabel, NewDescriptorMenuProc);
                nop = (NewObjectPtr) MemNew (sizeof (NewObjectData));
                if (nop != NULL) {
                  nop->ompp = ompp;
                  nop->bfp = bfp;
                  nop->descsubtype = ompp->subinputtype;
                }
                SetObjectExtra (i, (Pointer) nop, StdCleanupExtraProc);
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

static CharPtr  editOldDescMsg = "\
You may really want to edit an existing BioSource descriptor instead.\n\
Proceed anyway?";

static void NewFeatureMenuProc (IteM i)

{
  MsgAnswer      ans;
  BaseFormPtr    bfp;
  NewObjectPtr   nop;
  OMProcControl  ompc;
  ObjMgrProcPtr  ompp;
  Int2           retval;

  nop = (NewObjectPtr) GetObjectExtra (i);
  if (nop == NULL) return;
#ifdef WIN_MAC
  bfp = (BaseFormPtr) currentFormDataPtr;
#else
  bfp = nop->bfp;
#endif
  if (bfp == NULL) return;
  ompp = nop->ompp;
  if (ompp == NULL || ompp->func == NULL) return;
  if (ompp->subinputtype == FEATDEF_BIOSRC) {
    ans = Message (MSG_YN, editOldDescMsg);
    if (ans == ANS_NO) return;
  }
  MemSet ((Pointer) (&ompc), 0, sizeof (OMProcControl));
  ompc.input_entityID = bfp->input_entityID;
  ompc.input_itemID = bfp->input_itemID;
  ompc.input_itemtype = bfp->input_itemtype;
  GatherDataForProc (&ompc, FALSE);
  ompc.proc = ompp;
  retval = (*(ompp->func)) (&ompc);
  if (retval == OM_MSG_RET_ERROR) {
    ErrShow ();
  }
}

#ifdef WIN_MAC
VoidPtr macUserDataPtr = NULL;
#endif

extern void EnableFeaturesPerTarget (BaseFormPtr bfp)

{
  BioseqPtr     bsp;
  Uint1         mol;
  NewObjectPtr  nop;

  if (bfp == NULL) return;
#ifdef WIN_MAC
  nop = (NewObjectPtr) macUserDataPtr;
#else
  nop = (NewObjectPtr) bfp->userDataPtr;
#endif
  bsp =  GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
  mol = 0;
  if (bsp != NULL) {
    mol = bsp->mol;
  }
  while (nop != NULL) {
    if (nop->kind == 2) {
      /* analysis menu item, ignore it */
    } else if (mol == 0) {
      SafeDisable (nop->item);
    } else if (ISA_na (mol) && (nop->molgroup == 2 || nop->molgroup == 3)) {
      SafeEnable (nop->item);
    } else if (ISA_aa (mol) && (nop->molgroup == 1 || nop->molgroup == 3)) {
      SafeEnable (nop->item);
    } else {
      SafeDisable (nop->item);
    }
    nop = nop->next;
  }
}

static VoidPtr LinkNewObjectLists (NewObjectPtr list1, NewObjectPtr list2)

{
  NewObjectPtr  nop;

  if (list1 == NULL) return list2;
  nop = list1;
  while (nop->next != NULL) {
    nop = nop->next;
  }
  nop->next = list2;
  return list1;
}

extern void SetupNewFeaturesMenu (MenU m, BaseFormPtr bfp)

{
  FeatDispGroupPtr  fdgp;
  FeatDefPtr        fdp;
  NewObjectPtr      first;
  IteM              i;
  Uint1             key;
  CharPtr           label;
  NewObjectPtr      last;
  NewObjectPtr      nop;
  ObjMgrPtr         omp;
  ObjMgrProcPtr     ompp;
  ObjMgrTypePtr     omtp;
  MenU              sub;
  Uint2             subtype;

  if (m == NULL) return;
  omp = ObjMgrGet ();
  if (omp == NULL) return;
  ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT, 0, 0, NULL);
  if (ompp == NULL) return;
  omtp = NULL;
  first = NULL;
  last = NULL;
  while ((omtp = ObjMgrTypeFindNext (omp, omtp)) != NULL) {
    ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT, omtp->datatype, 0, NULL);
    if (ompp != NULL) {
      switch (omtp->datatype) {
        case OBJ_SEQFEAT :
          fdgp = NULL;
          while ((fdgp = DispGroupFindNext (fdgp, &key, &label)) != NULL) {
            if (fdgp->groupkey != 0) {
              sub = SubMenu (m, fdgp->groupname);
              fdp = NULL;
              label = NULL;
              while ((fdp = FeatDefFindNext (fdp, &key, &label,
                     fdgp->groupkey, FALSE)) != NULL) {
                if (key != FEATDEF_BAD) {
                  ompp = NULL;
                  while ((ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT,
				          omtp->datatype, 0, ompp)) != NULL) {
                    if (ompp->subinputtype == fdp->featdef_key &&
                        ompp->subinputtype != FEATDEF_PUB) {
                      i = CommandItem (sub, ompp->proclabel, NewFeatureMenuProc);
                      nop = (NewObjectPtr) MemNew (sizeof (NewObjectData));
                      if (nop != NULL) {
                        nop->kind = 1; /* feature creation item */
                        nop->ompp = ompp;
                        nop->bfp = bfp;
                        nop->item = i;
                        nop->molgroup = fdp->molgroup;
                      }
                      if (first == NULL) {
                        first = nop;
                      }
                      if (last != NULL) {
                        last->next = nop;
                      }
                      last = nop;
                      SetObjectExtra (i, (Pointer) nop, StdCleanupExtraProc);
                    }
                  }
                }
              }
            }
          }
          /* if (indexerVersion) { */
            sub = SubMenu (m, "Remaining Features");
            fdp = NULL;
            label = NULL;
            while ((fdp = FeatDefFindNext (fdp, &key, &label, 0, FALSE)) != NULL) {
              if (key != FEATDEF_BAD) {
                ompp = NULL;
                while ((ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT,
				        omtp->datatype, 0, ompp)) != NULL) {
				  subtype = ompp->subinputtype;
                  if (subtype == fdp->featdef_key &&
                      subtype != FEATDEF_PUB &&
                      subtype != FEATDEF_Imp_CDS &&
                      subtype != FEATDEF_misc_RNA &&
                      subtype != FEATDEF_precursor_RNA &&
                      subtype != FEATDEF_mat_peptide &&
                      subtype != FEATDEF_sig_peptide &&
                      subtype != FEATDEF_transit_peptide &&
                      subtype != FEATDEF_source &&
                      subtype != FEATDEF_virion &&
                      subtype != FEATDEF_mutation &&
                      subtype != FEATDEF_allele &&
                      subtype != FEATDEF_site_ref) {
                    i = CommandItem (sub, ompp->proclabel, NewFeatureMenuProc);
                    nop = (NewObjectPtr) MemNew (sizeof (NewObjectData));
                    if (nop != NULL) {
                      nop->kind = 1; /* feature creation item */
                      nop->ompp = ompp;
                      nop->bfp = bfp;
                      nop->item = i;
                      nop->molgroup = fdp->molgroup;
                    }
                    if (first == NULL) {
                      first = nop;
                    }
                    if (last != NULL) {
                      last->next = nop;
                    }
                    last = nop;
                    SetObjectExtra (i, (Pointer) nop, StdCleanupExtraProc);
                  }
                }
              }
            }
          /* } */
          break;
        default :
          break;
      }
    }
  }
#ifdef WIN_MAC
  macUserDataPtr = LinkNewObjectLists (macUserDataPtr, first);
#else
  bfp->userDataPtr = LinkNewObjectLists (bfp->userDataPtr, first);
#endif
}

extern void SetupNewPublicationsMenu (MenU m, BaseFormPtr bfp)

{
  NewObjectPtr   first;
  IteM           i;
  NewObjectPtr   nop;
  ObjMgrPtr      omp;
  ObjMgrProcPtr  ompp;
  ObjMgrTypePtr  omtp;

  if (m == NULL) return;
  omp = ObjMgrGet ();
  if (omp == NULL) return;
  ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT, 0, 0, NULL);
  if (ompp == NULL) return;
  omtp = NULL;
  while ((omtp = ObjMgrTypeFindNext (omp, omtp)) != NULL) {
    ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT, omtp->datatype, 0, NULL);
    if (ompp != NULL) {
      switch (omtp->datatype) {
        case OBJ_SEQFEAT :
            ompp = NULL;
            while ((ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT,
				    omtp->datatype, 0, ompp)) != NULL) {
              if (ompp->subinputtype == FEATDEF_PUB) {
                i = CommandItem (m, "Publication Feature", NewFeatureMenuProc);
                nop = (NewObjectPtr) MemNew (sizeof (NewObjectData));
                if (nop != NULL) {
                  nop->kind = 1; /* feature creation item */
                  nop->ompp = ompp;
                  nop->bfp = bfp;
                  nop->item = i;
                  nop->molgroup = 3;
#ifdef WIN_MAC
                  first = (NewObjectPtr) macUserDataPtr;
#else
                  first = (NewObjectPtr) bfp->userDataPtr;
#endif
                  if (first != NULL) {
                    while (first->next != NULL) {
                      first = first->next;
                    }
                    first->next = nop;
                  }
                }
                SetObjectExtra (i, (Pointer) nop, StdCleanupExtraProc);
              }
            }
          break;
        case OBJ_SEQDESC :
          ompp = NULL;
          while ((ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT,
				  omtp->datatype, 0, ompp)) != NULL) {
            if (ompp->subinputtype == Seq_descr_pub) {
              i = CommandItem (m, "Publication Descriptor", NewDescriptorMenuProc);
              nop = (NewObjectPtr) MemNew (sizeof (NewObjectData));
              if (nop != NULL) {
                nop->ompp = ompp;
                nop->bfp = bfp;
              }
              SetObjectExtra (i, (Pointer) nop, StdCleanupExtraProc);
            }
          }
          break;
        default :
          break;
      }
    }
  }
}

#ifdef WIN_MAC
extern IteM  addSecondaryItem;
#endif

extern void SetupEditSecondary (MenU m, BaseFormPtr bfp)

{
  IteM           i;
  NewObjectPtr   nop;
  ObjMgrPtr      omp;
  ObjMgrProcPtr  ompp;
  ObjMgrTypePtr  omtp;

  if (m == NULL) return;
  omp = ObjMgrGet ();
  if (omp == NULL) return;
  ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT, 0, 0, NULL);
  if (ompp == NULL) return;
  omtp = NULL;
  while ((omtp = ObjMgrTypeFindNext (omp, omtp)) != NULL) {
    ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT, omtp->datatype, 0, NULL);
    if (ompp != NULL) {
      switch (omtp->datatype) {
        case OBJ_SEQDESC :
          ompp = NULL;
          while ((ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT,
				  omtp->datatype, 0, ompp)) != NULL) {
            if (ompp->subinputtype != Seq_descr_pub) {
              if (ompp->subinputtype == Seq_descr_genbank) {
                i = CommandItem (m, "Add Secondary", NewDescriptorMenuProc);
                nop = (NewObjectPtr) MemNew (sizeof (NewObjectData));
                if (nop != NULL) {
                  nop->ompp = ompp;
                  nop->bfp = bfp;
                }
                SetObjectExtra (i, (Pointer) nop, StdCleanupExtraProc);
#ifdef WIN_MAC
                if (addSecondaryItem == NULL) {
                  addSecondaryItem = i;
                }
#endif
                return;
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

typedef struct gbformdata {
  FEATURE_FORM_BLOCK

  ButtoN         clearExtraAccns;
  ButtoN         clearSource;
  ButtoN         clearKeywords;
  ButtoN         clearOrigin;
  ButtoN         clearOldDate;
  ButtoN         clearEntryDate;
  ButtoN         clearDivision;
  ButtoN         clearTaxonomy;
} GbeditFormData, PNTR GbeditFormPtr;

static void EditGenbankCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  Boolean        empty;
  GBBlockPtr     gbp;
  GbeditFormPtr  gfp;
  ValNodePtr     nextsdp;
  Pointer PNTR   prevsdp;
  ValNodePtr     sdp;

  if (mydata == NULL) return;
  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  gfp = (GbeditFormPtr) mydata;
  if (gfp == NULL) return;
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
    empty = FALSE;
    if (sdp->choice == Seq_descr_genbank && sdp->data.ptrvalue != NULL) {
      gbp = (GBBlockPtr) sdp->data.ptrvalue;
      if (GetStatus (gfp->clearExtraAccns)) {
        gbp->extra_accessions = ValNodeFreeData (gbp->extra_accessions);
      }
      if (GetStatus (gfp->clearSource)) {
        gbp->source = MemFree (gbp->source);
      }
      if (GetStatus (gfp->clearKeywords)) {
        gbp->keywords = ValNodeFreeData (gbp->keywords);
      }
      if (GetStatus (gfp->clearOrigin)) {
        gbp->origin = MemFree (gbp->origin);
      }
      if (GetStatus (gfp->clearOldDate)) {
        gbp->date = MemFree (gbp->date);
      }
      if (GetStatus (gfp->clearEntryDate)) {
        gbp->entry_date = DateFree (gbp->entry_date);
      }
      if (GetStatus (gfp->clearDivision)) {
        gbp->div = MemFree (gbp->div);
      }
      if (GetStatus (gfp->clearTaxonomy)) {
        gbp->taxonomy = MemFree (gbp->taxonomy);
      }
      if (gbp->extra_accessions == NULL && gbp->source == NULL &&
          gbp->keywords == NULL && gbp->origin == NULL &&
          gbp->date == NULL && gbp->entry_date == NULL &&
          gbp->div == NULL && gbp->taxonomy == NULL) {
        empty = TRUE;
        ObjMgrDeSelect (0, 0, 0, 0, NULL);
      }
    }
    if (empty) {
      *(prevsdp) = sdp->next;
      sdp->next = NULL;
      SeqDescFree (sdp);
    } else {
      prevsdp = (Pointer PNTR) &(sdp->next);
    }
    sdp = nextsdp;
  }
}

static void DoEditGenbank (ButtoN b)

{
  GbeditFormPtr  gfp;
  SeqEntryPtr    sep;

  gfp = GetObjectExtra (b);
  if (gfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (gfp->input_entityID);
  if (sep == NULL) return;
  Hide (gfp->form);
  WatchCursor ();
  Update ();
  SeqEntryExplore (sep, (Pointer) gfp, EditGenbankCallback);
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (gfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, gfp->input_entityID, 0, 0);
  Remove (gfp->form);
}

static void EditGenbankMessageProc (ForM f, Int2 mssg)

{
  GbeditFormPtr  gfp;

  gfp = (GbeditFormPtr) GetObjectExtra (f);
  if (gfp != NULL) {
    if (gfp->appmessage != NULL) {
      gfp->appmessage (f, mssg);
    }
  }
}

extern void EditGenbankElements (Handle i)

{
  BaseFormPtr        bfp;
  ButtoN             b;
  GrouP              c;
  GrouP              g;
  GbeditFormPtr      gfp;
  GrouP              h;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
  WindoW             w;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  gfp = (GbeditFormPtr) MemNew (sizeof (GbeditFormData));
  if (gfp == NULL) return;
  w = FixedWindow (-50, -33, -10, -10, "GenBank Block Removal", StdCloseWindowProc);
  SetObjectExtra (w, gfp, StdCleanupFormProc);
  gfp->form = (ForM) w;
  gfp->formmessage = EditGenbankMessageProc;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    gfp->appmessage = sepp->handleMessages;
  }

  gfp->input_entityID = bfp->input_entityID;
  gfp->input_itemID = bfp->input_itemID;
  gfp->input_itemtype = bfp->input_itemtype;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  g = HiddenGroup (h, -1, 0, NULL);

  gfp->clearExtraAccns = CheckBox (g, "Clear Secondary Accessions", NULL);
  gfp->clearSource = CheckBox (g, "Clear Source Line", NULL);
  gfp->clearKeywords = CheckBox (g, "Clear Keywords", NULL);
  gfp->clearOrigin = CheckBox (g, "Clear Origin", NULL);
  gfp->clearOldDate = CheckBox (g, "Clear Old Date", NULL);
  gfp->clearEntryDate = CheckBox (g, "Clear Entry Date", NULL);
  gfp->clearDivision = CheckBox (g, "Clear Division", NULL);
  gfp->clearTaxonomy = CheckBox (g, "Clear Lineage", NULL);

  c = HiddenGroup (h, 4, 0, NULL);
  b = PushButton (c, "Accept", DoEditGenbank);
  SetObjectExtra (b, gfp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Update ();
}

typedef struct helpindex {
  Int2           item;
  CharPtr        heading;
  CharPtr        section;
  CharPtr        combined;
} HelpIndex, PNTR HelpIndexPtr;

typedef struct helpform {
  FORM_MESSAGE_BLOCK
  DoC            doc;
  DoC            list;
  TexT           findTxt;
  ButtoN         findBtn;
  GrouP          dismissGrp;
  ValNodePtr     mainStrings;
  ValNodePtr     indexStrings;
  ValNodePtr     index;
  Char           file [PATH_MAX];
} HelpForm, PNTR HelpFormPtr;

#define TBL_FMT 1
#define TXT_FMT 2
#define HDG_FMT 3
#define SUB_FMT 4  /* and beyond, encoding indent */

static ParData lstParFmt = {FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0};
static ColData lstColFmt = {0, 0, 80, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, TRUE};

static ParData hdgParFmt = {TRUE, FALSE, FALSE, FALSE, FALSE, 0, 0};
static ColData hdgColFmt = {0, 0, 80, 0, NULL, 'c', TRUE, FALSE, FALSE, FALSE, TRUE};

static ParData subParFmt = {TRUE, FALSE, FALSE, FALSE, FALSE, 0, 0};
static ColData subColFmt = {0, 0, 80, 0, NULL, 'l', TRUE, FALSE, FALSE, FALSE, TRUE};

static ParData tblParFmt = {TRUE, FALSE, FALSE, FALSE, TRUE, 0, 0};
static ColData tblColFmt = {0, 0, 80, 0, NULL, 'l', TRUE, FALSE, FALSE, FALSE, TRUE};

static ParData txtParFmt = {TRUE, FALSE, FALSE, FALSE, FALSE, 0, 0};
static ColData txtColFmt = {0, 0, 80, 0, NULL, 'l', TRUE, FALSE, FALSE, FALSE, TRUE};

static void ResetHelpLists (HelpFormPtr hfp)

{
  HelpIndexPtr  hip;
  ValNodePtr    vnp;

  if (hfp != NULL) {
    hfp->mainStrings = ValNodeFreeData (hfp->mainStrings);
    hfp->indexStrings = ValNodeFreeData (hfp->indexStrings);
    vnp = hfp->index;
    while (vnp != NULL) {
      hip = (HelpIndexPtr) vnp->data.ptrvalue;
      if (hip != NULL) {
        hip->heading = MemFree (hip->heading);
        hip->section = MemFree (hip->section);
        hip->combined = MemFree (hip->combined);
      }
      vnp = vnp->next;
    }
    hfp->index = ValNodeFreeData (hfp->index);
  }
}

static void TrimTrailingSpaces (CharPtr str)

{
  size_t  len;

  if (str != NULL && str [0] != '\0') {
    len = StringLen (str);
    while (len > 0 && str [len - 1] == ' ') {
      len--;
    }
    str [len] = '\0';
  }
}

static Boolean ParseHelpFile (HelpFormPtr hfp, Boolean printPath)

{
  Char          ch;
  Uint1         choice;
  FILE          *fp;
  Boolean       goOn;
  Char          heading [64];
  HelpIndexPtr  hip;
  Boolean       inTable;
  Int2          level;
  ValNodePtr    list;
  Int2          numItems;
  Char          path [PATH_MAX];
  CharPtr       ptr;
  Char          section [64];
  Char          str [256];
  ValNodePtr    vnp;

  if (hfp == NULL || hfp->doc == NULL) return FALSE;
  Hide (hfp->list);
  Hide (hfp->doc);
  Update ();
  Reset (hfp->list);
  Reset (hfp->doc);
  ResetHelpLists (hfp);
  if (hfp->file == NULL || hfp->file [0] == '\0') return FALSE;
  numItems = 0;
  ProgramPath (path, sizeof (path));
  ptr = StringRChr (path, DIRDELIMCHR);
  if (ptr != NULL) {
    *ptr = '\0';
  }
  FileBuildPath (path, NULL, hfp->file);
  fp = FileOpen (path, "r");
  if (fp == NULL) {
    if (GetAppParam ("NCBI", "ErrorProcessing", "MsgPath", NULL, path, sizeof (path))) {
      FileBuildPath (path, NULL, hfp->file);
      fp = FileOpen (path, "r");
    }
  }
  if (fp == NULL) {
    if (GetAppParam ("NCBI", "NCBI", "DATA", NULL, path, sizeof (path))) {
      FileBuildPath (path, NULL, hfp->file);
      fp = FileOpen (path, "r");
    }
  }
  if (fp != NULL) {
    if (printPath) {
      SetTitle (hfp->form, path);
    }
    list = NULL;
    heading [0] = '\0';
    section [0] = '\0';
    inTable = FALSE;
    goOn = (fgets (str, sizeof (str), fp) != NULL);
    while (goOn) {
      ptr = str;
      ch = *ptr;
      while (ch != '\n' && ch != '\r' && ch != '\0') {
        ptr++;
        ch = *ptr;
      }
      *ptr = '\0';
      if (inTable) {
        TrimTrailingSpaces (str);
      } else {
        TrimSpacesAroundString (str);
      }
      ch = str [0];
      if (ch == '>' || ch == '*' || ch == '#' || ch == '!') {
        if (list != NULL) {
          ptr = MergeValNodeStrings (list, inTable);
          if (inTable) {
            choice = TBL_FMT;
          } else {
            choice = TXT_FMT;
          }
          numItems++;
          vnp = ValNodeAdd (&(hfp->mainStrings));
          if (vnp != NULL) {
            vnp->choice = choice;
            vnp->data.ptrvalue = ptr;
          }
          /* ptr = MemFree (ptr); */
          list = ValNodeFreeData (list);
        }
        ch = str [0];
        if (ch == '>') {
          StringNCpy_0 (heading, str + 1, sizeof (heading));
          numItems++;
          vnp = ValNodeAdd (&(hfp->mainStrings));
          if (vnp != NULL) {
            vnp->choice = HDG_FMT;
            vnp->data.ptrvalue = StringSave (heading);
          }
          vnp = ValNodeAdd (&(hfp->indexStrings));
          if (vnp != NULL) {
            vnp->choice = 0;
            vnp->data.ptrvalue = StringSave (heading);
          }
          vnp = ValNodeAdd (&(hfp->index));
          if (vnp != NULL) {
            hip = (HelpIndexPtr) MemNew (sizeof (HelpIndex));
            vnp->data.ptrvalue = (Pointer) hip;
            if (hip != NULL) {
              hip->item = numItems;
              hip->heading = StringSave (heading);
            }
          }
        } else if (ch == '*') {
          level = 1;
          ch = str [level];
          while (ch == '*') {
            level++;
            ch = str [level];
          }
          StringNCpy_0 (section, str + level, sizeof (section));
          numItems++;
          vnp = ValNodeAdd (&(hfp->mainStrings));
          if (vnp != NULL) {
            if (level < 2) {
              vnp->choice = SUB_FMT;
            } else {
              vnp->choice = 5 * (level - 1);
            }
            vnp->data.ptrvalue = StringSave (section);
          }
          vnp = ValNodeAdd (&(hfp->indexStrings));
          if (vnp != NULL) {
            vnp->choice = 5 * level;
            vnp->data.ptrvalue = StringSave (section);
          }
          vnp = ValNodeAdd (&(hfp->index));
          if (vnp != NULL) {
            hip = (HelpIndexPtr) MemNew (sizeof (HelpIndex));
            vnp->data.ptrvalue = (Pointer) hip;
            if (hip != NULL) {
              hip->item = numItems;
              hip->heading = StringSave (heading);
              hip->section = StringSave (section);
              sprintf (str, "%s|%s", heading, section);
              hip->combined = StringSave (str);
            }
          }
        } else if (ch == '#' || ch == '!') {
          inTable = (Boolean) (ch == '!');
          vnp = ValNodeAdd (&list);
          if (vnp != NULL) {
            if (! StringHasNoText (str + 1)) {
              vnp->data.ptrvalue = StringSave (str + 1);
            }
          }
        }
      } else if (ch == '<') {
      } else if (ch != '\0') {
        vnp = ValNodeAdd (&list);
        if (vnp != NULL) {
          if (! StringHasNoText (str)) {
            vnp->data.ptrvalue = StringSave (str);
          }
        }
      }
      goOn = (Boolean) (goOn && (fgets (str, sizeof (str), fp) != NULL));
    }
    if (list != NULL) {
      ptr = MergeValNodeStrings (list, inTable);
      if (inTable) {
        choice = TBL_FMT;
      } else {
        choice = TXT_FMT;
      }
      numItems++;
      vnp = ValNodeAdd (&(hfp->mainStrings));
      if (vnp != NULL) {
        vnp->choice = choice;
        vnp->data.ptrvalue = ptr;
      }
      /* ptr = MemFree (ptr); */
      list = ValNodeFreeData (list);
    }
    FileClose (fp);
    return TRUE;
  } else {
    return FALSE;
  }
}

static Int2 GetSequinAppParam (CharPtr section, CharPtr type, CharPtr dflt, CharPtr buf, Int2 buflen)

{
  Int2  rsult;

  rsult = GetAppParam ("SEQUINCUSTOM", section, type, NULL, buf, buflen);
  if (rsult) return rsult;
  rsult = GetAppParam ("SEQUIN", section, type, dflt, buf, buflen);
  return rsult;
}

static FonT GetHelpFontFromConfig (CharPtr param, FonT dfault)

{
  FonT  f;
  Char  str [128];

  f = dfault;
  if (GetSequinAppParam ("SCREEN", param, NULL, str, sizeof (str))) {
    f = Nlm_ParseFontEx (str, NULL);
  }
  if (f == NULL) {
    f = dfault;
  }
  return f;
}

static void SetupHelpFonts (void)

{
  if (IsJapanese()) {
#ifdef WIN_MAC
    /* Osaka is common font for Japanese on Macintosh. */
    hdgColFmt.font = Nlm_ParseFontEx ("Osaka,14,b", NULL);
    subColFmt.font = Nlm_ParseFontEx ("Osaka,10,b", NULL);
    txtColFmt.font = Nlm_ParseFontEx ("Osaka,10", NULL);
    lstColFmt.font = Nlm_ParseFontEx ("Osaka\x81\x7c\x93\x99\x95\x9d,12", NULL);
    tblColFmt.font = Nlm_ParseFontEx ("Osaka\x81\x7c\x93\x99\x95\x9d,12", NULL);
/*    hdgColFmt.font = GetResidentFont (Nlm_Minchou(14, STYLE_BOLD));	*/
/*    subColFmt.font = GetResidentFont (Nlm_Minchou(10, STYLE_BOLD));	*/
/*    txtColFmt.font = GetResidentFont (Nlm_Gothic(10, STYLE_REGULAR));	*/
/*    lstColFmt.font = GetResidentFont (Nlm_MinchouFixed(12, STYLE_REGULAR));	*/
/*    tblColFmt.font = GetResidentFont (Nlm_MinchouFixed(12, STYLE_REGULAR));	*/
#endif
#ifdef WIN_MSWIN
    hdgColFmt.font = Nlm_ParseFontEx ("\x82\x6c\x82\x72\x20\x82\x6f\x96\xbe\x92\xa9,14,b,Kanji", NULL);
    subColFmt.font = Nlm_ParseFontEx ("\x82\x6c\x82\x72\x20\x82\x6f\x96\xbe\x92\xa9,11,b,Kanji", NULL);
    txtColFmt.font = Nlm_ParseFontEx ("\x82\x6c\x82\x72\x20\x82\x6f\x83\x53\x83\x56\x83\x62\x83\x4e,11,,Kanji", NULL);
    lstColFmt.font = Nlm_ParseFontEx ("\x82\x6c\x82\x72\x20\x96\xbe\x92\xa9,12,f,Kanji", NULL);
    tblColFmt.font = Nlm_ParseFontEx ("\x82\x6c\x82\x72\x20\x96\xbe\x92\xa9,12,f,Kanji", NULL);
/*    hdgColFmt.font = GetResidentFont (Nlm_Minchou(14, STYLE_BOLD));	*/
/*    subColFmt.font = GetResidentFont (Nlm_Minchou(11, STYLE_BOLD));	*/
/*    txtColFmt.font = GetResidentFont (Nlm_Gothic(11, STYLE_REGULAR));	*/
/*    lstColFmt.font = GetResidentFont (Nlm_MinchouFixed(12, STYLE_REGULAR));	*/
/*    tblColFmt.font = GetResidentFont (Nlm_MinchouFixed(12, STYLE_REGULAR));	*/
#endif
#ifdef WIN_MOTIF
    hdgColFmt.font = ParseFont ("Times,18,b");
    subColFmt.font = ParseFont ("Helvetica,12,b");
    txtColFmt.font = ParseFont ("fixed,13");
    lstColFmt.font = programFont;
    tblColFmt.font = programFont;
#endif
  } else if (IsEnglish() || IsFrench() || IsGerman() || IsItalian() || IsSystemLang()) {
#ifdef WIN_MAC
    hdgColFmt.font = ParseFont ("Times,14,b");
    subColFmt.font = ParseFont ("Geneva,10,b");
    txtColFmt.font = ParseFont ("Geneva,10");
#endif /*WIN_MAC*/
#ifdef WIN_MSWIN
    hdgColFmt.font = ParseFont ("Times New Roman,14,b");
    subColFmt.font = ParseFont ("Arial,11,b");
    txtColFmt.font = ParseFont ("Times New Roman,11");
#endif /*WIN_MSWIN*/
#ifdef WIN_MOTIF
    hdgColFmt.font = ParseFont ("Times,18,b");
    subColFmt.font = ParseFont ("Helvetica,12,b");
    txtColFmt.font = ParseFont ("Times,14");
#endif /*WIN_MOTIF*/
    lstColFmt.font = programFont;
    tblColFmt.font = programFont;
  } else {
  /* above call to IsSystemLang should override this section */
    /* default system font is often native character set */
    /* because native character sets have a set of ascii letters,
       english help file can also be drawn as well as native letters. */
    hdgColFmt.font = systemFont;
    subColFmt.font = systemFont;
    txtColFmt.font = systemFont;
    lstColFmt.font = systemFont;
    tblColFmt.font = systemFont;
  }

  /* now allow override by sequin config file */
  hdgColFmt.font = GetHelpFontFromConfig ("HEADING", hdgColFmt.font);
  subColFmt.font = GetHelpFontFromConfig ("SUBHEAD", subColFmt.font);
  txtColFmt.font = GetHelpFontFromConfig ("TEXT", txtColFmt.font);
  lstColFmt.font = GetHelpFontFromConfig ("LIST", lstColFmt.font);
  tblColFmt.font = GetHelpFontFromConfig ("TABLE", tblColFmt.font);
}

static Boolean PopulateHelpForm (HelpFormPtr hfp)

{
  Int2        firstLine;
  Int2        firstShown;
  RecT        r;
  BaR         sb;
  Int4        startsAt;
  CharPtr     text;
  ValNodePtr  vnp;

  if (hfp == NULL || hfp->doc == NULL) return FALSE;
  Hide (hfp->list);
  Hide (hfp->doc);
  Update ();
  if (! GetScrlParams4 (hfp->doc, NULL, &firstShown, &firstLine)) {
    firstShown = 0;
    firstLine = 0;
  }
  sb = GetSlateVScrollBar ((SlatE) hfp->doc);
  Reset (hfp->list);
  Reset (hfp->doc);
  if (hfp->file == NULL || hfp->file [0] == '\0') return FALSE;
  if (hfp->mainStrings == NULL || hfp->indexStrings == NULL) return FALSE;
  ObjectRect (hfp->doc, &r);
  InsetRect (&r, 4, 4);
  lstColFmt.pixWidth = r.right - r.left;
  hdgColFmt.pixWidth = r.right - r.left;
  subColFmt.pixWidth = r.right - r.left;
  /*
  tblColFmt.pixWidth = screenRect.right - screenRect.left;
  */
  tblColFmt.pixWidth = r.right - r.left;
  tblColFmt.pixInset = 10;
  txtColFmt.pixWidth = r.right - r.left;
  txtColFmt.pixInset = 10;

  for (vnp = hfp->indexStrings; vnp != NULL; vnp = vnp->next) {
    lstColFmt.pixInset = vnp->choice;
    AppendText (hfp->list, vnp->data.ptrvalue, &lstParFmt, &lstColFmt, programFont);
  }

  for (vnp = hfp->mainStrings; vnp != NULL; vnp = vnp->next) {
    switch (vnp->choice) {
      case TBL_FMT :
        AppendText (hfp->doc, vnp->data.ptrvalue, &tblParFmt, &tblColFmt, programFont);
        break;
      case TXT_FMT :
        AppendText (hfp->doc, vnp->data.ptrvalue, &txtParFmt, &txtColFmt, programFont);
        break;
      case HDG_FMT :
        AppendText (hfp->doc, vnp->data.ptrvalue, &hdgParFmt, &hdgColFmt, systemFont);
        break;
      case SUB_FMT :
        subColFmt.pixInset = 0;
        AppendText (hfp->doc, vnp->data.ptrvalue, &subParFmt, &subColFmt, programFont);
        break;
      default :
        subColFmt.pixInset = vnp->choice;
        AppendText (hfp->doc, vnp->data.ptrvalue, &subParFmt, &subColFmt, programFont);
        break;
    }
  }

  UpdateDocument (hfp->list, 0, 0);
  /*
  UpdateDocument (hfp->doc, 0, 0);
  */
  text = GetDocText (hfp->doc, firstShown, 0, 0);
  MemFree (text);
  AdjustDocScroll (hfp->doc);
  GetItemParams4 (hfp->doc, firstShown, &startsAt, NULL, NULL, NULL, NULL);
  CorrectBarValue (sb, startsAt + firstLine);
  Show (hfp->list);
  Show (hfp->doc);
  Update ();
  return TRUE;
}

static void RefreshHelpForm (ButtoN b)

{
  HelpFormPtr  hfp;

  hfp = (HelpFormPtr) GetObjectExtra (b);
  if (hfp == NULL) return;
  ParseHelpFile (hfp, TRUE);
  PopulateHelpForm (hfp);
  Update ();
}

static void CleanupHelpForm (GraphiC g, VoidPtr data)

{
  ResetHelpLists ((HelpFormPtr) data);
  StdCleanupFormProc (g, data);
}

static void HelpListNotify (DoC d, Int2 item, Int2 row, Int2 col, Boolean dblclick)

{
  HelpFormPtr   hfp;
  HelpIndexPtr  hip;
  BaR           sb;
  Int2          startsAt;
  ValNodePtr    vnp;

  hfp = (HelpFormPtr) GetObjectExtra (d);
  if (hfp == NULL || hfp->doc == NULL) return;
  if (item == 0 || row == 0 || col == 0) return;
  vnp = hfp->index;
  while (vnp != NULL && item > 1) {
    item--;
    vnp = vnp->next;
  }
  if (vnp != NULL) {
    hip = (HelpIndexPtr) vnp->data.ptrvalue;
    if (hip != NULL) {
      GetItemParams (hfp->doc, hip->item, &startsAt, NULL, NULL, NULL, NULL);
      ResetClip ();
      sb = GetSlateVScrollBar ((SlatE) hfp->doc);
      SetValue (sb, startsAt);
      Update ();
    }
  }
}

static void ResizeHelpForm (WindoW w)

{
  Int2         delta;
  Int2         diff;
  Int2         gap;
  Int2         height;
  HelpFormPtr  hfp;
  RecT         r;
  RecT         s;
  RecT         t;
  Int2         width;

  hfp = (HelpFormPtr) GetObjectExtra (w);
  if (hfp != NULL) {
    ObjectRect (w, &r);
    width = r.right - r.left;
    height = r.bottom - r.top;
    GetPosition (hfp->doc, &s);
    GetPosition (hfp->dismissGrp, &t);
    diff = t.bottom - t.top;
    gap = t.top - s.bottom;
    t.bottom = height - s.left;
    t.top = t.bottom - diff;
    delta = (width - t.right - t.left) / 2;
    t.left += delta;
    t.right += delta;
    s.right = width - s.left;
    /*
    s.bottom = height - s.left;
    */
    s.bottom = t.top - gap;
    SetPosition (hfp->dismissGrp, &t);
    SetPosition (hfp->doc, &s);
    AdjustPrnt (hfp->doc, &s, FALSE);
    PopulateHelpForm (hfp);
    Update ();
  }
}

static Boolean ScrollToTextInDoc (DoC d, Int2 item, CharPtr text)

{
  Pointer  data;
  Boolean  found;
  RecT     rct;
  BaR      sb;
  Int4     startsAt;
  CharPtr  str;
  WindoW   tempPort;

  found = FALSE;
  GetItemParams4 (d, item, &startsAt, NULL, NULL, NULL, &data);
  str = (CharPtr) data;
  if (StringISearch (str, text) != NULL) {
    found = TRUE;
  }
  if (found) {
    tempPort = SavePort (d);
    Select (d);
    sb = GetSlateVScrollBar ((SlatE) d);
    CorrectBarValue (sb, startsAt);
    ObjectRect (d, &rct);
    InsetRect (&rct, 4, 4);
    InsetRect (&rct, -1, -1);
    InvalRect (&rct);
    RestorePort (tempPort);
    Update ();
  }
  return found;
}

static void FindHelpBtnProc (ButtoN b)

{
  Int2         firstShown;
  HelpFormPtr  hfp;
  Int2         i;
  Int2         numItems;
  Char         str [256];

  hfp = (HelpFormPtr) GetObjectExtra (b);
  if (hfp == NULL) return;
  GetTitle (hfp->findTxt, str, sizeof (str) - 1);
  GetDocParams (hfp->doc, &numItems, NULL);
  if (GetScrlParams (hfp->doc, NULL, &firstShown, NULL)) {
    for (i = firstShown + 1; i <= numItems; i++) {
      if (ScrollToTextInDoc (hfp->doc, i, str)) {
        return;
      }
    }
    for (i = 1; i < firstShown; i++) {
      if (ScrollToTextInDoc (hfp->doc, i, str)) {
        return;
      }
    }
  }
}

static void FindHelpTextProc (TexT t)

{
  HelpFormPtr  hfp;

  hfp = (HelpFormPtr) GetObjectExtra (t);
  if (hfp == NULL) return;
  if (TextLength (t) > 0) {
    SafeEnable (hfp->findBtn);
  } else {
    SafeDisable (hfp->findBtn);
  }
}

static void HelpFormMessage (ForM f, Int2 mssg)

{
  FILE         *fp;
  HelpFormPtr  hfp;
  Char         path [PATH_MAX];

  hfp = (HelpFormPtr) GetObjectExtra (f);
  if (hfp != NULL) {
    switch (mssg) {
      case VIB_MSG_CLOSE :
        Hide (f);
        break;
      case VIB_MSG_PRINT :
        PrintDocument (hfp->doc);
        break;
      case VIB_MSG_EXPORT :
        if (GetOutputFileName (path, sizeof (path), "help.txt")) {
          WatchCursor ();
#ifdef WIN_MAC
          fp = FileOpen (path, "r");
          if (fp != NULL) {
            FileClose (fp);
          } else {
            FileCreate (path, "TEXT", "ttxt");
          }
#endif
          fp = FileOpen (path, "w");
          if (fp != NULL) {
            SaveDocument (hfp->doc, fp);
            FileClose (fp);
          }
          ArrowCursor ();
        }
        break;
      default :
        if (hfp->appmessage != NULL) {
          hfp->appmessage (f, mssg);
        }
        break;
    }
  }
}

static void HelpFormActivate (WindoW w)

{
  IteM         exportItm;
  HelpFormPtr  hfp;

  hfp = (HelpFormPtr) GetObjectExtra (w);
  if (hfp != NULL) {
    if (hfp->activate != NULL) {
      hfp->activate (w);
    }
    exportItm = FindFormMenuItem ((BaseFormPtr) hfp, VIB_MSG_EXPORT);
    SafeSetTitle (exportItm, "Export Help...");
  }
}

extern ForM CreateHelpForm (Int2 left, Int2 top, CharPtr title,
                            CharPtr file, BtnActnProc closeForm,
                            WndActnProc activateForm)

{
  ButtoN             b;
  GrouP              c;
  GrouP              h;
  Int2               height;
  HelpFormPtr        hfp;
  StdEditorProcsPtr  sepp;
  WindoW             w;
#ifndef WIN_MAC
  MenU               m;
#endif

  w = NULL;
  hfp = MemNew (sizeof (HelpForm));
  if (hfp != NULL) {
    w = DocumentWindow (left, top, -10, -10, title,
                        StdSendCloseWindowMessageProc, ResizeHelpForm);
    SetObjectExtra (w, hfp, CleanupHelpForm);
    hfp->form = (ForM) w;
    hfp->formmessage = HelpFormMessage;

    sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
    if (sepp != NULL) {
      hfp->appmessage = sepp->handleMessages;
    }

    StringNCpy_0 (hfp->file, file, sizeof (hfp->file));

#ifndef WIN_MAC
    m = PulldownMenu (w, "File");
    FormCommandItem (m, "Export", (BaseFormPtr) hfp, VIB_MSG_EXPORT);
    SeparatorItem (m);
#ifdef WIN_MSWIN
    FormCommandItem (m, "Print", (BaseFormPtr) hfp, VIB_MSG_PRINT);
    SeparatorItem (m);
#endif
    FormCommandItem (m, "Close", (BaseFormPtr) hfp, VIB_MSG_CLOSE);
#endif

    c = HiddenGroup (w, 4, 0, NULL);
    StaticPrompt (c, "Find", 0, dialogTextHeight, programFont, 'l');
    hfp->findTxt = DialogText (c, "", 20, FindHelpTextProc);
    SetObjectExtra (hfp->findTxt, hfp, NULL);
    hfp->findBtn = PushButton (c, "Find", FindHelpBtnProc);
    SetObjectExtra (hfp->findBtn, hfp, NULL);
    Disable (hfp->findBtn);

    h = HiddenGroup (w, -1, 0, NULL);
    SelectFont (programFont);
    height = LineHeight ();
    SelectFont (systemFont);
    hfp->list = DocumentPanel (h, stdCharWidth * 28, height * 5);
    SetDocAutoAdjust (hfp->list, FALSE);
    SetObjectExtra (hfp->list, hfp, NULL);
    SetDocNotify (hfp->list, HelpListNotify);

    hfp->doc = DocumentPanel (h, stdCharWidth * 33, stdLineHeight * 20);
    SetDocAutoAdjust (hfp->doc, FALSE);
    SetObjectExtra (hfp->doc, hfp, NULL);

    hfp->dismissGrp = HiddenGroup (w, 2, 0, NULL);
    PushButton (hfp->dismissGrp, "Dismiss", closeForm);
    if (indexerVersion) {
      b = PushButton (hfp->dismissGrp, "Refresh", RefreshHelpForm);
      SetObjectExtra (b, hfp, NULL);
    }

    AlignObjects (ALIGN_CENTER, (HANDLE) hfp->doc, (HANDLE) hfp->dismissGrp, NULL);

    RealizeWindow (w);

    SetupHelpFonts ();

    if (activateForm != NULL) {
      hfp->activate = activateForm;
    } else {
      if (sepp != NULL) {
        hfp->activate = sepp->activateForm;
      }
    }
    SetActivate (w, HelpFormActivate);
    HelpFormActivate ((WindoW) hfp->form);

    if (ParseHelpFile (hfp, FALSE)) {
      if (! PopulateHelpForm (hfp)) {
        w = Remove (w);
      }
    } else {
      w = Remove (w);
    }
  }
  return (ForM) w;
}

extern void SendHelpScrollMessage (ForM f, CharPtr heading, CharPtr section)

{
  HelpFormPtr   hfp;
  HelpIndexPtr  hip;
  BaR           sb;
  Int2          startsAt;
  CharPtr       str;
  Char          txt [256];
  Boolean       useBoth;
  Boolean       useHeading;
  Boolean       useSection;
  ValNodePtr    vnp;

  hfp = (HelpFormPtr) GetObjectExtra (f);
  if (hfp != NULL) {
    vnp = hfp->index;
    txt [0] = '\0';
    useBoth = FALSE;
    useHeading = FALSE;
    useSection = FALSE;
    if (heading != NULL && *heading != '\0' && section != NULL && *section != '\0') {
      useBoth = TRUE;
      if (StringLen (heading) + StringLen (section) < sizeof (txt) - 2) {
        StringCpy (txt, heading);
        StringCat (txt, "|");
        StringCat (txt, section);
      }
    } else if (heading != NULL && *heading != '\0') {
      useHeading = TRUE;
      StringNCpy_0 (txt, heading, sizeof (txt));
    } else if (section != NULL && *section != '\0') {
      useSection = TRUE;
      StringNCpy_0 (txt, section, sizeof (txt));
    }
    while (vnp != NULL) {
      hip = (HelpIndexPtr) vnp->data.ptrvalue;
      if (hip != NULL) {
        if (useBoth) {
          str = hip->combined;
        } else if (useHeading) {
          str = hip->heading;
        } else if (useSection) {
          str = hip->section;
        } else {
          str = NULL;
        }
        if (str != NULL && StringICmp (txt, str) == 0) {
          GetItemParams (hfp->doc, hip->item, &startsAt, NULL, NULL, NULL, NULL);
          sb = GetSlateVScrollBar ((SlatE) hfp->doc);
          ResetClip ();
          SetValue (sb, startsAt);
          Update ();
          return;
        }
      }
      vnp = vnp->next;
    }
  }
}

