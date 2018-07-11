/*
 *	TransportInterface.h
 *
 *	Interface class for audio data transport
 *
 *  Created by Ray Montagne on Mon Mar 12 2003.
 *  Copyright (c) 2003 AppleComputer. All rights reserved.
 *
 *	
 *
 */

#include <IOKit/IOService.h>
#include "PlatformInterface.h"

#ifndef	__TRANSPORT_INTERFACE
#define	__TRANSPORT_INTERFACE

enum TRANSPORT_CLOCK_SELECTOR {
	kTRANSPORT_MASTER_CLOCK = 0,
	kTRANSPORT_SLAVE_CLOCK
};

class TransportInterface : public OSObject {

    OSDeclareAbstractStructors ( TransportInterface );

public:

	virtual bool			init (PlatformInterface * inPlatformInterface);

	virtual IOReturn		transportSetSampleRate ( UInt32 sampleRate );
	virtual IOReturn		transportSetSampleWidth ( UInt32 sampleDepth, UInt32 dmaWidth );
	virtual IOReturn		transportBreakClockSelect ( UInt32 clockSource );
	virtual	IOReturn		transportMakeClockSelect ( UInt32 clockSource );
	
	virtual IOReturn		performTransportSleep ( void ) = 0;
	virtual IOReturn		performTransportWake ( void ) = 0;

	virtual bool			transportCanClockSelect ( UInt32 clockSource ) {return false;}
	
	UInt32					transportGetSampleRate ( void ) { return mTransportSampleRate; }
	UInt32					transportGetSampleWidth ( void ) { return mTransportSampleDepth; }
	UInt32					transportGetDMAWidth ( void ) { return mTransportDMAWidth; }
	UInt32					transportGetClockSelect ( void ) { return mClockSource; }	
	
protected:
	PlatformInterface *		mPlatformObject;
	UInt32					mTransportSampleRate;
	UInt32					mTransportSampleDepth;
	UInt32					mTransportDMAWidth;
	UInt32					mClockSource;
};

#endif

