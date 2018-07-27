/*   ncbimain.c
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
* File Name:  ncbimain.c
*
* Author:  Ostell, Schuler, Vakatov
*
* Version Creation Date:   07/15/91
*
* $Revision: 6.2 $
*
* File Description: 
*   	portable main() for console applications
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: ncbimain.c,v $
* Revision 6.2  2002/07/09 15:20:17  lavr
* Call CONNECT_Init(0) in initialization sequence
*
* Revision 6.1  1998/02/13 15:18:15  vakatov
* Added cleanup for the application context and thread library internals
*
* Revision 6.0  1997/08/25 18:16:30  madden
* Revision changed to 6.0
*
* Revision 5.1  1997/07/22 19:04:17  vakatov
* Initial revision:  merged Main()'s from former "ncbimain.[msw,unx,mac,vms]"
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <ncbithr.h>
#include <ncbiwin.h>

#include <connect/ncbi_core_c.h>

#ifdef COMP_THINKC
#include <console.h>
#endif

#ifdef COMP_MPW
#pragma segment NlmSegA
#endif


/*****************************************************************************
*
*   main()
*     this replaces the normal program main() for console applications
*     setups argc and argv
*
*****************************************************************************/
#ifdef OS_MSWIN
int
#endif
main(int argc, char *argv[])
{
  Nlm_Int2 retval;

  Nlm_SetupArguments(argc, argv);

#ifdef MSC_VIRT
  if ( !_vheapinit(0, 1250, _VM_ALLSWAP) )
    {
      ErrPost(CTX_NCBIOBJ, 1, "Can't open virtual memory");
      return 1;
    }
#endif

  /* Initialize connection library's logger, registry and lock */
  CONNECT_Init(0);

  retval = Nlm_Main();

  NlmThreadJoinAll();

  Nlm_FreeConfigStruct();
  ErrSetLogfile(NULL, 0);
  Nlm_ReleaseAppContext();

#ifdef MSC_VIRT
  _vheapterm();
#endif

  NlmThreadDestroyAll();

#if   defined(OS_MSWIN)
  return retval;
#elif defined(OS_UNIX) || defined(OS_VMS)
  exit( retval );
#endif
}
