/*  alignval.c
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
* File Name:  alignval.c
*
* Author:  Jian Ye, Colombe Chappey
*
* Version Creation Date:   6/3/99
*
* $Revision: 6.39 $
*
* File Description:  To validate sequence alignment.
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
#include <seqmgr.h>
#include <objmgr.h>
#include <sequtil.h> 
#include <sqnutils.h>
#include <satutil.h>

#include <txalign.h>
#include <salpacc.h>
#include <alignval.h>
#include <valid.h>



Uint1  jybitnum[8]={0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

typedef struct saval {
  Boolean     message;
  Boolean     msg_success;
  Boolean     find_remote_bsp;
  Boolean     find_acc_bsp;
  Boolean     delete_salp;
  Boolean     delete_bsp;
  Boolean     retdel;
  Boolean     do_hist_assembly;
  ValNodePtr  ids;
  Uint2       entityID;
  Boolean     dirty;
} SaVal, PNTR SaValPtr;

typedef struct JY_error_msg {
        Uint1 level;/* corresponds to levels of ErrPostEx [none(0), info(1), war
n(2), error(3) and fatal(4)] */
        CharPtr msg;
} JYErrorMsg, *JYErrorMsgPtr;

/******************************************************************
***
*** Error Messaging
***	copies of the BLASt functions in blastpri.h
***	JYConstructErrorMessage = BlastConstructErrorMessage
***	JYErrorChainDestroy = BlastErrorChainDestroy
***
******************************************************************/ 

static ValNodePtr errorp = NULL;
#define BUFFER_LENGTH 512


static ValNodePtr JYConstructErrorMessage (CharPtr function, CharPtr message, Uint1 level, ValNodePtr PNTR vnpp)
{
        Char buffer[BUFFER_LENGTH];
        CharPtr ptr;
        JYErrorMsgPtr error_msg;

        if (vnpp == NULL)
                return NULL;

        buffer[0] = NULLB;
        ptr = buffer;
        if (function != NULL)
        {
                sprintf(buffer, "%s: ", function);
                ptr = buffer + StringLen(buffer);
        }

        if (message != NULL)
        {
                sprintf(ptr, "%s", message);
        }

        error_msg = (JYErrorMsgPtr) MemNew(sizeof(JYErrorMsg));
        error_msg->msg = StringSave(buffer);
        error_msg->level = level;

        ValNodeAddPointer(vnpp, 0, error_msg);

        return *vnpp;
}

static ValNodePtr JYErrorChainDestroy (ValNodePtr vnp)

{
        ValNodePtr start = vnp;
        JYErrorMsgPtr error_msg;

        while (vnp)
        {
           error_msg = (JYErrorMsgPtr) vnp->data.ptrvalue;
           if (error_msg != NULL) {
              MemFree(error_msg->msg);
           }
           vnp->data.ptrvalue = MemFree(vnp->data.ptrvalue);
           vnp = vnp->next;
        }

        ValNodeFree(start);

        return NULL;
}
/******************************************************************
Output error message according to code defined in alignval.h.  
id refers to seqid of the sequence that causes the error 
and idcontext refers to other sequences in the same segment.  
Intvalue is used to indicate 1) the segment where the sequence 
with error is, or 2) the segtype in case of segtype error.  
Please note that not all errors report all three 
parameters(id, idcontext, Intvalue)
******************************************************************/ 

static Boolean useValErr = FALSE;
static Boolean useLockByID = FALSE;
static ValidStructPtr useVsp = NULL;

static BioseqPtr AlignValBioseqLockById (SeqIdPtr sid)

{
  if (useLockByID) {
    return BioseqLockById (sid);
  } else {
    return BioseqFindCore (sid);
  }
}

static Boolean AlignValBioseqUnlockById (SeqIdPtr sid)

{
  if (useLockByID) {
    return BioseqUnlockById (sid);
  } else {
    return TRUE;
  }
}

NLM_EXTERN void CDECL  ValidErr VPROTO((ValidStructPtr vsp, int severity, int code1, int code2, const char *fmt, ...));

/*****************************************************************
*  get the approximate sequence coordinate for an alignment segment
*  sip == NULL -> get alignment coordinate
*****************************************************************/
static Int4 valmsggetseqpos(SeqAlignPtr sap, Int4 segment, SeqIdPtr sip)
{
   Int4          c;
   DenseDiagPtr  ddp;
   DenseSegPtr   dsp;
   Boolean       found;
   Int4          i;
   Int4          j;
   Int4          pos;
   PackSegPtr    psp;
   Uint1Ptr      seqpresence;
   SeqIdPtr      sip_tmp;
   SeqLocPtr     slp;
   StdSegPtr     ssp;

   if (sap == NULL || sap->segs == NULL || segment == 0)
      return -1;
   if (sap->segtype == SAS_DENSEG)
   {
      dsp = (DenseSegPtr)sap->segs;
      if (sip == NULL)
      {
         pos = 0;
         for (c=0; c<segment; c++)
         {
            pos += dsp->lens[c];
         }
         return pos;
      }
      sip_tmp = dsp->ids;
      i = 0;
      found = FALSE;
      while (!found && sip_tmp != NULL)
      {
         if (SeqIdComp(sip, sip_tmp) == SIC_YES)
            found = TRUE;
         else
         {
            sip_tmp = sip_tmp->next;
            i++;
         }
      }
      if (!found || i>dsp->dim || segment > dsp->numseg)
         return -1;
      pos = 0;
      for (c=0; c<segment; c++)
      {
         if ((j = dsp->starts[(dsp->dim*c)+i])>0)
            pos=j;
      }
      return pos;
   } else if (sap->segtype == SAS_DENDIAG)
   {
      ddp = (DenseDiagPtr)sap->segs;
      pos = 0;
      for (c=0; c<segment; c++)
      {
         pos += ddp->len;
         ddp = ddp->next;
         if (ddp == NULL)
            return -1;
      }
      if (sip == NULL)
         return pos;
      sip_tmp = ddp->id;
      i = 0;
      found = FALSE;
      while (!found && sip_tmp != NULL)
      {
         if (SeqIdComp(sip, sip_tmp) == SIC_YES)
            found = TRUE;
         else
         {
            sip_tmp = sip_tmp->next;
            i++;
         }
      }
      if (!found || i>ddp->dim)
         return -1;
      return (ddp->starts[i]);
   } else if (sap->segtype == SAS_STD)
   {
      ssp = (StdSegPtr)(sap->segs);
      pos = 0;
      for (c=0; c<segment; c++)
      {
         pos += SeqLocLen(ssp->loc);
         ssp = ssp->next;
         if (ssp == NULL)
            return -1;
      }
      if (sip == NULL)
         return pos;
      slp = ssp->loc;
      while (!found && slp!=NULL)
      {
         sip_tmp = SeqLocId(slp);
         if (SeqIdComp(sip, sip_tmp) == SIC_YES)
            found = TRUE;
         else
            slp = slp->next;
      }
      if (!found)
         return -1;
      return (SeqLocStart(slp));
   } else if (sap->segtype == SAS_PACKED)
   {
      psp = (PackSegPtr)(sap->segs);
      if (segment > psp->numseg)
         return -1;
      if (sip == NULL)
      {
         pos = 0;
         for (c=0; c<segment; c++)
         {
            pos += psp->lens[c];
         }
         return pos;
      }
      sip_tmp = psp->ids;
      i = 0;
      found = FALSE;
      while (!found && sip_tmp != NULL)
      {
         if (SeqIdComp(sip, sip_tmp) == SIC_YES)
            found = TRUE;
         else
         {
            sip_tmp = sip_tmp->next;
            i++;
         }
      }
      if (!found || i>psp->dim)
         return -1;
      pos = 0;
      seqpresence = NULL;
      BSSeek(psp->present, 0, SEEK_SET);
      seqpresence=MemNew(BSLen(psp->present));
      if(!seqpresence)
         return -1;
      BSRead(psp->present, seqpresence, BSLen(psp->present));
      for (c=0; c<segment; c++)
      {
         if (seqpresence[(c*psp->numseg+i)/8]&jybitnum[(c*psp->numseg+i)%8])
            pos+=psp->lens[c];
      }
      return pos;
   } else
      return -1;
}

static void ValMessage (SeqAlignPtr salp, Int1 MessageCode, ErrSev errlevel, SeqIdPtr id, SeqIdPtr idcontext , Int4 Intvalue) 
{
  
  Char     buf[256], 
           buf3[64],
           string1[64],
           string2[552];
  GatherContextPtr gcp;
  Int4     pos;

  string1[0] = '\0';
  string2[0] = '\0';
  SeqIdWrite(id, buf, PRINTID_FASTA_LONG, sizeof(buf)-1);
  switch(MessageCode)
  {
    case Err_SeqId:
      sprintf(string1, "SeqId");
      sprintf(string2, "The sequence corresponding to SeqId %s could not be found", buf);
      break;

    case Err_Strand_Rev:
      pos = valmsggetseqpos(salp, Intvalue, id);
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "Strand");
      sprintf(string2, "The strand labels for SeqId %s are inconsistent across the alignment; the first inconsistent region is the %ld(th) region, near sequence position %ld, context %s", buf, (long) Intvalue, (long) pos, buf3);
      break;

    case Err_Denseg_Len_Start:
      pos = valmsggetseqpos(salp, Intvalue, id);
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "Start/Length");
      sprintf(string2, "There is a problem with sequence %s, in segment %ld (near sequence position %ld), context %s: the segment is too long or short or the next segment has an incorrect start position", buf, (long) Intvalue, (long) pos, buf3);
      break;

    case  Err_Start_Less_Than_Zero:
      pos = valmsggetseqpos(salp, Intvalue, id);
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "Start");
      sprintf(string2, "Start point is less than zero in segment %ld (near sequence position %ld) for sequence ID: %s in the context of %s", (long) Intvalue, (long) pos, buf, buf3);
      break;

    case Err_Start_More_Than_Biolen:
      pos = valmsggetseqpos(salp, Intvalue, id);
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "Start");
      sprintf(string2, "In sequence %s, segment %ld (near sequence position %ld) context %s, the alignment claims to contain residue coordinates that are past the end of the sequence.  Either the sequence is too short, or there are extra characters or formatting errors in the alignment", buf, (long) Intvalue, (long) pos, buf3);
      break;

    case Err_End_Less_Than_Zero:
      pos = valmsggetseqpos(salp, Intvalue, id);
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "Length");
      sprintf(string2, "End point is less than zero in segment %ld (near position %d) for sequence ID: %s in the context of %s.  This could be a formatting error", (long) Intvalue, (int) pos,buf, buf3);
      break;

    case Err_End_More_Than_Biolen:
      pos = valmsggetseqpos(salp, Intvalue, id);
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "Length");
      sprintf(string2, "In sequence %s, segment %ld (near sequence position %ld) context %s, the alignment claims to contain residue coordinates that are past the end of the sequence.  Either the sequence is too short, or there are extra characters or formatting errors in the alignment", buf, (long) Intvalue, (long) pos, buf3);
      break;

    case Err_Len_Less_Than_Zero:
      pos = valmsggetseqpos(salp, Intvalue, id);
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "Length");
      sprintf(string2, "Segment length is less than zero in segment %ld (near sequence position %ld) for sequence ID: %s in the context of %s.  Look for extra characters in this segment or flanking segments", (long) Intvalue, (long) pos, buf, buf3); 
      break;

    case Err_Len_More_Than_Biolen:
      pos = valmsggetseqpos(salp, Intvalue, id);
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "Length");
      sprintf(string2, "In sequence %s, segment %ld (near sequence position %ld) context %s, the alignment claims to contain residue coordinates that are past the end of the sequence.  Either the sequence is too short, or there are extra characters or formatting errors in the alignment", buf, (long) Intvalue, (long) pos, buf3);
      break; 
 
    case Err_Sum_Len_Start:
      pos = valmsggetseqpos(salp, Intvalue, id);
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "Start");
      sprintf(string2, "In sequence %s, segment %ld (near sequence position %ld) context %s, the alignment claims to contain residue coordinates that are past the end of the sequence.  Either the sequence is too short, or there are extra characters or formatting errors in the alignment", buf, (long) Intvalue, (long) pos, buf3);
      break;

    case Err_SeqAlign_DimSeqId_Not_Match:
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "SeqId");
      sprintf(string2, "The Seqalign has more or fewer ids than the number of rows in the alignment (context %s).  Look for possible formatting errors in the ids.", buf3);
      break;

    case Err_Segs_DimSeqId_Not_Match:
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "SeqId");
      sprintf(string2, "In segment %ld, there are more or fewer rows than there are seqids (context %s).  Look for possible formatting errors in the ids.", (long) Intvalue, buf3);
      break;

    case Err_Fastalike:
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "Fasta");
      sprintf(string2, "This may be a fasta-like alignment for SeqId: %s in the context of %s", buf, buf3); 
      break;

    case Err_Null_Segs:
      sprintf(string1, "Segs");
      sprintf(string2, "This alignment is missing all segments.  This is a non-correctable error -- look for serious formatting problems.");
      break;

    case Err_Segment_Gap:
      pos = valmsggetseqpos(salp, Intvalue, id);
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "Segs");
      sprintf(string2, "Segment %ld (near alignment position %ld) in the context of %s contains only gaps.  Each segment must contain at least one actual sequence -- look for columns with all gaps and delete them.", (long) Intvalue, (long) pos, buf3);
      break;

    case Err_Segs_Dim_One:
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "Segs");
      sprintf(string1, "Segment %ld apparently has only one sequence.  Each portion of the alignment must have at least two sequences.  context %s", (long) Intvalue, buf3);
      break;

    case Err_SeqAlign_Dim_One:
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "Dim");
      sprintf(string2, "This seqalign apparently has only one sequence.  Each alignment must have at least two sequences.  context %s", buf3);
      break;

    case Err_Segtype :
      sprintf(string1, "Segs");
      sprintf(string2, "This alignment has a undefined or unsupported Seqalign segtype %ld", (long) Intvalue);
      break;

    default:
      break;
  }
  if (useValErr) {
    if (salp != NULL && useVsp != NULL) {
      gcp = useVsp->gcp;
      if (gcp != NULL) {
	    gcp->entityID = salp->idx.entityID;
	    gcp->itemID = salp->idx.itemID;
	    gcp->thistype = salp->idx.itemtype;
        ValidErr (useVsp, errlevel, 6, MessageCode, "%s: %s", string1, string2);
      }
    }
    return;
  }
  if (StringLen(string1) > 0)
     errorp = JYConstructErrorMessage (string1, string2, errlevel, &errorp);
}

 
/******************************************************************
******************************************************************/ 
static SeqAlignPtr LIBCALL is_salp_in_sap (SeqAnnotPtr sap, Uint1 choice)
{
  SeqAlignPtr      salp = NULL;

  if (sap != NULL) {
     for (; sap!= NULL; sap=sap->next) {
        if (sap->type == choice) {
           salp = (SeqAlignPtr) sap->data;
           return salp;
        }
     }   
  }
  return NULL;
}

/******************************************************************
return the number of seqid
******************************************************************/ 
static Int2 CountSeqIdInSip (SeqIdPtr sip)
{
    Int2 numids=0;

     while(sip) 
       { 
	 numids++;
	 sip=sip->next;
       }
     return numids;
}

/*********************************************************/
static void delete_bioseqs (ValNodePtr ids, Uint2 entityID)
{
  SeqEntryPtr  sep_top;
  SeqEntryPtr  sep_del;
  ValNodePtr   vnp;
  SeqIdPtr     sip;
  SeqLocPtr    slp;
  BioseqPtr    bsp;
  ObjMgrDataPtr  omdptop;
  ObjMgrData     omdata;
  Uint2          parenttype;
  Pointer        parentptr;

  if (ids == NULL)
     return;
  sep_top = GetTopSeqEntryForEntityID (entityID);
  SaveSeqEntryObjMgrData (sep_top, &omdptop, &omdata);
  GetSeqEntryParent (sep_top, &parentptr, &parenttype);

  vnp=ids;
  while (vnp!=NULL)
  {
     sip = (SeqIdPtr) vnp->data.ptrvalue;
     if (sip!=NULL) {
        slp = (SeqLocPtr)ValNodeNew (NULL);
        slp->choice = SEQLOC_WHOLE;
        slp->data.ptrvalue = sip;
        bsp = GetBioseqGivenSeqLoc (slp, entityID);
        if (bsp!=NULL) {
           sep_del=GetBestTopParentForData (entityID, bsp);
           RemoveSeqEntryFromSeqEntry (sep_top, sep_del, FALSE);
        }
        slp->data.ptrvalue = NULL;
        SeqLocFree (slp);
     }
     vnp=vnp->next;
  }
  SeqMgrLinkSeqEntry (sep_top, parenttype, parentptr);
  RestoreSeqEntryObjMgrData (sep_top, omdptop, &omdata);
  RenormalizeNucProtSets (sep_top, TRUE);

  for (vnp=ids; vnp!=NULL; vnp=vnp->next) {
     SeqIdFree ((SeqIdPtr) vnp->data.ptrvalue);
     vnp->data.ptrvalue = NULL;
  }
  ValNodeFree (vnp);
  return;
}


/******************************************************************
validate a SeqId
******************************************************************/ 
static void ValidateSeqId (SeqIdPtr sip, SeqAlignPtr salp)
{
  SeqIdPtr  siptemp=NULL;
  BioseqPtr bsp=NULL;
  
  for(siptemp=sip; siptemp!=NULL; siptemp=siptemp->next)
  {
	/*
	bsp = AlignValBioseqLockById(siptemp);
	if(!bsp)
		ValMessage (salp, Err_SeqId, SEV_ERROR, siptemp, NULL, 0);
	else
		AlignValBioseqUnlockById(siptemp);
	*/
	bsp = BioseqFindCore (siptemp);
	if (bsp == NULL && siptemp->choice == SEQID_LOCAL) {
		ValMessage (salp, Err_SeqId, SEV_ERROR, siptemp, NULL, 0);
	}
  }
  return;
}

/******************************************************************
return seqid for each seg.  
Note that a newly created seqid chain is returned for stdseg 
and you need to free the memory after you use it in this case
******************************************************************/ 
static SeqIdPtr SeqIdInAlignSegs(Pointer segs, Uint1 segtype, SeqAlignPtr salp)
{

  SeqIdPtr sip=NULL;
  StdSegPtr ssp;
  DenseDiagPtr ddp;
  DenseSegPtr dsp;
  PackSegPtr psp;
  SeqLocPtr slp=NULL, slptemp;

  if(!segs)
  {
      ValMessage (salp, Err_Null_Segs, SEV_ERROR, NULL, NULL, 0);
      return NULL;
  }
  if(segtype==1) 
  { /* DenseDiag */
	  
	  ddp=(DenseDiagPtr)segs;    
	  sip=ddp->id;
  }
  else if (segtype==2)
  { /* DenseSeg */
	  
	  dsp = (DenseSegPtr) segs;
	  sip=dsp->ids;
  }
  else if (segtype==3)
  { /* StdSeg */
	  
	  ssp = (StdSegPtr)segs;
	  slp = ssp->loc;
	  /*make a new linked list of SeqId*/
	  for(slptemp=slp; slptemp!=NULL; slptemp=slptemp->next)
	    AddSeqId(&sip, SeqLocId(slptemp));
	  
  }
  else if(segtype==4)
  { /* Packed Seg. Optimal for editing alignments */
	  
	  psp = (PackSegPtr)segs;
	  if (psp!=NULL)
	    sip = psp->ids;
  }      
  return sip;
}

 
/******************************************************************
validate SeqId in sequence alignment
******************************************************************/ 
static void  ValidateSeqIdInSeqAlign (SeqAlignPtr salp)
{
  SeqIdPtr sip=NULL;
  Pointer segptr=NULL;
  DenseDiagPtr ddp=NULL, ddptemp;
  StdSegPtr    ssp=NULL, ssptemp;
 

  if(salp)
    {     
      segptr=salp->segs;
      if(!segptr)
	ValMessage (salp, Err_Null_Segs, SEV_ERROR, NULL, NULL, 0);
      else
	{

	  /*densediag */
	  if(salp->segtype==1)
	    {
	      /*cast to appropriate pointer*/
	      ddp=(DenseDiagPtr)segptr;
	      for(ddptemp=ddp; ddptemp!=NULL; ddptemp=ddptemp->next)
		{
		  
		  sip=SeqIdInAlignSegs((Pointer)ddptemp, salp->segtype, salp);	
		  ValidateSeqId(sip, salp);
		}
	    }
	  
	  /*Stdseg*/
	  else if(salp->segtype==3)
	    {
	      /*cast to appropriate pointer*/
	      ssp=(StdSegPtr)segptr;
	      for(ssptemp=ssp; ssptemp!=NULL; ssptemp=ssptemp->next)
		{
		  
		  sip=SeqIdInAlignSegs((Pointer)ssptemp, salp->segtype, salp);	
		  ValidateSeqId(sip, salp);
		  /*free Seqid if sip is a new chain created by SeqIdinAlignSegs*/
		  SeqIdSetFree(sip);
		}
	    }
	  
	  /*Denseseg, Packseg*/
	  else if(salp->segtype==2||salp->segtype==4)
	    {
	      
	      sip=SeqIdInAlignSegs(segptr, salp->segtype, salp);	
	      ValidateSeqId(sip, salp);
	    } 
	}
    }
}

/******************************************************************
return true if  two sip are the same, false otherwise.  
Also return false if there is error in sip
******************************************************************/ 
static Boolean SeqIdCmp (SeqIdPtr sip1, SeqIdPtr sip2)
{
  Char buf1[256], buf2[256];
 
  if(!sip1||!sip2)
    return FALSE;

  SeqIdWrite(sip1, buf1, PRINTID_FASTA_LONG, 255);
  SeqIdWrite(sip2, buf2, PRINTID_FASTA_LONG, 255);
  return(!StringCmp(buf1, buf2));
 
}
 

/******************************************************************
return the strand for a seqloc with seqid=sip in a stdseg.  
Note, it returns 255 if null sip or ssp
******************************************************************/ 
static Uint1 SeqLocStrandForSipInStdSeg (SeqIdPtr sip, StdSegPtr ssp, SeqAlignPtr salp)
{
  SeqLocPtr slp, slptemp;
  Uint1     strand=0;
    
  if(!sip||!ssp)
    return (255);

  slp=ssp->loc;
  for(slptemp=slp; slptemp!=NULL; slptemp=slptemp->next)
  {
      if(SeqIdCmp(sip, SeqLocId(slptemp)))
	{
	  strand=SeqLocStrand(slptemp);
	  break;
	}
  }
  return strand;
}


/******************************************************************
check if the  strand is consistent in Stdseg
******************************************************************/ 
static void ValidateStrandInStdSeg(StdSegPtr ssp, SeqAlignPtr salp)
{
  SeqIdPtr     sip=NULL,  sip_inseg=NULL;
  Uint1	       strand1=0, strand2=0;
  StdSegPtr    ssptemp, ssptemp2, ssptemp3;
  SeqLocPtr    slp, slptemp;
  ValNodePtr   FinishedSip=NULL, temp;
  Boolean      CheckedStatus;
  Int4         start_numseg=0, end_numseg=0;
  
  if(!ssp)
    ValMessage (salp, Err_Null_Segs, SEV_ERROR, NULL, NULL, 0);
  else
    for(ssptemp=ssp; ssptemp!=NULL; ssptemp=ssptemp->next)
      {
	sip_inseg=SeqIdInAlignSegs((Pointer)ssptemp, 3, salp);
	start_numseg++;
	slp=ssptemp->loc;
	for(slptemp=slp; slptemp!=NULL; slptemp=slptemp->next)
	  {
	    
	    CheckedStatus=FALSE;
	    sip=SeqLocId(slptemp);
	    if(sip)
	      {
		/*if a seqloc represented by a sip has been checked, set the checkedstatus flag to true so it will not be checked again*/
		for(temp=FinishedSip; temp!=NULL; temp=temp->next)
		  {
		    if(SeqIdCmp(sip, temp->data.ptrvalue))
		      {
			CheckedStatus=TRUE;
			break;
		      }
		  }
		/*seqloc not checked yet*/
		if(!CheckedStatus)
		  {
		    
		    /*keep a record of  checked sip*/
		    ValNodeAddPointer(&FinishedSip, 0, sip);
		    end_numseg=start_numseg;
		    /*go through all segs to get at least two strand, if any, for this seqloc*/
		    for(ssptemp2=ssptemp; ssptemp2!=NULL; ssptemp2=ssptemp2->next, end_numseg++)
		      {
			/*get the first defined strand */
			strand1=SeqLocStrandForSipInStdSeg(sip, ssptemp2, salp);
			
			if(strand1!=0&&strand1!=255)
			  {
			    ssptemp2=ssptemp2->next;
			    break;
			  }
			
		      }
		    
		    if(strand1!=0&&strand1!=255)
		      /*continue to get next strand */
		      for(ssptemp3=ssptemp2; ssptemp3!=NULL; ssptemp3=ssptemp3->next, end_numseg++)
			{
			  strand2=SeqLocStrandForSipInStdSeg(sip, ssptemp3, salp);
			  if(strand2==0||strand2==255)
			    continue;
			  
			  if(strand2!=0&&strand2!=255)
			    /*strand should be same for a given seq*/ 
			    if(strand1!=strand2)
			      
			      ValMessage (salp, Err_Strand_Rev, SEV_ERROR, sip, sip_inseg, end_numseg+1);
			}
		    }
	      }
	  }
	SeqIdSetFree(sip_inseg);
	
      }
  
  ValNodeFree(FinishedSip);
}
 
 
/******************************************************************
check if the  strand is consistent in Denseseg
******************************************************************/ 
static void ValidateStrandInPack_DenseSeg(Pointer segs, Uint1 segtype, SeqAlignPtr salp)
{ 
  DenseSegPtr dsp=NULL;
  PackSegPtr psp=NULL;
  Int2         numseg, aligndim, dimnumseg, i, j, m;
  SeqIdPtr     sip=NULL, siptemp;
  Uint1	       strand1=0, strand2=0;
  Uint1Ptr strandptr=NULL;
		
  if(!segs)
    ValMessage (salp, Err_Null_Segs, SEV_ERROR, NULL, NULL, 0);
  else if(segtype==2||segtype==4)
    {
      if(segtype==2)
	{
	  dsp=(DenseSegPtr)segs;
	  strandptr=dsp->strands;
	  sip=dsp->ids;
	  numseg=dsp->numseg;
	  aligndim=dsp->dim;
	}
      
      else if(segtype==4)
	{
	  psp=(PackSegPtr)segs;
	  strandptr=psp->strands;
	  sip=psp->ids;
	  numseg=psp->numseg;
	  aligndim=psp->dim;
	}
      dimnumseg=numseg*aligndim;
      if(strandptr)
	{
	 
	  /*go through id for each alignment sequence*/
	  for(j=0; j<aligndim; j++)
	    {
	      /* first  strand value for each sequence*/ 
	      strand1=dsp->strands[j];
	      /* go through all strand values for each sequence*/  
	      for(i=j+aligndim; i<dimnumseg; i=i+aligndim)
		{		  
		  strand2=dsp->strands[i];
		  
		  if(strand1==0||strand1==255)
		    {
		      strand1=strand2;
		      continue;
		    }
		  
		  /*skip undefined strand*/
		  if(strand2!=0&&strand2!=255)
		    /*strand should be same for a given seq*/ 
		    if(strand1!=strand2)
		      {
			/*find current seqid*/
			
			siptemp=sip;
			for(m=0; m<j&&siptemp!=NULL; m++)
			  siptemp=siptemp->next;
			ValMessage (salp, Err_Strand_Rev, SEV_ERROR, siptemp, sip, i/aligndim+1);
		
			
		      }
		}
	    }
	}
    }
}




/******************************************************************
check if the  strand is consistent in SeqAlignment of global 
or partial type
******************************************************************/ 
static void ValidateStrandinSeqAlign(SeqAlignPtr salp)
{
  StdSegPtr ssp=NULL ;
  
  if(salp)
    {
   
      /*Strands needs to be validated  in case of global or partial alignment*/ 
     
      /*denseseg or packseg*/
      if(salp->segtype==2||salp->segtype==4)
 
	ValidateStrandInPack_DenseSeg(salp->segs, salp->segtype, salp);

      /*stdseg*/
      else if(salp->segtype==3)
	{
	  ssp=(StdSegPtr)salp->segs;
	  ValidateStrandInStdSeg(ssp, salp);
	}
   } 
}



/******************************************************************
Make sure that, in Densediag alignment, segment length and 
start point is not less than zero, and  segment length is not greater 
than Bioseq length
******************************************************************/ 
static void ValidateSeqlengthInDenseDiag (DenseDiagPtr ddp, SeqAlignPtr salp)
{
  Int4Ptr      stptr=NULL; 
  DenseDiagPtr ddptemp;
  Int2         numseg, i;
  SeqIdPtr     sip=NULL, siptemp;
  Int4         bslen;
  BioseqPtr    bsp=NULL;
  

  if(!ddp)
    ValMessage (salp, Err_Null_Segs, SEV_ERROR, NULL, NULL, 0);
  else
    {
      for(ddptemp=ddp, numseg=0; ddptemp!=NULL; ddptemp=ddptemp->next, numseg++)
	{
	  sip=ddp->id;
	  stptr=ddptemp->starts;
	  
	  if(stptr)
	    {
	      for(i=0, siptemp=sip; i<ddptemp->dim; i++, siptemp=siptemp->next)
		{
		  bsp=AlignValBioseqLockById(siptemp);
		  if(bsp)
		    {
		      bslen=bsp->length; 
		      AlignValBioseqUnlockById(siptemp);
		      /*verify start*/
		      if(stptr[i]<0)
			ValMessage (salp, Err_Start_Less_Than_Zero, SEV_ERROR, siptemp, sip , numseg+1);     
		      if(stptr[i]>bslen)
			ValMessage (salp, Err_Start_More_Than_Biolen, SEV_ERROR, siptemp, sip , numseg+1); 
		      
		      /*verify length*/
		      
		      if(ddptemp->len<0)
			ValMessage (salp, Err_Len_Less_Than_Zero, SEV_ERROR, siptemp, sip , numseg+1); 
		      
		      if(ddptemp->len+stptr[i]>bslen)
			ValMessage (salp, Err_Sum_Len_Start, SEV_ERROR, siptemp, sip , numseg+1);  
		    }
		}
	    }
	}
    }
}


/******************************************************************
return a new copy of len array in reversed order 
******************************************************************/ 
static Int4Ptr GetReverseLength (Int2 numseg, Int4Ptr lenptr)
{
  Int4Ptr lenptrtemp=NULL;
  Int2 p;
  
  if(!lenptr)
    return NULL;

  lenptrtemp=(Int4Ptr)MemNew(numseg*sizeof(Int4Ptr));
  if(!lenptrtemp)
  {
      ErrPostEx (SEV_ERROR, 0,0,  "Warning:insufficient memory");
      return NULL;
  }
  for(p=0; p<numseg; p++)    
    lenptrtemp[p]=lenptr[numseg-1-p];
  return lenptrtemp;

}

/******************************************************************
return a new copy of start array in reversed "numseg" order .  
Note that the relative position of starts in each numseg has not changed.  
Example:  original length={0, 0, 10, -1, 30, 10}, numseg=3, 
lens={10, 20, 40}, the reversed length={30, 10, 10, -1, 0, 0}
******************************************************************/ 
static Int4Ptr GetReverseStart(Int2 numseg, Int2 dim, Int4Ptr stptr)
{
  Int4Ptr stptrtemp=NULL;
  Int2 p, q;

  if(!stptr)
    return NULL;

  stptrtemp=(Int4Ptr)MemNew(numseg*dim*sizeof(Int4Ptr));
  if(!stptrtemp)
  {
      ErrPostEx (SEV_ERROR, 0,0,  "Warning:insufficient memory"); 
      return NULL; 
  }
  for(p=0; p<numseg; p++)
    for(q=0; q<dim; q++)
      stptrtemp[q+p*dim]=stptr[q+(numseg-1-p)*dim];

  return stptrtemp;
}

 

/******************************************************************
Make sure that, in Denseseg alignment, segment length and 
start point agrees each other and the sum of segment length 
is not greater than Bioseq length
******************************************************************/ 
static void ValidateSeqlengthInDenseSeg (DenseSegPtr dsp, SeqAlignPtr salp)
{

  Int4Ptr      lenptr=NULL, stptr=NULL, lenptrtemp=NULL, stptrtemp=NULL, lenptrtemp2=NULL, stptrtemp2=NULL;
  
  Int2         numseg, aligndim, i, j;
  SeqIdPtr     sip=NULL, siptemp;
  Int4         bslen;
  BioseqPtr    bsp=NULL;

 if(!dsp)
   ValMessage (salp, Err_Null_Segs, SEV_ERROR, NULL, NULL, 0);
 else
    {
      numseg=dsp->numseg;
      aligndim=dsp->dim; 	
      
      stptr=dsp->starts;
      lenptr=dsp->lens;
      sip=dsp->ids;
     
      if(stptr==NULL||lenptr==NULL)
	return;
      
  
      /*go through each sequence*/
      for(j=0, siptemp=sip; j<aligndim&&siptemp; j++, siptemp=siptemp->next)
	{
       
	  lenptrtemp=lenptr;
	  stptrtemp=stptr;
	  /*if on minus strand, use reversed length and start array*/
	  if(dsp->strands)
	    {
	      if(dsp->strands[j]==Seq_strand_minus)
		{
		  if(!lenptrtemp2&&!stptrtemp2)
		    {
		      lenptrtemp2= GetReverseLength (numseg, lenptr);
		      if (lenptrtemp2==NULL)
		         return;
		      stptrtemp2= GetReverseStart (numseg, aligndim, stptr);
		      if (stptrtemp2==NULL)
		         return;
		    }
		  lenptrtemp=lenptrtemp2;
		  stptrtemp=stptrtemp2;
		}
	    }

	  bsp=AlignValBioseqLockById(siptemp);
	  if(bsp!=NULL)
	    {
	      bslen=bsp->length;  
	      AlignValBioseqUnlockById(siptemp);
	    }

	  /*go through each segment for a given sequence*/
	  for(i=0; i<numseg; i++)
	    {
	   
	      /*no need to verify if segment is not present*/
	      if(stptrtemp[j+i*aligndim]!=-1)
		{
 
		  /*length plus start should be equal to next start*/
		  /*check a start if it's not the last one and the next start is not -1*/
		  if(i!=numseg-1&&stptrtemp[j+(i+1)*aligndim]!=-1)
		    {	  
		      
		      if(stptrtemp[j+i*aligndim]+lenptrtemp[i]!=stptrtemp[j+(i+1)*aligndim]) 
			{
			  if (dsp->strands)
			    {
			      if(dsp->strands[j]==2)
				ValMessage (salp, Err_Denseg_Len_Start, SEV_ERROR, siptemp, sip , numseg-i); 
			      else
				ValMessage (salp, Err_Denseg_Len_Start, SEV_ERROR, siptemp, sip , i+1);    
			    }
                          else
			    ValMessage (salp, Err_Denseg_Len_Start, SEV_ERROR, siptemp, sip , i+1);
			}
		    }
		  /*check a start if it's not the last one and the next start is -1*/
		  else if (i!=numseg-1&&stptrtemp[j+(i+1)*aligndim]==-1)
		    {
		      Int4 k=i+1;
		      /*find the next start that is not last and not -1*/
		      while(k<numseg&&stptrtemp[j+k*aligndim]==-1)
			k++;

		      /*length plus start should be equal to the closest next start that is not -1*/		   
	 
		      if(k<numseg&&stptrtemp[j+i*aligndim]+lenptrtemp[i]!=stptrtemp[j+k*aligndim])
			{
			  if (dsp->strands)
			    {
			      if(dsp->strands[j]==2)
				ValMessage (salp, Err_Denseg_Len_Start, SEV_ERROR, siptemp, sip , numseg-i); 
			      else
				 ValMessage (salp, Err_Denseg_Len_Start, SEV_ERROR, siptemp, sip , i+1); 
			    }
                          else
			    ValMessage (salp, Err_Denseg_Len_Start, SEV_ERROR, siptemp, sip , i+1);	
			}
		    }
		  
		  
		 /*make sure the start plus segment does not exceed total bioseq length*/ 
		  if(bsp!=NULL)
		    {
		      
		      if(stptrtemp[j+i*aligndim]+lenptrtemp[i]>bslen)
			if (dsp->strands)
			  {
			    if(dsp->strands[j]==2)
			      ValMessage (salp, Err_Sum_Len_Start, SEV_ERROR, siptemp, sip , numseg-1); 
			    else
			      ValMessage (salp, Err_Sum_Len_Start, SEV_ERROR, siptemp, sip , i+1); 
			  }
			else
			  ValMessage (salp, Err_Sum_Len_Start, SEV_ERROR, siptemp, sip , i+1); 
		    }
		  
		}
	      	      
	    }		
	}
    }		


 MemFree(lenptrtemp2);
 MemFree(stptrtemp2);
	              

}

/******************************************************************
Make sure that, in Seqloc of a Stdseg alignment, 
end point, start point and length are not less than zero, 
and are not greater than Bioseq length
******************************************************************/ 
static void ValidateSeqlengthInStdSeg (StdSegPtr ssp, SeqAlignPtr salp)
{ 
  StdSegPtr    ssptemp;
  Int2         numseg;
  SeqIdPtr     sip=NULL, siptemp;
  Int4         start, end, length, bslen;
  BioseqPtr    bsp=NULL;
  SeqLocPtr    slp=NULL, slptemp;

    if(!ssp)
      ValMessage (salp, Err_Null_Segs, SEV_ERROR, NULL, NULL, 0);
    else
      for(ssptemp=ssp, numseg=0; ssptemp!=NULL; ssptemp=ssptemp->next, numseg++)
	{ 
	  /*get all seqid in current segment*/
	  sip=SeqIdInAlignSegs((Pointer)ssptemp, 3, salp);		 
	  slp=ssptemp->loc;
	  if(slp==NULL)
	    return;
	  for(slptemp=slp; slptemp!=NULL; slptemp=slptemp->next) 
	    
	    { 
	      siptemp=SeqLocId(slptemp);
	      start=SeqLocStart(slptemp);
	      end=SeqLocStop(slptemp);
	      length=SeqLocLen(slptemp);
	     
	      bsp=AlignValBioseqLockById(siptemp);
	      if(bsp)
		{
		  bslen=bsp->length;
		  AlignValBioseqUnlockById(siptemp); 
	 
		  /*verify start*/
		  if(start!=-1)
		    {
		      if(start<0)
			ValMessage (salp, Err_Start_Less_Than_Zero, SEV_ERROR, siptemp, sip , numseg+1);      
		      
		      if(start>bslen-1)
			ValMessage (salp, Err_Start_More_Than_Biolen, SEV_ERROR, siptemp, sip , numseg+1); 
		    }
		  
		  if(end!=-1)
		    {
		      /*verify end*/
		      if(end<0)
			ValMessage (salp, Err_End_Less_Than_Zero, SEV_ERROR, siptemp, sip , numseg+1); 
		      if(end>bslen-1)
			ValMessage (salp, Err_End_More_Than_Biolen, SEV_ERROR, siptemp, sip , numseg+1); 
		    }
		  
		  
		  if(length!=-1)
		    {
		      
		      /*verify length*/
		      if(length<0)
			ValMessage (salp, Err_Len_Less_Than_Zero, SEV_ERROR, siptemp, sip , numseg+1); 
		      
		      if(length>bslen)
			ValMessage (salp, Err_Len_More_Than_Biolen, SEV_ERROR, siptemp, sip , numseg+1);  
		    }
		
		}
	    }
	  /*free Seqid if sip is a new chain created by SeqIdinAlignSegs*/	  
	  SeqIdSetFree(sip);
	}

}

/******************************************************************
validate the start and segment length in packseg
******************************************************************/ 
static void ValidateSeqlengthInPackSeg (PackSegPtr psp, SeqAlignPtr salp)
{
  Uint1Ptr     seqpresence=NULL;
  Int2         numseg, aligndim, i, j; 
  SeqIdPtr     sip=NULL, siptemp;
  Int4Ptr      stptr=NULL, lenptr=NULL; 
  BioseqPtr    bsp=NULL;
  Int4         bslen, seg_start;

  if(!psp)
    ValMessage (salp, Err_Null_Segs, SEV_ERROR, NULL, NULL, 0);
  else
    {
      numseg=psp->numseg;
      aligndim=psp->dim; 		  
      sip=psp->ids;
      stptr=psp->starts;
      lenptr=psp->lens;
    
      if(stptr&&lenptr)
	{
	  if(psp->present)
	    {
	      BSSeek(psp->present, 0, SEEK_SET);
	      seqpresence=MemNew(BSLen(psp->present));
	      if(!seqpresence)
		{
		  
		  ErrPostEx (SEV_ERROR, 0,0,  "Warning:insufficient memory");
		  return;
		  
		}
	      BSRead(psp->present, seqpresence, BSLen(psp->present));
	      /*go through each sequence*/
	      for(j=0, siptemp=sip; j<aligndim; siptemp=siptemp->next, j++)
		{  
		  bsp=AlignValBioseqLockById(siptemp);
		  if(bsp)
		    {
		      bslen=bsp->length; 
		      AlignValBioseqUnlockById(siptemp);
		      seg_start=stptr[j];
		      /*check start*/
		      if(seg_start<0)
			ValMessage (salp, Err_Start_Less_Than_Zero, SEV_ERROR, siptemp, sip , 0);     
		      if(seg_start>bslen)
			ValMessage (salp, Err_Start_More_Than_Biolen, SEV_ERROR, siptemp, sip , 0);
		      
		      /*go through each segment*/
		      for(i=0; i<numseg; i++)
			{
			  /*if this segment is present*/
			  if(seqpresence[(i*aligndim+j)/8]&jybitnum[(i*aligndim+j)%8])      
			    {
			      /*check start plus seg length*/
			      seg_start=seg_start+lenptr[i];
			      if(seg_start>bslen)
				 ValMessage (salp, Err_Sum_Len_Start, SEV_ERROR, siptemp, sip, numseg+1);
			    }
			}
		    }
		}
	    }
	}
    }
  MemFree(seqpresence);		 
}

/******************************************************************
check segment length, start and end point in Denseseg, Densediag and Stdseg
******************************************************************/ 
static void  ValidateSeqlengthinSeqAlign (SeqAlignPtr salp)
{
   
  if (salp)
  { 
      if(salp->segtype==1)
	ValidateSeqlengthInDenseDiag ((DenseDiagPtr)salp->segs, salp);
      else if(salp->segtype==2)
	ValidateSeqlengthInDenseSeg ((DenseSegPtr)salp->segs, salp);
      else if(salp->segtype==3)
	ValidateSeqlengthInStdSeg ((StdSegPtr)salp->segs, salp);
      else if(salp->segtype==4)
	ValidateSeqlengthInPackSeg ((PackSegPtr)salp->segs, salp);
  }
}

/******************************************************************
check if # of seqid matches the dimensions, and 
if there is only one seqeuence in seqalign
******************************************************************/ 
static void ValidateDimSeqIds (SeqAlignPtr salp)
{
  SeqIdPtr sip=NULL;
  DenseDiagPtr ddp=NULL, ddptemp;
  StdSegPtr ssp=NULL, ssptemp;
  DenseSegPtr dsp=NULL;
  Int4 numseg=0;
  
 if(salp)
   {
     /*densediag */
     if(salp->segtype==1)
       {
	 
	 ddp=(DenseDiagPtr)salp->segs;
	 if(!ddp)
	   ValMessage (salp, Err_Null_Segs, SEV_ERROR, NULL, NULL, 0);
	 else
	   for(ddptemp=ddp, numseg=0; ddptemp!=NULL; ddptemp=ddptemp->next, numseg++)
	     {
	       sip=ddptemp->id;
	       if(ddptemp->dim==1)
		 ValMessage (salp, Err_Segs_Dim_One, SEV_ERROR, NULL, sip , numseg+1);
	       if(ddptemp->dim!=CountSeqIdInSip(sip)) 		 
		 ValMessage (salp, Err_Segs_DimSeqId_Not_Match, SEV_ERROR, NULL, sip , numseg+1);
		  
	     }
       }
     
     /*denseseg, packseg */
     else if(salp->segtype==2||salp->segtype==4)
       {
	 dsp=(DenseSegPtr) (salp->segs);
	 if(!dsp)
	   ValMessage (salp, Err_Null_Segs, SEV_ERROR, NULL, NULL, 0);
	 else
	   {
	     sip=dsp->ids;
	     if(dsp->dim==1)
	       ValMessage (salp, Err_SeqAlign_Dim_One, SEV_ERROR, NULL, sip , 0); 
	     if(dsp->dim!=CountSeqIdInSip(sip)) 
	        ValMessage (salp, Err_SeqAlign_DimSeqId_Not_Match, SEV_ERROR, NULL, sip , 0); 
	       
	   }
       }
     
     /*stdseg */
     else if(salp->segtype==3)
       {
	 
	 ssp=(StdSegPtr)salp->segs;
	 if(!ssp)
	   ValMessage (salp, Err_Null_Segs, SEV_ERROR, NULL, NULL, 0);
	 else
	   for(ssptemp=ssp, numseg=0; ssptemp!=NULL; ssptemp=ssptemp->next, numseg++)
	     {
	       
	       sip=SeqIdInAlignSegs((Pointer)ssptemp, 3, salp);
	       if(ssptemp->dim==1)
		 ValMessage (salp, Err_Segs_Dim_One, SEV_ERROR, NULL, sip , numseg+1);
	       if(ssptemp->dim!=CountSeqIdInSip( sip)) 
		 ValMessage (salp, Err_Segs_DimSeqId_Not_Match, SEV_ERROR, NULL, sip , numseg+1);
	       /*free Seqid if sip is a new chain created by SeqIdinAlignSegs*/
	       
	       SeqIdSetFree(sip);
	     }
       }
   }
}

/******************************************************************
return true if a sip is contained in a seg, or false if otherwise 
Note it returns FASLE for an empty seqloc.  
It also returns false if error in sip or ssp
******************************************************************/ 
static Boolean IsSipContainedInStdseg(SeqIdPtr sip, StdSegPtr ssp)
{
  SeqLocPtr slp, slptemp;
  
  if(!sip||!ssp)
    return FALSE;

  slp=ssp->loc;
  for(slptemp=slp; slptemp!=NULL; slptemp=slptemp->next)
    {
      if(slptemp->choice!=SEQLOC_EMPTY&&SeqIdCmp(sip, SeqLocId(slptemp)))
	return TRUE;
    }
  
  return FALSE;
}
 

/******************************************************************
check if an alignment is FASTA-like.  
If all gaps are at the end with dimensions>2, it's FASTA-like
******************************************************************/ 
static Boolean Is_Fasta_Seqalign (SeqAlignPtr salp)
{

  SeqIdPtr sip=NULL, siptemp=NULL, SipInSegs=NULL;
  DenseSegPtr dsp;
  Int4Ptr     startp;
  Boolean     gap, CheckedStatus;
  StdSegPtr ssp, ssptemp, ssptemp2;
  SeqLocPtr slp, slptemp;
  ValNodePtr FinishedSip=NULL, temp;
  PackSegPtr   psp=NULL;
  Uint1Ptr     seqpresence=NULL;
  Int4        k;
  Int2        j;
  
  /*check only global or partial type*/
  if(salp->type!=1&&salp->type!=3)
    return FALSE;
  
  if (salp->segtype==2) 
    {
      dsp = (DenseSegPtr) salp->segs;
      if(!dsp)
	ValMessage (salp, Err_Null_Segs, SEV_ERROR, NULL, NULL, 0);
      else
	{
	  if(dsp->dim<=2)
	    return FALSE;
	  
	  for (j=0, siptemp=dsp->ids; j<dsp->dim&&siptemp; j++, siptemp=siptemp->next)
	    {
	      gap=FALSE;
	      
	      for (k=0; k<dsp->numseg; k++)
		{
		  startp=dsp->starts;
		  
		  /*if start value is -1, set gap flag to true*/
		  if (startp[dsp->dim*k + j] < 0)
		    gap = TRUE;
		  /*if a positive start value is found after the initial -1 start value, then it's not  fasta like, no need to check this sequence further */
		  else if(gap)
		    break;
		  /*if no more positive start value is found after the initial -1 start value, then it's  fasta like */
		  if(k==dsp->numseg-1&&gap)
		    {
		      ValMessage (salp, Err_Fastalike, SEV_WARNING, siptemp, dsp->ids, 0); 
		      return TRUE;
		    }
		}
	    }
	}
    }
  /*packseg*/
  else if(salp->segtype==4)
    {
      psp=(PackSegPtr)salp->segs;
      if(!psp)
	ValMessage (salp, Err_Null_Segs, SEV_ERROR, NULL, NULL, 0);
      if(psp->dim<=2)
	return FALSE;
      else
	{	  
	  if(psp->present)
	    {
	      BSSeek(psp->present, 0, SEEK_SET);
	      seqpresence=MemNew(BSLen(psp->present));
	      if(!seqpresence)
		{
		  
		  ErrPostEx (SEV_ERROR, 0,0,  "Warning:insufficient memory");
		  return FALSE;
		  
		}
	      BSRead(psp->present, seqpresence, BSLen(psp->present));
	      /*go through each sequence*/
	      for (j=0, siptemp=psp->ids; j<psp->dim&&siptemp; j++, siptemp=siptemp->next)
		{
		  gap=FALSE;
		  /*go through each segment*/
		  for (k=0; k<psp->numseg; k++)
		    {
		      
		      /*if a segment is not present, set gap flag to true*/
		     if(!(seqpresence[(k*psp->dim+j)/8]&jybitnum[(k*psp->dim+j)%8]))
			gap = TRUE;     
		     /*if a segment is found later, then it's not  fasta like, no need to check this sequence further */
		     else if(gap)
		       break;
		     /*if no more segment is found for this sequence, then it's  fasta like */
		     if(k==psp->numseg-1&&gap)
		       {
			 ValMessage (salp, Err_Fastalike, SEV_WARNING, siptemp, psp->ids, 0); 
			 return TRUE;
		       }
		   
		    }
		}
	      MemFree(seqpresence);
	    }
	}
      
    }

  /*stdseg*/
  else if(salp->segtype==3)
    {    
      ssp=(StdSegPtr)salp->segs;
      if(!ssp)
	ValMessage (salp, Err_Null_Segs, SEV_ERROR, NULL, NULL, 0);
      else
	for(ssptemp=ssp; ssptemp->next!=NULL; ssptemp=ssptemp->next)
	  {
	    if(ssptemp->dim<=2)
	      continue;

	    SipInSegs=SeqIdInAlignSegs((Pointer)ssptemp, 3, salp);
	    slp=ssptemp->loc;
	    for(slptemp=slp; slptemp!=NULL; slptemp=slptemp->next)
	      {
		gap=FALSE;
		CheckedStatus=FALSE;
		sip=SeqLocId(slptemp);
		if(sip)
		  {
		    /*if a sip has been checked, skip it*/
		    for(temp=FinishedSip; temp!=NULL; temp=temp->next)
		      {
			if(SeqIdCmp(sip, temp->data.ptrvalue))
			  {
			    CheckedStatus=TRUE;
			    break;
			  }
		      }
		    /*determine whether a sequence (by following its sip) is fasta like */
		    if(!CheckedStatus)
		      {
			/*keep a record of  checked sip*/
			ValNodeAddPointer(&FinishedSip, 0, sip);
			
			/*go through each seg*/
			for(ssptemp2=ssptemp->next; ssptemp2!=NULL; ssptemp2=ssptemp2->next)
			  {
			    if(!IsSipContainedInStdseg(sip, ssptemp2))
			      gap=TRUE;
			    else if(gap)
			      break;
			    if(!ssptemp2->next&&gap)
			      {
				ValMessage (salp, Err_Fastalike, SEV_WARNING, sip, SipInSegs, 0);  
				ValNodeFree(FinishedSip);
				SeqIdSetFree(SipInSegs);
				return TRUE;
			      }
			  }
		      }
		  }
	      }
	    
	    SeqIdSetFree(SipInSegs);
	  }
      ValNodeFree(FinishedSip);
    }
  /*no fasta like sequence is found*/
  return FALSE;
  
}  
  
 

/******************************************************************
check if there is a gap for all sequence in a segment
******************************************************************/ 
static void Segment_Gap_In_SeqAlign(SeqAlignPtr salp)
{
  Int4Ptr      stptr=NULL;
  DenseSegPtr  dsp=NULL;
  DenseDiagPtr ddp=NULL, ddptemp;
  StdSegPtr    ssp=NULL, ssptemp;
  PackSegPtr   psp=NULL;
  Uint1Ptr     seqpresence=NULL;
  Int2         numseg, aligndim, i, j; 
  SeqIdPtr     sip=NULL;
  SeqLocPtr    slp=NULL, slptemp;
  

  if(salp)
    {
      /*densediag*/
      if(salp->segtype==1)
	{
	  ddp=(DenseDiagPtr)salp->segs;
	  if(!ddp)
	    ValMessage (salp, Err_Null_Segs, SEV_ERROR, NULL, NULL, 0);
	  else
	    {
	      for(ddptemp=ddp, numseg=0; ddptemp!=NULL; ddptemp=ddptemp->next, numseg++)
		{
		  sip=ddptemp->id;
		  /*empty segment*/
		  if(ddptemp->dim==0)   
		    ValMessage (salp, Err_Segment_Gap, SEV_ERROR, NULL, sip, numseg+1);
		}
	    }
	}
 
   
      /*denseseg*/
     else if(salp->segtype==2)
	{
	  dsp=(DenseSegPtr)salp->segs;
	  if(!dsp)
	    ValMessage (salp, Err_Null_Segs, SEV_ERROR, NULL, NULL, 0);
	  else
	    {
	      numseg=dsp->numseg;
	      aligndim=dsp->dim; 		  
	      stptr=dsp->starts;
	      sip=dsp->ids;
	      
	      if(stptr==NULL)
		return;
	      
	      /*go through each segment*/
	      for(j=0; j<numseg; j++)
		{    
		  /*go through each sequence */
		  for(i=0; i<aligndim; i++)
		    {
		      
		      if(stptr[j*aligndim+i]==-1)
			{  
			  /*all starts are -1 in this segment*/
			  if(i==aligndim-1)
			    ValMessage (salp, Err_Segment_Gap, SEV_ERROR, NULL, sip, j+1);
			}
		      /*at least one start that is not -1*/
		      else
			break;
		      
		    }
		}
	    }
	}

        /*stdseg*/
     else if(salp->segtype==3)
	{
	  ssp=(StdSegPtr)salp->segs;
	  if(!ssp)
	    ValMessage (salp, Err_Null_Segs, SEV_ERROR, NULL, NULL, 0);
	  else
	    {
	      /*go through each segment*/
	      for(ssptemp=ssp, numseg=0; ssptemp!=NULL; ssptemp=ssptemp->next, numseg++)
		{
		  sip=SeqIdInAlignSegs((Pointer)ssptemp, 3, salp);
		  slp=ssptemp->loc;
		  /*go through each sequence*/
		  for(slptemp=slp; slptemp!=NULL; slptemp=slptemp->next)
		    { 
		      if(slptemp->choice==SEQLOC_EMPTY||slptemp->choice==SEQLOC_NULL) 
			{
			  if(slptemp->next)   
			    continue;
			  /*all seqloc are empty*/ 
			  else
			    ValMessage (salp, Err_Segment_Gap, SEV_ERROR, NULL, sip, numseg+1);
			}
		      /*at least one non-empty seqloc*/
		      else
			break;
		    }
		  /*free Seqid if sip is a new chain created by SeqIdinAlignSegs*/
		  SeqIdSetFree(sip);
 
		}
	    }
	}
      /*packseg*/
      else if(salp->segtype==4)
	{
	  psp=(PackSegPtr)salp->segs;
	  if(!psp)
	    ValMessage (salp, Err_Null_Segs, SEV_ERROR, NULL, NULL, 0);
	  else
	    {
	      numseg=psp->numseg;
	      aligndim=psp->dim; 		  
	      sip=psp->ids;
	      if(psp->present)
		{
		  BSSeek(psp->present, 0, SEEK_SET);
		  seqpresence=MemNew(BSLen(psp->present));
		  if(!seqpresence)
		    {
		      ErrPostEx (SEV_ERROR, 0,0,  "Warning:insufficient memory");
		      return;
		      
		    }
		  BSRead(psp->present, seqpresence, BSLen(psp->present));
		  
		  /*go through each segment*/
		  for(j=0; j<numseg; j++)
		    {    
		      /*go through each sequence */
		      for(i=0; i<aligndim; i++)
			{
			  /*check the presence of each sequence by determining the bit value in a byte (0, not present; otherwise present)*/
			  if(!(seqpresence[(j*aligndim+i)/8]&jybitnum[(j*aligndim+i)%8]))      
			    {
			      /*more sequence to go*/
			      if(i<aligndim-1)   
				continue;
			      /*no sequence is present in this segment*/
			      else if(i==aligndim-1)
				ValMessage (salp, Err_Segment_Gap, SEV_ERROR, NULL, sip, j+1);
			    }
			  /*at least one sequence is present*/
			  else
			    break;
			}
		    }
		MemFree(seqpresence);
		}
	    }
	
	}


    }
}
 
/******************************************************************
validate seqid, segment length, strand in Seqalignment for Denseseg, 
Densediag and Stdseg.  Also check if it's FASTA-like
******************************************************************/ 
static Boolean ValidateSeqAlignFunc (SeqAlignPtr salp, Boolean find_remote_bsp)
{
  Boolean   error=FALSE;
  
  if(salp==NULL)
	return FALSE;

  /*validate if dimesion equals number of seqid*/     
  ValidateDimSeqIds (salp);
        
  if (find_remote_bsp) {
	ValidateSeqIdInSeqAlign (salp);
	ValidateSeqlengthinSeqAlign (salp);
  }
  /*validate strand*/
  ValidateStrandinSeqAlign (salp);
       
  /*validate Fasta like*/
  Is_Fasta_Seqalign (salp);
      
  /*validate segment gap*/
  Segment_Gap_In_SeqAlign (salp);
  return error;
}



/******************************************************************
validate each alignment sequentially.  
This function will subject the seqalign to all validation functions
******************************************************************/ 
NLM_EXTERN Boolean ValidateSeqAlign (SeqAlignPtr salp, Uint2 entityID, Boolean message,
                         Boolean msg_success, Boolean find_remote_bsp,
                         Boolean delete_bsp, Boolean delete_salp, BoolPtr dirty)
{  
  SeqAlignPtr  pre,
               salptmp;
  SaVal        sv;
  SaValPtr     svp;
  ValNodePtr   vnp;
  JYErrorMsgPtr bemp;
  MsgAnswer    ans;
  Int2         err_count=0,
               salp_count=0;
  Boolean      retdel = FALSE; 

  if(salp!=NULL)
  {
     sv.message = message;
     sv.msg_success = msg_success;
     sv.find_remote_bsp = find_remote_bsp;
     sv.delete_salp = delete_salp;
     sv.delete_bsp = delete_bsp;
     sv.retdel = TRUE;
     sv.do_hist_assembly = FALSE;
     sv.ids = NULL;
     sv.entityID = entityID; 
     sv.dirty = FALSE;   
     svp = &sv;   
     pre=NULL;
     salptmp=salp; 
     while (salptmp)
     {
        salp_count++;
        if(salptmp->segtype==5)
        {
           ValidateSeqAlign ((SeqAlignPtr) (salptmp->segs), entityID, message, msg_success, find_remote_bsp, delete_bsp, delete_salp, &svp->dirty);
        } 
        else if (salptmp->segtype<1 || salptmp->segtype>4)
        {
           ValMessage (salp, Err_Segtype, SEV_ERROR, NULL, NULL, salptmp->segtype);
        }
        else {
       	   ValidateSeqAlignFunc (salptmp, svp->find_remote_bsp);
        }     	
       	if (errorp)
       	{
       	   if(svp->message)
       	   {
       	      for (vnp=errorp; vnp!=NULL; vnp=vnp->next)
       	      {
       	         bemp=(JYErrorMsgPtr)vnp->data.ptrvalue;
       	         ErrPostEx ((ErrSev) bemp->level, 0, 0, bemp->msg);
       	      }
       	      errorp = JYErrorChainDestroy (errorp);
       	   }
       	   if (svp->delete_salp)
       	   {
            if (pre==NULL) {
              salp=salptmp->next;
              salptmp->next = NULL;
              SeqAlignFree (salptmp);
              salptmp = salp;
            }
            else {
              pre->next = salptmp->next;
              salptmp->next = NULL;
              SeqAlignFree (salptmp);
              salptmp = pre->next;
            }
           }
       	   else {
       	      salptmp = salptmp->next;
       	   }
       	   err_count++;
           svp->retdel=FALSE;
        }
       	else {
       	   salptmp = salptmp->next;
       	}
     }
     if (err_count==0 && svp->msg_success) {
        if (salp_count>1)
           ans = Message (MSG_OK, "Validation test of %d alignments succeeded", salp_count);
        else
           ans = Message (MSG_OK, "Validation test of the alignment succeeded");
     }
     if (dirty)
        *dirty = svp->dirty;
     retdel = svp->retdel;
  }   
  return retdel;
} 


/******************************************************************
call back function for REGISTER_ALIGNVALIDATION defined in sequin4.c.  
Starting point for seqalign validation if user clicked on 
SeqalignValidation under menu Filer/Alignment.  
Either individual alignment or alignment block 
should be highlighted for this validation to work
******************************************************************/ 

NLM_EXTERN Int2 LIBCALLBACK ValidateSeqAlignFromData (Pointer data)
{ 
 
  OMProcControlPtr  ompcp;
  SeqAlignPtr       salp=NULL;
  SeqAnnotPtr       sap=NULL;
  SeqEntryPtr       sep=NULL;
  
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
      /*if clicked on alignment block*/
    case OBJ_SEQANNOT:
      sap=(SeqAnnotPtr) (ompcp->input_data);
      break;
      /*if clicked on individual alignment*/
    case OBJ_SEQALIGN:
      salp=(SeqAlignPtr) (ompcp->input_data);
      break;
    case 0 :
      return OM_MSG_RET_ERROR;
    default :
      return OM_MSG_RET_ERROR;
  }
  
  ErrSetMessageLevel(SEV_ERROR);
  if(sap!=NULL)
  {
     salp=is_salp_in_sap(sap, 2);
     ValidateSeqAlign (salp, 0, TRUE, TRUE, TRUE, FALSE, FALSE, NULL);
  }
  if (salp!=NULL) {
     ValidateSeqAlign (salp, 0, TRUE, TRUE, TRUE, FALSE, FALSE, NULL);
  }
  if (sep!=NULL) {
     ValidateSeqAlignInSeqEntry (sep, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE);
  }
  return OM_MSG_RET_DONE;
}

static void ValidateSeqAlignInAnnot (SeqAnnotPtr sap, SaValPtr svp)

{
  SeqAlignPtr  salp;

  while (sap != NULL) {
    if (sap->type == 2) {
      salp = (SeqAlignPtr) sap->data;
      if (salp != NULL) {
        ValidateSeqAlign (salp, svp->entityID, svp->message, svp->msg_success, svp->find_remote_bsp, svp->delete_bsp, svp->delete_salp, &svp->dirty);
      }
    }
    sap = sap->next;
  }
}

static void ValidateSeqAlignInHist (SeqHistPtr hist, SaValPtr svp)

{
  SeqAlignPtr  salp;

  if (hist == NULL) return;
  for (salp = hist->assembly; salp != NULL; salp = salp->next) {
    ValidateSeqAlign (salp, svp->entityID, svp->message, svp->msg_success, svp->find_remote_bsp, svp->delete_bsp, svp->delete_salp, &svp->dirty);
  }
}

static void ValidateSeqAlignCallback (SeqEntryPtr sep, Pointer mydata,
                                          Int4 index, Int2 indent)
{
  BioseqPtr          bsp;
  BioseqSetPtr       bssp;
  SaValPtr           svp;

  if (sep != NULL && sep->data.ptrvalue && mydata != NULL) {
     svp = (SaValPtr)mydata;
     if (IS_Bioseq(sep)) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        if (bsp!=NULL) {
           ValidateSeqAlignInAnnot (bsp->annot, svp);
           if (svp != NULL && svp->do_hist_assembly) {
              ValidateSeqAlignInHist (bsp->hist, svp);
           }
        }
     }   
     else if(IS_Bioseq_set(sep)) {
        bssp = (BioseqSetPtr)sep->data.ptrvalue;
        if (bssp!=NULL) {
           ValidateSeqAlignInAnnot (bssp->annot, svp);
        }
     }
  }
}



NLM_EXTERN Boolean ValidateSeqAlignInSeqEntry (SeqEntryPtr sep, Boolean message, 
                                 Boolean msg_success, Boolean find_remote_bsp, 
                                 Boolean delete_bsp, Boolean delete_salp,
                                 Boolean do_hist_assembly)
{
  SeqEntryPtr      sep_head;
  Uint2            entityID;
  SaVal            sv;
  Boolean          success=TRUE;

  entityID = ObjMgrGetEntityIDForChoice (sep);
  if (entityID > 0) {
     sep_head = GetTopSeqEntryForEntityID (entityID);
     if (sep_head != NULL) {
        sv.message = message;
        sv.msg_success = msg_success;
        sv.find_remote_bsp = find_remote_bsp;
        sv.find_acc_bsp = FALSE;
        sv.delete_salp = delete_salp;
        sv.delete_bsp = delete_bsp;
        sv.retdel = TRUE;
        sv.do_hist_assembly = do_hist_assembly;
        sv.ids = NULL;
        sv.entityID = entityID; 
        sv.dirty = FALSE;
        SeqEntryExplore (sep_head, (Pointer)&sv, ValidateSeqAlignCallback);
        if (sv.dirty) {
           ObjMgrSetDirtyFlag (entityID, TRUE);
           ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
        }
        success = sv.retdel;
     }
  }
  return success;
}


/* alignment validator private for regular validator */

NLM_EXTERN Boolean ValidateSeqAlignWithinValidator (ValidStructPtr vsp, SeqEntryPtr sep, Boolean find_remote_bsp, Boolean do_hist_assembly);

NLM_EXTERN Boolean ValidateSeqAlignWithinValidator (ValidStructPtr vsp, SeqEntryPtr sep, Boolean find_remote_bsp, Boolean do_hist_assembly)

{
  GatherContext  gc;
  Boolean        rsult;

  if (vsp == NULL || sep == NULL) return FALSE;
  useLockByID = vsp->farIDsInAlignments;
  useValErr = TRUE;
  useVsp = vsp;
  vsp->gcp = &gc;
  vsp->bssp = NULL;
  vsp->bsp = NULL;
  vsp->sfp = NULL;
  vsp->descr = NULL;
  MemSet ((Pointer) &gc, 0, sizeof (GatherContext));
  rsult = ValidateSeqAlignInSeqEntry (sep, FALSE, FALSE, find_remote_bsp, FALSE, FALSE, do_hist_assembly);
  useLockByID = TRUE;
  useValErr = FALSE;
  useVsp = NULL;
  return rsult;
}

