/*   $Id: ddvcolor.c,v 1.22 2000/03/06 18:35:22 thiessen Exp $
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
* File Name:  $Id: ddvcolor.c,v 1.22 2000/03/06 18:35:22 thiessen Exp $
*
* Author:  Lewis Geer
*
* Version Creation Date:   6/4/99
*
* $Revision: 1.22 $
*
* File Description: Shared color information for viewers
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: ddvcolor.c,v $
* Revision 1.22  2000/03/06 18:35:22  thiessen
* fixes for 8-bit color
*
* Revision 1.21  2000/02/23 18:56:29  thiessen
* move to 1-based row numbers
*
* Revision 1.20  2000/02/15 22:40:57  lewisg
* add ability to launch udv so that it colors by row, fixes to colormgr, track rows from viewmgr, fix visual c projects
*
* Revision 1.19  2000/02/10 18:40:19  lewisg
* fix a freeing order for colorglobal
*
* Revision 1.18  2000/02/10 15:51:58  lewisg
* cn3d responds and send correct update messages.  many coloring bug fixes
*
* Revision 1.17  2000/02/10 14:15:18  durand
* add a function to build Feature colors table
*
* Revision 1.16  2000/01/28 19:25:39  lewisg
* columnwise binary search on color data
*
* Revision 1.15  1999/12/29 22:55:02  lewisg
* get rid of seqalign id
*
* Revision 1.14  1999/12/13 23:20:43  lewisg
* bug fixes: duplicate color structures eliminated, clear color doesn't clear case
*
* Revision 1.13  1999/12/11 01:30:34  lewisg
* fix bugs with sharing colors between ddv and cn3d
*
* Revision 1.12  1999/12/10 20:59:22  durand
* add support for italic, bold and underlined letters
*
* Revision 1.11  1999/11/24 21:24:23  vakatov
* Fixed for the C++ and/or MSVC DLL compilation
*
* Revision 1.10  1999/11/24 15:23:19  lewisg
* added color selection dialogs for SS
*
* Revision 1.9  1999/11/22 17:29:53  lewisg
* add back color selection dialogs, fix viewer3d half-selection bug
*
* Revision 1.8  1999/11/02 23:06:08  lewisg
* fix cn3d to launch correctly if there is no seqentry associated with bioseq
*
* Revision 1.7  1999/11/01 22:10:26  lewisg
* add ability to call color functions by type, and add this to cn3d
*
* Revision 1.6  1999/10/25 20:28:48  lewisg
* move ddvcolor to libncbiobj
*
* Revision 6.9  1999/10/15 20:56:40  lewisg
* append DDV_ColorGlobal as userdata.  free memory when cn3d terminates.
*
* Revision 6.8  1999/10/08 23:20:36  lewisg
* add case and box attributes to DDV_ColorCell
*
* Revision 6.7  1999/10/05 23:18:26  lewisg
* add ddv and udv to cn3d with memory management
*
* Revision 6.6  1999/09/21 19:50:36  kans
* cast on function parameter needed
*
* Revision 6.5  1999/09/21 18:09:13  lewisg
* binary search added to color manager, various bug fixes, etc.
*
* Revision 1.10  1999/09/03 23:27:32  lewisg
* minor speedups by avoiding casts
*
* Revision 1.9  1999/09/03 14:01:40  lewisg
* use faster seqid compare SAM_CompareID
*
* Revision 1.8  1999/09/01 23:02:59  lewisg
* binary search in color functions
*
* Revision 1.7  1999/08/13 22:08:16  lewisg
* color manager updated to use alignment coords
*
* Revision 1.6  1999/08/09 18:09:33  lewisg
* copy of ddvcolor from distrib to avoid changing daily build
*
* Revision 6.4  1999/07/20 14:38:11  durand
* update DDV_InRange to include the ends in the test
*
* Revision 6.3  1999/07/19 19:12:21  lewisg
* fix bug adding new pColor in old pMediaInfo.  Also added ability to detect overlaps when allocating new pColor
*
* Revision 6.2  1999/07/19 14:37:03  lewisg
* fix bug when allocating mediainfo with from != 0
*
* Revision 6.1  1999/07/16 18:46:46  lewisg
* moved ddvcolor from api to tools
*
* Revision 1.4  1999/07/13 23:24:48  lewisg
* added DDV_SipList
*
* Revision 1.3  1999/07/13 14:38:39  lewisg
* separated out networking code
*
* Revision 1.4  1999/06/24 17:48:28  lewisg
* added SpecialColors to global
*
* Revision 1.3  1999/06/16 13:51:50  lewisg
* added palette management functions from cn3d
*
* Revision 1.2  1999/06/14 17:20:47  lewisg
* minor bug fix for leaks
*
* Revision 1.1  1999/06/11 23:37:09  lewisg
* color management functions
*
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <objfdef.h>
#include <sequtil.h>
#include <ddvcolor.h>
#include <salutil.h>
#include <samutil.h>


/*****************************************************************************
*
*   get a color for a Feature. 
*
*****************************************************************************/
NLM_EXTERN DDV_ColorCell DDV_GetFeatColor(DDV_ColorGlobal* pColorGlobal, Int1 idx)
{
DDV_ColorCell dcc;

	memset(&dcc,0,sizeof(DDV_ColorCell));
	if (idx<0 || idx>=FEATDEF_MAX) return(dcc);
	if (!pColorGlobal->SpeClr.pFeatColorTable) return(dcc);
	
	return(pColorGlobal->SpeClr.pFeatColorTable[idx]);
}

/*****************************************************************************
*
*   setup a DDV_ColorCell with r,g,b values. 
*
*****************************************************************************/
NLM_EXTERN DDV_ColorCell DDV_GetColorRGB(Uint1 r, Uint1 g, Uint1 b)
{
DDV_ColorCell dcc;

	memset(&dcc,0,sizeof(DDV_ColorCell));
	dcc.rgb[0]=r;
	dcc.rgb[1]=g;
	dcc.rgb[2]=b;
	
	return(dcc);
}

/*****************************************************************************
*
*   Initialisation of the Feature colors table.  
*
*****************************************************************************/
static DDV_ColorCell * DDV_InitFeatureColors(void)
{
DDV_ColorCell * pClr;
Uint1    i;

	pClr=(DDV_ColorCell *)MemNew(FEATDEF_MAX*sizeof(DDV_ColorCell));
	if (!pClr) return(NULL);
	for (i=0;i<FEATDEF_MAX;i++)	
		pClr[i]=DDV_GetColorRGB(0,0,0);

	/*Warning: values are one-based*/
	pClr[FEATDEF_GENE]=DDV_GetColorRGB(128,128,128);			/*gray*/
	
	pClr[FEATDEF_precursor_RNA]=DDV_GetColorRGB(0,0,255);/*blue*/
	pClr[FEATDEF_misc_RNA]=DDV_GetColorRGB(0,0,255);
	pClr[FEATDEF_preRNA]=DDV_GetColorRGB(0,0,255);
	pClr[FEATDEF_mRNA]=DDV_GetColorRGB(0,0,255);
	pClr[FEATDEF_tRNA]=DDV_GetColorRGB(0,0,255);
	pClr[FEATDEF_rRNA]=DDV_GetColorRGB(0,0,255);
	pClr[FEATDEF_snRNA]=DDV_GetColorRGB(0,0,255);
	pClr[FEATDEF_scRNA]=DDV_GetColorRGB(0,0,255);
	pClr[FEATDEF_otherRNA]=DDV_GetColorRGB(0,0,255);
	pClr[FEATDEF_prim_transcript]=DDV_GetColorRGB(0,0,255);

	pClr[FEATDEF_CDS]=DDV_GetColorRGB(255,150,255);	/*pink*/
	pClr[FEATDEF_exon]=DDV_GetColorRGB(255,150,255);
	pClr[FEATDEF_intron]=DDV_GetColorRGB(255,150,255);

	pClr[FEATDEF_PROT]=DDV_GetColorRGB(0,128,0);		/*dk green*/
	pClr[FEATDEF_mat_peptide]=DDV_GetColorRGB(0,128,0);
	pClr[FEATDEF_sig_peptide]=DDV_GetColorRGB(0,128,0);
	pClr[FEATDEF_transit_peptide]=DDV_GetColorRGB(0,128,0);
	pClr[FEATDEF_preprotein]=DDV_GetColorRGB(0,128,0);
	pClr[FEATDEF_mat_peptide_aa]=DDV_GetColorRGB(0,128,0);
	pClr[FEATDEF_sig_peptide_aa]=DDV_GetColorRGB(0,128,0);
	pClr[FEATDEF_transit_peptide_aa]=DDV_GetColorRGB(0,128,0);


	pClr[FEATDEF_SITE]=DDV_GetColorRGB(255,0,0);		/*red*/

	pClr[FEATDEF_REGION]=DDV_GetColorRGB(210,154,14);		/*orange*/
	pClr[FEATDEF_mutation]=DDV_GetColorRGB(210,154,14);
	pClr[FEATDEF_variation]=DDV_GetColorRGB(210,154,14);

	pClr[FEATDEF_PSEC_STR]=DDV_GetColorRGB(104,201,220); /*cyan*/

	pClr[FEATDEF_HET]=DDV_GetColorRGB(128,128,0);	/*yellow*/

	pClr[FEATDEF_BOND]=DDV_GetColorRGB(255,92,255);	/*pink*/

	return(pClr);
}


/*****************************************************************************
*
*   Set default values for DDV_Range.  
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_DefaultRange(DDV_Range *pRange, Int4 lFrom, Int4 lTo)
{
    if(pRange == NULL || lFrom > lTo ) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_DefaultRange");
        return 0;
    }
    
    pRange->lFrom = lFrom;
    pRange->lTo = lTo;
    return 1;
}

/*****************************************************************************
*
*   Constructor for DDV_ColorEntry structure.  DDV_ColorEntry is a
*   (Red, Green, Blue) color triplet and associated szName.
*
*****************************************************************************/

NLM_EXTERN DDV_ColorEntry * DDV_CreateColorEntry
(Char *szName, Nlm_Uint1 Red, Nlm_Uint1 Green, Nlm_Uint1 Blue)
{
    DDV_ColorEntry *pColor;
    
    pColor = MemNew(sizeof(DDV_ColorEntry));
    if (pColor == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid malloc in DDV_CreateColorEntry");
        return NULL;
    }

    pColor->Name = StringSave(szName);
    pColor->Paints = NULL;
    pColor->Index = 0;
    DDV_SetColorCell(&pColor->ColorCell, Red, Green, Blue);
    pColor->ColorCell.BottomBox = FALSE;
    pColor->ColorCell.LeftBox = FALSE;
    pColor->ColorCell.LowerCase = FALSE;
    pColor->ColorCell.RightBox = FALSE;
    pColor->ColorCell.TopBox = FALSE;
    pColor->ColorCell.UseItalic = FALSE;
    pColor->ColorCell.UseBold = FALSE;
    pColor->ColorCell.UseUnderline = FALSE;

    return pColor;
}


/*****************************************************************************
*
*   Destructor for DDV_ColorEntry Color.
*
*****************************************************************************/

/* destructor for Color structure */
NLM_EXTERN void DDV_DeleteColorEntry(DDV_ColorEntry *Color)
{
    MemFree(Color->Name);
    MemFree(Color->Paints);
    MemFree(Color);
}


/*****************************************************************************
*
*   Frees a Palette.  Palette is a ValNode list of DDV_ColorEntries
*   Note that it frees the entire Valnode list pointed to by Palette and
*   sets Palette to NULL.
*
*****************************************************************************/

NLM_EXTERN void DDV_FreePalette(ValNode **Palette)
{
    ValNode *Index;
    
    if(Palette == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_FreePalette");
        return;
    }

    Index = *Palette;
    while (Index) {
        if (Index->data.ptrvalue)
          DDV_DeleteColorEntry((DDV_ColorEntry *)(Index->data.ptrvalue));
        Index = Index->next;
    }
    ValNodeFree(*Palette);
    *Palette = NULL;
}

/*****************************************************************************
*
*   Puts ColorCell on the Palette list if it isn't there already.  
*
*****************************************************************************/

NLM_EXTERN void DDV_RequestColor(ValNode **Palette, DDV_ColorCell *ColorCell)
{
    DDV_ColorEntry *NewColor;
    
    if(Palette == NULL || ColorCell == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_RequestColor");
        return;
    }

    if ( DDV_SearchColor(*Palette, ColorCell) == NULL ) {
        NewColor = DDV_CreateColorEntry
          ("", ColorCell->rgb[0], ColorCell->rgb[1], ColorCell->rgb[2]);
        if (!NewColor) return;
        ValNodeAddPointer(Palette, 0, NewColor);
    }
    return;
}

/*****************************************************************************
*
*   Puts ColorEntry on the Palette list if the sZname isn't there already,
*   Otherwise replaces the entry.  Does NOT make a copy of the DDV_ColorEntry
*
*****************************************************************************/

NLM_EXTERN void DDV_RequestColorbyName
(ValNode **Palette, DDV_ColorEntry *pColorIn)
{
    DDV_ColorEntry *pColorEntry;
    ValNode *pvn;
    
    if(Palette == NULL || pColorIn == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_RequestColorbyName");
        return;
    }

    pColorEntry = DDV_SearchColorbyName(*Palette, pColorIn->Name);
    if (pColorEntry == NULL) {
        ValNodeAddPointer(Palette, 0, pColorIn);
        return;
    }
    else {
        for(pvn = *Palette; pvn->data.ptrvalue != pColorEntry;
            pvn = pvn->next) continue;
        DDV_DeleteColorEntry((DDV_ColorEntry*)pvn->data.ptrvalue);
        pvn->data.ptrvalue = pColorIn;
    }
}


/*****************************************************************************
*
*   Looks for ColorCell on the Palette.  Returns * to the 
*   DDV_ColorEntry.
*
*****************************************************************************/

NLM_EXTERN DDV_ColorEntry * DDV_SearchColor
(ValNode *Palette, DDV_ColorCell *ColorCell)
{
    ValNode *pvnPalette = Palette;

    if(ColorCell == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_SearchColor");
        return NULL;
    }
 
    while(pvnPalette) {
        if(((DDV_ColorEntry *)(pvnPalette->data.ptrvalue))->ColorCell.rgb[0]
            == ColorCell->rgb[0] &&
            ((DDV_ColorEntry *)(pvnPalette->data.ptrvalue))->ColorCell.rgb[1]
            == ColorCell->rgb[1]  &&
            ((DDV_ColorEntry *)(pvnPalette->data.ptrvalue))->ColorCell.rgb[2]
            == ColorCell->rgb[2] )
            return (DDV_ColorEntry *)(pvnPalette->data.ptrvalue);
        pvnPalette = pvnPalette->next;
    }
    return NULL;
}

/*****************************************************************************
*
*   Looks for a DDV_ColorEntry on the Palette by szName.  Returns *
*   to the DDV_ColorEntry.
*
*****************************************************************************/

NLM_EXTERN DDV_ColorEntry * DDV_SearchColorbyName
(ValNode *Palette, Char *szName)
{
    ValNode *pvnPalette = Palette;

    while(pvnPalette) {
        if(StrCmp(((DDV_ColorEntry *)(pvnPalette->data.ptrvalue))->Name,
            szName) == 0 )
            return (DDV_ColorEntry *)(pvnPalette->data.ptrvalue);
        pvnPalette = pvnPalette->next;
    }
    return NULL;
}

/*****************************************************************************
*
*   Looks for a DDV_ColorEntry on the Palette by szName.  Returns *
*   to ColorCell in the DDV_ColorEntry.
*
*****************************************************************************/

NLM_EXTERN DDV_ColorCell * DDV_SearchColorCellbyName
(ValNode *Palette, Char *szName)
{
    DDV_ColorEntry *pColorEntry;

    pColorEntry = DDV_SearchColorbyName(Palette, szName);
    if(pColorEntry == NULL) return NULL;

    return &pColorEntry->ColorCell;
}

/*****************************************************************************
*
*   Sets the Index in a DDV_ColorEntry according to position in Palette
*
*****************************************************************************/

NLM_EXTERN void DDV_SetColorChoice(ValNode *Palette)
/* sets the valnode choice value according to position in palette */
{
    Int4 i = 0;
    ValNode *pvnPalette = Palette;

    if(Palette == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_SetColorChoice");
        return;
    }

    while(pvnPalette) {
        ((DDV_ColorEntry *)(pvnPalette->data.ptrvalue))->Index = i;
        i++;
        pvnPalette = pvnPalette->next;
    }
}

/*****************************************************************************
*
*   Returns the index into the Palette for a given ColorCell.
*   Return index on success, 0 on failure.
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_ColorIndex(ValNode *Palette, DDV_ColorCell *ColorCell)
/* returns the index into the palette for a given color */
{
    DDV_ColorEntry *Color;
    
    if(Palette == NULL || ColorCell == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_ColorIndex");
        return 0;
    }

    Color = DDV_SearchColor(Palette, ColorCell);
    if ( Color == NULL ) return 0;
    else return Color->Index;
}


/*****************************************************************************
*
*   Sets colors in a ColorCell.
*
*****************************************************************************/

NLM_EXTERN void DDV_SetColorInCell(DDV_ColorCell *pColorTo, Uint1 *Color)
{
    if(pColorTo == NULL || Color == NULL) return;
    pColorTo->rgb[0] = Color[0];
    pColorTo->rgb[1] = Color[1];
    pColorTo->rgb[2] = Color[2];
}


/*****************************************************************************
*
*   Copy a DDV_ColorCell.
*   Return 1 on success, 0 on failure.
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_CopyColorCell
(DDV_ColorCell *pDestination,  DDV_ColorCell *pSource)
{
    if(pDestination == NULL || pSource == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_CopyColorCell");
        return 0;
    }

    MemCpy(pDestination, pSource, sizeof(DDV_ColorCell));
    return 1;
}

/*****************************************************************************
*
*   Set a color cell using Uint1 values
*   Return 1 on success, 0 on failure
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_SetColorCell
(DDV_ColorCell *pColorCell, Nlm_Uint1 Red,
 Nlm_Uint1 Green, Nlm_Uint1 Blue)
{
    if(pColorCell == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_SetColorCell");
        return 0;
    }

    pColorCell->rgb[0] = Red;
    pColorCell->rgb[1] = Green;
    pColorCell->rgb[2] = Blue;
    return 1;
}


/*****************************************************************************
*
*   Returns a DDV_ColorCell for a given lPosition if it is inside of pColor.
*   Returns NULL otherwise.
*
*****************************************************************************/

NLM_EXTERN DDV_ColorCell * DDV_GetCellByPosition
(DDV_Color *pColor, Int4 lPosition)
{
    if(pColor == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_GetCellByPosition");
        return NULL;
    }
    
    if(SAM_InRange(lPosition, pColor->Range.lFrom,
        pColor->Range.lTo) & SAM_NOLAP) return NULL;
    return &(pColor->pColorCell[lPosition - pColor->Range.lFrom]);
}

/*****************************************************************************
*
*   Sets a DDV_ColorCell for a given lPosition if it is inside of pColor.
*   Returns 1 on success, 0 on failure.
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_SetCellByPosition
(DDV_Color * pColor, Int4 lPosition, DDV_ColorCell *pColorCell)
{
    if(pColor == NULL || pColorCell == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_SetCellByPosition");
        return 0;
    }
    
    if(SAM_InRange(lPosition, pColor->Range.lFrom, pColor->Range.lTo)
        & SAM_NOLAP) return 0;
    
    MemCpy(&pColor->pColorCell[lPosition - pColor->Range.lFrom],
        pColorCell, sizeof(DDV_ColorCell));

    return 1;
}

/*****************************************************************************

Function: DDV_CreateColorCell()

Purpose: Creates a new DDV_ColorCell and initializes it.
          
Parameters: none

Returns: pointer to new DDV_ColorCell or NULL on error

*****************************************************************************/

NLM_EXTERN DDV_ColorCell * DDV_CreateColorCell(void)
{
    DDV_ColorCell *pColorCell;

    pColorCell = MemNew(sizeof(DDV_ColorCell));
    if(pColorCell == NULL) return NULL;

    pColorCell->rgb[0] = 0;
    pColorCell->rgb[1] = 0;
    pColorCell->rgb[2] = 0;

    pColorCell->LowerCase = FALSE;
    pColorCell->TopBox = FALSE;
    pColorCell->RightBox = FALSE;
    pColorCell->BottomBox = FALSE;
    pColorCell->LeftBox = FALSE;
    pColorCell->UseItalic = FALSE;
    pColorCell->UseBold = FALSE;
    pColorCell->UseUnderline = FALSE;

    return pColorCell;
}

/*****************************************************************************
*
*   Creates a DDV_Color that spans the entire object with
*   coordinates lFrom to lTo.
*   The default color is taken from the pallete.
*   Returns NULL on error.
*
*****************************************************************************/

NLM_EXTERN DDV_Color * DDV_CreateDefaultColor
(DDV_ColorGlobal *pColorGlobal, Int4 lFrom, Int4 lTo)
{
    DDV_Color *pColor;
    Int4 i;
    DDV_ColorCell *pColorCell;

    if (pColorGlobal == NULL || lFrom > lTo) {
        ErrPostEx(SEV_ERROR, 0, 0, 
            "Invalid call on DDV_CreateDefaultColor");
        return NULL;
    }

    pColor = MemNew(sizeof(DDV_Color));
    if (pColor == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, 
            "Unable to malloc DDV_Color in DDV_CreateDefaultColor");
        return NULL;
    }

    pColor->Range.lFrom = lFrom; 
    pColor->Range.lTo = lTo;
    pColor->pColorCell = MemNew(sizeof(DDV_ColorCell) * (lTo - lFrom + 1));
    MemSet((void *)pColor->pColorCell, 0, sizeof(DDV_ColorCell) * (lTo - lFrom + 1));
    if( pColor->pColorCell == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, 
            "Unable to malloc DDV_ColorCells in DDV_CreateDefaultColor");
        return NULL;
    }

    pColorCell = DDV_SearchColorCellbyName(pColorGlobal->pvnSpecialColors,
            "Default");
    if(pColorCell == NULL) return NULL;

    DDV_RequestColor(&pColorGlobal->Palette, pColorCell);

    for(i = 0; i <= lTo - lFrom; i++) DDV_CopyColorCell(
        &pColor->pColorCell[i], pColorCell);
   
    return pColor;
}

/*****************************************************************************
*
*   Deletes a DDV_Color.
*   Returns 1 on success, 0 on failure.
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_DeleteColor(DDV_Color *pColor)
{
    if(pColor == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_DeleteColor");
        return 0;
    }

    MemFree(pColor->pColorCell);
    MemFree(pColor);
    return 1;
}

/* todo: delete mediainfo for a given sip */
/* todo: lock count for mediainfo */


/*****************************************************************************
*
*   Creates a new DDV_MediaInfo structure.  Creates a default color structure
*   for the given object specified by sip OR row.
*   Inserts the given pColor and sets fVisible.  Duplicates the sip.
*
*   Returns NULL on error.
*
*****************************************************************************/

static Int4 DDV_ComparePosition(DDV_Range *pRange1, DDV_Range *pRange2)
{
    Int4 test;

    if(pRange1 == NULL || pRange2 == NULL) return 0;
    test = SAM_RangeOverlap(pRange1->lFrom, pRange1->lTo,
        pRange2->lFrom, pRange2->lTo);
    if(test == SAM_NOLAPFRONT) return -1;
    if(test == SAM_NOLAPBACK) return 1;
    return 0;
}


NLM_EXTERN DDV_MediaInfo * DDV_NewMediaInfo
(SeqId *sip,  Int4 lRow, DDV_Color *pColor, Boolean fVisible)
{
    DDV_MediaInfo *pMediaInfo;

    if ((sip == NULL && lRow <= 0) || (sip != NULL && lRow > 0)) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_NewMediaInfo");
        return NULL;
    }


    pMediaInfo = MemNew(sizeof(DDV_MediaInfo));
    if (pMediaInfo == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid Malloc in DDV_NewMediaInfo");
        return NULL;
    }

    if(sip != NULL) pMediaInfo->Id.sip = SeqIdDupList(sip);
    pMediaInfo->Id.lRow = lRow;

    pMediaInfo->pvnColor = NULL;
    if(pColor != NULL) ValNodeAddPointer(&pMediaInfo->pvnColor, 0, pColor);
    pMediaInfo->fVisible = fVisible;

    pMediaInfo->pSearchColor =
      B_NewGlobal((B_CompareFunc) DDV_ComparePosition, 0);
    
    return pMediaInfo;
}

/*****************************************************************************
*
*   Returns a ValNode list, each element of which points to the SeqId of a
*   sequence that has a corresponding DDV_MediaInfo structure in pColorGlobal.
*
*   The Valnode list should be freed by the calling routine.  However, the 
*   SeqIds pointed to by the list should *not* be freed.
*
*   Returns NULL on error.
*
*****************************************************************************/

NLM_EXTERN ValNode * DDV_IdList(DDV_ColorGlobal *pColorGlobal)
{
    ValNode *pvn, *pvnReturn = NULL;
    DDV_MediaInfo *pMediaInfo;

    if(pColorGlobal == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_SipList");
        return NULL;
    }

    for(pvn = pColorGlobal->pvnMediaInfo ; pvn != NULL; pvn = pvn->next) {
        pMediaInfo = (DDV_MediaInfo *)pvn->data.ptrvalue;
        if(pMediaInfo->Id.sip != NULL)
            ValNodeAddPointer(&pvnReturn, 0, pMediaInfo->Id.sip);
    }

    return pvnReturn;
}

/*****************************************************************************
*
*   Creates a default MediaInfo structure if necessary and adds it to
*   pColorGlobal for the given object specified by sip OR lRow.
*   Duplicates the sip.
*   The sequence is visible by default.
*
*   If the coordinates do not exist in the MediaInfo structure (whether the 
*   MediaInfo was freshly created or not) then a DDV_Color is added to the
*   MediaInfo to cover the coordinate requested.
*
*   Coordinates lFrom to lTo.
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_DefaultMediaInfoByLen
(DDV_ColorGlobal * pColorGlobal, SeqId *sip, Int4 lRow, Int4 lFrom, Int4 lTo)
{
    DDV_MediaInfo *pMediaInfo;
    DDV_Color *pColor;
    ValNode *pvn;
    Int4 lOverlap;
    DDV_Range Range;

    if(pColorGlobal == NULL || (sip == NULL && lRow <= 0) ||
        (sip != NULL && lRow > 0) || lFrom > lTo) {
        ErrPostEx(SEV_ERROR, 0, 0, 
            "Invalid call on DDV_DefaultMediaInfoByLen");
        return 0;
    }

    Range.lFrom = lFrom;
    Range.lTo = lTo;

    /* check to see if it already exits */
    pMediaInfo = DDV_GetMediaInfo(pColorGlobal, sip, lRow);
    if(pMediaInfo == NULL) return 0;
    
    for(pvn = pMediaInfo->pvnColor; pvn != NULL; pvn = pvn->next) {
        pColor = (DDV_Color *)pvn->data.ptrvalue;
        lOverlap = SAM_RangeOverlap(Range.lFrom, Range.lTo,
            pColor->Range.lFrom, pColor->Range.lTo);
        if(lOverlap & SAM_NOLAP) continue;
        else if(lOverlap == SAM_TOTALLAP ) return 1;
        else if(lOverlap == SAM_FRONTLAP) {
            lTo = pColor->Range.lFrom - 1;
            break;
        }
        else if(lOverlap == SAM_BACKLAP) {
            lFrom = pColor->Range.lTo + 1;
            break;
        }
        else return 0;
    }
    
    pColor = DDV_CreateDefaultColor(pColorGlobal, lFrom, lTo);
    if(pColor == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, 
            "Unable to create pColor in DDV_DefaultMediaInfoByLen");
        return 0;
    }
    ValNodeAddPointer(&pMediaInfo->pvnColor, 0, pColor);
    B_Insert(pMediaInfo->pSearchColor,
        (void *)&pColor->Range, (void *) pColor);

    return 1;
}

/*****************************************************************************
*
*   Simplified version of DDV_DefaultMediaInfoByLen for sequence coordinates
*   inly.  Retrieves the length and sets the coordinate system to sequence 
*
*   Returns 1 on success, 0 on failure
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_DefaultMediaInfo(DDV_ColorGlobal* pColorGlobal, SeqId *sip)
{
    Int4 lLength;
    Bioseq *bsp;

    if(pColorGlobal == NULL || sip == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_DefaultMediaInfo");
        return 0;
    }

    bsp = BioseqLockById(sip);
    if(bsp == NULL) {
       ErrPostEx (SEV_ERROR, 0, 0, 
           "BioseqLockById failed in DDV_DefaultMediaInfo");
       return 0;
    }

    lLength = BioseqGetLen(bsp) - 1;
    BioseqUnlock (bsp);

    return DDV_DefaultMediaInfoByLen(pColorGlobal, sip, -1, 0, lLength);
}

/*****************************************************************************
*
*   Deletes a DDV_MediaInfo structure.
*
*   Returns 1 on success, 0 on failure
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_DeleteMediaInfo(DDV_MediaInfo *pMediaInfo)
{
    ValNode *pvn;

    if(pMediaInfo == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_DeleteMediaInfo");
        return 0;
    }

    SeqIdFree(pMediaInfo->Id.sip);
    
    for(pvn = pMediaInfo->pvnColor; pvn != NULL; pvn = pvn->next) 
        DDV_DeleteColor((DDV_Color *)pvn->data.ptrvalue);
    ValNodeFree(pMediaInfo->pvnColor);
    B_DeleteGlobal(pMediaInfo->pSearchColor);

    MemFree(pMediaInfo);
    return 1;
}




/*****************************************************************************
*
*   The color function queue.  The queue is used to 
*   register color function by szName (like "Color by Conservation") and to 
*   give each function an lPriority.  The algorithm to determine what function
*   to call is:
*   - if requested, a default coloration is performed.
*   - look for an fOverride algorithm with the correct szName. If found, run it,
*     then stop.
*   - look for all function that match the correct szName OR have fCallMe set.
*     Run each algorithm in priority order.
*   
*   Viewers must take care to set the visible bits beforehand as this
*   influences coloration.
*
*   pfnDDV_ColorFunc takes as arguments pData for user data and a range
*   lFrom to lTo.  If lTo < lFrom, the range should be ignored.
*
*****************************************************************************/


/*****************************************************************************
*
*   Returns a new color queue entry with name szName, priority lPriority.
*   fOverride is explained above.
*
*****************************************************************************/

NLM_EXTERN DDV_ColorQueue * DDV_NewColorQueue
(DDV_ColorFunc pfnColorFunc, Char * szName,
 Int4 lPriority, Boolean fOverride, Uint2 procid)
{
    DDV_ColorQueue *pColorQueue;

    if(pfnColorFunc == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_NewColorFn");
        return NULL;
    }

    pColorQueue = MemNew(sizeof(DDV_ColorQueue));
    if(pColorQueue == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Unable to Malloc in DDV_NewColorFn");
        return NULL;
    }

    pColorQueue->fOverride = fOverride;
    pColorQueue->lPriority = lPriority;
    pColorQueue->pfnColorFunc = pfnColorFunc;
    pColorQueue->szName = StringSave(szName);
    pColorQueue->next = NULL;
    pColorQueue->procid = procid;

    return pColorQueue;
}

/*****************************************************************************
*
*   Deletes a DDV_ColorQueue.  
*   Returns 1 on success, 0 on failure
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_DeleteColorQueue(DDV_ColorQueue *pColorQueue)
{
    if(pColorQueue == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_DeleteColorFn");
        return 0;
    }
    MemFree(pColorQueue->szName);
    MemFree(pColorQueue);
    return 1;
}


/*****************************************************************************

Function: DDV_SetPaletteColor()

Purpose: Sets a single color in the palette.  Trys to read the color out of 
         the configuration file.  If it isn't there, puts it in the file using
         the supplied default
  
Parameters: pColorGlobal, the color global object
            ParamFile, the name of the app property parameter file
            ColorName, the name of the color
            red, grn, blue, the default color

*****************************************************************************/

NLM_EXTERN void DDV_SetPaletteColor
(DDV_ColorGlobal *pColorGlobal, Char *ColorName, 
 Char *ParamFile, Uint1 red, Uint1 grn, Uint1 blu)
{
    long Value;
    DDV_ColorEntry *pColorEntry = NULL;

    if(pColorGlobal == NULL || ColorName == NULL || ParamFile == NULL) return;

    pColorEntry = DDV_SearchColorbyName(pColorGlobal->pvnSpecialColors, 
        ColorName);
    if(pColorEntry == NULL) {
        pColorEntry = DDV_CreateColorEntry(ColorName, red, grn, blu);
        DDV_RequestColorbyName(&pColorGlobal->pvnSpecialColors, pColorEntry); 
    }

    pColorEntry->ColorCell.rgb[0] = red;
    pColorEntry->ColorCell.rgb[1] = grn;
    pColorEntry->ColorCell.rgb[2] = blu;

    SetAppParamLong(ParamFile, "COLORS", ColorName, red<<16 | grn<<8 | blu);

}


static void DDV_GetPaletteColor(DDV_ColorGlobal *pColorGlobal, Char *ColorName, 
                         Char *ParamFile, Uint1 red, Uint1 grn, Uint1 blu)
{
    long Value;

    if(pColorGlobal == NULL || ColorName == NULL || ParamFile == NULL) return;

    Value = GetAppParamLong(ParamFile, "COLORS", ColorName, -1);
    if(Value == -1) DDV_SetPaletteColor(pColorGlobal, ColorName, ParamFile,
        red, grn, blu);
    else DDV_SetPaletteColor(pColorGlobal, ColorName, ParamFile,
        DDVRED(Value), DDVGRN(Value), DDVBLU(Value));
}

/*****************************************************************************
*
*   Sets up default secondary structure colors in 
*   pColorGlobal->pvnSpecialColors palette.
*   Returns 0 on failure.
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_DefaultSSColor
(DDV_ColorGlobal * pColorGlobal, Char *ParamFile)
{

    if(pColorGlobal == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid Call on DDV_DefaultSSColor");
        return 0;
    }

    DDV_SetPaletteColor(pColorGlobal, "Highlight", ParamFile, 255, 255, 0);
    DDV_SetPaletteColor(pColorGlobal, "Background", ParamFile, 0, 0, 0);
    DDV_SetPaletteColor(pColorGlobal, "Helix", ParamFile, 0, 255, 0);
    DDV_SetPaletteColor(pColorGlobal, "Strand", ParamFile, 255, 165, 0);
    DDV_SetPaletteColor(pColorGlobal, "Turn", ParamFile, 255, 69, 0);
    DDV_SetPaletteColor(pColorGlobal, "Coil", ParamFile, 0, 255, 255);
    return 1;
}


NLM_EXTERN void DDV_LoadSSColor(DDV_ColorGlobal * pColorGlobal, Char *ParamFile)
{
    if(pColorGlobal == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid Call on DDV_DefaultSSColor");
        return;
    }
    
    DDV_GetPaletteColor(pColorGlobal, "Highlight", ParamFile, 255, 255, 0);
    DDV_GetPaletteColor(pColorGlobal, "Background", ParamFile, 0, 0, 0);
    DDV_GetPaletteColor(pColorGlobal, "Helix", ParamFile, 0, 255, 0);
    DDV_GetPaletteColor(pColorGlobal, "Strand", ParamFile, 255, 165, 0);
    DDV_GetPaletteColor(pColorGlobal, "Turn", ParamFile, 255, 69, 0);
    DDV_GetPaletteColor(pColorGlobal, "Coil", ParamFile, 0, 255, 255);
}

static Int4 DDV_CompareRow(void *Key1, void *Key2)
{
    if(Key1 == NULL || Key2 == NULL) return 1;
    else return *(int *)Key1 - *(int *)Key2;
}

/*****************************************************************************
*
*   Returns a new DDV_ColorGlobal structure.
*   Returns NULL on failure.
*
*   Parameters: fDefaultColor, sets everything to the default color before
*                              executing color functions
*               pObject, the object this structure is coloring. can be NULL.
*
*****************************************************************************/

NLM_EXTERN DDV_ColorGlobal * DDV_CreateColorGlobal(Boolean fDefaultColor,
                                                   void *pObject)
{
    DDV_ColorGlobal *pColorGlobal;
    DDV_ColorEntry *pColorEntry;

    pColorGlobal = MemNew(sizeof(DDV_ColorGlobal));
    if(pColorGlobal == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid Malloc in DDV_CreateColorGlobal");
        return NULL;
    }
    
    pColorGlobal->pObject = pObject;
    pColorGlobal->fColorByMaster = FALSE;
    pColorGlobal->fDefaultColor = fDefaultColor;
    pColorGlobal->MasterSeqId = NULL;
    pColorGlobal->pvnColorQueue = NULL;
    pColorGlobal->pvnAllColorFns = NULL;
    pColorGlobal->pvnMediaInfo = NULL;
    pColorGlobal->Palette = NULL;
    pColorGlobal->pvnSpecialColors = NULL;
    pColorGlobal->pvnUser2Row = NULL;
    pColorGlobal->pSearchMI =
      B_NewGlobal((B_CompareFunc) SAM_OrderSeqID, 0);
    pColorGlobal->pSearchMIRow =
      B_NewGlobal((B_CompareFunc) DDV_CompareRow, 0);


    pColorEntry = DDV_CreateColorEntry("Highlight", 255, 255, 0);
    ValNodeAddPointer(&pColorGlobal->pvnSpecialColors, 0, pColorEntry);
    pColorEntry = DDV_CreateColorEntry("Default", 0, 0, 0);
    ValNodeAddPointer(&pColorGlobal->pvnSpecialColors, 0, pColorEntry);

	/*Feature Colors Table init*/
	pColorGlobal->SpeClr.pFeatColorTable=DDV_InitFeatureColors();
	
    return pColorGlobal;
}

/*****************************************************************************
*
*   Deletes a  DDV_ColorGlobal structure.
*   Returns 1 on success, 0 on failure
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_DeleteColorGlobal(DDV_ColorGlobal *pColorGlobal)
{
    ValNode *pvn;

    if(pColorGlobal == NULL) return 1;

    for(pvn = pColorGlobal->pvnColorQueue; pvn != NULL; pvn = pvn->next) 
        DDV_DeleteColorQueue((DDV_ColorQueue *)pvn->data.ptrvalue);
    ValNodeFree(pColorGlobal->pvnColorQueue);

    for(pvn = pColorGlobal->pvnAllColorFns; pvn != NULL; pvn = pvn->next) 
        DDV_DeleteColorQueue((DDV_ColorQueue *)pvn->data.ptrvalue);
    ValNodeFree(pColorGlobal->pvnAllColorFns);

    for(pvn = pColorGlobal->pvnMediaInfo ; pvn != NULL; pvn = pvn->next) 
        DDV_DeleteMediaInfo((DDV_MediaInfo *)pvn->data.ptrvalue);
    ValNodeFree(pColorGlobal->pvnMediaInfo);

    DDV_FreePalette(&pColorGlobal->Palette);
    DDV_FreePalette(&pColorGlobal->pvnSpecialColors);
    
    B_DeleteGlobal(pColorGlobal->pSearchMI);
    B_DeleteGlobal(pColorGlobal->pSearchMIRow);
	if (pColorGlobal->SpeClr.pFeatColorTable) 
		MemFree(pColorGlobal->SpeClr.pFeatColorTable);
    MemFree(pColorGlobal);

    return 1;
}

/*****************************************************************************
*
*   Completely deallocate all of the DDV_Color structures in pColorGlobal
*   This is intended for simple minded garbage collection.
*   Returns 1 on success, 0 on failure
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_ClearColor(DDV_ColorGlobal *pColorGlobal)
{
    ValNode *pvn, *pvnColor;
    DDV_MediaInfo *pMediaInfo;

    if(pColorGlobal == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_ClearColor");
        return 0;
    }

    for(pvn = pColorGlobal->pvnMediaInfo ; pvn != NULL; pvn = pvn->next) {
        pMediaInfo = (DDV_MediaInfo *)pvn->data.ptrvalue;
        for(pvnColor = pMediaInfo->pvnColor; pvnColor != NULL;
        pvnColor = pvnColor->next) {
            if(!DDV_DeleteColor((DDV_Color *)pvnColor->data.ptrvalue))
                return 0;
        }
        ValNodeFree(pMediaInfo->pvnColor);
        pMediaInfo->pvnColor = NULL;
        B_DeleteGlobal(pMediaInfo->pSearchColor);
        pMediaInfo->pSearchColor =
            B_NewGlobal((B_CompareFunc) DDV_ComparePosition, 0);
    }
    return 1;
}


/*****************************************************************************
*
*   Delete all information for the given sip AND lRow.
*   Returns 1 on success, 0 on failure
*
*****************************************************************************/

static Boolean DDV_CompareAllIds(SeqId *sip, Int4 lRow,
                                 DDV_Id *pId)
{    
    Boolean retval = FALSE;
    if(sip != NULL) retval = SeqIdIn(sip, pId->sip);
    else if(lRow == pId->lRow) retval = TRUE;
    return retval;
}


NLM_EXTERN Int4 DDV_RemoveMediaInfo
(DDV_ColorGlobal *pColorGlobal, SeqId *sip, Int4 lRow)
{
    ValNode *pvn, *pvnPrevious = NULL;
    DDV_MediaInfo *pMediaInfo;

    if(pColorGlobal == NULL || (sip == NULL && lRow <= 0) ||
       (sip != NULL && lRow > 0)) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_RemoveMediaInfo");
        return 0;
    }
    /* todo: delete from b search */
    for(pvn = pColorGlobal->pvnMediaInfo ; pvn != NULL; pvn = pvn->next) {
        pMediaInfo = (DDV_MediaInfo *)pvn->data.ptrvalue;

        if(DDV_CompareAllIds(sip, lRow, &pMediaInfo->Id)) {
            DDV_DeleteMediaInfo(pMediaInfo);
            if(pvnPrevious != NULL) {  /* splice out node */
                pvnPrevious->next = pvn->next;
            }
            else {  /* if first node */
                pColorGlobal->pvnMediaInfo = pvn->next;
            }
            MemFree(pvn);
            break;
        }
        pvnPrevious = pvn;
    }

    return 1;
}

/*****************************************************************************
*
*   Add MediaInfo to DDV_ColorGlobal
*   Returns 1 on success, 0 on failure
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_AddMediaInfo
(DDV_ColorGlobal *pColorGlobal, DDV_MediaInfo *pMediaInfo)
{
    if(pColorGlobal == NULL || pMediaInfo == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_AddMediaInfo");
        return 0;
    }

    ValNodeAddPointer(&pColorGlobal->pvnMediaInfo, 0, pMediaInfo);
    if(pMediaInfo->Id.sip != NULL) B_Insert(pColorGlobal->pSearchMI,
        (void *)pMediaInfo->Id.sip, (void *) pMediaInfo);
    else B_Insert(pColorGlobal->pSearchMIRow,
        (void *)&pMediaInfo->Id.lRow, (void *) pMediaInfo);
    return 1;
}

/*****************************************************************************
*
*   Given a sip OR lRow, return the associated DDV_MediaInfo.
*
*****************************************************************************/

NLM_EXTERN DDV_MediaInfo * DDV_GetMediaInfo
(DDV_ColorGlobal *pColorGlobal, SeqId * sip, Int4 lRow)
{
    DDV_MediaInfo *pMediaInfo = NULL;

    if(pColorGlobal == NULL || (sip == NULL && lRow <= 0) ||
        (sip != NULL && lRow > 0) ) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_GetMediaInfo");
        return NULL;
    }
    
    if(sip != NULL)
        pMediaInfo = (DDV_MediaInfo *)B_GetBag(pColorGlobal->pSearchMI,
            (void *)sip);
    else
        pMediaInfo = (DDV_MediaInfo *)B_GetBag(pColorGlobal->pSearchMIRow,
            (void *)&lRow);

    if(pMediaInfo == NULL) {
        pMediaInfo = DDV_NewMediaInfo(sip, lRow, NULL, TRUE);
        if(pMediaInfo == NULL) {
            ErrPostEx(SEV_ERROR, 0, 0, "No MediaInfo in DDV_GetMediaInfo");
            return NULL;
        }
        DDV_AddMediaInfo(pColorGlobal, pMediaInfo);
    }
    
    return pMediaInfo;
}

/*****************************************************************************
*
*   Given an sip OR lRow, return the visible state.
*
*****************************************************************************/

NLM_EXTERN Boolean DDV_IsVisible
(DDV_ColorGlobal *pColorGlobal, SeqId *sip, Int4 lRow)
{
    DDV_MediaInfo *pMediaInfo;

    if(pColorGlobal == NULL || (sip == NULL && lRow <= 0) ||
        (sip != NULL && lRow > 0 )) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_IsVisible");
        return TRUE;
    }

    pMediaInfo = DDV_GetMediaInfo(pColorGlobal, sip, lRow);
    if(pMediaInfo == NULL) return FALSE;

    return pMediaInfo->fVisible;
}

/*****************************************************************************
*
*   Set the visible state (fVisible) of a sip OR lRow.
*   Returns 1 on success, 0 on failure.
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_SetVisible
(DDV_ColorGlobal *pColorGlobal, SeqId *sip, Int4 lRow, Boolean fVisible)
{
    DDV_MediaInfo *pMediaInfo;

    if(pColorGlobal == NULL || (sip == NULL && lRow <= 0) ||
        (sip != NULL && lRow > 0)) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_SetVisible");
        return 0;
    }

    pMediaInfo = DDV_GetMediaInfo(pColorGlobal, sip, lRow);
    if(pMediaInfo == NULL) return 0;

    pMediaInfo->fVisible = fVisible;

    return 1;
}

/*****************************************************************************
*
*   Retrieve a color for a sip OR lRow at lPosition in.
*   Returns a DDV_ColorCell containing the color.  NULL on failure.
*
*   The DDV_ColorCell returned is read only.
*
*****************************************************************************/

NLM_EXTERN DDV_ColorCell * DDV_GetColor
(DDV_ColorGlobal *pColorGlobal, SeqId *sip, Int4 lRow, Int4 lPosition)
{
    DDV_MediaInfo *pMediaInfo;
    ValNode *pvn;
    DDV_Color *pColor;
    DDV_ColorCell *pColorCell = NULL;
    DDV_Range TestRange;
    Int4 retval = 0;


    if(pColorGlobal == NULL || (sip == NULL && lRow <= 0) ||
        (sip != NULL && lRow > 0)) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_GetColor");
        return NULL;
    }

    pMediaInfo = DDV_GetMediaInfo(pColorGlobal, sip, lRow);
    if(pMediaInfo == NULL) return NULL;

    TestRange.lFrom = lPosition;
    TestRange.lTo = lPosition;
    pColor = (DDV_Color *)B_GetBag(pMediaInfo->pSearchColor,
            (void *)&TestRange);
    if(pColor != NULL) pColorCell =
        DDV_GetCellByPosition(pColor, lPosition);

    if(pColorCell == NULL) {
        DDV_DefaultMediaInfoByLen(pColorGlobal, sip, lRow, lPosition,
            lPosition + DDV_BUFSIZE);
        pColorCell = DDV_GetColor(pColorGlobal, sip, lRow, lPosition);
    }
    return pColorCell;
    
}

/*****************************************************************************
*
*   Set a color for in a DDV_MediaInfo at lPosition.  The color set is
*   pColorCell.
*   Returns 1 on success, 0 on failure.
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_SetColorInMediaInfo
(DDV_ColorGlobal *pColorGlobal,
 DDV_MediaInfo *pMediaInfo, Int4 lPosition, DDV_ColorCell *pColorCell)
{
    ValNode *pvn;
    DDV_Color *pColor;
    Int4 retval = 0;
    DDV_Range TestRange;

    if(pColorGlobal == NULL || pColorCell == NULL || pMediaInfo == NULL ) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_SetColorInMediaInfo");
        return 0;
    }

    for(pvn = pMediaInfo->pvnColor; pvn != NULL; pvn = pvn->next) {
        pColor = (DDV_Color *)pvn->data.ptrvalue;
        retval = DDV_SetCellByPosition(pColor, lPosition, pColorCell);
        if(retval) break;
    }
    TestRange.lFrom = lPosition;
    TestRange.lTo = lPosition;
    pColor = (DDV_Color *)B_GetBag(pMediaInfo->pSearchColor,
            (void *)&TestRange);
    if(pColor != NULL) retval = 
        DDV_SetCellByPosition(pColor, lPosition, pColorCell);

    if(retval) DDV_RequestColor(&pColorGlobal->Palette, pColorCell);
    else {
        DDV_DefaultMediaInfoByLen(pColorGlobal, pMediaInfo->Id.sip,
            pMediaInfo->Id.lRow, lPosition, lPosition + DDV_BUFSIZE);
        retval = DDV_SetColorInMediaInfo(pColorGlobal, pMediaInfo, lPosition,
            pColorCell);
        if(retval) DDV_RequestColor(&pColorGlobal->Palette, pColorCell);
    }

    return retval;
}

/*****************************************************************************
*
*   Set a color for a sip OR lRow at lPosition.  The color set is
*   pColorCell.  Makes a copy of pColorCell.
*   Returns 1 on success, 0 on failure.
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_SetColor
(DDV_ColorGlobal *pColorGlobal, SeqId *sip, Int4 lRow,
 Int4 lPosition, DDV_ColorCell *pColorCell)
{
    DDV_MediaInfo *pMediaInfo;

    if(pColorGlobal == NULL || pColorCell == NULL ||
       (sip == NULL && lRow <= 0) ||
       (sip != NULL && lRow > 0)) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_SetColor");
        return 0;
    }

    pMediaInfo = DDV_GetMediaInfo(pColorGlobal, sip, lRow);
    if(pMediaInfo == NULL) return 0;

    return
      DDV_SetColorInMediaInfo(pColorGlobal, pMediaInfo, lPosition, pColorCell);

}

/*****************************************************************************
*
*   Set a color for a sip OR lRow at lPosition.
*   Uses Uint1's for color input.
*   Returns 1 on success, 0 on failure.
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_SetColorbyColor
(DDV_ColorGlobal *pColorGlobal, SeqId *sip, Int4 lRow, Int4 lPosition,
 Uint1 red, Uint1 green, Uint1 blue)
{
    DDV_ColorCell ColorCell;

    if(pColorGlobal == NULL || (sip == NULL && lRow <= 0) ||
        (sip != NULL && lRow > 0)) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_SetColorbyColor");
        return 0;
    }


    ColorCell.rgb[0] = red;
    ColorCell.rgb[1] = green;
    ColorCell.rgb[2] = blue;

    return DDV_SetColor(pColorGlobal, sip, lRow, lPosition, &ColorCell);
}


/*****************************************************************************
*
*   Calls the color functions.  pData is for user data.
*
*   Returns 1 on success, 0 on failure.
*
*   Note that this only updates the colors.  It doesn't tell the applications
*   to paint the colors.  This is done through OM_MSG_SETCOLOR with a new
*   regiontype TBD.
*
*   fColorByMaster in DDV_ColorGlobal (i.e. color all sequence like the master
*   sequence) is NOT handled by this function.  This
*   is because this code has, in general, no idea if it is using alignment
*   coordinates or alignment coordinates.  To color by master, the user must
*   register a specific function to do this.
*
*   lFrom and lTo are the range coloring is done over.
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_ColorExecute
(DDV_ColorGlobal *pColorGlobal, void * pData, DDV_Range *pRange, char *szName)
{
    DDV_ColorQueue  *pcq, *pcqPrev;
    ValNode *pvn, *pvnPrev, *pvnPrevPrev, *pvnTemp;
    Int4 lCount = 0, i;

    if(pColorGlobal == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_ColorExecute");
        return 0;
    }

    DDV_FreePalette(&pColorGlobal->Palette);

    /* color by default? */
    if(pColorGlobal->fDefaultColor) DDV_DefaultColor(pColorGlobal,
        NULL, NULL);

    /* look for an override */
    for(pvn = pColorGlobal->pvnColorQueue; pvn != NULL; pvn = pvn->next) {
        pcq = (DDV_ColorQueue *)pvn->data.ptrvalue;
        if(pcq->fOverride) {  /* this function is king */
            (*pcq->pfnColorFunc)(pColorGlobal, pData, pRange);
        }
        lCount++;
    }
    
    /* sort the color functions by priority */

    for(i = 0; i < lCount - 1; i++) {
        for(pvnPrevPrev = NULL,
            pvnPrev = pColorGlobal->pvnColorQueue, 
            pvn = pColorGlobal->pvnColorQueue->next;
            pvn != NULL;
            pvnPrevPrev = pvnPrev,
            pvnPrev = pvn,
            pvn = pvn->next) {

            pcq = (DDV_ColorQueue *)pvn->data.ptrvalue;
            pcqPrev = (DDV_ColorQueue *)pvnPrev->data.ptrvalue;

            if(pcq->lPriority < pcqPrev->lPriority) {
                pvnTemp = pvn->next;
                pvn->next = pvnPrev;
                pvnPrev->next = pvnTemp;
                if(pvnPrevPrev != NULL) pvnPrevPrev->next = pvn;
                else pColorGlobal->pvnColorQueue = pvn;
                pvnTemp = pvnPrev;
                pvnPrev = pvn;
                pvn = pvnTemp;
            }
        }
    }

    /* call the color functions in priority order */
    for(pvn = pColorGlobal->pvnColorQueue; pvn != NULL; pvn = pvn->next) {
        pcq = (DDV_ColorQueue *)pvn->data.ptrvalue;
        if(StrCmp(szName, pcq->szName) == 0) 
            (*pcq->pfnColorFunc)(pColorGlobal, pData, pRange);
    }

    return 1;
}

/*****************************************************************************
*
*   Callback for coloring all ColorCells by the default color.
*
*
*****************************************************************************/

NLM_EXTERN void DDV_DefaultColor
(DDV_ColorGlobal *pColorGlobal, void *pData, DDV_Range *pRange)
{
    ValNode *pvnMediaInfo, *pvnColor;
    DDV_MediaInfo *pMediaInfo;
    DDV_Color *pColor;
    DDV_ColorCell *pccDefault;
    Int4 i;

    if(pColorGlobal == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_DefaultColor");
        return;
    }

    pccDefault = DDV_SearchColorCellbyName(pColorGlobal->pvnSpecialColors,
        "Default");

    if(pColorGlobal == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "DDV_DefaultColor: no default color");
        return;
    }

    for(pvnMediaInfo = pColorGlobal->pvnMediaInfo; pvnMediaInfo != NULL;
        pvnMediaInfo = pvnMediaInfo->next) {
        pMediaInfo = (DDV_MediaInfo *)pvnMediaInfo->data.ptrvalue;
        for(pvnColor = pMediaInfo->pvnColor; pvnColor != NULL; 
            pvnColor = pvnColor->next) {
            pColor = (DDV_Color *)pvnColor->data.ptrvalue;
                for(i = 0; i <= pColor->Range.lTo - pColor->Range.lFrom; i++)
                    DDV_CopyColorCell(&pColor->pColorCell[i], pccDefault);
        }
    }
        /* put the default color on palette */
    DDV_RequestColor(&pColorGlobal->Palette, pccDefault);
    return;
}
/*****************************************************************************
*
*   Clears all the colors (only the color) to the default
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_Clear2Default(DDV_ColorGlobal *pColorGlobal)
{
    ValNode *pvn, *pvnColor;
    DDV_MediaInfo *pMediaInfo;
    DDV_ColorCell *pColorCell;
    DDV_Color *pColor;
    Int4 i;

    if(pColorGlobal == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_Clear2Default");
        return 0;
    }
    
    pColorCell = DDV_SearchColorCellbyName(pColorGlobal->pvnSpecialColors,
                "Default");
    if(pColorCell == NULL) return 0;

    for(pvn = pColorGlobal->pvnMediaInfo ; pvn != NULL; pvn = pvn->next) {
        pMediaInfo = (DDV_MediaInfo *)pvn->data.ptrvalue;
        for(pvnColor = pMediaInfo->pvnColor; pvnColor != NULL;
        pvnColor = pvnColor->next) {
            pColor = (DDV_Color *)pvnColor->data.ptrvalue;
            if (pColor == NULL) continue;
                    
            for(i = 0; i <= pColor->Range.lTo - pColor->Range.lFrom; i++) {
                pColor->pColorCell[i].rgb[0] = pColorCell->rgb[0];
                pColor->pColorCell[i].rgb[1] = pColorCell->rgb[1];
                pColor->pColorCell[i].rgb[2] = pColorCell->rgb[2];
            }            
        }
    }
    return 1;
}

/*****************************************************************************
*
*   Adds a DDV_ColorQueue to pColorGlobal
*   Returns 1 on success, 0 on failure.
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_AddColorFunc
(DDV_ColorGlobal *pColorGlobal, DDV_ColorQueue *pColorQueue)
{
    if(pColorGlobal == NULL || pColorQueue == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_AddColorQueue");
        return 0;
    }

    ValNodeAddPointer(&pColorGlobal->pvnColorQueue, 0, pColorQueue);
    return 1;
}

/*****************************************************************************
*
*   Deletes the given DDV_ColorFunc out of the ColorQueue
*   Returns number deleted on success, 0 on failure.
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_DeleteColorFunc
(DDV_ColorGlobal *pColorGlobal, DDV_ColorFunc  pfnColorFunc)
{
    ValNode *pvn, *pvnPrevious = NULL;
    DDV_ColorQueue *pColorQueue;
    Int4 retval = 0;

    if(pColorGlobal == NULL || pfnColorFunc == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on DDV_DeleteColorQueue");
        return 0;
    }

    for(pvn = pColorGlobal->pvnColorQueue; pvn != NULL; pvn = pvn->next) {
        pColorQueue = (DDV_ColorQueue *)pvn->data.ptrvalue;
        if(pColorQueue->pfnColorFunc == pfnColorFunc) {
            DDV_DeleteColorQueue(pColorQueue);
            retval++;
            if(pvnPrevious != NULL) {  /* splice out node */
                pvnPrevious->next = pvn->next;
            }
            else {  /* if first node */
                pColorGlobal->pvnColorQueue = pvn->next;
            }
            MemFree(pvn);
            break;
        }
        pvnPrevious = pvn;
    }

    return retval;
}

/*****************************************************************************

Function: DDV_AddColorGlobal()

Purpose: Adds a DDV_ColorGlobal to an entity as userdata.
  
Parameters: pColorGlobal, the OMProcControlPtr.
            pObject, pointer to the entity.

Returns: 1 on success, 0 on failure

Notes: - will only add to objects registered with the object manager
       - adds a free function to the userdata
       - the DDV_ColorGlobal userdata is identified by proc type
         OMPROC_COLORMGR

*****************************************************************************/

static void * LIBCALLBACK DDV_FreeUserData(void *pColorGlobal)
{
    if(pColorGlobal == NULL) return NULL;
    DDV_DeleteColorGlobal((DDV_ColorGlobal *)pColorGlobal);
    return NULL;
}


NLM_EXTERN Int4 DDV_AddColorGlobal
(DDV_ColorGlobal *pColorGlobal, void * pObject)
{
    OMUserData *omudp;
    Uint2 entityID;

    if(pColorGlobal == NULL || pObject == NULL) return 0;
    entityID = ObjMgrGetEntityIDForPointer(pObject);
    if(entityID == 0) return 0;
    omudp = ObjMgrAddUserData (entityID, 0, OMPROC_COLORMGR, 0);
    if(omudp == NULL) return 0;
    omudp->userdata.ptrvalue = pColorGlobal;
    omudp->freefunc = DDV_FreeUserData;

    return 1;
}


/*****************************************************************************

Function: DDV_GetColorGlobal()

Purpose: Returns the DDV_ColorGlobal attached to an entity as userdata.
  
Parameters: pObject, pointer to the entity.

Returns: DDV_ColorGlobal * on success, NULL on failure

*****************************************************************************/

NLM_EXTERN DDV_ColorGlobal * DDV_GetColorGlobal(void * pObject)
{
    OMUserData *omudp;
    Uint2 entityID;

    if(pObject == NULL) return NULL;
    entityID = ObjMgrGetEntityIDForPointer(pObject);
    if(entityID == 0) return NULL;
    omudp = ObjMgrGetUserData(entityID, 0, OMPROC_COLORMGR, 0);

    if(omudp == NULL) return NULL;
    return (DDV_ColorGlobal *)omudp->userdata.ptrvalue;
}


/*****************************************************************************

Function: DDV_GetColorGlobalEx()

Purpose: Returns the DDV_ColorGlobal attached to an entity as userdata. If 
         none is attached, create one and attach it.
  
Parameters: pObject, pointer to the entity.

Returns: DDV_ColorGlobal * on success, NULL on failure

*****************************************************************************/

NLM_EXTERN DDV_ColorGlobal * DDV_GetColorGlobalEx(void * pObject)
{
    DDV_ColorGlobal *pColorGlobal;

    pColorGlobal = DDV_GetColorGlobal(pObject);
    if (pColorGlobal != NULL) return pColorGlobal;

    pColorGlobal = DDV_CreateColorGlobal(FALSE, pObject);
    if(pColorGlobal == NULL) return NULL;

    if(DDV_AddColorGlobal(pColorGlobal, pObject)) return pColorGlobal;

    MemFree(pColorGlobal);
    return NULL;
}
