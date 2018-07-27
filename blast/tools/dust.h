/* dust.h
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
* File Name:  dust.h
*
* Author(s):	Roma Tatusov, John Kuzio
*   
* Version Creation Date: 5/26/95
*
* $Revision: 6.1 $
*
* File Description:  a utility to find low complexity NA regions
*
* Modifications:  
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: dust.h,v $
* Revision 6.1  1999/08/18 17:57:56  sicotte
* added includes for prototypes
*
* Revision 6.0  1997/08/25 18:53:06  madden
* Revision changed to 6.0
*
* Revision 5.2  1997/02/14 22:29:48  kuzio
* filter function updates
*
 * Revision 5.1  1997/02/11  00:20:45  kuzio
 * support for graphs added
 *
 * Revision 5.0  1996/05/28  13:43:15  ostell
 * Set to revision 5.0
 *
 * Revision 4.0  1995/07/26  13:50:15  ostell
 * force revision to 4.0
 *
 * Revision 1.1  1995/05/26  18:47:47  kuzio
 * Initial revision
 *
*
* ==========================================================================
*/
#include <objseq.h>
#include <seqport.h>
#include <objloc.h>

extern SeqLocPtr BioseqDust PROTO ((BioseqPtr bsp, Int4 start, Int4 end,
		      Int2 level, Int2 window, Int2 minwin, Int2 linker));
extern SeqLocPtr SeqLocDust PROTO ((SeqLocPtr slp,
		      Int2 level, Int2 window, Int2 minwin, Int2 linker));
extern FloatHiPtr DustGraph PROTO ((SeqPortPtr spp, Int4 length,
		      Int2 level, Int2 window, Int2 minwin, Int2 linker));

/****************************************************************************

Added notes:

	the values of the BioseqDust parameters:
			start, end
	are passed to a SeqPortNew function, and these should be set
	appropriately.

	calling the functions BioseqDust or SeqLocDust with any or all of
	the parameters:
			window, level, minwin, linker
	set to -1, sets the values for those variables to standard
	dust defaults.

	parameter	what it is		default		range

	level		cut off score		20		2 - 64

	window		size of dusting window	64		8 - 256

	minwin		minimum length of	4		4 - 128
			dusted region

	linker		minimum allowable	1		1 - 32
			distance between
			dusted regions

	dust turns all totally ambiguous (N) or invalid nucleotides into
	adenosines (A).  the former is because dust retrieves sequence using
	2-bit encoding; other ambiguities are resolved accordingly.  the
	latter is stubbornness on dust's part; in such cases, an
	informational notice is sent out.

	normalization by length isn't good.  increasing the size of the
	scanning window, thereby increasing the number of triplets looked
	at in a pass (increasing the length), will produce different "dusts".

****************************************************************************/
