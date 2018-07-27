/*   mmdblocl.c
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
* File Name:  mmdblocl.c
*
* Author:  Christopher Hogue
*
* Version Creation Date:  14 January 97  
*
* $Id: mmdblocl.c,v 6.6 2001/12/12 20:43:38 beloslyu Exp $
*
* File Description: Used to provide Biostruc data to MMDB-API when
*  MMDB files are on a local filesystem in standard MMDB ftp-site format.
*                   
* Modifications:  
* --------------------------------------------------------------------------
* $Log: mmdblocl.c,v $
* Revision 6.6  2001/12/12 20:43:38  beloslyu
* change the name of ftp site to ftp.ncbi.nih.gov
*
* Revision 6.5  1999/05/11 23:28:25  kimelman
* 1. no pubstruct by default
* 2. define 'extern' info
*
* Revision 6.4  1998/06/09 18:42:11  kimelman
* accepted __NO_DB__ flag to compile without DB access library
*
* Revision 6.3  1998/05/08 03:03:50  kimelman
* Open Server fix
*
* Revision 6.2  1998/04/15 14:54:02  kimelman
* 1. Make retrieval unifirm from open server and sql server.
* 2. mmdbsrv retrival performance tuning:
* 	- size of read-in buffers
* 	- direct pdb2mmdb DB lookup instead of full loading pdb to mmdb translataion
* 	  table
* 3. cleaning mmdblocl.* and make.mmdbsrv in order to remove obsolete code
*
* Revision 6.1  1998/04/03 20:25:22  kimelman
* PubStruct access code added to mmdbsrv
*
* Revision 6.0  1997/08/25 18:11:25  madden
* Revision changed to 6.0
*
* Revision 1.5  1997/07/14 18:29:03  madej
* Changed gunzip access to get pathname from config file.
*
 *
 * Revision 1.4  1997/03/25  22:24:42  hogue
 * Fixed an off-by-one error.
 *
 * Revision 1.3  1997/03/19  16:56:52  hogue
 * Added MMDB Iterators, made better error messages
 *
 * Revision 1.2  1997/02/05  23:13:56  hogue
 * Added gunzip as default method of decompression of files on UNIX systems.
 * Directs gunzip to a temp file, then deletes it after the load.
 *
 * Revision 1.1  1997/01/27  18:42:02  hogue
 * Initial revision
 *
*
*
* ==========================================================================
*/

/*********************************************************************************/
/* This file abstracts the calls to Network Entrez that were previously          */
/* embedded in mmdbapi1.c  Linking with this file forces local MMDB storage      */
/* Local MMDB storage is defined as that distributed on the MMDB FTP site at     */
/* ftp://ftp.ncbi.nih.gov/pub/mmdb/mmdb/Biostruc                                 */
/* which contains MMDB ASN.1 files of the form 12345.val and an index "mmdb.idx" */
/* This is set up so that one can mirror the MMDB database from the ftp site.    */

/********   NOTE - .mmdbrc (UNIX) mmdb.ini (Win) mmdb.cnf (Mac) required   *******/
/* containing paths to database and index file name:                         ****** 
 *
 * [MMDB]
 * Database = /net/vaster/mmdb/mmdb/data
 * Index    = mmdb.idx
 * 
 * or 
 * 
 * [MMDB]
 * PubStruct= yes
 * Database = PUBSEQ_OS:PubStruct=anyone:allowed
 * 
 */

#include <ncbi.h>
#include <mmdbapi1.h>
#include <mmdbdata.h>
#include <mmdblocl.h>
#include <accentr.h>
#include <accutils.h>
#include "PubStructAsn.h"
#include <assert.h>


#ifdef OS_UNIX
#define MMDB_UNIXCOMPRESSED 1
#endif

#ifdef MMDB_PUBSTRUCT
static int  file_db;    
#endif

/* LoadMMDBIdx reads the local MMDB index "mmdb.idx" into memory */
/* index format is
3\n
1 1ABC\n
3 3INS\n
234567 4GWA\n
eof
*/


static Int4Ptr pI4Mmdbidx = NULL;
static Uint4Ptr pU4Pdbidx = NULL;
static long iMMDBSize = 0;
static Char database[PATH_MAX];
static Char indexname[256];
static Int4Ptr pI4position = NULL;
static Char gunzip[PATH_MAX];

static Uint4
HashPDBCode(CharPtr pcPdb)
{

  Uint4 iPdbhash;
  Uint4 c1, c2, c3, c4;

  if (!pcPdb) return 0;
  if (StringLen(pcPdb) != 4) return 0;
  c1 =  pcPdb[3];
  c2 =  pcPdb[2];
  c3 =  pcPdb[1];
  c4 =  pcPdb[0];
  iPdbhash =  c1 + (c2 << 8) + (c3 << 16) + (c4 << 24);
  
  return iPdbhash;
}

static Boolean
LoadMMDBIdx(CharPtr db,  CharPtr idx)
{
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
  /* StrToLong stuff */
  Nlm_Char     ch;
  Nlm_Int2     i;
  Nlm_Int2     len;
  Nlm_Char     local [64];
  Nlm_Boolean  nodigits;
  Nlm_Boolean  rsult;
  long int     val;
 

  StringCpy(fullpath, db);
  StringCat(fullpath, idx);
  if ((f = FileOpen (fullpath, "r")) == NULL) 
    {
      ErrPostEx(SEV_FATAL,0,0, "MMDBInit - Failed to load MMDB index - check config file.");
      return FALSE;
    } 
   
  pcTest = fgets(pcBuf, (size_t)100, f);
  if (pcTest)
    {
      sscanf(pcBuf, "%ld", &iMMDBSize);
    }
  if ((iMMDBSize == 0) || (iMMDBSize > 100000))
    {
      ErrPostEx(SEV_FATAL,0,0, "Internal - LoadMMDBIdx() Failure 2;");
      return FALSE;
    } 
  pI4Mmdbidx = (Int4Ptr) MemNew((size_t) ((iMMDBSize)*sizeof(Int4)));
  pU4Pdbidx = (Uint4Ptr) MemNew((size_t) ((iMMDBSize)*sizeof(Uint4)));

  if ((!pI4Mmdbidx) || (!pU4Pdbidx))
    {
      ErrPostEx(SEV_FATAL,0,0, "Internal - LoadMMDBIdx() Out of Memory;");
      return FALSE;
    }
   
  pI4Mmdb = pI4Mmdbidx;
  pU4Pdb = pU4Pdbidx;
   
  do 
    {
      pcBuf[0] = '\0';
      pcTest = fgets(pcBuf,  (size_t)100,  f);
      if (pcTest)
        {
          sscanf(pcBuf, "%s%s",  pcMmdb ,  pcPdb);
          StrUpper(pcPdb);	 
          mmdbid = 0;
 	    
          /* this is code from Nlm StrToLong */
          rsult = FALSE;
          len = (Nlm_Int2) Nlm_StringLen (pcMmdb);
          if (len != 0) {
            rsult = TRUE;
            nodigits = TRUE;
            for (i = 0; i < len; i++) {
              ch = pcMmdb [i];
              if (ch == ' ' || ch == '+' || ch == '-') {
              } else if (ch < '0' || ch > '9') {
                rsult = FALSE;
              } else {
                nodigits = FALSE;
              }
            }
            if (nodigits) {
              rsult = FALSE;
            }
            if (rsult) {
              Nlm_StringNCpy (local, pcMmdb, sizeof (local));
              if (sscanf (local, "%ld", &val) == 1) {
                mmdbid = val;
              }
            }
          }

          /* printf("%ld %s %ld %ld\n", mmdbid, pcPdb, (long) count, (long) iMMDBSize-1);  */
          if ((mmdbid == 0) ||  (StringLen(pcPdb) > 4))
            {
              MMDBFini();
              ErrPostEx(SEV_FATAL,0,0, "Internal - LoadMMDBIdx() Bad Index Values");
              return FALSE;
            }
    
          *pI4Mmdb = mmdbid;
          *pU4Pdb =  HashPDBCode(pcPdb);
          pI4Mmdb++;
          pU4Pdb++;
          count++;
	}   
      if (count > (iMMDBSize))
        {
          MMDBFini();
          ErrPostEx(SEV_FATAL,0,0, "Internal - LoadMMDBIdx() Index count is wrong at top of file;");
          return FALSE;
        }
    } while (pcTest);   
  FileClose(f);
  return TRUE;
}

static void RemoveMMDBIdx()
{
 
   if (pI4Mmdbidx) MemFree(pI4Mmdbidx);
   if (pU4Pdbidx) MemFree(pU4Pdbidx);

  return;
}

DocUid  LIBCALL MMDBEvalPDB_fb (CharPtr str)
{
  register Uint4Ptr pU4Pdb = NULL;
  register Int4Ptr pI4Mmdb = NULL;
  register Uint4   iHash = 0;
  long i;
  
/* uses the index to make a lookup */

  ASSERT ((pI4Mmdbidx != NULL) && (pU4Pdbidx != NULL));
  
  pI4Mmdb = pI4Mmdbidx;
  pU4Pdb = pU4Pdbidx;
  iHash = HashPDBCode(str);
  for (i=0; i < iMMDBSize ; i++)
    {
        if (iHash == *pU4Pdb)
	 {
	      return (DocUid) *pI4Mmdb;
	 }
	
	pU4Pdb++;
	pI4Mmdb++;
    } 
 return (DocUid) 0;
}

DocUid  LIBCALL MMDBEvalPDB (CharPtr str)
{
  Char eval[10];
  
  StringNCpy(eval,  str,  4);
  StrUpper(eval);
  eval[4] = '\0';
#ifdef MMDB_PUBSTRUCT
  if(!file_db)
    return  PubStruct_pdb2mmdb( (database[0]?database:NULL), eval);
#endif
  return  MMDBEvalPDB_fb (eval);
  
}

BiostrucPtr LIBCALL
MMDBBiostrucGet (DocUid uid, Int4 mdlLvl, Int4 maxModels)
{
  AsnIoPtr    aip = NULL;
  BiostrucPtr pbs = NULL;
  int         index;
  Int4        acc;

#ifdef MMDB_PUBSTRUCT
  extern WWWInfoPtr 		info;
  
  if(!file_db)
    {
      Int4 state     = 0;
      char *dbserver = (database[0]?database:NULL);

      acc = uid;
      
      if(WWWFindName(info, "ps_acc") >=0)
        aip = PubStruct_readasn(dbserver, acc);
      else
        {
          index = WWWFindName(info, "ps_state");
          if (index >= 0)
            {
              char *      item;
              item = WWWGetValueByIndex(info,index);
              if (!StrToInt4(item,&state))
                ErrPostEx(SEV_ERROR,0,0, "cant convert Pubstruct_state<%s> into integer",item);
            }
          if (state == 0 )
            aip = PubStruct_viewasn(dbserver, uid);
          else
            {
              acc = PubStruct_lookup(dbserver,uid, state);
              /*
                fprintf(stderr,"%s:%d: acc=%d <<- %s(mmdb=%d,state=%d)\n",
                __FILE__,__LINE__,acc,dbserver,uid,state);
                */
              if (acc>0)
                aip = PubStruct_readasn(dbserver, acc);
            }
        }
    }
  else
#endif
    {
      Char path[PATH_MAX];
       
      sprintf(path, "%s%ld.val", database, (long) uid);
#ifdef MMDB_UNIXCOMPRESSED
      {
        FILE *pipe;
        Char compath[PATH_MAX];
         
        sprintf(compath,"%s -c %s.gz ", gunzip, path);
        pipe=popen(compath,"rb");
        if (pipe == 0)
          {
            ErrPostEx(SEV_FATAL,0,0, "MMDBBiostrucGet failed: Can't find gunzip in path.\n");
            return NULL;
          }
        aip = AsnIoNew(ASNIO_BIN_IN, pipe , NULL, NULL, NULL);
      }
#else
      if (FileLength(path) == 0)
        return NULL;
      aip = AsnIoOpen(path, "rb");
#endif
    }
   
  if (!aip)
    return NULL;

  pbs = BiostrucAsnGet(aip, NULL,  mdlLvl,  maxModels);
#ifdef MMDB_PUBSTRUCT
  if(!file_db)
    PubStruct_closeasn (aip,1);
  else
#endif
    AsnIoClose (aip);

  return pbs;
}

Boolean LIBCALL MMDBInit (void)
{
  char *msg;
  char path[PATH_MAX];
  FILE *f;
  
#define ERROR(err_msg) { msg = err_msg; goto errexit; }


#ifdef MMDB_PUBSTRUCT
  GetAppParam("mmdb", "MMDB", "PubStruct", "", path, sizeof(path));
  if (strcmp(path,"yes")==0)
    file_db = 0;
  else
    file_db = 1;

  if (!file_db) /* if database reading */
    {
      GetAppParam("mmdb", "MMDB", "Database", "", database, sizeof(database));
      return TRUE;
    }
#endif

  /* filebased mmdbsrv */
  GetAppParam("mmdb", "MMDB", "Database", "", database, sizeof(database));
  GetAppParam("mmdb", "MMDB", "Index"   , "", indexname, sizeof(indexname));
  if (database[0] == 0)
    ERROR("MMDBInit failed: config file / path to data missing.\n");
  
  if (indexname[0] == 0)
    ERROR("MMDBInit failed: config file / index file missing.\n");
  
  assert(sizeof(path) > strlen(database) + strlen(indexname));
  sprintf(path,"%s%s",database,indexname);

  f = fopen(path,"r");
  if(f)
    fclose(f);
  else
    ERROR("MMDBInit failed: incorrect mmdb.idx path.\n");

#ifdef MMDB_UNIXCOMPRESSED
  GetAppParam("mmdb", "MMDBSRV", "Gunzip", "", gunzip, sizeof(gunzip));
  
  if (gunzip[0] == 0)
    ERROR("MMDBInit failed: gunzip path missing.\n");

  f = fopen(gunzip,"r");
  if(f)
    fclose(f);
  else
    ERROR("MMDBInit failed: incorrect gunzip path .\n");
  
#endif
  
  return LoadMMDBIdx(database,  indexname);
#undef ERROR

errexit:
  ErrPostEx(SEV_FATAL,0,0, msg);
  return FALSE;
}

void LIBCALL MMDBFini (void)
{
  RemoveMMDBIdx();
  return;
}
