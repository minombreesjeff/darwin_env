/*  $Id: ncbibuf.c,v 6.8 2003/01/08 15:09:07 lavr Exp $
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
 * $Log: ncbibuf.c,v $
 * Revision 6.8  2003/01/08 15:09:07  lavr
 * Fix Nlm_BUF_Destroy() to use BUF_Destroy() w/o return value
 *
 * Revision 6.7  2000/02/25 16:45:54  vakatov
 * Redesigned to really share "ncbi_*.[ch]" etc. between the C and
 * the C++ toolkits, and even to use them in a "standalone" fashion
 *
 * Revision 6.6  1999/10/12 16:40:26  vakatov
 * Deleted all test code as there is one in "test/test_ncbi_buffer.c" now
 *
 * Revision 6.5  1999/08/17 22:30:21  vakatov
 * Use "Nlm_BUF" to avoid name clash with "BUF" in "ncbi_buffer.h" when
 * compiling with MipsPro IRIX compiler
 *
 * Revision 6.4  1999/08/17 19:47:24  vakatov
 * Moved all real code from NCBIBUF to NCBI_BUFFER;  the code has been cleaned
 * from the NCBI C toolkit specific types and API calls.
 * NCBIBUF module still exists for the backward compatibility -- it
 * provides old NCBI-wise interface.
 * ==========================================================================
 */


#include <ncbibuf.h>

/* ...and undefine "Nlm_BUF_*" to allow to access <ncbi_buffer.h> API */
#undef BUF
#undef BUF_SetChunkSize
#undef BUF_Size
#undef BUF_Write
#undef BUF_Peek
#undef BUF_Read
#undef BUF_PushBack
#undef BUF_Destroy

/* this is the only place where both <ncbibuf.h> and <ncbi_buffer.h> can
 * be #include'd in one source module! */
#undef NCBIBUF__H

#include <connect/ncbi_buffer.h>


NLM_EXTERN Nlm_Uint4 Nlm_BUF_SetChunkSize
(BUF* pBuf, Nlm_Uint4 chunk_size)
{
  return (Nlm_Uint4) BUF_SetChunkSize(pBuf, chunk_size);
}

NLM_EXTERN Nlm_Uint4 Nlm_BUF_Size
(BUF buf)
{
  return (Nlm_Uint4) BUF_Size(buf);
}

NLM_EXTERN Nlm_Boolean Nlm_BUF_Write
(BUF* pBuf, const void* data, Nlm_Uint4 size)
{
  return (Nlm_Boolean) BUF_Write(pBuf, data, size);
}

NLM_EXTERN Nlm_Boolean Nlm_BUF_PushBack
(BUF* pBuf, const void* data, Nlm_Uint4 size)
{
  return (Nlm_Boolean) BUF_PushBack(pBuf, data, size);
}


NLM_EXTERN Nlm_Uint4 Nlm_BUF_Peek
(BUF buf, void* data, Nlm_Uint4 size)
{
  return (Nlm_Uint4) BUF_Peek(buf, data, size);
}


NLM_EXTERN Nlm_Uint4 Nlm_BUF_Read
(BUF buf, void* data, Nlm_Uint4 size)
{
   return (Nlm_Uint4) BUF_Read(buf, data, size);
}


NLM_EXTERN BUF Nlm_BUF_Destroy
(BUF buf)
{
  BUF_Destroy(buf);
  return 0;
}
