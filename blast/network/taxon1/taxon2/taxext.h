/*  taxext.h
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
* File Name:  taxext.h
*
* Author:  Vladimir Soussov
*   
* File Description:  Header file for extendent taxonomy API
*
*
* $Log: taxext.h,v $
* Revision 1.4  2001/09/28 15:53:15  soussov
* tax1e_maxTaxId() added
*
* Revision 1.3  1999/03/26 19:18:46  soussov
* fixed bug in tax1e_needUpdate
*
* Revision 1.2  1999/03/25 21:14:58  soussov
* tax1e_needUpdate added
*
* Revision 1.1  1998/02/10 20:12:00  soussov
* taxon2 related soft
*
*
*/

#ifndef TAXEXT_H_DONE
#define TAXEXT_H_DONE

#include <ncbi.h>
#include <taxinc.h>
#include <treemgr.h>

/*-------------------------------------
Get pointer to partial taxonomy tree
You can call this function after tax1_init()
The partial taxonomy tree will exists until you call tax1_fini()
*/
TreePtr tax1e_getTaxTreePtr();

/*-------------------------------------
Invoke the taxonomy node from the database and add it to
taxonomy partial tree. This function invokes the whole path
from the root to given node and merges this path with existing
partial tree. 
Returns TRUE if node was added to the tree
*/
Boolean tax1e_invokeNode(Int4 tax_id);

/*-------------------------------------
Invoke the children of given node and add all of them to
taxonomy partial tree. (It works like tax1e_invokeNode for
all children but more efficient
*/
Boolean tax1e_invokeChildren(Int4 tax_id);

/*-------------------------------------
Move cursor to taxonomy node
*/
Boolean tax1e_toNode(TreeCursorPtr cursor, Int4 tax_id);

/*-------------------------------------
Get tax_id
*/
Int4 tax1e_getTaxId(TreeCursorPtr cursor);

/*-------------------------------------
Get taxname (scientific name)
This function makes a copy of tax_name, you have to free this memory
*/
CharPtr tax1e_getTaxName(TreeCursorPtr cursor);


/*-------------------------------------
  Check if OrgRef needs to be updated
  Returns: 
  -2 - Taxonomy server internal error
  -1 - Organism is not in taxonomy database
   0 - OrgRef is up to date
   1 - wrong tax_id
   2 - wrong genetic code
   3 - wrong mitogen code
   4 - inp_orgRef has empty OrgName
   10 - wrong taxname
   20 - wrong lineage
   30 - wrong common name
   40 - wrong OrgName
   50 - wrong division
   100 - wrong OrgMod
  
*/
Int4 tax1e_needUpdate(OrgRefPtr inp_orgRef);

/*-------------------------------------
  get max taxid
*/
Int4 tax1e_maxTaxId();

#endif
