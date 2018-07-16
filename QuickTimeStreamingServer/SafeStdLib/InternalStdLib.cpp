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
    File:       InternalStdLib.cpp

    Contains:   Thread safe std lib calls for internal modules and apps
    

*/


#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "OSMutex.h"
#include "OS.h"

#include "OSMemory.h"
#include "SafeStdLib.h"


#ifndef USE_DEFAULT_STD_LIB

#if __Win32__

#define VSNprintf _vsnprintf

#else

#define VSNprintf vsnprintf

#endif




int qtss_printf(char *fmt,  ...)
{
    OSMutexLocker locker(OS::GetStdLibMutex());
    va_list args;
    va_start(args,fmt);
    int result = ::vprintf(fmt, args);
    va_end(args);
    
    return result;
}

int qtss_sprintf(char *buffer, const char *fmt,  ...)
{
    OSMutexLocker locker(OS::GetStdLibMutex());
    va_list args;
    va_start(args,fmt);
    int result = ::vsprintf(buffer, fmt, args);
    va_end(args);
    
    return result;
}

int qtss_fprintf(FILE *file, const char *fmt,  ...)
{
    OSMutexLocker locker(OS::GetStdLibMutex());
    va_list args;
    va_start(args,fmt);
    int result = ::vfprintf(file, fmt, args);
    va_end(args);
    
    return result;
}

int  qtss_snprintf(char *str, size_t size, const char *fmt, ...)
{
    OSMutexLocker locker(OS::GetStdLibMutex());
    va_list args;
    va_start(args,fmt);
    int result = ::VSNprintf(str, size, fmt, args);
    va_end(args);
    
    return result;
}

size_t qtss_strftime(char *buf, size_t maxsize, const char *format, const struct tm *timeptr)
{
    OSMutexLocker locker(OS::GetStdLibMutex());
    return ::strftime(buf, maxsize, format, timeptr); 

}

#endif //USE_DEFAULT_STD_LIB


char *qtss_strerror(int errnum, char* buffer, int buffLen)
{
    OSMutexLocker locker(OS::GetStdLibMutex());
 	(void) ::strncpy( buffer, ::strerror(errnum), buffLen); 
	buffer[buffLen -1] = 0;  //make sure it is null terminated even if truncated.
 
    return buffer;
}

char *qtss_ctime(const time_t *timep, char* buffer, int buffLen)
{
#if __MacOSX__
    Assert(buffLen >= 26);
    return ::ctime_r(timep, buffer);
#else
    OSMutexLocker locker(OS::GetStdLibMutex());
    ::strncpy( buffer, ::ctime(timep), buffLen);//don't use terminator
	buffer[buffLen -1] = 0;  //make sure it is null terminated even if truncated.
    
	return buffer;
#endif
}

char *qtss_asctime(const struct tm *timeptr, char* buffer, int buffLen)
{
#if __MacOSX__
    Assert(buffLen >= 26);
    return ::asctime_r(timeptr, buffer);
#else
    OSMutexLocker locker(OS::GetStdLibMutex());
    ::strncpy( buffer, ::asctime(timeptr), buffLen);
    buffer[buffLen -1] = 0;  //make sure it is null terminated even if truncated.
    
    return buffer;
#endif
}

struct tm *qtss_gmtime(const time_t *timep, struct tm *result)
{
#if __MacOSX__
    return ::gmtime_r(timep, result);
#else
    OSMutexLocker locker(OS::GetStdLibMutex());
    struct tm *time_result = ::gmtime(timep);
    *result = *time_result;
    
    return result;
#endif
}

struct tm *qtss_localtime(const time_t *timep, struct tm *result)
{
 #if __MacOSX__
    return ::localtime_r(timep, result);
#else
   OSMutexLocker locker(OS::GetStdLibMutex());
    struct tm *time_result = ::localtime(timep);
    *result = *time_result;
    
    return result;
#endif
}



