/*   salfiles.c
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
* File Name:  salfiles.c
*
* Author:  Colombe Chappey
*
* Version Creation Date:   1/27/96
*
* $Revision: 6.96 $
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
#include <salfiles.h>
#include <salstruc.h>
#include <salutil.h>
#include <salsap.h>
#include <salpanel.h>
#include <salparam.h>
#include <salign.h>
#include <biosrc.h>
#include <cdrgn.h>
#include <fstyle.h>
#include <subutil.h>
#include <satutil.h>
#include <tofasta.h>
#include <objacces.h>
#include <seqmgr.h>

#define MAXSTR          512
#define OBJ_VIRT        254
#define SALSA_PHYLIP_MARGIN 11
#define SALSA_CLUSTALV_MARGIN 17

extern SeqEntryPtr ReadLocalAlignment (Uint1 format, CharPtr path);

/**********************************************************/
static CharPtr FGetLine (FILE *fp)
{
  ValNodePtr charp=NULL, 
             vnp; 
  CharPtr    buffer = NULL;
  Char       c;
  Int4       j, len=0;

  c = (Char)fgetc(fp);
  while (c!=255 && c!=NULLB && c!=EOF && c!='\0' && c!='\n') 
  {
     if (c!='\t' && c!='\015') {
        ValNodeAddInt (&charp, 1, (Int4)c);
        len++;
     }
     c = (Char)fgetc(fp);
  }
  if (charp) {
     buffer = (CharPtr)MemNew((size_t)((len+5)*sizeof(Char))); 
     for (j=0, vnp=charp; vnp!=NULL; vnp=vnp->next, j++)
        buffer[j] = (Char) vnp->data.intvalue;
     buffer[j]='\0';
     ValNodeFree (charp);
  }
  else if (c=='\0' || c=='\n') {
     buffer = (CharPtr)MemNew((size_t)(sizeof(Char)));
     buffer[0]='\0';
  }
  return buffer;
}

/**********************************************************/
static ValNodePtr new_sequence_vectors (Int2 n_seq, Int4 lens)
{
  ValNodePtr seqvnp = NULL;
  CharPtr    tmp;
  Int4       strlens;
  Int2       j;

  for (j = 0; j < n_seq; j++) {
     tmp = (CharPtr) MemNew((size_t) ((lens + 1) * sizeof(Char)));
     for (strlens = 0; strlens < lens; strlens++) 
         tmp [strlens] = ' ';
     tmp [lens] = '\0';
     ValNodeAddPointer (&seqvnp, 0, (Pointer)tmp);
  }
  return seqvnp;
}

/**********************************************************/
static Boolean stringhasnotext (CharPtr str)

{
  Char  ch;

  if (str != NULL) {
    ch = *str;
    while (ch != '\0') {
      if (ch > ' ' && ch <= '~') {
        return FALSE;
      }
      str++;
      ch = *str;
    }
  }
  return TRUE;
}

/**********************************************************/
static CharPtr get_first_notemptyline (FILE *fp)
{
  CharPtr str=NULL;

  str = FGetLine (fp);
  while (str) {
     if (! stringhasnotext (str)) {
        if (StringLen (str) > 0)
           break;
     }
     MemFree (str);
     str = FGetLine (fp);
  }
  return str;
}

/**********************************************************/
static SeqEntryPtr make_seqentry_for_seqentry (SeqEntryPtr sep)
{
  SeqEntryPtr  sep1 = NULL,
               tmp;
  BioseqPtr    bsp;
  BioseqSetPtr bssp;
  
  if (sep != NULL) {
     if (IS_Bioseq(sep) || IS_Bioseq_set(sep))
     {
        if (sep->next)
        {
           bssp = BioseqSetNew ();
           if (bssp)
           {
              bssp->_class = 14;
              bssp->seq_set = sep;
              sep1 = SeqEntryNew ();
              sep1->choice = 2;
              sep1->data.ptrvalue = bssp;
              SeqMgrLinkSeqEntry (sep1, 0, NULL);
           
              for (tmp = bssp->seq_set; tmp!=NULL; tmp=tmp->next) {
                 if (IS_Bioseq(tmp))
                 {
                    bsp = (BioseqPtr) tmp->data.ptrvalue;
                    ObjMgrConnect (OBJ_BIOSEQ, (Pointer) bsp, OBJ_BIOSEQSET, (Pointer) bssp);
                 }
              }
           }
           else sep1=sep;
        }
        else sep1=sep;
     }
     else 
        sep1=sep;
  }
  return sep1;
}


/**********************************************************/
static SeqEntryPtr strings_to_seqentry (ValNodePtr seqvnp, Uint1 mol_type, SeqIdPtr seqsip, SeqAnnotPtr sap)
{
  SeqEntryPtr sep,
              pre_sep = NULL,
              sep_list = NULL;
  ValNodePtr  vnp;
  SeqIdPtr    sip;
  CharPtr     str;
  Int4        lens;

  for (vnp=seqvnp, sip=seqsip; vnp!=NULL && sip!=NULL; vnp=vnp->next, sip=sip->next) 
  {
     str = (CharPtr) vnp->data.ptrvalue;
     if (str)
     {
        lens = (Int4) StringLen (str);
        sep = StringToSeqEntry (str, sip, lens, mol_type);
        if (sep != NULL) {
           if (sep_list == NULL) 
              sep_list = sep;
           else 
              pre_sep->next = sep;
           pre_sep = sep;
        }
     }
  }
  sep_list = make_seqentry_for_seqentry (sep_list);
  SeqAlignAddInSeqEntry (sep_list, sap);
  return sep_list;
}

/*******************************************************************
***    
***   FastaRead
***	returns a SeqEntryPtr given a path name, and the mol_type 
***   NewFastaRead
***	calls FastaToSeqEntryInternal
***   FastaReadAdvanced
***	calls NewFastaRead
***	makes a SeqEntryPtr-BioseqSet if 2 sequences or more
***
********************************************************************/
extern SeqEntryPtr FastaRead (CharPtr path, Uint2 mol_type)
{
  Char         name[PATH_MAX];
  SeqEntryPtr  sep_list = NULL, sep = NULL, pre_sep = NULL;
  FILE         *fpin;

  if (path == NULL) {
     if (! GetInputFileName (name, PATH_MAX,"","TEXT"))  {
        return NULL;
     }
     path = name;
  }
  if ( (fpin = FileOpen (path, "r")) != NULL)  {
     while ((sep = FastaToSeqEntry (fpin, (Boolean)ISA_na (mol_type) ) ) != NULL)
     {
        if (sep_list==NULL) 
           sep_list = sep;
        else  
           pre_sep->next = sep;
        pre_sep = sep;
     }
     FileClose(fpin);
     sep_list = make_seqentry_for_seqentry (sep_list);
  }
  return sep_list;
}

NLM_EXTERN SeqEntryPtr FastaToSeqEntryInternal
(
 VoidPtr input,          /* input pointer (file or memory) */
 Int4 type,              /* type of inquiry FASTA_MEM_IO or FASTA_FILE_IO */
 CharPtr PNTR last_char, /* returned pointer to next FASTA sequence */
 Boolean is_na,          /* type of sequence */
 CharPtr PNTR errormsg,  /* error messge for debugging */
 Boolean parseSeqId,     /* Parse SeqID from def line */
 CharPtr special_symbol     /* Returns special symbol if no SeqEntry */
 );

static SeqIdPtr new_local_sip (Int2 count, Boolean is_na)
{
  SeqIdPtr sip = NULL;
  ObjectIdPtr   oid = NULL;
  Char          str [32];
  
  oid = ObjectIdNew ();
  if (oid != NULL) {
     if (is_na) {
                    sprintf (str, "nuc %ld", (long) count);
                  } else {
                    sprintf (str, "prot %ld", (long) count);
                  }
                  oid->str = StringSave (str);
                  sip = ValNodeNew (NULL);
     if (sip != NULL) {
                    sip->choice = SEQID_LOCAL;
                    sip->data.ptrvalue = (Pointer) oid;
     }
  } else {
     ObjectIdFree (oid);
  }
  return sip;
}

static SeqEntryPtr NewFastaRead (FILE *fp, Boolean is_na, Boolean parseSeqId, Int2 *seqnumber, Int2 *segnumber, SeqIdPtr PNTR siplst, Int4 *lengthmax)
{
  BioseqPtr     bsp;
  SeqEntryPtr   sep = NULL;
  SeqEntryPtr   lastsep = NULL;
  SeqEntryPtr   nextsep = NULL;
  SeqEntryPtr   last = NULL;
  CharPtr       errormsg = NULL;
  ValNodePtr    head = NULL;
  Char          lastchar;
  ObjectIdPtr   oid = NULL;
  SeqIdPtr      sip;
  SeqIdPtr      siphead = NULL, 
                siptmp = NULL;
  SeqIdPtr      segsip=NULL, lastsegsip=NULL;
  ValNodePtr    vnp;
  Int4          count;
  Int4          lensmax = 0,
                lens;
  Int2          nseq = 0;
  Int2          segcount = 0, 
                segtotal = 0,
                j;
  Boolean       insegset;
  Boolean       isLocalUnknownID;
  
  SeqEntryPtr sepnuc;
  BioseqPtr   segbsp;

  count = 0;
  last = sep;
  lastsep = NULL;
  insegset = FALSE;
  nextsep = FastaToSeqEntryInternal ((void *)fp, 2, NULL, is_na, &errormsg, parseSeqId, &lastchar);
  while (nextsep != NULL || (lastchar != EOF && lastchar != NULLB && lastchar != 255)) {
          if (nextsep != NULL) {
            count++;
            if (IS_Bioseq (nextsep) && nextsep->data.ptrvalue != NULL) {
              bsp = (BioseqPtr) nextsep->data.ptrvalue;
              if (bsp->length > lensmax)
                 lensmax = bsp->length;
              isLocalUnknownID = FALSE;
              sip = bsp->id;
              if (sip != NULL && sip->choice == SEQID_LOCAL) {
                oid = (ObjectIdPtr) sip->data.ptrvalue;
                if (oid != NULL && oid->str != NULL) {
                  isLocalUnknownID = (Boolean) (StringICmp (oid->str, "unknown") == 0);
                }
              }
              if ((! parseSeqId) || isLocalUnknownID) {
                sip = new_local_sip (count, is_na);
                if (sip != NULL) {
                   bsp->id = SeqIdFree (bsp->id);
                   bsp->id = sip;
                   SeqMgrReplaceInBioseqIndex (bsp);
                }
              }
              j=SeqIdOrderInBioseqIdList(sip,siphead);
              if (j > 1) {
                 siptmp = NULL;
                 if (sip != NULL && sip->choice == SEQID_LOCAL) {
                    oid = (ObjectIdPtr) sip->data.ptrvalue;
                    if (oid != NULL && oid->str != NULL) {
                       lens = MIN(StringLen(oid->str), (Int4)7);
                       oid->str[lens] = '\0';
                    }
                 }
                 siptmp = MakeNewProteinSeqId (NULL, sip); 
                 if (siptmp == NULL)
                    siptmp = new_local_sip (count, is_na);
                 if (siptmp!=NULL) {
                    siptmp->next = NULL;
                    bsp->id = SeqIdFree (bsp->id);
                    bsp->id = siptmp;
                    SeqMgrReplaceInBioseqIndex (bsp);
                    sip = bsp->id;
                 }
              }
              siptmp = SeqIdDup (sip);
              siphead = AddSeqId (&siphead, siptmp);
            }
            SeqEntryPack (nextsep);
            if (sep != NULL) {     
              if (insegset) {
                if (lastsep != NULL) {
                  AddSeqEntryToSeqEntry (lastsep, nextsep, TRUE);
                  segcount ++;
                  if (segcount > segtotal)
                     segtotal = segcount;
                  sepnuc = FindNucSeqEntry (lastsep);
                  if (IS_Bioseq(sepnuc)) {
                     segbsp=(BioseqPtr)sepnuc->data.ptrvalue;
                     segsip=segbsp->id;
                     if (segsip != NULL) {
                          lastsegsip = segsip;
                     }
                  }
                } 
                else {
                  lastsep = nextsep;
                  last->next = nextsep;
                  last = nextsep;
                  segcount=1;
                  if (segcount > segtotal)
                    segtotal = segcount;
                  nseq++;
                }
              } 
              else {
                last->next = nextsep;
                last = nextsep;
                segcount=1;
                if (segcount > segtotal)
                  segtotal = segcount;
                nseq++;
                lastsegsip = sip;
                
              }
            } 
            else {
              if (insegset && lastsep == NULL) {
                lastsep = nextsep;
                sep = nextsep;
                last = sep;
                segcount=1;
                if (segcount > segtotal)
                  segtotal = segcount;
                nseq++;

              } 
              else {
                sep = nextsep;
                last = sep;
                segcount=1;
                if (segcount > segtotal)
                  segtotal = segcount;
                nseq++;
                lastsegsip = sip;
                
              }
            }
            vnp = ValNodeNew (head);
            if (head == NULL) {
              head = vnp;
            }
            if (vnp != NULL) {
              vnp->data.ptrvalue = errormsg;
              errormsg = NULL;
            }
          } else if (lastchar == '[') {
            insegset = TRUE;
            lastsep = NULL;
          } else if (lastchar == ']') {
            insegset = FALSE;
          }
          nextsep = FastaToSeqEntryInternal ((void *)fp, 2, NULL, is_na, &errormsg, parseSeqId, &lastchar);
  }
  if (segnumber !=NULL) 
     *segnumber = segtotal; 
  if (sip!=NULL)
     *siplst = siphead;
  else 
     SeqIdFree (siphead);
  if (lengthmax != NULL)
     *lengthmax = lensmax;
  if(seqnumber != NULL)
     *seqnumber = nseq; 
  return sep;
}

static SeqEntryPtr FastaReadAdvanced (CharPtr path, Uint2 mol_type, Int2 *seqnumber, Int2 *segnumber, SeqIdPtr PNTR sip, Int4 *lengthmax)
{
  Char         name[PATH_MAX];
  SeqEntryPtr  sep = NULL;
  FILE         *fpin;
   
  if (path == NULL) {
     if (! GetInputFileName (name, PATH_MAX,"","TEXT"))  {
        return NULL;
     }
     path = name;
  }
  if ( (fpin = FileOpen (path, "r")) != NULL)  {
     if (segnumber != NULL)
        sep = NewFastaRead (fpin, (Boolean)ISA_na (mol_type), TRUE, seqnumber, segnumber, sip, lengthmax);
     else 
        sep = NewFastaRead (fpin, (Boolean)ISA_na (mol_type), TRUE, NULL, NULL, NULL, NULL);
     FileClose (fpin);
     sep = make_seqentry_for_seqentry (sep);
  }
  return sep;
}

/*******************************************************************
***
***   LocalAlignsToSeqAnnotDimn
***   LocalAlign1ToSeqAnnotDimn
***
*************************************************************************/

static ValNodePtr get_lens_fromseqalign (SeqAlignPtr salp)
{
  DenseDiagPtr  ddp;
  DenseSegPtr   dsp;
  ValNodePtr    fromp = NULL;
  Int4Ptr       startp;
  Int4          j,
                val = (Int4)-1;
  Int2          index;
  Uint1         strand;
 
  if (salp == NULL)
     return NULL;
  if (salp->segtype == 1)
  {
     ddp = (DenseDiagPtr) salp->segs;
     if (ddp != NULL) {
      for (index=0; index<ddp->dim; index++) {
        startp = ddp->starts;
        if (index > 0)
           startp += index;
        val = *startp + ddp->len;
        ValNodeAddInt (&fromp, 1, (Int4)(val+1));
      }
     }   
  }  
  else if (salp->segtype == 2)
  {
     dsp = (DenseSegPtr) salp->segs;
     if (dsp!=NULL)
     {   
      for (index=0; index<dsp->dim; index++) 
      {
        if ((Boolean)(dsp->strands != NULL))
           strand = dsp->strands[index];
        else
           strand = Seq_strand_plus;
        startp = dsp->starts + ((dsp->dim * dsp->numseg) - dsp->dim);
        if (index > 0)
           startp += index;
        for (j = dsp->numseg-1; j >= 0; j--, startp-=dsp->dim)
           if (*startp > -1)
              break;
        if (j >= 0) {
           if (strand == Seq_strand_minus)
              val = *startp;
           else
              val = *startp + dsp->lens[j] - 1;
           ValNodeAddInt (&fromp, 1, (Int4)(val+1));
        }
        else 
           ValNodeAddInt (&fromp, 1, (Int4)(-1));
      }
     }
  }     
  return fromp;
}

static SeqAnnotPtr LocalAlign1ToSeqAnnotDimn (ValNodePtr vnpal, SeqIdPtr seqsip, ValNodePtr fromp, Int2 nbseq, Int4 lens, ValNodePtr strands, Boolean trunc_emptyends)
{
  SeqAnnotPtr  sap1=NULL;
  ValNodePtr   tmp;

  if (vnpal!=NULL && vnpal->data.ptrvalue != NULL) {
     tmp = (ValNodePtr) vnpal->data.ptrvalue;
     sap1 = LocalAlignToSeqAnnotDimn (tmp, seqsip, fromp, nbseq, lens, NULL, FALSE);
  }
  return sap1;
}

static SeqAnnotPtr LocalAlignsToSeqAnnotDimn (ValNodePtr vnpal, SeqIdPtr seqsip, ValNodePtr fromp, Int2 nbseq, Int2 nbseg, Int4 lens, ValNodePtr strands, Boolean trunc_emptyends)
{
  SeqAnnotPtr  sap1 = NULL, 
               sap = NULL;
  SeqAlignPtr  salphead = NULL,
               salptmp;
  ValNodePtr   vnp, 
               tmp;
  SeqIdPtr     siplst,
               siptmp,
               siptmp2;
  Int2         k1, k, j=0;

  vnp = vnpal; 
  salphead = NULL;
  while (salphead == NULL && vnp != NULL) 
  {
     siplst=NULL;
     siptmp = seqsip;
     for (k=0; k<j; k++) 
        siptmp=siptmp->next;
     for (k=0; k<nbseq; k++) {
        siptmp2=SeqIdDup(siptmp);
        siplst = AddSeqId (&siplst, siptmp2); 
        for (k1=0; k1<nbseg; k1++)
           siptmp=siptmp->next;
     }
     tmp = (ValNodePtr) vnp->data.ptrvalue;
     sap1 = LocalAlignToSeqAnnotDimn (tmp, siplst, fromp, nbseq, lens, NULL, FALSE);
     if (sap1!=NULL && sap1->data!=NULL)
        salphead = (SeqAlignPtr) sap1->data;
     vnp = vnp->next;
     j++;
  }
  if (fromp!=NULL)
     fromp = ValNodeFree (fromp);
  salptmp = salphead;
  while (vnp!=NULL) 
  {
/*
     fromp = get_lens_fromseqalign (salptmp);
*/
     siplst=NULL;
     siptmp = seqsip;
     for (k=0; k<j; k++) 
        siptmp=siptmp->next;
     for (k=0; k<nbseq; k++) {
        siptmp2=SeqIdDup(siptmp);
        siplst = AddSeqId (&siplst, siptmp2); 
        for (k1=0; k1<nbseg && siptmp!=NULL; k1++)
           siptmp=siptmp->next;
     }
     tmp = (ValNodePtr) vnp->data.ptrvalue;
     sap = LocalAlignToSeqAnnotDimn (tmp, siplst, fromp, nbseq, lens, NULL, FALSE);
     if (sap!=NULL && sap->data!=NULL) {
        salptmp->next = (SeqAlignPtr)sap->data;
        salptmp = salptmp->next;
     }
     vnp = vnp->next;
     if (fromp!=NULL)
        fromp = ValNodeFree (fromp);
     j++;
  }
  return sap1;
}  

/*****************************************************
***   GapFastaRead
***    1) reads the sequences as FASTA: FastaReadAdvanced
***    2) reads the sequence text with the gaps (-): ReadAlignmentToStrings
***       the max length allocated for the char array
***       that is the max length of the sequences plus a 1/2 of gaps.
***
***   ConvertPaupToFastaGap
***	
******************************************************/
static ValNodePtr ReadAlignmentToStrings (CharPtr path, Int4 length, Int2 segnumber)
{
  Char         name[PATH_MAX];
  FILE         *fp;
  ValNodePtr   vnpal, tmp, vnp;
  CharPtr      str = NULL; 
  CharPtr      strp,
               seqstr;
  Int4         strlens, 
               lmax=0,
               lgseq=0;
  Int2         inseg = 0;
  Boolean      insegb = FALSE;
  Boolean      startp;
  
  
  Int2         j = 0;   
  Int2         nseq = 0;
  
  if (path == NULL) {
     if (! GetInputFileName (name, PATH_MAX,"","TEXT"))  {
        return NULL;
     }
     path = name;
  }
  vnpal = ValNodeNew (NULL);
  tmp = vnpal;
  for (j=1; j<segnumber; j++) {
     vnp = ValNodeNew (NULL);
     tmp->next = vnp;
     tmp = tmp->next;
  }
  if ( (fp = FileOpen (path, "r")) == NULL)  {
     ValNodeFree (vnpal);
     return NULL;
  }
  vnp=NULL;
  lmax = length + length/2;
  str = FGetLine (fp);
  if (str) {
     strp = str;
     while (*strp == ' ' && *strp!='\0' && *strp!='\n')
        strp++;
     if (*strp!='\0' && *strp!='\n')
        strlens = StringLen (strp);
     else 
        str=NULL;   /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
  }
  while (str) 
  {
     if (strlens > 0) {
        if (*strp == '>') {
           if (!insegb) {
              vnp = vnpal;
           }
           else {
              inseg++;
              if (inseg==1)
                 vnp = vnpal;
              else
                 vnp = vnp->next;
           }
           startp = FALSE;
        }
        else if (StringStr(strp, "[")!= NULL) {
           if (vnp!=NULL) {
           }
           insegb = TRUE;
           inseg = 0;
           startp= FALSE;
        } 
        else if (StringStr(strp, "]")!= NULL) {
           insegb = FALSE;
           inseg = 0;
           startp= FALSE;
        } 
        else {
           if (!startp) {
              seqstr=(CharPtr)MemNew((size_t)((lmax + 1) * sizeof(Char)));
              for (strlens=0; strlens<lmax; strlens++) 
                 seqstr[strlens] = ' ';
              if (vnp) {
               if (vnp->data.ptrvalue==NULL) {
                 tmp = NULL;
                 ValNodeAddPointer (&tmp, 0, (Pointer)seqstr);
                 vnp->data.ptrvalue = (Pointer) tmp;
               } else {
                 tmp = (ValNodePtr)vnp->data.ptrvalue;
                 ValNodeAddPointer (&tmp, 0, (Pointer)seqstr);
               }
              }
              lgseq = 0;
              startp = TRUE;
           }              
           for (j=0; j<strlens; j++)
           {
              if (strp[j]=='\n' || strp[j]=='\0' || strp[j]=='\r' )
                 break;
              strp[j] = TO_UPPER(strp[j]);
              if (StringChr("ABCDEFGHIKLMNPQRSTUVWXYZ-*", strp[j]) != NULL) {
                 seqstr [lgseq] = strp[j];
                 lgseq++;
              }
           }
           seqstr [lgseq] = '\0';
        }
     }
     MemFree (str);
     str = FGetLine(fp);
     if (str) {
        strp = str;
        while (*strp == ' ' && *strp!='\0' && *strp!='\n')
           strp++;
        if (*strp!='\0' && *strp!='\n')
           strlens = StringLen (strp);
        else 
           str=NULL;            /****!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
     }
  }
  if (str)
     MemFree(str);
  fclose (fp);
  return vnpal;
}

static SeqEntryPtr GapFastaRead (CharPtr path, Uint2 mol_type)
{
  Char         name[PATH_MAX];
  SeqAnnotPtr  sap = NULL;
  SeqEntryPtr  sep = NULL;
  ValNodePtr   vnp = NULL;
  SeqIdPtr     sip = NULL,
               siptmp = NULL;
  Int4         lmax;
  Int2         nseq = 0,
               seqnumber = 0,
               segnumber;
  CharPtr      str;

  if (path == NULL) {
     if (! GetInputFileName (name, PATH_MAX,"","TEXT"))  {
        return NULL;
     }
     path = name;
  }
  sep = FastaReadAdvanced (path, mol_type, &seqnumber, &segnumber, &sip, &lmax);
  if (sep == NULL) {
     return NULL;
  }
  nseq=0;
  for (siptmp=sip; siptmp!=NULL; siptmp=siptmp->next) {
     nseq++;
  }
  if (nseq != seqnumber*segnumber) {
     ErrPostEx (SEV_ERROR, 0, 0, "Every sequences should have a sequence ID");
     sep = SeqEntryFree (sep);
  }
  str=matching_seqid (sip);  
  if (str) {
     ErrPostEx (SEV_ERROR, 0, 0, "The submission contains several sequences with the same name \"%s\"", str);
     MemFree (str);
     sep = SeqEntryFree (sep);
  } 
  else {
     vnp = ReadAlignmentToStrings (path, lmax, segnumber);
     if (segnumber > 1)
        sap=LocalAlignsToSeqAnnotDimn(vnp,sip,NULL,seqnumber,segnumber, 0, NULL, FALSE);
     else  
        sap=LocalAlign1ToSeqAnnotDimn (vnp, sip, NULL, seqnumber, 0, NULL, FALSE);
     if (sap)
        SeqAlignAddInSeqEntry (sep, sap);
  }
  if (vnp)
     ValNodeFree (vnp);
  SeqIdFree (sip);
  return sep;
}

/********************************************************************/
static Boolean has_extrachar (CharPtr str, Char missingchar, Char gapchar)
{
  Int2     j;
  Boolean  ret = FALSE;
 
  if (str==NULL)
     return TRUE;
  if (*str=='\0' || *str=='\n')
     return TRUE;
  for (j=0; j<StrLen(str); j++) {
     if (str[j]!='\n' && str[j]!='\0' && str[j]!='\r' 
      && str[j]!=' '
      && str[j]!='!') 
     {
        if (!isdigit(str[j])) 
        {
           if ((StringChr ("ABCDGHKMNRSTUVWY", str[j])) == NULL &&
             (StringChr ("abcdghkmnrstuvwy", str[j])) == NULL &&
             str[j]!=gapchar && str[j] != missingchar &&
             str[j]!='[' && str[j]!=']')  {
              ret = TRUE;
              break;
           }
        }
     }
  }
  return ret;
}

/********************************************************************/
static Char nexustoseq (Char car, Char missingchar, Char gapchar)
{
  if (car == ':')
     return ('-');
  if (car == '.')
     return ('-');
  if (car == missingchar)
     return ('N');
  if (car == gapchar)
     return ('-');
  if (isalpha (car))
     return car;
  return ('!');
}

/********************************************************************/
static Boolean ConvertPaupToFastaGap (CharPtr path, CharPtr tmpfile)
{
  FILE       *fp, *fpout;
  CharPtr    tmp;
  CharPtr    str = NULL;
  CharPtr    str2;
  Char       gapchar = '-';
  Char       missingchar = '?';
  Char       car;
  Int4       strlens;
  Int4       lg_seq = 0;
  Int2       n_seq = 0;
  Int2       n_tmp = 0;
  Int4       j, j1, 
             k, 
             k1=0;
  Boolean    first_line;
 
  if ( (fp = FileOpen (path, "r")) == NULL) {
     return FALSE;
  }
  str = FGetLine (fp);
  while (str) {
     if (! stringhasnotext (str)) {
        if (StringLen (str) > 0 && str [0] != '>')
           break;
     }   
     MemFree (str);
     str = FGetLine (fp);
  }
  if (!str) {
    FileClose(fp); 
    return FALSE;
  }
  while (str) {
        tmp = StringStr(str, "INTERLEAVE");
        if (tmp == NULL)
           tmp = StringStr(str, "interleave");
        if (tmp != NULL) {
           n_seq = 0;
           lg_seq= 0;
           ErrPostEx (SEV_ERROR, 0, 0, "This is a NEXUS interleave format"); 
           break;
        }
        tmp = StringStr(str, "GAP=");
        if (tmp == NULL)
           tmp = StringStr(str, "gap=");
        if (tmp != NULL) {
           while (*tmp!='\0' && *tmp!='\n' && *tmp!='=')
              tmp++;
           if (*tmp!='\0' && *tmp!='\n') 
              tmp++;
           while (*tmp!='\0' && *tmp!='\n' && *tmp==' ') 
              tmp++;
           if (*tmp!='\0' && *tmp!='\n')
              gapchar = *tmp;
        }
        tmp = StringStr(str, "MISSING=");
        if (tmp == NULL)
           tmp = StringStr(str, "missing=");
        if (tmp != NULL) {
           while (*tmp!='\0' && *tmp!='\n' && *tmp!='=')
              tmp++;
           if (*tmp!='\0' && *tmp!='\n')
              tmp++;
           while (*tmp!='\0' && *tmp!='\n' && *tmp==' ') 
              tmp++;
           if (*tmp!='\0' && *tmp!='\n')
              missingchar = *tmp;
        }
        if (n_seq == 0) {
           tmp = StringStr(str, "NTAX");
           if (tmp == NULL)
              tmp = StringStr(str, "ntax");
           if (tmp != NULL) {
              while (*tmp!='\0' && *tmp!='\n' && !isdigit (*tmp))
                 tmp++;
              if (*tmp!='\0' && *tmp!='\n')
                 n_seq = (Int2) atoi(tmp);
           }          
        }
        if (lg_seq == 0) {
           tmp = StringStr(str, "NCHAR");
           if (tmp == NULL)
              tmp = StringStr(str, "nchar");
           if (tmp != NULL) {
              while (*tmp!='\0' && !isdigit (*tmp))
                 tmp++;
              if (*tmp!='\0')
                 lg_seq = (Int4) atol(tmp);
           }
        }
        tmp = StringStr(str, "MATRIX");
        if (tmp == NULL)
           tmp = StringStr(str, "matrix");
        if (tmp!=NULL) {
           break;
        }   
        MemFree (str);
        str = FGetLine (fp);
  }   
  if (n_seq == 0 || lg_seq == -1) {
     FileClose(fp); 
     return FALSE;
  }
  while (str) {
     tmp = StringStr(str, "MATRIX");
     if (tmp == NULL)
        tmp = StringStr(str, "matrix");
     if (tmp != NULL)
        break;
     MemFree (str);
     str = FGetLine (fp);
  }
  if (!str) {
     FileClose(fp); 
     return FALSE;
  }
  MemFree (str);
  if ( (fpout = FileOpen (tmpfile, "w")) == NULL) {
     FileClose(fp); 
     return FALSE;
  }
  str = FGetLine (fp);
  first_line = TRUE;
  n_tmp = 0;
  k=0;
  while (str) {
     strlens = StringLen (str);
     if (strlens > 0) {
        str2 = (CharPtr)MemNew((size_t)((strlens+4) * sizeof(Char)));
        if (str[0] == ';' || (tmp = StringStr(str, "end;"))!=NULL || (tmp = StringStr(str, "END;"))!=NULL || (tmp = StringStr(str, "gap data"))!=NULL)
           break;
        if (has_extrachar (str, missingchar, gapchar)) {
           if (strlens ==1) {
              first_line = FALSE;
              str[0]='\0';
              strlens = 0;
           }
           else
              first_line = TRUE;
        }
        j1=j=0;
        while (j < strlens) 
        { 
           if (str[j]=='\0' || str[j] == '\n' || str[j] == '\r' ) {
              str[j1]='\0';
              break;
           }
           if (str[j]=='[') {
              while (str[j]!='\0' && str[j]!='\n' && j < strlens && str[j]!=']')
                 j++;
           }
           if (j < strlens)
           {
            if (!first_line) {
              car = nexustoseq (str[j], missingchar, gapchar);
              if (car != '\0' && car != '!') {
                 str[j1] = car;
                 j1++;
              }
              j++;
            }
            else if (first_line) {
              if (str[j]!=' ') {
                 str[j1] = str[j];
                 j1++;
                 j++;
              }
              else {
                 while (str[j] == ' ')
                    j++;
                 k1=0;
                 while (str[j]!='\0' && str[j]!='\n' && j < strlens) {
                    if (str[j]=='[') {
                       while (str[j]!='\0' && str[j]!='\n' && j < strlens && str[j]!=']')
                          j++;
                    }
                    if (j<strlens) {
                       car = nexustoseq (str[j], missingchar, gapchar);
                       if (car != '\0' && car != '!') {
                          str2[k1] = car;
                          k1++;
                       }
                       str[j] = ' ';
                       j++;
                    }
                 }
                 if (k1>0)
                    str2[k1] = '\0';       
              }
            }
           }
        }
        str[j1]='\0';
        strlens = StringLen (str);
        if (strlens > 0 && !stringhasnocharplus (str)) {
           if (!first_line && has_extrachar (str, missingchar, gapchar)) {
              first_line = TRUE; 
           }
           if (first_line) {
              if (strlens > 1) {
                 fprintf(fpout, ">%s\n", str);
                 first_line = FALSE; 
                 k=0;
                 n_tmp++;
                 if (k1 > 0) {
                    fprintf(fpout, "%s\n", str2);
                    k1 = 0;   
                    k += StringLen (str2);
                 }
              }
           }
           else {
              fprintf(fpout, "%s\n", str);
              k += strlens; 
              if (k >= lg_seq)  {
                 if (n_tmp == n_seq)
                    break;
                 first_line = TRUE;
              }
           }
        }
        MemFree(str2); 
        str2=NULL;
     }      
     k1=0;
     MemFree (str);
     str = FGetLine (fp);
  }    
  if (str)
     MemFree (str);
  FileClose(fp);
  fprintf(fpout, "\n");
  FileClose(fpout);
  return TRUE;
}

static ValNodePtr SequenceMacawRead (CharPtr path, Int2 n_seq)
{
  FILE     *fp;
  ValNodePtr vnp, seqvnp;
  Char     lenstr[8];
  CharPtr  seq,
           str = NULL, 
           tmp;
  Int4     lens = 0;
  Int2     j;
  Boolean  worksheet=FALSE;
  
  if ( (fp = FileOpen (path, "r")) == NULL) {
     return FALSE;
  }
  str = FGetLine (fp);
  while (!worksheet)
  {
     tmp = StringStr(str, "worksheet");
     if (tmp) {
        worksheet=TRUE;
        break;
     }
     MemFree (str);
     str = FGetLine (fp);
  }
  if (worksheet)
  {
     while (str) {
        tmp = StringStr(str, "num-cols");
        if (tmp) {
           while (*tmp!=' ' && *tmp!='\0' && *tmp!='\n')
              tmp++;
           while (!isdigit (*tmp) && *tmp!='\0' && *tmp!='\n')
              tmp++;
           j=0;
           lenstr[0]='\0';
           while (isdigit (*tmp) && *tmp!='\0' && *tmp!='\n') {
              lenstr[j]=*tmp;
              tmp++;
              j++;
           }
           lens = (Int4) atol (lenstr);
           break;
        }
        MemFree (str);
        str = FGetLine (fp);
     }
     if (lens > 0) 
     { 
        seqvnp = new_sequence_vectors (n_seq, lens);
        vnp = seqvnp;
        while (str) {
           tmp = StringStr(str, "gap-seq");
           if (tmp) {
              while (*tmp!='"' && *tmp!='\0' && *tmp!='\n')
                 tmp++;
              if (*tmp=='"') 
              {
                 tmp++;
                 seq = (CharPtr)vnp->data.ptrvalue;
                 j=0;
                 seq[0]='\0';
                 while (*tmp!='"')
                 {
                    if (*tmp=='\0' || *tmp=='\n') {
                       MemFree (str);
                       str = FGetLine (fp);
                       tmp = str;
                    }
                    else {
                       seq[j] = *tmp;
                       tmp++;
                       j++;
                    }
                 }
                 if (*tmp=='"')
                    vnp=vnp->next;
              }
           }
           if (str)
              MemFree (str);
           str = FGetLine (fp);
        }     
     }
  }
  if (str)
     MemFree (str);
  FileClose (fp);
  return seqvnp;
}

static SeqIdPtr NameMacawRead (CharPtr path)
{
  FILE     *fp;
  Char     name [255];
  CharPtr  str = NULL, 
           tmp;
  SeqIdPtr siphead=NULL,
           sip;
  Int2     j;
  
  if ( (fp = FileOpen (path, "r")) == NULL) {
     return FALSE;
  }
  str = FGetLine (fp);
  while (str) {
     tmp = StringStr(str, "name");
     if (tmp) {
        while (*tmp!='"' && *tmp!='\0' && *tmp!='\n')
           tmp++;
        if (*tmp=='"') 
        {
           tmp++;
           j=0;
           name[0]='\0';
           while (*tmp!='"' && *tmp!='\0' && *tmp!='\n' && j<254) {
              name[j] = *tmp;
              tmp++;
              j++;
           }
           name[j]='\0';
           if (StringLen(name) > 0) {
              sip = MakeSeqID (name);
              if (sip) {
                 siphead = AddSeqId (&siphead, sip);
              }
           }
        }
     }
     if (str)
        MemFree (str);
     str = FGetLine (fp);
     tmp = StringStr(str, "worksheet");
     if (tmp) {
        break;
     }
  }
  if (str)
     MemFree (str);
  return siphead;
}

static SeqEntryPtr MacawRead (CharPtr path, Uint1 mol_type, SeqIdPtr seqid, Boolean save_seqentry, Boolean save_sap)
{
  SeqEntryPtr sep = NULL;
  ValNodePtr  seqvnp=NULL,
              vnp = NULL;
  SeqAnnotPtr sap;
  SeqIdPtr    sip;
  Int4        n_seq=0;
  CharPtr     str;
 
  if (seqid == NULL)
     seqid = NameMacawRead (path);
  if (seqid == NULL)
     return NULL;
  str=matching_seqid (seqid);
  if (str)
  {
     ErrPostEx (SEV_ERROR, 0, 0, "The submission contains several sequences with the same name \"%s\"", str);
     MemFree (str);
     SeqIdFree (seqid);
     return NULL;
  }
  for (sip=seqid; sip!=NULL; sip=sip->next) 
     n_seq++;
  seqvnp = SequenceMacawRead (path, n_seq);
  if (seqvnp == NULL)
  {
     SeqIdFree (seqid);
     return NULL;
  }
  if ( save_sap )
     sap = LocalAlignToSeqAnnotDimn (seqvnp, seqid, NULL, n_seq, 0, NULL, FALSE);
  if ( save_seqentry )
     sep = strings_to_seqentry (seqvnp, mol_type, seqid, sap);
  ValNodeFree (seqvnp);
  SeqIdFree (seqid);
  return sep;
}

/*******************************************************
***   ReadAlignmentFunc
***	called for PHYLIP, NEXUS interleave formats
***	calls ReadLocalAlign
***	      ReadLocalName
***           LocalAlignToSeqAnnotDimn
***	returns a SeqEntryPtr sep_list including a SeqAnnotPtr
***
**********************************************************/
static Boolean seq_line (CharPtr str)
{
  CharPtr  str2;
  Int4     lens;
  Int4     val1, 
           val2, 
           j;

  if (str != NULL) 
  {
     str2 = StringSave (str);
     lens = StringLen(str2);
     val1 = 0;
     val2 = 0;
     for (j = lens; j > 0; j--) 
     {
        str2[j] = TO_UPPER(str2[j]);
        if (str2[j] >= 'A' && str2[j] <= 'Z') 
        {
           val1++;
           if (str2[j]=='A' || str2[j]=='C' || str2[j]=='T' ||
               str2[j]=='G' || str2[j]=='N' || str2[j]=='U')
              val2++;
        }
     }
     MemFree(str2);
     if (val2 > (2*val1/3))
        return TRUE;
  }
  return FALSE;
}

static Boolean seq_char (Char car, Char missingchar, Char gapchar)
{
  if (car == 'A') return TRUE;
  if (car == 'T') return TRUE;
  if (car == 'G') return TRUE;
  if (car == 'C') return TRUE;
  if (car == 'U') return TRUE;
  if (car == 'N') return TRUE;
  if (car == 'a') return TRUE;
  if (car == 't') return TRUE;
  if (car == 'g') return TRUE;
  if (car == 'c') return TRUE;
  if (car == 'u') return TRUE;
  if (car == 'n') return TRUE;
  if (car == missingchar) return TRUE;
  if (car == gapchar) return TRUE;
  if (car == '*') return TRUE;
  return FALSE;
}

static Boolean seqa_char (Char car, Char missingchar, Char gapchar)
{
  if (car >= 'A' && car <= 'Z') return TRUE;
  if (car >= 'a' && car <= 'z') return TRUE;
  if (car == missingchar) return TRUE;
  if (car == gapchar) return TRUE;
  if (car == '*') return TRUE;
  return FALSE;
}


static ValNodePtr ReadLocalAlign (CharPtr path, Int2 align_format, Int2 n_seq, Int2 *offset, Int2 *offset_line)

{
  FILE       *fp;
  ValNodePtr seqvnp = NULL, vnp;
  CharPtr    tmp,
             tmp1;
  CharPtr    str = NULL;
  Int4 PNTR  lgseq;
  Int4       lmax;
  Int4       strlens;
  Int2       i_seq, j;	
  Int2       leftmargin;
  Int2       top_lines=0;
  Int4       lg_seq = 0;
  int        val1;
  long       val2;
  Boolean    found_seq;
  Boolean    first;
  Char       gapchar = '-';
  Char       missingchar = '?';
  CharPtr    ptr;

  if ( (fp = FileOpen (path, "r")) == NULL) {
         return NULL;
  }
  str = FGetLine (fp); 
  while (str) {
     if (! stringhasnotext (str)) {
        if (StringLen (str) > 0 && str [0] != '>') 
           break;
     }
     top_lines++;
     if (str)
        MemFree (str);
     str = FGetLine (fp);
  }
  if (align_format == SALSAA_GCG){
     n_seq = 1;
     if (str)
        MemFree (str);
     str = FGetLine (fp);
     while (str) {
        n_seq++;
        MemFree (str);
        str = FGetLine (fp);
     } 
     FileClose(fp);
     fp = FileOpen (path, "r");
     str = FGetLine (fp);
     while (str) {
        if (! stringhasnotext (str)) {
           if (StringLen (str) > 0 && str [0] != '>')
              break;
        }   
        if (str)
           MemFree (str);
        str = FGetLine (fp);
     }
     leftmargin = SALSAA_GCG;
  }
  else if (align_format == SALSA_NEXUS) {
     found_seq = FALSE;
     lg_seq = 0;
     n_seq = 0;
     while (str) {
        /* the following break statement bypassed sequence reading */
        /*
        tmp = StringStr(str, "MATRIX");
        if (tmp == NULL)
           tmp = StringStr(str, "matrix");
        if (tmp != NULL) {
           break;
        }
        */
        if (n_seq == 0) {
           tmp = StringStr(str, "NTAX");
           if (tmp == NULL)  
              tmp = StringStr(str, "ntax");
           if (tmp != NULL) {   
              while (tmp!='\0' && !isdigit (*tmp)) 
                 tmp++;
              if (tmp!='\0') 
                 n_seq = (Int2) atoi(tmp); 
           }
        }
        if (lg_seq == 0) {
           tmp = StringStr(str, "NCHAR");
           if (tmp == NULL)  
              tmp = StringStr(str, "nchar");
           if (tmp != NULL) {   
              while (tmp!='\0' && !isdigit (*tmp)) 
                 tmp++;
              if (tmp!='\0') 
                 lg_seq = (Int4) atol(tmp); 
           }
        }
        tmp = StringStr(str, "GAP=");
        if (tmp == NULL)
           tmp = StringStr(str, "gap=");
        if (tmp != NULL) {
           while (*tmp!='\0' && *tmp!='\n' && *tmp!='=')
              tmp++;
           if (*tmp!='\0' && *tmp!='\n') 
              tmp++;
           while (*tmp!='\0' && *tmp!='\n' && *tmp==' ') 
              tmp++;
           if (*tmp!='\0' && *tmp!='\n')
              gapchar = *tmp;
        }
        tmp = StringStr(str, "MISSING=");
        if (tmp == NULL)
           tmp = StringStr(str, "missing=");
        if (tmp != NULL) {
           while (*tmp!='\0' && *tmp!='\n' && *tmp!='=')
              tmp++;
           if (*tmp!='\0' && *tmp!='\n')
              tmp++;
           while (*tmp!='\0' && *tmp!='\n' && *tmp==' ') 
              tmp++;
           if (*tmp!='\0' && *tmp!='\n')
              missingchar = *tmp;
        }
        if (n_seq>0 && lg_seq>-1 && seq_line (str)) {
           if (seq_char(str[0], missingchar, gapchar) 
           && seq_char(str[1], missingchar, gapchar) 
           && seq_char(str[2], missingchar, gapchar) 
           && seq_char(str[3], missingchar, gapchar) 
           && seq_char(str[4], missingchar, gapchar) 
           && seq_char(str[5], missingchar, gapchar)) {
              leftmargin = 0;
              found_seq = TRUE;
              break;
           }
           for (leftmargin = 0; leftmargin<MAXSTR-1; leftmargin++) {
              if (str[leftmargin] == ' ' 
              && seq_char(str[leftmargin+1], missingchar, gapchar)) {
                 found_seq = TRUE;
                 break;
              }
           }
           break;
        }
        top_lines++;
        if (str)
           MemFree (str);
        str = FGetLine (fp);
     }
     if (!found_seq) 
        n_seq = 0;
     else
        leftmargin++;
  } else if (align_format == SALSAA_NEXUS)
  {
     found_seq = FALSE;
     lg_seq = 0;
     n_seq = 0;
     while (str) {
        /* the following break statement bypassed sequence reading */
        /*
        tmp = StringStr(str, "MATRIX");
        if (tmp == NULL)
           tmp = StringStr(str, "matrix");
        if (tmp != NULL) {
           break;
        }
        */
        if (n_seq == 0) {
           tmp = StringStr(str, "NTAX");
           if (tmp == NULL)
              tmp = StringStr(str, "ntax");
           if (tmp != NULL) {
              while (tmp!='\0' && !isdigit (*tmp))
                 tmp++;
              if (tmp!='\0')
                 n_seq = (Int2) atoi(tmp);
           }
        }
        if (lg_seq == 0) {
           tmp = StringStr(str, "NCHAR");
           if (tmp == NULL)
              tmp = StringStr(str, "nchar");
           if (tmp != NULL) {
              while (tmp!='\0' && !isdigit (*tmp))
                 tmp++;
              if (tmp!='\0')
                 lg_seq = (Int4) atol(tmp);
           }
        }
        tmp = StringStr(str, "GAP=");
        if (tmp == NULL)
           tmp = StringStr(str, "gap=");
        if (tmp != NULL) {
           while (*tmp!='\0' && *tmp!='\n' && *tmp!='=')
              tmp++;
           if (*tmp!='\0' && *tmp!='\n')
              tmp++;
           while (*tmp!='\0' && *tmp!='\n' && *tmp==' ')
              tmp++;
           if (*tmp!='\0' && *tmp!='\n')
              gapchar = *tmp;
        }
        tmp = StringStr(str, "MISSING=");
        if (tmp == NULL)
           tmp = StringStr(str, "missing=");
        if (tmp != NULL) {
           while (*tmp!='\0' && *tmp!='\n' && *tmp!='=')
              tmp++;
           if (*tmp!='\0' && *tmp!='\n')
              tmp++;
           while (*tmp!='\0' && *tmp!='\n' && *tmp==' ')
              tmp++;
           if (*tmp!='\0' && *tmp!='\n')
              missingchar = *tmp;
        }
        if (n_seq>0 && lg_seq>-1 && seq_line (str)) {
           if (seqa_char(str[0], missingchar, gapchar)
           && seqa_char(str[1], missingchar, gapchar)
           && seqa_char(str[2], missingchar, gapchar)
           && seqa_char(str[3], missingchar, gapchar)
           && seqa_char(str[4], missingchar, gapchar)
           && seqa_char(str[5], missingchar, gapchar)) {
              leftmargin = 0;
              found_seq = TRUE;
              break;
           }
           for (leftmargin = 0; leftmargin<MAXSTR-1; leftmargin++) {
              if (str[leftmargin] == ' '
              && seqa_char(str[leftmargin+1], missingchar, gapchar)) {
                 found_seq = TRUE;
                 break;
              }
           }
           break;
        }
        top_lines++;
        if (str)
           MemFree (str);
        str = FGetLine (fp);
     }
     if (!found_seq)
        n_seq = 0;
     else
        leftmargin++;
  }
  else if (align_format == SALSA_PHYLIP || align_format == SALSAA_PHYLIP) {
     if (sscanf (str, "%d %ld", &val1, &val2) == 2) {
        n_seq = (Int2) val1;
        lg_seq = (Int4) val2;
     }
     if (str)
        MemFree (str);
     str = FGetLine (fp);
     leftmargin = SALSA_PHYLIP_MARGIN;
     top_lines++;
  }
  else if (align_format == SALSA_CLUSTALV) {
     if (n_seq == 0) {
        FileClose(fp);
        return NULL;
     }
     for ( j =0; j < 4; j++) {
        if (str)
           MemFree (str);
        str = FGetLine (fp);
        top_lines++;
     }
     leftmargin = SALSA_CLUSTALV_MARGIN;
  }
  else {
     ErrPostEx (SEV_ERROR, 0, 0, "We do not support this format yet");
     return NULL;
  }
  if (n_seq == 0) {
     FileClose(fp);
     return NULL;
  }
  if (lg_seq == 0) 
     lmax = LENGTHMAX;
  else 
     lmax = lg_seq;
  
  *offset = leftmargin;
  seqvnp = new_sequence_vectors (n_seq, lmax);

  lgseq = (Int4Ptr) MemNew((size_t) ((n_seq + 1) * sizeof(Int4)));
  for (j = 0; j < n_seq; j++) lgseq [j] = 0; 
  
  tmp1 = (CharPtr) seqvnp->data.ptrvalue;
  i_seq = 0;
  vnp = seqvnp;
  first = TRUE;
  while (str)
  {
     ptr = StringChr (str, '[');
     if (ptr != NULL) {
        *ptr = '\0';
     }
     strlens = StringLen (str);
     if (strlens > 0) {
           if (str[0] == ';' || (tmp = StringStr(str, "end;"))!=NULL || (tmp = StringStr(str, "END;"))!=NULL || (tmp = StringStr(str, "gap data"))!=NULL)
              break;
           if (! stringhasnocharplus (str) && str[0]!='>') 
           {
                tmp = (CharPtr) vnp->data.ptrvalue;
                for (j = leftmargin; j < strlens && lgseq [i_seq] <= lmax; j++) 
                { 
                   if (str[j] == '\n' || str[j] == '\r' ) break;
                   str[j] = TO_UPPER (str[j]);  
                   if (str[j] == gapchar)
                      str[j] = '-';
                   else if (str[j] == ':')
                      str[j] = '-';
                   else if (str[j] == '.') 
                   { 
                      if (align_format == SALSA_PHYLIP && i_seq != 0)
                      {
/**
                         if (tmp1[])
                            str [j]= tmp1[lgseq[i_seq]];
                         else
**/
                            str [j] = '-'; 
                      }
                      else
                         str[j] = '-';
                   }
                   else if (str[j] == missingchar)
                      str[j] = '-';
                   if ((str[j] >= 'A' && str[j] <= 'Z') || str[j]=='*' || str[j] == '-') { 
                      tmp [lgseq[i_seq]] = str[j]; 
                      ++lgseq [i_seq];
                   }
                }
                ++i_seq;
                if (i_seq == n_seq) {
                   i_seq = 0;
                   vnp = seqvnp;
                   if (align_format == SALSA_PHYLIP && first) {
                      leftmargin=0; 
                      first = FALSE;
                   }
                } 
                else vnp = vnp->next;
           }
     }
     if (str)
        MemFree (str);
     str = FGetLine (fp);
  }
  if (str)
     MemFree (str);
  FileClose(fp);
  for (lmax = 0, j = 0; j < n_seq; j++) 
     if (lgseq[j] > lmax) 
        lmax = lgseq[j];
  for (vnp = seqvnp, j = 0 && vnp != NULL; j < n_seq; j++, vnp = vnp->next) 
  {
     tmp = (CharPtr) vnp->data.ptrvalue;
     tmp [lmax] = '\0';
  }
  if (lg_seq == 0 ) 
     lg_seq = lmax;
  else if (lmax < lg_seq) 
  {
     if (lg_seq < LENGTHMAX ) {
        Message(MSG_OK, "Length in file %d != alignment length %d", (int) lg_seq, (int) lmax);
        /**** FREE DATA STRUCT ***********/
        return NULL;
     }
     lg_seq = lmax;
  }
  *offset_line = top_lines;
  return seqvnp;  
}

static SeqIdPtr ReadLocalName (CharPtr path, Int2 nbseq, Int2 leftmargin, Int2 offset_lines)
{
  FILE       *fp;
  SeqIdPtr   sip1 = NULL,
             sipnew = NULL, siptmp;
  CharPtr    str = NULL;
  Int2       j,
             i_seq = 0;	

  if ( (fp = FileOpen (path, "r")) != NULL) {
     j=0;
     str = FGetLine (fp);
     while (str && j<offset_lines) {
        MemFree (str);
        str = FGetLine (fp);
        j++;
     }
     while (str && i_seq < nbseq ) 
     {
         if ( StringLen (str) > 0 ) 
         {                        
                str [leftmargin] = '\0';
                for (j=leftmargin-1; j>0 && str[j] == ' '; j--) 
                   str[j] = '\0';
                sipnew = MakeSeqID (str);
                if (sip1 == NULL)
                   sip1 = sipnew;
                else
                   siptmp->next = sipnew;
                siptmp = sipnew;
         }
         MemFree (str);
         str = FGetLine (fp);
         ++i_seq;
     }
     if (str)
        MemFree (str);
     FileClose(fp);
  }
  return sip1;
}

static SeqEntryPtr ReadAlignmentFunc (CharPtr path, Uint1 mol_type, Uint1 format, Int2 n_seq, Boolean save_seqentry, Boolean save_sap, SeqIdPtr seqsip) 
{
  SeqEntryPtr  sep = NULL;
  ValNodePtr   seqvnp , vnp;
  SeqAnnotPtr  sap;
  Int2         leftmargin,
               offset_lines,
               k;
  CharPtr      str;

  seqvnp = ReadLocalAlign (path, format, n_seq, &leftmargin, &offset_lines);
  if (seqvnp == NULL)
     return NULL;
  for (k=0, vnp=seqvnp; vnp!=NULL; vnp=vnp->next) k++;
  if (n_seq == 0)
     n_seq = k;
  else 
     if (k != n_seq) {
         ValNodeFree (seqvnp);
         return NULL;
     }
  if (seqsip== NULL)
     seqsip = ReadLocalName (path, n_seq, leftmargin, offset_lines);
  if (seqsip == NULL)
  {
     ValNodeFree (seqvnp);
     return NULL;
  }
  str=matching_seqid (seqsip);
  if (str)
  {
     ErrPostEx (SEV_ERROR, 0, 0, "The submission contains several sequences with the same name \"%s\"", str); 
     MemFree (str);
  }
  else {
     if ( save_sap )
        sap = LocalAlignToSeqAnnotDimn (seqvnp, seqsip, NULL, n_seq, 0, NULL, FALSE);
     if ( save_seqentry )
        sep = strings_to_seqentry (seqvnp, mol_type, seqsip, sap);
  }
  ValNodeFree (seqvnp);
  SeqIdFree (seqsip);
  return sep;
}


/************************************************************
***  ReadLocalAlignment
***	called by sequin2.c 
***	calls ReadInterleaveAlign, ReadContiguouseAlign
***  ReadInterleaveAlign
***	reads formats: Phylip, NEXUS Interleave
***  ReadContiguouseAlign
***	reads formats: Fasta+gaps, NEXUS Contiguous, Macaw
***  ReadAnyAlignment
***	calls first ReadInterleaveAlign
***     if NULL is returned, calls ReadContiguouseAlign
***
************************************************************/
extern SeqEntryPtr ReadInterleaveAlign (CharPtr path, Uint1 mol_type)
{
  FILE        *fp;
  SeqEntryPtr sep=NULL;
  Char        name [PATH_MAX];
  CharPtr     tmp;
  CharPtr     str = NULL;
  int         val1;
  long        val2;
  
  if (path == NULL)
  {
     if (! GetInputFileName (name, PATH_MAX,"","TEXT"))  {
        return NULL;
     }   
     path = name;
  }
  if ( (fp = FileOpen (path, "r")) != NULL) {
     str = get_first_notemptyline (fp);
     FileClose (fp);     
  }
  if (str==NULL)
     return NULL;
  tmp = StringStr(str, "NEXUS");
  if (tmp == NULL)
     tmp = StringStr(str, "nexus");
  if (tmp)
  {
     if (ISA_aa(mol_type))
        sep = ReadLocalAlignment (SALSAA_NEXUS, path);
     else
        sep = ReadLocalAlignment (SALSA_NEXUS, path);
     if (str)
        MemFree (str);
     return sep;
  }
  if (sscanf (str, "%d %ld", &val1, &val2) == 2) {
     if (val1 > 0 && val2 > -1)
     {
        if (ISA_aa(mol_type))
           sep = ReadLocalAlignment (SALSAA_PHYLIP, path);
        else 
           sep = ReadLocalAlignment (SALSA_PHYLIP, path);
     } 
     if (str)
        MemFree (str);
     return sep;
  }
  ErrPostEx (SEV_ERROR, 0, 0, "We do not support this format yet"); 
  return NULL;
}

extern SeqEntryPtr ReadContiguouseAlign (CharPtr path, Uint1 mol_type)
{
  FILE        *fp;
  SeqEntryPtr sep=NULL;
  Char        name [PATH_MAX];
  CharPtr     tmp;
  CharPtr     str = NULL;
  
  if (path == NULL)
  {
     if (! GetInputFileName (name, PATH_MAX,"","TEXT"))  {
        return NULL;
     }   
     path = name;
  }
  if ( (fp = FileOpen (path, "r")) != NULL) {
     str = get_first_notemptyline (fp);
     FileClose (fp);     
  }
  if (str==NULL)
     return NULL;

  if (str[0] == '>' || str[0] == '[')
  {
     if (ISA_aa(mol_type))
        sep = ReadLocalAlignment (SALSAA_FASTGAP, path);
     else 
        sep = ReadLocalAlignment (SALSA_FASTGAP, path);
     if (str)
        MemFree (str);
     return sep;
  }
  tmp = StringStr(str, "NEXUS");
  if (tmp == NULL)
     tmp = StringStr(str, "nexus");
  if (tmp)
  {
     if (!ISA_aa(mol_type))
        sep = ReadLocalAlignment (SALSA_PAUP, path);
     if (str)
        MemFree (str);
     return sep;
  }
  tmp = StringStr(str, "MACAWDATAFILE");
  if (tmp == NULL)
     tmp = StringStr(str, "MacawDataFile");
  if (tmp)
  {
     if (!ISA_aa(mol_type))
        sep = ReadLocalAlignment (SALSA_MACAW, path);
     if (str)
        MemFree (str);
     return sep;
  }
  ErrPostEx (SEV_ERROR, 0, 0, "We do not support this format yet");
  return NULL;  
}

extern SeqEntryPtr ReadAnyAlignment (Boolean is_prot, CharPtr path)
{
  SeqEntryPtr sep = NULL;
  Uint1       mol_type=Seq_mol_na;
  Char        name [PATH_MAX];
  ErrSev      errlev;
 

  if (is_prot)
     mol_type = Seq_mol_aa;
  if (path == NULL)
  {
     if (! GetInputFileName (name, PATH_MAX,"","TEXT"))  {
        return NULL;
     }
     path = name;
  }
  errlev = ErrSetMessageLevel (SEV_FATAL);
  sep = AsnReadForSalsa (path);
  if (sep==NULL) {
     sep = ReadInterleaveAlign (path, mol_type);
  }
  if (sep==NULL) {
     sep = ReadContiguouseAlign (path, mol_type);
  }
  ErrSetMessageLevel (errlev);
  return sep;
}


extern SeqEntryPtr ReadLocalAlignment (Uint1 format, CharPtr path)
{
  SeqEntryPtr sep = NULL;
  Char        name [PATH_MAX];
  Char        tmpfile [PATH_MAX];
 
  if (path == NULL)
  {
     if (! GetInputFileName (name, PATH_MAX,"","TEXT"))  {
        return NULL;
     }   
     path = name;
  }
  switch (format) 
  {
     case SALSA_ND:
            sep = ReadAnyAlignment (FALSE, path);
            break;
            
     case SALSA_INTERLEAVE:
            sep = ReadInterleaveAlign (path, Seq_mol_na);
            break;
     case SALSAA_INTERLEAVE :
            sep = ReadInterleaveAlign (path, Seq_mol_aa);
            break;
     case SALSA_CONTIGUOUS:
            sep = ReadContiguouseAlign (path, Seq_mol_na);
            break;
     case SALSAA_CONTIGUOUS:
            sep = ReadContiguouseAlign (path, Seq_mol_aa);
            break;

     case SALSA_FASTA:
            sep = FastaReadAdvanced (path, Seq_mol_na, NULL, NULL, NULL, NULL); 
            break;
     case SALSAA_FASTA:
            sep = FastaReadAdvanced (path, Seq_mol_aa, NULL, NULL, NULL, NULL); 
            break;

     case SALSA_FASTGAP:
            sep = GapFastaRead (path, Seq_mol_na);
            break;
     case SALSAA_FASTGAP:
            sep = GapFastaRead (path, Seq_mol_aa);
            break;
     case SALSA_PHYLIP:
     case SALSA_NEXUS:
            sep = ReadAlignmentFunc (path, Seq_mol_na, format, 0, TRUE, TRUE, NULL);
            break;

     case SALSAA_PHYLIP:
     case SALSAA_NEXUS:
     case SALSAA_GCG:
            sep = ReadAlignmentFunc (path, Seq_mol_aa, format, 0, TRUE, TRUE, NULL);
            break;
     case SALSA_PAUP:
            TmpNam (tmpfile);
            if (ConvertPaupToFastaGap (path, tmpfile) ) 
            {
               sep = GapFastaRead (tmpfile, Seq_mol_na);
               FileRemove (tmpfile);
            }
            break;
     case SALSA_MACAW :
            sep = MacawRead (path, Seq_mol_na, NULL, TRUE, TRUE);
            break;

     default:
            ErrPostEx (SEV_ERROR, 0, 0, "We do not support this format yet");
            break;
  }
  return sep;
}


/***********************************************************
***
*** Import functions:
***    from any file (ASN.1, FASTA, gi/acc#), 
***       calls ReadAsnFastaOrFlatFile 
***    Download from Entrez
***       copy of FetchFromNet from sequin2.c (JK)
***
************************************************************/

extern SeqAlignPtr ImportFromFile (EditAlignDataPtr adp)
{
  SeqAlignPtr      salp = NULL,
                   salptmp,
                   salp_original=NULL;
  SeqAnnotPtr      sap = NULL;
  SeqEntryPtr      sep = NULL;
  ValNodePtr       importslp = NULL,
                   sqloc = NULL;
  SeqLocPtr        slp;
  Boolean          new_seqalign=FALSE,
                   ok=FALSE,
                   replace_salp=FALSE;
  MsgAnswer        ans;

  if (adp==NULL)
     return NULL;

  importslp = CCReadAnythingLoop (NULL, adp->seq_info);
  if (importslp != NULL) 
  {
     if (adp->sap_original != NULL) 
     {
        salp_original = (SeqAlignPtr)(adp->sap_original->data);
        if (salp_original)
           ok=SeqAlignSeqLocComp (salp_original, importslp); 
        if (!ok)
        {
           if (salp_original->dim==2 || is_dim2seqalign (salp_original))
              salp_original=salp_original;
           else if (salp_original->dim == 1)
           {
              replace_salp = TRUE;
              salp_original=NULL;
           }
           else {
              ans = Message (MSG_OKC, "You have a multiple alignment.\n Importing a sequence will convert it into a multiple pairwise alignment.\n Do you want to continue ?");
              if (ans != ANS_CANCEL) { 
                 salptmp = multseqalign_to_pairseqalign (salp_original);
                 if (salptmp) {
                    SeqAlignListFree (salp_original);
                    adp->sap_original->data = (Pointer) salptmp;
                    salp_original = salptmp;
                 }
                 else
                    salp_original=NULL;
              }
              else 
                 salp_original=NULL;
           }
        }
        if (salp_original)
        {
         slp = (SeqLocPtr) adp->master.region;
         if (slp!=NULL) 
         {
           ValNodeAddPointer (&sqloc, 0, (Pointer)slp);
           sqloc->next = importslp;
           salp = SeqLocListToSeqAlign (sqloc, (Int2)adp->align_format, NULL);
           if (salp!=NULL) 
           {
              if (salp_original != NULL) {
                 salp = SeqAlignLink (salp_original, salp);
                 new_seqalign = TRUE;
              }
           }
           else {
              if (adp->align_format==PRG_BLAST || adp->align_format==PRGALIGNDEFAULT)
                 Message (MSG_OK, "Blast detected no sequence similarity and could not construct an alignment");
              else
                 Message (MSG_OK, "No significant similarity detected. No alignment produced");
           }
         }
         if (!new_seqalign && !replace_salp)
           salp = SeqAlignSetFree (salp);
      }
     }
     else
        Message(MSG_OK, "Can not import a sequence already in the editor"); 
  }
  return salp;
}

/*------------------------------------------------------------*/
typedef struct salfilesfetchform {
  FORM_MESSAGE_BLOCK
  GrouP           accntype;
  TexT            accession;
  ButtoN          accept;
  EditAlignDataPtr adp;
  WindoW          editor_window;
} FetchForm, PNTR FetchFormPtr;

static void FetchFormMessage (ForM f, Int2 mssg)
{
  FetchFormPtr  ffp;

  ffp = (FetchFormPtr) GetObjectExtra (f);
  if (ffp != NULL) {
    switch (mssg) {
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
        if (ffp->appmessage != NULL) {
          ffp->appmessage (f, mssg);
        }
        break;
    }
  }
}

static void FetchTextProc (TexT t)
{
  FetchFormPtr  ffp;

  ffp = (FetchFormPtr) GetObjectExtra (t);
  if (ffp == NULL) return;
  if (TextHasNoText (t)) {
    SafeDisable (ffp->accept);
  } else {
    SafeEnable (ffp->accept);
  }
}

static SeqAlignPtr align_this (SeqEntryPtr sep, SeqLocPtr master, SeqAnnotPtr sap, WindoW editor_window, EditAlignDataPtr adp)
{
  SeqAlignPtr  salp = NULL, 
               salptmp,
               salp_original = NULL;
  ValNodePtr   vnp=NULL,
               vnp2=NULL;
  Int2         n;
  Boolean      ok = FALSE,
               new_seqalign = FALSE,
               replace_salp = FALSE;
  MsgAnswer    ans;

  if (sep==NULL)
     return NULL;
  if (!IS_Bioseq(sep))
     return NULL;

  vnp = SeqEntryToSeqLoc (sep, &n, adp->mol_type); 
  if (vnp)
  {
     if (sap)
     {
        salp_original = (SeqAlignPtr)(sap->data);
        if (salp_original->dim==2 || is_dim2seqalign (salp_original))
           salp_original=salp_original;
        else if (salp_original->dim == 1)
        {
              replace_salp = TRUE;
              salp_original=NULL;
        }
        else {
           ans = Message (MSG_OKC, "You have a multiple alignment.\n Importing a sequence will convert it into a multiple pairwise alignment.\n Do you want to continue ?");
           if (ans != ANS_CANCEL) { 
              salptmp = multseqalign_to_pairseqalign (salp_original);
              if (salptmp) 
              {
                 adp->sap_original->data = (Pointer) salptmp;
                 salp_original = salptmp;
              }
              else
                 salp_original=NULL;
           }
           else 
              salp_original=NULL;
        }
     } 
     if (salp_original)
        ok=SeqAlignSeqLocComp (salp_original, vnp);
     if (!ok)
     { 
        if (master!=NULL)
        {
           ValNodeAddPointer(&vnp2, 0, (Pointer)master);
           vnp2->next=vnp;
           vnp=vnp2;
        }
        salp = SeqLocListToSeqAlign (vnp, adp->align_format, NULL);
        if (salp!=NULL) 
        {
           if (salp_original) 
           {
              salp = SeqAlignLink (salp_original, salp);
              new_seqalign = TRUE;
           }
        }
        else {
           if (adp->align_format==PRG_BLAST || adp->align_format==PRGALIGNDEFAULT)
              Message (MSG_OK, "Blast detected no sequence similarity and could not construct an alignment");
           else
              Message (MSG_OK, "No significant similarity detected. No alignment produced");
        }
        if (new_seqalign || replace_salp) {
           repopulate_panel (editor_window, adp, salp);
        }
        else
           salp = SeqAlignSetFree (salp);
     }
     else
        Message(MSG_OK, "Can not import a sequence already in the editor"); 
  }
  return salp;
}

static SeqEntryPtr SeqEntryNewForBioseq (BioseqPtr bsp)
{
  SeqEntryPtr new_sep;
  BioseqPtr   new_bsp;
  SeqLocPtr   slp;
  SeqIdPtr    new_sip;

  slp=SeqLocIntNew(0, bsp->length-1, Seq_strand_plus, bsp->id);
  new_sip = MakeNewProteinSeqId (slp, NULL);
  new_bsp=BioseqCopy(new_sip, bsp->id, 0, bsp->length-1, Seq_strand_plus, TRUE);              
  if (new_bsp) {
     new_sep=SeqEntryNew();
     new_sep->choice = 1;
     new_sep->data.ptrvalue=(Pointer)new_bsp;
     SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) new_bsp, new_sep);
  }
  ValNodeFree(slp);
  return new_sep;
} 

static void CCDownloadProc (ButtoN b)

{
  CharPtr       accn;
  BioseqPtr     bsp;
  Uint2         entityID;
  FetchFormPtr  ffp;
  SeqEntryPtr   sep;
  Char          str [32];
  Int4          uid;
  ForM          w;
  Boolean       is_na,
                is_newbsp=FALSE;
  SeqEditViewProcsPtr  svpp;

  ffp = (FetchFormPtr) GetObjectExtra (b);
  if (ffp == NULL) return;
  svpp = (SeqEditViewProcsPtr) GetAppProperty ("SeqEditDisplayForm");
  if (svpp == NULL)
     return;
  if (svpp->download == NULL) 
     return;
  w = ffp->form;
  Hide (w);
  Update ();
  GetTitle (ffp->accession, str, sizeof (str));
  if (StringHasNoText (str)) {
    Message (MSG_OK, "Please enter an accession number or gi");
    Show (w);
    Select (w);
    Select (ffp->accession);
    return;
  }
  is_na = (Boolean) (ISA_na(ffp->adp->mol_type));
  WatchCursor ();
  sep = NULL;
  uid = 0;
  accn = NULL;
  if (GetValue (ffp->accntype) == 1) {
    accn = &(str [0]);
  } 
  else {
    if (! StrToLong (str, &uid)) {
     uid = 0;
    }
  }
  sep = svpp->download ("Salsa", accn, uid, is_na, &is_newbsp);
  if (sep == NULL) {
     Message (MSG_OK, "Unable to find this record in the database.");
  }
  else { 
     Remove (w);
     if (!is_newbsp) 
     {
        if (IS_Bioseq(sep)) {
           sep = SeqEntryNewForBioseq ((BioseqPtr)sep->data.ptrvalue);
        } else {
           SeqEntryFree (sep);
           sep=NULL;
        }
     } 
     if (sep!=NULL) {
        align_this(sep, (SeqLocPtr)ffp->adp->master.region, 
                           ffp->adp->sap_original, ffp->editor_window, ffp->adp);
     }
  } 
  ArrowCursor ();
  return;
}

static void CCCommonFetchFromNet (BtnActnProc actn, BtnActnProc cancel, EditAlignDataPtr adp, WindoW editor_window)

{
  GrouP              c;
  FetchFormPtr       ffp;
  GrouP              g;
  WindoW             w;

  Update ();
  w = NULL;
  ffp = MemNew (sizeof (FetchForm));
  if (ffp != NULL) {
    w = FixedWindow (-50, -33, -10, -10, "Download From Entrez", NULL);
    SetObjectExtra (w, ffp, StdCleanupFormProc);
    ffp->form = (ForM) w;
    ffp->formmessage = FetchFormMessage;
    ffp->adp= adp;
    ffp->editor_window = editor_window;
    SetGroupSpacing (w, 10, 10);

    g = HiddenGroup (w, -3, 0, NULL);
    StaticPrompt (g, "Type", 0, stdLineHeight, programFont, 'l');
    ffp->accntype = HiddenGroup (g, 4, 0, NULL);
    RadioButton (ffp->accntype, "Accession");
    RadioButton (ffp->accntype, "GI");
    SetValue (ffp->accntype, 1);
    ffp->accession = DialogText (g, "", 6, FetchTextProc);
    SetObjectExtra (ffp->accession, ffp, NULL);

    c = HiddenGroup (w, 4, 0, NULL);
    SetGroupSpacing (c, 10, 2);
    ffp->accept = DefaultButton (c, "Retrieve", actn);
    SetObjectExtra (ffp->accept, ffp, NULL);
    Disable (ffp->accept);
    PushButton (c, "Cancel", cancel);

    AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, NULL);
    RealizeWindow (w);
    Show (w);
    Select (w);
    Update ();
  }
}

extern void CCFetchFromNet (EditAlignDataPtr adp, WindoW editor_window)
{
  CCCommonFetchFromNet (CCDownloadProc, StdCancelButtonProc, adp, editor_window);
}


/************************************************************
***  EditBioseqToFasta called by salparam.c
SHOULD BE SEQ-ENTRY /Bioseq, Bioseqet

  if ( (fout = FileOpen ("ffile", "w")) != NULL) {
     sep = bssp->seq_set;
     while (sep != NULL)
     {
         count++;
         if (count == 1 && !firstout) {}
         else {
            bsp = (BioseqPtr) sep->data.ptrvalue;
            EditBioseqToFasta (bsp, fout, -1, -1);
         }
         sep = sep->next;
     }
     FileClose (fout);
     return TRUE;
  }
************************************************************/
extern void EditBioseqToFasta (BioseqPtr bsp, FILE *fout, Int4 from, Int4 to)
{
  SeqLocPtr        slp;
  SeqPortPtr       spp;
  Char             buffer[128];
  Char             str [128];
  Int4             txt_out;
  Int4             Width_Page = 60;
  Int4             j;

  if (bsp == NULL) 
     return;
  if (fout == NULL)
     return; 
  SeqIdWrite (SeqIdFindBest(bsp->id, 0), str, PRINTID_FASTA_LONG, 120);
  if (from < 0) 
     from = 0;
  if (to < 0) 
     to =  bsp->length-1;
  fprintf (fout, ">%s   (%ld - %ld)\n", str, (long)(from+1), (long)(to+1));
  slp = SeqLocIntNew (from, to, Seq_strand_plus, SeqIdFindBest(bsp->id, 0));
  if ( bsp->mol == Seq_mol_aa )
     spp = SeqPortNewByLoc (slp, Seq_code_ncbieaa);
  else
     spp = SeqPortNewByLoc (slp, Seq_code_iupacna);
  j = 0;
  while ( j < SeqLocStop (slp) - SeqLocStart (slp) +1)
  {
     txt_out = ReadBufferFromSep (spp, buffer, j, j +Width_Page, 0);
     if (txt_out == 0) break;
     j += txt_out;
     fprintf(fout, "%s\n", buffer);
  }
  SeqPortFree (spp);
  return;
}

/************************************************************/
extern Int2 seqannot_write (SeqAnnotPtr sap, CharPtr path)
{
  Char         name[PATH_MAX];
  AsnIoPtr     aip;
  AsnTypePtr   atp;
  AsnModulePtr amp;
  SeqAnnotPtr  saptmp;
  SeqAlignPtr  salp;

  if ( sap == NULL ) {
         return 0;
  }
  if ( ( salp = (SeqAlignPtr) sap->data ) == NULL ) {
         return 0;
  }
  if ( salp->segtype == COMPSEG ) {
         saptmp = SeqAnnotBoolSegToDenseSeg (sap); 
  }
  else   saptmp = sap;
  if (path == NULL) {
     if (! GetInputFileName (name, PATH_MAX,"","TEXT"))  {
        return 0;
     }
     path = name;
  }
  amp = AsnAllModPtr ();
  atp = AsnTypeFind (amp,"Seq-annot");
  if ((aip = AsnIoOpen (path, "w")) == NULL) {
         return 0;
  }
  while ( sap != NULL ) {
         if ( ! SeqAnnotAsnWrite ( sap, aip, atp ) ) {
                break;
         }
         sap = sap->next;
  }
  AsnIoReset(aip);
  aip = AsnIoClose (aip);
  if ( salp->segtype == COMPSEG ) CompSeqAnnotFree (saptmp);
  return 1;
}

/************************************************************/
extern void seqalign_write (SeqAlignPtr salp, CharPtr path)
{
  SeqAnnotPtr  sap;

  if (salp!=NULL) {
     sap = SeqAnnotNew (); 
     if (sap != NULL) {
        sap->type = 2; 
        sap->data = (Pointer) salp; 
        seqannot_write (sap, path); 
        sap->data = NULL; 
        sap = SeqAnnotFree (sap); 
     }
  }
}

/************************************************************
*************************************************************
*************************************************************
*************************************************************
*** FEATURES
*********************************************************
*************************************************************
*************************************************************
*************************************************************
*************************************************************
**********************************************************/
static Boolean seqentry_write (SeqEntryPtr sep, CharPtr path)
{
  Char         name[PATH_MAX];
  AsnIoPtr     aip;
  AsnTypePtr   atp;
  AsnModulePtr amp;

  if ( sep == NULL ) {
    return 0;
  }
  if (path == NULL )
  {
     if (! GetInputFileName (name, PATH_MAX,"","TEXT"))  {
        return 0;
     }
     path = name;
  }
  amp = AsnAllModPtr ();
  atp = AsnTypeFind (amp,"SeqEntry");
  if ((aip = AsnIoOpen (path,"w")) == NULL) {
    return 0;
  }
  if ( ! SeqEntryAsnWrite ( sep, aip, atp ) ) {
  }
  aip = AsnIoClose (aip);
  return 1;
}

static void get_client_rect (PaneL p, RectPtr prc)
{
  ObjectRect (p, prc);
  InsetRect (prc, HRZ_BORDER_WIDTH, VER_BORDER_WIDTH);
}

/***************************************************************
***  switch_featOrder
***
*****************************************************************/
static void switch_featOrder (EditAlignDataPtr adp, Uint1 choice)
{
  Int2  oldstyle;
  Int2  j;
  Int4  groupNum;

  if (choice > 0) {
     oldstyle = GetMuskCurrentSt ();
     SetMuskCurrentSt (GetMuskStyleName (adp->styleNum));
     for(j =0; j<FEATDEF_ANY; j++) 
     {   
        adp->featOrder[j] = (Uint1)GetMuskCParam(j, MSM_FORDER, MSM_NUM); 
        groupNum = (Uint1)GetMuskCParam(j, MSM_FGROUP, MSM_NUM); 
        adp->groupOrder[j] = (Uint1)GetMuskCParam(MSM_GROUPS, (Int2)groupNum, MSM_NUM); 
     } 
     SetMuskCurrentSt (GetMuskStyleName (oldstyle));
  } 
  else
     for(j=0; j<FEATDEF_ANY; ++j) adp->featOrder[j] = choice;
}


/*********************************************
***   sesp_to_pept
***
***
*********************************************/
typedef struct ccid {
  SeqIdPtr    sip;
  SeqEntryPtr sep;
  BioseqPtr   bsp;
} CcId, PNTR CcIdPtr;
 
static void FindSeqEntryForSeqIdCallback (SeqEntryPtr sep, Pointer mydata,
                                          Int4 index, Int2 indent)
{
  BioseqPtr          bsp;
  SeqIdPtr           sip;
  CcIdPtr            cip;
 
  if (sep != NULL && sep->data.ptrvalue && mydata != NULL) {
     cip = (CcIdPtr)mydata;
     if (IS_Bioseq(sep)) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        if (bsp!=NULL && ISA_na (bsp->mol)) {
           sip = SeqIdFindBest(bsp->id, 0);
           if (SeqIdForSameBioseq(cip->sip, sip))
              cip->sep = sep;
              cip->bsp = bsp;
        }
     }   
  }
}
 
static Int2 CC_SeqEntryToGeneticCode (Uint2 entityID, SeqIdPtr sip)
{
  SeqEntryPtr sep_head,
              sep;
  CcId        ci;
  Int2        genCode = 0;

  sep_head  = GetTopSeqEntryForEntityID (entityID);
  ci.sip = SeqIdDup (sip);
  ci.sep = NULL;
  ci.bsp = NULL;
  SeqEntryExplore(sep_head,(Pointer)&ci, FindSeqEntryForSeqIdCallback);
  sep = ci.sep;
  SeqIdFree (ci.sip);
  if (sep!=NULL)
     genCode = SeqEntryToGeneticCode (sep, NULL, NULL, 0);
  return genCode;
}


extern Boolean sesp_to_pept (SelEdStructPtr cds, SeqAlignPtr salp, ValNodePtr sqlocs, Boolean partial)
{
  SelEdStructPtr cdsp;
  SelEdStructPtr cds1;
  SeqLocPtr      slp;
  SeqIntPtr      sit;
  ValNodePtr     pept;
  ByteStorePtr   bsp;
  CharPtr        pep = NULL;
  CharPtr        pepPtr = NULL;
  CharPtr        str = NULL, 
                 strPtr = NULL;
  CharPtr        buffer = NULL, 
                 bufferPtr = NULL;
  Int4           k, 
                 strlens, slplens;
  Int4           sumlens;
  Int2           genCode;
  Int2           cb;
  Uint1          codonbase;
  Uint1          codonstart;
  Uint1          strand;

  if ( cds == NULL || salp == NULL )
     return FALSE;
  if (cds->regiontype == 0 || cds->region == NULL)
     return FALSE;
  cds1 = cds;
  while (cds1->prev != NULL) {
     cds1 = cds1->prev;
  }
  slp = sesp_to_slp (cds1, salp, sqlocs, partial);
  slplens = SeqLocLen (slp);
  if ( slplens < 3 ) {
     SeqLocFree (slp);
     return FALSE;
  }
  if (SeqLocStart(slp) > 0)
     codonstart = 1;
  else 
     codonstart = cds1->codonstart;
  strand = SeqLocStrand (slp);

  genCode = CC_SeqEntryToGeneticCode (cds1->entityID, SeqLocId(slp));
  if (genCode == 0)
     genCode = Seq_code_ncbieaa;

  codonbase = 0;
  sit = (SeqIntPtr) slp->data.ptrvalue;
  if (strand == Seq_strand_minus && codonstart > 1) {
     cb = (Int2)(slplens % (Int4) 3);
     if (cb == 1) {
          sit->to --;
     }
  }
  else if (strand == Seq_strand_minus) {
     cb = (Int2)(slplens % (Int4) 3);
     if (cb == 1 && sit->from >0) {
          sit->from --;
     } else if (cb == 2) {
          sit->from ++;
     }
     if (cb == 0) codonbase = 0;
     else if (cb == 1) codonbase = 1;
     else if (cb == 2) codonbase = 2;
  }  
  slplens = SeqLocLen (slp);
  bsp = cds_to_pept (slp, codonstart, genCode, TRUE);
  str = (CharPtr) BSMerge (bsp, NULL);
  BSFree (bsp);
  pep = MemNew ((size_t) ((slplens + 5) *sizeof(Char)));
  pep = emptystring (pep, (Int4)(slplens + 5));
  pep [slplens + 3] = '\0';
  pepPtr = pep;
  *pepPtr = ' ';
  pepPtr += codonbase +1;
  strlens = 3*StringLen(str);
  if (slplens < strlens) {
     strlens=(Int4)(slplens/(Int4)3);
     str [strlens] ='\0';
  }
  if (strand == Seq_strand_minus)
     reverse_string (str);
  strlens = StringLen(str);
  strPtr = str;
  for (k = 0; k < strlens; k++, pepPtr += 3, strPtr++) {
          *pepPtr = *strPtr; 
  }
  MemFree (str);
/*
  strlens = SeqLocLen (slp) + 5;
  buffer = MemNew ((size_t) (strlens *sizeof(Char)));
  buffer = emptystring (buffer, strlens);
  buffer [strlens -1] = '\0';
  bufferPtr = buffer;
  *bufferPtr = ' ';
  sip = SeqLocId (slp);
  for (cdsp= cds1; cdsp != NULL; cdsp = cdsp->next)
  {
     slp = (SeqLocPtr) cdsp->region; 
     buffer = ReadBufferFromSap (pep, buffer, salp, sip, SeqLocStart(slp), SeqLocStop(slp));
  }
  MemFree (pep);
*/
  buffer = pep;
  SeqLocFree (slp);
  if (cds1->data != NULL) {
     pept = cds1->data;
     cds1->data = NULL;
     pept->data.ptrvalue = MemFree (pept->data.ptrvalue);
     pept = ValNodeFree (pept);
  }
  sumlens = 0;
  for (cdsp= cds1; cdsp != NULL; cdsp = cdsp->next)
  {
     pept = ValNodeNew (NULL);
     pept->choice = 0;
     pept->data.ptrvalue = (Pointer) buffer;
     cdsp->data = pept;
     cdsp->offset = sumlens;
     sumlens += SeqLocLen ((SeqLocPtr) cdsp->region);
     pept = NULL;
  }
  return TRUE;
}


/*******************************************************************
***   TranslateProc, TranslateButton
***
***   CdRgnToProtProc
***
********************************************************************/
extern void CdRgnToProtProc (PaneL pnl,  EditAlignDataPtr adp)
{
  WindoW           temport;
  SelStructPtr     ssp;
  SelEdStructPtr   cds;
  ValNodePtr       feathead = NULL, 
                   vnp = NULL;
  Uint2            itemsubtype;
  Boolean          seq_select = FALSE;
  float hratio;
  
  hratio = (float)adp->hoffset / (float)adp->length;
  ssp = ObjMgrGetSelected(); 
  for (; ssp != NULL; ssp = ssp->next)
  {  
     if ( checkssp_for_editor (ssp) && ssp->itemtype == OBJ_VIRT) { 
        feathead = adp->feat;
        itemsubtype = FEATDEF_CDS;
     }
     else if ( checkssp_for_editor (ssp) && ssp->itemtype == OBJ_SEQFEAT) {
        feathead = adp->seqfeat;
        itemsubtype = SEQFEAT_CDREGION;
     }
     else feathead = NULL;
     if (feathead != NULL)
     {
        for (vnp = feathead; vnp != NULL; vnp = vnp->next)
        {
           if (vnp->choice == itemsubtype) {
              cds = (SelEdStructPtr) vnp->data.ptrvalue;
              if (cds->entityID == ssp->entityID && cds->itemID == ssp->itemID) 
              {
                 if (sesp_to_pept(cds, (SeqAlignPtr) adp->sap_align->data, adp->sqloc_list, TRUE))
                    seq_select = TRUE;
                 break;
              }
           }
        }
     }
  }
  if (!seq_select) return;
  data_collect_arrange (adp, TRUE);
  SeqEdSetCorrectBarMax (pnl, adp, hratio);
  temport = SavePort(ParentWindow(pnl));
  Select (pnl);
  inval_panel (pnl, -1, -1);
  RestorePort (temport);
  return; 
}

/*******************************************************************
***
***   UntranslateFunc
***
********************************************************************/
extern void UntranslateFunc (PaneL pnl,  EditAlignDataPtr adp)
{
  WindoW           temport;
  SelStructPtr     ssp;
  SelEdStructPtr   cds;
  ValNodePtr       pept;
  ValNodePtr       feathead = NULL, vnp = NULL;
  Uint2            itemsubtype;
  Boolean          seq_select = FALSE;
  float hratio;
  
  hratio = (float)adp->hoffset / (float)adp->length;
  ssp = ObjMgrGetSelected(); 
  for (; ssp != NULL; ssp = ssp->next)
  {  
     if ( checkssp_for_editor (ssp) && ssp->itemtype == OBJ_VIRT) { 
        feathead = adp->feat;
        itemsubtype = FEATDEF_CDS;
     }
     else if ( checkssp_for_editor (ssp) && ssp->itemtype == OBJ_SEQFEAT) {
        feathead = adp->seqfeat;
        itemsubtype = SEQFEAT_CDREGION;
     }
     else feathead = NULL;
     if (feathead != NULL) {
        for (vnp = feathead; vnp != NULL; vnp = vnp->next) {
           if (vnp->choice == itemsubtype) 
         {
              cds = (SelEdStructPtr) vnp->data.ptrvalue;
              if (cds->entityID == ssp->entityID && cds->itemID == ssp->itemID) 
              {
                 if (cds->data != NULL) {
                    pept = cds->data;
                    cds->data = NULL;
                    pept->data.ptrvalue = MemFree (pept->data.ptrvalue);
                    ValNodeFree (pept);
                    for (; cds != NULL; cds = cds->next) {
                       cds->data = NULL;
                    }
                    seq_select = TRUE;
                 }
                 break;
              }
           }
        }
     }
  }
  if (!seq_select) return;
  data_collect_arrange (adp, TRUE);
  SeqEdSetCorrectBarMax (pnl, adp, hratio);
  temport = SavePort(ParentWindow(pnl));
  Select (pnl);
  inval_panel (pnl, -1, -1);
  RestorePort (temport);
  return; 
}



extern Boolean ShowFeatureFunc (EditAlignDataPtr adp)
{
  AlignNodePtr     anp;
  ValNodePtr       vnp;
  SeqLocPtr        slp;
  Boolean          seq_select = FALSE;

  switch_featOrder (adp, 1);
  adp->seqfeat = SeqfeatlistFree (adp->seqfeat);
  for (vnp = adp->anp_list; vnp != NULL; vnp = vnp->next) {
     anp = (AlignNodePtr) vnp->data.ptrvalue;
     if ( anp != NULL ) {
        if ( anp->segs->cnp == NULL ) {
           slp = CollectSeqLocFromAlignNode (anp);
           CollectFeatureForAlign (slp, anp, adp->featOrder, adp->groupOrder);
           adp->seqfeat=CollectFeatureForEditor (slp, adp->seqfeat, anp->seq_entityID, anp->bsp_itemID, adp->featOrder, FALSE);
           seq_select = TRUE;
        } 
     }
  }
  if (!seq_select) {
     switch_featOrder (adp, 0);
     return FALSE;
  }
  OrderFeatProc (adp->anp_list);
  if (adp->seqfeat != NULL)
     checkselectsequinfeature_for_editor (adp->seqfeat);
  return TRUE;
}

/***********************************************************
***
***  HideFeatureProc
***
*** loop on Bioseq to delete the features in those selected only.
***
***********************************************************/
extern Boolean HideFeatureFunc (EditAlignDataPtr adp)
{
  AlignNodePtr     anp;
  SelStructPtr     ssp;
  ValNodePtr       vnp;
  AlignSegPtr      asp, aspnext;
  Boolean          seq_select = FALSE;

  switch_featOrder (adp, 0);
  if (adp->input_format == OBJ_BIOSEQ) 
  {
     if ( checkOMss_for_itemtype (OBJ_BIOSEQ) == 0 ) 
          ssp = &(adp->master);
     else ssp = ObjMgrGetSelected();  
     for (; ssp != NULL; ssp = ssp->next)  {
         if ( checkssp_for_editor (ssp) && ssp->itemtype == OBJ_BIOSEQ )  {
            adp->seqfeat =SeqfeatlistFree_fromID (adp->seqfeat, ssp->entityID);
            anp = (AlignNodePtr) AlignNodeFind (adp->anp_list, ssp->entityID, ssp->itemID, ssp->itemtype);
            if ( anp != NULL ) {
               asp = anp->segs;
               while(asp !=NULL)
               {
                     aspnext = asp->next;
                     if(asp->cnp != NULL)
                           FreeFeatureList(asp->cnp);
                     asp->cnp = NULL;
                     if(asp->mismatch)
                           ValNodeFree(asp->mismatch);
                     asp->mismatch = NULL;
                     asp = aspnext;
               }
               seq_select = TRUE;
            }
         }
     }
  }
  else if (adp->input_format == OBJ_SEQALIGN) 
  {
     for (vnp = adp->anp_list; vnp != NULL; vnp = vnp->next) {
            anp = (AlignNodePtr) vnp->data.ptrvalue;
            if ( anp != NULL ) {
               asp = anp->segs;
               while(asp !=NULL)
               {
                     aspnext = asp->next;
                     if(asp->cnp != NULL)
                           FreeFeatureList(asp->cnp);
                     asp->cnp = NULL;
                     if(asp->mismatch)
                           ValNodeFree(asp->mismatch);
                     asp->mismatch = NULL;
                     asp = aspnext;
               }
               seq_select = TRUE;
            }
     }
     if (seq_select) adp->seqfeat =SeqfeatlistFree (adp->seqfeat);
  }
  if (!seq_select) {
     switch_featOrder (adp, 1);
     return FALSE;
  }
  return TRUE;
}

/***********************************************************
***
***  ResetFeatureProc
***
***********************************************************/
extern Boolean ResetFeatureFunc (EditAlignDataPtr adp)
{
/*
  AlignNodePtr     anp;
  ValNodePtr       vnp;
  SeqLocPtr        slp;
  AlignSegPtr      asp, aspnext;
  Boolean          seq_select = FALSE;

  switch_featOrder (adp, 1);
  ssp = Gettranslation (adp->seqfeat);
  adp->seqfeat = SeqfeatlistFree (adp->seqfeat);
  for (vnp = adp->anp_list; vnp != NULL; vnp = vnp->next) {
     anp = (AlignNodePtr) vnp->data.ptrvalue;
     if ( anp != NULL ) {
        asp = anp->segs;
        while(asp !=NULL)
        {
           aspnext = asp->next;
           if(asp->cnp != NULL)
                           FreeFeatureList(asp->cnp);
           asp->cnp = NULL;
           if(asp->mismatch)
                           ValNodeFree(asp->mismatch);
           asp->mismatch = NULL;
           asp = aspnext;
           slp = CollectSeqLocFromAlignNode (anp);
           CollectFeatureForAlign (slp, anp, adp->featOrder, adp->groupOrder);
           adp->seqfeat=CollectFeatureForEditor (slp, adp->seqfeat, anp->seq_entityID, anp->bsp_itemID, adp->featOrder, FALSE);
        } 
     }
  }
  if (adp->seqfeat !=NULL) {
        for (vnp = adp->seqfeat; vnp != NULL; vnp = vnp->next)
        {
           if (vnp->choice == SEQFEAT_CDREGION) {
              cds = (SelEdStructPtr) vnp->data.ptrvalue;
              if (cds->entityID == ssp->entityID && cds->itemID == ssp->itemID)
              {
                 if (sesp_to_pept(cds, (SeqAlignPtr) adp->sap_align->data, adp->
sqloc_list, TRUE))
              }
           }
        }
  }
*/
  return TRUE;
}

/******************************************************************/
static SelEdStructPtr split_feat (SelEdStructPtr feat, Int4 pos, Int4 changevalue)
{
  SeqLocPtr      slpfeat;
  SeqIntPtr      sitfeat;
  SelEdStructPtr new, next;
  Int4           from , to;

  slpfeat = (SeqLocPtr) feat->region;
  next = feat->next;
  if (changevalue >= 0)
     from = (Int4)(pos + changevalue);
  else {
     from = pos;
  }
  to = (Int4)(SeqLocStop(slpfeat) + changevalue);
  sitfeat = (SeqIntPtr) slpfeat->data.ptrvalue;
  sitfeat->to = pos -1;
  new = new_seledstruct (feat->entityID, feat->itemID, feat->itemtype, 0, feat->bsp_itemID, from, to, SeqLocId (slpfeat), SeqLocStrand (slpfeat), FALSE, feat->label, feat->data, feat->offset + SeqLocLen(slpfeat), 1);
  feat->next = new;
  new->next = next;
  new->prev = feat;
  return new;
}

extern ValNodePtr update_featpept (EditAlignDataPtr adp, ValNodePtr feathead, RecT *rp, SelStructPtr ssp, Int4 changevalue, Uint2 itemsubtype)
{
  ValNodePtr     vnpfeat,
                 vnpfeatnext;
  SelEdStructPtr feat,
                 feat1, next;
  SeqLocPtr      slpfeat;
  SeqLocPtr      slpssp;
  SeqIntPtr      sitfeat;
  Int4           width;
  Int4           lg;
  Boolean        overlap, precede, succeed, deletefeat;

  if (ssp == NULL) return feathead;
  if (ssp->regiontype == 0 || ssp->region == NULL) return feathead;
  slpssp = (SeqLocPtr) ssp->region;
  if (SeqLocStart(slpssp) == SeqLocStop(slpssp)) return feathead;  
  width = adp->visibleWidth;
  if (adp->columnpcell > 0) 
         width += (Int2) adp->visibleWidth / (Int2) adp->columnpcell;
  vnpfeat = feathead;
  while (vnpfeat != NULL)
  {
    vnpfeatnext = vnpfeat->next;   
    feat1 = (SelEdStructPtr) vnpfeat->data.ptrvalue;
    if( feat1 != NULL && (vnpfeat->choice ==itemsubtype || itemsubtype == 255)) 
    {
      feat = feat1;
      while (feat != NULL) 
      {
         next = feat->next;
         slpfeat = (SeqLocPtr) feat->region;
         overlap = overlapp_ssp (slpssp, slpfeat); 
         precede = precede_ssp (slpssp, slpfeat);
         succeed = succeed_ssp (slpssp, slpfeat);
         deletefeat = FALSE;
         if (overlap || precede || succeed ) 
         {
            sitfeat = (SeqIntPtr) slpfeat->data.ptrvalue;
            if (precede) 
            {
               sitfeat->from = sitfeat->from + changevalue ;
               sitfeat->to = sitfeat->to + changevalue ;
            }
            else if (succeed) 
            {
               if (changevalue < 0 ) sitfeat->to =sitfeat->to +changevalue ;
            }
            else if (overlap) 
            {
               if (changevalue < 0)
               {
                  if ( include_ssp (slpssp, slpfeat) ) {
                     deletefeat = TRUE;
                     feathead = del_ssp_fromid (feathead, itemsubtype, feat);
                     if (rp != NULL)
                        inval_rect (rp->left, rp->top, rp->right, rp->bottom);
                  }
                  else if ( include_ssp (slpfeat, slpssp) ) {
                     if (!adp->spliteditmode)
                          sitfeat->to = sitfeat->to + changevalue;
                     else
                          feat=split_feat(feat,SeqLocStart(slpssp), changevalue);
                  }
                  else if ((lg = overlapp_startssp (slpssp, slpfeat)) > 0) {
                     if (changevalue < 0) {
                          sitfeat->from = sitfeat->from - (abs(changevalue)-lg);
                          sitfeat->to = sitfeat->to + changevalue;
                     }
                     else 
                        ErrPostEx (SEV_ERROR, 0, 0, "Cut what ?");
                  }
                  else if ((lg = overlapp_startssp (slpfeat, slpssp)) > 0) {
                     if (changevalue < 0) {
                          sitfeat->to = sitfeat->to - lg ;
                     } 
                     else sitfeat->to = sitfeat->to + lg ;
                  }
               }
               else {
                  if (!adp->spliteditmode)
                     sitfeat->to = sitfeat->to + changevalue ;
                  else {
                     feat = split_feat (feat, SeqLocStart(slpssp), changevalue);
                  }
               }
            }
            if (!deletefeat && rp != NULL)
            {
               inval_selstruct(adp, feat->entityID, feat->itemID, feat->itemtype, itemsubtype, rp, adp->margin.left,(Int2)(width *adp->charw));
               inval_selstruct(adp, feat->entityID, feat->itemID, feat->itemtype, itemsubtype, rp, adp->margin.left, (Int2)(width *adp->charw));
            }
         }
         feat = next;
      }
      if (feat1 != NULL)
         if (feat1->data != NULL)
            sesp_to_pept (feat1, (SeqAlignPtr) adp->sap_align->data, adp->sqloc_list, TRUE);
    }
    vnpfeat = vnpfeatnext;
  }
  return feathead;
}

static Boolean anpp_has_feature (ValNodePtr anp_list)
{
  ValNodePtr vnp;
  AlignNodePtr anp;
  AlignSegPtr asp;
  Boolean featb;

  for (vnp = anp_list; vnp != NULL; vnp = vnp->next) {
     anp = (AlignNodePtr)vnp->data.ptrvalue;
     featb = FALSE;
     if(anp != NULL)
     {
        for(asp = anp->segs; asp != NULL; asp = asp->next) {
           if(asp->cnp != NULL) {
              featb = TRUE;
              break;
           }
        }
     }
     if (featb)
        return TRUE;
  }
  return FALSE;
}

extern void ShowFeatureProc (PaneL pnl, Boolean invalidate) 
{
  WindoW             temport;
  SeqEditViewFormPtr wdp;
  EditAlignDataPtr   adp;
  Boolean            ok;
  float              hratio;

  wdp = (SeqEditViewFormPtr)GetObjectExtra (ParentWindow(pnl));
  if ( wdp == NULL ) return;
  if ( ( adp = GetAlignDataPanel (pnl) ) == NULL ) return;
  if ( adp->seqnumber == 0 ) return;
  hratio = (float)adp->hoffset / (float)adp->length;
  adp->showfeat = (!adp->showfeat);
  ResetClip ();
  WatchCursor ();
  if ( adp->showfeat ) {
        ok = TRUE;
        if (!anpp_has_feature (adp->anp_list)) 
           ok = (Boolean) ShowFeatureFunc (adp);
        if (ok) {
           Enable (wdp->hidefeatitem);
           Disable(wdp->showfeatitem);
           SetTitle (wdp->showfeatbt, "Hide Feat.");
        }
  }
  else  {
        ok = TRUE;
        if (anpp_has_feature (adp->anp_list)) 
           ok = (Boolean) HideFeatureFunc (adp);
        if (ok) {
           Disable (wdp->hidefeatitem);
           Enable(wdp->showfeatitem);
           SetTitle (wdp->showfeatbt, "Show Feat.");
        }
  }
  if (ok) {

     data_collect_arrange (adp, TRUE);
     SeqEdSetCorrectBarMax (pnl, adp, hratio);
     if (invalidate) {
        temport = SavePort(ParentWindow(pnl));
        Select (pnl);
        inval_panel (pnl, -1, -1);
        RestorePort (temport);
     }
  }
  ArrowCursor ();
}

/******************************************************
***
***   LaunchCDSEditor on a Bioseq (input_itemID)
***
*******************************************************/
static void LaunchCDSEditor (Uint2 input_entityID, Uint2 input_itemID, SeqLocPtr slp, Uint1 codonstart)
{
  WindoW         w;
  SeqEntryPtr    top_sep;
  FeatureFormPtr cfp;

  if (slp != NULL && input_entityID != 0) 
  {
     top_sep = GetTopSeqEntryForEntityID (input_entityID);
     input_entityID = SeqMgrGetEntityIDForSeqEntry (top_sep);
   
     w = (WindoW) CreateCdRgnForm (-50, -33, "Coding Region", NULL, top_sep, CdRgnFeatFormActnProc);
     cfp = (FeatureFormPtr) GetObjectExtra (w);
     if (cfp != NULL) {
        cfp->input_entityID = input_entityID;
        cfp->input_itemID = input_itemID;
        cfp->input_itemtype = OBJ_BIOSEQ;
        cfp->this_itemtype = OBJ_SEQFEAT;
        cfp->this_subtype = FEATDEF_CDS;
        PointerToForm (cfp->form, NULL);
        SendMessageToForm (cfp->form, VIB_MSG_INIT);
        PointerToDialog (cfp->location, (Pointer) slp);
        CdRgnTranslateWithFrame (cfp->form, 1);
     }
     Show (w);
     Select (w);
  }
  return;
}
/***************************************************************
***  slpfeatreplacefunc
***
*****************************************************************/
static Boolean slpfeatreplacefunc(GatherContextPtr gcp)
{
  SeqFeatPtr sfp;
  SeqLocPtr  slp;
  Boolean    p3, p5; 

  if(gcp->thistype != OBJ_SEQFEAT) 
     return FALSE;
  sfp = (SeqFeatPtr)(gcp->thisitem);
  slp = (SeqLocPtr) gcp->userdata;
  CheckSeqLocForPartial (sfp->location, &p5, &p3);
  SetSeqLocPartial (slp, p5, p3);
  sfp->location = SeqLocFree (sfp->location);  
  sfp->location = slp;
  return TRUE;
}

/******************************************************************
***
*** SaveFeatProc 
***     look at the selected items
***     if new feature, attaches it (AttachDataForProc)
***     other, replaces it (GatherItem)
***     sends a message to ObjMgr (ObjMgrSendMsg (OM_MSG_UPDATE..))
***     write the seqenrty in the temporary file
***
*** SaveFeatureProc, SaveFeatureButton : call SaveFeatProc
***     sends a message to ObjMgr (ObjMgrSendMsg (OM_MSG_UPDATE..))
***
*******************************************************************/
extern void SaveFeatProc (PaneL pnl)
{
  EditAlignDataPtr   adp;
  SeqEntryPtr        sep;
  SelStructPtr       ssp = NULL;
  SelEdStructPtr     feat;
  ValNodePtr         vnp,
                     next;
  SeqLocPtr          slp;
  RecT               rp;
  Int2               width;
  Uint2              bsp_eID, bsp_iID;
  Int2               handled;

  if ( ( adp = GetAlignDataPanel (pnl) ) == NULL ) return;
  if ( adp->seqnumber == 0 ) return;
  if ( checkOMss_for_itemtype (OBJ_VIRT) == 0 
    && checkOMss_for_itemtype (OBJ_SEQFEAT) == 0 ) return;
  sep = GetBestTopParentForItemID (adp->master.entityID, adp->master.itemID, adp->master.itemtype);
  if (sep == NULL)
     return;
  ssp = ObjMgrGetSelected(); 
  for (; ssp != NULL; ssp = ssp->next)
  {  
     if ( checkssp_for_editor (ssp) && ssp->itemtype == OBJ_VIRT) 
     {
        vnp = adp->feat; 
        while  (vnp != NULL) {
           next = vnp->next;
           if (vnp->choice == SEQFEAT_CDREGION) {
              feat = (SelEdStructPtr) vnp->data.ptrvalue;
              if ( is_samess_ses (ssp, feat) ) 
              {
                 if (SeqLocStart((SeqLocPtr)feat->region)==0 || feat->codonstart == 1) 
                 {
                    adp->curfeat = feat;
                    slp = sesp_to_slp (feat, (SeqAlignPtr) adp->sap_align->data, adp->sqloc_list, FALSE);
                    bsp_eID = SeqMgrGetEntityIDForSeqEntry (sep);
                    bsp_iID = feat->bsp_itemID; 
                    LaunchCDSEditor (bsp_eID, bsp_iID, slp, feat->codonstart);
                 }
                 else 
                    ErrPostEx (SEV_ERROR, 0, 0, "Codon start must be 1");
                 break;
              }
           }
        }
        vnp = next;
     }
     else if ( checkssp_for_editor (ssp) && ssp->itemtype == OBJ_SEQFEAT) 
     {
        vnp = adp->seqfeat; 
        while  (vnp != NULL) 
        {
           next = vnp->next;
           if (vnp->choice == SEQFEAT_CDREGION
            || vnp->choice == SEQFEAT_GENE || vnp->choice == SEQFEAT_RNA) 
           {
              feat = (SelEdStructPtr) vnp->data.ptrvalue;
              if ( is_samess_ses (ssp, feat) ) 
              {
                 slp = sesp_to_slp (feat,(SeqAlignPtr) adp->sap_align->data, adp->sqloc_list, FALSE);
                 if (slp != NULL) 
                 {
                    bsp_eID = feat->entityID;
                    bsp_iID = feat->itemID;
                    GatherItem (bsp_eID, bsp_iID, OBJ_SEQFEAT, (Pointer)(slp), slpfeatreplacefunc);
                    ObjMgrSendMsg (OM_MSG_UPDATE, adp->master.entityID,  adp->master.itemID, adp->master.itemtype);
                    HideFeatureFunc (adp);
                    adp->showfeat = FALSE;
                    ShowFeatureProc(pnl, FALSE);
                    get_client_rect (pnl, &rp);
                    width = adp->visibleWidth;
                    if (adp->columnpcell > 0)
                      width +=(Int2)adp->visibleWidth/(Int2) adp->columnpcell;
                    inval_all (adp, &rp, (Uint2)255, OBJ_VIRT, OBJ_SEQFEAT, width);
 
                    WatchCursor ();
                    Update ();
                    handled = GatherProcLaunch (OMPROC_EDIT, FALSE, bsp_eID, bsp_iID, OBJ_SEQFEAT, 0, 0, OBJ_SEQFEAT, 0);
                    ArrowCursor ();
                    Update ();
                 }
                 break;
              }
           }
           vnp = next;
        }
        seqentry_write (sep, adp->tmpfile);
     }
  }
  adp->dirty = TRUE;
  if (!adp->showfeat) {
     ShowFeatureProc(pnl, TRUE);
  }
  return;
}

/******************************************************************
***
*** SaveAllFeatProc 
***     looks at the features items
***     attaches the new features (AttachDataForProc)
***     replaces the old features (GatherItem)
***
OLD VERSION:

  OMProcControl      ompc;
  EditAlignDataPtr   adp;
  SelEdStructPtr     feat;
  ValNodePtr         vnp;
  SeqLocPtr          slp;

  if ( ( adp = GetAlignDataPanel (pnl) ) == NULL ) return;
  if ( adp->seqnumber == 0 ) return;

  MemSet ((Pointer) &ompc, 0, sizeof (OMProcControl));
  ompc.input_entityID = adp->master.entityID;
  ompc.input_itemID = adp->master.itemID;
  ompc.input_itemtype = adp->master.itemtype;
  ompc.output_itemtype = OBJ_SEQFEAT;

  for  (vnp=adp->seqfeat; vnp != NULL; vnp = vnp->next) 
  {
     if (vnp->choice == SEQFEAT_CDREGION
      || vnp->choice == SEQFEAT_GENE || vnp->choice == SEQFEAT_RNA) {
        feat = (SelEdStructPtr) vnp->data.ptrvalue;
        slp = sesp_to_slp (feat, (SeqAlignPtr) adp->sap_align->data, FALSE);
        if (slp != NULL) {
           GatherItem (feat->entityID, feat->itemID, OBJ_SEQFEAT,
                      (Pointer)(slp), slpfeatreplacefunc);
       }
     }
  }
*******************************************************************/
extern void SaveAllFeatProc (PaneL pnl)
{
  return;
}


static void MakeFeatFunc (EditAlignDataPtr adp, SelStructPtr ssp, Uint2 itemsubtype, Uint1 strand)
{
  SelEdStructPtr   feat = NULL;
  SelStructPtr     ssptmp;
  SeqLocPtr        slp;
  SeqIntPtr        sit;
  SeqLocPtr        slpfeat;
  Int4             from, to;
  Uint2            itemID;

  slp = (SeqLocPtr) ssp->region;
  ssptmp = is_selectedbyID (ssp->entityID, 255, OBJ_VIRT);
  if (ssptmp == NULL) {
         adp->nfeat++;
         itemID = adp->nfeat;
  } else {
         itemID = ssptmp->itemID;
  }
  from = SeqLocStart (slp);
  to = SeqLocStop (slp);
  if (to == APPEND_RESIDUE) {
     slpfeat = ValNodeNew (NULL);
     slpfeat->choice = SEQLOC_WHOLE;
     slpfeat->data.ptrvalue = (Pointer) SeqIdDup (SeqLocId(slp));
     to = SeqLocLen(slpfeat) -1;
     SeqLocFree (slpfeat);
  }
  if (from >= 0 && to > 0) {
  sit = (SeqIntPtr) slp->data.ptrvalue;
  sit->strand = strand;
  feat = ss_to_ses (ssp); 
  slpfeat = (SeqLocPtr)feat->region;
  setposition_toses (feat, from, to);

  /*  feat->entityID !!!!!!!!!!!!!!!!!!!!!*/
  feat->bsp_itemID = feat->itemID;
  feat->itemID = itemID;
  feat->itemtype = OBJ_VIRT;
  feat->codonstart = 1;
  feat->offset = 0;
  feat->dirty = TRUE;
  feat->next = NULL;
  feat->prev = NULL;
  adp->feat = AddFeatFunc (feat, &(adp->feat), itemsubtype);
  }
}

extern void MakeFeatProc (PaneL pnl, Uint2 itemsubtype, Uint1 strand)
{
  WindoW           temport;
  EditAlignDataPtr adp;
  SelStructPtr     ssp;
  float hratio;

  if ( ( adp = GetAlignDataPanel (pnl) ) != NULL ) {
     if (adp->seqnumber > 0 || ISA_na(adp->mol_type)) {
        hratio = (float)adp->hoffset / (float)adp->length;
        ssp = ObjMgrGetSelected();  
        for (; ssp != NULL; ssp = ssp->next) 
        {
           if (checkssp_for_editor (ssp) && ssp->itemtype == OBJ_BIOSEQ ) {
              MakeFeatFunc (adp, ssp, itemsubtype, strand);
           }
        }
        data_collect_arrange (adp, TRUE);
        SeqEdSetCorrectBarMax (pnl, adp, hratio);
        adp->dirty = TRUE;
        temport = SavePort(ParentWindow(pnl));
        Select (pnl);
        inval_panel (pnl, -1, -1);
        RestorePort (temport);
     }
  }
  return; 
}

extern void TranslateAllBioseq (PaneL pnl,  EditAlignDataPtr adp)
{
  WindoW           temport;
  SelEdStructPtr   cds;
  AlignNodePtr     anp;
  SelStructPtr     ssp;
  SeqLocPtr        slp;
  ValNodePtr       vnp = NULL;
  float  hratio;

  hratio = (float)adp->hoffset / (float)adp->length;
  if (adp->seqnumber > 0 || ISA_na(adp->mol_type)) {
     for (vnp = adp->anp_list; vnp != NULL; vnp = vnp->next) {
        if ( (anp = (AlignNodePtr) vnp->data.ptrvalue) != NULL)
        {
           slp = CollectSeqLocFromAlignNode(anp);
           if (slp!=NULL) {
              ssp = SelStructNew (anp->seq_entityID, anp->bsp_itemID, OBJ_BIOSEQ, SeqLocStart(slp), SeqLocStop(slp), SeqLocId(slp), SeqLocStrand(slp), FALSE);
              if (ssp!=NULL)
                 MakeFeatFunc (adp, ssp, SEQFEAT_CDREGION, Seq_strand_plus);
           }
        }
     }
     vnp = adp->feat;
     if (vnp != NULL)
     {   
        for (; vnp != NULL; vnp = vnp->next)
        {
           if (vnp->choice == FEATDEF_CDS) {
              cds = (SelEdStructPtr) vnp->data.ptrvalue;
              sesp_to_pept(cds, (SeqAlignPtr) adp->sap_align->data, adp->sqloc_list, TRUE); 
           }   
        }
        data_collect_arrange (adp, TRUE);
        SeqEdSetCorrectBarMax (pnl, adp, hratio);
        temport = SavePort(ParentWindow(pnl));
        Select (pnl);
        inval_panel (pnl, -1, -1);
        RestorePort (temport);
     }   
  }
  return;
}
/**************************************/
/**************************************/
/**************************************/
/**************************************/
/**************************************/

static void FindBioseqCB3 (SeqEntryPtr sep, Pointer mydata,
                                          Int4 index, Int2 indent)
{
  ValNodePtr  vnp;

  if (sep != NULL && sep->data.ptrvalue && mydata != NULL) {
     if (IS_Bioseq(sep)) {
        vnp = (ValNodePtr)mydata;
        if (vnp->data.ptrvalue==NULL)
        {
           vnp->data.ptrvalue=(BioseqPtr) sep->data.ptrvalue;
        }
     }
  }
}
 
static SelEdStructPtr is_sip_inseqinfo (SeqIdPtr sip, SelEdStructPtr seq_info)
{
  SelEdStructPtr tmp;
  SeqLocPtr      slp;

  for (tmp=seq_info; tmp!=NULL; tmp=tmp->next)
  {
     slp=(SeqLocPtr)tmp->region;
     if (SeqIdForSameBioseq (sip, SeqLocId(slp)))
     {
        return tmp;
     }
  }
  return NULL;
}

extern ValNodePtr CCReadAnythingLoop (CharPtr filename, SelEdStructPtr seq_info)
{
  Char         name [PATH_MAX];
  Pointer      dataptr;
  FILE         *fp;
  BioseqPtr    bsp;
  SeqEntryPtr  sep;
  SeqLocPtr    slp;
  ValNodePtr   head = NULL,
               vnp,
               slphead = NULL;
  Uint2        datatype; 
  Uint2        entityID;

  ValNode      vn;

SelEdStructPtr tmp;

  if (filename == NULL) 
  {
     if (! GetInputFileName (name, PATH_MAX,"","TEXT"))  {
        return NULL;
     }
     filename = name;
  }
  fp = FileOpen (filename, "r");
  if (fp != NULL) {
    while ((dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, NULL, FALSE, FALSE, TRUE, FALSE)) != NULL) {
      ValNodeAddPointer (&head, datatype, dataptr);
    }
    FileClose (fp);
    for (vnp = head; vnp != NULL; vnp = vnp->next) {
      datatype = vnp->choice;
      dataptr = vnp->data.ptrvalue;
      entityID = ObjMgrRegister (datatype, dataptr);
      if (datatype == OBJ_BIOSEQ)
      {
         bsp=(BioseqPtr)vnp->data.ptrvalue;
         slp = SeqLocIntNew (0, bsp->length-1, Seq_strand_plus, SeqIdFindBest (bsp->id, 0));

if (seq_info) {
 if ((tmp=is_sip_inseqinfo(bsp->id, seq_info)) != NULL) {
   sep=SeqEntryNew ();
   sep->choice=1;
   sep->data.ptrvalue = (Pointer)bsp; 
   SeqEntryReplaceSeqID (sep, SeqLocId(slp));
   ValNodeFree(slp);
   slp = SeqLocIntNew(0, bsp->length-1, Seq_strand_plus, bsp->id);
   sep->data.ptrvalue = NULL;
   SeqEntryFree(sep);
 }
}
         ValNodeAddPointer (&slphead, 0, (Pointer) slp);
      }
      else if (datatype == OBJ_SEQENTRY)
      {
         sep=(SeqEntryPtr)vnp->data.ptrvalue;
         vn.data.ptrvalue=NULL;
         SeqEntryExplore (sep, &vn, FindBioseqCB3);
         if (vn.data.ptrvalue!=NULL) {
            bsp=(BioseqPtr)vn.data.ptrvalue;
            slp = SeqLocIntNew (0, bsp->length-1, Seq_strand_plus, SeqIdFindBest (bsp->id, 0));

if (seq_info) {
 if ((tmp=is_sip_inseqinfo(bsp->id, seq_info)) != NULL) {
   SeqEntryReplaceSeqID (sep, SeqLocId(slp));
   ValNodeFree(slp);
   slp = SeqLocIntNew(0, bsp->length-1, Seq_strand_plus, bsp->id);
 }
}
            ValNodeAddPointer (&slphead, 0, (Pointer) slp);
         }
      }
    }
  }
  return slphead;
}







/*******************************************************
*** AsnReadForSalsa
***   copied from Jonathan's code
***   without the following lines:
***
          rsult = SeqEntryNew ();
          if (rsult != NULL) {
            rsult->choice = sep->choice;
            rsult->data.ptrvalue = sep->data.ptrvalue;
            sep->data.ptrvalue = NULL;
            if (datatype == OBJ_SEQSUB) {
              SeqSubmitFree ((SeqSubmitPtr) dataptr);
            } else {
              SeqEntryFree (sep);
            }
            if (!ObjMgrRegister (OBJ_SEQENTRY, (Pointer) rsult))
               rsult = SeqEntryFree (rsult);
          }  
***********************************************************/
extern SeqEntryPtr AsnReadForSalsa (CharPtr path)
{
  Char         name[PATH_MAX];
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Pointer       dataptr;
  Uint2         datatype;
  Uint2         entityID;
  SeqEntryPtr   rsult;
  SeqEntryPtr   sep;
  Boolean       delete_sep=FALSE;

  rsult = NULL;
  if (path == NULL) {
     if (! GetInputFileName (name, PATH_MAX,"","TEXT"))  {
        return NULL;
     }
     path = name;
  }
  if (path != NULL && path [0] != '\0') {
    dataptr = ObjMgrGenericAsnTextFileRead (path, &datatype, &entityID);
    if (dataptr != NULL && entityID > 0) {
      if (datatype == OBJ_SEQSUB || datatype == OBJ_SEQENTRY ||
          datatype == OBJ_BIOSEQ || datatype == OBJ_BIOSEQSET) {
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
            }
          }  
          sep = GetTopSeqEntryForEntityID (entityID);
          delete_sep=TRUE;
        }
        if (sep != NULL) {
           rsult = (SeqEntryPtr) AsnIoMemCopy((Pointer) sep, (AsnReadFunc)SeqEntryAsnRead, (AsnWriteFunc)SeqEntryAsnWrite);
           if (delete_sep) {
              sep->data.ptrvalue = NULL;
              sep = SeqEntryFree (sep);
           }
        }
      }
      ObjMgrFree (datatype, (Pointer) dataptr);
    }
  }
  return rsult;
}

extern SeqEntryPtr seqentry_read (CharPtr path)
{
  Char         name[PATH_MAX];
  AsnIoPtr     aip;
  AsnTypePtr   atp;
  AsnModulePtr amp;
  SeqEntryPtr  sep;

  if (path == NULL )
  {
     if (! GetInputFileName (name, PATH_MAX,"","TEXT"))  {
        return NULL;
     }
     path = name;
  }
  amp = AsnAllModPtr ();
  atp = AsnTypeFind (amp,"SeqEntry");
  if ((aip = AsnIoOpen (path,"r")) == NULL) {
    return NULL;
  }
  sep = SeqEntryAsnRead ( aip, atp );
  aip = AsnIoClose (aip);
  return sep;
}





