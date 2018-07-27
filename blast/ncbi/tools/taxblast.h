/* $Id: taxblast.h,v 6.3 2002/10/07 19:52:07 camacho Exp $
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
* File Name:  $RCSfile: taxblast.h,v $
*
* Authors:  Scott Federhen, Sergei Shavirin
*
* Initial Version Creation Date: 04/04/2000
*
* $Revision: 6.3 $
*
* File Description:
*         Header file for Tax-Blast program
*
* $Log: taxblast.h,v $
* Revision 6.3  2002/10/07 19:52:07  camacho
* Added RDTaxLookupReset
*
* Revision 6.2  2000/12/08 22:30:57  shavirin
* Added set of functions for creation of Taxonomy lookup database using
* formatdb API.
*
* Revision 6.1  2000/05/17 15:54:39  shavirin
* Initial revision in new location.
*
* Revision 1.2  2000/04/26 19:54:03  shavirin
* Added parameter "show_gi" optionaly show gis in seqid in organism list.
*
* Revision 1.1  2000/04/20 14:01:53  shavirin
* Initial revision
*
*
* ==========================================================================
*/

#ifndef TAXBLAST__H
#define TAXBLAST__H

#include <ncbi.h>
#include <readdb.h>

#ifdef __cplusplus
extern "C" {
#endif

    void TXBHtmlReport(SeqAlignPtr sap,   /* output of blast search */
                       FILE *outfile,     /* file where output goes to */ 
                       Boolean query_is_na,  /* If this is DNA sequence? */
                       Boolean db_is_na,     /* If database is DNA? */
                       CharPtr database, /* Database used in BLAST search */
                       CharPtr link_href, /* Link to the regular BLAST results */
                       CharPtr window_name, /* Window name for output */
                       Boolean show_gi); /* Show gis in the organism report */

    Boolean FDBTaxCallback (RDBTaxLookupPtr tax_lookup, Int4 tax_id);
    RDBTaxLookupPtr RDTaxLookupInit(void);
    RDBTaxLookupPtr RDTaxLookupReset(RDBTaxLookupPtr tax_lookup);
    void RDTaxLookupClose(RDBTaxLookupPtr tax_lookup);

#ifdef __cplusplus
}
#endif

#endif /*TAXBLAST__H */
