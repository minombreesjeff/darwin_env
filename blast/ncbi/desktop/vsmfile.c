/*   vsmfile.c
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
* File Name:  vsm.c
*
* Author:  Jim Ostell
*
* Version Creation Date:   11-29-94
*
* $Revision: 6.4 $
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

#include <vsmpriv.h>
#include <tofasta.h>
#include <objmgr.h>
#include <dlogutil.h>

/*****************************************************************************
*
*   VSMFileInit()
*		Initialize VSM file I/O routines
*
*****************************************************************************/
Boolean LIBCALL VSMFileInit(void)
{
                         /** register default functions */
                         /*** OPEN ***/
	ObjMgrProcLoad(OMPROC_OPEN, "Read FASTA Protein File","FASTA protein file", 0,0,0,0,NULL,
                           VSMFastaProtOpen, PROC_PRIORITY_DEFAULT);
                           
	ObjMgrProcLoad(OMPROC_OPEN, "Read FASTA Nucleotide File","FASTA nucleotide file", 0,0,0,0,NULL,
                           VSMFastaNucOpen, PROC_PRIORITY_DEFAULT);
                           
	ObjMgrProcLoad(OMPROC_OPEN, "Read Binary ASN1 File","ASN.1 binary file", 0,0,0,0,NULL,
                           VSMGenericBinAsnOpen, PROC_PRIORITY_DEFAULT);
                           
	ObjMgrProcLoad(OMPROC_OPEN, "Read Text ASN1 File","ASN.1 text file", 0,0,0,0,NULL,
                           VSMGenericTextAsnOpen, PROC_PRIORITY_DEFAULT);
                        
                        /*** SAVE ***/   
	ObjMgrProcLoad(OMPROC_SAVE, "Save As FASTA Protein File","FASTA protein file", 0,0,0,0,NULL,
                           VSMFastaProtSave, PROC_PRIORITY_DEFAULT);
                           
	ObjMgrProcLoad(OMPROC_SAVE, "Save As FASTA Nucleotide File","FASTA nucleotide file", 0,0,0,0,NULL,
                           VSMFastaNucSave, PROC_PRIORITY_DEFAULT);
                           
	ObjMgrProcLoad(OMPROC_SAVE, "Save As Binary ASN1 File","ASN.1 binary file", 0,0,0,0,NULL,
                           VSMGenericBinAsnSave, PROC_PRIORITY_DEFAULT);
                           
	ObjMgrProcLoad(OMPROC_SAVE, "Save As Text ASN1 File","ASN.1 text file", 0,0,0,0,NULL,
                           VSMGenericTextAsnSave, PROC_PRIORITY_DEFAULT);

	return TRUE;
}

static void PromoteToSeqEntry (Uint2 entityID, Uint2 datatype, Pointer dataptr)

{
	BioseqPtr     bsp;
	BioseqSetPtr  bssp;
	SeqEntryPtr   sep;

	sep = GetTopSeqEntryForEntityID (entityID);
	if (sep != NULL) return;
	sep = SeqEntryNew ();
	if (sep == NULL) return;
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
	} else {
		sep = SeqEntryFree (sep);
	}
}

Int2 LIBCALLBACK VSMGenericTextAsnOpen ( Pointer data )
{
	Char filename[255];
	Pointer ptr = NULL;
	Uint2 entityID, datatype;
	Int2 retval = OM_MSG_RET_ERROR;
	OMProcControlPtr ompcp;

	ompcp = (OMProcControlPtr)data;

	filename[0] = '\0';
	if (GetInputFileName(filename, (size_t)254, NULL, NULL))
	{
		WatchCursor();
		ptr = ObjMgrGenericAsnTextFileRead (filename, &datatype, &entityID);
		ArrowCursor();
		if (ptr == NULL) goto erret;

		ompcp->output_data = ptr;
		ompcp->output_entityID = entityID;
		PromoteToSeqEntry (entityID, datatype, ptr);

		retval = OM_MSG_RET_DONE;
	}
	else
		retval = OM_MSG_RET_OK;

ret:
	return retval;
erret:
	goto ret;
}

typedef struct vsmreadbinstr {
	Boolean do_it,
			window_done;
	Int2 the_type;
	PopuP p;
} VSMReadBinStr, PNTR VSMReadBinStrPtr;

static void AsnBinAcceptProc (ButtoN b)
{
	WindoW w;
	VSMReadBinStrPtr vrp;
	
	w = ParentWindow(b);
	vrp = (VSMReadBinStrPtr) GetWindowExtra(w);
	vrp->do_it = TRUE;
	vrp->the_type = GetValue(vrp->p);
	Remove(w);
	vrp->window_done = TRUE;
	return;	
}

static void AsnBinCancelProc (ButtoN b)
{
	WindoW w;
	VSMReadBinStrPtr vrp;
	
	w = ParentWindow(b);
	vrp = (VSMReadBinStrPtr) GetWindowExtra(w);
	vrp->do_it = FALSE;
	Remove(w);
	vrp->window_done = TRUE;
	return;	
}

Int2 LIBCALLBACK VSMGenericBinAsnOpen ( Pointer data )
{
	Char filename[255];
	AsnIoPtr aip;
	Pointer ptr;
	Uint2 entityID;
	Int2 ct, i, retval = OM_MSG_RET_OK;
	ObjMgrPtr omp;
	ObjMgrTypePtr omtp = NULL;
	OMProcControlPtr ompcp;
	WindoW w;
	GrouP g;
	ButtoN b;
	VSMReadBinStr vrb;
	VSeqMgrPtr vsmp;

	ompcp = (OMProcControlPtr)data;
	vsmp = VSeqMgrGet();
	omp = vsmp->omp;

	filename[0] = '\0';
	if (GetInputFileName(filename, (size_t)254, NULL, NULL))
	{
		vrb.do_it = FALSE;
		vrb.window_done = FALSE;
		vrb.the_type = 0;
		w = ModalWindow (-50, -33, -10, -10, NULL);
		SetWindowExtra(w, &vrb, NULL);
		g = HiddenGroup(w, 0, 2, NULL);
		StaticPrompt(g, "Select ASN.1 type:", 0,0,systemFont,'l');
		vrb.p = PopupList(g, TRUE, NULL);
		i = 0;
		ct = 0;
		omtp = NULL;
		while ((omtp = ObjMgrTypeFindNext(omp, omtp)) != NULL)
		{
			if (omtp->asnname != NULL)
			{
				i++;
				PopupItem(vrb.p, omtp->asnname);
				if (! StringCmp(vsmp->lastASNtype, omtp->asnname))
					ct = i;
			}
		}
		
		if (! i)
		{
			ErrPostEx(SEV_ERROR,0,0, "No ASN.1 types are registered");
			Remove(w);
			return OM_MSG_RET_ERROR;
		}
		
		if (! ct)
			ct = 1;
		SetValue(vrb.p, ct);
		
		g = HiddenGroup(w, 2, 0, NULL);
		DefaultButton(g, "Accept", AsnBinAcceptProc);
		b = PushButton(g, "Cancel", AsnBinCancelProc);
		
		Show(w);
		Nlm_WaitForCondition (! vrb.window_done);
		ProcessAnEvent();
		
		if (! vrb.do_it)
			return retval;
		
		i = 0;
		omtp = NULL;
		while ((omtp = ObjMgrTypeFindNext(omp, omtp)) != NULL)
		{
			i++;
			if (i == vrb.the_type)
				break;
		}
		
		if (omtp == NULL)
		{
			ErrPostEx(SEV_ERROR,0,0,"Couldn't find vrb.the_type");
			return OM_MSG_RET_ERROR;
		}
		
		StringMove(vsmp->lastASNtype, omtp->asnname);	
		WatchCursor();
		aip = AsnIoOpen(filename, "rb");
		ptr = (*(omtp->asnread))(aip, NULL);
		AsnIoClose(aip);
		if (ptr == NULL)
		{
			ErrPostEx(SEV_ERROR,0,0,"Couldn't read [%s], type [%s]", filename, omtp->asnname);
			retval = OM_MSG_RET_ERROR;
		}
		else
		{
			entityID = ObjMgrRegister(omtp->datatype, ptr);
			ompcp->output_data = ptr;
			ompcp->output_entityID = entityID;
			PromoteToSeqEntry (entityID, omtp->datatype, ptr);

			retval = OM_MSG_RET_DONE;
		}
		ArrowCursor();
	}
	else
		retval = OM_MSG_RET_OK;

	return retval;
}

static Int2 LIBCALLBACK VSMGenericFastaOpen ( Boolean is_na )
{
	Char filename[255];
	FILE * fp;
	SeqEntryPtr sep;
	
	filename[0] = '\0';
	if (GetInputFileName(filename, (size_t)254, NULL, NULL))
	{
		WatchCursor();
		fp = FileOpen(filename, "r");
		while ((sep = FastaToSeqEntry(fp, is_na)) != NULL)
			ObjMgrRegister(OBJ_SEQENTRY, (Pointer)sep);
		FileClose(fp);
		ArrowCursor();
	}
	
	return OM_MSG_RET_DONE;
}

Int2 LIBCALLBACK VSMFastaProtOpen ( Pointer data )
{
	return VSMGenericFastaOpen(FALSE);
}

Int2 LIBCALLBACK VSMFastaNucOpen ( Pointer data )
{
	return VSMGenericFastaOpen(TRUE);
}


static Int2 LIBCALLBACK VSMGenericFastaSave (OMProcControlPtr ompcp, Boolean is_na )
{
	Char filename[255];
	FILE * fp;
	ValNode vn;
	SeqEntryPtr sep = NULL;
	SeqFeatPtr sfp;
	SeqLocPtr slp;
	SeqPortPtr spp;
	Uint1 code;
	Char buf[255];
	BioseqPtr bsp;
	
	sfp = NULL;
	switch(ompcp->input_itemtype)
	{
		case OBJ_SEQENTRY:
		case OBJ_BIOSEQ:
		case OBJ_BIOSEQSET:
			break;
		case OBJ_SEQFEAT:
			sfp = (SeqFeatPtr) ompcp->input_data;
			if (sfp == NULL) return OM_MSG_RET_ERROR;
			break;
		default:
			ErrPostEx(SEV_ERROR, 0,0,"ToFasta: Can only write Seq-entry, Bioseq, or Bioseq-set");
			return OM_MSG_RET_ERROR;
	}
	if (sfp != NULL) {
	} else if (ompcp->input_choicetype == OBJ_SEQENTRY)
		sep = (SeqEntryPtr)(ompcp->input_choice);
	else
	{
		vn.next = NULL;
		vn.data.ptrvalue = ompcp->input_data;
		if (ompcp->input_itemtype == OBJ_BIOSEQ)
			vn.choice = 1;
		else
			vn.choice = 2;
		sep = &vn;
	}
		
	filename[0] = '\0';
	if (GetOutputFileName(filename, (size_t)254, NULL))
	{
		WatchCursor();
#ifdef WIN_MAC
		fp = FileOpen (filename, "r");
		if (fp != NULL) {
			FileClose (fp);
		} else {
			FileCreate (filename, "TEXT", "ttxt");
		}
#endif
		fp = FileOpen(filename, "w");
		if (sfp != NULL) {
			if (is_na)
				code = Seq_code_iupacna;
			else
				code = Seq_code_ncbieaa;
			slp = sfp->location;
			if (sfp->data.choice == SEQFEAT_CDREGION && (! is_na)) {
			  slp = sfp->product;
			}
			spp = SeqPortNewByLoc (slp, code);
			bsp = GetBioseqGivenSeqLoc (slp, ompcp->input_entityID);
			if (spp != NULL && bsp != NULL) {
				while (FastaSeqLine(spp, buf, 70, is_na))
					FastaFileFunc(bsp, FASTA_SEQLINE, buf, sizeof (buf), (Pointer)fp);
				SeqPortFree(spp);
				FastaFileFunc(bsp, FASTA_EOS, buf, sizeof (buf), (Pointer)fp);
			}
		} else {
			SeqEntryToFasta(sep, fp, is_na);
		}
		FileClose(fp);
		ArrowCursor();
	}
	
	return OM_MSG_RET_DONE;
}

Int2 LIBCALLBACK VSMFastaProtSave ( Pointer data )
{
	return VSMGenericFastaSave((OMProcControlPtr)data, FALSE);
}

Int2 LIBCALLBACK VSMFastaNucSave ( Pointer data )
{
	return VSMGenericFastaSave((OMProcControlPtr)data, TRUE);
}


static Int2 LIBCALLBACK VSMGenericAsnSave (OMProcControlPtr ompcp, CharPtr mode )
{
	Char filename[255];
	AsnIoPtr aip;
	ObjMgrPtr omp;
	ObjMgrTypePtr omtp;
	Uint2 the_type;
	Pointer the_data;
#ifdef WIN_MAC
	FILE * fp;
#endif

	omp = ObjMgrGet();
	if (ompcp->input_choicetype)
	{
		the_type = ompcp->input_choicetype;
		the_data = (Pointer)(ompcp->input_choice);
	}
	else
	{
		the_type = ompcp->input_itemtype;
		the_data = ompcp->input_data;
	}
	
	omtp = ObjMgrTypeFind(omp, the_type, NULL, NULL);
	if (omtp == NULL)
	{
		ErrPostEx(SEV_ERROR,0,0,"Can't locate type record for [%d]", (int)the_type);
		return OM_MSG_RET_ERROR;
	}	
		
	filename[0] = '\0';
	if (GetOutputFileName(filename, (size_t)254, NULL))
	{
		WatchCursor();
#ifdef WIN_MAC
		fp = FileOpen (filename, "r");
		if (fp != NULL) {
			FileClose (fp);
		} else {
			FileCreate (filename, "TEXT", "ttxt");
		}
#endif
		aip = AsnIoOpen(filename, mode);
		(*(omtp->asnwrite))(the_data, aip, NULL);
		AsnIoClose(aip);
		ArrowCursor();
	}
	
	return OM_MSG_RET_DONE;
}

Int2 LIBCALLBACK VSMGenericTextAsnSave ( Pointer data )
{
	return VSMGenericAsnSave((OMProcControlPtr)data, "w");
}

Int2 LIBCALLBACK VSMGenericBinAsnSave ( Pointer data )
{
	return VSMGenericAsnSave((OMProcControlPtr)data, "wb");
}


