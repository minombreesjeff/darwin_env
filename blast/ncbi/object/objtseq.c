#include <asn.h>

#define NLM_GENERATED_CODE_PROTO

#include <objtseq.h>

static Boolean loaded = FALSE;

#include <asntseq.h>

#ifndef NLM_EXTERN_LOADS
#define NLM_EXTERN_LOADS {}
#endif

NLM_EXTERN Boolean LIBCALL
objtseqAsnLoad(void)
{

   if ( ! loaded) {
      NLM_EXTERN_LOADS

      if ( ! AsnLoad ())
      return FALSE;
      loaded = TRUE;
   }

   return TRUE;
}



/**************************************************
*    Generated object loaders for Module NCBI-TSeq
*    Generated using ASNCODE Revision: 6.14 at Jun 28, 2002 12:46 PM
*
**************************************************/


/**************************************************
*
*    TSeqNew()
*
**************************************************/
NLM_EXTERN 
TSeqPtr LIBCALL
TSeqNew(void)
{
   TSeqPtr ptr = MemNew((size_t) sizeof(TSeq));

   return ptr;

}


/**************************************************
*
*    TSeqFree()
*
**************************************************/
NLM_EXTERN 
TSeqPtr LIBCALL
TSeqFree(TSeqPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> accver);
   MemFree(ptr -> sid);
   MemFree(ptr -> local);
   MemFree(ptr -> orgname);
   MemFree(ptr -> defline);
   MemFree(ptr -> sequence);
   return MemFree(ptr);
}


/**************************************************
*
*    TSeqAsnRead()
*
**************************************************/
NLM_EXTERN 
TSeqPtr LIBCALL
TSeqAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   TSeqPtr ptr;

   if (! loaded)
   {
      if (! objtseqAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* TSeq ::= (self contained) */
      atp = AsnReadId(aip, amp, TSEQ);
   } else {
      atp = AsnLinkType(orig, TSEQ);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = TSeqNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == TSEQ_seqtype) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> seqtype = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TSEQ_gi) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> gi = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TSEQ_accver) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> accver = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TSEQ_sid) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> sid = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TSEQ_local) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> local = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TSEQ_taxid) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> taxid = av.intvalue;
      ptr -> OBbits__ |= 1<<1;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TSEQ_orgname) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> orgname = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TSEQ_defline) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> defline = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TSEQ_length) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> length = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TSEQ_sequence) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> sequence = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = TSeqFree(ptr);
   goto ret;
}



/**************************************************
*
*    TSeqAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
TSeqAsnWrite(TSeqPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objtseqAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, TSEQ);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> seqtype;
   retval = AsnWrite(aip, TSEQ_seqtype,  &av);
   if (ptr -> gi || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> gi;
      retval = AsnWrite(aip, TSEQ_gi,  &av);
   }
   if (ptr -> accver != NULL) {
      av.ptrvalue = ptr -> accver;
      retval = AsnWrite(aip, TSEQ_accver,  &av);
   }
   if (ptr -> sid != NULL) {
      av.ptrvalue = ptr -> sid;
      retval = AsnWrite(aip, TSEQ_sid,  &av);
   }
   if (ptr -> local != NULL) {
      av.ptrvalue = ptr -> local;
      retval = AsnWrite(aip, TSEQ_local,  &av);
   }
   if (ptr -> taxid || (ptr -> OBbits__ & (1<<1) )){   av.intvalue = ptr -> taxid;
      retval = AsnWrite(aip, TSEQ_taxid,  &av);
   }
   if (ptr -> orgname != NULL) {
      av.ptrvalue = ptr -> orgname;
      retval = AsnWrite(aip, TSEQ_orgname,  &av);
   }
   if (ptr -> defline != NULL) {
      av.ptrvalue = ptr -> defline;
      retval = AsnWrite(aip, TSEQ_defline,  &av);
   }
   av.intvalue = ptr -> length;
   retval = AsnWrite(aip, TSEQ_length,  &av);
   if (ptr -> sequence != NULL) {
      av.ptrvalue = ptr -> sequence;
      retval = AsnWrite(aip, TSEQ_sequence,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    TSeqSetFree()
*
**************************************************/
NLM_EXTERN 
TSeqSetPtr LIBCALL
TSeqSetFree(TSeqSetPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) TSeqFree);
   return NULL;
}


/**************************************************
*
*    TSeqSetAsnRead()
*
**************************************************/
NLM_EXTERN 
TSeqSetPtr LIBCALL
TSeqSetAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   TSeqSetPtr ptr;

   if (! loaded)
   {
      if (! objtseqAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* TSeqSet ::= (self contained) */
      atp = AsnReadId(aip, amp, TSEQSET);
   } else {
      atp = AsnLinkType(orig, TSEQSET);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) TSeqAsnRead, (AsnOptFreeFunc) TSeqFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = TSeqSetFree(ptr);
   goto ret;
}



/**************************************************
*
*    TSeqSetAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
TSeqSetAsnWrite(TSeqSetPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objtseqAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, TSEQSET);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) TSeqAsnWrite, aip, atp, TSEQSET_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}

/* manually added */

#include <seqport.h>
#include <gather.h>
#include <tofasta.h>

static void LIBCALLBACK SaveTseqSequence (
  CharPtr sequence,
  Pointer userdata
)

{
  Char          ch;
  CharPtr       str;
  CharPtr       tmp;
  CharPtr PNTR  tmpp;

  tmpp = (CharPtr PNTR) userdata;
  tmp = *tmpp;

  str = sequence;
  if (sequence == NULL) return;
  ch = *str;
  while (ch != '\0') {
    if (ch == '\n' || ch == '\r' || ch == '\t') {
      *str = ' ';
    }
    str++;
    ch = *str;
  }
  TrimSpacesAroundString (sequence);

  tmp = StringMove (tmp, sequence);

  *tmpp = tmp;
}

NLM_EXTERN TSeqPtr BioseqToTSeq (BioseqPtr bsp)

{
	TSeqPtr tsp;
	SeqIdPtr sip;
	Char buf[255];
	CharPtr accession = NULL, organism = NULL, title = NULL;
	CharPtr seq;
	Uint1 seqcode;
	ValNodePtr vnp;
	BioSourcePtr biosp;
	OrgRefPtr orp;
	DbtagPtr dbp;
	ObjectIdPtr oip;

	if (bsp == NULL) return NULL;
	tsp = TSeqNew ();
	if (ISA_aa(bsp->mol))
		tsp->seqtype = TSeq_seqtype_protein;
	else
		tsp->seqtype = TSeq_seqtype_nucleotide;

	for (sip = bsp->id; sip != NULL; sip = sip->next)
	{
		switch (sip->choice)
		{
			case SEQID_GI:
				tsp->gi = sip->data.intvalue;
				break;
			case SEQID_GENBANK:
			case SEQID_DDBJ:
			case SEQID_EMBL:
			case SEQID_SWISSPROT:
		    case SEQID_OTHER:
		    case SEQID_TPG:
		    case SEQID_TPE:
		    case SEQID_TPD:
				SeqIdWrite(sip, buf, PRINTID_TEXTID_ACC_VER, 250);
				tsp->accver = StringSave(buf);
				break;
			default:
				SeqIdWrite(sip, buf, PRINTID_FASTA_SHORT, 250);
				tsp->sid = StringSave(buf);
				break;
		}
	}

	CreateDefLine(NULL, bsp, buf, 250, 0, accession, organism);
	tsp->defline = StringSave(buf);

	vnp = GetNextDescriptorUnindexed (bsp, Seq_descr_source, NULL);
	if (vnp != NULL && vnp->data.ptrvalue != NULL) {
		biosp = (BioSourcePtr)(vnp->data.ptrvalue);
		orp = biosp->org;
		if (orp != NULL) {
			if (orp->taxname != NULL)
				tsp->orgname = StringSave(orp->taxname);
			else if (orp->common != NULL)
				tsp->orgname = StringSave(orp->common);

			for (vnp = orp->db; vnp != NULL; vnp = vnp->next)
			{
				dbp = (DbtagPtr)(vnp->data.ptrvalue);
				if (! StringICmp("taxon", dbp->db))
				{
					oip = dbp->tag;
					tsp->taxid = oip->id;
					break;
				}
			}
		}
	}

	tsp->length = bsp->length;
	
	if (ISA_aa(bsp->mol))
	{
		seqcode = Seq_code_ncbieaa;
	}
	else
	{
		seqcode = Seq_code_iupacna;
	}

	seq = MemNew(bsp->length + 1);
	if (seq != NULL) {
		tsp->sequence = seq;
		SeqPortStream (bsp, STREAM_EXPAND_GAPS, (Pointer) &seq, SaveTseqSequence);
	}

	return tsp;
}

NLM_EXTERN Boolean AsnPrintString (CharPtr the_string, AsnIoPtr aip);

static void LIBCALLBACK TSeqStreamProc (CharPtr sequence, Pointer userdata)

{
  AsnIoPtr  aip;

  aip = (AsnIoPtr) userdata;

  AsnPrintString (sequence, aip);
}

static Boolean LIBCALL StreamTSeq (Pointer object, AsnIoPtr aip)

{
  SeqPortStream ((BioseqPtr) object, STREAM_EXPAND_GAPS, (Pointer) aip, TSeqStreamProc);
  return TRUE;
}

static TSeqPtr BioseqToMiniTSeq (BioseqPtr bsp)

{
	TSeqPtr tsp;
	SeqIdPtr sip;
	Char buf[255];
	CharPtr accession = NULL, organism = NULL, title = NULL;
	Uint1 seqcode;
	ValNodePtr vnp;
	BioSourcePtr biosp;
	OrgRefPtr orp;
	DbtagPtr dbp;
	ObjectIdPtr oip;

	if (bsp == NULL) return NULL;
	tsp = TSeqNew ();
	if (ISA_aa(bsp->mol))
		tsp->seqtype = TSeq_seqtype_protein;
	else
		tsp->seqtype = TSeq_seqtype_nucleotide;

	for (sip = bsp->id; sip != NULL; sip = sip->next)
	{
		switch (sip->choice)
		{
			case SEQID_GI:
				tsp->gi = sip->data.intvalue;
				break;
			case SEQID_GENBANK:
			case SEQID_DDBJ:
			case SEQID_EMBL:
			case SEQID_SWISSPROT:
		    case SEQID_OTHER:
		    case SEQID_TPG:
		    case SEQID_TPE:
		    case SEQID_TPD:
				SeqIdWrite(sip, buf, PRINTID_TEXTID_ACC_VER, 250);
				tsp->accver = StringSave(buf);
				break;
			default:
				SeqIdWrite(sip, buf, PRINTID_FASTA_SHORT, 250);
				tsp->sid = StringSave(buf);
				break;
		}
	}

	CreateDefLine(NULL, bsp, buf, 250, 0, accession, organism);
	tsp->defline = StringSave(buf);

	vnp = GetNextDescriptorUnindexed (bsp, Seq_descr_source, NULL);
	if (vnp != NULL && vnp->data.ptrvalue != NULL) {
		biosp = (BioSourcePtr)(vnp->data.ptrvalue);
		orp = biosp->org;
		if (orp != NULL) {
			if (orp->taxname != NULL)
				tsp->orgname = StringSave(orp->taxname);
			else if (orp->common != NULL)
				tsp->orgname = StringSave(orp->common);

			for (vnp = orp->db; vnp != NULL; vnp = vnp->next)
			{
				dbp = (DbtagPtr)(vnp->data.ptrvalue);
				if (! StringICmp("taxon", dbp->db))
				{
					oip = dbp->tag;
					tsp->taxid = oip->id;
					break;
				}
			}
		}
	}

	tsp->length = bsp->length;
	
	if (ISA_aa(bsp->mol))
	{
		seqcode = Seq_code_ncbieaa;
	}
	else
	{
		seqcode = Seq_code_iupacna;
	}

	/*
	seq = MemNew(bsp->length + 1);
	if (seq != NULL) {
		tsp->sequence = seq;
		SeqPortStream (bsp, STREAM_EXPAND_GAPS, (Pointer) &seq, SaveTseqSequence);
	}
	*/

	return tsp;
}

NLM_EXTERN Boolean BioseqAsnWriteAsTSeq (BioseqPtr bsp, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;
   TSeqPtr ptr = NULL;

   if (! loaded)
   {
      if (! objtseqAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, TSEQ);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   ptr = BioseqToMiniTSeq (bsp);
 
   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> seqtype;
   retval = AsnWrite(aip, TSEQ_seqtype,  &av);
   if (ptr -> gi || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> gi;
      retval = AsnWrite(aip, TSEQ_gi,  &av);
   }
   if (ptr -> accver != NULL) {
      av.ptrvalue = ptr -> accver;
      retval = AsnWrite(aip, TSEQ_accver,  &av);
   }
   if (ptr -> sid != NULL) {
      av.ptrvalue = ptr -> sid;
      retval = AsnWrite(aip, TSEQ_sid,  &av);
   }
   if (ptr -> local != NULL) {
      av.ptrvalue = ptr -> local;
      retval = AsnWrite(aip, TSEQ_local,  &av);
   }
   if (ptr -> taxid || (ptr -> OBbits__ & (1<<1) )){   av.intvalue = ptr -> taxid;
      retval = AsnWrite(aip, TSEQ_taxid,  &av);
   }
   if (ptr -> orgname != NULL) {
      av.ptrvalue = ptr -> orgname;
      retval = AsnWrite(aip, TSEQ_orgname,  &av);
   }
   if (ptr -> defline != NULL) {
      av.ptrvalue = ptr -> defline;
      retval = AsnWrite(aip, TSEQ_defline,  &av);
   }
   av.intvalue = ptr -> length;
   retval = AsnWrite(aip, TSEQ_length,  &av);
   if (bsp != NULL) {
      av.ptrvalue = bsp; /* pass bsp to SeqPortStream */
      retval = AsnWriteEx(aip, TSEQ_sequence,  &av, StreamTSeq);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   TSeqFree (ptr);
   return retval;
}

