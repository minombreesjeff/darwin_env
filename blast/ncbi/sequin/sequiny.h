/*   sequiny.h
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*            National Center for Biotechnology Information (NCBI)
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government do not place any restriction on its use or reproduction.
*  We would, however, appreciate having the NCBI and the author cited in
*  any work or product based on this material
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
* ===========================================================================
*
* File Name:  sequiny.h
*
* Author:  Jim Ostell
*
* Version Creation Date:   5/24/96
*
* $Revision: 6.0 $
*
* File Description: header file for sequiny.c -- demo of how to add your
*      own functions to sequin 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#ifndef _SEQUIN_
#include "sequin.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/***** this section defines the functions *****/

Int2 LIBCALLBACK MyBioseqEditFunc (Pointer data);  /* the work function */

/** the define below is used to register the function above ****
    OMPROC_EDIT - it is class editor
	"My Edit Bioseq" - is the name of the proceedure
	"MyBioseqEditor" - will appear on the desktop menu under "Edit"
	OBJ_BIOSEQ - input is a Bioseq
	2 - only Bioseq.repr = "raw"
	OBJ_BIOSEQ - output is a Bioseq
	2 - only Bioseq.repr = "raw"
	NULL - pointer to user data. Will be passed back whenever proceedure is invoked
	MyBioseqEditFunc - the function to call when procedure is invoked
	PROC_PRIORITY_DEFAULT - this function will be higher priority than any registered
	      before this call, and lower than any registered after this call.

*************************************************************/

#define REGISTER_MY_BIOSEQ_EDIT ObjMgrProcLoad(OMPROC_EDIT,"My Edit Bioseq","MyBioseqEditor",OBJ_BIOSEQ,2,OBJ_BIOSEQ,2,NULL,MyBioseqEditFunc,PROC_PRIORITY_DEFAULT)


#ifdef __cplusplus
}
#endif


