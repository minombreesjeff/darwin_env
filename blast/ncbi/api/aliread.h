#ifndef _ALI_READ_INCLUDED
#define _ALI_READ_INCLUDED

#include <ncbi.h>

/* Defined constants */

#define ALI_NUCLEOTIDE        1
#define ALI_PROTEIN           2
#define ALI_AMBIGUOUS         3

/* Data structures */

typedef struct 
{
  Int4    rownum;
  CharPtr data;
} RowPart, PNTR RowPartPtr;

typedef struct
{
  Int4    rowNum;
  CharPtr sequence;
  Int2    type;         /* Nucleotide = 1, Protein = 2, Ambiguous = 3 */
  CharPtr id;
  CharPtr junk;
  Boolean maybe;
  Int4    firstWordLen;
} SeqLineInfo, PNTR SeqLineInfoPtr;

typedef struct
{
  Int4    rowNum;
  CharPtr definitions;
  CharPtr id;
} DefLineInfo, PNTR DefLineInfoPtr;

typedef struct
{
  Int4    rowNum;
  CharPtr other;
  CharPtr id;
} OtherLineInfo, PNTR OtherLineInfoPtr;

typedef struct _ErrInfo
{
  Int2       errNum;
  Int2       level;
  Int4       rowNum;
  CharPtr    info;
  CharPtr    extraInfo;
  struct _ErrInfo PNTR next;
} ErrInfo, PNTR ErrInfoPtr;

typedef struct
{
  Int2    dataType;
  Int2    contigOrInter;
  Int4    idCount;
  Int4    seqLength;
} DataInfo, PNTR DataInfoPtr;

typedef struct
{
  CharPtr  gapChar;
  CharPtr  missingChar;
  CharPtr  unalignedChar;
  Boolean  useMaybes;
  Int4     readBuffSize;
  Int2     debugLevel;
  Int2     corruptSeqThreshold;
  Int2     nuclLineMinThreshold;
  Int2     nuclLineMaxThreshold;
  Int2     errExpandLevel;
  DataInfo declaredInfo;
} AliConfigInfo, PNTR AliConfigInfoPtr;

typedef struct
{
  CharPtr missingChar;
  CharPtr gapChar;
  CharPtr unalignedChar;
  Int2    dataType;	 /* ALI_DATA_NUCLEOTIDE,ALI_DATA_PROTEIN,ALI_UNKNOWN */
  Int2    contigOrInter; /* ALI_INTERLEAVED, ALI_CONTIGUOUS, ALI_UNKNOWN */
} ParsedInfo, PNTR ParsedInfoPtr;

typedef struct _SeqPart
{
  CharPtr  sequence;
  struct _SeqPart PNTR next;
} SeqPart, PNTR SeqPartPtr;

typedef struct _IdInfo
{
  CharPtr        id;
  Int4           length;
  SeqPartPtr     sequence;
  CharPtr        defline;
  struct _IdInfo PNTR next;
} IdInfo, PNTR IdInfoPtr;

typedef struct
{
  IdInfoPtr     sequences;
  IdInfoPtr     maybes;
  ErrInfoPtr    errors;
  ParsedInfoPtr info;
} AlignFileData, PNTR AlignFileDataPtr;

/* Function prototypes */

Int2       Ali_SeqLineGetType(CharPtr          seqStr,
			      AliConfigInfoPtr configPtr);
void       Ali_ChangeRowToOther (ValNodePtr rowPtr);
ValNodePtr Ali_ReadLines (FILE PNTR        alignFilePtr,
			  ErrInfoPtr PNTR  errorListPtr,
			  AliConfigInfoPtr configPtr,
			  AlignFileDataPtr fileInfoPtr);
#endif
