static char const rcsid[] = "$Id: copymat.c,v 6.36 2004/04/23 21:11:31 papadopo Exp $";

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
*/

/*****************************************************************************

File name: copymat.c

Authors: Alejandro Schaffer, Sergei Shavirin

Contents: main routines for copymatrices program to convert
score matrices output by makematrices into a single byte-encoded file.
   
$Log: copymat.c,v $
Revision 6.36  2004/04/23 21:11:31  papadopo
force the thick backbone (dumped to the RPS .loo file) to contain the number of cells assumed by RPS blast

Revision 6.35  2004/04/16 14:48:04  papadopo
remove unneeded argument to FillLookupTableOptions

Revision 6.34  2004/04/07 21:48:48  camacho
Add missing header file

Revision 6.33  2004/04/06 12:15:44  camacho
Rename DoubleInt -> SSeqRange

Revision 6.32  2004/03/10 20:21:27  papadopo
add (unused) RPS blast parameters to FillLookupTableOptions

Revision 6.31  2004/03/04 21:16:10  papadopo
add (unused) RPS blast parameter to FillLookupTable call

Revision 6.30  2004/01/30 20:34:45  coulouri
fix minor nit to FileWrite call

Revision 6.29  2004/01/26 19:40:48  coulouri
* Correct buffer overrun
* Use offset rather than pointer in LookupBackboneCell

Revision 6.28  2003/11/24 18:18:47  coulouri
Correction to previous fix for 64-bit irix

Revision 6.27  2003/11/21 18:01:15  ivanov
Added extern definition for impalaMakeFileNames()

Revision 6.26  2003/11/20 15:44:32  camacho
Tom Madden's changes to use lookup table contruction code from algo/blast.

Revision 6.25  2003/05/30 17:31:09  coulouri
add rcsid

Revision 6.24  2003/05/13 16:02:42  coulouri
make ErrPostEx(SEV_FATAL, ...) exit with nonzero status

Revision 6.23  2002/11/06 21:26:47  ucko
RPSConcatSequences: provide useful error messages, ignore all trailing space.

Revision 6.22  2002/04/08 19:02:31  madden
Allow float for threshold

Revision 6.21  2001/06/07 16:45:08  shavirin
Removed bug related to 64bit address structure on SGI platform.

Revision 6.20  2001/04/12 19:50:12  madden
Comment out unrescaling of matrix

Revision 6.19  2000/11/14 23:17:52  shavirin
Removed serious bug under NT platform related to diffence in "w" and "wb"
flag when opening file on PC NT computer. Removed unused header files.

Revision 6.18  2000/11/13 21:25:22  shavirin
Fixed possible bug in the function RPSUpdatePointers (64 bit architecture
specific).

Revision 6.17  2000/11/08 18:34:19  kans
commented out UNIX-specific headers, included by ncbilcl.h for UNIX anyway

Revision 6.16  2000/10/20 21:46:37  shavirin
Added additional parameters for creating RPS database.

Revision 6.15  2000/02/29 16:27:39  shavirin
Added protection against matrix with scaleFactor != 1 for RPS Blast

Revision 6.14  2000/02/28 21:08:34  shavirin
This fixes DEC Alpha problems of RPS Blast.

Revision 6.13  2000/02/28 19:06:47  shavirin
Added comments for RPS Blast functions.
Removed unused code.

Revision 6.12  2000/02/22 19:29:06  shavirin
Fixed DEC Alpha specific bug in the function RPSCreateLookupFile().

Revision 6.11  2000/02/17 19:11:15  shavirin
Removed reference to theCacheSize.

Revision 6.10  2000/01/13 15:27:10  shavirin
Added concatenation of files into single file (for later formatdb).

Revision 6.9  2000/01/12 14:39:46  shavirin
Added parameter to set cache size in lookup table foe RPS Blast.

Revision 6.8  2000/01/07 22:31:47  shavirin
Lookup table header now has notice, that this is single table.

Revision 6.7  1999/12/30 18:34:20  shavirin
Last row in the matrix for every sequence will be gap-row (-INT2_MAX)

Revision 6.6  1999/12/29 18:49:29  shavirin
Changed a little format of RPS lookup tables file.


*****************************************************************************/


#include <ncbi.h>
#include <sequtil.h>
#include <seqport.h>
#include <tofasta.h>
#include <algo/blast/core/blast_encoding.h>

#ifndef MAXLINELEN
#   define MAXLINELEN 2000
#endif
#ifndef MAX_NAME_LENGTH
#   define MAX_NAME_LENGTH 500
#endif
#ifndef PRO_ALPHABET_SIZE
#   define PRO_ALPHABET_SIZE  26
#endif
#ifndef SORT_THRESHOLD
#   define SORT_THRESHOLD 20
#endif
#ifndef RPS_MAGIC_NUMBER
#   define RPS_MAGIC_NUMBER 7702
#endif
#ifndef RPS_ARRAY_SIZE
#   define RPS_ARRAY_SIZE 32768
#endif
/*factor used to multiply the gapped K parameter to make it
  more accurate in most cases*/
#ifndef PRO_K_MULTIPLIER
#   define PRO_K_MULTIPLIER 1.2
#endif
#include <algo/blast/core/blast_lookup.h>
#include <algo/blast/core/blast_options.h>

typedef Int4 ScoreRow[PRO_ALPHABET_SIZE];
extern Boolean LIBCALL 
IMPALAPrintHelp PROTO((Boolean html, Int4 line_length, Char * programName, 
                       FILE *outfp));
extern void  LIBCALL
impalaMakeFileNames PROTO((Char * matrixDbName, Char * auxiliaryFileName,
                           Char * mmapFileName, Char * seqFileName,
                           Char *matrixFileName, Char * ckptFileName,
                           Char *directoryPrefix));

#define NUMARG (sizeof(myargs)/sizeof(myargs[0]))

static Args myargs [] = {
    { "Database for matrix profiles", /* 0 */
      "stdin", NULL, NULL, FALSE, 'P', ARG_FILE_IN, 0.0, 0, NULL},
    { "Print help; overrides all other arguments", /* 1 */
      "F", NULL, NULL, FALSE, 'H', ARG_BOOLEAN, 0.0, 0, NULL},
    { "Create RPS mem map file(s)", /* 2 */
      "T", NULL, NULL, FALSE, 'r', ARG_BOOLEAN, 0.0, 0, NULL},
    { "Threshold for extending hits for RPS database", /* 3 */
      "11", NULL, NULL, FALSE, 'f', ARG_FLOAT, 0.0, 0, NULL},
    { "Word size for RPS database", /* 4 */
      "3", NULL, NULL, FALSE, 'W', ARG_INT, 0.0, 0, NULL},
};

/*counts the number of items in sequencesFile and matricesFile, assumed to
  be one per line, and checks that the numbers are equal.
  returns the number if equal, 0 if unequal, rewinds the file descriptors
  before returning*/
static Int4 countProfiles(FILE *sequencesFile, FILE *matricesFile)
{
    Int4 sequencesCount = 0; /*count for sequencesFile*/
    Int4 matricesCount = 0; /*count for matricesFile*/
    Char oneFileName[MAXLINELEN]; /*for reading one line per file*/
    
    while (fgets(oneFileName,MAXLINELEN,sequencesFile))
        sequencesCount++;
    while (fgets(oneFileName,MAXLINELEN,matricesFile))
        matricesCount++;
    rewind(matricesFile);
    rewind(sequencesFile);
    if (sequencesCount == matricesCount)
        return(sequencesCount);
    else {
        ErrPostEx(SEV_FATAL, 1, 0, "copymatrices: Sequences file has %d entries; Matrices file has %d entries; these should be equal\n", sequencesCount,matricesCount);
        return(0);
    }
}

/*free the memory associated with the position-specific score matrices*/
static void  freeMatrix(ScoreRow *posMatrix)
{

  MemFree(posMatrix);
}

/*allocate memory for the position-specific score matrices
  enough memory is allocated to hold the largest matrix
  the memory is reused for each different matrix*/
static ScoreRow * allocateMatrix(Int4 maxSequenceLength)
{
  ScoreRow *returnMatrix; /*matrix to return*/

  returnMatrix = (ScoreRow *) MemNew(maxSequenceLength * sizeof(ScoreRow));
  return(returnMatrix);
}

/* read in a position-specific score matrix from thisMatrixFile
   the number of positions is dbSequenceLength
   kbp keeps the Karlin-ALtschul parameters
   returnMatrix is the memory address where the matrix is to be stored*/
static void readNextMatrix(FILE * thisMatrixFile,
              Int4 startPos, Int4 *endPos,
              ScoreRow *bigMatrix)
{
  Int4 i, r; /*row indices for sequence and matrix*/
  Int4 lengthInFile; /*length of query*/
  Nlm_FloatHi junkLambda, junkK, junklogK, junkH; /*used to read in useless
						    Karlin blocks*/
  Char *sequence;  /*sequence to read in*/
  Char rowOfScores[MAXLINELEN]; /*one row of scores to be read in*/

  fscanf(thisMatrixFile, "%d", &lengthInFile);
  sequence = (Char *) MemNew((lengthInFile + 2) * sizeof(Char));
  fscanf(thisMatrixFile,"%s",sequence);
  MemFree(sequence);
  /*read in useless Karlin block*/
  fscanf(thisMatrixFile,"%le", &junkLambda);
  fscanf(thisMatrixFile,"%le", &junkK);
  fscanf(thisMatrixFile,"%le", &junklogK);
  fscanf(thisMatrixFile,"%le", &junkH);
  /*read in useless Karlin block*/
  fscanf(thisMatrixFile,"%le", &junkLambda);
  fscanf(thisMatrixFile,"%le", &junkK);
  fscanf(thisMatrixFile,"%le", &junklogK);
  fscanf(thisMatrixFile,"%le", &junkH);
  /*read in useless Karlin block*/
  fscanf(thisMatrixFile,"%le", &junkLambda);
  fscanf(thisMatrixFile,"%le", &junkK);
  fscanf(thisMatrixFile,"%le", &junklogK);
  fscanf(thisMatrixFile,"%le\n", &junkH);
  for(i = 0, r = startPos; i < lengthInFile; i++, r++) {
    fgets(rowOfScores, MAXLINELEN, thisMatrixFile);
    sscanf(rowOfScores, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", 
                        &(bigMatrix[r][0]),
                        &(bigMatrix[r][1]),
                        &(bigMatrix[r][2]),
                        &(bigMatrix[r][3]),
                        &(bigMatrix[r][4]),
                        &(bigMatrix[r][5]),
                        &(bigMatrix[r][6]),
                        &(bigMatrix[r][7]),
                        &(bigMatrix[r][8]),
                        &(bigMatrix[r][9]),
                        &(bigMatrix[r][10]),
                        &(bigMatrix[r][11]),
                        &(bigMatrix[r][12]),
                        &(bigMatrix[r][13]),
                        &(bigMatrix[r][14]),
                        &(bigMatrix[r][15]),
                        &(bigMatrix[r][16]),
                        &(bigMatrix[r][17]),
                        &(bigMatrix[r][18]),
                        &(bigMatrix[r][19]),
                        &(bigMatrix[r][20]),
                        &(bigMatrix[r][21]),
                        &(bigMatrix[r][22]),
                        &(bigMatrix[r][23]),
                        &(bigMatrix[r][24]),
                        &(bigMatrix[r][25]));
  }

  if((Boolean) myargs[2].intvalue) {
      /* Last row in the matrix will be gap-row (-INT2_MAX) */
      
      for(i = 0; i < 26; i++) {
          bigMatrix[r][i] = -INT2_MAX;
      }
      r++;
  }

  *endPos = r;
}

/*read each matrix in turn and store its scores in combinedMatrix*/
static void readAllMatrices(FILE *matrixnamefp, ScoreRow *combinedMatrix, 
                            Int4 numProfiles, CharPtr directoryPrefix, 
                            Int4Ptr seqlens)
{
    Int4 i; /*loop index*/
    Char oneMatrixFileName[MAXLINELEN]; /*name of matrix file to read*/
    FILE *thisMatrixFile; /*descriptor for one matrix file*/
    Int4 startPos; /*starting row in big matrix for next small matrix*/
    Int4 endPos; /*ending row + 1 in big matrix for this small matrix*/
    Int4 prefixLength; /*length of directoryPrefix*/
    Int4 c1,c2; /*loop indices over characters*/
    Char relativeMatrixFileName[MAXLINELEN];
    
    startPos = 0;
    endPos = 0;
    if ('\0' != directoryPrefix[0]) {
        strcpy(oneMatrixFileName, directoryPrefix);
        prefixLength = strlen(directoryPrefix);
    }     
    for (i = 0; i < numProfiles; i++) {
        if ('\0' == directoryPrefix[0])
            fscanf(matrixnamefp,"%s", oneMatrixFileName);
        else {
            fscanf(matrixnamefp,"%s", relativeMatrixFileName); 
            for(c1 = prefixLength, c2 = 0; relativeMatrixFileName[c2] != '\0';
                c1++, c2++)
                oneMatrixFileName[c1] = relativeMatrixFileName[c2];
            oneMatrixFileName[c1] = '\0';
        }
        
        if ((thisMatrixFile = FileOpen(oneMatrixFileName, "r")) == NULL)  {
            ErrPostEx(SEV_FATAL, 1, 0, "profiles: Unable to open matrix file %s\n", oneMatrixFileName);
            return;
        }
        readNextMatrix(thisMatrixFile, startPos, &endPos,
                       combinedMatrix);

        if(seqlens != NULL) {
            seqlens[i] = startPos;
        }

        startPos = endPos;
        FileClose(thisMatrixFile);
    }

    if(seqlens != NULL) {   /* Last entry - is the end of last sequence */
        seqlens[i] = startPos;
    }
    
    return;
}

/*findTotalLength scans matrixAuxiliaryFile to find the
  total  number of positions among all the position-specific matrices*/
static Int4 findTotalLength(FILE *matrixAuxiliaryFile, Int4 numProfiles,
                            Nlm_FloatHiPtr scalingFactor)
{
    Int4 maxLength; /*maximum length of sequence*/
    Int4 thisLength; /*length of next sequence*/
    Int4 totalLength; /*total length to return*/
    Int4 dbLength; /*length of database*/
    Int4 i; /*loop index*/
    Nlm_FloatHi Kungapped, Hungapped; /*two values to read*/
    Char * underlyingMatrixName; /*name of matrix to read*/
    Int4 gap_open, gap_extend; /*gap costs to skip over in reading*/
    
    underlyingMatrixName = MemNew(MAXLINELEN * sizeof(Char));
    fscanf(matrixAuxiliaryFile,"%s",underlyingMatrixName);
    fscanf(matrixAuxiliaryFile,"%d\n", &gap_open);
    fscanf(matrixAuxiliaryFile,"%d\n", &gap_extend);
    fscanf(matrixAuxiliaryFile, "%le", &Kungapped);
    fscanf(matrixAuxiliaryFile, "%le", &Hungapped);
    fscanf(matrixAuxiliaryFile, "%d", &maxLength);
    fscanf(matrixAuxiliaryFile, "%d", &dbLength);
    fscanf(matrixAuxiliaryFile, "%lf", scalingFactor);
    totalLength = 0;
    for (i = 0; i < numProfiles; i++) {
        fscanf(matrixAuxiliaryFile, "%d", &thisLength);
        fscanf(matrixAuxiliaryFile, "%le", &Kungapped);
        totalLength += thisLength;
    }
    rewind(matrixAuxiliaryFile);
    MemFree(underlyingMatrixName);
    return(totalLength);
}

static Boolean RPSUpdateOffsets(LookupTable *lookup)
{
    Uint4 len;
    Int4 index;
    Int4 num_used;
    Int4 offset_diff;

    len = lookup->backbone_size;
    offset_diff = lookup->wordsize - 1;

    /* Walk through table, copying info into mod_lt[] */
    for(index = 0; index < len; index++) {
        
        if((num_used=lookup->thick_backbone[index].num_used) <= 3)
        {
            while (num_used > 0)
            {
                num_used--;
            	lookup->thick_backbone[index].payload.entries[num_used] += offset_diff;
            }
        }
        else
        {
            while (num_used > 0)
            {
                 num_used--;
                 lookup->overflow [ lookup->thick_backbone[index].payload.overflow_cursor + num_used] += offset_diff;
            }
        }
    }
    return TRUE;
}


/* #define RPS_THRESHOLD 11 */
/* #define RPS_WORDSIZE  3 */

/* -- SSH --
   Updates absolute pointers of the lookup table to relative pointers -
   pointers relative to the start of "mod_lookup_table_memory" chunk 
   RPS Blast will calculate real pointers in run time using these values
*/
Boolean RPSUpdatePointers(LookupTable *lookup, Uint4 *new_overflow, Uint4 *new_overflow_size)
{
    Uint4 len;
    Int4 index;
    Uint4 *start_address;
    long mlpp_address;
    Uint4 *new_overflow_cursor;
    Int4 *src;
    Int4 first_hit;

    len = lookup->backbone_size;

    start_address = new_overflow_cursor = new_overflow;

    /* Walk through table, copying info into mod_lt[] */
    for(index = 0; index < len; index++) {
        
        if(lookup->thick_backbone[index].num_used <= 3)
            continue;

        src = &(lookup->overflow[lookup->thick_backbone[index].payload.overflow_cursor]);
        MemCpy(new_overflow_cursor, &src[1], sizeof(Uint4)*(lookup->thick_backbone[index].num_used-1));

        mlpp_address = (long) new_overflow_cursor;

	new_overflow_cursor += lookup->thick_backbone[index].num_used-1;
	first_hit = src[0];

        mlpp_address -= (long) start_address;
        
        /* Now this is new relative address - usually small  */
        lookup->thick_backbone[index].payload.entries[1] = (Int4) mlpp_address;
        lookup->thick_backbone[index].payload.entries[0] = first_hit;

    }

    *new_overflow_size = new_overflow_cursor - new_overflow;

    return TRUE;
}

/* -- SSH --
   Write lookup table to the disk into file "*.loo", which will be
   used memory-mapped during RPS Blast search 
*/
Boolean RPSDumpLookupTable(LookupTable *lookup, FILE *fd)
{
    Uint4 *new_overflow;
    Uint4 new_overflow_size;
    LookupBackboneCell empty_cell;
    Int4 index;

    RPSUpdateOffsets(lookup);

    new_overflow = malloc(lookup->overflow_size*sizeof(Uint4)); 
    RPSUpdatePointers(lookup, new_overflow, &new_overflow_size);

    FileWrite(lookup->thick_backbone, sizeof(LookupBackboneCell), lookup->backbone_size, fd);
    
    /* write empty cells out to the thick backbone size that
       RPS blast expects */

    memset(&empty_cell, 0, sizeof(empty_cell));
    for (index = lookup->backbone_size; index < RPS_ARRAY_SIZE + 1; index++)
        FileWrite(&empty_cell, sizeof(empty_cell), 1, fd);

    if(new_overflow_size)
        FileWrite(new_overflow,
                  sizeof(Uint4),
                  new_overflow_size,
                  fd);

    sfree(new_overflow);
    
    return TRUE;
}
/* -- SSH --
   Create lookup table for the large sequence, that represented
   by all collection of PSSM matrixes and dump this table to disk
   Used by RPS Blast.
*/
Boolean RPSCreateLookupFile(ScoreRow *combinedMatrix, Int4 numProfiles,
                            Int4Ptr seqlens, CharPtr filename, 
                            Nlm_FloatHi scalingFactor)
{
    BlastScoreBlk *sbp;
    SSeqRange *sequence_range;
    FILE *fd;
    Int4  **posMatrix;
    Int4 start, i, header_size, all_length, magicNumber;
    Int4Ptr offsets;
    Int4 num_lookups;
    ListNode *lookup_segment=NULL;
    LookupTable *lookup;
    LookupTableWrap* lookup_wrap_ptr=NULL;
    LookupTableOptions* lookup_options;
   

    if((fd = FileOpen(filename, "wb")) == NULL)
        return FALSE;
    
    num_lookups = 1; /* Single lookup table for all set */

    all_length = seqlens[numProfiles] - seqlens[0];
    
    posMatrix = MemNew((all_length + 1) * sizeof(Int4 *));
    for (i = 0; i < all_length; i++) {
        posMatrix[i] = (Int4 *) &(combinedMatrix[i][0]);
    }
    
    /* Last row is necessary */
    posMatrix[all_length] = MemNew(sizeof(Int4) * PRO_ALPHABET_SIZE);

    for(i = 0; i < PRO_ALPHABET_SIZE; i++) {
        posMatrix[all_length][i] = -INT2_MAX;
    }

    sbp = BlastScoreBlkNew(BLASTAA_SEQ_CODE, 1);
    sbp->posMatrix = posMatrix;
    LookupTableOptionsNew(blast_type_blastp, &lookup_options);
    BLAST_FillLookupTableOptions(lookup_options, blast_type_blastp, FALSE, 
	(Int4) (myargs[3].floatvalue*scalingFactor), myargs[4].intvalue, FALSE, FALSE, TRUE);  /* add last arg for psi-blast?? */


    sequence_range = (SSeqRange*) calloc(1, sizeof(SSeqRange));
    sequence_range->left = 0;
    sequence_range->right = all_length;

    ListNodeAddPointer(&lookup_segment, 0, sequence_range);

    /* Need query for psi-blast??  where to put the PSSM? */
    LookupTableWrapInit(NULL, lookup_options, lookup_segment, sbp, &lookup_wrap_ptr, NULL);
   
    sbp->posMatrix = NULL;
    sbp = BlastScoreBlkFree(sbp);
    lookup_options = LookupTableOptionsFree(lookup_options);
    lookup_segment = ListNodeFreeData(lookup_segment);

    lookup = (LookupTable*) lookup_wrap_ptr->lut;

    /* Only Uint4 maximum length for lookup file allowed in current
       implementation */
    header_size = (numProfiles+1)*sizeof(Int4) + 8*sizeof(Int4);
    
    /* Beginning of file will be allocated for lookup offsets */
    fseek(fd, header_size, SEEK_SET);
    
    offsets = MemNew(sizeof(Int4) * (num_lookups + 1));
    

    offsets[0] = ftell(fd);
    
    start = seqlens[0]; /* 0 */
    
    RPSDumpLookupTable(lookup, fd);
    
    i = 1;
    
    offsets[i] = ftell(fd); /* Last offset also recorded */
    
    fseek(fd, 0, SEEK_SET);
    magicNumber = RPS_MAGIC_NUMBER;
    FileWrite(&magicNumber, sizeof(Int4), 1, fd); /* header[0] */
    FileWrite(&num_lookups, sizeof(Int4), 1, fd); /* header[1] */
    FileWrite(&lookup->neighbor_matches, sizeof(Int4), 1, fd); /* header[2] */
    FileWrite(&lookup->neighbor_matches, sizeof(Int4), 1, fd); /* header[3] */
    FileWrite(&lookup->overflow_size, sizeof(Int4), 1, fd); /* header[4] */
    
    /* Now writing recorded offsets in the beginning of the file */
    
    fseek(fd, 8*sizeof(Int4), SEEK_SET);
    FileWrite(offsets, sizeof(Int4), num_lookups + 1, fd);
    FileClose(fd);
    
    /* Final memory cleenup */
    
    MemFree(posMatrix[all_length]);
    MemFree(posMatrix);

    return TRUE;
}

/* -- SSH --
   Create file <database_name> (without extention), which is concatenation
   of all FASTA files used. Used by RPS Blast.
*/
Boolean RPSConcatSequences(FILE *sfp, CharPtr fastaname)
{
    FILE *fasta_fp, *fd;
    Char oneFileName[MAXLINELEN]; /*for reading one line per file*/
    Char buffer[1024];
    Int4 bytes;
    CharPtr chptr, last_non_space;

    if((fasta_fp = FileOpen(fastaname, "w")) == NULL) {
        ErrPostEx(SEV_FATAL, 1, 0, "concatenate sequences: "
                  "Unable to open target fasta file %s: %s\n",
                  fastaname, strerror(errno));
        return FALSE;
    }

    rewind(sfp);
    
    while (fgets(oneFileName, MAXLINELEN, sfp)) {

        /* Remove trailing whitespace */
        last_non_space = NULL;
        for(chptr = oneFileName; *chptr != NULLB; chptr++) {
            if (!isspace(*chptr))
                last_non_space = chptr;
        }
        if (last_non_space != NULL)
            last_non_space[1] = NULLB;
        
        if((fd = FileOpen(oneFileName, "r")) == NULL) {
            ErrPostEx(SEV_FATAL, 1, 0, "concatenate sequences: "
                      "Unable to open source fasta file %s: %s\n",
                      oneFileName, strerror(errno));
            FileClose(fasta_fp);
            return FALSE;
        }
        
        /* Now concatenating this file into set */
        while((bytes = FileRead(buffer, 1, 1024, fd)) > 0)
            FileWrite(buffer, 1, bytes, fasta_fp);
        FileClose(fd);
    }
    
    FileClose(fasta_fp);
    
    return TRUE;
}

Int2  Main(void)

{
    
    Char *profilesFileName; /*file name for list of profile file names*/
    Char sequencesFileName[MAX_NAME_LENGTH]; /*file anme for list of sequence file names*/
    Char matrixFileName[MAX_NAME_LENGTH]; /*file name for list of matrix file names*/
    Char auxFileName[MAX_NAME_LENGTH]; /*file name for file containing auxiliary information*/
    Char bigFileName[MAX_NAME_LENGTH]; /*file name to store byte-encoded coalesced matrix*/
    Char lookupName[MAX_NAME_LENGTH]; /*file name to store precalculated lookup table */
    FILE *auxiliaryfp; /*file descriptor for matrix auxiliary file*/
    FILE *sequencesfp; /*files descriptor for file containing list of sequences*/
    FILE *matrixnamefp; /*file descriptor for file containing matrix names*/
    FILE *bigmatrixfile; /*file descriptor for file containing single big matrix*/
    Int4 numProfiles; /*number of profiles*/
    Int4 totalProfileLength; /*total length of all profiles*/
    ScoreRow *combinedMatrix; /*combined matrix for all profiles*/
    Char *directoryPrefix; /*directory where profile library is kept, used
                             to reach other directories indirectly*/

    Int4Ptr seqlens;
    Nlm_FloatHi scalingFactor; /*matrix scale to skip over in reading*/

    if (! GetArgs ("copymatrices", NUMARG, myargs)) {
        return (1);
    }
    
    if ((Boolean) myargs[1].intvalue) {
        IMPALAPrintHelp(FALSE, 80, "copymat", stdout);
        return(1);
    }
    profilesFileName = myargs[0].strvalue;
    directoryPrefix = (Char *) MemNew(MAX_NAME_LENGTH *sizeof(char));
    strcpy(directoryPrefix,profilesFileName);
    
    impalaMakeFileNames(profilesFileName, auxFileName, bigFileName,
                        sequencesFileName, matrixFileName, NULL, 
                        directoryPrefix);
    
    if ((matrixnamefp = FileOpen(matrixFileName, "r")) == NULL) {
        ErrPostEx(SEV_FATAL, 1, 0, "copymatrices: Unable to open file with matrix file names %s\n", matrixFileName);
        return (1);
    }
    
    if ((sequencesfp = FileOpen(sequencesFileName, "r")) == NULL) {
        ErrPostEx(SEV_FATAL, 1, 0, "copymatrices: Unable to open file with sequence file names %s\n", sequencesFileName);
        return (1);
    }
    
    if ((auxiliaryfp = FileOpen(auxFileName, "r")) == NULL) {
        ErrPostEx(SEV_FATAL, 1, 0, "profiles: Unable to open auxiliary file %s\n", auxFileName);
        return (1);
    }

    /* -- SSH -- Name of matrix file depends on program - RPS or Impala */
    
    if((Boolean) myargs[2].intvalue) {
        sprintf(bigFileName, "%s.rps", profilesFileName);
    }
    
    if ((bigmatrixfile = FileOpen(bigFileName, "wb")) == NULL) {
        ErrPostEx(SEV_FATAL, 1, 0, "rps-blast: Unable to open big matrix file %s\n", bigFileName);
        return (1);
    }
    
    numProfiles =  countProfiles(sequencesfp, matrixnamefp);
    totalProfileLength = findTotalLength(auxiliaryfp, numProfiles, 
                                         &scalingFactor);
    
    /* -- SSH -- Additional line in matrix with -INT2_MAX values */
    if((Boolean) myargs[2].intvalue) {
        totalProfileLength += numProfiles;
    }

    combinedMatrix = allocateMatrix(totalProfileLength);
    if (NULL == combinedMatrix) {
        ErrPostEx(SEV_FATAL, 1, 0, "copymatrices: Unable to allocate matrix with%d rows\n", totalProfileLength);
        return (1);
        
    }
    /* -- SSH -- RPS Blast data */
    if ((Boolean) myargs[2].intvalue) {
        seqlens = (Int4Ptr) MemNew((numProfiles +1) * sizeof(Int4));
    } else {
        seqlens = NULL;
    }
    
    readAllMatrices(matrixnamefp, combinedMatrix, numProfiles,
                    directoryPrefix, seqlens);
    
    /* -- SSH -- For RPS Blast additional info will be added to the file */
    if ((Boolean) myargs[2].intvalue) {
        Int4 magicNumber = RPS_MAGIC_NUMBER;
        FileWrite(&magicNumber, sizeof(Int4), 1, bigmatrixfile);
        FileWrite(&numProfiles, sizeof(Int4), 1, bigmatrixfile);
        FileWrite(seqlens, sizeof(Int4), numProfiles + 1, bigmatrixfile);
        
        sprintf(lookupName, "%s.loo", profilesFileName);
        RPSCreateLookupFile(combinedMatrix, numProfiles, seqlens, lookupName,
                            scalingFactor);

        if(!RPSConcatSequences(sequencesfp, profilesFileName)) {
            ErrPostEx(SEV_ERROR, 0,0, "Failure to concatenate sequences");
            return 1;
        }
        
    }
    
    FileWrite((void *) combinedMatrix[0], sizeof(ScoreRow), 
              (size_t) totalProfileLength, bigmatrixfile);
    freeMatrix(combinedMatrix); 
    FileClose(bigmatrixfile);
    FileClose(matrixnamefp);
    FileClose(sequencesfp);
    FileClose(auxiliaryfp);
    return 0;
}
