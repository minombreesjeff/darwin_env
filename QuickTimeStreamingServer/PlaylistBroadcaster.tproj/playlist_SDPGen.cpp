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
#include "playlist_utils.h"
#include "playlist_SDPGen.h"
#include "playlist_broadcaster.h"
#include "QTRTPFile.h"
#include "OS.h"
#include "SocketUtils.h"

#include "StringFormatter.h"
#include "ResizeableStringFormatter.h"
#include "StringParser.h"
#include "OSMemory.h"

class SDPLine : public StrPtrLen
{
public:
	SDPLine() : fHeaderType('\0') {}
    virtual ~SDPLine() {}

    char    fHeaderType;
};

class SDPContainer
{
    enum { kBaseLines = 20 };

public:

    SDPContainer(UInt32 numStrPtrs = SDPContainer::kBaseLines) : fCurrentLine(0),fNumSDPLines(numStrPtrs),
	fNumUsedLines(0), fSDPLineArray(NULL), fValid(false)
    { fSDPLineArray = NEW SDPLine[fNumSDPLines]; }

    ~SDPContainer() {delete [] fSDPLineArray;}
	void		Initialize();
    SInt32      AddHeaderLine (StrPtrLen *theLinePtr);
    SInt32      FindHeaderLineType(char id, SInt32 start);
    SDPLine*    GetNextLine();
    SDPLine*    GetLine(SInt32 lineIndex);
    void        SetLine(SInt32 index);
    void        Parse();
    Bool16      SetSDPBuffer(char *sdpBuffer);
    Bool16      SetSDPBuffer(StrPtrLen *sdpBufferPtr);
    Bool16      IsSDPBufferValid() {return fValid;}
    void        PrintLine(SInt32 lineIndex);
    void        PrintAllLines();
    SInt32      GetNumLines() { return  fNumUsedLines; }
    
    SInt32      fCurrentLine;
    SInt32      fNumSDPLines;
    SInt32      fNumUsedLines;
    SDPLine*    fSDPLineArray;
    Bool16      fValid;
    StrPtrLen   fSDPBuffer;

};

class SDPLineSorter {

public:
	SDPLineSorter(): fSessionLineCount(0),fSDPSessionHeaders(NULL,0) {};
	SDPLineSorter(SDPContainer *rawSDPContainerPtr);
	
	StrPtrLen* GetSessionHeaders() { return &fSessionHeaders; }
	StrPtrLen* GetMediaHeaders() { return &fMediaHeaders; }
	char* GetSortedSDPCopy();
	
	StrPtrLen fullSDPBuffSPL;
	SInt32 fSessionLineCount;
	SDPContainer fSessionSDPContainer;
	ResizeableStringFormatter fSDPSessionHeaders;
	StrPtrLen fSessionHeaders;
	StrPtrLen fMediaHeaders;
	static char sSessionOrderedLines[];// = "vosiuepcbtrzka"; // chars are order dependent: declared by rfc 2327
	static char sessionSingleLines[];//  = "vosiuepcbzk";    // return only 1 of each of these session field types
	static StrPtrLen sEOL;
};

SInt32 SDPContainer::AddHeaderLine (StrPtrLen *theLinePtr)
{   
    Assert(theLinePtr);
    UInt32 thisLine = fNumUsedLines;
    Assert(fNumUsedLines < fNumSDPLines);
    fSDPLineArray[thisLine].Set(theLinePtr->Ptr, theLinePtr->Len);
    fNumUsedLines++;
    if (fNumUsedLines == fNumSDPLines)
    {
        SDPLine   *tempSDPLineArray = NEW SDPLine[fNumSDPLines * 2];
        for (int i = 0; i < fNumSDPLines; i++)
        {
            tempSDPLineArray[i].Set(fSDPLineArray[i].Ptr,fSDPLineArray[i].Len);
            tempSDPLineArray[i].fHeaderType = fSDPLineArray[i].fHeaderType;
        }
        delete [] fSDPLineArray;
        fSDPLineArray = tempSDPLineArray;
        fNumSDPLines = (fNumUsedLines * 2);
    }
    
    if (theLinePtr->Ptr)
        fSDPLineArray[thisLine].fHeaderType = theLinePtr->Ptr[0];
        
    return thisLine;
}

SInt32 SDPContainer::FindHeaderLineType(char id, SInt32 start)
{   
    SInt32 theIndex = -1;
    
    if (start >= fNumUsedLines || start < 0)
        return -1;
        
    for (int i = start; i < fNumUsedLines; i++)
    {   if (fSDPLineArray[i].fHeaderType == id)
        {   theIndex = i;
            fCurrentLine = theIndex;
            break;
        }
    }
    
    return theIndex;
}

SDPLine* SDPContainer::GetNextLine()
{
    if (fCurrentLine < fNumUsedLines)
    {   fCurrentLine ++;
        return &fSDPLineArray[fCurrentLine];
    }
    
    return NULL;

}

SDPLine* SDPContainer::GetLine(SInt32 lineIndex)
{
    
    if (lineIndex > -1 && lineIndex < fNumUsedLines)
    {   return &fSDPLineArray[lineIndex];
    }

    return NULL;
}

void SDPContainer::SetLine(SInt32 index)
{
    if (index > -1 && index < fNumUsedLines)
    {   fCurrentLine = index;
    }
    else
        Assert(0);
        
}

void SDPContainer::Parse()
{
	char*	    validChars = "vosiuepcbtrzkam";
	char        nameValueSeparator = '=';
	
	Bool16      valid = true;

	StringParser	sdpParser(&fSDPBuffer);
	StrPtrLen		line;
	StrPtrLen 		fieldName;
	StrPtrLen		space;
	
	while ( sdpParser.GetDataRemaining() != 0 )
	{
		sdpParser.GetThruEOL(&line);  // Read each line  
        StringParser lineParser(&line);

        lineParser.ConsumeWhitespace();//skip over leading whitespace
        if (lineParser.GetDataRemaining() == 0) // must be an empty line
            continue;

        char firstChar = lineParser.PeekFast();
        if (firstChar == '\0')
            continue; //skip over blank lines
        
		lineParser.ConsumeUntil(&fieldName, nameValueSeparator);
		if ((fieldName.Len != 1) || (::strchr(validChars, fieldName.Ptr[0]) == NULL))
		{
			valid = false; // line doesn't begin with one of the valid characters followed by an "="
			break;
		}
		
		if (!lineParser.Expect(nameValueSeparator))
		{
			valid = false; // line doesn't have the "=" after the first char
			break;
		}
		
		lineParser.ConsumeUntil(&space, StringParser::sWhitespaceMask);
		
		if (space.Len != 0)
		{
			valid = false; // line has whitespace after the "=" 
			break;
		}
		AddHeaderLine(&line);
	}
	
	if (fNumUsedLines == 0) // didn't add any lines
	{   valid = false;
	}
	fValid = valid;
	
}

Bool16 SDPContainer::SetSDPBuffer(char *sdpBuffer) 
{ 
    
    fCurrentLine = 0;
    fNumUsedLines = 0;
    fValid = false;
    if (sdpBuffer != NULL)
    {   fSDPBuffer.Set(sdpBuffer); 
        Parse(); 
    }
    
    return IsSDPBufferValid();
}

Bool16 SDPContainer::SetSDPBuffer(StrPtrLen *sdpBufferPtr)
{ 
    fCurrentLine = 0;
    fNumUsedLines = 0;
    fValid = false;

    if (sdpBufferPtr != NULL)
    {   fSDPBuffer.Set(sdpBufferPtr->Ptr, sdpBufferPtr->Len); 
        Parse(); 
    }
    
    return IsSDPBufferValid();
}


void  SDPContainer::PrintLine(SInt32 lineIndex)
{
    StrPtrLen *printLinePtr = GetLine(lineIndex);
    if (printLinePtr)
    {   printLinePtr->PrintStr();
        printf("\n");
    }

}

void  SDPContainer::PrintAllLines()
{
    if (fNumUsedLines > 0)
    {   for (int i = 0; i < fNumUsedLines; i++)
            PrintLine(i);
    }
    else
        printf("SDPContainer::PrintAllLines no lines\n"); 
}


char SDPLineSorter::sSessionOrderedLines[] = "vosiuepcbtrzka"; // chars are order dependent: declared by rfc 2327
char SDPLineSorter::sessionSingleLines[]  = "vosiuepcbzk";    // return only 1 of each of these session field types
StrPtrLen  SDPLineSorter::sEOL("\r\n");

SDPLineSorter::SDPLineSorter(SDPContainer *rawSDPContainerPtr) : fSessionLineCount(0),fSDPSessionHeaders(NULL,0)
{

	Assert(rawSDPContainerPtr != NULL);
	if (NULL == rawSDPContainerPtr) 
		return;
		
	StrPtrLen theSDPData(rawSDPContainerPtr->fSDPBuffer.Ptr,rawSDPContainerPtr->fSDPBuffer.Len);
	StrPtrLen *theMediaStart = rawSDPContainerPtr->GetLine(rawSDPContainerPtr->FindHeaderLineType('m',0));
 	if (theMediaStart && theMediaStart->Ptr && theSDPData.Ptr)
	{
		UInt32  mediaLen = theSDPData.Len - (UInt32) (theMediaStart->Ptr - theSDPData.Ptr);
		char *mediaStartPtr= theMediaStart->Ptr;
		fMediaHeaders.Set(mediaStartPtr,mediaLen);
	}       


	fSessionLineCount = rawSDPContainerPtr->FindHeaderLineType('m',0);
	if (fSessionLineCount < 0) // didn't find it use the whole buffer
	{   fSessionLineCount = rawSDPContainerPtr->GetNumLines();
	}

	for (SInt16 sessionLineIndex = 0; sessionLineIndex < fSessionLineCount; sessionLineIndex++)
		fSessionSDPContainer.AddHeaderLine( (StrPtrLen *) rawSDPContainerPtr->GetLine(sessionLineIndex));

	//printff("\nSession raw Lines:\n"); fSessionSDPContainer.PrintAllLines();

	SInt16 numHeaderTypes = sizeof(SDPLineSorter::sSessionOrderedLines) -1;

	for (SInt16 fieldTypeIndex = 0; fieldTypeIndex < numHeaderTypes; fieldTypeIndex ++)
	{
		SInt32 lineIndex = fSessionSDPContainer.FindHeaderLineType(SDPLineSorter::sSessionOrderedLines[fieldTypeIndex], 0);
		StrPtrLen *theHeaderLinePtr = fSessionSDPContainer.GetLine(lineIndex);
		while (theHeaderLinePtr != NULL)
		{
			fSDPSessionHeaders.Put(*theHeaderLinePtr);
			fSDPSessionHeaders.Put(SDPLineSorter::sEOL);

			if (NULL != ::strchr(sessionSingleLines, theHeaderLinePtr->Ptr[0] ) ) // allow 1 of this type: use first found
				break; // move on to next line type

			lineIndex = fSessionSDPContainer.FindHeaderLineType(SDPLineSorter::sSessionOrderedLines[fieldTypeIndex], lineIndex + 1);
			theHeaderLinePtr = fSessionSDPContainer.GetLine(lineIndex);
		}
	}
	fSessionHeaders.Set(fSDPSessionHeaders.GetBufPtr(),fSDPSessionHeaders.GetBytesWritten());

}

char* SDPLineSorter::GetSortedSDPCopy()
{
	char* fullbuffCopy = NEW char[fSessionHeaders.Len + fMediaHeaders.Len + 2];
	SInt32 buffPos = 0;
	memcpy(&fullbuffCopy[buffPos], fSessionHeaders.Ptr,fSessionHeaders.Len);
	buffPos += fSessionHeaders.Len;
	memcpy(&fullbuffCopy[buffPos], fMediaHeaders.Ptr,fMediaHeaders.Len);
	buffPos += fMediaHeaders.Len;
	fullbuffCopy[buffPos] = 0;	
	
	return fullbuffCopy;
}


short SDPGen::AddToBuff(char *aSDPFile, short currentFilePos, char *chars)
{
	short charLen = strlen(chars);
	short newPos = currentFilePos + charLen;
	
	if (newPos <= eMaxSDPFileSize)
	{	memcpy(&aSDPFile[currentFilePos],chars,charLen); // only the chars not the \0
		aSDPFile[currentFilePos +charLen] = '\0';
	}	
	else
	{	newPos =  (-newPos);	
	}
	currentFilePos = newPos;
	
	return currentFilePos;
}

UInt32 SDPGen::RandomTime(void)
{
	SInt64 curTime = 0;
	#ifndef __MACOS__
		curTime = PlayListUtils::Milliseconds();
	#endif
	curTime += rand() ;
	return (UInt32) curTime;
}

short  SDPGen::GetNoExtensionFileName(char *pathName, char *result, short maxResult)
{
	char *start;
	char *end;
	char *sdpPath = pathName;
	short pathNameLen = strlen(pathName);
	short copyLen = 0;
	
	
	do 
	{
		start = strrchr(sdpPath, ePath_Separator);
		if(start  == NULL ) // no path separator
		{	start = sdpPath;
			copyLen = 	pathNameLen;
			break;
		} 
		
		start ++; // move start to one past the separator
		end = strrchr(sdpPath, eExtension_Separator);		
		if (end == NULL) // no extension
		{	copyLen = strlen(start) + 1;
			break;
		}
		
		// both path separator and an extension
		short startLen = strlen(start);
		short endLen = strlen(end);
		copyLen = startLen - endLen;
		
	} while (false);
	
	if (copyLen > maxResult)
		copyLen = maxResult;
		
	memcpy(result, start, copyLen);	
	result[copyLen] = '\0';
	
	return copyLen;
}
			
/*
short SDPGen::GetLocalAddrStr(char *returnStr, short maxSize)
{
	short result = 0;
	char defaultAddress[] = "255.255.255.255";
	char* addr = defaultAddress;
	int err = -1;
	do 
	{
	
		//Most of this code is similar to the SIOCGIFCONF code presented in Stevens,
		//Unix Network Programming, section 16.6
		
		//Use the SIOCGIFCONF ioctl call to iterate through the network interfaces
		static const UInt32 kMaxAddrBufferSize = 2048;
		
		struct ifconf ifc;
		struct ifreq* ifr;
		char buffer[kMaxAddrBufferSize];
		
		int tempSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
		if (tempSocket == -1) break;
			
		ifc.ifc_len = kMaxAddrBufferSize;
		ifc.ifc_buf = buffer;
		
		err = ::ioctl(tempSocket, SIOCGIFCONF, (char*)&ifc);
		if (err == -1) break;
		
		::close(tempSocket);
		tempSocket = -1;

		char* ifReqIter = NULL;
		
		for (ifReqIter = buffer; ifReqIter < (buffer + ifc.ifc_len);)
		{
			ifr = (struct ifreq*)ifReqIter;
#if __MacOSX__
			ifReqIter += sizeof(ifr->ifr_name) + ifr->ifr_addr.sa_len;
			if (ifr->ifr_addr.sa_len == 0)
			{
				switch (ifr->ifr_addr.sa_family)
				{
					case AF_INET:
						ifReqIter += sizeof(struct sockaddr_in);
						break;
					default:
						ifReqIter += sizeof(struct sockaddr);
				}
			}
#else
			ifReqIter += sizeof(ifr->ifr_name) + 0;
			switch (ifr->ifr_addr.sa_family)
			{
				case AF_INET:
					ifReqIter += sizeof(struct sockaddr_in);
					break;
				default:
					ifReqIter += sizeof(struct sockaddr);
			}
#endif
	
			
			//Only count interfaces in the AF_INET family.
			//And don't count localhost, loopback interfaces
			if ((ifr->ifr_addr.sa_family == AF_INET) && (::strncmp(ifr->ifr_name, "lo", 2) != 0))
			{
				struct sockaddr_in* addrPtr = (struct sockaddr_in*)&ifr->ifr_addr;	
				addr = ::inet_ntoa(addrPtr->sin_addr);
				err = 0;
				break;
			}
		}
	
	} while (false);
		

	result = strlen(addr);
	
	if (maxSize < result)
	{	err = -1;
	}
	else
		strcpy(returnStr, addr);
	
	return err;
}
*/

char *SDPGen::Process(	char *sdpFileName,
				 		char * basePort,
				 		char *ipAddress,
				 		char *anSDPBuffer,
				 		char *startTime,
				 		char *endTime,	
				 		char *isDynamic,
				 		int *error
				 		)
{
	char *resultBuf = NULL;
	short currentPos = 0;
	short trackID = 1;
	*error = -1;
	do
	{
		fSDPFileContentsBuf = new char[eMaxSDPFileSize];
		
		// SDP required RFC 2327
	    //    v=  (protocol version)
		//	  o=<username> <session id = random time> <version = random time *> <network type = IN> <address type = IP4> <local address>
	    //    s=  (session name)
	    //    c=IN IP4 (destinatin ip address)
		// * see RFC for recommended Network Time Stamp (NTP implementation not required)

	    //    v=  (protocol version)
		{	char version[] = "v=0\r\n";
		
			currentPos = AddToBuff(fSDPFileContentsBuf, currentPos, version);
			if (currentPos < 0) break;
		}
			
		//	  o=<username> <session id = random time> <version = random time *> <network type = IN> <address type = IP4> <address>
		{	char *userName = "QTSS_Play_List";
			UInt32 sessIDAsRandomTime = RandomTime();
			UInt32 versAsRandomTime = RandomTime();
			char  ownerLine[255];

			Assert(SocketUtils::GetIPAddrStr(0) != NULL);
			Assert(SocketUtils::GetIPAddrStr(0)->Ptr != NULL);
			sprintf(ownerLine, "o=%s %lu %lu IN IP4 %s\r\n",userName ,sessIDAsRandomTime,versAsRandomTime,SocketUtils::GetIPAddrStr(0)->Ptr);
			currentPos = AddToBuff(fSDPFileContentsBuf, currentPos, ownerLine);
			if (currentPos < 0) break;
		}
		

	    //    s=  (session name)
		{	enum { eMaxSessionName = 64};
			char newSessionName[eMaxSessionName];
			short nameSize = 0;
			char  sessionLine[255];
			nameSize = GetNoExtensionFileName(sdpFileName, newSessionName, eMaxSessionName);
			if (nameSize < 0) break;
		
			sprintf(sessionLine, "s=%s\r\n", newSessionName);
			currentPos = AddToBuff(fSDPFileContentsBuf, currentPos, sessionLine);
			if (currentPos < 0) break;
		}
		
		//    c=IN IP4 (destinatin ip address)
		{	
			char  sdpLine[255];
			sprintf(sdpLine, "c=IN IP4 %s\r\n", ipAddress);
			currentPos = AddToBuff(fSDPFileContentsBuf, currentPos, sdpLine);
			if (currentPos < 0) break;
		}
		
		{
			char  controlLine[255];
			if ( 0 == ::strcmp( "enabled", isDynamic) )
				sprintf(controlLine, "a=x-broadcastcontrol:RTSP\r\n");			
			else
				sprintf(controlLine, "a=x-broadcastcontrol:TIME\r\n");			
			currentPos = AddToBuff(fSDPFileContentsBuf, currentPos, controlLine);
			if (currentPos < 0) break;
		}
				
		
		{
			char  timeLine[255];
			UInt32 startTimeNTPSecs = strtoul(startTime, NULL, 10);
			UInt32 endTimeNTPSecs = strtoul(endTime, NULL, 10);
			sprintf(timeLine, "t=%lu %lu\r\n", startTimeNTPSecs, endTimeNTPSecs);			
			currentPos = AddToBuff(fSDPFileContentsBuf, currentPos, timeLine);
			if (currentPos < 0) break;
		}

		{
			SimpleString resultString;
			SimpleString sdpBuffString(anSDPBuffer);
	
			short numLines = 0;
			char *found = NULL;
			
			enum  {eMaxLineLen = 1024};
			char aLine[eMaxLineLen];
			::memset(aLine, 0, eMaxLineLen);
			
			int portCount = atoi(basePort);
			
			
			
			SimpleParser sdpParser;
			while ( sdpParser.GetLine(&sdpBuffString,&resultString) ) 
			{							
				numLines ++;
				if (resultString.fLen > 1024) continue;
				 
				memcpy(aLine,resultString.fTheString,resultString.fLen);
				aLine[resultString.fLen] = '\0';
				
				int newBuffSize = sdpBuffString.fLen - (resultString.fLen);
				char *newBuffPtr = &resultString.fTheString[resultString.fLen];
				
				sdpBuffString.SetString(newBuffPtr, newBuffSize);	

				char firstChar = aLine[0];
				{ // we are setting these so ignore any defined						
					if (firstChar == 'v') continue;// (protocol version)
					if (firstChar == 'o') continue; //(owner/creator and session identifier).
					if (firstChar == 's') continue; //(session name)
					if (firstChar == 'c') continue; //(connection information - optional if included at session-level)
				}
				
				{ 	// no longer important as a play list broadcast
					// there may be others that should go here.......
					if (firstChar == 't') continue;// (time the session is active)				
					if (firstChar == 'r') continue;// (zero or more repeat times)

					// found =  strstr(aLine, "a=cliprect"); // turn this off
					// if (found != NULL) continue;
					
					found = strstr(aLine, "a=control:trackID"); // turn this off
					if (!fKeepTracks)
					{	
						if (fAddIndexTracks)
						{
							if (found != NULL) 
							{	char mediaLine[eMaxLineLen];											
								::sprintf(mediaLine,"a=control:trackID=%d\r\n",trackID);
								currentPos = AddToBuff(fSDPFileContentsBuf, currentPos, mediaLine); // copy rest of line starting with the transport protocol
								trackID ++;
							}
						}
						if (found != NULL) continue;
					}
					
					
					found = strstr(aLine,  "a=range"); // turn this off
					if (found != NULL) continue;
					
					// found = strstr(aLine,  "a=x"); // turn this off - 
					// if (found != NULL) continue;
				}
				
				{ // handle the media line and put in the port value past the media type
					found = strstr(aLine,"m=");  //(media name and transport address)
					if (found != NULL)
					{	
						char *startToPortVal = strtok(aLine," ");
						strtok(NULL," "); // step past the current port value we put it in below
						if (found != NULL) 
						{	char mediaLine[eMaxLineLen];				
							char *protocol = strtok(NULL,"\r\n"); // the transport protocol
							
							::sprintf(mediaLine,"%s %d %s\r\n",startToPortVal,portCount,protocol);
							currentPos = AddToBuff(fSDPFileContentsBuf, currentPos, mediaLine); // copy rest of line starting with the transport protocol
							if (portCount != 0)
								portCount += 2; // set the next port value ( this port + 1 is the RTCP port for this port so we skip by 2)
							continue;
						}
					}
				}

				{ 	// this line looks ok so just get it and make sure it has a carriage return + new line at the end
					// also remove any garbage that might be there
					// this is a defensive measure because the hinter may have bad line endings like a single \n or \r or 
					// there might also be chars after the last line delimeter and before a \0 so we get rid of it.
					
					short lineLen = strlen(aLine);
					
					// get rid of trailing characters
					while (lineLen > 0 && (NULL == strchr("\r\n",aLine[lineLen]))  )
					{	aLine[lineLen] = '\0';
						lineLen --;
					}
					
					// get rid of any line feeds and carriage returns
					while (lineLen > 0 && (NULL != strchr("\r\n",aLine[lineLen])) )
					{	aLine[lineLen] = '\0';
						lineLen --;
					}
					aLine[lineLen + 1] = '\r';
					aLine[lineLen + 2] = '\n';
					aLine[lineLen + 3] = 0;
					currentPos = AddToBuff(fSDPFileContentsBuf, currentPos, aLine); // copy this line	
				}
			}

			//  buffer delay 
			if (fClientBufferDelay > 0.0)
			{	
				char  delayLine[255];
				sprintf(delayLine, "a=x-bufferdelay:%.2f\r\n", fClientBufferDelay);
				currentPos = AddToBuff(fSDPFileContentsBuf, currentPos, delayLine);
				if (currentPos < 0) break;
			}
		}
	
        StrPtrLen theSDPStr(fSDPFileContentsBuf);
        SDPContainer rawSDPContainer; 
        if (!rawSDPContainer.SetSDPBuffer( &theSDPStr ))
        {    delete [] fSDPFileContentsBuf;
             fSDPFileContentsBuf = NULL;
             return NULL; 
        }
    
        SDPLineSorter sortedSDP(&rawSDPContainer);
        resultBuf = sortedSDP.GetSortedSDPCopy(); // return a new copy of the sorted SDP
        delete [] fSDPFileContentsBuf; // this has to happen after GetSortedSDPCopy
        fSDPFileContentsBuf = resultBuf;
		
		*error = 0;
	} while (false);
		
 	return resultBuf;
}

int SDPGen::Run(  char *movieFilename
				, char *sdpFilename
				, char *basePort
				, char *ipAddress
				, char *buff
				, short buffSize
				, bool overWriteSDP
				, bool forceNewSDP
				, char *startTime
				, char *endTime
				, char *isDynamic
				)
{
	int result = -1;
	int fdsdp = -1;
	bool sdpExists = false;
	
	do 
	{
		if (!movieFilename) break;
		if (!sdpFilename) break;
		
		if (buff && buffSize > 0) // set buff to 0 length string
			buff[0] = 0;
			
		fdsdp = open(sdpFilename, O_RDONLY, 0);		
		if (fdsdp != -1)
			sdpExists = true;
			
		if (sdpExists && !forceNewSDP)
		{	
			if (!overWriteSDP) 
			{
				if (buff && (buffSize > 0)) 
				{	int count = ::read(fdsdp,buff, buffSize -1);
					if (count > 0) 
						buff[count] = 0;
				}
					
			}

			close(fdsdp);
			fdsdp = -1;
			
			if (!overWriteSDP) 
			{	result = 0;
				break; // leave nothing to do
			}
		}	
			
		QTRTPFile::ErrorCode err = fRTPFile.Initialize(movieFilename);
		result = QTFileBroadcaster::EvalErrorCode(err);
		if( result != QTRTPFile::errNoError ) 
			break;
						
		// Get the file
		int		sdpFileLength=0;			
		int		processedSize=0;			
		char	*theSDPText = fRTPFile.GetSDPFile(&sdpFileLength);
		
		if( theSDPText == NULL || sdpFileLength <= 0) 
		{	break;
		}
				
		char *processedSDP = NULL;
		processedSDP = Process(sdpFilename, basePort, ipAddress, theSDPText,startTime,endTime,isDynamic, &result);
		if (result != 0) break;
		
		processedSize = strlen(processedSDP);
		
		if (buff != NULL)
		{	if (buffSize > processedSize )
			{	 
				buffSize = processedSize;
			}
			memcpy(buff,processedSDP,buffSize);
			buff[buffSize] = 0;
		}
		
		if (!overWriteSDP && sdpExists) 
		{
			break;
		}
		// Create our SDP file and write out the data			
#ifdef __Win32__
		fdsdp = open(sdpFilename, O_CREAT | O_TRUNC | O_WRONLY | O_BINARY, 0664);
#else
		fdsdp = open(sdpFilename, O_CREAT | O_TRUNC | O_WRONLY, 0664);
#endif
		if( fdsdp == -1 ) 
		{	
                        //result = -1;
                        result = -2;
                      	break;
		}	
		write(fdsdp, processedSDP, processedSize);	
		result = 0;
		
		// report that we made a file
		fSDPFileCreated = true;
		
		
	} while (false);
	
	if (fdsdp != -1)
	{	result = 0;
		close(fdsdp);
		fdsdp = -1;
	}
	
	return result;

}

