/*  $Id: ddvcreate.c,v 1.66 2001/02/01 00:39:53 lewisg Exp $
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
* File Name:  ddvcreate.c
*
* Author:  Sarah Wheelan
*
* Version Creation Date:   08/99
*
* $Revision: 1.66 $
*
* File Description: 
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: ddvcreate.c,v $
* Revision 1.66  2001/02/01 00:39:53  lewisg
* fix leak
*
* Revision 1.65  2001/01/29 20:35:49  hurwitz
* make gap between aligned blocks optional
*
* Revision 1.64  2001/01/25 21:49:14  hurwitz
* added option for extra gap char (tilde) between aligned blocks
*
* Revision 1.63  2001/01/19 22:05:40  wheelan
* took out small spaces created by UABuildDescriptor
*
* Revision 1.62  2001/01/10 23:38:39  lewisg
* fix seqid and various memory leaks
*
* Revision 1.61  2000/09/08 21:50:38  hurwitz
* made DDV_ReadSeqBin public
*
* Revision 1.60  2000/09/06 17:41:26  shavirin
* Fixed bug with printing gaps with discontinuous alignment.
*
* Revision 1.59  2000/08/25 18:55:27  shavirin
* Changed layout - unaligned regions will be always in lower characters.
*
* Revision 1.58  2000/08/22 19:59:28  shavirin
* Changed layout no.1 - it will be used for standalone blast with
* decline-to-align parameter not = 0
*
* Revision 1.57  2000/07/19 18:46:19  bauer
* Fixed bug in DDV_CreateDisplayFromIndex_EX
*
* Revision 1.56  2000/06/29 23:15:13  hurwitz
* leave single space between aligned blocks with no unaligned sequence between them, no auto-merge of adjacent aligned blocks
*
* Revision 1.55  2000/06/22 20:56:52  hurwitz
* assorted bug fixes
*
* Revision 1.54  2000/06/13 18:23:55  hurwitz
* made ViewMgr_MakeMultiple routine, call this on each launch of DDE rather than launch of DDV
*
* Revision 1.53  2000/06/12 23:02:40  hurwitz
* enable launch of DDE from Cn3D, swith from DDV_ComputeRuler to DDE_ReMakeRuler, get rid of styles option for DDE
*
* Revision 1.52  2000/06/08 20:04:38  hurwitz
* made warning about converting to true multiple alignment into a Message window, and other small fixes
*
* Revision 1.51  2000/06/07 19:09:36  hurwitz
* made DDE_ReMakeRuler work with linked list of ParaGs
*
* Revision 1.50  2000/05/10 16:17:00  hurwitz
* can show tails for just 1 aligned block
*
* Revision 1.49  2000/05/08 13:16:00  wheelan
* replaced segment-based counting with block-based counting
*
* Revision 1.48  2000/05/04 22:43:38  hurwitz
* don't launch DDE on top of DDV, change some wording, redraw DDE after save to AlnMgr
*
* Revision 1.47  2000/05/03 19:34:38  wheelan
* added fix for NULL alignments
*
* Revision 1.46  2000/05/03 14:51:02  thiessen
* revert to ~ gap char for Cn3D
*
* Revision 1.45  2000/04/21 23:00:50  hurwitz
* can launch DDE from DDV
*
* Revision 1.44  2000/04/19 19:08:23  hurwitz
* problem with justification of flanking unaligned regions fixed
*
* Revision 1.43  2000/04/19 15:45:50  hurwitz
* can create display for a block
*
* Revision 1.42  2000/04/06 17:59:50  durand
* fixed a conflict between BLAST and DDV in the use of the function DDV_SetSTyle
*
* Revision 1.41  2000/04/04 19:08:00  durand
* add new BLAST color schemas; fixed a bug in DDV_PopulateDisplayForDisc
*
* Revision 1.40  2000/03/31 21:33:07  durand
* added new default color schemas for BLAST
*
* Revision 1.39  2000/03/28 21:20:07  shavirin
* Adjusted for correct structure of BLAST location mask.
*
* Revision 1.38  2000/03/28 17:08:17  durand
* removed debugging code lines
*
* Revision 1.37  2000/03/27 14:19:28  durand
* fixed bugs in BLAST outputs
*
* Revision 1.36  2000/03/21 19:26:45  durand
* adapt ddvcreate to be used by pgppop
*
* Revision 1.35  2000/03/20 13:46:23  durand
* fixed a bug in BLAST functions; added DDV_BLAST_COLOR0 to produce a BLAST output without any layout
*
* Revision 1.34  2000/02/28 16:52:47  durand
* wrote a new manager for unaligned regions
*
* Revision 1.33  2000/02/23 21:34:29  durand
* allow custom display of gap characters
*
* Revision 1.32  2000/02/23 19:49:33  durand
* use row number instead of SeqId for coloring
*
* Revision 1.31  2000/02/17 15:54:56  durand
* use ~ for an unaligned gap and - for an aligned gap
*
* Revision 1.30  2000/02/15 22:40:57  lewisg
* add ability to launch udv so that it colors by row, fixes to colormgr, track rows from viewmgr, fix visual c projects
*
* Revision 1.29  2000/02/14 16:49:33  durand
* add new options for BLAST output
*
* Revision 1.28  2000/02/11 13:58:21  wheelan
* bug fix for unaligned region of length 0 (Durand)
*
* Revision 1.27  2000/02/07 14:03:35  durand
* replace BioseqUnlockById by BioseqUnlock
*
* Revision 1.26  2000/02/05 01:32:22  lewisg
* add viewmgr, move place freeing is done in ddv, modify visual c++ projects
*
* Revision 1.25  2000/02/02 14:44:30  durand
* added function to create data structure for block editor, fixed some bugs
*
* Revision 1.24  2000/01/26 13:38:54  durand
* update the GUI for the editor. Add functions to create the data to be used by the editor
*
* Revision 1.23  1999/12/29 22:55:03  lewisg
* get rid of seqalign id
*
* Revision 1.22  1999/12/20 20:20:41  lewisg
* allow cn3d to do color and ddv to do case when both are running
*
* Revision 1.21  1999/12/20 14:45:37  durand
* add new functions for the new BLAST outputs
*
* Revision 1.20  1999/12/08 22:42:17  durand
* add the code to produce colored BLAST outputs
*
* Revision 1.19  1999/12/03 23:17:22  lewisg
* Patrick's new global update msg, argument passing when launching ddv, experimental editing
*
* Revision 1.18  1999/12/03 18:24:08  durand
* gapped regions are displayed using lower cases; BLAST outputs
*
* Revision 1.17  1999/12/03 13:23:08  durand
* replace AlnMgrGetNumSeqs by AlnMgrGetNumRows
*
* Revision 1.16  1999/12/02 14:46:42  durand
* fix a bug in UABuildDescriptor; problem with unaligned region of size 0
*
* Revision 1.15  1999/11/24 21:26:29  vakatov
* Fixed for the C++ and/or MSVC DLL compilation
*
* Revision 1.14  1999/11/17 22:44:02  durand
* speed up the selection manager for large SeqAlign
*
* Revision 1.13  1999/11/09 22:14:01  shavirin
* Added parameter follower to the Blast score printing function
*
* Revision 1.12  1999/11/09 17:08:59  durand
* transfer some functions from ddvgraph to ddvcreate, so that ddvcreate remains Vibrant free and can be compiled with BLAST
*
* Revision 1.11  1999/11/03 21:29:48  durand
* add CTRL and SHFT keys for mouse selection. redesign the loader functions of DDV to properly register the message callbacks
*
* Revision 1.10  1999/10/29 14:15:40  durand
* add simple mouse selection functions
*
* Revision 1.9  1999/10/22 20:12:47  durand
* add Export command (text, HTML and Phylip formats)
*
* Revision 1.8  1999/10/22 14:57:25  durand
* use AlnMgrIndexSingleChildSeqAlign to build BLAST output
*
* Revision 1.7  1999/10/22 13:17:19  durand
* remove unreferenced variables to avoid warnings with Visual C
*
* Revision 1.6  1999/10/20 13:17:19  durand
* add display for disc. SeqAlign tails
*
* Revision 1.5  1999/10/15 21:57:35  durand
* add a UI for display options
*
* Revision 1.4  1999/10/12 15:01:29  lewisg
* resolve confict with internal/ddv
*
* Revision 1.3  1999/10/07 13:36:08  wheelan
* bug fixes -- AlnLengths were 1 too long
*
* Revision 1.2  1999/10/05 14:04:21  wheelan
* made DDV_CreateDisplayFromIndex_2, which creates the display for discontinuous alignments; minor bug fixes
*
* Revision 1.1  1999/09/30 14:10:25  durand
* add ddv to toolkit
*
* Revision 1.11  1999/09/30 13:38:09  durand
* DDV_CreateDisplayFromIndex takes ParaG_Size as an argument
*
*
*
* ==========================================================================
*/
#include <ddvcreate.h>
#include <pgppop.h>
#include <udvseq.h>
#include <tofasta.h>
#include <sqnutils.h>

typedef struct ddvdataforcolorfunc{
	SeqIdPtr sip;
	Int4     from;
	Int4     to;
	Int4     row;
	Uint1    strand;
	Boolean  IsUnAligned;
	Uint1    style;
	Uint1    rgb[3];
	Uint1    UAstyle;
	Uint1    UArgb[3];
} DDVDataForColorFunc, PNTR DDVDataForColorFuncPtr;

#define MAX_NCBIstdaa 26
#define MAX_NCBI4na 17
#define DDVCOL_DEFAULT 0
#define DDVCOL_HOTPINK 1
#define DDVCOL_MAGENTA 2
#define DDVCOL_PURPLE 3
#define DDVCOL_BLUE 4
#define DDVCOL_SKY 5
#define DDVCOL_CYAN 6
#define DDVCOL_SEA 7
#define DDVCOL_GREEN 8
#define DDVCOL_YELLOW 9
#define DDVCOL_GOLD 10
#define DDVCOL_ORANGE 11
#define DDVCOL_RED 12
#define DDVCOL_PINK 13
#define DDVCOL_PINKTINT 14
#define DDVCOL_WHITE 15
#define DDVCOL_BLACK 16
#define DDVCOL_BLUETINT 17
#define DDVCOL_GREENTINT 18
#define DDVCOL_YELLOWTINT 19
#define DDVCOL_GRAY 20
#define DDVCOL_BROWN 21
#define DDV_COLOR_MAX 33 
 static Uint1 DDV_PaletteRGB[DDV_COLOR_MAX][3] =
 {
     /* Red  Grn Blue     Color     ColorID             Old Hex  New Hex */
   255, 255, 255, /* default     0                 0xffffff - */ 
   255,   0, 153, /* hotpink     1                 0xff1493 0xff0099 */
   255,   0, 255, /* magenta     2                 0xff00ff - */
   153,  51, 255, /* purple      3                 0x9b30ff 0x9933ff */
     0,   0, 255, /* blue        4                 0x0000ff - */
    00, 153, 255, /* sky         5                 0x1e90ff 0x0099ff */
     0, 255, 255, /* cyan        6                 0x00ffff - */
     0, 255, 153, /* sea         7                 0x00ff8f 0x00ff99 */
     0, 192,   0, /* green       8                 0x00ff00 - */
   255, 255,   0, /* yellow      9                 0xffff00 - */
   255, 204,   0, /* gold       10                 0xffa500 0xffcc00 */
   255, 102,   0, /* orange     11                 0xff4500 0xff6600 */
   255,   0,   0, /* red        12                 0xff0000 - */
   255, 153, 153, /* pink       13                 0xff7256 0xff9999 */
   255, 204, 204, /* pinktint   14                 0xffaeb9 0xffcccc */
   255, 255, 255, /* white      15                0xffffff  -*/
     0,   0,   0, /* black      16                0x000000 - */
   204, 204, 255, /* bluetint   17                0xb0e2ff - 0xccccff*/
   153, 255, 153, /* greentint  18                0x9aff9a - 0x99ff99 */
   255, 255, 153, /* yellowtint 19                0xffec8b - 0xffff99 */
   102, 102, 102, /* gray       20                0x7d7d7d   0x666666 */
   153, 102,  51, /* brown      21                0x8b5742   01x996633 */
   255,   0,   0, /* perm colors 22: red          0xff0000 - */
     0, 255,   0, /* perm colors 23: green        0x00ff00 - */
   255,   0, 255, /* perm colors 24: magenta      0xff00ff - */
    00, 153, 255, /* perm colors 25: sky          0x1e90ff 0x0099ff */
   153,	 51, 255, /* perm colors 26: purple        0x9b30ff 0x9933ff */
     0, 255,   0, /* SS colors 27: helix, green   0x00ff00 - */
   255, 204,   0, /* SS colors 28: strand, gold   0xffa500 0xffcc00 */
   255, 102,   0, /* SS colors 29: turn, orange   0xff4500 0xff6600 */
     0, 255, 255, /* SS colors 30: coil, cyan     ox00ffff - */
   255, 255,   0, /* highlight colors 31: yellow  0xffff00 - */
     0,   0,   0 /* background colors 32: black  0x000000 - */
 };
Uint1 DDV_STD_AAColor[MAX_NCBIstdaa] =
{DDVCOL_BROWN, /*-*/
DDVCOL_GRAY,   /*A*/
DDVCOL_ORANGE, /*B*/
DDVCOL_PINK, /*C*/
DDVCOL_RED,    /*D*/
DDVCOL_RED,    /*E*/
DDVCOL_PURPLE, /*F*/
DDVCOL_GRAY,   /*G*/
DDVCOL_BLUE,   /*H*/
DDVCOL_GRAY,   /*I*/
DDVCOL_BLUE,   /*K*/
DDVCOL_GRAY,   /*L*/
DDVCOL_PINK, /*M*/
DDVCOL_CYAN,   /*N*/
DDVCOL_MAGENTA,/*P*/
DDVCOL_CYAN,   /*Q*/
DDVCOL_BLUE,   /*R*/
DDVCOL_GREEN,  /*S*/
DDVCOL_GREEN,  /*T*/
DDVCOL_GRAY,   /*V*/
DDVCOL_PURPLE, /*W*/
DDVCOL_GRAY,   /*X*/
DDVCOL_PURPLE, /*Y*/
DDVCOL_ORANGE, /*Z*/
DDVCOL_GOLD,   /*U*/
DDVCOL_HOTPINK /***/};

Uint1 DDV_STD_NAColor[MAX_NCBI4na] = {
DDVCOL_PINK,   /* gap */
DDVCOL_GREEN,  /* A */
DDVCOL_BLUE,   /* C */
DDVCOL_CYAN,   /* A or C */
DDVCOL_BLACK,  /* G */
DDVCOL_CYAN,   /* G or A */
DDVCOL_CYAN,   /* G or C */
DDVCOL_PURPLE, /* G C or A */
DDVCOL_RED,    /* T */
DDVCOL_CYAN,   /* A or T */
DDVCOL_CYAN,   /* T or C */
DDVCOL_PURPLE, /* A C ot T */
DDVCOL_CYAN,   /* G ot T */
DDVCOL_PURPLE, /* G or A or T */
DDVCOL_PURPLE, /* G T or C */
DDVCOL_HOTPINK,/* G T A or C */
DDVCOL_GRAY /*?, but MAX_NCBI4na==17, see mmdbapi2.h*/
};

extern void display_ParaG_content(MsaParaGPopListPtr mpplp,Int2 LineSize)
{
BioseqPtr bsp_debug=NULL;
Char                    szBuf[15];
ParaGPtr                pgp;
MsaTxtDispPtr mtdp;
ValNodePtr    vnp,vnp2;
Uint4 j,k,size,sum;
FILE *fp;
Boolean bClose=TRUE;
Int4 i;
	if (LineSize<=50)/*just for the integrity of the function...*/
		LineSize=ParaG_Size;
		
        fp=fopen("zpgplist.txt","w");
        if (fp==NULL) {fp=stdout;bClose=FALSE;}
        for(i=0;i<mpplp->nBsp;i++){
                /*for each line (i.e. bioseq) */
                vnp=mpplp->TableHead[i];
                if (vnp) pgp=(ParaGPtr)(vnp->data.ptrvalue);
                else pgp=NULL;
                if(pgp){
                        if (pgp->sip) bsp_debug=BioseqLockById(pgp->sip);
                        else bsp_debug=NULL;
                        if (bsp_debug){
                                /*get the access code*/
                                SeqIdWrite(bsp_debug->id, szBuf, PRINTID_TEXTID_ACCESSION, 14);
                                fprintf(fp,"[%5u] %s , size: %d, IsAA :%d \n",
                                        i,szBuf,BioseqGetLen(bsp_debug),
                                        ISA_aa(bsp_debug->mol));
                                BioseqUnlock(bsp_debug);
                        }
                        else StringCpy(szBuf,"unknown");
                }
                k=1;size=1;
                while(vnp){
                        pgp=(ParaGPtr)(vnp->data.ptrvalue);
                        if (pgp){/*create the name table*/
                                /*loop on the pgp*/
                                fprintf(fp,"  ->ParaG[%d] , range (%7d..%7d):\n",k++,size,size+LineSize-1);
                                size+=LineSize;
                                vnp2=pgp->ptxtList;
                                j=1;sum=0;
                                while(vnp2){
                                        mtdp=(MsaTxtDispPtr)vnp2->data.ptrvalue;
                                        if (mtdp){
                                        fprintf(fp,"    (%4u): range(%7d..%7d) (%2d), Gap: %2d, Strand(%d)\n",
                                                j++,mtdp->from,mtdp->to,mtdp->to-mtdp->from+1,mtdp->IsGap,
                                                mtdp->strand);
                                        }
										sum+=(mtdp->to-mtdp->from+1);
                                        vnp2=vnp2->next;
                                }
								fprintf (fp,"            Total = %d.\n",sum);
                        }
                        vnp=vnp->next;
                }
                fprintf(fp,"\n");
        }
        if (bClose) fclose(fp);

}

static Boolean DDV_CreateDisplayFromIndex_1(SeqAlignPtr sap, MsaParaGPopListPtr mpplp, Int2 LineSize,
		Int4 start,Int4 stop)
{
   AlnMsgPtr      amp;
   Int4           i,from,to;
   Boolean        more;
   MsaTxtDispPtr  mtdp;
   Int4           n;
   ParaGPtr       pgp;
   SeqIdPtr       sip;
   ValNodePtr     vnp;
   ValNodePtr     vnp_head;
   ValNodePtr     PNTR vnp_list;  
   ValNodePtr     vnp_para;
   ValNodePtr     vnp_para_head;

	if (LineSize<=50)/*just for the integrity of the function...*/
		LineSize=ParaG_Size;
		
   amp = AlnMsgNew();
   vnp_list = (ValNodePtr PNTR)MemNew((mpplp->nBsp)*sizeof(ValNodePtr));
	if (start!=(Int4)-1){
		from=start;
	}
	else{
		from=0;
	}
	if (stop!=(Int4)-1){
		to=stop+1;
	}
	else{
		to=mpplp->LengthAli;
	}

   for (n = 1; n<=(mpplp->nBsp); n++)
   {
      vnp_para = vnp_para_head = NULL;
      sip = AlnMgrGetNthSeqIdPtr(mpplp->sap, n);
      for (i = from; i<to; i+=LineSize)
      {
         amp = AlnMsgReNew(amp);
         pgp = (ParaGPtr)MemNew(sizeof(ParaG));
         pgp->StartLine = n - 1;
         pgp->nLines = 1;
         pgp->StartLetter = amp->from_m = i;
         if (i + LineSize >= to)
            pgp->StopLetter = amp->to_m = to - 1;
         else
            pgp->StopLetter = amp->to_m = i + LineSize -1;
         if(i == from) pgp->sip = sip;
         else pgp->sip = SeqIdDup(sip);
         pgp->ScaleStyle = SCALE_POS_NONE;
         amp->which_bsq = NULL;
         amp->row_num = n;
         vnp = vnp_head = NULL;
         amp->place = 0;
         while (more = AlnMgrGetNextAlnBit(mpplp->sap, amp))
         {
            vnp = ValNodeAdd(&vnp);
            if (!vnp_head)
               vnp_head = vnp;
            mtdp = (MsaTxtDispPtr)MemNew(sizeof(MsaTxtDisp));
            mtdp->from = amp->from_b;
            mtdp->to = amp->to_b;
            mtdp->IsGap = (Boolean)(amp->gap);
            if (mtdp->IsGap)
               mtdp->TextStyle = MSA_TXT_STYLE_GAP;
            else
               mtdp->TextStyle = MSA_TXT_STYLE_SEQ;
            mtdp->strand = amp->strand;
			mtdp->IsUnAligned=FALSE;
            vnp->data.ptrvalue = mtdp;
         }
         pgp->ptxtList = vnp_head;
         vnp_para = ValNodeAdd(&vnp_para);
         vnp_para->data.ptrvalue = pgp;
         if (!vnp_para_head)
            vnp_para_head = vnp_para;
      }
      vnp_list[n-1]=vnp_para_head;
   }
   mpplp->TableHead = vnp_list;
   /*display_ParaG_content(mpplp,LineSize);*/
   DDV_LocateParaG(mpplp->TableHead,mpplp->nBsp);
   return TRUE;
}


static Boolean DDV_CreateDisplayFromIndex_2(SeqAlignPtr sap, MsaParaGPopListPtr mpplp, Int2 LineSize)
{
   AlnMsgPtr        amp;
   Int4             i;
   Boolean          more;
   MsaTxtDispPtr    mtdp;
   Int4             n;
   ParaGPtr         pgp;


   ValNodePtr       vnp;
   ValNodePtr       vnp_head;
   ValNodePtr       PNTR vnp_list;
   ValNodePtr       vnp_para;
   ValNodePtr       vnp_para_head;
   Int4             PopulateTo;
   Int4             ReportSpacer;
   Int4             PlaceSpacer;
   Int4             RealAlnLength;
   SeqIdPtr         sip;
   Boolean          last;


   amp = AlnMsgNew();
   vnp_list = (ValNodePtr PNTR)MemNew((mpplp->nBsp)*sizeof(ValNodePtr));
   RealAlnLength = AlnMgrGetAlnLength(mpplp->sap, FALSE);
   for (n = 1; n<=(mpplp->nBsp); n++)
   {
      vnp_para = vnp_para_head = NULL;
      sip = AlnMgrGetNthSeqIdPtr(mpplp->sap, n);
          ReportSpacer=0;
          amp->to_m=-1;
      amp = AlnMsgReNew(amp);
      last = FALSE;
      for (i = 0; i<mpplp->LengthAli; i+=LineSize)
      {
         amp->place = 0;
         pgp = (ParaGPtr)MemNew(sizeof(ParaG));
         pgp->StartLine = n - 1;
         pgp->nLines = 1;
         pgp->StartLetter = i;
         if (i + LineSize >= mpplp->LengthAli)
         {
            last = TRUE;
            pgp->StopLetter = mpplp->LengthAli;
         }
         else
            pgp->StopLetter = i + LineSize -1;
         if(i == 0) pgp->sip = sip;
         else pgp->sip = SeqIdDup(sip);
         pgp->ScaleStyle = SCALE_POS_NONE;
         amp->which_master = 0;
         if (amp->to_m != 0)
                 amp->from_m = amp->to_m + 1 ;
         else
                 amp->from_m = 0;
         amp->to_m = amp->from_m + LineSize -1;
         amp->row_num = n;
         if (amp->to_m > RealAlnLength)
            amp->to_m = RealAlnLength - 1;
         more = TRUE;
         vnp = vnp_head = NULL;
                 PopulateTo=0;
                 PlaceSpacer=0;
         while (amp->place == 0)
         {
            vnp = ValNodeAdd(&vnp);
            if (!vnp_head)
               vnp_head = vnp;
            if (amp->send_space)
            {
               if (!last)
                  amp->to_m=amp->to_m - PlaceSpacer;
            }
                        if (ReportSpacer){
                           amp->to_m -=ReportSpacer;
               mtdp = (MsaTxtDispPtr)MemNew(sizeof(MsaTxtDisp));
               mtdp->from = pgp->StartLetter;
               mtdp->to = mtdp->from+ReportSpacer-1;
                           PopulateTo+=(ReportSpacer+1);
                           ReportSpacer=0;
               mtdp->IsGap = TRUE;
               mtdp->TextStyle = MSA_TXT_STYLE_1;
               vnp->data.ptrvalue = mtdp;
               vnp = ValNodeAdd(&vnp);
                        }
            amp->send_space = FALSE;
            more = AlnMgrGetNextAlnBit(mpplp->sap, amp);
            mtdp = (MsaTxtDispPtr)MemNew(sizeof(MsaTxtDisp));
            mtdp->from = amp->from_b;
            mtdp->to = amp->to_b;
            PopulateTo+=(amp->to_b-amp->from_b+1);
                        mtdp->IsGap = (Boolean)(amp->gap);
            if (mtdp->IsGap)
               mtdp->TextStyle = MSA_TXT_STYLE_GAP;
            else
               mtdp->TextStyle = MSA_TXT_STYLE_SEQ;
            mtdp->strand = amp->strand;
            vnp->data.ptrvalue = mtdp;
            if (amp->send_space)
            {
               vnp = ValNodeAdd(&vnp);
               mtdp = (MsaTxtDispPtr)MemNew(sizeof(MsaTxtDisp));
               mtdp->from = pgp->StartLetter+PopulateTo;
               mtdp->to = mtdp->from+SPACER_TXT_BLANK-1;
                           if (mtdp->to>pgp->StopLetter){
                               ReportSpacer=mtdp->to-pgp->StopLetter;
                               mtdp->to=pgp->StopLetter;
               }
                           else{
                               ReportSpacer=0;
                           }
                           PlaceSpacer=SPACER_TXT_BLANK-ReportSpacer;
                           PopulateTo+=(mtdp->to-mtdp->from+1);
               mtdp->IsGap = TRUE;
               mtdp->TextStyle = MSA_TXT_STYLE_1;
				mtdp->IsUnAligned=FALSE;
               vnp->data.ptrvalue = mtdp;
            }
         }
         pgp->ptxtList = vnp_head;
         vnp_para = ValNodeAdd(&vnp_para);
         vnp_para->data.ptrvalue = pgp;
         if (!vnp_para_head)
            vnp_para_head = vnp_para;
      }
      vnp_list[n-1]=vnp_para_head;
   }
   mpplp->TableHead = vnp_list;
   /*display_ParaG_content(mpplp,LineSize);*/
   DDV_LocateParaG(mpplp->TableHead,mpplp->nBsp);
   return TRUE;
}


NLM_EXTERN Boolean DDV_CreateDisplayFromIndex_EX(SeqAlignPtr sap, MsaParaGPopListPtr mpplp, 
		Int2 LineSize, DDV_Disp_OptPtr ddop,Int4 start, Int4 stop) {
/*******************************************************************************
*  same as DDV_CreateDisplayFromIndex_EX2, but by default no extra gap char.
*******************************************************************************/
  return(DDV_CreateDisplayFromIndex_EX2(sap, mpplp, LineSize, ddop, start, stop, FALSE));
}


NLM_EXTERN Boolean DDV_CreateDisplayFromIndex_EX2(SeqAlignPtr sap, MsaParaGPopListPtr mpplp, 
		Int2 LineSize, DDV_Disp_OptPtr ddop,Int4 start, Int4 stop, Boolean WantExtraGapChar)
{
   AMAlignIndexPtr  amaip;
   AlnMsgPtr        amp;
   Int4             i,from,to;
   Boolean          more;
   MsaTxtDispPtr    mtdp;
   Int4             n;
   ParaGPtr         pgp;


   ValNodePtr       vnp;
   ValNodePtr       vnp_head;
   ValNodePtr       PNTR vnp_list;
   ValNodePtr       vnp_para;
   ValNodePtr       vnp_para_head;
   Int4             PopulateTo;


   SeqIdPtr         sip;
   Boolean          last;

	if (LineSize<=50)/*just for the integrity of the function...*/
		LineSize=ParaG_Size;
		
   if (!sap)
      return FALSE;
   if (!AlnMgrIndexSeqAlign(sap))
      return FALSE;
   if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndexPtr)sap->saip;
      if (sap->type == SAT_PARTIAL || 
         (sap->type == SAT_MASTERSLAVE && (amaip->mstype == AM_SEGMENTED_MASTERSLAVE || 
                                           amaip->mstype == AM_NULL ||
                                           amaip->mstype == AM_MASTERSLAVE)))
      {
         return(DDV_CreateDisplay_DiscAlign2(sap,mpplp,LineSize,ddop,WantExtraGapChar));
      } 
      else if (sap->type == SAT_MASTERSLAVE && amaip->mstype == AM_MASTERSLAVE)
      {
         mpplp->sap = sap;
         if (start==(Int4)-1 && stop==(Int4)-1)
			 mpplp->LengthAli = AlnMgrGetAlnLength(mpplp->sap, FALSE);
		else
			mpplp->LengthAli=stop-start+1;
         mpplp->nBsp = AlnMgrGetNumRows(mpplp->sap);
         mpplp->DisplayType = DDV_DISP_HORZ;
         if (!DDV_CreateDisplayFromIndex_1(sap, mpplp,LineSize,start,stop))
            return FALSE;
         else
            return TRUE;
      } else
         mpplp->sap = (SeqAlignPtr)sap->segs;
   } else 
      mpplp->sap = sap;
   mpplp->LengthAli = AlnMgrGetAlnLength(mpplp->sap, FALSE);
   mpplp->nBsp = AlnMgrGetNumRows(mpplp->sap);
   mpplp->DisplayType = DDV_DISP_HORZ;
   amp = AlnMsgNew();
   vnp_list = (ValNodePtr PNTR)MemNew((mpplp->nBsp)*sizeof(ValNodePtr));
	if (start!=(Int4)-1){
		from=start;
	}
	else{
		from=0;
	}
	if (stop!=(Int4)-1){
		to=stop+1;
	}
	else{
		to=mpplp->LengthAli;
	}
   for (n = 1; n<=(mpplp->nBsp); n++)
   {
      vnp_para = vnp_para_head = NULL;
      sip = AlnMgrGetNthSeqIdPtr(mpplp->sap, n);
	  amp->to_m=-1;
      amp = AlnMsgReNew(amp);
      last = FALSE;
      for (i = from; i<to; i+=LineSize)
      {
         amp->place = 0;
         pgp = (ParaGPtr)MemNew(sizeof(ParaG));
         pgp->StartLine = n - 1;
         pgp->nLines = 1;
         pgp->StartLetter = i;
         if (i + LineSize >= to)
         {
            last = TRUE;
            pgp->StopLetter = to;
         }
         else
            pgp->StopLetter = i + LineSize -1;
         if(i == from) pgp->sip = sip;
         else pgp->sip = SeqIdDup(sip);
         pgp->ScaleStyle = SCALE_POS_NONE;
         amp->which_master = 0;
         if (amp->to_m != 0)
		 amp->from_m = amp->to_m + 1 ;
         else
                 amp->from_m = 0;
	 amp->to_m = amp->from_m + LineSize -1;
         amp->row_num = n;
         if (amp->to_m >= to)
            amp->to_m = to - 1;
         more = TRUE;
         vnp = vnp_head = NULL;
		 PopulateTo=0;
         while (more = AlnMgrGetNextAlnBit(mpplp->sap, amp))
         {
            vnp = ValNodeAdd(&vnp);
            if (!vnp_head)
               vnp_head = vnp;
            mtdp = (MsaTxtDispPtr)MemNew(sizeof(MsaTxtDisp));
            mtdp->from = amp->from_b;
            mtdp->to = amp->to_b;
			mtdp->IsGap = (Boolean)(amp->gap);
            if (mtdp->IsGap)
               mtdp->TextStyle = MSA_TXT_STYLE_GAP;
            else
               mtdp->TextStyle = MSA_TXT_STYLE_SEQ;
            mtdp->strand = amp->strand;
			mtdp->IsUnAligned=FALSE;
            vnp->data.ptrvalue = mtdp;
         }
         pgp->ptxtList = vnp_head;
         vnp_para = ValNodeAdd(&vnp_para);
         vnp_para->data.ptrvalue = pgp;
         if (!vnp_para_head)
            vnp_para_head = vnp_para;
      }
      vnp_list[n-1]=vnp_para_head;
   }
   mpplp->TableHead = vnp_list;
   /*display_ParaG_content(mpplp,LineSize);*/
   DDV_LocateParaG(mpplp->TableHead,mpplp->nBsp);
   return TRUE;
}


NLM_EXTERN Boolean DDV_CreateDisplayFromIndex(SeqAlignPtr sap, MsaParaGPopListPtr mpplp, 
		Int2 LineSize, DDV_Disp_OptPtr ddop) {
/*******************************************************************************
*  same as DDV_CreateDisplayFromIndex2, but by default no extra gap char.
*******************************************************************************/
  return(DDV_CreateDisplayFromIndex2(sap, mpplp, LineSize, ddop, FALSE));
}


NLM_EXTERN Boolean DDV_CreateDisplayFromIndex2(SeqAlignPtr sap, MsaParaGPopListPtr mpplp, 
		Int2 LineSize, DDV_Disp_OptPtr ddop, Boolean WantExtraGapChar)
{
	return(DDV_CreateDisplayFromIndex_EX2(sap,  mpplp, 
		 LineSize,  ddop, (Int4) -1,  (Int4) -1, WantExtraGapChar));
}


static Boolean DDV_PopulateDisplayForDisc(SeqAlignPtr sap,MsaParaGPopListPtr mpplp,
	Int4 nBsp,Int4 LineSize,DDV_Disp_OptPtr ddop,Int4 TotLength,ValNodePtr vnp_head,
  Boolean OverrideDiscJustification, Boolean ForceLeft)
{
/*-------------------------------------------------------------
  Added OverrideDiscJustification:
  Set this true for single block alignment.  In that case,
  the first unaligned region will be right-justified and
  the second unaligned region will be left-justified.

  Added ForceLeft:
  Set this true for single unaligned region alignment.
  In that case, the unaligned region will be left-aligned.
-------------------------------------------------------------*/
Int4            cumulpop,StartLetter,n,nPgp,from_bsp,to_bsp,MaxLength,
                BspLength=0,BspStart,BspStop,AbsPos;
Boolean         more,bFirstMtdp,bFirstPgp,IsGap;
ValNodePtr      vnp,vnp2,vnp3,vnp_para,vnp_mtdp=NULL;
ValNodePtr PNTR vnp_list;
ParaGPtr        pgp;
SeqIdPtr        sip;
DescriDispPtr   ddp;
MsaTxtDispPtr   mtdp_pgp;
AlnMsgPtr       amp;
UAMsgPtr        uamp;
Uint1           strand = Seq_strand_unknown;
Uint1           strand_tmp, Justification;
Int4            PassCount;

	/*now, use the previous descriptor to build the ParaG List*/
	mpplp->sap=sap;
	mpplp->nBsp = nBsp;
	mpplp->LengthAli = TotLength;
	mpplp->DisplayType = DDV_DISP_HORZ;
	amp = AlnMsgNew();
	vnp_list = (ValNodePtr PNTR)MemNew((mpplp->nBsp)*sizeof(ValNodePtr));
	if (!vnp_list || !amp){
		/*delete the descriptor*/
		ValNodeFreeData(vnp_head);
		return(FALSE);
	}

	cumulpop=0;
	
	/*loop on each sequence*/
	for (n = 1; n<=(mpplp->nBsp); n++){
		sip = AlnMgrGetNthSeqIdPtr(mpplp->sap, n);
		/*for each sequence, use the descriptors to build the ParaG list*/
		bFirstPgp=TRUE;
		bFirstMtdp=TRUE;
		StartLetter=0;AbsPos=0;/*disp coord*/
		nPgp=0;
		vnp=vnp_head;
    PassCount = 0;
		while(vnp){
			ddp=(DescriDispPtr)vnp->data.ptrvalue;
			if (ddp->TextStyle==MSA_TXT_STYLE_REG_ALIGN){/*aligned*/
				/*initialize the amp struct for the current aligned region*/
				amp = AlnMsgReNew(amp);
				amp->place = 0;
				amp->from_m = ddp->from;
				amp->to_m=ddp->to;
				amp->which_master=0;
				amp->row_num=n;
				amp->send_space = FALSE;
				more=TRUE;
				AlnMgrGetNextAlnBit(mpplp->sap, amp);
				while (more){
					/*list of TxT descriptors*/
					mtdp_pgp = (MsaTxtDispPtr)MemNew(sizeof(MsaTxtDisp));
					mtdp_pgp->IsGap = (Boolean)(amp->gap);
					if (mtdp_pgp->IsGap){/*disp coord*/
						mtdp_pgp->from=AbsPos;
						mtdp_pgp->to=mtdp_pgp->from+(amp->to_b-amp->from_b);
						mtdp_pgp->TextStyle = ddop->AGapStyle;
					}
					else{/*BSP coord*/
						mtdp_pgp->from = amp->from_b;
						mtdp_pgp->to = amp->to_b;
						mtdp_pgp->TextStyle = MSA_TXT_STYLE_SEQ;
					}
					mtdp_pgp->strand = amp->strand;
					mtdp_pgp->IsUnAligned=FALSE;
					strand=amp->strand;
					/*add the new node*/
					if (bFirstMtdp){
						vnp2=ValNodeAddPointer(NULL,0,(Pointer)mtdp_pgp);
						vnp_mtdp=vnp2;
						bFirstMtdp=FALSE;
					}
					else{
						vnp2=ValNodeAddPointer(&vnp2,0,(Pointer)mtdp_pgp);
					}
					more=AlnMgrGetNextAlnBit(mpplp->sap, amp);
					AbsPos+=(mtdp_pgp->to-mtdp_pgp->from+1);
				}
			}
			else{/*unaligned*/
				switch(ddop->DispDiscStyle){/*user's display choice*/
					case MSA_TXT_STYLE_1:/*just put a little spacer*/
						mtdp_pgp = (MsaTxtDispPtr)MemNew(sizeof(MsaTxtDisp));
						mtdp_pgp->from = StartLetter;/*put DISP coord*/
						mtdp_pgp->to = mtdp_pgp->from+(ddp->to-ddp->from);
						mtdp_pgp->IsGap = TRUE;
						mtdp_pgp->TextStyle = MSA_TXT_STYLE_1;
						mtdp_pgp->strand = strand;
						mtdp_pgp->IsUnAligned=TRUE;
						/*add the new node*/
						if (bFirstMtdp){
							vnp2=ValNodeAddPointer(NULL,0,(Pointer)mtdp_pgp);
							vnp_mtdp=vnp2;
							bFirstMtdp=FALSE;
						}
						else{
							vnp2=ValNodeAddPointer(&vnp2,0,(Pointer)mtdp_pgp);
						}
						break;
					case MSA_TXT_STYLE_2:/*display the unaligned sequence*/
						MaxLength=ddp->UAMaxlength;
						
						if (ddp->UAnum>0){/*within SAP*/
							AlnMgrGetNthUnalignedForNthRow(sap, ddp->UAnum, n, &BspStart, 
								&BspStop);
						}
						else{/*Tails of the SAP*/
							if (AlnMgrGetNthRowTail(sap,n,
								(Uint1)(ddp->UAnum==(Int4)-1 ? LEFT_TAIL : RIGHT_TAIL),
								&BspStart,&BspStop,&strand_tmp)==FALSE){
								BspStart=(Int4)-1;
								BspStop=(Int4)-1;
								BspLength=0;
							}
						
						}
												
						if (BspStart!=(Int4)-1 && BspStop!=(Int4)-1){
							BspLength=BspStop-BspStart+1;
						}
						/*init the analysis process of an UA region*/
						switch(ddp->UAnum){
							case (Int4)-1:/*left tail*/
                Justification = ForceLeft ? DISP_JUST_LEFT : DISP_JUST_RIGHT;
								uamp=UAMgrIntUAMsg(MaxLength,BspLength,BspStart,BspStop, 
									ddp->from,ddp->to, Justification, strand);
								break;
							case (Int4)-2:/*right tail*/
								uamp=UAMgrIntUAMsg(MaxLength,BspLength,BspStart,BspStop, 
									ddp->from,ddp->to,DISP_JUST_LEFT, strand);
								break;
							default:/*within SAP*/
                Justification = ddop->DiscJustification;
                if (OverrideDiscJustification) {
                  if (PassCount==0) {Justification = DISP_JUST_RIGHT;}
                  if (PassCount==2) {Justification = DISP_JUST_LEFT;}
                }
								uamp=UAMgrIntUAMsg(MaxLength,BspLength,BspStart,BspStop, 
									ddp->from,ddp->to, Justification, strand);
								break;
						}
						/*analyse the content of the UA region*/
						while(UAMgrGetNextUAbit(uamp, BspStart, BspStop)){
							/*get data*/
							UAMgrUAMsgGetInfo(uamp,&from_bsp,&to_bsp,&IsGap);
							/*list of TxT descriptors*/
							mtdp_pgp = (MsaTxtDispPtr)MemNew(sizeof(MsaTxtDisp));
							mtdp_pgp->IsGap = IsGap;
							if (mtdp_pgp->IsGap){/*disp coord*/
								mtdp_pgp->TextStyle = ddop->UAGapStyle;
								mtdp_pgp->from=AbsPos;
								mtdp_pgp->to=mtdp_pgp->from+(to_bsp-from_bsp);
							}
							else{/*BSP coord*/
								mtdp_pgp->from = from_bsp;
								mtdp_pgp->to = to_bsp;
								mtdp_pgp->TextStyle = MSA_TXT_STYLE_SEQ;
							}
							mtdp_pgp->strand = strand;
							mtdp_pgp->IsUnAligned=TRUE;
							/*add the new node*/
							if (bFirstMtdp){
								vnp2=ValNodeAddPointer(NULL,0,(Pointer)mtdp_pgp);
								vnp_mtdp=vnp2;
								bFirstMtdp=FALSE;
							}
							else{
								vnp2=ValNodeAddPointer(&vnp2,0,(Pointer)mtdp_pgp);
							}
							AbsPos+=(mtdp_pgp->to-mtdp_pgp->from+1);
						}
						uamp=UAMgrDelUAMsg(&uamp);
						break;
				}
			}
			cumulpop+=(ddp->to-ddp->from+1);
			StartLetter+=(ddp->to-ddp->from+1);
			if (cumulpop==LineSize || vnp->next==NULL){
				/*new ParaG*/
				pgp=(ParaGPtr)MemNew(sizeof(ParaG));
				pgp->StartLine=n-1;
				pgp->nLines=1;
				pgp->StartLetter=nPgp*LineSize;
				pgp->StopLetter=_min_(pgp->StartLetter+LineSize-1,mpplp->LengthAli-1);
				pgp->sip=SeqIdDup(sip);
				pgp->ScaleStyle=SCALE_POS_NONE;
				pgp->ptxtList=vnp_mtdp;
				if (bFirstPgp){
					vnp3=ValNodeAddPointer(NULL,0,(Pointer)pgp);
					vnp_para=vnp3;
					bFirstPgp=FALSE;
				}
				else{
					vnp3=ValNodeAddPointer(&vnp3,0,(Pointer)pgp);
				}
				bFirstMtdp=TRUE;
				cumulpop=0;
				nPgp++;
			}
			vnp=vnp->next;
      PassCount++;
		}/*while() on the descriptor list*/
		vnp_list[n-1]=vnp_para;
        SeqIdFree(sip);
	}/*for() on the bsp */
	
	/*delete the descriptor*/
	ValNodeFreeData(vnp_head);

	mpplp->TableHead = vnp_list;
	/*just for debugging purpose*/
	/*display_ParaG_content(mpplp,LineSize);*/
	DDV_LocateParaG(mpplp->TableHead,mpplp->nBsp);
	MemFree(amp);

	return(TRUE);
}

NLM_EXTERN Boolean DDV_CreateDisplay_DiscAlign(SeqAlignPtr sap, 
		MsaParaGPopListPtr mpplp, Int2 LineSize,DDV_Disp_OptPtr ddop) {
/*******************************************************************************
*  same as DDV_CreateDisplay_DiscAlign2, but by default no extra gap char.
*******************************************************************************/
  return(DDV_CreateDisplay_DiscAlign2(sap, mpplp, LineSize, ddop, FALSE));
}


/*******************************************************************************

  Function : DDV_CreateDisplay_DiscAlign2()
  
  Purpose : create a display for a Disc. SeqAlign

  Return value : FALSE if failure

*******************************************************************************/
NLM_EXTERN Boolean DDV_CreateDisplay_DiscAlign2(SeqAlignPtr sap, 
		MsaParaGPopListPtr mpplp, Int2 LineSize,DDV_Disp_OptPtr ddop, Boolean WantExtraGapChar)
{
Int4 nBsp,TotLength;
ValNodePtr vnp_head;

	/*descriptor*/
	nBsp = AlnMgrGetNumRows(sap);
	vnp_head=UABuildDescriptor2(sap,nBsp,LineSize,ddop,&TotLength,
		ddop->ShowLeftTail,ddop->ShowRightTail, WantExtraGapChar);
	if (vnp_head==NULL)
		return(FALSE);

	/*build the display*/
	return(DDV_PopulateDisplayForDisc(sap,mpplp,nBsp,LineSize,ddop,
		TotLength,vnp_head, FALSE, FALSE));
}

/*******************************************************************************

  Function : DDV_BuildDisp_BlockEditor()
  
  Purpose : create a display for a Disc. SeqAlign

  Return value : FALSE if failure

*******************************************************************************/
NLM_EXTERN Boolean DDV_BuildDisp_BlockEditor(SeqAlignPtr sap, 
		MsaParaGPopListPtr mpplp, Int4 block_num)
{
Int4 nBsp,TotLength;
ValNodePtr vnp_head;
DDV_Disp_Opt ddo;

	/*descriptor*/
	nBsp = AlnMgrGetNumRows(sap);
	vnp_head=UABuildDescriptorForBlockEditor(sap,nBsp, 
			block_num,LIMIT_EDITOR_SIZE,&TotLength);
	
	if (vnp_head==NULL)
		return(FALSE);

	DDV_InitDDESAPdispStyles(&ddo);
	/*build the display*/
	return(DDV_PopulateDisplayForDisc(sap,mpplp,nBsp,LIMIT_EDITOR_SIZE,&ddo,
		TotLength,vnp_head, FALSE, FALSE));
}

/*******************************************************************************

  Function : UABitDescrInit()
  
  Purpose : init a UABitDescr data block
  
  Return value : -

*******************************************************************************/
static void UABitDescrInit(UABitDescrPtr ubd,Int4 Start,Int4 Stop,Boolean IsGap)
{
	ubd->from=Start;
	ubd->to=Stop;
	ubd->IsGap=IsGap;
}

/*******************************************************************************

  Function : UAMgrInitBitList()
  
  Purpose : init the descriptor of an UA region
  
  Return value : -

*******************************************************************************/
static void UAMgrInitBitList(UAMsgPtr uamp, Int4 MaxLength, Int4 BspLength,
		Int4 BspStart,Int4 BspStop)
{
Int4 halfBspLengthSeqL,halfBspLengthSeqR,halfBspLengthDesc,start_middle,stop_middle;

	/*just a gap*/
	if (BspStart==(Int4)-1 && BspStop==(Int4)-1){
		UABitDescrInit(&uamp->ubd[0],0,MaxLength-1,TRUE);
		UABitDescrInit(&uamp->ubd[1],(Int4)-1,(Int4)-1,FALSE);
		UABitDescrInit(&uamp->ubd[2],(Int4)-1,(Int4)-1,FALSE);
		return;
	}
	/*only sequence*/
	if (BspLength==MaxLength){
		UABitDescrInit(&uamp->ubd[0],BspStart,BspStop,FALSE);
		UABitDescrInit(&uamp->ubd[1],(Int4)-1,(Int4)-1,FALSE);
		UABitDescrInit(&uamp->ubd[2],(Int4)-1,(Int4)-1,FALSE);
		return;
	}
	/*additional data for DISP_JUST_CENTER and DISP_JUST_SPLIT*/
	if (uamp->DispType==DISP_JUST_CENTER || uamp->DispType==DISP_JUST_SPLIT){
		halfBspLengthSeqL=BspLength/2;
		halfBspLengthSeqR=BspLength-halfBspLengthSeqL;
		halfBspLengthDesc=MaxLength/2;
		start_middle=halfBspLengthDesc-halfBspLengthSeqL;
		stop_middle=start_middle+BspLength;
	}

	/*only one sequence letter and DISP_JUST_SPLIT : use DISP_JUST_LEFT*/
	if (BspStart==BspStop && uamp->DispType==DISP_JUST_SPLIT){
		uamp->DispType=DISP_JUST_LEFT;
	}

	switch(uamp->DispType){
		case DISP_JUST_RIGHT:
			UABitDescrInit(&uamp->ubd[0],0,MaxLength-BspLength-1,TRUE);
			UABitDescrInit(&uamp->ubd[1],BspStart,BspStop,FALSE);
			UABitDescrInit(&uamp->ubd[2],(Int4)-1,(Int4)-1,FALSE);
			break;
		case DISP_JUST_CENTER:
			UABitDescrInit(&uamp->ubd[0],0,start_middle-1,TRUE);
			UABitDescrInit(&uamp->ubd[1],BspStart,BspStop,FALSE);
			UABitDescrInit(&uamp->ubd[2],stop_middle,MaxLength-1,TRUE);
			break;
		case DISP_JUST_SPLIT:
			UABitDescrInit(&uamp->ubd[0],BspStart,BspStart+halfBspLengthSeqL-1,FALSE);
			UABitDescrInit(&uamp->ubd[1],halfBspLengthSeqL,MaxLength-halfBspLengthSeqR-1,TRUE);
			UABitDescrInit(&uamp->ubd[2],BspStart+halfBspLengthSeqL,BspStop,FALSE);
			break;
		case DISP_JUST_LEFT:
			UABitDescrInit(&uamp->ubd[0],BspStart,BspStop,FALSE);
			UABitDescrInit(&uamp->ubd[1],BspLength,MaxLength-1,TRUE);
			UABitDescrInit(&uamp->ubd[2],(Int4)-1,(Int4)-1,FALSE);
		default:
			break;
	}
}


/*******************************************************************************

  Function : UAMgrIntUAMsg()
  
  Purpose : constructor for a struct UAMsg
  
  Note : from and to are in UnAligned region coord. system. Such a coord system
         always starts at 0. Length = max length of the UA region
		 
  Return value : -

*******************************************************************************/
NLM_EXTERN UAMsgPtr UAMgrIntUAMsg( Int4 MaxLength, Int4 BspLength,
		Int4 BspStart,Int4 BspStop,Int4 ddpFrom ,Int4 ddpTo , Uint1 DispType, Uint1 strand)
{
UAMsgPtr uamp=NULL;

	uamp=(UAMsgPtr)MemNew(sizeof(UAMsg));
	if (uamp){
		uamp->DispType=DispType;
		uamp->strand=strand;
		uamp->from_a=uamp->currentPos=ddpFrom;
		uamp->to_a=ddpTo;
		UAMgrInitBitList(uamp,MaxLength,BspLength,BspStart,BspStop);
	}
	return(uamp);
}

/*******************************************************************************

  Function : UAMgrDelUAMsg()
  
  Purpose : destructor for UAMsg structure
    
  Return value : -

*******************************************************************************/
NLM_EXTERN UAMsgPtr UAMgrDelUAMsg(UAMsgPtr PNTR uamp)
{
	if (*uamp)
		MemFree(*uamp);
	return(NULL);
}

/*******************************************************************************

  Function : UAMgrUAMsgGetInfo()
  
  Purpose : query function for a UAMsg structure
  
  Note : from_bsp and to_bsp are in BSP coord system.
    
  Return value : -

*******************************************************************************/
NLM_EXTERN void UAMgrUAMsgGetInfo(UAMsgPtr uamp,Int4Ptr from_bsp, Int4Ptr to_bsp, 
		BoolPtr IsGap)
{
	*from_bsp=uamp->from_r;
	*to_bsp=uamp->to_r;
	*IsGap=uamp->IsGap;
}

/*******************************************************************************

  Function : UAMgrGetNextUAbit()
  
  Purpose : analytical function to retrieve information about an unaligned
            region. uamp must be init. before entering this function.
    
*******************************************************************************/
NLM_EXTERN Boolean UAMgrGetNextUAbit(UAMsgPtr uamp,Int4 BspStart,Int4 BspStop)
{
Uint1 i;
Int4  from_a,to_a,cumul=0,diff,tmp;
	
	if (uamp->currentPos>uamp->to_a) return(FALSE);
	for (i=0;i<3;i++){
		if (uamp->ubd[i].from!=(Int4)-1 && uamp->ubd[i].to!=(Int4)-1){
			if (uamp->ubd[i].IsGap){/*disp coord*/
				from_a=uamp->ubd[i].from;
				to_a=uamp->ubd[i].to;
			}
			else{/*switch bsp coord to disp coord*/
				from_a=cumul;
				to_a=uamp->ubd[i].to-uamp->ubd[i].from+cumul;
			}
			if (uamp->currentPos>=from_a && uamp->currentPos<=to_a){
				if (uamp->ubd[i].IsGap)
					uamp->from_r=uamp->currentPos;
				else
					uamp->from_r=uamp->ubd[i].from+uamp->currentPos-cumul;
			}
			else{
				cumul+=(to_a-from_a+1);
				continue;
			}
			if (uamp->to_a>=from_a && uamp->to_a<=to_a){
				uamp->to_r=_min_(uamp->to_a,to_a);
				if (!uamp->ubd[i].IsGap)
					uamp->to_r+=uamp->ubd[i].from-cumul;
			}
			else{
				uamp->to_r=uamp->ubd[i].to;
			}
			uamp->currentPos+=(uamp->to_r-uamp->from_r+1);
			uamp->IsGap=uamp->ubd[i].IsGap;
			break;
		}
	}

	/*if minus strand (nuc. only); reverse the values*/
	if (uamp->strand==Seq_strand_minus && !uamp->IsGap){
		diff=uamp->to_r-uamp->from_r;
		uamp->from_r=uamp->from_r+(BspStop-uamp->from_r)-(uamp->from_r-BspStart);
		uamp->to_r=uamp->from_r-diff;
		tmp=uamp->to_r;
		uamp->to_r=uamp->from_r;
		uamp->from_r=tmp;
	}
	
	return(TRUE);
}

/*******************************************************************************

  Function : DDV_BuildBspEntitiesTbl()
  
  Purpose : build a table of eID, iID of each Bioseq of a SeqAlign.
    
  Return value : a table of entities

*******************************************************************************/
NLM_EXTERN Uint4Ptr DDV_BuildBspEntitiesTbl(ValNodePtr PNTR TableHead,Int4 nBsp)
{
ValNodePtr vnp;
ParaGPtr   pgp;
BioseqPtr  bsp;
Uint4Ptr    entitiesTbl;
Uint2      bsp_eID,bsp_iID;
Int4       i;

	entitiesTbl=(Uint4Ptr)MemNew(nBsp*sizeof(Uint4));
	if (!entitiesTbl) return(NULL);
	
	for (i=0;i<nBsp;i++){
		vnp=TableHead[i];
		if (vnp){
			pgp=(ParaGPtr)vnp->data.ptrvalue;
			
			bsp=BioseqLockById(pgp->sip);
			if (bsp){
				bsp_eID=ObjMgrGetEntityIDForPointer((Pointer)bsp);
				bsp_iID = GetItemIDGivenPointer (bsp_eID, 
						OBJ_BIOSEQ, (Pointer) bsp);	
				entitiesTbl[i]=UDV_EncodeIdxFeat(bsp_eID,bsp_iID);
				BioseqUnlock(bsp);
			}
		}
	}
	return(entitiesTbl);
}

/*******************************************************************************

  Function : DDV_BuildTailDescriptor()
  
  Purpose : build a descriptor for the unaligned left/right tails of a SeqAlign
    
  Return value : the descriptor (see also TotLength; total size of the tail)

*******************************************************************************/
NLM_EXTERN ValNodePtr DDV_BuildTailDescriptor(SeqAlignPtr sap, Int4 nBsp, 
	Int2 LineSize,Uint1 which_tail,Int4Ptr TotLength,Int4Ptr cumulpop)
{
Int4            startcopy,stopcopy,pop,i,MaxLength,start,stop;
Boolean         bError;
ValNodePtr      vnp,vnp_head;
DescriDispPtr   ddp;
Uint1           strand;

	vnp_head=NULL;
	bError=FALSE;
	MaxLength=0;
	/*get the size of the left UA region*/
	for(i=0;i<nBsp;i++){
		if (AlnMgrGetNthRowTail(sap,i+1,which_tail,&start,&stop,&strand)){
			if (start!=(Int4)-1 && stop!=(Int4)-1){
				MaxLength=MAX(MaxLength,ABS(stop-start)+1);
			}
		}
	}
	/*build a descriptor node for that region, if needed*/
	if (MaxLength>0){
		startcopy=0;
		stopcopy=MaxLength;
		*TotLength+=(stopcopy-startcopy);/*to be used for the display coord. system*/

		while(startcopy<stopcopy){
			/*store data*/
			pop=_min_(stopcopy-startcopy,LineSize-(*cumulpop));
			ddp=(DescriDispPtr)MemNew(sizeof(DescriDisp));
			if (ddp==NULL) {
				bError=TRUE;break;
			}
			ddp->from=startcopy;/*use relative coord, i.e. UA coord system*/
			ddp->to=ddp->from+pop-1;
			ddp->TextStyle=MSA_TXT_STYLE_REG_UNALIGN;
			if (which_tail==LEFT_TAIL)
				ddp->UAnum=(Int4)-1;/*UA region number; -1 means LEFT TAIL*/
			else
				ddp->UAnum=(Int4)-2;/*UA region number; -2 means RIGHT TAIL*/
			ddp->UAMaxlength=stopcopy;
			/*create a new node*/
			if (vnp_head==NULL) {
				vnp_head=ValNodeAddPointer(NULL,0,(Pointer)ddp);
				vnp=vnp_head;
			}
			else vnp=ValNodeAddPointer(&vnp,0,(Pointer)ddp);

			if (!vnp){
				bError=TRUE;
				break;
			}
			(*cumulpop)+=pop;
			if ((*cumulpop)==LineSize) (*cumulpop)=0;
			startcopy+=pop;
		}
	}

	if (bError){
		vnp_head=ValNodeFreeData(vnp_head);
	}
	
	return(vnp_head);
}

NLM_EXTERN ValNodePtr UABuildDescriptor(SeqAlignPtr sap, Int4 nBsp, Int2 LineSize,
	DDV_Disp_OptPtr ddop, Int4Ptr TotLength,Boolean AddLeftUAPart,
  Boolean AddRightUAPart) {
/*******************************************************************************
*  same as UABuildDescriptor2, but by default no extra gap char.
*******************************************************************************/
  return(UABuildDescriptor2(sap, nBsp, LineSize, ddop, TotLength,
         AddLeftUAPart, AddRightUAPart, FALSE));
}


/*******************************************************************************

  Function : UABuildDescriptor2()
  
  Purpose : build the descriptor of a SeqAlign. This function is ONLY designed 
            for discontinuous SeqAlign. The descriptor is a valnodelist. For
			each node, the field data.ptrvalue is of type MsaTxtDispPtr. It
			contains inportant info : start-stop of either an aligned or unaligned
			region. In the first case, SeqAlign coord system is used, otherwise
			UA coord system is used (UA=UnAligned). The descriptor should then
			be used as a guide to populate ParaG for the display. Note that this
			descriptor is a "pre"-layout of the ParaG structure : some descriptor's
			nodes put together = a ParaG in size.
    
  Return value : the descriptor (see also TotLength; total size of the SeqAlign,
	  display coordinates)

*******************************************************************************/
NLM_EXTERN ValNodePtr UABuildDescriptor2(SeqAlignPtr sap, Int4 nBsp, Int2 LineSize,
	DDV_Disp_OptPtr ddop, Int4Ptr TotLength,Boolean AddLeftUAPart,
	Boolean AddRightUAPart, Boolean WantExtraGapChar)
{
Int4            length,r,cumulpop,StartLetter,startcopy,stopcopy,pop,UAnum;
Boolean         bUnAligned,bError;
ValNodePtr      vnp,vnp_head,vnp2;
DescriDispPtr   ddp;


	r=0;
	cumulpop=0;
	StartLetter=0;/*align coord*/
	bError=FALSE;
	vnp_head=NULL;
	*TotLength=0;

	/*build UnAligned left part, if needed*/
	if (AddLeftUAPart && ddop->DispDiscStyle!=MSA_TXT_STYLE_1){
		vnp_head=DDV_BuildTailDescriptor(sap,nBsp,LineSize,LEFT_TAIL,TotLength,
			&cumulpop);
		/*get the last node; will be used in the SAP loop (see below)*/
		if (vnp_head){
			vnp=vnp_head;
			while(vnp->next){
				vnp=vnp->next;
			}
		}
	}

	/*loop through each aligned and unaligned regions to build
	a ValNode list of descriptors. These descriptors will be used to
	generate the ParaG list*/
	UAnum=0;
	while(AlnMgrGetNextLengthBit(sap,&length,&r)){

		startcopy=0;

		if (length<=0){
			bUnAligned=TRUE;
			switch(ddop->DispDiscStyle){/*user's display choice*/
				case MSA_TXT_STYLE_1:/*put a spacer between 2 align blocks*/
					stopcopy=ddop->SpacerSize;
					break;
				case MSA_TXT_STYLE_2:/*put sequence between 2 align blocks*/
					stopcopy=ABS(length);
          /* give a small space no matter what */
          /* took out small spaces SW 1/19/01 */
          /* put it back in optionally, DIH, 1/25/01 */
          if (WantExtraGapChar) {
            if (stopcopy==0) stopcopy=1; 
          }
					break;
			}
			UAnum++;
		}
		else if (length>0) {
			bUnAligned=FALSE;
			stopcopy=length;
		} 

		*TotLength+=(stopcopy-startcopy);/*to be used for the display coord. system*/
		
		while(startcopy<stopcopy){
			/*store data*/
			pop=_min_(stopcopy-startcopy,LineSize-cumulpop);
			ddp=(DescriDispPtr)MemNew(sizeof(DescriDisp));
			if (ddp==NULL) {
				bError=TRUE;break;
			}
			if (bUnAligned==FALSE){
				ddp->from=StartLetter;/*use align coord*/
				ddp->to=ddp->from+pop-1;
				ddp->TextStyle=MSA_TXT_STYLE_REG_ALIGN;
				StartLetter+=pop;
			}
			else{
				ddp->from=startcopy;/*use relative coord, i.e. UA coord system*/
				ddp->to=ddp->from+pop-1;
				ddp->TextStyle=MSA_TXT_STYLE_REG_UNALIGN;
				ddp->UAnum=UAnum;/*UA region number*/
				ddp->UAMaxlength=stopcopy;
			}			
			/*create a new node*/
			if (vnp_head==NULL) {
				vnp_head=ValNodeAddPointer(NULL,0,(Pointer)ddp);
				vnp=vnp_head;
			}
			else vnp=ValNodeAddPointer(&vnp,0,(Pointer)ddp);

			if (!vnp){
				bError=TRUE;
				break;
			}
			cumulpop+=pop;
			if (cumulpop==LineSize) cumulpop=0;
			startcopy+=pop;
		}
		if (bError) break;
	}

	if (bError){
		vnp_head=ValNodeFreeData(vnp_head);
		goto erreur;
	}

	/*build UnAligned right part, if needed*/
	if (AddRightUAPart && ddop->DispDiscStyle!=MSA_TXT_STYLE_1){
		vnp2=DDV_BuildTailDescriptor(sap,nBsp,LineSize,RIGHT_TAIL,TotLength,
			&cumulpop);
		/*add vnp2 at the end of vnp_head*/
		if (vnp2) {
			vnp=vnp_head;
			while(vnp->next){
				vnp=vnp->next;
			}
			vnp->next=vnp2;
		}
	}

/*debug only : print the content of the descriptor

	vnp=vnp_head;
	while(vnp){
		ddp=(DescriDispPtr)vnp->data.ptrvalue;
		printf("[%4i..%4i],%2i (%4i) ; ",ddp->from,ddp->to,ddp->TextStyle,
			ddp->to-ddp->from+1);
		if (ddp->TextStyle==MSA_TXT_STYLE_REG_UNALIGN){
			printf("MaxLength = %i ; UAnum = %i\n",ddp->UAMaxlength,ddp->UAnum);
		}
		else{
			printf("\n");
		}
		vnp=vnp->next;
	}

debug only - end*/

erreur :
	return(vnp_head);
}

/*******************************************************************************

  Function : UABuildDescriptorForBlockEditor()
  
  Purpose : build the descriptor of a SeqAlign for the block editor; i.e. the 
  		descriptor contains the information about one aligned block along with
		its two adjacent unaligned regions. See also the 'purpose' fo the function 
		UABuildDescriptor() above.
    
  Return value : the descriptor (see also TotLength; total size of the SeqAlign,
	  display coordinates)

*******************************************************************************/
NLM_EXTERN ValNodePtr UABuildDescriptorForBlockEditor(SeqAlignPtr sap, Int4 nBsp, 
	Int4 block_num,Int4 LineSize,Int4Ptr TotLength)
{
Int4            length,r,cumulpop,StartLetter,startcopy,stopcopy,pop,UAnum,nBlock,
				blockNum;
Boolean         bUnAligned,bError,bPopNow;
ValNodePtr      vnp,vnp_head,vnp2;
DescriDispPtr   ddp;


	/*test if we have a Discontinuous SeqAlign*/
	if (!AlnMgrIsSAPDiscAli(sap)) return(NULL);
	
	/*get the number of block(s)*/
	nBlock=AlnMgrGetNumAlnBlocks(sap);
	if (block_num<0 || block_num>nBlock-1){
		return(NULL);
	}
	r=cumulpop=UAnum=blockNum=0;
	StartLetter=0;/*align coord*/
	bError=FALSE;
	vnp_head=NULL;
	*TotLength=0;
	/*if the user wants to edit the first block (block_num==0), I have
	to use a special function to create the left-side part of the 
	descriptor*/
	if (block_num==0){
		vnp_head=DDV_BuildTailDescriptor(sap,nBsp,LIMIT_EDITOR_SIZE,
			LEFT_TAIL,TotLength,&cumulpop);
		if (vnp_head){
			vnp=vnp_head;
			while(vnp->next){
				vnp=vnp->next;
			}
		}
	}

	while(AlnMgrGetNextLengthBit(sap,&length,&r)){

		bPopNow=FALSE;
		startcopy=0;

		/*unaligned region ?*/
		if (length<=0){
			/*region adjacent to the block of interest ?*/
			if (UAnum==block_num-1 || UAnum==block_num){
				bUnAligned=TRUE;
				stopcopy=ABS(length);
				bPopNow=TRUE;
			}
			UAnum++;
		}
		else{/*aligned region*/
			if (blockNum==block_num){
				bUnAligned=FALSE;
				stopcopy=length;
				bPopNow=TRUE;
			}
			else{
				StartLetter+=length;
			}
			blockNum++;
		}
		
		if (!bPopNow) continue;
		
		*TotLength+=(stopcopy-startcopy);/*to be used for the display coord. system*/
		
		while(startcopy<stopcopy){
			/*store data*/
			pop=_min_(stopcopy-startcopy,LineSize-cumulpop);
			ddp=(DescriDispPtr)MemNew(sizeof(DescriDisp));
			if (ddp==NULL) {
				bError=TRUE;break;
			}
			if (bUnAligned==FALSE){
				ddp->from=StartLetter;/*use align coord*/
				ddp->to=ddp->from+pop-1;
				ddp->TextStyle=MSA_TXT_STYLE_REG_ALIGN;
				StartLetter+=pop;
			}
			else{
				ddp->from=startcopy;/*use relative coord, i.e. UA coord system*/
				ddp->to=ddp->from+pop-1;
				ddp->TextStyle=MSA_TXT_STYLE_REG_UNALIGN;
				ddp->UAnum=UAnum;/*UA region number*/
				ddp->UAMaxlength=stopcopy;
			}			
			/*create a new node*/
			if (vnp_head==NULL) {
				vnp_head=ValNodeAddPointer(NULL,0,(Pointer)ddp);
				vnp=vnp_head;
			}
			else vnp=ValNodeAddPointer(&vnp,0,(Pointer)ddp);

			if (!vnp){
				bError=TRUE;
				break;
			}
			cumulpop+=pop;
			if (cumulpop==LineSize) cumulpop=0;
			startcopy+=pop;
		}
		if (bError) break;
	}

	/*oups...*/
	if (bError){
		vnp_head=ValNodeFreeData(vnp_head);
		goto erreur;
	}

	
	/*if the user wants to edit the last block (block_num==nBlock-1), I have
	to use a special function to create the right-side part of the 
	descriptor*/
	if (block_num==nBlock-1){
		vnp2=DDV_BuildTailDescriptor(sap,nBsp,LIMIT_EDITOR_SIZE,
			RIGHT_TAIL,TotLength,&cumulpop);
		if (vnp2) {
			vnp=vnp_head;
			while(vnp->next){
				vnp=vnp->next;
			}
			vnp->next=vnp2;
		}
	}

erreur :
	return(vnp_head);
}

/*******************************************************************************

  Function : DDV_DumpSAPInAFile()
  
  Purpose : write an Indexed SeqALign in a file (use the code of the Population
      Viewer).
  
  Return value : -

*******************************************************************************/
NLM_EXTERN void DDV_DumpSAPInAFile(MsaParaGPopListPtr mpplp,DDVOptionsBlockPtr dobp, 
		FILE *hFile,Uint4 option,DDV_ColorGlobal * gclr)
{
ByteStorePtr    PNTR byteSpp;
Int4Ptr PNTR    matrix  ;
Int2            LineSize;

	if (!mpplp || !hFile) return;
	
	/*get the optional data*/
	if (dobp){
		LineSize=dobp->LineSize;
		byteSpp=dobp->byteSpp;
		matrix=dobp->matrix;
	}
	else{
		LineSize=ParaG_Size;
		byteSpp=NULL;
	}

	/*do the display */
	DDV_AffichageParaG(mpplp,0,0,mpplp->LengthAli-1,mpplp->LengthAli,0,option,
		LineSize,hFile,byteSpp,matrix,gclr,NULL);

}

/*******************************************************************************

  Function : DDV_DumpSAPInFastaFile()
  
  Purpose : write an Indexed SeqALign in a file (use FASTA format only).
  
  Return value : -

*******************************************************************************/
NLM_EXTERN void DDV_DumpSAPInFastaFile(MsaParaGPopListPtr mpplp,DDVOptionsBlockPtr dobp, 
		FILE *fp,Boolean bPrintGap)
{
CharPtr    szBuf,szSeq;
ParaGPtr   pgp;
SeqIdPtr   sip;
ValNodePtr vnp;
BioseqPtr  bsp;
Int4       i,j,n,LineSize,bspLength;
Boolean    bFirst,IsAA;
Char       letter;
Char       DefLine[255];

	if (!mpplp || !fp) return;

	if (dobp){
		LineSize=dobp->LineSize;
	}
	else{
		LineSize=ParaG_Size;
	}
	
	szBuf=(CharPtr)MemNew((LineSize+1)*sizeof(Char));
	if (!szBuf) return;/*todo : error msg for the user*/
	
	for(i=0;i<mpplp->nBsp;i++){
		vnp=mpplp->TableHead[i];
		n=0;
		bFirst=TRUE;
		while(vnp){
			pgp=(ParaGPtr)vnp->data.ptrvalue;
			sip=SeqIdFindBest(pgp->sip,0);
			if (sip==NULL) sip=pgp->sip;
			bsp=BioseqLockById(sip);
			if (!bsp) return;/*todo : error msg for the user*/
			bspLength=BioseqGetLen(bsp);
			IsAA=ISA_aa(bsp->mol);
			if (bFirst){
				FastaId(bsp,DefLine,254);
				fprintf(fp,">%s ",DefLine);
				CreateDefLine(NULL, bsp, DefLine, 254, 0,NULL, NULL);
				fprintf(fp,"%s\n",DefLine);
				bFirst=FALSE;
			}
			BioseqUnlock(bsp);
			szSeq=(CharPtr)MemNew((pgp->StopLetter-pgp->StartLetter+2)*sizeof(Char));
			if (!szSeq) return;/*todo : error msg for the user*/
			DDV_GetSequenceFromParaG(pgp,&szSeq,bspLength,IsAA,NULL,NULL,NULL);
			j=0;
			while(szSeq[j]){
				letter=szSeq[j++];
				if (isalpha(letter)){
					szBuf[n++]=letter;
				}
				else{
					if (bPrintGap)
						szBuf[n++]=letter;
				}
				if (n==LineSize){
					szBuf[n]='\0';
					fprintf(fp,"%s\n",szBuf);
					n=0;
				}
			}
			MemFree(szSeq);
			vnp=vnp->next;
			if (vnp==NULL && n!=0){
				szBuf[n]='\0';
				fprintf(fp,"%s\n",szBuf);
			}
		}
	}
	MemFree(szBuf);
}

/*******************************************************************************

  Function : DDV_InitDefSAPdispStyles_BLAST()
  
  Purpose : set the default styles for SeqAligns.
  
  Return value : -

*******************************************************************************/
static void DDV_InitDefSAPdispStyles_BLAST(DDV_Disp_OptPtr ddop)
{
	/*use colors*/
	ddop->bUseColors=FALSE;
	/*disc SAP styles*/
	ddop->ShowLeftTail=FALSE;
	ddop->ShowRightTail=FALSE;
	ddop->DispDiscStyle=MSA_TXT_STYLE_2;
	ddop->SpacerSize=SPACER_TXT_BLANK;
        ddop->DiscJustification=DISP_JUST_LEFT;

	ddop->UAGapStyle=MSA_TXT_STYLE_UAGAP;
	ddop->AGapStyle=MSA_TXT_STYLE_GAP;

	/*ruler style*/
	ddop->RulerStyle=RulerStyle_Continue_Start;

}


/*******************************************************************************

  Function : DDV_DisplayNewBLAST()
  
  Purpose : build a default SeqAlign display for BLAST.
  
  Parameters : 	sap; SeqAlign
				disp_format; display formats (see pgppop.h)
				disp_data; for future implementation
				fp; to send the output

  NOTE : this function uses the alignmgr.[ch] instead of blocks.[ch]
  
  Return value : FALSE if failed 

*******************************************************************************/
static Boolean DDV_DisplayNewBLAST(SeqAlignPtr sap, ValNodePtr mask,
		Int4Ptr PNTR matrix,Uint4 disp_format,Pointer disp_data,FILE *fp)
{
MsaParaGPopList mppl;/*contains the data for the display*/
Int2            LineSize;
DDVOptionsBlockPtr dobp;
ByteStorePtr   PNTR byteSpp;
DDV_ColorGlobal *layout;
DDV_Disp_Opt   ddo;
Boolean        bUseLayout;
Uint1          what;

	MemSet(&mppl,0,sizeof(MsaParaGPopList));
	MemSet(&ddo,0,sizeof(DDV_Disp_Opt));
	layout=NULL;

	/*get the optional data*/
	if (disp_data){
		dobp=(DDVOptionsBlockPtr)disp_data;
		LineSize=dobp->LineSize;
		byteSpp=dobp->byteSpp;
		bUseLayout=dobp->bUseLayout;
		what=dobp->LayoutType;
	}
	else{
		LineSize=ParaG_Size;
		byteSpp=NULL;
		bUseLayout=FALSE;
	}

	DDV_InitDefSAPdispStyles_BLAST(&ddo);
	
	/*build the indexed SeqAlign and the ParaG structure*/
	DDV_CreateDisplayFromIndex(sap, &mppl, LineSize,&ddo);
	
	/*create the layout*/
	if (bUseLayout){
		layout = DDV_CreateColorGlobal(TRUE, (void *)sap);
		if (layout){
			if (dobp->LayoutType&DDV_USE_STDCLR){
				DDV_InitColour_When_Start(sap,&mppl,&layout, FALSE);
			}
			if (dobp->LayoutType&DDV_USE_UALAYOUT || dobp->LayoutType&DDV_USE_NOMLAYOUT){
				DDV_LayoutUAregion(sap,&mppl,&layout,
					dobp->normlayout.rgb,dobp->normlayout.style,
					dobp->UAlayout.rgb,dobp->UAlayout.style);
			}
			if (dobp->LayoutType&DDV_USE_ISOLAYOUT){
				DDV_LayoutISOColors(layout,mppl.TableHead,mppl.nBsp,
					0,TRUE,matrix,dobp->ISOlayout.clr_ident,
					dobp->ISOlayout.clr_simil,dobp->ISOlayout.clr_other,
					dobp->ISOlayout.sty_ident,
					dobp->ISOlayout.sty_simil,dobp->ISOlayout.sty_other);
			}
			if (dobp->LayoutType&DDV_USE_GAPLAYOUT){
				DDV_LayoutGap(layout,mppl.TableHead[0],mppl.TableHead[1],1,
					dobp->GAPlayout.style,dobp->GAPlayout.rgb);
				DDV_LayoutGap(layout,mppl.TableHead[1],mppl.TableHead[0],0,
					dobp->GAPlayout.style,dobp->GAPlayout.rgb);
			}
			if (dobp->LayoutType&DDV_USE_MASKLAYOUT){
				DDV_LayoutMaskRegions(layout,mppl.TableHead[0],mask,0,
					dobp->MASKlayout.style,dobp->MASKlayout.rgb);
			}
		}
	}
	
	/*do the display */
	DDV_AffichageParaG(&mppl,0,0,mppl.LengthAli-1,mppl.LengthAli,0,disp_format,
		LineSize,fp,byteSpp,matrix,layout,mask);

	/*done... delete data*/
	DDV_DeleteDisplayList(&mppl);
	if (layout) DDV_DeleteColorGlobal(layout);

	
	return(TRUE);
}

/*******************************************************************************

  Function : DDV_BlastGetColorSchemaZero()
  
  Purpose : BLAST color sheme 0 : standard b&w BLAST output
  
*******************************************************************************/
static void DDV_BlastGetColorSchemaZero(DDVOptionsBlockPtr dobp)
{
	memset(dobp,0,sizeof(DDVOptionsBlock));
	
	dobp->bUseLayout=FALSE;
}

/*******************************************************************************

  Function : DDV_BlastGetColorSchemaOne()
  
  Purpose : BLAST color sheme 1 :
              - masked regions in lowe case,
			  - everything else in upper case.
  
*******************************************************************************/
static void DDV_BlastGetColorSchemaOne(DDVOptionsBlockPtr dobp)
{
    memset(dobp,0,sizeof(DDVOptionsBlock));
    
    dobp->bUseLayout=TRUE;
    
    /* dobp->LayoutType=DDV_USE_MASKLAYOUT|DDV_USE_NOMLAYOUT; */
    
    dobp->LayoutType=DDV_USE_UALAYOUT|DDV_USE_GAPLAYOUT;

    dobp->UAlayout.style=DDV_TXTSTYLE_LOWERCASE;
    dobp->GAPlayout.style=DDV_TXTSTYLE_LOWERCASE;
    
    /* dobp->MASKlayout.style=DDV_TXTSTYLE_LOWERCASE; */
}

/*******************************************************************************

  Function : DDV_BlastGetColorSchemaTwo()
  
  Purpose : BLAST color sheme 2 :
              - masked regions in lower case, gray letters,
			  - UnAligned regions in italic,
			  - everything else in upper case.
  
*******************************************************************************/
static void DDV_BlastGetColorSchemaTwo(DDVOptionsBlockPtr dobp)
{
	memset(dobp,0,sizeof(DDVOptionsBlock));
	
	dobp->bUseLayout=TRUE;

	dobp->LayoutType=DDV_USE_MASKLAYOUT|DDV_USE_NOMLAYOUT;
	dobp->LayoutType|=DDV_USE_UALAYOUT;

	dobp->UAlayout.style=DDV_TXTSTYLE_ITALIC|DDV_TXTSTYLE_LOWERCASE;
	dobp->normlayout.style=DDV_TXTSTYLE_BOLD;

	dobp->MASKlayout.style=DDV_TXTSTYLE_LOWERCASE|DDV_TXTSTYLE_COLOR;
	dobp->MASKlayout.rgb[0]=102;
	dobp->MASKlayout.rgb[1]=102;
	dobp->MASKlayout.rgb[2]=102;
}

/*******************************************************************************

  Function : DDV_BlastGetColorSchemaThree()
  
  Purpose : BLAST color sheme 3 :
              - masked regions in lower case, gray letters,
			  - UnAligned regions in italic,
			  - Identity/Similarity colors
			  - everything else in upper case.
  
*******************************************************************************/
static void DDV_BlastGetColorSchemaThree(DDVOptionsBlockPtr dobp)
{
	memset(dobp,0,sizeof(DDVOptionsBlock));
	
	dobp->bUseLayout=TRUE;

	dobp->LayoutType=DDV_USE_MASKLAYOUT;
	dobp->LayoutType|=DDV_USE_UALAYOUT;
	dobp->LayoutType|=DDV_USE_ISOLAYOUT;

	dobp->UAlayout.style=DDV_TXTSTYLE_ITALIC|DDV_TXTSTYLE_LOWERCASE;

	dobp->MASKlayout.style=DDV_TXTSTYLE_LOWERCASE;/*|DDV_TXTSTYLE_COLOR;
	dobp->MASKlayout.rgb[0]=102;
	dobp->MASKlayout.rgb[1]=102;
	dobp->MASKlayout.rgb[2]=102;
*/
	dobp->ISOlayout.clr_ident=DDVCOL_ORANGE;
	dobp->ISOlayout.clr_simil=DDVCOL_BLUE;
	dobp->ISOlayout.clr_other=DDVCOL_BLACK;
}

/*******************************************************************************

  Function : DDV_BlastGetColorSchemaFour()
  
  Purpose : BLAST color sheme 4 = BLAST color schema 3 but with different colors
  
*******************************************************************************/
static void DDV_BlastGetColorSchemaFour(DDVOptionsBlockPtr dobp)
{
	memset(dobp,0,sizeof(DDVOptionsBlock));
	
	dobp->bUseLayout=TRUE;

	dobp->LayoutType=DDV_USE_MASKLAYOUT;
	dobp->LayoutType|=DDV_USE_UALAYOUT;
	dobp->LayoutType|=DDV_USE_ISOLAYOUT;

	dobp->UAlayout.style=DDV_TXTSTYLE_ITALIC|DDV_TXTSTYLE_LOWERCASE;

	dobp->MASKlayout.style=DDV_TXTSTYLE_LOWERCASE|DDV_TXTSTYLE_COLOR;
	dobp->MASKlayout.rgb[0]=102;
	dobp->MASKlayout.rgb[1]=102;
	dobp->MASKlayout.rgb[2]=102;

	dobp->ISOlayout.clr_ident=DDVCOL_BLUE;
	dobp->ISOlayout.clr_simil=DDVCOL_BROWN;
	dobp->ISOlayout.clr_other=DDVCOL_RED;
}

/*******************************************************************************

  Function : DDV_BlastGetColorSchemaFive()
  
  Purpose : BLAST color sheme 5 = BLAST color schema 3 but with different colors
  
*******************************************************************************/
static void DDV_BlastGetColorSchemaFive(DDVOptionsBlockPtr dobp)
{
	memset(dobp,0,sizeof(DDVOptionsBlock));
	
	dobp->bUseLayout=TRUE;

	dobp->LayoutType=DDV_USE_MASKLAYOUT;
	dobp->LayoutType|=DDV_USE_UALAYOUT;
	dobp->LayoutType|=DDV_USE_ISOLAYOUT;

	dobp->UAlayout.style=DDV_TXTSTYLE_ITALIC|DDV_TXTSTYLE_LOWERCASE;

	dobp->MASKlayout.style=DDV_TXTSTYLE_LOWERCASE|DDV_TXTSTYLE_COLOR;
	dobp->MASKlayout.rgb[0]=102;
	dobp->MASKlayout.rgb[1]=102;
	dobp->MASKlayout.rgb[2]=102;

	dobp->ISOlayout.clr_ident=DDVCOL_RED;
	dobp->ISOlayout.clr_simil=DDVCOL_BLUE;
	dobp->ISOlayout.clr_other=DDVCOL_BLACK;
}

/*******************************************************************************

  Function : DDV_BlastGetColorSchemaSix()
  
  Purpose : BLAST color sheme 6 = BLAST color schema 3 but with different colors
  
*******************************************************************************/
static void DDV_BlastGetColorSchemaSix(DDVOptionsBlockPtr dobp)
{
	memset(dobp,0,sizeof(DDVOptionsBlock));
	
	dobp->bUseLayout=TRUE;

	dobp->LayoutType=DDV_USE_MASKLAYOUT;
	dobp->LayoutType|=DDV_USE_UALAYOUT;
	dobp->LayoutType|=DDV_USE_ISOLAYOUT;

	dobp->UAlayout.style=DDV_TXTSTYLE_ITALIC|DDV_TXTSTYLE_LOWERCASE;

	dobp->MASKlayout.style=DDV_TXTSTYLE_LOWERCASE;/*|DDV_TXTSTYLE_COLOR;
	dobp->MASKlayout.rgb[0]=102;
	dobp->MASKlayout.rgb[1]=102;
	dobp->MASKlayout.rgb[2]=102;
*/
	dobp->ISOlayout.clr_ident=DDVCOL_ORANGE;
	dobp->ISOlayout.clr_simil=DDVCOL_BLUE;
	dobp->ISOlayout.clr_other=DDVCOL_BLACK;

	dobp->ISOlayout.sty_ident=DDV_TXTSTYLE_BOLD;
}

/*******************************************************************************

  Function : DDV_DisplayBlastPairList()
  
  Purpose : function to display a BLAST output.
  
  Parameters : 	sap; seqalign
			    mask; list of masked regions in the query
				fp; output file;
				is_na; TRUE means nuc sequence
				tx_option; some display options
				ColorSchema; one of the available color schemes
				
  Return value : FALSE if failure

*******************************************************************************/
NLM_EXTERN Boolean DDV_DisplayBlastPairList(SeqAlignPtr sap, ValNodePtr mask,
	FILE *fp, Boolean is_na, Uint4 tx_option,Uint1 ColorSchema)
{
    DDVOptionsBlock dob;
    SeqAlignPtr     sap4;
    SeqIdPtr        new_id = NULL, old_id = NULL;    
    Int4Ptr PNTR    matrix;
    Uint4           option,i;
    Boolean         bRet, follower= FALSE;
    
    if (!sap || !fp) 
        return(FALSE);
    
    /*get the matrix*/
    if (is_na == FALSE){
        matrix=load_default_matrix();
        if (!matrix)
            return(FALSE);
    } else {
        matrix=NULL;
    }
    
    memset(&dob, '\0', sizeof(DDVOptionsBlock));
    
    /*init display format*/
    option =VIEW_FULLSEQ;

    if (ColorSchema==DDV_BLAST_COLOR0 || ColorSchema==DDV_BLAST_COLOR1)    
        option|=DISP_FULL_TXT;
    else
        option|=DISP_FULL_HTML;

    option|=DISP_BSP_COORD;
    option|=SEQNAME_BLAST_STD;
    option|=DISP_NOLINKONNAME;
    option|=DISP_BLAST_STD;
    option|=DISPE_COLOR;
    
    /*get the color scheme layout*/
    /*note that the following functions will set to 0 the entire dob structure, before
      filling in its content with the layout information. All subsequent init. of dob
      should be done after the following switch()*/
    switch(ColorSchema){
    case DDV_BLAST_COLOR0:
        option|=DISP_BLAST_MIDLINE;
        DDV_BlastGetColorSchemaZero(&dob);
        break;
    case DDV_BLAST_COLOR1:
        option|=DISP_BLAST_MIDLINE;
        DDV_BlastGetColorSchemaOne(&dob);
        break;
    case DDV_BLAST_COLOR2:
        option|=DISP_BLAST_MIDLINE;
        DDV_BlastGetColorSchemaTwo(&dob);
        break;
    case DDV_BLAST_COLOR3:
        DDV_BlastGetColorSchemaThree(&dob);
        break;
    case DDV_BLAST_COLOR4:
        DDV_BlastGetColorSchemaFour(&dob);
        break;
    case DDV_BLAST_COLOR5:
        DDV_BlastGetColorSchemaFive(&dob);
        break;
    case DDV_BLAST_COLOR6:
        DDV_BlastGetColorSchemaSix(&dob);
        break;
    }
    
    dob.LineSize=(Int2)60;
    dob.matrix=matrix;
    bRet=TRUE;
    sap4=sap;
    
    while(sap4) {
        /*build the Index*/
        if (sap4->segtype == SAS_DISC){
            if (!sap4 || !AlnMgrIndexSingleSeqAlign(sap4)){
                bRet=FALSE;
                break;
            }
        }
        else{
            if (!sap4 || !AlnMgrIndexSingleChildSeqAlign(sap4)){
                bRet=FALSE;
                break;
            }
        }
        
        if (option&DISP_FULL_HTML){
            fprintf(fp,"<pre>\n");
        }
        /* Attempt to print score for the alignment */
        new_id = TxGetSubjectIdFromSeqAlign(sap4);
        if(old_id != NULL) {
            if(SeqIdMatch(new_id, old_id))
                follower = TRUE;
        }
        
        /*separator*/
        if (option&DISP_FULL_HTML)
            fprintf(fp,"<HR WIDTH=\"400\">");
        
        old_id = new_id;
        if(!FormatScoreFromSeqAlign(sap4, tx_option, fp, matrix, follower)){
            bRet=FALSE;
            break;
        }
        follower = FALSE;
        if (option&DISP_FULL_HTML){
            fprintf(fp,"</pre>\n");
        }
        
        /*display a SeqAlign*/
        if (!DDV_DisplayNewBLAST(sap4, mask, matrix, option, (Pointer) &dob, fp)){
            bRet=FALSE;
            break;
        }
        sap4 = sap4->next;
    }
    
    if (matrix){
        for(i = 0; i<TX_MATRIX_SIZE; ++i)
            MemFree(matrix[i]);
        MemFree(matrix);
    } 
    
    return(bRet);
	
}

/*******************************************************************************

  Function : DDV_DisplayBlastSAP()
  
  Purpose : test function to display a BLAST output.
  
  Parameters : 	sap; seqalign
			    mask; list of masked regions in the query
				fo; output file;
				is_na; TRUE means nuc sequence
				tx_option; some display options
				
  Return value : -

*******************************************************************************/
static Boolean DDV_DisplayBlastSAP(SeqAlignPtr sap, ValNodePtr mask,
	FILE *fp, Boolean is_na, Uint4 tx_option)
{
DDVOptionsBlock dob;
SeqAlignPtr     sap4;
SeqIdPtr        new_id = NULL, old_id = NULL;    
Int4Ptr PNTR    matrix;
Uint4           option,i;
Boolean         bRet, follower= FALSE;

    if (!sap || !fp) 
        return(FALSE);
    
	/*get the matrix*/
    if (is_na == FALSE){
        matrix=load_default_matrix();
        if (!matrix)
            return(FALSE);
    } else {
        matrix=NULL;
    }
    
    /*init display format*/
    option =VIEW_FULLSEQ;
    option|=DISP_FULL_HTML;
    option|=DISP_BSP_COORD;
    option|=SEQNAME_BLAST_STD;
    option|=DISP_BLAST_STD;
	option|=DISP_BLAST_MIDLINE;
    option|=DISPE_COLOR;
    dob.LineSize=(Int2)60;
    dob.matrix=matrix;
	
    bRet=TRUE;
    sap4=sap;
    while(sap4) {
        /*build the Index*/
        if (sap4->segtype == SAS_DISC){
            if (!sap4 || !AlnMgrIndexSingleSeqAlign(sap4)){
                bRet=FALSE;
                break;
            }
            dob.bUseLayout=TRUE;
        }
        else{
            if (!sap4 || !AlnMgrIndexSingleChildSeqAlign(sap4)){
                bRet=FALSE;
                break;
            }
            dob.bUseLayout=TRUE;
        }
		
		/*set the layout type, if needed*/
		if (dob.bUseLayout){
			/*basic layout*/
			dob.LayoutType=DDV_USE_UALAYOUT;
			dob.LayoutType|=DDV_USE_GAPLAYOUT;
			dob.LayoutType|=DDV_USE_MASKLAYOUT;
			/*dob.LayoutType|=DDV_USE_ISOLAYOUT;*/
			/*ext. layout*/
			dob.UAlayout.style=DDV_TXTSTYLE_LOWERCASE;
			dob.UAlayout.style|=DDV_TXTSTYLE_ITALIC;
			dob.UAlayout.style|=DDV_TXTSTYLE_COLOR;
			dob.UAlayout.rgb[0]=255;
			dob.UAlayout.rgb[1]=0;
			dob.UAlayout.rgb[2]=0;
			dob.GAPlayout.style=DDV_TXTSTYLE_LOWERCASE;
			dob.GAPlayout.style|=DDV_TXTSTYLE_ITALIC;
			dob.GAPlayout.style|=DDV_TXTSTYLE_COLOR;
			dob.GAPlayout.rgb[0]=255;
			dob.GAPlayout.rgb[1]=0;
			dob.GAPlayout.rgb[2]=0;
			dob.MASKlayout.style=DDV_TXTSTYLE_BOLD;
			dob.MASKlayout.style|=DDV_TXTSTYLE_UNDERLINE;
			dob.ISOlayout.clr_ident=DDVCOL_ORANGE;
			dob.ISOlayout.clr_simil=DDVCOL_SKY;
			dob.ISOlayout.clr_other=DDVCOL_BLACK;
		}
					
        if (option&DISP_FULL_HTML){
			fprintf(fp,"<pre>\n");
		}
        /* Attempt to print score for the alignment */
        new_id = TxGetSubjectIdFromSeqAlign(sap4);
        if(old_id != NULL) {
            if(SeqIdMatch(new_id, old_id))
                follower = TRUE;
        }
		
		/*separator*/
		fprintf(fp,"<HR WIDTH=\"400\">");
        old_id = new_id;
        if(!FormatScoreFromSeqAlign(sap4, tx_option, fp, matrix, follower)){
            bRet=FALSE;
            break;
        }
        follower = FALSE;
        if (option&DISP_FULL_HTML){
			fprintf(fp,"</pre>\n");
		}

        /*display a SeqAlign*/
		if (!DDV_DisplayNewBLAST(sap4, mask, matrix, option, (Pointer) &dob, fp)){
            bRet=FALSE;
            break;
        }
        sap4 = sap4->next;
    }
    
    if (matrix){
        for(i = 0; i<TX_MATRIX_SIZE; ++i)
            MemFree(matrix[i]);
        MemFree(matrix);
    } 
    
    return(bRet);
}
/*******************************************************************************

  Function : DDV_InitDefSAPdispStyles()
  
  Purpose : set the default styles for SeqAligns.
  
  Return value : -

*******************************************************************************/
NLM_EXTERN void DDV_InitDefSAPdispStyles(DDV_Disp_OptPtr ddop)
{
	/*use colors*/
	ddop->bUseColors=TRUE;
	/*disc SAP styles*/
	ddop->ShowLeftTail=FALSE;
	ddop->ShowRightTail=FALSE;
	ddop->DispDiscStyle=MSA_TXT_STYLE_1;
	ddop->SpacerSize=SPACER_TXT_BLANK;
    ddop->DiscJustification=DISP_JUST_CENTER;
	ddop->UAGapStyle=MSA_TXT_STYLE_UAGAP;
	ddop->AGapStyle=MSA_TXT_STYLE_GAP;

	/*ruler style*/
	ddop->RulerStyle=RulerStyle_Continue_Start;
}

/*******************************************************************************

  Function : DDV_InitCn3DSAPdispStyles()
  
  Purpose : set the styles for Cn3D SeqAligns.
  
  Return value : -

*******************************************************************************/
NLM_EXTERN void DDV_InitCn3DSAPdispStyles(DDV_Disp_OptPtr ddop)
{
	/*use colors*/
	ddop->bUseColors=TRUE;
	/*disc SAP styles*/
	ddop->ShowLeftTail=TRUE;
	ddop->ShowRightTail=TRUE;
	ddop->DispDiscStyle=MSA_TXT_STYLE_2;
	ddop->SpacerSize=0;
    ddop->DiscJustification=DISP_JUST_SPLIT;
    ddop->UAGapStyle=MSA_TXT_STYLE_UAGAP; /* use different unaligned gap chars ('~') */
	ddop->AGapStyle=MSA_TXT_STYLE_GAP;

	/*ruler style*/
	ddop->RulerStyle=RulerStyle_Continue_Start;
}

/*******************************************************************************

  Function : DDV_InitDDESAPdispStyles()
  
  Purpose : set the styles for DDE (i.e. DDV used as an editor).
  
  Return value : -

*******************************************************************************/
NLM_EXTERN void DDV_InitDDESAPdispStyles(DDV_Disp_OptPtr ddop)
{
	/*use colors*/
	ddop->bUseColors=TRUE;
	/*disc SAP styles*/
	ddop->ShowLeftTail=TRUE;
	ddop->ShowRightTail=TRUE;
	ddop->DispDiscStyle=MSA_TXT_STYLE_2;
	ddop->SpacerSize=0;
    ddop->DiscJustification=DISP_JUST_LEFT;
	ddop->UAGapStyle=MSA_TXT_STYLE_UAGAP;
	ddop->AGapStyle=MSA_TXT_STYLE_GAP;

	/*ruler style*/
	ddop->RulerStyle=RulerStyle_Continue_Start;
}

/*******************************************************************************

  Function : DDV_ReadSeqBin()
  
  Purpose : read a sequence using Seq_code_ncbistdaa or Seq_code_ncbi4na
    
  Return value : the sequence 

*******************************************************************************/
NLM_EXTERN Uint1Ptr DDV_ReadSeqBin (SeqIdPtr sip, Int4 from, Int4 to, 
                                    Boolean IsProt,Int2 len,Uint1 strand)
{
SeqLocPtr  slp;
SeqPortPtr spp;
Uint1Ptr    btr;
Uint1	   residue;
Uint2	   i=0;

	/*from always < than to*/
	slp = SeqLocIntNew (from, to, strand, sip);
	if (!slp) return(NULL);
	spp = SeqPortNewByLoc (slp, (Uint1)(IsProt==TRUE ? Seq_code_ncbistdaa 
			: Seq_code_ncbi4na));
	if (spp != NULL) {
		btr = (Uint1Ptr) MemNew (len * sizeof(Uint1));
		if (!btr) return(NULL);
		while ((residue = SeqPortGetResidue(spp)) != SEQPORT_EOF) {
			if (IS_residue(residue)) {
				btr[i] = residue;
				i++;
			}
		}
		/*SeqPortRead(spp, btr, len);*/
		SeqPortFree (spp);
	}   
	SeqLocFree (slp);
	return btr;
}

/*******************************************************************************

  Function : DDV_SetStyle()
  
  Purpose : set the layout of a single letter 
  
  Return value : -.

*******************************************************************************/
static void DDV_SetStyle(DDV_ColorCell * dccp,Uint1 style, Uint1 *rgb,
	Boolean bAllowReset)
{
	if (style&DDV_TXTSTYLE_ITALIC){
		dccp->UseItalic=TRUE;
	}
    else if (bAllowReset){
			dccp->UseItalic=FALSE;
	}
	
	if (style&DDV_TXTSTYLE_BOLD){
		dccp->UseBold=TRUE;
	}
    else if (bAllowReset){
		dccp->UseBold=FALSE;
	}
	
	if (style&DDV_TXTSTYLE_UNDERLINE){
		dccp->UseUnderline=TRUE;
	}
	else if (bAllowReset){
    	dccp->UseUnderline=FALSE;
	}
	
	if (style&DDV_TXTSTYLE_LOWERCASE){
		dccp->LowerCase=TRUE;
	}
	else if(bAllowReset) {
    	dccp->LowerCase=FALSE;
	}
	
	if (style&DDV_TXTSTYLE_COLOR && rgb){
		dccp->rgb[0]=rgb[0];
		dccp->rgb[1]=rgb[1];
		dccp->rgb[2]=rgb[2];
	}
}

/*******************************************************************************

  Function : DDV_SetStd_AA_UA_ClrEx()
  
  Purpose : set the standard colours for a nuc or prot sequence; 
     or the same function can be used to layout UnAligned regions

  Note: For performance issue this function set up the layout of
		 both aligned and unaligned regions at the same time
    
  Return value : -

*******************************************************************************/
#define read_buf_size 3000
static void DDV_SetStd_AA_UA_ClrEx(DDV_ColorGlobal *pColorGlobal, void *pData,
                                DDV_Range *pRange,Uint1 what)
{
DDVDataForColorFuncPtr ddfcfp;
BioseqPtr              bsp;
Int4                   bsp_length,i,from,to,limit;
Boolean                IsAA;
Int4                   dp;
Uint1Ptr               SeqBuf;
Int2                   len;
Uint1                  idx;
DDV_ColorCell *        dccp;

	ddfcfp=(DDVDataForColorFuncPtr)pData;
	
	if (!ddfcfp) return;
	
	/*get bioseq size and type */
	bsp=BioseqLockById(ddfcfp->sip);
	if (bsp){
		bsp_length=bsp->length;
		IsAA=ISA_aa(bsp->mol);
		BioseqUnlock(bsp);
	}
	else return;


	/*compute the ends of the bioseq's region we want to analyse*/
	if (ddfcfp->from!=(Int4)-1)
		from=ddfcfp->from;
	else
		from=0;

	if (ddfcfp->to!=(Int4)-1)
		limit=ddfcfp->to;
	else
		limit=bsp_length-1;

	to=from+read_buf_size-1;
	if (to>limit) to=limit;

	/*read the sequence by chunk of 'read_buf_size' letters*/
	while(TRUE){
		len=(Int2)(to-from+1);
		/*get a sequence chunck ; from is always < than to*/
		SeqBuf=DDV_ReadSeqBin (ddfcfp->sip, from, to, IsAA,len,ddfcfp->strand);
		if (SeqBuf){/*scan each letter and get the appropriate colour*/
			for(i=0;i<len;i++){
				/*compute the correct position; depend on the strand*/
				if (ddfcfp->strand==Seq_strand_minus)
					dp=to-i;
				else
					dp=from+i;
				/*first, query Color Manager*/
                dccp=DDV_GetColor(pColorGlobal,NULL,ddfcfp->row+1,dp);
                if (!dccp) return;
                if(what & DDV_USE_STDCLR) {
                    if (IsAA) 
                        idx=DDV_STD_AAColor[*(SeqBuf+i)];/*AA*/
                    else
                        idx=DDV_STD_NAColor[*(SeqBuf+i)];/*NA*/
                    
                    if (ddfcfp->IsUnAligned)
						DDV_SetStyle(dccp,ddfcfp->UAstyle,DDV_PaletteRGB[idx],TRUE);
                    else 
						DDV_SetStyle(dccp,ddfcfp->style,DDV_PaletteRGB[idx],TRUE);
                }
                else if(what & DDV_USE_UALAYOUT) {
                    if (ddfcfp->IsUnAligned)
						DDV_SetStyle(dccp,ddfcfp->UAstyle,ddfcfp->UArgb,TRUE);
                    else 
						DDV_SetStyle(dccp,ddfcfp->style,DDV_PaletteRGB[DDVCOL_BLACK],TRUE);
                }				
                /*set the colour*/
                DDV_SetColor(pColorGlobal, NULL, ddfcfp->row+1, dp, dccp);
            }
            MemFree(SeqBuf);
        }
		
		/*compute the next chunk*/		
		from+=read_buf_size;
		to+=read_buf_size;
		if (to>limit) to=limit;
		/*end ?*/
		if (from>limit) break;
	}

}


/*******************************************************************************

  Function : DDV_InitColourSAP()
  
  Purpose : init the colours for each Bsp of a SeqAlign
  
  Parameters : sap; a (indexed) SeqAlign
  		global colour data struct (see ddvcolor.[ch] in api)
        NoColor; don't color the bsp's, but do change the case.
		
  Note : usually, this function is called when DDV loads a SeqAlign
         For performance issue this function set up the layout of
		 both aligned and unaligned regions at the same time
  
  Return value : FALSE is failure.

*******************************************************************************/
static Boolean DDV_InitColourSAP(SeqAlignPtr sap,MsaParaGPopListPtr mpplp,
	DDV_ColorGlobal * * gclr,Uint1 what,Uint1 * pRGB,Uint1 style,
	Uint1 * pUARGB,Uint1 UAstyle)
{
DDVDataForColorFunc  ddfcf;
SeqIdPtr             sip;
ValNodePtr           vnp,vnp2;
ParaGPtr             pgp;
MsaTxtDispPtr        mtdp;
Int4                 n;

    if (!sap)
    return (FALSE);

    if (!(*gclr)){
        *gclr = DDV_CreateColorGlobal(FALSE, (void *) sap);
        if (!(*gclr))
            return(FALSE);
    }

    for(n=0;n<mpplp->nBsp;n++){
        vnp=mpplp->TableHead[n];
        while(vnp){
            pgp=(ParaGPtr)vnp->data.ptrvalue;
            if (pgp){
                vnp2=pgp->ptxtList;
                sip=pgp->sip;
                while(vnp2){
                    mtdp = (MsaTxtDispPtr)vnp2->data.ptrvalue;
                    if(mtdp && mtdp->TextStyle==MSA_TXT_STYLE_SEQ){
                        ddfcf.sip=sip;
                        ddfcf.from=mtdp->from;
                        ddfcf.to=mtdp->to;
                        ddfcf.row=n;
                        ddfcf.strand=mtdp->strand;
                        ddfcf.IsUnAligned=mtdp->IsUnAligned;
                        ddfcf.style=style;
                        ddfcf.UAstyle=UAstyle;
                        if (pRGB){
                            ddfcf.rgb[0]=pRGB[0];
                            ddfcf.rgb[1]=pRGB[1];
                            ddfcf.rgb[2]=pRGB[2];
                        }
                        else{
                            ddfcf.rgb[0]=0;
                            ddfcf.rgb[1]=0;
                            ddfcf.rgb[2]=0;
                        }
                        if (pUARGB){
                            ddfcf.UArgb[0]=pUARGB[0];
                            ddfcf.UArgb[1]=pUARGB[1];
                            ddfcf.UArgb[2]=pUARGB[2];
                        }
                        else{
                            ddfcf.UArgb[0]=0;
                            ddfcf.UArgb[1]=0;
                            ddfcf.UArgb[2]=0;
                        }
                        DDV_SetStd_AA_UA_ClrEx(*gclr, (Pointer)&ddfcf, NULL, what);
                    }
                    vnp2=vnp2->next;
                }
            }
            vnp=vnp->next;
        }
    }
    return(TRUE);
}

/*******************************************************************************

  Function : DDV_InitColour_When_Start()
  
  Purpose : standard init function for Vibrant DDV ; see DDV_InitColourSAP()
  
*******************************************************************************/
NLM_EXTERN Boolean DDV_InitColour_When_Start(SeqAlignPtr sap,MsaParaGPopListPtr mpplp,
	DDV_ColorGlobal * * gclr, Boolean NoColor)
{
    if(NoColor) 
		return(DDV_InitColourSAP(sap,mpplp,gclr,DDV_USE_STDCLR,NULL,0,NULL,0));
    else 
		return(DDV_InitColourSAP(sap,mpplp,gclr,DDV_USE_STDCLR,NULL,
        	DDV_TXTSTYLE_COLOR,NULL,DDV_TXTSTYLE_COLOR|DDV_TXTSTYLE_LOWERCASE));
}

/*******************************************************************************

  Function : DDV_LayoutUAregion()
  
  Purpose : set a layout for UnAligned regions; see DDV_InitColourSAP()
  
*******************************************************************************/
NLM_EXTERN Boolean DDV_LayoutUAregion(SeqAlignPtr sap,MsaParaGPopListPtr mpplp,
	DDV_ColorGlobal * * gclr,Uint1 * pRGB,Uint1 style,Uint1 * pUARGB,Uint1 UAstyle)
{
	return(DDV_InitColourSAP(sap,mpplp,gclr,DDV_USE_UALAYOUT,pRGB,style,pUARGB,UAstyle));
}


/*******************************************************************************

  Function : UDV_BigEncodeIdxFeat4()
  
  Purpose : encode two Uint4 into one Uint8
  
*******************************************************************************/
static Uint8 UDV_BigEncodeIdxFeat4 (Uint4 val1,Uint4 val2)
{
Uint4 index_g[2];
	
	index_g[0]=val1;
	index_g[1]=val2;
	
	return *((Uint8Ptr) index_g);
	
}

/*******************************************************************************

  Function : UDV_BigEncodeIdxFeat4()
  
  Purpose : decode one Uint8 into two Uint4
  
*******************************************************************************/
static void  UDV_BigDecodeIdxFeat4 (Uint8 index_g, Uint4Ptr val1, Uint4Ptr val2)
{
Uint4Ptr  index_g2;

	index_g2 = (Uint4Ptr) (&index_g);
	if (val1) *val1 = (Uint4) index_g2 [0];
	if (val2) *val2 = (Uint4) index_g2 [1];
}

/*******************************************************************************

  Function : DDV_GetGapCoord()
  
  Purpose : get the list of gaps given a ParaG.
  
  Return value : -.

*******************************************************************************/
static void DDV_GetGapCoord(ParaGPtr pgp,ValNodePtr PNTR gap_list)
{
ValNodePtr    vnp;
MsaTxtDispPtr mtdp;

	if (!pgp) return;
	vnp=pgp->ptxtList;
	while(vnp){
		mtdp=(MsaTxtDispPtr)vnp->data.ptrvalue;
		if (mtdp){
			if(mtdp->IsGap && (mtdp->TextStyle==MSA_TXT_STYLE_GAP || 
				mtdp->TextStyle==MSA_TXT_STYLE_2)){
				ValNodeAddBigInt(gap_list,0,
					UDV_BigEncodeIdxFeat4 ((Uint4)mtdp->from,(Uint4)mtdp->to));
			}
		}
		vnp=vnp->next;
	}
}

/*******************************************************************************

  Function : DDV_AllGapInLowerCase()
  
  Purpose : analyse sequences by pair and switch to lower case all residues
            "aligned" with a gap.
  
  Return value : -.

*******************************************************************************/
NLM_EXTERN void DDV_LayoutGap(DDV_ColorGlobal *pcg,ValNodePtr vnp_seq1,
		ValNodePtr vnp_seq2,Int4 row2,Uint1 style,Uint1 *rgb)
{
DDV_ColorCell * dccp;
ValNodePtr      vnp1,vnp2,vnp3,vnp4;
ParaGPtr        pgp1,pgp2;
Uint4           i,from,to;/*display coord.*/
Int4            bsp_coord;
DDV_ColorCell   dcc;

	if (pcg==NULL || vnp_seq1==NULL || vnp_seq2==NULL) return;
	/*scan the first sequence to modify the second*/
	vnp1=vnp_seq1;
	vnp2=vnp_seq2;
	vnp3=NULL;
	while(vnp1){
		/*is there a gap in the vnp1 ParaG*/
		pgp1=(ParaGPtr)vnp1->data.ptrvalue;
		pgp2=(ParaGPtr)vnp2->data.ptrvalue;
		DDV_GetGapCoord(pgp1,&vnp3);
		if (vnp3){
			vnp4=vnp3;
			while(vnp4){
				UDV_BigDecodeIdxFeat4((Uint8)vnp4->data.bigintvalue, 
					&from, &to);
				to++;
				for (i=from;i<to;i++){
					bsp_coord=(Int4)DDV_GetBspCoordGivenDispCoord(pgp2,(Int4)i);
					if (bsp_coord>=0){
						dccp=DDV_GetColor(pcg,NULL,row2+1,bsp_coord);
						if (dccp){/*if there is already a color, just
						  update the required parameter*/
							DDV_SetStyle(dccp,style,rgb,FALSE);
							DDV_SetColor(pcg,NULL,row2+1,bsp_coord,dccp);
						}
						else{/*otherwise, set up a new layout for that letter*/
							memset(&dcc,0,sizeof(DDV_ColorCell));
							DDV_SetStyle(&dcc,style,rgb,FALSE);
							DDV_SetColor(pcg, NULL, row2+1, bsp_coord, &dcc);
						}
					}
				}
				vnp4=vnp4->next;
			}
		}
		vnp1=vnp1->next;
		vnp2=vnp2->next;
		if (vnp3) 
			vnp3=ValNodeFree(vnp3);
	}
}

/*******************************************************************************

  Function : DDV_LayoutMaskRegions()
  
  Purpose : layout the masked regions of a sequence (usually the query sequence
       of BLAST).
  
  Return value : -.

*******************************************************************************/
NLM_EXTERN void DDV_LayoutMaskRegions(DDV_ColorGlobal *pcg,
	ValNodePtr vnp_query,ValNodePtr mask,Int4 row,Uint1 style,Uint1 *rgb)
{
    DDV_ColorCell * dccp;
    ValNodePtr      vnp;
    SeqLocPtr       slp;
    Int4            i,bsp_start,bsp_stop,mask_start,mask_stop,scan_from,
        scan_to;
    DDV_ColorCell   dcc;
    
    if (pcg==NULL || vnp_query==NULL || mask==NULL) return;
    
    /*get sequence range*/
    UDV_GetBspRangeinPGPList(vnp_query,&bsp_start,&bsp_stop);
    
    /*scan the mask list*/
    vnp=mask;
    while(vnp) {
        slp=(SeqLocPtr)vnp->data.ptrvalue;
        
        if(slp == NULL) 
            return;
        
        if(slp->choice == SEQLOC_PACKED_INT)
            slp = slp->data.ptrvalue;
        else if (slp->choice != SEQLOC_INT)
            return;
        
        while(slp) {

            if(slp->choice != SEQLOC_INT) {
                slp = slp->next;
                continue;
            }
            
            mask_start=SeqLocStart(slp);
            mask_stop=SeqLocStop(slp);
            
            /*check if the masked region is in the bsp range*/
            if (mask_stop>=bsp_start && mask_start<=bsp_stop){
                scan_from=_max_(mask_start,bsp_start);
                scan_to=_min_(mask_stop,bsp_stop)+1;
                for(i=scan_from;i<scan_to;i++) {
                    dccp=DDV_GetColor(pcg,NULL,row+1,i);
                    if (dccp){/*if there is already a color, just
                                update the LowerCase parameter*/
                        DDV_SetStyle(dccp,style,rgb,FALSE);
                        DDV_SetColor(pcg,NULL,row+1,i,dccp);
                    } else{/*otherwies, set a new layout for that letter*/
                        memset(&dcc,0,sizeof(DDV_ColorCell));
                        DDV_SetStyle(&dcc,style,rgb,FALSE);
                        DDV_SetColor(pcg, NULL, row+1, i, &dcc);
                    }
                }
            }
            slp = slp->next;
        }
        vnp=vnp->next;
    }
}

/*******************************************************************************

  Function : DDV_SetISOClr()
  
  Purpose : layout the colors for ident/simil/other (I/S/O).
  
*******************************************************************************/
static void DDV_SetISOClr(DDV_ColorGlobal * pcg, ParaGPtr pgp,Int4 disp_coord, 
		Int4 row, Uint1 idx,Uint1 sty)
{					
DDV_ColorCell * dccp;
DDV_ColorCell   dcc;
Int4            bsp_coord;

	bsp_coord=DDV_GetBspCoordGivenDispCoord(pgp,disp_coord);
	if (bsp_coord>=0){
		dccp=DDV_GetColor(pcg,NULL,row+1,bsp_coord);
		if (dccp){/*if there is already a color, just
		  update the required parameter*/
			dccp->rgb[0]=DDV_PaletteRGB[idx][0];
			dccp->rgb[1]=DDV_PaletteRGB[idx][1];
			dccp->rgb[2]=DDV_PaletteRGB[idx][2];
			DDV_SetStyle(dccp,sty,NULL,FALSE);
			DDV_SetColor(pcg,NULL,row+1,bsp_coord,dccp);
		}
		else{/*otherwise, set up a new layout for that letter*/
			memset(&dcc,0,sizeof(DDV_ColorCell));
			dcc.rgb[0]=DDV_PaletteRGB[idx][0];
			dcc.rgb[1]=DDV_PaletteRGB[idx][1];
			dcc.rgb[2]=DDV_PaletteRGB[idx][2];
			DDV_SetStyle(&dcc,sty,NULL,FALSE);
			DDV_SetColor(pcg, NULL, row+1, bsp_coord, &dcc);
		}
	}
}

/*******************************************************************************

  Function : DDV_LayoutIdentColors()
  
  Purpose : layout the colors for ident/simil/other (I/S/O).
  
  Return value : -.

*******************************************************************************/
NLM_EXTERN void DDV_LayoutISOColors(DDV_ColorGlobal *pcg,ValNodePtr * row_list,Int4 nRow,
	Int4 Master,Boolean bSetMaster,Int4Ptr * matrix,Uint1 IdentClr,Uint1 SimilClr,
	Uint1 OtherClr,Uint1 IdentSty,Uint1 SimilSty,Uint1 OtherSty)
{
ValNodePtr * row;
ValNodePtr vnp;
ParaGPtr  pgpQuery,pgpSubject;
CharPtr   szQuery,szComp;
BioseqPtr bsp;
Boolean   IsAA;
Int4      i,j,len,bspLength;
Uint1     idx,sty;

	row=(ValNodePtr *)MemNew(nRow*sizeof(ValNodePtr));
	if (!row) return;
	for (i=0;i<nRow;i++){
		row[i]=row_list[i];
	}

	vnp=row_list[Master];
	while(vnp){
		/*get the master sequence*/
		pgpQuery=(ParaGPtr)vnp->data.ptrvalue;
		szQuery=(CharPtr)MemNew((pgpQuery->StopLetter-pgpQuery->StartLetter+3)*sizeof(Char));
		if (!szQuery) goto error;
		bsp=BioseqLockById(pgpQuery->sip);
		if (!bsp) goto error;
		bspLength=BioseqGetLen(bsp);
		IsAA=ISA_aa(bsp->mol);
		BioseqUnlock(bsp);
		if (!DDV_GetSequenceFromParaG(pgpQuery,&szQuery,bspLength,IsAA,NULL,
				NULL,NULL)) goto error;
		/*lopp on each row, get the sequence, set the colors*/
		for (i=0;i<nRow;i++){
			if (i==Master) continue;
			pgpSubject=(ParaGPtr)row[i]->data.ptrvalue;
			szComp=DDV_GetBLASTCompLine_3(szQuery,pgpSubject,matrix);
			if (szComp){
				len=StringLen(szComp)+1;
				for (j=0;j<len;j++){
					if (isalpha(szComp[j]) || szComp[j]=='|'){
						idx=IdentClr;
						sty=IdentSty;
					}
					else if (szComp[j]=='+'){
						idx=SimilClr;
						sty=SimilSty;
					}
					else{
						idx=OtherClr;
						sty=OtherSty;
					}
					DDV_SetISOClr(pcg,pgpSubject,pgpSubject->StartLetter+j,i,idx,sty);
					if (bSetMaster)
						DDV_SetISOClr(pcg,pgpQuery,pgpQuery->StartLetter+j,0,idx,sty);
				}
				MemFree(szComp);
			}
			row[i]=row[i]->next;
		}
		szQuery=MemFree(szQuery);
		vnp=vnp->next;
	}
error:

	MemFree(row);
	if (szQuery) MemFree(szQuery);
}


NLM_EXTERN MsaParaGPopListPtr DDE_CreateDisplayForBlock(SeqAlignPtr sap, Int4 BlockIndex) {
/*----------------------------------------------------------------------------
*  allocate and fill the paragraphs for displaying 1 block of a
*  sequence alignment, plus the flanking unaligned regions.
*  BlockIndex is a 0-based block count.
*
*  This function works by making a linked-list of 3 DescriDisp's:
*  head --> node1 --> node2 --> node3
*            ||        ||        ||
*            \/        \/        \/
*         unaligned  aligned  unaligned
*
*  This is a descriptor for the display.  This descriptor is passed
*  to DDV_PopulateDisplayForDisc to create the paragraphs.
*  Each node is easy to construct:
*
*
*                           Aligned (A)                UnAligned (UA)
*                           -----------                --------------
*  from:                SeqAlign coords                             0
*  to:                     "       "                         length-1
*  TextStyle:   MSA_TXT_STYLE_REG_ALIGN     MSA_TXT_STYLE_REG_UNALIGN
*  UAnum:                             0             (explained below)
*  UAMaxLength:                       0                        length
*
*
*  UAnum, for unaligned regions, is determined as shown here:
*
*  BlockIndex:               0                1                2
*               |<--UA-->|<--A-->|<--UA-->|<--A-->|<--UA-->|<--A-->|<--UA-->|
*  UAnum:           -1                1                2               -2
*  where -1 and -2 are for the LEFT_TAIL and RIGHT_TAIL, respectively.
*
*
*  return a pointer to the paragraph list.
*  return NULL for unsuccessful completion.
*---------------------------------------------------------------------------*/
  ValNodePtr          head = NULL;
  DescriDispPtr       ddp1, ddp2, ddp3;
  Int4                i, NumRows, NumBlocks, Size, SaveSize, start, stop;
  Int4                TotalLength=0;
  Uint1               strand=Seq_strand_unknown;
  DDV_Disp_Opt        DisplayOptions;
  MsaParaGPopListPtr  pPopList;

  /* index the seq align */
/*  if (!AlnMgrIndexSeqAlign(sap)) {
    return(NULL);
  } */

  NumRows = AlnMgrGetNumRows(sap);
  NumBlocks = AlnMgrGetNumAlnBlocks(sap);
  if ((BlockIndex < 0) || (BlockIndex >= NumBlocks)) {
    return(NULL);
  }

  /* figure out the length of the unaligned region preceeding the block */
  SaveSize = 0;
  for (i=0; i<NumRows; i++) {
    if (BlockIndex == 0) {
      AlnMgrGetNthRowTail(sap, i+1, LEFT_TAIL, &start, &stop, &strand);
    }
    else {
      AlnMgrGetNthUnalignedForNthRow(sap, BlockIndex, i+1, &start, &stop);
    }
    if ((start == -1) && (stop == -1)) {Size = 0;}
    else                               {Size = (stop-start)+1;}
    if (Size > SaveSize) {SaveSize = Size;}
  }
  TotalLength += SaveSize;

  /* make a node for this region */
  ddp1 = MemNew(sizeof(DescriDisp));
  ddp1->from = 0;
  ddp1->to = SaveSize - 1;
  ddp1->TextStyle = MSA_TXT_STYLE_REG_UNALIGN;
  /* set UAnum to -1 for LEFT_TAIL */
  ddp1->UAnum = (BlockIndex == 0) ? -1 : BlockIndex;
  ddp1->UAMaxlength = SaveSize;
  ddp1->strand = strand;
  ddp1->IsGap = FALSE;

  /* add it to the linked list */
  ValNodeAddPointer(&head, 0, ddp1);

  /* make node for aligned block and add it to linked list */
  AlnMgrGetNthBlockRange(sap, BlockIndex+1, &start, &stop);
  ddp2 = MemNew(sizeof(DescriDisp));
  ddp2->from = start;
  ddp2->to = stop;
  ddp2->TextStyle = MSA_TXT_STYLE_REG_ALIGN;
  ddp2->UAnum = 0;
  ddp2->UAMaxlength = 0;
  ddp2->strand = strand;
  ddp2->IsGap = FALSE;
  ValNodeAddPointer(&head, 0, ddp2);
  TotalLength += (stop - start) + 1;

  /* figure out the length of the unaligned region following the block */
  SaveSize = 0;
  for (i=0; i<NumRows; i++) {
    if (BlockIndex == (NumBlocks-1)) {
      AlnMgrGetNthRowTail(sap, i+1, RIGHT_TAIL, &start, &stop, &strand);
    }
    else {
      AlnMgrGetNthUnalignedForNthRow(sap, BlockIndex+1, i+1, &start, &stop);
    }
    if ((start == -1) && (stop == -1)) {Size = 0;}
    else                               {Size = (stop-start)+1;}
    if (Size > SaveSize) {SaveSize = Size;}
  }
  TotalLength += SaveSize;

  /* make a node for this region */
  ddp3 = MemNew(sizeof(DescriDisp));
  ddp3->from = 0;
  ddp3->to = SaveSize - 1;
  ddp3->TextStyle = MSA_TXT_STYLE_REG_UNALIGN;
  /* set UAnum to -2 for RIGHT_TAIL */
  ddp3->UAnum = (BlockIndex == (NumBlocks-1)) ? -2 : BlockIndex + 1;
  ddp3->UAMaxlength = SaveSize;
  ddp3->strand = strand;
  ddp3->IsGap = FALSE;

  /* add it to the linked list */
  ValNodeAddPointer(&head, 0, ddp3);

  /* create the paragraphs from the descriptor */
  pPopList = MemNew(sizeof(MsaParaGPopList));
  DDV_InitDDESAPdispStyles(&DisplayOptions);
  if (DDV_PopulateDisplayForDisc(sap, pPopList, NumRows, TotalLength+10,
                                 &DisplayOptions, TotalLength, head, TRUE, FALSE)) {
    return(pPopList);
  }
  else {
    return(NULL);
  }
}


NLM_EXTERN MsaParaGPopListPtr DDE_CreateDisplayForUnAligned(SeqAlignPtr sap, Int4 UAIndex) {
/*----------------------------------------------------------------------------
*  allocate and fill the paragraphs for displaying 1 unaligned
*  region of a sequence alignment.  UAIndex is a 0-based count.
*  (UA = unaligned)
*
*  This function works by making a linked-list of just 1 DescriDisp.
*  This is a descriptor for the display.  This descriptor is passed
*  to DDV_PopulateDisplayForDisc to create the paragraphs.
*
*  Each node is easy to construct:
*
*                             UnAligned (UA)
*                ---------------------------
*  from:                                   0
*  to:                              length-1
*  TextStyle:      MSA_TXT_STYLE_REG_UNALIGN
*  UAnum:                  (explained below)
*  UAMaxLength:                       length
*
*  UAnum is determined as shown here:
*
*  UAIndex:           0                1                2                3
*                |<--UA-->|<--A-->|<--UA-->|<--A-->|<--UA-->|<--A-->|<--UA-->|
*  UAnum:            -1                1                2               -2
*  where -1 and -2 are for the LEFT_TAIL and RIGHT_TAIL, respectively.
*
*  return a pointer to the paragraph list.
*  return NULL for unsuccessful completion.
*---------------------------------------------------------------------------*/
  ValNodePtr          head = NULL;
  DescriDispPtr       ddp1;
  Int4                NumRows, NumBlocks;
  Int4                i, Size, SaveSize, start, stop;
  Int4                TotalLength=0;
  Uint1               strand=Seq_strand_unknown;
  DDV_Disp_Opt        DisplayOptions;
  MsaParaGPopListPtr  pPopList;

  /* index the seq align */
/*  if (!AlnMgrIndexSeqAlign(sap)) {
    return(NULL);
  } */

  NumRows = AlnMgrGetNumRows(sap);
  NumBlocks = AlnMgrGetNumAlnBlocks(sap);
  if ((UAIndex < 0) || (UAIndex > NumBlocks)) {
    return(NULL);
  }

  /* figure out the length of the unaligned region preceeding the block */
  SaveSize = 0;
  for (i=0; i<NumRows; i++) {
    if (UAIndex == 0) {
      AlnMgrGetNthRowTail(sap, i+1, LEFT_TAIL, &start, &stop, &strand);
    }
    else if (UAIndex == NumBlocks) {
      AlnMgrGetNthRowTail(sap, i+1, RIGHT_TAIL, &start, &stop, &strand);
    }
    else {
      AlnMgrGetNthUnalignedForNthRow(sap, UAIndex, i+1, &start, &stop);
    }
    if ((start == -1) && (stop == -1)) {Size = 0;}
    else                               {Size = (stop-start)+1;}
    if (Size > SaveSize) {SaveSize = Size;}
  }
  TotalLength += SaveSize;

  /* make a node for this region */
  ddp1 = MemNew(sizeof(DescriDisp));
  ddp1->from = 0;
  ddp1->to = SaveSize - 1;
  ddp1->TextStyle = MSA_TXT_STYLE_REG_UNALIGN;
  /* set UAnum to -1 for LEFT_TAIL, -2 for RIGHT_TAIL, or UAIndex */
  if (UAIndex == 0)              {ddp1->UAnum = -1;}
  else if (UAIndex == NumBlocks) {ddp1->UAnum = -2;}
  else                           {ddp1->UAnum = UAIndex;}
  ddp1->UAMaxlength = SaveSize;
  ddp1->strand = strand;
  ddp1->IsGap = FALSE;

  /* add it to the linked list */
  ValNodeAddPointer(&head, 0, ddp1);

  /* create the paragraphs from the descriptor */
  pPopList = MemNew(sizeof(MsaParaGPopList));
  DDV_InitDDESAPdispStyles(&DisplayOptions);
  if (DDV_PopulateDisplayForDisc(sap, pPopList, NumRows, TotalLength+10,
                                 &DisplayOptions, TotalLength, head, FALSE, TRUE)) {
    return(pPopList);
  }
  else {
    return(NULL);
  }
}


NLM_EXTERN MsaParaGPopListPtr DDE_CreateDisplay(SeqAlignPtr sap, Int4 BlockIndex,
                                                Boolean IsUnAligned, Int4* pNumBlocks) {
/*----------------------------------------------------------------------------
*  build display, ruler, entitiesTbl for either an
*  unaligned region or an aligned block
*---------------------------------------------------------------------------*/
  MsaParaGPopListPtr  mpplp;
  Int4                from=-1, to;
 
  if (IsUnAligned) {
    mpplp = DDE_CreateDisplayForUnAligned(sap, BlockIndex);
    *pNumBlocks = 0;
  }
  else {
    mpplp = DDE_CreateDisplayForBlock(sap, BlockIndex);
    *pNumBlocks = 1;
    /* get the first align index of block */
    AlnMgrGetNthBlockRange(sap, BlockIndex+1, &from, &to);
  }
  ASSERT(mpplp != NULL);

  mpplp->entitiesTbl = DDV_BuildBspEntitiesTbl(mpplp->TableHead, mpplp->nBsp);
  ASSERT(mpplp->entitiesTbl != NULL);

  mpplp->RulerDescr = DDE_ReMakeRuler(mpplp, FALSE, from);
  return(mpplp);
}

