/*   $Id: ddvcolor.h,v 1.18 2001/05/08 19:04:29 lewisg Exp $
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
* File Name:  $Id: ddvcolor.h,v 1.18 2001/05/08 19:04:29 lewisg Exp $
*
* Author:  Lewis Geer
*
* Version Creation Date:   6/2/99
*
* $Revision: 1.18 $
*
* File Description: Shared color information for viewers
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: ddvcolor.h,v $
* Revision 1.18  2001/05/08 19:04:29  lewisg
* bitfield to unsigned
*
* Revision 1.17  2000/02/15 22:40:57  lewisg
* add ability to launch udv so that it colors by row, fixes to colormgr, track rows from viewmgr, fix visual c projects
*
* Revision 1.16  2000/02/10 15:51:58  lewisg
* cn3d responds and send correct update messages.  many coloring bug fixes
*
* Revision 1.15  2000/02/10 14:15:18  durand
* add a function to build Feature colors table
*
* Revision 1.14  2000/01/28 19:25:39  lewisg
* columnwise binary search on color data
*
* Revision 1.13  1999/12/29 22:55:02  lewisg
* get rid of seqalign id
*
* Revision 1.12  1999/12/13 23:20:43  lewisg
* bug fixes: duplicate color structures eliminated, clear color doesn't clear case
*
* Revision 1.11  1999/12/10 20:59:22  durand
* add support for italic, bold and underlined letters
*
* Revision 1.10  1999/11/24 21:24:24  vakatov
* Fixed for the C++ and/or MSVC DLL compilation
*
* Revision 1.9  1999/11/24 15:23:19  lewisg
* added color selection dialogs for SS
*
* Revision 1.8  1999/11/22 17:29:54  lewisg
* add back color selection dialogs, fix viewer3d half-selection bug
*
* Revision 1.7  1999/11/01 22:10:26  lewisg
* add ability to call color functions by type, and add this to cn3d
*
* Revision 1.6  1999/10/25 20:28:48  lewisg
* move ddvcolor to libncbiobj
*
* Revision 6.6  1999/10/15 20:56:40  lewisg
* append DDV_ColorGlobal as userdata.  free memory when cn3d terminates.
*
* Revision 6.5  1999/10/08 23:20:36  lewisg
* add case and box attributes to DDV_ColorCell
*
* Revision 6.4  1999/10/05 23:18:26  lewisg
* add ddv and udv to cn3d with memory management
*
* Revision 6.3  1999/09/21 18:09:13  lewisg
* binary search added to color manager, various bug fixes, etc.
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
* Revision 6.2  1999/07/19 19:12:21  lewisg
* fix bug adding new pColor in old pMediaInfo.  Also added ability to detect overlaps when allocating new pColor
*
* Revision 6.1  1999/07/16 18:46:46  lewisg
* moved ddvcolor from api to tools
*
* Revision 1.4  1999/07/13 23:24:48  lewisg
* added DDV_SipList
*
* Revision 1.3  1999/07/13 17:02:00  lewisg
* fix forward declaration error in sgi compiler
*
* Revision 1.2  1999/07/13 14:38:39  lewisg
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
* Revision 1.1  1999/06/11 23:37:10  lewisg
* color management functions
*
*
*
* ==========================================================================
*/


#ifndef DDVCOLOR_H
#define DDVCOLOR_H

#include <ncbilcl.h>
#include <ncbistd.h>
#include <objall.h>
#include <binary.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif


/* the default size of the buffer to allocate. */
#define DDV_BUFSIZE 1024
    
typedef struct _DDV_Id {
    Boolean fAlign;  /* pointing to alignment or sequence? */
    SeqId *sip;  /* for the sequence. duplicate */
    Int4 lRow; /* for the alignment */
} DDV_Id;
        
/* a range on a sequence or alignment row */
typedef struct _DDV_Range {
    Int4 lFrom;
    Int4 lTo;
} DDV_Range;

typedef struct _DDV_ColorCell {
  Uint1 rgb[3];  /* standard rgb color cell */
  unsigned LowerCase:1, TopBox:1, RightBox:1, BottomBox:1, LeftBox:1, UseBold:1, UseItalic:1, UseUnderline:1;
} DDV_ColorCell;

typedef struct _DDV_Color {
    DDV_Range Range;  /* the coordinates of the entity covered by the colorcell */
    DDV_ColorCell *pColorCell;  /* the array of colors */
} DDV_Color;

    /* standard color entry */
typedef struct _DDV_ColorEntry {
    DDV_ColorCell ColorCell;  /* the color cell */
    Char * Name;  /* the name of the color */
    Char * Paints;  /* what the color paints */
    Int4 Index; /* an index assigned to the colors after they are allocated */
} DDV_ColorEntry;

typedef struct _DDV_MediaInfo {
DDV_Id Id;  /* the id for either sequence or alignment row */ 
ValNode *pvnColor;  /* ValNode list of DDV_Color associated with the object */
Boolean fVisible;  /* is the sequence visible? */
B_Global *pSearchColor; /* used to search  pvnColor structure by position */
} DDV_MediaInfo;

/*bag to put various "standard color tables"*/
typedef struct _DDV_SpecialColor {
	DDV_ColorCell * pFeatColorTable;/*Table of color for features; max size=FEATDEF_MAX*/
} DDV_SpecialColor;

/*****************************************************************************
*
*   This is the global structure that contains all of the color info
*
*****************************************************************************/

typedef struct _DDV_ColorGlobal {
ValNode *pvnMediaInfo;   /* ValNode list of DDV_MediaInfo */
SeqId *MasterSeqId;      /* this refers to the master sequence */
Boolean fColorByMaster;  /* use only the master sequence to color */
Boolean fDefaultColor;   /* fDefaultColor means run the default coloration */
ValNode *pvnColorQueue;  /* Valnode list of DDV_ColorQueue */
ValNode *pvnAllColorFns; /* Valnode list of all color functions that can
                            be exposed externally to the viewers.  to be used
                            by SAM */
ValNode *Palette;         /* ValNode list of DDV_ColorEntries */
ValNode *pvnSpecialColors;  /* ValNode list of DDV_ColorEntries.  used for
                            highlight, secondary structure, etc. */
B_Global *pSearchMI;      /* used to search the MediaInfo structure for sips */
B_Global *pSearchMIRow;      /* used to search the MediaInfo structure for sips */
DDV_SpecialColor SpeClr;  /*"standard color tables"*/
ValNode *pvnUser2Row;     /* associates user keys with rows */
void *pObject;            /* pointer to the object we are coloring */
} DDV_ColorGlobal;

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
*   DDV_ColorFunc takes as arguments pData for user data and a range
*   pRange.  If To < From, the range should be ignored.
*
*****************************************************************************/

/* standard lPriority's */
#define DDV_PRIHI 20
#define DDV_PRIMED 0
#define DDV_PRILO -20

typedef void (*DDV_ColorFunc)(DDV_ColorGlobal *pColorGlobal, void *pData,
                                DDV_Range *pRange);

typedef struct _DDV_ColorQueue {
Char * szName;      /* the menu name.  must be unique if used by SAM to add
                        and delete functions */
DDV_ColorFunc  pfnColorFunc;
Int4 lPriority;     /* order that the functions are called */
Boolean fOverride;  /* ignore priority and only call this function */
Uint2 procid;       /* the procid of the viewer that owns this function */
struct _DDV_ColorQueue * next; /* these are used to sort the color functions */
struct _DDV_ColorQueue * prev;  /* *not* for storing them together. */
} DDV_ColorQueue;


/*****************************************************************************
*
*   get a color for a Feature. 
*
*****************************************************************************/
NLM_EXTERN DDV_ColorCell DDV_GetFeatColor(DDV_ColorGlobal* pColorGlobal, Int1 idx);

/*****************************************************************************
*
*   setup a DDV_ColorCell with r,g,b values. 
*
*****************************************************************************/
NLM_EXTERN DDV_ColorCell DDV_GetColorRGB(Uint1 r, Uint1 g, Uint1 b);

/*****************************************************************************
*
*   Set default values for DDV_Range.  
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_DefaultRange(DDV_Range *pRange, Int4 lFrom, Int4 lTo);

/*****************************************************************************
*
*   Constructor for DDV_ColorEntry structure.  DDV_ColorEntry is a
*   (Red, Green, Blue) color triplet and associated szName.
*
*****************************************************************************/

NLM_EXTERN DDV_ColorEntry * DDV_CreateColorEntry(Char *szName, Nlm_Uint1 Red,
                          Nlm_Uint1 Green, Nlm_Uint1 Blue);

/*****************************************************************************
*
*   Destructor for DDV_ColorEntry Color.
*
*****************************************************************************/

NLM_EXTERN void DDV_DeleteColorEntry(DDV_ColorEntry *Color);

/*****************************************************************************
*
*   Frees a Palette.
*   Note that it frees the entire Valnode list pointed to by Palette and
*   sets Palette to NULL.
*
*****************************************************************************/

NLM_EXTERN void DDV_FreePalette(ValNode **Palette);

/*****************************************************************************
*
*   Sets the Index in a DDV_ColorEntry according to position in Palette
*
*****************************************************************************/

NLM_EXTERN void DDV_SetColorChoice(ValNode *Palette);

/*****************************************************************************
*
*   Looks for ColorCell on the Palette.  Returns * to the DDV_ColorEntry
*
*****************************************************************************/

NLM_EXTERN DDV_ColorEntry * DDV_SearchColor(ValNode *Palette,
                                            DDV_ColorCell *ColorCell);

/*****************************************************************************
*
*   Looks for a DDV_ColorEntry on the Palette by szName.  Returns *
*   to the DDV_ColorEntry.
*
*****************************************************************************/

NLM_EXTERN DDV_ColorEntry * DDV_SearchColorbyName(ValNode *Palette,
                                                  Char *szName);

/*****************************************************************************
*
*   Looks for a DDV_ColorEntry on the Palette by szName.  Returns *
*   to ColorCell in the DDV_ColorEntry.
*
*****************************************************************************/

NLM_EXTERN DDV_ColorCell * DDV_SearchColorCellbyName(ValNode *Palette, Char *szName);

/*****************************************************************************
*
*   Puts ColorCell on the Palette list if it isn't there already.
*
*****************************************************************************/

NLM_EXTERN void DDV_RequestColor(ValNode **Palette, DDV_ColorCell *ColorCell);

/*****************************************************************************
*
*   Puts ColorEntry on the Palette list if the sZname isn't there already,
*   Otherwise replaces the entry.  Does NOT make a copy of the DDV_ColorEntry
*
*****************************************************************************/

NLM_EXTERN void DDV_RequestColorbyName(ValNode **Palette,
                                       DDV_ColorEntry *pColorIn);

/*****************************************************************************
*
*   Returns the index into the Palette for a given ColorCell.
*   Return index on success, 0 on failure.
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_ColorIndex(ValNode *Palette, DDV_ColorCell *ColorCell);


/*****************************************************************************
*
*   Sets colors in a ColorCell.
*
*****************************************************************************/

NLM_EXTERN void DDV_SetColorInCell(DDV_ColorCell *pColorTo, Uint1 *Color);


/*****************************************************************************
*
*   Copy a DDV_ColorCell.
*   Return 1 on success, 0 on failure.
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_CopyColorCell(DDV_ColorCell * pDestination,
                                  DDV_ColorCell * pSource);

/*****************************************************************************
*
*   Set a color cell using Uint1 values
*   Return 1 on success, 0 on failure
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_SetColorCell(DDV_ColorCell * pColorCell, Nlm_Uint1 Red,
                      Nlm_Uint1 Green, Nlm_Uint1 Blue);


/*****************************************************************************

Function: DDV_CreateColorCell()

Purpose: Creates a new DDV_ColorCell and initializes it.
          
Parameters: none

Returns: pointer to new DDV_ColorCell or NULL on error

*****************************************************************************/

NLM_EXTERN DDV_ColorCell * DDV_CreateColorCell(void);


/*****************************************************************************
*
*   Returns a DDV_ColorCell for a given lPosition if it is inside of pColor.
*   Returns NULL otherwise.
*
*****************************************************************************/

NLM_EXTERN DDV_ColorCell * DDV_GetCellByPosition(DDV_Color *pColor,
                                                 Int4 lPosition);

/*****************************************************************************
*
*   Sets a DDV_ColorCell for a given position if it is inside of pColor.
*   Returns 1 on success, 0 on failure.
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_SetCellByPosition(DDV_Color * pColor, Int4 lPosition,
                 DDV_ColorCell *pColorCell);

/*****************************************************************************
*
*   Creates a DDV_Color that spans the entire object with
*   coordinates lFrom to lTo.
*   The default color is black.
*   Returns NULL on error.
*
*****************************************************************************/

NLM_EXTERN DDV_Color * DDV_CreateDefaultColor(DDV_ColorGlobal *pColorGlobal,
                                        Int4 lFrom, Int4 lTo);

/*****************************************************************************
*
*   Deletes a DDV_Color.
*   Returns 1 on success, 0 on failure.
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_DeleteColor(DDV_Color *pColor);

/*****************************************************************************
*
*   Creates a new DDV_MediaInfo structure.  Creates a default color structure
*   for the given object specified by sip OR row.
*   Inserts the given pColor and sets fVisible.  Duplicates the sip.
*
*   Returns NULL on error.
*
*****************************************************************************/

NLM_EXTERN DDV_MediaInfo* DDV_NewMediaInfo
(SeqId *sip, Int4 lRow, DDV_Color *pColor, Boolean fVisible);

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

NLM_EXTERN ValNode * DDV_IdList(DDV_ColorGlobal *pColorGlobal);

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
(DDV_ColorGlobal * pColorGlobal, SeqId *sip, Int4 lRow, Int4 lFrom, Int4 lTo);

/* simplified version of above that retrieves the length and sets the coordinate
   system to sequence */

NLM_EXTERN Int4 DDV_DefaultMediaInfo(DDV_ColorGlobal* pColorGlobal,SeqId *sip);

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

NLM_EXTERN ValNode * DDV_SipList(DDV_ColorGlobal *pColorGlobal);

/*****************************************************************************
*
*   Deletes a DDV_MediaInfo structure.
*
*   Returns 1 on success, 0 on failure
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_DeleteMediaInfo(DDV_MediaInfo *pMediaInfo);


/*****************************************************************************
*
*   Returns a new color queue entry with name szName, priority lPriority.
*   fOverride is explained above.
*
*****************************************************************************/

NLM_EXTERN DDV_ColorQueue * DDV_NewColorQueue
(DDV_ColorFunc pfnColorFunc, Char * szName,
Int4 lPriority, Boolean fOverride, Uint2 procid);

/*****************************************************************************
*
*   Deletes a DDV_ColorQueue.  
*   Returns 1 on success, 0 on failure
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_DeleteColorQueue(DDV_ColorQueue *pColorQueue);

#define DDVRED(y) ((y)>>16&0xff)
#define DDVGRN(y) ((y)>>8&0xff)
#define DDVBLU(y) ((y)&0xff)

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
(DDV_ColorGlobal * pColorGlobal, Char * ColorName, 
 Char * ParamFile, Uint1 red, Uint1 grn, Uint1 blu);


/*****************************************************************************
*
*   Sets up default secondary structure colors in 
*   pColorGlobal->pvnSpecialColors palette.
*   Returns 0 on failure.
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_DefaultSSColor
(DDV_ColorGlobal * pColorGlobal, Char *ParamFile);
NLM_EXTERN void DDV_LoadSSColor
(DDV_ColorGlobal * pColorGlobal, Char *ParamFile);

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
                                                   void *pObject);

/*****************************************************************************
*
*   Deletes a  DDV_ColorGlobal structure.
*   Returns 1 on success, 0 on failture
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_DeleteColorGlobal(DDV_ColorGlobal *pColorGlobal);

/*****************************************************************************
*
*   Completely deallocate all of the DDV_Color structures in pColorGlobal
*   This is intended for simple minded garbage collection.
*   Returns 1 on success, 0 on failure
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_ClearColor(DDV_ColorGlobal *pColorGlobal);

/*****************************************************************************
*
*   Clears all the colors to the default
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_Clear2Default(DDV_ColorGlobal *pColorGlobal);

/*****************************************************************************
*
*   Delete all information for the given sip OR lRow.
*   Returns 1 on success, 0 on failure
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_RemoveMediaInfo(DDV_ColorGlobal *pColorGlobal, SeqId *sip,
                         Int4 lRow);

/*****************************************************************************
*
*   Add MediaInfo to DDV_ColorGlobal
*   Returns 1 on success, 0 on failure
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_AddMediaInfo(DDV_ColorGlobal *pColorGlobal,
                              DDV_MediaInfo *pMediaInfo);

/*****************************************************************************
*
*   Given a sip OR lRow, return the associated DDV_MediaInfo.
*
*****************************************************************************/

NLM_EXTERN DDV_MediaInfo * DDV_GetMediaInfo
(DDV_ColorGlobal *pColorGlobal, SeqId * sip, Int4 lRow);

/*****************************************************************************
*
*   Given an sip OR lRow, return the visible state.
*
*****************************************************************************/

NLM_EXTERN Boolean DDV_IsVisible(DDV_ColorGlobal *pColorGlobal, SeqId *sip,
                      Int4 lRow);

/*****************************************************************************
*
*   Set the visible state (fVisible) of a sip OR lRow.
*   Returns 1 on success, 0 on failure.
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_SetVisible(DDV_ColorGlobal *pColorGlobal, SeqId *sip,
                    Int4 lRow, Boolean fVisible);

/*****************************************************************************
*
*   Retrieve a color for a sip OR lRow at lPosition in.
*   Returns a DDV_ColorCell containing the color.  NULL on failure.
*
*   The DDV_ColorCell returned is read only.
*
*****************************************************************************/

NLM_EXTERN DDV_ColorCell * DDV_GetColor
(DDV_ColorGlobal *pColorGlobal, SeqId *sip, Int4 lRow, Int4 lPosition);

/*****************************************************************************
*
*   Set a color for in a DDV_MediaInfo at lPosition.  The color set is
*   pColorCell.
*   Returns 1 on success, 0 on failure.
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_SetColorInMediaInfo(DDV_ColorGlobal *pColorGlobal,
                             DDV_MediaInfo *pMediaInfo, Int4 lPosition,
                             DDV_ColorCell *pColorCell);

/*****************************************************************************
*
*   Set a color for a sip OR lRow at lPosition.  The color set is
*   pColorCell.  Makes a copy of pColorCell.
*   Returns 1 on success, 0 on failure.
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_SetColor(DDV_ColorGlobal *pColorGlobal, SeqId *sip,
                  Int4 lRow, Int4 lPosition, DDV_ColorCell *pColorCell);

/*****************************************************************************
*
*   Set a color for a sip OR lRow at lPosition.
*   Uses Uint1's for color input.
*   Returns 1 on success, 0 on failure.
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_SetColorbyColor(DDV_ColorGlobal *pColorGlobal, SeqId *sip,
                         Int4 lRow, Int4 lPosition,
                         Uint1 red, Uint1 green, Uint1 blue);

/*****************************************************************************
*
*   Calls the color functions.  pData is for user data.
*
*   Returns 1 on success, 0 on failure.
*
*   fColorByMaster in DDV_ColorGlobal (i.e. color all sequence like the master
*   sequence) is NOT handled by this function.  This
*   is because this code has, in general, no idea if it is using alignment
*   coordinates or alignment coordinates.  To color by master, the user must
*   register a specific function to do this.
*
*   pRange the range coloring is done over.
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_ColorExecute(DDV_ColorGlobal *pColorGlobal, void * pData,
                      DDV_Range *pRange, char *szName);

/*****************************************************************************
*
*   Callback for coloring all ColorCells by the default color.
*
*
*****************************************************************************/

NLM_EXTERN void DDV_DefaultColor(DDV_ColorGlobal *pColorGlobal, void *pData,
                                DDV_Range *pRange);

/*****************************************************************************
*
*   Adds a DDV_ColorQueue to pColorGlobal
*   Returns 1 on success, 0 on failure.
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_AddColorFunc(DDV_ColorGlobal *pColorGlobal,
                       DDV_ColorQueue *pColorQueue);

/*****************************************************************************
*
*   Deletes the given DDV_ColorFunc out of the ColorQueue
*   Returns number deleted on success, 0 on failure.
*
*****************************************************************************/

NLM_EXTERN Int4 DDV_DeleteColorFunc(DDV_ColorGlobal *pColorGlobal,
                       DDV_ColorFunc  pfnColorFunc);


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

NLM_EXTERN Int4 DDV_AddColorGlobal
(DDV_ColorGlobal *pColorGlobal, void * pObject);


/*****************************************************************************

Function: DDV_GetColorGlobal()

Purpose: Returns the DDV_ColorGlobal attached to an entity as userdata.
  
Parameters: pObject, pointer to the entity.

Returns: DDV_ColorGlobal * on success, NULL on failure

*****************************************************************************/

NLM_EXTERN DDV_ColorGlobal * DDV_GetColorGlobal(void * pObject);


/*****************************************************************************

Function: DDV_GetColorGlobalEx()

Purpose: Returns the DDV_ColorGlobal attached to an entity as userdata. If 
         none is attached, create one and attach it.
  
Parameters: pObject, pointer to the entity.

Returns: DDV_ColorGlobal * on success, NULL on failure

*****************************************************************************/

NLM_EXTERN DDV_ColorGlobal * DDV_GetColorGlobalEx(void * pObject);


#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* DDVCOLOR_H */
