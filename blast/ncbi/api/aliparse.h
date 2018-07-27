/*=========================================================================*/
/*                                                                         */
/*  aliparse.h -- Header file for aliparse.c                               */
/*                                                                         */
/*=========================================================================*/

#ifndef _ALIPARSE_
#define _ALIPARSE_

/* Include files */

#include <ncbi.h>
#include <objalign.h>
#include <sqnutils.h>
#include <alignmgr.h>
#include <aliread.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Defined constants */

#define ALI_SET_DEFAULTS       0x0000
#define ALI_SET_GAP_CHAR       0x0001
#define ALI_SET_MISSING_CHAR   0x0002
#define ALI_SET_MAYBES         0x0004
#define ALI_SET_READBUFF       0x0008
#define ALI_SET_NUCL_MIN       0x0010
#define ALI_SET_NUCL_MAX       0x0020
#define ALI_SET_CORRUPT_MAX    0x0040
#define ALI_SET_DEBUG_LEVEL    0x0080
#define ALI_SET_ERRMSG_EXPAND  0x0100
#define ALI_SET_ALL            0x00ff

#define ALI_SHOW_NONE          0x00
#define ALI_SHOW_SEQUENCES     0x01
#define ALI_SHOW_DEFLINES      0x02
#define ALI_SHOW_OTHERS        0x04
#define ALI_SHOW_ALL           0xff

#define ALI_CONTIGUOUS         1
#define ALI_INTERLEAVED        2

#define ALI_ERRMSG_EXPAND_ALL  1
#define ALI_ERRMSG_EXPAND_SOME 2
#define ALI_ERRMSG_EXPAND_NONE 3

#define SEQUENCE_LINE          1
#define DEFINITION_LINE        2
#define OTHER_LINE             3

/* The alignment file when read in is stored in a   */
/* linked list of ValNodes.  These ValNodes contain */
/* one of the following data types depending on the */
/* choice setting.                                  */
/*                                                  */
/*        choice             type of data.ptrvalue  */
/*        ------             ---------------------  */
/*     0 (ALI_UNKNOWN)           NULL               */
/*     1 (ALI_SEQLINE)           SeqLineInfoPtr     */
/*     2 (ALI_DEFLINE)           DefLineInfoPtr     */
/*     3 (ALI_OTHERLINE)         OtherLineInfoPtr   */

#define ALI_UNKNOWN        0
#define ALI_SEQLINE        1
#define ALI_DEFLINE        2
#define ALI_OTHERLINE      3

#define ERR_SEQ_BEFORE_ID         1
#define ERR_ID_WITHOUT_SEQ        2
#define ERR_SEQ_WITHOUT_ID        3
#define ERR_DUPLICATE_IDS         4
#define ERR_SEQUENCE_TOO_SHORT    5
#define ERR_SEQUENCE_TOO_LONG     6
#define ERR_OUT_OF_MEMORY         7
#define ERR_ID_NO_PRECEDING_SEQ   8
#define ERR_DEFLINE_WITH_NO_ID    9
#define ERR_ID_WITH_NO_DEFLINE    10
#define ERR_NOT_INTERLEAVED       11
#define ERR_NOT_CONTIGUOUS        12
#define ERR_NO_SEQUENCES_FOUND    13
#define ERR_ID_COUNT_MISMATCH     14
#define ERR_SEQ_LENGTH_MISMATCH   15
#define ERR_INVALID_DEFLINE       16
#define ERR_DEFLINE_NODEFS        17
#define ERR_GLOBAL_DEFLINE_NODEFS 18
#define ERR_MULTI_DEFLINE_NODEFS  19

#define LEVEL_MULTI    1
#define LEVEL_ERROR    2
#define LEVEL_WARNING  3
#define LEVEL_INFO     4

/* Data structures */

/* Function prototypes */

SeqLineInfoPtr   SeqLineReEval (SeqLineInfoPtr seqLinePtr);
AlignFileDataPtr Ali_Read (FILE PNTR alignFilePtr);
void             Ali_Free (AlignFileDataPtr fileInfoPtr);
ErrInfoPtr       XAli_AddError (ErrInfoPtr PNTR errorList, Int4 iError);
ErrInfoPtr       Ali_AddError (ErrInfoPtr PNTR errorListPtr,
			       Int4            iError,
			       ...);
void             Ali_ChangeRowToOther (ValNodePtr rowPtr);
Boolean          Ali_SetConfig (AliConfigInfoPtr configPtr,
				       Int2             options);
AliConfigInfoPtr Ali_GetConfig (void);

NLM_EXTERN SeqEntryPtr ALI_ConvertToNCBIData(AlignFileDataPtr afp);

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* _ALIPARSE_ */

