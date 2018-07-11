 /*
 *  AppleOnboardAudio.h
 *  AppleOnboardAudio
 *
 *  Created by cerveau on Mon Jun 04 2001.
 *  Copyright (c) 2001 Apple Computer Inc. All rights reserved.
 *
 */

#include <IOKit/IOCommandGate.h>
#include <IOKit/IOMessage.h>
#include <IOKit/pwr_mgt/RootDomain.h>
#include <IOKit/IOTimerEventSource.h>

#include "AppleOnboardAudio.h"
#include "AudioHardwareUtilities.h"
#include "AudioHardwareObjectInterface.h"

#include "PlatformFactory.h"
#include "TransportFactory.h"

OSDefineMetaClassAndStructors(AppleOnboardAudio, IOAudioDevice)

#define super IOAudioDevice

#pragma mark +UNIX LIKE FUNCTIONS

bool AppleOnboardAudio::init(OSDictionary *properties)
{
    debug2IOLog("+ AppleOnboardAudio[%p]::init\n", this);

    if (!super::init(properties)) return false;
        
    currentDevices = 0xFFFF;
    
	mHasHardwareInputGain = true;	// aml 5.10.02
	
	mInternalMicDualMonoMode = e_Mode_Disabled;	// aml 6.17.02, turn off by default
	

    debugIOLog("- AppleOnboardAudio::init\n");
    return true;
}

bool AppleOnboardAudio::start (IOService * provider) {
	OSArray *							layouts;
	OSArray *							hardwareObjectsList;
	OSDictionary *						layoutEntry;
	OSNumber *							layoutIDNumber;
	OSNumber *							ampRecoveryNumber;
	OSString *							platformObjectString;
	OSString *							transportObjectString;
	OSData *							tmpData;
	UInt32 *							layoutID;
	UInt32								layoutIDInt;
	UInt32								timeout;
	UInt32								layoutsCount;
	UInt32								index;
	UInt32								numPlugins;
	bool								done;
	bool								result;

    debug3IOLog ("+ AppleOnboardAudio[%p]::start (%p)\n", this, provider);
	result = FALSE;

	provider->open(this);

	if (NULL == mPluginObjects) {
		mPluginObjects = OSArray::withCapacity (0);
	}
	FailIf (NULL == mPluginObjects, Exit);

	tmpData = OSDynamicCast (OSData, provider->getProperty (kLayoutID));
	FailIf (NULL == tmpData, Exit);
	layoutID = (UInt32 *)tmpData->getBytesNoCopy ();
	FailIf (NULL == layoutID, Exit)
	mLayoutID = *layoutID;

	// Figure out which plugins need to be loaded for this machine.
	// Fix up the registry to get needed plugins to load using ourselves as a nub.
	layouts = OSDynamicCast (OSArray, getProperty (kLayouts));
	FailIf (NULL == layouts, Exit);

	// First thing to do is to find the array entry that describes the machine that we are on.
	layoutsCount = layouts->getCount ();
	layoutEntry = NULL;
	index = 0;
	while (layoutsCount--) {
		layoutEntry = OSDynamicCast (OSDictionary, layouts->getObject (index));
		FailIf (NULL == layoutEntry, Exit);
		layoutIDNumber = OSDynamicCast (OSNumber, layoutEntry->getObject (kLayoutIDInfoPlist));
		FailIf (NULL == layoutIDNumber, Exit);
		layoutIDInt = layoutIDNumber->unsigned32BitValue ();
		if (layoutIDInt != mLayoutID) {
			layouts->removeObject (index);				// Remove wrong entries from the IORegistry to save space
		} else {
			index++;
		}
	}
	debug2IOLog ( "><><>< mLayoutID %lX\n", mLayoutID);
	debug2IOLog ( "><><>< layoutIDInt %lX\n", layoutIDInt );

	layoutEntry = OSDynamicCast (OSDictionary, layouts->getObject (0));
	FailIf (NULL == layoutEntry, Exit);

	ampRecoveryNumber = OSDynamicCast ( OSNumber, layoutEntry->getObject (kAmpRecoveryTime) );
	FailIf (NULL == ampRecoveryNumber, Exit);
	mAmpRecoveryMuteDuration = ampRecoveryNumber->unsigned32BitValue();
	debug2IOLog ("AppleOnboardAudio::start - mAmpRecoveryMuteDuration = %ld\n", mAmpRecoveryMuteDuration);

	// Find out what the correct platform object is and request the platform factory to build it for us.
	platformObjectString = OSDynamicCast ( OSString, layoutEntry->getObject ( kPlatformObject ) );
	FailIf (NULL == platformObjectString, Exit);
	debug2IOLog ("AppleOnboardAudio::start - platformObjectString = %s\n", platformObjectString->getCStringNoCopy());

	mPlatformInterface = PlatformFactory::createPlatform (platformObjectString);
	FailIf (NULL == mPlatformInterface, Exit);
	debug2IOLog ("AppleOnboardAudio::start - mPlatformInterface = %p\n", mPlatformInterface);
	FailIf (!mPlatformInterface->init(provider, this, AppleDBDMAAudio::kDBDMADeviceIndex), Exit);

	// Find out what the correct transport object is and request the transport factory to build it for us.
	transportObjectString = OSDynamicCast ( OSString, layoutEntry->getObject ( kTransportObject ) );
	FailIf ( NULL == transportObjectString, Exit );
	debug2IOLog ("AppleOnboardAudio::start - transportObjectString = %s\n", transportObjectString->getCStringNoCopy());

	mTransportInterface = TransportFactory::createTransport ( transportObjectString );
	FailIf (NULL == mTransportInterface, Exit);
	debug2IOLog ("AppleOnboardAudio::start - mTransportInterface = %p\n", mTransportInterface);
	FailIf (!mTransportInterface->init ( mPlatformInterface ), Exit);

	// If we have the entry we were looking for then get the list of plugins that need to be loaded
	hardwareObjectsList = OSDynamicCast (OSArray, layoutEntry->getObject (kHardwareObjects));
	if ( NULL == hardwareObjectsList ) { debugIOLog ( "><><>< NULL == hardwareObjectsList\n" ); }
	FailIf (NULL == hardwareObjectsList, Exit);

	// Set the IORegistry entries that will cause the plugins to load
	numPlugins = hardwareObjectsList->getCount ();
	debug2IOLog ( "numPlugins to load = %ld\n", numPlugins);

	for (index = 0; index < numPlugins; index++) {
		setProperty (((OSString *)hardwareObjectsList->getObject(index))->getCStringNoCopy(), "YES");
	}

//	registerService (kIOServiceSynchronous);								// Gets the plugins to load.
	registerService ();														// Gets the plugins to load.

	// Wait for the plugins to load so that when we get to initHardware everything will be up and ready to go.
#if DEBUGLOG
	timeout = 50000;
#else
	timeout = 1000;
#endif
	done = FALSE;
	while (!done && timeout) {
		if (NULL == mPluginObjects) {
			IOSleep (10);
		} else {
			if (mPluginObjects->getCount () != numPlugins) {
				IOSleep (10);
			} else {
				done = TRUE;
			}
		}
		timeout--;
	}
	
	if ((0 == timeout) && (FALSE == done))
		debugIOLog ("$$$$$$ timeout and not enough plugins $$$$$$\n");

    debug3IOLog ("- AppleOnboardAudio[%p]::start (%p)\n", this, provider);

	result = super::start (provider);				// Causes our initHardware routine to be called.

Exit:
	return result;
}

bool AppleOnboardAudio::handleOpen (IOService * forClient, IOOptionBits options, void *	arg ) 
{
	bool							result;

	debug2IOLog ("AppleOnboardAudio::handleOpen(%p)\n", forClient);

	if (kFamilyOption_OpenMultiple == options) {
		result = true;
	} else {
		result = super::handleOpen ( forClient, options, arg );
		FailIf (!result, Exit);
	}

	registerPlugin ( (AudioHardwareObjectInterface *)forClient );

Exit:
	debug3IOLog ("AppleOnboardAudio::handleOpen(%p) returns %s\n", forClient, result == true ? "true" : "false");
	return result;
}

void AppleOnboardAudio::handleClose (IOService * forClient, IOOptionBits options ) 
{
	debug2IOLog ("AppleOnboardAudio::handleClose(%p)\n", forClient);

	unRegisterPlugin ( (AudioHardwareObjectInterface *)forClient );

	if (options != kFamilyOption_OpenMultiple) {
		super::handleClose ( forClient, options );
	}
	
	return;
}

bool AppleOnboardAudio::willTerminate ( IOService * provider, IOOptionBits options )
{
#if 0
	UInt32 count;
	UInt32 index;
	AudioHardwareObjectInterface* thePluginObject;
	
	if (NULL != mPluginObjects) {
		count = mPluginObjects->getCount ();
		for (index = 0; index < count; index++) {
			thePluginObject = getIndexedPluginObject (index);
			if((NULL != thePluginObject)) {
				thePluginObject->terminate();
				mPluginObjects->removeObject(index);
				debug2IOLog ("AppleOnboardAudio::willTerminate terminated  (%p)\n", thePluginObject);
			}
		}
	}
#endif
	
	bool result = super::willTerminate ( provider, options );

	provider->close(this);

	debug3IOLog("AppleOnboardAudio::willTerminate(%p) returns %d\n", provider, result);

	return result;
}

// Called by the plugin objects from their start() method so that AppleOnboardAudio knows about them and will call them as needed.
void AppleOnboardAudio::registerPlugin (AudioHardwareObjectInterface *thePlugin) {
//	IOCommandGate *				cg;

	debug2IOLog ("AppleOnboardAudio::registerPlugin (%p)\n", thePlugin);
/*	cg = getCommandGate ();
	if (NULL != cg) {
		cg->runAction (registerPluginAction, thePlugin);
	} else {
		debugIOLog ("no command gate for registering a plugin!\n");
	}*/
	
	FailIf ( NULL == mPluginObjects, Exit );
	mPluginObjects->setObject (thePlugin);

Exit:
	return;
}

void AppleOnboardAudio::stop (IOService * provider) {

	if (NULL != mSysPowerDownNotifier) {
		mSysPowerDownNotifier->remove();
		mSysPowerDownNotifier = NULL;
	}
	debug3IOLog ("AppleOnboardAudio::stop(), audioEngines = %p, isInactive() = %d\n", audioEngines, isInactive());
	super::stop(provider);
}

void AppleOnboardAudio::unRegisterPlugin (AudioHardwareObjectInterface *inPlugin) {
	AudioHardwareObjectInterface *		thePluginObject;
	UInt32								index;
	UInt32								count;

	debug2IOLog ("AppleOnboardAudio::unRegisterPlugin (%p)\n", inPlugin);

	if (NULL != mPluginObjects) {
		count = mPluginObjects->getCount ();
		for (index = 0; index < count; index++) {
			thePluginObject = getIndexedPluginObject (index);
			if ((NULL != thePluginObject) && (inPlugin == thePluginObject)) {
				mPluginObjects->removeObject(index);
				debug2IOLog ("removed  (%p)\n", inPlugin);
			}
		}
	}

	return;
}

IOReturn AppleOnboardAudio::registerPluginAction (OSObject *owner, void *arg1, void *arg2, void *arg3, void *arg4) {
	AppleOnboardAudio *			device;

	device = OSDynamicCast (AppleOnboardAudio, owner);
	debug2IOLog ("AppleOnboardAudio::plugin %p registering\n", arg1);

	if (NULL == device->mPluginObjects) {
		device->mPluginObjects = OSArray::withCapacity (1);
	}

	FailIf (NULL == device->mPluginObjects, Exit);
	device->mPluginObjects->setObject ((AudioHardwareObjectInterface *)arg1);

Exit:
	return kIOReturnSuccess;
}

OSObject * AppleOnboardAudio::getLayoutEntry (const char * entryID) {
	OSArray *							layouts;
	OSDictionary *						layoutEntry;
	OSObject *							entry;

	entry = NULL;

	layouts = OSDynamicCast (OSArray, getProperty (kLayouts));
	FailIf (NULL == layouts, Exit);

	layoutEntry = OSDynamicCast (OSDictionary, layouts->getObject (0));
	FailIf (NULL == layoutEntry, Exit);

	entry = OSDynamicCast (OSObject, layoutEntry->getObject (entryID));

Exit:
	return entry;
}

bool AppleOnboardAudio::hasMasterVolumeControl (const UInt32 inCode) {
	char *			connectionString;
	
	connectionString = getConnectionKeyFromCharCode (inCode, kIOAudioStreamDirectionOutput);
	return hasMasterVolumeControl (connectionString);
}

bool AppleOnboardAudio::hasMasterVolumeControl (const char * outputEntry) {
	OSDictionary *					dictEntry;
	OSArray *						controlsArray;
	OSString *						controlString;
	UInt32							controlsCount;
	UInt32							index;
	bool							hasMasterVolControl;

	hasMasterVolControl = FALSE;

	dictEntry = OSDynamicCast (OSDictionary, getLayoutEntry (outputEntry));
	FailIf (NULL == dictEntry, Exit);
	controlsArray = OSDynamicCast (OSArray, dictEntry->getObject (kControls));
	FailIf (NULL == controlsArray, Exit);
	controlsCount = controlsArray->getCount ();
	for (index = 0; index < controlsCount; index++) {
		controlString = OSDynamicCast (OSString, controlsArray->getObject (index));
		if ((NULL != controlString) && controlString->isEqualTo (kMasterVolControlString)) {
			hasMasterVolControl = TRUE;
		}
	}

Exit:
	return hasMasterVolControl;
}

bool AppleOnboardAudio::hasLeftVolumeControl (const UInt32 inCode) {
	char *			connectionString;
	
	connectionString = getConnectionKeyFromCharCode (inCode, kIOAudioStreamDirectionOutput);
	return hasLeftVolumeControl (connectionString);
}

bool AppleOnboardAudio::hasLeftVolumeControl (const char * outputEntry) {
	OSDictionary *					dictEntry;
	OSArray *						controlsArray;
	OSString *						controlString;
	UInt32							controlsCount;
	UInt32							index;
	bool							hasLeftVolControl;

	hasLeftVolControl = FALSE;

	dictEntry = OSDynamicCast (OSDictionary, getLayoutEntry (outputEntry));
	FailIf (NULL == dictEntry, Exit);
	controlsArray = OSDynamicCast (OSArray, dictEntry->getObject (kControls));
	FailIf (NULL == controlsArray, Exit);
	controlsCount = controlsArray->getCount ();
	for (index = 0; index < controlsCount; index++) {
		controlString = OSDynamicCast (OSString, controlsArray->getObject (index));
		if ((NULL != controlString) && controlString->isEqualTo (kLeftVolControlString)) {
			hasLeftVolControl = TRUE;
		}
	}

Exit:
	return hasLeftVolControl;
}

bool AppleOnboardAudio::hasRightVolumeControl (const UInt32 inCode) {
	char *			connectionString;
	
	connectionString = getConnectionKeyFromCharCode (inCode, kIOAudioStreamDirectionOutput);
	return hasRightVolumeControl (connectionString);
}

bool AppleOnboardAudio::hasRightVolumeControl (const char * outputEntry) {
	OSDictionary *					dictEntry;
	OSArray *						controlsArray;
	OSString *						controlString;
	UInt32							controlsCount;
	UInt32							index;
	bool							hasRightVolControl;

	hasRightVolControl = FALSE;

	dictEntry = OSDynamicCast (OSDictionary, getLayoutEntry (outputEntry));
	FailIf (NULL == dictEntry, Exit);
	controlsArray = OSDynamicCast (OSArray, dictEntry->getObject (kControls));
	FailIf (NULL == controlsArray, Exit);
	controlsCount = controlsArray->getCount ();
	for (index = 0; index < controlsCount; index++) {
		controlString = OSDynamicCast (OSString, controlsArray->getObject (index));
		if ((NULL != controlString) && controlString->isEqualTo (kRightVolControlString)) {
			hasRightVolControl = TRUE;
		}
	}

Exit:
	return hasRightVolControl;
}

void AppleOnboardAudio::setUseInputGainControls (const char * inputEntry) {
	OSDictionary *					dictEntry;
	OSArray *						controlsArray;
	OSString *						controlString;
	UInt32							controlsCount;
	UInt32							index;

	debug2IOLog("AppleOnboardAudio::setUseInputGainControls(%s)\n", inputEntry);
	mUseInputGainControls = FALSE;

	dictEntry = OSDynamicCast (OSDictionary, getLayoutEntry (inputEntry));
	FailIf (NULL == dictEntry, Exit);

	controlsArray = OSDynamicCast (OSArray, dictEntry->getObject (kControls));
	FailIf (NULL == controlsArray, Exit);

	controlsCount = controlsArray->getCount ();
	for (index = 0; index < controlsCount; index++) {
		controlString = OSDynamicCast (OSString, controlsArray->getObject (index));
		if ((NULL != controlString) && (controlString->isEqualTo (kLeftVolControlString) || controlString->isEqualTo (kRightVolControlString))) {
			mUseInputGainControls = TRUE;
			debugIOLog("mUseInputGainControls = true\n");
		}
	}
Exit:
	return;
}

IOReturn AppleOnboardAudio::formatChangeRequest (const IOAudioStreamFormat * inFormat, const IOAudioSampleRate * inRate) {
	IOReturn							result;
	
	result = kIOReturnError;

	if (NULL != inFormat) {
		debug2IOLog ("AppleOnboardAudio::formatChangeRequest with bit width %d\n", inFormat->fBitWidth);
		result = mTransportInterface->transportSetSampleWidth (inFormat->fBitDepth, inFormat->fBitWidth);
		callPluginsInOrder (kSetSampleBitDepth, inFormat->fBitDepth);
		result = kIOReturnSuccess;
	}
	if (NULL != inRate) {
		debug2IOLog ("AppleOnboardAudio::formatChangeRequest with rate %ld\n", inRate->whole);
		result = mTransportInterface->transportSetSampleRate (inRate->whole);
		callPluginsInOrder (kSetSampleRate, inRate->whole);
	}
	
	return result;
}

IOReturn AppleOnboardAudio::sInterruptEventAction (OSObject * owner, void * arg1, void * arg2, void * arg3, void * arg4) {
	IOReturn 							result;
	AppleOnboardAudio * 				aoa;
	
	result = kIOReturnError;
	
	IOLog ("AppleOnboardAudio::sInterruptEventAction - (%p, %ld, %ld, %ld, %ld)\n", owner, (UInt32)arg1, (UInt32)arg2, (UInt32)arg3, (UInt32)arg4);
	
	aoa = (AppleOnboardAudio *)owner;
	FailIf (NULL == aoa, Exit);
	aoa->interruptEventHandler ((UInt32)arg1, (UInt32)arg2);

	result = kIOReturnSuccess;
Exit:
	return result;
}

void AppleOnboardAudio::interruptEventHandler (UInt32 statusSelector, UInt32 newValue) {
	AudioHardwareObjectInterface *		thePlugin;
	OSNumber * 							connectionCodeNumber;
	char * 								pluginString;
	char * 								connectionString;
	UInt32 								connectionCode;
	UInt32 								selectorCode;

	debug3IOLog ("AppleOnboardAudio::interruptEventHandler (%ld, %ld)\n", statusSelector, newValue);
	selectorCode = getCharCodeForIntCode (statusSelector);
	debug2IOLog ("AppleOnboardAudio::interruptEventHandler - selectorCode = %ld\n", selectorCode);

	switch (statusSelector) {
		case kHeadphoneStatus:
			if (newValue == kInserted) {
				mDetectCollection |= kSndHWCPUHeadphone;
				debugIOLog ("headphones inserted.\n");
			} else if (newValue == kRemoved) {
				mDetectCollection &= ~kSndHWCPUHeadphone;
				debugIOLog ("headphones removed.\n");
			} else {
				debugIOLog ("Unknown headphone jack status.\n");
				break;
			}

			pluginString = getConnectionKeyFromCharCode (selectorCode, kIOAudioStreamDirectionOutput);
			debug2IOLog ("AppleOnboardAudio::interruptEventHandler - pluginString = %s.\n", pluginString);
			thePlugin = getPluginObjectForConnection (pluginString);
			FailIf (NULL == thePlugin, Exit);
			debug2IOLog ("AppleOnboardAudio::interruptEventHandler - thePlugin = %p.\n", thePlugin);

			cacheOutputVolumeLevels(thePlugin);
			thePlugin->prepareForOutputChange();
			
			connectionCode = parseOutputDetectCollection ();
			connectionCodeNumber = OSNumber::withNumber(connectionCode, 32);
			if (NULL == connectionCodeNumber) debugIOLog ("\n!!!connectionCodeNumber = NULL!!!\n");
			FailIf (NULL == connectionCodeNumber, Exit);
			connectionString = getConnectionKeyFromCharCode (connectionCode, kIOAudioStreamDirectionOutput);

			if (thePlugin == mCurrentOutputPlugin) {
				setClipRoutineForOutput (connectionString);
				selectAmplifier (connectionCode);
				if (NULL == mOutputSelector) debugIOLog ("\n!!!mOutputSelector = NULL!!!\n");
				FailIf (NULL == mOutputSelector, Exit);
				mOutputSelector->hardwareValueChanged (connectionCodeNumber);
				setSoftwareOutputDSP (connectionString);
			}
			connectionCodeNumber->release();

			setHardwareDSP (connectionString);
			
			if (thePlugin == mCurrentOutputPlugin) {
				AdjustOutputVolumeControls(thePlugin, connectionCode);
			}
			break;
		case kExtSpeakersStatus:
			break;
		case kLineOutStatus:
			break;
		case kDigitalOutStatus:
			break;
		case kLineInStatus:
			if (newValue == kInserted) {
				mDetectCollection |= kSndHWLineInput;
			} else if (newValue == kRemoved) {
				mDetectCollection &= ~kSndHWLineInput;
			} else {
				debugIOLog ("Unknown line in status.\n");
				break;
			}

			pluginString = getConnectionKeyFromCharCode (selectorCode, kIOAudioStreamDirectionInput);
			debug2IOLog ("AppleOnboardAudio::interruptEventHandler - pluginString = %s.\n", pluginString);
			thePlugin = getPluginObjectForConnection (pluginString);
			FailIf (NULL == thePlugin, Exit);
			debug2IOLog ("AppleOnboardAudio::interruptEventHandler - thePlugin = %p.\n", thePlugin);

			if (thePlugin == mCurrentInputPlugin) {
				UInt32 			connectionCode;
				OSNumber * 		connectionCodeNumber;
				
				connectionCode = parseInputDetectCollection ();
				connectionCodeNumber = OSNumber::withNumber(connectionCode, 32);
				
				mCurrentInputPlugin->setActiveInput(connectionCode);
				mInputSelector->hardwareValueChanged (connectionCodeNumber);
				
				connectionCodeNumber->release();

				AdjustInputGainControls(thePlugin);
			}
			break;
		case kExternalMicInStatus:
			break;
		case kDigitalInStatus:
			break;
		case kRequestCodecRecoveryStatus:
			//	Walk through the available plugin objects and invoke the recoverFromFatalError on each object in the correct order.
			callPluginsInOrder (kRequestCodecRecoveryStatus, newValue);
			break;
		default:
			break;
	}
Exit:
	return;
}

void AppleOnboardAudio::callPluginsInOrder (UInt32 inSelector, UInt32 newValue) {
	AudioHardwareObjectInterface *		thePluginObject;
	OSArray *							pluginOrderArray;
	OSString *							pluginName;
	UInt32								index;
	UInt32								pluginOrderArrayCount;
	
	debug3IOLog ("AppleOnboardAudio::callPluginsInOrder (%lX, %lX)\n", inSelector, newValue);

	pluginOrderArray = OSDynamicCast (OSArray, getLayoutEntry (kPluginRecoveryOrder));
	FailIf (NULL == pluginOrderArray, Exit);

	pluginOrderArrayCount = pluginOrderArray->getCount ();

	for (index = 0; index < pluginOrderArrayCount; index++) {
		pluginName = OSDynamicCast(OSString, pluginOrderArray->getObject(index));
		if (NULL == pluginName) {
			debug2IOLog ("Corrupt %s entry in AppleOnboardAudio Info.plist\n", kPluginRecoveryOrder);
			continue;
		}
		thePluginObject = getPluginObjectWithName (pluginName);
		if (NULL == thePluginObject) {
			debug2IOLog ("Can't find required AppleOnboardAudio plugin from %s entry loaded\n", kPluginRecoveryOrder);
			continue;
		}

		switch (inSelector) {
			case kRequestCodecRecoveryStatus:
				thePluginObject->recoverFromFatalError ((FatalRecoverySelector)newValue);
				break;
			case kBreakClockSelect:
				thePluginObject->breakClockSelect (newValue);
				break;
			case kMakeClockSelect:
				thePluginObject->makeClockSelect (newValue);
				break;
			case kSetSampleRate:
				thePluginObject->setSampleRate (newValue);
				break;
			case kSetSampleBitDepth:
				thePluginObject->setSampleDepth (newValue);
				break;
			default:
				break;
		}		
	}		
	
Exit:
	return;
}	

// --------------------------------------------------------------------------
//	Returns a target output selection for the current detect states as
//	follows:
//
//									Line Out	Headphone	External
//									Detect		Detect		Speaker
//															Detect
//									--------	---------	--------
//	Internal Speaker				out			out			out
//	External Speaker				out			out			in
//	Headphone						out			in			out
//	Headphone						out			in			in
//	Line Output						in			out			out
//	Line Output						in			out			in
//	Line Output & headphone			in			in			out
//	Line Output & headphone			in			in			in
//
UInt32 AppleOnboardAudio::parseOutputDetectCollection ( void ) {
	UInt32		result;
	
	debug2IOLog ( "+ AppleOnboardAudio::ParseDetectCollection() detectCollection 0x%lX\n", mDetectCollection );

	if ( mDetectCollection & kSndHWLineOutput ) {
		result = kIOAudioOutputPortSubTypeLine;
	} else if ( mDetectCollection & kSndHWCPUHeadphone ) {
		result = kIOAudioOutputPortSubTypeHeadphones;
	} else {
		result = kIOAudioOutputPortSubTypeInternalSpeaker;
	}

	debug2IOLog ( "- AppleOnboardAudio::ParseDetectCollection returns %lX\n", result );
	return result;
}

UInt32 AppleOnboardAudio::parseInputDetectCollection ( void ) {
	UInt32		result;
	
	debug2IOLog ( "+ AppleOnboardAudio::parseInputDetectCollection() detectCollection 0x%lX\n", mDetectCollection );

	if ( mDetectCollection & kSndHWLineInput ) {
		result = kIOAudioOutputPortSubTypeLine;
	} else {
		result = kIOAudioInputPortSubTypeInternalMicrophone;
	}

	debug2IOLog ( "- AppleOnboardAudio::parseInputDetectCollection returns %lX\n", result );
	return result;
}

void AppleOnboardAudio::initializeDetectCollection ( void ) {
	debugIOLog ( "+ AppleOnboardAudio::initializeDetectCollection() \n" );

	if ( kGPIO_Connected == mPlatformInterface->getHeadphoneConnected() ) {
		mDetectCollection |= kSndHWCPUHeadphone;
		debugIOLog ( "AppleOnboardAudio::initializeDetectCollection() - headphones are connected.\n" );
	} 
	if ( kGPIO_Connected == mPlatformInterface->getLineOutConnected() ) {
		mDetectCollection |= kSndHWLineOutput;
	} 
	if ( kGPIO_Connected == mPlatformInterface->getDigitalOutConnected() ) {
		mDetectCollection |= kSndHWDigitalOutput;
	} 
	if ( kGPIO_Connected == mPlatformInterface->getLineInConnected() ) {
		mDetectCollection |= kSndHWLineInput;
	} 
	if ( kGPIO_Connected == mPlatformInterface->getDigitalInConnected() ) {
		mDetectCollection |= kSndHWDigitalInput;
	} 
	
	debug2IOLog ( "- AppleOnboardAudio::initializeDetectCollection() %lX\n", mDetectCollection );
	return;
}

void AppleOnboardAudio::selectAmplifier (const UInt32 inSelection)
{
	switch (inSelection) {
		case kIOAudioOutputPortSubTypeHeadphones:
			debugIOLog ( "switching amps to headphones.\n" );
			mPlatformInterface->setHeadphoneMuteState ( kGPIO_Unmuted );
			mPlatformInterface->setSpeakerMuteState ( kGPIO_Muted );
			mPlatformInterface->setLineOutMuteState ( kGPIO_Muted );
			IOSleep (mAmpRecoveryMuteDuration);
			break;
		case kIOAudioOutputPortSubTypeLine:
			debugIOLog ( "switching amps to line out.\n" );
			mPlatformInterface->setHeadphoneMuteState ( kGPIO_Muted );
			mPlatformInterface->setSpeakerMuteState ( kGPIO_Muted );
			mPlatformInterface->setLineOutMuteState ( kGPIO_Unmuted );
			IOSleep (mAmpRecoveryMuteDuration);
			break;
		case kIOAudioOutputPortSubTypeInternalSpeaker:
			debugIOLog ( "switching amps to internal.\n" );
			mPlatformInterface->setHeadphoneMuteState ( kGPIO_Muted );
			mPlatformInterface->setSpeakerMuteState ( kGPIO_Unmuted );
			mPlatformInterface->setLineOutMuteState ( kGPIO_Muted );
			IOSleep (mAmpRecoveryMuteDuration);
			break;
		case kIOAudioOutputPortSubTypeSPDIF:
			debugIOLog ( "muting amps for digital output.\n" );
			mPlatformInterface->setHeadphoneMuteState ( kGPIO_Muted );
			mPlatformInterface->setSpeakerMuteState ( kGPIO_Muted );
			mPlatformInterface->setLineOutMuteState ( kGPIO_Muted );
			break;
		default:
			debug2IOLog("Amplifier not changed, selection = %ld\n", inSelection);
			break;
	}
    return;
}

void AppleOnboardAudio::free()
{
    debugIOLog("+ AppleOnboardAudio::free\n");
    
    if (mDriverDMAEngine) {
		debug2IOLog("  AppleOnboardAudio::free - mDriverDMAEngine retain count = %d\n", mDriverDMAEngine->getRetainCount());
        mDriverDMAEngine->release();
		mDriverDMAEngine = NULL;
	}
	CLEAN_RELEASE(mOutMuteControl);
	CLEAN_RELEASE(mHeadphoneConnected);
	CLEAN_RELEASE(mOutLeftVolumeControl);
	CLEAN_RELEASE(mOutRightVolumeControl);
	CLEAN_RELEASE(mOutMasterVolumeControl);
	CLEAN_RELEASE(mInLeftGainControl);
	CLEAN_RELEASE(mInRightGainControl);
	CLEAN_RELEASE(mOutputSelector);
	CLEAN_RELEASE(mInputSelector);
	CLEAN_RELEASE(mPluginObjects);
	CLEAN_RELEASE(mPlatformInterface);
	CLEAN_RELEASE(mTransportInterface);

	if (idleTimer) {
		if (workLoop) {
			workLoop->removeEventSource (idleTimer);
		}

		idleTimer->release ();
		idleTimer = NULL;
	}

    super::free();
    debugIOLog("- AppleOnboardAudio::free, (void)\n");
}

#pragma mark +PORT HANDLER FUNCTIONS
void AppleOnboardAudio::setCurrentDevices(UInt32 devices){
    UInt32					odevice;

    if(devices != currentDevices) {
        odevice = currentDevices;
        currentDevices = devices;
//        changedDeviceHandler(odevice);
    }
    
	debug2IOLog ("currentDevices = %ld\n", currentDevices);
	debug2IOLog ("fCPUNeedsPhaseInversion = %d\n", fCPUNeedsPhaseInversion);
    // if this CPU has a phase inversion feature see if we need to enable phase inversion    
    if (fCPUNeedsPhaseInversion) {
        bool state;

        if (currentDevices == 0 || currentDevices & kSndHWInternalSpeaker) {			// may only need the kSndHWInternalSpeaker check
            state = true;
        } else {
            state = false;
		}

         mDriverDMAEngine->setPhaseInversion(state);
    }

	if (devices & kSndHWInputDevices || odevice & kSndHWInputDevices) {
		if (NULL != mInputConnectionControl) {
			OSNumber *			inputState;
			UInt32				active;

			active = devices & kSndHWInputDevices ? 1 : 0;		// If something is plugged in, that's good enough for now.
			inputState = OSNumber::withNumber ((long long unsigned int)active, 32);
			(void)mInputConnectionControl->hardwareValueChanged (inputState);
		}
	}

}

AudioHardwareObjectInterface * AppleOnboardAudio::getIndexedPluginObject (UInt32 index) {
	AudioHardwareObjectInterface *		thePluginObject;
	OSObject *							theObject;
	
	thePluginObject = NULL;
	theObject = mPluginObjects->getObject(index);
	
	if (NULL != theObject) {
		thePluginObject = (AudioHardwareObjectInterface *)(theObject->metaCast("AudioHardwareObjectInterface"));
	}
	
	return thePluginObject;
}

PlatformInterface * AppleOnboardAudio::getPlatformInterfaceObject () {
	return mPlatformInterface;
}

#pragma mark +IOAUDIO INIT
bool AppleOnboardAudio::initHardware (IOService *provider){
	AudioHardwareObjectInterface *		thePluginObject;
	OSDictionary *						AOAprop;
	IOWorkLoop *						workLoop;
	OSBoolean *							softwareInputGainBoolean;
	OSNumber * 							inputLatencyNumber;
	OSNumber * 							outputLatencyNumber;
	OSNumber * 							connectionCodeNumber;
	OSNumber * 							volumeNumber;
	char * 								connectionString;
	UInt32 								connectionCode;
	UInt32								outputLatency;
	UInt32								inputLatency;
	UInt32								selectorCode;
	UInt32								index;
	UInt32								count;
    bool								result;

    debugIOLog("+ AppleOnboardAudio::initHardware\n");

	volumeNumber = OSNumber::withNumber((long long unsigned int)0, 32);

	result = FALSE;
    if (!super::initHardware (provider)) {
        goto Exit;
    }

	workLoop = getWorkLoop();
	FailIf (NULL == workLoop, Exit);

	// must occur in this order, and must be called in initHardware or later to have a valid workloop
	mPlatformInterface->setWorkLoop(workLoop);
	
	FailIf (NULL == mPluginObjects, Exit);

	count = mPluginObjects->getCount ();
	FailIf (0 == count, Exit);
	
	for (index = 0; index < count; index++) {
		thePluginObject = getIndexedPluginObject (index);

		FailIf (NULL == thePluginObject, Exit);

		thePluginObject->setWorkLoop(workLoop);

		thePluginObject->initPlugin(mPlatformInterface);

		// FIX - check the result of this call and remove plugin if it fails!
		thePluginObject->preDMAEngineInit ();

		thePluginObject->setProperty (kPluginPListMasterVol, volumeNumber);
		thePluginObject->setProperty (kPluginPListLeftVol, volumeNumber);
		thePluginObject->setProperty (kPluginPListRightVol, volumeNumber);
		thePluginObject->setProperty (kPluginPListLeftGain, volumeNumber);
		thePluginObject->setProperty (kPluginPListRightGain, volumeNumber);

		// XXX FIX - this is a temporary init
		mCurrentOutputPlugin = thePluginObject;
		mCurrentInputPlugin = thePluginObject;
	}

	volumeNumber->release ();

    setManufacturerName ("Apple");
    setDeviceName ("Built-in audio controller");
	setDeviceTransportType (kIOAudioDeviceTransportTypeBuiltIn);

    configureDMAEngines (provider);
	FailIf (NULL == mDriverDMAEngine, Exit);

	// Have to create the audio controls before calling activateAudioEngine
    createDefaultControls (); 

	debug2IOLog("  AppleOnboardAudio::initHardware - mDriverDMAEngine retain count before activateAudioEngine = %d\n", mDriverDMAEngine->getRetainCount());
    if (kIOReturnSuccess != activateAudioEngine (mDriverDMAEngine)) {
		mDriverDMAEngine->release ();
		mDriverDMAEngine = NULL;
        goto Exit;
    }
	debug2IOLog("  AppleOnboardAudio::initHardware - mDriverDMAEngine retain count after activateAudioEngine = %d\n", mDriverDMAEngine->getRetainCount());

	idleTimer = IOTimerEventSource::timerEventSource (this, sleepHandlerTimer);
	if (!idleTimer) {
		goto Exit;
	}
	workLoop->addEventSource (idleTimer);

	// Set this to a default for desktop machines (portables will get a setAggressiveness call later in the boot sequence).
	ourPowerState = kIOAudioDeviceActive;
	idleSleepDelayTime = kNoIdleAudioPowerDown;
	// [3107909] Turn the hardware off because IOAudioFamily defaults to the off state, so make sure the hardware is off or we get out of synch with the family.
	setIdleAudioSleepTime (idleSleepDelayTime);

	// Give drivers a chance to do something after the DMA engine and IOAudioFamily have been created/started
	for (index = 0; index < count; index++) {
		thePluginObject = getIndexedPluginObject (index);
		FailIf (NULL == thePluginObject, Exit);
		thePluginObject->postDMAEngineInit ();
	}

	initializeDetectCollection();
	
	connectionCode = parseOutputDetectCollection ();
	connectionCodeNumber = OSNumber::withNumber(connectionCode, 32);
	
	mOutputSelector->hardwareValueChanged (connectionCodeNumber);
	
	connectionCodeNumber->release();
		
	FailIf (NULL == mOutputSelector, Exit);
	selectorCode = mOutputSelector->getIntValue ();
	debug2IOLog("mOutputSelector->getIntValue () returns %lX\n", selectorCode);
	if (0 != selectorCode) {
		connectionString = getConnectionKeyFromCharCode (selectorCode, kIOAudioStreamDirectionOutput);
		debug2IOLog("mOutputSelector->getIntValue () char code is %s\n", connectionString);
		if (NULL != connectionString) {
			mCurrentOutputPlugin = getPluginObjectForConnection (connectionString);
		}
	}
	debug2IOLog("mCurrentOutputPlugin = %p\n", mCurrentOutputPlugin);

	mCurrentOutputPlugin->prepareForOutputChange ();
	selectAmplifier (connectionCode);
	
	FailIf (NULL == mInputSelector, Exit);
	selectorCode = mInputSelector->getIntValue ();
	debug2IOLog("mInputSelector->getIntValue () returns %4s\n", (char *)&selectorCode);
	if (0 != selectorCode) {
		connectionString = getConnectionKeyFromCharCode (selectorCode, kIOAudioStreamDirectionInput);
		debug2IOLog("mInputSelector->getIntValue () char code is %s\n", connectionString);
		if (NULL != connectionString) {
			mCurrentInputPlugin = getPluginObjectForConnection (connectionString);
		}
	}
	debug2IOLog("mCurrentInputPlugin = %p\n", mCurrentInputPlugin);
	
	mCurrentInputPlugin->setActiveInput (selectorCode);

	AOAprop = OSDynamicCast (OSDictionary, mCurrentInputPlugin->getProperty (kPluginPListAOAAttributes));
	FailIf (NULL == AOAprop, Exit);

	softwareInputGainBoolean = OSDynamicCast (OSBoolean, AOAprop->getObject (kPluginPListSoftwareInputGain));
	if (NULL != softwareInputGainBoolean) {
		mDriverDMAEngine->setUseSoftwareInputGain (softwareInputGainBoolean->getValue ());
		mCurrentPluginHasSoftwareInputGain = softwareInputGainBoolean->getValue ();
	} else {
		mDriverDMAEngine->setUseSoftwareInputGain (false);
		mCurrentPluginHasSoftwareInputGain = false;
	}
	inputLatencyNumber = OSDynamicCast (OSNumber, AOAprop->getObject (kPluginPListInputLatency));
	if (NULL != inputLatencyNumber) {
		inputLatency = inputLatencyNumber->unsigned32BitValue();
	}
	
	AOAprop = OSDynamicCast (OSDictionary, mCurrentOutputPlugin->getProperty (kPluginPListAOAAttributes));
	FailIf (NULL == AOAprop, Exit);

	outputLatencyNumber = OSDynamicCast (OSNumber, AOAprop->getObject (kPluginPListOutputLatency));
	if (NULL != outputLatencyNumber) {
		outputLatency = outputLatencyNumber->unsigned32BitValue();
	}

	mDriverDMAEngine->setSampleLatencies (outputLatency, inputLatency);
	//mDriverDMAEngine->setRightChanDelay(TRUE);			

	// Set the default volume to that stored in the PRAM in case we don't get a setValue call from the Sound prefs before being activated.
	mAutoUpdatePRAM = FALSE;			// Don't update the PRAM value while we're initing from it
	if (NULL != mOutLeftVolumeControl) {
		mOutLeftVolumeControl->setValue (PRAMToVolumeValue ());
	}
	if (NULL != mOutRightVolumeControl) {
		mOutRightVolumeControl->setValue (PRAMToVolumeValue ());
	}

	// Has to be after creating the controls so that interruptEventHandler isn't called before the selector controls exist.
	mPlatformInterface->registerInterrupts ( this );

    flushAudioControls ();
//	if (NULL != mOutMuteControl)
//		mOutMuteControl->flushValue ();
	mAutoUpdatePRAM = TRUE;

 	// Install power change handler so we get notified about shutdown
	mSysPowerDownNotifier = registerPrioritySleepWakeInterest (&sysPowerDownHandler, this, 0);

	result = TRUE;

Exit:
    debug2IOLog ("- AppleOnboardAudio::initHardware returns %d\n", result); 
	return (result);
}

IOReturn AppleOnboardAudio::configureDMAEngines(IOService *provider){
    IOReturn 						result;
    bool							hasInput;
	OSArray *						formatsArray;
	OSArray *						inputListArray;
    
    result = kIOReturnError;

	inputListArray = OSDynamicCast (OSArray, getLayoutEntry (kInputsList));
	hasInput = (NULL != inputListArray);

    debug3IOLog("+ AppleOnboardAudio[%p]::configureDMAEngines (%p)\n", this, provider);

	// All this config should go in a single method
    mDriverDMAEngine = new AppleDBDMAAudio;
    // make sure we get an engine
    FailIf (NULL == mDriverDMAEngine, Exit);

	formatsArray = OSDynamicCast (OSArray, getLayoutEntry (kFormats));

    if (!mDriverDMAEngine->init (NULL, mPlatformInterface, (IOService *)provider->getParentEntry (gIODTPlane), hasInput, formatsArray)) {
        mDriverDMAEngine->release ();
		mDriverDMAEngine = NULL;
        goto Exit;
    }
   
	result = kIOReturnSuccess;

Exit:
    debug4IOLog("- AppleOnboardAudio[%p]::configureDMAEngines (%p) returns %x\n", this, provider, result);
    return result;
}

UInt32 AppleOnboardAudio::getCharCodeForString (OSString * string) {
	UInt32								charCode;

	if (string->isEqualTo (kInternalSpeakers)) {
		charCode = kIOAudioOutputPortSubTypeInternalSpeaker;
	} else if (string->isEqualTo (kExternalSpeakers)) {
		charCode = kIOAudioOutputPortSubTypeExternalSpeaker;
	} else if (string->isEqualTo (kHeadphones)) {
		charCode = kIOAudioOutputPortSubTypeHeadphones;
	} else if (string->isEqualTo (kLineOut)) {
		charCode = kIOAudioOutputPortSubTypeLine;
	} else if (string->isEqualTo (kInternalMic)) {
		charCode = kIOAudioInputPortSubTypeInternalMicrophone;
	} else if (string->isEqualTo (kExternalMic)) {
		charCode = kIOAudioInputPortSubTypeExternalMicrophone;
	} else if (string->isEqualTo (kLineIn)) {
		charCode = kIOAudioInputPortSubTypeLine;
	} else if (string->isEqualTo (kDigitalIn) || string->isEqualTo (kDigitalOut)) {
		charCode = kIOAudioInputPortSubTypeSPDIF;
	} else {
		charCode = '????';
	}

	return charCode;
}

UInt32 AppleOnboardAudio::getCharCodeForIntCode (UInt32 inCode) {
	UInt32								charCode;

	if (kInternalSpeakerStatus == inCode) {
		charCode = kIOAudioOutputPortSubTypeInternalSpeaker;
	} else if (kExtSpeakersStatus == inCode) {
		charCode = kIOAudioOutputPortSubTypeExternalSpeaker;
	} else if (kHeadphoneStatus == inCode) {
		charCode = kIOAudioOutputPortSubTypeHeadphones;
	} else if (kLineOutStatus == inCode) {
		charCode = kIOAudioOutputPortSubTypeLine;
	} else if (kInputMicStatus == inCode) {
		charCode = kIOAudioInputPortSubTypeInternalMicrophone;
	} else if (kExternalMicInStatus == inCode) {
		charCode = kIOAudioInputPortSubTypeExternalMicrophone;
	} else if (kLineInStatus == inCode) {
		charCode = kIOAudioInputPortSubTypeLine;
	} else if (kDigitalInStatus == inCode) {
		charCode = kIOAudioInputPortSubTypeSPDIF;
	} else if (kDigitalOutStatus == inCode) {
		charCode = kIOAudioOutputPortSubTypeSPDIF;
	} else {
		charCode = '????';
	}

	return charCode;
}

OSString * AppleOnboardAudio::getStringForCharCode (UInt32 charCode) {
	OSString *							theString;

	switch (charCode) {
		case kIOAudioOutputPortSubTypeInternalSpeaker:
			theString = OSString::withCString ("Internal Speakers");
			break;
		case kIOAudioOutputPortSubTypeExternalSpeaker:
			theString = OSString::withCString ("External Speakers");
			break;
		case kIOAudioOutputPortSubTypeHeadphones:
			theString = OSString::withCString ("Headphones");
			break;
		case kIOAudioOutputPortSubTypeLine:
			theString = OSString::withCString ("Line");
			break;
		case kIOAudioOutputPortSubTypeSPDIF:
			theString = OSString::withCString ("Digital");
			break;
		case kIOAudioInputPortSubTypeInternalMicrophone:
			theString = OSString::withCString ("Internal Microphone");
			break;
		case kIOAudioInputPortSubTypeExternalMicrophone:
			theString = OSString::withCString ("External Microphone");
			break;
		default:
			theString = NULL;
			break;
	}

	return theString;
}

char * AppleOnboardAudio::getConnectionKeyFromCharCode (const SInt32 inSelection, const UInt32 inDirection) {
	char * selectedOutput;

	switch (inSelection) {
		// output
		case kIOAudioOutputPortSubTypeInternalSpeaker:
			selectedOutput = kInternalSpeakers;
			break;
		case kIOAudioOutputPortSubTypeExternalSpeaker:
			selectedOutput = kExternalSpeakers;
			break;
		case kIOAudioOutputPortSubTypeHeadphones:
			selectedOutput = kHeadphones;
			break;
		// input
		case kIOAudioInputPortSubTypeInternalMicrophone:
			selectedOutput = kInternalMic;
			break;
		case kIOAudioInputPortSubTypeExternalMicrophone:
			selectedOutput = kExternalMic;
			break;
		// input/output
		case kIOAudioOutputPortSubTypeSPDIF:
			if (kIOAudioStreamDirectionOutput == inDirection) {
				selectedOutput = kDigitalOut;
			} else {
				selectedOutput = kDigitalIn;
			}
			break;
		case kIOAudioOutputPortSubTypeLine:
			if (kIOAudioStreamDirectionOutput == inDirection) {
				selectedOutput = kLineOut;
			} else {
				selectedOutput = kLineIn;
			}
			break;
		default:
			selectedOutput = 0;
			break;
	}

	return selectedOutput;
}

IOReturn AppleOnboardAudio::createInputSelectorControl (void) {
	OSArray *							inputsList;
	OSString *							inputString;
	OSString *							selectionString;
	IOReturn							result;
	UInt32								inputsCount;
	UInt32								inputSelection;
	UInt32								index;

	result = kIOReturnError;
	inputsList = OSDynamicCast (OSArray, getLayoutEntry (kInputsList));
	FailIf (NULL == inputsList, Exit);

	inputsCount = inputsList->getCount ();
	inputString = OSDynamicCast (OSString, inputsList->getObject (0));
	FailIf (NULL == inputString, Exit);

	inputSelection = getCharCodeForString (inputString);

	mInputSelector = IOAudioSelectorControl::createInputSelector (inputSelection, kIOAudioControlChannelIDAll);
	if (NULL != mInputSelector) {
		mDriverDMAEngine->addDefaultAudioControl (mInputSelector);
		mInputSelector->setValueChangeHandler ((IOAudioControl::IntValueChangeHandler)inputControlChangeHandler, this);
		for (index = 0; index < inputsCount; index++) {
			inputString = OSDynamicCast (OSString, inputsList->getObject (index));
			FailIf (NULL == inputString, Exit);
			inputSelection = getCharCodeForString (inputString);
			selectionString = getStringForCharCode (inputSelection);
			mInputSelector->addAvailableSelection (inputSelection, selectionString);
			selectionString->release ();
		}
	}

	debug2IOLog("AppleOnboardAudio::createInputSelectorControl - mInputSelector = %p\n", mInputSelector);

	result = kIOReturnSuccess;

Exit:
	return result;
}

IOReturn AppleOnboardAudio::createOutputSelectorControl (void) {
	OSArray *							outputsList;
	OSString *							outputString;
	OSString *							selectionString;
	IOReturn							result;
	UInt32								outputsCount;
	UInt32								outputSelection;
	UInt32								index;

	result = kIOReturnError;
	outputsList = OSDynamicCast (OSArray, getLayoutEntry (kOutputsList));
	FailIf (NULL == outputsList, Exit);

	outputsCount = outputsList->getCount ();
	outputString = OSDynamicCast (OSString, outputsList->getObject (0));
	FailIf (NULL == outputString, Exit);

//	debugIOLog("AppleOnboardAudio::createOutputSelectorControl - outputString = %s\n", outputString->getCStringNoCopy());

	outputSelection = getCharCodeForString (outputString);

//	debugIOLog("AppleOnboardAudio::createOutputSelectorControl - outputSelection = %4s\n", (char *)&outputSelection);

	mOutputSelector = IOAudioSelectorControl::createOutputSelector (outputSelection, kIOAudioControlChannelIDAll);
	if (NULL != mOutputSelector) {
		mDriverDMAEngine->addDefaultAudioControl (mOutputSelector);
		mOutputSelector->setValueChangeHandler ((IOAudioControl::IntValueChangeHandler)outputControlChangeHandler, this);
		for (index = 0; index < outputsCount; index++) {
			outputString = OSDynamicCast (OSString, outputsList->getObject (index));
			FailIf (NULL == outputString, Exit);
			outputSelection = getCharCodeForString (outputString);
			selectionString = getStringForCharCode (outputSelection);
			mOutputSelector->addAvailableSelection (outputSelection, selectionString);
//			debugIOLog("AppleOnboardAudio::createOutputSelectorControl - addAvailableSelection(%4s, %s)\n", (char *)&outputSelection, selectionString->getCStringNoCopy());
			selectionString->release ();
		}
	}

	debug2IOLog("AppleOnboardAudio::createOutputSelectorControl - mOutputSelector = %p\n", mOutputSelector);

	result = kIOReturnSuccess;

Exit:
	return result;
}

AudioHardwareObjectInterface * AppleOnboardAudio::getPluginObjectForConnection (const char * entry) {
	AudioHardwareObjectInterface *		thePluginObject;
	OSDictionary *						dictEntry;
	OSString *							pluginIDMatch;

	thePluginObject = NULL;
	pluginIDMatch = NULL;
	dictEntry = NULL;
	
	dictEntry = OSDynamicCast (OSDictionary, getLayoutEntry (entry));
	FailIf (NULL == dictEntry, Exit);

	pluginIDMatch = OSDynamicCast (OSString, dictEntry->getObject (kPluginID));
	FailIf (NULL == pluginIDMatch, Exit);

	thePluginObject = getPluginObjectWithName (pluginIDMatch);
	
	debug2IOLog ("AppleOnboardAudio::getPluginObjectForConnection - pluginID = %s\n", pluginIDMatch->getCStringNoCopy());

Exit:
	return thePluginObject;
}

AudioHardwareObjectInterface * AppleOnboardAudio::getPluginObjectWithName (OSString * inName) {
	AudioHardwareObjectInterface *		thePluginObject;
    OSDictionary *						AOAprop;
	OSString *							thePluginID;
	UInt32								index;
	UInt32								count;
	Boolean								found;

	thePluginObject = NULL;

	count = mPluginObjects->getCount ();
	found = FALSE;
	index = 0;
	while (!found && index < count) {
		thePluginObject = getIndexedPluginObject (index);
		FailIf (NULL == thePluginObject, Exit);
		AOAprop = OSDynamicCast (OSDictionary, thePluginObject->getProperty (kPluginPListAOAAttributes));
		FailIf (NULL == AOAprop, Exit);
		thePluginID = OSDynamicCast (OSString, AOAprop->getObject (kPluginID));
		FailIf (NULL == thePluginID, Exit);

		if (thePluginID->isEqualTo (inName)) {
			debug2IOLog ("AppleOnboardAudio found matching plugin with ID %s\n", thePluginID->getCStringNoCopy());
			found = TRUE;
		}
		index++;
	}
	
Exit:	
	return thePluginObject;
}

IOReturn AppleOnboardAudio::createInputGainControls () {
	AudioHardwareObjectInterface *		thePluginObject;
	char *								selectedInput;
	IOReturn							result;
	IOFixed								mindBGain;
	IOFixed								maxdBGain;
	UInt32								curSelection;
	SInt32								minGain;
	SInt32								maxGain;

	result = kIOReturnError;

	curSelection = mInputSelector->getIntValue ();
	
	selectedInput = getConnectionKeyFromCharCode (curSelection, kIOAudioStreamDirectionInput);

	setUseInputGainControls (selectedInput);

	if (mUseInputGainControls) {
		thePluginObject = getPluginObjectForConnection (selectedInput);
		FailIf (NULL == thePluginObject, Exit);

		debug2IOLog ("creating input gain controls for input %s\n", selectedInput);
	
		mindBGain = thePluginObject->getMinimumdBGain ();
		maxdBGain = thePluginObject->getMaximumdBGain ();
		minGain = thePluginObject->getMinimumGain ();
		maxGain = thePluginObject->getMaximumGain ();
	
		mInLeftGainControl = IOAudioLevelControl::createVolumeControl ((maxGain-minGain) / 2, minGain, maxGain, mindBGain, maxdBGain, kIOAudioControlChannelIDDefaultLeft, kIOAudioControlChannelNameLeft, 0, kIOAudioControlUsageInput);
		if (NULL != mInLeftGainControl) {
			mDriverDMAEngine->addDefaultAudioControl (mInLeftGainControl);
			mInLeftGainControl->setValueChangeHandler ((IOAudioControl::IntValueChangeHandler)inputControlChangeHandler, this);
			// Don't release it because we might reference it later
		}
	
		mInRightGainControl = IOAudioLevelControl::createVolumeControl ((maxGain-minGain) / 2, minGain, maxGain, mindBGain, maxdBGain, kIOAudioControlChannelIDDefaultRight, kIOAudioControlChannelNameRight, 0, kIOAudioControlUsageInput);
		if (NULL != mInRightGainControl) {
			mDriverDMAEngine->addDefaultAudioControl (mInRightGainControl);
			mInRightGainControl->setValueChangeHandler ((IOAudioControl::IntValueChangeHandler)inputControlChangeHandler, this);
			// Don't release it because we might reference it later
		}
	}

	createPlayThruControl ();

	result = kIOReturnSuccess;

Exit:
	return result;
}

OSArray * AppleOnboardAudio::getControlsArray (const char * inSelectedOutput) {
	OSArray *							theArray;
	OSDictionary *						theOutput;
	
	theArray = NULL;
	
	theOutput = OSDynamicCast(OSDictionary, getLayoutEntry(inSelectedOutput));
	FailIf (NULL == theOutput, Exit);
	theArray = OSDynamicCast(OSArray, theOutput->getObject(kControls));
	
Exit:
	return theArray;
}

IOReturn AppleOnboardAudio::setHardwareDSP (UInt32 inSelectedOutput) {
	return setHardwareDSP (getConnectionKeyFromCharCode (inSelectedOutput, kIOAudioStreamDirectionOutput));
}

IOReturn AppleOnboardAudio::setHardwareDSP (const char * inSelectedOutput) {
	OSDictionary *						theSpeakerIDDict;
	OSDictionary *						theEQDict;
	OSDictionary *						theOutput;
	OSNumber *							theEQNumber;
	OSString *							speakerIDString;
	char								speakerIDCString[32];
	UInt32								theEQID;
	IOReturn							result;
	
	result = kIOReturnError;
	
	theEQID = kNoEQID;
	
	theOutput = OSDynamicCast(OSDictionary, getLayoutEntry(inSelectedOutput));
	FailIf (NULL == theOutput, Exit);

	theEQDict = OSDynamicCast(OSDictionary, theOutput->getObject(kSignalProcessing));
	FailIf (NULL == theEQDict, Exit);

	sprintf (speakerIDCString, "%s_%ld", kSpeakerID, mSpeakerID); 
	debug2IOLog("setHardwareDSP: speakerIDCString = %s\n", speakerIDCString);
	speakerIDString = OSString::withCString (speakerIDCString);
	FailIf (NULL == speakerIDString, Exit);
	
	theSpeakerIDDict = OSDynamicCast(OSDictionary, theEQDict->getObject(speakerIDString));
	speakerIDString->release();
	FailIf (NULL == theSpeakerIDDict, Exit);
	debug2IOLog("setHardwareDSP: theDict = %p\n", theSpeakerIDDict);

	theEQNumber = OSDynamicCast(OSNumber, theSpeakerIDDict->getObject(kHardwareDSPIndex));
	FailIf (NULL == theEQNumber, Exit);
	debug2IOLog("setHardwareDSP: theEQNumber = %p\n", theEQNumber);
	theEQID = theEQNumber->unsigned32BitValue();

	if (kNoEQID != theEQID) {
		mCurrentOutputPlugin->setEQ (theEQID);
	} else {
		mCurrentOutputPlugin->disableEQ ();
	}

	result = kIOReturnSuccess;

Exit:
	debug2IOLog("setHardwareDSP: returns %X\n", result);
	return result;
}

void AppleOnboardAudio::setSoftwareOutputDSP (const char * inSelectedOutput) {
	OSDictionary *						theSpeakerIDDict;
	OSDictionary *						theSignalProcessingDict;
	OSDictionary *						theEQDict;
	OSDictionary *						theDynamicsDict;
	OSDictionary *						theCrossoverDict;
	OSDictionary *						theOutput;
	OSDictionary *						theSoftwareDSPDict;
	OSString *							speakerIDString;
	char								speakerIDCString[32];

	// commmon case is disabled, this is the safer fail senario
	mDriverDMAEngine->disableSoftwareEQ ();
	mDriverDMAEngine->disableSoftwareLimiter ();
	mDriverDMAEngine->disableSoftwareCrossover ();

	theOutput = OSDynamicCast(OSDictionary, getLayoutEntry (inSelectedOutput));
	FailIf (NULL == theOutput, Exit);

	theSignalProcessingDict = OSDynamicCast (OSDictionary, theOutput->getObject (kSignalProcessing));
	FailIf (NULL == theSignalProcessingDict, Exit);

	sprintf (speakerIDCString, "%s_%ld", kSpeakerID, mSpeakerID); 
	debug2IOLog ("setSoftwareOutputDSP: speakerIDString = %s\n", speakerIDCString);
	speakerIDString = OSString::withCString (speakerIDCString);
	FailIf (NULL == speakerIDString, Exit);
	
	theSpeakerIDDict = OSDynamicCast (OSDictionary, theSignalProcessingDict->getObject (speakerIDString));
	speakerIDString->release ();
	FailIf (NULL == theSpeakerIDDict, Exit);
	debug2IOLog ("setSoftwareOutputDSP: theSpeakerIDDict = %p\n", theSpeakerIDDict);

	theSoftwareDSPDict = OSDynamicCast (OSDictionary, theSpeakerIDDict->getObject (kSoftwareDSP));
	FailIf (NULL == theSoftwareDSPDict, Exit);
	debug2IOLog ("setSoftwareOutputDSP: theSoftwareDSPDict = %p\n", theSoftwareDSPDict);

	theEQDict = OSDynamicCast(OSDictionary, theSoftwareDSPDict->getObject (kEqualization));
	if (NULL != theEQDict) {
		debug2IOLog ("setSoftwareOutputDSP: theEQDict = %p\n", theEQDict);
		mDriverDMAEngine->setEqualizationFromDictionary (theEQDict);
	} 
	theDynamicsDict = OSDynamicCast(OSDictionary, theSoftwareDSPDict->getObject (kDynamicRange));
	if (NULL != theDynamicsDict) {
		debug2IOLog ("setSoftwareOutputDSP: theDynamicsDict = %p\n", theDynamicsDict);
		mDriverDMAEngine->setLimiterFromDictionary (theDynamicsDict);
		
		theCrossoverDict = OSDynamicCast(OSDictionary, theDynamicsDict->getObject (kCrossover));
		// this handles the case of theCrossoverDict = NULL, setting the crossover state to 1 band
		mDriverDMAEngine->setCrossoverFromDictionary (theCrossoverDict);
	}
Exit:
	return;
}

UInt32 AppleOnboardAudio::setClipRoutineForOutput (const char * inSelectedOutput) {
	OSDictionary *						theOutput;
	OSString *							clipRoutineString;
	OSArray *							theArray;
	IOReturn							result;
	UInt32								arrayCount;
	UInt32								index;
	
	result = kIOReturnSuccess;
	theArray = NULL;
	
	theOutput = OSDynamicCast(OSDictionary, getLayoutEntry(inSelectedOutput));
	FailIf (NULL == theOutput, Exit);
	
	theArray = OSDynamicCast(OSArray, theOutput->getObject(kClipRoutines));
	FailIf (NULL == theArray, Exit);
	
	arrayCount = theArray->getCount();

	mDriverDMAEngine->resetOutputClipOptions();
	
	for (index = 0; index < arrayCount; index++) {
		clipRoutineString = OSDynamicCast(OSString, theArray->getObject(index));
		debug3IOLog("getClipRoutineForOutput: clip routine[%ld] = %s\n", index, clipRoutineString->getCStringNoCopy());

		if (clipRoutineString->isEqualTo (kPhaseInversionClipString)) {
			mDriverDMAEngine->setPhaseInversion (true);
		} else if (clipRoutineString->isEqualTo (kStereoToRightChanClipString)) {
			mDriverDMAEngine->setRightChanMixed (true);
		} else if (clipRoutineString->isEqualTo (kDelayRightChan1SampleClipString)) {
			mDriverDMAEngine->setRightChanDelay (true);
		}
	}	

	result = kIOReturnSuccess;
Exit:
	debug2IOLog("getClipRoutineForOutput returns %X\n", result);
	return result;
}

UInt32 AppleOnboardAudio::setClipRoutineForInput (const char * inSelectedInput) {
	OSDictionary *						theInput;
	OSString *							clipRoutineString;
	OSArray *							theArray;
	IOReturn							result;
	UInt32								arrayCount;
	UInt32								index;
	
	result = kIOReturnSuccess;
	theArray = NULL;
	
	theInput = OSDynamicCast(OSDictionary, getLayoutEntry(inSelectedInput));
	FailIf (NULL == theInput, Exit);
	
	theArray = OSDynamicCast(OSArray, theInput->getObject(kClipRoutines));
	FailIf (NULL == theArray, Exit);
	
	arrayCount = theArray->getCount();

	mDriverDMAEngine->resetInputClipOptions();
	
	for (index = 0; index < arrayCount; index++) {
		clipRoutineString = OSDynamicCast(OSString, theArray->getObject(index));
		debug3IOLog("getClipRoutineForInput: clip routine[%ld] = %s\n", index, clipRoutineString->getCStringNoCopy());

		if (clipRoutineString->isEqualTo (kCopyLeftToRight)) {
			mDriverDMAEngine->setDualMonoMode (e_Mode_CopyLeftToRight);
		} else if (clipRoutineString->isEqualTo (kCopyRightToLeft)) {
			mDriverDMAEngine->setDualMonoMode (e_Mode_CopyRightToLeft);
		}	
	}	

	result = kIOReturnSuccess;
Exit:
	debug2IOLog("getClipRoutineForInput returns %X\n", result);
	return result;
}

void AppleOnboardAudio::cacheOutputVolumeLevels (AudioHardwareObjectInterface * thePluginObject) {
	if (NULL != mOutMasterVolumeControl) {
		mCurrentOutputPlugin->setProperty(kPluginPListMasterVol, mOutMasterVolumeControl->getValue ());
	} else {
		if (NULL != mOutLeftVolumeControl) {
			mCurrentOutputPlugin->setProperty(kPluginPListLeftVol, mOutLeftVolumeControl->getValue ());
		}
		if (NULL != mOutLeftVolumeControl) {
			mCurrentOutputPlugin->setProperty(kPluginPListRightVol, mOutRightVolumeControl->getValue ());
		}
	}	
	return;
}

void AppleOnboardAudio::cacheInputGainLevels (AudioHardwareObjectInterface * thePluginObject) {
	if (NULL != mInLeftGainControl) {
		mCurrentInputPlugin->setProperty(kPluginPListLeftGain, mInLeftGainControl->getValue ());
	}
	if (NULL != mInRightGainControl) {
		mCurrentInputPlugin->setProperty(kPluginPListRightGain, mInRightGainControl->getValue ());
	}
	return;
}

IOReturn AppleOnboardAudio::createOutputVolumeControls (void) {
	AudioHardwareObjectInterface *		thePluginObject;
	OSArray *							controlsArray;
	OSString *							controlString;
	char *								selectedOutput;
	IOReturn							result;
	UInt32								curSelection;
	UInt32								count;
	UInt32								index;

	debugIOLog ("+ AppleOnboardAudio::createOutputVolumeControls\n");

	result = kIOReturnError;
	FailIf (NULL == mOutputSelector, Exit);
	curSelection = mOutputSelector->getIntValue ();
	
	selectedOutput = getConnectionKeyFromCharCode(curSelection, kIOAudioStreamDirectionOutput);

	thePluginObject = getPluginObjectForConnection (selectedOutput);
	FailIf (NULL == thePluginObject, Exit);

	AdjustOutputVolumeControls (thePluginObject, curSelection);

	controlsArray = getControlsArray (selectedOutput);
	FailIf (NULL == controlsArray, Exit);
	count = controlsArray->getCount ();
	for (index = 0; index < count; index++) {
		controlString = OSDynamicCast (OSString, controlsArray->getObject (index));
		if (controlString->isEqualTo (kMuteControlString)) {
			mOutMuteControl = IOAudioToggleControl::createMuteControl (false, kIOAudioControlChannelIDAll, kIOAudioControlChannelNameAll, 0, kIOAudioControlUsageOutput);
			if (NULL != mOutMuteControl) {
				mDriverDMAEngine->addDefaultAudioControl(mOutMuteControl);
				mOutMuteControl->setValueChangeHandler((IOAudioControl::IntValueChangeHandler)outputControlChangeHandler, this);
				// Don't release it because we might reference it later
			}
			break;		// out of for loop now that we have created the mute control
		}
	}

Exit:

	debugIOLog ("- AppleOnboardAudio::createOutputVolumeControls\n");
	return result;
}

IOReturn AppleOnboardAudio::createDefaultControls () {
	AudioHardwareObjectInterface *		thePluginObject;
    OSDictionary *						AOAprop;
	OSBoolean *							clockSelectBoolean;
	OSArray *							inputListArray;
	UInt32								index;
	UInt32								count;
    IOReturn							result;
	Boolean								hasPlaythrough;
	Boolean								hasInput;
	
	debugIOLog("+ AppleOnboardAudio::createDefaultControls\n");

	hasPlaythrough = FALSE;
	result = kIOReturnError;
	FailIf (NULL == mDriverDMAEngine, Exit);

	count = mPluginObjects->getCount ();
	for (index = 0; index < count; index++) {
		thePluginObject = getIndexedPluginObject (index);
		FailIf (NULL == thePluginObject, Exit);
		AOAprop = OSDynamicCast (OSDictionary, thePluginObject->getProperty (kPluginPListAOAAttributes));
		FailIf (NULL == AOAprop, Exit);
	}

	createOutputSelectorControl ();

	// The call to createOutputVolumeControls has to come after the creation of the selector control because
	// it references the current value of the selector control to know what controls to create.
	createOutputVolumeControls ();

	mPRAMVolumeControl = IOAudioLevelControl::create (PRAMToVolumeValue (), 0, 7, 0x00120000, 0, kIOAudioControlChannelIDAll, "Boot beep volume", 0, kIOAudioLevelControlSubTypePRAMVolume, kIOAudioControlUsageOutput);
	if (NULL != mPRAMVolumeControl) {
		mDriverDMAEngine->addDefaultAudioControl (mPRAMVolumeControl);
		mPRAMVolumeControl->setValueChangeHandler ((IOAudioControl::IntValueChangeHandler)outputControlChangeHandler, this);
		mPRAMVolumeControl->release ();
		mPRAMVolumeControl = NULL;
	}

	// Create a toggle control for reporting the status of the headphone jack
	mHeadphoneConnected = IOAudioToggleControl::create (FALSE, kIOAudioControlChannelIDAll, kIOAudioControlChannelNameAll, 0, kIOAudioControlTypeJack, kIOAudioControlUsageOutput);

	if (NULL != mHeadphoneConnected) {
		mDriverDMAEngine->addDefaultAudioControl (mHeadphoneConnected);
		// no value change handler because this isn't a settable control
		// Don't release it because we might reference it later
	}

	inputListArray = OSDynamicCast (OSArray, getLayoutEntry (kInputsList));
	hasInput = (NULL != inputListArray);
	
	if (hasInput) {
		createInputSelectorControl ();
	
		createInputGainControls ();
	}

	clockSelectBoolean = OSDynamicCast ( OSBoolean, getLayoutEntry (kExternalClockSelect) );
	if (NULL != clockSelectBoolean) {
		mExternalClockSelector = IOAudioSelectorControl::create (kClockSourceSelectionInternal, kIOAudioControlChannelIDAll, kIOAudioControlChannelNameAll, 0, kIOAudioSelectorControlSubTypeClockSource, kIOAudioControlUsageInput);		
		FailIf (NULL == mExternalClockSelector, Exit);
		mDriverDMAEngine->addDefaultAudioControl (mExternalClockSelector);
		mExternalClockSelector->setValueChangeHandler ((IOAudioControl::IntValueChangeHandler)inputControlChangeHandler, this);
		mExternalClockSelector->addAvailableSelection (kClockSourceSelectionInternal, kInternalClockString);
		mExternalClockSelector->addAvailableSelection (kClockSourceSelectionExternal, kExternalClockString);
		// don't release, may be used in event of loss of clock lock
	}
  	
	result = kIOReturnSuccess;
Exit:    
    debug2IOLog("- %d = AppleOnboardAudio::createDefaultControls\n", result);
    return result;
}

#pragma mark +IOAUDIO CONTROL HANDLERS
// --------------------------------------------------------------------------
// You either have only a master volume control, or you have both volume controls.
IOReturn AppleOnboardAudio::AdjustOutputVolumeControls (AudioHardwareObjectInterface * thePluginObject, UInt32 selectionCode) {
	IOFixed								mindBVol;
	IOFixed								maxdBVol;
	SInt32								minVolume;
	SInt32								maxVolume;
	Boolean								hasMaster;
	Boolean								hasLeft;
	Boolean								hasRight;

	FailIf (NULL == mDriverDMAEngine, Exit);

	mindBVol = thePluginObject->getMinimumdBVolume ();
	maxdBVol = thePluginObject->getMaximumdBVolume ();
	minVolume = thePluginObject->getMinimumVolume ();
	maxVolume = thePluginObject->getMaximumVolume ();

	debug5IOLog ("AppleOnboardAudio::AdjustOutputVolumeControls - mindBVol %lX, maxdBVol %lX, minVolume %ld, maxVolume %ld\n", mindBVol, maxdBVol, minVolume, maxVolume);

	mDriverDMAEngine->pauseAudioEngine ();
	mDriverDMAEngine->beginConfigurationChange ();

	hasMaster = hasMasterVolumeControl (selectionCode);
	hasLeft = hasLeftVolumeControl (selectionCode);
	hasRight = hasRightVolumeControl (selectionCode);

	if ((TRUE == hasMaster && NULL == mOutMasterVolumeControl) || (FALSE == hasMaster && NULL != mOutMasterVolumeControl) ||
		(NULL != mOutMasterVolumeControl && mOutMasterVolumeControl->getMinValue () != minVolume) ||
		(NULL != mOutMasterVolumeControl && mOutMasterVolumeControl->getMaxValue () != maxVolume)) {

		if (TRUE == hasMaster) {
			// We have only the master volume control (possibly not created yet) and have to remove the other volume controls (possibly don't exist)
			if (NULL != mOutMasterVolumeControl) {
				mOutMasterVolumeControl->setMinValue (minVolume);
				mOutMasterVolumeControl->setMinDB (mindBVol);
				mOutMasterVolumeControl->setMaxValue (maxVolume);
				mOutMasterVolumeControl->setMaxDB (maxdBVol);
				if (mOutMasterVolumeControl->getIntValue () > maxVolume) {
					mOutMasterVolumeControl->setValue (maxVolume);
				}
				mOutMasterVolumeControl->flushValue ();
			} else {				
				createMasterVolumeControl (mindBVol, maxdBVol, minVolume, maxVolume);
			}
		} else {
			removeMasterVolumeControl();
		}
	}

	if ((TRUE == hasLeft && NULL == mOutLeftVolumeControl) || (FALSE == hasLeft && NULL != mOutLeftVolumeControl) ||
		(NULL != mOutLeftVolumeControl && mOutLeftVolumeControl->getMinValue () != minVolume) ||
		(NULL != mOutLeftVolumeControl && mOutLeftVolumeControl->getMaxValue () != maxVolume)) {
		if (TRUE == hasLeft) {
			if (NULL != mOutLeftVolumeControl) {
				mOutLeftVolumeControl->setMinValue (minVolume);
				mOutLeftVolumeControl->setMinDB (mindBVol);
				mOutLeftVolumeControl->setMaxValue (maxVolume);
				mOutLeftVolumeControl->setMaxDB (maxdBVol);
				if (mOutLeftVolumeControl->getIntValue () > maxVolume) {
					mOutLeftVolumeControl->setValue (maxVolume);
				}
				mOutLeftVolumeControl->flushValue ();
			} else {
				createLeftVolumeControl(mindBVol, maxdBVol, minVolume, maxVolume);
			}
		} else {
			removeLeftVolumeControl();
		}
	}

	if ((TRUE == hasRight && NULL == mOutRightVolumeControl) || (FALSE == hasRight && NULL != mOutRightVolumeControl) ||
		(NULL != mOutRightVolumeControl && mOutRightVolumeControl->getMinValue () != minVolume) ||
		(NULL != mOutRightVolumeControl && mOutRightVolumeControl->getMaxValue () != maxVolume)) {
		if (TRUE == hasRight) {
			if (NULL != mOutRightVolumeControl) {
				mOutRightVolumeControl->setMinValue (minVolume);
				mOutRightVolumeControl->setMinDB (mindBVol);
				mOutRightVolumeControl->setMaxValue (maxVolume);
				mOutRightVolumeControl->setMaxDB (maxdBVol);
				if (mOutRightVolumeControl->getIntValue () > maxVolume) {
					mOutRightVolumeControl->setValue (maxVolume);
				}
				mOutRightVolumeControl->flushValue ();
			} else {
				createRightVolumeControl(mindBVol, maxdBVol, minVolume, maxVolume);
			}
		} else {
			removeRightVolumeControl ();
		}
	}

	mDriverDMAEngine->completeConfigurationChange ();
	mDriverDMAEngine->resumeAudioEngine ();

Exit:
	return kIOReturnSuccess;
}

// --------------------------------------------------------------------------
// You either have only a master volume control, or you have both volume controls.
IOReturn AppleOnboardAudio::AdjustInputGainControls (AudioHardwareObjectInterface * thePluginObject) {
	IOFixed								mindBGain;
	IOFixed								maxdBGain;
	SInt32								minGain;
	SInt32								maxGain;

	FailIf (NULL == mDriverDMAEngine, Exit);

	mindBGain = thePluginObject->getMinimumdBGain ();
	maxdBGain = thePluginObject->getMaximumdBGain ();
	minGain = thePluginObject->getMinimumGain ();
	maxGain = thePluginObject->getMaximumGain ();

	debug5IOLog ("AppleOnboardAudio::AdjustInputGainControls - mindBGain %lX, maxdBGain %lX, minGain %ld, maxGain %ld\n", mindBGain, maxdBGain, minGain, maxGain);

	mDriverDMAEngine->pauseAudioEngine ();
	mDriverDMAEngine->beginConfigurationChange ();

	removePlayThruControl ();
	createPlayThruControl ();

	if (mUseInputGainControls) {
		debugIOLog ("AdjustInputGainControls - creating input gain controls.\n");
		// or we have both controls (possibly not created yet) and we have to remove the master volume control (possibly doesn't exist)
		if (NULL != mInLeftGainControl) {
			mInLeftGainControl->setMinValue (minGain);
			mInLeftGainControl->setMinDB (mindBGain);
			mInLeftGainControl->setMaxValue (maxGain);
			mInLeftGainControl->setMaxDB (maxdBGain);
			if (mInLeftGainControl->getIntValue () > maxGain) {
				mInLeftGainControl->setValue (maxGain);
			}
			mInLeftGainControl->flushValue ();
		} else {
			createLeftGainControl(mindBGain, maxdBGain, minGain, maxGain);
		}
	
		if (NULL != mInRightGainControl) {
			mInRightGainControl->setMinValue (minGain);
			mInRightGainControl->setMinDB (mindBGain);
			mInRightGainControl->setMaxValue (maxGain);
			mInRightGainControl->setMaxDB (maxdBGain);
			if (mInRightGainControl->getIntValue () > maxGain) {
				mInRightGainControl->setValue (maxGain);
			}
			mInRightGainControl->flushValue ();
		} else {
			createRightGainControl(mindBGain, maxdBGain, minGain, maxGain);
		}
	} else {
		debugIOLog ("AdjustInputGainControls - removing input gain controls.\n");
		removeLeftGainControl();
		removeRightGainControl();
	}

	mDriverDMAEngine->completeConfigurationChange ();
	mDriverDMAEngine->resumeAudioEngine ();

Exit:
	return kIOReturnSuccess;
}

IORegistryEntry * AppleOnboardAudio::FindEntryByNameAndProperty (const IORegistryEntry * start, const char * name, const char * key, UInt32 value) {
	OSIterator				*iterator;
	IORegistryEntry			*theEntry;
	IORegistryEntry			*tmpReg;
	OSNumber				*tmpNumber;

	theEntry = NULL;
	iterator = NULL;
	FAIL_IF (NULL == start, Exit);

	iterator = start->getChildIterator (gIOServicePlane);
	FAIL_IF (NULL == iterator, Exit);

	while (NULL == theEntry && (tmpReg = OSDynamicCast (IORegistryEntry, iterator->getNextObject ())) != NULL) {
		if (strcmp (tmpReg->getName (), name) == 0) {
			tmpNumber = OSDynamicCast (OSNumber, tmpReg->getProperty (key));
			if (NULL != tmpNumber && tmpNumber->unsigned32BitValue () == value) {
				theEntry = tmpReg;
				// remove retain?
				//theEntry->retain();
			}
		}
	}

Exit:
	if (NULL != iterator) {
		iterator->release ();
	}
	return theEntry;
}

void AppleOnboardAudio::createLeftVolumeControl (IOFixed mindBVol, IOFixed maxdBVol, SInt32 minVolume, SInt32 maxVolume) {
	OSNumber * 						theNumber;
	SInt32							leftVol;
	
	theNumber = OSDynamicCast(OSNumber, mCurrentOutputPlugin->getProperty(kPluginPListLeftVol));
	if (NULL == theNumber) {
		leftVol = maxVolume / 2;
	} else {
		leftVol = theNumber->unsigned32BitValue();
	}
	
	debug2IOLog("createLeftVolumeControl - leftVol = %ld\n", leftVol);

	mOutLeftVolumeControl = IOAudioLevelControl::createVolumeControl (leftVol, minVolume, maxVolume, mindBVol, maxdBVol,
										kIOAudioControlChannelIDDefaultLeft,
										kIOAudioControlChannelNameLeft,
										0,
										kIOAudioControlUsageOutput);
	if (NULL != mOutLeftVolumeControl) {
		mDriverDMAEngine->addDefaultAudioControl (mOutLeftVolumeControl);
		mOutLeftVolumeControl->setValueChangeHandler ((IOAudioControl::IntValueChangeHandler)outputControlChangeHandler, this);
		mOutLeftVolumeControl->flushValue ();
	}
}

void AppleOnboardAudio::createRightVolumeControl (IOFixed mindBVol, IOFixed maxdBVol, SInt32 minVolume, SInt32 maxVolume) {
	OSNumber * 						theNumber;
	SInt32							rightVol;
	
	theNumber = OSDynamicCast(OSNumber, mCurrentOutputPlugin->getProperty(kPluginPListRightVol));
	if (NULL == theNumber) {
		rightVol = maxVolume / 2;
	} else {
		rightVol = theNumber->unsigned32BitValue();
	}
	
	debug2IOLog("createRightVolumeControl - rightVol = %ld\n", rightVol);

	mOutRightVolumeControl = IOAudioLevelControl::createVolumeControl (rightVol, minVolume, maxVolume, mindBVol, maxdBVol,
										kIOAudioControlChannelIDDefaultRight,
										kIOAudioControlChannelNameRight,
										0,
										kIOAudioControlUsageOutput);
	if (NULL != mOutRightVolumeControl) {
		mDriverDMAEngine->addDefaultAudioControl (mOutRightVolumeControl);
		mOutRightVolumeControl->setValueChangeHandler ((IOAudioControl::IntValueChangeHandler)outputControlChangeHandler, this);
		mOutRightVolumeControl->flushValue ();
	}
}

void AppleOnboardAudio::createMasterVolumeControl (IOFixed mindBVol, IOFixed maxdBVol, SInt32 minVolume, SInt32 maxVolume) {
	OSNumber * 						theNumber;
	SInt32							masterVol;
	
	theNumber = OSDynamicCast(OSNumber, mCurrentOutputPlugin->getProperty(kPluginPListRightVol));
	if (NULL == theNumber) {
		masterVol = maxVolume;
	} else {
		masterVol = theNumber->unsigned32BitValue();
	}
	theNumber = OSDynamicCast(OSNumber, mCurrentOutputPlugin->getProperty(kPluginPListLeftVol));
	if (NULL != theNumber) {
		masterVol += theNumber->unsigned32BitValue();
	}
	masterVol >>= 1;
	
	debug2IOLog("createMasterVolumeControl - masterVol = %ld\n", masterVol);
	
	mOutMasterVolumeControl = IOAudioLevelControl::createVolumeControl (masterVol, minVolume, maxVolume, mindBVol, maxdBVol,
																		kIOAudioControlChannelIDAll,
																		kIOAudioControlChannelNameAll,
																		0, 
																		kIOAudioControlUsageOutput);

	if (NULL != mOutMasterVolumeControl) {
		mDriverDMAEngine->addDefaultAudioControl(mOutMasterVolumeControl);
		mOutMasterVolumeControl->setValueChangeHandler((IOAudioControl::IntValueChangeHandler)outputControlChangeHandler, this);
		mOutMasterVolumeControl->flushValue ();
	}

}


void AppleOnboardAudio::createLeftGainControl (IOFixed mindBGain, IOFixed maxdBGain, SInt32 minGain, SInt32 maxGain) {
	OSNumber * 						theNumber;
	SInt32							leftGain;
	
	theNumber = OSDynamicCast(OSNumber, mCurrentInputPlugin->getProperty("left-gain"));
	if (NULL == theNumber) {
		leftGain = 0;
	} else {
		leftGain = theNumber->unsigned32BitValue();
	}
	
	debug2IOLog("createLeftGainControl - leftVol = %ld\n", leftGain);

	mInLeftGainControl = IOAudioLevelControl::createVolumeControl (leftGain, minGain, maxGain, mindBGain, maxdBGain,
										kIOAudioControlChannelIDDefaultLeft,
										kIOAudioControlChannelNameLeft,
										0,
										kIOAudioControlUsageInput);
	if (NULL != mInLeftGainControl) {
		mDriverDMAEngine->addDefaultAudioControl (mInLeftGainControl);
		mInLeftGainControl->setValueChangeHandler ((IOAudioControl::IntValueChangeHandler)inputControlChangeHandler, this);
		mInLeftGainControl->flushValue ();
	}
}

void AppleOnboardAudio::createRightGainControl (IOFixed mindBGain, IOFixed maxdBGain, SInt32 minGain, SInt32 maxGain) {
	OSNumber * 						theNumber;
	SInt32							rightGain;
	
	theNumber = OSDynamicCast(OSNumber, mCurrentInputPlugin->getProperty("right-gain"));
	if (NULL == theNumber) {
		rightGain = 0;
	} else {
		rightGain = theNumber->unsigned32BitValue();
	}
	
	debug2IOLog("createRightGainControl - rightVol = %ld\n", rightGain);

	mInRightGainControl = IOAudioLevelControl::createVolumeControl (rightGain, minGain, maxGain, mindBGain, maxdBGain,
										kIOAudioControlChannelIDDefaultRight,
										kIOAudioControlChannelNameRight,
										0,
										kIOAudioControlUsageInput);
	if (NULL != mInRightGainControl) {
		mDriverDMAEngine->addDefaultAudioControl (mInRightGainControl);
		mInRightGainControl->setValueChangeHandler ((IOAudioControl::IntValueChangeHandler)inputControlChangeHandler, this);
		mInRightGainControl->flushValue ();
	}
}

void AppleOnboardAudio::removeLeftVolumeControl() {
	if (NULL != mOutLeftVolumeControl) {
		mDriverDMAEngine->removeDefaultAudioControl (mOutLeftVolumeControl);
		mOutLeftVolumeControl = NULL;
	}
}

void AppleOnboardAudio::removeRightVolumeControl() {
	if (NULL != mOutRightVolumeControl) {
		mDriverDMAEngine->removeDefaultAudioControl (mOutRightVolumeControl);
		mOutRightVolumeControl = NULL;
	}
}

void AppleOnboardAudio::removeMasterVolumeControl() {
	if (NULL != mOutMasterVolumeControl) {
		mDriverDMAEngine->removeDefaultAudioControl (mOutMasterVolumeControl);
		mOutMasterVolumeControl = NULL;
	}
}

void AppleOnboardAudio::removeLeftGainControl() {
	if (NULL != mInLeftGainControl) {
		mDriverDMAEngine->removeDefaultAudioControl (mInLeftGainControl);
		mInLeftGainControl = NULL;
	}
}

void AppleOnboardAudio::removeRightGainControl() {
	if (NULL != mInRightGainControl) {
		mDriverDMAEngine->removeDefaultAudioControl (mInRightGainControl);
		mInRightGainControl = NULL;
	}
}

IOReturn AppleOnboardAudio::outputControlChangeHandler (IOService *target, IOAudioControl *control, SInt32 oldValue, SInt32 newValue) {
	IOReturn						result = kIOReturnError;
	AppleOnboardAudio *				audioDevice;
	IOAudioLevelControl *			levelControl;
	IODTPlatformExpert * 			platform;
	UInt32							leftVol;
	UInt32							rightVol;
	Boolean							wasPoweredDown;

	debug5IOLog ("+ AppleOnboardAudio::outputControlChangeHandler (%p, %p, %lX, %lX)\n", target, control, oldValue, newValue);

	audioDevice = OSDynamicCast (AppleOnboardAudio, target);
	FailIf (NULL == audioDevice, Exit);

	// We have to make sure the hardware is on before we can send it any control changes [2981190]
	if (kIOAudioDeviceSleep == audioDevice->ourPowerState) {
		audioDevice->ourPowerState = kIOAudioDeviceActive;
		if (NULL != audioDevice->mOutMuteControl) {
			audioDevice->mOutMuteControl->flushValue ();					// Restore hardware to the user's selected state
		}
		wasPoweredDown = TRUE;
	} else {
		wasPoweredDown = FALSE;
	}

	switch (control->getType ()) {
		case kIOAudioControlTypeLevel:
			switch (control->getSubType ()) {
				case kIOAudioLevelControlSubTypeVolume:
					levelControl = OSDynamicCast (IOAudioLevelControl, control);
		
					switch (control->getChannelID ()) {
						case kIOAudioControlChannelIDAll:
							result = audioDevice->volumeMasterChange (newValue);
							if (newValue == levelControl->getMinValue ()) {
								// If it's set to it's min, then it's mute, so tell the HAL it's muted
								OSNumber *			muteState;
								muteState = OSNumber::withNumber ((long long unsigned int)1, 32);
								if (NULL != audioDevice->mOutMuteControl) {
									audioDevice->mOutMuteControl->hardwareValueChanged (muteState);
									result = audioDevice->outputMuteChange (1);
								}
							} else if (oldValue == levelControl->getMinValue () && FALSE == audioDevice->mIsMute) {
								OSNumber *			muteState;
								muteState = OSNumber::withNumber ((long long unsigned int)0, 32);
								if (NULL != audioDevice->mOutMuteControl) {
									audioDevice->mOutMuteControl->hardwareValueChanged (muteState);
									result = audioDevice->outputMuteChange (0);
								}
							}
							break;
						case kIOAudioControlChannelIDDefaultLeft:
							result = audioDevice->volumeLeftChange (newValue);
							break;
						case kIOAudioControlChannelIDDefaultRight:
							result = audioDevice->volumeRightChange (newValue);
							break;
					}
					break;
				case kIOAudioLevelControlSubTypePRAMVolume:
					platform = OSDynamicCast (IODTPlatformExpert, getPlatform());
					if (platform) {
						UInt8 							curPRAMVol;
		
						result = platform->readXPRAM ((IOByteCount)kPRamVolumeAddr, &curPRAMVol, (IOByteCount)1);
						curPRAMVol = (curPRAMVol & 0xF8) | newValue;
						result = platform->writeXPRAM ((IOByteCount)kPRamVolumeAddr, &curPRAMVol, (IOByteCount) 1);
					}
					break;
				default:
					result = kIOReturnBadArgument;
			}
			break;
		case kIOAudioControlTypeToggle:
			result = audioDevice->outputMuteChange (newValue);
			break;
		case kIOAudioControlTypeSelector:
			result = audioDevice->outputSelectorChanged (newValue);
			break;
		default:
			;
	}

	if (control->getSubType () == kIOAudioLevelControlSubTypeVolume) {
		levelControl = OSDynamicCast (IOAudioLevelControl, control);
		if (audioDevice->mOutRightVolumeControl && audioDevice->mOutLeftVolumeControl) {
			if (audioDevice->mOutRightVolumeControl->getMinValue () == audioDevice->mVolRight &&
				audioDevice->mOutLeftVolumeControl->getMinValue () == audioDevice->mVolLeft) {
				// If it's set to it's min, then it's mute, so tell the HAL it's muted
				OSNumber *			muteState;
				muteState = OSNumber::withNumber ((long long unsigned int)1, 32);
				if (NULL != audioDevice->mOutMuteControl) {
					audioDevice->mOutMuteControl->hardwareValueChanged (muteState);
				}
			} else if (newValue != levelControl->getMinValue () && oldValue == levelControl->getMinValue () && FALSE == audioDevice->mIsMute) {
				OSNumber *			muteState;
				muteState = OSNumber::withNumber ((long long unsigned int)0, 32);
				if (NULL != audioDevice->mOutMuteControl) {
					audioDevice->mOutMuteControl->hardwareValueChanged (muteState);
				}
			}
		}
	}

	if (audioDevice->mIsMute) {
		leftVol = 0;
		rightVol = 0;
	} else {
		leftVol = audioDevice->mVolLeft;
		rightVol = audioDevice->mVolRight;
	}

	if (TRUE == audioDevice->mAutoUpdatePRAM) {				// We do that only if we are on a OS 9 like UI guideline
		audioDevice->WritePRAMVol (leftVol, rightVol);
	}

Exit:
	// Second half of [2981190], put us back to sleep after the right amount of time if we were off
	if (TRUE == wasPoweredDown) {
		audioDevice->setTimerForSleep ();
	}
	debug6IOLog ("- AppleOnboardAudio::outputControlChangeHandler (%p, %p, %lX, %lX) returns %X\n", target, control, oldValue, newValue, result);

	return result;
}

void AppleOnboardAudio::createPlayThruControl (void) {
	OSDictionary *						AOAprop;

	AOAprop = OSDynamicCast (OSDictionary, mCurrentInputPlugin->getProperty (kPluginPListAOAAttributes));
	FailIf (NULL == AOAprop, Exit);

	if (kOSBooleanTrue == OSDynamicCast (OSBoolean, AOAprop->getObject ("Playthrough"))) {
		mPlaythruToggleControl = IOAudioToggleControl::createMuteControl (TRUE, kIOAudioControlChannelIDAll, kIOAudioControlChannelNameAll, 0, kIOAudioControlUsagePassThru);

		if (NULL != mPlaythruToggleControl) {
			mDriverDMAEngine->addDefaultAudioControl (mPlaythruToggleControl);
			mPlaythruToggleControl->setValueChangeHandler ((IOAudioControl::IntValueChangeHandler)inputControlChangeHandler, this);
			// Don't release it because we might reference it later
		}
	}

Exit:
	return;
}

void AppleOnboardAudio::removePlayThruControl (void) {
	if (NULL != mPlaythruToggleControl) {
		mDriverDMAEngine->removeDefaultAudioControl (mPlaythruToggleControl);
		mPlaythruToggleControl->release ();
		mPlaythruToggleControl = NULL;
	}
}

IOReturn AppleOnboardAudio::outputSelectorChanged (SInt32 newValue) {
	AudioHardwareObjectInterface *		thePluginObject;
	IOAudioStream *						outputStream;
	OSDictionary *						AOApropInput;
	OSDictionary *						AOApropOutput;
	OSNumber *							inputLatencyNumber;
	OSNumber *							outputLatencyNumber;
	char *								connectionString;
	IOReturn							result;
	UInt32								inputLatency;
	UInt32								outputLatency;

	debug2IOLog ("+ AppleOnboardAudio::outputSelectorChanged(%4s)\n", (char *)&newValue);

	result = kIOReturnError;
	inputLatency = 0;
	outputLatency = 0;
	
	connectionString = getConnectionKeyFromCharCode (newValue, kIOAudioStreamDirectionOutput);
	FailIf (0 == connectionString, Exit);

	thePluginObject = getPluginObjectForConnection (connectionString);
	FailIf (NULL == thePluginObject, Exit);

	outputStream = mDriverDMAEngine->getAudioStream (kIOAudioStreamDirectionOutput, 1);
	FailIf (NULL == outputStream, Exit);
	// FIX  Make this be set to the correct type based on the selection
	outputStream->setTerminalType (OUTPUT_SPEAKER);

	cacheOutputVolumeLevels (mCurrentOutputPlugin);
	setClipRoutineForOutput (connectionString);
	
	if (mCurrentOutputPlugin != thePluginObject) {
		mCurrentOutputPlugin = thePluginObject;

		selectAmplifier (newValue);

		setHardwareDSP (connectionString);		
		setSoftwareOutputDSP (connectionString);

		AdjustOutputVolumeControls (mCurrentOutputPlugin, newValue);

		AOApropInput = OSDynamicCast (OSDictionary, mCurrentInputPlugin->getProperty (kPluginPListAOAAttributes));
		if (NULL != AOApropInput) {
			inputLatencyNumber = OSDynamicCast (OSNumber, AOApropInput->getObject (kPluginPListInputLatency));
			if (NULL != inputLatencyNumber) {
				inputLatency = inputLatencyNumber->unsigned32BitValue();
			}
		}

		AOApropOutput = OSDynamicCast (OSDictionary, mCurrentOutputPlugin->getProperty (kPluginPListAOAAttributes));
		if (NULL != AOApropOutput) {
			outputLatencyNumber = OSDynamicCast (OSNumber, AOApropOutput->getObject (kPluginPListOutputLatency));
			if (NULL != outputLatencyNumber) {
				outputLatency = outputLatencyNumber->unsigned32BitValue();
			}
		}
		
		mDriverDMAEngine->beginConfigurationChange();
	
		mDriverDMAEngine->setSampleLatencies (outputLatency, inputLatency);

		mDriverDMAEngine->completeConfigurationChange();
	} else {
		mCurrentOutputPlugin->prepareForOutputChange();

		selectAmplifier(newValue);

		setHardwareDSP (connectionString);
		setSoftwareOutputDSP (connectionString);

		AdjustOutputVolumeControls(mCurrentOutputPlugin, newValue);
	}

	result = kIOReturnSuccess;

Exit:
	debug2IOLog ("- AppleOnboardAudio::outputSelectorChanged returns %X\n", result);
	return result;
}

// This is called when we're on hardware that only has one active volume control (either right or left)
// otherwise the respective right or left volume handler will be called.
// This calls both volume handers becasue it doesn't know which one is really the active volume control.
IOReturn AppleOnboardAudio::volumeMasterChange (SInt32 newValue) {
	IOReturn						result = kIOReturnSuccess;

//	debugIOLog("+ AppleOnboardAudio::volumeMasterChange\n");

	result = kIOReturnError;

	// Don't know which volume control really exists, so adjust both -- they'll ignore the change if they don't exist
	result = volumeLeftChange (newValue);
	result = volumeRightChange (newValue);

	result = kIOReturnSuccess;

//	debug2IOLog ("- AppleOnboardAudio::volumeMasterChange, 0x%x\n", result);
	return result;
}

IOReturn AppleOnboardAudio::volumeLeftChange (SInt32 newValue) {
	IOReturn							result;

//	debug2IOLog ("+ AppleOnboardAudio::volumeLeftChange (%ld)\n", newValue);

	mCurrentOutputPlugin->setVolume (newValue, mVolRight);

	mVolLeft = newValue;

	result = kIOReturnSuccess;

//	debug2IOLog("- AppleOnboardAudio::volumeLeftChange, 0x%x\n", result);
	return result;
}

IOReturn AppleOnboardAudio::volumeRightChange (SInt32 newValue) {
	IOReturn							result;

//	debug2IOLog ("+ AppleOnboardAudio::volumeRightChange (%ld)\n", newValue);

	mCurrentOutputPlugin->setVolume (mVolLeft, newValue);

	mVolRight = newValue;

	result = kIOReturnSuccess;

//	debug2IOLog ("- AppleOnboardAudio::volumeRightChange, result = 0x%x\n", result);
	return result;
}

IOReturn AppleOnboardAudio::outputMuteChange (SInt32 newValue) {
	AudioHardwareObjectInterface *		thePluginObject;
	IOReturn							result;
	UInt32								index;
	UInt32								count;

//    debug2IOLog ("+ AppleOnboardAudio::outputMuteChange (%ld)\n", newValue);

    result = kIOReturnError;

	FailIf (NULL == mPluginObjects, Exit);
	count = mPluginObjects->getCount ();
	for (index = 0; index < count; index++) {
		thePluginObject = getIndexedPluginObject (index);
		FailIf (NULL == thePluginObject, Exit);
		thePluginObject->setMute (newValue);
	}
	mIsMute = newValue;

	result = kIOReturnSuccess;
Exit:
//    debug2IOLog ("- AppleOnboardAudio::outputMuteChange, 0x%x\n", result);
    return result;
}

IOReturn AppleOnboardAudio::inputControlChangeHandler (IOService *target, IOAudioControl *control, SInt32 oldValue, SInt32 newValue) {
	IOReturn						result = kIOReturnError;
	AppleOnboardAudio *				audioDevice;
	IOAudioLevelControl *			levelControl;
	Boolean							wasPoweredDown;

	debug5IOLog ("+ AppleOnboardAudio::inputControlChangeHandler (%p, %p, %lX, %lX)\n", target, control, oldValue, newValue);

	audioDevice = OSDynamicCast (AppleOnboardAudio, target);
	FailIf (NULL == audioDevice, Exit);

	// We have to make sure the hardware is on before we can send it any control changes [2981190]
	if (kIOAudioDeviceSleep == audioDevice->ourPowerState) {
		audioDevice->ourPowerState = kIOAudioDeviceActive;
		if (NULL != audioDevice->mOutMuteControl) {
			audioDevice->mOutMuteControl->flushValue ();					// Restore hardware to the user's selected state
		}
		wasPoweredDown = TRUE;
	} else {
		wasPoweredDown = FALSE;
	}

	switch (control->getType ()) {
		case kIOAudioControlTypeLevel:
			//switch (control->getSubType ()) {
			//	case kIOAudioLevelControlSubTypeVolume:
					levelControl = OSDynamicCast (IOAudioLevelControl, control);
		
					switch (control->getChannelID ()) {
						case kIOAudioControlChannelIDDefaultLeft:
							result = audioDevice->gainLeftChanged (newValue);
							break;
						case kIOAudioControlChannelIDDefaultRight:
							result = audioDevice->gainRightChanged (newValue);
							break;
					}
					break;
			//}
			//break;
		case kIOAudioControlTypeToggle:
			//switch (control->getSubType ()) {
			//	case kIOAudioToggleControlSubTypeMute:
					result = audioDevice->passThruChanged (newValue);
			//		break;
			//}
			break;
		case kIOAudioControlTypeSelector:
			debugIOLog ("input selector change handler\n");
			switch (control->getSubType ()) {
				case kIOAudioSelectorControlSubTypeInput:
					result = audioDevice->inputSelectorChanged (newValue);
					break;
				case kIOAudioSelectorControlSubTypeClockSource:
					result = audioDevice->clockSelectorChanged (newValue);
					break;
				default:
					break;
			}		
		default:
			;
	}

Exit:
	// Second half of [2981190], put us back to sleep after the right amount of time if we were off
	if (TRUE == wasPoweredDown) {
		audioDevice->setTimerForSleep ();
	}

	return result;
}

IOReturn AppleOnboardAudio::gainLeftChanged (SInt32 newValue) {
	IOReturn							result;

    debugIOLog ("+ AppleOnboardAudio::gainLeftChanged\n");    

	if (mCurrentPluginHasSoftwareInputGain) {
		mDriverDMAEngine->setInputGainL (newValue);
	} else {
		mCurrentInputPlugin->setInputGain (newValue, mGainRight);
	}
    mGainLeft = newValue;

	result = kIOReturnSuccess;

    debug2IOLog ("- AppleOnboardAudio::gainLeftChanged, %d\n", (result == kIOReturnSuccess));
    return result;
}

IOReturn AppleOnboardAudio::gainRightChanged (SInt32 newValue) {
	IOReturn							result;

    debugIOLog ("+ AppleOnboardAudio::gainRightChanged\n");    

	if (mCurrentPluginHasSoftwareInputGain) {
		mDriverDMAEngine->setInputGainR (newValue);
	} else {
		mCurrentInputPlugin->setInputGain (newValue, mGainRight);
	}
    mGainRight = newValue;

	result = kIOReturnSuccess;

    debug2IOLog ("- AppleOnboardAudio::gainRightChanged, %d\n", (result == kIOReturnSuccess));
    return result;
}

IOReturn AppleOnboardAudio::passThruChanged (SInt32 newValue) {
	IOReturn							result;

    debugIOLog ("+ AppleOnboardAudio::passThruChanged\n");

    result = kIOReturnError;

	mCurrentInputPlugin->setPlayThrough (!newValue);

	result = kIOReturnSuccess;

    debugIOLog ("- AppleOnboardAudio::passThruChanged\n");
    return result;
}

IOReturn AppleOnboardAudio::inputSelectorChanged (SInt32 newValue) {
	AudioHardwareObjectInterface *		thePluginObject;
	IOAudioStream *						inputStream;
	OSDictionary *						AOApropInput;
	OSDictionary *						AOApropOutput;
	OSBoolean *							softwareInputGainBoolean;
	OSNumber *							inputLatencyNumber;
	OSNumber *							outputLatencyNumber;
	char *								connectionString;
	IOReturn							result;
	UInt32								inputLatency;
	UInt32								outputLatency;

	debug2IOLog ("+ AppleOnboardAudio::inputSelectorChanged (%4s)\n", (char *)&newValue);

	result = kIOReturnError;
	inputLatency = 0;
	outputLatency = 0;
	
	connectionString = getConnectionKeyFromCharCode (newValue, kIOAudioStreamDirectionInput);
	FailIf (0 == connectionString, Exit);

	thePluginObject = getPluginObjectForConnection (connectionString);
	FailIf (NULL == thePluginObject, Exit);

	// FIX  Make this change as the selection changes.
	inputStream = mDriverDMAEngine->getAudioStream (kIOAudioStreamDirectionInput, 1);
	FailIf (NULL == inputStream, Exit);
	inputStream->setTerminalType (INPUT_MICROPHONE);

	cacheInputGainLevels (mCurrentInputPlugin);
	setUseInputGainControls (connectionString);

	setClipRoutineForInput (connectionString);

	if (mCurrentInputPlugin != thePluginObject) {
		mCurrentInputPlugin = thePluginObject;
	
		debug2IOLog ("+ AppleOnboardAudio::inputSelectorChanged - mCurrentInputPlugin updated to %p\n", mCurrentInputPlugin);

		mCurrentInputPlugin->setActiveInput (newValue);

		AOApropInput = OSDynamicCast (OSDictionary, mCurrentInputPlugin->getProperty (kPluginPListAOAAttributes));
		if (NULL != AOApropInput) {
			inputLatencyNumber = OSDynamicCast (OSNumber, AOApropInput->getObject (kPluginPListInputLatency));
			if (NULL != inputLatencyNumber) {
				inputLatency = inputLatencyNumber->unsigned32BitValue();
			}
		}
		
		AdjustInputGainControls (mCurrentInputPlugin);

		AOApropOutput = OSDynamicCast (OSDictionary, mCurrentOutputPlugin->getProperty (kPluginPListAOAAttributes));
		if (NULL != AOApropOutput) {
			softwareInputGainBoolean = OSDynamicCast (OSBoolean, AOApropInput->getObject (kPluginPListSoftwareInputGain));
			if (NULL != softwareInputGainBoolean) {
				mDriverDMAEngine->setUseSoftwareInputGain (softwareInputGainBoolean->getValue ());
				mCurrentPluginHasSoftwareInputGain = softwareInputGainBoolean->getValue ();
			} else {
				mDriverDMAEngine->setUseSoftwareInputGain (false);
				mCurrentPluginHasSoftwareInputGain = false;
			}
			outputLatencyNumber = OSDynamicCast (OSNumber, AOApropOutput->getObject (kPluginPListOutputLatency));
			if (NULL != outputLatencyNumber) {
				outputLatency = outputLatencyNumber->unsigned32BitValue();
			}
		}
		
		mDriverDMAEngine->beginConfigurationChange();
	
		mDriverDMAEngine->setSampleLatencies (outputLatency, inputLatency);

		mDriverDMAEngine->completeConfigurationChange();
	} else {
		mCurrentInputPlugin->setActiveInput (newValue);

		AdjustInputGainControls (mCurrentInputPlugin);
	}

	result = kIOReturnSuccess;

Exit:
	debugIOLog ("- AppleOnboardAudio::inputSelectorChanged\n");
	return result;
}


IOReturn AppleOnboardAudio::clockSelectorChanged (SInt32 newValue) {
	IOReturn							result;

	debug2IOLog ("+ AppleOnboardAudio::clockSelectorChanged (%4s)\n", (char *)&newValue);

	result = kIOReturnError;

	if (kClockSourceSelectionInternal == newValue) {
		mTransportInterface->transportBreakClockSelect(kTRANSPORT_MASTER_CLOCK);
		callPluginsInOrder (kBreakClockSelect, kTRANSPORT_MASTER_CLOCK);
		
		mTransportInterface->transportMakeClockSelect(kTRANSPORT_MASTER_CLOCK);
		callPluginsInOrder (kMakeClockSelect, kTRANSPORT_MASTER_CLOCK);
		
	} else if (kClockSourceSelectionExternal == newValue) {
		mTransportInterface->transportBreakClockSelect(kTRANSPORT_SLAVE_CLOCK);
		callPluginsInOrder (kBreakClockSelect, kTRANSPORT_SLAVE_CLOCK);
		
		mTransportInterface->transportMakeClockSelect(kTRANSPORT_SLAVE_CLOCK);
		callPluginsInOrder (kMakeClockSelect, kTRANSPORT_SLAVE_CLOCK);
		
	} else {
		debugIOLog ("Unknown clock source selection.\n");
		FailIf (1, Exit);
	}
	
	result = kIOReturnSuccess;

Exit:
	debugIOLog ("- AppleOnboardAudio::inputSelectorChanged\n");
	return result;
}

UInt32 AppleOnboardAudio::getCurrentSampleFrame (void) {
	return mCurrentOutputPlugin->getCurrentSampleFrame ();
}

void AppleOnboardAudio::setCurrentSampleFrame (UInt32 inValue) {
	return mCurrentOutputPlugin->setCurrentSampleFrame (inValue);
}

#pragma mark +POWER MANAGEMENT
void AppleOnboardAudio::setTimerForSleep () {
    AbsoluteTime				fireTime;
    UInt64						nanos;

	if (idleTimer && idleSleepDelayTime != kNoIdleAudioPowerDown) {
		clock_get_uptime (&fireTime);
		absolutetime_to_nanoseconds (fireTime, &nanos);
		nanos += idleSleepDelayTime;
		nanoseconds_to_absolutetime (nanos, &fireTime);
		idleTimer->wakeAtTime (fireTime);		// will call idleAudioSleepHandlerTimer
	}
}

void AppleOnboardAudio::sleepHandlerTimer (OSObject *owner, IOTimerEventSource *sender) {
	AppleOnboardAudio *				audioDevice;
	UInt32							time = 0;

	audioDevice = OSDynamicCast (AppleOnboardAudio, owner);
	FailIf (NULL == audioDevice, Exit);

	if (audioDevice->getPowerState () != kIOAudioDeviceActive) {
		audioDevice->performPowerStateChange (audioDevice->getPowerState (), kIOAudioDeviceIdle, &time);
	}

Exit:
	return;
}

// Have to call super::setAggressiveness to complete the function call
IOReturn AppleOnboardAudio::setAggressiveness(unsigned long type, unsigned long newLevel) {
	UInt32					time = 0;

	if (type == kPMPowerSource) {
		debugIOLog ("setting power aggressivness state to ");
		switch (newLevel) {
			case kIOPMInternalPower:								// Running on battery only
				debugIOLog ("battery power\n");
				idleSleepDelayTime = kBatteryPowerDownDelayTime;
				setIdleAudioSleepTime (idleSleepDelayTime);
				if (getPowerState () != kIOAudioDeviceActive) {
					performPowerStateChange (getPowerState (), kIOAudioDeviceIdle, &time);
				}
				break;
			case kIOPMExternalPower:								// Running on AC power
				debugIOLog ("wall power\n");
				// idleSleepDelayTime = kACPowerDownDelayTime;		// idle power down after 5 minutes
				idleSleepDelayTime = kNoIdleAudioPowerDown;
				setIdleAudioSleepTime (idleSleepDelayTime);			// don't tell us about going to the idle state
				if (getPowerState () != kIOAudioDeviceActive) {
					performPowerStateChange (getPowerState (), kIOAudioDeviceActive, &time);
				}
				break;
			default:
				break;
		}
	}

	return super::setAggressiveness(type, newLevel);
}

IOReturn AppleOnboardAudio::performPowerStateChange(IOAudioDevicePowerState oldPowerState,
                                                        IOAudioDevicePowerState newPowerState,
                                                        UInt32 *microsecondsUntilComplete)
{
	AudioHardwareObjectInterface *		thePluginObject;
	IOReturn							result;
	UInt32								index;
	UInt32								count;

	debug4IOLog ("+ AppleOnboardAudio::performPowerStateChange (%d, %d) -- ourPowerState = %d\n", oldPowerState, newPowerState, ourPowerState);

	result = super::performPowerStateChange (oldPowerState, newPowerState, microsecondsUntilComplete);

	FailIf (NULL == mPluginObjects, Exit);
	count = mPluginObjects->getCount ();

	switch (newPowerState) {
		case kIOAudioDeviceSleep:
			//	Sleep requires that the hardware plugin object(s) go to sleep
			//	prior to the transport object going to sleep.
			if (kIOAudioDeviceActive == ourPowerState) {
				outputMuteChange (TRUE);			// Mute before turning off power
				for (index = 0; index < count; index++) {
					thePluginObject = getIndexedPluginObject (index);
					FailIf (NULL == thePluginObject, Exit);
					thePluginObject->performDeviceSleep ();
				}
				if ( NULL != mTransportInterface ) { 
					mTransportInterface->performTransportSleep ();
				}
				ourPowerState = kIOAudioDeviceSleep;
			}
			break;
		case kIOAudioDeviceIdle:
			if (kIOAudioDeviceActive == ourPowerState) {
				outputMuteChange (TRUE);			// Mute before turning off power
				//	Sleep requires that the hardware plugin object(s) go to sleep
				//	prior to the transport object going to sleep.
				for (index = 0; index < count; index++) {
					thePluginObject = getIndexedPluginObject (index);
					FailIf (NULL == thePluginObject, Exit);
					thePluginObject->performDeviceSleep ();
				}
				if ( NULL != mTransportInterface ) { 
					mTransportInterface->performTransportSleep ();
				}
				ourPowerState = kIOAudioDeviceSleep;
			} else if (kIOAudioDeviceSleep == ourPowerState && kNoIdleAudioPowerDown == idleSleepDelayTime) {
				//	Wake requires that the transport object go active prior
				//	to the hardware plugin object(s) going active.
				if ( NULL != mTransportInterface ) {
					mTransportInterface->performTransportWake ();
				}
				for (index = 0; index < count; index++) {
					thePluginObject = getIndexedPluginObject (index);
					FailIf (NULL == thePluginObject, Exit);
					thePluginObject->performDeviceWake ();
				}
				ourPowerState = kIOAudioDeviceActive;
				if (NULL != mOutMuteControl) {
					mOutMuteControl->flushValue ();					// Restore hardware to the user's selected state
				}
			}
			break;
		case kIOAudioDeviceActive:
			if (kIOAudioDeviceActive != ourPowerState) {
				//	Wake requires that the transport object go active prior
				//	to the hardware plugin object(s) going active.
				if ( NULL != mTransportInterface ) {
					mTransportInterface->performTransportWake ();
				}
				for (index = 0; index < count; index++) {
					thePluginObject = getIndexedPluginObject (index);
					FailIf (NULL == thePluginObject, Exit);
					thePluginObject->performDeviceWake ();
				}
				ourPowerState = kIOAudioDeviceActive;
				if (NULL != mOutMuteControl) {
					mOutMuteControl->flushValue ();					// Restore hardware to the user's selected state
				}
			} else {
				debugIOLog ("trying to wake, but we're already awake\n");
			}
			break;
		default:
			break;
	}

	debug2IOLog ("- AppleOnboardAudio::performPowerStateChange -- ourPowerState = %d\n", ourPowerState);

Exit:
	return result;
}

//********************************************************************************
//
// Receives a notification when the RootDomain changes state. 
//
// Allows us to take action on system sleep, power down, and restart after
// applications have received their power change notifications and replied,
// but before drivers have powered down. We tell the device to go to sleep for a 
// silent shutdown on P80 and DACA.
//*********************************************************************************
IOReturn AppleOnboardAudio::sysPowerDownHandler (void * target, void * refCon, UInt32 messageType, IOService * provider, void * messageArgument, vm_size_t argSize) {
	AppleOnboardAudio *				appleOnboardAudio;
	IOReturn						result;
//	char							message[100];

	result = kIOReturnUnsupported;
	appleOnboardAudio = OSDynamicCast (AppleOnboardAudio, (OSObject *)target);
	FailIf (NULL == appleOnboardAudio, Exit);

	switch (messageType) {
		case kIOMessageSystemWillPowerOff:
		case kIOMessageSystemWillRestart:
			// Interested applications have been notified of an impending power
			// change and have acked (when applicable).
			// This is our chance to save whatever state we can before powering
			// down.
//			Debugger ("about to shut down the hardware");
			result = kIOReturnSuccess;
			break;
		default:
//			sprintf (message, "unknown selector %lx", messageType);
//			Debugger (message);
			break;
	}

Exit:
	return result;
}

#pragma mark +PRAM VOLUME
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Calculates the PRAM volume value for stereo volume.
UInt8 AppleOnboardAudio::VolumeToPRAMValue (UInt32 inLeftVol, UInt32 inRightVol) {
	UInt32			pramVolume;						// Volume level to store in PRAM
	UInt32 			averageVolume;					// summed volume
    UInt32		 	volumeRange;
    UInt32 			volumeSteps;
	UInt32			leftVol;
	UInt32			rightVol;

	debug3IOLog ( "+ AppleOnboardAudio::VolumeToPRAMValue ( 0x%X, 0x%X )\n", (unsigned int)inLeftVol, (unsigned int)inRightVol );
	pramVolume = 0;											//	[2886446]	Always pass zero as a result when muting!!!
	if ( ( 0 != inLeftVol ) || ( 0 != inRightVol ) ) {		//	[2886446]
		leftVol = inLeftVol;
		rightVol = inRightVol;
		if (NULL != mOutLeftVolumeControl) {
			leftVol -= mOutLeftVolumeControl->getMinValue ();
		}
	
		if (NULL != mOutRightVolumeControl) {
			rightVol -= mOutRightVolumeControl->getMinValue ();
		}
		debug3IOLog ( "... leftVol = 0x%X, rightVol = 0x%X\n", (unsigned int)leftVol, (unsigned int)rightVol );
	
		if (NULL != mOutMasterVolumeControl) {
			volumeRange = (mOutMasterVolumeControl->getMaxValue () - mOutMasterVolumeControl->getMinValue () + 1);
			debug2IOLog ( "... mOutMasterVolumeControl volumeRange = 0x%X\n", (unsigned int)volumeRange );
		} else if (NULL != mOutLeftVolumeControl) {
			volumeRange = (mOutLeftVolumeControl->getMaxValue () - mOutLeftVolumeControl->getMinValue () + 1);
			debug2IOLog ( "... mOutLeftVolumeControl volumeRange = 0x%X\n", (unsigned int)volumeRange );
		} else if (NULL != mOutRightVolumeControl) {
			volumeRange = (mOutRightVolumeControl->getMaxValue () - mOutRightVolumeControl->getMinValue () + 1);
			debug2IOLog ( "... mOutRightVolumeControl volumeRange = 0x%X\n", (unsigned int)volumeRange );
		} else {
			volumeRange = kMaximumPRAMVolume;
			debug2IOLog ( "... volumeRange = 0x%X **** NO AUDIO LEVEL CONTROLS!\n", (unsigned int)volumeRange );
		}

		averageVolume = (leftVol + rightVol) >> 1;		// sum the channel volumes and get an average
		debug2IOLog ( "... averageVolume = 0x%X\n", (unsigned int)volumeRange );
		debug3IOLog ( "... volumeRange %X, kMaximumPRAMVolume %X\n", (unsigned int)volumeRange, (unsigned int)kMaximumPRAMVolume );
		volumeSteps = volumeRange / kMaximumPRAMVolume;	// divide the range by the range of the pramVolume
		pramVolume = averageVolume / volumeSteps;    
	
		// Since the volume level in PRAM is only worth three bits,
		// we round small values up to 1. This avoids SysBeep from
		// flashing the menu bar when it thinks sound is off and
		// should really just be very quiet.
	
		if ((pramVolume == 0) && (leftVol != 0 || rightVol !=0 )) {
			pramVolume = 1;
		}
	
	}
	debug2IOLog ( "- AppleOnboardAudio::VolumeToPRAMValue returns 0x%X\n", (unsigned int)pramVolume );
	return (pramVolume & 0x07);
}

UInt32 AppleOnboardAudio::PRAMToVolumeValue (void) {
	UInt32		 	volumeRange;
	UInt32 			volumeSteps;

	if (NULL != mOutLeftVolumeControl) {
		volumeRange = (mOutLeftVolumeControl->getMaxValue () - mOutLeftVolumeControl->getMinValue () + 1);
	} else if (NULL != mOutRightVolumeControl) {
		volumeRange = (mOutRightVolumeControl->getMaxValue () - mOutRightVolumeControl->getMinValue () + 1);
	} else {
		volumeRange = kMaximumPRAMVolume;
	}

	volumeSteps = volumeRange / kMaximumPRAMVolume;	// divide the range by the range of the pramVolume

	return (volumeSteps * ReadPRAMVol ());
}

void AppleOnboardAudio::WritePRAMVol (UInt32 leftVol, UInt32 rightVol) {
	UInt8						pramVolume;
	UInt8 						curPRAMVol;
	IODTPlatformExpert * 		platform;
	IOReturn					err;
		
	platform = OSDynamicCast(IODTPlatformExpert,getPlatform());
    
    debug3IOLog("+ AppleOnboardAudio::WritePRAMVol leftVol=%lu, rightVol=%lu\n",leftVol,  rightVol);
    
    if (platform) {
		debug2IOLog ( "... platform 0x%X\n", (unsigned int)platform );
		pramVolume = VolumeToPRAMValue (leftVol, rightVol);
#if 0
		curPRAMVol = pramVolume ^ 0xFF;
		debug3IOLog ( "... target pramVolume = 0x%X, curPRAMVol = 0x%X\n", pramVolume, curPRAMVol );
#endif
		// get the old value to compare it with
		err = platform->readXPRAM((IOByteCount)kPRamVolumeAddr, &curPRAMVol, (IOByteCount)1);
		if ( kIOReturnSuccess == err ) {
			debug2IOLog ( "... curPRAMVol = 0x%X before write\n", (curPRAMVol & 0x07) );
			// Update only if there is a change
			if (pramVolume != (curPRAMVol & 0x07)) {
				// clear bottom 3 bits of volume control byte from PRAM low memory image
				curPRAMVol = (curPRAMVol & 0xF8) | pramVolume;
				debug2IOLog("... curPRAMVol = 0x%x\n",curPRAMVol);
				// write out the volume control byte to PRAM
				err = platform->writeXPRAM((IOByteCount)kPRamVolumeAddr, &curPRAMVol,(IOByteCount) 1);
				if ( kIOReturnSuccess != err ) {
					debug5IOLog ( "0x%X = platform->writeXPRAM( 0x%X, & 0x%X, 1 ), value = 0x%X\n", err, (unsigned int)kPRamVolumeAddr, (unsigned int)&curPRAMVol, (unsigned int)curPRAMVol );
				} else {
#if 0
					err = platform->readXPRAM((IOByteCount)kPRamVolumeAddr, &curPRAMVol, (IOByteCount)1);
					if ( kIOReturnSuccess == err ) {
						if ( ( 0x07 & curPRAMVol ) != pramVolume ) {
							debug3IOLog ( "PRAM Read after Write did not compare:  Write = 0x%X, Read = 0x%X\n", (unsigned int)pramVolume, (unsigned int)curPRAMVol );
						} else {
							debugIOLog ( "PRAM verified after write!\n" );
						}
					} else {
						debugIOLog ( "Could not readXPRAM to verify write!\n" );
					}
#endif
				}
			} else {
				debugIOLog ( "PRAM write request is to current value: no I/O\n" );
			}
		} else {
			debug2IOLog ( "Could not readXPRAM prior to write! Error 0x%X\n", err );
		}
	} else {
		debugIOLog ( "... no platform\n" );
	}
    debugIOLog("- AppleOnboardAudio::WritePRAMVol\n");
}

UInt8 AppleOnboardAudio::ReadPRAMVol (void) {
	UInt8						curPRAMVol;
	IODTPlatformExpert * 		platform;

	curPRAMVol = 0;
	platform = OSDynamicCast(IODTPlatformExpert,getPlatform());

    if (platform) {
		platform->readXPRAM((IOByteCount)kPRamVolumeAddr, &curPRAMVol, (IOByteCount)1);
		curPRAMVol &= 0x07;
	}

	return curPRAMVol;
}

/*
		User Client stuff
*/

//===========================================================================================================================
//	newUserClient
//===========================================================================================================================

IOReturn AppleOnboardAudio::newUserClient( task_t 			inOwningTask,
										 void *				inSecurityID,
										 UInt32 			inType,
										 IOUserClient **	outHandler )
{
	#pragma unused( inType )
	
    IOReturn 			err;
//    IOUserClient *		userClientPtr;
    bool				result;
	
	debug2IOLog( "[AppleOnboardAudio] creating user client for task 0x%08lX\n", ( UInt32 ) inOwningTask );
	
	// Create the user client object.
	
	err = kIOReturnNoMemory;
//	userClientPtr = AppleLegacyOnboardAudioUserClient::Create( this, inOwningTask );
//	if( !userClientPtr ) goto exit;
    
	// Set up the user client.
	
	err = kIOReturnError;
//	result = userClientPtr->attach( this );
	if( !result ) goto exit;

//	result = userClientPtr->start( this );
	if( !result ) goto exit;
	
	// Success.
	
//    *outHandler = userClientPtr;
	err = kIOReturnSuccess;
	
exit:
	debug2IOLog( "[AppleOnboardAudio] newUserClient done (err=%d)\n", err );
	if( err != kIOReturnSuccess )
	{
//		if( userClientPtr )
//		{
//			userClientPtr->detach( this );
//			userClientPtr->release();
//		}
	}
	return( err );
}

#if 0
#pragma mark -
#endif

#if 0
//===========================================================================================================================
//	Static member variables
//===========================================================================================================================

///
/// Method Table
///

const IOExternalMethod		AppleLegacyOnboardAudioUserClient::sMethods[] =
{
	//	Read
	{
		NULL,														// object
		( IOMethod ) &AppleLegacyOnboardAudioUserClient::gpioRead,		// func
		kIOUCScalarIScalarO,										// flags
		1,															// count of input parameters
		1															// count of output parameters
	},
	//	Write
	{
		NULL,														// object
		( IOMethod ) &AppleLegacyOnboardAudioUserClient::gpioWrite,		// func
		kIOUCScalarIScalarO,										// flags
		2,															// count of input parameters
		0															// count of output parameters
	},
	// gpioGetActiveState
	{
		NULL,														// object
		( IOMethod ) &AppleLegacyOnboardAudioUserClient::gpioGetActiveState,		// func
		kIOUCScalarIScalarO,										// flags
		1,															// count of input parameters
		1															// count of output parameters
	},
	// gpioSetActiveState
	{
		NULL,														// object
		( IOMethod ) &AppleLegacyOnboardAudioUserClient::gpioSetActiveState,		// func
		kIOUCScalarIScalarO,										// flags
		2,															// count of input parameters
		0															// count of output parameters
	},
	// gpioCheckIfAvailable
	{
		NULL,														// object
		( IOMethod ) &AppleLegacyOnboardAudioUserClient::gpioCheckAvailable,		// func
		kIOUCScalarIScalarO,										// flags
		1,															// count of input parameters
		1															// count of output parameters
	},
	// hwRegisterRead32
	{
		NULL,														// object
		( IOMethod ) &AppleLegacyOnboardAudioUserClient::hwRegisterRead32,		// func
		kIOUCScalarIScalarO,										// flags
		1,															// count of input parameters
		1															// count of output parameters
	},
	// hwRegisterWrite32
	{
		NULL,														// object
		( IOMethod ) &AppleLegacyOnboardAudioUserClient::hwRegisterWrite32,		// func
		kIOUCScalarIScalarO,										// flags
		2,															// count of input parameters
		0															// count of output parameters
	},
	// codecReadRegister
	{
		NULL,														// object
		( IOMethod ) &AppleLegacyOnboardAudioUserClient::codecReadRegister,		// func
		kIOUCScalarIStructO,										// flags
		1,															// count of input parameters
		kMaxCodecStructureSize										// size of output structure
	},
	// codecWriteRegister
	{
		NULL,														// object
		( IOMethod ) &AppleLegacyOnboardAudioUserClient::codecWriteRegister,		// func
		kIOUCScalarIStructI,										// flags
		1,															// count of input parameters
		kMaxCodecRegisterWidth										// size of input structure
	},
	// readSpeakerID
	{
		NULL,														// object
		( IOMethod ) &AppleLegacyOnboardAudioUserClient::readSpeakerID,		// func
		kIOUCScalarIScalarO,										// flags
		1,															// count of input parameters
		1															// count of output parameters
	},
	// codecRegisterSize
	{
		NULL,														// object
		( IOMethod ) &AppleLegacyOnboardAudioUserClient::codecRegisterSize,		// func
		kIOUCScalarIScalarO,										// flags
		1,															// count of input parameters
		1															// count of output parameters
	},
	// readPRAMVolume
	{
		NULL,														// object
		( IOMethod ) &AppleLegacyOnboardAudioUserClient::readPRAMVolume,		// func
		kIOUCScalarIScalarO,										// flags
		1,															// count of input parameters
		1															// count of output parameters
	},
	// readDMAState
	{
		NULL,														// object
		( IOMethod ) &AppleLegacyOnboardAudioUserClient::readDMAState,		// func
		kIOUCScalarIScalarO,										// flags
		1,															// count of input parameters
		1															// count of output parameters
	},
	// readStreamFormat
	{
		NULL,														// object
		( IOMethod ) &AppleLegacyOnboardAudioUserClient::readStreamFormat,		// func
		kIOUCScalarIStructO,										// flags
		1,															// count of input parameters
		sizeof ( IOAudioStreamFormat )								// size of output structure
	},
	// readPowerState
	{
		NULL,														// object
		( IOMethod ) &AppleLegacyOnboardAudioUserClient::readPowerState,	// func
		kIOUCScalarIScalarO,										// flags
		1,															// count of input parameters
		1															// count of output parameters
	},
	// hwRegisterWrite32
	{
		NULL,														// object
		( IOMethod ) &AppleLegacyOnboardAudioUserClient::setPowerState,	// func
		kIOUCScalarIScalarO,										// flags
		2,															// count of input parameters
		0															// count of output parameters
	},
	// ksetBiquadCoefficients
	{
		NULL,														// object
		( IOMethod ) &AppleLegacyOnboardAudioUserClient::setBiquadCoefficients,	// func
		kIOUCScalarIStructI,										// flags
		1,															// count of input parameters
		kMaxBiquadWidth												// size of input structure
	},
	//	getBiquadInfo
	{
		NULL,														// object
		( IOMethod ) &AppleLegacyOnboardAudioUserClient::getBiquadInfo,		// func
		kIOUCScalarIStructO,										// flags
		1,															// count of input parameters
		kMaxBiquadInfoSize											// size of output structure
	},
	//	getProcessingParams
	{
		NULL,														// object
		( IOMethod ) &AppleLegacyOnboardAudioUserClient::getProcessingParams,		// func
		kIOUCScalarIStructO,										// flags
		1,															// count of input parameters
		kMaxProcessingParamSize										// size of output structure
	},
	// setProcessingParams
	{
		NULL,														// object
		( IOMethod ) &AppleLegacyOnboardAudioUserClient::setProcessingParams,		// func
		kIOUCScalarIStructI,										// flags
		1,															// count of input parameters
		kMaxProcessingParamSize										// size of input structure
	},
	// invokeInternalFunction
	{
		NULL,														// object
		( IOMethod ) &AppleLegacyOnboardAudioUserClient::invokeInternalFunction,		// func
		kIOUCScalarIStructI,										// flags
		1,															// count of input parameters
		16															// size of input structure
	},
};

const IOItemCount		AppleLegacyOnboardAudioUserClient::sMethodCount = sizeof( AppleLegacyOnboardAudioUserClient::sMethods ) / 
																  sizeof( AppleLegacyOnboardAudioUserClient::sMethods[ 0 ] );

OSDefineMetaClassAndStructors( AppleLegacyOnboardAudioUserClient, IOUserClient )

//===========================================================================================================================
//	Create
//===========================================================================================================================

AppleLegacyOnboardAudioUserClient *	AppleLegacyOnboardAudioUserClient::Create( AppleOnboardAudio *inDriver, task_t inTask )
{
    AppleLegacyOnboardAudioUserClient *		userClient;
    
    userClient = new AppleLegacyOnboardAudioUserClient;
	if( !userClient )
	{
		debugIOLog( "[AppleOnboardAudio] create user client object failed\n" );
		goto exit;
	}
    
    if( !userClient->initWithDriver( inDriver, inTask ) )
	{
		debugIOLog( "[AppleOnboardAudio] initWithDriver failed\n" );
		
		userClient->release();
		userClient = NULL;
		goto exit;
	}
	
	debug2IOLog( "[AppleOnboardAudio] User client created for task 0x%08lX\n", ( UInt32 ) inTask );
	
exit:
	return( userClient );
}

//===========================================================================================================================
//	initWithDriver
//===========================================================================================================================

bool	AppleLegacyOnboardAudioUserClient::initWithDriver( AppleOnboardAudio *inDriver, task_t inTask )
{
	bool		result;
	
	debugIOLog( "[AppleOnboardAudio] initWithDriver\n" );
	
	result = false;
    if( !initWithTask( inTask, NULL, 0 ) )
	{
		debugIOLog( "[AppleOnboardAudio] initWithTask failed\n" );
		goto exit;
    }
    if( !inDriver )
	{
		debugIOLog( "[AppleOnboardAudio] initWithDriver failed (null input driver)\n" );
        goto exit;
    }
    
    mDriver 	= inDriver;
    mClientTask = inTask;
    result		= true;
	
exit:
	return( result );
}

//===========================================================================================================================
//	free
//===========================================================================================================================

void	AppleLegacyOnboardAudioUserClient::free( void )
{
	debugIOLog( "[AppleOnboardAudio] free\n" );
	
    IOUserClient::free();
}

//===========================================================================================================================
//	clientClose
//===========================================================================================================================

IOReturn	AppleLegacyOnboardAudioUserClient::clientClose( void )
{
	debugIOLog( "[AppleOnboardAudio] clientClose\n" );
	
    if( !isInactive() )
	{
        mDriver = NULL;
    }
    return( kIOReturnSuccess );
}

//===========================================================================================================================
//	clientDied
//===========================================================================================================================

IOReturn	AppleLegacyOnboardAudioUserClient::clientDied( void )
{
	debugIOLog( "[AppleOnboardAudio] clientDied\n" );
	
    return( clientClose() );
}

//===========================================================================================================================
//	getTargetAndMethodForIndex
//===========================================================================================================================

IOExternalMethod *	AppleLegacyOnboardAudioUserClient::getTargetAndMethodForIndex( IOService **outTarget, UInt32 inIndex )
{
	IOExternalMethod *		methodPtr;
	
	methodPtr = NULL;
	if( inIndex <= sMethodCount )  {
        *outTarget = this;
		methodPtr = ( IOExternalMethod * ) &sMethods[ inIndex ];
    } else {
		debug2IOLog( "[AppleOnboardAudio] getTargetAndMethodForIndex - bad index (index=%lu)\n", inIndex );
	}
	return( methodPtr );
}

//===========================================================================================================================
//	gpioRead
//===========================================================================================================================

IOReturn	AppleLegacyOnboardAudioUserClient::gpioRead (UInt32 selector, UInt8 * gpioState) {
	IOReturn		err = kIOReturnNotReadable;
	if ( NULL != mDriver && NULL != gpioState ) {
		*gpioState = mDriver->readGPIO ( selector );
		err = kIOReturnSuccess;
	}
	return (err);
}

//===========================================================================================================================
//	gpioWrite
//===========================================================================================================================

IOReturn	AppleLegacyOnboardAudioUserClient::gpioWrite (UInt32 selector, UInt8 data) {
	IOReturn		err = kIOReturnNotReadable;
	if ( NULL != mDriver ) {
		mDriver->writeGPIO ( selector, data );
		err = kIOReturnSuccess;
	}
	return (err);
}

//===========================================================================================================================
//	gpioGetActiveState
//===========================================================================================================================

IOReturn	AppleLegacyOnboardAudioUserClient::gpioGetActiveState (UInt32 selector, UInt8 * gpioActiveState) {
	IOReturn		err = kIOReturnNotReadable;
	if ( NULL != mDriver && NULL != gpioActiveState ) {
		*gpioActiveState = mDriver->getGPIOActiveState ( selector );
		err = kIOReturnSuccess;
	}
	return (err);
}

//===========================================================================================================================
//	gpioSetActiveState
//===========================================================================================================================

IOReturn	AppleLegacyOnboardAudioUserClient::gpioSetActiveState (UInt32 selector, UInt8 gpioActiveState) {
	IOReturn		err = kIOReturnNotReadable;
	if ( NULL != mDriver ) {
		mDriver->setGPIOActiveState ( selector, gpioActiveState );
		err = kIOReturnSuccess;
	}
	return (err);
}

//===========================================================================================================================
//	checkIfGPIOExists
//===========================================================================================================================

IOReturn	AppleLegacyOnboardAudioUserClient::gpioCheckAvailable ( UInt32 selector, UInt8 * gpioExists ) {
	IOReturn		err = kIOReturnNotReadable;
	if ( NULL != mDriver || NULL != gpioExists ) {
		*gpioExists = mDriver->checkGpioAvailable ( selector );
		err = kIOReturnSuccess;
	}
	return (err);
}

//===========================================================================================================================
//	hwRegisterRead32
//===========================================================================================================================

IOReturn	AppleLegacyOnboardAudioUserClient::hwRegisterRead32 ( UInt32 selector, UInt32 * data ) {
	IOReturn		err = kIOReturnNotReadable;
	if ( NULL != mDriver && NULL != data ) {
		err = mDriver->readHWReg32 ( selector, data );
	}
	return (err);
}

//===========================================================================================================================
//	hwRegisterWrite32
//===========================================================================================================================

IOReturn	AppleLegacyOnboardAudioUserClient::hwRegisterWrite32 ( UInt32 selector, UInt32 data ) {
	IOReturn		err = kIOReturnNotReadable;
	if ( NULL != mDriver ) {
		err = mDriver->writeHWReg32 ( selector, data );
	}
	return (err);
}

//===========================================================================================================================
//	codecReadRegister
//
//	This transaction copies the entire codec register cache (up to 512 bytes) in a single transaction in order to 
//	limit the number of user client transactions.  If a register cache larger than 512 bytes exists then the
//	scalarArg1 is used to provide a target 512 byte block address.  For register cache sizes of 512 bytes or less,
//	scalarArg1 will have a value of zero to indicate the base block address.
//===========================================================================================================================

IOReturn	AppleLegacyOnboardAudioUserClient::codecReadRegister ( UInt32 scalarArg1, void * outStructPtr, IOByteCount * outStructSizePtr ) {
	kern_return_t	err = kIOReturnNotReadable;
	
	if ( NULL != mDriver && NULL != outStructPtr  && NULL != outStructSizePtr ) {
		err = mDriver->readCodecReg ( scalarArg1, outStructPtr, outStructSizePtr );
	}
	return ( err );
}

//===========================================================================================================================
//	codecWriteRegister
//===========================================================================================================================

IOReturn	AppleLegacyOnboardAudioUserClient::codecWriteRegister ( UInt32 selector, void * data, UInt32 inStructSize ) {
	IOReturn		err = kIOReturnNotReadable;
	if ( NULL != mDriver && NULL != data ) {
		err = mDriver->writeCodecReg ( selector, data );
	}
	return (err);
}

//===========================================================================================================================
//	readSpeakerID
//===========================================================================================================================

IOReturn	AppleLegacyOnboardAudioUserClient::readSpeakerID ( UInt32 selector, UInt32 * data ) {
	IOReturn		err = kIOReturnNotReadable;

	if ( NULL != mDriver && NULL != data ) {
		err = mDriver->readSpkrID ( selector, data );
	}
	return (err);
}


//===========================================================================================================================
//	codecRegisterSize
//===========================================================================================================================

IOReturn	AppleLegacyOnboardAudioUserClient::codecRegisterSize ( UInt32 selector, UInt32 * codecRegSizePtr ) {
	IOReturn		err = kIOReturnNotReadable;
	if ( NULL != mDriver && NULL != codecRegSizePtr ) {
		err = mDriver->getCodecRegSize ( selector, codecRegSizePtr );
	}
	return (err);
}

//===========================================================================================================================
//	readPRAMVolume
//===========================================================================================================================

IOReturn	AppleLegacyOnboardAudioUserClient::readPRAMVolume ( UInt32 selector, UInt32 * pramDataPtr ) {
	IOReturn		err = kIOReturnNotReadable;
	if ( NULL != mDriver && NULL != pramDataPtr ) {
		err = (UInt32)mDriver->getVolumePRAM ( pramDataPtr );
	}
	return (err);
}


//===========================================================================================================================
//	readDMAState
//===========================================================================================================================

IOReturn	AppleLegacyOnboardAudioUserClient::readDMAState ( UInt32 selector, UInt32 * dmaStatePtr ) {
	IOReturn		err = kIOReturnNotReadable;
	if ( NULL != mDriver && NULL != dmaStatePtr ) {
		err = (UInt32)mDriver->getDmaState ( dmaStatePtr );
	}
	return err;
}


//===========================================================================================================================
//	readStreamFormat
//===========================================================================================================================

IOReturn	AppleLegacyOnboardAudioUserClient::readStreamFormat ( UInt32 selector, IOAudioStreamFormat * outStructPtr, IOByteCount * outStructSizePtr ) {
#pragma unused ( selector )
	IOReturn		err = kIOReturnNotReadable;
	if ( NULL != mDriver && NULL != outStructPtr && NULL != outStructSizePtr ) {
		if ( sizeof ( IOAudioStreamFormat ) <= *outStructSizePtr ) {
			err = (UInt32)mDriver->getStreamFormat ( outStructPtr );
			if ( kIOReturnSuccess == err ) {
				*outStructSizePtr = sizeof ( IOAudioStreamFormat );
			}
		}
	}
	return err;
}


//===========================================================================================================================
//	readPowerState
//===========================================================================================================================

IOReturn	AppleLegacyOnboardAudioUserClient::readPowerState ( UInt32 selector, IOAudioDevicePowerState * powerState ) {
	IOReturn		err = kIOReturnNotReadable;
	if ( NULL != mDriver && NULL != powerState ) {
		err = mDriver->readPowerState ( selector, powerState );
	}
	return (err);
}

//===========================================================================================================================
//	setPowerState
//===========================================================================================================================

IOReturn	AppleLegacyOnboardAudioUserClient::setPowerState ( UInt32 selector, IOAudioDevicePowerState powerState ) {
	IOReturn		err = kIOReturnNotReadable;
	if ( NULL != mDriver ) {
		err = mDriver->setPowerState ( selector, powerState );
	}
	return (err);
}

//===========================================================================================================================
//	setBiquadCoefficients
//
//	NOTE:	selector is used to pass a streamID.  Texas & Texas2 only have a single output stream and require that
//			the selector be passed with a value of zero.
//===========================================================================================================================

IOReturn	AppleLegacyOnboardAudioUserClient::setBiquadCoefficients ( UInt32 selector, void * biquadCoefficients, UInt32 coefficientSize ) {
	IOReturn		err = kIOReturnNotReadable;
	
	if ( NULL != mDriver && NULL != biquadCoefficients && kMaxBiquadWidth >= coefficientSize ) {
		err = mDriver->setBiquadCoefficients ( selector, biquadCoefficients, coefficientSize );
	}
	return (err);
}


//===========================================================================================================================
//	getBiquadInfo
//===========================================================================================================================

IOReturn	AppleLegacyOnboardAudioUserClient::getBiquadInfo ( UInt32 scalarArg1, void * outStructPtr, IOByteCount * outStructSizePtr ) {
	IOReturn		err = kIOReturnNotReadable;
	if ( NULL != mDriver && NULL != outStructPtr && NULL != outStructSizePtr ) {
		err = mDriver->getBiquadInformation ( scalarArg1, outStructPtr, outStructSizePtr );
	}
	return (err);
}


//===========================================================================================================================
//	getProcessingParams
//===========================================================================================================================

IOReturn	AppleLegacyOnboardAudioUserClient::getProcessingParams ( UInt32 scalarArg1, void * outStructPtr, IOByteCount * outStructSizePtr ) {
	IOReturn		err = kIOReturnNotReadable;
	if ( NULL != mDriver && NULL != outStructPtr && NULL != outStructSizePtr ) {
		if (  kMaxProcessingParamSize >= *outStructSizePtr ) {
			err = mDriver->getProcessingParameters ( scalarArg1, outStructPtr, outStructSizePtr );
		}
	}
	return (err);
}


//===========================================================================================================================
//	setProcessingParams
//===========================================================================================================================

IOReturn	AppleLegacyOnboardAudioUserClient::setProcessingParams ( UInt32 scalarArg1, void * inStructPtr, UInt32 inStructSize ) {
	IOReturn		err = kIOReturnNotReadable;
	if ( NULL != mDriver && NULL != inStructPtr && kMaxProcessingParamSize >= inStructSize ) {
		err = mDriver->setProcessingParameters ( scalarArg1, inStructPtr, inStructSize );
	}
	return (err);
}


//===========================================================================================================================
//	invokeInternalFunction
//===========================================================================================================================

IOReturn	AppleLegacyOnboardAudioUserClient::invokeInternalFunction ( UInt32 functionSelector, void * inData, UInt32 inDataSize ) {
	IOReturn		err = kIOReturnNotReadable;
	if ( NULL != mDriver ) {
		err = mDriver->invokeInternalFunction ( functionSelector, inData );
	}
	return (err);
}

#endif

#if 0
/*
		The following code goes into whatever application wants to call into AppleOnboardAudio
*/

//===========================================================================================================================
//	Private constants
//===========================================================================================================================

enum
{
	kgpioReadIndex	 		= 0,
	kgpioWriteIndex 		= 1
};

//===========================================================================================================================
//	Private prototypes
//===========================================================================================================================

static IOReturn	SetupUserClient( void );
static void		TearDownUserClient( void );

//===========================================================================================================================
//	Globals
//===========================================================================================================================

static mach_port_t		gMasterPort		= 0;
static io_object_t		gDriverObject	= 0;
static io_connect_t		gDataPort 		= 0;

//===========================================================================================================================
//	SetupUserClient
//===========================================================================================================================

static IOReturn	SetupUserClient( void )
{
	IOReturn			err;
	CFDictionaryRef		matchingDictionary;
	io_iterator_t		serviceIter;
	
	// Initialize variables for easier cleanup.
	
	err					= kIOReturnSuccess;
	matchingDictionary 	= NULL;
	serviceIter			= NULL;
	
	// Exit quickly if we're already set up.
	
	if( gDataPort )
	{
		goto exit;
	}
	
	// Get a port so we can communicate with IOKit.
	
	err = IOMasterPort( NULL, &gMasterPort );
	if( err != kIOReturnSuccess ) goto exit;
	
	// Build a dictionary of all the services matching our service name. Note that we do not release the dictionary
	// if IOServiceGetMatchingServices succeeds because it does the release itself.
	
	err = kIOReturnNotFound;
	matchingDictionary = IOServiceNameMatching( "AppleTexasAudio" );
	if( !matchingDictionary ) goto exit;
	
	err = IOServiceGetMatchingServices( gMasterPort, matchingDictionary, &serviceIter );
	if( err != kIOReturnSuccess ) goto exit;
	matchingDictionary = NULL;
	
	err = kIOReturnNotFound;
	gDriverObject = IOIteratorNext( serviceIter );
	if( !gDriverObject ) goto exit;
	
	// Open a connection to our service so we can talk to it.
	
	err = IOServiceOpen( gDriverObject, mach_task_self(), 0, &gDataPort );
	if( err != kIOReturnSuccess ) goto exit;
	
	// Success. Clean up stuff and we're done.
	
exit:
	if( serviceIter )
	{
		IOObjectRelease( serviceIter );
	}
	if( matchingDictionary )
	{
		CFRelease( matchingDictionary );
	}
	if( err != kIOReturnSuccess )
	{
		TearDownUserClient();
	}
	return( err );
}

//===========================================================================================================================
//	TearDownUserClient
//===========================================================================================================================

static void	TearDownUserClient( void )
{
	if( gDataPort )
	{
		IOServiceClose( gDataPort );
		gDataPort = 0;
	}
	if( gDriverObject )
	{
		IOObjectRelease( gDriverObject );
		gDriverObject = NULL;
	}
	if( gMasterPort )
	{
		mach_port_deallocate( mach_task_self(), gMasterPort );
		gMasterPort = 0;
	}
}

#if 0
#pragma mark -
#endif

//===========================================================================================================================
//	gpioRead
//===========================================================================================================================

OSStatus	gpioRead( UInt32 selector, UInt8 * gpioState )
{	
	OSStatus		err;
	
	// Set up user client if not already set up.
	
	err = SetupUserClient();
	if( err != noErr ) goto exit;
	
	// RPC to the kernel.
	
	err = IOConnectMethodScalarIScalarO( gDataPort, kgpioReadIndex, 1, 1, selector, gpioState );
	if( err != noErr ) goto exit;
	
exit:
	return( err );
}

//===========================================================================================================================
//	gpioWrite
//===========================================================================================================================

OSStatus	gpioWrite( UInt32 selector, UInt8 value )
{	
	OSStatus		err;
	
	// Set up user client if not already set up.
	
	err = SetupUserClient();
	if( err != noErr ) goto exit;
	
	// RPC to the kernel.
	
	err = IOConnectMethodScalarIScalarO( gDataPort, kgpioWriteIndex, 2, 0, selector, gpioState );
	if( err != noErr ) goto exit;
	
exit:
	return( err );
}

//===========================================================================================================================
//	getGPIOAddress
//===========================================================================================================================

OSStatus	gpioGetAddress( UInt32 selector, UInt8 ** gpioAddress )
{
	OSStatus		err;
	
	// Set up user client if not already set up.
	
	err = SetupUserClient();
	if( err != noErr ) goto exit;
	
	// RPC to the kernel.
	
	err = IOConnectMethodScalarIScalarO( gDataPort, kgpiogetAddressIndex, 2, selector, gpioAddress );
	if( err != noErr ) goto exit;
	
exit:
	return( err );
}

#endif
