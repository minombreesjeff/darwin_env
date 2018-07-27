/*
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
* File Name:  gather.c
*
* Author:  Jim Ostell, Jinghui Zhang, Jonathan Kans
*
* Version Creation Date:   10/7/94
*
* $Revision: 6.38 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: gather.c,v $
* Revision 6.38  2002/05/08 18:58:09  kans
* itemID is Uint4
*
* Revision 6.37  2002/04/13 20:23:53  kans
* fixed get next descriptor unindexed
*
* Revision 6.36  2001/11/15 18:47:13  kans
* fix to unindexed get next descriptor
*
* Revision 6.35  2001/11/15 18:34:52  kans
* added GetNextDescriptorUnindexed, requires AssignIDsInEntity be called first
*
* Revision 6.34  2001/07/06 17:27:38  kans
* AssignIDs does not clear deleteme flag
*
* Revision 6.33  2000/12/18 14:48:07  tatiana
* turn off reordering
*
* Revision 6.32  2000/02/07 16:48:34  kans
* fixed setting of context->index in SeqMgrGetNextFeatureByID
*
* Revision 6.31  2000/02/01 22:25:44  kans
* indexed speedup now returns features in itemID order, not position order, to allow simple diff of e2index
*
* Revision 6.30  2000/01/07 03:01:42  kans
* indexed speedup only if raw bioseq not part of segmented bioseq
*
* Revision 6.29  2000/01/07 02:48:13  kans
* useSeqMgrIndexes now works with get_feats_location and get_feats_product, should be suitable for e2index speedup
*
* Revision 6.28  2000/01/07 00:56:02  kans
* targeted bioseq presents features right after bioseq is visited
*
* Revision 6.27  2000/01/07 00:18:25  kans
* targeted bioseq gather visits each seqannot, presents features in seqannot index
*
* Revision 6.26  2000/01/06 23:32:13  kans
* targeted bioseq gather visits each seqfeat, checks against feature/bioseq index
*
* Revision 6.25  1999/11/30 17:07:51  egorov
* Protect against dividing by zero.
*
* Revision 6.24  1999/10/29 18:06:26  kans
* added GetPointerForIDs (with SW)
*
* Revision 6.23  1999/09/30 18:33:09  kans
* delete marked objects corrects gatherindex.prevlink on remaining objects
*
* Revision 6.22  1999/09/29 21:10:53  kans
* delete marked seqannot also frees seqannot if it has no remaining sap->data components
*
* Revision 6.21  1999/09/29 18:24:53  kans
* added DeleteMarkedObjects
*
* Revision 6.20  1999/09/28 18:10:15  kans
* added DeleteMarkedObjectsProc callback - not yet tested
*
* Revision 6.19  1999/09/28 12:10:24  kans
* finished implementing lightweight GatherObjectsInEntity
*
* Revision 6.18  1999/09/27 22:02:45  kans
* further implementation of lightweight gather replacement
*
* Revision 6.17  1999/09/27 17:46:59  kans
* uses GatherIndex structure
*
* Revision 6.16  1999/09/26 20:44:25  kans
* implemented most of VisitProc callbacks
*
* Revision 6.15  1999/09/26 00:17:14  kans
* VisitObjectsInEntity prototype added
*
* Revision 6.14  1999/09/25 01:46:08  kans
* AssignIDsInEntity split into internal function that can also call callback, EXTRA_OBJMGR_FIELDS values assigned to some objects
*
* Revision 6.13  1999/09/07 17:59:52  kans
* AssignIDsInEntity takes datatype and dataptr for when entityID is 0, allowing unlinked components to be updated
*
* Revision 6.12  1999/09/07 17:13:09  kans
* added AssignIDsInEntity
*
* Revision 6.11  1999/09/01 14:41:26  shavirin
* Adjusted functions gather_align_data() and get_align_ends() to accept
* discontinuous alignment type.
*
* Revision 6.10  1999/07/26 20:55:52  ostell
* added recursive support for SAS_DISC SeqAlign
*
* Revision 6.9  1999/03/16 13:17:28  ostell
* changes in SeqLocOffset() to deal with multi-interval seqloc which is
* a subset of a feature.
*
* Revision 6.8  1999/01/13 23:34:19  kans
* added GatherSpecificProcLaunch
*
* Revision 6.7  1998/08/24 18:27:07  kans
* removed solaris -v -fd warnings
*
* Revision 6.6  1998/06/23 16:53:37  zjing
* modify function check_reverse_strand
*
* Revision 6.5  1998/03/10 20:43:28  kans
* IGCCBuild now handles delta seqs
*
* Revision 6.4  1998/03/02 22:15:35  zjing
* fix gap collection in the minus strand in function gather_align_data
*
* Revision 6.3  1997/11/19 22:14:48  ostell
* added support for multithreaded programs
*
* Revision 6.2  1997/10/10 17:01:37  kans
* support for individual elements of OBJ_BIOSEQ_DELTA
*
* Revision 6.1  1997/09/30 19:59:35  zjing
* bug fixes in gather_align_data
*
* Revision 6.0  1997/08/25 18:05:46  madden
* Revision changed to 6.0
*
* Revision 5.15  1997/08/13 18:43:46  zjing
* correct errors in gather_align_data for tblastn and tblastx
*
* Revision 5.14  1997/07/01 15:16:33  zjing
* fix the strand in gathering continous std-seg
*
* Revision 5.13  1997/06/19 18:37:47  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.12  1997/04/23 12:44:49  zjing
* correct a bug for tblastn display
*
 * Revision 5.10  1997/03/17  21:22:00  kans
 * detach bioseq/bioseq set didn't unlink gcp->sep->next
 *
 * Revision 5.9  1997/01/15  17:25:45  zjing
 * a kludge fix for ck_extreme to display the features across zero properly
 *
 * Revision 5.8  1996/12/16  19:43:37  ostell
 * added an attachment/replacement of a Seq-loc to a feature in AttachDataProc
 *
 * Revision 5.7  1996/11/05  18:00:03  zjing
 * fix the problem of integer overflow for gather_align_data and speed up the process
 *
 * Revision 5.6  1996/08/29  01:18:34  ostell
 * added GatherAddExtraLoc for codebreak and trna.atncodon mapping
 *
 * Revision 5.5  1996/08/09  20:28:55  epstein
 * eliminate update_seq_loc()
 *
 * Revision 5.4  1996/08/06  19:56:03  kans
 * for SEQLOC_WHOLE, must call SeqIdFindBest on bsp->id
 *
 * Revision 5.3  1996/06/17  21:49:42  zjing
 * fix in gather_align_data for multiple alignments
 *
 * Revision 5.2  1996/06/10  18:35:36  zjing
 * fix in get_end_diag_val
 *
 * Revision 5.1  1996/06/10  15:08:53  epstein
 * replace make_seq_loc() with SeqLocIntNew() and make_pnt_loc with SeqLocPntNew()
 *
 * Revision 5.0  1996/05/28  13:23:23  ostell
 * Set to revision 5.0
 *
 * Revision 4.30  1996/05/22  20:35:25  ostell
 * changed GatherProcLaunch to cycle through proceedures in priority
 * order looking for OM_MSG_RET_DONE return, instead of just launching
 * the first one found.
 *
 * Revision 4.29  1996/05/06  14:49:12  zjing
 * fix a strand in load_align_data
 *
 * Revision 4.28  1996/04/24  20:12:13  ostell
 * removed an uncessary variable
 *
 * Revision 4.27  1996/04/08  15:44:02  kans
 * IGCC build scopes on omdp->choice or scope->scope
 *
 * Revision 4.26  1996/03/08  18:12:38  zjing
 * fix in check_reverse_strand for gather_align_data
 *
 * Revision 4.25  1996/02/28  04:53:06  ostell
 * added ObjMgrHold suport
 *
 * Revision 4.23  1996/01/22  13:28:11  kans
 * cast first parameter of MemSet to (Pointer) for SunOS compiler
 *
 * Revision 4.22  1996/01/03  23:01:04  ostell
 * added GatherOverWrite() to support find/replace external to Gather
 *
 * Revision 4.21  1995/12/22  20:12:01  ostell
 * added protection for NULL pointer on scope in IGCCBuild
 *
 * Revision 4.20  1995/12/22  14:42:30  ostell
 * added do_not_reload_from_cache to GatherScope
 * modified calls to support it
 * changed default behavior of gather to load and reclock entities
 *
 * Revision 4.19  1995/12/20  22:55:36  kans
 * bsp added to FocusSeqEntry, and MemSet (OBJ_MAX+1) bug fixed (JZ)
 *
 * Revision 4.18  1995/12/20  19:19:39  ostell
 * added GatherContext.igccp field
 * added FocusSeqEntry() function
 *
 * Revision 4.17  1995/12/20  15:05:15  zjing
 * Fix gather_align_data to get the end gaps in master sequence
 *
 * Revision 4.16  1995/12/15  15:17:07  kans
 * bsp is now initialized in IGCCBuild
 *
 * Revision 4.15  1995/12/15  02:47:01  ostell
 * fix to scoping for multiple records with same SeqId
 *
 * Revision 4.14  1995/12/14  21:43:04  ostell
 * added scope protection to IGCCBuild to protect against mulitple copies
 *
 * Revision 4.13  1995/12/13  19:28:13  ostell
 * more support for OBJ_SEQHIST_ALIGN
 *
 * Revision 4.12  1995/12/13  18:50:42  ostell
 * added support for OBJ_SEQHIST_ALIGN
 *
 * Revision 4.11  1995/11/21  23:08:38  ostell
 * added support in GatherContext for gatherstack
 *
 * Revision 4.10  1995/11/06  21:29:03  ostell
 * added newid and convert_loc to GatherScope, and new_loc to GatherContext
 * added functions ReMapIntFuzz and SeqLocReMap to support them
 * This allows SeqLocs on features to be copied into a remapped form by gather
 *
 * Revision 4.9  1995/10/06  19:25:24  ostell
 * added fields "ignore_top" and "stop_on_annot" to GatherScope
 * if "ignore_top" is TRUE, features on seglevel 0 are ignored
 * if "stop_on_annot" is TRUE, segments are traversed to a maximum depth
 * of gsp->seglevel, but traversing is stopped as soon as an annotation is
 * found.
 *
 * Revision 4.8  1995/10/01  20:51:28  kans
 * made GatherItemByDataProc static
 *
 * Revision 4.7  1995/09/30  03:38:31  ostell
 * Changed ObjMgrMessage functions to pass a structure
 * Added support for selecting regions
 * Added ability to remove entity when no more views on it
 *
 * Revision 4.6  1995/09/27  19:50:09  zjing
 * .
 *
 * Revision 4.1  1995/08/16  17:48:34  kans
 * add a chain parameter for gather Seq-align (jz)
 *
 * Revision 4.0  1995/07/26  13:49:01  ostell
 * force revision to 4.0
 *
 * Revision 1.38  1995/07/10  15:51:59  kans
 * changes in gather_align_data (zjing)
 *
 * Revision 1.37  1995/07/08  15:22:09  ostell
 * Set ObjMgrDirtyFlag on Attach..,Detach..,ReplaceDataForProc
 *
 * Revision 1.36  1995/06/02  17:53:17  kans
 * add gather range to gather bioseq
 *
 * Revision 1.35  1995/06/01  21:53:55  kans
 * support for Seq-align (zjing)
 *
 * Revision 1.34  1995/05/19  15:49:37  kans
 * fixed bug in mapping minus strand intervals
 *
 * Revision 1.33  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*
*
* ==========================================================================
*/

#include <gather.h>
#include <edutil.h>
#include <subutil.h>
#include <objfdef.h>
#include <explore.h>

static Boolean NEAR GatherSeqEntryFunc PROTO((SeqEntryPtr sep, InternalGCCPtr igccp, Pointer parent, Uint2 parenttype, SeqEntryPtr prev, Boolean in_scope, Pointer PNTR prevlink));
static Boolean NEAR GatherItemFunc PROTO((Uint2 entityID, Uint2 itemID, Uint2 itemtype,
                                   Pointer userdata, GatherItemProc userfunc, Pointer dataptr,
								   Boolean do_not_reload_from_cache));
static Boolean NEAR GatherAddToStack PROTO((GatherContextPtr gcp));
static void NEAR GatherAddExtraLoc PROTO((GatherContextPtr gcp, SeqLocPtr slp));

static void NEAR GatherAddExtraLoc (GatherContextPtr gcp, SeqLocPtr slp)
{
	SeqLocPtr PNTR tmp;

	gcp->extra_loc_cnt++;
	if (gcp->extra_loc_cnt > gcp->extra_loc_total)
	{
		tmp = gcp->extra_loc;
		gcp->extra_loc = MemNew((size_t)(sizeof(SeqLocPtr) *(gcp->extra_loc_total + 5)));
		MemMove (gcp->extra_loc, tmp,(size_t)(sizeof(SeqLocPtr) *
			(gcp->extra_loc_total)));
		MemFree(tmp);
		gcp->extra_loc_total += 5;
	}
	gcp->extra_loc[gcp->extra_loc_cnt - 1] = slp;
	return;
}

static Boolean NEAR GatherAddToStack (GatherContextPtr gcp)
{
	GatherElementPtr tmp;
	Int2 oldsize;
		
	if (gcp == NULL)
		return FALSE;

	if (gcp->numstack <= gcp->indent)  /* expand stack */
	{
		oldsize = gcp->numstack;
		tmp = gcp->gatherstack;
		gcp->numstack = oldsize + 20;
		gcp->gatherstack = MemNew((size_t)(sizeof(GatherElement) * (gcp->numstack)));
		if (oldsize)
		{
			MemCopy(gcp->gatherstack, tmp, 	(size_t)(sizeof(GatherElement) * (oldsize)));
			MemFree(tmp);
		}
	}

	tmp = gcp->gatherstack + gcp->indent;
	tmp->itemID = gcp->itemID;
	tmp->itemtype = gcp->thistype;
	tmp->tempload = gcp->tempload;
	tmp->thisitem = gcp->thisitem;

	return TRUE;
}

static Boolean ck_extreme(SeqLocPtr slp, BoolPtr across_zero)
{
	SeqLocPtr one_loc;
	SeqIdPtr sip;
	Boolean has_prev;

	*across_zero = FALSE;
	switch(slp->choice)
	{
		case SEQLOC_MIX:
		case SEQLOC_PACKED_PNT:
		case SEQLOC_PACKED_INT:
		   sip = SeqLocId(slp);
		   if(sip == NULL)
			return FALSE;
		   one_loc = NULL;
		   has_prev = FALSE;
		   while((one_loc = SeqLocFindNext(slp, one_loc))!=NULL)
		   {
			if(SeqLocStart(one_loc) == 0)
			{
				if(has_prev && SeqLocStrand(one_loc) != Seq_strand_minus)
					*across_zero = TRUE;
				else if(!has_prev && SeqLocStrand(one_loc) == Seq_strand_minus)
					*across_zero = TRUE;
			}
			if(one_loc->choice == SEQLOC_NULL)
				return FALSE;
			has_prev = TRUE;
		   }
		   return TRUE;

		default:
			return FALSE;
	}
}

static Boolean ck_parts_overlap(SeqLocPtr slp, SeqLocPtr seq_loc)
{
	SeqLocPtr one_loc;

	one_loc = NULL;
	while((one_loc = SeqLocFindNext(slp, one_loc))!=NULL)
	{
		if(one_loc->choice != SEQLOC_NULL)
		{
			if(SeqLocCompare(one_loc, seq_loc) != SLC_NO_MATCH)
				return TRUE;
		}
	}

	return FALSE;
}
	

/*****************************************************************************
*
*   SeqLocOffset(seq_loc, sfp_loc, range, offset)
*   	returns FALSE if seq_loc does not overlap sfp_loc
*   	else fills in range structure mapping sfp_loc to seq_loc
*       adds offset to final values
*       if (ends) will assure that left is always <= right
*
*****************************************************************************/
NLM_EXTERN Boolean SeqLocOffset (SeqLocPtr seq_loc, SeqLocPtr sfp_loc, GatherRangePtr range, Int4 offset)
{
  Uint1    strand_loc, strand_sfp;
  Int4 temp;
  SeqLoc sl;
  SeqInt si;
  Boolean across_zero;
  Int4 toffset, l, r, t;
  Boolean ltrunc, rtrunc;
  SeqLocPtr tslp;

  if (seq_loc == NULL || sfp_loc == NULL || range == NULL) {
    return FALSE;
  }


  if(ck_extreme(sfp_loc, &across_zero))
  {
	if(!across_zero)
	{
		si.from = SeqLocStart(sfp_loc);
		si.to = SeqLocStop(sfp_loc);
		si.strand = SeqLocStrand(sfp_loc);
		si.id = SeqLocId(sfp_loc);
		sl.choice = SEQLOC_INT;
		sl.data.ptrvalue = &si;
		sfp_loc = &sl;
	}
	else if(!ck_parts_overlap(sfp_loc, seq_loc))
		return FALSE;
		
  }

  strand_sfp = SeqLocStrand(sfp_loc);

  toffset = 0;
  l = INT4_MAX;
  r = -1;
  tslp = NULL;

  while ((tslp = SeqLocFindNext(seq_loc, tslp)) != NULL)
  {
	if (SeqLocCompare(tslp, sfp_loc)) {

		strand_loc = SeqLocStrand(tslp);

		t = GetOffsetInLoc(sfp_loc, tslp, SEQLOC_LEFT_END);
		if (t == -1) { /* truncated */
			if (strand_loc == Seq_strand_minus)
				t = toffset + SeqLocLen(tslp) - 1;
			else
				t = toffset;
			
			if (t < l)
			{
				l = t;
				ltrunc = TRUE;
			}
		}
		else if ((t + toffset) < l)
		{
			l = t + toffset;
			ltrunc = FALSE;
		}

		t = GetOffsetInLoc(sfp_loc, tslp, SEQLOC_RIGHT_END);
		if (t == -1)
		{
			if (strand_loc == Seq_strand_minus)
				t = toffset;
			else
				t = toffset + SeqLocLen (tslp) - 1;
			if (t > r) {
				r = t;
				rtrunc = TRUE;
			}

		}
		else if ((t + toffset) > r)
		{
			rtrunc = FALSE;
			r = t + toffset;
		}

	}
	toffset += SeqLocLen(tslp);
  }

  if (r == -1) /* didn't find it */
	return FALSE;

  range->l_trunc = ltrunc;
  range->r_trunc = rtrunc;
  range->left = l + offset;
  range->right = r + offset;

  strand_loc = SeqLocStrand(seq_loc);

  if (strand_loc == Seq_strand_minus)
	range->strand = StrandCmp(strand_sfp);
  else
  {
	if(strand_sfp == Seq_strand_unknown)
		strand_sfp = Seq_strand_plus;
	range->strand = strand_sfp;
  }

  if(range->left > range->right)
  {
	temp = range->left;
	range->left = range->right;
	range->right = temp;
  }

  return TRUE;
}


/*****************************************************************************
*
*   ReMapIntFuzz()
*
*****************************************************************************/
NLM_EXTERN IntFuzzPtr ReMapIntFuzz(IntFuzzPtr ifp, Boolean rev, SeqLocPtr seq_loc, SeqLocPtr sfp_loc, Int4 offset)
{
	IntFuzzPtr newfuzz=NULL;
	SeqInt si;
	SeqPnt sp;
	ValNode vn;
	GatherRange range;
	Int4 i;

	if (ifp == NULL) return newfuzz;

	newfuzz = MemNew(sizeof(IntFuzz));
	MemCopy(newfuzz, ifp, sizeof(IntFuzz));

	switch (ifp->choice)
	{
		case 1:      /* plus/minus - no changes */
		case 3:      /* percent - no changes */
			break;
		case 2:      /* range */
			vn.choice = SEQLOC_INT;
			vn.next = NULL;
			vn.data.ptrvalue = &si;
			MemSet((Pointer)(&si), 0, sizeof(SeqInt));
			si.id = SeqLocId(sfp_loc);
			if (si.id == NULL)
			{
				newfuzz = MemFree(newfuzz);
				break;
			}
			si.strand = SeqLocStrand(sfp_loc);
			if (! SeqLocOffset(seq_loc, &vn, &range, offset))
			{
				newfuzz = MemFree(newfuzz);
				break;
			}

			newfuzz->a = range.right;  /* max */
			newfuzz->b = range.left;	/* min */
			break;
		case 4:     /* lim */
			if (rev)  /* reverse/complement */
			{
				switch (newfuzz->a)
				{
					case 1:    /* greater than */
						newfuzz->a = 2;
						break;
					case 2:    /* less than */
						newfuzz->a = 1;
						break;
					case 3:    /* to right of residue */
						newfuzz->a = 4;
						break;
					case 4:    /* to left of residue */
						newfuzz->a = 3;
						break;
					default:
						break;
				}
			}
			break;
		case 5:      /* alternate positions */
			vn.choice = SEQLOC_PNT;
			vn.next = NULL;
			vn.data.ptrvalue = &sp;
			MemSet((Pointer)(&si), 0, sizeof(SeqPnt));
			sp.id = SeqLocId(sfp_loc);
			if (sp.id == NULL)
			{
				newfuzz = MemFree(newfuzz);
				break;
			}
			sp.strand = SeqLocStrand(sfp_loc);

			newfuzz->alt = MemNew((size_t)(sizeof(Int4) * ifp->b));
			newfuzz->a = 0;

			for (i = 0; i < ifp->a; i++)
			{
				sp.point = ifp->alt[i];

				if (SeqLocOffset(seq_loc, &vn, &range, offset))
				{
					newfuzz->alt[newfuzz->a] = range.left;
					newfuzz->a++;
				}
			}

			if (newfuzz->a == 0)
				newfuzz = IntFuzzFree(newfuzz);

			break;
		default:
			newfuzz = MemFree(newfuzz);
			break;

	}
	return newfuzz;
}


NLM_EXTERN SeqLocPtr SeqLocReMap (SeqIdPtr newid, SeqLocPtr seq_loc, SeqLocPtr head, Int4 offset, Boolean rev)
{
	GatherRange range;
	Uint1 the_strand;

	SeqLocPtr newhead = NULL, last=NULL, tmp, slp, prev, next, thead;
	SeqIntPtr sip, sip2;
	SeqPntPtr spp, spp2;
	PackSeqPntPtr pspp, pspp2;
	SeqBondPtr sbp, sbp2;
	Int4 numpnt, i, tpos, intcnt, othercnt;
	Pointer ptr = NULL;
	Boolean dropped_first, dropped_last, was_equiv = FALSE;
	IntFuzzPtr ifp, ifp1, ifp2;
	ValNode vn;
	SeqPnt sp;
	
	if ((head == NULL) || (seq_loc == NULL) || (newid == NULL)) return NULL;

	if (! SeqLocOffset(seq_loc, head, &range, offset))
		return NULL;

	switch (head->choice)
	{
		case SEQLOC_PACKED_PNT:
			pspp = (PackSeqPntPtr)(head->data.ptrvalue);
			numpnt = PackSeqPntNum(pspp);

			sp.id = pspp->id;
			sp.strand = pspp->strand;
			sp.fuzz = NULL;
			vn.next = NULL;
			vn.choice = SEQLOC_PNT;
			vn.data.ptrvalue = &sp;

			pspp2 = PackSeqPntNew();
			the_strand = range.strand;
			intcnt = 0;	     /* use for included points */
			othercnt = 0;	 /* use for exclued points */
			for (i = 0; i < numpnt; i++)
			{
				sp.point = PackSeqPntGet(pspp, i);

				if (SeqLocOffset(seq_loc, &vn, &range, offset))
				{
					intcnt++;
					PackSeqPntPut(pspp2, range.left);
				}
				else
					othercnt++;
			}
			if (! intcnt)  /* no points in region */
			{
				PackSeqPntFree(pspp2);
				break;
			}

			if (rev)  /* rev comp */
			{
				pspp = pspp2;
				pspp2 = PackSeqPntNew();
				numpnt = PackSeqPntNum(pspp);
				numpnt--;
				for (i = numpnt; i >= 0; i--)	 /* reverse order */
				{
					tpos = PackSeqPntGet(pspp, i);
					PackSeqPntPut(pspp2, tpos);
				}
				PackSeqPntFree(pspp);
			}
			pspp2->id = SeqIdDup(newid);
			pspp2->strand = the_strand;
			pspp2->fuzz = ReMapIntFuzz(pspp->fuzz, rev, seq_loc, head, offset);

			newhead = ValNodeNew(NULL);
			newhead->choice = SEQLOC_PACKED_PNT;
			newhead->data.ptrvalue = (Pointer)pspp2;
            break;
        case SEQLOC_WHOLE:    /* whole */
			newhead = ValNodeNew(NULL);
			sip2 = SeqIntNew();
			sip2->id = SeqIdDup(newid);
			sip2->from = range.left;
			sip2->to = range.right;
			sip2->strand = range.strand;
			if (range.r_trunc)
			{
				ifp = IntFuzzNew();
				ifp->choice = 4;   /* lim */
				ifp->a = 1;        /* greater than */
				sip2->if_to = ifp;
			}

			if (range.l_trunc)
			{
				ifp = IntFuzzNew();
				ifp->choice = 4;   /* lim */
				ifp->a = 2;        /* less than */
				sip2->if_from = ifp;
			}

			newhead->choice = SEQLOC_INT;
			newhead->data.ptrvalue = (Pointer)sip2;
			break;
        case SEQLOC_PNT:    /* pnt */
			spp = (SeqPntPtr)(head->data.ptrvalue);

			spp2 = SeqPntNew();
			spp2->id = SeqIdDup(newid);
			spp2->point = range.left;
			spp2->strand = range.strand;
			spp2->fuzz = ReMapIntFuzz(spp->fuzz, rev, seq_loc, head, offset);

			newhead = ValNodeNew(NULL);
			newhead->choice = SEQLOC_PNT;
			newhead->data.ptrvalue = (Pointer)spp2;
            break;
        case SEQLOC_INT:    /* int */
			sip = (SeqIntPtr)(head->data.ptrvalue);

			sip2 = SeqIntNew();
			sip2->id = SeqIdDup(newid);
			sip2->strand = range.strand;
			sip2->from = range.left;
			sip2->to = range.right;

			if (rev)  /* reverse ends if seq_loc on complement */
			{
				ifp1 = sip->if_to;
				ifp2 = sip->if_from;
			}
			else
			{
				ifp1 = sip->if_from;
				ifp2 = sip->if_to;
			}


			if (range.r_trunc)
			{
				ifp = IntFuzzNew();
				ifp->choice = 4;   /* lim */
				ifp->a = 1;        /* greater than */
				sip2->if_to = ifp;
			}
			else
			{
				sip2->if_to = ReMapIntFuzz(ifp2, rev, seq_loc, head, offset);
			}

			if (range.l_trunc)
			{
				ifp = IntFuzzNew();
				ifp->choice = 4;   /* lim */
				ifp->a = 2;        /* less than */
				sip2->if_from = ifp;
			}
			else
			{
				sip2->if_from = ReMapIntFuzz(ifp1, rev, seq_loc, head, offset);
			}

			newhead = ValNodeNew(NULL);
			newhead->choice = SEQLOC_INT;
			newhead->data.ptrvalue = (Pointer)sip2;
            break;
        case SEQLOC_BOND:   /* bond -- 2 seqs */
			sbp2 = NULL;
			sbp = (SeqBondPtr)(head->data.ptrvalue);
			vn.choice = SEQLOC_PNT;
			vn.data.ptrvalue = sbp->a;
			vn.next = NULL;
			tmp = SeqLocReMap (newid, seq_loc, (SeqLocPtr)(&vn), offset, rev);
			if (tmp != NULL)
			{
			 	sbp2 = SeqBondNew();
				sbp2->a = (SeqPntPtr)(tmp->data.ptrvalue);
				MemFree(tmp);
			}
			if (sbp->b != NULL)
			{
				vn.data.ptrvalue = sbp->b;
				tmp = SeqLocReMap (newid, seq_loc, (SeqLocPtr)(&vn), offset, rev);
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
			}
			break;
        case SEQLOC_FEAT:   /* feat -- can't track yet */
        case SEQLOC_NULL:    /* NULL */
        case SEQLOC_EMPTY:    /* empty */
			break;
        case SEQLOC_EQUIV:    /* does it stay equiv? */
		    was_equiv = TRUE;
        case SEQLOC_MIX:    /* mix -- more than one seq */
        case SEQLOC_PACKED_INT:    /* packed int */
			prev = NULL;
			thead = NULL;
			dropped_first = FALSE;
			dropped_last = FALSE;
			for (slp = (SeqLocPtr)(head->data.ptrvalue); slp != NULL; slp = next)
			{
				next = slp->next;
				if (slp->choice == SEQLOC_NULL)   /* special case */
				{
					tmp = NULL;
					if ((prev != NULL) && (next != NULL))
					{
						if (prev->choice != SEQLOC_NULL)
						{
							tmp = ValNodeNew(NULL);
							tmp->choice = SEQLOC_NULL;
						}
					}
				}
				else
					tmp = SeqLocReMap (newid, seq_loc, slp, offset, rev);
				if (tmp != NULL)
				{
					dropped_last = FALSE;
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
						}
					}
					else if (tmp->choice == SEQLOC_NULL)
					{
						tmp = SeqLocFree(tmp);
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
				{
					if (prev == NULL)
						dropped_first = TRUE;
					else
						dropped_last = TRUE;
				}
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
				}
			}
			if (thead != NULL)
			{
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

					rev = FALSE; /* KLUDGE: turn off reordering */
					if (! rev)
						newhead->data.ptrvalue = (Pointer)thead;
					else                            /* reverse order */
					{
						tmp = NULL;
						while (thead != NULL)
						{
							prev = NULL;
							for (slp = thead; slp->next != NULL; slp = slp->next)
								prev = slp;
							if (prev != NULL)
								prev->next = NULL;
							else
								thead = NULL;
							if (tmp != NULL)
								tmp->next = slp;
							else
								newhead->data.ptrvalue = (Pointer)slp;
							slp->next = NULL;
							tmp = slp;
						}

					}
				}
				else                 /* only one SeqLoc left */
					newhead = thead;

				if ((! was_equiv) && ((dropped_first) || (dropped_last)))
				{                       /* add Int_fuzz when intervals are dropped */
					if (rev)
					{
						was_equiv = dropped_first;
						dropped_first = dropped_last;
						dropped_last = was_equiv;
					}

					slp = NULL;
					tmp = NULL;
					while ((slp = SeqLocFindNext(newhead, slp)) != NULL)
					{
						if ((tmp == NULL) && (dropped_first))   /* first one */
						{
							switch (slp->choice)
							{
								case SEQLOC_INT:
									ifp = IntFuzzNew();
									ifp->choice = 4;   /* lim */
									ifp->a = 2;        /* assume lt */
									sip = (SeqIntPtr)(slp->data.ptrvalue);
									if (sip->strand != Seq_strand_minus)
									{
										sip->if_from = IntFuzzFree(sip->if_from);
										sip->if_from = ifp;
									}
									else
									{
										sip->if_to = IntFuzzFree(sip->if_to);
										sip->if_to = ifp;
										ifp->a = 1;   /* gt */
									}
									break;
								default:
									break;
							}
						}
						tmp = slp;
					}

					if ((tmp != NULL) && (dropped_last))
					{
						switch (tmp->choice)
						{
							case SEQLOC_INT:
								ifp = IntFuzzNew();
								ifp->choice = 4;   /* lim */
								ifp->a = 1;        /* assume gt */
								sip = (SeqIntPtr)(tmp->data.ptrvalue);
								if (sip->strand != Seq_strand_minus)
								{
									sip->if_from = IntFuzzFree(sip->if_from);
									sip->if_from = ifp;
								}
								else
								{
									sip->if_to = IntFuzzFree(sip->if_to);
									sip->if_to = ifp;
									ifp->a = 2;   /* lt */
								}
								break;
							default:
								break;
						}
					}
				}

			}
            break;
        default:
            break;

	}
	return newhead;
}

	/** citttype is currently 0, or 1=OBJ_SEQFEAT_CIT **/

static Boolean NEAR GatherPub(InternalGCCPtr gccp, ValNodePtr vnp,
        Uint1 cittype, Uint1 ttype, Pointer tparent, Pointer PNTR prevlink, Boolean in_scope)
{
	GatherContextPtr gcp;
	GatherScopePtr gsp;
	Boolean takeit;
	Int2 LocateItem = 0;
	Pointer LocateData = NULL;
	Uint1 thistype;

	if (vnp == NULL) return TRUE;

	gcp = &(gccp->gc);
	gsp = &(gccp->scope);

	if (cittype)
		thistype = OBJ_SEQFEAT_CIT;
	else
		thistype = OBJ_PUB;

	if (gsp->ignore[thistype])
		return TRUE;

	takeit = in_scope;

	if (gccp->locatetype == thistype)
	{
		LocateItem = gccp->locateID;
		LocateData = gccp->locatePtr;
	}

	gcp->parentitem = tparent;
	gcp->parenttype = ttype;

	gccp->itemIDs[thistype]++;

	if (LocateItem == gccp->itemIDs[thistype])
		takeit = TRUE;
	if (LocateData == (Pointer)vnp)
		takeit = TRUE;
	if (takeit)
	{
		gcp->itemID = gccp->itemIDs[thistype];
		gcp->thisitem = (Pointer)vnp;
		gcp->thistype = thistype;
		gcp->prevlink = prevlink;
		GatherAddToStack(gcp);
		if (! (*(gccp->userfunc))(gcp))
			return FALSE;
		if (LocateItem) return FALSE;
		if (LocateData != NULL) return FALSE;
	}

	return TRUE;
}

static Boolean NEAR GatherPubSet(InternalGCCPtr gccp, ValNodePtr vnp,
        Uint1 cittype, Uint1 ttype, Pointer tparent, Pointer PNTR prevlink, Boolean in_scope)
{
	GatherContextPtr gcp;
	GatherScopePtr gsp;
	Boolean takeit;
	Int2 LocateItem = 0;
	Pointer LocateData = NULL;
	Uint1 thistype;
	ValNodePtr vnp2;

	if (vnp == NULL) return TRUE;

	gcp = &(gccp->gc);
	gsp = &(gccp->scope);

	if (gsp->ignore[OBJ_PUB_SET])
		return TRUE;

	takeit = in_scope;

	if (gccp->locatetype == OBJ_PUB_SET)
 	{
		LocateItem = gccp->locateID;
		LocateData = gccp->locatePtr;
	}

	gcp->previtem = NULL;
	gcp->prevtype = OBJ_PUB_SET;
	gcp->parentitem = tparent;
	gcp->parenttype = ttype;
	thistype = OBJ_PUB_SET;

	gccp->itemIDs[OBJ_PUB_SET]++;
	if (LocateItem == gccp->itemIDs[OBJ_PUB_SET])
		takeit = TRUE;
	if (LocateData == (Pointer)vnp)
		takeit = TRUE;

	gcp->itemID = gccp->itemIDs[OBJ_PUB_SET];
	gcp->thisitem = (Pointer)vnp;
	gcp->thistype = thistype;
	GatherAddToStack(gcp);

	if (takeit)
	{
		gcp->prevlink = prevlink;
		if (! (*(gccp->userfunc))(gcp))
			return FALSE;
		if (LocateItem) return FALSE;
		if (LocateData != NULL) return FALSE;
	}

	gcp->indent++;
	prevlink = &(vnp->data.ptrvalue);
	gcp->previtem = NULL;
	gcp->prevtype = OBJ_PUB;

	for (vnp2 = (ValNodePtr)(vnp->data.ptrvalue); vnp2 != NULL; vnp2 = vnp2->next)
	{
		if (! GatherPub(gccp, vnp2, cittype, thistype, (Pointer)vnp,
						    prevlink, in_scope))
			return FALSE;
		prevlink = (Pointer PNTR)&(vnp2->next);
		gcp->previtem = (Pointer)vnp2;
	
	}
	gcp->indent--;

	return TRUE;
}

static Boolean NEAR GatherSeqIds(InternalGCCPtr gccp, SeqIdPtr sip,
        Uint1 ttype, Pointer tparent, Pointer PNTR prevlink)
{
	GatherContextPtr gcp;
	GatherScopePtr gsp;
	Boolean takeit;
	Int2 LocateItem = 0;
	Pointer LocateData = NULL;
	Uint1 thistype;

	if (sip == NULL) return TRUE;

	gcp = &(gccp->gc);
	gsp = &(gccp->scope);

	if (gsp->ignore[OBJ_SEQID])
		return TRUE;

	if (gccp->locatetype == OBJ_SEQID)
 	{
		LocateItem = gccp->locateID;
		LocateData = gccp->locatePtr;
	}
	else
		takeit = TRUE;

	gcp->previtem = NULL;
	gcp->prevtype = OBJ_SEQID;
	gcp->parentitem = tparent;
	gcp->parenttype = ttype;
	thistype = OBJ_SEQID;

	while (sip != NULL)
	{
		gccp->itemIDs[OBJ_SEQID]++;
		if (LocateItem == gccp->itemIDs[OBJ_SEQID])
			takeit = TRUE;
		if (LocateData == (Pointer)sip)
			takeit = TRUE;
		if (takeit)
		{
			gcp->itemID = gccp->itemIDs[OBJ_SEQID];
			gcp->thisitem = (Pointer)sip;
			gcp->thistype = thistype;
			gcp->prevlink = prevlink;
			GatherAddToStack(gcp);
			if (! (*(gccp->userfunc))(gcp))
				return FALSE;
			if (LocateItem) return FALSE;
			if (LocateData != NULL) return FALSE;
		}

		gcp->previtem = (Pointer)sip;
		prevlink = (Pointer PNTR)&(sip->next);
		sip = sip->next;
	}

	return TRUE;
}

static Boolean NEAR GatherSeqDescr(InternalGCCPtr gccp, ValNodePtr vnp,
       Uint1 ttype, Pointer tparent, Pointer PNTR prevlink, Boolean in_scope)
{
	GatherContextPtr gcp;
	GatherScopePtr gsp;
	Boolean takeit;
	Int2 LocateItem = 0;
	Pointer LocateData = NULL;
	Uint1 thistype;

	if (vnp == NULL) return TRUE;

	gcp = &(gccp->gc);
	gsp = &(gccp->scope);

	if (gsp->ignore[OBJ_SEQDESC])
		return TRUE;

	if (gccp->locatetype == OBJ_SEQDESC)
 	{
		LocateItem = gccp->locateID;
		LocateData = gccp->locatePtr;
	}

	if ((LocateItem) || (LocateData != NULL))   /* fetching an item */
	{
		takeit = FALSE;
	}
	else
	{
		takeit = in_scope;       /* if ! in_scope don't take it */
		if (gccp->bsp != NULL)   /* gsp->target set a Bioseq */
		{
			if (tparent != (Pointer)(gccp->bsp))
			{
				if (! ObjMgrIsChild(tparent, (Pointer)(gccp->bsp)))
					takeit = FALSE;   /* not in propagation path */
				else
					gcp->propagated = TRUE;
			}
		}
	}

	gcp->previtem = NULL;
	gcp->prevtype = OBJ_SEQDESC;
	gcp->parentitem = tparent;
	gcp->parenttype = ttype;
	thistype = OBJ_SEQDESC;

	while (vnp != NULL)
	{
		gccp->itemIDs[OBJ_SEQDESC]++;
		if (LocateItem == gccp->itemIDs[OBJ_SEQDESC])
			takeit = TRUE;
		if (LocateData == (Pointer)vnp)
			takeit = TRUE;
		if (takeit)
		{
			gcp->itemID = gccp->itemIDs[OBJ_SEQDESC];
			gcp->thisitem = (Pointer)vnp;
			gcp->thistype = thistype;
			gcp->prevlink = prevlink;
			GatherAddToStack(gcp);
			if (! (*(gccp->userfunc))(gcp))
				return FALSE;
			if (LocateItem) return FALSE;
			if (LocateData != NULL) return FALSE;
		}

		gcp->previtem = (Pointer)vnp;
		prevlink = (Pointer PNTR)&(vnp->next);
		vnp = vnp->next;
	}
	gcp->propagated = FALSE;   /* reset propagated flag */

	return TRUE;
}


static Int4 get_site_offset(SeqLocPtr slp, SeqLocPtr head, Int4 r_len)
{
	Uint1 m_strand, s_strand;

	m_strand = SeqLocStrand(slp);
	s_strand = SeqLocStrand(slp);

	if(m_strand == 0 || s_strand ==0)
		return 0;
	if(m_strand == 3 || s_strand == 3)
		return 0;

	if(m_strand == s_strand)
		return 0;

	if(m_strand == Seq_strand_plus && s_strand == Seq_strand_minus)
		return (-r_len);
	else
		return r_len;
}


static Boolean process_packed_pnt(SeqLocPtr slp, SeqLocPtr head, Int4 r_len, Int4 offset, GatherContextPtr gcp, Int2Ptr max_interval)
{
        PackSeqPntPtr pspp;
	Int4 site;
	Int4 site_offset;	/*for treating restriction site as an interval*/
	Int2 index;
	Int4 m_start, m_stop;
	Uint1 m_strand;
	Int4 min, max;
	Boolean rev;
	Int4 pos, ctr, i;
	GatherRangePtr trdp, lrdp;
	GatherRange trange;
	Boolean is_end = FALSE;

	if(head->choice !=SEQLOC_PACKED_PNT)
		return FALSE;

	if(!SeqIdForSameBioseq(SeqLocId(slp), SeqLocId(head)))
		return FALSE;
	m_strand = SeqLocStrand(slp);
	site_offset = get_site_offset(slp, head, (r_len-1));
	rev = (SeqLocStrand(slp) == Seq_strand_minus);
	lrdp = gcp->rdp;


	m_start = SeqLocStart(slp);
	m_stop = SeqLocStop(slp);
	pspp = head->data.ptrvalue;
        site =0;
        index =0;
	min = -1;
	max = -1;
	ctr = 0;
        while( !is_end && ((site = PackSeqPntGet(pspp, index))!= -1))
        {
                ++index;
		if (ctr >= (*max_interval))
		{
			trdp = lrdp;
			lrdp = (GatherRangePtr)MemNew((size_t)((*max_interval + 20) * sizeof(GatherRange)));
			MemCopy(lrdp, trdp, (size_t)(*max_interval * sizeof(GatherRange)));
			MemFree(trdp);
			*max_interval += 20;
			gcp->rdp = lrdp;
		}
		is_end = (site > m_stop);
                if(site >= m_start && site <=m_stop)
                {
			site += site_offset;
                        if(rev)
				pos = offset + (m_stop - site);
			else
				pos = offset + (site - m_start);
			if(max == -1)
			{
				max = pos;
				min = pos;
			}
			else
			{
				max = MAX(pos, max);
				min = MIN(pos, min);
			}
			lrdp[ctr].left = pos;
			lrdp[ctr].right = pos;
			lrdp[ctr].l_trunc = FALSE;
			lrdp[ctr].r_trunc = FALSE;
			lrdp[ctr].strand = m_strand;
			++ctr;
		}
	}

	if (ctr)     /* got some */
	{
		gcp->extremes.left = min;
		gcp->extremes.right = max;
		gcp->extremes.l_trunc = FALSE;
		gcp->extremes.r_trunc = FALSE;
		gcp->extremes.strand = m_strand;

		gcp->num_interval = (Int2)ctr;
		if (rev)    /* reverse order on rev location */
		{
			i = 0;
			ctr--;
			while (i < ctr)
			{
				MemCopy(&trange, &(lrdp[i]), sizeof(GatherRange));
				MemCopy(&(lrdp[i]), &(lrdp[ctr]), sizeof(GatherRange));
				MemCopy(&(lrdp[ctr]), &trange, sizeof(GatherRange));
				i++; ctr++;
			}
		}
		return TRUE;
	}
	else
		return FALSE;
}

/* functions to speed up targeted feature gather by using seqmgr explore index */

static ObjMgrDataPtr GatherGetOmdpForBioseq (BioseqPtr bsp)

{
  ObjMgrDataPtr  omdp;
  ObjMgrPtr      omp;

  if (bsp == NULL) return NULL;
  omdp = (ObjMgrDataPtr) bsp->omdp;
  if (omdp != NULL) return omdp;
  omp = ObjMgrWriteLock ();
  omdp = ObjMgrFindByData (omp, bsp);
  ObjMgrUnlock ();
  bsp->omdp = (Pointer) omdp;
  return omdp;
}

static SeqFeatPtr LIBCALL SeqMgrGetNextFeatureByID (BioseqPtr bsp, SeqFeatPtr curr,
                                                    Uint1 seqFeatChoice, Uint1 featDefChoice,
                                                    SeqMgrFeatContext PNTR context)

{
  BioseqExtraPtr      bspextra;
  Uint2               entityID;
  SMFeatItemPtr PNTR  featsByID;
  Uint2               i;
  SMFeatItemPtr       item;
  ObjMgrDataPtr       omdp;
  Uint1               seqfeattype;

  if (context == NULL) return NULL;

  if (curr == NULL) {
    if (bsp == NULL) return NULL;
    omdp = GatherGetOmdpForBioseq (bsp);
    if (omdp == NULL || omdp->datatype != OBJ_BIOSEQ) return NULL;

    context->omdp = (Pointer) omdp;
    context->index = 0;
  }

  omdp = (ObjMgrDataPtr) context->omdp;
  if (omdp == NULL) return NULL;
  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return NULL;
  featsByID = bspextra->featsByID;
  if (featsByID == NULL || bspextra->numfeats < 1) return NULL;

  entityID = ObjMgrGetEntityIDForPointer (omdp->dataptr);

  i = context->index;

  while (i < bspextra->numfeats) {
    item = featsByID [i];
    if (item != NULL) {
      curr = item->sfp;
      i++;
      if (curr != NULL) {
        seqfeattype = curr->data.choice;
        if ((seqFeatChoice == 0 || seqfeattype == seqFeatChoice) &&
            (featDefChoice == 0 || item->subtype == featDefChoice) &&
            (! item->ignore)) {
          context->entityID = entityID;
          context->itemID = item->itemID;
          context->sfp = curr;
          context->sap = item->sap;
          context->bsp = item->bsp;
          context->label = item->label;
          context->left = item->left;
          context->right = item->right;
          context->dnaStop = item->dnaStop;
          context->partialL = item->partialL;
          context->partialR = item->partialR;
          context->farloc = item->farloc;
          context->strand = item->strand;
          context->seqfeattype = seqfeattype;
          context->featdeftype = item->subtype;
          context->numivals = item->numivals;
          context->ivals = item->ivals;
          context->userdata = NULL;
          context->omdp = (Pointer) omdp;
          context->index = i;
          return curr;
        }
      }
    }
  }

  return NULL;
}

static Boolean NEAR ExploreSeqFeat (
  InternalGCCPtr gccp,
  BioseqPtr bsp,
  Boolean in_scope
)

{
  SeqMgrFeatContext  fcontext;
  GatherContextPtr   gcp;
  GatherScopePtr     gsp;
  SeqFeatPtr         sfp;
  Boolean            takecit;
  ValNodePtr         vnp;

  /* gccp->locatetype is known to be NULL if target used */

  if (gccp == NULL || bsp == NULL) return TRUE;

  gcp = &(gccp->gc);
  gsp = &(gccp->scope);

  if (gsp->ignore [OBJ_SEQFEAT]) return TRUE;
  if (gsp->ignore [OBJ_SEQFEAT_CIT]) {
    takecit = FALSE;
  } else {
    takecit = TRUE;
  }

  gcp->num_interval = 0;

  if (gsp->get_feats_location) {
    sfp = SeqMgrGetNextFeatureByID (bsp, NULL, 0, 0, &fcontext);
    while (sfp != NULL) {

      gcp->previtem = NULL;
      gcp->prevtype = OBJ_SEQFEAT;

      gcp->parentitem = sfp->idx.parentptr;
      gcp->parenttype = sfp->idx.parenttype;

      gcp->itemID = sfp->idx.itemID;
      gcp->thisitem = (Pointer) sfp;
      gcp->thistype = OBJ_SEQFEAT;
      gcp->prevlink = sfp->idx.prevlink;

      gcp->product = FALSE;

      GatherAddToStack (gcp);
      if (! (*(gccp->userfunc)) (gcp)) return FALSE;

      if (sfp->cit != NULL && takecit) {
        if (! GatherPubSet (gccp, sfp->cit, 1, OBJ_SEQFEAT, (Pointer) sfp,
                            (Pointer PNTR) &(sfp->cit), in_scope))
          return FALSE;
      }

      sfp = SeqMgrGetNextFeatureByID (bsp, sfp, 0, 0, &fcontext);
    }
  }

  if (gsp->get_feats_product) {
    for (vnp = SeqMgrGetSfpProductList (bsp); vnp != NULL; vnp = vnp->next) {
      sfp = (SeqFeatPtr) vnp->data.ptrvalue;
      if (sfp != NULL) {

        gcp->previtem = NULL;
        gcp->prevtype = OBJ_SEQFEAT;

        gcp->parentitem = sfp->idx.parentptr;
        gcp->parenttype = sfp->idx.parenttype;

        gcp->itemID = sfp->idx.itemID;
        gcp->thisitem = (Pointer) sfp;
        gcp->thistype = OBJ_SEQFEAT;
        gcp->prevlink = sfp->idx.prevlink;

        gcp->product = TRUE;

        GatherAddToStack (gcp);
        if (! (*(gccp->userfunc)) (gcp)) return FALSE;

        if (sfp->cit != NULL && takecit) {
          if (! GatherPubSet (gccp, sfp->cit, 1, OBJ_SEQFEAT, (Pointer) sfp,
                              (Pointer PNTR) &(sfp->cit), in_scope))
            return FALSE;
        }
      }
    }
  }

  /* ignoring sfp->cit on non-targeted records, not keeping counter in synch */

  return TRUE;
}

static Boolean NEAR GatherSeqFeat(InternalGCCPtr gccp, SeqFeatPtr sfp,
           Uint1 ttype, Pointer tparent, Pointer PNTR prevlink, Boolean in_scope, Uint1 sfptype)
{
	GatherContextPtr gcp;
	GatherScopePtr gsp;
	Boolean takeit=TRUE,
		takecit, checkseq=FALSE;
	SeqLocPtr slp, head, tslp, target[2];
	GatherRangePtr rdp, trdp, lrdp;
	Int4 offset, totlen, left_end;
	Boolean rev, revs[2];
	Int2 ctr, max_interval, i, numcheck, j;
	GatherRange trange;
	Int2 LocateItem = 0;
	Pointer LocateData = NULL;
	Uint1 thistype;
	Boolean is_packed_pnt = FALSE;	/*is the seq-loc a packed point?*/
	Boolean stop_now, convert_loc = FALSE;
	SeqIdPtr newid;
	CdRegionPtr cdr;
	CodeBreakPtr cbp;
	RnaRefPtr rrp;
	tRNAPtr trp;


	SeqFeatPtr prevsfp = NULL;

	if (sfp == NULL) return TRUE;

	gcp = &(gccp->gc);
	gsp = &(gccp->scope);

	if (gsp->ignore[sfptype])
		return TRUE;

	if (gccp->locatetype == sfptype)
 	{
		LocateItem = gccp->locateID;
		LocateData = gccp->locatePtr;
	}
	else
		LocateItem = 0;

	if (gsp->target != NULL)
	{
		checkseq = TRUE;
		numcheck = 1;
		target[0] = gsp->target;
		revs[0] = gccp->rev;
		if (gccp->segloc != NULL)
		{
			numcheck = 2;
			target[1] = gccp->segloc;
			revs[1] = FALSE;
		}
		rdp = &(gcp->extremes);
		offset = gsp->offset;
		max_interval = gccp->max_interval;
		lrdp = gcp->rdp;

		convert_loc = gsp->convert_loc;
		newid = gsp->newid;
	}

	if (gsp->ignore[OBJ_SEQFEAT_CIT])
		takecit = FALSE;
	else
		takecit = TRUE;

	gcp->prevtype = sfptype;
	gcp->parentitem = tparent;
	gcp->parenttype = ttype;
	gcp->num_interval = 0;
	thistype = sfptype;

	while (sfp != NULL)
	{
		gcp->previtem = (Pointer) prevsfp;
		gccp->itemIDs[sfptype]++;
		if (LocateItem == gccp->itemIDs[sfptype])
			in_scope = TRUE;
		if (LocateData == (Pointer)sfp)
			in_scope = TRUE;

		gcp->itemID = gccp->itemIDs[sfptype];
		takeit = TRUE;
		stop_now = FALSE;

		if (in_scope)
		{
			gcp->thisitem = (Pointer)sfp;
			gcp->thistype = thistype;
			gcp->prevlink = prevlink;
			gcp->product = FALSE;
			head = sfp->location;
			if (checkseq)    /* find by SeqLoc overlap */
			{
				takeit = FALSE;
				is_packed_pnt = (head->choice == SEQLOC_PACKED_PNT);
				for (j = 0; ((j < numcheck) && (! takeit) && (!stop_now)); j++)
				{
					slp = target[j];
					rev = revs[j];
					if (gsp->get_feats_location)
					{
						if(is_packed_pnt)
						{
							if(process_packed_pnt(slp, head, 0, offset, gcp, &(gccp->max_interval)))
							{
								takeit = TRUE;
								stop_now= TRUE;
							}
						}
						else
							takeit = SeqLocOffset(slp, head, rdp, offset);
					}

					if ((! takeit) && (gsp->get_feats_product))
					{
						head = sfp->product;
						takeit = SeqLocOffset(slp, head, rdp, offset);
						if (takeit)
							gcp->product = TRUE;
					}

					if ((takeit) && (! gsp->nointervals) && (!stop_now))  /* map intervals in loc */
					{
						tslp = NULL;
						ctr = 0;
						while ((tslp = SeqLocFindNext(head, tslp)) != NULL)
						{
							if (ctr >= max_interval)
							{
								trdp = lrdp;
								lrdp = (GatherRangePtr)MemNew((size_t)((max_interval + 20) * sizeof(GatherRange)));
								MemCopy(lrdp, trdp, (size_t)(max_interval * sizeof(GatherRange)));
								MemFree(trdp);
								max_interval += 20;
								gccp->max_interval = max_interval;
								gcp->rdp = lrdp;
							}
							if (SeqLocOffset(slp, tslp, &(lrdp[ctr]), offset))
								ctr++;
						}
						if (ctr)     /* got some */
						{
							gcp->num_interval = ctr;
							if (rev)    /* reverse order on rev location */
							{
								i = 0;
								ctr--;
								while (i < ctr)
								{
									MemCopy(&trange, &(lrdp[i]), sizeof(GatherRange));
									MemCopy(&(lrdp[i]), &(lrdp[ctr]), sizeof(GatherRange));
									MemCopy(&(lrdp[ctr]), &trange, sizeof(GatherRange));
									i++; ctr--;
								}
							}
						}
					}

					if ((takeit) && (convert_loc) && (! stop_now)) /* convert SeqLoc */
					{
						gcp->new_loc = SeqLocReMap(newid, slp, head, offset, rev);
						if (gsp->get_feats_location)
						{
							gcp->extra_loc_cnt = 0;
							if (sfp->data.choice == SEQFEAT_CDREGION)
							{
								cdr = (CdRegionPtr)(sfp->data.value.ptrvalue);
								for (cbp = cdr->code_break; cbp != NULL;
										cbp = cbp->next)
								{
									tslp = SeqLocReMap(newid, slp, cbp->loc, offset, rev);
									GatherAddExtraLoc(gcp, tslp);
								}
							}
							else if (sfp->data.choice == SEQFEAT_RNA)
							{
								rrp = (RnaRefPtr)(sfp->data.value.ptrvalue);
								if ((rrp->ext.choice == 2) && 
									(rrp->ext.value.ptrvalue != NULL)) /* tRNA */
								{
									trp = (tRNAPtr)(rrp->ext.value.ptrvalue);
									if (trp->anticodon != NULL)
									{
										tslp = SeqLocReMap(newid, slp, trp->anticodon,
											offset, rev);
										GatherAddExtraLoc(gcp, tslp);
									}
								}
							}
						}
					}
				}
			}

			if (takeit)
			{
				if (gccp->segcnt)    /* which segment was it in? */
				{
					left_end = offset;
					for (i = 0; i < gccp->segcnt; i++)
					{
						totlen = left_end + gccp->seglens[i];
						if ((rdp->left >= left_end) && (rdp->left < totlen))
							gccp->found_annot[i] = TRUE;
						else if ((rdp->right >= left_end) && (rdp->right < totlen))
							gccp->found_annot[i] = TRUE;
						left_end = totlen;
					}	
				}

				GatherAddToStack(gcp);
				if (! (*(gccp->userfunc))(gcp))
					return FALSE;
				if (LocateItem) return FALSE;
				if (LocateData != NULL) return FALSE;

				if ((sfp->cit != NULL) && (takecit))
				{
					if (! GatherPubSet(gccp, sfp->cit, 1, thistype, (Pointer)sfp,
						                                (Pointer PNTR)&(sfp->cit), in_scope))
						return FALSE;

					gcp->prevtype = thistype;
					gcp->parentitem = tparent;
					gcp->parenttype = ttype;
				}
			}
		}
		else /* out of scope */
		{

			if (sfp->cit != NULL)  /* just run the counter */
			{
				if (! GatherPubSet(gccp, sfp->cit, 1, thistype, (Pointer)sfp,
					                                (Pointer PNTR)&(sfp->cit), in_scope))
					return FALSE;

				gcp->prevtype = thistype;
				gcp->parentitem = tparent;
				gcp->parenttype = ttype;
			}
		}

		prevsfp = sfp;
		prevlink = (Pointer PNTR)&(sfp->next);
		sfp = sfp->next;
	}
	return TRUE;
}

static Uint1 align_strand_get(Uint1Ptr strands, Int2 order)
{
        if(strands == NULL)
                return 0;
        else
                return strands[order];
}

static Boolean check_reverse_strand(Uint1 loc_strand, Uint1 a_strand)
{
	if(loc_strand == Seq_strand_minus || a_strand == Seq_strand_minus)
		return (loc_strand != a_strand);
	return FALSE;

}

/****************************************************************************
***
*       get_align_ends(): map the two ends of the alignment
*
*****************************************************************************
***/

static void load_start_stop(Int4Ptr start, Int4Ptr stop, Int4 c_start, Int4 c_stop)
{
	if(*start == -1)
	{
		*start = c_start;
		*stop = c_stop;
	}
	else
	{
		*start = MIN(*start, c_start);
		*stop = MAX(*stop, c_stop);
	}
}

static Int2 get_master_order(SeqIdPtr ids, SeqIdPtr sip)
{
	Int2 i;
	
	for(i =0; ids!=NULL; ids = ids->next, ++i)
	{
		if(SeqIdForSameBioseq(ids, sip))
			return i;
	}
	return -1;
}

NLM_EXTERN Boolean get_align_ends(SeqAlignPtr align, SeqIdPtr id, 
                                  Int4Ptr start, Int4Ptr stop, Uint1Ptr strand)
{
    Int2 i, n;
    Boolean is_found;
    Int4 c_start, c_stop;
    DenseSegPtr dsp;
    DenseDiagPtr ddp;
    StdSegPtr ssp;
    SeqLocPtr loc;
    SeqAlignPtr sap;

    *start = -1;
    *stop = -1;
    switch(align->segtype) {
    case 5: /* Discontinuous alignment */
        
        sap = (SeqAlignPtr)(align->segs);
        
        for(; sap != NULL; sap = sap->next) {
            if(!get_align_ends(sap, id, &c_start, &c_stop, strand))
                return FALSE;
            load_start_stop(start, stop, c_start, c_stop);
        }
        return (*start != -1);
        
    case 2:         /*DenseSeg*/
        dsp = (DenseSegPtr)(align->segs);
        if(id == NULL)
            i =0;
        else {
            i = get_master_order(dsp->ids, id);
            if( i == -1)
                return FALSE;
        }
        
        for(n =0; n<dsp->numseg; ++n) {
            c_start = dsp->starts[n*(dsp->dim) +i];
            if(c_start != -1) { /*check for a non-gapped region*/
                c_stop = c_start + dsp->lens[n] -1;
                load_start_stop(start, stop, c_start, c_stop);
            }
        }
        *strand = align_strand_get(dsp->strands, i);
        return (*start != -1);
        
    case 3:
        ssp = (StdSegPtr)(align->segs);
        while(ssp) {
            is_found = FALSE;
            for (loc = ssp->loc; loc!=NULL && !is_found; loc=loc->next) {
                if(loc->choice != SEQLOC_EMPTY) {
                    if(id == NULL)
                        is_found = TRUE;
                    else
                        
                        is_found=SeqIdForSameBioseq(SeqLocId(loc), id);
                    if(is_found) {
                        load_start_stop(start, stop, SeqLocStart(loc), SeqLocStop(loc));
                        *strand = SeqLocStrand(loc);
                    }
                }
            }
            ssp = ssp->next;
        }
        return (*start != -1);
        
    case 1:
        ddp = (DenseDiagPtr)(align->segs);
        while(ddp) {
            if(id == NULL)
                i =0;
            else
                i = get_master_order(ddp->id, id);
            if(i != -1) {
                c_start = ddp->starts[i];
                c_stop = ddp->starts[i] + ddp->len -1;
                *strand = align_strand_get(ddp->strands, i);
                load_start_stop(start, stop, c_start, c_stop);
            }
            ddp = ddp->next;
        }			
        return (*start != -1);
        
    default:
        return FALSE;
    }
}
	
				
static void LinkAlignData(AlignDataPtr PNTR head, AlignDataPtr adp)
{
	AlignDataPtr curr;
	
	if(*head == NULL)
		*head = adp;
	else
	{
		curr = *head;
		while(curr->next != NULL)
			curr = curr->next;
		curr->next = adp;
	}
}

static void LinkAlignRangeToAlignData(AlignDataPtr adp, AlignRangePtr arp)
{
	
	if(arp == NULL)
		return;
	if(adp->arp  == NULL)
		adp->arp = arp;
	else
		adp->last->next = arp;
	adp->last = arp;
}

static AlignDataPtr get_adp_node(AlignDataPtr head, Int2 order, Uint2 chain)
{
	if(head == NULL)
		return NULL;
	while(head)
	{
		if(head->order == order && head->chain == chain)
			return head;
		head = head->next;
	}
	return NULL;
}



static void load_align_data(AlignDataPtr PNTR head, Int4 start, Int4 stop, Uint1 strand, SeqIdPtr sip, Boolean rev, Int2 seg_type, GatherRange t_range, Boolean ck_interval, Int2 order, Uint2 chain)
{
	AlignDataPtr adp;
	AlignRangePtr arp;
	Int4 left, right;
	
	adp = get_adp_node(*head, order, chain);
	if(rev)
	{
		if(strand == Seq_strand_minus)
			strand = Seq_strand_plus;
		else
			strand = Seq_strand_minus;
	}
	if(adp == NULL)
	{
		adp = MemNew(sizeof(AlignData));
		adp->chain= chain;
		adp->order = order;
		adp->sip = sip;
		MemCopy((&adp->extremes), &t_range, sizeof(GatherRange));
		adp->extremes.strand = strand;
		if(seg_type != GAP_SEG)
		{
			adp->seqends.start = start;
			adp->seqends.stop = stop;
		}
		else
		{
			adp->seqends.start = -1;
			adp->seqends.stop = -1;
		}
		adp->seqends.strand = strand;
		adp->arp = NULL;
		adp->next = NULL;
		LinkAlignData(head, adp);
	}
	else
	{
	    left = adp->extremes.left;
	    right = adp->extremes.right;
	    if(t_range.left < left)
	    {
	    	adp->extremes.left = t_range.left;
		adp->extremes.l_trunc = t_range.l_trunc;
	    }
	    if(t_range.right > right)
	    {
	    	adp->extremes.right = t_range.right;
		adp->extremes.r_trunc = t_range.r_trunc;
	    }
	    if(seg_type != GAP_SEG)
	    {
		if(adp->seqends.start == -1)
		{
			adp->seqends.start = start;
			adp->seqends.stop = stop;
		}
		else
		{
			adp->seqends.start = MIN(start, adp->seqends.start);
			adp->seqends.stop = MAX(stop, adp->seqends.stop);
		}
	    }
	}	
		
		
	if(ck_interval)
	{
		arp = MemNew(sizeof(AlignRange));
		arp->segtype = (Uint1)seg_type;
		MemCopy(&(arp->gr), &t_range, sizeof(GatherRange));
		arp->sr.start = start;
		arp->sr.stop = stop;
		arp->sr.strand = strand;
		LinkAlignRangeToAlignData(adp, arp);
	}
}


static Boolean get_end_diag_val(DenseSegPtr dsp, Int2 m_order, Int2 k_seg, Int4Ptr start, Int4Ptr stop, Boolean first)
{
	Int2 i, k;
	Int2 dim;
	Boolean increase = TRUE;

	dim = dsp->dim;
	if(first)
		k = k_seg;
	else
		k = dsp->numseg-1 -k_seg;

	increase = (k ==0);
		
	for(i =0; i<dsp->numseg; ++i)
	{
		if(dsp->starts[dim*k+m_order] != -1)
		{
			*start = dsp->starts[dim*k+m_order];
			*stop = *start + dsp->lens[k] -1;
			return TRUE;
		}
		if(increase)
			++k;
		else
			--k;
	}
	return FALSE;
}

static void load_trunc_info(SeqLocPtr slp, GatherRangePtr grp)
{
	SeqIntPtr sint;
	Boolean f_trunc = FALSE, t_trunc = FALSE;
	IntFuzzPtr ifp;

	if(slp == NULL || grp == NULL)
		return;
	if(slp->choice != SEQLOC_INT)
		return;
	sint = slp->data.ptrvalue;
	if(sint != NULL)
	{
		ifp = sint->if_from;
		if(ifp)
		{
			if(ifp->choice == 4 && ifp->a == 2)
				f_trunc = TRUE;
		}
		ifp = sint->if_to;
		if(ifp)
		{
			if(ifp->choice == 4 && ifp->a == 1)
				t_trunc = TRUE;
		}
	}
	
	grp->l_trunc = f_trunc;
	grp->r_trunc = t_trunc;
	if(grp->strand != sint->strand)
	{
		if(grp->strand == Seq_strand_minus || sint->strand == Seq_strand_minus)
		{
			grp->l_trunc = t_trunc;
			grp->r_trunc = f_trunc;
		}
	}
}

/**************************************************************
*
*
**************************************************************/

static Boolean is_end_gaps_for_master(DenseSegPtr dsp, Int2 m_order, Int2 k_seg, Int2 j_seg, Boolean left)
{
	Int2 i, k;
	Boolean is_gap = FALSE;
	Boolean increase;

	/*get the first non-gap segment*/
	if(left)
		k = k_seg;
	else
		k = dsp->numseg-1 - k_seg;
	increase = (k ==0);
	for(i = 0; i<dsp->numseg; ++i)
	{
		if(dsp->starts[(dsp->dim)*k+m_order] != -1)
			return FALSE;
		else
		{
			if(j_seg == k)
				return TRUE;
		}
		if(increase)
			++k;
		else
			--k;
	}

	return FALSE;
}

static Uint1 get_master_strand_for_continous_ssp (StdSegPtr ssp, SeqIdPtr m_sip)
{
	SeqLocPtr loc;

	while(ssp)
	{
		for(loc= ssp->loc; loc!=NULL; loc = loc->next)
		{
			if(SeqIdMatch(SeqLocId(loc), m_sip))
			{
				if(loc->choice != SEQLOC_EMPTY)
					return SeqLocStrand(loc);
			}
		}

		ssp = ssp->next;
	}

	return 0;
}

static Int2 get_ssp_numseg (StdSegPtr ssp)
{
	Int2 i = 0;

	while(ssp)
	{
		++i;
		ssp = ssp->next;
	}

	return i;
}

static StdSegPtr get_nth_ssp (StdSegPtr ssp, Int2 order)
{
	Int2 i;

	i = 0;
	while(ssp)
	{
		if(i == order)
			return ssp;
		++i;
		ssp = ssp->next;
	}

	return NULL;
}

static Boolean is_fuzz_loc (SeqLocPtr slp)
{
	SeqLocPtr t_slp;
	SeqIntPtr sint;
	SeqPntPtr spp;

	t_slp = NULL;
	while((t_slp = SeqLocFindNext(slp, t_slp)) != NULL)
	{
		if(t_slp->choice == SEQLOC_INT)
		{
			sint = t_slp->data.ptrvalue;
			if(sint->if_from || sint->if_to)
				return TRUE;
		}
		else if(t_slp->choice == SEQLOC_PNT)
		{
			spp = t_slp->data.ptrvalue;
			if(spp->fuzz)
				return TRUE;
		}
	}

	return FALSE;
}
			
static Int4 calculate_mag_val (StdSegPtr ssp, SeqIdPtr m_sip, BoolPtr inverse)
{
	SeqLocPtr m_slp, s_slp;
	SeqLocPtr slp;
	Int4 mag_val;

	while(ssp)
	{
		m_slp = NULL;
		s_slp = NULL;
		for(slp = ssp->loc; slp != NULL; slp = slp->next)
		{
			if(SeqIdMatch(SeqLocId(slp), m_sip))
				m_slp = slp;
			else
				s_slp = slp;
		}

		if(m_slp && s_slp && SeqLocLen(m_slp) && SeqLocLen(s_slp))
		{
			if(!is_fuzz_loc(m_slp) && !is_fuzz_loc(s_slp))
			{
				if(SeqLocLen(m_slp) > SeqLocLen(s_slp))
				{
					mag_val = SeqLocLen(m_slp)/SeqLocLen(s_slp);
					if(SeqLocLen(m_slp)%SeqLocLen(s_slp) == 0)
					{
						*inverse = FALSE;
						return mag_val;
					}
				}
				else
				{
					mag_val = SeqLocLen(s_slp)/SeqLocLen(m_slp);
					if(SeqLocLen(s_slp)%SeqLocLen(m_slp) == 0)
					{
						*inverse = TRUE;
						return mag_val;
					}
				}
			}
		}

		ssp = ssp->next;
	}


	return -1;
}

static Int4 modify_offset (SeqLocPtr m_loc, SeqLocPtr s_loc, Int4 m_offset, Int4Ptr left_over)
{
	Int4 m_len, s_len;

	*left_over = 0;
	if(m_offset == 0)
		return 0;
	else
	{
		m_len = SeqLocLen(m_loc);
		s_len = SeqLocLen(s_loc);
		if(m_len == s_len)
			return m_offset;
		else
		{
			if(m_len/s_len == 3)
			{
				*left_over = m_offset%3;
				if(*left_over > 0)
					*left_over = 3 - (*left_over);
			}
			if(*left_over > 0)
				return ((m_offset * s_len) /m_len + 1);
			else
				return ((m_offset * s_len) /m_len);
		}
	}
}

static Uint1 get_DenseSeg_strand(DenseSegPtr dsp, Int2 order)
{
	Int2 i, dim;

	if(dsp == NULL || dsp->strands == NULL)
		return 0;
	dim = dsp->dim;
	for(i = 0; i<dsp->numseg; ++i)
	{
		if(dsp->starts[dim * i + order] != -1)
			return (dsp->strands[dim*i + order]);
	}

	return 0;
}

NLM_EXTERN AlignDataPtr gather_align_data(SeqLocPtr m_slp, SeqAlignPtr align, 
                                          Int4 offset, Boolean ck_interval, 
                                          Boolean map)
{
    SeqIdPtr m_sip, sip;
    Uint1 m_strand, strand, c_strand;
    DenseSegPtr dsp;
    StdSegPtr ssp;
    DenseDiagPtr ddp;
    Int2 i, m_order, k, numseg, t_numseg;
    Int4 s1;
    Uint1 seg_type;
    Boolean is_found = FALSE;
    Int4 start, stop, c_start, c_stop;
    Int4 m_start, m_stop;
    Int4 master_pos;	/*position of the master sequence*/
    Int4 off_start, off_stop;
    Boolean rev;
    GatherRange gr, t_range;
    SeqLocPtr a_slp, loc;
    AlignDataPtr head = NULL, curr;
    Uint2 chain;
    Int4 h_start, h_stop;
    Boolean inc_left_mgap;	/*include the left gaps on the master sequence*/
    Boolean inc_right_mgap;	/*include the right gaps on the master sequence*/
    Boolean left_end_gap, right_end_gap;
    Int2 k_seg;
    Boolean collect;
    Boolean load;
    Boolean cont;	/*is it a continous segment ? */
    Int4 len;
    Int4 mag_val;	/*the magnification value */
    SeqLocPtr master_loc, m_loc;
    Boolean inverse;
    Int4 leftover;
    Int4 left, right;
    Boolean use_stop;
    Uint1 ma_strand;
    SeqAlignPtr sap;

    strand = 0;
    m_sip = SeqLocId(m_slp);
    if(!get_align_ends(align, m_sip, &start, &stop, &c_strand))
        return NULL;
    
    a_slp = SeqLocIntNew(start, stop, c_strand, m_sip);
    if(!SeqLocOffset(m_slp, a_slp, &gr, offset)) {
        SeqLocFree(a_slp);
        return NULL;
    }
    cont = FALSE;
    if(align->segtype == 2)
        cont = TRUE;
    else if(align->segtype == 3) {
        cont = (align->type == 3);
        /* cont = TRUE; */
        ssp = align->segs;
        mag_val = calculate_mag_val (ssp, m_sip, &inverse);
        if(mag_val == -1) {
            mag_val = 1;
            inverse = FALSE;
        }
    }
    
    m_strand = SeqLocStrand(m_slp);
    m_start = SeqLocStart(m_slp);
    m_stop = SeqLocStop(m_slp);
    c_start = -1;
    c_stop =-1;

    switch(align->segtype) {

    case 5: /* Discontinuous alignment */
        for(sap = (SeqAlignPtr) align->segs; sap != NULL; sap = sap->next) {
            curr =  gather_align_data(m_slp, sap, offset, ck_interval, map);
            LinkAlignData(&head, curr);
        }
        break;

    case 2: /*for DenseSegs*/
        dsp = (DenseSegPtr)(align->segs);
        m_order = get_master_order(dsp->ids, m_sip);
        ma_strand = get_DenseSeg_strand(dsp, m_order);
        rev = check_reverse_strand(m_strand, c_strand);
        if(rev)
            k = dsp->numseg -1;
        else
            k =0;
        k_seg = k;
        
        get_end_diag_val(dsp, m_order, k, &h_start, &h_stop, TRUE);
        if(!(h_stop < m_start || h_start > m_stop))	/*it is within the range*/
            inc_left_mgap = TRUE;
        else
            inc_left_mgap = FALSE;
        
        get_end_diag_val(dsp, m_order, k, &h_start, &h_stop, FALSE);
        if(!(h_stop < m_start || h_start > m_stop))	/*it is within the range*/
            inc_right_mgap = TRUE;
        else
            inc_right_mgap = FALSE;
        
        for(numseg = 0; numseg<dsp->numseg; ++numseg) {
            if(dsp->lens[k] >0) {
                s1 = (Int4)k*(Int4)(dsp->dim)+(Int4)m_order;
                master_pos = dsp->starts[s1];
                collect = FALSE;
                if(master_pos == -1) {	/*gap in the master sequence. It is an insertion*/
                    strand = ma_strand;
                    left_end_gap = is_end_gaps_for_master(dsp, m_order, k_seg, k, TRUE);
                    right_end_gap = is_end_gaps_for_master(dsp, m_order, k_seg, k, FALSE);
                    if(left_end_gap && inc_left_mgap && !map) {
                        collect = TRUE;
                        t_range.left = offset;
                        t_range.right = offset + dsp->lens[k] -1;
                        offset += dsp->lens[k];
                    }
                    if(right_end_gap && inc_right_mgap && !map) {
                        collect = TRUE;
                        t_range.left = t_range.right +1;
                        t_range.right = t_range.left + dsp->lens[k] -1;
                    }
                    if(!left_end_gap && !right_end_gap) {
                        use_stop = TRUE;
                        if((strand != Seq_strand_minus && rev) || (strand == Seq_strand_minus && rev == FALSE))
                            use_stop = FALSE;
                        if(use_stop)
                            update_seq_loc(c_stop, c_stop, 0, a_slp);
                        else
                            update_seq_loc(c_start, c_start, 0, a_slp);
                        collect = SeqLocOffset(m_slp, a_slp, &t_range, offset);
                        if(collect) {
                            ++(t_range.left);
                            if(!map) {
                                t_range.right = t_range.left+dsp->lens[k]-1;
                                offset += dsp->lens[k];
                            }
                        }
                    }
                    off_start = 0;
                    off_stop = 0;
                } else {/*master is not a gap*/
                    c_start = dsp->starts[s1];
                    c_stop = c_start + dsp->lens[k] -1;
                    update_seq_loc(c_start, c_stop, 0, a_slp);
                    collect = SeqLocOffset(m_slp, a_slp, &t_range, offset);
                    if(collect) {
                            off_start = MAX(0, (m_start - c_start));
                            off_stop = MAX(0, (c_stop - m_stop));
                    }
                }
                if(collect) {
                    for(sip = dsp->ids, i=0; sip!=NULL; sip = sip->next, ++i) {
                        load = TRUE;
                        start = dsp->starts[k*(dsp->dim) +i];
                        if(map) {
                            /*multiple pairwise, ignore the master and the gap in the master*/
                            if(i == m_order)
                                load = FALSE;
                            else if(master_pos == -1 && start == -1)
                                load= FALSE;
                        }
                        if(load)
                            {
                                strand = align_strand_get(dsp->strands, i);
                                if(start != -1) {
                                    stop = start + dsp->lens[k] -1;
                                    if(master_pos == -1 && map) { /*master sequence*/
                                        
                                        seg_type = INS_SEG;
                                        t_range.right = dsp->lens[k];
                                    } else {
                                        if(check_reverse_strand(c_strand, strand)) {
                                            start += off_stop;
                                            stop -= off_start;
                                        } else {
                                            start += off_start;
                                            stop -= off_stop;
                                        }
                                        seg_type = REG_SEG;
                                    }
                                } else {
                                    seg_type = GAP_SEG;
                                    start = -1;
                                    stop = dsp->lens[k];
                                } 
                                load_align_data(&head, start, stop, strand, sip, rev, seg_type, t_range, ck_interval, i, 1);
                            }
                    }
                }
            }
            if(rev)
                --k;
            else
                ++k;
        }
        break;
        
    case 3:
        ssp = (StdSegPtr)(align->segs);
        if(cont) {
            c_strand = get_master_strand_for_continous_ssp (ssp, m_sip);
            rev = check_reverse_strand(m_strand, c_strand);
            numseg = get_ssp_numseg (ssp);
            if(rev)
                k = numseg - 1;
            else
                k = 0;
            for(t_numseg = 0; t_numseg <numseg; ++t_numseg) {
                ssp = get_nth_ssp ((StdSegPtr)(align->segs), k);
                is_found = FALSE;
                for(loc= ssp->loc, i=0; loc!=NULL && !is_found; ++i, loc = loc->next) {
                    if(SeqIdMatch(SeqLocId(loc), m_sip)) {
                        master_loc = loc;
                        m_order = i;
                        if(loc->choice == SEQLOC_EMPTY)
                            master_pos = -1;
                        else {
                            master_pos = SeqLocStart(loc);
                            is_found = SeqLocOffset(m_slp, loc, &t_range, offset);
                        }
                        break;
                    }
                }
                len = -1;
                for(loc= ssp->loc, i = 0; loc!=NULL; ++i, loc = loc->next) {
                    if(i != m_order) {
                        if(loc->choice != SEQLOC_EMPTY)
                            len = SeqLocLen(loc);
                        break;
                    }
                }
				/*both segments are gaps */
                if(is_found) {
                    if(len == -1 && master_pos == -1)
                        is_found = FALSE;
                }
                if(loc != NULL && (is_found || master_pos == -1)) {
                    collect = FALSE;
                    if(master_pos == -1) {/*the master is a gap, it is an insertion*/
                        if(m_strand == Seq_strand_minus)
                            update_seq_loc(c_start, c_start, 0, a_slp);
                        else
                            update_seq_loc(c_stop, c_stop, 0, a_slp);
                        collect = SeqLocOffset(m_slp, a_slp, &t_range, offset);
                        if(collect) {
                            ++(t_range.left);
                            if(!map) {
                                if(!inverse) {
                                    t_range.right = t_range.left+len * mag_val -1;
                                    offset += len * mag_val;
                                } else {
                                    t_range.right = t_range.left+len / mag_val -1;
                                    offset += len / mag_val;
                                }
                            }
                        }
                        off_start = 0;
                        off_stop = 0;
                    } else {
                        c_start = SeqLocStart(master_loc);
                        c_stop = SeqLocStop(master_loc);
                        collect = TRUE;
                        off_start = MAX(0, (m_start - c_start));
                        off_stop = MAX(0, (c_stop - m_stop));
                    }
                    
                    if(collect) {
                        for(loc = ssp->loc, i= 0; loc != NULL; loc = loc->next, ++i) {
                            load = TRUE;
                            if(map && i == m_order)
                                load = FALSE;
                            if(load) {
                                left = t_range.left;
                                right = t_range.right;
                                if(loc->choice == SEQLOC_EMPTY) {/*it is a gap */
                                    strand = 0;
                                    start = -1;
                                    if(i == m_order)
                                        stop = inverse ? (len /mag_val) : (len * mag_val);
                                    else
                                        stop = inverse ? SeqLocLen(master_loc) * mag_val : SeqLocLen(master_loc)/mag_val;
                                    seg_type = GAP_SEG;
                                } else {
                                    start = SeqLocStart(loc);
                                    stop = SeqLocStop(loc);
                                    strand = SeqLocStrand(loc);
                                    if(master_pos == -1 && map) {
                                        seg_type = INS_SEG;
                                        t_range.right = SeqLocLen(loc);
                                    } else {
                                        if(check_reverse_strand(strand, c_strand)) {
                                            start += modify_offset(master_loc, loc, off_stop, &leftover);
                                            t_range.left += leftover;
                                            stop -= modify_offset(master_loc, loc, off_start, &leftover);
                                            t_range.right += leftover;
                                        } else {
                                            start += modify_offset(master_loc, loc, off_start, &leftover);
                                            t_range.left += leftover;
                                            stop -= modify_offset(master_loc, loc, off_stop, &leftover);
                                            t_range.right += leftover;
                                        }
                                        /* if(check_reverse_strand(c_strand, strand))
                                           {
                                           start += off_stop;
                                           stop -= off_start;
                                           }
                                           else
                                           {
                                           start += off_start;
                                           stop -= off_stop;
                                           } */
                                        seg_type = STD_SEG;
                                    }
                                }
                                load_align_data(&head, start, stop, strand, SeqLocId(loc), rev, seg_type, t_range, ck_interval, i, 1);
                                t_range.left = left;
                                t_range.right = right;
                                
                            }
                        }
                    }
                }
                if(rev)
                    --k;
                else
                    ++k;
            }
        } else {	/*end of if (count) */
            chain = 0;
            while(ssp) {
                ++chain;
                is_found = FALSE;
                curr = NULL;
                for(loc= ssp->loc, i=0; loc!=NULL && !is_found;) {
                    if(SeqLocOffset(m_slp, loc, &t_range, offset)) {
                        m_loc = loc;
                        is_found = TRUE;
                        break;
                    } else {
                        ++i;
                        loc = loc->next;
                    }
                }
                if(is_found) {
                    m_order = i;
                    off_start = MAX(0, (m_start - SeqLocStart(loc)));
                    off_stop = MAX(0, (SeqLocStop(loc) - m_stop));
                    c_strand = SeqLocStrand(loc);
                    if(map)
                        rev = check_reverse_strand(c_strand, m_strand);
                    else
                        rev = FALSE;
                    for(loc = ssp->loc,i=0; loc != NULL; loc = loc->next,++i) {
                        sip = SeqLocId(loc);
                        if(!map|| (m_order !=i)) {
                            start = SeqLocStart(loc);
                            stop = SeqLocStop(loc);
                            strand = SeqLocStrand(loc);
                            left = t_range.left;
                            right = t_range.right;
                            if(check_reverse_strand(strand, c_strand)) {
                                start += modify_offset(m_loc, loc, off_stop, &leftover);
                                t_range.left += leftover;
                                stop -= modify_offset(m_loc, loc, off_start, &leftover);
                                t_range.right += leftover;
                            } else {
                                start += modify_offset(m_loc, loc, off_start, &leftover);
                                t_range.left += leftover;
                                stop -= modify_offset(m_loc, loc, off_stop, &leftover);
                                t_range.right += leftover;
                            }
                            load_trunc_info(loc, &t_range);
                            load_align_data(&curr, start, stop, strand, sip, rev, STD_SEG, t_range, ck_interval, i, chain);
                            t_range.left = left;
                            t_range.right = right;
                        }
                    }
                }
                LinkAlignData(&head, curr);
                ssp = ssp->next;
            }
        }
        break;
        
    case 1:
        ddp = (DenseDiagPtr)(align->segs);
        chain = 0;
        while(ddp) {
            ++chain;
            curr = NULL;
            m_order = get_master_order(ddp->id, m_sip);
            if(m_order != -1) {
                c_strand = align_strand_get(ddp->strands, m_order);
                c_start = ddp->starts[m_order];
                c_stop = c_start + ddp->len -1;
                update_seq_loc(c_start, c_stop, 0, a_slp);
                if(SeqLocOffset(m_slp, a_slp, &t_range, offset)) {
                    rev = check_reverse_strand(m_strand, c_strand);
                    off_start = MAX(0, (m_start-c_start));
                    off_stop = MAX(0, (c_stop - m_stop));
                    for(sip = ddp->id, i=0; sip!=NULL; sip = sip->next, ++i) {
                        if(!map|| i != m_order) {
                            start = ddp->starts[i];
                            stop = start + ddp->len -1;
                            strand = align_strand_get(ddp->strands, i);
                            if(check_reverse_strand(strand, c_strand)) {
                                start += off_stop;
                                stop -= off_start;
                            } else {
                                start += off_start;
                                stop -= off_stop;
                            }
                            load_align_data(&curr, start, stop, strand, sip, rev, DIAG_SEG, t_range, ck_interval, i, chain);
                        }
                    }
                }
            }
            LinkAlignData(&head, curr);
            ddp = ddp->next;
        }
        break;
    default:
        break;
    }
    
    SeqLocFree(a_slp);
    return head;
}

					

static Boolean NEAR GatherSeqAlign(InternalGCCPtr gccp, SeqAlignPtr sap,
        Uint1 ttype, Pointer tparent, Pointer PNTR prevlink, Boolean in_scope, Boolean check, Uint1 obj_type)
{
	GatherContextPtr gcp;
	GatherScopePtr gsp;
	Int2 LocateItem = 0;
	Pointer LocateData = NULL;
	Uint1 thistype;
        SeqLocPtr slp, target[2];
        Boolean takeit=TRUE, checkseq = FALSE;
	Int2 j, numcheck=0;
        Int4 offset = 0;
	Boolean check_interval;
	AlignDataPtr adp;
	SeqAlignPtr prevsap = NULL;

	if (sap == NULL) return TRUE;

	if(obj_type != OBJ_SEQALIGN && obj_type != OBJ_SEQHIST_ALIGN)
		return TRUE;

	gcp = &(gccp->gc);
	gsp = &(gccp->scope);

	if (gsp->ignore[obj_type])
		return TRUE; 

	if (gccp->locatetype == obj_type)
 	{
		LocateItem = gccp->locateID;
		LocateData = gccp->locatePtr;
	}

	thistype = obj_type;


        if (gsp->target != NULL && check)
        {
                numcheck = 1;
                target[0] = gsp->target;
                if (gccp->segloc != NULL)
                {
                        numcheck = 2;
                        target[1] = gccp->segloc;
                }
                offset = gsp->offset;
		checkseq = TRUE;
	}

	check_interval = (gsp->nointervals == FALSE);
	while (sap != NULL)
	{
		gccp->itemIDs[obj_type]++;
		if (LocateItem == gccp->itemIDs[obj_type])
			in_scope = TRUE;
		if (LocateData == (Pointer)sap)
			in_scope = TRUE;

		if (in_scope)
		{
			gcp->previtem = (Pointer)prevsap;
			gcp->prevtype = thistype;
			gcp->parentitem = tparent;
			gcp->parenttype = ttype;
			gcp->itemID = gccp->itemIDs[obj_type];
			gcp->prevlink = prevlink;
			gcp->thisitem = (Pointer)sap;
			gcp->thistype = thistype;
			GatherAddToStack(gcp);
			if (checkseq)
			{
				takeit = FALSE;
				for (j =0; ((j<numcheck) && (!takeit)); j++)
				{
					slp = target[j];
					FreeAlignData(gcp->adp);
					gcp->adp = NULL;
					adp = gather_align_data(slp, sap, offset, check_interval, gsp->mapinsert);
					if(adp == NULL)
						takeit = FALSE;
					else
					{
						takeit = TRUE;
						gcp->adp = adp;
					}
					
				}
				if(takeit)
				{
					if (! (*(gccp->userfunc))(gcp))
						return FALSE;
				}
			}
			else
			{
				if(takeit && check)
					if (! (*(gccp->userfunc))(gcp))
						return FALSE;
			}
			if (LocateItem) return FALSE;
			if (LocateData) return FALSE;
		}

		if (sap->segtype == SAS_DISC)
		{
			gcp->indent++;
			if (! GatherSeqAlign(gccp, (SeqAlignPtr)(sap->segs),
			       thistype, (Pointer)sap, &(sap->segs),
			       in_scope, check, OBJ_SEQALIGN))
			{
			   return FALSE;
			}
			gcp->indent--;
		}
		             

		prevsap = sap;
		prevlink = (Pointer PNTR)&(sap->next);
		sap = sap->next;
	}
	return TRUE;
}

static Boolean NEAR GatherSeqGraph(InternalGCCPtr gccp, SeqGraphPtr sgp,
        Uint1 ttype, Pointer tparent, Pointer PNTR prevlink, Boolean in_scope)
{
	GatherContextPtr gcp;
	GatherScopePtr gsp;
	Int2 LocateItem = 0;
	Pointer LocateData = NULL;
	Uint1 thistype;

	if (sgp == NULL) return TRUE;

	gcp = &(gccp->gc);
	gsp = &(gccp->scope);

	if (gsp->ignore[OBJ_SEQGRAPH])
		return TRUE;

	LocateItem = gccp->locateID;
	LocateData = gccp->locatePtr;

	thistype = OBJ_SEQGRAPH;
	gcp->previtem = NULL;
	gcp->prevtype = thistype;
	gcp->parentitem = tparent;
	gcp->parenttype = ttype;

	while (sgp != NULL)
	{
		gccp->itemIDs[OBJ_SEQGRAPH]++;
		if (LocateItem == gccp->itemIDs[OBJ_SEQGRAPH])
			in_scope = TRUE;
		if (LocateData == (Pointer)sgp)
			in_scope = TRUE;

		if (in_scope)
		{
			gcp->itemID = gccp->itemIDs[OBJ_SEQGRAPH];
			gcp->thisitem = (Pointer)sgp;
			gcp->thistype = thistype;
			GatherAddToStack(gcp);
			gcp->prevlink = prevlink;
			if (! (*(gccp->userfunc))(gcp))
				return FALSE;
			if (LocateItem) return FALSE;
			if (LocateData != NULL) return FALSE;
		}

		gcp->previtem = (Pointer)sgp;
		prevlink = (Pointer PNTR)&(sgp->next);
		sgp = sgp->next;
	}
	return TRUE;
}

static Boolean NEAR GatherSeqAnnot(InternalGCCPtr gccp, SeqAnnotPtr sap,
        Uint1 ttype, Pointer tparent, Pointer PNTR prevlink, Boolean in_scope)
{
	GatherContextPtr gcp;
	GatherScopePtr gsp;
	SeqAnnotPtr prevsap = NULL;
	Int2 LocateItem = 0;
	Pointer LocateData = NULL;
	Uint1 thistype;

	if (sap == NULL) return TRUE;

	gcp = &(gccp->gc);
	gsp = &(gccp->scope);

	if (gsp->ignore[OBJ_SEQANNOT])
		return TRUE;

	if ((! gsp->currlevel) && (gsp->ignore_top))
		return TRUE;

	if (gccp->locatetype == OBJ_SEQANNOT)
 	{
		LocateItem = gccp->locateID;
		LocateData = gccp->locatePtr;
	}
	else
		LocateItem = 0;

	while (sap != NULL)
	{
		thistype = OBJ_SEQANNOT;
		gccp->itemIDs[OBJ_SEQANNOT]++;
		if (LocateItem == gccp->itemIDs[OBJ_SEQANNOT])
			in_scope = TRUE;
 		if (LocateData == (Pointer)sap)
			in_scope = TRUE;

		gcp->thisitem = (Pointer)sap;
		gcp->thistype = thistype;
		gcp->itemID = gccp->itemIDs[OBJ_SEQANNOT];
 		GatherAddToStack(gcp);

		if (in_scope)
		{
			gcp->previtem = (Pointer) prevsap;
			gcp->prevtype = thistype;
			gcp->parentitem = tparent;
			gcp->parenttype = ttype;
			gcp->prevlink = prevlink;
			if (! (*(gccp->userfunc))(gcp))
				return FALSE;
			if (LocateItem) return FALSE;
			if (LocateData != NULL) return FALSE;
		}

		gcp->indent++;
		switch (sap->type)
		{
		
			case 1:     /* feature table */
				if (gccp->useSeqMgrIndexes) {
					/* now sending targeted features all at once regardless of packaging - so do nothing here */
				} else {
					if (! GatherSeqFeat(gccp, (SeqFeatPtr)(sap->data), thistype, (Pointer)sap, &(sap->data), in_scope, OBJ_SEQFEAT))
						return FALSE;
				}
				break;
			case 2:     /* alignments */
				if (! GatherSeqAlign(gccp, (SeqAlignPtr)(sap->data), thistype, (Pointer)sap, &(sap->data), in_scope, TRUE, OBJ_SEQALIGN))
					return FALSE;
				break;
			case 3:     /* graphs */
				if (! GatherSeqGraph(gccp, (SeqGraphPtr)(sap->data), thistype, (Pointer)sap, &(sap->data), in_scope))
					return FALSE;
				break;

		}
		gcp->indent--;

		prevsap = sap;
		prevlink = (Pointer PNTR)&(sap->next);
		sap = sap->next;
	}
	return TRUE;
}



static Boolean NEAR GatherSeqHist(InternalGCCPtr gccp, SeqHistPtr hist,
	Uint1 ttype, Pointer tparent, Pointer PNTR prevlink, Boolean in_scope, Boolean check_seq)
{
	GatherContextPtr gcp;
	GatherScopePtr gsp;
	Int2 LocateItem = 0;
	Pointer LocateData = NULL;
	Uint1 thistype;

	if(hist == NULL) return TRUE;

	gcp = &(gccp->gc);
	gsp = &(gccp->scope);

	if(gsp->ignore[OBJ_SEQHIST])
		return TRUE;

	if (gccp->locatetype == OBJ_SEQHIST)
 	{
		LocateItem = gccp->locateID;
		LocateData = gccp->locatePtr;
	}
	else
		LocateItem = 0;

	gccp->itemIDs[OBJ_SEQHIST]++;
	thistype = OBJ_SEQHIST;
	if (LocateItem == gccp->itemIDs[OBJ_SEQHIST])
		in_scope = TRUE;
	if (LocateData == (Pointer)(hist))
		in_scope = TRUE;

	gcp->thistype = thistype;
	gcp->thisitem = (Pointer)(hist);
	gcp->itemID = gccp->itemIDs[OBJ_SEQHIST];
	GatherAddToStack(gcp);

	if (in_scope)
	{
		gcp->previtem = NULL;
		gcp->prevtype = 0;
		gcp->parentitem = (Pointer)tparent;
		gcp->parenttype = ttype;
		gcp->prevlink = prevlink;

		if (! (*(gccp->userfunc))(gcp))
			return FALSE;
		if (LocateItem) return FALSE;
		if (LocateData != NULL) return FALSE;
	}
	if((hist->assembly!=NULL) && (!gsp->ignore[OBJ_SEQHIST_ALIGN]))
		GatherSeqAlign(gccp,(SeqAlignPtr)(hist->assembly), thistype, 
	          (Pointer)(hist),(Pointer PNTR)&(hist->assembly),
		  in_scope, check_seq, OBJ_SEQHIST_ALIGN);
	return TRUE;

}





static Boolean NEAR GatherBioseqFunc (InternalGCCPtr gccp, BioseqPtr bsp,
         Pointer parent, Uint2 parenttype, SeqEntryPtr prev,
	     Pointer PNTR prevlink, SeqEntryPtr curr, Boolean in_scope)
{
	GatherContextPtr gcp;
	GatherScopePtr gsp;
	Boolean takeit, in_range=TRUE, rev, free_seg, trunc_l, trunc_r;
	Int2 LocateItem = 0, segctr, first_seg, last_seg;
	Pointer LocateData = NULL;
	ValNode vn;
	SeqLocPtr head, slp, target=NULL, tslp, segloc;
	SeqLocPtr targets[2];
	Int4 offset, toffset, seglen, tlen;
	GatherRangePtr rdp;
	Uint1 thistype, segtype;
	Int2 j, numcheck;
	Boolean match_target = TRUE, do_seg, is_delta;
	DeltaSeqPtr dsp;
	Pointer dataptr;  /* used for OBJ_BIOSEQ_SEG and OBJ_BIOSEQ_DELTA */

	if (bsp == NULL)
		return TRUE;

	gcp = &(gccp->gc);
	gsp = &(gccp->scope);

	takeit = in_scope;
	head = NULL;      
	if (gsp->ignore[OBJ_BIOSEQ])
		takeit = FALSE;
	else if ((takeit) && (gsp->target != NULL))
	{
		if (bsp != gccp->bsp)
			takeit = FALSE;
	}

	gccp->itemIDs[OBJ_BIOSEQ]++;
	gcp->itemID = gccp->itemIDs[OBJ_BIOSEQ];
	if (gccp->locatetype == OBJ_BIOSEQ)
	{
		LocateItem = gccp->locateID;
		LocateData = gccp->locatePtr;
		if (LocateItem == gccp->itemIDs[OBJ_BIOSEQ])
			takeit = TRUE;
		if (LocateData == (Pointer)bsp)
			takeit = TRUE;
	}

	thistype = OBJ_BIOSEQ;
	gcp->thistype = thistype;
	gcp->thisitem = (Pointer)bsp;
	GatherAddToStack(gcp);

        if (gsp->target != NULL)
        {
		match_target = FALSE;
		vn.choice = SEQLOC_WHOLE;
		vn.data.ptrvalue = SeqIdFindBest (bsp->id, 0);
                numcheck = 1;
                targets[0] = gsp->target;
                if (gccp->segloc != NULL)
                {
                       	numcheck = 2;
                       	targets[1] = gccp->segloc;
                }
		for (j =0; !match_target && j<numcheck; j++)
		{
			if(SeqLocOffset(targets[j], &vn, &(gcp->extremes), gsp->offset))
				match_target = TRUE;
		}
	}

	if (takeit)
	{
		gcp->sep = curr;
		gcp->previtem = prev;
		gcp->prevtype = OBJ_SEQENTRY;
		gcp->parentitem = parent;
		gcp->parenttype = parenttype;
		gcp->thistype = thistype;
		gcp->thisitem = (Pointer)bsp;
		gcp->prevlink = prevlink;

        	/*if (gsp->target != NULL)
        	{
			takeit = FALSE;
			vn.choice = SEQLOC_WHOLE;
			vn.data.ptrvalue = bsp->id;
                	numcheck = 1;
                	targets[0] = gsp->target;
                	if (gccp->segloc != NULL)
                	{
                        	numcheck = 2;
                        	targets[1] = gccp->segloc;
                	}
			for (j =0; !takeit && j<numcheck; j++)
			{
				if(SeqLocOffset(targets[j], &vn, &(gcp->extremes), gsp->offset))
					takeit = TRUE;
			}
		}*/
		takeit = match_target;

		if(!takeit)
			return FALSE;
		if (! (*(gccp->userfunc))(gcp))
			return FALSE;
		if (LocateItem) return FALSE;
		if (LocateData != NULL) return FALSE;
	}


	gcp->indent++;

	if ((bsp->repr == Seq_repr_map) && (! gsp->ignore[OBJ_BIOSEQ_MAPFEAT]))
	{
		if (! GatherSeqFeat(gccp, (SeqFeatPtr)(bsp->seq_ext), thistype, (Pointer)bsp, &(bsp->seq_ext), in_scope, OBJ_BIOSEQ_MAPFEAT))
			return FALSE;
	}

    do_seg = FALSE;   /* assume no pointer processing */

	if (! gsp->ignore[OBJ_BIOSEQ_SEG])
	{
		if ((bsp->repr == Seq_repr_seg) ||
			(bsp->repr == Seq_repr_ref))
		{
			do_seg = TRUE;
			is_delta = FALSE;
			segtype = OBJ_BIOSEQ_SEG;
		}
	}

	if (! gsp->ignore[OBJ_BIOSEQ_DELTA])
	{
		if (bsp->repr == Seq_repr_delta)
		{
			do_seg = TRUE;
			is_delta = TRUE;
			segtype = OBJ_BIOSEQ_DELTA;
		}
	}

	if (do_seg)   /* process segment, ref, delta sequence .ext */
	{
		if (bsp->repr == Seq_repr_seg)
		{
			vn.next = NULL;
			vn.choice = SEQLOC_MIX;
			vn.data.ptrvalue = bsp->seq_ext;
			head = &vn;
		}
		else if (bsp->repr == Seq_repr_ref)
			head = (SeqLocPtr)(bsp->seq_ext);
		else
		{
			dsp = (DeltaSeqPtr)(bsp->seq_ext);  /* real data is here */
			head = DeltaSeqsToSeqLocs(dsp);  /* this for mapping only */
		}

		if (head != NULL)
		{
			segloc = NULL;
			segctr = 0;
			rev = gccp->rev;
			free_seg = FALSE;
			rdp = &(gcp->extremes);

			if (gsp->target != NULL)   /* may have to map */
			{
				if (gccp->segloc != NULL)
				{
					segloc = gccp->segloc;
					first_seg = gccp->first_seg;
					last_seg = gccp->last_seg;
				}
				else
				{
					segloc = SeqLocCopyPart(head, SeqLocStart(gsp->target),
					SeqLocStop(gsp->target), SeqLocStrand(gsp->target),
					TRUE, &(first_seg), &(last_seg));
				    free_seg = TRUE;
				}

				toffset = SeqLocStart(gsp->target); /* partial first seg */
				if (toffset)
					trunc_l = TRUE;
				else
					trunc_l = FALSE;
				if ((toffset + SeqLocLen(gsp->target)) < BioseqGetLen(bsp))
					trunc_r = TRUE;
				else
					trunc_r = FALSE;

				tlen = SeqLocLen(segloc);

				if (rev)
				{
					segctr = first_seg;
					last_seg = first_seg;
					first_seg = segctr;
					segctr = 0;
				}

			}

			if (gccp->locatetype == segtype)
 			{
				LocateItem = gccp->locateID;
				LocateData = gccp->locatePtr;
			}

			gcp->previtem = NULL;
			gcp->prevtype = 0;
			gcp->parentitem = (Pointer)bsp;
			gcp->parenttype = thistype;
			gcp->thistype = segtype;
			gcp->prevlink = (Pointer PNTR)&(bsp->seq_ext);
			offset = gsp->offset;

			slp = NULL;
			tslp = NULL;
			rdp->l_trunc = FALSE;
			rdp->r_trunc = FALSE;
			while ((slp = SeqLocFindNext(head, slp)) != NULL)
			{
				if ((is_delta) && (segctr))  /* not first one */
				{
					dsp = dsp->next;         /* move up dsp */
					if (dsp == NULL)
						ErrPostEx(SEV_FATAL,0,0,"GatherDeltaSeq: dsp is NULL");
				}

				seglen = SeqLocLen (slp);
				segctr++;
				if (is_delta)
					dataptr = (Pointer)dsp;
				else
					dataptr = (Pointer)slp;

				gccp->itemIDs[segtype]++;
				if (gccp->locatetype == segtype)
				{
					if (LocateItem == gccp->itemIDs[segtype])
						takeit = TRUE;
					if (LocateData == dataptr)
						takeit = TRUE;
				}

				if (takeit)
				{
					if (segloc != NULL)	   /* adjust to target? */
					{
						if ((first_seg > segctr) || (last_seg < segctr))
							in_range = FALSE;
						else
						{
							in_range = TRUE;
							tslp = SeqLocFindNext(segloc, tslp);

							rdp->l_trunc = FALSE;
							rdp->r_trunc = FALSE;

							if (segctr == first_seg)
							{
								offset = gsp->offset; /* allow for partial */
								if (rev)
								{
								   if (trunc_r)
									rdp->l_trunc = TRUE;
								}
								else
								{
									if (trunc_l)
										rdp->l_trunc = TRUE;
								}
							}
							else if (segctr == last_seg)
							{
								if (rev)
								{
								   if (trunc_l)
									rdp->r_trunc = TRUE;
								}
								else
								{
									if (trunc_r)
										rdp->r_trunc = TRUE;
								}
							}

							seglen = SeqLocLen(tslp);

							if (rev)
							{
								rdp->right = offset + tlen - 1;
								rdp->left = rdp->right - seglen + 1;
							}
							else
							{
								rdp->left = offset;
								rdp->right = offset + seglen - 1;
							}
							rdp->strand = SeqLocStrand(tslp);
							tlen -= seglen;
						}
					}
					else
					{
						rdp->left = offset;
						rdp->right = offset + seglen - 1;
						rdp->strand = SeqLocStrand(slp);
					}

					if (in_range)  /* always in_range if no target */
					{
						gcp->thisitem = dataptr;
						gcp->itemID = gccp->itemIDs[segtype];
						GatherAddToStack(gcp);

		
						if ((! (*(gccp->userfunc))(gcp)) || (LocateItem) ||
							(LocateData != NULL))
						{
							if (free_seg)
								SeqLocFree(segloc);
							if (is_delta)
								SeqLocFree(head);
							return FALSE;
						}
					}
				}
				gcp->prevlink = (Pointer PNTR)&(((ValNodePtr)(dataptr))->next);
				gcp->previtem = dataptr;
				gcp->prevtype = segtype;
				offset += seglen;
			}

			if (free_seg)
				SeqLocFree(segloc);
			if (is_delta)
				SeqLocFree(head);
		}
	}

	if(!GatherSeqHist(gccp, bsp->hist, thistype, (Pointer)bsp, (Pointer PNTR)&(bsp->hist), in_scope, match_target))
			return FALSE;

	if (! GatherSeqDescr(gccp, bsp->descr, thistype, (Pointer)bsp,
		                   (Pointer PNTR)&(bsp->descr), in_scope))
		return FALSE;

	if (! GatherSeqAnnot(gccp, bsp->annot, thistype, (Pointer)bsp,
		                   (Pointer PNTR)&(bsp->annot), in_scope))
		return FALSE;

	/* now send targeted features all at once regardless of packaging */
	if (gccp->useSeqMgrIndexes && bsp == gccp->bsp) {
		if (! ExploreSeqFeat (gccp, bsp, in_scope)) {
			return FALSE;
		}
	}

	gcp->indent--;

	return TRUE;
}

static Boolean NEAR GatherBioseqSetFunc (InternalGCCPtr gccp, BioseqSetPtr bsp,
           Pointer parent, Uint2 parenttype, SeqEntryPtr prev, Boolean in_scope,
		   Pointer PNTR prevlink, SeqEntryPtr curr)
{
	GatherContextPtr gcp;
	GatherScopePtr gsp;
	Boolean takeit=TRUE, tscope, checkscope;
	SeqEntryPtr sep, prevsep = NULL, scope = NULL;
	Int2 LocateItem = 0;
	Pointer LocateData = NULL;
	Uint1 thistype;

	if (bsp == NULL)
		return TRUE;

	gcp = &(gccp->gc);
	gsp = &(gccp->scope);

	if (gsp->ignore[OBJ_BIOSEQSET])
		takeit = FALSE;
	else if (! in_scope)
		takeit = FALSE;

	scope = gsp->scope;
	if ((scope != NULL) && (! in_scope))
		checkscope = TRUE;
	else
		checkscope = FALSE;
		
	gccp->itemIDs[OBJ_BIOSEQSET]++;
	gcp->itemID = gccp->itemIDs[OBJ_BIOSEQSET];

	if (gccp->locatetype == OBJ_BIOSEQSET)
	{
		if ((gccp->locateID == gccp->itemIDs[OBJ_BIOSEQSET]) ||
			((Pointer)bsp == gccp->locatePtr))
		{
			takeit = TRUE;
			LocateItem = gccp->locateID;
			LocateData = gccp->locatePtr;
		}
	}

	thistype = OBJ_BIOSEQSET;
	gcp->thistype = thistype;
	gcp->thisitem = (Pointer)bsp;
	GatherAddToStack(gcp);

	if (takeit)
	{
		gcp->sep = curr;
		gcp->previtem = prev;
		gcp->prevtype = OBJ_SEQENTRY;
		gcp->parentitem = parent;
		gcp->parenttype = parenttype;
		gcp->prevlink = prevlink;


		if (! (*(gccp->userfunc))(gcp))
			return FALSE;
		if (LocateItem) return FALSE;
		if (LocateData != NULL) return FALSE;
	}
	gcp->indent++;
	if (! GatherSeqDescr(gccp, bsp->descr, thistype, (Pointer)bsp,
		               (Pointer PNTR)&(bsp->descr), in_scope))
		return FALSE;

	if (! GatherSeqAnnot(gccp, bsp->annot, thistype, (Pointer)bsp,
	               (Pointer PNTR)&(bsp->annot), in_scope))
		return FALSE;

	tscope = in_scope;
	prevlink = (Pointer PNTR)&(bsp->seq_set);
	for (sep = bsp->seq_set; sep != NULL; sep = sep->next)
	{
		gcp->previtem = prevsep;
		gcp->prevtype = OBJ_SEQENTRY;
		gcp->parentitem = (Pointer)bsp;
		gcp->parenttype = thistype;
		gcp->prevlink = prevlink;

		if (checkscope)
		{
			if (sep == scope)
				tscope = TRUE;
			else
				tscope = FALSE;
		}

		if (! GatherSeqEntryFunc(sep, gccp, (Pointer)bsp, OBJ_BIOSEQSET, prevsep, tscope, prevlink))
			return FALSE;

		if (checkscope)
		{
			if (tscope == TRUE)  /* just found it */
			{
				checkscope = FALSE;   /* don't look anymore */
				tscope = FALSE;       /* the siblings not in scope */
				gsp->scope = NULL;    /* no more to look */
			}
			else if (gsp->scope == NULL)   /* found lower down */
			{
				checkscope = FALSE;
				tscope = FALSE;
			}
		}

		prevsep = sep;
		prevlink = (Pointer PNTR)&(sep->next);
	}

	gcp->indent--;  /* reset to original indent level */
	return TRUE;
}

static Boolean NEAR GatherSeqEntryFunc (SeqEntryPtr sep, InternalGCCPtr igccp,
           Pointer parent, Uint2 parenttype, SeqEntryPtr prev, Boolean in_scope,
		   Pointer PNTR prevlink)
{
	if (sep == NULL)
		return TRUE;

	if (IS_Bioseq(sep))
	{
		if (! GatherBioseqFunc(igccp, (BioseqPtr)(sep->data.ptrvalue), parent, parenttype, prev, prevlink, sep, in_scope))
			return FALSE;
	}
	else
	{
		if (! GatherBioseqSetFunc(igccp, (BioseqSetPtr)(sep->data.ptrvalue), parent, parenttype, prev, in_scope, prevlink, sep))
			return FALSE;
	}

	return TRUE;
}

static Boolean NEAR GatherSeqSubCit(InternalGCCPtr gccp, CitSubPtr csp,
        Uint1 ttype, Pointer tparent, Pointer PNTR prevlink)
{
	GatherContextPtr gcp;
	GatherScopePtr gsp;
	Boolean doit = TRUE;
	Int2 LocateItem = 0;
	Pointer LocateData = NULL;

	if (csp == NULL) return TRUE;

	gcp = &(gccp->gc);
	gsp = &(gccp->scope);

	if (gsp->ignore[OBJ_SEQSUB_CIT])
		return TRUE;

	gccp->itemIDs[OBJ_SEQSUB_CIT]++;

	if (gccp->locatetype == OBJ_SEQSUB_CIT)
	{
		LocateItem = gccp->locateID;
		LocateData = gccp->locatePtr;
		if ((gccp->itemIDs[OBJ_SEQSUB_CIT] != LocateItem) &&
			(LocateData != (Pointer)csp))
			doit = FALSE;
	}

	gcp->previtem = NULL;
	gcp->prevtype = 0;
	gcp->parentitem = tparent;
	gcp->parenttype = ttype;
	gcp->thisitem = (Pointer)csp;
	gcp->thistype = OBJ_SEQSUB_CIT;
	gcp->prevlink = prevlink;
	gcp->itemID = gccp->itemIDs[OBJ_SEQSUB_CIT];
	GatherAddToStack(gcp);


	if (doit)
	{
		if (! (*(gccp->userfunc))(gcp))
			return FALSE;
		if (LocateItem) return FALSE;
		if (LocateData) return FALSE;
	}

	return TRUE;
}

static Boolean NEAR GatherSeqSubContact(InternalGCCPtr gccp, ContactInfoPtr cip,
        Uint1 ttype, Pointer tparent, Pointer PNTR prevlink)
{
	GatherContextPtr gcp;
	GatherScopePtr gsp;
	Int2 LocateItem = 0;
	Pointer LocateData = NULL;
	Boolean doit = TRUE;

	if (cip == NULL) return TRUE;

	gcp = &(gccp->gc);
	gsp = &(gccp->scope);

	if (gsp->ignore[OBJ_SEQSUB_CONTACT])
		return TRUE;

	gccp->itemIDs[OBJ_SEQSUB_CONTACT]++;

	if (gccp->locatetype == OBJ_SEQSUB_CONTACT)
	{
		LocateItem = gccp->locateID;
		LocateData = gccp->locatePtr;
		if ((gccp->itemIDs[OBJ_SEQSUB_CONTACT] != LocateItem) &&
			((Pointer)cip != LocateData))
			doit = FALSE;
	}

	gcp->previtem = NULL;
	gcp->prevtype = 0;
	gcp->parentitem = tparent;
	gcp->parenttype = ttype;
	gcp->thisitem = (Pointer)cip;
	gcp->thistype = OBJ_SEQSUB_CONTACT;
	gcp->prevlink = prevlink;
	gcp->itemID = gccp->itemIDs[OBJ_SEQSUB_CONTACT];
	GatherAddToStack(gcp);

	if (doit)
	{
		if (! (*(gccp->userfunc))(gcp))
			return FALSE;
		if (LocateItem) return FALSE;
		if (LocateData != NULL) return FALSE;
	}

	return TRUE;
}

static Boolean NEAR GatherSubBlock(InternalGCCPtr gccp, SubmitBlockPtr sbp,
        Uint1 ttype, Pointer tparent, Pointer PNTR prevlink)
{
	GatherContextPtr gcp;
	GatherScopePtr gsp;
	Int2 LocateItem = 0;
	Pointer LocateData = NULL;
	Boolean doit = TRUE;

	if (sbp == NULL) return TRUE;

	gcp = &(gccp->gc);
	gsp = &(gccp->scope);

	if (gsp->ignore[OBJ_SUBMIT_BLOCK])
		return TRUE;

	gccp->itemIDs[OBJ_SUBMIT_BLOCK]++;

	if (gccp->locatetype != 0) {
		doit = FALSE;
		if (gccp->locatetype == OBJ_SUBMIT_BLOCK)
		{
			LocateItem = gccp->locateID;
			LocateData = gccp->locatePtr;
			if (gccp->itemIDs[OBJ_SUBMIT_BLOCK] == LocateItem)
				doit = TRUE;
			if (LocateData == (Pointer)sbp)
				doit = TRUE;
		}
	}

	gcp->previtem = NULL;
	gcp->prevtype = 0;
	gcp->parentitem = tparent;
	gcp->parenttype = ttype;
	gcp->thisitem = (Pointer)sbp;
	gcp->thistype = OBJ_SUBMIT_BLOCK;
	gcp->prevlink = prevlink;
	gcp->itemID = gccp->itemIDs[OBJ_SUBMIT_BLOCK];
 	GatherAddToStack(gcp);

	if (doit)
	{
		if (! (*(gccp->userfunc))(gcp))
			return FALSE;
		if (LocateItem) return FALSE;
		if (LocateData != NULL) return FALSE;
	}

	gcp->indent++;

	if (! GatherSeqSubContact(gccp, sbp->contact, OBJ_SUBMIT_BLOCK,
										(Pointer)sbp, (Pointer PNTR)&(sbp->contact)))
		return FALSE;

	if (! GatherSeqSubCit(gccp, sbp->cit, OBJ_SUBMIT_BLOCK,
										(Pointer)sbp, (Pointer PNTR)&(sbp->cit)))
		return FALSE;

    gcp->indent--;
	return TRUE;
}


static Boolean NEAR GatherSeqSubmit (InternalGCCPtr gccp, SeqSubmitPtr ssp, Boolean in_scope)
{
	GatherContextPtr gcp;
	GatherScopePtr gsp;
	Boolean takeit=TRUE, tscope, checkscope;
	SeqEntryPtr sep, prevsep = NULL, scope = NULL;
	Int2 LocateItem = 0;
	Pointer LocateData = NULL;
	Uint1 thistype;
	Pointer PNTR prevlink;

	if (ssp == NULL)
		return TRUE;

	gcp = &(gccp->gc);
	gsp = &(gccp->scope);

	if (gsp->ignore[OBJ_SEQSUB])
		takeit = FALSE;
	else if (! in_scope)
		takeit = FALSE;

	scope = gsp->scope;
	if ((scope != NULL) && (! in_scope))
		checkscope = TRUE;
	else
		checkscope = FALSE;
		
	gccp->itemIDs[OBJ_SEQSUB]++;
	gcp->itemID = gccp->itemIDs[OBJ_SEQSUB];

	if (gccp->locatetype == OBJ_SEQSUB)
	{
		if ((gccp->locateID == gccp->itemIDs[OBJ_SEQSUB]) ||
			(gccp->locatePtr == (Pointer)ssp))
		{
			takeit = TRUE;
			LocateItem = gccp->locateID;
			LocateData = gccp->locatePtr;
		}
	}

	thistype = OBJ_SEQSUB;
	gcp->thistype = thistype;
	gcp->thisitem = (Pointer)ssp;
	GatherAddToStack(gcp);

	if (takeit)
	{
		gcp->sep = NULL;
		gcp->previtem = NULL;
		gcp->prevtype = 0;
		gcp->parentitem = NULL;
		gcp->parenttype = 0;
		gcp->thisitem = (Pointer)ssp;
		gcp->thistype = thistype;
		gcp->prevlink = NULL;


		if (! (*(gccp->userfunc))(gcp))
			return FALSE;
		if (LocateItem) return FALSE;
		if (LocateData != NULL) return FALSE;
	}
    
    gcp->indent++;
	if (! GatherSubBlock(gccp, ssp->sub, thistype, (Pointer)ssp,
		               (Pointer PNTR)&(ssp->sub)))
		return FALSE;
    
	tscope = in_scope;
	prevlink = (Pointer PNTR)&(ssp->data);

	switch(ssp->datatype)
	{
		case 1:    /* Seq-entrys */
			prevsep = NULL;
			for (sep = (SeqEntryPtr)(ssp->data); sep != NULL; sep = sep->next)
			{
				gcp->previtem = prevsep;
				gcp->prevtype = OBJ_SEQENTRY;
				gcp->parentitem = (Pointer)ssp;
				gcp->parenttype = thistype;
				gcp->prevlink = prevlink;

				if (checkscope)
				{
					if (sep == scope)
						tscope = TRUE;
					else
						tscope = FALSE;
				}

				if (! GatherSeqEntryFunc(sep, gccp, (Pointer)ssp, OBJ_SEQSUB, prevsep, tscope, prevlink))
					return FALSE;

				if (checkscope)
				{
					if (tscope == TRUE)  /* just found it */
					{
						checkscope = FALSE;   /* don't look anymore */
						tscope = FALSE;       /* the siblings not in scope */
						gsp->scope = NULL;    /* no more to look */
					}
					else if (gsp->scope == NULL)   /* found lower down */
					{
						checkscope = FALSE;
						tscope = FALSE;
					}
				}

				prevsep = sep;
				prevlink = (Pointer PNTR)&(sep->next);
			}
			break;
		case 2:    /* Seq-annots */
			if (! GatherSeqAnnot(gccp, (SeqAnnotPtr)(ssp->data), thistype,
						(Pointer)ssp,(Pointer PNTR)&(ssp->data), in_scope))
				return FALSE;

			break;
		case 3:    /* SeqIds */
			if (! GatherSeqIds(gccp, (SeqIdPtr)(ssp->data), thistype,
						(Pointer)ssp,(Pointer PNTR)&(ssp->data)))
				return FALSE;

			break;
		default:
			break;
	}
	gcp->indent--;  /* reset to original indent level */
	return TRUE;
}

/*****************************************************************************
*
*   GatherBioseqPartsFunc(gccp, top)
*      gets parts not contained in "top" for segmented entry
*
*****************************************************************************/
static Boolean NEAR GatherBioseqPartsFunc (InternalGCCPtr gccp, Pointer top)
{
	GatherContextPtr gcp;
	GatherScopePtr gsp, tgsp;
	GatherScope scopebuf;
	SeqLocPtr slp, head;
	SeqIdPtr sip;
	Boolean retval;
	BioseqPtr tbsp;
	SeqEntryPtr sep;
	Int4 len;
	Int2 ctr;
	Boolean doit;

	gcp = &(gccp->gc);
	gsp = &(gccp->scope);

	if (gsp->seglevels <= gsp->currlevel)
		return TRUE;


	head = gccp->segloc;

	tgsp = &scopebuf;
	MemCopy(tgsp, gsp, sizeof(GatherScope));
	tgsp->seglevels--;
	tgsp->scope = NULL;
	tgsp->currlevel++;
	tgsp->ignore [OBJ_BIOSEQ] = TRUE;

	slp = NULL;
	
	retval = FALSE;	/*??*/
	ctr = 0;
	while ((slp = SeqLocFindNext(head, slp)) != NULL)
	{
		doit = FALSE;
		tgsp->target = slp;
		sip = SeqLocId(slp);
		tbsp = BioseqLockById(sip);
		len = SeqLocLen(slp);

		if (len >= 0 && tbsp != NULL)
		{
			doit = TRUE;
			if ((gsp->stop_on_annot) && (ctr < gccp->segcnt))
			{
				if (gccp->found_annot[ctr])  /* already found annot here */
					doit = FALSE;
			}
		}

		if (doit)
		{
			if (! ObjMgrIsChild(top, (Pointer)tbsp)) /* in set we just did? */
			{
				sep = SeqEntryFind(sip);
				retval = GatherSeqEntry(sep, gcp->userdata, gccp->userfunc, tgsp);
			}
			BioseqUnlock(tbsp);
			if (! retval)
				return FALSE;
		}
		else if (tbsp != NULL)
			BioseqUnlock(tbsp);
		
		tgsp->offset += len;
		ctr++;
	}

	return TRUE;
}

static Boolean WholeLocOnBioseq (BioseqPtr bsp, SeqLocPtr slp)

{
  SeqIntPtr sintp;
  SeqIdPtr  sip;

  if (bsp == NULL || slp == NULL) return FALSE;

  sip = SeqLocId (slp);
  if (sip == NULL) return FALSE;
  if (! SeqIdIn (sip, bsp->id)) return FALSE;

  if (slp->choice == SEQLOC_WHOLE) return TRUE;
  if (slp->choice == SEQLOC_INT) {
    sintp = (SeqIntPtr) slp->data.ptrvalue;
    if (sintp != NULL &&
        sintp->from == 0 &&
        sintp->to == bsp->length - 1) return TRUE;
  }

  return FALSE;
}

static Boolean NEAR IGCCBuild (InternalGCCPtr ip, ObjMgrDataPtr omdp, Pointer userdata, GatherItemProc userfunc, GatherScopePtr scope)
{
	Boolean in_scope = TRUE;
	SeqIdPtr sip;
	BioseqPtr bsp = NULL;
	ValNode fake;
	SeqLocPtr slp;
	Int2 ctr;
	SeqEntryPtr oldsep;
	ObjMgrDataPtr newomdp = NULL;
	Boolean reload_from_cache = TRUE;   /* default behavior */
	DeltaSeqPtr dsp;
	SeqLocPtr loc = NULL;
	Uint2 entityID;

	if ((omdp == NULL) || (userfunc == NULL)) return FALSE;

	MemSet((Pointer)(ip), 0, sizeof(InternalGCC));


	if (scope != NULL)  /* check for turning off reload from cache */
	{
		if (scope->do_not_reload_from_cache)
			reload_from_cache = FALSE;
	}

	if ((omdp->tempload == TL_CACHED) && (reload_from_cache))  /* must reload from cache */
	{
		newomdp = BioseqReload(omdp, TRUE);
		if (newomdp == NULL)
		{
			ErrPostEx(SEV_ERROR,0,0,"IGCCBuild: Couldn't reload data from cache.");
			return FALSE;
		}
		ip->reloaded = TRUE;
		omdp = newomdp;

		if (scope != NULL)
		{
			if(scope->scope != NULL)
			{
				ErrPostEx(SEV_ERROR,0,0,"IGCCBuild: Scope invalid, data reloaded from cache.");
				ObjMgrLock(omdp->datatype, omdp->dataptr, FALSE);
				return FALSE;
			}
		}
	}
	else
		ObjMgrLock(omdp->datatype, omdp->dataptr, TRUE);  /* just lock it */

	ip->omdp = omdp;
	ip->gc.userdata = userdata;
	ip->userfunc = userfunc;
	if (scope != NULL)
		MemCopy(&(ip->scope), scope, sizeof(GatherScope));
	ip->gc.entityID = omdp->EntityID;
	ip->gc.tempload = omdp->tempload;
	ip->gc.seglevel = ip->scope.currlevel;
	ip->gc.igccp = (Pointer)ip;
	if (ObjMgrCheckHold())
		ip->gc.hold = TRUE;

	if (ip->scope.target != NULL)
	{
		sip = SeqLocId(ip->scope.target);

		/* first check if target in the SeqEntry -- necessary if multiple versions with same id */
		if ((omdp->choicetype == OBJ_SEQENTRY && omdp->choice != NULL) ||
		    (scope != NULL && scope->scope != NULL))
		{
			oldsep = SeqEntryGetScope();  /* save any current scope */
			if (omdp->choicetype == OBJ_SEQENTRY) {
				SeqEntrySetScope(omdp->choice);
			} else {
				SeqEntrySetScope(scope->scope);
			}
			bsp = BioseqFindCore(sip);
			if (bsp != NULL)    /* yes it is in here */
				bsp = BioseqLockById(sip);
			SeqEntrySetScope(oldsep);
		}

		if (bsp == NULL)			  /* out of scope? */
			bsp = BioseqLockById(sip);/* try again */

		ip->bsp = bsp;
		if (SeqLocStrand(ip->scope.target) == Seq_strand_minus)
			ip->rev = TRUE;
		if ((ip->scope.seglevels > ip->scope.currlevel)
			&& (bsp != NULL))     /* get seg parts? */
		{
			slp = NULL;
			loc = NULL;
			if (bsp->repr == Seq_repr_seg)
			{
				fake.choice = SEQLOC_MIX;
				fake.next = NULL;
				fake.data.ptrvalue = bsp->seq_ext;
				slp = &fake;
			} else if (bsp->repr == Seq_repr_delta && bsp->seq_ext_type == 4) {
				dsp = (DeltaSeqPtr) bsp->seq_ext;
				loc = DeltaSeqsToSeqLocs (dsp);
				slp = loc;
			}
			else if (bsp->repr == Seq_repr_ref)
				slp = (SeqLocPtr)(bsp->seq_ext);

			ip->segloc = SeqLocCopyPart(slp, SeqLocStart(ip->scope.target),
				SeqLocStop(ip->scope.target), SeqLocStrand(ip->scope.target),
				TRUE, &(ip->first_seg), &(ip->last_seg));

			if (ip->scope.stop_on_annot)
			{

				slp = NULL; ctr = 0;
				while ((slp = SeqLocFindNext(ip->segloc, slp)) != NULL)
					ctr++;
				if (ctr)
				{
					ip->segcnt = ctr;
					ip->seglens = MemNew((size_t)(sizeof(Int4) * ctr));
					ip->found_annot = MemNew((size_t)(sizeof(Boolean) * ctr));

					slp = NULL; ctr = 0;
					while ((slp = SeqLocFindNext(ip->segloc, slp)) != NULL)
					{
						ip->seglens[ctr] = SeqLocLen(slp);
						if (ip->seglens[ctr] < 0)
							ip->seglens[ctr] = 0;
						ctr++;
					}
				}
			}
			loc = SeqLocFree (loc);
		}

		/* if targeted gather, try to use feature indexing for speed */
		if (bsp != NULL && bsp->repr == Seq_repr_raw &&
			SeqMgrGetParentOfPart (bsp, NULL) == NULL &&
			ip->segloc == NULL && ip->scope.useSeqMgrIndexes &&
			(ip->scope.get_feats_location || ip->scope.get_feats_product) &&
			(! (ip->scope.ignore [OBJ_SEQFEAT]))) {

			/* target must be whole or full length interval on bioseq */
			if (! WholeLocOnBioseq (bsp, ip->scope.target)) return TRUE;

			entityID = ObjMgrGetEntityIDForPointer (bsp);

			/* index features if not already done */
			if (SeqMgrFeaturesAreIndexed (entityID) == 0) {
				SeqMgrIndexFeatures (entityID, NULL);
			}

			if (SeqMgrFeaturesAreIndexed (entityID) != 0) {
				ip->useSeqMgrIndexes = TRUE;
			}
		}
	}

	return TRUE;
}

NLM_EXTERN AlignDataPtr FreeAlignData(AlignDataPtr adp)
{
	AlignDataPtr next;
	AlignRangePtr arp, arp_next;
	
	while(adp)
	{
		arp = adp->arp;
		while(arp)
		{
			arp_next = arp->next;
			arp->next = NULL;
			MemFree(arp);
			arp = arp_next;
		}
		next = adp->next;
		adp->next = NULL;
		MemFree(adp);
		adp = next;
	}
	return NULL;
}
	
	
static Boolean IGCCclear(InternalGCCPtr ip)
{
	if (ip == NULL) return TRUE;

	BioseqUnlock(ip->bsp);
	ObjMgrLock(ip->omdp->datatype, ip->omdp->dataptr, FALSE);  /* unlock the entity */

	MemFree(ip->gc.rdp);
	FreeAlignData(ip->gc.adp);
	ip->gc.adp = NULL;
	SeqLocFree(ip->segloc);
	MemFree(ip->seglens);
	MemFree(ip->found_annot);
	MemFree(ip->gc.gatherstack);
	MemFree(ip->gc.extra_loc);

	return TRUE;
}

static void GatherEntityFunc (ObjMgrDataPtr omdp, InternalGCCPtr gccp, Boolean in_scope)
{
	ValNodePtr vnp;
	Pointer ptr;

	vnp = omdp->choice;
	ptr = omdp->dataptr;

	switch (omdp->choicetype)
	{
		case OBJ_SEQENTRY:
			GatherSeqEntryFunc(vnp, gccp, NULL, 0, NULL, in_scope, NULL);
			break;

		default:
			switch (omdp->datatype)
			{
				case OBJ_BIOSEQ:
					GatherBioseqFunc(gccp, (BioseqPtr)ptr, NULL, 0, NULL, NULL,
						NULL, in_scope);
					break;

				case OBJ_BIOSEQSET:
					GatherBioseqSetFunc(gccp, (BioseqSetPtr)ptr, NULL, 0, NULL,
						in_scope, NULL, NULL);
					break;

				case OBJ_SEQDESC:
					GatherSeqDescr(gccp, (ValNodePtr)ptr, 0, NULL, NULL,in_scope);
					break;

				case OBJ_SEQANNOT:
					GatherSeqAnnot(gccp, (SeqAnnotPtr)ptr, 0,NULL,NULL, in_scope);
					break;

				case OBJ_ANNOTDESC:              /* NOT SUPPORTED YET */
					break;

				case OBJ_SEQFEAT:
					GatherSeqFeat(gccp, (SeqFeatPtr)ptr, 0, NULL, NULL, in_scope, OBJ_SEQFEAT);
					break;

				case OBJ_SEQALIGN:
					GatherSeqAlign(gccp, (SeqAlignPtr)ptr, 0, NULL, NULL, in_scope, TRUE, OBJ_SEQALIGN);
					break;

				case OBJ_SEQHIST_ALIGN:
					GatherSeqAlign(gccp, (SeqAlignPtr)ptr, 0, NULL, NULL, in_scope, TRUE, OBJ_SEQHIST_ALIGN);
					break;

				case OBJ_SEQGRAPH:
					GatherSeqGraph(gccp, (SeqGraphPtr)ptr, 0, NULL, NULL, in_scope);
					break;

				case OBJ_SEQSUB:
					GatherSeqSubmit (gccp, (SeqSubmitPtr) ptr, in_scope);
					break;

				case OBJ_SUBMIT_BLOCK:
					GatherSubBlock(gccp, (SubmitBlockPtr)ptr,0,NULL,NULL);
					break;

				case OBJ_SEQSUB_CONTACT:
					GatherSeqSubContact(gccp, (ContactInfoPtr)ptr,0, NULL, NULL);
					break;

				case OBJ_BIOSEQ_MAPFEAT:        /* NOT SUPPORTED YET */
					break;

				case OBJ_BIOSEQ_SEG:			/* NOT SEPARATELY SUPPORTED */
					break;

				case OBJ_SEQHIST:    			/* NOT SEPARATELY SUPPORTED */
					GatherSeqHist(gccp, (SeqHistPtr)ptr, 0, NULL, NULL, in_scope, TRUE);
					break;

				case OBJ_PUB:
					GatherPub(gccp, (ValNodePtr)ptr, 0, 0, NULL,NULL, in_scope);
					break;

				case OBJ_SEQFEAT_CIT:			/* NOT SEPARATELY SUPPORTED */
					break;

				case OBJ_SEQSUB_CIT:
					GatherSeqSubCit(gccp, (CitSubPtr)ptr,0, NULL, NULL);
					break;

				case OBJ_PUB_SET:
					GatherPubSet(gccp, (ValNodePtr)ptr, 0, 0, NULL,NULL, in_scope);
					break;

				case OBJ_SEQID:
					GatherSeqIds(gccp, (SeqIdPtr)ptr,0,NULL,NULL);
					break;

				default:
					break;
			}
	}


}
/*****************************************************************************
*
*   FocusSeqEntry (sep, scope)
*      zeros out all fields in scope
*      sets scope.target, .scope, .propagate_desciptors appropriately for 
*         SeqEntry
*      if (Bioseq)
*         target is the bioseq
*         entityID is the containing set if any
*         scope.scope is null
*      if (BioseqSet)
*         target is NULL
*         entityID is the containing set if any
*         scope.scope is sep
*         propagate_descriptors is TRUE
*
*      return of FOCUS_INITIALIZED means scope is initialized
*      return of FOCUS_NOT_NEEDED means send NULL for GatherScope.. you
*        don't need to scope for this SeqEntryPtr
*      return of FOCUS_ERROR  means it could not be initialized
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL FocusSeqEntry (SeqEntryPtr sep, GatherScopePtr scope)
{
	Int2 retval = FOCUS_ERROR;
	ObjMgrDataPtr omdp;
	ObjMgrPtr omp;
	SeqIdPtr sip;
	SeqLocPtr slp;
	BioseqPtr bsp;

	if ((sep == NULL) || (scope == NULL))
		return retval;

	MemSet((Pointer)scope, 0, sizeof(GatherScope));

	omp = ObjMgrReadLock();
	omdp = ObjMgrFindByData(omp, sep->data.ptrvalue);
	if (omdp == NULL) goto erret;

	if (omdp->parentptr == NULL)
	{
		retval = FOCUS_NOT_NEEDED;
		goto erret;
	}

	if (IS_Bioseq(sep))
	{
		slp = ValNodeNew(NULL);
		slp->choice = SEQLOC_WHOLE;
		bsp = (BioseqPtr) sep->data.ptrvalue;
		sip = SeqIdDup (SeqIdFindBest (bsp->id, 0));
		slp->data.ptrvalue = sip;
		scope->target = slp;
	}
	else
	{
		scope->scope = sep;
		omdp = ObjMgrFindTop(omp, omdp);
		if (omdp->tempload == TL_CACHED)
		{
			ErrPostEx(SEV_ERROR, 0,0, "FocusSeqEntry: Scope to BioseqSet on Cached record");
			goto erret;
		}
	}
	retval = FOCUS_INITIALIZED;
erret:
	ObjMgrUnlock();
	return retval;
}

/*****************************************************************************
*
*   GatherSeqEntry (sep, userdata, userproc, scope)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL GatherSeqEntry (SeqEntryPtr sep, Pointer userdata, GatherItemProc userfunc, GatherScopePtr scope)
{
	Uint2 entityID;

	if ((sep == NULL) || (userfunc == NULL)) return FALSE;

	entityID = ObjMgrGetEntityIDForChoice(sep);
	
	return GatherEntity (entityID, userdata, userfunc, scope);
}

/*****************************************************************************
*
*   GatherEntity (entityID, userdata, userproc, scope)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL GatherEntity (Uint2 entityID, Pointer userdata, GatherItemProc userfunc, GatherScopePtr scope)
{
	InternalGCC igcc;
	Boolean in_scope;
	ObjMgrDataPtr omdp;
	Boolean reloaded_from_cache = FALSE;
	ObjMgrPtr omp;

	if ((! entityID) || (userfunc == NULL)) return FALSE;

	omp = ObjMgrReadLock();
	omdp = ObjMgrGetDataStruct (omp, entityID);
	ObjMgrUnlock();

	if (omdp == NULL) return FALSE;

	if (! IGCCBuild(&igcc, omdp, userdata, userfunc, scope))
		return FALSE;

	omdp = igcc.omdp;   /* could have been reloaded */
	if (igcc.scope.scope != NULL)
	{
			in_scope = ObjMgrIsChild((igcc.scope.scope->data.ptrvalue), (omdp->dataptr));
	}
	else                   /* no scope set.. all in scope */
		in_scope = TRUE;

	GatherEntityFunc(omdp, &igcc, in_scope);

	if ((igcc.segloc != NULL) && (igcc.scope.scope == NULL)) /* get segs first */
		GatherBioseqPartsFunc(&igcc, omdp->dataptr);


	return IGCCclear(&igcc);
}
/**************************************************************************
*
*   callback used by GatherItemIDByData
*
***************************************************************************/
static Boolean GatherItemByDataProc (GatherContextPtr gcp)
{
	Uint2Ptr ptr;

	ptr = (Uint2Ptr)(gcp->userdata);
	*ptr = gcp->itemID;

	return TRUE;
}
 
/*****************************************************************************
*
*   GatherItemIDByData (entityID, itemtype, dataptr)
*      Looks in entityID for an element of itemtype that matches the pointer
*         dataptr.
*      if found, returns the itemID
*      else returns 0
*      itemtype is as defined in objmgr.h for OBJ_
*
*****************************************************************************/
NLM_EXTERN Uint2 LIBCALL GatherItemIDByData (Uint2 entityID, Uint2 itemtype, Pointer dataptr)
{
	Uint2 itemID = 0;

	GatherData(entityID, dataptr, itemtype, (Pointer)(&itemID), GatherItemByDataProc);
	return itemID;
}


/*****************************************************************************
*
*   GatherData (entityID, itemID, itemtype, userdata, userproc)
*      Get an item by entityID, itemtype, and a Pointer of itemtype
*      GatherContext.seglevel and GatherContext.propagated will not be
*        set on the callback.
*   
*      Sets in_scope to FALSE so that the callback is not called
*         Scope is NULL, so in_scope is never TRUE
*      Sets ignore TRUE for everything not needed to find item
*      Sets locatetype and locateID, which are checked in the traversal
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL GatherData (Uint2 entityID, Pointer dataptr, Uint2 itemtype,
                                   Pointer userdata, GatherItemProc userfunc)
{
	return GatherItemFunc (entityID, 0, itemtype, userdata, userfunc, dataptr, FALSE);
}

/*****************************************************************************
*
*   GatherItem (entityID, itemID, itemtype, userdata, userproc)
*      Get an item by entityID, itemID, itemtype
*      GatherContext.seglevel and GatherContext.propagated will not be
*        set on the callback.
*   
*      Sets in_scope to FALSE so that the callback is not called
*         Scope is NULL, so in_scope is never TRUE
*      Sets ignore TRUE for everything not needed to find item
*      Sets locatetype and locateID, which are checked in the traversal
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL GatherItem (Uint2 entityID, Uint2 itemID, Uint2 itemtype,
                                   Pointer userdata, GatherItemProc userfunc)
{
	return GatherItemFunc (entityID, itemID, itemtype, userdata, userfunc, NULL, FALSE);
}

/*****************************************************************************
*
*   GatherItemFunc (entityID, itemID, itemtype, userdata, userproc, dataptr)
*      Get an item by entityID, itemID, itemtype if dataptr is NULL
*         else it matches entityID, itemtype, dataptr (of type itemtype)
*      GatherContext.seglevel and GatherContext.propagated will not be
*        set on the callback.
*   
*      Sets in_scope to FALSE so that the callback is not called
*         Scope is NULL, so in_scope is never TRUE
*      Sets ignore TRUE for everything not needed to find item
*      Sets locatetype and locateID, which are checked in the traversal
*
*****************************************************************************/
static Boolean NEAR GatherItemFunc (Uint2 entityID, Uint2 itemID, Uint2 itemtype,
                                   Pointer userdata, GatherItemProc userfunc, Pointer dataptr,
								   Boolean do_not_reload_from_cache)

{
	InternalGCC igcc;
	ObjMgrDataPtr omdp;
	GatherScope gs;
	ObjMgrPtr omp;

	if (userfunc == NULL) return FALSE;

	if (itemtype >= OBJ_MAX) return FALSE;

	omp = ObjMgrReadLock();
	omdp = ObjMgrGetDataStruct(omp, entityID);
	ObjMgrUnlock();

	if (omdp == NULL) return FALSE;

	MemSet((Pointer)(&gs), 0, sizeof(GatherScope));
	gs.do_not_reload_from_cache = do_not_reload_from_cache;
	MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)((OBJ_MAX)*sizeof(Boolean)));
	gs.ignore[itemtype] = FALSE;
	switch (itemtype)		  /* add higher levels if necessary */
	{
		case OBJ_SEQFEAT_CIT:
			gs.ignore[OBJ_SEQFEAT] = FALSE;
			gs.ignore[OBJ_PUB_SET] = FALSE;
		case OBJ_SEQFEAT:
			gs.ignore[OBJ_SEQANNOT] = FALSE;
			break;
		case OBJ_SEQALIGN:
			gs.ignore[OBJ_SEQANNOT] = FALSE;
			break;
		case OBJ_SEQHIST_ALIGN:
			gs.ignore[OBJ_SEQHIST] = FALSE;
			break;
		case OBJ_SEQGRAPH:
			gs.ignore[OBJ_SEQANNOT] = FALSE;
			break;
		case OBJ_SEQSUB_CONTACT:
		case OBJ_SEQSUB_CIT:
			gs.ignore[OBJ_SUBMIT_BLOCK] = FALSE;
		case OBJ_SUBMIT_BLOCK:
			gs.ignore[OBJ_SEQSUB] = FALSE;
			break;
	}

	if (! IGCCBuild(&igcc, omdp, userdata, userfunc, &gs))
		return FALSE;

	omdp = igcc.omdp;   /* could have been reloaded */

	igcc.locatetype = (Uint1)itemtype;
	igcc.locateID = itemID;
	igcc.locatePtr = dataptr;
    

	GatherEntityFunc(omdp, &igcc, FALSE);

	return IGCCclear(&igcc);
}

typedef struct gatherdatastruct {
	OMProcControlPtr ompcp;
	Boolean succeeded;
} GDS, PNTR GDSPtr;

static Boolean GatherDataProc (GatherContextPtr gcp)
{
	OMProcControlPtr ompcp;
	GDSPtr gdsp;

	gdsp = (GDSPtr)(gcp->userdata);
	ompcp = gdsp->ompcp;
    
    ompcp->input_data = gcp->thisitem;
    
	switch (gcp->thistype)
	{
		case OBJ_BIOSEQSET:
		case OBJ_BIOSEQ:
			if (gcp->sep != NULL)
			{
				ompcp->input_choice = gcp->sep;
				ompcp->input_choicetype = OBJ_SEQENTRY;
			}
			break;
		default:
			break;
	}
	gdsp->succeeded = TRUE;
	return TRUE;
}

static Boolean ReplaceDataProc (GatherContextPtr gcp)
{
	ObjMgrPtr omp;
	ObjMgrTypePtr omtp;
	ObjMgrDataPtr omdp;
	OMProcControlPtr ompcp;
	Pointer oldptr, newptr;
	GDSPtr gdsp;
	SeqEntryPtr sep=NULL;


	gdsp = (GDSPtr)(gcp->userdata);
	ompcp = gdsp->ompcp;
	newptr = ompcp->output_data;

    oldptr = gcp->thisitem;

	omp = ObjMgrReadLock();
	omtp = ObjMgrTypeFind(omp, gcp->thistype, NULL, NULL);
	ObjMgrUnlock();

	if (omtp == NULL)
	{
		ErrPostEx(SEV_ERROR,0,0,"ReplaceDataProc: can't find type [%d]", (int)gcp->thistype);
		return TRUE;
	}

	if (! GatherOverWrite(oldptr, newptr, gcp->thistype))	/* overwrite the old data object */
		return TRUE;

	if (ompcp->output_entityID)   /* remove with objmgr? */
	{
		omp = ObjMgrReadLock();
		omdp = ObjMgrGetDataStruct (omp, ompcp->output_entityID);
		ObjMgrUnlock();

		if (omdp != NULL)
		{
			if ((gcp->thistype == OBJ_BIOSEQ) || (gcp->thistype == OBJ_BIOSEQSET))
				sep = omdp->choice;
			if (ObjMgrWholeEntity(omdp, ompcp->output_itemID, ompcp->output_itemtype))
				ObjMgrDelete(ompcp->output_itemtype, ompcp->output_data);
		}
	}

	if (sep != NULL)
		SeqEntryFree(sep);
	else
		(*(omtp->freefunc))(ompcp->output_data);

	gdsp->succeeded = TRUE;

	return TRUE;
}

static Boolean DetachDataProc (GatherContextPtr gcp)
{
	OMProcControlPtr ompcp;
	Pointer next = NULL, ptr;
	GDSPtr gdsp;

	gdsp = (GDSPtr)(gcp->userdata);
	ompcp = gdsp->ompcp;

    ptr = gcp->thisitem;
    ompcp->input_data = ptr;
    
	switch (gcp->thistype)
	{
		case OBJ_BIOSEQSET:
		case OBJ_BIOSEQ:
			if (gcp->sep != NULL)
			{
				ompcp->input_choice = gcp->sep;
				ompcp->input_choicetype = OBJ_SEQENTRY;
				next = gcp->sep->next;
				gcp->sep->next = NULL;
			}
			break;
		case OBJ_SEQDESC:
		case OBJ_BIOSEQ_SEG:
		case OBJ_PUB:
		case OBJ_SEQFEAT_CIT:
		case OBJ_PUB_SET:
		case OBJ_SEQLOC:
		case OBJ_SEQID:
		case OBJ_SEQENTRY:
			next = (Pointer)(((ValNodePtr)(ptr))->next);
			(((ValNodePtr)(ptr))->next) = NULL;
			break;
		case OBJ_BIOSEQ_MAPFEAT:
		case OBJ_SEQFEAT:
			next = (Pointer)(((SeqFeatPtr)(ptr))->next);
			(((SeqFeatPtr)(ptr))->next) = NULL;
			break;
		case OBJ_SEQANNOT:
			next = (Pointer)(((SeqAnnotPtr)(ptr))->next);
			(((SeqAnnotPtr)(ptr))->next) = NULL;
			break;
		case OBJ_SEQALIGN:
		case OBJ_SEQHIST_ALIGN:
			next = (Pointer)(((SeqAlignPtr)(ptr))->next);
			(((SeqAlignPtr)(ptr))->next) = NULL;
			break;
		case OBJ_SEQGRAPH:
			next = (Pointer)(((SeqGraphPtr)(ptr))->next);
			(((SeqGraphPtr)(ptr))->next) = NULL;
			break;
		default:
			break;
	}
	if (ompcp->whole_entity)  /* nothing to detach from */
		return TRUE;

	if (gcp->prevlink != NULL)
		*(gcp->prevlink) = next;

	ObjMgrDetach(gcp->thistype, ptr);

	gdsp->succeeded = TRUE;

	return TRUE;
}

static void AddAnAnnot(SeqAnnotPtr PNTR head, Pointer ptr)
{
	SeqAnnotPtr prev, sap;

	if (head == NULL) return;
	sap = (SeqAnnotPtr)ptr;

	if (*head == NULL)
	{
		*head = sap;
		return;
	}

	for (prev = (*head); prev->next != NULL; prev = prev->next)
		continue;

	prev->next = sap;
	return;
}

static Boolean AddToSeqAnnot (SeqAnnotPtr sap, Int2 the_type, Pointer addptr)
{
	SeqFeatPtr sfp;
	SeqAlignPtr salp;
	SeqGraphPtr sgp;
	Pointer PNTR prevlink = NULL;

	if (sap == NULL) return FALSE;

	if (sap->type == 0)
		sap->type = (Uint1)the_type;

	if (sap->type != (Uint1)the_type)
		return FALSE;

	if (sap->data == NULL)
		prevlink = &(sap->data);
	else
	{
		switch (the_type)
		{
			case 1:   /* feature table */
				for (sfp = (SeqFeatPtr)(sap->data); sfp->next != NULL; sfp = sfp->next)
					continue;
				prevlink = (Pointer PNTR)&(sfp->next);
				break;
			case 2:   /* alignments */
				for (salp = (SeqAlignPtr)(sap->data); salp->next != NULL; salp = salp->next)
					continue;
				prevlink = (Pointer PNTR)&(salp->next);
				break;
			case 3:   /* Graph */
				for (sgp = (SeqGraphPtr)(sap->data); sgp->next != NULL; sgp = sgp->next)
					continue;
				prevlink = (Pointer PNTR)&(sgp->next);
				break;
		}
	}
	if (prevlink != NULL)
		*prevlink = addptr;

	return TRUE;
}

static void AddToAnnot(SeqAnnotPtr PNTR head, Int2 the_type, Pointer addptr)
{
	SeqAnnotPtr sap, prev=NULL;

	if (head == NULL) return;

	for (sap = *head; sap != NULL; sap = sap->next)
	{
		if (sap->type == the_type)
			break;
		prev = sap;
	}

	if (sap == NULL)
	{
		sap = SeqAnnotNew();
		sap->type = (Uint1)the_type;
		if (prev != NULL)
			prev->next = sap;
		else
			*head = sap;
	}

	AddToSeqAnnot(sap, the_type, addptr);

	return;
}

static ValNodePtr PubFromDescr(ValNodePtr desc)
{
	ValNodePtr vnp2, vnp;
	PubdescPtr pdp;

	if (desc->choice != Seq_descr_pub)
		return NULL;

	pdp = (PubdescPtr)(desc->data.ptrvalue);
	vnp = pdp->pub;
	pdp->pub = NULL;

	SeqDescFree(desc);

	if (vnp == NULL) return vnp;

	if (vnp->next != NULL)
	{
		vnp2 = vnp;
		vnp = ValNodeNew(NULL);
		vnp->choice = PUB_Equiv;
		vnp->data.ptrvalue = vnp2;
	}

	return vnp;
}

static ValNodePtr DescrFromPub(ValNodePtr pub)
{
	ValNodePtr vnp;
	PubdescPtr pdp;

	pdp = PubdescNew();
	if (pub->choice == PUB_Equiv)
	{
		vnp = (ValNodePtr)(pub->data.ptrvalue);
		MemFree(pub);
		pub = vnp;
	}
	pdp->pub = pub;
	vnp = ValNodeNew(NULL);
	vnp->choice = Seq_descr_pub;
	vnp->data.ptrvalue = (Pointer)pdp;
	return vnp;
}

static Boolean AttachDataProc (GatherContextPtr gcp)
{
	OMProcControlPtr ompcp;
	Pointer ptr, newptr;
	Uint2 into, newtype;
	Boolean no_good = FALSE, into_seqentry = FALSE;
	ValNodePtr vnp;
	ObjMgrDataPtr omdp=NULL;
	ObjMgrPtr omp;
	BioseqPtr bsp;
	BioseqSetPtr bssp;
	SeqAnnotPtr sap;
	SeqFeatPtr sfp;
	SeqAlignPtr salp;
	SeqGraphPtr sgp;
	SeqSubmitPtr ssp;
	SubmitBlockPtr sbp;
	GDSPtr gdsp;

	gdsp = (GDSPtr)(gcp->userdata);
	ompcp = gdsp->ompcp;

    ptr = gcp->thisitem;   /* insert before or into this */
    ompcp->input_data = ptr;

    into = gcp->thistype;
	newtype = ompcp->output_itemtype;
	newptr = ompcp->output_data;

	omp = ObjMgrWriteLock();
	omdp = ObjMgrFindByData(omp, newptr);

	switch (into)
	{
		case OBJ_BIOSEQSET:
		case OBJ_BIOSEQ:
			if (gcp->sep != NULL)
			{
				ompcp->input_choice = gcp->sep;
				ompcp->input_choicetype = OBJ_SEQENTRY;
				into_seqentry = TRUE;
			}
			break;
		default:
			break;
	}

	switch (into)
	{
		case OBJ_SEQSUB:
			ssp = (SeqSubmitPtr)(ptr);
			switch (newtype)
			{
				case OBJ_BIOSEQSET:
				case OBJ_BIOSEQ:
					if (omdp == NULL)
					{
						ErrPostEx(SEV_ERROR,0,0,"AttachDataProc: Not entity for Bioseq(Set)");
						return TRUE;
					}
					if (ssp->datatype == 0)
						ssp->datatype = 1;
					if (ssp->datatype != 1)
					{
						no_good = TRUE;
						break;
					}
					if (omdp->choice == NULL)
					{
						omdp->choice = ValNodeNew(NULL);
						omdp->choicetype = OBJ_SEQENTRY;
						omdp->choice->data.ptrvalue = newptr;
						if (newtype == OBJ_BIOSEQ)
							omdp->choice->choice = 1;
						else
							omdp->choice->choice = 2;
					}
					ValNodeLink((ValNodePtr PNTR)&(ssp->data), omdp->choice);
					ObjMgrConnectFunc(omp, newtype, newptr, into, ptr);
					break;
				case OBJ_PUB:
					vnp = (ValNodePtr)newptr;
					no_good = TRUE;
					if (vnp->choice == PUB_Sub)
					{
						if (ssp->sub == NULL)
							ssp->sub = SubmitBlockNew();
						if (ssp->sub->cit == NULL)
						{
							ssp->sub->cit = (CitSubPtr)(vnp->data.ptrvalue);
							ValNodeFree(vnp);
							no_good = FALSE;
						}
					}
					break;
				case OBJ_SEQENTRY:
					if (ssp->datatype == 0)
						ssp->datatype = 1;
					if (ssp->datatype != 1)
					{
						no_good = TRUE;
						break;
					}
					ValNodeLink((ValNodePtr PNTR)&(ssp->data), omdp->choice);
					ObjMgrConnectFunc(omp, newtype, newptr, into, ptr);
					break;
				case OBJ_SEQANNOT:
					if (ssp->datatype == 0)
						ssp->datatype = 2;
					if (ssp->datatype != 2)
					{
						no_good = TRUE;
						break;
					}
					AddAnAnnot((SeqAnnotPtr PNTR)&(ssp->data), newptr);
					break;
				case OBJ_SEQFEAT:
					if (ssp->datatype == 0)
						ssp->datatype = 2;
					if (ssp->datatype != 2)
					{
						no_good = TRUE;
						break;
					}
					AddToAnnot((SeqAnnotPtr PNTR)&(ssp->data), 1, newptr);
					break;
				case OBJ_SEQALIGN:
					if (ssp->datatype == 0)
						ssp->datatype = 2;
					if (ssp->datatype != 2)
					{
						no_good = TRUE;
						break;
					}
					AddToAnnot((SeqAnnotPtr PNTR)&(ssp->data), 2, newptr);
					break;
				case OBJ_SEQGRAPH:
					if (ssp->datatype == 0)
						ssp->datatype = 2;
					if (ssp->datatype != 2)
					{
						no_good = TRUE;
						break;
					}
					AddToAnnot((SeqAnnotPtr PNTR)&(ssp->data), 3, newptr);
					break;
				case OBJ_SUBMIT_BLOCK:
					if (ssp->sub == NULL)
						ssp->sub = (SubmitBlockPtr)newptr;
					else
						no_good = TRUE;
					break;
				case OBJ_SEQSUB_CONTACT:
					if (ssp->sub == NULL)
						ssp->sub = SubmitBlockNew();
					if (ssp->sub->contact == NULL)
						ssp->sub->contact = (ContactInfoPtr)newptr;
					else
						no_good = TRUE;
					break;
				default:
					no_good = TRUE;
					break;
			}
			break;
		case OBJ_SUBMIT_BLOCK:
			sbp = (SubmitBlockPtr)(ptr);
			switch (newtype)
			{
				case OBJ_PUB:
					vnp = (ValNodePtr)newptr;
					no_good = TRUE;
					if (vnp->choice == PUB_Sub)
					{
						if (sbp->cit == NULL)
						{
							sbp->cit = (CitSubPtr)(vnp->data.ptrvalue);
							ValNodeFree(vnp);
							no_good = FALSE;
						}
					}
					break;
				case OBJ_SEQSUB_CONTACT:
					if (sbp->contact == NULL)
						sbp->contact = (ContactInfoPtr)newptr;
					else
						no_good = TRUE;
					break;
				default:
					no_good = TRUE;
					break;
			}
			break;
		case OBJ_BIOSEQSET:
			bssp = (BioseqSetPtr)(ptr);
			switch (newtype)
			{
				case OBJ_BIOSEQSET:
				case OBJ_BIOSEQ:
					if (omdp == NULL)
					{
						ErrPostEx(SEV_ERROR,0,0,"AttachDataProc: Not entity for Bioseq(Set)");
						return TRUE;
					}
					if (omdp->choice == NULL)
					{
						omdp->choice = ValNodeNew(NULL);
						omdp->choicetype = OBJ_SEQENTRY;
						omdp->choice->data.ptrvalue = newptr;
						if (newtype == OBJ_BIOSEQ)
							omdp->choice->choice = 1;
						else
							omdp->choice->choice = 2;
					}
					ValNodeLink(&(bssp->seq_set), omdp->choice);
					ObjMgrConnectFunc(omp, newtype, newptr, into, ptr);
					break;
				case OBJ_SEQDESC:
					ValNodeLink(&(bssp->descr), (ValNodePtr)newptr);
					break;
				case OBJ_PUB:			   /* make a pubdesc */
				case OBJ_SEQFEAT_CIT:
					vnp = DescrFromPub((ValNodePtr)newptr);
					ValNodeLink(&(bssp->descr), vnp);
					break;
				case OBJ_SEQENTRY:
					ValNodeLink(&(bssp->seq_set), newptr);
					break;
				case OBJ_SEQANNOT:
					AddAnAnnot(&(bssp->annot), newptr);
					break;
				case OBJ_SEQFEAT:
					AddToAnnot(&(bssp->annot), 1, newptr);
					break;
				case OBJ_SEQALIGN:
				case OBJ_SEQHIST_ALIGN:
					AddToAnnot(&(bssp->annot), 2, newptr);
					break;
				case OBJ_SEQGRAPH:
					AddToAnnot(&(bssp->annot), 3, newptr);
					break;
				default:
					no_good = TRUE;
					break;
			}
			break;
		case OBJ_BIOSEQ:
			bsp = (BioseqPtr)(ptr);
			switch (newtype)
			{
				case OBJ_BIOSEQSET:
				case OBJ_BIOSEQ:
					if (omdp == NULL)
					{
						ErrPostEx(SEV_ERROR,0,0,"AttachDataProc: Not entity for Bioseq(Set)");
						return TRUE;
					}
					if (omdp->choice == NULL)
					{
						omdp->choice = ValNodeNew(NULL);
						omdp->choicetype = OBJ_SEQENTRY;
						omdp->choice->data.ptrvalue = newptr;
						if (newtype == OBJ_BIOSEQ)
							omdp->choice->choice = 1;
						else
							omdp->choice->choice = 2;
					}
					if ((gcp->parentitem != NULL) && (gcp->prevlink != NULL))
					{
						omdp->choice->next = *(gcp->prevlink);
						*(gcp->prevlink) = omdp->choice;
						ObjMgrConnectFunc(omp, newtype, newptr, gcp->parenttype, gcp->parentitem);
					}
					else
						no_good = TRUE;
					break;
				case OBJ_SEQDESC:
					ValNodeLink(&(bsp->descr), (ValNodePtr)newptr);
					break;
				case OBJ_PUB:			   /* make a pubdesc */
				case OBJ_SEQFEAT_CIT:
					vnp = DescrFromPub((ValNodePtr)newptr);
					ValNodeLink(&(bsp->descr), vnp);
					break;
				case OBJ_SEQENTRY:
					if ((gcp->parentitem != NULL) && (gcp->prevlink != NULL))
					{
						omdp->choice->next = *(gcp->prevlink);
						*(gcp->prevlink) = omdp->choice;
						ObjMgrConnectFunc(omp, newtype, newptr, gcp->parenttype, gcp->parentitem);
					}
					else
						no_good = TRUE;
					break;
				case OBJ_SEQANNOT:
					AddAnAnnot(&(bsp->annot), newptr);
					break;
				case OBJ_SEQFEAT:
					AddToAnnot(&(bsp->annot), 1, newptr);
					break;
				case OBJ_SEQALIGN:
				case OBJ_SEQHIST_ALIGN:
					AddToAnnot(&(bsp->annot), 2, newptr);
					break;
				case OBJ_SEQGRAPH:
					AddToAnnot(&(bsp->annot), 3, newptr);
					break;
				case OBJ_SEQHIST:
					if (bsp->hist == NULL)
					{
						bsp->hist = (SeqHistPtr)newptr;
						break;
					}
				default:
					no_good = TRUE;
					break;
			}
			break;
		case OBJ_SEQDESC:
			vnp = (ValNodePtr)newptr;
			switch (newtype)
			{
				case OBJ_PUB:
					vnp = DescrFromPub(vnp);
				case OBJ_SEQDESC:
					if (gcp->prevlink != NULL)
					{
						vnp->next = *(gcp->prevlink);
						*(gcp->prevlink) = vnp;
					}
					else
						no_good = TRUE;
					break;
				default:
					no_good = TRUE;
					break;
			}
			break;
		case OBJ_SEQFEAT_CIT:
		case OBJ_PUB_SET:
			vnp = (ValNodePtr)newptr;
			switch (newtype)
			{
				case OBJ_SEQDESC:
					vnp = PubFromDescr(vnp);
					if (vnp == NULL)
					{
						no_good = TRUE;
						break;
					}
				case OBJ_PUB:
					if (gcp->prevlink != NULL)
					{
						vnp->next = *(gcp->prevlink);
						*(gcp->prevlink) = vnp;
					}
					else
						no_good = TRUE;
					break;
				default:
					no_good = TRUE;
					break;
			}
			break;
		case OBJ_SEQANNOT:
			sap = (SeqAnnotPtr)ptr;
			switch (newtype)
			{
				case OBJ_SEQFEAT:
					if (! AddToSeqAnnot(sap, 1, newptr))
						no_good = TRUE;
					break;
				case OBJ_SEQALIGN:
				case OBJ_SEQHIST_ALIGN:
					if (! AddToSeqAnnot(sap, 2, newptr))
						no_good = TRUE;
					break;
				case OBJ_SEQGRAPH:
					if (! AddToSeqAnnot(sap, 3, newptr))
						no_good = TRUE;
					break;
				case OBJ_SEQANNOT:
					sap = (SeqAnnotPtr)newptr;
					if (gcp->prevlink != NULL)
					{
						sap->next = *(gcp->prevlink);
						*(gcp->prevlink) = sap;
					}
					else
						no_good = TRUE;
					break;
				default:
					no_good = TRUE;
					break;
			}
			break;
		case OBJ_BIOSEQ_MAPFEAT:
		case OBJ_SEQFEAT:
			sfp = (SeqFeatPtr)ptr;
			switch (newtype)
			{
				case OBJ_SEQDESC:
					newptr = (Pointer)PubFromDescr((ValNodePtr)newptr);
				case OBJ_PUB:
					if (sfp->cit == NULL)
					{
						sfp->cit = ValNodeNew(NULL);
						sfp->cit->choice = 1;
					}
					ValNodeLink((ValNodePtr PNTR)&(sfp->cit->data.ptrvalue), (ValNodePtr)newptr);
					break;
				case OBJ_SEQFEAT:
					sfp = (SeqFeatPtr)newptr;
					if (gcp->prevlink != NULL)
					{
						sfp->next = *(gcp->prevlink);
						*(gcp->prevlink) = sfp;
					}
					else
						no_good = TRUE;
					break;
				case OBJ_SEQLOC:
					SeqLocFree(sfp->location); /* make assumption */
					sfp->location = (ValNodePtr)newptr;
					break;
				default:
					no_good = TRUE;
					break;
			}
			break;
		case OBJ_SEQALIGN:
		case OBJ_SEQHIST_ALIGN:
		case OBJ_SEQHIST:
			switch (newtype)
			{
				case OBJ_SEQALIGN:
				case OBJ_SEQHIST_ALIGN:
					salp = (SeqAlignPtr)newptr;
					if (gcp->prevlink != NULL)
					{
						salp->next = *(gcp->prevlink);
						*(gcp->prevlink) = salp;
					}
					else
						no_good = TRUE;
					break;
				default:
					no_good = TRUE;
					break;
			}
			break;
		case OBJ_SEQGRAPH:
			switch (newtype)
			{
				case OBJ_SEQGRAPH:
					sgp = (SeqGraphPtr)newptr;
					if (gcp->prevlink != NULL)
					{
						sgp->next = *(gcp->prevlink);
						*(gcp->prevlink) = sgp;
					}
					else
						no_good = TRUE;
					break;
				default:
					no_good = TRUE;
					break;
			}
			break;
		case OBJ_SEQLOC:
		case OBJ_SEQID:
		case OBJ_SEQENTRY:
		default:
			no_good = TRUE;
			break;
	}
	
	ObjMgrUnlock();

	if (no_good)
	{
		ErrPostEx(SEV_ERROR,0,0,"AttachDataProc: [%d] into [%d]", (int)newtype, (int)into);
	}
	else
	{
		gdsp->succeeded = TRUE;
		switch (newtype)
		{
			case OBJ_BIOSEQ:
			case OBJ_BIOSEQSET:
				break;
			default:
				if (omdp != NULL)
					ObjMgrDelete(omdp->datatype, omdp->dataptr);
		}
	}

	return TRUE;
}

static Boolean CopyDataProc (GatherContextPtr gcp)
{
	OMProcControlPtr ompcp;
	ObjMgrTypePtr omtp;
	ObjMgrPtr omp;
	Uint2 type;
	Pointer ptr, ptr2;
	Boolean was_choice = FALSE;
	GDSPtr gdsp;

	gdsp = (GDSPtr)(gcp->userdata);
	ompcp = gdsp->ompcp;

    ptr = gcp->thisitem;
	type = gcp->thistype;
    ompcp->input_data = ptr;
    
	switch (gcp->thistype)
	{
		case OBJ_BIOSEQSET:
		case OBJ_BIOSEQ:
			if (gcp->sep != NULL)
			{
				ompcp->input_choice = gcp->sep;
				ptr = (Pointer)(gcp->sep);
				ompcp->input_choicetype = OBJ_SEQENTRY;
				type = OBJ_SEQENTRY;
				was_choice = TRUE;
			}
			break;
		default:
			break;
	}

	omp = ObjMgrReadLock();
	omtp = ObjMgrTypeFind(omp, type, NULL, NULL);
	ObjMgrUnlock();

	if (omtp == NULL)
	{
		ErrPostEx(SEV_ERROR,0,0,"CopyDataProc: can't find type [%d]", (int)type);
		return TRUE;
	}

	ptr2 = AsnIoMemCopy(ptr, omtp->asnread, omtp->asnwrite);
	if (ptr2 != NULL)
	{
		gdsp->succeeded = TRUE;

		if (! was_choice)
		{
			ompcp->output_data = ptr2;
			ompcp->output_itemtype = type;
		}
		else
		{
			ompcp->output_choice = ptr2;
			ompcp->output_choicetype = type;
			ompcp->output_data = ((ValNodePtr)(ptr2))->data.ptrvalue;
			ompcp->output_itemtype = gcp->thistype;
		}
		switch (ompcp->output_itemtype)
		{
			case OBJ_BIOSEQSET:    /* these types Add to the ObjMgr themselves */
			case OBJ_BIOSEQ:
			case OBJ_SEQSUB:
				break;
			default:
				ObjMgrAdd(ompcp->output_itemtype, ompcp->output_data);
				break;
		}
	}
	return TRUE;
}

static Boolean NEAR GenericGatherDataForProc (OMProcControlPtr ompcp, Boolean sel, Int2 func)
{
	ObjMgrDataPtr omdp;
	SelStructPtr ssp;
	Boolean retval = FALSE, data_changed = FALSE;
	static CharPtr funcs [5] = {
		"GatherDataForProc",
		"DetachDataForProc",
		"AttachDataForProc",
		"CopyDataForProc",
		"ReplaceDataForProc" };
	GatherItemProc gip;
	GDS gds;
	ObjMgrPtr omp;

	gds.succeeded = retval;
	gds.ompcp = ompcp;

	if (sel)
	{
		ssp = ObjMgrGetSelected();
		if (ssp == NULL)
		{
			/*
			ErrPostEx(SEV_ERROR,0,0,"%s: Nothing was selected", funcs[func]);
			*/
			return retval;
		}
		ompcp->input_entityID = ssp->entityID;
		ompcp->input_itemID = ssp->itemID;
		ompcp->input_itemtype = ssp->itemtype;
	}

	if (func == 1) /* detach: changes selection */
		ObjMgrDeSelect(ompcp->input_entityID, ompcp->input_itemID, ompcp->input_itemtype, 0, NULL);

	omp = ObjMgrReadLock();
	omdp = ObjMgrGetDataStruct (omp, ompcp->input_entityID);

	if (omdp == NULL)
	{
		ObjMgrUnlock();
		ErrPostEx(SEV_ERROR,0,0,"%s: can't match entityID", funcs[func]);
	 	return retval;
	}
	
	if ((ompcp->input_itemID <= 1) &&
	    ((ompcp->input_itemtype == 0) || (ompcp->input_itemtype == omdp->datatype)))  /* top level */
	{
		ompcp->input_choice = omdp->choice;
		ompcp->input_choicetype = omdp->choicetype;
		ompcp->input_itemtype = omdp->datatype;
		ompcp->input_data = omdp->dataptr;
		ompcp->whole_entity = TRUE;
		if ((func == 0) || (func == 1))  /* gather or detach */
		{
			ObjMgrUnlock();
			return TRUE;
		}
	}

	ObjMgrUnlock();

	if (func == 4)    /* replace */
	{
		if (ompcp->input_itemtype != ompcp->output_itemtype)
		{
			ErrPostEx(SEV_ERROR,0,0,"%s: input type %d != output type %d",
				funcs[func], (int)(ompcp->input_itemtype), (int)(ompcp->output_itemtype));
			return FALSE;
		}

		if (ompcp->output_data == NULL)
		{
			ErrPostEx(SEV_ERROR,0,0,"%s: no output_data", funcs[func]);
			return FALSE;
		}
	}

	switch(func)
	{
		case 0:
			gip = GatherDataProc;
			break;
		case 1:
			gip = DetachDataProc;
			data_changed = TRUE;
			break;
		case 2:
			gip = AttachDataProc;
			data_changed = TRUE;
			break;
		case 3:
			gip = CopyDataProc;
			break;
		case 4:
			gip = ReplaceDataProc;
			data_changed = TRUE;
			break;

	}

	retval = GatherItemFunc (ompcp->input_entityID, ompcp->input_itemID,
                  ompcp->input_itemtype,(Pointer)(&gds), gip, NULL, ompcp->do_not_reload_from_cache);

	if (! retval)
		ErrPostEx(SEV_ERROR,0,0,"%s: can't do the gather", funcs[func]);
	else
		retval = gds.succeeded;

	if ((retval) && (data_changed))   /* set the dirty flag */
	{
		ObjMgrSetDirtyFlag(ompcp->input_entityID, TRUE);
	}
	return retval;
}

/****************************************************************************
*
*   GatherDataForProc(ompcp, sel)
*
*       fills in data, choice, and choictype in OMProcControlPtr
*         sets ompcp->whole_entity TRUE if appropriate
*       returns TRUE if it did it
*       if (sel == TRUE), fills in ompcp with data from ObjMgrGetSelect first.
*          returns FALSE if nothing selected.. Does ErrPostEx() for it
*
****************************************************************************/
NLM_EXTERN Boolean LIBCALL GatherDataForProc (OMProcControlPtr ompcp, Boolean sel)
{
	return GenericGatherDataForProc(ompcp, sel, 0);
}

/****************************************************************************
*
*   DetachDataForProc(ompcp, sel)
*
*       fills in data, choice, and choictype in OMProcControlPtr
*         sets ompcp->whole_entity TRUE if appropriate
*       returns TRUE if it did it
*       if (sel == TRUE), fills in ompcp with data from ObjMgrGetSelect first.
*          returns FALSE if nothing selected.. Does ErrPostEx() for it
*       Detaches data item from surrounding data if necessary
*
****************************************************************************/
NLM_EXTERN Boolean LIBCALL DetachDataForProc (OMProcControlPtr ompcp, Boolean sel)
{
	return GenericGatherDataForProc(ompcp, sel, 1);
}

/****************************************************************************
*
*   AttachDataForProc(ompcp, sel)
*
*       fills in data, choice, and choictype in OMProcControlPtr
*         sets ompcp->whole_entity TRUE if appropriate
*       returns TRUE if it did it
*       if (sel == TRUE), fills in ompcp with data from ObjMgrGetSelect first.
*          returns FALSE if nothing selected.. Does ErrPostEx() for it
*       Attaches data in output section of ompcp into the input section
*
****************************************************************************/
NLM_EXTERN Boolean LIBCALL AttachDataForProc (OMProcControlPtr ompcp, Boolean sel)
{
	return GenericGatherDataForProc(ompcp, sel, 2);
}

/****************************************************************************
*
*   CopyDataForProc(ompcp, sel)
*
*       fills in data, choice, and choictype in OMProcControlPtr
*         sets ompcp->whole_entity TRUE if appropriate
*       returns TRUE if it did it
*       if (sel == TRUE), fills in ompcp with data from ObjMgrGetSelect first.
*          returns FALSE if nothing selected.. Does ErrPostEx() for it
*       Attaches copy of data in output section of ompcp
*
****************************************************************************/
NLM_EXTERN Boolean LIBCALL CopyDataForProc (OMProcControlPtr ompcp, Boolean sel)
{
	return GenericGatherDataForProc(ompcp, sel, 3);
}

/****************************************************************************
*
*   ReplaceDataForProc(ompcp, sel)
*
*       fills in data, choice, and choictype in OMProcControlPtr
*         sets ompcp->whole_entity TRUE if appropriate
*       returns TRUE if it did it
*       if (sel == TRUE), fills in ompcp with data from ObjMgrGetSelect first.
*          returns FALSE if nothing selected.. Does ErrPostEx() for it
*       Replaces data in input section of ompcp with the output section
*       Data in input section is deleted
*
****************************************************************************/
NLM_EXTERN Boolean LIBCALL ReplaceDataForProc (OMProcControlPtr ompcp, Boolean sel)
{
	return GenericGatherDataForProc(ompcp, sel, 4);
}

/*****************************************************************************
*
*   GatherProcLaunch(proctype, sel, entityID, itemID, itemtype,
*        inputtype, subinputtype, outputtype, suboutputtype)
*   	looks for ALL proctype in priority order that
*   		matches inputtype and outputtype
*     if (sel) then fills in entityID,itemID,itemtype with currently selected
*        item
*     else
*        uses the function arguments
*     locates the data pointer, determines the subtype (if any)
*     then finds the process that will return OM_MSG_RET_DONE in priority order
*   	0 on outputtype, inputsubtype, outputsubtype matches any
*     if subtype can be matched on input/output, takes in preference over
*        more general proc
*
*     USAGE:
*     1) To launch an editor for the currently selected item
*      GatherProcLaunch(OMPROC_EDIT,TRUE,0,0,0,0,0,0,0);
*     2) To launch an editor to create a new seq_descr of type pub
*      GatherProcLaunch(OMPROC_EDIT,FALSE,0,0,0,OBJ_SEQDESC,Seq_descr_pub,0,0);
*     3) To launch an editor for a specific seq_descr
*      GatherProcLaunch(OMPROC_EDIT,FALSE,2,1,4,0,0,0,0);
*        (where the 3 numbers identify the seq_descr)
*     4)To launch an editor which will create a new seq_descr and attach it
*        to the currently selected Bioseq
*      GatherProcLaunch(OMPROC_EDIT,TRUE,0,0,0,OBJ_SEQDESC,Seq_descr_pub,0,0)
*        Note in this case ompcp->input_entityid, .input_itemid, input_itemtype
*          well refer to a Bioseq. The editor should check the input_itemtype
*          and decide if it can attach it's output to it, or if it is an
*          input type mismatch error.
*     5) To launch an editor which will create a new seq_descr and attach to
*        a specific Bioseq
*         (Same as (4) but sel=FALSE, and entitid,itemid,itemtype filled in
*          for the Bioseq).
*
*     GENERAL RULES:
*
*     All this means the function will be called with OMProcControlPtr (ompcp)
*      fields filled in (input_entityid, input_itemid, input_itemtype) as:
*       1) Create new one, place in desktop = 0,0,0
*       2) Edit old one, non-zero values, with input_itemtype matching the type
*          of the editor.
*       3) Create a new one, attach it to something else, non-zero values,
*          with input_itemtype not matching the type of the editor.
*
*     Functions to install the returned values are for the cases above:
*       1) ObjMgrRegister()
*       2) GatherReplaceDataForProc()
*       3) GatherAttachDataForProc()
*
*     returns the return from the proc, or OM_MSG_RET_NOPROC if not found
*
*****************************************************************************/
NLM_EXTERN Int2 GatherProcLaunch (Uint2 proctype, Boolean sel, Uint2 entityID, Uint2 itemID,
                    Uint2 itemtype, Uint2 inputtype, Uint2 subinputtype, Uint2 outputtype, Uint2 suboutputtype)
{
	ObjMgrPtr omp;
	OMProcControl ompc;
	ObjMgrProcPtr ompp=NULL;
	ObjMgrTypePtr omtp;
	Boolean retval, do_general_proc = FALSE;
	Uint2 subtype = 0;
	Int2 procval = OM_MSG_RET_NOPROC;

	MemSet((Pointer)(&ompc), 0, sizeof(OMProcControl));

	ompc.input_entityID = entityID;
	ompc.input_itemID = itemID;
	ompc.input_itemtype = itemtype;

	retval = GatherDataForProc(&ompc, sel);

	if (sel && (! retval))
		return OM_MSG_RET_ERROR;

	if (entityID && (! retval))
		return OM_MSG_RET_ERROR;

	if (! inputtype)   /* not set on input */
		inputtype = ompc.input_itemtype;   /* could now be filled in */

	omp = NULL;  /* do all temporary read locks */
	if ((! subinputtype) && (inputtype == ompc.input_itemtype)
		               && (ompc.input_data != NULL))
	{
		omtp = ObjMgrTypeFind(omp, inputtype, NULL, NULL);
		if (omtp != NULL)
			subinputtype = (*(omtp->subtypefunc))(ompc.input_data);
	}

	while ((ompp = ObjMgrProcFindNext(omp, proctype, inputtype, outputtype, ompp)) != NULL)
	{
		if (ompp->subinputtype == subinputtype)
		{
			ompc.proc = ompp;
			procval = (*(ompp->func))((Pointer)&ompc);
			switch (procval)
			{
				case OM_MSG_RET_ERROR:
					ErrShow();
					break;
				case OM_MSG_RET_DEL:
					break;
				case OM_MSG_RET_OK:
					break;
				case OM_MSG_RET_DONE:
					goto all_done;
				default:
					break;
			}
		}
		else if (! ompp->subinputtype)  /* general proc found */
			do_general_proc = TRUE;
	}

	if (do_general_proc)    /* specific proc failed, try a general one */
	{
		while ((ompp = ObjMgrProcFindNext(omp, proctype, inputtype, outputtype, ompp)) != NULL)
		{
			if (! ompp->subinputtype)
			{
				ompc.proc = ompp;
				procval = (*(ompp->func))((Pointer)&ompc);
				switch (procval)
				{
					case OM_MSG_RET_ERROR:
						ErrShow();
						break;
					case OM_MSG_RET_DEL:
						break;
					case OM_MSG_RET_OK:
						break;
					case OM_MSG_RET_DONE:
						goto all_done;
					default:
						break;
				}
			}
		}
	}

all_done:
	return procval;

}

NLM_EXTERN Int2 GatherSpecificProcLaunch (Uint2 procid, CharPtr procname, Uint2 proctype,
                                          Boolean sel, Uint2 entityID, Uint2 itemID, Uint2 itemtype)
{
	ObjMgrPtr omp;
	OMProcControl ompc;
	ObjMgrProcPtr ompp=NULL;
	Boolean retval;
	Int2 procval = OM_MSG_RET_NOPROC;

	omp = ObjMgrGet ();

	MemSet((Pointer)(&ompc), 0, sizeof(OMProcControl));

	ompc.input_entityID = entityID;
	ompc.input_itemID = itemID;
	ompc.input_itemtype = itemtype;

	retval = GatherDataForProc(&ompc, sel);

	if (sel && (! retval))
		return OM_MSG_RET_ERROR;

	if (entityID && (! retval))
		return OM_MSG_RET_ERROR;

	ompp = ObjMgrProcFind (omp, procid, procname, proctype);
	if (ompp == NULL)
		return OM_MSG_RET_ERROR;

	ompc.proc = ompp;
	procval = (*(ompp->func))((Pointer)&ompc);
	switch (procval)
	{
		case OM_MSG_RET_ERROR:
			ErrShow();
			break;
		case OM_MSG_RET_DEL:
			break;
		case OM_MSG_RET_OK:
			break;
		case OM_MSG_RET_DONE:
			goto all_done;
		default:
			break;
	}

all_done:
	return procval;

}
/*****************************************************************
*
*  GatherOverWrite (oldptr, newptr, type)
*      type is OBJ_...
*      overwrites oldptr with contents of newptr
*      sets any "next" pointers contained in newptr to point to the same
*        chain as those in oldptr did
*      this function is used for making a varient copy of an object, then
*        replacing it in another object without changing points to or from
*        this object
*******************************************************************/
NLM_EXTERN Boolean LIBCALL GatherOverWrite (Pointer oldptr, Pointer newptr, Uint2 type)
{
    size_t size = 0;
	Pointer dest, next=NULL, oldsrc;
	ValNodePtr vnp;

	if ((oldptr == NULL) || (newptr == NULL))
		return FALSE;

	oldsrc = oldptr;
	switch (type)
	{
		case OBJ_SEQSUB:
			size = sizeof(SeqSubmit);
			break;
		case OBJ_SUBMIT_BLOCK:
			size = sizeof(SubmitBlock);
			break;
		case OBJ_SEQSUB_CONTACT:
			size = sizeof(ContactInfo);
			break;
		case OBJ_SEQSUB_CIT:
			size = sizeof(CitSub);
			break;
		case OBJ_SEQHIST:
			size = sizeof(SeqHist);
			break;
		case OBJ_BIOSEQSET:
			size = sizeof(BioseqSet);
			break;
		case OBJ_BIOSEQ:
			size = sizeof(Bioseq);
			break;
		case OBJ_SEQDESC:
		case OBJ_BIOSEQ_SEG:
		case OBJ_PUB:
		case OBJ_SEQFEAT_CIT:
		case OBJ_PUB_SET:
		case OBJ_SEQLOC:
		case OBJ_SEQID:
		case OBJ_SEQENTRY:
			vnp = (ValNodePtr)oldptr;
			oldsrc = &(vnp->data);
			size = sizeof(DataVal);
			vnp = (ValNodePtr)newptr;
			newptr = &(vnp->data);
			break;
		case OBJ_BIOSEQ_MAPFEAT:
		case OBJ_SEQFEAT:
			size = sizeof(SeqFeat);
			next = (Pointer)(((SeqFeatPtr)(oldptr))->next);
			(((SeqFeatPtr)(oldptr))->next) = NULL;
			(((SeqFeatPtr)(newptr))->next) = (SeqFeatPtr)next;
			break;
		case OBJ_SEQANNOT:
			size = sizeof(SeqAnnot);
			next = (Pointer)(((SeqAnnotPtr)(oldptr))->next);
			(((SeqAnnotPtr)(oldptr))->next) = NULL;
			(((SeqAnnotPtr)(newptr))->next) = (SeqAnnotPtr)next;
			break;
		case OBJ_SEQALIGN:
		case OBJ_SEQHIST_ALIGN:
			size = sizeof(SeqAlign);
			next = (Pointer)(((SeqAlignPtr)(oldptr))->next);
			(((SeqAlignPtr)(oldptr))->next) = NULL;
			(((SeqAlignPtr)(newptr))->next) = (SeqAlignPtr)next;
			break;
		case OBJ_SEQGRAPH:
			size = sizeof(SeqGraph);
			next = (Pointer)(((SeqGraphPtr)(oldptr))->next);
			(((SeqGraphPtr)(oldptr))->next) = NULL;
			(((SeqGraphPtr)(newptr))->next) = (SeqGraphPtr)next;
			break;
		default:
			ErrPostEx(SEV_ERROR,0,0,"ObjMgrOverWrite: unsupported type %d",
				(int)(type));
			return FALSE;
	}

	if (! size)
		return FALSE;

	dest = MemNew(size);   /* temporary buffer for copies */
	if (dest == NULL)
	{
		ErrPostEx(SEV_ERROR,0,0,"ObjMgrOverWrite: can't allocate buffer");
		return FALSE;
	}
    
	MemCopy(dest, oldsrc, size);    /* replace the contents */
	MemCopy(oldsrc, newptr, size);
	MemCopy(newptr, dest, size);

	MemFree(dest);

	return TRUE;
}



/*****************************************************************************/

/* AssignIDsInEntity/VisitObjectsInEntity section */

typedef struct internalacc {
  Uint2             entityID;
  Uint4             itemIDs [OBJ_MAX];
  Boolean           assignIDs;
  GatherObjectProc  callback;
  Pointer           userdata;
  BoolPtr           objMgrFilter;
} InternalACC, PNTR InternalACCPtr;

static void AssignIDs (InternalACCPtr iap, GatherIndexPtr gip, Uint1 itemtype, Uint1 subtype, Pointer parent, Uint2 parenttype, Pointer PNTR prevlink)

{
  if (iap == NULL || gip == NULL) return;

  if (iap->assignIDs) {
    gip->entityID = iap->entityID;
    gip->itemID = iap->itemIDs [itemtype];
    gip->itemtype = itemtype;
    gip->subtype = subtype;
    /* gip->deleteme = 0; */
    gip->parenttype = parenttype;
    gip->parentptr = parent;
    gip->prevlink = prevlink;
  }
}

static Boolean VisitCallback (InternalACCPtr iap, Pointer dataptr, Uint1 itemtype, Uint1 subtype, Pointer parent, Uint2 parenttype, Pointer PNTR prevlink)

{
  GatherObject  go;

  if (dataptr == NULL || iap == NULL || itemtype >= OBJ_MAX) return TRUE;

  if (iap->callback != NULL) {
    if (iap->objMgrFilter == NULL || iap->objMgrFilter [itemtype]) {
      go.entityID = iap->entityID;
      go.itemID = iap->itemIDs [itemtype];
      go.itemtype = itemtype;
      go.subtype = subtype;
      go.parenttype = parenttype;
      go.dataptr = dataptr;
      go.parentptr = parent;
      go.prevlink = prevlink;
      go.userdata = iap->userdata;
      if (! iap->callback (&go)) return FALSE;
    }
  }

  return TRUE;
}

static Boolean VisitSeqEntry (InternalACCPtr iap, SeqEntryPtr sep, Pointer parent, Uint2 parenttype, Pointer PNTR prevlink);

static Boolean VisitPub (InternalACCPtr iap, ValNodePtr pub, Uint1 itemtype, Pointer parent, Uint2 parenttype, Pointer PNTR prevlink)

{
  if (iap == NULL || pub == NULL) return TRUE;
  (iap->itemIDs [itemtype])++;

  if (iap->callback != NULL) {
    if (! VisitCallback (iap, (Pointer) pub, itemtype, 0, parent, parenttype, prevlink)) return FALSE;
  }

  return TRUE;
}

static Boolean VisitPubSet (InternalACCPtr iap, ValNodePtr vnp, Pointer parent, Uint2 parenttype, Pointer PNTR prevlink)

{
  Uint1       itemtype = OBJ_PUB_SET;
  ValNodePtr  pub;

  if (iap == NULL || vnp == NULL) return TRUE;
  (iap->itemIDs [itemtype])++;

  if (iap->callback != NULL) {
    if (! VisitCallback (iap, (Pointer) vnp, itemtype, 0, parent, parenttype, prevlink)) return FALSE;
  }

  prevlink = (Pointer PNTR) &(vnp->data.ptrvalue);
  for (pub = (ValNodePtr) vnp->data.ptrvalue; pub != NULL; pub = pub->next) {
    if (! VisitPub (iap, pub, OBJ_SEQFEAT_CIT, (Pointer) vnp, itemtype, prevlink)) return FALSE;
    prevlink = (Pointer PNTR) &(pub->next);
  }

  return TRUE;
}

static Boolean VisitSeqFeat (InternalACCPtr iap, SeqFeatPtr sfp, Uint1 itemtype, Pointer parent, Uint2 parenttype, Pointer PNTR prevlink)

{
  if (iap == NULL || sfp == NULL) return TRUE;

  if ((! iap->assignIDs) && iap->callback != NULL) {
    if (iap->objMgrFilter != NULL && (! iap->objMgrFilter [itemtype])) {
      return TRUE;
    }
  }

  while (sfp != NULL) {
    (iap->itemIDs [itemtype])++;

    if (iap->assignIDs) {
      AssignIDs (iap, &(sfp->idx), itemtype, FindFeatDefType (sfp), parent, parenttype, prevlink);
    }

    if (iap->callback != NULL) {
      if (! VisitCallback (iap, (Pointer) sfp, itemtype, sfp->idx.subtype, parent, parenttype, prevlink)) return FALSE;
    }

    if (sfp->cit != NULL) {
      if (! VisitPubSet (iap, sfp->cit, (Pointer) sfp, itemtype, (Pointer PNTR) &(sfp->cit))) return FALSE;
    }

    prevlink = (Pointer PNTR) &(sfp->next);
    sfp = sfp->next;
  }

  return TRUE;
}

static Uint2 nextAlignID = 0;

static Boolean VisitSeqAlign (InternalACCPtr iap, SeqAlignPtr sap, Uint1 itemtype, Pointer parent, Uint2 parenttype, Pointer PNTR prevlink)

{
  if (iap == NULL || sap == NULL) return TRUE;

  if ((! iap->assignIDs) && iap->callback != NULL) {
    if (iap->objMgrFilter != NULL && (! iap->objMgrFilter [itemtype])) {
      return TRUE;
    }
  }

  while (sap != NULL) {
    (iap->itemIDs [itemtype])++;

    if (iap->assignIDs) {
      AssignIDs (iap, &(sap->idx), itemtype, sap->type, parent, parenttype, prevlink);
      if (sap->alignID == 0) {
        nextAlignID++;
        sap->alignID = nextAlignID;
      }
    }

    if (iap->callback != NULL) {
      if (! VisitCallback (iap, (Pointer) sap, itemtype, sap->idx.subtype, parent, parenttype, prevlink)) return FALSE;
    }

    if (sap->segtype == SAS_DISC) {
      if (! VisitSeqAlign (iap, (SeqAlignPtr) sap->segs, OBJ_SEQALIGN, (Pointer) sap, itemtype, (Pointer PNTR) &(sap->segs))) return FALSE;
    }

    prevlink = (Pointer PNTR) &(sap->next);
    sap = sap->next;
  }

  return TRUE;
}

static Boolean VisitSeqGraph (InternalACCPtr iap, SeqGraphPtr sgp, Pointer parent, Uint2 parenttype, Pointer PNTR prevlink)

{
  Uint1  itemtype = OBJ_SEQGRAPH;

  if (iap == NULL || sgp == NULL) return TRUE;

  if ((! iap->assignIDs) && iap->callback != NULL) {
    if (iap->objMgrFilter != NULL && (! iap->objMgrFilter [itemtype])) {
      return TRUE;
    }
  }

  while (sgp != NULL) {
    (iap->itemIDs [itemtype])++;

    if (iap->assignIDs) {
      AssignIDs (iap, &(sgp->idx), itemtype, 0, parent, parenttype, prevlink);
    }

    if (iap->callback != NULL) {
      if (! VisitCallback (iap, (Pointer) sgp, itemtype, sgp->idx.subtype, parent, parenttype, prevlink)) return FALSE;
    }

    prevlink = (Pointer PNTR) &(sgp->next);
    sgp = sgp->next;
  }

  return TRUE;
}

static Boolean VisitSeqAnnot (InternalACCPtr iap, SeqAnnotPtr sap, Pointer parent, Uint2 parenttype, Pointer PNTR prevlink)

{
  Uint1  itemtype = OBJ_SEQANNOT;

  if (iap == NULL || sap == NULL) return TRUE;

  while (sap != NULL) {
    (iap->itemIDs [itemtype])++;

    if (iap->assignIDs) {
      AssignIDs (iap, &(sap->idx), itemtype, sap->type, parent, parenttype, prevlink);
    }

    if (iap->callback != NULL) {
      if (! VisitCallback (iap, (Pointer) sap, itemtype, sap->idx.subtype, parent, parenttype, prevlink)) return FALSE;
    }

    switch (sap->type) {
      case 1 : /* feature table */
        if (! VisitSeqFeat (iap, (SeqFeatPtr) sap->data, OBJ_SEQFEAT, (Pointer) sap, itemtype, (Pointer PNTR) &(sap->data))) return FALSE;
        break;
      case 2 : /* alignments */
        if (! VisitSeqAlign (iap, (SeqAlignPtr) sap->data, OBJ_SEQALIGN, (Pointer) sap, itemtype, (Pointer PNTR) &(sap->data))) return FALSE;
        break;
      case 3 : /* graphs */
        if (! VisitSeqGraph (iap, (SeqGraphPtr) sap->data, (Pointer) sap, itemtype, (Pointer PNTR) &(sap->data))) return FALSE;
        break;
      default :
        break;
    }

    prevlink = (Pointer PNTR) &(sap->next);
    sap = sap->next;
  }

  return TRUE;
}

static Boolean VisitSeqDescr (InternalACCPtr iap, SeqDescrPtr sdp, Pointer parent, Uint2 parenttype, Pointer PNTR prevlink)

{
  Uint1          itemtype = OBJ_SEQDESC;
  ObjValNodePtr  ovp;

  if (iap == NULL || sdp == NULL) return TRUE;

  if ((! iap->assignIDs) && iap->callback != NULL) {
    if (iap->objMgrFilter != NULL && (! iap->objMgrFilter [itemtype])) {
      return TRUE;
    }
  }

  while (sdp != NULL) {
    (iap->itemIDs [itemtype])++;

    if (iap->assignIDs) {
      if (sdp->extended != 0) {
        ovp = (ObjValNodePtr) sdp;
        AssignIDs (iap, &(ovp->idx), itemtype, sdp->choice, parent, parenttype, prevlink);
      } else {
        ErrPostEx (SEV_ERROR, 0, 0, "Descriptor item %d is not an ObjValNode",
                   (int) iap->itemIDs [itemtype]);
      }
    }

    if (iap->callback != NULL) {
      if (sdp->extended != 0) {
        ovp = (ObjValNodePtr) sdp;
        if (! VisitCallback (iap, (Pointer) sdp, itemtype, ovp->idx.subtype, parent, parenttype, prevlink)) return FALSE;
      }
    }

    prevlink = (Pointer PNTR) &(sdp->next);
    sdp = sdp->next;
  }

  return TRUE;
}

static Boolean VisitSeqHist (InternalACCPtr iap, SeqHistPtr shp, Pointer parent, Uint2 parenttype, Pointer PNTR prevlink)

{
  Uint1  itemtype = OBJ_SEQHIST;

  if (iap == NULL || shp == NULL) return TRUE;
  (iap->itemIDs [itemtype])++;

  if (iap->callback != NULL) {
    if (! VisitCallback (iap, (Pointer) shp, itemtype, 0, parent, parenttype, prevlink)) return FALSE;
  }

  VisitSeqAlign (iap, shp->assembly, OBJ_SEQHIST_ALIGN, (Pointer) shp, itemtype, (Pointer PNTR) &(shp->assembly));

  return TRUE;
}

static Boolean VisitSeqIds (InternalACCPtr iap, SeqIdPtr sip, Pointer parent, Uint2 parenttype, Pointer PNTR prevlink)

{
  Uint1  itemtype = OBJ_SEQID;

  if (iap == NULL || sip == NULL) return TRUE;

  while (sip != NULL) {
    (iap->itemIDs [itemtype])++;

    if (iap->callback != NULL) {
      if (! VisitCallback (iap, (Pointer) sip, itemtype, sip->choice, parent, parenttype, prevlink)) return FALSE;
    }

    prevlink = (Pointer PNTR) &(sip->next);
    sip = sip->next;
  }

  return TRUE;
}

static Boolean VisitDelta (InternalACCPtr iap, DeltaSeqPtr dsp, Pointer parent, Uint2 parenttype, Pointer PNTR prevlink)

{
  Uint1  itemtype = OBJ_BIOSEQ_DELTA;

  if (iap == NULL || dsp == NULL) return TRUE;

  while (dsp != NULL) {
    (iap->itemIDs [itemtype])++;

    if (iap->callback != NULL) {
      if (! VisitCallback (iap, (Pointer) dsp, itemtype, dsp->choice, parent, parenttype, prevlink)) return FALSE;
    }

    prevlink = (Pointer PNTR) &(dsp->next);
    dsp = dsp->next;
  }

  return TRUE;
}

static Boolean VisitSegment (InternalACCPtr iap, SeqLocPtr head, Pointer parent, Uint2 parenttype, Pointer PNTR prevlink)

{
  Uint1      itemtype = OBJ_BIOSEQ_SEG;
  SeqLocPtr  slp;

  if (iap == NULL || head == NULL) return TRUE;

  slp = NULL;
  while ((slp = SeqLocFindNext (head, slp)) != NULL) {
    (iap->itemIDs [itemtype])++;

    if (iap->callback != NULL) {
      if (! VisitCallback (iap, (Pointer) slp, itemtype, slp->choice, parent, parenttype, prevlink)) return FALSE;
    }
    prevlink = (Pointer PNTR) &(slp->next);
  }

  return TRUE;
}

static Boolean VisitBioseq (InternalACCPtr iap, BioseqPtr bsp, SeqEntryPtr curr, Pointer parent, Uint2 parenttype, Pointer PNTR prevlink)

{
  SeqLocPtr  head;
  Uint1      itemtype = OBJ_BIOSEQ;
  ValNode    vn;

  if (iap == NULL || bsp == NULL) return TRUE;
  (iap->itemIDs [itemtype])++;

  if (iap->assignIDs) {
    AssignIDs (iap, &(bsp->idx), itemtype, bsp->repr, parent, parenttype, prevlink);
    bsp->seqentry = curr;
  }

  if (iap->callback != NULL) {
    if (! VisitCallback (iap, (Pointer) bsp, itemtype, bsp->idx.subtype, parent, parenttype, prevlink)) return FALSE;
  }

  switch (bsp->repr) {
    case Seq_repr_map :
      if (iap->objMgrFilter == NULL || iap->objMgrFilter [OBJ_BIOSEQ_MAPFEAT]) {
        if (! VisitSeqFeat (iap, (SeqFeatPtr) bsp->seq_ext, OBJ_BIOSEQ_MAPFEAT, (Pointer) bsp, itemtype, (Pointer PNTR) &(bsp->seq_ext))) return FALSE;
      }
      break;
    case Seq_repr_seg :
      if (iap->objMgrFilter == NULL || iap->objMgrFilter [OBJ_BIOSEQ_SEG]) {
        vn.choice = SEQLOC_MIX;
        vn.extended = 0;
        vn.data.ptrvalue = bsp->seq_ext;
        vn.next = NULL;
        head = &vn;
        if (! VisitSegment (iap, head, (Pointer) bsp, itemtype, (Pointer PNTR) &(bsp->seq_ext))) return FALSE;
      }
      break;
    case Seq_repr_ref :
      if (iap->objMgrFilter == NULL || iap->objMgrFilter [OBJ_BIOSEQ_SEG]) {
        head = (SeqLocPtr) bsp->seq_ext;
        if (! VisitSegment (iap, head, (Pointer) bsp, itemtype, (Pointer PNTR) &(bsp->seq_ext))) return FALSE;
      }
      break;
    case Seq_repr_delta :
      if (iap->objMgrFilter == NULL || iap->objMgrFilter [OBJ_BIOSEQ_DELTA]) {
        if (! VisitDelta (iap, (DeltaSeqPtr) bsp->seq_ext, (Pointer) bsp, itemtype, (Pointer PNTR) &(bsp->seq_ext))) return FALSE;
      }
      break;
    default :
      break;
  }

  if (! VisitSeqHist (iap, bsp->hist, (Pointer) bsp, itemtype, (Pointer PNTR) &(bsp->hist))) return FALSE;

  if (! VisitSeqDescr (iap, bsp->descr, (Pointer) bsp, itemtype, (Pointer PNTR) &(bsp->descr))) return FALSE;

  if (! VisitSeqAnnot (iap, bsp->annot, (Pointer) bsp, itemtype, (Pointer PNTR) &(bsp->annot))) return FALSE;

  return TRUE;
}

static Boolean VisitBioseqSet (InternalACCPtr iap, BioseqSetPtr bssp, SeqEntryPtr curr, Pointer parent, Uint2 parenttype, Pointer PNTR prevlink)

{
  Uint1        itemtype = OBJ_BIOSEQSET;
  SeqEntryPtr  sep;

  if (iap == NULL || bssp == NULL) return TRUE;
  (iap->itemIDs [itemtype])++;

  if (iap->assignIDs) {
    AssignIDs (iap, &(bssp->idx), itemtype, bssp->_class, parent, parenttype, prevlink);
    bssp->seqentry = curr;
  }

  if (iap->callback != NULL) {
    if (! VisitCallback (iap, (Pointer) bssp, itemtype, bssp->idx.subtype, parent, parenttype, prevlink)) return FALSE;
  }

  if (! VisitSeqDescr (iap, bssp->descr, (Pointer) bssp, itemtype, (Pointer PNTR) &(bssp->descr))) return FALSE;

  if (! VisitSeqAnnot (iap, bssp->annot, (Pointer) bssp, itemtype, (Pointer PNTR) &(bssp->annot))) return FALSE;

  prevlink = (Pointer PNTR) &(bssp->seq_set);
  for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
    if (! VisitSeqEntry (iap, sep, (Pointer) bssp, itemtype, prevlink)) return FALSE;
    prevlink = (Pointer PNTR) &(sep->next);
  }

  return TRUE;
}

static Boolean VisitSeqEntry (InternalACCPtr iap, SeqEntryPtr sep, Pointer parent, Uint2 parenttype, Pointer PNTR prevlink)

{
  if (iap == NULL || sep == NULL) return TRUE;

  if (IS_Bioseq (sep)) {
    if (! VisitBioseq (iap, (BioseqPtr) sep->data.ptrvalue, sep, parent, parenttype, prevlink)) return FALSE;
  } else if (IS_Bioseq_set (sep)) {
    if (! VisitBioseqSet (iap, (BioseqSetPtr) sep->data.ptrvalue, sep, parent, parenttype, prevlink)) return FALSE;
  }

  return TRUE;
}

static Boolean VisitSeqSubCit (InternalACCPtr iap, CitSubPtr csp, Pointer parent, Uint2 parenttype, Pointer PNTR prevlink)

{
  Uint1  itemtype = OBJ_SEQSUB_CIT;

  if (iap == NULL || csp == NULL) return TRUE;
  (iap->itemIDs [itemtype])++;

  if (iap->callback != NULL) {
    if (! VisitCallback (iap, (Pointer) csp, itemtype, 0, parent, parenttype, prevlink)) return FALSE;
  }

  return TRUE;
}

static Boolean VisitSeqSubContact (InternalACCPtr iap, ContactInfoPtr cip, Pointer parent, Uint2 parenttype, Pointer PNTR prevlink)

{
  Uint1  itemtype = OBJ_SEQSUB_CONTACT;


  if (iap == NULL || cip == NULL) return TRUE;
  (iap->itemIDs [itemtype])++;

  if (iap->callback != NULL) {
    if (! VisitCallback (iap, (Pointer) cip, itemtype, 0, parent, parenttype, prevlink)) return FALSE;
  }

  return TRUE;
}

static Boolean VisitSubBlock (InternalACCPtr iap, SubmitBlockPtr sbp, Pointer parent, Uint2 parenttype, Pointer PNTR prevlink)

{
  Uint1  itemtype = OBJ_SUBMIT_BLOCK;

  if (iap == NULL || sbp == NULL) return TRUE;
  (iap->itemIDs [itemtype])++;

  if (iap->callback != NULL) {
    if (! VisitCallback (iap, (Pointer) sbp, itemtype, 0, parent, parenttype, prevlink)) return FALSE;
  }

  if (! VisitSeqSubContact (iap, sbp->contact, (Pointer) sbp, itemtype, (Pointer PNTR) &(sbp->contact))) return FALSE;

  if (! VisitSeqSubCit (iap, sbp->cit, (Pointer) sbp, itemtype, (Pointer PNTR) &(sbp->cit))) return FALSE;

  return TRUE;
}

static Boolean VisitSeqSubmit (InternalACCPtr iap, SeqSubmitPtr ssp)

{
  Uint1         itemtype = OBJ_SEQSUB;
  Pointer PNTR  prevlink;
  SeqEntryPtr   sep;

  if (iap == NULL || ssp == NULL) return TRUE;
  (iap->itemIDs [itemtype])++;

  if (iap->assignIDs) {
    AssignIDs (iap, &(ssp->idx), itemtype, 0, NULL, 0, NULL);
  }

  if (iap->callback != NULL) {
    if (! VisitCallback (iap, (Pointer) ssp, itemtype, 0, NULL, 0, NULL)) return FALSE;
  }

  if (! VisitSubBlock (iap, ssp->sub, (Pointer) ssp, itemtype, (Pointer PNTR) &(ssp->sub))) return FALSE;

  prevlink = (Pointer PNTR) &(ssp->data);
  switch (ssp->datatype) {
    case 1 : /* Seq-entrys */
      for (sep = (SeqEntryPtr) ssp->data; sep != NULL; sep = sep->next) {
        if (! VisitSeqEntry (iap, sep, (Pointer) ssp, itemtype, prevlink)) return FALSE;
        prevlink = (Pointer PNTR) &(sep->next);
      }
      break;
    case 2 : /* Seq-annots */
      if (! VisitSeqAnnot (iap, (SeqAnnotPtr) ssp->data, (Pointer) ssp, itemtype, prevlink)) return FALSE;
      break;
    case 3 : /* SeqIds */
      if (! VisitSeqIds (iap, (SeqIdPtr) ssp->data, (Pointer) ssp, itemtype, prevlink)) return FALSE;
      break;
    default :
      break;
  }

  return TRUE;
}

/*****************************************************************************
*
*   AssignIDsInEntity (entityID, datatype, dataptr)
*   	Assigns entityID/itemID/itemtype, parent pointer, and prevlink to several
*       data objects.  If entityID is > 0 it looks up the registered datatype and
*       dataptr from the object manager.  Otherwise it uses the remaining parameters,
*       assigning entityID 0 to the unregistered components.
*
*   GatherObjectsInEntity (entityID, datatype, dataptr, callback, userdata, objMgrFilter)
*   	Calls callback for objects within entity.  If the objMgrFilter parameter is NULL,
*       every object type is visited, otherwise the array length should be OBJ_MAX, and
*       the elements are from the OBJ_ list.
*
*****************************************************************************/

static Boolean VisitEntity (Uint2 entityID, Uint2 datatype, Pointer dataptr,
                            Boolean assignIDs, GatherObjectProc callback,
                            Pointer userdata, BoolPtr objMgrFilter)

{
  InternalACC    iac;
  ObjMgrDataPtr  omdp;
  ObjMgrPtr      omp;

  MemSet ((Pointer) &iac, 0, sizeof (InternalACC));
  iac.entityID = entityID;
  iac.assignIDs = assignIDs;
  iac.callback = callback;
  iac.userdata = userdata;
  iac.objMgrFilter = objMgrFilter;

  if (entityID > 0) {
    omp = ObjMgrReadLock ();
    omdp = ObjMgrGetDataStruct (omp, entityID);
    ObjMgrUnlock();
    if (omdp == NULL) return FALSE;
    if (omdp->choicetype == OBJ_SEQENTRY) {
      datatype = omdp->choicetype;
      dataptr = omdp->choice;
    } else {
      datatype = omdp->datatype;
      dataptr = omdp->dataptr;
    }
  }

  if (datatype == 0 || dataptr == NULL) return FALSE;

  switch (datatype) {
    case OBJ_SEQENTRY :
      VisitSeqEntry (&iac, (SeqEntryPtr) dataptr, NULL, 0, NULL);
      break;
    case OBJ_BIOSEQ :
      VisitBioseq (&iac, (BioseqPtr) dataptr, NULL, NULL, 0, NULL);
      break;
    case OBJ_BIOSEQSET :
      VisitBioseqSet (&iac, (BioseqSetPtr) dataptr, NULL, NULL, 0, NULL);
      break;
    case OBJ_SEQDESC :
      VisitSeqDescr (&iac, (SeqDescrPtr) dataptr, NULL, 0, NULL);
      break;
    case OBJ_SEQANNOT :
      VisitSeqAnnot (&iac, (SeqAnnotPtr) dataptr, NULL, 0, NULL);
      break;
    case OBJ_SEQFEAT :
      VisitSeqFeat (&iac, (SeqFeatPtr) dataptr, OBJ_SEQFEAT, NULL, 0, NULL);
      break;
    case OBJ_SEQALIGN :
      VisitSeqAlign (&iac, (SeqAlignPtr) dataptr, OBJ_SEQALIGN, NULL, 0, NULL);
      break;
    case OBJ_SEQGRAPH :
      VisitSeqGraph (&iac, (SeqGraphPtr) dataptr, NULL, 0, NULL);
      break;
    case OBJ_SEQSUB :
      VisitSeqSubmit (&iac, (SeqSubmitPtr) dataptr);
      break;
    case OBJ_SUBMIT_BLOCK :
      VisitSubBlock (&iac, (SubmitBlockPtr) dataptr, NULL, 0, NULL);
      break;
    case OBJ_SEQSUB_CONTACT :
      VisitSeqSubContact (&iac, (ContactInfoPtr) dataptr, NULL, 0, NULL);
      break;
    case OBJ_SEQHIST :
      VisitSeqHist (&iac, (SeqHistPtr) dataptr, NULL, 0, NULL);
      break;
    case OBJ_SEQHIST_ALIGN :
      VisitSeqAlign (&iac, (SeqAlignPtr) dataptr, OBJ_SEQHIST_ALIGN, NULL, 0, NULL);
      break;
    case OBJ_PUB :
      VisitPub (&iac, (ValNodePtr) dataptr, OBJ_PUB, NULL, 0, NULL);
      break;
    case OBJ_SEQSUB_CIT :
      VisitSeqSubCit (&iac, (CitSubPtr) dataptr, NULL, 0, NULL);
      break;
    case OBJ_SEQID :
      VisitSeqIds (&iac, (SeqIdPtr) dataptr, NULL, 0, NULL);
      break;
    default :
      return FALSE;
  }

  return TRUE;
}

NLM_EXTERN Boolean LIBCALL AssignIDsInEntity (Uint2 entityID, Uint2 datatype, Pointer dataptr)

{
  return VisitEntity (entityID, datatype, dataptr, TRUE, NULL, NULL, NULL);
}

NLM_EXTERN Boolean LIBCALL GatherObjectsInEntity (Uint2 entityID, Uint2 datatype, Pointer dataptr,
                                                  GatherObjectProc callback, Pointer userdata, BoolPtr objMgrFilter)

{
  if (callback == NULL) return FALSE;
  return VisitEntity (entityID, datatype, dataptr, FALSE, callback, userdata, objMgrFilter);
}

/*****************************************************************************
*
*   GetNextDescriptorUnindexed (bsp, choice, curr)
*   	After AssignIDsInEntity, gets next descriptor up the set hierarchy.
*
*****************************************************************************/

NLM_EXTERN SeqDescrPtr GetNextDescriptorUnindexed (
  BioseqPtr bsp,
  Uint1 choice,
  SeqDescrPtr curr
)

{
  BioseqSetPtr   bssp = NULL;
  ObjValNodePtr  ovp;
  SeqDescrPtr    sdp;

  if (bsp == NULL || choice == 0) return NULL;

  if (curr == NULL) {
    sdp = bsp->descr;
    curr = sdp;
  } else {
    sdp = curr->next;
  }
  while (sdp != NULL) {
    if (sdp->choice == choice) return sdp;
    sdp = sdp->next;
  }

  if (curr == NULL || curr->extended == 0) return NULL;
  ovp = (ObjValNodePtr) curr;
  if (ovp->idx.parenttype == OBJ_BIOSEQ) {
    bsp = (BioseqPtr) ovp->idx.parentptr;
    if (bsp == NULL) return NULL;
    if (bsp->idx.parenttype != OBJ_BIOSEQSET) return NULL;
    bssp = (BioseqSetPtr) bsp->idx.parentptr;
  } else if (ovp->idx.parenttype == OBJ_BIOSEQSET) {
    bssp = (BioseqSetPtr) ovp->idx.parentptr;
    if (bssp == NULL) return NULL;
    if (bssp->idx.parenttype != OBJ_BIOSEQSET) return NULL;
    bssp = (BioseqSetPtr) bssp->idx.parentptr;
  } else return NULL;

  while (bssp != NULL) {
    for (sdp = bssp->descr; sdp != NULL; sdp = sdp->next) {
      if (sdp->choice == choice) return sdp;
     }
     if (bssp->idx.parenttype != OBJ_BIOSEQSET) return NULL;
     bssp = (BioseqSetPtr) bssp->idx.parentptr;
  }
  return NULL;
}

typedef struct getptrforid {
  Uint2    entityID;
  Uint2    itemID;
  Uint2    itemtype;
  Pointer  dataptr;
} GetPtrForId, PNTR GetPtrForIdPtr;

static Boolean GetPointerProc (GatherObjectPtr gop)

{
  GetPtrForIdPtr  gfp;

  if (gop == NULL) return TRUE;
  gfp = (GetPtrForIdPtr) gop->userdata;
  if (gfp == NULL) return TRUE;
  if (gfp->itemID != gop->itemID || gfp->itemtype != gop->itemtype) return TRUE;
  gfp->dataptr = gop->dataptr;
  return TRUE;
}

NLM_EXTERN Pointer LIBCALL GetPointerForIDs (Uint2 entityID, Uint2 itemID, Uint2 itemtype)

{
  GetPtrForId  gfi;
  Boolean      objMgrFilter [OBJ_MAX];

  if (itemtype >= OBJ_MAX) return NULL;

  MemSet ((Pointer) objMgrFilter, FALSE, sizeof (objMgrFilter));
  objMgrFilter [itemtype] = TRUE;
  gfi.entityID = entityID;
  gfi.itemID = itemID;
  gfi.itemtype = itemtype;
  gfi.dataptr = NULL;

  GatherObjectsInEntity (entityID, 0, NULL, GetPointerProc, &gfi, objMgrFilter);

  return gfi.dataptr;
}

/*****************************************************************************
*
*   DeleteMarkedObjects (entityID, datatype, dataptr)
*   	Unlinks and removes all objects whose GatherIndex.deleteme flag is not 0.
*
*****************************************************************************/

static void DeleteMarkedSeqFeat (SeqFeatPtr sfp, Pointer PNTR prevlink)

{
  SeqFeatPtr  next;

  while (sfp != NULL) {
    next = sfp->next;

    if (sfp->idx.deleteme != 0) {
      *prevlink = sfp->next;
      sfp->next = NULL;
      SeqFeatFree (sfp);
    } else {
      sfp->idx.prevlink = prevlink;
      prevlink = (Pointer PNTR) &(sfp->next);
    }

    sfp = next;
  }
}

static void DeleteMarkedSeqAlign (SeqAlignPtr sap, Pointer PNTR prevlink)

{
  SeqAlignPtr  next;

  while (sap != NULL) {
    next = sap->next;

    if (sap->idx.deleteme != 0) {
      *prevlink = sap->next;
      sap->next = NULL;
      SeqAlignFree (sap);
    } else {
      sap->idx.prevlink = prevlink;
      prevlink = (Pointer PNTR) &(sap->next);
    }

    sap = next;
  }
}

static void DeleteMarkedSeqGraph (SeqGraphPtr sgp, Pointer PNTR prevlink)

{
  SeqGraphPtr  next;

  while (sgp != NULL) {
    next = sgp->next;

    if (sgp->idx.deleteme != 0) {
      *prevlink = sgp->next;
      sgp->next = NULL;
      SeqGraphFree (sgp);
    } else {
      sgp->idx.prevlink = prevlink;
      prevlink = (Pointer PNTR) &(sgp->next);
    }

    sgp = next;
  }
}

static void DeleteMarkedSeqAnnot (SeqAnnotPtr sap, Pointer PNTR prevlink)

{
  SeqAnnotPtr  next;

  while (sap != NULL) {
    next = sap->next;

    if (sap->idx.deleteme == 0) {
      switch (sap->type) {
        case 1 :
          DeleteMarkedSeqFeat ((SeqFeatPtr) sap->data, (Pointer PNTR) &(sap->data));
          break;
        case 2 :
          DeleteMarkedSeqAlign ((SeqAlignPtr) sap->data, (Pointer PNTR) &(sap->data));
          break;
        case 3 :
          DeleteMarkedSeqGraph ((SeqGraphPtr) sap->data, (Pointer PNTR) &(sap->data));
          break;
        default :
          break;
      }
      if (sap->data == NULL) {
        sap->idx.deleteme = 1;
      }
    }

    if (sap->idx.deleteme != 0) {
      *prevlink = sap->next;
      sap->next = NULL;
      SeqAnnotFree (sap);
    } else {
      sap->idx.prevlink = prevlink;
      prevlink = (Pointer PNTR) &(sap->next);
    }
  
    sap = next;
  }
}

static void DeleteMarkedSeqDescr (SeqDescrPtr sdp, Pointer PNTR prevlink)

{
  SeqDescrPtr    next;
  ObjValNodePtr  ovp;

  while (sdp != NULL) {
    next = sdp->next;

    ovp = (ObjValNodePtr) sdp;
    if (sdp->extended != 0 && ovp->idx.deleteme != 0) {
      *prevlink = sdp->next;
      sdp->next = NULL;
      SeqDescFree (sdp);
    } else {
      if (sdp->extended != 0) {
        ovp->idx.prevlink = prevlink;
      }
      prevlink = (Pointer PNTR) &(sdp->next);
    }

    sdp = next;
  }
}

static void DeleteMarkedSeqEntry (SeqEntryPtr sep, Pointer PNTR prevlink)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  SeqEntryPtr   next;
  Boolean       unlink;

  while (sep != NULL) {
    next = sep->next;
    unlink = FALSE;
    bsp = NULL;
    bssp = NULL;

    if (IS_Bioseq (sep)) {

      bsp = (BioseqPtr) sep->data.ptrvalue;
      if (bsp != NULL) {
        if (bsp->idx.deleteme != 0) {
          unlink = TRUE;
        } else {
          DeleteMarkedSeqDescr (bsp->descr, (Pointer PNTR) &(bsp->descr));
          DeleteMarkedSeqAnnot (bsp->annot, (Pointer PNTR) &(bsp->annot));
        }
      }

    } else if (IS_Bioseq_set (sep)) {

      bssp = (BioseqSetPtr) sep->data.ptrvalue;
      if (bssp != NULL) {
        if (bssp->idx.deleteme != 0) {
          unlink = TRUE;
        } else {
          DeleteMarkedSeqDescr (bssp->descr, (Pointer PNTR) &(bssp->descr));
          DeleteMarkedSeqAnnot (bssp->annot, (Pointer PNTR) &(bssp->annot));
          DeleteMarkedSeqEntry (bssp->seq_set, (Pointer PNTR) &(bssp->seq_set));
        }
      }
    }
    if (unlink) {
      *prevlink = sep->next;
      sep->next = NULL;
      SeqEntryFree (sep);
    } else {
      if (bsp != NULL) {
        bsp->idx.prevlink = prevlink;
      } else if (bssp != NULL) {
        bssp->idx.prevlink = prevlink;
      }
      prevlink = (Pointer PNTR) &(sep->next);
    }
    sep = next;
  }
}

NLM_EXTERN Boolean DeleteMarkedObjects (Uint2 entityID, Uint2 datatype, Pointer dataptr)

{
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  ObjMgrDataPtr  omdp;
  ObjMgrPtr      omp;
  SeqEntryPtr    sep = NULL;
  SeqSubmitPtr   ssp;

  if (entityID > 0) {
    omp = ObjMgrReadLock ();
    omdp = ObjMgrGetDataStruct (omp, entityID);
    ObjMgrUnlock();
    if (omdp == NULL) return FALSE;
    if (omdp->choicetype == OBJ_SEQENTRY) {
      datatype = omdp->choicetype;
      dataptr = omdp->choice;
    } else {
      datatype = omdp->datatype;
      dataptr = omdp->dataptr;
    }
  }

  if (datatype == 0 || dataptr == NULL) return FALSE;

  switch (datatype) {
    case OBJ_SEQENTRY :
      sep = (SeqEntryPtr) dataptr;
      break;
    case OBJ_BIOSEQ :
      bsp = (BioseqPtr) dataptr;
      sep = bsp->seqentry;
      break;
    case OBJ_BIOSEQSET :
      bssp = (BioseqSetPtr) dataptr;
      sep = bssp->seqentry;
      break;
    case OBJ_SEQSUB :
      ssp = (SeqSubmitPtr) dataptr;
      if (ssp->datatype == 1) {
        sep = (SeqEntryPtr) ssp->data;
      }
      break;
    default :
      return FALSE;
  }

  if (sep == NULL) return FALSE;

  DeleteMarkedSeqEntry (sep, (Pointer PNTR) &sep);

  return TRUE;
}

