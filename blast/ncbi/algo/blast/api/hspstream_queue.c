/*  $Id: hspstream_queue.c,v 1.8 2005/04/12 17:59:08 dondosha Exp $
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
 * Author:  Ilya Dondoshansky
 *
 */

/** @file hspstream_queue.c
 * Implementation of the BlastHSPStream interface for producing BLAST results
 * on the fly.
 */

#ifndef SKIP_DOXYGEN_PROCESSING
static char const rcsid[] = 
    "$Id: hspstream_queue.c,v 1.8 2005/04/12 17:59:08 dondosha Exp $";
#endif /* SKIP_DOXYGEN_PROCESSING */


#include <algo/blast/core/blast_hits.h>
#include <algo/blast/api/hspstream_queue.h>
#include <ncbithr.h>

/** @addtogroup CToolkitAlgoBlast
 *
 * @{
 */

/** Default hit saving stream methods */

/** Deallocate memory for the BlastHSPStream with an HSP list queue data 
 * structure. 
 * @param hsp_stream HSP stream to free [in]
 * @return NULL
 */
static BlastHSPStream* 
BlastHSPListQueueFree(BlastHSPStream* hsp_stream) 
{
   BlastHSPListQueueData* stream_data = 
      (BlastHSPListQueueData*) GetData(hsp_stream);
   ListNode* node;

   NlmSemaDestroy(stream_data->m_resultsSema);
   NlmMutexDestroy(stream_data->m_resultsMutex);

   for (node = stream_data->m_queueStart; node; node = node->next) {
      node->ptr = (void*) Blast_HSPListFree((BlastHSPList*)node->ptr);
   }
   stream_data->m_queueStart = ListNodeFree(stream_data->m_queueStart);
   sfree(stream_data);
   sfree(hsp_stream);
   return NULL;
}

/** Read one HSP list from a queue of HSP lists. If the queue is empty, this 
 * function waits for more results to be written, unless results queue is 
 * already closed for writing.
 * @param hsp_stream HSP list stream to read from [in]
 * @param hsp_list_out The read HSP list. NULL, if there is nothing left 
 *                     in the queue to read.
 * @return Status: success, error or end of reading.
 */
static int 
BlastHSPListQueueRead(BlastHSPStream* hsp_stream, 
                      BlastHSPList** hsp_list_out) 
{
   BlastHSPListQueueData* stream_data = 
      (BlastHSPListQueueData*) GetData(hsp_stream);
   int status = kBlastHSPStream_Error;

   /* Lock the mutex */
   NlmMutexLockEx(&stream_data->m_resultsMutex);

   if (!stream_data->m_writingDone) {
      while (!stream_data->m_writingDone && !stream_data->m_queueStart) {
         /* Decrement the semaphore count to 0, then wait for it to be 
          * incremented. Note that mutex must be locked whenever the 
          * contents of the stream are checked, but it must be unlocked
          * for the semaphore wait. */
         NlmMutexUnlock(stream_data->m_resultsMutex);
         NlmSemaWait(stream_data->m_resultsSema);
         NlmMutexLockEx(&stream_data->m_resultsMutex);
      }
   }

   if (!stream_data->m_queueStart) {
      /* Nothing in the queue, but no more writing to the queue is expected. */
      *hsp_list_out = NULL;
      status =  kBlastHSPStream_Eof;
   } else {
      ListNode* start_node = stream_data->m_queueStart;

      *hsp_list_out = (BlastHSPList*) start_node->ptr;

      stream_data->m_queueStart = start_node->next;
      start_node->next = NULL;
      ListNodeFree(start_node);
      if (!stream_data->m_queueStart)
         stream_data->m_queueEnd = NULL;
      status = kBlastHSPStream_Success;
   }

   NlmMutexUnlock(stream_data->m_resultsMutex);

   return status;
}

/** Write an HSP list to the results queue.
 * @param hsp_stream BlastHSPStream to write to. [in]
 * @param hsp_list Pointer to an HSP list to save in the queue. The HSP stream
 *                 takes ownership of the HSP list and sets the dereferenced
 *                 pointer to NULL [in]
 * @return Status: success or error, if queue is already closed for writing.
 */
static int 
BlastHSPListQueueWrite(BlastHSPStream* hsp_stream, 
                       BlastHSPList** hsp_list)
{
   BlastHSPListQueueData* stream_data = 
      (BlastHSPListQueueData*) GetData(hsp_stream);

   /* If input is Null, don't do anything, but return success */
   if (*hsp_list == NULL)
      return kBlastHSPStream_Success;

    /* If input HSP list is empty, free it and return success */
    if ((*hsp_list)->hspcnt == 0) {
        *hsp_list = Blast_HSPListFree(*hsp_list);
        return kBlastHSPStream_Success;
    }    

   /* If stream is closed for writing, return error */
   if (stream_data->m_writingDone)
      return kBlastHSPStream_Error;

   NlmMutexLockEx(&stream_data->m_resultsMutex);
   stream_data->m_queueEnd = 
      ListNodeAddPointer(&stream_data->m_queueEnd, 0, (void*)(*hsp_list));
   if (!stream_data->m_queueStart)
      stream_data->m_queueStart = stream_data->m_queueEnd;
   /* Free the caller from this pointer's ownership. */
   *hsp_list = NULL;
   /* Increment the semaphore count. */
   NlmSemaPost(stream_data->m_resultsSema);
   NlmMutexUnlock(stream_data->m_resultsMutex);

   return kBlastHSPStream_Success;
}

/** Prohibit any future writing to the HSP list queue. Also increment the 
 * read semaphore, to allow exit out of the wait state in the reading function.
 * @param hsp_stream The BlastHSPStream pointer [in] [out]
 */
static void 
BlastHSPListQueueClose(BlastHSPStream* hsp_stream)
{
   BlastHSPListQueueData* stream_data = 
      (BlastHSPListQueueData*) GetData(hsp_stream);
   NlmMutexLockEx(&stream_data->m_resultsMutex);
   stream_data->m_writingDone = TRUE;
   /* Increment the semaphore count so the reading thread can get out of 
    * the waiting state and check the m_writingDone variable. */
   NlmSemaPost(stream_data->m_resultsSema);
   NlmMutexUnlock(stream_data->m_resultsMutex);
}

/** Set functions pointers and data structure pointer in a new BlastHSPStream 
 * with an HSP list queue data structure.
 * @param hsp_stream The BlastHSPStream to initialize [in] [out]
 * @param args Pointer to the HSP list queue data structure [in]
 */
static BlastHSPStream* 
BlastHSPListQueueNew(BlastHSPStream* hsp_stream, void* args) 
{
    BlastHSPStreamFunctionPointerTypes fnptr;

    fnptr.dtor = &BlastHSPListQueueFree;
    SetMethod(hsp_stream, eDestructor, fnptr);
    fnptr.method = &BlastHSPListQueueRead;
    SetMethod(hsp_stream, eRead, fnptr);
    fnptr.method = &BlastHSPListQueueWrite;
    SetMethod(hsp_stream, eWrite, fnptr);
    fnptr.closeFn = &BlastHSPListQueueClose;
    SetMethod(hsp_stream, eClose, fnptr);

    SetData(hsp_stream, args);
    return hsp_stream;
}

/* Create a new BlastHSPStream with an HSP list queue data structure. */
BlastHSPStream* Blast_HSPListQueueInit()
{
    BlastHSPListQueueData* stream_data = 
       (BlastHSPListQueueData*) calloc(1, sizeof(BlastHSPListQueueData));
    BlastHSPStreamNewInfo info;

    /* At the start of the search there is nothing in the results queue, so
     * initialize the semaphore count with 0. */
    stream_data->m_resultsSema = NlmSemaInit(0);
    info.constructor = &BlastHSPListQueueNew;
    info.ctor_argument = (void*)stream_data;

    return BlastHSPStreamNew(&info);
}

/* @} */

