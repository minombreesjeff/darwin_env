#ifndef __ACCID1_H_
#define __ACCID1_H_

#include <id1arch.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif


#ifdef __cplusplus
extern "C" {
#endif


NLM_EXTERN Boolean     ID1Init(void);
NLM_EXTERN void        ID1Fini(void);

/**** Look Up a Uid from a SeqId using ID1 lookup ****/
NLM_EXTERN Int4        ID1FindSeqId(SeqIdPtr sip);

/**** Look Up the source SeqId given a GI ****************/
NLM_EXTERN SeqIdPtr    ID1SeqIdForGI(Int4 gi);

NLM_EXTERN SeqEntryPtr ID1SeqEntryGet(Int4 uid, Int2 retcode);


/*****************************************************************************
*
*   The Following two functions allow access by BioseqFetch using the
*   SeqMgr.  The application should call ID1BioseqFetchEnable() at the start
*   of the application and ID1BioseqFetchDisable() at the end; This
*   will make ID1BioseqFetch() the "remote" access procedure for the
*   SeqMgr. ID1Init() will only be called on the first fetch unless "now"
*   is true;
*
*   If you add your own fetch function after calling ID1BioseqFetchEnable,
*     it will be called BEFORE ID1BioseqFetchEnable. Add yours after this
*     call, and yours will be call AFTER ID1.
*
*****************************************************************************/
NLM_EXTERN Boolean ID1BioseqFetchEnable  (CharPtr progname, Boolean now);
NLM_EXTERN void    ID1BioseqFetchDisable (void);

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
