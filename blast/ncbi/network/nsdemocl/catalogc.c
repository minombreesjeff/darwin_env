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
* File Name:	catalogc.c
*
* Author:	Epstein, Shavirin
* 
* Version Creation Date:	1/1/92
*
* $Revision: 6.3 $
*
* File Description: Client to request services catalog from the dispatcher
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
* $Log: catalogc.c,v $
* Revision 6.3  2000/01/13 16:31:22  shavirin
* Added environment and notice of old dispatcher name.
*
* Revision 6.2  2000/01/13 16:12:38  beloslyu
* to be sure we are testing an old dispatcher
*
* Revision 6.1  1998/09/14 19:57:52  shavirin
* Initial revision with this filename.
*
* Revision 6.0  1997/08/25 18:40:05  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 14:13:55  ostell
* Set to revision 5.0
*
 * Revision 4.1  1995/11/27  21:01:47  epstein
 * add new parameter in NI_SetDispatcher()
 *
 * Revision 4.0  1995/07/26  13:57:04  ostell
 * force revision to 4.0
 *
 * Revision 1.8  1995/05/17  17:52:55  epstein
 * add RCS log revision history
 *
*/

#include "ncbinet.h"
#include "ni_net.h"

#include "echo_asn.h"


#define IOBUFSIZ	1024
#define SMBUFSIZ	256

static Int2		getUserInput PROTO((CharPtr bp, CharPtr prompt));
static Int2		getService PROTO((CharPtr bp, NI_DispatcherPtr disp));
static Int2 		getResource PROTO((CharPtr bp));
static Int2		getVersions PROTO((Uint2 *min, Uint2 *max));
static Boolean		getYN PROTO((CharPtr s));
static void      	printCatalog PROTO((NICatalogPtr catp));
static void      	printCatalogWithBars PROTO((NICatalogPtr catp));
static CharPtr		readServer PROTO((NI_HandPtr hp));
static Int2		writeServer PROTO((NI_HandPtr hp, CharPtr buf));

#ifdef COMP_THINKC
#include <console.h>
#define OS_UNIX 1 /* lie */
#endif /* COMP_THINKC */


main(int argc, CharPtr argv[])
{
#ifdef OS_UNIX
    extern CharPtr	optarg;
    Int2		errflg = 0, c;
#endif
    Int2        status;
    CharPtr		dispatcher = NULL, service = NULL, username = NULL;
    NI_HandPtr	svc_handle;
    Char		svcname[SMBUFSIZ], iobuf[IOBUFSIZ];
    Char		cuser[SMBUFSIZ], resname[SMBUFSIZ];
    Uint2		min, max, rmin, rmax;
    CharPtr		intext;
    NI_ReqPtr	reqp;	/* for user constructed request */
    CharPtr             module;
    NI_DispatcherPtr    disp;
    static CharPtr env_string = "SRV_CONN_MODE=DISPATCHER";

#ifdef COMP_THINKC
    argc = ccommand(&argv);
#endif /* COMP_THINKC */
    module = argv[0];
    
#ifdef OS_UNIX

    putenv(env_string);

    while ((c = getopt(argc, argv, "d:s:u:")) != -1)
	switch (c) {
	  case 'd':
	    dispatcher = optarg;
	    break;
	  case 's':
	    service = optarg;
	    break;
	  case 'u':
	    username = optarg;
	    break;
	  case '?':
	    default:
	    ++errflg;
	    
	}
    if (errflg > 0) {
	fprintf(stderr, "\nUsage: %s [-d dispatcher host] [-s service] [-u username]\n", module);
	exit(1);
    }
#else
    switch (argc) {
      case 4:
	username = argv[3];
      case 3:
	service = argv[2];
      case 2:
	dispatcher = argv[1];
	break;
      case 1:
	break;
	default:
	printf("\n Error: too many parameters\n");
	exit(1);
    }
#endif
    
    RECONNECT:
    
    disp = NI_SetDispatcher(NULL, dispatcher, service, 0, 0, NULL, FALSE);		/* NULL vals = defaults */
    
    if (username == NULL) {
	if (getUserInput(cuser, "\nEnter user name: ") == -1) {
	    printf("\nDone\n");
	    exit(0);
	}
    }
    else
	StringNCpy(cuser, username, SMBUFSIZ);
    
    
    if (NI_InitServices(disp, cuser, NULL, NULL, NULL) < 0) {
	printf("\nUnable to initiate services - %s\n", ni_errlist[ni_errno]);
	exit(1);
    }
    printf("\n=== Connected to NCBI Dispatcher '%s' ===\n", 
           disp->dispHostName);

    printf("\nEnter \"cat\" to get a catalog or \"catbars\" to get a machine-parsable catalog\n");
    
    while (getService(svcname, disp) == 0) {
	if (getVersions(&min, &max) != 0)
	    break;
	reqp = NI_SVCRequestBuild(disp);
	NI_RequestSetService(reqp, svcname, min, max);
	
	while ((status = getResource(resname)) == 0) {
	    if (getVersions(&rmin, &rmax) != 0)
		break;
	    NI_RequestAddResource(reqp, resname, "TEST", rmin, rmax);
	}
	if (status == -1)
	    break;
	if ((svc_handle = NI_ServiceRequest(reqp)) == NULL) {
	    printf("\nUnable to get service - %s: %s\n", ni_errlist[ni_errno], ni_errtext);
	    continue;
	}
	else {
	    printf("\nRequest OK: %s  %d - %d\n", disp->reqResponse->service->name, disp->reqResponse->service->minVersion, disp->reqResponse->service->maxVersion);
	}
	
	while (getUserInput(iobuf, "\nEnter message for Application: ") == 0) {
	    if (writeServer(svc_handle, iobuf) == 0) {
		if ((intext = readServer(svc_handle)) != NULL) {
		    printf("\nService echoed:\n%s\n", intext);
		    MemFree(intext);
		}
		else {
		    printf("\nService disconnected\n");
		    break;
		}
	    }
	    else {
		printf("\nServer disconnected (write)\n");
		break;
	    }
	}
	NI_ServiceDisconnect(svc_handle);
    }
    NI_EndServices(disp);
    printf("\nDisconnected from dispatcher\n");
    if (getYN("Do you want to re-connect? (y/n): "))
	goto RECONNECT;
    exit(0);
} /* main */


static Int2 getService(CharPtr bp, NI_DispatcherPtr disp)
{
    NICatalogPtr	catalog;
    
    RESET:
    printf("\nEnter service: ");
    gets(bp);
    if (strcasecmp(bp, "stop") == 0)
	return -1;
    
    if (strcasecmp(bp, "cat") == 0 || strcasecmp(bp, "catbars") == 0) {
	if ((catalog = NI_GetCatalog(disp)) != NULL) {
	    if (strcasecmp(bp, "cat") == 0)
	       printCatalog(catalog);
	    else
	       printCatalogWithBars(catalog);
	    NI_DestroyMsgCatalog(catalog);
	} else
	    printf("Error getting catalog - %s\n", ni_errlist[ni_errno]);
	goto RESET;
    }
    return 0;
} /* getService */


static Int2 getResource(CharPtr bp)
{
    printf("Enter resource: ");
    gets(bp);
    if (StringLen(bp) == 0)
	return 1;
    if (strcasecmp(bp, "stop") == 0)
	return -1;
    return 0;
} /* getResource */


static Int2 getVersions(Uint2 *min, Uint2 *max)
{
    unsigned	tmin, tmax;
    
    printf("Enter minimum version: ");
    scanf("%u", &tmin);
    printf("Enter maximum version: ");
    scanf("%u", &tmax);
    getchar();
    *min = (Uint2) tmin;
    *max = (Uint2) tmax;
    return 0;
} /* getVersions */


static Int2 getUserInput(CharPtr bp, CharPtr prompt)
{
    printf(prompt);
    gets(bp);
    if (strcasecmp(bp, "stop") == 0)
	return -1;
    return 0;
} /* getUserInput */


static Boolean getYN(CharPtr s)
{
    Char		ans[20];
    
    printf("%s", s);
    gets(ans);
    if (ans[0] == 'y' || ans[0] == 'Y')
	return TRUE;
    else
	return FALSE;
} /* getYN */


static Boolean
ResourceCompatWService(NIResPtr resp, NISvcPtr service)
{
    NodePtr np = service->typeL;
 
    if (np == NULL)
        return FALSE;
 
    do { 
        np = np->next;
        if (strcasecmp(np->elem, MATCHES_ANY_TYPE) == 0 ||
            strcasecmp(np->elem, resp->type) == 0)
            return TRUE;
    } while (np != NULL && np != service->typeL);

    return FALSE;
}


static void printCatalogWithBars(NICatalogPtr cat)
{
    NIToolsetPtr	tsp;
    NISvcPtr	svcp;
    NIResPtr	resp;
    NodePtr		tsnp, np, np2;
    
    printf("\n==============================================================================\n");
    printf("== %-72.72s ==\n", cat->motd);
    printf("==============================================================================\n");
    printf ("SERVER|SVC NAME|SVC DESCR|SVC MIN|SVC MAX|SVC PRIORITY|RES NAME|RES DESCR|RES MIN|RES MIN|RES TYPE\n");
    
    tsnp = cat->toolsetL;
    for (tsnp = cat->toolsetL; tsnp != NULL; tsnp = tsnp->next)
    { /* for each server */
	tsp = (NIToolsetPtr) tsnp->elem;
	for (np = tsp->services; np != NULL; np = np->next)
	{ /* for each service */
	    svcp = (NISvcPtr) np->elem;
	    for (np2 = tsp->resources; np2 != NULL; np2 = np2->next)
	    { /* for each resource */
		resp = (NIResPtr) np2->elem;
		if (ResourceCompatWService(resp,svcp))
		{
		    printf ("%s|%s|%s|%d|%d|%d|%s|%s|%d|%d|%s\n",
			    tsp->host, svcp->name, svcp->descrip,
			    svcp->minVersion, svcp->maxVersion, svcp->priority,
			    resp->name, resp->descrip, resp->minVersion,
			    resp->maxVersion, resp->type);
		}
	    }
	}
    }
}

static void printCatalog(NICatalogPtr cat)
{
    NIToolsetPtr	tsp;
    NISvcPtr	svcp;
    NIResPtr	resp;
    NodePtr		tsnp, np;
    
    printf("\n==============================================================================\n");
    printf("== %-72.72s ==\n", cat->motd);
    printf("==============================================================================\n");
    
    tsnp = cat->toolsetL;
    while (tsnp != NULL) {
	tsp = (NIToolsetPtr) tsnp->elem;
	printf("SERVER: %s  [%s]\n", tsp->host, tsp->motd);
	if ((np = tsp->services) != NULL) {
	    Boolean highest;
	    NIToolsetPtr tsp2;
            NodePtr	tsnp2, np2;
	    NISvcPtr	svcp2;

	    printf("SERVICES:\n");
	    while (np != NULL) {
		svcp = (NISvcPtr) np->elem;
		highest = TRUE;
		for (tsnp2 = cat->toolsetL; tsnp2 != NULL && highest; tsnp2 = tsnp2->next)
		{
		    tsp2 = (NIToolsetPtr) tsnp2->elem;
		    for (np2 = tsp2->services; np2 != NULL && highest; np2 = np2->next)
		    {
			svcp2 = (NISvcPtr) np2->elem;
			if (StrCmp(svcp->name, svcp2->name) == 0)
			    highest = svcp->priority >= svcp2->priority;
		    }
		}
		printf("%c  %-15.15s Vers. %2d-%2d  Pr.%3d %-39.39s\n", highest ? '*' : ' ', svcp->name, svcp->minVersion, svcp->maxVersion, svcp->priority, svcp->descrip);
		np = np->next;
	    }
	}
	if ((np = tsp->resources) != NULL) {
	    printf("RESOURCES:\n");
	    while (np != NULL) {
		resp = (NIResPtr) np->elem;
		printf("   %-15.15s Vers. %2d-%2d   %-45.45s\n", resp->name, resp->minVersion, resp->maxVersion, resp->descrip);
		np = np->next;
	    }
	}
	printf("------------------------------------------------------------------------------\n");
	tsnp = tsnp->next;
    }
} /* printCatalog */


static CharPtr readServer(NI_HandPtr hp)
{
    AsnTypePtr	atp;
    AsnIoPtr	aip;
    DataVal	value;
    CharPtr		process, sp;
    Char		buf[IOBUFSIZ];
    
    aip = hp->READ_AIP;
    atp = ECHO_STRING;
    
    if ((atp = AsnReadId(aip, amp, atp)) != ECHO_STRING) {
	if (atp != NULL)
	    AsnReadVal(aip, atp, &value);
	return NULL;
    }
    AsnReadVal(aip, atp, &value);
    
    if ((atp = AsnReadId(aip, amp, atp)) != ECHO_STRING_process) {
	AsnReadVal(aip, atp, &value);
	return NULL;
    }
    AsnReadVal(aip, atp, &value);
    process = value.ptrvalue;
    
    if ((atp = AsnReadId(aip, amp, atp)) != ECHO_STRING_text) {
	AsnReadVal(aip, atp, &value);
	return NULL;
    }
    AsnReadVal(aip, atp, &value);
    
    sprintf(buf, "\tProcess: %s\n\tValue:   %s", process, value.ptrvalue);
    MemFree(process);
    MemFree(value.ptrvalue);
    
    if ((atp = AsnReadId(aip, amp, atp)) != ECHO_STRING) {
	AsnReadVal(aip, atp, &value);
	return NULL;
    }
    AsnReadVal(aip, atp, &value);
    
    sp = MemNew(StringLen(buf)+1);
    StringCpy(sp, buf);
    return sp;
} /* readServer*/


static Int2 writeServer(NI_HandPtr hp, CharPtr buf)
{
    AsnIoPtr	aip;
    DataVal	value;
    
    aip = hp->WRITE_AIP;
    AsnStartStruct(aip, ECHO_STRING);
    value.ptrvalue = "client";
    AsnWrite(aip, ECHO_STRING_process, &value);
    value.ptrvalue = buf;
    AsnWrite(aip, ECHO_STRING_text, &value);
    AsnEndStruct(aip, ECHO_STRING);
    AsnIoFlush(aip);
    return 0;
} /* writeServer*/
