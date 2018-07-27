/*  $Id: matrix.c,v 6.1 1999/04/02 20:43:30 vakatov Exp $
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
*   NOTE:  most of advanced functions are valid for square matrices only!
*
* ===========================================================================
* $Log: matrix.c,v $
* Revision 6.1  1999/04/02 20:43:30  vakatov
* DLL'd for MS-Win
* Added detailed API function descriptions
*
* ==========================================================================
*/

#include <matrix.h>
#include <ncbimem.h>
#include <ncbierr.h>


typedef struct Nlm_MatrixTag {
  Nlm_Uint4 n_row;
  Nlm_Uint4 n_column;
  Nlm_FloatHi m[1];
} Nlm_MatrixStruct;


#ifdef CHECK_RANGE_MATRIX
static Nlm_FloatHiPtr VALPtr(Nlm_Matrix mat, Nlm_Uint4 row, Nlm_Uint4 column)
{
  ASSERT ( row < mat->n_row  &&  column < mat->n_column );
  return &mat->m[row * mat->n_column + column];
}
#define VAL(mat,row,column) (*VALPtr(mat,row,column))
#else  /* CHECK_RANGE_MATRIX */
#define VAL(matrix, row, column) (matrix->m[row * matrix->n_column + column])
#endif /* CHECK_RANGE_MATRIX */



#define MAT_SIZE(mat) (sizeof(Nlm_MatrixStruct) + (mat->n_row * mat->n_column - 1) * sizeof(Nlm_FloatHi))


/***********************************************************************
 *  INTERNAL
 ***********************************************************************/


static Nlm_Boolean lu_decompose
(Nlm_Matrix   mat,
 Nlm_Uint4Ptr indx)
{
  Nlm_Uint4 i, j, k, imax = UINT4_MAX;
  Nlm_FloatHi max, temp, sum;

  Nlm_Uint4 n = mat->n_row;
  Nlm_FloatHiPtr row_scale = (Nlm_FloatHiPtr)
    Nlm_MemNew(sizeof(Nlm_FloatHi) * n);

  ASSERT ( mat->n_row == mat->n_column );

  for (i = 0;  i < n;  i++)
    {
      max = 0.0;
      for (j = 0;  j < n;  j++)
        if ((temp = fabs( VAL(mat,i,j) )) > max)
          max = temp;

      if (max == 0.0) {
        Nlm_MemFree( row_scale );
        return FALSE;
      }

      row_scale[i] = 1.0 / max;
      indx[i] = i;
    }

  for (j = 0;  j < n;  j++)
    {
      for (i = 0;  i < j;  i++)
        {
          sum = VAL(mat,i,j);
          for (k = 0;  k < i;  k++)
            sum -= VAL(mat,i,k) * VAL(mat,k,j);
          VAL(mat,i,j) = sum;
        }

      max = 0.0;
      for (i = j;  i < n;  i++)
        {
          sum = VAL(mat,i,j);
          for (k = 0;  k < j;  k++)
            sum -= VAL(mat,i,k) * VAL(mat,k,j);
          VAL(mat,i,j) = sum;

          if ((temp = row_scale[i] * fabs(sum)) >= max) {
            max = temp;
            imax = i;
          }
        }
      
      if (j != imax)
        { /* row interchange */
          for (k = 0;  k < n;  k++)
            {
              temp = VAL(mat,imax,k);
              VAL(mat,imax,k) = VAL(mat,j,k);
              VAL(mat,j,k) = temp;
            }

          row_scale[imax] = row_scale[j];

          k = indx[j];
          indx[j] = indx[imax];
          indx[imax] = k;
        }

      if ((j+1) != n)
        {
          temp = 1.0 / VAL(mat,j,j);
          for (i = j + 1;  i < n;  i++)
            VAL(mat,i,j) *= temp;
        }
    }

  Nlm_MemFree( row_scale );
  return TRUE;
}


static Nlm_Matrix lu_back_subst
(const Nlm_Matrix mat,
 Uint4Ptr         indx,
 const Nlm_Matrix vector)
{
  Uint4 i,j;
  Uint4 n = mat->n_row;
  Uint4 i_nonzero = n;  /* it means -- only zero nodes up to now */

  Nlm_Matrix res_vector = Nlm_MatrixNew(vector->n_row, 1);
  for (i = 0;  i < res_vector->n_row;  i++)
    VAL(res_vector, i, 0) = VAL(vector, indx[i], 0);

  ASSERT ( mat->n_row == mat->n_column );
  ASSERT ( mat->n_row == res_vector->n_row );

  for (i = 0;  i < n;  i++)
    {
      FloatHi sum = VAL(res_vector, i,0);

      if (i_nonzero != n)
        for (j = i_nonzero;  j < i;  j++)
          sum -= VAL(mat, i,j) * VAL(res_vector, j,0);
      else if ( sum )
        i_nonzero = i;

      VAL(res_vector, i,0) = sum;
    }

  for (i = n;  i-- > 0; )
    {
      FloatHi sum = VAL(res_vector, i,0);
      for (j = i+1;  j < n;  j++)
        sum -= VAL(mat, i,j) * VAL(res_vector, j,0);
      VAL(res_vector, i,0) = sum / VAL(mat, i,i);
    }

  return res_vector;
}



/***********************************************************************
 *  EXTERNAL
 ***********************************************************************/


NLM_EXTERN Nlm_Matrix Nlm_MatrixNew
(Nlm_Uint4 n_row,
 Nlm_Uint4 n_column)
{
  Nlm_Matrix mat = (Nlm_Matrix)
    Nlm_MemNew(sizeof(Nlm_MatrixStruct) +
               (n_row * n_column - 1) * sizeof(Nlm_FloatHi));
  ASSERT ( n_row  &&  n_column );

  mat->n_row    = n_row;
  mat->n_column = n_column;
  return mat;
}


NLM_EXTERN void Nlm_MatrixDelete
(Nlm_Matrix mat)
{
  Nlm_MemFree( mat );
}


NLM_EXTERN void Nlm_MatrixSetNode
(Nlm_Matrix  mat,
 Nlm_Uint4   row,
 Nlm_Uint4   column,
 Nlm_FloatHi value)
{
  VAL(mat, row, column) = value;
}


NLM_EXTERN void Nlm_MatrixSetRow
(Nlm_Matrix       mat,
 Nlm_Uint4        row,
 const Nlm_Matrix mat_src,
 Nlm_Uint4        row_src)
{
  Uint4 j;
  ASSERT ( mat->n_column == mat_src->n_column );

  for (j = 0;  j < mat->n_column;  j++)
    VAL(mat, row, j) = VAL(mat_src, row_src, j);
}


NLM_EXTERN void Nlm_MatrixSetColumn
(Nlm_Matrix       mat,
 Nlm_Uint4        column,
 const Nlm_Matrix mat_src,
 Nlm_Uint4        column_src)
{
  Uint4 i;
  ASSERT ( mat->n_row == mat_src->n_row );

  for (i = 0;  i < mat->n_row;  i++)
    VAL(mat, i, column) = VAL(mat_src, i, column_src);
}


NLM_EXTERN Nlm_FloatHi Nlm_MatrixNode
(const Nlm_Matrix mat,
 Nlm_Uint4        row,
 Nlm_Uint4        column)
{
  return VAL(mat, row, column);
}


NLM_EXTERN Nlm_Matrix Nlm_MatrixRow
(const Nlm_Matrix mat,
 Nlm_Uint4        row)
{
  Uint4 j;
  Nlm_Matrix row_mat = Nlm_MatrixNew(1, mat->n_column);

  for (j = 0;  j < mat->n_column;  j++)
    VAL(row_mat, 0, j) = VAL(mat, row, j);

  return row_mat;
}


NLM_EXTERN Nlm_Matrix Nlm_MatrixColumn
(const Nlm_Matrix mat,
 Nlm_Uint4        column)
{
  Uint4 i;
  Nlm_Matrix column_mat = Nlm_MatrixNew(mat->n_row, 1);

  for (i = 0;  i < mat->n_row;  i++)
    VAL(column_mat, i, 0) = VAL(mat, i, column);

  return column_mat;
}


NLM_EXTERN Nlm_Boolean Nlm_MatrixCompare
(const Nlm_Matrix mat1,
 const Nlm_Matrix mat2)
{
  ASSERT ( mat1->n_row    == mat1->n_row    );
  ASSERT ( mat1->n_column == mat1->n_column );
  return (Nlm_Boolean)
    (Nlm_MemCmp((VoidPtr)mat1, (VoidPtr)mat2, MAT_SIZE(mat1)) == 0);
}

 
NLM_EXTERN Nlm_Matrix Nlm_MatrixCopy
(const Nlm_Matrix mat)
{
  size_t mat_size = MAT_SIZE( mat );
  Nlm_Matrix c_mat = (Nlm_Matrix)Nlm_MemNew( mat_size );

  Nlm_MemCpy((VoidPtr)c_mat, (VoidPtr)mat, mat_size);
  return c_mat;
}


NLM_EXTERN Nlm_Matrix Nlm_MatrixTranspose
(const Nlm_Matrix mat)
{
  Uint4 i,j;
  Nlm_Matrix t_mat = Nlm_MatrixNew(mat->n_column, mat->n_row);

  for (i = 0;  i < mat->n_row;     i++)
  for (j = 0;  j < mat->n_column;  j++)
    VAL(t_mat, i, j) = VAL(mat, j, i);

  return t_mat;
}


NLM_EXTERN Nlm_Matrix Nlm_MatrixMultiply
(const Nlm_Matrix mat_left,
 const Nlm_Matrix mat_right)
{
  Uint4 n_row    = mat_left->n_row;
  Uint4 n_column = mat_right->n_column;
  Nlm_Matrix mat = Nlm_MatrixNew(n_row, n_column);

  ASSERT ( mat_left->n_column  == mat_right->n_row );

  {{
    Uint4 n_sum = mat_left->n_column;
    Uint4 i,j,k;
    for (i = 0;  i < n_row;    i++)
    for (j = 0;  j < n_column; j++)
    for (k = 0;  k < n_sum;    k++)
      VAL(mat, i, j) += VAL(mat_left, i, k) * VAL(mat_right, k, j);
  }}

  return mat;
}


NLM_EXTERN Nlm_Matrix Nlm_MatrixSolve
(const Nlm_Matrix mat,
 const Nlm_Matrix vector)
{
  Nlm_Matrix   d_mat = Nlm_MatrixCopy( mat );
  Nlm_Uint4Ptr indx = (Uint4Ptr)Nlm_MemNew(sizeof(Nlm_Uint4) * mat->n_row);
  
  ASSERT ( mat->n_row       == mat->n_column );
  ASSERT ( vector->n_row    == mat->n_column );
  ASSERT ( vector->n_column == 1 );

  VERIFY ( lu_decompose(d_mat, indx) );

  {{
    Nlm_Matrix res_vector = lu_back_subst(d_mat, indx, vector);
    Nlm_MemFree( indx );
    Nlm_MatrixDelete( d_mat );
    return res_vector;
  }}
}


NLM_EXTERN Nlm_Matrix Nlm_MatrixInvert
(const Nlm_Matrix mat)
{
  Nlm_Matrix res_matrix  = Nlm_MatrixNew(mat->n_row, mat->n_column);
  Nlm_Matrix unit_vector = Nlm_MatrixNew(mat->n_row, 1);

  Uint4 j;
  for (j = 0;  j < mat->n_column;  j++) {
    VAL(unit_vector, j, 0) = 1.0;
    {{
      Nlm_Matrix res_vector = Nlm_MatrixSolve(mat, unit_vector);
      Nlm_MatrixSetColumn(res_matrix, j, res_vector, 0);
      Nlm_MatrixDelete( res_vector );
    }}
    VAL(unit_vector, j, 0) = 0;
  }

  Nlm_MatrixDelete( unit_vector );
  return res_matrix;
}


NLM_EXTERN void Nlm_MatrixPrint
(const Nlm_Matrix mat,
 FILE*            fd,
 const Char*      descr)
{
  Uint4 i,j;

  if ( descr )
    fprintf(fd, "\n%s:\n", descr);

  for (i = 0;  i < mat->n_row;  i++) {
    for (j = 0;  j < mat->n_column;  j++)
      fprintf(fd, "%13g  ",  VAL(mat, i, j));
    fprintf(fd, "\n");
  }
}


/***********************************************************************
 *  TEST
 ***********************************************************************/

#ifdef TEST_MODULE_MATRIX

#include <ncbifile.h>

static void TestMatrix(Nlm_FloatHiPtr float_mat, Nlm_Uint4 dim, FILE *fd)
{
  Nlm_Matrix mat = Nlm_MatrixNew(dim, dim);
  Nlm_Matrix inv_mat, unit_mat, inv2_mat;

  Nlm_Uint4 i,j;
  for (i = 0;  i < dim;  i++)
  for (j = 0;  j < dim;  j++)
    Nlm_MatrixSetNode(mat, i, j, float_mat[i*dim + j]);
  Nlm_MatrixPrint(mat, fd, "Original Matrix:");

  inv_mat = Nlm_MatrixInvert( mat );
  Nlm_MatrixPrint(inv_mat, fd, "Inverted Matrix:");
  
  unit_mat = Nlm_MatrixMultiply(mat, inv_mat);
  Nlm_MatrixPrint(unit_mat, fd, "Must be [1]:");
  
  inv2_mat = Nlm_MatrixInvert( inv_mat );
  Nlm_MatrixPrint(inv2_mat, fd, "Inverted Twice Matrix:");

  Nlm_MatrixDelete( inv2_mat );
  Nlm_MatrixDelete( unit_mat );
  Nlm_MatrixDelete( inv_mat );
  Nlm_MatrixDelete( mat );
}



static Nlm_FloatHi mat1[1][1] = {
  { 1 }
};

static Nlm_FloatHi mat2[2][2] = {
  { 3, 2 },
  { 4, 1 },
};

static Nlm_FloatHi mat3[3][3] = {
  { 1, 2, 3 },
  { 4, 5, 6 },
  { 0, 8, 7 }
};

static Nlm_FloatHi mat4[4][4] = {
  { 0, 9, 4, 0 },
  { 1, 5, 6, 4 },
  { 9, 5, 2, 0 },
  { 0, 0, 7, 1 }
};

#define N_MAT 4
static Nlm_FloatHiPtr test_mat[N_MAT] = {
  (Nlm_FloatHiPtr)mat1,
  (Nlm_FloatHiPtr)mat2,
  (Nlm_FloatHiPtr)mat3,
  (Nlm_FloatHiPtr)mat4
};


Nlm_Int2 Nlm_Main( void )
{
  Nlm_Uint4 dim;
  FILE *fd = Nlm_FileOpen("matrix.log", "w");
  ASSERT ( fd );

  for (dim = 1;  dim <= N_MAT;  dim++)
    TestMatrix(test_mat[dim-1], dim, fd);

  Nlm_FileClose( fd );
  return 0;
}

#endif /* TEST_MODULE_MATRIX */
