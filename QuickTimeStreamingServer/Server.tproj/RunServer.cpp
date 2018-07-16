/*
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * Copyright (c) 1999-2003 Apple Computer, Inc.  All Rights Reserved.
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 *
 */
/*
    File:       main.cpp

    Contains:   main function to drive streaming server.

    

*/

#include <errno.h>

#include "RunServer.h"
#include "SafeStdLib.h"
#include "OS.h"
#include "OSMemory.h"
#include "OSThread.h"
#include "Socket.h"
#include "SocketUtils.h"
#include "ev.h"
#include "OSArrayObjectDeleter.h"
#include "Task.h"
#include "IdleTask.h"
#include "TimeoutTask.h"
#ifndef __Win32__
    #include <sys/types.h>
    #include <unistd.h>
#endif
#include "QTSServerInterface.h"
#include "QTSServer.h"

#include <stdlib.h>
#include <sys/stat.h>

QTSServer* sServer = NULL;
int sStatusUpdateInterval = 0;
Bool16 sHasPID = false;

#ifdef __sgi__ 
#include <sched.h>
#endif

QTSS_ServerState StartServer(XMLPrefsParser* inPrefsSource, PrefsSource* inMessagesSource, UInt16 inPortOverride, int statsUpdateInterval, QTSS_ServerState inInitialState, Bool16 inDontFork)
{
    //Mark when we are done starting up. If auto-restart is enabled, we want to make sure
    //to always exit with a status of 0 if we encountered a problem WHILE STARTING UP. This
    //will prevent infinite-auto-restart-loop type problems
    Bool16 doneStartingUp = false;
    
    sStatusUpdateInterval = statsUpdateInterval;
    
    //Initialize utility classes
    OS::Initialize();
    OSThread::Initialize();
    Socket::Initialize();
    SocketUtils::Initialize(!inDontFork);

#if !MACOSXEVENTQUEUE
    ::select_startevents();//initialize the select() implementation of the event queue
#endif
    
    //start the server
    QTSSDictionaryMap::Initialize();
    QTSServerInterface::Initialize();// this must be called before constructing the server object
    sServer = NEW QTSServer();

	// re-parse config file
	inPrefsSource->Parse();


    sServer->Initialize(inPrefsSource, inMessagesSource, inPortOverride);

    if (inInitialState == qtssShuttingDownState)
    {  
        sServer->InitModules(inInitialState);
        return inInitialState;
    }
    
    if (sServer->GetServerState() != qtssFatalErrorState)
    {
        UInt32 numThreads = sServer->GetPrefs()->GetNumThreads();
        if (numThreads == 0)
            numThreads = OS::GetNumProcessors();

        TaskThreadPool::AddThreads(numThreads);
        
    #if DEBUG
        qtss_printf("Number of task threads: %lu\n",numThreads);
    #endif
        // Start up the server's global tasks, and start listening
        TimeoutTask::Initialize();  // The TimeoutTask mechanism is task based,
                                    // we therefore must do this after adding task threads
                                    // this be done before starting the sockets and server tasks
    }


    //Make sure to do this stuff last. Because these are all the threads that
    //do work in the server, this ensures that no work can go on while the server
    //is in the process of staring up
    if (sServer->GetServerState() != qtssFatalErrorState)
    {
        IdleTask::Initialize();
        Socket::StartThread();
        OSThread::Sleep(1000);
    
        //
        // On Win32, in order to call modwatch the Socket EventQueue thread must be
        // created first. Modules call modwatch from their initializer, and we don't
        // want to prevent them from doing that, so module initialization is separated
        // out from other initialization, and we start the Socket EventQueue thread first.
        // The server is still prevented from doing anything as of yet, because there
        // aren't any TaskThreads yet.
        sServer->InitModules(inInitialState);
        sServer->StartTasks();
        sServer->SetupUDPSockets(); // udp sockets are set up after the rtcp task is instantiated
    }

    QTSS_ServerState theServerState = sServer->GetServerState();
    if (theServerState != qtssFatalErrorState)
    {
        WritePid(!inDontFork);

        doneStartingUp = true;
        qtss_printf("Streaming Server done starting up\n");
        OSMemory::SetMemoryError(ENOMEM);
    }

    // SWITCH TO RUN USER AND GROUP ID
    if (!sServer->SwitchPersonality())
        theServerState = qtssFatalErrorState;
    
    //
    // Tell the caller whether the server started up or not
    return theServerState;
}

void WritePid(Bool16 forked)
{
#ifndef __Win32__
	// WRITE PID TO FILE
	OSCharArrayDeleter thePidFileName(sServer->GetPrefs()->GetPidFilePath());
	FILE *thePidFile = fopen(thePidFileName, "w");
	if(thePidFile)
	{
		if (!forked)
			fprintf(thePidFile,"%d\n",getpid());	// write own pid
		else
		{
			fprintf(thePidFile,"%d\n",getppid());	// write parent pid
			fprintf(thePidFile,"%d\n",getpid());	// and our own pid in the next line
		}				
		fclose(thePidFile);
		sHasPID = true;
	}
#endif
}

void CleanPid()
{
#ifndef __Win32__
    if (sHasPID)
    {
	    OSCharArrayDeleter thePidFileName(sServer->GetPrefs()->GetPidFilePath());
	    unlink(thePidFileName);
	}
#endif
}

void LogStatus(QTSS_ServerState theServerState)
{
    static QTSS_ServerState lastServerState = 0;
	static char *sPLISTHeader[] =
	{ 	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>",
#if __MacOSX__
		"<!DOCTYPE plist SYSTEM \"file://localhost/System/Library/DTDs/PropertyList.dtd\">",
#else
		"<!ENTITY % plistObject \"(array | data | date | dict | real | integer | string | true | false )\">",
		"<!ELEMENT plist %plistObject;>",
		"<!ATTLIST plist version CDATA \"0.9\">",
		"",
		"<!-- Collections -->",
		"<!ELEMENT array (%plistObject;)*>",
		"<!ELEMENT dict (key, %plistObject;)*>",
		"<!ELEMENT key (#PCDATA)>",
		"",
		"<!--- Primitive types -->",
		"<!ELEMENT string (#PCDATA)>",
        "<!ELEMENT data (#PCDATA)> <!-- Contents interpreted as Base-64 encoded -->",
        "<!ELEMENT date (#PCDATA)> <!-- Contents should conform to a subset of ISO 8601 (in particular, YYYY '-' MM '-' DD 'T' HH ':' MM ':' SS 'Z'.  Smaller units may be omitted with a loss of precision) -->",
        "",
        "<!-- Numerical primitives -->",
        "<!ELEMENT true EMPTY>  <!-- Boolean constant true -->",
        "<!ELEMENT false EMPTY> <!-- Boolean constant false -->",
        "<!ELEMENT real (#PCDATA)> <!-- Contents should represent a floating point number matching (\"+\" | \"-\")? d+ (\".\"d*)? (\"E\" (\"+\" | \"-\") d+)? where d is a digit 0-9.  -->",
        "<!ELEMENT integer (#PCDATA)> <!-- Contents should represent a (possibly signed) integer number in base 10 -->",
		"]>",
#endif
	};

	static int numHeaderLines = sizeof(sPLISTHeader) / sizeof(char*);

    static char*    sPlistStart = "<plist version=\"0.9\">";
    static char*    sPlistEnd = "</plist>";
    static char*    sDictStart = "<dict>";
    static char*    sDictEnd = "</dict>";
    
	static char*	sKey    = "     <key>%s</key>\n";
	static char*	sValue  = "     <string>%s</string>\n";
	
	static char *sAttributes[] =
	{
	    "qtssSvrServerName",
	    "qtssSvrServerVersion",
	    "qtssSvrServerBuild",
	    "qtssSvrServerPlatform",
	    "qtssSvrRTSPServerComment",
	    "qtssSvrServerBuildDate",
	    "qtssSvrStartupTime",
		"qtssSvrCurrentTimeMilliseconds",
		"qtssSvrCPULoadPercent",
	 	"qtssSvrState",
		"qtssRTPSvrCurConn",
		"qtssRTSPCurrentSessionCount",
		"qtssRTSPHTTPCurrentSessionCount",
		"qtssRTPSvrCurBandwidth",
		"qtssRTPSvrCurPackets",
		"qtssRTPSvrTotalConn",
		"qtssRTPSvrTotalBytes",
		"qtssMP3SvrCurConn",
		"qtssMP3SvrTotalConn",
		"qtssMP3SvrCurBandwidth",
		"qtssMP3SvrTotalBytes"
	};
	static int numAttributes = sizeof(sAttributes) / sizeof(char*);
		
	static StrPtrLen statsFileNameStr("server_status");    
	
    if (false == sServer->GetPrefs()->ServerStatFileEnabled())
        return;
        
    UInt32 interval = sServer->GetPrefs()->GetStatFileIntervalSec();
    if (interval == 0 || (OS::UnixTime_Secs() % interval) > 0 ) 
        return;
    
    // If the total number of RTSP sessions is 0  then we 
    // might not need to update the "server_status" file.
    char* thePrefStr = NULL;
    // We start lastRTSPSessionCount off with an impossible value so that
    // we force the "server_status" file to be written at least once.
	static int lastRTSPSessionCount = -1; 
    // Get the RTSP session count from the server.
    (void)QTSS_GetValueAsString(sServer, qtssRTSPCurrentSessionCount, 0, &thePrefStr);
    int currentRTSPSessionCount = ::atoi(thePrefStr);
    delete [] thePrefStr; thePrefStr = NULL;
    if (currentRTSPSessionCount == 0 && currentRTSPSessionCount == lastRTSPSessionCount)
    {
        // we don't need to update the "server_status" file except the
        // first time we are in the idle state.
        if (theServerState == qtssIdleState && lastServerState == qtssIdleState)
        {
            lastRTSPSessionCount = currentRTSPSessionCount;
            lastServerState = theServerState;
            return;
        }
    }
    else
    {
        // save the RTSP session count for the next time we execute.
        lastRTSPSessionCount = currentRTSPSessionCount;
    }

    StrPtrLenDel pathStr(sServer->GetPrefs()->GetErrorLogDir());
    StrPtrLenDel fileNameStr(sServer->GetPrefs()->GetStatsMonitorFileName());
    ResizeableStringFormatter pathBuffer(NULL,0);
    pathBuffer.PutFilePath(&pathStr,&fileNameStr);
    pathBuffer.PutTerminator();
    
	char*   filePath = pathBuffer.GetBufPtr();	
    FILE*   statusFile = ::fopen(filePath, "w");
    char*   theAttributeValue = NULL;
    int     i;
    
    if (statusFile != NULL)
    {
        ::chmod(filePath, 0640);
		for ( i = 0; i < numHeaderLines; i++)
		{	
			qtss_fprintf(statusFile, "%s\n",sPLISTHeader[i]);	
		}

		qtss_fprintf(statusFile, "%s\n", sPlistStart);
		qtss_fprintf(statusFile, "%s\n", sDictStart);	

  		// show each element value
 		for ( i = 0; i < numAttributes; i++)
		{
			(void)QTSS_GetValueAsString(sServer, QTSSModuleUtils::GetAttrID(sServer,sAttributes[i]), 0, &theAttributeValue);
			if (theAttributeValue != NULL)
 			{
 			   qtss_fprintf(statusFile, sKey, sAttributes[i]);	
			   qtss_fprintf(statusFile, sValue, theAttributeValue);	
 			   delete [] theAttributeValue;
 			   theAttributeValue = NULL;
 			}
 		}
 		 		
		qtss_fprintf(statusFile, "%s\n", sDictEnd);
		qtss_fprintf(statusFile, "%s\n\n", sPlistEnd);	
 		
  		::fclose(statusFile);
	}
    lastServerState = theServerState;
}


void RunServer()
{   
	Bool16 restartServer = false;

    //just wait until someone stops the server or a fatal error occurs.
    QTSS_ServerState theServerState = sServer->GetServerState();
    while ((theServerState != qtssShuttingDownState) &&
            (theServerState != qtssFatalErrorState))
    {
#ifdef __sgi__
		OSThread::Sleep(999);
#else
		OSThread::Sleep(1000);
#endif

        LogStatus(theServerState);

        if (sStatusUpdateInterval)
        {
            static int loopCount = 0;
    
            if ( (loopCount % sStatusUpdateInterval) == 0)      //every 10 times through the loop
            {       
                char* thePrefStr = NULL;
                UInt32 theLen = 0;

        
                if ( (loopCount % (sStatusUpdateInterval*10)) == 0 )
                    qtss_printf("  RTP-Conns RTSP-Conns HTTP-Conns  kBits/Sec   Pkts/Sec    TotConn   TotBytes   TotPktsLost\n");


                (void)QTSS_GetValueAsString(sServer, qtssRTPSvrCurConn, 0, &thePrefStr);
                qtss_printf( "%11s", thePrefStr);
                delete [] thePrefStr; thePrefStr = NULL;
                
                (void)QTSS_GetValueAsString(sServer, qtssRTSPCurrentSessionCount, 0, &thePrefStr);
                qtss_printf( "%11s", thePrefStr);
                delete [] thePrefStr; thePrefStr = NULL;
                
                (void)QTSS_GetValueAsString(sServer, qtssRTSPHTTPCurrentSessionCount, 0, &thePrefStr);
                qtss_printf( "%11s", thePrefStr);
                delete [] thePrefStr; thePrefStr = NULL;
                
                UInt32 curBandwidth = 0;
                theLen = sizeof(curBandwidth);
                (void)QTSS_GetValue(sServer, qtssRTPSvrCurBandwidth, 0, &curBandwidth, &theLen);
                qtss_printf( "%11lu", curBandwidth/1024);

                (void)QTSS_GetValueAsString(sServer, qtssRTPSvrCurPackets, 0, &thePrefStr);
                qtss_printf( "%11s", thePrefStr);
                delete [] thePrefStr; thePrefStr = NULL;

                (void)QTSS_GetValueAsString(sServer, qtssRTPSvrTotalConn, 0, &thePrefStr);
                qtss_printf( "%11s", thePrefStr);
                delete [] thePrefStr; thePrefStr = NULL;

                (void)QTSS_GetValueAsString(sServer, qtssRTPSvrTotalBytes, 0, &thePrefStr);
                qtss_printf( "%11s", thePrefStr);
                delete [] thePrefStr; thePrefStr = NULL;

                qtss_printf( "%11"_64BITARG_"u", sServer->GetTotalRTPPacketsLost());
                
                
                qtss_printf( "\n");
            }
            
            loopCount++;

        }
        
        if ((sServer->SigIntSet()) || (sServer->SigTermSet()))
        {
            //
            // start the shutdown process
            theServerState = qtssShuttingDownState;
            (void)QTSS_SetValue(QTSServerInterface::GetServer(), qtssSvrState, 0, &theServerState, sizeof(theServerState));

			if (sServer->SigIntSet())
				restartServer = true;
		}
        
        theServerState = sServer->GetServerState();
        if (theServerState == qtssIdleState)
            sServer->KillAllRTPSessions();
    }
    
    //
    // Kill all the sessions and wait for them to die,
    // but don't wait more than 5 seconds
    sServer->KillAllRTPSessions();
    for (UInt32 shutdownWaitCount = 0; (sServer->GetNumRTPSessions() > 0) && (shutdownWaitCount < 5); shutdownWaitCount++)
        OSThread::Sleep(1000);
        
    //Now, make sure that the server can't do any work
    TaskThreadPool::RemoveThreads();
    
    //now that the server is definitely stopped, it is safe to initate
    //the shutdown process
    delete sServer;
    
    CleanPid();
    //ok, we're ready to exit. If we're quitting because of some fatal error
    //while running the server, make sure to let the parent process know by
    //exiting with a nonzero status. Otherwise, exit with a 0 status
    if (theServerState == qtssFatalErrorState || restartServer)
        ::exit (-2);//-2 signals parent process to restart server
}
