/*   salutil.c
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
* File Name:  salutil.c
*
* Author:  Colombe Chappey , Hugues Sicotte
*
* Version Creation Date:   1/27/96
*
* $Revision: 6.9 $
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

#include <ncbi.h>
#include <salutil.h>
#include <salstruc.h>
#include <salsap.h>
#include <salpacc.h>
#include <salpedit.h>
#include <edutil.h>
#include <sequtil.h>
#include <sqnutils.h>

#ifdef SALSA_DEBUG
#include <simutil.h>
#include <toasn3.h>
#include <utilpub.h>
#include <tfuns.h>
#endif

#define OBJ_VIRT 254

#define BAND_LIMIT 100

static Uint2 OBJ_ (Uint2 feattype)
{
  if ( feattype == FEATDEF_BAD ) return OBJ_BIOSEQ;
  return OBJ_SEQFEAT;
}

/****************************************************
***   Read SeqPort.bsp from SeqPort.start to stop
***   in : spp, from + to in seq coordinates
***   out: length of buffer + buffer
***        compiled with -D SAP
*****************************************************/
NLM_EXTERN Int4 ReadBufferFromSep (SeqPortPtr spp, CharPtr buffer, Int4 from, Int4 to, Int4 buffsegstart)
{
  Uint1    residue;
  Int4     k;
  Int4     pos;

  SeqPortSeek (spp, from, SEEK_SET);
  k = buffsegstart;
  pos = from;
  residue = SeqPortGetResidue(spp);
  while (pos < to && residue != SEQPORT_EOF)
  {
    if ( ! IS_residue(residue)) {
      /*
      switch (residue)
      {  
           case SEQPORT_VIRT:
              Message(MSG_OK,"SEQPORT_VIRT [%d=%c] at %ld\n", (int)residue, (char)residue, (long)pos);
              break;
           case SEQPORT_EOS:
              Message(MSG_OK,"[EOS]\n");
              break;
           default:
              Message(MSG_OK,"unknown char\n");
              break;
      }  
      */
      pos++;
    } else {
      buffer[k] = (Char) residue;
      k++;  
      pos++;
    }
    residue = SeqPortGetResidue(spp);
  }
  buffer[k] = '\0';
  return k;
}
 

/*****************************************************************************
***  ReadBufferFromSap 
******************************************************************************/
NLM_EXTERN CharPtr ReadBufferFromSap (CharPtr str, CharPtr buffer, SeqAlignPtr salp, SeqIdPtr sip, Int4 from, Int4 to, Int4 *ncar)
{
  CompSegPtr  dsp;
  BoolPtr     dspstart;
  Int4Ptr     dsplens;
  Int4        dspfrom;
  Int4        sumstart;
  Int4        bufflen, buffstart;
  Int4        pre_from;
  Int4        bsplens;
  Int4        ibuff;
  Int4        istr;
  Int4        j = 0;
  Int4        k = 0;
  Int2        dim;
  Int2        numseg;
  Int2        index;
  Boolean     seen = FALSE;
  Boolean     nogap;
  Boolean     ok;

  *ncar = 0;
  if (buffer == NULL || salp == NULL) {
         ErrPostEx (SEV_ERROR, 0, 0, "fail in ReadBufferFromSap [1]\n");
         return NULL;
  }
  if ( salp->segtype != COMPSEG ) {
         ErrPostEx (SEV_ERROR, 0, 0, "fatal fail in ReadBufferFromSap"); 
         return NULL; 
  }
  if ( (dsp = (CompSegPtr) salp->segs ) == NULL) {
         ErrPostEx (SEV_ERROR, 0, 0, "fail in ReadBufferFromSap [3-2]\n");
         return NULL;
  }
  if (dsp->from == NULL || dsp->starts == NULL || dsp->lens == NULL) {
         ErrPostEx (SEV_ERROR, 0, 0, "fail in ReadBufferFromSap [3-3]\n");
         return NULL;
  }
  index = SeqIdOrderInBioseqIdList (sip, dsp->ids);
  if (index < 1) {
         ErrPostEx (SEV_ERROR, 0, 0, "fail in ReadBufferFromSap [3-4]");
         return NULL;
  }
  index -=1;
  dim = dsp->dim;
  dspfrom = *(dsp->from + index);
  dspstart = dsp->starts + index;
  dsplens = dsp->lens;
  seen = LocateInSeqAlign (from, dim, dsp->numseg, &dspstart, &dsplens, &numseg, 
                           &pre_from, &bsplens);
  if (!seen) {
         ErrPostEx (SEV_ERROR, 0, 0, "fail in ReadBufferFromSap [4]\n");
         return NULL;
  }
  ibuff = 0;
  istr = 0;
  ok = (ibuff < to - from);
  if (!ok) {
        ErrPostEx (SEV_ERROR, 0, 0, "fail in ReadBufferFromSap [6] %ld %ld %ld\n", (long) ibuff, (long) to, (long) from);
        return NULL;
  }
  nogap = (Boolean)( *dspstart );
  if ( nogap )
    buffstart= (Int4)(dspfrom +bsplens +pre_from);
  else  buffstart = 0;
  bufflen = (Int4)MIN((Int4)(*dsplens -pre_from),(Int4)(to-from +1));
  sumstart = 0;
  while ( ok ) 
  {
     if ( nogap ) 
     {
        for (j = 0; j < bufflen; j++, ibuff++, istr++) {
           buffer[ibuff] = str [istr];
           k++;
        }
        buffer[ibuff] = '\0';
        bsplens += *dsplens;
     } 
     else 
     {
        for (j = 0; j < bufflen; j++, ibuff++) {
           buffer[ibuff] = '-';
        }
        buffer[ibuff] = '\0';
     }
     numseg++;
     if (numseg > dsp->numseg) break;
     dspstart += dim; 
     dsplens++;
     nogap = (Boolean)(*dspstart);
     if ( nogap )
        buffstart = (Int4) (dspfrom +bsplens +pre_from);
     else buffstart = 0;
     bufflen = (Int4) MIN ((Int4)(*dsplens), (Int4) (to -from -ibuff +1));
  }
  *ncar = k;
  return buffer;
}


/*------------------------------------------
--  String Functions
--------------------------------------------*/
NLM_EXTERN Boolean CCStrToInt (CharPtr str, Int2Ptr intval)
{
  Char     ch;
  Int2     i;
  Int2     len;
  Char     local [64];
  Boolean  nodigits;
  Boolean  rsult;
  int      val;
 
  rsult = FALSE;
  if (intval != NULL) {
    *intval = (Int2) 0;
  }
  len = (Int2) StringLen (str);
  if (len != 0) {
    rsult = TRUE;
    nodigits = TRUE;
    for (i = 0; i < len; i++) {
      ch = str [i];
      if (ch == ' ' || ch == '+' || ch == '-') {
        rsult = rsult;
      } else if (ch < '0' || ch > '9') {
        rsult = FALSE;
      } else {
        nodigits = FALSE;
      }
    }
    if (nodigits) {
      rsult = FALSE;
    }
    if (rsult && intval != NULL) {
      StringNCpy_0 (local, str, sizeof (local));
      if (sscanf (local, "%d", &val) == 1) {
        *intval = (Int2) val;
      }
    }
  }
  return rsult;
}
 
NLM_EXTERN Boolean CCStrToLong (CharPtr str, Int4Ptr longval)
{
  Char     ch;
  Int2     i;
  Int2     len;
  Char     local [64];
  Boolean  nodigits;
  Boolean  rsult;
  long     val;
 
  rsult = FALSE;
  if (longval != NULL) {
    *longval = (Int4) 0;
  }
  len = (Int2) StringLen (str);
  if (len != 0) {
    rsult = TRUE;
    nodigits = TRUE;
    for (i = 0; i < len; i++) {
      ch = str [i];
      if (ch == ' ' || ch == '+' || ch == '-') {
        rsult = rsult;
      } else if (ch < '0' || ch > '9') {
        rsult = FALSE;
      } else {
        nodigits = FALSE;
      }
    }
    if (nodigits) {
      rsult = FALSE;
    }
    if (rsult && longval != NULL) {
      StringNCpy_0 (local, str, sizeof (local));
      if (sscanf (local, "%ld", &val) == 1) {
        *longval = (Int4) val;
      }
    }
  }
  return rsult;
}

NLM_EXTERN CharPtr dashedstring (CharPtr buffer, Int4 maxbufflength)
{
  Int4 j;

  for (j = 0; j < maxbufflength; j++) 
     buffer[j] = '-';
  buffer[0] = '\0';
  return buffer;
}

NLM_EXTERN CharPtr emptystring (CharPtr buffer, Int4 maxbufflength)
{
  Int4 j;

  if (buffer == NULL)
     return NULL;
  for (j = 0; j < maxbufflength; j++) buffer[j] = ' ';
  buffer[0] = '\0';
  return buffer;
}

NLM_EXTERN Boolean not_empty_string (CharPtr str, Int4 lens)
{
  CharPtr  strtmp;
  Int4     j = 0;

  if (str == NULL) return FALSE;
  strtmp = str;
  for (; *strtmp != '\0' && *strtmp == ' ' && j < lens; j++, strtmp++ ) 
         continue;
  if ( j == lens || *strtmp == '\0' ) return FALSE;
  return TRUE;
}

NLM_EXTERN Boolean stringhasnochar (CharPtr str, Int4 from, Int4 to)
{
  Char  ch;
 
  if (str != NULL) {
     if (from <= -1) 
        from = 0;
     if (to <= (Int4)-1 || to > (Int4)StringLen(str)) 
        to = StringLen(str);
     if (from < to && from < (Int4)StringLen(str)) {
        ch = TO_UPPER(str[from]);
        while (ch != '\0' && from < to) 
        {
           if (ch >= 'A' && ch <= 'Z') {
              return FALSE;
           }
           from++;
           ch = TO_UPPER(str[from]);
        }
     }
  }
  return TRUE;
}  

NLM_EXTERN Int1 getgapsfromstring (CharPtr str, Int4 from, Int4 to, BoolPtr *gapline)
{
  Char    ch;
  BoolPtr boolgap;
  Int4    nchar = 0; 
  Int4    len;

  if (str != NULL) {
     boolgap = *gapline;
     if (from <= -1) 
        from = 0;
     if (to <= (Int4)-1 || to > (Int4)StringLen(str)) 
        to = StringLen(str);
     else 
        to = MIN (to, (Int4)StringLen(str));
     len = (to-from);
     if (from < to) {
        ch = TO_UPPER(str[from]);
        while (ch != '\0' && from < to) 
        {
           if (ch >= 'A' && ch <= 'Z') {
              nchar++;
              *boolgap = FALSE;
           }
           else 
              *boolgap = TRUE;
           from++;
           ch = TO_UPPER(str[from]);
           boolgap++;
        }
        if (nchar == 0)
           return LINE_ONLYGAP;
        if (nchar < len)
           return LINE_WITHGAP;
        return LINE_NOGAP;
     }
  }
  return LINE_ONLYGAP;
}  


NLM_EXTERN Boolean stringhasnocharplus (CharPtr str)
{
  Char  ch;
 
  if (str != NULL) {
    ch = TO_UPPER(*str);
    while (ch != '\0') {
      if ((ch >= 'A' && ch <= 'Z') || ch == '-') {
        return FALSE;
      }
      str++;
      ch = TO_UPPER(*str);
    }
  }
  return TRUE;
}  

NLM_EXTERN CharPtr purge_string (CharPtr *st)
{
  CharPtr str;
  Int4    i, j, k, n;
  Int4    lens;

  str = *st;
  if (str==NULL)
     return NULL;
  lens = StringLen (str);
  for (i =0; i <lens; i++) {
         if (str[i] == ' ') 
         {
                j =1;
                while (str[i+j] == ' ') j++;
                lens--;
                for (k =j, n =0; k <lens; k++, n++) {
                       str[i+n] = str[k];
                }
         }
  }
  str[lens] = '\0';
  return str;
}

NLM_EXTERN CharPtr reverse_string (CharPtr str)
{
  Char    car;
  Int4    length;
  Int4    j;

  if (str==NULL)
     return NULL;
  length = StringLen (str);
  for (j = 0; j < length / 2; j++)
  {
     car = str[j]; str[j] = str[length-j-1]; str[length-j-1] = car;
  }
  return str;
}

NLM_EXTERN CharPtr to_lower (CharPtr str)
{
  CharPtr tmp;
 
  if (str==NULL)
     return NULL;
  tmp = str;
  while (*tmp!='\n' && *tmp!='\0') {
     *tmp = TO_LOWER(*tmp);
     tmp++;
  }
  return str;
}

/******************************************************************
****      complement_string
****                
*******************************************************************/
NLM_EXTERN CharPtr complement_string (CharPtr str)
{
  CharPtr strp;

  for (strp = str; *strp != '\0'; strp++) {
         if (*strp == 'a') *strp = 't';
         else if (*strp == 't') *strp = 'a';
         else if (*strp == 'c') *strp = 'g';
         else if (*strp == 'g') *strp = 'c';
  }
  *strp = '\0';
  return str;
}

/*************************************************************
***
*** compare_string
***    compare 2 string by sliding in ONE direction only
***    str2 slides along str1
***    returns the position where the PERCENT of matches and
***    sequence length compared are greater
***
***************************************************************/
NLM_EXTERN Int4 compare_string (CharPtr str1, CharPtr str2, Int4 *bestscorep)
{
  Int4  len1, len2;
  Int4  length = 0;
  Int4  score = 0;
  Int4  ratio;
  Int4  longer = 0;
  Int4  best_ratio = 0;
  Int4  best_pos = -1;
  Int4  j, k;

  if (str1 == NULL || str2 == NULL)
     return -1;
  len1 = StringLen (str1);
  len2 = StringLen (str2);
  for (j=0; j<len1; j++) {
     length = 0;
     score = 0;
     for (k=0; k<MIN(len2, len1-j); k++) {
        length ++;
        if (str1[j + k] == str2[k])
           score ++;
     }   
     ratio = (Int4) (100*score/length);
     if (ratio > best_ratio || (ratio == best_ratio && length > longer)) 
     {
        best_ratio = ratio;
        longer = length;
        best_pos = j;
     }
  }
  if (bestscorep != NULL) {
     *bestscorep = best_ratio;
  }
  return best_pos;
}  

/*************************************************************
***
*** load_seq_data
***    loads bioseq sequence into a string
***    sip: SeqId of the bioseq
***    from, to: included bondaries 
***    returns the length of the string (lenp)
***
***************************************************************/
NLM_EXTERN CharPtr load_seq_data (SeqIdPtr sip, Int4 from, Int4 to, Boolean is_prot, Int4 *lenp)
{
  BioseqPtr        bsp;
  SeqLocPtr        slp;
  SeqPortPtr       spp;
  CharPtr          str = NULL;
  Int4             lens;

  if (from > -1 && to > -1 && from >= to)
     return NULL;
  bsp = BioseqLockById (sip);
  if (bsp != NULL) {
     if (from < 0 || from > bsp->length -1)
        from = 0;
     if (to < 0 || to > bsp->length -1)
        to = bsp->length -1;
     BioseqUnlock (bsp);
     if (from < to)
        slp = SeqLocIntNew (from, to, Seq_strand_plus, sip);
     else 
        slp = SeqLocIntNew (to, from, Seq_strand_minus, sip);
     if (is_prot)
        spp = SeqPortNewByLoc (slp, Seq_code_ncbieaa);
     else
        spp = SeqPortNewByLoc (slp, Seq_code_iupacna);
     if (spp != NULL) {
        str = (CharPtr)MemNew ((to-from+4) * sizeof(Char));
        lens = ReadBufferFromSep (spp, str, 0, to -from +1, 0);
        SeqPortFree (spp);
        if (lenp != NULL)
           *lenp = lens;
     }   
     SeqLocFree (slp);
  }
  return str;
}


NLM_EXTERN Boolean IS_protSeqLoc (SeqLocPtr slp)
{
  CharPtr seq;
  Int4 len;

  seq = load_seq_data (SeqLocId(slp), SeqLocStart(slp), SeqLocStop(slp), TRUE, &len);
  return FALSE;
}

/*************************************************************
***
***  StringToSeqEntry :
***    in:  sequence (CharPtr) + name + length of the alignment
***    out: SeqEntryPtr
***
***************************************************************/
NLM_EXTERN SeqEntryPtr StringToSeqEntry (CharPtr str, SeqIdPtr sip, Int4 length_align, Uint1 mol_type)
{
  SeqEntryPtr  sep;
  BioseqPtr    bsp;
  ByteStorePtr bs;
  Char         ch;

  if (str == NULL || sip == NULL) return NULL;
  sep = SeqEntryNew ();
  if (sep == NULL) return NULL;
  bsp = BioseqNew ();
  if (bsp == NULL) { 
         ValNodeFree (sep); 
         return NULL; 
  }
  sep->choice = 1;
  sep->data.ptrvalue = (Pointer) bsp;
  bsp->id = SeqIdDup (sip);
  bsp->id->next = NULL;
  SeqMgrAddToBioseqIndex (bsp);
  bsp->repr = Seq_repr_raw;
  if ( ISA_na (mol_type) ) {
         bsp->mol = Seq_mol_na;
         bsp->seq_data_type = Seq_code_iupacna;
  } 
  else {
         bsp->mol = Seq_mol_aa;
         bsp->seq_data_type = Seq_code_ncbieaa;
  }
  bsp->length = 0;
  bs = BSNew (length_align);
  bsp->seq_data = bs;
  while ((ch = *str) != '\0' && ch != ';' && bsp->length < length_align) {   
         ch = TO_UPPER (ch); 
         if ( ISA_na (mol_type) ) {
                if (ch == 'U') ch = 'T';
                if (ch == 'X') ch = 'N';
                if ( StringChr ("EFIJLOPQXZ-.*", ch) == NULL ) { 
                       BSPutByte ( bs, (Int2) ch );
                       bsp->length++;
                }
         } 
         else {
                if ( StringChr("JO-.", ch) == NULL ) { 
                       BSPutByte ( bs, (Int2) ch );
                       bsp->length++;
                }
         } 
         str++;
  }
  if ( bsp->length == 0 ) {
         BioseqFree (bsp);
         ValNodeFree (sep);
         return NULL;
  }
  return sep;
}

/*******************************************
***
***
********************************************/
NLM_EXTERN ValNodePtr ValNodeFind (ValNodePtr head, Int2 start, Int2 index)
{
  Int2       j;

  if ( head == NULL ) return NULL;
  for (j = start; j < index && head != NULL; j++, head = head->next) 
          continue;
  return head;
}

NLM_EXTERN ValNodePtr ValNodeFreeType (ValNodePtr *head, Int2 seqtype)
{
  ValNodePtr   vnp;

  if ( ( vnp = *head ) == NULL ) return NULL;
  for ( ; vnp != NULL; vnp = vnp->next ) {
         switch ( seqtype ) {
           case TypeEmpty: { }
               break;
           case TypeSeqInt :
               SeqIntFree ((SeqIntPtr) vnp->data.ptrvalue);
               break;
           case TypeSeqId :
               SeqIdFree ((SeqIdPtr) vnp->data.ptrvalue);
               break;
           case TypeSeqLoc :
               SeqLocFree ((SeqLocPtr) vnp->data.ptrvalue);
               break;
           case TypeSelStruct :
               SelStructDel ((SelStructPtr) vnp->data.ptrvalue);
               break;
           case TypeSelEdStruct :
               SelEdStructDel ((SelEdStructPtr) vnp->data.ptrvalue);
               break;
           default:    break;
         }
         vnp->data.ptrvalue = NULL;
  }
  ValNodeFree (vnp);
  vnp = NULL;
  return NULL;
}

NLM_EXTERN SeqLocPtr seqloc2fuzzloc(SeqLocPtr slp, Boolean is_from, Boolean is_to)
{
   IntFuzzPtr fuzz;
   SeqIntPtr sint;

	   sint = (SeqIntPtr)slp->data.ptrvalue;
	   if(is_from){
	   	sint->if_from = IntFuzzNew();
	   	fuzz = sint->if_from;
	   	fuzz->choice = 4;
	   	fuzz->a =2;
	   }
	   if(is_to){
	     	sint->if_to = IntFuzzNew();
	     	fuzz = sint->if_to;
	     	fuzz->choice =4;
	     	fuzz->a =1;
	   }
	   return slp;
}

NLM_EXTERN TextAlignBufPtr TextAlignBufFind (ValNodePtr anpvnp, Uint2 entityID, Uint2 itemID, Uint2 itemtype)
{
  ValNodePtr      vnp;
  TextAlignBufPtr tap;
  Uint2           tentityID;

  if ( anpvnp == NULL ) return NULL;
  for (vnp = anpvnp; vnp != NULL; vnp = vnp->next) 
  {
         tap = (TextAlignBufPtr) vnp->data.ptrvalue; 
         if ( tap != NULL)
         {
            if (OBJ_(tap->feattype) == OBJ_BIOSEQ) tentityID = tap->seqEntityID;
            else tentityID = tap->entityID;
            if (tentityID == entityID && tap->itemID == itemID && OBJ_(tap->feattype) == itemtype)
               break;
         }
  }
  if (vnp==NULL) return NULL;
  return tap;
}

NLM_EXTERN CharPtr PNTR buf2array (ValNodePtr list, Int2 seq1, Int2 seq2)
{
  CharPtr  PNTR   tabp = NULL;
  ValNodePtr      listmp;
  TextAlignBufPtr tdp;
  Int2            nrows, j;
 
  nrows = seq2-seq1+1;
  if (nrows > 1 )  {
     tabp = (CharPtr PNTR) MemNew ((size_t)((nrows+4)*sizeof(CharPtr)));
     if (tabp != NULL)  {
        for (j=0; j<nrows+4; j++)
           tabp[j] = NULL;
        j = 0;    
        for (listmp=list; listmp!=NULL; listmp=listmp->next)
        {
           tdp = (TextAlignBufPtr) listmp->data.ptrvalue;
           if (tdp!=NULL) { 
              if (OBJ_(tdp->feattype) == OBJ_BIOSEQ) { 
                 if (tdp->buf != NULL && (j>=seq1 && j<=seq2)) {
                    tabp[j-seq1] = tdp->buf;
                    j++;
                 } 
              }  
           }  
        }
     }   
  }  
  return tabp;   
}

NLM_EXTERN AlignNodePtr AlignNodeFind (ValNodePtr anpvnp, Uint2 entityID, Uint2 itemID, Uint2 itemtype)
{
  ValNodePtr   vnp;
  AlignNodePtr anp;

  if ( itemtype != OBJ_BIOSEQ ) return NULL;
  if ( anpvnp == NULL ) return NULL;
  for (vnp = anpvnp; vnp != NULL; vnp = vnp->next) {
         if ( (anp = (AlignNodePtr) vnp->data.ptrvalue) != NULL)
         {
              if ( anp->seq_entityID == entityID && anp->bsp_itemID == itemID )
                   break;
         }
  }
  if ( vnp == NULL ) return NULL;
  return anp;
}


NLM_EXTERN Int2 AlignNodeIndex (ValNodePtr anpvnp, Uint2 entityID, Uint2 itemID, Uint2 itemtype)
{
  ValNodePtr   vnp;
  AlignNodePtr anp;
  Int2         index = 0;

  if ( itemtype != OBJ_BIOSEQ ) return 0;
  if ( anpvnp == NULL ) return 0;
  for (vnp = anpvnp; vnp != NULL; vnp = vnp->next, index++) {
         if ( (anp = (AlignNodePtr) vnp->data.ptrvalue) != NULL)
         {
              if ( anp->seq_entityID == entityID && anp->bsp_itemID == itemID )
                   break;
         }
  }
  if ( vnp == NULL ) return 0;
  return index;
}

/******************************************************************/
NLM_EXTERN void OrderFeatProc (ValNodePtr vnpanp)
{
/*
  ValNodePtr   vnp;
  AlignNodePtr anp;
  AlignSegPtr  asp;

  for (vnp = vnpanp; vnp != NULL; vnp = vnp->next)
  {
     anp = (AlignNodePtr) vnp->data.ptrvalue;
     for(asp = anp->segs; asp !=NULL; asp = asp->next)
     {
        if (asp->cnp != NULL)
           OrderCdsmRNA (&(asp->cnp));
     }
  }
*/
  return;
}

/******************************************************************/
NLM_EXTERN ValNodePtr SeqfeatlistFree (ValNodePtr feathead)
{
  ValNodePtr     vnp, next;
  SelEdStructPtr sesp;
 
  if (feathead==NULL)
     return NULL;
  vnp = feathead; 
  while (vnp != NULL)
  {
     next = vnp->next;
     if (vnp->data.ptrvalue != NULL) {
        sesp = (SelEdStructPtr) vnp->data.ptrvalue;
        vnp->data.ptrvalue = SelEdStructListDel (sesp);
     }
     vnp = next;
  }
  ValNodeFree (vnp);
  return NULL;
}

/******************************************************************/
NLM_EXTERN ValNodePtr SeqfeatlistFree_fromID (ValNodePtr feathead, Uint2 entityID)
{
  ValNodePtr     vnp, pre, next;
  SelEdStructPtr sesp;
 
  vnp = feathead; 
  pre = NULL;
  while (vnp != NULL)
  {
     next = vnp->next;
     if (vnp->data.ptrvalue != NULL) {
        sesp = (SelEdStructPtr) vnp->data.ptrvalue;
        if (sesp->entityID == entityID) {
           vnp->data.ptrvalue = SelEdStructListDel (sesp);
           vnp->next = NULL;
           if (pre != NULL) pre->next = next;
           else feathead = next;
           ValNodeFree (vnp);
        }
        else pre = vnp;
     }
     vnp = next;
  }
  return feathead;
}

/******************************************************************/
NLM_EXTERN SelEdStructPtr get_feat_fromid (ValNodePtr feat_vnp, Uint2 feattype, Uint2 ei, Uint2 ii, Int4 pos, SelEdStructPtr *prec)
{
  ValNodePtr     vnp;
  SelEdStructPtr sesp = NULL;
  SeqLocPtr      slp;
  Boolean        precedent;

  if (feat_vnp == NULL ) 
         return NULL;
  if (prec != NULL) precedent = TRUE;
  else precedent = FALSE;
  for (vnp = feat_vnp; vnp != NULL; vnp = vnp->next)
  {
         if (vnp->choice == feattype || feattype == 255) {
            sesp = (SelEdStructPtr) vnp->data.ptrvalue;
            if (sesp->entityID == ei && sesp->itemID == ii)
            {
               if (pos < 0)
                  return sesp;
               else {
                  if (precedent) *prec = NULL;
                  for (; sesp != NULL; sesp = sesp->next) {
                     if (sesp->regiontype == OM_REGION_SEQLOC && sesp->region != NULL) 
                     {
                        slp = (SeqLocPtr) sesp->region;
                        if (pos >= SeqLocStart(slp) && pos <= SeqLocStop(slp)) 
                           return sesp;
                     }
                     if (precedent) *prec = sesp;
                  }
               }
            }
         }
  }
  return NULL;
}

/******************************************************************/
NLM_EXTERN SeqLocPtr CollectSeqLocFromAlignNode (AlignNodePtr anp)
{
  AlignSegPtr asp;
  SeqLocPtr slp = NULL;
  Int4 start, stop;
  Uint1 strand;
  Int4 current_pos;
  Int4 seglen;

  current_pos = anp->seqpos;
  if(anp->seqpos < 0)
         strand = Seq_strand_minus;
  else
         strand = Seq_strand_plus;
  for(asp = anp->segs; asp !=NULL; asp = asp->next)
  {
         seglen = 0;
         if(asp->type == REG_SEG || asp->type == DIAG_SEG)
         {
                seglen = asp->gr.right - asp->gr.left +1;
                if(strand == Seq_strand_minus)
                {
                       stop = -current_pos;
                       start = stop - (seglen-1);
                }
                else
                {
                       start = current_pos;
                       stop = start + (seglen -1);
                }
                if(slp == NULL)
                       slp = SeqLocIntNew (start, stop, strand, anp->sip);
                else
                       expand_seq_loc (start, stop, strand, slp);
         }
         current_pos += seglen;
  }
  return slp;
}

NLM_EXTERN Int4 GetAlignLengthFromAlignNode (AlignNodePtr anp)
{
  AlignSegPtr asp;
  Int4 lens;

  lens = 0;
  for (asp = anp->segs; asp !=NULL; asp = asp->next)
  {
         if (asp->type == INS_SEG)
                lens += asp->gr.right;
         else 
                lens += asp->gr.right - asp->gr.left +1;
  }
  return lens;
}

/******************************************************************/
NLM_EXTERN SeqIdPtr SeqIdFromAlignNode (ValNodePtr anp_lst, Uint2 entityID, Uint2 itemID, Uint2 itemtype)
{
  AlignNodePtr anp;

  anp = (AlignNodePtr) AlignNodeFind (anp_lst, entityID, itemID, itemtype);
  if (anp == NULL) 
     return NULL;
  return anp->sip;
}

NLM_EXTERN Uint1 StrandFromAlignNode (ValNodePtr anp_lst, Uint2 entityID, Uint2 itemID, Uint2 itemtype)
{
  AlignNodePtr anp;

  anp = (AlignNodePtr) AlignNodeFind (anp_lst, entityID, itemID, itemtype);
  if (anp == NULL) 
     return Seq_strand_unknown;
  return anp->extremes.strand;
}

/*********************************************************
***
***  SeqIdPtr procedures
**********************************************************/
NLM_EXTERN CharPtr matching_seqid (SeqIdPtr sip1)
{
  SeqIdPtr siptmp1, siptmp2;
  Boolean  first = TRUE;
  Char     strLog[120];
  CharPtr  str;

  for (siptmp1 = sip1; siptmp1!=NULL; siptmp1=siptmp1->next) {
    first = TRUE;
    for (siptmp2 = sip1; siptmp2!=NULL; siptmp2=siptmp2->next) 
    {
       if (SeqIdForSameBioseq(siptmp1, siptmp2))  {
          if (first) 
             first = FALSE;
          else {
             SeqIdWrite(siptmp1, strLog, PRINTID_FASTA_LONG, 120);
             str = StringSave(strLog);
             return str;
          }
       }
    }
  }
  return NULL;
}

NLM_EXTERN SeqIdPtr ValNodeSeqIdListDup (ValNodePtr id_list)
{
  ValNodePtr   vnp=NULL;
  SeqIdPtr     sip = NULL;

  for (vnp = id_list; vnp != NULL; vnp = vnp->next) 
  {
         sip = AddSeqId (&sip, (SeqIdPtr) vnp->data.ptrvalue);  
  }
  return sip;
}
/*******************************************************
***
***   SeqIdListToCharArray
***
*******************************************************/
NLM_EXTERN CharPtr PNTR SeqIdListToCharArray (SeqIdPtr id_list, Int2 n)
{
  CharPtr PNTR  idarray; 
  SeqIdPtr      idtmp; 
  Int2          k;

  idarray = (CharPtr PNTR)MemNew ((size_t) ((n+1) * sizeof(CharPtr)));
  idtmp = id_list;
  for (k = 0; k < n && idtmp!=NULL; k++, idtmp =idtmp->next) {
         idarray [k] = (CharPtr) MemNew ((size_t) ((52) * sizeof(Char)));
         SeqIdWrite (idtmp, idarray [k], PRINTID_FASTA_SHORT, 50);
  }
  return idarray;
}

/******************************************************************/
NLM_EXTERN SeqIdPtr SeqIdReplaceID (SeqIdPtr head, SeqIdPtr pre, SeqIdPtr sip, SeqIdPtr next)
{
  SeqIdPtr tmp;

  if (pre == NULL)
  {
     head = SeqIdDup(sip);
     head->next = next;
     return head;
  }
  tmp = pre->next;
  pre->next = NULL;
  tmp->next = NULL;
  SeqIdFree (tmp);
  pre->next = SeqIdDup(sip);
  pre->next->next = next;
  return head;
}

typedef struct ccid {
  SeqIdPtr    sip;
  SeqEntryPtr sep;
  BioseqPtr   bsp;
} CcId, PNTR CcIdPtr;

typedef struct replaceseqid
{
  SeqIdPtr old_id;
  SeqIdPtr new_id;  
} ReplaceSeqIdData, PNTR ReplaceSeqIdPtr;

static void ReplaceSeqIdCallback (SeqFeatPtr sfp, Pointer userdata)
{
  ReplaceSeqIdPtr rsip;
  SeqIdPtr        current_sip;
  
  if (sfp == NULL || userdata == NULL)
  {
    return;
  }
  rsip = (ReplaceSeqIdPtr) userdata;
  if (rsip->old_id == NULL || rsip->new_id == NULL)
  {
    return;
  }
  current_sip = SeqLocId (sfp->location);
  if (SeqIdIn (current_sip, rsip->old_id))
  {
    sfp->location = SeqLocReplaceID (sfp->location, rsip->new_id);
  }
}

static void SeqAnnotReplaceID (SeqAnnotPtr sap, SeqIdPtr newsip)
{
  SeqFeatPtr  sfp;

  while (sap != NULL) {
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      while (sfp != NULL) {
        if (sfp->location != NULL)
           sfp->location = SeqLocReplaceID (sfp->location, newsip);
        sfp = sfp->next;
      }
    }
    sap = sap->next;
  }
}

NLM_EXTERN BioseqPtr BioseqReplaceID (BioseqPtr bsp, SeqIdPtr newsip)
{
  Uint2            entityID;
  SeqEntryPtr      sep;
  ReplaceSeqIdData rsid;
  
  if (bsp == NULL || bsp->id == NULL || newsip == NULL) 
  {
    return bsp;
  }

  entityID = ObjMgrGetEntityIDForPointer (bsp);  
  sep = GetTopSeqEntryForEntityID (entityID);
  rsid.old_id = bsp->id;
  rsid.new_id = newsip;
  
  VisitFeaturesInSep (sep, &rsid, ReplaceSeqIdCallback);
  SeqIdFree (bsp->id);
  bsp->id = SeqIdDup (newsip);
  SeqMgrReplaceInBioseqIndex (bsp);
  return bsp;
}

static void SeqEntryReplaceSeqIDCallBack (SeqEntryPtr sep, Pointer mydata,
                                          Int4 index, Int2 indent)
{
  SeqIdPtr PNTR      sipp;
  SeqIdPtr           newsip;
  BioseqPtr          bsp;
  BioseqSetPtr       bssp;

  if (sep != NULL && sep->data.ptrvalue && mydata != NULL) {
     sipp = (SeqIdPtr PNTR)mydata;
     newsip = *sipp;
     if (IS_Bioseq(sep)) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        if (ISA_na (bsp->mol)) {
           BioseqReplaceID (bsp, newsip);
        }
     } 
     else if (IS_Bioseq_set(sep)) {
        bssp = (BioseqSetPtr) sep->data.ptrvalue;
        if (bssp->annot) {
           SeqAnnotReplaceID (bssp->annot, newsip);
        }
     }
  }
}
  

static void FindSeqEntryForSeqIdCallback (SeqEntryPtr sep, Pointer mydata,
                                          Int4 index, Int2 indent)
{
  BioseqPtr          bsp;
  CcIdPtr            cip;

  if (sep != NULL && sep->data.ptrvalue && mydata != NULL) {
     cip = (CcIdPtr)mydata;
     if (cip->sep==NULL && IS_Bioseq(sep)) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        if (bsp!=NULL && ISA_na (bsp->mol)) 
        {
           if (SeqIdForSameBioseq(cip->sip, bsp->id)) {
              cip->sep = sep;
              cip->bsp = bsp;
           }
        }
     }
  }
}

NLM_EXTERN SeqEntryPtr SeqEntryReplaceSeqID (SeqEntryPtr source_sep, SeqIdPtr sip)
{
  SeqEntryPtr sep;
  SeqLocPtr   slp;
  SeqIdPtr    newsip;
  CcId        ci;
  Uint2       entityID;

  ci.sip = SeqIdDup (sip);
  ci.sep = NULL;
  ci.bsp = NULL;
  SeqEntryExplore(source_sep,(Pointer)&ci, FindSeqEntryForSeqIdCallback);
  if (ci.sep && ci.bsp)
  {
     entityID = ObjMgrGetEntityIDForPointer (ci.bsp);
     sep = GetBestTopParentForData (entityID, ci.bsp);
     slp = SeqLocIntNew (0, ci.bsp->length-1, Seq_strand_plus, ci.bsp->id);
     newsip = MakeNewProteinSeqId(slp, NULL);
     ValNodeFree (slp);
     SeqEntryExplore (sep, &newsip, SeqEntryReplaceSeqIDCallBack);
  }
  SeqIdFree (ci.sip);
  return source_sep;
}

/*********************************************************
***
***  ScorePtr procedures
***
**********************************************************/
NLM_EXTERN ScorePtr ScoreDup (ScorePtr sp)

{
  ScorePtr sp_copy;

  if(sp == NULL) return NULL;
  sp_copy = ScoreNew();
  if(sp_copy == NULL) return NULL;
  sp_copy->id = (ObjectIdPtr) ObjectIdDup (sp->id);
  sp_copy->choice = sp->choice;
  sp_copy->value = sp->value;
  sp_copy->next = NULL;
  return sp_copy;
}

NLM_EXTERN ScorePtr ScoreDupAdd (ScorePtr *sp_head, ScorePtr sp)
{
  ScorePtr sp_tmp, sp_copy;

  sp_copy = ScoreDup (sp);
  if ( (sp_tmp = *sp_head) != NULL ) {
         while (sp_tmp->next != NULL) sp_tmp = sp_tmp->next; 
         sp_tmp->next = sp_copy;
  }  
  else *sp_head = sp_copy;
  return *sp_head;
}

NLM_EXTERN ScorePtr ScoreAdd (ScorePtr *sp_head, ScorePtr sp)
{
  ScorePtr sp_tmp;

  if ( (sp_tmp = *sp_head) != NULL ) {
         while (sp_tmp->next != NULL) sp_tmp = sp_tmp->next; 
         sp_tmp->next = sp;
  }  
  else *sp_head = sp;
  return *sp_head;
}


/*********************************************************
***
***  SeqLocPtr procedures
***
**********************************************************/
NLM_EXTERN Int2 chkloc (SeqIdPtr sip, Int4 position, ValNodePtr vnp, Int4 *newpos)
{
  ValNodePtr vnptmp;
  SeqIdPtr   siptmp;
  SeqLocPtr  slp;

  *newpos = 0;
  for (vnptmp=vnp; vnptmp!=NULL; vnptmp=vnptmp->next)
  {
     slp = (SeqLocPtr)vnptmp->data.ptrvalue;
     siptmp = SeqLocId (slp);
     if (siptmp!=NULL) 
     {
        if (SeqIdForSameBioseq (sip, siptmp)) {
           if (position >= SeqLocStart(slp) && position <= SeqLocStop(slp)) {
              *newpos = position;
              return 0;
           }
           if (position >= SeqLocStop(slp) && position <= SeqLocStart(slp)) {
              *newpos = position;
              return 0;
           }
           if (position==APPEND_RESIDUE || position>=SeqLocStart(slp)+SeqLocLen(slp)) {
              *newpos = SeqLocStart(slp) + SeqLocLen(slp); 
              return APPEND_RESIDUE;
           }
           if (position < SeqLocStart(slp)) {
              *newpos = SeqLocStart(slp);
              return GAP_RESIDUE; 
           }
        }
     }
  }
  return NO_RESIDUE;
}

NLM_EXTERN SeqLocPtr expand_seq_loc(Int4 start, Int4 stop, Uint1 strand, SeqLocPtr loc)
{
   SeqIntPtr sint;
   SeqPntPtr spp;
 
        if(loc->choice == SEQLOC_INT)
        {
                sint = (SeqIntPtr)loc->data.ptrvalue;
                if(start != -1 && start < sint->from)
                        sint->from = start;
                if(stop != -1 && stop > sint->to)
                        sint->to = stop;
                if(strand != 0 && sint->strand != strand)
                        sint->strand = strand;
                loc->data.ptrvalue = sint;
        }
        else if(loc->choice == SEQLOC_PNT)
        {
                spp = (SeqPntPtr)(loc->data.ptrvalue);
                spp->point = start;
                spp->strand = strand;
                loc->data.ptrvalue = spp;
        }
 
        return loc;
}

NLM_EXTERN Int4 MaxLengthSeqLoc (ValNodePtr sqloc_list)
{
  ValNodePtr       vnp;
  SeqLocPtr        slp;
  Int4             len, maxlen = 0;

  for (vnp = sqloc_list; vnp != NULL; vnp = vnp->next)
  { 
         slp = (SeqLocPtr) vnp->data.ptrvalue;
         if ( ( len = SeqLocLen (slp)) > maxlen ) 
                maxlen = len;
  }
  return maxlen;
}

NLM_EXTERN Boolean SeqLocListMatch (ValNodePtr vnp1, ValNodePtr vnp2, Boolean *Fp, Boolean *Tp)
{
  ValNodePtr tmp1, tmp2;
  SeqLocPtr  slp1, slp2;
  Boolean    p5short = FALSE, p3short=FALSE;

  for (tmp1=vnp1, tmp2=vnp2; tmp1!=NULL && tmp2!=NULL; tmp1=tmp1->next, tmp2=tmp2->next)
  {
     slp1 = (SeqLocPtr) tmp1->data.ptrvalue;
     slp2 = (SeqLocPtr) tmp2->data.ptrvalue;
     if (!p5short) 
            p5short = ( SeqLocStart(slp1) < SeqLocStart(slp2) );
     if (!p3short) 
            p3short  = ( SeqLocStop (slp1) > SeqLocStop (slp2) );
     if (p5short && p3short) break;
  }
  *Fp = p5short;
  *Tp = p3short;
  if (p5short || p3short) return FALSE;
  return TRUE;
} 


/***********************************************************************
***    
***    SeqEntryToSeqLoc
***      read SeqEntry (1->Bioseq or 2->BioseqSetPtr)
***      return list of ValNodePtr->SeqLocPtr
***      The strand of the seqloc is 0, whatever the strand of the bsp 
************************************************************************/
typedef struct ccid3
{
  ValNodePtr vnp;
  Uint1      bsp_mol;
} CcId3, PNTR CcId3Ptr;

static void ListSeqEntryCallback (SeqEntryPtr sep, Pointer mydata,
                                          Int4 index, Int2 indent)
{
  BioseqPtr          bsp;
  SeqIdPtr           sip;
  SeqLocPtr          slp;
  CcId3Ptr           ccp;

  if (sep != NULL && sep->data.ptrvalue && mydata != NULL) {
     ccp = (CcId3Ptr)mydata;
     if (IS_Bioseq(sep)) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        if (bsp!=NULL) {
         if (ccp->bsp_mol==0 || ISA_aa(ccp->bsp_mol)==ISA_aa(bsp->mol))
         {
           sip = SeqIdFindBest(bsp->id, 0);
           if (sip!=NULL) {
              slp = SeqLocIntNew (0, bsp->length - 1, Seq_strand_plus, sip);
              ValNodeAddPointer (&(ccp->vnp), 0, slp);
           }
         }
        }
     }
  }
}

NLM_EXTERN ValNodePtr SeqEntryToSeqLoc (SeqEntryPtr sep, Int2 *n, Uint1 bsp_mol)
{
  CcId3              cc;
  ValNodePtr         vnp = NULL;
  Int2               j=0;

  cc.vnp=NULL;
  cc.bsp_mol=bsp_mol;
  SeqEntryExplore(sep,(Pointer)&cc, ListSeqEntryCallback);
  for (vnp = cc.vnp;vnp!=NULL; vnp=vnp->next)
  {
     j++;
  } 
  *n = j;
  return cc.vnp;
}


/************************************************************/
NLM_EXTERN SelStructPtr SelStructNew (Uint2 ssp_ed, Uint2 ssp_id, Uint2 ssp_it, Int4 from, Int4 to, SeqIdPtr sip, Uint1 strand, Boolean is_fuzz)
{
  SelStructPtr   ssp;
  SeqLocPtr      slp;

  ssp = (SelStructPtr) MemNew (sizeof (SelStruct));
  if (ssp == NULL) return NULL;
  ssp->entityID = ssp_ed;
  ssp->itemID = ssp_id;
  ssp->itemtype = ssp_it;
  ssp->regiontype = 0;
  ssp->region = NULL;
  if (from >= 0 && sip != NULL) {
     if (is_fuzz)
         slp = fuzz_loc (from, to, strand, sip, TRUE, TRUE);
     else 
         slp = SeqLocIntNew (from, to, strand, sip);
     if ( slp != NULL ) {
         ssp->regiontype = OM_REGION_SEQLOC;
         ssp->region = (Pointer) slp;
     }
  }
  ssp->prev = NULL;
  ssp->next = NULL;
  return ssp;
}
/******************************************************************/
NLM_EXTERN SelStructPtr SelStructCpy (SelStructPtr ssp, SelStructPtr ssp2)
{
  SeqLocPtr  slp, slp2;
  if (ssp == NULL) return NULL;
  ssp2->entityID = ssp->entityID;
  ssp2->itemID = ssp->itemID;
  ssp2->itemtype = ssp->itemtype;
  if ( ssp->regiontype == OM_REGION_SEQLOC) {
         ssp2->regiontype = OM_REGION_SEQLOC;
         if ( ssp->region == NULL ) {
               ErrPostEx (SEV_WARNING, 0, 0, "Fail in SelStructCpy [1]");
               return NULL;
         }
         if ( ssp2->region != NULL ) 
               SeqLocFree ((SeqLocPtr)ssp2->region);
         slp = (SeqLocPtr) ssp->region;
         slp2 = SeqLocIntNew (SeqLocStart(slp), SeqLocStop(slp), 
                              SeqLocStrand(slp), SeqLocId(slp));
         if ( slp2 == NULL ) {
               ErrPostEx(SEV_WARNING, 0, 0, "Fail in SelStructCpy [2]");
               return NULL;
         }
         ssp2->region = (Pointer) slp2;
  } 
  else {
         ssp2->regiontype = 0;
         ssp2->region = NULL;
  }
  ssp2->next = NULL;
  ssp2->prev = NULL;
  return ssp2;
}

/******************************************************************/
NLM_EXTERN SelStructPtr SelStructDup (SelStructPtr ssp)
{
  SelStructPtr ssp2;

  if (ssp == NULL) return NULL;
  ssp2 = (SelStructPtr) MemNew (sizeof (SelStruct));
  ssp2 = SelStructCpy (ssp, ssp2);
  return ssp2;
}

/******************************************************************/
NLM_EXTERN SelStructPtr SelStructAdd (SelStructPtr head, SelStructPtr ssp)
{
  SelStructPtr ssptmp;
  if (head == NULL) return ssp;
  for (ssptmp = head; ssptmp->next != NULL; ssptmp=ssptmp->next) continue;
  ssptmp->next = ssp;
  ssp->prev = ssptmp;
  return head;
}

/******************************************************************/
NLM_EXTERN SelStructPtr SelStructDel (SelStructPtr ssp)
{

  if (ssp == NULL) return NULL;
  ssp->next = NULL;
  if ( ssp->region != NULL ) 
         SeqLocFree ((SeqLocPtr) ssp->region);
  MemFree (ssp);
  return NULL;
}

/******************************************************************/
NLM_EXTERN SelStructPtr SelStructDelList (SelStructPtr ssp)
{
  SelStructPtr tmp, next;
 
  if (ssp!=NULL) {
     tmp=ssp;
     while (tmp!=NULL) {
        next = tmp->next;
        tmp->next = NULL;
        SelStructDel (tmp);
        tmp=next;
     }
  }   
  return NULL;
}

/*****************************************************************/
NLM_EXTERN void setposition_tossp (SelStructPtr ssp, Int4 from, Int4 to)
{
  SeqLocPtr        slp;
  SeqIntPtr        sitcaret;
  if (ssp == NULL) return;
  if (ssp->region == NULL) return;
  slp = (SeqLocPtr) ssp->region;
  sitcaret = (SeqIntPtr) slp->data.ptrvalue;
  sitcaret->from = from;
  sitcaret->to = to;
}

/******************************************************************/
NLM_EXTERN Boolean is_samessp (SelStructPtr ssp1, SelStructPtr ssp2)
{
  if (ssp1 == NULL || ssp2 == NULL) return FALSE;
  if (ssp1->entityID != ssp2->entityID) return FALSE;
  if (ssp1->itemID != ssp2->itemID) return FALSE;
  if (ssp1->itemtype != ssp2->itemtype) return FALSE;
  return TRUE;
}

/******************************************************************/
NLM_EXTERN Boolean is_sameId (Uint2 sei, Uint2 sii, Uint2 sit, Uint2 sist, Uint2 ei, Uint2 ii, Uint2 it, Uint2 ist)
{
  if ( ei < 255 && sei != ei) return FALSE;
  if ( ii < 255 && sii != ii) return FALSE;
  if ( it < 255 && sit != it) return FALSE;
  if ( ist< 255 && sist!= ist)return FALSE;
  return TRUE;
}

/******************************************************************/
NLM_EXTERN Boolean is_samepos (SelStructPtr ssp1, SelStructPtr ssp2)
{
  SeqLocPtr       slp1, slp2;

  if (ssp1->regiontype == 0 || ssp2->regiontype == 0) return FALSE;
  if (ssp1->region == NULL || ssp2->region == NULL) return FALSE;
  if ( !is_samessp (ssp1, ssp2) ) return FALSE;
  slp1 = (SeqLocPtr) ssp1->region;
  slp2 = (SeqLocPtr) ssp2->region;
  if ( slp1 == NULL || slp2 == NULL) return FALSE;
  if ( SeqLocStart(slp1) != SeqLocStart(slp2) ) return FALSE;
  if ( SeqLocStop(slp1) != SeqLocStop(slp2) ) return FALSE;
  return TRUE;
}

/******************************************************************/
NLM_EXTERN ValNodePtr del_ssp_fromid (ValNodePtr headp, Uint2 itemsubtype, SelEdStructPtr target)
{
  ValNodePtr     vnphead, pre_vnp = NULL;
  SelEdStructPtr ssp,  pressp = NULL;
  Boolean        found_ssp = FALSE;

  if (headp == NULL || target == NULL) {
         ErrPostEx(SEV_WARNING, 0, 0, "fail in del_ssp_fromid [1]");
         return NULL;
  }
  pre_vnp = NULL;
  for  (vnphead = headp; vnphead != NULL; vnphead = vnphead->next) 
  {
     if ( itemsubtype == 255 || vnphead->choice == itemsubtype ) {
        ssp = (SelEdStructPtr) vnphead->data.ptrvalue; 
        if (is_sameses (ssp, target)) {
           pressp = NULL;
           for (; ssp != NULL; ssp = ssp->next) 
           {
              if (include_ssp ((SeqLocPtr) ssp->region, (SeqLocPtr) target->region)) 
              {
                 found_ssp = TRUE; 
                 break;
              }
              pressp = ssp;
           }
           if (found_ssp) break;
        }
     }
     pre_vnp = vnphead;
  } 
  if (vnphead == NULL || ssp == NULL) return headp;
  SelEdStructListDel ((SelEdStructPtr) vnphead->data.ptrvalue);
  vnphead->data.ptrvalue = NULL;
  if (pre_vnp == NULL) {
     if (vnphead->next == NULL) {
        headp = NULL;
     }
     else {
        headp = vnphead->next;
        vnphead->next = NULL;
     }
  }
  else if (vnphead->next == NULL) {
     pre_vnp->next = NULL;
  }
  else {
     pre_vnp->next = vnphead->next;
     vnphead->next = NULL;
  }
  ValNodeFree (vnphead);
  return headp;
}

/******************************************************************/
NLM_EXTERN Boolean include_ssp (SeqLocPtr slp1, SeqLocPtr slp2)
{
  if ( SeqLocStart(slp1) <= SeqLocStart(slp2) 
  && SeqLocStop(slp1) >= SeqLocStop(slp2) ) return TRUE;
  return FALSE;
}

/******************************************************************/
NLM_EXTERN Int4 overlapp_startssp (SeqLocPtr slp1, SeqLocPtr slp2)
{
  if ( SeqLocStart(slp1) < SeqLocStart(slp2) 
  &&  SeqLocStop(slp1) < SeqLocStop(slp2) ) 
     return (SeqLocLen(slp1) - (SeqLocStart(slp2) - SeqLocStart(slp1)));
  return 0;
}

/******************************************************************/
NLM_EXTERN Boolean overlapp_ssp (SeqLocPtr slp1, SeqLocPtr slp2)
{
  if ( SeqLocStop(slp1) < SeqLocStart(slp2) ) return FALSE;
  if ( SeqLocStart(slp1) > SeqLocStop(slp2) ) return FALSE;
  return TRUE;
}

/******************************************************************/
NLM_EXTERN Boolean precede_ssp (SeqLocPtr slp1, SeqLocPtr slp2)
{
  if ( SeqLocStart(slp2) >= SeqLocStop(slp1) ) return TRUE;
  return FALSE;
}

/******************************************************************/
NLM_EXTERN Boolean succeed_ssp (SeqLocPtr slp1, SeqLocPtr slp2)
{
  if ( SeqLocStop(slp2) == SeqLocStart(slp1) ) return TRUE;
  return FALSE;
}


/******************************************************************/
NLM_EXTERN SelEdStructPtr new_seledstruct (Uint2 ssp_ed, Uint2 ssp_id, Uint2 ssp_it, Uint2 ssp_sit, Uint2 bspiID, Int4 from, Int4 to, SeqIdPtr sip, Uint1 strand, Boolean is_fuzz, CharPtr label, Pointer data, Int4 offset, Uint1 codonstart)
{
  SelEdStructPtr sesp;
  SeqLocPtr      slp;
  ValNodePtr     datavnp;

  sesp = (SelEdStructPtr) MemNew (sizeof (SelEdStruct));
  if (sesp == NULL) return NULL;
  sesp->entityID = ssp_ed;
  sesp->itemID = ssp_id;
  sesp->itemtype = ssp_it;
  sesp->itemsubtype = ssp_sit;
  sesp->bsp_itemID = bspiID;
  if (sesp->regiontype == OM_REGION_SEQLOC) 
         sesp->regiontype = 0;
  if (from < 0 || sip == NULL) {
         sesp->regiontype = 0;
         sesp->region = NULL;
  }
  else {
     sesp->region = NULL; 
     if (is_fuzz)
         slp = fuzz_loc (from, to, strand, sip, TRUE, TRUE);
     else 
         slp = SeqLocIntNew (from, to, strand, sip);
     if ( slp != NULL ) {
         sesp->regiontype = OM_REGION_SEQLOC;
         sesp->region = (Pointer) slp;
     }
     else   sesp->regiontype = 0; 
  }
  if (label != NULL && StringLen (label) > 0)
       StringCpy(sesp->label, label);
  else sesp->label[0] = '\0';
  if (data != NULL)
  {
     datavnp = ValNodeNew (NULL);
     datavnp->choice = 0;
     datavnp->data.ptrvalue = data;
     sesp->data = datavnp;
  }
  else sesp->data = NULL;
  sesp->codonstart = codonstart;
  sesp->offset = offset;
  sesp->dirty = TRUE;
  sesp->visible = TRUE;
  sesp->prev = NULL;
  sesp->next = NULL;
  return sesp;
}

NLM_EXTERN SelEdStructPtr new_seledstruct_fromseqloc (Uint2 ssp_ed, Uint2 ssp_id, Uint2 ssp_it, Uint2 ssp_sit, Uint2 bspiID, SeqLocPtr slp, CharPtr label, Pointer data, Int4 offset, Uint1 codonstart)
{
  SelEdStructPtr sesp;
  ValNodePtr     datavnp;

  sesp = (SelEdStructPtr) MemNew (sizeof (SelEdStruct));
  if (sesp == NULL) return NULL;
  sesp->entityID = ssp_ed;
  sesp->itemID = ssp_id;
  sesp->itemtype = ssp_it;
  sesp->itemsubtype = ssp_sit;
  sesp->bsp_itemID = bspiID;
  if (sesp->regiontype == OM_REGION_SEQLOC) 
     sesp->regiontype = 0;
  if (slp == NULL) {
     sesp->regiontype = 0;
     sesp->region = NULL;
  }
  else {
     sesp->regiontype = OM_REGION_SEQLOC;
     sesp->region = (Pointer) slp;
  }
  if (label != NULL && StringLen (label) > 0)
     StringCpy(sesp->label, label);
  else sesp->label[0] = '\0';
  if (data != NULL)
  {
     datavnp = ValNodeNew (NULL);
     datavnp->choice = 0;
     datavnp->data.ptrvalue = data;
     sesp->data = datavnp;
  }
  else sesp->data = NULL;
  sesp->codonstart = codonstart;
  sesp->offset = offset;
  sesp->dirty = TRUE;
  sesp->visible = TRUE;
  sesp->prev = NULL;
  sesp->next = NULL;
  return sesp;
}

/***************************************************************
*** sesp_to_slp
***    make seqloc from  ses->region (SelEdStructPtr) 
***    after changing the alignment coordinates into 
***    sequence coordinates
***    
***    the seqloc is NOT PARTIAL
***        fuzz_loc (start, stop, strand, sip, TRUE, TRUE);
***    but COMPLETE: 
***        SeqLocIntNew (start, stop, strand, sip);)
***************************************************************/
NLM_EXTERN SeqLocPtr sesp_to_slp (SelEdStructPtr ses, SeqAlignPtr salp, ValNodePtr sqlocs, Boolean partial)
{
  SelEdStructPtr sesp;
  SeqLocPtr      slptmp, slpnew, 
                 slp, slp1;
  SeqIdPtr       sip;
  Int4           start, stop;
  Uint1          strand;

  Int2           j,k;

  if (ses->next == NULL)
  {
     slptmp = (SeqLocPtr) ses->region;
     sip = SeqLocId (slptmp);
     start= (Int4)AlignCoordToSeqCoord (SeqLocStart(slptmp), sip, salp, sqlocs,0);
     stop = (Int4)AlignCoordToSeqCoord (SeqLocStop(slptmp), sip, salp, sqlocs, 0);
     slp = SeqLocIntNew (start, stop, SeqLocStrand (slptmp), sip);
     return slp;
  }
  slp1 = (SeqLocPtr) ValNodeNew (NULL);
  slp1->choice = SEQLOC_PACKED_INT;
  sesp = ses;
  slptmp = (SeqLocPtr) sesp->region;
  if (SeqLocStrand (slptmp) == Seq_strand_minus)
  {
     j=0;
     while (sesp->next!=NULL) {
        j++; sesp = sesp->next;
     }
     slptmp = (SeqLocPtr) sesp->region;
  }
  strand = SeqLocStrand (slptmp);
  sip = SeqLocId (slptmp);
  start= (Int4) AlignCoordToSeqCoord (SeqLocStart (slptmp), sip, salp, sqlocs, 0);
  stop = (Int4) AlignCoordToSeqCoord (SeqLocStop (slptmp), sip, salp, sqlocs, 0);
  if (partial)
     slpnew = fuzz_loc (start, stop, strand, sip, TRUE, TRUE);
  else
     slpnew = SeqLocIntNew (start, stop, strand, sip);
  slp1->data.ptrvalue = slpnew;
  slp = slpnew;
  if (strand != Seq_strand_minus)
  {
     sesp = sesp->next;
     for (; sesp != NULL; sesp = sesp->next)
     {
        slptmp = (SeqLocPtr) sesp->region;
        start=(Int4)AlignCoordToSeqCoord(SeqLocStart(slptmp),sip, salp, sqlocs, 0);
        stop =(Int4)AlignCoordToSeqCoord(SeqLocStop(slptmp), sip, salp, sqlocs,0);
        slpnew = SeqLocIntNew (start, stop, strand, sip);
        slp->next = slpnew;
        slp = slp->next;
     }
  }  
  else {
     while (j>0) {
        sesp=ses;
        for (k=1; k<j; k++) sesp=sesp->next;
        slptmp = (SeqLocPtr) sesp->region;
        start=(Int4)AlignCoordToSeqCoord(SeqLocStart(slptmp), sip, salp, sqlocs,0);
        stop =(Int4)AlignCoordToSeqCoord(SeqLocStop(slptmp), sip, salp, sqlocs, 0);
        slpnew = SeqLocIntNew (start, stop, strand, sip);
        slp->next = slpnew;
        slp = slp->next;
        j--;
     }
  }
  return slp1;
}


static SelEdStructPtr SelEdStructCpy (SelEdStructPtr ssp, SelEdStructPtr ssp2)
{
  SeqLocPtr  slp, slp2;
  if (ssp == NULL) return NULL;
  ssp2->entityID = ssp->entityID;
  ssp2->itemID = ssp->itemID;
  ssp2->itemtype = ssp->itemtype;
  if ( ssp->regiontype == OM_REGION_SEQLOC && ssp->region != NULL) {
         ssp2->regiontype = OM_REGION_SEQLOC;
         if ( ssp2->region != NULL ) 
               SeqLocFree ((SeqLocPtr)ssp2->region);
         slp = (SeqLocPtr) ssp->region;
         slp2 = SeqLocIntNew (SeqLocStart(slp), SeqLocStop(slp), SeqLocStrand(slp), SeqLocId(slp));
         if ( slp2 == NULL ) {
               ErrPostEx(SEV_WARNING, 0, 0, "Fail in SelStructCpy [2]");
               return NULL;
         }
         ssp2->region = (Pointer) slp2;
  } 
  else {
         ssp2->regiontype = 0;
         ssp2->region = NULL;
  }
  ssp2->data = NULL;
  ssp2->next = NULL;
  ssp2->prev = NULL;
  return ssp2;
}

/******************************************************************/
NLM_EXTERN SelEdStructPtr SelEdStructDup (SelEdStructPtr ssp)
{
  SelEdStructPtr ssp2;

  if (ssp == NULL) return NULL;
  ssp2 = (SelEdStructPtr) MemNew (sizeof (SelEdStruct));
  ssp2 = SelEdStructCpy (ssp, ssp2);
  return ssp2;
}

/******************************************************************/
NLM_EXTERN SelEdStructPtr SelEdStructAdd (SelEdStructPtr head, SelEdStructPtr ssp)
{
  SelEdStructPtr ssptmp;
  if (head == NULL) return ssp;
  for (ssptmp = head; ssptmp->next != NULL; ssptmp=ssptmp->next) continue;
  ssptmp->next = ssp;
  ssp->prev = ssptmp;
  return head;
}

/******************************************************************/
NLM_EXTERN SelEdStructPtr SelEdStructDel (SelEdStructPtr ssp)
{
  ValNodePtr  vnpdata;

  if (ssp == NULL) return NULL;
  if ( ssp->data != NULL ) 
  {
           vnpdata = ssp->data;
           if ( vnpdata->data.ptrvalue != NULL ) {
              if (ssp->prev == NULL)
                 vnpdata->data.ptrvalue = MemFree(vnpdata->data.ptrvalue);
              else 
                 vnpdata->data.ptrvalue = NULL;
           }
           vnpdata = ValNodeFree (vnpdata);
  }
  if ( ssp->region != NULL ) 
         ssp->region = SeqLocFree ((SeqLocPtr) ssp->region);
  MemFree (ssp);
  return NULL;
}

/******************************************************************/
NLM_EXTERN SelEdStructPtr SelEdStructListDel (SelEdStructPtr ssp)
{
  SelEdStructPtr next;
  ValNodePtr     vnpdata;
  Boolean        first = TRUE;

  if (ssp == NULL) return NULL;
  while (ssp != NULL) 
  {
     next = ssp->next;
     if ( ssp->region != NULL ) 
           ssp->region = SeqLocFree ((SeqLocPtr) ssp->region);
     if ( ssp->data != NULL ) 
     {
           vnpdata = ssp->data;
           if ( vnpdata->data.ptrvalue != NULL ) {
              if (first) 
		  vnpdata->data.ptrvalue = MemFree (vnpdata->data.ptrvalue);
	      else 
		  vnpdata->data.ptrvalue = NULL;
           }
           vnpdata = ValNodeFree (vnpdata);
     }
     MemFree (ssp);
     ssp = next;
	 if (first) first = FALSE;
  }
  return NULL;
}

NLM_EXTERN void set_seqnot_visible (Uint2 eID, Uint2 iID, SelEdStructPtr sesp)
{
  SelEdStructPtr tmp;
  for (tmp=sesp;tmp!=NULL; tmp=tmp->next)
  {
     if (tmp->entityID==eID && tmp->itemID==iID)
        tmp->visible = FALSE;
  }
}

NLM_EXTERN void set_seqvisible (Uint2 eID, Uint2 iID, SelEdStructPtr sesp)
{
  SelEdStructPtr tmp;
  for (tmp=sesp;tmp!=NULL; tmp=tmp->next)
  {
     if (tmp->entityID==eID && tmp->itemID==iID)
        tmp->visible = TRUE;
  }
}
NLM_EXTERN Boolean is_seqvisible (Uint2 eID, Uint2 iID, SelEdStructPtr sesp)
{
  SelEdStructPtr tmp;
  for (tmp=sesp;tmp!=NULL; tmp=tmp->next)
  {
     if (tmp->entityID==eID && tmp->itemID==iID)
        return (Boolean)tmp->visible;
  }
  return FALSE;
}

/******************************************************************/
NLM_EXTERN void setposition_toses (SelEdStructPtr ssp, Int4 from, Int4 to)
{
  SeqLocPtr        slp;
  SeqIntPtr        sitcaret;
  if (ssp == NULL) return;
  if (ssp->region == NULL) return;
  slp = (SeqLocPtr) ssp->region;
  sitcaret = (SeqIntPtr) slp->data.ptrvalue;
  sitcaret->from = from;
  sitcaret->to = to;
}


NLM_EXTERN SelEdStructPtr ss_to_ses (SelStructPtr ssp)
{
  SeqLocPtr      slp, slpses;
  SelEdStructPtr sesp;
  SeqIdPtr       sip;
  BioseqPtr      bsp;

  if (ssp == NULL) return NULL;
  sesp = (SelEdStructPtr) MemNew (sizeof (SelEdStruct));
  sesp->entityID = ssp->entityID;
  sesp->itemID = ssp->itemID;
  sesp->itemtype = ssp->itemtype;
  if ( ssp->regiontype == OM_REGION_SEQLOC) {
         sesp->regiontype = OM_REGION_SEQLOC;
         if ( ssp->region == NULL ) {
               ErrPostEx(SEV_WARNING, 0, 0, "Fail in SelStructCpy [1]");
               return NULL;
         }
         slp = (SeqLocPtr) ssp->region;
         sip = NULL;
         bsp = BioseqLockById (SeqLocId (slp));
         if (bsp != NULL) {
            for (sip=bsp->id; sip!= NULL; sip = sip->next) {
               if (sip->choice == SEQID_GI)
                  break; 
            }
            BioseqUnlock (bsp); 
         } 
         if (sip == NULL) {
            sip = SeqLocId (slp);
            sip = SeqIdFindBest (sip, 0);
         }
         slpses = SeqLocIntNew (SeqLocStart(slp), SeqLocStop(slp), SeqLocStrand(slp), sip);
         if ( slpses == NULL ) {
               ErrPostEx(SEV_WARNING, 0, 0, "Fail in SelStructCpy [2]");
               return NULL;
         }
         sesp->region = (Pointer) slpses;
  } 
  else {
         sesp->regiontype = 0;
         sesp->region = NULL;
  }
  sesp->label[0] ='\0';
  sesp->data = NULL;
  sesp->next = NULL;
  sesp->prev = NULL;
  return sesp;
}

NLM_EXTERN SelStructPtr ses_to_ss (SelEdStructPtr sesp)
{
  SeqLocPtr    slp, slpses;
  SelStructPtr ssp;

  if (sesp == NULL) return NULL;
  ssp = (SelStructPtr) MemNew (sizeof (SelStruct));
  ssp->entityID = sesp->entityID;
  ssp->itemID = sesp->itemID;
  ssp->itemtype = sesp->itemtype;
  if ( sesp->regiontype == OM_REGION_SEQLOC ) {
         ssp->regiontype = OM_REGION_SEQLOC;
         if ( sesp->region == NULL ) {
               ErrPostEx(SEV_WARNING, 0, 0, "Fail in SelStructCpy [1]");
               return NULL;
         }
         slpses = (SeqLocPtr) sesp->region;
         slp = SeqLocIntNew (SeqLocStart(slpses), SeqLocStop(slpses), 
                              SeqLocStrand(slpses), SeqLocId(slpses));
         if ( slp == NULL ) {
               ErrPostEx(SEV_WARNING, 0, 0, "Fail in SelStructCpy [2]");
               return NULL;
         }
         ssp->region = (Pointer) slp;
  } 
  else {
         ssp->regiontype = 0;
         ssp->region = NULL;
  }
  ssp->next = NULL;
  ssp->prev = NULL;
  return ssp;
}

/******************************************************************/
NLM_EXTERN Boolean is_samess_ses (SelStructPtr ssp1, SelEdStructPtr ssp2)
{
  if (ssp1 == NULL || ssp2 == NULL) return FALSE;
  if (ssp1->entityID != ssp2->entityID) return FALSE;
  if (ssp1->itemID != ssp2->itemID) return FALSE;
  if (ssp1->itemtype != ssp2->itemtype) return FALSE;
  return TRUE;
}

/******************************************************************/
NLM_EXTERN Boolean is_sameses (SelEdStructPtr ssp1, SelEdStructPtr ssp2)
{
  if (ssp1 == NULL || ssp2 == NULL) return FALSE;
  if (ssp1->entityID != ssp2->entityID) return FALSE;
  if (ssp1->itemID != ssp2->itemID) return FALSE;
  if (ssp1->itemtype != ssp2->itemtype) return FALSE;
  return TRUE;
}

/*********************************************************
***
***  ObjMgr procedures
***
**********************************************************/
NLM_EXTERN Boolean AfterAlsoSelect (void)
{
  SelStructPtr sspa = NULL,
               sspb = NULL;
  SeqLocPtr    slpa,
               slpb;
  SeqIntPtr    sint;
  Uint2        eIDa, iIDa, ita,
               eIDb, iIDb, itb;
  Boolean      check = TRUE,
               loopin = TRUE,
               changed = FALSE;
  
  while (check) 
  {
     check = FALSE;
     loopin = TRUE;
     sspa = ObjMgrGetSelected ();
     while (sspa != NULL && loopin) 
     {
        if ( checkssp_for_editor (sspa)) 
        {
           for (sspb = sspa->next; sspb != NULL; sspb = sspb->next) 
           {
              eIDa = sspa->entityID;
              iIDa = sspa->itemID;
              ita = sspa->itemtype;
              eIDb = sspb->entityID;
              iIDb = sspb->itemID;
              itb = sspb->itemtype;
              if ( checkssp_for_editor (sspb) && is_sameId (eIDa, iIDa, ita, 255, eIDb, iIDb, itb, 255) )
              {
                 slpa = (SeqLocPtr)sspa->region;
                 slpb = (SeqLocPtr)sspb->region;
                 if (SeqLocCompare (slpa, slpb) == SLC_A_IN_B) {
                    ObjMgrDeSelect(eIDa, iIDa, ita, sspa->regiontype, slpa);
                    check = TRUE;
                    loopin = FALSE;
                    changed = TRUE;
                    break;
                 }
                 else if (SeqLocCompare (slpa, slpb) == SLC_B_IN_A) {
                    ObjMgrDeSelect(eIDb, iIDb, itb, sspb->regiontype, slpb);
                    check = TRUE;
                    loopin = FALSE;
                    changed = TRUE;
                    break;
                 }
                 else if (SeqLocCompare (slpa, slpb) == SLC_A_EQ_B) {
                    ObjMgrDeSelect(eIDb, iIDb, itb, sspb->regiontype, slpb);
                    check = TRUE;
                    loopin = FALSE;
                    changed = TRUE;
                    break;
                 }
                 else if (SeqLocCompare (slpa, slpb) == SLC_A_OVERLAP_B) {
                    sint = (SeqIntPtr) slpa->data.ptrvalue;
                    if (SeqLocStart(slpa) < SeqLocStart(slpb)) {
                       sint->to = SeqLocStop(slpb);
                    }
                    else {
                       sint = (SeqIntPtr) slpa->data.ptrvalue;
                       sint->from = SeqLocStart(slpb);
                    }
                    ObjMgrDeSelect(eIDb, iIDb, itb, sspb->regiontype, slpb);
                    check = TRUE;
                    loopin = FALSE;
                    changed = TRUE;
                    break;
                 }
                 else if (SeqLocStop(slpa) == SeqLocStart(slpb)-1) {
                    sint = (SeqIntPtr) slpa->data.ptrvalue;
                    sint->to = SeqLocStop(slpb);
                    ObjMgrDeSelect(eIDb, iIDb, itb, sspb->regiontype, slpb);
                    check = TRUE;
                    loopin = FALSE;
                    changed = TRUE;
                    break;
                 }
                 else if (SeqLocStart(slpa) == SeqLocStop(slpb)+1) {
                    sint = (SeqIntPtr) slpa->data.ptrvalue;
                    sint->from = SeqLocStart(slpb);
                    ObjMgrDeSelect(eIDb, iIDb, itb, sspb->regiontype, slpb);
                    check = TRUE;
                    loopin = FALSE;
                    changed = TRUE;
                    break;
                 }
              }
           }
        }
        if (loopin && sspa != NULL)
           sspa = sspa->next;
     }
  }
  return changed;
}

NLM_EXTERN void ObjMgrSelectPrint (void)
{
  SelStructPtr ssp = NULL;
  FILE *fout;
  Char    strLog[128];

  fout = FileOpen("LogFile", "a");
  if (fout==NULL) 
     return;

  fprintf (fout, "ObjMgrSelectPrint\n");
  ssp = ObjMgrGetSelected();  
  for (; ssp != NULL; ssp = ssp->next) 
  {
    if (ssp->regiontype == OM_REGION_SEQLOC) {
     SeqIdWrite(SeqLocId((SeqLocPtr)ssp->region),strLog,PRINTID_FASTA_LONG,120);
     if ( ssp->region != NULL ) {
        fprintf (fout, "selstruc  %d %d %d  %s %ld %ld \n", (int)ssp->entityID, (int)ssp->itemID, (int)ssp->itemtype, strLog, (long)SeqLocStart((SeqLocPtr)ssp->region), (long)SeqLocStop((SeqLocPtr)ssp->region));
     }
     else 
        fprintf (fout, "selstruc %d %d %d region=NULL\n", (int)ssp->entityID, (int)ssp->itemID, (int)ssp->itemtype);
    }
    else 
     fprintf (fout, "selstruc %d %d %d regiontype=0\n", (int)ssp->entityID, (int)ssp->itemID, (int)ssp->itemtype);
  }
  fprintf (fout, "\n");
  FileClose(fout);
}

/******************************************************************/
NLM_EXTERN void SelectType (EditAlignDataPtr adp, Uint2 feattype, Int4 slpto)
{
  ValNodePtr       vnp;
  SeqLocPtr        slp;
  SelStructPtr     ssp;
  Uint2            ei, ii, it;
  AlignNodePtr     anp;
  Boolean          first = TRUE;
 
  if (adp == NULL) return;
  if (adp->anp_list == NULL) return;
  for (vnp =adp->anp_list; vnp !=NULL; vnp =vnp->next)
  {
     anp = (AlignNodePtr) vnp->data.ptrvalue;
     if (anp != NULL)
     {
           ei = anp->seq_entityID;
           ii = anp->bsp_itemID;
           it = feattype;
           ssp = is_selectedbyID (ei, ii, it);
           if (ssp == NULL)
           {
              slp = SeqLocIntNew (0, slpto, Seq_strand_plus, anp->sip);
              if (first) {
                 ObjMgrSelect (ei, ii, it, OM_REGION_SEQLOC, slp);
                 first = FALSE;
              }
              else
                 ObjMgrAlsoSelect (ei, ii, it, OM_REGION_SEQLOC, slp);
           }
     }
  }
  return;
}

/******************************************************************/
NLM_EXTERN Int2 GetNumberObjMgrSelect (void)
{
  SelStructPtr ssp = NULL;
  Int2         nselect = 0;

  ssp = ObjMgrGetSelected ();
  for (; ssp != NULL; ssp = ssp->next) 
     if (checkssp_for_editor(ssp)) nselect++;
  return nselect;
}

/******************************************************************/
NLM_EXTERN Int2 checkOMss_for_itemtype (Uint2 itemtype)
{
  SelStructPtr ssp = NULL;
  Int2         nselect = 0;

  ssp = ObjMgrGetSelected ();
  for (; ssp != NULL; ssp = ssp->next) 
     if (ssp->itemtype == itemtype && checkssp_for_editor (ssp)) 
        nselect++;
  return nselect;
}

/******************************************************************/
NLM_EXTERN SelStructPtr getOMselect_for_itemtype (Uint2 itemtype)
{
  SelStructPtr ssp = NULL;

  ssp = ObjMgrGetSelected ();
  for (; ssp != NULL; ssp = ssp->next) 
     if (ssp->itemtype == itemtype && checkssp_for_editor (ssp)) 
        break;
  return ssp;
}

/******************************************************************/
NLM_EXTERN SelStructPtr is_selectedbyID (Uint2 entityID, Uint2 itemID, Uint2 itemtype)
{
  SelStructPtr ssp = NULL;

  ssp = ObjMgrGetSelected();  
  for (; ssp != NULL; ssp = ssp->next) 
  {
     if ( is_sameId (ssp->entityID, ssp->itemID, ssp->itemtype, 255, entityID, itemID, itemtype, 255) ) 
     {
        break;
     }
  }
  return ssp;
}

NLM_EXTERN SelEdStructPtr getCDSselect (ValNodePtr seqfeathead, ValNodePtr feathead)
{
  SelStructPtr   ssp = NULL;
  SelEdStructPtr feat = NULL;

  ssp = ObjMgrGetSelected ();
  for (; ssp != NULL; ssp = ssp->next) if (checkssp_for_editor (ssp)) {
     if ( ssp->itemtype == OBJ_SEQFEAT ) 
     {
        feat = get_feat_fromid (seqfeathead, FEATDEF_CDS, ssp->entityID, ssp->itemID, -1, NULL);
        if (feat != NULL) break;
     }
     else if ( ssp->itemtype == OBJ_VIRT ) 
     {
        feat = get_feat_fromid (feathead, SEQFEAT_CDREGION, ssp->entityID, ssp->itemID, -1, NULL);
        if (feat != NULL) break;
     }
  }
  return feat;
}

NLM_EXTERN Int2 checkCDSselect_forprotein (ValNodePtr seqfeathead, ValNodePtr feathead, Boolean with_prot)
{
  SelStructPtr   ssp = NULL;
  SelEdStructPtr feat = NULL;
  Int2           nselect = 0;

  ssp = ObjMgrGetSelected ();
  for (; ssp != NULL; ssp = ssp->next) if (checkssp_for_editor (ssp)) {
     if ( ssp->itemtype == OBJ_SEQFEAT ) 
     {
        feat = get_feat_fromid (seqfeathead, FEATDEF_CDS, ssp->entityID, ssp->itemID, -1, NULL);
        if (feat != NULL)
        {
           if (with_prot && feat->data != NULL) {
               nselect++;
           } else if (!with_prot && feat->data == NULL) {
               nselect++;
           }
        }
     }
     else if ( ssp->itemtype == OBJ_VIRT ) 
     {
        feat = get_feat_fromid (feathead, SEQFEAT_CDREGION, ssp->entityID, ssp->itemID, -1, NULL);
        if (feat != NULL)
           if (with_prot && feat->data != NULL) 
              nselect++;
           else if (!with_prot && feat->data == NULL) 
              nselect++;
     }
  }
  return nselect;
}


/******************************************************************
***   checkssp_for_editor
******************************************************************/
NLM_EXTERN Boolean checkssp_for_editor (SelStructPtr ssp)
{
  if (ssp == NULL) return FALSE;
  if (ssp->regiontype == 0) return FALSE;
  if (ssp->region == NULL) {
         ssp->regiontype = 0;
         return FALSE;
  }
  return TRUE;
}

/***********************************************************
***
***
************************************************************/
NLM_EXTERN SeqLocPtr checkseqlocfeature_for_editor (Uint2 entityID, Uint2 itemID, ValNodePtr headfeat)
{
  SelEdStructPtr feat;
  SeqLocPtr      slp = NULL,
                 tmp;
  SeqIdPtr       sip;
  Int4           start, stop;
  Uint1          strand;

  if (headfeat == NULL) 
     return NULL;
  feat = get_feat_fromid (headfeat, 255, entityID, itemID, -1, NULL);
  if (feat != NULL) 
  {
     tmp = (SeqLocPtr) feat->region;
     sip = SeqLocId (tmp);
     strand = SeqLocStrand (tmp);
     start = SeqLocStart (tmp);
     if (feat->next != NULL)
        while (feat->next != NULL) feat = feat->next;
     stop = SeqLocStop ((SeqLocPtr) feat->region);
     slp = SeqLocIntNew (start, stop, strand, sip);
  }
  return slp;
}

NLM_EXTERN void checkselectsequinfeature_for_editor (ValNodePtr headfeat)
{
  SelStructPtr   ssp;
  SelEdStructPtr feat;
  SeqLocPtr      slp, tmp;
  SeqIdPtr       sip;
  Int4           start, stop;
  Int2           k = 0;
  Uint1          strand;

  if (headfeat == NULL) return;
  for (ssp = ObjMgrGetSelected (); ssp != NULL; ssp = ssp->next) 
          if (ssp->itemtype == OBJ_SEQFEAT) k++;
  if (k > 0) {
     ssp = ObjMgrGetSelected ();
     for (; ssp != NULL; ssp = ssp->next) {
        if (ssp->itemtype == OBJ_SEQFEAT) 
        {
           if (ssp->regiontype != OM_REGION_SEQLOC || ssp->region == NULL)
           {
              feat = get_feat_fromid (headfeat, 255, ssp->entityID, ssp->itemID, -1, NULL);
              if (feat != NULL) {
                 tmp = (SeqLocPtr) feat->region;
                 sip = SeqLocId (tmp);
                 strand = SeqLocStrand (tmp);
                 start = SeqLocStart (tmp);
                 if (feat->next != NULL)
                    while (feat->next != NULL) feat = feat->next;
                 stop = SeqLocStop ((SeqLocPtr) feat->region);
                 slp = SeqLocIntNew (start, stop, strand, sip);
                 ssp->regiontype = OM_REGION_SEQLOC;
                 ssp->region = slp;
              }
           }
        } 
     }
  }
}

NLM_EXTERN Int4 getminpos_fromOMselect (Uint2 itemsubtype)
{
  SelStructPtr   ssp;
  Int4           minpos = INT4_MAX;
  Int4           from;

  ssp = ObjMgrGetSelected ();
  for (; ssp != NULL; ssp = ssp->next) 
  {
     if (ssp->itemtype == itemsubtype) 
     {
        if (ssp->regiontype == OM_REGION_SEQLOC && ssp->region != NULL)
        {
            from = SeqLocStart ((SeqLocPtr)ssp->region);
            if (from < minpos) minpos = from;
        } 
     }
  }
  if ( minpos < INT4_MAX ) return minpos;
  return -1;
}

/***********************************************
***  locate_in_seqalign   
***    in : pos in Align coordinates
***    out: seen TRUE if pos in salp
************************************************/
NLM_EXTERN Boolean locate_in_seqalign (Int4 pos, Int2 dim, Int2 dspnumseg, BoolPtr *dspstart, Int4Ptr *dsplens, Int2 *numseg_before, Int2 *subdsplens, Int4 *sumdsplens_before)
{
  BoolPtr     start = *dspstart;
  Int4Ptr     lens  = *dsplens;
  Int4        sumlens= 0;
  Int4        sumlensseq= 0;
  Int2        numseg = 0;
  Int2        sublens;
  Boolean     seen = FALSE;

  if ( dspnumseg == 0 || start == NULL || lens == NULL ) {
         ErrPostEx(SEV_WARNING, 0, 0, "fail in locate_in_seqalign [1]\n");
         return FALSE;
  }
  while ( !seen && numseg < dspnumseg ) {
         numseg++;
         if ( pos  >= sumlens && pos < sumlens + *lens ) {
                sublens = abs (pos - sumlens);
                seen = TRUE;
         }
         else {
                if ((Boolean)(*start)) sumlensseq += *lens;
                if ( numseg == dspnumseg ) break;
                start += dim; 
                sumlens += *lens;
                lens++;
         }
  }
  if ( seen )
  {
        *dspstart  = start;
        *dsplens   = lens;
        *numseg_before = numseg;
        *subdsplens= sublens;
        *sumdsplens_before= sumlensseq;
  }
  return seen;
}

/************************************
*** SeqCoordToAlignCoord
**
************************************/
NLM_EXTERN Int4 SeqCoordToAlignCoord (Int4 position, SeqIdPtr sip, SeqAlignPtr salp, Int2 intersalpwidth, Int2 is_end)
{
  CompSegPtr  dsp;
  BoolPtr     dspstart;
  Int4Ptr     dsplens;
  Int4        from;
  Int4        sumlens = 0;
  Int4        seqlens = 0;
  Int4        lensplus;
  Int4        start, stop;
  Int2        numseg;
  Int2        inter_salp = 0;
  Int2        index;
  Uint1       dspstrand = Seq_strand_unknown;
  Boolean     seen = FALSE;

  if (is_end == NO_RESIDUE)
     return position;
  if (position < 0)
     return position;

  dsp = (CompSegPtr) salp->segs;
  if (dsp == NULL) {
     return GAP_RESIDUE;
  }
  index = SeqIdOrderInBioseqIdList (sip, dsp->ids);
  if (index < 1) {
     return GAP_RESIDUE;
  }
  index -= 1;
  from = *(dsp->from + index);
  if (is_end == GAP_RESIDUE)
     position = from;
  dspstart = dsp->starts + index;
  dsplens = dsp->lens;
  if (dspstart == NULL || dsplens == NULL ) {
     return GAP_RESIDUE;
  }
  if (dsp->strands!=NULL)
     dspstrand = *(dsp->strands + index);
  
  numseg = 1;
  while ( !seen && numseg <= dsp->numseg ) 
  {
     if (dspstrand ==Seq_strand_minus) {
        start= from - seqlens - *dsplens;
        stop = from - seqlens;
     } else {
        start= from + seqlens;
        stop = from + seqlens + *dsplens;
     }
     if (*dspstart && position >= start && position < stop) 
     {
        if (dspstrand ==Seq_strand_minus)
           lensplus = abs (from + seqlens - position);
        else
           lensplus = abs (position - from - seqlens);
        seen = TRUE;
     }
     else if (*dspstart && position <= stop 
          /***/ && is_end == APPEND_RESIDUE ) /***!!!!!!!!!**********/
     {
           if (dspstrand ==Seq_strand_minus)
              lensplus = abs (from + seqlens - position);
           else
              lensplus = abs (position - from - seqlens);
           seen = TRUE;
     }
     else if ( numseg == dsp->numseg ) 
     {
        if ( salp->next == NULL ) break; 
        else 
        { 
           salp = salp->next;
           dsp = (CompSegPtr) salp->segs;
           from = *(dsp->from + index);
           dspstart = dsp->starts + index;
           dsplens = dsp->lens;
           inter_salp++;
           numseg = 1;
        }
     }
     else if (numseg < dsp->numseg) 
     {
           sumlens += *dsplens;
           if (*dspstart) 
              seqlens += *dsplens;
           dspstart += dsp->dim; 
           dsplens++;
     }
     numseg++;
  }
  if ( !seen ) {
     if (!(*dspstart))    /***** if after sequence 2 mais seq1 last segment***/
         return seqlens; 
     return GAP_RESIDUE;
  }
  if (position == APPEND_RESIDUE)
     return position;
  position = sumlens + lensplus + intersalpwidth*inter_salp;
  return position;
}

/************************************
*** AlignCoordToSeqCoord
************************************/
NLM_EXTERN Int4 AlignCoordToSeqCoord (Int4 position, SeqIdPtr sip, SeqAlignPtr salp,ValNodePtr sqloc_list, Int2 intersalpwidth)
{
  CompSegPtr  dsp;
  BoolPtr     dspstart;
  Int4Ptr     dsplens;
  Int4        from;
  Int4        sumlens = 0;
  Int4        sumstart = 0;
  Int4        seqlens = 0;
  Int2        numseg = 0;
  Int2        inter_salp = 0;
  Int2        index;
  Uint1       dspstrand = Seq_strand_unknown;
  Boolean     seen = FALSE;

  if (position == APPEND_RESIDUE)
     return position;
  dsp = (CompSegPtr) salp->segs;
  if (dsp == NULL) 
     return (Int4)GAP_RESIDUE;
  index = SeqIdOrderInBioseqIdList (sip, dsp->ids);
  if (index < 1) 
     return (Int4)GAP_RESIDUE;
  index -= 1;
  from = *(dsp->from + index);
/*
  if (position <= from) {
     return from;
  }
*/
  dspstart = dsp->starts + index;
  dsplens = dsp->lens;
  if (dspstart == NULL || dsplens == NULL ) {
     return (Int4)GAP_RESIDUE;
  }
  if (!(*dspstart) && (position < *dsplens)) {
     return (Int4)GAP_RESIDUE;
  }
  if (dsp->strands!=NULL)
     dspstrand = *(dsp->strands + index);
  numseg = 0;
  sumlens = 0;
  for (; dsplens != NULL && numseg < dsp->numseg; dsplens++, numseg++) 
     sumlens += *dsplens;
  dsplens = dsp->lens; 
  if (position >= sumlens) {     
     sumlens = 0;
     numseg = 0;
     for (; dsplens != NULL && numseg < dsp->numseg; dsplens++, numseg++) {
        if (*dspstart)  sumlens += *dsplens;
        dspstart += dsp->dim; 
     }
    seen = TRUE;
     if (dspstrand == Seq_strand_minus) {
        position = from - sumlens;
     } else {
        position = from + sumlens;
     }
  }
  else {
        sumlens = 0;
        numseg = 0;
        while ( !seen && numseg < dsp->numseg ) {
            numseg++;
            if (position >=sumlens && position <sumlens +*dsplens ) {
               if (*dspstart) 
                  seqlens += abs (position - sumlens);
               seen = TRUE;
            }
            else if ( numseg == dsp->numseg ) 
            {
               if ( salp->next == NULL ) break; 
               else 
               { 
                  sumstart += sumlens + *dsplens;
                  salp = salp->next;
                  dsp = (CompSegPtr) salp->segs;
                  from = *(dsp->from + index);
                  dspstart = dsp->starts + index;
                  dsplens = dsp->lens;
                  inter_salp++;
                  numseg = 0;
               }
            }
            else 
            {
               if ( *dspstart ) 
                  seqlens += *dsplens;
               sumlens += *dsplens;
               dspstart += dsp->dim; 
               dsplens++;
            }
        }
        if (seen) {
           if (dspstrand == Seq_strand_minus) {
              position = from - seqlens - sumstart;
           } else {
              position = from + seqlens - sumstart;
           }
     }
  }
  if ( !seen ) { 
     return (Int4)GAP_RESIDUE;
  }
  index =chkloc (sip, position, sqloc_list, &from);
  if (index == GAP_RESIDUE || index == APPEND_RESIDUE) {
     return (Int4)index;
  }
  return position;
}

NLM_EXTERN Int4 AlignCoordToSeqCoord2 (Int4 position, SeqIdPtr sip, SeqAlignPtr salp,ValNodePtr sqloc_list, Int2 intersalpwidth)
{
  CompSegPtr  dsp;
  BoolPtr     dspstart;
  Int4Ptr     dsplens;
  Int4        from;
  Int4        sumlens = 0;
  Int4        sumstart = 0;
  Int4        seqlens = 0;
  Int2        numseg = 0;
  Int2        inter_salp = 0;
  Int2        index;
  Uint1       dspstrand = Seq_strand_unknown;
  Boolean     seen = FALSE;

  if (position == APPEND_RESIDUE)
     return position;
  dsp = (CompSegPtr) salp->segs;
  if (dsp == NULL) 
     return (Int4)GAP_RESIDUE;
  index = SeqIdOrderInBioseqIdList (sip, dsp->ids);
  if (index < 1) 
     return (Int4)GAP_RESIDUE;
  index -= 1;
  from = *(dsp->from + index);
/*
  if (position <= from) {
     return from;
  }
*/
  dspstart = dsp->starts + index;
  dsplens = dsp->lens;
  if (dspstart == NULL || dsplens == NULL ) {
     return (Int4)GAP_RESIDUE;
  }
  if (!(*dspstart) && (position < *dsplens)) {
     return (Int4)GAP_RESIDUE;
  }
  if (dsp->strands!=NULL)
     dspstrand = *(dsp->strands + index);
  numseg = 0;
  sumlens = 0;
  for (; dsplens != NULL && numseg < dsp->numseg; dsplens++, numseg++) 
     sumlens += *dsplens;
  dsplens = dsp->lens; 
  if (position >= sumlens) {     
     position = (Int4)APPEND_RESIDUE;
/*****
     sumlens = 0;
     numseg = 0;
     for (; dsplens != NULL && numseg < dsp->numseg; dsplens++, numseg++) {
        if (*dspstart)  sumlens += *dsplens;
        dspstart += dsp->dim; 
     }
    seen = TRUE;
     if (dspstrand == Seq_strand_minus) {
        position = from - sumlens;
     } else {
        position = from + sumlens;
     }
****/
  }
  else {
        sumlens = 0;
        numseg = 0;
        while ( !seen && numseg < dsp->numseg ) {
            numseg++;
            if (position >=sumlens && position <sumlens +*dsplens ) {
               if (*dspstart) 
                  seqlens += abs (position - sumlens);
               seen = TRUE;
            }
            else if ( numseg == dsp->numseg ) 
            {
               if ( salp->next == NULL ) break; 
               else 
               { 
                  sumstart += sumlens + *dsplens;
                  salp = salp->next;
                  dsp = (CompSegPtr) salp->segs;
                  from = *(dsp->from + index);
                  dspstart = dsp->starts + index;
                  dsplens = dsp->lens;
                  inter_salp++;
                  numseg = 0;
               }
            }
            else 
            {
               if ( *dspstart ) 
                  seqlens += *dsplens;
               sumlens += *dsplens;
               dspstart += dsp->dim; 
               dsplens++;
            }
        }
        if (seen) {
           if (dspstrand == Seq_strand_minus) {
              position = from - seqlens - sumstart;
           } else {
              if (!(*dspstart)) {
                 position =chkloc (sip, position, sqloc_list, &from);
                 if (position==0) 
                    position = (Int4)GAP_RESIDUE;
              }
              else
                 position = from + seqlens - sumstart;
           }
     }
  }
  if ( !seen ) { 
     position =chkloc (sip, position, sqloc_list, &from);
  }
  return position;
}


NLM_EXTERN Boolean GetAlignCoordFromSeqLoc (SeqLocPtr slp, SeqAlignPtr salp, Int4 *start, Int4 *stop)
{
  SeqIdPtr         sip;
  Int4             from, to;
 
  sip = SeqLocId (slp);
  from = SeqLocStart (slp);
  from = SeqCoordToAlignCoord (from, sip, salp, 0, 0);
  to = SeqLocStop (slp);
  to = SeqCoordToAlignCoord (to, sip, salp, 0, 0);
  if (from < 0 || to < 0)
     return FALSE;
  *start = from;
  *stop = to;
  return TRUE;
}
 
/************************************************************************
***
***   SeqPosToLineColumn
***
************************************************************************/
NLM_EXTERN Boolean  SeqPosToLineColumn (Uint2 itemID, Uint2 entityID, Uint2 itemtype, 
           Int4 pos, Int4 *line, Int4 *column, Int4 hoffset, EditAlignDataPtr adp)
{
  Boolean seen = FALSE;
  Int4    from, to;
  Int2    col = 0;
  Int4    length;
  Int2    j;

  *line = -1;
  *column = -1;
  if ( pos < 0 ) 
         return FALSE;
  hoffset -= adp->bufferstart;
  length = adp->length - adp->bufferstart;
  for ( j=0; !seen && j < MAXLineWindow; j++ )
  {
         if (itemID == adp->item_id[j] && entityID == adp->seqEntity_id[j] && itemtype == adp->itemtype[j] ) 
         { 
                from = hoffset + adp->alignline [j] * adp->visibleWidth;
                if (from > length) break;
                to = hoffset + (adp->alignline [j]+1) * adp->visibleWidth -1;
                if (to > length) to = length;
/**
                WriteLog ("SeqPosToLineColumn %d %ld %ld %ld %d %d %d %d\n", j,  
                       pos, from, to, adp->item_id[j], adp->seqEntity_id[j], 
                       adp->colonne[from], adp->colonne[to]);
**/
                if( pos >= adp->colonne[from]  && pos <= adp->colonne[to]) 
                {
                       col = (Int2)(pos - adp->colonne[from]);
                       if (adp->columnpcell > 0)
                          col += (Int2)(col) / (Int2)(adp->columnpcell);
                       seen = TRUE;
                       break;
                }
         }
  }
  if ( !seen || j == MAXLineWindow ) 
         return FALSE;
  *line = j;
  *column = col;
  return TRUE;
}

/************************************************************************
***
***   SeqPosInLineColumn
***
************************************************************************/
NLM_EXTERN Boolean  SeqPosInLineColumn (Int4 pos, Int4 alignline, Int4 *column, 
           Int4 hoffset, EditAlignDataPtr adp)
{
  Int4    from, to;
  Int4    length;
  Int4    col = 0;

  if ( pos < 0 )  return FALSE;
  hoffset -= adp->bufferstart;
  length = adp->length - adp->bufferstart;
  from = hoffset + alignline * adp->visibleWidth;
  to = hoffset + (alignline +1) * adp->visibleWidth - 1; 
  if (from > length) return FALSE;
  if (to > length) to = length;

  if ( pos < adp->colonne[from] ) 
  {
         *column = -1;
         return FALSE;
  }
  if ( pos > adp->colonne[to] ) 
  {
         *column = -2;
         return FALSE;
  }
  col = pos - adp->colonne[from];
  if (adp->columnpcell > 0)
         col += (Int4) col / (Int4) adp->columnpcell;
  *column = (Int2) col;
  return TRUE;
}
 
/***********************************************
***  LocateInSeqAlign
***    in : pos in Align coordinates
***    out: seen TRUE if pos in salp
************************************************/
NLM_EXTERN Boolean LocateInSeqAlign (Int4 pos, Int2 dim, Int2 dspnumseg, BoolPtr *dspstart, Int4Ptr *dsplens, Int2 *numseg_before, Int4 *subdsplens, Int4 *sumdsplens_before)
{
  BoolPtr     start = *dspstart;
  Int4Ptr     lens  = *dsplens;
  Int4        sumlens= 0;
  Int4        sumlensseq= 0;
  Int2        numseg = 0;
  Int4        sublens;
  Boolean     seen = FALSE;
 
  if ( dspnumseg == 0 || start == NULL || lens == NULL ) {
         ErrPostEx(SEV_WARNING, 0, 0, "fail in LocateInSeqAlign [1]\n");
         return FALSE;
  }
  while ( !seen && numseg < dspnumseg ) {
         numseg++;
         if ( pos  >= sumlens && pos < sumlens + *lens ) {
                sublens = abs (pos - sumlens);
                seen = TRUE;
         }
         else {
                if ((Boolean)(*start)) sumlensseq += *lens;
                if ( numseg == dspnumseg ) break;
                start += dim;
                sumlens += *lens;
                lens++;
         }
  }
  if ( seen )
  {
        *dspstart  = start;
        *dsplens   = lens;
        *numseg_before = numseg;
        *subdsplens= sublens;
        *sumdsplens_before= sumlensseq;
  }
  return seen;
}


/***********************************************
***  LocateInSeqAlign
***    in : pos in Align coordinates
***    out: seen TRUE if pos in salp
************************************************/
NLM_EXTERN Boolean LocateInSeqAlignDenSeg (Int4 pos, Int2 dim, Int2 dspnumseg, Int4Ptr *dspstart, Int4Ptr *dsplens, Int2 *numseg_before, Int4 *subdsplens)
{
  Int4Ptr     start = *dspstart;
  Int4Ptr     lens  = *dsplens;
  Int4        sumlens= 0;
  Int2        numseg = 0;
  Int4        sublens= 0;
  Boolean     seen = FALSE;
 
  if ( dspnumseg == 0 || start == NULL || lens == NULL ) {
         ErrPostEx(SEV_WARNING, 0, 0, "fail in LocateInSeqAlignDenSeg [1]");
         return FALSE;
  }
  if (pos == -1 || *start == -1) 
  {
     numseg = 1;      /*!!!!!!!!!!!!!!!!!!!!!*/
     seen = TRUE; 
  }
  else {
     sumlens = *start;
     while ( !seen && numseg < dspnumseg ) {
         numseg++;
         if ( pos  >= sumlens && pos < sumlens + *lens ) {
                sublens = abs (pos - sumlens);
                seen = TRUE;
         }
         else {
                if ( numseg == dspnumseg ) break;
                start += dim;
                sumlens += *lens;
                lens++;
         }
     }
  }
  if ( seen )
  {
        *dspstart  = start;
        *dsplens   = lens;
        *numseg_before = numseg;
        *subdsplens= sublens;
  }
  return seen;
}
 
/*******************************************************
***
***   FIND functions
***
*******************************************************/
NLM_EXTERN SelStructPtr SetupMatchPatList (ValNodePtr match_pat, ValNodePtr anp_list)
{
  SelStructPtr    head = NULL;
  SelStructPtr    ssp,
                  tmp;
  SeqLocPtr       slp;
  AlignNodePtr    anp;
  ValNodePtr      vnp, vnp2;

  if (match_pat == NULL || anp_list == NULL)
     return NULL;
  for (vnp2 = match_pat; vnp2!=NULL; vnp2=vnp2->next)
  {
     slp = (SeqLocPtr) vnp2->data.ptrvalue;
     if (slp != NULL) {
        for (vnp = anp_list; vnp != NULL; vnp = vnp->next) {
           anp = (AlignNodePtr) vnp->data.ptrvalue;
           if (SeqIdForSameBioseq(anp->sip, SeqLocId(slp)))
              break;
        }
        if (vnp != NULL) 
        {
           ssp = SelStructNew (anp->seq_entityID, anp->bsp_itemID, OBJ_BIOSEQ, SeqLocStart(slp), SeqLocStop(slp), SeqLocId(slp), Seq_strand_plus, FALSE);
           if (head == NULL)
              head = ssp;
           else {
              for (tmp=head; tmp->next!=NULL; tmp=tmp->next)
                 continue;
              tmp->next = ssp;
              ssp->prev = tmp;
           }
        }
     }
  }
  ValNodeFreeType (&match_pat, TypeSeqLoc);
  return head;
}

NLM_EXTERN SelStructPtr ShowNextPattern (SelStructPtr match_pat, SelStructPtr cur_pat, Int4 start)
{
  SeqLocPtr     slp;
  SelStructPtr  cur;
  Int4          from;
 
  if (match_pat == NULL) 
     return NULL; 
  if (cur_pat == NULL) {
     cur_pat = match_pat;
  }
  else if (cur_pat->next == NULL) {
     if (cur_pat == match_pat) 
        Beep();
     else cur_pat = match_pat;
  }
  else {
     cur_pat = cur_pat->next;
  }
  if (start > 0 && cur_pat != NULL) {
     from = SeqLocStart((SeqLocPtr)cur_pat->region);
     if (from < start) {
        for (cur = cur_pat; cur != NULL; cur = cur->next) {
           from = SeqLocStart((SeqLocPtr)cur->region);
           if (from > start)
              break;
        }
        if (cur != NULL)
           cur_pat = cur;
     }
  }
  if (cur_pat != NULL) {
     if (cur_pat->regiontype == OM_REGION_SEQLOC && cur_pat->region != NULL) {
        slp = (SeqLocPtr)AsnIoMemCopy (cur_pat->region, (AsnReadFunc) SeqLocAsnRead, (AsnWriteFunc) SeqLocAsnWrite);
        ObjMgrSelect(cur_pat->entityID, cur_pat->itemID, OBJ_BIOSEQ, OM_REGION_SEQLOC, (Pointer)slp);
     }
  }
  return cur_pat;
}

NLM_EXTERN SelStructPtr ShowPrecPattern (SelStructPtr match_pat, SelStructPtr cur_pat, Int4 start)
{
  SelStructPtr  tmp;
  SeqLocPtr     slp;
 
  if (match_pat == NULL) 
     return NULL; 
  if (cur_pat == NULL) {
     cur_pat = match_pat;
  }
  else if (cur_pat->prev == NULL) {
     for (tmp=match_pat; tmp->next!=NULL; tmp=tmp->next) 
        continue;
     cur_pat = tmp;
     if (cur_pat == match_pat) 
        Beep();
  }
  else {
     cur_pat = cur_pat->prev;
  }
  if (cur_pat != NULL) {
     if (cur_pat->regiontype == OM_REGION_SEQLOC && cur_pat->region != NULL) {
        slp = (SeqLocPtr)AsnIoMemCopy (cur_pat->region, (AsnReadFunc) SeqLocAsnRead, (AsnWriteFunc) SeqLocAsnWrite);
        ObjMgrSelect(cur_pat->entityID, cur_pat->itemID, OBJ_BIOSEQ, OM_REGION_SEQLOC, (Pointer)slp);
     }
  }
  return cur_pat;
}

/**************************************
***  
***  EDITOR Procedure
***  
***************************************/
static Char insertstr[] = "Insertboard";

NLM_EXTERN Boolean insertchar (CharPtr str, Int4 pos, SeqIdPtr target, Uint1 mol_type, Boolean spliteditmode)
{
  SeqEntryPtr      sep;
  Int4             strlens;
  SeqId            insertboard;
  ObjectId         oip;
  Boolean          ok = FALSE;

  if ( str == NULL ) 
     return FALSE;
  strlens = StringLen (str);
  if (strlens < 1) 
     return FALSE;
  if (pos < -2 || target == NULL) 
     return FALSE; 
  insertboard.choice = SEQID_LOCAL;
  oip.str = insertstr;
  oip.id = 0;
  insertboard.data.ptrvalue = (ObjectIdPtr) &oip; 
  insertboard.next = NULL;
  sep = StringToSeqEntry (str, &insertboard, strlens, mol_type);
  if (sep != NULL) {
     ok = BioseqInsert(&insertboard, FIRST_RESIDUE, LAST_RESIDUE, Seq_strand_plus, target, pos, TRUE, TRUE, spliteditmode);
     SeqEntryFree (sep);
  }
  else 
     ErrPostEx(SEV_WARNING, 0, 0,  "fail in insertchar [2]");
  return ok;
}

NLM_EXTERN Boolean insertchar_atcaret (CharPtr str, EditAlignDataPtr adp)
{
  SeqLocPtr        slpcaret;
  Int4             pos;
  Int4             strlens;

  if ( str == NULL ) 
     return FALSE;
  strlens = StringLen (str);
  if (strlens < 1) 
     return FALSE;
  if ( adp->caret.regiontype == 0 ) 
     return FALSE;
  slpcaret = (SeqLocPtr) adp->caret.region;
  pos = SeqLocStart (slpcaret); 
  if ( pos == adp->length ) 
     pos = -2;
  return (Boolean) insertchar (str, pos, SeqLocId (slpcaret), adp->mol_type, adp->spliteditmode);
}

NLM_EXTERN CharPtr char_to_insert (Char *ch, Int4 lens, Uint1 mol_type)
{
  CharPtr      str, strp;
  Int4         j;

  str = (CharPtr)MemNew ((size_t)((lens+2)*sizeof(Char)));
  for (j=0, strp = ch; j<lens && *strp!='\0'; j++, strp++)
     str[j] = *strp;
  str[j] = '\0';
  lens = StringLen (str);

  if ( ISA_na (mol_type)) 
  {
     for (j=0, strp = str; j<lens; j++, strp++) {
        *strp = TO_LOWER(*strp);
        if ( StringChr ("abcdghkmnrstuvwy", *strp) == NULL ) {
           MemFree (str);
           return NULL;
        }
     }
  }
  else if ( ISA_aa (mol_type) )
  {
     for (j=0, strp = str; j<lens; j++, strp++) {
        *strp = TO_UPPER(*strp);
        if ( StringChr ("ABCDEFGHIKLMNPQRSTUVWXYZ-*", *strp) == NULL ) {
           MemFree (str);
           return NULL;
        }
     }
  }
  else {
     for (j=0, strp = str; j<lens; j++, strp++) {
        *strp = TO_UPPER(*strp);
     }
  }
  return str;
}


NLM_EXTERN SeqEntryPtr getfirst_sep(SeqEntryPtr sep, Uint1 bsp_mol)
{
  SeqEntryPtr  septmp = NULL;
  SeqEntryPtr  sep2 = NULL;
  BioseqSetPtr bssp;
  BioseqPtr    bsp;
 
  if (sep == NULL) return NULL;
  if (IS_Bioseq_set(sep)) {
         bssp = (BioseqSetPtr)sep->data.ptrvalue;
         if (bssp == NULL)
                return NULL;
         septmp = (SeqEntryPtr) bssp->seq_set;
         if (septmp == NULL)
                return NULL;
  }
  else if (IS_Bioseq(sep))
     septmp = sep;
 
  while ( septmp != NULL ) {
     if  (IS_Bioseq_set(septmp)) {
        bssp = (BioseqSetPtr) septmp->data.ptrvalue;
        for (sep2 = bssp->seq_set; sep2 != NULL; sep2 = sep2->next) {
           if (IS_Bioseq(sep2)) {
              bsp = (BioseqPtr) sep2->data.ptrvalue;
              if (bsp!=NULL && bsp->id!=NULL) {
                 if (ISA_na(bsp->mol) == ISA_na(bsp_mol)) {
                    return sep2;
                 }
              }   
           }   
        }
     }   
     else if (IS_Bioseq(septmp)) {
        bsp = (BioseqPtr) septmp->data.ptrvalue;
        if (bsp!=NULL && bsp->id!=NULL)
        {
           if (ISA_na(bsp->mol) == ISA_na(bsp_mol)) {
              return septmp;
           }
        }
     }
     septmp = septmp->next;
  }   
  return NULL;
}


/*
static SeqAnnotPtr SeqAnnotForHistSeqAlign (SeqAlignPtr salp)
{
  SeqAnnotPtr      sap = NULL;
  ObjectIdPtr      oip;
  UserFieldPtr     ufp;
  UserObjectPtr    uop;

  sap = SeqAnnotForSeqAlign (salp);
  if (sap != NULL && sap->type == 2) 
  {
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
  }
  return sap;
}

static void AttachSeqAligntoBioseq (Uint2 entityID, SeqAlignPtr salp)
{
  SeqEntryPtr      sep = NULL;
  SeqAnnotPtr      sap = NULL, tmp;
  BioseqSetPtr     bssp= NULL;
  BioseqPtr        bsp = NULL;

  sep = GetTopSeqEntryForEntityID (entityID);
  if (sep!=NULL) {
     if (IS_Bioseq(sep))
        bsp = (BioseqPtr) sep->data.ptrvalue;
     else if (IS_Bioseq_set(sep)) { 
        bssp = (BioseqSetPtr) sep->data.ptrvalue;
        while (bssp!=NULL && bssp->_class == 7) {
           sep = bssp->seq_set;
           bssp = NULL;
           if (IS_Bioseq(sep))  {
              bsp = (BioseqPtr) sep->data.ptrvalue;  
              break;
           }
           else if (IS_Bioseq_set(sep))
              bssp = (BioseqSetPtr) sep->data.ptrvalue;
        }
     }
     if (bssp!=NULL) {
        if (bssp->annot==NULL) {
           sap = SeqAnnotForHistSeqAlign (salp);
           bssp->annot = sap;
        } else {
           for (tmp=bssp->annot;tmp->next!= NULL; tmp=tmp->next) 
              continue;
           sap = SeqAnnotForHistSeqAlign (salp);
           tmp->next = sap;
        }
     }
     else if (bsp != NULL) { 
        if (bsp->annot==NULL) {
           sap = SeqAnnotForHistSeqAlign (salp);
           bsp->annot = sap;  
        } else { 
           for (tmp=bsp->annot;tmp->next!= NULL; tmp=tmp->next)
              continue;   
           sap = SeqAnnotForHistSeqAlign (salp);
           tmp->next = sap;
        }
     }  
  }
}
*/
/*  
static Uint2 GetEntityIDForBioseqSet (void)
{
  SeqEntryPtr  sep;  
  ValNodePtr   vnp;
  AlignNodePtr anp;
  Uint2        eID = 0;

  sep=GetBestTopParentForItemID (master->entityID, master->itemID, OBJ_BIOSEQ);
  sep = GetTopSeqEntryForEntityID (master->entityID);
  if (sep == NULL) return 0;
  if (IS_Bioseq_set(sep)) {
     eID = SeqMgrGetEntityIDForSeqEntry(sep);
  }
  else {
     for (vnp=anp_list; vnp!=NULL; vnp=vnp->next) {
        anp = (AlignNodePtr) vnp->data.ptrvalue;
        sep = GetBestTopParentForItemID (anp->seq_entityID, anp->bsp_itemID, OBJ_BIOSEQ);
        sep = GetTopSeqEntryForEntityID (anp->seq_entityID);
        if (sep!=NULL)
           if (IS_Bioseq_set(sep)) {
              eID = SeqMgrGetEntityIDForSeqEntry(sep);
              break;
           }
     }
  }
  return eID;
}
*/

