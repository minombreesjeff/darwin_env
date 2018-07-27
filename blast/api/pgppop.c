/*  $Id: pgppop.c,v 6.67 2001/10/03 00:15:47 vakatov Exp $
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
* File Name:  pgppop.c
*
* Author:  Patrick Durand
*
* Version Creation Date:   05/03/99
*
* $Revision: 6.67 $
*
* File Description: 
*
* Modifications:
* --------------------------------------------------------------------------
*
* $Log: pgppop.c,v $
* Revision 6.67  2001/10/03 00:15:47  vakatov
* Replaced some ASSERTs by VERIFYs
*
* Revision 6.66  2001/01/16 17:15:54  hurwitz
* modified DDV_GetBspCoordGivenDispCoord
*
* Revision 6.65  2001/01/10 23:38:39  lewisg
* fix seqid and various memory leaks
*
* Revision 6.64  2000/10/25 01:22:56  bauer
* fixed DDV display of PDB-Id's in CDD-server
*
* Revision 6.63  2000/10/13 19:48:16  hurwitz
* added functions for getting first VALID disp coord in range of bsp coords
*
* Revision 6.62  2000/08/25 18:57:12  shavirin
* Removed printing of BLAST middle line if characters in 1st and 3d
* lines are in lower case - used for unaligned regions.
*
* Revision 6.61  2000/08/11 20:59:07  shavirin
* Added default character for a gap in the function DDV_GetSequenceFromParaG().
*
* Revision 6.60  2000/07/18 19:59:24  bauer
* fixed bug in DDV_Print_Sequence
*
* Revision 6.59  2000/05/19 14:30:52  wheelan
* fixed problem with formatting PDB ids
*
* Revision 6.58  2000/04/19 12:33:32  durand
* for HTML output, replaced double quote char. by a spacein the defline
*
* Revision 6.57  2000/03/31 21:33:21  durand
* added new default color schemas for BLAST
*
* Revision 6.56  2000/03/29 14:21:56  durand
* fixed problem when displaying middle line of BLAST SeqAligns
*
* Revision 6.54  2000/03/28 13:32:28  durand
* update DDV_DisplayDefaultAlign to receive a pre-initialized DDV_Disp_Opt data structure
*
* Revision 6.53  2000/03/27 14:19:13  durand
* fixed bugs in BLAST outputs
*
* Revision 6.52  2000/03/24 12:58:55  durand
* fixed a bug in DDV_DisplayDefaultAlign
*
* Revision 6.51  2000/03/22 14:14:25  durand
* updated DDV_DisplayDefaultAlign to get the SeqAlign size correclty
*
* Revision 6.50  2000/03/21 19:26:48  durand
* pgppop now uses AlignMgr
*
* Revision 6.49  2000/02/24 16:46:51  thiessen
* fixed improper acces to freed memory
*
* Revision 6.48  2000/02/23 19:49:49  durand
* use row number instead of SeqId for coloring
*
* Revision 6.47  2000/02/22 21:20:11  durand
* remove the second $LOG line
*
* Revision 6.46  2000/02/22 20:55:22  thiessen
* add null pointer test to DDV_GetBspCoordGivenDispCoord()
*
* Revision 6.45  2000/02/17 15:54:35  durand
* use ~ for an unaligned gap and - for an aligned gap
*
* Revision 6.44  2000/02/15 15:31:45  lewisg
* move DDVRulerDescr to pgppop
*
* Revision 6.43  2000/02/14 16:39:55  durand
* add new output options for BLAST
*
* Revision 6.42  2000/02/07 14:16:56  durand
* replaced BioseqUnlockById by BioseqUnlock
*
* Revision 6.41  2000/02/03 14:03:57  durand
* replaced call to FeatDefLine() by CreateDefLine()
*
* Revision 6.40  2000/01/26 15:08:22  durand
* update DDV_DeleteParaGList function
*
* Revision 6.39  1999/12/29 22:55:02  lewisg
* get rid of seqalign id
*
* Revision 6.38  1999/12/20 14:37:53  durand
* transfer some PopSet Viewer functions from here to wwwddv.c; update the code to better use Color Manager for BLAST outputs
*
* Revision 6.37  1999/12/08 22:40:54  durand
* add the code to produce colored BLAST outputs
*
* Revision 6.36  1999/12/07 18:46:33  durand
* add DDV_GetBspCoordGivenPgpList function
*
* Revision 6.35  1999/11/26 15:42:25  vakatov
* Fixed for the C++ and/or MSVC DLL compilation
*
* Revision 6.34  1999/10/29 14:14:24  durand
*  add DDV_GetBspCoordGivenDispCoord() and DDV_GetDispCoordGivenBspCoord()
*
* Revision 6.33  1999/10/20 13:13:53  durand
* add new fields in data structure for DDV
*
* Revision 6.32  1999/10/08 17:50:28  durand
* move DDV_DisplayBlastSAP from pgppop.c to ddvcreate.c due to conflict between api and ddv
*
* Revision 6.31  1999/10/07 19:18:46  durand
* Modified function DDV_DisplayBlastSAP to use AlignMgr
*
* Revision 6.30  1999/09/29 17:16:44  shavirin
* Modified function DDV_DisplayBlastSAP(): added new parameter and printing
* of the BLAST scores.
*
* Revision 6.29  1999/09/29 13:42:27  durand
* add middle line for BLAST output
*
* Revision 6.27  1999/09/28 19:49:30  shavirin
* Changed definition of the function DDV_DisplayBlastSAP()
*
* Revision 6.26  1999/09/28 13:06:39  durand
* add a first set of functions to display CDS in PopSet Viewer
*
* Revision 6.24  1999/09/16 18:52:26  durand
* redesign the PopSet viewer toolbar
*
* Revision 6.23  1999/09/07 13:41:55  durand
* update Entrez links for PopSet Viewer
*
* Revision 6.22  1999/09/02 19:08:56  chappey
* fixes in PrintSeqAlignCallback
*
* Revision 6.21  1999/09/01 21:04:20  durand
* call SeqAlignSetFree after PairSeqAlign2MultiSeqAlign
*
* Revision 6.20  1999/08/31 13:51:30  durand
* add PubMed link for PopSet Viewer
*
* Revision 6.18  1999/08/30 21:24:23  durand
* display Unpublished study when a PopSet entry doesn't have a title
*
* Revision 6.17  1999/08/30 20:29:56  durand
* make PrintSeqAlignCallback estern function
*
* Revision 6.16  1999/08/30 18:20:11  durand
* update SeqAlignToBS
*
* Revision 6.15  1999/08/30 17:57:05  sirotkin
* changed mydata in PrintSeqAlignCallback
*
* Revision 6.14  1999/08/30 14:18:08  durand
* use ByteStore to format the SeqAlign output
*
* Revision 6.12  1999/08/27 14:54:04  durand
* fix memory leaks in PopSet Viewer
*
* Revision 6.11  1999/08/16 18:56:59  lewisg
* made DDV_GetSeqAlign extern and added prototype to header
*
* Revision 6.10  1999/08/08 15:54:35  chappey
* made DDV_GetSeqAlign static as there is no prototype
*
* Revision 6.9  1999/08/07 16:57:48  sicotte
* fixed compiler warnings
*
* Revision 6.8  1999/08/07 16:53:13  sicotte
* added includes sqnutils.h and alignval.h for new code
*
* Revision 6.7  1999/08/06 21:43:17  chappey
* SeqAlignToBS new function to save in ByteStore structure the text output of the SeqAlign(s) packaged in a SeqEntry
*
* Revision 6.6  1999/07/22 13:23:13  durand
* made DDV_SearchAli external function
*
* Revision 6.5  1999/07/21 21:52:12  durand
* add some functions to display a summary for a PopSet entry
*
* Revision 6.4  1999/07/19 21:16:01  durand
* add DDV_ResetParaGSeqAlignCoord to reset the seqalign coord in the display data structures of DDV
*
* Revision 6.3  1999/07/15 18:20:51  durand
* add display options to support BLAST outputs
*
* Revision 6.2  1999/07/13 20:46:56  durand
* comment out call to PairSeqAlign2MultiSeqAlign to avoid DDV compiling problems
*
* Revision 6.1  1999/07/09 13:59:58  durand
* move pgppop from desktop to api
*
* Revision 6.2  1999/07/07 19:12:26  durand
* fix a tiny bug in DDV_GetSequenceFromParaG
*
* Revision 6.1  1999/07/06 20:18:07  kans
* initial public checkin
*
* Revision 1.35  1999/07/06 18:54:27  durand
* add new features for the display of PopSet viewer
*
* Revision 1.34  1999/07/02 13:22:17  durand
* fix bugs for the display of minus strand sequences
*
* Revision 1.32  1999/06/29 16:48:10  shavirin
* Changed definition of function DDV_ShowSeqAlign()
*
* Revision 1.31  1999/06/28 22:07:20  durand
* add loader functions and clean the code with Lint and Purify
*
* Revision 1.30  1999/06/24 20:49:41  shavirin
* Added new function DDV_ShowSeqAlign().
*
* Revision 1.29  1999/06/23 18:11:17  durand
* fix a variable initialization problem under NT
*
* Revision 1.28  1999/06/23 17:24:23  durand
* use a binary encoding to manage the display styles
*
* Revision 1.27  1999/06/21 18:37:56  durand
* update DDV_DisplayDefaultAlign to produce full text output
*
* Revision 1.26  1999/06/19 18:36:13  durand
* new display procedure
*
* Revision 1.25  1999/06/16 13:10:48  durand
* update/add functions for Vibrant DDV
*
* Revision 1.24  1999/06/14 23:49:43  durand
* add function for Vibrant DDV
*
* Revision 1.23  1999/06/11 22:33:01  durand
* add new functions for Vibrant DDV
*
* Revision 1.22  1999/06/11 17:59:40  durand
* popset viewer uses more code from UDV
*
* Revision 1.21  1999/06/09 21:35:30  durand
* add constructors/destructors for BspInfo struct as well as read seq function
*
*
*
*
* ==========================================================================
*/
#include <stdio.h>
#include <ncbi.h>
#include <blocks.h>
#include <pgppop.h>
#include <tofasta.h>
#include <udvdef.h>
#include <udvseq.h>
#include <salstruc.h>
#include <sqnutils.h>
#include <alignval.h>
#include <asn2ff6.h>
#include <subutil.h>
#include <ddvcolor.h>
#include <ddvcreate.h>

#define DDV_COLOR_MAX 33 
 static Uint1 DDV_PaletteRGB[DDV_COLOR_MAX][3] =
 {
     /* Red  Grn Blue     Color     ColorID             Old Hex  New Hex */
   255, 255, 255, /* default     0                 0xffffff - */ 
   255,  00, 153, /* hotpink     1                 0xff1493 0xff0099 */
   255,   0, 255, /* magenta     2                 0xff00ff - */
   153,  51, 255, /* purple      3                 0x9b30ff 0x9933ff */
     0,   0, 255, /* blue        4                 0x0000ff - */
    00, 153, 255, /* sky         5                 0x1e90ff 0x0099ff */
     0, 255, 255, /* cyan        6                 0x00ffff - */
     0, 255, 153, /* sea         7                 0x00ff8f 0x00ff99 */
     0, 255,   0, /* green       8                 0x00ff00 - */
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

static CharPtr DDV_PaletteRGB_NAV[DDV_COLOR_MAX] =
 {
     /* Red  Grn Blue     Color     ColorID             Old Hex  New Hex */
   "ffffff", /* default     0                 0xffffff - */ 
   "ff0099", /* hotpink     1                 0xff1493 0xff0099 */
   "ff00ff", /* magenta     2                 0xff00ff - */
   "9933ff", /* purple      3                 0x9b30ff 0x9933ff */
   "0000ff", /* blue        4                 0x0000ff - */
   "0099ff", /* sky         5                 0x1e90ff 0x0099ff */
   "00ffff", /* cyan        6                 0x00ffff - */
   "00ff99", /* sea         7                 0x00ff8f 0x00ff99 */
   "00ff00", /* green       8                 0x00ff00 - */
   "ffff00", /* yellow      9                 0xffff00 - */
   "ffcc00", /* gold       10                 0xffa500 0xffcc00 */
   "ff6600", /* orange     11                 0xff4500 0xff6600 */
   "ff0000", /* red        12                 0xff0000 - */
   "ff9999", /* pink       13                 0xff7256 0xff9999 */
   "ffcccc", /* pinktint   14                 0xffaeb9 0xffcccc */
   "ffffff", /* white      15                0xffffff  -*/
   "000000", /* black      16                0x000000 - */
   "ccccff", /* bluetint   17                0xb0e2ff - 0xccccff*/
   "99ff99", /* greentint  18                0x9aff9a - 0x99ff99 */
   "ffff99", /* yellowtint 19                0xffec8b - 0xffff99 */
   "666666", /* gray       20                0x7d7d7d   0x666666 */
   "996633", /* brown      21                0x8b5742   01x996633 */
   "ff0000", /* perm colors 22: red          0xff0000 - */
   "00ff00", /* perm colors 23: green        0x00ff00 - */
   "ff00ff", /* perm colors 24: magenta      0xff00ff - */
   "0099ff", /* perm colors 25: sky          0x1e90ff 0x0099ff */
   "9933ff", /* perm colors 26: purple        0x9b30ff 0x9933ff */
   "00ff00", /* SS colors 27: helix, green   0x00ff00 - */
   "ffcc00", /* SS colors 28: strand, gold   0xffa500 0xffcc00 */
   "ff6600", /* SS colors 29: turn, orange   0xff4500 0xff6600 */
   "00ffff", /* SS colors 30: coil, cyan     ox00ffff - */
   "ffff00", /* highlight colors 31: yellow  0xffff00 - */
   "000000" /* background colors 32: black  0x000000 - */
 };

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

#define DDVCOL_HIGHLIGHT 31
#define DDVCOL_BACKGROUND 32

/*******************************************************************************

  Function : DDV_ConcatStr()
  
  Purpose : merge to CharPtr; realloc if needed
  
  Parameters : 	src; CharPtr source
  				newStr; CharPtr to add to src

  Return value : the merging result of src and newStr

*******************************************************************************/
static CharPtr DDV_ConcatStr(CharPtr src,CharPtr newStr)
{
CharPtr retScr;

	if(src==NULL){
		retScr=StringSave(newStr);
	}
	else{
		retScr=(CharPtr)MemNew((StringLen(src)+StringLen(newStr)+4)*sizeof(Char));
		if (!retScr) return(NULL);
		StringCpy(retScr,src);
		StringCat(retScr,newStr);
		MemFree(src);
	}
	
	return(retScr);
}

/*******************************************************************************

  Function : ConvertToLower()
  
  Purpose : convert an upper case string to a lower case one
  
  Parameters : header of the ValNodeList
  
  Note : this function is a copy of to_lower() (ncbi/tools). Because pgppop.c
         is in ncbi/api, I cannot use the code of salutil.
		 
  Return value : none 

*******************************************************************************/
static CharPtr ConvertToLower (CharPtr str)
{
  CharPtr tmp;
 
  if (str==NULL)
     return NULL;
  tmp = str;
  while (*tmp!='\n' && *tmp!='\0') {
     *tmp = TO_LOWER(*tmp);
     tmp++;
  }
  return str;
}

/*******************************************************************************

  Function : DDV_DeleteTxtList()
  
  Purpose : delete a list of sequence content in a ParaG
  
  Parameters : header of the ValNodeList
  
  Return value : none 

*******************************************************************************/
NLM_EXTERN void DDV_DeleteTxtList(ValNodePtr PNTR vnp)
{
ValNodePtr 	vnp2;
MsaTxtDispPtr mtdp;

	for(vnp2=(*vnp) ; vnp2!=NULL ; vnp2=vnp2->next){
		mtdp=(MsaTxtDispPtr)vnp2->data.ptrvalue;
		if (mtdp) MemFree(mtdp);
	}
	ValNodeFree(*vnp);
	*vnp=NULL;
}

/*******************************************************************************

  Function : DDV_DeleteParaGList()
  
  Purpose : delete a list of ParaG
  
  Parameters : header of the ValNodeList
  
  Return value : none 

*******************************************************************************/
NLM_EXTERN void DDV_DeleteParaGList(ValNodePtr PNTR vnp)
{
ValNodePtr 	vnp2;
ParaGPtr pgp;

	for(vnp2=(*vnp) ; vnp2!=NULL ; vnp2=vnp2->next){
		pgp=(ParaGPtr)vnp2->data.ptrvalue;
		if (pgp){
			if (pgp->ptxtList) DDV_DeleteTxtList(&pgp->ptxtList);
			if (pgp->szEditSeq) MemFree(pgp->szEditSeq);
			if (pgp->pFeatList) ValNodeFree(pgp->pFeatList);
            if (pgp->sip) SeqIdFree(pgp->sip);
			MemFree(pgp);
		}
	}
	ValNodeFree(*vnp);
	*vnp=NULL;
}

/*******************************************************************************

  Function : DDV_DeleteDisplayList()
  
  Purpose : delete the complete lists used for the display
  
  Parameters : main data block used for the display
  
  Return value : none 

*******************************************************************************/
NLM_EXTERN void DDV_DeleteDisplayList(MsaParaGPopListPtr mpplp)
{
Int4 i;

	/*ValNode with ParaG*/
	if (mpplp->TableHead){
		for(i=0;i<mpplp->nBsp;i++){
			if (mpplp->TableHead[i]){
				DDV_DeleteParaGList(&(mpplp->TableHead[i]));
			}
		}
		MemFree(mpplp->TableHead);
	}
}

/*******************************************************************************

  Function : DDV_WWW_get_CDS_minus()
  
  Purpose : get the translation of a CDS located on a PLUS strand
  
  Parameters : context; feature data (see explore.h for a def of the structure)
  				start; get the sequence from...
				stop;... to.
				StartLetter; beginning of a ParaG (zero-based value)
				i; order number of the exon (if CDS is exon-encoded)
  				szDNA; DNA sequence
				LineSize; size of a line
				
  Return value : none

*******************************************************************************/
static void DDV_WWW_get_CDS_minus(SeqMgrFeatContextPtr context,Int4 start,
			Int4 stop,Int4 StartLetter,Int2 i,CharPtr szDNA,CharPtr szTrans,
			Int2 LineSize)
{						
CharPtr      	str=NULL;
Int4			il=0,
				pos=0,
				n=0,
				stop_prot=0,
				gap,
				limit;
Int2 			n2,nCompt,
				decal=0,
				numivals2=0;
ByteStorePtr	bs=NULL;
Boolean bGiForProductOk=FALSE;
SeqIdPtr sip=NULL;
Int4 gi=0,size_trans;
BioseqPtr prot;

	/*retrieve the protein sequence; need to be optimized in future release*/
	if (context->sfp->product){
		prot=BioseqFind(SeqLocId(context->sfp->product));
		sip=(SeqIdPtr)GetProductSeqId(context->sfp->product);
		if (sip){
			gi = GetGINumFromSip(sip);
			if (gi>0) bGiForProductOk=TRUE;
			str=UDV_Read_Sequence (sip,0,prot->length-1, 
				TRUE,prot->length-1);
		}
	}
	else{
		bs = ProteinFromCdRegion (context->sfp, TRUE);
		if (bs){
			str = (CharPtr) BSMerge (bs, NULL);
			BSFree (bs);
		}
	}

	if (!str) return;	

	/*count the lenght of the intron(s)*/
	numivals2=context->numivals*2;
	if (i>0){
		for(nCompt=i;nCompt>0;nCompt-=2){
			il+=(context->ivals[nCompt-2]-
				context->ivals[nCompt+1]-1);
		}
	}

	pos=context->ivals[1]-il-stop;
	/*place the current position within the correct translation frame*/
	if (pos % 3){
		if (!((pos-1) % 3)){
			pos=(pos-1)/3;
			decal=-1;
		}
		else if (!((pos+1) % 3)){
			pos=(pos+1)/3;
			decal=1;
		}						
	}else {
		pos=pos/3;
		decal=0;
	}

	n=pos;
	stop_prot=stop-decal-1; /*-1 == middle of the codon*/
	
	LineSize--;
	gap=0;
	if (szTrans){
		start--;
		while(stop_prot>start){
			limit=stop_prot-StartLetter+gap+1;
			gap=0;
			for(i=0;i<limit;i++){
				if (szDNA[i]=='-') gap++;
			}
			if (szDNA[limit-1]=='-'){
				while(szDNA[i]){
					if (szDNA[i]=='-') gap++;
					else break;
					i++;
				}
			}
			n2=stop_prot-StartLetter+gap+(stop_prot-StartLetter+gap)/LETTER_BLOCK_WIDTH;
			if (str[n])
				szTrans[n2]=str[n];
			else 
				break;
			
			stop_prot-=3;
			n++;
		}
	}

	if (str){
		MemFree (str);
	}
}


/*******************************************************************************

  Function : DDV_WWW_get_CDS_plus()
  
  Purpose : get the translation of a CDS located on a PLUS strand
  
  Parameters : context; feature data (see explore.h for a def of the structure)
  				start; get the sequence from...
				stop;... to.
				StartLetter; beginning of a ParaG (zero-based value)
				i; order number of the exon (if CDS is exon-encoded)
  				szDNA; DNA sequence
				LineSize; size of a line
				
  Return value : none

*******************************************************************************/
static void DDV_WWW_get_CDS_plus(SeqMgrFeatContextPtr context,Int4 start,
			Int4 stop,Int4 StartLetter,Int4 size_pgp,
			Int2 i,CharPtr szDNA,CharPtr szTrans,
			Int2 LineSize,Uint1 strand)
{						
CharPtr      	str=NULL;
Int4			il=0,
				pos=0,
				n=0,
				start_prot,
				stop_prot,
				limit,
				gap;
Int2 			n2=0,
				decal=0,
				numivals2=0,
				nCompt=0;
ByteStorePtr	bs=NULL;
Boolean bGiForProductOk=FALSE;
SeqIdPtr sip=NULL;
Int4 gi=0,size_trans;
BioseqPtr prot;

	/*retrieve the protein sequence; need to be optimized in future release*/
	if (context->sfp->product){
		prot=BioseqFind(SeqLocId(context->sfp->product));
		sip=(SeqIdPtr)GetProductSeqId(context->sfp->product);
		if (sip){
			gi = GetGINumFromSip(sip);
			if (gi>0) bGiForProductOk=TRUE;
			str=UDV_Read_Sequence (sip,0,prot->length-1, 
				TRUE,prot->length);
		}
	}
	else{
		bs = ProteinFromCdRegion (context->sfp, TRUE);
		if (bs){
			str = (CharPtr) BSMerge (bs, NULL);
			BSFree (bs);
		}
	}

	if (!str) return;	
	/*count the lenght of the intron(s)*/
	numivals2=context->numivals*2;
	if (i>0 && i<numivals2){
		for(nCompt=2;nCompt<i+2;nCompt+=2){
			il+=(context->ivals[nCompt]-
				context->ivals[nCompt-1]-1);
		}
	}

	pos=start-il-context->ivals[0];
						
	/*place the current position within the correct translation frame*/
	if (pos % 3){
		if (!((pos-1) % 3)){
			pos=(pos-1)/3;
			decal=-1;
		}
		else if (!((pos+1) % 3)){
			pos=(pos+1)/3;
			decal=1;
		}						
	}else {
		pos=pos/3;
		decal=0;
	}

	n=pos;
	LineSize--;
	gap=0;
	
	if (strand==Seq_strand_minus)
		szDNA=reverse_string(szDNA);

	size_pgp--;
	
	if (szTrans){
		start_prot=start+decal+1;
		stop++;
		while(start_prot<stop){
			limit=start_prot-StartLetter+gap+1;

			gap=0;
			for(i=0;i<limit;i++){
				if (szDNA[i]=='-') gap++;
			}
			if (szDNA[(strand==Seq_strand_minus ? limit : limit-1)]=='-'){
				while(szDNA[i]){
					if (szDNA[i]=='-') gap++;
					else break;
					i++;
				}
			}
			if (strand!=Seq_strand_minus){
				n2=start_prot-StartLetter+gap;
				n2+=(n2/LETTER_BLOCK_WIDTH);
			}
			else{
				n2=size_pgp-(start_prot-StartLetter+gap);
				n2+=(n2/LETTER_BLOCK_WIDTH);
			}
			if (n2>LineSize) break;
			if (str[n])
				szTrans[n2]=str[n];
			else 
				break;
			start_prot+=3;
			n++;
		}
	}
	if (str){
		MemFree (str);
	}
}


NLM_EXTERN void DDV_DispFeaturesForPGP(ParaGPtr pgp,BioseqPtr bsp,CharPtr PNTR szFLine,Int4 disp,
	Int2 LineSize, CharPtr szDNA, Uint1 strand)
{
SeqMgrFeatContext    context2;
SeqMgrFeatContextPtr context=NULL;
SeqMgrSegmentContext contextPart;
ValNodePtr           vnp4;
BioseqPtr            parent;
CharPtr              szTrans;
Int4                 from_bsp,to_bsp,start,stop,i,numivals2,i_decal;
Int2                 jj;
Uint2                iID,idx;
Boolean              bDispTrans=FALSE;

	/*the current bsp is just a segment ?*/
	parent=SeqMgrGetParentOfPart(bsp,&contextPart);
	context=NULL;
	szTrans=NULL;
	
	LineSize+=(LineSize/LETTER_BLOCK_WIDTH);
	szTrans=(CharPtr)MemNew(LineSize*sizeof(Char));

	if (szTrans==NULL) return;

	/*for each feature...*/
	for(jj=0,vnp4=pgp->pFeatList;jj<pgp->nFeat;jj++,vnp4=vnp4->next){
		
		if (vnp4 == NULL) break;
		
		/*...get desired feature given iID and idx...*/
		UDV_BigDecodeIdxFeat ((Uint8)vnp4->data.bigintvalue, &iID,&idx,NULL,NULL);
		if (!SeqMgrGetDesiredFeature(0,
			(parent!=NULL ? parent : bsp),iID,idx,NULL,&context2)) continue;
		/*...then copy the original context feature data ...*/
		if (context) {
			MemFree(context->ivals);
			context=MemFree(context);
		}
		if (context) {
			MemFree(context->ivals);
			context=MemFree(context);
		}
		/*...before I modify it*/
		context=UDV_ConvertFeatContext(&context2,contextPart.cumOffset,
			bsp->length);
		if (!context) continue;
		/*temporary situation; will be modified in the future*/
		if (context->strand>Seq_strand_minus ||
			context->strand==Seq_strand_unknown) context->strand=Seq_strand_plus;

		/*if circular DNA with feature overlapping the junction*/
		if(context->left<0 && context->bsp->topology==TOPOLOGY_CIRCULAR){
			/*technically, just for the purpose of a display, a feature
			overlapping the ends of a circular DNA covers all the sequence; that is
			because I ALWAYS display from LEFT to RIGHT.*/
			context->left=0;
			context->right=context->bsp->length-1;
			
			/*strand PLUS*/
			if (context->strand==Seq_strand_plus){
				numivals2=2*context->numivals-2;
				i=numivals2;
				i_decal=-2;
			}

			/*strand MINUS*/
			if (context->strand==Seq_strand_minus){
				numivals2=context->numivals*2;
				i=0;
				i_decal=2;
			}		
		}
		else{
			if (context->left<0){
				context->left=0;
			}
			/*strand PLUS*/
			if (context->strand==Seq_strand_plus){
				numivals2=context->numivals*2;
				i=0;
				i_decal=2;
			}

			/*strand MINUS*/
			if (context->strand==Seq_strand_minus){
				numivals2=2*context->numivals-2;
				i=numivals2;
				i_decal=-2;
			}		
		}
		
		MemSet(szTrans,'.',(LineSize-1)*sizeof(Char));
		
		while (TRUE){
			/*get the bsp coords for the current ParaG*/
			UDV_ComputeBspCoordRangeinPGP(pgp,&from_bsp,&to_bsp);
			/*the ParaG only contains a gap ?*/
			if (from_bsp==INT4_MAX && to_bsp==INT4_MIN) break;
			/*if ivals.stop > end ParaG -> end of drawing*/
			if (context->ivals[i]>to_bsp) break;
			/*if ivals.stop<= start ParaG : not yet in the current ParaG*/
			if (context->ivals[i+1]<from_bsp) {
				if (context->strand==Seq_strand_plus || 
						context->strand==Seq_strand_unknown){
					i=i+i_decal;
					if (i>numivals2-2 || i<0) break;
					else continue;
				}					
				if (context->strand==Seq_strand_minus){
					i=i+i_decal;
					if (i<0) break;
					else continue;
				}
			}
			/*...only display CDS */
			if(context->featdeftype==FEATDEF_CDS){
				/*compute the limits of the feature within ParaG*/
				start=MAX(context->ivals[i],from_bsp);
				stop=MIN(context->ivals[i+1],to_bsp);
				if (start>stop) continue;
				if (context->strand!=Seq_strand_minus)
					DDV_WWW_get_CDS_plus(context,start,
						stop,from_bsp,pgp->StopLetter-pgp->StartLetter,
						i,szDNA,szTrans,LineSize,strand);
				/*else
					DDV_WWW_get_CDS_minus(context,start,
						stop,from_bsp,i,szDNA,szTrans,LineSize);*/
				bDispTrans=TRUE;
			}/*end if(CDS)*/
			if (context->strand==Seq_strand_plus || 
						context->strand==Seq_strand_unknown){
				i=i+i_decal;
				if (i>numivals2-2 || i<0) break;
			}
			if (context->strand==Seq_strand_minus){
				i=i+i_decal;
				if (i<0) break;
			}
		}/*end while()*/
		if (bDispTrans){
			*szFLine=DDV_ConcatStr(*szFLine,"\n");
			DDV_AddBlank2(disp,szFLine);
			*szFLine=DDV_ConcatStr(*szFLine,szTrans);
			bDispTrans=FALSE;
		}
	}/*end for()*/
}	

/*******************************************************************************

  Function : DDV_ResetParaGSeqAlignCoord()
  
  Purpose : this function reset the SeqAlign coords in the display data, ParaG
  
  Parameters : mpplp;main data block used for the display
  				LineSize; max size of each ParaG. Must be the same value as
				the one passed in DDV_PopDisplay().
  
  Return value : none 

*******************************************************************************/
NLM_EXTERN void DDV_ResetParaGSeqAlignCoord(MsaParaGPopListPtr mpplp,Int2 LineSize)
{
Int4 i,decal;
Int2 OldLineSize;
ValNodePtr vnp;
ParaGPtr pgp;

	for(i=0;i<mpplp->nBsp;i++){
		vnp=mpplp->TableHead[i];
		decal=0;
		while(vnp){
			pgp=(ParaGPtr)(vnp->data.ptrvalue);
			if (pgp){
				/*the last ParaG of a list may be smaller than LineSize*/
				/*So, I get the current ParaG size and reuse it two lines below*/
				OldLineSize=pgp->StopLetter-pgp->StartLetter+1;
				pgp->StartLetter=decal;
				pgp->StopLetter=pgp->StartLetter+_min_(LineSize,OldLineSize)-1;
			}
			decal+=(Int4)LineSize;
			vnp=vnp->next;
		}
	}
}

/*******************************************************************************

  Function : DDV_PopDisplay2()
  
  Purpose : (re)populate ParaGs from the indexed seqalign
  
  Parameters : 	sabp;header of the indexed seqalign
  				mpplp; where ParaGs list are stored
  				nBSP;sabp contains 'nBsp' sequences
				MasterScaleStyle;scale stype for the master sequence
				SlaveScaleStyle;scale stype for the slave sequence
				cbl; number or char/line for a each ParaG
				nBlockToPop; number of blocks to analyse
				ali_from; create ParaG list given a from-to in SEQALIGN COORD
				ali_to;create ParaG list given a from-to in SEQALIGN COORD
				
  Return value : TRUE if success

*******************************************************************************/
NLM_EXTERN Boolean DDV_PopDisplay2(SABlockPtr sabp,MsaParaGPopListPtr mpplp,
			Int4 nBSP,Uint1 MasterScaleStyle,Boolean ShowTick,
			Int2 cbl,Int4 nBlockToPop,Int4 ali_from,Int4 ali_to)
{
SABlockPtr 	sabp2;	/*used to scan "SeqALign" Index blocks*/
SegmentPtr 	segp;	/*used to scan each Bsp segment in a block*/
Int4 		startPgpPos2;/*used to set the from value of each ParaG*/
Int4 		startcopy;/*used to copy the content of a segment in a ParaG*/
Int4 		stopcopy;/*idem*/
Int4 		segsize;/*idem*/
BoolPtr 	bFirst;/*idem*/
Int4 		nCharToCopy;/*idem*/
Int4 		countSegMax=0;/*used to count the number of sequence to display*/
Int4 		countSeg=0;/*idem*/
Int4		countBlock=0;
Int4 		AbsLengthAli=0;/*used to compute the size of the align to display*/
Int4 		i;/*just a little counter*/
ValNodePtr 	vnp,vnp2;/*used to delete additional ParaG nodes, if needed;
				used for REpopulation*/
ParaGPtr pgp1,pgp2;/*used to create new ParaGs*/
MsaTxtDispPtr mtdp;/*used to store the content of a line in a ParaG*/
ValNodePtr PNTR TableDesc=NULL;/*used to populate mpplp->TableHead*/
ValNodePtr PNTR TableText=NULL;/*used to populate ParaG with Text*/
Int4		gFrom,gTo,gDecal_L,gDecal_R;/*used to prepare a display from a range*/

/*
#ifdef DEBUG_DDV
	blk_PrintSABP(sabp);
#endif
*/
	/*some initialisations*/
	if (mpplp->TableHead==NULL){/*First time*/
		mpplp->TableHead=(ValNodePtr PNTR)MemNew(nBSP*sizeof(ValNodePtr));
		if (mpplp->TableHead==NULL) return(FALSE);
		MemSet(mpplp->TableHead,0,nBSP*sizeof(ValNodePtr));
	}
	else{/*reuse the structures*/
		/*same number of BSPs ?*/
		if (nBSP>mpplp->nBsp){/*increase the table*/
			ValNodePtr PNTR tmp;
			tmp=(ValNodePtr PNTR)MemNew(nBSP*sizeof(ValNodePtr));
			if (tmp==NULL) return(FALSE);
			MemSet(tmp,0,nBSP*sizeof(ValNodePtr));
			MemCopy(tmp,mpplp->TableHead,mpplp->nBsp*sizeof(ValNodePtr));
			MemFree(mpplp->TableHead);
			mpplp->TableHead=tmp;
		}
	}
	/*used to populate mpplp->TableHead*/
	TableDesc=(ValNodePtr PNTR)MemNew(nBSP*sizeof(ValNodePtr));
	if (TableDesc==NULL) return(FALSE);
	/*copy the ValNode heads in TableDesc*/
	for(i=0;i<nBSP;i++){
		TableDesc[i]=mpplp->TableHead[i];
	}

	bFirst=(BoolPtr)MemNew(nBSP*sizeof(BoolPtr));
	if (bFirst==NULL) return(FALSE);
	MemSet(bFirst,1,nBSP*sizeof(BoolPtr));

	/*used to populate ParaG with Text*/
	TableText=(ValNodePtr PNTR)MemNew(nBSP*sizeof(ValNodePtr));
	if (TableText==NULL) return(FALSE);
	MemSet(TableText,0,nBSP*sizeof(ValNodePtr));

	/*scan each block of the "SeqALign" Index*/
	for(sabp2=sabp ; sabp2!=NULL ; sabp2=sabp2->next){
		if (!sabp2->visible) continue;/*used only visible block*/
		if(ali_from!=(-1) && ali_to!=(-1)){/*use a range*/
			if (ali_from>sabp2->to) continue;/*not yet in range*/
			gFrom=_max_(ali_from,sabp2->from);
			gTo=_min_(ali_to,sabp2->to);
			gDecal_L=gFrom-sabp2->from;
			gDecal_R=sabp2->to-gTo;
		}
		else{/*display the full alignment*/
			gFrom=sabp2->from;
			gTo=sabp2->to;
			gDecal_L=gDecal_R=0;
		}
		/*used to compute the total align length to display*/
		AbsLengthAli+=(gTo-gFrom+1);
		/*used to compute the total number of bsp to display*/
		countSeg=0;
		countBlock++;
		/*scan each Bsp segment in a block*/
		for(segp=sabp2->segp_head ; segp!=NULL ; segp=segp->next){
			if (!segp->visible){/*hide sequence*/
				/*TableDesc[segp->BspID-1]=NULL;*/
				continue;
			}

			countSeg++;
			/*allocation ?*/
			if (TableDesc[segp->BspID-1]==NULL){
				mpplp->TableHead[segp->BspID-1]=ValNodeNew(NULL);
				TableDesc[segp->BspID-1]=
					mpplp->TableHead[segp->BspID-1];
			}
			/*is this segment a gap ?*/
			if (segp->gap==0) {/*no*/
				if(segp->strand!=Seq_strand_minus){
					startcopy=segp->from+gDecal_L;/*use Bsp coordinates*/
					stopcopy=segp->to+1-gDecal_R;/*use Bsp coordinates*/
				}
				else{
					startcopy=segp->to-gDecal_L;/*use Bsp coordinates*/
					stopcopy=segp->from-1+gDecal_R;/*use Bsp coordinates*/
				}
			}
			else {/*yes*/
				startcopy=gFrom;/*use Align coordinates*/
				stopcopy=gTo+1;/*use Align coordinates*/
			}
			startPgpPos2=gFrom;
			/*retrieve a ParaG*/
			pgp1=(ParaGPtr)TableDesc[segp->BspID-1]->data.ptrvalue;
			/*copy segment data to ParaG*/
			while(((segp->strand!=Seq_strand_minus || segp->gap!=0) ? 
					(startcopy<stopcopy) :
					(startcopy>stopcopy))){
				if (bFirst[segp->BspID-1]){
					bFirst[segp->BspID-1]=FALSE;
					/*is this ParaG already exist*/
					if (pgp1==NULL){/*no: create it*/
						pgp1=(ParaGPtr)MemNew(sizeof(ParaG));
						if (pgp1==NULL) return(FALSE);
						MemSet(pgp1,0,sizeof(ParaG));
						TableDesc[segp->BspID-1]->data.ptrvalue=(Pointer)pgp1;
					}

					/*is the Text fields exist ?*/
					if(pgp1->ptxtList==NULL){
						pgp1->ptxtList=ValNodeNew(NULL);
						if (!pgp1->ptxtList) return(FALSE);
					}
					TableText[segp->BspID-1]=pgp1->ptxtList;

					pgp1->StartLetter=pgp1->StopLetter=startPgpPos2;
					if (segp->BspID-1==0){/*master sequence*/
						pgp1->ScaleStyle=MasterScaleStyle;
						if (MasterScaleStyle&SCALE_POS_LEFT){ 
							pgp1->nLines=1;
						}
						else 
							if (MasterScaleStyle&SCALE_POS_TOP) {
								pgp1->nLines=2;
							}
						else 
							if (MasterScaleStyle&SCALE_POS_BOTH) {
								pgp1->nLines=2;
							}
						else 
							if (MasterScaleStyle&SCALE_POS_NONE) {
								pgp1->nLines=0;
							}
						if (ShowTick) pgp1->nLines++;
					}
					else{
						pgp1->nLines=1;
					}
					pgp1->sip=segp->sip;
				}
				else{
					if (!TableText[segp->BspID-1]->next){
						TableText[segp->BspID-1]->next=
							ValNodeAdd(&(TableText[segp->BspID-1]));
						if (!(TableText[segp->BspID-1]->next)) return(FALSE);
					}
					TableText[segp->BspID-1]=TableText[segp->BspID-1]->next;
				}
				if (segp->strand!=Seq_strand_minus || segp->gap!=0) 
					segsize=stopcopy-startcopy;
				else
					segsize=startcopy-stopcopy;
				
				nCharToCopy=MIN(segsize,cbl-(pgp1->StopLetter-pgp1->StartLetter));
				pgp1->StopLetter+=nCharToCopy;
				/*init a new block of info*/
				mtdp=(MsaTxtDispPtr)TableText[segp->BspID-1]->data.ptrvalue;
				if(!mtdp){
					mtdp=(MsaTxtDispPtr)MemNew(sizeof(MsaTxtDisp));
					if (mtdp==NULL) return(FALSE);
					TableText[segp->BspID-1]->data.ptrvalue=(Pointer)mtdp;
				}
				if (segp->strand!=Seq_strand_minus || segp->gap!=0){
					mtdp->from=startcopy;
					mtdp->to=startcopy+nCharToCopy-1;
				}
				else{
					mtdp->from=startcopy-nCharToCopy+1;
					mtdp->to=startcopy;
				}
				mtdp->SegID=sabp2->SegID;
				mtdp->BspID=segp->BspID;
				mtdp->IsGap=(segp->gap==0 ? FALSE : TRUE);
				mtdp->strand=segp->strand;
				if (mtdp->IsGap) mtdp->TextStyle=MSA_TXT_STYLE_GAP;
				else mtdp->TextStyle=MSA_TXT_STYLE_SEQ;
				
				/*add a next node if needed*/
				/*if (!TableText[segp->BspID-1]->next){
					TableText[segp->BspID-1]->next=
						ValNodeAdd(&(TableText[segp->BspID-1]));
					if (!(TableText[segp->BspID-1]->next)) return(FALSE);
				}
				TableText[segp->BspID-1]=TableText[segp->BspID-1]->next;*/
				pgp2=pgp1;
				vnp=TableDesc[segp->BspID-1];
				/*switch to a next ParaG*/
				if (pgp1->StopLetter-pgp1->StartLetter>=cbl){
					pgp1->StopLetter--;/*swith to zero-based*/
					if (!TableDesc[segp->BspID-1]->next){
						TableDesc[segp->BspID-1]->next=
							ValNodeAdd(&(TableDesc[segp->BspID-1]));
						if (!(TableDesc[segp->BspID-1]->next)) return(FALSE);
					}
					TableDesc[segp->BspID-1]=
						TableDesc[segp->BspID-1]->next;
					vnp2=TableDesc[segp->BspID-1];
					/*retrieve a ParaG*/
					pgp1=(ParaGPtr)TableDesc[segp->BspID-1]->data.ptrvalue;
					bFirst[segp->BspID-1]=TRUE;
					startPgpPos2+=nCharToCopy;
					/*delete additional ParaG->ptxtList if needed*/
					if (TableText[segp->BspID-1]->next){
						DDV_DeleteTxtList(&(TableText[segp->BspID-1]->next));
					}
				}
				else{
					vnp2=TableDesc[segp->BspID-1]->next;
				}
				/*used to compute the total number of bsp to display*/
				if (segp->strand!=Seq_strand_minus || segp->gap!=0)
					startcopy+=nCharToCopy;
				else
					startcopy-=nCharToCopy;
			}/*end while(segment)*/
			/*just in case: switch ParaG-> to zero based*/
			if (countBlock==nBlockToPop){
				if (pgp2 && ((pgp2->StopLetter-pgp2->StartLetter+1)>=AbsLengthAli)) {
					pgp2->StopLetter=pgp2->StartLetter+AbsLengthAli-1;
				}
				/*erase additional ParaG if needed*/
				if (vnp2){
					DDV_DeleteParaGList(&vnp2);
				}
				vnp->next=NULL;
				if (TableText[segp->BspID-1]->next){
					DDV_DeleteTxtList(&TableText[segp->BspID-1]->next);
				}
			}
		}/*end for (segment)*/
		if (countSeg>countSegMax) countSegMax=countSeg;
		if (ali_from!=(-1) && ali_to!=(-1)){/*out of range ?*/
			if (ali_to<=sabp2->to) break;
		}
	}/*end for(block)*/

	/*hack : sometimes a NULL node is added at the end: remove it*/
	for(i=0;i<nBSP;i++){
		vnp=mpplp->TableHead[i];
		while(vnp){
			vnp2=vnp->next;
			if (vnp2 && vnp2->data.ptrvalue==NULL){
				ValNodeFree(vnp2);
				vnp->next=NULL;
			}
			vnp=vnp->next;
		}
	}
	
	/*size of the align to display*/
	mpplp->LengthAli=AbsLengthAli;
	mpplp->nBsp=countSeg;
	MemFree(bFirst);	
	MemFree(TableDesc);
	MemFree(TableText);
	return(TRUE);
}
	/*small version of DDV_PopDisplay2*/
NLM_EXTERN Boolean DDV_PopDisplay(SABlockPtr sabp,MsaParaGPopListPtr mpplp,
			Int4 nBSP,Uint1 MasterScaleStyle,Boolean ShowTick,
			Int2 cbl,Int4 nBlockToPop)
{
	return(DDV_PopDisplay2(sabp, mpplp,nBSP, MasterScaleStyle, ShowTick,
			 cbl, nBlockToPop,-1,-1));
}

/*******************************************************************************

  Function : GetClrFromClrGlobal()
  
  Purpose : get the color of a letter using ColorManager
  				
  Return value : - (see szColor)

*******************************************************************************/
static void GetClrFromClrGlobal(DDV_ColorGlobal * gclr,Int4 bsp_coord,
	SeqIdPtr sip, CharPtr szColor,Int4 row, Uint1Ptr style)
{
DDV_ColorCell * dclrp;
Uint4 i,idx;
Uint1 rgb1[4],rgb2[4];	
Boolean bFound;

	dclrp=DDV_GetColor(gclr,NULL, row, bsp_coord);
	bFound=FALSE;
	(*style)=0;
	
	if (dclrp){
		rgb1[0]=dclrp->rgb[0];
		rgb1[1]=dclrp->rgb[1];
		rgb1[2]=dclrp->rgb[2];
		rgb1[3]=0;
		for(i=0;i<DDV_COLOR_MAX;i++){
			rgb2[0]=DDV_PaletteRGB[i][0];
			rgb2[1]=DDV_PaletteRGB[i][1];
			rgb2[2]=DDV_PaletteRGB[i][2];
			rgb2[3]=0;
			if (*((Uint4Ptr)rgb1 )== *((Uint4Ptr)rgb2)){
				idx=i;
				bFound=TRUE;
				break;
			}
		}
		if (dclrp->UseItalic)
			(*style)|=DDV_TXTSTYLE_ITALIC;
		if (dclrp->UseBold)
			(*style)|=DDV_TXTSTYLE_BOLD;
		if (dclrp->UseUnderline)
			(*style)|=DDV_TXTSTYLE_UNDERLINE;
	}
	
	if (!bFound){
		idx=DDVCOL_BLACK;
	}
	StringCpy(szColor,DDV_PaletteRGB_NAV[idx]);
}

/*******************************************************************************

  Function : GetClrFromLetter()
  
  Purpose : get the colour of a letter (na or aa)
  
  Parameters : 	residue; the letter
  				IsAA; True when prot
				szColor; HTML encoded color
				
  Return value : - (see szColor)

*******************************************************************************/
static void GetClrFromLetter(Char residue,Boolean IsAA,CharPtr szColor)
{

	/*AA colour code system*/
	if(IsAA){/*not yet implemented*/
		StringCpy(szColor,DDV_PaletteRGB_NAV[DDVCOL_BLACK]);
	}
	else{/*NA colour code*/
		switch(residue){
			case '-':
				StringCpy(szColor,DDV_PaletteRGB_NAV[DDVCOL_PINK]);
				break;
			case '.':
				StringCpy(szColor,DDV_PaletteRGB_NAV[DDVCOL_BACKGROUND]);
				break;
			case 'A':case 'a':
				StringCpy(szColor,DDV_PaletteRGB_NAV[DDVCOL_GREEN]);
				break;
			case 'G':case 'g':
				StringCpy(szColor,DDV_PaletteRGB_NAV[DDVCOL_BLACK]);
				break;
			case 'C':case 'c':
				StringCpy(szColor,DDV_PaletteRGB_NAV[DDVCOL_BLUE]);
				break;
			case 'T':case 't':
				StringCpy(szColor,DDV_PaletteRGB_NAV[DDVCOL_RED]);
				break;
			case 'N': case 'n':
				StringCpy(szColor,DDV_PaletteRGB_NAV[DDVCOL_HOTPINK]);
				break;
			default:
				StringCpy(szColor,DDV_PaletteRGB_NAV[DDVCOL_BLACK]);
				break;
		}		
	}
}


#define DDV_DumpStyles(_newStyle,_newColor,_boldOpen,_italicOpen,_underlineOpen,_colorOpen,_szSequence,_szBuf)\
{\
	if (_newStyle&DDV_TXTSTYLE_BOLD){\
		_szSequence=DDV_ConcatStr(_szSequence,"<B>");\
		_boldOpen=TRUE;\
	}\
	if (_newStyle&DDV_TXTSTYLE_ITALIC){\
		_szSequence=DDV_ConcatStr(_szSequence,"<I>");\
		_italicOpen=TRUE;\
	}\
	if (_newStyle&DDV_TXTSTYLE_UNDERLINE){\
		_szSequence=DDV_ConcatStr(_szSequence,"<U>");\
		_underlineOpen=TRUE;\
	}\
	_szSequence=DDV_ConcatStr(_szSequence,"<FONT COLOR=#");\
	_colorOpen=TRUE;\
	_szSequence=DDV_ConcatStr(_szSequence,_newColor);\
	_szSequence=DDV_ConcatStr(_szSequence,">");\
	_szSequence=DDV_ConcatStr(_szSequence,_szBuf);\
	if (_colorOpen){\
		_szSequence=DDV_ConcatStr(_szSequence,"</FONT>");\
		_colorOpen=FALSE;\
	}\
	if (_underlineOpen){\
		_szSequence=DDV_ConcatStr(_szSequence,"</U>");\
		_underlineOpen=FALSE;\
	}\
	if (_italicOpen){\
		_szSequence=DDV_ConcatStr(_szSequence,"</I>");\
		_italicOpen=FALSE;\
	}\
	if (_boldOpen){\
		_szSequence=DDV_ConcatStr(_szSequence,"</B>");\
		_boldOpen=FALSE;\
	}\
}
/*******************************************************************************

  Function : DDV_Print_Sequence()
  
  Purpose : display a sequence
  
  Parameters : 	szSeq; the sequence to display
  				bUseColour; print out a coloured sequence
  				IsAA; True is szSeq is a prot
				fp;file to put in the sequence

  Note: this function can be used to display both Ticks and Numbers for
        the top scale. See the 'what' parameter
				
  Return value : -

*******************************************************************************/
static CharPtr  DDV_Print_Sequence(CharPtr szSeq,Boolean bUseColour,Boolean IsAA,
		ParaGPtr pgp, DDV_ColorGlobal * gclr,Int4 row)
{
CharPtr szBuf,szTmp,szSequence=NULL;
Int4    pos,taille,stop,bsp_coord;
Char    residue;					
Uint2   nCompt=0,nCompt2=0;
Char    curColor[10],newColor[10]={""};
Uint1   curStyle,newStyle;
Boolean boldOpen,italicOpen,underlineOpen,colorOpen;
	
	/*alloc a buffer*/
	taille=StringLen(szSeq)+1;
	szBuf=(CharPtr)MemNew(taille*sizeof(Char));
	if (!szBuf) {
		szSequence=StringSave(szSeq);
		return(szSequence);
	}
	MemSet(szBuf,0,taille*sizeof(Char));
	
	/*prepare the colour display*/
	pos=1;	
	stop=taille+1;
	newStyle=curStyle=0;
	boldOpen=italicOpen=underlineOpen=colorOpen=FALSE;
	/*draw!*/
	if (!bUseColour){/*black-colour display*/
		szTmp=StringChr(szSeq,'\0');
		if (szTmp && *(szTmp-1)==' ') *(szTmp-1)='\0';
		szSequence=StringSave(szSeq);
	}
	else{ /*use colour-coded alphabet fo NA & AA*/
		while(pos<stop){
			residue=szSeq[nCompt];
			/*retrieve "standard" colors if Color Manger is not used*/
			if (gclr==NULL){
				GetClrFromLetter(residue,IsAA,curColor);
			}
			else{/*use Color Manager to get the colors*/
				bsp_coord=DDV_GetBspCoordGivenDispCoord(pgp,pgp->StartLetter+nCompt);
				if (bsp_coord!=(Int4)-1)/*bsp letter*/
					GetClrFromClrGlobal(gclr,bsp_coord,pgp->sip,curColor,row,&curStyle);
				else/*gap*/{
					StringCpy(curColor,DDV_PaletteRGB_NAV[DDVCOL_BLACK]);
					curStyle=0;
				}
					
			}
			/*new colour? or new style?*/
			if (StringCmp(curColor,newColor) || curStyle!=newStyle){
				if (szBuf[0]!='\0'){/*something to draw ?*/
					szBuf[nCompt2]='\0';/*CLOSE THE STRING*/
					DDV_DumpStyles(newStyle,newColor,boldOpen,italicOpen,
						underlineOpen,colorOpen,szSequence,szBuf);
				}
				StringCpy(newColor,curColor);
				newStyle=curStyle;
				nCompt2=0;
				szBuf[nCompt2]='\0';
			}
			szBuf[nCompt2++]=residue;
			
			/*each LETTER_BLOCK_WIDTH, add a empty column*/
			pos++;nCompt++;		
		}
		if (szBuf[0]!='\0'){/*something to draw ?*/
			szBuf[nCompt2-1]='\0';/*CLOSE THE STRING*/
			DDV_DumpStyles(newStyle,newColor,boldOpen,italicOpen,
				underlineOpen,colorOpen,szSequence,szBuf);
		}
	}
	MemFree(szBuf);
	return(szSequence);
}

/*******************************************************************************

  Function : DDV_DisplayTopScale()
  
  Purpose : display a Top scale above the first sequence of the alignment
  
  Parameters : 	from; from 
  				to;  to ... I have no other explanation !
  				offset;used to locate correctly the ruler
				disp_format; display format options
				what;tick or number

  Note: this function can be used to display both Ticks and Numbers for
        the top scale. See the 'what' parameter
				
  Return value : -

*******************************************************************************/
static CharPtr DDV_DisplayTopScale(Int4 from,Int4 to,Int4 offset,Uint4 disp_format,
	Uint1 what)
{
CharPtr	szText,szScale=NULL;
Int4 	pos,i,stop,size,j,k;
Char 	szPos[15]={""};	
Char 	szBlank[TABLE_LEFT_MARGIN+5]={""};	

	size=to-from+1;
	pos=from+1;
	stop=to+2;
	i=0;
	if (disp_format&DISPE_SHOWBLOCK) size+=(size/LETTER_BLOCK_WIDTH);
	szText=(CharPtr)MemNew((size+1)*sizeof(Char));
	if (szText){
		MemSet(szText,' ',size*sizeof(Char));
		while(pos<stop){
			if (!(pos % LETTER_BLOCK_WIDTH)){
				switch(what){
					case SCALE_TICK:
						szText[i]='|';
						break;
					case SCALE_NUM:
						sprintf(szPos,"%i",pos);
						j=StringLen(szPos);
						for(k=0;k<j;k++){
							szText[i-j+k+1]=szPos[k];
						}
						break;
				}
				if (disp_format&DISPE_SHOWBLOCK) i++;
			}
			if (!(pos % LETTER_HALF_BLOCK_WIDTH) && 
				(pos % LETTER_BLOCK_WIDTH) && (what==SCALE_TICK)){
				szText[i]='.';
			}
			pos++;i++;
		}
		if (what==SCALE_NUM) 
			szScale=DDV_ConcatStr(szScale,"\n");
		if(disp_format&DISP_FULL_HTML){
			szScale=DDV_ConcatStr(szScale,"<font color=#700777>");
			DDV_AddBlank2(offset,&szScale);
			szScale=DDV_ConcatStr(szScale,szText);
			szScale=DDV_ConcatStr(szScale,"</font>\n");
		}else if(disp_format&DISP_FULL_TXT){
			DDV_AddBlank2(offset,&szScale);
			szScale=DDV_ConcatStr(szScale,szText);
			szScale=DDV_ConcatStr(szScale,"\n");
		}
		MemFree(szText);
	}
	return(szScale);
}

/*******************************************************************************

  Function : DDV_GetSequenceFromParaG()
  
  Purpose : retrieve a sequence from a ParaG 
  
  Parameters : pgp ; a ParaG
				szSequence; filled here
				bspLength; size of the sequence
				IsAA; true if the sequence is a protein
  				bsp_strand; strand of the ParaG
				bsp_start; from
				bsp_stop; to of the Parag (bsp coord)

  Note : bsp_strand,bsp_start,bsp_stop ; pointers can be NULL if you don't want
  		to retrieve that information.

  Return value : TRUE if success; see also bsp_strand,bsp_start,bsp_stop

*******************************************************************************/
NLM_EXTERN Boolean DDV_GetSequenceFromParaG(ParaGPtr pgp,
	CharPtr PNTR szSequence,Int4 bspLength,Boolean IsAA,Uint1 PNTR bsp_strand,
	Int4 PNTR bsp_start,Int4 PNTR bsp_stop)
{
Int4          size,from,to,ret,seek;
CharPtr       SeqBuf;
ValNodePtr    vnp2;
MsaTxtDispPtr mtdp;
SeqLocPtr     slp=NULL;
SeqPortPtr    spp=NULL;
Boolean       bOk=FALSE;
SeqIdPtr      sip;
Uint1		  strand=Seq_strand_unknown;

	if (!szSequence || !pgp) 
		return(FALSE);

	/*analyse the sequence description; get the first block
	which describes a Bioseq, not a gap; then retrieves the from*/
	from=INT4_MAX;
	to=INT4_MIN;
	for(vnp2=pgp->ptxtList;vnp2!=NULL;vnp2=vnp2->next){
		mtdp=(MsaTxtDispPtr)(vnp2->data.ptrvalue);
		if (mtdp){
			if (mtdp->IsGap==0) {
				strand=mtdp->strand;
				from=MIN(from,mtdp->from);
				to=MAX(to,mtdp->to);
				if (!bOk) bOk=TRUE;
				/*break;*/
			}
		}
	}

	
	/*if bOk==FALSE, we have only a big gap to display*/
	if (bOk){
		/*open a seqport on the ParaG*/
		sip=SeqIdFindBest(pgp->sip,SEQID_GI);
		if (sip==NULL) sip=pgp->sip;
		slp=SeqLocIntNew(from,to,strand, sip);
		if (!slp) return(FALSE);
		spp = SeqPortNewByLoc (slp, (Uint1)(IsAA==TRUE ? Seq_code_iupacaa 
				: Seq_code_iupacna));
		if (!spp) return(FALSE);
	}

	/*build the text to display for the ParaG: AATAT---ATAT--AAA*/
	for(vnp2=pgp->ptxtList;vnp2!=NULL;vnp2=vnp2->next){
		mtdp=(MsaTxtDispPtr)vnp2->data.ptrvalue;
		if (mtdp){
			Char c;
			size=mtdp->to-mtdp->from+1;
			SeqBuf=(CharPtr)MemNew((size+1)*sizeof(Char));
			if (!SeqBuf) continue;
			if(mtdp->IsGap){
				switch(mtdp->TextStyle){
					case MSA_TXT_STYLE_1:
						c=' ';
						break;
					case MSA_TXT_STYLE_UAGAP:
						c='~';
						break;
					case MSA_TXT_STYLE_GAP:
					case MSA_TXT_STYLE_2:
                                        default:
						c='-';
						break;
				}
				MemSet(SeqBuf,c,size*sizeof(Char));
				SeqBuf[size]='\0';
			}
			else{
				if (strand == Seq_strand_minus)
					seek = to-mtdp->to;
				else
					seek = mtdp->from-from;
				SeqPortSeek(spp, seek, SEEK_SET);
				ret=SeqPortRead (spp,(Uint1Ptr)SeqBuf, (Int2)size);
				if (ret!=size){
					MemSet(SeqBuf,'?',size*sizeof(Char));
					SeqBuf[size]='\0';
				}
			}
			StringCat(*szSequence,SeqBuf);
			MemFree(SeqBuf);
			SeqBuf=NULL;
			
		}
	}

	if (spp) SeqPortFree (spp);
	if (slp) SeqLocFree (slp);

	if (bsp_strand) *bsp_strand=strand;
	if (bsp_start) *bsp_start=from;
	if (bsp_stop) *bsp_stop=to;

	return(TRUE);
}


NLM_EXTERN Int4 DDV_GetBspCoordGivenDispCoord(ParaGPtr pgp, Int4 disp_pos) {
/*****************************************************************************
*  same as DDV_GetBspCoordGivenDispCoord2, except alignment status is ignored
*****************************************************************************/
  Boolean  UnAligned;
  return(DDV_GetBspCoordGivenDispCoord2(pgp, disp_pos, &UnAligned));
}


/*****************************************************************************

Function: DDV_GetBspCoordGivenDispCoord2()

Purpose: compute a BSP coord given a display coord and a ParaG 

Return values: BSP coord
               Alignment status of this coord in pUnAligned

*****************************************************************************/
NLM_EXTERN Int4 DDV_GetBspCoordGivenDispCoord2(ParaGPtr pgp, Int4 disp_pos, Boolean* pUnAligned)
{
MsaTxtDispPtr  mtdp;
ValNodePtr     vnp;
Int4           pgp_start,diff,bsp_coord;

	bsp_coord=(Int4)-1;
    if (!pgp)
      return bsp_coord;
	pgp_start=pgp->StartLetter;
	vnp=pgp->ptxtList;
	while(vnp){
		mtdp=(MsaTxtDispPtr)vnp->data.ptrvalue;
		diff=mtdp->to-mtdp->from;
		if (disp_pos>=pgp_start && disp_pos<=(pgp_start+diff)){
			if (mtdp->TextStyle == MSA_TXT_STYLE_SEQ){
				bsp_coord=mtdp->from+(disp_pos-pgp_start);
				if(mtdp->strand==Seq_strand_minus){
					bsp_coord=mtdp->to-(bsp_coord-mtdp->from);
				}
        *pUnAligned = mtdp->IsUnAligned;
			}
			else{
				bsp_coord=(Int4)-1;
			}
			break;
		}
		pgp_start+=diff+1;
		vnp=vnp->next;
	}

	return(bsp_coord);
}


/*****************************************************************************

Function: DDV_GetBspCoordGivenPgpList()

Purpose: get a bsp_coord given a list of ParaG (for example a row in a SeqAlign)
         and a display coordinates.
		 
Return value: bsp coord

*****************************************************************************/
NLM_EXTERN Int4 DDV_GetBspCoordGivenPgpList(ValNodePtr ParaG_List,Int4 disp_pos)
{
ValNodePtr vnp;
ParaGPtr   pgp,TheParaG;
Boolean    bFound;
Int4       bsp_coord;

	bFound=FALSE;
	vnp=ParaG_List;
	bsp_coord=(Int4)-1;
	while(vnp){
		pgp=(ParaGPtr)vnp->data.ptrvalue;
		if (disp_pos>=pgp->StartLetter && disp_pos<=pgp->StopLetter){
			bFound=TRUE;
			TheParaG=pgp;
			break;
		}
		vnp=vnp->next;
	}
	if (bFound){
		bsp_coord=DDV_GetBspCoordGivenDispCoord(TheParaG,disp_pos);
	}
	return(bsp_coord);
}


/*****************************************************************************

Function: DDV_GetDispCoordGivenBspCoord()

Purpose: compute a display coord given a Bioseq coord and the ParaG list of
    a row (SeqAlign)

Return value: disp coord

*****************************************************************************/
NLM_EXTERN Int4 DDV_GetDispCoordGivenBspCoord(ValNodePtr vnp_head,Int4 bsp_pos)
{
MsaTxtDispPtr  mtdp;
ValNodePtr     vnp,vnp2;
ParaGPtr       pgp;
Int4           pgp_start,diff,disp_coord,bsp_start_pgp,bsp_stop_pgp;

	disp_coord=(Int4)-1;
	vnp2=vnp_head;
	while(vnp2){
		pgp=(ParaGPtr)vnp2->data.ptrvalue;
		UDV_ComputeBspCoordRangeinPGP(pgp,&bsp_start_pgp,&bsp_stop_pgp);
		if(bsp_start_pgp<=bsp_pos && bsp_pos<=bsp_stop_pgp){
			pgp_start=pgp->StartLetter;
			vnp=pgp->ptxtList;
			while(vnp){
				mtdp=(MsaTxtDispPtr)vnp->data.ptrvalue;
				diff=mtdp->to-mtdp->from;
				if (mtdp->TextStyle == MSA_TXT_STYLE_SEQ &&
				    (mtdp->from<=bsp_pos && bsp_pos<=mtdp->to)){
					if(mtdp->strand==Seq_strand_minus){
						disp_coord=pgp_start+(mtdp->to-bsp_pos);
					}
					else{
						disp_coord=pgp_start+(bsp_pos-mtdp->from);
					}
				}
				
				pgp_start+=diff+1;
				vnp=vnp->next;
			}
		}
		vnp2=vnp2->next;
	}
	return(disp_coord);
}


NLM_EXTERN Int4 DDV_GetFirstDispCoordGivenBspCoordRange(ValNodePtr vnp_head,
                                                        Int4 disp_start, Int4 disp_stop) {
/*----------------------------------------------------------------------------
*  get first valid display coord in the bioseq range
*---------------------------------------------------------------------------*/
  Int4  i, start, stop;
  Int4  RetVal;

  if ((disp_start < 0) || (disp_stop < 0)) return(-1);

  start = MIN(disp_start, disp_stop);
  stop = MAX(disp_start, disp_stop);

  for (i=start; i<=stop; i++) {
    RetVal = DDV_GetDispCoordGivenBspCoord(vnp_head, i);
    if (RetVal >= 0) {
      return(RetVal);
    }
  }
  return(-1);
}


NLM_EXTERN Int4 DDV_GetLastDispCoordGivenBspCoordRange(ValNodePtr vnp_head,
                                                       Int4 disp_start, Int4 disp_stop) {
/*----------------------------------------------------------------------------
*  get last valid display coord in the bioseq range
*---------------------------------------------------------------------------*/
  Int4  i, start, stop;
  Int4  RetVal;

  if ((disp_start < 0) || (disp_stop < 0)) return(-1);

  start = MIN(disp_start, disp_stop);
  stop = MAX(disp_start, disp_stop);

  for (i=stop; i>=start; i--) {
    RetVal = DDV_GetDispCoordGivenBspCoord(vnp_head, i);
    if (RetVal >= 0) {
      return(RetVal);
    }
  }
  return(-1);
}


/*******************************************************************************

  Function : DDV_GetFormattedSequence_1()
  
  Purpose : format a sequence given upper/lower case status for each letter
  
  Parameters : 	pgp; the paragraph containing the sequence szSeq.
                szSeq; the sequence
				gclr; contains the letters' layout data
				
  
*******************************************************************************/
static void DDV_GetFormattedSequence_1(ParaGPtr pgp,CharPtr szSeq,
		DDV_ColorGlobal * gclr,Int4 row)
{
DDV_ColorCell * dclrp;
Int4            i,dp;
Boolean         isAlpha;
		
	i=0;

	while(szSeq[i]){
		if (szSeq[i]!=' '){
			if (isalpha(szSeq[i])){
				isAlpha=TRUE;
				dp=DDV_GetBspCoordGivenDispCoord(pgp,pgp->StartLetter+i);
				if (dp==(Int4)-1)
					dclrp=NULL;
				else
					dclrp=DDV_GetColor(gclr,NULL, row, dp);
			}
			else {
				isAlpha=FALSE;
			}

			if (isAlpha && dclrp)
				szSeq[i]=(dclrp->LowerCase ? TO_LOWER(szSeq[i]) : szSeq[i]);
		}
		i++;
	}
}

/*******************************************************************************

  Function : DDV_GetBLASTCompLine_1()
  
  Purpose : get the middle line of a BLAST pairwise seqAlign
  
*******************************************************************************/
NLM_EXTERN CharPtr DDV_GetBLASTCompLine_1(CharPtr szQuery, 
                                          CharPtr szSubject, 
                                          Int4Ptr PNTR matrix, 
                                          Boolean is_aa)
{
    Int4    stop,pos;
    CharPtr szMiddleLine;
    Char    q,s;
    
    stop=StringLen(szSubject);

    if((szMiddleLine=(CharPtr)MemNew((stop+1)*sizeof(Char))) == NULL)
        return NULL;
    
    pos=0;
    while(pos < stop) {

        /* For protein alignments - unaligned regions should not anything
           in the middle line. We show unaligned regions in lower case */

        if(is_aa && IS_LOWER(szQuery[pos]) && IS_LOWER(szSubject[pos])) {
            szMiddleLine[pos]=' ';
            pos++;
            continue;
        }
        q = TO_UPPER(szQuery[pos]);
        s = TO_UPPER(szSubject[pos]);
        if (matrix) {
            if(IS_ALPHA(q) && IS_ALPHA(s)){
                if(q==s) { 
                    szMiddleLine[pos]=szQuery[pos];
                } else {
                    if (matrix[s][q]>0){
                        szMiddleLine[pos]='+';
                    } else {
                        szMiddleLine[pos]=' ';
                    }
                }
            } else {
                szMiddleLine[pos]=' ';
            }
            
        } else {
            if(q == s)
                szMiddleLine[pos]='|';
            else
                szMiddleLine[pos]=' ';
        }
        pos++;
    }
    return(szMiddleLine);
}

/*******************************************************************************

  Function : DDV_GetBLASTCompLine_2()
  
  Purpose : get the middle line of a BLAST pairwise seqAlign
  
*******************************************************************************/
NLM_EXTERN CharPtr DDV_GetBLASTCompLine_2(ParaGPtr pgpQuery, ParaGPtr pgpSubject, 
		Int4Ptr PNTR matrix)
{
CharPtr   szQuery,szSubject,szComp;
BioseqPtr bsp;
Boolean   IsAA;
Int4      bspLength;

	szComp=szQuery=szSubject=NULL;
	/*get the query sequence*/
	szQuery=(CharPtr)MemNew((pgpQuery->StopLetter-pgpQuery->StartLetter+3)*sizeof(Char));
	if (!szQuery) return(NULL);
	bsp=BioseqLockById(pgpQuery->sip);
	if (!bsp) goto error;
	bspLength=BioseqGetLen(bsp);
	IsAA=ISA_aa(bsp->mol);
	BioseqUnlock(bsp);
	if (!DDV_GetSequenceFromParaG(pgpQuery,&szQuery,bspLength,IsAA,NULL,
			NULL,NULL)) goto error;
	/*get the subject sequence*/
	szSubject=(CharPtr)MemNew((pgpSubject->StopLetter-pgpSubject->StartLetter+3)*sizeof(Char));
	if (!szSubject) goto error;
	bsp=BioseqLockById(pgpSubject->sip);
	if (!bsp) goto error;
	bspLength=BioseqGetLen(bsp);
	IsAA=ISA_aa(bsp->mol);
	BioseqUnlock(bsp);
	if (!DDV_GetSequenceFromParaG(pgpSubject,&szSubject,bspLength,IsAA,NULL,
			NULL,NULL)) goto error;

	szComp=DDV_GetBLASTCompLine_1(szQuery,szSubject,matrix, IsAA);
error:
	if (szQuery) MemFree(szQuery);
	if (szSubject) MemFree(szSubject);
	return(szComp);
}

/*******************************************************************************

  Function : DDV_GetBLASTCompLine_3()
  
  Purpose : get the middle line of a BLAST pairwise seqAlign
  
*******************************************************************************/
NLM_EXTERN CharPtr DDV_GetBLASTCompLine_3(CharPtr szQuery, ParaGPtr pgpSubject, 
		Int4Ptr PNTR matrix)
{
CharPtr   szSubject,szComp;
BioseqPtr bsp;
Boolean   IsAA;
Int4      bspLength;

	szComp=szSubject=NULL;
	/*get the subject sequence*/
	szSubject=(CharPtr)MemNew((pgpSubject->StopLetter-pgpSubject->StartLetter+3)*sizeof(Char));
	if (!szSubject) goto error;
	bsp=BioseqLockById(pgpSubject->sip);
	if (!bsp) goto error;
	bspLength=BioseqGetLen(bsp);
	IsAA=ISA_aa(bsp->mol);
	BioseqUnlock(bsp);
	if (!DDV_GetSequenceFromParaG(pgpSubject,&szSubject,bspLength,IsAA,NULL,
			NULL,NULL)) goto error;
			
	szComp=DDV_GetBLASTCompLine_1(szQuery,szSubject,matrix, IsAA);
error:
	if (szSubject) MemFree(szSubject);
	return(szComp);
}

/*******************************************************************************

  Function : DDV_HideMaskedLetters()
  
  Purpose : replace sequence letters with 'X' in masked regions (BLAST)
  
*******************************************************************************/
static void DDV_HideMaskedLetters(CharPtr szSeqMaster,ParaGPtr pgp,ValNodePtr mask)
{
ValNodePtr      vnp;
SeqLocPtr       slp;
Int4            i,bsp_start,bsp_stop,mask_start,mask_stop,scan_from,
                scan_to,max,real_pos;
ValNode         vn;

	if (!szSeqMaster || !pgp || !mask) return;
	
	memset(&vn,0,sizeof(ValNode));
	vn.data.ptrvalue=(Pointer)pgp;

	/*get query sequence range*/
	UDV_ComputeBspCoordRangeinPGP(pgp,&bsp_start,&bsp_stop);
	max=StringLen(szSeqMaster);
	
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
				for(i=scan_from;i<scan_to;i++){
					real_pos=DDV_GetDispCoordGivenBspCoord(&vn,i)-pgp->StartLetter;
					if (real_pos>=0 && real_pos<max){
						szSeqMaster[real_pos]='X';
					}
				}
			}
			slp = slp->next;
		}
		vnp=vnp->next;
	}
}

/*******************************************************************************

  Function : DDV_DisplayParaG()
  
  Purpose : display the content of a ParaG
  
  Parameters : 	vnp; the node containing the ParaG
  				i; sequence number (from 1 to nSequence)
  				ShowScale;if true, show the top scale
				disp_format; display format options
				szSeqAcc; Accession number of the sequence
				szSeqName; sequence name
				IsAA; TRUE means protein
				bDispPhilouName;used for PHYLIP format
				szFormattedLine; see 'Return value'
				szSeqMaster; sequence of a master (used to display variations)
				matrix; matrix used for BLAST output
				gclr; enhanced display options (colors, upper/lower case,...)
				
  Return value : the next node in the ParaG ValNode List of a sequence;
                fill szFormattedLine with the info ready to display

*******************************************************************************/
static ValNodePtr DDV_DisplayParaG
(ValNodePtr vnp,Int4 i,Boolean ShowScale, Uint4 disp_format,
 CharPtr szSeqAcc,CharPtr szSeqName,Boolean IsAA, Boolean bDispPhilouName,
 Int4 max_length,Int4 max_scale,Int2 LineSize, CharPtr PNTR szFormattedLine,
 CharPtr PNTR szSeqMaster, Int4Ptr PNTR matrix, DDV_ColorGlobal * gclr,
 ValNodePtr mask
 ) {
    ParaGPtr  pgp;	/*ParaG data*/
    CharPtr   szSequence,szDisp,szFLine=NULL,szTmp,szMiddleLine, idstring;
    BioseqPtr bsp;
    SeqIdPtr  sip;
    Int4      bspLength,size,stop,nCompt2,pos,gi,diff,disp;
    Int4      bsp_start,bsp_stop, n, j;
    Char      szBuf4[WWW_SCRIPT_SIZE]={""};   /*Entrez query*/
    Uint1     bsp_strand;
    Char      tmpstr[99];
	
    if (vnp==NULL) 
        return NULL;
    
    pgp=(ParaGPtr)vnp->data.ptrvalue;
    if (!pgp) 
        return(vnp->next);
    
    szSequence=(CharPtr)MemNew((pgp->StopLetter-pgp->StartLetter+3)*sizeof(Char));
    
    if (!szSequence) 
        return NULL;

    /*get some bsp info*/	
    sip=SeqIdFindBest(pgp->sip,0);
    if (sip==NULL) sip=pgp->sip;

    if((bsp=BioseqLockById(sip)) == NULL)
        goto error;

    bspLength=BioseqGetLen(bsp);
    
    if (!DDV_GetSequenceFromParaG(pgp,&szSequence,bspLength,IsAA,&bsp_strand,
                                  &bsp_start,&bsp_stop)) 
        goto error;
    
    if (gclr) {/*format the output; 
                 only upper/lower case letters are lait out in this function*/
        DDV_GetFormattedSequence_1(pgp,szSequence,gclr,i);
    } else if (disp_format&TEXT_LOWERCASE || ISA_na(bsp->mol)){
	/*switch to lower case if needed; this is for the PopSet viewer: it displays
          DNA sequences using ALWAYS lower case letters. We don't need a complex gclr
          data structure here (this takes time to create...)*/
        szSequence=ConvertToLower(szSequence);
    }
    /*right now, I consider the first sequence of the SeqAlign is the master*/
    if (i==1){
        *szSeqMaster=StringSave(szSequence);
    }
    
    /*if (i==1 && mask){
      DDV_HideMaskedLetters(szSequence,pgp,mask);
      }*/
    
    /*7 : StringLen("unknown"); see below, when I print szSeqAcc*/
    if (max_length<7) max_length=7;
    disp=0;
    
    if(disp_format&DISP_FULL_HTML || disp_format&DISP_FULL_TXT){/*output format: html*/
        /*compute the left part size of the line to display*/
        if (disp_format&DISP_ORDER_NUM) {disp+=7;max_length+=7;}
        if (*szSeqAcc) disp+=StringLen(szSeqAcc);
        else disp+=7;
        diff=max_length-disp;
        diff++;
        disp+=diff;
        if (disp_format&DISP_STRAND) disp+=2;
        if (disp_format&DISP_BSP_COORD)disp+=max_scale+2;
        /*display top scale if needed*/
        if ((disp_format&RULER_TOP) && ShowScale) {
            szTmp=DDV_DisplayTopScale(pgp->StartLetter,
                                      pgp->StopLetter,disp,disp_format,SCALE_NUM);
            if (szTmp){
                szFLine=DDV_ConcatStr(szFLine,szTmp);
                MemFree(szTmp);
            }
        }
        if ((disp_format&RULER_TICK) && ShowScale) {
            szTmp=DDV_DisplayTopScale(pgp->StartLetter,
                                      pgp->StopLetter,disp,disp_format,SCALE_TICK);
            if (szTmp){
                szFLine=DDV_ConcatStr(szFLine,szTmp);
                MemFree(szTmp);
            }
        }
        /*display order num if needed*/
        if (disp_format&DISP_ORDER_NUM){ /*fprintf(fp,"[%4i] ",i);*/
            sprintf(szBuf4,"[%4i] ",i);
            szFLine=DDV_ConcatStr(szFLine,szBuf4);
        }
        /*display sequence name*/
        if (*szSeqAcc) {
            if (disp_format&DISP_FULL_HTML && !(disp_format&DISP_NOLINKONNAME)){
                if (pgp->sip->choice == SEQID_PDB) {
                    n = j = 0;
                    idstring = StringSave(szSeqAcc);
                    while (idstring[n] != '\0') {
                        if (idstring[n] != '_') {
                            tmpstr[j] = idstring[n];
                            j++;
                        }
                        n++;
                    }
                    tmpstr[j] = '\0';
                    sprintf(szBuf4,szEntrezScriptSMART,tmpstr,(IsAA ? "p" : "n"));
                } else {
                    gi=GetGIForSeqId(pgp->sip);
                    if(gi > 0){
                        sprintf(szBuf4,szEntrezScript,gi,(IsAA ? "p" : "n"));
                    } else
                        sprintf(szBuf4,"javascript:void(0)");
                }
                if (szSeqName) {
                    szFLine=DDV_ConcatStr(szFLine,"<a href=\"");
                    szFLine=DDV_ConcatStr(szFLine,szBuf4);
                    szFLine=DDV_ConcatStr(szFLine,"\" onMouseOut=\"window.status=''\" \nonMouseOver=\"window.status='");
                    szFLine=DDV_ConcatStr(szFLine,szSeqName);
                    szFLine=DDV_ConcatStr(szFLine,"';return true\">");
                } else {
                    szFLine=DDV_ConcatStr(szFLine,"<a href=\"");
                    szFLine=DDV_ConcatStr(szFLine,szBuf4);
                    szFLine=DDV_ConcatStr(szFLine,"\" onMouseOut=\"window.status=''\" \nonMouseOver=\"window.status='");
                    szFLine=DDV_ConcatStr(szFLine,"unknown name");
                    szFLine=DDV_ConcatStr(szFLine,"';return true\">");
                }
            }
            szFLine=DDV_ConcatStr(szFLine,szSeqAcc);
            if (disp_format&DISP_FULL_HTML && !(disp_format&DISP_NOLINKONNAME)) 
                szFLine=DDV_ConcatStr(szFLine,"</a>");
        } else {
            szFLine=DDV_ConcatStr(szFLine,"unknown");
        }
        DDV_AddBlank2(diff,&szFLine);
        if (disp_format&DISP_STRAND){
            Char szStuff[]="< >";
            Int1 iStuff=1;
            
            if (bsp_start==INT4_MAX){
                iStuff=1;
            } else {
                if (bsp_strand==Seq_strand_minus)
                    iStuff=0;
                else
                    iStuff=2;
            }
            if (bsp_strand==Seq_strand_minus)
                szFLine=DDV_ConcatStr(szFLine,"<FONT COLOR=#FF0000>");
            sprintf(szBuf4,"%c ", szStuff[iStuff]);
            szFLine=DDV_ConcatStr(szFLine,szBuf4);
            if (bsp_strand==Seq_strand_minus)
                szFLine=DDV_ConcatStr(szFLine,"</FONT>");
        }
        /*display bsp coord if needed*/
        if (disp_format&DISP_BSP_COORD){
            Char szBuf2[15]={""};
            Int4 iVal;
            
            if (bsp_strand==Seq_strand_minus) iVal=bsp_stop;/**/
            else iVal=bsp_start;
            
            if (iVal!=INT4_MAX) {
                sprintf(szBuf2,"%i",++iVal);/*switch to base one*/
                szFLine=DDV_ConcatStr(szFLine,szBuf2);
                DDV_AddBlank2(MAX(2,max_scale-StringLen(szBuf2)+2),&szFLine);
            } 
            else DDV_AddBlank2(max_scale+2,&szFLine);
        }
    } else if (disp_format&DISP_PHYLIP_TXT){
        if (bDispPhilouName){/*Phylip format: seq. names for the first block only*/
            if (szSeqAcc) {/*name only*/
                sprintf(szBuf4,"%-10s ",szSeqAcc);
                szFLine=DDV_ConcatStr(szFLine,szBuf4);
            }
            else {
                szFLine=DDV_ConcatStr(szFLine,"unknown");
            }
        } else {
            sprintf(szBuf4,"%-10s "," ");
            szFLine=DDV_ConcatStr(szFLine,szBuf4);
        }
    }
    /*display sequence*/
    size=StringLen(szSequence);
    if (disp_format&DISPE_SHOWBLOCK){/*AFSDDGTFDG GDFDTEGDFG DFDGETDFGD*/
        stop=size+1;
        size+=(size/LETTER_BLOCK_WIDTH);
        szDisp=(CharPtr)MemNew((size+1)*sizeof(Char));
        if (szDisp){
            MemSet(szDisp,' ',size*sizeof(Char));
            pos=1;
            nCompt2=0;
            while(pos<stop){
                if(disp_format&VIEW_FULLSEQ){
                    szDisp[nCompt2++]=szSequence[pos-1];
                }else if(disp_format&VIEW_VARIA){
                    if(*szSeqMaster && i!=1){
                        if(szSequence[pos-1]=='-'){
                            szSequence[pos-1]='-';
                        }
                        if(szSequence[pos-1]!=(*szSeqMaster)[pos-1]){
                            szDisp[nCompt2++]=szSequence[pos-1];
                        } else {
                            if(szSequence[pos-1]!='-') szDisp[nCompt2++]='.';
                            else szDisp[nCompt2++]=szSequence[pos-1];
                        }
                    } else {
                        szDisp[nCompt2++]=szSequence[pos-1];
                    }
                }
                if (!(pos % LETTER_BLOCK_WIDTH)) {
                    szDisp[nCompt2++]=' ';
                }
                pos++;
            }
            szTmp=DDV_Print_Sequence(szDisp,(Boolean)(disp_format&DISPE_COLOR),IsAA,pgp,gclr,i);
            if (szTmp){
                szFLine=DDV_ConcatStr(szFLine,szTmp);
                MemFree(szTmp);
            }
            MemFree(szDisp);
        } else {
            szTmp=DDV_Print_Sequence(szSequence,(Boolean)(disp_format&DISPE_COLOR),IsAA,pgp,gclr,i);
            if (szTmp){
                szFLine=DDV_ConcatStr(szFLine,szTmp);
                MemFree(szTmp);
            }
        }
    } else {/*AFSDDGTFDGGDFDTEGDFGDFDGETDFGD*/
        if(disp_format&VIEW_FULLSEQ){
            if(disp_format&DISP_FULL_HTML){/*output format*/
                szTmp=DDV_Print_Sequence(szSequence,(Boolean)(disp_format&DISPE_COLOR),IsAA,pgp,gclr,i);
                if (szTmp){
                    szFLine=DDV_ConcatStr(szFLine,szTmp);
                    MemFree(szTmp);
                }
            } else {
                szFLine=DDV_ConcatStr(szFLine,szSequence);
            }
        } else if(disp_format&VIEW_VARIA){
            szDisp=(CharPtr)MemNew((size+1)*sizeof(Char));
            if (szDisp){
                MemSet(szDisp,' ',size*sizeof(Char));
                pos=0;
                stop=size+1;
                while(pos<stop){
                    if(*szSeqMaster && i!=1){
                        if(szSequence[pos]!=(*szSeqMaster)[pos]){
                            szDisp[pos]=szSequence[pos];
                        } else {
                            if(pos==stop-1)
                                szDisp[pos]=szSequence[pos];
                            else
                                szDisp[pos]='.';
                        }
                    } else {
                        szDisp[pos]=szSequence[pos];
                    }
                    pos++;
                }
                if(disp_format&DISP_FULL_HTML){/*output format*/
                    szTmp=DDV_Print_Sequence(szDisp,(Boolean)(disp_format&DISPE_COLOR),IsAA,pgp,gclr,i);
                    if (szTmp){
                        szFLine=DDV_ConcatStr(szFLine,szTmp);
                        MemFree(szTmp);
                    }
                } else {
                    szFLine=DDV_ConcatStr(szFLine,szDisp);
                }
                MemFree(szDisp);
            } else {
                if(disp_format&DISP_FULL_HTML){/*output format*/
                    szTmp=DDV_Print_Sequence(szSequence,(Boolean)(disp_format&DISPE_COLOR),IsAA,pgp,gclr,i);
                    if (szTmp){
                        szFLine=DDV_ConcatStr(szFLine,szTmp);
                        MemFree(szTmp);
                    }
                } else {
                    szFLine=DDV_ConcatStr(szFLine,szSequence);
                }
            }
        }
    }
    /*display bsp coord if needed*/
    if (disp_format&DISP_BSP_COORD){
        Int4 iVal;
        
        if (bsp_strand==Seq_strand_minus) iVal=bsp_start;/**/
        else iVal=bsp_stop;
        
        if (iVal!=INT4_MIN) {
            sprintf(szBuf4,"  %i", iVal+1);
            szFLine=DDV_ConcatStr(szFLine,szBuf4);
        }
        else DDV_AddBlank2(max_scale+2,&szFLine);
    }
    
    /*build the BLAST middle line of the alignment*/
    if(disp_format&DISP_BLAST_MIDLINE && i>1){
        szMiddleLine=DDV_GetBLASTCompLine_1(*szSeqMaster, szSequence, matrix,
                                            IsAA);
        if (szMiddleLine){
            szTmp=(CharPtr)MemNew((disp+1)*sizeof(Char));
            MemSet(szTmp,' ',disp);
            szTmp=DDV_ConcatStr(szTmp,szMiddleLine);
            szTmp=DDV_ConcatStr(szTmp,"\n");
            szTmp=DDV_ConcatStr(szTmp,szFLine);
            MemFree(szFLine);
            szFLine=szTmp;
            MemFree(szMiddleLine);
        }
    }
    
    /*hack: for test purpose (features display on SeqAlign)*/
    /*DDV_DispFeaturesForPGP(pgp, bsp, &szFLine, disp, LineSize, szSequence,
      bsp_strand);*/
    
    
 error:
    if (bsp)BioseqUnlock(bsp);
    MemFree(szSequence);
    szFLine=DDV_ConcatStr(szFLine,"\n");
    *szFormattedLine=szFLine;
    
    return(vnp->next);
}

/*******************************************************************************

  Function : DDV_Nav_Arrows()
  
  Purpose : display the "navigator" links (to jump to next/previous blocks)
  
  Parameters : 	
				gi; gi number used by PopSet viewer to build the HTTP links
				
  				TotalAliLength; SeqAlign size
				numBlockAffich; block number displayed (when display by block is
						required; otherwise numBlockAffich=0).
				disp_format; full HTML, Phylip, ...
				LineSize; size of one text line (number of letter for the sequence)
				
  Return value : -

*******************************************************************************/
static CharPtr DDV_Nav_Arrows(Int4 gi,Int4 TotalAliLength,
		Int4 numBlockAffich,Uint4 disp_format,Int2 LineSize)
{
CharPtr	szWide,szFLine=NULL;
Int4 	k,wide,lGoLeft,lGoRight,fromL,toL,fromR,toR;
Char	szGoLeft[30]={""};
Char	szGoRight[30]={""};
Char 	szWWW[WWW_SCRIPT_SIZE]={""};

	k=TotalAliLength/LineSize;
	if (TotalAliLength%LineSize) k++;
	if (numBlockAffich>1){/*left arrow*/
		fromL=(numBlockAffich-2)*LineSize+1;
		toL=_min_((numBlockAffich-1)*LineSize,TotalAliLength);
		sprintf(szGoLeft,"[%d..%d]",fromL,toL);
		lGoLeft=StringLen(szGoLeft)+2;
	}
	else{
		StringCpy(szGoLeft," ");
		fromL=0;
		toL=0;
		lGoLeft=1;
	}
	if (numBlockAffich<k){/*right arrow*/
		fromR=(numBlockAffich)*LineSize+1;
		toR=_min_((numBlockAffich+1)*LineSize,TotalAliLength);
		sprintf(szGoRight,"[%d..%d]",fromR,toR);
		lGoRight=StringLen(szGoRight)+2;
	}
	else{
		StringCpy(szGoRight," ");
		fromR=0;
		toR=0;
		lGoRight=1;
	}
	wide=LineSize+28;
	if (disp_format&DISPE_SHOWBLOCK) wide+=(LineSize/LETTER_BLOCK_WIDTH);
	szWide=(CharPtr)MemNew(wide*sizeof(Char));
	MemSet(szWide,'.',(wide-lGoLeft-lGoRight)*sizeof(Char));
	szWide[wide-lGoLeft-lGoRight]='\0';
	szFLine=DDV_ConcatStr(szFLine,"<pre>");
	if(fromL!=0 && toL!=0){
		sprintf(szWWW,WWWDDV_script,gi,fromL,toL,disp_format);
		szFLine=DDV_ConcatStr(szFLine,"&lt;&lt<a href=\"");
		szFLine=DDV_ConcatStr(szFLine,szWWW);
		szFLine=DDV_ConcatStr(szFLine,"\" onMouseOut=\"window.status='';return true\" ");
		szFLine=DDV_ConcatStr(szFLine,"onMouseOver=\"window.status='Click to view this region';return true\" >");
		szFLine=DDV_ConcatStr(szFLine,szGoLeft);
		szFLine=DDV_ConcatStr(szFLine,"</a>");
	}
	else{
		szFLine=DDV_ConcatStr(szFLine,szGoLeft);
	}
	szFLine=DDV_ConcatStr(szFLine,"<FONT COLOR=#FFFFFF>");
	szFLine=DDV_ConcatStr(szFLine,szWide);
	szFLine=DDV_ConcatStr(szFLine,"</FONT>");
	
	if(fromR!=0 && toR!=0){
		sprintf(szWWW,WWWDDV_script,gi,fromR,toR,disp_format);
		szFLine=DDV_ConcatStr(szFLine,"<a href=\"");
		szFLine=DDV_ConcatStr(szFLine,szWWW);
		szFLine=DDV_ConcatStr(szFLine,"\" onMouseOut=\"window.status='';return true\" ");
		szFLine=DDV_ConcatStr(szFLine,"onMouseOver=\"window.status='Click to view this region';return true\" >");
		szFLine=DDV_ConcatStr(szFLine,szGoRight);
		szFLine=DDV_ConcatStr(szFLine,"</a>&gt;&gt");
	}
	else{
		szFLine=DDV_ConcatStr(szFLine,szGoRight);
	}
	szFLine=DDV_ConcatStr(szFLine,"</pre>");

	MemFree(szWide);
	return(szFLine);
}

/*******************************************************************************

  Function : DDV_AffichageParaG()
  
  Purpose : function to create an HTML/TXT/PHYLIP display of an Indexed SeqAlign
  
  Parameters : 	mpplp; contain the data to create a display (sap, ...)
				gi; gi number used by PopSet viewer to build the HTTP links
				from - to; range of the SeqAlign to display
  				TotalAliLength; SeqAlign size
				numBlockAffich; block number displayed (when display by block is
						required; otherwise numBlockAffich=0).
				disp_format; full HTML, Phylip, ...
				LineSize; size of each line of sequence
				fp; file to put the ouptut
				bspp; filled here withthe formatted text
				matrix; matrix used for BLAST output
				gclr; enhanced display options (colors, upper/lower case,...)
				 
  Return value : - (see bspp)

  Note : if fp==NULL, formatted text will be stored in bspp; Otherwise the
         text is directly sent to fp.
		 
*******************************************************************************/
NLM_EXTERN void DDV_AffichageParaG(MsaParaGPopListPtr mpplp,Int4 gi,Int4 from,Int4 to,
		Int4 TotalAliLength,Int4 numBlockAffich,Uint4 disp_format,Int2 LineSize,
		FILE *fp,ByteStorePtr PNTR bspp,Int4Ptr PNTR matrix,DDV_ColorGlobal * gclr,
		ValNodePtr mask)
{
Int4 			i,j=0,max_length=INT4_MIN,max_scale=INT4_MIN;
ValNodePtr PNTR TableDesc=NULL;
CharPtr 		szColor[]={"#E6EEEE","ccccff","FFFFFF"};
CharPtr 		*szSeqAcc;
CharPtr 		*szSeqName;
CharPtr 		szFline,szTmp;
Char 			szBuf[25],szBuf2[25];
ParaGPtr 		pgp;
BioseqPtr 		bsp;
BoolPtr 		IsAAp;
Boolean			bDispPhilouName=TRUE;
SeqIdPtr        sip;
CharPtr  		szSeqMaster=NULL;

	/*if bspp is not allocated*/
	if (fp==NULL && (*bspp)==NULL){
		(*bspp)=BSNew((Int4)1);
		if ((*bspp)==NULL) return;
	}
	
	/*prepare a list of ParaG heads, names, Acc Number and Mol_type*/
	TableDesc=(ValNodePtr PNTR)MemNew(mpplp->nBsp*sizeof(ValNodePtr));
	szSeqAcc=(CharPtr *)MemNew(mpplp->nBsp*sizeof(CharPtr));
	szSeqName=(CharPtr *)MemNew(mpplp->nBsp*sizeof(CharPtr));
	IsAAp=(BoolPtr)MemNew(mpplp->nBsp*sizeof(Boolean));

	if (!TableDesc || !szSeqAcc || !szSeqName || !IsAAp) goto error;
	MemSet(szSeqAcc,0,mpplp->nBsp*sizeof(CharPtr));	
	MemSet(szSeqName,0,mpplp->nBsp*sizeof(CharPtr));	
	MemSet(IsAAp,0,mpplp->nBsp*sizeof(Boolean));	
	if (!(disp_format&DISPE_TABLE)) j=2;
	else j=0;

	for(i=0;i<mpplp->nBsp;i++){
		TableDesc[i]=mpplp->TableHead[i];/*copy the ValNode heads in TableDesc*/
		/*get the access code*/
		pgp=(ParaGPtr)TableDesc[i]->data.ptrvalue;
		if (pgp){/*create the name table*/
			bsp=BioseqLockById(pgp->sip);
			if (bsp){
				Int2 jj;
				if (disp_format&SEQNAME_BLAST_STD){ 
					if (i==0) 
						StringCpy(szBuf,"Query:");
					else
						StringCpy(szBuf,"Sbjct:");
				}
				else{
					sip = SeqIdFindBest(bsp->id, SEQID_PDB);
					if (!sip)
				   		sip = SeqIdFindBest(bsp->id, 0);
					SeqIdWrite(sip, szBuf,PRINTID_TEXTID_ACCESSION, 14);   
				}
				/*access code*/
				szSeqAcc[i]=StringSave(szBuf);
				max_length=MAX(max_length,(Int4)StringLen(szSeqAcc[i]));
				/*seq title*/
				szSeqName[i]=(CharPtr)MemNew(100*sizeof(Char));
				if (szSeqName[i] && !(disp_format&SEQNAME_BLAST_STD)){
					CreateDefLine(NULL, bsp, szSeqName[i], 99, 0, NULL, NULL);
					jj=0;
					while(szSeqName[i][jj]){
						if (szSeqName[i][jj]=='\'' || szSeqName[i][jj]=='\"') szSeqName[i][jj]=' ';
						jj++;
					}
				}
				
				IsAAp[i]=ISA_aa(bsp->mol);
				/*size for the BSP coord display*/
				sprintf(szBuf2,"%i",BioseqGetLen(bsp));
				max_scale=MAX(max_scale,(Int4)StringLen(szBuf2));
				BioseqUnlock(bsp);
			}
		}
	}

	szFline=NULL;
	if(disp_format&DISP_FULL_HTML){/*output format*/
		if (numBlockAffich>0){/*nav arrow*/
			szTmp=DDV_Nav_Arrows(gi,TotalAliLength,numBlockAffich,disp_format,LineSize);
			szFline=DDV_ConcatStr(szFline,szTmp);
			MemFree(szTmp);
		}
		if (disp_format&DISPE_TABLE){
			szFline=DDV_ConcatStr(szFline,"<table><tr><td bgcolor=");
			szFline=DDV_ConcatStr(szFline,szColor[j]);
			szFline=DDV_ConcatStr(szFline,">");
			szFline=DDV_ConcatStr(szFline,"<pre>");
		}
		else {
			/*if (!(disp_format&DISP_BLAST_STD)) */
				szFline=DDV_ConcatStr(szFline,"<pre>");
		}
	}else if(disp_format&DISP_PHYLIP_TXT){
		sprintf(szBuf,"%7d  %d\n",mpplp->nBsp,mpplp->LengthAli);
		szFline=DDV_ConcatStr(NULL,szBuf);
	}

	if(szFline){
		if (fp)
			fprintf(fp,"%s",szFline);
		else
			BSWrite(*bspp, (VoidPtr)szFline, StringLen(szFline));
		MemFree(szFline);
	}

	i=1;
	while(TRUE){
		szFline=NULL;
		/*display one ParaG (one row; except for BLAST : when the subject line is displayed,
		the comp. line is displayed at the same time*/
		TableDesc[i-1]=DDV_DisplayParaG(TableDesc[i-1],i,(Boolean)(i==1 ? TRUE : FALSE),
			disp_format,szSeqAcc[i-1],szSeqName[i-1],IsAAp[i-1],bDispPhilouName,
			max_length,max_scale,LineSize,&szFline,&szSeqMaster,matrix,gclr,mask);
		/*actually, DDV_DisplayParaG() put a formatted line in szFline; now put that in a file or BS */
		if (szFline){
			if (fp)
				fprintf(fp,"%s",szFline);
			else
				BSWrite(*bspp, (VoidPtr)szFline, StringLen(szFline));
			MemFree(szFline);
		}
		i++;
		/*I use 'i' to count the number of rows displayed. As soon as 'i' > mpplp->nBsp, I need to do
		specials things, especially for the PopSet Viewer. Indeed, PopSet-V puts each "block" of the
		SeqAlign in the cell of a table. So, for each new block, I have to close/open HTML tags.*/
		if (i>mpplp->nBsp){
			i=1;
			if (szSeqMaster) 
				szSeqMaster=MemFree(szSeqMaster);
			if(disp_format&DISP_FULL_HTML){
				if (disp_format&DISPE_TABLE){
					szFline=DDV_ConcatStr(NULL,"</pre>");
					szFline=DDV_ConcatStr(NULL,"</td></tr>");
				}
				else{
					szFline=DDV_ConcatStr(NULL,"\n");
				}
				if(szFline){
					if (fp)
						fprintf(fp,"%s",szFline);
					else
						BSWrite(*bspp, (VoidPtr)szFline, StringLen(szFline));
					MemFree(szFline);
				}
			}else if((disp_format&DISP_PHYLIP_TXT) || (disp_format&DISP_FULL_TXT)){
				szFline=DDV_ConcatStr(NULL,"\n");
				if(szFline){
					if (fp)
						fprintf(fp,"%s",szFline);
					else
						BSWrite(*bspp, (VoidPtr)szFline, StringLen(szFline));
					MemFree(szFline);
				}
			}
			if (numBlockAffich==0){
				if (j==1) j=0; else j=1;
			}
			if(disp_format&DISP_FULL_HTML){
                szFline=NULL;
				if (disp_format&DISPE_TABLE){
					szFline=DDV_ConcatStr(NULL,"<tr><td bgcolor=");
					szFline=DDV_ConcatStr(szFline,szColor[j]);
					szFline=DDV_ConcatStr(szFline,">");
					szFline=DDV_ConcatStr(szFline,"<pre>");
				}
				if(szFline){
					if (fp)
						fprintf(fp,"%s",szFline);
					else
						BSWrite(*bspp, (VoidPtr)szFline, StringLen(szFline));
					MemFree(szFline);
				}
			}else if(disp_format&DISP_PHYLIP_TXT){
				bDispPhilouName=FALSE;
			}
			if (TableDesc[mpplp->nBsp-1]==NULL) break;
		} 
	}
	/*we're close to the end... for the PopSet-Viewer, I need to close the table*/
	if(disp_format&DISP_FULL_HTML){
		if (disp_format&DISPE_TABLE){
			szFline=DDV_ConcatStr(NULL,"</pre>");
			szFline=DDV_ConcatStr(szFline,"</td></tr></table>");
		}
		else{
			/*if (!(disp_format&DISP_BLAST_STD)) */
				szFline=DDV_ConcatStr(NULL,"</pre>");
		}
		if (numBlockAffich>0)/*nav arrow*/{
			szTmp=DDV_Nav_Arrows(gi,TotalAliLength,numBlockAffich,disp_format,LineSize);
			szFline=DDV_ConcatStr(szFline,szTmp);
			MemFree(szTmp);
		}
		if(szFline){
			if (fp)
				fprintf(fp,"%s",szFline);
			else
				BSWrite(*bspp, (VoidPtr)szFline, StringLen(szFline));
			MemFree(szFline);
		}
	}
	/*end of the display*/
error:
	if (TableDesc) MemFree(TableDesc);
	if (szSeqAcc){
		for(i=0;i<mpplp->nBsp;i++){
			if (szSeqAcc[i]){
				MemFree(szSeqAcc[i]);
			}
		}
		MemFree(szSeqAcc);
	}
	if (szSeqName){
		for(i=0;i<mpplp->nBsp;i++){
			if (szSeqName[i]){
				MemFree(szSeqName[i]);
			}
		}
		MemFree(szSeqName);
	}
	if (IsAAp) MemFree(IsAAp);
}

/*******************************************************************************

  Function : DDV_GetGappedSequence()
  
  Purpose : get a gapped sequence from the indexed seq align
  
  Parameters : 	segp; head of the nodes list of one bsp
  				szSequence; ALLOCATED buffer of size SeqAlign, where the sequence
							will be stored.
				IsAA; doesn't need an explanation, I think...
				
  Note : do not forget to allocate 'szSequence' before entering this function
  
  Return value : none (see szSequence)

*******************************************************************************/
static void DDV_GetGappedSequence(SegmentPtr segp,CharPtr szSequence,
		Boolean IsAA)
{
SegmentPtr sp;
CharPtr SeqBuf;/*I use a pointer because I cannot use a fixed array*/

	if (!szSequence || !segp) return;
	/*loop each segment of the bioseq*/
	for(sp=segp ; sp!=NULL ; sp=sp->bsp_next){
		if(sp->gap){/*gap segment*/
			SeqBuf=(CharPtr)MemNew((sp->gap+1)*sizeof(Char));
			if (SeqBuf){
				MemSet(SeqBuf,'-',(sp->gap)*sizeof(Char));
			}
		}
		else{/*sequence segment*/
			SeqBuf=UDV_Read_SequenceEx (sp->sip,sp->from,sp->to, 
				IsAA,(Int2)(sp->to-sp->from+1),(Uint1)
				(sp->strand == Seq_strand_minus ? Seq_strand_minus : Seq_strand_plus));
			if (!SeqBuf){
				SeqBuf=(CharPtr)MemNew((sp->to-sp->from+2)*sizeof(Char));
				if (SeqBuf){
					MemSet(SeqBuf,'?',(sp->to-sp->from+1)*sizeof(Char));
				}
			}
		}
		if (SeqBuf){
			StringCat(szSequence,SeqBuf);
			MemFree(SeqBuf);
			SeqBuf=NULL;
		}
	}
}

/*******************************************************************************

  Function : DDV_PrintFastaGappedSequence()
  
  Purpose : print a character string as a FASTA sequence format
  
  Parameters : 	szSequence; what do you think it is ?
  				fp; may be a real file or 'stdout'
  
  Return value : none

*******************************************************************************/
#define line_fas_size 80
static void DDV_PrintFastaGappedSequence(CharPtr szSequence,FILE *fp)
{
Char szBuf[line_fas_size+1]={""};
Int4 i=0,j=0,len=StringLen(szSequence);

	if (!szSequence || !fp) return;

	while(szSequence[i]){
		szBuf[j++]=szSequence[i++];
		if (i==len || j==line_fas_size){
			szBuf[j]='\0';
			fprintf(fp,"%s\n",szBuf);
			j=0;
		}
	}
}

/*******************************************************************************

  Function : DDV_GetFullFASTAforIdxAli()
  
  Purpose : create the FASTA file for a full Indexed SeqAlign
  
  Parameters :  sap; seqalign (must be indexed before entering this funtion)
				fp; file to put the FASTA
  
  Return value : TRUE if success 

*******************************************************************************/
NLM_EXTERN Boolean DDV_GetFullFASTAforIdxAli(SeqAlignPtr sap,FILE *fp)
{
BioseqPtr	bsp;	
Int4        i,nRow;

	nRow=AlnMgrGetNumRows(sap);
	
	for (i=1 ; i <= nRow ; i++){
		bsp=BioseqLockById(AlnMgrGetNthSeqIdPtr(sap, i));
		if (bsp){
			BioseqToFasta(bsp,fp,(Boolean)ISA_na(bsp->mol));
			BioseqUnlock(bsp);
		}
	}
	return(TRUE);	
}

/*******************************************************************************

  Function : DDV_GetFullGapFASTAforIdxAli()
  
  Purpose : create the gapped FASTA file for a full Indexed SeqAlign
  
  Parameters :  mpplp; indexed seqalign
				fp; file to put the FASTA
  
  Return value : TRUE if success 

*******************************************************************************/
NLM_EXTERN Boolean DDV_GetFullGapFASTAforIdxAli(MsaParaGPopListPtr mpplp,FILE *fp)
{
Int4 i,bspLength;
ParaGPtr 		pgp;
ValNodePtr    vnp;
BioseqPtr bsp;
Boolean   IsAA;
CharPtr szSeq;
Char DefLine[255];

	for (i=0;i<mpplp->nBsp;i++){
		vnp=mpplp->TableHead[i];
		/*get some BSP data*/
		pgp=(ParaGPtr)vnp->data.ptrvalue;
		bsp=BioseqLockById(pgp->sip);
		if (!bsp) continue;
		bspLength=BioseqGetLen(bsp);
		IsAA=ISA_aa(bsp->mol);
		/*print the FASTA header for the bioseq*/
		FastaId(bsp,DefLine,254);
		fprintf(fp,">%s ",DefLine);
		CreateDefLine(NULL, bsp, DefLine, 254, 0,NULL, NULL);
		fprintf(fp,"%s\n",DefLine);
		BioseqUnlock(bsp);
		/*print the gapped sequence*/
		while(vnp){
			pgp=(ParaGPtr)vnp->data.ptrvalue;
			szSeq=(CharPtr)MemNew((pgp->StopLetter-pgp->StartLetter+3)*sizeof(Char));
			if (szSeq){
				if (!DDV_GetSequenceFromParaG(pgp,&szSeq,bspLength,IsAA,NULL,
					NULL,NULL)) continue;
				fprintf(fp,szSeq);
				fprintf(fp,"\n");
				MemFree(szSeq);
			}
			vnp=vnp->next;
		}
	}
	fprintf(fp,"\n");
	return(TRUE);
}

/*******************************************************************************

  Function : DDV_PrintStudyName()
  
  Purpose : print the name/author of the PopSet study
  
*******************************************************************************/
NLM_EXTERN void DDV_PrintStudyName(CharPtr szPopSetName,CharPtr szPopSetAuth,
		CharPtr szJournalTitle,Int4 pmid,FILE *fp)
{
	if (StringLen(szPopSetName)>0 && StringLen(szPopSetAuth)>0) {
		if (StringStr(szPopSetName,"* no title *") || 
			StringStr(szPopSetAuth,"* no title *")){
			fprintf(fp,"<h2>Unpublished study.</h2>");
		}
		else{
			if (pmid!=0){
				fprintf(fp,"<h2>%s (",szPopSetName);
				fprintf(fp,"<a href=\"http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Retrieve&db=PubMed&list_uids=%d&dopt=Abstract\">",pmid);
				fprintf(fp,"%s <i>et al.</i>",szPopSetAuth);
				if (StringLen(szJournalTitle)>0)
					fprintf(fp,"</a>, <i>%s</i>)<BR></h2>",szJournalTitle);
				else
					fprintf(fp,"</a>)<BR></h2>");
			}
			else{
				fprintf(fp,"<h2>%s (%s <i>et al.</i>", 
					szPopSetName,szPopSetAuth);
				if (StringLen(szJournalTitle)>0)
					fprintf(fp,", <i>%s</i>)<BR></h2>",szJournalTitle);
				else
					fprintf(fp,")<BR></h2>");
			}
		}
	}
	else{
		fprintf(fp,"<h2>Unpublished study.</h2>");
	}
}

/*******************************************************************************

  Function : DDV_GetFirstAuthor()
  
  Purpose : retrieve the first author of the PopSet
  
  Parameters : 	alp; valnode of authors
  				name; filled in this function
  
  Return value : - (see 'name')

*******************************************************************************/
static void DDV_GetFirstAuthor (AuthListPtr alp, CharPtr name)
{
ValNodePtr   names;
AuthorPtr    ap;
PersonIdPtr  pid;

	if (!alp) return;
	names = alp->names;/*this is a ValNodeList, but I only need to deal
	with the first node, i.e the first author*/
	if (alp->choice == 1) {
		ap = (AuthorPtr) names->data.ptrvalue;
		if (ap != NULL) {
			pid = ap->name;
			if (pid != NULL) {
				PersonIdLabel(pid, name, INFO_SIZE, PIDLABEL_GENBANK);
			}
		}
	}
	else if (alp->choice == 2 || alp->choice == 3) {
		StringNCpy(name,(CharPtr)names->data.ptrvalue,INFO_SIZE);
	}
}

/*******************************************************************************

  Function : DDV_GetPubValues()
  
  Purpose : analyse a Imprint pointer and get "82 (21), 7289-7293 (1985)"
  
  Parameters : 	imp; should contains 82 (21), 7289-7293 (1985)
  				szBuf; where to put "82 (21), 7289-7293 (1985)"
  
  Return value : TRUE if imp!=NULL (see also 'szBuf')

*******************************************************************************/
static Boolean DDV_GetPubValues(ImprintPtr imp,CharPtr szImp)
{
Int2         year=(Int2)-1;
Char         szBuf[6]={""};

	if (szImp==NULL) return(FALSE);
	
	StringCpy(szImp," ");
	DateRead (imp->date,&year, NULL, NULL, NULL);
	if (imp->volume){
		StringCat(szImp,imp->volume);
	}
	if (imp->issue){
		StringCat(szImp," (");
		StringCat(szImp,imp->volume);
		StringCat(szImp,")");
	}
	if (imp->pages){
		StringCat(szImp,", ");
		StringCat(szImp,imp->pages);
	}
	if (year!=(Int2)-1){
		StringCat(szImp," (");
		sprintf(szBuf,"%d",year);
		StringCat(szImp,szBuf);
		StringCat(szImp,")");
	}
	
	return(TRUE);
}

static void	DDV_AnalysePubDesc(PubdescPtr pdp, CharPtr szPopSetName,
		CharPtr szPopSetAuth,CharPtr szJournalTitle,Int4Ptr pmid)
{
ValNodePtr   vnp,vnp2;
CitArtPtr    cap;
CitGenPtr    cgp;
CitSubPtr    csp;
CitPatPtr    cpp;
CitBookPtr   cbp;
ImprintPtr   imp;
MedlineEntryPtr mep;
Char         szBuf[80+1]={""};

	for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
		switch(vnp->choice){
			case PUB_Article:/*article*/
				cap = (CitArtPtr) vnp->data.ptrvalue;
				if (!cap) break;
				for (vnp2 = cap->title; vnp2 != NULL; vnp2 = vnp2->next) {
					if(vnp2->choice==Cit_title_name){
						StringNCpy(szPopSetName,(CharPtr)vnp2->data.ptrvalue,INFO_SIZE);
					}
				}
				if (cap->from==1/*journal type*/){
					CitJourPtr cjp=(CitJourPtr)cap->fromptr;
					if (cjp && cjp->title && cjp->title->data.ptrvalue){
						StringNCpy(szBuf,(CharPtr)(cjp->title->data.ptrvalue),80);
						StringCpy(szJournalTitle,szBuf);
						imp=cjp->imp;
						if (imp){
							if (DDV_GetPubValues(imp,szBuf))
								StringCat(szJournalTitle,szBuf);
						}
					}
				}
				DDV_GetFirstAuthor (cap->authors, szPopSetAuth);
				break;
			case PUB_Gen:/*gen*/
				cgp = (CitGenPtr) vnp->data.ptrvalue;
				if (!cgp) break;
				if (cgp->title && *(cgp->title))
					StringNCpy(szPopSetName,cgp->title,INFO_SIZE);
				if (cgp->journal && cgp->journal->data.ptrvalue){
					StringNCpy(szJournalTitle,(CharPtr)(cgp->journal->data.ptrvalue),INFO_SIZE);
				}
				DDV_GetFirstAuthor (cgp->authors, szPopSetAuth);
				break;
			case PUB_Sub:/*Sub*/
				csp = (CitSubPtr) vnp->data.ptrvalue;
				if (!csp) break;
				if (csp->descr && *(csp->descr))
					StringNCpy(szPopSetName,csp->descr,INFO_SIZE);
				StringCpy(szJournalTitle,"direct submission");
				DDV_GetFirstAuthor (csp->authors, szPopSetAuth);
				break;
			case PUB_Patent:/*Pat*/
				cpp = (CitPatPtr) vnp->data.ptrvalue;
				if (!cpp) break;
				if (cpp->title && *(cpp->title))
					StringNCpy(szPopSetName,cpp->title,INFO_SIZE);
				DDV_GetFirstAuthor (cpp->authors, szPopSetAuth);
				break;
			case PUB_Book:/*Book*/
				cbp = (CitBookPtr) vnp->data.ptrvalue;
				if (!cbp) break;
				for (vnp2 = cbp->title; vnp2 != NULL; vnp2 = vnp2->next) {
					if(vnp2->choice==Cit_title_name){
						StringNCpy(szPopSetName,(CharPtr)vnp2->data.ptrvalue,INFO_SIZE);
					}
				}
				if (cbp->title && cbp->title->data.ptrvalue){
					StringNCpy(szBuf,(CharPtr)(cbp->title->data.ptrvalue),80);
					StringCpy(szJournalTitle,szBuf);
					imp=cbp->imp;
					if (imp){
						if (DDV_GetPubValues(imp,szBuf))
							StringCat(szJournalTitle,szBuf);
					}
				}
				DDV_GetFirstAuthor (cbp->authors, szPopSetAuth);
				break;
			case PUB_Medline:/*Medline*/
				mep = (MedlineEntryPtr) vnp->data.ptrvalue;
				if (!mep) break;
				cap = mep->cit;
				for (vnp2 = cap->title; vnp2 != NULL; vnp2 = vnp2->next) {
					if(vnp2->choice==Cit_title_name){
						StringNCpy(szPopSetName,(CharPtr)vnp2->data.ptrvalue,INFO_SIZE);
					}
				}
				if (cap->from==1/*journal type*/){
					CitJourPtr cjp=(CitJourPtr)cap->fromptr;
					if (cjp && cjp->title && cjp->title->data.ptrvalue){
						StringNCpy(szBuf,(CharPtr)(cjp->title->data.ptrvalue),80);
						StringCpy(szJournalTitle,szBuf);
						imp=cjp->imp;
						if (imp){
							if (DDV_GetPubValues(imp,szBuf))
								StringCat(szJournalTitle,szBuf);
						}
					}
				}
				DDV_GetFirstAuthor (cap->authors, szPopSetAuth);
				break;
			case PUB_PMid:/*PubMed Id*/
				*pmid=vnp->data.intvalue;
				break;
		}
	}
}

/*******************************************************************************

  Function : DDV_GetArticleInfo()
  
  Purpose : retrieve the article's title of the PopSet
  
  Parameters : 	sep; seqentry
  				szPopSetName; filled in this function
  				szPopSetAuth; filled in this function

  Note:
	A BioseqSetPtr contains the field 'descr'. This is a ValNodeList.
	Among the nodes, one is of type Seq_descr_pub, the PubMed data information.
	In this case, descr->data.ptrvalue if of type PubdescPtr. Let say
	pdp=(PubdescPtr)descr->data.ptrvalue. Now, pdp is a ValNode List of 
	PubMed fields where pdp->choice can be PUB_Article. In that case
	pdp->data.ptrvalue if of type CitArtPtr. This data block contains
	the article info: title and authors.

	This function is designed to retrieve the first title and the first
	author correponding to that title.

	I've enhanced this function to manage Gen, Sub, Pat, Book, Medline.
  
  Return value : - (see 'szPopSetName' & 'szPopSetAuth')

*******************************************************************************/
NLM_EXTERN void	DDV_GetArticleInfo(SeqEntryPtr sep,CharPtr szPopSetName,
	CharPtr szPopSetAuth,CharPtr szJournalTitle,Int4Ptr pmid)
{
BioseqSetPtr bssp;
PubdescPtr   pdp;
ValNodePtr   vnp3;

	if (!sep || !szPopSetName || !szPopSetAuth) return;
	
	*szPopSetName='\0';
	*szPopSetAuth='\0';
	*szJournalTitle='\0';
	*pmid=0;
	
	if (IS_Bioseq_set (sep)) {/*a Pop-set is a BSSP*/
		bssp = (BioseqSetPtr) sep->data.ptrvalue;		
		if (bssp){
			if(bssp->descr != NULL) {
				for (vnp3=bssp->descr ; vnp3 != NULL ; vnp3 = vnp3->next){
					if(vnp3->choice==Seq_descr_pub){
    					pdp = (PubdescPtr) vnp3->data.ptrvalue;
						if (pdp){
							DDV_AnalysePubDesc(pdp,szPopSetName,
								szPopSetAuth,szJournalTitle,pmid);
						}
					}
				}
			}
		}
	}
	if (*(szPopSetName)=='\0') StringCpy(szPopSetName,"* no title *");
	if (*(szPopSetAuth)=='\0') StringCpy(szPopSetAuth,"* no authors *");
}

/*******************************************************************************

  Function : DDV_AnalyseBiosource()
  
  Purpose : analyse a BioSource pointer and get TaxName and common name
  
  Parameters : 	biop; BioSource pointer
  				szTaxName; fills here with a TaxName
				szCommonName;fills here with the first common name
  
  Return value : - (see 'name')

*******************************************************************************/
static void	DDV_AnalyseBiosource(BioSourcePtr biop, CharPtr PNTR szTaxName,
		CharPtr PNTR szCommonName)
{

	if (!biop) return;

	if (biop->org && biop->org->taxname){
		*szTaxName=biop->org->taxname;
	}
	if (biop->org && biop->org->common){
		*szCommonName=biop->org->common;
	}
	
}

/*******************************************************************************

  Function : DDV_GetEntryBioSource()
  
  Purpose : retrieve the BioSource, given a sep
  
  Note : this function can be called several times to fill the valnode vnpp
  with all the Biosources of all SeqSet of an Entry. This is used, for example,
  by the PopSet Viewer to build the organim names list.
  
  Parameters : 	sep; seqentry
  				vnpp; list to fill. data.ptrvalue if of type PopSourcePtr (see pgppop.h)

*******************************************************************************/

NLM_EXTERN void	DDV_GetEntryBioSource(SeqEntryPtr sep,ValNodePtr PNTR vnpp)
{
BioseqSetPtr bssp;
BioSourcePtr biop;
ValNodePtr   vnp;
PopSourcePtr psp;

	if (!sep || !vnpp) return;
	
	if (IS_Bioseq_set (sep)) {/*a Pop-set is a BSSP*/
		bssp = (BioseqSetPtr) sep->data.ptrvalue;		
		if (bssp){
			if(bssp->descr != NULL) {/*scan the 'desc' field of the bssp*/
				for (vnp=bssp->descr ; vnp != NULL ; vnp = vnp->next){
					if(vnp->choice==Seq_descr_source){/*BioSource ?*/
    					biop = (BioSourcePtr) vnp->data.ptrvalue;
						if (biop){
							psp=(PopSourcePtr)MemNew(sizeof(PopSource));
							if (psp){
								DDV_AnalyseBiosource(biop,&(psp->szTaxName),
									&(psp->szCommonName));
								if (!(*vnpp)) *vnpp=ValNodeAddPointer(NULL,0,(Pointer)psp);
								else ValNodeAddPointer(vnpp,0,(Pointer)psp);
							}
						}
					}
				}
			}
		}
	}
}

/*******************************************************************************

  Function : SearchBioSource()
  
  Purpose : caalback for SeqEntryExplore to get all the biosources of all set
  in a popset entry, fo example
  
  Parameters : see Toolbox doc !
  
  Return value : none 

*******************************************************************************/
static void SearchBioSource(SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)
{
ValNodePtr PNTR	vnpp;

	vnpp=(ValNodePtr PNTR)mydata;

	if (IS_Bioseq_set (sep)) {
		DDV_GetEntryBioSource(sep, vnpp);
	}
}

/*******************************************************************************

  Function : DDV_DisplayDefaultAlign()
  
  Purpose : build a default SeqAlign display.
  
  Parameters : 	sap; SeqAlign
				gi;gi number - used by PopSet viewer to build the HTTP links
				from - to; range of the Sap to display
				disp_format; display formats (see pgppop.h)
				disp_data; for future implementation
				fp; to send the output
  Return value : FALSE if failed 

*******************************************************************************/
NLM_EXTERN Boolean DDV_DisplayDefaultAlign(SeqAlignPtr sap,Int4 gi,Int4 from,Int4 to,
		Uint4 disp_format,Pointer disp_data,FILE *fp)
{
SABlockPtr		sabp=NULL;/*indexed SeqAlign*/
Int4			sapLength;/*indexed SeqAlign size*/
MsaParaGPopList mppl;/*contains the data for the display*/
Int4			numBlockAffich=0,restdiv;
Int2            LineSize;
DDVOptionsBlockPtr dobp;
Int4Ptr PNTR    matrix;
ByteStorePtr   PNTR byteSpp;
DDV_Disp_Opt   ddo;

	MemSet(&mppl,0,sizeof(MsaParaGPopList));
	MemSet(&ddo,0,sizeof(DDV_Disp_Opt));

	if (disp_data){
		dobp=(DDVOptionsBlockPtr)disp_data;
		LineSize=dobp->LineSize;
		byteSpp=dobp->byteSpp;
		matrix=dobp->matrix;
		if (dobp->ddop) MemCpy(&ddo,dobp->ddop,sizeof(DDV_Disp_Opt));
	}
	else{
		LineSize=ParaG_Size;
		byteSpp=NULL;
		matrix=NULL;
		DDV_InitDefSAPdispStyles(&ddo);
	}

	if (disp_format&DISP_FASTA_NOGAP){/*don't need a ParaG structure*/
		DDV_GetFullFASTAforIdxAli(sap,fp);
	}
	else{/*HTML, gapped FASTA and PHYLIP: need a ParaG structure*/
		/*build the ParaG list for the display*/
		if (from==(Int4)-1 || to==(Int4)-1){
			if (!DDV_CreateDisplayFromIndex_EX(sap,&mppl,LineSize,&ddo,
					(Int4)-1,(Int4)-1)){
				goto error;
			}
			/*see the 'Important note' 20 lines below*/
			if (AlnMgrIsSAPDiscAli(sap))
				sapLength = mppl.LengthAli;
			else
				sapLength=AlnMgrGetAlnLength(mppl.sap, FALSE);
			from=0;
			to=sapLength-1;
		}
		else{
			/*for a nice view, always start as a multiple of LETTER_BLOCK_WIDTH*/
			restdiv=from%LETTER_BLOCK_WIDTH;
			from=from-restdiv;
			/*right now, this function is able to build a display for master-
			slave as well as disc. align. But note that 'from,to' don't work
			for a disc seqalign. To allow that, it's necassary to update the
			function UABuildDescriptor() in ddvcreate.c*/
			if (!DDV_CreateDisplayFromIndex_EX(sap,&mppl,LineSize,&ddo,
					from,to)){
				goto error;
			}
			/*Important note : sapLength MUST be the full length of the SeqAlign.
			Example : you have a master-slave SeqAlign, 1000 letters long.
			from=25 and to=250. mppl->LengthAli will be 250-25+1 but 
			sapLength will be 1000.
			Because right now it's not possible to only display a small region
			of a disc. seqalign, mppl->LengthAli==sapLength. As soon as it'll
			be possible to use from,to for a disc align, the following if-then
			will have to be updated*/
			if (AlnMgrIsSAPDiscAli(sap))
				sapLength = mppl.LengthAli;
			else
				sapLength=AlnMgrGetAlnLength(mppl.sap, FALSE);
		}
		/*show by block ?*/
		if ((to-from+1)<sapLength) {
			numBlockAffich=from/LineSize+1;
		}		

		/*Create the display */
		if (disp_format&DISP_FASTA_GAP){/*gapped FASTA output*/
			DDV_GetFullGapFASTAforIdxAli(&mppl,fp);
		}
		else{/*HTML of Phylip output*/
			DDV_AffichageParaG(&mppl,gi,from,to,sapLength,numBlockAffich,disp_format,
				LineSize,fp,byteSpp,matrix,NULL,NULL);
		}

		/*done... delete data*/
		DDV_DeleteDisplayList(&mppl);
	}
error:
	/*if (sabp) CleanupBlocks(sabp);*/
	
	return(TRUE);
}

/*******************************************************************************

  Function : DDV_ReadSeqGivenSpp()
  
  Purpose : read a sequence buffer.
  
  Parameters : 	spp; open seqport
  				from-to; range to read
				strand; one of the Seq_strand_... (see objloc.h)
				IsAA; TRUE if the sequence is a protein
				bError; set to TRUE only if an error occurs when reading the seq.
				
  Return value : a buffer with the sequence 

*******************************************************************************/
NLM_EXTERN CharPtr DDV_ReadSeqGivenSpp(SeqPortPtr spp,Int4 from,Int4 to,Uint1 strand,
	Boolean IsAA,BoolPtr bError)
{
Int4 len,seek,i;
CharPtr szSeq=NULL;

Byte residue;

	if (!spp) return (NULL);
	if (from>to) return (NULL);

	len=to-from+1;
	szSeq=(CharPtr)MemNew((len+1)*sizeof(Char));		
	if (!szSeq) return (NULL);
	if(IsAA) {
		seek=from;
	} 
	else {
		if(strand == Seq_strand_minus)
			seek = to;
		else
			seek = from;
	}
	SeqPortSeek(spp, seek, SEEK_SET);
	for(i=0;i<len;i++){
		residue = SeqPortGetResidue(spp);
		if (residue!=SEQPORT_EOF && IS_residue(residue)) {
			szSeq[i] = (Char)residue;
		}
	}

	return(szSeq);
}


/*******************************************************************************

  Function : DDV_OpenBspFullSeqPort()
  
  Purpose : open a seqport of a full sequence.
  
  Parameters : 	bsp; bioseq (be sure it's locked)
				strand; one of the Seq_strand_... (see objloc.h)

  Return value : an open seqport 

*******************************************************************************/
NLM_EXTERN SeqPortPtr DDV_OpenBspFullSeqPort(BioseqPtr bsp,Uint1 strand)
{
SeqPortPtr spp;

	if (!bsp) return (NULL);
		
	if(bsp->mol!=Seq_mol_aa) {
    	spp= SeqPortNew(bsp, (Int4)0,(Int4) bsp->length-1, 
			strand, Seq_code_iupacna);
	}
	else {
    	spp= SeqPortNew(bsp, (Int4)0,(Int4) bsp->length-1, 
			0, Seq_code_ncbistdaa);
	}
	return(spp);
}

/*******************************************************************************

  Function : DDV_BspInfoNew()
  
  Purpose : BspInfo constructor.
  
  Parameters : 	InitInfo; TRUE means init the 'ascii' fields of BspInfo
  				InitSeqPort; TRUE means open a seqport
				sip; sequence identifier
				strand; one of the Seq_strand_... (see objloc.h)

  Return value : a pointer to an initialized BspInfo structure 

*******************************************************************************/
NLM_EXTERN BspInfoPtr DDV_BspInfoNew(Boolean InitInfo,Boolean InitSeqPort,SeqIdPtr sip,
		Uint1 strand)
{
BspInfoPtr bip;


	if (!sip) return(NULL);

	/*alloc*/
	bip=(BspInfoPtr)MemNew(sizeof(BspInfo));
	if (!bip) return(NULL);
	
	/*lock the bsp in memory*/
	bip->bsp=BioseqLockById(sip);
	if (!bip->bsp) return(NULL);
	bip->sip=sip;
	/*init Bsp info fields*/
	if (InitInfo){
		UDV_ReadBspDataForViewer(bip);
	}
	
	/*open the seqport on the full sequence*/
	if (InitSeqPort){
		bip->spp=DDV_OpenBspFullSeqPort(bip->bsp,strand);
	}
	
	return(bip);
}


/*******************************************************************************

  Function : DDV_BspInfoDelete()
  
  Purpose : BspInfo destructor.
  
  Parameters : 	bip; a pointer to an initialized BspInfo structure

  Return value : NULL, always 

*******************************************************************************/
NLM_EXTERN BspInfoPtr DDV_BspInfoDelete(BspInfoPtr bip)
{ 
	if(bip) {
		if (bip->spp) SeqPortFree(bip->spp);
		if (bip->bsp) BioseqUnlock(bip->bsp);
		if (bip->bspGeneticCode) MemFree(bip->bspGeneticCode);
		if (bip->SeqBuf) MemFree(bip->SeqBuf);
		MemFree(bip);
	}
	return(NULL);
}

/*******************************************************************************

  Function : DDV_BspInfoDeleteList()
  
  Purpose : BspInfo list destructor.
  
  Parameters : 	bip; a pointer to an initialized BspInfo structure

  Return value : NULL, always 

*******************************************************************************/
NLM_EXTERN BspInfoPtr DDV_BspInfoDeleteList(BspInfoPtr bip)
{
BspInfoPtr next;

	while (bip != NULL){
		next = bip->next;
		DDV_BspInfoDelete(bip);
		bip = next;
	}
	return(NULL);
}

/*******************************************************************************

  Function : DDV_LocateParaG()
  
  Purpose : compute StartLine valies for PGP in a Full Horz display style 
  
  Parameters : -
  
  Return value : -

*******************************************************************************/
NLM_EXTERN void DDV_LocateParaG(ValNodePtr PNTR TableHead,Int4 nBsp)
{
ParaGPtr pgp;
Int4 StartLine=0,StartLine2=0,i;
ValNodePtr vnp;
Boolean bFirst;
	
	if (TableHead==NULL || nBsp==0) return;
	
	for(i=0;i<nBsp;i++){
		StartLine=StartLine2;
		bFirst=TRUE;
		for(vnp=TableHead[i];vnp!=NULL;vnp=vnp->next){
			pgp=(ParaGPtr)(vnp->data.ptrvalue);
			if (pgp){
				pgp->StartLine=StartLine;
				if (bFirst) {
					StartLine2+=pgp->nLines;
					bFirst=FALSE;
				}
			}
		}
	}	
}

/*******************************************************************************

  Function : DDV_CreateDisplay()
  
  Purpose : create a display for DDV panel.
  
  Parameters : 	sap; seqalign
				mpplp; allocated structure filled here

  Return value : TRUE if success

*******************************************************************************/
NLM_EXTERN Boolean DDV_CreateDisplay(SeqAlignPtr sap,MsaParaGPopListPtr mpplp)
{
Int4 numBlocks;
Boolean nRet;

	if (!sap) return(FALSE);

	/*build the indexed SeqAlign*/
	mpplp->sabp=Blockify(sap);
	if (!mpplp->sabp) return(FALSE);

	/*retrieve the size of the alignment*/
	GetBlockInfo(mpplp->sabp, &mpplp->LengthAli, &mpplp->nBsp,&numBlocks,NULL);
	
	/*build the ParaG list for the display*/
	nRet=DDV_PopDisplay2(mpplp->sabp,mpplp,mpplp->nBsp,SCALE_POS_NONE,
			TRUE,ParaG_Size,numBlocks,0,mpplp->LengthAli-1);

	DDV_LocateParaG(mpplp->TableHead,mpplp->nBsp);
	
	return(nRet);
}

NLM_EXTERN Boolean DDV_ShowSeqAlign(SeqAlignPtr seqalign, Int4 gi, Int4 from, Int4 to,
                         Uint4 disp_format)
{
    
    SABlockPtr sabp;       /* indexed SeqAlign */
    Int4  sapLength, sapNumBioseqs, numBlocks;/* indexed SeqAlign size */
    Int4  restdiv;         /* used to modify the udp->from value */
    MsaParaGPopList mppl;  /* contains the data for the display */
    Char szPopSetName[INFO_SIZE+1]={""};/*article title of the Pop-Set*/
    /*article first author name of the Pop-Set*/    
    Char szPopSetAuth[INFO_SIZE+1]={""};
    Int4 numBlockAffich=0;
    
    /*seqalign = PairSeqAlign2MultiSeqAlign(seqalign);*/
    
    /*build the indexed SeqAlign*/
    if((sabp = Blockify(seqalign)) == NULL) {
        printf("Error No. 54. Could not show align");
        return FALSE;
    }
    
    /*retrieve the size of the alignment*/
    GetBlockInfo(sabp, &sapLength, &sapNumBioseqs,&numBlocks,NULL);
    
    if (disp_format & DISP_FASTA_NOGAP){ /*don't need a ParaG structure*/
        DDV_GetFullFASTAforIdxAli(seqalign, stdout);
    } 
	else {   /* HTML, gapped FASTA and Phylip : 
		need to create the ParaG structure */
        /* for a nice view, always start as a multiple of LETTER_BLOCK_WIDTH */
        restdiv = from % LETTER_BLOCK_WIDTH;
        from = from-restdiv;
        
        /*user didn't give from-to; from=0 and to=sapLength-1*/
        if (to < 1) to = sapLength-1;
        
        /*some security*/
        
        if (from >= to) 
            return FALSE;
        
        if (from < 0 || to < 0) 
            return FALSE;
        
        /* build the ParaG list for the display */
        DDV_PopDisplay2(sabp, &mppl, sapNumBioseqs, SCALE_POS_TOP,
                        TRUE, ParaG_Size, numBlocks, from, to);
        
        /*show by block ?*/
        if ((to - from + 1) < sapLength) {
            numBlockAffich = from/ParaG_Size+1;
        }	
                
		if (disp_format&DISP_FASTA_GAP){/*gapped FASTA output*/
			DDV_GetFullGapFASTAforIdxAli(&mppl,stdout);
		}
        /*Create the HTML display for the Population division*/
		else{/*HTML of Phylip output*/
	        DDV_AffichageParaG(&mppl, gi, from, to, sapLength,
            numBlockAffich, disp_format, ParaG_Size,stdout,NULL,NULL,NULL,NULL);
		}
        /*done... delete data*/
        DDV_DeleteDisplayList(&mppl);
    }
    
    if (sabp) CleanupBlocks(sabp);
    
    return TRUE;
}

/*******************************************************************************

  Function : DDV_SearchAli()
  
  Purpose : callback of SeqEntryExplore from  DDV_GetSeqAlign()
  
  Parameters : see toolkit code...
  
  Return value : none 

*******************************************************************************/
NLM_EXTERN void DDV_SearchAli(SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)
{
ValNodePtr PNTR	vnpp;
BioseqPtr     bsp;
BioseqSetPtr  bssp;
SeqAnnotPtr   sap,sap2;
SeqAlignPtr   salp;

	vnpp=(ValNodePtr PNTR)mydata;
	
	/*Bioseq or BioseqSet ?... the SeqAlign (if any) is in the seqannot part*/
	if (IS_Bioseq (sep)) {
		bsp = (BioseqPtr) sep->data.ptrvalue;
		sap = bsp->annot;
	} else if (IS_Bioseq_set (sep)) {
		bssp = (BioseqSetPtr) sep->data.ptrvalue;
		sap = bssp->annot;
	} else return;
	sap2=sap;
	while (sap2 != NULL) {
		if (sap2->type == 2) {/*seqalign*/
			salp=(SeqAlignPtr)sap2->data;
			while(salp){
				if (!(*vnpp)) *vnpp=ValNodeAddPointer(NULL,OBJ_SEQALIGN,(Pointer)salp);
				else ValNodeAddPointer(vnpp,OBJ_SEQALIGN,(Pointer)salp);
				salp=salp->next;
			}
		}
		sap2=sap2->next;
	}
}

/*******************************************************************************

  Function : DDV_GetSeqAlign()
  
  Purpose : given dataptr and datatype, look for any SeqAligns
  
  Parameters : dataptr; the data to analyse
  				datatype; OBJ_BIOSEQ,OBJ_SEQENTRY, etc.
				vnp; filled here. data.ptrvalue == SeqALignPtr
  
  Return value : none 

*******************************************************************************/
NLM_EXTERN void DDV_GetSeqAlign(Pointer dataptr,Uint2 datatype,
		ValNodePtr PNTR vnp)
{
BioseqPtr     bsp;
BioseqSetPtr  bssp;
SeqEntryPtr   sep;
SeqAnnotPtr   sap;

	if(datatype==OBJ_SEQENTRY){
		SeqEntryExplore((SeqEntryPtr)dataptr,(Pointer) vnp,DDV_SearchAli);
	} 
	else if(datatype==OBJ_BIOSEQ || datatype==OBJ_BIOSEQSET){
		/*create a seqentry; then explore it*/
		sep = SeqEntryNew ();
		if (sep != NULL) {
			if (datatype == OBJ_BIOSEQ) {
				bsp = (BioseqPtr) dataptr;
				sep->choice = 1;
				sep->data.ptrvalue = bsp;
			} else if (datatype == OBJ_BIOSEQSET) {
				bssp = (BioseqSetPtr) dataptr;
				sep->choice = 2;
				sep->data.ptrvalue = bssp;
			}
			SeqEntryExplore(sep,(Pointer) vnp,DDV_SearchAli);
			SeqEntryFree (sep);
		}
	}
	else if(datatype==OBJ_SEQALIGN){
		ValNodeAddPointer(vnp,OBJ_SEQALIGN,(SeqAlignPtr)dataptr);
	}
	else if(datatype==OBJ_SEQANNOT){
		sap=(SeqAnnotPtr)dataptr;
		while (sap != NULL) {
			if (sap->type == 2) {/*seqalign*/
				ValNodeAddPointer(vnp,OBJ_SEQALIGN,sap->data);
			}
			sap=sap->next;
		}
	}
}

/*******************************************************************************

  Function : DDV_PrintPopSetSummary()
  
  Purpose : display a summary for a PopSet entry
  
*******************************************************************************/
NLM_EXTERN void DDV_PrintPopSetSummary(SeqEntryPtr sep, Int4 gi, FILE *FileOut)
{
ValNodePtr  vnp_sap,vnp_biosrc,vnp_biosrc2,vnp,vnp2;
SeqAlignPtr sap;
SABlockPtr  sabp;
Int4		sapLength, sapNumBioseqs, numBlocks;
Uint2	    i;	
Boolean bPairwise=TRUE,bPrintTax,bPrintCom;
Uint4 disp_format;
PopSourcePtr psp,psp2;

	disp_format=RULER_TOP;
	disp_format|=RULER_TICK;
	disp_format|=DISPE_SHOWBLOCK;
	disp_format|=VIEW_FULLSEQ;
	disp_format|=DISP_FULL_HTML;
	disp_format|=DISP_ORDER_NUM; 
	disp_format|=DISP_STRAND;
	disp_format|=DISP_BSP_COORD;

	/*scan the SeqEntry and get all the SeqALigns*/
	vnp_sap=NULL;
	DDV_GetSeqAlign((Pointer) sep, OBJ_SEQENTRY, &vnp_sap);

	/*scan the SeqEntry and get all the biosources*/
	vnp_biosrc=NULL;
	SeqEntryExplore(sep,(Pointer) &vnp_biosrc,SearchBioSource);

	fprintf(FileOut,"<UL>\n");

	/*summary of biosource; if a name occurs several times, display only once*/
	if (vnp_biosrc){
		vnp_biosrc2=NULL;
		vnp=vnp_biosrc;
		fprintf(FileOut,"<B>Source of the sequences : </B><BR><UL>");
		while(vnp){
			psp=(PopSourcePtr)vnp->data.ptrvalue;
			if (psp->szTaxName) bPrintTax=TRUE;
			else bPrintTax=FALSE;
			if (psp->szCommonName) bPrintCom=TRUE;
			else bPrintCom=FALSE;
			if (bPrintTax || bPrintCom){
				if (vnp_biosrc2){
					vnp2=vnp_biosrc2;
					while(vnp2){
						psp2=(PopSourcePtr)vnp2->data.ptrvalue;
						if (psp->szTaxName && psp2->szTaxName){
							if (!StringCmp(psp->szTaxName,psp2->szTaxName)) bPrintTax=FALSE;
						}
						if (psp->szCommonName && psp2->szCommonName){
							if (!StringCmp(psp->szCommonName,psp2->szCommonName)) bPrintCom=FALSE;
						}
						vnp2=vnp2->next;
					}
					if (bPrintTax || bPrintCom){
						fprintf(FileOut,", \n");
						ValNodeAddPointer(&vnp_biosrc2,0,(Pointer)psp);
					}
				}
				else{
					vnp_biosrc2=ValNodeAddPointer(NULL,0,(Pointer)psp);
				}
				if (bPrintTax && bPrintCom){
					fprintf(FileOut,"%s (%s)",psp->szTaxName,psp->szCommonName);
				}
				else if (bPrintTax && !bPrintCom){
					fprintf(FileOut,"%s",psp->szTaxName);
				}
				else if (!bPrintTax && bPrintCom){
					fprintf(FileOut,"%s",psp->szCommonName);
				}
			}
			vnp=vnp->next;
		}
		fprintf(FileOut,".<BR></UL>\n");
		ValNodeFreeData(vnp_biosrc);
		if (vnp_biosrc2) ValNodeFreeData(vnp_biosrc2);
	}
	
	fprintf(FileOut,"<B>Content :</B><BR><BR><UL>\n");
	/*scan vnp_sap and display the summary for SeqAlign*/
	if(vnp_sap){
		i=0;
		vnp=vnp_sap;
		fprintf(FileOut,"<B>List of sequence alignment(s) : </B><BR><UL>");
		while(vnp){
			/*get a SeqAlign*/
			sap = (SeqAlignPtr) vnp->data.ptrvalue;			
			/*build the indexed SeqAlign*/
			AlnMgrIndexSeqAlign(sap);
			/*retrieve the size of the alignment*/
			sapLength=AlnMgrGetAlnLength(sap, FALSE);
			sapNumBioseqs=AlnMgrGetNumRows(sap);
			if (sapNumBioseqs>2) 
				bPairwise=FALSE;
			fprintf(FileOut,"Sequence Alignment no. %i : %i sequences, %i letters ",
					i+1,sapNumBioseqs,sapLength);
			if (gi!=0) {
				fprintf(FileOut," (<a href=\"wwwddv.cgi?gi=%i\">Text view</a>).",gi);
			}
			fprintf(FileOut,"<BR>\n");
			vnp=vnp->next;i++;
		}

		fprintf(FileOut,"<BR></UL>\n");
		ValNodeFree(vnp_sap);
	}
	else{/*summary if no SeqAlign*/
		
		fprintf(FileOut,"This entry contains no sequence alignment.<BR>");
		if (gi!=0) {
			fprintf(FileOut,"Yan can either :<BR><BR>");
			fprintf(FileOut,"<a href=\"wwwddv.cgi?gi=%i\"",gi);
			fprintf(FileOut," onMouseOut=\"window.status='';return true\" \
onMouseOver=\"window.status='Click to see the list of sequences'; return true\" >");
			fprintf(FileOut,"- view the list of sequences</a>, or <BR>\n");
		}
	}
	fprintf(FileOut,"</UL></UL>\n");
}

/**************************************************************
***************************************************************
COLOMBE FOR KARL 
***************************************************************
***************************************************************/

NLM_EXTERN void PrintSeqAlignCallback (SeqEntryPtr sep, Pointer mydata,
                                   Int4 index, Int2 indent)
{
  BioseqPtr          bsp;
  BioseqSetPtr       bssp;
  SeqAnnotPtr        sap;
  SeqAlignPtr        salp,salp2;
  Uint4              option = 0;
  ByteStorePtr PNTR byteSpp;
  DDVOptionsBlockPtr dobp = (DDVOptionsBlockPtr) mydata;
  
  if (sep != NULL && sep->data.ptrvalue)
  {
     option = RULER_TOP;
     option|=DISPE_SHOWBLOCK;
     option|=VIEW_VARIA;
     option|=DISP_FULL_TXT;
     option|=DISP_STRAND;
     option|=DISP_BSP_COORD;

     if (IS_Bioseq(sep))
     {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        if (bsp!=NULL) {
           for (sap=bsp->annot;sap!=NULL;sap=sap->next)
           {
              if (sap->type == 2) {
                 salp = (SeqAlignPtr)sap->data;
                 salp2 = PairSeqAlign2MultiSeqAlign (salp);
                 if (salp2==NULL) {
                    salp2 = (SeqAlignPtr) AsnIoMemCopy (salp, (AsnReadFunc) SeqAlignAsnRead, (AsnWriteFunc) SeqAlignAsnWrite);   
                 }
                 DDV_DisplayDefaultAlign (salp2, 0, -1,-1, option, dobp, NULL);
                 SeqAlignSetFree(salp2);
                 BSWrite(*(dobp ->byteSpp),"\n//\n",4);
              }
           }
        }
     }
     else if(IS_Bioseq_set(sep)) {
        bssp = (BioseqSetPtr)sep->data.ptrvalue;
        if (bssp!=NULL) {
           for (sap=bssp->annot;sap!=NULL;sap=sap->next)
           {
              if (sap->type == 2) {
                 salp = (SeqAlignPtr)sap->data;
                 salp2 = PairSeqAlign2MultiSeqAlign (salp);
                 if (salp2==NULL) {
                    salp2 = (SeqAlignPtr) AsnIoMemCopy (salp, (AsnReadFunc) SeqAlignAsnRead, (AsnWriteFunc) SeqAlignAsnWrite);   
                 }
                 DDV_DisplayDefaultAlign (salp2, 0, -1,-1, option, dobp, NULL);
                 SeqAlignSetFree(salp2);
                 BSWrite(*(dobp ->byteSpp),"\n//\n",4);
              }
           }
        }
     }
  }
}

NLM_EXTERN ByteStorePtr SeqAlignToBS (Uint2 entityID)
{{
  SeqEntryPtr sep;
  ByteStorePtr bsp = NULL;
  FILE *fp;
  DDVOptionsBlockPtr dobp=MemNew(sizeof (DDVOptionsBlock));

  dobp->LineSize=50; 
  dobp->byteSpp = & bsp;
  sep = GetTopSeqEntryForEntityID (entityID);
  if (sep) {
     SeqEntryExplore (sep, (Pointer)dobp, PrintSeqAlignCallback);
  }
  dobp->byteSpp=NULL;
  MemFree(dobp);
  return bsp;
}}


NLM_EXTERN DDVRulerDescrPtr DDV_RulerDescrNew(DDVRulerDescrPtr pRulerDescr) {
/*******************************************************************************
*
*  make a copy of RulerDescr.
*  return pointer to new RulerDescr.
*
*******************************************************************************/
  DDVRulerDescrPtr  pCopy;

  pCopy = MemNew(sizeof(DDVRulerDescr));
  ASSERT(pCopy != NULL);
  MemCopy(pCopy, pRulerDescr, sizeof(DDVRulerDescr));
  return(pCopy);
}


NLM_EXTERN DDVRulerDescrPtr DDV_RulerDescrFree(DDVRulerDescrPtr pRulerDescr) {
/*******************************************************************************
*
*  free memory used for RulerDescr.
*  returns NULL for successful completion.
*
*******************************************************************************/
  VERIFY(MemFree(pRulerDescr) == NULL);
  return(NULL);
}

/****/

