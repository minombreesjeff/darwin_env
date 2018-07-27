/*   ncbiurl.h
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
* File Name:  ncbiurl.h
*
* Author:  Jonathan Kans
*
* Version Creation Date:   8/12/98
*
* $Revision: 6.5 $
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

#ifndef _NCBIURL_
#define _NCBIURL_

#include <ncbi.h>
#include <ncbisock.h>


#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
*
*   SOCK_SendURLQuery is passed the host machine (without http://), the port
*     (80 is the conventional public port), and a path.  The path can include
*     the full query (including a question mark to separate path from query),
*     or a separate query string (without a question mark) can be passed in.
*     An additional arguments parameter can also be passed in, and an ampersand
*     is used to separate the query and arguments parameters.
*
*   SOCK_CheckURLQuery returns the number of pending URL queries.  Call this
*     function at least once every few seconds.  This function polls the sockets,
*     and calls the designated result procedure (written by the user) to handle
*     the resulting data file.
*
*****************************************************************************/

typedef Nlm_Boolean (LIBCALLBACK *URLResultProc) (Nlm_CharPtr filename, Nlm_CharPtr format, Nlm_VoidPtr mydata);

NLM_EXTERN Nlm_Boolean SOCK_SendURLQuery (Nlm_CharPtr host_machine, Nlm_Uint2 host_port,
                                      Nlm_CharPtr host_path, Nlm_CharPtr query,
                                      Nlm_CharPtr arguments, Nlm_CharPtr data_file,
                                      URLResultProc resultproc, Nlm_Uint4 timeoutsec,
                                      Nlm_Boolean sendBinary, Nlm_Boolean receiveBinary,
                                      Nlm_CharPtr programName, Nlm_VoidPtr mydata);

NLM_EXTERN Nlm_Int4 SOCK_CheckURLQuery (void);

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* _NCBIURL_ */
