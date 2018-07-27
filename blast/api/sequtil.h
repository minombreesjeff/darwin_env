/*  sequtil.h
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
* File Name:  sequtil.h
*
* Author:  James Ostell
*   
* Version Creation Date: 4/1/91
*
* $Revision: 6.35 $
*
* File Description:  Sequence Utilities for objseq and objsset
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: sequtil.h,v $
* Revision 6.35  2001/09/28 22:42:49  vakatov
* Renamed "new" to "x_new" -- to avoid clash with the C++ "operator new"
*
* Revision 6.34  2001/09/28 14:31:01  madden
* Added functions BSCompressDNANew and GenericCompressDNAEx for long sequences with ambiguity
*
* Revision 6.33  2001/08/07 18:12:05  kans
* added macros for EMBL and DDBJ TPA and TPA_PROT prefixes
*
* Revision 6.32  2001/07/03 21:42:02  kans
* added macros and accession prefixes for TPA (third-party annotation) records
*
* Revision 6.31  2001/05/30 22:47:23  kans
* fixed Mac compiler warnings about unwanted assignments, moved function prototypes to header, removed unused variables
*
* Revision 6.30  2001/04/27 15:46:20  madden
* Add function RebuildDNA_4na
*
* Revision 6.29  2001/01/04 15:05:33  sicotte
* fix ACCN_IS_AMBIGOUSDB macro
*
* Revision 6.28  2000/12/07 16:34:43  sicotte
* Updated WHICH_db_accession and corresponding macros: I* accessions can no longer be proteins (they were PIR) and have completed hardcoding of N000?? accessions which can belong to twoDB. Added SeqIdFromAccessionEx, ACCN_PIR_FORMAT, and AccnIsSWISSPROT functions
*
* Revision 6.27  2000/11/16 17:23:26  sicotte
* IS_protdb_accession is now true for any 3 letter accession and IS_ntdb_accession is now also true for any unknown accession-looking accession number
*
* Revision 6.26  2000/11/14 20:49:48  sicotte
* add XM_ refseq prefix
*
* Revision 6.25  2000/10/31 21:20:06  vakatov
* [WIN32] DLL'zation
*
* Revision 6.24  2000/10/27 20:10:58  shavirin
* Added new function MakeNewProteinSeqIdExMT for MT save operation.
*
* Revision 6.23  2000/10/24 19:01:11  dondosha
* Added prototype for UniqueLocalId(), previously in mblast.h
*
* Revision 6.22  2000/10/11 21:59:36  kans
* added PRINTID_FASTA_GENERAL
*
* Revision 6.21  2000/07/11 15:01:50  kans
* added SeqIdFromAccessionDotVersion for genome mapping project
*
* Revision 6.20  2000/05/17 17:22:44  dondosha
* Added prototype for GetAccessionFromSeqId
*
* Revision 6.19  2000/05/03 17:56:05  sicotte
* Added BC prefix ; changed refseq SeqID
*
* Revision 6.18  2000/02/04 16:13:17  kans
* added prefix parameter to MakeNewProteinSeqIdEx
*
* Revision 6.17  1999/08/03 20:00:35  sicotte
* Add SeqIdFindBestAccession
*
* Revision 6.16  1999/07/30 09:28:18  sicotte
* Transfered fns from salutil.c(chappey) AddSeqId,SeqIdDupList,SeqIdDupBestList,SeqIdListfromSeqLoc
*
* Revision 6.15  1999/04/08 14:12:03  sicotte
* Add SeqIdOrderInBioseqIdList (consider synomymous SeqIds)
*
* Revision 6.14  1999/04/02 17:32:00  vakatov
* Added NLM_EXTERN for FindNuc() and FindProt() proto
*
* Revision 6.13  1999/04/01 22:23:23  sicotte
* Fixed doubly occuring prototype.
*
* Revision 6.12  1999/04/01 17:41:21  sicotte
* Added SeqIdInSeqLocList : Check if the Bioseq of SeqId sip is in list: may try to fetch the Bioseq
*
* Revision 6.11  1999/04/01 13:54:48  sicotte
* Added SeqIdOrderInList(To find the position of a SeqId in a Chain)
*       ExtractAccession ( To parse an accession into it's version and acc.)
*       SeqIdFromAccession (To make the proper type of SeqId given an
*                           accession string. (uses WHICH_db_accession)
*       moved IS_ntdb_accession, IS_protdb_accession, WHICH_db_accession
*          from accutils.ch to sequtil.ch
*
* Revision 6.10  1999/03/31 16:53:40  madden
* Added FindNuc and FindProt functions for SeqEntryExplore
*
* Revision 6.9  1999/03/04 19:38:42  kans
* now showing versions in accession numbers
*
* Revision 6.8  1999/02/02 21:38:12  kans
* moved SHOWVERSION to header, SeqMgrAddIndexElement with and without version (JO)
*
* Revision 6.7  1999/01/27 22:04:46  kans
* PRINTID_REPORT once again the end of the list
*
* Revision 6.6  1999/01/12 18:00:19  kans
* SeqIdComp now ignores version if < 1, and added PRINTID_TEXTID_ACC_VER and PRINTID_TEXTID_ACC_ONLY formats for SeqIdWrite
*
* Revision 6.5  1998/07/28 16:41:09  kans
* added MakeNewProteinSeqIdEx for faster creation of many new protein products
*
* Revision 6.4  1998/02/27 17:10:15  vakatov
* [WIN32 DLL]  Declared some functions as NLM_EXTERN(DLL-exportable)
*
* Revision 6.3  1998/02/26 19:14:08  madden
* Added AdjustOffSetsInSeqAlign
*
* Revision 6.2  1998/01/26 17:25:36  madden
* Added TxGetQueryIdFromSeqAlign function
*
* Revision 6.1  1997/10/06 14:01:28  zjing
* move TxGetSubjectId, GetScoreAndEvalue to sequtil.ch
*
* Revision 6.0  1997/08/25 18:07:27  madden
* Revision changed to 6.0
*
* Revision 5.17  1997/08/12 21:25:19  kans
* removed semicolons from SeqEntryList and BioseqList
*
* Revision 5.16  1997/06/19 18:39:03  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.15  1997/05/19 19:25:53  shavirin
* Added definition for the function CorrectGeneFeatLocation()
*
 * Revision 5.14  1997/04/09  18:43:50  tatiana
 * *** empty log message ***
 *
 * Revision 5.13  1997/04/09  18:00:34  tatiana
 * added MolTypeForGI()
 *
 * Revision 5.12  1997/03/28  21:18:38  shavirin
 * Added definition for a function BSRebuildDNA_4na()
 *
 * Revision 5.11  1997/03/06  22:48:21  shavirin
 * Moved definitions of SPCompress functions to seqport.h
 *
 * Revision 5.10  1997/03/06  21:28:25  shavirin
 * Added definitions for new set of functions: SPCompressDNA(), SPRebuildDNA(),
 * SPCompressNew(), SPCompressFree()
 *
 * Revision 5.9  1997/03/04  21:59:08  shavirin
 * Added definition of function GenericCompressDNA()
 *
 * Revision 5.8  1997/03/03  22:28:14  shavirin
 * Added definition for Compress DNA read/Write function
 *
 * Revision 5.7  1996/10/15  14:48:08  shavirin
 * Added definitions for new functions BSCompressDNA() and
 * BSRebuildDNA() handling ambiguity characters.
 *
 * Revision 5.6  1996/10/10  21:08:25  shavirin
 * Added definition for new function Convert4NaRandom(), that randomly
 * convert ncbi4na to ncbi2na encoding without SeqMapTable usage
 *
 * Revision 5.5  1996/08/07  20:12:59  epstein
 * move MuskSeqIdWrite, seqid_name, local_id_make and update_seq_loc from jzmisc to sequtil, to untangle the desktop and tools libraries from one another
 *
 * Revision 5.4  1996/07/15  14:42:42  epstein
 * add SeqEntryContainsSeqIdOfMolType() and FindCodingRegion()
 *
 * Revision 5.3  1996/06/24  18:38:33  epstein
 * move splicing functionality to sequtil.c
 *
 * Revision 5.2  1996/06/16  15:19:31  ostell
 * added delta seq support to SeqEntryPack
 *
 * Revision 5.1  1996/06/12  18:29:25  epstein
 * move SeqLocIntNew() and SeqLocPntNew() from edutil to sequtil
 *
 * Revision 5.0  1996/05/28  13:23:23  ostell
 * Set to revision 5.0
 *
 * Revision 4.3  1996/03/11  19:06:43  ostell
 * added MakeNewProteinSeqId()
 *
 * Revision 4.2  1996/01/31  16:09:15  tatiana
 * StringForSeqTech() added by Tatiana
 *
 * Revision 4.1  1995/10/17  04:15:37  ostell
 * added SeqLocFindPart()
 * made SeqLocFindNext() call it
 * added equiv_is_one to IS_one_loc()
 *
 * Revision 4.0  1995/07/26  13:49:01  ostell
 * force revision to 4.0
 *
 * Revision 2.30  1995/07/18  19:54:32  tatiana
 * add SeqIdWrite() to replace SeqIdPrint()
 *
 * Revision 2.29  1995/06/14  16:31:59  kans
 * EntrezASN1Detected function added (to be used by Sequin and ID)
 *
 * Revision 2.28  1995/05/19  04:02:08  ostell
 * added SeqLocAinB()
 * ==========================================================================
 */

#ifndef _NCBI_SeqUtil_
#define _NCBI_SeqUtil_

#ifndef _NCBI_Seqset_
#include <objsset.h>		   /* the object loader interface */
#endif

#ifndef _NCBI_SeqMgr_
#include <seqmgr.h>		   /* the Bioseq and SeqEntry manager */
#endif

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

  /*************************************************************
   *    this define decides if SeqIdWrite shows versions,
   *    if seqmgr seqid indexing functions use it
   *    and if e2index uses it
   *    files depending on SHOWVERSION are:
   *    sequtil.c, segmgr.c, e2iloc.c
   *    SHOWVERSION should be removed entirely when we are through
   *    the transition
   ************************************************************/

#define SHOWVERSION 1    /* do show versions */

/*****************************************************************************
*
*   What am I?
*
*****************************************************************************/
NLM_EXTERN Uint1 Bioseq_repr(BioseqPtr bsp);
NLM_EXTERN Uint1 BioseqGetCode(BioseqPtr bsp);

NLM_EXTERN ValNodePtr BioseqGetSeqDescr(BioseqPtr bsp, Int2 type, ValNodePtr curr);
NLM_EXTERN CharPtr BioseqGetTitle(BioseqPtr bsp);
NLM_EXTERN NumberingPtr BioseqGetNumbering(BioseqPtr bsp);

NLM_EXTERN Int4 BioseqGetLen(BioseqPtr bsp);
NLM_EXTERN Int4 BioseqGetGaps(BioseqPtr bsp);
NLM_EXTERN Int4 BioseqGetSegLens(BioseqPtr bsp, Int4Ptr lens);
#define BioseqCountSegs(x) BioseqGetSegLens(x, NULL)

NLM_EXTERN Boolean BioseqConvert(BioseqPtr bsp, Uint1 newcode);
NLM_EXTERN Boolean BioseqPack(BioseqPtr bsp);
NLM_EXTERN Boolean SeqLitPack(SeqLitPtr slp);
NLM_EXTERN Boolean BioseqRawConvert(BioseqPtr bsp, Uint1 newcode);
NLM_EXTERN Boolean BioseqRawPack(BioseqPtr bsp);
NLM_EXTERN ByteStorePtr BSConvertSeq(ByteStorePtr bsp, Uint1 newcode, Uint1 oldcode, Int4 seqlen);
NLM_EXTERN ByteStorePtr BSPack(ByteStorePtr from, Uint1 oldcode, Int4 length, Uint1Ptr newcodeptr);

NLM_EXTERN CharPtr StringForSeqMethod(Int2 method);

NLM_EXTERN CharPtr StringForSeqTech(Int2 tech);

/*****************************************************************************
*
*  Hook function definition for DNA Compression
*
*****************************************************************************/
typedef Int4 (*CompressRWFunc)(Pointer data,
                                       Uint1Ptr buf, Int4 length);

/*****************************************************************************
*
*   SeqCodeTable routines
*   SeqMapTable routines
*     Convert and Comp return INVALID_RESIDUE when a residue is out of range
*
*****************************************************************************/
#define INVALID_RESIDUE 255

/*****************************************************************************
*
*   SeqCodeTablePtr SeqCodeTableFind(code)
*   	Sequence codes defined in objseq.h
*
*****************************************************************************/
NLM_EXTERN SeqCodeTablePtr LIBCALL SeqCodeTableFind(Uint1 code);

/*****************************************************************************
*
*   SeqCodeTableComp(sctp, residue)
*       returns complement of residue if possible
*       or residue, if not
*       assumes residue is in the same code as sctp
*
*****************************************************************************/
NLM_EXTERN Uint1 SeqCodeTableComp(SeqCodeTablePtr sctp, Uint1 residue);

/*****************************************************************************
*
*   OneLetterCode(sctp)
*   	returns TRUE if sequence code table sctp uses one letter symbols
*
*****************************************************************************/
NLM_EXTERN Boolean OneLetterCode(SeqCodeTablePtr sctp);

/*****************************************************************************
*
*   FirstResidueInCode(sctp)
*   	returns first valid residue code in sequence code table
*
*****************************************************************************/
NLM_EXTERN Uint1 FirstResidueInCode(SeqCodeTablePtr sctp);

/*****************************************************************************
*
*   LastResidueInCode(sctp)
*      returns last valid residue code in sequence code table
*      nb: some codes have "holes", a range of invalid values between first
*      and last.
*
*****************************************************************************/
NLM_EXTERN Uint1 LastResidueInCode(SeqCodeTablePtr sctp);

/*****************************************************************************
*
*   GetSymbolForResidue(sctp, residue)
*   	returns the ONE LETTER symbol for residue if sequence code has one
*       letter symbols. returns INVALID_RESIDUE if not a valid residue or if
*       sequence code uses multi-letter symbols
*
*****************************************************************************/
NLM_EXTERN Uint1 GetSymbolForResidue(SeqCodeTablePtr sctp, Uint1 residue);

/*****************************************************************************
*
*   GetResidueForSymbol(sctp, residue)
*   	returns the residue for a ONE LETTER if sequence code has one
*       letter symbols. returns INVALID_RESIDUE if not a valid symbol or if
*       sequence code uses multi-letter symbols
*       CASE matters
*
*****************************************************************************/
NLM_EXTERN Uint1 GetResidueForSymbol(SeqCodeTablePtr sctp, Uint1 symbol);

/*****************************************************************************
*
*   GetLongSymbolForResidue(sctp, residue)
*   	returns string symbol for residue if sequence code has string 
*       symbols. returns NULL if not a valid residue or if
*       sequence code uses One letter symbols
*
*****************************************************************************/
NLM_EXTERN const char * GetLongSymbolForResidue(SeqCodeTablePtr sctp, Uint1 residue);

/*****************************************************************************
*
*   GetResidueForLongSymbol(sctp, symbol)
*   	returns the residue for a STRING symbol if sequence code has string
*       symbols. returns INVALID_RESIDUE if not a valid symbol or if
*       sequence code uses one-letter symbols
*       CASE matters
*
*****************************************************************************/
NLM_EXTERN Uint1 GetResidueForLongSymbol(SeqCodeTablePtr sctp, CharPtr symbol);

/*****************************************************************************
*
*   const char * GetNameForResidue (sctp, residue)
*      returns the descriptive name (eg. "Leucine") for a residue in the
*      sequence code defined by sctp
*      returns NULL if not a valid code in the alphabet
*      nb: some codes have "holes" in them, regions of values that are
*       invalid.
*
*****************************************************************************/
NLM_EXTERN const char * GetNameForResidue(SeqCodeTablePtr sctp, Uint1 residue);

/*****************************************************************************
*
*   SeqMapTablePtr SeqMapTableFind(to, from)
*      Map from sequence code "from" to sequence code "to"
*      Sequence codes defined in objseq.h
*
*****************************************************************************/
NLM_EXTERN SeqMapTablePtr LIBCALL SeqMapTableFind(Uint1 to, Uint1 from);

/*****************************************************************************
*
*   SeqMapTableConvert(smtp, from)
*       returns conversion of "from" using SeqMapTable smtp
*
*****************************************************************************/
NLM_EXTERN Uint1 SeqMapTableConvert(SeqMapTablePtr smtp, Uint1 residue);

/*****************************************************************************
*
*   Convert4NaRandom(from, to)
*       Converts Seq_code_ncbi4na "from" to  Seq_code_ncbi2na "to" 
*       with random conversions
*       Return TRUE if conversion done without randomization
*****************************************************************************/
NLM_EXTERN Boolean Convert4NaRandom(Uint1 from, Uint1 PNTR to);

/*****************************************************************************
*
*   BSCompressDNA(bytestoreptr, len, lbytes)
*       converts a ncbi4na bytestore into ncbi2na
*       returns pointer to ambiguity storage
*       lbytes[0] == length of this storage
*       frees old bytestore
*       returns pointer to new one, or NULL on fail.
*       len is residues
*
*****************************************************************************/
NLM_EXTERN ByteStorePtr BSCompressDNA(ByteStorePtr from, Int4 len, 
                                  Uint4Ptr PNTR lbytes);
NLM_EXTERN ByteStorePtr BSCompressDNANew(ByteStorePtr from, Int4 len, 
                                  Uint4Ptr PNTR lbytes);
  /* To be removed */
NLM_EXTERN ByteStorePtr BSCompressDNAOld(ByteStorePtr from, Int4 len, 
                                     Uint4Ptr PNTR lbytes);

/*****************************************************************************
*
*   GenericCompressDNA()
*       converts from VoidPtr "from" in 4na encoding to 
*       VoidPtr "to" in 2Na encoding
*       returns pointer to ambiguity storage
*       lbytes[0] == length of this storage
*       returns TRUE if succeded, or FALSE on fail.
*       seq_len is maximum number of residues in sequence 
*       or ((Uint4) -1) if final length is unknown.
*       read_func and write_func - hook functions to read from "from"
*       and to write to "to"
*
*       NOTE! read_func must return number of residues read, that usualy
*             twice as much as returned number of bytes. Only last returned
*             byte may have only one residue and this will be handled by
*             seq_len value or returned value from read_func()    
*****************************************************************************/
NLM_EXTERN Boolean GenericCompressDNA(VoidPtr from, 
                                  VoidPtr to,
                                  Uint4 length,
                                  CompressRWFunc read_func, 
                                  CompressRWFunc write_func,
                                  Uint4Ptr PNTR lbytes);

NLM_EXTERN Boolean GenericCompressDNAEx(VoidPtr from, 
                                  VoidPtr to,
                                  Uint4 length,
                                  CompressRWFunc read_func, 
                                  CompressRWFunc write_func,
                                  Uint4Ptr PNTR lbytes,
                                  Boolean x_new);

/*****************************************************************************
*
*   BSRebuildDNA(bytestoreptr, len, lbytes)
*       restore ASCII sequence with abmiguity characters
*       lbytes[0] == length of this storage
*       frees old bytestore
*       returns pointer to new one, or NULL on fail.
*       len is residues
*       lbytes is pointer to ambiguity storage
*
*****************************************************************************/
NLM_EXTERN ByteStorePtr BSRebuildDNA(ByteStorePtr from, Int4 len, 
                                 Uint4Ptr PNTR lbytes);
NLM_EXTERN Boolean RebuildDNA_4na (Uint1Ptr buffer, Int4 length, Uint4Ptr lbytes);

/*****************************************************************************
*
*   BSRebuildDNA_4na(bytestoreptr, lbytes)
*       restore ncbi4na sequence with abmiguity characters
*       lbytes[0] == length of this storage
*       frees old bytestore
*       returns pointer to new one, or NULL on fail.
*       lbytes is pointer to ambiguity storage
*
*****************************************************************************/
NLM_EXTERN ByteStorePtr BSRebuildDNA_4na (ByteStorePtr from, Uint4Ptr lbytes);


/*****************************************************************************
*
*   void NaI2TableFree(void)
*      Free allocated memory for
*      Seq_code_iupacna --> Seq_code_ncbi2na transfer
*****************************************************************************/
NLM_EXTERN void NaI2TableFree(void);

/*****************************************************************************
*
*   Numbering routines
*
*****************************************************************************/
                              /* convert any numbering value to seq offset */
NLM_EXTERN Int4 NumberingOffset(NumberingPtr np, DataValPtr avp);
                              /* convert seq offset to numbering value */
NLM_EXTERN Int2 NumberingValue(NumberingPtr np, Int4 offset, DataValPtr avp);
NLM_EXTERN Int2 NumberingValueBySeqId(SeqIdPtr sip, Int4 offset, DataValPtr avp);

NLM_EXTERN void NumberingDefaultLoad(void);
NLM_EXTERN NumberingPtr NumberingDefaultGet(void);

/*****************************************************************************
*
*   SeqEntry and BioseqSet stuff
*
*****************************************************************************/

NLM_EXTERN Uint1 Bioseq_set_class(SeqEntryPtr sep);

/*****************************************************************************
*
*   traversal routines
*       SeqEntry - any type
*
*****************************************************************************/
typedef void (* SeqEntryFunc)(SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent);
NLM_EXTERN Int4 SeqEntryList(SeqEntryPtr sep, Pointer mydata, SeqEntryFunc mycallback, Int4 index, Int2 indent);

#define SeqEntryCount( a )  SeqEntryList( a ,NULL,NULL,0,0)
#define SeqEntryExplore(a,b,c) SeqEntryList(a, b, c, 0L, 0)

/*****************************************************************************
 *
 *   void CorrectGeneFeatLocation(sep, data, n, m)
 *
 *	Correct gene location for mRNA sequences, i.e.
 *   puts start = 0, end = total_length_of_sequence - 1.
 *
 *****************************************************************************/
NLM_EXTERN void CorrectGeneFeatLocation(SeqEntryPtr sep, Pointer data, 
                             Int4 n, Int2 m);

/*****************************************************************************
*
*   traversal routines
*       Bioseq types only - "individual" sequences
*       do NOT traverse component parts of seqmented or constructed types
*
*****************************************************************************/
NLM_EXTERN Int4 BioseqList(SeqEntryPtr sep, Pointer mydata, SeqEntryFunc mycallback, Int4 index, Int2 indent);

#define BioseqCount( a )  BioseqList( a ,NULL,NULL,0,0)
#define BioseqExplore(a,b,c) BioseqList(a, b, c, 0L, 0)

/*****************************************************************************
*
*   Get parts routines
*
*****************************************************************************/
                       /* gets next Seqdescr after curr in sep of type type */
NLM_EXTERN ValNodePtr SeqEntryGetSeqDescr(SeqEntryPtr sep, Int2 type, ValNodePtr curr);
                       /* gets first title from sep */
NLM_EXTERN CharPtr SeqEntryGetTitle(SeqEntryPtr sep);

/*****************************************************************************
*
*   Manipulations
*
*****************************************************************************/

NLM_EXTERN Boolean SeqEntryConvert(SeqEntryPtr sep, Uint1 newcode);
#define SeqEntryPack(x) SeqEntryConvert(x, (Uint1)0)


/*****************************************************************************
*
*   SeqLoc stuff
*
*****************************************************************************/
#define PRINTID_FASTA_SHORT ( (Uint1)1)
#define PRINTID_FASTA_LONG ( (Uint1)2)
#define PRINTID_TEXTID_LOCUS ( (Uint1)3)
#define PRINTID_TEXTID_ACCESSION ( (Uint1)4)
#define PRINTID_TEXTID_ACC_VER ( (Uint1)5)
#define PRINTID_TEXTID_ACC_ONLY ( (Uint1)6)
#define PRINTID_REPORT ( (Uint1)7)
#define PRINTID_FASTA_GENERAL ( (Uint1)8)


/*****************************************************************************
*
*   SeqIdPtr SeqIdLocate (sip, order, num)
*   	Given a SeqId (sip):
*   		Locates the Bioseq in memory or cached
*   		Then calls SeqIdSelect with the Bioseq.id chain to find the
*             SeqId type you want.
*
*****************************************************************************/
NLM_EXTERN SeqIdPtr SeqIdLocate(SeqIdPtr sip, Uint1Ptr order, Int2 num);

/*****************************************************************************
*
*   SeqIdPtr SeqIdSelect (sip, order, num)
*   	takes an array (order) num long.
*   	goes down chain starting with sip.
*       finds lowest value of order[sip->choice] and returns it.
*       if order[] == 255, it is skipped.
*       if nothing is found < 255, NULL is returned
*   	ErrorMessage if sip->choice >= num
*
*****************************************************************************/
NLM_EXTERN SeqIdPtr SeqIdSelect(SeqIdPtr sip, Uint1Ptr order, Int2 num);

NLM_EXTERN Int2 SeqIdBestRank(Uint1Ptr buf, Int2 num);
NLM_EXTERN SeqIdPtr SeqIdFindBest(SeqIdPtr sip, Uint1 target);
NLM_EXTERN SeqIdPtr SeqIdFindBestAccession (SeqIdPtr sip);
NLM_EXTERN CharPtr SeqIdPrint(SeqIdPtr sip, CharPtr buf, Uint1 format);
NLM_EXTERN CharPtr SeqIdWrite(SeqIdPtr sip, CharPtr buf, Uint1 format, Int2 buflen);
NLM_EXTERN Boolean GetAccessionFromSeqId(SeqIdPtr sip, Int4Ptr gi, 
				     CharPtr PNTR id);
NLM_EXTERN SeqIdPtr SeqIdParse(CharPtr buf);

/*****************************************************************************
*
*   MakeNewProteinSeqId(SeqLocPtr slp, SeqIdPtr sip)
*   	Makes a new protein SeqId of attempting to keep it unique
*       Trys to match it to the input seqid type
*       slp is the location on the DNA of the coding region making the protein
*       sip is the SeqId of the DNA coding for the protein
*       if (sip != NULL) uses it for a "base" first
*       else if (slp != NULL) uses a SeqId from it for a base
*       else base is the string tmpprot
*
*       id is then base_X where X is a number assigned as a serial number
*       the returned id is guaranteed to be unique among all Bioseqs currently
*       loaded in memory. 
*   	
*   MakeNewProteinSeqIdEx(SeqLocPtr slp, SeqIdPtr sip, prefix, Int2 ctrptr)
*   	Allows you to indicate a starting count for the X in base_X, and returns
*       the next count for improved speed when allocating many protein bioseqs
*
*****************************************************************************/
NLM_EXTERN SeqIdPtr LIBCALL MakeNewProteinSeqIdExMT(SeqLocPtr slp, SeqIdPtr sip, CharPtr prefix, Int2Ptr ctrptr, Boolean is_MT_safe);
NLM_EXTERN SeqIdPtr LIBCALL MakeNewProteinSeqIdEx(SeqLocPtr slp, SeqIdPtr sip, CharPtr prefix, Int2Ptr ctrptr);
NLM_EXTERN SeqIdPtr LIBCALL MakeNewProteinSeqId(SeqLocPtr slp, SeqIdPtr sip);
NLM_EXTERN ObjectIdPtr UniqueLocalId(void);

/*****************************************************************************
*
*   Boolean BioseqMatch(bsp, seqid)
*       returns TRUE if bsp points to the Bioseq identified by seqid
*
*****************************************************************************/
NLM_EXTERN Boolean BioseqMatch(BioseqPtr bsp, SeqIdPtr sip);

NLM_EXTERN BioseqPtr BioseqFindInSeqEntry(SeqIdPtr sip, SeqEntryPtr sep);

/*****************************************************************************
*
*   Boolean SeqIdMatch(a, b)
*   	returns TRUE if SeqIds could be compared and are the same
*       returns FALSE both if SeqIds could not be compared OR if they were
*                        compared but are different
*
*   WARNING!!!! use SeqIdComp() instead of SeqIdMatch() in most cases
*
*  The code here must work the same is in two idloader
*  context: function id_flatten_seq_obj (idsybase.c)
*  and proc id_id_flatten_seq_obj
*
*****************************************************************************/
NLM_EXTERN Boolean SeqIdMatch(SeqIdPtr a, SeqIdPtr b);

/*****************************************************************************
*
*   SeqIdComp(a, b)
*   	Compares a to b and returns
*
*   SIC_DIFF   = different types, could not be compared
*   SIC_NO     = types could be compared, and ids are different
*   SIC_YES    = types could be compared, and ids are the same
*
*****************************************************************************/
NLM_EXTERN Uint1 SeqIdComp(SeqIdPtr a, SeqIdPtr b);
#define SIC_DIFF 1
#define SIC_NO 0
#define SIC_YES 2

/*************************
   SeqIdForSameBioseq(a,b)
   trys to locate all ids for a or b and determine
   if (a and b refer the the same Bioseq)
**************************/
NLM_EXTERN Boolean SeqIdForSameBioseq(SeqIdPtr a, SeqIdPtr b);

/*************************
 *      Boolean SeqIdIn (a,b)
 *   returns TRUE if a in list of b
 ******************/
NLM_EXTERN Boolean SeqIdIn(SeqIdPtr a, SeqIdPtr b);


/*****************************************************************************
*
*   SeqLocFindNext()
*     just calls SeqLocFindPart(seqlochead, currseqloc, EQUIV_IS_MANY)
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr SeqLocFindNext(SeqLocPtr seqlochead, SeqLocPtr currseqloc);

/*****************************************************************************
*
*   SeqLocFindPart(seqlochead, currseqloc, equiv_status)
*       finds the next Seq-loc after currseqloc
*       seqlochead is the first of a chain of Seq-locs
*       equiv_status defines how to treat SEQLOC_EQUIV
*         EQUIV_IS_MANY = treat same as SEQLOC_MIX
*         EQUIV_IS_ONE = return SEQLOC_EQUIV as one Seq-loc
*         FIRST_EQUIV_IS_MANY = if seqlochead is a SEQLOC_EQUIV, enter the
*            the chain of Seq-locs, but treat any later EQUIVs as
*            EQUIV_IS_ONE.
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr SeqLocFindPart(SeqLocPtr seqlochead, SeqLocPtr currseqloc, Uint1 equiv_status);

#define EQUIV_IS_MANY 0   /* treat SEQLOC_EQUIV same as SEQLOC_MIX */
#define EQUIV_IS_ONE 1	  /* treat SEQLOC_EQUIV as one Seq-loc */
#define FIRST_EQUIV_IS_MANY 2 /* treat only first EQUIV as SEQ_LOC_MIX */

NLM_EXTERN Boolean IS_one_loc(SeqLocPtr anp, Boolean equiv_is_one);  /* for SeqLoc */

NLM_EXTERN Int4 SeqLocStart(SeqLocPtr seqloc);
NLM_EXTERN Int4 SeqLocStop(SeqLocPtr seqloc);
NLM_EXTERN Uint1 SeqLocStrand(SeqLocPtr seqloc);
NLM_EXTERN Int4 SeqLocLen(SeqLocPtr seqloc);
NLM_EXTERN Int4 SeqLocGetSegLens(SeqLocPtr slp, Int4Ptr lens, Int4 ctr, Boolean gaps);
#define SeqLocCountSegs(x) SeqLocGetSegLens(x, NULL,0,FALSE)
#define SeqLocGetGaps(x) SeqLocGetSegLens(x,NULL,0,TRUE)
NLM_EXTERN SeqIdPtr SeqLocId(SeqLocPtr seqloc);
NLM_EXTERN Uint1 StrandCmp(Uint1 strand);
NLM_EXTERN Boolean SeqLocRevCmp(SeqLocPtr anp);

/**** defines for "which_end" below ****/

#define SEQLOC_LEFT_END  1    /* low numbered end of SeqLoc */
#define SEQLOC_RIGHT_END 2    /* high numbered end of SeqLoc */
#define SEQLOC_START     3	  /* beginning of SeqLoc (low on plus, high on minus)  */
#define SEQLOC_STOP      4	  /* end of SeqLoc (high on plus, low on minus)  */

NLM_EXTERN Int4 GetOffsetInLoc(SeqLocPtr of, SeqLocPtr in, Uint1 which_end);
NLM_EXTERN Int4 GetOffsetInBioseq(SeqLocPtr of, BioseqPtr in, Uint1 which_end);
NLM_EXTERN Int2 SeqLocOrder(SeqLocPtr a, SeqLocPtr b, BioseqPtr in);

NLM_EXTERN Int2 SeqLocMol(SeqLocPtr seqloc);

NLM_EXTERN CharPtr SeqLocPrint(SeqLocPtr slp);


/*****************************************************************************
*
*   SeqLocCompare(a, b)
*   	returns
*   	0 = no overlap
*   	1 = a is completely contained in b
*   	2 = b is completely contained in a
*   	3 = a == b
*   	4 = a and b overlap, but neither completely contained in the other
*
*****************************************************************************/
NLM_EXTERN Int2 SeqLocCompare(SeqLocPtr a, SeqLocPtr b);
#define SLC_NO_MATCH 0
#define SLC_A_IN_B 1
#define SLC_B_IN_A 2
#define SLC_A_EQ_B 3
#define SLC_A_OVERLAP_B 4


/*****************************************************************************
*
*   SeqLocAinB(a, b)
*      if a is completely contained in b, a positive number is returned
*         if 0, a is identical with b
*         if not 0, is the number of residues bigger b is than a
*      if a negative number is returned, a is not contained in b
*         could overlap or not
*      used to find features contained in genes
*
*****************************************************************************/
NLM_EXTERN Int4 SeqLocAinB(SeqLocPtr a, SeqLocPtr b);

NLM_EXTERN Boolean SeqIntCheck(SeqIntPtr sip);   /* checks for valid interval */
NLM_EXTERN Boolean SeqPntCheck(SeqPntPtr spp);  /* checks valid pnt */
NLM_EXTERN Boolean PackSeqPntCheck(PackSeqPntPtr pspp);
NLM_EXTERN Uint1 SeqLocCheck(SeqLocPtr slp);
#define SEQLOCCHECK_OK 2      /* location is fine */
#define SEQLOCCHECK_WARNING 1   /* location ok, but has mixed strands */
#define SEQLOCCHECK_ERROR 0     /* error in location */
/*****************************************************************************
*
*   SeqLocPartialCheck(head)
*       sets bits for incomplete location and/or errors
*       incomplete defined as Int-fuzz on start or stop with
*         lim.unk, lim.gt, or lim.lt set
*   
* SLP_COMPLETE = not partial and no errors
* SLP_START = incomplete on start (high number on minus strand, low on plus)
* SLP_STOP     = incomplete on stop
* SLP_INTERNAL = lim set on internal intervals
* SLP_OTHER    = partial location, but no details available
* SLP_NOSTART  = start does not include end of sequence
* SLP_NOSTOP   = stop does not include end of sequence
* SLP_NOINTERNAL = internal interval not on end of sequence
* SLP_LIM_WRONG  = lim gt/lt used inconsistently with position in location
*
* SLP_HAD_ERROR  = if AND with return, is TRUE if any errors encountered
*   
*****************************************************************************/

#define SLP_COMPLETE	0
#define SLP_START		1
#define SLP_STOP		2
#define SLP_INTERNAL	4
#define SLP_OTHER		8
#define SLP_NOSTART		16
#define SLP_NOSTOP		32
#define SLP_NOINTERNAL	64
#define SLP_LIM_WRONG	128

#define SLP_HAD_ERROR   240

NLM_EXTERN Uint2 SeqLocPartialCheck(SeqLocPtr head);

NLM_EXTERN CharPtr TaxNameFromCommon(CharPtr common);

/*****************************************************************************
*
*   QualLocCreate(from, to)
*   	creates a UserObject of _class NCBI, type 1
*       adds a field of type "qual_loc"
*       puts the from and to numbers in
*       These should be offsets, as in a Seq-loc, not numbers starting from
*           one.
*       no range check, no strand, no seqid
*       this just carries locations for the qualifiers anticodon and rpt_unit
*       Intended to go on SeqFeat.ext
*
*****************************************************************************/
NLM_EXTERN UserObjectPtr QualLocCreate(Int4 from, Int4 to);

/*****************************************************************************
*
*   QualLocWrite(uop, buf)
*   	Checks a SeqFeat.ext to see if it is
*   		1) not null
*           2) has a UserObject of _class NCBI, type 1
*           3) has a field of label "qual_loc"
*           4) if so, prints the two integers as a qualifier location
*               from..to and returns a pointer to the \0 after "to"
*           Adds 1 to the internal numbers to convert from offset to
*               number starting with 1
*       If any of the above fail, returns NULL
*
*****************************************************************************/
NLM_EXTERN CharPtr QualLocWrite(UserObjectPtr uop, CharPtr buf);

/*****************************************************************************
*
*   EntrezASN1Detected detects records retrieved from Entrez, which should
*       not be edited by Sequin and replaced into ID.
*
*****************************************************************************/

NLM_EXTERN Boolean EntrezASN1Detected (SeqEntryPtr sep);

/*****************************************************************************
*
*   SeqLocIntNew(Int4 from, Int4 to, Uint1 strand, SeqIdPtr sip)
*      creates a new SeqLoc of type SeqInt
*      makes copy of incoming SeqId
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL SeqLocIntNew (Int4 from, Int4 to, Uint1 strand, SeqIdPtr sip);

/*****************************************************************************
*
*   SeqLocPntNew(Int4 pos, Uint1 strand, SeqIdPtr sip, Boolean is_fuzz)
*      creates a new SeqLoc of type SeqPnt
*      makes copy of incoming SeqId
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL SeqLocPntNew (Int4 pos, Uint1 strand, SeqIdPtr sip, Boolean is_fuzz);

/*****************************************************************************
*
*   SeqLocPtr FindSpliceSites(SeqEntryPtr sep, Boolean findOnProtein)
*      Finds the splice sites on this SeqEntry and returns them as a
*      SeqLoc.
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL FindSpliceSites(SeqEntryPtr sep, Boolean findOnProtein);

/***************************************************************************
**
*
*   SeqFeatPtr FindCodingRegion(SeqEntryPtr sep)
*      Finds the coding region feature on this protein SeqEntry and
*      returns a copy of it.
*
****************************************************************************
*/
NLM_EXTERN SeqFeatPtr LIBCALL FindCodingRegion(SeqEntryPtr sep);

/*****************************************************************************
*
*   Boolean LIBCALL SeqEntryContainsSeqIdOfMolType(SeqEntryPtr sep, SeqIdPtr sip, Boolean isProtein)
*      Tests to see if this SeqEntry contains a bioseq of the specified moltype
*        (protein or DNA)
*      if sip != NULL then it also insists upon finding a bioseq of the
*        specified moltype where the SeqIds match
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqEntryContainsSeqIdOfMolType(SeqEntryPtr sep, SeqIdPtr sip, Boolean isProtein);

/*****************************************************************************
*
*      Tests to see if this SeqEntry contains a bioseq of the specified uid
*      returns moltype of the bioseq where the SeqIds match
*			  0     id not found in this SeqEntry 
*		      1     Amino Acid sequence 
*			  2     Nucleotide sequence 
*	  
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL MolTypeForGI(SeqEntryPtr sep, Int4 uid);

/* moved from jzmisc.h */
NLM_EXTERN Boolean seqid_name(SeqIdPtr, CharPtr, Boolean, Boolean);
NLM_EXTERN Boolean MuskSeqIdWrite(SeqIdPtr sip, CharPtr buf, Int2 buflen, Uint1 format, Boolean do_find, Boolean do_entrez_find);
NLM_EXTERN SeqIdPtr local_id_make(CharPtr);
NLM_EXTERN SeqLocPtr update_seq_loc(Int4, Int4, Uint1, SeqLocPtr );
NLM_EXTERN SeqIdPtr LIBCALL TxGetSubjectIdFromSeqAlign(SeqAlignPtr seqalign);
NLM_EXTERN SeqIdPtr LIBCALL TxGetQueryIdFromSeqAlign(SeqAlignPtr seqalign);
NLM_EXTERN Boolean LIBCALL GetScoreAndEvalue(
                    SeqAlignPtr seqalign, Int4 *score, 
                    Nlm_FloatHi *bit_score, 
                    Nlm_FloatHi *evalue, Int4 *number
);

/***********************************************************************
*
*       Adjust the Offset in the SeqAlign to correspond to the beginning
*       of the sequence and not where BLAST (or some other tool) started.
*
**********************************************************************/

NLM_EXTERN void LIBCALL AdjustOffSetsInSeqAlign(SeqAlignPtr salp, SeqLocPtr slp1, SeqLocPtr slp2);


/* Used with SeqEntryExplore to find Bioseq's in a SeqEntry. */
NLM_EXTERN void FindNuc(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
NLM_EXTERN void FindProt(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);

/*****************************************************************************
*
*   Boolean SeqIdOrderInList(a, b)
*     Looks for single SeqId, "a" in chain of SeqIds, "b"
*     returns the position (>0) if found.. else returns 0;
*
*****************************************************************************/

NLM_EXTERN Uint4 LIBCALL SeqIdOrderInList (SeqIdPtr a, SeqIdPtr list);

/*****************************************************************************
*
*   Boolean SeqIdOrderInBioseqIdList(a, b)
*     Looks for single SeqId, "a" in chain of SeqIds, "b"
*              and looks at all synonymous SeqIds of the Bioseq "b"
*     returns the position (>0) if found.. else returns 0;
*
*****************************************************************************/
NLM_EXTERN Uint4 LIBCALL SeqIdOrderInBioseqIdList (SeqIdPtr a, SeqIdPtr list);

/* Function to extract the Accession and version number 
   User must provide string buffers for answer.
   */
NLM_EXTERN void LIBCALL ExtractAccession(CharPtr accn,CharPtr accession,CharPtr version);



/*
  Function to make a proper type SeqId given a string that represents
  an accession Number 
  User must Call ExtractAccession function separately before calling this.
  to split accession and version number.
*/
NLM_EXTERN SeqIdPtr LIBCALL SeqIdFromAccession(CharPtr accession, Uint4 version,CharPtr name);

    /* Variant that also work with PIR accessions and LOCUS names 
       .. and can resolve conflict with network access (if pre-enabled)
     */
    NLM_EXTERN  SeqIdPtr LIBCALL SeqIdFromAccessionEx(CharPtr accession, Uint4 version,CharPtr name,Boolean Permissive, Boolean AllowPIR,Boolean UseNetwork,Boolean FavorNucleotide);

/* Variant of SeqIdFromAccession that works on accession.version string */

NLM_EXTERN SeqIdPtr SeqIdFromAccessionDotVersion (CharPtr accession);


    /*
      Following functions and defines moved from accutils.ch
      */
NLM_EXTERN Uint4 LIBCALL WHICH_db_accession (CharPtr s);
NLM_EXTERN Boolean LIBCALL IS_ntdb_accession (CharPtr s);
NLM_EXTERN Boolean LIBCALL IS_protdb_accession (CharPtr s);
NLM_EXTERN Boolean LIBCALL ACCN_PIR_FORMAT( CharPtr s);
NLM_EXTERN Boolean LIBCALL ACCN_1_5_FORMAT( CharPtr s);
NLM_EXTERN Boolean LIBCALL AccnIsSWISSPROT( CharPtr s);
NLM_EXTERN Boolean LIBCALL NAccnIsGENBANK (CharPtr s);
NLM_EXTERN Boolean LIBCALL NAccnIsEMBL (CharPtr s);
NLM_EXTERN Boolean LIBCALL NAccnIsDDBJ (CharPtr s);


/*
  #defines and macros for WHICH_ntdb_accession and
                          WHICH_protdb_accession

 The "divisions" implied by the following #defines are not all inclusives.
   a GSS or EST sequence submitted through DIRSUB, will have the 
   ACCN_NCBI_DIRSUB code.
   a sequence can full well be in GSS,EST,etc.. division
   but not have the appropriate accession number if they were submitted
   through DIRSUB.

*/
#define ACCN_UNKNOWN 0

#define ACCN_AMBIGOUS_DB 2 /* Primary can be from any Nucleotide database */
#define ACCN_SWISSPROT 3
#define ACCN_NCBI_PROT 4
#define ACCN_EMBL_PROT 5
#define ACCN_DDBJ_PROT 6

#define ACCN_GSDB_DIRSUB 7

#define ACCN_NCBI_GSDB  8 /* NCBI-assigned Accn to GSDB records */

#define ACCN_NCBI_EST 9
#define ACCN_NCBI_DIRSUB 10
#define ACCN_NCBI_GENOME 11
#define ACCN_NCBI_PATENT 12 /* Not used .. because all are Ambigous_mol */
#define ACCN_NCBI_HTGS 13
#define ACCN_NCBI_GSS 14
#define ACCN_NCBI_STS 15
#define ACCN_NCBI_BACKBONE 16 /* "S" record, typed from publications */
#define ACCN_NCBI_SEGSET 17
#define ACCN_NCBI_OTHER 18 /* unknown or 'other' nucleotide division */

#define ACCN_EMBL_EST 19
#define ACCN_EMBL_DIRSUB 20
#define ACCN_EMBL_GENOME 21
#define ACCN_EMBL_PATENT 22
#define ACCN_EMBL_HTGS 23 /* Not defined yet */
#define ACCN_EMBL_CON 24
#define ACCN_EMBL_OTHER 25 /* unknown or 'other' nucleotide division */

#define ACCN_DDBJ_EST 26
#define ACCN_DDBJ_DIRSUB 27
#define ACCN_DDBJ_GENOME 28
#define ACCN_DDBJ_PATENT 29
#define ACCN_DDBJ_HTGS 30
#define ACCN_DDBJ_CON 31 /* Not defined*/
#define ACCN_DDBJ_OTHER 32 /* unknown or 'other' nucleotide division */

#define ACCN_REFSEQ_PROT 33
#define ACCN_REFSEQ_mRNA 34
#define ACCN_REFSEQ_CONTIG 35
#define ACCN_REFSEQ_CHROMOSOME 36
#define ACCN_REFSEQ_mRNA_PREDICTED 37
#define ACCN_REFSEQ_PROT_PREDICTED 38
#define ACCN_REFSEQ_GENOMIC 39

#define ACCN_NCBI_cDNA 40 
#define ACCN_IS_PROTEIN 41 /* unreserved 3 letter code .. must be protein*/
#define ACCN_IS_NT 42  /* unreserved 1 or 2 letter code .. must be nuc */
#define ACCN_REFSEQ 43  /* unreserved refseq-type two_letters and underscore*/
#define ACCN_EMBL_GB 44
#define ACCN_EMBL_DDBJ 45
#define ACCN_GB_DDBJ 46
#define ACCN_EMBL_GB_DDBJ 47

#define ACCN_NCBI_TPA 48
#define ACCN_NCBI_TPA_PROT 49
#define ACCN_EMBL_TPA 50
#define ACCN_EMBL_TPA_PROT 51
#define ACCN_DDBJ_TPA 52
#define ACCN_DDBJ_TPA_PROT 53


/* Some accessions prefix can be either protein or nucleotide 
   such as NCBI PATENT I, AR .. or segmented set Bioseqs 'AH'
*/
#define ACCN_AMBIGOUS_MOL 65536 /* Ambigous Molecule */

/* 
   Macros to interpret above #defines codes returned by
   WHICH_db_accession
*/


/*
 Accession definitively points to a protein record 
*/
#define ACCN_IS_PROT(c) (((c)==ACCN_SWISSPROT) ||  ( (c)==ACCN_NCBI_PROT) || ((c)== ACCN_EMBL_PROT) || ((c)== ACCN_DDBJ_PROT) || ((c)== ACCN_REFSEQ_PROT) || ((c)== ACCN_IS_PROTEIN) || ((c)== ACCN_REFSEQ_PROT_PREDICTED) || ((c)== ACCN_NCBI_TPA_PROT) || ((c)== ACCN_EMBL_TPA_PROT) || ((c)== ACCN_DDBJ_TPA_PROT))

/*
  Accession definitively points to a nucleotide record 
   . note that ACCN_dbname_OTHER is a nucleotide.
*/
#define ACCN_IS_NUC(c) ((((c)&ACCN_AMBIGOUS_MOL)==0) && ((c)!=ACCN_UNKNOWN) && (!ACCN_IS_PROT(c)) )

#define ACCN_IS_AMBIGOUS_MOL(c) (((c)&ACCN_AMBIGOUS_MOL) == ACCN_AMBIGOUS_MOL)

/* 
   Define to detect Genbank's accessions: Genbank-subsumed GSDB accession numbers
   are defined to be Genbank's as well as GSDB DIRSUB records.
*/
#define ACCN_IS_GENBANK(c) ((((c)&65535) == ACCN_NCBI_GSDB) ||  (((c)&65535)==ACCN_GSDB_DIRSUB) || (((c)&65535) == ACCN_NCBI_EST) ||  (((c)&65535) == ACCN_NCBI_DIRSUB) ||  (((c)&65535) == ACCN_NCBI_GENOME) ||  (((c)&65535) == ACCN_NCBI_PATENT) ||  (((c)&65535) == ACCN_NCBI_HTGS) ||  (((c)&65535) == ACCN_NCBI_GSS) ||  (((c)&65535) == ACCN_NCBI_STS) ||  (((c)&65535) == ACCN_NCBI_BACKBONE) ||  (((c)&65535) == ACCN_NCBI_SEGSET) ||  (((c)&65535) == ACCN_NCBI_OTHER)  || (((c)&65535) == ACCN_NCBI_PROT) || (((c)&65535) == ACCN_NCBI_cDNA) || (((c)&65535) == ACCN_EMBL_GB) || (((c)&65535) == ACCN_EMBL_GB_DDBJ || (((c)&65535) == ACCN_GB_DDBJ)) )

/* XM_,NP_,NM_,NT_,NC_ reference sequence records created and curated by NCBI 
   REFSEQ project
*/
#define ACCN_IS_REFSEQ(c) (((c)== ACCN_REFSEQ_PROT) || ((c)== ACCN_REFSEQ_mRNA) || ((c)== ACCN_REFSEQ_CONTIG) || ((c)== ACCN_REFSEQ_CHROMOSOME) || ((c)== ACCN_REFSEQ_mRNA_PREDICTED) || ((c)== ACCN_REFSEQ_PROT_PREDICTED) || ((c)== ACCN_REFSEQ_GENOMIC) || (((c)&65535)== ACCN_REFSEQ) )

#define ACCN_IS_TPA(c) (((c)== ACCN_NCBI_TPA) || ((c)== ACCN_NCBI_TPA_PROT) || ((c)== ACCN_EMBL_TPA) || ((c)== ACCN_EMBL_TPA_PROT) || ((c)== ACCN_DDBJ_TPA) || ((c)== ACCN_DDBJ_TPA_PROT))

#define ACCN_IS_NCBI(c) (ACCN_IS_REFSEQ((c)) || ACCN_IS_GENBANK((c)) || ((c)== ACCN_NCBI_TPA) || ((c)== ACCN_NCBI_TPA_PROT))

/*
  Macro to detect EMBL accession numbers  (can also belong to another DB)
 */
#define ACCN_IS_EMBL(c) ( (((c)&65535) ==  ACCN_EMBL_EST) ||  (((c)&65535) == ACCN_EMBL_DIRSUB) ||  (((c)&65535) == ACCN_EMBL_GENOME) ||  (((c)&65535) == ACCN_EMBL_PATENT) ||  (((c)&65535) == ACCN_EMBL_HTGS) ||  (((c)&65535) == ACCN_EMBL_CON) ||  (((c)&65535) == ACCN_EMBL_OTHER)  || (((c)&65535) == ACCN_EMBL_PROT) || (((c)&65535) == ACCN_EMBL_GB) || (((c)&65535) == ACCN_EMBL_DDBJ) || (((c)&65535) == ACCN_EMBL_GB_DDBJ))

#define ACCN_IS_DDBJ(c) ((((c)&65535) ==  ACCN_DDBJ_EST) ||  (((c)&65535) == ACCN_DDBJ_DIRSUB) ||  (((c)&65535) == ACCN_DDBJ_GENOME) ||  (((c)&65535) == ACCN_DDBJ_PATENT) ||  (((c)&65535) == ACCN_DDBJ_HTGS) ||  (((c)&65535) == ACCN_DDBJ_CON) ||  (((c)&65535) == ACCN_DDBJ_OTHER) || (((c)&65535) == ACCN_DDBJ_PROT) || (((c)&65535) == ACCN_GB_DDBJ) || (((c)&65535) == ACCN_EMBL_DDBJ) || (((c)&65535) == ACCN_EMBL_GB_DDBJ))

#define ACCN_IS_SWISSPROT(c) ((c)== ACCN_SWISSPROT)
/* 
   detect the few accessions numbers (N000*-N1*) have been assigned to many databases 
   .. as well as unnasigned accessions.
*/
#define ACCN_IS_AMBIGOUSDB(c) (((c)&65535)==ACCN_AMBIGOUS_DB || (c)== ACCN_IS_PROTEIN || (c)== ACCN_IS_NT || (((c)&65535) == ACCN_EMBL_GB) || (((c)&65535) == ACCN_EMBL_DDBJ) || (((c)&65535) == ACCN_GB_DDBJ) || (((c)&65535) == ACCN_EMBL_GB_DDBJ))
    /*
      does not ressemble any accession types. (with the possible exception
      of PIR.. but must call ACCN_PIR_FORMAT() to check that.
     */
#define ACCN_IS_UNKNOWN(c) (c==ACCN_UNKNOWN)
    /* Unassigned : is of 3+5 (proteins) OR
                          2+5 (amino acids) OR
                          [A-Z][A-Z]_ (refseq type)
                          , but
       has not been formally been formally assigned (hardcoded)
    */
#define ACCN_IS_UNASSIGNED(c) ((c)== ACCN_IS_PROTEIN || (c)== ACCN_IS_NT || (c) == ACCN_UNKNOWN || (c)==ACCN_REFSEQ)

/*
  Try to Find if the Bioseq represented by a SeqId is a SeqLoc List;
  May fetch the Bioseq to get all the synonymous SeqIds.
 */

NLM_EXTERN Boolean LIBCALL SeqIdInSeqLocList(SeqIdPtr sip, ValNodePtr list);

NLM_EXTERN SeqIdPtr     AddSeqId (SeqIdPtr *sip_head, SeqIdPtr sip);
NLM_EXTERN SeqIdPtr     SeqIdDupList (SeqIdPtr id_list);
NLM_EXTERN SeqIdPtr     SeqIdDupBestList (SeqIdPtr id_list);
NLM_EXTERN SeqIdPtr     SeqIdListfromSeqLoc (ValNodePtr vnpslp);


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
