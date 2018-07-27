/*  $Id: mmdb_pubstruct.c,v 6.4 2001/05/25 01:42:10 kimelman Exp $
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
* File Name:  $Id: mmdb_pubstruct.c,v 6.4 2001/05/25 01:42:10 kimelman Exp $
*
* File Description: Defines MMDB access using sybase retrival.
*                   
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: mmdb_pubstruct.c,v $
* Revision 6.4  2001/05/25 01:42:10  kimelman
* mmdb2livemmdb
*
* Revision 6.3  1999/12/01 23:27:26  kimelman
* added guards to avoid repeatable connect/disconnect
*
* Revision 6.2  1999/08/02 19:50:51  kimelman
* keep connection alive foor mmdbsrv & vastsrv sessions
*
* Revision 6.1  1999/05/17 21:29:20  kimelman
* extracted version of pubstruct specific implementations of MMDBBiostrucGET & Co..
*
*
* ==========================================================================
*/

/*********************************************************************************/
/* This file abstracts the calls to Network Entrez that were previously          */
/* embedded in mmdbapi1.c  Linking with this file forces sysbase retrival        */
/********   NOTE - .mmdbrc (UNIX) mmdb.ini (Win) mmdb.cnf (Mac) required   *******/
/* containing paths to database and index file name:                         ****** 
 *
 * [MMDB]
 * PubStruct  = PUBSEQ_OS:PubStruct=anyone:allowed
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


static int  pubstruct_init=0;
static Char pubstruct[PATH_MAX];
static ps_handle_t db = NULL;

/**************************************************************
 *  PUBLIC INTERFACE
 **************************************************************/
Boolean  LIBCALL
MMDB2liveMMDB(DocUid *mmdb,Int4Ptr live,CharPtr pdb)
{
  Boolean b;
  if (!db && pubstruct[0]==0) {
    strcpy(pubstruct,"BACH:PubStruct=anyone,allowed");
  }
  MMDBInit();
  b = PubStruct_mmdb2livemmdb(db,*mmdb,mmdb,live,pdb);
  MMDBFini();
  return b;
}

DocUid  LIBCALL
MMDBEvalPDB (CharPtr str)
{
  Char eval[10];
  DocUid id=0;
  
  StringNCpy(eval,  str,  4);
  eval[4] = '\0';
  StrUpper(eval);
  { /* let's be paranoid */
    int i,is_mmdb=1;
    for(i=0;i<4;i++)if(eval[i]>='A') { is_mmdb=0; break; }
    if(is_mmdb)
      id=atol(eval);
  }
  MMDBInit();
  id = PubStruct_pdb2mmdb1(db, eval);
  MMDBFini();
  return  id;
}

BiostrucPtr LIBCALL
MMDBBiostrucGet (DocUid uid, Int4 mdlLvl, Int4 maxModels)
{
  extern WWWInfoPtr 	info; /* mmdbsrv:info */

  AsnIoPtr    aip = NULL;
  BiostrucPtr pbs = NULL;
  int         index;
  char       *item;
  Int4        sat_key   = 0;
  Int4        state     = 0;
  /*   char       *dbserver = (pubstruct[0]?pubstruct:NULL); */

  MMDBInit();

  /*
   * THE following block should be defined ONLY for server which show up internal blob revisions in DB
   */
#ifdef INTERNAL_SERVER
  /* printf("MMDBBiostrucGet_pubstruct(%s,%d)\n",dbserver,uid); */
  if(WWWFindName(info, "sat_key") >=0) 
    {
      index = WWWFindName(info, "sat_key");
      item  = WWWGetValueByIndex(info,index);
      sat_key=atol(item);
      if (sat_key<=0) {
        ErrPostEx(SEV_ERROR,0,0, "cant convert Pubstruct..Struct.acc (sat_key) <%s> into integer",item);
        return NULL;
      }
    }
  if (WWWFindName(info, "state"))
    {
      index = WWWFindName(info, "state");
      item = WWWGetValueByIndex(info,index);
      state=-1;
      state=atol(item);
      if (state<0) {
        ErrPostEx(SEV_ERROR,0,0, "cant convert Pubstruct..Struct.state<%s> into integer",item);
        return NULL;
      }
    }
#endif
  
  if(state>0 && sat_key==0) {
    sat_key = PubStruct_lookup1(db,uid, state);
  }
  if(sat_key>0)
    aip = PubStruct_readasn1(db, sat_key);
  else if (state==0)
    aip = PubStruct_viewasn1(db, uid);

  if(aip)
    {
      pbs = BiostrucAsnGet(aip, NULL,  mdlLvl,  maxModels);
      PubStruct_closeasn (aip,1);
    }
  MMDBFini();
  return pbs;
}

Boolean LIBCALL
MMDBInit (void)
{
  pubstruct_init++;
  if(db)
    return TRUE;
  if(pubstruct_init==1)
    {
      GetAppParam("mmdb", "MMDB", "PubStruct", "", pubstruct, sizeof(pubstruct));
      if (pubstruct[0]==0) {
        strcpy(pubstruct,"PUBSEQ_OS:PubStruct=anyone,allowed");
      }
    }
  db = PubStruct_connect(pubstruct);
  if(!db)
    {
      ErrPostEx(SEV_ERROR,0,0, "cant connect to Pubstruct (%s)",pubstruct);
      return FALSE;
    }
  return TRUE;
}

void LIBCALL
MMDBFini (void)
{
  pubstruct_init--;
  if(pubstruct_init>0)
    return;
  if(db)
    {
      PubStruct_disconnect(db);
      db = NULL;
    }
  return;
}

CharPtr LIBCALL
MMDB_configuration(void)
{
  return "Version:\t$Id: mmdb_pubstruct.c,v 6.4 2001/05/25 01:42:10 kimelman Exp $\nConfiguration: PubStruct" ;
}
