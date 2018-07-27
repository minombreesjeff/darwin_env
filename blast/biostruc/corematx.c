/*   corematx.c
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
* File Name:  corematx.c
*
* Author: Hogue 
*
* Version Creation Date:   25 JULY 95
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
* 16 Aug   Hogue       Added a vector of pointers.
*
* $Log: corematx.c,v $
* Revision 6.0  1997/08/25 18:10:30  madden
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
/*****************************************************************************
*
*   corematx.c
*
*   vector and matrix allocation and free-ing.  
*   address a vector like a Fortran [1:n] or like C [0:n-1]
*
*   CoreLib -ized  matrix routines from NUMERICAL RECIPIES IN C
*   1st Edition,  Appendix D.  
*   with the 2nd Edition, the NR_END value is used to ANSI-ize this...
*   
*   Use this to replace the files "nrutil.h" and "nrutil.c" if you want 
*   NCBI-TOOLKIT compatible Numerical Recipies code.  
*   You still have to replace NR code calls to C function with 
*   TOOLKIT macros, however,  but these are not too critical for 
*   most compilers supported by toolkit. 
*
*   There is STILL no check for out-of bounds with these routines.
*   as with any C matrix.  The advantage of these routines is that
*   you can allocate 2-D matricies with -any size- and with 
*   - any indices,  e.g  I2Vector(1, 5) gives a 
*   pointer to an array starting with index 1 ending with index 5, 5 elements
*   mymatrix = FHMatrix(1, 10, 1, 3) gets a matrix of double 
*   you can address from
*    [1:10][1:3] with 30 elements.
*   to free,  call FHMatrixFree(mymatrix, 1, 1); args are the starting 
*   indicies of each dimension.
*   
*
*   a Matrix is allocated as a CONTIGUOUS BLOCK OF MEMORY!
*  Vector allocators and free-ers for:
*  xx =I2 Int2 
*     =I4 Int4
*     =FL FloatLo  -
*     =FH FloatHi
*     =UC Uchar
*     =UI4 Uint4
*
*   Matrix allocators and free-ers for:
*   xx = I2,  I4,  FL,  FH,  
*   FL3Matrix()
*   allocates a 3-way matrix with range with FloatLo data.
*   t[nrl:nrh][ncl:nch][ndl:ndh]
* 
*   The  following are untested:
*
* xxConvtMatrix()
* Allocates a matrix m[nrl..nrh][ncl..nch] that points to the standard C matrix 
* a[nrow][ncol],  where nrow=nrh-nrl+1 and ncol=nch-ncl+1.  The routine 
* should be called with the address &a[0][0] as the first argument
* There are I2,  I4,  FL and FH ConvtMatrix() routines.
*
*   FLSubmatrix() - returns a FL submatrix with range 
* [newrl..newrl+(oldrh-oldrl][newcl..newcl+(oldch-oldcl)] which
* points to the EXISTING matrix range [oldrl..oldrh][oldcl..oldch]
* free with FLSubmatrixFree()
* no other submatrix calls presently other than for FL
* 
*  Note at the bottom are "wrappers" declared for using unmodified
*  Numerical Recipies
*  code calls,  just define NRTONCBI and compile.
*
*  Standard NCBI disclaimer for US Government Software applies.
*  programmer C.W.V. Hogue
*  created: 14 Mar 95
*  last mod: 20 Mar 95
*  TESTED: 20 Mar 95 - all but xxConvtMatrix() FLSubmatrix() tested.
*
**********/


#include <ncbi.h>
#include <corematx.h>
#define NR_END 1




FloatLoPtr LIBCALL FLVector(Int4 nl, Int4 nh)
{
  FloatLoPtr v = NULL;
  v=(FloatLoPtr)MemNew((size_t)((nh-nl+1+NR_END)*sizeof(FloatLo)));
  if (!v) 
  {
    ErrPostEx(SEV_FATAL, 0, 0, "Out of Memory in FLVector");
    ErrShow();
    return NULL;
  }
   return (FloatLoPtr)(v-nl+NR_END); 
}


Int2Ptr LIBCALL I2Vector(Int4 nl, Int4 nh)
{
  Int2Ptr v = NULL;
  v=(Int2Ptr)MemNew((size_t)((nh-nl+1+NR_END)*sizeof(Int2)));
  if (!v) 
  {
    ErrPostEx(SEV_FATAL, 0, 0, "Out of Memory in I2Vector");
    ErrShow();
    return NULL;
  }
   return (Int2Ptr)(v-nl+NR_END);  
}


Int4Ptr LIBCALL I4Vector(Int4 nl, Int4 nh)
{
  Int4Ptr v = NULL;
  v=(Int4Ptr)MemNew((size_t)((nh-nl+1+NR_END)*sizeof(Int4)));
  if (!v) 
  {
    ErrPostEx(SEV_FATAL, 0, 0, "Out of Memory in I4Vector");
    ErrShow();
    return NULL;
  }
   return (Int4Ptr)(v-nl+NR_END);  
}

FloatHiPtr LIBCALL FHVector(Int4 nl, Int4 nh)
{
  FloatHiPtr v = NULL;
  v=(FloatHiPtr)MemNew((size_t)((nh-nl+1+NR_END)*sizeof(FloatHi)));
  if (!v) 
  {
    ErrPostEx(SEV_FATAL, 0, 0, "Out of Memory in FHVector");
    ErrShow();
    return NULL;
  }
   return (FloatHiPtr)(v-nl+NR_END);
}

UcharPtr LIBCALL UCVector(Int4 nl, Int4 nh)
{
  UcharPtr v = NULL;
  v=(UcharPtr)MemNew((size_t)((nh-nl+1+NR_END)*sizeof(Uchar)));
  if (!v) 
  {
    ErrPostEx(SEV_FATAL, 0, 0, "Out of Memory in UCVector");
    ErrShow();
    return NULL;
  }
   return (UcharPtr)(v-nl+NR_END);
}


Uint4Ptr LIBCALL UI4Vector(Int4 nl, Int4 nh)
{
  Uint4Ptr v = NULL;
  v=(Uint4Ptr)MemNew((size_t)((nh-nl+1+NR_END)*sizeof(Uint4)));
  if (!v) 
  {
    ErrPostEx(SEV_FATAL, 0, 0, "Out of Memory in UI4Vector");
    ErrShow();
    return NULL;
  }
   return (Uint4Ptr)(v-nl+NR_END);
}


PointerPtr LIBCALL PTRVector(Int4 nl, Int4 nh)
{
  PointerPtr v = NULL;
  v=(PointerPtr)MemNew((size_t)((nh-nl+1+NR_END)*sizeof(Pointer)));
  if (!v) 
  {
    ErrPostEx(SEV_FATAL, 0, 0, "Out of Memory in PTRVector");
    ErrShow();
    return NULL;
  }
   return (PointerPtr)(v-nl+NR_END);
}




FloatLoPtrPtr LIBCALL FLMatrix(Int4 nrl, Int4 nrh,  Int4 ncl,  Int4 nch)
{
    Int4 i, nrow=nrh-nrl+1, ncol=nch-ncl+1;
    FloatLoPtr *m = NULL;
    
    m=(FloatLoPtr *)MemNew((size_t)(nrow+NR_END)*sizeof(FloatLoPtr));
    if (!m) 
    {
	ErrPostEx(SEV_FATAL, 0, 0, "Out of Memory 1 in FLMatrix");
	ErrShow();
	return NULL;
    }
    m += NR_END;
    m -= nrl;
    m[nrl] = (FloatLoPtr)MemNew((size_t)((nrow*ncol+NR_END)*sizeof(FloatLo)));
    if (!m[nrl]) 
    {
	ErrPostEx(SEV_FATAL, 0, 0, "Out of Memory 2 in FLMatrix");
	ErrShow();
	return NULL;
    }
    m[nrl] += NR_END;
    m[nrl] -= ncl;
    for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;
    return (FloatLoPtrPtr) m;
}



FloatHiPtrPtr LIBCALL FHMatrix(Int4 nrl, Int4 nrh,  Int4 ncl,  Int4 nch)
{
    Int4 i, nrow=nrh-nrl+1, ncol=nch-ncl+1;
    FloatHiPtr *m = NULL;
    
    m=(FloatHiPtr *)MemNew((size_t)(nrow+NR_END)*sizeof(FloatHiPtr));
    if (!m) 
    {
	ErrPostEx(SEV_FATAL, 0, 0, "Out of Memory 1 in FHMatrix");
	ErrShow();
	return NULL;
    }
    m += NR_END;
    m -= nrl;
    m[nrl] = (FloatHiPtr)MemNew((size_t)((nrow*ncol+NR_END)*sizeof(FloatHi)));
    if (!m[nrl]) 
    {
	ErrPostEx(SEV_FATAL, 0, 0, "Out of Memory 2 in FHMatrix");
	ErrShow();
	return NULL;
    }
    m[nrl] += NR_END;
    m[nrl] -= ncl;
    for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;
    return (FloatHiPtrPtr) m;    
}



Int2PtrPtr LIBCALL I2Matrix(Int4 nrl, Int4 nrh,  Int4 ncl,  Int4 nch)
{
    Int2 i, nrow=nrh-nrl+1, ncol=nch-ncl+1;
    Int2Ptr *m = NULL;
    
    m=(Int2Ptr *)MemNew((size_t)(nrow+NR_END)*sizeof(Int2Ptr));
    if (!m) 
    {
	ErrPostEx(SEV_FATAL, 0, 0, "Out of Memory 1 in I2Matrix");
	ErrShow();
	return NULL;
    }
    m += NR_END;
    m -= nrl;
    m[nrl] = (Int2Ptr)MemNew((size_t)((nrow*ncol+NR_END)*sizeof(Int2)));
    if (!m[nrl]) 
    {
	ErrPostEx(SEV_FATAL, 0, 0, "Out of Memory 2 in I2Matrix");
	ErrShow();
	return NULL;
    }
    m[nrl] += NR_END;
    m[nrl] -= ncl;
    for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;
    return (Int2PtrPtr) m;
}


Int4PtrPtr LIBCALL I4Matrix(Int4 nrl, Int4 nrh,  Int4 ncl,  Int4 nch)
{
    Int4 i, nrow=nrh-nrl+1, ncol=nch-ncl+1;
    Int4Ptr *m = NULL;
    
    m=(Int4Ptr *)MemNew((size_t)(nrow+NR_END)*sizeof(Int4Ptr));
    if (!m) 
    {
	ErrPostEx(SEV_FATAL, 0, 0, "Out of Memory 1 in I4Matrix");
	ErrShow();
	return NULL;
    }
    m += NR_END;
    m -= nrl;
    m[nrl] = (Int4Ptr)MemNew((size_t)((nrow*ncol+NR_END)*sizeof(Int4)));
    if (!m[nrl]) 
    {
	ErrPostEx(SEV_FATAL, 0, 0, "Out of Memory 2 in I4Matrix");
	ErrShow();
	return NULL;
    }
    m[nrl] += NR_END;
    m[nrl] -= ncl;
    for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;
    return (Int4PtrPtr) m;
}

FloatLoPtrPtr LIBCALL FLSubmatrix(FloatLoPtr *a,  Int4 oldrl,  Int4 oldrh, 
		Int4 oldcl,  Int4 oldch,  Int4 newrl,  Int4 newcl)
{  
    Int4 i,  j,  nrow=oldrh-oldrl+1, ncol=oldcl-newcl;
    FloatLoPtr *m = NULL;
    m=(FloatLoPtr *)MemNew((size_t)((nrow+NR_END)*sizeof(FloatLoPtr)));
    if (!m) 
    {
	ErrPostEx(SEV_FATAL, 0, 0, "Out of Memory in FLSubmatrix");
	ErrShow();
	return NULL;
    }
    m += NR_END;
    m -= newrl;
    for(i=oldrl,  j=newrl; i<=oldrh; i++, j++) m[j]=a[i]+ncol;
    return (FloatLoPtrPtr) m;
}


void LIBCALL FLSubmatrixFree(FloatLoPtr *b,  Int4 nrl)
{
    MemFree((CharPtr)(b+nrl-NR_END));
}

void LIBCALL FHVectorFree(FloatHiPtr v, Int4 nl)
{
    MemFree((CharPtr) (v+nl-NR_END));
}

void LIBCALL FLVectorFree(FloatLoPtr v,  Int4 nl)
{
    MemFree((CharPtr)(v+nl-NR_END));
}

void LIBCALL I4VectorFree(Int4Ptr v,  Int4 nl)
{
    MemFree((CharPtr)(v+nl-NR_END));
}

void LIBCALL I2VectorFree(Int2Ptr v,  Int4 nl)
{
    MemFree((Int2Ptr)(v+nl-NR_END));
}

void LIBCALL UCVectorFree(UcharPtr v,  Int4 nl)
{
    MemFree((CharPtr)(v+nl-NR_END));
}

void LIBCALL UI4VectorFree(Uint4Ptr v,  Int4 nl)
{
    MemFree((CharPtr)(v+nl-NR_END));
}

void LIBCALL PTRVectorFree(PointerPtr v,  Int4 nl)
{
    MemFree((CharPtr)(v+nl-NR_END));
}

void LIBCALL I2MatrixFree(Int2Ptr *m,  Int4 nrl, Int4 ncl)
{
 MemFree((CharPtr)(m[nrl]+ncl-NR_END));
 MemFree((CharPtr)(m+nrl-NR_END));   
}


void LIBCALL I4MatrixFree(Int4Ptr *m,  Int4 nrl, Int4 ncl)
{
 MemFree((CharPtr)(m[nrl]+ncl-NR_END));
 MemFree((CharPtr)(m+nrl-NR_END));   
}



void LIBCALL FLMatrixFree(FloatLoPtr *m,  Int4 nrl, Int4 ncl)
{
 MemFree((CharPtr)(m[nrl]+ncl-NR_END));
 MemFree((CharPtr)(m+nrl-NR_END));   
}


void LIBCALL FHMatrixFree(FloatHiPtr *m,  Int4 nrl, Int4 ncl)
{
 MemFree((CharPtr)(m[nrl]+ncl-NR_END));
 MemFree((CharPtr)(m+nrl-NR_END));
}

  
FloatHiPtrPtr LIBCALL FHConvtMatrix(FloatHiPtr a, Int4 nrl,  Int4 nrh,  Int4 ncl,  
			    Int4 nch)
{
    Int4 i,  j,  nrow=nrh-nrl+1,  ncol=nch-ncl+1;
    FloatHiPtr *m = NULL;
    m=(FloatHiPtr *)MemNew((size_t)((nrow+NR_END)*sizeof(FloatHiPtr)));
    if (!m)
    {
	ErrPostEx(SEV_FATAL, 0, 0, "Out of memory in FHConvtMatrix");
	ErrShow();
	return NULL;
    }
    m += NR_END;
    m -= nrl;
    m[nrl]=a-ncl;
    for(i=1, j=nrl+1;i<nrow;i++,j++) 
	m[j] = m[j-1]+ncol;
    return (FloatHiPtrPtr) m;
}


FloatLoPtrPtr LIBCALL FLConvtMatrix(FloatLoPtr a, Int4 nrl,  Int4 nrh,  Int4 ncl,  
			    Int4 nch)
{
    Int4 i,  j,  nrow=nrh-nrl+1,  ncol=nch-ncl+1;
    FloatLoPtr *m = NULL;
    m=(FloatLoPtr *)MemNew((size_t)((nrow+NR_END)*sizeof(FloatLoPtr)));
    if (!m)
    {
	ErrPostEx(SEV_FATAL, 0, 0, "Out of memory in FLConvtMatrix");
	ErrShow();
	return NULL;
    }
    m += NR_END;
    m -= nrl;
    m[nrl]=a-ncl;
    for(i=1, j=nrl+1;i<nrow;i++,j++) 
	m[j] = m[j-1]+ncol;
    return (FloatLoPtrPtr) m;
}
		
Int2PtrPtr LIBCALL I2ConvtMatrix(Int2Ptr a, Int4 nrl,  Int4 nrh,  Int4 ncl,  
			    Int4 nch)
{
    Int4 i,  j,  nrow=nrh-nrl+1,  ncol=nch-ncl+1;
    Int2Ptr *m = NULL;
    m=(Int2Ptr *)MemNew((size_t)((nrow+NR_END)*sizeof(Int2Ptr)));
    if (!m)
    {
	ErrPostEx(SEV_FATAL, 0, 0, "Out of memory in I2ConvtMatrix");
	ErrShow();
	return NULL;
    }
    m += NR_END;
    m -= nrl;
    m[nrl]=a-ncl;
    for(i=1, j=nrl+1;i<nrow;i++,j++) 
	m[j] = m[j-1]+ncol;
    return (Int2PtrPtr) m;
}
	    
		
Int4PtrPtr LIBCALL I4ConvtMatrix(Int4Ptr a, Int4 nrl,  Int4 nrh,  Int4 ncl,  
			    Int4 nch)
{
    Int4 i,  j,  nrow=nrh-nrl+1,  ncol=nch-ncl+1;
    Int4Ptr *m = NULL;
    m=(Int4Ptr *)MemNew((size_t)((nrow+NR_END)*sizeof(Int4Ptr)));
    if (!m)
    {
	ErrPostEx(SEV_FATAL, 0, 0, "Out of memory in I4ConvtMatrix");
	ErrShow();
	return NULL;
    }
    m += NR_END;
    m -= nrl;
    m[nrl]=a-ncl;
    for(i=1, j=nrl+1;i<nrow;i++,j++) 
	m[j] = m[j-1]+ncol;
    return (Int4PtrPtr) m;
}
	 
void LIBCALL I2ConvtFree(Int2Ptr *b, Int4 nrl)
{
    /* frees a matrix allocated by  xxConvtMatrix */
   MemFree((CharPtr)(b+nrl-NR_END));
}
	    
	 
void LIBCALL I4ConvtFree(Int4Ptr *b, Int4 nrl)
{
    /* frees a matrix allocated by  xxConvtMatrix */
   MemFree((CharPtr)(b+nrl-NR_END));
}
	    
	    
	 
void LIBCALL FLConvtFree(FloatLoPtr *b, Int4 nrl)
{
    /* frees a matrix allocated by  xxConvtMatrix */
   MemFree((CharPtr)(b+nrl-NR_END));
}
	    
	    
	 
void LIBCALL FHConvtFree(FloatHiPtr *b, Int4 nrl)
{
    /* frees a matrix allocated by  xxConvtMatrix */
   MemFree((CharPtr)(b+nrl-NR_END));
}


FloatLoPtrPtrPtr LIBCALL FL3Matrix(Int4 nrl,  Int4 nrh,  Int4 ncl,  Int4 nch,  Int4 ndl, 
Int4 ndh)
{
    Int4 i, j, nrow=nrh-nrl+1, ncol=nch-ncl+1, ndep=ndh-ndl+1;
    FloatLoPtr **t = NULL;
    
    t=(FloatLoPtr **)MemNew((size_t)((nrow+NR_END)*sizeof(FloatLoPtr *)));
    if (!t)
    {
	ErrPostEx(SEV_FATAL, 0, 0, "Out of Memory 1 in FL3Matrix");
	ErrShow();
	return NULL;
    }
    t += NR_END;
    t -= nrl;
    
    t[nrl]=(FloatLoPtr *)MemNew((size_t)((nrow*ncol+NR_END)*sizeof(FloatLoPtr)));
    if (!t[nrl])
    {
	ErrPostEx(SEV_FATAL, 0, 0, "Out of Memory 3 in FL3Matrix");
	ErrShow();
	return NULL;
    } 
    t[nrl] += NR_END;
    t[nrl] -= ncl;
    
    t[nrl][ncl]=(FloatLoPtr)MemNew((size_t)((nrow*ncol*ndep+NR_END)*sizeof(FloatLo)));
    if (!t[nrl][ncl])
    {
	ErrPostEx(SEV_FATAL, 0, 0, "Out of Memory 3 in FL3Matrix");
	ErrShow();
	return NULL;
    } 
    t[nrl][ncl] += NR_END;
    t[nrl][ncl] -= ncl;
    
    for(j=ncl+1;j<=nch;j++) t[nrl][j] = t[nrl][j-1]+ndep;
    for (i=nrl+1;i<=nrh;i++)
    {
	t[i]=t[i-1]+ncol;
	t[i][ncl]=t[i-1][ncl]+ncol*ndep;
	for(j=ncl+1;j<=nch;j++) t[i][j]=t[i][j-1]+ndep;
    }
    return (FloatLoPtrPtrPtr) t;
}

void LIBCALL FL3MatrixFree(FloatLoPtr **b, Int4 nrl, Int4 ncl, Int4 ndl)
{
    MemFree((CharPtr)(b[nrl][ncl]+ndl-NR_END));
    MemFree((CharPtr)(b[nrl]+ncl-NR_END));
    MemFree((CharPtr)(b+nrl-NR_END));
}  



/* WRAPPERS FOR NUMERICAL RECIPIES IN C 2nd Ed'n CODE */
/* define NRTONCBI and include this instead of nrutil.h nrutil.c */
/*  These are not tested but are here anyhow if they are ever needed... */
#ifdef NRTONCBI
void  nrerror(char error_text[])
{
    Char buf[200];
    StrCpy(buf, "Numerical Recipies run-time error...\n");
    StrCat(buf,  error_text);
    StrCat(buf,  "Fatal, must exit to system\n");
    ErrPostEx(SEV_FATAL, 0, 0, buf);
    ErrShow();
    exit(1);
}

FloatLoPtr vector(Int4 nl, Int4 nh)
{ 
    return FLVector(nl, nh);
}


Int4Ptr ivector(Int4 nl, Int4 nh)
{
    return I4Vector(nl, nh);
}  /* Note this removes the "int" platform dependence of the NR call */

FloatHiPtr dvector(Int4 nl, Int4 nh)
{
    return FHVector(nl, nh);
}

UcharPtr cvector(Int4 nl,  Int4 nh)
{
    return UCVector(nl, nh);
}

Uint4Ptr lvector(Int4 nl,  Int4 nh)
{
    return UI4Vector(nl,  nh);
}

FloatLoPtr *matrix(Int4 nrl, Int4 nrh,  Int4 ncl,  Int4 nch)
{
    return FLMatrix(nrl,  nrh,  ncl,  nch);
    
}
FloatHiPtr *dmatrix(Int4 nrl, Int4 nrh,  Int4 ncl,  Int4 nch)
{
    return FHMatrix(nrl,  nrh,  ncl,  nch);
}

Int4Ptr *imatrix(Int4 nrl, Int4 nrh,  Int4 ncl,  Int4 nch)
{
    return I4Matrix(nrl,  nrh,  ncl,  nch);
}

void free_dvector(FloaHiPtr v, Int4 nl,  Int4 nh)
{
    FHVectorFree(v, nl);
}

void free_vector(FloatLoPtr v,  Int4 nl,  Int4 nh)
{
    FLVectorFree(v, nl);
}

void free_cvector(UcharPtr v,  Int4 nl,  Int4 nh)
{
    UCVectorFree(v, nl);
}

void free_lvector(Uint4Ptr v,  Int4 nl,  Int4 nh)
{
    UI4VectorFree(v, nl);
}

void free_ivector(Int4Ptr v,  Int4 nl,  Int4 nh)
{
    I4VectorFree(v,  nl);
}

void free_imatrix(Int4Ptr *m,  Int4 nrl, Int4 nrh,  Int4 ncl,  Int4 nch)
{
    I4MatrixFree(m,  nrl,  ncl);
}

void free_matrix(FloatLoPtr *m,  Int4 nrl, Int4 nrh,  Int4 ncl,  Int4 nch)
{
    FLMatrixFree(m, nrl,  ncl);
}

void free_dmatrix(FloatHiPtr *m,  Int4 nrl, Int4 nrh,  Int4 ncl,  Int4 nch)
{
    FHMatrixFree(m,  nrl,  ncl);
}

FloatLoPtr convert_matrix(FloatLoPtr a, Int4 nrl,  Int4 nrh,  Int4 ncl,  
			    Int2 nch)
{
    return FLConvtMatrix(a,  nrl,  nrh,  ncl,  nch);
}
			 
void free_convert_matrix(FloatLoPtr *b,  Int4 nrl,  Int4 nrh,  Int4 ncl,  Int4 nch)
{
    FLConvtFree(b,  nrl)
}			   

FloatLoPtr *submatrix(FloatLoPtr *a,  Int4 oldrl,  Int4 oldrh, 
		Int4 oldcl,  Int4 oldch,  Int4 newrl,  Int4 newcl)
{
    return FLSubmatrix(a,  oldrl,  oldrh, oldcl,  oldch,  newrl,  newcl);
}

void free_submatrix(FloatLoPtr *b,  Int4 nrl,  Int4 nrh, 
		Int4 ncl,  Int4 nch)
{
    FLSubmatrixFree(b,  nrl);
}

FloatLoPtr **f3tensor(Int4 nrl,  Int4 nrh,  Int4 ncl,  Int4 nch,  Int4 ndl,  Int4 ndh)
{
    return FL3Matrix(nrl,  nrh,  ncl,  nch,  ndl, ndh);
}

void free_f3tensor(FloatLoPtr **t,  Int4 nrl,  Int4 nrh,  Int4 ncl,  Int4 nch, 
		    Int4 ndl,  Int4 ndh)
{
    FL3MatrixFree(t, nrl,  ncl, ndl);
}

#endif


/* test code follows */
/*
#define NUMARGS 1
Args myargs[NUMARGS] = {
     {"Dummy Arg","stdout", NULL,NULL, TRUE,'f',ARG_FILE_OUT,0.0,0,NULL}
      };

	
Int2 Main(void)
{
    Int2Ptr iv;
    Int2Ptr *im;
    Int4Ptr i4v;
    Int4Ptr *i4m;
    FloatLoPtr flv;
    FloatLoPtr *flm;
    FloatLoPtr *flmsub;
    FloatLoPtr **fltd;
    FloatHiPtr fhv;
    FloatHiPtr *fhm;
    UcharPtr ucv;
    Uint4Ptr uiv;
    Int4 i,  j,  k;
    FloatLo I, J, K;
    CharPtr mys;

   	
    if (! GetArgs("CoreMatx 1.0",NUMARGS, myargs))
	 	return 1;
    
    printf("starting\n");
    printf("Allocating integers\n");
    iv = I2Vector(1, 100);

    
    im = I2Matrix(1, 100,  1,  100);
    i4v = I4Vector(1, 100);
    i4m = I4Matrix(1, 100, 1, 100);
    printf("Allocating reals \n");
    flv = FLVector(1, 100);
    flm = FLMatrix(1, 100, 1, 100);
    fhv = FHVector(1, 100);
    fhm = FHMatrix(1, 100, 1, 100);
    printf("Allocating others\n");
    ucv = UCVector(1, 100);
    uiv = UI4Vector(1, 100);
    printf("Allocating 3D float matrix\n");
    fltd = FL3Matrix(1, 100, 1, 10, 1, 10);
    printf("Assigning\n");
    for (i = 1; i<= 100; i++)
    {
	I = (FloatHi) i;
	iv[i] = (Int2)i;
	i4v[i] = i * (-1000);
	flv[i] = (FloatLo)((I+1)/I);
	fhv[i] = (FloatHi) ((I+1/I) + I*10000000000000);
	ucv[i] = (Uchar)(i+64);
	uiv[i] = (Uint4)(i * 100000);
	for (j = 1;  j<=100; j++)
	{
	    J = (FloatLo) j;
	    im[i][j] = (Int2)i+j;
	    i4m[i][j] = (Int4) i+j * 100000;
	    flm[i][j] = (FloatLo)((J+1)/(I+J));
	    fhm[i][j] = (FloatHi)(I/(I+J) + I*J*1000000000000);
	}
    }
    for (i =1;i<=100;i++)
    for (j=1;j<=10;j++)
    for (k=1;k<=10;k++)
      {
       I = (FloatLo) i;
       J = (FloatLo) j;
       K = (FloatLo) k;
       fltd[i][j][k] = I+J+K + 1/(I*J*K);
      }
    printf("Vectors  \n");
    printf("Int2\n");
    for (i = 1;i<=91;i+=10)
    {
      for (j = 0; j<=9; j++)
        {
         printf("%d ", iv[i+j]);
	}
	printf("\n");  
    } 
    printf("Int4\n");
    for (i = 1;i<=91;i+=10)
    {
      for (j = 0; j<=9; j++)
        {
         printf("%ld ", i4v[i+j]);
	}
	printf("\n");  
    }
    printf("FloatLo\n");
    for (i = 1;i<=91;i+=10)
    {
      for (j = 0; j<=9; j++)
        {
         printf("%f ", flv[i+j]);
	}
	printf("\n");  
    }
    printf("FloatHi\n");
    for (i = 1;i<=91;i+=10)
    {
      for (j = 0; j<=9; j++)
        {
         printf("%ef ", fhv[i+j]);
	}
	printf("\n");  
    }
    printf("UChar\n");
    for (i = 1;i<=91;i+=10)
    {
      for (j = 0; j<=9; j++)
        {
         printf("%d ", ucv[i+j]);
	}
	printf("\n");  
    }
    printf("UInt4\n");
    for (i = 1;i<=91;i+=10)
    {
      for (j = 0; j<=9; j++)
        {
         printf("%ld ", uiv[i+j]);
	}
	printf("\n");  
    }
    printf("Matricies\n");
    printf("FloatHi\n");
    for (k=1;k<=100;k++)
    {
       printf("Row [%d]\n", k);
	for (i = 1;i<=91;i+=10)
	{
	    for (j = 0; j<=9; j++)
	    {
		printf("%ef ", fhm[k][i+j]);
	    }
	printf("\n");
	}  
    }
    printf("FloatLo\n");
    for (k=1;k<=100;k++)
    {
       printf("Row [%d]\n", k);
	for (i = 1;i<=91;i+=10)
	{
	    for (j = 0; j<=9; j++)
	    {
		printf("%f ", flm[k][i+j]);
	    }
	printf("\n");
	}  
    }
    printf("Int2\n");
    for (k=1;k<=100;k++)
    {
       printf("Row [%d]\n", k);
	for (i = 1;i<=91;i+=10)
	{
	    for (j = 0; j<=9; j++)
	    {
		printf("%d ", im[k][i+j]);
	    }
	printf("\n");
	}  
    }
    printf("Int4\n");
    for (k=1;k<=100;k++)
    {
       printf("Row [%d]\n", k);
	for (i = 1;i<=91;i+=10)
	{
	    for (j = 0; j<=9; j++)
	    {
		printf("%ld ", i4m[k][i+j]);
	    }
	printf("\n");
	}  
    }
    printf("3DFloat");
    for (k=1;k<=100;k++)
    {
       printf("Row [%d]\n", k);
	for (i = 1;i<=10;i++)
	{
	    printf("Col [%d]\n", i);
	    for (j = 1; j<=10; j++)
	    {
		printf("%ef ", fltd[k][i][j]);
	    }
	printf("\n");
	}  
    }
    printf("freeing floats\n");
    FL3MatrixFree(fltd, 1,  1, 1);
    FLMatrixFree(flm, 1,  1);
    FLVectorFree(flv,  1);
    FHMatrixFree(fhm,  1, 1);
    FHVectorFree(fhv, 1);
    printf("freeing ints\n");
    I4MatrixFree(i4m, 1, 1);
    I4VectorFree(i4v, 1);
    I2MatrixFree(im, 1, 1);
    I2VectorFree(iv, 1);
    UCVectorFree(ucv, 1);
    UI4VectorFree(uiv,  1);
    printf("done\n");
    mys = (CharPtr) MemNew((size_t)(100*sizeof(Char)));
    strcpy(mys, "adios amiogs, MemNew still functions..\n");
    printf(mys);
    MemFree(mys);
    printf("and MemFree is OK\n");
    return 0;
}	    
	    
*/
