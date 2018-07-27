/*   mmdbdumy.c
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
* File Name:  mmdbdumy.c
*
* Author:  Christopher Hogue
*
* Version Creation Date:  14 Jan 97  
*
* File Description: Used to link with MMDB-API 
* if MMDB data fetches not required.
*                   
* Modifications:  
* --------------------------------------------------------------------------

/* to dummy out all MMDB-database dependent calls, link this instead.         */
/* No explicit MMDB service, only local-loads of ASN.1 Biostrucs are allowed  */

#include <ncbi.h>
#include <mmdbapi.h>
#include <mmdbdata.h>

Boolean LIBCALL MMDBInit (void)
{
  return FALSE;
}


void LIBCALL MMDBFini (void)
{
  return;
}


BiostrucPtr LIBCALL MMDBBiostrucGet (DocUid uid, Int4 mdlLvl, Int4 maxModels)
{
   return (BiostrucPtr) NULL;
} 


DocUid  LIBCALL MMDBEvalPDB (CharPtr str);
{
 return (DocUid) 0;
}
