/*
 *  KeyLargoPlatform.cpp
 *  
 *  Created by Aram Lindahl on Mon Mar 10 2003.
 *  Copyright (c) 2003 Apple Computer. All rights reserved.
 *
 */

#include "KeyLargoPlatform.h"

#include "AudioHardwareConstants.h"
#include "AudioHardwareUtilities.h"
#include "AudioHardwareCommon.h"
#include "AppleOnboardAudio.h"

#define super PlatformInterface

OSDefineMetaClassAndStructors(KeyLargoPlatform, PlatformInterface)

const UInt32 KeyLargoPlatform::kFCR0Offset					= 0x00000038;
const UInt32 KeyLargoPlatform::kFCR1Offset					= 0x0000003C;
const UInt32 KeyLargoPlatform::kFCR2Offset					= 0x00000040;
const UInt32 KeyLargoPlatform::kFCR3Offset					= 0x00000044;
const UInt32 KeyLargoPlatform::kFCR4Offset					= 0x00000048;

const UInt16 KeyLargoPlatform::kAPPLE_IO_CONFIGURATION_SIZE = 256;
const UInt16 KeyLargoPlatform::kI2S_IO_CONFIGURATION_SIZE	= 256;

const UInt32 KeyLargoPlatform::kI2S0BaseOffset				= 0x10000;							/*	mapped by AudioI2SControl	*/
const UInt32 KeyLargoPlatform::kI2S1BaseOffset				= 0x11000;							/*	mapped by AudioI2SControl	*/

const UInt32 KeyLargoPlatform::kI2SIntCtlOffset				= 0x0000;
const UInt32 KeyLargoPlatform::kI2SSerialFormatOffset		= 0x0010;
const UInt32 KeyLargoPlatform::kI2SCodecMsgOutOffset		= 0x0020;
const UInt32 KeyLargoPlatform::kI2SCodecMsgInOffset			= 0x0030;
const UInt32 KeyLargoPlatform::kI2SFrameCountOffset			= 0x0040;
const UInt32 KeyLargoPlatform::kI2SFrameMatchOffset			= 0x0050;
const UInt32 KeyLargoPlatform::kI2SDataWordSizesOffset		= 0x0060;
const UInt32 KeyLargoPlatform::kI2SPeakLevelSelOffset		= 0x0070;
const UInt32 KeyLargoPlatform::kI2SPeakLevelIn0Offset		= 0x0080;
const UInt32 KeyLargoPlatform::kI2SPeakLevelIn1Offset		= 0x0090;

const UInt32 KeyLargoPlatform::kI2SClockOffset				= 0x0003C;							/*	FCR1 offset (not mapped by AudioI2SControl)	*/
const UInt32 KeyLargoPlatform::kI2S0ClockEnable 		    = ( 1 << kI2S0ClkEnBit );
const UInt32 KeyLargoPlatform::kI2S1ClockEnable 			= ( 1 << kI2S1ClkEnBit );
const UInt32 KeyLargoPlatform::kI2S0CellEnable	 		    = ( 1 << kI2S0CellEn );
const UInt32 KeyLargoPlatform::kI2S1CellEnable	 			= ( 1 << kI2S1CellEn );
const UInt32 KeyLargoPlatform::kI2S0InterfaceEnable 		= ( 1 << kI2S0Enable );
const UInt32 KeyLargoPlatform::kI2S1InterfaceEnable 		= ( 1 << kI2S1Enable );

const char*  KeyLargoPlatform::kHeadphoneAmpEntry 			= "headphone-mute";
const char*  KeyLargoPlatform::kAmpEntry					= "amp-mute";
const char*  KeyLargoPlatform::kSpeakerIDEntry				= "speaker-id";
const char*  KeyLargoPlatform::kLineOutAmpEntry				= "line-output-mute";
const char*  KeyLargoPlatform::kAnalogHWResetEntry			= "audio-hw-reset";
const char*  KeyLargoPlatform::kDigitalHWResetEntry			= "audio-dig-hw-reset";
const char*  KeyLargoPlatform::kHeadphoneDetectInt			= "headphone-detect";
const char*  KeyLargoPlatform::kLineInDetectInt				= "line-input-detect";
const char*  KeyLargoPlatform::kLineOutDetectInt			= "line-output-detect";

const char*  KeyLargoPlatform::kNumInputs					= "#-inputs";
const char*  KeyLargoPlatform::kI2CAddress					= "i2c-address";
const char*  KeyLargoPlatform::kAudioGPIO					= "audio-gpio";
const char*  KeyLargoPlatform::kAudioGPIOActiveState		= "audio-gpio-active-state";
const char*  KeyLargoPlatform::kIOInterruptControllers		= "IOInterruptControllers";

#pragma mark ---------------------------
#pragma mark ¥ Platform Interface Methods
#pragma mark ---------------------------

//	--------------------------------------------------------------------------------
bool KeyLargoPlatform::init (IOService* device, AppleOnboardAudio* provider, UInt32 inDBDMADeviceIndex)
{
	Boolean					result;

	IOService* 				theService;

	IORegistryEntry			*sound;
	IORegistryEntry			*macio;
	IORegistryEntry			*gpio;
	IORegistryEntry			*i2s;
	IORegistryEntry			*i2sParent;
    IORegistryEntry			*headphoneMute;
    IORegistryEntry			*speakerID;
    IORegistryEntry			*ampMute;
	IORegistryEntry			*lineOutMute;
	IORegistryEntry			*analogCodecReset;
	IORegistryEntry			*digitalCodecReset;
	IORegistryEntry			*intSource;
	
	UInt32					*hdpnMuteGpioAddr;
	UInt32					*ampMuteGpioAddr;
	UInt32					*speakerIDGpioAddr;
	UInt32					*headphoneExtIntGpioAddr;
	UInt32					*lineOutExtIntGpioAddr;
//	UInt32					*lineInExtIntGpioAddr;
//	UInt32					*codecExtIntGpioAddr;
//	UInt32					*codecErrorExtIntGpioAddr;
	UInt32					*tmpPtr;
	UInt32					*lineOutMuteGpioAddr;
	UInt32					*hwAnalogResetGpioAddr;
	UInt32					*hwDigitalResetGpioAddr;

	IODeviceMemory			*hdpnMuteRegMem;
	IODeviceMemory			*lineOutMuteGpioMem;
	IODeviceMemory			*hwAnalogResetRegMem;
	IODeviceMemory			*hwDigitalResetRegMem;
	IODeviceMemory			*ampMuteRegMem;
	IODeviceMemory			*speakerIDRegMem;
	IODeviceMemory			*headphoneExtIntGpioMem;
	IODeviceMemory			*lineOutExtIntGpioMem;
//	IODeviceMemory			*lineInExtIntGpioMem;
//	IODeviceMemory			*codecExtIntGpioMem;
//	IODeviceMemory			*codecErrorExtIntGpioMem;

	OSData					*tmpData;
	IOMemoryMap				*map;
	UInt8					curValue;

	OSDictionary*			keyLargoDictionary;

	debug2IOLog ("+ KeyLargoPlatform::init - device = %p\n", device);

	result = super::init (device, provider, inDBDMADeviceIndex);
	if (!result)
		return result;

	keyLargoDictionary = IOService::serviceMatching ("KeyLargo");
	// use to call platform functions 
	mKeyLargoService = IOService::waitForService (keyLargoDictionary);
	// could retain here...

	debug2IOLog ("mKeyLargoService = %p\n", mKeyLargoService);
	FailWithAction (NULL == mKeyLargoService, result = false, Exit);
	
	//  mac-io : is2 : i2s-x : sound : AOA

	sound = device;

	FailWithAction (!sound, result = false, Exit);
	debug2IOLog ("KeyLargoPlatform - sound's name is %s\n", sound->getName ());

	i2s = sound->getParentEntry (gIODTPlane);
	FailWithAction (!i2s, result = false, Exit);
	debug2IOLog ("KeyLargoPlatform - i2s's name is %s\n", i2s->getName ());

	i2sParent = i2s->getParentEntry (gIODTPlane);
	FailWithAction (!i2sParent, result = false, Exit);
	debug2IOLog ("KeyLargoPlatform - i2s's name is %s\n", i2sParent->getName ());

	macio = i2sParent->getParentEntry (gIODTPlane);
	FailWithAction (!macio, result = false, Exit);
	debug2IOLog ("KeyLargoPlatform - macio's name is %s\n", macio->getName ());
	
	gpio = macio->childFromPath (kGPIODTEntry, gIODTPlane);
	FailWithAction (!gpio, result = false, Exit);
	debug2IOLog ("KeyLargoPlatform - gpio's name is %s\n", gpio->getName ());

	
	FailWithAction (!findAndAttachI2C(), result = false, Exit);

	
	tmpData = OSDynamicCast(OSData, sound->getProperty("AAPL,i2c-port-select"));		
	if (tmpData != NULL) {
		mI2CPort = *((UInt32*)tmpData->getBytesNoCopy());
	}
	debug2IOLog ("KeyLargoPlatform - mI2CPort = %ld\n", mI2CPort);

	// get the physical address of the gpio pin for setting the headphone mute
	headphoneMute = FindEntryByProperty (gpio, kAudioGPIO, kHeadphoneAmpEntry);
	FailWithAction (!headphoneMute, result = false, Exit);
	tmpData = OSDynamicCast (OSData, headphoneMute->getProperty (kAAPLAddress));
	FailWithAction (!tmpData, result = false, Exit);
	hdpnMuteGpioAddr = (UInt32*)tmpData->getBytesNoCopy ();
	tmpData = OSDynamicCast (OSData, headphoneMute->getProperty (kAudioGPIOActiveState));
	if (tmpData) {
		tmpPtr = (UInt32*)tmpData->getBytesNoCopy ();
		hdpnActiveState = *tmpPtr;
		debug2IOLog ("KeyLargoPlatform - hdpnActiveState = 0x%X\n", hdpnActiveState);
	}
	debug2IOLog ("KeyLargoPlatform - hdpnMuteGpioAddr = %p\n", hdpnMuteGpioAddr);

	// take the hard coded memory address that's in the boot rom, and convert it a virtual address
	if (hdpnMuteGpioAddr) {
		debug2IOLog ("KeyLargoPlatform - hdpnMuteGpioAddr = %p\n", hdpnMuteGpioAddr);
		hdpnMuteRegMem = IODeviceMemory::withRange (*hdpnMuteGpioAddr, sizeof (UInt8));
		map = hdpnMuteRegMem->map (0);
		hdpnMuteGpio = (UInt8*)map->getVirtualAddress ();
	}
	debug2IOLog ("KeyLargoPlatform - hdpnMuteGpio = %p\n", hdpnMuteGpio);
	
	
	speakerID = FindEntryByProperty (gpio, kAudioGPIO, kSpeakerIDEntry);
	if ( speakerID ) {
		tmpData = OSDynamicCast ( OSData, speakerID->getProperty ( kAAPLAddress ) );
		if ( tmpData ) {
			speakerIDGpioAddr = (UInt32*)tmpData->getBytesNoCopy();
            if ( speakerIDGpioAddr ) {
                debug2IOLog ("KeyLargoPlatform - speakerIDGpioAddr = %p\n", speakerIDGpioAddr);
                tmpData = OSDynamicCast ( OSData, speakerID->getProperty ( kAudioGPIOActiveState ) );
                if ( tmpData ) {
                    tmpPtr = (UInt32*)tmpData->getBytesNoCopy();
                    speakerIDActiveState = *tmpPtr;
                    debug2IOLog ("KeyLargoPlatform - speakerIDActiveState = 0x%X\n", speakerIDActiveState);
                    //	Take the hard coded memory address that's in the boot rom and convert it to a virtual address
                    speakerIDRegMem = IODeviceMemory::withRange ( *speakerIDGpioAddr, sizeof ( UInt8 ) );
                    map = speakerIDRegMem->map ( 0 );
                    speakerIDGpio = (UInt8*)map->getVirtualAddress();
                }
            }
		}
	}	
	debug2IOLog ("KeyLargoPlatform - speakerIDGpio = %p\n", speakerIDGpio);	
	
	//	[2855519]	begin {
	//	Locate a line output amplifier mute control if one exists and setup to manage the control...
	//	This is an optional control that may not exist on all CPU configurations so conditional
	//	execution is appropriate but don't FailIf out of the initialization process if the
	//	line output mute control does not exist.
	
	lineOutMute = FindEntryByProperty ( gpio, kAudioGPIO, kLineOutAmpEntry );
	if ( lineOutMute ) {
		tmpData = OSDynamicCast ( OSData, lineOutMute->getProperty ( kAAPLAddress ) );
		if ( tmpData ) {
			lineOutMuteGpioAddr = (UInt32*)tmpData->getBytesNoCopy();
            if ( lineOutMuteGpioAddr ) {
                debug2IOLog ("KeyLargoPlatform - lineOutMuteGpioAddr = %p\n", lineOutMuteGpioAddr);
                tmpData = OSDynamicCast ( OSData, lineOutMute->getProperty ( kAudioGPIOActiveState ) );
                if ( tmpData ) {
                    tmpPtr = (UInt32*)tmpData->getBytesNoCopy();
                    lineOutMuteActiveState = *tmpPtr;
                    debug2IOLog ("KeyLargoPlatform - lineOutMuteActiveState = 0x%X\n", lineOutMuteActiveState);
                    //	Take the hard coded memory address that's in the boot rom and convert it to a virtual address
                    lineOutMuteGpioMem = IODeviceMemory::withRange ( *lineOutMuteGpioAddr, sizeof ( UInt8 ) );
                    map = lineOutMuteGpioMem->map ( 0 );
                    lineOutMuteGpio = (UInt8*)map->getVirtualAddress();
                }
            }
		}
	}	
	debug2IOLog ("KeyLargoPlatform - lineOutMuteGpio = %p\n", lineOutMuteGpio);
	
	//	[2855519]	} end
	
	//	[2855519]	begin {
	//	Determine audio Codec reset method "A".  Get the physical address of the GPIO pin for applying
	//	a reset to the audio Codec.  Don't FailIf out of this code segment as the Codec reset may be
	//	defined through another property so this object may need to run even if no Codec reset method
	//	is defined through an 'audio-gpio' / 'audio-gpio-active-state' property pair.
	analogCodecReset = FindEntryByProperty ( gpio, kAudioGPIO, kAnalogHWResetEntry );
	if ( analogCodecReset ) {
		tmpData = OSDynamicCast ( OSData, analogCodecReset->getProperty ( kAAPLAddress ) );
		if ( tmpData ) {
			hwAnalogResetGpioAddr = (UInt32*)tmpData->getBytesNoCopy();
            if ( hwAnalogResetGpioAddr ) {
                debug3IOLog ("KeyLargoPlatform - hwAnalogResetGpioAddr = %p, *hwAnalogResetGpioAddr = %X\n", hwAnalogResetGpioAddr, (unsigned int)*hwAnalogResetGpioAddr);
                tmpData = OSDynamicCast ( OSData, analogCodecReset->getProperty ( kAudioGPIOActiveState ) );
                if ( tmpData ) {
                    tmpPtr = (UInt32*)tmpData->getBytesNoCopy();
                    hwAnalogResetActiveState = *tmpPtr;
                    debug2IOLog ("KeyLargoPlatform - hwAnalogResetActiveState = 0x%X\n", hwAnalogResetActiveState);
                    //	Take the hard coded memory address that's in the boot rom and convert it to a virtual address
                    hwAnalogResetRegMem = IODeviceMemory::withRange ( *hwAnalogResetGpioAddr, sizeof ( UInt8 ) );
                    map = hwAnalogResetRegMem->map ( 0 );
                    hwAnalogResetGpio = (UInt8*)map->getVirtualAddress();
                }
            }
		}
	}
	debug2IOLog ("KeyLargoPlatform - hwAnalogResetGpio = %p\n", hwAnalogResetGpio);
	//	[2855519]	} end

	digitalCodecReset = FindEntryByProperty ( gpio, kAudioGPIO, kDigitalHWResetEntry );

	if ( digitalCodecReset && 0 ) {
		tmpData = OSDynamicCast ( OSData, digitalCodecReset->getProperty ( kAAPLAddress ) );
		if ( tmpData ) {
			hwDigitalResetGpioAddr = (UInt32*)tmpData->getBytesNoCopy();
            if ( hwDigitalResetGpioAddr ) {
                debug3IOLog ("KeyLargoPlatform - hwDigitalResetGpioAddr = %p, *hwDigitalResetGpioAddr = %X\n", hwDigitalResetGpioAddr, (unsigned int)*hwDigitalResetGpioAddr);
                tmpData = OSDynamicCast ( OSData, digitalCodecReset->getProperty ( kAudioGPIOActiveState ) );
                if ( tmpData ) {
                    tmpPtr = (UInt32*)tmpData->getBytesNoCopy();
                    hwDigitalResetActiveState = *tmpPtr;
                    debug2IOLog ("KeyLargoPlatform - hwDigitalResetActiveState = 0x%X\n", hwDigitalResetActiveState);
                    //	Take the hard coded memory address that's in the boot rom and convert it to a virtual address
                    hwDigitalResetRegMem = IODeviceMemory::withRange ( *hwDigitalResetGpioAddr, sizeof ( UInt8 ) );
                    map = hwDigitalResetRegMem->map ( 0 );
                    hwDigitalResetGpio = (UInt8*)map->getVirtualAddress();
                }
            }
		}
	} else { //// 	!!!! test !!!!!!  Danger Hard coding ahead !!!!
		IOMemoryDescriptor * theDescriptor = IOMemoryDescriptor::withPhysicalAddress ( (IOPhysicalAddress)kKL_AUDIO_MAC_IO_BASE_ADDRESS, kKL_AUDIO_MAC_IO_SIZE, kIODirectionOutIn );
		FailIf ( NULL == theDescriptor, Exit );
		map = theDescriptor->map ();
		FailIf ( NULL == map, Exit );
		mHwPtr = (UInt8*)map->getVirtualAddress();
		FailIf ( NULL == mHwPtr, Exit );
		hwDigitalResetGpio = (GpioPtr)&mHwPtr[0x00000076];
	

//		debugIOLog ( "digitalCodecReset no estan aqui?\n" );
//		hwDigitalResetGpioAddr = (UInt32*)0x80000076;
//		hwDigitalResetGpio = (UInt8*)0x80000076;

/*
		if ( hwDigitalResetGpioAddr ) {
			debug3IOLog ("KeyLargoPlatform - hwDigitalResetGpioAddr = %p, *hwDigitalResetGpioAddr = %X\n", hwDigitalResetGpioAddr, (unsigned int)*hwDigitalResetGpioAddr);
			hwDigitalResetActiveState = 0;
			debug2IOLog ("KeyLargoPlatform - hwDigitalResetActiveState = 0x%X\n", hwDigitalResetActiveState);
			//	Take the hard coded memory address that's in the boot rom and convert it to a virtual address
			hwDigitalResetRegMem = IODeviceMemory::withRange ( *hwDigitalResetGpioAddr, sizeof ( UInt8 ) );
			map = hwDigitalResetRegMem->map ( 0 );
			//	hwDigitalResetGpio = (UInt8*)map->getVirtualAddress();
			hwDigitalResetGpio = (UInt8*)0x80000076;
		}
*/
	}
	//debug3IOLog ("KeyLargoPlatform - hwDigitalResetGpio = %p is at physical %p\n", hwDigitalResetGpio, hwDigitalResetRegMem->getPhysicalAddress() );

	intSource = 0;

	// get the interrupt provider for the headphone insertion interrupt
	intSource = FindEntryByProperty (gpio, kAudioGPIO, kHeadphoneDetectInt);
	FailWithAction (!intSource, result = false, Exit);
	debug2IOLog ("KeyLargoPlatform - intSource = %p\n", intSource);

	mHeadphoneDetectIntProvider = OSDynamicCast (IOService, intSource);
	FailWithAction (!mHeadphoneDetectIntProvider, result = false, Exit);
	debug2IOLog ("KeyLargoPlatform - mHeadphoneDetectIntProvider = %p\n", mHeadphoneDetectIntProvider);

	// get the active state of the headphone inserted pin
	tmpData = OSDynamicCast (OSData, intSource->getProperty (kAudioGPIOActiveState));
	if (NULL == tmpData) {
		headphoneInsertedActiveState = 1;
	} else {
		tmpPtr = (UInt32*)tmpData->getBytesNoCopy ();
		headphoneInsertedActiveState = *tmpPtr;
	}
	debug2IOLog ("KeyLargoPlatform - headphoneInsertedActiveState = 0x%X\n", headphoneInsertedActiveState);

	// get the physical address of the pin for detecting the headphone insertion/removal
	tmpData = OSDynamicCast (OSData, intSource->getProperty (kAAPLAddress));
	FailWithAction (!tmpData, result = false, Exit);
	headphoneExtIntGpioAddr = (UInt32*)tmpData->getBytesNoCopy ();
	debug2IOLog ("KeyLargoPlatform - headphoneExtIntGpioAddr = %p\n", headphoneExtIntGpioAddr);

	// take the hard coded memory address that's in the boot rom, and convert it a virtual address
	headphoneExtIntGpioMem = IODeviceMemory::withRange (*headphoneExtIntGpioAddr, sizeof (UInt8));
	map = headphoneExtIntGpioMem->map (0);
	headphoneExtIntGpio = (UInt8*)map->getVirtualAddress ();
	debug2IOLog ("KeyLargoPlatform - headphoneExtIntGpio = %p\n", headphoneExtIntGpioAddr);
	
	if (NULL != headphoneExtIntGpio) {
		//	Set interrupt source for dual edge to support jack insertion & removal
		curValue = *headphoneExtIntGpio;
		curValue = curValue | (dualEdge << intEdgeSEL);
		*headphoneExtIntGpio = curValue;
	}
	
	//	begin {	[2878119]
	intSource = 0;
	// get the interrupt provider for the line output jack insertion & removal interrupt
	intSource = FindEntryByProperty (gpio, kAudioGPIO, kLineOutDetectInt);
	if ( 0 != intSource ) {
		debugIOLog ( "KeyLargoPlatform -  Found LINE OUT DETECT!\n" );
		mLineOutDetectIntProvider = OSDynamicCast (IOService, intSource);
		if ( mLineOutDetectIntProvider ) {
			// get the active state of the line output inserted pin
			// This should really be gotten from the sound-objects property, but we're not parsing that yet.
			tmpData = OSDynamicCast (OSData, intSource->getProperty (kAudioGPIOActiveState));
			if (NULL != tmpData) {
				tmpPtr = (UInt32*)tmpData->getBytesNoCopy ();
				lineOutExtIntActiveState = *tmpPtr;
				debug2IOLog ("KeyLargoPlatform - lineOutInsertedActiveState = 0x%X\n", lineOutExtIntActiveState);
			
				// get the physical address of the pin for detecting the line output insertion/removal
				tmpData = OSDynamicCast (OSData, intSource->getProperty (kAAPLAddress));
				if ( tmpData ) {
					lineOutExtIntGpioAddr = (UInt32*)tmpData->getBytesNoCopy ();
					debug2IOLog ("KeyLargoPlatform - lineOutExtIntGpioAddr = %p\n", lineOutExtIntGpioAddr);
				
					// take the hard coded memory address that's in the boot rom, and convert it a virtual address
					lineOutExtIntGpioMem = IODeviceMemory::withRange (*lineOutExtIntGpioAddr, sizeof (UInt8));
					map = lineOutExtIntGpioMem->map (0);
					lineOutExtIntGpio = (UInt8*)map->getVirtualAddress ();
					
					//	Set interrupt source for dual edge to support jack insertion & removal
					if (NULL != lineOutExtIntGpio) {
						curValue = *lineOutExtIntGpio;
						curValue = curValue | (dualEdge << intEdgeSEL);
						*lineOutExtIntGpio = curValue;
					}
				}
			}
		}
	}

	//	[2878119]	} end

	// get the physical address of the gpio pin for setting the amplifier mute
	ampMute = FindEntryByProperty (gpio, kAudioGPIO, kAmpEntry);
	FailWithAction (!ampMute, result = false, Exit);
	tmpData = OSDynamicCast (OSData, ampMute->getProperty (kAAPLAddress));
	FailWithAction (!tmpData, result = false, Exit);
	ampMuteGpioAddr = (UInt32*)tmpData->getBytesNoCopy ();
	debug2IOLog ("KeyLargoPlatform - ampMuteGpioAddr = %p\n", ampMuteGpioAddr);
	tmpData = OSDynamicCast (OSData, ampMute->getProperty (kAudioGPIOActiveState));
	tmpPtr = (UInt32*)tmpData->getBytesNoCopy ();
	ampActiveState = *tmpPtr;
	debug2IOLog ("KeyLargoPlatform - ampActiveState = 0x%X\n", ampActiveState);

	// take the hard coded memory address that's in the boot rom, and convert it a virtual address
	ampMuteRegMem = IODeviceMemory::withRange (*ampMuteGpioAddr, sizeof (UInt8));
	map = ampMuteRegMem->map (0);
	ampMuteGpio = (UInt8*)map->getVirtualAddress ();
	debug2IOLog ("KeyLargoPlatform - headphoneExtIntGpio = %p\n", headphoneExtIntGpioAddr);

	theService = (OSDynamicCast(IOService, i2s));
	FailWithAction (!theService, result = false, Exit);

	map = theService->mapDeviceMemoryWithIndex ( inDBDMADeviceIndex );
	FailWithAction (!map, result = false, Exit);
	FailWithAction (kIOReturnSuccess != initI2S(map), result = false, Exit);
	
	debugIOLog ("- KeyLargoPlatform::init\n");

Exit:
	return result;
}

void KeyLargoPlatform::free()
{
	debugIOLog ("+ KeyLargoPlatform::free()\n");

 	if (NULL != mIOBaseAddressMemory) {
		mIOBaseAddressMemory->release();
	}

// docs are unclear on whether waitForService adds to the retain count of the service it returns, 
// this panic'd once with the release.
//	CLEAN_RELEASE(mKeyLargoService);

	detachFromI2C();

	super::free();

	debugIOLog ("- KeyLargoPlatform::free()\n");
}

#pragma mark ---------------------------
#pragma mark ¥ Codec Methods	
#pragma mark ---------------------------

bool KeyLargoPlatform::writeCodecRegister(UInt8 address, UInt8 subAddress, UInt8 *data, UInt16 len, BusMode mode)
{
	bool success = false;

	if (openI2C()) {
		switch (mode) {
			case kI2C_StandardMode:
				mI2CInterface->setStandardMode();
				break;
			case kI2C_StandardSubMode:
				mI2CInterface->setStandardSubMode();
				break;
			case kI2C_CombinedMode:
				mI2CInterface->setCombinedMode();
				break;
			default:
				debugIOLog ("KeyLargoPlatform::writeCodecRegister() unknown bus mode!\n");
				FailIf ( true, Exit );
				break;
		}		
		mI2CInterface->setPollingMode( true );
		//
		//	Conventional I2C address nomenclature concatenates a 7 bit address to a 1 bit read/*write bit
		//	 ___ ___ ___ ___ ___ ___ ___ ___
		//	|   |   |   |   |   |   |   |   |
		//	| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
		//	|___|___|___|___|___|___|___|___|
		//	  |   |   |   |   |   |   |   |____	1 = Read, 0 = Write
		//	  |___|___|___|___|___|___|________	7 bit address
		//
		//	The conventional method of referring to the I2C address is to read the address in
		//	place without any shifting of the address to compensate for the Read/*Write bit.
		//	The I2C driver does not use this standardized method of referring to the address
		//	and instead, requires shifting the address field right 1 bit so that the Read/*Write
		//	bit is not passed to the I2C driver as part of the address field.
		//
		success = mI2CInterface->writeI2CBus (address >> 1, subAddress, data, len);
		
		if (!success) debug5IOLog ( "KeyLargoPlatform::writeCodecRegister(%X, %X, %p %d), mI2CInterface->writeI2CBus returned false.\n", address, subAddress, data, len );
Exit:
		closeI2C();
	} else {
		debugIOLog ("KeyLargoPlatform::writeCodecRegister() couldn't open the I2C bus!\n");
	}
	return success;
}

bool KeyLargoPlatform::readCodecRegister(UInt8 address, UInt8 subAddress, UInt8 *data, UInt16 len, BusMode mode)
{
	bool success = false;

	if (openI2C()) {
		switch (mode) {
			case kI2C_StandardMode:
				mI2CInterface->setStandardMode();
				break;
			case kI2C_StandardSubMode:
				mI2CInterface->setStandardSubMode();
				break;
			case kI2C_CombinedMode:
				mI2CInterface->setCombinedMode();
				break;
			default:
				debugIOLog ("KeyLargoPlatform::readCodecRegister() unknown bus mode!\n");
				FailIf ( true, Exit );
				break;
		}		
		//
		//	Conventional I2C address nomenclature concatenates a 7 bit address to a 1 bit read/*write bit
		//	 ___ ___ ___ ___ ___ ___ ___ ___
		//	|   |   |   |   |   |   |   |   |
		//	| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
		//	|___|___|___|___|___|___|___|___|
		//	  |   |   |   |   |   |   |   |____	1 = Read, 0 = Write
		//	  |___|___|___|___|___|___|________	7 bit address
		//
		//	The conventional method of referring to the I2C address is to read the address in
		//	place without any shifting of the address to compensate for the Read/*Write bit.
		//	The I2C driver does not use this standardized method of referring to the address
		//	and instead, requires shifting the address field right 1 bit so that the Read/*Write
		//	bit is not passed to the I2C driver as part of the address field.
		//
		success = mI2CInterface->readI2CBus (address >> 1, subAddress, data, len);

		if (!success) debugIOLog ("KeyLargoPlatform::readCodecRegister(), mI2CInterface->writeI2CBus returned false.\n");
Exit:
		closeI2C();
	} else {
		debugIOLog ("KeyLargoPlatform::readCodecRegister() couldn't open the I2C bus!\n");
	}
	return success;
}

#pragma mark ---------------------------
#pragma mark ¥ I2S
#pragma mark ---------------------------

IOReturn KeyLargoPlatform::setI2SEnable (bool enable)
{
	UInt32 regValue;

	debug2IOLog ( "KeyLargoPlatform::setI2SEnable %s\n", enable ? "TRUE" : "FALSE");

	regValue = getKeyLargoRegister( ((UInt8*)mIOConfigurationBaseAddress) + kFCR1Offset );						

	switch ( mI2SInterfaceNumber ) {
		case kUseI2SCell0:
			if (enable)
				regValue |= kI2S0InterfaceEnable;
			else
				regValue &= ~kI2S0InterfaceEnable;
			break;
		case kUseI2SCell1:	
			if (enable)
				regValue |= kI2S1InterfaceEnable;
			else
				regValue &= ~kI2S1InterfaceEnable;
			break;
		default:
			enable = false;
			break;
	}
	setKeyLargoRegister( ((UInt8*)mIOConfigurationBaseAddress) + kFCR1Offset, regValue );
	
	return kIOReturnSuccess;
}

bool KeyLargoPlatform::getI2SEnable ()
{
	bool enable;
	UInt32 regValue;

	debugIOLog ( "KeyLargoPlatform::getI2SEnable\n");

	regValue = getKeyLargoRegister( ((UInt8*)mIOConfigurationBaseAddress) + kFCR1Offset );						

	switch ( mI2SInterfaceNumber ) {
		case kUseI2SCell0:	
			enable = regValue & kI2S0InterfaceEnable;
			break;
		case kUseI2SCell1:	
			enable = regValue & kI2S1InterfaceEnable;
			break;
		default:
			enable = false;
			break;
	}
	
	return enable;
}

IOReturn KeyLargoPlatform::setI2SClockEnable (bool enable)
{
	UInt32 regValue;

	debug2IOLog ( "KeyLargoPlatform::setI2SClockEnable %s\n", enable ? "TRUE" : "FALSE");

	regValue = getKeyLargoRegister( ((UInt8*)mIOConfigurationBaseAddress) + kFCR1Offset );						

	switch ( mI2SInterfaceNumber ) {
		case kUseI2SCell0:	
			if (enable)
				regValue |= kI2S0ClockEnable;
			else
				regValue &= ~kI2S0ClockEnable;
			break;
		case kUseI2SCell1:	
			if (enable)
				regValue |= kI2S1ClockEnable;
			else
				regValue &= ~kI2S1ClockEnable;
			break;
		default:
			enable = false;
			break;
	}

	setKeyLargoRegister( ((UInt8*)mIOConfigurationBaseAddress) + kFCR1Offset, regValue );
	debug5IOLog ( "FCR3 = %lX, FCR1 = %lX, serial format = %lX, data word sizes = %lX\n", getFCR3(), getFCR1(), getSerialFormatRegister(), getDataWordSizes() );
	return kIOReturnSuccess;
}

bool KeyLargoPlatform::getI2SClockEnable ()
{
	bool enable;
	UInt32 regValue;

	debugIOLog ( "KeyLargoPlatform::getI2SClockEnable\n" );

	regValue = getKeyLargoRegister( ((UInt8*)mIOConfigurationBaseAddress) + kFCR1Offset );						

	switch ( mI2SInterfaceNumber ) {
		case kUseI2SCell0:	
			enable = regValue & kI2S0ClockEnable;
			break;
		case kUseI2SCell1:	
			enable = regValue & kI2S1ClockEnable;
			break;
		default:
			enable = false;
			break;
	}
	
	return enable;
}

IOReturn KeyLargoPlatform::setI2SCellEnable (bool enable)
{
	UInt32 regValue;

	debug2IOLog ( "KeyLargoPlatform::setI2SCellEnable %s\n", enable ? "TRUE" : "FALSE");

	regValue = getKeyLargoRegister( ((UInt8*)mIOConfigurationBaseAddress) + kFCR1Offset );						

	switch ( mI2SInterfaceNumber ) {
		case kUseI2SCell0:
			if (enable)
				regValue |= kI2S0CellEnable;
			else
				regValue &= ~kI2S0CellEnable;
			break;
		case kUseI2SCell1:	
			if (enable)
				regValue |= kI2S1CellEnable;
			else
				regValue &= ~kI2S1CellEnable;
			break;
		default:
			enable = false;
			break;
	}

	setKeyLargoRegister( ((UInt8*)mIOConfigurationBaseAddress) + kFCR1Offset, regValue );
	
	return kIOReturnSuccess;
}

bool KeyLargoPlatform::getI2SCellEnable ()
{
	bool enable;
	UInt32 regValue;

	debugIOLog ( "KeyLargoPlatform::getI2SCellEnable\n" );

	regValue = getKeyLargoRegister( ((UInt8*)mIOConfigurationBaseAddress) + kFCR1Offset );						

	switch ( mI2SInterfaceNumber ) {
		case kUseI2SCell0:	
			enable = regValue & kI2S0CellEnable;
			break;
		case kUseI2SCell1:	
			enable = regValue & kI2S1CellEnable;
			break;
		default:
			enable = false;
			break;
	}
	
	return enable;
}

IOReturn KeyLargoPlatform::setSerialFormatRegister (UInt32 serialFormat)
{
	debug2IOLog ( "KeyLargoPlatform::setSerialFormatRegister (0x%lX)\n", serialFormat);

	OSWriteLittleInt32(mI2SBaseAddress, kI2SSerialFormatOffset, serialFormat);
	return kIOReturnSuccess;
}

UInt32 KeyLargoPlatform::getSerialFormatRegister ()
{
	UInt32 result = OSReadLittleInt32(mI2SBaseAddress, kI2SSerialFormatOffset);
	debug2IOLog ( "KeyLargoPlatform::getSerialFormatRegister = 0x%lX\n", result);
	return result;
}

IOReturn KeyLargoPlatform::setDataWordSizes (UInt32 dataWordSizes)
{
	debug2IOLog ( "KeyLargoPlatform::setDataWordSizes (0x%lX)\n", dataWordSizes);

	OSWriteLittleInt32(mI2SBaseAddress, kI2SDataWordSizesOffset, dataWordSizes);

	return kIOReturnSuccess;
}

UInt32 KeyLargoPlatform::getDataWordSizes ()
{
	UInt32 result = OSReadLittleInt32(mI2SBaseAddress, kI2SDataWordSizesOffset);
	debug2IOLog ( "KeyLargoPlatform::getDataWordSizes = 0x%lX\n", result);
	return result;
}

IOReturn KeyLargoPlatform::setI2SIOMIntControl (UInt32 intCntrl)
{
	debug2IOLog ( "KeyLargoPlatform::setI2SIOMIntControl (0x%lX)\n", intCntrl);

	OSWriteLittleInt32(mI2SBaseAddress, kI2SIntCtlOffset, intCntrl);

	return kIOReturnSuccess;
}

UInt32 KeyLargoPlatform::getI2SIOMIntControl ()
{
	UInt32 result = OSReadLittleInt32(mI2SBaseAddress, kI2SIntCtlOffset);
	debug2IOLog ( "KeyLargoPlatform::getI2SIOMIntControl = 0x%lX\n", result);
	return result;
}

IOReturn KeyLargoPlatform::releaseI2SClockSource(I2SClockFrequency inFrequency)	
{
	//	NOTE:	This needs a new KeyLargo driver from Tom LaPerre.  Coordinate for Q37 / Q27.  (see Ray)
    if (NULL != mKeyLargoService) {
		switch ( mI2SInterfaceNumber ) {
			case kUseI2SCell0:	
				switch ( inFrequency ) {
					case kI2S_45MHz:
						mKeyLargoService->callPlatformFunction (OSSymbol::withCString ("keyLargo_powerI2S"), false, (void *)false, (void *)0, (void *)0, (void *)0);
						break;
					case kI2S_49MHz:
						mKeyLargoService->callPlatformFunction (OSSymbol::withCString ("keyLargo_powerI2S"), false, (void *)false, (void *)0, (void *)1, (void *)0);
						break;
					case kI2S_18MHz:
						mKeyLargoService->callPlatformFunction (OSSymbol::withCString ("keyLargo_powerI2S"), false, (void *)false, (void *)0, (void *)2, (void *)0);
						break;
				}
				break;
			case kUseI2SCell1:
				switch ( inFrequency ) {
					case kI2S_45MHz:
						mKeyLargoService->callPlatformFunction (OSSymbol::withCString ("keyLargo_powerI2S"), false, (void *)false, (void *)1, (void *)0, (void *)0);
						break;
					case kI2S_49MHz:
						mKeyLargoService->callPlatformFunction (OSSymbol::withCString ("keyLargo_powerI2S"), false, (void *)false, (void *)1, (void *)1, (void *)0);
						break;
					case kI2S_18MHz:
						mKeyLargoService->callPlatformFunction (OSSymbol::withCString ("keyLargo_powerI2S"), false, (void *)false, (void *)1, (void *)2, (void *)0);
						break;
				}
				break;
			default:
				break;
		}
	}
	return kIOReturnSuccess;
}

IOReturn KeyLargoPlatform::requestI2SClockSource(I2SClockFrequency inFrequency)	
{
	//	NOTE:	This needs a new KeyLargo driver from Tom LaPerre.  Coordinate for Q37 / Q27.  (see Ray)
    if (NULL != mKeyLargoService) {
		switch ( mI2SInterfaceNumber ) {
			case kUseI2SCell0:	
				switch ( inFrequency ) {
					case kI2S_45MHz:
						mKeyLargoService->callPlatformFunction (OSSymbol::withCString ("keyLargo_powerI2S"), false, (void *)true, (void *)0, (void *)0, (void *)0);
						break;
					case kI2S_49MHz:
						mKeyLargoService->callPlatformFunction (OSSymbol::withCString ("keyLargo_powerI2S"), false, (void *)true, (void *)0, (void *)1, (void *)0);
						break;
					case kI2S_18MHz:
						mKeyLargoService->callPlatformFunction (OSSymbol::withCString ("keyLargo_powerI2S"), false, (void *)true, (void *)0, (void *)2, (void *)0);
						break;
				}
				break;
			case kUseI2SCell1:	
				switch ( inFrequency ) {
					case kI2S_45MHz:
						mKeyLargoService->callPlatformFunction (OSSymbol::withCString ("keyLargo_powerI2S"), false, (void *)true, (void *)1, (void *)0, (void *)0);
						break;
					case kI2S_49MHz:
						mKeyLargoService->callPlatformFunction (OSSymbol::withCString ("keyLargo_powerI2S"), false, (void *)true, (void *)1, (void *)1, (void *)0);
						break;
					case kI2S_18MHz:
						mKeyLargoService->callPlatformFunction (OSSymbol::withCString ("keyLargo_powerI2S"), false, (void *)true, (void *)1, (void *)2, (void *)0);
						break;
				}
				break;
			default:
				break;
		}
	}
	return kIOReturnSuccess;
}

IOReturn KeyLargoPlatform::setFrameCount (UInt32 value)
{
	OSWriteLittleInt32(mI2SBaseAddress, kI2SFrameCountOffset, value);

	return kIOReturnSuccess;
}

UInt32 KeyLargoPlatform::getFrameCount ()
{
	UInt32 frameCount = 0;

	return OSReadLittleInt32(mI2SBaseAddress, kI2SFrameCountOffset);

	return frameCount;
}

#pragma mark ---------------------------
#pragma mark ¥ GPIO
#pragma mark ---------------------------

GpioAttributes KeyLargoPlatform::getHeadphoneConnected()
{
	UInt8			headphoneSenseContents;
	GpioAttributes	connection;

	connection = kGPIO_Unknown;
	headphoneSenseContents = 0;

	if ( NULL != headphoneExtIntGpio ) {
		headphoneSenseContents = *headphoneExtIntGpio;
	
		if ( (headphoneSenseContents & (gpioBIT_MASK << gpioPIN_RO)) == (headphoneInsertedActiveState << gpioPIN_RO) ) {
			connection = kGPIO_Connected;
		} else {
			connection = kGPIO_Disconnected;
		}
	}
	return connection;
}

GpioAttributes	KeyLargoPlatform::getSpeakerConnected() {
	UInt8			speakerSenseContents;
	GpioAttributes	connection;

	connection = kGPIO_Unknown;
	speakerSenseContents = 0;

	if ( NULL != speakerExtIntGpio ) {
		speakerSenseContents = *speakerExtIntGpio;
	
		if ( (speakerSenseContents & (gpioBIT_MASK << gpioPIN_RO)) == (speakerInsertedActiveState << gpioPIN_RO) ) {
			connection = kGPIO_Connected;
		} else {
			connection = kGPIO_Disconnected;
		}
	}
	return connection;
}

GpioAttributes KeyLargoPlatform::getLineOutConnected()
{
	UInt8				lineOutSenseContents;
	GpioAttributes		connection;

	connection = kGPIO_Unknown;
	lineOutSenseContents = 0;

	if ( NULL != lineOutExtIntGpio ) {
		lineOutSenseContents = *lineOutExtIntGpio;
	
		if ( (lineOutSenseContents & (gpioBIT_MASK << gpioPIN_RO)) == (lineOutExtIntActiveState << gpioPIN_RO) ) {
			connection = kGPIO_Connected;
		} else {
			connection = kGPIO_Disconnected;
		}
	}
	
	return connection;
}

IOReturn KeyLargoPlatform::setCodecReset ( CODEC_RESET target, GpioAttributes reset ) {
	IOReturn		result = kIOReturnError;
	
	debug2IOLog ( "+ KeyLargoPlatform::setCodecReset ( %d )\n", reset );
	if ( kCODEC_RESET_Analog == target ) {
		if ( NULL != hwAnalogResetGpio ) {
			if ( kGPIO_Reset == reset ) {
				result = gpioWrite ( hwAnalogResetGpio, assertGPIO ( hwAnalogResetActiveState ) );
			} else if ( kGPIO_Run == reset ) {
				result = gpioWrite ( hwAnalogResetGpio, negateGPIO ( hwAnalogResetActiveState ) );
			}
		}
	} else if ( kCODEC_RESET_Digital == target ) {
		if ( NULL != hwDigitalResetGpio ) {
			if ( kGPIO_Reset == reset ) {
				result = gpioWrite ( hwDigitalResetGpio, assertGPIO ( hwDigitalResetActiveState ) );
			} else if ( kGPIO_Run == reset ) {
				result = gpioWrite ( hwDigitalResetGpio, negateGPIO ( hwDigitalResetActiveState ) );
			}
		}
	}
	debug3IOLog ( "- KeyLargoPlatform::setCodecReset ( %d ) result = %X\n", reset, result );
	return result;
}

GpioAttributes KeyLargoPlatform::getCodecReset ( CODEC_RESET target ) {
	GpioAttributes	reset = kGPIO_Unknown;

	if ( kCODEC_RESET_Analog == target ) {
		if ( NULL != hwAnalogResetGpio ) {
			reset = hwAnalogResetActiveState == gpioRead ( hwAnalogResetGpio ) ? kGPIO_Reset : kGPIO_Run ;
		}
	} else if ( kCODEC_RESET_Digital == target ) {
		if ( NULL != hwDigitalResetGpio ) {
			reset = hwDigitalResetActiveState == gpioRead ( hwDigitalResetGpio ) ? kGPIO_Reset : kGPIO_Run ;
		}
	}
	return reset;
}

IOReturn KeyLargoPlatform::setHeadphoneMuteState ( GpioAttributes muteState )
{
	IOReturn		result = kIOReturnError;

	if ( NULL != hdpnMuteGpio ) {
		if ( kGPIO_Muted == muteState ) {	
			result = gpioWrite ( hdpnMuteGpio, assertGPIO ( hdpnActiveState) );
		} else {
			result = gpioWrite ( hdpnMuteGpio, negateGPIO ( hdpnActiveState) );
		}
	}		
	return result;
}

GpioAttributes KeyLargoPlatform::getHeadphoneMuteState()
{
	GpioAttributes muteState = kGPIO_Unknown;
	
	if ( NULL != hdpnMuteGpio ) {
		muteState = hdpnActiveState == gpioRead ( hdpnMuteGpio ) ? kGPIO_Muted : kGPIO_Unmuted ;
	}	
	return muteState;
}
	
IOReturn KeyLargoPlatform::setLineOutMuteState ( GpioAttributes muteState )
{
	IOReturn	result = kIOReturnError;
	
	if ( lineOutMuteGpio ) {
		if ( kGPIO_Muted == muteState ) {	
			result = gpioWrite ( lineOutMuteGpio, assertGPIO ( lineOutMuteActiveState ) );
		} else if ( kGPIO_Unmuted == muteState ) {
			result = gpioWrite ( lineOutMuteGpio, negateGPIO ( lineOutMuteActiveState ) );
		}
	}
	return result;
}

GpioAttributes KeyLargoPlatform::getLineOutMuteState()
{
	GpioAttributes muteState = kGPIO_Unknown;
	
	if (lineOutMuteGpio) {
		muteState = lineOutMuteActiveState == gpioRead ( lineOutMuteGpio ) ? kGPIO_Muted : kGPIO_Unmuted ;
	}	
	return muteState;
}

IOReturn KeyLargoPlatform::setSpeakerMuteState ( GpioAttributes muteState )
{
	IOReturn		result = kIOReturnError;
	
	if ( ampMuteGpio ) {
		if ( kGPIO_Muted == muteState ) {	
			result = gpioWrite( ampMuteGpio, assertGPIO ( ampActiveState ) );
		} else if ( kGPIO_Unmuted == muteState ) {
			result = gpioWrite( ampMuteGpio, negateGPIO ( ampActiveState ) );
		}
	}
	return kIOReturnSuccess;
}

GpioAttributes KeyLargoPlatform::getSpeakerMuteState()
{
	GpioAttributes muteState = kGPIO_Unknown;
	
	if (ampMuteGpio) {
		muteState = ampActiveState == gpioRead ( ampMuteGpio ) ? kGPIO_Muted : kGPIO_Unmuted ;
	}
	
	return muteState;
}

#pragma mark ---------------------------
#pragma mark ¥ Interrupts
#pragma mark ---------------------------

IOReturn KeyLargoPlatform::registerInterruptHandler (IOService * theDevice, void * interruptHandler, PlatformInterruptSource source ) {
	IOReturn				result;

	result = kIOReturnError;
	switch ( source ) {
		case kHeadphoneDetectInterrupt: 	result = setHeadphoneDetectInterruptHandler (theDevice, interruptHandler);		break;
		case kSpeakerDetectInterrupt: 		result = setSpeakerDetectInterruptHandler (theDevice, interruptHandler);		break;
		case kCodecInterrupt: 				result = setCodecInterruptHandler (theDevice, interruptHandler);				break;
		case kCodecErrorDetectInterrupt: 	result = setCodecErrorInterruptHandler (theDevice, interruptHandler);			break;
		case kLineInputDetectInterrupt: 	result = setLineInDetectInterruptHandler (theDevice, interruptHandler);			break;
		case kLineOutputDetectInterrupt: 	result = setLineOutDetectInterruptHandler (theDevice, interruptHandler);		break;
		case kDigitalInDetectInterrupt: 	result = setDigitalInDetectInterruptHandler (theDevice, interruptHandler);		break;
		case kDigitalOutDetectInterrupt: 	result = setDigitalOutDetectInterruptHandler (theDevice, interruptHandler);		break;
		case kUnknownInterrupt:
		default:							debugIOLog ( "Attempt to register unknown interrupt source\n" );				break;
	}

	return result;
}

IOReturn KeyLargoPlatform::unregisterInterruptHandler (IOService * theDevice, void * interruptHandler, PlatformInterruptSource source ) {
	IOReturn				result;

	result = kIOReturnError;
	switch ( source ) {
		case kHeadphoneDetectInterrupt: 	result = setHeadphoneDetectInterruptHandler (theDevice, NULL);		break;
		case kSpeakerDetectInterrupt: 		result = setSpeakerDetectInterruptHandler (theDevice, NULL);		break;
		case kCodecInterrupt: 				result = setCodecInterruptHandler (theDevice, NULL);				break;
		case kCodecErrorDetectInterrupt: 	result = setCodecErrorInterruptHandler (theDevice, NULL);			break;
		case kLineInputDetectInterrupt: 	result = setLineInDetectInterruptHandler (theDevice, NULL);			break;
		case kLineOutputDetectInterrupt: 	result = setLineOutDetectInterruptHandler (theDevice, NULL);		break;
		case kDigitalInDetectInterrupt: 	result = setDigitalInDetectInterruptHandler (theDevice, NULL);		break;
		case kDigitalOutDetectInterrupt: 	result = setDigitalOutDetectInterruptHandler (theDevice, NULL);		break;
		case kUnknownInterrupt:
		default:							debugIOLog ( "Attempt to register unknown interrupt source\n" );	break;
	}

	return result;
}

IOReturn KeyLargoPlatform::setHeadphoneDetectInterruptHandler (IOService* theDevice, void* interruptHandler)
{
	IOReturn result;
	IOInterruptEventSource * theInterruptEventSource;
	
	theInterruptEventSource = NULL;
	result = kIOReturnError;

	FailIf (NULL == mWorkLoop, Exit);

	if ( NULL == interruptHandler && NULL != mHeadphoneDetectIntEventSource) {
		mHeadphoneDetectIntEventSource->disable ();
		result = mWorkLoop->removeEventSource (mHeadphoneDetectIntEventSource);	
		mHeadphoneDetectIntEventSource = NULL;
	} else {
		FailIf (NULL == mHeadphoneDetectIntProvider, Exit);
		mHeadphoneDetectIntEventSource = theInterruptEventSource = IOInterruptEventSource::interruptEventSource (this,
																				(IOInterruptEventSource::Action)interruptHandler,
																				mHeadphoneDetectIntProvider,
																				0);
		FailIf (NULL == theInterruptEventSource, Exit);
		
		result = mWorkLoop->addEventSource (theInterruptEventSource);
		theInterruptEventSource->enable ();
	}

Exit:
	if (NULL != theInterruptEventSource) {
		theInterruptEventSource->release();
	}
	debug2IOLog ( "KeyLargoPlatform::setHeadphoneDetectInterruptHandler() returns %X\n", result );
	return result;
}

IOReturn KeyLargoPlatform::setSpeakerDetectInterruptHandler (IOService* theDevice, void* interruptHandler)
{
	IOReturn result;
	IOInterruptEventSource * theInterruptEventSource;
	
	theInterruptEventSource = NULL;
	result = kIOReturnError;

	if ( NULL == interruptHandler && NULL != mSpeakerDetectIntEventSource) {
		mSpeakerDetectIntEventSource->disable ();
		result = mWorkLoop->removeEventSource (mSpeakerDetectIntEventSource);	
		mSpeakerDetectIntEventSource = NULL;
	} else {
		FailIf (NULL == mHeadphoneDetectIntProvider, Exit);
		mSpeakerDetectIntEventSource = theInterruptEventSource = IOInterruptEventSource::interruptEventSource (theDevice,
																				(IOInterruptEventSource::Action)interruptHandler,
																				mSpeakerDetectIntProvider,
																				0);
		FailIf (NULL == theInterruptEventSource, Exit);
		
		result = mWorkLoop->addEventSource (theInterruptEventSource);	
		theInterruptEventSource->enable ();
	}

Exit:
	if (NULL != theInterruptEventSource) {
		theInterruptEventSource->release();
	}
	debug2IOLog ( "KeyLargoPlatform::setSpeakerDetectInterruptHandler() returns %X\n", result );
	return result;
}

IOReturn KeyLargoPlatform::setLineOutDetectInterruptHandler (IOService* theDevice, void* interruptHandler)
{
	IOReturn result;
	IOInterruptEventSource * theInterruptEventSource;

	theInterruptEventSource = NULL;
	result = kIOReturnError;

	if ( NULL == interruptHandler && NULL != mLineOutDetectIntEventSource) {
		mLineOutDetectIntEventSource->disable ();
		result = mWorkLoop->removeEventSource (mLineOutDetectIntEventSource);		mLineOutDetectIntEventSource = NULL;
	} else {
		FailIf (NULL == mLineOutDetectIntProvider, Exit);
		mLineOutDetectIntEventSource = theInterruptEventSource = IOInterruptEventSource::interruptEventSource (theDevice,
																				(IOInterruptEventSource::Action)interruptHandler,
																				mLineOutDetectIntProvider,
																				0);
		FailIf (NULL == theInterruptEventSource, Exit);
		
		result = mWorkLoop->addEventSource (theInterruptEventSource);	
		theInterruptEventSource->enable ();
	}
Exit:
	if (NULL != theInterruptEventSource) {
		theInterruptEventSource->release();
	}
	debug2IOLog ( "KeyLargoPlatform::setLineOutDetectInterruptHandler() returns %X\n", result );
	return result;
}

IOReturn KeyLargoPlatform::setLineInDetectInterruptHandler (IOService* theDevice, void* interruptHandler)
{
	IOReturn result;
	IOInterruptEventSource * theInterruptEventSource;

	theInterruptEventSource = NULL;
	result = kIOReturnError;

	if ( NULL == interruptHandler && NULL != mLineInDetectIntEventSource) {
		mLineInDetectIntEventSource->disable ();
		result = mWorkLoop->removeEventSource (mLineInDetectIntEventSource);		mLineInDetectIntEventSource = NULL;
	} else {
		FailIf (NULL == mLineInDetectIntProvider, Exit);
		mLineInDetectIntEventSource = theInterruptEventSource = IOInterruptEventSource::interruptEventSource (theDevice,
																				(IOInterruptEventSource::Action)interruptHandler,
																				mLineInDetectIntProvider,
																				0);
		FailIf (NULL == theInterruptEventSource, Exit);
		
		result = mWorkLoop->addEventSource (theInterruptEventSource);	
		theInterruptEventSource->enable ();
	}
Exit:
	if (NULL != theInterruptEventSource) {
		theInterruptEventSource->release();
	}
	debug2IOLog ( "KeyLargoPlatform::setLineInDetectInterruptHandler() returns %X\n", result );
	return result;
}

IOReturn KeyLargoPlatform::setDigitalOutDetectInterruptHandler (IOService* theDevice, void* interruptHandler)
{
	IOReturn result;
	IOInterruptEventSource * theInterruptEventSource;

	theInterruptEventSource = NULL;
	result = kIOReturnError;

	if ( NULL == interruptHandler && NULL != mDigitalOutDetectIntEventSource) {
		mDigitalOutDetectIntEventSource->disable ();
		result = mWorkLoop->removeEventSource (mDigitalOutDetectIntEventSource);		mDigitalOutDetectIntEventSource = NULL;
	} else {
		FailIf (NULL == mDigitalOutDetectIntProvider, Exit);
		mDigitalOutDetectIntEventSource = theInterruptEventSource = IOInterruptEventSource::interruptEventSource (theDevice,
																				(IOInterruptEventSource::Action)interruptHandler,
																				mDigitalOutDetectIntProvider,
																				0);
		FailIf (NULL == theInterruptEventSource, Exit);
		
		result = mWorkLoop->addEventSource (theInterruptEventSource);	
		theInterruptEventSource->enable ();
	}
Exit:
	if (NULL != theInterruptEventSource) {
		theInterruptEventSource->release();
	}
	debug2IOLog ( "KeyLargoPlatform::setDigitalOutDetectInterruptHandler() returns %X\n", result );
	return result;
}

IOReturn KeyLargoPlatform::setDigitalInDetectInterruptHandler (IOService* theDevice, void* interruptHandler)
{
	IOReturn result;
	IOInterruptEventSource * theInterruptEventSource;

	theInterruptEventSource = NULL;
	result = kIOReturnError;

	if ( NULL == interruptHandler && NULL != mDigitalInDetectIntEventSource) {
		mDigitalInDetectIntEventSource->disable ();
		result = mWorkLoop->removeEventSource (mDigitalInDetectIntEventSource);		mDigitalInDetectIntEventSource = NULL;
	} else {
		FailIf (NULL == mDigitalInDetectIntProvider, Exit);
		mDigitalInDetectIntEventSource = theInterruptEventSource = IOInterruptEventSource::interruptEventSource (theDevice,
																				(IOInterruptEventSource::Action)interruptHandler,
																				mDigitalInDetectIntProvider,
																				0);
		FailIf (NULL == theInterruptEventSource, Exit);
		
		result = mWorkLoop->addEventSource (theInterruptEventSource);	
		theInterruptEventSource->enable ();
	}
Exit:
	if (NULL != theInterruptEventSource) {
		theInterruptEventSource->release();
	}
	debug2IOLog ( "KeyLargoPlatform::setDigitalInDetectInterruptHandler() returns %X\n", result );
	return result;
}

IOReturn KeyLargoPlatform::setCodecInterruptHandler (IOService* theDevice, void* interruptHandler)
{
	IOReturn result;
	IOInterruptEventSource * theInterruptEventSource;

	theInterruptEventSource = NULL;
	result = kIOReturnError;

	if ( NULL == interruptHandler && NULL != mCodecInterruptEventSource) {
		mCodecInterruptEventSource->disable ();
		result = mWorkLoop->removeEventSource (mCodecInterruptEventSource);		mCodecInterruptEventSource = NULL;
	} else {
		FailIf (NULL == mCodecIntProvider, Exit);
		mCodecInterruptEventSource = theInterruptEventSource = IOInterruptEventSource::interruptEventSource (theDevice,
																				(IOInterruptEventSource::Action)interruptHandler,
																				mCodecIntProvider,
																				0);
		FailIf (NULL == theInterruptEventSource, Exit);
		
		result = mWorkLoop->addEventSource (theInterruptEventSource);	
		theInterruptEventSource->enable ();
	}
Exit:
	if (NULL != theInterruptEventSource) {
		theInterruptEventSource->release();
	}
	debug2IOLog ( "KeyLargoPlatform::setCodecInterruptHandler() returns %X\n", result );
	return result;
}

IOReturn KeyLargoPlatform::setCodecErrorInterruptHandler (IOService* theDevice, void* interruptHandler)
{
	IOReturn result;
	IOInterruptEventSource * theInterruptEventSource;

	theInterruptEventSource = NULL;
	result = kIOReturnError;

	if ( NULL == interruptHandler && NULL != mCodecErrorInterruptEventSource) {
		mCodecErrorInterruptEventSource->disable ();
		result = mWorkLoop->removeEventSource (mCodecErrorInterruptEventSource);		mCodecErrorInterruptEventSource = NULL;
	} else {
		FailIf (NULL == mCodecErrorIntProvider, Exit);
		mCodecErrorInterruptEventSource = theInterruptEventSource = IOInterruptEventSource::interruptEventSource (theDevice,
																				(IOInterruptEventSource::Action)interruptHandler,
																				mCodecErrorIntProvider,
																				0);
		FailIf (NULL == theInterruptEventSource, Exit);
		
		result = mWorkLoop->addEventSource (theInterruptEventSource);	
		theInterruptEventSource->enable ();
	}
Exit:
	if (NULL != theInterruptEventSource) {
		theInterruptEventSource->release();
	}
	debug2IOLog ( "KeyLargoPlatform::setCodecErrorInterruptHandler() returns %X\n", result );
	return result;
}

void KeyLargoPlatform::logFCR1( void )
{
	debug2IOLog ( "logFCR1 = %lX\n", OSReadLittleInt32 ( mIOConfigurationBaseAddress, kFCR1Offset ) );
	return;
}

void KeyLargoPlatform::logFCR3( void )
{
	debug2IOLog ( "logFCR3 = %lX\n", OSReadLittleInt32 ( mIOConfigurationBaseAddress, kFCR3Offset ) );
	return;
}

#pragma mark ---------------------------
#pragma mark ¥ Private Direct HW Access
#pragma mark ---------------------------

UInt8 KeyLargoPlatform::gpioReadByte( UInt8* gpioAddress )
{
	UInt8			gpioData;
	
	gpioData = 0;
	if( NULL != gpioAddress )
	{
		gpioData = *gpioAddress;
#ifdef kDEBUG_GPIO
		debug3IOLog( "KeyLargoPlatform::gpioReadByte( 0x%8.0X ), *gpioAddress 0x%X\n", (unsigned int)gpioAddress, gpioData );
#endif
	}

	return gpioData;
}

void KeyLargoPlatform::gpioWriteByte( UInt8* gpioAddress, UInt8 data )
{
	if( NULL != gpioAddress )
	{
		*gpioAddress = data;
#ifdef kDEBUG_GPIO
		debug3IOLog( "KeyLargoPlatform::gpioWrite( 0x%8.0X, 0x%2.0X )\n", (unsigned int)gpioAddress, data);
#endif
	}
}

//	Return 'true' if the 'gpioData' bit is non-zero.  This function does not
//	return the state of the pin.
bool KeyLargoPlatform::gpioRead( UInt8* gpioAddress )
{
	UInt8			gpioData;
	bool			result;
	
	result = 0;
	if( NULL != gpioAddress )
	{
		gpioData = *gpioAddress;
		if( 0 != ( gpioData & ( 1 << gpioPIN_RO ) ) )
			result = 1;
		debug4IOLog( "KeyLargoPlatform::gpioRead( 0x%8.0X ) result %d, *gpioAddress 0x%2.0X\n", (unsigned int)gpioAddress, result, *gpioAddress );
	}
	return result;
}

//	Sets the 'gpioDDR' to OUTPUT and sets the 'gpioDATA' to the 'data' state.
IOReturn KeyLargoPlatform::gpioWrite( UInt8* gpioAddress, UInt8 data )
{
	UInt8		gpioData;
	IOReturn	result = kIOReturnBadArgument;
	
	if( NULL != gpioAddress ) {
		if( 0 == data )
			gpioData = ( gpioDDR_OUTPUT << gpioDDR ) | ( 0 << gpioDATA );
		else
			gpioData = ( gpioDDR_OUTPUT << gpioDDR ) | ( 1 << gpioDATA );

		*gpioAddress = gpioData;
		result = kIOReturnSuccess;
		debug4IOLog( "KeyLargoPlatform::gpioWrite( 0x%8.0X, 0x%2.0X ), *gpioAddress 0x%2.0X\n", (unsigned int)gpioAddress, gpioData, *gpioAddress );
	}
	return result;
}

// This is a candidate for removal, as it appears only used on TAS3001...
//	Return 'true' if the 'gpioDDR' bit is non-zero.
UInt8 KeyLargoPlatform::gpioGetDDR( UInt8* gpioAddress )
{
	UInt8			gpioData;
	bool			result;
	
	result = gpioDDR_INPUT;
	if( NULL != gpioAddress )
	{
		gpioData = *gpioAddress;
		if( 0 != ( gpioData & ( 1 << gpioDDR ) ) )
			result = gpioDDR_OUTPUT ;
#ifdef kDEBUG_GPIO
		debug4IOLog( "***** GPIO DDR RD 0x%8.0X = 0x%2.0X returns %d\n", (unsigned int)gpioAddress, gpioData, result );
#endif
	}
	return result;
}

void KeyLargoPlatform::setKeyLargoRegister(void *klRegister, UInt32 value)
{
	debug3IOLog ( "Register %p = %lX\n", klRegister, value );
	OSWriteLittleInt32(klRegister, 0, value);
}

UInt32 KeyLargoPlatform::getKeyLargoRegister(void *klRegister)
{
    return (OSReadLittleInt32(klRegister, 0));
}

UInt32 KeyLargoPlatform::getFCR1( void )
{
	UInt32 result = OSReadLittleInt32 ( mIOConfigurationBaseAddress, kFCR1Offset );		
	debug2IOLog ( "getFCR1 = %lX\n", result );
	return result;
}

void KeyLargoPlatform::setFCR1(UInt32 value)
{
	debug3IOLog ( "Register %lX = %lX\n", (UInt32)(mIOConfigurationBaseAddress) + kFCR1Offset, value );
	OSWriteLittleInt32( mIOConfigurationBaseAddress, kFCR1Offset, value );
}

UInt32 KeyLargoPlatform::getFCR3( void )
{
	UInt32 result = OSReadLittleInt32 ( mIOConfigurationBaseAddress, kFCR3Offset );
	debug2IOLog ( "getFCR3 = %lX\n", result );
	return result;
}

void KeyLargoPlatform::setFCR3(UInt32 value)
{
	debug3IOLog ( "Register %lX = %lX\n", (UInt32)(mIOConfigurationBaseAddress) + kFCR1Offset, value );
	OSWriteLittleInt32( mIOConfigurationBaseAddress, kFCR3Offset, value );
}

#pragma mark ---------------------------
#pragma mark ¥ Private I2C, I2S
#pragma mark ---------------------------

// Init the I2S register memory maps
IOReturn KeyLargoPlatform::initI2S(IOMemoryMap* map)
{
	IOReturn		result = kIOReturnError;
	
	debug2IOLog ("KeyLargoPlatform::initI2S - map = 0x%X\n", (unsigned int)map);

    // cache the config space
	mSoundConfigSpace = (UInt8 *)map->getPhysicalAddress();

    // sets the clock base address figuring out which I2S cell we're on
    if ((((UInt32)mSoundConfigSpace ^ kI2S0BaseOffset) & 0x0001FFFF) == 0) 
    {
		//	[3060321]	ioBaseAddress is required by this object in order to enable the target
		//				I2S I/O Module for which this object is to service.  The I2S I/O Module
		//				enable occurs through the configuration registers which reside in the
		//				first block of ioBase.		rbm		2 Oct 2002
		mIOBaseAddress = (void *)((UInt32)mSoundConfigSpace - kI2S0BaseOffset);
		mIOBaseAddressMemory = IODeviceMemory::withRange ((IOPhysicalAddress)((UInt8 *)mSoundConfigSpace - kI2S0BaseOffset), 256);
        mI2SInterfaceNumber = kUseI2SCell0;
    }
    else if ((((UInt32)mSoundConfigSpace ^ kI2S1BaseOffset) & 0x0001FFFF) == 0) 
    {
		//	[3060321]	ioBaseAddress is required by this object in order to enable the target
		//				I2S I/O Module for which this object is to service.  The I2S I/O Module
		//				enable occurs through the configuration registers which reside in the
		//				first block of ioBase.		rbm		2 Oct 2002
		mIOBaseAddress = (void *)((UInt32)mSoundConfigSpace - kI2S1BaseOffset);
		mIOBaseAddressMemory = IODeviceMemory::withRange ((IOPhysicalAddress)((UInt8 *)mSoundConfigSpace - kI2S1BaseOffset), 256);
        mI2SInterfaceNumber = kUseI2SCell1;
    }
    else 
    {
        DEBUG_IOLOG("AudioI2SControl::init ERROR: unable to setup ioBaseAddress and i2SInterfaceNumber\n");
    }
	FailIf (NULL == mIOBaseAddressMemory, Exit);

	//	[3060321]	ioConfigurationBaseAddress is required by this object in order to enable the target
	//				I2S I/O Module for which this object is to service.  The I2S I/O Module
	//				enable occurs through the configuration registers which reside in the
	//				first block of ioBase.		rbm		2 Oct 2002
	
	mIOConfigurationBaseAddress = (void *)mIOBaseAddressMemory->map()->getVirtualAddress();
	FailIf ( NULL == mIOConfigurationBaseAddress, Exit );

	//
	//	There are three sections of memory mapped I/O that are directly accessed by the AppleLegacyAudio.  These
	//	include the GPIOs, I2S DMA Channel Registers and I2S control registers.  They fall within the memory map 
	//	as follows:
	//	~                              ~
	//	|______________________________|
	//	|                              |
	//	|         I2S Control          |
	//	|______________________________|	<-	soundConfigSpace = ioBase + i2s0BaseOffset ...OR... ioBase + i2s1BaseOffset
	//	|                              |
	//	~                              ~
	//	~                              ~
	//	|______________________________|
	//	|                              |
	//	|       I2S DMA Channel        |
	//	|______________________________|	<-	i2sDMA = ioBase + i2s0_DMA ...OR... ioBase + i2s1_DMA
	//	|                              |
	//	~                              ~
	//	~                              ~
	//	|______________________________|
	//	|            FCRs              |
	//	|            GPIO              |	<-	gpio = ioBase + gpioOffsetAddress
	//	|         ExtIntGPIO           |	<-	fcr = ioBase + fcrOffsetAddress
	//	|______________________________|	<-	ioConfigurationBaseAddress
	//	|                              |
	//	~                              ~
	//
	//	The I2S DMA Channel is mapped in by the AppleLegacyDBDMAAudioDMAEngine.  Only the I2S control registers are 
	//	mapped in by the AudioI2SControl.  The Apple I/O Configuration Space (i.e. FCRs, GPIOs and ExtIntGPIOs)
	//	are mapped in by the subclass of AppleLegacyAudio.  The FCRs must also be mapped in by the AudioI2SControl
	//	object as the init method must enable the I2S I/O Module for which the AudioI2SControl object is
	//	being instantiated for.
	//
	
	//	Map the I2S configuration registers
	if (kUseI2SCell0 == mI2SInterfaceNumber) {
		mIOI2SBaseAddressMemory = IODeviceMemory::withRange ((IOPhysicalAddress)((UInt8 *)mSoundConfigSpace), kI2S_IO_CONFIGURATION_SIZE);
	} else {
		mIOI2SBaseAddressMemory = IODeviceMemory::withRange ((IOPhysicalAddress)((UInt8 *)mSoundConfigSpace), kI2S_IO_CONFIGURATION_SIZE);
	}
	if (NULL != mIOI2SBaseAddressMemory) {
		mI2SBaseAddress = (void *)mIOI2SBaseAddressMemory->map()->getVirtualAddress();
	} else {
		return kIOReturnError;
	}
	FailIf (NULL == mIOI2SBaseAddressMemory, Exit)
	
	mI2SBaseAddress = (void *)mIOI2SBaseAddressMemory->map()->getVirtualAddress();
	FailIf (NULL == mI2SBaseAddress, Exit);

	debug2IOLog ("KeyLargoPlatform::initI2S - mI2SInterfaceNumber = 0x%X\n", mI2SInterfaceNumber);
	debug2IOLog ("KeyLargoPlatform::initI2S - mIOI2SBaseAddressMemory = %p\n", mIOI2SBaseAddressMemory);
	debug2IOLog ("KeyLargoPlatform::initI2S - mI2SBaseAddress = %p\n", mI2SBaseAddress);
	debug2IOLog ("KeyLargoPlatform::initI2S - mIOConfigurationBaseAddress = %p\n", mIOConfigurationBaseAddress);

	//	Enable the I2S interface by setting the enable bit in the feature 
	//	control register.  This one action requires knowledge of the address 
	//	of I/O configuration address space.		[3060321]	rbm		2 Oct 2002
	if (kUseI2SCell0 == mI2SInterfaceNumber) {
		setKeyLargoRegister ( ((UInt8*)mIOConfigurationBaseAddress) + kFCR1Offset, getKeyLargoRegister ( ((UInt8*)mIOConfigurationBaseAddress) + kFCR1Offset ) | kI2S0InterfaceEnable );
	} else {
		setKeyLargoRegister ( ((UInt8*)mIOConfigurationBaseAddress) + kFCR1Offset, getKeyLargoRegister ( ((UInt8*)mIOConfigurationBaseAddress) + kFCR1Offset ) | kI2S1InterfaceEnable );
	}

	result = kIOReturnSuccess;

Exit:
	return result;
}

bool KeyLargoPlatform::findAndAttachI2C()
{
	const OSSymbol	*i2cDriverName;
	IOService		*i2cCandidate;
	OSDictionary	*i2cServiceDictionary;
	
	// Searches the i2c:
	i2cDriverName = OSSymbol::withCStringNoCopy("PPCI2CInterface.i2c-mac-io");
	i2cServiceDictionary = IOService::resourceMatching(i2cDriverName);
	i2cCandidate = IOService::waitForService(i2cServiceDictionary);
	//i2cDriverName->release();
	//i2cServiceDictionary->release();
	
	mI2CInterface = (PPCI2CInterface*)i2cCandidate->getProperty(i2cDriverName);

	if (NULL == mI2CInterface) {
		debugIOLog("KeyLargoPlatform::findAndAttachI2C can't find the i2c in the registry\n");
		return false;
	}

	// Make sure that we hold the interface:
	mI2CInterface->retain();

	return true;
}

bool KeyLargoPlatform::openI2C()
{
	bool		result;
	
	result = false;
	FailIf (NULL == mI2CInterface, Exit);

	// Open the interface and sets it in the wanted mode:
	FailIf (!mI2CInterface->openI2CBus(mI2CPort), Exit);

	result = true;

Exit:
	return result;
}

void KeyLargoPlatform::closeI2C ()
{
	// Closes the bus so other can access to it:
	mI2CInterface->closeI2CBus ();
}

bool KeyLargoPlatform::detachFromI2C()
{
	if (mI2CInterface) {
		mI2CInterface->release();
		mI2CInterface = NULL;
	}
	return (true);
}

#pragma mark ---------------------------
#pragma mark ¥ DBDMA Memory Address Acquisition Methods
#pragma mark ---------------------------

//	--------------------------------------------------------------------------------
IODBDMAChannelRegisters *	KeyLargoPlatform::GetInputChannelRegistersVirtualAddress ( IOService * dbdmaProvider ) {
	IOMemoryMap *				map;
	IOService *					parentOfParent;
	IODBDMAChannelRegisters *	ioBaseDMAInput = NULL;
	
	FailIf ( NULL == dbdmaProvider, Exit );
	IOLog ( "KeyLargoPlatform::GetInputChannelRegistersVirtualAddress i2s-a name is %s\n", dbdmaProvider->getName() );
	parentOfParent = (IOService*)dbdmaProvider->getParentEntry ( gIODTPlane );
	FailIf ( NULL == parentOfParent, Exit );
	IOLog ( "   parentOfParent name is %s\n", parentOfParent->getName() );
	map = parentOfParent->mapDeviceMemoryWithIndex ( AppleDBDMAAudio::kDBDMAInputIndex );
	FailIf ( NULL == map, Exit );
	ioBaseDMAInput = (IODBDMAChannelRegisters *) map->getVirtualAddress();
	debug3IOLog ( "ioBaseDMAInput %p is at physical %p\n", ioBaseDMAInput, (void*)map->getPhysicalAddress() );
	if ( NULL == ioBaseDMAInput ) { IOLog ( "KeyLargoPlatform::GetInputChannelRegistersVirtualAddress IODBDMAChannelRegisters NOT IN VIRTUAL SPACE\n" ); }
Exit:
	return ioBaseDMAInput;
}

//	--------------------------------------------------------------------------------
IODBDMAChannelRegisters *	KeyLargoPlatform::GetOutputChannelRegistersVirtualAddress ( IOService * dbdmaProvider ) {
	IOMemoryMap *				map;
	IOService *					parentOfParent;
	IODBDMAChannelRegisters *	ioBaseDMAOutput = NULL;
	
	FailIf ( NULL == dbdmaProvider, Exit );
	IOLog ( "KeyLargoPlatform::GetOutputChannelRegistersVirtualAddress i2s-a name is %s\n", dbdmaProvider->getName() );
	parentOfParent = (IOService*)dbdmaProvider->getParentEntry ( gIODTPlane );
	FailIf ( NULL == parentOfParent, Exit );
	IOLog ( "   parentOfParent name is %s\n", parentOfParent->getName() );
	map = parentOfParent->mapDeviceMemoryWithIndex ( AppleDBDMAAudio::kDBDMAOutputIndex );
	FailIf ( NULL == map, Exit );
	ioBaseDMAOutput = (IODBDMAChannelRegisters *) map->getVirtualAddress();
	debug3IOLog ( "ioBaseDMAOutput %p is at physical %p\n", ioBaseDMAOutput, (void*)map->getPhysicalAddress() );
	if ( NULL == ioBaseDMAOutput ) { IOLog ( "KeyLargoPlatform::GetOutputChannelRegistersVirtualAddress IODBDMAChannelRegisters NOT IN VIRTUAL SPACE\n" ); }
Exit:
	return ioBaseDMAOutput;
}

#pragma mark ---------------------------
#pragma mark ¥ User Client Support
#pragma mark ---------------------------

//	Hardware Utility user client support
UInt8 KeyLargoPlatform::userClientReadGPIO (UInt32 selector)
{
	UInt8 *				address;
	UInt8				gpioValue;

	gpioValue = NULL;
	address = getGPIOAddress (selector);
	if (NULL != address)
		gpioValue = gpioReadByte (address);

	return (gpioValue);
}

void KeyLargoPlatform::userClientWriteGPIO (UInt32 selector, UInt8 data)
{
	UInt8 *				address;
	UInt32				gpioValue;

	gpioValue = NULL;
	address = getGPIOAddress (selector);
	if (NULL != address)
		gpioWriteByte (address, data);

	return;
}

UInt8 *	KeyLargoPlatform::getGPIOAddress (UInt32 gpioSelector)
{
	UInt8 *				gpioAddress;

	gpioAddress = NULL;
	switch (gpioSelector) {
		case kHeadphoneMuteSel:			gpioAddress = hdpnMuteGpio;						break;
		case kHeadphoneDetecteSel:		gpioAddress = headphoneExtIntGpio;				break;
		case kAmplifierMuteSel:			gpioAddress = ampMuteGpio;						break;
		case kCodecResetSel:			gpioAddress = hwAnalogResetGpio;				break;
		case kLineInDetectSel:			gpioAddress = lineInExtIntGpio;					break;
		case kLineOutDetectSel:			gpioAddress = lineOutExtIntGpio;				break;
		case kLineOutMuteSel:			gpioAddress = lineOutMuteGpio;					break;
		case kDigitalOutDetectSel:		gpioAddress = digitalOutExtIntGpio;				break;
	}
	if ( NULL == gpioAddress ) {
		debug2IOLog ( "KeyLargoPlatform::getGPIOAddress ( %d ) returns NULL\n", (unsigned int)gpioSelector );
	}
	return gpioAddress;
}

bool	KeyLargoPlatform::getGPIOActiveState (UInt32 gpioSelector)
{
	bool				activeState;

	activeState = NULL;
	switch (gpioSelector) {
		case kHeadphoneMuteSel:			activeState = hdpnActiveState;					break;
		case kHeadphoneDetecteSel:		activeState = headphoneInsertedActiveState;		break;
		case kAmplifierMuteSel:			activeState = ampActiveState;					break;
		case kCodecResetSel:			activeState = hwAnalogResetActiveState;			break;
		case kLineInDetectSel:			activeState = lineInExtIntActiveState;			break;
		case kLineOutDetectSel:			activeState = lineOutExtIntActiveState;			break;
		case kDigitalOutDetectSel:		activeState = digitalOutExtIntActiveState;		break;
		case kLineOutMuteSel:			activeState = lineOutMuteActiveState;			break;
		default:
			debug2IOLog ( "KeyLargoPlatform::getGPIOActiveState ( %d ) UNKNOWN\n", (unsigned int)gpioSelector );
			break;
	}

	return activeState;
}

void KeyLargoPlatform::setGPIOActiveState ( UInt32 selector, UInt8 gpioActiveState )
{

	gpioActiveState = 0 == gpioActiveState ? FALSE : TRUE;
	switch ( selector ) {
		case kHeadphoneMuteSel:			hdpnActiveState = gpioActiveState;					break;
		case kHeadphoneDetecteSel:		headphoneInsertedActiveState = gpioActiveState;		break;
		case kAmplifierMuteSel:			ampActiveState = gpioActiveState;					break;
		case kCodecResetSel:			hwAnalogResetActiveState = gpioActiveState;			break;
		case kLineInDetectSel:			lineInExtIntActiveState = gpioActiveState;			break;
		case kLineOutDetectSel:			lineOutExtIntActiveState = gpioActiveState;			break;
		case kDigitalOutDetectSel:		digitalOutExtIntActiveState = gpioActiveState;		break;
		case kLineOutMuteSel:			lineOutMuteActiveState = gpioActiveState;			break;
		default:
			debug3IOLog ( "KeyLargoPlatform::setGPIOActiveState ( %d, %d ) UNKNOWN\n", (unsigned int)selector, gpioActiveState );
			break;
	}
	return;
}

bool	KeyLargoPlatform::checkGpioAvailable ( UInt32 selector )
{
	bool			result = FALSE;
	switch ( selector ) {
		case kHeadphoneMuteSel:			if ( NULL != hdpnMuteGpio ) { result = TRUE; }				break;
		case kHeadphoneDetecteSel:		if ( NULL != headphoneExtIntGpio ) { result = TRUE; }		break;
		case kAmplifierMuteSel:			if ( NULL != ampMuteGpio ) { result = TRUE; }				break;
		case kCodecResetSel:			if ( NULL != hwAnalogResetGpio ) { result = TRUE; }			break;
		case kLineInDetectSel:			if ( NULL != lineInExtIntGpio ) { result = TRUE; }			break;
		case kLineOutDetectSel:			if ( NULL != lineOutExtIntGpio ) { result = TRUE; }			break;
		case kDigitalOutDetectSel:		if ( NULL != digitalOutExtIntGpio ) { result = TRUE; }		break;
		case kLineOutMuteSel:			if ( NULL != lineOutMuteGpio ) { result = TRUE; }			break;
	}
	return result;
}

IORegistryEntry *KeyLargoPlatform::FindEntryByProperty (const IORegistryEntry * start, const char * key, const char * value) {
	OSIterator				*iterator;
	IORegistryEntry			*theEntry;
	IORegistryEntry			*tmpReg;
	OSData					*tmpData;

	theEntry = NULL;
	iterator = start->getChildIterator (gIODTPlane);
	FailIf (NULL == iterator, Exit);

	while (NULL == theEntry && (tmpReg = OSDynamicCast (IORegistryEntry, iterator->getNextObject ())) != NULL) {
		tmpData = OSDynamicCast (OSData, tmpReg->getProperty (key));
		if (NULL != tmpData && tmpData->isEqualTo (value, strlen (value))) {
			theEntry = tmpReg;
		}
	}

Exit:
	if (NULL != iterator) {
		iterator->release ();
	}
	return theEntry;
}
