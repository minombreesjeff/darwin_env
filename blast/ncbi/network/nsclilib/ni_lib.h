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
* File Name:    ni_lib.h
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
* $Log: ni_lib.h,v $
* Revision 6.0  1997/08/25 18:38:49  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 14:11:55  ostell
* Set to revision 5.0
*
 * Revision 4.0  1995/07/26  13:56:32  ostell
 * force revision to 4.0
 *
 * Revision 1.4  1995/05/17  17:52:23  epstein
 * add RCS log revision history
 *
*/

#ifndef _NI_LIB_
#define _NI_LIB_

#include "ncbinet.h"
#include "ni_net.h"

#include "ni_msg.h"

/* DEFINES */

#define NI_TIMEOUT_SECS         60                      /* timeout for connections, requests, etc. */
#define NI_SERV_LISTEN_TIMEOUT  1800                    /* default: servers timeout after 30 minutes */
#define MAX_ALT_DISP_TRIES      2                       /* number of times to try alternate dispatchers */


#endif
