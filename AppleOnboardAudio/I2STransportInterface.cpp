/*
 *	I2STransportInterface.cpp
 *
 *	Interface class for audio data transport
 *
 *  Created by Ray Montagne on Mon Mar 12 2003.
 *  Copyright (c) 2003 AppleComputer. All rights reserved.
 *
 *	This instance of a TransportInterface object supports the I2S I/O module
 *	that is available in KeyLargo, Pangea and the K2 system I/O controllers.
 *
 */
#include "I2STransportInterface.h"
#include <IOKit/IOMessage.h>
#include <IOKit/pwr_mgt/RootDomain.h>
#include <IOKit/IOTimerEventSource.h>
#include "AudioHardwareUtilities.h"
#include "PlatformInterface.h"

#define super TransportInterface

OSDefineMetaClassAndStructors ( I2STransportInterface, TransportInterface );

//	--------------------------------------------------------------------------------
bool		I2STransportInterface::init ( PlatformInterface * inPlatformInterface ) {
	bool			result;
	IOReturn		error = kIOReturnError;
	
	debug2IOLog ( "+ I2STransportInterface::init ( %d )\n", (unsigned int)inPlatformInterface );
	
	result = super::init ( inPlatformInterface );
	FailIf ( !result, Exit );	
	FailIf ( NULL == mPlatformObject, Exit );
	
	//	SET UP default operating variables.  There are codec dependencies on how the clock 
	//	divide ratios are determined for the I2S audio data transport bus.  THIS WILL NEED 
	//	TO ASK AOA WHAT CODEC IS ATTACHED AT SOME POINT IN THE FUTURE (i.e. when the PCM3052 
	//	controller is used).  FOR NOW, AN ASSUMPTION THAT ALL CODECS REQUIRE THE SAME BEHAVIOR IS MADE.
	
	mMclkMultiplier = 256;		//	TAS3004, CS84xx run MCLK at 256 * fs
	mSclkMultiplier =  64;		//	TAS3004, CS84xx run MCLK at 64 * fs
	
	//	Setup for a 48.000 Khz sample rate with stereo 16 bit channels on both input and output.
	mDataWordSize = ( kDataIn16 << kDataInSizeShift ) | ( kDataOut16 << kDataOutSizeShift );
	mDataWordSize |=( kI2sStereoChannels << kNumChannelsOutShift ) | ( kI2sStereoChannels << kNumChannelsInShift );
	error = mPlatformObject->setDataWordSizes ( mDataWordSize );
	FailIf ( kIOReturnSuccess != error, Exit );
	
	error = mPlatformObject->setI2SCellEnable ( true );
	FailIf ( kIOReturnSuccess != error, Exit );
	
	error = mPlatformObject->setI2SEnable ( true );
	FailIf ( kIOReturnSuccess != error, Exit );
	
	error = transportSetSampleRate ( 44100 );
	FailIf ( kIOReturnSuccess != error, Exit );
	
Exit:
	if ( result ) { result = kIOReturnSuccess == error; }
	
	debug3IOLog ( "- I2STransportInterface::init ( %d ) = %d\n", (unsigned int)inPlatformInterface, (unsigned int)result );
	return result;
}

//	--------------------------------------------------------------------------------
//	Maps the sample rate request to register values and register sequences
IOReturn	I2STransportInterface::transportSetSampleRate ( UInt32 sampleRate ) {
	IOReturn	result = kIOReturnError;
	
	debug2IOLog ( "+ I2STransportInterface::transportSetSampleRate ( %d )\n", (unsigned int)sampleRate );
	
	FailIf ( NULL == mPlatformObject, Exit );
	
	mMClkFrequency = sampleRate * mMclkMultiplier;
	mSClkFrequency = sampleRate * mSclkMultiplier;
	
	//	The sample rate can be derived from 45.1584 MHz, 49.1520 MHz or 18.432 Mhz.
	//	The clock that the sample rate is derived from must be an integral multiple
	//	of the sample rate.  Some common sample rates are setup as follows:
	//
	//	SAMPLE RATE		CLOCK SOURCE	MCLK DIVISOR	SCLK DIVISOR	CODEC		MCLK RATIO
	//
	//	 32.000 Khz		49.1520 Mhz			6				4			TAS3004		256 * fs
	//	 44.100 Khz		45.1584 Mhz			4				4			TAS3004		256 * fs
	//	 48.000 Khz		49.1520 Mhz			4				4			TAS3004		256 * fs
	//	 96.000 Khz		49.1520 Mhz			2				4			PCM3052		256 * fs
	//	192.000 Khz		49.1520 Mhz			1				4			PCM3052		256 * fs
	//
	if ( 0 == ( kClock18MHz % mMClkFrequency ) ) {
		mClockSourceFrequency = kClock18MHz;
		mClockSelector = kI2S_18MHz;
		mSerialFormat = ( kClockSource18MHz << kClockSourceShift );
	} else if ( 0 == ( kClock45MHz % mMClkFrequency ) ) {
		mClockSourceFrequency = kClock45MHz;
		mClockSelector = kI2S_45MHz;
		mSerialFormat = ( kClockSource45MHz << kClockSourceShift );
	} else if ( 0 == ( kClock49MHz % mMClkFrequency ) ) {
		mClockSourceFrequency = kClock49MHz;
		mClockSelector = kI2S_49MHz;
		mSerialFormat = ( kClockSource49MHz << kClockSourceShift );
	} else {
		//	The sample rate cannot be derived from available clock sources
		FailIf ( true, Exit );
	}
	//	The BCLkMaster bit is set according to the clock source (i.e. system mastered
	//	clock or externally mastered clock such as a recovered clock from a S/PDIF AES3 stream.
	if ( kTRANSPORT_MASTER_CLOCK == mClockSource ) {
		mSerialFormat |= ( kSClkMaster << kBClkMasterShift );
	} else if ( kTRANSPORT_SLAVE_CLOCK == mClockSource ) {
		mSerialFormat |= ( kSClkSlave << kBClkMasterShift );
	} else {
		FailIf ( true, Exit );
	}
	//	The I2S I/O Module has exceptions for register configurations based on the clock 
	//	divisor which must be resolved to determine register confuration MCLK values.
	mMClkDivisor = mClockSourceFrequency / mMClkFrequency;
	switch ( mMClkDivisor ) {
		case 1:			mMClkDivider = 0x00000014;									break;
		case 3:			mMClkDivider = 0x00000013;									break;
		case 5:			mMClkDivider = 0x00000012;									break;
		default:		mMClkDivider = ( ( mMClkDivisor / 2 ) - 1 );				break;
	}
	mSerialFormat |= ( mMClkDivider << kMClkDivisorShift );
	//	The I2S I/O Module has exceptions for register configurations based on the clock 
	//	divisor which must be resolved to determine register confuration SCLK values.
	mSClkDivisor = ( mClockSourceFrequency / mMClkDivisor ) / mSClkFrequency;
	switch ( mSClkDivisor ) {
		case 1:			mSClkDivider = 0x00000008;									break;
		case 3:			mSClkDivider = 0x00000009;									break;
		default:		mSClkDivider = ( ( mSClkDivisor / 2 ) - 1 );				break;
	}
	mSerialFormat |= ( mSClkDivider << kSClkDivisorShift );
	switch ( mSclkMultiplier ) {
		case 32:	mSerialFormat |= ( kSerialFormat32x << kSerialFormatShift );	break;
		case 64:	mSerialFormat |= ( kSerialFormat64x << kSerialFormatShift );	break;
		default:	FailIf ( true, Exit );											break;
	}
	result = mPlatformObject->setI2SClockEnable ( false );
	FailIf ( kIOReturnSuccess != result, Exit );
	
	result = mPlatformObject->setI2SEnable ( false );
	FailIf ( kIOReturnSuccess != result, Exit );
	
	waitForClocksStopped ();
	
	result = mPlatformObject->setSerialFormatRegister ( mSerialFormat );
	FailIf ( kIOReturnSuccess != result, Exit );
	
	result = mPlatformObject->setI2SEnable ( true );
	FailIf ( kIOReturnSuccess != result, Exit );
	
	result = mPlatformObject->setI2SClockEnable ( true );
	FailIf ( kIOReturnSuccess != result, Exit );
	
	result = super::transportSetSampleRate ( sampleRate );
	
	debug2IOLog ( "I2S Serial Format Register %lX\n", mPlatformObject->getSerialFormatRegister() );
	debug2IOLog ( "I2S Data Word Sizes        %lX\n", mPlatformObject->getDataWordSizes() );
	debug2IOLog ( "I2S Frame Count            %lX\n", mPlatformObject->getFrameCount() );

Exit:	
	debug4IOLog ( "- I2STransportInterface::transportSetSampleRate ( %d ) = %d, mSerialFormat = %X\n", (unsigned int)sampleRate, (unsigned int)result, (unsigned int)mSerialFormat );
	return result;
}

//	--------------------------------------------------------------------------------
//	Sets the data word sizes to configure the data transfer sizes between the
//	DMA engine and the I2S bus.  SampleWidth indicates the width of the sample
//	being conveyed on the I2S audio transport bus while DMA width indicates the
//	width of data being transacted between the I2S I/O Module and the DMA engine.
//
//	Sequence:	1.	Stop I2S Clocks
//				2.	Wait for I2S clocks stopped
//				3.	Set data word sizes
//				4.	Enable I2S Clocks
//
IOReturn	I2STransportInterface::transportSetSampleWidth ( UInt32 sampleDepth, UInt32 dmaWidth ) {
	IOReturn	result = kIOReturnError;
	
	debug3IOLog ( "+ I2STransportInterface::transportSetSampleWidth ( %d, %d )\n", (unsigned int)sampleDepth, (unsigned int)dmaWidth );

	FailIf ( NULL == mPlatformObject, Exit );
	if ( 16 == sampleDepth && 16 == dmaWidth ) {
		mDataWordSize = ( kDataIn16 << kDataInSizeShift ) | ( kDataOut16 << kDataOutSizeShift );
	} else if ( 24 == sampleDepth && 32 == dmaWidth ) {
		mDataWordSize = ( kDataIn24 << kDataInSizeShift ) | ( kDataOut24 << kDataOutSizeShift );
	} else {
		FailIf ( true, Exit );
	}
	mDataWordSize |= ( ( kI2sStereoChannels << kNumChannelsOutShift ) | ( kI2sStereoChannels << kNumChannelsInShift ) );

	result = mPlatformObject->setI2SClockEnable ( false );
	FailIf ( kIOReturnSuccess != result, Exit );
	
	result = mPlatformObject->setI2SEnable ( false );
	FailIf ( kIOReturnSuccess != result, Exit );
	
	waitForClocksStopped ();
	
	result = mPlatformObject->setDataWordSizes ( mDataWordSize );
	FailIf ( kIOReturnSuccess != result, Exit );
	
	result = mPlatformObject->setI2SEnable ( true );
	FailIf ( kIOReturnSuccess != result, Exit );
	
	result = mPlatformObject->setI2SClockEnable ( true );
	FailIf ( kIOReturnSuccess != result, Exit );

	result = super::transportSetSampleWidth ( sampleDepth, dmaWidth );

Exit:	
	debug4IOLog ( "- I2STransportInterface::transportSetSampleWidth ( %d, %d ) = %d\n", (unsigned int)sampleDepth, (unsigned int)dmaWidth, (unsigned int)result );
	return result;
}

//	--------------------------------------------------------------------------------
//	Sets the I2S module to sleep state by stopping the clocks, disabling the I2S
//	I/O Module and releasing the clock source.
//
//	Sequence:	1.	Stop I2S Clocks
//				2.	Wait for I2S clocks stopped
//				3.	Disable the I2S I/O Module
//				4.	Release the source clock
//
IOReturn	I2STransportInterface::performTransportSleep ( void ) {
	IOReturn			result = kIOReturnError;

	debugIOLog ( "+ I2STransportInterface::performTransportSleep ()\n" );
	
	FailIf ( NULL == mPlatformObject, Exit );
	
	result = mPlatformObject->setI2SClockEnable ( false );
	FailIf ( kIOReturnSuccess != result, Exit );
	waitForClocksStopped ();
	
	result = mPlatformObject->setI2SCellEnable ( false );		
	FailIf ( kIOReturnSuccess != result, Exit );
	
	result = mPlatformObject->releaseI2SClockSource ( mClockSelector );
Exit:	
	debug2IOLog ( "- I2STransportInterface::performTransportSleep () = %d\n", (unsigned int)result );
	return result;
}

//	--------------------------------------------------------------------------------
//	Sets the I2S module to the active state by stopping the clocks, disabling the
//	I2S I/O Module and releasing the clock source.
//
//	Sequence:	1.	Request a clock source
//				2.	Enable the I2S I/O Module
//				3.	Enable the I2S I/O Module Clocks
//
IOReturn	I2STransportInterface::performTransportWake ( void ) {
	IOReturn			result = kIOReturnError;
	
	debugIOLog ( "+ I2STransportInterface::performTransportWake ()\n" );
	
	FailIf ( NULL == mPlatformObject, Exit );

	result = mPlatformObject->setI2SCellEnable ( true );		
	FailIf ( kIOReturnSuccess != result, Exit );
		
	result = mPlatformObject->requestI2SClockSource ( mClockSelector );
	FailIf ( kIOReturnSuccess != result, Exit );
	
	result = mPlatformObject->setI2SClockEnable ( true );		
	FailIf ( kIOReturnSuccess != result, Exit );
Exit:
	debug2IOLog ( "- I2STransportInterface::performTransportWake () = %d\n", (unsigned int)result );
	return result;
}

//	--------------------------------------------------------------------------------
//	Switching bewteen a system mastered clock and an external clock, such as a
//	recovered clock from an S/PDIF AES3 stream, requires a "BREAK BEFORE MAKE"
//	sequence to avoid having two hardware drivers connected together.  If selecting 
//	the master clock then the external MUX must be disconnected prior to enabling 
//	the system master clock.  If selecting an external MUX clock source then the 
//	internal system master clock must be disconnected first.  Sequences are:
//
//	MASTER								SLAVE
//
//	1a.	Stop I2S Clocks					1b.	Stop I2S Clocks
//	2a.	Wait for I2S clocks stopped		2b.	Wait for I2S clocks stopped
//	3a.	Deselect External Mux			3b.	Set BClkMaster to SLAVE
//
//	If not external MUX is available the steps 3a and 4b perform no function.
//
IOReturn	I2STransportInterface::transportBreakClockSelect ( UInt32 clockSource ) {
	IOReturn	result = kIOReturnError;
	
	debug2IOLog ( "+ I2STransportInterface::transportBreakClockSelect ( %d )\n", (unsigned int)clockSource );
	
	FailIf ( NULL == mPlatformObject, Exit );
	
	result = mPlatformObject->setI2SClockEnable ( false );
	FailIf ( kIOReturnSuccess != result, Exit );
	waitForClocksStopped ();

	switch ( clockSource ) {
		case kTRANSPORT_MASTER_CLOCK:
			result = mPlatformObject->setClockMux ( kGPIO_MuxSelectDefault );
			FailIf ( kIOReturnSuccess != result, Exit );
			break;
		case kTRANSPORT_SLAVE_CLOCK:
			mSerialFormat &= ~( 1 << kBClkMasterShift );
			mSerialFormat |= ( kSClkSlave << kBClkMasterShift );
			result = mPlatformObject->setSerialFormatRegister ( mSerialFormat );
			FailIf ( kIOReturnSuccess != result, Exit );
			break;
		default:
			FailIf ( true, Exit );
			break;
	}
	
	super::transportBreakClockSelect ( clockSource );
Exit:
	
	debug3IOLog ( "- I2STransportInterface::transportBreakClockSelect ( %d ) = %d\n", (unsigned int)clockSource, (unsigned int)result );
	return result;
}

//	--------------------------------------------------------------------------------
//	Switching bewteen a system mastered clock and an external clock, such as a
//	recovered clock from an S/PDIF AES3 stream, requires a "BREAK BEFORE MAKE"
//	sequence to avoid having two hardware drivers connected together.  If selecting 
//	the master clock then the external MUX must be disconnected prior to enabling 
//	the system master clock.  If selecting an external MUX clock source then the 
//	internal system master clock must be disconnected first.  Sequences are:
//
//	MASTER								SLAVE
//
//	4a.	Set BClkMaster to Master		4b.	Select External Mux
//	5a.	Enable I2S Clocks				5b.	Enable I2S Clocks
//
//	If not external MUX is available the steps 3a and 4b perform no function.
//
IOReturn	I2STransportInterface::transportMakeClockSelect ( UInt32 clockSource ) {
	IOReturn	result = kIOReturnError;
	
	debug2IOLog ( "+ I2STransportInterface::transportMakeClockSelect ( %d )\n", (unsigned int)clockSource );
	
	FailIf ( NULL == mPlatformObject, Exit );
	
	switch ( clockSource ) {
		case kTRANSPORT_MASTER_CLOCK:
			mSerialFormat &= ~( 1 << kBClkMasterShift );
			mSerialFormat |= ( kSClkMaster << kBClkMasterShift );
			result = mPlatformObject->setSerialFormatRegister ( mSerialFormat );
			FailIf ( kIOReturnSuccess != result, Exit );
			break;
		case kTRANSPORT_SLAVE_CLOCK:
			result = mPlatformObject->setClockMux ( kGPIO_MuxSelectAlternate );
			FailIf ( kIOReturnSuccess != result, Exit );
			break;
		default:
			FailIf ( true, Exit );
			break;
	}
	result = mPlatformObject->setI2SClockEnable ( true );
	FailIf ( kIOReturnSuccess != result, Exit );
	
	super::transportMakeClockSelect ( clockSource );
Exit:
	
	debug3IOLog ( "- I2STransportInterface::transportMakeClockSelect ( %d ) = %d\n", (unsigned int)clockSource, (unsigned int)result );
	return result;
}

//	--------------------------------------------------------------------------------
void		I2STransportInterface::waitForClocksStopped ( void ) {
	UInt32		timeOut = 100;
	
	IOSleep ( 10 );
	while ( ( ( mPlatformObject->getI2SIOMIntControl () & ( 1 << kClocksStoppedPendingShift ) ) == 0 ) && timeOut ) {
		IOSleep ( 10 );
		timeOut--;
	}
}



