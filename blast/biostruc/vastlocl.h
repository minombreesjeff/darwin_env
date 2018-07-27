/* vastlocl.h
*===========================================================================
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
* File Name:  vastlocl.h
*
* Author:  Christopher Hogue
*
* Version Creation Date:   09/18/95
*
* $Revision: 6.1 $
*
* File Description: Special Header for VAST-locl fetch operations.
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: vastlocl.h,v $
* Revision 6.1  1998/03/30 19:28:37  madej
* Changes to enable subset filtering.
*
 * Revision 6.0  1997/08/25  18:11:56  madden
 * Revision changed to 6.0
 *
* Revision 5.1  1997/05/16 21:12:03  hogue
* initial version
*
*
* ==========================================================================
*/

#ifndef _VASTLOCL_
#define _VASTLOCL_

#include <asn.h>
#include <objall.h>
#include <accentr.h>

#ifdef __cplusplus
extern "C" {
#endif

BiostrucAnnotSetPtr LIBCALL VASTBsAnnotSetGet (Int4 uid);
Boolean LIBCALL IsVASTData(Int4 uid); 
Boolean LIBCALL VASTInit (void); 
void LIBCALL VASTFini (void);
Int4 LIBCALL LoadSubsetTable(Char *fname);

#ifdef __cplusplus
}
#endif

#endif
