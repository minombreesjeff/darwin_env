/*   dtapp.c
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
* File Name:  dtapp.c
*
* Author:  Jim Ostell
*
* Version Creation Date:   11-29-94
*
* $Revision: 6.0 $
*
* File Description: Demo application in which the desktop is the main application
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#include <vsm.h>

/*** Main Program.. just initializes funcs, puts up a window ***/
    /** these are prototypes for two fake functions to register **/
static Int2 LIBCALLBACK FakeFunc1 PROTO((Pointer data));
static Int2 LIBCALLBACK FakeFunc2 PROTO((Pointer data));

    /** here is your main program **/
    
Int2 Main (void)
{
	Uint2 procid1, procid2;
	
	        /* register the functions. One takes any input type, One takes only a Seq-feat
	           they are both pretending to be viewers (in View in desktop), but will really
	           only put up a Message box */
	           
	procid1 = ObjMgrProcLoad(OMPROC_VIEW, "Fake viewer. Takes any type.", "FakeView1",
					0,0,0,0,NULL,FakeFunc1,PROC_PRIORITY_DEFAULT);
					
	procid2 = ObjMgrProcLoad(OMPROC_VIEW, "Fake viewer. Takes only Seq-feat.", "FakeView2",
					OBJ_SEQFEAT,0,0,0,NULL,FakeFunc2,PROC_PRIORITY_DEFAULT);
					
	VSeqMgrRun("Jim's DeskTop Demo", "Demonstration desktop application.~Just shows a few messages.~Jim Ostell~NCBI");

	return 0;
}

/*** these are the procedures called by the desktop. data will ALWAYS be a OMProcControlPtr.
*     the Pointer is because of order of typedefs in the header. It will be filled in with
*     the input entityID, data, etc. You are expected to fill in the output if necessary,
*     to send any update messages to the ObjMgr, and so on before returning
****/

static Int2 LIBCALLBACK FakeFunc1 (Pointer data)
{
	OMProcControlPtr ompcp;
	ObjMgrProcPtr ompp;
	Pointer my_userdata;
	
	ompcp = (OMProcControlPtr)data;
	ompp = ompcp->proc;      /* this is your proceedure */
	my_userdata = ompp->procdata;   /* this is the user data pointer you passed in, if any */
	
	Message(MSG_OK, "Called FakeFunc1: input_entityID=%d input_itemtype=%d input_data=%ld",
		(int)(ompcp->input_entityID), (int)(ompcp->input_itemtype),
		(long)(ompcp->input_data));
		
	return OM_MSG_RET_OK;
}


static Int2 LIBCALLBACK FakeFunc2 (Pointer data)
{
	OMProcControlPtr ompcp;
	ObjMgrProcPtr ompp;
	Pointer my_userdata;
	
	ompcp = (OMProcControlPtr)data;
	ompp = ompcp->proc;      /* this is your proceedure */
	my_userdata = ompp->procdata;   /* this is the user data pointer you passed in, if any */
	
	Message(MSG_OK, "Called FakeFunc2: input_entityID=%d input_itemtype=%d",
		(int)(ompcp->input_entityID), (int)(ompcp->input_itemtype));
		
	return OM_MSG_RET_OK;
}
