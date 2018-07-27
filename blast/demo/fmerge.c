/*****************************************************************************

    Name: fmerge.c                               

    Description: Program for merging FASTA databases.

    Author: Sergei Shavirin

   ***************************************************************************

                          PUBLIC DOMAIN NOTICE
              National Center for Biotechnology Information

    This software/database is a "United States Government Work" under the
    terms of the United States Copyright Act.  It was written as part of    
    the author's official duties as a United States Government employee
    and thus cannot be copyrighted.  This software/database is freely
    available to the public for use. The National Library of Medicine and
    the U.S. Government have not placed any restriction on its use or
    reproduction.

    Although all reasonable efforts have been taken to ensure the accuracy
    and reliability of the software and data, the NLM and the U.S.
    Government do not and cannot warrant the performance or results that
    may be obtained by using this software or data. The NLM and the U.S.
    Government disclaim all warranties, express or implied, including
    warranties of performance, merchantability or fitness for any
    particular purpose.

    Please cite the author in any work or product based on this material.

   ***************************************************************************

    Modification History:
           20 August of 1996 - Shavirin  -  originaly written

    Bugs and restriction on use:

    Notes:

*****************************************************************************/
#include <ncbi.h>

#define INCLENGTH 256
#define STRLENGTH 128

typedef struct MDeflineID {
  Uint4   gi;
  CharPtr defline;
  struct MDeflineID *next;
} MDeflineID, PNTR MDeflineIDPtr;

typedef struct FMergeID {
  MDeflineIDPtr id;
  CharPtr sequence;
} FMergeID, PNTR FMergeIDPtr;

Uint4Ptr GetIndexFromFasta(FILE *fd, Uint4Ptr MaxIndexCount);
Uint4Ptr GetIndexFromIndex(FILE *fd, Uint4Ptr MaxIndexCount);

static  FMergeIDPtr NextFastaFromFasta(FILE *fd);
Boolean IfGiInIndex(Uint4Ptr index, Uint4 MaxIndexCount, Uint4 gi);
Int4 MergeFMergeID(FMergeIDPtr fasta, 
                   FILE *fd, FILE *fd_ind,
                   Uint4Ptr index, 
                   Uint4 MaxIndexCount);
void FMergeIDFree(FMergeIDPtr fasta);

#define NUMARG 5

Args dump_args[NUMARG] = {
  {"Job mode (this value must be set):\n"
   "         1 - create index only\n"
   "         2 - update database from existing index\n"
   "         3 - create index and update database\n"
   "                                   ",

   NULL, NULL,NULL,FALSE,'t',ARG_STRING, 0.0,0,NULL},
  {"Logfile name:","fmerge.log",
     NULL,NULL,TRUE,'l',ARG_FILE_OUT, 0.0,0,NULL},
  {"Input file for indexing\n"
   "         Name of FASTA file, that you are going to update\n"
   "                                   ",
   "nr", NULL,NULL,TRUE,'n',ARG_FILE_IN,0.0,0,NULL},

  {"Input file for new gis:\n"
   "         Name of FASTA file with new sequences\n"
   "                                   ", 
   "month.aa", NULL,NULL,TRUE,'m',ARG_FILE_IN,0.0,0,NULL},
  {"Index file name\n"
   "         Name of file for index storage\n"
   "                                   ",
   "index.nr", NULL,NULL,TRUE,'i',ARG_FILE_IN,0.0,0,NULL},
};

#define JobModeStr  (const char *) dump_args[0].strvalue 
#define LogFileName (const char *) dump_args[1].strvalue  
#define NRFile      (const char *) dump_args[2].strvalue
#define MonthFile   (const char *) dump_args[3].strvalue
#define IndexFile   (const char *) dump_args[4].strvalue

#define INDEX_MODE  1
#define UPDATE_MODE 2
#define FULL_MODE   3

/* ------------------------------------------------------------------
                This is handler for HeapSort function
   ------------------------------------------------------------------*/
static int LIBCALLBACK intcompare(VoidPtr i, VoidPtr j)
{
  if (*(int *)i > *(int *)j)
    return (1);
  if (*(int *)i < *(int *)j)
    return (-1);
  return (0);
}

Int2 Main(void) 
{
  Uint4Ptr index = NULL;
  FILE *fd, *fd_ind, *fdout;
  Uint4 i, MaxIndexCount;
  FMergeIDPtr fasta;
  Int4 Added =0, TotalAdded =0, SequenceAdded =0;
  Int4 JobMode;

  if ( !GetArgs ("fmerge",NUMARG,dump_args) ) {
    return 1;
  }
  if ( !ErrSetLog (LogFileName) ) {
    ErrShow();
  } else {
    ErrSetOpts (ERR_CONTINUE, ERR_LOG_ON);
  }

  JobMode = atoi(JobModeStr);

  switch(JobMode) {
  case INDEX_MODE:
    ErrLogPrintf("fmerge started in \"Create index\" mode\n");
    break;
  case UPDATE_MODE:
    ErrLogPrintf("fmerge started in \"Update database\" mode\n");
    break;
  case FULL_MODE:
    ErrLogPrintf("fmerge started in \"Full mode\" mode\n");
    break;
  default:
    printf("fmerge mode \"%s\" invalid, exiting...\n", 
           JobModeStr);
    exit(1);
  }

  /* First creating index if necessary */

  if(JobMode == INDEX_MODE || JobMode == FULL_MODE) {

    if((fd = FileOpen(NRFile, "r")) == NULL) {
      ErrLogPrintf("Cannot open input file. Indexing failed...\n");
      exit(1);
    }
    ErrLogPrintf("Using file \"%s\" to create index\n", NRFile);
    
    if((index = GetIndexFromFasta(fd, &MaxIndexCount)) == NULL) {
      ErrLogPrintf("Error in creating gi index from FASTA file\n");
    }
    FileClose(fd);
    ErrLogPrintf("Created index of %d gis\n", MaxIndexCount);
    
    fd_ind = FileOpen(IndexFile, "w");
    for(i = 0; i < MaxIndexCount; i++)
      fprintf(fd_ind, "%ld\n", index[i]); 
    FileClose(fd_ind);
	
    if(JobMode == INDEX_MODE) {
      MemFree(index);
      exit(0);
    }
  }

  if(JobMode == UPDATE_MODE) {
    
    if((fd_ind = FileOpen(IndexFile, "r")) == NULL) {
      ErrLogPrintf("Cannot open file with index. Exiting...\n");
      exit(1);
    }
    if((index = GetIndexFromIndex(fd_ind, &MaxIndexCount)) == NULL) {
      ErrLogPrintf("Error in creating gi index from FASTA file\n");
    }
    FileClose(fd_ind);
    ErrLogPrintf("Created index of %d gis\n", MaxIndexCount);
  }

  /* Now searching FASTA file with new gis*/

  if((fd = FileOpen(MonthFile, "r")) == NULL) {
    ErrLogPrintf("Cannot open input file. Indexing failed...\n");
    exit(1);
  }
  ErrLogPrintf("Using file \"%s\" as FASTA database with new gis\n", 
               MonthFile);

  if((fdout = FileOpen(NRFile, "a+")) == NULL) {
    ErrLogPrintf("Error opening %s file\n", NRFile);
    exit(1);
  }
  
  if((fd_ind = FileOpen(IndexFile, "a+")) == NULL) {
    ErrLogPrintf("Error opening %s file\n", IndexFile);
    exit(1);
  }

  while((fasta = NextFastaFromFasta(fd)) != NULL) {
    if((Added = MergeFMergeID(fasta, fdout, fd_ind, 
                              index, MaxIndexCount)) != 0) {
      TotalAdded += Added;
      SequenceAdded++;
    }
  }
  MemFree(index);
  FileClose(fd);
  FileClose(fd_ind);
  ErrLogPrintf("Total gis added:       %d\n", TotalAdded);
  ErrLogPrintf("Total sequences added: %d\n", SequenceAdded);
  return 0;
}
Uint4Ptr GetIndexFromIndex(FILE *fd, Uint4Ptr MaxIndexCount)
{
 Uint4Ptr index;
 Uint4 IndexSize = INCLENGTH;
 Uint4 i =0, gi;

 index = MemNew(IndexSize*4);
 
 while (fscanf(fd, "%ld", &gi) > 0) {
   
   if(i == IndexSize) {
     IndexSize += INCLENGTH;
     index = Realloc(index, IndexSize*4);
   }
   index[i] = gi;
   i++;
 }
 *MaxIndexCount = i;
 HeapSort(index, *MaxIndexCount, sizeof(Uint4), intcompare);
 return index;
}

Uint4Ptr GetIndexFromFasta(FILE *fd, Uint4Ptr MaxIndexCount)
{
  Char str[20000];
  CharPtr pos;
  CharPtr start;
  Uint4Ptr index;
  Uint4 i =0, gi;
  Uint4 IndexSize = INCLENGTH;

  index = MemNew(IndexSize*4);

  while(fgets (str, sizeof (str), fd) != NULL) {
    start = str;
    while((pos = StringStr(start, "\1gi|")) != NULL  ||
          (pos = StringStr(start, ">gi|")) != NULL) {
      if(!sscanf(pos+4, "%ld", &gi)) {
        ErrLogPrintf("Error parsing gi number. Indexing failed...\n");
        exit(1);
      }
      /* Here we got next gi nnumber and will add it to the index */

      if(i == IndexSize) {
        IndexSize += INCLENGTH;
        index = Realloc(index, IndexSize*4);
      }
      index[i] = gi;
      i++;
      start = pos+4;
    }
  }
  *MaxIndexCount = i;
  HeapSort(index, *MaxIndexCount, sizeof(Uint4), intcompare);
  return index;
}

Boolean IfGiInIndex(Uint4Ptr index, Uint4 MaxIndexCount, Uint4 gi)
{
  Uint4 high_index, low_index, new_index; 

  if(MaxIndexCount == 0 || index == NULL || gi == 0)
    return FALSE;

  low_index  = 0;
  high_index = MaxIndexCount;
  new_index  = (low_index+high_index)/2;

  while(new_index != low_index && new_index != high_index){
    if (gi > index[new_index]) {
      low_index = new_index;
    } else if (gi < index[new_index]) {
      high_index = new_index;
    } else {	/* scores are equal. */
      return TRUE;
    }
    new_index = (low_index+high_index)/2;
  }
  if (gi == index[new_index]) 
    return TRUE;
  else
    return FALSE;
}

static FMergeIDPtr NextFastaFromFasta(FILE *fd)
{
  CharPtr str, start;
  Int4 i=0,j=0;
  Char ch;
  Int4 gi;
  CharPtr pos;
  Int4 f_pos;
  Char tmpbuff[2048];
  FMergeIDPtr fasta;
  MDeflineIDPtr id, last_id;
  CharPtr TmpSequence;
  Int4 SeqSize = STRLENGTH;

  if((fasta = MemNew(sizeof(FMergeID))) == NULL) {
    ErrLogPrintf("Cannot allocate memory for Fasta ID\n");
    exit(1);
  }
  fasta->id = NULL;

  str = MemNew(SeqSize + 5);
  str[0] = NULLB;
  for(i=0; (FileRead(&ch, sizeof(ch), 1, fd) == 1); i++) {      
    if((str[i] = ch) == '\n' || ch == '\r')
      break;
    if (i > SeqSize) {
      SeqSize = i + STRLENGTH;
      str = Realloc(str, SeqSize + 5);
    }
  }
  str[i+1] = NULLB;
  start = str;
  while((pos = StringStr(str, "\1gi|")) != NULL ||
        (pos = StringStr(str, ">gi|")) != NULL) {
    if(!sscanf(pos+4, "%ld", &gi)) {
      ErrLogPrintf("Error parsing gi number. Indexing failed...\n");
      exit(1);
    }
    pos++;
    for(j = 0;pos[j] != '\1' && 
              pos[j] != '\n' && 
              pos[j] != '\r'; j++) {
      tmpbuff[j] = pos[j];
    }
    tmpbuff[j] = '\0';

    id = MemNew(sizeof(MDeflineID));
    id->defline = StringSave(tmpbuff);
    id->gi = gi;
    id->next = NULL;

    if(fasta->id == NULL) {
      fasta->id = id;
      last_id = fasta->id;
    } else {
      last_id->next = id;
      last_id = last_id->next;
    }
    str = pos+4;
  }

  MemFree(start);

  if(fasta->id == NULL) {
    MemFree(fasta);
    return NULL;
  }

  TmpSequence = MemNew(SeqSize + 5);
  TmpSequence[0] = NULLB;


  while(TRUE) {
    f_pos = ftell(fd);
    if(fgets(tmpbuff, sizeof (tmpbuff), fd) == NULL)
      break;
    if(StringStr(tmpbuff, ">gi|") != NULL) { /* new fasta entry started */
      fseek(fd, (long) f_pos, SEEK_SET);
      break;
    }
    StringCat(TmpSequence, tmpbuff);
    SeqSize += STRLENGTH + 5;
    TmpSequence = Realloc(TmpSequence, SeqSize);
  }
  fasta->sequence = TmpSequence;
  return fasta;
}

Int4 MergeFMergeID(FMergeIDPtr fasta, FILE *fd, FILE *fd_ind,
                   Uint4Ptr index, Uint4 MaxIndexCount) 
{
  Boolean SomeNew = FALSE;
  MDeflineIDPtr id;
  Int4 TotalNot =0;

  id = fasta->id;
  while(id != NULL) {
    if(IfGiInIndex(index, MaxIndexCount, id->gi)) {
      id = id->next;
      continue;
    }
    TotalNot++;

    fprintf(fd_ind, "%ld\n", id->gi);

    if(SomeNew)
      fprintf(fd, "\1%s", id->defline);
    else {
      fprintf(fd, ">%s", id->defline);
      SomeNew = TRUE;
    }
    id = id->next;
  }

  if(SomeNew)
    fprintf(fd, "\n%s", fasta->sequence);
  
  FMergeIDFree(fasta);
  return TotalNot;
}
void FMergeIDFree(FMergeIDPtr fasta)
{
  MDeflineIDPtr id, id1;

  MemFree(fasta->sequence);
  id = fasta->id;
  while(id != NULL) {
    MemFree(id->defline);
    id1 = id;
    id = id->next;
    MemFree(id1);
  }
  MemFree(fasta);
}

