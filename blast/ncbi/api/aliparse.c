/*=========================================================================*/
/*                                                                         */
/*  aliparse.c                                                             */
/*                                                                         */
/*=========================================================================*/

#include <stdarg.h>

#include <aliparse.h>
#include <aliread.h>

/* Defined constants */

#define ALI_USE_MAYBES                FALSE  /* Default values for    */
#define ALI_READ_BUFFSIZE             80     /* configuration options */
#define ALI_GAP_CHAR                  '-'    /*          |            */
#define ALI_MISSING_CHAR              '?'    /*          |            */
#define ALI_CORRUPT_SEQ_THRESHOLD     95     /*          |            */
#define ALI_NUCL_LINE_MAX_THRESHOLD   75     /*          |            */
#define ALI_NUCL_LINE_MIN_THRESHOLD   25     /*          V            */

/* Data structures */

typedef struct
{
  DataInfo    foundInfo;
  IdInfoPtr   currentId;
  IdInfoPtr   currentDeflineId;
  ValNodePtr  lastRow;
  Boolean     hasFullLength;
  Boolean     isFirstGroup;
  Boolean     isFirstId;
  Boolean     maybesFound;
  SeqPartPtr  lastSeqPart;
  Boolean     gotAllIds;
  Int4        idCount;
  Int4        currentIdCount;
} PatternInfo, PNTR PatternInfoPtr;

/* Filewide static variables */

static AliConfigInfo s_configInfo;
static Boolean       s_configurationSet = FALSE;

/* Function prototypes */

static void      s_FreeErrorList (ErrInfoPtr errorList);
static void      s_FreeSequenceList (SeqPartPtr seqPtr);
static void      s_FreeIdList (IdInfoPtr idList);
static void      s_FreeRowList (ValNodePtr rowList);
static void      s_FreeRowList_Safe (ValNodePtr rowList);
static void      s_DisplayRowList (ValNodePtr rowList,
				   Int2 mask);
static CharPtr   s_GetRowIdString (ValNodePtr row);
static CharPtr   s_GetRowSeqString (ValNodePtr row);
static IdInfoPtr s_ProcessMaybes (ValNodePtr rowList);
static int       s_SegCompare(const void *i,
			      const void *j);
static Boolean   s_IsInterleaved (ValNodePtr rowList,
				  Int2 PNTR idCount);
static Boolean   s_ProcessInterId (CharPtr          newIdStr,
				   PatternInfoPtr   pattern,
				   AlignFileDataPtr fileInfoPtr,
				   Boolean          isMaybe);
static Boolean   s_ProcessInterSeq (CharPtr          newSeqStr,
				    PatternInfoPtr   pattern,
				    AlignFileDataPtr fileInfoPtr,
				    Boolean          isMaybe);
static Boolean   s_AnalyzeInterleaved (ValNodePtr       rowList,
				       AlignFileDataPtr fileInfoPtr,
				       Int2             idCount);
static Boolean   s_ProcessContigId (CharPtr          newIdStr,
				    PatternInfoPtr   pattern,
				    AlignFileDataPtr fileInfoPtr);
static Boolean   s_ProcessContigSeq (CharPtr          newSeqStr,
				     PatternInfoPtr   pattern,
				     AlignFileDataPtr fileInfoPtr);
static Boolean   s_AnalyzeContiguous (ValNodePtr       rowList,
				      AlignFileDataPtr fileInfoPtr);
static Boolean   s_AnalyzeContents (ValNodePtr       rowList,
				    AlignFileDataPtr fileInfoPtr);
static void      s_SortErrors (AlignFileDataPtr fileInfoPtr);
static void      s_AnalyzeErrors (AlignFileDataPtr fileInfoPtr);
static Boolean   s_CheckContext (ValNodePtr       rowList,
				 AlignFileDataPtr fileInfoPtr);


/*=========================================================================*/
/*                                                                         */
/* Ali_GetConfig () -- Get the current configuration settings.             */
/*                                                                         */
/*=========================================================================*/

AliConfigInfoPtr Ali_GetConfig (void)
{

  AliConfigInfoPtr configPtr;

  /* If configuration hasn't been set yet, */
  /* then set it to the defaults.          */

  if (s_configurationSet == FALSE)
    {
      s_configInfo.useMaybes                  = ALI_USE_MAYBES;
      s_configInfo.readBuffSize               = ALI_READ_BUFFSIZE;
      s_configInfo.debugLevel                 = ALI_SHOW_NONE;
      s_configInfo.corruptSeqThreshold        = ALI_CORRUPT_SEQ_THRESHOLD;
      s_configInfo.nuclLineMinThreshold       = ALI_NUCL_LINE_MIN_THRESHOLD;
      s_configInfo.nuclLineMaxThreshold       = ALI_NUCL_LINE_MAX_THRESHOLD;
      s_configInfo.errExpandLevel             = ALI_ERRMSG_EXPAND_SOME;
      s_configInfo.declaredInfo.dataType      = ALI_UNKNOWN;
      s_configInfo.declaredInfo.contigOrInter = ALI_UNKNOWN;
      s_configInfo.declaredInfo.idCount       = 0;
      s_configInfo.declaredInfo.seqLength     = 0;

      s_configInfo.gapChar = (CharPtr) MemNew (32);
      sprintf (s_configInfo.gapChar    , "%c%c", ALI_GAP_CHAR, '.');
      s_configInfo.missingChar = (CharPtr) MemNew (32);
      sprintf (s_configInfo.missingChar, "%c", ALI_MISSING_CHAR);

      s_configurationSet = TRUE;
    }

  /* Copy the current settings to the return struct */

  configPtr = (AliConfigInfoPtr) MemNew (sizeof (AliConfigInfo));
  MemSet (configPtr, 0, sizeof (AliConfigInfo));

  configPtr->useMaybes            = s_configInfo.useMaybes;
  configPtr->readBuffSize         = s_configInfo.readBuffSize;
  configPtr->debugLevel           = s_configInfo.debugLevel;
  configPtr->corruptSeqThreshold  = s_configInfo.corruptSeqThreshold;
  configPtr->nuclLineMinThreshold = s_configInfo.nuclLineMinThreshold;
  configPtr->nuclLineMaxThreshold = s_configInfo.nuclLineMaxThreshold;
  configPtr->errExpandLevel       = s_configInfo.errExpandLevel;

  configPtr->gapChar = (CharPtr) MemNew (32);
  StringCpy (configPtr->gapChar, s_configInfo.gapChar);
  configPtr->missingChar = (CharPtr) MemNew (32);
  StringCpy (configPtr->missingChar, s_configInfo.missingChar);

  /* Return successfully */

  return configPtr;
}

/*=========================================================================*/
/*                                                                         */
/*  Ali_SetConfig () - Sets various runtime configuration options used by  */
/*                     the Ali_Read () function.                           */
/*                                                                         */
/* configPtr                                                               */
/* ---------                                                               */
/*                                                                         */
/* The configPtr parameter contains new values for one or more             */
/* configuration settings.  The values that are applied are selected by    */
/* the options parameter.                                                  */
/*                                                                         */
/*     gapChar - [default: '-'] -- This is the character that will be used */
/*               as the gap character if the file does not define one.     */
/*                                                                         */
/*     missingChar - [default: '?'] -- This is the character that will be  */
/*                   used as missing character if the file does not define */
/*                   one.                                                  */
/*                                                                         */
/*     useMaybes - [default: FALSE] -- If a line is found that doesn't     */
/*                 quite meet the criteria for being a sequence, but is    */
/*                 close enough that it might be a slightly mangled        */
/*                 sequence line, then it is marked as a 'maybe'. The      */
/*                 useMaybes setting determines how these 'maybe'          */
/*                 sequences are treated.  If set to FALSE, they ARE NOT   */
/*                 treated as sequences, if set to TRUE they ARE treated   */
/*                 as sequences.                                           */
/*                                                                         */
/*     readBuffSize - [default: 2048] -- This is size (in bytes) of the    */
/*                    chunks that are read when reading in the file.       */
/*                    Setting it to higher values may increase the         */
/*                    efficiency, but with operating system and hardware   */
/*                    buffering going on, it probably doesn't make much    */
/*                    difference.                                          */
/*                                                                         */
/*     debugLevel - [default: ALI_SHOW_NONE] -- Determines what debugging  */
/*                  information to display to stderr during processing.    */
/*                  Can be set to one of the following:                    */
/*                                                                         */
/*                  ALI_SHOW_NONE       : Show no debugging info [default] */
/*                  ALI_SHOW_SEQUENCES  : Show lines classified as seqs    */
/*                  ALI_SHOW_DEFLINES   : Show lines classified as deflines*/
/*                  ALI_SHOW_OTHERS     : Show lines classified as others  */
/*                                        (ie, not sequences or deflines). */
/*                  ALI_SHOW_ALL        : Show all lines and their         */
/*                                        classification.                  */
/*                                                                         */
/*     corruptSeqThreshold - [Default: 95] -- Used to guess that a line is */
/*                           actually a corrupted sequence.  If the line   */
/*                           contains a percentage of sequence characters  */
/*                           equal to or above the corruptSeqThreshold     */
/*                           then it is marked as maybe a sequence line.   */
/*                                                                         */
/*     nuclLineMaxThreshold - [Default: 75] -- Used to determine whether a */
/*                            sequence is DNA or protein.  If the line has */
/*                            MORE than nuclLineMaxThreshold percent of    */
/*                            the characters "ACGT" and the missing and    */
/*                            gap chars (and all the other characters are  */
/*                            ambiguous protein/DNA characters), then it   */
/*                            is marked as a nucleotide sequence.          */
/*                                                                         */
/*     nuclLineMinThreshold - [Default: 25] -- Used to determine whether a */
/*                            sequence is DNA or protein.  If the line has */
/*                            LESS than nuclLineMinThreshold percent of    */
/*                            the characters "ACGT" and the missing and    */
/*                            gap chars (and all the other characters are  */
/*                            ambiguous protein/DNA characters), then it   */
/*                            is marked as a protein sequence.             */
/*                                                                         */
/*     errExpandLevel       -                                              */
/*                                                                         */
/*                                                                         */
/* options parameter                                                       */
/* -----------------                                                       */
/*                                                                         */
/* The options parameter determines which fields in the configPtr are      */
/* being given new values.  It contains one or more of the following       */
/* values OR'd together :                                                  */
/*                                                                         */
/*	ALI_SET_DEFAULTS                                                   */
/*	ALI_SET_ALL                                                        */
/*                                                                         */
/*	ALI_SET_GAP_CHAR                                                   */
/*	ALI_SET_MISSING_CHAR                                               */
/*	ALI_SET_MAYBES                                                     */
/*	ALI_SET_READBUFF                                                   */
/*	ALI_SET_NUCL_MIN                                                   */
/*	ALI_SET_NUCL_MAX                                                   */
/*	ALI_SET_CORRUPT_MAX                                                */
/*	ALI_SET_DEBUG_LEVEL                                                */
/*      ALI_SET_ERRMSG_EXPAND                                              */
/*                                                                         */
/* If ALI_SET_DEFAULTS or ALI_SET_ALL are used then any others are         */
/* ignored.                                                                */
/*                                                                         */
/*=========================================================================*/

Boolean Ali_SetConfig (AliConfigInfoPtr configPtr,
		       Int2             mask)
{

  /* If this is the first time called, or we're restoring */
  /* the defaults, then set all options to the defaults.  */

  if ((s_configurationSet == FALSE) ||
      (configPtr == NULL)           ||
      (mask == ALI_SET_DEFAULTS))
    {
      s_configInfo.useMaybes                  = ALI_USE_MAYBES;
      s_configInfo.readBuffSize               = ALI_READ_BUFFSIZE;
      s_configInfo.debugLevel                 = ALI_SHOW_NONE;
      s_configInfo.corruptSeqThreshold        = ALI_CORRUPT_SEQ_THRESHOLD;
      s_configInfo.nuclLineMinThreshold       = ALI_NUCL_LINE_MIN_THRESHOLD;
      s_configInfo.nuclLineMaxThreshold       = ALI_NUCL_LINE_MAX_THRESHOLD;
      s_configInfo.declaredInfo.dataType      = ALI_UNKNOWN;
      s_configInfo.declaredInfo.contigOrInter = ALI_UNKNOWN;
      s_configInfo.errExpandLevel             = ALI_ERRMSG_EXPAND_SOME;
      s_configInfo.declaredInfo.idCount       = 0;
      s_configInfo.declaredInfo.seqLength     = 0;
      s_configInfo.gapChar = (CharPtr) MemNew (32);
      sprintf (s_configInfo.gapChar    , "%c%c", ALI_GAP_CHAR, '.');
      s_configInfo.missingChar = (CharPtr) MemNew (32);
      sprintf (s_configInfo.missingChar, "%c", ALI_MISSING_CHAR);
    }

  s_configurationSet = TRUE;

  /* If we're setting to the defaults, then we're done */

  if ((configPtr == NULL) || (mask == ALI_SET_DEFAULTS))
    return TRUE;

  /* Otherwise, override the current settings */
  /* where instructed.                        */

  if ((mask & ALI_SET_GAP_CHAR) || (mask == ALI_SET_ALL))
    StringCpy (s_configInfo.gapChar, configPtr->gapChar);

  if ((mask & ALI_SET_MISSING_CHAR) || (mask == ALI_SET_ALL))
    StringCpy (s_configInfo.missingChar, configPtr->missingChar);

  if ((mask & ALI_SET_MAYBES) || (mask == ALI_SET_ALL))
    s_configInfo.useMaybes = configPtr->useMaybes;

  if ((mask & ALI_SET_READBUFF) || (mask == ALI_SET_ALL))
    s_configInfo.readBuffSize = configPtr->readBuffSize;

  if ((mask & ALI_SET_NUCL_MIN) || (mask == ALI_SET_ALL))
    s_configInfo.nuclLineMinThreshold = configPtr->nuclLineMinThreshold;

  if ((mask & ALI_SET_NUCL_MAX) || (mask == ALI_SET_ALL))
    s_configInfo.nuclLineMaxThreshold = configPtr->nuclLineMaxThreshold;

  if ((mask & ALI_SET_CORRUPT_MAX) || (mask == ALI_SET_ALL))
    s_configInfo.corruptSeqThreshold = configPtr->corruptSeqThreshold;

  if ((mask & ALI_SET_DEBUG_LEVEL) || (mask == ALI_SET_ALL))
    s_configInfo.debugLevel = configPtr->debugLevel;

  if ((mask & ALI_SET_ERRMSG_EXPAND) || (mask == ALI_SET_ALL))
    s_configInfo.errExpandLevel = configPtr->errExpandLevel;

  /* Return successfully */

  return TRUE;
}

/*=========================================================================*/
/*                                                                         */
/*  s_FreeErrorNode () - Free one error structure.                         */
/*                                                                         */
/*=========================================================================*/

static void s_FreeErrorNode (ErrInfoPtr errorPtr)
{
  if (errorPtr->info != NULL)
    {
      MemFree (errorPtr->info);
      errorPtr->info = NULL;
    }
  if (errorPtr->extraInfo != NULL)
    {
      MemFree (errorPtr->extraInfo);
      errorPtr->extraInfo = NULL;
    }
  MemFree (errorPtr);
}

/*=========================================================================*/
/*                                                                         */
/*  s_FreeErrorList () - Free a linked list of error structures and all    */
/*                       the memory that they point to.                    */
/*                                                                         */
/*=========================================================================*/

static void s_FreeErrorList (ErrInfoPtr errorPtr)
{
  ErrInfoPtr currentErr;

  while (errorPtr != NULL)
    {
      currentErr = errorPtr;
      errorPtr = errorPtr->next;
      s_FreeErrorNode (currentErr);
    }
}

/*=========================================================================*/
/*                                                                         */
/*  s_FreeSequenceList () - Free a linked list of SeqPart structures and   */
/*                          all the memory that they point to.             */
/*                                                                         */
/*=========================================================================*/

static void s_FreeSequenceList (SeqPartPtr seqPtr)
{
  SeqPartPtr currentSeq;

  while (seqPtr != NULL)
    {
      MemFree (seqPtr->sequence);
      currentSeq = seqPtr;
      seqPtr = seqPtr->next;
      MemFree (currentSeq);
    }
}

/*=========================================================================*/
/*                                                                         */
/*  s_FreeIdList () - Free a linked list of ID structures and all the      */
/*                    memory that they point to.                           */
/*                                                                         */
/*=========================================================================*/

static void s_FreeIdList (IdInfoPtr idPtr)
{
  IdInfoPtr currentId;

  while (idPtr != NULL)
    {
      MemFree (idPtr->id);
      s_FreeSequenceList (idPtr->sequence);
      MemFree (idPtr->defline);
      currentId = idPtr;
      idPtr = idPtr->next;
      MemFree (currentId);
    }
}

/*=========================================================================*/
/*                                                                         */
/*  s_FreeParsedInfo () - Free a ParsedInfo structure and the memory that  */
/*                        it points to.                                    */
/*                                                                         */
/*=========================================================================*/

static void s_FreeParsedInfo (ParsedInfoPtr info)
{
  if (info->missingChar != NULL)
    MemFree (info->missingChar);
  if (info->gapChar != NULL)
    MemFree (info->gapChar);
  if (info->unalignedChar != NULL)
    MemFree (info->unalignedChar);
  MemFree (info);
}

/*=========================================================================*/
/*                                                                         */
/*  Ali_Free () - Free a AlignFileData structure and all the memory that   */
/*                it points to.                                            */
/*                                                                         */
/*=========================================================================*/

void Ali_Free (AlignFileDataPtr fileInfoPtr)
{

  s_FreeIdList (fileInfoPtr->sequences);
  fileInfoPtr->sequences = NULL;
  s_FreeIdList (fileInfoPtr->maybes);
  fileInfoPtr->maybes = NULL;
  s_FreeErrorList (fileInfoPtr->errors);
  fileInfoPtr->errors = NULL;
  s_FreeParsedInfo (fileInfoPtr->info);
  fileInfoPtr->info = NULL;

  MemFree (fileInfoPtr);

  return;
}

/*=========================================================================*/
/*                                                                         */
/*  s_FreeRowList () - Free all row data structures and the strings that   */
/*                     they point to.                                      */
/*                                                                         */
/*         NOTE: The actual data strings in the row list may be pointed    */
/*               to by other structures, in which case                     */
/*               s_FreeRowList_Safe () should be used instead.             */
/*                                                                         */
/*=========================================================================*/

static void s_FreeRowList (ValNodePtr rowList)
{
  ValNodePtr       currentRow;
  SeqLineInfoPtr   seqLine;
  DefLineInfoPtr   defLine;
  OtherLineInfoPtr otherLine;

  while (rowList != NULL)
    {
      switch (rowList->choice)
	{
	case ALI_DEFLINE :
	  defLine = (DefLineInfoPtr) rowList->data.ptrvalue;
	  if (defLine->definitions != NULL)
	    MemFree (defLine->definitions);
	  if (defLine->id != NULL)
	    MemFree (defLine->id);
	  MemFree (defLine);
	  break;
	case ALI_SEQLINE :
	  seqLine = (SeqLineInfoPtr) rowList->data.ptrvalue;
	  if (seqLine->sequence != NULL)
	    MemFree (seqLine->sequence);
	  if (seqLine->id != NULL)
	    MemFree (seqLine->id);
	  if (seqLine->junk != NULL)
	    MemFree (seqLine->junk);
	  MemFree (seqLine);
	  break;
	case ALI_OTHERLINE :
	  otherLine = (OtherLineInfoPtr) rowList->data.ptrvalue;
	  if (otherLine->other != NULL)
	    MemFree (otherLine->other);
	  if (otherLine->id != NULL)
	    MemFree (otherLine->id);
	  MemFree (otherLine);
	  break;
	default:
	  break;
	}
      currentRow = rowList;
      rowList = rowList->next;
      MemFree (currentRow);
    }
}

/*=========================================================================*/
/*                                                                         */
/*  s_FreeRowList_Safe () - Free all row data structures, but don't free   */
/*                          the strings that they point since they are     */
/*                          still being used in the ID structures.         */
/*                                                                         */
/*=========================================================================*/

static void s_FreeRowList_Safe (ValNodePtr rowList)
{
  ValNodePtr       currentRow;
  SeqLineInfoPtr   seqLine;
  DefLineInfoPtr   defLine;
  OtherLineInfoPtr otherLine;

  while (rowList != NULL)
    {
      switch (rowList->choice)
	{
	case ALI_DEFLINE :
	  defLine = (DefLineInfoPtr) rowList->data.ptrvalue;
	  MemFree (defLine);
	  break;
	case ALI_SEQLINE :
	  seqLine = (SeqLineInfoPtr) rowList->data.ptrvalue;
	  MemFree (seqLine);
	  break;
	case ALI_OTHERLINE :
	  otherLine = (OtherLineInfoPtr) rowList->data.ptrvalue;
	  MemFree (otherLine);
	  break;
	default:
	  break;
	}
      currentRow = rowList;
      rowList = rowList->next;
      MemFree (currentRow);
    }
}

/*=========================================================================*/
/*                                                                         */
/*  s_GetRowIdStr ()                                                       */
/*                                                                         */
/*=========================================================================*/

static CharPtr s_GetRowIdString (ValNodePtr row)
{
  CharPtr          newIdStr;
  SeqLineInfoPtr   seqLinePtr;
  DefLineInfoPtr   defLinePtr;
  OtherLineInfoPtr otherLinePtr;

  if (row == NULL)
    return NULL;

  if (row->choice == ALI_SEQLINE)
    {
      seqLinePtr = (SeqLineInfoPtr) row->data.ptrvalue;
      if (seqLinePtr->id != NULL)
	{
	  if ((seqLinePtr->maybe == TRUE) && (s_configInfo.useMaybes == FALSE))
	    newIdStr = NULL;
	  else
	    newIdStr = seqLinePtr->id;
	}
      else
	newIdStr = NULL;
    }
  else if (row->choice == ALI_DEFLINE)
    {
      defLinePtr = (DefLineInfoPtr) row->data.ptrvalue;
      if (defLinePtr->id != NULL)
	newIdStr = defLinePtr->id;
      else
	newIdStr = NULL;
    }
  else if (row->choice == ALI_OTHERLINE)
    {
      otherLinePtr = (OtherLineInfoPtr) row->data.ptrvalue;
      if (otherLinePtr->id != NULL)
	newIdStr = otherLinePtr->id;
      else
	newIdStr = NULL;
    }

  return newIdStr;
}

/*=========================================================================*/
/*                                                                         */
/*  s_GetRowSeqStr ()                                                      */
/*                                                                         */
/*=========================================================================*/

static CharPtr s_GetRowSeqString (ValNodePtr row)
{
  CharPtr          newSeqStr;
  SeqLineInfoPtr   seqLinePtr;

  if (row == NULL)
    return NULL;

  if (row->choice == ALI_SEQLINE)
    {
      seqLinePtr = (SeqLineInfoPtr) row->data.ptrvalue;
      if (seqLinePtr->sequence != NULL)
	{
	  if ((seqLinePtr->maybe == TRUE) && (s_configInfo.useMaybes == FALSE))
	    newSeqStr = NULL;
	  else
	    newSeqStr = seqLinePtr->sequence;
	}
      else
	newSeqStr = NULL;
    }
  else
    newSeqStr = NULL;

  return newSeqStr;
}

/*=========================================================================*/
/*                                                                         */
/*  s_ProcessMaybes ()                                                     */
/*                                                                         */
/*=========================================================================*/

static IdInfoPtr s_ProcessMaybes (ValNodePtr rowList)
{
  ValNodePtr     currentRow;
  IdInfoPtr      badIdList = NULL;
  IdInfoPtr      existingId = NULL;
  IdInfoPtr      currentId = NULL;
  IdInfoPtr      lastId = NULL;
  CharPtr        idStr;
  CharPtr        currentIdStr;
  SeqPartPtr     newSeqPart;
  SeqPartPtr     lastSeqPart;
  SeqLineInfoPtr seqLinePtr;

  currentRow = rowList;

  while (currentRow != NULL)
    {
      idStr = s_GetRowIdString (currentRow);
      if (idStr != NULL)
	currentIdStr = idStr;

      if (currentRow->choice == ALI_SEQLINE)
	{
	  seqLinePtr = (SeqLineInfoPtr) currentRow->data.ptrvalue;
	  if (seqLinePtr->maybe == TRUE)
	    {

	      /* Find the ID that this sequence 'belongs to' */

	      existingId = badIdList;
	      while (existingId != NULL)
		{
		  if (StringCmp(existingId->id,currentIdStr) == 0)
		    break;
		  existingId = existingId->next;
		}
	      
	      if (existingId != NULL)
		currentId = existingId;
	      else
		{
		  currentId = (IdInfoPtr) MemNew (sizeof(IdInfo));
		  if (currentId == NULL)
		    return NULL;
		  
		  currentId->sequence = NULL;
		  currentId->id       = currentIdStr;
		  currentId->length   = 0;
		  currentId->next     = NULL;
		  
		  if (badIdList == NULL)
		    badIdList = currentId;
		  else
		    {
		      lastId = badIdList;
		      while (lastId->next != NULL)
			lastId = lastId->next;
		      lastId->next = currentId;
		    }
		}
	  
	      /* Add the sequence to the current ID */

	      newSeqPart = (SeqPartPtr) MemNew(sizeof(SeqPart));
	      if (newSeqPart == NULL)
		return NULL;
	      
	      newSeqPart->sequence = (CharPtr) currentRow->data.ptrvalue;
	      newSeqPart->next     = NULL;
	      
	      if (currentId->sequence == NULL)
		currentId->sequence = newSeqPart;
	      else
		lastSeqPart->next = newSeqPart;
	      
	      currentId->length += StringLen (newSeqPart->sequence);
	      lastSeqPart = newSeqPart;
	      
	    }
	}
      currentRow = currentRow->next;
    }

  return badIdList;
}

/*=========================================================================*/
/*                                                                         */
/*  DisplayRowList() - Prints to stderr the linked list of ValNodes that   */
/*                     contain the data read in from the alignment file.   */
/*                                                                         */
/*=========================================================================*/

static void s_DisplayRowList (ValNodePtr rowList,
			      Int2       mask)
{
  ValNodePtr       currRow;
  SeqLineInfoPtr   seqLinePtr;
  DefLineInfoPtr   defLinePtr;
  OtherLineInfoPtr otherLinePtr;
  Char             cLineType;

  currRow = rowList;
  while (currRow != NULL)
    {
      if ((currRow->choice == ALI_SEQLINE) &&
	  ((mask & ALI_SHOW_SEQUENCES) ||
	   (mask == ALI_SHOW_ALL)))
	{
	  seqLinePtr = (SeqLineInfoPtr) currRow->data.ptrvalue;

	  if (seqLinePtr->type == ALI_NUCLEOTIDE)
	    cLineType = 'N';
	  else if (seqLinePtr->type == ALI_PROTEIN)
	    cLineType = 'P';
	  else if (seqLinePtr->type == ALI_AMBIGUOUS)
	    cLineType = 'U';

	  if (seqLinePtr->maybe == FALSE)
	    {
	      if (seqLinePtr->id != NULL)
		fprintf(stderr,"%04d: ID          : %s\n",
			seqLinePtr->rowNum,
			seqLinePtr->id);
	      if (seqLinePtr->sequence != NULL)
		fprintf(stderr,"%04d: SEQUENCE[%c] : %s\n",
			seqLinePtr->rowNum,
			cLineType,
			seqLinePtr->sequence);
	    }
	  else
	    {
	      if (seqLinePtr->id != NULL)
		fprintf(stderr,"%04d: MAYBE ID          : %s\n",
			seqLinePtr->rowNum,
			seqLinePtr->id);
	      if (seqLinePtr->sequence != NULL)
		fprintf(stderr,"%04d: MAYBE SEQUENCE[%c] : %s\n",
			seqLinePtr->rowNum,
			cLineType,
			seqLinePtr->sequence);
	    }
	}
      else if ((currRow->choice == ALI_DEFLINE) &&
	       ((mask & ALI_SHOW_DEFLINES) ||
		(mask == ALI_SHOW_ALL)))
	{
	  defLinePtr = (DefLineInfoPtr) currRow->data.ptrvalue;
	  if (defLinePtr->id != NULL)
	    fprintf(stderr,"%04d: DEFLINE ID          : %s\n",
		    defLinePtr->rowNum,
		    defLinePtr->id);
	  if (defLinePtr->definitions != NULL)
	    fprintf(stderr,"%04d: DEFLINE DEFINITIONS : %s\n", 
		    defLinePtr->rowNum,
		    defLinePtr->definitions);
	}
      else if ((currRow->choice == ALI_OTHERLINE) &&
	       ((mask & ALI_SHOW_OTHERS) ||
		(mask == ALI_SHOW_ALL)))
	{
	  otherLinePtr = (OtherLineInfoPtr) currRow->data.ptrvalue;
	  if (otherLinePtr->id != NULL)
	    fprintf(stderr,"%04d: OTHER ID : %s\n", otherLinePtr->rowNum,
		    otherLinePtr->id);
	  if (otherLinePtr->other != NULL)
	    fprintf(stderr,"%04d: OTHER    : %s\n", otherLinePtr->rowNum,
		    otherLinePtr->other);
	}
      currRow = currRow->next;
    }

  return;
}


/*=========================================================================*/
/*                                                                         */
/* s_isInterleaved ()                                                      */
/*                                                                         */
/*=========================================================================*/

static Boolean s_IsInterleaved (ValNodePtr rowList,
				Int2 PNTR idCount)
{
  ValNodePtr       currentRow;
  CharPtr          newIdStr;
  IdInfoPtr        idList = NULL;
  IdInfoPtr        lastId = NULL;
  IdInfoPtr        currentId = NULL;
  IdInfoPtr        existingId = NULL;
  Boolean          isInterleaved;
  Int4             patternRowCount;
  Int4             patternCharCount;
  Int4             currentRowCount;
  Int4             currentCharCount;
  Boolean          isFirstId;
  SeqLineInfoPtr   seqLinePtr;
  DefLineInfoPtr   defLinePtr;
  OtherLineInfoPtr otherLinePtr;
  Boolean          isMaybe;

  isInterleaved = FALSE;
  currentRow  = rowList;

  patternRowCount  = 0;
  patternCharCount = 0;
  currentRowCount  = 0;
  currentCharCount = 0;
  isFirstId        = TRUE;
  *idCount         = 0;

  /* Search the row list for IDs */

  while (currentRow != NULL)
    {

      /* Look for an ID */

      newIdStr = NULL;
      isMaybe = FALSE;

      if (currentRow->choice == ALI_SEQLINE)
	{
	  seqLinePtr = (SeqLineInfoPtr) currentRow->data.ptrvalue;
	  if (seqLinePtr->id != NULL)
	    {
	      if ((seqLinePtr->maybe == TRUE) &&
		  (s_configInfo.useMaybes == FALSE))
		newIdStr = NULL;
	      else
		newIdStr = seqLinePtr->id;
	    }
	}
      else if (currentRow->choice == ALI_DEFLINE)
	{
	  defLinePtr = (DefLineInfoPtr) currentRow->data.ptrvalue;
	  if (defLinePtr->id != NULL)
	    newIdStr = defLinePtr->id;
	}
      else if (currentRow->choice == ALI_OTHERLINE)
	{
	  otherLinePtr = (OtherLineInfoPtr) currentRow->data.ptrvalue;
	  if (otherLinePtr->id != NULL)
	    newIdStr = otherLinePtr->id;
	}

      /* If we find an ID, see if it's one */
      /* that we already have.             */

      if (newIdStr != NULL)
	{

	  existingId = idList;
	  while (existingId != NULL)
	    {
	      if (StringCmp(existingId->id,newIdStr) == 0)
		break;
	      existingId = existingId->next;
	    }

	  /* Already have -- break and return TRUE */

	  if (existingId != NULL)
	    {
	      isInterleaved = TRUE;
	      break;
	    }

	  /* Otherwise, add the ID to the list */

	  currentRowCount  = 0;
	  currentCharCount = 0;

	  if (idList != NULL)
	    isFirstId = FALSE;

	  (*idCount)++;
	  
	  currentId = (IdInfoPtr) MemNew (sizeof(IdInfo));
	  if (currentId == NULL)
	    return FALSE;
	  
	  currentId->sequence = NULL;
	  currentId->id       = newIdStr;
	  currentId->length   = 0;
	  currentId->next     = NULL;
	  
	  if (idList == NULL)
	    idList = currentId;
	  else
	    {
	      lastId = idList;
	      while (lastId->next != NULL)
		lastId = lastId->next;
	      lastId->next = currentId;
	    }
	}

      /* Process sequence rows */

      if (currentRow->choice == ALI_SEQLINE)
	{

	  if (seqLinePtr->sequence != NULL)
	    if ((s_configInfo.useMaybes == TRUE) ||
		(s_configInfo.useMaybes == FALSE) &&
		(seqLinePtr->maybe == FALSE))
	      {
		/* There must be an ID before the first sequence */
		
		if (currentId == NULL)
		  {
		    isInterleaved = FALSE;
		    break;
		  }
		
		/* Look for sequences that probably */
		/* have no ID assigned to them.     */
		
		seqLinePtr = (SeqLineInfoPtr) currentRow->data.ptrvalue;
		if (isFirstId)
		  {
		    patternRowCount++;
		    patternCharCount += StringLen (seqLinePtr->sequence);
		  }
		else
		  {
		    currentRowCount++;
		    currentCharCount += StringLen (seqLinePtr->sequence);
		    if ((currentRowCount > patternRowCount) &&
			(currentCharCount > patternCharCount))
		      {
			isInterleaved = TRUE;
			break;
		      }
		  }
	      }
	  
	}

      /* Go to next row */

      currentRow = currentRow->next;
    }

  /* Delete the ID records that we created */
  /*  NOTE -- The ID strings themselves    */
  /*          are stored elsewhere and     */
  /*          only pointed to here, so     */
  /*          DON"T delete them.      o     */

  while (idList != NULL)
    {
      lastId = idList;
      idList = idList->next;
      MemFree(lastId);
    }

  /* Return result of search */

  return isInterleaved;
}

/*=========================================================================*/
/*                                                                         */
/* s_ProcessInterId ()                                                     */
/*                                                                         */
/*=========================================================================*/

static Boolean s_ProcessInterId (CharPtr          newIdStr,
				 PatternInfoPtr   pattern,
				 AlignFileDataPtr fileInfoPtr,
				 Boolean          isMaybe)
{
  IdInfoPtr   lastId = NULL;
  IdInfoPtr   existingId = NULL;
  ErrInfoPtr  errPtr;

  /* If we've got all our ID's then */
  /* ignore any further ones.       */

  if (pattern->gotAllIds == TRUE)
    return TRUE;

  /* All ID's, except for the first one, should */
  /* immediately follow a sequence line.        */
  
  if (pattern->isFirstId == FALSE)
    {
      if (pattern->lastRow->choice != ALI_SEQLINE)
	{
	  errPtr = Ali_AddError (&(fileInfoPtr->errors),
				 ERR_ID_NO_PRECEDING_SEQ,
				 newIdStr);
	  return FALSE;
	}
      else
	pattern->isFirstGroup = FALSE;
    }

  /* If this id already exists, */
  /* make it the current ID.    */
  
  existingId = fileInfoPtr->sequences;
  while (existingId != NULL)
    {
      if (StringCmp(existingId->id,newIdStr) == 0)
	break;
      existingId = existingId->next;
    }
  
  if (existingId != NULL)
    pattern->currentId = existingId;
  
  /* Otherwise create a new Id record */
  /* and add it to the end of list.   */
  
  else
    {
      pattern->currentId = (IdInfoPtr) MemNew (sizeof(IdInfo));
      if (pattern->currentId == NULL)
	{
	  Ali_AddError (&(fileInfoPtr->errors), ERR_OUT_OF_MEMORY);
	  return FALSE;
	}
      
      pattern->currentId->sequence = NULL;
      pattern->currentId->id       = newIdStr;
      pattern->currentId->length   = 0;
      pattern->currentId->next     = NULL;
      
      if (fileInfoPtr->sequences == NULL)
	fileInfoPtr->sequences = pattern->currentId;
      else
	{
	  lastId = fileInfoPtr->sequences;
	  while (lastId->next != NULL)
	    lastId = lastId->next;
	  lastId->next = pattern->currentId;
	}
      
      pattern->currentIdCount++;
      if (pattern->currentIdCount == pattern->idCount)
	pattern->gotAllIds = TRUE;
    }
  
  if (pattern->isFirstId)
    pattern->isFirstId = FALSE;

  /* Return successfully */

  return TRUE;
}

/*=========================================================================*/
/*                                                                         */
/* s_ProcessInterSeq ()                                                    */
/*                                                                         */
/*=========================================================================*/

static Boolean s_ProcessInterSeq (CharPtr          newSeqStr,
				  PatternInfoPtr   pattern,
				  AlignFileDataPtr fileInfoPtr,
				  Boolean          isMaybe)
{
  SeqPartPtr  newSeqPart = NULL;
  ErrInfoPtr  errPtr = NULL;

  /* There must be an ID before the first sequence */
  
  if (pattern->currentId == NULL)
    {
      errPtr = Ali_AddError (&(fileInfoPtr->errors), ERR_SEQ_WITHOUT_ID,
			     newSeqStr);
      return FALSE;
    }
  
  /* Add the sequence to the current ID */

  newSeqPart = (SeqPartPtr) MemNew(sizeof(SeqPart));
  if (newSeqPart == NULL)
    {
      Ali_AddError (&(fileInfoPtr->errors), ERR_OUT_OF_MEMORY);
      return FALSE;
    }
  
  newSeqPart->sequence = newSeqStr;
  newSeqPart->next     = NULL;
  
  if (pattern->currentId->sequence == NULL)
    pattern->currentId->sequence = newSeqPart;
  else
    pattern->lastSeqPart->next = newSeqPart;
  
  pattern->currentId->length += StringLen (newSeqPart->sequence);
  pattern->lastSeqPart = newSeqPart;
  
  /* If we've started repeating IDs then */
  /* rotate through the id list.         */
  
  if (pattern->gotAllIds == TRUE)
    {
      if (pattern->currentId->next == NULL)
	pattern->currentId = fileInfoPtr->sequences;
      else
	pattern->currentId = pattern->currentId->next;

      pattern->lastSeqPart = pattern->currentId->sequence;
      while (pattern->lastSeqPart->next != NULL)
	pattern->lastSeqPart = pattern->lastSeqPart->next;
    }

  /* Return successfully */
  
  return TRUE;
}

/*=========================================================================*/
/*                                                                         */
/* s_AnalyzeInterleaved ()                                                 */
/*                                                                         */
/*=========================================================================*/

static Boolean s_AnalyzeInterleaved (ValNodePtr       rowList,
				     AlignFileDataPtr fileInfoPtr,
				     Int2             idCount)
{
  ValNodePtr       currentRow;
  Boolean          isValidPattern;
  IdInfoPtr        currentId = NULL;
  Int4             previousLength;
  ErrInfoPtr       errPtr;
  PatternInfoPtr   pattern;
  SeqLineInfoPtr   seqLinePtr;
  DefLineInfoPtr   defLinePtr;
  OtherLineInfoPtr otherLinePtr;
  Boolean          firstDefline = TRUE;
  IdInfoPtr        lastId = NULL;

  pattern = (PatternInfoPtr) MemNew (sizeof (PatternInfo));

  pattern->currentDeflineId = NULL;
  pattern->lastRow          = NULL;
  pattern->isFirstId        = TRUE;
  pattern->isFirstGroup     = TRUE;
  pattern->maybesFound      = FALSE;
  pattern->gotAllIds        = FALSE;
  pattern->idCount          = idCount;
  pattern->currentIdCount   = 0;

  pattern->foundInfo.dataType      = ALI_UNKNOWN;
  pattern->foundInfo.contigOrInter = ALI_UNKNOWN;
  pattern->foundInfo.idCount       = 0;
  pattern->foundInfo.seqLength     = 0;

  /* Match the sequences up with the IDs */

  currentRow    = rowList;
  isValidPattern  = TRUE;

  while (currentRow != NULL)
    {

      if (currentRow->choice == ALI_SEQLINE)
	{
  
	  seqLinePtr = (SeqLineInfoPtr) currentRow->data.ptrvalue;

	  if ((seqLinePtr->maybe == FALSE) ||
	      (seqLinePtr->maybe == TRUE) && (s_configInfo.useMaybes == TRUE))
	    {
	      if (seqLinePtr->id != NULL)
		{
		  /* Process the ID */
  
		  isValidPattern = s_ProcessInterId (seqLinePtr->id,
						     pattern,
						     fileInfoPtr,
						     seqLinePtr->maybe);
		  if (isValidPattern == FALSE)
		    break;
		}

	      if (seqLinePtr->sequence != NULL)
		{
		  isValidPattern = s_ProcessInterSeq (seqLinePtr->sequence,
						      pattern,
						      fileInfoPtr,
						      seqLinePtr->maybe);
		  if (isValidPattern == FALSE)
		    break;
		}
	      pattern->lastRow = currentRow;
	    }
	  else
	    pattern->maybesFound = TRUE;
	}
      else if (currentRow->choice == ALI_DEFLINE)
	{
	  defLinePtr = (DefLineInfoPtr) currentRow->data.ptrvalue;
	  if (defLinePtr->id != NULL)
	    {
	      isValidPattern = s_ProcessInterId (defLinePtr->id,
						 pattern,
						 fileInfoPtr,
						 FALSE);
	      if (isValidPattern == FALSE)
		break;
	    }
	  if (defLinePtr->definitions != NULL)
	    {
	      if (firstDefline)
		{
		  firstDefline = FALSE;
		  pattern->currentDeflineId = fileInfoPtr->sequences;
		}
	      else
		pattern->currentDeflineId =
		  pattern->currentDeflineId->next;
	      
	      if (pattern->currentDeflineId == NULL)
		{
		  errPtr = Ali_AddError (&(fileInfoPtr->errors),
					 ERR_DEFLINE_WITH_NO_ID,
					 defLinePtr->definitions);
		  errPtr->rowNum = defLinePtr->rowNum;
		  isValidPattern = FALSE;
		  break;
		}
	      else
		{
		  pattern->currentDeflineId->defline =
		    defLinePtr->definitions;
		}
	    }
	  pattern->lastRow = currentRow;
	}
      else if (currentRow->choice == ALI_OTHERLINE)
	{
	  otherLinePtr = (OtherLineInfoPtr) currentRow->data.ptrvalue;
	  if (otherLinePtr->id != NULL)
	    {
	      isValidPattern = s_ProcessInterId (otherLinePtr->id,
						 pattern,
						 fileInfoPtr,
						 FALSE);
	      if (isValidPattern == FALSE)
		break;
	    }
	  pattern->lastRow = currentRow;
	}

      currentRow = currentRow->next;
    }

  /* If we found one defline, then */
  /* make sure they were all there */

  if (firstDefline == FALSE)
    {
      lastId = fileInfoPtr->sequences;
      if (lastId != NULL)
	{
	  while (lastId->next != NULL)
	    lastId = lastId->next;
	  if (lastId->defline == NULL)
	    {
	      errPtr = Ali_AddError (&(fileInfoPtr->errors),
				     ERR_ID_WITH_NO_DEFLINE,
				     lastId->id);
	      isValidPattern = FALSE;
	    }
	}
    }

  /* If pattern not found, return failure */

  if (!isValidPattern)
    return FALSE;

  /* If there was a declared number of sequences then */
  /* check to see that it matches the number found.   */

  if ((s_configInfo.declaredInfo.idCount !=0) &&
      (s_configInfo.declaredInfo.idCount != idCount))
    {
      errPtr = Ali_AddError (&(fileInfoPtr->errors), ERR_ID_COUNT_MISMATCH,
			     idCount, s_configInfo.declaredInfo.idCount);
      errPtr->level = LEVEL_WARNING;
    }

  /* Sequences should all be the same length. */

  currentId = fileInfoPtr->sequences;
  pattern->isFirstId = TRUE;

  while (currentId != NULL)
    {
      if (pattern->isFirstId)
	pattern->isFirstId = FALSE;
      else
	{
	  if (previousLength < currentId->length)
	    {
	      errPtr = Ali_AddError (&(fileInfoPtr->errors),
				     ERR_SEQUENCE_TOO_LONG,
				     currentId->id,
				     previousLength,
				     currentId->length);
	      break;
	    }
	  else if (previousLength > currentId->length)
	    {
	      errPtr = Ali_AddError (&(fileInfoPtr->errors),
				     ERR_SEQUENCE_TOO_SHORT,
				     currentId->id,
				     previousLength,
				     currentId->length);
	      break;
	    }
	}
      previousLength = currentId->length;
      currentId = currentId->next;
    }

  /* Check to see that declared sequence */
  /* length matches the lengths found.   */

  if ((s_configInfo.declaredInfo.seqLength != 0) &&
      (s_configInfo.declaredInfo.seqLength != previousLength))
    {
      errPtr = Ali_AddError (&(fileInfoPtr->errors),ERR_SEQ_LENGTH_MISMATCH,
			     previousLength,
			     s_configInfo.declaredInfo.seqLength);
      errPtr->level = LEVEL_WARNING;
    }

  /* Process the maybes if they weren't used already */

  if (pattern->maybesFound == TRUE)
    fileInfoPtr->maybes = s_ProcessMaybes (rowList);

  /* Return successfully */

  if (currentId == NULL)
    return TRUE;
  else
    return FALSE;
} 

/*=========================================================================*/
/*                                                                         */
/* s_ProcessContigId ()                                                    */
/*                                                                         */
/*=========================================================================*/

static Boolean s_ProcessContigId (CharPtr          newIdStr,
				  PatternInfoPtr   pattern,
				  AlignFileDataPtr fileInfoPtr)
{
  IdInfoPtr      existingId = NULL;
  ErrInfoPtr     errPtr;
  IdInfoPtr      lastId = NULL;

  if (pattern->isFirstId == FALSE)
    {
      pattern->isFirstGroup = FALSE;

      /* The length of the last pattern must match */
      /* the length of previous ones.              */
  
      if (pattern->currentId->length < pattern->foundInfo.seqLength)
	{
	  errPtr = Ali_AddError (&(fileInfoPtr->errors),
				 ERR_SEQUENCE_TOO_SHORT,
				 pattern->currentId->id,
				 pattern->foundInfo.seqLength,
				 pattern->currentId->length);
	  return FALSE;
	}
    }  
  
  pattern->hasFullLength = FALSE;

  /* See if this ID already exists */
  
  existingId = fileInfoPtr->sequences;
  while (existingId != NULL)
    {
      if (StringCmp(existingId->id,newIdStr) == 0)
	{
	  errPtr = Ali_AddError (&(fileInfoPtr->errors), ERR_DUPLICATE_IDS, 
				 newIdStr);
	  return FALSE;
	}
      existingId = existingId->next;
    }
  
  /* If this id already exists, */
  /* make it the current ID.    */
  
  if (existingId != NULL)
    pattern->currentId = existingId;
  
  /* Otherwise create a new Id record */
  /* and add it to the end of list.   */
  
  else
    {
      pattern->currentId = (IdInfoPtr) MemNew (sizeof(IdInfo));
      if (pattern->currentId == NULL)
	{
	  Ali_AddError (&(fileInfoPtr->errors), ERR_OUT_OF_MEMORY);
	  return FALSE;
	}
      
      pattern->currentId->sequence = NULL;
      pattern->currentId->id       = newIdStr;
      pattern->currentId->length   = 0;
      pattern->currentId->next     = NULL;
      
      if (fileInfoPtr->sequences == NULL)
	fileInfoPtr->sequences = pattern->currentId;
      else
	{
	  lastId = fileInfoPtr->sequences;
	  while (lastId->next != NULL)
	    lastId = lastId->next;
	  lastId->next = pattern->currentId;
	}
      pattern->foundInfo.idCount++;
    }
  
  if (pattern->isFirstId)
    pattern->isFirstId = FALSE;

  /* Return successfully */

  return TRUE;
}

/*=========================================================================*/
/*                                                                         */
/* s_ProcessContigSeq ()                                                   */
/*                                                                         */
/*=========================================================================*/

static Boolean s_ProcessContigSeq (CharPtr          newSeqStr,
				   PatternInfoPtr   pattern,
				   AlignFileDataPtr fileInfoPtr)
{
  SeqPartPtr     newSeqPart = NULL;
  ErrInfoPtr     errPtr;

  /* There must be an ID before we get a sequence */

  if (pattern->currentId == NULL)
    {
      errPtr = Ali_AddError (&(fileInfoPtr->errors), ERR_SEQ_WITHOUT_ID,
			     newSeqStr);
      return FALSE;
    }
  
  /* Add the sequence to the current ID */
  
  newSeqPart = (SeqPartPtr) MemNew(sizeof(SeqPart));
  if (newSeqPart == NULL)
    {
      Ali_AddError (&(fileInfoPtr->errors), ERR_OUT_OF_MEMORY);
      return FALSE;
    }
  
  newSeqPart->sequence = newSeqStr;
  newSeqPart->next     = NULL;
  
  if (pattern->currentId->sequence == NULL)
    pattern->currentId->sequence = newSeqPart;
  else
    pattern->lastSeqPart->next = newSeqPart;

  /* Make sure that sequence length hasn't */
  /* exceeded that of previous sequences.  */
  
  pattern->currentId->length += StringLen (newSeqPart->sequence);
  pattern->lastSeqPart = newSeqPart;
  
  if (pattern->isFirstGroup)
    {
      pattern->foundInfo.seqLength += StringLen (newSeqPart->sequence);
    }
  else
    {

      if (pattern->currentId->length == pattern->foundInfo.seqLength)
	pattern->hasFullLength = TRUE;
      else if (pattern->currentId->length > pattern->foundInfo.seqLength)
	{
	  errPtr = Ali_AddError (&(fileInfoPtr->errors),
				 ERR_SEQUENCE_TOO_LONG,
				 pattern->currentId->id,
				 pattern->foundInfo.seqLength,
				 pattern->currentId->length);
	  return FALSE;
	}
    }

  /* Return successfully */

  return TRUE;
}

/*=========================================================================*/
/*                                                                         */
/* s_AnalyzeContiguous ()                                                  */
/*                                                                         */
/*=========================================================================*/

static Boolean s_AnalyzeContiguous (ValNodePtr       rowList,
				    AlignFileDataPtr fileInfoPtr)
{
  ValNodePtr       currentRow;
  SeqLineInfoPtr   seqLinePtr;
  DefLineInfoPtr   defLinePtr;
  OtherLineInfoPtr otherLinePtr;
  Boolean          isValidPattern;
  IdInfoPtr        lastId = NULL;
  IdInfoPtr        nextToLastId = NULL;
  ErrInfoPtr       errPtr;
  PatternInfoPtr   pattern;
  Boolean          firstDefline = TRUE;

  /* Initialize the pattern info */

  pattern = (PatternInfoPtr) MemNew (sizeof (PatternInfo));

  pattern->currentDeflineId = NULL;
  pattern->currentId        = NULL;
  pattern->lastSeqPart      = NULL;
  pattern->hasFullLength    = FALSE;
  pattern->isFirstId        = TRUE;
  pattern->isFirstGroup     = TRUE;
  pattern->maybesFound      = FALSE;

  pattern->foundInfo.dataType      = ALI_UNKNOWN;
  pattern->foundInfo.contigOrInter = ALI_UNKNOWN;
  pattern->foundInfo.idCount       = 0;
  pattern->foundInfo.seqLength     = 0;

  /* Match the sequences up with the IDS */

  currentRow    = rowList;
  isValidPattern  = TRUE;

  while (currentRow != NULL)
    {

      /* Process sequence lines */

      if (currentRow->choice == ALI_SEQLINE)
	{

	  seqLinePtr = (SeqLineInfoPtr) currentRow->data.ptrvalue;

	  /* If we already have a sequence equal in */
	  /* in length to those that came before,   */
	  /* then this line may actually be an      */
	  /* ID.                                    */
	     

	  if ((pattern->hasFullLength == TRUE) &&
	      (seqLinePtr->id == NULL))
	    {
	      Ali_ChangeRowToOther (currentRow);
	      continue;
	    }

	  /* Process the line as a sequence */

	  if ((seqLinePtr->maybe == FALSE) ||
	      (seqLinePtr->maybe == TRUE) && (s_configInfo.useMaybes == TRUE))
	    {
	      if (seqLinePtr->id != NULL)
		{
		  isValidPattern = s_ProcessContigId (seqLinePtr->id,
						      pattern,
						      fileInfoPtr);
		  if (isValidPattern == FALSE)
		    break;
		}
	      
	      if (seqLinePtr->sequence != NULL)
		{
		  isValidPattern = s_ProcessContigSeq (seqLinePtr->sequence,
						       pattern,
						       fileInfoPtr);
		  if (isValidPattern == FALSE)
		    break;
		}
	      pattern->lastRow = currentRow;
	    }
	  else
	    pattern->maybesFound = TRUE;
	}

      /* Process Definition lines */

      else if (currentRow->choice == ALI_DEFLINE)
	{
	  defLinePtr = (DefLineInfoPtr) currentRow->data.ptrvalue;
	  if (defLinePtr->id != NULL)
	    {
	      isValidPattern = s_ProcessContigId (defLinePtr->id,
						  pattern,
						  fileInfoPtr);
	      if (isValidPattern == FALSE)
		break;
	    }

	  if (defLinePtr->definitions != NULL)
	    {
	      if (firstDefline)
		{
		  firstDefline = FALSE;
		  pattern->currentDeflineId = fileInfoPtr->sequences;
		}
	      else
		pattern->currentDeflineId =
		  pattern->currentDeflineId->next;
	      
	      if (pattern->currentDeflineId == NULL)
		{
		  errPtr = Ali_AddError (&(fileInfoPtr->errors),
					 ERR_DEFLINE_WITH_NO_ID,
					 defLinePtr->definitions);
		  errPtr->rowNum = defLinePtr->rowNum;
		  isValidPattern = FALSE;
		  break;
		}
	      else
		{
		  pattern->currentDeflineId->defline =
		    defLinePtr->definitions;
		}
	    }
	  pattern->lastRow = currentRow;
	}

      /* Process Other lines */

      else if (currentRow->choice == ALI_OTHERLINE)
	{
	  otherLinePtr = (OtherLineInfoPtr) currentRow->data.ptrvalue;
	  if (otherLinePtr->id != NULL)
	    {
	      isValidPattern = s_ProcessContigId (otherLinePtr->id,
						  pattern,
						  fileInfoPtr);
	      if (isValidPattern == FALSE)
		break;
	    }
	  pattern->lastRow = currentRow;
	}

      currentRow = currentRow->next;
    }

  /* If the last sequence is too short, mark */
  /* it as a maybe.                          */
  
  if (pattern->lastRow->choice == ALI_SEQLINE)
    {
      if (s_configInfo.useMaybes == FALSE)
	{
	  pattern->maybesFound = TRUE;
	  if (pattern->currentId->length < pattern->foundInfo.seqLength)
	    {
	      seqLinePtr = (SeqLineInfoPtr)pattern->lastRow->data.ptrvalue;
	      seqLinePtr->maybe = TRUE;
	      nextToLastId = NULL;
	      lastId = fileInfoPtr->sequences;
	      while (lastId->next != NULL)
		{
		  nextToLastId = lastId;
		  lastId = lastId->next;
		}
	      MemFree(lastId);
	      if (nextToLastId == NULL)
		fileInfoPtr->sequences = NULL;
	      else
		nextToLastId->next = NULL;
	    }
	}
      else
	{
	  if (pattern->currentId->length < pattern->foundInfo.seqLength)
	    {
	      errPtr = Ali_AddError (&(fileInfoPtr->errors),
				     ERR_SEQUENCE_TOO_SHORT,
				     pattern->currentId->id,
				     pattern->foundInfo.seqLength,
				     pattern->currentId->length);
	      isValidPattern = FALSE;
	    }
	}
    }

  /* If we found one defline, then */
  /* make sure they were all there */

  if (firstDefline == FALSE)
    {
      lastId = fileInfoPtr->sequences;
      if (lastId != NULL)
	{
	  while (lastId->next != NULL)
	    lastId = lastId->next;
	  if (lastId->defline == NULL)
	    {
	      errPtr = Ali_AddError (&(fileInfoPtr->errors),
				     ERR_ID_WITH_NO_DEFLINE,
				     lastId->id);
	      isValidPattern = FALSE;
	    }
	}
    }

  /* If pattern not found, return failure */

  if (!isValidPattern)
    {
      MemFree (pattern);
      return FALSE;
    }

  /* Check for inconsistant declarations ... */

  /* ... of file type */

  if (s_configInfo.declaredInfo.contigOrInter == ALI_INTERLEAVED)
    {
      errPtr = Ali_AddError (&(fileInfoPtr->errors), ERR_NOT_INTERLEAVED);
      errPtr->level = LEVEL_WARNING;
    }

  /* ... of number of sequences */

  if ((s_configInfo.declaredInfo.idCount != 0) &&
      (s_configInfo.declaredInfo.idCount != pattern->foundInfo.idCount))
    {
      errPtr = Ali_AddError (&(fileInfoPtr->errors), ERR_ID_COUNT_MISMATCH,
			     pattern->foundInfo.idCount,
			     s_configInfo.declaredInfo.idCount);
      errPtr->level = LEVEL_WARNING;
    }

  /* ... of sequence length */

  if ((s_configInfo.declaredInfo.seqLength != 0) &&
      (s_configInfo.declaredInfo.seqLength != pattern->foundInfo.seqLength))
    {
      errPtr = Ali_AddError (&(fileInfoPtr->errors),ERR_SEQ_LENGTH_MISMATCH,
			     pattern->foundInfo.seqLength,
			     s_configInfo.declaredInfo.seqLength);
      errPtr->level = LEVEL_WARNING;
    }

  /* If we have some possibly bad sequences that */
  /* weren't used, process them seperately.      */

  if (pattern->maybesFound == TRUE)
    fileInfoPtr->maybes = s_ProcessMaybes (rowList);

  /* Clean up and return successfully */

  if (pattern->currentId != NULL)
    {
      MemFree (pattern);
      return FALSE;
    }
  else
    {
      MemFree (pattern);
      return TRUE;
    }
}

/*=========================================================================*/
/*                                                                         */
/* Ali_AddError ()                                                         */
/*                                                                         */
/*=========================================================================*/

ErrInfoPtr Ali_AddError (ErrInfoPtr PNTR errorListPtr,
			 Int4            iError,
			 ...)
{
  ErrInfoPtr newError;
  ErrInfoPtr lastError;
  va_list    argPtr;
  CharPtr    seqId;
  CharPtr    seqStr;
  Int4       seqLength;
  Int4       prevSeqLength;
  CharPtr    defLineStr;
  Int4       foundCount;
  Int4       declaredCount;
  Int4       foundLen;
  Int4       declaredLen;
  Int4       sequenceCount;
  Int4       errorCount;
  Int4       invalidChar;

  static Int4 count = 0;

  count++;

  /* Create a new error record */

  newError = (ErrInfoPtr) MemNew (sizeof(ErrInfo));
  newError->errNum    = iError;
  newError->level     = LEVEL_ERROR;
  newError->rowNum    = 0;
  newError->extraInfo = NULL;
  newError->next      = NULL;

  /* Build the error message text */

  va_start (argPtr, iError);

  switch (iError)
    {
    case ERR_ID_WITHOUT_SEQ :
      seqId = va_arg (argPtr, CharPtr);
      newError->info = (CharPtr) MemNew (strlen (seqId) + 80);
      sprintf (newError->info, "Unable to match ID %s to any sequence", seqId);
      break;
    case ERR_SEQ_WITHOUT_ID :
      seqStr = va_arg (argPtr, CharPtr);
      newError->info = (CharPtr) MemNew (strlen (seqStr) + 80);
      sprintf (newError->info, "There is no ID for the sequence:\n%s", seqStr);
      break;
    case ERR_DUPLICATE_IDS :
      seqId = va_arg (argPtr, CharPtr);
      newError->info = (CharPtr) MemNew (strlen (seqId) + 80);
      sprintf (newError->info, "Duplicate ID: %s is used more than once",
	       seqId);
      break;
    case ERR_SEQUENCE_TOO_SHORT :
      seqId = va_arg (argPtr, CharPtr);
      prevSeqLength = va_arg (argPtr, Int4);
      seqLength     = va_arg (argPtr, Int4);
      newError->info = (CharPtr) MemNew (strlen (seqId) + 256);
      sprintf (newError->info,
	       "Sequence %s is shorter (%d characters) than the preceding"
	       " sequences (%d characters)", seqId, seqLength, prevSeqLength);
      break;
    case ERR_SEQUENCE_TOO_LONG :
      seqId         = va_arg (argPtr, CharPtr);
      prevSeqLength = va_arg (argPtr, Int4);
      seqLength     = va_arg (argPtr, Int4);
      newError->info = (CharPtr) MemNew (strlen (seqId) + 256);
      sprintf (newError->info,
	       "Sequence %s is longer (%d characters) than the preceding"
	       " sequences (%d characters)", seqId, seqLength, prevSeqLength);
      break;
    case ERR_OUT_OF_MEMORY :
      newError->info = (CharPtr) MemNew (80);
      sprintf (newError->info, "Out of memory -- memory allocation failed");
      break;
    case ERR_ID_NO_PRECEDING_SEQ :
      seqId = va_arg (argPtr, CharPtr);
      newError->info = (CharPtr) MemNew (strlen (seqId) + 100);
      sprintf (newError->info,
	       "ID %s is probably invalid -- it is not immediately"
	       " preceded by a sequence", seqId);
      break;
    case ERR_NOT_INTERLEAVED :
      newError->info = (CharPtr) MemNew (80);
      sprintf (newError->info, "File is declared to be interleaved,"
	       " but is contiguous");
      break;
    case ERR_NOT_CONTIGUOUS :
      newError->info = (CharPtr) MemNew (80);
      sprintf (newError->info, "File is declared to be contiguous,"
	       " but is interleaved");
      break;
    case ERR_NO_SEQUENCES_FOUND :
      newError->info = (CharPtr) MemNew (80);
      sprintf (newError->info, "No sequences were found in the file");
      break;
    case ERR_ID_COUNT_MISMATCH :
      foundCount = va_arg (argPtr, Int4);
      declaredCount = va_arg (argPtr, Int4);
      newError->info = (CharPtr) MemNew (128);
      sprintf (newError->info, "The number of sequences found (%d) doesn't"
	       " match the number declared (%d)", foundCount, declaredCount);
      break;
    case ERR_SEQ_LENGTH_MISMATCH :
      foundLen = va_arg (argPtr, Int4);
      declaredLen = va_arg (argPtr, Int4);
      newError->info = (CharPtr) MemNew (128);
      sprintf (newError->info, "The length (%d) of the sequences found doesn't"
	       " match the declared length (%d)", foundLen, declaredLen);
      break;
    case ERR_DEFLINE_WITH_NO_ID :
      defLineStr = va_arg (argPtr, CharPtr);
      newError->info = (CharPtr) MemNew (strlen (defLineStr) + 100);
      sprintf (newError->info, "Unable to match the following definition"
	       " line to any sequence :\n%s", defLineStr);
      break;
    case ERR_ID_WITH_NO_DEFLINE :
      seqId = va_arg (argPtr, CharPtr);
      newError->info = (CharPtr) MemNew (strlen (seqId) + 80);
      sprintf (newError->info, "Could not find a defline for the following"
	       " sequence :\n%s", seqId);
      break;
    case ERR_INVALID_DEFLINE :
      defLineStr = va_arg (argPtr, CharPtr);
      invalidChar = va_arg (argPtr, Int4);
      newError->info = (CharPtr) MemNew (strlen (defLineStr) + 100);
      sprintf (newError->info, "Invalid definitions line (illegal char '%c'):\n%s",
	       (Char) invalidChar, defLineStr);
      break;
    case ERR_DEFLINE_NODEFS :
      defLineStr = va_arg (argPtr, CharPtr);
      newError->info = (CharPtr) MemNew (strlen (defLineStr) + 100);
      sprintf (newError->info, "There is no source info enclosed by"
	       " brackets on the definition line :\n%s", defLineStr);
      break;
    case ERR_GLOBAL_DEFLINE_NODEFS :
      sequenceCount = va_arg (argPtr, Int4);
      newError->info = (CharPtr) MemNew (128);
      sprintf (newError->info, "All %d of the file's definition lines are"
	       " missing source info enclosed in [] brackets", sequenceCount);
      newError->level = LEVEL_MULTI;
      break;
    case ERR_MULTI_DEFLINE_NODEFS :
      errorCount = va_arg (argPtr, Int4);
      newError->info = (CharPtr) MemNew (128);
      sprintf (newError->info, "%d of the file's definition lines are"
	       " missing source info enclosed in [] brackets", errorCount);
      newError->level = LEVEL_MULTI;
      break;
    default:
      newError->info = (CharPtr) MemNew (32);
      sprintf (newError->info, "Unknown Error");
      break;
    }

  va_end (argPtr);

  /* Add it to the end of the linked list */

  if (*errorListPtr == NULL)
    *errorListPtr = newError;
  else
    {
      lastError = *errorListPtr;
      while (lastError->next != NULL)
	lastError = lastError->next;
      lastError->next = newError;
    }

  /* Return a pointer to new record for easy access */

  return newError;
}

/*=========================================================================*/
/*                                                                         */
/* s_AnalyzeContents () -                                                  */
/*                                                                         */
/*=========================================================================*/

static Boolean s_AnalyzeContents (ValNodePtr       rowList,
				  AlignFileDataPtr fileInfoPtr)
{
  Int2    idCount;
  Boolean result;

  if (s_IsInterleaved (rowList, &idCount))
    {
      fileInfoPtr->info->contigOrInter = ALI_INTERLEAVED;
      result = s_AnalyzeInterleaved (rowList, fileInfoPtr, idCount);
    }
  else 
    {
      fileInfoPtr->info->contigOrInter = ALI_CONTIGUOUS;
      result = s_AnalyzeContiguous (rowList, fileInfoPtr);
    }

  return result;
}

/*=========================================================================*/
/*                                                                         */
/* SeqLineReEval () - Re-evaluate a line after forcing the first 'word'    */
/*                    to be an ID.                                         */
/*                                                                         */
/*=========================================================================*/

SeqLineInfoPtr SeqLineReEval (SeqLineInfoPtr seqLinePtr)
{
  CharPtr        seqStr;
  CharPtr        idStr;
  CharPtr        oldStr;
  SeqLineInfoPtr newSeqLinePtr;

  /* If the line is already split up, */
  /* then this won't work.            */

  if ((seqLinePtr->sequence != NULL) && (seqLinePtr->id != NULL))
    return NULL;

  /* Determine the string that we're splitting up */

  if (seqLinePtr->sequence != NULL)
    oldStr = seqLinePtr->sequence;
  else if (seqLinePtr->id != NULL)
    oldStr = seqLinePtr->id;
  else
    return NULL;

  /* If there's only one 'word' then */
  /* we can't split it.              */

  if (StringLen (oldStr) == seqLinePtr->firstWordLen)
    return NULL;

  /* Allocate mem for the new strings */

  seqStr = (CharPtr) MemNew (StringLen (oldStr) -
			     seqLinePtr->firstWordLen + 1);
  if (seqStr == NULL)
    return NULL;
  idStr = (CharPtr) MemNew (seqLinePtr->firstWordLen + 1);
  if (idStr == NULL)
    {
      MemFree (seqStr);
      return NULL;
    }

  /* Break up the existing string */

  StringNCpy(idStr, oldStr, seqLinePtr->firstWordLen);
  idStr[seqLinePtr->firstWordLen] = '\0';
  StringCpy(seqStr, oldStr + seqLinePtr->firstWordLen);

  /* Return successfully */

  newSeqLinePtr = (SeqLineInfoPtr) MemNew (sizeof (SeqLineInfo));
  newSeqLinePtr->sequence     = seqStr;
  newSeqLinePtr->id           = idStr;
  newSeqLinePtr->rowNum       = seqLinePtr->rowNum;
  newSeqLinePtr->type         = Ali_SeqLineGetType(seqStr, &s_configInfo);
  newSeqLinePtr->maybe        = seqLinePtr->maybe;
  newSeqLinePtr->firstWordLen = seqLinePtr->firstWordLen;

  return newSeqLinePtr;
}

/*=========================================================================*/
/*                                                                         */
/* s_IsExistingId () -- Determine if the given ID is one that has already  */
/*                      been added to the linked list of IDs.              */
/*                                                                         */
/*=========================================================================*/

static s_IsExistingId (AlignFileDataPtr fileInfoPtr,
		       CharPtr          testIdStr)
{
  IdInfoPtr  idListPtr = NULL;

  /* See if this ID already exists */
  
  idListPtr = fileInfoPtr->sequences;
  while (idListPtr != NULL)
    {
      if (StringCmp(idListPtr->id,testIdStr) == 0)
	return TRUE;
      idListPtr = idListPtr->next;
    }
  
  /* If we made it to here, then */
  /* the ID wasn't found.        */

  return FALSE;
}

/*=========================================================================*/
/*                                                                         */
/* s_CheckContext ()                                                       */
/*                                                                         */
/*=========================================================================*/

static Boolean s_CheckContext (ValNodePtr       rowList,
			       AlignFileDataPtr fileInfoPtr)
{
  ValNodePtr       currentRow;
  ValNodePtr       lastRow;
  CharPtr          idStr;
  SeqLineInfoPtr   seqLinePtr;
  SeqLineInfoPtr   reEvalSeqPtr;
  SeqLineInfoPtr   prevSeqLinePtr = NULL;
  DefLineInfoPtr   defLinePtr;
  OtherLineInfoPtr otherLinePtr;
  Int2             patternSeqType;
  ErrInfoPtr       errPtr;
  Boolean          changesMade;
  Int4             currLen;
  Int4             prevLen;

  do  /* Until no changes are made */
    {
      currentRow     = rowList;
      lastRow        = NULL;
      patternSeqType = ALI_AMBIGUOUS;

      changesMade = FALSE;
      while (currentRow != NULL)
	{
	  if (currentRow->choice == ALI_SEQLINE)
	    {
	      seqLinePtr = (SeqLineInfoPtr) currentRow->data.ptrvalue;
	      
	      if ((seqLinePtr->maybe == FALSE) ||
		  (seqLinePtr->maybe == TRUE) &&
		  (s_configInfo.useMaybes == TRUE))
		{
		  /* If there is an ID, make sure that it  */
		  /* immediately precedes a sequence line. */
		  
		  if (seqLinePtr->id != NULL)
		    {
		      if ((seqLinePtr->sequence == NULL) &&
			  (s_GetRowSeqString(currentRow->next) == NULL))
			{
			  Ali_ChangeRowToOther (currentRow);
			  changesMade = TRUE;
			  continue;
			}
		    }

		  /* Check for an ID that was accidentally lumped */
		  /* in with a sequence due to being composed     */
		  /* entirely of sequence characters.             */

		  if (prevSeqLinePtr != NULL)
		    {
		      currLen = StringLen (seqLinePtr->sequence);
		      prevLen = StringLen (prevSeqLinePtr->sequence);

		      if ((currLen > prevLen) && 
			  (seqLinePtr->id == NULL) &&
			  (prevSeqLinePtr->id != NULL))
			{
			  reEvalSeqPtr = SeqLineReEval (seqLinePtr);

			  if (reEvalSeqPtr != NULL)
			    {
			      currLen = StringLen (reEvalSeqPtr->sequence);
			  
			      /* If the new seqline fits better, use it */
			  
			      if (currLen == prevLen)
				{
				  MemFree(seqLinePtr->sequence);
				  MemFree(seqLinePtr->id);
				  MemFree(seqLinePtr);
				  currentRow->data.ptrvalue = reEvalSeqPtr;
				  continue;
				}
			      else
				{
				  MemFree(reEvalSeqPtr->sequence);
				  MemFree(reEvalSeqPtr->id);
				  MemFree(reEvalSeqPtr);
				}
			    }
			}
		    }
		  
		  /* If there's an established pattern of sequence */
		  /* type, then match the current line against it. */
		  /* Otherwise, set the pattern.                   */
		  
		  if (seqLinePtr->type != ALI_AMBIGUOUS)
		    {
		      if (patternSeqType != ALI_AMBIGUOUS)
			{
			  if (patternSeqType != seqLinePtr->type)
			    {
			      reEvalSeqPtr = SeqLineReEval (seqLinePtr);
			      if ((reEvalSeqPtr == NULL) ||
				  ((reEvalSeqPtr != NULL) &&
				   (patternSeqType != reEvalSeqPtr->type)))
				{
				  if (reEvalSeqPtr != NULL)
				    {
				      MemFree(reEvalSeqPtr->sequence);
				      MemFree(reEvalSeqPtr->id);
				      MemFree(reEvalSeqPtr);
				    }
				  Ali_ChangeRowToOther (currentRow);
				  changesMade = TRUE;
				  continue;
				}
			      else
				{
				  MemFree(seqLinePtr->sequence);
				  MemFree(seqLinePtr->id);
				  MemFree(seqLinePtr);
				  currentRow->data.ptrvalue = reEvalSeqPtr;
				  continue;
				}
			    }
			}
		      else 
			patternSeqType = seqLinePtr->type;
		    }

		  /* */
		  
		  prevSeqLinePtr = seqLinePtr;

		}
	      lastRow = currentRow;
	    }
	  
	  else if (currentRow->choice == ALI_DEFLINE)
	    {
	      /* If there is an ID, make sure that it  */
	      /* immediately precedes a sequence line. */
	      
	      defLinePtr = (DefLineInfoPtr) currentRow->data.ptrvalue;
	      if ((defLinePtr->id != NULL) &&
		  (s_IsExistingId(fileInfoPtr, defLinePtr->id) == FALSE) &&
		  (s_GetRowSeqString(currentRow->next) == NULL))
		{
		  Ali_ChangeRowToOther (currentRow);
		  changesMade = TRUE;
		  continue;
		}
	      lastRow = currentRow;
	    }
	  
	  else if (currentRow->choice == ALI_OTHERLINE)
	    {
	      /* If there is an ID, make sure that it  */
	      /* immediately precedes a sequence line. */
	      
	      otherLinePtr = (OtherLineInfoPtr) currentRow->data.ptrvalue;
	      if (otherLinePtr->id != NULL)
		{
		  if (s_GetRowSeqString(currentRow->next) == NULL)
		    {
		      otherLinePtr->other = otherLinePtr->id;
		      otherLinePtr->id    = NULL;
		      changesMade = TRUE;
		    }
		}
	      lastRow = currentRow;
	    }
	  
	  currentRow = currentRow->next;
	}
    } while (changesMade == TRUE);

  /* Check for a dangling ID */

  if ((lastRow != NULL) &&
      ((idStr = s_GetRowIdString (lastRow)) != NULL) &&
      (s_GetRowSeqString (lastRow) == NULL))
    {
      errPtr = Ali_AddError (&(fileInfoPtr->errors),
			     ERR_ID_WITHOUT_SEQ,
			     idStr);
      return FALSE;
    }

  /* Return successfully */

  return TRUE;
}

/*=========================================================================*/
/*                                                                         */
/* s_SortErrors () -- Sort errors by level, so that the most severe appear */
/*                    first.                                               */
/*                                                                         */
/*   NOTE : Does a lame bubblesort, which nevertheless should be fast      */
/*          enough for the relatively small linked lists we're dealing     */
/*          with here.                                                     */
/*                                                                         */
/*=========================================================================*/

static void s_SortErrors (AlignFileDataPtr fileInfoPtr)
{
  Boolean  swapMade  = TRUE;
  ErrInfoPtr prevPtr = NULL;
  ErrInfoPtr nextPtr = NULL;
  ErrInfoPtr errPtr  = NULL;

  while (swapMade == TRUE)
    {
      swapMade = FALSE;
      errPtr = fileInfoPtr->errors;
      while (errPtr->next != NULL)
	{
	  nextPtr = errPtr->next;
	  if (errPtr->level > nextPtr->level)
	    {
	      swapMade = TRUE;

	      /* Remove the error from the list */

	      if (errPtr == fileInfoPtr->errors)
		fileInfoPtr->errors = nextPtr;
	      else
		prevPtr->next = nextPtr;

	      /* Then re-insert it after the following error */

	      errPtr->next = nextPtr->next;
	      nextPtr->next = errPtr;

	      /* The old next error is now the previous error */

	      prevPtr = nextPtr;
	    }
	  else
	    {
	      prevPtr = errPtr;
	      errPtr = errPtr->next;
	    }
	}
    }

  return;
}

/*=========================================================================*/
/*                                                                         */
/* s_ReplaceUWithT () -- Replace all the Us in a nucleotide sequence with  */
/*                       Ns.                                               */
/*                                                                         */
/*=========================================================================*/

static void s_ReplaceUWithT (AlignFileDataPtr fileInfoPtr)
{
  IdInfoPtr  seqPtr = NULL;
  SeqPartPtr seqPart = NULL;
  CharPtr    seqString;
  Int4       i;

  seqPtr = fileInfoPtr->sequences;
  while (seqPtr != NULL)
    {
      seqPart = seqPtr->sequence;
      while (seqPart != NULL)
	{
	  seqString = seqPart->sequence;
	  for (i = 0; seqString[i] != '\0'; i++)
	    if (seqString[i] == 'U')
	      seqString[i] = 'T';
	    else if (seqString[i] == 'u')
	      seqString[i] = 't';
	  seqPart = seqPart->next;
	}
      seqPtr = seqPtr->next;
    }

}

/*=========================================================================*/
/*                                                                         */
/* s_AnalyzeErrors () -- Look for patterns in the errors that can be used  */
/*                       to create more general, higher-level errors       */
/*                       instead.                                          */
/*                                                                         */
/*=========================================================================*/

static void s_AnalyzeErrors (AlignFileDataPtr fileInfoPtr)
{
  Int4       seqCount = 0;
  Int4       defCount = 0;
  Int4       errCount = 0;
  IdInfoPtr  seqPtr = NULL;
  ErrInfoPtr errPtr = NULL;
  ErrInfoPtr prevErrPtr = NULL;
  ErrInfoPtr nextErrPtr = NULL;

  if (fileInfoPtr->errors == NULL)
    return;

  /* Get counts of sequences and deflines */

  seqPtr = fileInfoPtr->sequences;
  while (seqPtr != NULL)
    {
      seqCount++;
      if (seqPtr->defline != NULL)
	defCount++;
      seqPtr = seqPtr->next;
    }

  /* Check for "missing bracket" defline errors */

  errPtr = fileInfoPtr->errors;
  while (errPtr != NULL)
    {
      if (errPtr->errNum == ERR_DEFLINE_NODEFS)
	  errCount++;
      errPtr = errPtr->next;
    }

  /* If ALL deflines have missing bracket errors */
  /* then replace the msgs with one global msg   */

  if (errCount == seqCount)
    {
      if (s_configInfo.errExpandLevel != ALI_ERRMSG_EXPAND_ALL)
	{
	  errPtr = fileInfoPtr->errors;
	  while (errPtr != NULL)
	    {
	      nextErrPtr = errPtr->next;
	      if (errPtr->errNum == ERR_DEFLINE_NODEFS)
		{
		  if (errPtr == fileInfoPtr->errors)
		    {
		      fileInfoPtr->errors = fileInfoPtr->errors->next;
		      s_FreeErrorNode (errPtr);
		      errPtr = NULL;
		    }
		  else
		    {
		      prevErrPtr->next = nextErrPtr;
		      s_FreeErrorNode (errPtr);
		      errPtr = NULL;
		    }
		}
	      else
		prevErrPtr = errPtr;
	      errPtr = nextErrPtr;
	    }
	}
      Ali_AddError (&(fileInfoPtr->errors), ERR_GLOBAL_DEFLINE_NODEFS,
		    seqCount);
    }

  /* If SOME deflines have missing bracket errors */
  /* then replace the msgs with one global msg    */

  else if (errCount > 1)
    {
      if (s_configInfo.errExpandLevel == ALI_ERRMSG_EXPAND_NONE)
	{
	  errPtr = fileInfoPtr->errors;
	  while (errPtr != NULL)
	    {
	      nextErrPtr = errPtr->next;
	      if (errPtr->errNum == ERR_DEFLINE_NODEFS)
		{
		  if (errPtr == fileInfoPtr->errors)
		    {
		      fileInfoPtr->errors = fileInfoPtr->errors->next;
		      s_FreeErrorNode (errPtr);
		      errPtr = NULL;
		    }
		  else
		    {
		      prevErrPtr->next = nextErrPtr;
		      s_FreeErrorNode (errPtr);
		      errPtr = NULL;
		    }
		}
	      else
		prevErrPtr = errPtr;
	      errPtr = nextErrPtr;
	    }
	}
      Ali_AddError (&(fileInfoPtr->errors), ERR_MULTI_DEFLINE_NODEFS,
		    errCount);
    }

  /* Finally, sort the errors by type */

  s_SortErrors (fileInfoPtr);

  return;

}

/*=========================================================================*/
/*                                                                         */
/* Ali_Read ()                                                             */
/*                                                                         */
/*=========================================================================*/

AlignFileDataPtr Ali_Read (FILE PNTR alignFilePtr)
{
  ValNodePtr        rowList = NULL;
  AlignFileDataPtr  fileInfoPtr;
  ErrInfoPtr        errorList = NULL;

  /* Check parameters */

  if (alignFilePtr == NULL)
    return FALSE;

  /* Initialize */

  fileInfoPtr = (AlignFileDataPtr) MemNew (sizeof(AlignFileData));
  fileInfoPtr->sequences = NULL;
  fileInfoPtr->maybes    = NULL;
  fileInfoPtr->errors    = NULL;
  fileInfoPtr->info = (ParsedInfoPtr) MemNew (sizeof (ParsedInfo));
  if (fileInfoPtr->info == NULL)
    {
      Ali_AddError (&(fileInfoPtr->errors), ERR_OUT_OF_MEMORY);
      Ali_Free (fileInfoPtr);
      return NULL;
    }
  fileInfoPtr->info->missingChar   = NULL;
  fileInfoPtr->info->gapChar       = NULL;
  fileInfoPtr->info->unalignedChar = NULL;
  
  if (s_configurationSet == FALSE)
    Ali_SetConfig (NULL, ALI_SET_DEFAULTS);

  /* Read in and parse each row */

  rowList = Ali_ReadLines (alignFilePtr, &errorList, &s_configInfo, fileInfoPtr);
  fileInfoPtr->errors = errorList;

  if (rowList == NULL)
    return fileInfoPtr;

  /* Make first pass to adjust the rows based on context */

  if (s_CheckContext(rowList, fileInfoPtr) != TRUE)
    return fileInfoPtr;

  s_DisplayRowList (rowList, s_configInfo.debugLevel);

  /* Analyze the IDs and sequences for consistancy */

  s_AnalyzeContents (rowList, fileInfoPtr);

  if (fileInfoPtr->sequences == NULL)
    Ali_AddError (&(fileInfoPtr->errors), ERR_NO_SEQUENCES_FOUND);

  /* Analyze the errors to see if they can be */
  /* combined into more general global errors */

  if (fileInfoPtr->errors != NULL)
    s_AnalyzeErrors (fileInfoPtr);

  /* Return the missing, gap, and unaligned chars used */

  fileInfoPtr->info->missingChar = (CharPtr) MemNew(16);
  StringCpy (fileInfoPtr->info->missingChar, s_configInfo.missingChar);

  fileInfoPtr->info->gapChar = (CharPtr) MemNew(16);
  StringCpy (fileInfoPtr->info->gapChar, s_configInfo.gapChar);

  fileInfoPtr->info->unalignedChar = (CharPtr) MemNew(16);
  StringCpy (fileInfoPtr->info->unalignedChar, s_configInfo.unalignedChar);

  /* If these are nucleotide sequences, then */
  /* replace all 'U's with 'T's.             */

  s_ReplaceUWithT (fileInfoPtr);

  /* Clean up and return successfully */
  
  s_FreeRowList_Safe (rowList);
  return fileInfoPtr;
}

/***************************************************************************
*
*  section to convert AlignFileDataPtr content into seqalign/seqentry
*  structures
*
***************************************************************************/
typedef struct tinyinfo {
   Int4  n;
   struct tinyinfo PNTR next;
} ALI_TinyInfo, PNTR ALI_TinyInfoPtr;


static Boolean is_gap_char(Char c, CharPtr gapChar)
{
   if (StrChr(gapChar, c) != NULL)
      return TRUE;
   return FALSE;
}

static int LIBCALLBACK ALI_SortTips(VoidPtr ptr1, VoidPtr ptr2)
{
   ALI_TinyInfoPtr  tip1;
   ALI_TinyInfoPtr  tip2;

   tip1 = *((ALI_TinyInfoPtr PNTR)ptr1);
   tip2 = *((ALI_TinyInfoPtr PNTR)ptr2);
   if (tip1->n > tip2->n)
      return 1;
   if (tip1->n < tip2->n)
      return -1;
   return 0;
}

static Boolean is_valid_seq(Char c, CharPtr missingChar, CharPtr gapChar)
{
  if (StrChr("\0", c))
    return FALSE;
  if (StrChr(missingChar, c) != NULL)
    return TRUE;
  if (StrChr(gapChar, c) != NULL)
    return TRUE;
  if (IS_ALPHA(c))
    return TRUE;
  if (c == '-')
    return TRUE;
  if (c == '?')
    return TRUE;
  return FALSE;
}

static Boolean is_missing(Char c, CharPtr missingChar)
{
   if (StrChr(missingChar, c) != NULL)
      return TRUE;
   else
      return FALSE;
}

static SeqAlignPtr ALI_MakeSeqAlign(AlignFileDataPtr afp, CharPtr PNTR PNTR stringsptr, Int4Ptr numseq, CharPtr PNTR PNTR deflineptr)
{
   Int4             alnlen;
   CharPtr          buf;
   CharPtr          c;
   Int4             ctr;
   Int4             ctr_prev;
   CharPtr          PNTR deflines;
   DenseSegPtr      dsp;
   Int4             i;
   IdInfoPtr        id_head;
   IdInfoPtr        iip;
   Boolean          ingap;
   Boolean          isgap;
   Int4             j;
   Int4             last;
   Int4             len;
   Int4             maxlen;
   Int4             numtips;
   SeqAlignPtr      sap;
   SeqPartPtr       seq;
   SeqIdPtr         sip;
   SeqIdPtr         sip_prev;
   CharPtr          PNTR strings;
   Char             text[100];
   ALI_TinyInfoPtr  tip;
   ALI_TinyInfoPtr  tip_head;
   ALI_TinyInfoPtr  tip_prev;
   ALI_TinyInfoPtr  PNTR tiparray;

   if (afp->info == NULL)
   {
      ErrPostEx(SEV_ERROR, 0, 0, "NULL afp->info -- alignment not read correctly\n");
      return NULL;
   }
   i = 0;
   id_head = afp->sequences;
   iip = id_head;
   while (iip != NULL)
   {
      i++;
      if (iip->id == NULL)
      {
         sprintf(text, "No id read for sequence %d\n", i);
         ErrPostEx(SEV_ERROR, 0, 0, text);
         return NULL;
      }
      iip = iip->next;
   }
   sap = SeqAlignNew();
   sap->type = SAT_PARTIAL;
   sap->segtype = SAS_DENSEG;
   sap->dim = i;
   dsp = DenseSegNew();
   dsp->dim = i;
   strings = (CharPtr PNTR)MemNew(i*sizeof(CharPtr));
   deflines = (CharPtr PNTR)MemNew(i*sizeof(CharPtr));
   tip_head = tip_prev = NULL;
   iip = id_head;
   maxlen = 0;
   tip_head = tip_prev = NULL;
   numtips = 0;
   alnlen = 0;
   i = 1;
   while (iip != NULL)
   {
      len = 0;
      ctr = 0;
      seq = iip->sequence;
      if (seq == NULL || seq->sequence == NULL)
      {
         sprintf(text, "Error in reading sequence %d -- no sequence characters read\n", i);
         ErrPostEx(SEV_ERROR, 0, 0, text);
         return NULL;
      }
      c = seq->sequence;
      if (is_gap_char(*c, afp->info->gapChar))
         ingap = TRUE;
      else
         ingap = FALSE;
      while (seq != NULL)
      {
         c = seq->sequence;
         if (c == NULL)
         {
            sprintf(text, "Error in reading sequence %d -- no sequence characters read\n", i);
            ErrPostEx(SEV_ERROR, 0, 0, text);
            return NULL;
         }
         while (is_valid_seq(*c, afp->info->missingChar, afp->info->gapChar))
         {
            if (is_gap_char(*c, afp->info->gapChar) && !ingap)
            {
               tip = (ALI_TinyInfoPtr)MemNew(sizeof(ALI_TinyInfo));
               tip->n = ctr;
               if (tip_head != NULL)
               {
                  tip_prev->next = tip;
                  tip_prev = tip;
               } else
                  tip_head = tip_prev = tip;
               ingap = TRUE;
               numtips++;
            } else if (!is_gap_char(*c, afp->info->gapChar) && ingap)
            {
               tip = (ALI_TinyInfoPtr)MemNew(sizeof(ALI_TinyInfo));
               tip->n = ctr;
               if (tip_head != NULL)
               {
                  tip_prev->next = tip;
                  tip_prev = tip;
               } else
                  tip_head = tip_prev = tip;
               ingap = FALSE;
               numtips++;
            }
            if (!is_gap_char(*c, afp->info->gapChar))
               len++;
            ctr++;
            c++;
         }
         seq = seq->next;
      }
      if (ctr > alnlen)
         alnlen = ctr;
      if (len > maxlen)
         maxlen = len;
      iip = iip->next;
      i++;
   }
   if (tip_head == NULL) /* this is a gapless alignment */
   {
      dsp->numseg = 1;
      dsp->starts = (Int4Ptr)MemNew((dsp->dim)*sizeof(Int4));
      dsp->lens = (Int4Ptr)MemNew(sizeof(Int4));
      dsp->strands = (Uint1Ptr)MemNew((dsp->dim)*sizeof(Uint1));
      for (i=0; i<dsp->dim; i++)
      {
         dsp->strands[i] = Seq_strand_plus;
      }
      dsp->lens[0] = id_head->length;
      /* all the starts are 0 anyway, just leave them and get the ids & seqs */
      iip = id_head;
      sip_prev = NULL;
      buf = (CharPtr)MemNew((maxlen+1)*sizeof(Char));
      i = 0;
      while (iip != NULL)
      {
         sip = MakeSeqID(iip->id);
         deflines[i] = StringSave(iip->defline);
         if (sip_prev != NULL)
         {
            sip_prev->next = sip;
            sip_prev = sip;
         } else
            dsp->ids = sip_prev = sip;
         seq = iip->sequence;
         for (ctr = 0; ctr<(maxlen+1); ctr++)
         {
            buf[ctr] = '\0';
         }
         ctr = 0;
         while (seq != NULL)
         {
            c = seq->sequence;
            while (is_valid_seq(*c, afp->info->missingChar, afp->info->gapChar))
            {
               if (is_missing(*c, afp->info->missingChar))
                  buf[ctr] = 'N';
               else
                  buf[ctr] = *c;
               ctr++;
               c++;
            }
            seq = seq->next;
         }
         strings[i] = StringSave(buf);
         iip = iip->next;
         i++;
      }
      sap->segs = (Pointer)dsp;
      MemFree(buf);
      *numseq = dsp->dim;
      *stringsptr = strings;
      *deflineptr = deflines;
      return sap;
   }
   /* now all the segment boundaries have been collected, so sort them */
   tiparray = (ALI_TinyInfoPtr PNTR)MemNew(numtips*sizeof(ALI_TinyInfoPtr));
   i = 0;
   tip = tip_head;
   while (tip != NULL)
   {
      tiparray[i] = tip;
      i++;
      tip = tip->next;
   }
   HeapSort(tiparray, numtips, sizeof(ALI_TinyInfoPtr), ALI_SortTips);
   dsp->numseg = 2; /* one for the first, one for the last */
   for (i=1; i<numtips; i++)
   {
      if (tiparray[i]->n != tiparray[i-1]->n)
         dsp->numseg++;
   }
   dsp->starts = (Int4Ptr)MemNew((dsp->dim)*(dsp->numseg)*sizeof(Int4));
   dsp->lens = (Int4Ptr)MemNew((dsp->numseg)*sizeof(Int4));
   last = 0;
   j=0;
   dsp->lens[0] = tiparray[0]->n;
   last = tiparray[0]->n;
   j++;
   for (i=1; i<numtips; i++)
   {
      if (tiparray[i]->n != tiparray[i-1]->n)
      {
         dsp->lens[j] = tiparray[i]->n-last;
         last = tiparray[i]->n;
         j++;
      }
   }
   dsp->lens[j] = alnlen - last;
   dsp->strands = (Uint1Ptr)MemNew((dsp->dim)*(dsp->numseg)*sizeof(Uint1));
/* do we have any strand info to the contrary? */
   for (i=0; i<(dsp->dim)*(dsp->numseg); i++)
   {
      dsp->strands[i] = Seq_strand_plus;
   }
   iip = id_head;
   i = 0;
   buf = (CharPtr)MemNew((maxlen+1)*sizeof(Char));
   sip_prev = NULL;
   while (iip != NULL)
   {
      j = 0;
      for (ctr = 0; ctr<(maxlen+1); ctr++)
      {
         buf[ctr] = '\0';
      }
      sip = MakeSeqID(iip->id);
      SeqIdSetFree(sip->next);
      sip->next = NULL;
      deflines[i] = StringSave(iip->defline);
      if (sip_prev != NULL)
      {
         sip_prev->next = sip;
         sip_prev = sip;
      } else
         dsp->ids = sip_prev = sip;
      ctr = 0;
      ctr_prev = 0;
      len = 0;
      seq = iip->sequence;
      while (seq != NULL)
      {
         c = seq->sequence;
         while (is_valid_seq(*c, afp->info->missingChar, afp->info->gapChar))
         {
            isgap = is_gap_char(*c, afp->info->gapChar);
            if (!isgap)
            {
               if (is_missing(*c, afp->info->missingChar))
                  buf[ctr] = 'N';
               else
                  buf[ctr] = *c;
               ctr++;
            }
            len++;
            if (len == dsp->lens[j])
            {
               if (isgap)
                  dsp->starts[dsp->dim*j+i] = -1;
               else
               {
                  dsp->starts[dsp->dim*j+i] = ctr_prev;
                  ctr_prev = ctr;
               }
               j++;
               len = 0;
            }
            if (*(c+1) == '\0' && seq->next == NULL && j < dsp->numseg)
            {
               if (isgap)
                  dsp->starts[dsp->dim*j+i] = -1;
               else
                  dsp->starts[dsp->dim*j+i] = ctr_prev;
            }
            c++;
         }
         seq = seq->next;
      }
      strings[i] = StringSave(buf);
      iip = iip->next;
      i++;
   }
   sap->segs = (Pointer)dsp;
   MemFree(buf);
   for (i=0; i<numtips; i++)
   {
      MemFree(tiparray[i]);
   }
   MemFree(tiparray);
   *numseq = dsp->dim;
   *stringsptr = strings;
   *deflineptr = deflines;
   return sap;
}

static SeqEntryPtr ALI_make_seqentry_for_seqentry (SeqEntryPtr sep)
{
   BioseqPtr     bsp;
   BioseqSetPtr  bssp;
   SeqEntryPtr   sep_new;
   SeqEntryPtr   sep_tmp;

   if (IS_Bioseq(sep) || IS_Bioseq_set(sep))
   {
      if (sep->next)
      {
         bssp = BioseqSetNew ();
         bssp->_class = 14;
         bssp->seq_set = sep;
         sep_new = SeqEntryNew ();
         sep_new->choice = 2;
         sep_new->data.ptrvalue = bssp;
         SeqMgrLinkSeqEntry (sep_new, 0, NULL);
         sep_tmp = bssp->seq_set;
         while (sep_tmp != NULL)
         {
            if (IS_Bioseq(sep_tmp))
            {
               bsp = (BioseqPtr)sep_tmp->data.ptrvalue;
               ObjMgrConnect (OBJ_BIOSEQ, (Pointer) bsp, OBJ_BIOSEQSET, (Pointer) bssp);
            }
            sep_tmp = sep_tmp->next;
         }
      } else
         return sep;
   }
   return sep_new;
}

static Uint1 ALI_GuessMoltype(CharPtr string)
{
   CharPtr  c;

   c = string;
   while (*c != '\0')
   {
       if (StringChr("EFIJLOPQUXZefijlopquxz", *c) != NULL) /* protein */
         return Seq_mol_aa;
      c++;
   }
   return Seq_mol_na;
}
static Int4 SPI_MapRowCoords(SeqAlignPtr sap, Int4 from, Int4 to, Int4 row, Uint1 direction)
{
   Int4  pos;

   if (direction == 1)
   {
      pos = AlnMgrMapRowCoords(sap, from, row, NULL);
      from++;
      while (pos < 0 && from <= to)
      {
         pos = AlnMgrMapRowCoords(sap, from, row, NULL);
         from++;
      }
   } else
   {
      pos = AlnMgrMapRowCoords(sap, to, row, NULL);
      to--;
      while (pos < 0 && to >= from)
      {
         pos = AlnMgrMapRowCoords(sap, to, row, NULL);
         to--;
      }
   }
   if (pos < 0)
      return -1;
   return pos;
}

static CharPtr SPI_WriteAlnLine(Int4 row, Int4 from, Int4 to, SeqAlignPtr sap)
{
   AlnMsgPtr   amp;
   BioseqPtr   bsp;
   Uint1       buf[65+2];
   Int4        ctr;
   Int4        i;
   Boolean     more;
   Int4        n;
   SeqIdPtr    sip;
   SeqPortPtr  spp;
   CharPtr     string;

   n = AlnMgrGetNumRows(sap);
   if (row > n || row < 1)
      return NULL;
   string = (CharPtr)MemNew((65+2)*sizeof(Char));
   for (n=0; n<(65+2); n++)
   {
      string[n] = '\0';
   }
   sip = AlnMgrGetNthSeqIdPtr(sap, row);
   bsp = BioseqLockById(sip);
   amp = AlnMsgNew();
   amp->row_num = row;
   amp->from_m = from;
   amp->to_m = to;
   if (amp->to_m < 0)
      amp->to_m = -1;
   n = 0;
   while ((more = AlnMgrGetNextAlnBit(sap, amp)) == TRUE)
   {
      if (amp->to_b - amp->from_b > amp->to_m - amp->from_m) /* kludge */
      {
         if (amp->strand == Seq_strand_minus)
            amp->from_b = amp->to_b - (amp->to_m - amp->from_m);
         else
            amp->to_b = amp->from_b + (amp->to_m - amp->from_m);
      }
      if (amp->gap == 0)
      {
         spp = SeqPortNew(bsp, amp->from_b, amp->to_b, amp->strand, Seq_code_iupacna);
         ctr = SeqPortRead(spp, buf, (amp->to_b - amp->from_b + 1));
         SeqPortFree(spp);
         for (i=n; i<n+ctr; i++)
         {
            string[i] = buf[i-n];
         }
         n += ctr;
      } else
      {
         for (i=n; i<(n+amp->to_b-amp->from_b+1); i++)
         {
            string[i] = '-';
         }
         n += amp->to_b-amp->from_b+1;
      }
   }
   AlnMsgFree(amp);
   SeqIdFree(sip);
   return string;
}
static Int4 spi_get_num_places(Int4 num)
{
   FloatHi  f;
   Int4     i;
   Int4     x;

   x = 10;
   for (i=1; i<21; i++)
   {
      f = (FloatHi)num/(FloatHi)x;
      if (f < 1)
      {
         if (num < 0)
            return (i+1);
         else
            return i;
      }
      x = x*10;
   }
   if (num < 0)
      i++;
   return i;
}
static void PrintOutMultAlign(SeqAlignPtr sap)
{
   Int4     c;
   Int4Ptr  coord;
   Int4     ctr;
   Int4     d;
   Int4     j;
   Int4     len;
   Int4     n;
   Int4     spacer;
   CharPtr  PNTR stringptr;

   spacer = 12;
   AlnMgrIndexSingleChildSeqAlign(sap);
   n = AlnMgrGetNumRows(sap);
   stringptr = (CharPtr PNTR)MemNew(n*sizeof(CharPtr));
   coord = (Int4Ptr)MemNew(n*sizeof(Int4));
   len = AlnMgrGetAlnLength(sap, FALSE);
   for (c=0; c<len; c+=65-10)
   {
      for (j=0; j<n; j++)
      {
         stringptr[j] = SPI_WriteAlnLine(j+1, c, MIN(c+65-10-1, len-1), sap);
         coord[j] = SPI_MapRowCoords(sap, c, MIN(c+65-10-1, len-1), j+1, 1);
         if (coord[j] >= 0)
            coord[j]++;
      }
      for (j=0; j<n; j++)
      {
         printf("%d", coord[j]);
         d = spi_get_num_places(coord[j]);
         for (d; d<spacer; d++)
         {
            printf(" ");
         }
         if (j == 0)
            printf("%s", stringptr[j]);
         else
         {
            for (ctr=0; ctr<MIN(65-10, len-c); ctr++)
            {
               if (stringptr[j][ctr] == stringptr[0][ctr])
                  printf(".");
               else
                  printf("%c", stringptr[j][ctr]);
            }
         }
         printf("\n");
         MemFree(stringptr[j]);
      }
      if (c+65-10 < len)
         printf("\n");
   }
   fflush(stdout);
}

static void PrintOutSegs(SeqAlignPtr sap)
{
   DenseSegPtr  dsp;
   Int4         i;
   Int4         j;

   dsp = (DenseSegPtr)(sap->segs);
   printf("nums:\t");
   for (i=0; i<dsp->numseg; i++)
   {
      printf("%d\t", i+1);
   }
   printf("\n");
   printf("lens:\t");
   for (i=0; i<dsp->numseg; i++)
   {
      printf("%d\t", dsp->lens[i]);
   }
   printf("\n");
   for (i=0; i<dsp->dim; i++)
   {
      printf("row %d\t", i+1);
      for (j=0; j<dsp->numseg; j++)
      {
         printf("%d\t", dsp->starts[(dsp->dim)*j+i]);
      }
      printf("\n");
   }
   fflush(stdout);
}

NLM_EXTERN SeqEntryPtr ALI_ConvertToNCBIData(AlignFileDataPtr afp)
{
   BioseqPtr    bsp;
   CharPtr      PNTR deflines;
   Int4         i;
   Int4         len;
   Uint1        moltype;
   Int4         numseq;
   SeqAnnotPtr  sanp;
   SeqAlignPtr  sap;
   SeqDescrPtr  sdp;
   SeqEntryPtr  sep;
   SeqEntryPtr  sep_head;
   SeqEntryPtr  sep_prev;
   SeqIdPtr     sip;
   CharPtr      str;
   CharPtr      PNTR strings;

   if (afp == NULL || afp->sequences == NULL)
   {
      ErrPostEx(SEV_ERROR, 0, 0, "NULL Data Passed to ConvertToNCBIData");
      return NULL;
   }
   sap = ALI_MakeSeqAlign(afp, &strings, &numseq, &deflines);
   if (sap == NULL)
   {
      ErrPostEx(SEV_ERROR, 0, 0, "Unable to create seqentry\n");
      return NULL;
   }
   sanp = SeqAnnotNew();
   sanp->type = 2;
   sanp->data = (Pointer)sap;
   moltype = ALI_GuessMoltype(strings[0]);
   sip = ((DenseSegPtr)(sap->segs))->ids;
   sep_head = sep_prev = NULL;
   for (i=0; i<numseq; i++)
   {
      len = StringLen(strings[i]);
      sep = StringToSeqEntry (strings[i], sip, len, moltype);
      if (sep != NULL) {
	bsp = (BioseqPtr)(sep->data.ptrvalue);
	if (! StringHasNoText (deflines[i])) {
	  str = deflines[i];
	  sdp = SeqDescrAddPointer(&(bsp->descr), Seq_descr_title, str);
	}
	if (sep != NULL)
	  {
	    if (sep_head != NULL)
	      {
		sep_prev->next = sep;
		sep_prev = sep;
	      } else
		sep_head = sep_prev = sep;
	  }
	sip = sip->next;
	MemFree(strings[i]);
      }
   }
   sep_head = ALI_make_seqentry_for_seqentry (sep_head);
   SeqAlignAddInSeqEntry (sep_head, sanp);
   MemFree(strings);
   MemFree(deflines);
   return sep_head;
}
