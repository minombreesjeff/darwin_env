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
* File Name:  asncode.c
*
* Author:  Karl Sirotkin
*
* Version Creation Date: 7/8/93
*
* $Revision: 6.16 $
*
* File Description:
*   Automatically generate C code from ASN.1 specifications
*
*       -l Loadable filename, not identical to string after -l in asntool
*       -o name for generated code prefix
*       -n hints file
*       -b Size         (set AsnIo buffer size)
*       -w Word length for #define
*
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: asncode.c,v $
* Revision 6.16  2005/01/24 17:12:11  kans
* added force_choice_struct (-V) to force struct plus object instead of ValNode for choices - for compatibility with old hand-coded object loaders
*
* Revision 6.15  2004/07/08 15:24:05  kans
* needed a couple additional TESTNIL wrappers
*
* Revision 6.14  2002/03/07 21:36:27  beloslyu
* typo fixed
*
* Revision 6.13  2001/11/05 20:25:34  madden
* Fix (by Karl Sirotkin) for underscores (e.g., struct_BlastDefLine to struct_Blast_def_line)
*
* Revision 6.12  2001/06/28 02:14:40  juran
* Fixed log message.
* Testing how new MacCVS Pro handles multi-line comments.
*
* Revision 6.11  2001/06/28 01:53:32  juran
* Mac compatibility:
* Redefine NULL to 0L, which gets promoted to any pointer type.
* Cast result of MemNew() to appropriate pointer type.
*
* Revision 6.10  2000/12/12 15:56:09  ostell
* added support BigInt
*
* Revision 6.9  2000/05/05 19:28:31  kans
* type for OCTET STRING really is ByteStorePtr, so this is now used instead of Pointer (KS)
*
* Revision 6.8  1999/08/02 14:18:55  sirotkin
* some compiler nits.
*
* Revision 6.7  1999/07/30 17:23:30  sirotkin
* removed.an.unused
*
* Revision 6.6  1999/03/08 15:20:22  kans
* changed single ampersand to double ampersand
*
* Revision 6.5  1998/06/12 19:27:44  kans
* fixed unix compiler warnings
*
* Revision 6.4  1998/05/02 18:54:27  kans
* for Mac, use %LG instead of %lg, which is not legal C, and not supported by CodeWarrior
*
* Revision 6.3  1998/05/01 23:35:21  kans
* use %lf on Mac, %lg elsewhere, sprintf bug reported
*
* Revision 6.2  1998/03/25 23:31:34  kans
* params to register new object manager type, give optional non-default label
*
* Revision 6.1  1997/12/16 14:51:42  kans
* header needed for asntool/asncode merge
*
* Revision 6.0  1997/08/25 18:09:41  madden
* Revision changed to 6.0
*
* Revision 5.2  1997/01/02 17:57:47  epstein
* add more NLM_EXTERN logic per D. Vakatov
*
 * Revision 5.1  1996/12/05  19:51:40  epstein
 * add support for NLM_EXTERN
 *
 * Revision 5.0  1996/05/28  14:00:29  ostell
 * Set to revision 5.0
 *
 * Revision 4.2  1995/11/07  20:07:29  epstein
 * change quotes on #includes to angle-brackets
 *
 * Revision 4.1  1995/08/24  16:57:10  epstein
 * remove stray backslashes per Peter Cartwright <Peter.Cartwright@genetics.utah.edu>
 *
 * Revision 4.0  1995/07/26  13:47:38  ostell
 * force revision to 4.0
 *
 * Revision 1.20  1995/06/26  20:32:02  kans
 * AsnCodeIsEnumType needed to be prototyped as static
 *
 * Revision 1.19  1995/05/15  18:38:28  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

#include "asnbuild.h"
#include "asntool.h"

#undef NULL
#define NULL 0L

/*****************************************************************************
*
*   prototypes
*
*****************************************************************************/

static Boolean AsnCodeIsEnumType PROTO ((AsnTypePtr atp));

static char     RCS_Rev [] = "$Revision: 6.16 $";

/*******************
 * Interator structure
 ***********************/

typedef struct struct_AsnCodeLines {
   CharPtr line;
   struct struct_AsnCodeLines PNTR next;
} AsnCodeLines, PNTR AsnCodeLinesPtr;

typedef struct struct_spooled {
   struct struct_AsnCodeLines PNTR head_line;
   struct struct_AsnCodeLines PNTR tail_line;
   struct struct_spooled PNTR next;
   struct struct_spooled PNTR last;
   Boolean finished;
} AsnCodeSpooled, PNTR AsnCodeSpooledPtr;

typedef struct struct_AsnCodeNode {
   AsnTypePtr atp;
   CharPtr slot_name;
   CharPtr asn_type_name;
   CharPtr obj_name;
   Int2 recur_type;
   Int2 type;
   Int2 slot_count;
   Int2 opt_count;
   struct struct_AsnCodeNode PNTR outer;
   struct struct_AsnCodeNode PNTR inner;
} AsnCodeNode, PNTR AsnCodeNodePtr;

#define ASNCODE_START 400

typedef struct struct_asniter {
   AsnCodeInfoPtr acip;
   FILE * fp; 
   Int2 mode;  /* so FnPtrs can be kept the same and this varied */
   ValNodePtr bufs;
   AsnCodeSpooledPtr routines;    /* head pointer */
   AsnCodeSpooledPtr cur_routine; /* active pointer */
   AsnCodeSpooledPtr last_routine;/* tail pointer */
   AsnTypePtr atp;
   Int2 depth;
   struct struct_AsnCodeNode PNTR stack;
   struct struct_AsnCodeNode PNTR base_of_stack;
   char buf [1024];
   char buf2 [60];
   CharPtr module_name;
   Int2 indent;
   Boolean leadingspaces;
   FnPtr basetype;
   FnPtr choiceend;
   FnPtr choicestart;
   FnPtr less301err;
   FnPtr seqend;
   FnPtr seqstart; 
   FnPtr setend;
   FnPtr setstart;
   FnPtr undef301err;
   FnPtr userobj;
   Boolean do_opt_bits;
   Boolean do_chs_struct;
} AsnIter, PNTR AsnIterPtr;

typedef void (* AsnCodeIterFunc) PROTO (( AsnIterPtr iter));

/*****************
*
* OPTIONS
*
**************/

#define OP_MAKE_LINKED 1
#define OP_HINTED 2
#define OP_SLOT_NAME 3 
#define OP_OBJ_NAME 4

#define WASH 1
#define CLEAN_FOR_SLOT 1

#define BUFF 2
#define CLEAN_FOR_OBJ_NAME 2

#define SLOT_BUFF 3
#define SLOT_POLISH 4
#define CLEAN_FOR_ASN_TYPE 4

#define ASNITER_USER_TYPE 1400
#define ASNITER_Pseudo_USER_TYPE 1450
#define ASNITER_LESS300 1300
#define ASNITER_BRANCH_TYPE 1500 
#define ASNITER_BASE_TYPE 1600 

#define ASNITER_RECUR_NONE 0
#define ASNITER_RECUR_WAS_LAST 1
#define ASNITER_RECUR_SEQSET 2
#define ASNITER_RECUR_CHOICE 3 
#define ASNITER_RECUR_SETOF 4

#define RECUR_OUTER 0
#define RECUR_SEQSET 1
#define RECUR_CHOICE 2
#define RECUR_SETOF_OUTER 3
#define RECUR_SETOF_SEQSET 4
#define RECUR_SETOF_CHOICE 5  

#define ITER_ASSIGN 1
#define ITER_CHECK_LIST 2
#define ITER_OBJECTS 3
#define ITER_NEW 4
#define ITER_FREE 5
#define ITER_READ 6
#define ITER_WRITE 7

#define BUF_LEN 100

#define DEBUG
#define DEEP_DEBUG
#define CFG_FILE_NAME "NCBI"
#define CTX_ASNCODE 500

#define TESTNIL(x) ((x) == NULL ? "(nil)" : x)

#define AsnIterTakeBuf(x)  AsnIterTakeBufReal(x, __LINE__)

#define USER_PSEUDOOBJECTS_STATIC

CharPtr c_reserved_word[] = {
   "asm", /* not part of ANSI, but commonly reserved */
   "auto",
   "break",
   "case",
   "catch", /* C++ (not ANSI C) */
   "char",
   "class", /* C++ (not ANSI C) */
   "const",
   "continue",
   "default",
   "defined",
   "delete", /* C++ (not ANSI C) */
   "do",
   "double",
   "else",
   "enum",
   "extern",
   "float",
   "for",
   "fortran", /* not part of ANSI, but commonly reserved */
   "friend", /* C++ (not ANSI C) */
   "goto",
   "if",
   "inline", /* C++ (not ANSI C) */
   "int",
   "long",
   "new", /* C++ (not ANSI C) */
   "noalias", /* not part of ANSI standard? (part of pre-release standard) */
   "operator", /* C++ (not ANSI C) */
   "private", /* C++ (not ANSI C) */
   "protected", /* C++ (not ANSI C) */
   "public", /* C++ (not ANSI C) */
   "register",
   "return",
   "short",
   "signed"
   "sizeof",
   "static",
   "struct",
   "switch",
   "template", /* C++ (not ANSI C) */
   "this", /* C++ (not ANSI C) */
   "throw", /* C++ (not ANSI C) */
   "try", /* C++ (not ANSI C) */
   "typedef",
   "union",
   "unsigned",
   "virtual", /* C++ (not ANSI C) */
   "void",
   "volatile",
   "while"};

#define NUM_C_RESERVED_WORDS (sizeof(c_reserved_word)/sizeof(CharPtr))

#ifdef TEST_AS_MAIN

#define NUMARGS 9

Args            asnargs[NUMARGS] = {
   {"Loadable Filenames, for parsetrees [comma separated\nlist, only first used for objects],\
not quite identical to\nstring after -l to asntool", NULL, NULL, NULL, FALSE, 'l', ARG_FILE_IN, 0.0, 0, NULL},
   {"Asnload directory [for parsetrees]", NULL, NULL, NULL, TRUE, 'd', ARG_FILE_IN, 0.0, 0, NULL},
   {"filename, without extensions, for objects and code", NULL, NULL, NULL, FALSE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},
   {"Debugging level\n     0 - No debugging\n     1 - Shallow debugging\n     2 - Deep", "0", "0", "9", TRUE, 'v', ARG_INT, 0.0, 0, NULL},
   {"Buffer Size", "1024", "512", "10000", TRUE, 'b', ARG_INT, 0.0, 0, NULL},
   {"See debugging file", "stderr", NULL, NULL, TRUE, 's', ARG_FILE_OUT, 0.0, 0, NULL},
   {"Include filename to include", NULL, NULL, NULL, TRUE, 'i', ARG_STRING, 0.0, 0, NULL},
   {"Word length maximum for #defines","31","31","128",TRUE,'w',ARG_INT,0.0,0,NULL},
   {"Bit twiddle for optional zero value base slots","F",NULL,NULL,TRUE,'t',ARG_BOOLEAN,0.0,0,NULL}
};

#endif


CharPtr AsnCodeCleanName PROTO ((CharPtr name, char PNTR buf, int wash_or_buf, int maxlen));
void AsnCodeIter PROTO ((AsnIterPtr iter));
void AsnCodePop PROTO ((AsnIterPtr iter, AsnTypePtr atp, Int2 type));
void AsnCodePush PROTO ((AsnIterPtr iter, AsnTypePtr atp, Int2 type, Int2 recurtype));
void AsnCodeIterCall PROTO ((AsnIterPtr iter, FnPtr slot, AsnTypePtr atp, Int2 placetype));
static void AsnCodeShowStack PROTO ((AsnIterPtr iter));

#define MACRO_SETARG(TAG,P) \
   {\
      P = Nlm_WhichArg (TAG, NUMARGS,asnargs);\
      if ( P < 0){\
         ErrPost(1,100,\
         "Program error looking for arg %c\n", TAG);\
         has_trouble = TRUE;\
      }\
   }

static CharPtr  defptr = "ASNCODE_PTRVAL_SLOT";
static CharPtr  defint = "ASNCODE_INTVAL_SLOT";
static CharPtr  defbigint = "ASNCODE_BIGINTVAL_SLOT";
static CharPtr  defreal = "ASNCODE_REALVAL_SLOT";
static CharPtr  defbool = "ASNCODE_BOOLVAL_SLOT";
static CharPtr  defbyte = "ASNCODE_BYTEVAL_SLOT";

Boolean AsnIterNeedsNullCheck PROTO((AsnIterPtr iter, AsnTypePtr final_atp));

static void
AsnCodeShowDepth (AsnIterPtr iter)
{
   int             dex;

   for (dex = 0; dex < iter->depth; dex++) {
      fprintf ((iter->acip -> bug_fp), "|");
   }

}

static CharPtr
SpanLengthyPrefix (CharPtr buf, int maxlen)
{
   CharPtr         symbol;
   int             dex;

   dex = maxlen;
   symbol = &buf[StrLen (buf)];
   while ((dex) && (symbol > buf)) {
      dex--;
      symbol--;
   }
   if (symbol != buf) {		/* longer than (acip -> maxDefineLength) */
      while (*symbol != '_')	/* end neatly */
	 symbol++;
      symbol++;
   }
   return symbol;
}

static CharPtr
Append_EAndPrestrip (CharPtr inbuf, int maxlen)
{
   static Char     the_name[160];

   sprintf (the_name, "%s_E", inbuf);
   return SpanLengthyPrefix (the_name, maxlen);
}

static CharPtr
GenerateTypeFromStack (AsnIterPtr iter, Boolean use_final_E)
{
   static Char     the_name[160];
   CharPtr         temp = the_name;
   CharPtr         symbol;
   int             dex;
   AsnCodeNodePtr  node;
   Boolean         first = TRUE;
   Char            buf[120];
   Boolean         needs_E = FALSE;

   for (dex = 0; dex < sizeof (the_name); dex++) {
      the_name[dex] = '\0';
   }
   for (node = iter->base_of_stack; node; node = node->inner) {
      if (node->atp->name == NULL)
	 continue;
      if (node->type == ASNITER_Pseudo_USER_TYPE)
	 continue;
      if (first) {
	 first = FALSE;
	 temp = StringMove (temp, AsnCodeCleanName (node->atp->name, buf,
						    CLEAN_FOR_ASN_TYPE, iter -> acip -> maxDefineLength));
      } else {
	 if (needs_E)
	    temp = StringMove (temp, "_E");
	 needs_E = FALSE;
	 temp = StringMove (temp, "_");
	 temp = StringMove (temp, AsnCodeCleanName (node->atp->name, buf,
						    CLEAN_FOR_SLOT, iter -> acip -> maxDefineLength));
      }
      if (node->atp->type) {
	 if (node->atp->type->isa == SETOF_TYPE ||
	     node->atp->type->isa == SEQOF_TYPE) {
	    needs_E = TRUE;
	 }
      }
   }
   if (needs_E && use_final_E)
      temp = StringMove (temp, "_E");

   symbol = SpanLengthyPrefix (the_name, iter -> acip -> maxDefineLength);

   if ((iter -> acip->debug_level) > 3) {
      AsnCodeShowStack (iter);
      fprintf ((iter -> acip -> bug_fp), "YIELDED:(%c)%s\n", use_final_E ? 'T' : 'F', symbol);
      fflush ((iter -> acip -> bug_fp));
   }
   return symbol;
}

static void
AsnCodeShowStack (AsnIterPtr iter)
{
   int             dex;
   AsnCodeNodePtr  stack;

   for (dex = 0, stack = iter->stack; stack; stack = stack->outer,
	dex++) {
      fprintf ((iter -> acip -> bug_fp), "%d:%s-%d,%d", dex,
	       TESTNIL (stack->atp->name), (int) stack->recur_type,
	       (int) stack->type);
      if (stack->atp)
	 if (stack->atp->type) {
	    fprintf ((iter -> acip -> bug_fp), "=%s", TESTNIL (stack->atp->type->name));
	 }
      fprintf ((iter -> acip -> bug_fp), "\n");
      fflush ((iter -> acip -> bug_fp));
   }
   fflush ((iter -> acip -> bug_fp));

}

#ifdef TEST_AS_MAIN
/*****************************************************************************
*
*   Nlm_WhichArg(ap)
*     returns array position for a tag
*
*****************************************************************************/
static Nlm_Int2
Nlm_WhichArg (Nlm_Char which, Nlm_Int2 numargs, Nlm_ArgPtr ap)
{
   Nlm_Boolean     okay = FALSE;
   Nlm_Int2        i;
   Nlm_ArgPtr      curarg;
   Nlm_Int2        retval = -1;

   if ((ap == NULL) || (numargs == 0))
      return okay;

   curarg = ap;			/* set defaults */

   for (i = 0; i < numargs; i++, curarg++) {
      if (curarg->tag == which) {
	 retval = i;
	 break;
      }
   }

   return retval;
}

Int2
Main (void)
{
   AsnModulePtr    amp = NULL;
   AsnTypePtr      atp = NULL;
   AsnValxNodePtr  avn = NULL;
   AsnModulePtr    last_amp = NULL;
   CharPtr         filename = NULL, p, last_comma;
   CharPtr         first_filename = NULL;
   int             len;
   Boolean         asnloadWasModified = FALSE;
   Boolean         asnloadValExisted = FALSE;
   Boolean         asnloadSectionExisted = FALSE;
   Char            asnloadOldVal[PATH_MAX];
   Char            buf[20];
   Int2            debugarg, loadfilesarg, dirarg, outnamearg, bufarg, seearg,
                   includearg, maxdefarg, bittwiddlearg;
   Boolean         has_trouble = FALSE;
   AsnCodeInfoPtr  acip = MemNew (sizeof(AsnCodeInfo));

   if (!GetArgs ("AsnCode 1", NUMARGS, asnargs))
      return 1;

   MACRO_SETARG ('l', loadfilesarg)
   MACRO_SETARG ('b', bufarg)
   MACRO_SETARG ('i', includearg)
   MACRO_SETARG ('o', outnamearg)
   MACRO_SETARG ('v', debugarg)
   MACRO_SETARG ('d', dirarg)
   MACRO_SETARG ('s', seearg)
   MACRO_SETARG ('t', bittwiddlearg)
   MACRO_SETARG ('w', maxdefarg)


#ifdef USE_LOG_ERRPOST
   /* log errors instead of die */
   if (asnargs[seearg].strvalue != NULL) {
      if (!ErrSetLog (asnargs[seearg].strvalue))
	 ErrShow ();
      else
	 ErrSetOpts (ERR_TEE, ERR_LOG_ON);
   }
#else
   if (asnargs[seearg].strvalue != NULL) {
      (acip -> bug_fp) = FileOpen (asnargs[seearg].strvalue, "w");
   } else {
      (acip -> bug_fp) = FileOpen ("stderr", "w");
   }
#endif

   if (!AsnIoSetBufsize (NULL, (Int2) asnargs[bufarg].intvalue))
      return 1;


   (acip->debug_level) = asnargs[debugarg].intvalue;
   (acip -> maxDefineLength) = asnargs[maxdefarg].intvalue;

   if (asnargs[dirarg].strvalue != NULL) {	/* temporarily set ASNLOAD
						 * value */
      asnloadValExisted = TRUE;
      asnloadSectionExisted = TRUE;

      if (GetAppParam (CFG_FILE_NAME, "NCBI", "ASNLOAD", "", asnloadOldVal, sizeof asnloadOldVal) == 0) {
	 asnloadValExisted = FALSE;
	 if (GetAppParam (CFG_FILE_NAME, "NCBI", NULL, "", buf, sizeof buf) == 0) {
	    asnloadSectionExisted = FALSE;
	 }
      }
      SetAppParam (CFG_FILE_NAME, "NCBI", "ASNLOAD", asnargs[dirarg].strvalue);
      asnloadWasModified = TRUE;
   }
   for (p = asnargs[loadfilesarg].strvalue; *p; p = last_comma + 1) {
      for (last_comma = p; *last_comma; last_comma++) {
	 if (*last_comma == ',')
	    break;
      }
      len = last_comma - p;
      filename = MemFree (filename);
      filename = MemNew (len + 1);
      StringNCpy (filename, p, len);
      filename[len] = '\0';
#ifdef WIN_DUMB
      printf ("Loading %s \n", filename);
#endif
      amp = NULL;
      if (!AsnTreeLoad (filename, &avn, &atp, &amp))
	 return 1;
      amp = AsnAllModPtr ();
      if (last_amp == NULL) {
	 for (last_amp = amp; last_amp->next; last_amp = last_amp->next);	/* empty body */
      }
      if (first_filename == NULL) {
	 first_filename = StringSave (filename);
      }
      if (!*last_comma)
	 break;
   }

   if (has_trouble)
      return (1);


   acip ->  loadname = first_filename;
   acip ->  filename = asnargs[outnamearg].strvalue;
   acip ->  do_bit_twiddle = asnargs[bittwiddlearg].intvalue;
   acip ->  include_filename = asnargs[includearg].strvalue;
   acip ->  amp = amp ;
   acip ->  last_amp = last_amp;

   AsnCode(acip);

   if (asnloadWasModified) {	/* restore ASNLOAD value */
      if (asnloadValExisted) {
	 SetAppParam (CFG_FILE_NAME, "NCBI", "ASNLOAD", asnloadOldVal);
      } else {
	 if (asnloadSectionExisted) {
	    SetAppParam (CFG_FILE_NAME, "NCBI", "ASNLOAD", NULL);
	 } else {
	    SetAppParam (CFG_FILE_NAME, "NCBI", NULL, NULL);
	 }
      }
   }
   return 0;
}
#endif


void AsnCodeHeader PROTO ((AsnIterPtr iter, CharPtr name));
void AsnCodeFunctionHeader PROTO ((AsnIterPtr iter, CharPtr name, CharPtr type));
Int2 AsnCodeSafeIsa PROTO ((AsnTypePtr atp));
void AsnCodeGenerateProtos PROTO ((struct struct_asniter PNTR iter, CharPtr buf, Boolean from_choice));
Int2 AsnCodeIterRecurLevel PROTO ((AsnIterPtr iter));
static Int2 AsnCodeFinalType PROTO ((struct struct_asniter PNTR iter, AsnTypePtr atp, AsnTypePtr PNTR base_atpPt));
static void userobj_OUTER_typedefs PROTO ((AsnIterPtr iter, CharPtr type, Boolean has_pointer));
static CharPtr AsnCodeNeedFree PROTO ((AsnTypePtr atp));
CharPtr AsnCodeWhichValSlot PROTO ((AsnTypePtr atp, CharPtr PNTR whichvalslot));
static void AsnIterTakeBufReal PROTO ((struct struct_asniter PNTR iter, int lineno));
static CharPtr AsnCodeLookupType PROTO ((AsnTypePtr atp));
static void userobj_and_basetype_SETOF PROTO ((AsnIterPtr iter, AsnTypePtr final_atp, Int2 recur_level, Int2 base_or_user));
void AsnCodeFinishedRoutine PROTO ((struct struct_asniter PNTR iter));
void AsnCodeNewRoutine PROTO ((struct struct_asniter PNTR iter));
void AsnCodeFunctionReadFuncChoiceStart PROTO ((AsnIterPtr iter, Boolean doing_outer_setof));
void AsnCodeFunctionReadFuncSeqStart PROTO ((AsnIterPtr iter, Boolean go_outer));
void AsnCodeFunctionWriteFuncChoiceStart PROTO ((AsnIterPtr iter, Boolean doing_outer_setof_choice));
void AsnCodeFunctionWriteFuncSeqStart PROTO ((AsnIterPtr iter, Boolean no_write_braces));
static Boolean AsnCodeIsReallyChoice PROTO ((AsnTypePtr atp));
static void AsnCode_SETOF_base_or_user PROTO(( AsnIterPtr iter, Int2 base_or_user));



DataVal         Dummy;


/*-----------AsnCodeSafeIsa()-------------*/

Int2
AsnCodeSafeIsa (AsnTypePtr atp)
{
   if (ISA_BASETYPE (atp->isa)) {
      return atp->isa;
   } else {
      return atp->type->isa;
   }

}


/*****************************************************************************
*
*   AsnCode(acip)
*
*
*****************************************************************************/
/*-------------- AsnCode_RightOuter()--------------*/

/*
 * In the case of a Pseudo User type (The unnamed CHOICE and SEQUENCE groups
 * that the author of this code has learned to love) the "owner" of the slot
 * count and some names is moved one level deeper for this extra
 * AsnIterCall's callback
 * 
 */
static AsnCodeNodePtr
AsnCode_RightOuter (AsnIterPtr iter, AsnCodeNodePtr outer)
{
   AsnCodeNodePtr  retval = outer;

   if (iter->stack->type == ASNITER_Pseudo_USER_TYPE) {
      retval = outer->outer;
   }
   return retval;
}

/*--------------- AsnCode_RightChoiceNode()--------*/

/*
 * SET OF CHOICE names are located one node level out from other CHOICE
 * names, so this function sets the correct node level.
 * 
 */

static AsnCodeNodePtr
AsnCode_RightChoiceNode (AsnIterPtr iter, AsnCodeNodePtr start_node, Boolean PNTR in_setof_choicePt)
{
   AsnCodeNodePtr  retval = start_node;
   AsnTypePtr      atp;

   if ((iter -> acip->debug_level) > 3) {
      AsnCodeShowStack (iter);
      fprintf ((iter -> acip -> bug_fp), "start at %s-%d,%d\n\n", TESTNIL (retval->slot_name),
	       (int) retval->recur_type, (int) retval->type);
   }
   if (in_setof_choicePt != NULL)
      *in_setof_choicePt = FALSE;

   atp = retval->atp;
   if (atp->type->isa == CHOICE_TYPE) {
      if (retval->outer) {
	 atp = retval->outer->atp;
	 if (atp != NULL)
	    if (atp->type != NULL)
	       if (atp->type->isa == SETOF_TYPE ||
		   atp->type->isa == SETOF_TYPE) {
		  retval = retval->outer;
		  if (in_setof_choicePt) {
		     *in_setof_choicePt = TRUE;
		     if ((iter -> acip->debug_level) > 3) {
			fprintf ((iter -> acip -> bug_fp), ">>>>>>>  YES(%s)\n", TESTNIL (retval->obj_name));
		     }
		  }
	       }
      }
   }
   if ((iter -> acip->debug_level) > 3) {
      fflush ((iter -> acip -> bug_fp));
   }
   return retval;
}


/****************************************************************************
*
*   AsnCodeIterCaseInChoice(iter)
*
* WASH == CLEAN_FOR_SLOT
* BUFF == CLEAN_FOR_OBJ_NAME
*
* This node is the slot in the choice.
* The outer node was the CHOICE
*****************************************************************************/
static void
AsnCodeIterCaseInChoice (struct struct_asniter PNTR iter)
{

   /*
    * JAE 4/6/94 AsnCodeNodePtr  which_node = AsnCode_RightChoiceNode (iter,
    * iter->stack->outer, NULL);
    */
   AsnCodeNodePtr  which_node = AsnCode_RightChoiceNode (iter,
		       AsnCode_RightOuter (iter, iter->stack->outer), NULL);

   sprintf (iter->buf, "      case %s_%s:\n",
	    which_node->obj_name,
	    iter->stack->slot_name);
   AsnIterTakeBuf (iter);
}

/****************************************************************************
*
*   AsnCodeGenEnumDefines(iter)
*
* Generates #defines for the values which an enumerated type may take on
*****************************************************************************/
static void
AsnCodeGenEnumDefines (struct struct_asniter PNTR iter)
{
   AsnValxNodePtr avtp;
   Char lclbuf[100];
   Boolean first = TRUE;

   if (iter != NULL && iter->atp != NULL && iter->atp->branch != NULL) {
      for (avtp = (AsnValxNodePtr) iter->atp->branch; avtp != NULL;
	   avtp = avtp->next) {
	 if (first) {
	    sprintf(iter->buf, "/* following #defines are for enumerated type, not used by object loaders */\n");
            AsnIterTakeBuf (iter);
	    first = FALSE;
	 }
	 sprintf (iter->buf, "#define ");
         AsnIterTakeBuf (iter);
	 if (iter->stack->outer != NULL)
	 {
	    sprintf (iter->buf, "%s_", AsnCodeCleanName(iter->stack->outer->atp->name, lclbuf, CLEAN_FOR_SLOT, iter -> acip -> maxDefineLength));
            AsnIterTakeBuf (iter);
	 }
	 sprintf (iter->buf, "%s_", AsnCodeCleanName (iter->atp->name, lclbuf, CLEAN_FOR_SLOT, (int) (iter -> acip -> maxDefineLength) ));
         AsnIterTakeBuf (iter);
	 sprintf (iter->buf, "%s %d\n", AsnCodeCleanName (avtp->name, lclbuf,
		  CLEAN_FOR_SLOT, iter -> acip -> maxDefineLength), avtp->intvalue);
         AsnIterTakeBuf (iter);
      }
      sprintf (iter->buf, "\n");
      AsnIterTakeBuf (iter);
   }
}

/****************************************************************************
*
*   AsnCodeGenBitDefines(iter)
*
* Generates #defines for bitmasks used in determining which optional fields
* were instantiated when reading the object ... this data is also used when
* determined which fields should be written when writing the object.  The
* bit-fields may be manipulated by the application programmer.
*****************************************************************************/
static void
AsnCodeGenBitDefines (struct struct_asniter PNTR iter)
{
   Char lclbuf[100];

    sprintf (iter->buf, "#define OB__");
    AsnIterTakeBuf (iter);

    if (iter->stack->outer != NULL) {
       sprintf (iter->buf, "%s_", AsnCodeCleanName(iter->stack->outer->atp->name, lclbuf, CLEAN_FOR_SLOT, iter -> acip -> maxDefineLength));
       AsnIterTakeBuf (iter);
    }

    sprintf (iter->buf, "%s %d\n", AsnCodeCleanName (iter -> atp->name, lclbuf,
             CLEAN_FOR_SLOT, iter -> acip -> maxDefineLength), iter -> stack -> outer -> opt_count ++);
    AsnIterTakeBuf (iter);
    sprintf (iter->buf, "\n");
    AsnIterTakeBuf (iter);
}


/*------------------------------CALLBACKS ARE HERE --------*/

/***************************
*
* basetype_SEQSET(iter,final_atp)
*
* We have a basetype within  a SEQUENCE ::= group (or SET ::=)
*
*  such as:
*
*    ParentSeq ::= SEQUENCE {
*  ... other stuff before
*     we_are_here SomeBaseType  <<<<< dealing with this thing
* --- other stuff after
************************/
static void
basetype_SEQSET (AsnIterPtr iter, AsnTypePtr final_atp)
{
   CharPtr         freetype;
   CharPtr         valslot;
   Boolean         has_default = FALSE;
   CharPtr         valslotdefine;


   valslot = AsnCodeWhichValSlot (final_atp->type, &valslotdefine);
   switch (iter->mode) {
   case ITER_ASSIGN:
   case ITER_CHECK_LIST:
      break;
   case ITER_OBJECTS:
      if (iter -> atp -> optional && iter -> do_opt_bits &&
	  StringCmp (valslot, "ptrvalue") != 0) {
	 AsnCodeGenBitDefines(iter);
      }
      StringCpy (iter->buf2, AsnCodeLookupType (final_atp));
      sprintf (iter->buf, "       %s ",
	       iter->buf2);
      AsnIterTakeBuf (iter);
      sprintf (iter->buf, "  %s;\n",
	       iter->stack->slot_name);
      AsnIterTakeBuf (iter);
      if (AsnCodeIsEnumType(iter->atp))
      {
         AsnCodeGenEnumDefines (iter);
      }
      break;
   case ITER_NEW:
      if (iter->atp->defaultvalue != NULL) {
	 if (StringCmp (valslot, "boolvalue") == 0
	     || StringCmp (valslot, "intvalue") == 0) {
	    sprintf (iter->buf2, "%d",
		     (int) iter->atp->defaultvalue->intvalue);
	    has_default = TRUE;
	 } else if (StringCmp (valslot, "ptrvalue") == 0) {
	    sprintf (iter->buf2, "\"%s\"",
		     (CharPtr) iter->atp->defaultvalue->name);
            has_default = TRUE;
	 } else if (StringCmp (valslot, "realvalue") == 0) {
#ifdef OS_MAC
	    /* sprintf (iter->buf2, "%lf",
		     (double) iter->atp->defaultvalue->realvalue); */
	    sprintf (iter->buf2, "%LG",
		     (double) iter->atp->defaultvalue->realvalue);
#else
	    sprintf (iter->buf2, "%lg",
		     (double) iter->atp->defaultvalue->realvalue);
#endif
	    has_default = TRUE;
	 }
	 if (has_default) {
	    sprintf (iter->buf, "        ptr -> %s = %s;\n",
		     iter->stack->slot_name,
		     iter->buf2);
	    AsnIterTakeBuf (iter);
	 }
      }
      break;
   case ITER_FREE:
      freetype = AsnCodeNeedFree (final_atp);
      if (freetype != NULL) {
	 sprintf (iter->buf, "        %s(ptr -> %s);\n",
		  freetype, iter->stack->slot_name);
	 AsnIterTakeBuf (iter);
      }
      break;
   case ITER_READ:
#ifdef EXTRA_READIF_BUG
      if (AsnCode_RightOuter (iter, iter->stack->outer) == 1) {
	 sprintf (iter->buf, "   atp = AsnReadId(aip,amp, atp);\n");
	 AsnIterTakeBuf (iter);
      }
#endif
      sprintf (iter->buf, "   if (atp == %s) {\n",	/* } */
	       GenerateTypeFromStack (iter, TRUE));
      AsnIterTakeBuf (iter);
      sprintf (iter->buf,
	    "    if ( AsnReadVal(aip, atp, &av) <= 0) {\ngoto erret;\n}\n");
      AsnIterTakeBuf (iter);
      sprintf (iter->buf, "      ptr -> %s = ",
	       iter->stack->slot_name);
      AsnIterTakeBuf (iter);
      sprintf (iter->buf, "av.%s;\n",
	       AsnCodeWhichValSlot (final_atp->type, NULL));
      AsnIterTakeBuf (iter);

      if (iter -> atp -> optional && iter -> do_opt_bits &&
	  StringCmp (valslot, "ptrvalue") != 0 &&
	  iter -> stack -> outer -> opt_count <= 31){
         sprintf (iter->buf, "      ptr -> OBbits__ |= 1<<%d;\n",
	          iter->stack -> outer -> opt_count++);
         AsnIterTakeBuf (iter);
      }


      sprintf (iter->buf, "   atp = AsnReadId(aip,amp, atp);\n");
      AsnIterTakeBuf (iter);
       /* { */ sprintf (iter->buf, "   }\n");
      AsnIterTakeBuf (iter);
      break;
   case ITER_WRITE:
      if (StringCmp (valslot, "ptrvalue") == 0) {
	 sprintf (iter->buf,
		  "   if (ptr -> %s != NULL) {\n",	/* } */
		  iter->stack->slot_name);
	 AsnIterTakeBuf (iter);
      } else if (iter -> atp -> optional && iter -> do_opt_bits &&
		 iter -> stack -> outer -> opt_count <=31 ){
         sprintf (iter->buf, 
                  "if (ptr -> %s || (ptr -> OBbits__ & (1<<%d) )){", /* } */
                  iter->stack->slot_name, iter->stack -> outer -> opt_count++);
         AsnIterTakeBuf (iter);
      }

      sprintf (iter->buf, "   av.%s = ptr -> %s;\n",
	       valslot, iter->stack->slot_name);
      AsnIterTakeBuf (iter);
      sprintf (iter->buf,
	       "   retval = AsnWrite(aip, %s,  &av);\n",
	       GenerateTypeFromStack (iter, TRUE));

      AsnIterTakeBuf (iter);

      if (StringCmp (valslot, "ptrvalue") == 0 ||
	  (iter -> atp -> optional && iter -> do_opt_bits &&
	   iter -> stack -> outer -> opt_count <= 31)) {
	 sprintf (iter->buf,
		   /* { */ "   }\n");
	 AsnIterTakeBuf (iter);
      }


   }

}

/***************************
*
* basetype_CHOICE(iter,final_atp)
*
* We have a basetype within  a CHOICE ::=
*
*  such as:
*
*    ParentSeq ::= CHOICE {
*  ... other stuff before
*     we_are_here SomeBaseObject  <<<<< dealing with this thing
* --- other stuff after
************************/
static void
basetype_CHOICE (AsnIterPtr iter, AsnTypePtr final_atp)
{
   CharPtr         freetype;
   CharPtr         valslot;
   CharPtr         valslotdefine;
   Boolean         in_setof_choice;
   AsnCodeNodePtr  which_node = AsnCode_RightChoiceNode (iter,
				      iter->stack->outer, &in_setof_choice);

   valslot = AsnCodeWhichValSlot (final_atp->type, &valslotdefine);
   switch (iter->mode) {
   case ITER_ASSIGN:
   case ITER_CHECK_LIST:
   case ITER_NEW:
      break;
   case ITER_OBJECTS:		/* no objects, as slot absorbed within
				 * data.ptrvalue of ValNode, used for Choice
				 * but need symbolic #defines for choice */
      sprintf (iter->buf, "#define %s_%s %d\n",
	       which_node->obj_name,
	       iter->stack->slot_name,
	       AsnCode_RightOuter (iter, iter->stack->outer)->slot_count);
      AsnIterTakeBuf (iter);
      if (AsnCodeIsEnumType(iter->atp))
      {
         AsnCodeGenEnumDefines(iter);
      }
      break;
   case ITER_FREE:
      freetype = AsnCodeNeedFree (final_atp);
      if (freetype != NULL) {
	 AsnCodeIterCaseInChoice (iter);
	 sprintf (iter->buf, "        %s(anp -> data.%s);\nbreak;\n",
		  freetype, valslot);
	 AsnIterTakeBuf (iter);
      }
      break;
   case ITER_WRITE:
      AsnCodeIterCaseInChoice (iter);
      sprintf (iter->buf,
	       "av.%s = anp->data.%s;\nretval = AsnWrite(aip, %s, &av);\n",
	       valslot, valslot, GenerateTypeFromStack (iter, TRUE));
      AsnIterTakeBuf (iter);
      sprintf (iter->buf, "            break;\n");
      AsnIterTakeBuf (iter);
      break;
   case ITER_READ:
      if (AsnCode_RightOuter (iter, iter->stack->outer)->slot_count != 1) {
	 sprintf (iter->buf,
		  " else ");
	 AsnIterTakeBuf (iter);
      }
      if ((iter -> acip->debug_level) > 3) {
	 AsnCodeShowStack (iter);
	 fflush ((iter -> acip -> bug_fp));
      }
      sprintf (iter->buf,
	       "if (atp == %s) {\n",	/* } */
	       GenerateTypeFromStack (iter, TRUE));
      AsnIterTakeBuf (iter);
      sprintf (iter->buf, "      choice = %s_%s;\n",
	       which_node->obj_name,
	       iter->stack->slot_name);
      AsnIterTakeBuf (iter);
      sprintf (iter->buf,
	       "if (AsnReadVal(aip, atp, &av) <= 0) {\ngoto erret;\n}\n");
      AsnIterTakeBuf (iter);
	 sprintf (iter->buf, "        anp->data.%s = av.%s;\n",
		  valslot, valslot);
      AsnIterTakeBuf (iter);
       /* { */ sprintf (iter->buf, "  }\n   ");
      AsnIterTakeBuf (iter);
      break;
   }

}


/***************************
*
* basetype_SETOF(iter,final_atp,recur_level)
*
* We have a SET OF user object within  a CHOICE ::=  or a SEQUENCE ::=
*
*  such as:
*
*    ParentSeq ::= CHOICE {
*          or
*    ParentSeq ::= SEQUENCE {
*  ... other stuff before
*     we_are_here SETOF SomeUserObject  <<<<< dealing with this thing
* --- other stuff after
************************/
static void
basetype_SETOF (AsnIterPtr iter, AsnTypePtr final_atp, Int2 recur_level)
{
   userobj_and_basetype_SETOF (iter, final_atp, recur_level,
			       ASNITER_BASE_TYPE);
}


/***************************
*
* AsnCode_basetype(iter)
*
****************************/
static void
AsnCode_basetype (struct struct_asniter PNTR iter)
{
   Int2            recur_level = AsnCodeIterRecurLevel (iter);

   switch (recur_level) {
   case RECUR_OUTER:
      if (iter->mode == ITER_OBJECTS && AsnCodeIsEnumType(iter->atp)) {
	 AsnCodeGenEnumDefines(iter);
      }
      break;
   case RECUR_SEQSET:		/* basetype in a SEQUENCE or SET */
      basetype_SEQSET (iter, iter->atp);
      break;
   case RECUR_CHOICE:		/* basetype in a CHOICE */
      basetype_CHOICE (iter, iter->atp);
      break;
   case RECUR_SETOF_OUTER:
/*  as     SetOfInteger ::= SET OF INTEGER */
			AsnCode_SETOF_base_or_user (iter, ASNITER_BASE_TYPE);
      break;
   case RECUR_SETOF_SEQSET:
   case RECUR_SETOF_CHOICE:

      /*
       * basetypes within SETOF in parental SEQUENCE or CHOICE the name is at
       * the one level outer
       */
      basetype_SETOF (iter, iter->stack->outer->atp, recur_level);
      break;
   }
}



/***************************
*
* AsnCode_choiceend(iter)
*
*
* action the same at all RecurLevels
*
****************************/
static void
AsnCode_choiceend (struct struct_asniter PNTR iter)
{
   Boolean         in_setof_choice;
   AsnCodeNodePtr  which_node = AsnCode_RightChoiceNode (iter,
					     iter->stack, &in_setof_choice);
   char            setof_buf[120];

   if ((iter -> acip->debug_level) >= 2) {
      fprintf ((iter -> acip -> bug_fp), "  In AsnCode_choiceend, ASSIGN mode %dat  type = (%d)\n",
	       iter->mode, (int) iter->atp->isa);
   }
   switch (iter->mode) {
   case ITER_ASSIGN:
      break;
   case ITER_CHECK_LIST:
      break;
   case ITER_OBJECTS:

      /*--------- PROTO TYPES ----------*/
      if (in_setof_choice) {
	 sprintf (setof_buf, "%s_element", which_node->obj_name);
	 AsnCodeGenerateProtos (iter, setof_buf, TRUE);
      } else {
	 AsnCodeGenerateProtos (iter, which_node->obj_name, TRUE);
      }
      AsnCodeFinishedRoutine (iter);
      if (in_setof_choice) {
	 AsnCodeNewRoutine (iter);
	 sprintf (iter->buf,
		  "typedef ValNodePtr %sPtr;\n",
		  which_node->obj_name);
	 AsnIterTakeBuf (iter);
	 sprintf (iter->buf,
		  "typedef ValNode %s;\n",
		  which_node->obj_name);
	 AsnIterTakeBuf (iter);

	 AsnCodeGenerateProtos (iter, which_node->obj_name, TRUE);
	 AsnCodeFinishedRoutine (iter);
      };
      break;

   case ITER_NEW:		/* valNode */
      break;
   case ITER_FREE:
       /* { { */ sprintf (iter->buf, "    }\n\
	return MemFree(anp);\n}\n");

      AsnIterTakeBuf (iter);
      AsnCodeFinishedRoutine (iter);
      if (in_setof_choice) {
	 AsnCodeNewRoutine (iter);
	 AsnCodeFunctionHeader (iter, "Free",
				which_node->obj_name);
	 sprintf (iter->buf,
		  "ValNodePtr anp)\n{\n\n" /* } */ );
	 AsnIterTakeBuf (iter);
	 sprintf (iter->buf, "\
             if (anp == NULL) {\n\
        return NULL;\n}\n\
\n");
	 AsnIterTakeBuf (iter);
	 sprintf (iter->buf,
	  " AsnGenericChoiceSeqOfFree(anp, (AsnOptFreeFunc) %s_elementFree);",
		  which_node->obj_name);
	 AsnIterTakeBuf (iter);

	  /* { { */ sprintf (iter->buf, "    \n\
	return NULL;\n}\n");

	 AsnIterTakeBuf (iter);

	 AsnCodeFinishedRoutine (iter);
      }
      break;
   case ITER_READ:

      /* { */
      sprintf (iter->buf, "\
    anp->choice = choice;\n\
    if (func != NULL)\n\
    {\n\
        anp->data.ptrvalue = (* func)(aip, atp);\n\
				if (aip -> io_failure) goto erret;\n\n\
        if (nullIsError && anp->data.ptrvalue == NULL) {\n\
            goto erret;\n}\n\
    }\n\n\
ret:\n\
	AsnUnlinkType(orig);       /* unlink local tree */\n\
	return anp;\n\n\
erret:\n\
    anp = MemFree(anp);\n\
		aip -> io_failure = TRUE;\n\
    goto ret;\n\
}\n");


      AsnIterTakeBuf (iter);

      AsnCodeFinishedRoutine (iter);
      if (in_setof_choice) {
	 AsnCodeNewRoutine (iter);
	 AsnCodeFunctionHeader (iter, "AsnRead", which_node->obj_name);
	 sprintf (iter->buf,
		  "AsnIoPtr aip, AsnTypePtr orig)\n{\n \n\n");	/* } */
	 AsnIterTakeBuf (iter);

	 sprintf (iter->buf, "\
	DataVal av;\n\
	AsnTypePtr atp;\n\
    ValNodePtr anp;\n\
    Uint1 choice;\n\
    Boolean isError = FALSE;\n\
    AsnReadFunc func;\n\n");
	 AsnIterTakeBuf (iter);
	 AsnCodeFunctionReadFuncChoiceStart (iter, TRUE);	/* making extra SET of
								 * header */

	 sprintf (iter->buf, "\
           anp =\n\
               AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError,\n\
                 (AsnReadFunc) %s_elementAsnRead, (AsnOptFreeFunc) %s_elementFree);\n\
		if (isError) \ngoto erret;\n",
		  which_node->obj_name,
		  which_node->obj_name);
	 AsnIterTakeBuf (iter);

	 sprintf (iter->buf, "\
    \n\n\
ret:\n\
	AsnUnlinkType(orig);       /* unlink local tree */\n\
	return anp;\n\n\
erret:\n\
		aip -> io_failure = TRUE;\n\
    goto ret;\n\
}\n");
	 AsnIterTakeBuf (iter);


	 AsnCodeFinishedRoutine (iter);
      }
      break;
   case ITER_WRITE:

      /* { { */
      sprintf (iter->buf, "\
          }\n\
    if (writetype != NULL) {\n\
        retval = (* func)(pnt, aip, writetype);   /* write it out */\n}\n\
        if (!retval) {\ngoto erret;\n}\n        retval = TRUE;\n\nerret:\n\
	AsnUnlinkType(orig);       /* unlink local tree */\n\
	return retval;\n\
}\n");

      AsnIterTakeBuf (iter);
      AsnCodeFinishedRoutine (iter);
      if (in_setof_choice) {
	 AsnCodeNewRoutine (iter);
	 AsnCodeNewRoutine (iter);
	 AsnCodeFunctionHeader (iter, "AsnWrite", which_node->obj_name);
	 sprintf (iter->buf,
		  "ValNodePtr anp, AsnIoPtr aip, AsnTypePtr orig)\n\n{\n");	/* } */

	 AsnIterTakeBuf (iter);
	 sprintf (iter->buf, "\
	DataVal av;\n\
	AsnTypePtr atp, writetype = NULL;\n\
    Pointer pnt;\n\
    AsnWriteFunc func = NULL;\n\
    Boolean retval = FALSE;\n\n");
	 AsnIterTakeBuf (iter);
	 AsnCodeFunctionWriteFuncChoiceStart (iter, TRUE);	/* extra call */

	 sprintf (iter->buf,
		  "      retval = AsnGenericChoiceSeqOfAsnWrite(anp, \n\
         (AsnWriteFunc) %s_elementAsnWrite, aip, atp, %s);",
		  which_node->obj_name,
		  GenerateTypeFromStack (iter, TRUE));
	 AsnIterTakeBuf (iter);



	  /* {    */ sprintf (iter->buf, "\
\nerret:\n\
	AsnUnlinkType(orig);       /* unlink local tree */\n\
	return retval;\n\
}\n");

	 AsnIterTakeBuf (iter);


	 AsnCodeFinishedRoutine (iter);
      }
      break;
   }

}

/***************************
*
* AsnCode_choicestart(iter)
* ****************************/

static void
AsnCode_choicestart (struct struct_asniter PNTR iter)
{
   Int2            recur_level = AsnCodeIterRecurLevel (iter);
   DataVal         auto_hint;
   Boolean         in_setof_choice;
   AsnCodeNodePtr  which_node = AsnCode_RightChoiceNode (iter,
					     iter->stack, &in_setof_choice);
   char            setof_buf[120];
   CharPtr         func_name = which_node->obj_name;

   if ((iter -> acip->debug_level) > 3) {
      fprintf ((iter -> acip -> bug_fp), "  In AsnCode_choicestart,  type = (%d)\n",
	       (int) iter->atp->isa);
   }
   if (recur_level != RECUR_OUTER
       && recur_level != RECUR_SETOF_OUTER) {
      /*------if not at outer level, is internal and pseudo-user---*/
      AsnCodeIterCall (iter, iter->userobj, iter->atp,
		       ASNITER_Pseudo_USER_TYPE);
   }
   if (in_setof_choice) {
      sprintf (setof_buf, "%s_element", func_name);
      func_name = setof_buf;
   }
   if (iter->mode == ITER_ASSIGN) {
      /********************
      *
      * choicestart_ASSIGN
      *
      * At the start of a CHOICE :: and seeing if a name
      * must be assigned. (mainly for #defines....
      *
      * If at anything but top level and isa is zero, is an
      * unnamed sequence, so it must
      **********************/

      switch (recur_level) {
      case RECUR_OUTER:
	 break;
      case RECUR_SETOF_OUTER:
	 /*-treat SET OF CHOICE specially */
	 if ((iter -> acip->debug_level) >= 2)
	    AsnCodeShowDepth (iter);

	 if ((iter -> acip->debug_level) > 0) {
	    fprintf ((iter -> acip -> bug_fp), "  In AsnCode_choicestart, ASSIGN mode at %s, type = (%d)\n",
		     TESTNIL (func_name), (int) iter->atp->isa);
	    if (iter->atp->isa == 0) {
	       fprintf ((iter -> acip -> bug_fp), "TRY NAME: %s_choice\n", TESTNIL (func_name));
	    }
	 }
	 if (iter->atp->isa == 0) {
	    sprintf (iter->buf, "%s_choice", func_name);
	    auto_hint.ptrvalue = StringSave (iter->buf);
	    AsnOptionNew (&(iter->atp->hints),
			  OP_NCBIASNTOOL, OP_SLOT_NAME, auto_hint,
			  Nlm_MemFree);
	    sprintf (iter->buf, "%s_choice", func_name);
	    auto_hint.ptrvalue = StringSave (iter->buf);
	    AsnOptionNew (&(iter->atp->hints),
			  OP_NCBIASNTOOL, OP_OBJ_NAME, auto_hint,
			  Nlm_MemFree);
	 }
	 break;
      case RECUR_SEQSET:
      case RECUR_CHOICE:
      case RECUR_SETOF_SEQSET:
      case RECUR_SETOF_CHOICE:
	 if ((iter -> acip->debug_level) >= 2)
	    AsnCodeShowDepth (iter);

	 if ((iter -> acip->debug_level) > 0) {
	    fprintf ((iter -> acip -> bug_fp), "  In AsnCode_choicestart, ASSIGN mode at %s, type = (%d)\n",
		     TESTNIL (func_name), (int) iter->atp->isa
	       );
	    if (iter->atp->isa == 0) {
	       fprintf ((iter -> acip -> bug_fp), "TRY NAME: %s_%s\n",
		      TESTNIL (func_name), TESTNIL (which_node->slot_name));
	    }
	 }
	 if (iter->atp->isa == 0) {
	    sprintf (iter->buf, "%s_%s",
		     func_name, which_node->slot_name);
	    auto_hint.ptrvalue = StringSave (iter->buf);
	    AsnOptionNew (&(iter->atp->hints),
			  OP_NCBIASNTOOL, OP_SLOT_NAME, auto_hint,
			  Nlm_MemFree);
	    sprintf (iter->buf, "%s_%s",
		     func_name, which_node->slot_name);
	    auto_hint.ptrvalue = StringSave (iter->buf);
	    AsnOptionNew (&(iter->atp->hints),
			  OP_NCBIASNTOOL, OP_OBJ_NAME, auto_hint,
			  Nlm_MemFree);
	 }
      }
   } else {
      switch (iter->mode) {
      case ITER_ASSIGN:
	 break;
      case ITER_CHECK_LIST:
	 break;
      case ITER_OBJECTS:

	 /*
	  * ValNode, but need to match from with choice end because of
	  * #defines and prototype generation.
	  */
	 AsnCodeNewRoutine (iter);
	 if (iter->atp->isa == 0) {
	    sprintf (iter->buf, "\n#ifdef NLM_GENERATED_CODE_PROTO\n\n");
	    AsnIterTakeBuf (iter);
	 }
	 sprintf (iter->buf,
		  "typedef ValNodePtr %sPtr;\n",
		  func_name);
	 AsnIterTakeBuf (iter);
	 sprintf (iter->buf,
		  "typedef ValNode %s;\n",
		  func_name);
	 AsnIterTakeBuf (iter);
	 if (iter->atp->isa == 0) {
	    sprintf (iter->buf, "\n#endif /* NLM_GENERATED_CODE_PROTO */\n\n");
	    AsnIterTakeBuf (iter);
	 }
	 break;
      case ITER_NEW:		/* ValNode */
	 break;
      case ITER_FREE:
	 AsnCodeNewRoutine (iter);
	 AsnCodeFunctionHeader (iter, "Free", func_name);
	 sprintf (iter->buf,
		  "ValNodePtr anp)\n{\n   Pointer pnt;\n\n" /* } */ );
	 AsnIterTakeBuf (iter);
	 sprintf (iter->buf, "\
             if (anp == NULL) {\n\
        return NULL;\n}\n\
\n\
    pnt = anp->data.ptrvalue;\n\
    switch (anp->choice)\n\
    {\n        default:\nbreak;\n");	/* } */
	 AsnIterTakeBuf (iter);
	 break;
      case ITER_READ:
	 AsnCodeNewRoutine (iter);
	 AsnCodeFunctionHeader (iter, "AsnRead", func_name);
	 sprintf (iter->buf,
		  "AsnIoPtr aip, AsnTypePtr orig)\n{\n");	/* } */
	 AsnIterTakeBuf (iter);

	 sprintf (iter->buf, "\
	DataVal av;\n\
	AsnTypePtr atp;\n\
    ValNodePtr anp;\n\
    Uint1 choice;\n\
    Boolean isError = FALSE;\n\
		Boolean nullIsError = FALSE;\n\
    AsnReadFunc func;\n\n");
	 AsnIterTakeBuf (iter);
	 AsnCodeFunctionReadFuncChoiceStart (iter, FALSE);

	 break;
      case ITER_WRITE:
	 AsnCodeNewRoutine (iter);
	 AsnCodeFunctionHeader (iter, "AsnWrite", func_name);
	 sprintf (iter->buf,
		  "%sPtr anp, AsnIoPtr aip, AsnTypePtr orig)\n\n{\n",
		  func_name);	/* } */

	 AsnIterTakeBuf (iter);
	 sprintf (iter->buf, "\
	DataVal av;\n\
	AsnTypePtr atp, writetype = NULL;\n\
    Pointer pnt;\n\
    AsnWriteFunc func = NULL;\n\
    Boolean retval = FALSE;\n\n");
	 AsnIterTakeBuf (iter);
	 AsnCodeFunctionWriteFuncChoiceStart (iter, FALSE);

	 sprintf (iter->buf, "\
    pnt = anp->data.ptrvalue;\n\
    switch (anp->choice)\n\
    {\n");			/* } */
	 AsnIterTakeBuf (iter);

      }

   }

}

/*************************************
*
*   AsnIterGenericDie301(iter)
*
****************************************/

static void
AsnIterGenericDie301 (struct struct_asniter PNTR iter)
{
   ErrPost (CTX_NCBIASN1, 101, "Type for %s is < 301 (%d)",
	    iter->atp->name, (int) (iter->atp->type->isa));
   exit (1);
}







/***************************
*
* AsnCode_SETOF_base_or_user(iter,base_or_user)
*
****************************/
static void
AsnCode_SETOF_base_or_user (AsnIterPtr iter, Int2 base_or_user)
{
   CharPtr         valslotdefine;
   CharPtr         valslot;
   Int2            real_type;
   AsnTypePtr      final_atp;
   Char            asntypename[128];

   sprintf (asntypename,
	    "%s",
	    GenerateTypeFromStack (iter, FALSE));
	 real_type = AsnCodeFinalType (iter, iter->atp, &final_atp);
   if (base_or_user == ASNITER_BASE_TYPE) {
      if (AsnCodeIsEnumType (final_atp)) {
	 valslot = "intvalue";
	 valslotdefine = defint;
      } else {
	 if (final_atp->type->isa == SETOF_TYPE ||
	     final_atp->type->isa == SEQOF_TYPE) {
	    valslot = AsnCodeWhichValSlot
	       (((AsnTypePtr) final_atp->branch)->type,
		&valslotdefine);
	 } else {
	    valslot = AsnCodeWhichValSlot
	       ((AsnTypePtr) final_atp->type, &valslotdefine);
	 }
      }
   }
      switch (iter->mode) {
      case ITER_ASSIGN:
      case ITER_CHECK_LIST:
	 break;
      case ITER_OBJECTS:
	 AsnCodeNewRoutine (iter);
	 AsnCodeHeader (iter, iter->stack->outer -> obj_name);
	 if (base_or_user == ASNITER_BASE_TYPE ||
	     iter->stack->outer -> atp -> branch == NULL ||
	     AsnCodeIsReallyChoice(iter -> atp)) {
	    sprintf (iter->buf, "typedef ValNode %s;\n",
		     iter->stack->outer->obj_name);
	    AsnIterTakeBuf (iter);
	    sprintf (iter->buf, "typedef ValNodePtr %sPtr;\n",
	             iter->stack->outer->obj_name);
	    AsnIterTakeBuf (iter);
	    sprintf (iter->buf, "#define %sNew() ValNodeNew(NULL) \n",
		     iter->stack->outer->obj_name);
	    AsnIterTakeBuf (iter);
	    if (AsnOptionGet (iter->atp->hints,
			      OP_NCBIASNTOOL, OP_MAKE_LINKED, NULL) != NULL) {
	       sprintf (iter->buf, "        struct struct_%s PNTR next;\n",
			iter->stack->outer->obj_name);
	       AsnIterTakeBuf (iter);
	    }
	 } else {
	    AsnTypePtr atpb = (AsnTypePtr) iter->stack->outer -> atp -> branch;
	    Char local_buf[60];

	    AsnCodeCleanName (atpb -> type->name,
			      local_buf, CLEAN_FOR_SLOT, iter -> acip -> maxDefineLength);
	    sprintf (iter->buf, "typedef struct struct_%s %s;\n",
		     local_buf, iter->stack->outer->obj_name);
	    AsnIterTakeBuf (iter);
	    sprintf (iter->buf, "typedef struct struct_%s PNTR %sPtr;\n",
		     local_buf, iter->stack->outer->obj_name);
	    AsnIterTakeBuf (iter);
	    sprintf (iter->buf, "#define %sNew() %sNew() \n",
		     iter->stack->outer->obj_name, local_buf);
	    AsnIterTakeBuf (iter);
	 }
	 AsnCodeGenerateProtos(iter, iter->stack->outer->obj_name, FALSE);
	 break;
      case ITER_NEW:
/*  ValNode */
	 break;
      case ITER_FREE:
	 AsnCodeNewRoutine (iter);
	 AsnCodeFunctionHeader (iter, "Free", iter->stack->outer -> obj_name);
	 sprintf (iter->buf,
		  "%sPtr ptr)\n{\n\n    if(ptr == NULL) {\n\
        return NULL;\n}\n", iter->stack->outer->obj_name);	/* } */
	 AsnIterTakeBuf (iter);


	 if (base_or_user == ASNITER_BASE_TYPE) {
	    sprintf (iter->buf, "     AsnGenericBaseSeqOfFree(ptr," );
	    AsnIterTakeBuf (iter);
	    sprintf (iter->buf, "%s);\n",
		     valslotdefine);
	    AsnIterTakeBuf (iter);
	 } else {
	    /* user obj here */

	    if (AsnCodeIsReallyChoice (iter->atp)) {

	       sprintf (iter->buf,
            "      AsnGenericChoiceSeqOfFree(ptr," );
	       AsnIterTakeBuf (iter);
	    } else {

	       sprintf (iter->buf, "      AsnGenericUserSeqOfFree(ptr, " );
	       AsnIterTakeBuf (iter);
	    }

	    sprintf (iter->buf, " (AsnOptFreeFunc) %sFree);\n",
		     AsnCodeCleanName (final_atp->name,
				       iter->buf2, CLEAN_FOR_OBJ_NAME, iter -> acip -> maxDefineLength));
	    AsnIterTakeBuf (iter);
	 }

         sprintf (iter->buf,
		  /* { */ "return NULL;\n}\n");
         AsnIterTakeBuf (iter);
         AsnCodeFinishedRoutine (iter);
	 break;
      case ITER_READ:
	 AsnCodeNewRoutine (iter);
	 AsnCodeFunctionHeader (iter, "AsnRead", iter->stack->outer -> obj_name);
	 sprintf (iter->buf,
		  "AsnIoPtr aip, AsnTypePtr orig)\n\
{\n\
DataVal av;\n\
	AsnTypePtr atp;\n\
    Boolean isError = FALSE;\n\
	AsnReadFunc func;\n\
    %sPtr ptr;\n\n", iter->stack->outer->obj_name);	/* } */
	 AsnIterTakeBuf (iter);

	 AsnCodeFunctionReadFuncSeqStart (iter, TRUE);
	 sprintf (iter->buf, "      ptr  = ");
	 AsnIterTakeBuf (iter);
	 if (base_or_user == ASNITER_BASE_TYPE) {
	    sprintf (iter->buf,
		     "AsnGenericBaseSeqOfAsnRead(aip, amp, atp, %s, &isError);\n ",
		     valslotdefine);
	    AsnIterTakeBuf (iter);
	 } else {
	    sprintf (iter->buf,
		     "AsnGeneric%sSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ",
		     AsnCodeIsReallyChoice (iter->atp) ? "Choice" : "User");
	    AsnIterTakeBuf (iter);
	    sprintf (iter->buf, "%sAsnRead,",
		     AsnCodeCleanName (final_atp->name,
				       iter->buf2, CLEAN_FOR_OBJ_NAME, iter -> acip -> maxDefineLength));
	    AsnIterTakeBuf (iter);
	    sprintf (iter->buf,
		     " (AsnOptFreeFunc) ");
	    AsnIterTakeBuf (iter);
	    sprintf (iter->buf, "%sFree);\n",
		     AsnCodeCleanName (final_atp->name,
				       iter->buf2, CLEAN_FOR_OBJ_NAME, iter -> acip -> maxDefineLength));
	    AsnIterTakeBuf (iter);
	 }
	 sprintf (iter->buf,
		  "      if (isError && ptr  == NULL) {\ngoto erret;\n}\n" );
	 AsnIterTakeBuf (iter);

         /* { */ sprintf (iter->buf, "\n\n\nret:\n\
	AsnUnlinkType(orig);       /* unlink local tree */\n\
	return ptr;\n\
\nerret:\n\
		aip -> io_failure = TRUE;\n\
    ptr = %sFree(ptr);\n\
    goto ret;\n\
}\n\n", iter->stack->outer->obj_name);
      AsnIterTakeBuf (iter);
      AsnCodeFinishedRoutine (iter);

	 break;
      case ITER_WRITE:
	 AsnCodeNewRoutine (iter);
	 AsnCodeFunctionHeader (iter, "AsnWrite", iter->stack->outer -> obj_name);

	 sprintf (iter->buf,
		  "%sPtr ptr, AsnIoPtr aip, AsnTypePtr orig)\n\
{\n\
	DataVal av;\n\
	AsnTypePtr atp;\n\
    Boolean retval = FALSE;\n\n",
		  iter->stack->outer->obj_name);	/* } */
	 AsnIterTakeBuf (iter);

	 AsnCodeFunctionWriteFuncSeqStart (iter, TRUE);

	 if (base_or_user == ASNITER_BASE_TYPE) {
	    sprintf (iter->buf,
          "     retval = AsnGenericBaseSeqOfAsnWrite(ptr, " );
	    AsnIterTakeBuf (iter);
	    sprintf (iter->buf, "%s",
		     valslotdefine);
	    AsnIterTakeBuf (iter);
	       sprintf (iter->buf, ", aip, atp, %s);\n",
			Append_EAndPrestrip (asntypename, iter -> acip -> maxDefineLength));
	    AsnIterTakeBuf (iter);
	 } else {
	    /* user obj here */

	    if (AsnCodeIsReallyChoice (iter->atp)) {

	       sprintf (iter->buf,
         "      retval = AsnGenericChoiceSeqOfAsnWrite(ptr ," );
	       AsnIterTakeBuf (iter);
	    } else {

	       sprintf (iter->buf, 
           "      retval = AsnGenericUserSeqOfAsnWrite(ptr ," );
	       AsnIterTakeBuf (iter);
	    }
	       sprintf (iter->buf, " (AsnWriteFunc) %sAsnWrite, aip, atp, %s);\n",
			AsnCodeCleanName (final_atp->name,
					  iter->buf2, CLEAN_FOR_OBJ_NAME, iter -> acip -> maxDefineLength),
			Append_EAndPrestrip (asntypename, iter -> acip -> maxDefineLength));
	    AsnIterTakeBuf (iter);
	 }

#ifdef EXTRA_READ_VAL_BUG_FROM_STRING
Hey guy, this is not asncode code, but from the sprintf string below;
	if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {\n\
        goto erret;\n}\n\

#endif

       /* { */ sprintf (iter->buf,"\
    retval = TRUE;\n\nerret:\n\
	AsnUnlinkType(orig);       /* unlink local tree */\n\
	return retval;\n\
}\n\n");
      AsnIterTakeBuf (iter);
      AsnCodeFinishedRoutine (iter);


	 break;
      }
}






/***************************
*
* AsnCode_seqend(iter)
*
****************************/
static void
AsnCode_seqend (struct struct_asniter PNTR iter)
{
   switch (iter->mode) {
      case ITER_ASSIGN:
      break;
   case ITER_CHECK_LIST:
      break;
   case ITER_OBJECTS:
       /* { */ sprintf (iter->buf, "} %s, PNTR %sPtr;\n",
			iter->stack->obj_name, iter->stack->obj_name);
      AsnIterTakeBuf (iter);
      if (iter->atp->isa == 0) {
	 sprintf (iter->buf, "#endif /* NLM_GENERATED_CODE_PROTO */\n");
	 AsnIterTakeBuf (iter);
      }
      AsnCodeGenerateProtos (iter, iter->stack->obj_name, FALSE);
      AsnCodeFinishedRoutine (iter);
      break;
   case ITER_NEW:
       /* { */ sprintf (iter->buf, "    return ptr;\n\n}\n");
      AsnIterTakeBuf (iter);
      AsnCodeFinishedRoutine (iter);

      break;
   case ITER_FREE:
       /* { */ sprintf (iter->buf,
			"return MemFree(ptr);\n}\n");
      AsnIterTakeBuf (iter);
      AsnCodeFinishedRoutine (iter);
      break;
   case ITER_READ:
       /* { */ sprintf (iter->buf,
			"\n\
    if (AsnReadVal(aip, atp, &av) <= 0) {\ngoto erret;\n}\n   /* end struct */\n\
\nret:\n\
	AsnUnlinkType(orig);       /* unlink local tree */\n\
	return ptr;\n\
\nerret:\n\
		aip -> io_failure = TRUE;\n\
    ptr = %sFree(ptr);\n\
    goto ret;\n\
}\n\n", iter->stack->obj_name);
      AsnIterTakeBuf (iter);
      AsnCodeFinishedRoutine (iter);
      break;
   case ITER_WRITE:
       /* { */ sprintf (iter->buf,
			"if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {\n\
        goto erret;\n}\n\
    retval = TRUE;\n\nerret:\n\
	AsnUnlinkType(orig);       /* unlink local tree */\n\
	return retval;\n\
}\n\n");
      AsnIterTakeBuf (iter);
      AsnCodeFinishedRoutine (iter);

      break;
   }

}

/***************************
*
* AsnCode_seqstart(iter)
*
****************************/
static void
AsnCode_seqstart (struct struct_asniter PNTR iter)
{
   DataVal         auto_hint;
   Int2            recur_level = AsnCodeIterRecurLevel (iter);
   CharPtr         try_name = NULL, safe_name=NULL,temp;
   AsnCodeNodePtr  safe_node = NULL;
	  static int      fail_count=0;
		char      fail_buf[12];
		

   if (recur_level != RECUR_OUTER
       && recur_level != RECUR_SETOF_OUTER) {
      /*------if not at outer level, is internal and pseudo-user---*/
      AsnCodeIterCall (iter, iter->userobj, iter->atp,
		       ASNITER_Pseudo_USER_TYPE);
   }
   if (iter->mode == ITER_ASSIGN) {
      /********************
      *
      * seqstart_ASSIGN
      *
      * At the start of a SEQUENCE :: and seeing if a name
      * must be assigned.
      *
      * If at anything but top level and isa is zero, is an
      * unnamed sequence, so it must
      **********************/

      switch (recur_level) {
      case RECUR_OUTER:
	 break;
      case RECUR_SEQSET:
      case RECUR_CHOICE:
      case RECUR_SETOF_OUTER:
      case RECUR_SETOF_SEQSET:
      case RECUR_SETOF_CHOICE:
	 if ((iter->acip->debug_level) >= 2)
	    AsnCodeShowDepth (iter);

	 if (iter->atp->isa == 0) {
	    if (iter->stack->outer->obj_name == NULL) {
	       try_name = iter->stack->outer->outer->obj_name;
	    } else {
	       try_name = iter->stack->outer->obj_name;
	    }
		for (safe_node = iter->stack; safe_node; safe_node = safe_node -> outer){
			if (safe_node->obj_name){
				safe_name = StringSave(safe_node->obj_name);
				break;
			}
		}
		if ( ! safe_node){
			temp = safe_name = (char *)MemNew(30);
			sprintf(fail_buf,"%d",fail_count++);
			temp = StringMove(temp,"FAIL__NAME_");
			temp = StringMove(temp,fail_buf);
		}
	 }
	 if ((iter->acip->debug_level) > 0) {
	    fprintf ((iter->acip -> bug_fp), "  In AsnCode_seqstart, ASSIGN mode at %s, type = (%d)\n",
		     TESTNIL (iter->atp->name), (int) iter->atp->isa
	       );
	    if ((iter->acip->debug_level) > 3) {
	       AsnCodeShowStack (iter);
	    }
	    fprintf ((iter->acip -> bug_fp), "TRY seq NAME: %s_%s\n",
		     TESTNIL (try_name),
		     TESTNIL (  (safe_node -> obj_name)?safe_node -> obj_name:safe_name));
	 }
	 if (iter->atp->isa == 0) {
	    sprintf (iter->buf, "%s_%s",
		     TESTNIL (try_name),
		     TESTNIL (  (safe_node -> obj_name)?safe_node -> obj_name:safe_name));
	    auto_hint.ptrvalue = StringSave (iter->buf);
	    AsnOptionNew (&(iter->atp->hints),
			  OP_NCBIASNTOOL, OP_SLOT_NAME, auto_hint,
			  Nlm_MemFree);
	    sprintf (iter->buf, "%s_%s",
		     TESTNIL (try_name),
		     TESTNIL (  (safe_node -> slot_name)?safe_node -> slot_name:safe_name));
	    auto_hint.ptrvalue = StringSave (iter->buf);
	    AsnOptionNew (&(iter->atp->hints),
			  OP_NCBIASNTOOL, OP_OBJ_NAME, auto_hint,
			  Nlm_MemFree);
	 }
      }
		MemFree(safe_name);
   } else {
      switch (iter->mode) {	/* ITER_ASSIGN handled above */
      case ITER_CHECK_LIST:
	 break;
      case ITER_OBJECTS:
	 AsnCodeNewRoutine (iter);
	 AsnCodeHeader (iter, iter->stack->obj_name);
	 if (iter->atp->isa == 0) {
	    sprintf (iter->buf, "\n#ifdef NLM_GENERATED_CODE_PROTO\n\n");
	    AsnIterTakeBuf (iter);
	 }
	 sprintf (iter->buf, "typedef struct struct_%s {\n",
		  iter->stack->slot_name);	/* } */
	 AsnIterTakeBuf (iter);
	 if (AsnOptionGet (iter->atp->hints,
			   OP_NCBIASNTOOL, OP_MAKE_LINKED, NULL) != NULL) {
	    sprintf (iter->buf, "        struct struct_%s PNTR next;\n",
		     iter->stack->slot_name);
	    AsnIterTakeBuf (iter);
	 }
	 if (iter -> do_opt_bits){
		 sprintf (iter->buf, "	Uint4 OBbits__;\n");
		 AsnIterTakeBuf (iter);
	}
	 break;
      case ITER_NEW:
	 AsnCodeNewRoutine (iter);
	 AsnCodeFunctionHeader (iter, "New", iter->stack->obj_name);
	 sprintf (iter->buf, "void)\n{\n\
    %sPtr ptr = MemNew((size_t) sizeof(%s));\n\n",
		  iter->stack->obj_name,
		  iter->stack->obj_name);
	 AsnIterTakeBuf (iter);
	 /* } */

	 break;
      case ITER_FREE:
	 AsnCodeNewRoutine (iter);
	 AsnCodeFunctionHeader (iter, "Free", iter->stack->obj_name);
	 sprintf (iter->buf,
		  "%sPtr ptr)\n{\n\n    if(ptr == NULL) {\n\
        return NULL;\n}\n", iter->stack->obj_name);	/* } */
	 AsnIterTakeBuf (iter);
	 break;
      case ITER_READ:
	 AsnCodeNewRoutine (iter);
	 AsnCodeFunctionHeader (iter, "AsnRead", iter->stack->obj_name);
	 sprintf (iter->buf,
		  "AsnIoPtr aip, AsnTypePtr orig)\n\
{\n\
DataVal av;\n\
	AsnTypePtr atp;\n\
    Boolean isError = FALSE;\n\
	AsnReadFunc func;\n\
    %sPtr ptr;\n\n", iter->stack->obj_name);	/* } */
	 AsnIterTakeBuf (iter);

	 AsnCodeFunctionReadFuncSeqStart (iter, FALSE);
	 break;
      case ITER_WRITE:
	 AsnCodeNewRoutine (iter);
	 AsnCodeFunctionHeader (iter, "AsnWrite", iter->stack->obj_name);

	 sprintf (iter->buf,
		  "%sPtr ptr, AsnIoPtr aip, AsnTypePtr orig)\n\
{\n\
	DataVal av;\n\
	AsnTypePtr atp;\n\
    Boolean retval = FALSE;\n\n",
		  iter->stack->obj_name);	/* } */
	 AsnIterTakeBuf (iter);

	 AsnCodeFunctionWriteFuncSeqStart (iter, FALSE);
	 break;
      }
   }
}

/***************************
*
* AsnCode_setstart(iter)
*
****************************/
static void
AsnCode_setstart (struct struct_asniter PNTR iter)
{
   AsnCode_seqstart (iter);	/* for now . . . */
}

/***************************
*
* AsnCode_setend(iter)
*
****************************/
static void
AsnCode_setend (struct struct_asniter PNTR iter)
{
   AsnCode_seqend (iter);	/* for now . . . */
}


/***************************
*
* AsnCode_under301err(iter)
*
****************************/
static void
AsnCode_under301err (struct struct_asniter PNTR iter)
{

   if (iter->atp->type == NULL)
      ErrPost (CTX_NCBIASN1, 101, "Type undef 301 error at %s < 301 (no type)",
	       iter->atp->name);
   else
      ErrPost (CTX_NCBIASN1, 101, "Type undef 301 error at %s < 301 (%d)",
	       iter->atp->name, (int) (iter->atp->type->isa));
   exit (1);


}
/***************************
*
* AsnIterNeedsNullCheck(iter,final_atp)
*
***************************/
Boolean
AsnIterNeedsNullCheck(AsnIterPtr iter, AsnTypePtr final_atp)
{
	return FALSE;
}

/***************************
*
* userobj_OUTER_typedefs(iter,type,has_pointer)
*
* Should we have prototypes as macro #defines?
***************************/
static void
userobj_OUTER_typedefs (AsnIterPtr iter, CharPtr type, Boolean has_pointer)
{
   AsnCodeHeader (iter, iter->stack->obj_name);
   sprintf (iter->buf, "#define %s  %s\n",
	    iter->stack->obj_name, type);
   AsnIterTakeBuf (iter);
   if (has_pointer) {
      sprintf (iter->buf, "#define %sPtr  %sPtr\n",
	       iter->stack->obj_name, type);
      AsnIterTakeBuf (iter);
      /*--pseudo PROTOtypes as defines---*/
      sprintf (iter->buf, "#define %sFree  %sFree\n",
	       iter->stack->obj_name, type);
      AsnIterTakeBuf (iter);
      sprintf (iter->buf, "#define %sNew  %sNew\n",
	       iter->stack->obj_name, type);
      AsnIterTakeBuf (iter);
      sprintf (iter->buf, "#define %sAsnRead  %sAsnRead\n",
	       iter->stack->obj_name, type);
      AsnIterTakeBuf (iter);
      sprintf (iter->buf, "#define %sAsnWrite  %sAsnWrite\n",
	       iter->stack->obj_name, type);
      AsnIterTakeBuf (iter);
   }
}

/***************************
*
* userobj_CHECK_LIST(iter)
*
*  User objects are the only candidates for adding a next
* slot, and only if in setof.  So regardless of whether
* in SEQUENCE/SET, at OUTER level or in CHOICE, if we
* have a user object in a SETOF, add the option to note need
* to have a next slot.
************************/
static void
userobj_CHECK_LIST (AsnIterPtr iter)
{
   AsnTypePtr      branch = iter->atp;
   AsnTypePtr      this_type, use_type;
   Int2            recur_level = AsnCodeIterRecurLevel (iter);

   switch (recur_level) {
   case RECUR_OUTER:
   case RECUR_SEQSET:
   case RECUR_CHOICE:
      break;			/* no action, unless setof */
   case RECUR_SETOF_OUTER:
   case RECUR_SETOF_SEQSET:
   case RECUR_SETOF_CHOICE:

      if ((iter->acip->debug_level) >= 2)
	 AsnCodeShowDepth (iter);
      if ((iter->acip->debug_level) > 0) {
	 fprintf ((iter->acip -> bug_fp), "  In userobj CHECK LIST at %s, bag type = (%s)\n",
		  TESTNIL (iter->atp->name), TESTNIL (iter->atp->type->name)
	    );
      }
      if (ISA_BASETYPE (branch->isa))
	 return;
      this_type = branch->type;
      if (ISA_BASETYPE (this_type->isa) || this_type->isa == 0)
	 return;
      if (this_type->imported) {
	 if (this_type->type != (AsnTypePtr) NULL) {
	    use_type = this_type->type;
	 } else {
	    ErrPost (CTX_NCBIASN1, 101,
		     "userobj CHECK LIST Unresolved imported type at %s.%s",
		     TESTNIL (iter->atp->name), TESTNIL (this_type->name));
	    use_type = (AsnTypePtr) NULL;
	 }
      } else {
	 use_type = this_type;
      }
      if (use_type != NULL) {
	 if (AsnOptionGet (use_type->hints,
			   OP_NCBIASNTOOL, OP_MAKE_LINKED, NULL) == NULL) {

	    if ((iter->acip->debug_level) > 0) {
	       fprintf ((iter->acip -> bug_fp), "userobj CHECK LIST: adding next slot for %s\n",
			TESTNIL (use_type->name));
	    }
	    AsnOptionNew (&(use_type->hints),
			  OP_NCBIASNTOOL, OP_MAKE_LINKED, Dummy, NULL);
	 }
      }
   }				/* end switch */

}

/***************************
*
* userobj_OUTER(iter)
*
* We have a user object at the outer
* recursion level.  This can only be if aliasing, as:
*
* ThisUserObject ::= ThatThing
*
* OBJECTS: Generally handled as a typedef.
* REST:    no code, as "ThatThing" object to handle
*          However, note that if ThisUserObject used, the code
*          has to look up how to handle "ThatThing".
*          ThatThing's type can be found with the function:
*       AsnCodeFinalType (iter -> atp, &name_at_type)
*       "name_at_type" is the AsnTypePointer for ThatThing.
************************/
static void
userobj_OUTER (AsnIterPtr iter)
{
   AsnTypePtr      name_at_type;
   Int2            real_type;

   if (iter->mode == ITER_OBJECTS) {	/* other modes, no action */

      real_type = AsnCodeFinalType (iter, iter->atp, &name_at_type);
      AsnCodeCleanName (name_at_type->name, iter->buf2, BUFF, iter -> acip -> maxDefineLength);
      switch (real_type) {
      case ENUM_TYPE:
	 userobj_OUTER_typedefs (iter, "Int2", FALSE);
	 break;

      case SEQ_TYPE:
      case SET_TYPE:
	 userobj_OUTER_typedefs (iter, iter->buf2, TRUE);
	 break;
      case SEQOF_TYPE:
      case SETOF_TYPE:
      case CHOICE_TYPE:
	 userobj_OUTER_typedefs (iter, "ValNode", TRUE);
	 break;
      case ASNITER_BASE_TYPE:
	 userobj_OUTER_typedefs (iter, AsnCodeLookupType (name_at_type),
				 FALSE);
	 break;
      }
   }
}

/***************************
*
* userobj_SEQSET(iter,final_atp)
*
* We have a user object within  a SEQUENCE ::= group (or SET ::=)
*
*  such as:
*
*    ParentSeq ::= SEQUENCE {
*  ... other stuff before
*     we_are_here SomeUserObject  <<<<< dealing with this thing
* --- other stuff after
*
* for choice type, iter->do_chs_struct forces e.g., struct_Date PNTR instead of
* newer default ValNodePtr, so old hand-coded object loaders can be linked with.
*
************************/
static void
userobj_SEQSET (AsnIterPtr iter, AsnTypePtr final_atp)
{
	Char buf_local[60];

   AsnCodeCleanName (final_atp->name, iter->buf2, CLEAN_FOR_SLOT, iter -> acip -> maxDefineLength);
   switch (iter->mode) {
   case ITER_ASSIGN:
   case ITER_CHECK_LIST:
      break;
   case ITER_OBJECTS:
      if ((AsnCodeIsReallyChoice (iter->atp) && (! iter->do_chs_struct)) || (final_atp != NULL &&
	  final_atp->branch != NULL &&
	  ((AsnTypePtr) final_atp->branch)->name == NULL &&
	  ((AsnTypePtr) final_atp->branch)->type != NULL &&
	  ISA_BASETYPE(((AsnTypePtr) final_atp->branch)->type->isa))) {
	 sprintf (iter->buf, "       ValNodePtr ");
	 AsnIterTakeBuf (iter);
      } else {
				if ((final_atp != NULL &&
						  final_atp->branch != NULL &&
						  ((AsnTypePtr) final_atp->branch)->name == NULL &&
						  ((AsnTypePtr) final_atp->branch)->type != NULL &&
						  (((AsnTypePtr) final_atp->branch)->type->isa) > 400 )) {
				 sprintf (iter->buf, " struct struct_%s PNTR ",
						AsnCodeCleanName ( ((AsnTypePtr) final_atp->branch)-> type ->name, 
							buf_local, CLEAN_FOR_SLOT, iter -> acip -> maxDefineLength));
					}else{
				 sprintf (iter->buf, " struct struct_%s PNTR ",
							 iter->buf2);
					}
			 AsnIterTakeBuf (iter);
      }
      sprintf (iter->buf, "  %s;\n",
	       iter->stack->slot_name);
      AsnIterTakeBuf (iter);
      break;
   case ITER_NEW:

      /*
       * since user object, deafaults probably not really there, should they
       * be needed, see the basetype code
       */

      break;
   case ITER_FREE:
      sprintf (iter->buf, "        %sFree(ptr -> %s);\n",
	       (iter->stack->type == ASNITER_Pseudo_USER_TYPE) ?
	       iter->stack->obj_name :
	       AsnCodeCleanName (final_atp->name,
				 iter->buf2, CLEAN_FOR_OBJ_NAME, iter -> acip -> maxDefineLength),
	       iter->stack->slot_name);
      AsnIterTakeBuf (iter);
      break;
   case ITER_READ:
#ifdef EXTRA_READID_BUG
      AsnCode_RightOuter (iter, iter->stack->outer);
      ? ? ?
	 if (iter->stack->outer->slot_count == 1) {
	 sprintf (iter->buf, "   atp = AsnReadId(aip,amp, atp);\n");
	 AsnIterTakeBuf (iter);
      }
#endif
      if ((iter->acip->debug_level) > 3) {
	 AsnCodeShowStack (iter);
	 fflush ((iter->acip -> bug_fp));
      }
      sprintf (iter->buf, "   if (atp == %s) {\n",	/* } */
	       GenerateTypeFromStack (iter, TRUE));
      AsnIterTakeBuf (iter);
      sprintf (iter->buf, "      ptr -> %s = ",
	       iter->stack->slot_name);
      AsnIterTakeBuf (iter);
      sprintf (iter->buf, "%sAsnRead(aip, atp);\n",
	       (iter->stack->type == ASNITER_Pseudo_USER_TYPE) ?
	       iter->stack->obj_name :
	       AsnCodeCleanName (final_atp->name,
				 iter->buf2, CLEAN_FOR_OBJ_NAME, iter -> acip -> maxDefineLength));
      AsnIterTakeBuf (iter);
      sprintf (iter->buf,
	       "      if (aip -> io_failure) {\ngoto erret;\n}\n",
	       iter->stack->slot_name);
      AsnIterTakeBuf (iter);

      if (AsnIterNeedsNullCheck(iter, final_atp)) {
         sprintf (iter->buf,
	          "      if (ptr -> %s == NULL) {\ngoto erret;\n}\n",
	          iter->stack->slot_name);
         AsnIterTakeBuf (iter);
      }

      sprintf (iter->buf, "   atp = AsnReadId(aip,amp, atp);\n");
      AsnIterTakeBuf (iter);
       /* { */ sprintf (iter->buf, "   }\n");
      AsnIterTakeBuf (iter);
      break;
   case ITER_WRITE:
      sprintf (iter->buf,
	       "   if (ptr -> %s != NULL) {\n",	/* } */
	       iter->stack->slot_name);
      AsnIterTakeBuf (iter);

      sprintf (iter->buf,
	       "      if ( ! %sAsnWrite(ptr ->",
	       (iter->stack->type == ASNITER_Pseudo_USER_TYPE) ?
	       iter->stack->obj_name :
	       AsnCodeCleanName (final_atp->name,
				 iter->buf2, CLEAN_FOR_OBJ_NAME, iter -> acip -> maxDefineLength));
      AsnIterTakeBuf (iter);
      /* { */
      sprintf (iter->buf,
	       " %s, aip, %s)) {\n         goto erret;\n}\n   }\n",
	       iter->stack->slot_name,
	       GenerateTypeFromStack (iter, TRUE));
      AsnIterTakeBuf (iter);
   }

}

/***************************
*
* userobj_CHOICE(iter,final_atp)
*
* We have a user object within  a CHOICE ::=
*
*  such as:
*
*    ParentSeq ::= CHOICE {
*  ... other stuff before
*     we_are_here SomeUserObject  <<<<< dealing with this thing
* --- other stuff after
************************/
static void
userobj_CHOICE (AsnIterPtr iter, AsnTypePtr final_atp)
{
   Boolean         in_setof_choice;
   AsnCodeNodePtr  which_node = AsnCode_RightChoiceNode (iter,
			      AsnCode_RightOuter (iter, iter->stack->outer),
							 &in_setof_choice);

   AsnCodeCleanName (final_atp->name, iter->buf2, CLEAN_FOR_SLOT, iter -> acip -> maxDefineLength);
   switch (iter->mode) {
   case ITER_ASSIGN:
   case ITER_CHECK_LIST:
   case ITER_NEW:
      break;
   case ITER_OBJECTS:		/* no objects, as slot absorbed within
				 * data.ptrvalue of ValNode, used for Choice
				 * but need symbolic #defines for choice */
      sprintf (iter->buf, "#define %s_%s %d\n",
	       which_node->obj_name,
	       iter->stack->slot_name,
	       AsnCode_RightOuter (iter, iter->stack->outer)->slot_count);
      AsnIterTakeBuf (iter);
      break;
   case ITER_FREE:
      AsnCodeIterCaseInChoice (iter);
      sprintf (iter->buf, "        %sFree(anp -> data.ptrvalue);\n",
	       (iter->stack->type == ASNITER_Pseudo_USER_TYPE) ?
	       iter->stack->obj_name :
	       AsnCodeCleanName (final_atp->name,
				 iter->buf2, CLEAN_FOR_OBJ_NAME, iter -> acip -> maxDefineLength));
      AsnIterTakeBuf (iter);
      sprintf (iter->buf, "            break;\n");
      AsnIterTakeBuf (iter);
      break;
   case ITER_WRITE:
      AsnCodeIterCaseInChoice (iter);
      sprintf (iter->buf,
	       "   writetype = %s;\n",
	       GenerateTypeFromStack (iter, TRUE));
      AsnIterTakeBuf (iter);
      sprintf (iter->buf, "   func = (AsnWriteFunc) %sAsnWrite;\n",
	       (iter->stack->type == ASNITER_Pseudo_USER_TYPE) ?
	       iter->stack->obj_name :
	       AsnCodeCleanName (final_atp->name,
				 iter->buf2, CLEAN_FOR_OBJ_NAME, iter -> acip -> maxDefineLength));

      AsnIterTakeBuf (iter);
      sprintf (iter->buf, "            break;\n");
      AsnIterTakeBuf (iter);
      break;
   case ITER_READ:
      if (AsnCode_RightOuter (iter, iter->stack->outer)->slot_count != 1) {
	 sprintf (iter->buf,
		  " else ");
	 AsnIterTakeBuf (iter);
      }
      if ((iter->acip->debug_level) > 3) {
	 AsnCodeShowStack (iter);
	 fflush ((iter->acip -> bug_fp));
      }
      sprintf (iter->buf,
	       "if (atp == %s) {\n",	/* } */
	       GenerateTypeFromStack (iter, TRUE));
      AsnIterTakeBuf (iter);
      sprintf (iter->buf, "      choice = %s_%s;\n",
	       which_node->obj_name,
	       iter->stack->slot_name);
      AsnIterTakeBuf (iter);


      if (AsnIterNeedsNullCheck(iter, final_atp)) {
	 sprintf (iter->buf,
	          "      nullIsError = TRUE;\n");
	 AsnIterTakeBuf (iter);
      }

      sprintf (iter->buf,
	       "      func = (AsnReadFunc) %sAsnRead;\n",
	       (iter->stack->type == ASNITER_Pseudo_USER_TYPE) ?
	       iter->stack->obj_name :
	       AsnCodeCleanName (final_atp->name,
				 iter->buf2, CLEAN_FOR_OBJ_NAME, iter -> acip -> maxDefineLength));
      AsnIterTakeBuf (iter);
       /* { */ sprintf (iter->buf, " }\n   ");
      AsnIterTakeBuf (iter);
      break;
   }

}

/***************************
*
* userobj_SETOF(iter,final_atp,recur_level)
*
* We have a SET OF user object within  a CHOICE ::=  or a SEQUENCE ::=
*
*  such as:
*
*    ParentSeq ::= CHOICE {
*          or
*    ParentSeq ::= SEQUENCE {
*  ... other stuff before
*     we_are_here SETOF SomeUserObject  <<<<< dealing with this thing
* --- other stuff after
************************/
static void
userobj_SETOF (AsnIterPtr iter, AsnTypePtr final_atp, Int2 recur_level)
{
   userobj_and_basetype_SETOF (iter, final_atp, recur_level,
			       ASNITER_USER_TYPE);

}

/***************************
*
* userobj_and_basetype_SETOF(iter,final_atp,recur_level,base_or_user)
*
* We have a SET OF object within  a CHOICE ::=  or a SEQUENCE ::=
*
*  such as:
*
*    ParentSeq ::= CHOICE {
*          or
*    ParentSeq ::= SEQUENCE {
*  ... other stuff before
*     we_are_here SETOF SomeObject  <<<<< dealing with this thing
* --- other stuff after
************************/
static void
userobj_and_basetype_SETOF (AsnIterPtr iter, AsnTypePtr final_atp, Int2 recur_level, Int2 base_or_user)
{
   CharPtr         valslot;
   CharPtr         valslotdefine;
   Boolean         in_setof_choice;
   AsnCodeNodePtr  which_node = AsnCode_RightChoiceNode (iter,
			       iter->stack->outer->outer, &in_setof_choice);
   Char            asntypename[128];
   AsnCodeNodePtr  right_outer_outer;
   AsnCodeNodePtr  right_outer =
   AsnCode_RightOuter (iter, iter->stack->outer);

   sprintf (asntypename,
	    "%s",
	    GenerateTypeFromStack (iter, FALSE));

   if (base_or_user == ASNITER_BASE_TYPE) {
      if (AsnCodeIsEnumType (final_atp)) {
	 valslot = "intvalue";
	 valslotdefine = defint;
      } else {
	 if (final_atp->type->isa == SETOF_TYPE ||
	     final_atp->type->isa == SEQOF_TYPE) {
	    valslot = AsnCodeWhichValSlot
	       (((AsnTypePtr) final_atp->branch)->type,
		&valslotdefine);
	 } else {
	    valslot = AsnCodeWhichValSlot
	       ((AsnTypePtr) final_atp->type, &valslotdefine);
	 }
      }
   }
   AsnCodeCleanName (final_atp->name, iter->buf2, CLEAN_FOR_SLOT, iter -> acip -> maxDefineLength);
   switch (iter->mode) {
   case ITER_ASSIGN:
   case ITER_CHECK_LIST:
   case ITER_NEW:
      break;
   case ITER_OBJECTS:
      if (recur_level == RECUR_SETOF_SEQSET) {

	 /*
	  * Will be linked list, but if a choice, will be ValNode if not,
	  * structure is to have next pointer...
	  */
	 if (AsnCodeIsReallyChoice (iter->atp)) {
	    sprintf (iter->buf, "       ValNodePtr ");
	    AsnIterTakeBuf (iter);
	 } else {
	    if (base_or_user == ASNITER_BASE_TYPE) {
	       sprintf (iter->buf, " ValNodePtr ");
	    } else {
	       sprintf (iter->buf, " struct struct_%s PNTR ",
			iter->buf2);
	    }
	    AsnIterTakeBuf (iter);
	 }
	 sprintf (iter->buf, "  %s;\n",
		  right_outer->slot_name);
	 AsnIterTakeBuf (iter);
      } else {
	 /* RECUR_SETOF_CHOICE:   */
	 right_outer_outer =
	    AsnCode_RightOuter (iter, iter->stack->outer->outer);

	 /*
	  * no objects, as slot absorbed within data.ptrvalue of ValNode,
	  * used for Choice but need symbolic #defines for choice
	  */
	 sprintf (iter->buf, "#define %s_%s %d\n",
		  which_node->obj_name,
		  right_outer->slot_name,
		  right_outer_outer->slot_count);
	 AsnIterTakeBuf (iter);
      }
      break;
   case ITER_FREE:
   case ITER_WRITE:
      if (recur_level == RECUR_SETOF_SEQSET) {
	 /* -- in SEQUENCE ::= */
	 if (base_or_user == ASNITER_BASE_TYPE) {
	    sprintf (iter->buf, "     %sAsnGenericBaseSeqOf%s(ptr -> %s ",
		     iter->mode == ITER_FREE ?
		     "" : "retval = ",
		     iter->mode == ITER_FREE ?
		     "Free" : "AsnWrite",
		     right_outer->slot_name);
	    AsnIterTakeBuf (iter);
	    sprintf (iter->buf, ",%s",
		     valslotdefine);
	    AsnIterTakeBuf (iter);
	    if (iter->mode == ITER_FREE) {
	       sprintf (iter->buf, ");\n");
	    } else {
	       sprintf (iter->buf, ", aip, %s, %s);\n",
			asntypename,
			Append_EAndPrestrip (asntypename, iter -> acip -> maxDefineLength));
	    }
	    AsnIterTakeBuf (iter);
	 } else if (iter->stack->type == ASNITER_Pseudo_USER_TYPE) {
	    if (iter->mode == ITER_FREE) {
	       sprintf (iter->buf, "%sFree(ptr -> %s);\n",
			right_outer->obj_name,
			right_outer->slot_name);
	    } else {
	       sprintf (iter->buf,
			"retval = %sAsnWrite(ptr -> %s, aip,%s);\n",
			right_outer->obj_name,
			right_outer->slot_name,
			asntypename);
	    }
	    AsnIterTakeBuf (iter);
	 } else {
	    /* user obj here */

	    if (AsnCodeIsReallyChoice (iter->atp)) {

	       sprintf (iter->buf, "      AsnGenericChoiceSeqOf%s(ptr -> %s,",
			iter->mode == ITER_FREE ?
			"Free" : "AsnWrite",
			right_outer->slot_name);
	       AsnIterTakeBuf (iter);
	    } else {

	       sprintf (iter->buf, "      AsnGenericUserSeqOf%s(ptr -> %s,",
			iter->mode == ITER_FREE ?
			"Free" : "AsnWrite",
			right_outer->slot_name);
	       AsnIterTakeBuf (iter);
	    }
	    if (iter->mode == ITER_FREE) {
	       sprintf (iter->buf, " (AsnOptFreeFunc) %sFree);\n",
			AsnCodeCleanName (final_atp->name,
					  iter->buf2, CLEAN_FOR_OBJ_NAME, iter -> acip -> maxDefineLength)
		  );
	    } else {
	       sprintf (iter->buf, " (AsnWriteFunc) %sAsnWrite, aip, %s, %s);\n",
			AsnCodeCleanName (final_atp->name,
					  iter->buf2, CLEAN_FOR_OBJ_NAME, iter -> acip -> maxDefineLength),
			asntypename,
			Append_EAndPrestrip (asntypename, iter -> acip -> maxDefineLength));
	    }
	    AsnIterTakeBuf (iter);
	 }
      } else {
	 /* RECUR_SETOF_CHOICE:   */

	 sprintf (iter->buf, "      case %s_%s:\n",
		  which_node->obj_name,
		  right_outer->slot_name);
	 AsnIterTakeBuf (iter);
	 if (base_or_user == ASNITER_BASE_TYPE) {
	    if (iter->mode == ITER_FREE) {
	       sprintf (iter->buf,
			"     AsnGenericBaseSeqOfFree((ValNodePtr) pnt,");
	    } else {
	       sprintf (iter->buf,
		 "      retval = AsnGenericBaseSeqOfAsnWrite((Pointer) pnt,");
	    }
	    AsnIterTakeBuf (iter);
	    sprintf (iter->buf, "%s",
		     valslotdefine);
	    AsnIterTakeBuf (iter);
	    if (iter->mode == ITER_FREE) {
	       sprintf (iter->buf, ");\n");
	       AsnIterTakeBuf (iter);
	    } else {
	       sprintf (iter->buf, ", aip, %s, %s);",
			asntypename,
			Append_EAndPrestrip (asntypename, iter -> acip -> maxDefineLength));
	       AsnIterTakeBuf (iter);

	    }
	 } else if (iter->stack->type == ASNITER_Pseudo_USER_TYPE) {


	    if (iter->mode == ITER_FREE) {
	       sprintf (iter->buf, "%sFree(ptr -> %s);\n",
			right_outer->obj_name,
			right_outer->slot_name);
	    } else {
	       sprintf (iter->buf,
			"retval = %sAsnWrite(ptr -> %s, aip,%s);\n",
			right_outer->obj_name,
			right_outer->slot_name,
			asntypename);
	    }
	    AsnIterTakeBuf (iter);
	 } else {
	    /* user type */


	    if (AsnCodeIsReallyChoice (iter->atp)) {

	       sprintf (iter->buf, "      %sAsnGenericChoiceSeqOf%s((Pointer) pnt,",
			iter->mode == ITER_FREE ?
			"" : "retval = ",
			iter->mode == ITER_FREE ?
			"Free" : "AsnWrite",
			right_outer->slot_name);
	       AsnIterTakeBuf (iter);
	    } else {

	       if (iter->mode == ITER_WRITE) {
		  sprintf (iter->buf,
			   "      retval = ");
	       } else {
		  sprintf (iter->buf,
			   "      ");
	       }
	       AsnIterTakeBuf (iter);
	       sprintf (iter->buf, "AsnGenericUserSeqOf%s((Pointer) pnt,",
			iter->mode == ITER_FREE ?
			"Free" : "AsnWrite",
			right_outer->slot_name);
	       AsnIterTakeBuf (iter);
	    }
	    if (iter->mode == ITER_FREE) {
	       sprintf (iter->buf, " (AsnOptFreeFunc) %sFree);\n",
			AsnCodeCleanName (final_atp->name,
					  iter->buf2, CLEAN_FOR_OBJ_NAME, iter -> acip -> maxDefineLength));
	       AsnIterTakeBuf (iter);
	    } else {
	       sprintf (iter->buf,
			" (AsnWriteFunc) %sAsnWrite, aip, %s, %s);\n",
			AsnCodeCleanName (final_atp->name,
					  iter->buf2, CLEAN_FOR_OBJ_NAME, iter -> acip -> maxDefineLength),
			asntypename,
			Append_EAndPrestrip (asntypename, iter -> acip -> maxDefineLength));
	       AsnIterTakeBuf (iter);
	    }
	 }
	 sprintf (iter->buf, "            break;\n");
	 AsnIterTakeBuf (iter);
      }
      break;

   case ITER_READ:
      if (recur_level == RECUR_SETOF_SEQSET) {
#ifdef EXTRA_READIDBUG
	 AsnCode_RightOuter (iter);
	 ? ? ?
	    if (iter->stack->outer->slot_count == 1) {
	    sprintf (iter->buf, "   atp = AsnReadId(aip,amp, atp);\n");
	    AsnIterTakeBuf (iter);
	 }
#endif
	 if ((iter->acip->debug_level) > 3) {
	    AsnCodeShowStack (iter);
	    fflush ((iter->acip -> bug_fp));
	 }
	 sprintf (iter->buf, "   if (atp == %s) {\n",	/* } */
		  asntypename);
	 AsnIterTakeBuf (iter);
	 sprintf (iter->buf, "      ptr -> %s = ",
		  right_outer->slot_name);
	 AsnIterTakeBuf (iter);
	 if (base_or_user == ASNITER_BASE_TYPE) {
	    sprintf (iter->buf,
		     "AsnGenericBaseSeqOfAsnRead(aip, amp, atp, %s, &isError);\n ",
		     valslotdefine);
	    AsnIterTakeBuf (iter);
	 } else if (iter->stack->type == ASNITER_Pseudo_USER_TYPE) {
	    sprintf (iter->buf,
		     " %sAsnRead(aip, atp);\n", right_outer->obj_name);
	    AsnIterTakeBuf (iter);
	 } else {
	    sprintf (iter->buf,
		     "AsnGeneric%sSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ",
		     AsnCodeIsReallyChoice (iter->atp) ? "Choice" : "User");
	    AsnIterTakeBuf (iter);
	    sprintf (iter->buf, "%sAsnRead,",
		     AsnCodeCleanName (final_atp->name,
				       iter->buf2, CLEAN_FOR_OBJ_NAME, iter -> acip -> maxDefineLength));
	    AsnIterTakeBuf (iter);
	    sprintf (iter->buf,
		     " (AsnOptFreeFunc) ");
	    AsnIterTakeBuf (iter);
	    sprintf (iter->buf, "%sFree);\n",
		     AsnCodeCleanName (final_atp->name,
				       iter->buf2, CLEAN_FOR_OBJ_NAME, iter -> acip -> maxDefineLength));
	    AsnIterTakeBuf (iter);
	 }
	 sprintf (iter->buf,
		  "      if (isError && ptr -> %s == NULL) {\ngoto erret;\n}\n",
		  right_outer->slot_name);
	 AsnIterTakeBuf (iter);
	 sprintf (iter->buf, "   atp = AsnReadId(aip,amp, atp);\n");
	 AsnIterTakeBuf (iter);
	  /* { */ sprintf (iter->buf, "   }\n");
	 AsnIterTakeBuf (iter);
      } else {
	 /* RECUR_SETOF_CHOICE:   */
	 if (which_node->slot_count != 1) {
	    sprintf (iter->buf, "  else ");
	    AsnIterTakeBuf (iter);
	 }
	 if ((iter->acip->debug_level) > 3) {
	    AsnCodeShowStack (iter);
	    fflush ((iter->acip -> bug_fp));
	 }
	 sprintf (iter->buf, "if (atp == %s) {\n",	/* } */
		  asntypename);
	 AsnIterTakeBuf (iter);
	 sprintf (iter->buf, "      choice = %s_%s;\n",
		  which_node->obj_name,
		  right_outer->slot_name);
	 AsnIterTakeBuf (iter);
	 if (base_or_user == ASNITER_BASE_TYPE) {
	    sprintf (iter->buf,
		     "          anp -> data.ptrvalue = \n");
	    AsnIterTakeBuf (iter);
	    sprintf (iter->buf,
		     "AsnGenericBaseSeqOfAsnRead(aip, amp, atp, %s, &isError);\n ",
		     valslotdefine);
	    AsnIterTakeBuf (iter);
	 } else if (iter->stack->type == ASNITER_Pseudo_USER_TYPE) {
	    sprintf (iter->buf,
		     " %sAsnRead(aip, atp);\n", right_outer->obj_name);
	    AsnIterTakeBuf (iter);
	 } else {
	    sprintf (iter->buf,
		     "          anp -> data.ptrvalue =\n\
               AsnGeneric%sSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ",
		     AsnCodeIsReallyChoice (iter->atp) ? "Choice" : "User");
	    AsnIterTakeBuf (iter);
	    sprintf (iter->buf, "%sAsnRead,",
		     AsnCodeCleanName (final_atp->name,
				       iter->buf2, CLEAN_FOR_OBJ_NAME, iter -> acip -> maxDefineLength));
	    AsnIterTakeBuf (iter);
	    sprintf (iter->buf,
		     "             (AsnOptFreeFunc) ");
	    AsnIterTakeBuf (iter);
	    sprintf (iter->buf, "%sFree);\n",
		     AsnCodeCleanName (final_atp->name,
				       iter->buf2, CLEAN_FOR_OBJ_NAME, iter -> acip -> maxDefineLength));
	    AsnIterTakeBuf (iter);
	 }
	 sprintf (iter->buf,
	       "      if (isError && anp -> data.ptrvalue == NULL) {\ngoto erret;\n}\n");
	 AsnIterTakeBuf (iter);
	  /* { */ sprintf (iter->buf, "   }\n   ");
	 AsnIterTakeBuf (iter);
      }
      break;
   }

}


/***************************
*
* AsnCode_userobj(iter)
*
****************************/
static void
AsnCode_userobj (struct struct_asniter PNTR iter)
{
   Int2            recur_level = AsnCodeIterRecurLevel (iter);
   Int2            real_type;
   AsnTypePtr      name_at_type;

   if (iter->mode == ITER_CHECK_LIST) {
      userobj_CHECK_LIST (iter);
   } else {
      switch (recur_level) {
      case RECUR_OUTER:
	 userobj_OUTER (iter);
	 break;
      case RECUR_SEQSET:	/* userobject in a SEQUENCE or SET */
	 real_type = AsnCodeFinalType (iter, iter->atp, &name_at_type);
	 switch (real_type) {
	 case ENUM_TYPE:
	 case ASNITER_BASE_TYPE:
	    basetype_SEQSET (iter, name_at_type);
	    break;

	 case SEQOF_TYPE:
	 case SETOF_TYPE:
	 case SEQ_TYPE:
	 case SET_TYPE:
	 case CHOICE_TYPE:

	    /*
	     * user objects that are Sequences and Choices are handled
	     * similarly from within sequences
	     */
	    userobj_SEQSET (iter, name_at_type);
	    break;
	 }
	 break;
      case RECUR_CHOICE:
	 real_type = AsnCodeFinalType (iter, iter->atp, &name_at_type);
	 switch (real_type) {
	 case ENUM_TYPE:
	 case ASNITER_BASE_TYPE:
	    basetype_CHOICE (iter, name_at_type);
	    break;

         case SEQOF_TYPE:
         case SETOF_TYPE:
	 case SEQ_TYPE:
	 case SET_TYPE:
	 case CHOICE_TYPE:

	    /*
	     * user objects that are Sequences and Choices are handled
	     * similarly from within choices
	     */
	    userobj_CHOICE (iter, name_at_type);
	    break;
	 }
	 break;
      case RECUR_SETOF_OUTER:

	 /*
as:     SupAtsRxnsStrs ::= SET OF SupAtRxnOrStr
	  */
	 real_type = AsnCodeFinalType (iter, iter->atp, &name_at_type);
	 switch (real_type) {
	 case ENUM_TYPE:
	 case ASNITER_BASE_TYPE:
			AsnCode_SETOF_base_or_user (iter, ASNITER_BASE_TYPE);
	    break;

         case SEQOF_TYPE:
         case SETOF_TYPE:
	 case SEQ_TYPE:
	 case SET_TYPE:
	 case CHOICE_TYPE:

	    /*
	     * user objects that are Sequences and Choices are handled
	     * similarly from within choices
	     */
		AsnCode_SETOF_base_or_user (iter, ASNITER_USER_TYPE);
	    break;
	 }


	 break;
      case RECUR_SETOF_SEQSET:
      case RECUR_SETOF_CHOICE:
	 real_type = AsnCodeFinalType (iter, iter->atp, &name_at_type);
	 switch (real_type) {
	 case ENUM_TYPE:
	 case ASNITER_BASE_TYPE:
	    basetype_SETOF (iter, name_at_type, recur_level);
	    break;

         case SEQOF_TYPE:
         case SETOF_TYPE:
	 case SEQ_TYPE:
	 case SET_TYPE:
	 case CHOICE_TYPE:

	    /*
	     * user objects that are Sequences and Choices are handled
	     * similarly from within SETOF in parental SEQUENCE or CHOICE
	     */
	    userobj_SETOF (iter, name_at_type, recur_level);
	    break;
	 }
	 break;
      }
   }

}

/*************************
*
* AsnCodeFinalType(iter,atp,vase_atpPt)
*
* returns of meaning are:
*
*   ENUM_TYPE;
*   SEQ_TYPE;
*   SET_TYPE;
*   CHOICE_TYPE;
*   ASNITER_BASE_TYPE  (captures all other base types)
********************/
static Int2
AsnCodeFinalType (struct struct_asniter PNTR iter, AsnTypePtr atp, AsnTypePtr PNTR base_atpPt)
{
   Int2            retval = 0;

   if ((iter->acip->debug_level) > 3) {
      AsnCodeShowStack (iter);
      fprintf ((iter->acip -> bug_fp), "In AsnCodeFinalType:atp name%s-%d\n",
	       TESTNIL (atp->name), (int) atp->isa);
   }
   if (atp != NULL){
      if (atp->imported) {
	 if (atp->type != NULL) {
	    retval = AsnCodeFinalType (iter, atp->type, base_atpPt);
	 } else {
	    retval = 0;
	    if (base_atpPt != NULL) {
	       *base_atpPt = NULL;
	    }
	 }
      } else if (atp->type != NULL) {

	 if (atp->type->isa == ENUM_TYPE) {
	    retval = ENUM_TYPE;
	    if (base_atpPt != NULL) {
	       *base_atpPt = atp;
	    }
	 } else if (atp->type->isa < 301) {
	    retval = 0;
	    if (base_atpPt != NULL) {
	       *base_atpPt = NULL;
	    }
	    /*------- USER OBJECTS -------*/
	 } else if (atp->type->isa > 400) {
	    /*------- USER OBJECTS -------*/
	    retval = AsnCodeFinalType (iter, atp->type, base_atpPt);
	    /*------SETs and SEQUENCEs ---*/
	 } else if (atp->type->isa == SEQ_TYPE) {
	    retval = SEQ_TYPE;
	    if (base_atpPt != NULL) {
	       *base_atpPt = atp;
	    }
	 } else if (atp->type->isa == SET_TYPE) {
	    retval = SET_TYPE;
	    if (base_atpPt != NULL) {
	       *base_atpPt = atp;
	    }
	    /*-------- CHOICE ---*/
	 } else if (atp->type->isa == CHOICE_TYPE) {
	    retval = CHOICE_TYPE;
	    if (base_atpPt != NULL) {
	       *base_atpPt = atp;
	    }
	    /*----- SET OF or SEQ OF-------*/
	 } else if (atp->type->isa == SETOF_TYPE ||
		    atp->type->isa == SEQOF_TYPE) {
	    /*----- SET OF CHOICE is a special case */
	    AsnTypePtr      test_branch = (AsnTypePtr) atp->branch;
	    if (test_branch->type->isa == CHOICE_TYPE) {
	       retval = CHOICE_TYPE;
	       if (base_atpPt != NULL) {
		  *base_atpPt = atp;
	       }
	    } else {
	       retval = atp->type->isa;
	       if (base_atpPt != NULL) {
		  *base_atpPt = atp;
	       }
	    }
	 } else if (ISA_BASETYPE (atp->type->isa)) {
	    retval = ASNITER_BASE_TYPE;
	    if (base_atpPt != NULL) {
	       *base_atpPt = atp;
	    }
	 }
      }
   }
   if ((iter->acip->debug_level) > 3) {
      if (base_atpPt) {
	 fprintf ((iter->acip -> bug_fp), " returning atp name %s-%d, retval %d\n",
		  (*base_atpPt)? TESTNIL ((*base_atpPt)->name):"NULL",
		  (int) ((*base_atpPt)? (*base_atpPt)->isa:0), (int) retval);
      }
      fflush ((iter->acip -> bug_fp));
   }
   return retval;
}

/*****************************************************************************
*
*   AsnCodeLookupType(atp)
*
*****************************************************************************/
static CharPtr
AsnCodeLookupType (AsnTypePtr atp)
{
   CharPtr         retval = "Pointer";


   if (atp != NULL){
      if (atp->type != NULL){
      	if (AsnCodeIsEnumType(atp)){
      	   retval = "Uint2";
      	}else{
      	 switch (atp->type->isa) {
      	 case BOOLEAN_TYPE:
      	    retval = "Uint1";
      	    break;
      	 case BITS_TYPE:
      	    retval = "Uint1";
      	    break;
      	 case INTEGER_TYPE:
      	    retval = "Int4";
      	    break;
	 case BIGINT_TYPE:
	    retval = "Int8";
	    break;
      	 case OCTETS_TYPE:
      	    retval = "ByteStorePtr";
      	    break;
      	 case NULL_TYPE:
      	    retval = "Uint1";
      	    break;
      	 case OBID_TYPE:
      	    retval = "Pointer";
      	    break;
      	 case OBDES_TYPE:
      	    retval = "Pointer";
      	    break;
      	 case REAL_TYPE:
      	    retval = "FloatHi";
      	    break;
      	 case ENUM_TYPE:
      	    retval = "Uint2";
      	    break;
      	 case ANY_TYPE:
      	    retval = "Pointer";
      	    break;
      	 case NUMERICSTRING_TYPE:
      	    retval = "CharPtr";
      	    break;
      	 case PRINTABLESTRING_TYPE:
      	    retval = "CharPtr";
      	    break;
      	 case TELETEXSTRING_TYPE:
      	    retval = "CharLenPtr";
      	    break;
      	 case VIDEOTEXSTRING_TYPE:
      	    retval = "CharLenPtr";
      	    break;
      	 case IA5STRING_TYPE:
      	    retval = "CharLenPtr";
      	    break;
      	 case GRAPHICSTRING_TYPE:
      	    retval = "CharLenPtr";
      	    break;
      	 case VISIBLESTRING_TYPE:
      	    retval = "CharPtr";
      	    break;
      	 case CHOICE_TYPE:
      	    retval = "ValNodePtr";
      	    break;
      	 case GENERALSTRING_TYPE:
      	    retval = "CharLenPtr";
      	    break;
      	 case CHARACTERSTRING_TYPE:
      	    retval = "CharLenPtr";
      	    break;
      	 case GENTIME_TYPE:
      	    retval = "Pointer";	/* Time types */
      	    break;
      	 case UTCTIME_TYPE:
      	    retval = "CharLenPtr";
      	    break;
      	 case STRSTORE_TYPE:
      	    retval = "StringStorePtr";
      	    break;
      	 }
      	}
   }
}

   return retval;
}

/*****************************************************************************
*
*   AsnCodeNeedFree(atp)
*
*****************************************************************************/
static CharPtr
AsnCodeNeedFree (AsnTypePtr atp)
{
   CharPtr         retval = NULL;
   static CharPtr  mem = "MemFree";
   static CharPtr  byte = "BSFree";


   if (atp != NULL)
      if (atp->type != NULL)
	 switch (atp->type->isa) {
	 case BOOLEAN_TYPE:
	 case BITS_TYPE:
	 case INTEGER_TYPE:
         case BIGINT_TYPE:
	 case NULL_TYPE:
	 case REAL_TYPE:
	 case ENUM_TYPE:
	    retval = NULL;
	    break;
	 case OBID_TYPE:
	 case OBDES_TYPE:
	 case ANY_TYPE:
	 case NUMERICSTRING_TYPE:
	 case PRINTABLESTRING_TYPE:
	 case TELETEXSTRING_TYPE:
	 case VIDEOTEXSTRING_TYPE:
	 case IA5STRING_TYPE:
	 case GRAPHICSTRING_TYPE:
	 case VISIBLESTRING_TYPE:
	 case GENERALSTRING_TYPE:
	 case CHARACTERSTRING_TYPE:
	 case GENTIME_TYPE:
	 case UTCTIME_TYPE:
	    retval = mem;
	    break;
	 case CHOICE_TYPE:	/* questionable! */
	    retval = mem;
	    break;
	 case STRSTORE_TYPE:
	 case OCTETS_TYPE:
	    retval = byte;
	    break;
	 };

   return retval;
}

/*****************************************************************
*
*       AsnCodeIsReallyChoice(atp)
*
*****************************************************************/

static Boolean
AsnCodeIsReallyChoice (AsnTypePtr atp)
{
   Boolean         retval = FALSE;
   AsnTypePtr      now_atp = atp;

   if (now_atp)
      if (now_atp->imported) {
	 if (now_atp->type != NULL) {
	    retval = AsnCodeIsReallyChoice (now_atp->type);
	 }
      } else if (now_atp->type->isa == CHOICE_TYPE) {
	 retval = TRUE;
      } else if (now_atp->type->isa == SETOF_TYPE ||
		 now_atp->type->isa == SEQOF_TYPE) {
	 AsnTypePtr      branch = (AsnTypePtr) atp->branch;
	 retval = AsnCodeIsReallyChoice (branch);
      } else if (now_atp->type->isa > 400) {
	 retval = AsnCodeIsReallyChoice (now_atp->type);
      }
   return retval;
}

/*****************************************************************
*
*       AsnCodeIsEnumType(atp)
*
*****************************************************************/

static Boolean
AsnCodeIsEnumType (AsnTypePtr atp)
{
   Boolean         retval = FALSE;
   AsnTypePtr      now_atp;

   for (now_atp = atp; now_atp != NULL; now_atp = now_atp -> type){
      if (now_atp->isa == ENUM_TYPE) {
	 retval = TRUE;
	 break;
      } else if (now_atp->type != NULL) {
	 if (now_atp->type->isa == ENUM_TYPE) {
	    retval = TRUE;
	    break;
	 }
         if (now_atp->type->isa <= 400 && ! now_atp -> imported  ) {
	    break;
         }
      }
   }
   return retval;
}

/***********************
*
* AsnCodeFinishedRoutine(iter)
*
****************************/
void
AsnCodeFinishedRoutine (struct struct_asniter PNTR iter)
{
   Boolean         use_last = FALSE;

   if (iter->cur_routine) {
      iter->cur_routine->finished = TRUE;
      if (iter->cur_routine->last) {
	 if (!iter->cur_routine->last->finished) {
	    /*--- last routine still active, make it current ---*/
	    iter->cur_routine = iter->cur_routine->last;
	    use_last = TRUE;
	 }
      }
      if (!use_last) {
	 /* -- last routine was not active, just say nothing active now */
	 iter->cur_routine = NULL;
      }
   }
}


/***********************
*
* AsnCodeNewRoutine(iter)
*
*************************/
void
AsnCodeNewRoutine (struct struct_asniter PNTR iter)
{
   AsnCodeSpooledPtr new_routine = (AsnCodeSpooledPtr)MemNew (sizeof (AsnCodeSpooled));
   AsnCodeSpooledPtr scanner;

   if (iter->cur_routine != NULL) {
      if (iter->cur_routine->finished) {
	 iter->cur_routine = NULL;
      }
   }
   if (iter->routines == NULL) {
      iter->routines = iter->cur_routine =
	 iter->last_routine = new_routine;
   } else if (iter->cur_routine == NULL) {
      /* no active routine, just add to end of chain */
      if (iter->last_routine == NULL) {
	  /* refind tail */
	    for (scanner = iter->routines; scanner; scanner = scanner->next) {
	    iter->last_routine = scanner;
	 }
      }
      new_routine->next = iter->last_routine->next;
      iter->last_routine->next = new_routine;
      iter->cur_routine = new_routine;
      new_routine->last = iter->last_routine;
      iter->last_routine = new_routine;
   } else {
      /* there is an active routine, insert this new one after it */
      new_routine->next = iter->cur_routine->next;
      new_routine->last = iter->cur_routine;
      if (iter->cur_routine->next) {
	 iter->cur_routine->next->last = new_routine;
      }
      new_routine->next = iter->cur_routine->next;
      iter->cur_routine->next = new_routine;
      iter->cur_routine = new_routine;
      iter->last_routine = new_routine;
   }

}

/************************************
*
* AsnCodeDumpLine(iter, line)
*
************************************/
static void
AsnCodeDumpLine (AsnIterPtr iter, CharPtr line)
{
   CharPtr         p;
   CharPtr         maxChar = &line[StrLen (line)];
   Int2            i;
   Int2            oneLess;

   for (p = line; p < maxChar; p++) {
      if (iter->leadingspaces) {
	 if (*p == ' ' || *p == '\t') {
	    continue;
	 } else {
	    iter->leadingspaces = FALSE;
	    if (*p != '\n') {
	       if (*p == '}' || StrNCmp (p, "case ", 5) == 0 ||
		   StrNCmp (p, "default:", 8) == 0 ||
		   StrNCmp (p, "ret:", 4) == 0 ||
		   StrNCmp (p, "erret:", 6) == 0 ||
		   StrNCmp (p, "#define", 6) == 0)
		  oneLess = 1;
	       else
		  oneLess = 0;
	       for (i = oneLess; i < iter->indent; i++)
		  fprintf (iter->fp, "   ");
	    }
	 }
	 /* fall through */
      }
      switch (*p) {
      case '{':
	 iter->indent++;
	 break;
      case '}':
	 if (iter->indent > 0) {
	    iter->indent--;
	 }
	 break;
      case '\n':
	 iter->leadingspaces = TRUE;
	 break;
      default:
	 break;
      }
      putc (*p, iter->fp);
   }
}

/************************************
*
* AsnCodeCheckFileDoneRoutines(iter)
*
************************************/
static void
AsnCodeCheckFileDoneRoutines (AsnIterPtr iter)
{
   AsnCodeSpooledPtr routine, next_routine;
   AsnCodeLinesPtr line, next_line;

   for (routine = iter->routines; routine; routine = next_routine) {
      next_routine = routine->next;
      for (line = routine->head_line; line; line = next_line) {
	 next_line = line->next;
	 AsnCodeDumpLine (iter, line->line);
	 if ((iter->acip->debug_level) > 2) {
	    fprintf ((iter->acip -> bug_fp), "Dumping:%x<%s>\n", routine, TESTNIL (line->line));
	 }
	 if ((iter->acip->debug_level) > 0)
	    fflush (iter->fp);
	 MemFree (line->line);
	 MemFree (line);
      }
      MemFree (routine);
   }
   iter->routines = NULL;
   iter->cur_routine = NULL;

}


/***********************
*
*    AsnIterTakeBuf(iter)
*
************************/

static void
AsnIterTakeBufReal (struct struct_asniter PNTR iter, int lineno)
{
   AsnCodeLinesPtr line =
   (AsnCodeLinesPtr) MemNew (sizeof (AsnCodeLines));

   if (iter->cur_routine == NULL) {
      AsnCodeNewRoutine (iter);
   }
   if (iter->cur_routine->head_line) {
      iter->cur_routine->tail_line->next = line;
   } else {
      iter->cur_routine->head_line = line;
   }
   iter->cur_routine->tail_line = line;
   if ((iter->acip->debug_level) > 2) {
      fprintf ((iter->acip -> bug_fp), "Buf (%d):%x<%s>\n", lineno, iter->cur_routine, TESTNIL (iter->buf));
      fflush ((iter->acip -> bug_fp));
   }
   line->line = StringSave (iter->buf);
   (iter->buf)[0] = '\0';

}

/*****************************************************************************
*
* AsnCodeStaticCandidate(iter)
*
*****************************************************************************/

static Boolean
AsnCodeStaticCandidate (AsnIterPtr iter)
{
#ifdef USER_PSEUDOOBJECTS_STATIC
   Int2            recur_level = AsnCodeIterRecurLevel (iter);
   Boolean         retval = FALSE;

   switch (recur_level) {
   case RECUR_SEQSET:
   case RECUR_CHOICE:
   case RECUR_SETOF_SEQSET:
   case RECUR_SETOF_CHOICE:
      if (iter->atp->isa == 0)
	 retval = TRUE;
      break;
   default:
      break;
   }
   if ((iter->acip->debug_level) > 3 && iter->atp->isa == 0) {
      AsnCodeShowStack (iter);
      fprintf ((iter->acip -> bug_fp), "AsnCodeStaticCandidate: depth %d, recur-level %d, retval %d\n",
	       iter->depth, AsnCodeIterRecurLevel (iter), (int) retval);
      fflush ((iter->acip -> bug_fp));
   }
   return retval;
#else
   return FALSE;
#endif
}

/*****************************************************************************
*
* AsnCodeGenerateProtos(iter,buf,from_choice)
*
*****************************************************************************/
void
AsnCodeGenerateProtos (struct struct_asniter PNTR iter, CharPtr buf, Boolean from_choice)
{
   AsnCodeNewRoutine (iter);
   if (iter->atp->isa == 0) {
      sprintf (iter->buf, "\n#ifdef NLM_GENERATED_CODE_PROTO");
      AsnIterTakeBuf (iter);
   }
   /* -- Free -- */
   sprintf (iter->buf, "\n\n%s%sPtr%s ",
	    AsnCodeStaticCandidate (iter) ? "static " : "NLM_EXTERN ", buf,
	    " LIBCALL");
   AsnIterTakeBuf (iter);
   sprintf (iter->buf, "%sFree PROTO ((", buf);
   AsnIterTakeBuf (iter);
   sprintf (iter->buf, "%sPtr ));\n", buf);
   AsnIterTakeBuf (iter);
   if (!from_choice) {
      /* New */
      sprintf (iter->buf, "%s%sPtr%s ",
	       AsnCodeStaticCandidate (iter) ? "static " : "NLM_EXTERN ", buf,
	       " LIBCALL");
      AsnIterTakeBuf (iter);
      sprintf (iter->buf, "%sNew PROTO (( void ));\n", buf);
      AsnIterTakeBuf (iter);
   }
   /* --- Read --- */
   sprintf (iter->buf, "%s%sPtr%s ",
	    AsnCodeStaticCandidate (iter) ? "static " : "NLM_EXTERN ", buf,
	    " LIBCALL");
   AsnIterTakeBuf (iter);
   sprintf (iter->buf, "%sAsnRead PROTO (( AsnIoPtr, AsnTypePtr));\n", buf);
   AsnIterTakeBuf (iter);
   /* --- Write --- */
   sprintf (iter->buf,
      "%sBoolean%s %sAsnWrite PROTO (( %sPtr , AsnIoPtr, AsnTypePtr));\n\n",
	    AsnCodeStaticCandidate (iter) ? "static " : "NLM_EXTERN ",
	    " LIBCALL", buf, buf);
   AsnIterTakeBuf (iter);
   if (iter->atp->isa == 0) {
      sprintf (iter->buf, "#endif /* NLM_GENERATED_CODE_PROTO */\n\n");
      AsnIterTakeBuf (iter);
   }
   AsnCodeFinishedRoutine (iter);
}

/*****************************
*
*    AsnCodeCleanName(name,buf,wash_or_buf,maxlen)
*
* WASH == CLEAN_FOR_SLOT
* BUFF == CLEAN_FOR_OBJ_NAME
* SLOT_POLISH == CLEAN_FOR_ASN_TYPE
*********************/

CharPtr
AsnCodeCleanName (CharPtr name, char buf[BUF_LEN], int wash_or_buf, int maxlen)
{
   CharPtr         p;
   int             dex, target = 0;
   Boolean         is_word;
   CharPtr         symbol;
   Boolean         AllUpperCase = TRUE;

   if (wash_or_buf != SLOT_BUFF) {
      is_word = TRUE;
   } else {
      is_word = FALSE;
   }
   if (name)			/* before pseudo-user types defined, might be
				 * null */
      for (p = name, dex = 0, target = 0;
	   *p && dex < BUF_LEN - 1; dex++, p++) {
	 if (*p == '-') {
	    is_word = TRUE;
	    if (wash_or_buf == WASH || wash_or_buf == SLOT_POLISH) {
	       buf[target++] = '_';
	    }
	    continue;
	 }
	 buf[target++] = *p;
	 if ((is_word && wash_or_buf == BUFF)
	     || wash_or_buf == SLOT_POLISH) {
	    buf[target - 1] = TO_UPPER (buf[target - 1]);
	 }
	 if (! IS_UPPER(*p)) {
	    AllUpperCase = FALSE;
	 }
	 is_word = FALSE;
      }

   if (AllUpperCase && wash_or_buf == BUFF) {
      buf[target++] = '_';
      buf[target++] = '_';
   }
   buf[target] = '\0';
   for (dex = 0; dex < NUM_C_RESERVED_WORDS; dex++) {
      if (StrCmp(buf, c_reserved_word[dex]) == 0) {
         buf[target++] = '_';
         buf[target++] = '_';
         buf[target] = '\0';
      }
   }
	 
   symbol = buf;
   if (wash_or_buf == SLOT_POLISH) {
      symbol = SpanLengthyPrefix (buf, maxlen);
   }
   return (CharPtr) symbol;
}

/******************
*
*   AsnCodeHeader(iter)
*
******************/

void
AsnCodeHeader (AsnIterPtr iter, CharPtr name)
{


   sprintf (iter->buf, "\n\n/**************************************************\n");
   AsnIterTakeBuf (iter);
   sprintf (iter->buf, "*\n");
   AsnIterTakeBuf (iter);
   sprintf (iter->buf, "*    %s\n", name);
   AsnIterTakeBuf (iter);
   sprintf (iter->buf, "*\n");
   AsnIterTakeBuf (iter);
   sprintf (iter->buf, "**************************************************/\n");
   AsnIterTakeBuf (iter);
}


/****************************************
*
* AsnCodeFunctionWriteFuncChoiceStart(iter,doing_outer_setof_choice)
*
****************************************/

void
AsnCodeFunctionWriteFuncChoiceStart (AsnIterPtr iter, Boolean doing_outer_setof_choice)
{
   Boolean         in_setof_choice;
   AsnCodeNodePtr  which_node = AsnCode_RightChoiceNode (iter, iter->stack, &in_setof_choice);
   CharPtr         obj_use = which_node->obj_name;
   CharPtr         asn_type_use;
   char            setof_objbuf[120];
   char            setof_asnbuf[120];

   obj_use = which_node->obj_name;
   asn_type_use = GenerateTypeFromStack (iter, !doing_outer_setof_choice);
   if (in_setof_choice) {
      sprintf (setof_objbuf, "%s_element", which_node->obj_name);
      sprintf (setof_asnbuf, "%s", GenerateTypeFromStack (iter, !doing_outer_setof_choice));
      obj_use = setof_objbuf;
      asn_type_use = setof_asnbuf;
   }
   sprintf (iter->buf, "\
	if (! loaded)\n\
	{\n\
		if (! %sAsnLoad())\n\
			return FALSE;\n\
	}\n\n", iter->module_name);
   AsnIterTakeBuf (iter);

   sprintf (iter->buf, "\
	if (aip == NULL)\n\
		return FALSE;\n\n");
   AsnIterTakeBuf (iter);
   sprintf (iter->buf, "\
	atp = AsnLinkType(orig, %s);   /* link local tree */\n\
    if (atp == NULL) {\n\
        return FALSE;\n}\n\n", asn_type_use);

   AsnIterTakeBuf (iter);
   sprintf (iter->buf, "\
	if (anp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }\n\
\n\
	av.ptrvalue = (Pointer)anp;\n");
   AsnIterTakeBuf (iter);
   if (!doing_outer_setof_choice) {
      sprintf (iter->buf, "\
	if (! AsnWriteChoice(aip, atp, (Int2)anp->choice, &av)) {\n\
        goto erret;\n}\n\n");
	 AsnIterTakeBuf (iter);
   }
}

/****************************************
*
* AsnCodeFunctionWriteFuncSeqStart(iter,no_write_braces)
*
****************************************/

void
AsnCodeFunctionWriteFuncSeqStart (AsnIterPtr iter, Boolean no_write_braces)
{

   sprintf (iter->buf, "\
	if (! loaded)\n\
	{\n\
		if (! %sAsnLoad()) {\n\
			return FALSE;\n}\n\
	}\n\n", iter->module_name);
   AsnIterTakeBuf (iter);

   sprintf (iter->buf, "\
	if (aip == NULL) {\n\
		return FALSE;\n}\n\n");
   AsnIterTakeBuf (iter);
   sprintf (iter->buf, "\
	atp = AsnLinkType(orig, %s);   /* link local tree */\n\
    if (atp == NULL) {\n\
        return FALSE;\n}\n\n",
	    GenerateTypeFromStack (iter, FALSE)
      );

   AsnIterTakeBuf (iter);
   sprintf (iter->buf, "\
            if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }\n");
   AsnIterTakeBuf (iter);

   if (! no_write_braces){
   sprintf (iter->buf, "\
	if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {\n\
        goto erret;\n}\n\n");
   AsnIterTakeBuf (iter);
   }
}

/****************************************
*
* AsnCodeFunctionReadFuncChoiceStart(iter,doing_outer_setof_choice)
*
****************************************/

void
AsnCodeFunctionReadFuncChoiceStart (AsnIterPtr iter, Boolean doing_outer_setof_choice)
{
   Boolean         in_setof_choice;
   AsnCodeNodePtr  which_node = AsnCode_RightChoiceNode (iter, iter->stack, &in_setof_choice);
   CharPtr         obj_use;
   CharPtr         asn_type_use;
   char            setof_objbuf[120];
   char            setof_asnbuf[120];

   obj_use = which_node->obj_name;
   asn_type_use = GenerateTypeFromStack (iter, !doing_outer_setof_choice);
   if (in_setof_choice) {
      sprintf (setof_objbuf, "%s_element", which_node->obj_name);
      sprintf (setof_asnbuf, "%s", GenerateTypeFromStack (iter, !doing_outer_setof_choice));
      obj_use = setof_objbuf;
      asn_type_use = setof_asnbuf;
   }
   sprintf (iter->buf, "\
	if (! loaded)\n\
	{\n\
		if (! %sAsnLoad()) {\n\
			return NULL;\n}\n\
	}\n\n", iter->module_name);
   AsnIterTakeBuf (iter);

   sprintf (iter->buf, "\
	if (aip == NULL) {\n\
		return NULL;\n}\n\n");
   AsnIterTakeBuf (iter);

   sprintf (iter->buf,
	    "if (orig == NULL) {         /* %s ::= (self contained) */\n\
		atp = AsnReadId(aip, amp, %s);\n} else {\n",
	    obj_use, asn_type_use);

   AsnIterTakeBuf (iter);

   sprintf (iter->buf, "\
		atp = AsnLinkType(orig, %s);    /* link in local tree */\n}\n\
    if (atp == NULL) {\n\
        return NULL;\n}\n\n", asn_type_use);
   AsnIterTakeBuf (iter);

   if (!doing_outer_setof_choice) {
      sprintf (iter->buf, "\
	anp = ValNodeNew(NULL);\n\
    if (anp == NULL) {\n\
        goto erret;\n}\n");
      AsnIterTakeBuf (iter);
      sprintf (iter->buf, "\
	if (AsnReadVal(aip, atp, &av) <= 0) { /* read the CHOICE or OpenStruct value (nothing) */\n\
        goto erret;\n}\n\n");
      AsnIterTakeBuf (iter);

      sprintf (iter->buf, "    func = NULL;\n\n");
      AsnIterTakeBuf (iter);
      sprintf (iter->buf, "\
	atp = AsnReadId(aip, amp, atp);  /* find the choice */\n\
    if (atp == NULL) {\n\
        goto erret;\n}\n");
      AsnIterTakeBuf (iter);
   }
}

/****************************************
*
* AsnCodeFunctionReadFuncSeqStart(iter,go_outer)
*
****************************************/

void
AsnCodeFunctionReadFuncSeqStart (AsnIterPtr iter, Boolean go_outer)
{
	AsnCodeNodePtr which_stack =
   go_outer? iter->stack -> outer :
   iter->stack;

   sprintf (iter->buf, "\
	if (! loaded)\n\
	{\n\
		if (! %sAsnLoad()) {\n\
			return NULL;\n}\n\
	}\n\n", iter->module_name);
   AsnIterTakeBuf (iter);

   sprintf (iter->buf, "\
	if (aip == NULL) {\n\
		return NULL;\n}\n\n");
   AsnIterTakeBuf (iter);

   sprintf (iter->buf, "\
	if (orig == NULL) {         /* %s ::= (self contained) */\n\
		atp = AsnReadId(aip, amp, %s);\n\
	} else {\n", which_stack->obj_name,
	    GenerateTypeFromStack (iter, FALSE)  /* 11/7/94 CAS */
      );
   AsnIterTakeBuf (iter);

   sprintf (iter->buf, "\
		atp = AsnLinkType(orig, %s);\n}\n    /* link in local tree */\n\
   if (atp == NULL) {\n\
        return NULL;\n}\n\n",
	    GenerateTypeFromStack (iter, FALSE)
      );
   AsnIterTakeBuf (iter);

   if ( ! go_outer) {
      /* since not going to use generic set readers, need these... */

      sprintf (iter->buf, "\
	ptr = %sNew();\n\
        if (ptr == NULL) {\n\
        goto erret;\n}\n", which_stack->obj_name);
      AsnIterTakeBuf (iter);
      sprintf (iter->buf, "\
	if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */\n\
        goto erret;\n}\n\n");
      AsnIterTakeBuf (iter);

      sprintf (iter->buf, "   atp = AsnReadId(aip,amp, atp);\n");
      AsnIterTakeBuf (iter);
   }

   sprintf (iter->buf, "\
	    func = NULL;\n\n");
   AsnIterTakeBuf (iter);
}

/****************************************************
*
* AsnCodeFunctionHeader(iter,type,name)
*
*****************************************/

void
AsnCodeFunctionHeader (AsnIterPtr iter, CharPtr type, CharPtr name)
{

   sprintf (iter->buf, "\n\n/**************************************************\n");
   AsnIterTakeBuf (iter);
   sprintf (iter->buf, "*\n");
   AsnIterTakeBuf (iter);
   sprintf (iter->buf, "*    %s%s()\n", name, type);
   AsnIterTakeBuf (iter);
   sprintf (iter->buf, "*\n");
   AsnIterTakeBuf (iter);
   sprintf (iter->buf, "**************************************************/\n");
   AsnIterTakeBuf (iter);

   sprintf (iter->buf, "%s",
	    AsnCodeStaticCandidate (iter) ? "static " : "NLM_EXTERN ");
   AsnIterTakeBuf (iter);

   if (StringCmp ("AsnWrite", type) == 0) {
      sprintf (iter->buf, "Boolean%s \n%s%s(", " LIBCALL", name, type);	/* ) */
   } else {
      sprintf (iter->buf, "\n %sPtr%s\n%s%s(", name, " LIBCALL", name, type);	/* ) */
   }
   AsnIterTakeBuf (iter);
}

/*****************************
*
* AsnCodeWhichValSlot(atp,valdefine)
*
*****************************/

CharPtr
AsnCodeWhichValSlot (AsnTypePtr atp, CharPtr PNTR valdefine)
{
   CharPtr         retval = NULL;
   static CharPtr  defptr = "ASNCODE_PTRVAL_SLOT";
   static CharPtr  defint = "ASNCODE_INTVAL_SLOT";
   static CharPtr  defbigint = "ASNCODE_BIGINTVAL_SLOT";
   static CharPtr  defreal = "ASNCODE_REALVAL_SLOT";
   static CharPtr  defbool = "ASNCODE_BOOLVAL_SLOT";
   static CharPtr  defbyte = "ASNCODE_BYTEVAL_SLOT";

   if (AsnCodeIsEnumType(atp)) {
      retval = "intvalue";
      if (valdefine != NULL) {
         *valdefine = defint;
      }
   } else {
      switch (atp->isa) {
         /********Uint1 types*******/
      case BOOLEAN_TYPE:
      case BITS_TYPE:
      case NULL_TYPE:
         retval = "boolvalue";
         if (valdefine != NULL) {
	    *valdefine = defbool;
         }
         break;
         /********Int4**********/
      case INTEGER_TYPE:
         retval = "intvalue";
         if (valdefine != NULL) {
	    *valdefine = defint;
         }
         break;
         /********Int8**********/
      case BIGINT_TYPE:
         retval = "bigintvalue";
         if (valdefine != NULL) {
	    *valdefine = defbigint;
         }
         break;
         /*------Uint2---------*/
      case ENUM_TYPE:
         retval = "intvalue";
         if (valdefine != NULL) {
	    *valdefine = defint;
         }
         break;
         /***********************************
          ****                           ****
          **** Pointer types, all are    ****
          **** handled by lower level    ****
          ****                           ****
          ***********************************/
      case OBID_TYPE:
      case OBDES_TYPE:
      case ANY_TYPE:
      case GENTIME_TYPE:
      default:
         retval = "ptrvalue";
         if (valdefine != NULL) {
	    *valdefine = defptr;
         }
         break;
         /*********Real****/
      case REAL_TYPE:
         retval = "realvalue";
         if (valdefine != NULL) {
	    *valdefine = defreal;
         }
         break;
         /*------CharPtr------*/
      case NUMERICSTRING_TYPE:
      case VISIBLESTRING_TYPE:
      case PRINTABLESTRING_TYPE:
         /*-------CharLenPtr, assume internally like CharPtr in C ---*/
      case TELETEXSTRING_TYPE:
      case VIDEOTEXSTRING_TYPE:
      case IA5STRING_TYPE:
      case GRAPHICSTRING_TYPE:
      case GENERALSTRING_TYPE:
      case CHARACTERSTRING_TYPE:
      case UTCTIME_TYPE:
         retval = "ptrvalue";
         if (valdefine != NULL) {
	    *valdefine = defptr;
         }
         break;
         /*------String Store --- handled in lower level, as ptrvalue */
      case STRSTORE_TYPE:
      case OCTETS_TYPE:
         retval = "ptrvalue";
         if (valdefine != NULL) {
	    *valdefine = defbyte;
         }
         break;
      }
   }
   return retval;
}

/***********************************************
*
* AsnCodeIterCall(iter, iter_SLOT,ATP,PLACETYPE)
*
**********************************8*/

void
AsnCodeIterCall (AsnIterPtr iter, FnPtr slot, AsnTypePtr atp, Int2 placetype)
{
   AsnCodePush (iter, iter->atp, placetype, ASNITER_RECUR_NONE);
   if (slot != NULL) {
      ((AsnCodeIterFunc) slot) ((struct struct_asniter PNTR) iter);
   }
   AsnCodePop (iter, iter->atp, placetype);

}

/**********************************************************************************
* AsnCodeIterRecursiveCall(iter, start_slot, end_slot, atp,placetype,recurtype)
*******************/

static void
AsnCodeIterRecursiveCall (AsnIterPtr iter, FnPtr start_slot, FnPtr end_slot, AsnTypePtr atp, Int2 placetype, Int2 recurtype)
{
   AsnTypePtr      branch_atp;

   if (start_slot != NULL) {
      ((AsnCodeIterFunc) start_slot) (iter);
   }
   /****************
    *  This recursive call is follow along SEQUENCE and CHOICE, etc.
    ***********/
   for (branch_atp = (AsnTypePtr) atp->branch; branch_atp != NULL;
	branch_atp = branch_atp->next) {
      iter->atp = branch_atp;
      iter->stack->slot_count++;
      AsnCodeIter (iter);	/* RECURSIVE CALL */
   }

   if (end_slot != NULL) {
      ((AsnCodeIterFunc) end_slot) (iter);
   }
}


/*******************************
*
* AsnCodePush(atp,type,recurtype)
*
*******************************/

void
AsnCodePush (AsnIterPtr iter, AsnTypePtr atp, Int2 type, Int2 recurtype)
{
   AsnCodeNodePtr  node = (AsnCodeNodePtr)MemNew (sizeof (AsnCodeNode));
   char            buf[60];
   AsnOptionPtr    slot_option;
   AsnOptionPtr    obj_option;

   /*
    * only get >1 in the non-Push recursive call at the end of AsnCodeIter ()
    */
   node->slot_count = 0;
   node->opt_count = 0;
   if (atp->name) {
      slot_option = AsnOptionGet ((iter->atp->hints), OP_NCBIASNTOOL,
				  OP_SLOT_NAME, (AsnOptionPtr) NULL);
      if (slot_option == (AsnOptionPtr) NULL) {
	 node->slot_name =
	    StringSave (AsnCodeCleanName (atp->name, buf,
					  CLEAN_FOR_SLOT, iter -> acip -> maxDefineLength));
      } else {
	 node->slot_name = StringSave ((char *)slot_option->data.ptrvalue);
	 if ((iter->acip->debug_level) > 1) {
	    AsnCodeShowDepth (iter);
	    fprintf ((iter->acip -> bug_fp), "... override SLOT name %s found.\n",
		     (CharPtr) TESTNIL (node->slot_name));
	 }
      }
      obj_option = AsnOptionGet ((iter->atp->hints), OP_NCBIASNTOOL,
				 OP_OBJ_NAME, (AsnOptionPtr) NULL);
      if (obj_option == (AsnOptionPtr) NULL) {
	 node->obj_name =
	    StringSave (AsnCodeCleanName (atp->name, buf,
					  CLEAN_FOR_OBJ_NAME, iter -> acip -> maxDefineLength));
      } else {
	 node->obj_name = StringSave ((char *)obj_option->data.ptrvalue);
	 if ((iter->acip->debug_level) > 1) {
	    AsnCodeShowDepth (iter);
	    fprintf ((iter->acip -> bug_fp), "... override OBJ name %s found.\n",
		     (CharPtr) TESTNIL (node->obj_name));
	 }
      }

      node->asn_type_name =
	 StringSave (AsnCodeCleanName (atp->name, buf,
				       CLEAN_FOR_ASN_TYPE, iter -> acip -> maxDefineLength));
   }
   node->recur_type = recurtype;
   node->atp = atp;
   iter->atp = atp;
   iter->depth++;
   if ((iter->acip->debug_level) > 1) {
      AsnCodeShowDepth (iter);
      fprintf ((iter->acip -> bug_fp), "PUSH:%s(%d)=%s=%s\n",
	       TESTNIL (node->slot_name), (int) type, TESTNIL (node->obj_name), TESTNIL (node->asn_type_name));
   }
   if (iter->stack != NULL) {
      iter->stack->inner = node;
   }
   node->outer = iter->stack;
   iter->stack = node;
   if (iter->base_of_stack == NULL) {
      iter->base_of_stack = node;
   }
   node->type = type;
}


/*******************************
*
* AsnCodePop(atp,type)
*
*******************************/

void
AsnCodePop (AsnIterPtr iter, AsnTypePtr atp, Int2 type)
{
   AsnCodeNodePtr  node = iter->stack;

   if (node == NULL) {
      ErrPost (CTX_NCBIASN1, 101,
	       "Poping empty stack");
      exit (2);
   }
   if (node->atp != atp || node->type != type) {
      ErrPost (CTX_NCBIASN1, 101,
	       "AsnCodePop stack corrupted, expected: %s-%d , saw %s-%d",
	       atp->name ? atp->name : "No atp name", (int) type,
	       node->atp->name ? node->atp->name :
	       "No node -> atp name", (int) type);
      exit (1);
   }
   iter->stack = iter->stack->outer;
   if (iter->stack == NULL) {
      iter->base_of_stack = NULL;
   } else {
      iter->stack->inner = NULL;
   }
   iter->atp = iter->stack ? iter->stack->atp : NULL;
   if ((iter->acip->debug_level) > 1) {
      AsnCodeShowDepth (iter);
      fprintf ((iter->acip -> bug_fp), "POP\n");
   }
   iter->depth--;
   MemFree (node->slot_name);
   MemFree (node->obj_name);
   MemFree (node->asn_type_name);
   MemFree (node);
}

/***************************************
*
* AsnIterNodeBack(node,recurPt)
*
***************************************/

static AsnCodeNodePtr
AsnIterNodeBack (AsnCodeNodePtr node, Int2Ptr recurPt)
{
   AsnCodeNodePtr  retval = NULL;
   if (node) {
      retval = node->outer;
      /*------- the context of the call is really one up from here
               is this is handling an unnamed SEQUENCE or CHOICE ---*/
      if (node->type == ASNITER_Pseudo_USER_TYPE)
	 retval = retval->outer;
      if (retval == NULL)
	 return retval;
      if (retval->recur_type == ASNITER_RECUR_WAS_LAST)
	 retval = retval->outer;
      if (recurPt != NULL) {
	 *recurPt = retval->recur_type;
      }
   }
   return retval;
}

/************************
*
* AsnCodeIterRecurLevel(iter)
*
************************/

Int2
AsnCodeIterRecurLevel (AsnIterPtr iter)
{
   Int2            retval = ASNITER_RECUR_NONE;
   AsnCodeNodePtr  node, back_node;
   Int2            recur = ASNITER_RECUR_NONE, previous_recur = ASNITER_RECUR_NONE;

   if (iter) {
      node = iter->stack;
      node = AsnIterNodeBack (node, &recur);
      back_node = AsnIterNodeBack (node, &previous_recur);
      switch (recur) {
      case ASNITER_RECUR_NONE:
	 retval = RECUR_OUTER;
	 break;
      case ASNITER_RECUR_SEQSET:
	 retval = RECUR_SEQSET;
	 break;
      case ASNITER_RECUR_CHOICE:
	 retval = RECUR_CHOICE;
	 break;
      case ASNITER_RECUR_SETOF:

	 /*
	  * If this while loop fires, we are unlikely to really handle this
	  * case.  However, the form is so bad, we will let it pass, for now.
	  */
	 while (previous_recur == ASNITER_RECUR_SETOF)
	    back_node = AsnIterNodeBack (back_node, &previous_recur);

	 switch (previous_recur) {
	 case ASNITER_RECUR_NONE:
	    retval = RECUR_SETOF_OUTER;
	    break;
	 case ASNITER_RECUR_SEQSET:
	    retval = RECUR_SETOF_SEQSET;
	    break;
	 case ASNITER_RECUR_CHOICE:
	    retval = RECUR_SETOF_CHOICE;
	    break;
	 }
	 break;
      }
   }
   return retval;
}

/******************************
*
*  AsnCodeIter(atp)
*
*******************************/
void
AsnCodeIter (AsnIterPtr iter)
{

   if (iter == NULL)
      return;

   if (iter->atp != NULL)
      if (iter->atp->type != NULL) {
	 if (AsnCodeIsEnumType (iter->atp->type)) {
	    AsnCodeIterCall (iter, iter->basetype, iter->atp, ENUM_TYPE);
	 } else if (iter->atp->type->isa < 301) {
	    AsnCodeIterCall (iter, iter->less301err, NULL, 0);
	    /*------- USER OBJECTS -------*/
	 } else if (iter->atp->type->isa > 400) {
	    AsnCodeIterCall (iter, iter->userobj, iter->atp,
			     ASNITER_USER_TYPE);

	    /*------SETs and SEQUENCEs ---*/

	 } else if (iter->atp->type->isa == SEQ_TYPE) {
	    AsnCodePush (iter, iter->atp, SEQ_TYPE, ASNITER_RECUR_SEQSET);
	    AsnCodeIterRecursiveCall (iter, iter->seqstart,
				      iter->seqend, iter->atp, SEQ_TYPE,
				      ASNITER_RECUR_SEQSET);

	    AsnCodePop (iter, iter->atp, SEQ_TYPE);
	 } else if (iter->atp->type->isa == SET_TYPE) {
	    AsnCodePush (iter, iter->atp, SET_TYPE, ASNITER_RECUR_SEQSET);
	    AsnCodeIterRecursiveCall (iter, iter->setstart, iter->setend,
				 iter->atp, SET_TYPE, ASNITER_RECUR_SEQSET);

	    AsnCodePop (iter, iter->atp, SET_TYPE);

	    /*-------- CHOICE ---*/

	 } else if (iter->atp->type->isa == CHOICE_TYPE) {
	    AsnCodePush (iter, iter->atp, CHOICE_TYPE, ASNITER_RECUR_CHOICE);
	    AsnCodeIterRecursiveCall (iter, iter->choicestart, iter->choiceend, iter->atp,
				      CHOICE_TYPE, ASNITER_RECUR_CHOICE);

	    AsnCodePop (iter, iter->atp, CHOICE_TYPE);


	    /*----- SET OF or SEQ OF-------*/
	 } else if (iter->atp->type->isa == SETOF_TYPE ||
		    iter->atp->type->isa == SEQOF_TYPE) {
	    AsnCodePush (iter, iter->atp, iter->atp->type->isa, ASNITER_RECUR_SETOF);
	    iter->atp = (AsnTypePtr) (iter->atp->branch);
	    AsnCodeIter (iter);
	    AsnCodePop (iter, iter->atp, iter->atp->type->isa);
	 } else if (ISA_BASETYPE (iter->atp->type->isa)) {
	    AsnCodeIterCall (iter, iter->basetype, iter->atp,
			     ASNITER_BASE_TYPE);
	 }
      } else {
	 AsnCodeIterCall (iter, iter->undef301err, NULL, ASNITER_LESS300);
      }


   return;
}
void
AsnCode (AsnCodeInfoPtr acip)
{
   FILE           *fp;
   int             numvalue = 0, numtype = 0, nummod = 0;
   AsnProc         p;
   AsnModulePtr    curr_mod;
   AsnTypePtr      curr_type, atp;
   AsnTmpTypePtr   attp;
   AsnTmpValuePtr  attvp;
   char            buf[BUF_LEN];
   CharPtr         pnt, hold_buf;
   CharPtr         include_name = NULL;
   AsnIterPtr      iter = (AsnIterPtr)MemNew (sizeof (AsnIter));
   Int2            mode;
   Char            dateTime[24];

   CharPtr loadname = acip -> loadname;
   CharPtr filename = acip -> filename;
   Boolean do_bit_twiddle  = acip -> do_bit_twiddle;
   Boolean force_choice_struct = acip -> force_choice_struct;
   CharPtr include_filename = acip ->  include_filename;
   CharPtr object_manager_entry = acip -> object_manager_entry;
   CharPtr objlabel = acip->object_label;
   CharPtr objnameuc = NULL, objnamemc = NULL, objnamety = NULL;
   char            objnamebuf[BUF_LEN];
   AsnModulePtr amp = acip -> amp;
   AsnModulePtr last_amp = acip -> last_amp;

   CharPtr         use_load = StringSave (loadname);

   iter -> acip = acip;
   iter -> do_opt_bits = do_bit_twiddle;
   iter -> do_chs_struct = force_choice_struct;
   StringCpy (buf, filename);
   /*-----remove any erroneous extension----*/
   pnt = buf;
   while (*pnt != '\0')
      pnt++;
   while ((pnt > buf) && (*pnt != '.'))
      pnt--;
   if (*pnt == '.')
      *pnt = '\0';
   hold_buf = StringSave (buf);
   StringCat (buf, ".h");
   include_name = StringSave (buf);

   fp = fopen (buf, "w");

   if (fp == NULL) {
      AsnIoErrorMsg (NULL, 82, buf);
      return;
   }
   curr_mod = amp;
   p.curr_tmptype = &p.root_tmptype;	/* fake node to start */
   p.curr_tmpvalue = &p.root_tmpvalue;
   p.root_tmptype.next = NULL;
   p.root_tmpvalue.next = NULL;

   while (curr_mod != NULL) {
      nummod++;
      curr_type = curr_mod->types;
      while (curr_type != NULL) {
	 curr_type = AsnOutAddType (&p, curr_type);
      }
      curr_mod = curr_mod->next;
   }

   /******************** set index values ***************************/

   attvp = p.root_tmpvalue.next;
   while (attvp != NULL) {
      attvp->index = numvalue;
      numvalue++;
      attvp = attvp->next;
   }

   attp = p.root_tmptype.next;
   while (attp != NULL) {
      attp->index = numtype;
      numtype++;
      attp = attp->next;
   }


   iter->fp = fp;

   iter->indent = 0;
   iter->leadingspaces = TRUE;
   iter->basetype = (FnPtr) AsnCode_basetype;
   iter->choiceend = (FnPtr) AsnCode_choiceend;
   iter->choicestart = (FnPtr) AsnCode_choicestart;
   iter->less301err = (FnPtr) AsnIterGenericDie301;
   iter->seqend = (FnPtr) AsnCode_seqend;
   iter->seqstart = (FnPtr) AsnCode_seqstart;
   iter->setstart = (FnPtr) AsnCode_setstart;
   iter->setend = (FnPtr) AsnCode_setend;
   iter->undef301err = (FnPtr) AsnCode_under301err;
   iter->userobj = (FnPtr) AsnCode_userobj;
   AsnCodeNewRoutine (iter);

   /*
    * Pre generation analysis for assigning names to unnamed internal objects
    * and setting the flag to note that an object need be used as a linked
    * list.
    */
   for (curr_mod = amp; curr_mod; curr_mod = curr_mod->next) {
      for (mode = ITER_ASSIGN; mode <= ITER_CHECK_LIST; mode++) {
	 iter->mode = mode;
	 atp = curr_mod->types;
	 /*------- loop through each TYPE ::= along the main list ---*/
	 for (; atp != NULL; atp = atp->next) {
	    if ((acip->debug_level) >= 1) {
	       fprintf ((acip -> bug_fp), "MODE:%d:name=%s\n",
		     (int) mode, atp->name != NULL ? atp->name : "No NAME");
	       fflush ((acip -> bug_fp));
	    }
	    if (!atp->imported) {	/* must be defined elsewhere */
	       iter->atp = atp;
	       AsnCodeIter (iter);
	       fflush (fp);
	    }
	 }
	 fflush (fp);
      }
      /* --- if just did last module in first filename, stop --- */
      if (curr_mod == last_amp && mode == ITER_ASSIGN)
	 break;
   }

   /*
    * Object (*.h file with C object definitions) Generated here
    */
   mode = ITER_OBJECTS;
   iter->mode = mode;

   for (curr_mod = amp; curr_mod; curr_mod = curr_mod->next) {
      atp = curr_mod->types;
      if (curr_mod == amp) {
	 iter->module_name = StringSave (hold_buf);
	 sprintf (iter->buf, "#ifndef _%s_ \n", hold_buf);
	 AsnIterTakeBuf (iter);
	 sprintf (iter->buf, "#define _%s_ \n", hold_buf);
	 AsnIterTakeBuf (iter);
	 sprintf (iter->buf, "\n#undef NLM_EXTERN\n#ifdef NLM_IMPORT\n#define NLM_EXTERN NLM_IMPORT\n#else\n#define NLM_EXTERN extern\n#endif\n\n");
	 AsnIterTakeBuf (iter);
	 sprintf (iter->buf, "\n#ifdef __cplusplus\nextern \"C\" { /* } */\n#endif\n");
	 AsnIterTakeBuf (iter);
      }
      sprintf (iter->buf, "\n\n/**************************************************\n");
      AsnIterTakeBuf (iter);
      sprintf (iter->buf, "*\n");
      AsnIterTakeBuf (iter);
      sprintf (iter->buf, "*    Generated objects for Module %s\n", curr_mod->modulename);
      AsnIterTakeBuf (iter);
      Nlm_DayTimeStr(dateTime, TRUE, TRUE);
      sprintf (iter->buf, "*    Generated using ASNCODE %sat %s\n", strtok(RCS_Rev, "$"), dateTime);
      AsnIterTakeBuf (iter);
      sprintf (iter->buf, "*\n");
      AsnIterTakeBuf (iter);
      sprintf (iter->buf, "**************************************************/\n");
      AsnIterTakeBuf (iter);

      if (curr_mod == amp) {
         sprintf (iter->buf, "\nNLM_EXTERN Boolean LIBCALL\n%sAsnLoad PROTO((void));\n",
	          iter->module_name);
         AsnIterTakeBuf (iter);
      }

      AsnCodeFinishedRoutine (iter);
      AsnCodeNewRoutine (iter);

      /*------- loop through each TYPE ::= along the main list ---*/
      for (; atp != NULL; atp = atp->next) {
	 if ((acip->debug_level) >= 1) {
	    fprintf ((acip -> bug_fp), "MODE:%d:name=%s\n",
		     (int) mode, atp->name != NULL ? atp->name : "No NAME");
	    fflush ((acip -> bug_fp));
	 }
	 if (!atp->imported) {	/* must be defined elsewhere */
	    iter->atp = atp;
	    AsnCodeNewRoutine (iter);
	    AsnCodeIter (iter);
	    AsnCodeFinishedRoutine (iter);
	    AsnCodeCheckFileDoneRoutines (iter);
	    fflush (fp);
	 }
      }

      if (curr_mod == last_amp) {
         AsnCodeNewRoutine (iter);
         sprintf (iter->buf, "#ifdef __cplusplus\n/* { */ }\n#endif\n\n");
         AsnIterTakeBuf (iter);
         sprintf (iter->buf, "#endif /* _%s_ */\n", hold_buf);
         AsnIterTakeBuf (iter);
	 sprintf (iter->buf, "\n#undef NLM_EXTERN\n#ifdef NLM_EXPORT\n#define NLM_EXTERN NLM_EXPORT\n#else\n#define NLM_EXTERN\n#endif\n\n");
	 AsnIterTakeBuf (iter);
         AsnCodeFinishedRoutine (iter);
         AsnCodeCheckFileDoneRoutines (iter);
      }

      fflush (fp);

      /* --- if just did last module in first filename, stop --- */
      if (curr_mod == last_amp)
	 break;
   }
   fclose (fp);
   StringCpy (buf, hold_buf);
   StringCat (buf, ".c");

   fp = fopen (buf, "w");
   iter->fp = fp;
   AsnCodeNewRoutine (iter);
   if (fp == NULL) {
      AsnIoErrorMsg (NULL, 82, buf);
      return;
   }
   sprintf (iter->buf, "#include <asn.h>\n");
   AsnIterTakeBuf (iter);
   sprintf (iter->buf, "\n#define NLM_GENERATED_CODE_PROTO\n\n");
   AsnIterTakeBuf (iter);
   if (include_filename) {
      sprintf (iter->buf,
	       "#include <%s>\n", include_filename);
      AsnIterTakeBuf (iter);
   }
   sprintf (iter->buf, "#include <%s>\n", include_name);
   AsnIterTakeBuf (iter);
   sprintf (iter->buf, "\nstatic Boolean loaded = FALSE;\n\n");
   AsnIterTakeBuf (iter);
   StringCpy (buf, use_load);
   /*-----remove any erroneous extension----*/
   pnt = buf;
   while (*pnt != '\0')
      pnt++;
   while ((pnt > buf) && (*pnt != '.'))
      pnt--;
   if (*pnt == '.')
      *pnt = '\0';
   hold_buf = StringSave (buf);
   StringCat (buf, ".h");
   include_name = StringSave (buf);
   sprintf (iter->buf, "#include <%s>\n\n", include_name);
   AsnIterTakeBuf (iter);
   if (object_manager_entry != NULL) {
     if (objlabel == NULL) {
       objlabel = object_manager_entry;
     }
     objnamety = StringSave (AsnCodeCleanName (object_manager_entry, objnamebuf, CLEAN_FOR_SLOT, sizeof (objnamebuf)));
     objnameuc = StringSave (AsnCodeCleanName (object_manager_entry, objnamebuf, SLOT_POLISH, sizeof (objnamebuf)));
     objnamemc = StringSave (AsnCodeCleanName (object_manager_entry, objnamebuf, CLEAN_FOR_OBJ_NAME, sizeof (objnamebuf)));
   }
   sprintf (iter->buf, "#ifndef NLM_EXTERN_LOADS\n#define NLM_EXTERN_LOADS {}\n#endif\n\n");
   AsnIterTakeBuf (iter);
   if (object_manager_entry != NULL) {
     sprintf (iter->buf, "static Pointer LIBCALLBACK %sNewFunc (void)\n{\n\treturn (Pointer) %sNew();\n}\n\n", objnamemc, objnamemc);
     AsnIterTakeBuf (iter);
     sprintf (iter->buf, "static Pointer LIBCALLBACK %sFreeFunc (Pointer data)\n{\n\treturn (Pointer) %sFree ((%sPtr) data);\n}\n\n", objnamemc, objnamemc, objnamemc);
     AsnIterTakeBuf (iter);
     sprintf (iter->buf, "static Boolean LIBCALLBACK %sAsnWriteFunc (Pointer data, AsnIoPtr aip, AsnTypePtr atp)\n{\n", objnamemc);
     AsnIterTakeBuf (iter);
     sprintf (iter->buf, "\treturn %sAsnWrite ((%sPtr) data, aip, atp);\n}\n\n", objnamemc, objnamemc);
     AsnIterTakeBuf (iter);
     sprintf (iter->buf, "static Pointer LIBCALLBACK %sAsnReadFunc (AsnIoPtr aip, AsnTypePtr atp)\n{\n", objnamemc);
     AsnIterTakeBuf (iter);
     sprintf (iter->buf, "\treturn (Pointer) %sAsnRead (aip, atp);\n}\n\n", objnamemc, objnamemc);
     AsnIterTakeBuf (iter);
     sprintf (iter->buf, "static Int2 LIBCALLBACK %sLabelFunc (Pointer data, CharPtr buffer, Int2 buflen, Uint1 content)\n{\n", objnamemc);
     AsnIterTakeBuf (iter);
     sprintf (iter->buf, "\treturn %sLabel ((%sPtr) data, buffer, buflen, content);\n}\n\n", objnamemc, objnamemc);
     AsnIterTakeBuf (iter);
   }
   sprintf (iter->buf, "NLM_EXTERN Boolean LIBCALL\n%sAsnLoad(void)\n{\n\n\tif ( ! loaded) {\n",
	    iter->module_name);
   AsnIterTakeBuf (iter);
      sprintf (iter->buf, "\t\tNLM_EXTERN_LOADS\n\n\t\tif ( ! AsnLoad ())\n\t\t\treturn FALSE;\n");
   AsnIterTakeBuf (iter);
   sprintf (iter->buf, "\t\tloaded = TRUE;\n\t}\n\n");
   AsnIterTakeBuf (iter);
   if (object_manager_entry != NULL) {
     sprintf (iter->buf, "\tObjMgrTypeLoad (OBJ_%s, \"%s\", \"%s\", \"%s\",\n",
              objnameuc, object_manager_entry, objnamemc, objlabel);
     AsnIterTakeBuf (iter);
     sprintf (iter->buf, "\t\t%s, %sNewFunc, %sAsnReadFunc, %sAsnWriteFunc,\n",
              objnameuc, objnamemc, objnamemc, objnamemc);
     AsnIterTakeBuf (iter);
     sprintf (iter->buf, "\t\t%sFreeFunc, %sLabelFunc, %sSubTypeFunc);\n\n",
              objnamemc, objnamemc, objnamemc);
     AsnIterTakeBuf (iter);
   }
   sprintf (iter->buf, "\treturn TRUE;\n}\n\n");
   AsnIterTakeBuf (iter);
      sprintf (iter->buf, "\n\n/**************************************************\n");
   AsnIterTakeBuf (iter);
   sprintf (iter->buf, "*    Generated object loaders for Module %s\n",
	    curr_mod->modulename);
   AsnIterTakeBuf (iter);
   Nlm_DayTimeStr(dateTime, TRUE, TRUE);
   sprintf (iter->buf, "*    Generated using ASNCODE %sat %s\n", strtok(RCS_Rev, "$"), dateTime);
   AsnIterTakeBuf (iter);
   sprintf (iter->buf, "*\n");
   AsnIterTakeBuf (iter);
   sprintf (iter->buf, "**************************************************/\n");
   AsnIterTakeBuf (iter);

   AsnCodeFinishedRoutine (iter);
   AsnCodeCheckFileDoneRoutines (iter);


   /*
    * Actual Object Loaders/Writers Generated here
    */
   for (curr_mod = amp; curr_mod; curr_mod = curr_mod->next) {
      /*------- loop through each TYPE ::= along the main list ---*/
      for (atp = curr_mod->types; atp != NULL; atp = atp->next) {
	 if ((acip->debug_level) >= 1) {
	    fprintf ((acip -> bug_fp), "MODE:%d:name=%s\n",
		     (int) mode, (atp->name != NULL ? atp->name : "No NAME"));
	    fflush ((acip -> bug_fp));
	 }
	 for (mode = ITER_NEW; mode <= ITER_WRITE; mode++) {
	    iter->mode = mode;
	    iter->atp = atp;
	    AsnCodeNewRoutine (iter);
	    AsnCodeIter (iter);
	    AsnCodeFinishedRoutine (iter);
	 }
	 /* Do all 4 modes for an atp, then produce output to file */
	 AsnCodeCheckFileDoneRoutines (iter);
	 fflush (fp);
      }
      if (curr_mod == last_amp)
	 break;
   }



   MemFree (iter);

   fclose (fp);
   return;
}
