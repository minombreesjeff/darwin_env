/*
 *	TransportInterface.cpp
 *
 *	Interface class for audio data transport
 *
 *  Created by Ray Montagne on Mon Mar 12 2003.
 *  Copyright (c) 2003 AppleComputer. All rights reserved.
 */

#include "TransportInterface.h"

#define super OSObject

OSDefineMetaClassAndAbstractStructors ( TransportInterface, OSObject );

//	--------------------------------------------------------------------------------
bool TransportInterface::init ( PlatformInterface* inPlatformInterface ) {
	bool	result;
	
	result = super::init ();

	if ( result ) {
		mPlatformObject = inPlatformInterface;
		result = NULL == mPlatformObject ? false : true ;
	}

	return result;
}

//	--------------------------------------------------------------------------------
//	Must be called from the inherited class after successfully setting the
//	audio data transport to the target sample rate.  Should not be called
//	from the inherited class if the target sample rate cannot be set on the
//	audio data transport.
IOReturn	TransportInterface::transportSetSampleRate ( UInt32 sampleRate ) {
	mTransportSampleRate = sampleRate;
	return kIOReturnSuccess;
}

//	--------------------------------------------------------------------------------
//	Must be called from the inherited class after successfully setting the
//	audio data transport to the target sample width.  Should not be called
//	from the inherited class if the target sample width cannot be set on the
//	audio data transport.
IOReturn	TransportInterface::transportSetSampleWidth ( UInt32 sampleDepth, UInt32 dmaWidth ) {
	mTransportSampleDepth = sampleDepth;
	mTransportDMAWidth = dmaWidth;
	return kIOReturnSuccess;
}

//	--------------------------------------------------------------------------------
//	Must be called from the inherited class after successfully setting the
//	audio data transport to the target sample width.  Should not be called
//	from the inherited class if the target sample width cannot be set on the
//	audio data transport.
IOReturn	TransportInterface::transportBreakClockSelect ( UInt32 clockSource ) {
	return kIOReturnSuccess;
}

//	--------------------------------------------------------------------------------
//	Must be called from the inherited class after successfully setting the
//	audio data transport to the target sample width.  Should not be called
//	from the inherited class if the target sample width cannot be set on the
//	audio data transport.
IOReturn	TransportInterface::transportMakeClockSelect ( UInt32 clockSource ) {
	mClockSource = clockSource;
	return kIOReturnSuccess;
}

