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
* File Name: suggcli.c
*
* Author:  Yuri Sadykov
*
* Version Creation Date:   08/14/95
*
* $Revision: 6.0 $
*
* File Description: 
*	Demo of using API for Suggest service
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
*/

#include <ncbi.h>
#include "suggapi.h"

Int2 Main()
{
    AsnIoPtr	pAsnIn;
    AsnIoPtr	pAsnOut;
    SeqAnnotPtr	pSeqAnnot;
    SuggestIntervalsPtr	pIntervals;

    pAsnIn = AsnIoOpen("test.in", "r");
    if (pAsnIn == NULL)
	exit(1);

    pAsnOut = AsnIoOpen("test.out", "w");
    if (pAsnOut == NULL)
	exit(2);

    if (!SuggestInit())
	exit(3);

    pIntervals = SuggestIntervalsAsnRead(pAsnIn, NULL);
    if (pIntervals == NULL)
	exit(4);

    pSeqAnnot = SuggestFindIntervals(pIntervals);
    if (pSeqAnnot == NULL)
	exit(5);

    SeqAnnotAsnWrite(pSeqAnnot, pAsnOut, NULL);
    AsnIoReset(pAsnOut);

    AsnIoClose(pAsnOut);
    AsnIoClose(pAsnIn);

    if (!SuggestFini())
	exit(6);
	
    return 0;
}
