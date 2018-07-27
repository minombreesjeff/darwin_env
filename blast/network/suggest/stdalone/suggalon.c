/*
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
* File Name: suggalon.c
*
* Author:  Yuri Sadykov
*
* Version Creation Date:   08/14/95
*
* $Revision: 6.0 $
*
* File Description: 
*	Implementation of Suggest application as a standalone version. Here it
*	is only part, mainly related with reading data for processing and
*	outputting of result of processing. The implementaion of the processing
*	of data is in suggest.c and adjust.c
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* ==========================================================================
*
*
* RCS Modification History:
*/

#include <ncbi.h>
#include "suggest.h"

/*****************************************************************************
 * Extern globals
 */
extern SuggestOutput	suggestOut;
extern SuggestRec	suggestRec;
extern IntPtr		intlist;

extern Char	*dna_seq;
extern Int4	dna_len;
extern Char	*rev_seq;

/*****************************************************************************
 * External function prototipes
 */

/*****************************************************************************
 * Defines
 */
#define TOTARG 9
#define NUMARG 9

/*****************************************************************************
 * Global variables
 */
static Args myargs [TOTARG] = {
  { "Sequence File", "stdin", NULL, NULL, FALSE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
  { "Output File", "stdout", NULL, NULL, FALSE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},
  { "Genetic Code", "0", NULL, NULL, FALSE, 'g', ARG_INT, 0.0, 0, NULL},
  { "Minimum Value", "-1", NULL, NULL, FALSE, 'l', ARG_INT, 0.0, 0, NULL},
  { "Maximum Value", "0", NULL, NULL, FALSE, 'u', ARG_INT, 0.0, 0, NULL},
  { "Min Exon Override - default set by program", "0", NULL, NULL,
        FALSE, 'x', ARG_INT, 0.0, 0, NULL},
  { "Terminal Stop", "T", NULL, NULL, FALSE, 't', ARG_BOOLEAN, 0.0, 0, NULL},
  { "Sequin Format", "F", NULL, NULL, FALSE, 's', ARG_BOOLEAN, 0.0, 0, NULL},
  { "Reverse", "F", NULL, NULL, TRUE, 'r', ARG_BOOLEAN, 0.0, 0, NULL}
};

static Char inputFile [PATH_MAX];
static Char outputFile [PATH_MAX];

static Char  empty [3];

/*****************************************************************************
 * Function prototipes
 */


/*****************************************************************************
*
*   ReadNextChar (f)
*       Reads the next character in a file, returning '\0' if end of file
*
*****************************************************************************/

static Char ReadNextChar (FILE *f)

{
  Char  ch;
  int   getcrsult;

  ch = '\0';
  if (f != NULL) {
    getcrsult = fgetc (f);
    ch = (Char) getcrsult;
    if (getcrsult == EOF && feof (f)) {
      ch = '\0';
    }
  }
  return ch;
}

/*****************************************************************************
*
*   ReadNextLine (f, str, maxsize)
*       Reads the next line in a file, returning FALSE if end of file
*
*****************************************************************************/

static Boolean ReadNextLine (FILE *f, CharPtr str, size_t maxsize)

{
  Boolean  done;
  Char     ch;
  Int2     count;

  done = FALSE;
  if (f != NULL && str != NULL && maxsize > 0) {
    str [0] = '\0';
    count = 0;
    ch = ReadNextChar (f);
    while (ch != '\n' && ch != '\r' && ch != '\0') {
      if (count < (Int2) maxsize && ch != '\0') {
        str [count] = ch;
        count++;
      }
      ch = ReadNextChar (f);
    }
    if (count <= (Int2) maxsize) {
      str [count] = '\0';
    }
    done = (Boolean) (! feof (f));
  }
  return done;
}

/*****************************************************************************
*
*   Get_Sequence (f, seqrp)
*       Gets a sequence from a file, recording the sequence and segment
*       lengths in ByteStores, and returning the final character string and
*       total length
*
*****************************************************************************/

static void Get_Sequence (FILE *f, SeqRecPtr seqrp)

{
  Char      ch;
  CharPtr   chptr;
  Boolean   goOn;
  IdRecPtr  last;
  Int4      len;
  Char      str [256];
  IdRecPtr  this;
  Int4      total;

  if (f != NULL && seqrp != NULL) {
    if (seqrp->rawSeq == NULL) {
      seqrp->rawSeq = BSNew (1000);
    }
    if (seqrp->segLens == NULL) {
      seqrp->segLens = BSNew (10);
    }
    len = 0;
    total = 0;
    goOn = TRUE;
    while (goOn && ReadNextLine (f, str, sizeof (str))) {
      if (str [0] == '&') {
        goOn = FALSE;
      } else if (str [0] == '!') {
        goOn = FALSE;
      } else if (str [0] == '>') {
        if (len > 0) {
          if (seqrp->lookForStop) {
            BSPutByte (seqrp->rawSeq, (Int2) '*');
            len++;
          }
          BSWrite (seqrp->segLens, &len, sizeof (Int4));
          total += len;
          len = 0;
        }
        chptr = StringChr (str, ' ');
        if (chptr != NULL) {
          *chptr = '\0';
        }
        if (seqrp->ids != NULL) {
          last = seqrp->ids;
          while (last->next != NULL) {
            last = last->next;
          }
          this = MemNew (sizeof (IdRec));
          if (this != NULL) {
            this->id.accn = StringSave (str + 1);
          }
          last->next = this;
        } else {
          last = MemNew (sizeof (IdRec));
          if (last != NULL) {
            last->id.accn = StringSave (str + 1);
          }
          seqrp->ids = last;
        }
      } else if (str [0] != '\0') {
        chptr = str;
        while (*chptr != '\0') {
          ch = TO_UPPER (*chptr);
          if (ch >= 'A' && ch <= 'Z') {
            if (seqrp->nuc) {
              if (ch == 'U') {
                ch = 'T';
              }
              if (StringChr ("BDEFHIJKLMOPQRSUVWXYZ", ch) == NULL) {
                BSPutByte (seqrp->rawSeq, (Int2) ch);
                len++;
              }
            } else {
              if (StringChr ("JOU", ch) == NULL) {
                BSPutByte (seqrp->rawSeq, (Int2) ch);
                len++;
              }
            }
          }
          chptr++;
        }
      }
    }
    if (seqrp->nuc) {
      BSPutByte (seqrp->rawSeq, (Int2) 'N');
      BSPutByte (seqrp->rawSeq, (Int2) 'N');
      BSPutByte (seqrp->rawSeq, (Int2) 'N');
      len += 3;
    }
    if (len > 0) {
      if (seqrp->lookForStop) {
        BSPutByte (seqrp->rawSeq, (Int2) '*');
        len++;
      }
      BSWrite (seqrp->segLens, &len, sizeof (Int4));
      total += len;
      len = 0;
    }
    if (total > 0) {
      seqrp->sequence = BSMerge (seqrp->rawSeq, NULL);
      seqrp->length = total;
    } else {
      seqrp->sequence = NULL;
      seqrp->length = 0;
    }
  }
}


/*****************************************************************************
*
*   Get_DNA (f, sugrp)
*       Gets a nucleotide sequence from a file
*
*****************************************************************************/

static void Get_DNA (FILE *f, SuggestRecPtr sugrp)

{
  if (f != NULL && sugrp != NULL) {
    sugrp->nucleotide.rawSeq = NULL;
    sugrp->nucleotide.segLens = NULL;
    sugrp->nucleotide.ids = NULL;
    sugrp->nucleotide.sequence = NULL;
    sugrp->nucleotide.length = 0L;
    sugrp->nucleotide.nuc = TRUE;
    sugrp->nucleotide.lookForStop = FALSE;
    Get_Sequence (f, &(sugrp->nucleotide));
  }
}

/*****************************************************************************
*
*   Get_Protein (f, sugrp)
*       Gets a protein sequence from a file
*
*****************************************************************************/

static void Get_Protein (FILE *f, SuggestRecPtr sugrp)

{
  if (f != NULL && sugrp != NULL) {
    sugrp->protein.rawSeq = NULL;
    sugrp->protein.segLens = NULL;
    sugrp->protein.ids = NULL;
    sugrp->protein.sequence = NULL;
    sugrp->protein.length = 0L;
    sugrp->protein.nuc = FALSE;
    sugrp->protein.lookForStop = sugrp->lookForStop;
    Get_Sequence (f, &(sugrp->protein));
  }
}

/*****************************************************************************
*
*   Get_Params (void)
*       Get command line arguments
*
*****************************************************************************/


static Boolean Get_Params (void)

{
  FILE  *f;
  FILE  *g;
  Int4  i;
  Int2  j;
  Int4  len;
  Int4  num_nuc;
  Int4  num_prt;

  if (GetArgs ("suggest", NUMARG, myargs)) {
    if (NUMARG == TOTARG && myargs [8].intvalue) {
      suggestRec.lookForStop = (Boolean) (myargs [6].intvalue);
      StringNCpy (inputFile, myargs [0].strvalue, sizeof (inputFile));
      if (inputFile [0] == '\0') {
        StringCpy (inputFile, "stdin");
      }
      StringNCpy (outputFile, myargs [1].strvalue, sizeof (outputFile));
      if (outputFile [0] == '\0') {
        StringCpy (outputFile, "stdout");
      }
      f = FileOpen (inputFile, "r");
      if (f != NULL) {
        Get_DNA (f, &suggestRec);
        FileClose (f);
        dna_seq = suggestRec.nucleotide.sequence;
        dna_len = StringLen (dna_seq);
        if (dna_len > 3 && dna_seq != NULL) {
          dna_len -= 3;
          dna_seq [dna_len] = '\0';
        }
        rev_seq = MemNew(dna_len+1);
        if (rev_seq != NULL) {
          reverse (dna_len);
          f = FileOpen (outputFile, "w");
          if (f != NULL) {
            j = 0;
            for (i = 0; i < dna_len; i++) {
              fputc ((int) (rev_seq [i]), f);
              j++;
              if (j >= 50) {
                fputc ((int) '\n', f);
                j = 0;
              }
            }
            if (j > 0) {
              fputc ((int) '\n', f);
            }
            FileClose (f);
          }
        }
      }
    } else {
      suggestRec.lookForStop = (Boolean) (myargs [6].intvalue);
      StringNCpy (inputFile, myargs [0].strvalue, sizeof (inputFile));
      if (inputFile [0] == '\0') {
        StringCpy (inputFile, "stdin");
      }
      StringNCpy (outputFile, myargs [1].strvalue, sizeof (outputFile));
      if (outputFile [0] == '\0') {
        StringCpy (outputFile, "stdout");
      }
      f = FileOpen (inputFile, "r");
      if (f != NULL) {
        g = FileOpen (outputFile, "w");
        if (g != NULL) {
	  suggestOut.out.fileOut = g;
          while (! feof (f)) {
            if ((Boolean) (myargs [7].intvalue)) {
              Get_DNA (f, &suggestRec);
              Get_Protein (f, &suggestRec);
            } else {
              Get_Protein (f, &suggestRec);
              Get_DNA (f, &suggestRec);
            }
            Get_Genetic_Code (myargs [2].intvalue, &suggestRec);
            suggestRec.minVal = (myargs [3].intvalue);
            suggestRec.maxVal = (myargs [4].intvalue);
            suggestRec.minExon = (myargs [5].intvalue);

	    ProcessData(&suggestOut);
          }
          FileClose (g);
        }
        FileClose (f);
      }
    }
    return TRUE;
  } else {
    return FALSE;
  }
}

/*****************************************************************************
*
*   Get_Prot_ID (sugrp, num)
*       Gets a protein id
*
*****************************************************************************/

static CharPtr Get_Prot_ID (Int2 num)

{
  CharPtr   str;
  IdRecPtr  this;

  empty [0] = 'x';
  empty [1] = '\0';
  str = empty;
    this = suggestRec.protein.ids;
    while (num > 0 && this != NULL) {
      num--;
      this = this->next;
    }
    if (this != NULL) {
      str = this->id.accn;
    }
  return str;
}

/*****************************************************************************
*
*   Get_Nuc_ID (sugrp, num)
*       Gets a nucleotide id
*
*****************************************************************************/

static CharPtr Get_Nuc_ID (Int2 num)

{
  CharPtr   str;
  IdRecPtr  this;

  empty [0] = 'x';
  empty [1] = '\0';
  str = empty;
    this = suggestRec.nucleotide.ids;
    while (num > 0 && this != NULL) {
      num--;
      this = this->next;
    }
    if (this != NULL) {
      str = this->id.accn;
    }
  return str;
}

void
OutProteinID(SuggestOutputPtr pSuggestOut, Int2 num)
{
    FilePuts(">", pSuggestOut->out.fileOut);
    FilePuts(Get_Prot_ID(num), pSuggestOut->out.fileOut);
    FilePuts("\n\n", pSuggestOut->out.fileOut);
}

void
OutLocation(SuggestOutputPtr pSuggestOut, Int4 num_nuc)
{
    Int4	i = 0;
    Int4	j = 0;
    FILE	*f = pSuggestOut->out.fileOut;
    Char	str[128];
    IntPtr	thisInt = intlist;

    while (thisInt != NULL) {
      if (i == 0) {
	  if (thisInt->next != NULL) {
	      FilePuts ("FT   CDS             join(", f);
	  } else {
	      FilePuts ("FT   CDS             ", f);
          }
      } else if (j == 0) {
          FilePuts ("FT                   ", f);
      }
      if (thisInt->orient == MinusStrand) {
          if (thisInt->next != NULL && num_nuc > 1) {
	      sprintf (str, "complement(%s:%ld..%ld)",
	               (CharPtr)Get_Nuc_ID((Int2)(thisInt->id)),
		       (long)thisInt->from, (long)thisInt->to);
	  } else {
	      sprintf (str, "complement(%ld..%ld)", (long)thisInt->from,
	               (long)thisInt->to);
          }
      } else {
          if (thisInt->next != NULL && num_nuc > 1) {
	      sprintf (str, "%s:%ld..%ld",
	               (CharPtr)Get_Nuc_ID((Int2)(thisInt->id)),
		       (long)thisInt->from, (long)thisInt->to);
          } else {
	      sprintf (str, "%ld..%ld", (long)thisInt->from,
	               (long)thisInt->to);
          }
       }
       FilePuts (str, f);
       j++;
       if (j >= 3) {
           if (thisInt->next != NULL) {
	       FilePuts (",\n", f);
           }
           j = 0;
        } else {
           if (thisInt->next != NULL) {
	       FilePuts (",", f);
           }
        }
        thisInt = thisInt->next;
        i++;
    }
    if (i > 1)
        FilePuts (")\n\n\n", f);
    else
        FilePuts ("\n\n\n", f);
}

/*****************************************************************************
*
*   Main (void)
*       Main program to test Suggest_Intervals
*
*****************************************************************************/
Int2
Main (void)
{
    if (!InitSuggest())
	return 1;

    if (!Get_Params ())
	return 1;
    
    return 0;
}
