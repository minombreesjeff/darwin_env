/*   $Id: PubStructAsn.h,v 6.7 2001/05/25 01:42:10 kimelman Exp $
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
 * Author:  Michael Kimelman
 *
 * File Description: PubStruct DB Asn (down)loader.
 * All functions can wotk with SQL server directly. Path of them, marked
 * 'production' can work throuth OpenServer. - only retrieval stuff
 *                   
 * Modifications:  
 * --------------------------------------------------------------------------
 * $Log: PubStructAsn.h,v $
 * Revision 6.7  2001/05/25 01:42:10  kimelman
 * mmdb2livemmdb
 *
 * Revision 6.6  1999/08/02 19:50:51  kimelman
 * keep connection alive foor mmdbsrv & vastsrv sessions
 *
 * Revision 6.5  1999/05/11 23:37:15  kimelman
 * throw away 'extern info' -- it's mmdbsrv specifics
 *
 * Revision 6.4  1998/05/14 16:11:14  kimelman
 * Compression stuff added in debug mode.
 * few bugs fixed, related to OpenServer/SQL Server switching
 *
 * Revision 6.3  1998/05/08 03:03:44  kimelman
 * Open Server fix
 *
 * Revision 6.2  1998/04/15 14:53:56  kimelman
 * 1. Make retrieval unifirm from open server and sql server.
 * 2. mmdbsrv retrival performance tuning:
 * 	- size of read-in buffers
 * 	- direct pdb2mmdb DB lookup instead of full loading pdb to mmdb translataion
 * 	  table
 * 3. cleaning mmdblocl.* and make.mmdbsrv in order to remove obsolete code
 *
 * Revision 6.1  1998/04/03 20:25:17  kimelman
 * PubStruct access code added to mmdbsrv
 *
 *
 * ==========================================================================
 */

#ifndef __PUBSTRUCTASN_H__ 
#define __PUBSTRUCTASN_H__

#include <ncbi.h>
#include <asn.h>

typedef struct pubstruct_t *ps_handle_t ;

NLM_EXTERN ps_handle_t PubStruct_connect   (char *server);
NLM_EXTERN void        PubStruct_disconnect(ps_handle_t handle);

/**
 * PubStruct_lookup transforms mmdb and state into accession number (return value)
 * the meaning of state is as follows.
 * state = 0 : Production data
 * state = 1 : data for entrez indexing. After indexing it becomes 0
 * state > 1 : intermediate stages of asn assembling. "up to user"
 * state < 0 : prohibited value.
 *
 */
NLM_EXTERN Int4     LIBCALL PubStruct_lookup(char       *server,Int4 mmdb,int state);
NLM_EXTERN Int4     LIBCALL PubStruct_lookup1(ps_handle_t server,Int4 mmdb,int state);

/** (production)
 * PubStruct_pdb2mmdb makes a lookup from pdb to current mmdb
 */

NLM_EXTERN Int4     LIBCALL PubStruct_pdb2mmdb(char *server,CharPtr pcPdb);
NLM_EXTERN Int4     LIBCALL PubStruct_pdb2mmdb1(ps_handle_t server,CharPtr pcPdb);
NLM_EXTERN Boolean  LIBCALL PubStruct_mmdb2livemmdb(ps_handle_t server,Int4 ommdb,Int4Ptr newmmdbid,Int4Ptr live,CharPtr pdb);

/**
 * PubStruct_newasn opens AsnIo stream to created new entry in the database. When
 * this stream is closed, database table's fields will be populated by data,
 * extracted from written asn. state is the only data - which is absent in asn.
 * (beside DB accession )
 * accp argument is optional - side effect return of new accession number
 */
NLM_EXTERN AsnIoPtr LIBCALL PubStruct_newasn     (char *server,int state,
                                                  Int4 *accp);

/**
 * PubStruct_readasn opens AsnIo stream for reading asn found by accession number
 */
NLM_EXTERN AsnIoPtr LIBCALL PubStruct_readasn    (char       *server,Int4 acc);
NLM_EXTERN AsnIoPtr LIBCALL PubStruct_readasn1   (ps_handle_t server,Int4 acc);

/** (production)
 * PubStruct_viewasn opens AsnIo stream for reading indexed structure found by mmdbID
 * 
 */
NLM_EXTERN AsnIoPtr LIBCALL PubStruct_viewasn    (char       *server,Int4 mmdbID);
NLM_EXTERN AsnIoPtr LIBCALL PubStruct_viewasn1   (ps_handle_t server,Int4 mmdbID);

/**
 * PubStruct_updateasn opens AsnIo stream for updating existing asn. asn identified
 * by accession number. after updating the 'state' of the data become 'newstate'. 
 */
NLM_EXTERN AsnIoPtr LIBCALL PubStruct_updateasn  (char *server,Int4 acc,
                                                  int newstate);

/** (production)
 * PubStruct_closeasn closes AsnIO stream, which was open by some of functions
 * above and does some "termination procedure" which determined by thw function,
 * which opens connection.
 */
NLM_EXTERN int      LIBCALL PubStruct_closeasn   (AsnIoPtr aip,int commit);

/**
 * PubStruct_closeasn suppress given asn. 
 */
NLM_EXTERN int      LIBCALL PubStruct_removeasn  (char *server,Int4 acc);

/**
 * File reading wrappers. ( a bit optimized )
 *
 * PubStruct_load read given asn from file stream and put it in database
 * (using ..._newasn) returns accession number if everything ok. or 0 in
 * case of fail
 */

int PubStruct_load(FILE *infile, int state_out, char *server);

/**
 * PubStruct_download read given asn from DB and dump it to file stream
 */
int PubStruct_download(char *server, Int4 acc, Int4 mmdb, FILE *outfile);

#endif
