/*   corematx.h
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
* File Name:  corematx.h
*
* Author:  Hogue
*
* Version Creation Date:   3/20/95
*
* $Revision: 6.0 $
*
* File Description: 
*   	Numerical Recipies in C toolkit-ized
*	matrix and vector allocators and free-ers
*	see corematx.c for implementation docs.
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 16 Aug   Hogue       Added PointerPtr typedef * vector allocators
* 
* $Log: corematx.h,v $
* Revision 6.0  1997/08/25 18:10:33  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 14:02:09  ostell
* Set to revision 5.0
*
 * Revision 1.3  1995/08/28  19:31:53  kans
 * version 4.1
 *
 * Revision 1.2  1995/08/03  21:44:33  kans
 * changes
 *
 * Revision 1.1  1995/08/02  17:07:15  kans
 * Initial revision
 *
*
* ==========================================================================
*/


typedef FloatHiPtr *FloatHiPtrPtr;
typedef FloatLoPtr *FloatLoPtrPtr;
typedef Int2Ptr *Int2PtrPtr;
typedef Int4Ptr *Int4PtrPtr;
typedef FloatLoPtr **FloatLoPtrPtrPtr;
typedef Pointer *PointerPtr;

/* To use NUMERICAL RECIPIES IN C wrapper functions */
/* set the compiler define NRTONCBI */
#ifndef _COREMATX_
#define _COREMATX_

#ifdef __cplusplus
extern "C" {
#endif

/* function prototypes here */
FloatHiPtr LIBCALL FHVector  (Int4 nl, Int4 nh);
FloatLoPtr LIBCALL FLVector (Int4 nl, Int4 nh);
Int2Ptr  LIBCALL I2Vector  (Int4 nl, Int4 nh);
Int4Ptr  LIBCALL I4Vector (Int4 nl, Int4 nh);
UcharPtr LIBCALL UCVector (Int4 nl, Int4 nh);
Uint4Ptr LIBCALL UI4Vector (Int4 nl, Int4 nh);
PointerPtr LIBCALL PTRVector(Int4 nl, Int4 nh);
void LIBCALL FHVectorFree (FloatHiPtr v, Int4 nl);
void LIBCALL FLVectorFree (FloatLoPtr v,  Int4 nl);
void LIBCALL I2VectorFree (Int2Ptr v,  Int4 nl);
void LIBCALL I4VectorFree (Int4Ptr v,  Int4 nl);
void LIBCALL UCVectorFree (UcharPtr v,  Int4 nl);
void LIBCALL UI4VectorFree (Uint4Ptr v,  Int4 nl);
void LIBCALL PTRVectorFree(PointerPtr v,  Int4 nl);




FloatHiPtrPtr LIBCALL FHMatrix (Int4 nrl, Int4 nrh,  Int4 ncl,  Int4 nch);
FloatLoPtrPtr LIBCALL FLMatrix  (Int4 nrl, Int4 nrh,  Int4 ncl,  Int4 nch);
Int2PtrPtr LIBCALL I2Matrix (Int4 nrl, Int4 nrh,  Int4 ncl,  Int4 nch);
Int4PtrPtr LIBCALL I4Matrix (Int4 nrl, Int4 nrh,  Int4 ncl,  Int4 nch);
void LIBCALL FLMatrixFree (FloatLoPtr *m,  Int4 nrl, Int4 ncl);
void LIBCALL FHMatrixFree (FloatHiPtr *m,  Int4 nrl, Int4 ncl);
void LIBCALL I2MatrixFree (Int2Ptr *m,  Int4 nrl, Int4 ncl);
void LIBCALL I4MatrixFree (Int4Ptr *m,  Int4 nrl, Int4 ncl);

FloatLoPtrPtr LIBCALL FLSubmatrix (FloatLoPtr *a,  Int4 oldrl,  Int4 oldrh, 
		Int4 oldcl,  Int4 oldch,  Int4 newrl,  Int4 newcl);
void LIBCALL FLSubmatrixFree (FloatLoPtr *b,  Int4 nrl);

FloatHiPtrPtr  LIBCALL FHConvtMatrix (FloatHiPtr a, Int4 nrl,  Int4 nrh,  Int4 ncl,  
			    Int4 nch);
FloatLoPtrPtr  LIBCALL FLConvtMatrix (FloatLoPtr a, Int4 nrl,  Int4 nrh,  Int4 ncl,  
			    Int4 nch);
Int2PtrPtr  LIBCALL I2ConvtMatrix (Int2Ptr a, Int4 nrl,  Int4 nrh,  Int4 ncl,  
			    Int4 nch);
Int4PtrPtr  LIBCALL I4ConvtMatrix (Int4Ptr a, Int4 nrl,  Int4 nrh,  Int4 ncl,  
			    Int4 nch);
void LIBCALL FHConvtFree (FloatHiPtr *b, Int4 nrl);
void LIBCALL FLConvtFree (FloatLoPtr *b, Int4 nrl);
void LIBCALL I2ConvtFree (Int2Ptr *b, Int4 nrl);
void LIBCALL I4ConvtFree (Int4Ptr *b, Int4 nrl);

FloatLoPtrPtrPtr LIBCALL FL3Matrix (Int4 nrl,  Int4 nrh,  Int4 ncl,  Int4 nch,  Int4 ndl, 
Int4 ndh);
void LIBCALL FL3MatrixFree (FloatLoPtr **b, Int4 nrl, Int4 ncl, Int4 ndl);

/* Numerical Recipies in C "Wrappers" */
#ifdef NRTONCBI
void nrerror (char error_text[]);
FloatLoPtr vector (Int4 nl, Int4 nh);
Int4Ptr ivector (Int4 nl, Int4 nh);
FloatHiPtr dvector (Int4 nl, Int4 nh);
UcharPtr cvector (Int4 nl,  Int4 nh);
Uint4Ptr lvector (Int4 nl,  Int4 nh);
FloatLoPtr *matrix (Int4 nrl, Int4 nrh,  Int4 ncl,  Int4 nch);
FloatHiPtr *dmatrix (Int4 nrl, Int4 nrh,  Int4 ncl,  Int4 nch);
Int4Ptr *imatrix (Int4 nrl, Int4 nrh,  Int4 ncl,  Int4 nch);
void free_dvector (FloaHiPtr v, Int4 nl,  Int4 nh);
void free_vector (FloatLoPtr v,  Int4 nl,  Int4 nh);
void free_cvector (UcharPtr v,  Int4 nl,  Int4 nh);
void free_lvector (Uint4Ptr v,  Int4 nl,  Int4 nh);
void free_ivector (Int4Ptr v,  Int4 nl,  Int4 nh);
void free_imatrix (Int4Ptr *m,  Int4 nrl, Int4 nrh,  Int4 ncl,  Int4 nch);
void free_matrix (FloatLoPtr *m,  Int4 nrl, Int4 nrh,  Int4 ncl,  Int4 nch);
void free_dmatrix (FloatHiPtr *m,  Int4 nrl, Int4 nrh,  Int4 ncl,  Int4 nch);
FloatLoPtr convert_matrix (FloatLoPtr a, Int4 nrl,  Int4 nrh,  Int4 ncl,  
			    Int2 nch);
void free_convert_matrix (FloatLoPtr *b,  Int4 nrl,  Int4 nrh,  Int4 ncl,
                          Int4 nch);
FloatLoPtr *submatrix (FloatLoPtr *a,  Int4 oldrl,  Int4 oldrh, 
		Int4 oldcl,  Int4 oldch,  Int4 newrl,  Int4 newcl);
void free_submatrix (FloatLoPtr *b,  Int4 nrl,  Int4 nrh, 
		Int4 ncl,  Int4 nch);
FloatLoPtr **f3tensor (Int4 nrl,  Int4 nrh,  Int4 ncl,  Int4 nch,  Int4 ndl,  
                       Int4 ndh);
void free_f3tensor (FloatLoPtr **t,  Int4 nrl,  Int4 nrh,  Int4 ncl,  Int4 nch, 
		    Int4 ndl,  Int4 ndh);

#endif

#ifdef __cplusplus
}
#endif


#endif

