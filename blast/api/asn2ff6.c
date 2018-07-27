/*   asn2ff6.c
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
* File Name:  asn2ff6.c
*
* Author:  Karl Sirotkin, Tom Madden, Tatiana Tatusov
*
* Version Creation Date:   7/15/95
*
* $Revision: 6.65 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: asn2ff6.c,v $
* Revision 6.65  2001/12/05 18:13:53  cavanaug
* Changes for new LOCUS line format
*
* Revision 6.64  2001/08/21 17:33:33  kans
* snoRNA can show /product
*
* Revision 6.63  2001/08/07 15:51:08  kans
* use NUM_SEQID, added third party annotation seqids
*
* Revision 6.62  2001/07/18 14:50:13  kans
* gather features with gsc.useSeqMgrIndexes if genpept, raw, indexing requested, and IndexedGetDescrForDiv to speed up finding division
*
* Revision 6.61  2001/07/03 20:01:41  kans
* AddGBQual ASN2GNBK_STRIP_NOTE_PERIODS trim trailing tilde first
*
* Revision 6.60  2001/07/03 00:05:51  kans
* TrimSpacesAndJunkFromEnds on genbankblock->source if ASN2GNBK_STRIP_NOTE_PERIODS
*
* Revision 6.59  2001/06/26 23:43:35  kans
* moved second period check to inside last period check
*
* Revision 6.58  2001/06/26 23:36:06  kans
* in AddGBQual if ASN2GNBK_STRIP_NOTE_PERIODS, trim one or two periods at end
*
* Revision 6.57  2001/06/13 14:41:58  yaschenk
* changing increment of 10 to 1024 in EnlargeSortList()
*
* Revision 6.56  2001/06/04 21:30:52  kans
* TrimSpacesAndSemicolons trims leading semicolons as well as leading spaces
*
* Revision 6.55  2001/06/01 18:46:26  tatiana
* NG_ added to ValidateAccession
*
* Revision 6.54  2001/05/31 23:45:48  kans
* if ASN2GNBK_STRIP_NOTE_PERIODS and IsEllipsis, do not strip period
*
* Revision 6.53  2001/05/29 23:27:47  kans
* added support for snoRNA - flatfile prints as misc_RNA for now
*
* Revision 6.52  2001/04/16 16:51:42  tatiana
* GetDivision(): CON division never use for aa
*
* Revision 6.51  2001/04/06 12:47:43  beloslyu
* missing flatloc declaration was added
*
* Revision 6.50  2001/04/05 21:41:26  tatiana
* REGION added in GetLocusPartsAwp()
*
* Revision 6.49  2001/04/04 22:05:16  kans
* In GB_PrintPubs under ASN2GNBK_STRIP_NOTE_PERIODS clean up comma/space/semicolon (TF)
*
* Revision 6.48  2001/04/04 21:46:56  kans
* TrimSpacesAndJunkFromEnds if ASN2GNBK_STRIP_NOTE_PERIODS (TF)
*
* Revision 6.47  2001/04/02 21:25:19  kans
* AddGBQual under ASN2GNBK_STRIP_NOTE_PERIODS also removes ; ; substrings
*
* Revision 6.46  2001/03/26 17:36:06  kans
* added NULL for endogenous-virus to genome prefix array
*
* Revision 6.45  2001/02/16 16:52:22  tatiana
* special case locus for NT_ records
*
* Revision 6.44  2001/01/26 19:21:48  kans
* extrachromosomal into source note, removed macronuclear, extrachrom, plasmid from organism line
*
* Revision 6.43  2001/01/19 21:51:04  kans
* finally got ASN2GNBK_STRIP_NOTE_PERIODS logic right
*
* Revision 6.42  2001/01/19 18:45:28  kans
* another attempt to use ASN2GNBK_STRIP_NOTE_PERIODS to remove extraneous asn2ff/asn2gnbk diffs
*
* Revision 6.41  2001/01/08 18:36:40  kans
* removed ASN2GNBK_STRIP_NOTE_PERIODS - this was not the right place
*
* Revision 6.40  2001/01/06 22:09:42  kans
* added ASN2GNBK_STRIP_NOTE_PERIODS to try to eliminate trivial note discrepancies
*
* Revision 6.39  2000/11/29 20:46:11  tatiana
* HTC division added for MI_TECH_htc
*
* Revision 6.38  2000/10/24 20:28:44  tatiana
* ValidateAccession accepts XP, XM
*
* Revision 6.37  2000/09/20 21:26:19  tatiana
* all organelles adde to ORGANISM line
*
* Revision 6.36  2000/09/11 18:52:59  tatiana
* PUBMED linetype is legal in release mode
*
* Revision 6.35  2000/08/25 16:16:46  kans
* ValidateLocus initializes num_of_digits even if > 1000 segments
*
* Revision 6.34  2000/08/01 21:09:39  tatiana
* ValidateVersion is colld in forgbrel option only
*
* Revision 6.33  2000/06/29 12:23:30  kans
* GenPept on Seq_repr_virtual shown only if is_www || ajp->mode != RELEASE_MODE, earlier kludge of ignoring get_www was probably too broad
*
* Revision 6.32  2000/06/28 19:31:22  kans
* in SeqToAwp always set is_www to TRUE, so virtual sequences show up on non-web applications
*
* Revision 6.31  2000/06/23 15:42:34  tatiana
* removed virion and proviral from ORGANISM line
*
* Revision 6.30  2000/06/21 15:04:57  tatiana
* space added to Virion
*
* Revision 6.29  2000/06/12 20:49:04  tatiana
* new organelles added to ORGANISM filed
*
* Revision 6.28  2000/06/05 17:51:53  tatiana
* increase size of feature arrays to Int4
*
* Revision 6.27  2000/02/09 19:34:39  kans
* added forgbrel flag to Asn2ffJobPtr, currently used to suppress PUBMED line, which was not formally announced in release notes
*
* Revision 6.26  2000/01/28 17:56:48  kans
* show_gi always FALSE to suppress NID and PID, added support for PUBMED line in GenBank format
*
* Revision 6.25  2000/01/18 17:09:24  tatiana
* NP added to ValidateAccession
*
* Revision 6.24  1999/10/06 20:20:24  bazhin
* Removed memory leaks in GeneStructContentFree() and GetPubsAwp()
* functions.
*
* Revision 6.23  1999/09/23 18:09:33  tatiana
* ValidateAccession modified for N*_ accession
*
* Revision 6.22  1999/09/15 18:17:12  tatiana
* GRAPHIK_FMT corrected
*
* Revision 6.18  1999/04/02 19:33:55  tatiana
* MI_TECH_htgs_0 added in BioseqGetGBDivCode()
*
* Revision 6.17  1999/04/01 20:44:12  kans
* Int2 lengths to Int4 to allow CountGapsInDeltaSeq with buffer > 32K
*
* Revision 6.16  1999/03/31 01:09:23  tatiana
* ValidateAccession accepts 3+5
*
* Revision 6.15  1999/03/30 21:00:45  tatiana
*  ValidateOtherAccession() added
*
* Revision 6.14  1999/03/22 23:22:32  tatiana
* accession.version modifications
*
* Revision 6.13  1999/01/12 16:57:55  kans
* SeqToAwp checks for null ep before dereferencing
*
* Revision 6.12  1998/11/24 20:15:03  kans
* seqid other has better priority than local so refgene id is used preferentially
*
* Revision 6.11  1998/10/30 01:12:00  kans
* GetPubsAwp GatherEntity filters out OBJ_SEQALIGN - this was being hit many times on big records, and there is no need for asn2ff to see alignments
*
* Revision 6.10  1998/09/24 17:46:00  kans
* fixed GetDBXrefFromGene problem (TT)
*
* Revision 6.9  1998/06/15 14:59:49  tatiana
* UNIX compiler warnings fixed
*
* Revision 6.8  1998/05/11 21:58:33  tatiana
* some functions moved from asn2ff1.c
*
* Revision 6.7  1998/05/05 19:53:50  tatiana
* SEQFEAT_RSITE supressed in GetNAFeatKey()
*
* Revision 6.6  1998/04/30 21:49:10  tatiana
* *** empty log message ***
*
* Revision 6.5  1998/02/10 17:01:14  tatiana
* AddGBQualEx() added
*
* Revision 6.4  1998/01/13 21:35:20  tatiana
*  AsnIoHash moved to asnio.c file
*
* Revision 6.3  1998/01/13 21:14:50  tatiana
* static AsnIoHash changed to AsnIoHash to avoid fubction name collision
*
* Revision 6.2  1997/12/15 15:53:29  tatiana
* features processing has been changed
*
* Revision 6.1  1997/09/16 15:41:49  kans
* added SEQFEAT_SITE case to GetNAFeatKey (TT)
*
* Revision 5.25  1997/07/28 19:03:59  vakatov
* [WIN32,MSVC++]  Restored lost "NCBIOBJ.LIB" pro-DLL modifications
*
 * Revision 5.24  1997/07/28 14:26:11  vakatov
 * BioseqGetGBDivCode() proto in-sync with its header-located declaration
 *
 * Revision 5.23  1997/07/24 23:57:41  tatiana
 * fixed sfp_order
 *
 * Revision 5.22  1997/07/24 15:59:06  tatiana
 * aaaaaaa bug fixed in Getscblknum
 *
 * Revision 5.21  1997/07/16 21:18:42  tatiana
 *  added sorting by feat type in CompareSfpForHeap()
 *
 * Revision 5.20  1997/06/19 18:37:17  vakatov
 * [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
 *
 * Revision 5.19  1997/05/21 14:43:27  tatiana
 * fix empty /product in GetNAFeatKey
 *
 * Revision 5.17  1997/01/13  22:33:04  tatiana
 * added CompareGeneName()
 *
 * Revision 5.16  1996/12/17  22:47:56  tatiana
 * added StoreFeatFree()
 *
 * Revision 5.15  1996/10/25  22:12:10  tatiana
 * doesn't add empty ("") val if qual is translation
 *
 * Revision 5.14  1996/10/02  15:14:38  tatiana
 * a bug fixed
 *
 * Revision 5.13  1996/10/01  22:42:09  tatiana
 * fixed duplicated notes in NoteToCharPtrStack
 *
 * Revision 5.12  1996/09/09  13:36:02  kans
 * moved BioseqGetGBDivCode from toasn.[ch] to asn2ff.h/asn2ff6.c
 *
 * Revision 5.11  1996/09/03  19:52:49  tatiana
 * extra_loc added
 *
 * Revision 5.10  1996/08/28  21:40:35  tatiana
 * don't copy new location from gather
 *
 * Revision 5.9  1996/08/16  20:34:45  tatiana
 * GetNAFeatKey() changed
 *
 * Revision 5.7  1996/08/09  21:08:57  tatiana
 * a bug fixed in GetNAFeatKey
 *
 * Revision 5.6  1996/07/30  16:35:05  tatiana
 * Boolean new added to GetNaFeatKey()
 *
 * Revision 5.5  1996/07/19  21:38:15  tatiana
 * ERR_GI_No_GI_Number changed from 	ErrPostEx to	ErrPostStr
 *
 * Revision 5.3  1996/07/02  18:11:18  tatiana
 * calculate hash in StoreFeat
 *
 * Revision 5.2  1996/06/14  18:05:03  tatiana
 * GetNAFeatKey change
 *
 * Revision 5.1  1996/06/11  15:26:36  tatiana
 * GetGINumber is modified to get also embl NI
 *
 * Revision 4.17  1996/05/16  21:00:52  tatiana
 * RemoveRedundantFeats addded
 *
 * Revision 4.16  1996/04/29  18:51:42  tatiana
 * whole_book format added
 *
 * Revision 4.15  1996/04/15  14:36:23  tatiana
 * memory leaks cleaning
 *
 * Revision 4.13  1996/02/28  04:53:06  ostell
 * changes to support segmented master seeuquences
 *
 * Revision 4.12  1996/02/15  15:54:51  tatiana
 * minor clean ups
 *
 * Revision 4.11  1996/01/29  22:39:10  tatiana
 * error posting MODULE
 *
 * Revision 4.10  1995/12/20  22:41:56  tatiana
 * removed redundant functions
 *
 * Revision 4.9  1995/12/12  20:21:05  tatiana
 * CitSub validation fixed
 *
 * Revision 4.8  1995/12/10  22:19:31  tatiana
 * Imprint in CitSub became optional
 *
 * Revision 4.7  1995/11/17  21:28:35  kans
 * asn2ff now uses gather (Tatiana)
 *
 * Revision 4.2  1995/08/04  15:26:42  tatiana
 * bug fixed in GetPubDate (check for Null pointer).
 *
 * Revision 4.1  1995/08/01  14:53:08  tatiana
 * change SeqIdPrint to SeqIdWrite
 *
 * Revision 1.57  1995/07/17  19:33:20  kans
 * parameters combined into Asn2ffJobPtr structure
* ==========================================================================
*/

#include <asn2ff6.h>
#include <asn2ffp.h>
#include <a2ferrdf.h>
#include <asn2ffg.h>
#include <utilpub.h>
#include <ffprint.h>
#include <explore.h>
#include <sqnutils.h>

#define BUF_EXT_LENGTH 4

/*---------- order for other id FASTA_LONG (copied from SeqIdWrite) ------- */

static Uint1 fasta_order[NUM_SEQID] = {  
33, /* 0 = not set */
20, /* 1 = local Object-id */
15,  /* 2 = gibbsq */
16,  /* 3 = gibbmt */
30, /* 4 = giim Giimport-id */
10, /* 5 = genbank */
10, /* 6 = embl */
10, /* 7 = pir */
10, /* 8 = swissprot */
15,  /* 9 = patent */
10, /* 10 = other TextSeqId */
20, /* 11 = general Dbtag */
32,  /* 12 = gi */
10, /* 13 = ddbj */
10, /* 14 = prf */
12, /* 15 = pdb */
10,  /* 16 = tpg */
10,  /* 17 = tpe */
10   /* 18 = tpd */
};


static Uint1 sfp_order[21] = {0, 
2, /* SEQFEAT_GENE */
5, /* SEQFEAT_ORG */
3, /* SEQFEAT_CDREGION */
5, /* SEQFEAT_PROT */
1, /* SEQFEAT_RNA */
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5};

SeqFeatPtr MakeSyntheticSeqFeat PROTO ((void));
ValNodePtr LookForPubsOnFeat PROTO ((SeqFeatPtr sfp, ValNodePtr PubOnFeat));

Boolean asn2ff_flags[13];

NLM_EXTERN GeneStructPtr GeneStructNew (void)
{
	GeneStructPtr gsp;
	
	gsp = (GeneStructPtr) MemNew(sizeof(GeneStruct));
	gsp->gene = NULL;
	gsp->product = NULL;
	gsp->standard_name = NULL;
	gsp->map = (CharPtr PNTR) MemNew(sizeof(CharPtr));
	gsp->map_index = 0;
	gsp->map_size = 1;
	gsp->ECNum = NULL;
	gsp->activity = NULL;
	gsp->grp = NULL;

	return gsp;
}

NLM_EXTERN NoteStructPtr NoteStructNew (NoteStructPtr nsp)
{
	nsp = (NoteStructPtr) MemNew(sizeof(NoteStruct));

	nsp->note = (CharPtr PNTR) MemNew(5*sizeof(CharPtr));
	nsp->note_annot = (CharPtr PNTR) MemNew(5*sizeof(CharPtr));
	nsp->note_alloc = (Uint1 PNTR) MemNew(5*sizeof(Uint1));
	nsp->note_index = 0;
	nsp->note_size = 5;

	return nsp;
}

static void GeneStructContentFree(GeneStructPtr gsp)
{
	ValNodePtr v, vnext;
	
	if (gsp->gene) {
		if (gsp->gene->data.ptrvalue != NULL) {
			MemFree(gsp->gene->data.ptrvalue);
		}
		gsp->gene = ValNodeFree(gsp->gene);
	}
	for (v = gsp->product; v; v = vnext) {
		vnext = v->next;
		if (v->data.ptrvalue != NULL) {
			MemFree(v->data.ptrvalue);
		}
		MemFree(v);
	}
	for (v = gsp->standard_name; v != NULL; v = vnext) {
		vnext = v->next;
		if (v->data.ptrvalue != NULL) {
			MemFree(v->data.ptrvalue);
		}
		MemFree(v);
	}
	for (v = gsp->ECNum; v; v = vnext) {
		vnext = v->next;
		if (v->data.ptrvalue != NULL) {
			MemFree(v->data.ptrvalue);
		}
		MemFree(v);
	}
	for (v = gsp->activity; v; v = vnext) {
		vnext = v->next;
		if (v->data.ptrvalue != NULL) {
			MemFree(v->data.ptrvalue);
		}
		MemFree(v);
	}
	if (gsp->grp) {
		GeneRefFree(gsp->grp);
	}
	return;
}

NLM_EXTERN void GeneStructFree (GeneStructPtr gsp)
{
	if (gsp == NULL)
		return;
	gsp->map = MemFree(gsp->map);
	GeneStructContentFree(gsp);
	MemFree(gsp);
}

NLM_EXTERN void NoteStructFree (NoteStructPtr nsp)
{
	Int2 index;

	if (nsp == NULL) {
	return;
	}
	for (index=0; index < nsp->note_index; index++) {
			if (nsp->note_alloc[index] == ASN2FLAT_ALLOC)
				nsp->note[index] = MemFree(nsp->note[index]);
	}
	nsp->note = MemFree(nsp->note);
	nsp->note_annot = MemFree(nsp->note_annot);
	nsp->note_alloc = MemFree(nsp->note_alloc);
	MemFree(nsp);
}

NLM_EXTERN void NoteStructReset (NoteStructPtr nsp)
{
	Int2 index;

	if (nsp == NULL) {
		return;
	}
	for (index=0; index<nsp->note_index; index++)
	{
		if (nsp->note_alloc[index] == ASN2FLAT_ALLOC)
			nsp->note[index] = MemFree(nsp->note[index]);
		nsp->note[index] = NULL;
		nsp->note_annot[index] = NULL;
	}
	nsp->note_index = 0;
}


NLM_EXTERN void ListFree (SeqFeatPtr PNTR PNTR List, Int4 range)
{
	Int4 index;

	for (index=0; index < range; index++)
		MemFree(List[index]);

	MemFree(List);
}

/***********************************************************************
*SeqFeatPtr MakeSyntheticSeqFeat(void) 
*
*	This function allocates a "synthetic" SeqFeatPtr, which is
*	used to print the SeqFeats out.  To print out SeqFeats, they
*	are copied to this "synthetic" sfp, which is an ImpFeat, 
*	adjusted, validated, and then a function prints out this ImpFeat.
*************************************************************************/

NLM_EXTERN SeqFeatPtr MakeSyntheticSeqFeat(void) 
{
	ImpFeatPtr ifp;
	SeqFeatPtr sfp_out;

	sfp_out = SeqFeatNew();
	if (sfp_out)
	{
		sfp_out->data.choice = SEQFEAT_IMP;
		sfp_out->qual = NULL;
		ifp = sfp_out->data.value.ptrvalue = ImpFeatNew();
	/*	ifp->key = (CharPtr) MemNew(20*sizeof(Char)); */
/* key may be more than 20 char one day and cause segmentation fault */
		ifp->key = NULL;
		ifp->loc = NULL;
		sfp_out->comment = NULL;
		sfp_out->location = NULL;
		sfp_out->product = NULL;
	}

	return sfp_out;
}

NLM_EXTERN void CpNoteToCharPtrStack (NoteStructPtr nsp, CharPtr annot, CharPtr string)
{
	NoteToCharPtrStack(nsp, annot, string, ASN2FLAT_NOT_ALLOC);
	return;
}

NLM_EXTERN void SaveNoteToCharPtrStack (NoteStructPtr nsp, CharPtr annot, CharPtr string)
{
	NoteToCharPtrStack(nsp, annot, string, ASN2FLAT_ALLOC);
	return;
}


NLM_EXTERN void NoteToCharPtrStack (NoteStructPtr nsp, CharPtr annot, CharPtr string, Uint1 alloc)
{
	Int2 index, note_size;

	if (nsp)
	{
		note_size = nsp->note_size;
		index = nsp->note_index;
	}
	else
		return;
/*** add check for duplicated notes 9-27-96 ***/
/*	if (string[StringLen(string)-1] == '.')
		string[StringLen(string)-1] = '\0';
	for (i = 0; i < note_size; i++) {
		if (nsp->note[i] && StringStr(nsp->note[i], string) != NULL) {
			return;
		}
	}
*/
	if (index == note_size)
		EnlargeCharPtrStack(nsp, 5);

	nsp->note_annot[index] = annot;

	if (alloc == ASN2FLAT_NOT_ALLOC)
	{
		nsp->note_alloc[index] = ASN2FLAT_NOT_ALLOC;
		nsp->note[index] = string;
	}
	else if (alloc == ASN2FLAT_ALLOC)
	{
		nsp->note_alloc[index] = ASN2FLAT_ALLOC;
		nsp->note[index] = StringSave(string);
	}

	nsp->note_index++;

	return;
}

NLM_EXTERN void EnlargeCharPtrStack (NoteStructPtr nsp, Int2 enlarge)
{
	CharPtr PNTR newstr;
	CharPtr PNTR new_annot;
	Int2 index;
	Uint1 PNTR new_alloc;

	newstr = (CharPtr PNTR) MemNew((size_t) 
		((enlarge+(nsp->note_size))*sizeof(CharPtr)));
	new_annot = (CharPtr PNTR) MemNew((size_t) 
		((enlarge+(nsp->note_size))*sizeof(CharPtr)));
	new_alloc = (Uint1 PNTR) MemNew((size_t) 
		((enlarge+(nsp->note_size))*sizeof(Uint1)));

	for (index=0; index<(nsp->note_size); index++) {
		newstr[index] = nsp->note[index];
		new_annot[index] = nsp->note_annot[index];
		new_alloc[index] = nsp->note_alloc[index];
	}
	nsp->note_size += enlarge;
	nsp->note = MemFree(nsp->note);
	nsp->note_annot = MemFree(nsp->note_annot);
	nsp->note_alloc = MemFree(nsp->note_alloc);
	nsp->note = newstr;
	nsp->note_annot = new_annot;
	nsp->note_alloc = new_alloc;
}

NLM_EXTERN SortStructPtr EnlargeSortList(SortStructPtr List, Int4 size)
{
	SortStructPtr NewList;

	if (size % 1024 == 0) {
		NewList = (SortStructPtr) MemNew((size+1024)*sizeof(SortStruct));
		if (size > 0) {
			MemCopy(NewList, List, (size * sizeof(SortStruct)));
			MemFree(List);
		}
		return NewList;
	}
	return List;

}	/* EnlargeSortList */

NLM_EXTERN int LIBCALLBACK CompareSfpForHeap (VoidPtr vp1, VoidPtr vp2)
{

	SortStructPtr sp1 = vp1;
	SortStructPtr sp2 = vp2;
	BioseqPtr bsp;
	SeqFeatPtr sfp1, sfp2;
	Int2 status = 0;

	bsp = sp1->bsp;
	sfp1 = sp1->sfp;
	sfp2 = sp2->sfp;
	if (sfp1 == NULL || sfp2 == NULL) {
		return status;
	}

	status = SeqLocOrder(sfp1->location, sfp2->location, bsp);

	if (ABS(status) >= 2 && sp1->seg_bsp) {
		status = SeqLocOrder(sfp1->location, sfp2->location, sp1->seg_bsp);
	}
	if (status == 0 && sfp1->data.choice < 6 && sfp2->data.choice < 6) {
		status = sfp_order[sfp1->data.choice] - sfp_order[sfp2->data.choice];
	}
	return status;
}
NLM_EXTERN int LIBCALLBACK CompareGeneName (VoidPtr vp1, VoidPtr vp2)
{

	SortStructPtr sp1 = vp1;
	SortStructPtr sp2 = vp2;
	SeqFeatPtr sfp1, sfp2;
	GeneRefPtr gr1, gr2;
	Int2 status = 0;

	sfp1 = sp1->sfp;
	sfp2 = sp2->sfp;
	if (sfp1 == NULL || sfp2 == NULL) {
		return status;
	}
	if (sfp1->data.choice != SEQFEAT_GENE)
		return status;
	if (sfp2->data.choice != SEQFEAT_GENE)
		return status;
	gr1 = (GeneRefPtr) sfp1->data.value.ptrvalue;	
	gr2 = (GeneRefPtr) sfp2->data.value.ptrvalue;
	if (gr1 == NULL || gr2 == NULL)
		return status;
	status = StringCmp(gr1->locus, gr2->locus);

	return status;
}

/**************************************************************************
*	This function returns the gi number
*	If no gi number is found, -1 is returned  and a warning is
*	issued.
*	06-10-96
*	This fubction is changed to void. It will find NCBI gi and embl ni
*	and fill up gbp structure
**************************************************************************/

NLM_EXTERN void GetGINumber(GBEntryPtr gbp)
{
	Boolean 	found_gi;
	ValNodePtr 	vnp;
	Int4 		gi = -1;
	CharPtr 	ni = NULL;
    DbtagPtr	dbtag;
    ObjectIdPtr oid;

	found_gi = FALSE;
	if (gbp == NULL)
		return;
	if (gbp->bsp == NULL)
		return;
	for (vnp=gbp->bsp->id; vnp; vnp=vnp->next) {
		if (vnp->choice == SEQID_GI) {
			gi = vnp->data.intvalue;
			if (gi != 0) {
				found_gi = TRUE;
				break;
			} else {
				if (ASN2FF_SHOW_ERROR_MSG == TRUE) {
					ErrPostEx(SEV_WARNING, ERR_GI_No_GI_Number, 
									"Zero gi number: %d", gi);
				}
			}
		} else if (vnp->choice == SEQID_GENERAL) {
			dbtag = vnp->data.ptrvalue;
			if (StringCmp(dbtag->db, "NID") == 0) {
				oid = dbtag->tag;
				if (oid->str) {
					ni = StringSave(oid->str);
				}
			}
		}
	}
	if (! found_gi) {
		if (ASN2FF_SHOW_ERROR_MSG == TRUE) {
			ErrPostStr(SEV_WARNING, ERR_GI_No_GI_Number, "");
		}
		gi = -1;
	}
	gbp->gi = gi;
	gbp->ni = ni;
	return;
}

/***********************************************************************
*
*	GetGIs gets the GI's.
*
************************************************************************/
NLM_EXTERN void GetGIs (Asn2ffJobPtr ajp)
{
	GBEntryPtr gbp;
		
	for (gbp = ajp->asn2ffwep->gbp; gbp; gbp = gbp->next) {
		GetGINumber(gbp);
	}
	return;
}

NLM_EXTERN SeqIdPtr GetProductSeqId(ValNodePtr product)
{
	SeqIdPtr sip=NULL;
	SeqIntPtr seq_int;

	if (product)
	{
		if (product->choice == SEQLOC_WHOLE)
		{
			sip = (SeqIdPtr) product->data.ptrvalue;
		} 
		else if (product->choice == SEQLOC_INT)
		{
			seq_int = (SeqIntPtr) product->data.ptrvalue;
			sip = seq_int->id;
		}
	}
	return sip;
}

/*****************************************************************************
*check_range
*
*	This function is called by the gbparse functions of Karl Sirotkin 
*	and determines if the length of a BioSeqPtr is sensible.
*	Pointer data is not used !! Tatiana !!
*	Tom Madden
*****************************************************************************/

NLM_EXTERN Int4 check_range(Pointer data, SeqIdPtr seq_id)

{
	BioseqPtr bsp;

	bsp = BioseqFind(seq_id);
	if (bsp)
		return bsp->length;
	else
		return 0;
}	/* check_range */

/****************************************************************************
*do_loc_errors
*
*	This function is called both by the gbparse functions of Karl Sirotkin
*	and by asn2ff.  If called by gbparse, error messages are stored in 
*	buffers and a flag is set; if called by asn2ff, the error messages
*	are retrieved and the flag reset.
*	
***************************************************************************/

NLM_EXTERN void do_loc_errors(CharPtr front, CharPtr details)
{
	ErrPostEx(SEV_INFO, ERR_FEATURE_Bad_location, "%s: %s\n", front, details);
}

/***************************************************************************
*do_no_loc_errors
*
*	Is used when no error messages are wanted.
****************************************************************************/

NLM_EXTERN void do_no_loc_errors(CharPtr front, CharPtr details)
{
	return;
}

/***************************************************************************
*Boolean GBQualPresent(CharPtr ptr, GBQualPtr gbqual)
*
*This function check that a qual, that is to be added to the list of qual
*isn't already present.
***************************************************************************/
NLM_EXTERN Boolean GBQualPresent(CharPtr ptr, GBQualPtr gbqual)

{
	Boolean present=FALSE;
	GBQualPtr qual;

	for (qual=gbqual; qual; qual=qual->next)
		if (StringCmp(ptr, qual->qual) == 0)
		{
			present = TRUE;
			break;
		}

	return present;
}	/* GBQualPresent */

/**********************************************************************
*Boolean GetNAFeatKey(CharPtr buffer, SeqFeatPtr sfp)
*
*	This function places the sfp "key" in buffer and returns TRUE
*	if successful, it returns FALSE if not successful.
*	This function only works for nucleic acid sequences, as the
*	keys (for corresponding numbers) are different for peptides.
***********************************************************************/

NLM_EXTERN Boolean GetNAFeatKey(Boolean is_new, CharPtr PNTR buffer, SeqFeatPtr sfp, SeqFeatPtr sfp_out)
{

	Boolean retval=TRUE;
	ImpFeatPtr ifp;
	RnaRefPtr rrp;
	CharPtr str = NULL;
	Int2 index;


	switch (sfp->data.choice)
	{
	case SEQFEAT_GENE:	/* gene becomes misc_feat for purposes of CheckNAFeat */
		if (is_new) {
			*buffer = StringSave("gene");
		} else {
			*buffer = StringSave("misc_feature");
		}
		break;
	case SEQFEAT_CDREGION:
		*buffer = StringSave("CDS");
		break;
	case SEQFEAT_RNA:
		rrp = sfp->data.value.ptrvalue;
		/* the following code was taken (almost) directly from Karl
		Sirotkin's code.					*/
		switch ( rrp -> type){ 
			case 1:
				*buffer =StringSave("precursor_RNA");
				break;
			case 2:
				*buffer = StringSave("mRNA");
				break;
			case 3:
				*buffer = StringSave("tRNA");
				break;
			case 4:
				*buffer = StringSave("rRNA");
				break;
			case 5:
				*buffer = StringSave("snRNA");
				break;
			case 6:
				*buffer = StringSave("scRNA");
				break;
			case 7:
				*buffer = StringSave("misc_RNA"); /* snoRNA */
				break;
			case 255:
				*buffer = StringSave("misc_RNA");
				break;
		}
		switch ( rrp -> type){ 
			case 2:
			case 4:
			case 5:
			case 6:
			case 7:
			case 255:
				if (rrp ->ext.choice == 1 && sfp_out) {
					 str = rrp->ext.value.ptrvalue;
					 if (str != NULL && *str != '\0') {
					 	index = GBFeatKeyNameValid(buffer, FALSE);
					 	if (GBQualValidToAdd(index, "product")) {
							sfp_out->qual = AddGBQual(sfp_out->qual, 
									"product", str);
						}
					}
				}
				break;
		}
		break;
	case SEQFEAT_IMP:
		ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
		*buffer = StringSave(ifp->key); 
		break;
	case SEQFEAT_SEQ:
	case SEQFEAT_SITE:
	case SEQFEAT_REGION:
	case SEQFEAT_COMMENT:
		*buffer = StringSave("misc_feature");
		break;
	case SEQFEAT_BIOSRC:
		*buffer = StringSave("source");
		break;
	case SEQFEAT_RSITE:
	default:
		retval = FALSE;	
		break;
	}

	return retval;
}	/* GetNAFeatKey */

/**************************************************************************
*SeqIdPtr CheckXrefFeat (BioseqPtr bsp, SeqFeatPtr sfp)
*
* 	First the location of the xref is checked to see if it overlaps 
*	the sequence.  If this feature has a xref that is NOT of type 
*	genbank, embl, or ddbj, it is put out as a misc_feat.  If it's 
*	one of genbank, embl, or ddbj, it has been put out as a second 
*	accession.  If the feature should be put out as a misc_feat, then
*	the SeqIdPtr (xid) is returned, otherwise NULL.
**************************************************************************/

NLM_EXTERN SeqIdPtr CheckXrefFeat (BioseqPtr bsp, SeqFeatPtr sfp)
{

	SeqIdPtr xid=NULL;
	SeqIntPtr si;
	SeqLocPtr xref;
	ValNodePtr location;
			
	location = ValNodeNew(NULL);
	si = SeqIntNew();
	location->choice = SEQLOC_INT;
	location->data.ptrvalue = si;
	si->from = 0;
	si->to = bsp->length - 1;
	si->id = bsp->id;	/* Don't delete id!! */
	if (SeqLocCompare(sfp->location, location) != 0)
	{
		xref = (SeqLocPtr) sfp->data.value.ptrvalue;
		xid = (SeqIdPtr) xref->data.ptrvalue;
		if (xid->choice != 5 && xid->choice != 6 && xid->choice != 13)
			;
		else
			xid = NULL;
	}
	si->id = NULL;
	SeqIntFree(si);
	ValNodeFree(location);
	return xid;
}

NLM_EXTERN Int4 GetGINumFromSip (SeqIdPtr sip)
{
	Int4 gi = -1;
	ValNodePtr vnp;

	for (vnp=sip; vnp; vnp=vnp->next)
		if (vnp->choice == SEQID_GI)
			gi = vnp->data.intvalue;

	return gi;
}

/*****************************************************************************
*FlatRefBest
*
*	returns ValNodePtr to best (for FlatFile production) pub in a equiv set
*****************************************************************************/
NLM_EXTERN ValNodePtr FlatRefBest(ValNodePtr equiv, Boolean error_msgs, Boolean anything)
{
	ValNodePtr the_pub, retval = NULL, newpub;
	CitBookPtr cb;
	CitSubPtr cs;
	CitGenPtr cg;	
	CitArtPtr ca;
	MedlineEntryPtr ml;
	CitJourPtr jp;
	ImprintPtr ip;
	Boolean good_one;
	Int1 bad_one= 0;
	CharPtr str_ret;

	if (equiv->choice == PUB_Equiv) {
		newpub = equiv->data.ptrvalue;
	} else {
		newpub = equiv;
	}
	for (the_pub = newpub, good_one = FALSE; the_pub && ! good_one
			; the_pub = the_pub -> next) {

		switch ( the_pub -> choice) {

      case PUB_Sub:
         cs = (CitSubPtr) the_pub -> data.ptrvalue;
         if (cs) {
			if ( cs -> imp){
				ip = cs -> imp;
				if ( ip -> date) {
					retval = the_pub;
					good_one = TRUE; /* good for submitted */
				}
			} else if (cs->date) {
					retval = the_pub;
					good_one = TRUE; /* good for submitted */
			}
		}
         break;
		case PUB_Man:
		case PUB_Book:
			cb = (CitBookPtr) the_pub -> data.ptrvalue;
			if ( cb -> imp) {
				ip = cb -> imp;
				if ( ip -> date) {
					retval = the_pub;
					good_one = TRUE; /* good for thesis or book */
				}
			}
		break;
		case PUB_Patent:
			retval = the_pub;
			good_one = TRUE; /* might exclude later...*/
		break;
		case PUB_Article:
		case PUB_Medline:
			if ( the_pub -> choice == PUB_Medline) {
				ml = (MedlineEntryPtr) the_pub -> data.ptrvalue;
				ca = (CitArtPtr) ml -> cit;

			} else {
				ca = (CitArtPtr) the_pub -> data.ptrvalue;
			}
		if (ca -> fromptr) {
			if (ca -> from ==1) {
				jp = (CitJourPtr) ca -> fromptr;
				if ( jp -> imp) {
					ip = jp -> imp;
					if ( ip -> date) {
						retval = the_pub;
						good_one = TRUE; /* good as it gets */
					}
				}
			} else {
				CitBookPtr book = (CitBookPtr) ca -> fromptr;
					if ( book -> imp) {
						ip = book -> imp;
						if ( ip -> date) {
							retval = the_pub;
							good_one = TRUE; /* good for book */
						}
					}
				
			}
		}
			break;
		case PUB_Gen: 
			cg = (CitGenPtr) the_pub -> data.ptrvalue;
			if (cg -> cit) {
				str_ret = NULL;
				str_ret = StrStr(cg -> cit ,"Journal=\"");
				if ((str_ret) || (cg->title) || (cg->journal) || (cg->date)) {
					retval = the_pub;  /*unless something better */
				} else {
					if (StringNICmp("unpublished", cg->cit, 11) == 0)
						retval = the_pub;
					else if (StringNICmp("to be published", cg->cit, 15) == 0)
						retval = the_pub;
					else if (StringNICmp("in press", cg->cit, 8) == 0)
						retval = the_pub;
					else if (StringNICmp("submitted", cg->cit, 8) == 0)
						retval = the_pub;
				}
			} else if (cg -> journal) {
				retval = the_pub;  /*unless something better */
			}

			break;
		case PUB_Proc:
			bad_one = the_pub -> choice;
			break;
		}
	}

	if (! retval && anything) {
	   for (the_pub = newpub; the_pub; the_pub = the_pub -> next) {
		if (the_pub->choice == PUB_Muid)
			retval = the_pub;
	   }
	   if (! retval) /* Take anything left over now and hope for the best */
		retval = newpub;
	}

	if ( ! retval && bad_one != 0) {
		if (error_msgs == TRUE)
			ErrPostEx(SEV_WARNING, ERR_REFERENCE_Illegalreference,
			"FlatRefBest: Unimplemented pub type = %d", bad_one);
	}
	
	return retval;
}	/* FlatRefBest */

NLM_EXTERN Int4 StoreFeatTemp(SortStruct PNTR List, SeqFeatPtr sfp,
Int4 currentsize, BioseqPtr bsp, BioseqPtr seg, Uint2 entityID, Uint2 itemID, Uint2 itemtype,SeqLocPtr slp, SeqLocPtr PNTR extra_loc, Int2 extra_loc_cnt,
Boolean temp)
{
	SeqLocPtr PNTR slpp = NULL;
	
	List[currentsize].entityID = entityID;
	List[currentsize].itemID = itemID;
	List[currentsize].itemtype = itemtype;
	List[currentsize].sfp = sfp;
	List[currentsize].bsp = bsp;
	List[currentsize].seg_bsp = seg;
	List[currentsize].dup = FALSE;
	List[currentsize].hash = AsnIoHash(sfp, 
						(AsnWriteFunc) SeqFeatAsnWrite);
	List[currentsize].slp = slp;
	if (extra_loc_cnt > 0) {
		slpp = MemNew(extra_loc_cnt*(sizeof(SeqLocPtr)));
		MemCpy(slpp, extra_loc, extra_loc_cnt*(sizeof(SeqLocPtr)));
	} 
	List[currentsize].extra_loc = slpp;
	List[currentsize].extra_loc_cnt = extra_loc_cnt;
	List[currentsize].tempload = temp;
	List[currentsize].gsp = NULL;
	List[currentsize].nsp = NoteStructNew(List[currentsize].nsp);

	currentsize++;

	return currentsize;
}

NLM_EXTERN Int4 StoreFeat(SortStruct PNTR List, SeqFeatPtr sfp, Int4 currentsize, 
BioseqPtr bsp, BioseqPtr seg, Uint2 entityID, Uint2 itemID, Uint2 itemtype,
SeqLocPtr slp, SeqLocPtr PNTR extra_loc, Int2 extra_loc_cnt)
{
	return StoreFeatFree(List, sfp, currentsize, bsp, seg, entityID, itemID, 
					itemtype,slp, extra_loc, extra_loc_cnt, FALSE);
}

NLM_EXTERN Int4 StoreFeatFree(SortStruct PNTR List, SeqFeatPtr sfp, Int4 currentsize, 
BioseqPtr bsp, BioseqPtr seg, Uint2 entityID, Uint2 itemID, Uint2 itemtype,
SeqLocPtr slp, SeqLocPtr PNTR extra_loc, Int2 extra_loc_cnt, Boolean feat_free)
{
	SeqLocPtr PNTR slpp = NULL;
	
	List[currentsize].entityID = entityID;
	List[currentsize].itemID = itemID;
	List[currentsize].itemtype = itemtype;
	List[currentsize].sfp = sfp;
	List[currentsize].bsp = bsp;
	List[currentsize].seg_bsp = seg;
	List[currentsize].dup = FALSE;
	List[currentsize].hash = AsnIoHash(sfp, 
						(AsnWriteFunc) SeqFeatAsnWrite);
	List[currentsize].slp = slp;
	if (extra_loc_cnt > 0) {
		slpp = MemNew(extra_loc_cnt*(sizeof(SeqLocPtr)));
		MemCpy(slpp, extra_loc, extra_loc_cnt*(sizeof(SeqLocPtr)));
	} 
	List[currentsize].extra_loc = slpp;
	List[currentsize].extra_loc_cnt = extra_loc_cnt;
	List[currentsize].feat_free = feat_free;
	List[currentsize].gsp = NULL;
	List[currentsize].nsp = NoteStructNew(List[currentsize].nsp);

	currentsize++;

	return currentsize;
}
/****************************************************************************
*CharPtr Cat2Strings (CharPtr string1, CharPtr string2, CharPtr separator, Int2 num)
*
* Concatenates two strings (string1 and string2) and separates them by a
* "separator".  If num>0, takes num spaces off the end of string1 on
* concatenation; if num<0 takes all spaces off the end of the complete 
* string.
*****************************************************************************/
NLM_EXTERN CharPtr Cat2Strings (CharPtr string1, CharPtr string2, CharPtr separator, Int2 num)

{
	Boolean no_space=FALSE;
	Int4 length1=0, length2=0, length_sep=0, length_total;
	CharPtr newstring=NULL;

	if (num < 0)
	{
		num=0;
		no_space=TRUE;
	}

	if (string1 != NULL)
		length1 = StringLen(string1);
	if (string2 != NULL)
		length2 = StringLen(string2);
	if (separator != NULL)
		length_sep = StringLen(separator);

	length_total = length1+length2+length_sep-num+1;

	newstring = (CharPtr) MemNew(length_total*sizeof(Char));

	if (string1 != NULL)	
		newstring = StringCat(newstring, string1);
	if ((length1-num) >= 0)
		newstring[length1-num] = '\0';
	if (no_space && length1 > 0)
		while (length1 > 0 && newstring[length1-1] == ' ')
		{
			newstring[length1-1] = '\0';
			length1--;
		}
	if (separator != NULL)	
		newstring = StringCat(newstring, separator);
	if (string2 != NULL)	
		newstring = StringCat(newstring, string2);


	return newstring;
}

NLM_EXTERN GBQualPtr AddGBQualEx (CharPtr PNTR key, GBQualPtr gbqual, CharPtr qual, CharPtr val)
{
	Int2 index;
	
	index = GBFeatKeyNameValid(key, FALSE);
	if (GBQualValidToAdd(index,qual)) {
		return AddGBQual(gbqual, qual, val);
	}
	return gbqual;
}

/************************************************************************
*AddGBQual
*
*	This function makes a new GBQual and adds a "val" and a
*	a "qual".
*   doesn't add qual if it's already there /tatiana/
* 	doesn't add empty ("") val if qual is translation
***********************************************************************/
#ifdef ASN2GNBK_STRIP_NOTE_PERIODS
static Boolean IsEllipsis (
  CharPtr str
)

{
  size_t   len;
  CharPtr  ptr;

  if (StringHasNoText (str)) return FALSE;
  len = StringLen (str);
  if (len < 3) return FALSE;
  ptr = str + len - 3;
  return (Boolean) (ptr [0] == '.' && ptr [1] == '.' && ptr [2] == '.');
}
#endif

NLM_EXTERN GBQualPtr AddGBQual (GBQualPtr gbqual, CharPtr qual, CharPtr val)
{
	GBQualPtr curq, note = NULL;

	if (StringCmp(qual, "translation") == 0) {
		if (val == NULL) 
			return gbqual;
		if (*val == '\0')
			return gbqual;
	}
	if (gbqual) {
		if (CheckForQual(gbqual, qual, val) == 1) {
			return gbqual;
		}
		for (curq=gbqual; curq->next != NULL; curq=curq->next)
			continue;
		curq->next = GBQualNew();
		curq = curq->next;
		if (val)
			curq->val = StringSave(val);
		curq->qual = StringSave(qual);
		note = curq;
	} else {
		gbqual = GBQualNew();
		gbqual->next = NULL;
		if (val)
			gbqual->val = StringSave(val);
		gbqual->qual = StringSave(qual);
		note = gbqual;
	}

#ifdef ASN2GNBK_STRIP_NOTE_PERIODS
	if (note != NULL && StringICmp (qual, "note") == 0) {
		size_t len;
		CharPtr p, q;
		len = StringLen (note->val);
		if (len > 0 && note->val [len - 1] == '~') {
			note->val [len - 1] = '\0';
		}
		if (! IsEllipsis (note->val)) {
			len = StringLen (note->val);
			if (len > 0 && note->val [len - 1] == '.') {
 				note->val [len - 1] = '\0';
				if (len > 1 && note->val [len - 2] == '.') {
 					note->val [len - 2] = '\0';
				}
			}
		}
		TrimSpacesAndJunkFromEnds (note->val,TRUE);
		TrimSpacesAndSemicolons (note->val);
		p = note->val;
		q = note->val;
		while (*p) {
		  if (*p == ';' && p [1] == ' ' && p [2] == ';') {
		    p += 2;
		  } else {
		    *q = *p;
		    p++;
		    q++;
		  }
		}
		*q = '\0';
	}
#endif

	return gbqual;
}

/****************************************************************************
*	Int2 CheckForQual(GBQualPtr gbqual, CharPtr string_q, CharPtr string_v)
*
*	Compares string (a potential gbqual->val) against all gbquals.
*	If a match is found, "1" is returned; if not "0".
****************************************************************************/

NLM_EXTERN Int2 CheckForQual (GBQualPtr gbqual, CharPtr string_q, CharPtr string_v)
{
	GBQualPtr curq;

	for (curq=gbqual; curq; curq=curq->next) {
		if (StringCmp(string_q, curq->qual) == 0) {
			if (curq->val == NULL) {
				curq->val = StringSave(string_v);
				return 1;
			} 
			if (StringCmp(string_v, curq->val) == 0) {
				return 1;
			}
		}
	}
	return 0;
}


/****************************************************************************
*
*	MakeAnAccession is for last ditch efforts to get an accession
*	after all the normal things have failed.
*
****************************************************************************/

NLM_EXTERN CharPtr MakeAnAccession (CharPtr new_buf, SeqIdPtr seq_id, Int2 buflen)
{
	SeqIdPtr new_id;

	new_id = SeqIdFindBest(seq_id, SEQID_GENBANK);
	SeqIdWrite(new_id, new_buf, PRINTID_TEXTID_ACCESSION, buflen);
	return new_buf;

}

NLM_EXTERN CharPtr GetGBSourceLine (GBBlockPtr gb)
{
	CharPtr source = NULL;

	if(gb && gb->source)
		source = StringSave(gb->source);

#ifdef ASN2GNBK_STRIP_NOTE_PERIODS
	if (source != NULL) {
		TrimSpacesAndJunkFromEnds (source,TRUE);
	}
#endif
	return source;
}

NLM_EXTERN CharPtr FlatOrganelle(Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	CharPtr retval = NULL;
	ValNodePtr man, vnp=NULL;
	static char * organelle_names [] = {
		 "Mitochondrion " ,
    "Chloroplast " ,
    "Kinetoplast ",
    "Cyanelle "};
	BioSourcePtr biosp=NULL;
/*	
	static CharPtr genome[] = {
	NULL, NULL, "Chloroplast ", "Chromoplast ", "Kinetoplast ", "Mitochondrion ", "Plastid ", "Macronuclear ", "Extrachrom ", "Plasmid ", NULL, NULL, "Cyanelle ", "Proviral ", "Virion ", "Nucleomorph ", "Apicoplast ", "Leucoplast ", "Proplastid "};
*/	
	static CharPtr genome[] = {
	NULL, NULL, "Chloroplast ", "Chromoplast ", "Kinetoplast ", "Mitochondrion ", "Plastid ", NULL, NULL, NULL, NULL, NULL, "Cyanelle ", NULL, NULL, "Nucleomorph ", "Apicoplast ", "Leucoplast ", "Proplastid ", NULL};
	
/* try new first */
	if ((vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_source)) != NULL) 
	{
		biosp = vnp->data.ptrvalue;
	/*	if (biosp->genome < 6 || biosp->genome > 12)*/
			retval = StringSave(genome[biosp->genome]);
	}
/* old next */
	if (biosp == NULL) {
		if ((vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_modif)) != NULL) 
		{
			for (man = (ValNodePtr) vnp-> data.ptrvalue; man; man = man -> next)
			{
				switch (man -> data.intvalue){
					case 4: case 5: case 6: case 7:
					if (! retval )
						retval = StringSave(organelle_names
								[man->data.intvalue-4]);
						break;
					default:
						break;
					}
			}
		}
	}
	return retval;
}

NLM_EXTERN Int4 GetNumOfSeqBlks (Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
        Int4 length, num_of_seqblks;

        length = BioseqGetLen(gbp->bsp);
		if (ajp->slp) {
			length = SeqLocLen(ajp->slp);
		}
        num_of_seqblks = ROUNDUP(length, SEQ_BLK_SIZE)/SEQ_BLK_SIZE;

        return num_of_seqblks;
}


/*************************************************************************
*	New asn.1 spec - division is in Orgname.div
*	check MolInfo.tech 
*	check GBBlock for PAT or SYN
*	get division from Orgname.div (in BioSource)
*	09-05-96
*************************************************************************/

static void IndexedGetDescrForDiv (BioseqPtr bsp, DivStructPtr PNTR dspp)

{
	SeqMgrDescContext context;
	ValNodePtr tmp;
	DivStructPtr	dsp;
	BioSourcePtr bsr;
	MolInfoPtr mol;
	CharPtr gb_div=NULL;
	GBBlockPtr gb;

	dsp = *dspp;
	tmp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &context);
			if (tmp != NULL) {
				if (tmp->data.ptrvalue != NULL) {
					mol = (MolInfoPtr) tmp->data.ptrvalue;
					if (mol->tech != 0) {
						if (dsp->tech == 0) {
							dsp->tech = mol->tech;
						} else if (mol->tech != dsp->tech) {
							dsp->was_err = TRUE;
							if (dsp->err_post) {
								ErrPostEx(SEV_WARNING, 0, 0, 
								"Different Molinfo in one entry: %d|%d", 
									mol->tech, dsp->tech);
							}
							dsp->tech = mol->tech;
						}
						dsp->techID = context.itemID;
						dsp->techtype = OBJ_SEQDESC;
						*dspp = dsp;
					}
				}
			}

	tmp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_source, &context);
			while (tmp != NULL && dsp->orgdiv == NULL) {
				bsr = (BioSourcePtr) tmp->data.ptrvalue;
				if (bsr && bsr->org) {
					if (bsr->org->orgname && bsr->org->orgname->div) {
						gb_div = bsr->org->orgname->div;
						if (dsp->orgdiv == NULL) {
							dsp->orgdiv = gb_div;
						} else if (StringCmp(gb_div, dsp->orgdiv) != 0) {
							dsp->was_err = TRUE;
							if (dsp->err_post) {
								ErrPostEx(SEV_WARNING, 0, 0, 
							"Different Taxonomy divisions in one entry: %s|%s", 
									gb_div, dsp->orgdiv);
							}
							dsp->orgdiv = gb_div;
						}
						dsp->biosrc = bsr;
						dsp->orgID = context.itemID;
						dsp->orgtype = OBJ_SEQDESC;
						*dspp = dsp;
					}
				}
				tmp = SeqMgrGetNextDescriptor (bsp, tmp, Seq_descr_source, &context);
			}

	tmp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_genbank, &context);
			if (tmp != NULL) {
				gb = (GBBlockPtr) tmp->data.ptrvalue;
				if (gb->div) {
					gb_div = gb->div;
					if (dsp->gbdiv == NULL) {
						dsp->gbdiv = gb_div;
					} else if (StringCmp(gb_div, dsp->gbdiv) != 0) {
						dsp->was_err = TRUE;
						if (dsp->err_post) {
							ErrPostEx(SEV_WARNING, 0, 0, 
							"Different GBBlock divisions in one entry: %s|%s", 
								gb_div, dsp->gbdiv);
						}
						dsp->gbdiv = gb_div;
					}
					dsp->gbID = context.itemID;
					dsp->gbtype = OBJ_SEQDESC;
					*dspp = dsp;
				}
			}
}

static Boolean GetDescrForDiv (GatherContextPtr gcp)
{
/* find only one (closest to the target!) vnp with given choice */
	ValNodePtr	tmp;
	DivStructPtr	PNTR dspp;
	DivStructPtr	dsp;
	BioSourcePtr bsr;
	MolInfoPtr mol;
	CharPtr gb_div=NULL;
	GBBlockPtr gb;
	
	dspp = gcp->userdata;
	dsp = *dspp;
	switch (gcp->thistype)
	{
		case OBJ_SEQDESC:
			tmp = (ValNodePtr) (gcp->thisitem);
			if (tmp->choice == Seq_descr_molinfo) {
				if (tmp->data.ptrvalue != NULL) {
					mol = (MolInfoPtr) tmp->data.ptrvalue;
					if (mol->tech != 0) {
						if (dsp->tech == 0) {
							dsp->tech = mol->tech;
						} else if (mol->tech != dsp->tech) {
							dsp->was_err = TRUE;
							if (dsp->err_post) {
								ErrPostEx(SEV_WARNING, 0, 0, 
								"Different Molinfo in one entry: %d|%d", 
									mol->tech, dsp->tech);
							}
							dsp->tech = mol->tech;
						}
						dsp->techID = gcp->itemID;
						dsp->techtype = gcp->thistype;
						*dspp = dsp;
					}
				}
			} else if (tmp->choice == Seq_descr_source) {
				bsr = (BioSourcePtr) tmp->data.ptrvalue;
				if (bsr && bsr->org) {
					if (bsr->org->orgname && bsr->org->orgname->div) {
						gb_div = bsr->org->orgname->div;
						if (dsp->orgdiv == NULL) {
							dsp->orgdiv = gb_div;
						} else if (StringCmp(gb_div, dsp->orgdiv) != 0) {
							dsp->was_err = TRUE;
							if (dsp->err_post) {
								ErrPostEx(SEV_WARNING, 0, 0, 
							"Different Taxonomy divisions in one entry: %s|%s", 
									gb_div, dsp->orgdiv);
							}
							dsp->orgdiv = gb_div;
						}
						dsp->biosrc = bsr;
						dsp->orgID = gcp->itemID;
						dsp->orgtype = gcp->thistype;
						*dspp = dsp;
					}
				}
			} else if (tmp->choice == Seq_descr_genbank) {
				gb = (GBBlockPtr) tmp->data.ptrvalue;
				if (gb->div) {
					gb_div = gb->div;
					if (dsp->gbdiv == NULL) {
						dsp->gbdiv = gb_div;
					} else if (StringCmp(gb_div, dsp->gbdiv) != 0) {
						dsp->was_err = TRUE;
						if (dsp->err_post) {
							ErrPostEx(SEV_WARNING, 0, 0, 
							"Different GBBlock divisions in one entry: %s|%s", 
								gb_div, dsp->gbdiv);
						}
						dsp->gbdiv = gb_div;
					}
					dsp->gbID = gcp->itemID;
					dsp->gbtype = gcp->thistype;
					*dspp = dsp;
				}
			}			
			break;
		default:
			break;
	}
	return TRUE;
}

/**************************************************************************
*	0 - nothing found
*	1 - return division code OK
*	2 - return division code but errors were found
**************************************************************************/
static Int2 BioseqGetGBDivCodeEx (BioseqPtr bsp, CharPtr buf, Int2 buflen, Boolean err_post, Boolean useFeatureIndexing)
{
	GatherScope gsc;
	SeqLocPtr slp = NULL;
	Uint2 bspID;
	DivStructPtr dsp;
	BioSourcePtr bsr = NULL;
	Int2 tech, /*UNUSED*/diff, retval = 0;
	CharPtr orgdiv, gbdiv;
	SeqIdPtr sip;

	if (buf == NULL)
		return 0;
	*buf = '\0';
/* check for Patent SeqId  */
	for (sip = bsp->id; sip; sip=sip->next) {
		if (sip->choice == SEQID_PATENT) {
			diff = LabelCopy(buf, "PAT", buflen);
			return 1;
		}
	}
	bspID = ObjMgrGetEntityIDForPointer(bsp);
	dsp = MemNew(sizeof(DivStruct));
	dsp->err_post = err_post;
	dsp->entityID = bspID;
	dsp->tech = 0;
	dsp->gbdiv = NULL;
	dsp->orgdiv = NULL;
	dsp->biosrc = NULL;
	dsp->was_err = FALSE;
  	MemSet ((Pointer) (&gsc), 0, sizeof (GatherScope));
	MemSet ((Pointer) (gsc.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
	gsc.ignore[OBJ_SEQDESC] = FALSE;
	slp = ValNodeNew(NULL);
	slp->choice = SEQLOC_WHOLE;
	slp->data.ptrvalue = (SeqIdPtr) SeqIdDup (SeqIdFindBest (bsp->id, 0));
	gsc.target = slp;

	if (useFeatureIndexing) {
		IndexedGetDescrForDiv (bsp, &dsp);
	} else {
		GatherEntity(bspID, &dsp, GetDescrForDiv, &gsc);
	}
	
	SeqLocFree(slp);
	orgdiv = dsp->orgdiv;
	gbdiv = dsp->gbdiv;
	tech = dsp->tech;
	bsr = dsp->biosrc;
	if (dsp->was_err) {
		retval = 2;
	} else {
		retval = 1;
	}
	MemFree(dsp);
	switch (tech) {
		case MI_TECH_est: 
			diff = LabelCopy(buf, "EST", buflen);
		break;
		case MI_TECH_sts:  /* Sequence Tagged Site */
			diff = LabelCopy(buf, "STS", buflen);
		break;
		case MI_TECH_survey:
			diff = LabelCopy(buf, "GSS", buflen);
		break;
		case MI_TECH_htc:
			diff = LabelCopy(buf, "HTC", buflen);
		break;
		case MI_TECH_htgs_0:
		case MI_TECH_htgs_1:
		case MI_TECH_htgs_2:
			diff = LabelCopy(buf, "HTG", buflen);
		break;
		default:
		break;
	}
	if (*buf != '\0') {
		return retval;
	}
/*  new slot for synthetic sequences */	
	if (bsr && bsr->origin == 5) {
		diff = LabelCopy(buf, "SYN", buflen);
		return retval;
	}
/***** division in GBBlock becomes obsolete  ********/	
	if (gbdiv != NULL) {
		if (StringCmp(gbdiv, "PAT") == 0 || 
					StringCmp(gbdiv, "SYN") == 0 || orgdiv == NULL) {
			diff = LabelCopy(buf, gbdiv, buflen);
		return retval;
		}
	}
	
/**********/	
	if (orgdiv != NULL) {
		diff = LabelCopy(buf, orgdiv, buflen);
		return retval;
	}
	return 0;
}

NLM_EXTERN Int2 BioseqGetGBDivCode(BioseqPtr bsp, CharPtr buf, Int2 buflen, Boolean err_post)

{
	return BioseqGetGBDivCodeEx (bsp, buf, buflen, err_post, FALSE);
}


/*============================================================================*\
 * Function:
 *	StrStripSpaces
 *
 * Purpose:
 *	Strips all spaces in string in following manner. If the function
 *	meet several spaces (spaces and tabs) in succession it replaces them
 *	with one space.
 *	Strips all spaces after '(' and before ')'
 *
\*----------------------------------------------------------------------------*/
static void StrStripSpaces(CharPtr str)
{
	CharPtr	new_str;

	if (str == NULL) {
		return;
	}

	new_str = str;
	while (*str != '\0') {
		*new_str++ = *str;
		if (*str == ' ' || *str == '\t' || *str == '(') {
			for (str++; *str == ' ' || *str == '\t'; str++) ;
			if (*str == ')' || *str == ',') {
				new_str--;
			}
		} else {
			str++;
		}
	}
	*new_str = '\0';
}

static CharPtr GetFlatRetract(ValNodePtr pub)
{
	CitArtPtr cit;
	CitJourPtr jour = NULL;
	CitRetractPtr ret = NULL;
	CharPtr buffer;
	Int2 len;
	
	if (pub == NULL)
		return NULL;
	if (pub->choice != PUB_Article)
		return NULL;
	cit = pub->data.ptrvalue;
	if (cit->from == 1) {
		jour = cit->fromptr;
		if (jour && jour->imp) {
			ret = jour->imp->retract;
			if (ret && ret->type == 3) { /* other types can be added later */
				len = StringLen(ret->exp) + 11;
				buffer = (CharPtr) MemNew(len*sizeof(Char));
				sprintf(buffer, "Erratum:[%s]", ret->exp);
				return buffer;
			}
		}
	}
	return NULL;
}

static CharPtr GetSubmitDescr(ValNodePtr pub)
{
	CitSubPtr cs;
	
	if (pub == NULL) {
		return NULL;
	}
	if (pub->choice != PUB_Sub) {
		return NULL;
	}
	cs = (CitSubPtr) pub->data.ptrvalue;
	if (cs->descr == NULL) {
		return NULL;
	}
	return (StringSave(cs->descr));
}

static Int4 GetMuid(ValNodePtr equiv)
{
	Int4 muid=0;
	ValNodePtr newpub, the_pub;
	MedlineEntryPtr ml;
	
	if (equiv->choice == PUB_Equiv)
		newpub = equiv->data.ptrvalue;
	else
		newpub = equiv;

	for (the_pub = newpub; the_pub; the_pub = the_pub -> next) {
		if (the_pub->choice == PUB_Muid) {
			muid = the_pub->data.intvalue;
			break;
		}
		if (the_pub->choice == PUB_Medline) {
			ml = (MedlineEntryPtr) the_pub -> data.ptrvalue;
			muid = ml->uid;
		}
	}

	return muid;

}	/* GetMuid */

static Int4 GetPmid(ValNodePtr equiv)
{
	Int4 pmid=0;
	ValNodePtr newpub, the_pub;
	MedlineEntryPtr ml;
	
	if (equiv->choice == PUB_Equiv)
		newpub = equiv->data.ptrvalue;
	else
		newpub = equiv;

	for (the_pub = newpub; the_pub; the_pub = the_pub -> next) {
		if (the_pub->choice == PUB_PMid) {
			pmid = the_pub->data.intvalue;
			break;
		}
		if (the_pub->choice == PUB_Medline) {
			ml = (MedlineEntryPtr) the_pub -> data.ptrvalue;
			pmid = ml->pmid;
		}
	}

	return pmid;

}	/* GetPmid */

/***************************************************************************
* SeqLocPtr GetBaseRangeForCitation (SeqLocPtr loc, SeqLocPtr slp, Int4Ptr start, Int4Ptr stop)
*
*
*	This function finds the start and stop Int4 values for a location.
*	If this is a cmplex location (e.g., SEQLOC_MIX), then the
*	function is called several times, with the returned slp used
*	as an argument on the next round.
*	
*	The first call should be with slp set to NULL.
*
****************************************************************************/

static SeqLocPtr GetBaseRangeForCitation (SeqLocPtr loc, SeqLocPtr slp, Int4Ptr start, Int4Ptr stop)
{
	Int4 tmp_start, tmp_stop, tmp_range;
	
	*start = 0;
	*stop = 0;

	switch (loc->choice)
	{
		case SEQLOC_BOND:   
       		case SEQLOC_FEAT:   
        	case SEQLOC_NULL:    
        	case SEQLOC_EMPTY:  
			slp = NULL;
                        break;
        	case SEQLOC_WHOLE:
        	case SEQLOC_INT:
			if ((tmp_start = SeqLocStart(loc)) >= 0  &&
					(tmp_stop = SeqLocStop(loc)) >= 0)
			{
				tmp_range = tmp_stop - tmp_start;
				if (tmp_range >= 0)
				{ /* +1 for Genbank format. */
					*start = tmp_start+1;
					*stop = tmp_stop+1;
				}
			}
			slp = NULL;
			break;
        	case SEQLOC_MIX:
        	case SEQLOC_EQUIV:
        	case SEQLOC_PACKED_INT:
			if (slp == NULL)
				slp = loc->data.ptrvalue;
			if (slp != NULL)
			{
				if ((tmp_start = SeqLocStart(slp)) >= 0  &&
						(tmp_stop = SeqLocStop(slp)) >= 0)
				{
					tmp_range = tmp_stop - tmp_start;
					if (tmp_range >= 0)
					{ /* +1 for Genbank format. */
						*start = tmp_start+1;
						*stop = tmp_stop+1;
					}
				}
				slp = slp->next;
			}
			break;
        	case SEQLOC_PACKED_PNT: 
       		case SEQLOC_PNT:
			slp = NULL;
			break;
		default:
			slp = NULL;
			break;
	}
	return slp;
}

/*************************************************************************
*GB_PrintPubs
*
*	"GB_PrintPubs" to dump pubs in Flat File (i.e., Genbank) format.
*
**************************************************************************/

void GB_PrintPubs (Asn2ffJobPtr ajp, GBEntryPtr gbp, PubStructPtr psp)

{

	BioseqPtr bsp=gbp->bsp;
	Boolean first_time, ignore_this=FALSE, submit=FALSE, tag;
	Char buffer[150];
	CharPtr authors=NULL,title=NULL,journal=NULL,string_start, string, retract;
	CharPtr descr = NULL;
	Int2 i;
	Int4 gibbsq, muid, pmid, pat_seqid=0, start=0, stop=0;
	PubdescPtr pdp;
	SeqFeatPtr sfp;
	SeqLocPtr loc, slp;
	ValNodePtr pub;

	if (ASN2FF_SHOW_ALL_PUBS) {
		pub = FlatRefBest(psp->pub, ajp->error_msgs, TRUE);
	} else {
		pub = FlatRefBest(psp->pub, ajp->error_msgs, FALSE);
	}
	if (pub == NULL)
	{
		if (ajp->error_msgs == TRUE)
			PostARefErrMessage (ajp, bsp, psp, NULL, -1, NULL);
		return;
	}
	ignore_this = FlatIgnoreThisPatentPub(bsp, pub, &pat_seqid);
	if (ajp->format != GENPEPT_FMT)
	{
		if (ignore_this == TRUE)
		{
			if (ajp->error_msgs == TRUE)
				PostARefErrMessage (ajp, bsp, psp, NULL, -1, NULL);
			return;
		}
	}

	ff_StartPrint(0, 12, ASN2FF_GB_MAX, NULL);
	ff_AddString("REFERENCE");
	TabToColumn(13);
	ff_AddInteger("%ld", (long) psp->number);
	if (psp->start == 1) {
		TabToColumn(16);
		if (psp->descr != NULL) {
			if (psp->descr->reftype != 0) {
				ff_AddString("(sites)");
			} else {
				if (ajp->format != GENPEPT_FMT) {
					ff_AddString("(bases ");
				} else {
					ff_AddString("(residues ");
				}
				if (ajp->slp) {
					ff_AddInteger("%ld", (long) (SeqLocStart(ajp->slp) + 1));
					ff_AddString(" to "); 
					ff_AddInteger("%ld", (long) (SeqLocStop(ajp->slp) + 1));
				} else {
					ff_AddString("1 to ");
					ff_AddInteger("%ld", (long) bsp->length);
				}
				ff_AddChar(')');
			}
		}
	}
	else if (psp->start == 2) {
		TabToColumn(16);
		if (ajp->format != GENPEPT_FMT)
			ff_AddString("(bases ");
		else
			ff_AddString("(residues ");
		for (i=0; i<psp->citcount; i++) {
			sfp = psp->citfeat[i];
			loc = (SeqLocPtr) sfp->location;
			slp = GetBaseRangeForCitation (loc, NULL, &start, &stop);
			if (start != 0 || stop != 0) {
				ff_AddInteger("%ld", (long) start);
				ff_AddString(" to ");
				ff_AddInteger("%ld", (long) stop);
				if (slp != NULL || i+1 != psp->citcount)
					ff_AddString("; ");
			}
			while (slp != NULL) {
				slp = GetBaseRangeForCitation (loc, slp, &start, &stop);
				if (start != 0 || stop != 0) {
					ff_AddInteger("%ld", (long) start);
					ff_AddString(" to ");
					ff_AddInteger("%ld", (long) stop);
					if (slp != NULL || i+1 != psp->citcount)
						ff_AddString("; ");
				}
			}
		}
		ff_AddChar(')');
	} else if (psp->start == 3) {
		TabToColumn(16);
		ff_AddString("(sites)");
	} else {
		if (ajp->error_msgs == TRUE)
			ErrPostEx(SEV_WARNING, CTX_NCBI2GB, 1, 
			 "Incorrect start value (%d) in PubStruct\n", psp->start);
	}
	ff_EndPrint();

	authors = FlatAuthor(ajp, pub);
	ff_StartPrint(2, 12, ASN2FF_GB_MAX, NULL);
	ff_AddString("AUTHORS");
	TabToColumn(13);

	if (authors && *authors != NULLB) {
		ff_AddString(authors);
	} else {
		ff_AddChar('.');
	}
	ff_EndPrint();

	title = FlatPubTitle(pub);
	if (title ) {
		if ( *title  != NULLB) {
			ff_StartPrint(2, 12, ASN2FF_GB_MAX, NULL);
			ff_AddString("TITLE");
			TabToColumn(13);
			StrStripSpaces(title);
			ff_AddString(title);
			ff_EndPrint();
		}
	}

	journal = FlatJournal(ajp, gbp, pub, pat_seqid, &submit, FALSE);
	ff_StartPrint(2, 12, ASN2FF_GB_MAX, NULL);
	ff_AddString("JOURNAL");
	TabToColumn(13);
	if (journal ) {
#ifdef ASN2GNBK_STRIP_NOTE_PERIODS
		CharPtr p, q;
		p = journal;
		q = journal;
		while (*p) {
		  if (*p == ',' && p [1] == ' ' && p [2] == ';') {
		    p += 2;
		  } else {
		    *q = *p;
		    p++;
		    q++;
		  }
		}
		*q = '\0';
#endif
		StrStripSpaces(journal);
		ff_AddString(journal);
	} else {
		ff_AddString("Unpublished");
	}
	ff_EndPrint();

	muid = GetMuid(psp->pub);
	if (muid > 0) {
		ff_StartPrint(2, 12, ASN2FF_GB_MAX, NULL);
		ff_AddString("MEDLINE");
		TabToColumn(13);
		www_muid(muid);
		ff_EndPrint();
	}
	pmid = GetPmid (psp->pub);
	if (pmid > 0) {
		ff_StartPrint(3, 12, ASN2FF_GB_MAX, NULL);
		ff_AddString("PUBMED");
		TabToColumn(13);
		www_muid(pmid);
		ff_EndPrint();
	}

	tag = FALSE;
	pdp = psp->descr;
	if (pdp != NULL && pdp->comment != NULL) {
		if (StringCmp(pdp->comment, "full automatic") != 0 &&
		  StringCmp(pdp->comment, "full staff_review") != 0 &&
		   StringCmp(pdp->comment, "full staff_entry") != 0 &&
		    StringCmp(pdp->comment, "simple staff_review") != 0 &&
		      StringCmp(pdp->comment, "simple staff_entry") != 0 &&
		       StringCmp(pdp->comment, "simple automatic") != 0 &&
		        StringCmp(pdp->comment, "unannotated automatic") != 0 &&
		         StringCmp(pdp->comment, "unannotated staff_review") != 0 &&
		          StringCmp(pdp->comment, "unannotated staff_entry") != 0)
		{
			ff_StartPrint(2, 12, ASN2FF_GB_MAX, NULL);
			ff_AddString("REMARK  ");
			TabToColumn(13);
			ff_AddStringWithTildes(pdp->comment);
			tag = TRUE;
		}
	}
	string = &buffer[0];
	gibbsq = GetGibbsqStatement(gbp, string);
	if (gibbsq > 0) {
		if (tag != TRUE) {
			ff_StartPrint(2, 12, ASN2FF_GB_MAX, NULL);
			ff_AddString("REMARK");
			TabToColumn(13);
		} else {
			NewContLine();
		}
		ff_AddStringWithTildes(string);
		tag = TRUE;
	}
	string = GetGibbsqComment(gbp);
	if (string) {
		string_start = string;
		if (tag != TRUE) {
			ff_StartPrint(2, 12, ASN2FF_GB_MAX, NULL);
			ff_AddString("REMARK");
			TabToColumn(13);
		} else {
			NewContLine();
		}
		first_time = TRUE;
/* Can't this be rewritten to use ff_AddString????  That would be faster! */
		while (*string != '\0') {
			if (*string == '~') {
				if (first_time == FALSE)
					NewContLine();
				else
					first_time = FALSE;
			} else if (*string == '\"') {
				*string = '\'';
				ff_AddChar(*string);
			} else {
				ff_AddChar(*string);
			}
			string++;
		}
		string_start = MemFree(string_start);
		tag=TRUE;
	}
	retract = GetFlatRetract(pub);
	if (retract) {
		if (tag != TRUE) {
			ff_StartPrint(2, 12, ASN2FF_GB_MAX, NULL);
			ff_AddString("REMARK");
			TabToColumn(13);
		} else {
			NewContLine();
		}
		ff_AddStringWithTildes(retract);
		tag = TRUE;
		MemFree(retract);
	}
	descr = GetSubmitDescr(pub);
	if (descr) {
		if (tag != TRUE) {
			ff_StartPrint(2, 12, ASN2FF_GB_MAX, NULL);
			ff_AddString("REMARK");
			TabToColumn(13);
		} else {
			NewContLine();
		}
		ff_AddStringWithTildes(descr);
		tag = TRUE;
	}
	if (tag == TRUE)
		ff_EndPrint();

	if (authors)
		MemFree(authors);

        MemFree(descr);
	MemFree(title);
	MemFree(journal);
}	/* GB_PrintPubs */

/*************************************************************************
*GR_PrintPubs
*
*	"GR_PrintPubs" to dump pubs in Flat File (i.e., Genbank) format.
*
**************************************************************************/

void GR_PrintPubs (Asn2ffJobPtr ajp, GBEntryPtr gbp, PubStructPtr psp)

{

	BioseqPtr bsp=gbp->bsp;
	Boolean first_time, ignore_this=FALSE, submit=FALSE, tag;
	Char buffer[150];
	CharPtr authors=NULL,title=NULL,journal=NULL,string_start, string, retract;
	CharPtr descr = NULL;
	Int2 i;
	Int4 gibbsq, muid, pmid, pat_seqid=0, start=0, stop=0;
	PubdescPtr pdp;
	SeqFeatPtr sfp;
	SeqLocPtr loc, slp;
	ValNodePtr pub;

	if (ASN2FF_SHOW_ALL_PUBS) {
		pub = FlatRefBest(psp->pub, ajp->error_msgs, TRUE);
	} else {
		pub = FlatRefBest(psp->pub, ajp->error_msgs, FALSE);
	}
	if (pub == NULL)
	{
		if (ajp->error_msgs == TRUE)
			PostARefErrMessage (ajp, bsp, psp, NULL, -1, NULL);
		return;
	}
	ignore_this = FlatIgnoreThisPatentPub(bsp, pub, &pat_seqid);
	if (ajp->format != GENPEPT_FMT)
	{
		if (ignore_this == TRUE)
		{
			if (ajp->error_msgs == TRUE)
				PostARefErrMessage (ajp, bsp, psp, NULL, -1, NULL);
			return;
		}
	}

	ff_StartPrint(0, 12, ASN2FF_GB_MAX, NULL);
	ff_AddString("<BR><BR>");
	title = FlatPubTitle(pub);
	if (title ) {
		if ( *title  != NULLB) {
			StrStripSpaces(title);
			ff_AddString("<B>");
			ff_AddString(title);
			ff_AddString("</B>");
			ff_EndPrint();
		}
	}
	authors = FlatAuthor(ajp, pub);

	if (authors && *authors != NULLB) {
		ff_AddString("<BR>");
		ff_AddString(authors);
	} else {
		ff_AddChar('.');
	}
	ff_EndPrint();


	journal = FlatJournal(ajp, gbp, pub, pat_seqid, &submit, FALSE);
	ff_StartPrint(2, 12, ASN2FF_GB_MAX, NULL);
	ff_AddString("<BR>");
	if (journal ) {
		StrStripSpaces(journal);
		ff_AddString(journal);
	} else {
		ff_AddString("Unpublished");
	}
	ff_EndPrint();

	muid = GetMuid(psp->pub);
	if (muid > 0) {
		ff_StartPrint(2, 12, ASN2FF_GB_MAX, NULL);
		ff_AddString("<BR>");
		TabToColumn(13);
		www_muid(muid);
		ff_EndPrint();
	}
	pmid = GetPmid (psp->pub); /* not sure what GR format should be generating */
	/*
	if (pmid > 0) {
		ff_StartPrint(3, 12, ASN2FF_GB_MAX, NULL);
		ff_AddString("<BR>");
		TabToColumn(13);
		www_muid(pmid);
		ff_EndPrint();
	}
	*/


	if (authors)
		MemFree(authors);

	MemFree(title);
	MemFree(journal);
	
}	/* GR_PrintPubs */

/*************************************************************************
*EMBL_PrintPubs
*
*	"EMBL_PrintPubs" to dump pubs in FlatFile (EMBL) format.
*
**************************************************************************/

void EMBL_PrintPubs (Asn2ffJobPtr ajp, GBEntryPtr gbp, PubStructPtr psp)

{

	BioseqPtr bsp=gbp->bsp;
	Boolean ignore_this=FALSE, submit=FALSE;
	CharPtr authors=NULL, title=NULL, journal=NULL, new_journal;
	Int2 i;
	Int4 pat_seqid=0;	
	Int4 start=0, stop=0, tmp_range, range;
	PubdescPtr descr=psp->descr;
	SeqFeatPtr sfp;
	SeqLocPtr loc, slp;
	ValNodePtr pub;
	Int4 muid;
	Char s[15];

	pub = FlatRefBest(psp->pub, ajp->error_msgs, FALSE);
	if (pub == NULL)
	{
		if (ajp->error_msgs == TRUE)
			ErrPostStr(SEV_WARNING, ERR_REFERENCE_Illegalreference, "FFDumpPubs: Invalid Pub found.");
		return;
	}
	ignore_this = FlatIgnoreThisPatentPub(bsp, pub, &pat_seqid);
	if (ignore_this == TRUE && ASN2FF_IGNORE_PATENT_PUBS != FALSE)
	{
		if (ajp->error_msgs == TRUE)
			ErrPostStr(SEV_WARNING, ERR_REFERENCE_Illegalreference, "FFDumpPubs: Invalid Patent Pub");
		return;
	}

	PrintXX();

	ff_StartPrint(5, 5, ASN2FF_EMBL_MAX, "RN");
	ff_AddChar('[');
	ff_AddInteger("%ld", (long) psp->number);
	ff_AddChar(']');
	ff_EndPrint();
	if (psp->start == 1)
	{
		ff_StartPrint(5, 5, ASN2FF_EMBL_MAX, "RP");
		ff_AddString("1-");
		ff_AddInteger("%ld", (long) bsp->length);
		ff_EndPrint();
	}
	else if (psp->start == 2)
	{
		range = 0;
		ff_StartPrint(5, 5, ASN2FF_EMBL_MAX, "RP");
		for (i=0; i<psp->citcount; i++)
		{
			sfp = psp->citfeat[i];
			loc = (SeqLocPtr) sfp->location;
			slp = GetBaseRangeForCitation (loc, NULL, &start, &stop);
			if (start != 0 || stop != 0)
			{ /* Why do I need the tmp_range test??? */
				tmp_range = stop - start;
				if (tmp_range >= range)
				{
					range = tmp_range;
					ff_AddInteger("%ld", (long) start);
					ff_AddChar('-');
					ff_AddInteger("%ld", (long) stop);
					if (slp != NULL || i+1 != psp->citcount)
						ff_AddString(", ");
				}
			}
			while (slp != NULL)
			{
				slp = GetBaseRangeForCitation (loc, slp, &start, &stop);
				if (start != 0 || stop != 0)
				{
					ff_AddInteger("%ld", (long) start);
					ff_AddChar('-');
					ff_AddInteger("%ld", (long) stop);
					if (slp != NULL || i+1 != psp->citcount)
						ff_AddString(", ");
				}
			}
		}
		ff_EndPrint();
	}
	else if (psp->start == 3 && ajp->pseudo == TRUE) 
	{ /* "sites" only for pseudo-embl.  */
		ff_StartPrint(5, 5, ASN2FF_EMBL_MAX, "RP");
		ff_AddString("(sites)");
		ff_EndPrint();
	}

	journal = FlatJournal(ajp, gbp, pub, pat_seqid, &submit, FALSE);

	if (descr && descr->comment)
	{
		ff_StartPrint(5, 5, ASN2FF_EMBL_MAX, "RC");
		ff_AddString(descr->comment);
		ff_EndPrint();
	}
	authors = FlatAuthor(ajp, pub);
	ff_StartPrint(5, 5, ASN2FF_EMBL_MAX, "RA");
	if (authors)
		ff_AddString(authors);
	ff_AddChar(';');
	ff_EndPrint();
	
	ff_StartPrint(5, 5, ASN2FF_EMBL_MAX, "RT");
	if (! submit)
	{
		title = FlatPubTitle(pub);
		if (title ){
			if ( *title )
			{
				ff_AddChar('\"');
				StrStripSpaces(title); 
				ff_AddString(title);
				ff_AddChar('\"');
			}
		}
	}
	ff_AddChar(';');
	ff_EndPrint();

	ff_StartPrint(5, 5, ASN2FF_EMBL_MAX, "RL");
	if (journal)
	{
		new_journal = CheckEndPunctuation(journal, '.');
		StrStripSpaces(new_journal);
		ff_AddString(new_journal);
		new_journal = MemFree(new_journal);
	}
	ff_EndPrint();
	
	muid = GetMuid(psp->pub);
	if (muid != 0) {
		sprintf(s, "%ld.", (long) muid);
		s[StringLen(s)] = '\0';
		ff_StartPrint(5, 5, ASN2FF_EMBL_MAX, "RX");
		ff_AddString("MEDLINE; ");
		ff_AddString(s);
		ff_EndPrint();
	}
	if (authors)
		MemFree(authors);
	MemFree(title);
	MemFree(journal);
}	/* EMBL_PrintPubs */

/***************************************************************************
*CharPtr CheckLocusLength (Boolean error_msgs, CharPtr locus, Int2 locus_max, Int2 total_segs)
*
*	Calculate the length of the locus; if it's too long, take characters
*	off the front.  If it's part of a segmented set and the locus ends 
*	in a number, add an "S".  If it appears to be an NCBI locus of the
*	form HSU00001, then take two letters off the front.
***************************************************************************/ 
static CharPtr CheckLocusLength (Boolean error_msgs, CharPtr locus, Int2 locus_max, Int2 total_segs)

{
	Boolean cut_two=FALSE;
	CharPtr buffer;
	Int2 length, surplus;

	length = StringLen(locus);
	buffer = MemNew((length+2)*sizeof(Char));
	buffer = StringCpy(buffer, locus);
	
	if (total_segs > 0 && IS_DIGIT(locus[length-1]) != 0)
		if (locus[length-1] != '0' || IS_DIGIT(locus[length-2]) != 0)
		{
			length++;
			buffer[length-1] = 'S';
			buffer[length] = '\0';
		}

	surplus = length - locus_max;

	if (surplus > 0)
	{
		if (surplus <= 2)
		{	/* Check if this is of the form HSU00001S */
			if (IS_ALPHA(buffer[0]) != 0 &&
				IS_ALPHA(buffer[1]) != 0 &&
				IS_ALPHA(buffer[2]) != 0 &&
	    		        IS_DIGIT(buffer[3]) != 0 &&
	        	        IS_DIGIT(buffer[4]) != 0 &&
	       	          	IS_DIGIT(buffer[5]) != 0 &&
	       	             	IS_DIGIT(buffer[6]) != 0 &&
	       	                IS_DIGIT(buffer[7]) != 0 &&
	       	                buffer[8] == 'S' &&
				buffer[9] == '\0')
					cut_two = TRUE;
		}
	
		if (cut_two == TRUE)
			locus = StringCpy(locus, buffer+2);
		else
			locus = StringCpy(locus, buffer+surplus);
		if (error_msgs == TRUE)
		{
			flat2asn_delete_locus_user_string();
			flat2asn_install_locus_user_string(buffer);
			ErrPostStr(SEV_INFO, ERR_LOCUS_ChangedLocusName, 
				"Locusname length is more than 16, locusname is truncated");
		}
	}

	buffer = MemFree(buffer);

	return locus;
}

NLM_EXTERN Int4 GetPubsAwp (Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	GatherScope gs;
	BioseqPtr bsp = NULL;
	ValNodePtr vnp, v;
	SeqLocPtr slp = NULL;
	SeqIdPtr isip;
	Int4 status, i;
	Char buffer[31];
	
	bsp = gbp->bsp;
	if (bsp == NULL) {
		return 0;
	}
	isip = bsp->id;
	vnp = NULL;
  	MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  	gs.get_feats_location = TRUE;
  	if (ajp->genome_view == TRUE) {
		gs.seglevels = 0;
  	} else if (ajp->only_one) { 
		gs.seglevels = 2;
	} else {
		gs.seglevels = 1;
	}
/*	MemSet ((Pointer) (gs.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
	gs.ignore[OBJ_SEQDESC] = FALSE;
	gs.ignore[OBJ_SEQANNOT] = FALSE;
	gs.ignore[OBJ_SEQFEAT] = FALSE;
	gs.ignore[OBJ_SEQSUB] = FALSE;
	gs.ignore[OBJ_SEQSUB_CIT] = FALSE;*/

	MemSet ((Pointer) (gs.ignore), (int)(FALSE), (size_t) (OBJ_MAX * sizeof(Boolean)));
	gs.ignore[OBJ_SEQALIGN] = TRUE; /* this was being hit many times on big records */

	if (ajp->slp == NULL) {
		slp = ValNodeNew(NULL);
		slp->choice = SEQLOC_WHOLE;
		slp->data.ptrvalue = (SeqIdPtr) SeqIdDup (SeqIdFindBest (bsp->id, 0));
		gs.target = slp;
	} else {
		gs.target = ajp->slp;
	}
	GatherEntity(ajp->entityID, &vnp, get_pubs, &gs);
	if (slp)
		SeqLocFree(slp);
	if ((status = CheckPubs(ajp, bsp, &vnp)) <= 0) {
		if (ajp->error_msgs == TRUE) {
			MakeAnAccession(buffer, isip, 30);
			flat2asn_delete_locus_user_string();
			flat2asn_install_locus_user_string(buffer);
			flat2asn_delete_accession_user_string();
			flat2asn_install_accession_user_string(buffer);
			ErrPostStr(SEV_ERROR, ERR_REFERENCE_NoValidRefs, 
			"No refs found that would result in legal flatfile format");
		}
		/* found something. */
		if (status < 0) {
			ValNodeFree(vnp);
			vnp = NULL;
		}
	}
	gbp->Pub = OrganizePubList(vnp); 
	for (v = gbp->Pub, i=0; v != NULL; v= v->next, i++);
	
	return i;
}
/*************************************************************************
*	Check for EMBL format first
*	Call   to find div for Genbank records 
*	Allocate a buffer for division	
*	09-05-96
*************************************************************************/
static CharPtr GetDivision(Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	ValNodePtr vnp;
	MolInfoPtr mol = NULL;
	EMBLBlockPtr eb=NULL;
	BioseqPtr bsp = gbp->bsp;
	Int2 buflen=4;
	CharPtr buffer;
	static CharPtr embl_divs [] = {
	"FUN","INV","MAM","ORG","PHG","PLN","PRI","PRO","ROD","SYN","UNA","VRL",
	"VRT","PAT","EST","STS", "HUM", "HTC"
	};

	buffer = MemNew(buflen);
	buffer[0] = '\0';
	if (ajp->format == EMBL_FMT || ajp->format == PSEUDOEMBL_FMT ||
					ajp->format == EMBLPEPT_FMT) {
		if ((vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_embl)) != NULL) {
			eb = (EMBLBlockPtr) vnp->data.ptrvalue;
		}
		if (eb ) {
			if (eb->div == 255) {
/* kludge for HUM division */
				if ((vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_molinfo)) != NULL) {
					gbp->descr = MemFree(gbp->descr);	
					mol = (MolInfoPtr) vnp->data.ptrvalue;
				}
				if (mol) {
					if (mol->tech == MI_TECH_survey) {
						StringNCpy_0(buffer, "GSS", buflen);
						return buffer;
					} else if (mol->tech == MI_TECH_htc) {
						StringNCpy_0(buffer, "HTC", buflen);
						return buffer;
					} else if (mol->tech == MI_TECH_htgs_1
							 || mol->tech == MI_TECH_htgs_2) {
						StringNCpy_0(buffer, "HTG", buflen);
						return buffer;
					}
				} else {
					StringNCpy_0(buffer, embl_divs[16], buflen);  /*HUM */
					return buffer;
				}
			} else {
				StringNCpy_0(buffer, embl_divs[eb->div], buflen);
				return buffer;
			}
		}
		BioseqGetGBDivCodeEx (bsp, buffer, buflen, FALSE, ajp->useSeqMgrIndexes);
		if (buffer[0] == NULLB) {
			StringNCpy_0(buffer, "   ", buflen);
		}
		return buffer;
	}
	BioseqGetGBDivCodeEx (bsp, buffer, buflen, FALSE, ajp->useSeqMgrIndexes);
	if (buffer[0] == NULLB) {
		StringNCpy_0(buffer, "   ", buflen);
	}
	if (gbp->bsp && gbp->bsp->mol == Seq_mol_aa) {
		return buffer;
	}
	if (ajp->genome_view) {
		StringNCpy_0(buffer, "CON", buflen);
	}
	return buffer;
}

/***************************************************************************
*
*	UseGIforLocus to get the GI number for the locus and accession numbers.
*	and to get division using Gather
*
***************************************************************************/

NLM_EXTERN void UseGIforLocus (Asn2ffJobPtr ajp)
{
	CharPtr buffer;
	GBEntryPtr gbp;
	
	for (gbp=ajp->asn2ffwep->gbp; gbp; gbp=gbp->next) {
		if (ajp->show_gi) {	
			sprintf(gbp->accession, "%ld", (long) (gbp->gi));
			sprintf(gbp->locus, "%-10ld", (long) (gbp->gi));
		} else {
			MemSet((VoidPtr) gbp->accession, ' ', 10);
			MemSet((VoidPtr) gbp->locus, ' ', 10);
		}
		buffer = GetDivision(ajp, gbp);
		if (buffer[0] != NULLB) {
			StringCpy(gbp->div, "   ");
		} else {
			StringNCpy_0(gbp->div, buffer, 4);
		}
                MemFree(buffer);
	}
}

/*****************************************************************************
*
*	ValidateLocus takes a locus name and assures that the format is 
*	proper. if segmented set adds the segment number at the end and 
*	returns new locus
*****************************************************************************/
CharPtr ValidateLocus(Asn2ffJobPtr ajp, BioseqPtr bsp, CharPtr base_locus, Int2 total_segs, Int2 num_seg, CharPtr new_buf, CharPtr buf_locus, CharPtr orig_buf)

{
	Boolean collision=FALSE;
	static Boolean order_init=FALSE;
	Char buf_ext[BUF_EXT_LENGTH], buffer[30];
	DbtagPtr db;
	int dex;
	Int2 /*UNUSED*/base_locus_max, buf_index, exp, length, num_of_digits;
	ObjectIdPtr ob;
	SeqIdPtr best_id, id;
	static Uint1 rel_order[NUM_SEQID];

	if (! order_init)
	{
		for (dex=0; dex<18; dex++)
			rel_order[dex] = 255;
		rel_order[SEQID_GENERAL ] = 14;
	}
	order_init = TRUE;

	if (ASN2FF_AVOID_LOCUS_COLL || ASN2FF_REPORT_LOCUS_COLL)
	{	/* Check for LOCUS collisions with Karl's algorithm */
		id = bsp->id;
		best_id = SeqIdSelect( id, rel_order,NUM_SEQID);
		if (best_id != NULL) {
			if (best_id -> choice == SEQID_GENERAL){ /* always! */

			    db = (DbtagPtr) best_id -> data.ptrvalue;
			    if (StringCmp(db -> db, LOCUS_COLLISION_DB_NAME) == 0){
				ob = db -> tag;
				    if ( ob != NULL)
				    {
					if (ASN2FF_REPORT_LOCUS_COLL)
					{
					    MakeAnAccession(buffer, id, 30);
					    flat2asn_delete_locus_user_string();
					    flat2asn_install_locus_user_string(buffer);
					    flat2asn_delete_accession_user_string();
					    flat2asn_install_accession_user_string(buffer);
					     ErrPostStr(SEV_WARNING, ERR_LOCUS_LocusNameCollision, "");
					}
					if (ASN2FF_AVOID_LOCUS_COLL)
					{
					    collision=TRUE;
					    StringNCpy_0(new_buf, ob -> str, MAX_LOCUS_NAME_LEN+1);
					}
				    }
				}
			}
		}
	}

	if (! collision)
	{
		if (total_segs == 0)
		{	/* Not a segmented set. */
			if ((length=StringLen(buf_locus)) <= 0)
				new_buf = StringCpy(new_buf, orig_buf);
			else
				new_buf = StringCpy(new_buf, buf_locus);
				 
			new_buf = CheckLocusLength (ajp->error_msgs, new_buf, MAX_LOCUS_NAME_LEN, 0);
		}
		else
		{
			if (total_segs < 10)
				num_of_digits = 1;
			else if (total_segs < 100)
				num_of_digits = 2;
			else if (total_segs < 1000)
				num_of_digits = 3;
			else 
			{
				num_of_digits = 4;
				ErrPostStr(SEV_INFO, ERR_SEGMENT_MoreThan1000Segs, "");
			}
			if (num_seg < 10)
				exp = 1;
			else if (num_seg < 100)
				exp = 2;
			else if (num_seg < 1000)
				exp = 3;
			base_locus_max = MAX_LOCUS_NAME_LEN - num_of_digits;
			length = StringLen(base_locus);
			StringCpy(new_buf, base_locus);
			MemSet((VoidPtr) buf_ext, '\0', BUF_EXT_LENGTH);	
			MemSet((VoidPtr) buf_ext, '0', num_of_digits);	
			sprintf(buf_ext+num_of_digits-exp, "%ld", (long) num_seg);
			buf_index = 0;
			while (buf_ext[buf_index] != '\0')
			{
			     new_buf[length+buf_index] = buf_ext[buf_index];
			     buf_index++;
			 }
			 new_buf[length+buf_index] = '\0';
		}
	}
	
	return new_buf;
}	/* ValidateLocus */

/***************************************************************************
*	example: NM_000756
***************************************************************************/
static Int2 ValidateOtherAccession(CharPtr new_buf, CharPtr orig_buf)
{
	Int2 count;
	Boolean FirstLetter=FALSE, FiveNum = FALSE;

	if (orig_buf == NULL || orig_buf[0] == '\0') {
		return -3;
	}
	if (StringLen(orig_buf) >= 10) {
		return -4;
	}
	if (orig_buf[0] != 'N') {
		return -1;
	}
	if (orig_buf[2] != '_') {
		return -1;
	}
	for (count=3; count < 8; count++) {
		if(! IS_DIGIT(orig_buf[count]))
			break;
	}
	if (count == 8 && (orig_buf[count+1] == '\0' || orig_buf[count+1] == ' ')) {
		StringCpy(new_buf, orig_buf);
		return 0;

	} else {
		return -1;
	}
}


/****************************************************************************
*
*	ValidateAccession takes an accession number and makes sure it is
*	in the proper format (starts with a capital letter that is followed
*	by five numbers).
*
*	Return values are:
*	 0: no problem
*	-1: Accession did not start with a letter (or two letters)
*	-2: Accession did not contain five numbers (or six numbers after 2 letters)
*	-3: the original Accession number to be validated was NULL
*	-4: the original Accession number is too long (>10)
*
****************************************************************************/
Int2 ValidateAccession(CharPtr new_buf, CharPtr orig_buf)
{
	Int2 count, start_count, stop_count;
	Boolean FirstLetter=FALSE, FiveNum = FALSE;

	if (orig_buf == NULL || orig_buf[0] == '\0') {
		return -3;
	}
	if (StringLen(orig_buf) >= 10) {
		return -4;
	}
	if (orig_buf[0] < 'A' || orig_buf[0] > 'Z') {
		return -1;
	} else {
		FirstLetter = TRUE;
	}
	for (count=1; count < 5; count++) {
		if(! IS_DIGIT(orig_buf[count]))
			break;
	}
	if (count == 5 && (orig_buf[count+1] == '\0' || orig_buf[count+1] == ' '))
		FiveNum = TRUE;

	if (FirstLetter == TRUE) {
		if (FiveNum == TRUE) {           /* 1 + 5 accession*/
			StringCpy(new_buf, orig_buf);
			return 0;
		} else if (IS_ALPHA(orig_buf[1])) {      /* 2 + 6 accession */
			if (orig_buf[1] < 'A' || orig_buf[1] > 'Z') {
				return -1;
			}
			start_count = 2;
			stop_count = 7;
			if (orig_buf[0] == 'N' || orig_buf[0] == 'X') {
				if ((orig_buf[1] == 'M' || orig_buf[1] == 'C' 
						|| orig_buf[1] == 'T'  || orig_buf[1] == 'P' 
													 || orig_buf[1] == 'G') 
												&&  orig_buf[2] == '_') {
						start_count = 3;
						stop_count = 8;
				}
			}
			for (count=start_count; count < stop_count; count++) {
				if(! IS_DIGIT(orig_buf[count]))
					break;
			}			
			if (count == stop_count && (orig_buf[count+1] == '\0' || orig_buf[count+1] == ' ')) {
				StringCpy(new_buf, orig_buf);
				return 0;
			} else if (IS_ALPHA(orig_buf[2])) {      /* 3 + 5 accession */
				if (orig_buf[0] =='A' || orig_buf[0] == 'B' || orig_buf[0] == 'C') {
					for (count=3; count < 7; count++) {
						if(! IS_DIGIT(orig_buf[count]))
							break;
					}			
					if (count == 7 && (orig_buf[count+1] == '\0' || orig_buf[count+1] == ' ')) {
						StringCpy(new_buf, orig_buf);
						return 0;
					} else {
						return -2;
					}
				} else {
					return -2;
				}
			} else {
				return -2;
			}
		} else {
			return -2;
		}
	} else {
		return -1;
	}
}

/**************************************************************************
*MakeBaseAccession
*
*	GetBaseAccession takes a BioseqPtr bsp and returns an 
*	accession if 1.) the set is segmented, and 2.) there is
*	an accession at a higher level.  Otherwise NULL is returned.
*	The user should deallocate the CharPtr.
**************************************************************************/

CharPtr MakeBaseAccession (BioseqPtr bsp)

{
	Char buffer[MAX_ACCESSION_LEN+1];
	CharPtr buf_acc=buffer;
	Int2 status = -1;
	SeqIdPtr sip, isip;
	TextSeqIdPtr tsip;


	if (bsp == NULL)
		return NULL;
	isip = bsp->id;
	sip = SeqIdSelect(isip, fasta_order, NUM_SEQID);
	if (sip && (sip->choice == SEQID_GENBANK || 
		sip->choice == SEQID_EMBL || 
		sip->choice == SEQID_PIR || 
		sip->choice == SEQID_SWISSPROT || 
		sip->choice == SEQID_DDBJ || 
		sip->choice == SEQID_PRF ||
		sip->choice == SEQID_OTHER ||
		sip->choice == SEQID_TPG ||
		sip->choice == SEQID_TPE ||
		sip->choice == SEQID_TPD))
	{
		tsip = (TextSeqIdPtr) sip->data.ptrvalue;
		switch (sip->choice) {
			case SEQID_GENBANK:
			case SEQID_EMBL:
			case SEQID_DDBJ:
			case SEQID_TPG:
			case SEQID_TPE:
			case SEQID_TPD:
			case SEQID_PIR:
			case SEQID_SWISSPROT:
				status = ValidateAccession(buf_acc, tsip->accession);
		}
	}
	if (status < 0)
		return NULL;
		
	return (StringSave(buf_acc));
}

/***************************************************************************
*
*	MakeBaseLocus takes a Asn2ffJobPtr and a CharPtr (base_locus)
*	and returns a CharPtr which is the new base_locus.  Checking is 
*	done to assure suitability of the new base locus name (i.e., 
*	no more than 15 characters for less than 10 segments and no more
*	than 14 characters for 10 or more segments).
*
***************************************************************************/

CharPtr MakeBaseLocusAwp (Asn2ffJobPtr ajp, CharPtr base_locus)

{
	BioseqPtr bsp, bbsp = NULL;
	Int2  index, length, base_locus_max, name_len, num_of_digits, num_seg;
	SeqIdPtr sip, bsip=NULL, isip=NULL;
	TextSeqIdPtr tsip = NULL, btsip=NULL;
	ObjectIdPtr obj;
	Char buffer[21], temp_buf[21];
	CharPtr localbuf=buffer, name, ptr=temp_buf;
	CharPtr tmp = "SEG_";
	Asn2ffWEPtr awp;
	GBEntryPtr	gbp;
	
	base_locus[0] = '\0'; 
	awp = ajp->asn2ffwep;
	num_seg = awp->total_seg;
	if (num_seg < 10)
		num_of_digits = 1;
	else if (num_seg < 100)
		num_of_digits = 2;
	else if (num_seg < 1000)
		num_of_digits = 3;
	else 
	{
		ErrPostStr(SEV_INFO, ERR_SEGMENT_MoreThan1000Segs, "");
	}
	base_locus_max = MAX_LOCUS_NAME_LEN - num_of_digits;
/* look for base locus in segmented bioseq */
	awp = ajp->asn2ffwep;
	bbsp = awp->seg; /* segmented Bioseq in segmented set */
	if (bbsp) {
		bsip = SeqIdSelect(bbsp->id, fasta_order, NUM_SEQID);
	}
	if (bsip && (bsip->choice == SEQID_GENBANK || 
				bsip->choice == SEQID_EMBL ||
				bsip->choice == SEQID_DDBJ ||
				bsip->choice == SEQID_SWISSPROT ||
				bsip->choice == SEQID_PIR || 
				bsip->choice == SEQID_OTHER || 
				bsip->choice == SEQID_TPG || 
				bsip->choice == SEQID_TPE || 
				bsip->choice == SEQID_TPD)) {
		btsip = (TextSeqIdPtr) bsip->data.ptrvalue;
	}
	if (btsip && StringLen(btsip->name) > 0) {
		localbuf = StringCpy(localbuf, btsip->name);
		if (StringNCmp(localbuf, tmp, 4) == 0) {
		/* check if name starts with "SEG_", remove if it does. */
		    StringCpy(ptr, localbuf+4);
		    length = StringLen(ptr);
		    ptr[length] = '\0';
		    if (ptr[length-1] == '1') {
		       bsp = awp->gbp->bsp;
		       isip = bsp->id;
		       sip = SeqIdSelect(isip, fasta_order, NUM_SEQID);
		       if (sip &&
		       	(name=((TextSeqIdPtr)sip->data.ptrvalue)->name) != NULL) {
		          name_len = StringLen(name);
		          if (name_len == length) {
		             if (name[length-1] == ptr[length-1])
		    	       for (index=2; index >= num_of_digits; index++) {
		               /* The following is *really* '0'! */
		                  if (ptr[length-index] == '0') {
		                     if (ptr[length-index] == name[length-index]) {
		                        StringNCpy(base_locus, ptr, length-index);
		                        base_locus[length-index] = '\0';
		                     } else {
		                        StringNCpy(base_locus, ptr, length-index+1);
		                        base_locus[length-index+1] = '\0';
		                     }
		                  } else {
		                     StringNCpy(base_locus, ptr, length-index+1);
		                     base_locus[length-index+1] = '\0';
				     break;
		                  }
		               }
		            }
		         }
		    } 
		    /* If nothing else worked, use base locus anyway. */
		    if (base_locus[0] == '\0')
		    	StringCpy(base_locus, ptr);
		}
		if (base_locus[0] == '\0')
			StringCpy(base_locus, btsip->name);

		/*check for length, truncate if necessary.	*/
		base_locus = CheckLocusLength (ajp->error_msgs, base_locus, base_locus_max, num_seg);
		return base_locus;
	}

/* Look for at least one sensible locus in all segments. */
	for (gbp = awp->gbp; gbp; gbp=gbp->next) {
		bsp = gbp->bsp;
		isip = bsp->id;
		sip = SeqIdSelect(isip, fasta_order, NUM_SEQID);
		if (sip && (sip->choice == SEQID_GENBANK || 
					sip->choice == SEQID_EMBL ||
					sip->choice == SEQID_DDBJ ||
					sip->choice == SEQID_SWISSPROT ||
					sip->choice == SEQID_OTHER || 
					sip->choice == SEQID_PIR || 
					sip->choice == SEQID_TPG || 
					sip->choice == SEQID_TPE || 
					sip->choice == SEQID_TPD)) {
			tsip = (TextSeqIdPtr) sip->data.ptrvalue;
		}
		if (tsip && tsip->name && StringLen(tsip->name) > 0) {
			base_locus = StringCpy(base_locus, tsip->name);
			length = StringLen(base_locus);
			base_locus[length-num_of_digits] = '\0';
			base_locus = CheckLocusLength (ajp->error_msgs,
									base_locus, base_locus_max, num_seg);
			return base_locus;
		}
	}

/* No option left but to take the first locus name.*/
	bsp = awp->gbp->bsp;
	isip = bsp->id;
	sip = SeqIdSelect(isip, fasta_order, NUM_SEQID);
	if (sip && sip->choice == SEQID_LOCAL) {
		obj = (ObjectIdPtr) sip->data.ptrvalue;
		if ( obj->str == NULL) {
			sprintf(base_locus, "%ld", (long)(obj->id));
		} else {
			base_locus = StringCpy(base_locus, obj->str);
		}
	} else if (sip && (sip->choice == SEQID_GENBANK || 
				sip->choice == SEQID_EMBL ||
				sip->choice == SEQID_SWISSPROT ||
				sip->choice == SEQID_DDBJ ||
				sip->choice == SEQID_PRF ||
				sip->choice == SEQID_PDB ||
				sip->choice == SEQID_OTHER ||
				sip->choice == SEQID_PIR || 
				sip->choice == SEQID_TPG || 
				sip->choice == SEQID_TPE || 
				sip->choice == SEQID_TPD)) {
		tsip = (TextSeqIdPtr)sip->data.ptrvalue;
		base_locus = StringCpy(base_locus, tsip->name);
	}
	base_locus = CheckLocusLength (ajp->error_msgs, base_locus, base_locus_max, num_seg);
	return base_locus;
	
}	/* MakeBaseLocusAwp */	

static Boolean ValidateVersion(SeqIdPtr sid, Asn2ffJobPtr ajp)
{
	TextSeqIdPtr tsip;
		
	if (ajp->forgbrel == FALSE)
		return TRUE;
	switch (sid->choice) {
	case SEQID_GENBANK:
	case SEQID_EMBL:
	case SEQID_DDBJ:
	case SEQID_OTHER:
	case SEQID_TPG:
	case SEQID_TPE:
	case SEQID_TPD:
		tsip = (TextSeqIdPtr) sid->data.ptrvalue;
		if (tsip->version == 0 || tsip->version == INT2_MIN) {
			return FALSE;
		}
	}
	return TRUE;
}

NLM_EXTERN void GetLocusPartsAwp (Asn2ffJobPtr ajp)
{
	BioseqPtr bsp=NULL;
	Asn2ffWEPtr awp;
	SeqIdPtr sip, isip;
	Int2 num_seg=0, total_segs=0;
	TextSeqIdPtr tsip;
	Char buf_a[MAX_ACCESSION_LEN+1], buf_l[MAX_ACCESSION_LEN+1],
		 base_l[MAX_ACCESSION_LEN+1];
	CharPtr buffer, buf_acc=buf_a, buf_locus=buf_l, base_locus=base_l, base_a;
	GBEntryPtr gbp;
	CharPtr loc;
	Int2 acc_len;

	awp = ajp->asn2ffwep;
	if (ajp->slp) {
		for (gbp = awp->gbp; gbp; gbp = gbp->next) {
			buffer = GetDivision(ajp, gbp);
			if (buffer[0] != NULLB) {
				StringNCpy_0(gbp->div, buffer, 4);
				MemFree(buffer);
			}
			if ((bsp = BioseqFindFromSeqLoc(ajp->slp)) != NULL) {
				CharPtr flatloc;

				isip = SeqIdSelect(gbp->bsp->id, fasta_order, NUM_SEQID);
				if (isip == NULL)
					isip = gbp->bsp->id;
				SeqIdWrite(isip, 
					buf_acc, PRINTID_TEXTID_ACCESSION, MAX_ACCESSION_LEN);

				if (ajp->new_locus_fmt == TRUE)
				  sprintf(gbp->locus, "%-16s", buf_acc);
				else
				  sprintf(gbp->locus, "%-10s", buf_acc);
				
				flatloc =  FlatLoc(bsp, ajp->slp);
				sprintf(gbp->accession, "%s REGION: %s", buf_acc, flatloc);
				flatloc = MemFree(flatloc);
				if (ajp->show_version) {
					SeqIdWrite(isip,
					buf_acc, PRINTID_TEXTID_ACC_VER, MAX_ACCESSION_LEN+1);
					StringNCpy_0(gbp->version,
								 buf_acc, MAX_ACCESSION_LEN+1);
				}
			} else {
				loc = SeqLocPrint(ajp->slp);
				StringNCpy_0(gbp->locus,  loc, MAX_LOCUS_NAME_LEN+1); 
				acc_len = MIN(StringLen(loc), 60);
				StringNCpy_0(gbp->accession, loc, acc_len+1);
				MemFree(loc);
			}
		}
		return; 
	}
	if (ajp->only_one) {
		for (gbp = awp->gbp; gbp; gbp = gbp->next) {
			if (gbp->bsp == NULL) {
				continue;
			}
			bsp = gbp->bsp;
			GetGINumber(gbp);
			buffer = GetDivision(ajp, gbp);
			if (buffer[0] != NULLB) {
				StringNCpy_0(gbp->div, buffer, 4);
				MemFree(buffer);
			}
			isip = SeqIdSelect(gbp->bsp->id, fasta_order, NUM_SEQID);
			if (isip == NULL)
				isip = gbp->bsp->id;
			SeqIdWrite(isip, buf_acc, 
					PRINTID_TEXTID_ACCESSION, MAX_ACCESSION_LEN+1);
			StringNCpy_0(gbp->accession, buf_acc, MAX_ACCESSION_LEN+1);

			if (ajp->new_locus_fmt == TRUE)
			  sprintf(gbp->locus, "%-16s", buf_acc); 
			else
			  sprintf(gbp->locus, "%-10s", buf_acc); 

			if (ajp->show_version) {
				SeqIdWrite(isip, buf_acc, 
					PRINTID_TEXTID_ACC_VER, MAX_ACCESSION_LEN+1);
				StringNCpy_0(gbp->version, buf_acc, MAX_ACCESSION_LEN+1);
			}
		}
		return;
	}
	total_segs = awp->total_seg; 
	base_a = MakeBaseAccession(awp->seg);
	base_locus = MakeBaseLocusAwp(ajp, base_locus);
	StringNCpy_0(ajp->asn2ffwep->base_name, base_locus, 11);
	
	for (gbp = awp->gbp; gbp != NULL; gbp = gbp->next) {
		if (gbp->bsp == NULL) {
			continue;
		}
		bsp = gbp->bsp;
		if ((isip = gbp->bsp->id) == NULL) {
			continue;
		} 
		buffer = GetDivision(ajp, gbp);
		if (buffer[0] != NULLB) {
			StringNCpy_0(gbp->div, buffer, 4);
			MemFree(buffer);
		}
		num_seg = gbp->num_seg; 
		sip = SeqIdSelect(isip, fasta_order, NUM_SEQID);
		if (sip == NULL) {
			sip = isip;
		}
		switch (sip->choice) {
		    case SEQID_GENBANK:
	    	case SEQID_EMBL:
	    	case SEQID_DDBJ:
	    	case SEQID_OTHER:
			case SEQID_TPG:
			case SEQID_TPE:
			case SEQID_TPD:
				tsip = (TextSeqIdPtr) sip->data.ptrvalue;
				if ((ValidateAccession(buf_acc, tsip->accession)) < 0) {
					if (base_a != NULL) {
						StringNCpy_0(buf_acc, base_a, MAX_ACCESSION_LEN+1);
					} else {
						buf_acc = MakeAnAccession(buf_acc, isip, 
													MAX_ACCESSION_LEN+1);
					}
				}
				buf_locus = ValidateLocus(ajp, bsp, base_locus, 
					total_segs, num_seg, buf_locus, tsip->name, buf_acc); 
				StringNCpy_0(gbp->accession, 
					buf_acc, MAX_ACCESSION_LEN+1);
				if (sip->choice == SEQID_OTHER 
						&& StringNCmp(tsip->accession, "NT_", 3) == 0) {
					if (ajp->new_locus_fmt == TRUE)
					  sprintf(gbp->locus, "%-16s", buf_acc);
					else
					  sprintf(gbp->locus, "%-10s", buf_acc);
				} else {
					if (ajp->new_locus_fmt == TRUE)
					  sprintf(gbp->locus, "%-16s", buf_locus);
					else
					  sprintf(gbp->locus, "%-10s", buf_locus);
				}
				num_seg--;
			if (ajp->show_version) {
				if (ValidateVersion(sip, ajp) == FALSE) {
					gbp->bsp = NULL;
					ErrPostEx(SEV_ERROR, ERR_ACCESSION_No_VERSION_Number, "%s", gbp->accession);
					continue;
				}
				SeqIdWrite(sip, buf_acc, 
					PRINTID_TEXTID_ACC_VER, MAX_ACCESSION_LEN+6);
				StringNCpy_0(gbp->version, buf_acc, MAX_ACCESSION_LEN+6);
			}
				break;
		    case SEQID_LOCAL:
				if ((((ObjectIdPtr)sip->data.ptrvalue)->str) == NULL) {
					buf_acc[0] = 'X';
					sprintf(buf_acc+1, "%ld", 
						(long)((ObjectIdPtr)sip->data.ptrvalue)->id);
				} else {
					StringNCpy_0(buf_acc,
				 ((ObjectIdPtr)sip->data.ptrvalue)->str, MAX_ACCESSION_LEN+1);
				}
				buf_locus = ValidateLocus(ajp, bsp, base_locus, 
				total_segs, num_seg,buf_locus,  buf_acc, buf_acc); 
				StringNCpy_0(gbp->accession, buf_acc, MAX_ACCESSION_LEN+1);

				if (ajp->new_locus_fmt == TRUE)
				  sprintf(gbp->locus, "%-16s", buf_locus); 
				else
				  sprintf(gbp->locus, "%-10s", buf_locus); 

				num_seg--;
				break;
		   case SEQID_GI:
			sprintf(buf_acc, "%ld", (long) (sip->data.intvalue));
			buf_locus = ValidateLocus(ajp, bsp, base_locus, 
					total_segs, num_seg, buf_locus, buf_acc, buf_acc); 
			StringNCpy_0(gbp->accession, buf_acc, MAX_ACCESSION_LEN+1);

			if (ajp->new_locus_fmt == TRUE)
			  sprintf(gbp->locus, "%-16s", buf_locus); 
			else
			  sprintf(gbp->locus, "%-10s", buf_locus); 

			num_seg--;
			break;

		   case SEQID_PIR:
		   case SEQID_SWISSPROT:
			tsip = (TextSeqIdPtr) sip->data.ptrvalue;
			if ((ValidateAccession(buf_acc, tsip->accession)) < 0) {
				if (base_a != NULL) {
					StringNCpy_0(buf_acc, base_a, MAX_ACCESSION_LEN+1);
				} else {
					buf_acc = MakeAnAccession(buf_acc, 
						isip, MAX_ACCESSION_LEN);
				}
			}
			if (ajp->show_version) {
				SeqIdWrite(sip, buf_acc, 
					PRINTID_TEXTID_ACC_VER, MAX_ACCESSION_LEN+6);
				StringNCpy_0(gbp->version, buf_acc, MAX_ACCESSION_LEN+6);
			}
			buf_locus = ValidateLocus(ajp, bsp, base_locus, 
				total_segs, num_seg, buf_locus, tsip->name, buf_acc); 
			StringNCpy_0(gbp->accession, buf_acc, MAX_ACCESSION_LEN+1);
			if (sip->choice == SEQID_OTHER 
					&& StringNCmp(tsip->accession, "NT_", 3) == 0) {
				if (ajp->new_locus_fmt == TRUE)
				  sprintf(gbp->locus, "%-16s", buf_acc);
				else
				  sprintf(gbp->locus, "%-10s", buf_acc);
			} else {
				if (ajp->new_locus_fmt == TRUE)
				  sprintf(gbp->locus, "%-16s", buf_locus);
				else
				  sprintf(gbp->locus, "%-10s", buf_locus);
			}
			num_seg--;
			
			break;
		   default:
			buf_acc = MakeAnAccession(buf_acc, isip, MAX_ACCESSION_LEN+1);
			buf_locus = ValidateLocus(ajp, bsp, base_locus, 
				total_segs, num_seg, buf_locus, buf_acc, buf_acc); 
			StringNCpy_0(gbp->accession, buf_acc,
													 MAX_ACCESSION_LEN+1);
			if (ajp->new_locus_fmt == TRUE)
			  sprintf(gbp->locus, "%-10s", buf_locus); 
			else
			  sprintf(gbp->locus, "%-16s", buf_locus); 

			num_seg--;
			break;
		}
	}
	if (base_a != NULL)
		base_a = MemFree(base_a);
	
}
/**************************************************************************
*	Looks in the descriptor  and feature->xref for any extra-accessions.
**************************************************************************/

NLM_EXTERN void AddExtraAccessions(Asn2ffJobPtr ajp, GBEntryPtr gbp)

{
	BioseqPtr bsp;
	Char buffer[10];
	CharPtr ptr=buffer, ac;
	EMBLBlockPtr eb;
	GBBlockPtr gb;
	Int2 index, status;
	SeqFeatPtr sfp;
	SeqIdPtr xid;
	SeqIntPtr si;
	SeqLocPtr xref;
	TextSeqIdPtr text;
	ValNodePtr extra_access=NULL, location=NULL, vnp;
	SortStructPtr p;
	Boolean /*UNUSED*/ncbi = FALSE;

	if (gbp == NULL) {
		return;
	}
	if ((bsp = gbp->bsp) == NULL) {
		return;
	}
	ac = gbp->accession;
	if (ac && *ac == 'U') {
		ncbi = TRUE;
	}
	for (vnp = bsp->descr; vnp; vnp=vnp->next) {
		if (vnp->choice == Seq_descr_genbank) {
			break;
		}
	}
	if (vnp != NULL) {
		gb = (GBBlockPtr) vnp->data.ptrvalue;
		extra_access = gb->extra_accessions;
		if (extra_access != NULL) {
			for (vnp=extra_access; vnp != NULL; vnp=vnp->next) {
				status = ValidateAccession(ptr, vnp->data.ptrvalue);
				if (status == 0) {
					if (ajp->format == EMBL_FMT || ajp->format ==
						 PSEUDOEMBL_FMT || ajp->format == EMBLPEPT_FMT) {
						ff_AddChar(';');
					} else {
						ff_AddChar(' ');
					}
				/*	www_extra_acc(ptr, ncbi); */
					ff_AddString( ptr);
				}
			}
		}
	}
	for (vnp = bsp->descr; vnp; vnp=vnp->next) {
		if (vnp->choice == Seq_descr_embl) {
			break;
		}
	}

	if (vnp != NULL) {
		eb = (EMBLBlockPtr) vnp->data.ptrvalue;
		extra_access = eb->extra_acc;
		if (extra_access != NULL) {
			for (vnp=extra_access; vnp != NULL; vnp=vnp->next) {
				status = ValidateAccession(ptr, vnp->data.ptrvalue);
				if (status == 0) {
					if (ajp->format == EMBL_FMT || ajp->format ==
						 PSEUDOEMBL_FMT || ajp->format == EMBLPEPT_FMT) {
						ff_AddChar(';');
					} else {
						ff_AddChar(' ');
					}
				/*	www_extra_acc(ptr, ncbi); */
					ff_AddString( ptr);
				}
			}
		}
	}
	if (gbp->feat) {
		p = gbp->feat->Xreflist;
		for (index=0; index < gbp->feat->sfpXrefsize; index++, p++) {
			if (location == NULL) {
				location = ValNodeNew(NULL);
				si = SeqIntNew();
				location->choice = SEQLOC_INT;
				location->data.ptrvalue = si;
			}
			si->from = 0;
			bsp = gbp->bsp;
			si->to = bsp->length - 1;
			si->id = bsp->id;	/* Don't delete id!! */
			if ((sfp = p->sfp) == NULL) {
				GatherItemWithLock(p->entityID,
					p->itemID, p->itemtype, &sfp, find_item);
			}
			if (sfp == NULL) {
				continue;
			}
			if (SeqLocCompare(sfp->location, location) != 0) {
				xref = (SeqLocPtr) sfp->data.value.ptrvalue;
				xid = (SeqIdPtr) xref->data.ptrvalue;
				if (xid->choice == 5 || xid->choice == 6 ||
					xid->choice == 13) {
					text = (TextSeqIdPtr) xid->data.ptrvalue;
					status = ValidateAccession(ptr, text->accession);
					if (status == 0) {
						if (ajp->format == EMBL_FMT || ajp->format ==
							 PSEUDOEMBL_FMT || ajp->format == EMBLPEPT_FMT) {
							ff_AddChar(';');
						} else {
							ff_AddChar(' ');
						}
					/*	www_extra_acc(ptr, ncbi); */
						ff_AddString( ptr);
					}
				}
			}
		}
	}

	if (location) {
		si->id = NULL;
		SeqIntFree(si);
		ValNodeFree(location);
	}

	return;
}static Boolean CompareToAwpList (BioseqPtr bsp, Asn2ffWEPtr	awp)

{
	GBEntryPtr gbp;

	if (bsp == NULL) {
		return FALSE;
	}
	for (gbp = awp->gbp; gbp != NULL; gbp = gbp->next) {
		if (bsp == gbp->bsp) {
			return TRUE;
		}
	}
	return FALSE;	
}

static GBEntryPtr GBEntryNew(void)
{
	GBEntryPtr gbp;
	
	gbp = (GBEntryPtr) MemNew(sizeof(GBEntry));
	gbp->feat = NULL;
	gbp->descr = NULL;
	gbp->source_info = NULL;
	gbp->comm = NULL;
	gbp->map = FALSE;
	
	return gbp; 
}

static GBEntryPtr tie_next_gbp(GBEntryPtr head, GBEntryPtr next)
/*  ties next node to the end of the chain */
{
	GBEntryPtr v;

	if (head == NULL) {
		return next;
	}
	for (v = head; v->next != NULL; v = v->next) {
		v = v;
	}
	v->next = next;
	return head;
}

static GBEntryPtr CreateGBEntry(Asn2ffWEPtr awp, BioseqPtr bsp, 
Int2 eID, Int2 iID, Int2 itype)
{
	GBEntryPtr	gbep;

	gbep = GBEntryNew();
	gbep->bsp = bsp;
	gbep->length = bsp->length;
	gbep->entityID = eID;
	gbep->itemID = iID;
	gbep->itemtype = itype;
	awp->gbp = tie_next_gbp(awp->gbp, gbep);
	
	return gbep;
}

/************************************************************************
*	SeqToAwp()
*		gather callback to create a list of GenBank entries
*************************************************************************/

NLM_EXTERN Boolean SeqToAwp (GatherContextPtr gcp)

{
	BioseqPtr bsp;
	SeqEntryPtr ep;
	BioseqSetPtr bssp;
	SeqLocPtr slp;
	Asn2ffWEPtr	awp;
	Asn2ffJobPtr ajp;
	GBEntryPtr	gbep;
	SeqIdPtr isip, sip;
	Uint1 format;
	Boolean is_www = get_www();

	ajp = (Asn2ffJobPtr) gcp->userdata;
	awp = ajp->asn2ffwep;
	format = ajp->format;
	switch (gcp->thistype)
	{
		case OBJ_BIOSEQ:
			bsp = gcp->thisitem;
			if (bsp->repr == Seq_repr_seg) {
				if (ajp->genome_view || ajp->only_one) {
					gbep = CreateGBEntry(awp, bsp, gcp->entityID, 
						gcp->itemID, gcp->thistype);
						if (ajp->only_one && !ajp->map_view) {
							return FALSE;
						}
				}
				if (ISA_na(bsp->mol) && (format == GENBANK_FMT ||
					format == EMBL_FMT || format == PSEUDOEMBL_FMT
						|| format == GRAPHIK_FMT)) {
					awp->seg = bsp;
				} else if (ISA_aa(bsp->mol) && 
					(format == GENPEPT_FMT || format == EMBLPEPT_FMT
						|| format == GRAPHIK_FMT)) {
					awp->seg = bsp;
				}
			}
			if (ASN2FF_LOOK_FOR_SEQ == FALSE) {
				if (ajp->format == GENPEPT_FMT || ajp->format == EMBLPEPT_FMT
					|| (ISA_aa(bsp->mol) && format == GRAPHIK_FMT)) {
					if (ISA_aa(bsp->mol) && (bsp->repr == Seq_repr_raw 
		   	|| bsp->repr == Seq_repr_const || bsp->repr == Seq_repr_delta 
		   	|| 	((is_www || ajp->mode != RELEASE_MODE) && bsp->repr == Seq_repr_virtual))) {
						gbep = CreateGBEntry(awp, bsp, gcp->entityID, 
							gcp->itemID, gcp->thistype);
						++awp->total_seg;
						gbep->num_seg = awp->total_seg;
					}
				} else {
					if (ISA_na(bsp->mol) && (bsp->repr == Seq_repr_raw 
		   		|| bsp->repr == Seq_repr_const|| bsp->repr == Seq_repr_delta
		   		|| 	(is_www && bsp->repr == Seq_repr_virtual))) {
						if (ASN2FF_LOCAL_ID == FALSE) {
							sip = SeqIdSelect(bsp->id, fasta_order, NUM_SEQID);
							if (sip && sip->choice != SEQID_LOCAL) {
								gbep = CreateGBEntry(awp, bsp, gcp->entityID, 
									gcp->itemID, gcp->thistype);
								++awp->total_seg;
								gbep->num_seg = awp->total_seg;
							}
						} else {
							gbep = CreateGBEntry(awp, bsp, gcp->entityID, 
								gcp->itemID, gcp->thistype);
							++awp->total_seg;
							gbep->num_seg = awp->total_seg;
						}
					} else if (ISA_na(bsp->mol) && bsp->repr == Seq_repr_map &&
							ajp->map_view) {
							gbep = CreateGBEntry(awp, bsp, gcp->entityID, 
								gcp->itemID, gcp->thistype);
							gbep->map = TRUE;
					}
				}
			} else {
				if (bsp->seq_ext_type == 1) {
					slp = bsp->seq_ext;
					while (slp) {
						bsp = BioseqFind(SeqLocId(slp));
						if (bsp->repr == Seq_repr_raw || 
							bsp->repr == Seq_repr_const 
							|| bsp->repr == Seq_repr_delta 
							|| (is_www && bsp->repr == Seq_repr_virtual)) {
							if (CompareToAwpList(bsp, awp) == FALSE) {
								if (ASN2FF_LOCAL_ID == FALSE) {
									isip = bsp->id;
									sip = SeqIdSelect(isip, 
										fasta_order, NUM_SEQID);
									if (sip && sip->choice != SEQID_LOCAL) {
										gbep = CreateGBEntry(awp, bsp, 
											gcp->entityID, gcp->itemID, 
												gcp->thistype);
										++awp->total_seg;
										gbep->num_seg = awp->total_seg;
									} else if (sip->choice == SEQID_LOCAL && 
										(format == GENPEPT_FMT || 
												format == EMBLPEPT_FMT)) {
										gbep = CreateGBEntry(awp, bsp, 
											gcp->entityID, gcp->itemID, 
													gcp->thistype);
										++awp->total_seg;
										gbep->num_seg = awp->total_seg;
									} else {
										gbep = CreateGBEntry(awp, bsp, 
											gcp->entityID, gcp->itemID, 
												gcp->thistype);
										++awp->total_seg;
										gbep->num_seg = awp->total_seg;
								}
								}
							}
						}
						slp = slp->next;
					}
				} else if (ISA_na(bsp->mol) && (bsp->repr == Seq_repr_raw || 
						bsp->repr == Seq_repr_const 
							|| bsp->repr == Seq_repr_delta 
							|| (is_www && bsp->repr == Seq_repr_virtual))) {
						if (CompareToAwpList(bsp, awp) == FALSE) {
						if (ASN2FF_LOCAL_ID == FALSE) {
							isip = bsp->id;
							sip = SeqIdSelect(isip, fasta_order, NUM_SEQID);
							if (sip && sip->choice != SEQID_LOCAL) {
								gbep = CreateGBEntry(awp, bsp, gcp->entityID, 
											gcp->itemID, gcp->thistype);
								++awp->total_seg;
								gbep->num_seg = awp->total_seg;
							} else if (sip->choice == SEQID_LOCAL && 
									(format == GENPEPT_FMT || 
												format == EMBLPEPT_FMT)) {
								gbep = CreateGBEntry(awp, bsp, gcp->entityID, 
											gcp->itemID, gcp->thistype);
								++awp->total_seg;
								gbep->num_seg = awp->total_seg;
							} else {
								gbep = CreateGBEntry(awp, bsp, gcp->entityID, 
											gcp->itemID, gcp->thistype);
								++awp->total_seg;
								gbep->num_seg = awp->total_seg;
							}
						}
					}
				}
			}
			break;
		case OBJ_BIOSEQSET:
			bssp = (BioseqSetPtr) gcp->thisitem;
			if (bssp->_class == 4) {/*parts*/
				ep = bssp->seq_set;
				if (ep != NULL) {
					bsp = ep->data.ptrvalue;
					if (ISA_na(bsp->mol) && (format == GENBANK_FMT ||
						format == EMBL_FMT || format == PSEUDOEMBL_FMT)) {
						awp->parts = bssp;
					} else if (ISA_aa(bsp->mol) && 
						(format == GENPEPT_FMT || format == EMBLPEPT_FMT)) {
						awp->parts = bssp;
					}
				}
			}
			break;
		default:
			break;
			
	}
	return TRUE;

}



