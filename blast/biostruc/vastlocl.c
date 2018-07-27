/*   vastlocl.c
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
* File Name:  vastlocl.c
*
* Author:  Christopher Hogue
*
* Version Creation Date:  21 February 97  
*
* $Revision: 6.6 $
*
* File Description: Used to provide VAST data to MMDB-API when
*  VAST files are on a local filesystem in standard VAST ftp-site format.
*                   
* Modifications:  
* --------------------------------------------------------------------------
* $Log: vastlocl.c,v $
* Revision 6.6  2000/06/21 21:40:07  lewisg
* change to using gzip
*
* Revision 6.5  1998/07/27 16:07:40  madej
* Pipe the results of gunzip when we get a BAS.
*
 * Revision 6.4  1998/05/19  20:13:21  madej
 * Gunzip needed to run vastsrv on Suns.
 *
 * Revision 6.3  1998/03/30  19:28:55  madej
 * Changes to enable subset filtering.
 *
 * Revision 6.2  1998/03/16  15:41:34  lewisg
 * got rid of gunzip in config file
 *
* Revision 6.1  1998/03/06 01:14:12  lewisg
* merge
*
*
*
* ==========================================================================
*/


/********   NOTE - .mmdbrc (UNIX) mmdb.ini (Win) mmdb.cnf (Mac) required   *******/
/* containing paths to database VAST          *****/
/* 
[VAST]
Database = /net/dorothy/export/home/hogue/mmdb/Oct96/vastdata/
 */



#include <ncbi.h>
#include <mmdbapi1.h>
#include "vastlocl.h"

#ifdef OS_UNIX
#define MMDB_UNIXCOMPRESSED 1
#endif
 

static Char database[PATH_MAX];
static Char gunzip[PATH_MAX];

    

BiostrucAnnotSetPtr LIBCALL VASTBsAnnotSetGet (Int4 uid)
{
   AsnIoPtr aip = NULL;
   AsnTypePtr atp = NULL;
   Char path[PATH_MAX];
   Char compath[PATH_MAX];
   Char tempfile[PATH_MAX];
   Char pcId[20];    
   Int2 iFileExists = 0;
   BiostrucAnnotSetPtr pbsa = NULL;
   int iAvail = 1;
   FILE *pipe;

   sprintf(pcId, "%ld", (long) uid);
   path[0] = '\0';
   StringCpy(path, database);
   StringCat(path, pcId);
   StringCat(path, ".bas");

#ifdef MMDB_UNIXCOMPRESSED
   compath[0] = '\0';
   sprintf(compath, "%s -d -c %s.gz ", gunzip, path);
   pipe = popen(compath, "rb");

   if (pipe == NULL) {
       ErrPostEx(SEV_FATAL,0,0, "VASTBsAnnotSetGet failed: Can't find gunzip in path.\n");
       return NULL;
   }

   aip = AsnIoNew(ASNIO_BIN_IN, pipe, NULL, NULL, NULL);
#else
   iFileExists = FileLength(path);
   if (iFileExists == 0)
      {
        return NULL;
      }
      
    aip = AsnIoOpen(path, "rb");
#endif

    if (aip) 
     {
       pbsa = BiostrucAnnotSetAsnRead(aip, NULL);
       AsnIoClose (aip);
     }

#ifdef MMDB_UNIXCOMPRESSED 
    pclose(pipe);
#endif

    if (!pbsa) return NULL;  
    return pbsa;
} 


Boolean LIBCALL IsVASTData(Int4 uid)
{
   AsnIoPtr aip = NULL;
   AsnTypePtr atp = NULL;
   Char path[PATH_MAX];
   Char pcId[30];

   sprintf(pcId, "%ld", (long) uid);
   path[0] = '\0';
   StringCpy(path, database);
   StringCat(path, pcId);
   StringCat(path, ".bas");

#ifdef MMDB_UNIXCOMPRESSED 
   StringCat(path, ".gz");
   if (FileLength(path) != 0)
     return TRUE;
#else

   if (FileLength(path) != 0)
     return TRUE;
#endif
   return FALSE;
}


#ifndef _lygmerge

Boolean LIBCALL VASTInit (void)
{
  Char fname[PATH_MAX];

  database[0] = gunzip[0] = '\0';
  GetAppParam("vast", "VASTSRV", "Database", "", database, PATH_MAX);
  if (database[0] == '\0')
    {
      	ErrPostEx(SEV_FATAL,0,0, "MMDB config file / path to VAST data missing.\n");
	return FALSE;
    }
  GetAppParam("vast", "VASTSRV", "Gunzip", "", gunzip, (size_t) 256 * (sizeof(char)));
  if (gunzip[0] == '\0')
    {
      	ErrPostEx(SEV_FATAL,0,0, "VASTInit failed: gunzip path missing.\n");
	return FALSE;
    }

  GetAppParam("vast", "VASTSRV", "SubsetFile", "", fname, PATH_MAX);

  if (fname[0] == '\0') {
	ErrPostEx(SEV_FATAL, 0, 0, "VASTInit failed: no subset file name.\n");
	return FALSE;
  }

  if (LoadSubsetTable(fname) != 1) {
    ErrPostEx(SEV_FATAL, 0, 0, "VASTInit failed: can't load subset table.\n");
    return FALSE;
  }

  return TRUE;

}

#else
Boolean LIBCALL VASTInit (void)
{

  GetAppParam("mmdb", "VAST", "Database", "", database, PATH_MAX);
  if (database[0] == NULL)
    {
      	ErrPostEx(SEV_FATAL,0,0, "MMDB config file / path to VAST data missing.\n");
	return FALSE;
    }
  return TRUE;
}
#endif

void LIBCALL VASTFini (void)
{
  return;
}
