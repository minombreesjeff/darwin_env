/*   ncbi.h
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
* File Name:  ncbi.h
*
* Author:  Gish, Kans, Ostell, Schuler
*
* Version Creation Date:   1/1/91
*
* $Revision: 6.2 $
*
* File Description: 
*   	this includes typedefs, defines, and prototypes for the basic
*   	ncbi portable programming system
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: ncbi.h,v $
* Revision 6.2  1999/04/14 20:04:46  vakatov
* For better consistency, moved <ncbiopt.h> from <ncbi.h> to <ncbistd.h>
*
* Revision 6.1  1998/08/11 22:11:05  vakatov
* + <ncbiopt.h>
*
* Revision 5.1  1997/05/09 16:01:20  vakatov
* "ncbicoll.[ch]" is not being used anywhere anymore -- remove it!
* Move "ncbiwww.h" and "wwwutils.c" from /network/www2(ncbiwww2.lib)
* to /corelib(ncbi.lib)
* ==========================================================================
*/

#ifndef _NCBI_
#define _NCBI_

#include <ncbilcl.h>
#include <ncbistd.h>
#include <ncbimem.h>
#include <ncbimsg.h>
#include <ncbierr.h>
#include <ncbistr.h>
#include <ncbibs.h>
#include <ncbitime.h>
#include <ncbifile.h>
#include <ncbimath.h>
#include <ncbimisc.h>
#include <ncbiprop.h>
#include <ncbisgml.h>
#include <ncbienv.h>
#include <ncbiwww.h>
#include <ncbimain.h>

#endif  /* _NCBI_ */
