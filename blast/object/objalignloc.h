#ifndef _objalignloc_ 
#define _objalignloc_ 

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif


#ifdef __cplusplus
extern "C" { /* } */
#endif


/**************************************************
*
*    Generated objects for Module NCBI-Alignloc
*    Generated using ASNCODE Revision: 6.8 at Aug 13, 1999  2:44 PM
*
**************************************************/

NLM_EXTERN Boolean LIBCALL
objalignlocAsnLoad PROTO((void));


/**************************************************
*
*    AlignLocSet
*
**************************************************/
typedef struct struct_AlignLoc AlignLocSet;
typedef struct struct_AlignLoc PNTR AlignLocSetPtr;
#define AlignLocSetNew() AlignLocNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN AlignLocSetPtr LIBCALL AlignLocSetFree PROTO ((AlignLocSetPtr ));
NLM_EXTERN AlignLocSetPtr LIBCALL AlignLocSetNew PROTO (( void ));
NLM_EXTERN AlignLocSetPtr LIBCALL AlignLocSetAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL AlignLocSetAsnWrite PROTO (( AlignLocSetPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    AlignLoc
*
**************************************************/
typedef struct struct_Align_loc {
   struct struct_Align_loc PNTR next;
   ValNodePtr   id;
   ValNodePtr   Loc_loc;
} AlignLoc, PNTR AlignLocPtr;


NLM_EXTERN AlignLocPtr LIBCALL AlignLocFree PROTO ((AlignLocPtr ));
NLM_EXTERN AlignLocPtr LIBCALL AlignLocNew PROTO (( void ));
NLM_EXTERN AlignLocPtr LIBCALL AlignLocAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL AlignLocAsnWrite PROTO (( AlignLocPtr , AsnIoPtr, AsnTypePtr));


#ifdef NLM_GENERATED_CODE_PROTO

typedef ValNodePtr Loc_locPtr;
typedef ValNode Loc_loc;

#endif /* NLM_GENERATED_CODE_PROTO */

#define Loc_loc_whole 1
#define Loc_loc_box 2
#define Loc_loc_rows 3
#define Loc_loc_columns 4

#ifdef NLM_GENERATED_CODE_PROTO

static Loc_locPtr LIBCALL Loc_locFree PROTO ((Loc_locPtr ));
static Loc_locPtr LIBCALL Loc_locAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
static Boolean LIBCALL Loc_locAsnWrite PROTO (( Loc_locPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */

typedef ValNodePtr AlignIdPtr;
typedef ValNode AlignId;
#define AlignId_id 1
#define AlignId_itemid 2
#define AlignId_gi 3


NLM_EXTERN AlignIdPtr LIBCALL AlignIdFree PROTO ((AlignIdPtr ));
NLM_EXTERN AlignIdPtr LIBCALL AlignIdAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL AlignIdAsnWrite PROTO (( AlignIdPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    AlignBox
*
**************************************************/
typedef struct struct_Align_box {
   Int4   row_from;
   Int4   row_to;
   Int4   column_from;
   Int4   column_to;
} AlignBox, PNTR AlignBoxPtr;


NLM_EXTERN AlignBoxPtr LIBCALL AlignBoxFree PROTO ((AlignBoxPtr ));
NLM_EXTERN AlignBoxPtr LIBCALL AlignBoxNew PROTO (( void ));
NLM_EXTERN AlignBoxPtr LIBCALL AlignBoxAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL AlignBoxAsnWrite PROTO (( AlignBoxPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    AlignRows
*
**************************************************/
typedef struct struct_Align_rows {
   Int4   from;
   Int4   to;
} AlignRows, PNTR AlignRowsPtr;


NLM_EXTERN AlignRowsPtr LIBCALL AlignRowsFree PROTO ((AlignRowsPtr ));
NLM_EXTERN AlignRowsPtr LIBCALL AlignRowsNew PROTO (( void ));
NLM_EXTERN AlignRowsPtr LIBCALL AlignRowsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL AlignRowsAsnWrite PROTO (( AlignRowsPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    AlignColumns
*
**************************************************/
typedef struct struct_Align_columns {
   Int4   from;
   Int4   to;
} AlignColumns, PNTR AlignColumnsPtr;


NLM_EXTERN AlignColumnsPtr LIBCALL AlignColumnsFree PROTO ((AlignColumnsPtr ));
NLM_EXTERN AlignColumnsPtr LIBCALL AlignColumnsNew PROTO (( void ));
NLM_EXTERN AlignColumnsPtr LIBCALL AlignColumnsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL AlignColumnsAsnWrite PROTO (( AlignColumnsPtr , AsnIoPtr, AsnTypePtr));

#ifdef __cplusplus
/* { */ }
#endif

#endif /* _objalignloc_ */

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

