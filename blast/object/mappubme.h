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
* File Name:  mappubme.h
*
* Author:  Grisha Starchenko
*
* Version Creation Date:  5 Nov 1996
*
* $Revision: 6.0 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* ==========================================================================
*
*
* RCS Modification History:
* $Log: mappubme.h,v $
* Revision 6.0  1997/08/25 18:49:03  madden
* Revision changed to 6.0
*
* Revision 1.1  1996/11/07 15:47:00  grisha
* Initial revision
*
*/

#include <objmedli.h>

/* legitimate substitutions to handle differences between existing object */
/* loaders and symbol names generated for automatically-generated object  */
/* loaders (for imported types)                                           */
#define struct_Medline_entry medline

#if defined(NLM_EXTERN_LOADS)
#undef NLM_EXTERN_LOADS
#endif
#define NLM_EXTERN_LOADS { if (! MedlineAsnLoad()) return FALSE; }
