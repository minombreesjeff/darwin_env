/*  txcommon.h
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
* File Name:  txcommon.h
*
* Author:  Vladimir Soussov
*   
* File Description:  Common data types & prototypes for taxonomy server/client
*
*
* $Log: txcommon.h,v $
* Revision 1.5  2001/09/18 16:55:38  soussov
* switching to the new ASN
*
* Revision 1.4  1999/12/20 17:05:16  soussov
* taxid4gi added
*
* Revision 1.3  1999/01/28 15:43:11  soussov
* rename the formal argument
*
* Revision 1.2  1998/06/12 15:16:07  soussov
* kill some warnings
*
* Revision 1.1  1998/02/10 20:12:05  soussov
* taxon2 related soft
*
* Revision 1.2  1997/05/12 18:31:44  soussov
* 05/12/97
*
 * Revision 1.1.1.1  1997/04/30  21:29:57  soussov
 * initial tree for taxon2
 *
*
*/

#ifndef TXCOMMON_H_DONE
#define TXCOMMON_H_DONE

#include <ncbi.h>

#define TAX_DIV_TXT 0
#define TAX_DIV_CDE 1
#define TAX_DIV_COM 2

#define TAX_GC_NM     0
#define TAX_GC_ABBREV 1
#define TAX_GC_CDE    2
#define TAX_GC_STARTS 3

#define TAX_NAME_SEARCH  0
#define TAX_RE_SEARCH    1
#define TAX_TOKEN_SEARCH 2

#define INHERIT_DIV 0x1
#define INHERIT_GC  0x2
#define INHERIT_MGC 0x4
#define GB_HIDDEN   0x8
#define TAX_HIDDEN  0x10
#define TAX_REFERENCED 0x20

#define TXC_INH_DIV 0x4000000
#define TXC_INH_GC  0x8000000
#define TXC_INH_MGC 0x10000000
/* the following flags are the same (it is not bug) */
#define TXC_SUFFIX  0x20000000
#define TXC_UPDATED 0x20000000
#define TXC_UNCULTURED 0x20000000

#define TXC_GBHIDE  0x40000000
#define TXC_STHIDE  0x80000000

#define TXCL_DEL_NAME 1
#define TXCL_ADD_NAME 2
#define TXCL_ADD_NODE 3
#define TXCL_UPD_NODE 4
#define TXCL_DEL_NODE 5
#define TXCL_DEL_SUBTREE 6
#define TXCL_MOVE_SUBTREE 7
#define TXCL_MERGE_NODES 8
#define TXCL_MOVE_CHILDREN 9
#define TXCL_CIT_LINK 10
#define TXCL_CIT_UNLINK 11
#define TXCL_CIT_DELETE 12
#define TXCL_SS_ADD 13
#define TXCL_SS_CHECK 14
#define TXCL_SS_DELETE 15

#define WWW_NAME_NOT_FOUND   -100
#define WWW_NODE_NOT_FOUND   -110
#define WWW_MULT_NODES_BEGIN -200
#define WWW_MULT_NODES_END   -210
#define WWW_TREE_BEGIN       -300
#define WWW_LEVEL_BEGIN      -320
#define WWW_LEVEL_END        -330
#define WWW_TREE_END         -390
#define WWW_LIN_BEGIN        -400
#define WWW_LIN_END          -490
#define WWW_INFO_BEGIN       -500
#define WWW_INFO_END         -590
#define WWW_NAMES_BEGIN      -600
#define WWW_NAMES_END        -690
#define WWW_DIVISION         -1000
#define WWW_RANK             -1010
#define WWW_GC               -1020
#define WWW_MGC              -1030
#define WWW_NUCLEOTIDE       -1040
#define WWW_PROTEIN          -1050
#define WWW_NO_SEQUENCES     -2000

#define WWWTAX_NAME_SEARCH  0x1
#define WWWTAX_REXP_SEARCH  0x2
#define WWWTAX_TOKEN_SEARCH 0x4
#define WWW_SHOW_ALL        0x8
#define WWW_SHOW_NUCLEOTIDE 0x10
#define WWW_SHOW_PROTEIN    0x20
#define WWW_SHOW_SEQ        0x30

typedef struct t_TaxNamePublic {
    Int4 tax_id;
    CharPtr name_txt;
    CharPtr unique_name;
    Uint1 class_cde;
    Uint1 designator;
} TaxName, PNTR TaxNamePtr;

/* taxonomy node */

typedef struct t_TXC_NodeData {
    Int4       tax_id;
    Uint4      crt_date;
    Uint4      upd_date;
    Uint4      nucleotides, proteins;
    Int1       rank_id;
    Int1       gc_id;
    Int1       mgc_id;
    Int1       div_id;
    Char       embl_cde[4];
    Uint2      hist;
    Uint2      flags;
} TXC_NodeData, PNTR TXC_NodeDataPtr;

typedef struct t_TXC_TreeNode {
    Int4  tax_id;
    Uint4 flags;
    char  node_label[2];
} PNTR TXC_TreeNodePtr;

typedef struct t_nameClass {
    Int2 priority;
    char class_txt[34];
} TaxNameClass, PNTR TaxNameClassPtr;

typedef struct t_rank {
    char rank_txt[64];
} TaxRank, PNTR TaxRankPtr;

typedef struct t_division {
    char div_cde[4];
    char div_txt[64];
} TaxDivision, PNTR TaxDivisionPtr;

typedef struct t_genCode {
    char gc_name[128];
} TaxGenCode, PNTR TaxGenCodePtr;

typedef struct t_ssRec {
    Int4  r_id;
    char  sname[120];
    char  rname[120];
    Uint1 stype, rtype;
} TaxSSRec, PNTR TaxSSRecPtr;

CharPtr tax_prntTime(Int4 t);
Int4 tax_getBaseTime(void);
Int4 tax_getTime(void);

Int4    tax_findByName(CharPtr sname, int mode, TaxNamePtr* res_name);
Int4 tax_getIdByName(CharPtr org_name, CharPtr qualif, Int1 q_type);
Int4    tax_getDesignator(CharPtr name);
Int4    tax_uniqueName(CharPtr s_name, Int4 tax_id);
Boolean tax_setDesignator(CharPtr name, Int4 tax_id);

Int4    tax_issueTaxId(void);

Boolean tax_addName(Int4 tax_id, CharPtr name, CharPtr uname, Int2 name_class, Boolean designat);
Int4    tax_getOrgNames(Int4 tax_id, TaxNamePtr* org_names);
CharPtr tax_getTaxName(Int4 tax_id, CharPtr* unique_name);
Boolean tax_delName(Int4 tax_id, CharPtr dname);
Boolean tax_delAllNodeNames(Int4 tax_id);

Boolean tax_addNameClass(Int4 class_cde, CharPtr class_txt, Int4 priority);
CharPtr tax_getNameClass(Int4 class_cde, Int4Ptr priority);
Int4    tax_getClass_cde(CharPtr class_txt);
Boolean tax_dumpNameClasses(void (*dmpFunc)(VoidPtr, Int2, Int2, CharPtr), VoidPtr usrData);

Boolean tax_addRank(Int2 rank_id, CharPtr rank_txt);
CharPtr tax_getRank(Int2 rank_id);
Int2    tax_getRankId(CharPtr rank_txt);
Boolean tax_dumpRanks(void (*dmpFunc)(VoidPtr, Int2, CharPtr), VoidPtr usrData);

Boolean tax_addDivision(Int4 div_id, CharPtr div_cde, CharPtr div_txt);
Boolean tax_getDivision(Int2 div_id, CharPtr* div_cde, CharPtr* div_txt);
Int2    tax_getDivisionId(CharPtr div_cde, CharPtr div_txt);
Boolean tax_dumpDivisions(void (*dmpFunc)(VoidPtr, Int2, CharPtr, CharPtr), VoidPtr usrData);

Boolean tax_addGC(Int2 gc_id, CharPtr gc_name);
CharPtr tax_getGCName(Int2 gc_id);
Int2    tax_getGCId(CharPtr gc_txt);
Boolean tax_dumpGCs(void (*dmpFunc)(VoidPtr, Int2, CharPtr), VoidPtr usrData);

Boolean tax_matchName(CharPtr orgName, CharPtr str, Int4 mode);
Int4 tax_getTaxId4GI(Int4 gi);

#endif
