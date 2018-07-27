/* $Id: mmdbFF.c,v 6.4 2000/06/20 20:47:03 lewisg Exp $
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
* File Name:  mmdbFF.c - file based retrieval interface
* 
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: mmdbFF.c,v $
* Revision 6.4  2000/06/20 20:47:03  lewisg
* use gzip
*
* Revision 6.3  1999/09/08 18:43:54  zimmerma
* Modified HashPDBCode with calls to "toupper" to ensure case-insensitivity
*
* Revision 6.2  1999/06/22 22:35:02  kimelman
* function names were changed to original names
* so we have now 3 version of access files again
*
* Revision 6.1  1999/05/11 21:31:07  kimelman
* file based retrival funciton extracted here from mmdblocl.c
* (function names slightly changed)
*
*
*/

/* LoadMMDBIdx reads the local MMDB index "mmdb.idx" into memory */
/* index format is
3\n
1 1ABC\n
3 3INS\n
234567 4GWA\n
eof
*/

#include <ncbi.h>
#include <mmdbapi1.h>
#include <mmdbdata.h>
#include <mmdblocl.h>
#include <assert.h>

static Int4Ptr pI4Mmdbidx = NULL;
static Uint4Ptr pU4Pdbidx = NULL;
static long iMMDBSize = 0;
static Char indexname[PATH_MAX];
static Char database[PATH_MAX];
static Int4Ptr pI4position = NULL;
static Char gunzip[PATH_MAX];


/* These functions are only required when working with flat files:
 * - Load/Remove the mmbd.idx file for cross referencing MMDBids with PDBids
 * - Define the hash function for PDB strings					*/

static Uint4 HashPDBCode(CharPtr pcPdb) {
  Uint4 iPdbhash;
  Uint4 c1, c2, c3, c4;

  if (!pcPdb) return 0;
  if (StringLen(pcPdb) != 4) return 0;
  c1 =  toupper(pcPdb[3]);
  c2 =  toupper(pcPdb[2]);
  c3 =  toupper(pcPdb[1]);
  c4 =  toupper(pcPdb[0]);
  iPdbhash =  c1 + (c2 << 8) + (c3 << 16) + (c4 << 24);
  
  return iPdbhash;
}

/* The index file is loaded as a list of (long) mmdbids correlated with a 
   list of unsigned long HASHED PDB codes					*/

static Boolean LoadMMDBIdx(CharPtr db,  CharPtr idx) {
  FILE *f;
  Char fullpath [PATH_MAX];
  CharPtr ptr;
  Char pcBuf[100];
  CharPtr pcTest;
  Char  pcMmdb[20];
  Char  pcPdb[20];
  Uint4Ptr     pU4Pdb;
  Int4Ptr      pI4Mmdb;
  long count = 0;
  long mmdbid = 0;
  int status;
  char *msg;

#define ERROR(err_msg) { msg = err_msg; goto errexit; }

  StringCpy(fullpath, db);
  StringCat(fullpath, idx);
  if ((f = FileOpen (fullpath, "r")) == NULL) 
    ERROR("MMDBInit: Couldn't load MMDB index.");
   
  fscanf(f, "%ld", &iMMDBSize);
  if ((iMMDBSize == 0) || (iMMDBSize > 100000))
    ERROR ("Internal - LoadMMDBIdx() Failure 2;");

  pI4Mmdbidx = (Int4Ptr) MemNew((size_t) ((iMMDBSize)*sizeof(Int4)));
  pU4Pdbidx = (Uint4Ptr) MemNew((size_t) ((iMMDBSize)*sizeof(Uint4)));

  if ((!pI4Mmdbidx) || (!pU4Pdbidx))
    ERROR ("Internal - LoadMMDBIdx() Out of Memory;");
   
  pI4Mmdb = pI4Mmdbidx;
  pU4Pdb = pU4Pdbidx;
   
  while ((status = fscanf(f, "%ld%s",  &mmdbid,  pcPdb)) != EOF) {

    StrUpper(pcPdb);
    if ((mmdbid == 0) ||  (StringLen(pcPdb) > 4)) {
      MMDBFini();
      ERROR ("Internal - LoadMMDBIdx() Bad Index Values");
    }
    
    *pI4Mmdb = mmdbid;
    *pU4Pdb =  HashPDBCode(pcPdb);
    pI4Mmdb++;
    pU4Pdb++;
    count++;
    if (count > (iMMDBSize)) {
      MMDBFini();
      ERROR ("Internal - LoadMMDBIdx() Index count is wrong at top of file;");
    }
  }
  FileClose(f);
  return TRUE;

#undef ERROR
errexit:
  ErrPostEx(SEV_FATAL,0,0, msg);
  return FALSE;
}

static void CloseMMDBIdx() {
  if (pI4Mmdbidx) MemFree(pI4Mmdbidx);
  if (pU4Pdbidx) MemFree(pU4Pdbidx);
  return;
}

/* Given a Char PDBid, hash-encode it and retrieve the corresponding long MMDBid */

DocUid MMDBEvalPDB (CharPtr str) 
{
  register Uint4Ptr pU4Pdb = NULL;
  register Int4Ptr pI4Mmdb = NULL;
  register Uint4   iHash = 0;
  long i = 0;
  
  ASSERT ((pI4Mmdbidx != NULL) && (pU4Pdbidx != NULL));
  pI4Mmdb = pI4Mmdbidx;
  pU4Pdb = pU4Pdbidx;
  iHash = HashPDBCode(str);

  while (i++ < iMMDBSize && iHash != *pU4Pdb) {
    pU4Pdb++;
    pI4Mmdb++;
  } 
  if (iHash == *pU4Pdb)
    return (DocUid) *pI4Mmdb;
  return (DocUid) 0;
}

/*****************************FF_BiostrucGet******************************/
BiostrucPtr MMDBBiostrucGet (DocUid uid, Int4 mdlLvl, Int4 maxModels)
{
  Char path[PATH_MAX], compath[PATH_MAX];
  AsnIoPtr    aip = NULL;
  FILE *pipe;
  BiostrucPtr pbs = NULL;

  sprintf(path, "%s%ld.val", database, (long) uid);
  if (FileLength(path) >0)
    aip = AsnIoOpen(path, "rb");
  else
    {
      if (gunzip[0] == 0) {
        ErrPostEx(SEV_FATAL,0,0, "MMDBBiostrucGet_files failed: gunzip path missing.");
        return NULL;
      }
      sprintf(path, "%s%ld.val.gz", database, (long) uid);
      if (FileLength(path) <=0)
        return NULL;
      
      sprintf(compath,"%s -c -d %s ", gunzip, path);
      pipe=popen(compath,"rb");
      if (pipe == 0)
        {
          ErrPostEx(SEV_FATAL,0,0, "MMDBBiostrucGet failed: Can't find gunzip in path");
          return NULL;
        }
      aip = AsnIoNew(ASNIO_BIN_IN, pipe , NULL, NULL, NULL);
    }
  if (!aip)
    return NULL;

  pbs = BiostrucAsnGet(aip, NULL,  mdlLvl,  maxModels);
  AsnIoClose (aip);
  return pbs;
}

Boolean MMDBInit (void) 
{
  char *msg;
  char ndxPath[PATH_MAX];
  FILE *f;
  
#define ERROR(err_msg) { msg = err_msg; goto errexit; }

  GetAppParam("mmdb", "MMDB", "Index"   , "", indexname, sizeof(indexname));
  GetAppParam("mmdb", "MMDB", "Database", "", database, sizeof(database));
  if (!(database[0] && indexname[0]))
    return FALSE ;
  
  /* let's check if index file really there */
    
  assert(sizeof(ndxPath) > strlen(database) + strlen(indexname));
  sprintf(ndxPath,"%s%s",database,indexname);
  
  if ((f = fopen(ndxPath,"r")) == NULL)
    ERROR("MMDBInit failed: incorrect mmdb.idx path.");
  fclose(f);
  if(FALSE==LoadMMDBIdx(database,  indexname))
    return FALSE;
  GetAppParam("mmdb", "MMDBSRV", "Gunzip", "", gunzip, sizeof(gunzip));
  if(gunzip[0])
    {
      f = fopen(gunzip,"r");
      if(f)
        fclose(f);
      else
        gunzip[0]=0;
    }
  return TRUE;
  
#undef ERROR

errexit:
  ErrPostEx(SEV_ERROR,0,0, msg);
  return FALSE;
}

void LIBCALL
MMDBFini (void)
{
  CloseMMDBIdx();
}

CharPtr LIBCALL
MMDB_configuration(void)
{
  return "Version:\t$Id: mmdbFF.c,v 6.4 2000/06/20 20:47:03 lewisg Exp $\nConfiguration: Flat Files" ;
}
