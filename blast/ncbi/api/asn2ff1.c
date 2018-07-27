/*   asn2ff1.c
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
* File Name:  asn2ff1.c
*
* Author:  Karl Sirotkin, Tom Madden, Tatiana Tatusov
*
* Version Creation Date:   7/15/95
*
* $Revision: 6.118 $
* $Revision: 6.118 $
*
* File Description:  files that go with "asn2ff"
*
* Modifications:  
* --------------------------------------------------------------------------
* ==========================================================================
*/

#include <seqmgr.h>
#include <gather.h>
#include <asn2ffg.h>
#include <asn2ffp.h>
#include <a2ferrdf.h>
#include <parsegb.h>
#include <gbfeat.h>
#include <ffprint.h>
#include <tofasta.h>
#include <subutil.h>
#include <explore.h>
#include <objfdef.h>
#include <sqnutils.h>
#include <lsqfetch.h>

#ifdef ENABLE_ENTREZ
#include <accentr.h>
#endif

#define NUM_OF_ESTIMATES 20

static void PrintSeqRegion (Asn2ffJobPtr ajp, GBEntryPtr gbp);

static Int2 line_estimate[NUM_OF_ESTIMATES] = {
1, /* 0; Locus, Segment, Base Count, Origin, Feature Header lines */
1, /* 1; Definition line(s) */
1, /* 2; Accession line(s) */
1, /* 3; Keyword line(s) */
2, /* 4; Source lines */
6, /* 5; Reference (pub) lines */
10, /* 6; GBComAndFH */
11, /* 7; GBComAndXref */
6, /* 8; Features */
NUM_SEQ_LINES, /* 9; Sequence lines */
2, /* 10; EMBL Date lines */
2, /* 11; EMBL Organism lines */
4, /* 12; DBSOURCE field */
};

/* The following corresponds to NUM_SEQ_LINES lines each with 60 
residues/basepairs */
#define SEQ_BLK_SIZE (60*NUM_SEQ_LINES)
#define A2F_OTHER ( (Uint1)0)
#define A2F_SOURCE_FEATURE ( (Uint1)1)
#define A2F_FEATURE ( (Uint1)2)
#define A2F_REFERENCE ( (Uint1)3)
#define A2F_FEATURE_NEW ( (Uint1)4)
#define A2F_COMMENT ( (Uint1)5)
#define A2F_SEQUENCE ( (Uint1)6)

static Boolean Template_load = FALSE;

/* ---------------Function Prototypes ---------------*/
Int4 asn2pr_setup PROTO ((Asn2ffJobPtr ajp, FFPrintArrayPtr PNTR papp));
Int4 asn2hp_setup PROTO ((Asn2ffJobPtr ajp, FFPrintArrayPtr PNTR papp));
Int4 asn2gb_setup PROTO ((Asn2ffJobPtr ajp, FFPrintArrayPtr PNTR papp));
Int4 asn2embl_setup PROTO ((Asn2ffJobPtr ajp, FFPrintArrayPtr PNTR papp));
Int4 asn2gp_setup PROTO ((Asn2ffJobPtr ajp, FFPrintArrayPtr PNTR papp));
Int4 asn2ep_setup PROTO ((Asn2ffJobPtr ajp, FFPrintArrayPtr PNTR papp));
Int4 asn2gr_setup PROTO ((Asn2ffJobPtr ajp, FFPrintArrayPtr PNTR papp));
void PrintTaxonomy  PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));
void LoadPap PROTO ((FFPrintArrayPtr pap, FFPapFct fct, Asn2ffJobPtr ajp, Int4 index, Uint1 last, Uint1 printxx, Int2 estimate, Uint1 element_type, GBEntryPtr gbp));

void CheckSeqPort PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp, Int4 start));
void PrintGenome PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));
void GetMolInfo PROTO ((Asn2ffJobPtr ajp, CharPtr buffer, GBEntryPtr gbp));
CharPtr GetPDBSourceLine PROTO ((PdbBlockPtr pdb));
void PrintDateLines PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));
void PrintXrefLine PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));
Boolean CheckXrefLine PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));
void set_flags PROTO ((Asn2ffJobPtr ajp));
void PrintSeqBlk PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));


#define  TOTAL_ESTKW         11
#define  TOTAL_STSKW         5
#define  TOTAL_GSSKW         2

static CharPtr EST_kw_array[TOTAL_ESTKW] = {
     "EST", "EST PROTO((expressed sequence tag)", "expressed sequence tag",
     "EST (expressed sequence tag)", "EST(expressed sequence tag)",
     "partial cDNA sequence", "transcribed sequence fragment", "TSR",
     "putatively transcribed partial sequence", "UK putts"
     };

static CharPtr GSS_kw_array[TOTAL_GSSKW] = {
     "GSS", "trapped exon"
     };
static CharPtr STS_kw_array[TOTAL_STSKW] = {
     "STS", "STS(sequence tagged site)", "STS (sequence tagged site)", 
     "STS sequence", "sequence tagged site"
     };

static Int2 MatchArrayString(CharPtr array_string[], Int2 totalstr, CharPtr text)
{
   Int2 i;

   for (i = 0; i < totalstr && text != NULL; i++)
       if (StringCmp(array_string[i], text) == 0)
          return (i);

   return (-1);

} /* MatchArrayString */

/***************************************************************************
 *	 Using the chain that was spliced on, we can reconize the splice    
 *	 and break it.                     
 ****************************************************************************/
void FlatSpliceOff (SeqEntryPtr the_set, ValNodePtr desc)
{
      BioseqSetPtr bss;
      BioseqPtr bs;
      ValNodePtr PNTR desc_head=NULL;
      ValNodePtr PNTR desc_target=NULL;
      ValNodePtr scan;

      if (IS_Bioseq(the_set) ){
         bs = (BioseqPtr) the_set -> data.ptrvalue;
         desc_head = & (bs -> descr);
      }else{
         bss = (BioseqSetPtr) the_set -> data.ptrvalue;
         desc_head = & (bss -> descr);
      }  
      if (* desc_head){
         desc_target = desc_head;
         for (scan = * desc_head; scan; scan = scan -> next){
            if (scan == desc){
               * desc_target = NULL;
               break;
            }
            desc_target = & (scan -> next);
         }
      }   

}

void FlatSpliceOn (SeqEntryPtr the_set, ValNodePtr desc)
{
      BioseqSetPtr bss;
      BioseqPtr bs;

      if (IS_Bioseq(the_set) ){
         bs = (BioseqPtr) the_set -> data.ptrvalue;
         bs -> descr = tie_next(bs -> descr, desc);
      } else {
         bss = (BioseqSetPtr) the_set -> data.ptrvalue;
         bss -> descr = tie_next(bss -> descr, desc);
      }  
}

/**************************************************************************
*	Get the ValNodePtr associated with a certain reference.
**************************************************************************/

static void GetPapRefPtr (Asn2ffJobPtr ajp, GBEntryPtr gbp, Int4 ext_index, Int4 pap_index, FFPrintArrayPtr pap)

{
	PubStructPtr psp=NULL;
	ValNodePtr vnp;
	Int4 i;
	DescrStructPtr dsp;
	
	for (vnp=gbp->Pub, i=0; vnp && i < ext_index; vnp=vnp->next, i++);
	if (vnp == NULL) {
		return;
	}
	psp = (PubStructPtr)vnp->data.ptrvalue;
	if (psp == NULL) {
		return;
	}
	if ((dsp = pap[pap_index].descr) == NULL) {
		dsp =  (DescrStructPtr) MemNew(sizeof(DescrStruct));
		pap[pap_index].descr = dsp;
	}
	dsp->entityID = psp->entityID;
	dsp->itemID = psp->itemID;
	dsp->itemtype = psp->itemtype;
	
	return;
}

/**************************************************************************
*	Get the Comment structure associated with a certain comment block.
**************************************************************************/

static void GetPapCommPtr (Asn2ffJobPtr ajp, GBEntryPtr gbp, Int4 ext_index, Int4 pap_index, FFPrintArrayPtr pap)

{
	ComStructPtr s=NULL;
	Int4 i;
	DescrStructPtr dsp;
	
	for (s=gbp->comm, i=0; s && i < ext_index; s=s->next, i++);
	if (s == NULL) {
		return;
	}
	if ((dsp = pap[pap_index].descr) == NULL) {
		dsp =  (DescrStructPtr) MemNew(sizeof(DescrStruct));
		pap[pap_index].descr = dsp;
	}
	dsp->entityID = s->entityID;
	dsp->itemID = s->itemID;
	dsp->itemtype = s->itemtype;
	
	return;
}

/**************************************************************************
*	Find the SeqFeatPtr that is associated with this entry in the 
*	FFPrintArrayPtr.
*************************************************************************/

static void GetPapSeqFeatPtr (GBEntryPtr gbp, Int4 ext_index, Int4 pap_index, FFPrintArrayPtr pap)

{
	Int4 feat_index, index, listsize;
	OrganizeFeatPtr ofp;
	DescrStructPtr dsp;
	
	if (gbp == NULL || gbp->feat == NULL) {
		return;
	}
	ofp = gbp->feat;
	listsize=ofp->sfpListsize;
	index = (Int4) ext_index;

	feat_index = index - listsize;
	if (feat_index < 0) {
		if ((dsp = pap[pap_index].descr) == NULL) {
			dsp =  (DescrStructPtr) MemNew(sizeof(DescrStruct));
			pap[pap_index].descr = dsp;
		}
		dsp->entityID = ofp->List[index].entityID;
		dsp->itemID = ofp->List[index].itemID;
		dsp->itemtype = ofp->List[index].itemtype;
	}
	return;
}

NLM_EXTERN Boolean asn2ff_print (Asn2ffJobPtr ajp)
{
	AsnIoPtr			aip;
	CharPtr          string;
	FFPrintArrayPtr  pap = NULL;
	Int4             index, pap_size;
	Boolean 	 result = FALSE, hold = TRUE;
	DescrStructPtr	dsp = NULL;

	if ((ajp->sep == NULL && ajp->slp == NULL) ||
            (ajp->fp == NULL && ajp->ajp_print_data == NULL))
		return FALSE;
	if (ajp->no_hold)
		hold = FALSE;
	if (hold)
		ObjMgrSetHold();   /* hold any autoloaded records in memory */

    pap_size = asn2ff_setup (ajp, &pap);
    if(ajp->ajp_count_index != NULL)
        (*ajp->ajp_count_index)(ajp, pap_size, ajp->user_data);

    if (ajp->ssp && ajp->format == EMBL_FMT && ajp->fp != NULL) {
		aip = AsnIoNew(ASNIO_TEXT_OUT, ajp->fp, NULL, NULL, NULL);
		SubmitBlockAsnWrite(ajp->ssp->sub, aip, NULL);
		AsnIoFlush(aip);
		AsnIoReset(aip);
    }
    if (pap_size > 0) {
		if(ajp->fp != NULL)
			head_www(ajp->fp, ajp->sep);
		asn2ff_set_output (NULL, "\n");
		for (index = 0; index < pap_size; index++) {
			string = FFPrint (pap, index, pap_size);
			if (string != NULL && *string != '\0') {
				if(ajp->fp != NULL)
					ff_print_string (ajp->fp, string, "\n");
				if(ajp->ajp_print_data != NULL)
					(*ajp->ajp_print_data)(ajp, string, ajp->user_data);
				string = (char *)MemFree (string);
			} else if (ajp->null_str) {
				ErrPostStr(SEV_WARNING, ERR_PRINT_NullString, 
				"CAUTION: NULL String returned\n");
			}
			if (pap[index].descr) {
				pap[index].descr = (DescrStructPtr)MemFree(pap[index].descr);
			}
		}
                
		if(ajp->fp != NULL)
			tail_www(ajp->fp);
		result = TRUE;
		MemFree(pap);
	}
	if(ajp->ajp_print_index != NULL)
		(*ajp->ajp_print_index)(ajp, ajp->user_data);
	free_buff();
	asn2ff_cleanup (ajp); 
	if (hold)
		ObjMgrClearHold();
	if (ajp->free_cache)
		ObjMgrFreeCache(0);
	
	return result;
}

Asn2ffJobPtr Asn2ffJobCreate(SeqEntryPtr sep, SeqSubmitPtr ssp, SeqLocPtr slp, FILE *fp, Uint1 format, Uint1 mode, StdPrintOptionsPtr	Spop)
{
	Asn2ffJobPtr	ajp;
	Uint2			entityID = 0, itemID = 0;
	BioseqPtr bsp;
	
	ajp = (Asn2ffJobPtr) MemNew(sizeof(Asn2ffJob));
	ajp->show_seq = TRUE;
	ajp->show_gi = TRUE;
	ajp->error_msgs = TRUE;
	ajp->null_str = FALSE;
	ajp->non_strict = TRUE;
	ajp->format = format;
	ajp->mode = mode;
	ajp->show_gene = TRUE;
	ajp->gb_style = TRUE;
	ajp->fp = fp;
	ajp->Spop = Spop;
	if (ssp != NULL) {
		if ((entityID = ObjMgrGetEntityIDForPointer(ssp)) == 0) {
			ErrPostStr(SEV_WARNING, 0, 0, "Couldn't get entityID");
			MemFree(ajp);
			return NULL;
		}
		ajp->ssp = ssp;
		ajp->sep = (SeqEntryPtr) ssp->data;
	} else if (slp != NULL) {
		if ((bsp = BioseqFindFromSeqLoc(slp)) != NULL) {
    		entityID = ObjMgrGetEntityIDForPointer (bsp);
    	} else {
			fprintf(stderr, "Couldn't get Bioseq from location %s", SeqLocPrint(slp));
			MemFree(ajp);
			return NULL;
		}
		ajp->slp = slp;
    	ajp->sep = NULL;
	} else {
		if ((entityID = ObjMgrGetEntityIDForChoice(sep)) == 0) {
			ErrPostStr(SEV_WARNING, 0, 0, "Couldn't get entityID");
			MemFree(ajp);
			return NULL;
		}
		ajp->sep = sep;
	} 
	ajp->entityID = entityID;
	
	return ajp;
}

/**********************************************************/
NLM_EXTERN LinkStrPtr asn2ff_print_to_mem(Asn2ffJobPtr ajp, LinkStrPtr lsp)
{
    CharPtr         string;
    FFPrintArrayPtr pap = NULL;
    Int4            index, pap_size;
    Boolean         hold = TRUE;
    DescrStructPtr  dsp;
    DescrStructPtr  dspnext;

    if(ajp->sep == NULL && ajp->slp == NULL)
        return(lsp);

    if(ajp->no_hold)
        hold = FALSE;
    else
        ObjMgrSetHold();        /* hold any autoloaded records in memory */

    pap_size = asn2ff_setup(ajp, &pap);

    if(pap_size > 0) {
        for(index = 0; index < pap_size; index++) {
            asn2ff_set_output(NULL, "\n");
            string = FFPrint(pap, index, pap_size);
            if(string != NULL && *string != '\0') {
            	string = ff_print_string_mem(string);
                lsp->next = (LinkStrPtr) MemNew(sizeof(LinkStr));
                lsp = lsp->next;
                lsp->next = NULL;
                lsp->line = string;
                string = NULL;
            } else if(ajp->null_str != FALSE) {
                ErrPostStr(SEV_WARNING, ERR_PRINT_NullString,
                           "CAUTION: NULL String returned\n");
            }
            if(pap[index].descr != NULL)
            {
                for(dsp = pap[index].descr; dsp != NULL; dsp = dspnext)
                {
                    dspnext = dsp->next;
                    MemFree(dsp);
                }
                pap[index].descr = NULL;
            }
        }
        MemFree(pap);
    }
    free_buff();
    asn2ff_cleanup(ajp);
    if(hold != FALSE)
        ObjMgrClearHold();
    if(ajp->free_cache)
        ObjMgrFreeCache(0);

    return(lsp);
}
static Boolean is_contig(BioseqPtr bsp)
{
	SeqLocPtr slp=NULL;
	DeltaSeqPtr dsp;
	SeqEntryPtr	oldscope;

	oldscope=SeqEntrySetScope(GetTopSeqEntryForEntityID(ObjMgrGetEntityIDForPointer(bsp)));
	
	if (bsp->repr == Seq_repr_seg || bsp->repr == Seq_repr_delta) {
		if (bsp->seq_ext_type == 1) {
			slp = (SeqLocPtr) bsp->seq_ext;
		} else if (bsp->seq_ext_type == 4) {
			for (dsp = (DeltaSeqPtr) bsp->seq_ext; dsp; dsp=dsp->next) {
				if (dsp->choice == 1) {  /* SeqLoc */
					slp = (SeqLocPtr)(dsp->data.ptrvalue);
					break;
				}
			}
		}
	}
	if (slp && BioseqFindCore(SeqLocId(slp)) == NULL) {
		SeqEntrySetScope(oldscope);
		return TRUE;
	}
	SeqEntrySetScope(oldscope);
	return FALSE;
}

/***********************************************************************
*
*	SeqEntryToFlatEx is a stand-alone function works as SeqEntryToFlat
*	takes SeqIdPtr and various types of the output
*
*	successful, TRUE is returned; otherwise FALSE is returned.
*
*	display =0,1 - generates CONTIG view only
*	display = 2	- generates CONTIG and all parts regardless of seqid
*
**************************************************************************/
NLM_EXTERN Boolean SeqEntryToEntrez (SeqEntryPtr sep, FILE *fp, SeqIdPtr seqid,  Uint1 format, Uint1 display, Int4 from, Int4 to, Uint1 strand)
{
	Boolean				rsult=FALSE;
	Asn2ffJobPtr		ajp=NULL;
	BioseqPtr 			bsp;
	BioseqSetPtr		bssp;
	SeqLocPtr 			slp = NULL;
	SeqLoc				sl;
	SeqInt				seqint;
	StdPrintOptionsPtr  Spop = NULL;
	SeqLocPtr			slp_region=NULL;

	if (sep == NULL) {
		return FALSE;
	}
	
	if (seqid == NULL) {  /*should never happen */
		if (IS_Bioseq(sep)) {
			if ((bsp = (BioseqPtr) sep->data.ptrvalue) == NULL) {
				return rsult;
			}
			if (is_contig(bsp)) {
				return (SeqEntryToFlatEx(sep, fp, format, 
							RELEASE_MODE, seqid, FF_TOP_CONTIG));
			}
			return (SeqEntryToFlatEx(sep, fp, format, 
							RELEASE_MODE, seqid, FF_REGULAR));
		} else {
    		if ((bssp = (BioseqSetPtr) sep->data.ptrvalue) == NULL) {
				return rsult;
    		}
    		if (bssp->_class == 7 ||
    	                 (bssp->_class >= 13 && bssp->_class <= 16)) {
      			for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        			rsult = SeqEntryToFlat (sep, fp, GENBANK_FMT, RELEASE_MODE);
				}
				return rsult;
			} else { 
				if (format == GENPEPT_FMT)	{
					return (SeqEntryToFlatEx(sep, fp, GENPEPT_FMT, 
							RELEASE_MODE, seqid, FF_TOP_COMPLETE));
				} else if (display == 1) {
					return (SeqEntryToFlatEx(sep, fp, GENBANK_FMT, 
							RELEASE_MODE, seqid, FF_TOP_CONTIG));
				} else {
					return (SeqEntryToFlatEx(sep, fp, GENBANK_FMT, 
							RELEASE_MODE, seqid, FF_REGULAR));
				}
			}
		}
	}
	if ((bsp = BioseqFind(seqid)) == NULL) {
		return FALSE;
	}
	if (from > 0 && to > 0) {
		seqint.from = from-1;
		seqint.to = to-1;
		seqint.id=seqid;
		seqint.strand=strand;
		seqint.if_from=NULL;
		seqint.if_to=NULL;
		sl.choice= SEQLOC_INT;
		sl.data.ptrvalue=&seqint;
		sl.next=NULL;
		slp_region=&sl;
	}
	if(format == GENPEPT_FMT) {
		if(Template_load == FALSE) {
		    PrintTemplateSetLoad("asn2ff.prt");
		    Template_load = TRUE;
		}
		Spop = StdPrintOptionsNew(NULL);
		if(Spop != NULL) {
		    Spop->newline = "~";
		    Spop->indent = "";
		} else {
		    ErrPostStr(SEV_FATAL, 0, 0, "StdPrintOptionsNew failed");
		    return FALSE;
		}
	}

	ajp = Asn2ffJobCreate(sep, NULL, slp_region, fp, format, RELEASE_MODE, Spop);
	if (ajp == NULL) {
		if(Spop) StdPrintOptionsFree(Spop);
		return FALSE;
	}
	ajp->show_version = TRUE;
	ajp->show_gi = FALSE;
	ajp->gb_style = FALSE;
	ajp->id_print = seqid;
	ajp->sep = sep;
	if(ajp->entityID > 0 && SeqMgrFeaturesAreIndexed (ajp->entityID) == 0) {
                 SeqMgrIndexFeatures (ajp->entityID, NULL);
        }

	if (bsp->repr == Seq_repr_seg || bsp->repr == Seq_repr_delta) {   
/* always print CONTIG view first */
		if (slp_region == NULL) {
		 	if (bsp->repr == Seq_repr_delta && !is_contig(bsp)) { 
				ajp->genome_view = FALSE;
/* delta with internal parts cannot use CONTIG view */
			} else {
				ajp->genome_view = TRUE;
			}
		}					
		ajp->only_one = TRUE;
		ajp->gb_style = FALSE;
		ajp->sep = sep;
		if (is_contig(bsp) && slp_region == NULL) { 
			ajp->contig_view = TRUE;
			ajp->show_seq = FALSE;
		}
		rsult = SeqEntryToFlatAjp(ajp, sep, fp, format, RELEASE_MODE);
		if (display == 2) { 
			ajp->only_one = FALSE;				
			ajp->contig_view = FALSE;
			ajp->genome_view = FALSE;
			ajp->gb_style = TRUE;
			ajp->id_print = NULL;
			ajp->only_one = FALSE;
			ajp->show_seq = TRUE;
			rsult = SeqEntryToFlatAjp(ajp, sep, fp, format, RELEASE_MODE);
		}
	} else {
		rsult = SeqEntryToFlatAjp(ajp, sep, fp, format, RELEASE_MODE);
	}
	if(ajp) MemFree(ajp);
	if(Spop) StdPrintOptionsFree(Spop);
	return rsult;
}

/**********************************************************/
static LinkStrPtr SeqEntryToLinkStr(Asn2ffJobPtr ajp, SeqEntryPtr sep,
                                    LinkStrPtr lsp, Uint1 format, Uint1 mode)
{
    StdPrintOptionsPtr Spop = NULL;
    BioseqSetPtr       bssp;
    Int2               tofree;

    if(sep == NULL)
        return(lsp);

    if(format == GENPEPT_FMT)
    {
        if(Template_load == FALSE)
        {
            PrintTemplateSetLoad("asn2ff.prt");
            Template_load = TRUE;
        }
        Spop = StdPrintOptionsNew(NULL);
        if(Spop != NULL)
        {
            Spop->newline = "~";
            Spop->indent = "";
        }
        else
        {
            ErrPostStr(SEV_FATAL, 0, 0, "StdPrintOptionsNew failed");
            return(lsp);
        }
    }
    if(ajp == NULL)
    {
        ajp = Asn2ffJobCreate(sep, NULL, NULL, NULL, format, mode, Spop);
        tofree = 1;
    }
    else
        tofree = 0;
	
    if(IS_Bioseq_set(sep) != 0 && ajp->id_print == NULL)
    {
        bssp = (BioseqSetPtr) sep->data.ptrvalue;
        if(bssp != NULL && (bssp->_class == 7 ||
                            (bssp->_class >= 13 && bssp->_class <= 16)))
        {
            for(sep = bssp->seq_set; sep != NULL; sep = sep->next)
            {
                lsp = SeqEntryToLinkStr(ajp, sep, lsp, format, mode);
            }
            return(lsp);
        }
    }
    if(ajp == NULL)
        return(lsp);

    ajp->sep = sep;

    lsp = asn2ff_print_to_mem(ajp, lsp);

    StdPrintOptionsFree(ajp->Spop);
    if(ajp != NULL && tofree != 0)
        MemFree(ajp);

    return(lsp);
}
/**********************************************************/
NLM_EXTERN CharPtr PNTR SeqEntryToStrArray(SeqEntryPtr sep, Uint1 format,
                                           Uint1 mode)
{
    LinkStrPtr   lsp;
    LinkStrPtr   tlsp;
    CharPtr PNTR res;
    CharPtr PNTR tres;
    Int4         num;

    lsp = (LinkStrPtr) MemNew(sizeof(LinkStr));
    lsp->next = NULL;
    lsp->line = NULL;
    SeqEntryToLinkStr(NULL, sep, lsp, format, mode);
    tlsp = lsp;
    lsp = lsp->next;
    MemFree(tlsp);

    for(tlsp = lsp, num = 1; tlsp != NULL; tlsp = tlsp->next, num++)
        continue;

    if(num == 1)
        return(NULL);

    res = (CharPtr PNTR) MemNew(sizeof(CharPtr) * num);
    for(tres = res; lsp != NULL; lsp = tlsp, tres++)
    {
        tlsp = lsp->next;
        *tres = lsp->line;
        MemFree(lsp);
    }
    *tres = NULL;
    return(res);
}

/**********************************************************/
static LinkStrPtr AjpToLinkStr(Asn2ffJobPtr ajp, SeqEntryPtr sep,
                               LinkStrPtr lsp)
{
    BioseqSetPtr bssp;

    if(sep == NULL)
        return(lsp);

    if(IS_Bioseq_set(sep) != 0 && ajp->id_print == NULL)
    {
        bssp = (BioseqSetPtr) sep->data.ptrvalue;
        if(bssp != NULL && (bssp->_class == 7 ||
                            (bssp->_class >= 13 && bssp->_class <= 16)))
        {
            for(sep = bssp->seq_set; sep != NULL; sep = sep->next)
            {
                lsp = AjpToLinkStr(ajp, sep, lsp);
            }
            return(lsp);
        }
    }
    if(ajp == NULL)
        return(lsp);

    ajp->sep = sep;

    lsp = asn2ff_print_to_mem(ajp, lsp);

    return(lsp);
}

/***********************************************************/
NLM_EXTERN CharPtr PNTR AjpToStrArray(Asn2ffJobPtr ajp)
{
    StdPrintOptionsPtr Spop = NULL;
    LinkStrPtr         lsp;
    LinkStrPtr         tlsp;
    CharPtr PNTR       res;
    CharPtr PNTR       tres;
    Int4               num;

    if(ajp == NULL || ajp->sep == NULL)
        return(NULL);

    if(ajp->format == GENPEPT_FMT)
    {
        if(Template_load == FALSE)
        {
            PrintTemplateSetLoad("asn2ff.prt");
            Template_load = TRUE;
        }
        Spop = StdPrintOptionsNew(NULL);
        if(Spop == NULL)
        {
            ErrPostStr(SEV_FATAL, 0, 0, "StdPrintOptionsNew failed");
            return(NULL);
        }
        Spop->newline = "~";
        Spop->indent = "";
        ajp->Spop = Spop;
    }

    lsp = (LinkStrPtr) MemNew(sizeof(LinkStr));
    lsp->next = NULL;
    lsp->line = NULL;
    AjpToLinkStr(ajp, ajp->sep, lsp);
    tlsp = lsp;
    lsp = lsp->next;
    MemFree(tlsp);

    if(ajp->Spop != NULL)
        StdPrintOptionsFree(ajp->Spop);

    for(tlsp = lsp, num = 1; tlsp != NULL; tlsp = tlsp->next, num++)
        continue;

    if(num == 1)
        return(NULL);

    res = (CharPtr PNTR) MemNew(sizeof(CharPtr) * num);
    for(tres = res; lsp != NULL; lsp = tlsp, tres++)
    {
        tlsp = lsp->next;
        *tres = lsp->line;
        MemFree(lsp);
    }
    *tres = NULL;
    return(res);
}

/**********************************************************
*	This function allocates memory for the linked list
***********************************************************/
static LinkStrPtr SeqEntryToLinkStrEx(Asn2ffJobPtr ajp, SeqEntryPtr sep, LinkStrPtr lsp, Uint1 format)
{
    BioseqSetPtr		bssp;
		
    if(sep == NULL || ajp == NULL)
        return(NULL);

    if(IS_Bioseq_set(sep) != 0) {
        bssp = (BioseqSetPtr) sep->data.ptrvalue;
        if(bssp != NULL && (bssp->_class == 7 ||
                            (bssp->_class >= 13 && bssp->_class <= 16))) {
            for(sep = bssp->seq_set; sep != NULL; sep = sep->next) {
                lsp = SeqEntryToLinkStrEx(ajp, sep, lsp, format);
            }
            return(lsp);
        }
    }


    lsp = asn2ff_print_to_mem(ajp, lsp);
    return(lsp);
}

/**********************************************************/
NLM_EXTERN LinkStrPtr SeqEntryToStrArrayEx(SeqEntryPtr sep, Uint1 format,
                                           Int4 gi, Boolean is_html)
{
    StdPrintOptionsPtr	Spop = NULL;
    LinkStrPtr   lsp;
    LinkStrPtr   tlsp;
	Asn2ffJobPtr ajp;
	ValNodePtr v;
	BioseqPtr bsp;
	SeqIdPtr sip;
	TextSeqIdPtr tsip;

	
    if (is_html) {
    	init_www();
    }
    if(format == GENPEPT_FMT) {
        if(Template_load == FALSE) {
            PrintTemplateSetLoad("asn2ff.prt");
            Template_load = TRUE;
        }
        Spop = StdPrintOptionsNew(NULL);
        if(Spop != NULL) {
            Spop->newline = "~";
            Spop->indent = "";
        } else {
            ErrPostStr(SEV_FATAL, 0, 0, "StdPrintOptionsNew failed");
            return NULL;
        }
    }
	ajp = Asn2ffJobCreate(sep, NULL, NULL, NULL, format, RELEASE_MODE, Spop);
	/* ajp->entityID = ObjMgrGetEntityIDForPointer (ajp->slp); */ /* entityID already set - slp is NULL */
	if (SeqMgrFeaturesAreIndexed (ajp->entityID) == 0) {
		SeqMgrIndexFeatures (ajp->entityID, NULL);
	}
	ajp->useSeqMgrIndexes = TRUE;
	ajp->contig_view = FALSE;
	if (gi > 0) {
	ajp->show_version = TRUE;
		v = ValNodeNew(NULL);
		v->choice = SEQID_GI;
		v->data.intvalue = gi;
		ajp->id_print = v;
		if (v != NULL) {
			ajp->gb_style = FALSE;
			if ((bsp = BioseqFind(v)) == NULL) {
				ErrPostEx(SEV_FATAL, 0, 0, "BioseqFind failed for %ld", gi);
				return NULL;
			}
    		if (bsp->repr == Seq_repr_seg) {
				ajp->gb_style = TRUE;
			}
			for (sip=bsp->id; sip; sip=sip->next) {
				if (sip->choice == SEQID_OTHER) {
					tsip = (TextSeqIdPtr) sip->data.ptrvalue;
					if (StringNCmp(tsip->accession, "NT_", 3) == 0) {
						ajp->contig_view = TRUE;
						ajp->genome_view = TRUE;
						break;
					}
				}
			}
    		if (bsp->repr == Seq_repr_seg && ajp->contig_view == FALSE) {
				ajp->id_print = NULL;
			}
			if (bsp->repr != Seq_repr_seg) {
				sep = SeqMgrGetSeqEntryForData((Pointer)bsp);
			}
		}
	}
    lsp = (LinkStrPtr) MemNew(sizeof(LinkStr));
    lsp->next = NULL;
    lsp->line = NULL;
    SeqEntryToLinkStr(ajp, sep, lsp, format, RELEASE_MODE);
    tlsp = lsp;
    lsp = lsp->next;
    MemFree(tlsp);

	if(Spop != NULL) {
    	StdPrintOptionsFree(Spop);
	}
    return (lsp);
}

/**********************************************************/
NLM_EXTERN LinkStrPtr SeqEntryToStrArrayQEx(SeqEntryPtr sep,  Uint1 format, Int4 gi, Boolean is_html)
{
    StdPrintOptionsPtr	Spop = NULL;
    LinkStrPtr   lsp;
    LinkStrPtr   tlsp;
	Asn2ffJobPtr ajp;
	ValNodePtr v;
	BioseqPtr bsp;
	SeqIdPtr sip;
	SeqLocPtr slp;
	TextSeqIdPtr tsip;

	
    if (is_html) {
    	init_www();
    }
    if(format == GENPEPT_FMT) {
        if(Template_load == FALSE) {
            PrintTemplateSetLoad("asn2ff.prt");
            Template_load = TRUE;
        }
        Spop = StdPrintOptionsNew(NULL);
        if(Spop != NULL) {
            Spop->newline = "~";
            Spop->indent = "";
        } else {
            ErrPostStr(SEV_FATAL, 0, 0, "StdPrintOptionsNew failed");
            return NULL;
        }
    }
	ajp = Asn2ffJobCreate(sep, NULL, NULL, NULL, format, RELEASE_MODE, Spop);
	/* ajp->entityID = ObjMgrGetEntityIDForPointer (ajp->slp); */ /* entityID already set - slp is NULL */
	if (SeqMgrFeaturesAreIndexed (ajp->entityID) == 0) {
		SeqMgrIndexFeatures (ajp->entityID, NULL);
	}
	ajp->useSeqMgrIndexes = TRUE;
	ajp->contig_view = FALSE;
	/*if (is_html && gi > 0) {*/
	if (gi > 0) {
		ajp->show_version = TRUE;
		v = ValNodeNew(NULL);
		v->choice = SEQID_GI;
		v->data.intvalue = gi;
		if ((bsp = BioseqFind(v)) != NULL) {
			if (bsp->repr == Seq_repr_seg) {
				 if ((slp = bsp->seq_ext) != NULL) {
					if (BioseqFindCore(SeqLocId(slp)) == NULL) {
						ajp->genome_view = TRUE;
    					ajp->only_one = TRUE;
					}
				}
			}
		}
		ajp->id_print = v;
		if (v != NULL) {
			ajp->gb_style = FALSE;
			if ((bsp = BioseqFind(v)) == NULL) {
				ErrPostEx(SEV_FATAL, 0, 0, "BioseqFind failed for %ld", gi);
				return NULL;
			}
    		if (bsp->repr == Seq_repr_seg) {
				ajp->gb_style = TRUE;
			}
			for (sip=bsp->id; sip; sip=sip->next) {
				if (sip->choice == SEQID_OTHER) {
					tsip = (TextSeqIdPtr) sip->data.ptrvalue;
					if (StringNCmp(tsip->accession, "NT_", 3) == 0) {
						ajp->contig_view = TRUE;
						ajp->genome_view = TRUE;
						ajp->show_seq = FALSE;
						break;
					}
				}
			}
    		if (bsp->repr == Seq_repr_seg && ajp->contig_view == FALSE) {
				ajp->id_print = NULL;
			}
			if (bsp->repr != Seq_repr_seg) {
				sep = SeqMgrGetSeqEntryForData((Pointer)bsp);
			}
		}
	}
    lsp = (LinkStrPtr) MemNew(sizeof(LinkStr));
    lsp->next = NULL;
    lsp->line = NULL;
    SeqEntryToLinkStr(ajp, sep, lsp, format, RELEASE_MODE);
    tlsp = lsp;
    lsp = lsp->next;
    MemFree(tlsp);

	if(Spop != NULL) {
    	StdPrintOptionsFree(Spop);
	}
    return (lsp);
}

/**********************************************************/
NLM_EXTERN void asn2ff_print_bs(Asn2ffJobPtr ajp)
{
    FFPrintArrayPtr pap = NULL;
    Int4            index;
    Int4            pap_size;
    Boolean         hold = TRUE;


    if(ajp->no_hold != FALSE)
        hold = FALSE;
    if(hold != FALSE)
        ObjMgrSetHold();        /* hold any autoloaded records in memory */

    pap_size = asn2ff_setup(ajp, &pap);
    if(pap_size > 0)
    {
        for(index = 0; index < pap_size; index++)
        {
            asn2ff_set_output(NULL, "\n");
            FFBSPrint(pap, index, pap_size);
            if(pap[index].descr)
            {
                pap[index].descr = (DescrStructPtr)MemFree(pap[index].descr);
            }
        }
        MemFree(pap);
    }

    free_buff();
    asn2ff_cleanup(ajp); 
    if(hold != FALSE)
        ObjMgrClearHold();
    if(ajp->free_cache)
        ObjMgrFreeCache(0);
}

/**********************************************************/
NLM_EXTERN ByteStorePtr AjpToByteStore(Asn2ffJobPtr ajp)
{
    StdPrintOptionsPtr Spop = NULL;
    ByteStorePtr       bs;
    BioseqSetPtr       bssp;
    SeqEntryPtr        tsep;

    if(ajp == NULL)
        return(NULL);

    if(ajp->ssp != NULL)
    {
        ajp->sep = (SeqEntryPtr)ajp->ssp->data;
        ajp->ssp = NULL;
    }

    if(ajp->sep == NULL && ajp->slp == NULL)
    {
        bs = ajp->byte_st;
        ajp->byte_st = NULL;
        return(bs);
    }

    if(ajp->format == GENPEPT_FMT)
    {
        if(Template_load == FALSE)
        {
            PrintTemplateSetLoad("asn2ff.prt");
            Template_load = TRUE;
        }
        if(ajp->Spop == NULL)
        {
            Spop = StdPrintOptionsNew(NULL);
            if(Spop != NULL)
            {
                Spop->newline = "~";
                Spop->indent = "";
                ajp->Spop = Spop;
            }
            else
            {
                ErrPostStr(SEV_FATAL, 0, 0, "StdPrintOptionsNew failed");
                bs = ajp->byte_st;
                ajp->byte_st = NULL;
                return(bs);
            }
        }
    }

    if(ajp->sep != NULL && ajp->sep->choice == 2)
    {
        bssp = (BioseqSetPtr) ajp->sep->data.ptrvalue;
        if(bssp != NULL && (bssp->_class == 7 ||
                            (bssp->_class >= 13 && bssp->_class <= 16)))
        {
            for(tsep = bssp->seq_set; tsep != NULL; tsep = tsep->next)
            {
                ajp->sep = tsep;
                bs = AjpToByteStore(ajp);
                ajp->byte_st = bs;
            }
        }
        else
            asn2ff_print_bs(ajp);
    }
    else
        asn2ff_print_bs(ajp);

    bs = ajp->byte_st;
    ajp->byte_st = NULL;

    return(bs);
}

/***********************************************************************
*
*	SeqEntryToFlat is a stand-alone function that takes a SeqEntryPtr
*	and writes a flat file to a disk file.  If the formatting is
*	successful, TRUE is returned; otherwise FALSE is returned.
*
Choices for the Uint1's format and mode are defined in asn2ff.h.

For format they are:

GENBANK_FMT 	standard GenBank flat file for nucleotides
EMBL_FMT	standard EMBL flat file  for nucleotides
GENPEPT_FMT 	standard GenBank flat file for proteins
PSEUDOEMBL_FMT  a flavor of the EMBL flat file used by the "Authorin" program

The modes are:

RELEASE_MODE	this mode assures that all the requirements (e.g., identifiers
		features, references as described in the GenBank release notes
		and the feature table) are met.
		are met 
DUMP_MODE 	dump out the ASN.1 to a flat file
SEQUIN_MODE 	mode used by sequin
CHROMO_MODE 	mode used by Chromoscope
DIRSUB_MODE 	mode used by NCBI indexers during the "dirsub" process.
DIRSUB_DEBUG_MODE 	mode used by NCBI indexers during the "dirsub" process.
REVISE_MODE 	mode used by the "revise" program at NCBI (for in-house
		editing of entries).
*
**************************************************************************/

NLM_EXTERN Boolean SeqEntryToFlat (SeqEntryPtr sep, FILE *fp, Uint1 format, Uint1 mode)

{
	Boolean				rsult = FALSE;
	Asn2ffJobPtr		ajp;
	StdPrintOptionsPtr	Spop = NULL;
	BioseqSetPtr		bssp;

	if (sep == NULL) {
		return FALSE;
	}
	if (format == GENPEPT_FMT) {
		if (AllObjLoad () && SubmitAsnLoad () && SeqCodeSetLoad ()) {
			ErrShow();
		}
		if (!Template_load) {
			PrintTemplateSetLoad ("asn2ff.prt");
			Template_load = TRUE;
		}
		Spop = StdPrintOptionsNew(NULL);
		if (Spop) {
			Spop->newline = "~";
			Spop->indent = "";
		} else {
			ErrPostStr(SEV_FATAL,0,0, "StdPrintOptionsNew failed");;
			return FALSE;
		}
	}
	if (IS_Bioseq_set (sep)) {
    	bssp = (BioseqSetPtr) sep->data.ptrvalue;
    	if (bssp != NULL && (bssp->_class == 7 ||
    	                     (bssp->_class >= 13 && bssp->_class <= 16))) {
      		for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        		rsult = SeqEntryToFlat (sep, fp, format, mode);
			}
			return rsult;
		}
	}

	ajp = Asn2ffJobCreate(sep, NULL, NULL, fp, format, mode, Spop);

	if (ajp == NULL) {
		return FALSE;
	}
	ajp->show_version = TRUE;
	rsult = asn2ff_print(ajp);
	StdPrintOptionsFree(ajp->Spop);
	MemFree(ajp);
	
	return rsult;
}

NLM_EXTERN Boolean SeqEntryToFlatAjp (Asn2ffJobPtr ajp, SeqEntryPtr sep, FILE *fp, Uint1 format, Uint1 mode)

{
	Boolean				rsult = FALSE;
	StdPrintOptionsPtr	Spop = NULL;
	BioseqSetPtr		bssp;
	
	if (sep == NULL) {
		return FALSE;
	}
	if (format == GENPEPT_FMT) {
		if (AllObjLoad () && SubmitAsnLoad () && SeqCodeSetLoad ()) {
			ErrShow();
		}
		if (!Template_load) {
			PrintTemplateSetLoad ("asn2ff.prt");
			Template_load = TRUE;
		}
		Spop = StdPrintOptionsNew(NULL);
		if (Spop) {
			Spop->newline = "~";
			Spop->indent = "";
		} else {
			ErrPostStr(SEV_FATAL,0,0, "StdPrintOptionsNew failed");;
			return FALSE;
		}
	}
	if (IS_Bioseq_set (sep)) {
    	bssp = (BioseqSetPtr) sep->data.ptrvalue;
    	if (bssp != NULL && (bssp->_class == 7 ||
    	                     (bssp->_class >= 13 && bssp->_class <= 16))) {
      		for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        		rsult = SeqEntryToFlatAjp (ajp, sep, fp, format, mode);
			}
			if (format == GENPEPT_FMT && Spop) {
				Spop = StdPrintOptionsFree(Spop);
			}
			return rsult;
		}
	}
	if (sep == NULL && ajp != NULL) {
		StdPrintOptionsFree(ajp->Spop);
		return rsult;
	}
	if (ajp == NULL) {
		if ((ajp = Asn2ffJobCreate(sep, NULL, NULL, fp, format, mode, Spop))
																	== NULL) {
			if (format == GENPEPT_FMT && Spop) {
				Spop = StdPrintOptionsFree(Spop);
			}
			return FALSE;
		}
	} else {
		if ((ajp->entityID=ObjMgrGetEntityIDForPointer(sep)) == 0) {
			ErrPostStr(SEV_WARNING, 0, 0, "Couldn't get entityID");
			if (format == GENPEPT_FMT && Spop) {
				Spop = StdPrintOptionsFree(Spop);
			}
			return rsult;
		}
		ajp->sep = sep;
	}
	rsult = asn2ff_print(ajp);
	if (format == GENPEPT_FMT && Spop) {
		Spop = StdPrintOptionsFree(Spop);
	}
	
	return rsult;
}

/**************************************************************************
 *	Prints out flat file in GenBank format WITHOUT Sequence
 **************************************************************************/
NLM_EXTERN Boolean SeqEntryToGBFlatNoSeq(SeqEntryPtr sep, FILE *fp,
                                         Uint1 mode, Boolean show_gi)
{
    Boolean	  rsult;
    Asn2ffJobPtr  ajp;
    BioseqSetPtr  bssp;
    
    if (sep == NULL) {
        return FALSE;
    }

    if (IS_Bioseq_set (sep)) {
    	bssp = (BioseqSetPtr) sep->data.ptrvalue;
    	if (bssp != NULL && (bssp->_class == 7 ||
    	                     (bssp->_class >= 13 && bssp->_class <= 16))) {
            for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
                rsult = SeqEntryToGBFlatNoSeq(sep, fp, mode, show_gi);
            }
            return rsult;
        }
    }
    
    ajp = Asn2ffJobCreate(sep, NULL, NULL, fp, GENBANK_FMT, mode, NULL);
    
    if (ajp == NULL)
        return FALSE;
    
    ajp->show_seq = FALSE; /* This is the point */
    ajp->show_gi = show_gi;
    
    rsult = asn2ff_print(ajp);
    MemFree(ajp);
	
    return rsult;
}

/***********************************************************************
*
*	SeqEntryToFlatEx is a stand-alone function works as SeqEntryToFlat
*	takes SeqIdPtr and various types of the output
*
*	successful, TRUE is returned; otherwise FALSE is returned.
*
*	Choices for the Uint1's type are defined in asn2ff.h.
*	FF_REGULAR 			0
*	FF_TOP_COMPLETE		1
*	FF_TOP_CONTIG		2
*
**************************************************************************/

NLM_EXTERN Boolean SeqEntryToFlatEx (SeqEntryPtr sep, FILE *fp, Uint1 format, Uint1 mode, SeqIdPtr seqid, Uint1 type)
{
	Boolean				rsult=FALSE;
	Asn2ffJobPtr		ajp;
	StdPrintOptionsPtr	Spop = NULL;
	BioseqPtr 			bsp;
	BioseqSetPtr		bssp;

	rsult = FALSE;
	if (format == GENPEPT_FMT) {
		if (AllObjLoad () && SubmitAsnLoad () && SeqCodeSetLoad ()) {
			ErrShow();
		}
		if (!Template_load) {
			PrintTemplateSetLoad ("asn2ff.prt");
			Template_load = TRUE;
		}
		Spop = StdPrintOptionsNew(NULL);
		if (Spop) {
			Spop->newline = "~";
			Spop->indent = "";
		} else {
			Message (MSG_FATAL, "StdPrintOptionsNew failed");
			return rsult;
		}
	}
	ajp = Asn2ffJobCreate(sep, NULL, NULL, fp, format, mode, Spop);
	if (ajp == NULL) {
		return FALSE;
	}
	ajp->show_version = TRUE;
	if (mode == RELEASE_MODE) {
		ajp->show_gi = FALSE;
	}
	if (type == FF_REGULAR) {
		ajp->gb_style = TRUE;
		ajp->id_print = NULL;
	}
	if (type == FF_TOP_COMPLETE) {
		ajp->gb_style = FALSE;
		ajp->only_one = TRUE;
		ajp->ignore_top = TRUE;
	}
	if (type == FF_TOP_CONTIG) {
		ajp->gb_style = FALSE;
		ajp->only_one = TRUE;
		ajp->ignore_top = TRUE;
		ajp->genome_view = TRUE;
		ajp->show_seq = FALSE;
	}
	if (seqid != NULL) {
		ajp->gb_style = FALSE;
		ajp->id_print = seqid;
		bsp = BioseqFind(seqid);
    	if (bsp->repr == Seq_repr_seg) {
			ajp->sep = sep;
		} else {
			ajp->sep = SeqMgrGetSeqEntryForData((Pointer)bsp);
		}
		if ((ajp->entityID = ObjMgrGetEntityIDForChoice(ajp->sep)) == 0) {
			ErrPostStr(SEV_WARNING, 0, 0, "Couldn't get entityID");
		}
		rsult = asn2ff_print(ajp);

		StdPrintOptionsFree(ajp->Spop);
		MemFree(ajp);
	
		return rsult;
	} 
	if (IS_Bioseq_set (sep)) {
	    bssp = (BioseqSetPtr) sep->data.ptrvalue;
	    if (bssp != NULL && (bssp->_class == 7 ||
	                         (bssp->_class >= 13 && bssp->_class <= 16))) {
		      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
			rsult = SeqEntryToFlatEx (sep, fp, format, mode, seqid, type);
			}
			return rsult;
		}
	}
	ajp->sep = sep;
	if ((ajp->entityID = ObjMgrGetEntityIDForChoice(ajp->sep)) == 0) {
		ErrPostStr(SEV_WARNING, 0, 0, "Couldn't get entityID");
	}
	rsult = asn2ff_print(ajp);

	StdPrintOptionsFree(ajp->Spop);
	MemFree(ajp);
	
	return rsult;
}

/**************************************************************************
*	Prints out short flat file report in GenBank format
**************************************************************************/
NLM_EXTERN Boolean SeqEntryToPartRpt (SeqEntryPtr sep, FILE *fp)
{
	Boolean			rsult;
	Asn2ffJobPtr	ajp;

	ajp = Asn2ffJobCreate(sep, NULL, NULL, fp, GENBANK_FMT, PARTIAL_MODE, NULL);
	if (ajp == NULL) {
		return FALSE;
	}
	rsult = asn2ff_print(ajp);
	MemFree(ajp);
	
	return rsult;
}

NLM_EXTERN Boolean SeqSubmitToFlat (SeqSubmitPtr ssp, FILE *fp, Uint1 mode, Boolean show_gi, Uint1 format, Boolean show_gene)
{
	Boolean			rsult = FALSE;
	Asn2ffJobPtr	ajp;

	if (ssp == NULL) {
		return rsult;
	}
	if (ssp->datatype != 1) {
		return rsult;
	}
	ajp = Asn2ffJobCreate(NULL, ssp, NULL, fp, format, mode, NULL);
	if (ajp == NULL) {
		return FALSE;
	}
	rsult = asn2ff_print(ajp);
	MemFree(ajp);
	return rsult;
}

NLM_EXTERN Boolean SeqGenomeToFlat (SeqEntryPtr sep, FILE *fp, Uint1 format, Uint1 mode)

{
	Boolean          rsult;
	Asn2ffJobPtr		ajp;
	StdPrintOptionsPtr Spop = NULL;

	rsult = FALSE;
	ajp = Asn2ffJobCreate(sep, NULL, NULL, fp, format, mode, Spop);
	if (ajp == NULL) {
		return FALSE;
	}
    ajp->only_one = TRUE;
	ajp->ignore_top = TRUE;
	ajp->genome_view = TRUE;
	
	rsult = asn2ff_print(ajp);
	MemFree(ajp);
	
	return rsult;
}

NLM_EXTERN Boolean SeqGenomeToFlatEx (SeqEntryPtr sep, FILE *fp, Uint1 format, Uint1 mode, Boolean map_view)

{
	Boolean				rsult;
	Asn2ffJobPtr		ajp;
	StdPrintOptionsPtr Spop = NULL;

	rsult = FALSE;
	ajp = Asn2ffJobCreate(sep, NULL, NULL, fp, format, mode, Spop);
	if (ajp == NULL) {
		return FALSE;
	}
    ajp->only_one = TRUE;
	ajp->ignore_top = TRUE;
	ajp->genome_view = TRUE;
	ajp->map_view = map_view;
	ajp->forgbrel = FALSE;

	rsult = asn2ff_print(ajp);
	MemFree(ajp);
	return rsult;
}

NLM_EXTERN Boolean SeqLocToFlat (SeqLocPtr slp, FILE *fp, Uint1 format, Uint1 mode)
{
	Boolean          rsult;
	Asn2ffJobPtr		ajp;
	StdPrintOptionsPtr Spop = NULL;
	Uint2            itemID=0;

	rsult = FALSE;
	if (format == GENPEPT_FMT) {
		if (AllObjLoad () && SubmitAsnLoad () && SeqCodeSetLoad () &&
								PrintTemplateSetLoad ("asn2ff.prt")) {
			ErrShow();
		}
		Spop = StdPrintOptionsNew(NULL);
		if (Spop) {
			Spop->newline = "~";
			Spop->indent = "";
		} else {
			Message (MSG_FATAL, "StdPrintOptionsNew failed");
			return rsult;
		}
	}

	ajp = Asn2ffJobCreate(NULL, NULL, slp, fp, format, mode, Spop);
	if (ajp == NULL) {
		return FALSE;
	}
	ajp->show_version = TRUE; 
	ajp->only_one = TRUE; 
	ajp->ignore_top = FALSE;
	ajp->id_print = SeqLocId(slp);
	
	rsult = asn2ff_print(ajp);
	return rsult;
}

static void ChangeObsoleteImpFeats (SeqFeatPtr sfp, Pointer userdata)

{
  ImpFeatPtr  ifp;

  if (sfp == NULL || sfp->data.choice != SEQFEAT_IMP) return;
  ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
  if (ifp != NULL &&
      (StringCmp (ifp->key, "allele") == 0 ||
       StringCmp (ifp->key, "mutation") == 0)) {
    ifp->key = MemFree (ifp->key);
    ifp->key = StringSave ("variation");
    sfp->idx.subtype = FEATDEF_variation;
  }
}


/***************************************************************************
*
*	Setup the FFPrintArrayPtr to be used by "FFPrint", the number 
*	returned is the number of entries in the array.
***************************************************************************/

NLM_EXTERN Int4 asn2ff_setup (Asn2ffJobPtr ajp, FFPrintArrayPtr PNTR papp)

{
	Int4 pap_size = -1;
	Asn2ffWEPtr awp;
	GatherScope gs;
	Uint1 focus;
	BioseqPtr bsp;
	SeqEntryPtr sep = NULL;

Message (MSG_POSTERR, "The asn2ff flatfile generator is obsolete and unsupported.  Please switch to using asn2gb/SeqEntryToGnbk in the future.");

	if (ajp->sep != NULL) {
	  sep = ajp->sep;
	} else if (ajp->ssp != NULL && ajp->ssp->datatype == 1) {
	  sep = (SeqEntryPtr) ajp->ssp->data;
	} else if (ajp->entityID > 0 && ajp->slp == NULL) {
	  sep = GetTopSeqEntryForEntityID (ajp->entityID);
	}

	if (sep != NULL) {
		VisitFeaturesInSep (sep, NULL, ChangeObsoleteImpFeats);
	} 

	ajp->show_gi = FALSE; /* displayed two obsolete line types - should always be FALSE */

  	MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
	MemSet ((Pointer) (gs.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
	gs.ignore[OBJ_BIOSEQ] = FALSE;
	gs.ignore[OBJ_BIOSEQSET] = FALSE;

	if (ajp->format == EMBLPEPT_FMT) /* Turn off Validators for EMBLPEPT */
		ajp->mode = DUMP_MODE;

	if (ajp->format == EMBLPEPT_FMT || ajp->format == GENPEPT_FMT)
		ajp->gb_style = FALSE;
	set_flags(ajp);


	flat2asn_install_accession_user_string("SET-UP");
	flat2asn_install_locus_user_string("SET-UP");

	ajp->sfp_out = 	MakeSyntheticSeqFeat();
	awp = (Asn2ffWEPtr) MemNew(sizeof(Asn2ffWE));
	awp->seg = NULL;
	awp->parts = NULL;
	ajp->hup = FALSE;
	if (ajp->ssp && ajp->ssp->sub) {
		ajp->hup = ajp->ssp->sub->hup;
	}
	ajp->asn2ffwep = awp;
	if (ajp->entityID == 0) {
	  if (ajp->sep != NULL) {
	    ajp->entityID = ObjMgrGetEntityIDForChoice (ajp->sep);
	  } else if (ajp->ssp != NULL) {
	    ajp->entityID = ObjMgrGetEntityIDForPointer (ajp->ssp);
	  } else if (ajp->slp != NULL) {
		bsp = BioseqFindFromSeqLoc(ajp->slp);
		ajp->entityID = ObjMgrGetEntityIDForPointer (ajp->slp);
      	SeqMgrIndexFeatures (ajp->entityID, NULL);
      	ajp->useSeqMgrIndexes = TRUE;
	  }
	}

	if (ajp->entityID != 0) {
		if (ajp->slp != NULL) {
			SeqMgrIndexFeatures (ajp->entityID, NULL);
			ajp->useSeqMgrIndexes = TRUE;
		}
		focus = (Uint1)FocusSeqEntry(ajp->sep, &gs);
		MemSet ((Pointer) (gs.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
		gs.ignore[OBJ_BIOSEQ] = FALSE;
		gs.ignore[OBJ_BIOSEQSET] = FALSE;
			GatherEntity(ajp->entityID, (Pointer) ajp, SeqToAwp, &gs);
		if (focus == FOCUS_INITIALIZED) {
			SeqLocFree(gs.target);
		}
		awp = ajp->asn2ffwep;
		if (!ajp->only_one && awp->gbp == NULL) {
			if (awp) {
				ajp->asn2ffwep = (Asn2ffWEPtr)MemFree(awp);
			}
			return 0;
		}
		if (ajp->forgbrel && awp->seg == NULL && ajp->genome_view == TRUE) {
			return 0;
		}
		if (awp->seg == NULL && awp->parts == NULL) {
			awp->total_seg = 0;
			if (awp->gbp) {
				awp->gbp->num_seg = 0;
			}
			if (SeqMgrFeaturesAreIndexed (ajp->entityID)) {
				ajp->useSeqMgrIndexes = TRUE;  /* initial use of new indexes */
			}
		}
		if (awp->gbp && awp->gbp->next == NULL) {
			awp->total_seg = 0;
			awp->gbp->num_seg = 0;
			if (SeqMgrFeaturesAreIndexed (ajp->entityID)) {
				ajp->useSeqMgrIndexes = TRUE;  /* initial use of new indexes */
			}
		}
/*
		if (ajp->slp != NULL) {
			if (awp->gbp) {
				if (ajp->slp->choice == SEQLOC_MIX || ajp->slp->choice == SEQLOC_PACKED_INT) {
					awp->gbp->next = NULL;
					awp->gbp->num_seg = 0;
					awp->total_seg = 0;
				}
			}
		}
*/
		ajp->asn2ffwep = awp;
		if (ajp->mode != DIRSUB_MODE)
			GetGIs(ajp);
	}

	init_buff();
	ajp->pseudo = FALSE;
	if (ajp->format == SELECT_FMT) {    /* quick fix 07.17.95 change later */
		ajp->format = GENBANK_FMT;
		ajp->pseudo = TRUE;
	}
	if (ajp->format == PSEUDOEMBL_FMT) {
		ajp->pseudo = TRUE;
	}
	if (ajp->help) {
		pap_size = asn2hp_setup(ajp, papp);
		return pap_size;
	}
	if (ajp->format == GENBANK_FMT || ajp->format == SELECT_FMT) {
		if (ajp->mode == PARTIAL_MODE) {
			pap_size = asn2pr_setup(ajp, papp);
		} else {
			pap_size = asn2gb_setup(ajp, papp);
		}
	} else if (ajp->format == EMBL_FMT || ajp->format == PSEUDOEMBL_FMT) {
		pap_size = asn2embl_setup(ajp, papp);
	} else if (ajp->format == EMBLPEPT_FMT) {
			pap_size = asn2ep_setup(ajp,  papp);
	}else if (ajp->format == GENPEPT_FMT) {
		pap_size = asn2gp_setup(ajp, papp);
	}else if (ajp->format == GRAPHIK_FMT) {
		pap_size = asn2gr_setup(ajp, papp);
	}
	return pap_size;
}	/* asn2ff_setup */

/****************************************************************************
*void set_flags (Asn2ffJobPtr ajp)
*
*	set_flags to determine which tasks to perform.
*****************************************************************************/
void set_flags (Asn2ffJobPtr ajp)

{

/* The defines are:
ASN2FF_LOCAL_ID                 asn2ff_flags[0]
	If FALSE then entries with "local" id's are NOT formatted 
ASN2FF_LOOK_FOR_SEQ             asn2ff_flags[1]
	If TRUE BioseqFind is run in an attempt to "find" entries that
	have been loaded into memory and are referenced by an entry 
ASN2FF_VALIDATE_FEATURES        asn2ff_flags[2]
	If TRUE then validation is run on features.  If they are invalid
	they are dropped.
ASN2FF_IGNORE_PATENT_PUBS            asn2ff_flags[3]
	This flag only applies to patent pubs.  If FlatIgnoreThisPatentPub
	is true and this flag is TRUE, that pub is dropped.  ALL OTHER
	PUBS are validated all the time.
ASN2FF_DROP_SHORT_AA            asn2ff_flags[4]
	Drop amino acid sequences that are too short.  Only applies to 
	GenPept (i.e., protein) format  
ASN2FF_AVOID_LOCUS_COLL         asn2ff_flags[5]
	If TRUE Check for LOCUS collisions with Karl's algorithm
	Otherwise Use the LOCUS in the id field.
ASN2FF_DATE_ERROR_MSG		asn2ff_flags[6]
	If TRUE report a missing date.  SHould be FALSE for indexing
	work when no date for a record has been set.
ASN2FF_IUPACAA_ONLY		asn2ff_flags[7]
	Use only iupaca characters if TRUE.  Only iupacaa is the flat
	file standard.
ASN2FF_TRANSL_TABLE		asn2ff_flags[8]
	If TRUE print the transl_table qualifiers.  Set to FALSE until
	the database correctly reflects transl_tables.
ASN2FF_REPORT_LOCUS_COLL     	asn2ff_flags[9]
	If TRUE, report locus collisions via ErrPostEx
ASN2FF_SHOW_ALL_PUBS	        asn2ff_flags[10]
	if TRUE don't drop CitGen reference or replace CitGen->cit with
	"Unpublished"
ASN2FF_SHOW_ERROR_MSG	        asn2ff_flags[11]
ASN2FF_SHOW_GB_STYLE	        asn2ff_flags[12]
	show only features comleted on this bioseq or location - gb_style
	
*/

	asn2ff_flags[11] = ajp->error_msgs;
	asn2ff_flags[12] = ajp->gb_style;
	if (ajp->mode == RELEASE_MODE)
	{
		asn2ff_flags[0] = FALSE; 
		if (GetAppProperty ("InternalNcbiSequin") != NULL) {
			asn2ff_flags[0] = TRUE; 
		}
		asn2ff_flags[1] = FALSE; 
		asn2ff_flags[2] = TRUE; 
		asn2ff_flags[3] = TRUE; 
		asn2ff_flags[4] = TRUE; 
		asn2ff_flags[5] = TRUE; 
		asn2ff_flags[6] = TRUE; 
		asn2ff_flags[7] = TRUE; 
		asn2ff_flags[8] = TRUE; 
		asn2ff_flags[9] = FALSE; 
		asn2ff_flags[10] = FALSE; 
	}
	else if (ajp->mode == DIRSUB_MODE)
	{
		asn2ff_flags[0] = FALSE; 
		asn2ff_flags[1] = FALSE; 
		asn2ff_flags[2] = TRUE; 
		asn2ff_flags[3] = TRUE; 
		asn2ff_flags[4] = TRUE; 
		asn2ff_flags[5] = TRUE; 
		asn2ff_flags[6] = FALSE; 
		asn2ff_flags[7] = FALSE; 
		asn2ff_flags[8] = TRUE; 
		asn2ff_flags[9] = FALSE; 
		asn2ff_flags[10] = FALSE;
		ajp->show_gi = FALSE; 
	}
	else if (ajp->mode == DIRSUB_DEBUG_MODE)
	{
		asn2ff_flags[0] = FALSE; 
		asn2ff_flags[1] = FALSE; 
		asn2ff_flags[2] = FALSE; 
		asn2ff_flags[3] = TRUE; 
		asn2ff_flags[4] = TRUE; 
		asn2ff_flags[5] = TRUE; 
		asn2ff_flags[6] = FALSE; 
		asn2ff_flags[7] = FALSE; 
		asn2ff_flags[8] = TRUE; 
		asn2ff_flags[9] = FALSE; 
		asn2ff_flags[10] = FALSE;
		ajp->show_gi = FALSE; 
	}
	else if (ajp->mode == REVISE_MODE)
	{
		asn2ff_flags[0] = TRUE; 
		asn2ff_flags[1] = FALSE; 
		asn2ff_flags[2] = FALSE; 
		asn2ff_flags[3] = FALSE; 
		asn2ff_flags[4] = FALSE; 
		asn2ff_flags[5] = FALSE; 
		asn2ff_flags[6] = TRUE; 
		asn2ff_flags[7] = FALSE; 
		asn2ff_flags[8] = TRUE; 
		asn2ff_flags[9] = TRUE; 
		asn2ff_flags[10] = FALSE; 
	}
	else if (ajp->mode == DUMP_MODE)
	{
		asn2ff_flags[0] = TRUE; 
		asn2ff_flags[1] = FALSE; 
		asn2ff_flags[2] = FALSE; 
		asn2ff_flags[3] = FALSE; 
		asn2ff_flags[4] = FALSE; 
		asn2ff_flags[5] = FALSE; 
		asn2ff_flags[6] = TRUE; 
		asn2ff_flags[7] = FALSE; 
		asn2ff_flags[8] = TRUE; 
		asn2ff_flags[9] = FALSE; 
		asn2ff_flags[10] = TRUE; 
	}
	else if (ajp->mode == SEQUIN_MODE)
	{
		asn2ff_flags[0] = TRUE; 
		asn2ff_flags[1] = FALSE; 
		asn2ff_flags[2] = FALSE; 
		asn2ff_flags[3] = TRUE; 
		asn2ff_flags[4] = TRUE; 
		asn2ff_flags[5] = TRUE; 
		asn2ff_flags[6] = FALSE; 
		asn2ff_flags[7] = FALSE; 
		asn2ff_flags[8] = TRUE; 
		asn2ff_flags[9] = FALSE; 
		asn2ff_flags[10] = FALSE; 
	}
	else if (ajp->mode == CHROMO_MODE)
	{
		asn2ff_flags[0] = TRUE; 
		asn2ff_flags[1] = TRUE; 
		asn2ff_flags[2] = FALSE; 
		asn2ff_flags[3] = TRUE; 
		asn2ff_flags[4] = FALSE; 
		asn2ff_flags[5] = FALSE; 
		asn2ff_flags[6] = FALSE; 
		asn2ff_flags[7] = FALSE; 
		asn2ff_flags[8] = TRUE; 
		asn2ff_flags[9] = FALSE; 
		asn2ff_flags[10] = FALSE; 
	}
}

static Boolean check_whole(SeqFeatPtr f, Int4 len)
{
	Boolean whole = FALSE;
	SeqLocPtr		slp;
	SeqIntPtr		sip;
	
		slp = f->location;
		if (slp->choice == SEQLOC_WHOLE) {
			whole = TRUE;
		} else if (slp->choice == SEQLOC_INT) {
			sip = (SeqIntPtr)slp->data.ptrvalue;
			if (sip->from == 0 && sip->to == len-1) {
				whole = TRUE;
			}
		}
	return whole;
}
Boolean get_pubs (GatherContextPtr gcp)
{
	ValNodePtr	tmp, vnp, v;
	PubdescPtr 	pdp;
	ValNodePtr	PNTR vnpp;
	BioseqPtr	bsp;
	SeqLocPtr	slp;
	SeqFeatPtr	sfp;
	ImpFeatPtr	ifp;
	SubmitBlockPtr sbp;
	CitSubPtr 	the_cit;
	
	vnpp = (ValNodePtr *)gcp->userdata;
	vnp = *vnpp;
	switch (gcp->thistype)
	{
		case OBJ_SEQDESC:
			tmp = (ValNodePtr) (gcp->thisitem);
			if (gcp->parenttype == OBJ_BIOSEQ) {
				bsp = (BioseqPtr) (gcp->parentitem);
			} else {
				bsp = NULL;
			}
			if (tmp->choice == Seq_descr_pub) {
				vnp = StorePub(bsp, vnp, tmp, NULL, 1, gcp->entityID,
					gcp->itemID, gcp->thistype);
			}
			break;
		case OBJ_SEQFEAT:
			sfp = (SeqFeatPtr) (gcp->thisitem);
			if (sfp->data.choice == SEQFEAT_PUB) {
				slp = sfp->location;
				bsp = BioseqFindCore(SeqLocId(slp));
				if (bsp && check_whole(sfp, bsp->length)) {
					tmp = ValNodeNew(NULL);
					tmp->choice = Seq_descr_pub;
					tmp->data.ptrvalue = (PubdescPtr) sfp->data.value.ptrvalue;
					vnp = StorePub(bsp, vnp, tmp, NULL, 1, gcp->entityID,
					gcp->itemID, gcp->thistype);
					ValNodeFree(tmp);
				} else {
					vnp = StorePub(NULL, vnp, NULL, sfp, 2, gcp->entityID,
					gcp->itemID, gcp->thistype);
				}
			} 
			if (sfp->data.choice == SEQFEAT_IMP) {
				ifp = (ImpFeatPtr)sfp->data.value.ptrvalue;
				if (StringCmp(ifp->key, "Site-ref") == 0) {
					if (sfp->cit != NULL) {
						vnp = StorePub(NULL, vnp, NULL, sfp, 3, gcp->entityID,
						gcp->itemID, gcp->thistype);
					}
				}
			}
			break;
		case OBJ_SUBMIT_BLOCK:
			sbp = (SubmitBlockPtr) (gcp->thisitem);
			the_cit = (CitSubPtr)AsnIoMemCopy(sbp->cit, (AsnReadFunc) CitSubAsnRead,
			(AsnWriteFunc) CitSubAsnWrite);
			v = ValNodeNew(NULL);
			v->choice = PUB_Sub;
			v->data.ptrvalue = the_cit;
			pdp = PubdescNew();
			pdp->pub = v;
			tmp = ValNodeNew(NULL);
			tmp->choice = Seq_descr_pub;
			tmp->data.ptrvalue = pdp;
			vnp = StorePub(NULL, vnp, tmp, NULL, 1, gcp->entityID,
						gcp->itemID, gcp->thistype);
			PubdescFree(pdp);
			break;
	/*	case OBJ_SEQSUB_CIT:
			csp = (CitSubPtr) (gcp->thisitem);
			the_cit = AsnIoMemCopy(csp, (AsnReadFunc) CitSubAsnRead,
			(AsnWriteFunc) CitSubAsnWrite);
			v = ValNodeNew(NULL);
			v->choice = PUB_Sub;
			v->data.ptrvalue = the_cit;
			pdp = PubdescNew();
			pdp->pub = v;
			tmp = ValNodeNew(NULL);
			tmp->choice = Seq_descr_pub;
			tmp->data.ptrvalue = pdp;
			vnp = StorePub(NULL, vnp, tmp, NULL, 1, gcp->entityID,
						gcp->itemID, gcp->thistype);
			MemFree(csp);
			break;
	*/
		case OBJ_SEQFEAT_CIT:
/***** not used now ********/
			tmp = (ValNodePtr) (gcp->thisitem); /* PubSet->data.ptrvalue */
			break;
		default:
			break;
	}
	*vnpp = vnp;
	return TRUE;
}

static Int2 GetPubNum(GBEntryPtr gbp)
{
	ValNodePtr v;
	Int4 i;

	for (v = gbp->Pub, i=0; v != NULL; v= v->next, i++);
	
	return (Int2)i;
}
static void CheckSourceFeat(Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	OrgRefPtr orp;
	BioSourcePtr biosp;
	ValNodePtr vnp;
	DescrStructPtr ds;
	
	if (gbp && gbp->feat) {
		if (gbp->feat->sfpSourcesize != 0) 
			return;
	}
	ds = gbp->source_info;
	if ((vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_source)) != NULL) {
		biosp = (BioSourcePtr)vnp->data.ptrvalue;
		orp = (OrgRefPtr) biosp->org;	
		if (orp) {
			if (ds == NULL) {
				ds = (DescrStructPtr) MemNew(sizeof(DescrStruct));
				gbp->source_info = ds;
			}
			ds->vnp = vnp;
			ds->entityID = gbp->descr->entityID;
			ds->itemID = gbp->descr->itemID;
			ds->itemtype = gbp->descr->itemtype;
			gbp->descr = (DescrStructPtr)MemFree(gbp->descr);
			return;
		}
	}
	if (gbp && gbp->descr) {
			MemFree(gbp->descr);
	}
	if ((vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_org)) != NULL) {
		orp = (OrgRefPtr) vnp->data.ptrvalue;
		if (orp) {
			if (ds == NULL) {
				ds = (DescrStructPtr) MemNew(sizeof(DescrStruct));
				gbp->source_info = ds;
			}
			ds->vnp = vnp;
			ds->entityID = gbp->descr->entityID;
			ds->itemID = gbp->descr->itemID;
			ds->itemtype = gbp->descr->itemtype;
			gbp->descr = (DescrStructPtr)MemFree(gbp->descr);
			return;
		}
	}
	if (gbp && gbp->descr) {
			gbp->descr = (DescrStructPtr)MemFree(gbp->descr);
	}
	return;
}

Int4 asn2hp_setup(Asn2ffJobPtr ajp, FFPrintArrayPtr PNTR papp)
{
	FFPrintArrayPtr pap;
	Int4 index, total, pub_num;
	GBEntryPtr gbp;

	GetLocusPartsAwp(ajp); 
	total=2;
	for (gbp=ajp->asn2ffwep->gbp; gbp; gbp = gbp->next) {
		gbp->descr = NULL;	
		if (GB_GetSeqDescrComms(ajp, gbp) > 0) {
			total += gbp->comm_num;
		}
		pub_num = (Int2)GetPubsAwp(ajp, gbp);
		total += pub_num; 
		GetGBDate(ajp, gbp);
	}
	*papp = (FFPrintArrayPtr) MemNew((size_t) total*sizeof(FFPrintArray));
	pap = *papp;
	/* pap_total = total; -- NO EFFECT */
	LoadPap(NULL, NULL, ajp, 0, (Uint1)0, (Uint1)0, 0, A2F_OTHER, NULL);
	for (gbp=ajp->asn2ffwep->gbp; gbp; gbp = gbp->next)
	{
		GetDefinitionLine(ajp, gbp);
		LoadPap(pap, PrintDefinitionLine, ajp, 0, (Uint1)0, (Uint1)0, 
			line_estimate[1], A2F_OTHER, gbp);
		LoadPap(pap, PrintGBOrganismLine, ajp, 0, (Uint1)0, (Uint1)0, 
			line_estimate[4], A2F_OTHER, gbp);
		pub_num = GetPubNum(gbp);
		for (index=0; index < pub_num; index++) {
			LoadPap(pap, 
				PrintPubsByNumber, ajp, index, (Uint1)0, (Uint1)0, 
				line_estimate[5], A2F_REFERENCE, gbp);
		}
		for (index=0; index < gbp->comm_num; index++) {
			if (index == 0) {
				LoadPap(pap, 
					PrintFirstComment, ajp, index, (Uint1)0, (Uint1)0, 
						line_estimate[5], A2F_COMMENT, gbp);
			} else {
				LoadPap(pap, 
					PrintCommentByNumber, ajp, index, (Uint1)0, (Uint1)0, 
						line_estimate[5], A2F_COMMENT, gbp);
			}
		}
	}

	return total;
}

static void PrintLastLine (Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	PrintTerminator ();
}

static void GetFeatDefinitionLine(Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	CharPtr			string, string_start, title=NULL, fstr, label=NULL;
	ValNodePtr		vnp = NULL;
	MolInfoPtr		mfp;
	CharPtr			buf;
	Int2			buflen = 1001;
	ItemInfoPtr		iip;
	DescrStructPtr	dsp = NULL;
	Uint1			tech = 0;
	SeqFeatPtr		sfp=NULL;
	
	buf = (CharPtr)MemNew(buflen+1);
	gbp->descr = NULL;
/*****************  deflines for htg sequences *****************/
	vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_molinfo);
	if (vnp != NULL) {
		mfp = (MolInfoPtr)vnp->data.ptrvalue;
		if (mfp) {
			tech = mfp->tech;
		}
	}
	if (gbp && gbp->descr) {
		gbp->descr = (DescrStructPtr)MemFree(gbp->descr);
	}
	
	iip = (ItemInfoPtr)MemNew(sizeof(ItemInfo));
	CreateDefLine(iip, gbp->bsp, buf, buflen, tech, NULL, NULL);
	if (iip != NULL) {
		dsp = (DescrStructPtr)MemNew(sizeof(DescrStruct));
		dsp->entityID = iip->entityID;
		dsp->itemID = iip->itemID;
		dsp->itemtype = iip->itemtype;
	}
	MemFree(iip);
	gbp->descr = dsp;
	title = buf;
	string_start = string = CheckEndPunctuation(title, '.');

	while (*string != '\0')
	{
		if (*string == '\"')
			*string = '\'';
		string++;
	}
	GatherItem(ajp->entityID, ajp->itemID, OBJ_SEQFEAT, 
									&sfp, find_item);
	if (sfp)
		label = (CharPtr) FeatDefTypeLabel(sfp);
	if (label) {
		fstr = (CharPtr)MemNew(StringLen(label) + StringLen(string_start) + 7);
		sprintf(fstr, "%s from: %s", label, string_start);
	} else {
		fstr = StringSave(string_start);
	}

	gbp->defline = StringSave(fstr);
	MemFree(string_start);
	MemFree(fstr);
	MemFree(buf);
}

static Boolean find_user_object(BioseqPtr bsp)
{
	UserObjectPtr uop;
    ObjectIdPtr		oip;
    ValNodePtr desc;
	
	for (desc=bsp->descr; desc; desc=desc->next) {
		if (desc->choice == Seq_descr_user) {
			uop = desc->data.ptrvalue;
			if ((oip = uop->type) == NULL) return FALSE;
			if (StringCmp(oip->str, "RefGeneTracking") != 0) return FALSE;
			
			return TRUE;
		}
	}
	return FALSE;
}

static void PrintStatusLine(Asn2ffJobPtr ajp, GBEntryPtr gbp)
{

	BioseqPtr bsp;
	ValNodePtr desc;
	UserObjectPtr uop;
    ObjectIdPtr		oip;
	UserFieldPtr	ufp;
	CharPtr			u, ptr=NULL;
	
	if (gbp == NULL)
		return;
	bsp = gbp->bsp;
	for (desc=bsp->descr; desc; desc=desc->next) {
		if (desc->choice == Seq_descr_user) {
				uop = desc->data.ptrvalue;
			if ((oip = uop->type) == NULL) return;
			if (StringCmp(oip->str, "RefGeneTracking") != 0) return;
			for (ufp=uop->data; ufp; ufp=ufp->next) {
				oip = ufp->label;
					printf("%s\n", oip->str);
				if (StringCmp(oip->str, "Status") == 0) {
					u = (CharPtr) ufp->data.ptrvalue;
					printf("%s\n", u);
					if (StringCmp(u, "Provisional") == 0) {
						ff_StartPrint(0, 0, ASN2FF_GB_MAX, NULL);
						ff_AddString("PROVISIONAL");
						TabToColumn(13);
						ff_AddString("This is a provisional reference sequence record that has record may be somewhat different from this one.");
						ff_EndPrint();
			}
					break;
				}
			}
		}
	}
}

Int4 asn2gb_setup(Asn2ffJobPtr ajp, FFPrintArrayPtr PNTR papp)
{
	FFPrintArrayPtr pap;
	Int4 index, total, pub_num, seqblks_num;
	GBEntryPtr gbp;
	SeqIdPtr sip;
	TextSeqIdPtr tsip;
	BioseqPtr bsp;

	GetLocusPartsAwp(ajp);
	if ((gbp=ajp->asn2ffwep->gbp) != NULL) {
		if ((bsp = (BioseqPtr) gbp->bsp) != NULL) {
			for (sip=bsp->id; sip; sip=sip->next) {
				if (sip->choice == SEQID_OTHER) {
					tsip = (TextSeqIdPtr) sip->data.ptrvalue;
					if (StringNCmp(tsip->accession, "NT_", 3) == 0) {
						ajp->contig_view = TRUE;
						break;
					}
				}
			}
		}
	}
	if (!ajp->genome_view) {
		GetSeqFeat(ajp);
	}
	if (ajp->contig_view) {
		ajp->ignore_top = 0;
		GetSeqFeat(ajp);
	}
	total=0;
	for (gbp=ajp->asn2ffwep->gbp; gbp; gbp = gbp->next) {
		if ((bsp=gbp->bsp) == NULL) {
			continue;
		}
		if (gbp->bsp && ajp->id_print) {
			sip = SeqIdFindBest(gbp->bsp->id, SEQID_GI);
			if (SeqIdComp(sip, ajp->id_print) != SIC_YES) {
				continue;
			}
		}
		CheckSourceFeat(ajp, gbp);
		if (gbp->map == TRUE && ajp->show_seq == FALSE) {
			total += 7;   
		} else if (ajp->genome_view || ajp->contig_view) {
			total += 6;   
		} else {
			total += 8;
		}
		if (ajp->show_version) {
			total++;
		}
		if (ajp->show_gi && (gbp->gi != -1 || gbp->ni != NULL)) {
			total++;
		}
		if (ajp->asn2ffwep->total_seg > 0) {
			total++;
		}	
		gbp->descr = NULL;	
		if (GB_GetSeqDescrComms(ajp, gbp) > 0) {
			total += gbp->comm_num;
		}
		if	(gbp->feat && gbp->feat->sfpCommsize > 0) {
			total++;
		}
		if (ajp->genome_view || ajp->map_view || ajp->contig_view) {
			total += 2;				/* FEATURES and 'source' feature*/
			total ++;				/* last line '//' */
			if (gbp->map) {
				gbp->feat_num = GetMapFeats(ajp, gbp);
				total += gbp->feat_num;
			} else {
				total ++;
			}
			if (ajp->contig_view && gbp->feat) {
				gbp->feat_num = gbp->feat->sfpListsize;
				total += gbp->feat_num;
			}
			if (ajp->contig_view && ajp->show_seq == TRUE) {
				total += 2;				/* BASE COUNT and ORIGIN*/
				seqblks_num = (Int4) GetNumOfSeqBlks(ajp, gbp);
				total += seqblks_num;
			}
		} else {
			if (gbp->feat) {
				total += 2;				/* FEATURES and 'source' feature*/
				gbp->feat_num = gbp->feat->sfpListsize;
				total += gbp->feat_num;
			}
			if (ajp->show_seq == TRUE) {
				seqblks_num = (Int4) GetNumOfSeqBlks(ajp, gbp);
				total += seqblks_num;
			}
		}
		pub_num = (Int2)GetPubsAwp(ajp, gbp);
		total += pub_num; 
		GetGBDate(ajp, gbp);
	}
	*papp = (FFPrintArrayPtr) MemNew((size_t) total*sizeof(FFPrintArray));
	pap = *papp;
	/* pap_total = total; -- NO EFFECT */
	LoadPap(NULL, NULL, ajp, 0, (Uint1)0, (Uint1)0, 0, A2F_OTHER, NULL);
	for (gbp=ajp->asn2ffwep->gbp; gbp; gbp = gbp->next)
	{
		if (gbp->bsp && ajp->id_print) {
			sip = SeqIdFindBest(gbp->bsp->id, SEQID_GI);
			if (SeqIdComp(sip, ajp->id_print) != SIC_YES) {
				continue;
			}
		}
		LoadPap(pap, PrintLocusLine, ajp, 0, (Uint1)0, (Uint1)0,
									line_estimate[0], A2F_OTHER, gbp);
		if (gbp->descr) {
			gbp->descr = (DescrStructPtr)MemFree(gbp->descr);
			gbp->descr = NULL;
		}
		flat2asn_delete_locus_user_string();
		flat2asn_install_locus_user_string(gbp->locus);
		if (ajp->slp || ajp->itemID) {
			GetFeatDefinitionLine(ajp, gbp);
		} else {
			GetDefinitionLine(ajp, gbp);
		}
		LoadPap(pap, PrintDefinitionLine, ajp, 0, (Uint1)0, (Uint1)0, 
			line_estimate[1], A2F_OTHER, gbp);
		if (gbp->descr) {
			gbp->descr = (DescrStructPtr)MemFree(gbp->descr);
			gbp->descr = NULL;
		}
		LoadPap(pap, PrintAccessLine, ajp, 0, (Uint1)0, (Uint1)0, 
			line_estimate[2], A2F_OTHER, gbp);
		flat2asn_delete_accession_user_string();
		flat2asn_install_accession_user_string(gbp->accession);
		if (ajp->show_gi) {
			if (gbp->gi != -1) {
				LoadPap(pap, PrintNCBI_GI, ajp, 0, (Uint1)0, (Uint1)0, 
					line_estimate[2], A2F_OTHER, gbp);
			} else if (gbp->ni != NULL) {
				LoadPap(pap, PrintNID, ajp, 0, (Uint1)0, (Uint1)0, 
					line_estimate[2], A2F_OTHER, gbp);
			}
		}
		if (ajp->show_version) {
			LoadPap(pap, PrintVersionLine, ajp, 0, (Uint1)0, (Uint1)0, 
				line_estimate[2], A2F_OTHER, gbp);
		}
		LoadPap(pap, PrintKeywordLine, ajp, 0, (Uint1)0, (Uint1)0, 
			line_estimate[3], A2F_OTHER, gbp);
		if (ajp->asn2ffwep->total_seg > 0)
			LoadPap(pap, PrintSegmentLine, ajp, 0, (Uint1)0, (Uint1)0, 
				line_estimate[0], A2F_OTHER, gbp);
		LoadPap(pap, PrintGBSourceLine, ajp, 0, (Uint1)0, (Uint1)0, 
			line_estimate[4], A2F_SOURCE_FEATURE, gbp);
		LoadPap(pap, PrintGBOrganismLine, ajp, 0, (Uint1)0, (Uint1)0, 
			line_estimate[4], A2F_SOURCE_FEATURE, gbp);
		pub_num = GetPubNum(gbp);
		for (index=0; index < pub_num; index++) {
			LoadPap(pap, 
				PrintPubsByNumber, ajp, index, (Uint1)0, (Uint1)0, 
				line_estimate[5], A2F_REFERENCE, gbp);
		}
		for (index=0; index < gbp->comm_num; index++) {
			if (index == 0) {
				LoadPap(pap, 
					PrintFirstComment, ajp, index, (Uint1)0, (Uint1)0, 
						line_estimate[5], A2F_COMMENT, gbp);
			} else {
				LoadPap(pap, 
					PrintCommentByNumber, ajp, index, (Uint1)0, (Uint1)0, 
						line_estimate[5], A2F_COMMENT, gbp);
			}
		}
		if (gbp->feat && gbp->feat->sfpCommsize > 0) {
			LoadPap(pap, GBDescrComFeat, ajp, 0, (Uint1)0, (Uint1)0, 
					line_estimate[2], A2F_OTHER, gbp);
		}
		if (ajp->genome_view && gbp->map == FALSE || ajp->contig_view) {
			LoadPap(pap, PrintFeatHeader, ajp, 0, (Uint1)0, (Uint1)0, 
					line_estimate[6], A2F_OTHER, gbp);
			LoadPap(pap, PrintSourceFeat, ajp, 0, (Uint1)0, (Uint1)0, 
					line_estimate[8], A2F_SOURCE_FEATURE, gbp);
			for (index=0; index < gbp->feat_num; index++) {
				LoadPap(pap, PrintNAFeatByNumber, ajp, index, 
						(Uint1)0, (Uint1)0, line_estimate[8], A2F_FEATURE, gbp);
			}
			if (ajp->contig_view && ajp->show_seq == TRUE) {
				LoadPap(pap, PrintBaseCount, ajp, 0, (Uint1)0, (Uint1)0, 
					line_estimate[0], A2F_OTHER, gbp);
				LoadPap(pap, PrintOriginLine, ajp, 0, (Uint1)0, (Uint1)0, 
					line_estimate[0], A2F_OTHER, gbp);
				if (ajp->slp) {
					LoadPap(pap, PrintSeqRegion, ajp, index, 
							(Uint1)1, (Uint1)0, line_estimate[9], 
									A2F_SEQUENCE, gbp);
				} else {
					seqblks_num = GetNumOfSeqBlks(ajp, gbp);
					for (index=0; index < seqblks_num; index++) {
						if (seqblks_num == index+1) {
							LoadPap(pap, PrintSeqBlk, ajp, index, 
							(Uint1)1, (Uint1)0, line_estimate[9], 
									A2F_SEQUENCE, gbp);
						} else {
							LoadPap(pap, PrintSeqBlk, ajp, index, 
							(Uint1)0, (Uint1)0, line_estimate[9], 
									A2F_SEQUENCE, gbp);
						}
					}
				}
			} else {
				LoadPap(pap, PrintGenome, ajp, 0, (Uint1)0, (Uint1)0, 
					line_estimate[0], A2F_OTHER, gbp);
				LoadPap(pap, PrintLastLine, ajp, 0, (Uint1)0, (Uint1)0, 
					line_estimate[0], A2F_OTHER, gbp);
			}
		} else {
		
			if (gbp->feat) {
				LoadPap(pap, 	PrintFeatHeader, ajp, 0, (Uint1)0, (Uint1)0, 
					line_estimate[6], A2F_OTHER, gbp);
				LoadPap(pap, PrintSourceFeat, ajp, 0, (Uint1)0, (Uint1)0, 
					line_estimate[8], A2F_SOURCE_FEATURE, gbp);
				for (index=0; index < gbp->feat_num; index++) {
					LoadPap(pap, PrintNAFeatByNumber, ajp, index, 
						(Uint1)0, (Uint1)0, line_estimate[8], A2F_FEATURE, gbp);
				}
			}
			if (gbp->map == FALSE && ajp->show_seq == TRUE) {
				LoadPap(pap, PrintBaseCount, ajp, 0, (Uint1)0, (Uint1)0, 
					line_estimate[0], A2F_OTHER, gbp);
				LoadPap(pap, PrintOriginLine, ajp, 0, (Uint1)0, (Uint1)0, 
					line_estimate[0], A2F_OTHER, gbp);
				if (ajp->slp) {
					LoadPap(pap, PrintSeqRegion, ajp, index, 
							(Uint1)1, (Uint1)0, line_estimate[9], 
									A2F_SEQUENCE, gbp);
				} else {
					seqblks_num = GetNumOfSeqBlks(ajp, gbp);
					for (index=0; index < seqblks_num; index++) {
						if (seqblks_num == index+1) {
							LoadPap(pap, PrintSeqBlk, ajp, index, 
							(Uint1)1, (Uint1)0, line_estimate[9], 
									A2F_SEQUENCE, gbp);
						} else {
							LoadPap(pap, PrintSeqBlk, ajp, index, 
							(Uint1)0, (Uint1)0, line_estimate[9], 
									A2F_SEQUENCE, gbp);
						}
					}
				}
			} else {
				LoadPap(pap, PrintLastLine, ajp, 0, (Uint1)0, (Uint1)0, 
					line_estimate[0], A2F_OTHER, gbp);
			}
		}
	}

	return total;
}
Int4 asn2gr_setup(Asn2ffJobPtr ajp, FFPrintArrayPtr PNTR papp)
{
	FFPrintArrayPtr pap;
	Int4 index, total, pub_num;
	GBEntryPtr gbp;

	GetLocusPartsAwp(ajp);
	total=0;
	gbp=ajp->asn2ffwep->gbp;
	CheckSourceFeat(ajp, gbp);
	if (gbp->map == TRUE && ajp->show_seq == FALSE) {
		total += 7;   
	} else if (ajp->genome_view) {
		total += 6;   
	} else {
		total += 8;
	}
	gbp->descr = NULL;	
	if (GB_GetSeqDescrComms(ajp, gbp) > 0) {
		total += gbp->comm_num;
	}
	if	(gbp->feat && gbp->feat->sfpCommsize > 0) {
		total++;
	}
	pub_num = (Int2)GetPubsAwp(ajp, gbp);
	total += pub_num; 
	GetGBDate(ajp, gbp);
	
	*papp = (FFPrintArrayPtr) MemNew((size_t) total*sizeof(FFPrintArray));
	pap = *papp;
	LoadPap(NULL, NULL, ajp, 0, (Uint1)0, (Uint1)0, 0, A2F_OTHER, NULL);
	LoadPap(pap, PrintTaxonomy, ajp, 0, (Uint1)0, (Uint1)0,
								line_estimate[0], A2F_OTHER, gbp);
	pub_num = GetPubNum(gbp);
	for (index=0; index < pub_num; index++) {
		LoadPap(pap, 
			PrintPubsByNumber, ajp, index, (Uint1)0, (Uint1)0, 
			line_estimate[5], A2F_REFERENCE, gbp);
	}
	for (index=0; index < gbp->comm_num; index++) {
		if (index == 0) {
			LoadPap(pap, 
				PrintFirstComment, ajp, index, (Uint1)0, (Uint1)0, 
					line_estimate[5], A2F_COMMENT, gbp);
		} else {
			LoadPap(pap, 
				PrintCommentByNumber, ajp, index, (Uint1)0, (Uint1)0, 
					line_estimate[5], A2F_COMMENT, gbp);
		}
	}
	if (gbp->feat && gbp->feat->sfpCommsize > 0) {
		LoadPap(pap, GBDescrComFeat, ajp, 0, (Uint1)0, (Uint1)0, 
				line_estimate[2], A2F_OTHER, gbp);
	}
	if (ajp->genome_view && gbp->map == FALSE) {
		LoadPap(pap, PrintGenome, ajp, 0, (Uint1)0, (Uint1)0, 
			line_estimate[0], A2F_OTHER, gbp);
	}

	return total;
}

/*______________________________________________________________________
**
**	This code is not currently used.
**	I do not remove this piece of code, just comment it out.
**	-- Dmitri Lukyanov
*/
#if 0

static Int2 GetCDSNumber (OrganizeFeatPtr feat)

{
	SortStructPtr p;
	SeqFeatPtr sfp;
	Int2 i,j;

	for (p = feat->List, j=0, i=0; i < feat->sfpListsize; i++, p++) {
		sfp = p->sfp;
		if (sfp->data.choice != SEQFEAT_CDREGION) {
			continue;
		}
		j++;
	}
	return j;
}

static Int2 GetPubsRptNum (ValNodePtr pubs)
{
	PubStructPtr 	psp;
	ValNodePtr 		vnp;
	Int2 			i;

	for (vnp=pubs, i=0; vnp; vnp=vnp->next) {
		psp = vnp->data.ptrvalue;
		if (psp->choice == PUB_Sub) {
			continue;
		}
		i++;
	}
	return i;
}

#endif
/*______________________________________________________________________
*/

Int4 asn2pr_setup(Asn2ffJobPtr ajp, FFPrintArrayPtr PNTR papp)
{
	FFPrintArrayPtr pap;
	Int4 index, total, feat_num, pub_num;
	GBEntryPtr gbp;

	GetLocusPartsAwp(ajp);
	GetSeqFeat(ajp);
	total=0;
	for (gbp=ajp->asn2ffwep->gbp; gbp; gbp = gbp->next) {
		total += 2;
		if (gbp->feat) {
			total++;
			feat_num = gbp->feat->sfpListsize;
			total += feat_num;
		}
		pub_num = GetPubsAwp(ajp, gbp);
		total += pub_num; 
	}
	*papp = (FFPrintArrayPtr) MemNew((size_t) total*sizeof(FFPrintArray));
	pap = *papp;
	/* pap_total = total; -- NO EFFECT */
	LoadPap(NULL, NULL, ajp, 0, (Uint1)0, (Uint1)0, 0, A2F_OTHER, NULL);
	for (gbp=ajp->asn2ffwep->gbp; gbp; gbp = gbp->next) {
		LoadPap(pap, PrintAccessLine, ajp, 0, (Uint1)0, (Uint1)0, 
			line_estimate[2], A2F_OTHER, gbp);
		pub_num = (Int2)GetPubsAwp(ajp, gbp);
		for (index=0; index < pub_num; index++) {
			LoadPap(pap, 
				PrintPubsByNumber, ajp, index, (Uint1)0, (Uint1)0, 
				line_estimate[5], A2F_REFERENCE, gbp);
		}
		if (gbp->feat) {
			LoadPap(pap, 	PrintFeatHeader, ajp, 0, (Uint1)0, (Uint1)0, 
				line_estimate[6], A2F_OTHER, gbp);
			for (index=0; index < gbp->feat->sfpListsize; index++) {
				LoadPap(pap, PrintNAFeatByNumber, ajp, index, 
					(Uint1)0, (Uint1)0, line_estimate[8], A2F_FEATURE, gbp);
			}
		}
		LoadPap(pap, PrintLastLine, ajp, 0, (Uint1)0, (Uint1)0, 
				line_estimate[0], A2F_OTHER, gbp);
	}

	return total;
}

Int4 asn2embl_setup(Asn2ffJobPtr ajp, FFPrintArrayPtr PNTR papp)
{
	FFPrintArrayPtr pap;
	Int4 index, max, total, pub_num, seqblks_num;
	GBEntryPtr gbp;

	GetLocusPartsAwp(ajp);
	GetSeqFeat(ajp);

	total=0;
	for (gbp=ajp->asn2ffwep->gbp; gbp; gbp = gbp->next) {
		CheckSourceFeat(ajp, gbp);
		total += 7;
		gbp->xref_present = FALSE;
		if (CheckXrefLine(ajp, gbp) == TRUE) {
			total ++;
			gbp->xref_present = TRUE;
		}
		if (gbp->gi != -1 || gbp->ni != NULL) {
			total++;
		}
		gbp->descr = NULL;	
		if (GB_GetSeqDescrComms(ajp, gbp) > 0) {
			total += gbp->comm_num;
		}
		if	(gbp->feat && gbp->feat->sfpCommsize > 0) {
			total++;
		}
		if (gbp->feat) {
			total += 2;				/* FEATURES and 'source' feature*/
			total += gbp->feat->sfpListsize;
		}
		seqblks_num = GetNumOfSeqBlks(ajp, gbp);
		total += seqblks_num;
		pub_num = GetPubsAwp(ajp, gbp); 
		total += pub_num; 

		GetEMBLDate(ajp, gbp);
		GetEntryVersion(gbp);
	}
	if (ajp->ssp && ajp->hup)
		total --;
	*papp = (FFPrintArrayPtr) MemNew((size_t) total*sizeof(FFPrintArray));
	pap = *papp;

	LoadPap(NULL, NULL, ajp, 0, (Uint1)0, (Uint1)0, 0, A2F_OTHER, NULL);
	for (gbp=ajp->asn2ffwep->gbp; gbp; gbp = gbp->next) {
		LoadPap(pap,
			PrintLocusLine, ajp, 0,(Uint1)0,(Uint1)1,line_estimate[0], 
																A2F_OTHER, gbp);
		flat2asn_delete_locus_user_string();
		flat2asn_install_locus_user_string(gbp->locus);
		LoadPap(pap, 
			PrintAccessLine,ajp,0,(Uint1)0,(Uint1)1,line_estimate[2], 
																A2F_OTHER, gbp);
		flat2asn_delete_accession_user_string();
		flat2asn_install_accession_user_string(gbp->accession);
		if (gbp->gi != -1) {
			LoadPap(pap, PrintNCBI_GI, ajp, 0, (Uint1)0, (Uint1)0, 
				line_estimate[2], A2F_OTHER, gbp);
		} else if (gbp->ni != NULL) {
			LoadPap(pap, PrintNID, ajp, 0, (Uint1)0, (Uint1)0, 
				line_estimate[2], A2F_OTHER, gbp);
		}
		if (ajp->ssp == NULL || ajp->hup == FALSE) {
			LoadPap(pap, 
			PrintDateLines, ajp,0,(Uint1)0,(Uint1)1,line_estimate[10],
																A2F_OTHER, gbp);
		}
		GetDefinitionLine(ajp, gbp);
		LoadPap(pap,
			 PrintDefinitionLine,ajp,0,(Uint1)0,(Uint1)1,line_estimate[1],
			 								 					A2F_OTHER, gbp);
		LoadPap(pap, PrintKeywordLine,ajp,0,(Uint1)0,(Uint1)1,line_estimate[3],
															 A2F_OTHER, gbp);
		LoadPap(pap,
		 	PrintOrganismLine,ajp,0,(Uint1)0,(Uint1)0,line_estimate[11], 
		 													A2F_OTHER, gbp);
		pub_num = GetPubNum(gbp);
		for (index=0; index < pub_num; index++) {
			LoadPap(pap, 
				PrintPubsByNumber, ajp, index, (Uint1)0, (Uint1)0, 
				line_estimate[5], A2F_REFERENCE, gbp);
		}
		for (index=0; index < gbp->comm_num; index++) {
			if (index == 0) {
				LoadPap(pap, 
					PrintFirstComment, ajp, index, (Uint1)0, (Uint1)0, 
						line_estimate[5], A2F_COMMENT, gbp);
			} else {
				LoadPap(pap, 
					PrintCommentByNumber, ajp, index, (Uint1)0, (Uint1)0, 
						line_estimate[5], A2F_COMMENT, gbp);
			}
		}
		if (gbp->feat && gbp->feat->sfpCommsize > 0) {
			LoadPap(pap, GBDescrComFeat, ajp, 0, (Uint1)0, (Uint1)0, 
				line_estimate[2], A2F_OTHER, gbp);
		}
		if (gbp->xref_present == TRUE) {
			LoadPap(pap, 
				PrintXrefLine, ajp, 0, (Uint1)0, (Uint1)0, line_estimate[0],
															 A2F_OTHER, gbp);
		}
		if (gbp->feat) {
				LoadPap(pap, 	PrintFeatHeader, ajp, 0, (Uint1)0, (Uint1)0, 
					line_estimate[6], A2F_OTHER, gbp);
		}
		if (gbp->feat) {
			max = gbp->feat->sfpListsize;
		}
		LoadPap(pap, 
				PrintSourceFeat, ajp, 0, (Uint1)0, (Uint1)1, line_estimate[8],
													 A2F_SOURCE_FEATURE, gbp);
		for (index=0; index< max; index++) {
			if (max == index+1) {
				LoadPap(pap, 
					PrintNAFeatByNumber,ajp,index,(Uint1)0,
						(Uint1)1,line_estimate[8], A2F_FEATURE, gbp);
			} else {
				LoadPap(pap, 
					PrintNAFeatByNumber, ajp, index,(Uint1)0,
						(Uint1)0,line_estimate[8], A2F_FEATURE, gbp);
			}
		}
		LoadPap(pap, 
			PrintBaseCount, ajp, 0, (Uint1)0, (Uint1)0, line_estimate[0], 
																A2F_OTHER, gbp);
		seqblks_num = GetNumOfSeqBlks(ajp, gbp);
		for (index=0; index < seqblks_num; index++) {
			if (seqblks_num == index+1) {
				LoadPap(pap, 
				PrintSeqBlk, ajp, index, (Uint1)1, (Uint1)0, line_estimate[9],
															 A2F_SEQUENCE, gbp);
			} else {
				LoadPap(pap, 
				PrintSeqBlk, ajp, index, (Uint1)0, (Uint1)0, line_estimate[9],
															 A2F_SEQUENCE, gbp);
			}
		}
	}

	return total;
}

/*************************************************************************
*asn2gp_setup
*
*	This code calls the routines to output a GenPept Flat File
*
**************************************************************************/
Int4 asn2gp_setup(Asn2ffJobPtr ajp, FFPrintArrayPtr PNTR papp)
{

	BioseqPtr bsp;
	FFPrintArrayPtr pap;
	Int4 feat_num;
	Int2 pub_num;
	Int4 index, total;
	Int4 seqblks_num;
	GBEntryPtr gbp;
	SeqIdPtr sip;

	GetLocusPartsAwp(ajp);
	GetSeqFeat(ajp);

	total=0;
	for (gbp=ajp->asn2ffwep->gbp; gbp; gbp = gbp->next) {
		if (gbp->bsp && ajp->id_print) {
			sip = SeqIdFindBest(gbp->bsp->id, SEQID_GI);
			if (SeqIdComp(sip, ajp->id_print) != SIC_YES) {
				continue;
			}
		}
		CheckSourceFeat(ajp, gbp);
		bsp = gbp->bsp;
		if (ASN2FF_DROP_SHORT_AA == TRUE &&
			ajp->asn2ffwep->total_seg == 0 && bsp->length < GENPEPT_MIN) {
			flat2asn_delete_accession_user_string();
			flat2asn_delete_locus_user_string();
			flat2asn_install_accession_user_string(gbp->accession);
			flat2asn_install_locus_user_string(gbp->locus);
			if (ajp->error_msgs == TRUE)
				ErrPostStr(SEV_INFO, ERR_ENTRY_Partial_peptide, 
					"Entry dropped due to length.");
			continue;
		}
		total += 8;
		if (ajp->show_version) {
			total++;
		}
		if (gbp->gi != -1 || gbp->ni != NULL) {
			total++;
		}
		if (ajp->asn2ffwep->total_seg > 0) {
			total++;
		}
		gbp->descr = NULL;	
		if (GP_GetSeqDescrComms(ajp, gbp) > 0) {
			total += gbp->comm_num;
		}
		if	(gbp->feat && gbp->feat->sfpCommsize > 0) {
			total++;
		}
		if (gbp->feat) {
				total += 2;				/* FEATURES and 'source' feature*/
				feat_num = gbp->feat->sfpListsize;
				total += feat_num;
		}
		seqblks_num = GetNumOfSeqBlks(ajp, gbp);
		total += seqblks_num;
		pub_num = (Int2)GetPubsAwp(ajp, gbp);
		total += pub_num; 

		GetGPDate(ajp, gbp);
	}

	*papp = (FFPrintArrayPtr) MemNew((size_t) total*sizeof(FFPrintArray));
	pap = *papp;

	LoadPap(NULL, NULL, ajp, 0, (Uint1)0, (Uint1)0, 0, A2F_OTHER, NULL);
	for (gbp=ajp->asn2ffwep->gbp; gbp; gbp = gbp->next) {
		bsp = gbp->bsp;
		if (bsp && ajp->id_print) {
			sip = SeqIdFindBest(bsp->id, SEQID_GI);
			if (SeqIdComp(sip, ajp->id_print) != SIC_YES) {
				continue;
			}
		}
		if (ASN2FF_DROP_SHORT_AA == TRUE &&
			ajp->asn2ffwep->total_seg == 0 && bsp->length < GENPEPT_MIN) {
			continue;
		}
		LoadPap(pap, 
			PrintLocusLine, ajp, 0, (Uint1)0, (Uint1)0, line_estimate[0],
																A2F_OTHER, gbp);
		flat2asn_delete_locus_user_string();
		flat2asn_install_locus_user_string(gbp->locus);
		GetDefinitionLine(ajp, gbp);
		LoadPap(pap,
			PrintDefinitionLine, ajp, 0, (Uint1)0, (Uint1)0, line_estimate[1], 
											A2F_OTHER, gbp);
                MemFree(gbp->descr);
		gbp->descr = NULL;
		LoadPap(pap, 
			PrintAccessLine, ajp, 0, (Uint1)0, (Uint1)0, line_estimate[2],
															 A2F_OTHER, gbp);
		flat2asn_delete_accession_user_string();
		flat2asn_install_accession_user_string(gbp->accession);
		if (gbp->gi != -1) {
			LoadPap(pap, PrintNCBI_GI, ajp, 0, (Uint1)0, (Uint1)0, 
				line_estimate[2], A2F_OTHER, gbp);
		} else if (gbp->ni != NULL) {
			LoadPap(pap, PrintNID, ajp, 0, (Uint1)0, (Uint1)0, 
				line_estimate[2], A2F_OTHER, gbp);
		}
		if (ajp->show_version) {
			LoadPap(pap, PrintVersionLine, ajp, 0, (Uint1)0, (Uint1)0, 
				line_estimate[2], A2F_OTHER, gbp);
		}
		LoadPap(pap, 
			PrintDBSourceLine, ajp, 0, (Uint1)0, (Uint1)0, line_estimate[12], 
																A2F_OTHER, gbp);
		LoadPap(pap, 
			PrintKeywordLine, ajp, 0, (Uint1)0, (Uint1)0, line_estimate[3], 
																A2F_OTHER, gbp);
		if (ajp->asn2ffwep->total_seg > 0)
			LoadPap(pap, 
				PrintSegmentLine, ajp, 0, (Uint1)0, (Uint1)0, line_estimate[0], 
																A2F_OTHER, gbp);
		LoadPap(pap, PrintGBSourceLine, ajp, 0, (Uint1)0, (Uint1)0, 
			line_estimate[4], A2F_OTHER, gbp);
		LoadPap(pap, PrintGBOrganismLine, ajp, 0, (Uint1)0, (Uint1)0, 
			line_estimate[4], A2F_OTHER, gbp);
		pub_num = GetPubNum(gbp);
		for (index=0; index < pub_num; index++) {
			LoadPap(pap, 
				PrintPubsByNumber, ajp, index, (Uint1)0, (Uint1)0, 
				line_estimate[5], A2F_REFERENCE, gbp);
		}
		for (index=0; index < gbp->comm_num; index++) {
			if (index == 0) {
				LoadPap(pap, 
					PrintFirstComment, ajp, index, (Uint1)0, (Uint1)0, 
						line_estimate[5], A2F_COMMENT, gbp);
			} else {
				LoadPap(pap, 
					PrintCommentByNumber, ajp, index, (Uint1)0, (Uint1)0, 
						line_estimate[5], A2F_COMMENT, gbp);
			}
		}
		if (gbp->feat && gbp->feat->sfpCommsize > 0) {
			LoadPap(pap, GBDescrComFeat, ajp, 0, (Uint1)0, (Uint1)0, 
					line_estimate[2], A2F_OTHER, gbp);
		}
		if (gbp->feat) {
			LoadPap(pap, 	PrintFeatHeader, ajp, 0, (Uint1)0, (Uint1)0, 
				line_estimate[6], A2F_OTHER, gbp);
			LoadPap(pap, PrintSourceFeat, ajp, 0, (Uint1)0, (Uint1)0, 
				line_estimate[8], A2F_SOURCE_FEATURE, gbp);
			if (gbp->feat) {
				feat_num = gbp->feat->sfpListsize;
			}
			for (index=0; index < feat_num; index++) {
				LoadPap(pap, PrintAAFeatByNumber, ajp, index, 
					(Uint1)0, (Uint1)0, line_estimate[8], A2F_FEATURE, gbp);
			}
		}
		LoadPap(pap, 
			PrintOriginLine, ajp, 0, (Uint1)0, (Uint1)0, line_estimate[0], 
																A2F_OTHER, gbp);
		seqblks_num = GetNumOfSeqBlks(ajp, gbp);
		for (index=0; index < seqblks_num; index++) {
			if (seqblks_num == index+1) {
				LoadPap(pap, 
					PrintSeqBlk, ajp, index, (Uint1)1, (Uint1)0, 
										line_estimate[9], A2F_SEQUENCE, gbp);
			} else {
				LoadPap(pap, 
					PrintSeqBlk, ajp, index, (Uint1)0, (Uint1)0, 
										line_estimate[9], A2F_SEQUENCE, gbp);
			}
		}
	}

	return total;
} 

/*************************************************************************
*asn2ep_setup
*
*	This code calls the routines to output an "EMBLPept" Flat File
*
**************************************************************************/

Int4 asn2ep_setup(Asn2ffJobPtr ajp, FFPrintArrayPtr PNTR papp)
{

	BioseqPtr bsp;
	FFPrintArrayPtr pap;
	Int4 index, total;
	Int4 feat_num;
	Int2 pub_num;
	Int4 seqblks_num;
	GBEntryPtr gbp;


	ajp->format = GENPEPT_FMT;
	GetLocusPartsAwp(ajp);
	GetSeqFeat(ajp);
	ajp->format = EMBLPEPT_FMT;

	total=0;
	for (gbp=ajp->asn2ffwep->gbp; gbp; gbp = gbp->next) {
		CheckSourceFeat(ajp, gbp);
		bsp = gbp->bsp;
		if (ASN2FF_DROP_SHORT_AA == TRUE && ajp->asn2ffwep->total_seg == 0 && 
				bsp->length < GENPEPT_MIN) {
			flat2asn_delete_accession_user_string();
			flat2asn_delete_locus_user_string();
			flat2asn_install_accession_user_string(gbp->accession);
			flat2asn_install_locus_user_string(gbp->locus);
			if (ajp->error_msgs == TRUE)
				ErrPostStr(SEV_INFO, ERR_ENTRY_Partial_peptide, 
					"Entry dropped due to length.");
			continue;
		}
		total += 8;
		if (ajp->asn2ffwep->total_seg > 0) {
			total++;
		}
		if (GP_GetSeqDescrComms(ajp, gbp) > 0) {
			total += gbp->comm_num;
		}
		if	(gbp->feat && gbp->feat->sfpCommsize > 0) {
			total++;
		}
		if (gbp->feat) {
				total += 2;				/* FEATURES and 'source' feature*/
				feat_num = gbp->feat->sfpListsize;
				total += feat_num;
		}
		seqblks_num = GetNumOfSeqBlks(ajp, gbp);
		total += seqblks_num;
		pub_num = (Int2)GetPubsAwp(ajp, gbp);
		total += pub_num; 

		GetEMBLDate(ajp, gbp);
		GetEntryVersion(gbp);
	}

	*papp = (FFPrintArrayPtr) MemNew((size_t) total*sizeof(FFPrintArray));
	pap = *papp;

	LoadPap(NULL, NULL, ajp, 0, (Uint1)0, (Uint1)0, 0, A2F_OTHER, NULL);
	for (gbp=ajp->asn2ffwep->gbp; gbp; gbp = gbp->next) {
		bsp = gbp->bsp;
		if (ASN2FF_DROP_SHORT_AA == TRUE && ajp->asn2ffwep->total_seg == 0 && 
				bsp->length < GENPEPT_MIN) {
			continue;
		}
		LoadPap(pap, 
			PrintEPLocusLine, ajp, 0, (Uint1)0, (Uint1)1, line_estimate[0],
															 	A2F_OTHER, gbp);
		flat2asn_delete_locus_user_string();
		flat2asn_install_locus_user_string(gbp->locus);
		LoadPap(pap, 
			PrintAccessLine, ajp, 0, (Uint1)0, (Uint1)1, line_estimate[2],
															 	A2F_OTHER, gbp);
		flat2asn_delete_accession_user_string();
		flat2asn_install_accession_user_string(gbp->accession);
		LoadPap(pap, 
			PrintDateLines, ajp,  0, (Uint1)0, (Uint1)1, line_estimate[10], 
																A2F_OTHER, gbp);
		GetDefinitionLine(ajp, gbp);
		LoadPap(pap, 
			PrintDefinitionLine,  ajp, 0, (Uint1)0, (Uint1)1, line_estimate[1], 
																A2F_OTHER, gbp);
		LoadPap(pap, 
			PrintKeywordLine, ajp, 0, (Uint1)0, (Uint1)1, line_estimate[3], 
																A2F_OTHER, gbp);
		LoadPap(pap, 
			PrintOrganismLine, ajp, 0, (Uint1)0, (Uint1)0, line_estimate[11], 
																A2F_OTHER, gbp);
		pub_num = GetPubNum(gbp);
		for (index=0; index < pub_num; index++) {
			LoadPap(pap, PrintPubsByNumber, ajp, index, (Uint1)0, (Uint1)0, 
								line_estimate[5], A2F_REFERENCE, gbp);
		} 
		for (index=0; index < gbp->comm_num; index++) {
			if (index == 0) {
				LoadPap(pap, 
					PrintFirstComment, ajp, index, (Uint1)0, (Uint1)0, 
						line_estimate[5], A2F_COMMENT, gbp);
			} else {
				LoadPap(pap, 
					PrintCommentByNumber, ajp, index, (Uint1)0, (Uint1)0, 
						line_estimate[5], A2F_COMMENT, gbp);
			}
		}
		if (gbp->feat && gbp->feat->sfpCommsize > 0) {
			LoadPap(pap, GBDescrComFeat, ajp, 0, (Uint1)0, (Uint1)0, 
					line_estimate[2], A2F_OTHER, gbp);
		}
		if (gbp->feat) {
			LoadPap(pap, 	PrintFeatHeader, ajp, 0, (Uint1)0, (Uint1)0, 
				line_estimate[6], A2F_OTHER, gbp);
			LoadPap(pap, PrintSourceFeat, ajp, 0, (Uint1)0, (Uint1)0, 
				line_estimate[8], A2F_SOURCE_FEATURE, gbp);
			for (index=0; index < gbp->feat->sfpListsize; index++) {
				LoadPap(pap, PrintAAFeatByNumber, ajp, index, 
					(Uint1)0, (Uint1)0, line_estimate[8], A2F_FEATURE, gbp);
			}
		}
		seqblks_num = GetNumOfSeqBlks(ajp, gbp);
		for (index=0; index < seqblks_num; index++) {
			if (seqblks_num == index+1) {
				LoadPap(pap, 
					PrintSeqBlk, ajp, index, (Uint1)1, (Uint1)0, 
										line_estimate[9], A2F_SEQUENCE, gbp);
			} else {
				LoadPap(pap, 
					PrintSeqBlk, ajp, index, (Uint1)0, (Uint1)0, 
										line_estimate[9], A2F_SEQUENCE, gbp);
			}
		}
	}
	return total;
}	

static void FreeSortStructLoc(Int4 size, SortStructPtr p)
{
	Int4 index;
	Int4 size_loc;
	
				size_loc = p->extra_loc_cnt;
				if (size_loc > 0) {
					for (index=0; index < size_loc; index++) {
						SeqLocFree(p->extra_loc[index]);
					}
					MemFree(p->extra_loc);
				}
				for (index=0; index < size; index++) {
					if (p[index].feat_free == TRUE) {
						SeqFeatFree(p[index].sfp);
					}
				}
	return;
}

/**********************************************************/
static void FreeSortStructSet(Int4 size, SortStructPtr ssp)
{
    SortStructPtr p;
    Int4          i;

    if(size <= 0)
        return;

    for(i = 0, p = ssp; i < size; i++, p++)
    {
        if(p == NULL)
            continue;
        if(p->gsp != NULL)
            GeneStructFree(p->gsp);
        if(p->nsp != NULL)
            NoteStructFree(p->nsp);
    }
    FreeSortStructLoc(size, ssp);
    MemFree(ssp);
}

/**********************************************************/
NLM_EXTERN void asn2ff_cleanup(Asn2ffJobPtr ajp)
{
    GBEntryPtr      gbp;
    GBEntryPtr      next;
    ValNodePtr      v;
    ValNodePtr      vnext;
    ComStructPtr    s;
    ComStructPtr    snext;
    OrganizeFeatPtr ofp;

#if 0 /***have no idea why this is needed (EY) ***/
	
	if (get_www()) {
		return;
	}
#endif

    if(ajp->asn2ffwep != NULL)
    {
        for(gbp = ajp->asn2ffwep->gbp; gbp != NULL; gbp = next)
        {
            next = gbp->next;
            if(gbp->spp != NULL)
                SeqPortFree(gbp->spp);
            if(gbp->base_cnt_line != NULL)
                MemFree(gbp->base_cnt_line);
            if(gbp->feat != NULL)
            {
                ofp = gbp->feat;
                NoteStructFree(ofp->source_notes);
                FreeSortStructSet(ofp->sfpListsize, ofp->List);
                FreeSortStructSet(ofp->sfpCommsize, ofp->Commlist);
                FreeSortStructSet(ofp->sfpGenesize, ofp->Genelist);
                FreeSortStructSet(ofp->sfpOrgsize, ofp->Orglist);
                FreeSortStructSet(ofp->sfpSitesize, ofp->Siteslist);
                FreeSortStructSet(ofp->sfpSourcesize, ofp->Sourcelist);
                FreeSortStructSet(ofp->sfpXrefsize, ofp->Xreflist);
                FreeSortStructSet(ofp->biosrcsize, ofp->Biosrclist);
                MemFree(ofp);
            }
            for(v = gbp->Pub; v != NULL; v = vnext)
            {
                vnext = v->next;
                FreePubStruct(v->data.ptrvalue);
                MemFree(v);
            }
            for(s = gbp->comm; s != NULL; s = snext)
            {
                snext = s->next;
                MemFree(s->string);
                MemFree(s);
            }
            if(gbp->source_info != NULL)
            {
                MemFree(gbp->source_info);
            }
            if(gbp->defline != NULL)
            {
                MemFree(gbp->defline);
            }
            MemFree(gbp);
        }
        MemFree(ajp->asn2ffwep);
    }
    SeqFeatFree(ajp->sfp_out);

    /* Delete these strings so they don't interfere with others
     */
    flat2asn_delete_locus_user_string();
    flat2asn_delete_accession_user_string();
}

/*****************************************************************************
*	void LoadPap(FFPrintArrayPtr pap, FFPapFct fct, Asn2ffJobPtr ajp, 
*	Int4 index, Uint1 last, Uint1 printxx, Uint1 element_type) 
*
*	This function places the parameters in the correct spaces in the
*	FFPrintArrayPtr.
*
****************************************************************************/
void LoadPap(FFPrintArrayPtr pap, FFPapFct fct, Asn2ffJobPtr ajp, Int4 index, Uint1 last, Uint1 printxx, Int2 estimate, Uint1 element_type, GBEntryPtr gbp) 
{
	static Int4 pap_index;
	DescrStructPtr dsp;
	
	if (! pap) {
		pap_index=0;
	} else {
		pap[pap_index].fct = fct;
		pap[pap_index].ajp = ajp;
		pap[pap_index].gbp = gbp;
		pap[pap_index].index = index;
		pap[pap_index].last = last;
		pap[pap_index].printxx = printxx;
		pap[pap_index].estimate = estimate;
		pap[pap_index].descr = NULL;
		if (element_type == A2F_SOURCE_FEATURE) {
			dsp =  (DescrStructPtr) MemNew(sizeof(DescrStruct));
			pap[pap_index].descr = dsp;
			if (gbp->feat && gbp->feat->Sourcelist != NULL) {
				dsp->entityID = gbp->feat->Sourcelist[0].entityID;
				dsp->itemID = gbp->feat->Sourcelist[0].itemID;
				dsp->itemtype = gbp->feat->Sourcelist[0].itemtype;
			} else if (gbp->source_info != NULL) {
				dsp->entityID = gbp->source_info->entityID;
				dsp->itemID = gbp->source_info->itemID;
				dsp->itemtype = gbp->source_info->itemtype;
			}
		} else if (element_type == A2F_FEATURE && gbp->feat) {
			GetPapSeqFeatPtr (gbp, index, pap_index, pap);
		} else if (element_type == A2F_REFERENCE) {
			GetPapRefPtr (ajp, gbp, index, pap_index, pap);
		} else if (element_type == A2F_FEATURE_NEW && gbp->feat) {
			dsp =  (DescrStructPtr) MemNew(sizeof(DescrStruct));
			pap[pap_index].descr = dsp;
			dsp->entityID = gbp->feat->List[index].entityID;
			dsp->itemID = gbp->feat->List[index].itemID;
			dsp->itemtype = gbp->feat->List[index].itemtype;
		} else if (element_type == A2F_COMMENT) {
			GetPapCommPtr (ajp, gbp, index, pap_index, pap);
		} else if (element_type == A2F_SEQUENCE) {
			dsp =  (DescrStructPtr) MemNew(sizeof(DescrStruct));
			pap[pap_index].descr = dsp;
			dsp->entityID = gbp->entityID;
			dsp->itemID = gbp->itemID;
			dsp->itemtype = gbp->itemtype;
		} else {
			if (gbp->descr != NULL) {
				dsp =  (DescrStructPtr) MemNew(sizeof(DescrStruct));
				pap[pap_index].descr = dsp;
				dsp->entityID = gbp->descr->entityID;
				dsp->itemID = gbp->descr->itemID;
				dsp->itemtype = gbp->descr->itemtype;
			}
		}
		pap_index++;
	}

	return;
}

/****************************************************************************
*	This function checks a SeqPortPtr, maintained on the Biotable Ptr, 
*	and compares it's BioseqPtr to that of the BioseqPtr associated
*	with segment count of the btp.  At present, used only for nucleic
*	acids (4/14/94).
****************************************************************************/

void CheckSeqPort (Asn2ffJobPtr ajp, GBEntryPtr gbp, Int4 start)
{
	BioseqPtr bsp=gbp->bsp;
	SeqPortPtr spp=gbp->spp;
	Int4 start1;

	if (spp) {
		if (ajp->slp == NULL && bsp == spp->bsp) {
			if (spp->curpos != start)
				SeqPortSeek(spp, start, SEEK_SET);
		} else {
			SeqPortFree(spp);
			if (ajp->slp) {
			/*	spp = SeqPortNew(bsp, 0, -1, 0, Seq_code_iupacna);
				start1 = start - spp->start - SeqLocStart(ajp->slp);*/
				spp = SeqPortNewByLoc(ajp->slp, Seq_code_iupacna);
				start1 = start - spp->start;
			} else {
				spp = SeqPortNew(bsp, 0, -1, 0, Seq_code_iupacna);
				start1 = start - spp->start;
			}
			if (start1 != spp->curpos)
				SeqPortSeek(spp, start1, SEEK_SET);
		}
	} else {
		if (ajp->slp) {
			spp = SeqPortNewByLoc(ajp->slp, Seq_code_iupacna);
			if (spp != NULL) {
			  start1 = start - spp->start;
			}
		/*	spp = SeqPortNew(bsp, 0, -1, 0, Seq_code_iupacna);
			start1 = start - spp->start - SeqLocStart(ajp->slp);*/
		} else {
			spp = SeqPortNew(bsp, 0, -1, 0, Seq_code_iupacna);
			if (spp != NULL) {
		  	start1 = start - spp->start;
			}
		}
		if (spp != NULL && start1 != spp->curpos) 
			SeqPortSeek(spp, start1, SEEK_SET);
	}
	gbp->spp = spp;

	return;
}


/***************************************************************************
*
*	"GetMolInfo" gets information about the molecule for the locus
*	line.  The formatted information is in "buffer".
*
***************************************************************************/

void GetMolInfo (Asn2ffJobPtr ajp, CharPtr buffer, GBEntryPtr gbp)
{
	static CharPtr strand [4]= { "   ", "ss-", "ds-","ms-"};

	/* WARNING : The mol[] table was originally designed to convert
	   MolInfo->biomol values <= 8 in the ASN.1 spec to molecule type strings.
	   The addition of snoRNA with biomol value 12 breaks this design.
	   Also, the new LOCUS line format requires larger molecule type
	   strings, since the space for that field has been increased from 4 to 6.
	   Hence new_locus_mol, utilized if ajp->old_locus_fmt is *NOT* set. */
	
	static CharPtr mol [10] = {"    ", "DNA ", "RNA ", "mRNA", "rRNA", "tRNA", "uRNA", "scRNA", " AA ","oRNA"};

	static CharPtr new_locus_mol [10] = {"      ", "DNA   ", "RNA   ", "mRNA  ", "rRNA  ", "tRNA  ", "uRNA  ", "scRNA ", " AA   ", "snoRNA"};

	static CharPtr embl_mol [8] = {"xxx", "DNA", "RNA", "RNA", "RNA", "RNA", "RNA", "AA "};

	BioseqPtr bsp;
	Int2 istrand, imol;
	ValNodePtr vnp = NULL;
	MolInfoPtr mfp;
	Int4 length;
	
	if ((bsp = gbp->bsp) == NULL)
		return;
	istrand = bsp->strand;
	if (istrand > 3) 
		istrand = 0;

	imol = bsp->mol;
	if (imol > 3)
		imol = 0;

	if (ajp->slp) {
		length = SeqLocLen(ajp->slp);
	} else {
		length = bsp->length;
	}
/*keep both old and new style, get new first*/
	if ((vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_molinfo)) != NULL) {
		mfp = (MolInfoPtr)vnp->data.ptrvalue;
		if (mfp->biomol <= 8) {
			imol = (Int2) (mfp->biomol);
		} else if (mfp->biomol == 12) {
			imol = 9;
		}
	} else {
		for (vnp = bsp->descr; vnp; vnp = vnp->next) {
			if (vnp->choice == Seq_descr_mol_type) {
				if (vnp->data.intvalue <= 8) {
					imol = (Int2) (vnp->data.intvalue);
				}
				break;
			}
		}
	}
	if (imol < 2) {  /* check Seq-inst.mol if mol-type is not-set or genomic */
		imol = bsp->mol;
		if (imol == 3)
			imol = 8;
		if (imol == 4)
			imol = 0;
	}

/* if ds-DNA don't show ds */
	if (imol == 1 && istrand == 2) { 
		istrand = 0;
	} 
/* ss-any RNA don't show ss */
	if (imol > 2 && istrand == 1) { 
		istrand = 0;
	} 
	if (ajp->slp != NULL) {
		bsp->topology = 1;
	}
	
	if (ajp->format == GENBANK_FMT || ajp->format == SELECT_FMT) {
		if (bsp->topology == 2) {
		  if (ajp->old_locus_fmt == TRUE) {
			sprintf(buffer, "%7ld bp %s%-4s  circular", 
				(long) length, strand[istrand], mol[imol]);
		  } else {
			sprintf(buffer, "%12ld bp %s%-6s  circular", 
				(long) length, strand[istrand], new_locus_mol[imol]);
		  }
		} else {
		  if (ajp->old_locus_fmt == TRUE) {
 			sprintf(buffer, "%7ld bp %s%-4s          ", 
 				(long) length, strand[istrand], mol[imol]);
		  } else {
			sprintf(buffer, "%12ld bp %s%-6s  linear  ", 
				(long) length, strand[istrand], new_locus_mol[imol]);
		  }
		}
	} else if (ajp->format == GENPEPT_FMT) {
		  if (ajp->old_locus_fmt == TRUE) {
			sprintf(buffer, "%7ld aa", (long) length);
		  } else {
			sprintf(buffer, "%12ld aa            linear", (long) length);
		  }
	} else if (ajp->format == EMBL_FMT || ajp->format == PSEUDOEMBL_FMT ||
		ajp->format == EMBLPEPT_FMT) {
		if (ajp->pseudo == FALSE) { /* do authentic EMBL */
			if (imol < 8) {
				if (bsp->topology == 2)
					sprintf(buffer, "circular %s", embl_mol[imol]);
				else
					sprintf(buffer, "%s", embl_mol[imol]);
			}
		} else {	/* Use GenBank molecule names */
			if (bsp->topology == 2)
				sprintf(buffer, "circular %s", mol[imol]);
			else
				sprintf(buffer, "%s", mol[imol]);
		}
	}
	return;
}

/*************************************************************************
*	Checks if there is a Xref in EMBL format.
*	Used ONLY to make EMBL output.
*This could probably be done more efficiently???????????????????
**************************************************************************/

Boolean CheckXrefLine (Asn2ffJobPtr ajp, GBEntryPtr gbp)

{
	Boolean ret_val=FALSE;
	Char buffer[20];
	CharPtr name;
	EMBLBlockPtr eb=NULL;
	EMBLXrefPtr xref=NULL;
	ValNodePtr descr=NULL, ds_vnp, tvnp;
	DescrStructPtr dsp;

	tvnp = GatherDescrListByChoice(ajp, gbp, Seq_descr_embl);
	for (descr=	tvnp;
				descr; descr=descr->next) {
		dsp = (DescrStructPtr)descr->data.ptrvalue;
		ds_vnp = dsp->vnp;
		eb = (EMBLBlockPtr) ds_vnp->data.ptrvalue;
		for (xref=eb->xref; xref; xref=xref->next) {
			name=NULL;
			if (xref->_class) {
				if (xref->_class == 5)
					StringCpy(buffer, "SWISS-PROT");
				else if (xref->_class == 8)
					StringCpy(buffer, "EPD");
				else if (xref->_class == 10)
					StringCpy(buffer, "TFD");
				else if (xref->_class == 11)
					StringCpy(buffer, "FLYBASE");
				name = &(buffer[0]);
			} else if (xref->name) {
				name = xref->name;
			}
			if (name && xref->id)
				ret_val = TRUE;
			else
				ret_val = FALSE;
		}
		MemFree(ds_vnp);
	}
	ValNodeFreeData(tvnp);
	return ret_val;
}

void PrintLocusLine(Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	BioseqPtr bsp;
	Char buffer[34];	/* Gack, what a hack! */

	if (gbp == NULL)
		return;
	gbp->descr = NULL;
	bsp=gbp->bsp;
	if (ajp->format == EMBL_FMT || ajp->format == PSEUDOEMBL_FMT ||
				ajp->format == EMBLPEPT_FMT)
	{
		ff_StartPrint(5, 0, ASN2FF_EMBL_MAX, "ID");
		ff_AddString(gbp->locus);
		if (ajp->hup == TRUE) {
			ff_AddString(" confidential; ");
		} else {
			ff_AddString(" standard; ");
		}
		GetMolInfo(ajp, buffer, gbp);
		ff_AddString( buffer);
		ff_AddString("; ");
		if (ajp->ssp && ajp->format == EMBL_FMT && *(gbp->div) == ' ') {
			ff_AddString("UNA");			
		} else {
			ff_AddString(gbp->div);
		}
		ff_AddString("; ");
		if (ajp->slp) {
			ff_AddInteger("%ld", (long) SeqLocLen(ajp->slp));
		} else {
			ff_AddInteger("%ld", (long) bsp->length);
		}
		ff_AddString(" BP.");
		ff_EndPrint();
	} else {
		ff_StartPrint(0, 0, ASN2FF_GB_MAX, NULL);
		ff_AddString("LOCUS");
		TabToColumn(13);
		ff_AddString( gbp->locus);
		GetMolInfo(ajp, buffer, gbp);
		ff_AddString( buffer);

		if (ajp->old_locus_fmt)
		  TabToColumn(53);
		else
		  TabToColumn(65);
		ff_AddString(gbp->div);

		if (ajp->old_locus_fmt)
		  TabToColumn(63);
		else
		  TabToColumn(69);
		ff_AddString(gbp->date);

		ff_EndPrint();
	}
}

void PrintEPLocusLine(Asn2ffJobPtr ajp, GBEntryPtr gbp)

{
	BioseqPtr bsp=gbp->bsp;
	Char buffer[30];

	if (gbp == NULL)
		return;
	gbp->descr = NULL;	
	ff_StartPrint(5, 0, ASN2FF_EMBL_MAX, "ID");
	ff_AddString(gbp->locus);
	ff_AddString(" standard; ");
	GetMolInfo(ajp, buffer, gbp);
	ff_AddString(buffer);
	ff_AddString("; ");
	ff_AddString(gbp->div);
	ff_AddString("; ");
	ff_AddInteger("%ld", (long) bsp->length);
	ff_AddString(" RS.");
	ff_EndPrint();
}


void PrintAccessLine(Asn2ffJobPtr ajp, GBEntryPtr gbp)

{

	if (gbp == NULL)
		return;
	gbp->descr = NULL;
	if (ajp->format == EMBL_FMT || ajp->format == PSEUDOEMBL_FMT ||
				ajp->format == EMBLPEPT_FMT)
	{
		ff_StartPrint(5, 5, ASN2FF_EMBL_MAX, "AC");
	}
	else
	{
		ff_StartPrint(0, 12, ASN2FF_GB_MAX, NULL);
		ff_AddString( "ACCESSION");
		TabToColumn(13);
	}
	if (ajp->ssp && ajp->hup) {
		ff_AddChar(';');
	} else  if (ajp->slp)  {
		www_accession(gbp->accession); 
	} else {
		ff_AddString(gbp->accession);
	}
	if (ajp->slp == NULL) {
		AddExtraAccessions(ajp, gbp);
	}
	ff_EndPrint();
	return;
}

void PrintVersionLine(Asn2ffJobPtr ajp, GBEntryPtr gbp)
{

	if (gbp == NULL)
		return;
	gbp->descr = NULL;
	if (ajp->format == EMBL_FMT || ajp->format == PSEUDOEMBL_FMT ||
				ajp->format == EMBLPEPT_FMT)
	{
		ff_StartPrint(5, 5, ASN2FF_EMBL_MAX, "AC");
	}
	else
	{
		ff_StartPrint(0, 12, ASN2FF_GB_MAX, NULL);
		ff_AddString( "VERSION");
		TabToColumn(13);
	}
	ff_AddString(gbp->version);
	if (gbp->gi != -1) {
		ff_AddString( "  GI:");
		ff_AddInteger("%ld", (long) gbp->gi);
	}
	ff_EndPrint();
	return;
}

void PrintNCBI_GI(Asn2ffJobPtr ajp, GBEntryPtr gbp)

{

	if (gbp == NULL)
		return;
	gbp->descr = NULL;

	if (ajp->format == EMBL_FMT || ajp->format == PSEUDOEMBL_FMT ||
				ajp->format == EMBLPEPT_FMT) {
		ff_StartPrint(5, 5, ASN2FF_EMBL_MAX, "NI");
	} else {
		ff_StartPrint(0, 12, ASN2FF_GB_MAX, NULL);
		if (ajp->format == GENBANK_FMT) {
			ff_AddString( "NID");
		} else if (ajp->format == GENPEPT_FMT) {
			ff_AddString( "PID");
		}
		TabToColumn(13);
	}
	ff_AddChar('g');
	ff_AddInteger("%ld", (long) gbp->gi);
/*		if (ajp->format == GENBANK_FMT) {
			TabToColumn(26);
			ff_AddString( "GI:");
			ff_AddInteger("%ld", (long) gbp->gi);
		}
*/
	ff_EndPrint();
	if (ajp->format == EMBL_FMT || ajp->format == PSEUDOEMBL_FMT ||
				ajp->format == EMBLPEPT_FMT) {
		PrintXX();
	}
	return;
}
void PrintNID(Asn2ffJobPtr ajp, GBEntryPtr gbp)

{

	if (gbp == NULL)
		return;
	gbp->descr = NULL;

	if (ajp->format == EMBL_FMT || ajp->format == PSEUDOEMBL_FMT ||
				ajp->format == EMBLPEPT_FMT) {
		ff_StartPrint(5, 5, ASN2FF_EMBL_MAX, "NI");
		ff_AddString(gbp->ni);
	} else {
		ff_StartPrint(0, 12, ASN2FF_GB_MAX, NULL);
		if (ajp->format == GENBANK_FMT) {
			ff_AddString( "NID");
		} else if (ajp->format == GENPEPT_FMT) {
			ff_AddString( "PID");
		}
		TabToColumn(13);
		ff_AddString(gbp->ni);
/*		if (gbp->gi != -1) {
			TabToColumn(26);
			ff_AddString( "GI:");
			ff_AddInteger("%ld", (long) gbp->gi);
		}
*/
	}
	ff_EndPrint();
	if (ajp->format == EMBL_FMT || ajp->format == PSEUDOEMBL_FMT ||
				ajp->format == EMBLPEPT_FMT) {
		PrintXX();
	}
	return;
}
void PrintDateLines (Asn2ffJobPtr ajp, GBEntryPtr gbp)

{
	if (gbp == NULL)
		return;
	gbp->descr = NULL;
	ff_StartPrint(5, 5, ASN2FF_EMBL_MAX, "DT");
	if (gbp->update_date)
	{	/* both create and update date exist.	*/
		if (ajp->pseudo == FALSE)
		{ /* In pseudo-EMBL mode only one date line */
			if (gbp->create_date) {
				ff_AddString(gbp->create_date);
				NewContLine();
			}
		}
		ff_AddString(gbp->update_date);
		if (gbp->embl_rel)
		{
			ff_AddString(" (Rel. ");
			ff_AddString(gbp->embl_rel);
			ff_AddString(", Last updated, Version ");
			ff_AddInteger("%ld", (long) gbp->embl_ver);
			ff_AddChar(')');
		}
	}
	else
	{	/* only create date exists.	*/
		ff_AddString(gbp->create_date);
		if (gbp->embl_rel)
		{
			ff_AddString(" (Rel. ");
			ff_AddString(gbp->embl_rel);
			ff_AddString(", Last updated, Version ");
			ff_AddInteger("%ld", (long) gbp->embl_ver);
			ff_AddChar(')');
		}
		if (ajp->pseudo == FALSE)
		{ /* In pseudo-EMBL only one date line. */
			NewContLine();
			ff_AddString(gbp->create_date);
			if (gbp->embl_rel)
			{
				ff_AddString(" (Rel. ");
				ff_AddString(gbp->embl_rel);
				ff_AddString(", Last updated, Version ");
				ff_AddInteger( "%ld", (long) gbp->embl_ver);
				ff_AddChar(')');
			}
		}
	}
	ff_EndPrint();
}	/* PrintDateLines */

void PrintSegmentLine(Asn2ffJobPtr ajp, GBEntryPtr gbp)
{

	if (gbp == NULL)
		return;
	gbp->descr = NULL;
	if (ajp->asn2ffwep->total_seg > 1)
	{
		ff_StartPrint(0, 0, ASN2FF_GB_MAX, NULL);
		ff_AddString("SEGMENT");
		TabToColumn(13);
		ff_AddInteger("%ld", (long) gbp->num_seg);
		ff_AddString(" of ");
		ff_AddInteger("%ld", (long) ajp->asn2ffwep->total_seg);
		ff_EndPrint();
	}
}

static ValNodePtr AddKeyword(ValNodePtr key, CharPtr add)
{
	ValNodePtr vnp;
	
	for (vnp = key; vnp; vnp = vnp->next) {
		if (StringCmp((CharPtr)vnp->data.ptrvalue, add) == 0) {
			return key;
		}
	}
	vnp = ValNodeNew(NULL);
	vnp->data.ptrvalue = StringSave(add);
	key = tie_next(key, vnp);
	
	return key;
}

static Boolean CheckSpecialKeyword(Boolean is_est, Boolean is_sts, Boolean is_gss, CharPtr kwd)
{
	if (is_est == FALSE && is_sts == FALSE && is_gss == FALSE) {
		return TRUE;
	}
	if (is_est) {
		if (MatchArrayString(STS_kw_array, TOTAL_STSKW, kwd) != -1) {
			return FALSE;
		}
		if (MatchArrayString(GSS_kw_array, TOTAL_GSSKW, kwd) != -1) {
			return FALSE;
		}
		return TRUE;
	}
	if (is_sts) {
		if (MatchArrayString(EST_kw_array, TOTAL_ESTKW, kwd) != -1) {
			return FALSE;
		}
		if (MatchArrayString(GSS_kw_array, TOTAL_GSSKW, kwd) != -1) {
			return FALSE;
		}
		return TRUE;
	}
	if (is_gss) {
		if (MatchArrayString(STS_kw_array, TOTAL_STSKW, kwd) != -1) {
			return FALSE;
		}
		if (MatchArrayString(EST_kw_array, TOTAL_ESTKW, kwd) != -1) {
			return FALSE;
		}
		return TRUE;
	}
	return TRUE;
}

ValNodePtr GetKeywordLine(Asn2ffJobPtr ajp, GBEntryPtr gbp)

{
	ValNodePtr block, keyword=NULL, v, vnp;
	GBBlockPtr gbblk;
	EMBLBlockPtr ebp;
	PirBlockPtr pbp;
	PrfBlockPtr prfp;
	SPBlockPtr spbp;
	MolInfoPtr mfp;
	Boolean is_est=FALSE, is_sts=FALSE, is_gss=FALSE;
	DescrStructPtr dsp;
	DescrStructPtr dspnext;

	if ((block = GatherDescrByChoice(ajp, gbp, Seq_descr_molinfo)) != NULL) 
	{
		mfp = (MolInfoPtr) block->data.ptrvalue;
			switch (mfp->tech) {
				case MI_TECH_htc:
					keyword = ValNodeNew(NULL);
					keyword->data.ptrvalue = StringSave("HTC");
				break;
				case MI_TECH_htgs_0:
					keyword = ValNodeNew(NULL);
					keyword->data.ptrvalue = StringSave("HTG");
					v = ValNodeNew(keyword);
					v->data.ptrvalue = StringSave("HTGS_PHASE0");
				break;
				case MI_TECH_htgs_1:
					keyword = ValNodeNew(NULL);
					keyword->data.ptrvalue = StringSave("HTG");
					v = ValNodeNew(keyword);
					v->data.ptrvalue = StringSave("HTGS_PHASE1");
				break;
				case MI_TECH_htgs_2:
					keyword = ValNodeNew(NULL);
					keyword->data.ptrvalue = StringSave("HTG");
					v = ValNodeNew(keyword);
					v->data.ptrvalue = StringSave("HTGS_PHASE2");
				break;
				case MI_TECH_htgs_3:
					keyword = ValNodeNew(NULL);
					keyword->data.ptrvalue = StringSave("HTG");
				break;
				case MI_TECH_est:
					is_est = TRUE;
					keyword = ValNodeNew(NULL);
					keyword->data.ptrvalue = StringSave("EST");
				break;
				case MI_TECH_sts:
					is_sts = TRUE;
					keyword = ValNodeNew(NULL);
					keyword->data.ptrvalue = StringSave("STS");
				break;
				case MI_TECH_survey:
					is_gss = TRUE;
					keyword = ValNodeNew(NULL);
					keyword->data.ptrvalue = StringSave("GSS");
				break;
				case MI_TECH_fli_cdna:
					keyword = ValNodeNew(NULL);
					keyword->data.ptrvalue = StringSave("FLI_CDNA");
				break;
				default:
					break;
			}
	}

	if ((block = GatherDescrByChoice(ajp, gbp, Seq_descr_genbank)) != NULL)
	{
		gbblk = (GBBlockPtr) block->data.ptrvalue;
		if (gbblk->keywords != NULL) {
			for (vnp = gbblk->keywords; vnp; vnp = vnp->next) {
				if (CheckSpecialKeyword(is_est, is_sts, is_gss, (CharPtr)vnp->data.ptrvalue) == TRUE) {
					keyword = AddKeyword(keyword, (CharPtr)vnp->data.ptrvalue);
				}
			}
			return keyword;
		} else {
			if (gbp->descr) {
				for(dsp = gbp->descr; dsp != NULL; dsp = dspnext)
				{
					dspnext = dsp->next;
					MemFree(dsp);
				}
				gbp->descr = NULL;
			}
		}
	}
	if ((block = GatherDescrByChoice(ajp, gbp, Seq_descr_embl)) != NULL)
	{
		ebp = (EMBLBlockPtr) block->data.ptrvalue;
		if (ebp->keywords != NULL) {
			for (vnp = ebp->keywords; vnp; vnp = vnp->next) {
				if (CheckSpecialKeyword(is_est, is_sts, is_gss, (CharPtr)vnp->data.ptrvalue) == TRUE) {
					keyword = AddKeyword(keyword, (CharPtr)vnp->data.ptrvalue);
				}
			}
			return keyword;
		} else {
			if (gbp->descr) {
				for(dsp = gbp->descr; dsp != NULL; dsp = dspnext)
				{
					dspnext = dsp->next;
					MemFree(dsp);
				}
				gbp->descr = NULL;
			}
		}
	} 
	if ((block = GatherDescrByChoice(ajp, gbp, Seq_descr_pir)) != NULL)
	{
		pbp = (PirBlockPtr) block->data.ptrvalue;
		if (pbp->keywords != NULL) {
			for (vnp = pbp->keywords; vnp; vnp = vnp->next) {
				keyword = AddKeyword(keyword, (CharPtr)vnp->data.ptrvalue);
			}
			return keyword;
		} else {
			if (gbp->descr) {
				for(dsp = gbp->descr; dsp != NULL; dsp = dspnext)
				{
					dspnext = dsp->next;
					MemFree(dsp);
				}
				gbp->descr = NULL;
			}
		}
	}
	if ((block = GatherDescrByChoice(ajp, gbp, Seq_descr_prf)) != NULL) 
	{
		prfp = (PrfBlockPtr) block->data.ptrvalue;
		if (prfp->keywords != NULL) {
			for (vnp = prfp->keywords; vnp; vnp = vnp->next) {
				keyword = AddKeyword(keyword, (CharPtr)vnp->data.ptrvalue);
			}
			return keyword;
		} else {
			if (gbp->descr) {
				for(dsp = gbp->descr; dsp != NULL; dsp = dspnext)
				{
					dspnext = dsp->next;
					MemFree(dsp);
				}
				gbp->descr = NULL;
			}
		}
	}
	if ((block = GatherDescrByChoice(ajp, gbp, Seq_descr_sp)) != NULL) 
	{
		spbp = (SPBlockPtr) block->data.ptrvalue;
		if (spbp->keywords != NULL) {
			for (vnp = spbp->keywords; vnp; vnp = vnp->next) {
				keyword = AddKeyword(keyword, (CharPtr)vnp->data.ptrvalue);
			}
			return keyword;
		} else {
			if (gbp->descr) {
				for(dsp = gbp->descr; dsp != NULL; dsp = dspnext)
				{
					dspnext = dsp->next;
					MemFree(dsp);
				}
				gbp->descr = NULL;
			}
		}
	}
	return keyword;

}	/* GetKeywordLine */


void PrintKeywordLine(Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	Boolean line_return = FALSE;
	Boolean first = TRUE;
	CharPtr string;
	Int2 tab_length=12;
	ValNodePtr keyword, vnp;

	gbp->descr = NULL;
	keyword = GetKeywordLine(ajp, gbp);

	if (ajp->format == EMBL_FMT || ajp->format == PSEUDOEMBL_FMT ||
			ajp->format == EMBLPEPT_FMT) {
		ff_StartPrint(5, 5, ASN2FF_EMBL_MAX, "KW");
	} else {
		ff_StartPrint(0, tab_length, ASN2FF_GB_MAX, NULL);
		ff_AddString("KEYWORDS");
		TabToColumn((Int2)(tab_length+1));
	}
	if (keyword != NULL) {	/* the next line initializes the length */
		for (vnp=keyword; vnp != NULL; vnp=vnp->next) {
			string = (CharPtr)vnp->data.ptrvalue;
			if (first == TRUE) {
				first = FALSE;
			} else {
				if (line_return)
					NewContLine();
			}
	
			ff_AddString(string);
			if (vnp->next != NULL) {
				ff_AddChar(';');
				ff_AddChar(' ');
			}
		}
		ValNodeFreeData(keyword);
	} else if (gbp->descr) {
		MemFree(gbp->descr);
		gbp->descr = NULL;
	}
	ff_AddChar('.');
	ff_EndPrint();
	

}	/* PrintKeywordLine */

void PrintDefinitionLine(Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	if (ajp->format == EMBL_FMT || ajp->format == PSEUDOEMBL_FMT ||
				ajp->format == EMBLPEPT_FMT) {
		ff_StartPrint(5, 5, ASN2FF_EMBL_MAX, "DE");
	} else {
		ff_StartPrint(0, 12, ASN2FF_GB_MAX, NULL);
		ff_AddString("DEFINITION");
		TabToColumn(13);
	}
	ff_AddString(gbp->defline);
	ff_EndPrint();
	return;
}

void GetDefinitionLine(Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	CharPtr string, string_start, title=NULL;
	ValNodePtr vnp = NULL;
	MolInfoPtr mfp;
	CharPtr buf;
	Int2 buflen = 1001;
	ItemInfoPtr iip;
	DescrStructPtr dsp = NULL;
	Uint1 tech = 0;
	
	buf = (CharPtr)MemNew(buflen+1);
	gbp->descr = NULL;
/*  deflines for htg sequences */
	vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_molinfo);
	if (vnp != NULL) {
		mfp = (MolInfoPtr)vnp->data.ptrvalue;
		if (mfp) {
			tech = mfp->tech;
		}
	}
	if (gbp && gbp->descr) {
		gbp->descr = (DescrStructPtr)MemFree(gbp->descr);
		gbp->descr = NULL;
	}
	
	iip = (ItemInfoPtr)MemNew(sizeof(ItemInfo));
	CreateDefLine(iip, gbp->bsp, buf, buflen, tech, NULL, NULL);
	if (iip != NULL) {
		dsp = (DescrStructPtr)MemNew(sizeof(DescrStruct));
		dsp->entityID = iip->entityID;
		dsp->itemID = iip->itemID;
		dsp->itemtype = iip->itemtype;
	}
	MemFree(iip);
	gbp->descr = dsp;
	title = buf;
	string_start = string = CheckEndPunctuation(title, '.');

	while (*string != '\0')
	{
		if (*string == '\"')
			*string = '\'';
		string++;
	}

	gbp->defline = StringSave(string_start);
	MemFree(string_start);
	MemFree(buf);
}

void PrintOriginLine(Asn2ffJobPtr ajp, GBEntryPtr gbp)

{
	Char buffer[68];
	CharPtr origin=NULL;
	GBBlockPtr gb;
	Int2 length=0;
	ValNodePtr vnp=NULL;

	gbp->descr = NULL;	
	ff_StartPrint(0, 12, ASN2FF_GB_MAX, NULL);
	ff_AddString("ORIGIN");
	TabToColumn(13);
	if ((vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_genbank)) != NULL)
	{
		gb = (GBBlockPtr) vnp->data.ptrvalue;
		if (gb)
		{
			if (gb->origin && (length=StringLen(gb->origin)) > 0)
			{ /*???? What if gb->origin is longer than 68 chars. */
				StringNCpy(buffer, gb->origin, 66);
				if (length < 66)
					buffer[length] = '\0';
				else
					buffer[66] = '\0';
				origin = CheckEndPunctuation(buffer, '.');
				ff_AddString(origin);
			}
			if (length > 66)
				ErrPostStr(SEV_WARNING, ERR_ENTRY_OriginTooLong, "");
		}
	}
	if (origin != NULL)
		MemFree(origin);
	ff_EndPrint();

}
static void print_source(Asn2ffJobPtr ajp, CharPtr source, OrgRefPtr orp)
{
	CharPtr		newsource, s;
	Boolean 	has_point = FALSE;
	ValNodePtr 	v;

	ff_StartPrint(0, 12, ASN2FF_GB_MAX, NULL);
	ff_AddString("SOURCE");
	TabToColumn(13);
	if (source) {
		newsource = CheckEndPunctuation(source, '.');
		ff_AddString(newsource);
		MemFree(source);
		MemFree(newsource);
	} else if (orp) {
		source = orp->common?orp->common:orp->taxname;
		ff_AddString(source);
		if (orp->mod == NULL && source != NULL) {
			if (*(source + StringLen(source) -1) == '.')
				has_point = TRUE;
		}
		for (v = orp->mod; v; v = v->next) {
			has_point = FALSE;
			s = (CharPtr) (v->data.ptrvalue);
			if (*(s + StringLen(s) -1) == '.')
				has_point = TRUE;
			ff_AddString(" ");
			ff_AddString(s);
			
		}
		if (!has_point)
			ff_AddChar('.');
	} else {
		ff_AddString("Unknown.");
		if (ajp->error_msgs == TRUE)
			ErrPostStr(SEV_WARNING, ERR_ENTRY_No_source_line, "");
	}
	ff_EndPrint();
}

/***************************************************************************
*PrintGBSourceLine
*
*	"PrintGBSourceLine" to print the source ONLY line for 
*	genbank and genpept FlatFiles. (modified from PrintSourceLine)
*
****************************************************************************/
void PrintGBSourceLine (Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	CharPtr 		source=NULL;
	GBBlockPtr 		gb=NULL;
	OrgRefPtr 		orp=NULL;
	BioSourcePtr 	biosp;
	ValNodePtr 		vnp=NULL;
	SeqFeatPtr 		sfp;
	SortStructPtr 	p;

	
	if (gbp == NULL) {
		return;
	}
	gbp->descr = NULL;	
	if ((vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_genbank)) != NULL) {
		gb = (GBBlockPtr) vnp->data.ptrvalue;
		if (gb)
			source = GetGBSourceLine(gb);
	}
	if (source) {
		print_source(ajp, source, NULL);
		return;
	}
	if (gbp->descr) {
		gbp->descr = (DescrStructPtr)MemFree(gbp->descr);
	}
	if ((vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_source)) != NULL) {
		biosp = (BioSourcePtr) vnp->data.ptrvalue;
		if (biosp != NULL) {
			orp = biosp->org;
		}
	} else if ((vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_org)) != NULL) {
		orp = (OrgRefPtr) vnp->data.ptrvalue;
	} else if (gbp->feat && gbp->feat->sfpOrgsize != 0) {
		p = gbp->feat->Orglist;
		if ((sfp = p->sfp) == NULL) {
		GatherItemWithLock(p->entityID,
			p->itemID, p->itemtype, &sfp, find_item);
		}
		if (sfp != NULL) {
			orp = (OrgRefPtr) sfp->data.value.ptrvalue;
		}
	}
	print_source(ajp, NULL, orp);
	return;
}

static void print_organism(Asn2ffJobPtr ajp, GBEntryPtr gbp, OrgRefPtr orp, CharPtr lineage)
{
	DbtagPtr dbp;
	Int4	id = -1;
	CharPtr organelle, taxonomy=NULL;

	if (orp) {
		if(orp->common && !orp->taxname)
			orp->taxname = TaxNameFromCommon(orp->common);
		if (lineage == NULL && orp->orgname) {
			lineage = orp->orgname->lineage;
		}
	}
	organelle = FlatOrganelle(ajp, gbp);
	ff_StartPrint(2, 12, ASN2FF_GB_MAX, NULL);
	ff_AddString("ORGANISM");
	TabToColumn(13);
	if (orp && orp->taxname) {
		if (organelle) {
			ff_AddString(organelle);
		}
		if (orp->db != NULL) {
			dbp = (DbtagPtr) (orp->db)->data.ptrvalue;
			if (StringCmp(dbp->db, "taxon") == 0)
				id = dbp->tag->id;
		}
		www_organism(orp->taxname, id);
	} else {
		ff_AddString("Unknown.");
	}
	MemFree(organelle);
	ff_EndPrint();

	ff_StartPrint(12, 12, ASN2FF_GB_MAX, NULL);
	if (lineage) {
		taxonomy = CheckEndPunctuation(lineage, '.');
		ff_AddString(taxonomy);
		MemFree(taxonomy);
	} else {
		ff_AddString("Unclassified.");
	}
	ff_EndPrint();
}
static void print_taxinfo(Asn2ffJobPtr ajp, GBEntryPtr gbp, OrgRefPtr orp, CharPtr lineage, Boolean is_mito)
{
	DbtagPtr dbp;
	Int4	id = -1, gcode=1;
	CharPtr organelle, taxonomy=NULL;
	static Char tmp[3];

	if (orp) {
		if(orp->common && !orp->taxname)
			orp->taxname = TaxNameFromCommon(orp->common);
		if (lineage == NULL && orp->orgname) {
			lineage = orp->orgname->lineage;
		}
	}
	organelle = FlatOrganelle(ajp, gbp);
	ff_StartPrint(2, 12, ASN2FF_GB_MAX, NULL);
	if (orp && orp->taxname) {
		if (organelle) {
			ff_AddString(organelle);
		}
		if (orp->db != NULL) {
			dbp = (DbtagPtr) (orp->db)->data.ptrvalue;
			if (StringCmp(dbp->db, "taxon") == 0)
				id = dbp->tag->id;
		}
	/*	ff_AddString("<BR>Taxonomy id: ");*/
		ff_AddString("<BR>Organism: ");
		www_taxid(orp->taxname, id);
	} else {
		ff_AddString("Unknown.");
	}
	MemFree(organelle);
	ff_EndPrint();

	if (orp && orp->orgname) {
		ff_StartPrint(12, 12, ASN2FF_GB_MAX, NULL);
		ff_AddString("<BR>Genetic Code: ");
		if (StringICmp(organelle, "Mitochondrion") == 0 || is_mito == TRUE) {
			gcode = orp->orgname->mgcode;
		} else {
			gcode = orp->orgname->gcode;
		}
		sprintf(tmp, "%d", gcode);
		www_gcode(tmp);
		ff_EndPrint();
	}
	ff_StartPrint(12, 12, ASN2FF_GB_MAX, NULL);
	if (lineage) {
		taxonomy = CheckEndPunctuation(lineage, '.');
		ff_AddString("<BR>Lineage: ");
		ff_AddString(taxonomy);
		MemFree(taxonomy);
	} else {
		ff_AddString("Unclassified.");
	}
		ff_AddString("<BR>");
	ff_EndPrint();
}

/***************************************************************************
*PrintGBOrganismLine
*
*	"PrintGBOrganismLine" to print the ONLY organism field for 
*	genbank and genpept FlatFiles. (modified from PrintSourceLine)
*
****************************************************************************/
void PrintGBOrganismLine (Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	CharPtr 		lineage = NULL;
	GBBlockPtr 		gb=NULL;
	OrgRefPtr 		orp=NULL;
	BioSourcePtr 	biosp;
	ValNodePtr		vnp=NULL;
	SeqFeatPtr 		sfp;
	SortStructPtr 	p;
	DescrStructPtr 	dsp;

	
	if (gbp == NULL) {
		return;
	}
	gbp->descr = NULL;
/* Find Biosource with focus 
	v=GatherDescrListByChoice(ajp, gbp, Seq_descr_source);
	for (tvnp=v; tvnp; tvnp=tvnp->next) {
		dsp = (DescrStructPtr) tvnp->data.ptrvalue;
		vnp = dsp->vnp;
		biosp = (BioSourcePtr) vnp->data.ptrvalue;
		if (biosp->is_focus == TRUE) {
			orp = biosp->org;
			gbp->descr = dsp; 
			break;
		}
	}
	ValNodeFreeData(v);
	if (orp == NULL && gbp->feat && gbp->feat->biosrcsize != 0) { 
		p = gbp->feat->Biosrclist;
		for (i = 0; i < gbp->feat->biosrcsize; i++, p++) {
			if ((sfp = p->sfp) == NULL) {
				GatherItemWithLock(p->entityID,
					p->itemID, p->itemtype, &sfp, find_item);
			}
			if (sfp != NULL) {
				biosp = (BioSourcePtr) sfp->data.value.ptrvalue;
				if (biosp->is_focus == TRUE) {
					orp = biosp->org;
					dsp = MemNew(sizeof(DescrStruct));
					gbp->descr = dsp;
					dsp->entityID = p->entityID;
					dsp->itemID = p->itemID;
					dsp->itemtype = p->itemtype;
					break;
				}
			}
		}
	}
	if (orp != NULL) { 
		if (orp->orgname) {
			lineage = orp->orgname->lineage;
		}
		print_organism(ajp, gbp, orp, lineage);
		return;
	}
*/
/* BioSource descr*/
	if ((vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_source)) != NULL) 
	{
		biosp = (BioSourcePtr)vnp->data.ptrvalue;
		orp = (OrgRefPtr) biosp->org;
		if (orp && orp->orgname) {
			lineage = orp->orgname->lineage;
		}
	} 
/* try to find lineage in GenBank block */
	if (lineage == NULL) {
		dsp = gbp->descr;
		gbp->descr = NULL;
		if ((vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_genbank)) != NULL) {
			gb = (GBBlockPtr) vnp->data.ptrvalue;
			if (gb)
				lineage = gb->taxonomy;
		}
		gbp->descr = (DescrStructPtr)MemFree(gbp->descr);
		gbp->descr = dsp;  /* keep Seq_descr_source dsp for sequin */
	}
	if (orp) {
		print_organism(ajp, gbp, orp, lineage);
		return;
	}
/* Organism descr*/
	if ((vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_org)) != NULL) {
		orp = (OrgRefPtr) vnp->data.ptrvalue;
		print_organism(ajp, gbp, orp, lineage);
		return;
	}
/* OrgRef feature */
	gbp->descr = (DescrStructPtr)MemFree(gbp->descr);
	if (gbp->feat && gbp->feat->sfpOrgsize != 0) {
		p = gbp->feat->Orglist;
		if ((sfp = p->sfp) == NULL) {
		GatherItemWithLock(p->entityID,
			p->itemID, p->itemtype, &sfp, find_item);
		}
		if (sfp != NULL) {
			orp = (OrgRefPtr) sfp->data.value.ptrvalue;
			dsp = (DescrStructPtr)MemNew(sizeof(DescrStruct));
			gbp->descr = dsp;
			dsp->entityID = p->entityID;
			dsp->itemID = p->itemID;
			dsp->itemtype = p->itemtype;
		}
	}
	print_organism(ajp, gbp, orp, lineage);
	return;
}

/***************************************************************************
*PrintTaxonomy
*
*	"PrintTaxonomy" to print taxonomy info in graphical view 
*
****************************************************************************/
void PrintTaxonomy (Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	CharPtr 		lineage = NULL;
	GBBlockPtr 		gb=NULL;
	OrgRefPtr 		orp=NULL;
	BioSourcePtr 	biosp;
	ValNodePtr		vnp=NULL;
	SeqFeatPtr 		sfp;
	SortStructPtr 	p;
	DescrStructPtr 	dsp;
	Boolean 		is_mito = FALSE;

	
	if (gbp == NULL) {
		return;
	}
	gbp->descr = NULL;
/* BioSource descr*/
	if ((vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_source)) != NULL) 
	{
		biosp = (BioSourcePtr)vnp->data.ptrvalue;
		if (biosp->genome == 5 || biosp->genome == 4) is_mito = TRUE;
		orp = (OrgRefPtr) biosp->org;
		if (orp && orp->orgname) {
			lineage = orp->orgname->lineage;
		}
	}
/* try to find lineage in GenBank block */
	if (lineage == NULL) {
		dsp = gbp->descr;
		gbp->descr = NULL;
		if ((vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_genbank)) != NULL) {
			gb = (GBBlockPtr) vnp->data.ptrvalue;
			if (gb)
				lineage = gb->taxonomy;
		}
		gbp->descr = (DescrStructPtr)MemFree(gbp->descr);
		gbp->descr = dsp;  /* keep Seq_descr_source dsp for sequin */
	}
	if (orp) {
		print_taxinfo(ajp, gbp, orp, lineage, is_mito);
		return;
	}
/* Organism descr*/
	if ((vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_org)) != NULL) {
		orp = (OrgRefPtr) vnp->data.ptrvalue;
		print_taxinfo(ajp, gbp, orp, lineage, is_mito);
		return;
	}
/* OrgRef feature */
	gbp->descr = (DescrStructPtr)MemFree(gbp->descr);
	if (gbp->feat && gbp->feat->sfpOrgsize != 0) {
		p = gbp->feat->Orglist;
		if ((sfp = p->sfp) == NULL) {
		GatherItemWithLock(p->entityID,
			p->itemID, p->itemtype, &sfp, find_item);
		}
		if (sfp != NULL) {
			orp = (OrgRefPtr) sfp->data.value.ptrvalue;
			dsp = (DescrStructPtr)MemNew(sizeof(DescrStruct));
			gbp->descr = dsp;
			dsp->entityID = p->entityID;
			dsp->itemID = p->itemID;
			dsp->itemtype = p->itemtype;
		}
	}
	print_taxinfo(ajp, gbp, orp, lineage, is_mito);
	return;
}

/***************************************************************************
*PrintOrganismLine
*
*	"PrintOrganismLine" to print the source and organism entries for 
*	EMBL FlatFiles.
*
****************************************************************************/

void PrintOrganismLine (Asn2ffJobPtr ajp, GBEntryPtr gbp)

{
	ValNodePtr vnp=NULL;
	OrgRefPtr orp=NULL, orp1=NULL;
	CharPtr organelle, taxonomy=NULL, lineage = NULL;
	BioSourcePtr biosp = NULL;
	GBBlockPtr gb=NULL;
	DescrStructPtr dsp;
	ValNodePtr tvnp;
	SeqFeatPtr	sfp;
	SortStructPtr p;

	if (gbp == NULL) {
		return;
	}	
/* new first */
	gbp->descr = NULL;	
	if ((tvnp=GatherDescrListByChoice(ajp, gbp, Seq_descr_source)) != NULL) {
		dsp = (DescrStructPtr) tvnp->data.ptrvalue;
		vnp = dsp->vnp;
		biosp = (BioSourcePtr) vnp->data.ptrvalue;
		orp = (OrgRefPtr) biosp->org;
		if (tvnp->next != NULL) {
			dsp = (DescrStructPtr) tvnp->next->data.ptrvalue;
			vnp = dsp->vnp;
			biosp = (BioSourcePtr) vnp->data.ptrvalue;
			orp1 = (OrgRefPtr) biosp->org;
		}
		ValNodeFreeData(tvnp);
	}
	if (orp && orp->orgname) {
		lineage = orp->orgname->lineage;
	}
	if (orp == NULL) {
		if ((tvnp=GatherDescrListByChoice(ajp, gbp, Seq_descr_org)) != NULL) {
			dsp = (DescrStructPtr) tvnp->data.ptrvalue;
			vnp = dsp->vnp;
			orp = (OrgRefPtr) vnp->data.ptrvalue;
			if (tvnp->next != NULL) {
				dsp = (DescrStructPtr) tvnp->next->data.ptrvalue;
				vnp = dsp->vnp;
				orp1 = (OrgRefPtr) (vnp->data.ptrvalue);
			}
			ValNodeFreeData(tvnp);
		} else if (gbp->feat && gbp->feat->sfpOrgsize != 0) {
			p = gbp->feat->Orglist;	/* gbp->feat->Orglist[0] */
			if ((sfp = p->sfp) == NULL) {
				GatherItemWithLock(p->entityID,
					p->itemID, p->itemtype, &sfp, find_item);
			}
			if (sfp != NULL) {
				orp = (OrgRefPtr) sfp->data.value.ptrvalue;
			}
			p++;   /* gbp->feat->Orglist[1] */
			if ((sfp = p->sfp) == NULL) {
				GatherItemWithLock(p->entityID,
					p->itemID, p->itemtype, &sfp, find_item);
			}
			if (sfp != NULL) {
				orp1 = (OrgRefPtr) sfp->data.value.ptrvalue;
			}
		}
	}

	if (orp)
		if(orp->common && !orp->taxname)
			orp->taxname = TaxNameFromCommon(orp->common);

	ff_StartPrint(5, 5, ASN2FF_EMBL_MAX, "OS");
	if (orp && orp->taxname)
	{
		ff_AddString(orp->taxname);
		if (orp->common)
		{
			ff_AddString(" (");
			ff_AddString(orp->common);
			ff_AddChar(')');
		}
	}
	else
		ff_AddString("Unclassified.");

	ff_EndPrint();

	ff_StartPrint(5, 5, ASN2FF_EMBL_MAX, "OC");
	if (lineage == NULL) {
		if ((vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_genbank)) != NULL){
			gb = (GBBlockPtr) vnp->data.ptrvalue;
			lineage = gb->taxonomy;
		}
	}
	if (lineage) {
		taxonomy = CheckEndPunctuation(lineage, '.');
		ff_AddString(taxonomy);
		MemFree(taxonomy);
	} else {
		ff_AddString("Unclassified.");
	}
	ff_EndPrint();
	
	if (orp1) {	/* second organism */
		if (orp1 && orp1->taxname) {
			PrintXX();
			ff_StartPrint(5, 5, ASN2FF_EMBL_MAX, "OS");
			ff_AddString(orp1->taxname);
			if (orp1->common) {
				ff_AddString(" (");
				ff_AddString(orp1->common);
				ff_AddChar(')');
			}
			ff_EndPrint();
		}
	}

/* What about plasmids on the OG line???????????????*/
/* Get this info from a qual of the SourceFeat that has qual "plasmid"??*/
	organelle = FlatOrganelle(ajp, gbp);
	if (organelle) {
		PrintXX();
		ff_StartPrint(5, 5, ASN2FF_EMBL_MAX, "OG");
		ff_AddString(organelle);
		ff_EndPrint();
		MemFree(organelle);
	}

}	/* PrintOrganismLine */



/****************************************************************************
*GetPDBSourceLine
*
*	Gets the source from the PDBBlock.
*
****************************************************************************/

CharPtr GetPDBSourceLine (PdbBlockPtr pdb)

{
	CharPtr source = NULL;
	ValNodePtr vnp;

	if(pdb && pdb->source)
	{
		vnp = pdb->source;
		source = StringSave((CharPtr)vnp->data.ptrvalue);
	}

	return source;
}

/***********************************************************************
*       This function prints out a block of the sequence (at most
*       of size SEQ_BLK_SIZE).
*       After the last sequence block, the terminator is printed also.
***********************************************************************/
 
void PrintSeqBlkEx (Asn2ffJobPtr ajp, GBEntryPtr gbp)
 
{
        Int4 start, stop, index=ajp->pap_index;
        Uint1 last=ajp->pap_last;
		DescrStructPtr dsp;

		dsp = (DescrStructPtr)MemNew(sizeof(DescrStruct));
		gbp->descr = dsp;
		dsp->entityID = gbp->entityID;
		dsp->itemID = gbp->itemID;
		dsp->itemtype = gbp->itemtype;
        if (index == 0) {
			if (ajp->slp != NULL) {
				start = SeqLocStart(ajp->slp);
			} else {
				start = 0;
            }    
        } else {
			if (ajp->slp != NULL) {
				start = index*SEQ_BLK_SIZE + SeqLocStart(ajp->slp);
			} else {
                start = index*SEQ_BLK_SIZE;
            }
        }
        if (last != LAST) {
			if (ajp->slp != NULL) {
                stop = SeqLocStart(ajp->slp) + (index+1)*SEQ_BLK_SIZE - 1;
            } else {
                stop = (index+1)*SEQ_BLK_SIZE - 1;
            }
        } else {
			if (ajp->slp != NULL) {
				stop = SeqLocStart(ajp->slp) + SeqLocLen(ajp->slp);
            } else {
                stop = -1;
            }
        }
        if (ajp->format == EMBLPEPT_FMT) {
        	PrintEPSequence(ajp, gbp, start, stop);
        } else {
        	PrintSequence(ajp, gbp, start, stop);
        }
        if (last ==  LAST)
                PrintTerminator();
}

void PrintSeqBlk (Asn2ffJobPtr ajp, GBEntryPtr gbp)
 
{
        Int4 start, stop, index=ajp->pap_index;
        Uint1 last=ajp->pap_last;
		DescrStructPtr dsp;

		dsp = (DescrStructPtr)MemNew(sizeof(DescrStruct));
		gbp->descr = dsp;
		dsp->entityID = gbp->entityID;
		dsp->itemID = gbp->itemID;
		dsp->itemtype = gbp->itemtype;
        if (index == 0) {
			if (ajp->slp != NULL) {
				start = SeqLocStart(ajp->slp);
			} else {
				start = 0;
            }    
        } else {
                start = index*SEQ_BLK_SIZE;
                if (ajp->slp) {
					start = index*SEQ_BLK_SIZE + SeqLocStart(ajp->slp);
               }
        }
        if (last != LAST) {
                stop = (index+1)*SEQ_BLK_SIZE - 1;
                if (ajp->slp) {
					stop = (index+1)*SEQ_BLK_SIZE - 1 + SeqLocStart(ajp->slp);
               }
        } else {
			if (ajp->slp != NULL) {
				stop = SeqLocStart(ajp->slp) + SeqLocLen(ajp->slp) - 1;
            } else {
                stop = -1;
            }
        }
        if (ajp->format == EMBLPEPT_FMT) {
        	PrintEPSequence(ajp, gbp, start, stop);
        } else {
        	PrintSequence(ajp, gbp, start, stop);
        }
        if (last ==  LAST)
                PrintTerminator();

	MemFree(dsp);
}

void PrintPubsByNumber (Asn2ffJobPtr ajp, GBEntryPtr gbp)

{
	PubStructPtr psp;
	ValNodePtr vnp;
	Int4 i, index = ajp->pap_index;

	for (vnp=gbp->Pub, i=0; vnp && i < index; vnp=vnp->next, i++);
	if (vnp) {
		psp = (PubStructPtr)vnp->data.ptrvalue;
		if (ajp->format == EMBL_FMT || ajp->format == PSEUDOEMBL_FMT ||
			ajp->format == EMBLPEPT_FMT) {
			EMBL_PrintPubs(ajp, gbp, psp);
		} else {
			if (ajp->mode == PARTIAL_MODE && psp->choice == PUB_Sub) {
				return;
			} else if (ajp->format == GRAPHIK_FMT) {
				GR_PrintPubs(ajp, gbp, psp);
			} else {
				GB_PrintPubs(ajp, gbp, psp);
			}
		}
	}
}
void PrintFeatHeader (Asn2ffJobPtr ajp, GBEntryPtr gbp)

{
	gbp->descr = NULL;
	if (ajp->format == EMBL_FMT || ajp->format == PSEUDOEMBL_FMT ||
		ajp->format == EMBLPEPT_FMT) {
		PrintXX();
		ff_StartPrint( 5, 0, ASN2FF_EMBL_MAX, "FH");
		ff_AddString("Key");
		TabToColumn(22);
		ff_AddString("Location/Qualifiers");
		NewContLine();
	} else {
		ff_StartPrint(0, 0, ASN2FF_GB_MAX, NULL);
		ff_AddString("FEATURES");
		TabToColumn(22);
		ff_AddString("Location/Qualifiers");
	}
	ff_EndPrint();
}


/**************************************************************************
*void PrintTerminator ()
*
*	Prints the double slash (//) at the end of an entry.
**************************************************************************/

void PrintTerminator (void)

{
	ff_StartPrint(0, 0, 0, NULL);
	ff_AddChar( '/');
	ff_AddChar('/');
	ff_EndPrint();
}

/*************************************************************************
*	Prints out the cross-refs from the EMBL block, in the descriptor.
*	Used ONLY to make EMBL output.
**************************************************************************/

void PrintXrefLine (Asn2ffJobPtr ajp, GBEntryPtr gbp)

{
	Boolean done_once=FALSE;
	Char buffer[20], buffer1[20], buffer2[20];
	CharPtr name, string;
	EMBLBlockPtr eb=NULL;
	EMBLXrefPtr xref=NULL;
	ObjectIdPtr oip;
	ValNodePtr descr=NULL, id;

	gbp->descr = NULL;	
	if ((descr=GatherDescrByChoice(ajp, gbp, Seq_descr_embl)) != NULL) 
	{
		eb = (EMBLBlockPtr) descr->data.ptrvalue;
		for (xref=eb->xref; xref; xref=xref->next)
		{
			name=NULL;
			if (xref->_class) {
				if (xref->_class == 5)
					StringCpy(buffer, "SWISS-PROT");
				else if (xref->_class == 8)
					StringCpy(buffer, "EPD");
				else if (xref->_class == 10)
					StringCpy(buffer, "TFD");
				else if (xref->_class == 11)
					StringCpy(buffer, "FLYBASE");
				name = &(buffer[0]);
			}
			else if (xref->name)
				name = xref->name;
			if (name && xref->id)
			{
				id=xref->id;
			
				oip = (ObjectIdPtr)id->data.ptrvalue;
				if (oip->str)
					StringCpy(buffer1, oip->str);
				else if (oip->id)
					sprintf(buffer1, "%ld", (long) (oip->id));
				id = id->next;
				if (id)
				{
					oip = (ObjectIdPtr)id->data.ptrvalue;
					if (oip->str)
						StringCpy(buffer2, oip->str);
					else if (oip->id)
						sprintf(buffer2, "%ld", (long) (oip->id));
				}
				if (done_once == FALSE) {
					PrintXX();
					ff_StartPrint(5, 5, ASN2FF_EMBL_MAX, "DR");
					done_once=TRUE;
				} else {
					NewContLine();
				}
				ff_AddString(name);
				ff_AddString("; ");
				ff_AddString(buffer1);
				ff_AddString("; ");
				string = CheckEndPunctuation(buffer2, '.');
				ff_AddString(string);
				string = (CharPtr)MemFree(string);
			}
		}
	}
	if (done_once)
	{
		ff_EndPrint();
	/*	PrintXX();*/
	}
}
/****************************************************************************
*
*	"PrintBaseCount" counts and prints the number of a, c, g, t, and 
*	other in a sequence.
*
****************************************************************************/

void PrintBaseCount (Asn2ffJobPtr ajp, GBEntryPtr gbp)

{
	CharPtr buffer;
	Int4 base_count[5], total=0;
	SeqPortPtr spp = NULL;
	Uint1 residue;
	DescrStructPtr dsp;
	BioseqPtr bsp = gbp->bsp;
	
	dsp = (DescrStructPtr) MemNew(sizeof(DescrStruct));
	gbp->descr = dsp;
	dsp->entityID = 0;
	dsp->itemID = 0;
	dsp->itemtype = 0;		
	if (gbp->base_cnt_line)
	{	/* Been there (at least once), done that.	*/
		buffer = gbp->base_cnt_line;
	} else {
		base_count[0]=0;
		base_count[1]=0;
		base_count[2]=0;
		base_count[3]=0;
		base_count[4]=0;

		if (ajp->slp) {
			spp = SeqPortNewByLoc(ajp->slp, Seq_code_iupacna);
		} else {
			spp = SeqPortNew(gbp->bsp, 0, -1, 0, Seq_code_iupacna);
		}
		if (bsp->repr == Seq_repr_delta || bsp->repr == Seq_repr_virtual) {
			SeqPortSet_do_virtual(spp, TRUE);
		}
		while ((residue=SeqPortGetResidue(spp)) != SEQPORT_EOF)
		{
			if ( !IS_residue(residue) && residue != INVALID_RESIDUE )
				continue;
	
			total++;
			switch (residue) {
				case 'A':
					base_count[0]++;
					break;
				case 'C':
					base_count[1]++;
					break;
				case 'G':
					base_count[2]++;
					break;
				case 'T':
					base_count[3]++;
					break;
				default:
						base_count[4]++;
						break;
			}
		}
		buffer = (CharPtr) MemNew(80*sizeof(Char));
		if (ajp->format == EMBL_FMT || ajp->format == PSEUDOEMBL_FMT ||
			ajp->format == EMBLPEPT_FMT)
		{
			sprintf(buffer, 
			"%ld BP; %ld A; %ld C; %ld G; %ld T; %ld other;", 
			(long) total, (long) base_count[0], (long) base_count[1], 
			(long) base_count[2], (long) base_count[3], (long) base_count[4]);
		}
		else 	/* GENBANK format */
		{
			if (base_count[4] == 0)
			{
			sprintf(buffer, 
			"%7ld a%7ld c%7ld g%7ld t", 
			(long) base_count[0], (long) base_count[1], 
				(long) base_count[2], (long) base_count[3]);
			}
			else
			{
			sprintf(buffer, 
			"%7ld a%7ld c%7ld g%7ld t%7ld others", 
			(long) base_count[0], (long) base_count[1], 
			(long) base_count[2], (long) base_count[3], (long) base_count[4]);
			}
		}
		gbp->base_cnt_line = buffer;
		if (spp) {
			SeqPortFree(spp);
		}
	}

		if (ajp->format == EMBL_FMT || ajp->format == PSEUDOEMBL_FMT ||
			ajp->format == EMBLPEPT_FMT)
	{
		ff_StartPrint(5, 5, ASN2FF_EMBL_MAX, "SQ");
		ff_AddString("Sequence ");
		ff_AddString(buffer);
	}
	else
	{
		ff_StartPrint(0, 0, ASN2FF_GB_MAX, NULL);
		ff_AddString("BASE COUNT");
		TabToColumn(13);
		ff_AddString( buffer);
	}

	ff_EndPrint();
}	/* PrintBaseCount */
/*****************************************************************************
*
*	"PrintSequence" to get the biological sequence (in iupacna or
*	iupacaa format)	and put it in a buffer suitable for Genbank 
*	or EMBL format.
*
*	The variables "start" and "stop" allow one to read from a point 
*	not at the beginning of the sequence to a point not at the end
*	of the sequence.
*
*	Rewrite to store in a buffer and print out more at once????????
*****************************************************************************/

void PrintSequence (Asn2ffJobPtr ajp, GBEntryPtr gbp, Int4 start, Int4 stop)

{
	BioseqPtr bsp=gbp->bsp;
	Char buffer[MAX_BTP_BUF], num_buffer[10];
	CharPtr ptr = &(buffer[0]), num_ptr;
	Int4 index, inner_index, inner_stop, total=start;
	Int4 loc_start;
	SeqPortPtr spp;
	Uint1 residue;


	if ((loc_start = SeqLocStart(ajp->slp)) == -1) {
		loc_start = 0;
	}
	total = start;
	if (ajp->format == GENBANK_FMT || ajp->format == SELECT_FMT)
	{
		ff_StartPrint(0, 0, ASN2FF_GB_MAX, NULL);
		sprintf(ptr, "%9ld ", (long) (total+1 - loc_start));
		ptr += StringLen(ptr);
		CheckSeqPort(ajp, gbp, start);
		spp = gbp->spp;
		if (spp == NULL) {
          ff_AddString(ptr);
          ff_EndPrint();
		  return;
		}
		if (bsp->repr == Seq_repr_delta || bsp->repr == Seq_repr_virtual) {
			SeqPortSet_do_virtual(spp, TRUE);
		}
		if (stop == -1) {
			stop = spp->stop;
		}
		for (index=start; index<=stop; index += 10) {
			if (stop < (index+10)) {
				inner_stop = stop;
			} else {
				inner_stop = index+9;
			}
			for (inner_index=index; inner_index<=inner_stop; inner_index++) {
				if ((residue=SeqPortGetResidue(spp)) == SEQPORT_EOF) {
                    break;
               }
/*
				if (ajp->only_one) {
					if (residue == SEQPORT_VIRT) {
						*ptr = '\0';
						ff_AddString(buffer);
						NewContLine();
						MemSet(buffer, ' ', ptr - buffer);
						inner_index--;
						continue;
					}
				}
*/		
				if ( !IS_residue(residue) && residue != INVALID_RESIDUE) {
					if (residue != SEQPORT_EOF) {
						inner_index--;
						continue;
					}
					inner_index--;
					continue;
				}
				if (residue == INVALID_RESIDUE) {
					residue = (Uint1) 'X';
				}
				*ptr++ = TO_LOWER(residue);
			}
			total = inner_stop+1;
		/* Put in a space every ten, unless it's the end of a row. */
			if (ROUNDUP(total-start, 60) == (total-start)) {
				if (total != (start+1) && total != (stop+1)) {
					*ptr = '\0';
					ptr = &buffer[0];
					ff_AddString(ptr);
					NewContLine();
					sprintf(ptr, "%9ld ", (long) (total+1 - loc_start));
					ptr += StringLen(ptr);
				}
			}
			else if (ROUNDUP(total-start, 10) == total-start)
			{
				*ptr = ' '; ptr++;
			}
		}
		*ptr = '\0';
		ptr = &buffer[0];
		ff_AddString( ptr);
	}
	else if (ajp->format == GENPEPT_FMT)
	{
		total++;

		ff_StartPrint(0, 0, ASN2FF_GB_MAX, NULL);
		sprintf(ptr, "%9ld ", (long) (total - loc_start));
		ptr += StringLen(ptr);
		if (ASN2FF_IUPACAA_ONLY == TRUE)
			spp = SeqPortNew(bsp, start, stop, 0, Seq_code_iupacaa);
		else
			spp = SeqPortNew(bsp, start, stop, 0, Seq_code_ncbieaa);
		if (spp == NULL) {
          ff_AddString(ptr);
          ff_EndPrint();
		  return;
		}
		if (bsp->repr == Seq_repr_delta || bsp->repr == Seq_repr_virtual) {
			SeqPortSet_do_virtual(spp, TRUE);
		}
		while ((residue=SeqPortGetResidue(spp)) != SEQPORT_EOF)
		{
			if ( !IS_residue(residue) && residue != INVALID_RESIDUE )
				continue;
			if (residue == INVALID_RESIDUE)
				residue = (Uint1) 'X';
	
			*ptr = TO_LOWER(residue); ptr++;
			if (ROUNDUP(total - start, 10) == total - start) 
			{
				if (ROUNDUP(total - start, 60) == total - start)
				{
					if (total != (start+1) && total != (stop+1))
					{
						*ptr = '\0';
						ptr = &buffer[0];
						ff_AddString(ptr);
						NewContLine();
						num_ptr = &(num_buffer[0]);
						sprintf(num_ptr, "%9ld", (long) (total+1 - loc_start));
						while ((*ptr = *num_ptr) != '\0')
						{
							ptr++; num_ptr++;
						}
						*ptr = ' '; ptr++;
					}
				}
				else
				{
					*ptr = ' '; ptr++;
				}
			}
			total++;
		}
		*ptr = '\0';
		ptr = &buffer[0];
		ff_AddString(ptr);
		SeqPortFree(spp);
	}
	else if (ajp->format == EMBL_FMT || ajp->format == PSEUDOEMBL_FMT ||
			ajp->format == EMBLPEPT_FMT)
	{	/* numbers at far right, let line go to MAX_BTP_BUF */

		ff_StartPrint(5, 5, 0, NULL);
		CheckSeqPort(ajp, gbp, start);
		spp = gbp->spp;
		if (spp == NULL) {
          ff_AddString(ptr);
          ff_EndPrint();
		  return;
		}
		if (stop == -1)
			stop = spp->stop;
		for (index=start; index<=stop; index += 10)
		{
			if (stop < (index+10))
				inner_stop = stop;
			else
				inner_stop = index+9;
			for (inner_index=index; inner_index<=inner_stop; inner_index++)
			{
				residue=SeqPortGetResidue(spp);
				if ( !IS_residue(residue) && residue != INVALID_RESIDUE )
					continue;
				if (residue == INVALID_RESIDUE)
					residue = (Uint1) 'X';
		
				*ptr = TO_LOWER(residue); ptr++;
			}
			total = inner_index;
			if (ROUNDUP(total - start, 10) == total - start) 
			{
				if (ROUNDUP(total - start, 60) == total - start)
				{
					*ptr = '\0';
					ptr = &buffer[0];
					ff_AddString(ptr);
					TabToColumn(73);
					ff_AddInteger("%8ld", (long) (total - loc_start));
					if (ROUNDUP(total, SEQ_BLK_SIZE) != total)
						NewContLine();
				}
				else
				{
					*ptr = ' '; ptr++;
				}
			}
		}
		total = stop+1;
		if (ROUNDUP(total - start, 60) != total - start)
		{
			*ptr = '\0';
			ptr = &buffer[0];
			ff_AddString(ptr);
			TabToColumn(73);
			ff_AddInteger("%8ld", (long) (total - loc_start));
		}
	}

	ff_EndPrint();


}	/* PrintSequence */

/*****************************************************************************
*
*	"PrintEPSequence" to get the biological sequence (in iupacna or
*	iupacaa format)	and put it in a buffer suitable for Genbank 
*	or EMBL format.
*
*	The variables "start" and "stop" allow one to read from a point 
*	not at the beginning of the sequence to a point not at the end
*	of the sequence.
*
*	Rewrite to store in a buffer and print out more at once????????
*****************************************************************************/

void PrintEPSequence (Asn2ffJobPtr ajp, GBEntryPtr gbp, Int4 start, Int4 stop)

{
	BioseqPtr bsp=gbp->bsp;
	Char buffer[MAX_BTP_BUF];
	CharPtr ptr = &(buffer[0]);
	Int4 index, inner_index, inner_stop, total=start;
	SeqPortPtr spp;
	Uint1 residue;


	/* numbers at far right, let line go to MAX_BTP_BUF */

	ff_StartPrint(5, 5, 0, NULL);
	if (ASN2FF_IUPACAA_ONLY == TRUE)
		spp = SeqPortNew(bsp, start, stop, 0, Seq_code_iupacaa);
	else
		spp = SeqPortNew(bsp, start, stop, 0, Seq_code_ncbieaa);
	if (stop == -1)
		stop = spp->stop;
	for (index=start; index<=stop; index += 10)
	{
		if (stop < (index+10))
			inner_stop = stop;
		else
		inner_stop = index+9;
		for (inner_index=index; inner_index<=inner_stop; inner_index++)
		{
			residue=SeqPortGetResidue(spp);
			if ( !IS_residue(residue) && residue != INVALID_RESIDUE )
				continue;
			if (residue == INVALID_RESIDUE)
				residue = (Uint1) 'X';
	
			*ptr = TO_LOWER(residue); ptr++;
		}
		total = inner_index;
		if (ROUNDUP(total, 10) == total) 
		{
			if (ROUNDUP(total, 60) == total)
			{
				*ptr = '\0';
				ptr = &buffer[0];
				ff_AddString(ptr);
				TabToColumn(73);
				ff_AddInteger("%8ld", (long) total);
				if (ROUNDUP(total, SEQ_BLK_SIZE) != total)
					NewContLine();
			}
			else
			{
				*ptr = ' '; ptr++;
			}
		}
	}
	total = stop+1;
	if (ROUNDUP(total, 60) != total)
	{
		*ptr = '\0';
		ptr = &buffer[0];
		ff_AddString(ptr);
		TabToColumn(73);
		ff_AddInteger("%8ld", (long) total);
	}

	ff_EndPrint();

	SeqPortFree(spp);


}	/* PrintEPSequence */

void GatherItemWithLock(Uint2 entityID, Uint2 itemID, Uint2 itemtype,
                                   Pointer userdata, GatherItemProc userfunc)
{
	GatherItem(entityID, itemID, itemtype, userdata,  userfunc);
    return;
}

Boolean find_item (GatherContextPtr gcp)
{
	SeqFeatPtr sfp;
	SeqFeatPtr PNTR sfpp;


	sfpp = (SeqFeatPtr PNTR)gcp->userdata;
	switch (gcp->thistype) {
		case OBJ_SEQFEAT:
			sfp = (SeqFeatPtr) (gcp->thisitem);
			*sfpp = sfp;
		break;
		default:
		break;
	}
	return TRUE;
}

static void PrintSeqRegion (Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	SeqPortPtr spp;
	Uint1 residue;
	Char buffer[MAX_BTP_BUF];
	CharPtr ptr = &(buffer[0]);
	Int4 total;
	BioseqPtr bsp;

	if (ajp == NULL || ajp->slp == NULL) {
		return;
	}
	bsp = gbp->bsp;
	total = 0;
	ff_StartPrint(0, 0, ASN2FF_GB_MAX, NULL);
	spp = SeqPortNewByLoc(ajp->slp, Seq_code_iupacna);
	if (bsp->repr == Seq_repr_delta || bsp->repr == Seq_repr_virtual) {
		SeqPortSet_do_virtual(spp, TRUE);
	}
	while ((residue=SeqPortGetResidue(spp)) != SEQPORT_EOF) {
		if (!IS_residue(residue) && residue != INVALID_RESIDUE) {
			continue;
		}
/*
		if (ajp->only_one) {
			if (residue == SEQPORT_VIRT) {
				*ptr = '\0';
				ff_AddString(buffer);
				NewContLine();
				MemSet(buffer, ' ', ptr - buffer);
				continue;
			}
		}
*/
		if (residue == INVALID_RESIDUE) {
			residue = (Uint1) 'X';
		}
		if (ROUNDUP(total, 60) == total) {
			if (total > 0) {
				*ptr = '\0';
				ptr = &buffer[0];
				ff_AddString(ptr);
				NewContLine();
			}
			sprintf(ptr, "%9ld ", (long) (total+1));
			ptr += StringLen(ptr);
		} else if (ROUNDUP(total, 10) == total) {
			*ptr++ = ' ';
		}
		*ptr++ = TO_LOWER(residue);
		total++;
	
	}
	*ptr = '\0';
	ptr = &buffer[0];
	ff_AddString(ptr);
	SeqPortFree(spp);
	ff_EndPrint();
	PrintTerminator();
}
