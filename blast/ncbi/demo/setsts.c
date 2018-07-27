/* $Id: setsts.c,v 6.0 1997/08/25 18:20:40 madden Exp $ */
/*****************************************************************************

    Name: setsts.c                               

    Description:  Program for formatting memory map for WWW STS Search Tool

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

 File Name:  $RCSfile: setsts.c,v $

  Author:  Sergei Shavirin

  Version Creation Date: 12/19/1996

  $Revision: 6.0 $

  File Description:
          Main program for formatting databases of Electronic PCR

  $Log: setsts.c,v $
  Revision 6.0  1997/08/25 18:20:40  madden
  Revision changed to 6.0

  Revision 5.1  1997/05/23 15:28:21  shavirin
  Initial revision


*****************************************************************************/

#include <ncbi.h>
#include <stsutil.h>

#define RealAddress(x)  (MMAddress + ((Int4)x))
#define MemMapFileName "sts.map" /* STS map file with initialized hash info */

typedef struct STSHash {
  Int4 offset;
  Int4 organism;
  Int4 len;
  CharPtr pr1;
  CharPtr pr2;
  struct STSHash *next;
} STSHash, PNTR STSHashPtr;

/* Functions used to create STS map file */

#ifdef __cplusplus
extern "C" {
#endif

extern STSHashPtr PNTR InitHashTable(STSDbNamesPtr db_name, 
                                     Int4Ptr StsCount_out, 
                                     Int4Ptr StsInvalid_out);
extern Int4 DumpHashTable(STSDbNamesPtr db_name, 
                          STSHashPtr PNTR StsHashTable);

#ifdef __cplusplus
}
#endif

/* Static functions */

static Int4 CheckHashTable(void);

/* Global variables */

static CharPtr MMAddress; /* address of MM hash file */

#define NUMARGS 4

Args dump_args[NUMARGS] = {

  {"Input STS dump file for formatting (this parameter must be set)",
   NULL, NULL,NULL,FALSE,'i',ARG_FILE_IN, 0.0,0,NULL},
  {"Organism Index file",
   "org.db", NULL,NULL,TRUE,'o',ARG_FILE_IN, 0.0,0,NULL},
  {"Output STS Map file name",
   "sts.map", NULL,NULL,TRUE,'m',ARG_FILE_OUT, 0.0,0,NULL},
  {"Logfile name:",
   "setsts.log", NULL,NULL,TRUE,'l',ARG_FILE_OUT, 0.0,0,NULL}
};

#define StsName     (CharPtr)      dump_args[0].strvalue 
#define OrgName     (CharPtr)      dump_args[1].strvalue 
#define MapName     (CharPtr)      dump_args[2].strvalue 
#define LogFileName (CharPtr)      dump_args[3].strvalue  

Int2 Main(void)
{
  Int4 StsCount = 0, StsInvalid = 0;
  STSHashPtr PNTR StsHashTable;
  STSDbNamesPtr db_name;

  if (!GetArgs("setsts",NUMARGS,dump_args)) {
    return 1;
  }
  if (!ErrSetLog (LogFileName)) {
    ErrShow();
  } else {
    ErrSetOpts (ERR_CONTINUE, ERR_LOG_ON);
  }

  db_name = MemNew(sizeof(STSDbNames));
  db_name->sts_db_name = StsName;
  db_name->sts_map_name = MapName;
  db_name->sts_org_name = OrgName;
  
  if((StsHashTable = InitHashTable(db_name, &StsCount, &StsInvalid)) == NULL) {
    ErrLogPrintf("Error in initialization of Hash Table. Exiting...");
    return 1;
  }
 
  ErrLogPrintf("Invalid database entries found: %d\n",   StsInvalid); 
  ErrLogPrintf("Hash table created with %d sequences\n", StsCount);
  
  if((StsCount = DumpHashTable(db_name, StsHashTable)) < 0) {
    ErrLogPrintf("Error in dumping of Hash Table. Exiting...");
    return 1;
  }

  ErrLogPrintf("Map file dumped with %d sequences\n", StsCount);
  return 0;
}

static Int4 CheckHashTable(void)
{
  register Int4 i;
  STSHashPtr sts;
  Nlm_MemMapPtr mmp;
  Uint4Ptr int_ptr, tmp_ptr;
  Int4 TotalCount =0;

  /* Now, lets try to read this map file */
  
  mmp = Nlm_MemMapInit(MemMapFileName);
  MMAddress = mmp->mmp_begin;
  printf("Allocated %d bytes of memory on address %x\n", 
         mmp->file_size, MMAddress);
  int_ptr = (Uint4Ptr) MMAddress;

  if((sts = (STSHashPtr)(RealAddress(int_ptr[7733]))) == 
     (STSHashPtr)MMAddress) {
    printf("No entries found\n");
    exit(1);
  } 

  printf("Off = %d Org = %d Len = %d "
         "Pr1=%d Pr2=%d Next= %d\n",
         sts->offset, 
         sts->organism, 
         sts->len, sts->pr1, 
         sts->pr2, sts->next );

  printf("Primer1 = %s, Primer2 = %s\n", 
         RealAddress(sts->pr1), 
         RealAddress(sts->pr2)
         );

  tmp_ptr = (Uint4Ptr)(RealAddress(int_ptr[7733]));
  
  printf("\nMM Start address %x\n", MMAddress);

  printf("\nRelative address %lu, Real Address %x\n",
         int_ptr[0], tmp_ptr);

  for(i=0; i < 120; i++) {
    printf("%ld ", tmp_ptr[i]);
  }
  
  printf("\nNext STS:\n");

  tmp_ptr = (Uint4Ptr)(RealAddress(sts->next));

  printf("\nRelative address %lu, Real Address %x\n",
         sts->next, tmp_ptr);

  for(i=0; i < 12; i++) {
    printf("%ld ", tmp_ptr[i]);
    fflush(stdout);
  }
  
  printf("\n");


  sts = (STSHashPtr)(MMAddress + (Int4)(sts->next));

  printf("\n Offset = %d Organism = %d Length = %d\n", 
         sts->offset, 
         sts->organism, 
         sts->len);

  return TotalCount;
}
