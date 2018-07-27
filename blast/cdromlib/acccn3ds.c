/*   acccn3ds.c
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
 * File Name:  acccn3ds.c
 *
 * Author:  Jonathan Epstein
 *
 * Version Creation Date:   6/20/96
 *
 * File Description: 
 *       Stub file for CN3D functions (real functions in cn3d*.c)
 *
 * Modifications:  
 * --------------------------------------------------------------------------
 * $Log: acccn3ds.c,v $
 * Revision 6.2  1999/01/15 19:52:49  kans
 * needed to add vibrant typedefs
 *
 * Revision 6.1  1999/01/15 19:41:32  kans
 * Cn3DWin_Entrez stub needed new parameters
 *
 * Revision 6.0  1997/08/25 18:12:23  madden
 * Revision changed to 6.0
 *
 * Revision 1.4  1997/07/29 21:17:04  vakatov
 * [WIN32,DLL]  Made Cn3D's stubbed functions be DLL-exportable
 *
 * Revision 1.3  1997/03/20 19:04:37  vakatov
 * Cn3DWin --> Cn3DWin_Entrez
 *
 * Revision 1.2  1996/07/29  21:13:42  epstein
 * add stub for function Cn3D_SetQueryCallback()
 *
 * Revision 1.1  1996/06/20  19:57:11  epstein
 * Initial revision
 *
 * ==========================================================================
 */

#include <accentr.h>
#ifdef Biostruc_supported
#include <mmdbapi.h>
#endif

NLM_EXTERN void LIBCALL Cn3D_ResetActiveStrucProc(void)
{
}

NLM_EXTERN void LIBCALL Cn3D_Redraw(Boolean New)
{
}

NLM_EXTERN void LIBCALL Cn3D_SetQueryCallback(BeepHook queryFunc, VoidPtr w)
{
}

typedef  struct  Nlm_item {
  Nlm_VoidPtr  dummy;
} HNDL Nlm_IteM;

typedef  void  (*Nlm_ItmActnProc) PROTO((Nlm_IteM));

NLM_EXTERN Handle LIBCALL Cn3DWin_Entrez(Nlm_ItmActnProc netconfig, Boolean usingEntrez)
{
  return NULL;
}
