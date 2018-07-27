/*   cn3dsave.h
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
* File Name:  cn3dsave.h
*
* Author:  Christopher Hogue
*
* Version Creation Date:   1/31/96
*
* $Revision: 6.3 $
*
* File Description: Cn3d file saving routines 
*                   
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* $Log: cn3dsave.h,v $
* Revision 6.3  2000/03/22 23:17:50  thiessen
* added ability to save ARS in ASN1
*
* Revision 6.2  2000/03/08 21:46:15  lewisg
* cn3d saves viewport, misc bugs
*
* Revision 6.1  1999/10/29 14:15:30  thiessen
* ran all Cn3D source through GNU Indent to prettify
*
* Revision 6.0  1997/08/25 18:13:43  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 14:05:44  ostell
* Set to revision 5.0
*
 * Revision 1.1  1996/02/01  18:47:38  kans
 * Initial revision
 *
*
* ==========================================================================
*/

/* cn3dsave.h */

#ifndef _CN3DSAVE_
#define _CN3DSAVE_ 1

#ifdef __cplusplus
extern "C" {
#endif

NLM_EXTERN void Cn3D_SaveBiostruc(IteM i);
NLM_EXTERN void Cn3D_GetRenderSettingsFromBiostruc(PDNMS pdnms);

#ifdef __cplusplus
}
#endif
#endif
