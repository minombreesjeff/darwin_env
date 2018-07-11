#ifndef _APPLEDBDMAAUDIO_H
#define _APPLEDBDMAAUDIO_H

#include <IOKit/ppc/IODBDMA.h>
#include <IOKit/audio/IOAudioEngine.h>
#include <IOKit/audio/IOAudioTypes.h>
#include <IOKit/audio/IOAudioStream.h>

#include "AudioHardwareCommon.h"
#include "PlatformInterface.h"
#include "AppleDBDMAFloatLib.h"

//#define _TIME_CLIP_ROUTINE

// aml 2.28.02 adding header to get constants
#include "AppleiSubEngine.h"

class AppleOnboardAudio;
class IOInterruptEventSource;
class IOFilterInterruptEventSource;

// reducing the block size makes the problem happen less often
#define DBDMAAUDIODMAENGINE_DEFAULT_NUM_BLOCKS		512
#define DBDMAAUDIODMAENGINE_ROOT_BLOCK_SIZE			64
#define DBDMAAUDIODMAENGINE_DEFAULT_SAMPLE_RATE		44100
#define DBDMAAUDIODMAENGINE_DEFAULT_BIT_DEPTH		16
#define DBDMAAUDIODMAENGINE_DEFAULT_NUM_CHANNELS	2

// minimum safety offset, 16 when no iSub, and 45 when iSub is attached
//#define kMinimumLatency			16
#define kMinimumLatency			45
#define kMinimumLatencyiSub		45

#define kChannels				"Channels"
#define kBitDepth				"BitDepth"
#define kBitWidth				"BitWidth"
#define kSampleRates			"SampleRates"

class AppleiSubEngine;

class AppleDBDMAAudio : public IOAudioEngine
{
    OSDeclareDefaultStructors(AppleDBDMAAudio)

public:


	virtual void 		free();
	virtual UInt32 		getCurrentSampleFrame();
	virtual bool 		init(OSDictionary 			*properties,
								PlatformInterface *	inPlatformInterface,
								IOService 			*theDeviceProvider,
								bool				hasInput,
								OSArray *			formatsArray,
								UInt32				numBlocks = DBDMAAUDIODMAENGINE_DEFAULT_NUM_BLOCKS);
    virtual bool 		initHardware(IOService *provider);
    virtual IOReturn	performAudioEngineStart();
    virtual IOReturn 	performAudioEngineStop();
    IOReturn     		restartDMA();
	virtual void 		setSampleLatencies (UInt32 outputLatency, UInt32 inputLatency); 
	virtual void 		stop(IOService *provider);
	virtual bool		willTerminate (IOService * provider, IOOptionBits options);
	virtual bool 		requestTerminate ( IOService * provider, IOOptionBits options );

	virtual void		detach(IOService *provider);

	virtual IOReturn 	clipOutputSamples(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat, IOAudioStream *audioStream);
    virtual IOReturn 	convertInputSamples(const void *sampleBuf, void *destBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat, IOAudioStream *audioStream);

			void		resetiSubProcessingState();
	
			void		resetOutputClipOptions();
			void		resetInputClipOptions();
	
			void  		setPhaseInversion(const bool needsPhaseInversion); 
    inline 	bool  		getPhaseInversion() { return fNeedsPhaseInversion; };
			void  		setRightChanMixed(const bool needsRightChanMixed);
    inline	bool  		getRightChanMixed() { return fNeedsRightChanMixed; };
			void  		setRightChanDelay(const bool needsRightChanDelay);
    inline	bool  		getRightChanDelay() { return fNeedsRightChanDelay; };
	void 				setDualMonoMode(const DualMonoModeType inDualMonoMode);

    void		 		setUseSoftwareInputGain(bool inUseSoftwareInputGain);
    void	 			setInputGainL(UInt32 inGainL); 
    void		 		setInputGainR(UInt32 inGainR); 

	void				clearSoftwareEQ ();
	void				clearSoftwareLimiter ();
	void				clearSoftwareCrossover ();

	void				disableSoftwareEQ ();
	void				disableSoftwareLimiter ();
	void				disableSoftwareCrossover ();
	
	void				setEqualizationFromDictionary (OSDictionary * inDictionary);
	void				setLimiterFromDictionary (OSDictionary * inDictionary);
	void				setCrossoverFromDictionary (OSDictionary * inDictionary);
	
 	virtual void 		resetClipPosition (IOAudioStream *audioStream, UInt32 clipSampleFrame);

    virtual IOReturn	performFormatChange(IOAudioStream *audioStream, const IOAudioStreamFormat *newFormat, const IOAudioSampleRate *newSampleRate);
	
	virtual	bool		getDmaState (void );
    
    static const int 	kDBDMADeviceIndex;
    static const int 	kDBDMAOutputIndex;
    static const int 	kDBDMAInputIndex;
#if 0
	virtual bool		getRunEraseHead ( void ) { return ( false ); }
#endif

protected:
	IOMemoryDescriptor *			dmaCommandBufferInMemDescriptor;
	IOMemoryDescriptor *			dmaCommandBufferOutMemDescriptor;
	IOMemoryDescriptor *			sampleBufferInMemDescriptor;
	IOMemoryDescriptor *			sampleBufferOutMemDescriptor;
	IOMemoryDescriptor *			stopCommandInMemDescriptor;
	IOMemoryDescriptor *			stopCommandOutMemDescriptor;
    IODBDMAChannelRegisters *		ioBaseDMAOutput;
    IODBDMAChannelRegisters *		ioBaseDMAInput;
    IODBDMADescriptor *				dmaCommandBufferOut;
    IODBDMADescriptor *				dmaCommandBufferIn;
	IOAudioStream *					mOutputStream;
	IOAudioStream *					mInputStream;
	OSArray *						deviceFormats;
	void *							mOutputSampleBuffer;
	void *							mInputSampleBuffer;
    UInt32							commandBufferSize;
	IOFilterInterruptEventSource *	interruptEventSource;
    UInt32							numBlocks;
    UInt32							blockSize;
	PlatformInterface *				mPlatformObject;

	Boolean							mNeedToRestartDMA;

    // Next lines for iSub
	iSubProcessingParams_t			miSubProcessingParams;

    IOMemoryDescriptor *			iSubBufferMemory; 
	IOAudioToggleControl *			iSubAttach;
    UInt32							ourSampleFrameAtiSubLoop;
    UInt32							clipAdjustment;
    AppleOnboardAudio *				ourProvider;
    IONotifier *					iSubEngineNotifier;
    AppleiSubEngine *				iSubEngine;
	
	IOService *						mDeviceProvider;
 
    UInt32							previousClippedToFrame;
    UInt32							initialiSubLead;
	Boolean							needToSync;
    Boolean							startiSub;
    Boolean							justResetClipPosition;
   
	Boolean							restartedDMA;
	Boolean							iSubOpen;

    bool							fNeedsPhaseInversion;
	bool							fNeedsRightChanMixed;
	bool							fNeedsRightChanDelay;
	
	DualMonoModeType				mInputDualMonoMode;

    bool 							mUseSoftwareInputGain;
    float *							mInputGainLPtr;				
    float *							mInputGainRPtr;				

	EQStruct						mEQStruct;
	LimiterStruct					mLimiterStruct;
	CrossoverStruct					mCrossoverStruct;
	
	bool							dmaRunState;			//	rbm 7.12.02 added for user client support
	IOAudioStreamFormat				mDBDMAFormat;			//	rbm 7.15.02 added for user client support

    IOAudioStreamDirection			direction;

    virtual bool					filterInterrupt(int index);

	void							chooseOutputClippingRoutinePtr();
	void							chooseInputConversionRoutinePtr();

	bool							publishStreamFormats (void);
	void							allocateDMABuffers (void);
	bool							allocateOutputDMADescriptors (void);
	bool							allocateInputDMADescriptors (void);
	bool							createDMAPrograms (bool hasInput);
	void							deallocateDMAMemory ();

	void	 						iSubSynchronize(UInt32 firstSampleFrame, UInt32 numSampleFrames);
	void							updateiSubPosition(UInt32 firstSampleFrame, UInt32 numSampleFrames);
	
	static bool 					interruptFilter(OSObject *owner, IOFilterInterruptEventSource *source);
    static void 					interruptHandler(OSObject *owner, IOInterruptEventSource *source, int count);
	static IOReturn 				iSubAttachChangeHandler (IOService *target, IOAudioControl *attachControl, SInt32 oldValue, SInt32 newValue);
    static bool						iSubEnginePublished (AppleDBDMAAudio * dbdmaEngineObject, void * refCon, IOService * newService);
	static IOReturn 				iSubCloseAction (OSObject *owner, void *arg1, void *arg2, void *arg3, void *arg4);
	static IOReturn 				iSubOpenAction (OSObject *owner, void *arg1, void *arg2, void *arg3, void *arg4);

	IOReturn 						(AppleDBDMAAudio::*mClipAppleDBDMAToOutputStreamRoutine)(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat);
	IOReturn 						(AppleDBDMAAudio::*mConvertInputStreamToAppleDBDMARoutine)(const void *sampleBuf, void *destBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat);

	inline	void					startTiming();
	inline 	void					endTiming();
#ifdef _TIME_CLIP_ROUTINE
	UInt32 							mCallCount;	
	AbsoluteTime					mPreviousUptime;
#endif

#pragma mark ---------------------------------------- 
#pragma mark еее Output Conversion Routines
#pragma mark ---------------------------------------- 

	IOReturn clipAppleDBDMAToOutputStream16(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat);
	IOReturn clipAppleDBDMAToOutputStream16DelayRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat);
	IOReturn clipAppleDBDMAToOutputStream16MixRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat);
	IOReturn clipAppleDBDMAToOutputStream16InvertRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat);
	
	IOReturn clipAppleDBDMAToOutputStream32(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat);
	IOReturn clipAppleDBDMAToOutputStream32DelayRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat);
	IOReturn clipAppleDBDMAToOutputStream32MixRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat);
	
#pragma mark ---------------------------------------- 
#pragma mark еее Output Conversion Routines with iSub
#pragma mark ---------------------------------------- 
	
	IOReturn clipAppleDBDMAToOutputStream16iSub(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat);
	IOReturn clipAppleDBDMAToOutputStream16iSubDelayRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat);
	IOReturn clipAppleDBDMAToOutputStream16iSubMixRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat);
	IOReturn clipAppleDBDMAToOutputStream16iSubInvertRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat);
	
	IOReturn clipAppleDBDMAToOutputStream32iSub(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat);
	IOReturn clipAppleDBDMAToOutputStream32iSubDelayRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat);
	IOReturn clipAppleDBDMAToOutputStream32iSubMixRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat);
	
#pragma mark ---------------------------------------- 
#pragma mark еее Input Conversion Routines
#pragma mark ---------------------------------------- 
	
	IOReturn convertAppleDBDMAFromInputStream16(const void *sampleBuf, void *destBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat);
	IOReturn convertAppleDBDMAFromInputStream16CopyR2L(const void *sampleBuf, void *destBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat);
	IOReturn convertAppleDBDMAFromInputStream16WithGain(const void *sampleBuf, void *destBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat);
	
	IOReturn convertAppleDBDMAFromInputStream32(const void *sampleBuf, void *destBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat);
	IOReturn convertAppleDBDMAFromInputStream32WithGain(const void *sampleBuf, void *destBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat);

};

#endif /* _APPLEDBDMAAUDIO_H */
