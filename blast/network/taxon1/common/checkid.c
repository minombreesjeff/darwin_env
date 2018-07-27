/*  checkid.c
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE                          
*               National Center for Biotechnology Information
*                                                                          
*  This software/database is a "United States Government Work" under the   
*  terms of the United States Copyright Act.  It was written as part of    
*  the author's official duties as a United States Government employee and 
*  thus cannot be copyrighted.  This software/database is freely available 
*  to the public for use. The National Library of Medicine and the U.S.    
*  Government have not placed any restriction on its use or reproduction.  
*                                                                          
*  Although all reasonable efforts have been taken to ensure the accuracy  
*  and reliability of the software and data, the NLM and the U.S.          
*  Government do not and cannot warrant the performance or results that    
*  may be obtained by using this software or data. The NLM and the U.S.    
*  Government disclaim all warranties, express or implied, including       
*  warranties of performance, merchantability or fitness for any particular
*  purpose.                                                                
*                                                                          
*  Please cite the author in any work or product based on this material.   
*
* ===========================================================================
*
* File Name:  checkid.c
*
* Author:  Vladimir Soussov
*   
* File Description:  check OrgRef integrity function
*
*
* $Log: checkid.c,v $
* Revision 6.0  1997/08/25 18:41:26  madden
* Revision changed to 6.0
*
* Revision 1.1  1997/04/29 16:03:02  soussov
* Initial revision
*
*
*/

#include <ncbi.h>
#include <taxinc.h>

static Int4 get_tax_id(ValNodePtr dbnode)
{
  DbtagPtr dbtag;
  ObjectIdPtr object_id;

  while(dbnode != NULL) {
      dbtag= dbnode->data.ptrvalue;
      if(StringICmp(dbtag->db, "taxon") == 0) {
	  object_id= dbtag->tag;
	  return object_id->id;
      }
      dbnode= dbnode->next;
  }
  return 0;
}

Uint2 CheckTaxId(OrgRefPtr orp, CharPtr ** msg_out)
{
    Int4 tax_id;
    Taxon1DataPtr tdp;
    CharPtr* msg_arr= NULL;
    CharPtr msg;
    Uint2 n= 0;

    if(orp == NULL) {
	msg= StringSave("WARNING\t empty OrgRef");
	msg_arr= MemNew(sizeof(CharPtr));
	*msg_arr= msg;
	*msg_out= msg_arr;
	return 1;
    }
	
    tax_id= get_tax_id(orp->db);

    if(tax_id > 0) {
	/* check this tax_id */
	tdp= tax1_getbyid(tax_id);
	if(tdp == NULL) {
	    /* taxonomy id was killed */
	    msg= MemNew(80);
	    sprintf(msg, 
		    "ERROR\t taxid %d is killed in taxonomy database but exists in entry",
		    tax_id);
	    msg_arr= MemNew(sizeof(CharPtr));
	    *msg_arr= msg;
	    *msg_out= msg_arr;
	    return 1;
	}

	/* check that tax_id still correct */
	if(orp->taxname == NULL) {
	    msg= StringSave("WARNING\t empty taxname in OrgRef");
	    msg_arr= MemNew(sizeof(CharPtr));
	    *msg_arr= msg;
	    *msg_out= msg_arr;
	    return 1;
	}

	if(StringICmp(orp->taxname, tdp->org->taxname) != 0) {
	    msg= MemNew(256);
	    sprintf(msg,
    "WARNING: taxname in OrgRef <%s> is differ from the name in taxonomy <%s> tax_id=%d",
		    orp->taxname, tdp->org->taxname, tax_id);
	    msg_arr= MemNew(sizeof(CharPtr));
	    *msg_arr= msg;
	    *msg_out= msg_arr;
	    return 1;
	}
	return 0;
    }	    
    
    /* we have no tax_id */
    msg= StringSave("WARNING\t no taxid in OrgRef");
    *msg_out= msg_arr= MemNew(3*sizeof(CharPtr));
    msg_arr[0]= msg;
    n= 1;
    tdp= tax1_lookup(orp, 0);
    if(tdp == NULL) {
	/* no such organism */
	msg= MemNew(256);
	sprintf(msg,"WARNING\t organism not found [scientific name <%s> common name <%s>]",
		(orp->taxname == NULL)? "" : orp->taxname,
		(orp->common == NULL)? "" : orp->common);
	msg_arr[1]= msg;
	return 2;
    }

    tax_id= get_tax_id(tdp->org->db);
    msg= MemNew(80);
    sprintf(msg, "INFO\t OrgRef should be mapped to <%s> tax_id=%d",
	    tdp->org->taxname, tax_id);
    msg_arr[1]= msg;
    n= 2;

    if(orp->taxname == NULL) {
	msg= MemNew(256);
	sprintf(msg,"WARNING\t taxname <%s> does not exist in original OrgRef",
		tdp->org->taxname);
	msg_arr[2]= msg;
	n= 3;
    }
    else if(StringICmp(orp->taxname, tdp->org->taxname) != 0) {
	msg= MemNew(256);
	sprintf(msg,
		"WARNING: taxname in OrgRef <%s> is differ from the name in taxonomy <%s> tax_id=%d",
		    orp->taxname, tdp->org->taxname, tax_id);
	msg_arr[2]= msg;
	n= 3;
    }
	
    return n;
}
