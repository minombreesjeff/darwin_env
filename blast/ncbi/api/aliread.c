/* Include files */

#include <ncbi.h>
#include <objalign.h>

#include <aliparse.h>

#ifdef OS_UNIX_DARWIN
#define NLM_GETC fgetc
#else
#define NLM_GETC getc
#endif

/* Defined constants */

#define ALI_DATA_NUCLEOTIDE  1      /* Values for dataType   */
#define ALI_DATA_PROTEIN     2      /* field in DataInfo.    */
#define ALI_MAX_LINE_LEN     256

/* Function prototypes */

Boolean     IsNucleotideChar (Char ch);
Boolean     IsProteinChar (Char ch);
Boolean     IsSequenceChar (Char    ch,
			    CharPtr gapChar,
			    CharPtr missingChar,
			    CharPtr unalignedChar);
Int2        IsValidIdChar (Char idChar);
Boolean     IsValidId (CharPtr idStr);
CharPtr     ReadAlignFileLine (FILE PNTR        alignFilePtr,
			       ErrInfoPtr PNTR  errorListPtr,
			       AliConfigInfoPtr configPtr,
			       Boolean    PNTR  isEOF);


static Boolean          s_MightBeCorruptSequence (Int4             seqCharCount,
						  CharPtr          seqString,
						  AliConfigInfoPtr configPtr);
static DefLineInfoPtr   s_ParseDefLine (CharPtr          lineStr,
					Int4             rowNum,
					ErrInfoPtr PNTR  errorListPtr);
static SeqLineInfoPtr   s_ParseSequenceLine (CharPtr lineStr,
					     AliConfigInfoPtr configPtr);
CharPtr                 s_OtherGetValue (CharPtr otherStr);
static Boolean          s_ProcessOtherLine (AliConfigInfoPtr configPtr,
					    CharPtr          otherStr,
					    AlignFileDataPtr fileInfoPtr);
static OtherLineInfoPtr s_ParseOtherLine (CharPtr lineStr);

/*=========================================================================*/
/*                                                                         */
/*  IsNucleotideChar ()                                                    */
/*                                                                         */
/*=========================================================================*/

Boolean IsNucleotideChar (Char ch)
{
  if (StringChr("abcdghkmnrstuvwxyABCDGHKMNRSTUVWXY",ch) != NULL)
    return TRUE;
  else
    return FALSE;
}

/*=========================================================================*/
/*                                                                         */
/*  IsProteinChar ()                                                       */
/*                                                                         */
/*=========================================================================*/

Boolean IsProteinChar (Char ch)
{
  if (StringChr("ABCDEFGHIKLMNPQRSTUVWXYZ*abcdefghiklmnpqrstuvwxyz",ch) != NULL)
    return TRUE;
  else
    return FALSE;
}

/*=========================================================================*/
/*                                                                         */
/* Ali_SeqLineGetType ()                                                   */
/*                                                                         */
/*=========================================================================*/

Int2 Ali_SeqLineGetType(CharPtr seqLine,
			AliConfigInfoPtr configPtr)
{
  Int4    position;
  Int4    nuclCount;
  Int4    miscCount;
  FloatLo percentNucl;
  FloatLo percentMisc;
  Char    commonNucls[20];
  Char    miscChars[5];

  /* Is it definitely a protein sequence? */
  /* The following chars are only in      */
  /* protein sequences.                   */

  if ((StringChr (seqLine, 'E')) ||
      (StringChr (seqLine, 'e')) ||
      (StringChr (seqLine, 'F')) ||
      (StringChr (seqLine, 'f')) ||
      (StringChr (seqLine, 'I')) ||
      (StringChr (seqLine, 'i')) ||
      (StringChr (seqLine, 'L')) ||
      (StringChr (seqLine, 'l')) ||
      (StringChr (seqLine, 'P')) ||
      (StringChr (seqLine, 'p')) ||
      (StringChr (seqLine, 'Q')) ||
      (StringChr (seqLine, 'q')) ||
      /*
      (StringChr (seqLine, 'U')) ||
      (StringChr (seqLine, 'u')) ||
      */
      (StringChr (seqLine, 'Z')) ||
      (StringChr (seqLine, 'z')) ||
      (StringChr (seqLine, '*')))
    return ALI_PROTEIN;

  /* All others are technically ambiguous, but */
  /* if we have a high enough percentage of    */
  /* common nucleotides, then it is probably a */
  /* nucleotide sequence.                      */
  
  nuclCount = 0;
  miscCount = 0;
  sprintf (commonNucls, "ATCGNXatcgnx");

  if (configPtr->unalignedChar != NULL)
    sprintf (miscChars, "-%s%s%s ", configPtr->gapChar,
	     configPtr->missingChar, configPtr->unalignedChar);
  else
    sprintf (miscChars, "-%s%s ", configPtr->gapChar,
	     configPtr->missingChar);

  for (position = 0; seqLine[position] != '\0'; position++) {
    if (StringChr (commonNucls, seqLine[position]) != NULL)
      nuclCount++;
    else if (StringChr (miscChars, seqLine[position]) != NULL)
      miscCount++;
  }

  /* If we have a high percentage of misc chars then */
  /* we don't have enough data to make a decision.   */

  percentMisc = ((FloatLo) miscCount) / ((FloatLo) StringLen (seqLine));
  if ((percentMisc * 100) > 80)
    return ALI_AMBIGUOUS;

  /* Else, if a high percentage are common nucleotide */
  /* characters then it is a nucleotide line.         */

  percentNucl = ((FloatLo) nuclCount + (FloatLo) miscCount) / 
                (FloatLo) StringLen (seqLine);

  if ((percentNucl * 100) > configPtr->nuclLineMaxThreshold)
    return ALI_NUCLEOTIDE;
  else if ((percentNucl * 100) < configPtr->nuclLineMinThreshold)
    return ALI_PROTEIN;

  /* If we haven't come to a conclusion */
  /* then say so.                       */

  return ALI_AMBIGUOUS;
}

/*=========================================================================*/
/*                                                                         */
/*  IsSequenceChar ()                                                      */
/*                                                                         */
/*=========================================================================*/

Boolean IsSequenceChar (Char    ch,
			CharPtr gapChar,
			CharPtr missingChar,
			CharPtr unalignedChar)
{

  
  if (IsNucleotideChar(ch))
    return TRUE;

  if (IsProteinChar(ch))
    return TRUE;

  if (StrChr (gapChar, ch) != NULL)
    return TRUE;

  if (StrChr (missingChar, ch) != NULL)
    return TRUE;

  if ((unalignedChar != NULL) &&
      (StrChr (unalignedChar, ch) != NULL))
    return TRUE;

  return FALSE;
}

/*=========================================================================*/
/*                                                                         */
/* IsValidIdChar ()                                                        */
/*                                                                         */
/*=========================================================================*/

#define ID_BAD_CHAR         0
#define ID_GOOD_CHAR_LETTER 1
#define ID_GOOD_CHAR_NUMBER 2
#define ID_GOOD_CHAR_OTHER  3

Int2 IsValidIdChar (Char idChar)
{
  if (StringChr("ABCDEFGHIJKLMNOPQRSTUVWXYZ",idChar) != NULL)
    return ID_GOOD_CHAR_LETTER;

  if (StringChr("abcdefghijklmnopqrstuvwxyz",idChar) != NULL)
    return ID_GOOD_CHAR_LETTER;

  if (StringChr("0123456789",idChar) != NULL)
    return ID_GOOD_CHAR_NUMBER;

  if (StringChr("\"._-|",idChar) != NULL)
    return ID_GOOD_CHAR_OTHER;

  return FALSE;
}

/*=========================================================================*/
/*                                                                         */
/* IsValidId ()                                                            */
/*                                                                         */
/*=========================================================================*/

Boolean IsValidId (CharPtr idStr)
{
  Int4    position;
  Boolean letterFound = FALSE;
  Int2    charType;

  /* Check for illegal characters */

  for (position = 0; idStr[position] != '\0'; position++)
    {
      charType = IsValidIdChar(idStr[position]);
      switch (charType)
	{
	case ID_GOOD_CHAR_LETTER :
	  letterFound = TRUE;
	  break;
	case ID_GOOD_CHAR_NUMBER :
	case ID_GOOD_CHAR_OTHER :
	  break;
	default:
	  return FALSE;
	}
    }

  if (!letterFound)
    return FALSE;

  /* Check to see if ID matches a reserved word */

  if (StringICmp (idStr,"MATRIX") == 0)
    return FALSE;
  else if (StringICmp (idStr,"BEGIN") == 0)
    return FALSE;
  else if (StringICmp (idStr, "END") == 0)
    return FALSE;

  /* If we passed all tests, mark */
  /* it as a valid ID.            */

  return TRUE;
}

/*=========================================================================*/
/*                                                                         */
/* IsNumString ()                                                          */
/*                                                                         */
/*=========================================================================*/

Boolean IsNumString (CharPtr someStr)
{
  Int4    position;

  /* Check for non-numeric characters */

  for (position = 0; someStr[position] != '\0'; position++)
    if (StrChr ("0123456789.", someStr[position]) == NULL)
      return FALSE;

  /* If we made it to here, all characters are */
  /* numeric, and so the string is numeric.    */

  return TRUE;
}

/*=========================================================================*/
/*                                                                         */
/* Ali_ChangeRowToOther ()                                                 */
/*                                                                         */
/*=========================================================================*/

void Ali_ChangeRowToOther (ValNodePtr rowPtr)
{
  DefLineInfoPtr   defLinePtr;
  SeqLineInfoPtr   seqLinePtr;
  OtherLineInfoPtr otherLinePtr;

  /* Sequence line to Other Line */

  if (rowPtr->choice == ALI_SEQLINE)
    {
      otherLinePtr = (OtherLineInfoPtr) MemNew (sizeof(OtherLineInfo));
      seqLinePtr = (SeqLineInfoPtr) rowPtr->data.ptrvalue;

      if (seqLinePtr->junk != NULL)
	sprintf(seqLinePtr->sequence,"%s%s",seqLinePtr->sequence,
		seqLinePtr->junk);

      if ((seqLinePtr->sequence != NULL) && (seqLinePtr->id == NULL))
	{
	  if (IsValidId(seqLinePtr->sequence))
	    {
	      otherLinePtr->id    = seqLinePtr->sequence;
	      otherLinePtr->other = NULL;
	    }
	  else
	    {
	      otherLinePtr->id    = NULL;
	      otherLinePtr->other = seqLinePtr->sequence;
	    }
	}
      else
	{
	  otherLinePtr->other = seqLinePtr->sequence;
	  otherLinePtr->id    = seqLinePtr->id;
	}

      otherLinePtr->rowNum = seqLinePtr->rowNum;

      MemFree(seqLinePtr);
      rowPtr->data.ptrvalue = otherLinePtr;
      rowPtr->choice = ALI_OTHERLINE;
    }


  /* Definition line to Other line */

  else if (rowPtr->choice == ALI_DEFLINE)
    {
      otherLinePtr = (OtherLineInfoPtr) MemNew (sizeof(OtherLineInfo));
      defLinePtr = (DefLineInfoPtr) rowPtr->data.ptrvalue;

      otherLinePtr->other = defLinePtr->definitions;
      otherLinePtr->id    = defLinePtr->id;
      otherLinePtr->rowNum   = defLinePtr->rowNum;

      MemFree(defLinePtr);
      rowPtr->data.ptrvalue = otherLinePtr;
      rowPtr->choice = ALI_OTHERLINE;
    }

  /* Return successfully */

  return;
}

/*=========================================================================*/
/*                                                                         */
/* ReadAlignFileLine() -                                                   */
/*                                                                         */
/*=========================================================================*/

CharPtr ReadAlignFileLine (FILE PNTR        alignFilePtr,
			   ErrInfoPtr PNTR  errorListPtr,
			   AliConfigInfoPtr configPtr,
			   Boolean    PNTR  isEOF)
     
{
  CharPtr lineStr = NULL;
  CharPtr tempBuff = NULL;
  Int4    totalLen = 0;
  Int4    segmentLen = 0;
  Int4    segmentCount = 1;
  Boolean done = FALSE;
  Char    ch = 0;

  /* Allocate memory for the line.  More */
  /* can be added later as necessary.    */

  lineStr = (CharPtr) MemNew(sizeof(Char) * configPtr->readBuffSize);
  if (lineStr == NULL)
    {
      Ali_AddError (errorListPtr, ERR_OUT_OF_MEMORY);
      return NULL;
    }

  /* Read in the characters one at a time */

  while (!done && !(ch == EOF))
    {

      /* Process the current character */

      ch = (Char) NLM_GETC (alignFilePtr);

      if (ch == '\n')
	{
	  done = TRUE;
	  ch = (Char) NLM_GETC (alignFilePtr);
	  if (ch != '\r') {
	    ungetc (ch, alignFilePtr);
	  
	  }
	}
      else if (ch == '\r') {
        done = TRUE;
      }
      else
	{
	  lineStr[totalLen] = ch;
	  segmentLen++;
	  totalLen++;
	}

      /* Allocate more memory for the */
      /* sequence if needed.          */

      if (segmentLen == configPtr->readBuffSize)
	{
	  segmentCount++;
	  tempBuff = (CharPtr) MemNew(sizeof(Char) * 
				      segmentCount *
				      configPtr->readBuffSize);
	  if (tempBuff == NULL)
	    {
	      Ali_AddError (errorListPtr, ERR_OUT_OF_MEMORY);
	      MemFree (lineStr);
	      MemFree (tempBuff);
	      return NULL;
	    }
	  MemCpy(tempBuff, lineStr, (segmentCount-1) * configPtr->readBuffSize);
	  MemFree(lineStr);
	  lineStr = tempBuff;
	  segmentLen = 0;
	}

    }

  /* Return successfully */

  if (EOF == ch)
    *isEOF = TRUE;

  lineStr[totalLen] = '\0';

  return lineStr;
}

/*=========================================================================*/
/*                                                                         */
/* s_ParseDefLine () -                                                     */
/*                                                                         */
/*=========================================================================*/

#define DEFLINE_PRE_DATA      0
#define DEFLINE_DEFINITION    1
#define DEFLINE_SEQID         2

static DefLineInfoPtr s_ParseDefLine (CharPtr lineStr,
				      Int4 rowNum,
				      ErrInfoPtr PNTR  errorListPtr)
{
  Char           ch;
  CharPtr        defStr;
  CharPtr        idStr;
  Int4           defPosition;
  Int4           idPosition;
  Int4           position;
  Int2           state;
  DefLineInfoPtr defLinePtr = NULL;
  ErrInfoPtr     errPtr;

  defPosition = 0;
  idPosition = 0;

  defStr = (CharPtr) MemNew (StringLen(lineStr)+1);
  idStr  = (CharPtr) MemNew (StringLen(lineStr)+1);

  /* Parse the line character by character */

  state = DEFLINE_PRE_DATA;

  for (position = 0; lineStr[position] != '\0'; position++)
    {
      ch = lineStr[position];

      switch (state)
	{
	case DEFLINE_PRE_DATA :
	  if (IS_WHITESP(ch))
	    continue;
	  else if (ch == '>')
	    state = DEFLINE_SEQID;
	  else
	    {
	      MemFree(defStr);
	      MemFree(idStr);
	      return NULL;  /* Not a defline */
	    }
	  break;
	case DEFLINE_SEQID : 
	  if (IsValidIdChar(ch))
	    {
	      idStr[idPosition] = ch;
	      idPosition++;
	    }
	  else if (IS_WHITESP(ch))	
	    {
	      if (idPosition > 0)
		{
		  state = DEFLINE_DEFINITION;
		  defStr[defPosition] = ch;
		  defPosition++;
		}
	      else
		continue;
	    }
	  else if (ch == '[')
	    {
	      state = DEFLINE_DEFINITION;
	      defStr[defPosition] = ch;
	      defPosition++;
	    }
	  else
	    {
	      errPtr = Ali_AddError (errorListPtr, ERR_INVALID_DEFLINE,
				     lineStr, (Int4) ch);
	      errPtr->rowNum = rowNum;
	      MemFree(defStr);
	      MemFree(idStr);
	      return NULL;
	    }
	  break;
	case DEFLINE_DEFINITION :
	  defStr[defPosition] = ch;
	  defPosition++;
	  break;
	default:
	  break;
	}
    }

  /* Check for blank line */
  
  if (state == DEFLINE_PRE_DATA)
    {
      MemFree(defStr);
      MemFree(idStr);
      return NULL;
    }

  idStr[idPosition]   = '\0';
  defStr[defPosition] = '\0';

  /* Make sure that it has at least one */
  /* set of square brackets.            */

  if ((StringChr(defStr,'[') == NULL) || (StringChr(defStr,']') == NULL))
    {
      errPtr = Ali_AddError (errorListPtr, ERR_DEFLINE_NODEFS, lineStr);
      errPtr->rowNum = rowNum;
    }
  
  /* If we made it to here, then */
  /* it's a valid definition line. */

  defLinePtr = (DefLineInfoPtr) MemNew (sizeof (DefLineInfo));

  if (StringLen (defStr) != 0)
    defLinePtr->definitions = defStr;
  else
    defLinePtr->definitions = NULL;

  if (StringLen (idStr) != 0)
    defLinePtr->id = idStr;
  else
    defLinePtr->id = NULL;

  return defLinePtr;
}

/*=========================================================================*/
/*                                                                         */
/* s_MightBeCorruptSequence ()                                             */
/*                                                                         */
/*=========================================================================*/

static Boolean s_MightBeCorruptSequence (Int4             seqCharCount,
					 CharPtr          seqString,
					 AliConfigInfoPtr configPtr)
{
  Int4    i;
  Int4    badCharCount;
  Int4    seqStrLen;
  FloatLo percentGood;

  seqStrLen = StringLen(seqString);
  badCharCount = 0;

  for (i = 0; i < seqStrLen; i++)
    {
      if (IsSequenceChar(seqString[i],
			 configPtr->gapChar,
			 configPtr->missingChar,
			 configPtr->unalignedChar))
	seqCharCount++;
      else
	badCharCount++;
    }

  percentGood = (FloatLo) seqCharCount / ((FloatLo) seqCharCount + 
                                         (FloatLo) badCharCount);

  if ((percentGood * 100) >= configPtr->corruptSeqThreshold)
    return TRUE;
  else
    return FALSE;
}

/*=========================================================================*/
/*                                                                         */
/* s_ParseSequenceLine () -                                                */
/*                                                                         */
/*=========================================================================*/

#define PRE_DATA      0
#define FIRST_WORD    1
#define SEQUENCE_DATA 2
#define EOL_JUNK      3
#define POST_JUNK     4

static SeqLineInfoPtr s_ParseSequenceLine (CharPtr lineStr,
					   AliConfigInfoPtr configPtr)
{
  CharPtr        seqStr;
  Int4           seqPosition = 0;
  CharPtr        idStr;
  Int4           idPosition = 0;
  Int4           firstWordLen = 0;
  Char           ch;
  Int2           state = PRE_DATA;
  Int4           position;
  Boolean        firstWordNotSequence = FALSE;
  Boolean        sequenceFound = FALSE;
  CharPtr        tempStr;
  Boolean        corruptSequence = FALSE;
  SeqLineInfoPtr seqLinePtr;

  if (StringLen(lineStr) == 0)
    return NULL;

  seqStr = (CharPtr) MemNew (StringLen(lineStr)+1);
  idStr  = (CharPtr) MemNew (StringLen(lineStr)+1);

  for (position = 0; lineStr[position] != '\0'; position++)
    {
      ch = lineStr[position];

      switch (state)
	{
	case PRE_DATA :

	  /* If it's the first non-whitespace char */
	  /* then we've found our first word.      */

	  if (!IS_WHITESP(ch))
	    {
	      state = FIRST_WORD;
	      if (!IsSequenceChar(ch,
				  configPtr->gapChar,
				  configPtr->missingChar,
				  configPtr->unalignedChar))
		firstWordNotSequence = TRUE;
	      idStr[idPosition] = ch;
	      idPosition++;
	      firstWordLen++;
	    }
	  break;
	case FIRST_WORD :
	  if (IS_WHITESP(ch))
	    {
	      state = SEQUENCE_DATA;
	      if ((idPosition > 0)   &&
		  (firstWordNotSequence == FALSE))
		{
		  tempStr = seqStr;
		  seqStr  = idStr;
		  idStr   = tempStr;
		  seqPosition = idPosition;
		  idPosition  = 0;
		  sequenceFound = TRUE;
		}
	    }
	  else
	    {
	      /* If we find a non-sequence char in the */
	      /* first word then it might be an ID,    */
	      /* with the sequence following.          */
	      
	      if (!IsSequenceChar(ch,
				  configPtr->gapChar,
				  configPtr->missingChar,
				  configPtr->unalignedChar))
		firstWordNotSequence = TRUE;
	      idStr[idPosition] = ch;
	      idPosition++;
	      firstWordLen++;
	    }
	  break;
	case SEQUENCE_DATA :
	  if (IS_WHITESP(ch))
	    continue;
	    
	  /* If we're in a sequence, then a non-sequence */
	  /* char invalidates it, although we do allow   */
	  /* 'junk' at the end.                          */
	  
	  if (!IsSequenceChar(ch,
			      configPtr->gapChar,
			      configPtr->missingChar,
			      configPtr->unalignedChar))
	    {
	      if ((lineStr[position - 1] == ' ') && sequenceFound)
		state = EOL_JUNK;
	      else if ((corruptSequence == TRUE) ||
		       (s_MightBeCorruptSequence (seqPosition,
						  &(lineStr[position]),
						  configPtr)))
		{
		  seqStr[seqPosition] = ch;
		  seqPosition++;
		  sequenceFound = TRUE;
		  corruptSequence = TRUE;
		}
	      else
		{
		  MemFree(seqStr);
		  MemFree(idStr);
		  return NULL;
		}
	    }
	  else
	    {
	      seqStr[seqPosition] = ch;
	      seqPosition++;
	      sequenceFound = TRUE;
	    }
	  break;
	case EOL_JUNK :
	  if (IS_WHITESP(ch))
	    state = POST_JUNK;
	  break;
	case POST_JUNK :

	  /* Only one 'word' of junk allowed */

	  if (!IS_WHITESP(ch))
	    {
	      MemFree(seqStr);
	      MemFree(idStr);
	      return NULL;
	    }
	  break;
	}
    }

  /* Check for blank line */
  
  if (state == PRE_DATA)
    {
      MemFree(seqStr);
      MemFree(idStr);
      return NULL;
    }
  
  if (state == FIRST_WORD)
    {

      /* If there was just one word, and it isn't */
      /* a sequence string, then this isn't a     */
      /* sequence line.                           */
      
      if (firstWordNotSequence == TRUE)
	{
	  MemFree(seqStr);
	  MemFree(idStr);
	  return NULL;
	}
      
      /* If there was just one word, and it IS a sequence */
      /* then the idStr is actually the seqStr.           */
      
      else
	{
	  tempStr = seqStr;
	  seqStr  = idStr;
	  idStr   = tempStr;
	  seqPosition = idPosition;
	  idPosition  = 0;
	}
    }

  /* If still no sequence string, */
  /* then not a sequence line.    */

  if (StringLen(seqStr) == 0)
    {
      MemFree(seqStr);
      MemFree(idStr);
      return NULL;
    }

  /* Check to see if the ID is a valid one */

  idStr[idPosition]   = '\0';
  seqStr[seqPosition] = '\0';

  if ((idPosition > 0) &&
      (IsValidId (idStr) == FALSE) &&
      (IsNumString (idStr) == FALSE))
    {
      MemFree(idStr);
      MemFree(seqStr);
      return NULL;
    }

  /* If we made it to here, then */
  /* it's a valid sequence line. */

  seqLinePtr = (SeqLineInfoPtr) MemNew (sizeof (SeqLineInfo));

  seqLinePtr->firstWordLen = firstWordLen;

  if (StringLen (seqStr) != 0)
    seqLinePtr->sequence = seqStr;
  else
    {
      seqLinePtr->sequence = NULL;
      MemFree (seqStr);
    }

  if (StringLen (idStr) != 0)
    seqLinePtr->id = idStr;
  else
    {
      seqLinePtr->id = NULL;
      MemFree (idStr);
    }

  if (corruptSequence)
    seqLinePtr->maybe  = TRUE;
  else
    seqLinePtr->maybe  = FALSE;

  return seqLinePtr;
}

/*=========================================================================*/
/*                                                                         */
/* s_ParseOtherLine () -                                                   */
/*                                                                         */
/*=========================================================================*/

#define OTHER_PRE_DATA  0
#define OTHER_DATA      1

static OtherLineInfoPtr s_ParseOtherLine (CharPtr lineStr)
{
  Char             ch;
  CharPtr          otherStr;
  Int4             otherPosition;
  Int4             position;
  Int2             state;
  Int4             wordCount;
  OtherLineInfoPtr otherLinePtr;

  /* Parse the line character by character */

  otherStr = (CharPtr) MemNew (StringLen(lineStr)+1);
  otherPosition = 0;
  state     = OTHER_PRE_DATA;
  wordCount = 0;

  for (position = 0; lineStr[position] != '\0'; position++)
    {
      ch = lineStr[position];

      switch (state)
	{
	case OTHER_PRE_DATA :
	  if (IS_WHITESP(ch))
	    continue;
	  else
	    {
	      wordCount = 1;
	      state = OTHER_DATA;
	      otherStr[otherPosition] = ch;
	      otherPosition++;
	    }
	  break;
	case OTHER_DATA : 
	  if (IS_WHITESP(ch))	
	    wordCount++;
	  otherStr[otherPosition] = ch;
	  otherPosition++;
	  break;
	default:
	  break;
	}
    }

  /* Check for blank line */
  
  if (state == OTHER_PRE_DATA)
    {
      MemFree(otherStr);
      return NULL;
    }
  
  /* If we made it to here, then */
  /* it's a valid definition line. */

  otherStr[otherPosition]   = '\0';

  otherLinePtr = (OtherLineInfoPtr) MemNew (sizeof (OtherLineInfo));
  if ((wordCount == 1) && IsValidId(otherStr))
    {
      otherLinePtr->id    = otherStr;
      otherLinePtr->other = NULL;
    }
  else
    {
      otherLinePtr->id    = NULL;
      otherLinePtr->other = otherStr;
    }

  /* Return successfully */

  return otherLinePtr;
}

/*=========================================================================*/
/*                                                                         */
/* s_OtherGetValue ()                                                      */
/*                                                                         */
/*=========================================================================*/

CharPtr s_OtherGetValue (CharPtr otherStr)
{
  CharPtr tempStrPtr;
  CharPtr valueBuff;
  Int2    charCount;

  /* Go to the first character after the '=' */

  if ((tempStrPtr = StringChr (otherStr, '=')) == NULL)
    return NULL;
  tempStrPtr++;

  /* Skip spaces */

  while (*tempStrPtr == ' ')
    tempStrPtr++;

  if (*tempStrPtr == '\0')
    return NULL;

  /* Get the value */

  valueBuff = (CharPtr) MemNew (ALI_MAX_LINE_LEN);
  charCount = 0;

  while ((*tempStrPtr != ' ') && (*tempStrPtr != '\0'))
    {
      valueBuff[charCount] = *tempStrPtr;
      charCount++;
      tempStrPtr++;
    }

  valueBuff[charCount] = '\0';

  /* Return successfully */

  return valueBuff;
}

/*=========================================================================*/
/*                                                                         */
/* s_ProcessOtherLine ()                                                   */
/*                                                                         */
/*=========================================================================*/

static Boolean s_ProcessOtherLine (AliConfigInfoPtr configPtr,
				   CharPtr          otherStr,
				   AlignFileDataPtr fileInfoPtr)
{
  CharPtr strPtr;
  CharPtr tmpStr;

  /* Check for datatype declaration */

  if (((strPtr = StringStr (otherStr, "datatype")) != NULL) ||
      ((strPtr = StringStr (otherStr, "DATATYPE")) != NULL) ||
      ((strPtr = StringStr (otherStr, "Datatype")) != NULL))
    {
      tmpStr = s_OtherGetValue(strPtr);
      if (StringICmp (tmpStr, "DNA") == 0)
	configPtr->declaredInfo.dataType = ALI_DATA_NUCLEOTIDE;
      else
	configPtr->declaredInfo.dataType = ALI_DATA_PROTEIN;
      MemFree (tmpStr);
    }

  /* Check for interleaved/contiguous */

  if (((strPtr = StringStr (otherStr, "INTERLEAVED")) != NULL) ||
      ((strPtr = StringStr (otherStr, "interleaved")) != NULL) ||
      ((strPtr = StringStr (otherStr, "Interleaved")) != NULL))
    configPtr->declaredInfo.contigOrInter = ALI_INTERLEAVED;
  else if (((strPtr = StringStr (otherStr, "CONTIGUOUS")) != NULL) ||
	   ((strPtr = StringStr (otherStr, "contiguous")) != NULL) ||
	   ((strPtr = StringStr (otherStr, "Contiguous")) != NULL))
    configPtr->declaredInfo.contigOrInter = ALI_CONTIGUOUS;

  /* Check for dimensions */

  if (((strPtr = StringStr (otherStr, "NTAX")) != NULL) ||
      ((strPtr = StringStr (otherStr, "ntax")) != NULL) ||
      ((strPtr = StringStr (otherStr, "nTax")) != NULL))
    {
      tmpStr = s_OtherGetValue (strPtr);
      configPtr->declaredInfo.idCount = atoi(tmpStr);
      MemFree (tmpStr);
    }

  if (((strPtr = StringStr (otherStr, "NCHAR")) != NULL) ||
      ((strPtr = StringStr (otherStr, "nchar")) != NULL) ||
      ((strPtr = StringStr (otherStr, "nChar")) != NULL))
    {
      tmpStr = s_OtherGetValue (strPtr);
      configPtr->declaredInfo.seqLength = atoi(tmpStr);
      MemFree (tmpStr);
    }

  /* Check for definition of missing character */
  
  if (((strPtr = StringStr (otherStr, "MISSING")) != NULL) ||
      ((strPtr = StringStr (otherStr, "missing")) != NULL) ||
      ((strPtr = StringStr (otherStr, "Missing")) != NULL))
    {
      tmpStr = s_OtherGetValue(strPtr);
      configPtr->missingChar = (CharPtr) MemNew (2);
      sprintf (configPtr->missingChar, "%c", tmpStr[0]);
      MemFree (tmpStr);

      /* If the new missing char conflicts with the */
      /* gap or unaligned char, then blank them out */
      /* to give the new one precedence.            */

      if (StringICmp (configPtr->missingChar, configPtr->gapChar) == 0)
	StringCpy (configPtr->gapChar, "");

      if (StringICmp (configPtr->missingChar, configPtr->unalignedChar) == 0)
	StringCpy (configPtr->unalignedChar, "");
	
    }
  
  /* Check for definition of gap character */
  
  if (((strPtr = StringStr (otherStr, "GAP")) != NULL) ||
      ((strPtr = StringStr (otherStr, "gap")) != NULL) ||
      ((strPtr = StringStr (otherStr, "Gap")) != NULL))
    {
      tmpStr = s_OtherGetValue(strPtr);
      configPtr->gapChar = (CharPtr) MemNew (2);
      sprintf (configPtr->gapChar, "%c", tmpStr[0]);
      MemFree (tmpStr);

      /* If the new gap char conflicts with the missing */
      /* or unaligned char, then blank them out to give */
      /* the new one precedence.                        */

      if (StringICmp (configPtr->gapChar, configPtr->missingChar) == 0)
	StringCpy (configPtr->missingChar, "");

      if (StringICmp (configPtr->gapChar, configPtr->unalignedChar) == 0)
	StringCpy (configPtr->unalignedChar, "");
	
    }
  
  /* Check for definition of unaligned character */
  
  if (((strPtr = StringStr (otherStr, "UNALIGNED")) != NULL) ||
      ((strPtr = StringStr (otherStr, "unaligned")) != NULL) ||
      ((strPtr = StringStr (otherStr, "Unaligned")) != NULL))
    {
      tmpStr = s_OtherGetValue(strPtr);
      configPtr->unalignedChar = (CharPtr) MemNew (2);
      sprintf (configPtr->unalignedChar, "%c", tmpStr[0]);
      MemFree (tmpStr);

      /* If the new unaligned char conflicts with the */
      /* gap or missing char, then blank them out to  */
      /* give the new one precedence.                 */

      if (StringICmp (configPtr->unalignedChar, configPtr->gapChar) == 0)
	StringCpy (configPtr->gapChar, "");

      if (StringICmp (configPtr->unalignedChar, configPtr->missingChar) == 0)
	StringCpy (configPtr->missingChar, "");
	
    }
  
  /* Return successfully */
  
  return TRUE;
}

/*=========================================================================*/
/*                                                                         */
/* Ali_ReadLines ()                                                        */
/*                                                                         */
/*=========================================================================*/

ValNodePtr Ali_ReadLines (FILE PNTR        alignFilePtr,
			  ErrInfoPtr PNTR  errorListPtr,
			  AliConfigInfoPtr configPtr,
			  AlignFileDataPtr fileInfoPtr)
{
  CharPtr          lineStr = NULL;
  ValNodePtr       rowList = NULL;
  ValNodePtr       newRow;
  Boolean          first = TRUE;
  SeqLineInfoPtr   seqLine;
  SeqLineInfoPtr   reEvalSeqPtr;
  DefLineInfoPtr   defLine;
  OtherLineInfoPtr otherLine;
  Boolean          nextRowMustBeSeq;
  Boolean          idFound;
  Boolean          lastRowWasOther = FALSE;
  Int4             rowNum;
  ErrInfoPtr       errPtr;
  Boolean          isEOF;

  nextRowMustBeSeq = FALSE;
  rowNum = 0;
  isEOF = FALSE;

  while (FALSE == isEOF)
    {

      /* Process the line according to its content ... */

      lineStr = ReadAlignFileLine(alignFilePtr, errorListPtr,
				  configPtr, &isEOF);
      if (lineStr == NULL) {
	return NULL;
      }

      rowNum++;

      /* ... DefLine */
      
      if ((defLine = s_ParseDefLine(lineStr, rowNum, errorListPtr)) != NULL)
	{
	  defLine->rowNum = rowNum;
	  lastRowWasOther = FALSE;
	  if (nextRowMustBeSeq)
	    nextRowMustBeSeq = FALSE;

	  /* If we found an ID, then the next */
	  /* row must have a sequence.        */

	  if ((defLine->id != NULL) && (StringLen(defLine->id) != 0))
	    nextRowMustBeSeq = TRUE;
	  else
	    nextRowMustBeSeq = FALSE;

	  /* Add a record for the defline */

	  newRow = ValNodeAdd(&rowList);
	  if (NULL == newRow)
	    {
	      errPtr = Ali_AddError (errorListPtr, ERR_OUT_OF_MEMORY);
	      errPtr->rowNum = rowNum;
	      return NULL;
	    }
	  
	  newRow->choice = ALI_DEFLINE;
	  newRow->data.ptrvalue = defLine;

	}
      
      /* ... Sequence Data */

      else if ((seqLine = s_ParseSequenceLine(lineStr, configPtr))
	       != NULL)
	{
	  seqLine->rowNum = rowNum;

	  /* Is it a Nucleotide sequence or a Protein sequence? */

	  seqLine->type = Ali_SeqLineGetType(seqLine->sequence, configPtr);

	  /* Add a record for the sequence */
	      
	  newRow = ValNodeAdd(&rowList);
	  if (NULL == newRow)
	    {
	      errPtr = Ali_AddError (errorListPtr, ERR_OUT_OF_MEMORY);
	      errPtr->rowNum = rowNum;
	      return NULL;
	    }
	  
	  newRow->data.ptrvalue = seqLine;

	  /* Mark it as a sequence line */
	  
	  if ((seqLine->maybe == FALSE) ||
	      ((seqLine->maybe == TRUE) && (configPtr->useMaybes == TRUE)))
	    {
	      
	      if (StringLen(seqLine->id) != 0)
		lastRowWasOther = FALSE;
	      
	      newRow->choice = ALI_SEQLINE;
	      
	      if (nextRowMustBeSeq)
		nextRowMustBeSeq = FALSE;
	      
	      /* A sequence must follow either a defline, */
	      /* an ID, or another sequence.              */
	      
	      if (lastRowWasOther == TRUE)
		{
		  reEvalSeqPtr = SeqLineReEval (seqLine);
		  if (NULL == reEvalSeqPtr)
		    Ali_ChangeRowToOther(newRow);
		  else
		    newRow->data.ptrvalue = reEvalSeqPtr;
		}
	    }
	  else  /* A 'maybe' sequence that we're not using */
	    {
	      Ali_ChangeRowToOther(newRow);
	      lastRowWasOther = TRUE;
	    }
	}      

      /* ... Other */
      
      else
	{
	  if (StringLen(lineStr) > 0)
	    {
	      if ((otherLine = s_ParseOtherLine(lineStr)) != NULL)
		{
		  otherLine->rowNum = rowNum;
		  if (otherLine->id != NULL)
		    {
		      idFound = TRUE;
		      lastRowWasOther = FALSE;
		    }
		  else
		    {
		      idFound = FALSE;
		      lastRowWasOther = TRUE;
		    }

		  newRow = ValNodeAdd(&rowList);
		  if (NULL == newRow)
		    {
		      errPtr = Ali_AddError (errorListPtr,
					     ERR_OUT_OF_MEMORY);
		      errPtr->rowNum = rowNum;
		      return NULL;
		    }
		  
		  newRow->choice = ALI_OTHERLINE;
		  newRow->data.ptrvalue = otherLine;

		  /* If the next row needs to be a Sequence, */
		  /* and we're not still on the same row,    */
		  /* then change the previous ID to other.   */
		  
		  if (nextRowMustBeSeq && !idFound)
		    nextRowMustBeSeq = FALSE;
		  
		  if (idFound)
		    nextRowMustBeSeq = TRUE;
		  else
		    nextRowMustBeSeq = FALSE;

		  /* Attempt to parse any configuration */
		  /* information from the line.         */

		  if (otherLine->other != NULL)
		    s_ProcessOtherLine (configPtr, otherLine->other, fileInfoPtr);
  
		}
	    }
	}
      MemFree (lineStr);
    }

  return rowList;
}
