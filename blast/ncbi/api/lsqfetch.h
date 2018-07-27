/*  lsqfetch.h
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
* File Name:  lsqfetch.h
*
* Author:  Jinghui Zhang
*
* Version Creation Date: 5/25/95
*
* $Revision: 6.6 $
*
* File Description:  Utilities for fetching local sequences
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: lsqfetch.h,v $
* Revision 6.6  2004/08/03 17:51:49  kans
* added AsnIndexedLibFetch enable and disable functions
*
* Revision 6.5  2004/08/02 19:10:14  kans
* added CreateAsnIndex for indexing Bioseq-set ftp release files
*
* Revision 6.4  2003/11/12 23:38:49  kans
* changing AltIndexedFastaLibFetchEnable prototype, implementation not yet finished
*
* Revision 6.3  2003/08/27 19:27:43  kans
* added AltIndexedFastaLibFetch functions for chimpanzee genome project
*
* Revision 6.2  2001/03/12 23:19:33  kans
* added IndexedFastaLib functions - currently uses genome contig naming conventions
*
* Revision 6.1  1998/02/06 17:41:39  zjing
* make the function CheckDnaResidue external
*
* Revision 6.0  1997/08/25 18:06:30  madden
* Revision changed to 6.0
*
* Revision 5.2  1997/07/18 21:26:07  zjing
* *** empty log message ***
*
* Revision 5.1  1997/06/19 18:38:18  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.0  1996/05/28 13:23:23  ostell
* Set to revision 5.0
*
 * Revision 4.5  1995/11/02  21:37:55  zjing
 * .
 *
 * Revision 4.4  1995/10/11  19:29:28  zjing
 * add LIBCALL for find_big_bioseq
 *
 * Revision 4.3  1995/08/31  18:55:36  zjing
 * add include file accentr.h
 *
 * Revision 4.2  1995/08/04  17:31:32  kans
 * JZ added LocalSeqFetchInit, LocalSeqFetchDisable
 *
 * Revision 4.1  1995/08/03  20:56:18  kans
 * paths can now be specified in a regular NCBI config file
 *
 * Revision 4.0  1995/07/26  13:49:01  ostell
 * force revision to 4.0
 *
 * Revision 1.1  1995/05/25  19:50:52  zjing
 * Initial revision
 *
*
*
* ==========================================================================
*/

#ifndef _LSQFETCH_
#define _LSQFETCH_


#ifndef _NCBI_SeqMgr_
#include <seqmgr.h>
#endif

#ifndef _NCBI_Seqloc_
#include <objloc.h>
#endif

#ifndef _NCBI_Seqset_
#include <objsset.h>    
#endif

#ifndef _NCBI_SeqUtil_
#include <sequtil.h>
#endif

/* #include <accentr.h> */

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
*
*       seqid_to_string(sip, name, use_locus)
*       print the most important field in Seqid to a string stored in
*       name.
*
**********************************************************************/
NLM_EXTERN Boolean seqid_to_string PROTO((SeqIdPtr sip, CharPtr name, Boolean use_locus));

/********************************************************************** 
* 
*       FastaLibBioseqFetchEnable(libs, now)
*       Initiate the function for fetch a Bioseq from a Fasta Library 
*       file. libs is a list of library file names. 
*       If now = TRUE, open the library files and set the state to  
*       FASTALIB_OPEN. return TRUE for success.  
* 
***********************************************************************/ 
NLM_EXTERN Boolean FastaLibBioseqFetchEnable PROTO((ValNodePtr libs, Boolean now));

/********************************************************************** 
* 
*       FastaLibBioseqFetchEnable(libs, now)
*       Initiate the function for fetch a Bioseq from a Fasta Library 
*       file. libs is a list of library file names. 
*       If now = TRUE, open the library files and set the state to  
*       FASTALIB_OPEN. return TRUE for success.  
* 
***********************************************************************/ 
NLM_EXTERN void FastaLibBioseqFetchDisable PROTO((void));


#define BIN_ASN 1
#define TEXT_ASN 2
#define FASTA_FILE 3

#define FASTALIB_ERROR	0	/*can not open fasta lib*/
#define FASTALIB_OPEN	1	/*fasta library is open*/
#define FASTALIB_CLOSE	2	/*fasta libary is closed*/
typedef struct fastalib{
	Boolean use_locus;
	Char  file_name[100];
	FILE  *fp;
	Uint1 state;
	Int2 order;
	struct fastalib PNTR next;
}FastaLib, PNTR FastaLibPtr;

typedef ValNode SingleBspFile, FAR *SingleBspFilePtr; 

typedef struct filebsp{	/*structure for opening a bioseq from a file*/
	Boolean use_locus;
	CharPtr path;		/*path for the files*/
	CharPtr ext;		/*extension for files*/
	Int2 order;		/*the order of the search */
}FileBsp, PNTR FileBspPtr;	

/*********************************************************************
*
*       FileBioseqFetchEnable(path, ext)
*       Initiate a BioseqFetch function by either reading an ASN.1 
*       Seq-entry file or FASTA file. path->choice determines the 
*       type of the file, such as text ASN, binary ASN and FASTA file 
*       ext is the extension that is needed to add to the end of the 
*       sequence name to make the sequence file
*
*********************************************************************/
NLM_EXTERN Boolean FileBioseqFetchEnable PROTO((ValNodePtr path, ValNodePtr ext));





/********************************************************************** 
* 
*       FileBioseqFetchDisable() 
*       Remove the proc associated with FileBioseqFetch and free all the  
*       sequence names in userdata 
*
***********************************************************************/ 
NLM_EXTERN void FileBioseqFetchDisable PROTO((void));



NLM_EXTERN Boolean LocalSeqFetchInit PROTO((Boolean now));

/*********************************************************************
*
*       BioseqFetchInit()
*       Initiate BioseqFetch functions from local data and Entrez.
*       Local data files are stored in a config file.
*       If non is successful, return FALSE
*
*********************************************************************/
NLM_EXTERN Boolean BioseqFetchInit PROTO((Boolean now));



/***********************************************************************
* 
*       BioseqFetchDisable(): Remove all the functions associated with 
*       BioseqFetch 
* 
**********************************************************************/ 
NLM_EXTERN void BioseqFetchDisable PROTO((void));

NLM_EXTERN void LocalSeqFetchDisable PROTO((void));

/*********************************************************************
*
*	find_big_bioseq(sep)
*	return the longest Bioseq found in Seq-entry sep
*
*
**********************************************************************/
NLM_EXTERN BioseqPtr LIBCALL find_big_bioseq PROTO((SeqEntryPtr sep));

/***********************************************************************
*
*       Check if the sequence is a DNA or protein 
*       ck_len: the length for checking
*       pnon_DNA: store the number of non-DNA residue
*       return TRUE if it is a DNA sequence, FALSE for protein
*
***********************************************************************/
NLM_EXTERN Boolean CheckDnaResidue PROTO((CharPtr seq_ptr, Int4 ck_len, Int4Ptr pnon_DNA));

/**********************************************************************/

/* indexed FASTA lib functions - currently uses human genome contig naming convention */

NLM_EXTERN Boolean IndexedFastaLibFetchEnable (CharPtr path);
NLM_EXTERN void IndexedFastaLibFetchDisable (void);

/* alternative indexed FASTA lib functions - currently uses new chimpanzee genome contig naming convention */

NLM_EXTERN Boolean AltIndexedFastaLibFetchEnable (CharPtr path);
NLM_EXTERN void AltIndexedFastaLibFetchDisable (void);

/* common function for creating indexes of fasta library files */

NLM_EXTERN void CreateFastaIndex (CharPtr file);

/* indexed ASN.1 lib functions */

NLM_EXTERN Boolean AsnIndexedLibFetchEnable (CharPtr path, Boolean binary);
NLM_EXTERN void AsnIndexedLibFetchDisable (void);


/* common function for creating indexes of ASN.1 Bioseq-set ftp release files */

NLM_EXTERN void CreateAsnIndex (CharPtr file, Boolean binary);


#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif
