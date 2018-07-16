/*
 *
 * @APPLE_LICENSE_HEADER_START@
 *
 * Copyright (c) 1999-2001 Apple Computer, Inc.  All Rights Reserved. The
 * contents of this file constitute Original Code as defined in and are
 * subject to the Apple Public Source License Version 1.2 (the 'License').
 * You may not use this file except in compliance with the License.  Please
 * obtain a copy of the License at http://www.apple.com/publicsource and
 * read it before using this file.
 *
 * This Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.  Please
 * see the License for the specific language governing rights and
 * limitations under the License.
 *
 *
 * @APPLE_LICENSE_HEADER_END@
 *
 */
/*
	File:		SpamPro.cpp

	Contains:	.  
					
	$Log: SpamPro.cpp,v $
	Revision 1.26  2002/06/19 21:38:49  murata
	change comment.
	
	Revision 1.25  2002/06/18 22:07:54  murata
	QuickFix for hardcoded trackID to allow user to replace with another string.
	
	Revision 1.24  2002/03/23 15:05:40  murata
	remove MacOS support
	
	Revision 1.23  2002/03/13 19:23:10  murata
	*** empty log message ***
	
	Revision 1.22  2002/02/22 10:51:05  murata
	fix spam so it checks for streamigloadtool.mov using the correct address.
	
	Revision 1.21  2002/02/07 23:49:46  sussery
	change for new ProjectBuilder in Jaguar
	
	Revision 1.20  2001/10/18 23:28:22  lecroy
	optarg fix
	
	Revision 1.19  2001/10/09 12:01:47  murata
	fix various crashes.
	
	Revision 1.16  2001/09/13 23:06:34  lecroy
	Modified tool to skip over bad IP addresses or sites lacking a"streamingloadtool.mov" rather than quitting the process
	
	Revision 1.15  2001/05/03 16:51:05  serenyi
	turned on optimization, fixed a use of "SpamPro"
	
	Revision 1.14  2001/03/13 22:28:43  murata
	*** empty log message ***
	
	Revision 1.13  2001/03/13 22:11:48  murata
	New copyright notice.
	
	Revision 1.12  2001/03/09 14:23:17  murata
	Teardown session after checking for a the streamingloadtool.mov
	
	Revision 1.11  2001/03/08 01:27:15  serenyi
	Changed references from SpamPro to StreamingLoadTool
	
	Revision 1.9  2000/12/20 02:38:46  serenyi
	Use spampro.cfg on win32
	
	Revision 1.8  2000/12/19 06:18:19  serenyi
	Fixed line ending bug in spampro.conf, made SpamPro.cpp work on solaris
	
	Revision 1.7  2000/12/08 02:17:10  serenyi
	Added new option "randomthumb"
	
	Revision 1.6  2000/11/29 04:33:08  serenyi
	added new config options
	
	Revision 1.3  2000/10/04 22:16:02  serenyi
	Check for 'spampro.mov'
	
	Revision 1.2  2000/09/19 08:31:32  serenyi
	Added caching protocol implementation
	
	Revision 1.1.1.1  2000/08/31 00:30:51  serenyi
	Mothra Repository
	
	Revision 1.2  2000/04/01 02:44:57  serenyi
	Added proxy functionality to spampro
	
	Revision 1.1  2000/03/04 01:44:14  serenyi
	Created
	
	
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

#ifndef kVersionString
#include "revision.h"
#endif
#ifndef __Win32__
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#endif

#include "ClientSession.h"
#include "FilePrefsSource.h"
#include "OSMemory.h"
#include "SocketUtils.h"
#include "StringFormatter.h"
#include "Socket.h"
#include "OS.h"
#include "Task.h"
#include "TimeoutTask.h"
#ifndef __MacOSX__
#include "getopt.h"
#include "revision.h"
#endif

#define STREAMINGLOADTOOL_DEBUG 0

//
// Static data
static UInt32	sConnectionsThatErrored = 0;
static UInt32	sFailedConnections = 0;
static UInt32	sSuccessfulConnections = 0;
static FILE*	sLog = NULL;

static ClientSession** sClientSessionArray = NULL;
static UInt32 sNumClients = 1;
static Bool16 sGotSigInt = false;
static Bool16 sQuitNow = false;
static SInt64 sSigIntTime = 0;
static Bool16 sVerbose = false;

int main(int argc, char *argv[]);

//
// Helper functions
char* 	GetClientTypeDescription(ClientSession::ClientType inClientType);
void	DoDNSLookup(FilePrefsSource* inFileParser, UInt32* inURLIndexes, UInt32* ioIPAddrs, UInt32 inNumURLs);
void 	RecordClientInfoBeforeDeath(ClientSession* inSession);
char*	GetDeathReasonDescription(UInt32 inDeathReason);
char*	GetPayloadDescription(QTSS_RTPPayloadType inPayload);
void	CheckForStreamingLoadToolDotMov(UInt32* inIPAddrArray, UInt32 inNumURLs, UInt16 inPort);
UInt32 CalcStartTime(Bool16 inRandomThumb, UInt32 inMovieLength);
extern char* optarg;



#ifndef __Win32__
void sigcatcher(int sig, int /*sinfo*/, struct sigcontext* /*sctxt*/);

void sigcatcher(int sig, int /*sinfo*/, struct sigcontext* /*sctxt*/)
{
	//printf("sigcatcher =%d\n", sig);

	if ((sig == SIGINT) || (sig == SIGTERM))
	{       // TheTime check and sQuitNow flag tests are needed test with Linux and OSX. 
		if (sGotSigInt && (OS::Milliseconds() > sSigIntTime + 500) )
		{	if (!sQuitNow)// print only once
				printf("Force quitting\n");
			sQuitNow = true;
		}
		else
		{
			sSigIntTime = OS::Milliseconds();
			sGotSigInt = true;
		}
	}
}
#endif

int main(int argc, char *argv[])
{
#ifndef __Win32__
	struct sigaction act;
	
#if defined(sun) || defined(i386)
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_handler = (void(*)(int))&sigcatcher;
#else
	act.sa_mask = 0;
	act.sa_flags = 0;
	act.sa_handler = (void(*)(...))&sigcatcher;
#endif
	(void)::sigaction(SIGPIPE, &act, NULL);
	(void)::sigaction(SIGINT, &act, NULL);
	(void)::sigaction(SIGTERM, &act, NULL);

#if __MacOSX__ || __solaris__
	//grow our pool of file descriptors to the max!
	struct rlimit rl;
               
	rl.rlim_cur = Socket::kMaxNumSockets;
	rl.rlim_max = Socket::kMaxNumSockets;

	setrlimit (RLIMIT_NOFILE, &rl);
#endif
#else
	//
	// Start Win32 DLLs
	WORD wsVersion = MAKEWORD(1, 1);
	WSADATA wsData;
	(void)::WSAStartup(wsVersion, &wsData);
#endif

#ifdef __Win32__
	char* configFilePath = "streamingloadtool.cfg";
#else
	char* configFilePath = "streamingloadtool.conf";
#endif
	Bool16 dropPost = false;
	ClientSession::ClientType theClientType = ClientSession::kRTSPUDPClientType;
	UInt16 thePort = 554;
	UInt32 theMovieLength = 60;
	Bool16 runForever = false;
	UInt32 theHTTPCookie = 1;
	Bool16 shouldLog = false;
	char* logPath = "spam.log";
	UInt32 proxyIP = 0;
	Bool16 appendJunk = false;
	UInt32 theReadInterval = 50;
	UInt32 sockRcvBuf = 32768;
	Float32 lateTolerance = 0;
	char* rtpMetaInfo = NULL;
	Float32 speed = 1;
	Bool16 verbose = false;
	char* packetPlayHeader = NULL;
    UInt32 overbufferwindowInK = 0;
    Bool16 randomThumb = false;
	//
	// Set up our User Agent string for the RTSP client
	char theUserAgentStr[128];
	::sprintf(theUserAgentStr, "StreamingLoadTool-%s",kVersionString);
	RTSPClient::SetUserAgentStr(theUserAgentStr);
	
	UInt32	theURLIndexArraySize = 8;
	UInt32* theURLIndexArray = NEW UInt32[theURLIndexArraySize];
	UInt32 theNumURLs = 0;
    char* controlID = NULL;
	char			ch = 0;
	//
	// Read our command line options
	while( (ch = getopt(argc, argv, "vf:c:i:d")) != -1 )
	{
		switch( ch )
		{
			case 'v':
				printf("StreamingLoadTool %s, built on %s, %s.\n", kVersionString, __DATE__  , __TIME__);
				printf("Usage: StreamingLoadTool [-i urlid] [-f path] [-c #]\n");
				printf("-f: Config file to use. Defaults to streamingloadtool.conf\n");
				printf("-c: HTTP cookie to use. Overrides what is in config file\n");
				printf("-d: Debug messages\n");
				printf("-i: RTSP stream URL id (i.e. trackID or streamID etc.). Default is -i trackID\n");
				exit(0);
				break;
			case 'f':
				configFilePath = optarg;
				break;
		    case 'i':
		        controlID = optarg;
		        break;
			case 'c':
				theHTTPCookie = atoi(optarg);
				break;
			case 'd':
				verbose = true;
				sVerbose = true;
				break;
		}
	}
	
	//
	// Get settings from the file
	FilePrefsSource theFileParser(true);
	if (theFileParser.InitFromConfigFile(configFilePath))
	{
		printf("Couldn't find StreamingLoadTool config file at: %s\n", configFilePath);
		::exit(-1);
	}
	
	
	for (UInt32 x = 0; true; x++)
	{
		char* theValue = theFileParser.GetValueAtIndex(x);
		char* theKey = theFileParser.GetKeyAtIndex(x);

		if (theKey == NULL)
			break;
		
		if (theValue != NULL)
		{
			if (::strcmp("clienttype", theKey) == 0)
			{
				if (::strcmp("http", theValue) == 0)
					theClientType = ClientSession::kRTSPHTTPClientType;
				else if (::strcmp("reliableudp", theValue) == 0)
					theClientType = ClientSession::kRTSPReliableUDPClientType;
				else if (::strcmp("tcp", theValue) == 0)
					theClientType = ClientSession::kRTSPTCPClientType;
				else
					theClientType = ClientSession::kRTSPUDPClientType;
			}
			else if (::strcmp("droppost", theKey) == 0)
			{
				if (::strcmp("yes", theValue) == 0)
					dropPost = true;
			}
			else if (::strcmp("concurrentclients", theKey) == 0)
			{
				::sscanf(theValue, "%lu", &sNumClients);
			}
			else if (::strcmp("port", theKey) == 0)
			{
				UInt32 tempPort = 0;
				::sscanf(theValue, "%lu", &tempPort);
				thePort = (UInt16)tempPort;
			}
			else if (::strcmp("movielength", theKey) == 0)
			{
				::sscanf(theValue, "%lu", &theMovieLength);
			}
			else if (::strcmp("runforever", theKey) == 0)
			{
				if (::strcmp("yes", theValue) == 0)
					runForever = true;
			}
			else if (::strcmp("shouldlog", theKey) == 0)
			{
				if (::strcmp("yes", theValue) == 0)
					shouldLog = true;
			}
			else if (::strcmp("appendjunk", theKey) == 0)
			{
				if (::strcmp("yes", theValue) == 0)
					appendJunk = true;
			}
			else if (::strcmp("logpath", theKey) == 0)
			{
				logPath = theValue;
			}
			else if (::strcmp("proxyip", theKey) == 0)
			{
				proxyIP = SocketUtils::ConvertStringToAddr(theValue);
			}
			else if (::strcmp("clientwindow", theKey) == 0)
			{
				::sscanf(theValue, "%lu", &sockRcvBuf);
			}
			else if (::strcmp("httpcookie", theKey) == 0)
			{
				if (theHTTPCookie == 1)
				{
					// Ignore if set by command line
					::sscanf(theValue, "%lu", &theHTTPCookie);
					theHTTPCookie *= 1000000;
				}
			}
			else if (::strcmp("readinterval", theKey) == 0)
			{
				::sscanf(theValue, "%lu", &theReadInterval);
			}
			else if (::strcmp("latetolerance", theKey) == 0)
			{
				::sscanf(theValue, "%f", &lateTolerance);
			}
			else if (::strcmp("rtpmetainfo", theKey) == 0)
			{
				if (::strlen(theValue) > 0)
					rtpMetaInfo = theValue;				
			}
			else if (::strcmp("speed", theKey) == 0)
			{
				::sscanf(theValue, "%f", &speed);
			}
			else if (::strcmp("packetplayheader", theKey) == 0)
			{
				if (::strlen(theValue) > 0)
					packetPlayHeader = theValue;
			}
            else if (::strcmp("overbufferwindowsize", theKey) == 0)
            {
                ::sscanf(theValue, "%lu", &overbufferwindowInK);
            }
			else if (::strcmp("randomthumb", theKey) == 0)
			{
				if (::strcmp("yes", theValue) == 0)
					randomThumb = true;
			}
			else if (::strcmp("url", theKey) == 0)
			{
				if (theNumURLs == theURLIndexArraySize)
				{
					UInt32* tempArray = NEW UInt32[theURLIndexArraySize * 2];
					::memcpy(tempArray, theURLIndexArray, sizeof(UInt32) * theURLIndexArraySize);
					theURLIndexArraySize *= 2;
					delete [] theURLIndexArray;
					theURLIndexArray = tempArray;
				}
				theURLIndexArray[theNumURLs] = x;
				theNumURLs++;
			}
			else
				printf("Found bad directive in StreamingLoadTool config file: %s\n", theKey);
		}
	}
	
	if (theNumURLs == 0)
	{
		printf("Didn't find any URLs in StreamingLoadTool config file.\n");
		exit(-1);
	}
	
	//
	// Figure out what type of clients we are going to run
	if ((theClientType == ClientSession::kRTSPHTTPClientType) && dropPost)
		theClientType = ClientSession::kRTSPHTTPDropPostClientType;
		
	// Do IP lookups on all the URLs
	UInt32* theIPAddrArray = NEW UInt32[theNumURLs + 1];
	
	if (proxyIP == 0)
	{	::DoDNSLookup(&theFileParser, theURLIndexArray, theIPAddrArray, theNumURLs);
	    ::CheckForStreamingLoadToolDotMov(theIPAddrArray, theNumURLs, thePort); // do this on the target not the proxy
	}
	else
	{
	    //
	    // Check for "StreamingLoadTool.mov" on the target machines
		::DoDNSLookup(&theFileParser, theURLIndexArray, theIPAddrArray, theNumURLs);
	    ::CheckForStreamingLoadToolDotMov(theIPAddrArray, theNumURLs, thePort); // do this on the target not the proxy

		//
		// If user specified a proxy to connect through, use that IP address,
		// not the IP address in the URL.
		for (UInt32 ipAddrCounter = 0; ipAddrCounter < theNumURLs; ipAddrCounter++)
			theIPAddrArray[ipAddrCounter] = proxyIP;
	}
	
	//
	// Check for "StreamingLoadTool.mov" on the target machines
    //	::CheckForStreamingLoadToolDotMov(theIPAddrArray, theNumURLs, thePort); // checking for movies on wrong ip if proxy
	
	// Open the log if we are logging
	if (shouldLog)
		sLog = ::fopen(logPath, "w");
	
	//
	// Final setup before running the show
	OS::Initialize();
	OSThread::Initialize();
	OSMemory::SetMemoryError(ENOMEM);
	Socket::Initialize();
	SocketUtils::Initialize();

#if !MACOSXEVENTQUEUE
	::select_startevents();//initialize the select() implementation of the event queue
#endif
	
	TaskThreadPool::AddThreads(OS::GetNumProcessors());
	TimeoutTask::Initialize();
	Socket::StartThread();

	//
	// Create the array of sessions
	if ((sNumClients > theNumURLs) && (!runForever))
		sNumClients = theNumURLs;
		
	sClientSessionArray = NEW ClientSession*[sNumClients];
	
	UInt32 theCurURLIndex = 0;
	for (UInt32 clientCount = 0; clientCount < sNumClients; clientCount++, theCurURLIndex++)
	{
		if (theCurURLIndex == theNumURLs)
			theCurURLIndex = 0;
			
		if (theIPAddrArray[theCurURLIndex] == 0)
			continue;  //skip over non-spammable IP addresses
			
		sClientSessionArray[clientCount] = NEW ClientSession( theIPAddrArray[theCurURLIndex],
																thePort,					
																theFileParser.GetValueAtIndex(theURLIndexArray[theCurURLIndex]),
																theClientType,		// Client type
																theMovieLength,		// Movie length
																CalcStartTime(randomThumb, theMovieLength),		// Movie start time
																5,					// RTCP interval
																0,					// Options interval
																theHTTPCookie++,	// HTTP cookie
																appendJunk,
																theReadInterval,	// Interval between data reads	
																sockRcvBuf,			// socket recv buffer size
																lateTolerance,		// late tolerance
																rtpMetaInfo,
																speed,
																verbose,
																packetPlayHeader,
																overbufferwindowInK);	
																
	     if (sClientSessionArray[clientCount])
	        sClientSessionArray[clientCount]->GetClient()->SetControlID(controlID);

#if STREAMINGLOADTOOL_DEBUG
		printf("Creating a ClientSession for URL: %s\n", theFileParser.GetValueAtIndex(theURLIndexArray[theCurURLIndex]));
#endif
	}
	
	//
	// Now, all we have to do is loop, destroying and re-creating ClientSession objects when they die.
	// Occassionally, lets print out status to show what is currently going on
	Bool16 isStillActive = false;
	UInt32 loopCount = 0;
	do
	{
		if (sGotSigInt)
			break;

		OSThread::Sleep(1000);
		
		isStillActive = false;
		for (UInt32 y = 0; y < sNumClients; y++)
		{
			if (sClientSessionArray == NULL)
				continue; //skip over NULL client sessions
		
			if (sClientSessionArray[y] != NULL && sClientSessionArray[y]->IsDone())
			{
				// If we are looping forever, when we run out of URLs, go back to the beginning of the list
				if ((runForever) && (theCurURLIndex == theNumURLs))
						theCurURLIndex = 0;
						
				if (theCurURLIndex < theNumURLs)
				{
					isStillActive = true;
					// If there is a new URL to fetch, kill this client and restart
					::RecordClientInfoBeforeDeath(sClientSessionArray[y]);
					sClientSessionArray[y]->Signal(Task::kKillEvent); // Tell it to destroy itself
					
					sClientSessionArray[y] = NEW ClientSession(	theIPAddrArray[theCurURLIndex],	// IP addr
                                                                                        thePort,			// IP port
                                                                                        theFileParser.GetValueAtIndex(theURLIndexArray[theCurURLIndex]),
                                                                                        theClientType,		// Client type
                                                                                        theMovieLength,		// Movie length
                                                                                        CalcStartTime(randomThumb, theMovieLength),		// Movie start time
                                                                                        5,					// RTCP interval
                                                                                        0,					// Options interval
                                                                                        theHTTPCookie++,	// HTTP cookie
                                                                                        appendJunk,
                                                                                        theReadInterval,	// Interval between data reads	
                                                                                        sockRcvBuf,			// socket recv buffer size	
                                                                                        lateTolerance,		// late tolerance
                                                                                        rtpMetaInfo,
                                                                                        speed,
                                                                                        verbose,
                                                                                        packetPlayHeader,
                                                                                        overbufferwindowInK);	

#if STREAMINGLOADTOOL_DEBUG
					printf("Creating a ClientSession for URL: %s\n", theFileParser.GetValueAtIndex(theURLIndexArray[theCurURLIndex]));
#endif
					theCurURLIndex++;
				}
			}
			else
					isStillActive = true;
		}
		
		if ((loopCount & 7) == 0) // Fancy way of mod'ing by 8, so the following will execute every 8 seconds
		{
			if ((loopCount & 127) == 0) // Fancy way of dividing by 127,
			{
				// Occassionally give the user lots of info
				printf("StreamingLoadTool test in progress.\n");
				printf("Config file: %s. Client type: %s. Num clients: %lu.\n",
								configFilePath, GetClientTypeDescription(theClientType), sNumClients);
				printf("Movie length: %lu. Run forever: %d. HTTP cookie: %lu. Port: %d\n",
								theMovieLength, runForever, theHTTPCookie, thePort);
				if (shouldLog)
					printf("Writing StreamingLoadTool log at: %s\n", logPath);
				printf("Active Playing Attempts Success Errors Failed\n");
			}
			
			printf("%5lu %6lu %8lu %6lu %6lu %6lu\n",
				ClientSession:: GetActiveConnections (),
				ClientSession:: GetPlayingConnections (),
				ClientSession:: GetConnectionAttempts (),
				sSuccessfulConnections,
				sConnectionsThatErrored,
				sFailedConnections);
		}
		loopCount++;
		
	} while (isStillActive == true);

	if (sGotSigInt)
	{
		//
		// If someone hits Ctrl-C, force all the clients to wrap it up
		printf("Sending TEARDOWNs.\n");
		
		//
		// Tell all the clients to wrap it up.
		for (UInt32 clientCount = 0; clientCount < sNumClients; clientCount++)
		{
			if (sClientSessionArray == NULL)
				continue; //skip over NULL client sessions
			
			if (sClientSessionArray[clientCount] != NULL)	
				sClientSessionArray[clientCount]->Signal(ClientSession::kTeardownEvent);
		}
		
		//
		// Wait for the clients to complete
		Bool16 isDone = false;
		while (!isDone && !sQuitNow)
		{
			OSThread::Sleep(1000);
			isDone = true;
			for (UInt32 cc2 = 0; cc2 < sNumClients; cc2++)
			{
				if (sClientSessionArray == NULL)
					continue; //skip over NULL client sessions
			
				if (sClientSessionArray[cc2] == NULL)	
					continue;

				if (!sClientSessionArray[cc2]->IsDone())
					isDone = false;
			}
		}	
	}
	
	//
	// We're done... now go through and delete the last sessions
	for (UInt32 z = 0; z < sNumClients; z++)
	{
		if (sClientSessionArray == NULL)
			continue; //skip over NULL client sessions
				
		if (sClientSessionArray[z] == NULL)	
			continue;
	
		::RecordClientInfoBeforeDeath(sClientSessionArray[z]);
	}
	
	if (sLog != NULL)
		::fclose(sLog);
		
	printf("StreamingLoadTool test complete. Total number of connections: %lu.\n", ClientSession:: GetConnectionAttempts ());
}

UInt32 CalcStartTime(Bool16 inRandomThumb, UInt32 inMovieLength)
{
	UInt32 theMovieLength = inMovieLength;
	if (theMovieLength > 1)
		theMovieLength--;
		
	if (inRandomThumb)
		return ::rand() % theMovieLength;
	else
		return 0;
}

void CheckForStreamingLoadToolPermission(UInt32* inIPAddrArray, UInt32 inNumURLs, UInt16 inPort)
{
	//Eventually check for the existance of a specially formatted sdp file (assuming the server blindly returns sdps)
}

void CheckForStreamingLoadToolDotMov(UInt32* ioIPAddrArray, UInt32 inNumURLs, UInt16 inPort)
{
	printf("Checking for 'streamingloadtool.mov' on the target servers\n");

	UInt32* uniqueIPAddrs = NEW UInt32[inNumURLs + 1];
	::memset(uniqueIPAddrs, 0, sizeof(UInt32) * (inNumURLs + 1));
	

	for (UInt32 count = 0; count < inNumURLs; count++)
	{
		UInt32 badAddr = 0;
		Bool16 dup = false;
		for (UInt32 x = 0; uniqueIPAddrs[x] != 0; x++)
		{
			if (uniqueIPAddrs[x] == ioIPAddrArray[count])
			{
				dup = true;
				break;
			}
		}
		
		if (dup)
			continue;
		
		//
		// For tracking dups.
		uniqueIPAddrs[count] = ioIPAddrArray[count];

		//
		// Make an RTSP client. We'll send a DESCRIBE to the server to check for this sucker
		
		TCPClientSocket* theSocket = NEW TCPClientSocket(0); // blocking socket.
		RTSPClient* theClient = NEW RTSPClient(theSocket, sVerbose);
 		//
		// Format the URL: rtsp://xx.xx.xx.xx/streamingloadtool.mov
		char theAddrBuf[20];
		StrPtrLen theAddrBufPtr(theAddrBuf, 20);
		struct in_addr theAddr;
		theAddr.s_addr = htonl(ioIPAddrArray[count]);
		
		SocketUtils::ConvertAddrToString(theAddr, &theAddrBufPtr);
		
		char theURLBuf[100];
		StringFormatter theFormatter(theURLBuf, 100);
		
		theFormatter.Put("rtsp://");
		theFormatter.Put(theAddrBufPtr);
		theFormatter.Put("/streamingloadtool.mov");
		theFormatter.PutTerminator();
		
		StrPtrLen theURL(theFormatter.GetBufPtr(), theFormatter.GetCurrentOffset());
		
		//
		// tell the client this is the URL to use
		theSocket->Set(ioIPAddrArray[count], inPort);
		theClient->Set(theURL);
		
		//
		// Send the DESCRIBE! Whoo hoo!
		OS_Error theErr = theClient->SendDescribe();
		
		if (theErr != OS_NoErr)
		{
			printf("##WARNING: Couldn't contact %s.\n\n", theAddrBufPtr.Ptr);
			badAddr = ioIPAddrArray[count];
			//::exit(-1);
		}
		else if (theClient->GetStatus() != 200)
		{
			printf("##WARNING: %s doesn't have %s.\n\n",theAddrBufPtr.Ptr, theURL.Ptr);
			badAddr = ioIPAddrArray[count];
			//::exit(-1);
		}
		
		//if the address was unreachable or did not give permission to be spammed, then clear it from our IPAddress list.
		if (badAddr != 0)
		{
			for (UInt32 x = 0; x < inNumURLs; x++)
			{
				if (badAddr == ioIPAddrArray[count])
					ioIPAddrArray[count] = 0;
			}
		}
		
		theClient->SendTeardown();
		delete theClient;
		delete theSocket;
	}

	int addrCount = 0;
	for (UInt32 x = 0; x < inNumURLs; x++)
	{
		if ( 0 != ioIPAddrArray[x])
			addrCount++ ;
	}
	if (addrCount == 0)
	{	printf("No valid destinations\n");
		exit (-1);
	}
	delete [] uniqueIPAddrs;
	printf("Done checking for 'streamingloadtool.mov' on all servers\n");	
}


void DoDNSLookup(FilePrefsSource* inFileParser, UInt32* inURLIndexes, UInt32* ioIPAddrs, UInt32 inNumURLs)
{
	char theDNSName[128];
	
	for (UInt32 x = 0; x < inNumURLs; x++)
	{
		// First extract the DNS name from this URL as a c-string
		StrPtrLen theURL(inFileParser->GetValueAtIndex(inURLIndexes[x]));
		StringParser theURLParser(&theURL);
		StrPtrLen theDNSNamePtr;
		
		theURLParser.ConsumeLength(NULL, 7); // skip over rtsp://
		theURLParser.ConsumeUntil(&theDNSNamePtr, '/'); // grab the DNS name
		StringParser theDNSParser(&theDNSNamePtr);
		theDNSParser.ConsumeUntil(&theDNSNamePtr, ':'); // strip off the port number if any
		
		theDNSName[0] = 0;
		::memcpy(theDNSName, theDNSNamePtr.Ptr, theDNSNamePtr.Len);
		theDNSName[theDNSNamePtr.Len] = 0;
		
		Assert(theDNSNamePtr.Len < 128);
		
		ioIPAddrs[x] = 0;
		
		// Now pass that DNS name into gethostbyname.
		struct hostent* theHostent = ::gethostbyname(theDNSName);
		
		if (theHostent != NULL)
			ioIPAddrs[x] = ntohl(*(UInt32*)(theHostent->h_addr_list[0]));
		else
			ioIPAddrs[x] = SocketUtils::ConvertStringToAddr(theDNSName);
		
		if (ioIPAddrs[x] == 0)
		{
			printf("Couldn't look up host name: %s.\n", theDNSName);
			//exit(-1);
		}
	}
}

char* 	GetClientTypeDescription(ClientSession::ClientType inClientType)
{
	static char* kUDPString = "RTSP/UDP client";
	static char* kTCPString = "RTSP/TCP client";
	static char* kHTTPString = "RTSP/HTTP client";
	static char* kHTTPDropPostString = "RTSP/HTTP drop post client";
	static char* kReliableUDPString = "RTSP/ReliableUDP client";
	
	switch (inClientType)
	{
		case ClientSession::kRTSPUDPClientType:
			return kUDPString;
		case ClientSession::kRTSPTCPClientType:
			return kTCPString;
		case ClientSession::kRTSPHTTPClientType:
			return kHTTPString;
		case ClientSession::kRTSPHTTPDropPostClientType:
			return kHTTPDropPostString;
		case ClientSession::kRTSPReliableUDPClientType:
			return kReliableUDPString;
	}
	Assert(0);
	return NULL;
}

char*	GetDeathReasonDescription(UInt32 inDeathReason)
{
	static char* kDiedNormallyString = "Completed normally";
	static char* kTeardownFailedString = "Couldn't complete TEARDOWN";
	static char* kRequestFailedString = "Failed RTSP request";
	static char* kBadSDPString = "misformatted SDP";
	static char* kSessionTimedoutString = "Couldn't connect to server";
	static char* kConnectionFailedString = "Server refused connection";
	static char* kDiedWhilePlayingString = "Disconnected while playing";

	switch (inDeathReason)
	{
		case ClientSession::kDiedNormally:
			return kDiedNormallyString;
		case ClientSession::kTeardownFailed:
			return kTeardownFailedString;
		case ClientSession::kRequestFailed:
			return kRequestFailedString;
		case ClientSession::kBadSDP:
			return kBadSDPString;
		case ClientSession::kSessionTimedout:
			return kSessionTimedoutString;
		case ClientSession::kConnectionFailed:
			return kConnectionFailedString;
		case ClientSession::kDiedWhilePlaying:
			return kDiedWhilePlayingString;
	}
	Assert(0);
	return NULL;
}

char*	GetPayloadDescription(QTSS_RTPPayloadType inPayload)
{
	static char*	kSound = "Sound";
	static char*	kVideo = "Video";
	static char*	kUnknown = "Unknown";

	switch (inPayload)
	{
		case qtssVideoPayloadType:
			return kVideo;
		case qtssAudioPayloadType:
			return kSound;
		default:
			return kUnknown;
	}
	return NULL;
}

void RecordClientInfoBeforeDeath(ClientSession* inSession)
{
	if (inSession->GetReasonForDying() == ClientSession::kRequestFailed)
		sConnectionsThatErrored++;
	else if (inSession->GetReasonForDying() != ClientSession::kDiedNormally)
		sFailedConnections++;
	else
		sSuccessfulConnections++;
				
	if (sLog != NULL)
	{
		UInt32 theReason = inSession->GetReasonForDying();
		in_addr theAddr;
		theAddr.s_addr = htonl(inSession->GetSocket()->GetHostAddr());
		char* theAddrStr = ::inet_ntoa(theAddr);
		
		//
		// Write a log entry for this client
		::fprintf(sLog, "Client complete. IP addr = %s, URL = %s. Connection status: %s. ",
						theAddrStr,
						inSession->GetClient()->GetURL()->Ptr,
						::GetDeathReasonDescription(theReason));
						
		if (theReason == ClientSession::kRequestFailed)
			::fprintf(sLog, "Failed request status: %lu", inSession->GetRequestStatus());
		
		::fprintf(sLog, "\n");
		
		//
		// If this was a successful connection, log statistics for this connection
		if ((theReason == ClientSession::kDiedNormally) || (theReason == ClientSession::kTeardownFailed))
		{
			UInt32 bytesReceived = 0;
			for (UInt32 trackCount = 0; trackCount < inSession->GetSDPInfo()->GetNumStreams(); trackCount++)
			{
				::fprintf(sLog, "Track type: %s. Total packets received: %lu. Total packets lost: %lu. Total out of order packets: %lu. Total duplicate packets: %lu. Total ACKs sent: %lu.\n",
					::GetPayloadDescription(inSession->GetTrackType(trackCount)), 
					inSession->GetNumPacketsReceived(trackCount),
					inSession->GetNumPacketsLost(trackCount),
					inSession->GetNumPacketsOutOfOrder(trackCount),
					inSession->GetNumDuplicates(trackCount),
					inSession->GetNumAcks(trackCount));
					
				bytesReceived += inSession->GetNumBytesReceived(trackCount);
			}
			UInt32 duration = (UInt32)(inSession->GetTotalPlayTimeInMsec() / 1000);
			Float32 bitRate = (((Float32)bytesReceived) / ((Float32)duration) * 8) / 1024;
			::fprintf(sLog, "Play duration in sec: %lu. Total stream bit rate in Kbits / sec: %f.\n", duration, bitRate);
		}
		
		::fprintf(sLog, "\n");		
	}
}

