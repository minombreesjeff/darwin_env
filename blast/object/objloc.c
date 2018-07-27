/*  objloc.c
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE                          
*               National Center for Biotechnology Information
*                                                                          
*  This software/database is a "United States Government Work" under the   
*  terms of the United States Copyright Act.  It was written as part of    
*  the author's official duties as a United States Government employee and 
*  thus cannot be copyrighted.  This software/database is freely available 
*  to the loclic for use. The National Library of Medicine and the U.S.    
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
* File Name:  objloc.c
*
* Author:  James Ostell
*   
* Version Creation Date: 4/1/91
*
* $Revision: 6.6 $
*
* File Description:  Object manager for module NCBI-Seqloc
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 05-13-93 Schuler     All public functions are now declared LIBCALL.
*
*
* $Log: objloc.c,v $
* Revision 6.6  2001/08/06 20:07:26  ostell
* added SEQID_TPG, TPE, TPD types
*
* Revision 6.5  2001/01/31 15:24:20  kans
* PatentSeqId.seqid is now an Int4 (JO)
*
* Revision 6.4  2000/04/05 21:42:33  hurwitz
* made SeqIdSetDup consistent with declaration
*
* Revision 6.3  2000/04/05 18:11:54  dondosha
* Moved SeqIdSetDup from mblast.c
*
* Revision 6.2  1998/08/26 17:46:12  kans
* fixed -v -fd warnings in label functions
*
* Revision 6.1  1998/08/24 18:28:05  kans
* removed solaris -v -fd warnings
*
* Revision 6.0  1997/08/25 18:50:03  madden
* Revision changed to 6.0
*
* Revision 4.3  1997/06/19 18:41:29  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 4.2  1996/12/09 16:57:20  kans
* SeqLocAsnRead now returns NULL if not a legal choice
*
 * Revision 4.1  1995/08/29  20:23:05  ostell
 * changed SeqIdPrint to SeqIdWrite
 *
 * Revision 4.0  1995/07/26  13:48:06  ostell
 * force revision to 4.0
 *
 * Revision 3.5  1995/05/15  21:22:00  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/
#include <objloc.h>		   /* the loc interface */
#include <asnloc.h>        /* the AsnTool header */
#include <objmgr.h>
#include <sequtil.h>       /* for label routines */

/*****************************************************************************
*
*   SeqLoc ObjMgr Routines
*
*****************************************************************************/
static CharPtr seqlocname = "SeqLoc";

static Pointer LIBCALLBACK SeqLocNewFunc (void)
{
	return (Pointer) ValNodeNew(NULL);
}

static Pointer LIBCALLBACK SeqLocFreeFunc (Pointer data)
{
	return (Pointer) SeqLocFree ((SeqLocPtr) data);
}

static Boolean LIBCALLBACK SeqLocAsnWriteFunc (Pointer data, AsnIoPtr aip, AsnTypePtr atp)
{
	return SeqLocAsnWrite((SeqLocPtr)data, aip, atp);
}

static Pointer LIBCALLBACK SeqLocAsnReadFunc (AsnIoPtr aip, AsnTypePtr atp)
{
	return (Pointer) SeqLocAsnRead (aip, atp);
}

static Int2 LIBCALLBACK SeqLocLabelFunc ( Pointer data, CharPtr buffer, Int2 buflen, Uint1 content)
{
	return SeqLocLabel((SeqLocPtr)data, buffer, buflen, content);
}

NLM_EXTERN Int2 LIBCALL SeqLocLabel (SeqLocPtr slp, CharPtr buffer, Int2 buflen, Uint1 content)
{
	CharPtr label;
	Int2 diff, len;

	len = buflen;
	switch (content)
	{
		case OM_LABEL_BOTH:
			diff = LabelCopyExtra(buffer, seqlocname, buflen, NULL, ": ");
			buflen -= diff;
		case OM_LABEL_CONTENT:
		case OM_LABEL_SUMMARY:
			label = SeqLocPrint(slp);
			diff = LabelCopy(buffer, label, buflen);
			buflen -= diff;
			MemFree(label);
			return len - buflen;
		case OM_LABEL_TYPE:
		default:
			return LabelCopy(buffer, seqlocname, buflen);
	}
}

static Uint2 LIBCALLBACK SeqLocSubTypeFunc (Pointer ptr)
{
	if (ptr == NULL)
		return 0;
	return (Uint2)((SeqLocPtr)ptr)->choice;
}

/*****************************************************************************
*
*   SeqId ObjMgr Routines
*
*****************************************************************************/
static CharPtr seqidname = "SeqID";

static Pointer LIBCALLBACK SeqIdNewFunc (void)
{
	return (Pointer) ValNodeNew(NULL);
}

static Pointer LIBCALLBACK SeqIdFreeFunc (Pointer data)
{
	return (Pointer) SeqIdFree ((SeqIdPtr) data);
}

static Boolean LIBCALLBACK SeqIdAsnWriteFunc (Pointer data, AsnIoPtr aip, AsnTypePtr atp)
{
	return SeqIdAsnWrite((SeqIdPtr)data, aip, atp);
}

static Pointer LIBCALLBACK SeqIdAsnReadFunc (AsnIoPtr aip, AsnTypePtr atp)
{
	return (Pointer) SeqIdAsnRead (aip, atp);
}

static Int2 LIBCALLBACK SeqIdLabelFunc ( Pointer data, CharPtr buffer, Int2 buflen, Uint1 content)
{
	return SeqIdLabel ((SeqIdPtr)data, buffer, buflen, content);
}

NLM_EXTERN Int2 LIBCALL SeqIdLabel ( SeqIdPtr sip, CharPtr buffer, Int2 buflen, Uint1 content)
{
	Char label[40];
	Int2 diff, len;
	Int2 rsult = 0;

	if ((sip == NULL) || (buflen < 1))
		return 0;

	len = buflen;
	switch (content)
	{
		case OM_LABEL_BOTH:
			diff = LabelCopyExtra(buffer, seqidname, buflen, NULL, ": ");
			buflen -= diff;
		case OM_LABEL_CONTENT:
		case OM_LABEL_SUMMARY:
                        label[0] = '\0';
			SeqIdWrite(sip, label, PRINTID_FASTA_SHORT, 39);
			diff = LabelCopy(buffer, label, buflen);
			buflen -= diff;
			return len - buflen;
		case OM_LABEL_TYPE:
		default:
			rsult = LabelCopy(buffer, seqidname, buflen);
	}
	return rsult;
}

static Uint2 LIBCALLBACK SeqIdSubTypeFunc (Pointer ptr)
{
	if (ptr == NULL)
		return 0;
	return (Uint2)((SeqIdPtr)ptr)->choice;
}


static Boolean loaded = FALSE;

/*****************************************************************************
*
*   Seqloc Routines
*
*****************************************************************************/

/*****************************************************************************
*
*   Seqloc loader
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqLocAsnLoad (void)
{
    if (loaded)
        return TRUE;
    loaded = TRUE;

    if (! GeneralAsnLoad())
    {
        loaded = FALSE;
        return FALSE;
    }
    if (! BiblioAsnLoad())
    {
        loaded = FALSE;
        return FALSE;
    }
    if (! SeqFeatAsnLoad())
    {
        loaded = FALSE;
        return FALSE;
    }
    if (! AsnLoad())
    {
        loaded = FALSE;
        return FALSE;
    }

	ObjMgrTypeLoad(OBJ_SEQLOC, "Seq-loc", seqlocname, "Sequence Location",
		SEQ_LOC, SeqLocNewFunc, SeqLocAsnReadFunc, SeqLocAsnWriteFunc,
		SeqLocFreeFunc, SeqLocLabelFunc, SeqLocSubTypeFunc);

	ObjMgrTypeLoad(OBJ_SEQID, "Seq-align", seqidname, "Sequence ID",
		SEQ_ID, SeqIdNewFunc, SeqIdAsnReadFunc, SeqIdAsnWriteFunc,
		SeqIdFreeFunc, SeqIdLabelFunc, SeqIdSubTypeFunc);

    return TRUE;
}

/*****************************************************************************
*
*   SeqId is a choice using an ValNode, most types in data.ptrvalue
*      except integers, in data.intvalue
*   choice:
*   0 = not set
    1 = local Object-id ,      -- local use
    2 = gibbsq INTEGER ,         -- Geninfo backbone seqid
    3 = gibbmt INTEGER ,         -- Geninfo backbone moltype
    4 = giim Giimport-id ,       -- Geninfo import id
    5 = genbank Textseq-id ,
    6 = embl Textseq-id ,
    7 = pir Textseq-id ,
    8 = swissprot Textseq-id ,
    9 = patent Patent-seq-id ,
    10 = other Textseq-id ,       -- catch all
    11 = general Dbtag }          -- for other databases
    12 = gi  INTEGER          -- GenInfo Integrated Database
    13 = ddbj Textseq-id    -- ddbj
	14 = prf Textseq-id ,         -- PRF SEQDB
	15 = pdb PDB-seq-id          -- PDB sequence
    16 = tpg Textseq-id ,         -- Third Party Annot/Seq Genbank
    17 = tpe Textseq-id ,         -- Third Party Annot/Seq EMBL
    18 = tpd Textseq-id }         -- Third Party Annot/Seq DDBJ
*
*****************************************************************************/
/*****************************************************************************
*
*   SeqIdFree(anp)
*       Frees one SeqId and associated data
*
*****************************************************************************/
NLM_EXTERN SeqIdPtr LIBCALL SeqIdFree (SeqIdPtr anp)
{
    Pointer pnt;

    if (anp == NULL)
        return anp;
    
    pnt = anp->data.ptrvalue;
    switch (anp->choice)
    {
        case SEQID_LOCAL:      /* local */
            ObjectIdFree((ObjectIdPtr)pnt);
            break;
        case SEQID_GIBBSQ:      /* gibbseq */
        case SEQID_GIBBMT:      /* gibbmt */
            break;
        case SEQID_GIIM:      /* giimid */
            GiimFree((GiimPtr)pnt);
            break;
        case SEQID_GENBANK:      /* genbank */
        case SEQID_EMBL:      /* embl */
        case SEQID_PIR:      /* pir   */
        case SEQID_SWISSPROT:      /* swissprot */
        case SEQID_OTHER:     /* other */
        case SEQID_DDBJ:
		case SEQID_PRF:
	case SEQID_TPG:
	case SEQID_TPE:
	case SEQID_TPD:
            TextSeqIdFree((TextSeqIdPtr)pnt);
            break;
        case SEQID_PATENT:      /* patent seq id */
            PatentSeqIdFree((PatentSeqIdPtr)pnt);
            break;
        case SEQID_GENERAL:     /* general */
            DbtagFree((DbtagPtr)pnt);
            break;
        case SEQID_GI:     /* gi */
            break;
		case SEQID_PDB:
			PDBSeqIdFree((PDBSeqIdPtr)pnt);
			break;
    }

	ObjMgrDelete(OBJ_SEQID, (Pointer)anp);

	return (SeqIdPtr)MemFree(anp);
}

/*****************************************************************************
*
*   SeqIdAsnWrite(anp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (SeqId ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqIdAsnWrite (SeqIdPtr anp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, writetype = NULL;
    Pointer pnt;
    AsnWriteFunc func = NULL;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return retval;
	}

	if (aip == NULL)
		return retval;

	atp = AsnLinkType(orig, SEQ_ID);   /* link local tree */
    if (atp == NULL)
        return retval;

	if (anp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

	av.ptrvalue = (Pointer)anp;
    if (! AsnWriteChoice(aip, atp, (Int2)anp->choice, &av))
        goto erret;

    pnt = anp->data.ptrvalue;
    av.intvalue = anp->data.intvalue;
    switch (anp->choice)
    {
        case SEQID_LOCAL:      /* local */
            writetype = SEQ_ID_local;
            func = (AsnWriteFunc) ObjectIdAsnWrite;
            break;
        case SEQID_GIBBSQ:      /* gibbseq */
            retval = AsnWrite(aip, SEQ_ID_gibbsq, &av);
            break;
        case SEQID_GIBBMT:      /* gibbmt */
            retval = AsnWrite(aip, SEQ_ID_gibbmt, &av);
            break;
        case SEQID_GIIM:      /* giimid */
            writetype = SEQ_ID_giim;
            func = (AsnWriteFunc) GiimAsnWrite;
            break;
        case SEQID_GENBANK:      /* genbank */
            writetype = SEQ_ID_genbank;
            func = (AsnWriteFunc) TextSeqIdAsnWrite;
            break;
        case SEQID_EMBL:      /* embl */
            writetype = SEQ_ID_embl;
            func = (AsnWriteFunc) TextSeqIdAsnWrite;
            break;
        case SEQID_PIR:      /* pir   */
            writetype = SEQ_ID_pir;
            func = (AsnWriteFunc) TextSeqIdAsnWrite;
            break;
        case SEQID_SWISSPROT:      /* swissprot */
            writetype = SEQ_ID_swissprot;
            func = (AsnWriteFunc) TextSeqIdAsnWrite;
            break;
        case SEQID_OTHER:     /* other */
            writetype = SEQ_ID_other;
            func = (AsnWriteFunc) TextSeqIdAsnWrite;
            break;
        case SEQID_PATENT:      /* patent seq id */
            writetype = SEQ_ID_patent;
            func = (AsnWriteFunc) PatentSeqIdAsnWrite;
            break;
        case SEQID_GENERAL:     /* general */
            writetype = SEQ_ID_general;
            func = (AsnWriteFunc) DbtagAsnWrite;
            break;
        case SEQID_GI:     /* gi */
            retval = AsnWrite(aip, SEQ_ID_gi, &av);
            break;
        case SEQID_DDBJ:      /* ddbj */
            writetype = SEQ_ID_ddbj;
            func = (AsnWriteFunc) TextSeqIdAsnWrite;
            break;
        case SEQID_PRF:      /* prf   */
            writetype = SEQ_ID_prf;
            func = (AsnWriteFunc) TextSeqIdAsnWrite;
            break;
        case SEQID_PDB:      /* pdb   */
            writetype = SEQ_ID_pdb;
            func = (AsnWriteFunc) PDBSeqIdAsnWrite;
            break;
        case SEQID_TPG:      /* tpg   */
            writetype = SEQ_ID_tpg;
            func = (AsnWriteFunc) TextSeqIdAsnWrite;
            break;
        case SEQID_TPE:      /* tpe   */
            writetype = SEQ_ID_tpe;
            func = (AsnWriteFunc) TextSeqIdAsnWrite;
            break;
        case SEQID_TPD:      /* tpd   */
            writetype = SEQ_ID_tpd;
            func = (AsnWriteFunc) TextSeqIdAsnWrite;
            break;
    }
    if (writetype != NULL)
        retval = (* func)(pnt, aip, writetype);   /* write it out */
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   SeqIdAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occured
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occured.
*
*****************************************************************************/
NLM_EXTERN SeqIdPtr LIBCALL SeqIdAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    SeqIdPtr anp=NULL;
    Uint1 choice;
    AsnReadFunc func;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return anp;
	}

	if (aip == NULL)
		return anp;

	if (orig == NULL)           /* SeqId ::= (self contained) */
		atp = AsnReadId(aip, amp, SEQ_ID);
	else
		atp = AsnLinkType(orig, SEQ_ID);    /* link in local tree */
    if (atp == NULL)
        return anp;

	anp = ValNodeNew(NULL);
    if (anp == NULL)
        goto erret;

    if ( AsnReadVal(aip, atp, &av) <= 0)
        goto erret;    /* read the CHOICE value (nothing) */
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)
        goto erret;  /* find the choice */

    func = NULL;

	if (atp == SEQ_ID_local)
    {
        choice = SEQID_LOCAL;
        func = (AsnReadFunc) ObjectIdAsnRead;
    }
    else if (atp == SEQ_ID_gibbsq)
    {
        choice = SEQID_GIBBSQ;
        AsnReadVal(aip, atp, &av);
        anp->data.intvalue = av.intvalue;
    }
    else if (atp == SEQ_ID_gibbmt)
    {
        choice = SEQID_GIBBMT;
        AsnReadVal(aip, atp, &av);
        anp->data.intvalue = av.intvalue;
    }
    else if (atp == SEQ_ID_giim)
    {
        choice = SEQID_GIIM;
        func = (AsnReadFunc) GiimAsnRead;
    }
    else if (atp == SEQ_ID_genbank)
    {
        choice = SEQID_GENBANK;
        func = (AsnReadFunc) TextSeqIdAsnRead;
    }
    else if (atp == SEQ_ID_embl)
    {
        choice = SEQID_EMBL;
        func = (AsnReadFunc) TextSeqIdAsnRead;
    }
    else if (atp == SEQ_ID_pir)
    {
        choice = SEQID_PIR;
        func = (AsnReadFunc) TextSeqIdAsnRead;
    }
    else if (atp == SEQ_ID_swissprot)
    {
        choice = SEQID_SWISSPROT;
        func = (AsnReadFunc) TextSeqIdAsnRead;
    }
    else if (atp == SEQ_ID_patent)
    {
        choice = SEQID_PATENT;
        func = (AsnReadFunc) PatentSeqIdAsnRead;
    }
    else if (atp == SEQ_ID_other)
    {
        choice = SEQID_OTHER;
        func = (AsnReadFunc) TextSeqIdAsnRead;
    }
    else if (atp == SEQ_ID_general)
    {
        choice = SEQID_GENERAL;
        func = (AsnReadFunc) DbtagAsnRead;
    }
    else if (atp == SEQ_ID_gi)
    {
        choice = SEQID_GI;
        AsnReadVal(aip, atp, &av);
        anp->data.intvalue = av.intvalue;
    }
    else if (atp == SEQ_ID_ddbj)
    {
        choice = SEQID_DDBJ;
        func = (AsnReadFunc) TextSeqIdAsnRead;
    }
    else if (atp == SEQ_ID_prf)
    {
        choice = SEQID_PRF;
        func = (AsnReadFunc) TextSeqIdAsnRead;
    }
    else if (atp == SEQ_ID_pdb)
    {
        choice = SEQID_PDB;
        func = (AsnReadFunc) PDBSeqIdAsnRead;
    }
    else if (atp == SEQ_ID_tpg)
    {
        choice = SEQID_TPG;
        func = (AsnReadFunc) TextSeqIdAsnRead;
    }
    else if (atp == SEQ_ID_tpe)
    {
        choice = SEQID_TPE;
        func = (AsnReadFunc) TextSeqIdAsnRead;
    }
    else if (atp == SEQ_ID_tpd)
    {
        choice = SEQID_TPD;
        func = (AsnReadFunc) TextSeqIdAsnRead;
    }
    else
        goto erret;

    anp->choice = choice;
    if (func != NULL)
        anp->data.ptrvalue = (* func)(aip, atp);
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return anp;
erret:
    anp = SeqIdFree(anp);
    goto ret;
}

/*****************************************************************************
*
*   SeqIdPtr SeqIdDup(oldid)
*
*****************************************************************************/
NLM_EXTERN SeqIdPtr LIBCALL SeqIdDup (SeqIdPtr oldid)
{
    TextSeqIdPtr at, bt;
	GiimPtr ga, gb;
	PatentSeqIdPtr psa, psb;
	PDBSeqIdPtr pdba, pdbb;
	SeqIdPtr newid = NULL;

    if (oldid == NULL)
        return oldid;

	newid = ValNodeNew(NULL);
	if (newid == NULL) return newid;
	MemCopy(newid, oldid, sizeof(ValNode));
	newid->next = NULL;    /* not in chain */
    switch (oldid->choice)
    {
        case SEQID_NOT_SET:
            break;
        case SEQID_LOCAL:
			newid->data.ptrvalue = ObjectIdDup((ObjectIdPtr)oldid->data.ptrvalue);
			break;
			                 /* integer types */
        case SEQID_GIBBSQ:   /* gibbsq */
        case SEQID_GIBBMT:   /* gibbmt */
        case SEQID_GI:  /* gi */
            break;

        case SEQID_GIIM:   /* giim */
			ga = (GiimPtr) oldid->data.ptrvalue;
			gb = GiimNew();
			if (gb == NULL) return NULL;
			gb->id = ga->id;
			gb->db = StringSave(ga->db);
			gb->release = StringSave(ga->release);
			newid->data.ptrvalue = gb;
			break;
        case SEQID_PATENT:   /* patent seq */
			psa = (PatentSeqIdPtr)oldid->data.ptrvalue;
			psb = PatentSeqIdNew();
			if (psb == NULL) return NULL;
			psb->seqid = psa->seqid;
			psb->cit = IdPatNew();
			psb->cit->country = StringSave(psa->cit->country);
			psb->cit->number = StringSave(psa->cit->number);
			psb->cit->app_number = StringSave(psa->cit->app_number);
			newid->data.ptrvalue = psb;
			break;
							/* TextSeqId Types */
		case SEQID_GENBANK:
		case SEQID_EMBL:
		case SEQID_PIR:
		case SEQID_SWISSPROT:
		case SEQID_OTHER:
		case SEQID_DDBJ:
		case SEQID_PRF:
		case SEQID_TPG:
		case SEQID_TPE:
		case SEQID_TPD:
			at = (TextSeqIdPtr)oldid->data.ptrvalue;
            bt = TextSeqIdNew();
			if (bt == NULL) return NULL;
			bt->name = StringSave(at->name);
			bt->accession = StringSave(at->accession);
			bt->release = StringSave(bt->release);
			bt->version = at->version;
			newid->data.ptrvalue = bt;
			break;
		case SEQID_GENERAL:
			newid->data.ptrvalue = DbtagDup((DbtagPtr)oldid->data.ptrvalue);
			break;
		case SEQID_PDB:
			pdba = (PDBSeqIdPtr)oldid->data.ptrvalue;
            pdbb = PDBSeqIdNew();
			if (pdbb == NULL) return NULL;
			newid->data.ptrvalue = pdbb;
			pdbb->mol = StringSave(pdba->mol);
			pdbb->chain = pdba->chain;
			pdbb->rel = DateDup(pdba->rel);
			break;
     }
	return newid;
}

/*****************************************************************************
*
*   SeqIdPtr SeqIdSetDup(oldid)
*       duplicates a chain of SeqId's
*
*****************************************************************************/
NLM_EXTERN SeqIdPtr LIBCALL 
SeqIdSetDup(SeqIdPtr seqid)
{
   SeqIdPtr sid_head, sid, seqid_var;

   if (seqid == NULL)
      return seqid;
   else {
      seqid_var = seqid;
      sid_head = sid = SeqIdDup(seqid);
   }

   while ((seqid_var = seqid_var->next) != NULL) {
      sid->next = SeqIdDup(seqid_var);
      sid = sid->next;
   }

   return sid_head;
}


/*****************************************************************************
*
*   SeqIdSetFree()
*       frees a chain of SeqId's
*
*****************************************************************************/
NLM_EXTERN SeqIdPtr LIBCALL SeqIdSetFree (SeqIdPtr sip)
{
    SeqIdPtr next;

    while (sip != NULL)
    {
        next = sip->next;
        SeqIdFree(sip);
        sip = next;
    }
    return sip;
}

/*****************************************************************************
*
*   SeqIdSetAsnRead(aip, settype, elementtype)
*       read a set/seq of SeqId's
*
*****************************************************************************/
NLM_EXTERN SeqIdPtr LIBCALL SeqIdSetAsnRead (AsnIoPtr aip, AsnTypePtr settype, AsnTypePtr elementtype)
{
    DataVal av;
    AsnTypePtr atp, atp2;
    SeqIdPtr first = NULL, curr = NULL, anp;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return first;
	}

    atp2 = AsnLinkType(elementtype, SEQ_ID);
    if (atp2 == NULL)
        return first;

    if (AsnReadVal(aip, settype, &av) <= 0)   /* start struct */
        goto erret;

    atp = settype;
    while ((atp = AsnReadId(aip, amp, atp)) != settype)
    {
        if (atp == NULL)
            goto erret;
        anp = SeqIdAsnRead(aip, atp2);
        if (anp == NULL)
            goto erret;

        if (first == NULL)
            first = anp;
        else
            curr->next = anp;
        curr = anp;
    }
    if (AsnReadVal(aip, atp, &av) <= 0)    /* end struct */
        goto erret;
	if (first == NULL)
		ErrPost(CTX_NCBIOBJ, 1, "Empty SET OF Seq-id. line %ld", aip->linenumber);
ret:
    AsnUnlinkType(elementtype);
    return first;
erret:
    first = SeqIdSetFree(first);
    goto ret;
}

/*****************************************************************************
*
*   SeqIdSetAsnWrite(anp, aip, settype, elementtype)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqIdSetAsnWrite (SeqIdPtr anp, AsnIoPtr aip, AsnTypePtr settype, AsnTypePtr elementtype)
{
    AsnTypePtr atp2;
	SeqIdPtr oldanp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return retval;
	}

    if (aip == NULL)
        return retval;

    atp2 = AsnLinkType(elementtype, SEQ_ID);
    if (atp2 == NULL)
        return retval;

	if (anp == NULL) { AsnNullValueMsg(aip, settype); goto erret; }

	oldanp = anp;
    if (! AsnOpenStruct(aip, settype, (Pointer)oldanp))
        goto erret;
    while (anp != NULL)
    {
        if (! SeqIdAsnWrite(anp, aip, atp2))
            goto erret;
        anp = anp->next;
    }
    if (! AsnCloseStruct(aip, settype, (Pointer)oldanp))
        goto erret;
    retval = TRUE;
erret:
    AsnUnlinkType(elementtype);
    return retval;
}

/*****************************************************************************
*
*   TextSeqIdNew()
*
*****************************************************************************/
NLM_EXTERN TextSeqIdPtr LIBCALL TextSeqIdNew (void)
{
	TextSeqIdPtr tsip;

    tsip = (TextSeqIdPtr)MemNew(sizeof(TextSeqId));
	if (tsip == NULL) return tsip;

	tsip->version = INT2_MIN;
	return tsip;
}

/*****************************************************************************
*
*   TextSeqIdFree(anp)
*       Frees one TextSeqId
*
*****************************************************************************/
NLM_EXTERN TextSeqIdPtr LIBCALL TextSeqIdFree (TextSeqIdPtr tsip)
{
    if (tsip == NULL)
        return tsip;
    
    MemFree(tsip->name);
    MemFree(tsip->accession);
    MemFree(tsip->release);
	return (TextSeqIdPtr)MemFree(tsip);
}

/*****************************************************************************
*
*   TextSeqIdAsnWrite(tsip, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (TextSeqId ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL TextSeqIdAsnWrite (TextSeqIdPtr tsip, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return retval;
	}

	if (aip == NULL)
		return retval;

	atp = AsnLinkType(orig, TEXTSEQ_ID);   /* link local tree */
    if (atp == NULL)
        return retval;

	if (tsip == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)tsip))
        goto erret;
    if (tsip->name != NULL)
    {
        av.ptrvalue = tsip->name;
        if (! AsnWrite(aip, TEXTSEQ_ID_name, &av))
            goto erret;
    }
    if (tsip->accession != NULL)
    {
        av.ptrvalue = tsip->accession;
        if (! AsnWrite(aip, TEXTSEQ_ID_accession, &av))
            goto erret;
    }
    if (tsip->release != NULL)
    {
        av.ptrvalue = tsip->release;
        if (! AsnWrite(aip, TEXTSEQ_ID_release, &av))
            goto erret;
    }
	if (tsip->version != INT2_MIN)
	{
		av.intvalue = (Int4) tsip->version;
		if (! AsnWrite(aip, TEXTSEQ_ID_version, &av))
            goto erret;
	}
    if (! AsnCloseStruct(aip, atp, (Pointer)tsip))
        goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   TextSeqIdAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occured
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occured.
*
*****************************************************************************/
NLM_EXTERN TextSeqIdPtr LIBCALL TextSeqIdAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldtype;
    TextSeqIdPtr tsip=NULL;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return tsip;
	}

	if (aip == NULL)
		return tsip;

	if (orig == NULL)           /* TextSeqId ::= (self contained) */
		atp = AsnReadId(aip, amp, TEXTSEQ_ID);
	else
		atp = AsnLinkType(orig, TEXTSEQ_ID);    /* link in local tree */
    oldtype = atp;
    if (atp == NULL)
        return tsip;

	tsip = TextSeqIdNew();
    if (tsip == NULL)
        goto erret;

	if ((AsnReadVal(aip, oldtype, &av)) <= 0)    /* read the START_STRUCT */
        goto erret;
    while ((atp = AsnReadId(aip, amp, atp)) != oldtype)
    {
        if (atp == NULL)
            goto erret;
        if ((AsnReadVal(aip, atp, &av)) <= 0)
            goto erret;
    	if (atp == TEXTSEQ_ID_name)
            tsip->name = (CharPtr)av.ptrvalue;
        else if (atp == TEXTSEQ_ID_accession)
            tsip->accession = (CharPtr)av.ptrvalue;
        else if (atp == TEXTSEQ_ID_release)
            tsip->release = (CharPtr)av.ptrvalue;
		else if (atp == TEXTSEQ_ID_version)
			tsip->version = (Int2)av.intvalue;
        else
            goto erret;
    }
    if ((AsnReadVal(aip, atp, &av)) <= 0)      /* read END_STRUCT */
        goto erret;
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return tsip;
erret:
    tsip = TextSeqIdFree(tsip);
    goto ret;
}

/*****************************************************************************
*
*   PatentSeqIdNew()
*
*****************************************************************************/
NLM_EXTERN PatentSeqIdPtr LIBCALL PatentSeqIdNew (void)
{
    return (PatentSeqIdPtr)MemNew(sizeof(PatentSeqId));
}

/*****************************************************************************
*
*   PatentSeqIdFree(anp)
*       Frees one PatentSeqId
*
*****************************************************************************/
NLM_EXTERN PatentSeqIdPtr LIBCALL PatentSeqIdFree (PatentSeqIdPtr psip)
{
    if (psip == NULL)
        return psip;
    
    IdPatFree(psip->cit);
	return (PatentSeqIdPtr)MemFree(psip);
}

/*****************************************************************************
*
*   PatentSeqIdAsnWrite(psip, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (PatentSeqId ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL PatentSeqIdAsnWrite (PatentSeqIdPtr psip, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, PATENT_SEQ_ID);   /* link local tree */
    if (atp == NULL)
        return FALSE;

	if (psip == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)psip))
        goto erret;
    av.intvalue = psip->seqid;
    if (! AsnWrite(aip, PATENT_SEQ_ID_seqid, &av))
        goto erret;
    if (! IdPatAsnWrite(psip->cit, aip, PATENT_SEQ_ID_cit))
        goto erret;
    if (! AsnCloseStruct(aip, atp, (Pointer)psip))
        goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   PatentSeqIdAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occured
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occured.
*
*****************************************************************************/
NLM_EXTERN PatentSeqIdPtr LIBCALL PatentSeqIdAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    PatentSeqIdPtr psip=NULL;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return psip;
	}

	if (aip == NULL)
		return psip;

	if (orig == NULL)           /* PatentSeqId ::= (self contained) */
		atp = AsnReadId(aip, amp, PATENT_SEQ_ID);
	else
		atp = AsnLinkType(orig, PATENT_SEQ_ID);    /* link in local tree */
    if (atp == NULL)
        return psip;

	psip = PatentSeqIdNew();
    if (psip == NULL)
        goto erret;

	if (AsnReadVal(aip, atp, &av) <= 0)    /* read the START_STRUCT */
        goto erret;
    atp = AsnReadId(aip, amp, atp);   /* read the seqid */
    if (atp == NULL)
        goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0)
        goto erret;
    psip->seqid = av.intvalue;
    atp = AsnReadId(aip, amp, atp);   /* read the cit */
    if (atp == NULL)
        goto erret;
    psip->cit = IdPatAsnRead(aip, atp);
    if (psip->cit == NULL)
        goto erret;
    atp = AsnReadId(aip, amp, atp);
    if (atp == NULL)
        goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0)      /* read END_STRUCT */
        goto erret;
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return psip;
erret:
    psip = PatentSeqIdFree(psip);
    goto ret;
}

/*****************************************************************************
*
*   GiimNew()
*
*****************************************************************************/
NLM_EXTERN GiimPtr LIBCALL GiimNew (void)
{
    return (GiimPtr)MemNew(sizeof(Giim));
}

/*****************************************************************************
*
*   GiimFree(anp)
*       Frees one Giim
*
*****************************************************************************/
NLM_EXTERN GiimPtr LIBCALL GiimFree (GiimPtr gip)
{
    if (gip == NULL)
        return gip;
    
    MemFree(gip->db);
    MemFree(gip->release);
	return (GiimPtr)MemFree(gip);
}

/*****************************************************************************
*
*   GiimAsnWrite(gip, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (Giim ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL GiimAsnWrite (GiimPtr gip, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, GIIMPORT_ID);   /* link local tree */
    if (atp == NULL)
        return FALSE;

	if (gip == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)gip))
        goto erret;
    av.intvalue = gip->id;
    if (! AsnWrite(aip, GIIMPORT_ID_id, &av))
        goto erret;
    if (gip->db != NULL)
    {
        av.ptrvalue = gip->db;
        if (! AsnWrite(aip, GIIMPORT_ID_db, &av))
            goto erret;
    }
    if (gip->release != NULL)
    {
        av.ptrvalue = gip->release;
        if (! AsnWrite(aip, GIIMPORT_ID_release, &av))
            goto erret;
    }
    if (! AsnCloseStruct(aip, atp, (Pointer)gip))
        goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   GiimAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occured
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occured.
*
*****************************************************************************/
NLM_EXTERN GiimPtr LIBCALL GiimAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldtype;
    GiimPtr gip=NULL;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return gip;
	}

	if (aip == NULL)
		return gip;

	if (orig == NULL)           /* Giim ::= (self contained) */
		atp = AsnReadId(aip, amp, GIIMPORT_ID);
	else
		atp = AsnLinkType(orig, GIIMPORT_ID);    /* link in local tree */
    if (atp == NULL)
        return gip;
    oldtype = atp;

	gip = GiimNew();
    if (gip == NULL)
        goto erret;

	if (AsnReadVal(aip, oldtype, &av) <= 0)    /* read the START_STRUCT */
        goto erret;
    atp = AsnReadId(aip, amp, atp);   /* read the seqid */
    if (atp == NULL)
        goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0)
        goto erret;
    gip->id = av.intvalue;
    while ((atp = AsnReadId(aip, amp, atp)) != oldtype)
    {
        if (atp == NULL)
            goto erret;
        if (AsnReadVal(aip, atp, &av) <= 0)
            goto erret;
        if (atp == GIIMPORT_ID_db)
            gip->db = (CharPtr)av.ptrvalue;
        else if (atp == GIIMPORT_ID_release)
            gip->release = (CharPtr)av.ptrvalue;
        else
            goto erret;
    }
    if (AsnReadVal(aip, atp, &av) <= 0)      /* read END_STRUCT */
        goto erret;
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return gip;
erret:
    gip = GiimFree(gip);
    goto ret;
}

/*****************************************************************************
*
*   PDBSeqIdNew()
*
*****************************************************************************/
NLM_EXTERN PDBSeqIdPtr LIBCALL PDBSeqIdNew (void)
{
	PDBSeqIdPtr pdbsip;

    pdbsip = (PDBSeqIdPtr)MemNew(sizeof(PDBSeqId));
	if (pdbsip == NULL) return pdbsip;
	pdbsip->chain = (Uint1)32;
	return pdbsip;
}

/*****************************************************************************
*
*   PDBSeqIdFree(anp)
*       Frees one PDBSeqId
*
*****************************************************************************/
NLM_EXTERN PDBSeqIdPtr LIBCALL PDBSeqIdFree (PDBSeqIdPtr pdbsip)
{
    if (pdbsip == NULL)
        return pdbsip;
    
    MemFree(pdbsip->mol);
	DateFree(pdbsip->rel);
	return (PDBSeqIdPtr)MemFree(pdbsip);
}

/*****************************************************************************
*
*   PDBSeqIdAsnWrite(pdbsip, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (PDBSeqId ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL PDBSeqIdAsnWrite (PDBSeqIdPtr pdbsip, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return retval;
	}

	if (aip == NULL)
		return retval;

	atp = AsnLinkType(orig, PDB_SEQ_ID);   /* link local tree */
    if (atp == NULL)
        return retval;

	if (pdbsip == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)pdbsip))
        goto erret;
    if (pdbsip->mol != NULL)
    {
        av.ptrvalue = pdbsip->mol;
        if (! AsnWrite(aip, PDB_SEQ_ID_mol, &av))
            goto erret;
    }
    if (pdbsip->chain != (Uint1)32)
    {
        av.intvalue = (Int4) pdbsip->chain;
        if (! AsnWrite(aip, PDB_SEQ_ID_chain, &av))
            goto erret;
    }
    if (pdbsip->rel != NULL)
    {
        if (! DateAsnWrite(pdbsip->rel, aip, PDB_SEQ_ID_rel))
            goto erret;
    }
    if (! AsnCloseStruct(aip, atp, (Pointer)pdbsip))
        goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   PDBSeqIdAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occured
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occured.
*
*****************************************************************************/
NLM_EXTERN PDBSeqIdPtr LIBCALL PDBSeqIdAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldtype;
    PDBSeqIdPtr pdbsip=NULL;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return pdbsip;
	}

	if (aip == NULL)
		return pdbsip;

	if (orig == NULL)           /* PDBSeqId ::= (self contained) */
		atp = AsnReadId(aip, amp, PDB_SEQ_ID);
	else
		atp = AsnLinkType(orig, PDB_SEQ_ID);    /* link in local tree */
    oldtype = atp;
    if (atp == NULL)
        return pdbsip;

	pdbsip = PDBSeqIdNew();
    if (pdbsip == NULL)
        goto erret;

	if ((AsnReadVal(aip, oldtype, &av)) <= 0)    /* read the START_STRUCT */
        goto erret;
    while ((atp = AsnReadId(aip, amp, atp)) != oldtype)
    {
        if (atp == NULL)
            goto erret;
        if (atp == PDB_SEQ_ID_rel)
		{
            pdbsip->rel = DateAsnRead(aip, atp);
			if (pdbsip->rel == NULL)
				goto erret;
		}
		else
		{
	        if ((AsnReadVal(aip, atp, &av)) <= 0)
    	        goto erret;
    		if (atp == PDB_SEQ_ID_mol)
            	pdbsip->mol = (CharPtr)av.ptrvalue;
	        else if (atp == PDB_SEQ_ID_chain)
    	        pdbsip->chain = (Uint1)av.intvalue;
        	else
            	goto erret;
		}
    }
    if ((AsnReadVal(aip, atp, &av)) <= 0)      /* read END_STRUCT */
        goto erret;
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return pdbsip;
erret:
    pdbsip = PDBSeqIdFree(pdbsip);
    goto ret;
}

/*****************************************************************************
*
*   SeqLoc
*   SeqLoc is a choice using an ValNode, most types in data.ptrvalue
*      except integers, in data.intvalue
*   choice:
    1 = null NULL ,           -- not placed
    2 = empty Seq-id ,        -- to NULL one Seq-id in a collection
    3 = whole Seq-id ,        -- whole sequence
    4 = int Seq-interval ,    -- from to
    5 = packed-int Packed-seqint ,
    6 = pnt Seq-point ,
    7 = packed-pnt Packed-seqpnt ,
    8 = mix SEQUENCE OF Seq-loc ,
    9 = equiv SET OF Seq-loc ,  -- equivalent sets of locations
    10 = bond Seq-bond
    11 = feat Feat-id
*
*****************************************************************************/
/*****************************************************************************
*
*   SeqLocFree(anp)
*       Frees one SeqLoc and associated data
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL SeqLocFree (SeqLocPtr anp)
{
    Pointer pnt;

    if (anp == NULL)
        return anp;
    
    pnt = anp->data.ptrvalue;
    switch (anp->choice)
    {
        case SEQLOC_NULL:      /* null */
            break;
        case SEQLOC_EMPTY:      /* empty */
        case SEQLOC_WHOLE:      /* whole */
            SeqIdFree((SeqIdPtr)pnt);
            break;
        case SEQLOC_INT:      /* int */
            SeqIntFree((SeqIntPtr)pnt);
            break;
        case SEQLOC_PNT:      /* pnt */
            SeqPntFree((SeqPntPtr)pnt);
            break;
        case SEQLOC_PACKED_PNT:      /* packed-pnt   */
            PackSeqPntFree((PackSeqPntPtr)pnt);
            break;
        case SEQLOC_MIX:      /* mix */
        case SEQLOC_EQUIV:     /* equiv */
        case SEQLOC_PACKED_INT:      /* packed seqint */
            SeqLocSetFree((SeqLocPtr)pnt);
            break;
        case SEQLOC_BOND:      /* bond */
            SeqBondFree((SeqBondPtr)pnt);
            break;
        case SEQLOC_FEAT:
            SeqFeatIdFree((ChoicePtr)pnt);
            MemFree(pnt);    /* get rid of Choice too */
            break;
    }

	ObjMgrDelete(OBJ_SEQLOC, (Pointer)anp);

	return (SeqLocPtr)MemFree(anp);
}

/*****************************************************************************
*
*   SeqLocAsnWrite(anp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (SeqLoc ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqLocAsnWrite (SeqLocPtr anp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, writetype = NULL;
    Pointer pnt;
    AsnWriteFunc func = NULL;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, SEQ_LOC);   /* link local tree */
    if (atp == NULL)
        return FALSE;

	if (anp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

	av.ptrvalue = (Pointer)anp;
	if (! AsnWriteChoice(aip, atp, (Int2)anp->choice, &av))
        goto erret;

    pnt = anp->data.ptrvalue;
    av.intvalue = anp->data.intvalue;
    switch (anp->choice)
    {
        case SEQLOC_NULL:      /* null */
            retval = AsnWrite(aip, SEQ_LOC_null, &av);
            break;
        case SEQLOC_EMPTY:      /* empty */
            writetype = SEQ_LOC_empty;
            func = (AsnWriteFunc) SeqIdAsnWrite;
            break;
        case SEQLOC_WHOLE:      /* whole */
            writetype = SEQ_LOC_whole;
            func = (AsnWriteFunc) SeqIdAsnWrite;
            break;
        case SEQLOC_INT:      /* int */
            writetype = SEQ_LOC_int;
            func = (AsnWriteFunc) SeqIntAsnWrite;
            break;
        case SEQLOC_PACKED_INT:      /* packed-int */
            writetype = SEQ_LOC_packed_int;
            func = (AsnWriteFunc) PackSeqIntAsnWrite;
            break;
        case SEQLOC_PNT:      /* pnt */
            writetype = SEQ_LOC_pnt;
            func = (AsnWriteFunc) SeqPntAsnWrite;
            break;
        case SEQLOC_PACKED_PNT:      /* packed-pnt */
            writetype = SEQ_LOC_packed_pnt;
            func = (AsnWriteFunc) PackSeqPntAsnWrite;
            break;
        case SEQLOC_MIX:      /* mix */
            writetype = SEQ_LOC_mix;
            func = (AsnWriteFunc) SeqLocMixAsnWrite;
            break;
        case SEQLOC_EQUIV:     /* equiv */
            writetype = SEQ_LOC_equiv;
            func = (AsnWriteFunc) SeqLocEquivAsnWrite;
            break;
        case SEQLOC_BOND:      /* bond */
            writetype = SEQ_LOC_bond;
            func = (AsnWriteFunc) SeqBondAsnWrite;
            break;
        case SEQLOC_FEAT:      /* Feat-id */
            writetype = SEQ_LOC_feat;
            func = (AsnWriteFunc) SeqFeatIdAsnWrite;
            break;
    }
    if (writetype != NULL)
        retval = (* func)(pnt, aip, writetype);   /* write it out */
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   SeqLocAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occured
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occured.
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL SeqLocAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    SeqLocPtr anp=NULL;
    Uint1 choice;
    AsnReadFunc func;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return anp;
	}

	if (aip == NULL)
		return anp;

	if (orig == NULL)           /* SeqLoc ::= (self contained) */
		atp = AsnReadId(aip, amp, SEQ_LOC);
	else
		atp = AsnLinkType(orig, SEQ_LOC);    /* link in local tree */
    if (atp == NULL)
        return anp;

	anp = ValNodeNew(NULL);
    if (anp == NULL)
        goto erret;

	if (AsnReadVal(aip, atp, &av) <= 0)  /* read the CHOICE value (nothing) */
        goto erret;
	atp = AsnReadId(aip, amp, atp);  /* find the choice */
    if (atp == NULL)
        goto erret;

    func = NULL;

	if (atp == SEQ_LOC_null)
    {
        choice = SEQLOC_NULL;
        if (AsnReadVal(aip, atp, &av) <= 0)
            goto erret;
    }
    else if (atp == SEQ_LOC_empty)
    {
        choice = SEQLOC_EMPTY;
        func = (AsnReadFunc) SeqIdAsnRead;
    }
    else if (atp == SEQ_LOC_whole)
    {
        choice = SEQLOC_WHOLE;
        func = (AsnReadFunc) SeqIdAsnRead;
    }
    else if (atp == SEQ_LOC_int)
    {
        choice = SEQLOC_INT;
        func = (AsnReadFunc) SeqIntAsnRead;
    }
    else if (atp == SEQ_LOC_packed_int)
    {
        choice = SEQLOC_PACKED_INT;
        func = (AsnReadFunc) PackSeqIntAsnRead;
    }
    else if (atp == SEQ_LOC_pnt)
    {
        choice = SEQLOC_PNT;
        func = (AsnReadFunc) SeqPntAsnRead;
    }
    else if (atp == SEQ_LOC_packed_pnt)
    {
        choice = SEQLOC_PACKED_PNT;
        func = (AsnReadFunc) PackSeqPntAsnRead;
    }
    else if (atp == SEQ_LOC_mix)
    {
        choice = SEQLOC_MIX;
        func = (AsnReadFunc) SeqLocMixAsnRead;
    }
    else if (atp == SEQ_LOC_equiv)
    {
        choice = SEQLOC_EQUIV;
        func = (AsnReadFunc) SeqLocEquivAsnRead;
    }
    else if (atp == SEQ_LOC_bond)
    {
        choice = SEQLOC_BOND;
        func = (AsnReadFunc) SeqBondAsnRead;
    }
    else if (atp == SEQ_LOC_feat)
    {
        choice = SEQLOC_FEAT;
        anp->data.ptrvalue = MemNew(sizeof(Choice));
        if (anp->data.ptrvalue == NULL)
            goto erret;
        if (! SeqFeatIdAsnRead(aip, atp, (ChoicePtr)anp->data.ptrvalue))
            goto erret;
    } else
    	goto erret;

    anp->choice = choice;
    if (func != NULL)
    {
        anp->data.ptrvalue = (* func)(aip, atp);
        if (anp->data.ptrvalue == NULL)
            goto erret;
    }
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return anp;
erret:
    anp = SeqLocFree(anp);
    goto ret;
}

/*****************************************************************************
*
*   SeqLocMixAsnWrite(anp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (SeqLocMix ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqLocMixAsnWrite (SeqLocPtr anp, AsnIoPtr aip, AsnTypePtr orig)
{
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, SEQ_LOC_MIX);   /* link local tree */
    if (atp == NULL)
        return FALSE;

	if (anp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    retval = SeqLocSetAsnWrite(anp, aip, atp, SEQ_LOC_MIX_E);
erret:
    AsnUnlinkType(orig);
    return retval;
}

/*****************************************************************************
*
*   SeqLocMixAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occured
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occured.
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL SeqLocMixAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	AsnTypePtr atp;
    SeqLocPtr anp=NULL;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return anp;
	}

	if (aip == NULL)
		return anp;

	if (orig == NULL)           /* Seq-loc-mix ::= (self contained) */
		atp = AsnReadId(aip, amp, SEQ_LOC_MIX);
	else
		atp = AsnLinkType(orig, SEQ_LOC_MIX);    /* link in local tree */
    if (atp == NULL)
        return anp;
    anp = SeqLocSetAsnRead(aip, atp, SEQ_LOC_MIX_E);
    AsnUnlinkType(orig);
    return anp;
}

/*****************************************************************************
*
*   SeqLocEquivAsnWrite(anp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (SeqLocEquiv ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqLocEquivAsnWrite (SeqLocPtr anp, AsnIoPtr aip, AsnTypePtr orig)
{
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, SEQ_LOC_EQUIV);   /* link local tree */
    if (atp == NULL)
        return FALSE;

	if (anp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    retval = SeqLocSetAsnWrite(anp, aip, atp, SEQ_LOC_EQUIV_E);
erret:
    AsnUnlinkType(orig);
    return retval;
}

/*****************************************************************************
*
*   SeqLocEquivAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occured
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occured.
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL SeqLocEquivAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	AsnTypePtr atp;
    SeqLocPtr anp=NULL;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return anp;
	}

	if (aip == NULL)
		return anp;

	if (orig == NULL)           /* Seq-loc-equiv ::= (self contained) */
		atp = AsnReadId(aip, amp, SEQ_LOC_EQUIV);
	else
		atp = AsnLinkType(orig, SEQ_LOC_EQUIV);    /* link in local tree */
    if (atp == NULL)
        return anp;
    anp = SeqLocSetAsnRead(aip, atp, SEQ_LOC_EQUIV_E);
    AsnUnlinkType(orig);
    return anp;
}

/*****************************************************************************
*
*   SeqLocSetAsnWrite(anp, aip, atp, set, element)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (SeqLocSet ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqLocSetAsnWrite (SeqLocPtr anp, AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element)
{
    AsnTypePtr atp2;
	SeqLocPtr oldanp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

    atp2 = AsnLinkType(element, SEQ_LOC);
    if (atp2 == NULL)
        return FALSE;

	if (anp == NULL) { AsnNullValueMsg(aip, set); goto erret; }

	oldanp = anp;
    if (! AsnOpenStruct(aip, set, (Pointer)oldanp))
        goto erret;
    while (anp != NULL)
    {
        if (! SeqLocAsnWrite(anp, aip, atp2))
            goto erret;
        anp = anp->next;
    }
    if (! AsnCloseStruct(aip, set, (Pointer)oldanp))
        goto erret;
    retval = TRUE;
erret:
    AsnUnlinkType(element);
	return retval;
}

/*****************************************************************************
*
*   SeqLocSetAsnRead(aip, atp, set, element)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occured
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occured.
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL SeqLocSetAsnRead (AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element)
{
	DataVal av;
	AsnTypePtr atp, atp2;
    SeqLocPtr anp, first=NULL, prev;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return first;
	}

	if (aip == NULL)
		return first;

    prev = NULL;

    atp2 = AsnLinkType(element, SEQ_LOC);
    if (atp2 == NULL)
        return first;
    atp = set;
	if (AsnReadVal(aip, atp, &av) <= 0)    /* read the START_STRUCT */
        goto erret;
    while ((atp = AsnReadId(aip, amp, atp)) != set)
    {
        if (atp == NULL)
            goto erret;
        anp = SeqLocAsnRead(aip, atp2);
        if (anp == NULL)
            goto erret;
        if (first == NULL)
            first = anp;
        else
            prev->next = anp;
        prev = anp;
    }
    if (AsnReadVal(aip, atp, &av) <= 0)      /* read END_STRUCT */
        goto erret;
	if (first == NULL)
		ErrPost(CTX_NCBIOBJ, 1, "Empty SET OF Seq-loc.  line %ld", aip->linenumber);
ret:
    AsnUnlinkType(element);
	return first;
erret:
    first = SeqLocSetFree(first);
    goto ret;
}

/*****************************************************************************
*
*   SeqLocSetFree(anp)
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL SeqLocSetFree (SeqLocPtr anp)
{
    SeqLocPtr next;

    while (anp != NULL)
    {
        next = anp->next;
        SeqLocFree(anp);
        anp = next;
    }
    return anp;
}

/*****************************************************************************
*
*   SeqIntNew()
*
*****************************************************************************/
NLM_EXTERN SeqIntPtr LIBCALL SeqIntNew (void)
{
    return (SeqIntPtr)MemNew(sizeof(SeqInt));
}

/*****************************************************************************
*
*   SeqIntFree(sip)
*       Frees one SeqInt if next == NULL
*       or PackSeqInt if next != NULL
*
*****************************************************************************/
NLM_EXTERN SeqIntPtr LIBCALL SeqIntFree (SeqIntPtr sip)
{
    if (sip == NULL)
        return sip;

    IntFuzzFree(sip->if_from);
    IntFuzzFree(sip->if_to);
    SeqIdFree(sip->id);
    return (SeqIntPtr)MemFree(sip);
}

/*****************************************************************************
*
*   SeqIntAsnWrite(sip, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (SeqInt ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqIntAsnWrite (SeqIntPtr sip, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, SEQ_INTERVAL);   /* link local tree */
    if (atp == NULL)
        goto erret;

	if (sip == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)sip))
        goto erret;
    av.intvalue = sip->from;
    if (! AsnWrite(aip, SEQ_INTERVAL_from, &av))
        goto erret;
    av.intvalue = sip->to;
    if (! AsnWrite(aip, SEQ_INTERVAL_to, &av))
        goto erret;
    if (sip->strand)
    {
        av.intvalue = sip->strand;
        if (! AsnWrite(aip, SEQ_INTERVAL_strand, &av))
            goto erret;
    }
    if (sip->id != NULL)
    {
        if (! SeqIdAsnWrite(sip->id, aip, SEQ_INTERVAL_id))
            goto erret;
    }
    if (sip->if_from != NULL)
    {
        if (! IntFuzzAsnWrite(sip->if_from, aip, SEQ_INTERVAL_fuzz_from))
            goto erret;
    }
    if (sip->if_to != NULL)
    {
        if (! IntFuzzAsnWrite(sip->if_to, aip, SEQ_INTERVAL_fuzz_to))
            goto erret;
    }
    if (! AsnCloseStruct(aip, atp, (Pointer)sip))
        goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   SeqIntAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occured
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occured.
*
*****************************************************************************/
NLM_EXTERN SeqIntPtr LIBCALL SeqIntAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldtype;
    SeqIntPtr sip=NULL;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return sip;
	}

	if (aip == NULL)
		return sip;

	if (orig == NULL)           /* SeqInt ::= (self contained) */
		atp = AsnReadId(aip, amp, SEQ_INTERVAL);
	else
		atp = AsnLinkType(orig, SEQ_INTERVAL);    /* link in local tree */
    oldtype = atp;
    if (atp == NULL)
        return sip;

	sip = SeqIntNew();
    if (sip == NULL)
        goto erret;

	if (AsnReadVal(aip, oldtype, &av) <= 0)    /* read the START_STRUCT */
        goto erret;
    atp = AsnReadId(aip, amp, atp);   /* read the from */
    if (atp == NULL)
        goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0)
        goto erret;
    sip->from = av.intvalue;
    atp = AsnReadId(aip, amp, atp);   /* read the to */
    if (atp == NULL)
        goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0)
        goto erret;
    sip->to = av.intvalue;
    while ((atp = AsnReadId(aip, amp, atp)) != oldtype)  /* get options */
    {
        if (atp == NULL)
            goto erret;
        if (atp == SEQ_INTERVAL_strand)
        {
            if (AsnReadVal(aip, atp, &av) <= 0)
                goto erret;
            sip->strand = (Uint1)av.intvalue;
        }
        else if (atp == SEQ_INTERVAL_id)
        {
            sip->id = SeqIdAsnRead(aip, atp);
            if (sip->id == NULL)
                goto erret;
        }
        else if (atp == SEQ_INTERVAL_fuzz_from)
        {
            sip->if_from = IntFuzzAsnRead(aip, atp);
            if (sip->if_from == NULL)
                goto erret;
        }
        else if (atp == SEQ_INTERVAL_fuzz_to)
        {
            sip->if_to = IntFuzzAsnRead(aip, atp);
            if (sip->if_to == NULL)
                goto erret;
        }
    }
    if (AsnReadVal(aip, atp, &av) <= 0)      /* read END_STRUCT */
        goto erret;

ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return sip;
erret:
    sip = SeqIntFree(sip);
    goto ret;
}

/*****************************************************************************
*
*   PackSeqIntAsnWrite(sip, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (SeqInt ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL PackSeqIntAsnWrite (SeqLocPtr sip, AsnIoPtr aip, AsnTypePtr orig)
{
	AsnTypePtr atp;
	SeqLocPtr oldsip;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, PACKED_SEQINT);   /* link local tree */
    if (atp == NULL)
        return FALSE;
	oldsip = sip;

	if (sip == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)oldsip))
        goto erret;
	for ( ;sip != NULL; sip = sip->next)
	{
        if (! SeqIntAsnWrite((SeqIntPtr)sip->data.ptrvalue, aip,
                PACKED_SEQINT_E))
            goto erret;
    }
    if (! AsnCloseStruct(aip, atp, (Pointer)oldsip))
        goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   PackSeqIntAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occured
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occured.
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL PackSeqIntAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldtype;
	SeqLocPtr head = NULL, now = NULL;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return head;
	}

	if (aip == NULL)
		return head;

	if (orig == NULL)           /* SeqInt ::= (self contained) */
		atp = AsnReadId(aip, amp, PACKED_SEQINT);
	else
		atp = AsnLinkType(orig, PACKED_SEQINT);    /* link in local tree */
    oldtype = atp;
    if (atp == NULL)
        return head;
	
    if (AsnReadVal(aip, oldtype, &av) <= 0)    /* read the START_STRUCT */
        goto erret;
    while ((atp = AsnReadId(aip, amp, atp)) != oldtype)  /* get options */
    {
            if (atp == NULL)
                goto erret;
			now = ValNodeNew(now);
            if (now == NULL)
                goto erret;
			if ( ! head)
				head = now;
			now -> choice = 4;
			now -> data.ptrvalue =  SeqIntAsnRead(aip, atp);
            if (now->data.ptrvalue == NULL)
                goto erret;
    }
    if (AsnReadVal(aip, atp, &av) <= 0)      /* read END_STRUCT */
        goto erret;
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return head;
erret:
    head = SeqLocFree(head);
    goto ret;
}

/*****************************************************************************
*
*   SeqPntNew()
*
*****************************************************************************/
NLM_EXTERN SeqPntPtr LIBCALL SeqPntNew (void)
{
    return (SeqPntPtr)MemNew(sizeof(SeqPnt));
}

/*****************************************************************************
*
*   SeqPntFree(spp)
*       Frees one SeqPnt if next == NULL
*       or PackSeqPnt if next != NULL
*
*****************************************************************************/
NLM_EXTERN SeqPntPtr LIBCALL SeqPntFree (SeqPntPtr spp)
{
    if (spp == NULL)
        return spp;

    IntFuzzFree(spp->fuzz);
    SeqIdFree(spp->id);
    return (SeqPntPtr)MemFree(spp);
}

/*****************************************************************************
*
*   SeqPntAsnWrite(spp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (SeqPnt ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqPntAsnWrite (SeqPntPtr spp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, SEQ_POINT);   /* link local tree */
    if (atp == NULL)
        goto erret;

	if (spp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)spp))
        goto erret;
    av.intvalue = spp->point;
    if (! AsnWrite(aip, SEQ_POINT_point, &av))
        goto erret;
    if (spp->strand)
    {
        av.intvalue = spp->strand;
        if (! AsnWrite(aip, SEQ_POINT_strand, &av))
            goto erret;
    }
    if (spp->id != NULL)
    {
        if (! SeqIdAsnWrite(spp->id, aip, SEQ_POINT_id))
            goto erret;
    }
    if (spp->fuzz != NULL)
    {
        if (! IntFuzzAsnWrite(spp->fuzz, aip, SEQ_POINT_fuzz))
            goto erret;
    }
    if (! AsnCloseStruct(aip, atp, (Pointer)spp))
        goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   SeqPntAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occured
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occured.
*
*****************************************************************************/
NLM_EXTERN SeqPntPtr LIBCALL SeqPntAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldtype;
    SeqPntPtr spp=NULL;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return spp;
	}

	if (aip == NULL)
		return spp;

	if (orig == NULL)           /* SeqPnt ::= (self contained) */
		atp = AsnReadId(aip, amp, SEQ_POINT);
	else
		atp = AsnLinkType(orig, SEQ_POINT);    /* link in local tree */
    oldtype = atp;
    if (atp == NULL)
        return spp;

	spp = SeqPntNew();
    if (spp == NULL)
        goto erret;

	if (AsnReadVal(aip, oldtype, &av) <= 0)    /* read the START_STRUCT */
        goto erret;
    atp = AsnReadId(aip, amp, atp);   /* read the point */
    if (atp == NULL)
        goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0)
        goto erret;
    spp->point = av.intvalue;
    while ((atp = AsnReadId(aip, amp, atp)) != oldtype)  /* get options */
    {
        if (atp == NULL)
            goto erret;
        if (atp == SEQ_POINT_strand)
        {
            if (AsnReadVal(aip, atp, &av) <= 0)
                goto erret;
            spp->strand = (Uint1)av.intvalue;
        }
        else if (atp == SEQ_POINT_id)
        {
            spp->id = SeqIdAsnRead(aip, atp);
            if (spp->id == NULL)
                goto erret;
        }
        else if (atp == SEQ_POINT_fuzz)
        {
            spp->fuzz = IntFuzzAsnRead(aip, atp);
            if (spp->fuzz == NULL)
                goto erret;
        }
    }
    if (AsnReadVal(aip, atp, &av) <= 0)      /* read END_STRUCT */
        goto erret;

ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return spp;
erret:
    spp = SeqPntFree(spp);
    goto ret;
}

/*****************************************************************************
*
*   PackSeqPntNew()
*
*****************************************************************************/
NLM_EXTERN PackSeqPntPtr LIBCALL PackSeqPntNew (void)
{
    return (PackSeqPntPtr)MemNew(sizeof(PackSeqPnt));
}

/*****************************************************************************
*
*   PackSeqPntFree(pspp)
*       Frees one PackSeqPnt if next == NULL
*       or PackPackSeqPnt if next != NULL
*
*****************************************************************************/
NLM_EXTERN PackSeqPntPtr LIBCALL PackSeqPntFree (PackSeqPntPtr pspp)
{
    PackSeqPntPtr next;

    while (pspp != NULL)
    {
        next = pspp->next;
        IntFuzzFree(pspp->fuzz);
        SeqIdFree(pspp->id);
        MemFree(pspp);
        pspp = next;
    }
    return pspp;
}

/*****************************************************************************
*
*   PackSeqPntAsnWrite(pspp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (PackSeqPnt ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL PackSeqPntAsnWrite (PackSeqPntPtr pspp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Int4 num, index;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, PACKED_SEQPNT);   /* link local tree */
    if (atp == NULL)
        return FALSE;

	if (pspp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)pspp))
        goto erret;
    if (pspp->strand)
    {
        av.intvalue = pspp->strand;
        if (! AsnWrite(aip, PACKED_SEQPNT_strand, &av))
            goto erret;
    }
    if (pspp->id != NULL)
    {
        if (! SeqIdAsnWrite(pspp->id, aip, PACKED_SEQPNT_id))
            goto erret;
    }
    if (pspp->fuzz != NULL)
    {
        if (! IntFuzzAsnWrite(pspp->fuzz, aip, PACKED_SEQPNT_fuzz))
            goto erret;
    }
    if (! AsnOpenStruct(aip, PACKED_SEQPNT_points, (Pointer)pspp))
        goto erret;
    num = PackSeqPntNum(pspp);   /* total number of points */
    for (index = 0; index < num; index++)
    {
        av.intvalue = PackSeqPntGet(pspp, index);

        if (! AsnWrite(aip, PACKED_SEQPNT_points_E, &av))
            goto erret;
    }
    if (! AsnCloseStruct(aip, PACKED_SEQPNT_points, (Pointer)pspp))
        goto erret;
    if (! AsnCloseStruct(aip, atp, (Pointer)pspp))
        goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   PackSeqPntAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occured
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occured.
*
*****************************************************************************/
NLM_EXTERN PackSeqPntPtr LIBCALL PackSeqPntAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldtype;
    PackSeqPntPtr pspp=NULL;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return pspp;
	}

	if (aip == NULL)
		return pspp;

	if (orig == NULL)           /* PackSeqPnt ::= (self contained) */
		atp = AsnReadId(aip, amp, PACKED_SEQPNT);
	else
		atp = AsnLinkType(orig, PACKED_SEQPNT);    /* link in local tree */
    oldtype = atp;
    if (atp == NULL)
        return pspp;

	pspp = PackSeqPntNew();
    if (pspp == NULL)
        goto erret;

	if (AsnReadVal(aip, oldtype, &av) <= 0)    /* read the START_STRUCT */
        goto erret;
    while ((atp = AsnReadId(aip, amp, atp)) != oldtype)  /* get options */
    {
        if (atp == NULL)
            goto erret;
        if (atp == PACKED_SEQPNT_strand)
        {
            if (AsnReadVal(aip, atp, &av) <= 0)
                goto erret;
            pspp->strand = (Uint1)av.intvalue;
        }
        else if (atp == PACKED_SEQPNT_id)
        {
            pspp->id = SeqIdAsnRead(aip, atp);
            if (pspp->id == NULL)
                goto erret;
        }
        else if (atp == PACKED_SEQPNT_fuzz)
        {
            pspp->fuzz = IntFuzzAsnRead(aip, atp);
            if (pspp->fuzz == NULL)
                goto erret;
        }
        else if (atp == PACKED_SEQPNT_points)
        {
            if (AsnReadVal(aip, atp, &av) <= 0)   /* start struct */
                goto erret;
            while ((atp = AsnReadId(aip, amp, atp)) == PACKED_SEQPNT_points_E)
            {
                if (AsnReadVal(aip, atp, &av) <= 0)
                    goto erret;

                if (! PackSeqPntPut(pspp, av.intvalue))
                    goto erret;
            }
            if (atp == NULL)
                 goto erret;
            if (AsnReadVal(aip, atp, &av) <= 0)   /* end struct */
                goto erret;
        }
    }
    if (AsnReadVal(aip, atp, &av) <= 0)      /* read END_STRUCT */
        goto erret;
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return pspp;
erret:
    pspp = PackSeqPntFree(pspp);
    goto ret;
}

/*****************************************************************************
*
*   PackSeqPntNum(pspp)
*
*****************************************************************************/
NLM_EXTERN Int4 LIBCALL PackSeqPntNum (PackSeqPntPtr pspp)
{
    Int4 num = 0;
    while (pspp != NULL)
    {
        num += pspp->used;
        pspp = pspp->next;
    }
    return num;
}

/*****************************************************************************
*
*   PackSeqPntGet(pspp, index)
*       returns point at index, or -1 if index out of range
*
*****************************************************************************/
NLM_EXTERN Int4 LIBCALL PackSeqPntGet (PackSeqPntPtr pspp, Int4 index)
{
    Int4 num = 0;
    if (index < 0)
        return -1;

    while (pspp != NULL)
    {
        if (index < (Int4) (num + (pspp->used)))
            return pspp->pnts[index - num];
        num += pspp->used;
        pspp = pspp->next;
    }
    return -1;
}

/*****************************************************************************
*
*   PackSeqPntPut(pspp, point)
*       adds to end of list
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL PackSeqPntPut (PackSeqPntPtr pspp, Int4 point)
{
    if ((pspp == NULL) || (point < 0))
        return FALSE;

    while (pspp->next != NULL)
        pspp = pspp->next;

    if (pspp->used == PACK_PNT_NUM)   /* full */
    {
        pspp->next = PackSeqPntNew();
        if (pspp->next == NULL)
            return FALSE;
        pspp = pspp->next;
    }
    pspp->pnts[pspp->used] = point;
    pspp->used++;
    return TRUE;
}

/*****************************************************************************
*
*   SeqBondNew()
*
*****************************************************************************/
NLM_EXTERN SeqBondPtr LIBCALL SeqBondNew (void)
{
    return (SeqBondPtr)MemNew(sizeof(SeqBond));
}

/*****************************************************************************
*
*   SeqBondFree(sbp)
*       Frees one SeqBond
*
*****************************************************************************/
NLM_EXTERN SeqBondPtr LIBCALL SeqBondFree (SeqBondPtr sbp)
{
    if (sbp == NULL)
        return sbp;
    
    SeqPntFree(sbp->a);
    SeqPntFree(sbp->b);
    return (SeqBondPtr)MemFree(sbp);
}

/*****************************************************************************
*
*   SeqBondAsnWrite(sbp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (SeqBond ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqBondAsnWrite (SeqBondPtr sbp, AsnIoPtr aip, AsnTypePtr orig)
{
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, SEQ_BOND);   /* link local tree */
    if (atp == NULL)
        return FALSE;

	if (sbp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)sbp))
        goto erret;
    if (! SeqPntAsnWrite(sbp->a, aip, SEQ_BOND_a))
        goto erret;
    if (sbp->b != NULL)
    {
        if (! SeqPntAsnWrite(sbp->b, aip, SEQ_BOND_b))
            goto erret;
    }
    if (! AsnCloseStruct(aip, atp, (Pointer)sbp))
        goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   SeqBondAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occured
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occured.
*
*****************************************************************************/
NLM_EXTERN SeqBondPtr LIBCALL SeqBondAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldtype;
    SeqBondPtr sbp=NULL;
    SeqPntPtr tmp;

	if (! loaded)
	{
		if (! SeqLocAsnLoad())
			return sbp;
	}

	if (aip == NULL)
		return sbp;

	if (orig == NULL)           /* SeqBond ::= (self contained) */
		atp = AsnReadId(aip, amp, SEQ_BOND);
	else
		atp = AsnLinkType(orig, SEQ_BOND);    /* link in local tree */
    oldtype = atp;
    if (atp == NULL)
        return sbp;

	sbp = SeqBondNew();
    if (sbp == NULL)
        goto erret;

	if (AsnReadVal(aip, oldtype, &av) <= 0)    /* read the START_STRUCT */
        goto erret;
    while ((atp = AsnReadId(aip, amp, atp)) != oldtype)  /* get options */
    {
        if (atp == NULL)
            goto erret;
        tmp = SeqPntAsnRead(aip, atp);
        if (tmp == NULL)
            goto erret;
        if (atp == SEQ_BOND_a)
            sbp->a = tmp;
        else
            sbp->b = tmp;
    }
    if (AsnReadVal(aip, atp, &av) <= 0)      /* read END_STRUCT */
        goto erret;
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return sbp;
erret:
    sbp = SeqBondFree(sbp);
    goto ret;
}

