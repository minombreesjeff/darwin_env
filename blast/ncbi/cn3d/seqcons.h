/*   $Id: seqcons.h,v 6.15 2000/04/27 13:36:19 thiessen Exp $
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
* File Name:  $Id: seqcons.h,v 6.15 2000/04/27 13:36:19 thiessen Exp $
*
* Authors:  Paul Thiessen
*
* Version Creation Date: 2/10/2000
*
* File Description: to calculate colors for columns to visualize
*                   sequence conservation
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: seqcons.h,v $
* Revision 6.15  2000/04/27 13:36:19  thiessen
* change algorithm GUI names
*
* Revision 6.14  2000/04/06 19:03:20  thiessen
* new BLOSUM62 conservation coloring
*
* Revision 6.13  2000/03/16 20:31:45  thiessen
* fixes to color; new color-by-variety algorithm
*
* Revision 6.12  2000/03/14 18:03:14  thiessen
* add target row to MMD; remove rowmgr
*
* Revision 6.11  2000/03/03 20:05:17  thiessen
* removal of palette-building pass if in 24-bit color
*
* Revision 6.10  2000/03/03 14:13:42  thiessen
* moved back into ncbicn3d
*
* Revision 1.1  2000/03/02 21:20:41  thiessen
* many improvements, moved into object library
*
* Revision 6.8  2000/02/25 02:53:15  thiessen
* check AlignMgr rather than bAligned flag to see if a residue is aligned
*
* Revision 6.7  2000/02/19 21:25:58  thiessen
* split of cn3dmodl into cn3dmodl and cn3dstyl
*
* Revision 6.6  2000/02/17 15:41:11  thiessen
* added CSC algorithm selection submenu
*
* Revision 6.5  2000/02/16 21:25:32  thiessen
* new row manager module; made Cn3D use row-wise color storage for aligned strucs
*
* Revision 6.4  2000/02/16 14:02:01  thiessen
* further progress on seqcons module
*
* Revision 6.3  2000/02/14 20:04:24  thiessen
* another fix for solvent; more filling in of seqcons
*
* Revision 6.2  2000/02/14 12:32:31  thiessen
* fix solvent identification; progress on seqcons module
*
* Revision 6.1  2000/02/11 01:12:35  thiessen
* new module for sequence conservation coloring
*
*
*
* ==========================================================================
*/

#ifndef _SEQCONS_
#define _SEQCONS_ 1


#include <objalign.h>
#include <mmdbapi.h>
#include <ddvcolor.h>


#ifdef __cplusplus
extern "C" {
#endif


/**** identities and number of available algorithms ****/
enum {
    CSC_BYVARIETY=0, /* first one must be 0 */
    CSC_BYBLOSUM62,
    CSC_SHOWIDENTITY,
    CSC_SHOWALIGNED,
    CSC_NUMALGORITHMS
};

/**** public module functions ****/

/*
 * Returns the string name for the specified algorithm type; NULL if invalid type.
 */
extern const Nlm_Char * CSC_GetAlgorithmName(Nlm_Int2);

/*
 * Calculates (with the given algorithm) colors for all aligned colums.
 * Returns TRUE on success, FALSE on error. Should be called before any colors
 * are read or a different coloring algorithm is desired.
 */
extern Nlm_Boolean CSC_CalculateColumnColors(SeqAlignPtr, Nlm_Int2 useAlgorithm);

/*
 * Returns a color given a row and a bioseq coordinate. Returns CSC_COLOR_ERROR
 * upon error; if successful, the returned color is a packed 3-byte RGB color
 * where
 *   red =   (color & 0x00FF0000) >> 16
 *   green = (color & 0x0000FF00) >> 8
 *   blue =  (color & 0x000000FF)
 *
 * Expects the "seqLoc" numbering to start at 0.
 */
#define CSC_COLOR_ERROR (0xFF222222)
extern Nlm_Int4 CSC_GetColumnColorByRow(Nlm_Int4 row, Nlm_Int4 seqLoc);

/*
 * Goes over a given row, applying appropriate capitalization and,
 * depending on whether the boolean passed is TRUE, color to a residue.
 * The "unalignedColor" gets applied to unaligned residues.
 */
extern void CSC_SetDDVRowCells(Nlm_Int4 row, 
                               Nlm_Boolean applyColumnColor,
                               Nlm_Uint1 *unalignedColor,
                               DDV_ColorGlobal *pDDVCG);

/*
 * Given a PMSD, scans through it and current SeqAlign to find rows that
 * don't have associated structure, and applying CSC_SetDDVRowCells.
 */
extern void CSC_SetNonStructureDDVRowCells(PMSD pmsd, 
                                           Nlm_Boolean applyColumnColor,
                                           Nlm_Uint1 *unalignedColor,
                                           DDV_ColorGlobal *pDDVCG);
 
/*
 * Same as above, just does all rows.
 */
extern void CSC_SetAllDDVRowCells(Nlm_Boolean applyColumnColor,
                                  Nlm_Uint1 *unalignedColor,
                                  DDV_ColorGlobal *pDDVCG);


#ifdef __cplusplus
}
#endif

#endif /* _SEQCONS_ */
