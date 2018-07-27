/*  $Id: pgppop.h,v 6.39 2001/01/16 17:15:54 hurwitz Exp $
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
* File Name:  pgppop.h
*
* Author:  Patrick Durand
*
* Version Creation Date:   05/03/99
*
* $Revision: 6.39 $
*
* File Description: 
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: pgppop.h,v $
* Revision 6.39  2001/01/16 17:15:54  hurwitz
* modified DDV_GetBspCoordGivenDispCoord
*
* Revision 6.38  2000/10/13 19:48:16  hurwitz
* added functions for getting first VALID disp coord in range of bsp coords
*
* Revision 6.37  2000/08/25 18:58:29  shavirin
* Added parameter Boolean is_aa to the function DDV_GetBLASTCompLine_1().
*
* Revision 6.36  2000/07/08 20:43:55  vakatov
* Get all "#include" out of the 'extern "C" { }' scope;  other cleanup...
*
* Revision 6.35  2000/05/24 21:42:59  hurwitz
* getting hide/show rows to work with DDV and DDE together
*
* Revision 6.34  2000/05/19 14:30:53  wheelan
* fixed problem with formatting PDB ids
*
* Revision 6.33  2000/03/31 21:33:22  durand
* added new default color schemas for BLAST
*
* Revision 6.32  2000/03/28 13:32:29  durand
* update DDV_DisplayDefaultAlign to receive a pre-initialized DDV_Disp_Opt data structure
*
* Revision 6.31  2000/03/27 14:19:14  durand
* fixed bugs in BLAST outputs
*
* Revision 6.30  2000/03/21 19:26:48  durand
* pgppop now uses AlignMgr
*
* Revision 6.29  2000/02/15 15:31:46  lewisg
* move DDVRulerDescr to pgppop
*
* Revision 6.28  2000/02/14 16:39:56  durand
* add new output options for BLAST
*
* Revision 6.27  1999/12/20 14:37:54  durand
* transfer some PopSet Viewer functions from here to wwwddv.c; update the code to better use Color Manager for BLAST outputs
*
* Revision 6.26  1999/12/08 22:40:54  durand
* add the code to produce colored BLAST outputs
*
* Revision 6.25  1999/12/07 18:46:34  durand
* add DDV_GetBspCoordGivenPgpList function
*
* Revision 6.24  1999/11/26 15:42:26  vakatov
* Fixed for the C++ and/or MSVC DLL compilation
*
* Revision 6.23  1999/11/17 22:42:37  durand
* add entitiesTbl in MsaParaGPopList data structure
*
* Revision 6.22  1999/10/29 14:14:25  durand
*  add DDV_GetBspCoordGivenDispCoord() and DDV_GetDispCoordGivenBspCoord()
*
* Revision 6.21  1999/10/08 17:50:29  durand
* move DDV_DisplayBlastSAP from pgppop.c to ddvcreate.c due to conflict between api and ddv
*
* Revision 6.20  1999/09/29 17:16:45  shavirin
* Modified function DDV_DisplayBlastSAP(): added new parameter and printing
* of the BLAST scores.
*
* Revision 6.19  1999/09/29 13:42:27  durand
* add middle line for BLAST output
*
* Revision 6.18  1999/09/28 19:49:31  shavirin
* Changed definition of the function DDV_DisplayBlastSAP()
*
* Revision 6.17  1999/09/28 13:06:40  durand
* add a first set of functions to display CDS in PopSet Viewer
*
* Revision 6.15  1999/09/16 18:52:27  durand
* redesign the PopSet viewer toolbar
*
* Revision 6.14  1999/09/10 14:27:10  durand
* add RulerDescr in MsaParaGPopList struct to be used for complex ruler types
*
* Revision 6.13  1999/09/02 17:32:23  durand
* add some defines for new display types in DDV
*
* Revision 6.12  1999/08/31 16:45:39  durand
* add sap in MsaParaGPopList structure
*
* Revision 6.11  1999/08/31 13:51:31  durand
* add PubMed link for PopSet Viewer
*
* Revision 6.10  1999/08/30 20:29:56  durand
* make PrintSeqAlignCallback estern function
*
* Revision 6.9  1999/08/30 14:18:09  durand
* use ByteStore to format the SeqAlign output
*
* Revision 6.8  1999/08/16 18:57:00  lewisg
* made DDV_GetSeqAlign extern and added prototype to header
*
* Revision 6.7  1999/08/06 21:43:16  chappey
* SeqAlignToBS new function to save in ByteStore structure the text output of the SeqAlign(s) packaged in a SeqEntry
*
* Revision 6.6  1999/07/22 13:23:14  durand
* made DDV_SearchAli external function
*
* Revision 6.5  1999/07/21 21:52:13  durand
* add some functions to display a summary for a PopSet entry
*
* Revision 6.4  1999/07/20 17:01:48  durand
* add eID field in URLData structure for PopSet Viewer
*
* Revision 6.3  1999/07/19 21:16:02  durand
* add DDV_ResetParaGSeqAlignCoord to reset the seqalign coord in the display data structures of DDV
*
* Revision 6.2  1999/07/15 18:20:51  durand
* add display options to support BLAST outputs
*
* Revision 6.1  1999/07/09 13:59:58  durand
* move pgppop from desktop to api
*
* Revision 6.2  1999/07/06 22:31:24  kans
* removed whitespace before #define directives
*
* Revision 6.1  1999/07/06 20:18:07  kans
* initial public checkin
*
* Revision 1.30  1999/07/06 18:54:27  durand
* add new features for the display of PopSet viewer
*
* Revision 1.29  1999/07/02 13:22:18  durand
* fix bugs for the display of minus strand sequences
*
* Revision 1.28  1999/06/29 16:48:11  shavirin
* Changed definition of function DDV_ShowSeqAlign()
*
* Revision 1.27  1999/06/28 22:07:22  durand
* add loader functions and clean the code with Lint and Purify
*
* Revision 1.26  1999/06/25 14:21:07  durand
* add a new command-line to wwwddv.cgi
*
* Revision 1.25  1999/06/24 20:49:42  shavirin
* Added new function DDV_ShowSeqAlign().
*
* Revision 1.24  1999/06/23 17:24:24  durand
* use a binary encoding to manage the display styles
*
* Revision 1.23  1999/06/21 18:37:57  durand
* update DDV_DisplayDefaultAlign to produce full text output
*
* Revision 1.22  1999/06/19 18:36:13  durand
* new display procedure
*
* Revision 1.21  1999/06/14 23:49:44  durand
* add function for Vibrant DDV
*
* Revision 1.20  1999/06/11 22:33:02  durand
* add new functions for Vibrant DDV
*
* Revision 1.19  1999/06/11 18:03:14  durand
* add declarations
*
* Revision 1.17  1999/06/11 14:01:07  durand
* add a define _PGPPOP_ to avoid compilation error when mutilple include
*
* Revision 1.16  1999/06/09 21:35:30  durand
* add constructors/destructors for BspInfo struct as well as read seq function
*
*
*
*
* ==========================================================================
*/
#ifndef _PGPPOP_
#define _PGPPOP_

#include <ncbi.h>
#include <objseq.h>
#include <blocks.h>
#include <udvseq.h>
#include <ddvcolor.h>


#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define WWW_SCRIPT_SIZE 1000
#define LETTER_BLOCK_WIDTH 10
#define LETTER_HALF_BLOCK_WIDTH 5
#define SCALE_TICK 1
#define SCALE_NUM 2
#define TABLE_LEFT_MARGIN 16

#define INFO_SIZE 255
#define ParaG_Size 70

/*used by the layout manager (see DDVOptionsBlock
  data structure in this file*/
#define DDV_USE_STDCLR      ((Uint1)1)/*use standard aa or na colors-mainly used by DDV itself*/
#define DDV_USE_NOMLAYOUT   ((Uint1)2)/*layout the Aligned regions*/
#define DDV_USE_UALAYOUT    ((Uint1)4)/*layout the UA regions*/
#define DDV_USE_GAPLAYOUT   ((Uint1)8)/*layout the GAP*/
#define DDV_USE_MASKLAYOUT  ((Uint1)16)/*layout masked regions*/
#define DDV_USE_ISOLAYOUT   ((Uint1)32)/*layout ident/simil/other*/

/*used to set the layout for letters (initially designed for new BLAST outputs*/
/*note: it's possible to combine all the values*/
#define DDV_TXTSTYLE_ITALIC     ((Uint1)1)
#define DDV_TXTSTYLE_BOLD       ((Uint1)2)
#define DDV_TXTSTYLE_UNDERLINE  ((Uint1)4)
#define DDV_TXTSTYLE_LOWERCASE  ((Uint1)8)
#define DDV_TXTSTYLE_COLOR      ((Uint1)16)
	

typedef struct ddvrulerdescr{
	Int4    disp_start;/*start in display coordinate*/
	Int4    disp_stop;/*stop in display coordinate*/
	Int4    align_start;/*start in align coordinate*/
	Boolean bUnAligned;
} DDVRulerDescr, PNTR DDVRulerDescrPtr;

/*****************************************************************************
	Populated lists of the elements ready for the display
	
    Note : use the following data structure to display in the 
           Vibrant viewer/editor
	       if (DisplayType==DDV_DISP_HORZ) use 'TableHead '
		   else use 'DisplayVert'
	
*****************************************************************************/
	typedef struct msaparagpoplist {
		Int4 LengthAli;				/*Alignment size after population from index*/
		Int4 nBsp;					/*number of Bioseq after...*/
		SABlockPtr sabp;/*indexed seqalign*/
		SeqAlignPtr sap;
    SeqAlignPtr viewed_sap;  /* so that DDE can remember DDV's sap */
		ValNodePtr PNTR TableHead;	/*Table list of the heads of the ValNode
									ParaG list for each bioseq*/
		ValNodePtr DisplayVert;		/*List of ParaG Ready for display*/
		BspInfoPtr bspp; /*info for each bsp of the seqalign*/
		Uint1 DisplayType;/*see defines above*/
		ValNodePtr RulerDescr; /*use by DDV only; ruler descriptor*/
		Uint4Ptr   entitiesTbl;/* "   "  "  "   ; eID, iID of each bioseq*/
	} MsaParaGPopList, PNTR MsaParaGPopListPtr;

/*****************************************************************************
	Command-Line or URL content
*****************************************************************************/
typedef struct urldata{	/*structure containing the decoded URL*/
	Int4		gi;		/*gi to retrieve in ID1*/
	Int4		from;	/*want to display from...*/
	Int4		to;		/*... to (SeqAlign coord)*/
	Uint4		disp_format;/*display format (seebelow)*/
	CharPtr		szFileNameIn;/*used only for Unix command-line*/
	CharPtr		szFileNameOut;/*used only for Unix command-line*/
	SeqEntryPtr sep; /*used by DDV_ToolsScript*/
	CharPtr     szGIs;/*used of the realign stuff*/
	Uint2        eID;/*entityID; used by PopSet viewer to display onSeqAlign
			among others*/
	} URLData, PNTR URLDataPtr;

/*****************************************************************************
	data block to get BioSource(s) 
*****************************************************************************/
typedef struct popsource{
	CharPtr szTaxName;
	CharPtr szCommonName;
} PopSource, PNTR PopSourcePtr;
	
typedef struct ddvextlayout{
	Uint1 style;
	Uint1 rgb[3];
	Uint1 clr_ident;
	Uint1 clr_simil;
	Uint1 clr_other;
	Uint1 sty_ident;
	Uint1 sty_simil;
	Uint1 sty_other;
}DDVExtLayout, PNTR DDVExtLayoutPtr;

/*****************************************************************************
	data blocks for enhanced display options
*****************************************************************************/
typedef struct ddv_disp_opt{
	/*color display ?*/
	Boolean bUseColors;
	/*styles for a disc. seqalign*/
	Boolean ShowLeftTail;
	Boolean ShowRightTail;
	Uint1   DispDiscStyle;
	Uint1   SpacerSize;
    Uint1   DiscJustification;
	Uint1   UAGapStyle;
	Uint1   AGapStyle;
	/*this field is here because it's closely related to the SeqAlign*/
	Uint1   RulerStyle; /*for bioseq coord system; used only when rebuild the display*/
	}DDV_Disp_Opt, PNTR DDV_Disp_OptPtr;

typedef struct ddvoptionsblock{	
	Int2          LineSize;/*size of a sequence line*/
	ByteStorePtr PNTR byteSpp;/*use this to store the formatted align as a
	    ByteStore*/
	/*the following are mainly designed for enhanced HTML BLAST outputs*/
	Int4Ptr PNTR  matrix;
	Boolean       bUseLayout;
	Uint1         LayoutType;
	DDVExtLayout  normlayout;
	DDVExtLayout  UAlayout;
	DDVExtLayout  GAPlayout;
	DDVExtLayout  MASKlayout;
	DDVExtLayout  ISOlayout;
	/*additional styles*/
	DDV_Disp_OptPtr ddop;
	} DDVOptionsBlock, PNTR DDVOptionsBlockPtr;
	
/*************************************************************************
	DISPLAY FORMATS. the following values can be putted in a single Uint4
		using the binary operator '|='
*************************************************************************/
		/*ruler styles : left, right, top (all), ticks*/
#define RULER_TOP ((Uint4)1)
#define RULER_LEFT ((Uint4)2)
#define RULER_RIGHT ((Uint4)4)
#define RULER_TICK ((Uint4)8)
		/*display enhancement : color, show by block of 10 letters*/
#define DISPE_COLOR ((Uint4)16)
#define DISPE_SHOWBLOCK ((Uint4)32)
		/*seq. letters to show : all, variations*/
#define VIEW_FULLSEQ ((Uint4)64)
#define VIEW_VARIA ((Uint4)128)
		/*output format */
#define DISP_FULL_HTML ((Uint4)256)
#define DISP_PHYLIP_TXT ((Uint4)512)
#define DISP_FASTA_NOGAP ((Uint4)1024)
#define DISP_FASTA_GAP ((Uint4)2048)
#define DISP_FULL_TXT ((Uint4)4096)
		/*content of a line */
		/* a line is like that : [...] Name < 4125 AGCT AGCT ... 4254*/
		/*Name is always displayed*/
#define DISP_ORDER_NUM ((Uint4)8192) /*[...]*/
#define DISP_STRAND ((Uint4)16384)   /* < or > */
#define DISP_BSP_COORD ((Uint4)32768)
		/*use this flag to force a rebuild a new align*/
#define ALIGN_REBUILD ((Uint4)65536)
		/*use this flag to display lower case letters; otherwise upper case is used*/
#define TEXT_LOWERCASE ((Uint4)131072)
		/*use this flag to display 'QUERY:' and 'SBJCT:' as seq. name
		should be used for BLAST output only*/
#define SEQNAME_BLAST_STD ((Uint4)262144)
#define DISP_BLAST_STD ((Uint4)524288)
		/*display a SeqAlign in a table; require to use DISP_FULL_HTML*/
#define DISPE_TABLE ((Uint4)1048576)
		/*BLAST only : if set, display the middle line between query & subject*/
#define DISP_BLAST_MIDLINE ((Uint4)2097152)
		/*don't add a hot libk on the acc. number*/
#define DISP_NOLINKONNAME ((Uint4)4194304)

#define DDV_AddBlank(_diff, _fp) \
do { Int4 _i;  for (_i=0; _i<(Int4)_diff; _i++) fprintf(_fp," "); } while(0)

#define DDV_AddBlank2(_diff, _str) \
do { Int4 _i;  for (_i=0; _i<(Int4)_diff; _i++) *_str=DDV_ConcatStr(*_str," "); } while(0)

/*Entrez and PopSet Viewer scripts*/
#define szEntrezScript "http://www.ncbi.nlm.nih.gov/entrez/utils/qmap.cgi?uid=%d&form=6&db=%s&Dopt=g"
#define szEntrezScriptSMART "http://www.ncbi.nlm.nih.gov/entrez/utils/qmap.cgi?uid=%s&form=6&db=%s&Dopt=g"
#define szSeqNameStatus "<a href=\"%s\" onMouseOut=\"window.status=''\" \nonMouseOver=\"window.status='%s';return true\">"
#define WWWDDV_save_script1 "wwwddv.cgi"
#define WWWDDV_script "wwwddv.cgi?gi=%d&from=%d&to=%d&disp=%u"
#define WWWDDV_script2 "wwwddv.cgi?gis=%s&disp=%u"


NLM_EXTERN void DDV_DeleteTxtList(ValNodePtr PNTR vnp);
NLM_EXTERN void DDV_DeleteParaGList(ValNodePtr PNTR vnp);
NLM_EXTERN void DDV_DeleteDisplayList(MsaParaGPopListPtr mpplp);
NLM_EXTERN void DDV_ResetParaGSeqAlignCoord(MsaParaGPopListPtr mpplp,Int2 LineSize);
NLM_EXTERN CharPtr DDV_GetBLASTCompLine_1(CharPtr szQuery, CharPtr szSubject, 
		Int4Ptr PNTR matrix, Boolean is_aa);
NLM_EXTERN CharPtr DDV_GetBLASTCompLine_2(ParaGPtr pgpQuery, ParaGPtr pgpSubject, 
		Int4Ptr PNTR matrix);
NLM_EXTERN CharPtr DDV_GetBLASTCompLine_3(CharPtr szQuery, ParaGPtr pgpSubject, 
		Int4Ptr PNTR matrix);
NLM_EXTERN void DDV_AffichageParaG(MsaParaGPopListPtr mpplp,Int4 gi,Int4 from,Int4 to,
		Int4 TotalAliLength,Int4 numBlockAffich,Uint4 disp_format,Int2 LineSize,
		FILE *fp,ByteStorePtr PNTR bspp,Int4Ptr PNTR matrix,DDV_ColorGlobal * gclr,
		ValNodePtr mask);
NLM_EXTERN Boolean DDV_PopDisplay(SABlockPtr sabp,MsaParaGPopListPtr mpplp,
			Int4 nBSP,Uint1 MasterScaleStyle,Boolean ShowTick,
			Int2 cbl,Int4 nBlockToPop);
NLM_EXTERN Boolean DDV_PopDisplay2(SABlockPtr sabp,MsaParaGPopListPtr mpplp,
			Int4 nBSP,Uint1 MasterScaleStyle,Boolean ShowTick,
			Int2 cbl,Int4 nBlockToPop,Int4 ali_from,Int4 ali_to);
NLM_EXTERN Boolean DDV_GetSequenceFromParaG(ParaGPtr pgp,
		CharPtr PNTR szSequence,Int4 bspLength,Boolean IsAA,Uint1 PNTR bsp_strand,
		Int4 PNTR bsp_start,Int4 PNTR bsp_stop);
NLM_EXTERN Boolean DDV_GetFullFASTAforIdxAli(SeqAlignPtr sap,FILE *fp);
NLM_EXTERN Boolean DDV_GetFullGapFASTAforIdxAli(MsaParaGPopListPtr mpplp,FILE *fp);
NLM_EXTERN void DDV_PrintStudyName(CharPtr szPopSetName,CharPtr szPopSetAuth,
		CharPtr szJournalTitle,Int4 pmid,FILE *fp);
NLM_EXTERN void	DDV_GetArticleInfo(SeqEntryPtr sep,CharPtr szPopSetName,
	CharPtr szPopSetAuth,CharPtr szJournalTitle,Int4Ptr pmid);
NLM_EXTERN Boolean DDV_DisplayDefaultAlign(SeqAlignPtr sap,Int4 gi,Int4 from,Int4 to,
		Uint4 disp_format,Pointer disp_data,FILE *fp);
NLM_EXTERN CharPtr DDV_ReadSeqGivenSpp(SeqPortPtr spp,Int4 from,Int4 to,Uint1 strand,
	Boolean IsAA,BoolPtr bError);
NLM_EXTERN SeqPortPtr DDV_OpenBspFullSeqPort(BioseqPtr bsp,Uint1 strand);
NLM_EXTERN BspInfoPtr DDV_BspInfoNew(Boolean InitInfo,Boolean InitSeqPort,SeqIdPtr sip,
		Uint1 strand);
NLM_EXTERN BspInfoPtr DDV_BspInfoDelete(BspInfoPtr bip);
NLM_EXTERN BspInfoPtr DDV_BspInfoDeleteList(BspInfoPtr bip);
NLM_EXTERN void DDV_LocateParaG(ValNodePtr PNTR TableHead,Int4 nBsp);
NLM_EXTERN Boolean DDV_CreateDisplay(SeqAlignPtr sap,MsaParaGPopListPtr mpplp);
NLM_EXTERN Boolean DDV_ShowSeqAlign(SeqAlignPtr seqalign, Int4 gi, Int4 from, Int4 to,
                         Uint4 disp_format);
NLM_EXTERN void	DDV_GetEntryBioSource(SeqEntryPtr sep,ValNodePtr PNTR vnpp);

NLM_EXTERN void DDV_PrintPopSetSummary(SeqEntryPtr sep, Int4 gi, FILE *FileOut);
NLM_EXTERN void DDV_SearchAli(SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent);
NLM_EXTERN void DDV_GetSeqAlign(Pointer dataptr, Uint2 datatype,
		ValNodePtr PNTR vnp);


NLM_EXTERN void PrintSeqAlignCallback (SeqEntryPtr sep, Pointer mydata,
                                   Int4 index, Int2 indent);
NLM_EXTERN ByteStorePtr SeqAlignToBS (Uint2 entity);

NLM_EXTERN Int4 DDV_GetBspCoordGivenDispCoord(ParaGPtr pgp, Int4 disp_pos);
NLM_EXTERN Int4 DDV_GetBspCoordGivenDispCoord2(ParaGPtr pgp, Int4 disp_pos, Boolean* pUnAligned);
NLM_EXTERN Int4 DDV_GetBspCoordGivenPgpList(ValNodePtr ParaG_List,Int4 disp_pos);
NLM_EXTERN Int4 DDV_GetDispCoordGivenBspCoord(ValNodePtr vnp_head,Int4 bsp_pos);
NLM_EXTERN DDVRulerDescrPtr DDV_RulerDescrNew(DDVRulerDescrPtr pRulerDescr);
NLM_EXTERN DDVRulerDescrPtr DDV_RulerDescrFree(DDVRulerDescrPtr pRulerDescr);

NLM_EXTERN Int4 DDV_GetFirstDispCoordGivenBspCoordRange(ValNodePtr vnp_head,
                                                        Int4 disp_start, Int4 disp_stop);
NLM_EXTERN Int4 DDV_GetLastDispCoordGivenBspCoordRange(ValNodePtr vnp_head,
                                                       Int4 disp_start, Int4 disp_stop);

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* ndef _PGPPOP_ */
