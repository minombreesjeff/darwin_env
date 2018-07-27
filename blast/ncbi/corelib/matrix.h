/*  $Id: matrix.h,v 6.1 1999/04/02 20:43:30 vakatov Exp $
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*            National Center for Biotechnology Information (NCBI)
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government do not place any restriction on its use or reproduction.
*  We would, however, appreciate having the NCBI and the author cited in
*  any work or product based on this material
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
* ===========================================================================
*
* Author:  Denis Vakatov
*
* File Description: 
*   Basic matrix & vector handling library.
*   NOTE:  most of the advanced functions are valid for square matrices only!
*
* ===========================================================================
* $Log: matrix.h,v $
* Revision 6.1  1999/04/02 20:43:30  vakatov
* DLL'd for MS-Win
* Added detailed API function descriptions
*
* ==========================================================================
*/

#ifndef MATRIX_H
#define MATRIX_H

#include <ncbilcl.h>
#include <ncbistd.h>

/* The following preprocessor variables can be set on the compiling stage:
 * #TEST_MODULE_MATRIX -- to build test application (needs CORELIB library)
 * #CHECK_RANGE_MATRIX -- to check for the matrix indexes be in-range (slow) 
 */

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif


/* Forward declaration of the "Nlm_Matrix" internal structure.
 */
struct Nlm_MatrixTag;
typedef struct Nlm_MatrixTag *Nlm_Matrix;


/* Create new matrix("n_row" X "n_column").
 */
NLM_EXTERN Nlm_Matrix Nlm_MatrixNew
(Nlm_Uint4 n_row,
 Nlm_Uint4 n_column
 );

/* Destroy matrix "mat".
 */
NLM_EXTERN void Nlm_MatrixDelete
(Nlm_Matrix mat
 );

/* Assign the node of matrix "mat" located in the specified "row" and "column"
 * to value "value".
 */
NLM_EXTERN void Nlm_MatrixSetNode
(Nlm_Matrix  mat,
 Nlm_Uint4   row,
 Nlm_Uint4   column,
 Nlm_FloatHi value
 );

/* Assign all nodes in the whole row "row" of matrix "mat" to values
 * of the relevant nodes in row "row_src" of matrix "mat_src".
 * NOTE:  "mat" and "src_mat" must have the same # of columns!
 */
NLM_EXTERN void Nlm_MatrixSetRow
(Nlm_Matrix       mat,
 Nlm_Uint4        row,
 const Nlm_Matrix mat_src,
 Nlm_Uint4        row_src
);

/* Assign all nodes in the whole column "column" of matrix "mat" to values
 * of the relevan nodes in column "column_src" of matrix "mat_src".
 * NOTE:  "mat" and "src_mat" must have the same # of rows!
 */
NLM_EXTERN void Nlm_MatrixSetColumn
(Nlm_Matrix       mat,
 Nlm_Uint4        column,
 const Nlm_Matrix mat_src,
 Nlm_Uint4        column_src
 );

/* Return the value of specified matrix node.
 */
NLM_EXTERN Nlm_FloatHi Nlm_MatrixNode
(const Nlm_Matrix mat,
 Nlm_Uint4        row,
 Nlm_Uint4        column
 );

/* Create a vector and assign it with the
 * values from the row "row" of matrix "mat".
 * Return the created vector.
 */
NLM_EXTERN Nlm_Matrix Nlm_MatrixRow
(const Nlm_Matrix mat,
 Nlm_Uint4        row
 );

/* Create a transposed vector and assign it with the
 * values from the column "column" of matrix "mat".
 * Return the created transposed vector.
 */
NLM_EXTERN Nlm_Matrix Nlm_MatrixColumn
(const Nlm_Matrix mat,
 Nlm_Uint4        column
 );

/* Compare the two matrices;  return TRUE if they are identical.
 * NOTE:  "mat1" and "mat2" must have the same # of rows and columns!
 */
NLM_EXTERN Nlm_Boolean Nlm_MatrixCompare
(const Nlm_Matrix mat1,
 const Nlm_Matrix mat2
 );

/* Create and return an exact copy of matrix "mat".
 */
NLM_EXTERN Nlm_Matrix Nlm_MatrixCopy
(const Nlm_Matrix mat
 );

/* Create and return transposition of matrix "mat".
 */
NLM_EXTERN Nlm_Matrix Nlm_MatrixTranspose
(const Nlm_Matrix mat
 );

/* Return the result of multiplication of the two matrices.
 * NOTE 1:  the order is important, as in most cases:  m1*m2 != m2*m1
 * NOTE 2:  "mat_left" must have exactly as many columns as # of rows
 *          in "mat_right"!
 */
NLM_EXTERN Nlm_Matrix Nlm_MatrixMultiply
(const Nlm_Matrix mat_left,
 const Nlm_Matrix mat_right
 );

/* Solve equation:  (mat) * |V| = |vector|.
 * Return the found solution(vector |V|).
 * NOTE 1:  "vector" must be a 1-column matrix(i.e. vector)!
 * NOTE 2:  matrix "mat" must be square!
 */
NLM_EXTERN Nlm_Matrix Nlm_MatrixSolve
(const Nlm_Matrix mat,
 const Nlm_Matrix vector
 );

/* Solve equation:  (mat) * (M) = (1).
 * Return the found solution(matrix (M)).
 * NOTE 1:  matrix "mat" must be square!
 */
NLM_EXTERN Nlm_Matrix Nlm_MatrixInvert
(const Nlm_Matrix mat
 );

/* Printout the content of matrix "mat" to the file stream "fd",
 * with title "descr".
 */
NLM_EXTERN void Nlm_MatrixPrint
(const Nlm_Matrix mat,
 FILE*            fd,
 const Char*      descr
 );

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif  /* MATRIX_H */
