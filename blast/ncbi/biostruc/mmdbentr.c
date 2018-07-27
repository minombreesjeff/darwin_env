/*   mmdbentr.c
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
* File Name:  mmdbentr.c
*
* Author:  Christopher Hogue
*
* Version Creation Date:  14 Jan 1997  
*
* $Revision: 6.3 $
*
* File Description: Used to provide Biostrucs data using
* Conventional Entrez subsystems (Network or CDRom) 
*                   
* Modifications:  
* --------------------------------------------------------------------------
*
* $Log: mmdbentr.c,v $
* Revision 6.3  2002/06/11 19:13:46  kans
* StrucSynchronousQuery returns BiostrucSeqPtr instead of BiostrucPtr
*
* Revision 6.2  2001/01/26 15:06:39  lewisg
* use entrez2 to retrieve structures
*
* Revision 6.1  1999/04/22 01:59:18  kimelman
* MMDB_configuration added
*
* Revision 6.0  1997/08/25 18:11:23  madden
* Revision changed to 6.0
*
* Revision 1.3  1997/01/15 17:52:30  hogue
* Initial Version
*
*
* ==========================================================================
*/

/* This file abstracts the calls to Network Entrez that were previously     */
/* embedded in mmdbapi1.c                                                   */
/* Use this to make a network MMDB client using network/CDRom Entrez        */
/* compile this into ncbimmdb.a and link with Network or CDEntrez libraries */

#include <ncbi.h>
#include <mmdbapi.h>
#include <mmdbdata.h>
#include <ent2api.h>
#include <strucapi.h>


Boolean LIBCALL MMDBInit (void)
{
   /*
   Boolean bIsNetwork = FALSE;
   return EntrezInit("MMDBAPI client", FALSE, &bIsNetwork);
   */
   return TRUE;
}


void LIBCALL MMDBFini (void)
{
   /*
   EntrezFini();
   return;
   */
}


BiostrucPtr LIBCALL MMDBBiostrucGet (DocUid uid, Int4 mdlLvl, Int4 maxModels)
{
   BiostrucSeqPtr  bsqp;

/* MMDB - Caching would check here for matching file first */

   /*
   return EntrezBiostrucGet(uid,  mdlLvl, maxModels);
   */
   bsqp = StrucSynchronousQuery (uid);
   if (bsqp == NULL) return NULL;
   return bsqp->structure;

/* Caching would also save file here */

}


DocUid LIBCALL MMDBEvalPDB(CharPtr str)
{
  Entrez2BooleanReplyPtr  e2br;
  Entrez2IdListPtr        e2id;
  Entrez2RequestPtr       e2rq;
  Entrez2ReplyPtr         e2ry;
  Char                    tmp [61];
  Uint4                   uid = 0;

  if (str == NULL) return 0;

  StringNCpy_0 (tmp, str, sizeof (tmp) - 10);
  if (StringStr (tmp, "[ACCN]") == NULL) {
    StringCat (tmp, " [ACCN]");
  }

  e2rq = EntrezCreateBooleanRequest (TRUE, FALSE, "Structure", tmp,
                                     0, 0, NULL, 1, 0);
  if (e2rq == NULL) return 0;
  e2ry = EntrezSynchronousQuery (e2rq);
  e2rq = Entrez2RequestFree (e2rq);
  if (e2ry == NULL) return 0;
  e2br = EntrezExtractBooleanReply (e2ry);
  if (e2br == NULL) return 0;

  if (e2br->count > 0) {
    e2id = e2br->uids;
    if (e2id != NULL && e2id->num > 0 && e2id->uids != NULL) {
      BSSeek (e2id->uids, 0, SEEK_SET);
      uid = Nlm_BSGetUint4 (e2id->uids);
    }
  }

  Entrez2BooleanReplyFree (e2br);

  return uid;
   /*
   LinkSetPtr plsLink = NULL;
   DocUid duUID = 0;
 
   if ((!str)) return (DocUid) 0;
   plsLink = EntrezTLEvalString(str, (DocType) TYP_ST,  
			  (DocField) FLD_ACCN,  NULL, NULL);  
  
   if (plsLink != NULL && plsLink->num > 0 && plsLink->uids != NULL)
	{
	   duUID = plsLink->uids[0];
        }
   LinkSetFree(plsLink); 

   return duUID;
   */
}

CharPtr  LIBCALL MMDB_configuration(void)
{
  return
    "Version:\t$Id: mmdbentr.c,v 6.3 2002/06/11 19:13:46 kans Exp $\nConfiguration:"
    " Entrez"
    "\n";
}
