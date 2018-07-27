/*   ffprint.h
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
* File Name:  ffprint.h
*
* Author:  Karl Sirotkin, Tom Madden, Tatiana Tatusov
*
* Version Creation Date:   7/15/95
*
* $Revision: 6.10 $
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

/*************************************
*
 * $Log: ffprint.h,v $
 * Revision 6.10  2002/08/26 22:06:57  kans
 * ff_RecalculateLinks (MS) to fix hotlink artifact
 *
 * Revision 6.9  1999/08/31 14:36:39  tatiana
 * ff_print_string_mem() added
 *
 * Revision 6.8  1999/04/09 22:21:53  kans
 * fixed prototype for FFBSPrint
 *
 * Revision 6.7  1999/04/09 21:15:27  bazhin
 * Added function "FFBSPrint()".
 *
 * Revision 6.6  1999/04/06 22:37:07  tatiana
 * www_protein_id() added
 *
 * Revision 6.5  1999/03/30 21:02:24  tatiana
 * www_accession www_taxid added
 *
 * Revision 6.4  1999/03/12 17:34:26  tatiana
 * www_featkey() added
 *
 * Revision 6.3  1999/02/02 17:29:21  kans
 * added ff_MergeString
 *
 * Revision 6.2  1998/07/23 22:43:08  tatiana
 * added www_PrintComment()
 *
 * Revision 6.1  1998/05/28 18:30:57  tatiana
 * changed prototype for head_tail_ff()
 *
 * Revision 6.0  1997/08/25 18:05:35  madden
 * Revision changed to 6.0
 *
 * Revision 5.9  1997/08/04 22:56:28  tatiana
 * init_buff_ex() added
 *
 * Revision 5.8  1997/07/18 15:45:09  tatiana
 * AddLinkLater defined as NLM_EXTERN
 *
 * Revision 5.7  1997/07/16 21:22:49  tatiana
 * add AddPintLater
 *
 * Revision 5.6  1997/06/19 18:37:39  vakatov
 * [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
 *
 * Revision 5.5  1997/03/24 20:35:13  shavirin
 * Added protection for usage with C++ compiler
 *
 * Revision 5.4  1997/03/13  19:42:37  tatiana
 * *** empty log message ***
 *
 * Revision 5.3  1997/01/08  23:00:42  kans
 * added ifndef _FFPRINT_ multiple include protection
 *
 * Revision 5.2  1997/01/08  18:52:45  madden
 * Added LIBCALL's.
 *
 * Revision 5.1  1996/06/27  17:18:00  tatiana
 * www_map added
 *
 * Revision 4.12  1996/04/15  18:44:55  tatiana
 * free_buff() added
 *
 * Revision 4.11  1996/04/08  21:52:55  tatiana
 * change in www_featloc
 *
 * Revision 4.10  1996/03/25  15:22:07  tatiana
 * www_featloc added
 *
 * Revision 4.8  1996/02/21  20:11:04  tatiana
 * *** empty log message ***
 *
 * Revision 4.7  1996/01/29  22:45:22  tatiana
 * ChangeStringWithTildes added
 *
 * Revision 4.6  1995/12/20  22:46:19  tatiana
 * Int2 changed to Int4 in www_organism()
 *
 * Revision 4.5  1995/12/13  16:37:46  tatiana
 * www_dbxref added
 *
 * Revision 4.4  1995/11/17  21:52:50  tatiana
 * hot link to genetic code added.c
 *
 * Revision 4.3  1995/11/17  21:28:35  kans
 * asn2ff now uses gather (Tatiana)
 *
 * Revision 1.10  1995/07/17  19:33:20  kans
 * parameters combined into Asn2ffJobPtr structure
 *
*
**************************************/

#ifndef _FFPRINT_
#define _FFPRINT_

#include <asn2ffg.h>

/*--------------the structure for the buffered printing-----*/

#define LINKS 20

#define LOCUS_line 1
#define DEF_line 2
#define ACC_line 3
#define NID_line 4
#define KW_line 5
#define SOURCE_line 6
#define BASECOUNT_line 7
#define ORIGIN_line 8
#define SEQ_line 9

typedef void (*HeadTailProc) PROTO((Pointer, FILE*));

typedef struct buffstruct {

/* The next eight variables are used by the "printing" utilities of asn2ff6.c
(StartPrint, AddChar, CheckBufferState, NewContLine) to perform the "buffered"
printing */ 
	CharPtr buffer;		/* buffer to hold line */
	Int2 init_indent;  /*indentation of the first line, set by StartPrint */
	Int2 cont_indent;  /*indentation of continuation lines */
	Int2 line_max;	/* maximum allowable length of line, set in StartPrint*/
	CharPtr line_prefix; /* prefix, such as "ID" on EMBL id lines */
	Char newline;		/* newline character */
	FILE *fp;		/* file to print to. */
	ByteStorePtr byte_sp;	/* Used to save paragraph (i.e., several lines)
				until printing. */
	CharPtr line_return;
/*  next three variables are used for HTML URLs   */
	Int4  PNTR	pos_links;
	CharPtr	PNTR links;
	Int2	n_links;
	Int2	buf_n_links;
} BuffStruct, PNTR BuffStructPtr;


#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

NLM_EXTERN void LIBCALL asn2ff_set_output PROTO((FILE *fp, CharPtr line_return));
NLM_EXTERN CharPtr LIBCALL ff_MergeString PROTO((void));
NLM_EXTERN CharPtr LIBCALL FFPrint PROTO((FFPrintArrayPtr pap, Int4 index, Int4 pap_size));
NLM_EXTERN void LIBCALL FFBSPrint PROTO((FFPrintArrayPtr pap, Int4 index, Int4 pap_size));
NLM_EXTERN void LIBCALL ff_print_string PROTO((FILE *fp, CharPtr string, CharPtr line_return));
NLM_EXTERN CharPtr LIBCALL ff_print_string_mem PROTO((CharPtr string));
NLM_EXTERN Int2 LIBCALL ff_StartPrint PROTO((Int2 init_indent, Int2 cont_indent, Int2 line_max, CharPtr line_prefix));
NLM_EXTERN void LIBCALL ff_AddString PROTO((CharPtr string));
NLM_EXTERN void LIBCALL ff_AddInteger PROTO((CharPtr fmt, long integer));
NLM_EXTERN void LIBCALL AddLink PROTO((CharPtr str));
NLM_EXTERN void LIBCALL AddLinkLater PROTO((CharPtr str, Int2 prevlen));
NLM_EXTERN void LIBCALL ff_AddChar PROTO((Char character));
NLM_EXTERN void LIBCALL PrintXX PROTO((void));
NLM_EXTERN void LIBCALL ff_AddStringWithTildes PROTO ((CharPtr string));
NLM_EXTERN void LIBCALL ChangeStringWithTildes PROTO ((CharPtr string));
NLM_EXTERN CharPtr LIBCALL CheckBufferState  PROTO((Int2Ptr increment_string, Char next_char));
NLM_EXTERN Int2 LIBCALL NewContLine PROTO((void));
NLM_EXTERN Int2 LIBCALL TabToColumn PROTO((Int2 column));
NLM_EXTERN void LIBCALL ff_EndPrint PROTO((void));
NLM_EXTERN void LIBCALL FlushBuffer PROTO((void));
NLM_EXTERN CharPtr LIBCALL CheckEndPunctuation PROTO((CharPtr string, Char end));
NLM_EXTERN CharPtr PrintDate PROTO((NCBI_DatePtr date));
NLM_EXTERN void LIBCALL BuffFree PROTO((void));
NLM_EXTERN void LIBCALL init_buff PROTO((void));
NLM_EXTERN void LIBCALL init_buff_ex PROTO((Int2 init_size));
NLM_EXTERN void LIBCALL free_buff PROTO((void));
NLM_EXTERN void LIBCALL init_www PROTO((void));
NLM_EXTERN void LIBCALL fini_www PROTO((void));
NLM_EXTERN void LIBCALL head_tail_ff PROTO((Pointer mydata, HeadTailProc headfun, HeadTailProc tailfun));
NLM_EXTERN void LIBCALL head_www PROTO((FILE *fp, SeqEntryPtr sep));
NLM_EXTERN void LIBCALL tail_www PROTO((FILE *fp));
NLM_EXTERN Boolean LIBCALL get_www PROTO((void));
NLM_EXTERN Boolean LIBCALL www_muid PROTO((Int4 muid));
NLM_EXTERN Boolean LIBCALL www_gcode PROTO((CharPtr gcode));
NLM_EXTERN Boolean LIBCALL www_source PROTO((CharPtr orgname, OrgRefPtr orp));
NLM_EXTERN Boolean LIBCALL www_organism PROTO((CharPtr orgname, Int4 id));
NLM_EXTERN Boolean LIBCALL www_taxid PROTO((CharPtr orgname, Int4 id));
NLM_EXTERN Boolean LIBCALL www_extra_acc PROTO((CharPtr acc, Boolean ncbi));
NLM_EXTERN Boolean LIBCALL www_note_gi PROTO((CharPtr str));
NLM_EXTERN Boolean LIBCALL www_db_xref PROTO((CharPtr str));
NLM_EXTERN Boolean LIBCALL www_protein_id PROTO((CharPtr str));
NLM_EXTERN Boolean LIBCALL www_map PROTO((CharPtr str));
NLM_EXTERN Boolean LIBCALL www_genpept_gi PROTO((CharPtr str));
NLM_EXTERN Boolean LIBCALL www_dbsource PROTO((CharPtr str, Boolean first, Uint1 choice));
NLM_EXTERN Boolean LIBCALL www_xref PROTO((CharPtr str, Uint1 link));
NLM_EXTERN Boolean LIBCALL www_xref_button PROTO((FILE *fp, CharPtr str, Uint1 link, Uint1 db));
NLM_EXTERN CharPtr LIBCALL ReportPrint PROTO((FFPrintArrayPtr pap, Int4 index, Int4 pap_size));
NLM_EXTERN Boolean LIBCALL PrintSPBlock PROTO((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN Boolean LIBCALL ff_PrintLine PROTO((Asn2ffJobPtr ajp, GBEntryPtr gbp, Int2 type));
NLM_EXTERN CharPtr LIBCALL www_featloc PROTO((CharPtr loc));
NLM_EXTERN void LIBCALL GetHelpMsg PROTO((SeqEntryPtr sep));
NLM_EXTERN void LIBCALL www_PrintComment  PROTO((CharPtr string, Boolean identifier, Uint1 format));
NLM_EXTERN Boolean LIBCALL www_featkey PROTO((CharPtr key, Int4 gi, Int2 entityID, Int2 itemID));
NLM_EXTERN void LIBCALL www_accession PROTO((CharPtr string));
NLM_EXTERN void LIBCALL ff_RecalculateLinks(Int4 indent);

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* _FFPRINT_ */
