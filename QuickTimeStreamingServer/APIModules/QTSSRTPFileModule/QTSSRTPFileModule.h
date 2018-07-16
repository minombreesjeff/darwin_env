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
    File:       QTSSFileModule.h

    Contains:   Content source module that uses the QTFileLib to serve Hinted QuickTime
                files to clients. 
                    
    $Log: QTSSRTPFileModule.h,v $
    Revision 1.5  2003/08/15 23:52:45  sbasu
    3370815 Need to update to the APSL 2
    Bug #:
    Submitted by:
    Reviewed by:

    Revision 1.4  2002/02/26 00:18:24  murata
    Convert tabs to spaces in .cpp .c and .h files.
    Bug #:
    Submitted by:
    Reviewed by:

    Revision 1.3  2001/07/16 08:09:53  mythili
    added newline to fix warnings for solaris compile
    
    Revision 1.2  2001/03/20 02:19:19  mythili
    updated/added APSL 1.2
    
    Revision 1.1  2000/10/20 07:24:52  serenyi
    Created
    
    Revision 1.2  1999/09/24 21:19:58  rtucker
    added code for client tcp flow control debugging
    
    Revision 1.1.1.1  1999/09/13 16:16:32  rtucker
    initial checkin from rrv14
    
    
    Revision 1.2  1999/02/19 23:07:22  ds
    Created
    

*/

#ifndef _RTPRTPFILEMODULE_H_
#define _RTPRTPFILEMODULE_H_

#include "QTSS.h"

extern "C"
{
    EXPORT QTSS_Error QTSSRTPFileModule_Main(void* inPrivateArgs);
}

#endif //_RTPRTPFILEMODULE_H_
