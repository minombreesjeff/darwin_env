/*  edutil.c
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
* File Name:  edutil.c
*
* Author:  James Ostell
*   
* Version Creation Date: 2/4/94
*
* $Revision: 6.21 $
*
* File Description:  Sequence editing utilities
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: edutil.c,v $
* Revision 6.21  2003/11/03 19:37:42  bollin
* SegLocToPartsEx now handles SEQLOC_PNT as well as SEQLOC_INT
*
* Revision 6.20  2003/06/03 20:25:34  kans
* SeqLocReplaceID works on bonds if both ends bonded to the same Seq-id
*
* Revision 6.19  2003/02/10 22:57:45  kans
* added BioseqCopyEx, which takes a BioseqPtr instead of a SeqIdPtr for the source
*
* Revision 6.18  2002/07/26 20:15:55  kans
* BioseqInsert can do feature indexed collection of features to adjust
*
* Revision 6.17  2002/07/17 15:39:40  kans
* BioseqInsert calls Nlm_BSAdd, need to figure out when not to call
*
* Revision 6.16  2002/07/11 17:45:53  kans
* BioseqInsert does not call Nlm_BSAdd due to a bug in that code
*
* Revision 6.15  2002/07/02 13:23:42  kans
* added SeqLocDeleteEx
*
* Revision 6.14  2001/06/01 18:07:20  kans
* changes to SeqLocAdd to allow one plus and one unknown strand to be accepted
*
* Revision 6.13  2001/02/23 21:30:09  shkeda
* Fixed SeqLocAdd: Int-fuzz pointers should be set to NULL after IntFuzzFree
*
* Revision 6.12  2001/02/23 01:26:07  ostell
* Added support to BioseqDelete() for delta seqs
*
* Revision 6.11  2000/10/31 17:11:06  kans
* SeqLocReplaceID was handling SEQLOC_PACKED_PNT incorrectly
*
* Revision 6.10  1999/12/20 20:47:12  kans
* oldscope test was wrong everywhere
*
* Revision 6.9  1999/12/15 20:52:16  kans
* added IndexedSeqFeatsCopy if SeqMgrFeaturesAreIndexed
*
* Revision 6.8  1999/12/07 20:32:13  kans
* for most editing functions, if BioseqFind failed, temporarily clear scope/try again/reset scope
*
* Revision 6.7  1999/11/19 19:54:19  kans
* SeqLocAdd checks for NULL slp before dereferencing
*
* Revision 6.6  1998/09/03 20:43:52  kans
* added delta bioseq support to BioseqCopy
*
* Revision 6.5  1998/06/22 20:00:46  kans
* DelFeat was a bit too agressive when there were multiple feature tables
*
* Revision 6.4  1998/06/17 21:50:11  kans
* fixed unix compiler warnings, including 64-bit SGI
*
* Revision 6.3  1997/11/10 19:40:48  bazhin
* Fixed incorrect comment for ISAGappedSeqLoc() function.
*
* Revision 6.2  1997/10/24 19:16:17  bazhin
* Added three easy functions GapToSeqLoc(...), ISAGappedSeqLoc(...)
* and GappedSeqLocsToDeltaSeqs(...) for processing "gap(...)" tokens
* in CONTIG line.
*
* Revision 6.1  1997/10/10 20:18:02  ostell
* removed tab character from SeqLitTag for DeltaSeqsToSeqLoc
*
* Revision 6.0  1997/08/25 18:05:24  madden
* Revision changed to 6.0
*
* Revision 5.10  1997/07/25 20:34:51  kans
* added SegLocToPartsEx
*
* Revision 5.9  1997/06/19 18:37:30  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.8  1996/12/20 17:59:34  kans
* SeqLocCopyRegion already reversed order for Seq_strand_minus, so no need
* to reverse it again (JO + JK)
*
 * Revision 5.7  1996/10/21  18:56:19  ostell
 * made SegLocToParts accept a complicated Seq-loc argument
 *
 * Revision 5.6  1996/10/09  17:27:34  chappey
 * *** empty log message ***
 *
 * Revision 5.5  1996/10/09  16:34:59  chappey
 * added SeqLocReplaceID() that replaces the Seq-Id of a Seq-Loc
 *
 * Revision 5.4  1996/07/15  14:43:51  epstein
 * change SeqLocAdd() so that it merges identical SEQLOC_PNTs
 *
 * Revision 5.3  1996/06/12  18:29:41  epstein
 * move SeqLocIntNew() and SeqLocPntNew() from edutil to sequtil
 *
 * Revision 5.1  1996/06/10  15:07:17  epstein
 * replace make_seq_loc() with SeqLocIntNew() and make_pnt_loc with SeqLocPntNew()
 *
 * Revision 5.0  1996/05/28  13:23:23  ostell
 * Set to revision 5.0
 *
 * Revision 4.10  1996/03/19  19:45:24  kans
 * fix of SegLocToParts (JO)
 *
 * Revision 4.9  1996/03/12  22:14:22  ostell
 * added SeqLocToParts()
 *
 * Revision 4.7  1996/02/19  19:58:05  ostell
 * added support for Code-break and tRNA.anticodon
 *
 * Revision 4.6  1996/01/30  16:24:04  ostell
 * changed name of SeqLocPack() to SeqLocPackage()
 *
 * Revision 4.5  1996/01/29  22:03:52  ostell
 * revised SeqLocAdd
 * added SeqLocPack
 *
 * Revision 4.4  1996/01/10  22:25:25  ostell
 * added SeqLocIntNew()
 *
 * Revision 4.3  1995/12/29  21:31:44  ostell
 * added mapping functions between delta seq and seq loc, for editing utilities
 *
 * Revision 4.2  1995/12/21  02:35:50  ostell
 * changed call for BSAdd
 *
 * Revision 4.1  1995/11/15  20:40:20  ostell
 * fixed SeqLocCopyPart so it correctly handles SEQLOC_NULL in segmented
 * records
 *
 * Revision 4.0  1995/07/26  13:49:01  ostell
 * force revision to 4.0
 *
 * Revision 1.22  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

#include <edutil.h>
#include <explore.h>



/*****************************************************************************
*
*   SeqLocPackage(head)
*     head is a chain of 1 or more SeqLocs connected by slp->next
*     Assumes was built by SeqLocAdd to remove redundancy
*     Frees the last element if it is a NULL.
*     If more than one element left, then packages the chain into a SEQLOC_MIX,
*       or SEQLOC_PACKED_INT as appropriate
*     returns pointer to the head of the resulting single SeqLoc
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL SeqLocPackage (SeqLocPtr head)
{
	SeqLocPtr newhead = NULL, tmp, prev;
	Boolean packed_int = TRUE;
	Int4 ctr = 0;

	if (head == NULL) return head;

	prev = NULL;    /* remove trailing NULL */
	for (tmp = head; tmp->next != NULL; tmp = tmp->next)
		prev = tmp;

	if (tmp->choice == SEQLOC_NULL)
	{
		SeqLocFree(tmp);
		if (prev != NULL)
			prev->next = NULL;
		else
			return NULL;   /* nothing left */
	}

	for (tmp = head; tmp != NULL; tmp = tmp->next)
	{
		ctr++;
		if (tmp->choice != SEQLOC_INT)
			packed_int = FALSE;
	}

	if (ctr == 1)
		return head;

	newhead = ValNodeNew(NULL);
	if (packed_int)
		newhead->choice = SEQLOC_PACKED_INT;
	else
		newhead->choice = SEQLOC_MIX;
	newhead->data.ptrvalue = head;

	return newhead;
}

/*****************************************************************************
*
*   SeqLocAdd(headptr, slp, merge, do_copy)
*   	creates a linked list of SeqLocs.
*       returns a pointer to the last SeqLoc in the chain
*       if (merge)
*   	  deletes double NULLs or Nulls at start (application must delete at stop)
*         merges adjacent intervals on the same strand
*       if (do_copy)
*   	  Makes copies of incoming SeqLocs
*         if incoming is merged, deletes the incoming SeqLoc
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL SeqLocAdd (SeqLocPtr PNTR head, SeqLocPtr slp, Boolean merge, Boolean do_copy)
{
	SeqLocPtr tmp, last, retval = NULL;
	Boolean merged = FALSE;   /* intervals were merged */

	if (slp == NULL) return NULL;

	last = NULL;
	if (* head != NULL)
	{
		for (tmp = *head; tmp != NULL; tmp = tmp->next)
		{
			last = tmp;
		}
	}

	if ((slp->choice == SEQLOC_NULL) && (merge))  /* no null at start, or two in a row */
	{
		if (last == NULL)  /* first one */
		{
			merged = TRUE;
			goto ret;
		}
		if (last->choice == SEQLOC_NULL)  /* double NULL */
		{
			merged = TRUE;
			goto ret;
		}
	}

	if ((last != NULL) && (merge))     /* check for merging intervals */
	{
		if ((last->choice == SEQLOC_INT) && (slp->choice == SEQLOC_INT))
		{
			SeqIntPtr sip1, sip2;
			Boolean samestrand;
			Uint1 strand = Seq_strand_unknown;

			sip1 = (SeqIntPtr)(last->data.ptrvalue);
			sip2 = (SeqIntPtr)(slp->data.ptrvalue);
			samestrand = FALSE;
			if ((sip1->strand == sip2->strand) ||
				(sip1->strand == Seq_strand_unknown && sip2->strand != Seq_strand_minus) ||
          		(sip1->strand == Seq_strand_unknown && sip2->strand != Seq_strand_minus)) {
				samestrand = TRUE;
				if (sip1->strand == Seq_strand_minus || sip1->strand == Seq_strand_minus) {
					strand = Seq_strand_minus;
				} else if (sip1->strand == Seq_strand_plus || sip1->strand == Seq_strand_plus) {
					strand = Seq_strand_plus;
				} else {
					strand = Seq_strand_unknown;
				}
          	}
			if (samestrand && (SeqIdForSameBioseq(sip1->id, sip2->id)))
			{
				if (strand == Seq_strand_minus)
				{
					if (sip1->from == (sip2->to + 1))  /* they are adjacent */
					{
						sip1->from = sip2->from;
						sip1->if_from = IntFuzzFree(sip1->if_from);
						if (sip2->if_from != NULL)   /* copy the fuzz */
						{
							if (do_copy)
								sip1->if_from = (IntFuzzPtr)AsnIoMemCopy((Pointer)(sip2->if_from),
								    (AsnReadFunc)IntFuzzAsnRead, (AsnWriteFunc)IntFuzzAsnWrite);
							else
							{
								sip1->if_from = sip2->if_from;
								sip2->if_from = NULL;
							}
							sip1->strand = strand;
						}
						merged = TRUE;
					}
				}
				else
				{
					if (sip1->to == (sip2->from - 1))  /* they are adjacent */
					{
						sip1->to = sip2->to;
						sip1->if_to = IntFuzzFree(sip1->if_to);
						if (sip2->if_to != NULL)   /* copy the fuzz */
						{
							if (do_copy)
								sip1->if_to = (IntFuzzPtr)AsnIoMemCopy((Pointer)(sip2->if_to),
								    (AsnReadFunc)IntFuzzAsnRead, (AsnWriteFunc)IntFuzzAsnWrite);
							else
							{
								sip1->if_to = sip2->if_to;
								sip2->if_to = NULL;
							}
							sip1->strand = strand;
						}
						merged = TRUE;
					}
				}
			}
		} else if ((last->choice == SEQLOC_PNT) && (slp->choice == SEQLOC_PNT))
		{
			SeqPntPtr sip1, sip2;

			sip1 = (SeqPntPtr)(last->data.ptrvalue);
			sip2 = (SeqPntPtr)(slp->data.ptrvalue);
			if ((sip1->strand == sip2->strand) && sip1->point == sip2->point && (SeqIdForSameBioseq(sip1->id, sip2->id)))
			{
				sip1->fuzz = IntFuzzFree(sip1->fuzz);
				if (sip2->fuzz != NULL)   /* copy the fuzz */
				{
					if (do_copy)
						sip1->fuzz = (IntFuzzPtr)AsnIoMemCopy((Pointer)(sip2->fuzz),
						    (AsnReadFunc)IntFuzzAsnRead, (AsnWriteFunc)IntFuzzAsnWrite);
					else
					{
						sip1->fuzz = sip2->fuzz;
						sip2->fuzz = NULL;
					}
				}
				merged = TRUE;
			}
		}
	}

ret:
	if (! merged)  /* then have to add a new one */
	{
		if (do_copy)
			tmp = (SeqLocPtr)AsnIoMemCopy((Pointer)slp, (AsnReadFunc)SeqLocAsnRead, (AsnWriteFunc)SeqLocAsnWrite);
		else
			tmp = slp;

		tmp->next = NULL;

		if (last != NULL)
			last->next = tmp;
		else
			*head = tmp;
		retval = tmp;
	}
	else
	{
		retval = last;
		if (! do_copy)   /* got to free it here */
			SeqLocFree(slp);
	}
		
	return retval;
}

/*****************************************************************************
*
*   SegLocToParts(BioseqPtr seg, SeqLocPtr slp)
*   	seg must be a segmented Bioseq
*       slp must be a SeqLoc on it
*       function maps slp to the components of seg
*       returns a new SeqLocPtr
*       does not delete slp
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL SegLocToPartsEx (BioseqPtr seg, SeqLocPtr slp, Boolean nullsBetween)
{
	SeqLocPtr newloc = NULL, tmp, tmp2, tmp3, next, curr;
	ValNode thead;
	SeqIdPtr sip, tsip;
	Int4 left_end, right_end, tlen, tstart;
	SeqIntPtr sintp;
	Boolean split, notFirst = FALSE;

	if ((seg == NULL) || (slp == NULL)) return newloc;
	if (seg->repr != Seq_repr_seg) return newloc;

	sip = SeqLocId(slp);
	if (sip == NULL) return newloc;
	if (! SeqIdIn(sip, seg->id)) return newloc;

	MemSet(&thead, 0, sizeof(ValNode));
	thead.choice = SEQLOC_MIX;
	thead.data.ptrvalue = seg->seq_ext;

	curr = NULL;
	while ((curr = SeqLocFindNext(slp, curr)) != NULL)
	{
		left_end = 0;
		tmp = NULL;
		while ((tmp = SeqLocFindNext(&thead, tmp)) != NULL)
		{
			tlen = SeqLocLen(tmp);
			if (tlen > 0)
			{
				right_end = left_end + tlen - 1;
				tsip = SeqLocId(tmp);
				tstart = SeqLocStart(tmp);
				tmp2 = SeqLocCopyRegion(tsip, curr, seg, left_end, right_end, SeqLocStrand(tmp),
					&split);
				while (tmp2 != NULL)
				{
				  next = tmp2->next;
				  tmp2->next = NULL;
				  if (tmp2->choice == SEQLOC_INT)
				  {
				    if (nullsBetween  && notFirst) {
				      tmp3 = ValNodeNew (NULL);
				      if (tmp3 != NULL) {
				        tmp3->choice = SEQLOC_NULL;
				        SeqLocAdd (&newloc, tmp3, TRUE, FALSE);
				      }
				    }
				    notFirst = TRUE;
				    sintp = (SeqIntPtr)(tmp2->data.ptrvalue);
				    sintp->from += tstart;
				    sintp->to += tstart;
				    SeqLocAdd(&newloc, tmp2, TRUE, FALSE);
				  }
                                  else if (tmp2->choice == SEQLOC_PNT)
                                  {
				    if (nullsBetween  && notFirst) {
				      tmp3 = ValNodeNew (NULL);
				      if (tmp3 != NULL) {
				        tmp3->choice = SEQLOC_NULL;
				        SeqLocAdd (&newloc, tmp3, TRUE, FALSE);
				      }
				    }
				    notFirst = TRUE;
                                    SeqLocAdd (&newloc, tmp2, TRUE, FALSE);
                                  }
				  tmp2 = next;
				}
				left_end = right_end + 1;
			}
		}
	}

	if (newloc != NULL)
		newloc = SeqLocPackage(newloc);
	return newloc;
}

NLM_EXTERN SeqLocPtr LIBCALL SegLocToParts (BioseqPtr seg, SeqLocPtr slp)

{
	return SegLocToPartsEx (seg, slp, FALSE);
}

static CharPtr seqlitdbtag = "SeqLit";
/*****************************************************************************
*
*   ISADeltaSeqsToSeqLoc(slp)
*   	returns Index (> 0) if this (one) SeqLoc was converted from a Delta Seq by
*         DeltaSeqsToSeqLocs() by looking for the special Dbtag name
*
*****************************************************************************/
NLM_EXTERN Int4 LIBCALL ISADeltaSeqsToSeqLoc (SeqLocPtr slp)
{
	SeqIdPtr sip;
	Int4 retval = 0;

	if (slp == NULL) return retval;
	sip = SeqLocId(slp);
	if (sip == NULL) return retval;

	if (sip->choice != SEQID_GENERAL) return retval;

	if (! StringCmp(seqlitdbtag, ((DbtagPtr)(sip->data.ptrvalue))->db))
		retval = (((DbtagPtr)(sip->data.ptrvalue))->tag->id);

	return retval;
}

/*****************************************************************************
*
*   DeltaSeqsToSeqLocs(dsp)
*   	converts a chain of delta seqs to seqlocs
*   	each SeqLit is converted to SeqLoc of type Int with a SeqId of type
*          Dbtag where db="Seq\tLit" and objectId.id which is the index of the
*          element in the delta seq chain where 1 is the first one.
*   	Returned SeqLoc is of type "mix" and must be freed by caller.
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL DeltaSeqsToSeqLocs (DeltaSeqPtr dsp)
{
	SeqLocPtr head = NULL, thead=NULL;
	DeltaSeqPtr curr;
	SeqInt si;
	Dbtag db;
	ObjectId oi;
	ValNode vn, vn2;

	MemSet(&vn, 0, sizeof(ValNode));
	MemSet(&vn2, 0, sizeof(ValNode));
	MemSet(&si, 0, sizeof(SeqInt));
	MemSet(&db, 0, sizeof(Dbtag));
	MemSet(&oi, 0, sizeof(ObjectId));
	vn.choice = SEQLOC_INT;
	vn.data.ptrvalue = &si;
	si.id = &vn2;
	vn2.choice = SEQID_GENERAL;
	vn2.data.ptrvalue = &db;
	db.db = seqlitdbtag;
	db.tag = &oi;
	oi.id = 1;

	
	for (curr = dsp; curr != NULL; curr = curr->next)
	{
		if (curr->choice == 1)   /* a SeqLoc */
			SeqLocAdd(&thead, (SeqLocPtr)(curr->data.ptrvalue), TRUE, TRUE);
		else
		{
			si.to = ((SeqLitPtr)(curr->data.ptrvalue))->length - 1;
			SeqLocAdd(&thead, &vn, TRUE, TRUE); 
		}
		oi.id++;
	}

	head = SeqLocPackage(thead);
	return head;
}

/*****************************************************************************
* GOHERE
*   SeqLocsToDeltaSeqs(dsp, slp)
*   	converts a chain of seqlocs	generated by DeltaSeqToSeqLocs() back into
*         delta seqs. dsp is the original chain of DeltaSeqs, which is required
*         to convert the delta seqs back.
*
*****************************************************************************/
NLM_EXTERN DeltaSeqPtr LIBCALL SeqLocsToDeltaSeqs (DeltaSeqPtr dsp, SeqLocPtr slp)
{
	DeltaSeqPtr dhead=NULL, dcurr=NULL, dtmp;
	SeqLocPtr scurr;
	Int4 ctr, index, strt, stp;
	SeqIdPtr sip;
	Uint1 strand, newcode;
	SeqLitPtr slitp, slitp_new;
	SeqPortPtr spps;
	ByteStorePtr bsp;
	Int2 residue;
	ValNode vn;

	if ((dsp == NULL) || (slp == NULL))
		return dhead;

	vn.choice = SEQLOC_MIX;
	vn.next = NULL;
	vn.data.ptrvalue = slp;
	scurr = NULL;
	while ((scurr = SeqLocFindNext(&vn, scurr)) != NULL)
	{
		dcurr = ValNodeNew(dhead);
		if (dhead == NULL)
			dhead = dcurr;

		index = ISADeltaSeqsToSeqLoc(scurr);

		if (index == 0)   /* just a SeqLoc */
		{
			dcurr->choice = 1;
			dcurr->data.ptrvalue = NULL;
			dcurr->data.ptrvalue = AsnIoMemCopy((Pointer)scurr, (AsnReadFunc)SeqLocAsnRead, (AsnWriteFunc)SeqLocAsnWrite);

		}
		else                                 /* convert to a delta seq */
		{
			dcurr->choice = 2;
			sip = SeqLocId(scurr);
			dtmp = dsp;
			for (ctr = 1; ctr < index; ctr++)
				dtmp = dtmp->next;

			if (dtmp->choice != 2)   /* wups */
			{
				ErrPostEx(SEV_ERROR,0,0,"Wrong type in SeqLocsToDeltaSeqs");
				dhead = DeltaSeqFree(dhead);
				return dhead;
			}
			slitp = (SeqLitPtr)(dtmp->data.ptrvalue);

			strt = SeqLocStart(scurr);
			stp = SeqLocStop(scurr);
			strand = SeqLocStrand(scurr);

			if ((strt == 0) && (stp == (slitp->length - 1)) && (strand != Seq_strand_minus))  /* no change */
			{
				dcurr->data.ptrvalue = AsnIoMemCopy((Pointer)slitp, (AsnReadFunc)SeqLitAsnRead, (AsnWriteFunc)SeqLitAsnWrite);
			}
			else   /* got to copy part of it */
			{
				switch (slitp->seq_data_type)
				{
					case Seq_code_iupacna:
					case Seq_code_iupacaa:
					case Seq_code_ncbi8na:
					case Seq_code_ncbi8aa:
					case Seq_code_ncbieaa:
					case Seq_code_ncbistdaa:
					case Seq_code_iupacaa3:
						newcode = slitp->seq_data_type;     /* one byte codes.. fine */
						break;
					case Seq_code_ncbipna:
						ErrPostEx(SEV_ERROR,0,0,"Converting from P residue codes");
						newcode = Seq_code_ncbieaa;
						break;
					case Seq_code_ncbipaa:
						ErrPostEx(SEV_ERROR,0,0,"Converting from P residue codes");
					case Seq_code_ncbi2na:
					case Seq_code_ncbi4na:
						newcode = Seq_code_iupacna;
						break;
					default:
						ErrPostEx(SEV_FATAL,0,0,"Unrecognized residue code [%d] in SeqLocsToDeltaSeqs",
							(int)(slitp->seq_data_type));
						return DeltaSeqFree(dhead);
				}
	 			spps = MemNew(sizeof(SeqPort));
				SeqPortSetUpFields (spps, strt, stp, strand, newcode);
				SeqPortSetUpAlphabet(spps, slitp->seq_data_type, newcode);
				spps->bp = slitp->seq_data;
				slitp_new = SeqLitNew();
				dcurr->data.ptrvalue = slitp_new;
				slitp_new->seq_data_type = newcode;
				slitp_new->length = (stp - strt + 1);
				bsp = BSNew(slitp_new->length);
				slitp_new->seq_data = bsp;
				SeqPortSeek(spps, 0, SEEK_SET);
				BSSeek(bsp, 0, SEEK_SET);
			    while (stp >= strt)
				{
					residue = SeqPortGetResidue(spps);
					BSPutByte(bsp, residue);
					strt++;
				}
				SeqPortFree(spps);
			}

		}

	}
	return dhead;
}
/*****************************************************************************
*
*   BioseqDelete (target, from, to, do_feat, do_split)
*      Deletes the region of sequence between from-to, inclusive, on the
*        Bioseq whose SeqId is target.
*      If do_feat, the feature table is updated to reflect the deletion
*        using SeqEntryDelFeat()
*      If do_split, the features across the deleted region are split into
*        two intervals on either side. If not, the feature is just shortened.
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL BioseqDelete (SeqIdPtr target, Int4 from, Int4 to, Boolean do_feat, Boolean do_split)
{
	Boolean retval = FALSE;
	BioseqPtr bsp;
	SeqLocPtr tmp, head;
	Int4 len, deleted;
	Int4 totlen, templen, tfrom, tto, diff1, diff2;
	SeqLocPtr slp, tloc, newhead, prev;
	ValNode vn;
	SeqInt si;
	SeqLocPtr PNTR newheadptr;
	SeqFeatPtr sfpcurr, sfpnext, sfpprev;
	Int2 dropped;
	SeqEntryPtr oldscope;
	DeltaSeqPtr tdsp;

	bsp = BioseqFind(target);
	if (bsp == NULL) {
		oldscope = SeqEntrySetScope (NULL);
		if (oldscope != NULL) {
			bsp = BioseqFind(target);
			SeqEntrySetScope (oldscope);
		}
	}
	if (bsp == NULL) return retval;

	if ((from < 0) || (from >= bsp->length) || (to < 0) ||
		(to >= bsp->length) || (from > to)) return retval;

	if (do_feat)
		SeqEntryDelFeat(NULL, target, from, to, do_split);

	len = to - from + 1;
	           /* if actual sequence present */

	if ((bsp->repr == Seq_repr_raw) || (bsp->repr == Seq_repr_const))
	{
		if (ISA_na(bsp->mol))
		{
			if (bsp->seq_data_type != Seq_code_iupacna)  /* need 1 byte/base */
				BioseqRawConvert(bsp, Seq_code_iupacna);
		}
		else
		{
			if (bsp->seq_data_type != Seq_code_ncbieaa)
				BioseqRawConvert(bsp, Seq_code_ncbieaa);
		}

		BSSeek(bsp->seq_data, from, SEEK_SET);
		deleted = BSDelete(bsp->seq_data, len);
		if (deleted != len)  /* error */
			ErrPost(CTX_NCBIOBJ, 1, "Delete of %ld residues failed", len);
		else
			retval = TRUE;
	}

			   /* update segmented sequence */
	if ((bsp->repr == Seq_repr_seg) || (bsp->repr == Seq_repr_delta))
	{
		head = ValNodeNew(NULL);  /* allocate to facilitate SeqLocFree */
		head->choice = SEQLOC_MIX;   /* make a SeqLoc out of the extension */
		if (bsp->repr == Seq_repr_seg)
			head->data.ptrvalue = bsp->seq_ext;
		else
		{
			tdsp = (DeltaSeqPtr)(bsp->seq_ext);
			head->data.ptrvalue = DeltaSeqsToSeqLocs(tdsp);
		}
		
		newhead = NULL;
		newheadptr = &newhead;

		tloc = &vn;
		MemSet((Pointer)tloc, 0, sizeof(ValNode));
		MemSet((Pointer)&si, 0, sizeof(SeqInt));
		tloc->choice = SEQLOC_INT;
		tloc->data.ptrvalue = (Pointer)(&si);
		
		slp = NULL;
		totlen = 0;
		while ((slp = SeqLocFindNext(head, slp)) != NULL)
		{
			templen = SeqLocLen(slp);
		    tfrom = SeqLocStart(slp);
			tto = SeqLocStop(slp);
			
			if (((totlen + templen - 1) < from) ||   /* before cut */
				(totlen > to))						  /* after cut */
				tmp = SeqLocAdd(newheadptr, slp, TRUE, TRUE); /* add whole SeqLoc */
			else                            
			{
				retval = 1;    /* will modify or drop interval */
		 		diff1 = from - totlen;        /* partial beginning? */
				diff2 = (templen + totlen - 1) - to;  /* partial end? */
				si.id = SeqLocId(slp);
				si.strand = SeqLocStrand(slp);
				
				if (diff1 > 0)	  /* partial start */
				{
					if (si.strand != Seq_strand_minus)
					{
					   si.from = tfrom;
					   si.to = tfrom + diff1 - 1;
					}
					else
					{
						si.from = tto - diff1 + 1;
						si.to = tto;
					}
					tmp = SeqLocAdd(newheadptr, tloc, TRUE, TRUE);
				}

				if (diff2 > 0)    /* partial end */
				{
					if (si.strand != Seq_strand_minus)
					{
					   si.from = tto - diff2 + 1;
					   si.to = tto;
					}
					else
					{
						si.from = tfrom;
						si.to = tfrom + diff2 - 1;
					}
					tmp = SeqLocAdd(newheadptr, tloc, TRUE, TRUE);
				}
				
			}
			totlen += templen;
		}

		prev = NULL;
		for (tmp = newhead; tmp != NULL; tmp = tmp->next)
		{
			if (tmp->next == NULL)   /* last one */
			{
				if (tmp->choice == SEQLOC_NULL)
				{
					if (prev != NULL)
						prev->next = NULL;
					else				  /* only a NULL left */
					{
						newhead = NULL;
					}
					MemFree(tmp);
					break;
				}
			}
			prev = tmp;
		}

		if (bsp->repr == Seq_repr_seg)
			bsp->seq_ext = newhead;
		else
		{
			bsp->seq_ext = SeqLocsToDeltaSeqs(tdsp, newhead);
			DeltaSeqSetFree(tdsp);
			SeqLocSetFree(newhead);
		}
		SeqLocFree(head);
		retval = TRUE;
	}

	if (bsp->repr == Seq_repr_map)      /* map bioseq */
	{
		sfpprev = NULL;
		sfpnext = NULL;
		sfpcurr = (SeqFeatPtr)(bsp->seq_ext);
		bsp->seq_ext = NULL;
		for (; sfpcurr != NULL; sfpcurr = sfpnext)
		{
			sfpnext = sfpcurr->next;
			dropped = SeqFeatDelete(sfpcurr, target, from, to, TRUE);
			if (dropped == 2)   /* completely gone */
			{
				SeqFeatFree(sfpcurr);
			}
			else
			{
				if (sfpprev == NULL)
					bsp->seq_ext = (Pointer)sfpcurr;
				else
					sfpprev->next = sfpcurr;
				sfpcurr->next = NULL;
				sfpprev = sfpcurr;
			}
		}
		retval = TRUE;
	}

	if (bsp->repr == Seq_repr_virtual)
		retval = TRUE;                 /* nothing to do */

	if (retval)
		bsp->length -= len;
	return retval;
}


/*****************************************************************************
*
*   BioseqOverwrite (target, pos, residue, seqcode)
*      Overwrites the residue at pos with residue in the
*        Bioseq whose SeqId is target.
*      residue is iupacna for DNA or ncbieaa for protein
*      target MUST be a raw Bioseq right now
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL BioseqOverwrite (SeqIdPtr target, Int4 pos, Uint1 residue)
{
	BioseqPtr bsp;
	Boolean retval = FALSE;
	SeqEntryPtr oldscope;


	bsp = BioseqFind(target);
	if (bsp == NULL) {
		oldscope = SeqEntrySetScope (NULL);
		if (oldscope != NULL) {
			bsp = BioseqFind(target);
			SeqEntrySetScope (oldscope);
		}
	}
	if (bsp == NULL) return retval;

	if ((pos < 0) || (pos >= bsp->length)) return retval;
	if (bsp->repr != Seq_repr_raw) return retval;

	if (ISA_na(bsp->mol))
	{
		if (bsp->seq_data_type != Seq_code_iupacna)  /* need 1 byte/base */
			BioseqRawConvert(bsp, Seq_code_iupacna);
	}
	else
	{
		if (bsp->seq_data_type != Seq_code_ncbieaa)
			BioseqRawConvert(bsp, Seq_code_ncbieaa);
	}

	BSSeek(bsp->seq_data, pos, SEEK_SET);
	BSPutByte(bsp->seq_data, (Int2)(TO_UPPER(residue)));
	retval = TRUE;

	return retval;
}


/*****************************************************************************
*
*   SeqInsertByLoc (target, offset, fragment)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqInsertByLoc (SeqIdPtr target, Int4 offset, SeqLocPtr fragment)
{
	return TRUE;
}


/*****************************************************************************
*
*   SeqDeleteByLoc (slp, do_feat, do_split)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqDeleteByLoc (SeqLocPtr slp, Boolean do_feat, Boolean do_split)
{
	SeqLocPtr tmp;
	Boolean retval = FALSE;
	Int2 numloc, i = 0, ctr, pick, totloc;
	SeqLocPtr PNTR locs, PNTR tlocs, PNTR theorder;
	SeqIdPtr the_id = NULL;
	BioseqPtr bsp;
	Int4 tstart, tstop;

	if (slp == NULL) return retval;

	numloc = 0;
	totloc = 0;
	locs = NULL;
	tmp = NULL;

	while ((tmp = SeqLocFindNext(slp, tmp)) != NULL)
	{
		switch (tmp->choice)
		{
			case SEQLOC_INT:
			case SEQLOC_PNT:
				if (BioseqFind(SeqLocId(tmp)) != NULL)
				{
					if (numloc == totloc)
					{
						tlocs = locs;
						locs = (SeqLocPtr PNTR)(MemNew((totloc+20) * sizeof(SeqLocPtr)));
						MemCopy(locs, tlocs, (size_t)(totloc * sizeof(SeqLocPtr)));
						MemFree(tlocs);
						totloc += 20;
					}
					locs[numloc] = tmp;
					numloc++;
				}
				break;
			default:
				Message(MSG_ERROR, "Unsupported Seqloc [%d] in SeqDeleteByLoc",
					(int)(tmp->choice));
				break;

		}
	}

	if (! numloc) return retval;

	              
				/***********************************************************
				*
				*   first gather all the seqlocs, grouped by Bioseq, and
				*   ordered from end to beginning. They must be ordered
				*   before the underlying Bioseq is changed.
				*
				***********************************************************/

	retval = TRUE;

	bsp = NULL;
	theorder = (SeqLocPtr PNTR)MemNew((sizeof(SeqLocPtr) * numloc));
	for (ctr = 0; ctr < numloc; ctr++)
	{
		pick = -1;   /* flag none found */
		if (bsp != NULL)
		{
			for (i = 0; i < numloc; i++)
			{
				if (locs[i] != NULL)
				{
				  	if (SeqIdIn(SeqLocId(locs[i]), bsp->id))
					{
						pick = i;
						i++;
						break;
					}
				}
			}
			if (pick < 0)
				bsp = NULL;   /* no more locs on this bioseq */
		}

		if (bsp == NULL)  /* have to find a new bioseq */
		{
			for (i = 0; i < numloc; i++)
			{
				if (locs[i] != NULL)
				{
					bsp = BioseqFind(SeqLocId(locs[i]));
					pick = i;
					i++;
					break;
				}
			}
		}

		while (i < numloc)
		{
			if (SeqLocOrder(locs[pick], locs[i], bsp) == (-1)) /* it's after */
				pick = i;
			i++;
		}

		theorder[ctr] = locs[pick];
		locs[pick] = NULL;
	}

	MemFree(locs);   /* finished with original list */

				/*************************************************************
				*
				*   Now do the actual deletions
				*
				*************************************************************/


	for (ctr = 0; ctr < numloc; ctr++)
	{
		tstart = SeqLocStart(theorder[ctr]);
		tstop = SeqLocStop(theorder[ctr]);
		BioseqDelete(SeqLocId(theorder[ctr]), tstart, tstop, do_feat, do_split);
	}

	MemFree(theorder);

	return retval;
}


/*****************************************************************************
*
*   SeqFeatDelete()
*     0 = no changes made to location or product
*     1 = changes made but feature still has some location
*     2 = all of sfp->location in deleted interval
*
*   if (merge)
*      1) correct numbers > to by subtraction
*      2) do not split intervals spanning the deletion
*   else
*      1) do not change numbers > to
*      2) split intervals which span the deletions
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL SeqFeatDelete (SeqFeatPtr sfp, SeqIdPtr target, Int4 from, Int4 to, Boolean merge)
{
	ValNode vn;
	SeqLocPtr tloc;
	SeqInt si;
	Boolean changed = FALSE, tmpbool = FALSE;
	CdRegionPtr crp;
	CodeBreakPtr cbp, prevcbp, nextcbp;
	RnaRefPtr rrp;
	tRNAPtr trp;

	tloc = &vn;
	MemSet((Pointer)tloc, 0, sizeof(ValNode));
	MemSet((Pointer)&si, 0, sizeof(SeqInt));
	tloc->choice = SEQLOC_INT;
	tloc->data.ptrvalue = (Pointer)(&si);
	si.id = target;
	si.from = from;
	si.to = to;

	sfp->location = SeqLocDelete(sfp->location, target, from, to, merge, &changed);

	sfp->product = SeqLocDelete(sfp->product, target, from, to, merge, &changed);

	if (sfp->location == NULL)
		return 2;

	switch (sfp->data.choice)
	{
		case SEQFEAT_CDREGION:   /* cdregion */
			crp = (CdRegionPtr)(sfp->data.value.ptrvalue);
			prevcbp = NULL;
			for (cbp = crp->code_break; cbp != NULL; cbp = nextcbp)
			{
				nextcbp = cbp->next;
				cbp->loc = SeqLocDelete(cbp->loc, target, from, to, merge, &tmpbool);
				if (cbp->loc == NULL)
				{
					if (prevcbp != NULL)
						prevcbp->next = nextcbp;
					else
						crp->code_break = nextcbp;
					cbp->next = NULL;
					CodeBreakFree(cbp);
				}
				else
					prevcbp = cbp;
			}
			break;
		case SEQFEAT_RNA:
			rrp = (RnaRefPtr)(sfp->data.value.ptrvalue);
			if (rrp->ext.choice == 2)   /* tRNA */
			{
				trp = (tRNAPtr)(rrp->ext.value.ptrvalue);
				if (trp->anticodon != NULL)
				{
					trp->anticodon = SeqLocDelete(trp->anticodon, target, from, to, merge, &tmpbool);
				}
			}
			break;
		default:
			break;
	}
			
	if (changed)
	{
		sfp->partial = TRUE;
		return 1;
	}
	else
		return 0;
}

/*****************************************************************************
*
*   SeqLocDelete()
*   	returns altered head or NULL if nothing left.
*   sets changed=TRUE if all or part of loc is deleted
*   does NOT set changed if location coordinates are only moved
*   if (merge) then corrects coordinates upstream of to
*   else
*     splits intervals covering from-to, does not correct upstream of to
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL SeqLocDeleteEx (SeqLocPtr head, SeqIdPtr target, Int4 from, Int4 to, Boolean merge, BoolPtr changed, BoolPtr partial5, BoolPtr partial3)
{
	SeqIntPtr sip, sip2;
	SeqPntPtr spp;
	PackSeqPntPtr pspp, pspp2;
	SeqBondPtr sbp;
	SeqIdPtr sidp;
	SeqLocPtr slp, tmp, prev, next, thead;
	Int4 diff, numpnt, i, tpos;
	BioseqPtr bsp;
	Boolean part5, part3, first;

	if ((head == NULL) || (target == NULL))
		return head;

	head->next = NULL;   /* caller maintains chains */
	diff = to - from + 1;
	
    switch (head->choice)
    {
        case SEQLOC_BOND:   /* bond -- 2 seqs */
			sbp = (SeqBondPtr)(head->data.ptrvalue);
			spp = sbp->a;
			if (SeqIdForSameBioseq(spp->id, target))
			{
				if (spp->point >= from)
				{
					if (spp->point <= to)   /* delete it */
					{
					    *changed = TRUE;
						sbp->a = SeqPntFree(spp);
					}
					else if (merge)
						spp->point -= diff;
				}
			}
			spp = sbp->b;
			if (spp != NULL)
			{
				if (SeqIdForSameBioseq(spp->id, target))
				{
					if (spp->point >= from)
					{
						if (spp->point <= to)   /* delete it */
						{
						    *changed = TRUE;
							sbp->b = SeqPntFree(spp);
						}
						else if (merge)
							spp->point -= diff;
					}
				}
			}
			if (sbp->a == NULL)
			{
				if (sbp->b != NULL)   /* only a required */
				{
					sbp->a = sbp->b;
					sbp->b = NULL;
				}
				else
				{
					head = SeqLocFree(head);
				}
			}
			break;
        case SEQLOC_FEAT:   /* feat -- can't track yet */
        case SEQLOC_NULL:    /* NULL */
        case SEQLOC_EMPTY:    /* empty */
			break;
        case SEQLOC_WHOLE:    /* whole */
			sidp = (SeqIdPtr)(head->data.ptrvalue);
			if (SeqIdForSameBioseq(sidp, target))
			{
				bsp = BioseqFind(target);
				if (bsp != NULL)           /* split it */
				{
					if ((from == 0) && (to >= (bsp->length - 1)))
					{					   /* complete delete */
						head = SeqLocFree(head);
						*changed = TRUE;
						break;
					}

					if (! merge)   /* split it up */
					{
						SeqIdFree(sidp);
						head->choice = SEQLOC_PACKED_INT;
						head->data.ptrvalue = NULL;
						slp = NULL;
						if (from != 0)
						{
							sip = SeqIntNew();
							sip->from = 0;
							sip->to = from - 1;
							sip->id = SeqIdDup(target);
							slp = ValNodeNew(NULL);
							slp->choice = SEQLOC_INT;
							slp->data.ptrvalue = sip;
							head->data.ptrvalue = slp;
							*changed = TRUE;
						}
						if (to < (bsp->length - 1))
						{
							sip = SeqIntNew();
							sip->from = to + 1;
							sip->to = bsp->length - 1;
							sip->id = SeqIdDup(target);
							tmp = ValNodeNew(NULL);
							tmp->choice = SEQLOC_INT;
							tmp->data.ptrvalue = sip;
							if (slp != NULL)
								slp->next = tmp;
							else
								head->data.ptrvalue = tmp;
							*changed = TRUE;
						}

					}
				}
			}
			break;
        case SEQLOC_MIX:    /* mix -- more than one seq */
        case SEQLOC_EQUIV:    /* equiv -- ditto */
        case SEQLOC_PACKED_INT:    /* packed int */
			prev = NULL;
			thead = NULL;
			part5 = FALSE;
			part3 = FALSE;
			first = TRUE;
			for (slp = (SeqLocPtr)(head->data.ptrvalue); slp != NULL; slp = next)
			{
				next = slp->next;
				tmp = SeqLocDeleteEx (slp, target, from, to, merge, changed, &part5, &part3);
				if (first) {
					if (partial5 != NULL) {
						*partial5 = part5;
					}
				}
				first = FALSE;
				if (tmp != NULL)
				{
					if (prev != NULL)
					{
						if ((merge) && (prev->choice == SEQLOC_INT) && (tmp->choice == SEQLOC_INT))
						{
							sip = (SeqIntPtr)(prev->data.ptrvalue);
							sip2 = (SeqIntPtr)(tmp->data.ptrvalue);

							if (SeqIdForSameBioseq(sip->id, sip2->id))
							{
							         /* merge intervals? */
								if ((sip->strand == Seq_strand_minus) &&
									(sip2->strand == Seq_strand_minus))
								{
									if (sip->from == (sip2->to + 1))
									{
										sip->from = sip2->from;
										sip->if_from = sip2->if_from;
										sip2->if_from = NULL;
										tmp = SeqLocFree(tmp);
									}
								}
								else if((sip->strand != Seq_strand_minus) &&
									(sip2->strand != Seq_strand_minus))
								{
									if (sip->to == (sip2->from - 1))
									{
										sip->to = sip2->to;
										sip->if_to = sip2->if_to;
										sip2->if_to = NULL;
										tmp = SeqLocFree(tmp);
									}
								}
							}
						}
						else if ((prev->choice == SEQLOC_NULL) && (tmp->choice == SEQLOC_NULL))
						{
							tmp = SeqLocFree(tmp);
							*changed = TRUE;
						}
					}
					else if (tmp->choice == SEQLOC_NULL)
					{
						tmp = SeqLocFree(tmp);
						*changed = TRUE;
					}

					if (tmp != NULL)   /* still have one? */
					{
						if (prev != NULL)
							prev->next = tmp;
						else
							thead = tmp;
						prev = tmp;
					}
				}
				else
					*changed = TRUE;
			}
			if (partial3 != NULL) {
				*partial3 = part3;
			}
			if (prev != NULL)
			{
				if (prev->choice == SEQLOC_NULL)  /* ends with NULL */
				{
					prev = NULL;
					for (slp = thead; slp->next != NULL; slp = slp->next)
						prev = slp;
					if (prev != NULL)
					{
						prev->next = NULL;
						SeqLocFree(slp);
					}
					else
					{
						thead = SeqLocFree(thead);
					}
					*changed = TRUE;
				}
			}
			head->data.ptrvalue = thead;
			if (thead == NULL)
				head = SeqLocFree(head);
            break;
        case SEQLOC_INT:    /* int */
			sip = (SeqIntPtr)(head->data.ptrvalue);
			if (SeqIdForSameBioseq(sip->id, target))
			{
				if (sip->to < from)  /* completely before cut */
					break;

								     /* completely contained in cut */
				if ((sip->from >= from) && (sip->to <= to))
				{
					head = SeqLocFree(head);
					*changed = TRUE;
					break;
				}

				if (sip->from > to)  /* completely past cut */
				{
					if (merge)
					{
						sip->from -= diff;
						sip->to -= diff;
					}
					break;
				}
									/* overlap here */

				if (sip->to > to)
				{
					if (merge)
						sip->to -= diff;
				}
				else                /* to inside cut, so partial delete */
				{
					sip->to = from - 1;
					*changed = TRUE;
					if (partial3 != NULL) {
						*partial3 = TRUE;
					}
				}
		
				if (sip->from >= from)   /* from inside cut, partial del */
				{
					*changed = TRUE;
					sip->from = to + 1;
					if (merge)
						sip->from -= diff;
					if (partial5 != NULL) {
						*partial5 = TRUE;
					}
				}

				if (merge)
					break;

						   /* interval spans cut.. only in non-merge */
				           /* have to split */

				if ((sip->from < from) && (sip->to > to))
				{
					*changed = TRUE;
					head->choice = SEQLOC_PACKED_INT;
					head->data.ptrvalue = NULL;
					tmp = ValNodeNew(NULL);
					tmp->choice = SEQLOC_INT;
					tmp->data.ptrvalue = sip;

					sip2 = SeqIntNew();
					sip2->from = to + 1;
					sip2->to = sip->to;
					sip2->strand = sip->strand;
					sip2->if_to = sip->if_to;
					sip2->id = SeqIdDup(target);
					slp = ValNodeNew(NULL);
					slp->choice = SEQLOC_INT;
					slp->data.ptrvalue = sip2;

					sip->if_to = NULL;
					sip->to = from - 1;

					if (sip->strand == Seq_strand_minus)
					{
						head->data.ptrvalue = slp;
						slp->next = tmp;
					}
					else
					{
						head->data.ptrvalue = tmp;
						tmp->next = slp;
					}

				}

			}
            break;
        case SEQLOC_PNT:    /* pnt */
			spp = (SeqPntPtr)(head->data.ptrvalue);
			if (SeqIdForSameBioseq(spp->id, target))
			{
				if ((spp->point >= from) && (spp->point <= to))
				{
					head = SeqLocFree(head);
					*changed = TRUE;
				}
				else if (spp->point > to)
				{
					if (merge)
						spp->point -= diff;
				}
			}
            break;
        case SEQLOC_PACKED_PNT:    /* packed pnt */
			pspp = (PackSeqPntPtr)(head->data.ptrvalue);
			if (SeqIdForSameBioseq(pspp->id, target))
			{
				numpnt = PackSeqPntNum(pspp);
				pspp2 = PackSeqPntNew();
				head->data.ptrvalue = pspp2;
				for (i = 0; i < numpnt; i++)
				{
					tpos = PackSeqPntGet(pspp, i);
					if (tpos < from)
						PackSeqPntPut(pspp2, tpos);
					else
					{
						if (tpos > to)
						{
							if (merge)
								tpos -= diff;
							PackSeqPntPut(pspp2, tpos);
						}
						else
							*changed = TRUE;
					}
				}
				pspp2->id = pspp->id;
				pspp->id = NULL;
				pspp2->fuzz = pspp->fuzz;
				pspp->fuzz = NULL;
				pspp2->strand = pspp->strand;
				PackSeqPntFree(pspp);
				numpnt = PackSeqPntNum(pspp2);
				if (! numpnt)
					head = SeqLocFree(head);

			}
            break;
        default:
            break;
    }

	return head;
}

NLM_EXTERN SeqLocPtr LIBCALL SeqLocDelete (SeqLocPtr head, SeqIdPtr target, Int4 from, Int4 to, Boolean merge, BoolPtr changed)

{
  return SeqLocDeleteEx (head, target, from, to, merge, changed, NULL, NULL);
}

typedef struct delstruct {
	SeqIdPtr sip;
	Int4 from, to;
	Boolean merge;
} DelStruct, PNTR DelStructPtr;

NLM_EXTERN void DelFeat (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);

NLM_EXTERN void DelFeat (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	DelStructPtr dsp;
	BioseqPtr bsp;
	BioseqSetPtr bssp;
	SeqAnnotPtr sap, nextsap;
	SeqFeatPtr sfp, nextsfp;
	Pointer PNTR prevsap, PNTR prevsfp;

	dsp = (DelStructPtr)data;
	if (IS_Bioseq(sep))
	{
		bsp = (BioseqPtr)(sep->data.ptrvalue);
		sap = bsp->annot;
		prevsap = (Pointer PNTR) &(bsp->annot);
	}
	else
	{
		bssp = (BioseqSetPtr)(sep->data.ptrvalue);
		sap = bssp->annot;
		prevsap = (Pointer PNTR) &(bssp->annot);
	}

	while (sap != NULL)
	{
		nextsap = sap->next;
		if (sap->type == 1)   /* feature table */
		{
			sfp = (SeqFeatPtr) sap->data;
			prevsfp = (Pointer PNTR) &(sap->data);
			while (sfp != NULL)
			{
				nextsfp = sfp->next;
				if (SeqFeatDelete(sfp, dsp->sip, dsp->from, dsp->to, dsp->merge) == 2)
				{
					/* location completely gone */
					*(prevsfp) = sfp->next;
					sfp->next = NULL;
					SeqFeatFree(sfp);
				} else {
					prevsfp = (Pointer PNTR) &(sfp->next);
				}
				sfp = nextsfp;
			}
		}

		if (sap->data == NULL)  /* all features deleted */
		{
			*(prevsap) = sap->next;
			sap->next = NULL;
			SeqAnnotFree (sap);
		} else {
			prevsap = (Pointer PNTR) &(sap->next);
		}

		sap = nextsap;
	}

	return;
}

/*****************************************************************************
*
*   SeqEntryDelFeat(sep, id, from, to, do_split)
*   	Deletes or truncates features on Bioseq (id) in the range
*       from-to, inclusive
*       
*		Moves features > to left to account for decrease in length
*       if do_split, breaks intervals across the deletion
*       else just reduces their size
*
*       If sep == NULL, then calls SeqEntryFind(id) to set scope to look
*       for features.
*   
*****************************************************************************/
NLM_EXTERN Boolean	LIBCALL SeqEntryDelFeat (SeqEntryPtr sep, SeqIdPtr sip, Int4 from, Int4 to, Boolean do_split)
{

	DelStruct ds;

	if (sip == NULL)
		return FALSE;

	if (sep == NULL)
		sep	= SeqEntryFind(sip);

	if (sep == NULL) return FALSE;

	ds.sip = sip;
	ds.from = from;
	ds.to = to;
	if (do_split)
		ds.merge = FALSE;
	else
		ds.merge = TRUE;

	SeqEntryExplore(sep, (Pointer)(&ds), DelFeat);

	return TRUE;
}

/*****************************************************************************
*
*   DescrToFeatures(sep)
*   	Moves all Seqdescr to features in sep where possible
*
*****************************************************************************/


/*****************************************************************************
*
*   BioseqCopy(newid, sourceid, from, to, strand, do_feat)
*      Creates a new Bioseq from sourceid in the range from-to inclusive.
*      If strand==Seq_strand_minus, reverse complements the sequence in
*        the copy and (if do_feat) corrects the feature table appropriately.
*      Names new Bioseq as newid, if not NULL
*        else Creates seqid.local = "Clipboard" if newid is NULL
*      If do_feat == TRUE copies appropriate region of feature table from
*        sourceid to new copy using SeqFeatsCopy().
*
*****************************************************************************/
NLM_EXTERN BioseqPtr LIBCALL BioseqCopyEx (SeqIdPtr newid, BioseqPtr oldbsp, Int4 from, Int4 to,
                               Uint1 strand, Boolean do_feat)
{
	BioseqPtr newbsp=NULL, tmpbsp;
	SeqPortPtr spp=NULL;
	ByteStorePtr bsp;
	Uint1 seqtype;
	ValNodePtr tmp;
	ObjectIdPtr oid;
	Int4 len, i;
	Int2 residue;
	ValNode fake;
	SeqLocPtr the_segs, head, curr;
	Boolean handled = FALSE, split;
	SeqFeatPtr sfp, newsfp, lastsfp;
	DeltaSeqPtr dsp;
	SeqEntryPtr oldscope;


	if ((oldbsp == NULL) || (from < 0)) return FALSE;

	len = to - from + 1;
	if (len <= 0) return NULL;

	newbsp = BioseqNew();
	if (newid != NULL)
		newbsp->id = SeqIdDup(newid);
	else
	{
		tmp = ValNodeNew(NULL);
		tmp->choice = SEQID_LOCAL;
		oid = ObjectIdNew();
		tmp->data.ptrvalue = (Pointer)oid;
		oid->str = StringSave("Clipboard");
		tmpbsp = BioseqFind(tmp);   /* old clipboard present? */
		if (tmpbsp == NULL) {
			oldscope = SeqEntrySetScope (NULL);
			if (oldscope != NULL) {
				tmpbsp = BioseqFind(tmp);
				SeqEntrySetScope (oldscope);
			}
		}
		if (tmpbsp != NULL)
			BioseqFree(tmpbsp);
		newbsp->id = tmp;
	}

	newbsp->repr = oldbsp->repr;
	newbsp->mol = oldbsp->mol;
	newbsp->length = len;
	newbsp->seq_ext_type = oldbsp->seq_ext_type;

	if (newbsp->repr == Seq_repr_virtual)
		handled = TRUE;               /* no more to do */

	if ((newbsp->repr == Seq_repr_raw) ||
		(newbsp->repr == Seq_repr_const))
	{
		if (ISA_aa(newbsp->mol))
		{
			seqtype = Seq_code_ncbieaa;
		}
		else
		{
			seqtype = Seq_code_iupacna;
		}
		newbsp->seq_data_type = seqtype;
		bsp = BSNew(len);
		if (bsp == NULL) goto erret;

		newbsp->seq_data = bsp;
		spp = SeqPortNew(oldbsp, from, to, strand, seqtype);
		if (spp == NULL) goto erret;

		for (i = 0; i < len; i++)
		{
			residue = SeqPortGetResidue(spp);
			if (! IS_residue(residue)) goto erret;
			BSPutByte(bsp, residue);
		}

		SeqPortFree(spp);
		handled = TRUE;
	}

	if ((newbsp->repr == Seq_repr_seg) ||
		(newbsp->repr == Seq_repr_ref) ||
		(newbsp->repr == Seq_repr_delta))
	{
        if (newbsp->repr == Seq_repr_seg)  /* segmented */
		{
			fake.choice = SEQLOC_MIX;   /* make SEQUENCE OF Seq-loc, into one */
			fake.data.ptrvalue = oldbsp->seq_ext;
			fake.next = NULL;
			the_segs = (SeqLocPtr)&fake;
			head = SeqLocCopyPart (the_segs, from, to, strand, FALSE, NULL, NULL);
		}
		else if (newbsp->repr == Seq_repr_ref)  /* reference: is a Seq-loc */
		{
			head = SeqLocCopyPart ((SeqLocPtr)(oldbsp->seq_ext), from, to,
			                         strand, TRUE, NULL, NULL);
		}
		else if (newbsp->repr == Seq_repr_delta)
		{
			dsp = (DeltaSeqPtr)(oldbsp->seq_ext);  /* real data is here */
			the_segs = DeltaSeqsToSeqLocs(dsp);
			head = SeqLocCopyPart (the_segs, from, to, strand, FALSE, NULL, NULL);
			SeqLocFree (the_segs);
		}

        newbsp->seq_ext = (Pointer)head;
		handled = TRUE;
	}

	if (newbsp->repr == Seq_repr_map)
	{
		lastsfp = NULL;
		for (sfp = (SeqFeatPtr)(oldbsp->seq_ext); sfp != NULL; sfp = sfp->next)
		{
			split = FALSE;
			curr = SeqLocCopyRegion(newbsp->id, sfp->location, oldbsp, from, to, strand, &split);
			if (curr != NULL)   /* got one */
			{
				newsfp = (SeqFeatPtr)AsnIoMemCopy((Pointer)sfp, (AsnReadFunc)SeqFeatAsnRead, (AsnWriteFunc)SeqFeatAsnWrite);
				SeqLocFree(newsfp->location);
				newsfp->location = curr;
				if (split)
					newsfp->partial = TRUE;
				if (lastsfp == NULL)  /* first one */
					newbsp->seq_ext = (Pointer)newsfp;
				else
					lastsfp->next = newsfp;
				lastsfp = newsfp;
			}
		}
		handled = TRUE;
	}
	

	if (! handled) goto erret;

	               /* get descriptors */
	               /* get features */

	if (do_feat)
		SeqFeatsCopy (newbsp, oldbsp, from, to, strand);

	return newbsp;

erret:
	BioseqFree(newbsp);
	SeqPortFree(spp);
	return NULL;
}

NLM_EXTERN BioseqPtr LIBCALL BioseqCopy (SeqIdPtr newid, SeqIdPtr sourceid, Int4 from, Int4 to,
                               Uint1 strand, Boolean do_feat)
{
	BioseqPtr oldbsp;
	SeqEntryPtr oldscope;

	if ((sourceid == NULL) || (from < 0)) return FALSE;

	oldbsp = BioseqFind(sourceid);
	if (oldbsp == NULL) {
		oldscope = SeqEntrySetScope (NULL);
		if (oldscope != NULL) {
			oldbsp = BioseqFind(sourceid);
			SeqEntrySetScope (oldscope);
		}
	}
	if (oldbsp == NULL) return NULL;

	return BioseqCopyEx (newid, oldbsp, from, to, strand, do_feat);
}

/*****************************************************************************
*
*	SeqLocCopyPart (the_segs, from, to, strand, group, first_segp, last_segp)
*      cuts out from the_segs the part from offset from to offset to
*      reverse complements resulting seqloc if strand == Seq_strand_minus
*      if (group) puts resulting intervals into a new Seq-loc (of type
*        PACKED_INT if no SEQLOC_NULL, else SEQLOC_MIX).
*      Currently this always makes intervals or nulls. Is really for segmented and
*        reference sequence extensions
*      If first_segp and last_segp are not NULL, then they are filled in with the
*        ordinal number of the source segments that remain in the copy, based
*        on SeqLocFindNext, where 1 is the first one. Thus if the third and
*        fourth segments were copied, first is 3 and last is 4. If the
*        location was reverse complemented, first is 4 and last is 3.
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL SeqLocCopyPart (SeqLocPtr the_segs, Int4 from, Int4 to, Uint1 strand,
					Boolean group, Int2Ptr first_segp, Int2Ptr last_segp)
{
	SeqLocPtr currseg, newhead, head, prev, curr, last;
	Int2 numloc, first_seg = 0, last_seg = 0, seg_ctr = 0;
	Int4 oldpos, tlen, tfrom, tto, tstart, tstop, xfrom, xto;
	Uint1 tstrand;
	SeqIdPtr tid;
        SeqIntPtr sip;
	Boolean done, started, wasa_null, hada_null;
	BioseqPtr bsp;

	if (the_segs == NULL) return NULL;
	if ((from < 0) || (to < 0)) return NULL;

   currseg = NULL;
   oldpos = 0;	   /* position in old sequence */
   done = FALSE;
   started = FALSE;
   head = NULL;
   prev = NULL;
   numloc = 0;
   wasa_null = FALSE;
   hada_null = FALSE;
   while ((oldpos <= to) && ((currseg = SeqLocFindNext(the_segs, currseg)) != NULL))
   {
		seg_ctr++;
   		tlen = SeqLocLen(currseg);
   		tid = SeqLocId(currseg);
   		if (tlen < 0) {
   			bsp = BioseqLockById (tid);  /* only necessary for locations of type WHOLE */
   			tlen = SeqLocLen (currseg);
   			BioseqUnlock (bsp);
   		}
	   	tstrand = SeqLocStrand(currseg);
   		tfrom = SeqLocStart(currseg);
	   	tto = SeqLocStop(currseg);

	   	if (! started)
   		{
			wasa_null = FALSE;
   			if (((oldpos + tlen - 1) >= from) &&
   				(currseg->choice != SEQLOC_NULL))
	   		{
   				tstart = from - oldpos;
   				started = TRUE;
				first_seg = seg_ctr;
	   		}
   			else
   				tstart = -1;
	   	}
   		else
		{
			if (currseg->choice == SEQLOC_NULL)
			{
				wasa_null = TRUE;
				tstart = -1;  /* skip it till later */
			}
			else
				tstart = 0;
		}

	   	if (tstart >= 0)   /* have a start */
   		{
   			if ((oldpos + tlen - 1) >= to)
	   		{
   				done = TRUE;   /* hit the end */
   				tstop = ((oldpos + tlen - 1) - to);
	   		}
   			else
   				tstop = 0;

	   		if (tstrand == Seq_strand_minus)
   			{
   				xfrom = tfrom + tstop;
   				xto = tto - tstart;
	   		}
   			else
   			{
   				xfrom = tfrom + tstart;
	   			xto = tto - tstop;
   			}

   			sip = SeqIntNew();
	   		sip->id = SeqIdDup(tid);
   			sip->strand = tstrand;
   			sip->from = xfrom;
	   		sip->to = xto;
			if (wasa_null)  /* previous SEQLOC_NULL */
			{
				curr = ValNodeAddInt(&head, SEQLOC_NULL, 0);
				numloc++;
				wasa_null = FALSE;
				hada_null = TRUE;
			}
   			curr = ValNodeAddPointer(&head, SEQLOC_INT, (Pointer)sip);
   			numloc++;
			last_seg = seg_ctr;
	   	}

   		oldpos += tlen;
   }

   if (strand == Seq_strand_minus)  /* reverse order and complement */
   {
	   	newhead = NULL;
   		last = NULL;
	   	while (head != NULL)
   		{
   			prev = NULL;
	   		for (curr = head; curr->next != NULL; curr = curr->next)
   				prev = curr;
   			if (prev != NULL)
   				prev->next = NULL;
	   		else
   				head = NULL;

   			if (newhead == NULL)
   				newhead = curr;
	   		else
   				last->next = curr;
   			last = curr;
			if (curr->choice == SEQLOC_INT)
			{
				sip = (SeqIntPtr)(curr->data.ptrvalue);
				sip->strand = StrandCmp(sip->strand);
			}
	   	}

   		head = newhead;
		seg_ctr = last_seg;
		last_seg = first_seg;
		first_seg = seg_ctr;
   }

   if ((numloc) && (group))
   {
   		curr = ValNodeNew(NULL);
		if (hada_null)
			curr->choice = SEQLOC_MIX;
		else
			curr->choice = SEQLOC_PACKED_INT;
	   	curr->data.ptrvalue = (Pointer)head;
   		head = curr;
   }

   if (first_segp != NULL)
	 *first_segp = first_seg;
   if (last_segp != NULL)
	 *last_segp = last_seg;

   return head;
}

/*****************************************************************************
*
*   SeqFeatCopy(new, old, from, to, strand)
*
*****************************************************************************/
static Int2 LIBCALL IndexedSeqFeatsCopy (BioseqPtr newbsp, BioseqPtr oldbsp, Int4 from, Int4 to, Uint1 strand)

{
	Int2 ctr=0;
	SeqFeatPtr head=NULL, sfp, last=NULL, newsfp;
	SeqInt si;
	ValNode vn;
	ValNodePtr region;
	SeqLocPtr newloc;
	Boolean split = FALSE;
	SeqAnnotPtr sap = NULL, saptmp;
	CdRegionPtr crp;
	CodeBreakPtr cbp, prevcbp, nextcbp;
	RnaRefPtr rrp;
	tRNAPtr trp;
	SeqMgrFeatContext fcontext;

	region = &vn;
	vn.choice = SEQLOC_INT;
	vn.data.ptrvalue = (Pointer)(&si);
	si.from = from;
	si.to = to;
	si.id = oldbsp->id;
	si.if_from = NULL;
	si.if_to = NULL;

	sfp = NULL;
	while ((sfp = SeqMgrGetNextFeature (oldbsp, sfp, 0, 0, &fcontext)) != NULL)
	{
		/* can exit once past rightmost limit */
		if (fcontext.left > to) return ctr;

		if (fcontext.right >= from && fcontext.left <= to) {

			split = FALSE;
			newloc = SeqLocCopyRegion(newbsp->id, sfp->location, oldbsp, from, to, strand, &split);
			if (newloc != NULL)   /* got one */
			{
				newsfp = (SeqFeatPtr)AsnIoMemCopy((Pointer)sfp, (AsnReadFunc)SeqFeatAsnRead, (AsnWriteFunc)SeqFeatAsnWrite);
				SeqLocFree(newsfp->location);
				newsfp->location = newloc;
				if (split)
					newsfp->partial = TRUE;
				if (last == NULL)  /* first one */
				{
					sap = SeqAnnotNew();
					if (newbsp->annot == NULL)
						newbsp->annot = sap;
					else
					{
						for (saptmp = newbsp->annot; saptmp->next != NULL; saptmp = saptmp->next)
							continue;
						saptmp->next = sap;
					}
					sap->type = 1;   /* feature table */
					sap->data = (Pointer)newsfp;
				}
				else
					last->next = newsfp;
				last = newsfp;

				switch (newsfp->data.choice)
				{
					case SEQFEAT_CDREGION:   /* cdregion */
						crp = (CdRegionPtr)(newsfp->data.value.ptrvalue);
						prevcbp = NULL;
						for (cbp = crp->code_break; cbp != NULL; cbp = nextcbp)
						{
							nextcbp = cbp->next;
							cbp->loc = SeqLocCopyRegion(newbsp->id, cbp->loc, oldbsp, from, to, strand, &split);
							if (cbp->loc == NULL)
							{
								if (prevcbp != NULL)
									prevcbp->next = nextcbp;
								else
									crp->code_break = nextcbp;
								cbp->next = NULL;
								CodeBreakFree(cbp);
							}
							else
								prevcbp = cbp;
						}
						break;
					case SEQFEAT_RNA:
						rrp = (RnaRefPtr)(newsfp->data.value.ptrvalue);
						if (rrp->ext.choice == 2)   /* tRNA */
						{
							trp = (tRNAPtr)(rrp->ext.value.ptrvalue);
							if (trp->anticodon != NULL)
							{
								trp->anticodon = SeqLocCopyRegion(newbsp->id, trp->anticodon, oldbsp, from, to, strand, &split);
							}
						}
						break;
					default:
						break;
				}
			}
		}
		
	}
	return ctr;
}

NLM_EXTERN Int2 LIBCALL SeqFeatsCopy (BioseqPtr newbsp, BioseqPtr oldbsp, Int4 from, Int4 to, Uint1 strand)
{
	Int2 ctr=0;
	BioseqContextPtr bcp = NULL;
	SeqFeatPtr head=NULL, sfp, last=NULL, newsfp;
	SeqInt si;
	ValNode vn;
	ValNodePtr region;
	SeqLocPtr newloc;
	Boolean split = FALSE;
	SeqAnnotPtr sap = NULL, saptmp;
	CdRegionPtr crp;
	CodeBreakPtr cbp, prevcbp, nextcbp;
	RnaRefPtr rrp;
	tRNAPtr trp;
	Uint2 entityID;

	if (oldbsp == NULL) return ctr;

	entityID = ObjMgrGetEntityIDForPointer (oldbsp);
	if (entityID > 0 && SeqMgrFeaturesAreIndexed (entityID)) {
		/* indexed version should be much faster */
		return IndexedSeqFeatsCopy (newbsp, oldbsp, from, to, strand);
	}

	bcp = BioseqContextNew(oldbsp);
	if (bcp == NULL) return ctr;

	region = &vn;
	vn.choice = SEQLOC_INT;
	vn.data.ptrvalue = (Pointer)(&si);
	si.from = from;
	si.to = to;
	si.id = oldbsp->id;
	si.if_from = NULL;
	si.if_to = NULL;

	sfp = NULL;
	while ((sfp = BioseqContextGetSeqFeat(bcp, 0, sfp, NULL, 0)) != NULL)
	{
		split = FALSE;
		newloc = SeqLocCopyRegion(newbsp->id, sfp->location, oldbsp, from, to, strand, &split);
		if (newloc != NULL)   /* got one */
		{
			newsfp = (SeqFeatPtr)AsnIoMemCopy((Pointer)sfp, (AsnReadFunc)SeqFeatAsnRead, (AsnWriteFunc)SeqFeatAsnWrite);
			SeqLocFree(newsfp->location);
			newsfp->location = newloc;
			if (split)
				newsfp->partial = TRUE;
			if (last == NULL)  /* first one */
			{
				sap = SeqAnnotNew();
				if (newbsp->annot == NULL)
					newbsp->annot = sap;
				else
				{
					for (saptmp = newbsp->annot; saptmp->next != NULL; saptmp = saptmp->next)
						continue;
					saptmp->next = sap;
				}
				sap->type = 1;   /* feature table */
				sap->data = (Pointer)newsfp;
			}
			else
				last->next = newsfp;
			last = newsfp;

			switch (newsfp->data.choice)
			{
				case SEQFEAT_CDREGION:   /* cdregion */
					crp = (CdRegionPtr)(newsfp->data.value.ptrvalue);
					prevcbp = NULL;
					for (cbp = crp->code_break; cbp != NULL; cbp = nextcbp)
					{
						nextcbp = cbp->next;
						cbp->loc = SeqLocCopyRegion(newbsp->id, cbp->loc, oldbsp, from, to, strand, &split);
						if (cbp->loc == NULL)
						{
							if (prevcbp != NULL)
								prevcbp->next = nextcbp;
							else
								crp->code_break = nextcbp;
							cbp->next = NULL;
							CodeBreakFree(cbp);
						}
						else
							prevcbp = cbp;
					}
					break;
				case SEQFEAT_RNA:
					rrp = (RnaRefPtr)(newsfp->data.value.ptrvalue);
					if (rrp->ext.choice == 2)   /* tRNA */
					{
						trp = (tRNAPtr)(rrp->ext.value.ptrvalue);
						if (trp->anticodon != NULL)
						{
							trp->anticodon = SeqLocCopyRegion(newbsp->id, trp->anticodon, oldbsp, from, to, strand, &split);
						}
					}
					break;
				default:
					break;
			}
		}
		
	}
	BioseqContextFree (bcp);
	return ctr;
}


NLM_EXTERN SeqLocPtr LIBCALL SeqLocCopyRegion(SeqIdPtr newid, SeqLocPtr head, BioseqPtr oldbsp,
	Int4 from, Int4 to, Uint1 strand, BoolPtr split)
{
	SeqLocPtr newhead = NULL, last=NULL, tmp, slp, prev, next, thead;
	SeqIntPtr sip, sip2;
	SeqPntPtr spp, spp2;
	PackSeqPntPtr pspp, pspp2;
	SeqBondPtr sbp, sbp2;
	SeqIdPtr sidp, oldids;
	Int4 numpnt, i, tpos, len, intcnt, othercnt;
	Pointer ptr = NULL;
	Boolean dropped_one;
	IntFuzzPtr ifp;
	ValNode vn;
	
	if ((head == NULL) || (oldbsp == NULL)) return NULL;

	oldids = oldbsp->id;
	len = to - from + 1;
	switch (head->choice)
	{
        case SEQLOC_BOND:   /* bond -- 2 seqs */
			sbp2 = NULL;
			sbp = (SeqBondPtr)(head->data.ptrvalue);
			vn.choice = SEQLOC_PNT;
			vn.data.ptrvalue = sbp->a;
			vn.next = NULL;
			tmp = SeqLocCopyRegion(newid, (SeqLocPtr)(&vn), oldbsp, from, to, strand, split);
			if (tmp != NULL)
			{
			 	sbp2 = SeqBondNew();
				sbp2->a = (SeqPntPtr)(tmp->data.ptrvalue);
				MemFree(tmp);
			}
			if (sbp->b != NULL)
			{
				vn.data.ptrvalue = sbp->b;
				tmp = SeqLocCopyRegion(newid, (SeqLocPtr)(&vn), oldbsp, from, to, strand, split);
				if (tmp != NULL)
				{
					if (sbp2 == NULL)
					{
					 	sbp2 = SeqBondNew();
						sbp2->a = (SeqPntPtr)(tmp->data.ptrvalue);
					}
					else
						sbp2->b = (SeqPntPtr)(tmp->data.ptrvalue);
					MemFree(tmp);
				}
			}
			if (sbp2 != NULL)
			{
				newhead = ValNodeNew(NULL);
				newhead->choice = SEQLOC_BOND;
				newhead->data.ptrvalue = sbp2;
				if ((sbp->b != NULL) && (sbp2->b == NULL))
					*split = TRUE;
			}
			break;
        case SEQLOC_FEAT:   /* feat -- can't track yet */
        case SEQLOC_NULL:    /* NULL */
        case SEQLOC_EMPTY:    /* empty */
			break;
        case SEQLOC_WHOLE:    /* whole */
			sidp = (SeqIdPtr)(head->data.ptrvalue);
			if (SeqIdIn(sidp, oldids))
			{
				if ((from != 0) || (to != (oldbsp->length - 1)))
				{
					*split = TRUE;
				}
				newhead = ValNodeNew(NULL);
				sip2 = SeqIntNew();
				sip2->id = SeqIdDup(newid);
				sip2->from = 0;
				sip2->to = to - from;
				newhead->choice = SEQLOC_INT;
				newhead->data.ptrvalue = (Pointer)sip2;
				if (strand == Seq_strand_minus)
					sip2->strand = Seq_strand_minus;
			}
			break;
        case SEQLOC_EQUIV:    /* does it stay equiv? */
        case SEQLOC_MIX:    /* mix -- more than one seq */
        case SEQLOC_PACKED_INT:    /* packed int */
			prev = NULL;
			thead = NULL;
			dropped_one = FALSE;
			for (slp = (SeqLocPtr)(head->data.ptrvalue); slp != NULL; slp = next)
			{
				next = slp->next;
				tmp = SeqLocCopyRegion(newid, slp, oldbsp, from, to, strand, split);
				if (tmp != NULL)
				{
					if (prev != NULL)
					{
						if ((prev->choice == SEQLOC_INT) && (tmp->choice == SEQLOC_INT))
						{
							sip = (SeqIntPtr)(prev->data.ptrvalue);
							sip2 = (SeqIntPtr)(tmp->data.ptrvalue);

							if ((sip->strand == Seq_strand_minus) &&
								(sip2->strand == Seq_strand_minus))
							{
								if (sip->from == (sip2->to + 1))
								{
									sip->from = sip2->from;
									sip->if_from = sip2->if_from;
									sip2->if_from = NULL;
									tmp = SeqLocFree(tmp);
								}
							}
							else if((sip->strand != Seq_strand_minus) &&
								(sip2->strand != Seq_strand_minus))
							{
								if (sip->to == (sip2->from - 1))
								{
									sip->to = sip2->to;
									sip->if_to = sip2->if_to;
									sip2->if_to = NULL;
									tmp = SeqLocFree(tmp);
								}
							}
						}
						else if ((prev->choice == SEQLOC_NULL) && (tmp->choice == SEQLOC_NULL))
						{
							tmp = SeqLocFree(tmp);
							dropped_one = TRUE;
						}
					}
					else if (tmp->choice == SEQLOC_NULL)
					{
						tmp = SeqLocFree(tmp);
						dropped_one = TRUE;
					}

					if (tmp != NULL)   /* still have one? */
					{
						if (prev != NULL)
							prev->next = tmp;
						else
							thead = tmp;
						prev = tmp;
					}
					else
						dropped_one = TRUE;
				}
				else
					dropped_one = TRUE;
			}
			if (prev != NULL)
			{
				if (prev->choice == SEQLOC_NULL)  /* ends with NULL */
				{
					prev = NULL;
					for (slp = thead; slp->next != NULL; slp = slp->next)
						prev = slp;
					if (prev != NULL)
					{
						prev->next = NULL;
						SeqLocFree(slp);
					}
					else
					{
						thead = SeqLocFree(thead);
					}
					dropped_one = TRUE;
				}
			}
			if (thead != NULL)
			{
				if (dropped_one)
					*split = TRUE;
				intcnt = 0;
				othercnt = 0;
				for (slp = thead; slp != NULL; slp = slp->next)
				{
					if (slp->choice == SEQLOC_INT)
						intcnt++;
					else
						othercnt++;
				}
				if ((intcnt + othercnt) > 1)
				{
					newhead = ValNodeNew(NULL);
					if (head->choice == SEQLOC_EQUIV)
						newhead->choice = SEQLOC_EQUIV;
					else
					{
						if (othercnt == 0)
							newhead->choice = SEQLOC_PACKED_INT;
						else
							newhead->choice = SEQLOC_MIX;
					}

					newhead->data.ptrvalue = (Pointer)thead;
				}
				else                 /* only one SeqLoc left */
					newhead = thead;

			}
            break;
        case SEQLOC_INT:    /* int */
			sip = (SeqIntPtr)(head->data.ptrvalue);
			if (SeqIdIn(sip->id, oldids))
			{
				if (sip->to < from)  /* completely before cut */
					break;
				if (sip->from > to)  /* completely after cut */
					break;

				sip2 = SeqIntNew();
				sip2->id = SeqIdDup(newid);
				sip2->strand = sip->strand;

				if (sip->to > to)
				{
					sip2->to = to;
					*split = TRUE;
					ifp = IntFuzzNew();
					ifp->choice = 4;   /* lim */
					ifp->a = 1;        /* greater than */
					sip2->if_to = ifp;
				}
				else
				{
					sip2->to = sip->to;
					if (sip->if_to != NULL)
					{
						ifp = IntFuzzNew();
						MemCopy((Pointer)ifp, (Pointer)(sip->if_to), sizeof(IntFuzz));
						sip2->if_to = ifp;
					}
				}

				if (sip->from < from)
				{
					sip2->from = from;
					*split = TRUE;
					ifp = IntFuzzNew();
					ifp->choice = 4;   /* lim */
					ifp->a = 2;        /* less than */
					sip2->if_from = ifp;
				}
				else
				{
					sip2->from = sip->from;
					if (sip->if_from != NULL)
					{
						ifp = IntFuzzNew();
						MemCopy((Pointer)ifp, (Pointer)(sip->if_from), sizeof(IntFuzz));
						sip2->if_from = ifp;
					}
				}
									  /* set to region coordinates */
				sip2->from -= from;
				sip2->to -= from;
				IntFuzzClip(sip2->if_from, from, to, strand, split);
				IntFuzzClip(sip2->if_to, from, to, strand, split);

				if (strand == Seq_strand_minus)  /* rev comp */
				{
					sip2->strand = StrandCmp(sip2->strand);
					tpos = len - sip2->from - 1;
					sip2->from = len - sip2->to - 1;
					sip2->to = tpos;
					      /* IntFuzz already complemented by IntFuzzClip */
					      /* just switch order */
					ifp = sip2->if_from;
					sip2->if_from = sip2->if_to;
					sip2->if_to = ifp;
				}

				newhead = ValNodeNew(NULL);
				newhead->choice = SEQLOC_INT;
				newhead->data.ptrvalue = (Pointer)sip2;
			}
            break;
        case SEQLOC_PNT:    /* pnt */
			spp = (SeqPntPtr)(head->data.ptrvalue);
			if (SeqIdIn(spp->id, oldids))
			{
				if ((spp->point >= from) && (spp->point <= to))
				{
					spp2 = SeqPntNew();
					spp2->id = SeqIdDup(newid);
					spp2->point = spp->point - from;
					spp2->strand = spp->strand;
					if (spp->fuzz != NULL)
					{
						ifp = IntFuzzNew();
						spp2->fuzz = ifp;
						MemCopy((Pointer)ifp, (Pointer)spp->fuzz, sizeof(IntFuzz));
						IntFuzzClip(ifp, from, to, strand, split);
					}
					if (strand == Seq_strand_minus)
					{
						spp2->point = len - spp2->point - 1;
						spp2->strand = StrandCmp(spp->strand);
					}
					newhead = ValNodeNew(NULL);
					newhead->choice = SEQLOC_PNT;
					newhead->data.ptrvalue = (Pointer)spp2;
				}
			}
            break;
        case SEQLOC_PACKED_PNT:    /* packed pnt */
			pspp = (PackSeqPntPtr)(head->data.ptrvalue);
			if (SeqIdIn(pspp->id, oldids))
			{
				numpnt = PackSeqPntNum(pspp);
				pspp2 = PackSeqPntNew();
				pspp2->strand = pspp->strand;
				intcnt = 0;	     /* use for included points */
				othercnt = 0;	 /* use for exclued points */
				for (i = 0; i < numpnt; i++)
				{
					tpos = PackSeqPntGet(pspp, i);
					if ((tpos < from) || (tpos > to))
					{
						othercnt++;
					}
					else
					{
						intcnt++;
						PackSeqPntPut(pspp2, tpos - from);
					}
				}
				if (! intcnt)  /* no points in region */
				{
					PackSeqPntFree(pspp2);
					break;
				}
				if (othercnt)
					*split = TRUE;
				if (pspp->fuzz != NULL)
				{
					ifp = IntFuzzNew();
					MemCopy((Pointer)ifp, (Pointer)(pspp->fuzz), sizeof(IntFuzz));
				}
				else
					ifp = NULL;

				if (strand == Seq_strand_minus)  /* rev comp */
				{
					IntFuzzClip(ifp, from, to, strand, split);
					pspp = pspp2;
					pspp2 = PackSeqPntNew();
					pspp2->strand = StrandCmp(pspp->strand);
					numpnt = PackSeqPntNum(pspp);
					numpnt--;
					for (i = numpnt; i >= 0; i--)	 /* reverse order */
					{
						tpos = PackSeqPntGet(pspp, i);
						PackSeqPntPut(pspp2, (len - tpos - 1));
					}
					PackSeqPntFree(pspp);
				}
				pspp2->id = SeqIdDup(newid);
				pspp2->fuzz = ifp;

				newhead = ValNodeNew(NULL);
				newhead->choice = SEQLOC_PACKED_PNT;
				newhead->data.ptrvalue = (Pointer)pspp2;

			}
            break;
        default:
            break;

	}
	return newhead;
}

/*****************************************************************************
*
*   IntFuzzClip()
*   	returns TRUE if clipped range values
*       in all cases, adjusts and/or complements IntFuzz
*       Designed for IntFuzz on SeqLocs
*
*****************************************************************************/
NLM_EXTERN void LIBCALL IntFuzzClip(IntFuzzPtr ifp, Int4 from, Int4 to, Uint1 strand, BoolPtr split)
{
	Int4 len, tmp;

	if (ifp == NULL) return;
	len = to - from + 1;
	switch (ifp->choice)
	{
		case 1:      /* plus/minus - no changes */
		case 3:      /* percent - no changes */
			break;
		case 2:      /* range */
			if (ifp->a > to)     /* max */
			{
				*split = TRUE;
				ifp->a = to;
			}
			if (ifp->a < from) 
			{
				*split = TRUE;
				ifp->a = from;
			}
			if (ifp->b > to)     /* min */
			{
				*split = TRUE;
				ifp->b = to;
			}
			if (ifp->b < from) 
			{
				*split = TRUE;
				ifp->b = from;
			}
			ifp->a -= from;     /* adjust to window */
			ifp->b -= to;
			if (strand == Seq_strand_minus)
			{
				tmp = len - ifp->a;   /* reverse/complement */
				ifp->a = len - ifp->b;
				ifp->b = tmp;
			}
			break;
		case 4:     /* lim */
			if (strand == Seq_strand_minus)  /* reverse/complement */
			{
				switch (ifp->a)
				{
					case 1:    /* greater than */
						ifp->a = 2;
						break;
					case 2:    /* less than */
						ifp->a = 1;
						break;
					case 3:    /* to right of residue */
						ifp->a = 4;
						break;
					case 4:    /* to left of residue */
						ifp->a = 3;
						break;
					default:
						break;
				}
			}
			break;
	}
	return;
}

/*****************************************************************************
*
* BioseqInsert (from_id, from, to, strand, to_id, pos, from_feat, to_feat,
*                                                                  do_split)
*   	Inserts a copy the region "from"-"to" on "strand" of the Bioseq
*          identified by "from_id" into the Bioseq identified by "to_id" 
*          before "pos".
*       if from_feat = TRUE, copies the feature table from "from" and updates
*          to locations to point to the proper residues in "to_id"
*       If to_feat = TRUE, updates feature table on "to_id" as well.
*          if do_split == TRUE, then splits features in "to_id" (to_feat must
*             be TRUE as well). Otherwise expands features at insertion.
*
*       All operations are copies. "frombsp" is unchanged.
*       Insert will only occur between certain Bioseq.repr classes as below
*
*   From Bioseq.repr                      To Bioseq.repr
*   
*					      virtual       raw      segmented        map
*					   +---------------------------------------------------
*	         virtual   |   length	    inst	  SeqLoc		 length
*					   +---------------------------------------------------
*				 raw   |   error        copy      SeqLoc         error
*					   +---------------------------------------------------
*		   segmented   |   error        inst      SeqLoc*        error
*					   +---------------------------------------------------
*				 map   |   error        inst*     SeqLoc         copy
*					   +---------------------------------------------------
*
*   length = changes length of "to" by length of "from"
*   error  = insertion not allowed
*   inst   = "from" instantiated as residues ("N" or "X" for virtual "from")
*   inst*  = as above, but a restriction map can instantiate other bases
*            than "N" for known restriction recognition sites.
*   copy   = copy of "from" inserted into "to"
*   SeqLoc = a SeqLoc added to "to" which points to "from". No copy of residues.
*   SeqLoc* = as above, but note that "to" points to "from" directly, not
*             what "from" itself may point to.
*   
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL BioseqInsert (SeqIdPtr from_id, Int4 from, Int4 to, Uint1 strand, SeqIdPtr to_id, Int4 pos,
			Boolean from_feat, Boolean to_feat, Boolean do_split)
{
	BioseqPtr tobsp, frombsp;
	Int4 len, i, ctr, tlen;
	Boolean from_type, to_type;
	Uint1 seqtype;
	SeqAnnotPtr sap, newsap;
	SeqFeatPtr sfp, newsfp, prevsfp, sfphead = NULL;
	BioseqContextPtr bcp;
	Boolean handled = FALSE;
	SeqPortPtr spp;
	Int2 residue;
	Boolean split, added = FALSE, do_bsadd = TRUE;
	SeqLocPtr newloc, curr, head, tloc, xloc, yloc, fake;
	SeqIntPtr sip;
	CdRegionPtr crp;
	CodeBreakPtr cbp, prevcbp, nextcbp;
	RnaRefPtr rrp;
	tRNAPtr trp;
	SeqEntryPtr oldscope;
	Uint2 entityID;
	SeqMgrFeatContext fcontext;
	ValNodePtr prods, vnp;


	if ((from_id == NULL) || (to_id == NULL)) return FALSE;

	tobsp = BioseqFind(to_id);
	if (tobsp == NULL) {
		oldscope = SeqEntrySetScope (NULL);
		if (oldscope != NULL) {
			tobsp = BioseqFind(to_id);
			SeqEntrySetScope (oldscope);
		}
	}
	if (tobsp == NULL) return FALSE;

	len = BioseqGetLen(tobsp);

	if (pos == LAST_RESIDUE)
		pos = len - 1;
	else if (pos == APPEND_RESIDUE) {
		pos = len;
	}

	if ((pos < 0) || (pos > len)) return FALSE;

	frombsp = BioseqFind(from_id);
	if (frombsp == NULL) {
		oldscope = SeqEntrySetScope (NULL);
		if (oldscope != NULL) {
			frombsp = BioseqFind(from_id);
			SeqEntrySetScope (oldscope);
		}
	}
	if (frombsp == NULL) return FALSE;
	
	from_type = ISA_na(frombsp->mol);
	to_type = ISA_na(tobsp->mol);

	if (from_type != to_type) return FALSE;

	len = BioseqGetLen(frombsp);
	if (to == LAST_RESIDUE)
		to = len - 1;
	
	if ((from < 0) || (to >= len)) return FALSE;

	len = to - from + 1;

	if (tobsp->repr == Seq_repr_virtual)
	{
		if (frombsp->repr != Seq_repr_virtual)
			return FALSE;

		handled = TRUE;                    /* just length and features */
	}

 	if ((tobsp->repr == Seq_repr_raw) || (tobsp->repr == Seq_repr_const))
	{
		if (ISA_na(tobsp->mol))
		{
			seqtype = Seq_code_iupacna;
		}
		else
		{
			seqtype = Seq_code_ncbieaa;
		}

		if (tobsp->seq_data_type != seqtype)
			BioseqRawConvert(tobsp, seqtype);
		BSSeek(tobsp->seq_data, pos, SEEK_SET);
		if (do_bsadd) {
			Nlm_BSAdd(tobsp->seq_data, len, FALSE);
		}

		i = 0;

		spp = SeqPortNew(frombsp, from, to, strand, seqtype);
		while ((residue = SeqPortGetResidue(spp)) != SEQPORT_EOF)
		{
			if (! IS_residue(residue))
			{
				ErrPost(CTX_NCBIOBJ, 1, "Non-residue in BioseqInsert [%d]",
					(int)residue);
			}
			else
			{
				BSPutByte(tobsp->seq_data, residue);
				i++;
			}
		}
		SeqPortFree(spp);

		if (i != len)
		{
			ErrPost(CTX_NCBIOBJ, 1, "Tried to insert %ld residues but %ld went in",
				len, i);
			return FALSE;
		}

		handled = TRUE;
	}

	if ((tobsp->repr == Seq_repr_seg) || (tobsp->repr == Seq_repr_ref))
	{
		sip = SeqIntNew();
		sip->id = SeqIdDup(from_id);
		sip->from = from;
		sip->to = to;
		sip->strand = strand;
		tloc = ValNodeNew(NULL);
		tloc->choice = SEQLOC_INT;
		tloc->data.ptrvalue = (Pointer)sip;
		head = NULL;
		if (tobsp->repr == Seq_repr_seg)
		{
			fake = ValNodeNew(NULL);
			fake->choice = SEQLOC_MIX;
			fake->data.ptrvalue = (Pointer)(tobsp->seq_ext);
		}
		else
			fake = (SeqLocPtr)(tobsp->seq_ext);
		curr = NULL;
		ctr = 0;
		while ((curr = SeqLocFindNext(fake, curr)) != NULL)
		{
			if ((! added) && (ctr == pos))
			{
				newloc = SeqLocAdd(&head, tloc, TRUE, TRUE);
				added = TRUE;
			}
			tlen = SeqLocLen(curr);
			if ((! added) && ((ctr + tlen) > pos))  /* split interval */
			{
				yloc = NULL;
				xloc = SeqLocAdd(&yloc, curr, TRUE, TRUE);
				i = (pos - ctr) + SeqLocStart(curr);
			    newloc = SeqLocInsert(xloc, SeqLocId(xloc), i, 0, TRUE, NULL);
				xloc = newloc;
				yloc = newloc->next;
				SeqLocAdd(&head, xloc, TRUE, TRUE);
				SeqLocAdd(&head, tloc, TRUE, TRUE);
				SeqLocAdd(&head, yloc, TRUE, TRUE);
				SeqLocFree(xloc);
				SeqLocFree(yloc);
				added = TRUE;
			}
			else
				newloc = SeqLocAdd(&head, curr, TRUE, TRUE);
			ctr += tlen;
		}
		if ((! added) && (ctr == pos))
		{
			newloc = SeqLocAdd(&head, tloc, TRUE, TRUE);
			added = TRUE;
		}
		SeqLocFree(tloc);
		SeqLocFree(fake);
		if (tobsp->repr == Seq_repr_seg)
		{
			tobsp->seq_ext = (Pointer)head;
		}
		else
		{
			tobsp->seq_ext = SeqLocPackage(head);
		}
		handled = TRUE;
	}

	if (tobsp->repr == Seq_repr_map)
	{
		if (! ((frombsp->repr == Seq_repr_map) || (frombsp->repr == Seq_repr_virtual)))
			return FALSE;

		prevsfp = NULL;
		for (sfp = (SeqFeatPtr)(tobsp->seq_ext); sfp != NULL; sfp = sfp->next)
		{
			sfp->location = SeqLocInsert(sfp->location, to_id, pos, len, TRUE, NULL);
			prevsfp = sfp;
		}

		if (frombsp->repr == Seq_repr_map)
		{
			for (sfp = (SeqFeatPtr)(frombsp->seq_ext); sfp != NULL; sfp = sfp->next)
			{
				split = FALSE;
				newloc = SeqLocCopyRegion(to_id, sfp->location, frombsp, from, to, strand, &split);
				if (newloc != NULL)   /* got one */
				{
					newsfp = (SeqFeatPtr)AsnIoMemCopy((Pointer)sfp, (AsnReadFunc)SeqFeatAsnRead, (AsnWriteFunc)SeqFeatAsnWrite);
					SeqLocFree(newsfp->location);
					newsfp->location = newloc;
					if (split)
						newsfp->partial = TRUE;
					
					if (prevsfp == NULL)
						tobsp->seq_ext = (Pointer)newsfp;
					else
						prevsfp->next = newsfp;
					prevsfp = newsfp;
		
					newsfp->location = SeqLocInsert(newsfp->location, to_id, 0,
				                                          pos, TRUE, to_id);
				}
			}
		}
		handled = TRUE;
	}

	if (! handled) return FALSE;

	tobsp->length += len;

	if (to_feat)		     /* fix up sourceid Bioseq feature table(s) */
	{
		entityID = ObjMgrGetEntityIDForPointer (tobsp);
		if (entityID > 0 && SeqMgrFeaturesAreIndexed (entityID)) {
			sfp = NULL;
			while ((sfp = SeqMgrGetNextFeature (tobsp, sfp, 0, 0, &fcontext)) != NULL)
			{
				sfp->location = SeqLocInsert (sfp->location, to_id,pos, len, do_split, NULL);
				switch (sfp->data.choice)
				{
					case SEQFEAT_CDREGION:   /* cdregion */
						crp = (CdRegionPtr)(sfp->data.value.ptrvalue);
						prevcbp = NULL;
						for (cbp = crp->code_break; cbp != NULL; cbp = nextcbp)
						{
							nextcbp = cbp->next;
							cbp->loc = SeqLocInsert (cbp->loc, to_id,pos, len, do_split, NULL);
							if (cbp->loc == NULL)
							{
								if (prevcbp != NULL)
									prevcbp->next = nextcbp;
								else
									crp->code_break = nextcbp;
								cbp->next = NULL;
								CodeBreakFree (cbp);
							}
							else
								prevcbp = cbp;
						}
						break;
					case SEQFEAT_RNA:
						rrp = (RnaRefPtr)(sfp->data.value.ptrvalue);
						if (rrp->ext.choice == 2)   /* tRNA */
						{
							trp = (tRNAPtr)(rrp->ext.value.ptrvalue);
							if (trp->anticodon != NULL)
							{
								trp->anticodon = SeqLocInsert (trp->anticodon, to_id,pos, len, do_split, NULL);
							}
						}
						break;
					default:
						break;
				}
			}

			/* adjust features pointing by product */
			prods = SeqMgrGetSfpProductList (tobsp);
			for (vnp = prods; vnp != NULL; vnp = vnp->next) {
				sfp = (SeqFeatPtr) vnp->data.ptrvalue;
				if (sfp == NULL) continue;
				sfp->product = SeqLocInsert (sfp->product, to_id,pos, len, do_split, NULL);
			}
			ValNodeFree (prods);

		} else {
			bcp = BioseqContextNew(tobsp);
			sfp = NULL;
	                     	        /* adjust features pointing by location */
			while ((sfp = BioseqContextGetSeqFeat(bcp, 0, sfp, NULL, 0)) != NULL)
			{
				sfp->location = SeqLocInsert(sfp->location, to_id,pos, len, do_split, NULL);
				switch (sfp->data.choice)
				{
					case SEQFEAT_CDREGION:   /* cdregion */
						crp = (CdRegionPtr)(sfp->data.value.ptrvalue);
						prevcbp = NULL;
						for (cbp = crp->code_break; cbp != NULL; cbp = nextcbp)
						{
							nextcbp = cbp->next;
							cbp->loc = SeqLocInsert(cbp->loc, to_id,pos, len, do_split, NULL);
							if (cbp->loc == NULL)
							{
								if (prevcbp != NULL)
									prevcbp->next = nextcbp;
								else
									crp->code_break = nextcbp;
								cbp->next = NULL;
								CodeBreakFree(cbp);
							}
							else
								prevcbp = cbp;
						}
						break;
					case SEQFEAT_RNA:
						rrp = (RnaRefPtr)(sfp->data.value.ptrvalue);
						if (rrp->ext.choice == 2)   /* tRNA */
						{
							trp = (tRNAPtr)(rrp->ext.value.ptrvalue);
							if (trp->anticodon != NULL)
							{
								trp->anticodon = SeqLocInsert(trp->anticodon, to_id,pos, len, do_split, NULL);
							}
						}
						break;
					default:
						break;
				}
			}

			sfp = NULL;
	   	                          /* adjust features pointing by product */
			while ((sfp = BioseqContextGetSeqFeat(bcp, 0, sfp, NULL, 1)) != NULL)
				sfp->product = SeqLocInsert(sfp->product, to_id,pos, len, do_split, NULL);

			BioseqContextFree(bcp);
		}
	}

	if (from_feat)				/* add source Bioseq features to sourceid */
	{
		bcp = BioseqContextNew(frombsp);
		sfp = NULL;					/* NOTE: should make NEW feature table */
		prevsfp = NULL;
	                            /* is there an old feature table to use? */
		for (newsap = tobsp->annot; newsap != NULL; newsap = newsap->next)
		{
			if (newsap->type == 1)  /* feature table */
				break;
		}
		if (newsap != NULL)
		{							/* create a new one if necessary */
			for (prevsfp = (SeqFeatPtr)(newsap->data); prevsfp != NULL;
			                                            prevsfp = prevsfp->next)
			{
				if (prevsfp->next == NULL)
					break;
			}
		}
		                                     /* get features by location */
		while ((sfp = BioseqContextGetSeqFeat(bcp, 0, sfp, NULL, 0)) != NULL)
		{									/* copy all old features */
			split = FALSE;
			newloc = SeqLocCopyRegion(to_id, sfp->location, frombsp, from, to, strand, &split);
			if (newloc != NULL)   /* got one */
			{
				newsfp = (SeqFeatPtr)AsnIoMemCopy((Pointer)sfp, (AsnReadFunc)SeqFeatAsnRead, (AsnWriteFunc)SeqFeatAsnWrite);
				SeqLocFree(newsfp->location);
				newsfp->location = newloc;

				if (split)
					newsfp->partial = TRUE;

				if (prevsfp == NULL)
					sfphead = newsfp;
				else
					prevsfp->next = newsfp;
				prevsfp = newsfp;

				newsfp->location = SeqLocInsert(newsfp->location, to_id, 0,
				                                          pos, TRUE, to_id);
				switch (newsfp->data.choice)
				{
					case SEQFEAT_CDREGION:   /* cdregion */
						crp = (CdRegionPtr)(newsfp->data.value.ptrvalue);
						prevcbp = NULL;
						for (cbp = crp->code_break; cbp != NULL; cbp = nextcbp)
						{
							nextcbp = cbp->next;
							cbp->loc = SeqLocCopyRegion(to_id, cbp->loc, frombsp, from, to, strand, &split);
							if (cbp->loc == NULL)
							{
								if (prevcbp != NULL)
									prevcbp->next = nextcbp;
								else
									crp->code_break = nextcbp;
								cbp->next = NULL;
								CodeBreakFree(cbp);
							}
							else
							{
								cbp->loc = SeqLocInsert(cbp->loc, to_id, 0,
				                                          pos, TRUE, to_id);
								prevcbp = cbp;
							}
						}
						break;
					case SEQFEAT_RNA:
						rrp = (RnaRefPtr)(newsfp->data.value.ptrvalue);
						if (rrp->ext.choice == 2)   /* tRNA */
						{
							trp = (tRNAPtr)(rrp->ext.value.ptrvalue);
							if (trp->anticodon != NULL)
							{
								trp->anticodon = SeqLocCopyRegion(to_id, trp->anticodon, frombsp, from, to, strand, &split);
								trp->anticodon = SeqLocInsert(trp->anticodon, to_id, 0,
				                                          pos, TRUE, to_id);
							}
						}
						break;
					default:
						break;
				}
			}
		}

		sfp = NULL;
								/* get features by product */
		while ((sfp = BioseqContextGetSeqFeat(bcp, 0, sfp, NULL, 1)) != NULL)
		{									/* copy all old features */
			split = FALSE;
			newloc = SeqLocCopyRegion(to_id, sfp->product, frombsp, from, to, strand, &split);
			if (newloc != NULL)   /* got one */
			{
				newsfp = (SeqFeatPtr)AsnIoMemCopy((Pointer)sfp, (AsnReadFunc)SeqFeatAsnRead, (AsnWriteFunc)SeqFeatAsnWrite);
				SeqLocFree(newsfp->product);
				newsfp->product = newloc;
				if (split)
					newsfp->partial = TRUE;

				if (prevsfp == NULL)
					sfphead = newsfp;
				else
					prevsfp->next = newsfp;
				prevsfp = newsfp;

				newsfp->product = SeqLocInsert(newsfp->product, to_id, 0, pos,
				                                                TRUE, to_id);
			}
		}
		BioseqContextFree(bcp);
	

		if (sfphead != NULL)    /* orphan chain of seqfeats to attach */
		{
			if (newsap == NULL)
			{
				for (sap = tobsp->annot; sap != NULL; sap = sap->next)
				{
					if (sap->next == NULL)
						break;
				}
				newsap = SeqAnnotNew();
				newsap->type = 1;
				if (sap == NULL)
					tobsp->annot = newsap;
				else
					sap->next = newsap;
			}

			newsap->data = (Pointer)sfphead;
		}
	}

	return TRUE;
}

/*****************************************************************************
*
*   SeqLocInsert()
*       alters "head" by insert "len" residues before "pos" in any SeqLoc
*         on the Bioseq "target"
*       all SeqLocs not on "target" are unaltered
*       for SeqLocs on "target"
*          all SeqLocs before "pos" are unaltered
*          all SeqLocs >= "pos" are incremented by "len"
*          all SeqLocs spanning "pos"
*             if "split" == TRUE, are split into two SeqLocs, one to the
*               left of the insertion, the other to right
*             if "split" != TRUE, the SeqLoc is increased in length to cover
*               the insertion
*   	returns altered head or NULL if nothing left.
*       if ("newid" != NULL) replaces "target" with "newid" whether the
*          SeqLoc is altered on not.
*
*       Usage hints:
*          1) To update a feature location on "target" when 10 residues of
*               sequence have been inserted before position 5
*          SeqFeatPtr->location = SeqLocInsert ( SeqFeatPtr->location ,
*                "target", 5, 10, TRUE, NULL);  [for some feature types
*                      you may want "split" equal FALSE]
*          2) To insert the complete feature table from "source" into a
*                different Bioseq "dest" before position 20 in "dest"
*          SFP->location = SeqLocInsert(SFP->location, "source", 0, 20,
*                FALSE, "dest");
*   
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL SeqLocInsert (SeqLocPtr head, SeqIdPtr target, Int4 pos, Int4 len,
                                               Boolean split, SeqIdPtr newid)
{
	SeqIntPtr sip, sip2;
	SeqPntPtr spp;
	PackSeqPntPtr pspp, pspp2;
	SeqBondPtr sbp;
	SeqLocPtr slp, tmp, prev, next, thead, tmp2;
	Int4 diff, numpnt, i, tpos;
	Uint1 oldchoice;
	ValNode vn;
	SeqIdPtr sidp;

	if ((head == NULL) || (target == NULL))
		return head;

	head->next = NULL;   /* caller maintains chains */

	diff = len;

    switch (head->choice)
    {
        case SEQLOC_BOND:   /* bond -- 2 seqs */
			vn.next = NULL;
			vn.choice = SEQLOC_PNT;

			sbp = (SeqBondPtr)(head->data.ptrvalue);
			vn.data.ptrvalue = (Pointer)(sbp->a);
			SeqLocInsert(&vn, target, pos, len, split, newid);
			sbp->a = (SeqPntPtr)(vn.data.ptrvalue);
			if (sbp->b != NULL)
			{
				vn.data.ptrvalue = (Pointer)(sbp->b);
				SeqLocInsert(&vn, target, pos, len, split, newid);
				sbp->b = (SeqPntPtr)(vn.data.ptrvalue);
			}
			break;
        case SEQLOC_FEAT:   /* feat -- can't track yet */
        case SEQLOC_NULL:    /* NULL */
			break;
        case SEQLOC_EMPTY:    /* empty */
        case SEQLOC_WHOLE:    /* whole */
			if (newid != NULL)
			{
				sidp = (SeqIdPtr)(head->data.ptrvalue);
				if (SeqIdForSameBioseq(sidp, target))
				{
					SeqIdFree(sidp);
					sidp = SeqIdDup(newid);
					head->data.ptrvalue = (Pointer)sidp;
				}
			}
			break;
        case SEQLOC_MIX:    /* mix -- more than one seq */
        case SEQLOC_EQUIV:    /* equiv -- ditto */
        case SEQLOC_PACKED_INT:    /* packed int */
			prev = NULL;
			thead = NULL;
			for (slp = (SeqLocPtr)(head->data.ptrvalue); slp != NULL; slp = next)
			{
				next = slp->next;
				oldchoice = slp->choice;
				tmp = SeqLocInsert(slp, target, pos, len, split, newid);
				if (tmp != NULL)
				{
					if ((head->choice != SEQLOC_EQUIV) &&
						(oldchoice != tmp->choice))  /* split interval? */
					{
						if ((oldchoice == SEQLOC_INT) &&
							(tmp->choice == SEQLOC_PACKED_INT))
						{
							tmp2 = tmp;
							tmp = (SeqLocPtr)(tmp2->data.ptrvalue);
							MemFree(tmp2);
							while (tmp->next != NULL)
							{
								if (prev != NULL)
									prev->next = tmp;
								else
									thead = tmp;
								prev = tmp;
								tmp = tmp->next;
							}
						}
					}
					if (prev != NULL)
						prev->next = tmp;
					else
						thead = tmp;
					prev = tmp;
				}
			}
			head->data.ptrvalue = thead;
			if (thead == NULL)
				head = SeqLocFree(head);
            break;
        case SEQLOC_INT:    /* int */
			sip = (SeqIntPtr)(head->data.ptrvalue);
			if (SeqIdForSameBioseq(sip->id, target))
			{
				if (newid != NULL)   /* change id? */
				{
					SeqIdFree(sip->id);
					sip->id = SeqIdDup(newid);
				}

				if (sip->to < pos)  /* completely before insertion */
				{
					break;
				}

				if ((! split) || (sip->from >= pos))  /* interval unbroken */
				{
					if (sip->from >= pos)
						sip->from += len;
					sip->to += len;
					break;
				}

						                          /* split interval */
				sip2 = SeqIntNew();
				slp = ValNodeNew(NULL);
				slp->choice = SEQLOC_INT;
				slp->data.ptrvalue = (Pointer)sip2;
				sip2->strand = sip->strand;
				sip2->id = SeqIdDup(sip->id);

				sip2->to = sip->to + len;
				sip2->from = pos + len;
				sip2->if_to = sip->if_to;
				sip->if_to = NULL;
				sip->to = pos - 1;
				head->next = slp;

				if (sip->strand == Seq_strand_minus)  /* reverse order */
				{
					head->data.ptrvalue = (Pointer)sip2;
					slp->data.ptrvalue = (Pointer)sip;
				}

				thead = head;   /* make split interval into PACKED_INT */
				head = ValNodeNew(NULL);
				head->choice = SEQLOC_PACKED_INT;
				head->data.ptrvalue = thead;

			}
            break;
        case SEQLOC_PNT:    /* pnt */
			spp = (SeqPntPtr)(head->data.ptrvalue);
			if (SeqIdForSameBioseq(spp->id, target))
			{
				if (newid != NULL)   /* change id? */
				{
					SeqIdFree(spp->id);
					spp->id = SeqIdDup(newid);
				}

				if (spp->point >= pos)
					spp->point += len;
			}
            break;
        case SEQLOC_PACKED_PNT:    /* packed pnt */
			pspp = (PackSeqPntPtr)(head->data.ptrvalue);
			if (SeqIdForSameBioseq(pspp->id, target))
			{
				if (newid != NULL)   /* change id? */
				{
					SeqIdFree(pspp->id);
					pspp->id = SeqIdDup(newid);
				}

				numpnt = PackSeqPntNum(pspp);
				pspp2 = PackSeqPntNew();
				head->data.ptrvalue = pspp2;
				for (i = 0; i < numpnt; i++)
				{
					tpos = PackSeqPntGet(pspp, i);
					if (tpos >= pos)
						tpos += len;
					PackSeqPntPut(pspp2, tpos);
				}
				pspp2->id = pspp->id;
				pspp->id = NULL;
				pspp2->fuzz = pspp->fuzz;
				pspp->fuzz = NULL;
				pspp2->strand = pspp->strand;
				PackSeqPntFree(pspp);
			}
            break;
        default:
            break;
    }

	if (head == NULL)
		ErrPost(CTX_NCBIOBJ, 1, "SeqLocInsert: lost a SeqLoc");

	return head;
}

/*****************************************************************************
*
*   SeqLocSubtract (SeqLocPtr head, SeqLocPtr piece)
*   	Deletes piece from head.
*       head may be changed.
*       returns the changed head.
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL SeqLocSubtract (SeqLocPtr head, SeqLocPtr piece)
{
	SeqLocPtr slp = NULL;
	SeqIdPtr sip;
	Int4 from, to;
	Boolean changed = FALSE;

	if ((head == NULL) || (piece == NULL))
		return NULL;

	while ((slp = SeqLocFindNext(piece, slp)) != NULL)
	{
		sip = SeqLocId(slp);
		from = SeqLocStart(slp);
		to = SeqLocStop(slp);
		head = SeqLocDelete(head, sip, from, to, FALSE, &changed);
	}

	return head;
}

/********************************************************************
*
* SeqLocReplaceID
*   replaces the Seq-Id in a Seq-Loc (slp) with a new Seq-Id (new_sip)
*
**********************************************************************/
NLM_EXTERN SeqLocPtr SeqLocReplaceID (SeqLocPtr slp, SeqIdPtr new_sip)
{
  SeqLocPtr        curr;
  PackSeqPntPtr    pspp;
  SeqIntPtr        target_sit;
  SeqBondPtr       sbp;
  SeqPntPtr        spp;

  switch (slp->choice) {
     case SEQLOC_PACKED_INT :
     case SEQLOC_MIX :
     case SEQLOC_EQUIV :
        curr = NULL;
        while ((curr = SeqLocFindNext (slp, curr)) != NULL) {
           curr = SeqLocReplaceID (curr, new_sip);
        }
        break;
     case SEQLOC_PACKED_PNT :
        pspp = (PackSeqPntPtr) slp->data.ptrvalue;
        if (pspp != NULL) {
          SeqIdFree (pspp->id);
          pspp->id = SeqIdDup (new_sip);
        }
        break;
     case SEQLOC_EMPTY :
     case SEQLOC_WHOLE :
        SeqIdFree ((SeqIdPtr) slp->data.ptrvalue);
        slp->data.ptrvalue = (Pointer) SeqIdDup (new_sip);
        break;
     case SEQLOC_INT :
        target_sit = (SeqIntPtr) slp->data.ptrvalue;
        SeqIdFree (target_sit->id);
        target_sit->id = SeqIdDup (new_sip);
        break;
     case SEQLOC_PNT :
        spp = (SeqPntPtr) slp->data.ptrvalue;
        SeqIdFree(spp->id);
        spp->id = SeqIdDup(new_sip);
        break;
     case SEQLOC_BOND :
        sbp = (SeqBondPtr) slp->data.ptrvalue;
        if (sbp == NULL || sbp->a == NULL || sbp->b == NULL) break;
        /* only do this if both ends bonded to same Seq-id */
        if (SeqIdMatch (sbp->a->id, sbp->b->id)) {
          spp = sbp->a;
          SeqIdFree(spp->id);
          spp->id = SeqIdDup(new_sip);
          spp = sbp->b;
          SeqIdFree(spp->id);
          spp->id = SeqIdDup(new_sip);
        }
        break;
     default :
        break;
  }
  return slp;
}

/**********************************************************
 *
 *   NLM_EXTERN SeqLocPtr LIBCALL GapToSeqLoc(range):
 *
 *      Gets the size of gap and constructs SeqLoc block with
 *   $(seqlitdbtag) value as Dbtag.db and Dbtag.tag.id = 0.
 *
 **********************************************************/
NLM_EXTERN SeqLocPtr LIBCALL GapToSeqLoc(Int4 range)
{
    SeqLocPtr slp;
    SeqIntPtr sip;
    SeqIdPtr  sidp;
    DbtagPtr  dp;

    if(range < 0)
        return(NULL);

    slp = ValNodeNew(NULL);
    if(range == 0)
    {
        slp->choice = SEQLOC_NULL;
        slp->data.ptrvalue = NULL;
        slp->next = NULL;
        return(slp);
    }

    dp = DbtagNew();
    dp->db = StringSave(seqlitdbtag);
    dp->tag = ObjectIdNew();
    dp->tag->id = 0;
    dp->tag->str = NULL;

    sidp = ValNodeNew(NULL);
    sidp->choice = SEQID_GENERAL;
    sidp->data.ptrvalue = dp;

    sip = SeqIntNew();
    sip->from = 0;
    sip->to = range - 1;
    sip->id = sidp;

    slp->choice = SEQLOC_INT;
    slp->data.ptrvalue = sip;

    return(slp);
}

/**********************************************************
 *
 *   NLM_EXTERN Boolean LIBCALL ISAGappedSeqLoc(slp):
 *
 *      Looks at a single SeqLoc item. If it has the SeqId
 *   of type GENERAL with Dbtag.db == $(seqlitdbtag) and
 *   Dbtag.tag.id == 0, then returns TRUE, otherwise
 *   returns FALSE.
 *
 **********************************************************/
NLM_EXTERN Boolean LIBCALL ISAGappedSeqLoc(SeqLocPtr slp)
{
    SeqIdPtr sip;
    DbtagPtr dp;

    if(slp == NULL)
        return(FALSE);

    sip = SeqLocId(slp);
    if(sip == NULL || sip->choice != SEQID_GENERAL)
        return(FALSE);

    dp = (DbtagPtr) sip->data.ptrvalue;
    if(dp == NULL || dp->db == NULL || dp->tag == NULL)
        return(FALSE);

    if(StringCmp(seqlitdbtag, dp->db) == 0 && dp->tag->id == 0)
        return(TRUE);

    return(FALSE);
}

/**********************************************************
 *
 *   NLM_EXTERN DeltaSeqPtr LIBCALL GappedSeqLocsToDeltaSeqs(slp):
 *
 *      This functions is used only in the case, if ISAGappedSeqLoc()
 *   has returned TRUE.
 *      Converts SeqLoc set to the sequence of DeltaSeqs.
 *   Gbtag'ed SeqLocs it turns into SeqLits with the only "length"
 *   element. The regular SeqLocs saves as they are. Returns
 *   obtained DeltaSeq.
 *
 **********************************************************/
NLM_EXTERN DeltaSeqPtr LIBCALL GappedSeqLocsToDeltaSeqs(SeqLocPtr slp)
{
    DeltaSeqPtr res;
    DeltaSeqPtr dsp;
    SeqIntPtr   sip;
    SeqLitPtr   slip;

    dsp = ValNodeNew(NULL);
    dsp->next = NULL;
    dsp->choice = 0;
    res = dsp;
    for(; slp != NULL; slp = slp->next)
    {
        if(ISAGappedSeqLoc(slp) != FALSE)
        {
            dsp->next = ValNodeNew(NULL);
            dsp = dsp->next;
            sip = slp->data.ptrvalue;
            slip = SeqLitNew();
            slip->length = sip->to - sip->from + 1;
            dsp->choice = 2;
            dsp->data.ptrvalue = slip;
        }
        else
        {
            dsp->next = ValNodeNew(NULL);
            dsp = dsp->next;
            dsp->choice = 1;
            dsp->data.ptrvalue = AsnIoMemCopy((Pointer) slp,
                                              (AsnReadFunc) SeqLocAsnRead,
                                              (AsnWriteFunc) SeqLocAsnWrite);
        }
    }
    dsp = res->next;
    MemFree(res);
    return(dsp);
}
