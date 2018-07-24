/*
 * Copyright (c) 2006 Apple Computer, Inc. All rights reserved.
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
 *  BLCreateBooterInformationDictionary.c
 *  bless
 *
 *  Created by Shantonu Sen on 5/22/06.
 *  Copyright 2006 Apple Computer, Inc. All rights reserved.
 *
 */

#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/mount.h>

#include <mach/mach_error.h>

#include <IOKit/IOKitLib.h>
#include <IOKit/IOBSD.h>
#include <IOKit/storage/IOMedia.h>
#include <IOKit/storage/IOPartitionScheme.h>
#include <IOKit/storage/IOApplePartitionScheme.h>
#include <IOKit/storage/IOGUIDPartitionScheme.h>

#include <CoreFoundation/CoreFoundation.h>

#include "bless.h"
#include "bless_private.h"

static int addRAIDInfo(BLContextPtr context, CFDictionaryRef dict,
            CFMutableSetRef dataPartitions,
            CFMutableSetRef booterPartitions,
            CFMutableSetRef systemPartitions);      

static int addDataPartitionInfo(BLContextPtr context, io_service_t dataPartition,
                       CFMutableSetRef dataPartitions,
                       CFMutableSetRef booterPartitions,
                       CFMutableSetRef systemPartitions);      

static CFArrayRef set2array(CFMutableSetRef setRef);

static CFComparisonResult sortCompare(const void *val1, const void *val2, void *context);

/*
 * For the given device, we return the set of Auxiliary Partitions and
 * System Partitions. System Partitions are not OS-specific. This routine
 * works for both APM and GPT disks, as well as IOMedia filters/aggregates
 * Using those partition map types.
 *
 * A given partition may not have an Auxiliary Partitions nor System Partitions,
 * for example a single HFS+ partition on an APM disk, or it may have many
 * APs and SPs, for example a 4-member RAID mirror on GPT disks.
 */

int BLCreateBooterInformationDictionary(BLContextPtr context, const char * bsdName,
                                        CFDictionaryRef *outDict)
{
    CFMutableSetRef dataPartitions = NULL;
    CFMutableSetRef booterPartitions = NULL;
    CFMutableSetRef systemPartitions = NULL;
    CFMutableDictionaryRef booters = NULL;
    
    CFArrayRef      array;
    
    CFTypeRef               bootData = NULL;
    
    io_service_t            rootDev;
    int                     ret = 0;
    
    dataPartitions = CFSetCreateMutable(kCFAllocatorDefault, 0, &kCFTypeSetCallBacks);
    if(dataPartitions == NULL)
        return 1;

    booterPartitions = CFSetCreateMutable(kCFAllocatorDefault, 0, &kCFTypeSetCallBacks);
    if(booterPartitions == NULL)
        return 1;
    
    systemPartitions = CFSetCreateMutable(kCFAllocatorDefault, 0, &kCFTypeSetCallBacks);
    if(systemPartitions == NULL)
        return 1;
    
    booters = CFDictionaryCreateMutable(kCFAllocatorDefault, 0,
                                        &kCFTypeDictionaryKeyCallBacks,
                                        &kCFTypeDictionaryValueCallBacks);
    if(booters == NULL)
        return 1;
        
    rootDev = IOServiceGetMatchingService(kIOMasterPortDefault,
                                          IOBSDNameMatching(kIOMasterPortDefault, 0, bsdName));
    
    if(rootDev == IO_OBJECT_NULL) {
        CFRelease(booters);
        
        contextprintf(context, kBLLogLevelError,
                      "Could not get IOService for %s\n", bsdName);
        return 2;
    }
    
    if(!IOObjectConformsTo(rootDev,kIOMediaClass)) {
        CFRelease(booters);
        IOObjectRelease(rootDev);
        
        contextprintf(context, kBLLogLevelError,
                      "%s is not an IOMedia object\n", bsdName);
        return 2;        
    }
    
    bootData = IORegistryEntrySearchCFProperty( rootDev,
                                            kIOServicePlane,
                                            CFSTR(kIOBootDeviceKey),
                                            kCFAllocatorDefault,
                                            kIORegistryIterateRecursively|
                                            kIORegistryIterateParents);
    
	if(bootData) {

        // if there's boot data, this is an IOMedia filter/aggregate publishing
        // its data members
        if(CFGetTypeID(bootData) == CFArrayGetTypeID()) {
            CFIndex i, count = CFArrayGetCount(bootData);
            
            for(i=0; i < count; i++) {
                CFDictionaryRef dict = CFArrayGetValueAtIndex((CFArrayRef)bootData,i);
                
                ret = addRAIDInfo(context, dict,
                                  dataPartitions,
                                  booterPartitions,
                                  systemPartitions);            
                if(ret) {
                    break;
                }
                
            }
        } else if( CFGetTypeID(bootData) == CFDictionaryGetTypeID()) {
            
            ret = addRAIDInfo(context, (CFDictionaryRef)bootData,
                              dataPartitions,
                              booterPartitions,
                              systemPartitions);            
        } else {
            contextprintf(context, kBLLogLevelError,
                          "Invalid boot data for %s\n", bsdName);

            ret = 5;;
        }
        
        if(ret) {
            CFRelease(booters);
            IOObjectRelease(rootDev);

            return ret;
            
        }
		
	} else {
        ret = addDataPartitionInfo(context, rootDev,
                                   dataPartitions,
                                   booterPartitions,
                                   systemPartitions);    
        if(ret) {
            CFRelease(booters);
            IOObjectRelease(rootDev);
            
            return ret;
        }
    }
	

    IOObjectRelease(rootDev);
    
    array = set2array(dataPartitions);
    CFDictionaryAddValue(booters, kBLDataPartitionsKey, array);
    CFRelease(array);
    CFRelease(dataPartitions);
    
    array = set2array(booterPartitions);
    CFDictionaryAddValue(booters, kBLAuxiliaryPartitionsKey, array);
    CFRelease(array);
    CFRelease(booterPartitions);
    
    array = set2array(systemPartitions);
    CFDictionaryAddValue(booters, kBLSystemPartitionsKey, array);
    CFRelease(array);
    CFRelease(systemPartitions);
    
	contextprintf(context, kBLLogLevelVerbose, "Returning booter information dictionary:\n%s\n",
				  BLGetCStringDescription(booters));
	
    *outDict = booters;
    
    return 0;
}

static int addRAIDInfo(BLContextPtr context, CFDictionaryRef dict,
                       CFMutableSetRef dataPartitions,
                       CFMutableSetRef booterPartitions,
                       CFMutableSetRef systemPartitions)
{
    CFStringRef bootpath = NULL;
    io_string_t iostring;
    io_service_t    service;

    int         ret;
    
    bootpath = CFDictionaryGetValue(dict, CFSTR(kIOBootDevicePathKey));
    if(bootpath == NULL || CFGetTypeID(bootpath) != CFStringGetTypeID()) {
        contextprintf(context, kBLLogLevelError,  "Could not find boot path entry\n");
        return 1;            
    }
    
    if(!CFStringGetCString(bootpath,iostring,sizeof(iostring),kCFStringEncodingUTF8)) {
        contextprintf(context, kBLLogLevelError,  "Invalid UTF8 for path entry\n");
        return 2;                        
    }
    
    contextprintf(context, kBLLogLevelVerbose,  "Aggregate boot path is %s\n" , iostring);
    
    service = IORegistryEntryFromPath(kIOMasterPortDefault, iostring );
    if(service == IO_OBJECT_NULL) {
        contextprintf(context, kBLLogLevelError,  "Could not find IOKit entry for %s\n" , iostring);
        return 4;                                    
    }

    ret = addDataPartitionInfo(context, service,
                               dataPartitions,
                               booterPartitions,
                               systemPartitions);      
    
    IOObjectRelease(service);
    
    return ret;
}

static int addDataPartitionInfo(BLContextPtr context, io_service_t dataPartition,
                                CFMutableSetRef dataPartitions,
                                CFMutableSetRef booterPartitions,
                                CFMutableSetRef systemPartitions)
{
    CFStringRef bsdName;
    kern_return_t   kret;
    io_service_t    parent;
    uint32_t        partitionID, searchID;
    CFNumberRef     partitionNum;
    CFStringRef     content;
    bool            needsBooter = false;
    CFNumberRef     neededBooterPartitionNum = NULL;
    CFStringRef     neededBooterContent = NULL;
    CFStringRef     neededSystemContent = NULL;
    
    io_iterator_t   childIterator;
    io_service_t    child;
    
    /* don't require this at this point
    kret = IORegistryEntryGetPath(dataPartition, kIODeviceTreePlane, devPath);
	if(kret != KERN_SUCCESS) {
		contextprintf(context, kBLLogLevelError,  "Could not get path in device plane for service\n" );
		return 1;
	}
    */
    
    bsdName = IORegistryEntryCreateCFProperty(dataPartition, CFSTR(kIOBSDNameKey), kCFAllocatorDefault, 0);
    if(bsdName == NULL || (CFGetTypeID(bsdName) != CFStringGetTypeID())) {
        if(bsdName) CFRelease(bsdName);
        
        return 1;
    }

    partitionNum = IORegistryEntryCreateCFProperty(dataPartition, CFSTR(kIOMediaPartitionIDKey), kCFAllocatorDefault, 0);
    if(partitionNum == NULL || (CFGetTypeID(partitionNum) != CFNumberGetTypeID())) {
        if(partitionNum) CFRelease(partitionNum);
        
        return 1;
    }
    
    content = (CFStringRef)IORegistryEntryCreateCFProperty(dataPartition, CFSTR(kIOMediaContentKey), kCFAllocatorDefault, 0);
    if(content == NULL || (CFGetTypeID(content) != CFStringGetTypeID())) {
        if(content) CFRelease(content);
        contextprintf(context, kBLLogLevelError,  "Partition does not have Content key\n" );
        
        return 1;
    }
    
    
    if(!CFNumberGetValue(partitionNum,kCFNumberSInt32Type, &partitionID)) {
        contextprintf(context, kBLLogLevelError,  "Could not get Partition ID for service\n" );
		return 1;        
    }
    
    CFSetAddValue(dataPartitions, bsdName);
    CFRelease(bsdName);
    CFRelease(partitionNum);
    
    kret = IORegistryEntryGetParentEntry(dataPartition, kIOServicePlane, &parent);
    if(kret != KERN_SUCCESS) {
        contextprintf(context, kBLLogLevelError,  "Could not get parent path in device plane for service\n" );
		return 1;
    }
    
        
    if(IOObjectConformsTo(parent, kIOApplePartitionSchemeClass)) {
        contextprintf(context, kBLLogLevelVerbose,  "APM detected\n" );
        // from the OS point of view, only it's an HFS or boot partition, it needs a booter

        if(CFEqual(content, CFSTR("Apple_HFS"))  ||
           CFEqual(content, CFSTR("Apple_Boot")) ||
           CFEqual(content, CFSTR("Apple_Boot_RAID")) ) {
            needsBooter = false;
        } else {
            needsBooter = true;
            searchID = partitionID - 1;
            neededBooterContent = CFSTR("Apple_Boot");
            neededBooterPartitionNum = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &searchID);
        }
        
    } else if(IOObjectConformsTo(parent, kIOGUIDPartitionSchemeClass)) {
        contextprintf(context, kBLLogLevelVerbose,  "GPT detected\n" );
        
        // from the OS point of view, only it's an HFS or boot partition, it needs a booter
        if(CFEqual(content, CFSTR("48465300-0000-11AA-AA11-00306543ECAC"))  ||
           CFEqual(content, CFSTR("426F6F74-0000-11AA-AA11-00306543ECAC"))) {
            needsBooter = false;
        } else {
            needsBooter = true;
            searchID = partitionID + 1;
            neededBooterContent = CFSTR("426F6F74-0000-11AA-AA11-00306543ECAC");
            neededBooterPartitionNum = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &searchID);
        }
        
        neededSystemContent = CFSTR("C12A7328-F81F-11D2-BA4B-00A0C93EC93B");
        
    } else {
        contextprintf(context, kBLLogLevelVerbose,  "Other partition scheme detected\n" );
    }
    
    if(needsBooter) {
        contextprintf(context, kBLLogLevelVerbose,  "Booter partition required at index %u\n", searchID);        
    } else {
        contextprintf(context, kBLLogLevelVerbose,  "No auxiliary booter partition required\n");                
    }
    
    if(needsBooter || neededSystemContent) {
        kret = IORegistryEntryGetChildIterator(parent, kIOServicePlane, &childIterator);
        if(kret != KERN_SUCCESS) {
            contextprintf(context, kBLLogLevelError,  "Could not get child iterator for parent\n" );
            return 4;
        }
        
        while((child = IOIteratorNext(childIterator)) != IO_OBJECT_NULL) {
            CFStringRef childContent;
            
            childContent = IORegistryEntryCreateCFProperty(child, CFSTR(kIOMediaContentKey), kCFAllocatorDefault, 0);
            if(childContent && CFGetTypeID(childContent) == CFStringGetTypeID()) {
                CFStringRef childBSDName;
                // does it match
                if(needsBooter && CFEqual(childContent, neededBooterContent)) {
                    CFNumberRef childPartitionID = IORegistryEntryCreateCFProperty(child, CFSTR(kIOMediaPartitionIDKey), kCFAllocatorDefault, 0);
                    
                    if(childPartitionID && (CFGetTypeID(childPartitionID) == CFNumberGetTypeID()) && CFEqual(childPartitionID, neededBooterPartitionNum)) {
                        childBSDName = IORegistryEntryCreateCFProperty(child, CFSTR(kIOBSDNameKey), kCFAllocatorDefault, 0);
                        if(childBSDName && (CFGetTypeID(childBSDName) == CFStringGetTypeID())) {
                            CFSetAddValue(booterPartitions, childBSDName);
                        }
                        CFRelease(childBSDName);
                        contextprintf(context, kBLLogLevelVerbose,  "Booter partition found\n" );                        
                    }
                } else if(neededSystemContent && CFEqual(childContent, neededSystemContent)) {
                    childBSDName = IORegistryEntryCreateCFProperty(child, CFSTR(kIOBSDNameKey), kCFAllocatorDefault, 0);
                    if(childBSDName && (CFGetTypeID(childBSDName) == CFStringGetTypeID())) {
                        CFSetAddValue(systemPartitions, childBSDName);
                    }
                    CFRelease(childBSDName);
                    contextprintf(context, kBLLogLevelVerbose,  "System partition found\n" );                    
                }
            }
            
            if(childContent) CFRelease(childContent);
            
            IOObjectRelease(child);
        }
        
        IOObjectRelease(childIterator);
        
    }
    
    IOObjectRelease(parent);
    
    return 0;
}

static CFArrayRef set2array(CFMutableSetRef setRef)
{
    CFIndex count = CFSetGetCount(setRef);
    const void    *values[count];
    CFArrayRef   array;
    CFMutableArrayRef mutArray;
    
    CFSetGetValues(setRef, values);
    
    array = CFArrayCreate(kCFAllocatorDefault, values, count, &kCFTypeArrayCallBacks);
    
    mutArray = CFArrayCreateMutableCopy(kCFAllocatorDefault, count, array);
    
    CFRelease(array);
    
    CFArraySortValues(mutArray, CFRangeMake(0, count), sortCompare, NULL);
    
    return mutArray;
}

static CFComparisonResult sortCompare(const void *val1, const void *val2, void *context)
{
    return CFStringCompare((CFStringRef)val1, (CFStringRef)val2, 0);
}
