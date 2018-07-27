/*   asn2ff6.h
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
* File Name:  asn2ff6.h
*
* Author:  Karl Sirotkin, Tom Madden, Tatiana Tatusov
*
* Version Creation Date:   7/15/95
*
* $Revision: 6.5 $
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

/*****************************************************************************
*
*	Header file for asn2gb files.
*
****************************************************************************/

/*************************************
*
* $Log: asn2ff6.h,v $
* Revision 6.5  2001/12/05 18:13:59  cavanaug
* Changes for new LOCUS line format
*
* Revision 6.4  2001/07/24 15:12:37  beloslyu
* change the var.name in prototype from new to New to avoid the conflict with c++
*
* Revision 6.3  2000/06/05 17:51:58  tatiana
* increase size of feature arrays to Int4
*
* Revision 6.2  1998/02/10 17:01:22  tatiana
* AddGBQualEx() added
*
* Revision 6.0  1997/08/25 18:05:10  madden
* Revision changed to 6.0
*
* Revision 5.13  1997/06/19 18:37:20  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.12  1997/05/28 16:58:46  tatiana
* GENPEPT_MIN set to 0
*
* Revision 5.11  1997/05/28 16:51:28  tatiana
* GENPEPT_MIN set to 1
*
 * Revision 5.10  1997/01/14  20:53:40  vakatov
 * added LIBCALL to DoSpecialLineBreak
 *
 * Revision 5.9  1997/01/13  22:33:31  tatiana
 * added CompareGeneName()
 *
 * Revision 5.7  1996/12/17  22:47:25  tatiana
 * added StoreFeatFree()
 *
 * Revision 5.6  1996/09/09  13:36:02  kans
 * moved BioseqGetGBDivCode from toasn.[ch] to asn2ff.h/asn2ff6.c
 *
 * Revision 5.5  1996/09/03  19:54:20  tatiana
 * extra_loc added
 *
 * Revision 5.4  1996/08/16  20:36:47  tatiana
 * GetNAFeatKey PROTO changed
 *
 * Revision 5.3  1996/07/30  16:35:40  tatiana
 * Boolean new added to GetNaFeatKey()
 *
 * Revision 5.2  1996/06/14  18:05:18  tatiana
 * GetNAFeatKey change.
 *
 * Revision 5.1  1996/06/11  15:27:34  tatiana
 * GetGINumber has been changed
 *
 * Revision 4.5  1996/01/29  22:43:06  tatiana
 * error posting MODULE removed
 *
 * Revision 4.2  1995/08/18  22:19:04  tatiana
 * *** empty log message ***
 *
 * Revision 4.1  1995/08/01  14:54:04  tatiana
 * change SeqIdPrint to SeqIdWrite
 *
 * Revision 1.12  1995/07/17  19:33:20  kans
 * parameters combined into Asn2ffJobPtr structure
 *
 * Revision 1.11  1995/06/19  21:40:02  kans
 * Tatiana's first major reorganization, moving printing, adding HTML
 *
 * Revision 1.10  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
**************************************/

#ifndef _ASN2FF6_
#define _ASN2FF6_

#include <asn.h>
#include <objall.h>
#include <objpubd.h>
#include <objsub.h>
#include <utilpars.h>
#include <utilpub.h>
#include <gbfeat.h>
#include <gbftdef.h>
#include <asnprt.h>
#include <ftusrstr.h>
#include <asn2ffg.h>


typedef struct {
	Int2 		tech;
	Int2		entityID;
	Int2		techID,
				techtype;
	CharPtr		gbdiv,
				orgdiv;
	Int2		gbID,
				gbtype;
	Int2		orgID,
				orgtype;
	Boolean		err_post,
				was_err;
	BioSourcePtr biosrc;
} DivStruct, PNTR DivStructPtr;

#define REPORT_ON_NA ( (Uint1)0)
#define REPORT_ON_EVERYTHING ( (Uint1)1)
#define REPORT_ON_AA ( (Uint1)2)

#define NOT_LAST ( (Uint1)0)
#define LAST ( (Uint1)1)
#define NO_PRINTXX ( (Uint1)0)
#define PRINTXX ( (Uint1)1)

#define ASN2FLAT_NOT_ALLOC ( (Uint1)0)
#define ASN2FLAT_ALLOC ( (Uint1)1)
#define ALL ( (Uint1)0)
#define RELEASE ( (Uint1)0)
#define DUMP ( (Uint1)1)
#define FLATLOC_CONTEXT_LOC 1
#define FLATLOC_CONTEXT_NOTE 2
#define GENPEPT_MIN 0
#define MAX_BTP_BUF 80
/* THere is a *bad* effect if MAX_TO_RT_SIDE is 60 or more.  For a /translation
the first character is a space and CheckBufferState returns a buffer with
79 characters.  This results in an ABR. */
#define MAX_TO_RT_SIDE 59
#define ASN2FF_EMBL_MAX 78
#define ASN2FF_GB_MAX 79
#define NEWLINE '\n'
#define ASN2FF_STD_BUF 35
#define MAX_LOCUS_NAME_LEN 16
#define MAX_ACCESSION_LEN 25
#define REP_NUM_OF_SEQ_DESC 22

#define SEQ_BLK_SIZE (60*NUM_SEQ_LINES)

#define LOCUS_COLLISION_DB_NAME "GenBank FlatFile LOCUS"

#define GeneStringCmp(s, q)     StringCmp(s, q)


#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

NLM_EXTERN SeqFeatPtr MakeSyntheticSeqFeat PROTO ((void));
NLM_EXTERN GeneStructPtr GeneStructNew PROTO ((void));
NLM_EXTERN void GeneStructFree PROTO ((GeneStructPtr gsp));
NLM_EXTERN void GeneStructReset PROTO ((GBEntryPtr gbp, Int2 index));
NLM_EXTERN NoteStructPtr NoteStructNew PROTO ((NoteStructPtr gsp));
NLM_EXTERN void NoteStructFree PROTO ((NoteStructPtr gsp));
NLM_EXTERN void NoteStructReset PROTO ((NoteStructPtr nsp));
NLM_EXTERN int LIBCALL CompareSfpForHeap PROTO ((VoidPtr vp1, VoidPtr vp2));
NLM_EXTERN int LIBCALL CompareGeneName PROTO ((VoidPtr vp1, VoidPtr vp2));
NLM_EXTERN ValNodePtr FlatRefBest PROTO ((ValNodePtr equiv, Boolean error_msgs, Boolean accept_muid));
NLM_EXTERN SeqFeatPtr PNTR CopyList PROTO((SortStructPtr List, Int4 currentsize));
NLM_EXTERN Int4 StoreFeatFree PROTO((SortStruct PNTR List, SeqFeatPtr sfp, Int4 currentsize, BioseqPtr bsp, BioseqPtr seg, Uint2 entityID, Uint2 itemID, Uint2 thistype, SeqLocPtr slp, SeqLocPtr PNTR extr_loc, Int2 extr_loc_cnt, Boolean feat_free));
NLM_EXTERN Int4 StoreFeatTemp PROTO((SortStruct PNTR List, SeqFeatPtr sfp, Int4 currentsize, BioseqPtr bsp, BioseqPtr seg, Uint2 entityID, Uint2 itemID, Uint2 thistype, SeqLocPtr slp, SeqLocPtr PNTR extr_loc, Int2 extr_loc_cnt, Boolean temp));
NLM_EXTERN Int4 StoreFeat PROTO((SortStruct PNTR List, SeqFeatPtr sfp, Int4 currentsize, BioseqPtr bsp, BioseqPtr seg, Uint2 entityID, Uint2 itemID, Uint2 thistype, SeqLocPtr slp, SeqLocPtr PNTR extr_loc, Int2 extr_loc_cnt));
NLM_EXTERN void ListFree PROTO ((SeqFeatPtr PNTR PNTR List, Int4 range));
NLM_EXTERN CharPtr Cat2Strings PROTO ((CharPtr string1, CharPtr string2, CharPtr separator, Int2 num));
NLM_EXTERN void EnlargeCharPtrStack PROTO ((NoteStructPtr nsp, Int2 enlarge));
NLM_EXTERN SortStructPtr EnlargeSortList PROTO((SortStructPtr List, Int4 size));
NLM_EXTERN void NoteToCharPtrStack PROTO ((NoteStructPtr nsp, CharPtr annot, CharPtr string, Uint1 alloc));
NLM_EXTERN void CpNoteToCharPtrStack PROTO ((NoteStructPtr nsp, CharPtr annot, CharPtr string));
NLM_EXTERN void SaveNoteToCharPtrStack PROTO ((NoteStructPtr nsp, CharPtr annot, CharPtr string));
NLM_EXTERN GBQualPtr AddGBQualEx  PROTO ((CharPtr PNTR key, GBQualPtr gbqual, CharPtr qual, CharPtr val));
NLM_EXTERN GBQualPtr AddGBQual PROTO ((GBQualPtr gbqual, CharPtr qual, CharPtr val));
NLM_EXTERN Int2 CheckForQual PROTO ((GBQualPtr gbqual, CharPtr qual, CharPtr val));
NLM_EXTERN Int4 check_range PROTO ((Pointer data, SeqIdPtr seq_id));
NLM_EXTERN void do_loc_errors PROTO ((CharPtr front, CharPtr details));
NLM_EXTERN void do_no_loc_errors PROTO ((CharPtr front, CharPtr details));
NLM_EXTERN Boolean GBQualPresent PROTO ((CharPtr ptr, GBQualPtr gbqual));
NLM_EXTERN CharPtr MakeAnAccession PROTO ((CharPtr new_buf, SeqIdPtr sip, Int2 buflen));
NLM_EXTERN Boolean GetNAFeatKey PROTO ((Boolean New, CharPtr PNTR buffer, SeqFeatPtr sfp, SeqFeatPtr out));
NLM_EXTERN CharPtr GetGBSourceLine PROTO ((GBBlockPtr gb));
NLM_EXTERN CharPtr FlatOrganelle PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN Int4 GetNumOfSeqBlks PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));

NLM_EXTERN SeqIdPtr GetProductSeqId PROTO ((ValNodePtr product));
NLM_EXTERN Int4 GetGINumFromSip PROTO ((SeqIdPtr sip));
NLM_EXTERN SeqIdPtr CheckXrefFeat PROTO ((BioseqPtr bsp, SeqFeatPtr sfp));
NLM_EXTERN Boolean LIBCALL DoSpecialLineBreak PROTO ((CharPtr string, Int2 indent));

NLM_EXTERN void GetNAPubFeat PROTO ((GBEntryPtr gbp, ValNodePtr PNTR vnpp, SortStructPtr PNTR List, Int2Ptr currentsites));

NLM_EXTERN void CountTheSeqEntrys PROTO ((SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent));
NLM_EXTERN void GetGIs PROTO ((Asn2ffJobPtr ajp));
NLM_EXTERN void GetGINumber PROTO ((GBEntryPtr gbp));


#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif
