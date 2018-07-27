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
* File Name: suggen.h
*
* Author:  Yuri Sadykov
*
* Version Creation Date:   08/14/95
*
* $Revision: 6.0 $
*
* File Description: 
*
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

#ifndef _suggen_ 
#define _suggen_ 

#ifdef __cplusplus
extern "C" { /* } */
#endif


/**************************************************
*
*    Generated objects for Module NCBI-Suggest
*    Generated using ASNCODE Revision: 4.0 at Aug 16, 1995  2:17 PM
*
**************************************************/

Boolean LIBCALL
suggenAsnLoad PROTO((void));
typedef ValNodePtr SuggestRequestPtr;
typedef ValNode SuggestRequest;
#define SuggestRequest_init 1
#define SuggestRequest_intervals 2
#define SuggestRequest_fini 3


SuggestRequestPtr LIBCALL SuggestRequestFree PROTO ((SuggestRequestPtr ));
SuggestRequestPtr LIBCALL SuggestRequestAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL SuggestRequestAsnWrite PROTO (( SuggestRequestPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    SuggestIntervals
*
**************************************************/
#ifndef _SUGGEST_
typedef struct struct_Suggest_intervals {
   Uint4 OBbits__;
   struct struct_Suggest_parameters PNTR   params;
   ValNodePtr   dna;
   ValNodePtr   protein;
   Int4   code;
} SuggestIntervals, PNTR SuggestIntervalsPtr;
#endif


SuggestIntervalsPtr LIBCALL SuggestIntervalsFree PROTO ((SuggestIntervalsPtr ));
SuggestIntervalsPtr LIBCALL SuggestIntervalsNew PROTO (( void ));
SuggestIntervalsPtr LIBCALL SuggestIntervalsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL SuggestIntervalsAsnWrite PROTO (( SuggestIntervalsPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    SuggestParameters
*
**************************************************/
#ifndef _SUGGEST_
typedef struct struct_Suggest_parameters {
   Uint4 OBbits__;
#define OB__Suggest_parameters_size 0

   Int4   size;
#define OB__Suggest_parameters_begin_search 1

   Int4   begin_search;
#define OB__Suggest_parameters_end_search 2

   Int4   end_search;
#define OB__Suggest_parameters_term_stop 3

   Uint1   term_stop;
} SuggestParameters, PNTR SuggestParametersPtr;
#endif


SuggestParametersPtr LIBCALL SuggestParametersFree PROTO ((SuggestParametersPtr ));
SuggestParametersPtr LIBCALL SuggestParametersNew PROTO (( void ));
SuggestParametersPtr LIBCALL SuggestParametersAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL SuggestParametersAsnWrite PROTO (( SuggestParametersPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    SuggestError
*
**************************************************/
#ifndef _SUGGEST_
typedef struct struct_Suggest_error {
   Uint4 OBbits__;
   Uint2   level;
   /* following #defines are for enumerated type, not used by object loaders */
#define Suggest_error_level_none 0
#define Suggest_error_level_info 1
#define Suggest_error_level_warn 2
#define Suggest_error_level_error 3
#define Suggest_error_level_fatal 4

   CharPtr   msg;
} SuggestError, PNTR SuggestErrorPtr;
#endif


SuggestErrorPtr LIBCALL SuggestErrorFree PROTO ((SuggestErrorPtr ));
SuggestErrorPtr LIBCALL SuggestErrorNew PROTO (( void ));
SuggestErrorPtr LIBCALL SuggestErrorAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL SuggestErrorAsnWrite PROTO (( SuggestErrorPtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr SuggestResponsePtr;
typedef ValNode SuggestResponse;
#define SuggestResponse_init 1
#define SuggestResponse_error 2
#define SuggestResponse_intervals 3
#define SuggestResponse_fini 4


SuggestResponsePtr LIBCALL SuggestResponseFree PROTO ((SuggestResponsePtr ));
SuggestResponsePtr LIBCALL SuggestResponseAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL SuggestResponseAsnWrite PROTO (( SuggestResponsePtr , AsnIoPtr, AsnTypePtr));

#ifdef __cplusplus
/* { */ }
#endif

#endif /* _suggen_ */
