/*
 *  AppleTopazAudio.cpp
 *  AppleOnboardAudio
 *
 *  Created by Matthew Xavier Mora on Thu Mar 13 2003.
 *  Copyright © 2003 Apple Computer, Inc. All rights reserved.
 *
 */

#include "AppleTopazAudio.h"
#include "PlatformInterface.h"
#include "AudioHardwareUtilities.h"

// uncomment to get more logging
//#define kVERBOSE_LOG

OSDefineMetaClassAndStructors(AppleTopazAudio, AudioHardwareObjectInterface)

#define super IOService

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// call into superclass and initialize.
bool AppleTopazAudio::init(OSDictionary *properties)
{
	debugIOLog ( "+ AppleTopazAudio::init\n" );
	if (!super::init(properties))
		return false;
		
	mRecoveryInProcess = false;
	
	debugIOLog ( "- AppleTopazAudio::init\n" );
	return true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool AppleTopazAudio::start (IOService * provider) {
	bool					result;

	debug3IOLog ("+AppleTopazAudio[%p]::start(%p)\n", this, provider);

	FailIf (!provider, Exit);
	mAudioDeviceProvider = (AppleOnboardAudio *)provider;

	result = super::start (provider);

	result = provider->open (this, kFamilyOption_OpenMultiple);
	
Exit:
	debug4IOLog ("-AppleTopazAudio[%p]::start(%p) returns bool %d\n", this, provider, result);
	return result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// call inherited free
void AppleTopazAudio::free()
{
	debugIOLog ( "+AppleTopazAudio::free\n" );

	super::free();
	debugIOLog ( "-AppleTopazAudio::free\n" );
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AppleTopazAudio::initPlugin(PlatformInterface* inPlatformObject) {

	
	debug2IOLog( "+AppleTopazAudio::initPlugin inPlatformObject = %X\n", (unsigned int)inPlatformObject);
	
	mPlatformInterface = inPlatformObject;

	debugIOLog ( "-AppleTopazAudio::initPlugin\n" );
		
	return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool AppleTopazAudio::willTerminate ( IOService * provider, IOOptionBits options ) {

	bool result = super::willTerminate( provider, options );

	debug4IOLog("AppleTopazAudio::willTerminate(%p) returns %d, mAudioDeviceProvider = %p\n", provider, result, mAudioDeviceProvider);

	if (provider == mAudioDeviceProvider) {
		debugIOLog ("closing our provider\n" );
		provider->close (this);
	}

	debug4IOLog ("- AppleTopazAudio[%p]::willTerminate(%p) returns %s\n", this, provider, result == true ? "true" : "false" );
	return result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool AppleTopazAudio::requestTerminate ( IOService * provider, IOOptionBits options ) {

	bool result = super::requestTerminate( provider, options );
	debug4IOLog("AppleTopazAudio::requestTerminate(%p) returns %d, mAudioDeviceProvider = %p\n", provider, result, mAudioDeviceProvider);

	return result;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool AppleTopazAudio::preDMAEngineInit () {
	IOReturn		err;
	UInt8			data;
	bool			result = false;
	
	//	Initialize the register cache to the current hardware register values by iterating
	//	through read accesses to all registers.  Passing a NULL result pointer will result
	//	in updating the register cache without copying data from the cache to a destination
	//	buffer.
	debugIOLog ( "+AppleTopazAudio::preDMAEngineInit\n" );

	FailIf ( NULL == mPlatformInterface, Exit );
	
	CODEC_Reset ();
	
	mShadowRegs[map_ID_VERSION] = 0;
	err = CODEC_ReadRegister ( map_ID_VERSION, NULL, kTOPAZ_AccessMode_FORCE_UPDATE_ALL, 1 );
	FailIf ( kIOReturnSuccess != err, Exit );
	
	if ( ( cs8420_id << baID ) == ( mShadowRegs[map_ID_VERSION] & kCS84XX_ID_MASK ) ) {
		debugIOLog ( "Topaz Codec = CS8420\n" );
		mCodecID = kCS8420_CODEC;
	} else if ( ( cs8406_id << baID ) == ( mShadowRegs[map_ID_VERSION] & kCS84XX_ID_MASK ) ) {
		debugIOLog ( "Topaz Codec = CS8406\n" );
		mCodecID = kCS8406_CODEC;
	} else {
		debugIOLog ( "Topaz Codec = UNKNOWN\n" );
		FailIf ( true, Exit );
	}
	for ( UInt32 loopCnt = map_MISC_CNTRL_1; loopCnt <= map_BUFFER_23; loopCnt++ ) {
		if ( map_RX_ERROR != loopCnt && 0x1F != loopCnt ) {												//	avoid hole in register address space
			err = CODEC_ReadRegister ( loopCnt, NULL, kTOPAZ_AccessMode_FORCE_UPDATE_ALL, 1 );			//	read I2C register into cache only
		}
	}
	
	//	The CS8420 does not require the I2S clocks be running in order to access the I2C registers so
	//	initialization of the registers is not deferred until after an instance of the AudioI2SControl
	//	exists (unlike the TAS3004 for example).
	if ( kCS8406_CODEC == mCodecID ) {
		debug2IOLog ( "kMISC_CNTRL_1_INIT_8406          = %x\n", kMISC_CNTRL_1_INIT_8406 );
	} else if ( kCS8420_CODEC == mCodecID ) {
		debug2IOLog ( "kMISC_CNTRL_1_INIT_8420          = %x\n", kMISC_CNTRL_1_INIT_8420 );
	}
	debug2IOLog ( "kMISC_CNTRL_2_INIT               = %x\n", kMISC_CNTRL_2_INIT );
	debug2IOLog ( "kDATA_FLOW_CTRL_INIT             = %x\n", kDATA_FLOW_CTRL_INIT );
	debug2IOLog ( "kCLOCK_SOURCE_CTRL_INIT_STOP     = %x\n", kCLOCK_SOURCE_CTRL_INIT_STOP );
	debug2IOLog ( "kCLOCK_SOURCE_CTRL_INIT          = %x\n", kCLOCK_SOURCE_CTRL_INIT );
	debug2IOLog ( "kSERIAL_AUDIO_INPUT_FORMAT_INIT  = %x\n", kSERIAL_AUDIO_INPUT_FORMAT_INIT );
	debug2IOLog ( "kSERIAL_AUDIO_OUTPUT_FORMAT_INIT = %x\n", kSERIAL_AUDIO_OUTPUT_FORMAT_INIT );
	debug2IOLog ( "kRX_ERROR_MASK1_INIT             = %x\n", kRX_ERROR_MASK1_INIT );
	
	//	Place device into power down state prior to initialization
	data = kCLOCK_SOURCE_CTRL_INIT_STOP;
	data &= ~( kCS84XX_BIT_MASK << baRUN );
	data |= ( runSTOP << baRUN );
	err = CODEC_WriteRegister ( map_CLOCK_SOURCE_CTRL, data, kTOPAZ_AccessMode_FORCE_UPDATE_ALL );
	FailIf ( kIOReturnSuccess != err, Exit );
	
	data = kCS8420_CODEC == mCodecID ? kMISC_CNTRL_1_INIT_8420 : kMISC_CNTRL_1_INIT_8406 ;
	err = CODEC_WriteRegister ( map_MISC_CNTRL_1, data, kTOPAZ_AccessMode_FORCE_UPDATE_ALL );
	FailIf ( kIOReturnSuccess != err, Exit );
	
	err = CODEC_WriteRegister ( map_MISC_CNTRL_2, kMISC_CNTRL_2_INIT, kTOPAZ_AccessMode_FORCE_UPDATE_ALL );
	FailIf ( kIOReturnSuccess != err, Exit );
	
	err = CODEC_WriteRegister ( map_DATA_FLOW_CTRL, kDATA_FLOW_CTRL_INIT, kTOPAZ_AccessMode_FORCE_UPDATE_ALL );
	FailIf ( kIOReturnSuccess != err, Exit );
	
	err = CODEC_WriteRegister ( map_CLOCK_SOURCE_CTRL, kCLOCK_SOURCE_CTRL_INIT, kTOPAZ_AccessMode_FORCE_UPDATE_ALL );
	FailIf ( kIOReturnSuccess != err, Exit );
	
	err = CODEC_WriteRegister ( map_SERIAL_INPUT_FMT, kSERIAL_AUDIO_INPUT_FORMAT_INIT, kTOPAZ_AccessMode_FORCE_UPDATE_ALL );
	FailIf ( kIOReturnSuccess != err, Exit );
	
	if ( kCS8420_CODEC == mCodecID ) {
		err = CODEC_WriteRegister ( map_SERIAL_OUTPUT_FMT, kSERIAL_AUDIO_OUTPUT_FORMAT_INIT, kTOPAZ_AccessMode_FORCE_UPDATE_ALL );
		FailIf ( kIOReturnSuccess != err, Exit );
	
		err = CODEC_WriteRegister ( map_RX_ERROR_MASK, kRX_ERROR_MASK1_INIT, kTOPAZ_AccessMode_FORCE_UPDATE_ALL );
		FailIf ( kIOReturnSuccess != err, Exit );
		
		err = CODEC_ReadRegister ( map_RX_ERROR, NULL, kTOPAZ_AccessMode_FORCE_UPDATE_ALL, 1 );			//	read I2C register into cache only
		if ( kIOReturnSuccess == err ) {
			mClockUnlockStatus = (( mShadowRegs[map_RX_ERROR] >> baUNLOCKM ) & 0x01);
		}
	}

	data = kCLOCK_SOURCE_CTRL_INIT_STOP;
	data &= ~( kCS84XX_BIT_MASK << baRUN );
	data |= ( runNORMAL << baRUN );
	err = CODEC_WriteRegister ( map_CLOCK_SOURCE_CTRL, data, kTOPAZ_AccessMode_FORCE_UPDATE_ALL );
	FailIf ( kIOReturnSuccess != err, Exit );
	
	FailIf ( kIOReturnSuccess != err, Exit );
	
	result = true;
Exit:
	CODEC_LogRegisters();
	debug2IOLog( "-AppleTopazAudio::preDMAEngineInit err = %X\n", err );

	return result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AppleTopazAudio::postDMAEngineInit () {

	debugIOLog ( "+ AppleTopazAudio::postDMAEngineInit\n" );
	CODEC_LogRegisters();
	debugIOLog ( "- AppleTopazAudio::postDMAEngineInit\n" );

	return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool AppleTopazAudio::getMute () {
	bool		result;
	
	debugIOLog( "+ AppleTopazAudio::getMute\n" );
	
	result = false;
	FailIf ( kIOReturnSuccess != CODEC_ReadRegister ( map_MISC_CNTRL_1, NULL, kTOPAZ_AccessMode_FORCE_UPDATE_ALL, 1 ), Exit );
	if ( 0 != ( mShadowRegs[map_MISC_CNTRL_1] & ( 1 << baMuteAES ) ) ) {
		result = true;
	}
Exit:	
	debug2IOLog( "- AppleTopazAudio::getMute result = %d\n", result );
	return ( result );	
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
IOReturn AppleTopazAudio::setMute (bool muteState) {
	UInt8			data;
	IOReturn		result;
	
	result = kIOReturnSuccess;
	if ( true == muteState ) {
		// mute the part
		result = CODEC_ReadRegister ( map_MISC_CNTRL_1, &data, kTOPAZ_AccessMode_FORCE_UPDATE_ALL, 1 );
		FailIf ( kIOReturnSuccess != result, Exit );
		
		data &= ~( ( 1 << baMuteSAO ) | ( 1 << baMuteAES ) );
		data |= ( ( muteSAO << baMuteSAO ) | ( muteAES3 << baMuteAES ) );
		result = CODEC_WriteRegister ( map_MISC_CNTRL_1, data, kTOPAZ_AccessMode_FORCE_UPDATE_ALL );
		FailIf ( kIOReturnSuccess != result, Exit );
	} else {
		// unmute the part
		result = CODEC_ReadRegister ( map_MISC_CNTRL_1, &data, kTOPAZ_AccessMode_FORCE_UPDATE_ALL, 1 );
		FailIf ( kIOReturnSuccess != result, Exit );
		
		data &= ~( ( 1 << baMuteSAO ) | ( 1 << baMuteAES ) );
		data |= ( ( normalSAO << baMuteSAO ) | ( normalAES3 << baMuteAES ) );
		result = CODEC_WriteRegister ( map_MISC_CNTRL_1, data, kTOPAZ_AccessMode_FORCE_UPDATE_ALL );
		FailIf ( kIOReturnSuccess != result, Exit );
	}
	CODEC_LogRegisters();
Exit:
	debug3IOLog ( "-AppleTopazAudio::setMute ( %d ) result %d\n", muteState, result );
	return ( result );	
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Stop the internal clocks and set the transmit output to 0 volts for low 
//	power consumption.  Device register updates are applied only to the
//	device and avoid touching the register cache.
IOReturn AppleTopazAudio::performDeviceSleep () {
	IOReturn			result;
	
	mShadowRegs[map_DATA_FLOW_CTRL] &= ~( kCS84XX_BIT_MASK << baTXOFF );
	mShadowRegs[map_DATA_FLOW_CTRL] |= ( aes3TX0v << baTXOFF );
	result = CODEC_WriteRegister ( map_DATA_FLOW_CTRL, mShadowRegs[map_DATA_FLOW_CTRL], kTOPAZ_AccessMode_UPDATE_HW );
	FailIf ( kIOReturnSuccess != result, Exit );
	
	mShadowRegs[map_CLOCK_SOURCE_CTRL] &= ~( kCS84XX_BIT_MASK << baRUN );
	mShadowRegs[map_CLOCK_SOURCE_CTRL] |= ( runSTOP << baRUN );
	result = CODEC_WriteRegister ( map_CLOCK_SOURCE_CTRL, mShadowRegs[map_CLOCK_SOURCE_CTRL], kTOPAZ_AccessMode_UPDATE_HW );
	FailIf ( kIOReturnSuccess != result, Exit );

Exit:
	return result;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Reset the device and flush the cached register set to the device.  This
//	is necessary for portable CPUs where power will have been removed from
//	the device during sleep.  After flushing the cached register values to
//	the device, place the device into the RUN state.
IOReturn AppleTopazAudio::performDeviceWake () {
	IOReturn			result;
	
	CODEC_Reset ();
	for ( UInt32 regAddr = map_MISC_CNTRL_1; regAddr <= map_BUFFER_23; regAddr++ ) {
		if ( kIOReturnSuccess == CODEC_IsControlRegister ( regAddr ) ) {
			result = CODEC_WriteRegister ( regAddr, mShadowRegs[regAddr], kTOPAZ_AccessMode_UPDATE_HW );
			FailIf ( kIOReturnSuccess != result, Exit );
		}
	}
	
	mShadowRegs[map_DATA_FLOW_CTRL] &= ~( kCS84XX_BIT_MASK << baTXOFF );
	mShadowRegs[map_DATA_FLOW_CTRL] |= ( aes3TXNormal << baTXOFF );
	result = CODEC_WriteRegister ( map_DATA_FLOW_CTRL, mShadowRegs[map_DATA_FLOW_CTRL], kTOPAZ_AccessMode_UPDATE_ALL );
	FailIf ( kIOReturnSuccess != result, Exit );

	mShadowRegs[map_CLOCK_SOURCE_CTRL] &= ~( kCS84XX_BIT_MASK << baRUN );
	mShadowRegs[map_CLOCK_SOURCE_CTRL] |= ( runNORMAL << baRUN );
	result = CODEC_WriteRegister ( map_CLOCK_SOURCE_CTRL, mShadowRegs[map_CLOCK_SOURCE_CTRL], kTOPAZ_AccessMode_UPDATE_ALL );
	FailIf ( kIOReturnSuccess != result, Exit );

Exit:
	return result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
IOReturn	AppleTopazAudio::setSampleRate ( UInt32 sampleRate ) {
	IOReturn		result = kIOReturnBadArgument;

	FailIf ( !( 32000 == sampleRate ||  44100 == sampleRate || 48000 == sampleRate || 88200 == sampleRate  || 96000 == sampleRate || 192000 == sampleRate ), Exit );
	generalRecovery();
	result = kIOReturnSuccess;
Exit:
	return result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
IOReturn	AppleTopazAudio::setSampleDepth ( UInt32 sampleDepth ) {
	IOReturn		result = kIOReturnBadArgument;
	
	FailIf ( !( 16 == sampleDepth || 24 == sampleDepth ), Exit );
	generalRecovery();
	result = kIOReturnSuccess;
Exit:
	return result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
UInt32	AppleTopazAudio::getClockLock ( void ) {
	debugIOLog ( "+ AppleTopazAudio::getClockLock\n" );
	debug2IOLog ( "- AppleTopazAudio::getClockLock returns %d\n", (unsigned int)mClockUnlockStatus );
	return mClockUnlockStatus;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Clock selection sequence is as follows:
//	1.	Transport object receives breakClockSelect notification and
//		performs the "BREAK" portion of a "BREAK BEFORE MAKE" action.
//	2.	Hardware Plugin object receives breakClockSelect notification and
//		performs the "BREAK" portion of a "BREAK BEFORE MAKE" action.
//	3.	Transport object receives makeClockSelect notification and
//		performs the "MAKE" portion of a "BREAK BEFORE MAKE" action.
//	4.	Hardware Plugin object receives makeClockSelect notification and
//		performs the "MAKE" portion of a "BREAK BEFORE MAKE" action.
IOReturn	AppleTopazAudio::breakClockSelect ( UInt32 clockSource ) {
	UInt8				data;
	IOReturn			result;
	
	debug2IOLog ( "+ AppleTopazAudio::breakClockSelect ( %d )\n", (unsigned int)clockSource );
	result = kIOReturnSuccess;
	if ( kTRANSPORT_MASTER_CLOCK == clockSource || kTRANSPORT_SLAVE_CLOCK == clockSource ) {
		//	Mute by using the cache for a read / modify / write but don't touch the
		//	cache during the write so that the make operation can flush the cache to
		//	the device to restore the mute state.
		data = mShadowRegs[map_DATA_FLOW_CTRL];
		data &= ~( kCS84XX_BIT_MASK << baTXOFF );
		data |= ( aes3TX0v << baTXOFF );
		result = CODEC_WriteRegister ( map_DATA_FLOW_CTRL, data, kTOPAZ_AccessMode_UPDATE_HW );
		FailIf ( kIOReturnSuccess != result, Exit );
	}
	switch ( clockSource ) {
		case kTRANSPORT_MASTER_CLOCK:
			if ( kCS8420_CODEC == mCodecID ) {
				mShadowRegs[map_CLOCK_SOURCE_CTRL] &= ~( kCS84XX_TWO_BIT_MASK << baRXD );
				mShadowRegs[map_CLOCK_SOURCE_CTRL] |= ( rxd256fsiILRCLK << baRXD );
				result = CODEC_WriteRegister ( map_CLOCK_SOURCE_CTRL, mShadowRegs[map_CLOCK_SOURCE_CTRL], kTOPAZ_AccessMode_UPDATE_HW );
				FailIf ( kIOReturnSuccess != result, Exit );
			}
			break;
		case kTRANSPORT_SLAVE_CLOCK:
			if ( kCS8420_CODEC == mCodecID ) {
				mShadowRegs[map_CLOCK_SOURCE_CTRL] &= ~( kCS84XX_TWO_BIT_MASK << baRXD );
				mShadowRegs[map_CLOCK_SOURCE_CTRL] |= ( rxd256fsiAES3 << baRXD );
				result = CODEC_WriteRegister ( map_CLOCK_SOURCE_CTRL, mShadowRegs[map_CLOCK_SOURCE_CTRL], kTOPAZ_AccessMode_UPDATE_HW );
				FailIf ( kIOReturnSuccess != result, Exit );
			}
			break;
		default:
			debugIOLog ( "breakClockSelect clockSource UNKNOWN\n" );
			result = kIOReturnBadArgument;
			break;
	}
Exit:
	debug3IOLog ( "- AppleTopazAudio::breakClockSelect ( %d ) returns %d\n", (unsigned int)clockSource, (unsigned int)result );
	return result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
IOReturn	AppleTopazAudio::makeClockSelect ( UInt32 clockSource ) {
	IOReturn			result;
	
	debug2IOLog ( "+ AppleTopazAudio::makeClockSelect ( %d )\n", (unsigned int)clockSource );
	result = kIOReturnSuccess;
	switch ( clockSource ) {
		case kTRANSPORT_MASTER_CLOCK:		//	fall through to slave
		case kTRANSPORT_SLAVE_CLOCK:
			if ( kCS8420_CODEC == mCodecID ) {
				generalRecovery();
			}
			break;
		default:
			debugIOLog ( "makeClockSelect clockSource UNKNOWN\n" );
			result = kIOReturnBadArgument;
			break;
	}
	debug3IOLog ( "- AppleTopazAudio::makeClockSelect ( %d ) returns %d\n", (unsigned int)clockSource, (unsigned int)result );
	return result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Fatal error recovery 
void AppleTopazAudio::generalRecovery ( void ) {

	CODEC_Reset ();
	for ( UInt32 regAddr = map_MISC_CNTRL_1; regAddr <= map_BUFFER_23; regAddr++ ) {
		if ( kIOReturnSuccess == CODEC_IsControlRegister ( regAddr ) ) {
			FailIf ( kIOReturnSuccess != CODEC_WriteRegister ( regAddr, mShadowRegs[regAddr], kTOPAZ_AccessMode_UPDATE_HW ), Exit );
		}
	}
Exit:
	return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Fatal error recovery 
#pragma WARNING NEED TO IMPLEMENT RECEIVER IN AOA AND BROADCASTER IN HW PLUGINS
void AppleTopazAudio::recoverFromFatalError ( FatalRecoverySelector selector ) {

	debug2IOLog ( "+ AppleTopazAudio::recoverFromFatalError ( %d )\n", (unsigned int)selector );
	FailIf ( NULL == mPlatformInterface, Exit );
	if ( mRecoveryInProcess ) { debugIOLog ( "REDUNDANT RECOVERY FROM FATAL ERROR\n" ); }
	
	mRecoveryInProcess = true;
	switch ( selector ) {
		case kControlBusFatalErrorRecovery:
			generalRecovery();
			break;
		case kClockSourceInterruptedRecovery:
			generalRecovery();
			break;
		default:
			debugIOLog ( "*** Requested recovery from unknown condition!\n" );
			break;
	}
	mRecoveryInProcess = false;
	
Exit:
	debug2IOLog ( "- AppleTopazAudio::recoverFromFatalError ( %d )\n", (unsigned int)selector );
	return;
}

UInt32 AppleTopazAudio::getCurrentSampleFrame (void) {
	return mPlatformInterface->getFrameCount ();
}

void AppleTopazAudio::setCurrentSampleFrame (UInt32 value) {
	mPlatformInterface->setFrameCount (value);
}

#pragma mark ---------------------
#pragma mark •	INTERRUPT HANDLERS
#pragma mark ---------------------
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AppleTopazAudio::notifyHardwareEvent ( UInt32 statusSelector, UInt32 newValue ) {
	IOReturn		err;
	UInt8			oldData, changedBits, data;
	bool			badDataPending = false;
	
	debug3IOLog ( "+ notifyHardwareEvent( 0x%X, %d )\n", (unsigned int)statusSelector, (unsigned int)newValue );
	
	if ( kCodecInterruptStatus == statusSelector ) {
		
	} else if ( kCodecErrorInterruptStatus == statusSelector ) {
		//	Determine the current clock lock status and if the lock status has changed then
		//	notify AppleOnboardAudio of the change in clock lock status.
		oldData = mShadowRegs[map_RX_ERROR];
		err = CODEC_ReadRegister ( map_RX_ERROR, &data, kTOPAZ_AccessMode_FORCE_UPDATE_ALL, 1 );
		FailIf ( kIOReturnSuccess != err, Exit );
		
		if ( oldData != data ) {
			changedBits = oldData ^ data;
			for ( UInt32 index = baPARITYM; index <= baQCRCM; index++ ) {
				if ( 0 != ( changedBits & ( 1 << index ) ) ) {
					switch ( index ) {
						case baUNLOCKM:	
							mClockUnlockStatus = (( data >> baUNLOCKM ) & 0x01);
							mAudioDeviceProvider->interruptEventHandler ( kClockLockStatus, (UInt32)mClockUnlockStatus );
							break;
						case baQCRCM:			
						case baCCRCM:			
						case baVALIDM:			
						case baCONFM:			
						case baBIPM:			
						case baPARITYM:
							badDataPending = true;
							break;
					}
				}
			}
			if ( badDataPending ) {
				mAudioDeviceProvider->interruptEventHandler ( kAES3StreamErrorStatus, (UInt32)mShadowRegs[map_RX_ERROR] );
			}
		}
	}
Exit:
	debug3IOLog ( "- notifyHardwareEvent( 0x%X, %d )\n", (unsigned int)statusSelector, (unsigned int)newValue );
	return;
}

#pragma mark ---------------------
#pragma mark • CODEC Functions
#pragma mark ---------------------

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Mask off CS8420 bits that are not supported by the CS8406.
//
UInt8	AppleTopazAudio::CODEC_GetDataMask ( UInt8 regAddr ) {
	UInt8		mask;
	
	mask = kMASK_NONE;
	if ( kCS8406_CODEC == mCodecID ) {
		switch ( regAddr ) {
			case map_MISC_CNTRL_1:					mask = kMISC_CNTRL_1_INIT_8406_MASK;			break;
			case map_MISC_CNTRL_2:					mask = kMISC_CNTRL_2_INIT_8406_MASK;			break;
			case map_DATA_FLOW_CTRL:				mask = kDATA_FLOW_CTR_8406_MASK;				break;
			case map_CLOCK_SOURCE_CTRL:				mask = kCLOCK_SOURCE_CTR_8406_MASK;				break;
			case map_SERIAL_INPUT_FMT:				mask = kSERIAL_AUDIO_INPUT_FORMAT_8406_MASK;	break;
			case map_IRQ1_MASK:						mask = kIRQ1_8406_MASK_8406_MASK;				break;
			case map_IRQ1_MODE_MSB:					mask = kIRQ1_8406_MASK_8406_MASK;				break;
			case map_IRQ1_MODE_LSB:					mask = kIRQ1_8406_MASK_8406_MASK;				break;
			case map_IRQ2_MASK:						mask = kIRQ2_8406_MASK_8406_MASK;				break;
			case map_IRQ2_MODE_MSB:					mask = kIRQ2_8406_MASK_8406_MASK;				break;
			case map_IRQ2_MODE_LSB:					mask = kIRQ2_8406_MASK_8406_MASK;				break;
			case map_CH_STATUS_DATA_BUF_CTRL:		mask = kCH_STATUS_DATA_BUF_CTRL_8406_MASK;		break;
			case map_USER_DATA_BUF_CTRL:			mask = kUSER_DATA_BUF_CTRLL_8406_MASK;			break;
			default:								mask = kMASK_ALL;								break;
		}
	}
	return mask;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
IOReturn	AppleTopazAudio::CODEC_GetRegSize ( UInt32 regAddr, UInt32 * codecRegSizePtr )
{
	IOReturn		result;
	
#ifdef kVERBOSE_LOG	
	debug3IOLog("+AppleTopazAudio::CODEC_GetRegSize selector = %d, codecRegSizePtr value = %X\n", (int)regAddr, (unsigned int)codecRegSizePtr);
#endif
	
	result = kIOReturnError;
	if ( NULL != codecRegSizePtr ) {
		if ( map_BUFFER_0 == regAddr ) {
			* codecRegSizePtr = 24;
			result = kIOReturnSuccess;
		} else if ( CODEC_IsControlRegister( (char)regAddr ) || CODEC_IsStatusRegister( (char)regAddr ) ) {
			* codecRegSizePtr = 1;
			result = kIOReturnSuccess;
		} else {
			* codecRegSizePtr = 0;
		}
	}
	
	if ( kIOReturnSuccess != result ) { debug3IOLog( " AppleTopazAudio::CODEC_GetRegSize register %x, result = %X\n", (unsigned int)regAddr, result ); }

#ifdef kVERBOSE_LOG	
	debug3IOLog("-AppleTopazAudio::CODEC_GetRegSize selector = %d, codecRegSizePtr value = %X\n", (int)regAddr, (unsigned int)codecRegSizePtr);
#endif
	return result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
IOReturn AppleTopazAudio::CODEC_IsControlRegister ( UInt8 regAddr ) {
	IOReturn	result;

	switch ( regAddr ) {
		case map_SERIAL_OUTPUT_FMT:
		case map_RX_ERROR_MASK:
			switch ( mCodecID ) {
				case kCS8406_CODEC:					result = kIOReturnError;			break;
				case kCS8420_CODEC:					result = kIOReturnSuccess;			break;
			};
			break;
		case map_MISC_CNTRL_1:
		case map_MISC_CNTRL_2:
		case map_DATA_FLOW_CTRL:
		case map_CLOCK_SOURCE_CTRL:
		case map_SERIAL_INPUT_FMT:
		case map_IRQ1_MASK:
		case map_IRQ1_MODE_MSB:
		case map_IRQ1_MODE_LSB:
		case map_IRQ2_MASK:
		case map_IRQ2_MODE_MSB:
		case map_IRQ2_MODE_LSB:
		case map_CH_STATUS_DATA_BUF_CTRL:
		case map_USER_DATA_BUF_CTRL:				result = kIOReturnSuccess;			break;
		default:									result = kIOReturnError;			break;
	}

	return result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
IOReturn 	AppleTopazAudio::CODEC_IsStatusRegister ( UInt8 regAddr ) {
	IOReturn	result;
	
	switch ( regAddr ) {
		case map_IRQ1_STATUS:
		case map_IRQ2_STATUS:
		case map_RX_CH_STATUS:
		case map_RX_ERROR:
		case map_ID_VERSION:						result = kIOReturnSuccess;			break;
		case map_Q_CHANNEL_SUBCODE_AC:
		case map_Q_CHANNEL_SUBCODE_TRK:
		case map_Q_CHANNEL_SUBCODE_INDEX:
		case map_Q_CHANNEL_SUBCODE_MIN:
		case map_Q_CHANNEL_SUBCODE_SEC:
		case map_Q_CHANNEL_SUBCODE_FRAME:
		case map_Q_CHANNEL_SUBCODE_ZERO:
		case map_Q_CHANNEL_SUBCODE_ABS_MIN:
		case map_Q_CHANNEL_SUBCODE_ABS_SEC:
		case map_Q_CHANNEL_SUBCODE_ABS_FRAME:
		case map_SAMPLE_RATE_RATIO:
			switch ( mCodecID ) {
				case kCS8406_CODEC:					result = kIOReturnError;			break;
				case kCS8420_CODEC:					result = kIOReturnSuccess;			break;
			};
			break;
		default:
			if ( ( map_BUFFER_0 <= regAddr ) && ( regAddr <= map_BUFFER_23 ) ) {
				result = kIOReturnSuccess;
			} else {
				result = kIOReturnError;
			}
			break;
	}
	
	return result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void 	AppleTopazAudio::CODEC_LogRegisters ( void ) {
#if DEBUGLOG
	UInt8			regData[256];

	CODEC_ReadRegister ( 1, &regData[1], kTOPAZ_AccessMode_FORCE_UPDATE_ALL, 1 );
	CODEC_ReadRegister ( 2, &regData[2], kTOPAZ_AccessMode_FORCE_UPDATE_ALL, 1 ); 
	CODEC_ReadRegister ( 3, &regData[3], kTOPAZ_AccessMode_FORCE_UPDATE_ALL, 1 ); 
	CODEC_ReadRegister ( 4, &regData[4], kTOPAZ_AccessMode_FORCE_UPDATE_ALL, 1 ); 
	CODEC_ReadRegister ( 5, &regData[5], kTOPAZ_AccessMode_FORCE_UPDATE_ALL, 1 ); 
	CODEC_ReadRegister ( 6, &regData[6], kTOPAZ_AccessMode_FORCE_UPDATE_ALL, 1 ); 
	CODEC_ReadRegister ( 7, &regData[7], kTOPAZ_AccessMode_FORCE_UPDATE_ALL, 1 ); 
	CODEC_ReadRegister ( 8, &regData[8], kTOPAZ_AccessMode_FORCE_UPDATE_ALL, 1 );
	CODEC_ReadRegister ( 9, &regData[9], kTOPAZ_AccessMode_FORCE_UPDATE_ALL, 1 ); 
	CODEC_ReadRegister ( 10, &regData[10], kTOPAZ_AccessMode_FORCE_UPDATE_ALL, 1 ); 
	CODEC_ReadRegister ( 11, &regData[11], kTOPAZ_AccessMode_FORCE_UPDATE_ALL, 1 ); 
	CODEC_ReadRegister ( 12, &regData[12], kTOPAZ_AccessMode_FORCE_UPDATE_ALL, 1 ); 
	CODEC_ReadRegister ( 13, &regData[13], kTOPAZ_AccessMode_FORCE_UPDATE_ALL, 1 ); 
	CODEC_ReadRegister ( 14, &regData[14], kTOPAZ_AccessMode_FORCE_UPDATE_ALL, 1 ); 
	CODEC_ReadRegister ( 15, &regData[15], kTOPAZ_AccessMode_FORCE_UPDATE_ALL, 1 ); 
	CODEC_ReadRegister ( 16, &regData[16], kTOPAZ_AccessMode_FORCE_UPDATE_ALL, 1 );
	debug9IOLog ( "map = 1: %x %x %x %x %x %x %x %x\n", 
		regData[1], regData[2], regData[3], regData[4], regData[5], regData[6], regData[7], regData[8]
	);
	debug9IOLog ( "map = 9: %x %x %x %x %x %x %x %x\n",
		regData[9], regData[10], regData[11], regData[12], regData[13], regData[14], regData[15], regData[16]
	);
#endif	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Read operations can source data from the device or from a write through
//	cache.  By specifying a 'mode' of kTOPAZ_AccessMode_FORCE_UPDATE_ALL, the device will be
//	accessed.  The size of the access may be passed in.  For most cases, the
//	size will be a single byte.  Channel status or user status may stipulate
//	a size of 24 bytes to optimize the register access.
IOReturn 	AppleTopazAudio::CODEC_ReadRegister ( UInt8 regAddr, UInt8 * registerData, TOPAZ_AccessMode mode, UInt32 size ) {
	IOReturn		result;
	Boolean			success;
	UInt32			index;
	UInt32			codecRegSize;

#ifdef kVERBOSE_LOG	
	if ( NULL == registerData ) {
		debug5IOLog("+AppleTopazAudio::CODEC_ReadRegister regAddr = %X registerData = %p mode = %d size = %ul\n", regAddr, registerData, mode, (unsigned int)size);
	} else {
		debug5IOLog("+AppleTopazAudio::CODEC_ReadRegister regAddr = %X registerData = %X mode = %d size = %ul\n", regAddr, *registerData, mode, (unsigned int)size);
	}
#endif

	FailIf ( NULL == mPlatformInterface, Exit );
	result = kIOReturnSuccess;
	success = false;
	if ( 1 < size ) { regAddr |= (UInt8)kMAP_AUTO_INCREMENT_ENABLE; }
	
	if ( kIOReturnSuccess == CODEC_GetRegSize ( regAddr, &codecRegSize ) ) {
		if ( 0 != size && size <= codecRegSize ) {
			//	Write through to the shadow register as a 'write through' cache would and
			//	then write the data to the hardware;
			if ( kIOReturnSuccess == CODEC_IsControlRegister ( regAddr ) || kIOReturnSuccess == CODEC_IsStatusRegister ( regAddr ) ) {
				success = true;
				//	If an update is forced, the hardware register will be read into the register
				//	cache prior to returning the register value from the register cache.
				if ( kTOPAZ_AccessMode_FORCE_UPDATE_ALL == mode ) {

					//	Must write the MAP register prior to performing the READ access
					success = mPlatformInterface->writeCodecRegister( kCS84xx_I2C_ADDRESS, 0, &regAddr, 1, kI2C_StandardMode);

					if ( success ) {
						success = mPlatformInterface->readCodecRegister(kCS84xx_I2C_ADDRESS, 0, &mShadowRegs[regAddr & ~kMAP_AUTO_INCREMENT_ENABLE], size, kI2C_StandardMode);

						if ( !success ) {
							debug4IOLog ( "S/PDIF FAIL readI2CBus ( %X, %X, %X );\n", (unsigned int)kCS84xx_I2C_ADDRESS, (unsigned int)&mShadowRegs[regAddr & ~kMAP_AUTO_INCREMENT_ENABLE], (unsigned int)size );
						}
					} else {
						debug4IOLog ( "S/PDIF FAIL writeI2CBus ( %X, %X, %X, 0 ) could not set MAP;\n", (unsigned int)kCS84xx_I2C_ADDRESS, (unsigned int)regAddr & ~kMAP_AUTO_INCREMENT_ENABLE, (unsigned int)&regAddr );
					}
				}
				if ( NULL != registerData && success ) {
					for ( index = 0; index < size; index++ ) {
						registerData[index] = mShadowRegs[(regAddr & ~kMAP_AUTO_INCREMENT_ENABLE) + index];
					}
				}
			} else {
				debug2IOLog ( "not a control or status register at register %X\n", regAddr );
			}
		} else {
			debugIOLog ( "codec register size is invalid\n" );
		}
	}
	if ( !success ) { result = kIOReturnError; }
 
Exit:	
	if ( kIOReturnSuccess != result ) { debug2IOLog( "-AppleTopazAudio::CODEC_ReadRegister result = %X\n", result ); }
#ifdef kVERBOSE_LOG	
	if ( NULL == registerData ) {
		debug6IOLog("-AppleTopazAudio::CODEC_ReadRegister regAddr = %X registerData = %p mode = %d size = %ul, returns %d\n", regAddr, registerData, mode, (unsigned int)size, result);
	} else {
		debug6IOLog("-AppleTopazAudio::CODEC_ReadRegister regAddr = %X registerData = %X mode = %d size = %ul, returns %d\n", regAddr, *registerData, mode, (unsigned int)size, result);
	}
	debugIOLog ( "\n" );
	IOSleep ( 500 );
#endif
	return result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void	AppleTopazAudio::CODEC_Reset ( void ) {
	debugIOLog ("+ AppleTopazAudio::CODEC_Reset ()\n");

	FailIf ( NULL == mPlatformInterface, Exit );

	mPlatformInterface->setCodecReset ( kCODEC_RESET_Digital, kGPIO_Run );
	IODelay ( 250 );
	mPlatformInterface->setCodecReset ( kCODEC_RESET_Digital, kGPIO_Reset );
	IODelay ( 250 );
	mPlatformInterface->setCodecReset ( kCODEC_RESET_Digital, kGPIO_Run );
	IODelay ( 250 );
Exit:
	debugIOLog ("- AppleTopazAudio::CODEC_Reset ()\n");
	return;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	All CODEC write operations pass through this function so that a shadow
//	copy of the registers can be kept in global storage.  All CS8420 control
//	registers are one byte in length.
IOReturn 	AppleTopazAudio::CODEC_WriteRegister ( UInt8 regAddr, UInt8 registerData, TOPAZ_AccessMode mode ) {
	IOReturn		result;
	Boolean			updateRequired;

#ifdef kVERBOSE_LOG	
	debug4IOLog("+AppleTopazAudio::CODEC_WriteRegister regAddr = %X registerData =  %X mode = %X\n",regAddr,  registerData,  mode);
#endif
	
	FailIf ( NULL == mPlatformInterface, Exit );
	result = kIOReturnError;
	updateRequired = false;

	//	Write through to the shadow register as a 'write through' cache would and
	//	then write the data to the hardware;
	if ( kIOReturnSuccess == CODEC_IsControlRegister ( regAddr ) ) {
		registerData &= CODEC_GetDataMask ( regAddr );
		if( kTOPAZ_AccessMode_UPDATE_SHADOW == mode || kTOPAZ_AccessMode_UPDATE_ALL == mode || kTOPAZ_AccessMode_FORCE_UPDATE_ALL == mode ) {
			if( mShadowRegs[regAddr] != registerData && kTOPAZ_AccessMode_UPDATE_ALL == mode ) {
				updateRequired = true;
			}
			mShadowRegs[regAddr] = registerData;
		}

		if( kTOPAZ_AccessMode_UPDATE_HW == mode || updateRequired || kTOPAZ_AccessMode_FORCE_UPDATE_ALL == mode )
		{
			FailIf (!mPlatformInterface->writeCodecRegister( kCS84xx_I2C_ADDRESS, regAddr, &registerData, 1, kI2C_StandardSubMode ), Exit );
		}
	}
	result = kIOReturnSuccess;
	
Exit:
	if ( kIOReturnSuccess != result && !mRecoveryInProcess) {
		mAudioDeviceProvider->interruptEventHandler ( kRequestCodecRecoveryStatus, (UInt32)kControlBusFatalErrorRecovery );
		debug5IOLog ("AppleTopazAudio::CODEC_WriteRegister ( regAddr %X, registerData %X, mode %d ) result = %X\n", regAddr, registerData, mode, result );
	}
#ifdef kVERBOSE_LOG	
	debug5IOLog("- AppleTopazAudio::CODEC_WriteRegister ( regAddr %X registerData %X mode %X ) returns %d\n\n",regAddr,  registerData,  mode, result);
	IOSleep ( 500 );
#endif
    return result;
}

