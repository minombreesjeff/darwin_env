static char const rcsid[] = "$Id: stsutil.c,v 6.8 2003/05/30 17:25:38 coulouri Exp $";

/* $Id: stsutil.c,v 6.8 2003/05/30 17:25:38 coulouri Exp $
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
* File Name:  $RCSfile: stsutil.c,v $
*
* Author:  Sergei Shavirin
*
* Version Creation Date: 12/18/1996
*
* $Revision: 6.8 $
*
* File Description:  NCBI STS Search Tool utilities
*   
* $Log: stsutil.c,v $
* Revision 6.8  2003/05/30 17:25:38  coulouri
* add rcsid
*
* Revision 6.7  1999/07/27 18:44:01  shavirin
* Fixed problems found on PC NT computer.
*
* Revision 6.6  1998/08/24 20:25:15  kans
* fixed -v -fd warnings
*
* Revision 6.5  1998/05/22 19:21:23  shavirin
* Updated for transfer to sunweb.
*
* Revision 6.4  1998/04/28 19:30:51  shavirin
* Fixed minor bug detected by purify
*
* Revision 6.3  1997/11/28 15:47:47  shavirin
* Removed not-used "*" in the function STSSearch_r()
*
* Revision 6.2  1997/09/25 21:32:23  shavirin
* Removed bug and changed default databases from muncher to vaster
*
* Revision 6.1  1997/09/03 17:23:00  shavirin
* Changed function STSGetORGTable()
*
* Revision 6.0  1997/08/25 18:54:49  madden
* Revision changed to 6.0
*
* Revision 1.4  1997/05/23 15:23:10  shavirin
* STS library on this step was made independent from little/big
* endian and memory mapping
*
 * Revision 1.3  1996/12/30  22:34:29  shavirin
 * Removed unused variable.
 *
 * Revision 1.2  1996/12/20  16:49:13  shavirin
 * Moved some hardcoding to defines, added detailed discription of
 * function STSMatch
 *
 * Revision 1.1  1996/12/18  20:53:26  shavirin
 * Initial revision
 *
*
* ==========================================================================
*/

/****************************************************************************/
/* INCLUDES */
/****************************************************************************/

#include <ncbi.h>
#include <stsutil.h>

/****************************************************************************/
/* DEFINES */
/****************************************************************************/

#define REAL_ADDRESS(x, y)     (x->MMAddress + ((Int4)y))
#define MAX_DBSTS_LINE_LEN 2056  /* Maximum length of STS DB file */

#define HASH_SIZE 128
#define MAXDBPAR  9   /* Number of entries in STS database file line */
#define ORGDBNUM  3   /* Number of entries in Organism list file */

#define WSIZE 7       /* Word size for STS hash table */
#define HSIZE 16384   /* Hash table size 4^WSIZE */

#define INT4_SIZE 4
#define INT2_SIZE 2
#define INT1_SIZE 1

#define DEFAULT_LEN    120  /* Default amplicon size if 0 or negative 
                               value given in STS database file */
#define DEFAULT_MARGIN 100  /* Default deviation from expected amplicon
                               size in absence of expected length*/

#define STD_MARGIN     10   /* Default deviation from expected amplican
                               size */

#define DEFAULT_START  10   /* Position of first primer in returned 
                               sequence */

/* Here are hardcoded file names used for STS Search */

/* Primers dump file from STS database */

#define STS_DEFAULT_DATABASE  "/net/ray/web/public/htdocs/STS/DB/sts.db"  

/* Organism index from STS database */

#define ORG_DEFAULT_DATABASE  "/net/ray/web/public/htdocs/STS/DB/org.db"  

/* STS map file with initialized hash info */

#define MAP_DEFAULT_DATABASE  "/net/ray/web/public/htdocs/STS/DB/sts.map" 

/****************************************************************************/
/* TYPEDEFS */
/****************************************************************************/

typedef struct STSHash {
    Int4 offset;        /* Offset in STS database file with information */
    Int4 organism;      /* Organism index as set in Organism file */
    Int4 len;           /* Expected amplicon length */
    Int4 pr1;           /* Primer 1 offset */
    Int4 pr2;           /* Primer 2 offset */
    Int4 next;
} STSHash, PNTR STSHashPtr;

typedef struct STSHashInit {
    Int4 offset;        /* Offset in STS database file with information */
    Int4 organism;      /* Organism index as set in Organism file */
    Int4 len;           /* Expected amplicon length */
    CharPtr pr1;           /* Primer 1 offset */
    CharPtr pr2;           /* Primer 2 offset */
    struct STSHashInit *next;
} STSHashInit, PNTR STSHashInitPtr;

/****************************************************************************/
/* STATIC FINCTIONS */
/****************************************************************************/

static Boolean GetSTSEntry(STSDataPtr sts_data, 
                           StsResultPtr result, Int4 offset, FILE *fd);

static STSHashInitPtr STSHashInitNew(void);
static void STSHashInitFree(STSHashInitPtr sts);

static STSOrgPtr STSOrgNew(void);
static void STSOrgFree(STSOrgPtr PNTR org);
static STSOrgPtr PNTR InitSTSOrgTable(CharPtr filename);
static void InitHashCode(void);
static void InitComplCode(void);
static CharPtr STSReverse (CharPtr from);
static Int4 STSCalculateHash(CharPtr p);
static Int4 STSMatch(STSDataPtr sts_data, CharPtr pr1, 
                     CharPtr pr2, Int4 len, CharPtr sequence);

/****************************************************************************/
/* EXTERNAL FINCTIONS */
/****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

STSHashInitPtr PNTR InitHashTable(STSDbNamesPtr db_name, 
                              Int4Ptr StsCount_out, 
                              Int4Ptr StsInvalid_out);

Int4 DumpHashTable(STSDbNamesPtr db_name , 
                   STSHashInitPtr PNTR StsHashTable);

static STSDataPtr STSDataNew(STSDbNamesPtr db_name);

#ifdef __cplusplus
}
#endif

/****************************************************************************/
/* GLOBAL VARIABLES */
/****************************************************************************/

static Int1 HashCode[128];
static Int1 ComplCode[128];
static STSDataPtr sts_glb_data = NULL; /* This will be used for non-reentrant
                                          code type */
Boolean codes_initialised = FALSE;

static STSDataPtr STSDataNew(STSDbNamesPtr db_name)
{
    STSDataPtr sts_data;
    sts_data = MemNew(sizeof(STSData));
    
    if(db_name == NULL) { 
        sts_data->sts_db_name = StringSave(STS_DEFAULT_DATABASE);      
        sts_data->sts_org_name = StringSave(ORG_DEFAULT_DATABASE);
        sts_data->sts_map_name = StringSave(MAP_DEFAULT_DATABASE);
    } else {
        if(db_name->sts_db_name != NULL)
            sts_data->sts_db_name = StringSave(db_name->sts_db_name);
        else 
            sts_data->sts_db_name = StringSave(STS_DEFAULT_DATABASE);

        if(db_name->sts_org_name != NULL)
            sts_data->sts_org_name = StringSave(db_name->sts_org_name);
        else
            sts_data->sts_org_name = StringSave(ORG_DEFAULT_DATABASE);
        
        if(db_name->sts_map_name != NULL)
            sts_data->sts_map_name = StringSave(db_name->sts_map_name);
        else
            sts_data->sts_map_name = StringSave(MAP_DEFAULT_DATABASE);
    }
    return sts_data;
}

/* -----------------------  InitSTSSearch  --------------------------
   Purpose:      All main structures, tables and memory mappings
                 will be initiated. To override default filenames of
                 main STS Search files use non-NULL parameter
   Parameters:   Structure, that contains filenames of 3 main files
                 used in search - override default names.
   Returns:      Number of STS in initialised database of -1 if
                 error.
   NOTE:         Default filenames will be set to centralized place
                 accessible via NFS. To speed up initialization
                 reccommended to copy these files on local computer
                 and initialise "db_name" structure.
  ------------------------------------------------------------------*/
Int4 InitSTSSearch(STSDbNamesPtr db_name) 
{
    return(InitSTSSearch_r(db_name, &sts_glb_data));
}


/* -----------------------  InitSTSSearch_r  --------------------------
   Purpose:      All main structures, tables and memory mappings
                 will be initiated. To override default filenames of
                 main STS Search files use non-NULL parameter
   Parameters:   Structure, that contains filenames of 3 main files
                 used in search - override default names.
   Returns:      Number of STS in initialised database of -1 if
                 error.
   NOTE:         Default filenames will be set to centralized place
                 accessible via NFS. To speed up initialization
                 reccommended to copy these files on local computer
                 and initialise "db_name" structure.
  ------------------------------------------------------------------*/
Int4 InitSTSSearch_r(STSDbNamesPtr db_name, STSDataPtr PNTR data_out) 
{
    Int4Ptr HashIndex;
    Int4 StsCount=0;
    STSDataPtr sts_data;
    FILE *fd;

    if(data_out == NULL)
        return -1;
    
    if(!codes_initialised) {
        InitHashCode();
        InitComplCode();
    }

    sts_data = STSDataNew(db_name);
    
    if((sts_data->fd_stsdb = 
        FileOpen(sts_data->sts_db_name, "rb")) == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Cannot open STS database file");
        return -1;
    }
    
    if((sts_data->OrgTable = 
        InitSTSOrgTable(sts_data->sts_org_name)) == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "ERROR: Cannot initiate Organism index\n");
        return -1;
    }    

    sts_data->mmp = NULL; /* If finaly this will stay NULL we will use
                             file in memory */
    
    if (Nlm_MemMapAvailable() == TRUE) {
  
        if((sts_data->mmp = Nlm_MemMapInit(sts_data->sts_map_name)) == NULL) {
            ErrPostEx(SEV_ERROR, 0, 0, "ERROR: Failure to map STS search "
                      "map file.");
            return -1;
        }
        
        sts_data->MMAddress = sts_data->mmp->mmp_begin;
    } else {
        if((fd = FileOpen(sts_data->sts_map_name, "rb")) == NULL)
            return -1;
        if((sts_data->MMAddress = WWWReadFileInMemory(fd, 0, FALSE)) == NULL) {
            ErrPostEx(SEV_ERROR, 0, 0, "ERROR: Failure to read STS search "
                      "map file in memory.");
            return -1;
        }
        FileClose(fd);
    }
        
    sts_data->margin = STD_MARGIN; /* This may be changed later */
    
    *data_out = sts_data;
    HashIndex = (Int4Ptr) sts_data->MMAddress;

#ifdef CHECK_MAPFILE
    {{

        STSHashPtr sts;
        int i;
        CharPtr ch1, ch2;
        
        for(i=0; i < HSIZE; i++) {
            for(sts = (STSHashPtr)(REAL_ADDRESS(sts_data, 
                                                SwapUint4(HashIndex[i])));
                
                sts != (STSHashPtr)sts_data->MMAddress; 
                sts = (STSHashPtr)(REAL_ADDRESS(sts_data, 
                                                SwapUint4(sts->next)))) {
                
                ch1 = REAL_ADDRESS(sts_data, SwapUint4(sts->pr1));
                ch2 = REAL_ADDRESS(sts_data, SwapUint4(sts->pr2));
                
                printf("%d.%d:%s:%s\n", i, StsCount, ch1, ch2);
                fflush(stdout);
                StsCount++;
            }
        }
    }}
    exit(1);
#endif 

    return SwapUint4(HashIndex[HSIZE]);
}

void STSDataFree(STSDataPtr sts_data)
{
    
    MemFree(sts_data->sts_db_name);
    MemFree(sts_data->sts_org_name);
    MemFree(sts_data->sts_map_name);
    
    FileClose(sts_data->fd_stsdb);
    if(sts_data->mmp != NULL) /* used memory mapping */
        Nlm_MemMapFini(sts_data->mmp);
    else 
        MemFree(sts_data->MMAddress);
    
    STSOrgFree(sts_data->OrgTable);


    MemFree(sts_data);
}

/* -----------------------  FiniSTSSearch  -------------------------
   Purpose:     Terminate memory mapping, deallocate structures and 
                close opened files.
   Parameters:  None
   Returns:     FALSE if error, TRUE mean success
   NOTE:        
   ------------------------------------------------------------------*/
Boolean FiniSTSSearch(void) 
{
    STSDataFree(sts_glb_data);
    return TRUE;
}
/* -------------------------  STSSearch  ---------------------------
   Purpose:      To search given string in IUPACna encoding for
                 STS primer pairs.

   Parameters:   query      - Input IUPACna string
                 organism   - index of organism in STS search database
                              0 will search all organisms
                 result     - returned STSResult structure of found STS
                              if result == NULL no hits found
   Returns:      FALSE returned in case of fatal error
   NOTE:         First check for returned TRUE if search was correct
                 then check if result not == NULL. Result is linked list,
                 so few entries may be returned at once.
  ------------------------------------------------------------------*/
Boolean STSSearch(CharPtr p, Int4 organism, 
                  StsResultPtr PNTR result_out)
{
    return STSSearch_r(sts_glb_data, p, organism, result_out);
}

/* -------------------------  STSSearch_r  ---------------------------
   Purpose:      Reentrant version of STSSearch()
                 To search given string in IUPACna encoding for
                 STS primer pairs.

   Parameters:   query      - Input IUPACna string
                 organism   - index of organism in STS search database
                              0 will search all organisms
                 result     - returned STSResult structure of found STS
                              if result == NULL no hits found
   Returns:      FALSE returned in case of fatal error
   NOTE:         First check for returned TRUE if search was correct
                 then check if result not == NULL. Result is linked list,
                 so few entries may be returned at once.
  ------------------------------------------------------------------*/
Boolean STSSearch_r(STSDataPtr sts_data, 
                    CharPtr p, Int4 organism, 
                    StsResultPtr PNTR result_out){
    
    StsResultPtr result = NULL, tmpresult = NULL, headresult = NULL;
    STSHashPtr sts;
    Int4 len, i, CopyLen;
    Int4 h;
    Int4Ptr HashIndex;
    
    if(sts_data == NULL)
        return FALSE;

    if(sts_data->MMAddress == NULL) {
        if((InitSTSSearch_r(NULL, &sts_data)) < 0)
            return FALSE;
    }
    
    HashIndex = (Int4Ptr) sts_data->MMAddress;
    
    for(i=1; *p; p++, i++) {
        
        if((h = STSCalculateHash(p)) < 0)
            continue;
    
        sts = (STSHashPtr)(REAL_ADDRESS(sts_data, SwapUint4(HashIndex[h])));
    
        while(sts != (STSHashPtr)sts_data->MMAddress) {
            if(!organism || organism == (Int4) SwapUint4(sts->organism)) {
                if((len = STSMatch(sts_data, 
                                   REAL_ADDRESS(sts_data, SwapUint4(sts->pr1)), 
                                   REAL_ADDRESS(sts_data, SwapUint4(sts->pr2)), 
                                   SwapUint4(sts->len), p)) != -1) {
                    tmpresult = (StsResultPtr) MemNew(sizeof(StsResult));
                    tmpresult->real_len = len;
                    tmpresult->exp_len = SwapUint4(sts->len);
                    tmpresult->pos = i;
                    
                    /* Fetching some data from STS Database and Entrez 
                       if detailed */
                    
                    if(!GetSTSEntry(sts_data, tmpresult, SwapUint4(sts->offset), 
                                    sts_data->fd_stsdb)) {
                        ErrPostEx(SEV_ERROR, 0, 0, 
                                  "Error reading STS database");
                        return FALSE;
                    }
          
                    tmpresult->start = i > MAX_DBSTS_LINE_LEN ? 
                        MAX_DBSTS_LINE_LEN : i-1;
                    CopyLen = (StringLen(p) - 
                               tmpresult->real_len) < MAX_DBSTS_LINE_LEN ? 
                        StringLen(p) + tmpresult->start : 
                        tmpresult->start + tmpresult->real_len + 
                        MAX_DBSTS_LINE_LEN;
                    
                    tmpresult->sequence = 
                        (CharPtr) MemNew(tmpresult->real_len + 
                                         2*MAX_DBSTS_LINE_LEN + 2);
                    
                    MemCopy(tmpresult->sequence,  p-tmpresult->start, CopyLen);
                    
                    tmpresult->sequence[CopyLen+1] = NULLB;
          
                    if(headresult == NULL) { /* just first entry */
                        headresult = tmpresult;
                        result = headresult;
                    } else {
                        result->next = tmpresult;
                        result = tmpresult;
                    }
                }
            }
            sts = (STSHashPtr)(REAL_ADDRESS(sts_data, SwapUint4(sts->next)));
        }
    }

    *result_out = headresult;
    return TRUE;
}

/* -------------------------  STSResultNew  -------------------------
   Purpose:      To allocate space for Result structure
   Parameters:   None
   Returns:     Pointer to allocated STSResult structure
   NOTE:        
  ------------------------------------------------------------------*/
StsResultPtr StsResultNew(void)
{
    StsResultPtr result;
    result = (StsResultPtr) MemNew(sizeof(StsResult));
    result->acc = NULL;
    result->sts_name = NULL;
    result->org = NULL;
    result->sequence = NULL;
    result->pr1 = NULL;
    result->pr2 = NULL;
    return result;
}

/* -------------------------  STSResultFree  ------------------------
   Purpose:      To deallocate result structure
   Parameters:   STSResult structure
   Returns:      None
   NOTE:        
  ------------------------------------------------------------------*/
void StsResultFree(StsResultPtr result)
{
    StsResultPtr tmpresult;

    while(result != NULL) {
        tmpresult = result->next;
        
        MemFree(result->acc);
        MemFree(result->sts_name);
        MemFree(result->org);
        MemFree(result->chrom);
        MemFree(result->sequence);
        MemFree(result->pr1);
        MemFree(result->pr2);
        MemFree(result);
        
        result = tmpresult;
    } 
}

/* -----------------------  STSGetOrgTable  -------------------------
   Purpose:      To return pointer to OrgTable used in STS Search
                 if It was not yet initialised filename will be used
                 to initialize it NULL initialize default filename

   Parameters:   filename to override default filename if OrgTable
                 was not yet initialized
   Returns:      Pointer to created Orgtable or NULL if error
   NOTE:         If OrgTable was already initialized by InitSTSSerch
                 filename ignored.
  ------------------------------------------------------------------*/
STSOrgPtr PNTR STSGetOrgTable(void)
{
    if(sts_glb_data == NULL)
        return InitSTSOrgTable(ORG_DEFAULT_DATABASE);
    else
        return(sts_glb_data->OrgTable);
}

/* ----------------------  STSGetOrganismIndex  ---------------------
   Purpose:      To return organism index by organism name
   Parameters:   Organism name
   Returns:      Index in OrgTable
   NOTE:         OrgTable will be initialized with default filename
                 if it was not done before
  ------------------------------------------------------------------*/
Int4 STSGetOrganismIndex(STSDataPtr sts_data, CharPtr name) 
{
    register Int4 i;
    STSDataPtr sts_touse;

    if(sts_data == NULL && sts_glb_data == NULL)
        return 0;
    
    if(sts_data != NULL)
        sts_touse = sts_data;
    else
        sts_touse = sts_glb_data;
    
    for(i = 0; i < MAXORGNUM && sts_touse->OrgTable[i] != NULL; i++) {
        if(StringStr(name, sts_touse->OrgTable[i]->string) != NULL)
            return sts_touse->OrgTable[i]->id;
    }
    return 0;  /* default */
}

/* ----------------------  STSGetOrganismName  ----------------------
   Purpose:       To return Organism name by index
   Parameters:    Inderx in STS Search OrgTable
   Returns:       Pointer to organism name
   NOTE:          OrgTable will be initialized with default filename
                  if it was not done before
  ------------------------------------------------------------------*/
CharPtr STSGetOrganismName(STSDataPtr sts_data, Int4 id) 
{
    register Int4 i;
    STSDataPtr sts_touse;

    if(sts_data == NULL && sts_glb_data == NULL)
        return NULL;
    
    if(sts_data != NULL)
        sts_touse = sts_data;
    else
        sts_touse = sts_glb_data;

    for(i = 0; i < MAXORGNUM && 
            sts_touse->OrgTable[i] != NULL; 
        i++) {
        if(sts_touse->OrgTable[i]->id == id)
            return sts_touse->OrgTable[i]->string;
    }
    return NULL;  /* default */
}

/*****************************************************************************
*
*  Function:	GetSTSEntry
*
*  Description: Look into STS dump file and populate result structure
*
*****************************************************************************/
static Boolean GetSTSEntry(STSDataPtr sts_data, StsResultPtr result, 
                           Int4 offset, FILE *fd)
{
    register Int4 i=0, j=0, k=0; 
    Int4 LineLength;
    Char TmpVal[MAXDBPAR][128];
    Char line[MAX_DBSTS_LINE_LEN];
    CharPtr p;
    
    fseek(fd, offset, SEEK_SET);
    
    if(FileGets(line, sizeof(line), fd) == NULL)
        return FALSE;
    
    LineLength = StringLen(line);
    p = line;
    for(i=0; i < LineLength; i++) {
        TmpVal[j][k] = line[i];
        
        if(line[i] == '|') {
            TmpVal[j][k] = NULLB;
            j++; k = 0;
        } else
            k++;
    }
    
    TmpVal[j][k-1] = NULLB; 
    
    if(++j != MAXDBPAR) {
        return FALSE;
  }
    
    /* Assigning some values */
    
    result->id_sts = atol(TmpVal[0]);
    result->acc =   StringSave(TmpVal[1]);
    
    result->pr1 = StringSave(TmpVal[3]);
    result->pr2 = StringSave(TmpVal[4]); 
    
    result->sts_name  =  StringSave(TmpVal[5]);
    result->org =        StringSave(STSGetOrganismName(sts_data, 
                                                       atol(TmpVal[6])));
    result->chrom     =  StringSave(TmpVal[8]);
    
    return TRUE;
}

/* ---------------------------  STSMatch  --------------------------
   Purpose:      This is core function finaly find STS hit. First it
                 checks if 1st primer totaly match query string, when
                 if expected amplicaon size is set (and not == default
                 value, that mean bad record in STS database) it uses
                 margin STD_MARGIN for deviation of second primer in
                 query string. Otherwise it uses DEFAULT_MARGIN for
                 second primer deviation.
   Parameters:   pr1, pr2 - STS primers, 
                 len - expected amplicon size
                 p - query string from position of 1st primer 
   Returns:      Observed amplicon size in case of match or
                 -1 if no match found.
   NOTE:        
  ------------------------------------------------------------------*/
static Int4 STSMatch(STSDataPtr sts_data, CharPtr pr1, 
                     CharPtr pr2, Int4 len, CharPtr p)
{
    Int4 i, len1, len2, seq_len;
    Int4 margin = DEFAULT_MARGIN;
    Int4 StartLen = DEFAULT_LEN;
    CharPtr t;
    
    if(pr1 == NULL || pr2 == NULL || p == NULL || *p == NULLB)
        return -1;
    
    if(!(len1 = StringLen(pr1)))
        return -1;
    
    if(StringNCmp(pr1, p, len1))
        return -1;
    
    if(labs(len) != DEFAULT_LEN) {
        margin = sts_data->margin;
        StartLen = labs(len);
    } 
    
    if(!(len2 = StringLen(pr2)))
        return -1;
    
    if((seq_len = StringLen(p)) < StartLen - margin) {
        return -1; /* Sequence too small */
    }

    t = p + (StartLen - len2);
  
    if(seq_len >=StartLen && !StringNCmp(t, pr2, len2)) {
        /* Complete match */
        return StartLen;
    }
    
    for(i=1; i <= margin; i++) {
        
        /* Relative match */
        if(!StringNCmp(t-i, pr2, len2)) {
            return StartLen - i;
        }
        if(seq_len>=StartLen+i && !StringNCmp(t+i, pr2, len2)) {
            return StartLen +i;
        }
    }
    return -1;
}

/*---------------------------------------------------------------
   This function calculate hash value for a nucleotide string
 ---------------------------------------------------------------*/

static Int4 STSCalculateHash(CharPtr p)
{
    Uint4 h=0;
    register Int4 i, j;
    
    if(p == NULL || p[0] == NULLB)
        return -1;
    
    for(i = 0; i < WSIZE; ++i) {
        if ((j = HashCode[*p++]) < 0)
            return -1;
        h <<=2;
        h |= (Uint4) j;
    }
    return h;
}

static CharPtr STSReverse (CharPtr from)
{
    CharPtr s;
    CharPtr t, p;
    Int4 len;
    
    len = StringLen(from);
    p = t = (CharPtr) MemNew(len+1);
    
    for (s = from+len-1; s >= from; --s, ++t)
        if ((*t = ComplCode[*s]) == 0)
            *t = 'N';
    *t = '\0';
    return p;
}

static void InitHashCode(void)
{
    Int4 i;
    for(i=0; i < HASH_SIZE; i++)
        HashCode[i] = -1;
    
    HashCode['A'] = 0;
    HashCode['C'] = 1;
    HashCode['G'] = 2;
    HashCode['T'] = 3;
    HashCode['a'] = 0;
    HashCode['c'] = 1;
    HashCode['g'] = 2;
    HashCode['t'] = 3;
    
}

static void InitComplCode(void)
{
    Int4 i;
    for(i=0; i < sizeof(ComplCode); i++)
        ComplCode[i] = -1;
    
    ComplCode['A'] = 'T';
    ComplCode['C'] = 'G';
    ComplCode['G'] = 'C';
    ComplCode['T'] = 'A';
    ComplCode['a'] = 'T';
    ComplCode['c'] = 'G';
    ComplCode['g'] = 'C';
    ComplCode['t'] = 'A';
    
    /* ambiguity codes */
    ComplCode['B'] = 'V';       /* B = C, G or T */
    ComplCode['D'] = 'H';       /* D = A, G or T */
    ComplCode['H'] = 'D';       /* H = A, C or T */
    ComplCode['K'] = 'M';       /* K = G or T */
    ComplCode['M'] = 'K';       /* M = A or C */
    ComplCode['N'] = 'N';       /* N = A, C, G or T */
    ComplCode['R'] = 'Y';       /* R = A or G (purines) */
    ComplCode['S'] = 'S';       /* S = C or G */
    ComplCode['V'] = 'B';       /* V = A, C or G */
    ComplCode['W'] = 'W';       /* W = A or T */
    ComplCode['X'] = 'X';       /* X = A, C, G or T */
    ComplCode['Y'] = 'R';       /* Y = C or T (pyrimidines) */
}

static STSOrgPtr PNTR InitSTSOrgTable(CharPtr filename)
{
    FILE *fd;
    Char line[256];
    Char TmpVal[ORGDBNUM][128];
    register Int4 i, j, k;
    Int4 LineLength, m=0;
    STSOrgPtr PNTR org;
    
    if((org = (STSOrgPtr PNTR) 
        MemNew((MAXORGNUM + 1) * sizeof(Uint4))) == NULL)
        return NULL;
  
    if((fd = FileOpen(filename, "rb")) == NULL)
        return NULL;
    
    while(FileGets(line, sizeof(line), fd) != NULL && m < MAXORGNUM) {
        j = 0; k =0;
        LineLength = StringLen(line);
        
        for(i=0; i < LineLength; i++) {
            TmpVal[j][k] = line[i];
            
            if(line[i] == '|') {
                TmpVal[j][k] = NULLB;
                j++; k = 0;
            } else
                k++;
        }
        org[m] = STSOrgNew();    
        org[m]->id = atol(TmpVal[0]);
        org[m]->string = StringSave(TmpVal[1]);
        org[m]->num = atol(TmpVal[2]);
        m++;
    }
    org[m] = NULL; /* Last Entry will be NULL */
    return org;
}

static STSOrgPtr STSOrgNew(void)
{
    STSOrgPtr org;
    org = (STSOrgPtr) MemNew(sizeof(STSOrg));
    org->string = NULL;
    return org;
}
static void STSOrgFree(STSOrgPtr PNTR org)
{
    Int4 i;

    for(i = 0; org[i] != NULL && i < MAXORGNUM; i++) {       
        MemFree(org[i]->string);
        MemFree(org[i]);
    }
    MemFree(org);
}

static STSHashInitPtr STSHashInitNew(void)
{
    STSHashInitPtr sts;
    
    sts = (STSHashInitPtr) MemNew(sizeof(STSHashInit));
    sts->len = 0;
    sts->pr1 = NULL;
    sts->pr2 = NULL;
    sts->next = NULL;
    return sts;
}
static void STSHashInitFree(STSHashInitPtr sts)
{
    MemFree(sts->pr1);
    MemFree(sts->pr2);
    MemFree(sts);
}

STSHashInitPtr PNTR InitHashTable(STSDbNamesPtr db_name, 
                              Int4Ptr StsCount_out, 
                              Int4Ptr StsInvalid_out)
{
    FILE *fd;
    Char line[2048];
    STSHashInitPtr sts;
    register Int4 i, j, k;
    Int4  LineLength, error= FALSE;
    Int4 id_sts, len, gi, StsCount = 0, StsInvalid =0;
    CharPtr pr1, pr2;
    CharPtr gb_uid, sts_uid;
    Int4 h1, h2;
    Char TmpVal[MAXDBPAR][128];
    Int4 FileOffset = 0, organism;
    STSHashInitPtr PNTR StsHashTable;
    STSOrgPtr PNTR OrgTable;

    if((StsHashTable = 
        (STSHashInitPtr PNTR) MemNew(HSIZE * 
                                 sizeof(STSHashInitPtr) + 1)) == NULL)
        return NULL;
    
    for(i = 0; i < HSIZE; i++)
        StsHashTable[i] = NULL;
    
    InitHashCode();
    InitComplCode();
  
    if((OrgTable = InitSTSOrgTable(db_name == NULL? ORG_DEFAULT_DATABASE :
                                   db_name->sts_org_name)) == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "ERROR: Cannot initiate Organism index");
        return NULL;
    }
    
    if ((fd = FileOpen(db_name == NULL? STS_DEFAULT_DATABASE : 
                       db_name->sts_db_name, "rb")) == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Cannot open database file.\n");
        return NULL;
    }
    
    FileOffset = ftell(fd);
    
    while(FileGets(line, sizeof(line), fd) != NULL) {
        j = 0; k =0;
        LineLength = StringLen(line);
        
        for(i=0; i < LineLength; i++) {
            TmpVal[j][k] = line[i];
            
            if(line[i] == '|') {
                TmpVal[j][k] = NULLB;
                j++; k = 0;
            } else
                k++;
        }
        
        /* Assigning some values */
        
        id_sts = atol(TmpVal[0]);
        gb_uid =      TmpVal[1];
        len    = atol(TmpVal[2]);
        pr1    =      TmpVal[3];
        pr2    =      TmpVal[4];
        sts_uid  =    TmpVal[5];
        organism = atol(TmpVal[6]);
        gi     =  atol(TmpVal[7]);
        
        if(++j != MAXDBPAR) {
            ErrPostEx(SEV_ERROR, 0, 0, "Error parcing database line\n");
            return NULL;
        } 
        
        if((h1 = STSCalculateHash(pr1)) < 0 || 
           (h2 = STSCalculateHash(pr2)) < 0) {
            /*      printf("Invalid database entry: %s:%s\n", pr1, pr2); */
            StsInvalid++;
            FileOffset = ftell(fd);
            continue;
        }
        
        StsCount++;
        
        /* Forward entry */
        
        sts = STSHashInitNew(); 
        sts->offset = FileOffset;
        sts->organism = organism;
        
        if(len > 0)
            sts->len = len;
        else
            sts->len = DEFAULT_LEN;
        
        sts->pr1 = StringSave(pr1);
        sts->pr2 = STSReverse(pr2); 
        
        sts->next = StsHashTable[h1];
        StsHashTable[h1] = sts;
        
        /* Backward entry */
        
        sts = STSHashInitNew();
        
        if(len > 0)
            sts->len = len;
        else
            sts->len = DEFAULT_LEN;
        
        sts->offset = FileOffset;
        sts->organism = organism;
        sts->len *= -1; 
        sts->pr1 = StringSave(pr2);
        sts->pr2 = STSReverse(pr1);
        
        sts->next = StsHashTable[h2];
        StsHashTable[h2] = sts;
        
        FileOffset = ftell(fd);
    }
    
    FileClose(fd);
    
    *StsCount_out = StsCount;
    *StsInvalid_out = StsInvalid;

    return StsHashTable;
}
#define ALIGN_NUMBER 4
Int4 DumpHashTable(STSDbNamesPtr db_name , STSHashInitPtr PNTR StsHashTable)
{
    register Int4 i;
    STSHashInitPtr sts;
    FILE *fd;
    Int4 pos, NextEntry, TotalCount =0;
    Uint4 value;

    if((fd = FileOpen(db_name == NULL? MAP_DEFAULT_DATABASE :
                      db_name->sts_map_name, "wb")) == NULL)
        return -1;
    
    NextEntry = (HSIZE+1)*INT4_SIZE; /* first entry after hash */
    
    for(i = 0; i < HSIZE; i++) {
        
        fseek(fd, i*INT4_SIZE, SEEK_SET);

        value = SwapUint4(NextEntry);
        FileWrite(&value, INT4_SIZE, 1, fd);  
        
        fseek(fd, NextEntry, SEEK_SET);
        
        for(sts=StsHashTable[i]; sts != NULL; sts = sts->next) {
            
            TotalCount++;

            value = SwapUint4(sts->offset);
            FileWrite(&value, INT4_SIZE, 1, fd);

            value = SwapUint4(sts->organism);
            FileWrite(&value, INT4_SIZE, 1, fd);

            value = SwapUint4(sts->len);
            FileWrite(&value, INT4_SIZE, 1, fd); 

            pos = ftell(fd) + 12;
            value = SwapUint4(pos);
            FileWrite(&value, INT4_SIZE, 1, fd);     /* offset of first primer */
            
            pos = pos + StringLen(sts->pr1) +1;
            value = SwapUint4(pos);
            FileWrite(&value, INT4_SIZE, 1, fd);     /* offset of second primer */

            pos = pos + StringLen(sts->pr2) +1;
            
            if(sts->next != NULL) {
                pos +=ALIGN_NUMBER-pos%ALIGN_NUMBER;
                value = SwapUint4(pos);
                FileWrite(&value, INT4_SIZE, 1, fd);     /* offset of next sts */  
            } else {
                pos = 0;
                value = SwapUint4(pos);
                FileWrite(&value, INT4_SIZE, 1, fd);     /* terminating NULL */
            }        
            
            FileWrite(sts->pr1, 1, StringLen(sts->pr1) + 1, fd);
            FileWrite(sts->pr2, 1, StringLen(sts->pr2) + 1, fd);
            
            if(sts->next != NULL) 
                fseek(fd, pos, SEEK_SET);
        }
        
        if((pos = ftell(fd)) == NextEntry) { /* Nothing were written */
            fseek(fd, i*INT4_SIZE, SEEK_SET);
            pos = 0;
            value = SwapUint4(pos);
            FileWrite(&value, INT4_SIZE, 1, fd);     /* NULL for no entries */    
        } else {
            pos += ALIGN_NUMBER-pos%ALIGN_NUMBER;
            NextEntry = pos;
        }
    }
    
    /* Saving total number of entries in the database */
    
    TotalCount = TotalCount/2;
    
    fseek(fd, HSIZE*INT4_SIZE, SEEK_SET);
    value = SwapUint4(TotalCount);
    FileWrite(&value, INT4_SIZE, 1, fd);  
    
    FileClose(fd);
    return TotalCount;
}
