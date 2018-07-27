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
* File Name: netblap3.h
*
* Author: Tom Madden
*
* Version Creation Date:   05/12/97
*
* File Description: 
*       Application Programming Interface (API) for BLAST network server
*
* RCS Modification History:
* $Log: netblap3.h,v $
* Revision 1.32  2004/06/30 12:34:14  madden
* Add include for blfmtutl.h
*
* Revision 1.31  2003/01/10 21:45:06  bealer
* - Modify to return errors from BLASTGetUidsFromQuery instead of logging them.
*
* Revision 1.30  2002/05/23 22:57:03  dondosha
* Made GetResponsePtr external
*
* Revision 1.29  2002/05/15 16:32:51  madden
* Make QueryIsProteinFromType extern
*
* Revision 1.28  2002/05/10 12:56:27  madden
* Allow network version of matrix as input
*
* Revision 1.27  2001/05/02 19:42:48  egorov
* Make the NetBlastGetMatrix() external
*
* Revision 1.26  2000/11/16 22:25:39  dondosha
* Added other_returns argument to BlastBioseq
*
* Revision 1.25  2000/09/28 16:44:31  dondosha
* Changed prototype for MegaBlastSeqLocNetCore to return SeqAlignPtr
*
* Revision 1.24  2000/08/28 16:29:19  dondosha
* Added definitions related to megablast
*
* Revision 1.23  2000/06/15 16:22:44  egorov
* Make SubmitRequest() external
*
* Revision 1.22  2000/05/04 18:09:26  shavirin
* Added new function  BLASTGetUidsFromQuery().
*
* Revision 1.21  1999/11/24 21:42:31  vakatov
* Fixed for the C++ and/or MSVC DLL compilation
*
* Revision 1.20  1999/10/27 13:00:02  madden
* Changes to return-parts
*
* Revision 1.19  1999/08/19 19:40:01  shavirin
* Returned to previous version.
*
* Revision 1.16  1999/08/17 18:59:47  shavirin
* Added definition of the function SeedBioseqNetCore.
*
* Revision 1.15  1999/04/21 20:54:47  madden
* Make BlastBioseq non-private
*
* Revision 1.14  1999/04/20 14:51:36  madden
* BlastBioseqNetCore and BlastSeqLocNetCore have return status
*
* Revision 1.13  1999/04/16 15:53:26  madden
* Add TraditionalBlastReportExtra function
*
* Revision 1.12  1999/03/19 21:47:38  madden
* Add Blast3GetDbinfo function
*
* Revision 1.11  1999/01/12 21:05:58  victorov
* server will now report an error if the ni-queue if full
*
* Revision 1.10  1998/12/14 19:38:31  egorov
* add file #defines
*
* Revision 1.9  1998/09/22 16:14:07  egorov
* Add prototype for parametersToOptions()
*
* Revision 1.8  1998/09/01 20:17:04  madden
*  Fixed uninitialzed problem in BlastNetBioseqFetchDisable, changed prototype
*
* Revision 1.7  1998/05/08 21:41:47  vakatov
* use NetProgressCallback in the function prototypes
*
* Revision 1.6  1998/05/08 20:56:23  madden
* Fix PC compile warnings, rename callback
*
* Revision 1.5  1998/04/23 14:18:42  egorov
* Add number_of_hits parameter to TraditionalBlastReportLoc
*
* Revision 1.4  1998/04/22 18:10:07  egorov
* Add support for SeqLoc to blastcl3
*
* Revision 1.3  1998/04/16 19:35:32  madden
* Added Int4Ptr arg to TraditionalBlastReport specifying the numbers of hits
*
* Revision 1.2  1997/11/25 14:43:36  madden
* Changes to allow iterative searches
*
* Revision 1.1  1997/10/08 19:27:22  madden
* Network support for gapped blast
*
*/

#ifndef _NETBLAP3_
#define _NETBLAP3_ 1

#include <ncbi.h>
#include <sequtil.h>
#include <blstspc.h>
#include <objblst3.h>
#include <ni_types.h>
#include <blastdef.h>
#include <blastpri.h>
#include <blfmtutl.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif


/* BLAST Overview options, more appropriate somewhere else? */
#define BLAST_OVERVIEW_NONE 0
#define BLAST_OVERVIEW_1 1

typedef Boolean (LIBCALLBACK *NetProgressCallback)(BlastResponsePtr response, Boolean PNTR cancel);


typedef struct _blastnet3h {
	NI_HandPtr svcp;
} BlastNet3H, PNTR BlastNet3Hptr;

typedef struct _blastnet3_block {
	BioseqPtr bsp;		/* The query. */
	Uint2 prog_type;	/* blast[npx], tblast[nx] */
	CharPtr dbname;		/* Name of database. */
	BlastParametersPtr parameters;
	BlastResponsePtr response;	/* Response from server. */
	SeqLocPtr mask;	/* Sequence to be masked */
	BlastNet3Hptr bl3hptr;	/* BlastNet 3 handler returned from BlastInit. */
	NetProgressCallback callback;
	BLAST_MatrixPtr blast_matrix;	/* Matrix to be sent to server. */
	BlastMatrixPtr net_matrix;	/* Network version of Matrix to be sent to server. */
        BioseqSetPtr bsp_set;   /* All queries if more than one */
} BlastNet3Block, PNTR BlastNet3BlockPtr;

/*
BlastNet3Hptr PNTR bl3hpp must not be NULL when this is called,
it must be saved and used to call BlastBioseq, as well as BlastFini.
BlastResponsePtr PNTR resp contains a response pointer of type BlastResponse_init.
This function is MT-safe.
*/

NLM_EXTERN Boolean LIBCALL BlastInit(CharPtr program_name, BlastNet3Hptr PNTR bl3hpp, BlastResponsePtr PNTR resp);



NLM_EXTERN SeqAlignPtr LIBCALL BlastBioseq(BlastNet3BlockPtr blnet3blkptr, ValNodePtr *error_returns, Boolean PNTR status, ValNodePtr *other_returns);

/*
The BlastNet3Hptr returned by BlastInitMt must be passed in.
*/

NLM_EXTERN Boolean LIBCALL BlastFini(BlastNet3Hptr bl3hptr);

/*
	Function to get a Bioseq for a give SeqIdPtr.  Should be used 
	for BioseqFetch function.
*/

NLM_EXTERN BioseqPtr LIBCALL BlastGetBioseq(BlastNet3BlockPtr blnet3blkptr, SeqIdPtr sip);

NLM_EXTERN BlastNet3BlockPtr LIBCALL BlastNet3BlockNew(CharPtr program, CharPtr dbname);

NLM_EXTERN BlastNet3BlockPtr LIBCALL BlastNet3BlockDestruct(BlastNet3BlockPtr blnet);

NLM_EXTERN CharPtr LIBCALL Blast3GetMotd(BlastNet3Hptr bl3hptr);

NLM_EXTERN SeqLocPtr LIBCALL BlastGetMaskedLoc(BlastNet3BlockPtr blnet3blkptr);

NLM_EXTERN Boolean LIBCALL BlastNetBioseqFetchEnable(BlastNet3Hptr bl3hp, CharPtr dbname, Boolean is_na, Boolean now);

NLM_EXTERN void LIBCALL BlastNetBioseqFetchDisable(BlastNet3Hptr bl3hp, CharPtr dbname, Boolean is_na);

NLM_EXTERN BlastParametersPtr LIBCALL BlastOptionsToParameters(BLAST_OptionsBlkPtr options);


NLM_EXTERN CharPtr LIBCALL BlastGetParameterBuffer(BlastNet3BlockPtr blnet3blkptr);

NLM_EXTERN BlastKABlkPtr LIBCALL BlastGetKaParams(BlastNet3BlockPtr blnet3blkptr, Boolean gapped);

NLM_EXTERN BlastDbinfoPtr LIBCALL BlastRequestDbInfo(BlastNet3Hptr bl3hp, CharPtr database, Boolean is_prot);

NLM_EXTERN BlastDbinfoPtr LIBCALL BlastGetDbInfo(BlastNet3BlockPtr blnet3blkptr);

NLM_EXTERN TxDfDbInfoPtr LIBCALL NetDbinfo2TxDbinfo(BlastDbinfoPtr net_dbinfo);

NLM_EXTERN Boolean LIBCALL TraditionalBlastReport(BioseqPtr bsp, BLAST_OptionsBlkPtr options, BlastNet3Hptr bl3hp, CharPtr program, CharPtr database, Boolean html, FILE *outfp, Boolean verbose, Uint4 print_options, Uint4 align_options, Int4 number_of_descriptions, Int4 number_of_alignments, Int4Ptr number_of_hits);

NLM_EXTERN Boolean LIBCALL TraditionalBlastReportLoc(SeqLocPtr slp, BLAST_OptionsBlkPtr options, BlastNet3Hptr bl3hp, CharPtr program, CharPtr database, Boolean html, FILE *outfp, Boolean verbose, Uint4 print_options, Uint4 align_options, Int4 number_of_descriptions, Int4 number_of_alignments, Int4Ptr number_of_hits);

NLM_EXTERN Boolean LIBCALL TraditionalBlastReportExtra(BioseqPtr bsp, BLAST_OptionsBlkPtr options, BlastNet3Hptr bl3hp, CharPtr program, CharPtr database, Boolean html, FILE *outfp, Boolean verbose, Uint4 print_options, Uint4 align_options, Int4 number_of_descriptions, Int4 number_of_alignments, Int4Ptr number_of_hits, Uint4 overview);

NLM_EXTERN Boolean LIBCALL TraditionalBlastReportLocExtra(SeqLocPtr slp, BLAST_OptionsBlkPtr options, BlastNet3Hptr bl3hp, CharPtr program, CharPtr database, Boolean html, FILE *outfp, Boolean verbose, Uint4 print_options, Uint4 align_options, Int4 number_of_descriptions, Int4 number_of_alignments, Int4Ptr number_of_hits, Uint4 overview);

NLM_EXTERN SeqAlignPtr LIBCALL BlastBioseqNet(BlastNet3Hptr bl3hp, BioseqPtr bsp, CharPtr program, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, NetProgressCallback callback);

NLM_EXTERN SeqAlignPtr LIBCALL BlastSeqLocNet(BlastNet3Hptr bl3hp, SeqLocPtr slp, CharPtr program, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, NetProgressCallback callback);

NLM_EXTERN SeqAlignPtr LIBCALL BlastBioseqNetCore(BlastNet3Hptr bl3hp, BioseqPtr bsp, CharPtr program, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, NetProgressCallback callback, BLAST_MatrixPtr blast_matrix, Boolean PNTR status);

NLM_EXTERN SeqAlignPtr LIBCALL BlastSeqLocNetCore(BlastNet3Hptr bl3hp, SeqLocPtr slp, CharPtr program, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, NetProgressCallback callback, BLAST_MatrixPtr blast_matrix, Boolean PNTR status);
NLM_EXTERN SeqAlignPtr LIBCALL MegaBlastSeqLocNetCore(BlastNet3Hptr bl3hp, SeqLocPtr slp, CharPtr program, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, NetProgressCallback callback, Boolean PNTR status);

NLM_EXTERN BlastPhialignPtr LIBCALL SeedBioseqNetCore(BlastNet3Hptr bl3hp, BioseqPtr bsp, CharPtr program, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, NetProgressCallback callback, BLAST_MatrixPtr blast_matrix, Boolean PNTR ret_status, ValNodePtr PNTR vnp);

NLM_EXTERN BlastPartsPtr LIBCALL BlastBioseqNetReturnParts(BlastNet3Hptr bl3hp, BioseqPtr bsp, CharPtr program, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, NetProgressCallback callback, BLAST_MatrixPtr blast_matrix, Boolean PNTR ret_status);

NLM_EXTERN BLAST_MatrixPtr LIBCALL BlastNetMatrixToBlastMatrix(BlastMatrixPtr net_matrix);

NLM_EXTERN BlastMatrixPtr LIBCALL BlastMatrixToBlastNetMatrix(BLAST_MatrixPtr matrix);

NLM_EXTERN BLAST_OptionsBlkPtr parametersToOptions (BlastParametersPtr parameters, CharPtr program,
	ValNodePtr PNTR error_returns);

NLM_EXTERN BlastDbinfoPtr LIBCALL Blast3GetDbinfo(BlastNet3Hptr bl3hptr);

NLM_EXTERN Int4 BLASTGetUidsFromQuery(CharPtr       query, 
				      Int4Ptr PNTR  uids, 
                                      Boolean       is_na, 
				      Boolean       count_only,
				      BlastError ** err_ret);

NLM_EXTERN Boolean SubmitRequest PROTO((BlastNet3Hptr bl3hptr, BlastRequestPtr blreqp, BlastResponsePtr PNTR response, NetProgressCallback callback, Boolean reestablish));


NLM_EXTERN BlastMatrixPtr LIBCALL NetBlastGetMatrix(BlastNet3BlockPtr blnet3blkptr);

BlastResponsePtr GetResponsePtr(BlastResponsePtr response, Nlm_Uint1 choice);

/* Return TRUE if the query for this program is protein (e.g., blastp). */
NLM_EXTERN Boolean LIBCALL QueryIsProteinFromType(Uint2 type);

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* NETBLAP3 */
