#ifndef _TAXUTIL_H_
#define _TAXUTIL_H_ taxutil

/*  $Id: taxutil.h,v 6.4 2000/01/12 19:14:19 vakatov Exp $
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
 * Author:  Vladimir Soussov
 *
 * File Description:
 *
 *
 * ---------------------------------------------------------------------------
 * $Log: taxutil.h,v $
 * Revision 6.4  2000/01/12 19:14:19  vakatov
 * Get rid of the "new" arg name (C++ keyword)
 * Extended extern "C" {} section to include "typedef"s
 * Added NCBI copyright header
 *
 * ===========================================================================
 */

#include <ncbi.h>
#include <asn.h>
#include <sequtil.h>
#include <taxinc.h>

#define MAXIDLIST 50  

#ifdef __cplusplus
extern "C" {
#endif

typedef struct taxonomy_block {
	Int4  			hits;
	Taxon1DataPtr	tax;
} TaxBlk, PNTR TaxBlkPtr;

typedef struct struct_GeneticCodeList {
	Int4   genomic;
	Int4   mitochondrial;
} GeneticCodeList, PNTR GeneticCodeListPtr;


void tax_init(void);
Int4 taxname_replace(CharPtr iname, Taxon1DataPtr tdp1);
Int4 taxname_match(CharPtr orgname, Boolean err);
OrgRefPtr check_org_ref(OrgRefPtr orp, Boolean replace);
OrgRefPtr get_tax_org(CharPtr name);
CharPtr get_lineage(CharPtr name);
GeneticCodeListPtr get_gcode(CharPtr name);
GeneticCodeListPtr get_gcode_from_lineage(CharPtr name);
OrgRefPtr replace_org(OrgRefPtr orp, Boolean replace);
OrgRefPtr replace_org_err(OrgRefPtr orp, Boolean replace);
CharPtr get_tax_division(OrgRefPtr orp);
CharPtr get_embl_code(OrgRefPtr orp);
OrgRefPtr check_org_ref(OrgRefPtr orp, Boolean replace);
void GetTaxserverOrg(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
Boolean CheckTaxId(SeqEntryPtr sep);

/* temporary simulant functions for old-new Taxon switch period */
Boolean TaxArchInit(void);
Boolean TaxArchFini(void);
void TaxMergeBSinDescr(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* _TAXUTIL_H_ */
