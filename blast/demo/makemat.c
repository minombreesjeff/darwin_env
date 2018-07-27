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

File name: makemat.c

Author: Alejandro Schaffer

Contents: main routines for makematrices program to convert
    PSI-BLAST checkpoints into score matrices.

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
#include <posit.h>
#include <profiles.h>
#include <sqnutils.h>


/*counts the number of items in sequencesFile and matricesFile, assumed to
  be one per line, and checks that the numbers are equal.
  returns the number if equal, 0 if unequal, rewinds the file descriptors
  before returning*/
static Int4 countProfiles(FILE *sequencesFile, FILE *profilesFile)
{
    Int4 sequencesCount = 0; /*count for sequencesFile*/
    Int4 matricesCount = 0; /*count for profilesFile*/
    Char oneFileName[MAXLINELEN]; /*for reading one line per file*/
    
    while (fgets(oneFileName,MAXLINELEN,sequencesFile))
        sequencesCount++;
    while (fgets(oneFileName,MAXLINELEN,profilesFile))
        matricesCount++;
    rewind(profilesFile);
    rewind(sequencesFile);
    if (sequencesCount == matricesCount) {
        return(sequencesCount);
    } else {
        ErrPostEx(SEV_FATAL, 0, 0, "profiles: Sequences file has %ld entries; Matrices file has %d entries; these should be equal\n", (long) sequencesCount,matricesCount);
        return(0);
    }
}


/*converts name of profile file to matrix file by changing
  suffix to mtx or appending suffix mtx*/
static Char *makeMatrixName(Char *profileName)
{
    int length; /*length of a name*/
    Char *returnName; /*string to treturn*/
    int c, lastc; /*loop indices*/
    
    length = strlen(profileName);
    returnName = (Char *) MemNew((length + 5) * sizeof(Char));
    
    for(c = 0; c < length; c++) {
        returnName[c] = profileName[c];
        if(('.' == profileName[c]) && ('c' == profileName[c+1])
           && ('h' == profileName[c+2]))
            lastc = c;
    }
    returnName[lastc] = '.';
    returnName[lastc+1] = 'm';
    returnName[lastc+2] = 't';
    returnName[lastc+3] = 'x';
    returnName[lastc+4] = '\0';
    return(returnName);
}

/*print out some parameters associated with a Karlin-Alschul
  scoring system
  checkFile is the file descriptor to write to
  kbp is the pointer to a structure with the parameters
  scaling determines whether scores are being scaled or not
  scalingDown is 1/scalingFactor because if scores are
  scaled up, then Lambda is to be scaled down*/
static void putMatrixKbp(FILE * checkFile, BLAST_KarlinBlkPtr kbp, Boolean scaling, Nlm_FloatHi scalingDown)
{
    if (scaling)
        fprintf(checkFile,"%le\n",kbp->Lambda * scalingDown);
    else
        fprintf(checkFile,"%le\n",kbp->Lambda);

   fprintf(checkFile,"%le\n",kbp->K);
   fprintf(checkFile,"%le\n",kbp->logK);
   fprintf(checkFile,"%le\n",kbp->H);
}

/*print out a score matrix into the file descriptor checkfile
  compactSerarch and psoSearch stroe information about the
  matrix and the associated sequence
  scaleScores determines whether scores are scaled or not*/
static void putMatrixMatrix(FILE *checkFile, compactSearchItems * compactSearch, posSearchItems *posSearch, Boolean scaleScores)
{
    Int4 i, j; /*loop indices*/
    
    if (scaleScores) {
        for(i = 0; i < compactSearch->qlength; i++) {
            for(j = 0; j < compactSearch->alphabetSize; j++)
                fprintf(checkFile,"%ld  ", (long) posSearch->posPrivateMatrix[i][j]);
            fprintf(checkFile,"\n");
        }
    }
    else {
        for(i = 0; i < compactSearch->qlength; i++) {
            for(j = 0; j < compactSearch->alphabetSize; j++)
                fprintf(checkFile,"%ld  ", (long) posSearch->posMatrix[i][j]);
            fprintf(checkFile,"\n");
        }
    }
}

/*Write out the matrix
  compactSearch and PosSearch include fields that store the matrix
  and the sequence
  sbp includes information about the underlying
  matrix
  fileName is where the matrix is to be written
  error_return holds error messages
  scaleScores indicates whether scores in the matrix are to be scaled
  scalingFactor is the multiplicative factor to use if scaleScores
   is true */
static Boolean takeMatrixCheckpoint(compactSearchItems * compactSearch,
    posSearchItems *posSearch,  BLAST_ScoreBlkPtr sbp, 
    Char *fileName,ValNodePtr *error_return, Boolean scaleScores, Nlm_FloatHi
    scalingFactor)
{

    FILE * checkFile; /*file in which to take the checkpoint*/
    Int4 length; /*length of query sequence, and an index for it*/
    Int4 i; /*indices to position and alphabet */
    Char localChar; /*temporary character*/
    
    checkFile = FileOpen(fileName, "w");
    
    if (NULL == checkFile) {
        ErrPostEx(SEV_ERROR, 0,0, "Could not open checkpoint file");
        return(FALSE);
    }

    length = compactSearch->qlength;
    fprintf(checkFile,"%ld\n",(long) length);
    
    for(i = 0; i < length; i++) {
        localChar = getRes(compactSearch->query[i]);

        fprintf(checkFile,"%c",localChar);
        posSearch->posMatrix[i][Xchar] = Xscore;
        posSearch->posPrivateMatrix[i][Xchar] = Xscore * scalingFactor;
    }  

    fprintf(checkFile,"\n");
    putMatrixKbp(checkFile, compactSearch->kbp_gap_std[0], scaleScores, 1/scalingFactor);
    putMatrixKbp(checkFile, compactSearch->kbp_gap_psi[0], scaleScores, 1/scalingFactor);
    putMatrixKbp(checkFile, sbp->kbp_ideal, scaleScores, 1/scalingFactor);
    putMatrixMatrix(checkFile, compactSearch, posSearch, scaleScores);

    FileClose(checkFile);
    return(TRUE);
}

/*convert to matrices is the high-level procedure to convert a set of
  PSI-BLAST checkpoints into a corresponding set of score matrices
  profilesFile is a descriptor to a file listing the file names of files
  containing checkpoints, one file name per line
  sequencesFile is a descriptor to a file listing the file names of sequences
  containing the master sequences for checkpoints, one file name per line
  matricesFile is an output file to print out the names of the newly
  produced files containing score matrices, one file name per line
  auxiliaryFile is an outputput file to contain some general information
  about the library of matrices and some parameters for each matrix
  count is the number of checkpoints/sequences
  gap_open is the cost of opening a gap
  gap_extend is the cost of extending a gap
  effectiveSize is the the size of the original sequence database used to
  make the PSI-BLAST matrices
  underlyignMatrixName is the original score matrix used to make the
  PSI-BLAST matrices
  scaleScores indicates whether scores should be scaled
  scalingFactor indicates by how much scores are scaled, if at all*/

static Int4 convertToMatrices(FILE *profilesFile, FILE *sequencesFile, FILE *matricesFile, FILE *auxiliaryFile, Int4 count, Int4 gap_open, Int4 gap_extend,
Int4 effectiveSize, Char *underlyingMatrixName, Boolean scaleScores,
Nlm_FloatHi scalingFactor, Char *directoryPrefix)
{
    int i; /*loop index over profiles*/
    FILE *thisProfileFile, *thisSequenceFile; /*file 
                descriptors for a single profile*/
    Char profileFileName[MAX_NAME_LENGTH], sequenceFileName[MAX_NAME_LENGTH]; 
    /*file names for profiles*/
    Char * matrixFileName, *relativeMatrixFileName;  /*file name for corresponding matrix file*/
    Char relativeProfileFileName[MAX_NAME_LENGTH], relativeSequenceFileName[MAX_NAME_LENGTH];
    Int4 prefixLength; /*length of directoryPrefix*/
    Int4 c1,c2; /*indices over characters in names*/
    posSearchItems *posSearch; /*used to store matrix*/
    Uint1Ptr query =NULL;  /*query sequence read in*/
    Int4  queryLength;  /*length of query sequence*/
    Int4 c; /*index over query*/
    compactSearchItems *compactSearch; /*stores query related items*/
    ValNodePtr  error_return; /*stores error messages*/
    Boolean success;      /*did one checkpoint recovery succeed*/
    BLAST_ResFreqPtr stdrfp; /* gets standard frequencies in prob field */
    Int4 a; /*index over characters*/
    SeqCodeTablePtr sctp;
    BLAST_ScoreBlkPtr sbp;
    BioseqPtr query_bsp;  /*structure to hold query information*/
    SeqEntryPtr sep;      /*structure to hold query retrieval result*/
    Int4 array_size;  /*holds returns from computing the Karlin-Altschul
                        parameters*/
    Int4Ptr open, extend; /*gap open and extension costs*/
    Nlm_FloatHiPtr lambda, K, H; /*Karlin_Altschul score paramters*/
    Int4 index; /*loop index for array_size*/
    Int4 *lengthArray; /*array of sequence lengths*/
    Nlm_FloatHi *KArray;  /*array of K values, one per sequence*/
    Int4 maxLength; /*maximum length of a sequence*/
    Int4 KarlinReturn; /*return value from calls to set up matrix parameters*/
    
    error_return = NULL;
    posSearch = (posSearchItems *) MemNew (1 * sizeof(posSearchItems));
    compactSearch = (compactSearchItems *) MemNew (1 * sizeof(compactSearchItems));
    sctp = SeqCodeTableFindObj(Seq_code_ncbistdaa);
    compactSearch->alphabetSize = sctp->num;
    
    fprintf(auxiliaryFile,"%s\n",underlyingMatrixName);
    fprintf(auxiliaryFile,"%ld\n",(long) gap_open);
    fprintf(auxiliaryFile,"%ld\n",(long) gap_extend);
    
    lengthArray = (Int4 *) MemNew(count * sizeof(Int4));
    KArray = (Nlm_FloatHi *) MemNew(count * sizeof(Nlm_FloatHi));
    maxLength = 0;
    
    if ('\0' != directoryPrefix[0]) {
        strcpy(profileFileName, directoryPrefix);
        strcpy(sequenceFileName, directoryPrefix);
        prefixLength = strlen(directoryPrefix);
    }     
    
    for(i = 0; i < count; i++) {
        if ('\0' == directoryPrefix[0])
            fscanf(profilesFile,"%s", profileFileName); 
        else {
            fscanf(profilesFile,"%s", relativeProfileFileName); 
            for(c1 = prefixLength, c2 = 0; relativeProfileFileName[c2] != '\0';
                c1++, c2++)
                profileFileName[c1] = relativeProfileFileName[c2];
            profileFileName[c1] = '\0';
        }
        if ((thisProfileFile = FileOpen(profileFileName, "rb")) == NULL) {
            ErrPostEx(SEV_FATAL, 0, 0, "Unable to open file %s\n", profileFileName);
            return (1);
        }
        if ('\0' == directoryPrefix[0])
            fscanf(sequencesFile,"%s", sequenceFileName); 
        else {
            fscanf(sequencesFile,"%s", relativeSequenceFileName); 
            for(c1 = prefixLength, c2 = 0; relativeSequenceFileName[c2] != '\0';
                c1++, c2++)
                sequenceFileName[c1] = relativeSequenceFileName[c2];
            sequenceFileName[c1] = '\0';
        }
        if ((thisSequenceFile = FileOpen(sequenceFileName, "r")) == NULL) {
            ErrPostEx(SEV_FATAL, 0, 0, "Unable to open file %s\n", sequenceFileName);
            return (1);
        }
        
        sep = FastaToSeqEntryEx(thisSequenceFile, FALSE, NULL, FALSE);
        if (sep != NULL) {
            query_bsp = NULL;
            SeqEntryExplore(sep, &query_bsp, FindProt);
            if (query_bsp == NULL) {
                ErrPostEx(SEV_FATAL, 0, 0, "Unable to obtain bioseq\n");
                return 2;
            }
            query = BlastGetSequenceFromBioseq(query_bsp, &queryLength);
        }
        compactSearch->query = query;
        for (c= 0; c < queryLength; c++)
            query[c] = ResToInt(query[c]);
        compactSearch->qlength = queryLength;
        
        
        sbp = BLAST_ScoreBlkNew(Seq_code_ncbistdaa, 1);
        sbp->read_in_matrix = TRUE;
        sbp->protein_alphabet = TRUE;
        sbp->posMatrix = NULL;
        sbp->number_of_contexts = 1;
        BlastScoreBlkMatFill(sbp, underlyingMatrixName);
        compactSearch->matrix = sbp->matrix;
        compactSearch->gapped_calculation = TRUE;
        compactSearch->pseudoCountConst = 10;
        compactSearch->ethresh = 0.001;
        BlastScoreBlkFill(sbp,  (CharPtr) query, queryLength, 0);
        
        if (0 == i) {
            fprintf(auxiliaryFile, "%le\n", sbp->kbp_std[0]->K);
            fprintf(auxiliaryFile, "%le\n", sbp->kbp_std[0]->H);
        }
        
        sbp->kbp_gap_std[0] = BlastKarlinBlkCreate();
        KarlinReturn = BlastKarlinBlkGappedCalc(sbp->kbp_gap_std[0], gap_open, gap_extend, 
                                                sbp->name, &error_return);
        if (1 == KarlinReturn) {
            BlastErrorPrint(error_return);
            return(-1);
        }
        sbp->kbp_gap_psi[0] = BlastKarlinBlkCreate();
        KarlinReturn = BlastKarlinBlkGappedCalc(sbp->kbp_gap_psi[0], gap_open, gap_extend, 
                                                sbp->name, &error_return);
        if (1 == KarlinReturn) {
            BlastErrorPrint(error_return);
            return(-1);
        }
        
        array_size = BlastKarlinGetMatrixValues(sbp->name, &open, &extend, &lambda, &K, &H, NULL);
        if (array_size > 0) {
            for (index=0; index<array_size; index++) {
                if (open[index] == INT2_MAX && extend[index] == INT2_MAX) {
                    sbp->kbp_ideal = BlastKarlinBlkCreate();
                    sbp->kbp_ideal->Lambda = lambda[index];
                    sbp->kbp_ideal->K = K[index];
                    sbp->kbp_ideal->H = H[index];
                }
            }
            MemFree(open);
            MemFree(extend);
            MemFree(lambda);
            MemFree(K);
            MemFree(H);
        }
        if (sbp->kbp_ideal == NULL)
            sbp->kbp_ideal = BlastKarlinBlkStandardCalcEx(sbp);
        compactSearch->lambda =  sbp->kbp_gap_std[0]->Lambda;
        compactSearch->kbp_std = sbp->kbp_std;
        compactSearch->kbp_psi = sbp->kbp_psi;
        compactSearch->kbp_gap_psi = sbp->kbp_gap_psi;
        compactSearch->kbp_gap_std = sbp->kbp_gap_std;
        compactSearch->lambda_ideal = sbp->kbp_ideal->Lambda;
        compactSearch->K_ideal = sbp->kbp_ideal->K;
        
        stdrfp = BlastResFreqNew(sbp);
        BlastResFreqStdComp(sbp,stdrfp); 
        compactSearch->standardProb = MemNew(compactSearch->alphabetSize * sizeof(Nlm_FloatHi));
        
        if (NULL == compactSearch->standardProb)
            exit(EXIT_FAILURE);
        for(a = 0; a < compactSearch->alphabetSize; a++)
            compactSearch->standardProb[a] = stdrfp->prob[a];
        stdrfp = BlastResFreqDestruct(stdrfp);
        
        posSearch->posInformation = NULL;
        success = impalaReadCheckpoint(posSearch, compactSearch, profileFileName, &error_return, scalingFactor);
        if (!success) {
            ErrPostEx(SEV_FATAL,0,0, "Unable to recover checkpoint from %s\n",profileFileName);
            return(1);
        }
        /*conversion to matrix and scaling is done in impalaReadCheckpopint*/
        if ('\0' == directoryPrefix[0]) {
            matrixFileName = makeMatrixName(profileFileName);
            fprintf(matricesFile,"%s\n",matrixFileName);
        } else {
            matrixFileName = makeMatrixName(profileFileName);
            relativeMatrixFileName = makeMatrixName(relativeProfileFileName);
            fprintf(matricesFile,"%s\n",relativeMatrixFileName);
        }
        
        success = takeMatrixCheckpoint(compactSearch, posSearch, sbp, matrixFileName, &error_return, scaleScores, scalingFactor);

        if (!success) {
            ErrPostEx(SEV_FATAL,0,0, "Unable to take matrix checkpoint from %s\n",profileFileName);
            return(1);
        }

        lengthArray[i] = queryLength;
        KArray[i] = sbp->kbp_gap_psi[0]->K;

        if (lengthArray[i] > maxLength)
            maxLength = lengthArray[i];

        posCheckpointFreeMemory(posSearch, queryLength);
        FileClose(thisProfileFile);
        thisProfileFile = NULL;
        FileClose(thisSequenceFile);
        thisSequenceFile = NULL;
        MemFree(query);
        SeqEntryFree(sep);
        sbp = BLAST_ScoreBlkDestruct(sbp);
        compactSearch->standardProb = MemFree(compactSearch->standardProb);

        if (success) {
            MemFree(matrixFileName);
            if ('\0' != directoryPrefix[0]) 
                MemFree(relativeMatrixFileName);
        }
    }

    fprintf(auxiliaryFile, "%ld\n", (long) maxLength);
    fprintf(auxiliaryFile, "%ld\n", (long) effectiveSize);
    fprintf(auxiliaryFile, "%lf\n", scalingFactor);

    for(i = 0; i < count; i++) {
        fprintf(auxiliaryFile, "%ld\n", (long) lengthArray[i]);
        fprintf(auxiliaryFile, "%le\n", KArray[i]);
    }
    MemFree(KArray);
    MemFree(lengthArray);
    FileClose(profilesFile);
    FileClose(sequencesFile);
    FileClose(matricesFile);
    FileClose(auxiliaryFile);
    compactSearchDestruct(compactSearch);
    MemFree(posSearch);
    BLAST_ScoreBlkDestruct(sbp);
    return(0);
}

#define NUMARG 8

static Args myargs [NUMARG] = {
    { "Database name for profile database", 
      "stdin", NULL, NULL, FALSE, 'P', ARG_FILE_IN, 0.0, 0, NULL},
    { "Cost to open a gap",
      "11", NULL, NULL, FALSE, 'G', ARG_INT, 0.0, 0, NULL},
    { "Cost to extend a gap",
      "1", NULL, NULL, FALSE, 'E', ARG_INT, 0.0, 0, NULL},
    { "Underlying Matrix", 
      "BLOSUM62", NULL, NULL, FALSE, 'U', ARG_STRING, 0.0, 0, NULL},
    { "Underlying sequence database used to make profiles", 
      "nr", NULL, NULL, FALSE, 'd', ARG_STRING, 0.0, 0, NULL},
    { "Effective length of the profile database (0 for length of -d option)",
      "0", NULL, NULL, FALSE, 'z', ARG_INT, 0.0, 0, NULL},
    { "Scaling factor for  matrix outputs to avoid round-off problems",
      "100.0", NULL, NULL, FALSE, 'S', ARG_FLOAT, 0.0, 0, NULL},
    { "Print help; overrides all other arguments",
      "F", NULL, NULL, FALSE, 'H', ARG_BOOLEAN, 0.0, 0, NULL}
};  

Int2 Main(void)
     
{
    Char *profilesDatabase;
    Char profilesFileName[MAX_NAME_LENGTH];
    Char sequencesFileName[MAX_NAME_LENGTH]; 
    Char matricesFileName[MAX_NAME_LENGTH];
    Char auxiliaryFileName[MAX_NAME_LENGTH];
    Char mmapFileName[MAX_NAME_LENGTH];
    FILE *profilesFile, *sequencesFile, *matricesFile, *auxiliaryFile;
    Int4 count; /*how many profiles*/
    Int4 effSize; /*effective database size to use*/
    Int4 retcode;
    ReadDBFILEPtr rdpt=NULL;  /*holds result of attempt to read database*/
    Boolean scaling; /*are score matrix values going to be scaled*/
    Char *directoryPrefix; /*directory where profile library is kept, used
                             to reach other directories indirectly*/
    
    if (! GetArgs ("makematrices", NUMARG, myargs)) {
        return (1);
    }
    
    if (! SeqEntryLoad())
        return (1);
    
    UseLocalAsnloadDataAndErrMsg();
    ErrSetLogLevel(SEV_WARNING);

    if ((Boolean) myargs[7].intvalue) {
        IMPALAPrintHelp(FALSE, 80, "makemat", stdout);
        return(1);
    }
    profilesDatabase = myargs[0].strvalue;
    directoryPrefix = (Char *) MemNew(MAX_NAME_LENGTH *sizeof(char));
    strcpy(directoryPrefix,profilesDatabase);
    impalaMakeFileNames(profilesDatabase,auxiliaryFileName,
                        mmapFileName,sequencesFileName,matricesFileName, 
                        profilesFileName,  directoryPrefix);
    
    if ((profilesFile = FileOpen(profilesFileName, "r")) == NULL) {
	ErrPostEx(SEV_FATAL, 0, 0, "Unable to open profiles file %s\n", profilesFileName);
	return (1);
    }
    
    if ((sequencesFile = FileOpen(sequencesFileName, "r")) == NULL) {
	ErrPostEx(SEV_FATAL, 0, 0, "Unable to open sequences file %s\n", sequencesFileName);
	return (1);
    }
    
    if ((matricesFile = FileOpen(matricesFileName, "w")) == NULL) {
	ErrPostEx(SEV_FATAL, 0, 0, "Unable to open matrices file %s\n", matricesFileName);
	return (1);
    }
    
    if ((auxiliaryFile = FileOpen(auxiliaryFileName, "w")) == NULL) {
	ErrPostEx(SEV_FATAL, 0, 0, "Unable to open auxiliary file %s\n", auxiliaryFileName);
	return (1);
    }
    
    effSize = myargs[5].intvalue;
    if (0 == effSize) {
        rdpt = readdb_new(myargs[4].strvalue, TRUE);
        effSize = readdb_get_dblen(rdpt);     
        rdpt = readdb_destruct(rdpt);
    }
    
    count = countProfiles(sequencesFile, profilesFile);
    scaling = ((myargs[6].floatvalue < 0.99) ||
               (myargs[6].floatvalue > 1.01));
    
    retcode = convertToMatrices(profilesFile, sequencesFile, matricesFile, 
                                auxiliaryFile, count,  myargs[1].intvalue, 
                                myargs[2].intvalue, effSize, myargs[3].strvalue,
                                scaling,  myargs[6].floatvalue, directoryPrefix);
    
    MemFree(directoryPrefix);
    return retcode;
}


