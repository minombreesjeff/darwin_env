/* asn.h
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
* File Name: asn.h
*
* Author:  James Ostell
*
* Version Creation Date: 1/1/91
*
* $Revision: 6.11 $
*
* File Description:
*   This header the interface to all the routines in the ASN.1 libraries
*     that an application should ever use.  It also includes the necessary
*     typedefs -- however the application programmer is not meant to use
*     the internal structures directly outside of the specified functions,
*     as the internal structures may be changed without notice.
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 04-20-93 Schuler     LIBCALL calling convention
* 05-13-93 Schuler     Changed definitions of AsnReadFunc and AsnWriteFunc to
*                      use the LIBCALL calling convention.
* 02-24-94 Schuler     AsnTypeStringToHex moved here (from asntypes.h)
*
* $Log: asn.h,v $
* Revision 6.11  2003/12/03 19:31:09  gouriano
* Corrected DTD generation (a different approach)
*
* Revision 6.10  2003/09/15 16:16:32  kans
* added AsnWriteEx, AsnTxtWriteEx, and AsnPrintStream
*
* Revision 6.9  2002/05/20 23:13:39  ivanov
* Fixed overburn memory AsnIo buf in the AsnPrint*() -- increased
* buffers reserved room
*
* Revision 6.8  2001/10/11 14:39:08  ostell
* added support for XMLModulePrefix
*
* Revision 6.7  2000/12/12 15:56:08  ostell
* added support BigInt
*
* Revision 6.6  2000/07/25 20:30:59  ostell
* added support for printing multiple ASN.1 modules as multiple XML DTD and .mod files
*
* Revision 6.5  2000/05/12 20:44:01  ostell
* make changes to collect comments from spec and print in DTD
*
* Revision 6.4  2000/05/10 03:12:36  ostell
* added support for XML DTD and XML data output
*
* Revision 6.3  1998/02/27 17:22:18  vakatov
* [WIN32 DLL]  Declared some functions as NLM_EXTERN(DLL-exportable)
*
* Revision 6.2  1998/01/09 15:54:08  shavirin
* Added definition of hash calculating function
*
* Revision 6.1  1997/10/28 15:13:23  epstein
* add AsnFindNthPieceOfObject
*
* Revision 6.0  1997/08/25 18:09:34  madden
* Revision changed to 6.0
*
* Revision 5.3  1997/04/24 12:47:49  ostell
* in AsnTypeStringToHex, allowed argument "left" to be NULL
*
 * Revision 5.2  1997/04/23  21:23:03  ostell
 * changed BitHex reading routine to strip internal spaces and allow for linewraps at
 * any spacing.
 *
 * Revision 5.1  1996/12/03  21:43:48  vakatov
 * Adopted for 32-bit MS-Windows DLLs
 *
 * Revision 5.0  1996/05/28  14:00:29  ostell
 * Set to revision 5.0
 *
 * Revision 4.2  1996/02/18  16:45:36  ostell
 * changed fix_non_print behavior and added option 3
 *
 * Revision 4.1  1995/12/21  14:00:51  ostell
 * added AsnIoFree()
 *
 * Revision 4.0  1995/07/26  13:47:38  ostell
 * force revision to 4.0
 *
 * Revision 2.27  1995/06/29  15:58:51  epstein
 * add OP_NCBIOBJSTR for complex handling when reading biostrucs
 *
 * Revision 2.26  95/05/24  19:08:49  ostell
 * added spec_version to AsnIoPtr. This is used to set a non-zero asn.1
 * spec version number on a particular stream. The object loaders check this
 * number and remove new asn1 elements on either read or write. This allows
 * filtering of data that conforms to a not yet public spec.
 *
 * Revision 2.25  1995/05/15  18:38:28  ostell
 * added Log line
 *
*
* ==========================================================================
*/

#ifndef _ASNTOOL_
#define _ASNTOOL_

                      /*** depends on NCBI core routines ***/
#ifndef _NCBI_
#include <ncbi.h>
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

   /**** ValNode is used for internal representation of values from
	****  CHOICE, SET OF, SEQ OF and combinations for many cases.
	****  it is provided in ncbimisc for build object routines ****/

/***  The following defines can be used for backward compatibility
#define AsnValue DataVal
#define AsnNode ValNode
***/
/***  In addition, AsnValueNode was changed to AsnValxNode so it would
      not conflict with the AsnValue define above
****/

#ifndef START_STRUCT
#define START_STRUCT 	411		/* { found */
#define END_STRUCT 		412		/* } found */
#endif

   /******* AsnOptions allow customization of AsnIo and AsnType ****/

typedef Pointer (LIBCALLBACK * AsnOptFreeFunc) PROTO ((Pointer));
#define DefAsnOptionFree  Nlm_MemFree	/* default function for freeing AsnOption's */

typedef struct asnopt {
	Int2 ao_class;               /* class of option. all negative numbers res.*/
	Int2 type;                /* type within ao_class */
	DataVal data;            /* data used for setting option */
	AsnOptFreeFunc freefunc;  /* function to free data.ptrvalue */
	struct asnopt PNTR next;
} AsnOption, PNTR AsnOptionPtr;

   /******** AsnValXNode holds ENUM, Named Integer, Default values *****/

typedef struct asnvaluenode {
	Int2 valueisa;
	CharPtr name;	               /* use for strings and named int */
	Int4 intvalue;		           /* use for int and boolean */
	FloatHi realvalue;
	struct asnvaluenode PNTR next;
	AsnOptionPtr aop;              /* for comments */
}	AsnValxNode, PNTR AsnValxNodePtr;

   /******** AsnType is a node in the AsnTool parse tree *******/

typedef struct asntype {
	Int2 isa;
	CharPtr name;
	Uint1 tagclass;
	Int2 tagnumber;
	unsigned implicit   : 1;
	unsigned optional   : 1;
	unsigned hasdefault : 1;
	unsigned exported   : 1;
	unsigned imported   : 1;
	unsigned resolved   : 1;
	AsnValxNodePtr defaultvalue;   /* used for default value, range, subtypes */
	struct asntype PNTR type;
	Pointer branch;				   /* used for named ints, enum, set, sequence */
	Int2 tmp;                      /* for temporary ->type link to local tree */
	struct asntype PNTR next;
	AsnOptionPtr hints;            /* used to customize the type by application */
	CharPtr XMLname;               /* for reading/writing XML */
	Boolean been_here;             /* needed for printing DTD */
}	AsnType, PNTR AsnTypePtr;

typedef struct asnmodule {
	CharPtr modulename;
	CharPtr filename;              /* if module to be loaded from disk */
	AsnTypePtr types;
	AsnTypePtr values;
	struct asnmodule PNTR next;    /* for chain of modules */
	Int2 lasttype;                 /* for isa defined types */
	Int2 lastvalue; 		       /* for isa defined values */
}	AsnModule, PNTR AsnModulePtr;

#define ASNIO_BUFSIZE	      1024 /* default size of AsnIo.buf */
#define ASNIO_BUFSIZE_RESERVE  200 /* default size of reserved room in AsnIo.buf */

                                   /* AsnIo.type  bit[0] = text? bit[1]=binary?*/
                                   /* bit[2]=input? bit[3]=output?           */
#define ASNIO_TEXT       1
#define ASNIO_BIN        2
#define ASNIO_IN         4
#define ASNIO_OUT        8
#define ASNIO_FILE      16
#define ASNIO_CARRIER   32         /* is a pure iterator */
#define ASNIO_XML       64

#define ASNIO_TEXT_IN	21         /* AsnIo.type */
#define ASNIO_TEXT_OUT	25
#define ASNIO_BIN_IN	22
#define ASNIO_BIN_OUT	26

typedef struct pstack {
    AsnTypePtr type;               /* type at this level of stack */
    Int4 len;                      /* length of item for binary decode */
    Boolean resolved;              /* resolution of type for binary decode */
	Boolean tag_indef;             /* indefinate tag length on input? */
} Pstack, PNTR PstackPtr;

typedef struct asnexpoptstruct {
	struct asnio PNTR aip;
	AsnTypePtr atp;
	DataValPtr dvp;
	Int2 the_choice;
	Pointer the_struct;
	Pointer data;
} AsnExpOptStruct, PNTR AsnExpOptStructPtr;

typedef void (LIBCALLBACK * AsnExpOptFunc) PROTO ((AsnExpOptStructPtr));
#define NO_CHOICE_SET INT2_MIN     /* for AsnExpOptStruct.the_choice  */

typedef struct expopt {
	Int2 numtypes;
	AsnTypePtr PNTR types;         /* the type to check */
	Pointer user_data;             /* user supplied data */
	AsnExpOptFunc user_callback;   /* user supplied callback function */
	struct expopt PNTR next;
} AsnExpOpt, PNTR AsnExpOptPtr;

typedef void (LIBCALLBACK * AsnErrorFunc) PROTO((Int2, CharPtr));
#define ErrorRetType AsnErrorFunc  /* for backward compatibility */
typedef Int2 (LIBCALLBACK * AsnIoFunc) PROTO((Pointer, CharPtr, Uint2));
#define IoFuncType AsnIoFunc	   /* for backward compatibility */


typedef struct asnio {
	CharPtr linebuf;
	Int1 type;            /* type- text-in, text-out, bin-in, bin-out */
	Int2 linepos;         /* current offset in linebuf */
	FILE * fp;            /* file to write or read to */
	BytePtr buf;          /* buffer for I/O */
    Int2 bufsize;         /* sizeof this buffer */
	Int2 bytes,           /* bytes of data available in buf */
		offset;           /* current offset of processing in buf */
	Uint1 tagclass;       /* last BER tag-id-len read */
	Int2 tagnumber;
	Boolean constructed;
	Int4 length;            /* length of BER encoded data */
	Boolean tagsaved;       /* TRUE if tag info already here - stops read */
	Int4 used;              /* if tagsaved, bytes used recorded here */
	Int1 tabsize,           /* spaces per tab */
		indent_level,       /* current indent level for print output */
		linelength,         /* max line length on output */
		max_indent,         /* current maximum indent levels for first */
		state;              /* parsing state */
    BoolPtr first;          /* for first element on indented line for printing */
	Int4 linenumber;        /* for reporting errors */
	CharPtr word;           /* current word in linebuf */
	Int2 wordlen,           /* length of word in linebuf */
	     token;             /* current parsing token for word */
    PstackPtr typestack;    /* the parsing stack for input and output */
	Int1 type_indent,       /* used like indent_level and max_indent, but for */
		max_type;           /* typestack */
	ErrorRetType error_ret; /* user error return */
    Pointer iostruct;       /* non-FILE io structure */
    IoFuncType readfunc,    /* read/write functions for sockets */
          writefunc;        /*  open and close MUST be done outside AsnIo */
	Boolean read_id;        /* for checking AsnReadId AsnReadVal alternation */
	CharPtr fname;          /* name of file in use */
	AsnOptionPtr aop;       /* head of options chain */
	AsnExpOptPtr aeop;      /* exploration options chain */
	AsnExpOptStructPtr aeosp;
	Boolean io_failure;     /* set on failed write, or read  */
	Uint1 fix_non_print;    /* fix non-printing chars in VisibleStrings (see below)*/
	Boolean scan_for_start; /* if TRUE, scan over garbage in print form */
	Int2 spec_version;      /* used for filtering between asn.1 spec versions */
	Boolean no_newline;     /* to suppress internal newlines in long XML strings */
	Boolean XMLModuleWritten; /* to put header on first XML DTD only */
} AsnIo, PNTR AsnIoPtr;


/*****************************************************************************
*
*   fix_non_print
*   	Non-printing chars (like \t, \n) are illegal in VisibleStrings. The
*   default case is to replace it with '#' and post an error message. Whether
*   the error will be fatal or not depends on ERR_SET_OPTS. Supported values
*   of fix_non_print are:
*
*   0 = (default) replace with '#', post an error of SEV_ERROR,
*             do not set aip->io_failure
*   1 = replace with '#' silently
*   2 = pass non-printing char through unchanged with no error message
*   3 = replace with '#', post an error of SEV_FATAL, do set aip->io_failure
*
*   NOTE: if you are using a pure iterator (AsnIoNullOpen()) instead of really
*     writing...
*
*   To avoid checking all strings, set aip->fix_non_print = 2
*   To check, get an error message, but not die, leave aip->fix_non_print=0
*   To check and fail with non printing chars, set aip->fix_non_print=3
*
*
*****************************************************************************/


typedef struct asniomem {    /* for AsnIo to and from a memory block */
	AsnIoPtr aip;			 /* the AsnIoPtr for this */
	BytePtr buf;			 /* a buffer for the data */
	Int4 size,		/* size of this buffer (w) or bytes_to_read (r) */
			count;		/* count of bytes read from or written to buffer */
} AsnIoMem, PNTR AsnIoMemPtr;

typedef struct asniobs {    /* for AsnIo to and from a memory ByteStore */
	AsnIoPtr aip;			 /* the AsnIoPtr for this */
	ByteStorePtr bsp;        /* byte store for this */
} AsnIoBS, PNTR AsnIoBSPtr;

/***** typedefs used often in object loaders **********/

typedef Pointer (LIBCALL *AsnReadFunc) PROTO((AsnIoPtr aip, AsnTypePtr atp));
typedef Boolean (LIBCALL *AsnWriteFunc) PROTO((Pointer object, AsnIoPtr aip, AsnTypePtr atp));

typedef Boolean (LIBCALLBACK *AsnStreamStringFunc) (Pointer object, AsnIoPtr aip);

/*****************************************************************************
*
*   prototypes
*
*****************************************************************************/
/*** asngen.c ****/

NLM_EXTERN AsnTypePtr LIBCALL AsnReadId PROTO((AsnIoPtr aip, AsnModulePtr amp, AsnTypePtr atp));
NLM_EXTERN Int2  LIBCALL AsnReadVal PROTO((AsnIoPtr aip, AsnTypePtr atp, DataValPtr vp));
NLM_EXTERN Boolean LIBCALL AsnWrite PROTO((AsnIoPtr aip, AsnTypePtr atp, DataValPtr dvp));
NLM_EXTERN Boolean LIBCALL AsnWriteEx (AsnIoPtr aip, AsnTypePtr atp, DataValPtr dvp, AsnStreamStringFunc stream);
NLM_EXTERN Boolean LIBCALL AsnSkipValue PROTO((AsnIoPtr aip, AsnTypePtr atp));

NLM_EXTERN Boolean LIBCALL AsnOpenStruct PROTO((AsnIoPtr aip, AsnTypePtr atp,
			Pointer the_struct));
NLM_EXTERN Boolean LIBCALL AsnCloseStruct PROTO((AsnIoPtr aip, AsnTypePtr atp,
			Pointer the_struct));
NLM_EXTERN Boolean LIBCALL AsnWriteChoice PROTO((AsnIoPtr aip, AsnTypePtr atp, Int2 choice,
			DataValPtr the_value));
NLM_EXTERN void LIBCALL AsnCheckExpOpt PROTO((AsnIoPtr aip, AsnTypePtr atp, DataValPtr dvp));
NLM_EXTERN AsnExpOptPtr LIBCALL AsnExpOptNew PROTO((AsnIoPtr aip, CharPtr path,
			Pointer user_data, AsnExpOptFunc user_func));
NLM_EXTERN AsnExpOptPtr LIBCALL AsnExpOptFree PROTO((AsnIoPtr aip, AsnExpOptPtr aeop));
NLM_EXTERN VoidPtr LIBCALL AsnFindNthPieceOfObject PROTO((AsnWriteFunc wfunc, Pointer datum, CharPtr string, Int4 n));

NLM_EXTERN Int2 LIBCALL AsnGetLevel PROTO((AsnIoPtr aip));
NLM_EXTERN void LIBCALL AsnNullValueMsg PROTO((AsnIoPtr aip, AsnTypePtr node));

/*** asntypes.c ***/

NLM_EXTERN void LIBCALL AsnKillValue PROTO((AsnTypePtr atp, DataValPtr dvp));
NLM_EXTERN AsnTypePtr PNTR LIBCALL AsnTypePathFind PROTO((AsnModulePtr amp, CharPtr str, Int2Ptr numtypes));
NLM_EXTERN AsnTypePtr LIBCALL AsnTypeFind PROTO((AsnModulePtr amp, CharPtr str));
#define AsnFind(x) AsnTypeFind(NULL,x)    /* find type (all) */
NLM_EXTERN CharPtr LIBCALL AsnFindPrimName PROTO((AsnTypePtr atp));
NLM_EXTERN AsnTypePtr LIBCALL AsnFindBaseType PROTO((AsnTypePtr atp));
NLM_EXTERN AsnTypePtr LIBCALL AsnFindBaseTypeDTD PROTO((AsnTypePtr atp));
NLM_EXTERN CharPtr LIBCALL AsnFindBaseName PROTO((AsnTypePtr atp));
NLM_EXTERN Int2 LIBCALL AsnFindBaseIsa PROTO((AsnTypePtr atp));
NLM_EXTERN AsnTypePtr LIBCALL AsnLinkType PROTO((AsnTypePtr type, AsnTypePtr localtype));
NLM_EXTERN void LIBCALL AsnUnlinkType PROTO((AsnTypePtr type));
NLM_EXTERN CharPtr LIBCALL AsnTypeDumpStack PROTO((CharPtr str, AsnIoPtr aip));
NLM_EXTERN Boolean LIBCALL AsnTreeLoad PROTO((char * file, AsnValxNodePtr * avnptr, AsnTypePtr * atptr, AsnModulePtr * ampptr));
NLM_EXTERN void LIBCALL AsnStoreTree PROTO((CharPtr file, AsnModulePtr amp));
#define AsnLoad() AsnTreeLoad(asnfilename, &avn, &at, &amp)   /* simple loader */
NLM_EXTERN void LIBCALL AsnModuleLink PROTO((AsnModulePtr amp));
NLM_EXTERN CharPtr LIBCALL AsnEnumStr PROTO((CharPtr str, Int2 val));
NLM_EXTERN CharPtr LIBCALL AsnEnumTypeStr PROTO((AsnTypePtr atp, Int2 val));
NLM_EXTERN AsnModulePtr LIBCALL AsnAllModPtr PROTO((void));

/*****************************************************************************
*
*   Int4 AsnTypeStringToHex(from, len, to, left)
*   	converts an octet string to binary
*   	returns number of hex digits created if all ok
*       *left is chars left at the end of the buffer including first letter of
*          a remaining digit (from does not have an even number of letters)
*          since this could include white space, could be more than 1
*       returns a negative number on an error
*       skips over internal or trailing white space
*       left can be NULL, in which case it is ignored
*
*****************************************************************************/
NLM_EXTERN Int4 LIBCALL AsnTypeStringToHex (Pointer from, Int4 len, Pointer to, Int4Ptr left);

/*** asnio.c ****/

NLM_EXTERN CharPtr LIBCALL AsnErrGetTypeName PROTO((CharPtr name));
NLM_EXTERN AsnIoPtr LIBCALL AsnIoOpen PROTO((CharPtr file_name, CharPtr mode));
NLM_EXTERN AsnIoPtr LIBCALL AsnIoClose PROTO((AsnIoPtr aip));
NLM_EXTERN AsnIoPtr LIBCALL AsnIoFree PROTO((AsnIoPtr aip, Boolean close_file));
NLM_EXTERN void LIBCALL AsnIoReset PROTO((AsnIoPtr aip));
NLM_EXTERN void LIBCALL AsnIoSetErrorMsg PROTO((AsnIoPtr aip, ErrorRetType error_ret));
NLM_EXTERN Int4 LIBCALL AsnIoSeek PROTO((AsnIoPtr aip, Int4 pos));
NLM_EXTERN Int4 LIBCALL AsnIoTell PROTO((AsnIoPtr aip));
NLM_EXTERN void LIBCALL AsnIoFlush PROTO((AsnIoPtr aip));
NLM_EXTERN AsnIoPtr LIBCALL AsnIoNew PROTO((Int1 type, FILE * fp, Pointer iostruct, IoFuncType readfunc, IoFuncType writefunc));
NLM_EXTERN Boolean LIBCALL AsnIoSetBufsize PROTO((AsnIoPtr aip, Int2 size));
NLM_EXTERN AsnOptionPtr LIBCALL AsnIoOptionNew PROTO((AsnIoPtr aip, Int2 ao_class, Int2 type, DataVal av, AsnOptFreeFunc freefunc));
NLM_EXTERN void LIBCALL AsnIoOptionFree PROTO((AsnIoPtr aip, Int2 ao_class, Int2 type));
NLM_EXTERN Boolean LIBCALL AsnClassTypeMatch PROTO((Int2 ao_class, Int2 type, Int2 this_class, Int2 this_type));
NLM_EXTERN AsnOptionPtr LIBCALL AsnIoOptionGet PROTO((AsnIoPtr aip, Int2 ao_class, Int2 type, AsnOptionPtr last));
NLM_EXTERN AsnOptionPtr LIBCALL AsnOptionNew PROTO((AsnOptionPtr PNTR aopp, Int2 ao_class, Int2 type, DataVal av, AsnOptFreeFunc freefunc));
NLM_EXTERN void LIBCALL AsnOptionFree PROTO((AsnOptionPtr PNTR aopp, Int2 ao_class, Int2 type));
NLM_EXTERN AsnOptionPtr LIBCALL AsnOptionGet PROTO((AsnOptionPtr head, Int2 ao_class, Int2 type, AsnOptionPtr last));
NLM_EXTERN Boolean LIBCALL AsnSetXMLmodulePrefix (CharPtr prefix);
NLM_EXTERN Boolean LIBCALL AsnSetXMLmodulePrefixToDefault (void);
NLM_EXTERN CharPtr LIBCALL AsnGetXMLmodulePrefix (void);

    /*** calculate hash value from ASN.1 ***/
NLM_EXTERN Uint4 LIBCALL  AsnIoHash (Pointer from, AsnWriteFunc writefunc);

    /*** read and write to memory buffer ***/
NLM_EXTERN AsnIoMemPtr LIBCALL AsnIoMemOpen PROTO((CharPtr mode, BytePtr buf, Int4 size));
NLM_EXTERN AsnIoMemPtr LIBCALL AsnIoMemClose PROTO((AsnIoMemPtr aimp));
NLM_EXTERN Boolean LIBCALL AsnIoMemReset PROTO((AsnIoMemPtr aimp, Int4 bytes_to_read));
NLM_EXTERN Int2 LIBCALL AsnIoMemRead PROTO((Pointer, CharPtr, Uint2));
NLM_EXTERN Int2 LIBCALL AsnIoMemWrite PROTO((Pointer, CharPtr, Uint2));

   /*** read and write to a ByteStore in memory ***/
NLM_EXTERN AsnIoBSPtr LIBCALL AsnIoBSOpen PROTO((CharPtr mode, ByteStorePtr bsp));
NLM_EXTERN AsnIoBSPtr LIBCALL AsnIoBSClose PROTO((AsnIoBSPtr aibp));
NLM_EXTERN Int2 LIBCALL AsnIoBSRead PROTO((Pointer, CharPtr, Uint2));
NLM_EXTERN Int2 LIBCALL AsnIoBSWrite PROTO((Pointer, CharPtr, Uint2));

  /** Copy and Compare functions ***/
NLM_EXTERN Pointer LIBCALL AsnIoCopy PROTO((Pointer from, AsnReadFunc readfunc, AsnWriteFunc writefunc));
NLM_EXTERN Pointer LIBCALL AsnIoMemCopy PROTO((Pointer from, AsnReadFunc readfunc, AsnWriteFunc writefunc));
NLM_EXTERN Boolean LIBCALL AsnIoMemComp PROTO((Pointer a, Pointer b, AsnWriteFunc writefunc));

#define AsnIoNullOpen() AsnIoNew((ASNIO_OUT | ASNIO_TEXT | ASNIO_CARRIER), NULL, NULL, NULL, NULL)

/*** asndebin.c ***/

NLM_EXTERN AsnTypePtr LIBCALL AsnBinReadId PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Int2 LIBCALL AsnBinReadVal PROTO((AsnIoPtr aip, AsnTypePtr atp, DataValPtr vp));

/*** asnenbin.c ***/

NLM_EXTERN Boolean LIBCALL AsnBinWrite PROTO((AsnIoPtr aip, AsnTypePtr atp, DataValPtr dvp));
         /** expert use only ***/
NLM_EXTERN Boolean LIBCALL AsnEnBinTheBytes PROTO((Pointer ptr, Uint4 len, AsnIoPtr aip, Boolean is_string));
#define AsnEnBinBytes(a,b,c) AsnEnBinTheBytes(a, b, c, FALSE)

/*** asnlex.c ***/

NLM_EXTERN AsnTypePtr LIBCALL AsnTxtReadId PROTO((AsnIoPtr aip, AsnModulePtr amp, AsnTypePtr atp));
NLM_EXTERN Int2 LIBCALL AsnTxtReadVal PROTO((AsnIoPtr aip, AsnTypePtr atp, DataValPtr vp));

/*** asnprint.c ***/

NLM_EXTERN Boolean LIBCALL AsnTxtWrite PROTO((AsnIoPtr aip, AsnTypePtr atp, DataValPtr dvp));
NLM_EXTERN Boolean LIBCALL  AsnTxtWriteEx (AsnIoPtr aip, AsnTypePtr atp, DataValPtr dvp, AsnStreamStringFunc stream);

/*** asnlext.c ***/

NLM_EXTERN AsnModulePtr LIBCALL AsnLoadModules PROTO((AsnIoPtr aip));

/*** asngenob.c ***/
NLM_EXTERN ValNodePtr LIBCALL AsnGenericBaseSeqOfAsnRead PROTO ((AsnIoPtr aip, AsnModulePtr amp, AsnTypePtr orig, int whichvalslot, BoolPtr isError));
NLM_EXTERN Boolean LIBCALL AsnGenericBaseSeqOfAsnWrite PROTO ((ValNodePtr ptr, int whichvalslot, AsnIoPtr aip, AsnTypePtr bag_atp, AsnTypePtr element_atp));
NLM_EXTERN Boolean LIBCALL AsnGenericBaseSeqOfFree PROTO ((ValNodePtr ptr, int whichvalslot));
NLM_EXTERN Pointer AsnGenericUserSeqOfAsnRead PROTO ((AsnIoPtr aip, AsnModulePtr amp, AsnTypePtr orig, BoolPtr isError, AsnReadFunc readfunc, AsnOptFreeFunc freefunc));
NLM_EXTERN Boolean LIBCALL AsnGenericUserSeqOfAsnWrite PROTO ((Pointer ptr, AsnWriteFunc writefunc, AsnIoPtr aip, AsnTypePtr bag_atp, AsnTypePtr element_atp));
NLM_EXTERN Boolean LIBCALL AsnGenericUserSeqOfFree PROTO ((Pointer ptr, AsnOptFreeFunc freefunc));
NLM_EXTERN Pointer LIBCALL AsnGenericChoiceSeqOfAsnRead PROTO ((AsnIoPtr aip, AsnModulePtr amp, AsnTypePtr orig, BoolPtr isError, AsnReadFunc readfunc, AsnOptFreeFunc freefunc));
NLM_EXTERN Boolean LIBCALL AsnGenericChoiceSeqOfAsnWrite PROTO ((Pointer ptr, AsnWriteFunc writefunc, AsnIoPtr aip, AsnTypePtr bag_atp, AsnTypePtr element_atp));
NLM_EXTERN Boolean LIBCALL AsnGenericChoiceSeqOfFree PROTO ((Pointer ptr, AsnOptFreeFunc freefunc));

/*** asnbufo.c ***/
NLM_EXTERN Boolean LIBCALL AsnBufWrite PROTO ((AsnIoPtr aip, AsnTypePtr atp, CharPtr buf, size_t buflen));
NLM_EXTERN Boolean LIBCALL AsnBinBufWrite PROTO ((AsnIoPtr aip, AsnTypePtr atp, CharPtr buf, size_t buflen));
NLM_EXTERN Boolean LIBCALL AsnTxtBufWrite PROTO ((AsnIoPtr aip, AsnTypePtr atp, CharPtr buf, size_t buflen));



/******** temporary defines for older code *************/

#define AsnStartStruct(x,y) AsnOpenStruct(x, y, NULL)
#define AsnEndStruct(x,y) AsnCloseStruct(x, y, NULL)

/***** AsnOption ao_class values - do not reuse ***************/
/***** all positive numbers > 0 are available to non-NCBI applications ***/

#define OP_ANY          0
#define OP_TOGENBNK    -1
#define OP_BB2ASN      -2
#define OP_NCBIOBJSSET -3
#define OP_NCBIOBJSEQ  -4
#define OP_GET_MUID    -5
#define OP_NCBIASNTOOL -6
#define OP_NCBIHINTS   -7
#define OP_GESTALT     -8
#define OP_NCBIOBJSTR  -9
#define OP_TYPEORDER   -10     /* for printing out DTD or spec */
#define OP_COMMENTBEFORE -11   /*  ditto */
#define OP_COMMENT     -12      /*  ditto */

/****** these are the possible returns from AsnFindBaseIsa() *****/
/****** the numbers are arbitrary, but should never be changed ***/

#define BOOLEAN_TYPE			301		/* BOOLEAN */
#define INTEGER_TYPE			302		/* INTEGER */
#define BITS_TYPE				303		/* BIT STRING */
#define OCTETS_TYPE				304		/* OCTET STRING */
#define NULL_TYPE				305		/* NULL */
#define OBID_TYPE				306		/* OBJECT IDENTIFIER */
#define OBDES_TYPE				307		/* ObjectDescriptor */
#define EXTERNAL_TYPE			308		/* EXTERNAL */
#define REAL_TYPE				309		/* REAL */
#define ENUM_TYPE				310		/* ENUMERATED */
#define SEQ_TYPE				311		/* SEQUENCE */
#define SEQOF_TYPE				312		/* SEQUENCE OF */
#define SET_TYPE				313		/* SET */
#define SETOF_TYPE				314		/* SET OF */
#define CHOICE_TYPE				315		/* CHOICE */
#define ANY_TYPE				316		/* ANY */
#define	NUMERICSTRING_TYPE		317	    /* String Types */
#define	PRINTABLESTRING_TYPE	318
#define TELETEXSTRING_TYPE		319
#define VIDEOTEXSTRING_TYPE		320
#define IA5STRING_TYPE 			321
#define GRAPHICSTRING_TYPE		322
#define VISIBLESTRING_TYPE		323
#define GENERALSTRING_TYPE		324
#define CHARACTERSTRING_TYPE	325
#define GENTIME_TYPE			326		/* Time types */
#define UTCTIME_TYPE			327

#define STRSTORE_TYPE			351		/* Application: StringStore */
#define BIGINT_TYPE                     352             /* Application: Int8 */

/******* grouping macros on ISA defines above ********************/

#define ISA_STRINGTYPE(x)	(((x) >= 317) && ((x) <= 325))
#define ISA_STRUCTTYPE(x)	(((x) >= 311) && ((x) <= 314))
#define ISA_INTTYPE(x)		(((x) == 302) || ((x) == 310))

/* #defines used by automatically-generated object loaders */
#define ASNCODE_PTRVAL_SLOT 1
#define ASNCODE_REALVAL_SLOT 2
#define ASNCODE_INTVAL_SLOT 3
#define ASNCODE_BOOLVAL_SLOT 4
#define ASNCODE_BYTEVAL_SLOT 5
#define ASNCODE_BIGINTVAL_SLOT 6

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
