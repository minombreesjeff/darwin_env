/*
 *  K2Platform.cpp
 *  
 *  Created by Aram Lindahl on Mon Mar 10 2003.
 *  Copyright (c) 2003 Apple Computer. All rights reserved.
 *
 */

#include "K2Platform.h"

#include "PlatformInterface.h"
#include "AudioHardwareConstants.h"
#include "AudioHardwareUtilities.h"
#include "AudioHardwareCommon.h"
#include "AppleOnboardAudio.h"

#define super PlatformInterface

#pragma mark ---------------------------
#pragma mark ¥ Platform Interface Methods
#pragma mark ---------------------------

OSDefineMetaClassAndStructors(K2Platform, PlatformInterface)

const char * 	K2Platform::kAppleK2pHandle								= "phK2";
const char * 	K2Platform::kAppleI2S0pHandle							= "phI0";
const char * 	K2Platform::kAppleGPIOpHandle							= "phGn";

const char * 	K2Platform::kAppleI2S_Enable 							= "platform-enable";					//	Ã
const char * 	K2Platform::kAppleI2S_Disable							= "platform-disable";					//	Ã
const char * 	K2Platform::kAppleI2S_ClockEnable						= "platform-clock-enable";				//	Ã
const char * 	K2Platform::kAppleI2S_ClockDisable						= "platform-clock-disable";				//	Ã
const char * 	K2Platform::kAppleI2S_Reset								= "platform-sw-reset";					//	Ã
const char * 	K2Platform::kAppleI2S_Run								= "platform-clear-sw-reset";			//	Ã
const char * 	K2Platform::kAppleI2S_CellEnable						= "platform-cell-enable";				//	Ã
const char * 	K2Platform::kAppleI2S_CellDisable						= "platform-cell-disable";				//	Ã
const char * 	K2Platform::kAppleI2S_GetEnable							= "platform-get-enable";				//	Ã
const char * 	K2Platform::kAppleI2S_GetClockEnable					= "platform-get-clock-enable";			//	Ã
const char * 	K2Platform::kAppleI2S_GetReset							= "platform-get-sw-reset";				//	Ã
const char * 	K2Platform::kAppleI2S_GetCellEnable						= "platform-get-cell-enable";			//	Ã

const char * 	K2Platform::kAppleGPIO_DisableSpeakerDetect				= "disable-audio-spkr-detect";			//	
const char * 	K2Platform::kAppleGPIO_EnableSpeakerDetect				= "enable-audio-spkr-detect";			//	
const char * 	K2Platform::kAppleGPIO_GetSpeakerDetect					= "platform-audio-spkr-detect";			//
const char * 	K2Platform::kAppleGPIO_RegisterSpeakerDetect			= "register-audio-spkr-detect";			//	
const char * 	K2Platform::kAppleGPIO_UnregisterSpeakerDetect			= "unregister-audio-spkr-detect";		//	

const char * 	K2Platform::kAppleGPIO_DisableDigitalInDetect			= "disable-audio-dig-in-det";			//	
const char * 	K2Platform::kAppleGPIO_EnableDigitalInDetect			= "enable-audio-dig-in-det";			//	
const char * 	K2Platform::kAppleGPIO_GetDigitalInDetect				= "platform-audio-dig-in-det";		
const char * 	K2Platform::kAppleGPIO_RegisterDigitalInDetect			= "register-audio-dig-in-det";			//	
const char * 	K2Platform::kAppleGPIO_UnregisterDigitalInDetect		= "unregister-audio-dig-in-det";		//	

const char * 	K2Platform::kAppleGPIO_GetDigitalInType					= "platform-audio-dig-in-type";		

const char * 	K2Platform::kAppleGPIO_DisableDigitalOutDetect			= "disable-audio-dig-out-detect";		//	
const char * 	K2Platform::kAppleGPIO_EnableDigitalOutDetect			= "enable-audio-dig-out-detect";		//	
const char * 	K2Platform::kAppleGPIO_GetDigitalOutDetect				= "platform-audio-dig-out-det";	
const char * 	K2Platform::kAppleGPIO_RegisterDigitalOutDetect			= "register-audio-dig-out-detect";		//	
const char * 	K2Platform::kAppleGPIO_UnregisterDigitalOutDetect		= "unregister-audio-dig-out-detect";	//	
	
const char * 	K2Platform::kAppleGPIO_GetDigitalOutType				= "platform-audio-dig-out-type";		//	

const char * 	K2Platform::kAppleGPIO_DisableLineInDetect				= "disable-linein-detect";				//	Ã
const char * 	K2Platform::kAppleGPIO_EnableLineInDetect				= "enable-linein-detect";				//	Ã
const char * 	K2Platform::kAppleGPIO_GetLineInDetect					= "platform-linein-detect";				//	Ã
const char * 	K2Platform::kAppleGPIO_RegisterLineInDetect				= "register-linein-detect";				//	Ã
const char * 	K2Platform::kAppleGPIO_UnregisterLineInDetect			= "unregister-linein-detect";			//	Ã

const char *	K2Platform::kAppleGPIO_DisableLineOutDetect				= "disable-lineout-detect";				//	Ã
const char * 	K2Platform::kAppleGPIO_EnableLineOutDetect				= "enable-lineout-detect";				//	Ã
const char * 	K2Platform::kAppleGPIO_GetLineOutDetect					= "platform-lineout-detect";			//	Ã
const char * 	K2Platform::kAppleGPIO_RegisterLineOutDetect			= "register-lineout-detect";			//	Ã
const char * 	K2Platform::kAppleGPIO_UnregisterLineOutDetect			= "unregister-lineout-detect";			//	Ã

const char * 	K2Platform::kAppleGPIO_DisableHeadphoneDetect			= "disable-headphone-detect";			//	Ã
const char * 	K2Platform::kAppleGPIO_EnableHeadphoneDetect			= "enable-headphone-detect";			//	Ã
const char * 	K2Platform::kAppleGPIO_GetHeadphoneDetect				= "platform-headphone-detect";			//	Ã
const char * 	K2Platform::kAppleGPIO_RegisterHeadphoneDetect			= "register-headphone-detect";			//	Ã
const char * 	K2Platform::kAppleGPIO_UnregisterHeadphoneDetect		= "unregister-headphone-detect";		//	Ã

const char * 	K2Platform::kAppleGPIO_SetHeadphoneMute					= "platform-headphone-mute";			//	Ã

const char * 	K2Platform::kAppleGPIO_SetAmpMute						= "platform-amp-mute";					//	Ã

const char * 	K2Platform::kAppleGPIO_SetAudioHwReset					= "platform-hw-reset";					//	Ã

const char * 	K2Platform::kAppleGPIO_SetAudioDigHwReset				= "platform-dig-hw-reset";				//	Ã

const char * 	K2Platform::kAppleGPIO_SetLineOutMute					= "platform-lineout-mute";				//	Ã

const char * 	K2Platform::kAppleGPIO_DisableCodecIRQ					= "disable-codec-irq";					//	Ã
const char * 	K2Platform::kAppleGPIO_EnableCodecIRQ					= "enable-codec-irq";					//	Ã
const char * 	K2Platform::kAppleGPIO_GetCodecIRQ						= "platform-codec-irq";					//	Ã
const char * 	K2Platform::kAppleGPIO_RegisterCodecIRQ					= "register-codec-irq";					//	Ã
const char * 	K2Platform::kAppleGPIO_UnregisterCodecIRQ				= "unregister-codec-irq";				//	Ã

const char * 	K2Platform::kAppleGPIO_EnableCodecErrorIRQ				= "enable-codec-error-irq";				//	Ã
const char * 	K2Platform::kAppleGPIO_DisableCodecErrorIRQ				= "disable-codec-error-irq";			//	Ã
const char * 	K2Platform::kAppleGPIO_GetCodecErrorIRQ					= "platform-codec-error-irq";			//	Ã
const char * 	K2Platform::kAppleGPIO_RegisterCodecErrorIRQ			= "register-codec-error-irq";			//	Ã
const char * 	K2Platform::kAppleGPIO_UnregisterCodecErrorIRQ			= "unregister-codec-error-irq";			//	Ã

const char * 	K2Platform::kAppleGPIO_SetCodecClockMux					= "platform-codec-clock-mux";			//	Ã

const char * 	K2Platform::kAppleGPIO_SetCodecInputDataMux				= "platform-codec-input-data-mu";		//	Ã

//	--------------------------------------------------------------------------------
bool K2Platform::init (IOService* device, AppleOnboardAudio* provider, UInt32 inDBDMADeviceIndex) {
	IORegistryEntry			*sound;
	bool					result;
	OSData					*osdata;
	IORegistryEntry			*i2S;
	IORegistryEntry			*macIO;
	
	debugIOLog ( "+ K2Platform::init\n" );
	result = super::init (device, provider, inDBDMADeviceIndex);
	FailIf ( !result, Exit );

	debug2IOLog ( "    about to waitForService on mK2Service %p\n", mK2Service );
	mK2Service = IOService::waitForService ( IOService::serviceMatching ( "AppleK2" ) );
	debug2IOLog ( "    mK2Service %p\n", mK2Service );
	
	sound = device;

	FailWithAction (!sound, result = false, Exit);
	debug2IOLog ("K2 - sound's name is %s\n", sound->getName ());

	mI2S = sound->getParentEntry (gIODTPlane);
	FailWithAction (!mI2S, result = false, Exit);
	debug2IOLog ("K2Platform - i2s's name is %s\n", mI2S->getName ());

	osdata = OSDynamicCast ( OSData, mI2S->getProperty ( "AAPL,phandle" ) );
	mI2SPHandle = *((UInt32*)osdata->getBytesNoCopy());
	debug2IOLog ( "mI2SPHandle %lX", mI2SPHandle );
	
	osdata = OSDynamicCast ( OSData, mI2S->getProperty ( "reg" ) );
	mI2SOffset = *((UInt32*)osdata->getBytesNoCopy());
		
	i2S = mI2S->getParentEntry (gIODTPlane);
	FailWithAction (!i2S, result = false, Exit);
	debug2IOLog ("mI2S - parent name is %s\n", i2S->getName ());

	macIO = i2S->getParentEntry (gIODTPlane);
	FailWithAction (!macIO, result = false, Exit);
	debug2IOLog ("i2S - parent name is %s\n", macIO->getName ());
	
	osdata = OSDynamicCast ( OSData, macIO->getProperty ( "AAPL,phandle" ) );
	mMacIOPHandle = *((UInt32*)osdata->getBytesNoCopy());
	debug2IOLog ( "mMacIOPHandle %lX", mMacIOPHandle );

	osdata = OSDynamicCast ( OSData, macIO->getProperty ( "reg" ) );
	mMacIOOffset = *((UInt32*)osdata->getBytesNoCopy());

	mAppleGPIO_AnalogCodecReset = kGPIO_Unknown;
	mAppleGPIO_DigitalCodecReset = kGPIO_Unknown;
	mAppleGPIO_HeadphoneMute = kGPIO_Unknown;
	mAppleGPIO_AmpMute = kGPIO_Unknown;
	mAppleGPIO_LineOutMute = kGPIO_Unknown;
	mAppleGPIO_CodecClockMux = kGPIO_Unknown;
	mAppleGPIO_CodecInputDataMux = kGPIO_Unknown;

	debugIOLog ( "about to findAndAttachI2C\n" );
	result = findAndAttachI2C();
	if ( !result ) { debugIOLog ( "K2Platform::init COULD NOT FIND I2C\n" ); }
	FailIf ( !result, Exit );
#ifndef kBUILD_FOR_DIRECT_I2S_HW_ACCESS	
	debugIOLog ( "about to findAndAttachI2S\n" );
	result = findAndAttachI2S();
	if ( !result ) { debugIOLog ( "K2Platform::init COULD NOT FIND I2S\n" ); }
	FailIf ( !result, Exit );
#endif
	IOMemoryMap *				map;
	IOMemoryDescriptor *		theDescriptor;

	theDescriptor = IOMemoryDescriptor::withPhysicalAddress ( (IOPhysicalAddress)kAUDIO_MAC_IO_BASE_ADDRESS, kAUDIO_MAC_IO_SIZE, kIODirectionOutIn );
	FailIf ( NULL == theDescriptor, Exit );
	map = theDescriptor->map ();
	FailIf ( NULL == map, Exit );
	mHwPtr = (UInt8*)map->getVirtualAddress();
	FailIf ( NULL == mHwPtr, Exit );
	
	theDescriptor = IOMemoryDescriptor::withPhysicalAddress ( (IOPhysicalAddress)kAUDIO_I2S_BASE_ADDRESS, kAUDIO_I2S_SIZE, kIODirectionOutIn );
	FailIf ( NULL == theDescriptor, Exit );
	map = theDescriptor->map ();
	FailIf ( NULL == map, Exit );
	mHwI2SPtr = (UInt8*)map->getVirtualAddress();
	FailIf ( NULL == mHwI2SPtr, Exit );
	
	mFcr1						= (UInt32*)&mHwPtr[kAUDIO_MAC_IO_FCR1];
	mFcr3						= (UInt32*)&mHwPtr[kAUDIO_MAC_IO_FCR3];
	mSerialFormat				= (UInt32*)&mHwI2SPtr[kAUDIO_I2S_SERIAL_FORMAT];
	mI2SIntCtrl					= (UInt32*)&mHwI2SPtr[kAUDIO_I2S_INTERRUPT_CONTROL];
	mDataWordSize				= (UInt32*)&mHwI2SPtr[kAUDIO_I2S_DATA_WORD_SIZES];
	mFrameCounter				= (UInt32*)&mHwI2SPtr[kAUDIO_I2S_FRAME_COUNTER];
	mGPIO_inputDataMuxSelect	= (UInt8*)&mHwPtr[kAUDIO_GPIO_INPUT_DATA_MUX_SELECT];
	mGPIO_lineInSense			= (UInt8*)&mHwPtr[kAUDIO_GPIO_LINE_IN_SENSE];
	mGPIO_digitalCodecErrorIrq	= (UInt8*)&mHwPtr[kAUDIO_GPIO_CODEC_ERROR_IRQ];
	mGPIO_digitalCodecReset		= (UInt8*)&mHwPtr[kAUDIO_GPIO_DIGITAL_CODEC_RESET];
	mGPIO_lineOutSense			= (UInt8*)&mHwPtr[kAUDIO_GPIO_LINE_OUT_SENSE];
	mGPIO_headphoneSense		= (UInt8*)&mHwPtr[kAUDIO_GPIO_HEADPHONE_SENSE];
	mGPIO_digitalCodecIrq		= (UInt8*)&mHwPtr[kAUDIO_GPIO_CODEC_IRQ];
	mGPIO_headphoneMute			= (UInt8*)&mHwPtr[kAUDIO_GPIO_HEADPHONE_MUTE];
	mGPIO_analogCodecReset		= (UInt8*)&mHwPtr[kAUDIO_GPIO_ANALOG_CODEC_RESET];
	mGPIO_lineOutMute			= (UInt8*)&mHwPtr[kAUDIO_GPIO_LINE_OUT_MUTE];
	mGPIO_mclkMuxSelect			= (UInt8*)&mHwPtr[kAUDIO_GPIO_CLOCK_MUX_SELECT];
	mGPIO_speakerMute			= (UInt8*)&mHwPtr[kAUDIO_GPIO_AMPLIFIER_MUTE];
Exit:

	debug2IOLog ( "- K2Platform::init returns %d\n", result );
	return result;
}

//	--------------------------------------------------------------------------------
void K2Platform::free()
{
	debugIOLog ("+ K2Platform::free()\n");

	detachFromI2C();
	//detachFromI2S();

	super::free();

	debugIOLog ("- K2Platform::free()\n");
}

#pragma mark ---------------------------
#pragma mark ¥ Codec Methods	
#pragma mark ---------------------------

//	--------------------------------------------------------------------------------
bool K2Platform::writeCodecRegister(UInt8 address, UInt8 subAddress, UInt8 *data, UInt16 len, BusMode mode) {
	bool success = false;

#ifdef	kVERBOSE_LOG
	debug6IOLog ( "+ K2Platform::writeCodecRegister ( %X, %X, %p, %d, %d )\n", address, subAddress, data, len, mode );
#endif
	FailIf ( NULL == data, Exit );
	if (openI2C()) {
		switch (mode) {
			case kI2C_StandardMode:			mI2CInterface->setStandardMode();			break;
			case kI2C_StandardSubMode:		mI2CInterface->setStandardSubMode();		break;
			case kI2C_CombinedMode:			mI2CInterface->setCombinedMode();			break;
			default:
				debugIOLog ("K2Platform::writeCodecRegister() unknown bus mode!\n");
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
#ifdef kVERBOSE_LOG	
		debug6IOLog ( " mI2CInterface->writeI2CBus ( %X, %X, %p, %d ), data->%X\n", (unsigned int)address, (unsigned int)subAddress, data, (unsigned int)len, *data );
#endif
		success = mI2CInterface->writeI2CBus (address >> 1, subAddress, data, len);
		
		if (!success) { 
			debug5IOLog ( "K2Platform::writeCodecRegister( %X, %X, %p %d), mI2CInterface->writeI2CBus returned false.\n", address, subAddress, data, len );
		}
Exit:
		closeI2C();
	} else {
		debugIOLog ("K2Platform::writeCodecRegister() couldn't open the I2C bus!\n");
	}

#ifdef kVERBOSE_LOG	
	debug7IOLog ( "- K2Platform::writeCodecRegister ( %X, %X, %p, %d, %d ) returns %d\n", address, subAddress, data, len, mode, success );
#endif
	return success;
}

//	--------------------------------------------------------------------------------
bool K2Platform::readCodecRegister(UInt8 address, UInt8 subAddress, UInt8 *data, UInt16 len, BusMode mode) {
	bool success = false;

#ifdef kVERBOSE_LOG	
	debug6IOLog ( "+ K2Platform::readCodecRegister ( %X, %X, %p, %d, %d )\n", address, subAddress, data, len, mode );
#endif
	if (openI2C()) {
		switch (mode) {
			case kI2C_StandardMode:			mI2CInterface->setStandardMode();			break;
			case kI2C_StandardSubMode:		mI2CInterface->setStandardSubMode();		break;
			case kI2C_CombinedMode:			mI2CInterface->setCombinedMode();			break;
			default:
				debugIOLog ("K2Platform::readCodecRegister() unknown bus mode!\n");
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

		if (!success) debugIOLog ("K2Platform::readCodecRegister(), mI2CInterface->writeI2CBus returned false.\n");
Exit:
		closeI2C();
	} else {
		debugIOLog ("K2Platform::readCodecRegister() couldn't open the I2C bus!\n");
	}
#ifdef kVERBOSE_LOG	
	debug7IOLog ( "- K2Platform::readCodecRegister ( %X, %X, %p, %d, %d ) returns %d\n", address, subAddress, data, len, mode, success );
#endif
	return success;
}

//	--------------------------------------------------------------------------------
IOReturn K2Platform::setCodecReset ( CODEC_RESET target, GpioAttributes reset ) {
	IOReturn				result;

//	debug3IOLog ( "K2Platform::setCodecReset ( %d, %d )\n", (unsigned int)target, (unsigned int)reset );
	switch ( target ) {
		case kCODEC_RESET_Analog:	result = writeGpioState ( kGPIO_Selector_AnalogCodecReset, reset );		break;
		case kCODEC_RESET_Digital:	result = writeGpioState ( kGPIO_Selector_DigitalCodecReset, reset );	break;
		default:					result = kIOReturnBadArgument;											break;
	}
	if ( kIOReturnSuccess != result ) {
		debug4IOLog ( "- K2Platform::setCodecReset ( %d, %d ) returns %X\n", (unsigned int)target, (unsigned int)reset, (unsigned int)result );
	}
	return result;
}


//	--------------------------------------------------------------------------------
GpioAttributes K2Platform::getCodecReset ( CODEC_RESET target ) {
	GpioAttributes		result = kGPIO_Unknown;
	
	switch ( target ) {
		case kCODEC_RESET_Analog:		result = readGpioState ( kGPIO_Selector_AnalogCodecReset );			break;
		case kCODEC_RESET_Digital:		result = readGpioState ( kGPIO_Selector_DigitalCodecReset );		break;
		default:						result = kGPIO_Unknown;												break;
	}
	return result;
}

#pragma mark ---------------------------
#pragma mark ¥ I2S
#pragma mark ---------------------------

//	--------------------------------------------------------------------------------
IOReturn K2Platform::setI2SEnable (bool enable) {
	IOReturn				result;
#ifndef kBUILD_FOR_DIRECT_I2SEnable_HW_ACCESS
	const OSSymbol*			funcSymbolName;
#endif

//	debug2IOLog( "+ K2Platform::setI2SEnable enable=%d\n", enable );

	result = kIOReturnError;
#ifdef kBUILD_FOR_DIRECT_I2SEnable_HW_ACCESS
	UInt32			data;
	
	if ( mFcr1 ) {
		if ( enable ) {
			data = (unsigned int)OSReadSwapInt32 ( mFcr1, 0 );
			data |= ( 1 << 13 );
			OSWriteSwapInt32 ( mFcr1, 0, data );
		} else {
			data = (unsigned int)OSReadSwapInt32 ( mFcr1, 0 );
			data &= ~( 1 << 13 );
			OSWriteSwapInt32 ( mFcr1, 0, data );
		}
		result = kIOReturnSuccess;
	}
#else
	if ( mK2Service ) {
		if ( enable ) {
			funcSymbolName = makeFunctionSymbolName( kAppleI2S_Enable, mI2SPHandle );
			result = mK2Service->callPlatformFunction ( funcSymbolName, false, (void *)0, (void *)(UInt32)0, 0, 0 );
		} else {
			funcSymbolName = makeFunctionSymbolName( kAppleI2S_Disable, mI2SPHandle );
			result = mK2Service->callPlatformFunction ( funcSymbolName, false, (void *)0, (void *)(UInt32)0, 0, 0 );
		}
	}
#endif
//	debug3IOLog( "- K2Platform::setI2SEnable enable=%d returns %d\n", enable, result );

	return result;
}

//	--------------------------------------------------------------------------------
bool K2Platform::getI2SEnable () {
#ifndef kBUILD_FOR_DIRECT_FCR_HW_ACCESS
	const OSSymbol*			funcSymbolName;
	IOReturn				result;
#endif
	UInt32					value = 0;

#ifdef kBUILD_FOR_DIRECT_FCR_HW_ACCESS
	if ( NULL != mFcr1 ) {
		value = ( (unsigned int)OSReadSwapInt32 ( mFcr1, 0 ) >> 13 ) & 0x00000001;
	}
#else
	if ( mK2Service ) {
		funcSymbolName = makeFunctionSymbolName( kAppleI2S_GetEnable, mI2SPHandle );
		result = mK2Service->callPlatformFunction ( funcSymbolName, false, (void *)&value, (void *)(UInt32)0, 0, 0 );
		FailIf ( kIOReturnSuccess != result, Exit );
	}
Exit:
#endif
	return ( 0 != value );
}

//	--------------------------------------------------------------------------------
IOReturn K2Platform::setI2SClockEnable (bool enable) {
	IOReturn				result;
#ifndef kBUILD_FOR_DIRECT_I2SClockEnable_HW_ACCESS
	const OSSymbol*			funcSymbolName;
#endif

//	debug2IOLog( "+ K2Platform::setI2SClockEnable enable=%d\n", enable );

	result = kIOReturnError;
#ifdef kBUILD_FOR_DIRECT_I2SClockEnable_HW_ACCESS
	UInt32			data;
	
	if ( mFcr1 ) {
		if ( enable ) {
			data = (unsigned int)OSReadSwapInt32 ( mFcr1, 0 );
			data |= ( 1 << 12 );
			OSWriteSwapInt32 ( mFcr1, 0, data );
		} else {
			data = (unsigned int)OSReadSwapInt32 ( mFcr1, 0 );
			data &= ~( 1 << 12 );
			OSWriteSwapInt32 ( mFcr1, 0, data );
		}
		result = kIOReturnSuccess;
	}
#else
	if ( mK2Service ) {
		if ( enable ) {
			funcSymbolName = makeFunctionSymbolName( kAppleI2S_ClockEnable, mI2SPHandle );
			result = mK2Service->callPlatformFunction ( funcSymbolName, false, (void *)0, (void *)0, 0, 0 );
		} else {
			funcSymbolName = makeFunctionSymbolName( kAppleI2S_ClockDisable, mI2SPHandle );
			result = mK2Service->callPlatformFunction ( funcSymbolName, false, (void *)0, (void *)0, 0, 0 );
		}
		if ( kIOReturnSuccess == result ) mAppleI2S_ClockEnable = enable;
	}
#endif
//	debug3IOLog( "- K2Platform::setI2SClockEnable enable=%d returns %d\n", enable, result );
	LogFCR ();
	LogI2S ();
	LogGPIO ();

	return result;
}

//	--------------------------------------------------------------------------------
bool K2Platform::getI2SClockEnable () {
#ifndef kBUILD_FOR_DIRECT_FCR_HW_ACCESS
	const OSSymbol*			funcSymbolName;
	IOReturn				result;
#endif
	UInt32					value = 0;

#ifdef kBUILD_FOR_DIRECT_FCR_HW_ACCESS
	if ( NULL != mFcr1 ) {
		value = ( (unsigned int)OSReadSwapInt32 ( mFcr1, 0 ) >> 12 ) & 0x00000001;
	}
#else
	if ( mK2Service ) {
		funcSymbolName = makeFunctionSymbolName( kAppleI2S_GetClockEnable, mI2SPHandle );
		result = mK2Service->callPlatformFunction ( funcSymbolName, false, (void *)&value, (void *)(UInt32)0, 0, 0 );
		FailIf ( kIOReturnSuccess != result, Exit );
	}
Exit:
#endif
	return ( 0 != value );
}

//	--------------------------------------------------------------------------------
IOReturn K2Platform::setI2SCellEnable (bool enable) {
	IOReturn				result;
#ifndef kBUILD_FOR_DIRECT_I2SCellEnable_HW_ACCESS
	const OSSymbol*			funcSymbolName;
#endif
	
//	debug2IOLog("+ K2Platform::setI2SCellEnable enable=%d\n",enable);
	
#ifdef kBUILD_FOR_DIRECT_I2SCellEnable_HW_ACCESS
	UInt32			data;
	
	if ( mFcr1 ) {
		if ( enable ) {
			data = (unsigned int)OSReadSwapInt32 ( mFcr1, 0 );
			data |= ( 1 << 10 );
			OSWriteSwapInt32 ( mFcr1, 0, data );
		} else {
			data = (unsigned int)OSReadSwapInt32 ( mFcr1, 0 );
			data &= ~( 1 << 10 );
			OSWriteSwapInt32 ( mFcr1, 0, data );
		}
		result = kIOReturnSuccess;
	}
#else
	result = kIOReturnError;
	if ( mK2Service ) {
		if ( enable ) {
			funcSymbolName = makeFunctionSymbolName( kAppleI2S_CellEnable, mI2SPHandle );
			result = mK2Service->callPlatformFunction ( funcSymbolName, false, (void *)0, (void *)0, 0, 0 );
		} else {
			funcSymbolName = makeFunctionSymbolName( kAppleI2S_CellDisable, mI2SPHandle );
			result = mK2Service->callPlatformFunction ( funcSymbolName, false, (void *)0, (void *)0, 0, 0 );
		}
		if ( kIOReturnSuccess == result ) mAppleI2S_CellEnable = enable;
	}
#endif
//	debug2IOLog("- K2Platform::setI2SCellEnable result = %x\n",result);

	return result;
}

//	--------------------------------------------------------------------------------
bool K2Platform::getI2SCellEnable () {
#ifndef kBUILD_FOR_DIRECT_I2SCellEnable_HW_ACCESS
	const OSSymbol*			funcSymbolName;
	IOReturn				result;
#endif
	UInt32					value = 0;

#ifdef kBUILD_FOR_DIRECT_I2SCellEnable_HW_ACCESS
	if ( NULL != mFcr1 ) {
		value = ( (unsigned int)OSReadSwapInt32 ( mFcr1, 0 ) >> 10 ) & 0x00000001;
	}
#else
	if ( mK2Service ) {
		funcSymbolName = makeFunctionSymbolName( kAppleI2S_GetCellEnable, mI2SPHandle );
		result = mK2Service->callPlatformFunction ( funcSymbolName, false, (void *)&value, (void *)(UInt32)0, 0, 0 );
		FailIf ( kIOReturnSuccess != result, Exit );
	}
Exit:
#endif
	return ( 0 != value );
}

//	--------------------------------------------------------------------------------
IOReturn K2Platform::setSerialFormatRegister (UInt32 serialFormat) {
	IOReturn				result;

#ifdef kBUILD_FOR_DIRECT_I2S_HW_ACCESS
	if ( mSerialFormat ) {
		OSWriteSwapInt32 ( mSerialFormat, 0, serialFormat );
		result = kIOReturnSuccess;
	}
#else
	result = kIOReturnError;
	if ( mI2SInterface ) {
		result = mI2SInterface->callPlatformFunction ( kI2SSetSerialFormatReg, false, (void *)mI2SOffset, (void *)serialFormat, 0, 0 );
	}
#endif
	return result;
}

//	--------------------------------------------------------------------------------
UInt32 K2Platform::getSerialFormatRegister () {
	UInt32					serialFormat = 0;

#ifdef kBUILD_FOR_DIRECT_I2S_HW_ACCESS
	if ( NULL != mSerialFormat ) serialFormat = (unsigned int)OSReadSwapInt32 ( mSerialFormat, 0 );
#else
	if ( mI2SInterface ) {
		mI2SInterface->callPlatformFunction ( kI2SGetSerialFormatReg, false, (void *)mI2SOffset, (void *)&serialFormat, 0, 0 );
	}
#endif
	return serialFormat;
}

//	--------------------------------------------------------------------------------
IOReturn K2Platform::setDataWordSizes (UInt32 dataWordSizes) {
	IOReturn				result;

	result = kIOReturnError;
#ifdef kBUILD_FOR_DIRECT_I2S_HW_ACCESS
	if ( mDataWordSize ) {
		OSWriteSwapInt32 ( mDataWordSize, 0, dataWordSizes );
		result = kIOReturnSuccess;
	}
#else
	if ( mI2SInterface ) {
		result = mI2SInterface->callPlatformFunction ( kI2SSetDataWordSizesReg, false, (void *)mI2SOffset, (void *)dataWordSizes, 0, 0 );
		debug2IOLog("K2Platform::setDataWordSizes = %x \n",result);
	}
#endif
	return result;
}

//	--------------------------------------------------------------------------------
UInt32 K2Platform::getDataWordSizes () {
	UInt32					dataWordSizes = 0;

#ifdef kBUILD_FOR_DIRECT_I2S_HW_ACCESS
	if ( NULL != mDataWordSize ) dataWordSizes = (unsigned int)OSReadSwapInt32 ( mDataWordSize, 0 );
#else
	if ( mI2SInterface ) {
		mI2SInterface->callPlatformFunction ( kI2SGetDataWordSizesReg, false, (void *)mI2SOffset, (void *)&dataWordSizes, 0, 0 );
	}
#endif
	return dataWordSizes;
}

//	--------------------------------------------------------------------------------
IOReturn K2Platform::setI2SIOMIntControl (UInt32 intCntrl) {
	IOReturn				result;

	result = kIOReturnError;
#ifdef kBUILD_FOR_DIRECT_I2S_HW_ACCESS
	if ( mI2SIntCtrl ) {
		OSWriteSwapInt32 ( mI2SIntCtrl, 0, intCntrl );
		result = kIOReturnSuccess;
	}
#else
	if ( mI2SInterface ) {
		result = mI2SInterface->callPlatformFunction ( kI2SSetIntCtlReg, false, (void *)mI2SOffset, (void *)intCntrl, 0, 0 );
	}
#endif
	return result;
}

//	--------------------------------------------------------------------------------
UInt32 K2Platform::getI2SIOMIntControl () {
	UInt32					i2sIntCntrl = 0;

#ifdef kBUILD_FOR_DIRECT_I2S_HW_ACCESS
	if ( NULL != mI2SIntCtrl ) i2sIntCntrl = (unsigned int)OSReadSwapInt32 ( mI2SIntCtrl, 0 );
#else
	if ( mI2SInterface ) {
		mI2SInterface->callPlatformFunction ( kI2SGetIntCtlReg, false, (void *)mI2SOffset, (void *)&i2sIntCntrl, 0, 0 );
	}
#endif
	return i2sIntCntrl;
}

//	--------------------------------------------------------------------------------
IOReturn K2Platform::setFrameCount (UInt32 value) {
	IOReturn				result;
// do not debugIOLog in here it screws up the hal timing and causes stuttering audio
	result = kIOReturnError;
#ifdef kBUILD_FOR_DIRECT_I2S_HW_ACCESS
	if ( mFrameCounter ) {
		OSWriteSwapInt32 ( mFrameCounter, 0, value );
		result = kIOReturnSuccess;
	}
#else
	if ( mI2SInterface ) {
		result = mI2SInterface->callPlatformFunction ( kI2SSetFrameCountReg, false, (void *)mI2SOffset, (void *)value, 0, 0 );
	}
#endif
	return result;
}

//	--------------------------------------------------------------------------------
UInt32 K2Platform::getFrameCount () {
	UInt32					frameCount = 0;

#ifdef kBUILD_FOR_DIRECT_I2S_HW_ACCESS
	if ( NULL != mFrameCounter ) frameCount = (unsigned int)OSReadSwapInt32 ( mFrameCounter, 0 );
#else
	if ( mI2SInterface ) {
		mI2SInterface->callPlatformFunction ( kI2SGetFrameCountReg, false, (void *)mI2SOffset, (void *)&frameCount, 0, 0 );
	}
#endif
	return frameCount;
}

#pragma mark ---------------------------
#pragma mark ¥ GPIO
#pragma mark ---------------------------

//	--------------------------------------------------------------------------------
IOReturn K2Platform::registerInterruptHandler ( IOService * theDevice, void * interruptHandler, PlatformInterruptSource source ) {
	const OSSymbol * 		selector = OSSymbol::withCString ( kIOPFInterruptRegister );
	const OSSymbol *		funcSymbolName = NULL;
	const OSSymbol *		enFuncSymbolName = NULL;
	IOReturn				result;

	result = kIOReturnError;
	if ( mK2Service ) {
		switch ( source ) {
			case kHeadphoneDetectInterrupt: 	
				funcSymbolName = makeFunctionSymbolName( kAppleGPIO_RegisterHeadphoneDetect, mI2SPHandle );	
				enFuncSymbolName = makeFunctionSymbolName( kAppleGPIO_EnableHeadphoneDetect, mI2SPHandle );		
				break;
			case kSpeakerDetectInterrupt: 		
				funcSymbolName = makeFunctionSymbolName( kAppleGPIO_RegisterSpeakerDetect, mI2SPHandle );
				enFuncSymbolName = makeFunctionSymbolName(kAppleGPIO_EnableSpeakerDetect, mI2SPHandle );		
				break;
			case kCodecInterrupt: 				
				funcSymbolName = makeFunctionSymbolName( kAppleGPIO_RegisterCodecIRQ, mI2SPHandle );
				enFuncSymbolName = makeFunctionSymbolName(kAppleGPIO_EnableCodecIRQ, mI2SPHandle );			
				break;
			case kCodecErrorDetectInterrupt: 	
				funcSymbolName = makeFunctionSymbolName( kAppleGPIO_RegisterCodecErrorIRQ, mI2SPHandle );
				enFuncSymbolName = makeFunctionSymbolName(kAppleGPIO_EnableCodecErrorIRQ, mI2SPHandle );			
				break;
			case kLineInputDetectInterrupt: 	
				funcSymbolName = makeFunctionSymbolName( kAppleGPIO_RegisterLineInDetect, mI2SPHandle );
				enFuncSymbolName = makeFunctionSymbolName(kAppleGPIO_EnableLineInDetect, mI2SPHandle );		
				break;
			case kLineOutputDetectInterrupt: 	
				funcSymbolName = makeFunctionSymbolName( kAppleGPIO_RegisterLineOutDetect, mI2SPHandle );
				enFuncSymbolName = makeFunctionSymbolName(kAppleGPIO_EnableLineOutDetect, mI2SPHandle );		
				break;
			case kDigitalInDetectInterrupt: 	
				funcSymbolName = makeFunctionSymbolName( kAppleGPIO_RegisterDigitalOutDetect, mI2SPHandle );
				enFuncSymbolName = makeFunctionSymbolName(kAppleGPIO_EnableDigitalOutDetect, mI2SPHandle );	
				break;
			case kDigitalOutDetectInterrupt: 	
				funcSymbolName = makeFunctionSymbolName( kAppleGPIO_RegisterDigitalInDetect, mI2SPHandle );
				enFuncSymbolName = makeFunctionSymbolName(kAppleGPIO_EnableDigitalInDetect, mI2SPHandle );		
				break;
			case kUnknownInterrupt:
			default:							
				debugIOLog ( "Attempt to register unknown interrupt source\n" );	
				break;
		}
		FailIf ( NULL == funcSymbolName, Exit );
		FailIf ( NULL == enFuncSymbolName, Exit );
		result = mK2Service->callPlatformFunction ( funcSymbolName, true, interruptHandler, this, (void*)source, (void*)selector);
		FailIf ( kIOReturnSuccess != result, Exit );
		result = mK2Service->callPlatformFunction ( enFuncSymbolName, true, interruptHandler, this, (void*)source, (void*)selector);
		FailIf ( kIOReturnSuccess != result, Exit );
	}
Exit:
	return result;
}

//	--------------------------------------------------------------------------------
IOReturn K2Platform::unregisterInterruptHandler ( IOService * theDevice, void * interruptHandler, PlatformInterruptSource source ) {
	const OSSymbol * 		selector = OSSymbol::withCString ( kIOPFInterruptUnRegister );
	const OSSymbol *		funcSymbolName = NULL;
	const OSSymbol *		enFuncSymbolName = NULL;
	IOReturn				result;

	result = kIOReturnError;
	FailIf (TRUE || NULL == mK2Service, Exit);

	switch ( source ) {
		case kHeadphoneDetectInterrupt: 	
			funcSymbolName = makeFunctionSymbolName( kAppleGPIO_UnregisterHeadphoneDetect, mI2SPHandle );	
			enFuncSymbolName = makeFunctionSymbolName( kAppleGPIO_DisableHeadphoneDetect, mI2SPHandle );		
			break;
		case kSpeakerDetectInterrupt: 		
			funcSymbolName = makeFunctionSymbolName( kAppleGPIO_UnregisterSpeakerDetect, mI2SPHandle );
			enFuncSymbolName = makeFunctionSymbolName(kAppleGPIO_DisableSpeakerDetect, mI2SPHandle );		
			break;
		case kCodecInterrupt: 				
			funcSymbolName = makeFunctionSymbolName( kAppleGPIO_UnregisterCodecIRQ, mI2SPHandle );
			enFuncSymbolName = makeFunctionSymbolName(kAppleGPIO_DisableCodecIRQ, mI2SPHandle );			
			break;
		case kCodecErrorDetectInterrupt: 	
			funcSymbolName = makeFunctionSymbolName( kAppleGPIO_UnregisterCodecErrorIRQ, mI2SPHandle );
			enFuncSymbolName = makeFunctionSymbolName(kAppleGPIO_DisableCodecErrorIRQ, mI2SPHandle );			
			break;
		case kLineInputDetectInterrupt: 	
			funcSymbolName = makeFunctionSymbolName( kAppleGPIO_UnregisterLineInDetect, mI2SPHandle );
			enFuncSymbolName = makeFunctionSymbolName(kAppleGPIO_DisableLineInDetect, mI2SPHandle );		
			break;
		case kLineOutputDetectInterrupt: 	
			funcSymbolName = makeFunctionSymbolName( kAppleGPIO_UnregisterLineOutDetect, mI2SPHandle );
			enFuncSymbolName = makeFunctionSymbolName(kAppleGPIO_DisableLineOutDetect, mI2SPHandle );		
			break;
		case kDigitalInDetectInterrupt: 	
			funcSymbolName = makeFunctionSymbolName( kAppleGPIO_UnregisterDigitalOutDetect, mI2SPHandle );
			enFuncSymbolName = makeFunctionSymbolName(kAppleGPIO_DisableDigitalOutDetect, mI2SPHandle );	
			break;
		case kDigitalOutDetectInterrupt: 	
			funcSymbolName = makeFunctionSymbolName( kAppleGPIO_UnregisterDigitalInDetect, mI2SPHandle );
			enFuncSymbolName = makeFunctionSymbolName(kAppleGPIO_DisableDigitalInDetect, mI2SPHandle );		
			break;
		case kUnknownInterrupt:
		default:							
			debugIOLog ( "Attempt to register unknown interrupt source\n" );	
			break;
	}

	FailIf ( NULL == funcSymbolName, Exit );
	FailIf ( NULL == enFuncSymbolName, Exit );
	result = mK2Service->callPlatformFunction ( enFuncSymbolName, true, interruptHandler, theDevice, (void*)source, (void*)selector);
	FailIf ( kIOReturnSuccess != result, Exit );
	result = mK2Service->callPlatformFunction ( funcSymbolName, true, interruptHandler, theDevice, (void*)source, (void*)selector);

Exit:
	return result;
}

//	--------------------------------------------------------------------------------
GpioAttributes 	K2Platform::getHeadphoneConnected() {
	return readGpioState ( kGPIO_Selector_HeadphoneDetect );
}
	
//	--------------------------------------------------------------------------------
GpioAttributes 	K2Platform::getSpeakerConnected() {
	return readGpioState ( kGPIO_Selector_SpeakerDetect );
}
	
//	--------------------------------------------------------------------------------
GpioAttributes 	K2Platform::getLineOutConnected() {
	return readGpioState ( kGPIO_Selector_LineOutDetect );
}

//	--------------------------------------------------------------------------------
GpioAttributes 	K2Platform::getLineInConnected() {
	return readGpioState ( kGPIO_Selector_LineInDetect );
}

//	--------------------------------------------------------------------------------
GpioAttributes 	K2Platform::getDigitalOutConnected() {
	return readGpioState ( kGPIO_Selector_DigitalOutDetect );
}

//	--------------------------------------------------------------------------------
GpioAttributes 	K2Platform::getDigitalOutTypeConnected() {
	return readGpioState ( kGPIO_Selector_DigitalOutType );
}

//	--------------------------------------------------------------------------------
GpioAttributes 	K2Platform::getDigitalInConnected() {
	return readGpioState ( kGPIO_Selector_DigitalInDetect );
}

//	--------------------------------------------------------------------------------
GpioAttributes 	K2Platform::getDigitalInTypeConnected() {
	return readGpioState ( kGPIO_Selector_DigitalInType );
}

//	--------------------------------------------------------------------------------
GpioAttributes 	K2Platform::getCodecInterrupt() {
	return readGpioState ( kGPIO_Selector_CodecInterrupt );
}

//	--------------------------------------------------------------------------------
GpioAttributes 	K2Platform::getCodecErrorInterrupt() {
	return readGpioState ( kGPIO_Selector_CodecErrorInterrupt );
}


//	--------------------------------------------------------------------------------
IOReturn 	K2Platform::setHeadphoneMuteState ( GpioAttributes muteState ) {
	return writeGpioState ( kGPIO_Selector_HeadphoneMute, muteState );
}
	
//	--------------------------------------------------------------------------------
GpioAttributes 	K2Platform::getHeadphoneMuteState() {
	return mAppleGPIO_HeadphoneMute;
}
	
//	--------------------------------------------------------------------------------
IOReturn 	K2Platform::setLineOutMuteState ( GpioAttributes muteState ) {
	return writeGpioState ( kGPIO_Selector_LineOutMute, muteState );
}
	
//	--------------------------------------------------------------------------------
GpioAttributes 	K2Platform::getLineOutMuteState() {
	return mAppleGPIO_LineOutMute;
}
	
//	--------------------------------------------------------------------------------
IOReturn 	K2Platform::setSpeakerMuteState ( GpioAttributes muteState ) {
	return writeGpioState ( kGPIO_Selector_SpeakerMute, muteState );
}
	
//	--------------------------------------------------------------------------------
GpioAttributes 	K2Platform::getSpeakerMuteState() {
	return mAppleGPIO_AmpMute;
}

//	--------------------------------------------------------------------------------
IOReturn 	K2Platform::setClockMux ( GpioAttributes muxState ) {
	return writeGpioState ( kGPIO_Selector_ClockMux, muxState );
}
	
//	--------------------------------------------------------------------------------
GpioAttributes 	K2Platform::getClockMux() {
	return mAppleGPIO_CodecClockMux;
}

//	--------------------------------------------------------------------------------
IOReturn 	K2Platform::setInputDataMux(GpioAttributes muxState) {
	return writeGpioState ( kGPIO_Selector_InputDataMux, muxState );
}

//	--------------------------------------------------------------------------------
GpioAttributes 	K2Platform::getInputDataMux() {
	return mAppleGPIO_CodecInputDataMux;
}

#pragma mark ---------------------------
#pragma mark ¥ Private I2C
#pragma mark ---------------------------

//	--------------------------------------------------------------------------------
bool K2Platform::findAndAttachI2C()
{
	const OSSymbol	*i2cDriverName;
	IOService		*i2cCandidate = NULL;
	OSDictionary	*i2cServiceDictionary;
	mach_timespec_t	timeout;
	bool			result = false;
	OSIterator		*iterator;
	OSObject		*theObject;
	bool			found = false;
	
//	debug2IOLog ( "+K2Platform::findAndAttachI2C result = %d ",result );	
	
	// Searches the i2c:
	i2cDriverName = OSSymbol::withCStringNoCopy ( "PPCI2CInterface.i2c-mac-io" );
	i2cServiceDictionary = IOService::resourceMatching ( i2cDriverName );
	FailIf ( NULL == i2cServiceDictionary, Exit );
	
	debugIOLog ( "about to waitForService on i2cServiceDictionary timeout = 5 seconds\n" );
	timeout.tv_sec = 5;
	timeout.tv_nsec = 0;
	i2cCandidate = IOService::waitForService ( i2cServiceDictionary, &timeout );
	if ( NULL == i2cCandidate ) {
		iterator = IOService::getMatchingServices ( i2cServiceDictionary );
		if ( NULL != iterator ) {
			do {
				theObject = iterator->getNextObject ();
				if ( theObject ) {
					debug2IOLog("found theObject=%p\n",theObject);
					i2cCandidate = OSDynamicCast(IOService,theObject);
				}
			} while ( !found && NULL != theObject );
		} else {
			debugIOLog(" NULL != iterator\n");
		}
	} else {
		debug2IOLog ( "K2Platform::findAndAttachI2C i2cCandidate = %p ",i2cCandidate );
	}
	
//	debug2IOLog ( "i2cServiceDictionary %p\n", i2cServiceDictionary );	
	FailIf(NULL == i2cCandidate,Exit);
	
	mI2CInterface = (PPCI2CInterface*)i2cCandidate->getProperty ( i2cDriverName );
	FailIf ( NULL == mI2CInterface, Exit );

	mI2CInterface->retain();
	result = true;
Exit:
//	debug2IOLog ( "-K2Platform::findAndAttachI2C result = %d ",result );

	return result;
}

//	--------------------------------------------------------------------------------
bool K2Platform::openI2C()
{
	bool		result = false;
	
	FailIf ( NULL == mI2CInterface, Exit );
	FailIf ( !mI2CInterface->openI2CBus ( mI2CPort ), Exit );
	result = true;
Exit:
	return result;
}

//	--------------------------------------------------------------------------------
void K2Platform::closeI2C ()
{
	mI2CInterface->closeI2CBus ();
}

//	--------------------------------------------------------------------------------
bool K2Platform::detachFromI2C()
{
	if (mI2CInterface) {
		mI2CInterface->release();
		mI2CInterface = NULL;
	}
	return (true);
}

#pragma mark ---------------------------
#pragma mark ¥ Private I2S
#pragma mark ---------------------------

#ifndef kBUILD_FOR_DIRECT_I2S_HW_ACCESS	
//	--------------------------------------------------------------------------------
bool K2Platform::findAndAttachI2S()
{

	const OSSymbol	*i2sDriverName;
	IOService		*i2sCandidate;
	OSDictionary	*i2sServiceDictionary;
	bool			result = false;
	mach_timespec_t	timeout	= {5,0};

	i2sDriverName = OSSymbol::withCStringNoCopy ( "AppleI2S" );
	i2sServiceDictionary = IOService::resourceMatching ( i2sDriverName );
	FailIf ( NULL == i2sServiceDictionary, Exit );
	
//	debug2IOLog ( "about to waitForService on i2sServiceDictionary with timeout = %d\n",	timeout.tv_sec );
	i2sCandidate = IOService::waitForService ( i2sServiceDictionary, &timeout );
	debug2IOLog ( "i2sServiceDictionary %p\n", i2sServiceDictionary );
	FailIf(NULL == i2sCandidate,Exit);
	
	mI2SInterface = (AppleI2S*)i2sCandidate->getProperty ( i2sDriverName );
	FailIf ( NULL == mI2SInterface, Exit );
	
	mI2SInterface->retain();
	result = true;
	
Exit:
//	debug2IOLog ( "K2Platform::findAndAttachI2S result = %d\n",	result );

	return result;
}

//	--------------------------------------------------------------------------------
bool K2Platform::openI2S()
{
	return true;			//	No open in K2 I2S driver
}

//	--------------------------------------------------------------------------------
void K2Platform::closeI2S ()
{
	return;					//	No close in K2 I2S driver
}

//	--------------------------------------------------------------------------------
bool K2Platform::detachFromI2S()
{

	if ( NULL != mI2SInterface ) {
		mI2SInterface->release();
		mI2SInterface = NULL;
	}

	return ( true );
}
#endif

#pragma mark ---------------------------
#pragma mark ¥ GPIO Utilities
#pragma mark ---------------------------

//	--------------------------------------------------------------------------------
const OSSymbol* K2Platform::makeFunctionSymbolName(const char * name,UInt32 pHandle) {
	const OSSymbol* 	funcSymbolName = NULL;
	char		stringBuf[256];
		
	sprintf ( stringBuf, "%s-%08lx", name, pHandle );
	funcSymbolName = OSSymbol::withCString ( stringBuf );
	
	return funcSymbolName;
}

//	--------------------------------------------------------------------------------
GpioAttributes K2Platform::readGpioState ( GPIOSelector selector ) {
	GpioAttributes		result;
	
	result = kGPIO_Unknown;
#ifdef kBUILD_FOR_DIRECT_GPIO_HW_ACCESS
	switch ( selector ) {
		case kGPIO_Selector_AnalogCodecReset:			result = 0 == ( *mGPIO_analogCodecReset & 0x02 ) ? kGPIO_Reset : kGPIO_Run ;											break;
		case kGPIO_Selector_ClockMux:					result = 0 == ( *mGPIO_mclkMuxSelect & 0x02 ) ? kGPIO_MuxSelectDefault : kGPIO_MuxSelectAlternate ;						break;
		case kGPIO_Selector_CodecInterrupt:				result = 0 == ( *mGPIO_digitalCodecIrq & 0x02 ) ? kGPIO_CodecInterruptInactive : kGPIO_CodecInterruptActive ;			break;
		case kGPIO_Selector_CodecErrorInterrupt:		result = 0 == ( *mGPIO_digitalCodecErrorIrq & 0x02 ) ? kGPIO_CodecInterruptInactive : kGPIO_CodecInterruptActive ;		break;
		case kGPIO_Selector_DigitalCodecReset:			result = 0 == ( *mGPIO_digitalCodecReset & 0x02 ) ? kGPIO_Reset : kGPIO_Run ;											break;
#ifdef kHAS_HEADPHONE_BOARD
		case kGPIO_Selector_HeadphoneDetect:			result = 0 == ( *mGPIO_headphoneSense & 0x02 ) ? kGPIO_Connected : kGPIO_Disconnected ;									break;
#else
		case kGPIO_Selector_HeadphoneDetect:			result = 0 == ( *mGPIO_headphoneSense & 0x02 ) ? kGPIO_Disconnected : kGPIO_Connected ;									break;
#endif
		case kGPIO_Selector_HeadphoneMute:				result = 0 == ( *mGPIO_headphoneMute & 0x02 ) ? kGPIO_Muted : kGPIO_Unmuted ;											break;
		case kGPIO_Selector_InputDataMux:				result = 0 == ( *mGPIO_inputDataMuxSelect & 0x02 ) ? kGPIO_MuxSelectDefault : kGPIO_MuxSelectAlternate ;				break;
		case kGPIO_Selector_LineInDetect:				result = 0 == ( *mGPIO_lineInSense & 0x02 ) ? kGPIO_Connected : kGPIO_Disconnected ;									break;
		case kGPIO_Selector_LineOutDetect:				result = 0 == ( *mGPIO_lineOutSense & 0x02 ) ? kGPIO_Connected : kGPIO_Disconnected ;									break;
		case kGPIO_Selector_LineOutMute:				result = 0 == ( *mGPIO_lineOutMute & 0x02 ) ? kGPIO_Muted : kGPIO_Unmuted ;												break;
		case kGPIO_Selector_SpeakerMute:				result = 0 == ( *mGPIO_speakerMute & 0x02 ) ? kGPIO_Unmuted : kGPIO_Muted ;												break;
		default:										FailIf ( true, Exit );																									break;
	}
#else
	UInt32				value;
	const OSSymbol*		funcSymbolName;
	IOReturn			err;
	bool				waitForFunction;

	if ( mK2Service ) {
		switch ( selector ) {
			case kGPIO_Selector_AnalogCodecReset:			funcSymbolName = makeFunctionSymbolName( kAppleGPIO_SetAudioHwReset, mI2SPHandle);				break;
			case kGPIO_Selector_ClockMux:					funcSymbolName = makeFunctionSymbolName( kAppleGPIO_SetCodecClockMux, mI2SPHandle);				break;
			case kGPIO_Selector_CodecInterrupt:				funcSymbolName = makeFunctionSymbolName( kAppleGPIO_GetCodecIRQ, mI2SPHandle);					break;
			case kGPIO_Selector_CodecErrorInterrupt:		funcSymbolName = makeFunctionSymbolName( kAppleGPIO_GetCodecErrorIRQ, mI2SPHandle);				break;
			case kGPIO_Selector_DigitalCodecReset:			funcSymbolName = makeFunctionSymbolName( kAppleGPIO_SetAudioDigHwReset, mI2SPHandle);			break;
			case kGPIO_Selector_DigitalInDetect:			funcSymbolName = makeFunctionSymbolName( kAppleGPIO_GetDigitalInDetect, mI2SPHandle);			break;
			case kGPIO_Selector_DigitalInType:				funcSymbolName = makeFunctionSymbolName( kAppleGPIO_GetDigitalInType, mI2SPHandle);				break;
			case kGPIO_Selector_DigitalOutDetect:			funcSymbolName = makeFunctionSymbolName( kAppleGPIO_GetDigitalOutDetect, mI2SPHandle);			break;
			case kGPIO_Selector_DigitalOutType:				funcSymbolName = makeFunctionSymbolName( kAppleGPIO_GetDigitalOutType, mI2SPHandle);			break;
			case kGPIO_Selector_HeadphoneDetect:			funcSymbolName = makeFunctionSymbolName( kAppleGPIO_GetHeadphoneDetect, mI2SPHandle);			break;
			case kGPIO_Selector_HeadphoneMute:				funcSymbolName = makeFunctionSymbolName( kAppleGPIO_SetHeadphoneMute, mI2SPHandle);				break;
			case kGPIO_Selector_InputDataMux:				funcSymbolName = makeFunctionSymbolName( kAppleGPIO_SetCodecInputDataMux, mI2SPHandle);			break;
			case kGPIO_Selector_LineInDetect:				funcSymbolName = makeFunctionSymbolName( kAppleGPIO_GetLineInDetect, mI2SPHandle);				break;
			case kGPIO_Selector_LineOutDetect:				funcSymbolName = makeFunctionSymbolName( kAppleGPIO_GetLineOutDetect, mI2SPHandle);				break;
			case kGPIO_Selector_LineOutMute:				funcSymbolName = makeFunctionSymbolName( kAppleGPIO_SetLineOutMute, mI2SPHandle);				break;
			case kGPIO_Selector_SpeakerDetect:				funcSymbolName = makeFunctionSymbolName( kAppleGPIO_GetSpeakerDetect, mI2SPHandle);				break;
			case kGPIO_Selector_SpeakerMute:				funcSymbolName = makeFunctionSymbolName( kAppleGPIO_SetAmpMute, mI2SPHandle);					break;
			default:										FailIf ( true, Exit );																			break;
		}
		if ( NULL != funcSymbolName ) {
			waitForFunction = kGPIO_Selector_HeadphoneDetect == selector ? true : false ;
			err = mK2Service->callPlatformFunction ( funcSymbolName , waitForFunction, (void*)&value, (void*)0, (void*)0, (void*)0 );
			if ( kIOReturnSuccess != err ) { debug2IOLog ( "K2Platform:readGpioState ( %d ) *** NOT FOUND ***\n", (unsigned int)selector ); }
			FailIf ( kIOReturnSuccess != err, Exit );
			//	Translate the GPIO state to a GPIO attribute
			switch ( selector ) {
				case kGPIO_Selector_AnalogCodecReset:		result = value ? kGPIO_Run : kGPIO_Reset;														break;
				case kGPIO_Selector_ClockMux:				result = value ? kGPIO_MuxSelectAlternate : kGPIO_MuxSelectDefault;								break;
				case kGPIO_Selector_CodecInterrupt:			result = value ? kGPIO_CodecInterruptActive : kGPIO_CodecInterruptInactive;						break;
				case kGPIO_Selector_CodecErrorInterrupt:	result = value ? kGPIO_CodecInterruptActive : kGPIO_CodecInterruptInactive;						break;
				case kGPIO_Selector_DigitalCodecReset:		result = value ? kGPIO_Run : kGPIO_Reset;														break;
				case kGPIO_Selector_DigitalInDetect:		result = value ? kGPIO_Connected : kGPIO_Disconnected;											break;
				case kGPIO_Selector_DigitalInType:			result = value ? kGPIO_TypeIsDigital : kGPIO_TypeIsAnalog;										break;
				case kGPIO_Selector_DigitalOutDetect:		result = value ? kGPIO_Connected : kGPIO_Disconnected;											break;
				case kGPIO_Selector_DigitalOutType:			result = value ? kGPIO_TypeIsDigital : kGPIO_TypeIsAnalog;										break;
				case kGPIO_Selector_HeadphoneDetect:		result = value ? kGPIO_Connected : kGPIO_Disconnected;											break;
				case kGPIO_Selector_HeadphoneMute:			result = value ? kGPIO_Muted : kGPIO_Unmuted ;													break;
				case kGPIO_Selector_InputDataMux:			result = value ? kGPIO_MuxSelectAlternate : kGPIO_MuxSelectDefault;								break;
				case kGPIO_Selector_LineInDetect:			result = value ? kGPIO_Connected : kGPIO_Disconnected;											break;
				case kGPIO_Selector_LineOutDetect:			result = value ? kGPIO_Connected : kGPIO_Disconnected;											break;
				case kGPIO_Selector_LineOutMute:			result = value ? kGPIO_Muted : kGPIO_Unmuted;													break;
				case kGPIO_Selector_SpeakerDetect:			result = value ? kGPIO_Muted : kGPIO_Unmuted;													break;
				case kGPIO_Selector_SpeakerMute:			result = value ? kGPIO_Connected : kGPIO_Disconnected;											break;
			}
		} else {
			//	If there is no platform function then return the cached GPIO state if a cache state is available
			switch ( selector ) {
				case kGPIO_Selector_AnalogCodecReset:		result = mAppleGPIO_AnalogCodecReset;															break;
				case kGPIO_Selector_ClockMux:				result = mAppleGPIO_CodecClockMux;																break;
				case kGPIO_Selector_CodecInterrupt:																											break;
				case kGPIO_Selector_CodecErrorInterrupt:																									break;
				case kGPIO_Selector_DigitalCodecReset:		result = mAppleGPIO_DigitalCodecReset;															break;
				case kGPIO_Selector_DigitalInDetect:																										break;
				case kGPIO_Selector_DigitalInType:																											break;
				case kGPIO_Selector_DigitalOutDetect:																										break;
				case kGPIO_Selector_DigitalOutType:																											break;
				case kGPIO_Selector_HeadphoneDetect:																										break;
				case kGPIO_Selector_HeadphoneMute:			result = mAppleGPIO_HeadphoneMute;																break;
				case kGPIO_Selector_InputDataMux:			result = mAppleGPIO_CodecInputDataMux;															break;
				case kGPIO_Selector_LineInDetect:																											break;
				case kGPIO_Selector_LineOutDetect:																											break;
				case kGPIO_Selector_LineOutMute:			result = mAppleGPIO_LineOutMute;																break;
				case kGPIO_Selector_SpeakerDetect:																											break;
				case kGPIO_Selector_SpeakerMute:			result = mAppleGPIO_AmpMute;																	break;
			}
		}
	}
#endif
Exit:
	if ( kIOReturnSuccess != result ) {
		debug2IOLog ( "K2Platform::readGpioState ( %d ) FAILED\n", selector );
	}
	return result;
}

//	--------------------------------------------------------------------------------
IOReturn K2Platform::writeGpioState ( GPIOSelector selector, GpioAttributes gpioState ) {
	UInt32				value;
	IOReturn			result;
#ifndef kBUILD_FOR_DIRECT_GPIO_HW_ACCESS
	const OSSymbol*		funcSymbolName;
#endif

	result = kIOReturnError;
	value = 0;
#ifdef kBUILD_FOR_DIRECT_GPIO_HW_ACCESS
	switch ( selector ) {
		case kGPIO_Selector_AnalogCodecReset:		FailIf ( kIOReturnSuccess != translateGpioAttributeToGpioState ( kGPIO_Type_Reset, gpioState, &value ), Exit );		break;
		case kGPIO_Selector_ClockMux:				FailIf ( kIOReturnSuccess != translateGpioAttributeToGpioState ( kGPIO_Type_Mux, gpioState, &value ), Exit );		break;
		case kGPIO_Selector_DigitalCodecReset:		FailIf ( kIOReturnSuccess != translateGpioAttributeToGpioState ( kGPIO_Type_Reset, gpioState, &value ), Exit );		break;
		case kGPIO_Selector_HeadphoneMute:			FailIf ( kIOReturnSuccess != translateGpioAttributeToGpioState ( kGPIO_Type_MuteH, gpioState, &value ), Exit );		break;
		case kGPIO_Selector_InputDataMux:			FailIf ( kIOReturnSuccess != translateGpioAttributeToGpioState ( kGPIO_Type_Mux, gpioState, &value ), Exit );		break;
		case kGPIO_Selector_LineOutMute:			FailIf ( kIOReturnSuccess != translateGpioAttributeToGpioState ( kGPIO_Type_MuteH, gpioState, &value ), Exit );		break;
		case kGPIO_Selector_SpeakerMute:			FailIf ( kIOReturnSuccess != translateGpioAttributeToGpioState ( kGPIO_Type_MuteL, gpioState, &value ), Exit );		break;
		default:									FailIf ( true, Exit );																								break;
	}
	switch ( selector ) {
		case kGPIO_Selector_AnalogCodecReset:		*mGPIO_analogCodecReset = value ? kGPIO_DATA_WR_PIN_LEVEL_H : kGPIO_DATA_WR_PIN_LEVEL_L ;							break;
		case kGPIO_Selector_ClockMux:				*mGPIO_mclkMuxSelect = value ? kGPIO_DATA_WR_PIN_LEVEL_H : kGPIO_DATA_WR_PIN_LEVEL_L ;								break;
		case kGPIO_Selector_DigitalCodecReset:		*mGPIO_digitalCodecReset = value ? kGPIO_DATA_WR_PIN_LEVEL_H : kGPIO_DATA_WR_PIN_LEVEL_L ;							break;
		case kGPIO_Selector_HeadphoneMute:			*mGPIO_headphoneMute = value ? kGPIO_DATA_WR_PIN_LEVEL_H : kGPIO_DATA_WR_PIN_LEVEL_L ;								break;
		case kGPIO_Selector_InputDataMux:			*mGPIO_inputDataMuxSelect = value ? kGPIO_DATA_WR_PIN_LEVEL_H : kGPIO_DATA_WR_PIN_LEVEL_L ;							break;
		case kGPIO_Selector_LineOutMute:			*mGPIO_lineOutMute = value ? kGPIO_DATA_WR_PIN_LEVEL_H : kGPIO_DATA_WR_PIN_LEVEL_L ;								break;
		case kGPIO_Selector_SpeakerMute:			*mGPIO_speakerMute = value ? kGPIO_DATA_WR_PIN_LEVEL_H : kGPIO_DATA_WR_PIN_LEVEL_L ;								break;
		default:									FailIf ( true, Exit );																								break;
	}
	result = kIOReturnSuccess;
#else
	if ( mK2Service ) {
		//	Translate GPIO attribute to gpio state
		switch ( selector ) {
			case kGPIO_Selector_AnalogCodecReset:		
				FailIf ( kIOReturnSuccess != translateGpioAttributeToGpioState ( kGPIO_Type_Reset, gpioState, &value ), Exit );	
				value = !value; // !!! test !!! inverted 
				break;
			case kGPIO_Selector_ClockMux:				FailIf ( kIOReturnSuccess != translateGpioAttributeToGpioState ( kGPIO_Type_Mux, gpioState, &value ), Exit );	break;
			case kGPIO_Selector_DigitalCodecReset:		FailIf ( kIOReturnSuccess != translateGpioAttributeToGpioState ( kGPIO_Type_Reset, gpioState, &value ), Exit );	break;
			case kGPIO_Selector_HeadphoneMute:			FailIf ( kIOReturnSuccess != translateGpioAttributeToGpioState ( kGPIO_Type_MuteH, gpioState, &value ), Exit );	break;
			case kGPIO_Selector_InputDataMux:			FailIf ( kIOReturnSuccess != translateGpioAttributeToGpioState ( kGPIO_Type_Mux, gpioState, &value ), Exit );	break;
			case kGPIO_Selector_LineOutMute:			FailIf ( kIOReturnSuccess != translateGpioAttributeToGpioState ( kGPIO_Type_MuteH, gpioState, &value ), Exit );	break;
			case kGPIO_Selector_SpeakerMute:			FailIf ( kIOReturnSuccess != translateGpioAttributeToGpioState ( kGPIO_Type_MuteL, gpioState, &value ), Exit );	break;
			default:									FailIf ( true, Exit );																							break;
		}
		switch ( selector ) {
			case kGPIO_Selector_AnalogCodecReset:		funcSymbolName = makeFunctionSymbolName( kAppleGPIO_SetAudioHwReset, mI2SPHandle );					break;
			case kGPIO_Selector_ClockMux:				funcSymbolName = makeFunctionSymbolName( kAppleGPIO_SetCodecClockMux, mI2SPHandle );				break;
			case kGPIO_Selector_DigitalCodecReset:		funcSymbolName = makeFunctionSymbolName( kAppleGPIO_SetAudioDigHwReset, mI2SPHandle );				break;
			case kGPIO_Selector_HeadphoneMute:			funcSymbolName = makeFunctionSymbolName( kAppleGPIO_SetHeadphoneMute, mI2SPHandle );				break;
			case kGPIO_Selector_InputDataMux:			funcSymbolName = makeFunctionSymbolName( kAppleGPIO_SetCodecInputDataMux, mI2SPHandle );			break;
			case kGPIO_Selector_LineOutMute:			funcSymbolName = makeFunctionSymbolName( kAppleGPIO_SetLineOutMute, mI2SPHandle );					break;
			case kGPIO_Selector_SpeakerMute:			funcSymbolName = makeFunctionSymbolName( kAppleGPIO_SetAmpMute, mI2SPHandle );						break;
			default:																																		break;
		}
		FailIf ( NULL == funcSymbolName, Exit );
		result = mK2Service->callPlatformFunction ( funcSymbolName, false, (void*)value, (void*)0, (void*)0, (void*)0 );
		FailIf ( kIOReturnSuccess != result, Exit );
		if ( kIOReturnSuccess == result ) {
			//	Cache the gpio state in case the gpio does not have a platform function supporting read access
			switch ( selector ) {
				case kGPIO_Selector_AnalogCodecReset:	mAppleGPIO_AnalogCodecReset = gpioState;															break;
				case kGPIO_Selector_ClockMux:			mAppleGPIO_CodecClockMux = gpioState;																break;
				case kGPIO_Selector_DigitalCodecReset:	mAppleGPIO_DigitalCodecReset = gpioState;															break;
				case kGPIO_Selector_HeadphoneMute:		mAppleGPIO_HeadphoneMute = gpioState;																break;
				case kGPIO_Selector_InputDataMux:		mAppleGPIO_CodecInputDataMux = gpioState;															break;
				case kGPIO_Selector_LineOutMute:		mAppleGPIO_LineOutMute = gpioState;																	break;
				case kGPIO_Selector_SpeakerMute:		mAppleGPIO_AmpMute = gpioState;																		break;
				default:																																	break;
			}
		}
	}
#endif
Exit:
	return result;
}

//	--------------------------------------------------------------------------------
//	Convert a GPIOAttribute to a binary value to be applied to a GPIO pin level
IOReturn K2Platform::translateGpioAttributeToGpioState ( GPIOType gpioType, GpioAttributes gpioAttribute, UInt32 * valuePtr ) {
	IOReturn		result;
	
//	debug4IOLog ( "K2Platform::translateGpioAttributeToGpioState ( %d, %d, %p )\n", (unsigned int)gpioType, (unsigned int)gpioAttribute, valuePtr );
#ifndef kBUILD_FOR_DIRECT_GPIO_HW_ACCESS	
	result = kIOReturnBadArgument;
	if ( NULL != valuePtr ) {
		result = kIOReturnSuccess;
		switch ( gpioType ) {
			case kGPIO_Type_MuteL:
				if ( kGPIO_Muted == gpioAttribute ) {
					*valuePtr = 1;
				} else if ( kGPIO_Unmuted == gpioAttribute ) {
					*valuePtr = 0;
				} else {
					result = kIOReturnBadArgument;
				}
				break;
			case kGPIO_Type_MuteH:
				if ( kGPIO_Muted == gpioAttribute ) {
					*valuePtr = 1;
				} else if ( kGPIO_Unmuted == gpioAttribute ) {
					*valuePtr = 0;
				} else {
					result = kIOReturnBadArgument;
				}
				break;
			case kGPIO_Type_Mux:
				if ( kGPIO_MuxSelectAlternate == gpioAttribute ) {
					*valuePtr = 1;
				} else if ( kGPIO_MuxSelectDefault == gpioAttribute ) {
					*valuePtr = 0;
				} else {
					result = kIOReturnBadArgument;
				}
				break;
			case kGPIO_Type_Reset:
				if ( kGPIO_Run == gpioAttribute ) {
					*valuePtr = 0;
				} else if ( kGPIO_Reset == gpioAttribute ) {
					*valuePtr = 1;
				} else {
					result = kIOReturnBadArgument;
				}
				break;
			default:
				result = kIOReturnBadArgument;
				break;
		}
	}
#else	// use this for direct hardware access (takes into account active high/low of gpio's)
	if ( NULL != valuePtr ) {
		result = kIOReturnSuccess;
		switch ( gpioType ) {
			case kGPIO_Type_ConnectorType:
				if ( kGPIO_TypeIsDigital == gpioAttribute ) {
					*valuePtr = 1;
				} else if ( kGPIO_TypeIsAnalog == gpioAttribute ) {
					*valuePtr = 0;
				} else {
					result = kIOReturnBadArgument;
				}
				break;
			case kGPIO_Type_Detect:
				if ( kGPIO_Disconnected == gpioAttribute ) {
					*valuePtr = 1;
				} else if ( kGPIO_Connected == gpioAttribute ) {
					*valuePtr = 0;
				} else {
					result = kIOReturnBadArgument;
				}
				break;
			case kGPIO_Type_Irq:
				if ( kGPIO_CodecInterruptActive == gpioAttribute ) {
					*valuePtr = 1;
				} else if ( kGPIO_CodecInterruptInactive == gpioAttribute ) {
					*valuePtr = 0;
				} else {
					result = kIOReturnBadArgument;
				}
				break;
			case kGPIO_Type_MuteL:
				if ( kGPIO_Muted == gpioAttribute ) {
					*valuePtr = 0;
				} else if ( kGPIO_Unmuted == gpioAttribute ) {
					*valuePtr = 1;
				} else {
					result = kIOReturnBadArgument;
				}
				break;
			case kGPIO_Type_MuteH:
				if ( kGPIO_Muted == gpioAttribute ) {
					*valuePtr = 1;
				} else if ( kGPIO_Unmuted == gpioAttribute ) {
					*valuePtr = 0;
				} else {
					result = kIOReturnBadArgument;
				}
				break;
			case kGPIO_Type_Mux:
				if ( kGPIO_MuxSelectAlternate == gpioAttribute ) {
					*valuePtr = 1;
				} else if ( kGPIO_MuxSelectDefault == gpioAttribute ) {
					*valuePtr = 0;
				} else {
					result = kIOReturnBadArgument;
				}
				break;
			case kGPIO_Type_Reset:
				if ( kGPIO_Run == gpioAttribute ) {
					*valuePtr = 1;
				} else if ( kGPIO_Reset == gpioAttribute ) {
					*valuePtr = 0;
				} else {
					result = kIOReturnBadArgument;
				}
				break;
			default:
				result = kIOReturnBadArgument;
				break;
		}
	}
#endif	
	if ( kIOReturnSuccess != result ) { debug3IOLog ( "K2Platform::translateGpioAttributeToGpioState ( %X, %p )\n", (unsigned int)gpioAttribute, valuePtr ); }
	return result;
}

#pragma mark ---------------------------
#pragma mark ¥ DBDMA Memory Address Acquisition Methods
#pragma mark ---------------------------

//	--------------------------------------------------------------------------------
IODBDMAChannelRegisters *	K2Platform::GetInputChannelRegistersVirtualAddress ( IOService * dbdmaProvider ) {
	IOMemoryMap *				map;
	IOService *					parentOfParent;
	IOPhysicalAddress			ioPhysAddr;
	IOMemoryDescriptor *		theDescriptor;
	IOByteCount					length = 256;
	
	mIOBaseDMAInput = NULL;
	FailIf ( NULL == dbdmaProvider, Exit );
	debug2IOLog ( "K2Platform::GetInputChannelRegistersVirtualAddress i2s-a name is %s\n", dbdmaProvider->getName() );
	parentOfParent = (IOService*)dbdmaProvider->getParentEntry ( gIODTPlane );
	FailIf ( NULL == parentOfParent, Exit );
	debug2IOLog ( "   parentOfParent name is %s\n", parentOfParent->getName() );
	map = parentOfParent->mapDeviceMemoryWithIndex ( AppleDBDMAAudio::kDBDMAOutputIndex );
	FailIf ( NULL == map, Exit );
	ioPhysAddr = map->getPhysicalSegment( kIODMAInputOffset, &length );
	FailIf ( NULL == ioPhysAddr, Exit );
	theDescriptor = IOMemoryDescriptor::withPhysicalAddress ( ioPhysAddr, 256, kIODirectionOutIn );
	FailIf ( NULL == theDescriptor, Exit );
	map = theDescriptor->map ();
	FailIf ( NULL == map, Exit );
	mIOBaseDMAInput = (IODBDMAChannelRegisters*)map->getVirtualAddress();
	
	debug2IOLog ( "mIOBaseDMAInput %p\n", mIOBaseDMAInput );
	if ( NULL == mIOBaseDMAInput ) { IOLog ( "K2Platform::GetInputChannelRegistersVirtualAddress IODBDMAChannelRegisters NOT IN VIRTUAL SPACE\n" ); }
Exit:
	return mIOBaseDMAInput;
}

//	--------------------------------------------------------------------------------
IODBDMAChannelRegisters *	K2Platform::GetOutputChannelRegistersVirtualAddress ( IOService * dbdmaProvider ) {
	IOMemoryMap *				map;
	IOService *					parentOfParent;
	
	mIOBaseDMAOutput = NULL;
	FailIf ( NULL == dbdmaProvider, Exit );
	debug2IOLog ( "K2Platform::GetOutputChannelRegistersVirtualAddress i2s-a name is %s\n", dbdmaProvider->getName() );
	parentOfParent = (IOService*)dbdmaProvider->getParentEntry ( gIODTPlane );
	FailIf ( NULL == parentOfParent, Exit );
	debug2IOLog ( "   parentOfParent name is %s\n", parentOfParent->getName() );
	map = parentOfParent->mapDeviceMemoryWithIndex ( AppleDBDMAAudio::kDBDMAOutputIndex );
	FailIf ( NULL == map, Exit );
	mIOBaseDMAOutput = (IODBDMAChannelRegisters*)map->getVirtualAddress();
	
	debug3IOLog ( "mIOBaseDMAOutput %p is at physical %p\n", mIOBaseDMAOutput, (void*)map->getPhysicalAddress() );
	if ( NULL == mIOBaseDMAOutput ) { IOLog ( "K2Platform::GetOutputChannelRegistersVirtualAddress IODBDMAChannelRegisters NOT IN VIRTUAL SPACE\n" ); }
Exit:
	return mIOBaseDMAOutput;
}

#pragma mark ---------------------------
#pragma mark ¥ DEBUGGING UTILITIES
#pragma mark ---------------------------

//	--------------------------------------------------------------------------------
void K2Platform::LogFCR ( void ) {
#if 0
	debug3IOLog ( "FCR1: %X -> %X\n", kAUDIO_MAC_IO_BASE_ADDRESS + kAUDIO_MAC_IO_FCR1, (unsigned int)OSReadSwapInt32 ( mHwPtr, kAUDIO_MAC_IO_FCR1 ) );
	debug3IOLog ( "FCR3: %X -> %X\n", kAUDIO_MAC_IO_BASE_ADDRESS + kAUDIO_MAC_IO_FCR3, (unsigned int)OSReadSwapInt32 ( mHwPtr, kAUDIO_MAC_IO_FCR3 ) );
#endif
	return;
}

//	--------------------------------------------------------------------------------
void K2Platform::LogI2S ( void ) {
#if 0
	debug3IOLog ( "I2S Serial Format:     %X -> %X\n", kAUDIO_I2S_BASE_ADDRESS + kAUDIO_I2S_SERIAL_FORMAT, (unsigned int)OSReadSwapInt32 ( mHwI2SPtr, kAUDIO_I2S_SERIAL_FORMAT ) );
	debug3IOLog ( "I2S Data Word Size:    %X -> %X\n", kAUDIO_I2S_BASE_ADDRESS + kAUDIO_I2S_DATA_WORD_SIZES, (unsigned int)OSReadSwapInt32 ( mHwI2SPtr, kAUDIO_I2S_DATA_WORD_SIZES ) );
	debug3IOLog ( "I2S Frame Count:       %X -> %X\n", kAUDIO_I2S_BASE_ADDRESS + kAUDIO_I2S_FRAME_COUNTER, (unsigned int)OSReadSwapInt32 ( mHwI2SPtr, kAUDIO_I2S_FRAME_COUNTER ) );
	debug3IOLog ( "I2S Interrupt Control: %X -> %X\n\n", kAUDIO_I2S_BASE_ADDRESS + kAUDIO_I2S_INTERRUPT_CONTROL, (unsigned int)OSReadSwapInt32 ( mHwI2SPtr, kAUDIO_I2S_INTERRUPT_CONTROL ) );
#endif
	return;
}

//	--------------------------------------------------------------------------------
void K2Platform::LogGPIO ( void ) {
#if 1
	debug3IOLog ( "GPIO3  AUD_DTI_SEL:           %X -> %X\n", kAUDIO_MAC_IO_BASE_ADDRESS + kAUDIO_GPIO_INPUT_DATA_MUX_SELECT, (unsigned int)mHwPtr[kAUDIO_GPIO_INPUT_DATA_MUX_SELECT] );
	debug3IOLog ( "GPIO4  AUDIO_LINEIN_SENSE_L:  %X -> %X\n", kAUDIO_MAC_IO_BASE_ADDRESS + kAUDIO_GPIO_LINE_IN_SENSE, (unsigned int)mHwPtr[kAUDIO_GPIO_LINE_IN_SENSE] );
	debug3IOLog ( "GPIO5  AUD_RERR:              %X -> %X\n", kAUDIO_MAC_IO_BASE_ADDRESS + kAUDIO_GPIO_CODEC_ERROR_IRQ, (unsigned int)mHwPtr[kAUDIO_GPIO_CODEC_ERROR_IRQ] );
	debug3IOLog ( "GPIO12 AUDIO_SPDIF_RESET_L:   %X -> %X\n", kAUDIO_MAC_IO_BASE_ADDRESS + kAUDIO_GPIO_DIGITAL_CODEC_RESET, (unsigned int)mHwPtr[kAUDIO_GPIO_DIGITAL_CODEC_RESET] );
	debug3IOLog ( "GPIO14 AUDIO_LINEOUT_SENSE_L: %X -> %X\n", kAUDIO_MAC_IO_BASE_ADDRESS + kAUDIO_GPIO_LINE_OUT_SENSE, (unsigned int)mHwPtr[kAUDIO_GPIO_LINE_OUT_SENSE] );
	debug3IOLog ( "GPIO15 AUDIO_HP_SENSE_L:      %X -> %X\n", kAUDIO_MAC_IO_BASE_ADDRESS + kAUDIO_GPIO_HEADPHONE_SENSE, (unsigned int)mHwPtr[kAUDIO_GPIO_HEADPHONE_SENSE] );
	debug3IOLog ( "GPIO16 AUDIO_DIO_IRQ:         %X -> %X\n", kAUDIO_MAC_IO_BASE_ADDRESS + kAUDIO_GPIO_CODEC_IRQ, (unsigned int)mHwPtr[kAUDIO_GPIO_CODEC_IRQ] );
	debug3IOLog ( "GPIO23 AUDIO_HP_MUTE_L:       %X -> %X\n", kAUDIO_MAC_IO_BASE_ADDRESS + kAUDIO_GPIO_HEADPHONE_MUTE, (unsigned int)mHwPtr[kAUDIO_GPIO_HEADPHONE_MUTE] );
	debug3IOLog ( "GPIO24 AUDIO_AMP_MUTE_H:      %X -> %X\n", kAUDIO_MAC_IO_BASE_ADDRESS + kAUDIO_GPIO_AMPLIFIER_MUTE, (unsigned int)mHwPtr[kAUDIO_GPIO_AMPLIFIER_MUTE] );
	debug3IOLog ( "GPIO28 AUDIO_HW_RESET_L:      %X -> %X\n", kAUDIO_MAC_IO_BASE_ADDRESS + kAUDIO_GPIO_ANALOG_CODEC_RESET, (unsigned int)mHwPtr[kAUDIO_GPIO_ANALOG_CODEC_RESET] );
	debug3IOLog ( "GPIO29 AUDIO_LINEOUT_MUTE_H:  %X -> %X\n", kAUDIO_MAC_IO_BASE_ADDRESS + kAUDIO_GPIO_LINE_OUT_MUTE, (unsigned int)mHwPtr[kAUDIO_GPIO_LINE_OUT_MUTE] );
	debug3IOLog ( "GPIO30 AUDIO_MCLK_SEL:        %X -> %X\n\n", kAUDIO_MAC_IO_BASE_ADDRESS + kAUDIO_GPIO_CLOCK_MUX_SELECT, (unsigned int)mHwPtr[kAUDIO_GPIO_CLOCK_MUX_SELECT] );
#endif
	return;
}
	
//	--------------------------------------------------------------------------------
void K2Platform::LogDBDMAChannelRegisters ( void ) {
#if 0
	debug2IOLog ( "Output ChannelControl:  %lX\n", mIOBaseDMAOutput->channelControl );
	debug2IOLog ( "Output channelStatus:   %lX\n", mIOBaseDMAOutput->channelStatus );
	debug2IOLog ( "Output commandPtrHi:    %lX\n", mIOBaseDMAOutput->commandPtrHi );
	debug2IOLog ( "Output commandPtrLo:    %lX\n", mIOBaseDMAOutput->commandPtrLo );
	debug2IOLog ( "Output interruptSelect: %lX\n", mIOBaseDMAOutput->interruptSelect );
	debug2IOLog ( "Output branchSelect:    %lX\n", mIOBaseDMAOutput->branchSelect );
	debug2IOLog ( "Output waitSelect:      %lX\n", mIOBaseDMAOutput->waitSelect );
	debug2IOLog ( "Output transferModes:   %lX\n", mIOBaseDMAOutput->transferModes );
	debug2IOLog ( "Output data2PtrHi:      %lX\n", mIOBaseDMAOutput->data2PtrHi );
	debug2IOLog ( "Output data2PtrLo:      %lX\n", mIOBaseDMAOutput->data2PtrLo );
	debug2IOLog ( "Output reserved1:       %lX\n", mIOBaseDMAOutput->reserved1 );
	debug2IOLog ( "Output addressHi:       %lX\n\n", mIOBaseDMAOutput->addressHi );
#endif
	return;
}





