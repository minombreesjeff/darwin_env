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
* File Name:	vclient.c
*
* Author:	Beatty
*
* Version Creation Date:	4/9/92
*
* $Revision: 6.0 $
*
* File Description: 
*     Simple Vibrant-based NCBI network services client
*
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*
*
* RCS Modification History:
* $Log: vclient.c,v $
* Revision 6.0  1997/08/25 18:40:12  madden
* Revision changed to 6.0
*
* Revision 5.1  1997/07/18 15:50:08  epstein
* add declaration for catalog deletion function
*
* Revision 5.0  1996/05/28 14:13:55  ostell
* Set to revision 5.0
*
 * Revision 4.1  1995/11/27  21:01:41  epstein
 * add new parameter in NI_SetDispatcher()
 *
 * Revision 4.0  1995/07/26  13:57:04  ostell
 * force revision to 4.0
 *
 * Revision 1.10  1995/05/17  17:53:03  epstein
 * add RCS log revision history
 *
*/

#include <vibrant.h>
#include <panels.h>
#include "ncbinet.h"
#include "echo_asn.h"

#define DISPATCHER   "dispatch1.nlm.nih.gov"
#define USER         "anonymous"
#define TEXT_SIZE    20
#define IOSTR_SIZE   255

static Nlm_TexT WidDialogText PROTO((Nlm_GrouP, Nlm_CharPtr, Nlm_CharPtr, Nlm_Int2, Nlm_TxtActnProc, Nlm_Int2));
static void ReadProc PROTO((TexT));
static void WriteProc PROTO((TexT));
static void SvcReadProc PROTO((TexT));
static void ShowCatalog PROTO((NICatalogPtr));
static void catalogAction PROTO((TablE, Int2, Int2));
static void DismissCatalogProc PROTO((ButtoN));
static void MakeCatWindow PROTO((void));
static void SvcCatalogProc PROTO((ButtoN));
static void DispReadProc PROTO((TexT));
static void DispAcceptProc PROTO((ButtoN));
static void DispCancelProc PROTO((ButtoN));
static void SvcAcceptProc PROTO((ButtoN));
static void SvcCancelProc PROTO((ButtoN));
static void SvcSelectProc PROTO((ButtoN));
static void SendProc PROTO((ButtoN));
static void SvcConnectProc PROTO((ButtoN));
static void SvcDisconnectProc PROTO((ButtoN));
static void DispSelectProc PROTO((ButtoN));
static void DispConnectProc PROTO((ButtoN));
static void DispDisconnectProc PROTO((ButtoN));
static void QuitProc PROTO((IteM));
static Boolean ReadServer PROTO((NI_HandPtr));
static Int2 WriteServer PROTO((NI_HandPtr, CharPtr));
extern Int2 NI_DestroyMsgCatalog(NICatalogPtr cp);


static WindoW  ioWindow;
static GrouP   ioGroup;
static GrouP   dispGroup;
static GrouP   svcGroup;

static ButtoN  serviceButton;
static ButtoN  sendButton;
static ButtoN  svcConnButton;
static ButtoN  svcDisconnButton;
static ButtoN  dispatchButton;
static ButtoN  dispConnButton;
static ButtoN  dispDisconnButton;
static TexT    readText;
static TexT    processText;
static TexT    echoText;
static CharPtr procTextP;
static CharPtr echoTextP;

static WindoW  catalogWindow;
static SlatE   catSlate;
static TablE   catTable;
static ButtoN  dismissButton;
static Int2    currentCatRow;

static WindoW  dispWindow;
static ButtoN  dispAcceptButton;
static ButtoN  dispCancelButton;
static TexT    dispName;
static TexT    userName;
static TexT    groupName;
static TexT    passWord;

static WindoW  svcWindow;
static GrouP   svcSvcGroup;
static GrouP   svcResGroup;
static ButtoN  catalogButton;
static ButtoN  acceptButton;
static ButtoN  cancelButton;
static TexT    service;
static TexT    resource;
static TexT    smin, smax, rmin, rmax;
static Char    svc [64], res [64], svctype[64], tsvmin [8], tsvmax [8], trsmin [8], trsmax [8];
static Char    dispatcher_name [64];
static Char    user_name [64];
static Char    group_name [64];
static Char    pass_word [64];
static Char    dname [64], uname [64], pword[64], gname[64];
static Int2    svmin, svmax, rsmin, rsmax;
static NICatalogPtr   catalog;
static NI_DispatcherPtr dispatcher = NULL;


Boolean dispConnected = FALSE;
Boolean svcConnected = FALSE;
Boolean svcSpecified = FALSE;

NI_HandPtr  shSvc;


static Nlm_TexT
WidDialogText(Nlm_GrouP prnt, Nlm_CharPtr prompt, Nlm_CharPtr dfault, Nlm_Int2 charWidth, Nlm_TxtActnProc actn, Nlm_Int2 promptwid)
{
    GrouP g;

    g = HiddenGroup (prnt, 2, 0, NULL);
    (void) StaticPrompt (g, prompt, stdCharWidth * promptwid,
			 stdLineHeight, systemFont, 'l');
    return DialogText (g, dfault, charWidth, actn);
}

static void
ReadProc (TexT readText)
{
   Char  str [IOSTR_SIZE];   

   GetTitle (readText, str, sizeof (str));
   if (StrngEql (str, "", FALSE)) {
      Disable (sendButton);                                
   }
   else {
      if (svcConnected)
         Enable (sendButton);
   }
} /* ReadProc */

static void
WriteProc (TexT readText)
{
   Disable (sendButton);
} /* ReadProc */

static void
SvcReadProc (TexT t)
{
   GetTitle (service, svc, sizeof (svc));
   GetTitle (smin, tsvmin, sizeof (tsvmin));
   GetTitle (smax, tsvmax, sizeof (tsvmax));
   GetTitle (resource, res, sizeof (res));
   GetTitle (rmin, trsmin, sizeof (trsmin));
   GetTitle (rmax, trsmax, sizeof (trsmax));

   if (StringLen (svc) > 0 && StringLen (tsvmin) > 0 && StringLen (tsvmax) > 0
      && StringLen (res) > 0 && StringLen (trsmin) > 0 && StringLen (trsmax) > 0) {
      Enable (acceptButton);                                
   }
   else {
      Disable (acceptButton);
   }
} /* SvcReadProc */

static void
ShowCatalog (NICatalogPtr cat)
{
   Char  buf [512];
   NIToolsetPtr	tsp;
   NISvcPtr	   svcp;
   NIResPtr	   resp;
   NodePtr		tsnp, np;

   Char  nm[64], desc[255], type[64];
    
   SetTitle (catalogWindow, cat->motd);
    
   tsnp = cat->toolsetL;
   while (tsnp != NULL) {
   	if ((tsp = (NIToolsetPtr) tsnp->elem) != NULL) {
       	if ((np = tsp->services) != NULL) {
            while (np != NULL) {
           		svcp = (NISvcPtr) np->elem;
               StringCpy(nm, svcp->name);
               StringCpy(desc, svcp->descrip);
	       if (svcp->typeL == NULL)
	           StringCpy(type, "NO TYPE");
	       else
	           StringCpy(type, svcp->typeL->elem);
               sprintf(buf, "%s\t%s\t%d\t%d\t%s\n", type, nm, svcp->minVersion, svcp->maxVersion, desc);
               AppendTableText (catTable, buf);
           		np = np->next;
	         }
     	   }
      }
      tsnp = tsnp->next;
   }
   Show (catalogWindow);
   Select (catalogWindow);
} /* ShowCatalog */

static void
catalogAction (TablE t, Int2 row, Int2 col)
{

   if (row != currentCatRow) {
      if (currentCatRow != 0) {
         SetTableBlockHilight (catTable, currentCatRow, currentCatRow, 1, 7, FALSE);
      }
      currentCatRow = row;
      SetTableBlockHilight (catTable, currentCatRow, currentCatRow, 1, 7, TRUE);
   }
   if (dblClick) {
      GetTableText (catTable, row, 1, svctype, sizeof(svctype));
      GetTableText (catTable, row, 2, svc, sizeof(svc));
      GetTableText (catTable, row, 3, tsvmin, sizeof(tsvmin));
      GetTableText (catTable, row, 4, tsvmax, sizeof(tsvmax));
      SetTitle (service, svc);
      SetTitle (smin, tsvmin);
      SetTitle (smax, tsvmax);
      GetTitle (resource, res, sizeof (res));
      GetTitle (rmin, trsmin, sizeof (trsmin));
      GetTitle (rmax, trsmax, sizeof (trsmax));
      Enable (acceptButton);
/*      Remove (catalogWindow); */
      Hide (catalogWindow);
      NI_DestroyMsgCatalog(catalog);
      catalog = NULL;
   }
} /* catalogAction */

static void
DismissCatalogProc (ButtoN dcButton)
{
   Hide (catalogWindow);
/*   Remove (catalogWindow); */
   NI_DestroyMsgCatalog(catalog);
   catalog = NULL;
} /* DismissCatalogProc */

static void
MakeCatWindow (void)
{
   GrouP    pg;
   PrompT   p;
   RecT  r;
   WindoW   tPort;

   catalogWindow = FixedWindow (-50, -33, -10, -10, "Catalog", NULL);
   
   pg = HiddenGroup (catalogWindow, 10, 0, NULL);
   SetGroupMargins (pg, 1, 1);
   SetGroupSpacing (pg, 1, 1);
   p = StaticPrompt (pg, "Type", stdCharWidth * 6, stdLineHeight, systemFont, 'l');
   p = StaticPrompt (pg, "Name", stdCharWidth * 10, stdLineHeight, systemFont, 'l');
   p = StaticPrompt (pg, "Min", stdCharWidth * 3, stdLineHeight, systemFont, 'l');
   p = StaticPrompt (pg, "Max", stdCharWidth * 3, stdLineHeight, systemFont, 'l');
   p = StaticPrompt (pg, "Description", stdCharWidth * 21, stdLineHeight, systemFont, 'l');

   catSlate = ScrollSlate (catalogWindow, 43, 6);
   catTable = TablePanel (catSlate, 0, systemFont, catalogAction);   
   RecordColumn (catTable, 6, 'l', FALSE, FALSE, NULL);
   RecordColumn (catTable, 10, 'l', FALSE, TRUE, NULL);
   RecordColumn (catTable, 3, 'r', FALSE, TRUE, NULL);
   RecordColumn (catTable, 3, 'r', FALSE, TRUE, NULL);
   RecordColumn (catTable, 21, 'l', FALSE, TRUE, NULL);
   currentCatRow = 0;
   ObjectRect (catSlate, &r);
   InsetRect (&r, 4, 4);
   RegisterRect ((PaneL) catTable, &r);
   if (Visible (catTable) && AllParentsVisible (catTable)) {
      tPort = SavePort (catTable);
      Select (catTable);      
      InvalRect (&r);
      RestorePort (tPort);
   }
   dismissButton = DefaultButton (catalogWindow, "Dismiss", DismissCatalogProc);

} /* MakeCatWindow */

static void
SvcCatalogProc (ButtoN scButton)
{
   if ((catalog = NI_GetCatalog (dispatcher)) != NULL) {
      MakeCatWindow ();
      ShowCatalog (catalog);
   } else
      Message (MSG_OK, "Unable to get catalog");
} /* SvcCatalogProc */

static void
DispReadProc (TexT t)
{
   
   GetTitle (dispName, dname, sizeof(dname));
   GetTitle (userName, uname, sizeof(uname));
   GetTitle (groupName, gname, sizeof(gname));
   GetTitle (passWord, pword, sizeof(pword));
   
   if (StringLen(dname) > 0 && StringLen(uname) > 0) { /* password may not be needed */
      Enable (dispAcceptButton);
   }
   else {
      Disable (dispAcceptButton);
   }
}

static void
DispAcceptProc (ButtoN scButton)
{

   StringCpy (dispatcher_name, dname);
   StringCpy (user_name, uname);
   StringCpy (group_name, gname);
   StringCpy (pass_word, pword);
   Hide (dispWindow);
}

static void
DispCancelProc (ButtoN scButton)
{
   Hide (dispWindow);
   SetTitle (dispName, dispatcher_name);
   SetTitle (userName, user_name);
   SetTitle (groupName, group_name);
   SetTitle (passWord, pass_word);
   /* Enable (dispAcceptButton); */
}

static void
SvcAcceptProc (ButtoN scButton)
{
   svmin = atoi (tsvmin);
   svmax = atoi (tsvmax);
   rsmin = atoi (trsmin);
   rsmax = atoi (trsmax);
   Hide (svcWindow);
   Enable (serviceButton);
   svcSpecified = TRUE;
} /* SvcAcceptProc */

static void
SvcCancelProc (ButtoN scButton)
{
   Hide (svcWindow);
   Enable (serviceButton);
} /* SvcCancelProc */

static void
SvcSelectProc (ButtoN scButton)
{
   Disable (serviceButton);
   Disable (acceptButton);
   if (dispConnected)
      Enable (catalogButton);
   else
      Disable (catalogButton);
   Show (svcWindow);
   Select (service);
   Select (svcWindow);
} /* SvcSelectProc */

static void
SendProc (ButtoN sendButton)
{
   Char  str [IOSTR_SIZE];

   Disable (sendButton);
   GetTitle (readText, str, sizeof (str));
   SetTitle (readText, "");

   if (WriteServer (shSvc, str) == 0) {
      if (ReadServer (shSvc)) {
         SetTitle (processText, procTextP);
         MemFree (procTextP);
         SetTitle (echoText, echoTextP);
         MemFree (echoTextP);
      }
   }
   Select (readText);                             
} /* SendProc */

static void
SvcConnectProc (ButtoN cButton)
{
   if (! svcSpecified) {
      Message (MSG_OK, "No service has been specified");
      return;
   }
   if ((shSvc = NI_ServiceGet (dispatcher, svc, svmin, svmax, res, svctype, rsmin, rsmax)) == NULL) {
      Message (MSG_OK, "Unable to get service [%s]: %s", svc, ni_errlist[ni_errno]);
      return;
   }
   svcConnected = TRUE;
   Enable (svcDisconnButton);
   Disable (svcConnButton);
   SetTitle (echoText, "");
   SetTitle (processText, "");
   Select (readText); /* put the insertion point in the read text box */
} /* SvcConnectProc */

static void
SvcDisconnectProc (ButtoN dButton)
{
   if (svcConnected) {
      NI_ServiceDisconnect (shSvc);
      svcConnected = FALSE;
   }
   Enable (svcConnButton);
   Disable (svcDisconnButton);
} /* SvcDisconnectProc */

static void
DispSelectProc (ButtoN dButton)
{
   Disable (dispAcceptButton);
   Show (dispWindow);
   Select (dispName);
   Select (dispWindow);
} /* DispSelectProc */

static void
DispConnectProc (ButtoN cButton)
{
   dispatcher = NI_SetDispatcher (NULL, dispatcher_name, NULL, 0, 0, NULL, FALSE);
   if (NI_InitServices (dispatcher, user_name, group_name[0] == '\0' ? NULL : group_name, pass_word, NULL) < 0) {
      Message (MSG_OK, "Unable to connect to dispatcher");
      return;
   }
   else
      dispConnected = TRUE;

   Disable (dispConnButton);
   Enable (dispDisconnButton);
   Enable (svcConnButton);
} /* DispConnectProc */

static void
DispDisconnectProc (ButtoN dButton)
{
   if (svcConnected) {
      NI_ServiceDisconnect (shSvc);
      svcConnected = FALSE;
   }
   if (dispConnected) {
      NI_EndServices (dispatcher);
      dispatcher = NULL;
      dispConnected = FALSE;
   }
   Disable (dispDisconnButton);
   Disable (svcConnButton);
   Disable (svcDisconnButton);
   Enable (dispConnButton);
} /* DispDisconnectProc */

static void
QuitProc (IteM i)
{
   if (svcConnected)
      NI_ServiceDisconnect (shSvc);
   if (dispConnected) {
      NI_EndServices (dispatcher);
      dispatcher = NULL;
   }
   QuitProgram ();            
} /* QuitProc */

extern Int2
Main ()
{
   GrouP g, gs;
   MenU  m;
   IteM  i;

   /* service window */

   svcWindow = FixedWindow (-60, -40, -10, -10, "Service Selection", NULL);

   gs = HiddenGroup (svcWindow, 0, 3, NULL);
   SetGroupMargins (gs, 10, 10);
   SetGroupSpacing (gs, 10, 10);

   svcSvcGroup = NormalGroup (gs, 0, 10, "Service", NULL, NULL);
   SetGroupMargins (svcSvcGroup, 10, 10);
   SetGroupSpacing (svcSvcGroup, 10, 10);

   svcSvcGroup = NormalGroup (gs, 0, 10, "Service", NULL, NULL);
   SetGroupMargins (svcSvcGroup, 10, 10);
   SetGroupSpacing (svcSvcGroup, 10, 10);
   service = WidDialogText (svcSvcGroup, "Name", "", 8, SvcReadProc, 3);
   Break (svcSvcGroup);
   smin = WidDialogText (svcSvcGroup, "Min", "1", 5, SvcReadProc, 3);
   Advance (svcSvcGroup);
   smax = WidDialogText (svcSvcGroup, "Max", "0", 5, SvcReadProc, 3);

   svcResGroup = NormalGroup (gs, 0, 10, "Resource", NULL, NULL);
   SetGroupMargins (svcResGroup, 10, 10);
   SetGroupSpacing (svcResGroup, 10, 10);
   resource = WidDialogText (svcResGroup, "Name", "echo", 8, SvcReadProc, 3);

   Break (svcWindow);
   catalogButton = PushButton (svcWindow, "Catalog", SvcCatalogProc);
   Advance (svcWindow);
   acceptButton = PushButton (svcWindow, "Accept", SvcAcceptProc);
   Advance (svcWindow);
   cancelButton = PushButton (svcWindow, "Cancel", SvcCancelProc);
   
   /* dispatcher window */
   
   dispWindow = FixedWindow (-30, -20, -10, -10, "Dispatcher Selection", NULL);

   g = HiddenGroup (dispWindow, 0, 4, NULL);
   SetGroupMargins (g, 10, 10);
   SetGroupSpacing (g, 10, 10);
   
   StringCpy (dispatcher_name, DISPATCHER);
   StringCpy (user_name, USER);
   StringCpy (group_name, "");
   StringCpy (pass_word, "");
   
   gs = HiddenGroup (g, 2, 0, NULL);
   StaticPrompt (gs, "Dispatcher Name", 0, 0, systemFont, 'l');
   dispName = DialogText (gs, dispatcher_name, TEXT_SIZE, DispReadProc);
   StaticPrompt (gs, "User Name", 0, 0, systemFont, 'l');
   userName = DialogText (gs, user_name, TEXT_SIZE, DispReadProc);
   StaticPrompt (gs, "Group Name", 0, 0, systemFont, 'l');
   groupName = DialogText (gs, group_name, TEXT_SIZE, DispReadProc);
   StaticPrompt (gs, "Password", 0, 0, systemFont, 'l');
   passWord = PasswordText (gs, pass_word, TEXT_SIZE, DispReadProc);
   
   Break (dispWindow);
   dispAcceptButton = PushButton (dispWindow, "Accept", DispAcceptProc);
   Advance (dispWindow);
   dispCancelButton = PushButton (dispWindow, "Cancel", DispCancelProc);
  
   /* main window */

   ioWindow = FixedWindow (-50, -33, -10, -10, "Network Services Test Client", NULL);
   m = PulldownMenu (ioWindow, "File");
   i = CommandItem (m, "Quit", QuitProc);

   g = HiddenGroup (ioWindow, 0, 3, NULL);
   SetGroupMargins (g, 10, 10);
   SetGroupSpacing (g, 10, 10);

   ioGroup = NormalGroup (g, 0, 10, "Echo", NULL, NULL);
   SetGroupMargins (ioGroup, 10, 10);
   SetGroupSpacing (ioGroup, 10, 10);

   gs = HiddenGroup (ioGroup, 2, 0, NULL);
   StaticPrompt (gs, "Enter Text", 0, 0, systemFont, 'l');
   readText = DialogText (gs, "", TEXT_SIZE, WriteProc);
   StaticPrompt (gs, "Process", 0, 0, systemFont, 'l');
   processText = DialogText (gs, "", TEXT_SIZE, WriteProc);
   StaticPrompt (gs, "Echo Text", 0, 0, systemFont, 'l');
   echoText = DialogText (gs, "", TEXT_SIZE, WriteProc);

   sendButton = PushButton (ioGroup, "Send", SendProc);
   Disable (sendButton);
   Select (readText);

   svcGroup = NormalGroup (g, 0, 10, "Service", NULL, NULL);
   SetGroupMargins (svcGroup, 10, 10);
   SetGroupSpacing (svcGroup, 30, 10);
   serviceButton = PushButton (svcGroup, "Specify", SvcSelectProc);
   Advance (svcGroup);
   svcConnButton = PushButton (svcGroup, "Connect", SvcConnectProc);
   Advance (svcGroup);
   svcDisconnButton = PushButton (svcGroup, "Disconnect", SvcDisconnectProc);
   Disable (svcConnButton);
   Disable (svcDisconnButton);

   dispGroup = NormalGroup (g, 0, 10, "Dispatcher", NULL, NULL);
   SetGroupMargins (dispGroup, 10, 10);
   SetGroupSpacing (dispGroup, 30, 10);
   dispatchButton = PushButton (dispGroup, "Specify", DispSelectProc);
   Advance (dispGroup);
   dispConnButton = PushButton (dispGroup, "Connect", DispConnectProc);
   Advance (dispGroup);
   dispDisconnButton = PushButton (dispGroup, "Disconnect", DispDisconnectProc);
   Disable (dispDisconnButton);

   Show (ioWindow);
   ProcessEvents ();
   return 0;
} /* Main */

static Boolean
ReadServer (NI_HandPtr hp)
{
    AsnTypePtr	atp;
    AsnIoPtr	aip;
    DataVal	value;
    
    aip = hp->READ_AIP;
    atp = ECHO_STRING;
    
    if ((atp = AsnReadId(aip, amp, atp)) != ECHO_STRING) {
	if (atp != NULL)
	    AsnReadVal(aip, atp, &value);
   	return FALSE;
    }
    AsnReadVal(aip, atp, &value);
    
    if ((atp = AsnReadId(aip, amp, atp)) != ECHO_STRING_process) {
   	AsnReadVal(aip, atp, &value);
	   return FALSE;
    }
    AsnReadVal(aip, atp, &value);
    procTextP = value.ptrvalue;

    if ((atp = AsnReadId(aip, amp, atp)) != ECHO_STRING_text) {
	   AsnReadVal(aip, atp, &value);
   	return FALSE;
    }
    AsnReadVal(aip, atp, &value);
   echoTextP = value.ptrvalue;
    
    if ((atp = AsnReadId(aip, amp, atp)) != ECHO_STRING) {
      AsnReadVal(aip, atp, &value);
   	return FALSE;
    }
    AsnReadVal(aip, atp, &value);
    
    return TRUE;
} /* readServer*/

static Int2
WriteServer (NI_HandPtr hp, CharPtr buf)
{
    AsnIoPtr	aip;
    DataVal	value;
    
    aip = hp->WRITE_AIP;
    AsnStartStruct(aip, ECHO_STRING);
    value.ptrvalue = StringSave("WinClient");
    AsnWrite(aip, ECHO_STRING_process, &value);
    value.ptrvalue = StringSave(buf);
    AsnWrite(aip, ECHO_STRING_text, &value);
    AsnEndStruct(aip, ECHO_STRING);
    AsnIoFlush(aip);
    return 0;
} /* writeServer */


