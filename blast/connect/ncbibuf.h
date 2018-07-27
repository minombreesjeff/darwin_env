#ifndef NCBIBUF__H
#define NCBIBUF__H

/*  $Id: ncbibuf.h,v 6.5 1999/08/17 22:30:22 vakatov Exp $
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
 * Author:  Denis Vakatov
 *
 * File Description:
 *   Memory-resident FIFO storage area (to be used e.g. in I/O buffering)
 *
 *   This is just a back-compatibility interface("Nlm_*") to the real
 *   BUF API located in "ncbi_buffer.[ch]".
 *   Unlike the "real" BUF API, this API uses:
 *    a) "Nlm_" name prefix for structures, types and functions;
 *    b) "Nlm_*" fixed size integer types like "Nlm_Uint4";
 *    c) "Nlm_Boolean" rather than a native "int" for the boolean type;
 *    d) [MSWIN] "NLM_EXTERN" rather than just "extern" to ease the compilation
 *       for MSWIN DLL.
 *
 * --------------------------------------------------------------------------
 * $Log: ncbibuf.h,v $
 * Revision 6.5  1999/08/17 22:30:22  vakatov
 * Use "Nlm_BUF" to avoid name clash with "BUF" in "ncbi_buffer.h" when
 * compiling with MipsPro IRIX compiler
 *
 * Revision 6.4  1999/08/17 19:47:25  vakatov
 * Moved all real code from NCBIBUF to NCBI_BUFFER;  the code has been cleaned
 * from the NCBI C toolkit specific types and API calls.
 * NCBIBUF module still exists for the backward compatibility -- it
 * provides old NCBI-wise interface.
 *
 * ==========================================================================
 */

#include <ncbilcl.h>
#include <ncbistd.h>


#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define BUF               Nlm_BUF
#define BUF_SetChunkSize  Nlm_BUF_SetChunkSize
#define BUF_Size          Nlm_BUF_Size
#define BUF_Write         Nlm_BUF_Write
#define BUF_Peek          Nlm_BUF_Peek
#define BUF_Read          Nlm_BUF_Read
#define BUF_PushBack      Nlm_BUF_PushBack
#define BUF_Destroy       Nlm_BUF_Destroy


/* handle of a buffer (see "ncbi_buffer.[ch]") */
struct BUF_tag;
typedef struct BUF_tag* BUF;


/* Set minimal size of the buffer memory chunk.
 * Return the actually set chunk size on success;  zero on error
 * NOTE:  if "*pBuf" == NULL then create it
 *        if "chunk_size" is passed 0 then set it to BUF_DEF_CHUNK_SIZE
 */
NLM_EXTERN Nlm_Uint4 BUF_SetChunkSize(BUF* pBuf, Nlm_Uint4 chunk_size);


/* Return the number of bytes stored in "buf".
 * NOTE: return 0 if "buf" == NULL
 */
NLM_EXTERN Nlm_Uint4 BUF_Size(BUF buf);


/* Add new data to the end of "*pBuf" (to be read last).
 * NOTE:  if "*pBuf" == NULL then create it
 */
NLM_EXTERN Nlm_Boolean BUF_Write(BUF* pBuf, const void* data, Nlm_Uint4 size);


/* Write the data to the very beginning of "*pBuf" (to be read first).
 * NOTE:  if "*pBuf" == NULL then create it
 */
NLM_EXTERN Nlm_Boolean BUF_PushBack(BUF* pBuf,
                                    const void* data, Nlm_Uint4 size);


/* Copy up to "size" bytes stored in "buf" to "data".
 * Return the # of copied bytes(can be less than "size").
 * NOTE:  "buf" and "data" can be NULL; in both cases, do nothing
 *        and return 0.
 */
NLM_EXTERN Nlm_Uint4 BUF_Peek(BUF buf, void* data, Nlm_Uint4 size);


/* Copy up to "size" bytes stored in "buf" to "data" and remove
 * copied data from the "buf".
 * Return the # of copied-and/or-removed bytes(can be less than "size")
 * NOTE: if "buf"  == NULL then do nothing and return 0
 *       if "data" == NULL then do not copy data anywhere(still, remove it)
 */
NLM_EXTERN Nlm_Uint4 BUF_Read(BUF buf, void* data, Nlm_Uint4 size);


/* Destroy all internal data;  return NULL
 * NOTE: do nothing if "buf" == NULL
 */
NLM_EXTERN BUF BUF_Destroy(BUF buf);


#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* NCBIBUF__H */
