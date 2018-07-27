/* $Id: jzcoll.c,v 6.17 2001/06/26 16:42:58 vakatov Exp $
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
* File Name:  $RCSfile: jzcoll.c,v $
*
* Author:  Jinghui Zhang
*
* Initial Version Creation Date: 03/24/97
*
* $Revision: 6.17 $
*
* File Description:
*         File for various alignments
*
* $Log: jzcoll.c,v $
* Revision 6.17  2001/06/26 16:42:58  vakatov
* POINT --> BAND_POINT  (to avoid conflicts with MS-Win standard headers)
*
* Revision 6.16  2000/11/16 22:10:37  shavirin
* Moved many functions from txalign.c - due to move of txalign.c to
* distrib/tools directory and libncbitool.a library.
*
*
* ==========================================================================
*/

#include <jzcoll.h>
#include <txalign.h>
#include <codon.h>

static Char pchars[] = "ARNDCQEGHILKMFPSTWYVBZX";	/* amino acid names */
static Int4 webb_blosum62[WEBB_asize][WEBB_asize] = {
   { 4,-1,-2,-2, 0,-1,-1, 0,-2,-1,-1,-1,-1,-2,-1, 1, 0,-3,-2, 0,-2,-1, 0 },
   {-1, 5, 0,-2,-3, 1, 0,-2, 0,-3,-2, 2,-1,-3,-2,-1,-1,-3,-2,-3,-1, 0,-1 },
   {-2, 0, 6, 1,-3, 0, 0, 0, 1,-3,-3, 0,-2,-3,-2, 1, 0,-4,-2,-3, 3, 0,-1 },
   {-2,-2, 1, 6,-3, 0, 2,-1,-1,-3,-4,-1,-3,-3,-1, 0,-1,-4,-3,-3, 4, 1,-1 },
   { 0,-3,-3,-3, 9,-3,-4,-3,-3,-1,-1,-3,-1,-2,-3,-1,-1,-2,-2,-1,-3,-3,-2 },
   {-1, 1, 0, 0,-3, 5, 2,-2, 0,-3,-2, 1, 0,-3,-1, 0,-1,-2,-1,-2, 0, 3,-1 },
   {-1, 0, 0, 2,-4, 2, 5,-2, 0,-3,-3, 1,-2,-3,-1, 0,-1,-3,-2,-2, 1, 4,-1 },
   { 0,-2, 0,-1,-3,-2,-2, 6,-2,-4,-4,-2,-3,-3,-2, 0,-2,-2,-3,-3,-1,-2,-1 },
   {-2, 0, 1,-1,-3, 0, 0,-2, 8,-3,-3,-1,-2,-1,-2,-1,-2,-2, 2,-3, 0, 0,-1 },
   {-1,-3,-3,-3,-1,-3,-3,-4,-3, 4, 2,-3, 1, 0,-3,-2,-1,-3,-1, 3,-3,-3,-1 },
   {-1,-2,-3,-4,-1,-2,-3,-4,-3, 2, 4,-2, 2, 0,-3,-2,-1,-2,-1, 1,-4,-3,-1 },
   {-1, 2, 0,-1,-3, 1, 1,-2,-1,-3,-2, 5,-1,-3,-1, 0,-1,-3,-2,-2, 0, 1,-1 },
   {-1,-1,-2,-3,-1, 0,-2,-3,-2, 1, 2,-1, 5, 0,-2,-1,-1,-1,-1, 1,-3,-1,-1 },
   {-2,-3,-3,-3,-2,-3,-3,-3,-1, 0, 0,-3, 0, 6,-4,-2,-2, 1, 3,-1,-3,-3,-1 },
   {-1,-2,-2,-1,-3,-1,-1,-2,-2,-3,-3,-1,-2,-4, 7,-1,-1,-4,-3,-2,-2,-1,-2 },
   { 1,-1, 1, 0,-1, 0, 0, 0,-1,-2,-2, 0,-1,-2,-1, 4, 1,-3,-2,-2, 0, 0, 0 },
   { 0,-1, 0,-1,-1,-1,-1,-2,-2,-1,-1,-1,-1,-2,-1, 1, 5,-2,-2, 0,-1,-1, 0 },
   {-3,-3,-4,-4,-2,-2,-3,-2,-2,-3,-2,-3,-1, 1,-4,-3,-2,11, 2,-3,-4,-3,-2 },
   {-2,-2,-2,-3,-2,-1,-2,-3, 2,-1,-1,-2,-1, 3,-3,-2,-2, 2, 7,-1,-3,-2,-1 },
   { 0,-3,-3,-3,-1,-2,-2,-3,-3, 3, 1,-2, 1,-1,-2,-2, 0,-3,-1, 4,-3,-2,-1 },
   {-2,-1, 3, 4,-3, 0, 1,-1, 0,-3,-4, 0,-3,-3,-2, 0,-1,-4,-3,-3, 4, 1,-1 },
   {-1, 0, 0, 1,-3, 3, 4,-2, 0,-3,-3, 1,-1,-3,-1, 0,-1,-3,-2,-2, 1, 4,-1 },
   { 0,-1,-1,-1,-2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-2, 0, 0,-2,-1,-1,-1,-1,-1 },
 };

NLM_EXTERN Int4Ptr PNTR load_default_matrix (void)
{
	Int4Ptr PNTR ss;
	Int2 i, j;

	ss = (Int4Ptr PNTR) MemNew((size_t)TX_MATRIX_SIZE * sizeof (Int4Ptr));
	for(i = 0; i<TX_MATRIX_SIZE; ++i)
		ss[i] = (Int4Ptr) MemNew((size_t)TX_MATRIX_SIZE * sizeof (Int4));

	for(i = 0; i < TX_MATRIX_SIZE; i++) 
		for(j = 0; j < TX_MATRIX_SIZE;j++) 
			ss[i][j] = -1000;
	for(i = 0; i < WEBB_asize; ++i)
		for(j = 0; j < WEBB_asize; ++j)
			ss[pchars[i]][pchars[j]] = webb_blosum62[i][j];
	for(i = 0; i < WEBB_asize; ++i)
		ss[pchars[i]]['*'] = ss['*'][pchars[i]] = -4;
	ss['*']['*'] = 1;
	return ss;
}

NLM_EXTERN void free_default_matrix (Int4Ptr PNTR matrix)
{
	Int2 i;

	for(i = 0; i<TX_MATRIX_SIZE; ++i)
		MemFree(matrix[i]);
	MemFree(matrix);
}

NLM_EXTERN SeqIdPtr LIBCALL
ScorePtrUseThisGi (ScorePtr sp)

{
    ObjectIdPtr obid;
    ScorePtr scrp;
    SeqIdPtr gilist=NULL;
    
    for (scrp=sp; scrp; scrp = scrp->next) {
        obid = scrp->id;
        if(obid && obid->str) {
            if (StringICmp(obid->str, "use_this_gi") == 0) {
                ValNodeAddInt(&gilist, SEQID_GI, scrp->value.intvalue);
            }
        }
    }
    
    return gilist;
}

/*
  GetUseThisGi(SeqAlignPtr) looks for the "use_this_gi" flag in the ScorePtr.
*/

NLM_EXTERN SeqIdPtr LIBCALL
GetUseThisGi(SeqAlignPtr seqalign)

{
	Boolean retval=FALSE;
        DenseDiagPtr ddp;
        DenseSegPtr dsp;
        ScorePtr sp;
	SeqIdPtr gilist=NULL;
        StdSegPtr ssp;
	
	sp = seqalign->score;
	if (sp == NULL)
	{
		switch (seqalign->segtype)
		{
			case 1: /*Dense-diag*/
				ddp = (DenseDiagPtr) seqalign->segs;
				while (ddp)
				{
					sp = ddp->scores;
					if (sp)
						break;
					ddp = ddp->next;
				}
				break;
			case 2:
				dsp = ( DenseSegPtr) seqalign->segs;
				if (dsp)
				{
					sp = dsp->scores;
				}
				break;
			case 3:
				ssp = (StdSegPtr) seqalign->segs;
				while (ssp)
				{
					sp = ssp->scores;
					if (sp)
						break;
					ssp = ssp->next;
				}
				break;
			default:
				break;
		}
	}


	gilist = ScorePtrUseThisGi(sp);
	return gilist;
}

/*************************************************************************
*
*	functions and structure related to create a text buffer for the 
*	alignment
*
*************************************************************************/

NLM_EXTERN ValNodePtr FreeTextAlignList(ValNodePtr tdp_list)
{
	TextAlignBufPtr tdp;
	ValNodePtr next;
	Int2 i;

	while(tdp_list)
	{
		next = tdp_list->next;
		tdp_list->next = NULL;
		tdp = (TextAlignBufPtr) tdp_list->data.ptrvalue;
		if(tdp->label)
			MemFree(tdp->label);
		if(tdp->buf)
			MemFree(tdp->buf);
		if(tdp->matrix_val)
			MemFree(tdp->matrix_val);
		if(tdp->exonCount > 0)
		{
			for(i =0; i<3; ++i)
				MemFree(tdp->codon[i]);
		}
		MemFree(tdp);
		MemFree(tdp_list);
		tdp_list = next;
	}

	return NULL;
}


/*######################################################################
#
#	functions related to ProcessTextAlignNode
#
#######################################################################*/


/******************************************************************************
*
*	load_text(bsp, pos1, pos2, l_seq, l_pos, mbuf, maxlen)
*	load the sequence into text
*	bsp: the Bioseq
*	pos1: the first position on the sequence. 
*	pos2: the second position on the sequence. 
*		if(pos1 and pos2 are negative val, indicate the region in on the*	minus strand
*	l_seq: the buffer for loading the sequence
*	l_pos: the current position in l_seq. Will be updated after the sequence
*		is loaded
*	mbuf: buffer from the master sequence. For checking mismatches and positive scores
*	maxlen: the maximum length per line. Used to determine the special 
*	format used for long insertions
*	spacing is the space between the two adjacent residues
*	mismatch: if TRUE, show the identical residue with 
*
*****************************************************************************/

static Boolean load_text(BioseqPtr bsp, Int4 pos1, Int4 pos2, CharPtr l_seq, Int4Ptr l_pos, CharPtr mbuf, Int2 maxlen, Int2 spacing, Boolean translate, Boolean mismatch, Int2Ptr matrix_val, Int4Ptr PNTR matrix, Uint1 strand, Int4Ptr PNTR posMatrix, Int4 q_start)
{
    SeqPortPtr spp = NULL;
    ByteStorePtr b_store = NULL;
    Uint1 code;
    Int4 start, stop;
    Uint1 m_res, t_res, stdaa_res;
    Int2 i;
    Int2 val;
    Int4 length, s_len;
    Int2 c_pos;
    Char temp[100];
    Boolean protein;
    Boolean overflow;
    Boolean reverse;
    Boolean is_real;
    SeqFeatPtr fake_cds;
    Boolean check_neg;	/*if aa is negative, load it as lower case char*/
    SeqMapTablePtr smtp;
    
    if(*l_pos >= maxlen )
        return FALSE;

    /* posMatrix uses NCBIstdaa encoding */
    
    if(posMatrix != NULL) { 
        if((smtp = SeqMapTableFindObj(Seq_code_ncbistdaa, 
                                      Seq_code_ncbieaa)) == NULL)
            return FALSE;
    }

    protein = (bsp->mol == Seq_mol_aa);
    reverse = FALSE;
    if(protein)
        code = Seq_code_ncbieaa;
    else
        code = Seq_code_iupacna;
    check_neg = (matrix_val == NULL && matrix != NULL);
    if(strand == Seq_strand_minus) {	/*on the minus strand*/

        start = -pos2;
        stop = -pos1;
        
        if(protein) {
            strand = Seq_strand_plus;
            reverse = TRUE;
        }
        
    } else {
        start = pos1;
        stop = pos2;
    }
    if(translate) {
        fake_cds = make_fake_cds(bsp, start, stop, strand);
        b_store = ProteinFromCdRegionEx(fake_cds, TRUE, FALSE);
        SeqFeatFree(fake_cds);
        if(b_store == NULL)
            return FALSE;
        length = (stop - start +1)/3;
        BSSeek(b_store, 0, SEEK_SET);
    } else {
        spp = SeqPortNew(bsp, start, stop, strand, code);
        length = stop - start +1;
    }
    c_pos = (Int2)(*l_pos);
    overflow = (c_pos >= maxlen);
    if(maxlen>0 && (length > maxlen)) {	/*large insertions*/
	
        for(i =0; i<5 && !overflow; ++i) {
            if(translate)
                l_seq[c_pos++] = (Uint1)BSGetByte(b_store);
            else {
                if(reverse)
                    SeqPortSeek(spp, length-1 -i, SEEK_SET);
                l_seq[c_pos++] = SeqPortGetResidue(spp);
            }
            overflow = (c_pos >= maxlen);
        }
        for(i =0; i<3 && !overflow; ++i) {
            l_seq[c_pos++] = '.';
            overflow = (c_pos >= maxlen);
        }
        if(!overflow) {
            if(translate)
                BSSeek(b_store, length-1, SEEK_SET);
            else if(!reverse)
                SeqPortSeek(spp, length-5, SEEK_SET);
            for(i =0; i<5 && !overflow; ++i) {
                if(translate)
                    l_seq[c_pos++] = (Uint1)BSGetByte(b_store);
                else {
                    if(reverse)
                        SeqPortSeek(spp, 4-i, SEEK_SET);
                    l_seq[c_pos++] = SeqPortGetResidue(spp);
                }
                overflow = (c_pos >= maxlen);
            }
        }
        if(overflow)
            l_seq[maxlen-1] = '\0';
        else
            l_seq[c_pos] = '\0';
        sprintf(temp, "(length=%ld)", (long) length);
        s_len = StringLen(temp);
        StringCat(l_seq, temp);
        *l_pos = c_pos+s_len;
    } else {
        if(translate) {
            while((val = BSGetByte(b_store)) != EOF) {
                t_res = (Uint1)val;
                l_seq[c_pos]= t_res;
                if(mbuf != NULL) {
                    m_res = mbuf[c_pos];
                    if(matrix_val && matrix)
                        matrix_val[c_pos] = (Int2)matrix[m_res][t_res];
                    if(mismatch && t_res == m_res)
                        l_seq[c_pos] = '.';
                    else if(check_neg && matrix[t_res][m_res] < 0)
                        l_seq[c_pos] = TO_LOWER(t_res);
                }
                c_pos += spacing;
                if(c_pos >= maxlen) {
                    c_pos = maxlen;
                    break;
                }
            }
        } else {
            if(reverse)
                SeqPortSeek(spp, length-1, SEEK_SET);
            s_len = 0;
            while((t_res = SeqPortGetResidue(spp)) != SEQPORT_EOF) {
                is_real = IS_ALPHA(t_res);
                if(is_real || t_res == '*' || t_res == '-') {
                    if(is_real && !protein)
                        t_res = TO_LOWER(t_res);
                    l_seq[c_pos] = t_res;
                    if(mbuf != NULL) {
                        m_res = mbuf[c_pos];
                        if(matrix_val) {
                            if(matrix) {
                                if(posMatrix != NULL) {
                                    if(t_res == m_res) /* complete match */
                                        matrix_val[c_pos] = INT2_MAX;
                                    else {
                                        stdaa_res = SeqMapTableConvert(smtp, t_res);
                                        matrix_val[c_pos] = (Int2)posMatrix[c_pos + q_start][stdaa_res];

                                        /* 
                                     if(posMatrix[c_pos + q_start][t_res] == 
                                     matrix[t_res][t_res]) {
                                     printf("Got it!");
                                     } */

                                    }
                                } else {
                                    matrix_val[c_pos] = (Int2)matrix[m_res][t_res];
                                }
                                
                            } else if(t_res == m_res)
                                matrix_val[c_pos] = '|';
                        }
                        
                        if(mismatch && t_res == m_res)
                            l_seq[c_pos] = '.';
                        else if(posMatrix != NULL) {
                            stdaa_res = SeqMapTableConvert(smtp, m_res);
                            if(check_neg && posMatrix[c_pos + q_start][stdaa_res] < 0)
                                l_seq[c_pos] = TO_LOWER(t_res);
                        } else { /*regular BLOSSUM62*/
                            if(check_neg && matrix[t_res][m_res] < 0)
                                l_seq[c_pos] = TO_LOWER(t_res);
                        }
                    }
                    c_pos += spacing;
                    if(c_pos >= maxlen) {
                        c_pos = maxlen;
                        break;
                    }
                    ++s_len;
                }
                if(reverse) {
                    if(s_len == length)
                        break;
                    else
                        SeqPortSeek(spp, length -1 - s_len, SEEK_SET);
                }
            }
        }
        *l_pos = c_pos;
    }
    
    if(translate)
        BSFree(b_store);
    else
        SeqPortFree(spp);
    return TRUE;
}

/*##########################################################################
#
#	functions related to add the features to the alignment
#
###########################################################################*/


typedef struct protbuf{	/*for loading the translation of a CDs*/
	CharPtr buf;	/*load the protein sequence*/
	Int4 start;	/*start position in CDs*/
	Int4 stop;	/*stop position in CDs*/
	Int4 pos;	/*position for the feature*/
	Boolean load_codon;	/*load the codon data for aa sequence*/
	ValNodePtr cvp_list;	/*list for loading the codon of an aa*/
}ProtBuf, PNTR ProtBufPtr;

		
			
/************************************************************************
*
*	check the protein sequence from CDs feature into the buffer
*
*************************************************************************/
static Boolean load_prot_seq(GatherContextPtr gcp)
{
	SeqFeatPtr sfp;
	ProtBufPtr pbp;
	SeqLocPtr loc;

	if(gcp->thistype != OBJ_SEQFEAT)
		return FALSE;
	sfp = (SeqFeatPtr)(gcp->thisitem);
	if(sfp->data.choice !=3)
		return FALSE;

	pbp = (ProtBufPtr)(gcp->userdata);
	if(pbp->load_codon)	/*looking for codon in aa sequence*/
	{
		pbp->cvp_list = aa_to_codon(sfp, pbp->start, pbp->stop);
		return (pbp->cvp_list !=NULL);
	}
		

	if(pbp->start <0)/*minus strand*/
		loc = SeqLocIntNew((-pbp->stop), (-pbp->start), Seq_strand_minus, SeqLocId(sfp->location));
	else
		loc = SeqLocIntNew(pbp->start, pbp->stop, Seq_strand_plus, SeqLocId(sfp->location));

	pbp->pos = print_protein_for_cds(sfp, pbp->buf, loc, TRUE);
	SeqLocFree(loc);
	return (pbp->pos != -1);
}



static Boolean buffer_for_feature(Int4 c_left, Int4 c_right, Int4 seq_start, Int4 seq_stop, ValNodePtr fnp_node, Boolean load_codon, ProtBufPtr pbp)
{
	FeatNodePtr fnp;
	Uint2 itemtype;
	CharPtr buf = NULL;
	Int2 i=0;
	Char symbol;
	ValNodePtr curr;
	IvalNodePtr inp;
	Int4 i_left, i_right;
	Int4 f_len;


	itemtype = (Uint2)(fnp_node->choice);

	if(itemtype!= OBJ_SEQFEAT)
		return FALSE;
	fnp = (FeatNodePtr) fnp_node->data.ptrvalue;
	f_len = seq_stop - seq_start +1; 
	if(load_codon)
		pbp->buf = NULL;
	else
		pbp->buf = (CharPtr) MemNew((size_t)(f_len +1)*sizeof(Char));
	pbp->start = seq_start;
	pbp->stop = seq_stop;
	pbp->pos = -1;
	pbp->load_codon= load_codon;
	pbp->cvp_list = NULL;

	buf = pbp->buf;
	if(buf !=NULL)
		MemSet((Pointer)buf,  '~', (size_t)(f_len) * sizeof(Char));
	switch(fnp->feattype)/*check symbol for different features*/
	{
		case FEATDEF_GENE:
			symbol = '+';
			break;
		case FEATDEF_mRNA:
			symbol = '^';
			break;
		case FEATDEF_CDS:
			symbol = '$';
			break;
		default:
			symbol = '*';
			break;
	}
	if(fnp->feattype ==FEATDEF_CDS)

		GatherItem(fnp->entityID, fnp->itemID, itemtype, (Pointer)(pbp), load_prot_seq);
	else
	{
		if(fnp->interval !=NULL)
		{
			for(curr = fnp->interval; curr !=NULL; curr = curr->next)
			{
				inp = (IvalNodePtr) curr->data.ptrvalue;
				i_left = inp->gr.left;
				i_right = inp->gr.right;
				if(!(i_left > c_right || i_right < c_left))
				{
					i_left = MAX(i_left, c_left);
					i_right = MIN(i_right, c_right);
					i_left -= c_left;
					i_right -=c_left;
					for(; i_left<=i_right; ++i_left)
						buf[i_left] = symbol;
				}
			}
		}
		else
		{
			i_left = fnp->extremes.left;
			i_right = fnp->extremes.right;
			if(!(i_left > c_right || i_right < c_left))
			{
				i_left = MAX(i_left, c_left);
				i_right = MIN(i_right, c_right);
				i_left -= c_left;
				i_right -=c_left;
				for(; i_left<=i_right; ++i_left)
					buf[i_left] = symbol;
			}
		}
			
	}
	if(buf!=NULL)
		buf[f_len]= '\0';
	if(pbp->pos == -1)
		pbp->pos = ABS(seq_start);

	if(pbp->buf != NULL || pbp->cvp_list !=NULL)
		return TRUE;
	else
		return FALSE;
}


	
static Boolean load_feature_data(ProtBufPtr pbp, FeatNodePtr fnp, Int4 pos, Int4 maxlen, ValNodePtr PNTR fbp_head)
{
	Boolean found;
	TextAlignBufPtr fbp;
	ValNodePtr curr, pcvp;
	CodonVectorPtr cvp;
	Boolean load_codon;
	CharPtr PNTR codon;
	Int2 i;
	Int4 f_len;
	Char label[100];
	CharPtr buf;
	Boolean locus = FALSE;
	
	if(pbp == NULL)
		return FALSE;
	if(pbp->buf == NULL && pbp->cvp_list == NULL)
		return FALSE;
	load_codon = (pbp->cvp_list !=NULL);
	f_len = pbp->stop - pbp->start +1;
	
	found = FALSE;
	for(curr = *fbp_head; curr !=NULL; curr = curr->next)
	{
	   fbp = (TextAlignBufPtr) curr->data.ptrvalue;
	   if(fbp->itemID == fnp->itemID)
	   {
	     if(load_codon)
	     {
		for(pcvp = pbp->cvp_list; pcvp!=NULL; pcvp= pcvp->next)
		{
		   cvp = (CodonVectorPtr) pcvp->data.ptrvalue;
		   if(cvp->exonCount == fbp->exonCount)
		   {
			codon = fbp->codon;
			for(i =0; i<3; ++i)
			{
			   if(pos > fbp->f_pos)
				make_empty(codon[i] + fbp->f_pos, (Int2)(pos - fbp->f_pos));
			   StringCat(codon[i], (cvp->buf[i]+cvp->aa_index));
			}
			cvp->exonCount = 0;
			fbp->f_pos = pos + f_len;
		   }
		}/*end of for*/
	     }
	     else
	     {
		if(fbp->pos == -1)
		   fbp->pos = pbp->pos+1;
		if(pos > fbp->f_pos)
		   make_empty(fbp->buf+fbp->f_pos, (Int2)(pos - fbp->f_pos));	
		StringCat(fbp->buf, pbp->buf);
		fbp->f_pos = pos + f_len;
		found = TRUE;
	     }
	   }
	}


	if(load_codon)
	{
	   for(pcvp = pbp->cvp_list; pcvp!=NULL; pcvp= pcvp->next)
	   {
		cvp = (CodonVectorPtr) pcvp->data.ptrvalue;
		if(cvp->exonCount !=0)
		{
		   fbp = (TextAlignBufPtr) MemNew(sizeof(TextAlignBuf));
		   fbp->seqEntityID = fnp->entityID;
		   fbp->pos = cvp->dna_pos +1;
		   fbp->strand = cvp->strand;
		   seqid_name(cvp->sip, label, locus, FALSE);
		   fbp->label = StringSave(label);
		   fbp->buf = NULL;
		   for(i =0; i<3; ++i)
		   {
		   	buf = (CharPtr) MemNew((size_t)(maxlen+1+1+1) * sizeof(Char));
			/*1 for partial start, 1 for partial stop*/
			if(pos > 0)
				make_empty(buf, (Int2)pos);
			StringCat(buf, cvp->buf[i]+cvp->aa_index);
			fbp->codon[i] = buf;
		   }
		   fbp->frame = cvp->frame;
		   fbp->f_pos = pos+f_len;
		   fbp->exonCount = cvp->exonCount;
		   fbp->itemID = fnp->itemID;
		   fbp->itemID = fnp->itemID;
		   fbp->feattype = fnp->feattype;
		   fbp->subtype = fnp->subtype;
		   fbp->entityID = fnp->entityID;
		   fbp->extra_space = (cvp->aa_index == 0);
		   ValNodeAddPointer(fbp_head, 0, fbp);
		
		}
	     }
	}
	else
	{
	   	if(!found)	/*create a new node*/
		{
		   fbp = (TextAlignBufPtr) MemNew(sizeof(TextAlignBuf));
		   buf = (CharPtr) MemNew((size_t)(maxlen+1) * sizeof(Char));
		   if(pos > 0)
			make_empty(buf, (Int2)pos);
		   StringCat(buf, pbp->buf);
		   fbp->seqEntityID = fnp->entityID;
		   fbp->f_pos = pos + f_len;
		   fbp->itemID = fnp->itemID;
		   fbp->buf = buf;
		   fbp->pos = pbp->pos+1;
		   if(fnp->label !=NULL)
			fbp->label = StringSave(fnp->label);
		   fbp->strand = fnp->extremes.strand;
		   fbp->itemID = fnp->itemID;
		   fbp->feattype = fnp->feattype;
		   fbp->subtype = fnp->subtype;
		   fbp->entityID = fnp->entityID;
		   fbp->exonCount = 0;
		   ValNodeAddPointer(fbp_head, 0, fbp);
		}
	}
	if(pbp->buf)
		MemFree(pbp->buf);
	if(pbp->cvp_list)
		free_cvp_list(pbp->cvp_list);
	return TRUE;
}


	
/**************************************************************************
*
*	collect_feature_buf(fnp_list, g_left, g_right, seq_start, l_pos, 
*	fbp_head, max_len)
*	collect the features to be shown together with the alignment
*	fnp_list: a list of FeatNode associated with the current segment
*	g_left: the left position 
*
***************************************************************************/
static ValNodePtr collect_feature_buf(ValNodePtr fnp_list, Int4 g_left, Int4 g_right, Int4 seq_start, Int4 l_pos, ValNodePtr fbp_head, Int4 maxlen, Boolean is_aa)
{
	ProtBuf pb;
	FeatNodePtr fnp;
	Int4 c_left, c_right;
	Int4 pos;
	Int4 fseq_start, fseq_stop;	/*map sequence start stop to the feature*/
	Int4 f_len;		/*length of the feature*/
	Boolean load_codon;

	if(fnp_list == NULL)
		return fbp_head;

	
	while(fnp_list)
	{
	   fnp = (FeatNodePtr) fnp_list->data.ptrvalue;
	   c_left = fnp->extremes.left;
	   c_right = fnp->extremes.right;
	   load_codon = (is_aa && fnp->feattype == FEATDEF_CDS);
	   if(!(c_left > g_right || c_right < g_left))
	   {
		if(c_left > g_left)	/*map the seq pos from the graphic pos*/
			fseq_start = seq_start + (c_left-g_left);
		else
			fseq_start = seq_start;
		c_left = MAX(c_left, g_left);
		c_right = MIN(c_right, g_right);
		f_len = c_right - c_left+1;
		fseq_stop = fseq_start+f_len-1;

		if(c_left > g_left)
		   pos = l_pos + (c_left - g_left);
		else
		   pos = l_pos;

		if(buffer_for_feature(c_left, c_right, fseq_start, fseq_stop, fnp_list, load_codon, &pb))
		
			load_feature_data(&pb, fnp, pos, maxlen, &fbp_head);
	   }
	   fnp_list = fnp_list->next;
	}

	return fbp_head;
}
	
static Int4 map_position_by_spacing(Int4 distance, Int4 spacing, Boolean is_head)
{
	Int4 pos, left_over;

	if(spacing == 1)
		return distance;

	pos = distance/spacing;
	left_over = distance%spacing;

	if(left_over == 0 && !is_head)
		pos = MAX(pos-1, 0);
	else if(left_over == 2 && is_head)
		++pos;
	return pos;
}

static void add_empty_space(CharPtr buf, Int4 maxlen)
{
	Int4 buf_len;

	buf_len = StringLen(buf);	
	if(buf_len < maxlen)
		make_empty(buf+buf_len, (Int2)(maxlen-buf_len));
}
static void copy_insertion_bar(CharPtr buf, CharPtr ins_2, Int2 sym_pos, Int4 len)
{
	Int2 k;

	if(buf == NULL || ins_2 == NULL)
		return;
	add_empty_space(buf, len);
	for(k = 0; k<sym_pos; ++k)
		if(ins_2[k] == '|' && buf[k] == ' ')
			buf[k] = '|';
}

static Int4 get_long_insert_len(Int4 length)
{
	Char temp[50];

	sprintf(temp, "(length=%ld)", (long) length);
	return (StringLen(temp) + 13);
}

static ValNodePtr load_tdp_data(ValNodePtr PNTR head, CharPtr label, CharPtr text, Uint2 itemID, Uint2 entityID, Uint2 seqEntityID, Uint2 bsp_itemID)
{
	TextAlignBufPtr tdp;

	tdp = (TextAlignBufPtr) MemNew(sizeof(TextAlignBuf));
	tdp->pos = -1;
	tdp->label = label;
	tdp->buf= text;
	tdp->itemID = itemID;
	tdp->entityID = entityID;
	tdp->seqEntityID = seqEntityID;
	tdp->bsp_itemID = bsp_itemID;

	return ValNodeAddPointer(head, 0, (Pointer)tdp);
}
			 
/******************************************************************************
*
*	ProcessTextInsertion(anp, m_left, m_right, bsp)
*	convert the insertions that are located within [m_left, m_right] into
*	text buffer (a list of TextDrawPtr)
*	anp: AlignNodePtr
*	m_left, m_right: the current region for selection
*	bsp: the BioseqPtr for this anp
*
*	return a list of TextDrawPtr
*
******************************************************************************/
static ValNodePtr ProcessTextInsertion(AlignNodePtr anp, Int4 m_left, Int4 m_right, BioseqPtr bsp, Int4 line_len, Int1 m_frame)
{
	AlignSegPtr asp;
	Int4 inslen;		/*length of insertion*/
	Int2 insnum;		/*the number of insertions*/
	Int2 i, j;
	Int4Ptr inslevel;	/*for layout the level of insertions*/
	Int4 level;
	Int4 inspos;		/*position for insertion*/
	Int4 left;
	Int4 len;
	Int4 last_ins;

	CharPtr ins_1;	/* \ symbols for insertions*/
	CharPtr ins_2;	/*| symbols for insertion*/
	CharPtr ins_seq;
	Int4 sym_pos;
	Int4 l_pos;
	Int4 seq_offset, seq_start, seq_stop;
	ValNodePtr head = NULL;
	ValNodePtr fbuf_list = NULL, curr;
	TextAlignBufPtr fbp;
	Int4 g_left, g_right;
	Boolean is_aa;
	Int4 seq_expand;
	Int4 spacing;
	Boolean translate;
	Uint1 strand;

	strand = Seq_strand_plus;
	if(anp->seqpos < 0)
		strand = Seq_strand_minus;
	else if(anp->seqpos == 0 && anp->extremes.strand == Seq_strand_minus)
		strand = Seq_strand_minus;
	spacing = 1;
	if(m_frame > 0)
		spacing = 3;
	if(m_frame  == -1)
	{
		translate = TRUE;
		seq_expand = 3;
	}
	else
	{
		seq_expand = 1;
		translate = FALSE;
	}
	is_aa = (bsp->mol == Seq_mol_aa);
	insnum = 0;
	for(asp = anp->segs; asp !=NULL; asp = asp->next)
	/*checking the insertion numbers*/
	{ 
	   if(asp->type == INS_SEG)
	   {
	   	inspos = asp->ins_pos;
		if (inspos >= m_left && inspos<=m_right)
		{
			++insnum;
			asp->line = 0;
		}
		else
			asp->line = -1;
	   }
	}
	if(insnum == 0)
		return head;

	/*layout the insertions*/
	inslevel = (Int4Ptr) MemNew((size_t)(2*insnum) * sizeof(Int4));	/*layout insert*/
	level = 0;
	len = MAX(m_right - m_left +1, line_len);
	for(asp = anp->segs; asp !=NULL; asp = asp->next)
	{
	   if(asp->type == INS_SEG && asp->line == 0)
	   {
	   	inspos = asp->ins_pos;
		inslen = asp->gr.right/seq_expand;
		/* if(inslen > (m_right-m_left+1)) */
		if(inslen > len)
			inslen = get_long_insert_len(inslen);
		inspos -= m_left;
		asp->line = find_insert_ypos(&left, inslen, inspos, 0, len-1, inslevel, 2, insnum);
		asp->gr.left = left;
		level = MAX(asp->line, level);
	   }	
	}
	MemFree(inslevel);
	

	/*comput the insertion text*/
	for(j = 0; j< (level+1); ++j)
	{
	   l_pos = 0;
	   sym_pos = 0;
	   fbuf_list = NULL;
	   ins_seq = (CharPtr) MemNew((size_t)(len+1) * sizeof(Char));
	   ins_2 = (CharPtr) MemNew((size_t)(len+1) * sizeof(Char));
	   if(j == 0)
		ins_1 = (CharPtr) MemNew((size_t)(len+1) * sizeof(Char));
	   seq_offset = 0;
	   for(asp = anp->segs; asp !=NULL; asp = asp->next)
	   {
	   	if(asp->type == INS_SEG && asp->line >=j)
		{
	
			inspos = asp->ins_pos - m_left;
			if(inspos > sym_pos)
			{
			   if(j == 0)	/*the first level*/
				make_empty(ins_1+sym_pos, (Int2)(inspos-sym_pos));
			   make_empty(ins_2+sym_pos, (Int2)(inspos-sym_pos));
			   sym_pos = inspos;
			}
			if(j == 0)
				ins_1[sym_pos] = '\\';
			ins_2[sym_pos] = '|';
			if(asp->line == j)
				last_ins = inspos+1;
			++sym_pos;
			
			if(asp->line == j)
			{
			   seq_start = anp->seqpos + seq_offset;
			   seq_stop = seq_start + asp->gr.right -1;
			   /* seq_stop = seq_start + map_position_by_spacing(asp->gr.right, spacing, FALSE) * seq_expand + seq_expand -1; */
			   if(asp->gr.left > l_pos)
			   {
				make_empty(ins_seq+l_pos, (Int2)(asp->gr.left-l_pos));
				l_pos = asp->gr.left;
			   }

			   g_left = asp->ins_pos;
			   g_right = asp->ins_pos + asp->gr.right -1;
			   /* g_left = asp->gr.left;
			   g_right = g_left + asp->gr.right -1;*/

			   if((seq_stop - seq_start+1)>len)/*long insertions*/
			   {
		   	      fbuf_list = collect_feature_buf(asp->cnp, g_left, (g_left+4), seq_start, l_pos, fbuf_list, len, is_aa);	/*check the features first*/
		   	      fbuf_list = collect_feature_buf(asp->cnp, g_left, (g_left+4), seq_stop-4, l_pos+8, fbuf_list, len, is_aa);	/*check the features ffirst. 3 is the 3 dots*/
			   }
			   else
			      fbuf_list = collect_feature_buf(asp->cnp, g_left, g_right, seq_start, l_pos, fbuf_list, len, is_aa);

			   load_text(bsp, seq_start, seq_stop, ins_seq, &l_pos, NULL, (Int2)len, 1, translate, FALSE, NULL, NULL, strand, NULL, 0);
			}

		}
		if(asp->type == INS_SEG)
			seq_offset += asp->gr.right;
		if(asp->type == DIAG_SEG || asp->type == REG_SEG || asp->type == STD_SEG)
			seq_offset += map_position_by_spacing(asp->gr.right - asp->gr.left +1, 
				spacing, TRUE) * seq_expand; 
			/* seq_offset += (asp->gr.right - asp->gr.left +1) * seq_expand; */
	   }

	   ins_2[sym_pos] = '\0';
	   ins_seq[l_pos] = '\0';
	   if(j == 0)
	   {
		ins_1[sym_pos] = '\0';	
		load_tdp_data(&head, NULL, ins_1, 0, 0, 0, 0);
	   }

	   for(curr = head; curr !=NULL; curr = curr->next)
	   /*for(curr = fbuf_list; curr !=NULL; curr = curr->next)*/
	   {
		fbp = (TextAlignBufPtr) curr->data.ptrvalue;
		if(fbp->buf != NULL)
			copy_insertion_bar(fbp->buf, ins_2, (Int2)sym_pos, len);
		else
		{
			for(i =0; i<3; ++i)
				copy_insertion_bar(fbp->codon[i], ins_2, (Int2)sym_pos, len);
		}
	   }

	   copy_insertion_bar(ins_seq, ins_2, (Int2)sym_pos, len);
	   load_tdp_data(&head, NULL, ins_2, 0, 0, 0, 0);
	   load_tdp_data(&head, NULL, ins_seq, anp->itemID, anp->entityID, anp->seq_entityID, anp->bsp_itemID);
	   ValNodeLink(&head, fbuf_list);
	   fbuf_list = head;
	}

	return head;

}	

/***********************************************************************
*
*	ProcessTextAlignNode(anp, m_left, m_right, p_stop, m_buf, locus)
*	Process the AlignNode to make a list of text buffer on the 
*	current region
*	anp: AlignNodePtr
*	m_left, m_right: the region on the alignment. Mapped in response
*	to anp->extremes.left, and anp->extremes.right
*	p_stop: the stop position of the previous segment. Used to label
*	the position of a line composed entirely of gaps
*	m_buf: buffer for the master sequence. Used to compare mismatches
*	locus: if TRUE, show the locus name of the alignment
*
*	frame: frame >0, those are the hits from blastx. So, the 
*	protein need to be displayed to the proper frame
*	frame 1-3: match to the plus strand of the master
*	frame 4-6: match to the minus strand of the master
*	frame 0:   no tranlsation, no frame match to the master
*	frame -1:  translate the DNA sequence
*	option:    option for display the alignments
*   matrix:	   the protein alignment matrix
*
*
************************************************************************/

NLM_EXTERN ValNodePtr ProcessTextAlignNode2(AlignNodePtr anp, Int4 m_left, Int4 m_right, Int4Ptr p_stop, CharPtr m_buf, Int4 line_len, Int1 m_frame, Uint4 option, Int4Ptr PNTR matrix, Int4Ptr PNTR posMatrix, Int4 q_start)
{
    Int4 maxlen;
    Int4 g_left, g_right;
    Int4 len;		/*length of the segment*/
    CharPtr l_seq;	/*the buffer for the sequence*/
    Int2Ptr matrix_val;	/*value of each residue in alignment matrix*/
    Int4 l_pos;		/*the start position on the line*/	
    Int4 offset;
    BioseqPtr bsp;
    SeqEntryPtr sep;
    
    AlignSegPtr asp;
    Int4 seq_offset, off_len;
    Int4 seq_start, seq_stop;
    Int4 s_start, s_stop;	/*for marking the position on one line*/
    CharPtr str;
    
    ValNodePtr head = NULL, ins_node;
    ValNodePtr fbuf_list = NULL;
    TextAlignBufPtr tdp;
    Boolean is_aa;
    Int4 spacing;
    Boolean translate;
    Int4 seq_expand;
    Boolean show_mismatch;
    Boolean set_matrix;
    Uint1 strand;
    
    
    if(m_frame > 6 || m_frame < -1)	/*check the m_frame. -1 for translate the hits*/
        return NULL;
    
    
    g_left = anp->extremes.left;
    g_right = anp->extremes.right;
    if(m_left > g_right || m_right < g_left)/*no overlap*/ {
        if(m_frame > 0) {
            if(anp->m_frame != m_frame)
                return NULL;
            if(m_buf == NULL)
                return NULL;
        }
        if(option & TXALIGN_BLUNT_END) {
            maxlen = m_right - m_left +1;
            l_seq = (CharPtr) MemGet((size_t)(maxlen+1)*sizeof(Char), MGET_ERRPOST);
            MemSet((Pointer)l_seq, '-',(size_t)(maxlen) * sizeof(Char));
            l_seq[maxlen] = '\0';
            tdp = (TextAlignBufPtr) MemNew(sizeof(TextAlignBuf));
            tdp->pos = *p_stop;
            tdp->strand = anp->extremes.strand;
            tdp->label = StringSave(anp->label);
            tdp->buf = l_seq;
            tdp->matrix_val = NULL;
            tdp->itemID = anp->itemID;
            tdp->feattype = 0;
            tdp->subtype = 0;
            tdp->entityID = anp->entityID;
            tdp->seqEntityID = anp->seq_entityID;
            tdp->bsp_itemID = anp->bsp_itemID;
            ValNodeAddPointer(&head, 0, tdp);
            return head;
        }
        else
            return NULL;
    }
    
    strand = Seq_strand_plus;
    if(anp->seqpos < 0)
        strand = Seq_strand_minus;
    else if(anp->seqpos == 0 && anp->extremes.strand == Seq_strand_minus)
        strand = Seq_strand_minus;
    
    l_pos = 0;
    spacing = 1;
    offset = 0;
    if(m_frame > 0) {
        if(anp->m_frame != m_frame)
            return NULL;
        if(m_buf == NULL)
            return NULL;
        /*add the empty space to reflect the reading frame*/
        for(str = m_buf; *str != '\n' && *str != '\0'; ++str) {
            if(IS_WHITESP(*str))
                ++offset;
            else
                break;
        }
        spacing = 3;
    }
    if(m_left < g_left) {
        l_pos += (g_left - m_left);
        if(m_frame > 0)
            ++l_pos;
    } else
        l_pos += offset;
    
    bsp = BioseqLockById(anp->sip);
    if(bsp == NULL)
        return NULL;
    is_aa = (bsp->mol == Seq_mol_aa);
    if((m_frame > 0 && !is_aa) || (m_frame == -1 && is_aa)) {
        BioseqUnlock(bsp);
        return NULL;
    }
    if(anp->seq_entityID == 0) {
        sep = SeqEntryFind(bsp->id);
        anp->seq_entityID = SeqMgrGetEntityIDForSeqEntry(sep);
    }
    if(anp->bsp_itemID == 0)
        anp->bsp_itemID = get_bioseq_itemID(bsp, anp->seq_entityID);
    
    if(m_frame == -1) {
        translate = TRUE;
        seq_expand = 3;
    } else {
        translate = FALSE;
        seq_expand = 1;
    }
    
    maxlen = m_right - m_left +1;
    l_seq = (CharPtr) MemGet((size_t)(maxlen+1)*sizeof(Char), 
                             MGET_ERRPOST);
    if(option & TXALIGN_BLUNT_END)
        MemSet((Pointer)l_seq, '-',(size_t)maxlen * sizeof(Char));
    else
        MemSet((Pointer)l_seq, ' ',(size_t)maxlen * sizeof(Char));
    l_seq[maxlen] = '\0';
    
    
    set_matrix = FALSE;
    if(m_frame == 0 && bsp->mol != Seq_mol_aa) { /*DNA-DNA alignment*/
        if(option & TXALIGN_MATRIX_VAL)
            set_matrix = TRUE;
    } else {
        if(matrix != NULL && (option & TXALIGN_MATRIX_VAL))
            set_matrix = TRUE;
    }
    if(set_matrix) {
        matrix_val = (Int2Ptr) MemGet((size_t)(maxlen+1)*sizeof(Int2), MGET_ERRPOST);
        MemSet((Pointer)matrix_val, 0,(size_t)maxlen * sizeof(Int2));
    } else
        matrix_val = NULL;
    show_mismatch = (Boolean)(option & TXALIGN_MISMATCH);
    
    
    /*process  the GAPs and the DIAGs segs*/
    s_start = -1;
    s_stop = -1;
    off_len = 0;
    for(asp = anp->segs; asp !=NULL; asp = asp->next) { 
        g_left = asp->gr.left;
        g_right = asp->gr.right;
        if(!(g_left > m_right || g_right < m_left)) {
            switch(asp->type) {
            case GAP_SEG:
                g_left = MAX(m_left, g_left);
                g_right = MIN(m_right, g_right);
                len = g_right - g_left +1;
                MemSet((Pointer)(l_seq +l_pos), '-',(size_t)len * sizeof(Char));
                l_pos += len;
                break;
                
            case REG_SEG:
            case DIAG_SEG:
            case STD_SEG:	/* Std-seg only works if the m_frame != 0 */
                if(m_left > g_left)
                    len = off_len + m_left - g_left;
                else
                    len = off_len;
                seq_offset = map_position_by_spacing(len, spacing, TRUE) * seq_expand;
                seq_start = anp->seqpos + seq_offset;
                g_left = MAX(m_left, g_left);
                g_right = MIN(m_right, g_right);
                len += (g_right - g_left);
                seq_stop = anp->seqpos + map_position_by_spacing(len, spacing, FALSE) * seq_expand + seq_expand -1;
                
                if(seq_start <= seq_stop) {	/*the order of start and stop is reversed*/
                    if(s_start == -1)	/*record the end point*/
                        s_start = ABS(seq_start);
                    s_stop = ABS(seq_stop);
                    
                    if(m_frame == 0)
                        fbuf_list = collect_feature_buf(asp->cnp, g_left, g_right, seq_start, l_pos, fbuf_list, maxlen, is_aa);	/*check the features first*/
                    load_text(bsp, seq_start, seq_stop, l_seq, &l_pos, m_buf, (Int2)maxlen, 
                              (Int2)spacing, translate, show_mismatch, matrix_val, matrix, strand, posMatrix, q_start);
                    
                }
                break;
                
            default:
                break;
            }
        }
        if(asp->type == INS_SEG)
            off_len += (asp->gr.right * spacing);
        if(asp->type == REG_SEG || asp->type == DIAG_SEG || asp->type == STD_SEG)
            off_len+=(asp->gr.right - asp->gr.left +1);
    }
    
    
    /*the first segment in the layout is a gap segment*/
    if(s_start == -1)
        s_start = *p_stop;
    if(s_stop == -1)	/*gap across the entire region*/
        s_stop = *p_stop;
    *p_stop = s_stop	/*update the stop value*/;
    tdp = (TextAlignBufPtr) MemNew(sizeof(TextAlignBuf));
    tdp->pos = s_start+1;
    tdp->strand = anp->extremes.strand;
    tdp->label = StringSave(anp->label);
    tdp->buf = l_seq;
    tdp->matrix_val = matrix_val;
    tdp->itemID = anp->itemID;
    tdp->feattype = 0;
    tdp->subtype = 0;
    tdp->entityID = anp->entityID;
    tdp->seqEntityID = anp->seq_entityID;
    tdp->bsp_itemID = anp->bsp_itemID;
    ValNodeAddPointer(&head, 0, tdp);
    ValNodeLink(&head, fbuf_list);
    
    ins_node = ProcessTextInsertion(anp, m_left, m_right, bsp, line_len, m_frame);
    ValNodeLink(&head, ins_node);
    BioseqUnlock(bsp);
    return head;
}

NLM_EXTERN ValNodePtr ProcessTextAlignNode(AlignNodePtr anp, Int4 m_left, Int4 m_right, Int4Ptr p_stop, CharPtr m_buf, Int4 line_len, Int1 m_frame, Uint4 option, Int4Ptr PNTR matrix)
{
    return ProcessTextAlignNode2(anp, m_left, m_right, p_stop, m_buf, line_len, m_frame, option, matrix, NULL, 0);
}

NLM_EXTERN ValNodePtr clean_annot_for_anp(ValNodePtr PNTR head)
{
	ValNodePtr prev, next, anp_list;
	
	prev = NULL;
	anp_list = *head;
	while(anp_list)
	{
		next = anp_list->next;
		if(anp_list->choice == OBJ_SEQANNOT)
		{
			if(prev == NULL)
				*head = next;
			else
				prev->next = next;
			anp_list->next = NULL;
			FreeAlignNode(anp_list);
		}
		else
			prev = anp_list;
		anp_list = next;
	}
	
	return (*head);
}



/***********************************************************************
*
*	FreeFeatureList(list)
*	free a list of FeatNode
*
***********************************************************************/
NLM_EXTERN ValNodePtr FreeFeatureList  (ValNodePtr list)
{
	FeatNodePtr  fnp;
	ValNodePtr   next;

 	while (list != NULL) 
 	{
		next = list->next;
		fnp = list->data.ptrvalue;
		if (fnp != NULL) 
		{
			ValNodeFreeData (fnp->interval);
			MemFree (fnp->label);
			MemFree(fnp->pos_label);
			if(fnp->supress_node != NULL)	/*hidden features*/
				FreeFeatureList(fnp->supress_node);
			MemFree (fnp);
		}
		MemFree (list);

		list = next;
	}
	return NULL;
}

/*********************************************************************
*
*	extract_node_list(head, itemType, entityID, feattype, subtype, 
*	label_type)
*	extract a list of featnode from head which will have the 
*	selected itemType, entityID, feattye, subtype, label_type. 
*	set values to 0 if it is not considered in the selection
*
*********************************************************************/
static Boolean do_collect(ValNodePtr vnp, Uint1 itemType, Uint2 entityID, Uint1 feattype, Uint1 label_type)
{
	Boolean is_num;		/*is the gene mark a number*/
	FeatNodePtr fnp;

	if(vnp->choice != itemType)
		return FALSE;

	fnp = (FeatNodePtr)(vnp->data.ptrvalue);
	if(fnp == NULL)
		return FALSE;

	if(entityID !=0)
		if(fnp->entityID !=entityID)
			return FALSE;


	if(itemType == OBJ_SEQFEAT)
	{
		if((feattype == 0) || (fnp->feattype == feattype))
		{
			if(label_type == ALL_LABEL)
				return TRUE;
			is_num = IS_NUM_GENE(fnp->label);
			if(label_type == STR_LABEL)
				return (is_num == FALSE);
			if(label_type == NUM_LABEL)
				return (is_num == TRUE);
		}
		else
			return FALSE;
	}

	return TRUE;
	
}



NLM_EXTERN ValNodePtr extract_node_list(ValNodePtr PNTR head, Uint1 itemType, Uint2 entityID, Uint1 feattype, Uint1 label_type)
{
	ValNodePtr vnp, prev, list, next;


	list = NULL;
	prev = NULL;
	vnp = *head;
	while(vnp)
	{
		next = vnp->next;
		if(do_collect(vnp, itemType, entityID, feattype, label_type))
		{
			if(prev == NULL)
				*head = vnp->next;
			else
				prev->next = vnp->next;
			vnp->next = NULL;
			ValNodeLink(&list, vnp);
		}
		else
			prev = vnp;
		vnp = next;
	}
	
	return list;

}

NLM_EXTERN ValNodePtr extract_lollipop_feature(ValNodePtr PNTR head, Int4 scale, BoolPtr lolli_feature)
{
	ValNodePtr vnp, prev, list, next;
	FeatNodePtr fnp;
	Boolean extract = FALSE;


	list = NULL;
	prev = NULL;
	vnp = *head;
	while(vnp)
	{
		next = vnp->next;
		extract = FALSE;
		if(vnp->choice == OBJ_SEQFEAT)
		{
			fnp = vnp->data.ptrvalue;
			if((fnp->extremes.right - fnp->extremes.left +1) <= scale)
				extract = TRUE;
			else if(lolli_feature != NULL)
				extract = lolli_feature[fnp->feattype];
		}
		if(extract)
		{
			if(prev == NULL)
				*head = vnp->next;
			else
				prev->next = vnp->next;
			vnp->next = NULL;
			ValNodeLink(&list, vnp);
		}
		else
			prev = vnp;
		vnp = next;
	}
	
	return list;

}

/*deside whether the alignment is of different molecules */
NLM_EXTERN Uint1 get_alignment_type(AnnotInfoPtr annot_info)
{
	if(annot_info->blast_type == ALIGN_BLASTX)
		return ALIGN_DNA_TO_PROT;
	if(annot_info->blast_type == ALIGN_TBLASTN)
		return ALIGN_PROT_TO_DNA;
        if(annot_info->blast_type == ALIGN_PSITBLASTN)
                return ALIGN_PROT_TO_DNA;
	if(annot_info->blast_type == ALIGN_TBLASTX)
		return ALIGN_TDNA_TO_TDNA;
	return 0;
}


/*********************************************************************
*
*	FreeAlignNode(list)
*	free a list of AlignNodePtr
*
*********************************************************************/
NLM_EXTERN ValNodePtr FreeAlignNode(ValNodePtr list)
{
	AlignNodePtr anp;
	AlignSegPtr asp, aspnext;
	ValNodePtr   next;
	AlignBlockPtr abp, abpnext;
	AnnotInfoPtr annot_info;

 	while (list != NULL) 
 	{
		next = list->next;
		if(list->choice == OBJ_SEQANNOT)
		{
			annot_info = list->data.ptrvalue;
			MemFree(annot_info);
		}
		else
		{
			anp = list->data.ptrvalue;
			if (anp != NULL) 
			{
				asp = anp->segs;
				while(asp !=NULL)
				{
					aspnext = asp->next;
					asp->next = NULL;
					if(asp->cnp != NULL)
						FreeFeatureList(asp->cnp);
					if(asp->mismatch)
						ValNodeFree(asp->mismatch);
					MemFree(asp);
					asp = aspnext;
				}
				abp = anp->blocks;
				while(abp != NULL)
				{
					abpnext = abp->next;
					MemFree(abp);
					abp = abpnext;
				}
				if(anp->pop_sap !=NULL)
					SeqAnnotFree(anp->pop_sap);
				SeqIdFree(anp->sip);
				MemFree (anp->label);
				MemFree (anp->clone_id);
				MemFree(anp);
			}
		}
		MemFree (list);

		list = next;
	}
	return NULL;
}




/***********************************************************************
*
* 	CollectSegmentSeq(bsp, slp, seqID, offset, head)
*		collect the segments in Bioseq
*	bsp: Bioseq
*	slp: the location on bsp to be collected
*	seqID: the order of bsp in the current list
*	offset: the offset to the graphic
*	head: the head of the previous list
*	return the head of new list
*	if bsp is a segmented sequence, the corresponding segments are recorded
*	in inp. Otherwise there is only one inp for slp.
*	
************************************************************************/


typedef struct collectheader{	/*for collecting data of a sequence display*/
	CollectSeqOptionPtr csop;	/*option for the sequences+features*/
	ValNodePtr features;		/*a list of FeatNode for storing the feature data*/
	ValNodePtr prev_feat;		/*previous node, for speed it up */
	CollectAlignOptionPtr caop;	/*option for the alignment*/
	ValNodePtr aligns;			/*a list of AlignNode for storing alignment data*/
	ValNodePtr prev_align;		/*the previous node for alignment*/	
	SeqLocPtr slp;				/*target Seq-loc*/
	SeqIdPtr maybe_mapid;			/*a possible mapid*/
	
	ObjMgrPtr omp;				/*for save some space in the collection*/
	Char thislabel[101];
	Char ftype[101];
	Uint2 subtype;
	Int2 filter_level;
	GeneDataPtr gdata;
	Uint2 priority;
	Boolean take_all_annot;		/*take everything in a Seq-annot*/
	Boolean load_align;
	Boolean skip_feature;
	Uint1 index;
	Char annotDB[21];
	Boolean is_lod_score;
}CollectHeader, PNTR CollectHeaderPtr;



static void link_data_for_collect (ValNodePtr PNTR head, ValNodePtr PNTR prev, Pointer data, Uint1 type)
{
	ValNodePtr curr;

	curr = ValNodeNew(NULL);
	curr->choice = type;
	curr->data.ptrvalue = data;

	if(*prev == NULL)
		*head = curr;
	else
		(*prev)->next = curr;

	*prev = curr;
}

static ValNodePtr get_last_node (ValNodePtr head)
{
	if(head == NULL)
		return NULL;

	while(head->next != NULL)
		head = head->next;
	return head;
}

static FeatNodePtr CreateFeatNode (ValNodePtr PNTR f_head, ValNodePtr PNTR prev, Uint2 itemType, Uint2 itemID, Uint2 entityID, Uint2 feattype)
{
	FeatNodePtr  fnp;

	fnp = MemNew (sizeof (FeatNode));
	fnp->itemID = itemID;
	fnp->entityID = entityID;
	fnp->feattype = (Uint1)feattype;
	link_data_for_collect(f_head, prev, (Pointer)fnp, (Uint1)(itemType));
	return fnp;
}

static Boolean collect_feature_label(Uint1 format)
{
	return (format <=OM_LABEL_SUMMARY);
}

static Boolean collect_sequence_label(Uint1 format)
{
	return (format >= PRINTID_FASTA_SHORT && format <=PRINTID_REPORT);
}

/*#####################################################################
#
#	functions related to the collection of the features of alignment
#
#####################################################################*/


/****************************************************************
*
*	satcollfunc()
*	callback function for collecting features on Sequence 
*	alignment. It recalculates the feature intervals based on 
*	the intervals in the aligned segments
*
****************************************************************/
typedef struct alignfeat
{
	ObjMgrPtr omp;
	AlignNodePtr anp;
	CollectSeqOptionPtr csop;
	Int2 filter_level;
	Boolean all_feature;
}AlignFeat, PNTR AlignFeatPtr;

static Boolean is_powerblast_feature(SeqAnnotPtr annot)
{
	ValNodePtr desc;

	if(annot->type != 1)
		return FALSE;
	for(desc = annot->desc; desc != NULL; desc = desc->next)
	{
		if(desc->choice == Annot_descr_name)
		{
			if(StringICmp(desc->data.ptrvalue, "powblast") == 0)
				return TRUE;
			if(StringICmp(desc->data.ptrvalue, "powerblast") == 0)
				return TRUE;
			/*powerBlast feature*/
			if(StringNCmp(desc->data.ptrvalue, "PB:", 3) == 0)
				return TRUE;
		}
	}
	return FALSE;
}


static Boolean satcollfunc(GatherContextPtr gcp)
{
	SeqFeatPtr sfp;
	AlignFeatPtr afp;
	CollectSeqOptionPtr csop;
	
	AlignNodePtr anp;
	Uint2 feat_subtype;	/*types defined by objfdef.h*/
	SeqLocPtr slp = NULL;
	Char label[101];
	ObjMgrTypePtr omtp;
	IvalNodePtr new;
	FeatNodePtr fnp;
	AlignSegPtr asp;
	Int4 current_pos;
	Uint1 strand;
	Int4 seglen;
	SeqLocPtr head;
	Int2 label_size;
	Int4 left, right, e_left, e_right;
	Int4 i_left, i_right;
	Int4 ins_len, gap_len;
	GatherRangePtr grp;
	Int2 i;
	ValNodePtr prev;
	
	afp= (AlignFeatPtr)(gcp->userdata);
	if(afp == NULL || afp->csop == NULL)
		return FALSE;

	if(gcp->thistype == OBJ_SEQANNOT)
	{
		afp->all_feature = is_powerblast_feature((SeqAnnotPtr)(gcp->thisitem));
		return TRUE;
	}
	if(gcp->thistype != OBJ_SEQFEAT)
		return TRUE;
		
	if(afp->filter_level == gcp->seglevel+1)
		return TRUE;
	csop = afp->csop;
	label_size = MIN(100, csop->label_size);
	if(csop->features == NULL && afp->all_feature == FALSE)
		return FALSE;

	omtp=ObjMgrTypeFind(afp->omp, OBJ_SEQFEAT, NULL, NULL);
	if(omtp == NULL)
		return TRUE;

	feat_subtype = 0;
	if(omtp->subtypefunc !=NULL)
		feat_subtype =  (*(omtp->subtypefunc)) (gcp->thisitem); 
	if((afp->all_feature == FALSE) && 
		(csop->features[feat_subtype] == FALSE))	/*do not collect the current feature*/
		return TRUE;


	anp = afp->anp;
	current_pos = anp->seqpos;
	if(anp->seqpos < 0)
		strand = Seq_strand_minus;
	else
		strand = Seq_strand_plus;
	sfp = gcp->thisitem;
	label[0] = '\0';
	if(collect_feature_label(csop->flabel_format[feat_subtype]))
		if(omtp->labelfunc !=NULL)
			(*(omtp->labelfunc))(sfp, label, label_size, csop->flabel_format[feat_subtype]);

	/*map to the location of aligned segs*/
	if(gcp->product)	/*for protein sequence alignment*/
		head = sfp->product;
	else
		head = sfp->location;
	left = anp->extremes.left;
	ins_len = 0;
	gap_len = 0;
	e_left = gcp->extremes.left;
	e_right = gcp->extremes.right;
	for(asp = anp->segs; asp !=NULL; asp = asp->next)
	{
		if(asp->type != GAP_SEG)
		{
			prev = get_last_node (asp->cnp);
			if(asp->type == INS_SEG)
			{
				seglen = asp->gr.right;
				/*ins_len += seglen;*/
			}
			else
				seglen = asp->gr.right - asp->gr.left +1;
			right = left + seglen -1;
			if(!(left > e_right || right < e_left))
			{
				fnp = CreateFeatNode (&(asp->cnp), &prev, OBJ_SEQFEAT, gcp->itemID, gcp->entityID, feat_subtype);
				fnp->extremes.left = MAX(left, e_left)  + gap_len;
				fnp->extremes.right = MIN(right, e_right) + gap_len;
				fnp->extremes.left -=ins_len;
				fnp->extremes.right -= ins_len;
				fnp->extremes.strand = gcp->extremes.strand;
				if(label[0] != '\0')
					fnp->label = StringSave(label);
				grp = gcp->rdp;
				for(i=0; (grp!=NULL) && i<gcp->num_interval; ++i)
				{
					i_left = grp->left;
					i_right = grp->right;
					if(!(left > i_right || right < i_left))
					{
						new = MemNew(sizeof(IvalNode));
						new->gr.left = MAX(left, i_left) - ins_len + gap_len;
						new->gr.right = MIN(right, i_right) - ins_len + gap_len;
						new->gr.strand = grp->strand;
						ValNodeAddPointer(&(fnp->interval), 0, new);
					}
					++grp;
				}
			}
			left = right +1;
			if(asp->type == INS_SEG)
				ins_len += seglen;
		}
		else
			gap_len += (asp->gr.right - asp->gr.left +1);
	}
	return TRUE;
}
	
		
/******************************************************************
*
*	CollectFeatureForAlignNode(slp, anp, csop)
*	collect feature for the alignment
*	slp: the target Seq-loc
*	anp: the AlignNode belong to the target Seq-loc
*	csop: the option for gathering the features
*	
******************************************************************/			
NLM_EXTERN Boolean CollectFeatureForAlignNode(SeqLocPtr slp, AlignNodePtr anp, CollectSeqOptionPtr csop)
{
	GatherScope gs;
	AlignFeat af;
	BioseqPtr bsp;
	
	if(slp == NULL || anp == NULL || csop == NULL)
		return FALSE;
		
	if(anp->seq_entityID == 0)
		return FALSE;
	bsp = BioseqLockById(SeqLocId(slp));


	MemSet((Pointer)&gs, 0, sizeof (GatherScope));
	gs.get_feats_location = TRUE;
	gs.get_feats_product =( bsp->mol == Seq_mol_aa);
	MemSet((Pointer)(gs.ignore), (int)TRUE, (size_t)(OBJ_MAX)*sizeof(Boolean));

	gs.ignore[OBJ_SEQANNOT] = FALSE;
	gs.ignore[OBJ_SEQFEAT] = FALSE;

	gs.nointervals = FALSE;	/*need to recalculate the intervals*/
	/* gs.seglevels = 1;
	gs.seglevels = 1;
	gs.stop_on_annot = TRUE;*/
	gs.ignore_top = FALSE;
	gs.currlevel = 0;
	gs.offset = anp->extremes.left;
	gs.target = slp;

	af.anp = anp;
	af.csop = csop;
	af.omp = ObjMgrGet();
	af.filter_level = 0;

	GatherEntity(anp->seq_entityID, (Pointer)(&af), satcollfunc, &gs);
	BioseqUnlock(bsp);
	return TRUE;
}


/******************************************************************
*
*	CollectFeatureForAlign(slp, anp, featureOrder, groupOrder)
*	collect feature for the alignment
*	slp: the target Seq-loc
*	anp: the AlignNode belong to the target Seq-loc
*	featureOrder: the order of features
*	groupOrder: the order of the groups
*	it takes the anp->seq_entityID and searches for the features
*	
******************************************************************/			

static Boolean CollectAlignFeature(SeqLocPtr slp, AlignNodePtr anp, Uint1Ptr featureOrder, Uint1Ptr groupOrder, Uint1Ptr flabel_format) 
{
	CollectSeqOption cs_option;
	Boolean show_feature, collect = FALSE;
	Int2 i;
	ValNode vn;
	
	if(featureOrder == NULL || groupOrder == NULL || slp == NULL || anp == NULL)
		return FALSE;
	
	cs_option.nointerval = FALSE;
	cs_option.slabel_format = PRINTID_TEXTID_ACCESSION;
	cs_option.seglevels = 0;
	cs_option.label_size = 10;
	for( i =0; i<FEATDEF_ANY; ++i)	/*for checking the features to load*/
	{
		show_feature = (featureOrder[i] != 0);
		cs_option.features[i] = show_feature;
		if(show_feature)
			collect = TRUE;
	}
	if(collect)
	{
		if(flabel_format == NULL)
			MemSet((Pointer)(cs_option.flabel_format), OM_LABEL_CONTENT, (size_t)FEATDEF_ANY*sizeof(Uint1));
		else
			MemCopy(&(cs_option.flabel_format), &flabel_format, (size_t)FEATDEF_ANY*sizeof(Uint1));
		CollectFeatureForAlignNode(slp, anp, &cs_option);
		vn.choice = OBJ_SEQALIGN;
		vn.data.ptrvalue = anp;
		vn.next = NULL;
		SortAlignmentFeature(&vn, featureOrder, groupOrder);
		return TRUE;
	}
	else
		return FALSE;
}

NLM_EXTERN Boolean CollectFeatureForAlign(SeqLocPtr slp, AlignNodePtr anp, Uint1Ptr featureOrder, Uint1Ptr groupOrder) 
{
	return CollectAlignFeature(slp, anp, featureOrder, groupOrder, NULL);
}

/******************************************************************
*
*	SortAlignmentFeature(anp_node, featureOrder, groupOrder)
*	sort the list of FeatNode in aligned segment (asp->cnp) to the 
*	proper order of featureOrder and groupOrder
*
*******************************************************************/
NLM_EXTERN void SortAlignmentFeature(ValNodePtr anp_node, Uint1Ptr featureOrder, Uint1Ptr groupOrder)
{
	AlignNodePtr anp;
	AlignSegPtr asp;
	
	while(anp_node)
	{
		if(anp_node->choice != OBJ_SEQANNOT)
		{
			anp = anp_node->data.ptrvalue;
			for(asp = anp->segs; asp !=NULL; asp = asp->next)
					if(asp->cnp !=NULL)
						asp->cnp = SortFeatNode(asp->cnp, featureOrder, groupOrder);
		}
		anp_node = anp_node->next;
	}
}
		

static SeqPortPtr make_current_seqport(SeqLocPtr masterloc, Int4 offset, Uint1 code)
{
	SeqLocPtr slp;
	Int4 start, stop;
	Uint1 strand;
	SeqPortPtr spp;


	start = SeqLocStart(masterloc);
	stop = SeqLocStop(masterloc);
	strand = SeqLocStrand(masterloc);

	if(strand == Seq_strand_minus)
		stop -= offset;
	else
		start += offset;
	slp = SeqLocIntNew(start, stop, strand, SeqLocId(masterloc));
	spp = SeqPortNewByLoc(slp, code);
	SeqLocFree(slp);

	return spp;
}


static void add_int_to_node (ValNodePtr PNTR head, ValNodePtr PNTR prev, Int4 val, Uint1 choice)
{
	ValNodePtr curr;

	curr = ValNodeNew(NULL);	
	curr->choice = choice;
	curr->data.intvalue = val;

	if(*prev == NULL)
		*head = curr;
	else
		(*prev)->next = curr;
	*prev = curr;
}


static Boolean CollectMismatchForAlign(AlignNodePtr anp, SeqLocPtr masterloc, BioseqPtr bsp, Int4 offset)
{
	
	AlignSegPtr asp;
	SeqPortPtr spp, mspp;
	Uint1 code;
	Uint1 res, mres;
	Int4 start = 0, stop = 0;
	Uint1 strand;
	Int4 current_pos;
	Int4 seglen, j;
	ValNodePtr prev;


		
	if(anp->is_master || SeqIdForSameBioseq(SeqLocId(masterloc), anp->sip))
		return FALSE;
	if(bsp->mol == Seq_mol_aa)
		code = Seq_code_ncbieaa;
	else
		code = Seq_code_iupacna;
	/* mspp = SeqPortNewByLoc(masterloc, code); */

	current_pos = anp->seqpos;
	if(anp->seqpos < 0)
	{
		strand = Seq_strand_minus;
		if(ABS(anp->seqpos) < bsp->length-1)
			anp->extremes.l_trunc = TRUE;
	}
	else
	{
		strand = Seq_strand_plus;
		if(anp->seqpos > 0)
			anp->extremes.l_trunc = TRUE;
	}
	for(asp = anp->segs; asp !=NULL; asp = asp->next)
	{
		if(asp->type == INS_SEG)
			seglen = asp->gr.right;
		else
			seglen = asp->gr.right - asp->gr.left + 1;
		switch(asp->type)
		{
			case GAP_SEG:
				break;
				
			case INS_SEG:
				current_pos += seglen;
				break;
				
			case REG_SEG:
			case DIAG_SEG:
				prev = get_last_node (asp->mismatch);
				if(strand == Seq_strand_minus)
				{
					stop = - current_pos;
					start = stop - (seglen-1);
				}
				else
				{
					start = current_pos;
					stop = start + (seglen -1);
				}
				spp = SeqPortNew(bsp, start, stop, strand, code);
				
				mspp = make_current_seqport(masterloc, (asp->gr.left - offset), code);
				/* SeqPortSeek(mspp, (asp->gr.left - offset), SEEK_SET); */

				for(j =0; j<seglen; ++j)
				{
					res = SeqPortGetResidue(spp);
					mres = SeqPortGetResidue(mspp);
					while(res == SEQPORT_EOS || res == SEQPORT_VIRT)
						res = SeqPortGetResidue(spp);
					while(mres == SEQPORT_EOS || mres == SEQPORT_VIRT)
						mres = SeqPortGetResidue(mspp);
					if(IS_ALPHA(res) && IS_ALPHA(mres))
					{
						if(res != mres)
						{
							if(bsp->mol != Seq_mol_aa && !StrChr("acgtACGT", res))
								add_int_to_node (&(asp->mismatch), &prev, (j+asp->gr.left), MISMATCH_AMB);
							else
								add_int_to_node (&(asp->mismatch), &prev, (j+asp->gr.left), MISMATCH_LINE);
							/* ValNodeAddInt(&(asp->mismatch), 0, (j+asp->gr.left)); */
						}
					}
					else if(res == SEQPORT_EOF || mres == SEQPORT_EOF)
						break;
					
				}
				current_pos += seglen;
				
				SeqPortFree(spp);
				SeqPortFree(mspp);
				break;
				
			default:
				break;
		}
		/*current_pos += seglen;*/
	}
	/* SeqPortFree(mspp); */
	if(strand == Seq_strand_minus)
	{
		if(start > 0)
			anp->extremes.r_trunc = TRUE;
	}
	else
	{
		if(stop < bsp->length-1)
			anp->extremes.r_trunc = TRUE;
	}
	
	return TRUE;
}


static AlignBlockPtr make_one_block(SeqRangePtr srp, Int4 seq_start, Int4 seq_stop, Int4 left, Int4 right, Int2 order, AlignNodePtr anp)
{
	AlignBlockPtr abp;
	Int4 off_left, off_right;
	
	if(srp == NULL || anp == NULL)
		return NULL;
	if(srp->start > seq_stop || srp->stop < seq_start)
		return NULL;

	if(srp->strand == Seq_strand_minus)
	{
		off_left = MAX(0, (srp->stop - seq_stop));
		off_right = MAX(0, (seq_start - srp->start));
	}
	else
	{
		off_left = MAX(0, (seq_start - srp->start));
		off_right = MAX(0, (srp->stop - seq_stop));
	}
	
	abp = MemNew(sizeof(AlignBlock));
	abp->gr.left = left + off_left;
	abp->gr.right = right - off_right;
	abp->gr.strand = 0;
	if(abp->gr.left == anp->extremes.left&& anp->extremes.strand == Seq_strand_minus)
		abp->gr.strand = Seq_strand_minus;
	if(abp->gr.right == anp->extremes.right && anp->extremes.strand == Seq_strand_plus)
		abp->gr.strand = Seq_strand_plus;
	abp->order = order;
	return abp;
}


static AlignBlockPtr link_align_blocks(AlignBlockPtr PNTR head, AlignBlockPtr new)
{
	AlignBlockPtr curr;

	if(*head == NULL)
		*head = new;
	else
	{
		curr = *head;
		while(curr->next != NULL)
			curr = curr->next;
		curr->next = new;
	}
	return new;
}


static Boolean make_blocks(AlignDataPtr adp, Int4 seq_start, Int4 seq_stop, Int2 order, AlignNodePtr anp)
{
	AlignBlockPtr abp = NULL;
	AlignRangePtr arp;
	

	if(adp == NULL || anp == NULL)
		return FALSE;
	if(adp->arp == NULL)
	{
		abp = make_one_block(&(adp->seqends), seq_start, seq_stop, anp->extremes.left, anp->extremes.right, order, anp);
		if(abp != NULL)
			link_align_blocks(&(anp->blocks), abp);
	}
	else
	{
		for(arp = adp->arp; arp != NULL; arp = arp->next)
		{
			if(arp->segtype == REG_SEG)
			{
				abp = make_one_block(&(arp->sr), seq_start, seq_stop, arp->gr.left, arp->gr.right, order, anp); 
				if(abp != NULL)
				{
					link_align_blocks(&(anp->blocks), abp);
					break;
				}
			}
		}
	}
	return (abp != NULL);
}
		

static Boolean sequence_has_alignment(ValNodePtr align_id_list, SeqIdPtr sip)
{
	Uint1 kludge_factor;
	Int4 gi;
	
	
	kludge_factor = (Uint1)get_kludge_factor(sip, &gi);
	if(gi == -1)
		return FALSE;
		
	while(align_id_list)
	{
		if(align_id_list->choice == kludge_factor)
		{
			if(align_id_list->data.intvalue == gi)
				return TRUE;
		}
		
		align_id_list = align_id_list->next;
	}
	
	return FALSE;
}



static Boolean add_sequence_alignment_info(ValNodePtr align_id_list, ValNodePtr anp_list)
{

	AlignNodePtr anp;
	
	if(align_id_list == NULL || anp_list == NULL)
		return FALSE;
		
	while(anp_list)
	{
		if(anp_list->choice != OBJ_SEQANNOT)
		{
			anp = anp_list->data.ptrvalue;
			if(anp->seq_has_align == FALSE)
				anp->seq_has_align = sequence_has_alignment(align_id_list, anp->sip);
		}
		anp_list = anp_list->next;
	}
	
	return TRUE;
}


typedef struct temp_bsp_data{
	BioseqPtr bsp;
	Uint2 itemID;
	Boolean found;
}TempBsp, PNTR TempBspPtr;

static Boolean bspcountfunc(GatherContextPtr gcp)
{
	TempBspPtr tbp;
	BioseqPtr bsp;

	if(gcp == NULL)
		return FALSE;
	tbp = (TempBspPtr)(gcp->userdata);
	if(tbp == NULL || tbp->bsp == NULL)
		return FALSE;
	if(tbp->found)
		return FALSE;
	bsp = (BioseqPtr)(gcp->thisitem);
	if(tbp->bsp == bsp)
	{
		tbp->itemID= gcp->itemID;
		tbp->found = TRUE;
		return FALSE;
	}
	else
		return TRUE;
}
	
/*****************************************************************
*
*       given the bioseq and its entityID, figure out the 
*       itemID for the Bioseq
*
*****************************************************************/
NLM_EXTERN Uint2 get_bioseq_itemID(BioseqPtr bsp, Uint2 entityID)
{
	GatherScope gs;
	TempBsp tb;


	if(bsp == NULL || entityID == 0)
		return 0;

	tb.bsp = bsp;
	tb.itemID= 0;
	tb.found = FALSE;

	MemSet((Pointer)(&gs), 0, sizeof(GatherScope));
	MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)OBJ_MAX * sizeof(Boolean));
	gs.ignore[OBJ_BIOSEQ] = FALSE;
	GatherEntity(entityID, &tb, bspcountfunc, &gs);

	return tb.itemID;
}
	
	
static Boolean stop_collecting_alignment(ValNodePtr anp_list, Int4 max_num)
{
	Int2 i;

	i = 0 ;
	while(anp_list)
	{
		++i;
		if(i > max_num)
		{
			if(anp_list->next == NULL)
				return TRUE;
		}
		anp_list = anp_list->next;
	}

	return FALSE;
}

static void FindCloneCallback(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	AlignNodePtr anp;
	BioseqPtr bsp;
	BioseqSetPtr bssp;
	ValNodePtr descr;
	ValNodePtr curr;
	OrgRefPtr orp;
	ValNodePtr mod;
	CharPtr str;

	BioSourcePtr source;
	SubSourcePtr ssp;

	anp = (AlignNodePtr)data;
	if(anp->clone_id != NULL)
		return;


	if(sep->choice == 1)
	{
		bsp = (BioseqPtr)(sep->data.ptrvalue);
		descr = bsp->descr;
	}
	else
	{
		bssp = (BioseqSetPtr)(sep->data.ptrvalue);
		descr = bssp->descr;
	}

	for(curr = descr; curr != NULL; curr = curr->next)
	{
		if(curr->choice == Seq_descr_source)
		{
			source = curr->data.ptrvalue;
			/* search for /chromosome= */
			for(ssp = source->subtype; ssp != NULL; ssp = ssp->next)
			{
				if(ssp->subtype == 3 && ssp->name != NULL)
				{	/* 3 == clone */
					anp->clone_id = StringSave(ssp->name);
					return;
				}
			}
		}
		else if(curr->choice == Seq_descr_org)
		{
			orp = curr->data.ptrvalue;
			if(orp)
			{
				for(mod = orp->mod; mod != NULL; mod = mod->next)
				{
					str = mod->data.ptrvalue;
					if(StringNCmp(str, "clone=", 6) == 0)
					{
						anp->clone_id = StringSave(str+6);
					}
				}
			}
		}
	}
}

static Boolean LoadIndexLabelBlock(AlignNodePtr anp)

{
	AlignBlockPtr abp;
	SeqIdPtr sip;
	ObjectIdPtr oip;
	DbtagPtr db_tag;

	sip = anp->sip;
	if(sip == NULL || sip->choice != SEQID_GENERAL)
		return FALSE;

	db_tag = sip->data.ptrvalue;
	if(db_tag == NULL || db_tag->db == NULL)
		return FALSE;
	oip = db_tag->tag;
	if(oip== NULL || oip->id <= 0)
		return FALSE;
	
	
	abp = MemNew(sizeof(AlignBlock));
	MemCopy((Pointer)(&(abp->gr)), (Pointer)(&(anp->extremes)), sizeof(GatherRange));
	abp->order = (Uint2)oip->id;
	anp->blocks = abp;

	anp->label = StringSave(db_tag->db);
	return TRUE;
}


/***********************************************************************
*
*	coll_align_data(align, m_sip, adp, clone, featureOrder, show_mismatch, 
*	itemID, entityID, anp_list)
*	convert all the alignment data stored in adp into the drawing 
*	structure AlignNode
*
*	align: the current Seq-align
*	m_sip: the master sequence, also the target sequence in gather
*	adp: the collected structure from gather
*	clone: for filtering out unwanted clone type. set to NULL for all
*	featureOrder: for features to be displayed together with alignment
*	show_mismatch: show the mismatched base-pairs
*	itemID: itemID for the current align
*	entityID: entityID for the Seq-entry of m_sip
*
*	NOTE: if either show_mismatch or featureOrder is selected, it puts 
*	the newly retrieved sequence for itemID and entityID 
*	
*	anp_list: the list of AlignNodePtr to stored the coverted result
*
************************************************************************/	

static Boolean coll_align_data(SeqAlignPtr align, Uint1 index, AlignDataPtr adp, CollectAlignOptionPtr caop, Int2 itemID, Int2 entityID, Int2 itemType, SeqLocPtr mloc, ValNodePtr PNTR anp_list, ValNodePtr PNTR prev)
{
	Char label[41];
	
	SeqLocPtr slp, extloc;
	SeqIdPtr sip;
	Boolean feat;	/*collect any features?*/
	Boolean show_mismatch;
	Boolean is_master;
	
	AlignRangePtr arp;
	AlignNodePtr anp;
	AlignSegPtr asp, pasp;

	BioseqPtr bsp;
	Uint2 order;
	Int4 e_left = 0, e_right = 0;
	Boolean match_seg;
	Int2 label_size;
	Int4 offset = 0;
	SeqIdPtr best_id;
	SeqEntryPtr sep;

#ifdef NONO
        if(align->segtype == 5) /* Discontinuous aligment not collected */
            return TRUE;
#endif
	label_size = MIN(caop->label_size, 100);
	feat = caop->show_feature;
	if(align->segtype == 3)	/*for std-seg, no feature or mismatch*/
	{
		show_mismatch = FALSE;
		feat = FALSE;
	}
	else
		show_mismatch = caop->show_mismatch;

	if(align->segtype == 2)	/*for Dense-seg or Dense-diag only*/
	{
		if(caop->align_num != -1)
		{
			/* if(stop_collecting_alignment(*anp_list, caop->align_num)) */
			if(caop->curr_align_num > caop->align_num)
			{
				/* ErrPostEx (SEV_WARNING, 0, 0, "The top %ld alignments are displayed. The rest are truncated", caop->align_num);
				return FALSE; */
				feat = FALSE;
				show_mismatch = FALSE;
			}
		}
	}

	while(adp)
	{
		++(caop->curr_align_num);
		anp = MemNew(sizeof (AlignNode));
		anp->pop_sap = NULL;
		anp->itemID = itemID;
		anp->entityID = entityID;
		anp->seqOrder = adp->order;
		anp->chain = adp->chain;
		anp->seq_has_align = FALSE;
		anp->index = index;
		anp->keep_label = FALSE;
		MemCopy(&(anp->extremes), &(adp->extremes), sizeof(GatherRange));

		if(adp->seqends.strand == Seq_strand_minus)
			anp->seqpos = -(adp->seqends.stop);
		else
			anp->seqpos = adp->seqends.start;
			
		pasp = NULL;
		for(arp = adp->arp; arp !=NULL; arp = arp->next)
		{
			asp = MemNew(sizeof(AlignSeg));
			MemCopy(&(asp->gr), &(arp->gr), sizeof(GatherRange));
			asp->type = arp->segtype;
			if(asp->type == INS_SEG)
				asp->ins_pos = asp->gr.left;
			if(pasp == NULL)
				anp->segs = asp;
			else
				pasp->next = asp;
			pasp = asp;
		}
		
		if(index == ALIGN_NON_INDEX  && (feat|| show_mismatch))
		{
			bsp = BioseqLockById(adp->sip);
			if(bsp != NULL)
			{
				if(adp->sip->choice == SEQID_GI)
				{
					sep = SeqEntryFind(adp->sip);
					if(sep != NULL)
						SeqEntryExplore(sep, (Pointer)anp, FindCloneCallback);
				}
				if(bsp->hist && bsp->hist->assembly)
					anp->seq_has_align = TRUE;
				anp->seq_entityID = ObjMgrGetEntityIDForPointer((Pointer)bsp);
				anp->bsp_itemID = get_bioseq_itemID(bsp, anp->seq_entityID);
				best_id = SeqIdFindBest(bsp->id, SEQID_GI);
				if(best_id == NULL)
					best_id = bsp->id;
				anp->sip = SeqIdDup(best_id);
				
				if(feat)
				{
					if(BioseqHasFeature(bsp))
						caop->csop->seglevels = 0;
					else
						caop->csop->seglevels = 1;
					slp = SeqLocIntNew(adp->seqends.start, adp->seqends.stop, adp->seqends.strand, best_id);
					CollectFeatureForAlignNode(slp, anp, caop->csop);
					SeqLocFree(slp);
				}
				if(show_mismatch && bsp->repr != Seq_repr_map)
					CollectMismatchForAlign(anp, mloc, bsp, offset+caop->graphic_offset);

				BioseqUnlock(bsp);
			}
			/* else
				printf("fail to get sequence for %ld\n", adp->sip->data.intvalue);  */
		}
		if(anp->sip == NULL)
			anp->sip = SeqIdDup(adp->sip);
			


		/*collecting matching piece to show the content of a segmented sequence*/			
		if(caop->segloc != NULL && index == ALIGN_NON_INDEX )	
		{
			is_master = SeqIdForSameBioseq(adp->sip, SeqLocId(mloc));
			if(is_master)
			{
				e_left = 0;
				e_right = -1;
			}
			order = 0;
			for(extloc = caop->segloc; extloc != NULL; extloc = extloc->next)
			{
				++order;
				match_seg = FALSE;
				if(is_master)
				{
					e_right += SeqLocLen(extloc);
					match_seg = TRUE;
				}
				else
				{
					sip = SeqLocId(extloc);
					match_seg = SeqIdForSameBioseq(sip, anp->sip);
						
				}
				if(match_seg)
				{
					if(!is_master)
					{
						e_left = SeqLocStart(extloc);
						e_right = SeqLocStop(extloc);
					}
					match_seg = make_blocks(adp, e_left, e_right, order, anp);
				}
				if(is_master)
					e_left = e_right +1;
				if(match_seg)
					if(e_right > adp->seqends.stop)
						break;
			}
		}

		/*store the index information in the blocks*/
		if(index != ALIGN_NON_INDEX)
			LoadIndexLabelBlock(anp);
		else if(label_size > 0)
		{
			if(MuskSeqIdWrite (anp->sip, label, label_size, caop->slabel_format, TRUE, TRUE)) {
			    SeqIdPtr	gilist = GetUseThisGi(align);
			    if (gilist) {
				Char	buf[1024];
				sprintf(buf, "%d", gilist->data.intvalue);
				anp->label = StringSave(buf);
				anp->keep_label = TRUE;
				gilist = SeqIdSetFree(gilist);
			    } else {
				anp->label = StringSave(label);
			    }
			}
		}

		link_data_for_collect (anp_list, prev, (Pointer)anp, (Uint1)itemType);
		adp = adp->next;
	}
	return TRUE;
}
	
static Boolean does_annot_match_target (SeqLocPtr target, SeqAnnotPtr annot)
{
    SeqAlignPtr sap, sap_tmp;
    SeqIdPtr sip;
    DenseDiagPtr ddp;
    DenseSegPtr dsp;
    StdSegPtr ssp;
    SeqIdPtr target_id;
    SeqLocPtr slp;
    Boolean result;

    if(target == NULL || annot == NULL || annot->type != 2)
        return FALSE;
    target_id = SeqLocId(target);
    sap = annot->data;

    if(sap == NULL)
        return FALSE;

    switch(sap->segtype) {
    case 1:
        ddp = sap->segs;
        for(sip = ddp->id; sip != NULL; sip = sip->next)
            if(SeqIdForSameBioseq(sip, target_id))
                return TRUE;
        break;
    case 2:
        dsp = sap->segs;
        for(sip = dsp->ids; sip != NULL; sip = sip->next)
            if(SeqIdForSameBioseq(sip, target_id))
                return TRUE;
        break;
    case 3:
        ssp = sap->segs;
        for(slp = ssp->loc; slp != NULL; slp = slp->next)
            if(SeqIdForSameBioseq(SeqLocId(slp), target_id))
                return TRUE;
        break;
    case 5:

        annot->data = (SeqAlignPtr) sap->segs;
        result =  does_annot_match_target (target, annot);
        annot->data = sap;
        return result;

    default:
        break;
    }
    
    return FALSE;
}
			
static Boolean collalignfunc(GatherContextPtr gcp)
{
	SeqAnnotPtr annot;
	CollectHeaderPtr chp;
	AnnotInfoPtr info;
	SeqAlignPtr align;
	Uint1 annot_type;
	
	
	chp= (CollectHeaderPtr)(gcp->userdata);

	switch(gcp->thistype)
	{
	case OBJ_SEQANNOT:
		annot = (SeqAnnotPtr)(gcp->thisitem);
		if(annot->type == 2)
		{
		chp->caop->curr_align_num = 0;
		chp->load_align = TRUE;
		chp->index = 0;
		if(!chp->take_all_annot)
		{
			if(!is_annot_for_hist_alignment(annot))
			{
				chp->load_align = FALSE;
				return TRUE;
			}
		}
		info = MemNew(sizeof(AnnotInfo));
		info->annotDB[0] = '\0';
		info->displayOrder = get_align_annot_qual(annot, info->annotDB, 20, &annot_type);
		info->annot_type = annot_type;
		if(annot_type == ANNOT_BLAST)
			info->blast_type = info->displayOrder;
		/*load the index values*/
		if(info->annotDB[0] != '\0')
		{
			if(StringCmp(info->annotDB, "Sequencing Status") == 0)
				chp->index = ALIGN_SEQ_INDEX;
			else if(StringCmp(info->annotDB, "Mapping Status") == 0)
				chp->index = ALING_MAP_INDEX;
		}

		/* Eric Green's un-aligned guys */
		if(annot_type == ANNOT_CONSIST)
		{
			info->consistent = info->displayOrder;
			if(info->consistent == ALIGN_UNKNOWN)
			{	/*un-aligned guys, check if the Seq-loc matches */
				if(!does_annot_match_target (chp->slp, annot))
					info = MemFree(info);
				chp->load_align = FALSE;
			}
		}
		else if(annot_type == ANNOT_FISH)
			info->is_fish_align= TRUE;
		if(info != NULL)
		{
			info->entityID = gcp->entityID;
			info->itemID = gcp->itemID;
			link_data_for_collect (&(chp->aligns), &(chp->prev_align), (Pointer)info, (Uint1)(gcp->thistype));
		}
		}
		return TRUE;

	case OBJ_SEQALIGN:
		align = (SeqAlignPtr)(gcp->thisitem);
		if(chp->load_align)
			return coll_align_data(align, chp->index, gcp->adp, chp->caop, gcp->itemID, gcp->entityID, gcp->thistype, chp->slp, &(chp->aligns), &(chp->prev_align));
		else
			return TRUE;
	case OBJ_SEQHIST_ALIGN:
		align = (SeqAlignPtr)(gcp->thisitem);
		return coll_align_data(align, chp->index, gcp->adp, chp->caop, gcp->itemID, gcp->entityID, gcp->thistype, chp->slp, &(chp->aligns), &(chp->prev_align));
	case OBJ_SEQHIST:
		chp->caop->curr_align_num = 0;
		return TRUE;
	default:
		return TRUE;
	}
}

	
/*********************************************************************
*
*	CollectItemForAlignment(slp, entityID, left, caop)
*	return a list of AlignNode for the alignment in the target seqloc
*	slp: the target Seq-loc
*	entityID: the entity source for collection
*	left: the left offset on the graphic
*	caop: the option for alignment collection
*
**********************************************************************/
NLM_EXTERN ValNodePtr CollectItemForAlignment(SeqLocPtr slp, Uint2 entityID, Int4 left, CollectAlignOptionPtr caop, Boolean take_all_annot)
{
	GatherScope gs;
	CollectHeader ch;
	BioseqPtr mbsp;
	SeqIdPtr sip;
	ValNodePtr align_id_list = NULL;
	SeqLocPtr curr, next;

	if(slp == NULL || entityID == 0 || caop == NULL)
		return NULL;

	sip = SeqLocId(slp);
	ch.aligns = NULL;
	ch.caop = caop;
	ch.take_all_annot = take_all_annot;
	ch.load_align = TRUE;
	ch.prev_feat = NULL;
	ch.prev_align = NULL;
	ch.index = 0;
	/*ch.slp = slp;*/
	

	MemSet((Pointer)&gs, 0, sizeof (GatherScope));
	MemSet((Pointer)(gs.ignore), (int)TRUE, (size_t)(OBJ_MAX)*sizeof(Boolean));

	if(caop->only_history == FALSE)
	{
		gs.ignore[OBJ_SEQANNOT] = FALSE;
		gs.ignore[OBJ_SEQALIGN] = FALSE;
	}
	gs.ignore[OBJ_SEQHIST] = FALSE;
	gs.ignore[OBJ_SEQHIST_ALIGN] = FALSE;
	

	gs.nointervals = caop->nointerval;
	gs.seglevels = 0;
	gs.currlevel = 0;
	gs.split_packed_pnt = FALSE;
	gs.mapinsert = caop->map_insert;


	curr = slp;
	while(curr)
	{
		next = curr->next;
		curr->next = NULL;
		gs.offset = left;
		gs.target = curr;
		ch.slp = curr;
		caop->graphic_offset = left;
		GatherEntity(entityID, (Pointer)(&ch), collalignfunc, &gs);
		left += SeqLocLen(curr);
		curr->next = next;
		curr = next;
	}
	
	if(ch.aligns != NULL)
	{
		mbsp = BioseqLockById(sip);
		align_id_list = get_seqids_with_alignment(mbsp);
		if(align_id_list != NULL)
		{
			add_sequence_alignment_info(align_id_list, ch.aligns);
			ValNodeFree(align_id_list);
		}
		BioseqUnlock(mbsp);
	}
			
	
	return ch.aligns;
}


static void merge_master_head(ValNodePtr head, ValNodePtr new_node)
{
	AlignNodePtr anp_head, anp;
	AlignSegPtr asp;
	AlignBlockPtr block;

	if(head == NULL || new_node == NULL)
		return;
	anp_head = head->data.ptrvalue;
	anp = new_node->data.ptrvalue;
	
	if(anp_head == NULL || anp == NULL)
		return;

	anp_head->extremes.right = anp->extremes.right;
	asp = anp_head->segs;
	if(asp == NULL)
		anp_head->segs = anp->segs;
	else
	{
		while(asp->next != NULL)
			asp = asp->next;
		asp->next = anp->segs;
	}
	anp->segs = NULL;

	block = anp_head->blocks;
	if(block == NULL)
		anp_head->blocks = anp->blocks;
	else
	{
		while(block->next != NULL)
			block = block->next;
		if(anp->blocks != NULL)
			block->gr.strand = 0;
		block->next = anp->blocks;
	}
	anp->blocks = NULL;

	FreeAlignNode(new_node);
}
	

/*****************************************************************************
*
*	cllect_master_align_node(m_loc, featureOrder, groupOrder)
*	in the master-slave alignment, a fake Seq-align is created for the 
*	master sequence where the master is aligned to itself. The AlignNode
*	can be computed for this faked alignment. When this is done, the fake
*	Seq-align will be freed
*
*	m_loc: the Seq-loc for the master sequence
*	featureOrder: the selected features
*
*******************************************************************************/
NLM_EXTERN ValNodePtr collect_master_align_node(CollectAlignOptionPtr caop, SeqLocPtr m_loc, Uint1 obj_type, Uint2 entityID)
{
	SeqAlignPtr align;
	DenseSegPtr dsp;
	SeqIdPtr m_sip;
	ValNodePtr anp_node, anp_head = NULL, curr;
	ValNodePtr prev = NULL;
	AlignNodePtr anp;
	AlignDataPtr adp;
	Int4 left =0;
	Boolean show_mismatch;

	if(caop == NULL || m_loc == NULL)
		return NULL;
		
	show_mismatch = caop->show_mismatch;
	caop->show_mismatch = FALSE;
   while(m_loc)
   {
	m_sip = SeqLocId(m_loc);

	dsp = DenseSegNew();
	dsp->dim = 2;
	dsp->numseg =1;
	dsp->strands = MemNew((size_t)2*sizeof(Uint1));
	dsp->strands[0] = Seq_strand_plus;
	dsp->strands[1] = SeqLocStrand(m_loc);
	dsp->ids = SeqIdDup(m_sip);
	dsp->ids->next = SeqIdDup(m_sip);
	dsp->starts = MemNew((size_t)2*sizeof(Int4));
	dsp->starts[0] = SeqLocStart(m_loc);
	dsp->starts[1] = SeqLocStart(m_loc);
	dsp->lens = MemNew(sizeof(Int4));
	dsp->lens[0] = SeqLocLen(m_loc);

	align = SeqAlignNew();
	align->type = 3;
	align->segtype = 2;
	align->dim = 2;
	align->segs = dsp;

	anp_node = NULL;
	adp = gather_align_data(m_loc, align, left, TRUE, TRUE);
	if(adp !=NULL)
	{
		coll_align_data(align, 0, adp, caop, 0, entityID, obj_type, m_loc, &anp_node, &prev);
		FreeAlignData(adp);
	}
	if(anp_head == NULL)
		anp_head = anp_node;
	else
		merge_master_head(anp_head, anp_node);
	SeqAlignFree(align);
	left = SeqLocLen(m_loc);
	m_loc = m_loc->next;
   }
   for(curr = anp_head; curr != NULL; curr = curr->next)
   {
	anp = curr->data.ptrvalue;
	anp->use_seq_ids = TRUE; /*use the Seq-id as the itemID for graphic display*/
	anp->is_master = TRUE;
   }
	caop->show_mismatch = show_mismatch;
   return anp_head;
}

NLM_EXTERN Boolean set_option_for_collect_align(CollectAlignOptionPtr caop, Int2 label_size, Uint1 style)
{
	if(caop == NULL)
		return FALSE;
		
	MemSet((Pointer)caop, 0, sizeof(CollectAlignOption));
	if(style < COLLECT_HISTORY || style > COLLECT_FIXED)
	{
		Message(MSG_ERROR, "Illegal style for alignment display %d", (int)style);
		return FALSE;
	}
	
	caop->nointerval = FALSE;
	caop->label_size= label_size;
	if(style == COLLECT_MD || style == COLLECT_FIXED)
	{
		caop->only_history = FALSE;
		caop->map_insert = FALSE;
	}
	else
	{
		caop->only_history = TRUE;
		caop->map_insert = TRUE;
	}
	caop->map_graphic = (style != COLLECT_FIXED);
	caop->show_mismatch = (style != COLLECT_HISTORY);
	caop->show_feature = FALSE;
	caop->slabel_format = PRINTID_TEXTID_ACCESSION;
	caop->segloc = NULL;
	caop->align_num = DEFAULT_ALIGN_NUM;
	caop->graphic_offset = 0;
	return TRUE;
}

static Boolean alignment_are_blast_hits(BioseqPtr bsp)
{
	SeqAnnotPtr annot;
	Char label[101];
	Uint1 annot_type;

	if(bsp == NULL || bsp->annot == NULL)
		return FALSE;
	for(annot = bsp->annot; annot != NULL; annot = annot->next)
	{
		if(annot->type == 2)
		{
			label[0] = '\0';
			get_align_annot_qual(annot, label, 20, &annot_type);
			if(label[0] != '\0' && StringNCmp(label, "BLAST", 5) == 0)
				return TRUE;
		}
	}
	return FALSE;
}

NLM_EXTERN ValNodePtr collect_anpnode_with_option(CollectAlignOptionPtr caop, SeqLocPtr m_loc, Uint2 entityID, Int4 style, Uint1 itemType, Uint1Ptr f_order, Uint1Ptr g_order, Boolean take_all_annot)
{
	ValNodePtr anp_list = NULL, list;
	BioseqPtr mbsp;
	Uint1 featureOrder[FEATDEF_ANY];
	Uint1 groupOrder[FEATDEF_ANY];
	Int2 i;
	CollectSeqOptionPtr csop = NULL;
	ValNodePtr align_id_list = NULL;
	AlignNodePtr anp;
	Boolean show_feature;
	ValNodePtr prev = NULL;



	if(caop == NULL || m_loc == NULL || entityID == 0)
		return NULL;
	if(style < COLLECT_HISTORY || style > COLLECT_MD)
		return NULL;
	mbsp = BioseqLockById(SeqLocId(m_loc));
	if(mbsp == NULL)
		return NULL;

	if(mbsp->repr == Seq_repr_seg)
		caop->segloc  = (SeqLocPtr)(mbsp->seq_ext);
	else
		caop->segloc = NULL;

	show_feature = FALSE;
	if(style != COLLECT_HISTORY)
	{
		if(f_order != NULL && g_order != NULL)
		{
			MemCopy((Pointer)(featureOrder), (Pointer)f_order, (size_t)(FEATDEF_ANY* sizeof(Uint1)));
			MemCopy((Pointer)(groupOrder), (Pointer)g_order, (size_t)(FEATDEF_ANY* sizeof(Uint1)));
		}
		else	/*use the default features*/
		{
			if(mbsp->mol == Seq_mol_aa)
			{
				MemSet((Pointer)(featureOrder), 1, (size_t)(FEATDEF_ANY* sizeof(Uint1)));
				MemSet((Pointer)(groupOrder), 1, (size_t)(FEATDEF_ANY* sizeof(Uint1)));
				featureOrder[FEATDEF_BAD] = 0;
				/* featureOrder[FEATDEF_ANY] = 0; */ /* out of bounds */
				featureOrder[FEATDEF_PUB] = 0;
				featureOrder[FEATDEF_source] = 0;
				featureOrder[FEATDEF_NUM] = 0;
				featureOrder[FEATDEF_BIOSRC] = 0;
				featureOrder[FEATDEF_ORG] = 0;
				featureOrder[FEATDEF_CDS] =0;
				featureOrder[FEATDEF_PROT] =0;
			}
			else
			{
				MemSet((Pointer)(featureOrder), 0, (size_t)(FEATDEF_ANY* sizeof(Uint1)));
				MemSet((Pointer)(groupOrder), 0, (size_t)(FEATDEF_ANY* sizeof(Uint1)));
				featureOrder[FEATDEF_Imp_CDS] = 1;
				groupOrder[FEATDEF_Imp_CDS] = 1;
				featureOrder[FEATDEF_CDS] = 1;
				groupOrder[FEATDEF_CDS] = 1;
			}
		}

		csop = caop->csop;
		for(i =0; i<FEATDEF_ANY; ++i)
		{
			if(featureOrder[i] != 0)
			{
				csop->features[i] = TRUE;
				show_feature = TRUE;
			}
			else
				csop->features[i] = FALSE;
		}
	}
	else
		caop->show_mismatch = FALSE;



	if(style == COLLECT_MP)
	{
		if(csop->features[FEATDEF_repeat_region] == FALSE || 
			csop->features[FEATDEF_repeat_unit] == FALSE)
		{
			if(mbsp->repr == Seq_repr_seg || mbsp->repr == Seq_repr_raw 
				|| mbsp->repr == Seq_repr_const)
			{
	
				if(alignment_are_blast_hits(mbsp))
				{
					csop->features[FEATDEF_repeat_region] = TRUE;
					csop->features[FEATDEF_repeat_unit] = TRUE;
					csop->features[FEATDEF_repeat_region] = TRUE;
					csop->features[FEATDEF_repeat_unit] = TRUE;
					caop->show_feature = TRUE;
				}
			}
		} 
		anp_list = collect_master_align_node(caop, m_loc, itemType, entityID);
		if(anp_list == NULL)
		{
			BioseqUnlock(mbsp);
			Message(MSG_ERROR, "Fail to make AlignNode for the master sequence");
			return NULL;
		}
		/* if(caop->map_graphic == FALSE)
		{
			csop->features[FEATDEF_repeat_region] = FALSE;
			csop->features[FEATDEF_repeat_unit] = FALSE;
		} */

	}
	caop->show_feature = show_feature;
	
	
	list = CollectItemForAlignment(m_loc, entityID, 0, caop, take_all_annot);
	if(caop->no_sort == FALSE)
		list = SortAlignNode(list);
	ValNodeLink(&anp_list, list);
	if(style == COLLECT_MD)
	{
		for(list = anp_list; list != NULL; list = list->next)
		{
			if(list->choice != OBJ_SEQANNOT)
			{
				anp = list->data.ptrvalue;
				if(anp != NULL)
					anp->use_seq_ids = TRUE;
			}
		}
	}

		
	if(caop->show_feature)
		SortAlignmentFeature(anp_list, featureOrder, groupOrder);
	align_id_list = get_seqids_with_alignment(mbsp);
	if(align_id_list != NULL)
	{
		add_sequence_alignment_info(align_id_list, anp_list);
		ValNodeFree(align_id_list);
	}
	if(style == COLLECT_MP && caop->flat_insert)
		FlatAlignNode(anp_list);

	BioseqUnlock(mbsp);
	return anp_list;
}



/***************************************************************
*
*	CollAlignFromSeqAnnot(annot, m_loc, featureOrder, groupOrder, 
*	style,graphic)
*
*	collect the AlignNode for Seq-aligns stored in Seq-annot
*	annot: the Seq-annot
*	m_loc: the target sequence
*	left: the offset of the leftmost position
*	featureOrder, groupOrde: the features selected to be displayed together 
*	with alignment
*	style: the style of the display. Only valid for multiple-pairwise 
*	and multiple dimension for now
*	graphic: if TRUE, it is designed to show the display on graphic, 
*	so the mismatch data will be collected. Otherwise, it will not 
*	collect mismatch data
*
****************************************************************/
NLM_EXTERN ValNodePtr CollAlignFromSeqAnnot(SeqAnnotPtr annot, SeqLocPtr m_loc, Uint1Ptr featureOrder, Uint1Ptr groupOrder, Uint1 style, Boolean graphic, Boolean sort, Boolean flat_insert)
{
	Uint2 entityID;

	CollectAlignOption ca_option;
	CollectSeqOption cs_option;
	Int2 label_size = 32;

	if(annot->type !=2)	/*it is not an alignment*/
		return NULL;
		
	entityID = ObjMgrRegister(OBJ_SEQANNOT, (Pointer)annot);
	if(entityID == 0)
		return NULL;

		
	if(style == COLLECT_MP || style == COLLECT_MD)
	{
		set_option_for_collect_align(&ca_option, label_size, style);
		cs_option.nointerval = FALSE;
		cs_option.slabel_format = PRINTID_TEXTID_ACCESSION;
		MemSet((Pointer)&(cs_option.flabel_format), OM_LABEL_CONTENT, (size_t)FEATDEF_ANY * sizeof(Uint1));
		cs_option.label_size = label_size;
		cs_option.seglevels = 0;
		ca_option.csop = &cs_option;
		ca_option.no_sort = 1- sort;
		if(style == COLLECT_MP && flat_insert)
			ca_option.flat_insert = TRUE;
		else
			ca_option.flat_insert = FALSE;

		ca_option.only_history = FALSE;
		if(!graphic)
		{
			ca_option.show_mismatch = FALSE;
			ca_option.align_num = -1;
		}
		return collect_anpnode_with_option(&ca_option, m_loc, entityID, style, OBJ_SEQALIGN, featureOrder, groupOrder, TRUE);
	}
	else
		return NULL;


}
	
/*###################################################################
#
#	functions related to collect Seq-feat, Bioseq and Bioseq-seg
#
###################################################################*/


static void get_mapmarker_info(UserObjectPtr uop, Uint4Ptr extra, Uint2Ptr bin_order)
{
	ObjectIdPtr oip;
	Int4 val;
	Uint4 temp;
	UserFieldPtr ufp;

	temp = *extra;

	while(uop)
	{
		oip = uop->type;
		if(oip && oip->str != NULL)
		{
			if(StringCmp(oip->str, "MapMarkerInfo") == 0)
			{
				ufp = uop->data;
				while(ufp)
				{
					oip = ufp->label;
					if(StringCmp(oip->str, "Marker Type") == 0)
					{
						if(ufp->choice == 2)
						{
							val = ufp->data.intvalue;
							switch(val)
							{
								case FRAME_WORK:
									temp |= EXTRA_FRAME_WORK;
									break;
								case RECMIN:
									temp |= EXTRA_RECMIN;
									break;
								case LIKELY:
									temp |= EXTRA_LIKELY;
									break;
								case MDUP:
									temp |= EXTRA_MDUP;
									break;
								case DUP:
									temp |= EXTRA_DUP;
									break;

								case CONTIG_STS:
									temp |= EXTRA_CONTIG_STS;
									break;
								default:
									break;
							}
							*extra = temp;
						}
					}
					if(StringCmp(oip->str, "Bin Order") == 0)
					{
						if(ufp->choice == 2)
							*bin_order = (Uint2)(ufp->data.intvalue);
					}

					if(StringCmp(oip->str, "Marker Category") == 0)
					{
						if(ufp->choice == 2)
						{
							val = ufp->data.intvalue;
							switch(val)
							{
								case EG_YAC_END:
									temp |= EXTRA_YAC_END;
									break;
								case EG_RANDOME:
									temp |= EXTRA_RANDOM;
									break;
	
								case EG_GENETIC:
									temp |= EXTRA_GENETIC;
									break;
	
								case EG_GENE:
									temp |= EXTRA_GENE;
									break;
								case EG_EST:
									temp |= EXTRA_EST;
									break;
								case EG_MISC:
									temp |= EXTRA_MISC;
									break;
								default:
									break;
							}
						}
					}
					ufp = ufp->next;
				}
			}
			else if(StringCmp(oip->str, "Marker Category") == 0)
			{
				ufp = uop->data;
				while(ufp)
				{
					if(ufp->choice == 2)
					{
						val = ufp->data.intvalue;
						switch(val)
						{
							case EG_YAC_END:
								temp |= EXTRA_YAC_END;
								break;
							case EG_RANDOME:
								temp |= EXTRA_RANDOM;
								break;

							case EG_GENETIC:
								temp |= EXTRA_GENETIC;
								break;

							case EG_GENE:
								temp |= EXTRA_GENE;
								break;
							case EG_EST:
								temp |= EXTRA_EST;
								break;
							case EG_MISC:
								temp |= EXTRA_MISC;
								break;
							default:
								break;
						}
					}
					ufp = ufp->next;
				}
			}
		}
		uop = uop->next;
	}
	*extra = temp;
}
	

/*******************************************************************
*
*	ck_seqfeat_extra: check if there is  extra data, such as 
*	Genbank accessions assocated with a GeneRef or Medlines 
*	associated with a Seq-feat
*
*******************************************************************/
NLM_EXTERN Uint4 ck_seqfeat_extra(SeqFeatPtr sfp)
{
	GeneRefPtr grp;
	ValNodePtr db;
	DbtagPtr db_tag;
	ValNodePtr cit;
	ValNodePtr pub;
	Boolean has_gb = FALSE, has_med = FALSE;
	Uint4 extra_data = 0;
	

	if(sfp->data.choice == 1)
	{
		grp = sfp->data.value.ptrvalue;

		for(db = grp->db; db!=NULL; db = db->next)
		{
			db_tag = db->data.ptrvalue;
			if(StringICmp(db_tag->db, "GenBank") == 0)
			{
				extra_data |= EXTRA_GENBANK;
				break;
			}
		}
	}
	for(cit = sfp->cit; !has_med && cit!=NULL; cit = cit->next)
	{
		if(cit->choice == 3)
			has_med = TRUE;
		if(cit->choice ==1)
		{
			pub = (ValNodePtr)(cit->data.ptrvalue);
			while(pub)
			{
				if(pub->choice == PUB_Muid)
				{
					has_med = TRUE;
					break;
				}
				pub = pub->next;
			}
		}
	}

	if(has_med)
		extra_data |= EXTRA_MEDLINE;
	return extra_data;
}
		
			 
		
/******************************************************************
*
*	get_bin_order(sfp)
*	get the 1000:1 bin data()
*
*******************************************************************/
static Uint2 get_bin_order(SeqFeatPtr sfp)
{
	GeneRefPtr grp;
	ValNodePtr db;
	DbtagPtr db_tag;
	ObjectIdPtr oip;

	if(sfp->data.choice != 1)
		return 0;
	grp = sfp->data.value.ptrvalue;
	if(grp == NULL)
		return 0;

	for(db = grp->db; db != NULL; db = db->next)
	{
		db_tag = db->data.ptrvalue;
		if(db_tag != NULL && StringCmp(db_tag->db, "1000:1 Bin") ==0)
		{
			oip = db_tag->tag;
			return (Uint2)(oip->id);
		}
	}

	return 0;
}
	
		
static Boolean load_annot_name(SeqAnnotPtr annot, CharPtr annot_db)
{
	ValNodePtr desc;
	CharPtr name, title;
	Int4 len;

	annot_db[0] = '\0';
	if(annot == NULL)
		return FALSE;
	name = NULL;
	title = NULL;
	for(desc = annot->desc; desc != NULL; desc = desc->next)
	{
		if(desc->choice == Annot_descr_name)
		{
			if(name == NULL)
				name = (CharPtr)(desc->data.ptrvalue);
		}
		if(desc->choice == Annot_descr_title)
		{
			if(title == NULL)
				title = (CharPtr)(desc->data.ptrvalue);
		}
	}

	if(name != NULL)
		StringNCpy_0(annot_db, name, 20);
	len = StringLen(annot_db);
	if(title != NULL && len < 19)
	{
		StringCat(annot_db, ":");
		++len;
		StringNCpy_0(annot_db+len, title, 20-len);
	}

	return (annot_db[0] != '\0');
}


static Boolean check_feature_for_landmark(CharPtr label, GeneDataPtr gdata, SeqFeatPtr sfp, GatherContextPtr gcp, Uint2 priority)
{
	Boolean found;
	GeneDataPtr c_gdp;

	if(gdata == NULL || sfp == NULL)
		return FALSE;
	found = FALSE;
	c_gdp = NULL;
	if(label[0] != '\0')
	{
		for(c_gdp = gdata; c_gdp != NULL; c_gdp = c_gdp->next)
		{
			if(StringICmp(c_gdp->symbol, label) == 0)
			{
				found = TRUE;
				break;
			}
		}
	}
					
	if(!found && sfp->data.choice == 1)
	{
		for(c_gdp = gdata; c_gdp != NULL; c_gdp = c_gdp->next)
		{
			if(check_landmark(sfp, c_gdp->symbol))
			{
				found = TRUE;
				break;
			}
		}
	}

	if(!found)
		return FALSE;
	
	if(c_gdp->priority == 0 || priority < c_gdp->priority)
	{
		c_gdp->entityID = gcp->entityID;
		c_gdp->itemID = gcp->itemID;
		c_gdp->itemType = gcp->thistype;
		c_gdp->priority = priority;
	}

	StringCpy(label, c_gdp->symbol);
	return TRUE;
}



/*******************************************************************
*
*	collseqfunc( )
*	callback function for collecting sequence related data in 
*	gather, such as segments, features
*
*******************************************************************/
static Boolean collseqfunc(GatherContextPtr gcp)
{
	SeqFeatPtr sfp;
	SeqLocPtr slp;
	BioseqPtr bsp;
	CollectHeaderPtr chp;
	FeatNodePtr fnp;
	CollectSeqOptionPtr csop;
	ObjMgrTypePtr   omtp;
	
	UserObjectPtr uop;
	UserFieldPtr ufp;
	GatherRangePtr grp;
	IvalNodePtr inp;
	Uint1 band;
	Int2 i;
	Int2 label_size;
	ValNodePtr delta_node;
	Boolean is_gap;
	SeqLitPtr slitp;


	chp= (CollectHeaderPtr)(gcp->userdata);
	chp->subtype = 0;
	chp->thislabel[0] = '\0';
	chp->ftype [0] = '\0';
	csop = chp->csop;
	label_size = MIN(100, (Int2)(csop->label_size));
		
		
	switch (gcp->thistype)
	{
		case OBJ_SEQANNOT:	/*for the cytogenetic map, skip certain 
						Seq-annot*/
			chp->annotDB[0] = '\0';
			chp->is_lod_score = is_lod_score_annot((SeqAnnotPtr)(gcp->thisitem));
			load_annot_name((SeqAnnotPtr)(gcp->thisitem), chp->annotDB);
			if(csop->bsp_type == CYTO_MAP)
			{
				if(!annot_is_user_defined((SeqAnnotPtr)(gcp->thisitem)))
					chp->skip_feature = TRUE;
				else
					chp->skip_feature = FALSE;
			}
			break;
				
		case OBJ_BIOSEQ_SEG:
			slp = (SeqLocPtr)(gcp->thisitem);
			if(is_map_segment(slp))	/*not very reliable*/
				return TRUE;
			if(chp->maybe_mapid != NULL)
				if(SeqIdMatch(chp->maybe_mapid, SeqLocId(slp)))
					return TRUE;
			fnp = CreateFeatNode (&(chp->features), &(chp->prev_feat), gcp->thistype, gcp->itemID, gcp->entityID, 0);
			MemCopy(&(fnp->extremes), &(gcp->extremes), sizeof(GatherRange));
			if(slp->choice == SEQLOC_NULL || slp->choice == SEQLOC_EMPTY)
				fnp->follower = TRUE;	/*used to present the empty Seq-loc*/
			else
			{
				if(MuskSeqIdWrite (SeqLocId(slp), chp->thislabel, label_size, csop->slabel_format, TRUE, TRUE))
					fnp->label = StringSave(chp->thislabel);
			}
			break;
		case OBJ_BIOSEQ_DELTA:
			delta_node = (ValNodePtr)(gcp->thisitem);
			is_gap = FALSE;
			chp->thislabel[0] = '\0';
			if(delta_node->choice ==1)
			{
				slp = delta_node->data.ptrvalue;
				if(slp->choice == SEQLOC_NULL || slp->choice == SEQLOC_EMPTY)
					is_gap = TRUE;
				else
					MuskSeqIdWrite (SeqLocId(slp), chp->thislabel, label_size, csop->slabel_format, TRUE, TRUE);

			}
			else
			{
				slitp = delta_node->data.ptrvalue;
				if(slitp->length == 0 || slitp->seq_data == NULL)
				{
					is_gap = TRUE;
					if(slitp->length > 0)
						return TRUE;
				}
			}

			fnp = CreateFeatNode (&(chp->features), &(chp->prev_feat), gcp->thistype, gcp->itemID, gcp
->entityID, 0);
			MemCopy(&(fnp->extremes), &(gcp->extremes), sizeof(GatherRange));
			if(is_gap)
				fnp->follower = TRUE;
			else if(chp->thislabel[0] != '\0')
				fnp->label = StringSave(chp->thislabel);
			break;
		
		case OBJ_BIOSEQ:
			fnp = CreateFeatNode (&(chp->features), &(chp->prev_feat), gcp->thistype, gcp->itemID, gcp->entityID, 0);
			MemCopy(&(fnp->extremes), &(gcp->extremes), sizeof(GatherRange));
			bsp = (BioseqPtr) gcp->thisitem;
			if(MuskSeqIdWrite(bsp->id, chp->thislabel, label_size, csop->slabel_format, TRUE, FALSE))
				fnp->label = StringSave(chp->thislabel);
			break;
			
		case OBJ_BIOSEQ_MAPFEAT:
		case OBJ_SEQFEAT:
			if(gcp->thistype == OBJ_SEQFEAT && chp->skip_feature)
				return TRUE;
			sfp = (SeqFeatPtr) gcp->thisitem;
			/* if(gcp->thistype == OBJ_SEQFEAT)
			{
				if(chp->filter_level == gcp->seglevel +1)
					return TRUE;
			} */
			omtp = ObjMgrTypeFind (chp->omp, OBJ_SEQFEAT, NULL, NULL);
			if(omtp == NULL)
				return TRUE;
			if (omtp->subtypefunc != NULL)
				chp->subtype = (*(omtp->subtypefunc)) (gcp->thisitem);
			else
				chp->subtype = 0;
			if(gcp->thistype == OBJ_SEQFEAT && 
				csop->features[chp->subtype] == 0)	/*filter unwanted features*/
				return TRUE;
				
			/*tolerate the unknown band*/
			/*
			if(sfp->data.choice == 14 && gcp->thistype == OBJ_BIOSEQ_MAPFEAT)
			{
				uop = sfp->data.value.ptrvalue;
				band = get_band_type(uop);
				if(band == 0)
					return TRUE;
			}
			*/
			fnp = CreateFeatNode (&(chp->features), &(chp->prev_feat), gcp->thistype, gcp->itemID, gcp->entityID, chp->subtype);

			/*special collection for the LOD scores*/
			if(chp->is_lod_score && gcp->thistype == OBJ_SEQFEAT)	/*it is the LOD score data*/
			{
				fnp->extra_data = EXTRA_LOD_SCORE;
				fnp->bin_order = GetLODScoreBitValue(sfp);
				StringCpy(fnp->annotDB, chp->annotDB);
				MemCopy(&(fnp->extremes), &(gcp->extremes), sizeof(GatherRange));
				return TRUE;
			}

			fnp->has_product = (sfp->product !=NULL);
			fnp->extra_data = ck_seqfeat_extra(sfp);	/*extra data associated with a Gene-ref*/
			get_mapmarker_info(sfp->ext, &(fnp->extra_data), &(fnp->bin_order));
			if(fnp->bin_order == 0)	/*just as a backup*/
				fnp->bin_order = get_bin_order(sfp);
			if(gcp->thistype == OBJ_SEQFEAT && chp->annotDB[0] != '\0')
				StringCpy(fnp->annotDB, chp->annotDB);
			else
				fnp->annotDB[0]= '\0';
			MemCopy(&(fnp->extremes), &(gcp->extremes), sizeof(GatherRange));

			/*special collection for cytogenetic band*/			
			if(sfp->data.choice == 14 && gcp->thistype == OBJ_BIOSEQ_MAPFEAT)
			{
				uop = sfp->data.value.ptrvalue;
				band = get_band_type(uop);
				fnp->band = band;
				fnp->label = StringSave(get_band_name(uop));
				if(band < BAND_POINT)	/*for flybase*/
				{
					for(ufp = uop->data; ufp!=NULL; ufp=ufp->next)
					{
						if(is_label_match(ufp->label, "Subdivision"))
							fnp->pos_label = StringSave(ufp->data.ptrvalue);
					}	
				}
			}
			else	/*for non-cyto band*/
			{
				/* if((gcp->thistype == OBJ_BIOSEQ_MAPFEAT) || collect_feature_label(csop->flabel_format[chp->subtype])) */
				/* check the landmark genes*/

				if(collect_feature_label(csop->flabel_format[chp->subtype]))
				{
					if(omtp->labelfunc !=NULL)
						(*(omtp->labelfunc)) (gcp->thisitem, chp->thislabel, label_size, OM_LABEL_CONTENT);
						fnp->landmark = check_feature_for_landmark(chp->thislabel, chp->gdata, sfp, gcp, chp->priority);
					if(fnp->landmark == FALSE && omtp->labelfunc != NULL && 
						csop->flabel_format[chp->subtype] != OM_LABEL_CONTENT)
						(*(omtp->labelfunc)) (gcp->thisitem, chp->thislabel, label_size, csop->flabel_format[chp->subtype]);

				}					

				if(chp->thislabel[0] != '\0')
					fnp->label = StringSave(chp->thislabel);
				slp = sfp->location;	/*collect the intervals*/
				if(slp->choice == SEQLOC_PACKED_PNT || csop->nointerval == FALSE)
				{
					grp = gcp->rdp;
					for(i=0; (grp!=NULL) && i<gcp->num_interval; ++i)
					{
						inp = MemNew(sizeof(IvalNode));
						MemCopy(&(inp->gr), grp, sizeof(GatherRange));
						ValNodeAddPointer(&(fnp->interval), (Uint1)(i+1), (Pointer)inp);
						++grp;
					}
				}
			}
			break;
		default:
			break;
	}
	
	return TRUE;
}


static Boolean ignore_feature(BoolPtr f_list)
{
	Int2 i;

	if(f_list == NULL)
		return TRUE;
	for(i = 0; i<FEATDEF_ANY; ++i)
		if(f_list[i])
			return FALSE;
	return TRUE;
}
			
/***********************************************************************
*
*	CollectItemForSeqLoc(slp, entityID, left, is_aa, csop)
*	Collect sequences, features for a Seq-loc
*	slp: the target Seq-loc
*	entityID: the top level entityID for the current sequence
*	left: the left offset on the graph
*	is_aa: if TRUE, set get_feats_product flag to TRUE
*	csop: the collection option
*
*
***********************************************************************/	
NLM_EXTERN ValNodePtr CollectItemForSeqLocEx(SeqLocPtr slp, Uint2 entityID, Int4 left, Boolean is_aa, SeqIdPtr maybe_mapid, CollectSeqOptionPtr csop, GeneDataPtr gdata, Uint2 priority, Boolean forceSeglevelsTo1)
{
	GatherScope gs;
	CollectHeader ch;

	if(slp == NULL || entityID == 0 || csop == NULL)
		return NULL;
	ch.omp = ObjMgrGet();	/*set up the options*/
	ch.features = NULL;
	ch.csop = csop;
	ch.maybe_mapid = maybe_mapid;
	ch.filter_level = csop->filter_level;
	ch.gdata = gdata;
	ch.priority = priority;
	ch.skip_feature = FALSE;
	ch.prev_feat = NULL;
	ch.prev_align = NULL;
	ch.index = 0;
	ch.annotDB[0] = '\0';
	ch.is_lod_score = FALSE;
	

	MemSet((Pointer)&gs, 0, sizeof (GatherScope));
	gs.get_feats_location = TRUE;
	gs.get_feats_product = is_aa;
	MemSet((Pointer)(gs.ignore), (int)TRUE, (size_t)(OBJ_MAX)*sizeof(Boolean));

	gs.ignore[OBJ_SEQENTRY] = FALSE;
	gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
	gs.ignore[OBJ_BIOSEQ] = FALSE;
	gs.ignore[OBJ_BIOSEQ_MAPFEAT] = FALSE;
	gs.ignore[OBJ_BIOSEQ_DELTA] = FALSE;
	if(!ignore_feature(csop->features))
	{
		gs.ignore[OBJ_SEQANNOT] = FALSE;
		gs.ignore[OBJ_SEQFEAT] = FALSE;
	}

	gs.nointervals = csop->nointerval;
	gs.seglevels = csop->seglevels;
	/*gs.stop_on_annot = TRUE;*/
	if(gs.seglevels == 0)
	{
		gs.ignore_top = FALSE;
		gs.stop_on_annot = FALSE;
		/* gs.ignore_top = FALSE;
		gs.stop_on_annot = TRUE; */
	}
	else if (forceSeglevelsTo1)
	{
		gs.ignore_top = TRUE; /* JK */
		gs.stop_on_annot = FALSE; /* JK */
	}
	else
	{
		gs.ignore_top = FALSE;
		gs.stop_on_annot = TRUE;
	}
	gs.currlevel = 0;
	gs.split_packed_pnt = TRUE;


	for(; slp!= NULL; slp = slp->next)
	{
		gs.offset = left;
		gs.target = slp;
		GatherEntity(entityID, (Pointer)(&ch), collseqfunc, &gs);
		left += SeqLocLen(slp);
	}
	return ch.features;
}

NLM_EXTERN ValNodePtr CollectItemForSeqLoc(SeqLocPtr slp, Uint2 entityID, Int4 left, Boolean is_aa, SeqIdPtr maybe_mapid, CollectSeqOptionPtr csop, GeneDataPtr gdata, Uint2 priority)
{
	return CollectItemForSeqLocEx(slp, entityID, left, is_aa, maybe_mapid, csop, gdata, priority, FALSE);
}

static Uint1 is_segmap_align_annot(SeqAnnotPtr annot)
{
	UserObjectPtr uop;
	ValNodePtr desc;
	ObjectIdPtr oip;
	UserFieldPtr ufp;
	
	if(annot == NULL)
		return 0;
	if(annot->type != 2)
		return 0;
	if(is_annot_for_hist_alignment(annot))
		return 0;

	desc =annot->desc;
	while(desc)
	{
		if(desc->choice == Annot_descr_user)
		{
			uop = desc->data.ptrvalue;
			if(uop->type)
			{
				oip = uop->type;
				if(StringCmp(oip->str, "SegMap STS Alignment") == 0)
				{
					ufp = uop->data;
					if(ufp && ufp->choice == 2)
						return (Uint1)(ufp->data.intvalue);
				}
			}
		}
		desc = desc->next;
	}
	return 0;
}

static GatherRangePtr create_gr_data(SeqAlignPtr align, SeqLocPtr m_loc, Int4 m_left)
{
	Int2 i, num;
	SeqAlignPtr curr;
	GatherRangePtr grp;

	for(num = 0, curr = align; curr!= NULL; curr = curr->next)
		++num;
	if( num == 0)
		return NULL;

	grp = MemNew((size_t)num * sizeof(GatherRange));
	for(i =0, curr = align; curr != NULL; curr = curr->next, ++i)
	{
		if(!SeqLocOffset (m_loc, curr->bounds, &(grp[i]), m_left))
		{
			grp[i].left = -1;
			grp[i].right = -1;
		}
	}
	return grp;
}

static void add_int_with_order(ValNodePtr PNTR head, Uint1 type, Int4 pos)
{
	ValNodePtr prev, curr;
	ValNodePtr cnew;

	if(*head == NULL)
		ValNodeAddInt(head, type, pos);
	else
	{
		prev = NULL;
		curr = *head;
		cnew = ValNodeNew(NULL);
		cnew->choice = type;
		cnew->data.intvalue = pos;
		while(curr)
		{
			if(pos < curr->data.intvalue)
			{
				if(prev == NULL)
					*head = cnew;
				else
					prev->next = cnew;
				cnew->next = curr;
				return;
			}

			prev = curr;
			curr = curr->next;
		}
		if(prev != NULL)
			prev->next = cnew;
	}
}
					
	
					
static void load_open_close_sts_mark(SeqAlignPtr halign, ValNodePtr anp_list, Uint1 annot_type, SeqLocPtr m_loc, Int4 m_left)
{
	SeqAlignPtr align;
	AlignNodePtr anp;
	AlignSegPtr asp;
	SeqIdPtr sip;
	SeqLocPtr slp;
	GatherRange gr;
	GatherRangePtr grp;
	Int4 start, stop;
	Uint1 strand;
	SeqInt sint;
	SeqLoc sl;
	Boolean collected = FALSE;
	StdSegPtr ssp;
	Int4 e_left, e_right;
	Int2 i;
	
	grp = create_gr_data(halign, m_loc, m_left);
	if(grp == NULL)
		return;
	while(anp_list)
	{
		if(anp_list->choice != OBJ_SEQANNOT)
		{
			anp = anp_list->data.ptrvalue;
			sip = anp->sip;
			if(!SeqIdForSameBioseq(sip, SeqLocId(m_loc)))
			{
			if(anp->seqpos < 0)	/*minus strand*/
			{
				stop = ABS(anp->seqpos);
				start = stop - (anp->extremes.right - anp->extremes.left);
				strand = Seq_strand_minus;
			}
			else
			{
				start = anp->seqpos;
				stop = start + (anp->extremes.right - anp->extremes.left);
				strand = Seq_strand_plus;
			}

			sint.from = start;
			sint.to = stop;
			sint.strand = strand;
			sint.id = sip;
			sl.choice = SEQLOC_INT;
			sl.data.ptrvalue = &sint;
			sl.next = NULL;

			e_left = anp->extremes.left;
			e_right = anp->extremes.right;
			asp = anp->segs;
			for(align = halign, i=0; align != NULL; align = align->next, ++i)
			{
				if(grp[i].left != -1)
				{
					if(!(e_left > grp[i].right || e_right < grp[i].left))
					{
						for(ssp = align->segs; ssp != NULL; ssp = ssp->next)
						{
							/*slp = ssp->loc->next;*/
							slp = ssp->loc;
							while(slp)
							{
								if(SeqIdMatch(SeqLocId(slp), sip))
									break;
								else
									slp = slp->next;
							}
					
							if(slp != NULL)
							{
								if(SeqLocOffset (&sl, slp, &gr, e_left))
								{
									if(asp == NULL)
									{
										asp = MemNew(sizeof(AlignSeg));
										MemCopy(&(asp->gr), &(anp->extremes), sizeof(GatherRange));
										asp->type = REG_SEG;
										anp->segs = asp;
									}
											
									/*ValNodeAddInt(&(asp->mismatch), annot_type, gr.left);*/
									add_int_with_order(&(asp->mismatch), annot_type, gr.left);
									collected = TRUE;
								}
							}
				
						}
					}
					/*if(grp[i].left> e_right)
						break;*/
				}
			}
			}
		}
		anp_list = anp_list->next;
	}
	MemFree(grp);
	
}
					
					
typedef struct segmap_data{
	ValNodePtr anp_list;
	SeqLocPtr m_loc;
	Int4 left;
}SegMapData, PNTR SegMapDataPtr;

static Boolean coll_segmap_func(GatherContextPtr gcp)
{
	ValNodePtr anp_list;
	SeqAnnotPtr annot;
	Uint1 annot_type;
	SeqAlignPtr align;
	SegMapDataPtr smdp;
	
	smdp = (SegMapDataPtr)(gcp->userdata);
	if(smdp == NULL || smdp->anp_list == NULL || smdp->m_loc == NULL)
		return FALSE;
	anp_list = smdp->anp_list;

	annot = (SeqAnnotPtr)(gcp->thisitem);
	if(annot == NULL || annot->type != 2)
		return TRUE;
	annot_type = is_segmap_align_annot(annot);
	if(annot_type == 0)
		return TRUE;
	align = annot->data;
	load_open_close_sts_mark(align, anp_list, annot_type, smdp->m_loc, smdp->left);
	
	return TRUE;
}


/*******************************************************************
*
*	void CollectSegMapSTSAlign( entityID, anp_list)
*	look for the sts alignment from segmap stored as Seq-annot in 
*	in entityID. Add the alignment as the mismatch marker in the 
*	AlignSeg of the anp_list
*
*******************************************************************/
NLM_EXTERN void CollectSegMapSTSAlign( Uint2 entityID, ValNodePtr anp_list, SeqLocPtr m_loc, Int4 m_left)
{
	GatherScope gs;
	SegMapData smd;

	if(entityID == 0 || anp_list == NULL)
		return;

	MemSet((Pointer)&gs, 0, sizeof (GatherScope));
	MemSet((Pointer)(gs.ignore), (int)TRUE, (size_t)(OBJ_MAX)*sizeof(Boolean));
	gs.ignore[OBJ_SEQANNOT] = FALSE;

	smd.anp_list = anp_list;
	smd.m_loc = m_loc;
	smd.left = m_left;
	GatherEntity(entityID, (Pointer)(&smd), coll_segmap_func, &gs);
}


/*#####################################################################
#
#	functions related to the layout for FeatNode
#
######################################################################*/



/***********************************************************************
*  SortFeatNode(list)
*	HeapSort the FeatNode list to the accending order of fnp->left
*	return the head of the sorted list
*	
************************************************************************/
static Uint1Ptr featureSortOrder;
static Uint1Ptr groupSortOrder;

static int FeatNodeIntervalCompare (ValNodePtr vnp1, ValNodePtr vnp2)

{
  GatherRangePtr  grp1, grp2;

  while (vnp1 != NULL && vnp2 != NULL) {
    grp1 = (GatherRangePtr) vnp1->data.ptrvalue;
    grp2 = (GatherRangePtr) vnp2->data.ptrvalue;
    if (grp1 != NULL && grp2 != NULL) {
      /*
      if (grp1->left > grp2->left) {
        return 1;
      } else if (grp1->left < grp2->left) {
        return -1;
      } else if (grp1->right > grp2->right) {
        return 1;
      } else if (grp1->right < grp2->right) {
        return -1;
      }
      */
      if (grp1->left > grp2->left) {
        return -1;
      } else if (grp1->left < grp2->left) {
        return 1;
      } else if (grp1->right > grp2->right) {
        return -1;
      } else if (grp1->right < grp2->right) {
        return 1;
      }
    }
    vnp1 = vnp1->next;
    vnp2 = vnp2->next;
  }
  if (vnp1 != NULL) {
    return -1;
  } else if (vnp2 != NULL) {
    return 1;
  } else {
    return 0;
  }
}

static int LIBCALLBACK FeatNodeCompProc (VoidPtr ptr1, VoidPtr ptr2)
{
  FeatNodePtr  fnp1;
  FeatNodePtr  fnp2;
  ValNodePtr   vnp1;
  ValNodePtr   vnp2;
  GatherRange	gr1, gr2;
  Uint1 group1, group2;
  Uint1 order1, order2;
  int rsult;

  if (ptr1 != NULL && ptr2 != NULL) {
    vnp1 = *((ValNodePtr PNTR) ptr1);
    vnp2 = *((ValNodePtr PNTR) ptr2);
    if (vnp1 != NULL && vnp2 != NULL) {
      fnp1 = (FeatNodePtr) vnp1->data.ptrvalue;
      fnp2 = (FeatNodePtr) vnp2->data.ptrvalue;
      if (fnp1 != NULL && fnp2 != NULL) {
	gr1 = fnp1->extremes;
	gr2 = fnp2->extremes;
        /*
        if (gr1.left > gr2.left) {
          return 1;
        } else if (gr1.left < gr2.left) {
          return -1;
        } else if (gr1.right > gr2.right) {
          return 1;
        } else if (gr1.right < gr2.right) {
          return -1;
        } else */
        if ((rsult = FeatNodeIntervalCompare (fnp1->interval, fnp2->interval)) != 0) {
          return rsult;
        } else {
	  if(featureSortOrder == NULL || groupSortOrder == NULL)
          	return 0;
	  else
	  {
		group1 = groupSortOrder[fnp1->feattype];
		group2 = groupSortOrder[fnp2->feattype];
		if(group1 !=group2)
			return -1;
		order1 = featureSortOrder[fnp1->feattype];
		order2 = featureSortOrder[fnp2->feattype];
		if(order1 < order2)
			return -1;
		if(order1 > order2)
			return 1;
		return 0;
	  }
        }
      } else {
        return 0;
      }
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}




/***********************************************************************
*
*	SortFeatNode(list)
*	sort a list of FeatNode to the ascending order of (extremes.left, 
*	extremes.right)
*
**********************************************************************/

NLM_EXTERN ValNodePtr SortFeatNode(ValNodePtr fnp_list, Uint1Ptr featureOrder, Uint1Ptr groupOrder)
{
	featureSortOrder = featureOrder;
	groupSortOrder = groupOrder;
	return SortValNode(fnp_list, FeatNodeCompProc);
}


static int LIBCALLBACK AlignNodeCompProc (VoidPtr ptr1, VoidPtr ptr2)
{
  AlignNodePtr  anp1, anp2;
  ValNodePtr   vnp1;
  ValNodePtr   vnp2;
  GatherRange	gr1, gr2;

  if (ptr1 != NULL && ptr2 != NULL) {
    vnp1 = *((ValNodePtr PNTR) ptr1);
    vnp2 = *((ValNodePtr PNTR) ptr2);
    if (vnp1 != NULL && vnp2 != NULL) {
      anp1 = (AlignNodePtr) vnp1->data.ptrvalue;
      anp2 = (AlignNodePtr) vnp2->data.ptrvalue;
      if (anp1 != NULL && anp2 != NULL) {
	gr1 = anp1->extremes;
	gr2 = anp2->extremes;
	/* len1 = anp1->extremes.right - anp1->extremes.left;
	len2 = anp2->extremes.right - anp2->extremes.left;
        if(len1 > len2)
          return -1;
        else if(len1 < len2)
          return 1; */

        if (gr1.left > gr2.left) {
          return 1;
        } else if (gr1.left < gr2.left) {
          return -1;
        } else if (gr1.right < gr2.right) {
          return 1;
        } else if (gr1.right > gr2.right) {
          return -1;
        } else {
          return 0;
        }
      } else {
        return 0;
      }
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}

/*sort to make the display showing the alignment that are 
* consistuent of the master sequence first
*/
static ValNodePtr modify_align_node_block(ValNodePtr anp_list)
{
	ValNodePtr block_list = NULL;
	ValNodePtr curr, next, prev = NULL;
	AlignNodePtr anp;

	if(anp_list == NULL)
		return NULL;

	curr = anp_list;
	while(curr)
	{
		next = curr->next;
		anp = curr->data.ptrvalue;
		if(anp->blocks != NULL)
		{
			if(prev == NULL)
				anp_list = curr->next;
			else
				prev->next = curr->next;
			curr->next = NULL;
			ValNodeLink(&block_list, curr);
		}
		else
			prev = curr;
		curr = next;
	}

	if(block_list == NULL)
		return anp_list;
	else
	{
		ValNodeLink(&block_list, anp_list);
		return block_list;
	}
}

		

/***********************************************************************
*
*	SortAlignNode(anp_list)
*	sort a list of AlignNode to the ascending order of (extremes.left, 
*	extremes.right)
*
**********************************************************************/
NLM_EXTERN ValNodePtr SortAlignNode(ValNodePtr anp_list)
{
	ValNodePtr list, curr, prev, last, next;
	ValNodePtr head;

	if(anp_list == NULL)
		return NULL;
	list = anp_list;
	prev = NULL;
	head = NULL;
	while(list != NULL)
	{
		if(prev != NULL)
			prev->next = list;
		while(list && list->choice == OBJ_SEQANNOT)
		{
			if(head == NULL)
				head = list;
			prev = list;
			list = list->next;
		}
		if(list != NULL)
		{
			curr = list;
			last = NULL;
			while(curr && curr->choice != OBJ_SEQANNOT)
			{
				last = curr;
				curr = curr->next;
			}
			next = last->next;
			last->next = NULL;
			list = SortValNode(list, AlignNodeCompProc);
			list = modify_align_node_block(list);
			if(prev == NULL)
				head = 	list;
			else
				prev->next = list;
			while(list->next != NULL)
				list = list->next;
			prev = list;
			list = next;
		}
	}

	return head;
}



/*#######################################################################
#
#	function related to Layout of AlignNode
#
########################################################################*/


/***********************************************************************
*
*	find_insert_ypos(left, seglen, ins, l_bound, r_bound, p_pos, space
*	num)
*	find the level for placing the insertions. Used in both the layout
*	for text and graphic
*	left: to store the left-most position calculated for an insertion
*	seglen: length of the insertion
*	ins: the position for insertions
*	l_bound: the leftmost position in the current line
*	r_bound: the rightmost position in the current line
*	p_pos: position for storing all the layout info
*	num: number of elements in p_pos
*	return the current level found for an insertion
*
***********************************************************************/
NLM_EXTERN Int2 find_insert_ypos(Int4Ptr left, Int4 seglen, Int4 ins, Int4 l_bound, Int4 r_bound, Int4Ptr p_pos, Int4 space, Int2 num)
{
 	Int2 i =0;
	Int4 start, stop;

	--seglen;
	*left = MAX(l_bound, (ins-seglen));
	start = *left;
        for(i =0; i<num; ++i)
	{
		if(p_pos[i] == 0)
		{
			p_pos[i] = (*left + seglen);
             		return i;
		}

		if(ins > (p_pos[i]+space))
		{
			start = (*left);
			start +=MAX(0, (seglen - (ins - (p_pos[i]+space))));
			stop = start+seglen;
			if(stop <= r_bound)
			{
				*left = start;
				p_pos[i] = (*left) + seglen;
				return i;
			}
		}


	}

	return -1;
}


		
/************************************************************************
*
*	convert_gdata_for_featnode(gdata, cyto_loc, offset)
*	gdata: the GeneDataPtr
*	cyto_loc: the current location on the cytogenetic map
*	offset: the offset of cyto_loc to the graphic viewer1
*	for human cytogenetic map, the markers are not shown. But for 
*	the markers that were queried, it will display the interval for 
*	gene data
*
************************************************************************/

NLM_EXTERN ValNodePtr convert_gdata_to_featnode (GeneDataPtr gdata, SeqLocPtr cyto_loc, Int4 offset)
{
	ValNodePtr fnp_node;
	ValNodePtr prev;
	FeatNodePtr fnp;
	SeqLocPtr slp;
	SeqPntPtr spp;
	Boolean mod_fuzz;	/*for the old style of FeatNode. To modify the 
					fuzziness on a point*/
	IntFuzzPtr fuzz;
	GatherRange gr;
	SeqFeatPtr sfp;


	if(gdata == NULL || cyto_loc == NULL)
		return NULL;
	fnp_node = NULL;
	prev = NULL;
	while(gdata)
	{
		slp = NULL;
		sfp = gdata->sfp;
		if(sfp != NULL && sfp->location != NULL)
		{
			mod_fuzz = FALSE;
			if(sfp->location->choice == SEQLOC_PNT)
			{
				spp = sfp->location->data.ptrvalue;
				if(spp->fuzz != NULL)
				{
					fuzz = spp->fuzz;
					if(fuzz->choice == 2)	/*range */
					{
						mod_fuzz = TRUE;
						slp = SeqLocIntNew(fuzz->b, fuzz->a, 0, spp->id);
					}
				}
			}
			if(!mod_fuzz)
				slp = sfp->location;
			if(SeqLocOffset(cyto_loc, slp, &gr, offset))
			{
				fnp = CreateFeatNode (&fnp_node, &prev, gdata->itemType, gdata->itemID, gdata->entityID, gdata->subtype);
				MemCopy(&(fnp->extremes), &gr,  sizeof(GatherRange));
				fnp->label = StringSave(gdata->symbol);
				fnp->landmark = TRUE;
				if(gdata->sfp != NULL)
				{
					fnp->extra_data = ck_seqfeat_extra(gdata->sfp);
					get_mapmarker_info(gdata->sfp->ext, &(fnp->extra_data), &(fnp->bin_order));
				}
			}


			if(mod_fuzz)
				SeqLocFree(slp);
		}
		gdata = gdata->next;
	}

	return fnp_node;
}

				
/*
*	for AlignNode that includes insertions, map the insertion 
*	to gaps on the master sequence
*/

/*the data structure for storing the insertion information*/
typedef struct insert_list {
	Int4 max_size;
	Int4 master_pos;	/*position on the master sequence*/
	Boolean  used;	/* this position is acturally at an inserted segment*/
	Boolean after;	/*does the insertion occurs after the master_pos*/
	struct insert_list PNTR next;
}InsertList, PNTR InsertListPtr;


static void load_insertion_list(InsertListPtr PNTR head, Int4 insert_pos, Int4 insert_size, Boolean after)
{
	InsertListPtr curr, prev, ilp;

	prev = NULL;
	curr = *head;
	while(curr)
	{
		if(curr->master_pos == insert_pos)
		{
			curr->max_size = MAX(curr->max_size, insert_size);
			return;
		}
		if(curr->master_pos > insert_pos)
			break;
		else
			prev = curr;
		curr = curr->next;
	}

	ilp = MemNew(sizeof(InsertList));
	ilp->max_size = insert_size;
	ilp->master_pos = insert_pos;
	ilp->next = curr;
	ilp->after = after;

	if(prev == NULL)
		*head = ilp;
	else
		prev->next = ilp;
}

static void add_offset_to_featnode(ValNodePtr fnp_node, Int4 offset)
{
	FeatNodePtr fnp;
	ValNodePtr interval;
	IvalNodePtr inp;

	while(fnp_node)
	{
		fnp = fnp_node->data.ptrvalue;
		fnp->extremes.left += offset;
		fnp->extremes.right += offset;

		for (interval = fnp->interval; interval != NULL; interval = interval->next)
		{
			inp = interval->data.ptrvalue;
			inp->gr.left += offset;
			inp->gr.right += offset;
		}
		fnp_node = fnp_node->next;
	}
}

NLM_EXTERN void AddOffsetToAlignNode(AlignNodePtr anp, Int4 offset)
{
	AlignSegPtr asp;
	AlignBlockPtr abp;

	anp->extremes.left += offset;
	anp->extremes.right += offset;
	for(abp = anp->blocks; abp != NULL; abp = abp->next)
	{
		abp->gr.left += offset;
		abp->gr.right += offset;
	}

	for(asp = anp->segs; asp != NULL; asp = asp->next)
	{
		if(asp->type == INS_SEG)
		{
			asp->ins_pos += offset;
			asp->gr.left += offset;
		}
		else
		{
			asp->gr.left += offset;
			asp->gr.right += offset;
		}
		if(asp->cnp != NULL)
			add_offset_to_featnode(asp->cnp, offset);
	}
}

static ValNodePtr split_feature_interval(ValNodePtr PNTR p_interval, Int4 offset, 
										 Int4 ins_pos, Int4 ins_size)
{
	ValNodePtr interval, next, prev;
	ValNodePtr second_list = NULL;
	IvalNodePtr inp, new_inp;


	prev = NULL;
	interval = *p_interval; 
	while(interval != NULL)
	{
		next = interval->next;
		inp = interval->data.ptrvalue;
		if(inp->gr.right <= ins_pos)
		{
			inp->gr.left += offset;
			inp->gr.right += offset;
			prev = interval;
		}
		else if(inp->gr.left > ins_pos)
		{
			if(prev == NULL)
				*p_interval = NULL;
			else
				prev->next = NULL;
			return interval;
		}
		else
		{	/*there is overlap */
			new_inp = MemNew(sizeof(IvalNode));
			new_inp->gr.strand = inp->gr.strand;
			/* new_inp->gr.right = inp->gr.right + offset + ins_size;
			new_inp->gr.left = ins_pos + offset + ins_size;	 */
			new_inp->gr.right = inp->gr.right;
			new_inp->gr.left = ins_pos +1;

			inp->gr.left += offset;
			inp->gr.right = ins_pos + offset;
			interval->next = NULL;

			ValNodeAddPointer(&second_list, 0, new_inp);
			ValNodeLink(&second_list, next);
			return second_list;
		}
		interval = next;
	}

	return NULL;
}

static ValNodePtr add_insertion_to_featnode(ValNodePtr PNTR pfnp_node, Int4 offset, 
											Int4 ins_pos, Int4 ins_size)
{
	ValNodePtr fnp_node, next, prev;
	ValNodePtr second_list;
	FeatNodePtr fnp, new_fnp;
	ValNodePtr interval;
	IvalNodePtr inp;

	fnp_node = *pfnp_node;
	second_list = NULL;
	prev = NULL;
	while(fnp_node)
	{
		next = fnp_node->next;
		fnp = fnp_node->data.ptrvalue;
		if(fnp->extremes.right <= ins_pos)
		{
			for (interval = fnp->interval; interval != NULL; interval = interval->next)
			{
				inp = interval->data.ptrvalue;
				inp->gr.left += offset;
				inp->gr.right += offset;
			}
			fnp->extremes.left += offset;
			fnp->extremes.right += offset;
			prev = fnp_node;
		}
		else if(fnp->extremes.left > ins_pos)
		{
			/* fnp->extremes.left += offset + ins_size;
			fnp->extremes.right += offset + ins_size;

			for (interval = fnp->interval; interval != NULL; interval = interval->next)
			{
				inp = interval->data.ptrvalue;
				inp->gr.left += offset + ins_size;
				inp->gr.right += offset + ins_size;
			} */

			fnp_node->next = NULL;
			ValNodeLink(&second_list, fnp_node);
			if(prev == NULL)
				*pfnp_node = next;
			else
				prev->next = next;
		}
		else	/*resides between the insertion points, needs to split the featnode*/
		{
			new_fnp = MemNew(sizeof(FeatNode));
			MemCopy((Pointer)new_fnp, fnp, sizeof(FeatNode));
			if(fnp->label != NULL)
				new_fnp->label = StringSave	(fnp->label);
			if(fnp->pos_label != NULL)
				new_fnp->pos_label = StringSave	(fnp->pos_label);
			if(fnp->annotDB[0] != '\0')
				StringCpy(new_fnp->annotDB, fnp->annotDB);
			/* new_fnp->extremes.right = fnp->extremes.right + offset + ins_size;
			new_fnp->extremes.left = ins_pos + ins_size + offset; */
			new_fnp->extremes.right = fnp->extremes.right;
			new_fnp->extremes.left = ins_pos + 1;
			new_fnp->extremes.strand = fnp->extremes.strand;

			ValNodeAddPointer(&second_list, fnp_node->choice, new_fnp);

			fnp->extremes.left += offset;
			fnp->extremes.right = ins_pos + offset;

			new_fnp->interval = split_feature_interval(&(fnp->interval), offset, 
										 ins_pos, ins_size);

			prev = fnp_node;
		}
		fnp_node = next;
	}

	return second_list;
}


static Int4 find_insertion_size (InsertListPtr ilp, Int4Ptr ins_pos)
{
	while(ilp)
	{
		if(ilp->master_pos == *ins_pos)
		{
			if(ilp->after == FALSE)
				-- (*ins_pos);
			return ilp->max_size;
		}
		ilp = ilp->next;
	}

	return 0;
}
static Int4 get_max_insert_size (InsertListPtr ilp, Int4 from, Int4 to, Int4Ptr insert_pos)
{
	Int4 t_from, t_to;

	while(ilp)
	{
		if(ilp->used == FALSE)	/*it is not used by insertion and mapping*/
		{
			t_from = from;
			t_to = to;
			if(ilp->after == FALSE)
			{
				t_from +=1;
				t_to += 1;
			}
			if(ilp->master_pos >= from && ilp->master_pos <= to)
			{
				*insert_pos = ilp->master_pos;
				if(ilp->after == FALSE)
					--(*insert_pos);
				ilp->used = TRUE;
				return ilp->max_size;
			}
		}
		else if(ilp->master_pos > to)
			return -1;
		ilp = ilp->next;
	}

	return -1;
}

static Int4 get_max_gap_size(InsertListPtr ilp, Int4 from, Int4 to)
{
	Int4 max_size = 0;

	while(ilp)
	{
		if(ilp->used == FALSE)
		{
			if(ilp->master_pos >= from && ilp->master_pos <= to)
			{
				ilp->used = TRUE;
				max_size += ilp->max_size;
			}
		}
		if(ilp->master_pos > to)
			return max_size;

		ilp = ilp->next;
	}
	return max_size;
}


static ValNodePtr add_offset_to_mismatch(ValNodePtr PNTR mismatch, Int4 offset, Int4 ins_pos, Int4 ins_size)
{
	ValNodePtr second_list, prev, curr;

	second_list = NULL;
	curr = *mismatch;
	prev = NULL;
	while(curr)
	{
		if(curr->data.intvalue <= ins_pos || ins_pos == -1)
			curr->data.intvalue += offset;
		else
		{
			ValNodeLink(&second_list, curr);
			if(prev == NULL)
				*mismatch = NULL;
			else
				prev->next = NULL;
			return second_list;

		}
		prev = curr;
		curr = curr->next;
	}
	return second_list;
}

static void reset_insertion_list(InsertListPtr ilp, AlignSegPtr asp)
{
	AlignSegPtr curr;

	while(ilp)
	{
		ilp->used = FALSE;
		for(curr = asp; curr != NULL; curr = curr->next)
		{
			if(curr->ins_pos == ilp->master_pos)
			{
				ilp->used = TRUE;
				break;
			}
		}

		ilp = ilp->next;
	}
}

static void refresh_insertion_list(InsertListPtr ilp)
{
	while(ilp)
	{
		ilp->used = FALSE;
		ilp = ilp->next;
	}
}

static Int4 get_offset_of_insertion(InsertListPtr ilp, Int4 left)
{
	Int4 offset = 0;

	while(ilp)
	{
		if(ilp->master_pos >= left)
			return offset;
		else
			offset += ilp->max_size;
		ilp = ilp->next;
	}

	return offset;
}

static void modify_anp_with_insertion(AlignNodePtr anp, InsertListPtr ilp)
{
	Int4 offset;
	Int4 leftover;
	AlignSegPtr asp, next, new_asp, t_asp, prev;
	Int4 max_insert_size;
	Int4 insert_pos;
	AlignBlockPtr abp, new_abp, next_abp;
	ValNodePtr second_ms_list;


	asp = anp->segs;
	prev = NULL;
	reset_insertion_list(ilp, asp);
	offset = get_offset_of_insertion(ilp, anp->extremes.left);
	anp->extremes.left += offset;
	while(asp)
	{
		next = asp->next;
		if(asp->type == INS_SEG)
		{
			max_insert_size = find_insertion_size (ilp, &(asp->ins_pos));
			if(max_insert_size >0)
			{
				leftover = max_insert_size - asp->gr.right;	/*gr.right is the size of the insertion*/
				/*convert the insertion into a REG_SEG */
				asp->gr.left = asp->ins_pos + offset + 1;	/*insert after */
				asp->gr.right += (asp->gr.left -1);
				asp->type = REG_SEG;
				add_offset_to_featnode(asp->cnp, offset +1);

				/*insert the additional one for gaps*/
				if(leftover > 0)
				{
					new_asp = MemNew(sizeof(AlignSeg));
					new_asp->type = GAP_SEG;
					new_asp->gr.left = asp->gr.right + 1;
					new_asp->gr.right = new_asp->gr.left + leftover -1;
					asp->next = new_asp;
					new_asp->next = next;
				}
				offset += max_insert_size;
			}
			prev = asp;
		}
		else if(asp->type == GAP_SEG)
		{ /*a gap */
			max_insert_size = get_max_gap_size(ilp, asp->gr.left, asp->gr.right);
			asp->gr.left += offset;
			asp->gr.right += max_insert_size + offset;
			offset += max_insert_size;
			prev = asp;
		}
		else if(asp->type == REG_SEG)
		{	/* a diagnol */
			while( asp != NULL && (max_insert_size = 
				get_max_insert_size (ilp, asp->gr.left, 
				asp->gr.right, &insert_pos)) >0)
			{
				/*insertion at the very begining */
				if(insert_pos == -1)
				{
					new_asp = MemNew(sizeof(AlignSeg));
					new_asp->type = GAP_SEG;
					new_asp->gr.left = asp->gr.left;
					new_asp->gr.right = asp->gr.left + max_insert_size -1;
					if(prev == NULL)
						anp->segs = new_asp;
					else
						prev->next = new_asp;
					prev = new_asp;
					new_asp->next = asp;
				}
				else
				{
				if(asp->mismatch != NULL)
					second_ms_list = add_offset_to_mismatch(&(asp->mismatch), offset, 
						insert_pos, max_insert_size);
				else
					second_ms_list = NULL;

				leftover = asp->gr.right - insert_pos;
				asp->gr.left += offset;
				asp->gr.right = insert_pos + offset;

				new_asp = MemNew(sizeof(AlignSeg));
				new_asp->type = GAP_SEG;
				new_asp->gr.left = asp->gr.right + 1;
				new_asp->gr.right = insert_pos + offset + max_insert_size;
				new_asp->next = next;
				t_asp = asp;
				asp->next = new_asp;
				asp = new_asp;

				if(leftover > 0)
				{
					new_asp = MemNew(sizeof(AlignSeg));
					new_asp->type = REG_SEG;
					new_asp->gr.left = insert_pos +1;
					new_asp->gr.right = insert_pos + leftover;
					asp->next = new_asp;
					new_asp->next = next;
					new_asp->cnp = add_insertion_to_featnode(&(t_asp->cnp), offset, 
										insert_pos, max_insert_size);
					new_asp->mismatch = second_ms_list;
					new_asp->next = next;
					asp->next = new_asp;
					asp = new_asp;
					prev = new_asp;
				}
				else
				{
					add_offset_to_featnode(asp->cnp, offset);
					prev = asp;
					asp = NULL;
					break;
				}
				}

				offset += max_insert_size;
			}	/*end of while*/
			if(asp != NULL)
			{

				asp->gr.left += offset;
				asp->gr.right += offset;
				if(asp->cnp)
					add_offset_to_featnode(asp->cnp, offset);
				if(asp->mismatch != NULL)
					add_offset_to_mismatch(&(asp->mismatch), offset, -1, -1);
				prev = asp;
			}
		}
		asp = next;
	}
	anp->extremes.right += offset;

	if(offset > 0 && anp->blocks != NULL)
	{
		refresh_insertion_list(ilp);
		abp = anp->blocks;
		while(abp)
		{
			next_abp = abp->next;
			abp->next = NULL;
			offset = get_offset_of_insertion(ilp, abp->gr.left);
			while( abp && (max_insert_size = get_max_insert_size (ilp, 
					abp->gr.left, abp->gr.right, &insert_pos)) > 0)
			{
				/*insertion at the very begining */
				if(insert_pos == -1)
					offset += max_insert_size;
				else
				{
					leftover = abp->gr.right - insert_pos;
					if(leftover > 0)
					{
						new_abp = MemNew(sizeof(AlignBlock));
						new_abp->gr.left = insert_pos + 1;
						new_abp->gr.right = abp->gr.right;
						new_abp->order = abp->order;
						new_abp->next = next_abp;

						abp->gr.left += offset;
						abp->gr.right = insert_pos + offset;
						abp->next = new_abp;
						if(abp->gr.strand != Seq_strand_minus)
						{
							new_abp->gr.strand = abp->gr.strand;
							abp->gr.strand = 0;
						}
						abp = new_abp;
						offset += max_insert_size;
					}
					else	/*reach the end */
						break;
				}
			}
			if(abp != NULL)
			{
				abp->gr.left += offset;
				abp->gr.right += offset;
				abp->next = next_abp;
			}
			abp = next_abp;
		}
	}
}

static void free_insert_list(InsertListPtr ilp)
{
	InsertListPtr next;

	while(ilp)
	{
		next = ilp->next;
		MemFree(ilp);
		ilp = next;
	}
}


NLM_EXTERN Boolean FlatAlignNode(ValNodePtr anp_list)
{
	ValNodePtr curr;
	AlignNodePtr master_anp, anp;
	AnnotInfoPtr annot_info;
	Uint1 align_type;
	InsertListPtr ilp;
	AlignSegPtr asp;
	Int4 p_pos;

	
	master_anp = NULL;
	for(curr = anp_list; curr != NULL; curr = curr->next)
	{
		if(curr->choice == OBJ_SEQANNOT)
		{
			annot_info = curr->data.ptrvalue;
			align_type = get_alignment_type (annot_info);
			if(align_type == ALIGN_DNA_TO_PROT || 
				align_type == ALIGN_PROT_TO_DNA || align_type == ALIGN_TDNA_TO_TDNA)
				return FALSE;
		}
		else
		{
			anp = curr->data.ptrvalue;
			if(anp->is_master)
				master_anp = anp;
		}
	}
	if(master_anp == NULL)
		return FALSE;

	/*load all the insertions in the alignments*/
	ilp = NULL;
	for(curr = anp_list; curr != NULL; curr = curr->next)
	{
		if(curr->choice != OBJ_SEQANNOT)
		{
			anp = curr->data.ptrvalue;
			if(anp != master_anp)
			{
				p_pos = -1;
				for(asp = anp->segs; asp != NULL; asp = asp->next)
				{
					if(asp->type == INS_SEG)
						load_insertion_list(&ilp, asp->ins_pos, asp->gr.right, (Boolean)(p_pos == asp->ins_pos));
					else if(asp->type != GAP_SEG)
						p_pos = asp->gr.right;
				}
			}
		}
	}

	if(ilp == NULL)
		return FALSE;

	/*do the real flatting*/
	for(curr = anp_list; curr != NULL; curr = curr->next)
	{
		if(curr->choice != OBJ_SEQANNOT)
		{
			anp = curr->data.ptrvalue;
			modify_anp_with_insertion(anp, ilp);
		}
	}


	free_insert_list(ilp);
	return TRUE;
}


/*
*	Delete all the bad YACs from the list
*	anything on the NHGRI map that is recorded inconsistent will 
*	be considered inconsistent. For the Whitehead map, the 
*	inconsistent+ambiguous is inconsistent. Inconsistent alone 
*	is not considered inconsistent
*/
static Boolean is_ambiguous_annot(AnnotInfoPtr info, Uint1 db)
{
	if(info == NULL)
		return FALSE;
	if(info->annot_type == ANNOT_CONSIST)
	{
		if(info->consistent == ALIGN_CONSISTENT)
			return FALSE;
		else
		{
			if(info->consistent == ALIGN_INCONSISTENT)
			{
				if(db == YAC_NHGRI)
					return TRUE;
				else if(db == YAC_MIT)
				{	/*inconsistent and ambiguous are different*/
					if(StringCmp(info->annotDB, "Ambiguous") == 0)
					
						return TRUE;
					else
						return FALSE;
				}
			}
		}
	}

	return FALSE;
}

/*
*	delete any of the whitehead yacs that only contains 
*	ambiguous STS hits
*/
static Boolean delete_alignnode (AlignNodePtr anp, Uint1 db)
{
	AlignSegPtr asp;
	ValNodePtr curr;
	Boolean has_sts_hits;

	has_sts_hits = FALSE;
	for(asp = anp->segs; asp != NULL; asp = asp->next)
	{
		if(asp->mismatch != NULL)
		{
			for(curr = asp->mismatch; curr != NULL; curr = curr->next)
			{
				if(curr->choice == MISMATCH_CLOSE)	/*unambiguous hits*/
					return FALSE;
				if(curr->choice == MISMATCH_SQUARE)
				{
					if(db == YAC_NHGRI)
						return FALSE;
				}
			}
			has_sts_hits = TRUE;
		}
	}

	return (has_sts_hits == FALSE);
}


/*
*	Delete all the bad YACs from the list
*	anything on the NHGRI map that is recorded inconsistent will 
*	be considered inconsistent. For the Whitehead map, the 
*	inconsistent+ambiguous is inconsistent. Inconsistent alone 
*	is not considered inconsistent
*/
NLM_EXTERN void CleanUpAmbiguousYAC (ValNodePtr PNTR anp_node, Uint1 db, SeqIdPtr chr_id)
{
	AnnotInfoPtr info;
	AlignNodePtr anp;
	ValNodePtr curr, prev, next;
	Boolean del_annot;
	Boolean del;

	prev = NULL;
	del_annot = FALSE;

	curr = *anp_node;
	while(curr)
	{
		next = curr->next;
		del = FALSE;
		if(curr->choice == OBJ_SEQANNOT)
		{
			info = curr->data.ptrvalue;
			del_annot = is_ambiguous_annot(info, db);
		}
		else if(!del_annot || chr_id != NULL)
		{
			anp = curr->data.ptrvalue;
			if(chr_id != NULL)
			{
				if(anp->sip != NULL)
				{
					if(SeqIdMatch(chr_id, anp->sip))
						del = TRUE;
				}
			}
			if(!del)
				del = delete_alignnode (anp, db);
		}
		if(del_annot)
			del = TRUE;

		if(del)
		{
			if(prev == NULL)
				*anp_node = next;
			else
				prev->next = next;
			curr->next = NULL;
			FreeAlignNode(curr);
		}
		else
			prev = curr;
		curr = next;
	}
}


	
/*****************************************************************
*
*	check if the AlignNode only contains Seq-annot or it 
*	has real sequence alignment. 
*	the empty Seq-annot may be the unaligned contigs in 
*	Eric Green's map
*
******************************************************************/
NLM_EXTERN Boolean alignode_has_alignments(ValNodePtr aligns)
{
	while(aligns)
	{
		if(aligns->choice != OBJ_SEQANNOT)
			return TRUE;
		aligns = aligns->next;
	}

	return FALSE;
}







	
