#ifdef NOWAY_ABOUT_TO_BE_ZOMBIED
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
* File Name:    ni_list.h
*
* Author:       Beatty, Gish
*
* Version Creation Date:        1/1/92
*
* $Revision: 6.0 $
*
* File Description: 
*
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 5/12/92  Epstein     Converted tabs to spaces
*
*
* ==========================================================================
*
*
* RCS Modification History:
* $Log: ni_list.h,v $
* Revision 6.0  1997/08/25 18:38:53  madden
* Revision changed to 6.0
*
* Revision 5.1  1997/01/28 21:23:46  epstein
* prepare to be zombied; functionality has been moved to ncbimisc.[ch]
*
 * Revision 5.0  1996/05/28  14:11:55  ostell
 * Set to revision 5.0
 *
 * Revision 4.0  1995/07/26  13:56:32  ostell
 * force revision to 4.0
 *
 * Revision 1.2  1995/05/17  17:52:30  epstein
 * add RCS log revision history
 *
*/

#ifndef _NI_LIST_
#define _NI_LIST_

#include "ncbinet.h"

#define ASCEND  0       /* order for ListSort */
#define DESCEND 1
 

/* FUNCTIONS */

extern NodePtr          ListInsert PROTO((VoidPtr elem, NodePtr after));

extern NodePtr          ListInsertPrev PROTO((VoidPtr elem, NodePtr before));

extern NodePtr          ListDelete PROTO((NodePtr node));

extern NodePtr          ListGetNext PROTO((NodePtr after));

extern void             ListSwapAdj PROTO((NodePtr priornode, NodePtr nextnode));

extern NodePtr          ListSort PROTO((NodePtr sl, int (*cmpfunc)(NodePtr, NodePtr), int order));

extern void             ListBreakRing PROTO((NodePtr np));

extern void             ListConnectRing PROTO((NodePtr np));
extern NodePtr          ListStrCopy PROTO((NodePtr strlist));
extern void             ListStrDel PROTO((NodePtr np));


#endif
#endif /* NOWAY_ABOUT_TO_BE_ZOMBIED */
