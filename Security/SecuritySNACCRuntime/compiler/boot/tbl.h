/*
 * Copyright (c) 2000-2001 Apple Computer, Inc. All Rights Reserved.
 * 
 * The contents of this file constitute Original Code as defined in and are
 * subject to the Apple Public Source License Version 1.2 (the 'License').
 * You may not use this file except in compliance with the License. Please obtain
 * a copy of the License at http://www.apple.com/publicsource and read it before
 * using this file.
 * 
 * This Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS
 * OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT. Please see the License for the
 * specific language governing rights and limitations under the License.
 */


/*
 *    tbl.h
 *
 *    "TBL" ASN.1 module C type definitions and prototypes
 *
 *    This .h file was generated by snacc on Mon Jun  2 11:23:51 1997
 *
 *    UBC snacc written compiler by Mike Sample
 *
 *    NOTE: This is a machine generated file--editing not recommended
 */


#ifndef _tbl_h_
#define _tbl_h_




typedef enum
    {
        TBL_BOOLEAN = 0,
        TBL_INTEGER = 1,
        TBL_BITSTRING = 2,
        TBL_OCTETSTRING = 3,
        TBL_NULL = 4,
        TBL_OID = 5,
        TBL_REAL = 6,
        TBL_ENUMERATED = 7,
        TBL_SEQUENCE = 8,
        TBL_SET = 9,
        TBL_SEQUENCEOF = 10,
        TBL_SETOF = 11,
        TBL_CHOICE = 12,
        TBL_TYPEREF = 13
    } TBLTypeId; /* ENUMERATED { TBL_BOOLEAN (0), TBL_INTEGER (1), TBL_BITSTRING (2), TBL_OCTETSTRING (3), TBL_NULL (4), TBL_OID (5), TBL_REAL (6), TBL_ENUMERATED (7), TBL_SEQUENCE (8), TBL_SET (9), TBL_SEQUENCEOF (10), TBL_SETOF (11), TBL_CHOICE (12), TBL_TYPEREF (13) }  */

#define BEncTBLTypeIdContent BEncAsnEnumContent




typedef AsnInt TBLTypeDefId; /* INTEGER */

#define BEncTBLTypeDefIdContent BEncAsnIntContent




typedef enum
    {
        UNIVERSAL = 0,
        APPLICATION = 1,
        CONTEXT = 2,
        PRIVATE = 3
    } TBLTagClass; /* ENUMERATED { UNIVERSAL (0), APPLICATION (1), CONTEXT (2), PRIVATE (3) }  */

#define BEncTBLTagClassContent BEncAsnEnumContent




typedef struct TBLRange /* SEQUENCE */
{
    AsnInt from; /* [0] IMPLICIT INTEGER */
    AsnInt to; /* [1] IMPLICIT INTEGER */
} TBLRange;

AsnLen BEncTBLRangeContent PROTO ((BUF_TYPE b, TBLRange *v));




typedef struct TBLNamedNumber /* SEQUENCE */
{
    PrintableString name; /* [0] IMPLICIT PrintableString */
    AsnInt value; /* [1] IMPLICIT INTEGER */
} TBLNamedNumber;

AsnLen BEncTBLNamedNumberContent PROTO ((BUF_TYPE b, TBLNamedNumber *v));




typedef AsnList TBLNamedNumberList; /* SEQUENCE OF TBLNamedNumber */

AsnLen BEncTBLNamedNumberListContent PROTO ((BUF_TYPE b, TBLNamedNumberList *v));




typedef struct TBLTypeRef /* SEQUENCE */
{
    TBLTypeDefId typeDef; /* TBLTypeDefId */
    AsnBool implicit; /* BOOLEAN */
} TBLTypeRef;

AsnLen BEncTBLTypeRefContent PROTO ((BUF_TYPE b, TBLTypeRef *v));




typedef struct TBLTag /* SEQUENCE */
{
    TBLTagClass tclass; /* TBLTagClass */
    AsnInt code; /* INTEGER (0..MAX) */
} TBLTag;

AsnLen BEncTBLTagContent PROTO ((BUF_TYPE b, TBLTag *v));




typedef AsnList TBLTypeSeqOf; /* SEQUENCE OF TBLTag */

AsnLen BEncTBLTypeSeqOfContent PROTO ((BUF_TYPE b, TBLTypeSeqOf *v));




typedef AsnList TBLTypeContentSeqOf; /* SEQUENCE OF TBLType */

AsnLen BEncTBLTypeContentSeqOfContent PROTO ((BUF_TYPE b, TBLTypeContentSeqOf *v));




typedef struct TBLType /* SEQUENCE */
{
    TBLTypeId typeId; /* [0] IMPLICIT TBLTypeId */
    AsnBool optional; /* [1] IMPLICIT BOOLEAN */
    TBLTypeSeqOf* tagList; /* [2] IMPLICIT TBLTypeSeqOf OPTIONAL */
    struct TBLTypeContent* content; /* [3] TBLTypeContent */
    PrintableString fieldName; /* [4] IMPLICIT PrintableString OPTIONAL */
    struct TBLRange* constraint; /* [5] IMPLICIT TBLRange OPTIONAL */
    TBLNamedNumberList* values; /* [6] IMPLICIT TBLNamedNumberList OPTIONAL */
} TBLType;

AsnLen BEncTBLTypeContent PROTO ((BUF_TYPE b, TBLType *v));




typedef struct TBLTypeContent /* CHOICE */
{
    enum TBLTypeContentChoiceId
    {
        TBLTYPECONTENT_PRIMTYPE,
        TBLTYPECONTENT_ELMTS,
        TBLTYPECONTENT_TYPEREF
    } choiceId;
    union TBLTypeContentChoiceUnion
    {
    AsnNull primType; /* [0] IMPLICIT NULL */
    TBLTypeContentSeqOf* elmts; /* [1] IMPLICIT TBLTypeContentSeqOf */
    struct TBLTypeRef* typeRef; /* [2] IMPLICIT TBLTypeRef */
    } a;
} TBLTypeContent;

AsnLen BEncTBLTypeContentContent PROTO ((BUF_TYPE b, TBLTypeContent *v));




typedef struct TBLTypeDef /* SEQUENCE */
{
    TBLTypeDefId typeDefId; /* TBLTypeDefId */
    PrintableString typeName; /* PrintableString */
    struct TBLType* type; /* TBLType */
    AsnNull* isPdu; /* NULL OPTIONAL */
} TBLTypeDef;

AsnLen BEncTBLTypeDefContent PROTO ((BUF_TYPE b, TBLTypeDef *v));




typedef AsnList TBLModuleSeqOf; /* SEQUENCE OF TBLTypeDef */

AsnLen BEncTBLModuleSeqOfContent PROTO ((BUF_TYPE b, TBLModuleSeqOf *v));




typedef struct TBLModule /* SEQUENCE */
{
    PrintableString name; /* [0] IMPLICIT PrintableString */
    AsnOid id; /* [1] IMPLICIT OBJECT IDENTIFIER OPTIONAL */
    AsnBool isUseful; /* [2] IMPLICIT BOOLEAN */
    TBLModuleSeqOf* typeDefs; /* [3] IMPLICIT TBLModuleSeqOf */
} TBLModule;

AsnLen BEncTBLModuleContent PROTO ((BUF_TYPE b, TBLModule *v));




typedef AsnList TBLSeqOf; /* SEQUENCE OF TBLModule */

AsnLen BEncTBLSeqOfContent PROTO ((BUF_TYPE b, TBLSeqOf *v));




typedef struct TBL /* SEQUENCE */
{
    AsnInt totalNumModules; /* INTEGER */
    AsnInt totalNumTypeDefs; /* INTEGER */
    AsnInt totalNumTypes; /* INTEGER */
    AsnInt totalNumTags; /* INTEGER */
    AsnInt totalNumStrings; /* INTEGER */
    AsnInt totalLenStrings; /* INTEGER */
    TBLSeqOf* modules; /* TBLSeqOf */
} TBL;

AsnLen BEncTBL PROTO ((BUF_TYPE b, TBL *v));

AsnLen BEncTBLContent PROTO ((BUF_TYPE b, TBL *v));





#endif /* conditional include of tbl.h */
