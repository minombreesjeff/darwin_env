/*
 * Copyright (c) 2005-2006 Apple Computer, Inc. All rights reserved.
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
 *  BLCreateEFIXMLRepresentationForLegacyDevice.c
 *  bless
 *
 *  Created by Shantonu Sen on 1/24/06.
 *  Copyright 2006 Apple Computer, Inc. All rights reserved.
 *
 */

#include <IOKit/IOKitLib.h>
#include <IOKit/IOCFSerialize.h>
#include <IOKit/IOBSD.h>
#include <IOKit/IOKitKeys.h>
#include <IOKit/storage/IOMedia.h>
#include <IOKit/usb/IOUSBLib.h>
#include <IOKit/storage/IOStorageProtocolCharacteristics.h>

#include <DiskArbitration/DiskArbitration.h>
#include <CoreFoundation/CoreFoundation.h>

#include <string.h>
#include <sys/param.h>
#include <sys/stat.h>

#include "bless.h"
#include "bless_private.h"

static int addLegacyTypeForBSDName(BLContextPtr context,
									 mach_port_t masterPort,
									 CFMutableDictionaryRef dict,
									 const char *bsdName);

int BLCreateEFIXMLRepresentationForLegacyDevice(BLContextPtr context,
										  const char *bsdName,
										  CFStringRef *xmlString)
{
    mach_port_t masterPort;
    kern_return_t kret;
    int ret;
    
    CFDataRef xmlData;
    CFMutableDictionaryRef dict;
    CFMutableArrayRef array;
    CFNumberRef number;
    uint64_t    num64;
    
    const UInt8 *xmlBuffer;
    UInt8 *outBuffer;
    CFIndex count;
    
	if(!BLSupportsLegacyMode(context)) {
        contextprintf(context, kBLLogLevelError, "Legacy mode not supported on this system\n");		
		return 1;
	}
	
    kret = IOMasterPort(MACH_PORT_NULL, &masterPort);
    if(kret) return 1;
    
    array = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);
    
    dict = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, 
                                     &kCFTypeDictionaryKeyCallBacks,
                                     &kCFTypeDictionaryValueCallBacks);
    
    CFDictionaryAddValue(dict, CFSTR("IOEFIDevicePathType"),
                         CFSTR("HardwareMemoryMapped"));

    num64 = 0xb;
    number = CFNumberCreate(kCFAllocatorDefault,kCFNumberSInt64Type, &num64);
    CFDictionaryAddValue(dict, CFSTR("MemoryType"),
                         number);
    CFRelease(number);

    num64 = 0xffe00000ULL;
    number = CFNumberCreate(kCFAllocatorDefault,kCFNumberSInt64Type, &num64);
    CFDictionaryAddValue(dict, CFSTR("StartingAddress"),
                         number);
    CFRelease(number);

    num64 = 0xfff9ffffULL;
    number = CFNumberCreate(kCFAllocatorDefault,kCFNumberSInt64Type, &num64);
    CFDictionaryAddValue(dict, CFSTR("EndingAddress"),
                         number);
    CFRelease(number);
    
    CFArrayAppendValue(array, dict);
    CFRelease(dict);
    
    
    dict = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, 
                                     &kCFTypeDictionaryKeyCallBacks,
                                     &kCFTypeDictionaryValueCallBacks);
    
    CFDictionaryAddValue(dict, CFSTR("IOEFIDevicePathType"),
                         CFSTR("MediaFirmwareVolumeFilePath"));
    CFDictionaryAddValue(dict, CFSTR("Guid"),
                         CFSTR("2B0585EB-D8B8-49A9-8B8C-E21B01AEF2B7"));

    CFArrayAppendValue(array, dict);
    CFRelease(dict);

    
    
    dict = CFDictionaryCreateMutable(kCFAllocatorDefault, 0,
                                     &kCFTypeDictionaryKeyCallBacks,
                                     &kCFTypeDictionaryValueCallBacks);
    ret = addLegacyTypeForBSDName(context,
                                  masterPort,
                                  dict,
                                  bsdName);
    if(ret) {
        CFRelease(dict);
        CFRelease(array);
        contextprintf(context, kBLLogLevelError, "Can't determine legacy media type for %s\n", bsdName);
        return 2;
    }
    
    CFArrayAppendValue(array, dict);
    CFRelease(dict);        
            
    xmlData = IOCFSerialize(array, 0);
    CFRelease(array);
    
    if(xmlData == NULL) {
        contextprintf(context, kBLLogLevelError, "Can't create XML representation\n");
        return 2;
    }
    
    count = CFDataGetLength(xmlData);
    xmlBuffer = CFDataGetBytePtr(xmlData);
    outBuffer = calloc(count+1, sizeof(char)); // terminate
    
    memcpy(outBuffer, xmlBuffer, count);
    CFRelease(xmlData);
    
    *xmlString = CFStringCreateWithCString(kCFAllocatorDefault, (const char *)outBuffer, kCFStringEncodingUTF8);
    
    free(outBuffer);
    
    return 0;
}

static int addLegacyTypeForBSDName(BLContextPtr context,
                                   mach_port_t masterPort,
                                   CFMutableDictionaryRef dict,
                                   const char *bsdName)
{
    io_service_t                service = IO_OBJECT_NULL, media;
    io_iterator_t               iter;
    kern_return_t               kret;
    int                         spaces = 0;
    bool                        foundUSB = false;
    bool                        foundATAPI = false;
    CFStringRef                 type;
    CFDictionaryRef             protocolCharacteristics;
    
    media = IOServiceGetMatchingService(masterPort,
                                        IOBSDNameMatching(masterPort, 0, bsdName));

    if(media == IO_OBJECT_NULL) {
        contextprintf(context, kBLLogLevelError, "Could not find object for %s\n", bsdName);
        return 1;
    }
    
        
    protocolCharacteristics = IORegistryEntrySearchCFProperty(media,
                                                              kIOServicePlane,
                                                              CFSTR(kIOPropertyProtocolCharacteristicsKey),
                                                              kCFAllocatorDefault,
                                                              kIORegistryIterateRecursively|
                                                                kIORegistryIterateParents);
    
    if(protocolCharacteristics && CFGetTypeID(protocolCharacteristics) == CFDictionaryGetTypeID()) {
        CFStringRef interconnect = CFDictionaryGetValue(protocolCharacteristics,
                                                        CFSTR(kIOPropertyPhysicalInterconnectTypeKey));
        if(interconnect && CFGetTypeID(interconnect) == CFStringGetTypeID()) {
            if(CFEqual(interconnect, CFSTR(kIOPropertyPhysicalInterconnectTypeUSB))) {
                contextprintf(context, kBLLogLevelVerbose,
                              "Found USB interconnect in protocol characteristics\n");        
                foundUSB = true;
            } else if(CFEqual(interconnect, CFSTR(kIOPropertyPhysicalInterconnectTypeATAPI))) {
                contextprintf(context, kBLLogLevelVerbose,
                              "Found ATAPI interconnect in protocol characteristics\n");        
                foundATAPI = true;
            }
            //otherwise assume it's an HD-class device
        }
        
    }
    
    if(protocolCharacteristics) CFRelease(protocolCharacteristics);
    
    
    if(!foundUSB && !foundATAPI) {
        // try to use the registry topology to see if it's a USB device
        
        kret = IORegistryEntryCreateIterator (media, kIOServicePlane,
                                              kIORegistryIterateRecursively|kIORegistryIterateParents,
                                              &iter);

        if(kret) {
            contextprintf(context, kBLLogLevelError, "Could not get parent iterator for %s\n", bsdName);
            IOObjectRelease(media);
            return 2;
        }
        
        IOObjectRelease(media);

        while ( (service = IOIteratorNext(iter)) != IO_OBJECT_NULL ) {
            io_name_t name;
            
            kret = IORegistryEntryGetNameInPlane(service, kIOServicePlane, name);
            if(kret) strcpy(name, "unknown");
            contextprintf(context, kBLLogLevelVerbose, "%*s%s\n", spaces, "", name);        
            
            if(IOObjectConformsTo(service, kIOUSBInterfaceClassName)) {
                contextprintf(context, kBLLogLevelVerbose,
                              "Found %s in parent hierarchy\n", kIOUSBInterfaceClassName);        
                foundUSB = true;
                IOObjectRelease(service);    
                break;
            }
            
            spaces++;
            IOObjectRelease(service);    
        }
        IOObjectRelease(iter);
    }
    
    if(foundUSB) {
        type = CFSTR("USB");
    } else if(foundATAPI) {
        type = CFSTR("CD");
    } else {
        type = CFSTR("HD");
    }
    
    CFDictionaryAddValue(dict, CFSTR("IOEFIBootOption"),
                         type);

    return 0;
}
