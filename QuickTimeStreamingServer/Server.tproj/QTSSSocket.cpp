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
    File:       QTSSSocket.cpp

    Contains:   A QTSS Stream object for a generic socket
    
    Written By: Denis Serenyi
            
    $Log: QTSSSocket.cpp,v $
    Revision 1.5  2003/08/15 23:53:21  sbasu
    3370815 Need to update to the APSL 2
    Bug #:
    Submitted by:
    Reviewed by:

    Revision 1.4  2002/02/26 00:25:36  murata
    Convert tabs to spaces in .cpp .c and .h files.
    Bug #:
    Submitted by:
    Reviewed by:

    Revision 1.3  2001/03/13 22:24:39  murata
    Replace copyright notice for license 1.0 with license 1.2 and update the copyright year.
    Bug #:
    Submitted by:
    Reviewed by:
    
    Revision 1.2  2000/10/11 07:06:15  serenyi
    import
    
    Revision 1.1.1.1  2000/08/31 00:30:49  serenyi
    Mothra Repository
    
    Revision 1.1  2000/05/22 06:05:28  serenyi
    Added request body support, API additions, SETUP without DESCRIBE support, RTCP bye support
    
    
    
*/

#include "QTSSSocket.h"

QTSS_Error  QTSSSocket::RequestEvent(QTSS_EventType inEventMask)
{
    int theMask = 0;
    
    if (inEventMask & QTSS_ReadableEvent)
        theMask |= EV_RE;
    if (inEventMask & QTSS_WriteableEvent)
        theMask |= EV_WR;
        
    fEventContext.SetTask(this->GetTask());
    fEventContext.RequestEvent(theMask);
    return QTSS_NoErr;
}
