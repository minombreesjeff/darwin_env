/* $Id: stsutil.h,v 6.0 1997/08/25 18:54:52 madden Exp $
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
* File Name:  $RCSfile: stsutil.h,v $
*
* Author:  Sergei Shavirin
*
* Version Creation Date: 12/18/1996
*
* $Revision: 6.0 $
*
* File Description:  Header file for NCBI STS Search Tool utilities
*   
* $Log: stsutil.h,v $
* Revision 6.0  1997/08/25 18:54:52  madden
* Revision changed to 6.0
*
* Revision 1.2  1997/05/23 15:23:22  shavirin
* STS library on this step was made independent from little/big
* endian and memory mapping
*
 * Revision 1.1  1996/12/18  20:53:38  shavirin
 * Initial revision
 *
*
* ==========================================================================
*/

#ifndef _STSUTILH_
#define _STSUTILH_ stsutil

/****************************************************************************/
/* DEFINES & TYPEDEFS */
/****************************************************************************/

typedef struct StsResult {
  Int4      id_sts;       /* STS database reference ID number        */
  Int4      gi;           /* GI currently is not available           */
  CharPtr   acc;          /* GenBank Accession number                */
  Int4      pos;          /* Start position of STS in query sequence */ 
  CharPtr   sts_name;     /* STS name from STS database              */
  Int4      real_len;     /* Amplicon length detected in search      */
  Int4      exp_len;      /* Amplicon length, that was expected      */
  CharPtr   org;          /* Organism STS belong to                  */
  CharPtr   chrom;        /* Chromosom ( may be few )                */
  CharPtr   pr1;          /* First STS primer                        */
  CharPtr   pr2;          /* Second STS primer                       */
  CharPtr   sequence;     /* Piece of query sequence containing STS  */
  Int4      start;        /* Start of STS in returned piece          */
  struct StsResult *next; /* Pointer to the next Result structure    */
} StsResult, PNTR StsResultPtr;

typedef struct STSOrg {
  Int4 id;              /* Organism id number (used only in this search) */
  Int4 num;             /* Number of entries in STS dump file            */
  CharPtr string;       /* Organism name as a string                     */
} STSOrg, PNTR STSOrgPtr;

typedef struct STSDbNames {
  CharPtr sts_db_name;       /* Override default STS database name      */
  CharPtr sts_map_name;      /* Override default STS map file name      */
  CharPtr sts_org_name;      /* Override default STS organism list name */
} STSDbNames, PNTR STSDbNamesPtr;

typedef struct STSData {
    CharPtr sts_db_name;      /* STS database filename */
    CharPtr sts_org_name;     /* Organism index name */
    CharPtr sts_map_name;     /* Formatted STS search file */
    Int4 margin;
    CharPtr MMAddress;        /* address of MM hash file */
    Nlm_MemMapPtr mmp;        /* Memory mapping pointer  */
    STSOrgPtr PNTR OrgTable; /* Pointer to organism table */
    FILE *fd_stsdb;           /* File with STS db information */
} STSData, PNTR STSDataPtr;

#define MAXORGNUM 50  /* Maximum number of organisms to be initialized */

/****************************************************************************/
/* FINCTION DEFINITIONS */
/****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

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
Boolean STSSearch(CharPtr query,  
                  Int4 organism, 
                  StsResultPtr PNTR result
                  );
Boolean STSSearch_r(STSDataPtr sts_data, 
                    CharPtr query, Int4 organism, 
                    StsResultPtr PNTR result_out);

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
Int4 InitSTSSearch(STSDbNamesPtr db_names);
Int4 InitSTSSearch_r(STSDbNamesPtr db_name, STSDataPtr PNTR data_out);

/* -----------------------  FiniSTSSearch  -------------------------
   Purpose:     Terminate memory mapping, deallocate structures and 
                close opened files.
   Parameters:  None
   Returns:     FALSE if error, TRUE mean success
   NOTE:        
  ------------------------------------------------------------------*/
Boolean FiniSTSSearch(void);


/* -----------------------  STSGetOrgTable  -------------------------
   Purpose:      To return pointer to OrgTable used in STS Search
                 if It was not yet initialised filename will be used
                 to initialize it NULL initialize default filename

   Parameters:   none
                 
   Returns:      Pointer to global Orgtable (non-reentrant)
  ------------------------------------------------------------------*/
STSOrgPtr PNTR STSGetOrgTable(void);


/* ----------------------  STSGetOrganismIndex  ---------------------
   Purpose:      To return organism index by organism name
   Parameters:   Organism name
   Returns:      Index in OrgTable
   NOTE:         OrgTable will be initialized with default filename
                 if it was not done before
  ------------------------------------------------------------------*/
Int4 STSGetOrganismIndex(STSDataPtr sts_data, CharPtr name);


/* ----------------------  STSGetOrganismName  ----------------------
   Purpose:       To return Organism name by index
   Parameters:    Inderx in STS Search OrgTable
   Returns:       Pointer to organism name
   NOTE:          OrgTable will be initialized with default filename
                  if it was not done before
  ------------------------------------------------------------------*/
CharPtr STSGetOrganismName(STSDataPtr sts_data, Int4 id);

/* -------------------------  STSResultNew  -------------------------
   Purpose:      To allocate space for Result structure
   Parameters:   None
   Returns:     Pointer to allocated STSResult structure
   NOTE:        
  ------------------------------------------------------------------*/
StsResultPtr StsResultNew(void);


/* -------------------------  STSResultFree  ------------------------
   Purpose:      To deallocate result structure
   Parameters:   STSResult structure
   Returns:      None
   NOTE:        
  ------------------------------------------------------------------*/
void StsResultFree(StsResultPtr result);
void STSDataFree(STSDataPtr sts_data);


#ifdef __cplusplus
}
#endif


#endif
