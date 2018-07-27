/*  txclient.h
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
* File Name:  txclient.h
*
* Author:  Vladimir Soussov
*   
* File Description:  Data types for taxonomy client
*
*
* $Log: txclient.h,v $
* Revision 1.6  1998/07/23 18:27:01  soussov
* one prototype added
*
* Revision 1.5  1998/06/12 15:14:26  soussov
* kill some warnings
*
* Revision 1.4  1998/04/01 17:34:00  soussov
* changed tp include <>
*
* Revision 1.3  1998/03/31 23:12:57  kans
* minor changes needed for code warrior
*
* Revision 1.2  1998/02/11 15:52:31  soussov
* ctpublic.h has been removed
*
* Revision 1.1  1998/02/10 20:12:04  soussov
* taxon2 related soft
*
* Revision 1.2  1997/05/12 18:31:31  soussov
* 05/12/97
*
 * Revision 1.1.1.1  1997/04/30  21:29:42  soussov
 * initial tree for taxon2
 *
*
*/

#ifndef TXCLIENT_H_DONE
#define TXCLIENT_H_DONE

#include <ncbi.h>
#include <txcommon.h>
#include <treemgr.h>
#ifdef TAXSERVICE
#include <objfeat.h>
#endif

typedef struct t_TXC_Name {
    Int4       tax_id;
    Uint1      class_cde;
    Uint1      designator;
    Uint1      name_length;
    Uint1      flags;
    char       name_txt[2];
} TXC_Name, PNTR TXC_NamePtr;

/* to map tax_id to node_id use tax_node_id map index */

typedef struct t_TXC_CitList {
    Int4    cit_id;
    CharPtr cit_key;
} TXC_CitList, PNTR TXC_CitListPtr;

typedef struct t_citation {
  Int4    id;
  CharPtr key;
  Int4    flags;
  Int4    mu_id;
  Int4    pm_id;
  CharPtr url;
  CharPtr txt;
} _citation;

typedef struct t_OrgMod {
    struct t_OrgMod* next;
    CharPtr subname;
    Uint1   subtype;
} tax_OrgMod, PNTR tax_OrgModPtr; 

typedef struct t_subspec {
    struct t_subspec* next;
    Int4    r_id;
    CharPtr sname;
    CharPtr rname;
    Uint1   stype, rtype;
    
} _subspec, PNTR _subspecPtr;

/* client connection library prototypes */
#if 0
Boolean        txc_setSubtree(Int4 tax_id);
Int4           txc_findByName(CharPtr sname, Uint1 mode, TaxNamePtr* found_names);
Int4           txc_getOrgNames(Int4 tax_id, TaxNamePtr* found_names);
Int4           txc_getDesignator(CharPtr name);
Int4           txc_uniqueName(CharPtr s_name);
Boolean        txc_setDesignator(CharPtr name, Int4 tax_id);
Boolean        txc_addName(Int4 tax_id, CharPtr name, CharPtr uname, Int2 class, Boolean designat);
Boolean        txc_delName(Int4 tax_id, CharPtr dname);

TaxNodeDataPtr txc_getNodeRecord(Int4 tax_id);
Boolean        txc_delNode(Int4 tax_id);
Boolean        txc_delSubtree(Int4 tax_id);
Boolean        txc_moveNode(Int4 new_parent, Int4 tax_id);
Boolean        txc_moveChildren(Int4 new_parent, Int4 tax_id);
Boolean        txc_mergeNodes(Int4 tax_id1, Int4 tax_id2);
Int4           txc_splitNode(Int4 tax_id);
Int4           txc_addNode(Int4 parent_id, CharPtr name, TaxNodeDataPtr node_data);
Boolean        txc_updateNode(Int4 tax_id, TaxNodeDataPtr node_data);
Boolean        txc_save(void);
#endif

Boolean        txc_connect2Server(CharPtr srv_name, CharPtr usr, CharPtr passwd, CharPtr applic);

Boolean        txc_load(Int4 root, Int2 levels);
TreeNodeId     tax_getNodeId(Int4 tax_id);
Boolean        txc_loadNameClasses(void);
Boolean        txc_loadRanks(void);
Boolean        txc_loadDivisions(void);
Boolean        txc_loadGCs(void);
TXC_NodeDataPtr txc_getOrgData(Int4 tax_id);
TXC_TreeNodePtr* txc_getLineage(Int4 lin_id, Int4Ptr lin_len);
TXC_TreeNodePtr* txc_getChildren(Int4 node_id, Int4Ptr nof_children);


TreePtr tax_ptree_new(void);
Boolean tax_ptree_addNode(TreePtr ptree, Int4 tax_id);
Boolean tax_ptree_toTaxId(TreeCursorPtr cursor, Int4 tax_id, Boolean search_in_subtree);
Boolean tax_ptree_addChildren(TreeCursorPtr cursor);
Boolean tax_ptree_addSubtree(TreeCursorPtr cursor);


TXC_CitListPtr txc_citGet4Node(Int4 tax_id, Int4Ptr nof_cit);
CharPtr txc_citGetKey(Int4 cit_id);
TXC_CitListPtr txc_citFind(CharPtr key_str, Int4Ptr nof_cit);
_citation* txc_citGetByKey(CharPtr key_str);
Boolean txc_citUpdate(_citation* cit);
Int4 txc_citInsert(_citation* cit);
void txc_close(void);

Boolean wwwtax_getByName(CharPtr sname, int mode_in, int levels_in, 
		      void (*callBack)(int, int, CharPtr));
Boolean wwwtax_getById(Int4 tax_id, int mode_in, int levels_in, 
		      void (*callBack)(int, int, CharPtr));
Boolean te_saveEditWindows(void);

#ifdef TAXSERVICE
Int4 txc_getTaxIdByOrgRef(OrgRefPtr orgRef);
#endif

_subspecPtr tax_SSgetAll(Int4 tax_id);
_subspecPtr tax_SSget(Int4 tax_id, _subspecPtr ssrec);
tax_OrgModPtr tax_SSgetLegal(Int4 tax_id);
Int4 tax_SSgetNodes(Uint1 stype, CharPtr sname, Uint1 mode, Int4Ptr* ids);

#endif
