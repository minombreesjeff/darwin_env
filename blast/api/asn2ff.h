/*   asn2ff.h
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
* File Name:  asn2ff.h
*
* Author:  Karl Sirotkin, Tom Madden, Tatiana Tatusov
*
* Version Creation Date:   7/15/95
*
* $Revision: 6.14 $
*
* File Description:  Header file for asn2gb files.
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: asn2ff.h,v $
* Revision 6.14  2001/04/05 21:52:18  tatiana
* format and strand added to SeqEntryToEntrez
*
* Revision 6.13  2001/03/15 21:42:00  tatiana
* SeqEntryToEntrez added
*
* Revision 6.12  2000/03/24 17:56:11  tatiana
* SeqEntryToStrArrayQEx added
*
* Revision 6.10  1999/04/09 21:22:10  bazhin
* Added function "ByteStorePtr AjpToByteStore()".
*
* Revision 6.9  1999/04/02 16:31:46  tatiana
* asn2ff_print_to_mem and SeqEntryToStrArray changed to NLM_EXTERN
*
* Revision 6.8  1999/03/05 16:41:40  tatiana
* GRAPHIK_FMT changed to 14
*
* Revision 6.7  1999/03/04 23:39:53  tatiana
* GRAPHIK_FMT added
*
* Revision 6.6  1998/11/05 16:12:17  bazhin
* Added 4th parameter "Boolean show_gi" to SeqEntryToGBFlatNoSeq()
* function. It will say do or does not print GI number into output
* flatfile. Same for error messages related to missing GIs.
*
* Revision 6.5  1998/09/14 16:37:46  tatiana
* SeqEntryToFlatAjp added
*
* Revision 6.4  1998/09/08 20:40:52  tatiana
* Asn2ffJobCreate added
*
* Revision 6.3  1998/08/21 16:58:48  shavirin
* Added definition of function SeqEntryToGBFlatNoSeq()
*
* Revision 6.2  1998/05/11 21:58:50  tatiana
* some functions moved from asn2ff1.c to asn2ff6.c
*
* Revision 6.1  1998/05/08 21:58:36  tatiana
* added PARTIAL_MODE and SeqEntryToPartRpt PROTO
*
* Revision 6.0  1997/08/25 18:04:33  madden
* Revision changed to 6.0
*
* Revision 5.7  1997/07/28 19:03:50  vakatov
* [WIN32,MSVC++]  Restored lost "NCBIOBJ.LIB" pro-DLL modifications
*
* Revision 5.6  1997/07/23 18:36:58  tatiana
* SeqEntryToFlatEx() added
*
* Revision 5.5  1997/06/19 18:36:48  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
 * Revision 5.3  1996/09/09  13:36:02  kans
 * moved BioseqGetGBDivCode from toasn.[ch] to asn2ff.h/asn2ff6.c
 *
 * Revision 5.1  1996/07/12  19:46:09  tatiana
 * SeqSubmitToFlat PROTO added
 *
 * Revision 4.3  1996/03/19  15:58:14  tatiana
 * SeqGenomeToFlat prototype
 *
 * Revision 1.24  1995/07/17  19:33:20  kans
 * parameters combined into Asn2ffJobPtr structure
 *
 * Revision 1.23  1995/06/19  21:40:02  kans
 * Tatiana's first major reorganization, moving printing, adding HTML
 *
* ==========================================================================
*/

#ifndef _ASN2FF_
#define _ASN2FF_

#include <ncbi.h>
#include <objsset.h>
#include <prtutil.h>
#include <seqport.h>
#include <sequtil.h>
#include <stdio.h>
#include <asn2ffg.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define GENBANK_FMT ( (Uint1)0)
#define EMBL_FMT ( (Uint1)1)
#define GENPEPT_FMT ( (Uint1)2)
#define PSEUDOEMBL_FMT ( (Uint1)3)
#define SELECT_FMT ( (Uint1)4)
#define EMBLPEPT_FMT ( (Uint1)5)
#define RELEASE_MODE ( (Uint1)6)
#define DUMP_MODE ( (Uint1)7)
#define SEQUIN_MODE ( (Uint1)8)
#define CHROMO_MODE ( (Uint1)9)
#define DIRSUB_MODE ( (Uint1)10)
#define REVISE_MODE ( (Uint1)11)
#define DIRSUB_DEBUG_MODE ( (Uint1)12)
#define PARTIAL_MODE ( (Uint1)13)
#define GRAPHIK_FMT ( (Uint1)14)

#define NUM_OF_ESTIMATES 20
#define NUM_SEQ_LINES 10


NLM_EXTERN Int4 asn2ff_setup PROTO ((Asn2ffJobPtr job, FFPrintArrayPtr PNTR papp));

NLM_EXTERN void asn2ff_cleanup PROTO ((Asn2ffJobPtr job));

NLM_EXTERN Boolean asn2ff_print PROTO ((Asn2ffJobPtr job));

NLM_EXTERN LinkStrPtr asn2ff_print_to_mem PROTO ((Asn2ffJobPtr job, LinkStrPtr lsp));

NLM_EXTERN Asn2ffJobPtr Asn2ffJobCreate PROTO ((SeqEntryPtr sep, SeqSubmitPtr ssp, SeqLocPtr slp, FILE *fp, Uint1 format, Uint1 mode, StdPrintOptionsPtr	Spop));

NLM_EXTERN Boolean SeqEntryToEntrez PROTO ((SeqEntryPtr sep, FILE *fp, SeqIdPtr seqid, Uint1 format, Uint1 display, Int4 from, Int4 to, Uint1 strand));

NLM_EXTERN CharPtr PNTR SeqEntryToStrArray PROTO ((SeqEntryPtr sep, Uint1 format, Uint1 mode));

NLM_EXTERN LinkStrPtr SeqEntryToStrArrayQEx PROTO ((SeqEntryPtr sep, Uint1 format, Int4 gi, Boolean is_html));
NLM_EXTERN LinkStrPtr SeqEntryToStrArrayEx PROTO ((SeqEntryPtr sep, Uint1 format, Int4 gi, Boolean is_html));
NLM_EXTERN Boolean SeqEntryToFlatAjp PROTO ((Asn2ffJobPtr ajp, SeqEntryPtr sep, FILE *fp, Uint1 format, Uint1 mode));
NLM_EXTERN Boolean SeqEntryToFlat PROTO ((SeqEntryPtr sep, FILE *fp, Uint1 format, Uint1 mode));
NLM_EXTERN Boolean SeqEntryToFlatEx PROTO ((SeqEntryPtr sep, FILE *fp, Uint1 format, Uint1 mode, SeqIdPtr seqid, Uint1 type));
NLM_EXTERN Boolean SeqEntryToGBFlatNoSeq PROTO ((SeqEntryPtr sep, FILE *fp, Uint1 mode, Boolean show_gi));
NLM_EXTERN Boolean SeqSubmitToFlat  PROTO ((SeqSubmitPtr ssp, FILE *fp, Uint1 mode, Boolean show_gi, Uint1 format, Boolean show_gene));
NLM_EXTERN Boolean SeqGenomeToFlat PROTO ((SeqEntryPtr sep, FILE *fp, Uint1 format, Uint1 mode));
NLM_EXTERN Boolean SeqEntryToPartRpt PROTO ((SeqEntryPtr sep, FILE *fp));
NLM_EXTERN Boolean SeqLocToFlat PROTO ((SeqLocPtr slp, FILE *fp, Uint1 format, Uint1 mode));
NLM_EXTERN ByteStorePtr AjpToByteStore PROTO ((Asn2ffJobPtr ajp));

NLM_EXTERN Int2 BioseqGetGBDivCode PROTO((BioseqPtr bsp, CharPtr buffer, Int2 buflen, Boolean err_post));
/* moved from asn2ff1.c to asn2ff6.c */
NLM_EXTERN void AddExtraAccessions PROTO((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN void GetLocusPartsAwp PROTO((Asn2ffJobPtr ajp));
NLM_EXTERN Int4 GetPubsAwp PROTO((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN Boolean SeqToAwp PROTO((GatherContextPtr gcp));
NLM_EXTERN void UseGIforLocus PROTO((Asn2ffJobPtr ajp));

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif
