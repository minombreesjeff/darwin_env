static char const rcsid[] = "$Id: suggslp.c,v 6.2 2003/05/30 17:25:38 coulouri Exp $";

/* suggslp.c
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
* File Name: suggslp.c
*
* Author:  Yuri Sadykov
*
* Version Creation Date:   11/22/95
*
* $Revision: 6.2 $
*
* File Description: 
*	Implementation of Suggest standalone prediction function. Here it
*	is only part, mainly related with reading data for processing and
*	outputting of result of processing. The implementaion of the processing
*	of data is in suggest.c and adjust.c
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* ==========================================================================
*
*
* RCS Modification History:
*/

#include <ncbi.h>
#include <seqport.h>
#include <suggest.h>
#include <suggslp.h>



SuggestError	suggestError = {0, Suggest_error_level_none, NULL};
MessageHook	fOldMsgHook = NULL;

/*****************************************************************************
 * Extern globals
 */
extern SuggestOutput	suggestOut;
extern SuggestRec	suggestRec;
extern IntPtr		intlist;

/*****************************************************************************
 * Globals
 */
static Int4		i4GenCode;

/*============================================================================*\
 * Function:
 *	CheckForBioseq
 *
 * Purpose:
 *	Callback function for SeqEntryExplore, which stores first met Bioseq.
 *
 * Parameters:
 *	pSeqEntry	- pointer to a SeqEntry;
 *	mydata		- pointer to a void data, but in context of this
 *		function it's pointer to a BioseqPtr, which will hold first
 *		found Bioseq;
 *	index		- not in use;
 *	indent		- not in use;
 *
 * Return:
 *	none
\*----------------------------------------------------------------------------*/
static void
CheckForBioseq(SeqEntryPtr pSeqEntry, Pointer mydata,
	       Int4 index, Int2 indent)
{
    BioseqPtr*	pBSP = (BioseqPtr*)mydata;

    if (pBSP == NULL)
	return; 

    if (*pBSP != NULL)
	return;	/* First Bioseq inside of the whole SeqEntry
		 * already has been found
		 */
    if (IS_Bioseq(pSeqEntry))
	*pBSP = pSeqEntry->data.ptrvalue;	/* First Bioseq is found,
						 * store it
						 */
    return;
}

/*
** This SeqEntry exploration function copy the current pointer position in the
** the Bioseq entry to a list of Bioseq pointers
*/

static void
BuildList(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	((BioseqPtr PNTR) data)[index] = (BioseqPtr)sep->data.ptrvalue;
	return;
}

/*============================================================================*\
 * Function:
 *	FindFirstBioseq
 *
 * Purpose:
 *	Finds first Bioseq inside whole SeqEntry.
 *
 * Parameters:
 *	pSeqEntry	- pointer to a SeqEntry;
 *
 * Return:
 *	pointer to the Bioseq if found, otherwise NULL.
\*----------------------------------------------------------------------------*/
static BioseqPtr
FindFirstBioseq(SeqEntryPtr pSeqEntry)
{
    BioseqPtr	pBSP = NULL;
    
    if (pSeqEntry == NULL)
	return NULL;

    SeqEntryExplore(pSeqEntry, &pBSP, CheckForBioseq);

    return pBSP;
}

/*
* =================================================================
 ----- copied from reverse () --
* ======================================
*
* Description: generates antiparallel complement
*
* Return Value: void
*
* Input Via Formal Parameters: dna_seq, n is always dna_len
*
* Output Via Formal Parameters: rev_seq
* 
===========================*/

static void MakeReverseSequence (CharPtr dna_seq, CharPtr rev_seq, Int4 n)

{
	register char	*s, *t;
	char r[128];

	r['A'] = 'T';
	r['T'] = 'A';
	r['U'] = 'A';
	r['C'] = 'G';
	r['M'] = 'K';
	r['G'] = 'C';
	r['R'] = 'Y';
	r['S'] = 'S';
	r['V'] = 'B';
	r['W'] = 'W';
	r['Y'] = 'R';
	r['H'] = 'D';
	r['K'] = 'M';
	r['D'] = 'H';
	r['B'] = 'V';
	r['N'] = 'N';
	r['-'] = '-';
	for (s = dna_seq + n, t = rev_seq; s > dna_seq; )
		*t++ = r[*--s];
	*t = '\0';
}

/*============================================================================*\
 * Function:
 *	GetDNA
 *
 * Purpose:
 *	Gets list DNA and store it's data in the internal structure.
 *
 * Parameters:
 *	pSeqEntry	- pointer to SeqEntry which contains the set of
 *		protein sequences;
 *	pSeqRec		- pointer to SeqRec which will contain needed data
 *		corresponding to set of protein sequences;
 *
 * Return:
 *	none
\*----------------------------------------------------------------------------*/

#define BUFSIZE 128

static void
GetDNA(SeqEntryPtr pSeqEntry, SeqRecPtr pSeqRec)
{
    Int4	i4Len;
    Uint1	u1Residue;
    BioseqPtr	pBSP;
    SeqPortPtr	pSeqPort;
    Int2  j;
    Char  buf [BUFSIZE + 5];

    if (pSeqEntry == NULL || pSeqRec == NULL)
	return;

    /* Find out first Bioseq from SeqEntry.
     * It's necessary to create SeqPort
     */
    pBSP = FindFirstBioseq(pSeqEntry);
    if (pBSP == NULL)
	return;	/* There is no data to get, return; */

    pSeqPort = SeqPortNew(pBSP, 0, -1, 0, Seq_code_iupacna);
    if (pSeqPort == NULL)
	return;

    /* Allocate memory for storing bioseq's data
     */
    if (pSeqRec->rawSeq == NULL)
	pSeqRec->rawSeq = BSNew(pBSP->length + 5);

    if (pSeqRec->rawSeq == NULL)
	return;
    
    /* Allocate memory for storing length of bioseq's data
     */
    if (pSeqRec->segLens == NULL)
	pSeqRec->segLens = BSNew(10);
    
    if (pSeqRec->segLens == NULL)
	return;

    /* Allocate memory for storing bioseq's IDs
     */
    if (pSeqRec->ids == NULL)
	pSeqRec->ids = MemNew(sizeof(IdRec));

    if (pSeqRec->ids == NULL)
	return;
	
    /* Store IDs of the bioseq
     */
    pSeqRec->ids->id.pSeqId = SeqIdDup((IS_Bioseq(pSeqEntry) ?
	    pBSP->id : (SeqIdPtr)((BioseqSetPtr)pSeqEntry->data.ptrvalue)->id)
	);
	    
    /* Store DNA data
     */
    j = 0;
    buf [0] = '\0';
    while((u1Residue = SeqPortGetResidue(pSeqPort)) != SEQPORT_EOF) {
	if (IS_residue(u1Residue)) {
	    u1Residue = TO_UPPER(u1Residue);
	    if (u1Residue == 'U')
		u1Residue = 'T';
		/*
	    BSPutByte(pSeqRec->rawSeq, u1Residue);
	    */
	    buf [j] = (Char) u1Residue;
	    j++;
	    if (j >= BUFSIZE) {
	      BSWrite (pSeqRec->rawSeq, buf, j * sizeof (Char));
	      j = 0;
	    }
	}
    }
	
    SeqPortFree(pSeqPort);
    
    /*
    BSPutByte(pSeqRec->rawSeq, 'N');
    BSPutByte(pSeqRec->rawSeq, 'N');
    BSPutByte(pSeqRec->rawSeq, 'N');
    */
    buf [j] = '\0';
    StringCat (buf, "NNN");
    j += 3;
	if (j >= 0) {
	  BSWrite (pSeqRec->rawSeq, buf, j * sizeof (Char));
	  j = 0;
	}

    i4Len = BSLen(pSeqRec->rawSeq);
    
    /* Store the length of DNA data
     */
    BSWrite(pSeqRec->segLens, &i4Len, sizeof(i4Len));

    pSeqRec->sequence = BSMerge(pSeqRec->rawSeq, NULL);
    pSeqRec->length = i4Len;

    pSeqRec->reverse = MemNew (i4Len + 1);
    if (pSeqRec->reverse != NULL) {
      MakeReverseSequence (pSeqRec->sequence, pSeqRec->reverse, i4Len);
    }
}

/*============================================================================*\
 * Function:
 *	GetProtein
 *
 * Purpose:
 *	Gets list protein sequences and store their data in the internal
 * structures
 *
 * Parameters:
 *	pSeqEntry	- pointer to SeqEntry which contains the set of
 *		protein sequences;
 *	pSeqRec		- pointer to SeqRec which will contain needed data
 *		corresponding to set of protein sequences;
 *
 * Return:
 *	none
\*----------------------------------------------------------------------------*/
static void
GetProtein(SeqEntryPtr pSeqEntry, SeqRecPtr pSeqRec)
{
    int		iChar;
    Int4	i;
    Int4	i4NumSeq;
    Int4	i4Len;
    Int4	i4TotalLen = 0;
    IdRecPtr	pIdRec;
    BioseqPtr PNTR	ppSeqList;
    SeqPortPtr	pSeqPort;
    Int2  j;
    Char  buf [BUFSIZE + 5];

    if (pSeqEntry == NULL || pSeqRec == NULL)
	return;

    /* Count the number of Bioseqs in input SeqEntry
     */
    i4NumSeq = BioseqCount(pSeqEntry);
    if (i4NumSeq == 0)
	return;

    /* Allocate memory for pointers to the Bioseqs
     */
    ppSeqList = MemNew(i4NumSeq * sizeof(BioseqPtr));
    if (ppSeqList == NULL)
	return;

    /* Get all Bioseqs in SeqEntry
     */
    BioseqExplore(pSeqEntry, (Pointer)ppSeqList, BuildList);

    /* Allocate memory for storing bioseq's data
     */
    if (pSeqRec->rawSeq == NULL)
	pSeqRec->rawSeq = BSNew(1000);

    if (pSeqRec->rawSeq == NULL) {
    	MemFree (ppSeqList);
		return;
	}
    
    /* Allocate memory for storing length of bioseq's data
     */
    if (pSeqRec->segLens == NULL)
	pSeqRec->segLens = BSNew(10);
    
    if (pSeqRec->segLens == NULL) {
    	MemFree (ppSeqList);
		return;
	}

    for (i = 0; i < i4NumSeq; i++) {	/* Read data from each Bioseq */
	/* Store data of each bioseq into internal suggest's data structure
	 */
	i4Len = 0;
    pSeqPort = SeqPortNew(ppSeqList[i], 0, -1, 0, Seq_code_ncbieaa);
    if (pSeqPort != NULL) {
    j = 0;
    buf [0] = '\0';
	while ((iChar = SeqPortGetResidue(pSeqPort)) != SEQPORT_EOF) {
		if (IS_residue(iChar)) {
	    iChar = TO_UPPER(iChar);
	    if (((iChar >= 'A' && iChar <= 'Z') || iChar == '*' || iChar == '-')
		    && StringChr("JO", iChar) == NULL) { /* allow selenocysteine = U */
		/*
		BSPutByte(pSeqRec->rawSeq, iChar);
		*/
	    if (iChar == 'U' || iChar == '*' || iChar == '-') {
	      iChar = 'X';
	    }
	    buf [j] = (Char) iChar;
	    j++;
	    if (j >= BUFSIZE) {
	      BSWrite (pSeqRec->rawSeq, buf, j * sizeof (Char));
	      j = 0;
	    }
		i4Len++;
	    }
	    }
	}
	
    SeqPortFree(pSeqPort);
    
	if (i4Len > 0) {
	    if (pSeqRec->lookForStop) {
	    /*
		BSPutByte(pSeqRec->rawSeq, '*');
		*/
	    buf [j] = (Char) '*';
	    j++;
		if (j >= 0) {
		  BSWrite (pSeqRec->rawSeq, buf, j * sizeof (Char));
		  j = 0;
		}
		i4Len++;
	    }
    
	    /* Store the length of data for current Bioseq
	     */
	    BSWrite(pSeqRec->segLens, &i4Len, sizeof(i4Len));
	    
	    i4TotalLen += i4Len;
	}

	/* Store ID of each bioseq
	 */
	pIdRec = MemNew(sizeof(IdRec));
	if (pIdRec != NULL) {
	    pIdRec->id.pSeqId = SeqIdDup(ppSeqList[i]->id);
	    pIdRec->next = NULL;
	}
	if (pSeqRec->ids == NULL)
	    /* It is first bioseq's id in the chain, simply store it
	     */
	    pSeqRec->ids = pIdRec;
	else {
	    /* It is not first id in the chain, so attach it to the tail
	     */
	    IdRecPtr	pIdTmp;
	    
	    pIdTmp = pSeqRec->ids;
	    while (pIdTmp->next != NULL)	/* Find tail of the chain */
		pIdTmp = pIdTmp->next;

	    pIdTmp->next = pIdRec;
	}
	}
    }
    MemFree (ppSeqList);
    
    if (i4TotalLen > 0) {
	pSeqRec->sequence = BSMerge(pSeqRec->rawSeq, NULL);
	pSeqRec->length = i4TotalLen;	/* The whole length of all Bioseqs */
    } else {
	pSeqRec->sequence = NULL;
	pSeqRec->length = 0;
    }
}

/*============================================================================*\
 * Function:
 *	GetDNAandProtein
 *
 * Purpose:
 *	Gets sequences corresponding to DNA and proteins for farther processing
 *
 * Parameters:
 *	pSIIntervals	- pointer to SuggestIntervals structure which contains
 *		a data read from input asn-stream for processing
 *
 *	pSugRec		- pointer to internal structure which will hold input
 *		data
 *
 * Return:
 *	none
 *
\*----------------------------------------------------------------------------*/

static SeqRec batchNucSeqRec;
static SeqRecPtr batchNucPtr = NULL;

void ClearBatchSuggestNucleotide (void)

{
  ClearSeqRec (&batchNucSeqRec);
  batchNucPtr = NULL;
  ClearBatchSuggestFrames ();
}

void SetBatchSuggestNucleotide (BioseqPtr nuc, Int2 genCode)

{
  if (nuc == NULL) return;
  batchNucSeqRec.rawSeq = NULL;
  batchNucSeqRec.segLens = NULL;
  batchNucSeqRec.ids = NULL;
  batchNucSeqRec.sequence = NULL;
  batchNucSeqRec.reverse = NULL;
  batchNucSeqRec.length = 0L;
  batchNucSeqRec.nuc = TRUE;
  batchNucSeqRec.lookForStop = FALSE;
  i4GenCode = Get_Genetic_Code (genCode, &suggestRec);
  GetDNA (SeqMgrGetSeqEntryForData (nuc), &batchNucSeqRec);
  batchNucPtr = &batchNucSeqRec;
  SetBatchSuggestFrames (batchNucSeqRec.sequence, batchNucSeqRec.reverse, batchNucSeqRec.length);
}

static void GetDNAandProtein(SuggestIntervalsPtr pSIntervals, SuggestRecPtr pSugRec)
{
   if (pSIntervals == NULL || pSugRec == NULL)
       return;

   /* Get DNA
    */
   pSugRec->nucleotide.rawSeq = NULL;
   pSugRec->nucleotide.segLens = NULL;
   pSugRec->nucleotide.ids = NULL;
   pSugRec->nucleotide.sequence = NULL;
   pSugRec->nucleotide.reverse = NULL;
   pSugRec->nucleotide.length = 0L;
   pSugRec->nucleotide.nuc = TRUE;
   pSugRec->nucleotide.lookForStop = FALSE;
   if (batchNucPtr != NULL) {
     pSugRec->nucleotide.rawSeq = batchNucPtr->rawSeq;
     pSugRec->nucleotide.segLens = batchNucPtr->segLens;
     pSugRec->nucleotide.ids = batchNucPtr->ids;
     pSugRec->nucleotide.sequence = batchNucPtr->sequence;
     pSugRec->nucleotide.reverse = batchNucPtr->reverse;
     pSugRec->nucleotide.length = batchNucPtr->length;
     pSugRec->nucleotide.nuc = batchNucPtr->nuc;
     pSugRec->nucleotide.lookForStop = batchNucPtr->lookForStop;
   } else {
     GetDNA(pSIntervals->dna, &(pSugRec->nucleotide));
   }
   
   /* Get protein
    */
   pSugRec->protein.rawSeq = NULL;
   pSugRec->protein.segLens = NULL;
   pSugRec->protein.ids = NULL;
   pSugRec->protein.sequence = NULL;
   pSugRec->protein.reverse = NULL;
   pSugRec->protein.length = 0L;
   pSugRec->protein.nuc = FALSE;
   pSugRec->protein.lookForStop = pSugRec->lookForStop;
   GetProtein(pSIntervals->protein, &(pSugRec->protein));
}

/*============================================================================*\
 * Function:
 *	SetMinVal
 *
 * Purpose:
 *	Sets minVal field in SuggestRec structure taking the value from
 *	SuggestParameters structure, which is part of the request.
 *
 * Parameters:
 *	pSuggRec	- pointer to internal structure which will hold input
 *		data;
 *
 *	pParams		- pointer to SuggestParameters structure;
 *
 * Return:
 *	none
 *
\*----------------------------------------------------------------------------*/
static void
SetMinVal(SuggestRecPtr pSuggRec, SuggestParametersPtr pParams)
{
    ASSERT(pSuggRec);

    if (pParams == NULL)
	/* parameters are not given, set default value
	 */
	pSuggRec->minVal = -1;
    else if (!(pParams->OBbits__ & (1 << OB__Suggest_parameters_begin_search)))
	/* this parameter is not given, set default value
	 */
	pSuggRec->minVal = -1;
    else
	pSuggRec->minVal = pParams->begin_search;
}	

/*============================================================================*\
 * Function:
 *	SetMaxVal
 *
 * Purpose:
 *	Sets MaxVal field in SuggestRec structure taking the value from
 *	SuggestParameters structure, which is part of the request.
 *
 * Parameters:
 *	pSuggRec	- pointer to internal structure which will hold input
 *		data;
 *
 *	pParams		- pointer to SuggestParameters structure;
 *
 * Return:
 *	none
 *
\*----------------------------------------------------------------------------*/
static void
SetMaxVal(SuggestRecPtr pSuggRec, SuggestParametersPtr pParams)
{
    ASSERT(pSuggRec);

    if (pParams == NULL)
	/* parameters are not given, set default value
	 */
	pSuggRec->maxVal = 0;
    else if (!(pParams->OBbits__ & (1 << OB__Suggest_parameters_end_search)))
	/* this parameter is not given, set default value
	 */
	pSuggRec->maxVal = 0;
    else
	pSuggRec->maxVal = pParams->end_search;
}	

/*============================================================================*\
 * Function:
 *	SetTermStop
 *
 * Purpose:
 *	Sets lookForStop field in SuggestRec structure taking the value from
 *	SuggestParameters structure, which is part of the request.
 *
 * Parameters:
 *	pSuggRec	- pointer to internal structure which will hold input
 *		data;
 *
 *	pParams		- pointer to SuggestParameters structure;
 *
 * Return:
 *	none
 *
\*----------------------------------------------------------------------------*/
static void
SetTermStop(SuggestRecPtr pSuggRec, SuggestParametersPtr pParams)
{
    ASSERT(pSuggRec);

    if (pParams == NULL)
	/* parameters are not given, set default value
	 */
	pSuggRec->lookForStop = TRUE;
    else if (!(pParams->OBbits__ & (1 << OB__Suggest_parameters_term_stop)))
	/* this parameter is not given, set default value
	 */
	pSuggRec->lookForStop = TRUE;
    else
	pSuggRec->lookForStop = pParams->term_stop;
}	

/*============================================================================*\
 * Function:
 *	SetMinExon
 *
 * Purpose:
 *	Sets minExon field in SuggestRec structure taking the value from
 *	SuggestParameters structure, which is part of the request.
 *
 * Parameters:
 *	pSuggRec	- pointer to internal structure which will hold input
 *		data;
 *
 *	pParams		- pointer to SuggestParameters structure;
 *
 * Return:
 *	none
 *
\*----------------------------------------------------------------------------*/
static void
SetMinExon(SuggestRecPtr pSuggRec, SuggestParametersPtr pParams)
{
    ASSERT(pSuggRec);

    if (pParams == NULL)
	/* parameters are not given, set default value
	 */
	pSuggRec->minExon = 0;
    else if (!(pParams->OBbits__ & (1 << OB__Suggest_parameters_size)))
	/* this parameter is not given, set default value
	 */
	pSuggRec->minExon = 0;
    else
	pSuggRec->minExon = pParams->size;
}	

/*============================================================================*\
 * Function:
 *	GetSuggestParams
 *
 * Purpose:
 *	Sets all fields in SuggestRec structure taking the valueis from
 *	SuggestParameters structure, which is part of the request.
 *
 * Parameters:
 *	pSuggRec	- pointer to internal structure which will hold input
 *		data;
 *
 *	pParams		- pointer to SuggestParameters structure;
 *
 * Return:
 *	none
 *
\*----------------------------------------------------------------------------*/
static void GetSuggestParams(SuggestRecPtr pSuggRec, SuggestParametersPtr pParams)
{
    SetMinVal(pSuggRec, pParams);
    SetMaxVal(pSuggRec, pParams);
    SetMinExon(pSuggRec, pParams);
    SetTermStop(pSuggRec, pParams);
}

/*============================================================================*\
 * Function:
 *	PutGeneticCode
 *
 * Purpose:
 *	Creates new GeneticCode object, initializing it by genetic code ID
 *	and attaches it to CdRegion.
 *
 * Parameters:
 *	pCDRegion	- pointer to CdRegion structure;
 *
 *	genetic code ID is taken from global variable i4GenCode;
 *
 * Return:
 *	none
 *
\*----------------------------------------------------------------------------*/
static void
PutGeneticCode(CdRegionPtr pCDRegion)
{
    GeneticCodePtr	pGCode;
    ValNodePtr		pValNode;

    ASSERT(pCDRegion);

    /* Suggest server always puts the value of the frame equals 1.
     */
    pCDRegion->frame = 1;

    /* Create GeneticCode object, it's only empty head!
     */
    pGCode = GeneticCodeNew();

    /* Create new GeneticCode's "cell" chaining it to the tail and populating
     * with gencode's ID
     */
    pValNode = ValNodeNew(NULL);
    pValNode->choice = 2;	/* GeneticCode will be put in intvalue field */
    pValNode->data.intvalue = i4GenCode;
    pGCode->data.ptrvalue = pValNode;

    /* Attach the GeneticCode object to CdRegion
     */
    pCDRegion->genetic_code = pGCode;
}

/*============================================================================*\
 * Function:
 *	Get_Prot_ID
 *
 * Purpose:
 *	Obtains ID of the protein
 *
 * Parameters:
 *	num	- index of the protein; under this index protein was
 *		stored in internal structure;
 *
 * Return:
 *	Pointer to SeqId for the protein
 *
\*----------------------------------------------------------------------------*/
static SeqIdPtr 
Get_Prot_ID(Int2 num)
{
    IdRecPtr	pCurrentID;

    /* Simply find out the ID and return pointer to it
     */
    pCurrentID = suggestRec.protein.ids;
    while (num > 0 && pCurrentID != NULL) {
	num--;
	pCurrentID = pCurrentID->next;
    }
    
    return (pCurrentID ? pCurrentID->id.pSeqId : NULL);
}

/*============================================================================*\
 * Function:
 *	OutProteinID
 *
 * Purpose:
 *	Outputs ID of the protein which was processed.
 *
 * Parameters:
 *	pSuggestOut	- pointer to SuggestOutput structure; this structure
 *		contains pointer to the open output file if this application
 *		is running as a standalone application or pointer to SeqAnnot
 *		otherwise.
 *
 *	num		- index of the protein; under this index protein was
 *		stored in internal structure;
 *
 * Return:
 *	none
 *
\*----------------------------------------------------------------------------*/
void
OutProteinID(SuggestOutputPtr pSuggestOut, Int2 num)
{
    SeqAnnotPtr	pSeqAnnot;
    SeqFeatPtr	pSeqFeat;
    SeqLocPtr	pSeqLoc;
    CdRegionPtr	pCDRegion;
    
    ASSERT(pSuggestOut);
    
    pSeqAnnot = pSuggestOut->out.pSeqAnnot;

    ASSERT(pSeqAnnot);

    /* Create new feature which will hold SeqLoc for the protein
     */
    pSeqFeat = SeqFeatNew();
    pSeqFeat->data.choice = SEQFEAT_CDREGION;
    pCDRegion = CdRegionNew();
    pSeqFeat->data.value.ptrvalue = pCDRegion;
    PutGeneticCode(pCDRegion);

    /* Create new SeqLoc and populate it with ID of the protein
     */
    pSeqLoc = ValNodeNew(NULL);
    pSeqLoc->choice = SEQLOC_WHOLE;
    pSeqLoc->data.ptrvalue = SeqIdDup(Get_Prot_ID(num));

    /* Attach the SeqLoc to the feature
     */
    pSeqFeat->product = pSeqLoc;

    /* Put newly created feature in SeqAnnot
     */
    if (pSeqAnnot->data == NULL) {
	/* SeqAnnot doesn't contain any feature, so new one is first
	 * in chain, attach it to SeqAnnot
	 */
	pSeqAnnot->data = pSeqFeat;
    } else {
	/* SeqAnnot already contains at least one feature, so attach new
	 * one to the tail of existing chain of features
	 */
	SeqFeatPtr	pSeqFeatTmp;

	pSeqFeatTmp = (SeqFeatPtr)(pSeqAnnot->data);
	while (pSeqFeatTmp->next != NULL)	/* find out the tail */
	    pSeqFeatTmp = pSeqFeatTmp->next;
	
	pSeqFeatTmp->next = pSeqFeat;
    }
}

/*============================================================================*\
 * Function:
 *	Get_Nuc_ID
 *
 * Purpose:
 *	Obtains ID of the nucleotide
 *
 * Parameters:
 *	num	- index of the nucleotide; under this index nucleotide was
 *		stored in internal structure;
 *
 * Return:
 *	Pointer to SeqId for the nucleotide
 *
\*----------------------------------------------------------------------------*/
static SeqIdPtr 
Get_Nuc_ID(Int2 num)
{
    IdRecPtr	pCurrentID;

    /* Simply find out the ID and return pointer to it
     */
    pCurrentID = suggestRec.nucleotide.ids;
    while (num > 0 && pCurrentID != NULL) {
	num--;
	pCurrentID = pCurrentID->next;
    }
    
    return (pCurrentID ? pCurrentID->id.pSeqId : NULL);
}

/*============================================================================*\
 * Function:
 *	OutLocation
 *
 * Purpose:
 *	Outputs calculated locations (intervals) in the output SeqAnnot
 *
 * Parameters:
 *	pSuggestOut	- pointer to SuggestOutput structure; this structure
 *		contains pointer to the open output file if this application
 *		is running as a standalone application or pointer to SeqAnnot
 *		otherwise.
 *
 *	num_nuc		- index of the nucleotide; under this index nucleotide
 *		was stored in internal structure; WARNIG: now it is ignored
 *		within body of the function. See comments in the body.
 *
 * Return:
 *	none
 *
\*----------------------------------------------------------------------------*/
void
OutLocation(SuggestOutputPtr pSuggestOut, Int4 num_nuc)
{
    IntPtr	thisInt = intlist;
    SeqLocPtr	pSeqLoc;
    SeqLocPtr	pSeqLocMix;
    SeqLocPtr	pSeqLocHead = NULL;
    SeqLocPtr	pSeqLocTail = NULL;
    SeqIntPtr	pSeqInt;
    SeqFeatPtr	pSeqFeat;
    SeqAnnotPtr	pSeqAnnot;
    
    ASSERT(pSuggestOut);
    
    pSeqAnnot = pSuggestOut->out.pSeqAnnot;

    ASSERT(pSeqAnnot);

    pSeqFeat = pSeqAnnot->data;

    /* Find out the tail of the SeqFeat chain. We have to populate last
     * SeqFeat with SeqLocation which contains calculated intervals.
     */
    while (pSeqFeat->next != NULL)
	pSeqFeat = pSeqFeat->next;
    
    /*-----------------------------
     * Put all calculated intervals
     */
    while (thisInt != NULL) {
	/* Create new SeqInterval and populate it
	 */
	pSeqInt = SeqIntNew();
	pSeqInt->from = thisInt->from - 1;
	pSeqInt->to = thisInt->to - 1;
	pSeqInt->strand = thisInt->orient;
	/* On the next line in the call to Get_Nuc_ID the parameter is
	 * ignored by the function, because suggest server works only with
	 * one DNA at once. It's for uniformity and probably for a future.
	 */
	pSeqInt->id = SeqIdDup(Get_Nuc_ID((Int2)(thisInt->id)));
	
	/* Create new SeqLocation and attach above seqinterval to it
	 */
	pSeqLoc = ValNodeNew(NULL);
	pSeqLoc->choice = SEQLOC_INT;
	pSeqLoc->data.ptrvalue = pSeqInt;

	if (pSeqLocHead == NULL)
	    pSeqLocTail = pSeqLocHead = pSeqLoc;
	else {
	    pSeqLocTail->next = pSeqLoc;
	    pSeqLocTail = pSeqLoc;
	}
	
	thisInt = thisInt->next;	/* Go to the next interval */
    }

    if (pSeqLocHead && pSeqLocHead->next) {
	pSeqLocMix = ValNodeNew(NULL);
	pSeqLocMix->choice = SEQLOC_MIX;
	pSeqLocMix->data.ptrvalue = pSeqLocHead;

	pSeqFeat->location = pSeqLocMix;
    } else {
	pSeqFeat->location = pSeqLocHead;
    }
}

void SuggestClientService (SuggestIntervalsPtr pSIntervals)

{
  BioseqPtr    bsp;
  SeqEntryPtr  sep;

  suggestOut.out.pSeqAnnot = NULL;
  if (pSIntervals == NULL) return;
  GetSuggestParams(&suggestRec, pSIntervals->params);
  i4GenCode = Get_Genetic_Code (pSIntervals->code, &suggestRec);
  sep = pSIntervals->dna;
  if (sep != NULL && sep->data.ptrvalue != NULL) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp->length + 5 < MAXALLOC) {
      GetDNAandProtein (pSIntervals, &suggestRec);
      suggestOut.out.pSeqAnnot = SeqAnnotNew ();
      suggestOut.out.pSeqAnnot->type = 1;
      ProcessData (&suggestOut, (Boolean) (batchNucPtr == NULL));
    }
  }
}

SeqAnnotPtr SuggestCodingRegion (BioseqPtr nuc, BioseqPtr prot, Int2 genCode)

{
  SeqAnnotPtr          annot;
  SuggestIntervalsPtr  pSIntervals;
  SuggestIntervals     sIntervals;

  if (nuc == NULL || prot == NULL) return NULL;
  pSIntervals = &sIntervals;
  pSIntervals->OBbits__ = 0;
  pSIntervals->params = NULL;
  pSIntervals->dna = SeqMgrGetSeqEntryForData (nuc);
  pSIntervals->protein = SeqMgrGetSeqEntryForData (prot);
  pSIntervals->code = (Int4) genCode;
  SuggestClientService (pSIntervals);
  annot = suggestOut.out.pSeqAnnot;
  suggestOut.out.pSeqAnnot = NULL;
  return annot;
}


