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
* File Name:  mapmla.h
*
* Author:  Epstein
*
* Version Creation Date:   03/31/93
*
* $Revision: 6.0 $
*
* File Description: 
*       Mappings file used to work around incompleteness of automatic ASN.1
*       code generation for Medline Archive object loaders
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 031596   grisha      delete warning: Attemp to redefine NLM_EXTERN_LOADS
*                      without using #undef
*
* ==========================================================================
*
*
* RCS Modification History:
* $Log: mapmla.h,v $
* Revision 6.0  1997/08/25 18:35:36  madden
* Revision changed to 6.0
*
* Revision 5.2  1997/02/06 21:42:48  grisha
* Add new functionality for support Medlars entry transfer
*
 * Revision 5.1  1996/12/16  20:18:23  grisha
 * grisha add definition for support PubMed entry definition
 *
 * Revision 5.0  1996/05/28  14:11:11  ostell
 * Set to revision 5.0
 *
 * Revision 4.1  1996/03/15  18:49:17  grisha
 * Fix warning: Attempt to redefine NLM_EXTERN_LOADS without using #undef
 *              I add undef if NLM_EXTERN_LOADS already defined
 *
 * Revision 4.0  1995/07/26  13:55:12  ostell
 * force revision to 4.0
 *
 * Revision 1.2  1995/05/17  17:53:51  epstein
 * add RCS log revision history
 *
*/

#include <mappubme.h>
#include <objmedli.h>
#include <objpubme.h>
#include <objmdrs.h>
#include <objpub.h>

/* legitimate substitutions to handle differences between existing object */
/* loaders and symbol names generated for automatically-generated object  */
/* loaders (for imported types)                                           */
#define TitlePtr ValNodePtr

#if defined(NLM_EXTERN_LOADS)
#undef NLM_EXTERN_LOADS
#endif
#define NLM_EXTERN_LOADS { if (! objpubmeAsnLoad()) return FALSE; \
                         if (! objmdrsAsnLoad()) return FALSE; \
                         if (! MedlineAsnLoad()) return FALSE; \
                         if (! PubAsnLoad()) return FALSE; \
                         if (! BiblioAsnLoad()) return FALSE; }
