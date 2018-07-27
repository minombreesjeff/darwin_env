/*   accutils.h
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
* RCS $Id: accutils.h,v 6.8 1999/04/01 13:56:35 sicotte Exp $
*
* Author:  J. Epstein
*
* Version Creation Date:   10/18/93
*
* File Description: 
*       Utilities which make use of the Entrez "data access library"
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 05-16-94 Schuler     Added RCS Log directive.  Comments will henceforth
*                      be inserted automatically into the source at the
*                      time it is checked into the system.

$Log: accutils.h,v $
Revision 6.8  1999/04/01 13:56:35  sicotte
Moved WHICH_db_accession,IS_ntdb_accession,IS_protdb_accession to
  sequtil.ch
Removed old static functions of Colombe (that are public in salutil.c)
The only code left in accutils.c is the Entrez Access code.

Revision 6.7  1999/03/18 20:24:06  sicotte
changed the define name for NC_ accession

Revision 6.6  1999/03/18 20:18:18  sicotte
added REFSEQ accession numbers and macro ACCN_IS_GENBANK()

Revision 6.5  1999/03/18 15:34:04  sicotte
Updated Accession List and added protein Accessions
for functions IS_ntdb_accession and IS_protdb_accession.
New function WHICH_db_accession with return code allowing
to figure out the molecule type and the database from macros
in accutils.h

Revision 6.4  1999/02/24 16:48:10  kans
added IS_ntdb_accession and IS_protdb_accession, removed NormalizeSeqAlignId

Revision 6.3  1997/11/14 22:13:52  vakatov
[WIN32,DLL]  Added NLM_EXTERN's

Revision 6.2  1997/09/12 15:31:01  chappey
changes in NormalizeSeqAlign

Revision 6.1  1997/09/04 14:14:52  chappey
changes in NormalizeSeqAlign

Revision 6.0  1997/08/25 18:12:38  madden
Revision changed to 6.0

Revision 5.8  1997/08/07 16:02:43  kans
added NormalizeSeqAlignId (Colombe)

Revision 5.7  1997/06/26 21:55:19  vakatov
[PC] DLL'd "ncbicdr.lib", "ncbiacc.lib", "ncbinacc.lib" and "ncbicacc.lib"

Revision 5.6  1997/02/04 18:57:58  epstein
add GetFullEntrezTermList() function

 * Revision 5.5  1996/09/23  21:23:02  shavirin
 * Added include files <tofasta.h> and <lsqfetch.h> for
 * AccessionToFasta() function
 *
 * Revision 5.4  1996/08/14  17:08:29  brandon
 * *** empty log message ***
 *
 * Revision 5.2  1996/08/14  15:15:05  brandon
 * added date parameter to tleval functions
 *
 * Revision 5.1  1996/07/01  14:06:32  epstein
 * add 'join function' EntrezCommonHierAncestor()
 *
 * Revision 5.0  1996/05/28  13:55:34  ostell
 * Set to revision 5.0
 *
 * Revision 4.0  1995/07/26  13:50:32  ostell
 * force revision to 4.0
 *
 * Revision 1.4  1995/05/16  14:12:46  schuler
 * Automatic comment insertion enabled
 *



* ==========================================================================
*/

#ifndef _ACCUTILS_
#define _ACCUTILS_

#ifndef _ACCENTR_
#include <accentr.h>
#endif

#include <tofasta.h>
#include <lsqfetch.h>

/* 4/1/99 Temporately include sequtil.h to avoid breaking code that
   uses IS_ntdb_accession
   */
#include <sequtil.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif




NLM_EXTERN CharPtr LIBCALL EntrezFieldToString PROTO((DocType db, DocField fld));
NLM_EXTERN DocField LIBCALL EntrezStringToField PROTO((DocType db, CharPtr str));

/* Evaluate or parse a text expression describing a boolean query */

/* In the event of a lexical or parsing error, begin and end describe the     */
/* range wherein an error was detected ... this range is then a candidate for */
/* visible highlighting                                                       */

/* If fld == -1, then use "[*]" field mechanism (union over all fields) by    */
/* default                                                                    */

NLM_EXTERN LinkSetPtr LIBCALL EntrezPMTLEvalString PROTO((CharPtr str, DocType db, DocField fld, Int2Ptr begin, Int2Ptr end, void * edc));
NLM_EXTERN ByteStorePtr LIBCALL EntrezPMTLEvalXString PROTO((CharPtr str, DocType db, DocField fld, Int2Ptr begin, Int2Ptr end, void * edc));
NLM_EXTERN Int4 LIBCALL EntrezPMTLEvalCountString PROTO((CharPtr str, DocType db, DocField fld, Int2Ptr begin, Int2Ptr end, void * edc));
NLM_EXTERN Boolean LIBCALL EntrezPMTLParseString PROTO((CharPtr str, DocType db, DocField fld, Int2Ptr begin, Int2Ptr end, void *edc));

NLM_EXTERN LinkSetPtr LIBCALL EntrezTLEvalString PROTO((CharPtr str, DocType db, DocField fld, Int2Ptr begin, Int2Ptr end));
NLM_EXTERN ByteStorePtr LIBCALL EntrezTLEvalXString PROTO((CharPtr str, DocType db, DocField fld, Int2Ptr begin, Int2Ptr end));
NLM_EXTERN Int4 LIBCALL EntrezTLEvalCountString PROTO((CharPtr str, DocType db, DocField fld, Int2Ptr begin, Int2Ptr end));
NLM_EXTERN Boolean LIBCALL EntrezTLParseString PROTO((CharPtr str, DocType db, DocField fld, Int2Ptr begin, Int2Ptr end));

NLM_EXTERN CharPtr LIBCALL EntrezCommonHierAncestor PROTO((EntrezHierarchyPtr ehp1, EntrezHierarchyPtr ehp2, Int2Ptr distance1, Int2Ptr distance2));

NLM_EXTERN CharPtr        * LIBCALL GetFullEntrezTermList PROTO((DocType database, DocField field, Int4Ptr count));


#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif
