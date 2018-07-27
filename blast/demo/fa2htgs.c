/*****************************************************************************
*
*   fa2htgs.c
*      This is a framework for reading an ASN.1 Seq-entry or Bioseq-set,
*       doing some processing on it, and outputting it again. seqport.h
*       is included since it covers most necessary utilities that you might
*       need. You may need to add others for specialized reports and so on.
*
*       The check for aipout == NULL is to show how to change the code if
*         no output is desired. Change the default in myargs from "stdout" to
*         NULL to make output command line optional.
*
*       This program can be used "as is" to convert between binary and text
*        ASN.1 through the object loaders.
*
*   -- original version from Jim Ostell
*   -- version 1.3, add "-m" and "-u" parameters   Hsiu-Chuan  2-27-97
*   -- version 1.4, add "-x" parameter             Hsiu-Chuan  4-10-97 
*   -- test ci to CVS in ncbi demo directory       Hsiu-Chuan  5-1-97
*   -- version 1.5, add "-C" for clone-lib
*                       "-M" for map
*                       "-O" for comment from a file
*                       "-T" for phrap file          Hsiu-Chuan  1-30-98
*                       "-P" for contig names
*                       "-A" for accession list file  Kans  4-8-98
*                       "-X" for coordinates on resulting master or individual accessions
*
*****************************************************************************/
#include <subutil.h>
#include <tofasta.h>
#include <sqnutils.h>

#define NUMARG 34
Args myargs[NUMARG] = {
   {"Filename for fasta input","stdin",NULL,NULL,TRUE,'i',ARG_FILE_IN,0.0,0,NULL},
   {"Filename for Seq-submit template","template.sub",NULL,NULL,FALSE,'t',ARG_FILE_IN,0.0,0,NULL},
   {"Filename for asn.1 output","stdout", NULL,NULL,TRUE,'o',ARG_FILE_OUT,0.0,0,NULL},
   {"Log errors to file named:",NULL,NULL,NULL,TRUE,'e',ARG_FILE_OUT,0.0,0,NULL} ,
   {"Organism name?","Homo sapiens", NULL ,NULL ,TRUE,'n',ARG_STRING,0.0,0,NULL},
   {"Sequence name?",NULL, NULL ,NULL ,FALSE,'s',ARG_STRING,0.0,0,NULL},
   {"length of sequence in bp?","0", NULL ,NULL ,FALSE,'l',ARG_INT,0.0,0,NULL},
   {"Genome Center tag?",NULL, NULL ,NULL ,FALSE,'g',ARG_STRING,0.0,0,NULL},
   {"HTGS phase?","1", "0" ,"3" ,FALSE,'p',ARG_INT,0.0,0,NULL},
   {"GenBank accession (if an update)",NULL, NULL ,NULL ,TRUE,'a',ARG_STRING,0.0,0,NULL},
   {"Remark for update?",NULL, NULL ,NULL ,TRUE,'r',ARG_STRING,0.0,0,NULL},
   {"Clone name?",NULL, NULL ,NULL ,TRUE,'c',ARG_STRING,0.0,0,NULL},
   {"Chromosome?",NULL, NULL ,NULL ,TRUE,'h',ARG_STRING,0.0,0,NULL},
   {"Title for sequence?",NULL, NULL ,NULL ,TRUE,'d',ARG_STRING,0.0,0,NULL},
   {"Take comment from template ?","F", NULL ,NULL ,TRUE,'m',ARG_BOOLEAN,0.0,0,NULL},
   {"Take biosource from template ?","F", NULL ,NULL ,TRUE,'u',ARG_BOOLEAN,0.0,0,NULL},
   {"Secondary accession number, separate by commas if multiple, s.t. U10000,L11000", NULL, NULL ,NULL ,TRUE,'x',ARG_STRING,0.0,0,NULL},
   {"Clone library name?",NULL, NULL ,NULL ,TRUE,'C',ARG_STRING,0.0,0,NULL},
   {"Map?",NULL, NULL ,NULL ,TRUE,'M',ARG_STRING,0.0,0,NULL},
   {"Filename for the comment:",NULL,NULL,NULL,TRUE,'O',ARG_FILE_IN,0.0,0,NULL} ,
   {"Filename for phrap input",NULL,NULL,NULL,TRUE,'T',ARG_FILE_IN,0.0,0,NULL} ,
   {"Contigs to use, separate by commas if multiple", NULL, NULL ,NULL ,TRUE,'P',ARG_STRING,0.0,0,NULL},
   {"Filename for accession list input",NULL,NULL,NULL,TRUE,'A',ARG_FILE_IN,0.0,0,NULL} ,
   {"Coordinates are on the resulting sequence ?","F", NULL ,NULL ,TRUE,'X',ARG_BOOLEAN,0.0,0,NULL},
   {"HTGS_DRAFT sequence?","F", NULL ,NULL ,TRUE,'D',ARG_BOOLEAN,0.0,0,NULL},
   {"Strain name?",NULL, NULL ,NULL ,TRUE,'S',ARG_STRING,0.0,0,NULL},
   {"Gap length","100", "0" ,"1000000000" ,FALSE,'b',ARG_INT,0.0,0,NULL},
   {"Annotate assembly_fragments","F", NULL ,NULL ,TRUE,'N',ARG_BOOLEAN,0.0,0,NULL},
   {"SP6 clone (e.g., Contig1,left)",NULL, NULL ,NULL ,TRUE,'6',ARG_STRING,0.0,0,NULL},
   {"T7 clone (e.g., Contig2,right)",NULL, NULL ,NULL ,TRUE,'7',ARG_STRING,0.0,0,NULL},
   {"Filename for phrap contig order",NULL,NULL,NULL,TRUE,'L',ARG_FILE_IN,0.0,0,NULL},
   {"htgs_fulltop keyword","F", NULL ,NULL ,TRUE,'f',ARG_BOOLEAN,0.0,0,NULL},
   {"htgs_activefin keyword","F", NULL ,NULL ,TRUE,'v',ARG_BOOLEAN,0.0,0,NULL},
   {"htgs_cancelled keyword","F", NULL ,NULL ,TRUE,'q',ARG_BOOLEAN,0.0,0,NULL},
};

/*------------- MakeAc2GBSeqId() -----------------------*/
/***************************************************************
*   MakeAc2GBSeqId:
*   -- return NULL if acnum == null
*                                             Hsiu-Chuan 4-18-97
****************************************************************/
static SeqIdPtr  MakeAc2GBSeqId(CharPtr accession)
{
   TextSeqIdPtr tsip;
   SeqIdPtr sip;

   if (accession == NULL || *accession == '\0')
      return NULL;

   sip = ValNodeNew(NULL);
   sip->choice = SEQID_GENBANK;
   tsip = TextSeqIdNew();
   sip->data.ptrvalue = tsip;
   tsip->accession = StringSave(accession);

   return sip;

} /* MakeAc2GBSeqId */

/*----------- AddExtraAc2Entry() ----------------------------*/
/***************************************************************
*   AddExtraAc2Entry:
*                                             Hsiu-Chuan 4-11-97
****************************************************************/
static Boolean AddExtraAc2Entry (SeqEntryPtr entry , CharPtr extra_ac )
{
   BioseqPtr  bsp;
   ValNodePtr vnp;
   GBBlockPtr gbp;
   Char       acnum[17];
   CharPtr    p;
   Int4       i, j;
   SeqHistPtr shp;
   SeqIdPtr   sip;

   if ((entry == NULL) || (extra_ac == NULL))
      return FALSE;

   bsp = (BioseqPtr)(entry->data.ptrvalue);

   for (gbp= NULL, vnp = bsp->descr; vnp != NULL; vnp = vnp->next)
   {
       if (vnp->choice == Seq_descr_genbank)
       {
          gbp = vnp->data.ptrvalue;
          break;
       }
   }

   shp = bsp->hist; 

   if (gbp == NULL)
   {
      vnp = (ValNodePtr) NewDescrOnSeqEntry (entry, Seq_descr_genbank);
      gbp = GBBlockNew();
      vnp->data.ptrvalue = (Pointer)gbp;
   }
   
   for (p = extra_ac; *p != '\0';)
   {
       for (i = 0; isalnum(*p) && *p != '\0'; ++p, ++i)
           acnum[i] = *p;
       acnum[i] = '\0'; 
               /* check one_letter+5digits or two_letter+6digits */
       if (i == 6 || i == 8)
       {
          if (!isalpha(acnum[0]) || (!(isdigit(acnum[1]) && i == 6) &&
              !(isalpha(acnum[1]) && i == 8)))
          {
             ErrPostEx(SEV_ERROR,0,0,
 "Invalid accession (one_letter+5digits or two_letter+6digits): %s",
                                                           acnum);
             return FALSE;
          }

          for (j = 2; j < i; ++j)
          {
              if (!(isdigit(acnum[j])))
              {
                 ErrPostEx(SEV_ERROR,0,0,
 "Invalid accession (one_letter+5digits or two_letter+6digits): %s",
                                                           acnum);
                 return FALSE;
              }
          }

          ValNodeCopyStr(&gbp->extra_accessions, 0, acnum);
          sip = MakeAc2GBSeqId (acnum);
          if (shp == NULL)
          {
             shp = SeqHistNew();
             bsp->hist = shp;
          }
          ValNodeLink(&shp->replace_ids, sip);
       }
       else
       {
          ErrPostEx(SEV_ERROR,0,0,
 "Invalid accession (one_letter+5digits or two_letter+6digits): %s",
                                                           acnum);
          return FALSE;
       }

       while (!isalnum(*p) && *p != '\0')
           ++p;
   }

   return TRUE;

} /* AddExtraAc2Entry */

typedef struct resqseqgph {
  Int2         index;
  SeqGraphPtr  sgp;
} ResqSeqgph, PNTR ResqSeqgphPtr;

static void RescueSeqGraphs (BioseqPtr bsp, Int2 index, ValNodePtr PNTR vnpp)

{
  SeqAnnotPtr    nextsap;
  SeqGraphPtr    nextsgp;
  Pointer PNTR   prevsap;
  Pointer PNTR   prevsgp;
  ResqSeqgphPtr  rsp;
  SeqAnnotPtr    sap;
  SeqGraphPtr    sgp;

  if (bsp == NULL || vnpp == NULL) return;
  sap = bsp->annot;
  prevsap = (Pointer PNTR) &(bsp->annot);
  while (sap != NULL) {
    nextsap = sap->next;
    if (sap->type == 3) {
      sgp = (SeqGraphPtr) sap->data;
      prevsgp = (Pointer PNTR) &(sap->data);
      while (sgp != NULL) {
        nextsgp = sgp->next;
        *(prevsgp) = sgp->next;
        sgp->next = NULL;
        rsp = (ResqSeqgphPtr) MemNew (sizeof (ResqSeqgph));
        rsp->index = index;
        rsp->sgp = sgp;
        ValNodeAddPointer (vnpp, 0, (Pointer) rsp);
        sgp = nextsgp;
      }
    }
    if (sap->data == NULL) {
      *(prevsap) = sap->next;
      sap->next = NULL;
      SeqAnnotFree (sap);
    } else {
      prevsap = (Pointer PNTR) &(sap->next);
    }
    sap = nextsap;
  }
}

static Boolean HasNoText (CharPtr str)

{
  Char  ch;

  if (str != NULL) {
    ch = *str;
    while (ch != '\0') {
      if (ch > ' ') {
        return FALSE;
      }
      str++;
      ch = *str;
    }
  }
  return TRUE;
}

static SeqAnnotPtr NewSeqAnnotType3 (CharPtr name, SeqGraphPtr sgp)

{
  SeqAnnotPtr  sap = NULL;

  if (sgp == NULL) return NULL;
  sap = SeqAnnotNew ();
  if (sap == NULL) return NULL;

  if (! HasNoText (name)) {
    ValNodeAddPointer (&(sap->desc), Annot_descr_name, StringSave (name));
  }
  sap->type = 3;
  sap->data = (Pointer) sgp;

  return sap;
}

static void OffsetAndLinkSeqGraph (BioseqPtr bsp, SeqGraphPtr sgp, Int2 index)

{
  DeltaSeqPtr  dsp;
  SeqGraphPtr  lastsgp;
  Int4         len;
  SeqLitPtr    litp;
  SeqAnnotPtr  sap;
  SeqIntPtr    sintp;
  SeqLocPtr    slp;

  if (bsp == NULL || sgp == NULL || index < 1) return;
  len = 0;
  if (bsp->repr == Seq_repr_delta && bsp->seq_ext_type == 4) {
    for (dsp = (DeltaSeqPtr) (bsp->seq_ext);
         dsp != NULL && index > 1; dsp = dsp->next, index--) {
      if (dsp->choice == 1) {
        len += SeqLocLen ((SeqLocPtr) dsp->data.ptrvalue);
      } else if (dsp->choice == 2) {
        litp = (SeqLitPtr) dsp->data.ptrvalue;
        if (litp != NULL) {
          len += litp->length;
        }
      }
    }
  }
  slp = sgp->loc;
  if (slp != NULL && slp->choice == SEQLOC_INT) {
    sintp = (SeqIntPtr) slp->data.ptrvalue;
    if (sintp != NULL) {
      sintp->from += len;
      sintp->to += len;
      sintp->id = SeqIdFree (sintp->id);
      sintp->id = SeqIdDup (bsp->id);
    }
  }
  for (sap = bsp->annot; sap != NULL; sap = sap->next) {
    if (sap->type == 3) {
      for (lastsgp = sap->data; lastsgp->next != NULL; lastsgp = lastsgp->next) {
        continue;
      }
      lastsgp->next = sgp;
      break;
    }
  }
  if (sap == NULL) {
    if (bsp->annot != NULL) {
      for (sap = bsp->annot; sap->next != NULL; sap = sap->next) {
        continue;
      }
      sap->next = NewSeqAnnotType3 ("Graphs", sgp);
    } else {
      bsp->annot = NewSeqAnnotType3 ("Graphs", sgp);
    }
  }
}

static CharPtr LIBCALL TrimWhitespaceAroundString (CharPtr str)

{
  Uchar    ch;	/* to use 8bit characters in multibyte languages */
  CharPtr  dst;
  CharPtr  ptr;

  if (str != NULL && str [0] != '\0') {
    dst = str;
    ptr = str;
    ch = *ptr;
    while (ch != '\0' && ch <= ' ') {
      ptr++;
      ch = *ptr;
    }
    while (ch != '\0') {
      *dst = ch;
      dst++;
      ptr++;
      ch = *ptr;
    }
    *dst = '\0';
    dst = NULL;
    ptr = str;
    ch = *ptr;
    while (ch != '\0') {
      if (ch > ' ') {
        dst = NULL;
      } else if (dst == NULL) {
        dst = ptr;
      }
      ptr++;
      ch = *ptr;
    }
    if (dst != NULL) {
      *dst = '\0';
    }
  }
  return str;
}

static CharPtr ReadSingleStringFromFile (CharPtr filename)

{
  Char     ch;
  FILE     *fp;
  Int8     len;
  CharPtr  ptr;
  CharPtr  str = NULL;

  if (StringHasNoText (filename)) return NULL;
  len = FileLength (filename);
  if (len < 1) return NULL;
  fp = FileOpen (filename, "r");
  if (fp == NULL) return NULL;
  str = MemNew ((size_t) (len + 100));
  if (str == NULL) return NULL;
  if (fgets (str, (size_t) (len + 50), fp) == NULL) {
    FileClose (fp);
    return NULL;
  }
  FileClose (fp);
  TrimWhitespaceAroundString (str);
  ptr = str;
  ch = *ptr;
  while (ch != '\0') {
    if (ch == '\r' || ch == '\n') {
      str = MemFree (str);
      return NULL;
    }
    ptr++;
    ch = *ptr;
  }
  return str;
}

static CharPtr BioseqGetLocalIdStr (BioseqPtr bsp)

{
  ObjectIdPtr  oip;
  SeqIdPtr     sip;

  if (bsp == NULL) return NULL;
  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_LOCAL) {
      oip = (ObjectIdPtr) sip->data.ptrvalue;
      if (oip != NULL && oip->str != NULL) {
        return oip->str;
      }
    }
  }
  return NULL;
}

static void MakeAssemblyFragments (BioseqPtr bsp, CharPtr name, Int2 index,
                                   CharPtr sp6_clone, CharPtr sp6_end,
                                   CharPtr t7_clone, CharPtr t7_end,
                                   Uint1 frag)

{
  DeltaSeqPtr  dsp;
  Int4         from, to;
  ImpFeatPtr   ifp;
  SeqLitPtr    litp;
  SeqFeatPtr   sfp;
  SeqInt       sint;
  Char         str [128];
  Char         tmp [32];
  ValNode      vn;

  if (bsp == NULL || name == NULL || index < 1) return;
  from = 0;
  to = 0;
  if (bsp->repr == Seq_repr_delta && bsp->seq_ext_type == 4) {
    for (dsp = (DeltaSeqPtr) (bsp->seq_ext);
         dsp != NULL && index > 1; dsp = dsp->next, index--) {
      if (dsp->choice == 1) {
        from += SeqLocLen ((SeqLocPtr) dsp->data.ptrvalue);
      } else if (dsp->choice == 2) {
        litp = (SeqLitPtr) dsp->data.ptrvalue;
        if (litp != NULL) {
          from += litp->length;
        }
      }
    }
  }
  if (dsp != NULL && dsp->choice == 2) {
    litp = (SeqLitPtr) dsp->data.ptrvalue;
    if (litp != NULL) {
      to = litp->length + from - 1;
    }
  }
  MemSet ((Pointer) &vn, 0, sizeof (ValNode));
  vn.choice = SEQLOC_INT;
  vn.data.ptrvalue = &sint;

  MemSet ((Pointer) &sint, 0, sizeof (SeqInt));
  sint.id = SeqIdDup (SeqIdFindBest (bsp->id, 0));

  sint.from = from;
  sint.to = to;
  sint.strand = Seq_strand_plus;

  sfp = CreateNewFeatureOnBioseq (bsp, SEQFEAT_IMP, &vn);
  ifp = ImpFeatNew ();
  sfp->data.value.ptrvalue = (Pointer) ifp;
  ifp->key = StringSave ("misc_feature");

  sprintf (str, "assembly_name:%s", name);
  if (frag > 0) {
    sprintf (tmp, "~fragment_group:%d", (int) frag);
    StringCat (str, tmp);
  }
  if (StringICmp (name, sp6_clone) == 0) {
    StringCat (str, "~clone_end:SP6");
    if (sp6_end != NULL) {
      StringCat (str, "~vector_side:");
      StringCat (str, sp6_end);
    }
  } else if (StringICmp (name, t7_clone) == 0) {
    StringCat (str, "~clone_end:T7");
    if (t7_end != NULL) {
      StringCat (str, "~vector_side:");
      StringCat (str, t7_end);
    }
  }
  sfp->comment = StringSaveNoNull (str);
}

/*****************************************************************************
*
*   Main program loop to read, process, write SeqEntrys
*
*****************************************************************************/
typedef struct reqcontig {
  Int2  index;
  Char  str [41];
} ResqContig, PNTR ResqContigPtr;

#define MAX_FIELDS  8

Int2 Main(void)
{
   AsnIoPtr aip;
   FILE *fp, *cfp;
   SeqSubmitPtr ssp;
   NCBISubPtr nsp;
   SeqEntryPtr sep, oldsep, the_entry, sep_list, nextsep;
   BioseqPtr bsp, the_bsp;
   Uint1 htgs_phase; /* a value from 0-3 */
   Uint1 MI_htgs_phase;  /* mapping of htgs_phase to MI_TECH_htgs_? */
   Uint2 frag;
   CharPtr  newstr, accession, remark, center, organism, clone, seqbuf,
      seqname, strain, chromosome, title, extra_ac, clone_lib, map,
      comment_fname, comment_fstr, phrap_fname, fasta_fname, contigs, accn_fname,
      contig_table, sp6_clone, t7_clone, sp6_end = NULL, t7_end = NULL,
      pstring = NULL;
   Char  instr[120], buf [256], dumsp6 [64], dumt7 [64];
   CharPtr field [MAX_FIELDS], ptr;
   Int4   totalen, filelen, len, length = 0, cumlength = 0, gaplen;
   SeqLitPtr slp;
   Int2 errs, numFields;
   BioseqSetPtr bssp;
   ValNodePtr vnp, vnp2, PNTR prevpnt, next;
   Boolean   temp_org, temp_comment, lastwasraw, coordsOnMaster,
      htgsDraft, usedelta = FALSE, do_contig, left_end, right_end,
      htgsFulltop, htgsActivefin, htgsCancelled;
   Int2 index = 0;
   ValNodePtr rescuedsgps = NULL, rescuedcontigs = NULL, fragmentgroups = NULL;
   ValNodePtr seqlitlist = NULL;
   IntFuzzPtr ifp;
   int frg;
   ResqSeqgphPtr rsp;
   ResqContigPtr rcp;
   CharPtr tool_ver = "fa2htgs 2.1";
   CharPtr keywords [3];
   Int2 currkeyword = 0;

               /* check command line arguments */

   if ( ! GetArgs(tool_ver, NUMARG, myargs))
      return 1;

   fasta_fname = myargs[0].strvalue;
   organism = myargs[4].strvalue;
   seqname = myargs[5].strvalue;
   length = myargs[6].intvalue;
   center = myargs[7].strvalue;
   htgs_phase = (Uint1)(myargs[8].intvalue);
   if (htgs_phase == 0)
     MI_htgs_phase = (Uint1)MI_TECH_htgs_0;
   else
     MI_htgs_phase = (Uint1)(MI_TECH_htgs_1 + htgs_phase - 1);

   accession = myargs[9].strvalue;
   remark = myargs[10].strvalue;
   clone = myargs[11].strvalue;
   chromosome = myargs[12].strvalue;
   title = myargs[13].strvalue;
   temp_comment = (Boolean) myargs[14].intvalue;
   temp_org = (Boolean) myargs[15].intvalue;
   extra_ac = myargs[16].strvalue;
   clone_lib = myargs[17].strvalue;
   map = myargs[18].strvalue;
   comment_fname = myargs[19].strvalue;
   comment_fstr = NULL;
   phrap_fname = myargs[20].strvalue;
   contigs = myargs[21].strvalue;
   accn_fname = myargs[22].strvalue;
   coordsOnMaster = (Boolean) myargs[23].intvalue;
   htgsDraft = (Boolean) myargs[24].intvalue;
   strain = myargs[25].strvalue;
   gaplen = myargs[26].intvalue;
   do_contig = (Boolean) myargs [27].intvalue;
   sp6_clone = myargs [28].strvalue;
   t7_clone = myargs [29].strvalue;
   contig_table = myargs [30].strvalue;
   htgsFulltop = (Boolean) myargs[31].intvalue;
   htgsActivefin = (Boolean) myargs[32].intvalue;
   htgsCancelled = (Boolean) myargs[33].intvalue;

   dumsp6 [0] = '\0';
   dumt7 [0] = '\0';
   if (StringHasNoText (sp6_clone)) {
     sp6_clone = NULL;
   }
   if (StringHasNoText (t7_clone)) {
     t7_clone = NULL;
   }

   if (StringHasNoText (contigs)) {
     if (! StringHasNoText (contig_table)) {
       if ((fp = FileOpen (contig_table, "r")) == NULL) {
         ErrPostEx(SEV_ERROR,0,0, "Can't open %s", contig_table);
         ErrShow();
         return 1;
       }
       while (fgets (buf, sizeof (buf), fp) != NULL) {
         MemSet ((Pointer) field, 0, sizeof (field));

/*
*  parse tab-delimited output line into array of fields, avoiding use of
*  strtok so that empty columns (adjacent tabs) are properly assigned to
*  field array
*/

         ptr = buf;
         for (numFields = 0; numFields < MAX_FIELDS && ptr != NULL; numFields++) {
           field [numFields] = ptr;
           ptr = StringChr (ptr, '\t');
           if (ptr == NULL) {
             ptr = StringChr (ptr, '\n');
           }
           if (ptr == NULL) {
             ptr = StringChr (ptr, '\r');
           }
           if (ptr != NULL) {
             *ptr = '\0';
             ptr++;
           }
         }

         if (! StringHasNoText (field [0])) {
           StringNCpy_0 (instr, field [0], sizeof (instr) - 2);
           if (! StringHasNoText (field [1])) {
             if (StringNICmp (field [1], "-", 1) == 0) {
               StringCat (instr, "-");
             }
           }
           ValNodeCopyStr (&rescuedcontigs, 0, instr);
           if (! StringHasNoText (field [2])) {
             if (sscanf (field [2], "%d", &frg) == 1) {
               ValNodeCopyStr (&fragmentgroups, (Uint1) frg, field [0]);
             }
           }
           left_end = FALSE;
           right_end = FALSE;
           if (! StringHasNoText (field [3])) {
              if (! StringHasNoText (field [4])) {
                if (StringNICmp (field [4], "l", 1) == 0) {
                 left_end = TRUE;
                } else if (StringNICmp (field [4], "r", 1) == 0) {
                  right_end = TRUE;
                }
              }
              if (StringICmp (field [3], "sp6") == 0) {
                StringCpy (dumsp6, field [0]);
                if (left_end) {
                  StringCat (dumsp6, ",left");
                } else if (right_end) {
                  StringCat (dumsp6, ",right");
                }
                if (sp6_clone == NULL) {
                  sp6_clone = dumsp6;
                }
              } else if (StringICmp (field [3], "t7") == 0) {
                StringCpy (dumt7, field [0]);
                if (left_end) {
                  StringCat (dumt7, ",left");
                } else if (right_end) {
                  StringCat (dumt7, ",right");
                }
                if (t7_clone == NULL) {
                  t7_clone = dumt7;
                }
              }
           }
         }
       }
       FileClose (fp);

       len = 0;
       for (vnp = rescuedcontigs; vnp != NULL; vnp = vnp->next) {
         len += StringLen ((CharPtr) vnp->data.ptrvalue) + 1;
       }
       if (len > 1) {
         pstring = MemNew ((size_t) (len + 2));
         contigs = pstring;
         for (vnp = rescuedcontigs; vnp != NULL; vnp = vnp->next) {
           if (vnp != rescuedcontigs) {
             StringCat (pstring, ",");
           }
           StringCat (pstring, (CharPtr) vnp->data.ptrvalue);
         }
       }
 
       rescuedcontigs = ValNodeFreeData (rescuedcontigs);
     }
   }

   if (sp6_clone != NULL) {
     sp6_end = StringChr (sp6_clone, ',');
     if (sp6_end != NULL) {
       *sp6_end = '\0';
       sp6_end++;
       if (StringICmp (sp6_end, "left") == 0) {
         sp6_end = "left";
       } else if (StringICmp (sp6_end, "right") == 0) {
         sp6_end = "right";
       } else {
         sp6_end = NULL;
       }
     }
   }
   if (t7_clone != NULL) {
     t7_end = StringChr (t7_clone, ',');
     if (t7_end != NULL) {
       *t7_end = '\0';
       t7_end++;
       if (StringICmp (t7_end, "left") == 0) {
         t7_end = "left";
       } else if (StringICmp (t7_end, "right") == 0) {
         t7_end = "right";
       } else {
         t7_end = NULL;
       }
     }
   }

   UseLocalAsnloadDataAndErrMsg (); /* finds data directory without a .ncbirc file */

               /* load the sequence alphabets  */
               /* (and sequence parse trees)   */
   if (! SeqEntryLoad())
   {
      ErrShow();
      return 1;
   }
   if (! SubmitAsnLoad())
   {
      ErrShow();
      return 1;
   }
                                /* log errors instead of die */
   if (myargs[3].strvalue != NULL)
   {
      if (! ErrSetLog (myargs[3].strvalue))
      {
         ErrShow();
         return 1;
      }
      else
         ErrSetOpts (ERR_CONTINUE, ERR_LOG_ON);
   }
                         /* open phrap contig file */
   if (phrap_fname != NULL)
   {
      if ((fp = FileOpen (phrap_fname, "r")) == NULL)
      {
         ErrPostEx(SEV_ERROR,0,0, "Can't open %s", phrap_fname);
         ErrShow();
         return 1;
      }
   }
 
   else if (accn_fname != NULL) {
      if ((fp = FileOpen (accn_fname, "r")) == NULL)
      {
         ErrPostEx(SEV_ERROR,0,0, "Can't open %s", accn_fname);
         ErrShow();
         return 1;
      }
   }
                         /* open input fasta file */
   else if ((fp = FileOpen (fasta_fname, "r")) == NULL)
   {
      ErrPostEx(SEV_ERROR,0,0, "Can't open %s", fasta_fname);
      ErrShow();
      return 1;
   }
                        /* open comment file */
   if (comment_fname != NULL) {

      comment_fstr = ReadSingleStringFromFile (comment_fname);
      if (comment_fstr == NULL) {

         if ((cfp = FileOpen (comment_fname, "r")) == NULL)
         {
            ErrPostEx(SEV_ERROR,0,0, "Can't open %s", comment_fname);
            ErrShow();
            return 1;
         }
         /* rules for building the comment string a file:
            -- maximum 100 characters per line
            -- insert a " " to concatnate lines
            -- insert a "~" to concatnate lines if
               it is a blank line or there are leading
               spaces in the beginning of the line
                                    Hsiu-Chuan Chen 1-30-98
         */ 

         filelen = FileLength (comment_fname);
         filelen = filelen + 1000;
         comment_fstr = MemNew (filelen);
         totalen = 0;
         while (fgets (instr, 110, cfp) != NULL)
         {
             len = StringLen (instr);
             while (len > 0 && instr[len-1] == '\n')
             {
                instr[len-1] = '\0';
                len = StringLen (instr);
             }

             totalen = totalen + len + 2;
             if (totalen > filelen)
             {
                filelen = filelen + 1000;
                newstr = MemNew (filelen);
                StringCpy (newstr, comment_fstr);
                MemFree (comment_fstr);

                comment_fstr = newstr;
             }

             if (comment_fstr != NULL)
             {
                if (instr[0] == '\0' || instr[0] == ' ')
                   StringCat (comment_fstr, "~");
                else
                   StringCat (comment_fstr, " ");

                StringCat (comment_fstr, instr);
             }
             else
                StringCpy (comment_fstr, instr);

         } /* while */
      }
   }
                        /* open template file */
   if ((aip = AsnIoOpen (myargs[1].strvalue, "r")) == NULL)
   {
      ErrPostEx(SEV_ERROR,0,0, "Can't open %s", myargs[1].strvalue);
      ErrShow();
      FileClose(fp);
      return 1;
   }
   ssp = SeqSubmitAsnRead(aip, NULL);
   AsnIoClose(aip);
   if (ssp == NULL)
   {
      ErrPostEx(SEV_ERROR,0,0, "Can't read %s", myargs[1].strvalue);
      ErrShow();
      FileClose(fp);
      return 1;
   }

   oldsep = (SeqEntryPtr)(ssp->data);  /* clear out template */
   ssp->data = NULL;
   MemFree(ssp->sub->tool);
   ssp->sub->tool = StringSave(tool_ver);
   nsp = MemNew(sizeof(NCBISub));
   nsp->ssp = ssp;
   nsp->submittor_key = StringSave(center);
   MemFree(ssp->sub->cit->descr);
   ssp->sub->cit->descr = StringSave(remark);

   sep_list = NULL;
   if (phrap_fname != NULL) {
      sep_list = ReadPhrapFile (fp);
      sep_list = SetPhrapContigOrder (sep_list, contigs);
   } else if (accn_fname != NULL) {
      sep_list = ReadContigList (fp, coordsOnMaster);
   } else {
      while ((sep = FastaToSeqEntry (fp, TRUE)) != NULL) {
         ValNodeLink (&sep_list, sep);
      }
   }

   sep = sep_list;
   if (sep != NULL && sep->next != NULL) {
     usedelta = TRUE;
   }

   cumlength = 0;
   index = 0;
   if (accn_fname != NULL) {
    the_entry = AddSeqOnlyToSubmission (
                        nsp,
                        seqname,
                        NULL,
                        accession,
                        0,
                        MOLECULE_CLASS_DNA,
                        MOLECULE_TYPE_GENOMIC,
                        length,
                        TOPOLOGY_LINEAR,
                        STRANDEDNESS_DOUBLE);

      sep = sep_list;
      nextsep = sep->next;
      sep->next = NULL;
      if (the_entry != NULL && the_entry->choice == 1 && sep != NULL && sep->choice == 1) {
         the_bsp = (BioseqPtr)(the_entry->data.ptrvalue);
         bsp = (BioseqPtr)(sep->data.ptrvalue);
         if (the_bsp->repr == Seq_repr_raw) {
            the_bsp->seq_data = BSFree (the_bsp->seq_data);
            the_bsp->repr = bsp->repr;
            the_bsp->seq_data = bsp->seq_data;
            bsp->seq_data = NULL;
            the_bsp->seq_data_type = bsp->seq_data_type;
            the_bsp->seq_ext_type = bsp->seq_ext_type;
            the_bsp->seq_ext = bsp->seq_ext;
            bsp->seq_ext = NULL;
         }
         index++;
      cumlength += bsp->length;
      SeqEntryFree(sep);
      }
   } else if (htgs_phase < 3 || usedelta)
   {
      the_entry = AddDeltaSeqOnlyToSubmission (
                        nsp,
                        seqname,
                        NULL,
                        accession,
                        0,
                        MOLECULE_CLASS_DNA,
                        MOLECULE_TYPE_GENOMIC,
                        length,
                        TOPOLOGY_LINEAR,
                        STRANDEDNESS_DOUBLE);

      sep = sep_list;
      lastwasraw = FALSE;
      while (sep != NULL)
      {
         nextsep = sep->next;
         sep->next = NULL;
         bsp = (BioseqPtr)(sep->data.ptrvalue);
         if (bsp->repr == Seq_repr_raw)
         {
            if (lastwasraw) {
               slp = AddFakeGapToDeltaSeq(nsp, the_entry, gaplen);
               ValNodeAddPointer (&seqlitlist, 0, (Pointer) slp);
               index++;
               cumlength += gaplen;
            }
            BioseqRawConvert(bsp, Seq_code_iupacna);
            seqbuf = BSMerge((ByteStorePtr)(bsp->seq_data), NULL);
            slp = AddLiteralToDeltaSeq(nsp, the_entry,
               bsp->length);
            AddBasesToLiteral(nsp, slp, seqbuf);
            MemFree(seqbuf);
            lastwasraw = TRUE;
            index++;
         }
         else
         {
            if (bsp->length < 0)
               bsp->length = 0;  /* -1 may be set */
            AddGapToDeltaSeq(nsp, the_entry,
               bsp->length);
            lastwasraw = FALSE;
            index++;
         }
         cumlength += bsp->length;
         RescueSeqGraphs (bsp, index, &rescuedsgps);
         if (do_contig) {
            rcp = (ResqContigPtr) MemNew (sizeof (ResqContig));
            if (rcp != NULL) {
              rcp->index = index;
              StringNCpy_0 (rcp->str, BioseqGetLocalIdStr (bsp), sizeof (rcp->str));
              ValNodeAddPointer (&rescuedcontigs, 0, (Pointer) rcp);
            }
         }
         SeqEntryFree(sep);
         sep = nextsep;
      }
   }
   else
   {
    the_entry = AddSeqOnlyToSubmission (
                        nsp,
                        seqname,
                        NULL,
                        accession,
                        0,
                        MOLECULE_CLASS_DNA,
                        MOLECULE_TYPE_GENOMIC,
                        length,
                        TOPOLOGY_LINEAR,
                        STRANDEDNESS_DOUBLE);

      sep = sep_list;
      nextsep = sep->next;
      sep->next = NULL;
      bsp = (BioseqPtr)(sep->data.ptrvalue);
      if (bsp->repr == Seq_repr_raw)
      {
         BioseqRawConvert(bsp, Seq_code_iupacna);
         seqbuf = BSMerge((ByteStorePtr)(bsp->seq_data), NULL);
         AddBasesToBioseq(nsp, the_entry, seqbuf);
         MemFree(seqbuf);
         index++;
      }
      cumlength += bsp->length;
      RescueSeqGraphs (bsp, index, &rescuedsgps);
      SeqEntryFree(sep);
   }

   FileClose(fp);
                             
    /* get data from template: pub, organism, and comment */
   if (IS_Bioseq(oldsep))
   {
      bsp = (BioseqPtr)(oldsep->data.ptrvalue);
      prevpnt = &(bsp->descr);
   }
   else
   {
      bssp = (BioseqSetPtr)(oldsep->data.ptrvalue);
      prevpnt = &(bssp->descr);
   }

   bsp = (BioseqPtr)(the_entry->data.ptrvalue);
   if (bsp != NULL) {
     bsp->length = MAX (cumlength, length);
   }

   for (vnp = *prevpnt; vnp != NULL; vnp = next)
   {
      next = vnp->next;
      if (vnp->choice == Seq_descr_pub
               || (vnp->choice == Seq_descr_comment && temp_comment)
       || ((vnp->choice == Seq_descr_org || vnp->choice == Seq_descr_source)
                && temp_org))
      {
         *prevpnt = next;
         vnp->next = NULL;
         ValNodeLink(&(bsp->descr), vnp);
      }
      else
         prevpnt = &(vnp->next);
   }

   if (comment_fstr != NULL)
   {
      SeqDescrAddPointer (&(bsp->descr), Seq_descr_comment, comment_fstr);
      comment_fstr = NULL;
   } 
   
   SeqEntryFree(oldsep);

   if (organism != NULL)
      AddOrganismToEntryNew(nsp, the_entry, organism, NULL, NULL, NULL,
               NULL, NULL, NULL, NULL);

   AddGenomeToEntry(nsp, the_entry, 1);
   if (clone != NULL)
      AddSubSourceToEntry(nsp, the_entry, 3, clone);
   if (chromosome != NULL)
       AddSubSourceToEntry(nsp, the_entry, 1, chromosome);
   if (clone_lib != NULL)
       AddSubSourceToEntry(nsp, the_entry, 11, clone_lib);
   if (map != NULL)
       AddSubSourceToEntry(nsp, the_entry, 2, map);
   if (strain != NULL)
       AddOrgModToEntry(nsp, the_entry, 2, strain);

   if (title != NULL)
      AddTitleToEntry(nsp, the_entry, title);

   keywords [0] = NULL;
   keywords [1] = NULL;
   keywords [2] = NULL;
   if (htgsDraft) {
      keywords [currkeyword] = "HTGS_DRAFT";
      currkeyword++;
   }
   if (htgsFulltop) {
      keywords [currkeyword] = "HTGS_FULLTOP";
      currkeyword++;
   }
   if (htgsActivefin) {
      keywords [currkeyword] = "HTGS_ACTIVEFIN";
      currkeyword++;
   }
   if (htgsCancelled) {
      keywords [currkeyword] = "HTGS_CANCELLED";
      currkeyword++;
   }
   if (currkeyword > 0) {
      AddGenBankBlockToEntry (nsp, the_entry, NULL, NULL, keywords [0], keywords [1], keywords [2]);
   }

   if (extra_ac != NULL)
      AddExtraAc2Entry(the_entry, extra_ac);

   AddBiomolToEntry(nsp, the_entry, 1);
   AddTechToEntry(nsp, the_entry, MI_htgs_phase);

   if (bsp != NULL) {
     for (vnp = rescuedsgps; vnp != NULL; vnp = vnp->next) {
       rsp = (ResqSeqgphPtr) vnp->data.ptrvalue;
       if (rsp != NULL) {
         OffsetAndLinkSeqGraph (bsp, rsp->sgp, (Int2) rsp->index);
       }
     }
     for (vnp = rescuedcontigs; vnp != NULL; vnp = vnp->next) {
       rcp = (ResqContigPtr) vnp->data.ptrvalue;
       if (rcp != NULL) {
         frag = 0;
         for (vnp2 = fragmentgroups; vnp2 != NULL; vnp2 = vnp2->next) {
           if (StringICmp ((CharPtr) vnp2->data.ptrvalue, rcp->str) == 0) {
             frag = (Uint1) vnp2->choice;
           }
         }
         MakeAssemblyFragments (bsp, rcp->str, (Int2) rcp->index,
                                sp6_clone, sp6_end, t7_clone, t7_end, frag);
       }
     }
   }
   rescuedsgps = ValNodeFreeData (rescuedsgps);
   rescuedcontigs = ValNodeFreeData (rescuedcontigs);
   fragmentgroups = ValNodeFreeData (fragmentgroups);
   MemFree (pstring);

   for (vnp = seqlitlist; vnp != NULL; vnp = vnp->next) {
     slp = (SeqLitPtr) vnp->data.ptrvalue;
     if (slp != NULL) {
       ifp = IntFuzzNew();
       ifp->choice = 4;    /* lim - unk*/
       slp->fuzz = ifp;
     }
   }
   seqlitlist = ValNodeFree (seqlitlist);

   errs = NCBISubValidate(nsp, NULL);

   NCBISubWrite(nsp, myargs[2].strvalue);

   NCBISubFree(nsp);

   return(errs);
}

