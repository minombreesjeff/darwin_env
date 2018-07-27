#ifndef _objproj_ 
#define _objproj_ 

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
*    Generated objects for Module NCBI-Project
*    Generated using ASNCODE Revision: 6.1 at Mar 25, 1998  6:32 PM
*
**************************************************/

NLM_EXTERN Boolean LIBCALL
objprojAsnLoad PROTO((void));


/**************************************************
*
*    Project
*
**************************************************/
typedef struct struct_Project {
   struct struct_Project PNTR next;
   struct struct_Project_descr PNTR   descr;
   ValNodePtr   data;
} Project, PNTR ProjectPtr;


NLM_EXTERN ProjectPtr LIBCALL ProjectFree PROTO ((ProjectPtr ));
NLM_EXTERN ProjectPtr LIBCALL ProjectNew PROTO (( void ));
NLM_EXTERN ProjectPtr LIBCALL ProjectAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ProjectAsnWrite PROTO (( ProjectPtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr ProjectItemPtr;
typedef ValNode ProjectItem;
#define ProjectItem_pmuid 1
#define ProjectItem_protuid 2
#define ProjectItem_nucuid 3
#define ProjectItem_sequid 4
#define ProjectItem_genomeuid 5
#define ProjectItem_structuid 6
#define ProjectItem_pmid 7
#define ProjectItem_protid 8
#define ProjectItem_nucid 9
#define ProjectItem_seqid 10
#define ProjectItem_genomeid 11
#define ProjectItem_structid 12
#define ProjectItem_pment 13
#define ProjectItem_protent 14
#define ProjectItem_nucent 15
#define ProjectItem_seqent 16
#define ProjectItem_genomeent 17
#define ProjectItem_structent 18
#define ProjectItem_seqannot 19
#define ProjectItem_loc 20
#define ProjectItem_proj 21


NLM_EXTERN ProjectItemPtr LIBCALL ProjectItemFree PROTO ((ProjectItemPtr ));
NLM_EXTERN ProjectItemPtr LIBCALL ProjectItemAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ProjectItemAsnWrite PROTO (( ProjectItemPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ProjectDescr
*
**************************************************/
typedef struct struct_Project_descr {
   ValNodePtr   id;
   CharPtr   name;
   ValNodePtr   descr;
} ProjectDescr, PNTR ProjectDescrPtr;


NLM_EXTERN ProjectDescrPtr LIBCALL ProjectDescrFree PROTO ((ProjectDescrPtr ));
NLM_EXTERN ProjectDescrPtr LIBCALL ProjectDescrNew PROTO (( void ));
NLM_EXTERN ProjectDescrPtr LIBCALL ProjectDescrAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ProjectDescrAsnWrite PROTO (( ProjectDescrPtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr ProjdescPtr;
typedef ValNode Projdesc;
#define Projdesc_pub 1
#define Projdesc_date 2
#define Projdesc_comment 3
#define Projdesc_title 4


NLM_EXTERN ProjdescPtr LIBCALL ProjdescFree PROTO ((ProjdescPtr ));
NLM_EXTERN ProjdescPtr LIBCALL ProjdescAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ProjdescAsnWrite PROTO (( ProjdescPtr , AsnIoPtr, AsnTypePtr));

#ifdef __cplusplus
/* { */ }
#endif

#endif /* _objproj_ */

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

