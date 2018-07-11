/*
 *	PlatformInterface.cpp
 *
 *	Interface class for IO controllers
 *
 *  Created by Aram Lindahl on Mon Mar 10 2003.
 *  Copyright (c) 2003 AppleComputer. All rights reserved.
 */

#include "PlatformInterface.h"
#include <IOKit/IOWorkLoop.h>
#include <IOKit/IOCommandGate.h>

#include "AudioHardwareConstants.h"
#include "AudioHardwareUtilities.h"
#include "AudioHardwareCommon.h"
#include "AppleOnboardAudio.h"

#define super OSObject

class AppleOnboardAudio;

OSDefineMetaClassAndStructors(PlatformInterface, OSObject);

//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool PlatformInterface::init (IOService* device, AppleOnboardAudio* provider, UInt32 inDBDMADeviceIndex) {
	Boolean result;

	result = super::init ();
	FailIf ( !result, Exit );
	
	FailIf ( NULL == provider, Exit );
	mProvider = provider;
//	IOSleep (500);
//	IOLog ("AppleOnboardAudio provider = %p\n", mProvider);
//	IOSleep (1000);
	
Exit:
	return result;
}

//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void	PlatformInterface::free ( void ) {
	
	debugIOLog ( "+ PlatformInterface::free\n" );

	if ( kGPIO_Unknown != getHeadphoneConnected() ) {
		unregisterInterruptHandler ( NULL, NULL, kHeadphoneDetectInterrupt );
	}
	
	if ( kGPIO_Unknown != getSpeakerConnected() ) {
		unregisterInterruptHandler ( NULL, NULL, kSpeakerDetectInterrupt );
	}
	
	if ( kGPIO_Unknown != getLineInConnected() ) {
		unregisterInterruptHandler ( NULL, NULL, kLineInputDetectInterrupt );
	}
	
	if ( kGPIO_Unknown != getLineOutConnected() ) {
		unregisterInterruptHandler ( NULL, NULL, kLineOutputDetectInterrupt );
	}
	
	if ( kGPIO_Unknown != getDigitalInConnected() && kGPIO_Unknown == getDigitalInTypeConnected() ) {
		unregisterInterruptHandler ( NULL, NULL, kDigitalInDetectInterrupt );
	}
	
	if ( kGPIO_Unknown != getDigitalOutConnected() && kGPIO_Unknown == getDigitalOutTypeConnected() ) {
		unregisterInterruptHandler ( NULL, NULL, kDigitalOutDetectInterrupt );
	}
	
	if ( kGPIO_Unknown != getCodecInterrupt() ) {
		unregisterInterruptHandler ( NULL, NULL, kCodecInterrupt );
	}
	
	if ( kGPIO_Unknown != getCodecErrorInterrupt() ) {
		unregisterInterruptHandler ( NULL, NULL, kCodecErrorDetectInterrupt );
	}

	return super::free ();
}

//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool	PlatformInterface::registerInterrupts ( IOService * device ) {
	IOReturn	err;
	bool		result = false;
	
	debug2IOLog ( "+ PlatformInterface::registerInterrupts ( %p )\n", device );

	FailIf (NULL == device, Exit );

	if ( kGPIO_Unknown != getHeadphoneConnected() ) {
		//	IMPORTANT:	The headphone connector may also be a digital output connector.  If a 
		//				digital out type is available then the headphone is a combo connector
		//				supporting both the headphone and digital output connector.  In this 
		//				case, there will be no low level registration of a digital output
		//				interrupt at the derived class and the headphone handler will query
		//				the detect type on insert to determine if the event is associated with
		//				the headphone or the digital output.
		if ( kGPIO_Unknown != getDigitalOutTypeConnected() ) {
			mIsComboOutJack = true;
		}
		err = registerInterruptHandler ( device, (void*)headphoneDetectInterruptHandler, kHeadphoneDetectInterrupt );
		FailIf ( kIOReturnSuccess != err, Exit );
	}
	else
	{
		debugIOLog("PlatformInterface::registerInterrupts kGPIO_Unknown == getHeadphoneConnected()\n");
	}
	
	if ( kGPIO_Unknown != getSpeakerConnected() ) {
		err = registerInterruptHandler ( device, (void*)speakerDetectInterruptHandler, kSpeakerDetectInterrupt );
		FailIf ( kIOReturnSuccess != err, Exit );
	}
	else
	{
		debugIOLog("PlatformInterface::registerInterrupts kGPIO_Unknown == getSpeakerConnected()\n");
	}
	
	if ( kGPIO_Unknown != getLineInConnected() ) {
		//	IMPORTANT:	The line input connector may also be a digital input connector.  If a 
		//				digital out type is available then the line input is a combo connector
		//				supporting both the line input and digital input connector.  In this 
		//				case, there will be no low level registration of a digital input
		//				interrupt at the derived class and the line input handler will query
		//				the detect type on insert to determine if the event is associated with
		//				the line input or the digital input.
		if ( kGPIO_Unknown != getDigitalInTypeConnected() ) {
			mIsComboInJack = true;
		}
		err = registerInterruptHandler ( device, (void*)lineInDetectInterruptHandler, kLineInputDetectInterrupt );
		FailIf ( kIOReturnSuccess != err, Exit );
	}
	else
	{
		debugIOLog("PlatformInterface::registerInterrupts kGPIO_Unknown == getLineInConnected()\n");
	}
	
	if ( kGPIO_Unknown != getLineOutConnected() ) {
		err = registerInterruptHandler ( device, (void*)lineOutDetectInterruptHandler, kLineOutputDetectInterrupt );
		FailIf ( kIOReturnSuccess != err, Exit );
	}
	else
	{
		debugIOLog("PlatformInterface::registerInterrupts kGPIO_Unknown == getLineOutConnected()\n");
	}
	
	if ( kGPIO_Unknown != getDigitalInConnected() && kGPIO_Unknown == getDigitalInTypeConnected() ) {
		//	IMPORTANT:	Only supported if the digital line input connector is not a combo connector
		//				that is already being supported by the line input handler.
		err = registerInterruptHandler ( device, (void*)digitalInDetectInterruptHandler, kDigitalInDetectInterrupt );
		FailIf ( kIOReturnSuccess != err, Exit );
	}
	else
	{
		debugIOLog("PlatformInterface::registerInterrupts kGPIO_Unknown == getDigitalInConnected()\n");
	}
	
	if ( kGPIO_Unknown != getDigitalOutConnected() && kGPIO_Unknown == getDigitalOutTypeConnected() ) {
		//	IMPORTANT:	Only supported if the digital line output connector is not a combo connector
		//				that is already being supported by the headphone handler.
		err = registerInterruptHandler ( device, (void*)digitalOutDetectInterruptHandler, kDigitalOutDetectInterrupt );
		FailIf ( kIOReturnSuccess != err, Exit );
	}
	else
	{
		debugIOLog("PlatformInterface::registerInterrupts kGPIO_Unknown == getDigitalOutConnected()\n");
	}

#if 0	
	if ( kGPIO_Unknown != getCodecInterrupt() ) {
		err = registerInterruptHandler ( device, (void*)codecInterruptHandler, kCodecInterrupt );
		FailIf ( kIOReturnSuccess != err, Exit );
	}
	else
	{
		debugIOLog("PlatformInterface::registerInterrupts kGPIO_Unknown == getCodecInterrupt()\n");
	}
	
	if ( kGPIO_Unknown != getCodecErrorInterrupt() ) {
		err = registerInterruptHandler ( device, (void*)codecErrorInterruptHandler, kCodecErrorDetectInterrupt );
		FailIf ( kIOReturnSuccess != err, Exit );
	}
	else
	{
		debugIOLog("PlatformInterface::registerInterrupts kGPIO_Unknown == getCodecErrorInterrupt()\n");
	}
#endif

	result = true;
Exit:
	debug2IOLog ( "- PlatformInterface::registerInterrupts ( %p )\n", device );
	return result;
}

//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void PlatformInterface::headphoneDetectInterruptHandler ( OSObject *owner, IOInterruptEventSource *source, UInt32 count, void * arg4 ) {
	PlatformInterface * 	platformInterface;
	IOCommandGate *		cg;
	
	platformInterface = (PlatformInterface*)owner;
	FailIf (NULL == platformInterface, Exit);
	FailIf ( NULL == platformInterface->mProvider, Exit );
	cg = platformInterface->mProvider->getCommandGate ();
	if ( NULL != cg ) {
		if ( !platformInterface->mIsComboOutJack ) {
			cg->runAction ( platformInterface->mProvider->sInterruptEventAction, (void *)kHeadphoneStatus, (void *)platformInterface->getHeadphoneConnected (), (void *)0 );
		} else {
			if ( kGPIO_Connected == platformInterface->getDigitalOutTypeConnected() ) {
				cg->runAction ( platformInterface->mProvider->sInterruptEventAction, (void *)kDigitalOutStatus, (void *)platformInterface->getHeadphoneConnected (), (void *)0 );
			} else {
				cg->runAction ( platformInterface->mProvider->sInterruptEventAction, (void *)kHeadphoneStatus, (void *)platformInterface->getHeadphoneConnected (), (void *)0 );
			}
		}
	}
Exit:
	return;
}


//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void PlatformInterface::speakerDetectInterruptHandler ( OSObject *owner, IOInterruptEventSource *source, UInt32 count, void * arg4 ) {
	PlatformInterface * 	platformInterface;
	IOCommandGate *		cg;
	
	platformInterface = (PlatformInterface*)owner;
	FailIf (NULL == platformInterface, Exit);
	FailIf ( NULL == platformInterface->mProvider, Exit );
	cg = platformInterface->mProvider->getCommandGate ();
	if ( NULL != cg ) {
		cg->runAction ( platformInterface->mProvider->sInterruptEventAction, (void *)kExtSpeakersStatus, (void *)platformInterface->getSpeakerConnected (), (void *)0 );
	}
	
Exit:
	return;
}


//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void PlatformInterface::lineInDetectInterruptHandler ( OSObject *owner, IOInterruptEventSource *source, UInt32 count, void * arg4 ) {
	PlatformInterface * 	platformInterface;
	IOCommandGate *		cg;
	
	platformInterface = (PlatformInterface*)owner;
	FailIf (NULL == platformInterface, Exit);
	FailIf ( NULL == platformInterface->mProvider, Exit );
	cg = platformInterface->mProvider->getCommandGate ();
	if ( NULL != cg ) {
		if ( !platformInterface->mIsComboInJack ) {
			cg->runAction ( platformInterface->mProvider->sInterruptEventAction, (void *)kLineInStatus, (void *)platformInterface->getLineInConnected (), (void *)0 );
		} else {
			if ( kGPIO_Connected == platformInterface->getDigitalInTypeConnected() ) {
				cg->runAction ( platformInterface->mProvider->sInterruptEventAction, (void *)kDigitalInStatus, (void *)platformInterface->getLineInConnected (), (void *)0 );
			} else {
				cg->runAction ( platformInterface->mProvider->sInterruptEventAction, (void *)kLineInStatus, (void *)platformInterface->getLineInConnected (), (void *)0 );
			}
		}
	}
Exit:
	return;
}

//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void PlatformInterface::lineOutDetectInterruptHandler ( OSObject *owner, IOInterruptEventSource *source, UInt32 count, void * arg4 ) {
	PlatformInterface * 	platformInterface;
	IOCommandGate *		cg;
	
	platformInterface = (PlatformInterface*)owner;
	FailIf (NULL == platformInterface, Exit);
	FailIf ( NULL == platformInterface->mProvider, Exit );
	cg = platformInterface->mProvider->getCommandGate ();
	if ( NULL != cg ) {
		cg->runAction ( platformInterface->mProvider->sInterruptEventAction, (void *)kLineOutStatus, (void *)platformInterface->getLineOutConnected (), (void *)0 );
	}
Exit:
	return;
}


//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void PlatformInterface::digitalInDetectInterruptHandler ( OSObject *owner, IOInterruptEventSource *source, UInt32 count, void * arg4 ) {
	PlatformInterface * 	platformInterface;
	IOCommandGate *		cg;
	
	platformInterface = (PlatformInterface*)owner;
	FailIf (NULL == platformInterface, Exit);
	FailIf ( NULL == platformInterface->mProvider, Exit );
	cg = platformInterface->mProvider->getCommandGate ();
	if ( NULL != cg ) {
		cg->runAction ( platformInterface->mProvider->sInterruptEventAction, (void *)kDigitalInStatus, (void *)platformInterface->getDigitalInConnected (), (void *)0 );
	}
Exit:
	return;
}


//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void PlatformInterface::digitalOutDetectInterruptHandler ( OSObject *owner, IOInterruptEventSource *source, UInt32 count, void * arg4 ) {
	PlatformInterface * 	platformInterface;
	IOCommandGate *		cg;
	
	platformInterface = (PlatformInterface*)owner;
	FailIf (NULL == platformInterface, Exit);
	FailIf ( NULL == platformInterface->mProvider, Exit );
	cg = platformInterface->mProvider->getCommandGate ();
	if ( NULL != cg ) {
		cg->runAction ( platformInterface->mProvider->sInterruptEventAction, (void *)kDigitalOutStatus, (void *)platformInterface->getDigitalOutConnected (), (void *)0 );
	}
Exit:
	return;
}


//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void PlatformInterface::codecInterruptHandler ( OSObject *owner, IOInterruptEventSource *source, UInt32 count, void * arg4 ) {
	PlatformInterface * 	platformInterface;
	IOCommandGate *		cg;

	platformInterface = (PlatformInterface*)owner;
	FailIf (NULL == platformInterface, Exit);
	FailIf ( NULL == platformInterface->mProvider, Exit );
	cg = platformInterface->mProvider->getCommandGate ();
	if ( NULL != cg ) {
		cg->runAction ( platformInterface->mProvider->sInterruptEventAction, (void *)kCodecInterruptStatus, (void *)platformInterface->getCodecInterrupt (), (void *)0 );
	}
Exit:
	return;
}


//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void PlatformInterface::codecErrorInterruptHandler ( OSObject *owner, IOInterruptEventSource *source, UInt32 count, void * arg4 ) {
	PlatformInterface * 	platformInterface;
	IOCommandGate *		cg;
	
	platformInterface = (PlatformInterface*)owner;
	FailIf (NULL == platformInterface, Exit);
	FailIf ( NULL == platformInterface->mProvider, Exit );
	cg = platformInterface->mProvider->getCommandGate ();
	if ( NULL != cg ) {
		cg->runAction ( platformInterface->mProvider->sInterruptEventAction, (void *)kCodecErrorInterruptStatus, (void *)platformInterface->getCodecErrorInterrupt (), (void *)0 );
	}
Exit:
	return;
}
