/**************************************************************************
*                                                                         *
*                             COPYRIGHT NOTICE                            *
*                                                                         *
* This software/database is categorized as "United States Government      *
* Work" under the terms of the United States Copyright Act.  It was       *
* produced as part of the author's official duties as a Government        *
* employee and thus can not be copyrighted.  This software/database is    *
* freely available to the public for use without a copyright notice.      *
* Restrictions can not be placed on its present or future use.            *
*                                                                         *
* Although all reasonable efforts have been taken to ensure the accuracy  *
* and reliability of the software and data, the National Library of       *
* Medicine (NLM) and the U.S. Government do not and can not warrant the   *
* performance or results that may be obtained by using this software,     *
* data, or derivative works thereof.  The NLM and the U.S. Government     *
* disclaim any and all warranties, expressed or implied, as to the        *
* performance, merchantability or fitness for any particular purpose or   *
* use.                                                                    *
*                                                                         *
* In any work or product derived from this material, proper attribution   *
* of the author(s) as the source of the software or data would be         *
* appreciated.                                                            *
*                                                                         *
**************************************************************************/
/* $Revision 1.0$ *
* $Log: dotmatrx.h,v $
* Revision 6.2  1999/03/17 17:05:49  madden
* removed comment from within comment
*
* Revision 6.1  1998/01/26 17:23:42  madden
* Changes to allow color-coding of alignments
*
 * Revision 6.0  1997/08/25 18:21:46  madden
 * Revision changed to 6.0
 *
 * Revision 1.2  1997/01/06 19:26:42  madden
 * Changed FnPtr to (LIBCALLBACK *callback... in DotMatrixSearch and CreateDotMatrixForm.
 *
 * Revision 1.1  1997/01/03  22:01:27  madden
 * Initial revision
 *
 * Initial revision
 *
*/

#include <vibrant.h>
#include <picture.h>
#include <viewer.h>
#include <objseq.h>
#include <objsset.h>

#ifndef __DOTMATRIX__
#define __DOTMATRIX__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dotmatrix_alignment {
	struct dotmatrix_alignment PNTR next;
	SeqAlignPtr sap;
	Uint1Ptr color;	/* Defined in picture.[ch] */
} DotMatrixAlignment, PNTR DotMatrixAlignmentPtr;

DotMatrixAlignmentPtr LIBCALL DotMatrixAlignmentNew (SeqAlignPtr sap, Uint1Ptr color, DotMatrixAlignmentPtr PNTR old);

Int2 LIBCALLBACK DotMatrixGenFunc (Pointer data);

ForM LIBCALL CreateDotMatrixForm PROTO((SeqAlignPtr sap, int (LIBCALLBACK *callback)PROTO((SeqAlignPtr seqalign))));

ForM LIBCALL CreateDotMatrixFormEx (DotMatrixAlignmentPtr dmap, int (LIBCALLBACK *callback)PROTO((SeqAlignPtr seqalign)));

Int2 LIBCALL DotMatrixSearch PROTO((SeqIdPtr sip1, SeqIdPtr sip2, int (LIBCALLBACK *callback)PROTO((SeqAlignPtr seqalign))));

#define REGISTER_SEQALIGN_DOT_MATRIX ObjMgrProcLoad (OMPROC_FILTER, \
"Dot Matrix", "DotMatrix", OBJ_SEQALIGN, 0, OBJ_SEQALIGN, 0, NULL, \
DotMatrixGenFunc, PROC_PRIORITY_DEFAULT)

#ifdef __cplusplus
}
#endif
#endif /* !__DOTMATRIX__ */
