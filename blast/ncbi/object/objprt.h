/*  objprt.h
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
* File Name:  objprt.h
*
* Author:  James Ostell
*   
* Version Creation Date: 4/1/94
*
* $Revision: 6.0 $
*
* File Description:  Object manager interface for print templates
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: objprt.h,v $
* Revision 6.0  1997/08/25 18:50:24  madden
* Revision changed to 6.0
*
* Revision 4.2  1997/06/19 18:41:43  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 4.1  1995/10/30 18:38:13  kans
* changed true and false fields to truepfb and falsepfb (CodeWarrior error)
*
 * Revision 4.0  1995/07/26  13:48:06  ostell
 * force revision to 4.0
 *
 * Revision 3.1  1995/05/15  21:22:00  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

#ifndef _NCBI_ObjPrt_
#define _NCBI_ObjPrt_

#ifndef _ASNTOOL_
#include <asn.h>
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

/*****************************************************************************
*
*   loader
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjPrtAsnLoad PROTO((void));


/**************************************************
*
*    PrintForm
*
**************************************************/
typedef ValNode  PrintForm;
typedef ValNodePtr PrintFormPtr;
#define PrintForm_block 1 	/*  	PrintFormBlock  	*/
#define PrintForm_boolean 2 	/*  	PrintFormBoolean  	*/
#define PrintForm_enum 3 	/*  	PrintFormEnum  	*/
#define PrintForm_text 4 	/*  	PrintFormText  	*/
#define PrintForm_use_template 5 	/*  	TemplateName  	*/
#define PrintForm_user 6 	/*  	UserFormat  	*/
#define PrintForm_null 7 	/*  	NULL  	*/


NLM_EXTERN PrintFormPtr LIBCALL PrintFormFree PROTO ((PrintFormPtr ));
NLM_EXTERN PrintFormPtr LIBCALL PrintFormAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL PrintFormAsnWrite PROTO (( PrintFormPtr , AsnIoPtr, AsnTypePtr));

/**************************************************
*
*    PrintFormat
*
**************************************************/
typedef struct struct_PrintFormat {
	 struct struct_PrintFormat PNTR next;
	CharPtr   asn1;
	CharPtr   label;
	CharPtr   prefix;
	CharPtr   suffix;
	PrintFormPtr   form;
} PrintFormat, PNTR PrintFormatPtr;


NLM_EXTERN PrintFormatPtr LIBCALL PrintFormatFree PROTO ((PrintFormatPtr ));
NLM_EXTERN PrintFormatPtr LIBCALL PrintFormatNew PROTO (( void ));
NLM_EXTERN PrintFormatPtr LIBCALL PrintFormatAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL PrintFormatAsnWrite PROTO (( PrintFormatPtr , AsnIoPtr, AsnTypePtr));

/**************************************************
*
*    PrintTemplate
*
**************************************************/
typedef struct struct_PrintTemplate {
	CharPtr   name;
	CharPtr   labelfrom;
	PrintFormatPtr   format;
} PrintTemplate, PNTR PrintTemplatePtr;


NLM_EXTERN PrintTemplatePtr LIBCALL PrintTemplateFree PROTO ((PrintTemplatePtr ));
NLM_EXTERN PrintTemplatePtr LIBCALL PrintTemplateNew PROTO (( void ));
NLM_EXTERN PrintTemplatePtr LIBCALL PrintTemplateAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL PrintTemplateAsnWrite PROTO (( PrintTemplatePtr , AsnIoPtr, AsnTypePtr));
NLM_EXTERN PrintTemplatePtr PNTR LIBCALL PrintTemplateInMem PROTO((Int2Ptr numptr));
NLM_EXTERN void LIBCALL PrintTemplateFreeAll PROTO((void));

/**************************************************
*
*    PrintTemplateSet
*   	Just reads a set into the PTlist
*
**************************************************/
NLM_EXTERN Boolean LIBCALL PrintTemplateSetAsnRead PROTO (( AsnIoPtr ));

/**************************************************
*
*    PrintFormBlock
*
**************************************************/
typedef struct struct_PrintFormBlock {
	CharPtr   separator;
	PrintFormatPtr   components;
} PrintFormBlock, PNTR PrintFormBlockPtr;

NLM_EXTERN PrintFormBlockPtr LIBCALL PrintFormBlockFree PROTO ((PrintFormBlockPtr ));
NLM_EXTERN PrintFormBlockPtr LIBCALL PrintFormBlockNew PROTO (( void ));
NLM_EXTERN PrintFormBlockPtr LIBCALL PrintFormBlockAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL PrintFormBlockAsnWrite PROTO (( PrintFormBlockPtr , AsnIoPtr, AsnTypePtr));

/**************************************************
*
*    PrintFormBoolean
*
**************************************************/
typedef struct struct_PrintFormBoolean {
	CharPtr   truepfb;
	CharPtr   falsepfb;
} PrintFormBoolean, PNTR PrintFormBooleanPtr;

NLM_EXTERN PrintFormBooleanPtr LIBCALL PrintFormBooleanFree PROTO ((PrintFormBooleanPtr ));
NLM_EXTERN PrintFormBooleanPtr LIBCALL PrintFormBooleanNew PROTO (( void ));
NLM_EXTERN PrintFormBooleanPtr LIBCALL PrintFormBooleanAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL PrintFormBooleanAsnWrite PROTO (( PrintFormBooleanPtr , AsnIoPtr, AsnTypePtr));

/**************************************************
*
*    PrintFormEnum
*
**************************************************/
typedef struct struct_PrintFormEnum {
	ValNodePtr values;
} PrintFormEnum, PNTR PrintFormEnumPtr;

NLM_EXTERN PrintFormEnumPtr LIBCALL PrintFormEnumFree PROTO ((PrintFormEnumPtr ));
NLM_EXTERN PrintFormEnumPtr LIBCALL PrintFormEnumNew PROTO (( void ));
NLM_EXTERN PrintFormEnumPtr LIBCALL PrintFormEnumAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL PrintFormEnumAsnWrite PROTO (( PrintFormEnumPtr , AsnIoPtr, AsnTypePtr));

/**************************************************
*
*    PrintFormText
*
**************************************************/
typedef struct struct_PrintFormText {
	CharPtr   textfunc;
} PrintFormText, PNTR PrintFormTextPtr;

NLM_EXTERN PrintFormTextPtr LIBCALL PrintFormTextFree PROTO ((PrintFormTextPtr ));
NLM_EXTERN PrintFormTextPtr LIBCALL PrintFormTextNew PROTO (( void ));
NLM_EXTERN PrintFormTextPtr LIBCALL PrintFormTextAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL PrintFormTextAsnWrite PROTO (( PrintFormTextPtr , AsnIoPtr, AsnTypePtr));

/**************************************************
*
*    UserFormat
*
**************************************************/
typedef struct struct_UserFormat {
	CharPtr   printfunc;
	CharPtr   defaultfunc;
} UserFormat, PNTR UserFormatPtr;

NLM_EXTERN UserFormatPtr LIBCALL UserFormatFree PROTO ((UserFormatPtr ));
NLM_EXTERN UserFormatPtr LIBCALL UserFormatNew PROTO (( void ));
NLM_EXTERN UserFormatPtr LIBCALL UserFormatAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL UserFormatAsnWrite PROTO (( UserFormatPtr , AsnIoPtr, AsnTypePtr));

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

