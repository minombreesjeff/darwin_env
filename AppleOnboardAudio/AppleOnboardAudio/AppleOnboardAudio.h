/*
 *  AppleOnboardAudio.h
 *  AppleOnboardAudio
 *
 *  Created by cerveau on Mon Jun 04 2001.
 *  Copyright (c) 2001 Apple Computer Inc. All rights reserved.
 *
 */

#ifndef __APPLEONBOARDAUDIO__
#define __APPLEONBOARDAUDIO__

#include <IOKit/IOInterruptEventSource.h>
#include <IOKit/IOUserClient.h>

#include "AudioHardwareCommon.h"
#include "AudioHardwareConstants.h"
#include "AppleDBDMAAudio.h"
#include "AudioHardwareObjectInterface.h"

#include "PlatformInterface.h"
#include "TransportInterface.h"

enum invokeInternalFunctionSelectors {
	kInvokeHeadphoneInterruptHandler,
	kInvokeSpeakerInterruptHandler
};
		
#define kBatteryPowerDownDelayTime		30000000000ULL				/* 30 seconds					*/
#define kACPowerDownDelayTime			300000000000ULL				/* 300 seconds == 5 minutes		*/
#define kiSubMaxVolume					60
#define kiSubVolumePercent				92

typedef struct {
	UInt32			layoutID;			//	identify the target CPU
	UInt32			portID;				//	identify port (see: AudioPortTypes in AudioHardwareConstants.h)
	UInt32			speakerID;			//	dallas ROM ID (concatenates all fields)
} SpeakerIDStruct;
typedef SpeakerIDStruct * SpeakerIDStructPtr;

class IOAudioControl;

enum outputClipRoutineOptions {
	kFloatToIntClip				= (1 << 0),		// basic clipping
	kPhaseInversionClip			= (1 << 1),		// invert the right channel
	kStereoToLeftChanClip		= (1 << 2),		// mix right channel to left and mute right chan
	kStereoToRightChanClip		= (1 << 3),		// mix left channel to right and mute left chan
	kDelayLeftChan1SampleClip	= (1 << 4),		// insert a 1 sample delay in left channel stream
	kDelayRightChan1SampleClip	= (1 << 5)		// insert a 1 sample delay in right channel stream
};

enum connectionPossibilities {
	kMonoSpeaker				= (1 << 0),
	kStereoSpeaker				= (1 << 1),
	kInternalClock				= (1 << 2),
	kExternalClock				= (1 << 3),
	kMux						= (1 << 4)
};

enum deviceConnections {
	kInternalSpeakerOutput		= 0,
	kHeadphoneOutput			= 1, 
	kLineOutOutput				= 2,
	kExternalSpeakerOutput		= 3,
	kDigitalOutOutput			= 4,
	kDigitalInInput				= 5,
	kLineInInput				= 6,
	kInternalMicInput			= 7,
	kExternalMicInput			= 8
};
	
enum jackStates {
	kRemoved					= 0,
	kInserted					= 1,
	kUnknown					= 2
};

#define kPluginPListInputLatency		"InputLatency"
#define kPluginPListOutputLatency		"OutputLatency"
#define kPluginPListAOAAttributes		"AOAAttributes"
#define kPluginPListSoftwareInputGain	"SoftwareInputGain"

#define kSoundEntry						"sound"
#define	kLayoutID						"layout-id"
#define	kCompatible						"compatible"
#define kLayouts						"Layouts"
#define kLayoutIDInfoPlist				"LayoutID"
#define kHardwareObjects				"HardwareObjects"
#define kPlatformObject					"PlatformObject"
#define kAmpRecoveryTime				"AmpRecoveryTime"
#define kExternalClockSelect			"ExternalClockSelect"
#define kTransportObject				"TransportObject"
#define kControls						"Controls"
#define kFormats						"Formats"
#define kPluginID						"PluginID"
#define kSignalProcessing				"SignalProcessing"
#define kHardwareDSPIndex				"HardwareDSPIndex"
#define kEqualization					"Equalization"
#define kDynamicRange					"DynamicRange"
#define kSoftwareDSP					"SoftwareDSP"
#define kSpeakerID						"SpeakerID"
#define kPluginRecoveryOrder			"RecoveryOrder"
#define kClipRoutines					"ClipRoutines"

#define kFilterType						"FilterType"
#define kFilterFrequency				"Frequency"
#define kFilterQ						"Q"
#define kFilterGain						"Gain"

#define kLimiter						"Limiter"
#define kLimiterType					"LimiterType"
#define kLimiterBandIndex				"BandIndex"
#define kLimiterAttackTime				"AttackTime"
#define kLimiterReleaseTime				"ReleaseTime"
#define kLimiterThreshold				"Threshold"
#define kLimiterRatio					"Ratio"
#define kLimiterLookahead				"Lookahead"

#define kCrossover						"Crossover"
#define kCrossoverFrequency				"Frequency"
#define kCrossoverNumberOfBands			"NumberOfBands"

#define kLeftVolControlString			"Left"
#define kRightVolControlString			"Right"
#define kMasterVolControlString			"Master"
#define kMuteControlString				"Mute"

#define kOutputsList					"Outputs"
#define kHeadphones						"Headphones"
#define kInternalSpeakers				"IntSpeakers"
#define kExternalSpeakers				"ExtSpeakers"
#define kLineOut						"LineOut"
#define kDigitalOut						"DigitalOut"

#define kInputsList						"Inputs"
#define kInternalMic					"InternalMic"
#define kExternalMic					"ExternalMic"
#define kLineIn							"LineIn"
#define kDigitalIn						"DigitalIn"

#define kPluginPListMasterVol			"master-vol"
#define kPluginPListLeftVol				"left-vol"
#define kPluginPListRightVol			"right-vol"
#define kPluginPListLeftGain			"left-gain"
#define kPluginPListRightGain			"right-gain"

#define kInternalClockString			"Internal Clock"
#define kExternalClockString			"External Clock"

#define kFloatToIntClipString				"FloatToInt"
#define kIntToFloatClipString				"IntToFloat"
#define kPhaseInversionClipString			"PhaseInversion"
#define kStereoToLeftChanClipString			"StereoToLeft"
#define kStereoToRightChanClipString		"StereoToRight"
#define kDelayLeftChan1SampleClipString		"DelayLeft"
#define kDelayRightChan1SampleClipString	"DelayRight"
#define kCopyLeftToRight					"LeftToRight"
#define kCopyRightToLeft					"RightToLeft"

#define kNoEQID							0xFFFFFFFF

enum {
	kClockSourceSelectionInternal	= 'int ',
	kClockSourceSelectionExternal	= 'ext '
};

class AppleOnboardAudio : public IOAudioDevice
{
    
    OSDeclareDefaultStructors(AppleOnboardAudio);

protected:
	// general controls : these are the default controls attached to a DMA audio engine
    IOAudioToggleControl *				mOutMuteControl;
    IOAudioToggleControl *				mPlaythruToggleControl;
	IOAudioToggleControl *				mHeadphoneConnected;
	IOAudioToggleControl *				mInputConnectionControl;
	IOAudioLevelControl *				mPRAMVolumeControl;
	IOAudioLevelControl *				mOutMasterVolumeControl;
    IOAudioLevelControl *				mOutLeftVolumeControl;
    IOAudioLevelControl *				mOutRightVolumeControl;
    IOAudioLevelControl *				mInLeftGainControl;
    IOAudioLevelControl *				mInRightGainControl;
    IOAudioSelectorControl *			mInputSelector;
	IOAudioSelectorControl *			mOutputSelector;
	IOAudioSelectorControl *			mExternalClockSelector;
	
	IONotifier *						mSysPowerDownNotifier;

    OSArray *							mPluginObjects;
	PlatformInterface *					mPlatformInterface;
	TransportInterface *				mTransportInterface;
	AudioHardwareObjectInterface *		mCurrentOutputPlugin;				
	AudioHardwareObjectInterface *		mCurrentInputPlugin;				
	
	// we keep the engines around to have a cleaner initHardware
    AppleDBDMAAudio *					mDriverDMAEngine;

    Boolean								mVolMuteActive;
    SInt32								mVolLeft;
    SInt32								mVolRight;
    SInt32								mGainLeft;
    SInt32								mGainRight;
	Boolean								mUseMasterVolumeControl;
	Boolean								mUseInputGainControls;
	UInt32								mLayoutID;
	UInt32								mDetectCollection;
	UInt32								mSpeakerID;
	bool								mCurrentPluginHasSoftwareInputGain;
	UInt32								mAmpRecoveryMuteDuration;
	
	
	unsigned long long					idleSleepDelayTime;
	IOTimerEventSource *				idleTimer;
    Boolean								mIsMute;
    Boolean								mAutoUpdatePRAM;
	IOAudioDevicePowerState				ourPowerState;
	Boolean								shuttingDown;

    OSArray	*							AudioSoftDSPFeatures;

	// Dynamic variable that handle the connected devices
    sndHWDeviceSpec						currentDevices;
    bool 								fCPUNeedsPhaseInversion;	// true if this CPU's channels are out-of-phase
    bool 								mHasHardwareInputGain;		// aml 5.3.02
	bool 								mRangeInChanged;	
	
	DualMonoModeType					mInternalMicDualMonoMode;	// aml 6.17.02
	
	UInt32								mProcessingParams[kMaxProcessingParamSize/sizeof(UInt32)];
	bool								disableLoadingEQFromFile;

public:
	// Classical Unix funxtions
	virtual bool			start (IOService * provider);
    virtual bool			init (OSDictionary * properties);
    virtual void			free ();
	virtual void			stop (IOService * provider);

	virtual bool 			willTerminate ( IOService * provider, IOOptionBits options );
	virtual bool 			handleOpen (IOService * forClient, IOOptionBits	options, void *	arg ) ;
	virtual void			handleClose (IOService * forClient, IOOptionBits options );

	// IOAudioDevice subclass
    virtual bool			initHardware (IOService * provider);
    virtual IOReturn		createDefaultControls ();
	virtual IOReturn		createInputGainControls (void);
	virtual IOReturn		createOutputVolumeControls (void);
    virtual UInt32			getCharCodeForString (OSString * inputString);
	virtual IOReturn		createInputSelectorControl (void);
	virtual IOReturn 		createOutputSelectorControl (void);
	void					createPlayThruControl (void);
	void					removePlayThruControl (void);

	virtual OSString *		getStringForCharCode (UInt32 charCode);
	virtual UInt32			getCharCodeForIntCode (UInt32 inCode);

    virtual IORegistryEntry * FindEntryByNameAndProperty (const IORegistryEntry * start, const char * name, const char * key, UInt32 value);

    static IOReturn			outputControlChangeHandler (IOService *target, IOAudioControl *volumeControl, SInt32 oldValue, SInt32 newValue);
	static IOReturn			inputControlChangeHandler (IOService *target, IOAudioControl *control, SInt32 oldValue, SInt32 newValue);

	virtual IOReturn		volumeMasterChange (SInt32 newValue);
    virtual IOReturn		volumeLeftChange (SInt32 newValue);
    virtual IOReturn		volumeRightChange (SInt32 newValue);
	virtual IOReturn		outputMuteChange (SInt32 newValue);
    virtual IOReturn		gainLeftChanged (SInt32 newValue);
    virtual IOReturn		gainRightChanged (SInt32 newValue);
    virtual IOReturn		passThruChanged (SInt32 newValue);

    virtual IOReturn		inputSelectorChanged (SInt32 newValue);
    virtual IOReturn		outputSelectorChanged (SInt32 newValue);
	virtual IOReturn		clockSelectorChanged (SInt32 newValue);

    virtual IOReturn		performPowerStateChange (IOAudioDevicePowerState oldPowerState, IOAudioDevicePowerState newPowerState, UInt32 * microsecondsUntilComplete);
	virtual void			setTimerForSleep ();
	static void				sleepHandlerTimer (OSObject * owner, IOTimerEventSource * sender);
    
	virtual IOReturn		newUserClient (task_t 			inOwningTask,
							void *			inSecurityID,
							UInt32 			inType,
							IOUserClient **	outHandler);

	virtual void			registerPlugin (AudioHardwareObjectInterface * thePlugin);
	static	IOReturn		registerPluginAction (OSObject * owner, void * arg1, void * arg2, void * arg3, void * arg4);
	virtual void			unRegisterPlugin (AudioHardwareObjectInterface *inPlugin);

//	virtual void			pluginStatusChangedNotification (AudioHardwareObjectInterface * thePlugin, UInt32 statusSelector, UInt32 newValue);
	virtual void			interruptEventHandler (UInt32 statusSelector, UInt32 newValue);
	static	IOReturn		sInterruptEventAction (OSObject * owner, void * arg1, void * arg2, void * arg3, void * arg4);

	virtual PlatformInterface * getPlatformInterfaceObject (void);
	virtual IOReturn 		AdjustOutputVolumeControls (AudioHardwareObjectInterface * thePluginObject, UInt32 inSelection);
	virtual IOReturn 		AdjustInputGainControls (AudioHardwareObjectInterface * thePluginObject);

	// Functions DBDMAAudio calls on AppleOnboardAudio
	virtual IOReturn		formatChangeRequest (const IOAudioStreamFormat * inFormat, const IOAudioSampleRate * inRate);
	virtual	UInt32			getCurrentSampleFrame (void);
	virtual void			setCurrentSampleFrame (UInt32 value);

protected:
	// Do the link to the IOAudioFamily 
	// These will help to create the port config through the OF Device Tree
            
    IOReturn			configureDMAEngines(IOService *provider);
    IOReturn			parseAndActivateInit(IOService *provider);
    IOReturn			configureAudioDetects(IOService *provider);
    IOReturn			configureAudioOutputs(IOService *provider);
    IOReturn			configureAudioInputs(IOService *provider);
    IOReturn			configurePowerObject(IOService *provider);

	AudioHardwareObjectInterface * getIndexedPluginObject (UInt32 index);
	OSObject * 			getLayoutEntry (const char * entryID);
	bool				hasMasterVolumeControl (const char * outputEntry);
	bool				hasMasterVolumeControl (const UInt32 inCode);
	bool				hasLeftVolumeControl (const char * outputEntry);
	bool				hasLeftVolumeControl (const UInt32 inCode);
	bool				hasRightVolumeControl (const char * outputEntry);
	bool				hasRightVolumeControl (const UInt32 inCode);

	void				setUseInputGainControls (const char * outputEntry);

	OSArray *			getControlsArray (const char * outputEntry);
	AudioHardwareObjectInterface * getPluginObjectForConnection (const char * entry);

	IOReturn			setHardwareDSP (UInt32 outputEntry);
	IOReturn			setHardwareDSP (const char * outputEntry);
	void				setSoftwareOutputDSP (const char * inSelectedOutput);

	UInt32				setClipRoutineForOutput (const char * inSelectedOutput);
	UInt32				setClipRoutineForInput (const char * inSelectedInput);

	static IOReturn		sysPowerDownHandler (void * target, void * refCon, UInt32 messageType, IOService * provider, void * messageArgument, vm_size_t argSize);

    void				setCurrentDevices(sndHWDeviceSpec devices);

	IOReturn			setAggressiveness(unsigned long type, unsigned long newLevel);

	void				createLeftVolumeControl (IOFixed mindBVol, IOFixed maxdBVol, SInt32 minVolume, SInt32 maxVolume);
	void				createRightVolumeControl (IOFixed mindBVol, IOFixed maxdBVol, SInt32 minVolume, SInt32 maxVolume);
	void				createMasterVolumeControl (IOFixed mindBVol, IOFixed maxdBVol, SInt32 minVolume, SInt32 maxVolume);
	void				createLeftGainControl (IOFixed mindBGain, IOFixed maxdBGain, SInt32 minGain, SInt32 maxGain);
	void				createRightGainControl (IOFixed mindBGain, IOFixed maxdBGain, SInt32 minGain, SInt32 maxGain);

	void				cacheOutputVolumeLevels (AudioHardwareObjectInterface * thePluginObject);
	void				cacheInputGainLevels (AudioHardwareObjectInterface * thePluginObject);
	
	void				removeLeftVolumeControl ();
	void				removeRightVolumeControl ();
	void				removeMasterVolumeControl ();
	void				removeLeftGainControl ();
	void				removeRightGainControl ();

	void				initializeDetectCollection ( void );
	UInt32				parseOutputDetectCollection (void);
	UInt32				parseInputDetectCollection (void);
	void				selectAmplifier (const UInt32 inSelection);

	char *				getConnectionKeyFromCharCode (const SInt32 inSelection, const UInt32 inDirection);

	AudioHardwareObjectInterface *	getPluginObjectWithName (OSString * inName);
	void				callPluginsInOrder (UInt32 inSelector, UInt32 newValue);

protected:
    // The PRAM utility
	UInt32				PRAMToVolumeValue (void);
    UInt8				VolumeToPRAMValue (UInt32 leftVol, UInt32 rightVol);
    void				WritePRAMVol (UInt32 volLeft, UInt32 volRight);
	UInt8				ReadPRAMVol (void);

#if 0
public:
	// User Client calls residing in AOA derived object (accessed indirectly through public APIs)
	virtual UInt8		readGPIO (UInt32 selector) = 0;
	virtual void		writeGPIO (UInt32 selector, UInt8 data) = 0;
	virtual Boolean		getGPIOActiveState (UInt32 gpioSelector) = 0;
	virtual void		setGPIOActiveState ( UInt32 selector, UInt8 gpioActiveState ) = 0;
	virtual Boolean		checkGpioAvailable ( UInt32 selector ) {return 0;}
	virtual IOReturn	readHWReg32 ( UInt32 selector, UInt32 * registerData ) = 0;
	virtual IOReturn	writeHWReg32 ( UInt32 selector, UInt32 registerData ) = 0;
	virtual IOReturn	readCodecReg ( UInt32 selector, void * registerData,  UInt32 * registerDataSize ) = 0;
	virtual IOReturn	writeCodecReg ( UInt32 selector, void * registerData ) = 0;
	virtual IOReturn	readSpkrID ( UInt32 selector, UInt32 * speakerIDPtr ) = 0;
	virtual IOReturn	getCodecRegSize ( UInt32 selector, UInt32 * codecRegSizePtr ) = 0;
	virtual IOReturn	getVolumePRAM ( UInt32 * pramDataPtr ) = 0;
	virtual IOReturn	getDmaState ( UInt32 * dmaStatePtr ) = 0;
	virtual IOReturn	getStreamFormat ( IOAudioStreamFormat * streamFormatPtr ) = 0;
	virtual IOReturn	readPowerState ( UInt32 selector, IOAudioDevicePowerState * powerState ) = 0;
	virtual IOReturn	setPowerState ( UInt32 selector, IOAudioDevicePowerState powerState ) = 0;
	virtual IOReturn	setBiquadCoefficients ( UInt32 selector, void * biquadCoefficients, UInt32 coefficientSize ) = 0;
	virtual IOReturn	getBiquadInformation ( UInt32 scalarArg1, void * outStructPtr, IOByteCount * outStructSizePtr ) = 0;
	virtual IOReturn	getProcessingParameters ( UInt32 scalarArg1, void * outStructPtr, IOByteCount * outStructSizePtr ) = 0;
	virtual IOReturn	setProcessingParameters ( UInt32 scalarArg1, void * inStructPtr, UInt32 inStructSize ) = 0;
	virtual	IOReturn	invokeInternalFunction ( UInt32 functionSelector, void * inData ) = 0;
#endif
};

#if 0
//===========================================================================================================================
//	AppleLegacyOnboardAudioUserClient
//===========================================================================================================================

class	AppleLegacyOnboardAudioUserClient : public IOUserClient
{
    OSDeclareDefaultStructors( AppleLegacyOnboardAudioUserClient )
	
	public:
	
		static const IOExternalMethod		sMethods[];
		static const IOItemCount			sMethodCount;
	
	protected:
		
		AppleOnboardAudio *					mDriver;
		task_t								mClientTask;
		
	public:
		
		//	WARNING:	The following enumerations must maintain the current order.  New
		//				enumerations may be added to the end of the list but must not
		//				be inserted into the center of the list.  Insertion of enumerations
		//				in the center of the list will cause the 'Audio Hardware Utility'
		//				to panic.  
		enum
		{
			kgpioReadIndex	 		=	0,		/*	returns data from gpio																		*/
			kgpioWriteIndex 		=	1,		/*	writes data to gpio																			*/
			kgetGpioActiveState		=	2,		/*	returns TRUE if gpio is active high															*/
			ksetGpioActiveState		=	3,		/*	sets the gpio active state (polarity)														*/
			kcheckIfGpioExists		=	4,		/*	returns TRUE if gpio exists on host CPU														*/
			kreadHWRegister32		=	5,		/*	returns data memory mapped I/O by hardware register reference								*/
			kwriteHWRegister32		=	6,		/*	writed stat to memory mapped I/O by hardware register reference								*/
			kreadCodecRegister		=	7,		/*	returns data CODEC (i.e. Screamer, DACA, Burgundy, Tumbler, Snapper by register reference	*/
			kwriteCodecRegister		=	8,		/*	writes data to CODEC (i.e. Screamer, DACA, Burgundy, Tumbler, Snapper by register reference	*/
			kreadSpeakerID			=	9,		/*	returns data from Dallas ROM																*/
			kgetCodecRegisterSize	=	10,		/*	return the size of a codec register in expressed in bytes									*/
			kreadPRAM				=	11,		/*	return PRAM contents																		*/
			kreadDMAState			=	12,		/*	return DMA state																			*/
			kreadStreamFormat		=	13,		/*	return IOAudioStreamFormat																	*/
			kreadPowerState			=	14,
			ksetPowerState			=	15,
			ksetBiquadCoefficients	=	16,
			kgetBiquadInfo			=	17,
			kgetProcessingParams	=	18,		/*	tbd: (see Aram)	*/
			ksetProcessingParams	=	19,		/*	tbd: (see Aram)	*/
			kinvokeInternalFunction	=	20
		};

		
		//	END WARNING:
		
		// Static member functions
		
		static AppleLegacyOnboardAudioUserClient * Create( AppleOnboardAudio *inDriver, task_t task );
		
		// Creation/Deletion
		
		virtual bool		initWithDriver( AppleOnboardAudio *inDriver, task_t task );
		virtual void		free();
		
		// Public API's
		
		virtual IOReturn	gpioRead ( UInt32 selector, UInt8 * gpioState );
		virtual IOReturn	gpioWrite ( UInt32 selector, UInt8 gpioState );
		virtual IOReturn	gpioGetActiveState (UInt32 selector, UInt8 * gpioActiveState);
		virtual IOReturn	gpioSetActiveState ( UInt32 selector, UInt8 gpioActiveState );
		virtual IOReturn	gpioCheckAvailable ( UInt32 selector, UInt8 * gpioExists );
		virtual IOReturn	hwRegisterRead32 ( UInt32 selector, UInt32 * registerData );
		virtual IOReturn	hwRegisterWrite32 ( UInt32 selector, UInt32 registerData );
		virtual IOReturn	codecReadRegister ( UInt32 scalarArg1, void * outStructPtr, IOByteCount * outStructSizePtr );
		virtual IOReturn	codecWriteRegister ( UInt32 selector, void * data, UInt32 inStructSize );
		virtual IOReturn	readSpeakerID ( UInt32 selector, UInt32 * speakerIDPtr );
		virtual IOReturn	codecRegisterSize ( UInt32 selector, UInt32 * codecRegSizePtr );
		virtual IOReturn	readPRAMVolume ( UInt32 selector, UInt32 * pramDataPtr );
		virtual IOReturn	readDMAState ( UInt32 selector, UInt32 * dmaStatePtr );
		virtual IOReturn	readStreamFormat ( UInt32 selector, IOAudioStreamFormat * outStructPtr, IOByteCount * outStructSizePtr );
		virtual IOReturn	readPowerState ( UInt32 selector, IOAudioDevicePowerState * powerState );
		virtual IOReturn	setPowerState ( UInt32 selector, IOAudioDevicePowerState powerState );
		virtual IOReturn	setBiquadCoefficients ( UInt32 selector, void * biquadCoefficients, UInt32 coefficientSize );
		virtual IOReturn	getBiquadInfo ( UInt32 scalarArg1, void * outStructPtr, IOByteCount * outStructSizePtr );
		virtual IOReturn	getProcessingParams ( UInt32 scalarArg1, void * outStructPtr, IOByteCount * outStructSizePtr );
		virtual IOReturn	setProcessingParams ( UInt32 scalarArg1, void * inStructPtr, UInt32 inStructSize );
		virtual IOReturn	invokeInternalFunction ( UInt32 functionSelector, void * inData, UInt32 inDataSize );

	protected:
		
		// IOUserClient overrides
		
		virtual IOReturn			clientClose();
		virtual IOReturn			clientDied();
		virtual	IOExternalMethod *	getTargetAndMethodForIndex( IOService **outTarget, UInt32 inIndex );
};
#endif
#endif
