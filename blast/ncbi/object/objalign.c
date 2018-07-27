/*  objalign.c
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE                          
*               National Center for Biotechnology Information
*                                                                          
*  This software/database is a "United States Government Work" under the   
*  terms of the United States Copyright Act.  It was written as part of    
*  the author's official duties as a United States Government employee and 
*  thus cannot be copyrighted.  This software/database is freely available 
*  to the public for use. The National Library of Medicine and the U.S.    
*  Government have not placed any restriction on its use or reproduction.  
*                                                                          
*  Although all reasonable efforts have been taken to ensure the accuracy  
*  and reliability of the software and data, the NLM and the U.S.          
*  Government do not and cannot warrant the performance or results that    
*  may be obtained by using this software or data. The NLM and the U.S.    
*  Government disclaim all warranties, express or implied, including       
*  warranties of performance, merchantability or fitness for any particular
*  purpose.                                                                
*                                                                          
*  Please cite the author in any work or product based on this material.   
*
* ===========================================================================
*
* File Name:  objalign.c
*
* Author:  James Ostell
*   
* Version Creation Date: 4/1/91
*
* $Revision: 6.10 $
*
* File Description:  Object manager for module NCBI-Seqalign
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 05-13-93 Schuler     All public functions are now declared LIBCALL.
*
* $Log: objalign.c,v 
 * Revision 4.1  1995/09/21  02:26:14  ostel
 * added ProgMon call
 *
 * Revision 4.0  1995/07/26  13:48:06  ostell
 * force revision to 4.0
 *
 * Revision 3.10  1995/07/25  01:00:07  ostell
 * added checks to strip asn1 spec 4.0 objects on demand
 *
 * Revision 3.9  1995/07/22  21:59:13  ostell
 * added support for ASN.1 spec 4.0
 *
 * Revision 3.8  1995/06/27  17:35:03  epstein
 * add missing return values to new Seq-align-set functions
 *
 * Revision 3.7  95/06/21  17:17:46  epstein
 * add Seq-align-set
 * 
 * Revision 3.6  95/05/15  21:22:00  ostell
 * added Log line
 * 
*
*
*
* ==========================================================================
*/
#include <objalign.h>		   /* the align interface */
#include <asnalign.h>        /* the AsnTool header */
#include <objmgr.h>          /* object manager interface */
#include <sequtil.h>

static Boolean loaded = FALSE;
static CharPtr seqaligntypelabel = "SeqAlign",
               seqhistaligntypelabel = "SeqHistAlign";

static ScorePtr InternalScoreSetAsnRead (AsnIoPtr aip, AsnTypePtr settype, AsnTypePtr elementtype);
static Boolean InternalScoreSetAsnWrite (ScorePtr sp, AsnIoPtr aip, AsnTypePtr settype, AsnTypePtr elementtype);
                       /* this is really an internal type at the moment */
NLM_EXTERN Int2 LIBCALL SeqHistAlignLabel PROTO((SeqAlignPtr sap, CharPtr buffer, Int2 buflen, Uint1 content));

/*****************************************************************************
*
*   SeqAlign ObjMgr Routines
*
*****************************************************************************/
static Int2 NEAR SeqAlignLabelContent (SeqAlignPtr sap, CharPtr buf, Int2 buflen, Uint1 labeltype, CharPtr typelabel)

{
	Char label [90];
	Char str [40];
	Int2 j;
	DenseDiagPtr ddp;
	DenseSegPtr dsp;
	StdSegPtr ssp;
	SeqIdPtr sip;
	SeqLocPtr slp;

	if (sap == NULL) return 0;

	label [0] = '\0';
	switch (sap->segtype) {
		case 1 :
			ddp = (DenseDiagPtr) sap->segs;
			if (ddp != NULL) {
				for (sip = ddp->id, j = 0; sip != NULL && j < 2; sip = sip->next, j++) {
					SeqIdWrite (sip, str, PRINTID_FASTA_SHORT, sizeof (str));
					if (j > 0) {
						StringCat (label, ",");
					}
					StringCat (label, str);
				}
				if (sip != NULL) {
					StringCat (label, ",...");
				}
			}
		  break;
		case 2 :
			dsp = (DenseSegPtr) sap->segs;
			if (dsp != NULL) {
				for (sip = dsp->ids, j = 0; sip != NULL && j < 2; sip = sip->next, j++) {
					SeqIdWrite (sip, str, PRINTID_FASTA_SHORT, sizeof (str));
					if (j > 0) {
						StringCat (label, ",");
					}
					StringCat (label, str);
				}
				if (sip != NULL) {
					StringCat (label, ",...");
				}
			}
		  break;
		case 3 :
			ssp = (StdSegPtr) sap->segs;
			if (ssp != NULL && ssp->loc != NULL) {
				for (slp = ssp->loc, j = 0; slp != NULL && j < 2; slp = slp->next, j++) {
					sip = SeqLocId (slp);
					SeqIdWrite (sip, str, PRINTID_FASTA_SHORT, sizeof (str));
					if (j > 0) {
						StringCat (label, ",");
					}
					StringCat (label, str);
				}
				if (slp != NULL) {
					StringCat (label, ",...");
				}
			}
		  break;
		default :
		  break;
	}

	return LabelCopyExtra (buf, label, buflen, NULL, NULL);
}

static Int2 NEAR CommonSeqAlignLabelFunc (SeqAlignPtr sap, CharPtr buf, Int2 buflen, Uint1 labeltype, CharPtr prefix)

{
	Int2 len, diff;
	CharPtr curr, typelabel, tmp;
	Uint1 extras = 0;
	Char tbuf[40];
	CharPtr suffix = NULL;

	if ((sap == NULL) || (buf == NULL) || (buflen < 1))
		return 0;

	buf[0] = '\0';
	curr = buf;
	len = buflen;

	tmp = StringMove(tbuf, prefix);
	typelabel = tbuf;

	if ((labeltype == OM_LABEL_TYPE) || (labeltype == OM_LABEL_BOTH))
	{
		if (labeltype == OM_LABEL_BOTH)
			suffix = ": ";
		else
			suffix = NULL;

		diff = LabelCopyExtra(curr, typelabel, buflen, NULL, suffix);
		curr += diff;
		buflen -= diff;
	}

	if ((labeltype == OM_LABEL_TYPE) || (! buflen))
		return (len - buflen);

	diff = SeqAlignLabelContent (sap, curr, buflen, labeltype, typelabel);
	buflen -= diff;

	if ((! diff) && (labeltype == OM_LABEL_CONTENT))
	{
		buflen -= LabelCopy(curr, typelabel, buflen);
	}

	return (len - buflen);
}

static Pointer LIBCALLBACK SeqAlignNewFunc (void)
{
	return (Pointer) SeqAlignNew();
}

static Pointer LIBCALLBACK SeqAlignFreeFunc (Pointer data)
{
	return (Pointer) SeqAlignFree ((SeqAlignPtr) data);
}

static Boolean LIBCALLBACK SeqAlignAsnWriteFunc (Pointer data, AsnIoPtr aip, AsnTypePtr atp)
{
	return SeqAlignAsnWrite((SeqAlignPtr)data, aip, atp);
}

static Pointer LIBCALLBACK SeqAlignAsnReadFunc (AsnIoPtr aip, AsnTypePtr atp)
{
	return (Pointer) SeqAlignAsnRead (aip, atp);
}

static Int2 LIBCALLBACK SeqAlignLabelFunc ( Pointer data, CharPtr buffer, Int2 buflen, Uint1 content)
{
	return SeqAlignLabel((SeqAlignPtr)data, buffer, buflen, content);
}

NLM_EXTERN Int2 LIBCALL SeqAlignLabel (SeqAlignPtr sap, CharPtr buffer, Int2 buflen, Uint1 content)
{
	return CommonSeqAlignLabelFunc (sap, buffer, buflen, content, seqaligntypelabel);
}


static Int2 LIBCALLBACK SeqHistAlignLabelFunc ( Pointer data, CharPtr buffer, Int2 buflen, Uint1 content)
{
	return SeqHistAlignLabel((SeqAlignPtr)data, buffer, buflen, content);
}

NLM_EXTERN Int2 LIBCALL SeqHistAlignLabel (SeqAlignPtr sap, CharPtr buffer, Int2 buflen, Uint1 content)
{
	return CommonSeqAlignLabelFunc (sap, buffer, buflen, content, seqhistaligntypelabel);
}

static Uint2 LIBCALLBACK SeqAlignSubTypeFunc (Pointer ptr)
{
	if (ptr == NULL)
		return 0;
	return (Uint2)((SeqAlignPtr)ptr)->type;
}

/*****************************************************************************
*
*   SeqAlignAsnLoad()
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqAlignAsnLoad (void)
{
    if (loaded)
        return TRUE;
    loaded = TRUE;

    if (! GeneralAsnLoad())
    {
        loaded = FALSE;
        return FALSE;
    }
    if (! SeqLocAsnLoad())
    {
        loaded = FALSE;
        return FALSE;
    }
    if (! AsnLoad())
    {
        loaded = FALSE;
        return FALSE;
    }

	ObjMgrTypeLoad(OBJ_SEQALIGN, "Seq-align", seqaligntypelabel, "Sequence Alignment",
		SEQ_ALIGN, SeqAlignNewFunc, SeqAlignAsnReadFunc, SeqAlignAsnWriteFunc,
		SeqAlignFreeFunc, SeqAlignLabelFunc, SeqAlignSubTypeFunc);

	
	ObjMgrTypeLoad(OBJ_SEQHIST_ALIGN, "Seq-align", seqhistaligntypelabel, "Sequence History Alignment",
		SEQ_ALIGN, SeqAlignNewFunc, SeqAlignAsnReadFunc, SeqAlignAsnWriteFunc,
		SeqAlignFreeFunc, SeqHistAlignLabelFunc, SeqAlignSubTypeFunc);

    return TRUE;
}

/*****************************************************************************
*
*   SeqAlign Routines
*
*****************************************************************************/
NLM_EXTERN SeqAlignIndexPtr LIBCALL SeqAlignIndexFree (SeqAlignIndexPtr saip)
{
	Boolean retval;
	VoidPtr ptr;
	SeqAlignIndexPtr tmp = NULL;

	if (saip == NULL)
		return saip;

	if (saip->freefunc != NULL)
	{
		ptr = (VoidPtr)(saip);
		retval = (*(saip->freefunc))(ptr);
		if (! retval)
			ErrPostEx(SEV_ERROR,0,0,"SeqAlignFreeFunc: saip->freefunc returned FALSE");
		return tmp;
	}

	ErrPostEx(SEV_ERROR,0,0,"SeqAlignIndexFree: saip lacking a freefunc");

	return tmp;
}

/*****************************************************************************
*
*   SeqAlignNew()
*
*****************************************************************************/
NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignNew (void)
{
    return (SeqAlignPtr)MemNew(sizeof(SeqAlign));
}
/*****************************************************************************
*
*   SeqAlignFree(sap)
*       Frees one SeqAlign and associated data
*
*****************************************************************************/
NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignFree (SeqAlignPtr sap)
{
	DenseDiagPtr ddp, ddpnext;
	StdSegPtr ssp, sspnext;
	
    if (sap == NULL)
        return (SeqAlignPtr)NULL;

    sap->saip = SeqAlignIndexFree (sap->saip);  /* free index elements */
    
    switch (sap->segtype)
    {
        case 1:                   /* dendiag */
			ddpnext = NULL;
			for (ddp = (DenseDiagPtr)(sap->segs); ddp != NULL; ddp = ddpnext)
			{
				ddpnext = ddp->next;
	            DenseDiagFree(ddp);
			}
            break;
        case 2:                   /* denseg */
            DenseSegFree((DenseSegPtr)sap->segs);
            break;
        case 3:                   /* std-seg */
			sspnext = NULL;
			for (ssp = (StdSegPtr)(sap->segs); ssp !=NULL; ssp = sspnext)
			{
				sspnext = ssp->next;
	            StdSegFree(ssp);
			}
            break;
        case 4:                   /* packseg */
            PackSegFree((PackSegPtr)sap->segs);
            break;
        case 5:                   /* disc */
            SeqAlignSetFree((SeqAlignPtr)sap->segs);
            break;
    }
    ScoreSetFree(sap->score);
	SeqLocSetFree(sap->bounds);
        SeqIdFree(sap->master);

	ObjMgrDelete(OBJ_SEQALIGN, (Pointer)sap);

	return (SeqAlignPtr)MemFree(sap);
}

/*****************************************************************************
*
*   SeqAlignAsnWrite(sap, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (SeqAlign ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqAlignAsnWrite (SeqAlignPtr sap, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    DenseDiagPtr ddp;
    StdSegPtr ssp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqAlignAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	if ((aip->spec_version == 3) &&   /* ASN3 strip new value */
		(sap != NULL) && (sap->segtype > 3))
	{
		ErrPostEx(SEV_ERROR,0,0,"ASN3: SeqAlign.segs > 3 stripped");
		return TRUE;
	}

	atp = AsnLinkType(orig, SEQ_ALIGN);   /* link local tree */
    if (atp == NULL)
        return FALSE;

	if (sap == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)sap))
        goto erret;

    av.intvalue = sap->type;
	if (! AsnWrite(aip, SEQ_ALIGN_type, &av))	  /* write the type */
        goto erret;

    if (sap->dim)                     /* default global dimensionality? */
    {
        av.intvalue = sap->dim;
        if (! AsnWrite(aip, SEQ_ALIGN_dim, &av)) goto erret;
    }

	if (sap->score != NULL)
	{
	    if (! InternalScoreSetAsnWrite(sap->score, aip, SEQ_ALIGN_score, SEQ_ALIGN_score_E))
    	    goto erret;
	}

    av.ptrvalue = (Pointer)sap->segs;
    if (! AsnWriteChoice(aip, SEQ_ALIGN_segs, (Int2)sap->segtype, &av)) goto erret;

    switch (sap->segtype)                 /* which CHOICE */
    {
        case 1:                   /* dendiag */
            if (! AsnOpenStruct(aip, SEQ_ALIGN_segs_dendiag, (Pointer)sap->segs))
                goto erret;
            ddp = (DenseDiagPtr) sap->segs;
            while (ddp != NULL)
            {
                if (! DenseDiagAsnWrite(ddp, aip, SEQ_ALIGN_segs_dendiag_E))
                    goto erret;
                ddp = ddp->next;
            }
            if (! AsnCloseStruct(aip, SEQ_ALIGN_segs_dendiag, (Pointer)sap->segs))
                goto erret;
            break;
        case 2:                   /* denseg */
            if (! DenseSegAsnWrite((DenseSegPtr)sap->segs, aip, SEQ_ALIGN_segs_denseg))
                goto erret;
            break;
        case 3:                   /* std-seg */
            if (! AsnOpenStruct(aip, SEQ_ALIGN_segs_std, (Pointer)sap->segs))
                goto erret;
            ssp = (StdSegPtr) sap->segs;
            while (ssp != NULL)
            {
                if (! StdSegAsnWrite(ssp, aip, SEQ_ALIGN_segs_std_E))
                    goto erret;
                ssp = ssp->next;
            }
            if (! AsnCloseStruct(aip, SEQ_ALIGN_segs_std, (Pointer)sap->segs))
                goto erret;
            break;
        case 4:                   /* packseg */
            if (! PackSegAsnWrite((PackSegPtr)sap->segs, aip, SEQ_ALIGN_segs_packed))
                goto erret;
            break;
        case 5:                   /* disc */
            if (! SpecialSeqAlignSetAsnWrite((SeqAlignPtr)sap->segs, aip, SEQ_ALIGN_segs_disc))
                goto erret;
            break;
    }

	if (sap->bounds != NULL)
	{
	   if (aip->spec_version == 3)    /* ASN3 strip new value */
	   {
	   	ErrPostEx(SEV_ERROR,0,0,"ASN3: SeqAlign.bounds stripped");
	   }
	   else
		if (! SeqLocSetAsnWrite(sap->bounds, aip, SEQ_ALIGN_bounds, SEQ_ALIGN_bounds_E))
			goto erret;
	}

	if (! AsnCloseStruct(aip, atp, (Pointer)sap))
        goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   SeqAlignAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldtype;
    SeqAlignPtr sap=NULL;
    DenseDiagPtr currddp = NULL, ddp;
    StdSegPtr currssp = NULL, ssp;

	if (! loaded)
	{
		if (! SeqAlignAsnLoad())
			return sap;
	}

	if (aip == NULL)
		return sap;

	if (orig == NULL)           /* SeqAlign ::= (self contained) */
		atp = AsnReadId(aip, amp, SEQ_ALIGN);
	else
		atp = AsnLinkType(orig, SEQ_ALIGN);    /* link in local tree */
    oldtype = atp;
    if (atp == NULL)
        return sap;

	sap = SeqAlignNew();
    if (sap == NULL)
        goto erret;

	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the start struct */
	atp = AsnReadId(aip, amp, atp);  /* read the type */
    if (atp == NULL)
        goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    sap->type = (Uint1) av.intvalue;
    while ((atp = AsnReadId(aip, amp, atp)) != oldtype)
    {
        if (atp == NULL)
            goto erret;
        if (atp == SEQ_ALIGN_dim)
        {
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
            sap->dim = (Int2) av.intvalue;
        }
        else if (atp == SEQ_ALIGN_score)
        {
            sap->score = InternalScoreSetAsnRead(aip, SEQ_ALIGN_score, SEQ_ALIGN_score_E);
            if (sap->score == NULL)
                goto erret;
        }
        else if (atp == SEQ_ALIGN_bounds)
        {
            sap->bounds = SeqLocSetAsnRead(aip, SEQ_ALIGN_bounds, SEQ_ALIGN_bounds_E);
            if (sap->bounds == NULL)
                goto erret;
			if (aip->spec_version == 3)    /* ASN3 strip new value */
			{
				ErrPostEx(SEV_ERROR,0,0,"ASN3: SeqAlign.bounds stripped");
				sap->bounds = SeqLocSetFree(sap->bounds);
			}
        }
        else if (atp == SEQ_ALIGN_segs_dendiag_E)
        {
            sap->segtype = 1;
            ddp = DenseDiagAsnRead(aip, atp);
            if (ddp == NULL)
                goto erret;
            if (sap->segs == NULL)
                sap->segs = (Pointer)ddp;
            else
                currddp->next = ddp;
            currddp = ddp;
        }
        else if (atp == SEQ_ALIGN_segs_denseg)
        {
            sap->segtype = 2;
            sap->segs = (Pointer) DenseSegAsnRead(aip, SEQ_ALIGN_segs_denseg);
            if (sap->segs == NULL)
                goto erret;
        }
        else if (atp == SEQ_ALIGN_segs_std_E)
        {
            sap->segtype = 3;
            ssp = StdSegAsnRead(aip, atp);
            if (ssp == NULL)
                goto erret;
            if (sap->segs == NULL)
                sap->segs = (Pointer) ssp;
            else
                currssp->next = ssp;
            currssp = ssp;
        }
        else if (atp == SEQ_ALIGN_segs_packed)
        {
            sap->segtype = 4;
            sap->segs = (Pointer) PackSegAsnRead(aip, atp);
            if (sap->segs == NULL)
                goto erret;
        }
        else if (atp == SEQ_ALIGN_segs_disc)
        {
            sap->segtype = 5;
            sap->segs = (Pointer) SpecialSeqAlignSetAsnRead(aip, atp);
            if (sap->segs == NULL)
                goto erret;
        }
		else
        {
			if (AsnReadVal(aip, atp, NULL) <= 0)
                goto erret;
        }
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end struct */
	if ((aip->spec_version == 3) &&   /* ASN3 strip new value */
		(sap->segtype > 3))
	{
		ErrPostEx(SEV_ERROR,0,0,"ASN3: SeqAlign.segs > 3 stripped");
		sap = SeqAlignFree(sap);
	}
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return sap;
erret:
    sap = SeqAlignFree(sap);
    goto ret;
}

/*****************************************************************************
*
*   ScoreNew()
*
*****************************************************************************/
NLM_EXTERN ScorePtr LIBCALL ScoreNew (void)
{
    return (ScorePtr)MemNew(sizeof(Score));
}

/*****************************************************************************
*
*   ScoreSetFree(sp)
*       Frees a complete CHAIN of scores
*
*****************************************************************************/
NLM_EXTERN ScorePtr LIBCALL ScoreSetFree (ScorePtr sp)
{
    ScorePtr next;

    while (sp != NULL)
    {
        next = sp->next;
		ObjectIdFree(sp->id);
        MemFree(sp);
        sp = next;
    }

	return (ScorePtr)NULL;
}

NLM_EXTERN Boolean LIBCALL ScoreSetAsnWrite (ScorePtr sp, AsnIoPtr aip, AsnTypePtr settype)
{
    Boolean retval;
    AsnTypePtr atp;

    if (settype == NULL)
	atp = AsnLinkType(settype, SCORE_SET);
    else
	atp = settype;
    retval = InternalScoreSetAsnWrite(sp, aip, atp, SCORE_SET_E);
    if (settype == NULL)
	AsnUnlinkType(settype);

    return retval;
}

/*****************************************************************************
*
*   InternalScoreSetAsnWrite(sp, aip, settype, elementtype)
*     =====  NOTE:  Reads a complete CHAIN of scores ===================
*     Score never stands alone
*
*****************************************************************************/
static Boolean InternalScoreSetAsnWrite (ScorePtr sp, AsnIoPtr aip, AsnTypePtr settype, AsnTypePtr elementtype)
{
	ScorePtr oldsp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqAlignAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	if (sp == NULL) { AsnNullValueMsg(aip, settype); goto erret; }

								 /* score is local - no link to tree */
	oldsp = sp;
    if (! AsnOpenStruct(aip, settype, (Pointer)sp))
        goto erret;
    while (sp != NULL)       /* write scores */
    {
        if (! AsnOpenStruct(aip, elementtype, (Pointer)sp))
            goto erret;
		if (sp->id != NULL)
		{
	        if (! ObjectIdAsnWrite(sp->id, aip, SCORE_id))
    	        goto erret;
		}
        if (! AsnWriteChoice(aip, SCORE_value, (Int2)sp->choice, &sp->value))
            goto erret;
        switch (sp->choice)
        {
            case 1:      /* int */
                if (! AsnWrite(aip, SCORE_value_int, &sp->value))
                    goto erret;
                break;
            case 2:     /* real */
                if (! AsnWrite(aip, SCORE_value_real, &sp->value))
                    goto erret;
                break;
        }
        if (! AsnCloseStruct(aip, elementtype, (Pointer)sp))
            goto erret;
        sp = sp->next;
    }
				     /* score is local -- no link to tree */	
    if (! AsnCloseStruct(aip, settype, oldsp))
        goto erret;
    retval = TRUE;
erret:
	return retval;
}

NLM_EXTERN ScorePtr LIBCALL ScoreSetAsnRead (AsnIoPtr aip, AsnTypePtr settype)
{
    ScorePtr retval;
    AsnTypePtr atp;

    if (settype == NULL) {
        atp = AsnReadId(aip, amp, SCORE_SET);
    } else {
        atp = AsnLinkType(settype, SCORE_SET);
    }

    retval =  InternalScoreSetAsnRead(aip, atp, SCORE_SET_E);

    AsnUnlinkType (settype);

    return retval;
}

/*****************************************************************************
*
*   InternalScoreSetAsnRead(aip, settype, elementtype)
*       assumes first settype has been read
*       score never stands alone
*       reads a CHAIN of scores
*   	An empty chain is treated as an error even though it is syntactically
*   		correct
*
*****************************************************************************/
static ScorePtr InternalScoreSetAsnRead (AsnIoPtr aip, AsnTypePtr settype, AsnTypePtr elementtype)
{
	DataVal av;
	AsnTypePtr atp;
    ScorePtr sp=NULL, curr = NULL, first = NULL;

	if (! loaded)
	{
		if (! SeqAlignAsnLoad())
			return sp;
	}

	if (aip == NULL)
		return sp;

	atp = settype;
	
    if (AsnReadVal(aip, settype, &av) <= 0) goto erret;   /* start set/seq */

    while ((atp = AsnReadId(aip, amp, atp)) != settype)
    {
        if (atp == NULL)
            goto erret;
        if (atp == SCORE_id)
        {
            sp->id = ObjectIdAsnRead(aip, atp);
            if (sp->id == NULL)
                goto erret;
        }
		else
		{
	    	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the value */
    	    if ((atp == elementtype) && (av.intvalue == START_STRUCT))
        	{
            	sp = ScoreNew();
	            if (sp == NULL)
    	            goto erret;
        	    if (first == NULL)
            	    first = sp;
	            else
    	            curr->next = sp;
        	    curr = sp;
	        }
    	    else if (atp == SCORE_value_int)
        	{
            	sp->choice = 1;
	            sp->value.intvalue = av.intvalue;
    	    }
        	else if (atp == SCORE_value_real)
	        {
    	        sp->choice = 2;
        	    sp->value.realvalue = av.realvalue;
	        }
		}
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end set/seq , no unlink */
	if (first == NULL)    /* empty set of scores, treat as error */
		ErrPost(CTX_NCBIOBJ, 1, "Empty SET OF Score.  line %ld", aip->linenumber);
ret:
	return first;
erret:
    first = ScoreSetFree(first);
    goto ret;
}

/*****************************************************************************
*
*   DenseDiagNew()
*
*****************************************************************************/
NLM_EXTERN DenseDiagPtr LIBCALL DenseDiagNew (void)
{
    return (DenseDiagPtr)MemNew(sizeof(DenseDiag));
}

/*****************************************************************************
*
*   DenseDiagFree(ddp)
*       Frees one DenseDiag and associated data
*
*****************************************************************************/
NLM_EXTERN DenseDiagPtr LIBCALL DenseDiagFree (DenseDiagPtr ddp)
{
    if (ddp == NULL)
        return ddp;

    SeqIdSetFree(ddp->id);  /* frees chain */
    MemFree(ddp->starts);
    MemFree(ddp->strands);
    ScoreSetFree(ddp->scores);   /* frees chain */
	return (DenseDiagPtr)MemFree(ddp);
}

/*****************************************************************************
*
*   DenseDiagAsnWrite(ddp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (DenseDiag ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL DenseDiagAsnWrite (DenseDiagPtr ddp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Int2 dim = 0, i;              /* global dimensionality */
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqAlignAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, DENSE_DIAG);   /* link local tree */
    if (atp == NULL)
        return FALSE;

	if (ddp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)ddp))
        goto erret;

    if (ddp->dim)                     /* default global dimensionality? */
    {
        dim = ddp->dim;
        av.intvalue = ddp->dim;
        if (! AsnWrite(aip, DENSE_DIAG_dim, &av)) goto erret;
    }
    else
        dim = 2;     /* default value */
                                           /* write the SeqIds */
    if (! SeqIdSetAsnWrite(ddp->id, aip, DENSE_DIAG_ids, DENSE_DIAG_ids_E))
        goto erret;

    if (! AsnOpenStruct(aip, DENSE_DIAG_starts, (Pointer)ddp->starts))
        goto erret;
    for (i = 0; i < dim; i++)
    {
        av.intvalue = ddp->starts[i];
        if (! AsnWrite(aip, DENSE_DIAG_starts_E, &av)) goto erret;
    }
    if (! AsnCloseStruct(aip, DENSE_DIAG_starts, (Pointer)ddp->starts))
        goto erret;

    av.intvalue = ddp->len;
    if (! AsnWrite(aip, DENSE_DIAG_len, &av)) goto erret;

    if (ddp->strands != NULL)
    {
        if (! AsnOpenStruct(aip, DENSE_DIAG_strands, (Pointer)ddp->strands))
            goto erret;
        for (i = 0; i < dim; i++)
        {
            av.intvalue = ddp->strands[i];
            if (! AsnWrite(aip, DENSE_DIAG_strands_E, &av)) goto erret;
        }
        if (! AsnCloseStruct(aip, DENSE_DIAG_strands, (Pointer)ddp->strands))
            goto erret;
    }

	if (ddp->scores != NULL)
	{
	    if (! InternalScoreSetAsnWrite(ddp->scores, aip, DENSE_DIAG_scores, DENSE_DIAG_scores_E))
    	    goto erret;
	}

    if (! AsnCloseStruct(aip, atp, (Pointer)ddp))
        goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   DenseDiagAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN DenseDiagPtr LIBCALL DenseDiagAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldtype;
    DenseDiagPtr ddp=NULL;
    Int2 dim, i;

	if (! loaded)
	{
		if (! SeqAlignAsnLoad())
			return ddp;
	}

	if (aip == NULL)
		return ddp;

	if (orig == NULL)           /* DenseDiag ::= (self contained) */
		atp = AsnReadId(aip, amp, DENSE_DIAG);
	else
		atp = AsnLinkType(orig, DENSE_DIAG);    /* link in local tree */
    oldtype = atp;
    if (atp == NULL)
        return ddp;

	ddp = DenseDiagNew();
    if (ddp == NULL)
        goto erret;

	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the start struct */
	atp = AsnReadId(aip, amp, atp);  /* read the dim or ids */
    if (atp == NULL)
        goto erret;
    if (atp == DENSE_DIAG_dim)
    {
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        ddp->dim = (Int2)av.intvalue;
        dim = ddp->dim;
        atp = AsnReadId(aip, amp, atp);   /* ids */
        if (atp == NULL)
            goto erret;
    }
    else
        dim = 2;   /* default */
    
    ddp->id = SeqIdSetAsnRead(aip, DENSE_DIAG_ids, DENSE_DIAG_ids_E);
    if (ddp->id == NULL)
        goto erret;

    ddp->starts = (Int4Ptr) MemNew(sizeof(Int4) * dim);
    if (ddp->starts == NULL)
        goto erret;
    atp = AsnReadId(aip, amp, atp);   /* SEQ OF INTEGER */
    if (atp == NULL)
        goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    for (i = 0; i < dim; i++)
    {
        atp = AsnReadId(aip, amp, atp);
        if (atp == NULL)
            goto erret;
        if (atp != DENSE_DIAG_starts_E)
        {
            ErrPost(CTX_NCBIOBJ, 1, "Too few starts in Dense-diag");
            goto erret;
        }
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        ddp->starts[i] = av.intvalue;
    }
    atp = AsnReadId(aip, amp, atp);
    if (atp != DENSE_DIAG_starts)
    {
        ErrPost(CTX_NCBIOBJ, 1, "Too many starts in Dense-diag");
        goto erret;
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;      /* end SEQ OF INTEGER */

    atp = AsnReadId(aip, amp, atp);   /* len */
    if (atp == NULL)
        goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    ddp->len = av.intvalue;

    while ((atp = AsnReadId(aip, amp, atp)) != oldtype)
    {
        if (atp == NULL)
            goto erret;
        if (atp == DENSE_DIAG_strands)
        {
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* start SEQ OF */
            ddp->strands = (Uint1Ptr) MemNew(sizeof(Uint1) * dim);
            if (ddp->strands == NULL)
                goto erret;
            for (i = 0; i < dim; i++)
            {
                atp = AsnReadId(aip, amp, atp);
                if (atp == NULL)
                    goto erret;
                if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
                ddp->strands[i] = (Uint1)av.intvalue;
            }
            atp = AsnReadId(aip, amp, atp);
            if (atp == NULL)
                goto erret;
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end SEQ OF */
        }
        else if (atp == DENSE_DIAG_scores)
        {
            ddp->scores = InternalScoreSetAsnRead(aip, DENSE_DIAG_scores, DENSE_DIAG_scores_E);
            if (ddp->scores == NULL)
                goto erret;
        }
    }

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end struct */
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return ddp;
erret:
    ddp = DenseDiagFree(ddp);
    goto ret;
}

/*****************************************************************************
*
*   DenseSegNew()
*
*****************************************************************************/
NLM_EXTERN DenseSegPtr LIBCALL DenseSegNew (void)
{
    return (DenseSegPtr)MemNew(sizeof(DenseSeg));
}

/*****************************************************************************
*
*   DenseSegFree(dsp)
*       Frees one DenseSeg and associated data
*
*****************************************************************************/
NLM_EXTERN DenseSegPtr LIBCALL DenseSegFree (DenseSegPtr dsp)
{
    if (dsp == NULL)
        return dsp;

    SeqIdSetFree(dsp->ids);  /* frees chain */
    MemFree(dsp->starts);
    MemFree(dsp->lens);
    MemFree(dsp->strands);
    ScoreSetFree(dsp->scores);   /* frees chain */
	return (DenseSegPtr)MemFree(dsp);
}

/*****************************************************************************
*
*   DenseSegAsnWrite(dsp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (DenseSeg ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL DenseSegAsnWrite (DenseSegPtr dsp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Int2 dim = 0, i,              /* global dimensionality */
        numseg = 0;            /* number of segments represented */
    Int4 total, j;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqAlignAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, DENSE_SEG);   /* link local tree */
    if (atp == NULL)
        return FALSE;

	if (dsp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)dsp))
        goto erret;

    if (dsp->dim)                     /* default global dimensionality? */
    {
        dim = dsp->dim;
        av.intvalue = dsp->dim;
        if (! AsnWrite(aip, DENSE_SEG_dim, &av)) goto erret;
    }
    else
        dim = 2;     /* default value */

    numseg = dsp->numseg;
    av.intvalue = numseg;
    if (! AsnWrite(aip, DENSE_SEG_numseg, &av)) goto erret;

    total = numseg * dim;
                                           /* write the SeqIds */
    if (! SeqIdSetAsnWrite(dsp->ids, aip, DENSE_SEG_ids, DENSE_SEG_ids_E))
        goto erret;

    if (! AsnOpenStruct(aip, DENSE_SEG_starts, (Pointer)dsp->starts))
        goto erret;
    for (j = 0; j < total; j++)
    {
        av.intvalue = dsp->starts[j];
        if (! AsnWrite(aip, DENSE_SEG_starts_E, &av)) goto erret;
    }
    if (! AsnCloseStruct(aip, DENSE_SEG_starts, (Pointer)dsp->starts))
        goto erret;

    if (! AsnOpenStruct(aip, DENSE_SEG_lens, (Pointer)dsp->lens))
        goto erret;
    for (i = 0; i < numseg; i++)
    {
        av.intvalue = dsp->lens[i];
        if (! AsnWrite(aip, DENSE_SEG_lens_E, &av)) goto erret;
    }
    if (! AsnCloseStruct(aip, DENSE_SEG_lens, (Pointer)dsp->lens))
        goto erret;

    if (dsp->strands != NULL)
    {
        if (! AsnOpenStruct(aip, DENSE_SEG_strands, (Pointer)dsp->strands))
            goto erret;
        for (j = 0; j < total; j++)
        {
            av.intvalue = dsp->strands[j];
            if (! AsnWrite(aip, DENSE_SEG_strands_E, &av)) goto erret;
        }
        if (! AsnCloseStruct(aip, DENSE_SEG_strands, (Pointer)dsp->strands))
            goto erret;
    }

	if (dsp->scores != NULL)
	{
	    if (! InternalScoreSetAsnWrite(dsp->scores, aip, DENSE_SEG_scores, DENSE_SEG_scores_E))
    	    goto erret;
	}

    if (! AsnCloseStruct(aip, atp, (Pointer)dsp))
        goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   DenseSegAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN DenseSegPtr LIBCALL DenseSegAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldtype;
    DenseSegPtr dsp=NULL;
    Int2 dim, i, numseg;
    Int4 j, total;

	if (! loaded)
	{
		if (! SeqAlignAsnLoad())
			return dsp;
	}

	if (aip == NULL)
		return dsp;

	if (orig == NULL)           /* DenseSeg ::= (self contained) */
		atp = AsnReadId(aip, amp, DENSE_SEG);
	else
		atp = AsnLinkType(orig, DENSE_SEG);    /* link in local tree */
    oldtype = atp;
    if (atp == NULL)
        return dsp;

	dsp = DenseSegNew();
    if (dsp == NULL)
        goto erret;

	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the start struct */
	atp = AsnReadId(aip, amp, atp);  /* read the dim or numseg */
    if (atp == NULL)
        goto erret;
    if (atp == DENSE_SEG_dim)
    {
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        dsp->dim = (Int2)av.intvalue;
        dim = dsp->dim;
        if (dim == 0) {
        	ErrPostEx(SEV_ERROR,0,0,"DenseSegAsnRead: dim = 0");
        	goto erret;
        }
        atp = AsnReadId(aip, amp, atp);   /* ids */
        if (atp == NULL)
            goto erret;
    }
    else
        dim = 2;   /* default */

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* numseg */
    dsp->numseg = (Int2)av.intvalue;
    numseg = dsp->numseg;
    if (numseg == 0) {
      	ErrPostEx(SEV_ERROR,0,0,"DenseSegAsnRead: numseg = 0");
        goto erret;
    }
    total = numseg * dim;

	atp = AsnReadId(aip, amp, atp);
    if (atp == NULL)
        goto erret;
    dsp->ids = SeqIdSetAsnRead(aip, atp, DENSE_SEG_ids_E);
    if (dsp->ids == NULL)
        goto erret;

    dsp->starts = (Int4Ptr) MemNew(sizeof(Int4) * total);
    if (dsp->starts == NULL)
        goto erret;
    atp = AsnReadId(aip, amp, atp);   /* SEQ OF INTEGER */
    if (atp == NULL)
        goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    for (j = 0; j < total; j++)
    {
        atp = AsnReadId(aip, amp, atp);
        if (atp != DENSE_SEG_starts_E)
        {
            ErrPost(CTX_NCBIOBJ, 1, "Too few starts in Dense-seg");
            goto erret;
        }
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        dsp->starts[j] = av.intvalue;
    }
    atp = AsnReadId(aip, amp, atp);
    if (atp != DENSE_SEG_starts)
    {
        ErrPost(CTX_NCBIOBJ, 1, "Too many starts in Dense-seg");
        goto erret;
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;      /* end SEQ OF INTEGER */

    dsp->lens = (Int4Ptr) MemNew(sizeof(Int4) * numseg);
    if (dsp->lens == NULL)
        goto erret;
    atp = AsnReadId(aip, amp, atp);   /* SEQ OF INTEGER */
    if (atp == NULL)
        goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    for (i = 0; i < numseg; i++)
    {
        atp = AsnReadId(aip, amp, atp);
        if (atp != DENSE_SEG_lens_E)
        {
            ErrPost(CTX_NCBIOBJ, 1, "Too few lens in Dense-seg");
            goto erret;
        }
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        dsp->lens[i] = av.intvalue;
    }
    atp = AsnReadId(aip, amp, atp);
    if (atp != DENSE_SEG_lens)
    {
        ErrPost(CTX_NCBIOBJ, 1, "Too many lens in Dense-seg");
        goto erret;
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;      /* end SEQ OF INTEGER */

    while ((atp = AsnReadId(aip, amp, atp)) != oldtype)
    {
        if (atp == NULL)
            goto erret;
        if (atp == DENSE_SEG_strands)
        {
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* start SEQ OF */
            dsp->strands = (Uint1Ptr) MemNew(sizeof(Uint1) * total);
            if (dsp->strands == NULL)
                goto erret;
            for (j = 0; j < total; j++)
            {
                atp = AsnReadId(aip, amp, atp);
                if (atp == NULL)
                    goto erret;
                if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
                dsp->strands[j] = (Uint1)av.intvalue;
            }
            atp = AsnReadId(aip, amp, atp);
            if (atp == NULL)
                goto erret;
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end SEQ OF */
        }
        else if (atp == DENSE_SEG_scores)
        {
            dsp->scores = InternalScoreSetAsnRead(aip, DENSE_SEG_scores, DENSE_SEG_scores_E);
            if (dsp->scores == NULL)
                goto erret;
        }
    }

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end struct */
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return dsp;
erret:
    dsp = DenseSegFree(dsp);
    goto ret;
}

/*****************************************************************************
*
*   StdSegNew()
*
*****************************************************************************/
NLM_EXTERN StdSegPtr LIBCALL StdSegNew (void)
{
    return (StdSegPtr)MemNew(sizeof(StdSeg));
}
/*****************************************************************************
*
*   StdSegFree(ddp)
*       Frees one StdSeg and associated data
*
*****************************************************************************/
NLM_EXTERN StdSegPtr LIBCALL StdSegFree (StdSegPtr ssp)

{
    if (ssp == NULL)
        return ssp;

    SeqIdSetFree(ssp->ids);  /* frees chain */
    SeqLocSetFree(ssp->loc);  /* frees chain */
    ScoreSetFree(ssp->scores);   /* frees chain */
	return (StdSegPtr)MemFree(ssp);
}

/*****************************************************************************
*
*   StdSegAsnWrite(ssp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (StdSeg ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL StdSegAsnWrite (StdSegPtr ssp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqAlignAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, STD_SEG);   /* link local tree */
    if (atp == NULL)
        return FALSE;

	if (ssp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)ssp))
        goto erret;

    if (ssp->dim)                     /* default global dimensionality? */
    {
        av.intvalue = ssp->dim;
        if (! AsnWrite(aip, STD_SEG_dim, &av)) goto erret;
    }
                                          /* write the SeqIds */
	if (ssp->ids != NULL)
	{
	    if (! SeqIdSetAsnWrite(ssp->ids, aip, STD_SEG_ids, STD_SEG_ids_E))
    	    goto erret;
	}
                                           /* write the SeqLocs */
    if (! SeqLocSetAsnWrite(ssp->loc, aip, STD_SEG_loc, STD_SEG_loc_E))
        goto erret;
                                           /* write the Scores */
	if (ssp->scores != NULL)
	{
	    if (! InternalScoreSetAsnWrite(ssp->scores, aip, STD_SEG_scores, STD_SEG_scores_E))
    	    goto erret;
	}

    if (! AsnCloseStruct(aip, atp, (Pointer)ssp))
        goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}
/*****************************************************************************
*
*   StdSegAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/

NLM_EXTERN StdSegPtr LIBCALL StdSegAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    StdSegPtr ssp=NULL;

	if (! loaded)
	{
		if (! SeqAlignAsnLoad())
			return ssp;
	}

	if (aip == NULL)
		return ssp;

	if (orig == NULL)           /* StdSeg ::= (self contained) */
		atp = AsnReadId(aip, amp, STD_SEG);
	else
		atp = AsnLinkType(orig, STD_SEG);    /* link in local tree */
    if (atp == NULL)
        return ssp;

	ssp = StdSegNew();
    if (ssp == NULL)
        goto erret;

	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the start struct */
	atp = AsnReadId(aip, amp, atp);  /* read the dim or ids */
    if (atp == NULL)
        goto erret;
    if (atp == STD_SEG_dim)
    {
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        ssp->dim = (Int2)av.intvalue;
        atp = AsnReadId(aip, amp, atp);   /* ids */
        if (atp == NULL)
            goto erret;
    }

	if (atp == STD_SEG_ids)
	{
	    ssp->ids = SeqIdSetAsnRead(aip, atp, STD_SEG_ids_E);
    	if (ssp->ids == NULL)
        	goto erret;
        atp = AsnReadId(aip, amp, atp);   /* ids */
        if (atp == NULL)
            goto erret;
	}

    ssp->loc = SeqLocSetAsnRead(aip, STD_SEG_loc, STD_SEG_loc_E);
    if (ssp->loc == NULL)
        goto erret;

    atp = AsnReadId(aip, amp, atp);
    if (atp == NULL)
        goto erret;
    if (atp == STD_SEG_scores)
    {
        ssp->scores = InternalScoreSetAsnRead(aip, STD_SEG_scores, STD_SEG_scores_E);
        if (ssp->scores == NULL)
            goto erret;
        atp = AsnReadId(aip, amp, atp);
        if (atp == NULL)
            goto erret;
    }

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end struct */
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return ssp;
erret:
    ssp = StdSegFree(ssp);
    goto ret;
}

/*****************************************************************************
*
*   SeqAlignSetAsnWrite(sap, aip, set, element)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqAlignSetAsnWrite (SeqAlignPtr sap, AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element)
{
	AsnTypePtr atp;
	SeqAlignPtr oldsap;
    Boolean retval = FALSE;
	Int2 ctr = 0;

	if (! loaded)
	{
		if (! SeqAlignAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(element, SEQ_ALIGN);   /* link local tree */
    if (atp == NULL)
        return FALSE;


	oldsap = sap;
    if (! AsnOpenStruct(aip, set, (Pointer)oldsap))
        goto erret;

    while (sap != NULL)
    {
        if (! SeqAlignAsnWrite(sap, aip, atp))
            goto erret;
        sap = sap->next;
		ctr++;
		if (ctr == 10)
		{
			if (! ProgMon("Write SeqAlign"))
				goto erret;
			ctr = 0;
		}
    }
    
    if (! AsnCloseStruct(aip, set, (Pointer)oldsap))
        goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(element);       /* unlink local tree */
	return retval;
}
/*****************************************************************************
*
*   SeqAlignSetAsnRead(aip, set, element)
*
*****************************************************************************/
NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignSetAsnRead (AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element)
{
	DataVal av;
	AsnTypePtr atp;
    SeqAlignPtr sap=NULL, curr = NULL, first = NULL;
	Int2 ctr = 0;

	if (! loaded)
	{
		if (! SeqAlignAsnLoad())
			return sap;
	}

	if (aip == NULL)
		return sap;


	if (set == NULL)           /* SeqAlignSet ::= (self contained) */
		atp = AsnReadId(aip, amp, SEQ_ALIGN_SET);
	else
		atp = AsnLinkType(set, SEQ_ALIGN_SET);    /* link in local tree */

        if (atp == NULL)
          return sap;

	AsnLinkType(element, SEQ_ALIGN);    /* link in local tree */

	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the start struct */
	while ((atp = AsnReadId(aip, amp, atp)) == element)  /* read the type */
    {
        sap = SeqAlignAsnRead(aip, atp);
        if (sap == NULL)
            goto erret;
        if (first == NULL)
            first = sap;
        else
            curr->next = sap;
        curr = sap;
		ctr++;
		if (ctr == 10)
		{
			if (! ProgMon("Read SeqAlign"))
				goto erret;
			ctr = 0;
		}
    }
    if (atp == NULL)
        goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;  /* end struct */
ret:
	AsnUnlinkType(element);       /* unlink local tree */
	return first;
erret:
    while (first != NULL)
    {
        curr = first;
        first = first->next;
        SeqAlignFree(curr);
    }
    goto ret;
}


/* free a set of Seq-aligns */
NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignSetFree (SeqAlignPtr sap)
{
    SeqAlignPtr next;

    while (sap != NULL)
    {
	next = sap->next;
	SeqAlignFree(sap);
	sap = next;
    }

    return NULL;
}

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignSetNew (void)
{
    return SeqAlignNew();
}

NLM_EXTERN SeqAlignPtr LIBCALL SpecialSeqAlignSetAsnRead (AsnIoPtr aip, AsnTypePtr set)
{
    return SeqAlignSetAsnRead(aip, set, SEQ_ALIGN_SET_E);
}

NLM_EXTERN Boolean LIBCALL SpecialSeqAlignSetAsnWrite (SeqAlignPtr sap, AsnIoPtr aip, AsnTypePtr set)
{
    return SeqAlignSetAsnWrite(sap, aip, set, SEQ_ALIGN_SET_E);
}

NLM_EXTERN Boolean LIBCALL GenericSeqAlignSetAsnWrite (SeqAlignPtr sap, AsnIoPtr aip)
{
    return SeqAlignSetAsnWrite(sap, aip, SEQ_ALIGN_SET, SEQ_ALIGN_SET_E);
}


/*****************************************************************************
*
*   PackSegNew()
*
*****************************************************************************/
NLM_EXTERN PackSegPtr LIBCALL PackSegNew (void)
{
    return (PackSegPtr)MemNew(sizeof(PackSeg));
}

/*****************************************************************************
*
*   PackSegFree(psp)
*       Frees one PackSeg and associated data
*
*****************************************************************************/
NLM_EXTERN PackSegPtr LIBCALL PackSegFree (PackSegPtr psp)
{
    if (psp == NULL)
        return psp;

    SeqIdSetFree(psp->ids);  /* frees chain */
    MemFree(psp->starts);
	BSFree(psp->present);
    MemFree(psp->lens);
    MemFree(psp->strands);
    ScoreSetFree(psp->scores);   /* frees chain */
	return (PackSegPtr)MemFree(psp);
}

/*****************************************************************************
*
*   PackSegAsnWrite(psp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (PackSeg ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL PackSegAsnWrite (PackSegPtr psp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Int2 dim = 0, i,              /* global dimensionality */
        numseg = 0;            /* number of segments represented */
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqAlignAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, PACKED_SEG);   /* link local tree */
    if (atp == NULL)
        return FALSE;

	if (psp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)psp))
        goto erret;

    if (psp->dim)                     /* default global dimensionality? */
    {
        dim = psp->dim;
        av.intvalue = psp->dim;
        if (! AsnWrite(aip, PACKED_SEG_dim, &av)) goto erret;
    }
    else
        dim = 2;     /* default value */

    numseg = psp->numseg;
    av.intvalue = numseg;
    if (! AsnWrite(aip, PACKED_SEG_numseg, &av)) goto erret;

                                           /* write the SeqIds */
    if (! SeqIdSetAsnWrite(psp->ids, aip, PACKED_SEG_ids, PACKED_SEG_ids_E))
        goto erret;

    if (! AsnOpenStruct(aip, PACKED_SEG_starts, (Pointer)psp->starts))
        goto erret;
    for (i = 0; i < dim; i++)
    {
        av.intvalue = psp->starts[i];
        if (! AsnWrite(aip, PACKED_SEG_starts_E, &av)) goto erret;
    }
    if (! AsnCloseStruct(aip, PACKED_SEG_starts, (Pointer)psp->starts))
        goto erret;

	av.ptrvalue = psp->present;
	if (! AsnWrite(aip, PACKED_SEG_present, &av)) goto erret;

    if (! AsnOpenStruct(aip, PACKED_SEG_lens, (Pointer)psp->lens))
        goto erret;
    for (i = 0; i < numseg; i++)
    {
        av.intvalue = psp->lens[i];
        if (! AsnWrite(aip, PACKED_SEG_lens_E, &av)) goto erret;
    }
    if (! AsnCloseStruct(aip, PACKED_SEG_lens, (Pointer)psp->lens))
        goto erret;

    if (psp->strands != NULL)
    {
        if (! AsnOpenStruct(aip, PACKED_SEG_strands, (Pointer)psp->strands))
            goto erret;
        for (i = 0; i < dim; i++)
        {
            av.intvalue = psp->strands[i];
            if (! AsnWrite(aip, PACKED_SEG_strands_E, &av)) goto erret;
        }
        if (! AsnCloseStruct(aip, PACKED_SEG_strands, (Pointer)psp->strands))
            goto erret;
    }

	if (psp->scores != NULL)
	{
	    if (! InternalScoreSetAsnWrite(psp->scores, aip, PACKED_SEG_scores, PACKED_SEG_scores_E))
    	    goto erret;
	}

    if (! AsnCloseStruct(aip, atp, (Pointer)psp))
        goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   PackSegAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN PackSegPtr LIBCALL PackSegAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldtype;
    PackSegPtr psp=NULL;
    Int2 dim, i, numseg;

	if (! loaded)
	{
		if (! SeqAlignAsnLoad())
			return psp;
	}

	if (aip == NULL)
		return psp;

	if (orig == NULL)           /* PackSeg ::= (self contained) */
		atp = AsnReadId(aip, amp, PACKED_SEG);
	else
		atp = AsnLinkType(orig, PACKED_SEG);    /* link in local tree */
    oldtype = atp;
    if (atp == NULL)
        return psp;

	psp = PackSegNew();
    if (psp == NULL)
        goto erret;

	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the start struct */
	atp = AsnReadId(aip, amp, atp);  /* read the dim or numseg */
    if (atp == NULL)
        goto erret;
    if (atp == PACKED_SEG_dim)
    {
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        psp->dim = (Int2)av.intvalue;
        dim = psp->dim;
        atp = AsnReadId(aip, amp, atp);   /* ids */
        if (atp == NULL)
            goto erret;
    }
    else
        dim = 2;   /* default */

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* numseg */
    psp->numseg = (Int2)av.intvalue;
    numseg = psp->numseg;

	atp = AsnReadId(aip, amp, atp);
    if (atp == NULL)
        goto erret;
    psp->ids = SeqIdSetAsnRead(aip, atp, PACKED_SEG_ids_E);
    if (psp->ids == NULL)
        goto erret;

    psp->starts = (Int4Ptr) MemNew(sizeof(Int4) * dim);
    if (psp->starts == NULL)
        goto erret;
    atp = AsnReadId(aip, amp, atp);   /* SEQ OF INTEGER */
    if (atp == NULL)
        goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    for (i = 0; i < dim; i++)
    {
        atp = AsnReadId(aip, amp, atp);
        if (atp != PACKED_SEG_starts_E)
        {
            ErrPost(CTX_NCBIOBJ, 1, "Too few starts in Dense-seg");
            goto erret;
        }
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        psp->starts[i] = av.intvalue;
    }
    atp = AsnReadId(aip, amp, atp);
    if (atp != PACKED_SEG_starts)
    {
        ErrPost(CTX_NCBIOBJ, 1, "Too many starts in Dense-seg");
        goto erret;
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;      /* end SEQ OF INTEGER */

	atp = AsnReadId(aip, amp, atp);    /* Packed-seg.present */
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
	psp->present = (ByteStorePtr)(av.ptrvalue);

    psp->lens = (Int4Ptr) MemNew(sizeof(Int4) * numseg);
    if (psp->lens == NULL)
        goto erret;
    atp = AsnReadId(aip, amp, atp);   /* SEQ OF INTEGER */
    if (atp == NULL)
        goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    for (i = 0; i < numseg; i++)
    {
        atp = AsnReadId(aip, amp, atp);
        if (atp != PACKED_SEG_lens_E)
        {
            ErrPost(CTX_NCBIOBJ, 1, "Too few lens in Dense-seg");
            goto erret;
        }
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        psp->lens[i] = av.intvalue;
    }
    atp = AsnReadId(aip, amp, atp);
    if (atp != PACKED_SEG_lens)
    {
        ErrPost(CTX_NCBIOBJ, 1, "Too many lens in Dense-seg");
        goto erret;
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;      /* end SEQ OF INTEGER */

    while ((atp = AsnReadId(aip, amp, atp)) != oldtype)
    {
        if (atp == NULL)
            goto erret;
        if (atp == PACKED_SEG_strands)
        {
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* start SEQ OF */
            psp->strands = (Uint1Ptr) MemNew(sizeof(Uint1) * dim);
            if (psp->strands == NULL)
                goto erret;
            for (i = 0; i < dim; i++)
            {
                atp = AsnReadId(aip, amp, atp);
                if (atp == NULL)
                    goto erret;
                if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
                psp->strands[i] = (Uint1)av.intvalue;
            }
            atp = AsnReadId(aip, amp, atp);
            if (atp == NULL)
                goto erret;
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end SEQ OF */
        }
        else if (atp == PACKED_SEG_scores)
        {
            psp->scores = InternalScoreSetAsnRead(aip, PACKED_SEG_scores, PACKED_SEG_scores_E);
            if (psp->scores == NULL)
                goto erret;
        }
    }

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end struct */
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return psp;
erret:
    psp = PackSegFree(psp);
    goto ret;
}

