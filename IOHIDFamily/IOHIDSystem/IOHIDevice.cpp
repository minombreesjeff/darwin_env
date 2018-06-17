/*
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
 */
#include <libkern/c++/OSCollectionIterator.h>
#include <IOKit/assert.h>
#include <IOKit/IOLib.h>
#include <IOKit/IOService.h>
#include <IOKit/hidsystem/IOHIDevice.h>
#include <IOKit/hidsystem/IOHIDParameter.h>
#include "IOHIDevicePrivateKeys.h"

#define super IOService
OSDefineMetaClassAndStructors(IOHIDevice, IOService);

bool IOHIDevice::init(OSDictionary * properties)
{
  if (!super::init(properties))  return false;

  /*
   * Initialize minimal state.
   */

  return true;
}

void IOHIDevice::free()
{
  super::free();
}

bool IOHIDevice::start(IOService * provider) 
{
    if (!super::start(provider))
        return false;
   
    // RY: If the kIOHIDVirtualHIDevice property isn't
    // set scan the up provider chain to determine if
    // this is a resource or if the provider is another
    // IOHIDevice.  Also propegate value is property
    // was already set in provider.
    if (!getProperty(kIOHIDVirtualHIDevice))
    {
        OSObject * prop;

        while (provider)
        {
            if ( prop = OSDynamicCast(OSBoolean, provider->getProperty(kIOHIDVirtualHIDevice)) )
            {
                setProperty(kIOHIDVirtualHIDevice, prop);
                break;
            }
            else if ( provider == getResourceService() || OSDynamicCast(IOHIDevice, provider) )
            {
                setProperty(kIOHIDVirtualHIDevice, kOSBooleanTrue);
                break;
            }

            provider = provider->getProvider();
        }
        
        if ( !provider )
            setProperty(kIOHIDVirtualHIDevice, kOSBooleanFalse);
    }
    
    updateProperties();
    
    return true;
}

bool IOHIDevice::open(IOService *    forClient,
                      IOOptionBits   options,
                      void *         arg)
{
    if(forClient == this) return true;
    
    return super::open(forClient, options, arg);
}


IOHIDKind IOHIDevice::hidKind()
{
  return kHIUnknownDevice;
}

UInt32 IOHIDevice::interfaceID()
{
  return 0;
}

UInt32 IOHIDevice::deviceType()
{
  return 0;
}

UInt64 IOHIDevice::getGUID()
{
  return(0xffffffffffffffffULL);
}

bool IOHIDevice::updateProperties( void )
{
    bool ok;

    ok = setProperty( kIOHIDKindKey, hidKind(), 32 )
    &    setProperty( kIOHIDInterfaceIDKey, interfaceID(), 32 )
    &    setProperty( kIOHIDSubinterfaceIDKey, deviceType(), 32 );

    return( ok );
}

// RY: Override IORegistryEntry::setProperties().  This will allow properties 
// to be set per device, instead of globally via setParamProperties.
IOReturn IOHIDevice::setProperties( OSObject * properties )
{
    OSDictionary * propertyDict = OSDynamicCast(OSDictionary, properties);
    IOReturn       ret          = kIOReturnBadArgument;
    
    if ( propertyDict ) {
        propertyDict->setObject(kIOHIDDeviceParametersKey, kOSBooleanTrue);
        ret = setParamProperties( propertyDict );
        propertyDict->removeObject(kIOHIDDeviceParametersKey);
    }

    return ret;
}


IOReturn IOHIDevice::setParamProperties( OSDictionary * dict )
{
    IOService *    eventService             = NULL;
    OSDictionary * eventServiceProperties   = NULL;
    
    if ( dict->getObject(kIOHIDEventServicePropertiesKey) == NULL ) {
        IOService * eventService = getProvider();            
        if ( eventService && eventService->metaCast("IOHIDEventService"))
            eventServiceProperties = OSDynamicCast(OSDictionary, eventService->copyProperty(kIOHIDEventServicePropertiesKey));
    }

    if ( dict->getObject(kIOHIDDeviceParametersKey) == kOSBooleanTrue ) {
        OSDictionary * deviceParameters = OSDynamicCast(OSDictionary, copyProperty(kIOHIDParametersKey));
        
        if ( !deviceParameters )
            deviceParameters = OSDictionary::withCapacity(4);
    
        if ( deviceParameters ) {
            // RY: Because K&M Prefs and Admin still expect device props to be
            // top level, let's continue to set them via setProperty. When we get
            // Max to migrate over, we can remove the interator code and use:
            // deviceParameters->merge(dict);    
            // deviceParameters->removeObject(kIOHIDResetKeyboardKey);
            // deviceParameters->removeObject(kIOHIDResetPointerKey);
            // setProperty(kIOHIDParametersKey, deviceParameters);
            // deviceParameters->release();
 
            OSCollectionIterator * iterator = OSCollectionIterator::withCollection(dict);
            if ( iterator ) {
                OSSymbol * key;
                
                while ( key = (OSSymbol *)iterator->getNextObject() )
                    if ( !key->isEqualTo(kIOHIDResetKeyboardKey) && !key->isEqualTo(kIOHIDResetPointerKey) && !key->isEqualTo(kIOHIDDeviceParametersKey)) {
                        OSObject * value = dict->getObject(key);
                        
                        deviceParameters->setObject(key, value);
                        setProperty(key, value);                        
                    }

                iterator->release();
            }
            
            setProperty(kIOHIDParametersKey, deviceParameters);
            deviceParameters->release();
            
            // RY: Propogate up to IOHIDEventService level
            if ( eventServiceProperties ) {
                eventServiceProperties->merge(dict);
                eventServiceProperties->removeObject(kIOHIDResetKeyboardKey);
                eventServiceProperties->removeObject(kIOHIDResetPointerKey);
                eventServiceProperties->removeObject(kIOHIDEventServicePropertiesKey);
                eventService->setProperty(kIOHIDEventServicePropertiesKey, eventServiceProperties);
                eventServiceProperties->release();
            }
            
        }
    }
    
    return( kIOReturnSuccess );
}


