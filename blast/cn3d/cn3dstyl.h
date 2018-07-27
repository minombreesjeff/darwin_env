/*   $Id: cn3dstyl.h,v 6.4 2000/03/17 22:48:59 thiessen Exp $
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
* File Name:  $Id: cn3dstyl.h,v 6.4 2000/03/17 22:48:59 thiessen Exp $
*
* Version Creation Date: 2/19/2000
*
* File Description: code to handle special styles/features
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: cn3dstyl.h,v $
* Revision 6.4  2000/03/17 22:48:59  thiessen
* fix for multi-chain / multi-model features ; added feature-move ; misc bug fixes
*
* Revision 6.3  2000/03/13 16:04:26  thiessen
* re-implementation of features
*
* Revision 6.2  2000/02/26 00:01:41  thiessen
* OpenGL improvements, progress on cleanup of Show/Hide
*
* Revision 6.1  2000/02/19 21:25:58  thiessen
* split of cn3dmodl into cn3dmodl and cn3dstyl
*
*
*
* ==========================================================================
*/

#ifndef _CN3DSTYL_
#define _CN3DSTYL_ 1

#ifdef __cplusplus
extern "C" {
#endif

extern GrouP LIBCALL ModelControls PROTO((Nlm_GrouP prnt));
extern void ClearRest(void);
extern void Cn3DIndexUserDefinedFeature(void);
extern PDNMS Cn3DAddUserDefinedFeatureToBiostruc(PDNMS pdnmsThis);
extern void Cn3DRemoveUserDefinedFeatureFromBiostruc(PDNMS pdnmsThis);

#ifdef __cplusplus
}
#endif

#endif /* _CN3DSTYL_ */
