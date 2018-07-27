/* ===========================================================================
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
* File Name:  mmdbdata.h
*
* Author:  Christopher Hogue
*
* Version Creation Date:   09/18/95
*
* $Revision: 6.1 $
*
* File Description: General Header for MMDB-data fetch operations.
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: mmdbdata.h,v $
* Revision 6.1  2001/01/26 15:06:39  lewisg
* use entrez2 to retrieve structures
*
* Revision 6.0  1997/08/25 18:11:21  madden
* Revision changed to 6.0
*
* Revision 1.2  1997/01/15 18:17:15  hogue
* Added missing include.
*
 * Revision 1.1  1997/01/15  17:12:17  hogue
 * Initial revision
 *
*
* ==========================================================================
*/

#ifndef _MMDBDATA_
#define _MMDBDATA_

#include <asn.h>
#include <objall.h>
#include <mmdbapi1.h>
#include <mmdbapi2.h>
#include <mmdbapi3.h>

#ifdef __cplusplus
extern "C" {
#endif


#ifndef DocUid
#define DocUid Int4
#endif

Boolean     LIBCALL MMDBInit (void);
void        LIBCALL MMDBFini (void);
BiostrucPtr LIBCALL MMDBBiostrucGet (DocUid uid, Int4 mdlLvl, Int4 maxModels);
DocUid      LIBCALL MMDBEvalPDB (CharPtr str);
CharPtr     LIBCALL MMDB_configuration(void);

#ifdef __cplusplus
}
#endif

#endif
