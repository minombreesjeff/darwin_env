static char const rcsid[] = "$Id: xmlblast.c,v 6.33 2004/04/29 19:55:35 dondosha Exp $";

/* $Id: xmlblast.c,v 6.33 2004/04/29 19:55:35 dondosha Exp $ */
/**************************************************************************
*                                                                         *
*                             COPYRIGHT NOTICE                            *
*                                                                         *
* This software/database is categorized as "United States Government      *
* Work" under the terms of the United States Copyright Act.  It was       *
* produced as part of the author's official duties as a Government        *
* employee and thus can not be copyrighted.  This software/database is    *
* freely available to the public for use without a copyright notice.      *
* Restrictions can not be placed on its present or future use.            *
*                                                                         *
* Although all reasonable efforts have been taken to ensure the accuracy  *
* and reliability of the software and data, the National Library of       *
* Medicine (NLM) and the U.S. Government do not and can not warrant the   *
* performance or results that may be obtained by using this software,     *
* data, or derivative works thereof.  The NLM and the U.S. Government     *
* disclaim any and all warranties, expressed or implied, as to the        *
* performance, merchantability or fitness for any particular purpose or   *
* use.                                                                    *
*                                                                         *
* In any work or product derived from this material, proper attribution   *
* of the author(s) as the source of the software or data would be         *
* appreciated.                                                            *
*                                                                         *
************************************************************************** 
* File Name:  xmlblast.c
*
* Author:  Sergei B. Shavirin
*   
* Version Creation Date: 05/17/2000
*
* $Revision: 6.33 $
*
* File Description:  Functions to print simplified BLAST output (XML)
*
* 
* $Log: xmlblast.c,v $
* Revision 6.33  2004/04/29 19:55:35  dondosha
* Mask filtered locations in query sequence lines
*
* Revision 6.32  2004/03/31 17:58:23  dondosha
* Added PSIXmlReset function to allow keeping the AsnIoPtr between outputs for multiple queries in blastpgp
*
* Revision 6.31  2003/08/04 16:19:16  dondosha
* Added effective HSP length (length adjustment) to other returns, so it can be reported in XML output
*
* Revision 6.30  2003/05/30 17:25:38  coulouri
* add rcsid
*
* Revision 6.29  2003/03/21 21:01:16  camacho
* Fixed inversion of subject and query sequences for tblastx
*
* Revision 6.28  2003/02/19 15:42:32  madden
* Check glb_matrix before freeing
*
* Revision 6.27  2003/01/28 16:57:11  dondosha
* For single query, call the old BXMLPrintOutput function from BXMLPrintMultiQueryOutput
*
* Revision 6.26  2003/01/23 20:02:53  dondosha
* Distinguish between blastn and megablast programs in multi-query XML output
*
* Revision 6.25  2003/01/23 19:55:20  dondosha
* Added the closing part for multi-query XML output.
*
* Revision 6.24  2003/01/06 23:01:40  dondosha
* Added function to create a multi-query XML output for web megablast
*
* Revision 6.23  2002/11/14 15:37:18  dondosha
* Added functions to extract all hit information from seqalign that can be extracted without loading sequences
*
* Revision 6.22  2002/07/17 22:28:13  dondosha
* Added support for megablast XML output
*
* Revision 6.21  2002/04/23 20:48:24  madden
* Fix hsp_count for ungapped case
*
* Revision 6.20  2002/03/08 21:34:57  madden
* If no title for Bioseq use "No definition line found"
*
* Revision 6.19  2002/01/15 21:56:38  madden
* Fixes from Jed Wing (Turbogenomics) for ungapped runs
*
* Revision 6.18  2001/05/01 20:54:32  madden
* Set glb_matrix in BXMLSeqAlignToHits if not already set
*
* Revision 6.17  2001/02/28 21:37:25  shavirin
* Fixed XML printing in case when definition line is missing.
*
* Revision 6.16  2001/01/31 18:43:49  dondosha
* Test whether subject Bioseq is found before trying to show the hit
*
* Revision 6.15  2001/01/19 20:02:17  dondosha
* Set the query and hit frames depending on strand for blastn output
*
* Revision 6.14  2000/11/28 20:51:57  shavirin
* Adopted for usage with mani-iterational XML definition.
*
* Revision 6.13  2000/11/22 21:55:49  shavirin
* Added function BXMLPrintOutputEx() with new parameter iteration_number
* for usage with PSI-Blast.
*
* Revision 6.12  2000/11/17 21:49:33  shavirin
* Changed order from/to for negative strand AFTER retrieval of the sequence.
*
* Revision 6.11  2000/11/08 21:39:25  shavirin
* Changed order from/to depending on strand and frame.
*
* Revision 6.10  2000/11/08 20:49:05  shavirin
* Added paramter "score" as in Traditional Blast Output. Previous score
* changed to "bit_score".
*
* Revision 6.9  2000/11/08 20:09:32  shavirin
* Added new parameter align_len analogos to the number reported in
* the Traditional Blast Output.
*
* Revision 6.8  2000/11/07 21:51:22  shavirin
* Added check if query sequence is available for retrieval.
*
* Revision 6.7  2000/10/23 22:12:46  shavirin
* Added possibility to create XML with error message in case of failure or
* no hits.
*
* Revision 6.6  2000/10/23 19:56:06  dondosha
* AsnIo should be opened and closed outside function BXMLPrintOutput
*
* Revision 6.5  2000/10/12 21:35:31  shavirin
* Added support for OOF alignment.
*
* Revision 6.4  2000/08/22 14:53:00  shavirin
* Changed variable from enthropy to entropy.
*
* Revision 6.3  2000/08/11 16:54:17  kans
* return FALSE instead of NULL for Mac compiler
*
* Revision 6.2  2000/08/10 14:42:33  shavirin
* Added missing comment.
*
* Revision 6.1  2000/08/09 20:40:04  shavirin
* Initial revision.
* *
*
*/

#include <xmlblast.h>

static Int4Ptr PNTR glb_matrix;

Boolean BXMLGetSeqLineForDenseDiag(DenseDiagPtr ddp, HspPtr hsp, Int4 length,
                                   Boolean is_aa, Int4Ptr PNTR matrix)
{
    SeqIdPtr m_id, t_id;
    SeqInt si;
    SeqLoc sl;
    Int4 i;
    Uint1 m_res, t_res;
    SeqPortPtr m_spp, t_spp;
    
    if(ddp == NULL || hsp == NULL)
        return FALSE;
    
    hsp->qseq = MemNew(length+1);
    hsp->hseq = MemNew(length+1);
    hsp->midline = MemNew(length+1);

    sl.choice = SEQLOC_INT;
    sl.data.ptrvalue = &si;

    /* !! Here we assume, that this is 2-dimensional DenseDiag where
       first element is query and second element is database sequence 
       This is the case for ALL non-gapped blastp and blastn SeqAligns */
    
    /* SeqLoc for query sequence */
    
    m_id = SeqIdDup(ddp->id);
    si.id = m_id;
    si.from = hsp->query_from;
    si.to = hsp->query_to;
    si.strand = (ddp->strands == NULL) ? 0 : ddp->strands[0];

    m_spp = SeqPortNewByLoc(&sl, is_aa ? Seq_code_ncbieaa : Seq_code_iupacna);
    
    /* SeqLoc for the subject */
    
    t_id = SeqIdDup(ddp->id->next);
    si.id = t_id;
    si.from = hsp->hit_from;
    si.to = hsp->hit_to;
    si.strand = (ddp->strands == NULL) ? 0 : ddp->strands[1];
    
    t_spp = SeqPortNewByLoc(&sl, is_aa ? Seq_code_ncbieaa : Seq_code_iupacna);
    
    if(m_spp == NULL || t_spp == NULL) {
        if(m_spp == NULL)
            SeqPortFree(m_spp);
        if(t_spp != NULL)
            SeqPortFree(t_spp);
        return FALSE;
    }
    
    for(i = 0; i < length; ++i) {
        m_res = SeqPortGetResidue(m_spp);
        t_res = SeqPortGetResidue(t_spp);
        
        hsp->qseq[i] = m_res;
        hsp->hseq[i] = t_res;
        
        if(m_res == t_res) {
            hsp->midline[i] = is_aa ? m_res : '|';
        } else if(matrix != NULL && is_aa) {
            if (IS_residue(m_res) && IS_residue(t_res) && 
                matrix[m_res][t_res] >0) {
                hsp->midline[i] = '+';
            } else {
                hsp->midline[i] = ' ';
            }
        } else {
            hsp->midline[i] = ' ';
        }
    }

    SeqIdFree(t_id);
    SeqIdFree(m_id);
    SeqPortFree(m_spp);
    SeqPortFree(t_spp);
    
    return TRUE;
}
Boolean BXMLGetSeqLineForDenseSeg(DenseSegPtr dsp, HspPtr hsp, Int4 length,
                                  Boolean is_aa, Int4Ptr PNTR matrix)
{

    SeqInt msi, tsi;
    SeqIntPtr sint;
    SeqLoc sl;
    Int2 i, k;
    Int2 dim;
    Int2 m_order, t_order;	/*order of the master and the target sequence*/
    Int4 index, abs_index;
    Int4 j, val, t_val;
    Uint1 m_res, t_res, stdaa_res;
    SeqIdPtr sip;
    SeqPortPtr m_spp, t_spp;
    SeqMapTablePtr smtp;
    
    
    if(dsp == NULL || hsp == NULL)
        return FALSE;
    
    hsp->qseq = MemNew(length+1);
    hsp->hseq = MemNew(length+1);
    hsp->midline = MemNew(length+1);

    m_order = 0;
    t_order = 1;
    dim = 2;
    
    msi.id = SeqIdDup(dsp->ids);
    msi.from = hsp->query_from;
    msi.to = hsp->query_to;
    msi.strand = (dsp->strands == NULL) ? 0 : dsp->strands[m_order];
    
    tsi.id = dsp->ids->next;
    tsi.from = hsp->hit_from;
    tsi.to = hsp->hit_to;
    tsi.strand = (dsp->strands == NULL) ? 0 : dsp->strands[t_order];
        
    sl.choice = SEQLOC_INT;
    sl.data.ptrvalue = &msi;
    m_spp = SeqPortNewByLoc(&sl, 
                            (is_aa) ? Seq_code_ncbieaa : Seq_code_iupacna);
    
    sl.choice = SEQLOC_INT;
    sl.data.ptrvalue = &tsi;
    t_spp = SeqPortNewByLoc(&sl, 
                            (is_aa) ? Seq_code_ncbieaa : Seq_code_iupacna);
    
    abs_index = 0;
    for(i = 0; i<dsp->numseg; ++i) {
        val = dsp->starts[i*dim + m_order];
        t_val = dsp->starts[i*dim + t_order];
        if(val == -1 || t_val == -1) {
            
            if(val == -1 && t_val == -1) /* never should happend */
                continue;
            
            if(val != -1) {
                index = dsp->lens[i];
                while (index > 0) {
                    index--;
                    m_res = SeqPortGetResidue(m_spp);
                    hsp->qseq[abs_index] = m_res;
                    hsp->hseq[abs_index] = '-';
                    hsp->midline[abs_index] = ' ';
                    abs_index++;
                }
            }
            if(t_val != -1) {
                index = dsp->lens[i];
                while (index > 0) {
                    index--;
                    t_res = SeqPortGetResidue(t_spp);
                    hsp->qseq[abs_index] = '-';
                    hsp->hseq[abs_index] = t_res;
                    hsp->midline[abs_index] = ' ';
                    abs_index++;
                }
            }
        } else {
            for(j = 0; j<dsp->lens[i]; ++j) {
                m_res = SeqPortGetResidue(m_spp);
                t_res = SeqPortGetResidue(t_spp);

                hsp->qseq[abs_index] = m_res;
                hsp->hseq[abs_index] = t_res;

                if(m_res == t_res) {
                    if(is_aa)
                        hsp->midline[abs_index] = m_res;
                    else
                        hsp->midline[abs_index] = '|';
                    
                } else if(matrix != NULL && is_aa && 
                          IS_residue(m_res) && IS_residue(t_res)) {
                    
                    if(matrix[m_res][t_res] >0)
                        hsp->midline[abs_index] = '+';
                    else
                        hsp->midline[abs_index] = ' ';
                    
                } else {
                    hsp->midline[abs_index] = ' ';
                }
                
                abs_index++;
            }
        }
    }

    SeqIdFree(msi.id);
    SeqPortFree(m_spp);
    SeqPortFree(t_spp);
    return TRUE;
}
Boolean BXMLGetSeqLineForStdSeg(StdSegPtr ssp, HspPtr hsp, Int4 length,
                                Int4Ptr PNTR matrix)
{
    Boolean master_is_translated=FALSE, both_translated=FALSE;
    Boolean target_is_translated = FALSE;
    CharPtr genetic_code1, genetic_code2;
    SeqPortPtr spp1, spp2;
    Uint1 codon[4], residue1, residue2;
    Int4 abs_index;
    Boolean  ungapped_align = FALSE;
    
    if(ssp == NULL || hsp == NULL)
        return FALSE;
    
    hsp->qseq = MemNew(length+1);
    hsp->hseq = MemNew(length+1);
    hsp->midline = MemNew(length+1);
    
    /* Check for valid sequence. */
    if (SeqLocLen(ssp->loc) == 3*SeqLocLen(ssp->loc->next))
        master_is_translated = TRUE;
    else if (3*SeqLocLen(ssp->loc) == SeqLocLen(ssp->loc->next))
        target_is_translated = TRUE;	
    else if (SeqLocLen(ssp->loc) == SeqLocLen(ssp->loc->next))
        both_translated = TRUE;
    else
        return FALSE;
    
    if (master_is_translated) {
        genetic_code1 = GetGeneticCodeFromSeqId(ssp->ids);
    } else if (both_translated) {
        genetic_code1 = GetGeneticCodeFromSeqId(ssp->ids);
        genetic_code2 = GetGeneticCodeFromSeqId(ssp->ids->next);
    } else {
        genetic_code1 = GetGeneticCodeFromSeqId(ssp->ids->next);
    }
    
    
    for(abs_index = 0; ssp != 0 && abs_index < length; ssp = ssp->next) {

        if (ssp->loc->choice != SEQLOC_EMPTY || ssp->loc->next->choice != SEQLOC_EMPTY) {
            /* Non - gap element */
            if (ssp->loc->choice != SEQLOC_EMPTY && ssp->loc->next->choice != SEQLOC_EMPTY) {
                if (both_translated) { /* TBLASTX */
                    spp1 = SeqPortNewByLoc(ssp->loc, Seq_code_ncbi4na);
                    spp2 = SeqPortNewByLoc(ssp->loc->next, Seq_code_ncbi4na);
                    while ((codon[0]=SeqPortGetResidue(spp2)) != SEQPORT_EOF) {
                        codon[1] = SeqPortGetResidue(spp2);
                        codon[2] = SeqPortGetResidue(spp2);
                        residue1 = AAForCodon(codon, genetic_code1);
                        codon[0] = SeqPortGetResidue(spp1);
                        codon[1] = SeqPortGetResidue(spp1);
                        codon[2] = SeqPortGetResidue(spp1);
                        residue2 = AAForCodon(codon, genetic_code2);
                        
                        hsp->qseq[abs_index] = residue2;
                        hsp->hseq[abs_index] = residue1;
                        
                        if (residue1 == residue2)
                            hsp->midline[abs_index] = residue1;
                        else if (matrix != NULL && matrix[residue1][residue2] >0)
                            hsp->midline[abs_index] = '+';
                        else
                            hsp->midline[abs_index] = ' ';
                        
                        abs_index++;
                        
                    }
                } else {     /* TBLASTN or BLASTX */
                    if (master_is_translated) { /* BLASTX */
                        spp1 = SeqPortNewByLoc(ssp->loc, Seq_code_ncbi4na);
                        spp2 = SeqPortNewByLoc(ssp->loc->next, Seq_code_ncbieaa);
                    } else {                    /* TBLASTN */
                        spp2 = SeqPortNewByLoc(ssp->loc, Seq_code_ncbieaa);
                        spp1 = SeqPortNewByLoc(ssp->loc->next, Seq_code_ncbi4na);
                    }
                    
                    while ((residue1=SeqPortGetResidue(spp2)) != SEQPORT_EOF) {
                        codon[0] = SeqPortGetResidue(spp1);
                        codon[1] = SeqPortGetResidue(spp1);
                        codon[2] = SeqPortGetResidue(spp1);
                        residue2 = AAForCodon(codon, genetic_code1);
                        
                        if (master_is_translated) { /* BLASTX */
                            hsp->qseq[abs_index] = residue2;
                            hsp->hseq[abs_index] = residue1;
                        } else {                    /* TBLASTN */
                            hsp->qseq[abs_index] = residue1;
                            hsp->hseq[abs_index] = residue2;
                        }
                        
                        if (residue1 == residue2)
                            hsp->midline[abs_index] = residue1;
                        else if (matrix != NULL && matrix[residue1][residue2] >0)
                            hsp->midline[abs_index] = '+';
                        else
                            hsp->midline[abs_index] = ' ';
                        
                        abs_index++;
                    }
                }
                SeqPortFree(spp1);
                SeqPortFree(spp2);
                /* Check if this is an ungapped alignment;
                   in this case do not go to next link */
                if (ssp->next && 
                    ssp->next->loc->choice != SEQLOC_EMPTY && 
                    ssp->next->loc->next->choice != SEQLOC_EMPTY)
                    ungapped_align = TRUE;

            } else if (ssp->loc->choice == SEQLOC_EMPTY) { /* gap in query */

                if (target_is_translated) { /* TBLASTN */
                    spp1 = SeqPortNewByLoc(ssp->loc->next, Seq_code_ncbi4na);
                    
                    while(TRUE) {
                        if((codon[0] = SeqPortGetResidue(spp1)) == SEQPORT_EOF)
                            break;
                        if((codon[1] = SeqPortGetResidue(spp1)) == SEQPORT_EOF)
                            break;
                        if((codon[2] = SeqPortGetResidue(spp1)) == SEQPORT_EOF)
                            break;
                        
                        residue1 = AAForCodon(codon, master_is_translated ? genetic_code2 : genetic_code1);
                        
                        hsp->qseq[abs_index]  = '-'; /* gap character */
                        hsp->hseq[abs_index] = residue1;
                        hsp->midline[abs_index] = ' ';
                        abs_index++;
                    }
                    SeqPortFree(spp1);
                } else {        /* BLASTX */
                    spp1 = SeqPortNewByLoc(ssp->loc->next, Seq_code_ncbieaa);

                    while ((residue1=SeqPortGetResidue(spp1)) != SEQPORT_EOF) {
                        hsp->qseq[abs_index] = '-'; /* gap character */
                        hsp->hseq[abs_index] = residue1;
                        hsp->midline[abs_index] = ' ';
                        abs_index++;
                    }
                    
                    SeqPortFree(spp1);
                }
            } else if (ssp->loc->next->choice == SEQLOC_EMPTY) {
                if (master_is_translated) { /* BLASTX */
                    spp1 = SeqPortNewByLoc(ssp->loc, Seq_code_ncbi4na);

                    while(TRUE) {
                        if((codon[0] = SeqPortGetResidue(spp1)) == SEQPORT_EOF)
                            break;
                        if((codon[1] = SeqPortGetResidue(spp1)) == SEQPORT_EOF)
                            break;
                        if((codon[2] = SeqPortGetResidue(spp1)) == SEQPORT_EOF)
                            break;

                        residue1 = AAForCodon(codon, genetic_code1);
                        
                        hsp->qseq[abs_index] = residue1;
                        hsp->hseq[abs_index] = '-'; /* gap character */
                        hsp->midline[abs_index] = ' ';
                        abs_index++;
                    }
                    SeqPortFree(spp1);
                } else {                    /* TBLASTN */
                    
                    spp1 = SeqPortNewByLoc(ssp->loc, Seq_code_ncbieaa);
                    
                    while ((residue1=SeqPortGetResidue(spp1)) != SEQPORT_EOF) {
                        hsp->qseq[abs_index] = residue1;
                        hsp->hseq[abs_index] = '-'; /* gap character */
                        hsp->midline[abs_index] = ' ';
                        abs_index++;
                    }

                    SeqPortFree(spp1);
                }
            }
            
            if (both_translated || ungapped_align)	
                /* for tblastx perform only one StdSegPtr. so far...*/
                break;
        } else {
            continue;    /* Both EMPTY - never should happened */
        }
    }
    
    return TRUE;
}

Boolean BXMLGetSeqLines(SeqAlignPtr align, HspPtr hsp, Int4 length,
                        Boolean is_aa, Int4 chain, Int4Ptr PNTR matrix)
{
    DenseDiagPtr ddp;
    DenseSegPtr dsp;
    StdSegPtr ssp;
    Uint2 order = 0;
    SeqAlignPtr sap;
    ScorePtr    sp;

    if(align == NULL)
        return FALSE;
    
    switch (align->segtype) {
    case 1: /*Dense-diag*/
        ddp = (DenseDiagPtr) align->segs;

        while(ddp) {
            ++order;
            if(order == chain) {
                BXMLGetSeqLineForDenseDiag(ddp, hsp, length, is_aa, matrix);
                break;
            }
            ddp = ddp->next;
        }

        break;
    case 2:
        dsp = (DenseSegPtr) align->segs;
        BXMLGetSeqLineForDenseSeg(dsp, hsp, length, is_aa, matrix);
        break;
    case 3:
        ssp = (StdSegPtr) align->segs;
        while(ssp) {
            ++order;
            if(order == chain) {
                BXMLGetSeqLineForStdSeg(ssp, hsp, length, matrix);
                break;
            }
            ssp = ssp->next;
        }
        break;
    case 5: /* Discontinuous alignment */
        sap = (SeqAlignPtr) align->segs;
        return BXMLGetSeqLines(sap, hsp, length, is_aa, chain, matrix);
    default:
        break;
    }

    return TRUE;
}

static void 
MaskFilteredLocInHsp(HspPtr hsp, ValNodePtr mask, Boolean is_prot)
{
   Char mask_char;
   SeqLocPtr mask_loc;
   SeqLocPtr seqloc = NULL;
   Int4 start, stop, index;

   if (!mask)
      return;

   mask_loc = (SeqLocPtr) mask->data.ptrvalue;
   mask_char = (is_prot ? 'X' : 'N');

   while((seqloc = SeqLocFindNext(mask_loc, seqloc)) != NULL) {
      start = MAX(0, SeqLocStart(seqloc) - hsp->query_from);
      stop = MIN(SeqLocStop(seqloc), hsp->query_to) - hsp->query_from;
      for (index = start; index <= stop; ++index) {
         hsp->qseq[index] = mask_char;
         if (hsp->hseq[index] != mask_char)
            hsp->midline[index] = ' ';
      }
   }
}

HspPtr BXMLGetHspFromSeqAlign(SeqAlignPtr sap, Boolean is_aa, Int4 chain,
                              Boolean is_ooframe, ValNodePtr mask_loc)
{
    HspPtr hsp;
    AlignSum as;
    ScorePtr score, sp;
    Boolean matrix_allocated = FALSE;
    BioseqPtr bsp;
    Char tmp[256];
    static Boolean master_checked;
    Boolean prot_alphabet;

    if((hsp = HspNew()) == NULL)
        return NULL;

    MemSet(&as, NULLB, sizeof(AlignSum));
    
    as.master_sip = SeqIdDup(TxGetQueryIdFromSeqAlign(sap));
    as.target_sip = TxGetSubjectIdFromSeqAlign(sap);

    /* Checkup for query Bioseq to be available in the index */

    if(!master_checked && (bsp = BioseqLockById(as.master_sip)) == NULL) {
        SeqIdWrite(as.master_sip, tmp, PRINTID_FASTA_LONG, sizeof(tmp));
        ErrPostEx(SEV_ERROR, 0, __LINE__, "Query sequence '%s' is not "
                  "available in Bioseq index.", tmp);
        return NULL;
    } else if(!master_checked) {
        master_checked = TRUE;
        BioseqUnlock(bsp);
    }


    if(glb_matrix != NULL)
        as.matrix = glb_matrix;
    else {
        as.matrix = load_default_matrix ();
        matrix_allocated = TRUE;
    }
    as.is_aa = is_aa;
    as.ooframe = is_ooframe;

    if(chain == 0)
        chain = 1;
    
    if((score = find_score_in_align(sap, chain, &as)) == NULL) {
        if(matrix_allocated)
            free_default_matrix(as.matrix);
        HspFree(hsp);
        return NULL;
    }

    for(sp = score; sp != NULL; sp = sp->next) {
        if(!(StringICmp(sp->id->str, "e_value")) || 
           !(StringICmp(sp->id->str, "sum_e")))
            hsp->evalue = sp->value.realvalue;
        if(!StringICmp(sp->id->str, "bit_score")) {
            hsp->bit_score = sp->value.realvalue;
        }
        if(!StringICmp(sp->id->str, "score")) {
            hsp->score = sp->value.intvalue;
        }
    }

    if (as.m_frame_set || as.t_frame_set) {
       hsp->query_frame = as.m_frame;
       hsp->hit_frame = as.t_frame;
    } else { /* blastn program! */
       hsp->query_frame = 1;
       if (as.m_strand == as.t_strand)
          hsp->hit_frame = 1;
       else
          hsp->hit_frame = -1;
    }
    hsp->identity = as.identical;
    hsp->positive = as.positive + as.identical;
    hsp->gaps = as.gaps;
    hsp->align_len = as.totlen;
    
    hsp->density = 0;             /* ???? */
    

    /* For sequence retrieval from should always be less than to */
    hsp->query_from = as.master_from;
    hsp->query_to = as.master_to;
    hsp->hit_from = as.target_from;
    hsp->hit_to = as.target_to;

    BXMLGetSeqLines(sap, hsp, as.totlen, is_aa, chain, as.matrix);
    /* Sequences are shown in protein alphabet in all cases except blastn,
       that is if either subject is protein, or it is a translated search, 
       in which case alignment segments have StdSeg form. */
    prot_alphabet = (is_aa || sap->segtype == SAS_STD);
    MaskFilteredLocInHsp(hsp, mask_loc, prot_alphabet);

    /* For display it depends on strand */
    
    if(as.m_strand == Seq_strand_minus || as.m_frame < 0) {
        hsp->query_from = as.master_to;
        hsp->query_to = as.master_from;
    } 
    
    if(as.t_strand == Seq_strand_minus || as.t_frame < 0) {
        hsp->hit_from = as.target_to;
        hsp->hit_to = as.target_from;
    }

    /* ... and 1 position larger, that array number (started from 0) */

    hsp->query_from++;
    hsp->query_to++;
    hsp->hit_from++;
    hsp->hit_to++;
    
    if(matrix_allocated)
        free_default_matrix(as.matrix);

    SeqIdFree(as.master_sip);
    
    return hsp;
}

HitPtr BXMLSeqAlignToHits(SeqAlignPtr seqalign, Boolean ungapped, 
                          Boolean is_ooframe, ValNodePtr mask_loc)
{
    HitPtr hitp, hitp_head;
    HspPtr hspp;
    SeqAlignPtr sap, sap2;
    SeqIdPtr subject_sip, new_sip;
    BioseqPtr bsp;
    Char buffer[526];
    HspPtr hsp;
    Int4 hit_count, hsp_count, chain;
    Boolean is_aa;
    
    /* We assume, that  seqalignes in this set ordered by 
       database sequences and by e-values for a given database sequence */

    /* For optimization BLOSUM62 may be loaded ones */
    if(glb_matrix == NULL)
        glb_matrix = load_default_matrix ();
    
    hitp_head = NULL;
    hit_count = 1;              /* Hits starting with 1 */
    
    for(sap = seqalign; sap != NULL;) {
        subject_sip = TxGetSubjectIdFromSeqAlign(sap);
        bsp = BioseqLockById(subject_sip);
        if (!bsp) { /* Apparently a recently deleted sequence */
           sap = sap->next;
           continue;
        }
        
        if(hitp_head == NULL) { /* first element */
            hitp_head = hitp = HitNew();
        } else {
            hitp->next = HitNew();
            hitp = hitp->next;
        }
        hitp->num = hit_count;
        hit_count++;
        
        is_aa = (bsp->mol == Seq_mol_aa);
           
        if (BioseqGetTitle(bsp))
        	hitp->def = StringSave(BioseqGetTitle(bsp));
        else
        	hitp->def = StringSave("No definition line found");
          
        SeqIdWrite(bsp->id, buffer, PRINTID_FASTA_LONG, sizeof(buffer));
        hitp->id = StringSave(buffer);

        if(bsp->id->choice == SEQID_GI && bsp->id->next != NULL)
           SeqIdWrite(bsp->id->next, buffer, PRINTID_TEXTID_ACCESSION, sizeof(buffer));
        else
           SeqIdWrite(bsp->id, buffer, PRINTID_TEXTID_ACCESSION, sizeof(buffer));
           
        hitp->accession = StringSave(buffer);
           
        hitp->len = bsp->length;
        hsp_count = 1;          /* Hsps starting with 1 */
        chain = 1;

        for(sap2 = sap; sap2 != NULL;) {
            
            /* Filling info about specific alignments */

            while(TRUE) {
                if(hitp->hsps == NULL) {
                    hspp = hitp->hsps = 
                       BXMLGetHspFromSeqAlign(sap2, is_aa, chain, is_ooframe, 
                                              mask_loc);
                } else {
                    if((hspp->next = BXMLGetHspFromSeqAlign(sap2, is_aa, chain,
                                        is_ooframe, mask_loc)) == NULL)
                        break;
                    else
                        hspp = hspp->next;
                }
                
                if(hspp == NULL)
                    break;
                
                hspp->num = hsp_count;
                hsp_count++;
                if(!ungapped) {  /* Only one chain for gapped */
                    break;
                }
                chain++;
            }

            sap2 = sap2->next;
            new_sip = TxGetSubjectIdFromSeqAlign(sap2);
            
            if(SeqIdMatch(subject_sip, new_sip)) {
                continue;
            } else {
                sap = sap2;
                break;
            }
        }
    }

    return hitp_head;
}

IterationPtr BXMLBuildOneIteration(SeqAlignPtr seqalign, 
                                   ValNodePtr other_returns,
                                   Boolean is_ooframe, Boolean ungapped,
                                   Int4 iter_num, CharPtr message,
                                   ValNodePtr mask_loc)
{
   return BXMLBuildOneQueryIteration(seqalign, other_returns, is_ooframe,
                                     ungapped, iter_num, message, NULL, 
                                     mask_loc);
}

IterationPtr BXMLBuildOneQueryIteration(SeqAlignPtr seqalign, 
                                   ValNodePtr other_returns,
                                   Boolean is_ooframe, Boolean ungapped,
                                   Int4 iter_num, CharPtr message,
                                   BioseqPtr query, ValNodePtr mask_loc)
{
    IterationPtr iterp;
    TxDfDbInfoPtr dbinfo=NULL;
    BLAST_KarlinBlkPtr ka_params_gap=NULL;
    BLAST_KarlinBlkPtr ka_params_ungap=NULL;
    ValNodePtr vnp;
    Char buffer[1024];
    
    iterp = IterationNew();
    iterp->iter_num = iter_num;

    if (query) {
       SeqIdWrite(query->id, buffer, PRINTID_FASTA_LONG, sizeof(buffer));
       iterp->query_ID = StringSave(buffer);

       if((iterp->query_def = StringSave(BioseqGetTitle(query))) == NULL) {
          iterp->query_def = StringSave("No definition line found");
       }

       iterp->query_len = query->length;
    }

    if(seqalign != NULL) {
       iterp->hits = 
          BXMLSeqAlignToHits(seqalign, ungapped, is_ooframe, mask_loc);
    }

    iterp->stat = BXMLBuildStatistics(other_returns, ungapped);
    
    iterp->message = StringSave(message);
    
    return iterp;
}

StatisticsPtr
BXMLBuildStatistics(ValNodePtr other_returns, Boolean ungapped)
{
   TxDfDbInfoPtr dbinfo=NULL;
   BLAST_KarlinBlkPtr ka_params_gap=NULL;
   BLAST_KarlinBlkPtr ka_params_ungap=NULL;
   ValNodePtr vnp;
   StatisticsPtr stat;

   if (!other_returns) 
      return NULL;

   stat = StatisticsNew();

   for (dbinfo = NULL, vnp=other_returns; vnp; vnp = vnp->next) {
      switch (vnp->choice) {
      case TXDBINFO:
         dbinfo = vnp->data.ptrvalue;
         break;
      case TXKABLK_GAP:
         ka_params_gap = vnp->data.ptrvalue;
         break;
      case TXKABLK_NOGAP:
         ka_params_ungap = vnp->data.ptrvalue;
         break;
      case EFF_SEARCH_SPACE:
         stat->eff_space = vnp->data.realvalue;
         break;
      case EFF_HSP_LENGTH:
         stat->hsp_len = vnp->data.intvalue;
         break;
      default:
         break;
      }
   }
   
   if(dbinfo != NULL) {
      stat->db_num= dbinfo->number_seqs;
      stat->db_len = dbinfo->total_length;
   }
   
   if(ungapped) {
      if(ka_params_ungap != NULL) {
         stat->lambda = ka_params_ungap->Lambda;
         stat->kappa = ka_params_ungap->K;
         stat->entropy = ka_params_ungap->H;
      }
   } else {
      if(ka_params_gap != NULL) {
         stat->lambda = ka_params_gap->Lambda;
         stat->kappa = ka_params_gap->K;
         stat->entropy = ka_params_gap->H;
      }
   }

   return stat;
}

BlastOutputPtr BXMLCreateBlastOutputHead(CharPtr program, CharPtr database, 
                                         BLAST_OptionsBlkPtr options, 
                                         BioseqPtr query, Int4 flags)
{
    BlastOutputPtr boutp;
    Char buffer[1024];
    SeqPortPtr spp;
    Boolean is_aa = FALSE;
    Int4 i;
    
    if((boutp = BlastOutputNew()) == NULL)
        return FALSE;
    
    /* For optimization BLOSUM62 may be loaded ones */
    if(glb_matrix == NULL)
        glb_matrix = load_default_matrix ();
    
    if (query) {
       SeqIdWrite(query->id, buffer, PRINTID_FASTA_LONG, sizeof(buffer));
       boutp->query_ID = StringSave(buffer);

       if((boutp->query_def = StringSave(BioseqGetTitle(query))) == NULL) {
          boutp->query_def = StringSave("No definition line found");
       }

       boutp->query_len = query->length;

       if(flags & BXML_INCLUDE_QUERY) {
          boutp->query_seq = MemNew(query->length+1);
          is_aa = (query->mol == Seq_mol_aa);
          spp = SeqPortNew(query, 0, -1, Seq_strand_plus, 
                           (is_aa) ? Seq_code_ncbieaa : Seq_code_iupacna);
          
          for (i = 0; i < query->length; i++) {
             boutp->query_seq[i] = SeqPortGetResidue(spp);
          }
          spp = SeqPortFree(spp);
       } else {
          boutp->query_seq = NULL;    /* Do we need sequence here??? */
       }
    }
    /* Program name */
    boutp->program = StringSave(program);

    /* Database name */
    boutp->db = StringSave(database);

    /* Version text */
    sprintf(buffer, "%s %s [%s]", program, BlastGetVersionNumber(), 
            BlastGetReleaseDate());
    boutp->version = StringSave(buffer);

    /* Reference */
    boutp->reference = BlastGetReference(FALSE);

    /* Filling parameters */
    
    boutp->param = ParametersNew();    
    boutp->param->expect = options->expect_value;
    boutp->param->matrix = StringSave(options->matrix);
    boutp->param->sc_match = options->reward;
    boutp->param->sc_mismatch = options->penalty;
    boutp->param->gap_open = options->gap_open;
    boutp->param->gap_extend = options->gap_extend;
    boutp->param->include = options->ethresh;
    
    if(options->filter_string != NULL)
        boutp->param->filter = StringSave(options->filter_string);
    
    return boutp;
}

/* 
   This function will create and print out simplified ASN.1/XML of
   one-iterational Blast output - like regular Blast etc. This function
   should not be used for multi-iterational PSI-Blast.
*/
Boolean BXMLPrintOutput(AsnIoPtr aip, SeqAlignPtr seqalign, 
                        BLAST_OptionsBlkPtr options,CharPtr program,
                        CharPtr database, BioseqPtr query, 
                        ValNodePtr other_returns, Int4 flags, 
                        CharPtr message, ValNodePtr mask_loc)
{
    BlastOutputPtr boutp;
    Boolean ungapped = FALSE;

    if((boutp = BXMLCreateBlastOutputHead(program, database, options, query,
                                          flags)) == NULL)
        return FALSE;
    
    if(options->gapped_calculation == FALSE || !StringICmp(program, "tblastx"))
        ungapped = TRUE;
    
    /* Here is one-iterational Blast output */
    boutp->iterations = BXMLBuildOneIteration(seqalign, other_returns,
                                              options->is_ooframe, ungapped,
                                              1, message, mask_loc);
    
    if (aip != NULL)      
        BlastOutputAsnWrite(boutp, aip, NULL);
    
    if (glb_matrix)
    	free_default_matrix(glb_matrix);
    glb_matrix = NULL;
    
    BlastOutputFree(boutp);
    
    ObjMgrFreeCache(0);
    
    return TRUE;
}

Boolean BXMLPrintMultiQueryOutput(AsnIoPtr aip, SeqAlignPtr seqalign, 
           BLAST_OptionsBlkPtr options, CharPtr program, CharPtr database, 
           BioseqSetPtr query_set, ValNodePtr other_returns, Int4 flags,
           CharPtr message, ValNodePtr mask_loc)
{
    Boolean ungapped = FALSE;
    BioseqPtr query;
    SeqEntryPtr sep;
    Boolean q_is_na, d_is_na;
    MBXmlPtr mbxp = NULL;
    IterationPtr iterp;
    SeqEntryFunc seqentry_callback;
    Int4 index;
    Boolean query_found;
    SeqIdPtr seqid = NULL;
    SeqAlignPtr sap = NULL, next_seqalign = NULL;
    ValNodePtr next_mask_loc = NULL, current_mask_loc = NULL;

    sep = (SeqEntryPtr) ((BioseqSetPtr)query_set)->seq_set;
    BlastGetTypes(program, &q_is_na, &d_is_na);
    if (q_is_na)
       seqentry_callback = FindNuc;
    else 
       seqentry_callback = FindProt;

    /* If no queries, there is nothing to report */
    if (!sep)
       return FALSE;

    /* If only one query, call a one-query output function */
    if (!sep->next) {
       Boolean return_value;
       SeqEntryExplore(sep, &query, seqentry_callback);
       return_value = BXMLPrintOutput(aip, seqalign, options, program, 
                         database, query, other_returns, flags, 
                         message, mask_loc);
       /* This function is presumed to close the AsnIoPtr inside */
       AsnIoClose(aip);
       return return_value;
    }

    next_mask_loc = mask_loc;

    index = 0;
    while (seqalign) {
       /* Find the corresponding query */
       query_found = FALSE;
       for ( ; sep; ++index, sep = sep->next) {
          SeqEntryExplore(sep, &query, seqentry_callback);
          seqid = TxGetQueryIdFromSeqAlign(seqalign);
          if (SeqIdComp(query->id, seqid) == SIC_YES) {
             query_found = TRUE;
             break;
          }
       }
       if (query_found) {
          /* Find where seqaligns for this query end */
          for (sap = seqalign; sap && sap->next; sap = sap->next) {
             if (SeqIdComp(seqid, TxGetQueryIdFromSeqAlign(sap->next)) 
                 != SIC_YES) {
                break;
             }
          }
          next_seqalign = sap->next;
          /* Unlink this query seqaligns from the rest */
          sap->next = NULL;
       } else {
          break;
       }

       /* Find the masking locations for this query */
       if (next_mask_loc && 
           SeqIdComp(SeqLocId((SeqLocPtr)next_mask_loc->data.ptrvalue), 
                     seqid) == SIC_YES) {
          current_mask_loc = (SeqLocPtr) 
             MemDup(next_mask_loc, sizeof(SeqLoc));
          next_mask_loc = next_mask_loc->next;
          current_mask_loc->next = NULL;
       } else {
          current_mask_loc = NULL;
       }

       if (!mbxp) {
          if (options->is_megablast_search) {
             mbxp = PSIXmlInit(aip, "megablast", database, options, query, 0);
          } else {
             mbxp = PSIXmlInit(aip, program, database, options, query, 0);
          }
       }
       if(options->gapped_calculation == FALSE || 
          !StringICmp(program, "tblastx"))
          ungapped = TRUE;
    
       /* Here is one-iterational Blast output */
       iterp = BXMLBuildOneQueryIteration(seqalign, NULL, options->is_ooframe,
                                          ungapped, index, NULL, query, 
                                          current_mask_loc);
       current_mask_loc = (ValNodePtr) MemFree(current_mask_loc);
    
       IterationAsnWrite(iterp, mbxp->aip, mbxp->atp);
       AsnIoFlush(mbxp->aip);
       IterationFree(iterp);
       /* Reconnect the SeqAlign chain */
       if (sap)
          sap->next = next_seqalign;
       seqalign = next_seqalign;
    }
    
    free_default_matrix(glb_matrix);
    glb_matrix = NULL;
    
    MBXmlClose(mbxp, other_returns, !options->gapped_calculation);

    ObjMgrFreeCache(0);
    
    return TRUE;
}


PSIXmlPtr PSIXmlInit(AsnIoPtr aip, CharPtr program, CharPtr database, 
                     BLAST_OptionsBlkPtr options, BioseqPtr query, Int4 flags)
{
    PSIXmlPtr psixp;
    AsnModulePtr amp;
    DataVal av;
    AsnTypePtr atp;
    Boolean retval = FALSE;

    AsnTypePtr       BLASTOUTPUT;
    AsnTypePtr       BLASTOUTPUT_program;
    AsnTypePtr       BLASTOUTPUT_version;
    AsnTypePtr       BLASTOUTPUT_reference;
    AsnTypePtr       BLASTOUTPUT_db;
    AsnTypePtr       BLASTOUTPUT_query_ID;
    AsnTypePtr       BLASTOUTPUT_query_def;
    AsnTypePtr       BLASTOUTPUT_query_len;
    AsnTypePtr       BLASTOUTPUT_query_seq;
    AsnTypePtr       BLASTOUTPUT_param;
    AsnTypePtr       BLASTOUTPUT_iterations;
    AsnTypePtr       BLASTOUTPUT_iterations_E;
    AsnTypePtr       BLASTOUTPUT_mbstat;

    psixp = (PSIXmlPtr) MemNew(sizeof(PSIXml));
    
    psixp->aip = aip;
    
    if (! bxmlobjAsnLoad()) {
        return NULL;
    }
    
    amp = AsnAllModPtr();

    MACRO_atp_find(BLASTOUTPUT,BlastOutput);
    MACRO_atp_find(BLASTOUTPUT_program,BlastOutput.program);
    MACRO_atp_find(BLASTOUTPUT_version,BlastOutput.version);
    MACRO_atp_find(BLASTOUTPUT_reference,BlastOutput.reference);
    MACRO_atp_find(BLASTOUTPUT_db,BlastOutput.db);
    MACRO_atp_find(BLASTOUTPUT_query_ID,BlastOutput.query-ID);
    MACRO_atp_find(BLASTOUTPUT_query_def,BlastOutput.query-def);
    MACRO_atp_find(BLASTOUTPUT_query_len,BlastOutput.query-len);
    MACRO_atp_find(BLASTOUTPUT_query_seq,BlastOutput.query-seq);
    MACRO_atp_find(BLASTOUTPUT_param,BlastOutput.param);
    MACRO_atp_find(BLASTOUTPUT_iterations,BlastOutput.iterations);
    MACRO_atp_find(BLASTOUTPUT_iterations_E,BlastOutput.iterations.E);
    MACRO_atp_find(BLASTOUTPUT_mbstat,BlastOutput.mbstat);

    /* Start of iterations structure */
    psixp->atp = BLASTOUTPUT_iterations_E;

    /* Head of all BlastOutput structure */
    psixp->BlastOutput = BLASTOUTPUT;
    
    /* Head of iterations strucure */
    psixp->BlastOutput_iterations = BLASTOUTPUT_iterations;

    /* Head of the final statistics for Mega BLAST */
    psixp->BlastOutput_mbstat = BLASTOUTPUT_mbstat;
    
    psixp->boutp = BXMLCreateBlastOutputHead(program, database, options, 
                                             query, flags);
    
    atp = AsnLinkType(NULL, BLASTOUTPUT);   /* link local tree */
    
    if (atp == NULL) {
        return NULL;
    }
    
    if (! AsnOpenStruct(psixp->aip, atp, (Pointer) psixp->boutp)) {
        return NULL;
    }

    if (psixp->boutp->program != NULL) {
        av.ptrvalue = psixp->boutp -> program;
        retval = AsnWrite(psixp->aip, BLASTOUTPUT_program,  &av);
    }
    
    if (psixp->boutp->version != NULL) {
        av.ptrvalue = psixp->boutp->version;
        retval = AsnWrite(psixp->aip, BLASTOUTPUT_version,  &av);
    }
    
    if (psixp->boutp->reference != NULL) {
        av.ptrvalue = psixp->boutp->reference;
        retval = AsnWrite(psixp->aip, BLASTOUTPUT_reference,  &av);
    }

    if (psixp->boutp -> db != NULL) {
        av.ptrvalue = psixp->boutp->db;
        retval = AsnWrite(psixp->aip, BLASTOUTPUT_db,  &av);
    }

    if (psixp->boutp -> query_ID != NULL) {
        av.ptrvalue = psixp->boutp->query_ID;
        retval = AsnWrite(psixp->aip, BLASTOUTPUT_query_ID,  &av);
    }

    if (psixp->boutp->query_def != NULL) {
        av.ptrvalue = psixp->boutp->query_def;
        retval = AsnWrite(psixp->aip, BLASTOUTPUT_query_def,  &av);
    }

    av.intvalue = psixp->boutp->query_len;
    retval = AsnWrite(psixp->aip, BLASTOUTPUT_query_len,  &av);

    if (psixp->boutp->query_seq != NULL) {
        av.ptrvalue = psixp->boutp->query_seq;
        retval = AsnWrite(psixp->aip, BLASTOUTPUT_query_seq,  &av);
    }

    if (psixp->boutp->param != NULL) {
        if (!ParametersAsnWrite(psixp->boutp->param, 
                                psixp->aip, BLASTOUTPUT_param)) {
            return NULL;
        }
    }

    if(!AsnOpenStruct(psixp->aip, BLASTOUTPUT_iterations, NULL))
        return NULL;
    
    AsnIoFlush(psixp->aip);
    
    return psixp;
}

void PSIXmlReset(PSIXmlPtr psixp)
{
    AsnCloseStruct(psixp->aip, psixp->BlastOutput_iterations, NULL);
    AsnCloseStruct(psixp->aip, psixp->BlastOutput, NULL);
    psixp->BlastOutput_iterations = NULL;
    psixp->BlastOutput = NULL;
    AsnIoReset(psixp->aip);
    
    psixp->boutp = BlastOutputFree(psixp->boutp);
}

void PSIXmlClose(PSIXmlPtr psixp)
{
   if (psixp->BlastOutput_iterations)
      AsnCloseStruct(psixp->aip, psixp->BlastOutput_iterations, NULL);
   if (psixp->BlastOutput)
      AsnCloseStruct(psixp->aip, psixp->BlastOutput, NULL);
    
    AsnIoClose(psixp->aip);
    
    BlastOutputFree(psixp->boutp);
    MemFree(psixp);
    
    return;
}

void MBXmlClose(PSIXmlPtr mbxp, ValNodePtr other_returns, Boolean ungapped)
{
   StatisticsPtr stat;
   AsnTypePtr atp;

    AsnCloseStruct(mbxp->aip, mbxp->BlastOutput_iterations, NULL);

    if (other_returns) {
       atp = AsnLinkType(NULL, mbxp->BlastOutput_mbstat);

       /*AsnOpenStruct(mbxp->aip, mbxp->BlastOutput_mbstat, NULL);*/
       stat = BXMLBuildStatistics(other_returns, ungapped);

       StatisticsAsnWrite(stat, mbxp->aip, atp);
       AsnIoFlush(mbxp->aip);
       StatisticsFree(stat);                 
       /*AsnCloseStruct(mbxp->aip, mbxp->BlastOutput_mbstat, NULL);*/
    }

    AsnCloseStruct(mbxp->aip, mbxp->BlastOutput, NULL);
    
    AsnIoClose(mbxp->aip);
    
    BlastOutputFree(mbxp->boutp);
    MemFree(mbxp);
    
    return;
}

static void FillHspScoreInfo(HspPtr hsp, ScorePtr score)
{
   ScorePtr sp;
   for(sp = score; sp != NULL; sp = sp->next) {
      if(!(StringICmp(sp->id->str, "e_value")) || 
         !(StringICmp(sp->id->str, "sum_e")))
         hsp->evalue = sp->value.realvalue;
      if(!StringICmp(sp->id->str, "bit_score")) {
         hsp->bit_score = sp->value.realvalue;
      }
      if(!StringICmp(sp->id->str, "score")) {
         hsp->score = sp->value.intvalue;
      }
      if(!StringICmp(sp->id->str, "num_ident")) {
         hsp->identity = sp->value.intvalue;
      }
      if (!StringICmp(sp->id->str, "sum_n")) {
         hsp->num = sp->value.intvalue;
      }
   }
}

static HspPtr GetHspFromSeqAlign(SeqAlignPtr align, Boolean ungapped, 
                          Int4Ptr hspcnt_ptr)
{
    HspPtr hsp, head_hsp = NULL, last_hsp = NULL;
    ScorePtr score, sp;
    DenseDiagPtr ddp;
    DenseSegPtr dsp;
    StdSegPtr ssp;
    Int4 hspcnt = 0, last_seg, i;

    *hspcnt_ptr = 0;

    switch (align->segtype) {
    case 1: /*Dense-diag; blastn, blastp ungapped */
       ddp = (DenseDiagPtr) align->segs;
       while(ddp) {
          if((hsp = HspNew()) == NULL)
             return head_hsp;
          if (!head_hsp)
             head_hsp = last_hsp = hsp;
          else {
             last_hsp->next = hsp;
             last_hsp = last_hsp->next;
          }

          ++hspcnt;
          hsp->align_len = ddp->len;
          if (ddp->strands[0] == Seq_strand_minus) {
             hsp->query_from = ddp->starts[0] + ddp->len;
             hsp->query_to = ddp->starts[0] + 1;
          } else {
             hsp->query_from = ddp->starts[0] + 1;
             hsp->query_to = ddp->starts[0] + ddp->len;
          }
          if (ddp->strands[1] == Seq_strand_minus) {
             hsp->hit_from = ddp->starts[1] + ddp->len;
             hsp->hit_to = ddp->starts[1] + 1;
          } else {
             hsp->hit_from = ddp->starts[1];
             hsp->hit_to = ddp->starts[1] + ddp->len - 1;
          }
          FillHspScoreInfo(hsp, ddp->scores);
          ddp = ddp->next;
       }
       break;
    case 2: /* Dense-seg; blastn, blastp gapped */
       dsp = (DenseSegPtr) align->segs;
       if((head_hsp = hsp = HspNew()) == NULL)
          return head_hsp;
       if (dsp->scores)
          score = dsp->scores;
       else
          score = align->score;
       hspcnt = 1;
       last_seg = dsp->numseg - 1;
       
       if (dsp->strands[0] == Seq_strand_minus) {
          hsp->query_from = dsp->starts[0] + dsp->lens[0];
          hsp->query_to = dsp->starts[2*last_seg] + 1;
       } else {
          hsp->query_from = dsp->starts[0] + 1;
          hsp->query_to = dsp->starts[2*last_seg] + dsp->lens[last_seg];
       }
       if (dsp->strands[1] == Seq_strand_minus) {
          hsp->hit_from = dsp->starts[1] + dsp->lens[1];
          hsp->hit_to = dsp->starts[2*last_seg+1] + 1;
       } else {
          hsp->hit_from = dsp->starts[1] + 1;
          hsp->hit_to = dsp->starts[2*last_seg+1] + dsp->lens[last_seg];
       }
       
       for (i=0; i<dsp->numseg; i++) {
          hsp->align_len += dsp->lens[i];
          if (dsp->starts[2*i] == -1 || dsp->starts[2*i+1] == -1) {
             hsp->gaps++;
          }
       }
       FillHspScoreInfo(hsp, score);
       break;
    case 3: /* Std-seg; translated gapped or ungapped */
       ssp = (StdSegPtr) align->segs;

       if (ungapped) {
          while(ssp) {
             if((hsp = HspNew()) == NULL)
                return head_hsp;
             if (!head_hsp)
                head_hsp = last_hsp = hsp;
             else {
                last_hsp->next = hsp;
                last_hsp = last_hsp->next;
             }
             
             ++hspcnt;
             if (ssp->scores)
                score = ssp->scores;
             else 
                score = align->score;
             
             FillHspScoreInfo(hsp, score);

             if (SeqLocStrand(ssp->loc) == Seq_strand_minus) {
                hsp->query_to = SeqLocStart(ssp->loc) + 1;
                hsp->query_from = SeqLocStop(ssp->loc);
             } else {
                hsp->query_from = SeqLocStart(ssp->loc) + 1;
                hsp->query_to = SeqLocStop(ssp->loc);
             }
             if (SeqLocStrand(ssp->loc->next) == Seq_strand_minus) {
                hsp->hit_to = SeqLocStart(ssp->loc->next) + 1;
                hsp->hit_from = SeqLocStop(ssp->loc->next);
             } else {
                hsp->hit_from = SeqLocStart(ssp->loc->next) + 1;
                hsp->hit_to = SeqLocStop(ssp->loc->next);
             }
             if (SeqLocStrand(ssp->loc) == Seq_strand_unknown) {
                /* Protein location */
                hsp->align_len = SeqLocLen(ssp->loc);
             } else { /* Nucleotide location; need to divide length by 3 
                        to get protein length */
                hsp->align_len = SeqLocLen(ssp->loc) / 3;
             }
             ssp = ssp->next;
          }
       } else {
          SeqLocPtr slp;
          StdSegPtr last_ssp;

          if((head_hsp = hsp = HspNew()) == NULL)
             return head_hsp;

          hspcnt = 1;
          FillHspScoreInfo(hsp, align->score);
          /* Get relevant endpoints from the first StdSeg in the list */
          if (SeqLocStrand(ssp->loc) == Seq_strand_minus) {
             hsp->query_from = SeqLocStop(ssp->loc);
          } else {
             hsp->query_from = SeqLocStart(ssp->loc) + 1;
          }
          if (SeqLocStrand(ssp->loc->next) == Seq_strand_minus) {
             hsp->hit_from = SeqLocStop(ssp->loc->next);
          } else {
             hsp->hit_from = SeqLocStart(ssp->loc->next) + 1;
          }
          /* Advance StdSeg to the end of the list, and calculate the 
             alignment length in the process */
          for ( ; ssp; ssp = ssp->next) {
             slp = ((ssp->loc->choice == SEQLOC_EMPTY) ? ssp->loc->next : 
                    ssp->loc);
             if (SeqLocStrand(slp) == Seq_strand_unknown) {
                hsp->align_len += SeqLocLen(slp);
             } else {
                hsp->align_len += (SeqLocLen(slp) / 3);
             }
             last_ssp = ssp;
          }
          /* Get relevant endpoints from the last StdSeg in the list */
          if (SeqLocStrand(last_ssp->loc) == Seq_strand_minus) {
             hsp->query_to = SeqLocStart(last_ssp->loc) + 1;
          } else {
             hsp->query_to = SeqLocStop(last_ssp->loc);
          }
          if (SeqLocStrand(last_ssp->loc->next) == Seq_strand_minus) {
             hsp->hit_to = SeqLocStart(last_ssp->loc->next) + 1;
          } else {
             hsp->hit_to = SeqLocStop(last_ssp->loc->next);
          }
       }
       break;
    default: break;
    }    
    
    *hspcnt_ptr = hspcnt;
    return head_hsp;
}

HitPtr SeqAlignToHits(SeqAlignPtr seqalign, Boolean ungapped)
{
   HitPtr hitp, hitp_head;
   SeqAlignPtr sap, sap2;
   SeqIdPtr subject_id, sip;
   Char buffer[526];
   HspPtr hspp;
   Int4 hsp_count, chain;
   
   hitp_head = NULL;
   
   for(sap = seqalign; sap != NULL;) {
      subject_id = TxGetSubjectIdFromSeqAlign(sap);
      
      if(hitp_head == NULL) { /* first element */
         hitp_head = hitp = HitNew();
      } else {
         hitp->next = HitNew();
         hitp = hitp->next;
      }
      
      SeqIdWrite(subject_id, buffer, PRINTID_FASTA_LONG, sizeof(buffer));
      hitp->id = StringSave(buffer);
      SeqIdWrite(SeqIdFindBestAccession(subject_id), buffer, 
                 PRINTID_TEXTID_ACCESSION, sizeof(buffer));
      
      hitp->accession = StringSave(buffer);
      
      for(sap2 = sap; sap2 != NULL; ) {
         /* Filling info about specific alignments */
         if (!hitp->hsps) {
            hspp = hitp->hsps = 
               GetHspFromSeqAlign(sap2, ungapped, &hsp_count);
         } else {
            hspp->next = GetHspFromSeqAlign(sap2, ungapped, &hsp_count);
            hspp = hspp->next;
         }

         hitp->num += hsp_count;

         sap2 = sap2->next;
         sip = TxGetSubjectIdFromSeqAlign(sap2);
         
         if(SeqIdMatch(subject_id, sip)) {
            continue;
         } else {
            sap = sap2;
            break;
         }
      }
   }
   
   return hitp_head;
}
