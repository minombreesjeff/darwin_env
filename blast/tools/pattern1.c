/* $Id: pattern1.c,v 6.11 1999/09/22 17:51:02 shavirin Exp $
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
*/
 
/*****************************************************************************
File name: pattern1.c
 
Original Author: Zheng Zhang
 
Contents: central pattern matching routines for PHI-BLAST and pseed3

$Revision: 6.11 $ 

$Log: pattern1.c,v $
Revision 6.11  1999/09/22 17:51:02  shavirin
Now functions will collect messages in ValNodePtr before printing out.


*****************************************************************************/


#include <ncbi.h>
#include <objseq.h>
#include <objsset.h>
#include <sequtil.h>
#include <seqport.h>
#include <tofasta.h>
#include <blast.h>
#include <blastpri.h>
#include <txalign.h>
#include <simutil.h>
#include <gapxdrop.h>
#include <posit.h>
#include <readdb.h>
#include <ncbithr.h>
#include <seed.h>

#ifdef __cplusplus
extern "C" {
#endif
#define MININT INT4_MIN/2

 




static Int4 expanding PROTO((Int4 *inputPatternMasked, Uint1 *inputPattern, 
		      Int4 length, Int4 maxLength));
static Int4 packing PROTO((Uint1 *inputPattern, Int4 length));

static void longpacking PROTO((Int4 numPlaces, Uint1 *inputPattern, patternSearchItems *patternSearch));

static void longpacking2 PROTO((Int4 *inputPatternMasked, Int4 numPlacesInPattern, patternSearchItems *patternSearch));

static void init_pattern_DNA PROTO((patternSearchItems *patternSearch));

/*Initialize occurrence probabilities for each amino acid*/
void LIBCALL initProbs(seedSearchItems * seedSearch)
{
   double totalCount;  /*for Robinson frequencies*/
   seedSearch->pchars[0] = '-';
   seedSearch->pchars[1] = 'A';
   seedSearch->pchars[2] = 'B';
   seedSearch->pchars[3] = 'C';
   seedSearch->pchars[4] = 'D';
   seedSearch->pchars[5] = 'E';
   seedSearch->pchars[6] = 'F';
   seedSearch->pchars[7] = 'G';
   seedSearch->pchars[8] = 'H';
   seedSearch->pchars[9] = 'I';
   seedSearch->pchars[10] = 'K';
   seedSearch->pchars[11] = 'L';
   seedSearch->pchars[12] = 'M';
   seedSearch->pchars[13] = 'N';
   seedSearch->pchars[14] = 'P';
   seedSearch->pchars[15] = 'Q';
   seedSearch->pchars[16] = 'R';
   seedSearch->pchars[17] = 'S';
   seedSearch->pchars[18] = 'T';
   seedSearch->pchars[19] = 'V';
   seedSearch->pchars[20] = 'W';
   seedSearch->pchars[21] = 'X';
   seedSearch->pchars[22] = 'Y';
   seedSearch->pchars[23] = 'Z';
   seedSearch->pchars[24] = 'U';
   seedSearch->pchars[25] = '*';
   totalCount = 78.0 + 19.0 + 54.0 + 63.0 + 39.0 +
    74.0 + 22.0 + 52.0 + 57.0 + 90.0 + 22.0 + 45.0 + 52.0 +
     43.0 + 51.0 + 71.0 + 59.0 + 64.0 + 13.0 + 32.0;
   seedSearch->standardProb[0] = 0.0;
   seedSearch->standardProb[1] = 78.0/totalCount; /*A*/
   seedSearch->standardProb[2] = 0.0;
   seedSearch->standardProb[3] = 19.0/totalCount; /*C*/
   seedSearch->standardProb[4] = 54.0/totalCount; /*D*/
   seedSearch->standardProb[5] = 63.0/totalCount; /*E*/
   seedSearch->standardProb[6] = 39.0/totalCount; /*F*/
   seedSearch->standardProb[7] = 74.0/totalCount; /*G*/
   seedSearch->standardProb[8] = 22.0/totalCount; /*H*/
   seedSearch->standardProb[9] = 52.0/totalCount; /*I*/
   seedSearch->standardProb[10] = 57.0/totalCount; /*K*/
   seedSearch->standardProb[11] = 90.0/totalCount; /*L*/
   seedSearch->standardProb[12] = 22.0/totalCount; /*M*/
   seedSearch->standardProb[13] = 45.0/totalCount; /*N*/
   seedSearch->standardProb[14] = 52.0/totalCount; /*P*/
   seedSearch->standardProb[15] = 43.0/totalCount; /*Q*/
   seedSearch->standardProb[16] = 51.0/totalCount; /*R*/
   seedSearch->standardProb[17] = 71.0/totalCount; /*S*/
   seedSearch->standardProb[18] = 59.0/totalCount; /*T*/
   seedSearch->standardProb[19] = 64.0/totalCount; /*V*/
   seedSearch->standardProb[20] = 13.0/totalCount; /*W*/
   seedSearch->standardProb[21] = 0.0;   /*X*/
   seedSearch->standardProb[22] = 32.0/totalCount;   /*Y*/
   seedSearch->standardProb[23] = 0.0;   /*Z*/
   seedSearch->standardProb[24] = 0.0;   /*U*/
}

/*pattern is a string describing the pattern to search for;
  is_dna is a boolean describing the strings are DNA or protein*/
Int4 LIBCALL init_pattern(Uint1 *pattern, Boolean is_dna, patternSearchItems * patternSearch,
	     seedSearchItems *seedSearch, ValNodePtr * error_return)
{
    Int4 i; /*index over string describing the pattern*/
    Int4 j; /*index for position in pattern*/
    Int4 charIndex; /*index over characters in alphabet*/
    Int4 secondIndex; /*second index into pattern*/
    Int4 numIdentical; /*number of consec. positions with identical specification*/
    Int4 charSetMask;  /*index over masks for specific characters*/
    Int4 currentSetMask, prevSetMask ; /*mask for current and previous character positions*/    
    Int4 thisMask;    /*integer representing a bit pattern for a 
                        set of characters*/
    Int4 minWildcard, maxWildcard; /*used for variable number of wildcard
                                     positions*/
    Int4  tj; /*temporary copy of j*/
    Int4 tempInputPatternMasked[MaxP]; /*local copy of parts
            of inputPatternMasked*/
    Uint1 c;  /*character occuring in pattern*/
    Uint1 localPattern[MaxP]; /*local variable to hold
                               for each position whether it is
                               last in pattern (1) or not (0) */
    Nlm_FloatHi positionProbability; /*probability of a set of characters
                                    allowed in one position*/
    Int4 currentWildcardProduct; /*product of wildcard lengths for
                                   consecutive character positions that
                                   overlap*/

    patternSearch->flagPatternLength = ONE_WORD_PATTERN; 
    patternSearch->patternProbability = 1.0;
    patternSearch->minPatternMatchLength = 0;
    patternSearch->wildcardProduct = 1;
    currentWildcardProduct = 1;
    prevSetMask = 0;
    currentSetMask = 0;

    for (i = 0 ; i < MaxP; i++) {
      patternSearch->inputPatternMasked[i] = 0; 
      localPattern[i] = 0;
    }
    for (i = 0, j = 0; i < strlen((Char *) pattern); i++) {
      if ((c=pattern[i]) == '-' || c == '\n' || c == '.' || c =='>' || c ==' ' 
|| c == '<')  /*spacers that mean nothing*/
	continue;
      if ( c != '[' && c != '{') { /*not the start of a set of characters*/
	if (c == 'x' || c== 'X') {  /*wild-card character matches anything*/
          /*next line checks to see if wild card is for multiple positions*/
	  if (pattern[i+1] == '(') {
	    i++;
	    secondIndex = i;
            /*find end of description of how many positions are wildcarded
               will look like x(2) or x(2,5) */
	    while (pattern[secondIndex] != ',' && pattern[secondIndex] != ')')
	      secondIndex++;
	    if (pattern[secondIndex] == ')') {  /*fixed number of positions wildcarded*/
	      i -= 1; 
              /*wildcard, so all characters are allowed*/
	      charSetMask=allone; 
	      positionProbability = 1;
	    }
	    else { /*variable number of positions wildcarded*/	  
	      sscanf((Char*) &pattern[++i], "%d,%d", &minWildcard, &maxWildcard);
	      maxWildcard = maxWildcard - minWildcard;
              currentWildcardProduct *= (maxWildcard + 1);
              if (currentWildcardProduct > patternSearch->wildcardProduct)
		patternSearch->wildcardProduct = currentWildcardProduct;
              patternSearch->minPatternMatchLength += minWildcard;
	      while (minWildcard-- > 0) { 
		/*use one position each for the minimum number of
                  wildcard spaces required */
		patternSearch->inputPatternMasked[j++] = allone; 
		if (j >= MaxP) {
                  BlastConstructErrorMessage("init_pattern", "pattern too long", 1, error_return);
		  /*ErrPostEx(SEV_FATAL, 0, 0, "pattern too long\n");*/
		  return(-1);
		}
	      }
	      if (maxWildcard != 0) {
		/*negative masking used to indicate variability
                  in number of wildcard spaces; e.g., if pattern looks
                  like x(3,5) then variability is 2 and there will
                  be three wildcard positions with mask allone followed
                  by a single position with mask -2*/
		patternSearch->inputPatternMasked[j++] = -maxWildcard;
		patternSearch->patternProbability *= maxWildcard;
	      }
              /*now skip over wildcard description with the i index*/
	      while (pattern[++i] != ')') ; 
	      continue;
	    }
	  }
	  else {  /*wild card is for one position only*/
	    charSetMask=allone; 
	    positionProbability =1;
	  }
	} 
	else {
	  if (c == 'U') {   /*look for special U character*/
	    charSetMask = allone*2+1;
	    positionProbability = 1; 
	  }
	  else { 
            /*exactly one character matches*/
            prevSetMask = currentSetMask;
            currentSetMask =  charSetMask = (1 << seedSearch->order[c]);
            if (!(prevSetMask & currentSetMask)) /*character sets don't overlap*/
	      currentWildcardProduct = 1;
	    positionProbability = seedSearch->standardProb[seedSearch->order[c]];
	  }
	}
      }
      else {
	if (c == '[') {  /*start of a set of characters allowed*/
	  charSetMask = 0;
	  positionProbability = 0;
	  /*For each character in the set add it to the mask and
            add its probability to positionProbability*/
	  while ((c=pattern[++i]) != ']') { /*end of set*/
            if ((c < 'A') || (c > 'Z') || (c == '\0')) {
	      /* ErrPostEx(SEV_FATAL, 0, 0, "your pattern description has a non-alphabetic character inside a bracket\n"); */
              BlastConstructErrorMessage("init_pattern", "your pattern description has a non-alphabetic character inside a bracket", 1, error_return);
              return(-1);
	    }
	    charSetMask = charSetMask | (1 << seedSearch->order[c]);
	    positionProbability += seedSearch->standardProb[seedSearch->order[c]];
	  }
          prevSetMask = currentSetMask;
          currentSetMask = charSetMask;
	  if (!(prevSetMask & currentSetMask)) /*character sets don't overlap*/
	      currentWildcardProduct = 1;
 	} 
	else {   /*start of a set of characters forbidden*/
	  /*For each character forbidden remove it to the mask and
            subtract its probability from positionProbability*/
	  charSetMask = allone; 
	  positionProbability = 1;
	  while ((c=pattern[++i]) != '}') { /*end of set*/
	    charSetMask = charSetMask -  (charSetMask & (1 << seedSearch->order[c]));
	    positionProbability -= seedSearch->standardProb[seedSearch->order[c]];
	  }
          prevSetMask = currentSetMask;
          currentSetMask = charSetMask;
	  if (!(prevSetMask & currentSetMask)) /*character sets don't overlap*/
	      currentWildcardProduct = 1;
	}
      }
      /*handle a number of positions that are the same */
      if (pattern[i+1] == '(') {  /*read opening paren*/
	i++;
	numIdentical = atoi((Char *) &pattern[++i]);  /*get number of positions*/
        patternSearch->minPatternMatchLength += numIdentical;
	while (pattern[++i] != ')') ;  /*skip over piece in pattern*/
	while ((numIdentical--) > 0) {
	  /*set up mask for these positions*/
	  patternSearch->inputPatternMasked[j++] = charSetMask;
	  patternSearch->patternProbability *= positionProbability; 
	}
      } 
      else {   /*specification is for one posiion only*/
	patternSearch->inputPatternMasked[j++] = charSetMask;
        patternSearch->minPatternMatchLength++;
	patternSearch->patternProbability *= positionProbability;
      }
      if (j >= MaxP) {
	BlastConstructErrorMessage("init_pattern", "pattern too long", 1, error_return);
      }
    }
    localPattern[j-1] = 1;
    if (patternSearch->patternProbability > 1.0)
      patternSearch->patternProbability = 1.0;

    for (i = 0; i < j; i++) {
      tempInputPatternMasked[i] = patternSearch->inputPatternMasked[i]; 
      tj = j;
    }
    j = expanding(patternSearch->inputPatternMasked, localPattern, j, MaxP);
    if ((j== -1) || ((j > BITS_PACKED_PER_WORD) && is_dna)) {
      patternSearch->flagPatternLength = PATTERN_TOO_LONG;
      longpacking2(tempInputPatternMasked, tj, patternSearch);
      for (i = 0; i < tj; i++) 
	patternSearch->inputPatternMasked[i] = tempInputPatternMasked[i];
      patternSearch->highestPlace = tj;
      if (is_dna) 
	init_pattern_DNA(patternSearch);
      return 1;
    }
    if (j > BITS_PACKED_PER_WORD) {
      patternSearch->flagPatternLength = MULTI_WORD_PATTERN;
      longpacking(j, localPattern, patternSearch);
      return j;
    } 
    /*make a bit mask out of local pattern of length j*/
    patternSearch->match_mask = packing(localPattern, j);
    /*store for each character a bit mask of which positions
      that character can occur in*/
    for (charIndex = 0; charIndex < ALPHABET_SIZE; charIndex++) {
      thisMask = 0;
      for (charSetMask = 0; charSetMask < j; charSetMask++) {
	if ((1<< charIndex) & patternSearch->inputPatternMasked[charSetMask]) 
	  thisMask |= (1 << charSetMask);
      }
      patternSearch->whichPositionsByCharacter[charIndex] = thisMask;
    }
    patternSearch->whichPositionPtr = patternSearch->whichPositionsByCharacter;
    if (is_dna) 
      init_pattern_DNA(patternSearch);
    return j; /*return number of places for pattern representation*/
}

/*Looks for 1 bits in the same position of s and mask
  Let rightOne be the rightmost position where s and mask both have
  a 1.
  Let rightMaskOnly < rightOne be the rightmost position where mask has a 1, if any
     or -1 otherwise
  returns (rightOne - rightMaskOnly) */
  
static Int4 lenof(Int4 s, Int4 mask)
{
    Int4 checkingMatches = s & mask;  /*look for 1 bits in same position*/
    Int4 rightOne; /*loop index looking for 1 in checkingMatches*/
    Int4 rightMaskOnly; /*rightnost bit that is 1 in the mask only*/
    rightMaskOnly = -1;
    /*AAS Changed upper bound on loop here*/
    for (rightOne = 0; rightOne < BITS_PACKED_PER_WORD; rightOne++) {
	if ((checkingMatches >> rightOne) % 2  == 1) 
	  return rightOne - rightMaskOnly;
	if ((mask >> rightOne) %2  == 1) 
	  rightMaskOnly = rightOne;
    }
    ErrPostEx(SEV_FATAL, 0, 0, "wrong\n");
    return(-1);
}

/* routine to find hits of pattern to sequence when sequence is proteins
   hitArray is an array of matches to pass back
   seq is the input sequence
   len1 is the length of the input sequence
   patternSearch carries variables that keep track of
      search parameters
   returns the number of matches*/
static Int4 find_hitsS(Int4 *hitArray, Uint1Ptr seq, Int4 len1, 
		patternSearchItems *patternSearch)
{
    Int4 i; /*loop index on sequence*/
    Int4 prefixMatchedBitPattern = 0; /*indicates where pattern aligns
                 with seq; e.g., if value is 9 = 0101 then 
                 last 3 chars of seq match first 3 positions in pattern
                 and last 1 char of seq matches 1 position of pattern*/
    Int4 numMatches = 0; /*number of matches found*/
    Int4 mask;  /*mask of input pattern positions after which
                  a match can be declared*/
    Int4 maskShiftPlus1; /*mask shifted left 1 plus 1 */

    mask = patternSearch->match_mask; 
    maskShiftPlus1 = (mask << 1) + 1;
    for (i = 0; i < len1; i++) {
      /*shift the positions matched by 1 and try to match up against
        the next character, also allow next character to match the
        first position*/
      prefixMatchedBitPattern =  
	((prefixMatchedBitPattern << 1) | maskShiftPlus1) & 
	patternSearch->whichPositionPtr[seq[i]];
      if (prefixMatchedBitPattern & mask) { 
        /*first part of pair is index of place in seq where match
          ends; second part is where match starts*/
	hitArray[numMatches++] = i;
	hitArray[numMatches++] = i - lenof(prefixMatchedBitPattern, mask)+1;
      }
    }
    return numMatches;
}

/*set uo matches for words that encode 4 DNA characters; figure out
  for each of 256 possible DNA 4-mers, where a prefix matches the pattern
 and where a suffix matches the pattern; store in prefixPos and
 suffixPos; mask has 1 bits for whatever lengths of string
the pattern can match, mask2 has 4 1 bits corresponding to
the last 4 positions of a match; they are used to
do the prefixPos and suffixPos claculations with bit arithmetic*/
static void setting_tt(Int4Ptr S, Int4 mask, Int4 mask2, Uint4Ptr prefixPos, 
		       Uint4Ptr suffixPos)
{
  Int4 i; /*index over possible DNA encoded words, 4 bases per word*/
  Int4 tmp; /*holds different mask combinations*/
  Int4 maskLeftPlusOne; /*mask shifted left 1 plus 1; guarantees 1
                           1 character match effectively */
  Uint1 a1, a2, a3, a4;  /*four bases packed into an integer*/

  maskLeftPlusOne = (mask << 1)+1;
  for (i = 0; i < ASCII_SIZE; i++) {
    /*find out the 4 bases packed in integer i*/
    a1 = READDB_UNPACK_BASE_1(i);
    a2 = READDB_UNPACK_BASE_2(i);
    a3 = READDB_UNPACK_BASE_3(i);
    a4 = READDB_UNPACK_BASE_4(i);
    /*what positions match a prefix of a4 followed by a3*/
    tmp = ((S[a4]>>1) | mask) & S[a3];
    /*what positions match a prefix of a4 followed by a3 followed by a2*/
    tmp = ((tmp >>1) | mask) & S[a2];
    /*what positions match a prefix of a4, a3, a2,a1*/
    prefixPos[i] = mask2 & ((tmp >>1) | mask) & S[a1];
    
    /*what positions match a suffix of a2, a1*/
    tmp = ((S[a1]<<1) | maskLeftPlusOne) & S[a2];
    /* what positions match a suffix of a3, a2, a1*/
    tmp = ((tmp <<1) | maskLeftPlusOne) & S[a3];
    /*what positions match a suffix of a4, a3, a2, a1*/
    suffixPos[i] = ((((tmp <<1) | maskLeftPlusOne) & S[a4]) << 1) | maskLeftPlusOne;
  }
}


/*find hits when sequence is DNA and pattern is short
  returns twice the number of hits
  pos indicates the starting position
  len is length of sequence seq
  hitArray stores the results*/
static Int4 find_hitsS_DNA(Int4Ptr hitArray, Uint1Ptr seq, Char pos, Int4 len,
	       patternSearchItems *patternSearch)
{
  /*Some variables and the algorithm are similar to what is
    used in find_hits() above; see more detailed comments there*/
  Uint4 prefixMatchedBitPattern; /*indicates where pattern aligns
                                  with sequence*/
  Uint4 mask2; /*mask to match agaist*/
  Int4 maskShiftPlus1; /*mask2 shifted plus 1*/
  Uint4 tmp; /*intermediate result of masked comparisons*/
  Int4 i; /*index on seq*/
  Int4 end; /*count of number of 4-mer iterations needed*/
  Int4 remain; /*0,1,2,3 DNA letters left over*/
  Uint1 suffixRemnant; /*part of seq that is beyond last 4-mer in a byte*/
  Int4 j; /*index on suffixRemnant*/
  Int4 twiceNumHits = 0; /*twice the number of hits*/


  mask2 = patternSearch->match_mask*BITS_PACKED_PER_WORD+15; 
  maskShiftPlus1 = (patternSearch->match_mask << 1)+1;
  if (pos != 0) {
    prefixMatchedBitPattern = ((patternSearch->match_mask * ((1 << (pos+1))-1)*2) +
	 (1 << (pos+1))-1)& patternSearch->DNAwhichSuffixPosPtr[seq[0]];
    seq++;
  } 
  else {
    prefixMatchedBitPattern = maskShiftPlus1;
  }
  end = (len-pos)/4; 
  remain = (len-pos) % 4;
  for (i = 0; i < end; i++) {
    if (tmp = (prefixMatchedBitPattern & patternSearch->DNAwhichPrefixPosPtr[seq[i]])) {
      for (j = 0; j < 4; j++) {
	if (tmp & patternSearch->match_mask) {
	  hitArray[twiceNumHits++] = pos+i*4+j;
	  hitArray[twiceNumHits++] = pos+i*4+j-lenof(tmp & patternSearch->match_mask, 
					  patternSearch->match_mask)+1;
	}
	tmp = (tmp << 1);
      }
    }
    prefixMatchedBitPattern = (((prefixMatchedBitPattern << 4) | mask2) & patternSearch->DNAwhichSuffixPosPtr[seq[i]]);
  }
  suffixRemnant = seq[i]; 
  j = 0; 
  while (j < remain) {
    prefixMatchedBitPattern =  ((prefixMatchedBitPattern << 1) | maskShiftPlus1) & patternSearch->whichPositionPtr[seq[i]];
    if (prefixMatchedBitPattern & patternSearch->match_mask) { 
      hitArray[twiceNumHits++] = pos+i*4+j;
      hitArray[twiceNumHits++] = pos+i*4+j-lenof(prefixMatchedBitPattern, patternSearch->match_mask)+1;
    }
    j++;
    suffixRemnant <<= 2;
  }
  return twiceNumHits;
}

/*Top level routine when pattern has a short description
  hitArray is to return the hits
  seq is the input sequence
  start is what position to start at in seq
  len is the length of seq
  is_dna is 1 if and only if seq is a DNA sequence
  the return value from the appropriate lower level routine is passed
  back*/
static Int4  find_hitsS_head(Int4Ptr hitArray, Uint1Ptr seq, Int4 start, Int4 len, 
		      Uint1 is_dna, patternSearchItems *patternSearch)
{
  if (is_dna) 
    return find_hitsS_DNA(hitArray, &seq[start/4], start % 4, len, patternSearch);
  return find_hitsS(hitArray, &seq[start], len, patternSearch);
}

/*length is the length of inputPattern, maxLength is a limit on
   how long inputPattern can get;
   return the final length of the pattern or -1 if too long*/
static Int4 expanding(Int4 *inputPatternMasked, Uint1 *inputPattern, 
		      Int4 length, Int4 maxLength)
{
    Int4 i, j; /*pattern indices*/
    Int4 numPos; /*number of positions index*/
    Int4  k, t; /*loop indices*/
    Int4 recReturnValue1, recReturnValue2; /*values returned from
                                             recursive calls*/
    Int4 thisPlaceMasked; /*value of one place in inputPatternMasked*/
    Int4 tempPatternMask[MaxP]; /*used as a local representation of
                               part of inputPatternMasked*/
    Uint1 tempPattern[MaxP]; /*used as a local representation of part of
                               inputPattern*/

    for (i = 0; i < length; i++) {
      thisPlaceMasked = -inputPatternMasked[i];
      if (thisPlaceMasked > 0) {  /*represented variable wildcard*/
	inputPatternMasked[i] = allone;
	for (j = 0; j < length; j++) {
	  /*use this to keep track of pattern*/
	  tempPatternMask[j] = inputPatternMasked[j]; 
	  tempPattern[j] = inputPattern[j];
	}
	recReturnValue2 = recReturnValue1 = 
	  expanding(inputPatternMasked, inputPattern, length, maxLength);
	if (recReturnValue1 == -1)
	  return -1;
	for (numPos = 0; numPos <= thisPlaceMasked; numPos++) {
	  if (numPos == 1)
	    continue;
	  for (k = 0; k < length; k++) {
	    if (k == i) {
	      for (t = 0; t < numPos; t++) {
		inputPatternMasked[recReturnValue1++] = allone;
                if (recReturnValue1 >= maxLength)
                  return(-1);
	      }
	    }
	    else {
	      inputPatternMasked[recReturnValue1] = tempPatternMask[k];
	      inputPattern[recReturnValue1++] = tempPattern[k];
              if (recReturnValue1 >= maxLength)
                  return(-1);
	    }
	    if (recReturnValue1 >= maxLength) 
	      return (-1);
	  }
	  recReturnValue1 = 
	    expanding(&inputPatternMasked[recReturnValue2], 
		      &inputPattern[recReturnValue2], 
		      length + numPos - 1, 
		      maxLength - recReturnValue2);
	  if (recReturnValue1 == -1) 
	    return -1;
	  recReturnValue2 += recReturnValue1; 
	  recReturnValue1 = recReturnValue2;
	}
	return recReturnValue1;
      }
    }
    return length;
}

/*Pack the next length bytes of inputPattern into a bit vector
  where the bit is 1 if and only if the byte is non-0 
  Returns packed bit vector*/
static Int4 packing(Uint1 *inputPattern, Int4 length)
{
    Int4 i; /*loop index*/
    Int4 returnValue = 0; /*value to return*/
    for (i = 0; i < length; i++) {
      if (inputPattern[i])
	returnValue += (1 << i);
    }
    return returnValue;
}


/*Pack the bit representation of the inputPattern into
   the array patternSearch->match_maskL
   numPlaces is the number of positions in
   inputPattern
   Also packs patternSearch->bitPatternByLetter  */
static void longpacking(Int4 numPlaces, Uint1 *inputPattern, patternSearchItems *patternSearch)
{
    Int4 charIndex; /*index over characters in alphabet*/
    Int4 bitPattern; /*bit pattern for one word to pack*/
    Int4 i;  /*loop index over places*/
    Int4 wordIndex; /*loop counter over words to pack into*/
    
    patternSearch->numWords = (numPlaces-1) / BITS_PACKED_PER_WORD +1;

    for (wordIndex = 0; wordIndex < patternSearch->numWords; wordIndex++) {
      bitPattern = 0;
      for (i = 0; i < BITS_PACKED_PER_WORD; i++) {
	if (inputPattern[wordIndex*BITS_PACKED_PER_WORD+i]) 
	  bitPattern += (1 << i);
      }
      patternSearch->match_maskL[wordIndex] = bitPattern;
    }
    for (charIndex = 0; charIndex < ALPHABET_SIZE; charIndex++) {
      for (wordIndex = 0; wordIndex < patternSearch->numWords; wordIndex++) {
	bitPattern = 0;
	for (i = 0; i < BITS_PACKED_PER_WORD; i++) {
	  if ((1<< charIndex) & patternSearch->inputPatternMasked[wordIndex*BITS_PACKED_PER_WORD + i]) 
	    bitPattern = bitPattern | (1 << i);
	}
	patternSearch->bitPatternByLetter[charIndex][wordIndex] = 
	  bitPattern;
	}
    }
}


/*Let F be the number of the first bit in s that is 1
  Let G be the first bit in mask that is one such that G < F;
  Else let G = -1;
  Returns F - G*/
static Int4 lenofL(Int4 *s, Int4 *mask, patternSearchItems *patternSearch)
{
    Int4 bitIndex; /*loop index over bits in a word*/
    Int4 wordIndex;  /*loop index over words*/
    Int4 firstOneInMask;

    firstOneInMask = -1;
    for (wordIndex = 0; wordIndex < patternSearch->numWords; wordIndex++) {
      for (bitIndex = 0; bitIndex < BITS_PACKED_PER_WORD; bitIndex++) { 
	if ((s[wordIndex] >> bitIndex) % 2  == 1) 
	  return wordIndex*BITS_PACKED_PER_WORD+bitIndex-firstOneInMask;
	if ((mask[wordIndex] >> bitIndex) %2  == 1) 
	  firstOneInMask = wordIndex*BITS_PACKED_PER_WORD+bitIndex;
      }
    }
    ErrPostEx(SEV_FATAL, 0, 0, "wrong\n");
    return(-1);
}

/*Shift each word in the array left by 1 bit and add bit b,
  if the new values is bigger that OVERFLOW1, then subtract OVERFLOW1 */
static void lmove(Int4 *a, Uint1 b, patternSearchItems *patternSearch)
{
    Int4 x;
    Int4 i; /*index on words*/
    for (i = 0; i < patternSearch->numWords; i++) {
      x = (a[i] << 1) + b;
      if (x >= OVERFLOW1) {
	a[i] = x - OVERFLOW1; 
	b = 1;
      }
      else { 
	a[i] = x; 
	b = 0;
      }
    }
}  

/*Do a word-by-word bit-wise or of a and b and put the result back in a*/
static void or(Int4 *a, Int4 *b, patternSearchItems *patternSearch)
{
    Int4 i; /*index over words*/
    for (i = 0; i < patternSearch->numWords; i++) 
	a[i] = (a[i] | b[i]);
}

/*Do a word-by-word bit-wise or of a and b and put the result in
  result; return 1 if there are any non-zero words*/
static and(Int4 *result, Int4 *a, Int4 *b, patternSearchItems *patternSearch)
{
    Int4 i; /*index over words*/
    Int4 returnValue = 0;

    for (i = 0; i < patternSearch->numWords; i++) 
      if (result[i] = (a[i] & b[i])) 
	returnValue = 1;
    return returnValue;
}

/* Finds places where pattern matches seq and returns them as
   pairs of positions in consecutive entries of hitArray;
   similar to find_hitsS
   hitArray is array of hits to return
   seq is the input sequence and len1 is its length
   patternSearch carries all the pattern variables
   return twice the number of hits*/
static Int4 find_hitsL(Int4 *hitArray, Uint1Ptr seq, Int4 len1, 
		       patternSearchItems *patternSearch)
{
    Int4 wordIndex; /*index on words in mask*/
    Int4 i; /*loop index on seq */
    Int4  *prefixMatchedBitPattern; /*see similar variable in
                                      find_hitsS*/
    Int4 twiceNumHits = 0; /*counter for hitArray*/
    Int4 *mask; /*local copy of match_maskL version of pattern
                  indicates after which positions a match can be declared*/
    Int4 *matchResult; /*Array of words to hold the result of the
                         final test for a match*/

    matchResult = (Int4 *) ckalloc(sizeof(Int4)*patternSearch->numWords);
    mask = (Int4 *) ckalloc(sizeof(Int4)*patternSearch->numWords);
    prefixMatchedBitPattern = (Int4 *) ckalloc(sizeof(Int4)*patternSearch->numWords);
    for (wordIndex = 0; wordIndex < patternSearch->numWords; wordIndex++) {
      mask[wordIndex] = patternSearch->match_maskL[wordIndex];
      prefixMatchedBitPattern[wordIndex] = 0;
    }
    /*This is a multiword version of the algorithm in find_hitsS*/
    lmove(mask, 1, patternSearch);
    for (i = 0; i < len1; i++) {
      lmove(prefixMatchedBitPattern, 0, patternSearch);
      or(prefixMatchedBitPattern, mask, patternSearch); 
      and(prefixMatchedBitPattern, prefixMatchedBitPattern, patternSearch->bitPatternByLetter[seq[i]], patternSearch);
      if (and(matchResult, prefixMatchedBitPattern, patternSearch->match_maskL, patternSearch)) { 
	hitArray[twiceNumHits++] = i; 
	hitArray[twiceNumHits++] = i-lenofL(matchResult, patternSearch->match_maskL, patternSearch)+1;
      }
    }
    MemFree(prefixMatchedBitPattern); 
    MemFree(matchResult); 
    MemFree(mask);
    return twiceNumHits;
}


/*initialize mask and other arrays for DNA patterns*/
static void init_pattern_DNA(patternSearchItems *patternSearch)
{
  Int4 mask1; /*mask for one word in a set position*/
  Int4 compositeMask; /*superimposed mask1 in 4 adjacent positions*/
  Int4 wordIndex; /*index over words in pattern*/

  if (patternSearch->flagPatternLength != ONE_WORD_PATTERN) {
    for (wordIndex = 0; wordIndex < patternSearch->numWords; wordIndex++) {
      mask1 = patternSearch->match_maskL[wordIndex];
      compositeMask = mask1 + (mask1>>1)+(mask1>>2)+(mask1>>3);
      setting_tt(patternSearch->SLL[wordIndex], 
      patternSearch->match_maskL[wordIndex], 
	 compositeMask, patternSearch->DNAprefixSLL[wordIndex], patternSearch->DNAsuffixSLL[wordIndex]);
    }
  } 
  else {
    compositeMask = patternSearch->match_mask + 
      (patternSearch->match_mask>>1) + 
      (patternSearch->match_mask>>2) + (patternSearch->match_mask>>3); 
    patternSearch->DNAwhichPrefixPosPtr = patternSearch->DNAwhichPrefixPositions; 
    patternSearch->DNAwhichSuffixPosPtr = patternSearch->DNAwhichSuffixPositions;
    setting_tt(patternSearch->whichPositionsByCharacter, 
    patternSearch->match_mask, compositeMask, 
    patternSearch->DNAwhichPrefixPositions, patternSearch->DNAwhichSuffixPositions);
  }
}

/*Return the number of 1 bits in the base 2 representation of a*/
static Int4 num_of_one(Int4 a)
{
  Int4 returnValue;
  returnValue = 0;
  while (a > 0) {
    if (a % 2 == 1) 
      returnValue++;
    a = (a >> 1);
  }
  return returnValue;
}

/*Sets up field in patternSearch when pattern is very long*/
static void longpacking2(Int4 *inputPatternMasked, Int4 numPlacesInPattern, patternSearchItems *patternSearch)
{
    Int4 placeIndex; /*index over places in pattern rep.*/
    Int4 wordIndex; /*index over words*/
    Int4 placeInWord, placeInWord2;  /*index for places in a single word*/
    Int4 charIndex; /*index over characters in alphabet*/
    Int4 oneWordMask; /*mask of matching characters for one word in
                        pattern representation*/
    Nlm_FloatHi patternWordProbability;
    Nlm_FloatHi  most_specific; /*lowest probability of a word in the pattern*/
    Int4 *oneWordSLL; /*holds patternSearch->SLL for one word*/

    most_specific = 1.0; 
    patternSearch->whichMostSpecific = 0; 
    patternWordProbability = 1.0;
    for (placeIndex = 0, wordIndex = 0, placeInWord=0; 
	 placeIndex <= numPlacesInPattern; 	 placeIndex++, placeInWord++) {
      if (placeIndex==numPlacesInPattern || inputPatternMasked[placeIndex] < 0 
	  || placeInWord == BITS_PACKED_PER_WORD ) {
	patternSearch->match_maskL[wordIndex] = 1 << (placeInWord-1);
	oneWordSLL = patternSearch->SLL[wordIndex];
	for (charIndex = 0; charIndex < ALPHABET_SIZE; charIndex++) {
	  oneWordMask = 0;
	  for (placeInWord2 = 0; placeInWord2 < placeInWord; placeInWord2++) {
	    if ((1<< charIndex) & 
		inputPatternMasked[placeIndex-placeInWord+placeInWord2]) 
	      oneWordMask |= (1 << placeInWord2);
	  }
	  oneWordSLL[charIndex] = oneWordMask;
	}
	patternSearch->numPlacesInWord[wordIndex] = placeInWord;
	if (patternWordProbability < most_specific) {
	  most_specific = patternWordProbability;
	  patternSearch->whichMostSpecific = wordIndex;
	}
	if (placeIndex == numPlacesInPattern) 
	  patternSearch->spacing[wordIndex++] = 0; 
	else 
	  if (inputPatternMasked[placeIndex] < 0) { 
	    patternSearch->spacing[wordIndex++] = -inputPatternMasked[placeIndex];
	  }
	  else { 
	    placeIndex--; 
	    patternSearch->spacing[wordIndex++] = 0;
	  }
	placeInWord = -1; 
	patternWordProbability = 1.0;
      }
      else {
	patternWordProbability *= (Nlm_FloatHi) 
	  num_of_one(inputPatternMasked[placeIndex])/ (Nlm_FloatHi) ALPHABET_SIZE;
	}
    }
    patternSearch->numWords = wordIndex;
}

/*find matches when pattern is very long,
  hitArray is used to pass back pairs of end position. start position for hits
  seq is the sequence; len is its length
  is_dna indicates if the sequence is DNA or protein*/
static Int4 find_hitsLL(Int4 *hitArray, Uint1Ptr seq, Int4 len, Boolean is_dna,
		 patternSearchItems *patternSearch)
{
    Int4 twiceNumHits; /*twice the number of matches*/
    Int4 twiceHitsOneCall; /*twice the number of hits in one call to 
                                 find_hitsS */
    Int4 wordIndex;  /*index over words in pattern*/
    Int4 start; /*start position in sequence for calls to find_hitsS */
    Int4 hitArray1[MAX_HIT]; /*used to get hits against different words*/
    Int4 nextPosInHitArray; /*next available position in hitArray1 */
    Int4 hitIndex1, hitIndex2;  /*indices over hitArray1*/

    patternSearch->whichPositionPtr = 
      patternSearch->SLL[patternSearch->whichMostSpecific]; 
    patternSearch->match_mask = 
      patternSearch->match_maskL[patternSearch->whichMostSpecific];
    if (is_dna) {
      patternSearch->DNAwhichPrefixPosPtr = patternSearch->DNAprefixSLL[patternSearch->whichMostSpecific];
      patternSearch->DNAwhichSuffixPosPtr = patternSearch->DNAsuffixSLL[patternSearch->whichMostSpecific];
    }
    /*find matches to most specific word of pattern*/
    twiceNumHits = find_hitsS_head(hitArray, seq, 0, len, is_dna, patternSearch);
    if (twiceNumHits < 2) 
      return 0;
    /*extend matches word by word*/
    for (wordIndex = patternSearch->whichMostSpecific+1; 
	 wordIndex < patternSearch->numWords; wordIndex++) {
	patternSearch->whichPositionPtr = 
	  patternSearch->SLL[wordIndex]; 
	patternSearch->match_mask = patternSearch->match_maskL[wordIndex];
	if (is_dna) {
	  patternSearch->DNAwhichPrefixPosPtr = patternSearch->DNAprefixSLL[wordIndex]; 
	  patternSearch->DNAwhichSuffixPosPtr = patternSearch->DNAsuffixSLL[wordIndex];
	}
	nextPosInHitArray = 0;
	for (hitIndex2 = 0; hitIndex2 < twiceNumHits; hitIndex2 += 2) {
	  twiceHitsOneCall = find_hitsS_head(&hitArray1[nextPosInHitArray], seq, 
       hitArray[hitIndex2]+1, MIN(len-hitArray[hitIndex2]-1, 
       patternSearch->spacing[wordIndex-1] + patternSearch->numPlacesInWord[wordIndex]), is_dna, patternSearch);
	  for (hitIndex1 = 0; hitIndex1 < twiceHitsOneCall; hitIndex1+= 2) {
	    hitArray1[nextPosInHitArray+hitIndex1] = 
	      hitArray[hitIndex2]+hitArray1[nextPosInHitArray+hitIndex1]+1;
	    hitArray1[nextPosInHitArray+hitIndex1+1] = hitArray[hitIndex2+1];
	  }
	  nextPosInHitArray += twiceHitsOneCall;
	}
	twiceNumHits = nextPosInHitArray;
	if (twiceNumHits < 2) 
	  return 0;
        /*copy back matches that extend */
	for (hitIndex2 = 0; hitIndex2 < nextPosInHitArray; hitIndex2++) 
	  hitArray[hitIndex2] = hitArray1[hitIndex2];
    }
    /*extend each match back one word at a time*/
    for (wordIndex = patternSearch->whichMostSpecific-1; wordIndex >=0; 
	 wordIndex--) {
      patternSearch->whichPositionPtr = 
	patternSearch->SLL[wordIndex]; 
      patternSearch->match_mask = patternSearch->match_maskL[wordIndex];
      if (is_dna) {
	patternSearch->DNAwhichPrefixPosPtr = patternSearch->DNAprefixSLL[wordIndex]; 
	patternSearch->DNAwhichSuffixPosPtr = patternSearch->DNAsuffixSLL[wordIndex];
      }
      nextPosInHitArray = 0;
      for (hitIndex2 = 0; hitIndex2 < twiceNumHits; hitIndex2 += 2) {
	start = hitArray[hitIndex2+1]-patternSearch->spacing[wordIndex]-patternSearch->numPlacesInWord[wordIndex];
	if (start < 0) 
	  start = 0;
	twiceHitsOneCall = find_hitsS_head(&hitArray1[nextPosInHitArray], seq, start, 
			    hitArray[hitIndex2+1]-start, is_dna, patternSearch);
	for (hitIndex1 = 0; hitIndex1 < twiceHitsOneCall; hitIndex1+= 2) {
	  hitArray1[nextPosInHitArray+hitIndex1] = hitArray[hitIndex2];
	  hitArray1[nextPosInHitArray+hitIndex1+1] = start + 
	    hitArray1[nextPosInHitArray+hitIndex1+1];
	}
	nextPosInHitArray += twiceHitsOneCall;
      }
      twiceNumHits = nextPosInHitArray;
      if (twiceNumHits < 2) 
	return 0;
      /*copy back matches that extend*/
      for (hitIndex2 = 0; hitIndex2 < nextPosInHitArray; hitIndex2++) 
	hitArray[hitIndex2] = hitArray1[hitIndex2];
    }
    return twiceNumHits;
}

/*stores dynamic programming information */
typedef struct DP {
    Int4 CC; /*minimum cost for these coordiantes*/
    Int4 DD; /*minimum cost when preceded by a deletion (vertical edge)
               for these coordinates*/
} *dp_ptr, dp_node;

#define gap(k)  ((k) <= 0 ? 0 : (alignSearch->gapOpen+alignSearch->gapExtend*(k)))	/* k-symbol indel cost */

typedef struct {
  Int4Ptr sapp;			/* Current script append ptr */
  Int4  last;	               /*last edit operation*/
} data_dp;

						/* Append "Delete k" op */
#define DEL_(k)				\
{ 					\
  if (data->last < 0)				\
    data->last = data->sapp[-1] -= (k);		\
  else					\
    data->last = (*data->sapp++ = -(k));		\
}
						/* Append "Insert k" op */
#define INS_(k)				\
{ 					\
  if (data->last > 0)				\
    data->last = data->sapp[-1] += (k);		\
  else					\
    data->last = (*data->sapp++ = (k));		\
}

						/* Append "Replace" op */
#define REP_ 				\
{ data->last = (*data->sapp++ = 0); 			\
}

/* Returns the cost of an optimum conversion 
   within highDiag and lowDiag between
   seq1[1..end1] and seq2[1..end2] and appends such a conversion 
   to the current script.
Score_only is 1 if only the score is desired, not the conversion script.
              0 otherwise
   CD and data stores dynamic program intermediate scores
*/
static Int4 align(Uint1 * seq1, Uint1 * seq2, Int4 end1, Int4 end2,
  Int4 lowDiag, Int4 highDiag, Char score_only, dp_ptr CD, alignSearchItems * alignSearch, data_dp * data)
{
        Int4 nextState; /*stores code for next entry in state*/
        Int4 score; /*score to return*/
	Int4 band; /*number of diagonals between highDiag and lowDiag 
                     inclusive*/
        Int4 diagIndex; /*loop index over diagonals*/
	Int4 leftd, rightd;	/* diagonal indices for CC, DD, CP and DP */
	register Int4 curd;	/* current index for CC, DD CP and DP */
	register Int4 i;  /*loop index*/
        register Int4 index1; /*index on seq1*/
	register Int4 tempCC; /*placeholder for a CC field*/
	register Int4 tempDD; /*placeholder for a DD field*/
	register Int4 tempHorScore; /*dual of tempDD for the case where a
                              horizontal edge (insertion) is the last step*/
	register dp_ptr CDrow; /*points to a row of CD*/
	Int4 stateDecoder; /*used to decode the edge information in a state*/
        Int4 initialScore; /*score to initialize dynamic program entries*/
        Int4 *matrixRow; /*row of score matrix*/
	Int1 **state; /*stores dynamic program information*/
        Int1 *stateRow; /*holds one row of state*/
        Int1 *editInstructions; /*holds instruction for string-to-string edit*/
	Int4 index2; /*index on seq2*/
        Int4 charCounter; /*counts number of characters involved in 
                            optimal edit sequence*/
        Int4 charIndex; /*index over character positions in optimal
                          edit sequence*/
        Int4 editStep, nextEditStep; /*steps in string conversion sequence*/


	/* Boundary cases: end1 <= 0 , end2 <= 0, or highDiag-lowDiag <= 0 */
	band = highDiag-lowDiag+1;
	state = (Int1 **) ckalloc(sizeof(Int1 *)*(end1+1));
	state[0] = (Int1 *) ckalloc((end1+1)*(band+2));
	for (index1 = 1; index1 <= end1; index1++) 
	  state[index1] = state[index1-1]+band+2;

	/* Initialization */
	leftd = 1-lowDiag;
	rightd = highDiag-lowDiag+1;

	CD[leftd].CC = 0; 
	state[0][leftd] = -1;
	initialScore = -alignSearch->gapOpen;
	for (diagIndex = leftd+1; diagIndex <= rightd; diagIndex++) {
	  CD[diagIndex].CC = initialScore = initialScore-alignSearch->gapExtend;
	  CD[diagIndex-1].DD = initialScore-alignSearch->gapCost;
	  state[0][diagIndex] = DIAGONAL_INSERT;
	}
	CD[rightd+1].CC = MININT;
	CD[rightd].DD = MININT;
	CD[leftd-1].DD = -alignSearch->gapCost;
	CD[leftd-1].CC = MININT;
	for (i = 1; i <= end1; i++) {
	  if (i > end2-highDiag) 
	    rightd--;
	  if (leftd > 1) 
	    leftd--;
	  matrixRow = alignSearch->matrix[seq1[i]];
	  tempDD = CD[leftd].DD;
	  nextState = 0;
	  if ((index2 = leftd+lowDiag-1+i) > 0) 
	    tempCC = CD[leftd].CC+matrixRow[seq2[index2]];
	  if (tempDD > tempCC || index2 <= 0) {
	    tempCC = tempDD;
	    nextState = DIAGONAL_DELETE;
	  }
	  tempHorScore = tempCC-alignSearch->gapCost;
	  if (leftd >= 1) 
	    if ((tempDD-= alignSearch->gapExtend) >= tempHorScore) {
	      CD[leftd-1].DD = tempDD;
	      nextState += DELETE_CODE;
	    } 
	    else {
	      CD[leftd-1].DD = tempHorScore;
	    }
	  stateRow = &state[i][leftd];
	  *stateRow++ = nextState;
	  CD[leftd].CC = tempCC;
	  for (curd=leftd+1, CDrow = &CD[curd]; curd <= rightd; curd++) {
	    tempCC = CDrow->CC + matrixRow[seq2[curd+lowDiag-1+i]];
	    if ((tempDD=CDrow->DD) > tempCC) { 
	      if (tempDD > tempHorScore) {
		CDrow->CC = tempDD; 
		tempHorScore -= alignSearch->gapExtend; 
		(CDrow++-1)->DD = tempDD-alignSearch->gapExtend; 
		*stateRow++=DELETE_CODE + INSERT_CODE + DIAGONAL_DELETE;
	      } 
	      else {
		CDrow->CC = tempHorScore; 
		tempHorScore -= alignSearch->gapExtend;
		(CDrow++-1)->DD = tempDD-alignSearch->gapExtend; 
		*stateRow++=DELETE_CODE + INSERT_CODE + DIAGONAL_INSERT;
	      }
	    } 
	    else  
	      if (tempHorScore > tempCC) { 	       
		CDrow->CC = tempHorScore; 
		tempHorScore -= alignSearch->gapExtend;
		(CDrow++-1)->DD = tempDD-alignSearch->gapExtend; 
		*stateRow++=DELETE_CODE + INSERT_CODE + DIAGONAL_INSERT;;
	      }
	      else {
		CDrow->CC = tempCC;
		if ((tempCC -= alignSearch->gapCost) > 
		    (tempHorScore-=alignSearch->gapExtend)) {
		  tempHorScore = tempCC;
		  nextState = 0;
		} 
		else
		  nextState = INSERT_CODE;
		if (tempCC > (tempDD -= alignSearch->gapExtend)) { 
		  *stateRow++= nextState; 
		  (CDrow++-1)->DD = tempCC;
		} 
		else { 
		  *stateRow++ = nextState+DELETE_CODE; 
		  (CDrow++-1)->DD = tempDD;
		}
	      }
	  }
	}

	/* decide which path to be traced back */
	score = (CDrow-1)->CC;
	if (score_only) {
	  MemFree(state[0]); 
	  MemFree(state);
	  return score;
	}
	editInstructions = (Int1Ptr) ckalloc(end1+end2);
	for (index1 = end1, diagIndex = rightd, editStep=0, charCounter = 0; 
	     index1>=0; index1--, charCounter++) {
	  nextEditStep  = (stateDecoder=state[index1][diagIndex]) % INSERT_CODE;
	  if (stateDecoder == -1) 
	    break;
	  if (editStep == DIAGONAL_INSERT
                  && ((stateDecoder/INSERT_CODE)%2) == 1) 
	    nextEditStep = DIAGONAL_INSERT;
	  if (editStep == DIAGONAL_DELETE && (stateDecoder/DELETE_CODE)== 1) 
	    nextEditStep = DIAGONAL_DELETE;
	  if (nextEditStep == DIAGONAL_INSERT) { 
	    diagIndex--;
	    index1++;
	  }
	  else
	    if (nextEditStep == DIAGONAL_DELETE) 
	      diagIndex++;
	  editInstructions[charCounter] = editStep = nextEditStep;
	}
	for (charIndex = charCounter-1; charIndex >= 0; charIndex--) 
	  switch(editInstructions[charIndex]) {
	  case 0: 
	    REP_;
	    break;
	  case DIAGONAL_INSERT:
	    INS_(1);
	    break;
	  case DIAGONAL_DELETE:
	    DEL_(1);
	    break;
	  }
	MemFree(editInstructions); 
	MemFree(state[0]); 
	MemFree(state);
	return(score);
}


/*Do a banded, gapped alignment of seq1 and seq2 starting at position start1 
  of seq1 and position start2 of seq2; the band boundaries are lowBand
  and highBand, matrix is the score matrix; gapOpen and gapExtend are
  the gap costs; alignScript is used to describe the alignment;
  alignScript is NULL if only the alignment score is desired
  the alignment score is returned*/
static Int4 ALIGN_B(Uint1 *seq1, Uint1 *seq2,Int4 start1, Int4 start2, 
 Int4 lowDiag, Int4 highDiag,Int4 **matrix, Int4 gapOpen,
 Int4 gapExtend, Int4 * alignScript, data_dp * data)
{ 
	Int4 score; /*score to return*/
        Int4 i; /*index over sequences*/
	Int4 band; /*width of band*/
        alignSearchItems *alignSearch; /*holds alignment parameters*/
        dp_ptr CD; /*array for dynamic program information*/

        alignSearch = (alignSearchItems *) ckalloc(sizeof(alignSearchItems));
        /* Setup alignment parameters */
	alignSearch->matrix = matrix;
	alignSearch->gapOpen = gapOpen;
	alignSearch->gapExtend = gapExtend;
	alignSearch->gapCost = gapOpen+gapExtend;
	lowDiag = MIN(MAX(-start1, lowDiag),MIN(start2-start1,0));
	highDiag = MAX(MIN(start2, highDiag),MAX(start2-start1,0));

	if (start2 <= 0) { 
	  if (start1 > 0) 
	    if (alignScript)  
	      DEL_(start1);
	  return -gap(start1);
	}
	if (start1 <= 0) {
	  if (alignScript) 
	    INS_(start2);
	  return -gap(start2);
	}
	if ((band = highDiag-lowDiag+1) <= 1) {
	  score = 0;
	  for (i = 1; i <= start1; i++) {
	    if (alignScript)
	      REP_;
	    score += alignSearch->matrix[seq1[i]][seq2[i]];
	  }
          MemFree(alignSearch);
	  return score;
	}

	CD = (dp_ptr) ckalloc(sizeof(dp_node)*(band+2));
	if (alignScript) {
	  score = align(seq1,seq2,start1,start2,lowDiag,highDiag,0, CD, alignSearch, data);
	} 
	else {
	  score = align(seq1,seq2,start1,start2,lowDiag,highDiag, 1, CD, alignSearch, data);
	}
	MemFree(CD);
        MemFree(alignSearch);
	return score;
}

/*seq is a piece of a sequence, len is the sength of seq
 pass back start and end position where first pattern match occurs*/
static void get_pat(Uint1 *seq, Int4 len, Int4 *start, Int4 *end, 
	       patternSearchItems *patternSearch)
{
    Int4 mask;   /*mask of input pattern positions after which
                  a match can be declared*/
    Int4  maskShiftPlus1; /*mask shifted left plus 1*/
    Int4 prefixMatchedBitPattern = 0; /*indicates where pattern aligns
                 with seq; e.g., if value is 9 = 0101 then 
                 last 3 chars of seq match first 3 positions in pattern
                 and last 1 char of seq matches 1 position of pattern*/
    Int4 i; /*index over seq */
    Int4 rightOne;  /*loop index looking for 1 in both mask and
                     prefixMatchedBitPattern*/
    Int4 rightMaskOnly; /*rightmost bit that is 1 in the mask only*/

    mask = patternSearch->match_mask; 
    maskShiftPlus1 = (mask << 1) +1;
    for (i = 0, prefixMatchedBitPattern= 0; i < len; i++) {
      prefixMatchedBitPattern =  
	((prefixMatchedBitPattern << 1) | maskShiftPlus1) & 
	patternSearch->whichPositionPtr[seq[i]];
    }
    /*do the work of lenof here*/
    rightMaskOnly = -1; 
    rightOne = prefixMatchedBitPattern & mask; 
    for (i = 0; i < BITS_PACKED_PER_WORD; i++) {
      if ((rightOne >> i) % 2  == 1) 
	break;
      if ((mask >> i) %2  == 1) 
	rightMaskOnly = i;
    }
    *start = rightMaskOnly + 1;
    *end = i;
}

/*seq is a piece of a sequence, len is the sength of seq
 pass back start and end position where first pattern match occurs
 Similar to get_pat, but for patterns longer than a word*/
static void get_patL(Uint1 *seq, Int4 len, Int4 *start, Int4 *end,
                 patternSearchItems *patternSearch)
{
    Int4 *mask;  /*mask of input pattern positions after which
                  a match can be declared*/
    Int4 *prefixMatchedBitPattern; /*indicates where pattern aligns with seq*/
    Int4 wordIndex; /*index over words in pattern*/
    Int4  i;  /*index over seq*/
    Int4 rightMaskOnly; /*rightmost bit that is 1 in the mask only*/
    Int4 j; /*index over bits in a word*/
    Boolean found = FALSE;  /*found match position yet*/

    mask = (Int4 *) ckalloc(sizeof(Int4)*patternSearch->numWords);
    prefixMatchedBitPattern = (Int4 *) 
      ckalloc(sizeof(Int4)*patternSearch->numWords);
    for (wordIndex = 0; wordIndex < patternSearch->numWords; wordIndex++) {
      mask[wordIndex] = patternSearch->match_maskL[wordIndex];
      prefixMatchedBitPattern[wordIndex] = 0;
    }
    lmove(mask, 1, patternSearch);
    for (i = 0; i < len; i++) {
      lmove(prefixMatchedBitPattern, 0, patternSearch);
      or(prefixMatchedBitPattern, mask, patternSearch); 
      and(prefixMatchedBitPattern, prefixMatchedBitPattern, 
	  patternSearch->bitPatternByLetter[seq[i]], patternSearch);
    }
    and(prefixMatchedBitPattern, prefixMatchedBitPattern, 
	patternSearch->match_maskL, patternSearch);
    rightMaskOnly = -1;
    for (wordIndex = 0; (wordIndex < patternSearch->numWords) && (!found); 
	 wordIndex++) {
      for (j = 0; j < BITS_PACKED_PER_WORD && (!found); j++) {
	if ((prefixMatchedBitPattern[wordIndex]>>j) % 2 == 1)
	  found = TRUE;
	else
	  if ((patternSearch->match_maskL[wordIndex] >> j)%2 == 1) 
	    rightMaskOnly = wordIndex*BITS_PACKED_PER_WORD+j;
      }
    }
    if (found) {
      wordIndex--;
      j --;
    }
    *start = rightMaskOnly+1; 
    *end = wordIndex*BITS_PACKED_PER_WORD+j;
}

/*Find pattern occurrences in seq when the pattern description is
  extra long, report the results back in hitArray
  seq is input sequence, len is length of seq 
  hitArray stores pairs of length/position for matches */
static void get_patLL(Uint1 *seq, Int4 len, Int4 *hitArray, 
		      patternSearchItems *patternSearch)
{
    Int4 i, j; /*indices on one word hits*/
    Int4  wordIndex, wordIndex2; /*indices on words in pattern representation*/
    Int4  twiceHitsOneWord; /*Twice the number of hits against one
                              word of the pattern*/
    Int4  hitIndex; /*index over hits against one word*/
    Int4 pos; /*keeps track of how many intermediate hits*/
    Int4 placeIndex; /*index over the number of places in the pattern rep*/
    Int4 hitArray1[MAX_HIT];
    Int4 oneWordHitArray[MAX_WORDS_IN_PATTERN]; /*hits for one word of 
                              pattern representation*/

    i = 1; 
    hitArray[0] = patternSearch->numPlacesInWord[0]-1;
    for (wordIndex = 1; wordIndex < patternSearch->numWords; wordIndex++) {
      patternSearch->whichPositionPtr = patternSearch->SLL[wordIndex]; 
      patternSearch->match_mask = patternSearch->match_maskL[wordIndex];
      pos = 0;
      for (j = 0; j < i; j += wordIndex) {
	twiceHitsOneWord = find_hitsS(oneWordHitArray, 
		       (Uint1Ptr) &seq[hitArray[j]+1], 
       MIN(len-hitArray[j]-1, 
 patternSearch->spacing[wordIndex-1]+patternSearch->numPlacesInWord[wordIndex]),
				      patternSearch);
	for (hitIndex = 0; hitIndex < twiceHitsOneWord; 
	     hitIndex+= 2, pos+= wordIndex+1) {
	  hitArray1[pos] = hitArray[j]+oneWordHitArray[hitIndex]+1;
	  for (wordIndex2 = 1; wordIndex2 < wordIndex; wordIndex2++)
	    hitArray1[pos+wordIndex2] = hitArray[j+wordIndex2];
	  hitArray1[pos+wordIndex] = oneWordHitArray[hitIndex+1];
	}
      }
      i = pos;
      for (j = 0; j < pos; j++) 
	hitArray[j] = hitArray1[j];
    }
    for (j = 0; j < pos; j+= wordIndex) {
      for (placeIndex = patternSearch->numPlacesInWord[0], i=j+1; 
	   i < j+wordIndex; i++) 
	placeIndex += hitArray[i]+patternSearch->numPlacesInWord[i-j];
      if (placeIndex != len) 
	continue;
      if (j == 0) 
	return;
      for (i = 0; i < wordIndex; i++) 
	hitArray[i] = hitArray[i+j];
      return;
    }
    ErrPostEx(SEV_FATAL, 0, 0, "getLL wrong\n");
    exit(1);
}

/*align querySeq to dbSeq when guaranteed that the pattern occurs
  in each 
  lenQuerySeq and lenDbSeq are their lengths
  alignScript is alignment script; NULL if only score is desired
  tback will hold adjusted alignment script for return
  gap_align keeps track of parameters for a gapped alignment
  multiple is related to significance of the pattern match
  the overal match score is returned; a variant of the
  score is passed back in useful_score*/

Int4 LIBCALL align_of_pattern(Uint1 *querySeq, Uint1 *dbSeq, Int4 lenQuerySeq, 
		 Int4 lenDbSeq, Int4 *alignScript,  Int4 **tback, 
		 GapAlignBlkPtr gap_align, Int4 *useful_score, 
		 Nlm_FloatHi *multiple, patternSearchItems *patternSearch,
                 seedSearchItems * seedSearch)
{
    data_dp *data; /*stores dynaic program info*/
    Int4  startQueryMatch, endQueryMatch; /*positions delimiting
                             where query matches pattern first */
    Int4 startDbMatch, endDbMatch; /*positions delimiting where
                                     database sequence matches pattern first*/
    Int4  local_score, local_useful_score; /*two versions of score for return*/
    Int4 queryMatchOffset, dbMatchOffset; /*offset from sequence start where
                                            pattern character matches,
                                            used as indices*/
    Int4 patternPosQuery, patternPosDb; /*positions in pattern
                            for matches to query and database sequences*/
    Int4 wordIndex; /*index over words*/

    Int4 placeIndex1, placeIndex2; /*indices over places in pattern*/
    Int4  hitArray1[MAX_HIT], hitArray2[MAX_HIT];
    Int4 numPlacesInWord[MAX_WORDS_IN_PATTERN];
    Int4 **matrix;  /*score matrix*/
    Int4 gap_open; /*gap opening penalty*/
    Int4 gap_extend; /*gap extension penalty*/
    Nlm_FloatHi mul = 1.0; /*local copy of what to pass back in multiple*/

    if (alignScript)
      data = (data_dp *) ckalloc(1 * sizeof(data_dp));
    gap_open = gap_align->gap_open;
    gap_extend = gap_align->gap_extend;
    matrix = gap_align->matrix;
    if (patternSearch->flagPatternLength == ONE_WORD_PATTERN) {
      get_pat(querySeq, lenQuerySeq, &startQueryMatch, &endQueryMatch,
	      patternSearch);
      get_pat(dbSeq, lenDbSeq, &startDbMatch, &endDbMatch, patternSearch);
    } 
    else 
      if (patternSearch->flagPatternLength == MULTI_WORD_PATTERN) {
	get_patL(querySeq, lenQuerySeq, &startQueryMatch, &endQueryMatch,
                    patternSearch);
	get_patL(dbSeq, lenDbSeq, &startDbMatch, &endDbMatch,
                    patternSearch);
      } 
      else {
	if (alignScript) {
	    data->sapp = alignScript;
	    data->last = 0;
	}
	get_patLL(querySeq, lenQuerySeq, hitArray1, patternSearch);
	get_patLL(dbSeq, lenDbSeq, hitArray2, patternSearch);
	mul = 1.0; 
	local_useful_score = 0; 
	local_score = 0;
	for (wordIndex = 0; wordIndex < patternSearch->numWords; wordIndex++) 
	  numPlacesInWord[wordIndex] = patternSearch->numPlacesInWord[wordIndex];
	for (placeIndex1 = 0, wordIndex = 0; 
	     placeIndex1 < patternSearch->highestPlace; placeIndex1++) {
	  if (patternSearch->inputPatternMasked[placeIndex1] < 0) {
	    for (placeIndex2 = placeIndex1-1; placeIndex2 >=0; placeIndex2--) 
	      if (patternSearch->inputPatternMasked[placeIndex2] != allone) 
		break;
	    numPlacesInWord[wordIndex++]-= placeIndex1-placeIndex2-1;
	    hitArray1[wordIndex] += placeIndex1-placeIndex2-1;
	    hitArray2[wordIndex] += placeIndex1-placeIndex2-1;
	  }
	}
	queryMatchOffset = dbMatchOffset = 0;
	for (wordIndex = 0; wordIndex < patternSearch->numWords; wordIndex++) {
	  for (placeIndex1 = 0; placeIndex1 < numPlacesInWord[wordIndex]; placeIndex1++) {
	    if (alignScript) 
	      REP_
		else {		    
		  if (patternSearch->inputPatternMasked[wordIndex] > allone) { 
		    local_score +=matrix[*querySeq][*dbSeq];
		    local_useful_score += matrix[*querySeq][*dbSeq]; 
		    mul *= seedSearch->charMultiple[*querySeq];
		    } 
		  else 
		    local_score += matrix[*querySeq][*dbSeq];
		}
		querySeq++;
		dbSeq++;
	    }
	    if (wordIndex < patternSearch->numWords-1) { 
	      local_score += ALIGN_B(querySeq-1, dbSeq-1, hitArray1[wordIndex+1], hitArray2[wordIndex+1],
			   BAND_LOW, BAND_HIGH, matrix, gap_open, gap_extend, alignScript, data);
	      querySeq += hitArray1[wordIndex+1];
	      dbSeq += hitArray2[wordIndex+1];
	    }
	}
	if (!alignScript) {
	  *useful_score = local_useful_score; 
	  *multiple = mul;
	}
	else 
	  *tback = data->sapp;
	return local_score;     
      }

    local_score = local_useful_score = 0;
    if (alignScript) {
      data->sapp = alignScript;
      data->last = 0;
    }
    for (patternPosQuery = startQueryMatch, patternPosDb = startDbMatch; patternPosQuery <= endQueryMatch || patternPosDb <= endDbMatch; ) {
      if (patternSearch->inputPatternMasked[patternPosQuery] != allone && patternSearch->inputPatternMasked[patternPosDb] != allone) {
	if (alignScript) 
	  REP_
	else {
	  local_score += matrix[*querySeq][*dbSeq];
	  if (patternSearch->inputPatternMasked[patternPosQuery] > allone) { 
	    local_useful_score+= matrix[*querySeq][*dbSeq]; 
	    mul *= seedSearch->charMultiple[*querySeq];
	  }
	}
	patternPosQuery++; 
	patternPosDb++; 
	querySeq++; 
	dbSeq++;
	} 
      else {
	for (queryMatchOffset =0; patternSearch->inputPatternMasked[patternPosQuery] == allone 
  && patternPosQuery <= endQueryMatch; patternPosQuery++, queryMatchOffset++) ;
	for (dbMatchOffset = 0; patternSearch->inputPatternMasked[patternPosDb] == allone && patternPosDb <= endDbMatch; patternPosDb++, dbMatchOffset++) ;
	if (queryMatchOffset == dbMatchOffset) {
	  do {
	    if (alignScript) 
	      REP_
	    else
	      local_score+= matrix[*querySeq][*dbSeq];
	    querySeq++;
	    dbSeq++; 
	    queryMatchOffset--;
	  } while (queryMatchOffset > 0);
	}
	else {
	  local_score += ALIGN_B(querySeq-1, dbSeq-1, queryMatchOffset, dbMatchOffset, 
		       BAND_LOW, BAND_HIGH, matrix, gap_open, gap_extend, alignScript, data); 
	  querySeq+=queryMatchOffset; 
	  dbSeq+=dbMatchOffset;
	}
      }
    }
    if (!alignScript) {
      *multiple = mul; 
      *useful_score = local_useful_score;
    } 
    else 
      *tback = data->sapp;
    if (alignScript)
      MemFree(data);
    return local_score;
}


/*print output for sequence seq starting at offset begin and
ending at offset end
called once for each match*/
void LIBCALL pat_output(Uint1 *seq, Int4 begin, Int4 end, patternSearchItems *patternSearch, ValNodePtr PNTR info_vnp)
{
    Int4 startSeqMatch, endSeqMatch; /*positions in seq where
                                       pattern match starts and ends*/
    Int4 position; /*position of match*/
    Int4 nextMatchStart; /*increment for start of next match*/
    Int4 wordIndex; /*index over words in pattern*/
    Int4 i; /*index over seq*/
    Int4 placeIndex1, placeIndex2; /*indices over places in pattern*/
    Int4 spacingArray[MAX_WORDS_IN_PATTERN]; /*number of characters of
              sequence used across each variable-length region in pattern*/
    Int4 numPlacesInWord[MAX_WORDS_IN_PATTERN]; /*number of places in each word
                                                of the pattern*/
    Char buffer[512];

    ValNodeCopyStr(info_vnp, 0, "HI "); /*Fixed printing command here*/
    
    if (patternSearch->flagPatternLength == ONE_WORD_PATTERN) {
      get_pat(seq+begin, end-begin+1, &startSeqMatch, &endSeqMatch, patternSearch);
    }
    else 
      if (patternSearch->flagPatternLength == MULTI_WORD_PATTERN) {
	get_patL(seq+begin, end-begin+1, &startSeqMatch, &endSeqMatch, patternSearch);
      } 
      else {
	get_patLL(seq+begin, end-begin+1, spacingArray, patternSearch);
	for (wordIndex = 0; wordIndex < patternSearch->numWords; wordIndex++) 
	  numPlacesInWord[wordIndex] = patternSearch->numPlacesInWord[wordIndex];
	for (placeIndex1 = 0, wordIndex = 0; placeIndex1 < patternSearch->highestPlace; placeIndex1++) {
	  if (patternSearch->inputPatternMasked[placeIndex1] < 0) {
	    for (placeIndex2 = placeIndex1-1; placeIndex2 >=0; placeIndex2--) 
	      if (patternSearch->inputPatternMasked[placeIndex2] != allone) 
		break;
	    numPlacesInWord[wordIndex++]-= placeIndex1-placeIndex2-1;
	    spacingArray[wordIndex] += placeIndex1-placeIndex2-1;
	  }
	}
	position = begin;
	for (wordIndex = 0; wordIndex < patternSearch->numWords; wordIndex++) {
	  sprintf(buffer, "(%ld %ld)", (long) position, (long) position+numPlacesInWord[wordIndex]-1);
          ValNodeCopyStr(info_vnp, 0, buffer);
	  position += numPlacesInWord[wordIndex]+spacingArray[wordIndex+1];
	} 
        ValNodeCopyStr(info_vnp, 0, "\n");
	return;
      }
    nextMatchStart  = 0;
    for (i = startSeqMatch; i <= endSeqMatch; ) {
      if (patternSearch->inputPatternMasked[i] != allone) {
	i++;
      } else {
          sprintf(buffer, "(%ld %ld) ", (long) (begin+nextMatchStart), (long) (begin+i-1));
          ValNodeCopyStr(info_vnp, 0, buffer);
          
          for (; patternSearch->inputPatternMasked[i] == allone && i <= endSeqMatch; i++) ;
          nextMatchStart = i;
      }
    }
    if (nextMatchStart != i) {  /*last match*/
        sprintf(buffer, "(%ld %ld)\n", (long) (begin+nextMatchStart), (long) (begin+i-1));
        ValNodeCopyStr(info_vnp, 0, buffer);
    } else { 
        ValNodeCopyStr(info_vnp, 0, "\n");
    }
}

/*find the places where the pattern matches seq;
  len is the length of seq
  hitArray stores the results as pairs of positions in consecutive
  entries
  is_dna indicates whether seq is made of DNA or protein letters
  twice the number of hits (length of hitArray filled in is returned
  3 different methods are used depending on the length of the pattern*/
Int4 LIBCALL find_hits(Int4 *hitArray, Uint1Ptr seq, Int4 len, Boolean is_dna, 
	       patternSearchItems * patternSearch)
{
    if (patternSearch->flagPatternLength == ONE_WORD_PATTERN) 
      return find_hitsS_head(hitArray, seq, 0, len, is_dna, patternSearch);
    if (patternSearch->flagPatternLength == MULTI_WORD_PATTERN) 
      return find_hitsL(hitArray, seq, len, patternSearch);
    return find_hitsLL(hitArray, seq, len, is_dna, patternSearch);
}

#ifdef __cplusplus
}
#endif
