/* ===========================================================================
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
* ===========================================================================*/
/*****************************************************************************

File name: blfmtutl.h

Author: Tom Madden

Contents: prototypes and defines for Blast formatting utilities

******************************************************************************/

/* $Revision: 1.3 $ 
* $Log: blfmtutl.h,v $
* Revision 1.3  2004/10/04 17:54:14  madden
* BlastPrintVersionInfo[Ex] now takes const char* as arg for program
*
* Revision 1.2  2004/06/30 13:38:18  madden
* Add prototypes for add_string_to_buffer and add_string_to_bufferEx
*
* Revision 1.1  2004/06/30 12:31:30  madden
* Structures and prototypes for blast formatting utilities
*
 *
 * */
#ifndef __BLFMTUTL__
#define __BLFMTUTL__

#include <ncbi.h>
#include <objcode.h>
#include <objseq.h>
#include <sequtil.h>
#include <readdb.h>
#include <ncbithr.h>
#include <txalign.h>
#include <ffprint.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef struct _txdbinfo {
   struct _txdbinfo PNTR next;
   Boolean   is_protein;
   CharPtr   name;
   CharPtr   definition;
   CharPtr   date;
   Int8   total_length;
   Int4   number_seqs;
   Boolean subset;	/* Print the subset message. */
} TxDfDbInfo, *TxDfDbInfoPtr;

/*****************************************************************
*
*       Used for pruing SeqALigns that are too big.
*
********************************************************************/

typedef struct _blast_prune_hits_from_sap {
                SeqAlignPtr sap;
                Int4    original_number,        /* how may unique hits were there originally. */
                        number;         /* How many hits on SeqALignPtr above. */
                Boolean allocated; /* If FALSE, SeqAlignPtr above does NOT belong to this struc.*/
        } BlastPruneSapStruct, PNTR BlastPruneSapStructPtr;
	

/*
	Allocates memory for TxDfDbInfoPtr.
	Link up new (returned) value to 'old', if non-NULL.
*/
TxDfDbInfoPtr LIBCALL TxDfDbInfoNew PROTO((TxDfDbInfoPtr old));

/*
	Deallocates memory (including strings for name, definition, and date).
*/
TxDfDbInfoPtr LIBCALL TxDfDbInfoDestruct PROTO((TxDfDbInfoPtr dbinfo));


/*
        Duplicate a SeqAlignPtr, keeping on the number of unique db
        hits specified.
*/

BlastPruneSapStructPtr LIBCALL BlastPruneHitsFromSeqAlign PROTO((SeqAlignPtr sap, Int4 number, BlastPruneSapStructPtr prune));

/* Deallocates BlastPruneSapStruct and deallocates SeqAlignSet if "allocated" flag is set. */
BlastPruneSapStructPtr LIBCALL BlastPruneSapStructDestruct PROTO((BlastPruneSapStructPtr prune));

/*
Print a summary of the query.
*/
Boolean LIBCALL AcknowledgeBlastQuery PROTO((BioseqPtr bsp, Int4 line_length, FILE *outfp, Boolean believe_query, Boolean html));

/*
	Print a report of the database used.
*/
Boolean LIBCALL PrintDbReport PROTO((TxDfDbInfoPtr dbinfo, Int4 line_length, FILE *outfp));

/*
	print out some of the Karlin-Altschul parameters.
*/
Boolean LIBCALL PrintKAParameters PROTO((Nlm_FloatHi Lambda, Nlm_FloatHi K, Nlm_FloatHi H, Int4 line_length, FILE *outfp, Boolean gapped));
Boolean LIBCALL PrintKAParametersExtra PROTO((Nlm_FloatHi Lambda, Nlm_FloatHi K, Nlm_FloatHi H, Nlm_FloatHi C, Int4 line_length, FILE *outfp, Boolean gapped));

/*
	Print a CharPtr (VisibleString), printing a new line every time
	a tilde is encountered.
*/
Boolean LIBCALL PrintTildeSepLines PROTO((CharPtr buffer, Int4 line_length, FILE *outfp));


/*
        Prints some header information.
*/

CharPtr LIBCALL BlastGetVersionNumber PROTO((void));

CharPtr LIBCALL BlastGetReference PROTO((Boolean html));

Boolean LIBCALL BlastPrintReference PROTO((Boolean html, Int4 line_length, FILE *outfp));
Boolean LIBCALL MegaBlastPrintReference PROTO((Boolean html, Int4 line_length, FILE *outfp));

CharPtr LIBCALL BlastGetPhiReference PROTO((Boolean html));

Boolean LIBCALL BlastPrintPhiReference PROTO((Boolean html, Int4 line_length, FILE *outfp));

Boolean BlastPrintVersionInfo PROTO((const char* program, Boolean html, FILE *outfp));
Boolean BlastPrintVersionInfoEx PROTO((const char* program, Boolean html, CharPtr version, CharPtr date, FILE *outfp));

CharPtr LIBCALL BlastGetReleaseDate PROTO((void));

/*
        adds the new string to the buffer, separating by a tilde.
        Checks the size of the buffer for FormatBlastParameters and
        allocates longer replacement if needed.
*/

Boolean LIBCALL add_string_to_bufferEx PROTO((CharPtr buffer, CharPtr *old, Int2Ptr old_length, Boolean add_tilde));

Boolean LIBCALL add_string_to_buffer PROTO((CharPtr buffer, CharPtr *old, Int2Ptr old_length));


#ifdef __cplusplus
}
#endif
#endif /* !__BLFMTUTL__ */

