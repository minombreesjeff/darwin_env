/*   tax3api.c
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
* File Name:  tax3api.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   7/8/04
*
* $Revision: 1.13 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <objseq.h>
#include <objsset.h>
#include <tax3api.h>
#include <sqnutils.h>

/* low-level connection functions */

NLM_EXTERN CONN Tax3OpenConnection (
  void
)

{
  return QUERY_OpenServiceQuery ("TaxService3", NULL, 30);
}

#ifdef OS_MAC
#include <Events.h>
#endif

NLM_EXTERN Taxon3ReplyPtr Tax3WaitForReply (
  CONN conn
)

{
  AsnIoConnPtr    aicp;
  time_t          currtime, starttime;
  Int2            max = 0;
  EIO_Status      status;
  STimeout        timeout;
  Taxon3ReplyPtr  t3ry = NULL;
#ifdef OS_MAC
  EventRecord     currEvent;
#endif

  if (conn == NULL) return NULL;

#ifdef OS_MAC
  timeout.sec = 0;
  timeout.usec = 0;
#else
  timeout.sec = 100;
  timeout.usec = 0;
#endif

  starttime = GetSecs ();
  while ((status = CONN_Wait (conn, eIO_Read, &timeout)) == eIO_Timeout && max < 300) {
    currtime = GetSecs ();
    max = currtime - starttime;
#ifdef OS_MAC
    WaitNextEvent (0, &currEvent, 0, NULL);
#endif
  }
  if (status == eIO_Success) {
    aicp = QUERY_AsnIoConnOpen ("rb", conn);
    t3ry = Taxon3ReplyAsnRead (aicp->aip, NULL);
    QUERY_AsnIoConnClose (aicp);
  }
  CONN_Close (conn);

  return t3ry;
}

/* high-level connection functions */

NLM_EXTERN Taxon3ReplyPtr Tax3SynchronousQuery (
  Taxon3RequestPtr t3rq
)

{
  AsnIoConnPtr    aicp;
  CONN            conn;
  Taxon3ReplyPtr  t3ry;

  if (t3rq == NULL) return NULL;

  conn = Tax3OpenConnection ();

  if (conn == NULL) return NULL;

  aicp = QUERY_AsnIoConnOpen ("wb", conn);

  Taxon3RequestAsnWrite (t3rq, aicp->aip, NULL);

  AsnIoFlush (aicp->aip);
  QUERY_AsnIoConnClose (aicp);

  QUERY_SendQuery (conn);

  t3ry = Tax3WaitForReply (conn);

  return t3ry;
}

NLM_EXTERN Boolean Tax3AsynchronousQuery (
  Taxon3RequestPtr t3rq,
  QUEUE* queue,
  QueryResultProc resultproc,
  VoidPtr userdata
)

{
  AsnIoConnPtr  aicp;
  CONN          conn;

  if (t3rq == NULL) return FALSE;

  conn = Tax3OpenConnection ();

  if (conn == NULL) return FALSE;

  aicp = QUERY_AsnIoConnOpen ("wb", conn);

  Taxon3RequestAsnWrite (t3rq, aicp->aip, NULL);

  AsnIoFlush (aicp->aip);
  QUERY_AsnIoConnClose (aicp);

  QUERY_SendQuery (conn);

  QUERY_AddToQueue (queue, conn, resultproc, userdata, TRUE);

  return TRUE;
}

NLM_EXTERN Int4 Tax3CheckQueue (
  QUEUE* queue
)

{
  return QUERY_CheckQueue (queue);
}

NLM_EXTERN Taxon3ReplyPtr Tax3ReadReply (
  CONN conn,
  EIO_Status status
)

{
  AsnIoConnPtr    aicp;
  Taxon3ReplyPtr  t3ry = NULL;

  if (conn != NULL && status == eIO_Success) {
    aicp = QUERY_AsnIoConnOpen ("rb", conn);
    t3ry = Taxon3ReplyAsnRead (aicp->aip, NULL);
    QUERY_AsnIoConnClose (aicp);
  }
  return t3ry;
}

NLM_EXTERN Taxon3RequestPtr CreateTaxon3Request (
  Int4 taxid,
  CharPtr name,
  OrgRefPtr orp
)

{
  Taxon3RequestPtr  t2rp;

  t2rp = Taxon3RequestNew ();
  if (t2rp == NULL) return NULL;

  if (StringDoesHaveText (name)) {
    ValNodeCopyStr (&(t2rp->request), 2, name);
  } else if (taxid > 0) {
    ValNodeAddInt (&(t2rp->request), 1, taxid);
  } else if (orp != NULL) {
    orp = AsnIoMemCopy ((Pointer) orp,
                        (AsnReadFunc) OrgRefAsnRead,
                        (AsnWriteFunc) OrgRefAsnWrite);
    ValNodeAddPointer (&(t2rp->request), 3, (Pointer) orp);
  }

  return t2rp;
}

NLM_EXTERN Taxon3RequestPtr CreateMultiTaxon3Request (ValNodePtr org_list)
{
  ValNodePtr vnp;
  Taxon3RequestPtr t3rp;
  OrgRefPtr orp;
  
  t3rp = Taxon3RequestNew ();
  if (t3rp == NULL) return NULL;

  for (vnp = org_list; vnp != NULL; vnp = vnp->next)
  {
    switch (vnp->choice)
    {
      case 1:
        ValNodeAddInt (&(t3rp->request), 1, vnp->data.intvalue);
        break;
      case 2:
        ValNodeCopyStr (&(t3rp->request), 2, vnp->data.ptrvalue);
        break;
      case 3:
        orp = AsnIoMemCopy (vnp->data.ptrvalue,
                        (AsnReadFunc) OrgRefAsnRead,
                        (AsnWriteFunc) OrgRefAsnWrite);
        ValNodeAddPointer (&(t3rp->request), 3, (Pointer) orp);
        break;
    }
  }
  return t3rp;
}

NLM_EXTERN ValNodePtr Taxon3GetOrgRefList (ValNodePtr org_list)
{
  Taxon3RequestPtr t3rq;
  Taxon3ReplyPtr   t3ry;
  T3DataPtr        tdp;
  OrgRefPtr        t3orp = NULL;
  T3ReplyPtr       trp;
  T3ErrorPtr       tep;
  ValNodePtr       response_list = NULL;

  if (org_list == NULL) return NULL;
  
  t3rq = CreateMultiTaxon3Request (org_list);
  if (t3rq == NULL) return NULL;
  t3ry = Tax3SynchronousQuery (t3rq);
  Taxon3RequestFree (t3rq);
  if (t3ry != NULL) {
    for (trp = t3ry->reply; trp != NULL; trp = trp->next) {
      switch (trp->choice) {
        case T3Reply_error :
          tep = (T3ErrorPtr) trp->data.ptrvalue;
          if (tep != NULL) {
            ErrPostEx (SEV_ERROR, 0, 0, tep->message);
          }
          ValNodeAddPointer (&response_list, 3, NULL);
          break;
        case T3Reply_data :
          tdp = (T3DataPtr) trp->data.ptrvalue;
          if (tdp != NULL) {
            t3orp = (OrgRefPtr)(tdp->org);
            ValNodeAddPointer (&response_list, 3, (Pointer) t3orp);
            tdp->org = NULL;
          }
          break;
        default :
          break;
      }
    }
    Taxon3ReplyFree (t3ry);
  }
  
  return response_list;
}

NLM_EXTERN OrgRefPtr Taxon3GetOrg (OrgRefPtr orp)

{
  Taxon3RequestPtr t3rq;
  Taxon3ReplyPtr   t3ry;
  T3DataPtr        tdp;
  OrgRefPtr        t3orp = NULL;
  T3ReplyPtr        trp;
  T3ErrorPtr        tep;
	
  if (orp == NULL) return NULL;
  
  t3rq = CreateTaxon3Request (0, NULL, orp);
  if (t3rq == NULL) return NULL;
  t3ry = Tax3SynchronousQuery (t3rq);
  Taxon3RequestFree (t3rq);
  if (t3ry != NULL) {
    for (trp = t3ry->reply; trp != NULL; trp = trp->next) {
      switch (trp->choice) {
        case T3Reply_error :
          tep = (T3ErrorPtr) trp->data.ptrvalue;
          if (tep != NULL) {
            ErrPostEx (SEV_ERROR, 0, 0, tep->message);
          }
          break;
        case T3Reply_data :
          tdp = (T3DataPtr) trp->data.ptrvalue;
          if (tdp != NULL) {
            t3orp = (OrgRefPtr)(tdp->org);
            tdp->org = NULL;
          }
          break;
        default :
          break;
      }
    }
    Taxon3ReplyFree (t3ry);
  }
  
  return t3orp;
}

static Boolean DoOrgIdsMatch(BioSourcePtr b1, BioSourcePtr b2)
{
  DbtagPtr d1 = NULL, d2 = NULL;
  ValNodePtr vnp;
	
  if (b1 == NULL || b2 == NULL) 
  {
    return FALSE;
  }
  if (b1->org ==  NULL || b2->org == NULL) 
  {
    return FALSE;
  }
  for (vnp = b1->org->db; vnp; vnp = vnp->next) 
  {
    d1 = (DbtagPtr) vnp->data.ptrvalue;
    if (StringCmp(d1->db, "taxon") == 0) 
    {
      break;
    }
  }
  for (vnp = b2->org->db; vnp; vnp = vnp->next) 
  {
    d2 = (DbtagPtr) vnp->data.ptrvalue;
	if (StringCmp(d2->db, "taxon") == 0) 
	{
      break;
	}
  }
  if (d1 && d2) 
  {
	if (d1->tag->id == d2->tag->id) 
	{
      return TRUE;
	}
  }
  else if (StringICmp(b1->org->taxname, b2->org->taxname) == 0) 
  {
	return TRUE;
  }
  return FALSE;
}

static BioSourcePtr Tax3BioSourceMerge(BioSourcePtr host, BioSourcePtr guest)
{
  SubSourcePtr ssp, sp, last_ssp;
  OrgModPtr omp, homp, last_omp;
  OrgNamePtr	onp;
	
  if (host == NULL && guest == NULL) 
  {
    return NULL;
  }
  if (host == NULL && guest != NULL) 
  {
	host = AsnIoMemCopy(guest, (AsnReadFunc) BioSourceAsnRead, 
		   						(AsnWriteFunc) BioSourceAsnWrite);
	return host;
  }
  if (host != NULL && guest == NULL) 
  {
    return host;
  }
  if (host->genome == 0 && guest->genome != 0) 
  {
    host->genome = guest->genome;
  }
  if (host->origin == 0 && guest->origin != 0) 
  {
    host->origin = guest->origin;
  }
  last_ssp = host->subtype;
  while (last_ssp != NULL && last_ssp->next != NULL)
  {
  	last_ssp = last_ssp->next;
  }
  for (ssp = guest->subtype; ssp; ssp = ssp->next) 
  {
    sp = AsnIoMemCopy(ssp, (AsnReadFunc) SubSourceAsnRead, 
		   						(AsnWriteFunc) SubSourceAsnWrite);
    if (last_ssp == NULL)
    {
      host->subtype = sp;
    }
    else
    {
      last_ssp->next = sp;
      last_ssp = sp;
    }
  }
  if (guest->org->orgname) 
  {
   	if ((onp = host->org->orgname)	== NULL) 
   	{
   	  onp = OrgNameNew();
   	  host->org->orgname = onp;
    }	
    last_omp = onp->mod;		
    while (last_omp != NULL && last_omp->next != NULL)
    {
      last_omp = last_omp->next;
    }
    for (omp = guest->org->orgname->mod; omp; omp = omp->next) 
    {
      homp = AsnIoMemCopy(omp, (AsnReadFunc) OrgModAsnRead, 
		   						(AsnWriteFunc) OrgModAsnWrite);
      if (last_omp == NULL)
      {
      	onp->mod = homp;
      }
      else
      {
      	last_omp->next = homp;
      	last_omp = homp;
      }
    }
  }
  return host;
}


/**************************************************************************
*	Compare BioSources in one bioseq->descr using Taxonomy to find
*	their join parent
*	merge if organisms are the same or create a feature if different
*
**************************************************************************/
NLM_EXTERN void Tax3MergeSourceDescr (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	BioseqPtr    bsp = NULL;
	ValNodePtr   vnp, newlist;
	SeqFeatPtr   sfp;
	BioSourcePtr first_biop = NULL;
	BioSourcePtr other_biop;
	BioSourcePtr tmp_biop;
	ObjValNodePtr ovp;

	if (!IS_Bioseq(sep)) {
		return;
	}
	newlist = (ValNodePtr) data;
	bsp = (BioseqPtr) sep->data.ptrvalue;
	if ((bsp->repr != Seq_repr_raw) && (bsp->repr != Seq_repr_const) 
			&& (bsp->repr != Seq_repr_delta))
		return;

	if (! ISA_na(bsp->mol))
		return;
	
	/* add the descriptors in newlist to the end of the list in bsp->descr*/
	if (bsp->descr == NULL)
	{
	  bsp->descr = newlist;
	}
	else
	{
	  for (vnp = bsp->descr; vnp->next != NULL; vnp = vnp->next)
	  {	
	  }
	  vnp->next = newlist;
	}
	
	/* now find the first source descriptor in bsp->descr that has an org*/
    /* note - we can't use SeqMgrGetNextDescriptor here because we have just
     * added to the descriptors, so they are not indexed. */
	for (vnp = bsp->descr; vnp != NULL; vnp = vnp->next)
	{
	  if (vnp->choice != Seq_descr_source) continue;
	  if (vnp->data.ptrvalue == NULL)
	  {
	  	ErrPostStr(SEV_WARNING, 0, 0, "Source descriptor missing data");
	  	if (vnp->extended)
	  	{
	  	  ovp = (ObjValNodePtr) vnp;
	  	  ovp->idx.deleteme = TRUE;
	  	}
	  }
	  if (first_biop == NULL)
	  {
	  	first_biop = vnp->data.ptrvalue;
	  }
	  else
	  {
		other_biop = vnp->data.ptrvalue;
		/* detach biosource pointer from descr, so that it will not be freed
		 * when the descriptor is deleted.
		 */
		vnp->data.ptrvalue = NULL;
        if (vnp->extended)
        {
          ovp = (ObjValNodePtr) vnp;
	  	  ovp->idx.deleteme = TRUE;
        }
        if (DoOrgIdsMatch(first_biop, other_biop)) 
		{
		  /* merge the two sources */
		  tmp_biop = Tax3BioSourceMerge(first_biop, other_biop);
		  if (tmp_biop == NULL)
		  {
		  	ErrPostStr (SEV_WARNING, 0, 0, "Failed to merge biosources");
		  }
		  else
		  {
		  	first_biop = tmp_biop;
		  }
		  other_biop = BioSourceFree (other_biop);
		} else {
		  /* create a source feature */
		  sfp = CreateNewFeatureOnBioseq (bsp, SEQFEAT_BIOSRC, NULL);
		  if (sfp != NULL)
		  {
            sfp->data.value.ptrvalue = other_biop;
		  }
        }
	  }
	}
	return;
}

static Int4 GetTaxIdFromOrgRef (OrgRefPtr orp)
{
  Int4       tax_id = -1;
  ValNodePtr vnp;
  DbtagPtr   d;

  if (orp != NULL)
  {
    for (vnp = orp->db; vnp != NULL; vnp = vnp->next) 
    {
      d = (DbtagPtr) vnp->data.ptrvalue;
      if (StringCmp(d->db, "taxon") == 0) 
      {
        tax_id = d->tag->id;
        break;
      }
    }
  }
  return tax_id;
}

NLM_EXTERN Int4 Taxon3GetTaxIdByOrgRef (OrgRefPtr orp)
{
  OrgRefPtr  orp_repl;
  Int4       tax_id = -1;
  
  if (orp == NULL) return -1;
  
  orp_repl = Taxon3GetOrg (orp);
  tax_id = GetTaxIdFromOrgRef (orp_repl);
  OrgRefFree (orp_repl);
  
  return tax_id;
}

NLM_EXTERN OrgRefPtr Taxon3GetOrgRefByName (CharPtr orgname)
{
  OrgRefPtr request, org;
  
  request = OrgRefNew ();
  if (request == NULL) return NULL;
  request->taxname = orgname;
  org = Taxon3GetOrg (request);
  request->taxname = NULL;
  OrgRefFree (request);
  return org;
}

NLM_EXTERN Int4 Taxon3GetTaxIdByName (CharPtr orgname)
{
  OrgRefPtr orp;
  Int4      tax_id;
  
  orp = Taxon3GetOrgRefByName (orgname);
  tax_id = GetTaxIdFromOrgRef (orp);

  OrgRefFree(orp);
  return tax_id;
}

static void AddBioSourceToList (BioSourcePtr biop, Pointer userdata)
{
  ValNodePtr PNTR list;
  
  if (biop == NULL || userdata == NULL) return;
  list = (ValNodePtr PNTR) userdata;
  ValNodeAddPointer (list, 4, (Pointer) biop);
}

NLM_EXTERN void Taxon3ReplaceOrgInSeqEntry (SeqEntryPtr sep, Boolean keep_syn)
{
  ValNodePtr   biop_list = NULL;
  ValNodePtr   request_list = NULL;
  ValNodePtr   response_list = NULL;
  ValNodePtr   biop_vnp, response_vnp;
  BioSourcePtr biop;
  OrgRefPtr    swap_org, response_org;
  
  VisitBioSourcesInSep (sep, &biop_list, AddBioSourceToList);

  for (biop_vnp = biop_list; biop_vnp != NULL; biop_vnp = biop_vnp->next)
  {
    biop = (BioSourcePtr) biop_vnp->data.ptrvalue;
    ValNodeAddPointer (&request_list, 3, biop->org);
  }
  response_list = Taxon3GetOrgRefList (request_list);
 
  if (ValNodeLen (response_list) != ValNodeLen (request_list))
  {
    Message (MSG_POST, "Unable to retrieve information from tax server");
    return;
  }

  for (biop_vnp = biop_list, response_vnp = response_list;
       biop_vnp != NULL && response_vnp != NULL;
       biop_vnp = biop_vnp->next, response_vnp = response_vnp->next)
  {
    biop = (BioSourcePtr) biop_vnp->data.ptrvalue;
    swap_org = biop->org;
    response_org = response_vnp->data.ptrvalue;
    if (response_org != NULL)
    {
      biop->org = response_org;
      response_vnp->data.ptrvalue = NULL;
      OrgRefFree (swap_org);
      if (! keep_syn)
      {
        biop->org->syn = ValNodeFreeData(biop->org->syn);
      }
    }
  }
  ValNodeFree (request_list);
  ValNodeFree (response_list);
  ValNodeFree (biop_list);   
}
