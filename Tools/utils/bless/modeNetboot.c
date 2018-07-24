/*
 * Copyright (c) 2005 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
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
 */
/*
 *  modeNetboot.c
 *  bless
 *
 *  Created by Shantonu Sen on 10/10/05.
 *  Copyright 2005 Apple Computer, Inc. All rights reserved.
 *
 */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <net/if.h>
#include <arpa/nameser.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include "enums.h"
#include "structs.h"

#include "bless.h"
#include "bless_private.h"

extern int blesscontextprintf(BLContextPtr context, int loglevel, char const *fmt, ...)
    __attribute__ ((format (printf, 3, 4)));

extern int setefinetworkpath(BLContextPtr context, const char * interface,
                             const char *host, const char *path,
                             int bootNext, const char *optionalData);

static bool validateAddress(const char *host);

int modeNetboot(BLContextPtr context, struct clarg actargs[klast])
{
    int ret;
    BLPreBootEnvType	preboot;
    char                interface[IF_NAMESIZE];
    char                host[NS_MAXDNAME];
    CFURLRef            serverURL;
    CFStringRef         scheme, interfaceString, pathString, hostString;
    
    
    if(!actargs[kserver].present) {
        if(actargs[knetbootserver].present) {
            strcpy(actargs[kserver].argument, actargs[knetbootserver].argument);
            actargs[kserver].present = 1;
        }
    }
    
    if(!actargs[kserver].present) {
        blesscontextprintf(context, kBLLogLevelError,
                           "No NetBoot server specification provided\n");
        return 1;
    } else {
        blesscontextprintf(context, kBLLogLevelVerbose,
                           "NetBoot server specification given as: %s\n",
                           actargs[kserver].argument);
    }
    
    serverURL = CFURLCreateAbsoluteURLWithBytes(kCFAllocatorDefault,
                                     (const UInt8 *)actargs[kserver].argument,
                                     strlen(actargs[kserver].argument),
                                     kCFStringEncodingUTF8,
                                     NULL, false);
    if(!serverURL || !CFURLCanBeDecomposed(serverURL)) {
        if(serverURL) CFRelease(serverURL);
        
        blesscontextprintf(context, kBLLogLevelError,
                           "Could not interpret specification as a URL\n");
        return 2;        
    }

    scheme = CFURLCopyScheme(serverURL);
    if(!scheme || !CFEqual(scheme, CFSTR("bsdp"))) {
        if(scheme) CFRelease(scheme);
        
        blesscontextprintf(context, kBLLogLevelError,
                           "Unrecognized scheme\n");
        return 2;               
    }
    
    CFRelease(scheme);
    
    interfaceString = CFURLCopyUserName(serverURL);
    
    if(interfaceString == NULL) {
        
        ret = BLGetPreferredNetworkInterface(context, interface);
        if(ret) {
            blesscontextprintf(context, kBLLogLevelError,
                               "Failed to determine preferred network interface\n");
            return 1;            
        } else {
            blesscontextprintf(context, kBLLogLevelVerbose,
                               "Preferred network interface is %s\n", interface);            
        }
    } else {
        if(!CFStringGetCString(interfaceString,interface,sizeof(interface),kCFStringEncodingUTF8)) {
            CFRelease(interfaceString);
            blesscontextprintf(context, kBLLogLevelError,
                               "Can't interpret interface as string\n");
            return 3;            
        }
    
        if(!BLIsValidNetworkInterface(context, interface)) {
            blesscontextprintf(context, kBLLogLevelError,
                               "%s is not a valid interface\n", interface);            
            return 4;
        }
        
        CFRelease(interfaceString);
    }
    
    blesscontextprintf(context, kBLLogLevelVerbose,
                       "Using interface %s\n", interface);            
    
    pathString = CFURLCopyStrictPath(serverURL, NULL);
    
    // path component must be NULL or empty
    if(!(pathString == NULL || CFEqual(pathString, CFSTR("")))) {
        CFRelease(pathString);
        blesscontextprintf(context, kBLLogLevelError,
                           "Specification can't contain a path\n");
        return 5;        
    }
    
    if(pathString) CFRelease(pathString);
    
    hostString = CFURLCopyHostName(serverURL);
    
    // host must be present
    if(hostString == NULL || CFEqual(hostString, CFSTR(""))) {
        if(hostString) CFRelease(hostString);
        blesscontextprintf(context, kBLLogLevelError,
                           "Specification doesn't contain host\n");
        return 5;        
    }

    if(!CFStringGetCString(hostString,host,sizeof(host),kCFStringEncodingUTF8)) {
        CFRelease(hostString);
        blesscontextprintf(context, kBLLogLevelError,
                           "Can't interpret host as string\n");
        return 5;            
    }
    
    CFRelease(hostString);
    
    if(!validateAddress(host)) {
        blesscontextprintf(context, kBLLogLevelError,
                           "Can't interpret host %s as an IPv4 address\n",
                           host);
        return 6;
    }
    
    ret = BLGetPreBootEnvironmentType(context, &preboot);
    if(ret)
        return 2;
    
    if(preboot == kBLPreBootEnvType_OpenFirmware) {
        // XXX temporary stub
#define NVRAM "/usr/sbin/nvram"
        char * OFSettings[6];
        
        char bootdevice[1024];
        char bootfile[1024];
        char bootcommand[1024];
        char bootargs[1024];
        
        pid_t p;
        int status;

        sprintf(bootdevice, "boot-device=enet:bootp");
        sprintf(bootfile, "boot-file=");
        sprintf(bootcommand, "boot-command=mac-boot");
        sprintf(bootargs, "boot-args=");
        
        OFSettings[0] = NVRAM;
        OFSettings[1] = bootdevice;
        OFSettings[2] = bootfile;
        OFSettings[3] = bootcommand;
        OFSettings[4] = bootargs;
        OFSettings[5] = NULL;
        
        blesscontextprintf(context, kBLLogLevelVerbose,  "OF Setings:\n" );    
        blesscontextprintf(context, kBLLogLevelVerbose,  "\t\tprogram: %s\n", OFSettings[0] );
        blesscontextprintf(context, kBLLogLevelVerbose,  "\t\t%s\n", OFSettings[1] );
        blesscontextprintf(context, kBLLogLevelVerbose,  "\t\t%s\n", OFSettings[2] );
        blesscontextprintf(context, kBLLogLevelVerbose,  "\t\t%s\n", OFSettings[3] );
        blesscontextprintf(context, kBLLogLevelVerbose,  "\t\t%s\n", OFSettings[4] );
        
        p = fork();
        if (p == 0) {
            int ret = execv(NVRAM, OFSettings);
            if(ret == -1) {
                blesscontextprintf(context, kBLLogLevelError,  "Could not exec %s\n", NVRAM );
            }
            _exit(1);
        }
        
        wait(&status);
        if(status) {
            blesscontextprintf(context, kBLLogLevelError,  "%s returned non-0 exit status\n", NVRAM );
            return 3;
        }
        
    } else if(preboot == kBLPreBootEnvType_BIOS) {
        blesscontextprintf(context, kBLLogLevelError,
                           "NetBoot not supported on BIOS systems\n");
        return 3;
    } else if(preboot == kBLPreBootEnvType_EFI) {
        ret = setefinetworkpath(context, interface,
                                strcmp(host, "255.255.255.255") == 0 ? NULL : host,
                                NULL,
                                actargs[knextonly].present,
                                actargs[koptions].present ? actargs[koptions].argument : NULL);
        
		if(ret) {
			blesscontextprintf(context, kBLLogLevelError,  "Can't set EFI\n" );
			return 1;
		} else {
			blesscontextprintf(context, kBLLogLevelVerbose,  "EFI set successfully\n" );
		}
    }

    return 0;
}

static bool validateAddress(const char *host)
{
    in_addr_t addr;
    int ret;
    
    ret = inet_pton(PF_INET, host, &addr);
    if(ret == 1)
        return true;
    else
        return false;
}

