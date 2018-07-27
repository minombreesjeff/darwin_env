#ifndef SALOGIF__H
#define SALOGIF__H

/*  $RCSfile: salogif.h,v $  $Revision: 6.2 $  $Date: 2003/11/20 18:36:08 $
* ==========================================================================
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
* ==========================================================================
*
* Author:	Jinghui Zhang
*
* File Description:
*	The Blast Search result visualization utilities    
*
* --------------------------------------------------------------------------
* $Log: salogif.h,v $
* Revision 6.2  2003/11/20 18:36:08  dondosha
* Added function PrintOneAlignmentOverview to create an overview gif for one of the multiple query sequences
*
* Revision 6.1  2000/03/20 19:00:28  shavirin
* Initial revision in the new location.
*
* Revision 1.1  1999/04/15 15:20:20  egorov
* Initial commit
*
* Revision 1.1  1998/05/13 20:20:05  egorov
* This revision borns salogif.[hc] files for store functions for visualization results of BLAST search
*
* ==========================================================================
*/

#include <ncbi.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Shows alignments in mouse clickable gif image
 * and generates html page for this 
 */
NLM_EXTERN Boolean PrintAlignmentOverview
(SeqAnnotPtr h_annot,   /* output of blast search */
 FILE*       fp,        /* file where output goes to */
 const Char* formname,  /* it is needed to specify name of embeded HTML form,
                           * create this form before if there is no one */
 const Char* href,      /* hyperlink location of the GIF file */
 const Char* gif_name,  /* GIF file name */
 const Char* title      /* title printed at the top of generated HTML output */
 );

/* Shows alignments gif image for one query 
 * from a set of multiple queries. 
 */
NLM_EXTERN Boolean PrintOneAlignmentOverview
(SeqAnnotPtr h_annot,   /* output of blast search */
 FILE*       fp,        /* file where output goes to */
 const Char* formname,  /* it is needed to specify name of embeded HTML form,
                           * create this form before if there is no one */
 const Char* href,      /* hyperlink location of the GIF file */
 const Char* gif_name,  /* GIF file name */
 const Char* title,     /* title printed at the top of generated HTML output */
 int query_index        /* Index of this query in the set of queries */
 );

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* SALOGIF__H */
