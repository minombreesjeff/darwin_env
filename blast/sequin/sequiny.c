/*   sequiny.c
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
* File Name:  sequiny.c
*
* Author:  Jim Ostell
*
* Version Creation Date:   5/24/96
*
* $Revision: 6.0 $
*
* File Description:
*    This is a companion file to sequinx.c. In order to register additional proceedures
*    of your own design, to replace or supplement those intrinsic to sequin,
*    we have provided sequinx.c where you can register your functions with the
*    object manager. For demonstration purposes, we have included a REGISTER call
*    (commented out) to functions in this file.
*
*    This file contains the demonstration functions themselves, as well as extensive
*    comments describing what they do. They are intended to serve as a model for what
*    you might do in a file of your own design.
*
*    Basically, to register a proceedure with the object manager, you call a function
*    which specifies a name for the function, a string to show in a menu, an input
*    data type (OBJ_ in objmgr.h), possibly a subtype, and an output type, and possibly
*    a subtype, the type of function (filter, editor, etc), and a pointer to the function
*    to be called. The called function always
*    returns an Int2 (whose values are defined in objmgr.h as OM_MSG_RET_...) and takes
*    a single Pointer as an argument. This pointer always must be cast to an OMProcControlPtr
*    (objmgr.h) which contains the input arguments. It is only declared as a Pointer because
*    of C typedef order limitations. We will call this function the "work" function.
*
*    The work function does the real job. It looks at the contents of the data pointer,
*    executes the appropriate actions and returns a completion or error code. Normally it
*    will return one of:
*      OM_MSG_RET_ERROR - an error occured, presumably reported with ErrPostEx()
*      OM_MSG_RET_OK - everything was ok, but this function did not do anything.
*      OM_MSG_RET_DONE - everything was ok, and this function did the processing requested.
*
*    When the object manager looks for a function to satisfy a request it enters a loop
*    which goes through appropriate functions until the action is done. First, it goes
*    through all registered functions of the requested class (editor, filter,..) in priority
*    order. First it looks for a match of type, and subtype. Failing that, it looks for a
*    match to type, with subtype = 0 (any). For each match, it calls the function with the
*    data. If the functions returns OM_MSG_RET_ERROR, or OM_MSG_RET_OK, the object manager
*    tries the next function. When a function returns OM_MSG_RET_DONE or there are no more
*    matching functions, the object manager returns. This means your functions can be called
*    before or after the default functions (priority), or by specifying a subtype not
*    supported by a default function. You can have your function called first, check the
*    data to decide if you want to handle it, return OM_MSG_RET_DONE if you do, or 
*    OM_MSG_RET_OK if you want the object manager to find some other function to handle it.
*
*    If you wish your function to receive messages from the object manager you must also
*    supply a message loop function and register it with the object manager when your
*    function is first called. You must remember to unregister it when your function exits.
*    If your function performs some quick computation and returns, you probably don't need
*    a message loop function. If it will persist awhile (like an editor or viewer) you
*    probably will. This is for cases such as 1) your function is a viewer showing Bioseq X
*    2) some other function is an editor on Bioseq X. When Bioseq X is updated in the editor
*    it will send an update message to the object manager. Your function will want to receive
*    this message so it can update its display from the underlying (now edited) data
*    appropriately. Your message loop should return OM_MSG_RET_OK (not _DONE) so the object
*    manager will continue to relay the message to all functions that want to see it.
*
*     
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#include <gather.h>
#include "sequin.h"
#include "sequiny.h"


							/** prototype for message loop function **/
static Int2 LIBCALLBACK MyBioseqEditMsgFunc (OMMsgStructPtr ommsp);

Int2 LIBCALLBACK MyBioseqEditFunc (Pointer data)
{
  BioseqPtr         bsp;
  OMProcControlPtr  ompcp;
  Char buf[41];
  OMUserDataPtr omudp;  /* user data structure used for message loop */
  Uint2 userkey;        /* used to track omudp uniquely */

  ompcp = (OMProcControlPtr) data;   /* always do this cast */

									 /* check that all is cool */
  if (ompcp == NULL) return OM_MSG_RET_ERROR;
  if (ompcp->input_itemtype != OBJ_BIOSEQ) return OM_MSG_RET_ERROR;

  if (ompcp->input_data == NULL)     /* no BioseqPtr yet */
  {
	  if (! GatherDataForProc (ompcp, FALSE))  /* find it -- gather.h */
	  {
		  ErrPostEx(SEV_ERROR,0,0,"MyBioseqEditFunc: couldn't gather BioseqPtr");
		  return OM_MSG_RET_ERROR;
	  }
  }

  bsp = (BioseqPtr)(ompcp->input_data);   /* get the pointer */

  BioseqLabel(bsp, buf, 40, OM_LABEL_BOTH);   /* make a text string to show */

  Message(MSG_OK, "MyBioseqEditFunc called with [%s]", buf);   /* show it */

  /* assuming we need a message loop ************************************************/

	userkey = OMGetNextUserKey ();		  /* get a unique key for this proc instance */
	                                      /* only necessary if the same proc could be
										     invoked multiple times at once */
	                                      /* attach data to the entity for this procedure */
	omudp = ObjMgrAddUserData(ompcp->input_entityID, ompcp->proc->procid, OMPROC_EDIT, userkey);
	omudp->userdata.ptrvalue = (Pointer)bsp; /* this could point to any data */
	omudp->messagefunc = MyBioseqEditMsgFunc; /* add my message loop */


	                                      /* now we can send a message to ourselves */
	                                      /* just to prove it works. The message could just
										  /* as well be from another proceedure though */

	ObjMgrSendMsg (OM_MSG_SELECT, ompcp->input_entityID, 0, 0);

   /*** end of message loop setup ***************************************************/

	/** here you would do whatever work you wanted on this entry **/
	/** in this case we will just arbitrarily change the length to 10 */

	bsp->length = 10;	       /* make the change */
	                           /* inform any listening proceedures */
	ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, ompcp->input_itemID, ompcp->input_itemtype);

  /*** if we set up the message loop, we need to clean up before we leave *****/

	ObjMgrFreeUserData(ompcp->input_entityID, ompcp->proc->procid, OMPROC_EDIT, userkey);

  /**  Now we are done. Give a Done return code *******************************/

	return OM_MSG_RET_DONE;
}

/******************************************************************************************
*
*  Here's the sample message loop function. It really does nothing but say hello
*
*******************************************************************************************/
static Int2 LIBCALLBACK MyBioseqEditMsgFunc (OMMsgStructPtr ommsp)
{
	OMUserDataPtr omudp;
	BioseqPtr bsp;
	Char buf[41];
   
	omudp = (OMUserDataPtr)(ommsp->omuserdata);
	bsp = (BioseqPtr)(omudp->userdata.ptrvalue);

	BioseqLabel(bsp, buf, 40, OM_LABEL_BOTH);   /* make a text string to show */
	
	switch (ommsp->message)
	{
		case OM_MSG_DEL:
			break;
		case OM_MSG_CREATE:
			break;
		case OM_MSG_UPDATE:
			Message(MSG_OK, "Got an update message on [%s]", buf);
			break;
		case OM_MSG_SELECT:          /* add highlight code */
			Message(MSG_OK, "Got a select message on [%s]", buf);
			break;
		case OM_MSG_DESELECT:        /* add deselect code */
			break;
		case OM_MSG_CACHED:
			break;
		case OM_MSG_UNCACHED:
			break;
		case OM_MSG_TO_CLIPBOARD:  /* this is just because no clipboard now */
			break;
		default:
			break;
	}

	return OM_MSG_RET_OK;
}
