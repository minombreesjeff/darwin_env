/*   cn3dmsg.h
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
* File Name:  cn3dmsg.h
*
* Author: Yanli Wang
*
* Version Creation Date:   3/26/98
*
* File Description: Main functions for building up cn3d/salsa communication
*
* Modifications:
* $Log: cn3dmsg.h,v $
* Revision 6.55  2000/03/18 00:06:00  lewisg
* add blast, new help, new menus
*
* Revision 6.54  2000/02/07 20:17:37  lewisg
* minor bug fixes, use gui font for win32
*
* Revision 6.53  2000/01/06 00:04:42  lewisg
* selection bug fixes, update message outbound, animation APIs moved to vibrant
*
* Revision 6.52  1999/12/28 15:08:44  lewisg
* remove remaining mediainfo code
*
* Revision 6.51  1999/12/01 16:15:54  lewisg
* interim checkin to fix blocking memory leak
*
* Revision 6.50  1999/11/22 14:46:44  thiessen
* moved _OPENGL code blocks to only vibrant and ncbicn3d libraries
*
* Revision 6.49  1999/11/10 23:19:41  lewisg
* rewrite of selection code for ddv
*
* Revision 6.48  1999/11/03 18:15:10  kans
* added prototypes needed by other files
*
* Revision 6.47  1999/10/29 14:15:29  thiessen
* ran all Cn3D source through GNU Indent to prettify
*
* Revision 6.46  1999/10/05 23:18:24  lewisg
* add ddv and udv to cn3d with memory management
*
* Revision 6.45  1999/09/21 18:09:15  lewisg
* binary search added to color manager, various bug fixes, etc.
*
* Revision 6.44  1999/08/04 21:19:46  lewisg
* modularized open operations to allow sequin to launch cn3d
*
* Revision 6.43  1999/07/09 20:50:55  ywang
* set highlight color for salsa in Cn3dObjRegiste
*
* Revision 6.42  1999/07/07 20:45:37  ywang
* clear domaindata, mediadata, special feature before reading in new data in cn3d
*
* Revision 6.41  1999/04/06 20:12:57  lewisg
* more opengl
*
* Revision 6.40  1999/03/30 22:36:19  ywang
* add functions to color salsa for NcbiMimeAsn1_strucseqs & code reorganization
*
* Revision 6.39  1999/03/22 22:41:14  ywang
* remove argument in MediaObjSelect
*
* Revision 6.38  1999/02/12 15:34:01  ywang
* include Cn3DSendColorMsg
*
* Revision 6.37  1999/02/11 22:40:15  ywang
* rename functions
*
* Revision 6.36  1999/02/11 18:48:15  lewisg
* delete color index functions
*
* Revision 6.35  1999/02/10 23:49:43  lewisg
* use RGB values instead of indexed palette
*
* Revision 6.34  1999/01/20 22:57:25  ywang
* customize color for secondary structure & rearrange Option menu
*
* Revision 6.33  1999/01/20 16:06:48  durand
* move mediainfo to salmedia.h
*
* Revision 6.32  1998/12/16 22:49:39  ywang
* fix compiling warnings on Win32
*
 * Revision 6.31  1998/12/16  19:32:56  ywang
 * improve highlight residues function when rerendering
 *
 * Revision 6.30  1998/10/27  15:55:52  ywang
 * add functions for testing color by sequence conservation
 *
 * Revision 6.29  1998/10/19  20:16:06  ywang
 * add function FillSeqinfoForSeqEditViewProcs so that salsa can get color array
 *
 * Revision 6.28  1998/10/19  17:43:03  kans
 * prototype needed for RealColorSalsa
 *
* Revision 6.27  1998/10/16 22:06:08  ywang
* make global color array for sequence display
*
 * Revision 6.26  1998/09/23  22:09:42  ywang
 * to record checkin log
 *
* ===========================================================================  */

#ifndef _CN3DMSG_
#define _CN3DMSG_ 1

#include <ncbi.h>
#include <objseq.h>
#include <objmgr.h>
#include <objfdef.h>
#include <gather.h>
#include <vibrant.h>
#include <salsa.h>
#include <salmedia.h>
#include <ddvcolor.h>

#include <mmdbapi1.h>

#ifdef __cplusplus
extern "C" {
#endif


#define REGISTER_BIOSEQ_BIOSTRUC_MEDIA ObjMgrProcLoad(OMPROC_VIEW, "Seq-Struc Communication", "Media", OBJ_BIOSEQ, 0, OBJ_BIOSEQ, 0, NULL, SeqStrucMediaFunc, 0)
#define REGISTER_SEQANNOT_BIOSTRUC_MEDIA ObjMgrProcLoad(OMPROC_VIEW, "Cn3D SeqAnnot", "Media", OBJ_SEQANNOT, 0, OBJ_SEQANNOT, 0, NULL, Cn3D_AnnotEditFunc, 0)

/*****************************************************************************

Function: Cn3D_LaunchSeqEntry()

Purpose: Launch the Bioseq viewer on all bioseqs contained in a SeqEntry.

Parameters: pvnsep, the valnode list of SeqEntries

*****************************************************************************/

void Cn3D_LaunchSeqEntry(ValNode * pvnsep);


/*****************************************************************************

Function: Cn3D_RegisterSeqEntry()

Purpose: Adds a message func to the SeqEntry and makes it
     OM_OPT_FREE_IF_NO_VIEW

Parameters: pvnsep, a valnode list of SeqEntries

*****************************************************************************/

void Cn3D_RegisterSeqEntry(ValNode * pvnsep);


/*****************************************************************************

Function: Cn3D_LaunchSeqAnnot()

Purpose: Launch the SeqAlign viewer on all SeqAligns contained in a SeqAnnot.

Parameters: sap, the SeqAnnot

*****************************************************************************/

void Cn3D_LaunchSeqAnnot(SeqAnnot * sap);


/*****************************************************************************

Function: Cn3D_RegisterSeqAnnot()

Purpose: Adds a message func to the SeqAnnot and makes it
     OM_OPT_FREE_IF_NO_VIEW

Parameters: sap, the SeqAnnot pointer

*****************************************************************************/

void Cn3D_RegisterSeqAnnot(SeqAnnot *sap, Boolean Neat, Boolean IBM);


extern Int2 LIBCALLBACK SeqStrucMediaFunc PROTO((Pointer data));
extern void MediaObjSelect(PDNMG pdnmgThis, Boolean highlight);
extern void MediaHL(SelStructPtr sel, Boolean highlight);
extern void fnPreCHLresidue(PDNMG pdnmgThis, Boolean highlight);
extern PDNMM FindMM(void);
extern void DoCHighlightSeg(PFB pfbThis, Int4 iModel, Int4 iIndex,
                            Pointer ptr, Boolean highlight);
extern void LaunchMediaViewer(BioseqPtr bsp);
extern void SalsaRegister(void);
extern void Cn3dObjMgrGetSelected(void);
extern void DoMediaHL(PMMD pmmdThis, Int4 from, Int4 to,
                      Boolean highlight);
extern void LaunchSequenceWindow(void);
extern void LIBCALLBACK Cn3DCheckAndDoHighlight
    PROTO((PFB pfbThis, Int4 iModel, Int4 iIndex, Pointer ptr));

extern void Cn3D_RegisterColor(void);


/*****************************************************************************

Function: Cn3D_AnnotEditFunc()

Purpose: The object manager callback to register a SeqAnnot.

Parameters: data, the OMProcControlPtr.

Returns: OM_MSG_RET_*

*****************************************************************************/

extern Int2 LIBCALLBACK Cn3D_AnnotEditFunc(Pointer data);


/*****************************************************************************

Function: Cn3D_SendUpdate()

Purpose: Sends an update message to everyone about everything.
  
*****************************************************************************/

NLM_EXTERN void Cn3D_SendUpdate();




#ifdef __cplusplus
}
#endif
#endif                          /* _CN3DMSG_ */
