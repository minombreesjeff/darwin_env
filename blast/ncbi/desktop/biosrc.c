/*   biosrc.c
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
* File Name:  biosrc.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   1/22/95
*
* $Revision: 6.63 $
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

#include <biosrc.h>
#include <document.h>
#include <gather.h>
#include <subutil.h>

extern EnumFieldAssoc  orgmod_subtype_alist [];
ENUM_ALIST(orgmod_subtype_alist)
  {" ",                 0},
  {"Acronym",          19},
  {"Anamorph",         29},
  {"Authority",        24},
  {"Biotype",          14},
  {"Biovar",           13},
  {"Breed",            31},
  {"Chemovar",         12},
  {"Common",           18},
  {"Dosage",           20},
  {"Cultivar",         10},
  {"Ecotype",          27},
  {"Forma",            25},
  {"Forma-specialis",  26},
  {"Group",            15},
  {"Isolate",          17},
  {"Old Lineage",     253},
  {"Old Name",        254},
  {"Pathovar",         11},
  {"Serogroup",         8},
  {"Serotype",          7},
  {"Serovar",           9},
  {"Specific-host",    21},
  {"Specimen-voucher", 23},
  {"Strain",            2},
  {"Subgroup",         16},
  {"Sub-species",      22},
  {"Substrain",         3},
  {"Subtype",           5},
  {"Synonym",          28},
  {"Teleomorph",       30},
  {"Type",              4},
  {"Variety",           6},
END_ENUM_ALIST

extern EnumFieldAssoc  subsource_subtype_alist [];
ENUM_ALIST(subsource_subtype_alist)
  {" ",                      0},
  {"Cell-line",              8},
  {"Cell-type",              9},
  {"Chromosome",             1},
  {"Clone",                  3},
  {"Clone-lib",             11},
  {"Collected-by",          31},
  {"Collection-date",       30},
  {"Country",               23},
  {"Dev-stage",             12},
  {"Endogenous-virus-name", 25},
  {"Environmental-sample",  27},
  {"Frequency",             13},
  {"Fwd-primer-name",       35},
  {"Fwd-primer-seq",        33},
  {"Genotype",               6},
  {"Germline",              14},
  {"Haplotype",              5},
  {"Identified-by",         32},
    /*
  {"Ins-seq-name",          21},
    */
  {"Isolation-source",      28},
  {"Lab-host",              16},
  {"Lat-Lon",               29},
  {"Map",                    2},
  {"Plasmid-name",          19},
  {"Plastid-name",          22},
  {"Pop-variant",           17},
  {"Rearranged",            15},
  {"Rev-primer-name",       36},
  {"Rev-primer-seq",        34},
  {"Segment",               24},
  {"Sex",                    7},
  {"Subclone",               4},
  {"Tissue-lib",            18},
  {"Tissue-type",           10},
  {"Transgenic",            26},
    /*
  {"Transposon-name",       20},
    */
END_ENUM_ALIST

static ENUM_ALIST(biosource_genome_alist)
  {" ",                    0},
  {"Genomic",              1},
  {"Chloroplast",          2},
  {"Chromoplast",          3},
  {"Kinetoplast",          4},
  {"Mitochondrion",        5},
  {"Plastid",              6},
  {"Macronuclear",         7},
  {"Extrachromosomal",     8},
  {"Plasmid",              9},
    /*
  {"Transposon",          10},
  {"Insertion Sequence",  11},
    */
  {"Cyanelle",            12},
  {"Proviral",            13},
  {"Virion",              14},
  {"Nucleomorph",         15},
  {"Apicoplast",          16},
  {"Leucoplast",          17},
  {"Proplastid",          18},
  {"Endogenous-virus",    19},
END_ENUM_ALIST

extern EnumFieldAssoc  biosource_genome_simple_alist [];
ENUM_ALIST(biosource_genome_simple_alist)
  {" ",                    0},
  {"Genomic",              1},
  {"Chloroplast",          2},
  {"Kinetoplast",          4},
  {"Mitochondrion",        5},
  {"Plastid",              6},
  {"Macronuclear",         7},
  {"Extrachromosomal",     8},
  {"Plasmid",              9},
    /*
  {"Transposon",          10},
  {"Insertion Sequence",  11},
    */
  {"Cyanelle",            12},
  {"Proviral",            13},
  {"Virion",              14},
  {"Nucleomorph",         15},
  {"Apicoplast",          16},
  {"Leucoplast",          17},
  {"Proplastid",          18},
  {"Endogenous-virus",    19},
END_ENUM_ALIST

extern EnumFieldAssoc  biosource_origin_alist [];
extern ENUM_ALIST(biosource_origin_alist)
  {" ",               0},
  {"Natural",         1},
  {"Natural Mutant",  2},
  {"Mutant",          3},
  {"Artificial",      4},
  {"Synthetic",       5},
  {"Other",         255},
END_ENUM_ALIST

Int2     numGeneticCodes = 0;
Int2     gcIdToIndex [NUM_GENETIC_CODES];
Int2     gcIndexToId [NUM_GENETIC_CODES];
CharPtr  gcNames [NUM_GENETIC_CODES];

static CharPtr  orgTxtPtr = NULL;
static CharPtr  PNTR orgStrIdx = NULL;
static Int2     orgNum = 0;

#define ORGANISM_PAGE         0
#define MODIFIERS_PAGE        1
#define MISCELLANEOUS_PAGE    2
#define COMMON_PAGE           3
#define LOCATION_PAGE         4

#define NUM_PAGES  8

typedef struct genbiopage {
  DIALOG_MESSAGE_BLOCK
  TexT            taxName;
  Handle          commonName;
  Boolean         typedSciName;
  Boolean         typedComName;
  Int2            selectedOrg;
  Int2            clickedOrg;
  DoC             orglist;
  Int2            nuclGC;
  Int2            mitoGC;
  Int4            taxID;
  PopuP           genome;
  PopuP           origin;
  ButtoN          is_focus;
  PopuP           simplecode;
  PopuP           gcode;
  PopuP           mgcode;
  DialoG          orgmod;
  TexT            orgcomment;
  DialoG          subsource;
  TexT            subcomment;
  ButtoN          add_type_strain_to_orgcomment;
  TexT            lineage;
  TexT            gbDiv;
  DialoG          db;
  DialoG          syn;
  DialoG          mod;
  GrouP           orgGrp [5];
  GrouP           modGrp [5];
  GrouP           miscGrp [5];
  PrompT          gbacr;
  PrompT          gbana;
  PrompT          gbsyn;
  CharPtr         origTaxName;
  Boolean         stripOldName;
  EnumFieldAssoc  PNTR genomeAlist;
  Uint1           orgname_choice;
  Pointer         orgname_data;
} GenBioPage, PNTR GenBioPagePtr;

typedef struct genbioform {
  FEATURE_FORM_BLOCK
  SeqEntryPtr   sep;
  GrouP         pages [NUM_PAGES];
  DialoG        foldertabs;
  Int2          currentPage;

  LookupTaxonomyProc  lookupTaxonomy;
} GenBioForm, PNTR GenBioFormPtr;

extern Boolean LoadOrganismTable (void)

{
  Char     ch;
  FILE     *f;
  Boolean  failed;
  Char     first [16];
  Int2     idx;
  Int4     len;
  Int2     num;
  CharPtr  ptr;
  Char     str [PATH_MAX];
  CharPtr  tmp;
  Int2     version;
#ifdef WIN_MAC
  CharPtr  p;
#endif
#if (defined(OS_DOS) || defined (OS_NT))
  CharPtr  p;
  CharPtr  q;
#endif

  orgTxtPtr = NULL;
  orgStrIdx = NULL;
  orgNum = 0;
  failed = TRUE;
  ProgramPath (str, sizeof (str));
  ptr = StringRChr (str, DIRDELIMCHR);
  if (ptr != NULL) {
    *ptr = '\0';
    FileBuildPath (str, NULL, "taxlist.txt");
    len = FileLength (str);
    f = FileOpen (str, "r");
    if (f == NULL) {
      if (GetAppParam ("NCBI", "NCBI", "DATA", "", str, sizeof (str))) {
        FileBuildPath (str, NULL, "taxlist.txt");
        len = FileLength (str);
        f = FileOpen (str, "r");
      }
    }
    if (f != NULL) {
      ptr = MemNew ((size_t) (len + 5));
      if (ptr != NULL) {
        orgTxtPtr = ptr;
        FileRead (ptr, (size_t) len, 1, f);
#if (defined(OS_DOS) || defined (OS_NT))
        p = ptr;
        q = ptr;
        while (*p) {
          if (*p == '\r') {
            p++;
          } else {
            *q = *p;
            p++;
            q++;
          }
        }
        *q = '\0';
#endif
#ifdef WIN_MAC
        p = ptr;
        while (*p) {
          if (*p == '\r') {
            *p = '\n';
          }
          p++;
        }
#endif
        tmp = ptr;
        ch = *tmp;
        while (ch != '\0' && ch != '\n') {
          tmp++;
          ch = *tmp;
        }
        *tmp = '\0';
        StringNCpy_0 (first, ptr, sizeof (first) - 1);
        *tmp = ch;
        if (StrToInt (first, &version)) {
          tmp++;
          ptr = tmp;
        }
        num = 0;
        tmp = ptr;
        ch = *tmp;
        while (ch != '\0') {
          if (ch == '\n') {
            num++;
          }
          tmp++;
          ch = *tmp;
        }
        orgStrIdx = MemNew (sizeof (CharPtr) * (size_t) (num + 3));
        if (orgStrIdx != NULL) {
          idx = 0;
          tmp = ptr;
          ch = *tmp;
          orgStrIdx [idx] = tmp;
          while (ch != '\0') {
            if (ch == '\n') {
              idx++;
              tmp++;
              ch = *tmp;
              orgStrIdx [idx] = tmp;
            } else {
              tmp++;
              ch = *tmp;
            }
          }
          orgNum = num;
          failed = FALSE;
        }
      }
      FileClose (f);
    }
  }
  if (failed) {
    orgTxtPtr = MemFree (orgTxtPtr);
    orgStrIdx = MemFree (orgStrIdx);
    return FALSE;
  } else {
    return TRUE;
  }
}

extern void FreeOrganismTable (void)

{
  orgTxtPtr = MemFree (orgTxtPtr);
  orgStrIdx = MemFree (orgStrIdx);
}

extern void SetupGeneticCodes (void)

{
  Char            ch;
  GeneticCodePtr  codes;
  GeneticCodePtr  gcp;
  Int2            i;
  Int4            id;
  Int2            j;
  Int2            index;
  Char            name [64];
  CharPtr         ptr;
  Char            str [256];
  ValNodePtr      tmp;

  numGeneticCodes = 0;
  for (i = 0; i < NUM_GENETIC_CODES; i++) {
    gcIndexToId [i] = 0;
    gcIdToIndex [i] = 1;
    gcNames [i] = NULL;
  }
  index = 1;
  codes = GeneticCodeTableLoad ();
  if (codes != NULL) {
    for (gcp = codes; gcp != NULL; gcp = gcp->next) {
      id = 0;
      str [0] = '\0';
      for (tmp = (ValNodePtr) gcp->data.ptrvalue; tmp != NULL; tmp = tmp->next) {
        switch (tmp->choice) {
          case 1 :
            if (StringLen (str) < 1) {
              StringNCpy_0 (str, (CharPtr) tmp->data.ptrvalue, sizeof (str));
              ptr = str;
              ch = *ptr;
              while (ch != '\0') {
                if (ch == '/') {
                  *ptr = '-';
                }
                ptr++;
                ch = *ptr;
              }
            }
            break;
          case 2 :
            id = tmp->data.intvalue;
            break;
          default :
            break;
        }
      }
      if (id != 7 && id != 8) {
        if (id > 0 /* && id < 30 */ ) {
          i = 0;
          if (StringLen (str + i) > 0 && index < NUM_GENETIC_CODES - 1) {
            ch = str [i];
            while (ch == ' ' || ch == ';') {
              i++;
              ch = str [i];
            }
            j = 0;
            ch = str [i + j];
            while (ch != '\0' && ch != ';') {
              name [j] = ch;
              j++;
              ch = str [i + j];
            }
            name [j] = '\0';
            i += j;
            index++;
            if (ch == ';') {
              StringCat (name, ", etc.");
            }
            gcIndexToId [index] = id;
            gcIdToIndex [id] = index;
            gcNames [index] = StringSave (name);
          }
        }
      }
    }
  }
  numGeneticCodes = index;
}

extern void FreeGeneticCodes (void)

{
  Int2  i;

  for (i = 0; i < NUM_GENETIC_CODES; i++) {
    gcNames [i] = MemFree (gcNames [i]);
  }
}

extern ValNodePtr GetGeneticCodeValNodeList (void)
{
  ValNodePtr gencodelist = NULL;
  Int4       index;
  
  for (index = 0; index < numGeneticCodes; index++)
  {
    if (StringHasNoText (gcNames[index]))
    {
      continue;
    }
    ValNodeAddPointer (&gencodelist, gcIndexToId [index], StringSave (gcNames[index]));
  }
  return gencodelist;
}

static void CopyField (CharPtr str, size_t max, CharPtr source, Int2 col)

{
  Char     ch;
  size_t   count;
  CharPtr  ptr;

  if (str != NULL && max > 0 && source != NULL) {
    MemSet (str, 0, max);
      ptr = source;
      ch = *ptr;
      while (col > 1 && ch != '\n' && ch != '\0') {
        while (ch != '\t' && ch != '\n' && ch != '\0') {
          ptr++;
          ch = *ptr;
        }
        if (ch == '\t') {
          ptr++;
          ch = *ptr;
        }
        col--;
      }
      count = 0;
      ch = ptr [count];
      while (ch != '\t' && ch != '\n' && ch != '\0') {
        count++;
        ch = ptr [count];
      }
      max = MIN (max, count);
      StringNCpy (str, ptr, max); /* remains StringNCpy, not _0 */
  }
}

static void CopyStrFromTaxPtr (CharPtr str, size_t max, Int2 row, Int2 col)

{
  CharPtr  source;

  if (str != NULL && max > 0) {
    MemSet (str, 0, max);
    if (orgTxtPtr != NULL && orgStrIdx != NULL && row > 0) {
      source = orgStrIdx [row - 1];
      CopyField (str, max, source, col);
    }
  }
}

static Int2 FindTaxText (CharPtr text, Int2 num)

{
  Int2  compare;
  Int2  left;
  Int2  mid;
  Int2  right;
  Char  str [256];

  mid = 0;
  if (text != NULL && num > 0) {
    left = 1;
    right = num;
    while (left <= right) {
      mid = (left + right) / 2;
      CopyStrFromTaxPtr (str, sizeof (str) - 2, mid, 1);
      compare = StringICmp (text, str);
      if (compare <= 0) {
        right = mid - 1;
      }
      if (compare >= 0) {
        left = mid + 1;
      }
    }
    if (left <= right + 1) {
      CopyStrFromTaxPtr (str, sizeof (str) - 2, mid, 1);
      str [StringLen (text)] = '\0';
      compare = StringICmp (text, str);
      if (compare > 0) {
        mid++;
        if (mid <= num) {
          CopyStrFromTaxPtr (str, sizeof (str) - 2, mid, 1);
          str [StringLen (text)] = '\0';
          compare = StringICmp (text, str);
          if (compare > 0) {
            mid = 0;
          }
        }
      }
    }
  }
  return mid;
}

extern Boolean BioSourceDialogToGenBankDivision (DialoG d, CharPtr div, size_t maxsize)

{
  GenBioPagePtr  gbp;

  if (div == NULL || maxsize < 1) return FALSE;
  div [0] = '\0';
  gbp = (GenBioPagePtr) GetObjectExtra (d);
  if (gbp != NULL) {
    GetTitle (gbp->gbDiv, div, maxsize);
    if (! StringHasNoText (div)) return TRUE;
  }
  return FALSE;
}

static void PopulateGeneticCodePopup (PopuP gc)

{
  Int2  i;

   if (gc != NULL) {
    PopupItem (gc, " ");
    for (i = 1; i <= numGeneticCodes; i++) {
      PopupItem (gc, gcNames [i]);
    }
  }
}

static void ChangeGencodePopups (GenBioPagePtr gbp)

{
  UIEnum  genome;

  if (gbp != NULL) {
    if (gbp->simplecode != NULL) {
      if (GetEnumPopup (gbp->genome, gbp->genomeAlist, &genome)) {
        if (genome == 4 || genome == 5) {
          SafeSetValue (gbp->simplecode, gcIdToIndex [gbp->mitoGC]);
        } else if (genome == GENOME_chloroplast ||
                   genome == GENOME_chromoplast ||
                   genome == GENOME_plastid ||
                   genome == GENOME_cyanelle ||
                   genome == GENOME_apicoplast ||
                   genome == GENOME_leucoplast ||
                   genome == GENOME_proplastid) {
          SafeSetValue (gbp->simplecode, gcIdToIndex [11]);
        } else {
          SafeSetValue (gbp->simplecode, gcIdToIndex [gbp->nuclGC]);
        }
      }
      SafeSetValue (gbp->gcode, gcIdToIndex [gbp->nuclGC]);
      SafeSetValue (gbp->mgcode, gcIdToIndex [gbp->mitoGC]);
    } else {
      SafeSetValue (gbp->gcode, gcIdToIndex [gbp->nuclGC]);
      SafeSetValue (gbp->mgcode, gcIdToIndex [gbp->mitoGC]);
    }
  }
}

static void SetCodes (GenBioPagePtr gbp, Int2 row, Boolean changepopups)

{
  Char  str [256];

  if (gbp != NULL && row > 0) {
    CopyStrFromTaxPtr (str, sizeof (str) - 2, row, 3);
    StrToInt (str, &gbp->nuclGC);
    CopyStrFromTaxPtr (str, sizeof (str) - 2, row, 4);
    StrToInt (str, &gbp->mitoGC);
    CopyStrFromTaxPtr (str, sizeof (str) - 2, row, 5);
    SafeSetTitle (gbp->gbDiv, str);
    CopyStrFromTaxPtr (str, sizeof (str) - 2, row, 6);
    StrToLong (str, &gbp->taxID);
    if (changepopups) {
      ChangeGencodePopups (gbp);
    }
  }
}

extern void SetGenome (PopuP p);
void SetGenome (PopuP p)

{
  GenBioPagePtr  gbp;

  gbp = (GenBioPagePtr) GetObjectExtra (p);
  if (gbp != NULL) {
    ChangeGencodePopups (gbp);
  }
}

static void AutoScrollTax (GenBioPagePtr gbp, TexT t, Boolean isSciName,
                           Boolean setText, Boolean changepopups)

{
  Int2  num;
  Int2  oldOrg;
  Int2  row;
  Char  str [256];
  Char  txt [256];

  if (gbp != NULL && t != NULL) {
    gbp->nuclGC = 0;
    gbp->mitoGC = 0;
    gbp->taxID = 0;
    SafeSetTitle (gbp->gbDiv, "");
    GetTitle (t, str, sizeof (str) - 2);
    if (str [0] == '\0') {
      if (isSciName) {
        gbp->typedSciName = FALSE;
      } else {
        gbp->typedComName = FALSE;
      }
    }
    num = 0;
    oldOrg = gbp->selectedOrg;
    GetItemParams (gbp->orglist, 1, NULL, &num, NULL, NULL, NULL);
    if (num > 0) {
      row = FindTaxText (str, num);
      if (row > 0 && row <= num) {
        if (! RowIsVisible (gbp->orglist, 1, row, NULL, NULL)) {
          SetOffset (gbp->orglist, 0, row - 1);
        }
        CopyStrFromTaxPtr (txt, sizeof (txt) - 2, row, 1);
        if (StringICmp (txt, str) != 0) {
          if (setText) {
            if (isSciName) {
              if (! gbp->typedComName) {
                SafeSetTitle (gbp->commonName, "");
              }
            } else {
              if (! gbp->typedSciName) {
                /*
                SafeSetTitle (gbp->taxName, "");
                */
              }
            }
          }
          gbp->selectedOrg = 0;
          InvalDocRows (gbp->orglist, 1, oldOrg, oldOrg);
          if (changepopups) {
            ChangeGencodePopups (gbp);
          }
          return;
        }
        if (isSciName) {
          CopyStrFromTaxPtr (str, sizeof (str) - 2, row, 2);
          if (! gbp->typedComName) {
            if (setText) {
              SafeSetTitle (gbp->commonName, str);
            }
          } else {
            GetTitle (gbp->commonName, txt, sizeof (txt));
            if (StringICmp (txt, str) != 0) {
              if (changepopups) {
                ChangeGencodePopups (gbp);
              }
              return;
            }
          }
        } else {
          if (changepopups) {
            ChangeGencodePopups (gbp);
          }
          return;
        }
        SetCodes (gbp, row, changepopups);
        if (oldOrg != row) {
          gbp->selectedOrg = 0;
          InvalDocRows (gbp->orglist, 1, oldOrg, oldOrg);
          gbp->selectedOrg = row;
          InvalDocRows (gbp->orglist, 1, row, row);
        }
      }
    }
  }
}

static void FreeGenBioOrgNameData (GenBioPagePtr gbp);

static void TaxNameText (TexT t)

{
  DbtagPtr         dbt;
  GenBioPagePtr    gbp;
  ValNodePtr       head;
  ValNodePtr       nextvnp;
  ValNodePtr PNTR  prevvnp;
  ValNodePtr       vnp;

  gbp = (GenBioPagePtr) GetObjectExtra (t);
  if (gbp != NULL) {
    gbp->typedSciName = TRUE;
    AutoScrollTax (gbp, t, TRUE, TRUE, TRUE);
    SafeSetTitle (gbp->lineage, "");
    SafeSetTitle (gbp->gbDiv, "");
    head = DialogToPointer (gbp->db);
    if (head != NULL) {
      prevvnp = &head;
      vnp = head;
      while (vnp != NULL) {
        nextvnp = vnp->next;
        dbt = (DbtagPtr) vnp->data.ptrvalue;
        if (dbt != NULL) {
          if (StringICmp (dbt->db, "taxon") == 0) {
            *prevvnp = vnp->next;
            vnp->next = NULL;
            DbtagFree (dbt);
            ValNodeFree (vnp);
            FreeGenBioOrgNameData (gbp);
          } else {
            prevvnp = (ValNodePtr PNTR) &(vnp->next);
          }
        }
        vnp = nextvnp;
      }
      PointerToDialog (gbp->db, head);
    }
  }
}

static void CommonNameText (TexT t)

{
  GenBioPagePtr  gbp;

  gbp = (GenBioPagePtr) GetObjectExtra (t);
  if (gbp != NULL) {
    /*
    gbp->typedComName = TRUE;
    AutoScrollTax (gbp, t, FALSE, TRUE, TRUE);
    */
  }
}

static void ClickTaxName (DoC d, PoinT pt)

{
  GenBioPagePtr  gbp;
  Int2           item;
  Int2           oldOrg;
  Int2           row;

  gbp = (GenBioPagePtr) GetObjectExtra (d);
  if (gbp != NULL) {
    MapDocPoint (d, pt, &item, &row, NULL, NULL);
    if (item > 0 && row > 0) {
      gbp->clickedOrg = row;
      oldOrg = gbp->selectedOrg;
      if (gbp->selectedOrg != row) {
        gbp->selectedOrg = 0;
        InvalDocRows (d, 1, oldOrg, oldOrg);
        gbp->selectedOrg = row;
        InvalDocRows (d, 1, row, row);
      }
    }
  }
}

static void ReleaseTaxName (DoC d, PoinT pt)

{
  GenBioPagePtr  gbp;
  Int2           item;
  Int2           row;
  Char           str [256];

  gbp = (GenBioPagePtr) GetObjectExtra (d);
  if (gbp != NULL) {
    gbp->nuclGC = 0;
    gbp->mitoGC = 0;
    gbp->taxID = 0;
    SafeSetTitle (gbp->gbDiv, "");
    MapDocPoint (d, pt, &item, &row, NULL, NULL);
    if (item > 0 && row > 0 && row == gbp->clickedOrg) {
      ResetClip ();
      if (row == gbp->clickedOrg) {
        CopyStrFromTaxPtr (str, sizeof (str) - 2, row, 1);
        SafeSetTitle (gbp->taxName, str);
        CopyStrFromTaxPtr (str, sizeof (str) - 2, row, 2);
        SafeSetTitle (gbp->commonName, str);
        Select (gbp->taxName);
        TaxNameText (gbp->taxName);
        SetCodes (gbp, row, TRUE);
      }
    }
  }
}

extern Boolean SetBioSourceDialogTaxName (DialoG d, CharPtr taxname)

{
  GenBioPagePtr gbp;
  Int2          num;
  Int2          oldOrg;
  Int2          row;
  Char          str [256];

  gbp = (GenBioPagePtr) GetObjectExtra (d);
  if (gbp == NULL) return FALSE;
  num = 0;
  oldOrg = gbp->selectedOrg;
  GetItemParams (gbp->orglist, 1, NULL, &num, NULL, NULL, NULL);
  if (num > 0) {
    row = FindTaxText (taxname, num);
    if (row > 0 && row <= num) {
      if (! RowIsVisible (gbp->orglist, 1, row, NULL, NULL)) {
        SetOffset (gbp->orglist, 0, row - 1);
      }
      CopyStrFromTaxPtr (str, sizeof (str) - 2, row, 1);
      if (StringICmp (str, taxname) != 0) {
        SafeSetTitle (gbp->commonName, "");
        SafeSetTitle (gbp->taxName, taxname);
        gbp->selectedOrg = 0;
        InvalDocRows (gbp->orglist, 1, oldOrg, oldOrg);
        ChangeGencodePopups (gbp);
        return TRUE;
      }
      CopyStrFromTaxPtr (str, sizeof (str) - 2, row, 1);
      SafeSetTitle (gbp->taxName, str);
      CopyStrFromTaxPtr (str, sizeof (str) - 2, row, 2);
      SafeSetTitle (gbp->commonName, str);
      Select (gbp->taxName);
      SetCodes (gbp, row, TRUE);
      gbp->selectedOrg = row;
      InvalDocRows (gbp->orglist, 1, oldOrg, oldOrg);
      InvalDocRows (gbp->orglist, 1, row, row);
      return TRUE;
    }
    else
    {
      SafeSetTitle (gbp->taxName, taxname);
      SafeSetTitle (gbp->commonName, "");
      gbp->selectedOrg = 0;
      InvalDocRows (gbp->orglist, 1, oldOrg, oldOrg);
      ChangeGencodePopups (gbp);
    }
  }
  return FALSE;
}

static Boolean HighlightTaxName (DoC d, Int2 item, Int2 row, Int2 col)

{
  GenBioPagePtr  gbp;

  gbp = (GenBioPagePtr) GetObjectExtra (d);
  if (gbp != NULL) {
    return (Boolean) (row == gbp->selectedOrg);
  } else {
    return FALSE;
  }
}

static ParData orgListPar = {FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0};
static ColData orgListCol [] = {
  {0, 0, 80, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE},
  {0, 0,  0, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE},
  {0, 0,  0, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE},
  {0, 0,  0, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE},
  {0, 0,  0, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE},
  {0, 0,  0, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE},
  {0, 0,  0, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE},
  {0, 0,  0, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, TRUE}};

static CharPtr AllButFirstLinePrtProc (DoC d, Int2 item, Pointer ptr)

{
  Char     ch;
  CharPtr  tmp;

  if (ptr != NULL) {
    tmp = (CharPtr) ptr;
    ch = *tmp;
    while (ch != '\0' && ch != '\n') {
      tmp++;
      ch = *tmp;
    }
    if (ch != '\0') {
      tmp++;
      return StringSave (tmp);
    } else {
      return NULL;
    }
  } else {
    return NULL;
  }
}

static CharPtr MergeValNodeStrings (ValNodePtr list)

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
          if (vnp->data.ptrvalue != NULL) {
            if (tmp == NULL) {
              tmp = ptr;
            } else {
              tmp = StringMove (tmp, "; ");
            }
            str = StringSave ((CharPtr) vnp->data.ptrvalue);
            TrimSpacesAroundString (str);
            len = StringLen (str);
            if (len > 0 && str [len - 1] == ';') {
              str [len - 1] = '\0';
            }
            tmp = StringMove (tmp, str);
            MemFree (str);
          }
          vnp = vnp->next;
        }
      }
    }
  }
  return ptr;
}

static void AddTypeStrainButtonProc (ButtoN b)
{
  GenBioPagePtr  gbp;
  CharPtr        old_orgcomment;
  Int4           old_orgcomment_len;
  CharPtr        org_name;
  Int4           org_name_len;
  const CharPtr  ts = "type strain of ";
  const CharPtr  sep = "; ";
  CharPtr        new_orgcomment;

  gbp = GetObjectExtra(b);
  if (gbp == NULL) return;
  old_orgcomment_len = TextLength (gbp->orgcomment) + 1;
  old_orgcomment = MemNew (old_orgcomment_len + 1);
  if (old_orgcomment == NULL) return;
  org_name_len = TextLength (gbp->taxName) + 1;
  org_name = MemNew (org_name_len + 1);
  if (org_name == NULL) 
  {
    MemFree (old_orgcomment);
    return;
  }
  new_orgcomment = MemNew (old_orgcomment_len
			+ StringLen (sep)
			+ StringLen (ts)
			+ org_name_len
			+ 1);
  if (new_orgcomment == NULL)
  {
    MemFree (old_orgcomment);
    MemFree (org_name);
  }

  GetTitle (gbp->orgcomment, old_orgcomment, old_orgcomment_len);
  TrimSpacesAroundString (old_orgcomment);
  GetTitle (gbp->taxName, org_name, org_name_len);
  TrimSpacesAroundString (org_name);
  if (old_orgcomment[0] != 0)
  {
    StringCpy(new_orgcomment, old_orgcomment);
    StringCat(new_orgcomment, sep);
  }
  else
  {
    new_orgcomment[0] = 0;
  }
    
  StringCat (new_orgcomment, ts);
  StringCat (new_orgcomment, org_name);
  SetTitle (gbp->orgcomment, new_orgcomment);
  MemFree (org_name);
  MemFree (old_orgcomment);
  MemFree (new_orgcomment);
  Disable (b);
}

static Char useGenomicText [] = "\
(Use 'Genomic' for a sequence encoded by a nuclear gene.)\n";

static Pointer MakeOrgNameDataCopy (OrgNamePtr onp)
{
  OrgNamePtr onp_copy;
  Pointer    retval;

  if (onp == NULL) return NULL;

  onp_copy = (OrgNamePtr) AsnIoMemCopy (onp, (AsnReadFunc) OrgNameAsnRead, (AsnWriteFunc) OrgNameAsnWrite);
  if (onp_copy == NULL) return NULL;
  retval = onp_copy->data;
  onp_copy->data = NULL;
  onp_copy->choice = 0;
  OrgNameFree (onp_copy);
  return retval;
}

static void FreeGenBioOrgNameData (GenBioPagePtr gbp)
{
  OrgNamePtr     onp;

  if (gbp != NULL) {
    if (gbp->orgname_choice != 0) {
      onp = OrgNameNew ();
      if (onp != NULL) {
        onp->choice = gbp->orgname_choice;
        onp->data = gbp->orgname_data;
        OrgNameFree (onp);
        gbp->orgname_choice = 0;
        gbp->orgname_data = NULL;
      }
    }
  }
}

static void BioSourcePtrToGenBioPage (DialoG d, Pointer data)

{
  BioSourcePtr   biop;
  GenBioPagePtr  gbp;
  UIEnum         genome;
  ValNodePtr     head;
  OrgModPtr      mod;
  OrgNamePtr     onp;
  OrgRefPtr      orp;
  SubSourcePtr   ssp;
  CharPtr        str;
  WindoW         tempPort;

  gbp = (GenBioPagePtr) GetObjectExtra (d);
  biop = (BioSourcePtr) data;
  if (gbp != NULL) {
    orp = NULL;
    onp = NULL;
    tempPort = SavePort (gbp->taxName);
    SafeSetTitle (gbp->taxName, "");
    SafeSetTitle (gbp->commonName, "");
    gbp->typedSciName = FALSE;
    gbp->typedComName = FALSE;
    gbp->selectedOrg = 0;
    gbp->clickedOrg = 0;
    gbp->nuclGC = 0;
    gbp->mitoGC = 0;
    gbp->taxID = 0;
    SafeSetTitle (gbp->gbDiv, "");
    SafeSetValue (gbp->genome, 2);
    SafeSetValue (gbp->origin, 1);
    SafeSetStatus (gbp->is_focus, FALSE);
    SafeSetValue (gbp->gcode, 1);
    SafeSetValue (gbp->mgcode, 1);
    SafeSetValue (gbp->simplecode, 1);
    SafeSetTitle (gbp->orgcomment, "");
    SafeSetTitle (gbp->gbacr, "");
    SafeSetTitle (gbp->gbana, "");
    SafeSetTitle (gbp->gbsyn, "");
    SafeSetTitle (gbp->subcomment, "");
    SafeSetTitle (gbp->lineage, "");
    PointerToDialog (gbp->db, NULL);
    PointerToDialog (gbp->syn, NULL);
    PointerToDialog (gbp->mod, NULL);
    if (biop != NULL) {
      SetEnumPopup (gbp->genome, gbp->genomeAlist, (UIEnum) biop->genome);
      SetEnumPopup (gbp->origin, biosource_origin_alist, (UIEnum) biop->origin);
      SafeSetStatus (gbp->is_focus, biop->is_focus);
      if (biop->is_focus) {
        SafeEnable (gbp->is_focus);
      }
      orp = biop->org;
      if (orp != NULL) {
        gbp->origTaxName = StringSave (orp->taxname);
        SafeSetTitle (gbp->taxName, orp->taxname);
        SafeSetTitle (gbp->commonName, orp->common);
        PointerToDialog (gbp->db, orp->db);
        PointerToDialog (gbp->syn, orp->syn);
        PointerToDialog (gbp->mod, orp->mod);
        onp = orp->orgname;
        if (onp != NULL) {
          /* store orgname data for unaltered retrieval later */
          gbp->orgname_choice = onp->choice;
          gbp->orgname_data = MakeOrgNameDataCopy(onp);

          SafeSetTitle (gbp->lineage, onp->lineage);
          PointerToDialog (gbp->orgmod, onp->mod);
          mod = onp->mod;
          head = NULL;
          while (mod != NULL) {
            switch (mod->subtype) {
              case 32 :
                SetTitle (gbp->gbacr, mod->subname);
                break;
              case 33 :
                SetTitle (gbp->gbana, mod->subname);
                break;
              case 34 :
                SetTitle (gbp->gbsyn, mod->subname);
                break;
              case 255 :
                ValNodeAddStr (&head, 0, mod->subname);
                /*
                SetTitle (gbp->orgcomment, mod->subname);
                */
                break;
              default :
                break;
            }
            mod = mod->next;
          }
          if (head != NULL) {
            str = MergeValNodeStrings (head);
            SetTitle (gbp->orgcomment, str);
            if ( StringStr (str, "type strain of ")) {
              Disable (gbp->add_type_strain_to_orgcomment);
            }
            MemFree (str);
          }
          ValNodeFree (head);
        }
      }
      PointerToDialog (gbp->subsource, biop->subtype); 
      ssp = biop->subtype;
      head = NULL;
      while (ssp != NULL) {
        if (ssp->subtype == 255) {
          ValNodeAddStr (&head, 0, ssp->name);
          /*
          SetTitle (gbp->subcomment, ssp->name);
          */
        }
        ssp = ssp->next;
      }
      if (head != NULL) {
        str = MergeValNodeStrings (head);
        SetTitle (gbp->subcomment, str);
        MemFree (str);
      }
      ValNodeFree (head);
    }
    if (orp != NULL) {
      if (! TextHasNoText (gbp->taxName)) {
        AutoScrollTax (gbp, gbp->taxName, TRUE, FALSE, FALSE);
      /*
      } else if (! TextHasNoText (gbp->commonName)) {
        AutoScrollTax (gbp, gbp->commonName, FALSE, FALSE, FALSE);
      */
        SafeSetTitle (gbp->gbDiv, "");
        SafeSetValue (gbp->gcode, 1);
        SafeSetValue (gbp->mgcode, 1);
        SafeSetValue (gbp->simplecode, 1);
      }
    }
    if (onp != NULL) {
      SafeSetTitle (gbp->gbDiv, onp->div);
      if (onp->gcode > 0) {
        gbp->nuclGC = onp->gcode;
      }
      if (onp->mgcode > 0) {
        gbp->mitoGC = onp->mgcode;
      }
      if (gbp->simplecode != NULL) {
        if (GetEnumPopup (gbp->genome, gbp->genomeAlist, &genome)) {
          if (genome == 4 || genome == 5) {
            SafeSetValue (gbp->simplecode, gcIdToIndex [onp->mgcode]);
          } else if (genome == GENOME_chloroplast ||
                     genome == GENOME_chromoplast ||
                     genome == GENOME_plastid ||
                     genome == GENOME_cyanelle ||
                     genome == GENOME_apicoplast ||
                     genome == GENOME_leucoplast ||
                     genome == GENOME_proplastid) {
            SafeSetValue (gbp->simplecode, gcIdToIndex [11]);
          } else {
            SafeSetValue (gbp->simplecode, gcIdToIndex [onp->gcode]);
          }
        }
        SafeSetValue (gbp->gcode, gcIdToIndex [onp->gcode]);
        SafeSetValue (gbp->mgcode, gcIdToIndex [onp->mgcode]);
      } else {
        SafeSetValue (gbp->gcode, gcIdToIndex [onp->gcode]);
        SafeSetValue (gbp->mgcode, gcIdToIndex [onp->mgcode]);
      }
    }
    RestorePort (tempPort);
  }
}


static Pointer GenBioPageToBioSourcePtr (DialoG d)

{
  BioSourcePtr   biop;
  Char           buf [256];
  Char           ch;
  CharPtr        chptr;
  FILE           *f;
  GenBioPagePtr  gbp;
  UIEnum         genome;
  Boolean        goOn;
  OrgModPtr      mod;
  OrgModPtr      nextmod;
  OrgNamePtr     onp;
  OrgRefPtr      orp;
  OrgModPtr      PNTR prevmod;
  CharPtr        ptr;
  SubSourcePtr   ssp;
  Char           str [PATH_MAX];
  Int4           taxID;
  OrgModPtr      tmpmod;
  SubSourcePtr   tmpssp;
  UIEnum         val;
  
  Int2           num; /* contains number of items in gbp->orglist */
  Int4           row; /* contains closest row to match in gbp->orglist */
  Char           txt [256]; /* holds tax name copied from gbp->orglist */

  biop = NULL;
  gbp = (GenBioPagePtr) GetObjectExtra (d);
  if (gbp != NULL) {
    biop = BioSourceNew ();
    if (biop != NULL) {
      if (GetEnumPopup (gbp->genome, gbp->genomeAlist, &val)) {
        biop->genome = (Uint1) val;
      }
      if (GetEnumPopup (gbp->origin, biosource_origin_alist, &val)) {
        biop->origin = (Uint1) val;
      }
      biop->is_focus = GetStatus (gbp->is_focus);
      orp = OrgRefNew ();
      biop->org = orp;
      if (orp != NULL) {
        orp->taxname = SaveStringFromText (gbp->taxName);
      
        /* make sure we use capitalization from list */
        if (gbp->orglist != NULL)
        {
          GetItemParams (gbp->orglist, 1, NULL, &num, NULL, NULL, NULL);
          if (num > 0) {
            row = FindTaxText (orp->taxname, num);
            if (row > 0 && row <= num) {
              CopyStrFromTaxPtr (txt, sizeof (txt) - 2, row, 1);
              if (StringICmp (txt, orp->taxname) == 0
                  && StringCmp (txt, orp->taxname) != 0) {
                orp->taxname = MemFree (orp->taxname);
                orp->taxname = StringSave (txt);
              }
            }
          }
        }
        

        /*
        orp->common = SaveStringFromText (gbp->commonName);
        */
        GetTitle (gbp->commonName, str, sizeof (str) - 1);
        TrimSpacesAroundString (str);
        if (! StringHasNoText (str)) {
          orp->common = StringSave (str);
        }
        orp->db = DialogToPointer (gbp->db);
        orp->syn = DialogToPointer (gbp->syn);
        orp->mod = DialogToPointer (gbp->mod);
        onp = OrgNameNew ();
        orp->orgname = onp;
        if (onp != NULL) {
          /* retrieve unaltered orgname data */
          onp->choice = gbp->orgname_choice;
          onp->data = gbp->orgname_data;
          gbp->orgname_choice = 0;
          gbp->orgname_data = NULL;

          if (gbp->simplecode != NULL) {
            if (GetEnumPopup (gbp->genome, gbp->genomeAlist, &genome)) {
              if (genome == 4 || genome == 5) {
                onp->mgcode = gcIndexToId [GetValue (gbp->simplecode)];
                onp->gcode = gcIndexToId [GetValue (gbp->gcode)];
              } else {
                onp->gcode = gcIndexToId [GetValue (gbp->simplecode)];
                onp->mgcode = gcIndexToId [GetValue (gbp->mgcode)];
              }
            }
          } else {
            onp->gcode = gcIndexToId [GetValue (gbp->gcode)];
            onp->mgcode = gcIndexToId [GetValue (gbp->mgcode)];
          }
          if (! TextHasNoText (gbp->gbDiv)) {
            onp->div = SaveStringFromText (gbp->gbDiv);
          }
          if (gbp->lineage == NULL && gbp->taxID != 0) {
            ProgramPath (str, sizeof (str));
            ptr = StringRChr (str, DIRDELIMCHR);
            if (ptr != NULL) {
              *ptr = '\0';
              FileBuildPath (str, NULL, "lineages.txt");
              f = FileOpen (str, "r");
              if (f == NULL) {
                if (GetAppParam ("NCBI", "NCBI", "DATA", "", str, sizeof (str))) {
                  FileBuildPath (str, NULL, "lineages.txt");
                  f = FileOpen (str, "r");
                }
              }
              if (f != NULL) {
                if (FileGets (str, sizeof (str), f) != NULL) {
                  goOn = (Boolean) (FileGets (str, sizeof (str), f) != NULL);
                  while (goOn) {
                    ptr = StringChr (str, '\t');
                    if (ptr != NULL) {
                      *ptr = '\0';
                      if (StrToLong (str, &taxID) && taxID == gbp->taxID) {
                        ptr++;
                        chptr = ptr;
                        ch = *chptr;
                        while (ch != '\0' && ch != '\r' && ch != '\n') {
                          chptr++;
                          ch = *chptr;
                        }
                        *chptr = '\0';
                        onp->lineage = StringSave (ptr);
                        goOn = FALSE;
                      }
                    }
                    goOn = (Boolean) (goOn && (FileGets (str, sizeof (str), f) != NULL));
                  }
                }
                FileClose (f);
              }
            }
          } else if (! TextHasNoText (gbp->lineage)) {
            onp->lineage = SaveStringFromTextAndStripNewlines (gbp->lineage);
          }
          onp->mod = DialogToPointer (gbp->orgmod);
          GetTitle (gbp->gbacr, buf, sizeof (buf) - 1);
          if (! StringHasNoText (buf)) {
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
            if (mod != NULL) {
              mod->subtype = 32;
              mod->subname = StringSave (buf);
            }
          }
          GetTitle (gbp->gbana, buf, sizeof (buf) - 1);
          if (! StringHasNoText (buf)) {
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
            if (mod != NULL) {
              mod->subtype = 33;
              mod->subname = StringSave (buf);
            }
          }
          GetTitle (gbp->gbsyn, buf, sizeof (buf) - 1);
          if (! StringHasNoText (buf)) {
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
            if (mod != NULL) {
              mod->subtype = 34;
              mod->subname = StringSave (buf);
            }
          }
          if (! TextHasNoText (gbp->orgcomment)) {
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
            if (mod != NULL) {
              mod->subtype = 255;
              mod->subname = SaveStringFromTextAndStripNewlines (gbp->orgcomment);
            }
          }
          if (gbp->stripOldName && onp->mod != NULL) {
            prevmod = (OrgModPtr PNTR) &(onp->mod);
            tmpmod = onp->mod;
            while (tmpmod != NULL) {
              nextmod = tmpmod->next;
              if (tmpmod->subtype == 254) {
                *(prevmod) = tmpmod->next;
                tmpmod->next = NULL;
                OrgModFree (tmpmod);
              } else {
                prevmod = (OrgModPtr PNTR) &(tmpmod->next);
              }
              tmpmod = nextmod;
            }
          }
          if (onp->lineage == NULL && onp->mod == NULL &&
              onp->gcode == 0 && onp->mgcode == 0) {
            orp->orgname = OrgNameFree (orp->orgname);
          }
        }
      }
      biop->subtype = DialogToPointer (gbp->subsource); 
      if (! TextHasNoText (gbp->subcomment)) {
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
        if (ssp != NULL) {
          ssp->subtype = 255;
          ssp->name = SaveStringFromTextAndStripNewlines (gbp->subcomment);
        }
      }
      RemoveTextFromTextFreeSubSourceModifiers (biop, NULL);     

      /* if we find plasmid-name on a location that cannot have
       * plasmids, change the location to plasmid */
      if (biop->genome != GENOME_mitochondrion
          && biop->genome != GENOME_chloroplast
          && biop->genome != GENOME_kinetoplast
          && biop->genome != GENOME_chromoplast
          && biop->genome != GENOME_plastid
          && biop->genome != GENOME_apicoplast
          && biop->genome != GENOME_leucoplast
          && biop->genome != GENOME_proplastid)
      {
        tmpssp = biop->subtype;
        while (tmpssp != NULL)
        {
          if (tmpssp->subtype == SUBSRC_plasmid_name) {
            biop->genome = GENOME_plasmid;
            break;
          }
          tmpssp = tmpssp->next;
        }
      }
 
      if (orp != NULL) {
        if (orp->taxname == NULL && orp->common == NULL &&
            orp->mod == NULL && orp->db == NULL && orp->syn == NULL &&
            orp->orgname == NULL && biop->subtype == NULL) {
          biop = BioSourceFree (biop);
        }
      } else {
        biop = BioSourceFree (biop);
      }
    }
  }
  return (Pointer) biop;
}

static ValNodePtr TestGenBioDialog (DialoG d)

{
  Char           comm [64];
  GenBioPagePtr  gbp;
  ValNodePtr     head;

  head = NULL;
  gbp = (GenBioPagePtr) GetObjectExtra (d);
  if (gbp != NULL) {
    GetTitle (gbp->commonName, comm, sizeof (comm) - 1);
    if (TextHasNoText (gbp->taxName) && StringHasNoText (comm) ) {
      head = AddStringToValNodeChain (head, "organism name", 1);
    }
  }
  return head;
}

static void BioSourceMessage (DialoG d, Int2 mssg)

{
  GenBioPagePtr  gbp;

  gbp = (GenBioPagePtr) GetObjectExtra (d);
  if (gbp != NULL) {
    switch (mssg) {
      case VIB_MSG_INIT :
        UpdateDocument (gbp->orglist, 0, 0);
        AdjustDocScroll (gbp->orglist);
        break;
      case VIB_MSG_ENTER :
        Select (gbp->taxName);
        break;
      default :
        break;
    }
  }
}

extern PopuP ReplaceBioSourceGenomePopup (DialoG d, PopuP genome);
PopuP ReplaceBioSourceGenomePopup (DialoG d, PopuP genome)

{
  GenBioPagePtr  gbp;
  PopuP          orig_genome = NULL;

  gbp = (GenBioPagePtr) GetObjectExtra (d);
  if (gbp != NULL) {
    orig_genome = gbp->genome;
    gbp->genome = genome;
  }
  return orig_genome;
}


extern PopuP ReplaceBioSourceGencodePopup (DialoG d, PopuP gencode);
PopuP ReplaceBioSourceGencodePopup (DialoG d, PopuP gencode)

{
  GenBioPagePtr  gbp;
  PopuP          orig_gencode = NULL;

  gbp = (GenBioPagePtr) GetObjectExtra (d);
  if (gbp != NULL) {
    orig_gencode = gbp->simplecode;
    gbp->simplecode = gencode;
  }
  return orig_gencode;
}


extern DialoG CreateSimpleBioSourceDialog (GrouP h, CharPtr title)

{
  GrouP          f;
  GrouP          g;
  GenBioPagePtr  gbp;
  Int2           height;
  GrouP          m;
  GrouP          p;
  GrouP          q;
  RecT           r;
  GrouP          s;
  GrouP          x;

  p = HiddenGroup (h, 1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  gbp = (GenBioPagePtr) MemNew (sizeof (GenBioPage));
  if (gbp != NULL) {

    SetObjectExtra (p, gbp, StdCleanupExtraProc);
    gbp->dialog = (DialoG) p;
    gbp->todialog = BioSourcePtrToGenBioPage;
    gbp->fromdialog = GenBioPageToBioSourcePtr;
    gbp->dialogmessage = BioSourceMessage;
    gbp->testdialog = TestGenBioDialog;

    if (title != NULL && title [0] != '\0') {
      s = NormalGroup (p, 0, -2, title, systemFont, NULL);
    } else {
      s = HiddenGroup (p, 0, -2, NULL);
    }
    SetGroupSpacing (s, 10, 10);

    m = HiddenGroup (s, 0, 0, NULL);

    q = HiddenGroup (m, -1, 0, NULL);
    SetGroupSpacing (q, 10, 10);

    gbp->origTaxName = NULL;
    gbp->stripOldName = FALSE;

    g = HiddenGroup (q, 1, 0, NULL);
    /*
    StaticPrompt (g, "Organism", 0, 0, programFont, 'c');
    */
    f = HiddenGroup (g, 2, 0, NULL);
    StaticPrompt (f, "Scientific Name", 0, dialogTextHeight, programFont, 'l');
    gbp->taxName = DialogText (f, "", 20, TaxNameText);
    SetObjectExtra (gbp->taxName, gbp, NULL);
    StaticPrompt (f, "Common Name", 0, dialogTextHeight, programFont, 'l');
    /*
    gbp->commonName = DialogText (f, "", 10, CommonNameText);
    */
    gbp->commonName = (Handle) StaticPrompt (f, "", 10 * stdCharWidth, dialogTextHeight, systemFont, 'l');
    SetObjectExtra (gbp->commonName, gbp, NULL);
    StaticPrompt (f, "", 0, dialogTextHeight, programFont, 'l');
    f = HiddenGroup (g, 1, 0, NULL);
    SelectFont (programFont);
    height = LineHeight ();
    SelectFont (systemFont);
    gbp->orglist = DocumentPanel (f, stdCharWidth * 25, height * 6);
    SetObjectExtra (gbp->orglist, gbp, NULL);
    SetDocAutoAdjust (gbp->orglist, TRUE);
    orgListCol [0].pixWidth = screenRect.right - screenRect.left;
    AppendItem (gbp->orglist, AllButFirstLinePrtProc, orgTxtPtr, FALSE, orgNum,
                &orgListPar, orgListCol, programFont);
    SetDocAutoAdjust (gbp->orglist, TRUE);
    SetDocProcs (gbp->orglist, ClickTaxName, NULL, ReleaseTaxName, NULL);
    SetDocShade (gbp->orglist, NULL, NULL, HighlightTaxName, NULL);

    AlignObjects (ALIGN_RIGHT, (HANDLE) gbp->taxName, (HANDLE) gbp->commonName,
                  (HANDLE) gbp->orglist, NULL);

    g = HiddenGroup (q, -1, 0, NULL);
    f = HiddenGroup (g, 3, 0, NULL);
    StaticPrompt (f, "Location of Sequence",
                  0, popupMenuHeight, programFont, 'l');
    gbp->genome = PopupList (f, TRUE, SetGenome);
    SetObjectExtra (gbp->genome, gbp, NULL);
    gbp->genomeAlist = biosource_genome_simple_alist;
    InitEnumPopup (gbp->genome, gbp->genomeAlist, NULL);
    SetValue (gbp->genome, 2);
    ObjectRect (gbp->orglist, &r);
    MultiLinePrompt (g, useGenomicText, r.right - r.left - 2, programFont);

    x = HiddenGroup (q, 0, 0, NULL);

    f = HiddenGroup (x, 2, 0, NULL);
    StaticPrompt (f, "Genetic Code for Translation", 0, popupMenuHeight, programFont, 'l');
    gbp->simplecode = PopupList (f, TRUE, NULL);
    PopulateGeneticCodePopup (gbp->simplecode);
    SetValue (gbp->simplecode, 1);
    gbp->gbDiv = DialogText (x, "", 4, NULL);
    Hide (gbp->gbDiv);

/* superimpose two hidden genetic code controls to save both in resulting biosource */

    gbp->gcode = PopupList (x, TRUE, NULL);
    PopulateGeneticCodePopup (gbp->gcode);
    SetValue (gbp->gcode, 1);
    gbp->mgcode = PopupList (x, TRUE, NULL);
    PopulateGeneticCodePopup (gbp->mgcode);
    SetValue (gbp->mgcode, 1);
    Hide (gbp->gcode);
    Hide (gbp->mgcode);

    SelectFont (systemFont);
  }

  return (DialoG) p;
}

static void OrgModPtrToOrgmodDialog (DialoG d, Pointer data)

{
  ValNodePtr  head;
  Int2        j;
  size_t      len;
  OrgModPtr   list;
  CharPtr     str;
  TagListPtr  tlp;
  Char        tmp [16];
  ValNodePtr  vnp;

  tlp = (TagListPtr) GetObjectExtra (d);
  list = (OrgModPtr) data;
  if (tlp != NULL) {
    head = NULL;
    while (list != NULL) {
      if (list->subname != NULL && list->subtype != 255 &&
          list->subtype != 32 && list->subtype != 33 && list->subtype != 34) {
        vnp = ValNodeNew (head);
        if (head == NULL) {
          head = vnp;
        }
        if (vnp != NULL) {
          sprintf (tmp, "%d", (int) list->subtype);
          len = StringLen (tmp) + StringLen (list->subname);
          str = MemNew (len + 4);
          if (str != NULL) {
            StringCpy (str, tmp);
            StringCat (str, "\t");
            StringCat (str, list->subname);
            StringCat (str, "\n");
          }
          vnp->data.ptrvalue = str;
        }
      }
      list = list->next;
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

static Pointer OrgmodDialogToOrgModPtr (DialoG d)

{
  Char        ch;
  OrgModPtr   head;
  Int2        j;
  Int2        len;
  OrgModPtr   omp;
  OrgModPtr   omplast;
  Boolean     okay;
  CharPtr     str;
  TagListPtr  tlp;
  CharPtr     tmp;
  int         val;
  ValNodePtr  vnp;

  head = NULL;
  tlp = (TagListPtr) GetObjectExtra (d);
  if (tlp != NULL && tlp->vnp != NULL) {
    omp = NULL;
    omplast = NULL;
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
          MemFree (tmp);
          tmp = ExtractTagListColumn ((CharPtr) vnp->data.ptrvalue, 1);
          if (! StringHasNoText (tmp)) {
            omp = OrgModNew ();
            if (omplast == NULL) {
              head = omp;
            } else {
              omplast->next = omp;
            }
            omplast = omp;
            if (omp != NULL) {
              omp->subtype = (Uint1) val;
              omp->subname = tmp;
            }
          } else {
            MemFree (tmp);
          }
        } else {
          MemFree (tmp);
        }
      }
    }
  }
  return (Pointer) head;
}

static void SubSourcePtrToSubsourceDialog (DialoG d, Pointer data)

{
  ValNodePtr    head;
  Int2          j;
  size_t        len;
  SubSourcePtr  list;
  CharPtr       str;
  TagListPtr    tlp;
  Char          tmp [16];
  ValNodePtr    vnp;

  tlp = (TagListPtr) GetObjectExtra (d);
  list = (SubSourcePtr) data;
  if (tlp != NULL) {
    head = NULL;
    while (list != NULL) {
      if (list->name != NULL && list->subtype != 255) {
        vnp = ValNodeNew (head);
        if (head == NULL) {
          head = vnp;
        }
        if (vnp != NULL) {
          sprintf (tmp, "%d", (int) list->subtype);
          len = StringLen (tmp) + StringLen (list->name);
          str = MemNew (len + 4);
          if (str != NULL) {
            StringCpy (str, tmp);
            StringCat (str, "\t");
            StringCat (str, list->name);
            StringCat (str, "\n");
          }
          vnp->data.ptrvalue = str;
        }
      }
      list = list->next;
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

typedef struct fixmodifiertextform
{
  WindoW     w;
  Boolean    done;
  Boolean    move_to_text;
  Boolean    remove;
  Boolean    do_all;
} FixModifierTextFormData, PNTR FixModifierTextFormPtr;

static void FixModifierTextMove (ButtoN b)
{
  FixModifierTextFormPtr fp;
  
  fp = (FixModifierTextFormPtr) GetObjectExtra (b);
  if (fp == NULL) return;
  
  Remove (fp->w);
  fp->remove = FALSE;
  fp->move_to_text = TRUE;
  fp->do_all = FALSE;
  fp->done = TRUE;	
}

static void FixModifierTextMoveAll (ButtoN b)
{
  FixModifierTextFormPtr fp;
  
  fp = (FixModifierTextFormPtr) GetObjectExtra (b);
  if (fp == NULL) return;
  
  Remove (fp->w);
  fp->remove = FALSE;
  fp->move_to_text = TRUE;
  fp->do_all = TRUE;
  fp->done = TRUE;	
}

static void FixModifierTextRemove (ButtoN b)
{
  FixModifierTextFormPtr fp;
  
  fp = (FixModifierTextFormPtr) GetObjectExtra (b);
  if (fp == NULL) return;
  
  Remove (fp->w);
  fp->remove = TRUE;
  fp->move_to_text = FALSE;
  fp->do_all = FALSE;
  fp->done = TRUE;	
}

static void FixModifierTextRemoveAll (ButtoN b)
{
  FixModifierTextFormPtr fp;
  
  fp = (FixModifierTextFormPtr) GetObjectExtra (b);
  if (fp == NULL) return;
  
  Remove (fp->w);
  fp->remove = TRUE;
  fp->move_to_text = FALSE;
  fp->do_all = TRUE;
  fp->done = TRUE;	
}

extern ModTextFixPtr ModTextFixNew (void)
{
  ModTextFixPtr tfp;
  
  tfp = (ModTextFixPtr) MemNew (sizeof (ModTextFixData));
  if (tfp == NULL) return NULL;
  tfp->remove_this = FALSE;
  tfp->move_this = FALSE;
  tfp->remove_all_germline = FALSE;
  tfp->remove_all_transgenic = FALSE;
  tfp->remove_all_environmental = FALSE;
  tfp->remove_all_rearranged = FALSE;
  tfp->move_all_germline = FALSE;
  tfp->move_all_transgenic = FALSE;
  tfp->move_all_environmental = FALSE;
  tfp->move_all_rearranged = FALSE;
  return tfp;
}

static void 
GetModifierTextFix (ModTextFixPtr tfp, Uint1 subtype, CharPtr txt)
{
  GrouP  g, c, t;
  ButtoN b;
  FixModifierTextFormData fd;
  CharPtr prompt_fmt = "You have text (%s) in %s modifier field.";
  CharPtr prompt_str = NULL;
  CharPtr btn_str = NULL;
  
  if (tfp == NULL) return;
  switch (subtype)
  {
  	case SUBSRC_rearranged:
  	  if (tfp->remove_all_rearranged)
  	  {
  	  	tfp->remove_this = TRUE;
  	  	tfp->move_this = FALSE;
  	  	return;
  	  }
  	  else if (tfp->move_all_rearranged)
  	  {
  	  	tfp->move_this = TRUE;
  	  	tfp->remove_this = FALSE;
  	  	return;
  	  }
  	  break;
  	case SUBSRC_transgenic:
  	  if (tfp->remove_all_transgenic)
  	  {
  	  	tfp->remove_this = TRUE;
  	  	tfp->move_this = FALSE;
  	  	return;
  	  }
  	  else if (tfp->move_all_transgenic)
  	  {
  	  	tfp->move_this = TRUE;
  	  	tfp->remove_this = FALSE;
  	  	return;
  	  }
  	  break;
  	case SUBSRC_germline:
  	  if (tfp->remove_all_germline)
  	  {
  	  	tfp->remove_this = TRUE;
  	  	tfp->move_this = FALSE;
  	  	return;
  	  }
  	  else if (tfp->move_all_germline)
  	  {
  	  	tfp->move_this = TRUE;
  	  	tfp->remove_this = FALSE;
  	  	return;
  	  }
  	  break;
  	case SUBSRC_environmental_sample:
  	  if (tfp->remove_all_environmental)
  	  {
  	  	tfp->remove_this = TRUE;
  	  	tfp->move_this = FALSE;
  	  	return;
  	  }
  	  else if (tfp->move_all_environmental)
  	  {
  	  	tfp->move_this = TRUE;
  	  	tfp->remove_this = FALSE;
  	  	return;
  	  }
  	  break;
  }

  fd.w = ModalWindow(-20, -13, -10, -10, NULL);
  g = HiddenGroup(fd.w, -1, 0, NULL);
  
  prompt_str = (CharPtr) MemNew (sizeof (Char) * (StringLen (prompt_fmt) + StringLen (txt)
                                  + StringLen ("an environmental sample")));
  if (prompt_str == NULL) return;
  switch (subtype)
  {
  	case SUBSRC_rearranged:
  	  sprintf (prompt_str, prompt_fmt, txt, "a rearranged");
  	  break;
  	case SUBSRC_germline:
  	  sprintf (prompt_str, prompt_fmt, txt, "a germline");
  	  break;
  	case SUBSRC_transgenic:
  	  sprintf (prompt_str, prompt_fmt, txt, "a transgenic");
  	  break;
  	case SUBSRC_environmental_sample:
  	  sprintf (prompt_str, prompt_fmt, txt, "an environmental sample");
  	  break;
  }
  
  t = HiddenGroup (g, 1, 0, NULL);
  StaticPrompt (t, prompt_str, 0, dialogTextHeight, programFont, 'l');
  StaticPrompt (t, "This text will never be displayed in your GenBank record.", 0, dialogTextHeight, programFont, 'l');
  StaticPrompt (t, "Do you want to move this text to a note or remove it?", 0, dialogTextHeight, programFont, 'l');
  
  c = HiddenGroup (g, 4, 0, NULL);
  b = PushButton (c, "Move to note", FixModifierTextMove);
  SetObjectExtra (b, &fd, NULL);  
  b = PushButton (c, "Move all to note", FixModifierTextMoveAll);
  SetObjectExtra (b, &fd, NULL);
  b = PushButton (c, "Remove", FixModifierTextRemove);
  SetObjectExtra (b, &fd, NULL);
  b = PushButton (c, "Remove all", FixModifierTextRemoveAll);
  SetObjectExtra (b, &fd, NULL);
  AlignObjects (ALIGN_CENTER, (HANDLE) t, (HANDLE) c, NULL);
  
  Show(fd.w); 
  Select (fd.w);
  fd.done = FALSE;
  while (!fd.done)
  {
    ProcessExternalEvent ();
    Update ();
  }
  ProcessAnEvent ();

  if (fd.remove)
  {
  	tfp->remove_this = TRUE;
  	if (fd.do_all)
  	{
  	  switch (subtype)
  	  {
  	  	case SUBSRC_rearranged:
  	  	  tfp->remove_all_rearranged = TRUE;
  	  	  tfp->move_all_rearranged = FALSE;
  	  	  break;
  	  	case SUBSRC_transgenic:
  	  	  tfp->remove_all_transgenic = TRUE;
  	  	  tfp->move_all_transgenic = FALSE;
  	  	  break;
  	  	case SUBSRC_germline:
  	  	  tfp->remove_all_germline = TRUE;
  	  	  tfp->move_all_germline = FALSE;
  	  	  break;
  	  	case SUBSRC_environmental_sample:
  	  	  tfp->remove_all_environmental = TRUE;
  	  	  tfp->move_all_environmental = FALSE;
  	  	  break;
  	  }
  	}
  }
  else if (fd.move_to_text)
  {
  	tfp->move_this = TRUE;
  	if (fd.do_all)
  	{
  	  switch (subtype)
  	  {
  	  	case SUBSRC_rearranged:
  	  	  tfp->remove_all_rearranged = FALSE;
  	  	  tfp->move_all_rearranged = TRUE;
  	  	  break;
  	  	case SUBSRC_transgenic:
  	  	  tfp->remove_all_transgenic = FALSE;
  	  	  tfp->move_all_transgenic = TRUE;
  	  	  break;
  	  	case SUBSRC_germline:
  	  	  tfp->remove_all_germline = FALSE;
  	  	  tfp->move_all_germline = TRUE;
  	  	  break;
  	  	case SUBSRC_environmental_sample:
  	  	  tfp->remove_all_environmental = FALSE;
  	  	  tfp->move_all_environmental = TRUE;
  	  	  break;
  	  }
  	}
  }
}

extern void RemoveTextFromTextFreeSubSourceModifiers (BioSourcePtr biop, Pointer userdata)
{
  SubSourcePtr ssp;
  SubSourcePtr note_ssp = NULL;
  Int4         len;
  CharPtr      new_note;
  ModTextFixPtr   tfp;
  
  if (biop == NULL || biop->subtype == NULL) return;
  
  if (userdata == NULL)
  {
    tfp = ModTextFixNew();
    if (tfp == NULL) return;
  }
  else
  {
  	tfp = (ModTextFixPtr) userdata;
  }
  
  for (ssp = biop->subtype; ssp != NULL; ssp = ssp->next)
  {
  	tfp->move_this = FALSE;
  	tfp->remove_this = FALSE;
  	if ((ssp->subtype == SUBSRC_germline
  	    || ssp->subtype == SUBSRC_transgenic
  	    || ssp->subtype == SUBSRC_rearranged
  	    || ssp->subtype == SUBSRC_environmental_sample)
  	    && ! StringHasNoText (ssp->name))
  	{
 	  GetModifierTextFix (tfp, ssp->subtype, ssp->name);
  	  if (tfp->move_this)
  	  {
  	    /* if a note modifier is found, add this text to it, otherwise create a new
  	     * note modifier to hold this text.
  	     */
  	    if (note_ssp == NULL)
  	    {
  	      for (note_ssp = biop->subtype; note_ssp != NULL && note_ssp->subtype != 255; note_ssp = note_ssp->next)
  	      {	
  	      }
  	    }
  	    if (note_ssp == NULL)
  	    {
  	  	  note_ssp = SubSourceNew ();
  	  	  if (note_ssp != NULL)
  	  	  {
  	  	    note_ssp->subtype = 255;
  	  	    note_ssp->name = ssp->name;
  	  	    ssp->name = StringSave ("");
  	  	    note_ssp->next = ssp->next;
  	  	    ssp->next = note_ssp;
  	  	  }
  	    }
  	    else if (StringHasNoText (note_ssp->name))
  	    {
          note_ssp->name = MemFree (note_ssp->name);
          note_ssp->name = ssp->name;
          ssp->name = StringSave ("");  	  		
  	    }
  	    else
  	    {
  	  	  len = StringLen (note_ssp->name) + StringLen (ssp->name) + 3;
  	  	  new_note = (CharPtr) MemNew (len * sizeof (Char));
  	  	  if (new_note != NULL)
  	  	  {
  	  	    StringCpy (new_note, note_ssp->name);
  	  	    StringCat (new_note, "; ");
  	  	    StringCat (new_note, ssp->name);
  	  	    note_ssp->name = MemFree (note_ssp->name);
  	  	    note_ssp->name = new_note;
  	  	    ssp->name = MemFree (ssp->name);
  	  	    ssp->name = StringSave ("");
  	  	  }
  	    }
  	  }
  	  else if (tfp->remove_this)
  	  {
  	    ssp->name = MemFree (ssp->name);
  	    ssp->name = StringSave ("");
  	  }
  	}
  }
  if (userdata == NULL)
  {
  	MemFree (tfp);
  }
}

static Pointer SubsourceDialogToSubSourcePtr (DialoG d)

{
  Char          ch;
  SubSourcePtr  head;
  Int2          j;
  Int2          len;
  SubSourcePtr  ssp;
  SubSourcePtr  ssplast;
  Boolean       okay;
  CharPtr       str;
  TagListPtr    tlp;
  CharPtr       tmp;
  int           val;
  ValNodePtr    vnp;

  head = NULL;
  tlp = (TagListPtr) GetObjectExtra (d);
  if (tlp != NULL && tlp->vnp != NULL) {
    ssp = NULL;
    ssplast = NULL;
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
          MemFree (tmp);
          tmp = ExtractTagListColumn ((CharPtr) vnp->data.ptrvalue, 1);
          if ((val == SUBSRC_germline ||
               val == SUBSRC_rearranged ||
               val == SUBSRC_transgenic ||
               val == SUBSRC_environmental_sample) &&
              StringHasNoText (tmp)) {
            MemFree (tmp);
            tmp = StringSave ("");
          }
          if ((! StringHasNoText (tmp)) ||
              val == SUBSRC_germline ||
              val == SUBSRC_rearranged ||
              val == SUBSRC_transgenic ||
              val == SUBSRC_environmental_sample) {
            ssp = SubSourceNew ();
            if (ssplast == NULL) {
              head = ssp;
            } else {
              ssplast->next = ssp;
            }
            ssplast = ssp;
            if (ssp != NULL) {
              ssp->subtype = (Uint1) val;
              ssp->name = tmp;
              tmp = NULL;
            }
          }
          MemFree (tmp);
        }
      }
    }
  }
  return (Pointer) head;
}

EnumFieldAssocPtr orgmod_alists [] = {
  orgmod_subtype_alist, NULL
};

EnumFieldAssocPtr subsource_alists [] = {
  subsource_subtype_alist, NULL
};

Uint2 orgmod_widths [] = {
  0, 25
};

Uint2 subsource_widths [] = {
  0, 25
};

Uint2 orgmod_types [] = {
  TAGLIST_POPUP, TAGLIST_TEXT
};

Uint2 subsource_types [] = {
  TAGLIST_POPUP, TAGLIST_TEXT
};

static CharPtr orgmod_extra_prompts [] = {
  "Additional", "Organism", "Information", NULL
};

static CharPtr subsource_extra_prompts [] = {
  "Additional", "Source", "Information", NULL
};

static CharPtr orgTabs [] = {
  "Names", "Location", "Genetic Codes", "Lineage", NULL
};

static CharPtr modTabs [] = {
  "Source", "Organism", "GenBank", NULL
};

static CharPtr modTabsUns [] = {
  "Source", "Organism", "GenBank", "Unstructured", NULL
};

static CharPtr miscTabs [] = {
  "Synonyms", "Cross-Refs", NULL
};

static void ChangeOrgSubPage (VoidPtr data, Int2 newval, Int2 oldval)

{
  GenBioPagePtr  gbp;

  gbp = (GenBioPagePtr) data;
  if (gbp != NULL) {
    if (oldval >= 0 && oldval <= 3) {
      SafeHide (gbp->orgGrp [oldval]);
    }
    if (newval >= 0 && newval <= 3) {
      SafeShow (gbp->orgGrp [newval]);
    }
    Update ();
  }
}

static void ChangeModSubPage (VoidPtr data, Int2 newval, Int2 oldval)

{
  GenBioPagePtr  gbp;

  gbp = (GenBioPagePtr) data;
  if (gbp != NULL) {
    if (oldval >= 0 && oldval <= 3) {
      SafeHide (gbp->modGrp [oldval]);
    }
    if (newval >= 0 && newval <= 3) {
      SafeShow (gbp->modGrp [newval]);
    }
    Update ();
  }
}

static void ChangeMiscSubPage (VoidPtr data, Int2 newval, Int2 oldval)

{
  GenBioPagePtr  gbp;

  gbp = (GenBioPagePtr) data;
  if (gbp != NULL) {
    if (oldval >= 0 && oldval <= 1) {
      SafeHide (gbp->miscGrp [oldval]);
    }
    if (newval >= 0 && newval <= 1) {
      SafeShow (gbp->miscGrp [newval]);
    }
    Update ();
  }
}

static void LookupTheTaxonomyProc (ButtoN b)

{
  GenBioFormPtr  gfp;

  gfp = (GenBioFormPtr) GetObjectExtra (b);
  if (gfp != NULL && gfp->lookupTaxonomy != NULL) {
    if (gfp->lookupTaxonomy (gfp->input_entityID)) {
      Remove (gfp->form);
      ObjMgrSendMsg (OM_MSG_UPDATE, gfp->input_entityID, 0, 0);
      Update ();
    }
  }
}

static void CleanupBioSourceDialog (GraphiC g, VoidPtr data)

{
  GenBioPagePtr  gbp;

  gbp = (GenBioPagePtr) data;
  if (gbp != NULL) {
    gbp->origTaxName = MemFree (gbp->origTaxName);
    FreeGenBioOrgNameData (gbp);
  }
  StdCleanupExtraProc (g, data);
}

static DialoG CreateBioSourceDialog (GrouP h, CharPtr title, GrouP PNTR pages,
                                     BioSourcePtr biop, GenBioFormPtr gfp,
                                     Boolean diableFocusControl)

{
  ButtoN         b;
  GrouP          c;
  GrouP          f, f1, f2, f3, f4;
  GrouP          g;
  GenBioPagePtr  gbp;
  Int2           height;
  Char           just;
  GrouP          k;
  GrouP          m;
  Int2           max;
  OrgRefPtr      orp;
  GrouP          p;
  PrompT         ppt;
  GrouP          q;
  RecT           r;
  GrouP          s;
  Boolean        showUnstructMods;
  GrouP          t;
  DialoG         tbs;
  GrouP          x;
  PrompT         y;

  p = HiddenGroup (h, 1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  gbp = (GenBioPagePtr) MemNew (sizeof (GenBioPage));
  if (gbp != NULL && pages != NULL) {

    SetObjectExtra (p, gbp, CleanupBioSourceDialog);
    gbp->dialog = (DialoG) p;
    gbp->todialog = BioSourcePtrToGenBioPage;
    gbp->fromdialog = GenBioPageToBioSourcePtr;
    gbp->dialogmessage = BioSourceMessage;
    gbp->testdialog = TestGenBioDialog;
    gbp->orgname_choice = 0;
    gbp->orgname_data = NULL;

    if (title != NULL && title [0] != '\0') {
      s = NormalGroup (p, 0, -2, title, systemFont, NULL);
    } else {
      s = HiddenGroup (p, 0, -2, NULL);
    }
    SetGroupSpacing (s, 10, 10);

    m = HiddenGroup (s, 0, 0, NULL);

    pages [0] = HiddenGroup (m, -1, 0, NULL);
    SetGroupSpacing (pages [0], 10, 10);

    tbs = CreateFolderTabs (pages [0], orgTabs, 0, 0, 0,
                            PROGRAM_FOLDER_TAB,
                            ChangeOrgSubPage, (Pointer) gbp);
    k = HiddenGroup (pages [0], 0, 0, NULL);

    gbp->orgGrp [0] = HiddenGroup (k, -1, 0, NULL);
    SetGroupSpacing (gbp->orgGrp [0], 10, 10);

    gbp->origTaxName = NULL;
    gbp->stripOldName = FALSE;

    g = HiddenGroup (gbp->orgGrp [0], 1, 0, NULL);
    SetGroupSpacing (g, 10, 10);
    f = HiddenGroup (g, 2, 0, NULL);
    SetGroupSpacing (f, 3, 5);
    StaticPrompt (f, "Scientific Name", 0, dialogTextHeight, programFont, 'l');
    gbp->taxName = DialogText (f, "", 20, TaxNameText);
    SetObjectExtra (gbp->taxName, gbp, NULL);
    StaticPrompt (f, "Common Name", 0, dialogTextHeight, programFont, 'l');
    gbp->commonName = (Handle) DialogText (f, "", 10, CommonNameText);
    /*
    gbp->commonName = StaticPrompt (f, "", 10 * stdCharWidth, dialogTextHeight, systemFont, 'l');
    */
    SetObjectExtra (gbp->commonName, gbp, NULL);
    StaticPrompt (f, "", 0, dialogTextHeight, programFont, 'l');
    f = HiddenGroup (g, 1, 0, NULL);
    SelectFont (programFont);
    height = LineHeight ();
    SelectFont (systemFont);
    gbp->orglist = DocumentPanel (f, stdCharWidth * 25, height * 6);
    SetObjectExtra (gbp->orglist, gbp, NULL);
    SetDocAutoAdjust (gbp->orglist, FALSE);
    orgListCol [0].pixWidth = screenRect.right - screenRect.left;
    AppendItem (gbp->orglist, AllButFirstLinePrtProc, orgTxtPtr, FALSE, orgNum,
                &orgListPar, orgListCol, programFont);
    SetDocAutoAdjust (gbp->orglist, TRUE);
    SetDocProcs (gbp->orglist, ClickTaxName, NULL, ReleaseTaxName, NULL);
    SetDocShade (gbp->orglist, NULL, NULL, HighlightTaxName, NULL);

    AlignObjects (ALIGN_RIGHT, (HANDLE) gbp->taxName, (HANDLE) gbp->commonName,
                  (HANDLE) gbp->orglist, NULL);

    gbp->orgGrp [1] = HiddenGroup (k, -1, 0, NULL);
    SetGroupSpacing (gbp->orgGrp [1], 10, 10);

    g = HiddenGroup (gbp->orgGrp [1], -1, 0, NULL);
    SelectFont (programFont);
    f = HiddenGroup (g, 3, 0, NULL);
    StaticPrompt (f, "Location of Sequence",
                  0, popupMenuHeight, programFont, 'l');
    gbp->genome = PopupList (f, TRUE, SetGenome);
    SetObjectExtra (gbp->genome, gbp, NULL);
    gbp->genomeAlist = biosource_genome_alist;
    InitEnumPopup (gbp->genome, gbp->genomeAlist, NULL);
    SetValue (gbp->genome, 2);
    ObjectRect (gbp->orglist, &r);
    MultiLinePrompt (g, useGenomicText, r.right - r.left - 2, programFont);

    f = HiddenGroup (gbp->orgGrp [1], 3, 0, NULL);
    StaticPrompt (f, "Origin of Sequence",
                  0, popupMenuHeight, programFont, 'l');
    gbp->origin = PopupList (f, TRUE, NULL);
    SetObjectExtra (gbp->origin, gbp, NULL);
    InitEnumPopup (gbp->origin, biosource_origin_alist, NULL);
    SetValue (gbp->origin, 0);

    gbp->is_focus = CheckBox (gbp->orgGrp [1], "Biological focus (if multiple source features)", NULL);
    if (diableFocusControl) {
      Disable (gbp->is_focus);
    }
    AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) f, NULL);

    Hide (gbp->orgGrp [1]);

    gbp->orgGrp [2] = HiddenGroup (k, -1, 0, NULL);
    SetGroupSpacing (gbp->orgGrp [2], 10, 10);

    f = HiddenGroup (gbp->orgGrp [2], 2, 0, NULL);
    SetGroupSpacing (f, 10, 5);
    StaticPrompt (f, "Nuclear", 0, popupMenuHeight, programFont, 'l');
    gbp->gcode = PopupList (f, TRUE, NULL);
    PopulateGeneticCodePopup (gbp->gcode);
    SetValue (gbp->gcode, 1);
    StaticPrompt (f, "Mitochondrial", 0, popupMenuHeight, programFont, 'l');
    gbp->mgcode = PopupList (f, TRUE, NULL);
    PopulateGeneticCodePopup (gbp->mgcode);
    SetValue (gbp->mgcode, 1);

    Hide (gbp->orgGrp [2]);

    gbp->orgGrp [3] = HiddenGroup (k, -1, 0, NULL);
    SetGroupSpacing (gbp->orgGrp [3], 10, 10);

    f = HiddenGroup (gbp->orgGrp [3], 0, 2, NULL);
    y = StaticPrompt (f, "Taxonomic Lineage", 0, 0, programFont, 'c');
    gbp->lineage = ScrollText (f, 20, 3, programFont, TRUE, NULL);
    q = HiddenGroup (gbp->orgGrp [3], 2, 0, NULL);
    StaticPrompt (q, "Division", 0, dialogTextHeight, programFont, 'l');
    gbp->gbDiv = DialogText (q, "", 4, NULL);
    c = HiddenGroup (gbp->orgGrp [3], -1, 0, NULL);
    if (gfp != NULL && gfp->lookupTaxonomy != NULL) {
      ppt = StaticPrompt (c, "Looking up taxonomy will close this window.", 0, 0, programFont, 'c');
      b = PushButton (c, "Lookup Taxonomy", LookupTheTaxonomyProc);
      SetObjectExtra (b, gfp, NULL);
      AlignObjects (ALIGN_CENTER, (HANDLE) ppt, (HANDLE) b, NULL);
    }
    AlignObjects (ALIGN_CENTER, (HANDLE) f, (HANDLE) q, (HANDLE) c, NULL);

    Hide (gbp->orgGrp [3]);

    AlignObjects (ALIGN_CENTER, (HANDLE) tbs,
                  (HANDLE) gbp->orgGrp [0], (HANDLE) gbp->orgGrp [1],
                  (HANDLE) gbp->orgGrp [2], (HANDLE) gbp->orgGrp [3], NULL);

    pages [1] = HiddenGroup (m, -1, 0, NULL);
    SetGroupSpacing (pages [1], 10, 10);

    showUnstructMods = FALSE;
    if (biop != NULL) {
      orp = biop->org;
      if (orp != NULL && orp->mod != NULL) {
        showUnstructMods = TRUE;
      }
    }

    if (showUnstructMods) {
      tbs = CreateFolderTabs (pages [1], modTabsUns, 0, 0, 0,
                              PROGRAM_FOLDER_TAB,
                              ChangeModSubPage, (Pointer) gbp);
    } else {
      tbs = CreateFolderTabs (pages [1], modTabs, 0, 0, 0,
                              PROGRAM_FOLDER_TAB,
                              ChangeModSubPage, (Pointer) gbp);
    }
    k = HiddenGroup (pages [1], 0, 0, NULL);

    gbp->modGrp [0] = HiddenGroup (k, -1, 0, NULL);
    SetGroupSpacing (gbp->modGrp [0], 10, 10);

    g = HiddenGroup (gbp->modGrp [0], -1, 0, NULL);
    SetGroupSpacing (g, 3, 10);
    gbp->subsource = CreateTagListDialog (g, 3, 2, STD_TAG_SPACING, subsource_types,
                                          subsource_widths, subsource_alists,
                                          SubSourcePtrToSubsourceDialog,
                                          SubsourceDialogToSubSourcePtr);
    f2 = HiddenGroup (g, 2, 0, NULL);
    SelectFont (programFont);
    max = MaxStringWidths (subsource_extra_prompts) + 2;
    x = MultiLinePrompt (f2, "Additional Source Information", max, programFont);
    gbp->subcomment = ScrollText (f2, 20, 3, programFont, TRUE, NULL);
    AlignObjects (ALIGN_MIDDLE, (HANDLE) x, (HANDLE) gbp->subcomment, NULL);
    AlignObjects (ALIGN_CENTER, (HANDLE) gbp->subsource, (HANDLE) f2, NULL);

    gbp->modGrp [1] = HiddenGroup (k, -1, 0, NULL);
    SetGroupSpacing (gbp->modGrp [1], 10, 10);

    g = HiddenGroup (gbp->modGrp [1], -1, 0, NULL);
    SetGroupSpacing (g, 3, 10);
    gbp->orgmod = CreateTagListDialog (g, 3, 2, STD_TAG_SPACING, orgmod_types,
                                       orgmod_widths, orgmod_alists,
                                       OrgModPtrToOrgmodDialog,
                                       OrgmodDialogToOrgModPtr);
    f1 = HiddenGroup (g, 2, 0, NULL);
    SelectFont (programFont);
    max = MaxStringWidths (orgmod_extra_prompts) + 2;
    x = MultiLinePrompt (f1, "Additional Organism Information", max, programFont);
    gbp->orgcomment = ScrollText (f1, 20, 3, programFont, TRUE, NULL);
    AlignObjects (ALIGN_MIDDLE, (HANDLE) x, (HANDLE) gbp->orgcomment, NULL);
    f4 = HiddenGroup (g, 1, 0, NULL);
    gbp->add_type_strain_to_orgcomment = PushButton (f4, "Add type strain to comment", AddTypeStrainButtonProc);
    SetObjectExtra (gbp->add_type_strain_to_orgcomment, gbp, NULL);
    AlignObjects (ALIGN_CENTER, (HANDLE) gbp->orgmod, (HANDLE) f1, (HANDLE) f4, NULL);

    Hide (gbp->modGrp [1]);

    gbp->modGrp [2] = HiddenGroup (k, -1, 0, NULL);
    SetGroupSpacing (gbp->modGrp [2], 10, 10);

    g = HiddenGroup (gbp->modGrp [2], 2, 0, NULL);
    SetGroupSpacing (g, 3, 10);

    StaticPrompt (g, "Assigned Acronym", 0, stdLineHeight, programFont, 'l');
    gbp->gbacr = StaticPrompt (g, "", 15 * stdCharWidth, stdLineHeight, systemFont, 'l');
    StaticPrompt (g, "Assigned Anamorph", 0, stdLineHeight, programFont, 'l');
    gbp->gbana = StaticPrompt (g, "", 15 * stdCharWidth, stdLineHeight, systemFont, 'l');
    StaticPrompt (g, "Assigned Synonym", 0, stdLineHeight, programFont, 'l');
    gbp->gbsyn = StaticPrompt (g, "", 15 * stdCharWidth, stdLineHeight, systemFont, 'l');

    Hide (gbp->modGrp [2]);

    gbp->modGrp [3] = HiddenGroup (k, -1, 0, NULL);
    SetGroupSpacing (gbp->modGrp [3], 10, 10);

    if (showUnstructMods) {
      f3 = HiddenGroup (gbp->modGrp [3], 0, 2, NULL);
      StaticPrompt (f3, "Unstructured Modifiers", 0, 0, programFont, 'c');
      gbp->mod = CreateVisibleStringDialog (f3, 3, -1, 15);
    }

    Hide (gbp->modGrp [3]);

    AlignObjects (ALIGN_CENTER, (HANDLE) tbs,
                  (HANDLE) gbp->modGrp [0],
                  (HANDLE) gbp->modGrp [1],
                  (HANDLE) gbp->modGrp [2],
                  (HANDLE) gbp->modGrp [3], NULL);

    Hide (pages [1]);

    pages [2] = HiddenGroup (m, -1, 0, NULL);
    SetGroupSpacing (pages [2], 10, 10);

    tbs = CreateFolderTabs (pages [2], miscTabs, 0, 0, 0,
                            PROGRAM_FOLDER_TAB,
                            ChangeMiscSubPage, (Pointer) gbp);
    k = HiddenGroup (pages [2], 0, 0, NULL);

    gbp->miscGrp [0] = HiddenGroup (k, -1, 0, NULL);
    SetGroupSpacing (gbp->miscGrp [0], 10, 10);

    f1 = HiddenGroup (gbp->miscGrp [0], 0, 2, NULL);
    StaticPrompt (f1, "Synonyms", 0, 0, programFont, 'c');
    gbp->syn = CreateVisibleStringDialog (f1, 3, -1, 15);

    gbp->miscGrp [1] = HiddenGroup (k, -1, 0, NULL);
    SetGroupSpacing (gbp->miscGrp [1], 10, 10);

    f2 = HiddenGroup (gbp->miscGrp [1], -1, 0, NULL);
    SetGroupSpacing (f2, 10, 10);
    if (GetAppProperty ("ReadOnlyDbTags") == NULL) {
      just = 'c';
    } else {
      just = 'l';
      StaticPrompt (f2, "This page is read-only", 15 * stdCharWidth, 0, programFont, 'c');
    }
    t = HiddenGroup (f2, 2, 0, NULL);
    StaticPrompt (t, "Database", 7 * stdCharWidth, 0, programFont, just);
    StaticPrompt (t, "Object ID", 8 * stdCharWidth, 0, programFont, just);
    gbp->db = CreateDbtagDialog (f2, 3, -1, 7, 8);

    Hide (gbp->miscGrp [1]);

    AlignObjects (ALIGN_CENTER, (HANDLE) tbs,
                  (HANDLE) gbp->miscGrp [0],
                  (HANDLE) gbp->miscGrp [1], NULL);

    Hide (pages [2]);


    AlignObjects (ALIGN_CENTER, (HANDLE) pages [ORGANISM_PAGE],
                  (HANDLE) pages [MODIFIERS_PAGE],
                  (HANDLE) pages [MISCELLANEOUS_PAGE], NULL);
  }

  return (DialoG) p;
}

static void SetBioSourceImportExportItems (GenBioFormPtr gfp)

{
  IteM  exportItm;
  IteM  importItm;

  if (gfp != NULL) {
    importItm = FindFormMenuItem ((BaseFormPtr) gfp, VIB_MSG_IMPORT);
    exportItm = FindFormMenuItem ((BaseFormPtr) gfp, VIB_MSG_EXPORT);
    switch (gfp->currentPage) {
      case ORGANISM_PAGE :
        SafeSetTitle (importItm, "Import BioSource...");
        SafeSetTitle (exportItm, "Export BioSource...");
        SafeEnable (importItm);
        SafeEnable (exportItm);
        break;
      case MODIFIERS_PAGE :
        SafeSetTitle (importItm, "Import...");
        SafeSetTitle (exportItm, "Export...");
        SafeDisable (importItm);
        SafeDisable (exportItm);
        break;
      case MISCELLANEOUS_PAGE :
        SafeSetTitle (importItm, "Import...");
        SafeSetTitle (exportItm, "Export...");
        SafeDisable (importItm);
        SafeDisable (exportItm);
        break;
      case COMMON_PAGE :
        SafeSetTitle (importItm, "Import...");
        SafeSetTitle (exportItm, "Export...");
        SafeDisable (importItm);
        SafeDisable (exportItm);
        break;
      case LOCATION_PAGE :
        SafeSetTitle (importItm, "Import SeqLoc...");
        SafeSetTitle (exportItm, "Export SeqLoc...");
        SafeEnable (importItm);
        SafeEnable (exportItm);
        break;
      default :
        break;
    }
  }
}

static void ChangeBioSourcePage (VoidPtr data, Int2 newval, Int2 oldval)

{
  GenBioFormPtr  gfp;

  gfp = (GenBioFormPtr) data;
  if (gfp != NULL) {
    gfp->currentPage = newval;
    SafeHide (gfp->pages [oldval]);
    SafeShow (gfp->pages [newval]);
    switch (newval) {
      case ORGANISM_PAGE :
        break;
      case MODIFIERS_PAGE :
        break;
      case MISCELLANEOUS_PAGE :
        break;
      case COMMON_PAGE :
        break;
      case LOCATION_PAGE :
        SendMessageToDialog (gfp->location, VIB_MSG_ENTER);
        break;
      default :
        break;
    }
    SetBioSourceImportExportItems (gfp);
    Update ();
  }
}

static Boolean ImportBioSourceForm (ForM f, CharPtr filename)

{
  AsnIoPtr       aip;
  BioSourcePtr   biop;
  GenBioFormPtr  gfp;
  Char           path [PATH_MAX];

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  gfp = (GenBioFormPtr) GetObjectExtra (f);
  if (gfp != NULL) {
    switch (gfp->currentPage) {
      case ORGANISM_PAGE :
        if (path [0] != '\0' || GetInputFileName (path, sizeof (path), "", "TEXT")) {
          aip = AsnIoOpen (path, "r");
          if (aip != NULL) {
            biop = BioSourceAsnRead (aip, NULL);
            AsnIoClose (aip);
            if (biop != NULL) {
              PointerToDialog (gfp->data, (Pointer) biop);
              biop = BioSourceFree (biop);
              Update ();
              return TRUE;
            }
          }
        }
        break;
      case LOCATION_PAGE :
        return ImportDialog (gfp->location, filename);
      default :
        break;
    }
  }
  return FALSE;
}

static Boolean ExportBioSourceForm (ForM f, CharPtr filename)

{
  AsnIoPtr       aip;
  BioSourcePtr   biop;
  GenBioFormPtr  gfp;
  Char           path [PATH_MAX];
#ifdef WIN_MAC
  FILE           *fp;
#endif

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  gfp = (GenBioFormPtr) GetObjectExtra (f);
  if (gfp != NULL) {
    switch (gfp->currentPage) {
      case ORGANISM_PAGE :
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
            biop = DialogToPointer (gfp->data);
            BioSourceAsnWrite (biop, aip, NULL);
            AsnIoClose (aip);
            biop = BioSourceFree (biop);
            return TRUE;
          }
        }
        break;
      case LOCATION_PAGE :
        return ExportDialog (gfp->location, filename);
      default :
        break;
    }
  }
  return FALSE;
}

static CharPtr  biosourceDescFormTabs [] = {
  "Organism", "Modifiers", "Miscellaneous", NULL
};

static void BioSourceDescFormMessage (ForM f, Int2 mssg)

{
  GenBioFormPtr  gfp;

  gfp = (GenBioFormPtr) GetObjectExtra (f);
  if (gfp != NULL) {
    switch (mssg) {
      case VIB_MSG_INIT :
        SendMessageToDialog (gfp->data, VIB_MSG_INIT);
        break;
      case VIB_MSG_IMPORT :
        ImportBioSourceForm (f, NULL);
        break;
      case VIB_MSG_EXPORT :
        ExportBioSourceForm (f, NULL);
        break;
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
        if (gfp->appmessage != NULL) {
          gfp->appmessage (f, mssg);
        }
        break;
    }
  }
}

static void BioSourceFormActivate (WindoW w)

{
  GenBioFormPtr  gfp;

  gfp = (GenBioFormPtr) GetObjectExtra (w);
  if (gfp != NULL) {
    if (gfp->activate != NULL) {
      gfp->activate (w);
    }
    SetBioSourceImportExportItems (gfp);
  }
}

static Boolean OkayToAcceptBioSource (ButtoN b)

{
  Boolean        abort;
  MsgAnswer      ans;
  OrgModPtr      curr;
  GenBioPagePtr  gbp;
  GenBioFormPtr  gfp;
  OrgModPtr      mod;
  CharPtr        str;

  gfp = (GenBioFormPtr) GetObjectExtra (b);
  if (gfp != NULL) {
    gbp = (GenBioPagePtr) GetObjectExtra (gfp->data);
    if (gbp != NULL) {
      mod = DialogToPointer (gbp->orgmod);
      if (mod == NULL) return TRUE;
      abort = TRUE;
      for (curr = mod; curr != NULL; curr = curr->next) {
        if (curr->subtype == 254) {
          abort = FALSE;
        }
      }
      OrgModFree (mod);
      if (abort) return TRUE;
      str = SaveStringFromText (gbp->taxName);
      ans = ANS_NO;
      if (StringICmp (str, gbp->origTaxName) != 0) {
        ans = Message (MSG_YNC, "Delete original name (necessary for correct lookup)?");
      }
      MemFree (str);
      if (ans == ANS_CANCEL) return FALSE;
      if (ans == ANS_YES) {
        gbp->stripOldName = TRUE;
      }
    }
  }
  return TRUE;
}

static void BioSourceDescFormAcceptButtonProc (ButtoN b)

{
  if (OkayToAcceptBioSource (b)) {
    StdAcceptFormButtonProc (b);
  }
}

extern ForM CreateBioSourceDescForm (Int2 left, Int2 top, Int2 width,
                                     Int2 height, CharPtr title, ValNodePtr sdp,
                                     SeqEntryPtr sep, FormActnFunc actproc,
                                     BioSourceEditProcsPtr bepp)

{
  ButtoN             b;
  BioSourcePtr       biop;
  GrouP              c;
  GrouP              g;
  GenBioFormPtr      gfp;
  StdEditorProcsPtr  sepp;
  WindoW             w;

  w = NULL;
  gfp = (GenBioFormPtr) MemNew (sizeof (GenBioForm));
  if (gfp != NULL) {
    w = FixedWindow (left, top, width, height, title, StdCloseWindowProc);
    SetObjectExtra (w, gfp, StdDescFormCleanupProc);
    gfp->form = (ForM) w;
    gfp->actproc = actproc;
    gfp->toform = NULL;
    gfp->fromform = NULL;
    gfp->testform = NULL;
    gfp->formmessage = BioSourceDescFormMessage;
    gfp->importform = ImportBioSourceForm;
    gfp->exportform = ExportBioSourceForm;

#ifndef WIN_MAC
    CreateStdEditorFormMenus (w);
#endif

    if (bepp == NULL) {
      bepp = (BioSourceEditProcsPtr) GetAppProperty ("BioSourcEditForm");
    }
    if (bepp != NULL) {
      gfp->lookupTaxonomy = bepp->lookupTaxonomy;
    }
    sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
    if (sepp != NULL) {
      gfp->activate = sepp->activateForm;
      gfp->appmessage = sepp->handleMessages;
    }
    SetActivate (w, BioSourceFormActivate);

    g = HiddenGroup (w, -1, 0, NULL);
    SetGroupSpacing (g, 3, 10);

    gfp->foldertabs = CreateFolderTabs (g, biosourceDescFormTabs, ORGANISM_PAGE,
                                        0, 0, SYSTEM_FOLDER_TAB,
                                        ChangeBioSourcePage, (Pointer) gfp);
    gfp->currentPage = ORGANISM_PAGE;

    biop = NULL;
    if (sdp != NULL && sdp->choice == Seq_descr_source) {
      biop = sdp->data.ptrvalue;
    }
    gfp->data = CreateBioSourceDialog (g, NULL, gfp->pages, biop, gfp, FALSE);

    AlignObjects (ALIGN_CENTER, (HANDLE) gfp->foldertabs, (HANDLE) gfp->data, NULL);

    c = HiddenGroup (w, 2, 0, NULL);
    b = PushButton (c, "Accept", BioSourceDescFormAcceptButtonProc);
    SetObjectExtra (b, gfp, NULL);
    PushButton (c, "Cancel", StdCancelButtonProc);
    AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, NULL);
    RealizeWindow (w);

    SendMessageToDialog (gfp->data, VIB_MSG_INIT);
    Show (gfp->pages [gfp->currentPage]);
    SendMessageToDialog (gfp->data, VIB_MSG_ENTER);
    Update ();
  }
  return (ForM) w;
}

static CharPtr  biosourceFeatFormTabs [] = {
  "Organism", "Modifiers", "Miscellaneous", "Properties", "Location", NULL
};

static void BioSourceFeatFormMessage (ForM f, Int2 mssg)

{
  GenBioFormPtr  gfp;

  gfp = (GenBioFormPtr) GetObjectExtra (f);
  if (gfp != NULL) {
    switch (mssg) {
      case VIB_MSG_INIT :
        SendMessageToDialog (gfp->data, VIB_MSG_INIT);
        StdInitFeatFormProc (f);
        break;
      case VIB_MSG_IMPORT :
        ImportBioSourceForm (f, NULL);
        break;
      case VIB_MSG_EXPORT :
        ExportBioSourceForm (f, NULL);
        break;
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
        if (gfp->appmessage != NULL) {
          gfp->appmessage (f, mssg);
        }
        break;
    }
  }
}

static void BioSourceFeatFormAcceptButtonProc (ButtoN b)

{
  if (OkayToAcceptBioSource (b)) {
    StdFeatFormAcceptButtonProc (b);
  }
}

extern ForM CreateBioSourceFeatForm (Int2 left, Int2 top, Int2 width,
                                     Int2 height, CharPtr title, SeqFeatPtr sfp,
                                     SeqEntryPtr sep, FormActnFunc actproc,
                                     BioSourceEditProcsPtr bepp)

{
  ButtoN             b;
  BioSourcePtr       biop;
  GrouP              c;
  GrouP              g;
  GenBioFormPtr      gfp;
  GrouP              h;
  GrouP              s;
  StdEditorProcsPtr  sepp;
  WindoW             w;

  w = NULL;
  gfp = (GenBioFormPtr) MemNew (sizeof (GenBioForm));
  if (gfp != NULL) {
    w = FixedWindow (left, top, width, height, title, StdCloseWindowProc);
    SetObjectExtra (w, gfp, StdFeatFormCleanupProc);
    gfp->form = (ForM) w;
    gfp->actproc = actproc;
    gfp->toform = StdSeqFeatPtrToFeatFormProc;
    gfp->fromform = NULL;
    gfp->testform = NULL;
    gfp->formmessage = BioSourceFeatFormMessage;
    gfp->importform = ImportBioSourceForm;
    gfp->exportform = ExportBioSourceForm;

#ifndef WIN_MAC
    CreateStdEditorFormMenus (w);
#endif

    if (bepp == NULL) {
      bepp = (BioSourceEditProcsPtr) GetAppProperty ("BioSourcEditForm");
    }
    if (bepp != NULL) {
      gfp->lookupTaxonomy = bepp->lookupTaxonomy;
    }
    sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
    if (sepp != NULL) {
      gfp->activate = sepp->activateForm;
      gfp->appmessage = sepp->handleMessages;
    }
    SetActivate (w, BioSourceFormActivate);

    g = HiddenGroup (w, -1, 0, NULL);
    SetGroupSpacing (g, 3, 10);

    gfp->foldertabs = CreateFolderTabs (g, biosourceFeatFormTabs, ORGANISM_PAGE,
                                        0, 0, SYSTEM_FOLDER_TAB,
                                        ChangeBioSourcePage, (Pointer) gfp);
    gfp->currentPage = ORGANISM_PAGE;

    h = HiddenGroup (g, 0, 0, NULL);

    biop = NULL;
    if (sfp != NULL && sfp->data.choice == SEQFEAT_BIOSRC) {
      biop = sfp->data.value.ptrvalue;
    }
    gfp->data = CreateBioSourceDialog (h, NULL, gfp->pages, biop, gfp, TRUE);
 
    s = HiddenGroup (h, -1, 0, NULL);
    CreateCommonFeatureGroup (s, (FeatureFormPtr) gfp, sfp, FALSE, TRUE);
    gfp->pages [COMMON_PAGE] = s;
    Hide (gfp->pages [COMMON_PAGE]);

    s = HiddenGroup (h, -1, 0, NULL);
    gfp->location = CreateIntervalEditorDialogEx (s, NULL, 4, 2, sep, TRUE, TRUE,
                                                  TRUE, TRUE, FALSE,
                                                  (FeatureFormPtr) gfp,
                                                  StdFeatIntEdPartialCallback);
    gfp->pages [LOCATION_PAGE] = s;
    Hide (gfp->pages [LOCATION_PAGE]);

    AlignObjects (ALIGN_CENTER, (HANDLE) gfp->data,
                  (HANDLE) gfp->pages [COMMON_PAGE],
                  (HANDLE) gfp->pages [LOCATION_PAGE], NULL);
    AlignObjects (ALIGN_CENTER, (HANDLE) gfp->foldertabs, (HANDLE) h, NULL);

    c = HiddenGroup (w, 2, 0, NULL);
    b = PushButton (c, "Accept", BioSourceFeatFormAcceptButtonProc);
    SetObjectExtra (b, gfp, NULL);
    PushButton (c, "Cancel", StdCancelButtonProc);
    AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, NULL);
    RealizeWindow (w);

    SendMessageToDialog (gfp->data, VIB_MSG_INIT);
    SendMessageToDialog (gfp->location, VIB_MSG_INIT);
    Show (gfp->pages [gfp->currentPage]);
    SendMessageToDialog (gfp->data, VIB_MSG_ENTER);
    Update ();
  }
  return (ForM) w;
}

extern Int2 LIBCALLBACK BioSourceGenFunc (Pointer data)

{
  GenBioFormPtr     gfp;
  HelpMessageFunc   helpfunc;
  Uint2             itemtype;
  OMProcControlPtr  ompcp;
  OMUserDataPtr     omudp;
  ObjMgrProcPtr     proc;
  ValNodePtr        sdp;
  SeqEntryPtr       sep;
  SeqFeatPtr        sfp;
  Uint2             subtype;
  WindoW            w;

  ompcp = (OMProcControlPtr) data;
  w = NULL;
  sdp = NULL;
  sfp = NULL;
  sep = NULL;
  itemtype = 0;
  subtype = 0;
  if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;
  proc = ompcp->proc;
  switch (ompcp->input_itemtype) {
    case OBJ_SEQFEAT :
      sfp = (SeqFeatPtr) ompcp->input_data;
      if (sfp != NULL && sfp->data.choice != SEQFEAT_BIOSRC) {
        return OM_MSG_RET_ERROR;
      }
      itemtype = OBJ_SEQFEAT;
      subtype = FEATDEF_BIOSRC;
      break;
    case OBJ_SEQDESC :
      sdp = (ValNodePtr) ompcp->input_data;
      if (sdp != NULL && sdp->choice != Seq_descr_source) {
        return OM_MSG_RET_ERROR;
      }
      itemtype = OBJ_SEQDESC;
      subtype = Seq_descr_source;
      break;
    case OBJ_BIOSEQ :
      break;
    case OBJ_BIOSEQSET :
      break;
    case 0 :
      break;
    default :
      return OM_MSG_RET_ERROR;
  }
  omudp = ItemAlreadyHasEditor (ompcp->input_entityID, ompcp->input_itemID,
                                ompcp->input_itemtype, ompcp->proc->procid);
  if (omudp != NULL) {
    gfp = (GenBioFormPtr) omudp->userdata.ptrvalue;
    if (gfp != NULL) {
      Select (gfp->form);
    }
    return OM_MSG_RET_DONE;
  }
  sep = GetTopSeqEntryForEntityID (ompcp->input_entityID);
  if (sfp != NULL) {
    w = (WindoW) CreateBioSourceFeatForm (-50, -33, -10, -10,
                                          "Organism Information", sfp, sep,
                                          StdFeatFormActnProc, NULL);
  } else if (sdp != NULL) {
    w = (WindoW) CreateBioSourceDescForm (-50, -33, -10, -10,
                                          "Organism Information", sdp, sep,
                                          StdDescFormActnProc, NULL);
  } else {
    itemtype = proc->inputtype;
    subtype = proc->subinputtype;
    if (itemtype == OBJ_SEQFEAT && subtype == FEATDEF_BIOSRC) {
      w = (WindoW) CreateBioSourceFeatForm (-50, -33, -10, -10,
                                            "Organism Information", sfp, sep,
                                            StdFeatFormActnProc, NULL);
    } else if (itemtype == OBJ_SEQDESC && subtype == Seq_descr_source) {
      w = (WindoW) CreateBioSourceDescForm (-50, -33, -10, -10,
                                            "Organism Information", sdp, sep,
                                            StdDescFormActnProc, NULL);
    } else {
      return OM_MSG_RET_ERROR;
    }
  }
  gfp = (GenBioFormPtr) GetObjectExtra (w);
  if (gfp != NULL) {
    gfp->input_entityID = ompcp->input_entityID;
    gfp->input_itemID = ompcp->input_itemID;
    gfp->input_itemtype = ompcp->input_itemtype;
    gfp->this_itemtype = itemtype;
    gfp->this_subtype = subtype;
    gfp->procid = ompcp->proc->procid;
    gfp->proctype = ompcp->proc->proctype;
    gfp->userkey = OMGetNextUserKey ();
    omudp = ObjMgrAddUserData (ompcp->input_entityID, ompcp->proc->procid,
	                           OMPROC_EDIT, gfp->userkey);
    if (omudp != NULL) {
      omudp->userdata.ptrvalue = (Pointer) gfp;
      omudp->messagefunc = StdVibrantEditorMsgFunc;
    }
    SendMessageToForm (gfp->form, VIB_MSG_INIT);
    if (sdp != NULL) {
      PointerToDialog (gfp->data, (Pointer) sdp->data.ptrvalue);
      SetClosestParentIfDuplicating ((BaseFormPtr) gfp);
    } else if (sfp != NULL) {
      PointerToForm (gfp->form, (Pointer) sfp);
      SetClosestParentIfDuplicating ((BaseFormPtr) gfp);
    } else if (itemtype == OBJ_SEQFEAT) {
      SetNewFeatureDefaultInterval ((FeatureFormPtr) gfp);
    }
  }
  Show (w);
  Select (w);
  helpfunc = (HelpMessageFunc) GetAppProperty ("HelpMessageProc");
  if (helpfunc != NULL) {
    helpfunc ("Biological Source", NULL);
  }
  return OM_MSG_RET_DONE;
}

