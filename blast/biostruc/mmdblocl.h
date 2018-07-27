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
* File Name:  mmdblocl.h
*
* Author:  Christopher Hogue
*
* Version Creation Date:   09/18/95
*
* $Revision: 6.4 $
*
* File Description: Special Header for MMDB-locl fetch operations.
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: mmdblocl.h,v $
* Revision 6.4  1999/06/22 23:03:07  kimelman
* synced to mmdbFF.c
*
* Revision 6.3  1999/05/11 21:32:02  kimelman
* added mmdbFF interface.
*
* Revision 6.2  1999/04/22 01:58:03  kimelman
* decl of MMDB_configuraion added
*
* Revision 6.1  1998/04/15 14:54:03  kimelman
* 1. Make retrieval unifirm from open server and sql server.
* 2. mmdbsrv retrival performance tuning:
* 	- size of read-in buffers
* 	- direct pdb2mmdb DB lookup instead of full loading pdb to mmdb translataion
* 	  table
* 3. cleaning mmdblocl.* and make.mmdbsrv in order to remove obsolete code
*
* Revision 6.0  1997/08/25 18:11:26  madden
* Revision changed to 6.0
*
* Revision 1.1  1997/03/19 16:58:02  hogue
* Initial revision
*
*
* ==========================================================================
*/

#ifndef _MMDBLOCL_
#define _MMDBLOCL_

#include <asn.h>
#include <objall.h>
#include <accentr.h>

#ifdef __cplusplus
extern "C" {
#endif

DocUid      LIBCALL MMDBEvalPDB        (CharPtr str);
BiostrucPtr LIBCALL MMDBBiostrucGet    (DocUid uid, Int4 mdlLvl, Int4 maxModels);
Boolean     LIBCALL MMDBInit           (   void    );
void        LIBCALL MMDBFini           (   void    );
CharPtr     LIBCALL MMDB_configuration (   void    );

#ifdef __cplusplus
}
#endif

#endif
