/* asnprint.h
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
* File Name: asnprint.h
*
* Author:  James Ostell
*
* Version Creation Date: 1/1/91
*
* $Revision: 6.5 $
*
* File Description:
*   typedefs and prototypes used internally by asnprint.c
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: asnprint.h,v $
* Revision 6.5  2003/09/15 16:16:32  kans
* added AsnWriteEx, AsnTxtWriteEx, and AsnPrintStream
*
* Revision 6.4  2001/06/26 22:02:42  juran
* Make C++-compatible.
*
* Revision 6.3  2000/12/12 15:56:14  ostell
* added support BigInt
*
* Revision 6.2  2000/07/25 20:30:58  ostell
* added support for printing multiple ASN.1 modules as multiple XML DTD and .mod files
*
* Revision 6.1  2000/05/10 03:12:36  ostell
* added support for XML DTD and XML data output
*
* Revision 6.0  1997/08/25 18:10:20  madden
* Revision changed to 6.0
*
* Revision 5.1  1996/12/03 21:43:48  vakatov
* Adopted for 32-bit MS-Windows DLLs
*
 * Revision 5.0  1996/05/28  14:00:29  ostell
 * Set to revision 5.0
 *
 * Revision 4.0  1995/07/26  13:47:38  ostell
 * force revision to 4.0
 *
 * Revision 2.3  1995/05/15  18:38:28  ostell
 * added Log line
 *
*
* ==========================================================================
*/

#ifndef _ASNPRINT_
#define _ASNPRINT_

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
*   prototypes
*   	AsnTxtWrite prototyped in asngen.h
*
*****************************************************************************/
NLM_EXTERN void AsnPrintModule PROTO((AsnModulePtr amp, AsnIoPtr aip));
NLM_EXTERN void AsnPrintModuleXML (AsnModulePtr amp, AsnIoPtr aip);
NLM_EXTERN void AsnPrintModuleXMLInc (AsnModulePtr amp, CharPtr name);
NLM_EXTERN void AsnPrintTreeModule(AsnModulePtr amp, FILE * fp);
NLM_EXTERN void AsnPrintType PROTO((AsnTypePtr atp, AsnIoPtr aip));
NLM_EXTERN Boolean AsnPrintStrStore PROTO((ByteStorePtr bsp, AsnIoPtr aip));
NLM_EXTERN void AsnPrintReal PROTO((FloatHi realvalue, AsnIoPtr aip));
NLM_EXTERN void AsnPrintInteger PROTO((Int4 theInt, AsnIoPtr aip));
NLM_EXTERN void AsnPrintBigInt PROTO((Int8 theInt, AsnIoPtr aip));
NLM_EXTERN void AsnPrintBoolean PROTO((Boolean value, AsnIoPtr aip));
NLM_EXTERN void AsnPrintOctets PROTO((ByteStorePtr ssp, AsnIoPtr aip));
NLM_EXTERN void AsnPrintChar PROTO((char theChar, AsnIoPtr aip));
NLM_EXTERN void AsnPrintIndent PROTO((Boolean increase, AsnIoPtr aip));
NLM_EXTERN void AsnPrintNewLine PROTO((AsnIoPtr aip));
NLM_EXTERN Boolean AsnPrintString PROTO((CharPtr str, AsnIoPtr aip));
NLM_EXTERN Boolean AsnPrintStream (Pointer object, AsnIoPtr aip, AsnStreamStringFunc stream);
NLM_EXTERN void AsnPrintCharBlock PROTO((CharPtr str, AsnIoPtr aip));
NLM_EXTERN int AsnPrintGetWordBreak PROTO((CharPtr str, int maxlen));
NLM_EXTERN void AsnPrintOpenStruct PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN void AsnPrintCloseStruct PROTO((AsnIoPtr aip, AsnTypePtr atp));

#ifdef __cplusplus
}
#endif


#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN
#endif

#endif
