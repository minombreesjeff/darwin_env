/*  objall.c
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
* File Name:  objall.c
*
* Author:  James Ostell
*   
* Version Creation Date: 9/19/91
*
* $Revision: 6.1 $
*
* File Description:  Object manager for "ALL" modules
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 05-13-93 Schuler     All public functions are now declared LIBCALL.
*
* $Log: objall.c,v $
* Revision 6.1  1998/08/24 18:27:58  kans
* removed solaris -v -fd warnings
*
* Revision 6.0  1997/08/25 18:49:15  madden
* Revision changed to 6.0
*
* Revision 4.1  1997/06/19 18:40:43  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 4.0  1995/07/26 13:48:06  ostell
* force revision to 4.0
*
 * Revision 3.1  1995/05/15  21:22:00  ostell
 * added Log line
 *
*
*
*
* ==========================================================================
*/
#include <objall.h>		   /* the Bioseq-set interface */

/*****************************************************************************
*
*   AllObjLoad()
*
*****************************************************************************/
NLM_EXTERN AsnModulePtr LIBCALL AllObjLoad (void)
{
    if (SeqSetAsnLoad())     /* load the most complicated object loader */
        return AsnAllModPtr();
    else
        return NULL;
}

