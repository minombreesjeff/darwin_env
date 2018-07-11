#define DEBUGTIMESTAMPS		FALSE

#include "AppleDBDMAAudio.h"

#include <IOKit/IOMemoryDescriptor.h>
#include <IOKit/audio/IOAudioDebug.h>

#include <IOKit/IOFilterInterruptEventSource.h>
#include <IOKit/IOWorkLoop.h>

#include "AudioHardwareUtilities.h"
#include "AppleiSubEngine.h"
#include "AppleOnboardAudio.h"

#pragma mark ------------------------ 
#pragma mark еее Constants
#pragma mark ------------------------ 

#ifdef _TIME_CLIP_ROUTINE
#define kCallFrequency 10
#endif 

extern "C" {
extern vm_offset_t phystokv(vm_offset_t pa);
};

#define super IOAudioEngine

OSDefineMetaClassAndStructors(AppleDBDMAAudio, IOAudioEngine)

const int AppleDBDMAAudio::kDBDMADeviceIndex	= 0;
const int AppleDBDMAAudio::kDBDMAOutputIndex	= 1;
const int AppleDBDMAAudio::kDBDMAInputIndex		= 2;

#pragma mark ------------------------ 
#pragma mark еее IOAudioEngine Methods
#pragma mark ------------------------ 

bool AppleDBDMAAudio::filterInterrupt (int index) {
	// check to see if this interupt is because the DMA went bad
    UInt32 resultOut = IOGetDBDMAChannelStatus (ioBaseDMAOutput);
    UInt32 resultIn = 1;

	if (ioBaseDMAInput) {
		resultIn = IOGetDBDMAChannelStatus (ioBaseDMAInput) & kdbdmaActive;
	}

    if (!(resultOut & kdbdmaActive) || !resultIn) {
		mNeedToRestartDMA = TRUE;
	}

	// test the takeTimeStamp :it will increment the fCurrentLoopCount and time stamp it with the time now
	takeTimeStamp ();

    return false;
}

void AppleDBDMAAudio::free()
{
	debugIOLog("+ AppleDBDMAAudio::free()\n");

    if (interruptEventSource) {
        interruptEventSource->release();
        interruptEventSource = 0;
    }
    
	if (NULL != mOutputStream) {
		mOutputStream->release ();
		mOutputStream = NULL;
	}

	if (NULL != mInputStream) {
		mInputStream->release ();
		mInputStream = NULL;
	}

	deallocateDMAMemory ();
	
    if (NULL != miSubProcessingParams.lowFreqSamples) {
        IOFree (miSubProcessingParams.lowFreqSamples, (numBlocks * blockSize) * sizeof (float));
    }

    if (NULL != miSubProcessingParams.highFreqSamples) {
        IOFree (miSubProcessingParams.highFreqSamples, (numBlocks * blockSize) * sizeof (float));
    }

	if (NULL != deviceFormats) {
		deviceFormats->release ();
		deviceFormats = NULL;
	}

    super::free();

	debugIOLog("- AppleDBDMAAudio::free()\n");
}

UInt32 AppleDBDMAAudio::getCurrentSampleFrame()
{
	SInt32		curFrame;
#if 0
    UInt32 currentBlock = 0;
	UInt32	index;
	Boolean	found;

    if (ioBaseDMAOutput) {
        vm_offset_t currentDMACommandPhys, currentDMACommand;

        currentDMACommandPhys = (vm_offset_t)IOGetDBDMAChannelRegister(ioBaseDMAOutput, commandPtrLo);
        currentDMACommand = phystokv(currentDMACommandPhys);

        if ((UInt32)currentDMACommand > (UInt32)dmaCommandBufferOut) {
            currentBlock = ((UInt32)currentDMACommand - (UInt32)dmaCommandBufferOut) / sizeof(IODBDMADescriptor);
        }
    }

    return currentBlock * blockSize / 4;	// 4 bytes per frame - 2 per sample * 2 channels - BIG HACK
#endif

	curFrame = ((AppleOnboardAudio *)audioDevice)->getCurrentSampleFrame () % getNumSampleFramesPerBuffer ();
#if 0
	curFrame -= 128;
	if (curFrame < 0) {
		curFrame += 16384;
	}
	IOLog ("curFrame = %ld\n", curFrame);
#endif
	return curFrame;
}

bool AppleDBDMAAudio::init (OSDictionary *			properties,
								 PlatformInterface* inPlatformInterface,
                                 IOService *		theDeviceProvider,
                                 bool				hasInput,
								 OSArray *			formatsArray,
                                 UInt32				nBlocks )
{
	Boolean					result;

	debug6IOLog( "+ AppleDBDMAAudio::init ( %X, %X, %d, %X, %X )\n",
			(unsigned int)properties,
			(unsigned int)theDeviceProvider,
			(unsigned int)hasInput,
			(unsigned int)formatsArray,
			(unsigned int)nBlocks);
			
	result = FALSE;

	// Ususal check
	FailIf (FALSE == super::init (NULL), Exit);
	FailIf (NULL == theDeviceProvider, Exit);
	FailIf (NULL == formatsArray, Exit);
	FailIf (NULL == inPlatformInterface, Exit);

	mPlatformObject = inPlatformInterface;
	deviceFormats = formatsArray;
	deviceFormats->retain ();

	mDeviceProvider = theDeviceProvider; // i2s-a

	//	There is a system I/O controller dependency here.  Keylargo systems describe the DMA channel registers
	//	as having separate sets for input versus output while K2 systems describe a block of memory that
	//	encapsulates both the input and output channel registers.  Since there is a system I/O controller
	//	dependency, it would seem prudent to move the acquisition of these addresses to the appropriate
	//	platform interface instance (i.e. K2 Platform Interface or Keylargo Platform Interface).
	ioBaseDMAOutput = mPlatformObject->GetOutputChannelRegistersVirtualAddress ( mDeviceProvider );
	if ( NULL == ioBaseDMAOutput ) {
		debugIOLog ( "ioBaseDMAOutput = NULL\n" );
		IOSleep ( 50000 );
		FailIf ( true, Exit );
	}
	ioBaseDMAInput = hasInput ? mPlatformObject->GetInputChannelRegistersVirtualAddress ( mDeviceProvider ) : NULL ;
	if ( NULL == ioBaseDMAInput && hasInput ) {
		debugIOLog ( "ioBaseDMAInput = NULL\n" );
		IOSleep ( 50000 );
		FailIf ( true, Exit );
	}

	dmaCommandBufferIn = 0;
	dmaCommandBufferOut = 0;
	commandBufferSize = 0;
	interruptEventSource = 0;

	numBlocks = nBlocks;
	// blockSize will be init'ed when publishFormats calls stream->setFormat with the default format, which results in performFormatChange being called
	blockSize = 0;

	mInputDualMonoMode = e_Mode_Disabled;		   
		   
	resetiSubProcessingState();
	
	mUseSoftwareInputGain = false;	
	mInputGainLPtr = NULL;	
	mInputGainRPtr = NULL;	

	initializeEQ (&mEQStruct);
	initializeLimiter (&mLimiterStruct);
	initializeCrossover (&mCrossoverStruct);
	
	mLimiterStruct.bypassAll = true;
	mEQStruct.bypassAll = true;
	mCrossoverStruct.numBands = 0;

    mOutputSampleBuffer = IOMallocAligned(numBlocks * blockSize, PAGE_SIZE);

	mCrossoverStruct.outBufferPtr[0] = (float *)IOMallocAligned(16384, 16);
	mCrossoverStruct.outBufferPtr[1] = (float *)IOMallocAligned(16384, 16);

#ifdef _TIME_CLIP_ROUTINE
	mCallCount = 0;
	mPreviousUptime.hi = 0;
	mPreviousUptime.lo = 0;
#endif

	result = TRUE;

Exit:
	debug2IOLog( "- AppleDBDMAAudio::init returns %d\n", (unsigned int)result );
			
	return result;
}

bool AppleDBDMAAudio::initHardware (IOService *provider) {
    UInt32						interruptIndex;
    IOWorkLoop *				workLoop;
	Boolean						result;

	debugIOLog ("+ AppleDBDMAAudio::initHardware ()\n");

	result = FALSE;
    FailIf (!super::initHardware (provider), Exit);

    ourProvider = (AppleOnboardAudio *)provider;
	mNeedToRestartDMA = FALSE;

	// create the streams, this will also cause the DMA programs to be created.
	result = publishStreamFormats ();
	FailIf (FALSE == result, Exit);

	setSampleOffset (kMinimumLatency);
	// blockSize was set by the call to publishStreamFormats
	setNumSampleFramesPerBuffer (numBlocks * blockSize / ((mDBDMAFormat.fBitWidth / 8) * mDBDMAFormat.fNumChannels));

	debug4IOLog ("AppleDBDMAAudio:: setNumSampleFramesPerBuffer(%lu)  numBlocks=%lu blockSize=%lu \n",(numBlocks * blockSize / ((mDBDMAFormat.fBitWidth / 8) * mDBDMAFormat.fNumChannels)),numBlocks,blockSize);

	// install an interrupt handler only on the Output size of it !!! input only??
    workLoop = getWorkLoop();
    FailIf (!workLoop, Exit);

	debug2IOLog("AppleDBDMAudio::initHardware() interrupt source's name = %s\n", mDeviceProvider->getName());	

    interruptIndex = kDBDMAOutputIndex;
    interruptEventSource = IOFilterInterruptEventSource::filterInterruptEventSource(this,
																					AppleDBDMAAudio::interruptHandler,
																					AppleDBDMAAudio::interruptFilter,
																					mDeviceProvider,
																					interruptIndex);
    FailIf (!interruptEventSource, Exit);
    workLoop->addEventSource(interruptEventSource);
	// don't release interruptEventSource since we enable/disable it later

    iSubBufferMemory = NULL;
	iSubEngine = NULL;

	// Set up a control that sound prefs can set to tell us if we should install our notifier or not
	iSubAttach = IOAudioToggleControl::create (FALSE,
										kIOAudioControlChannelIDAll,
										kIOAudioControlChannelNameAll,
										0,
										kIOAudioToggleControlSubTypeiSubAttach,
										kIOAudioControlUsageOutput);

	// Don't release this control. We reference it when we terminate.
	if (NULL != iSubAttach) {
		addDefaultAudioControl (iSubAttach);
		iSubAttach->setValueChangeHandler ((IOAudioControl::IntValueChangeHandler)iSubAttachChangeHandler, this);
	}

	performAudioEngineStart();
	performAudioEngineStop();
	result = TRUE;

Exit:
	debug2IOLog("- AppleDBDMAAudio::initHardware() returns %d\n", result);
    return result;
}

bool AppleDBDMAAudio::publishStreamFormats (void) {
	OSArray *					sampleRatesArray;
	OSDictionary *				formatDict;
	IOAudioSampleRate			sampleRate;
	IOAudioStreamFormat			dbdmaFormat;
	UInt32						numFormats;
	UInt32						formatIndex;
	UInt32						rateIndex;
	UInt32						numSampleRates;
	bool						result;

	result = FALSE;

    if (ioBaseDMAInput) {
		mInputStream = new IOAudioStream;
	}

    mOutputStream = new IOAudioStream;
	FailIf (NULL == mOutputStream, Exit);

	mOutputStream->initWithAudioEngine (this, kIOAudioStreamDirectionOutput, 1, 0, 0);
	if (mInputStream) mInputStream->initWithAudioEngine (this, kIOAudioStreamDirectionInput, 1, 0, 0);

	dbdmaFormat.fSampleFormat = kIOAudioStreamSampleFormatLinearPCM;
	dbdmaFormat.fNumericRepresentation = kIOAudioStreamNumericRepresentationSignedInt;
	dbdmaFormat.fAlignment = kIOAudioStreamAlignmentHighByte;
	dbdmaFormat.fByteOrder = kIOAudioStreamByteOrderBigEndian;
	dbdmaFormat.fIsMixable = true;
	dbdmaFormat.fDriverTag = 0;

	sampleRate.fraction = 0;

	numFormats = deviceFormats->getCount ();
	for (formatIndex = 0; formatIndex < numFormats; formatIndex++) {
		formatDict = OSDynamicCast (OSDictionary, deviceFormats->getObject (formatIndex));
		FailIf (NULL == formatDict, Exit);
		dbdmaFormat.fNumChannels = ((OSNumber *)formatDict->getObject(kChannels))->unsigned32BitValue ();
		dbdmaFormat.fBitDepth = ((OSNumber *)formatDict->getObject(kBitDepth))->unsigned32BitValue ();
		dbdmaFormat.fBitWidth = ((OSNumber *)formatDict->getObject(kBitWidth))->unsigned32BitValue ();
		debug4IOLog("dbdmaFormat: fNumChannels = %d, fBitDepth = %d, fBitWidth = %d\n", (unsigned int)dbdmaFormat.fNumChannels, (unsigned int)dbdmaFormat.fBitDepth, (unsigned int)dbdmaFormat.fBitWidth);
		sampleRatesArray = OSDynamicCast (OSArray, formatDict->getObject (kSampleRates));
		FailIf (NULL == sampleRatesArray, Exit);
		numSampleRates = sampleRatesArray->getCount ();
		for (rateIndex = 0; rateIndex < numSampleRates; rateIndex++) {
			sampleRate.whole = ((OSNumber *)sampleRatesArray->getObject(rateIndex))->unsigned32BitValue ();
			debug2IOLog("dbdmaFormat: sampleRate.whole = %d\n", (unsigned int)sampleRate.whole);
			mOutputStream->addAvailableFormat (&dbdmaFormat, &sampleRate, &sampleRate);
			if (mInputStream) mInputStream->addAvailableFormat (&dbdmaFormat, &sampleRate, &sampleRate);
			// XXX Remove hardcoding of default format!
			if ( dbdmaFormat.fNumChannels == 2 && dbdmaFormat.fBitDepth == 16 && sampleRate.whole == 44100 ) {
				debugIOLog("dbdmaFormat: mOutputStream->setFormat to 2, 16, 44100\n");
				mOutputStream->setFormat (&dbdmaFormat);
				if (mInputStream) mInputStream->setFormat (&dbdmaFormat);
				setSampleRate (&sampleRate);
				ourProvider->formatChangeRequest (NULL, &sampleRate);
			}
		}
	}

//	allocateDMABuffers ();
    
//	FailIf (NULL == mOutputSampleBuffer, Exit);
//    if (ioBaseDMAInput) 
//        FailIf (NULL == mInputSampleBuffer, Exit);

//	mOutputStream->setSampleBuffer ((void *)mOutputSampleBuffer, numBlocks * blockSize);
//	if (mInputStream) mInputStream->setSampleBuffer ((void *)mInputSampleBuffer, numBlocks * blockSize);

	addAudioStream (mOutputStream);

	if (mInputStream) {
		addAudioStream (mInputStream);
	}

	result = TRUE;

Exit:
	return result;
}

void AppleDBDMAAudio::deallocateDMAMemory () {
    if (dmaCommandBufferOut && (commandBufferSize > 0)) {
        IOFreeAligned(dmaCommandBufferOut, commandBufferSize);
        dmaCommandBufferOut = NULL;
    }
    
    if (dmaCommandBufferIn && (commandBufferSize > 0)) {
        IOFreeAligned(dmaCommandBufferIn, commandBufferSize);
        dmaCommandBufferOut = NULL;
    }

	if (NULL != dmaCommandBufferOutMemDescriptor) {
		dmaCommandBufferOutMemDescriptor->release ();
		dmaCommandBufferOutMemDescriptor = NULL;
	}
	if (NULL != dmaCommandBufferInMemDescriptor) {
		dmaCommandBufferInMemDescriptor->release ();
		dmaCommandBufferInMemDescriptor = NULL;
	}	
	if (NULL != sampleBufferOutMemDescriptor) {
		sampleBufferOutMemDescriptor->release ();
		sampleBufferOutMemDescriptor = NULL;
	}
	if (NULL != sampleBufferInMemDescriptor) {
		sampleBufferInMemDescriptor->release ();
		sampleBufferInMemDescriptor = NULL;
	}
	if (NULL != stopCommandOutMemDescriptor) {
		stopCommandOutMemDescriptor->release ();
		stopCommandOutMemDescriptor = NULL;
	}
	if (NULL != stopCommandInMemDescriptor) {
		stopCommandInMemDescriptor->release ();
		stopCommandInMemDescriptor = NULL;
	}

	return;
}

void AppleDBDMAAudio::allocateDMABuffers (void) {

	debug2IOLog ("allocateDMABuffers: buffer size = %ld\n", numBlocks * blockSize);

    mOutputSampleBuffer = IOMallocAligned(numBlocks * blockSize, PAGE_SIZE);

	if(ioBaseDMAInput) {
        mInputSampleBuffer = IOMallocAligned(numBlocks * blockSize, PAGE_SIZE);
	}
}

bool AppleDBDMAAudio::allocateOutputDMADescriptors (void) {
	bool						result;

	result = FALSE;

    commandBufferSize = (numBlocks + 1) * sizeof(IODBDMADescriptor);
    dmaCommandBufferOut = (IODBDMADescriptor *)IOMallocAligned(commandBufferSize, 32);	// needs to be more than 4 byte aligned
    FailIf (!dmaCommandBufferOut, Exit);

	dmaCommandBufferOutMemDescriptor = IOMemoryDescriptor::withAddress (dmaCommandBufferOut, commandBufferSize, kIODirectionOutIn);
	FailIf (NULL == dmaCommandBufferOutMemDescriptor, Exit);
	sampleBufferOutMemDescriptor = IOMemoryDescriptor::withAddress (mOutputSampleBuffer, numBlocks * blockSize, kIODirectionOutIn);
	FailIf (NULL == sampleBufferOutMemDescriptor, Exit);
	stopCommandOutMemDescriptor = IOMemoryDescriptor::withAddress (&dmaCommandBufferOut[numBlocks], sizeof (IODBDMADescriptor *), kIODirectionOutIn);
	FailIf (NULL == stopCommandOutMemDescriptor, Exit);

	result = TRUE;

Exit:
	return result;
}

bool AppleDBDMAAudio::allocateInputDMADescriptors (void) {
	bool						result;

	result = FALSE;

	if(ioBaseDMAInput) {
		commandBufferSize = (numBlocks + 1) * sizeof(IODBDMADescriptor);	// needs to be more than 4 byte aligned
		
		dmaCommandBufferIn = (IODBDMADescriptor *)IOMallocAligned(commandBufferSize, 32);	// needs to be more than 4 byte aligned
        FailIf (!dmaCommandBufferIn, Exit);

		dmaCommandBufferInMemDescriptor = IOMemoryDescriptor::withAddress (dmaCommandBufferIn, commandBufferSize, kIODirectionOutIn);
		FailIf (NULL == dmaCommandBufferInMemDescriptor, Exit);
		sampleBufferInMemDescriptor = IOMemoryDescriptor::withAddress (mInputSampleBuffer, numBlocks * blockSize, kIODirectionOutIn);
		FailIf (NULL == sampleBufferInMemDescriptor, Exit);
		stopCommandInMemDescriptor = IOMemoryDescriptor::withAddress (&dmaCommandBufferIn[numBlocks], sizeof (IODBDMADescriptor *), kIODirectionOutIn);
		FailIf (NULL == stopCommandInMemDescriptor, Exit);
	}

	result = TRUE;

Exit:
	return result;
}

bool AppleDBDMAAudio::createDMAPrograms (bool hasInput) {
	vm_offset_t					offset;
	IOPhysicalAddress			commandBufferPhys;
	IOPhysicalAddress			sampleBufferPhys;
	IOPhysicalAddress			stopCommandPhys;
	UInt32						dmaCommand;
	UInt32						blockNum;
	Boolean						doInterrupt;
	bool						result;

	result = FALSE;			// Didn't successfully create DMA program

    offset = 0;
    dmaCommand = kdbdmaOutputMore;
	doInterrupt = false;

	result = allocateOutputDMADescriptors ();
	FailIf (FALSE == result, Exit);

	commandBufferPhys = dmaCommandBufferOutMemDescriptor->getPhysicalAddress ();
	FailIf (NULL == commandBufferPhys, Exit);
	sampleBufferPhys = sampleBufferOutMemDescriptor->getPhysicalAddress ();
	FailIf (NULL == sampleBufferPhys, Exit);
	stopCommandPhys = stopCommandOutMemDescriptor->getPhysicalAddress ();
	FailIf (NULL == stopCommandPhys, Exit);

    for (blockNum = 0; blockNum < numBlocks; blockNum++) {
		IOPhysicalAddress	cmdDest;

        if (offset >= PAGE_SIZE) {
			sampleBufferPhys = sampleBufferOutMemDescriptor->getPhysicalSegment (blockNum * blockSize, 0);
			FailIf (NULL == sampleBufferPhys, Exit);
            offset = 0;
        }

        // This code assumes that the size of the IODBDMADescriptor divides evenly into the page size
        // If this is the last block, branch to the first block
        if (blockNum == (numBlocks - 1)) {
            cmdDest = commandBufferPhys;
            doInterrupt = true;
        // Else if the next block starts on a page boundry, branch to it
        } else if ((((blockNum + 1) * sizeof(IODBDMADescriptor)) % PAGE_SIZE) == 0) {
			cmdDest = dmaCommandBufferOutMemDescriptor->getPhysicalSegment ((blockNum + 1) * sizeof(IODBDMADescriptor), 0);
			FailIf (NULL == cmdDest, Exit);
		// No branch in the common case
        } else {
            cmdDest = 0;
        }

        if (cmdDest) {
            IOMakeDBDMADescriptorDep(&dmaCommandBufferOut[blockNum],
                                     dmaCommand,
                                     kdbdmaKeyStream0,
                                     doInterrupt ? kdbdmaIntAlways : kdbdmaIntNever,
                                     kdbdmaBranchAlways,
                                     kdbdmaWaitNever,
                                     blockSize,
                                     sampleBufferPhys + offset,
                                     cmdDest);
        } else {
            IOMakeDBDMADescriptorDep(&dmaCommandBufferOut[blockNum],
                                     dmaCommand,
                                     kdbdmaKeyStream0,
                                     kdbdmaIntNever,
                                     kdbdmaBranchIfTrue,
                                     kdbdmaWaitNever,
                                     blockSize,
                                     sampleBufferPhys + offset,
                                     stopCommandPhys);
        }
        offset += blockSize;
    }

    IOMakeDBDMADescriptor(&dmaCommandBufferOut[blockNum],
                          kdbdmaStop,
                          kdbdmaKeyStream0,
                          kdbdmaIntNever,
                          kdbdmaBranchNever,
                          kdbdmaWaitNever,
                          0,
                          0);
    
	// create the DMA input code
    if(ioBaseDMAInput) {
        doInterrupt = false;
        offset = 0;
        dmaCommand = kdbdmaInputMore;    
        
		result = allocateInputDMADescriptors ();
		FailIf (FALSE == result, Exit);

		commandBufferPhys = dmaCommandBufferInMemDescriptor->getPhysicalAddress ();
		FailIf (NULL == commandBufferPhys, Exit);
		sampleBufferPhys = sampleBufferInMemDescriptor->getPhysicalAddress ();
		FailIf (NULL == sampleBufferPhys, Exit);
		stopCommandPhys = stopCommandInMemDescriptor->getPhysicalAddress ();
		FailIf (NULL == stopCommandPhys, Exit);

        for (blockNum = 0; blockNum < numBlocks; blockNum++) {
			IOPhysicalAddress	cmdDest;

            if (offset >= PAGE_SIZE) {
				sampleBufferPhys = sampleBufferInMemDescriptor->getPhysicalSegment (blockNum * blockSize, 0);
				FailIf (NULL == sampleBufferPhys, Exit);
                offset = 0;
            }

                // This code assumes that the size of the IODBDMADescriptor 
                // divides evenly into the page size
                // If this is the last block, branch to the first block
            if (blockNum == (numBlocks - 1)) {
                cmdDest = commandBufferPhys;
                // doInterrupt = true;
                // Else if the next block starts on a page boundry, branch to it
            } else if ((((blockNum + 1) * sizeof(IODBDMADescriptor)) % PAGE_SIZE) == 0) {
				cmdDest = dmaCommandBufferInMemDescriptor->getPhysicalSegment ((blockNum + 1) * sizeof(IODBDMADescriptor), 0);
				FailIf (NULL == cmdDest, Exit);
                // No branch in the common case
            } else {
                cmdDest = 0;
            }

            if (cmdDest) {
                IOMakeDBDMADescriptorDep(&dmaCommandBufferIn[blockNum],
                                     dmaCommand,
                                     kdbdmaKeyStream0,
                                     doInterrupt ? kdbdmaIntAlways : kdbdmaIntNever,
                                     kdbdmaBranchAlways,
                                     kdbdmaWaitNever,
                                     blockSize,
                                     sampleBufferPhys + offset,
                                     cmdDest);
            } else {
                IOMakeDBDMADescriptorDep(&dmaCommandBufferIn[blockNum],
                                     dmaCommand,
                                     kdbdmaKeyStream0,
                                     kdbdmaIntNever,
                                     kdbdmaBranchIfTrue,
                                     kdbdmaWaitNever,
                                     blockSize,
                                     sampleBufferPhys + offset,
                                     stopCommandPhys);
            }
            offset += blockSize;
        }

        IOMakeDBDMADescriptor(&dmaCommandBufferIn[blockNum],
                          kdbdmaStop,
                          kdbdmaKeyStream0,
                          kdbdmaIntNever,
                          kdbdmaBranchNever,
                          kdbdmaWaitNever,
                          0,
                          0);

    }

	result = TRUE;

Exit:
	return result;
}

bool AppleDBDMAAudio::interruptFilter(OSObject *owner, IOFilterInterruptEventSource *source)
{
    register AppleDBDMAAudio *dmaEngine = (AppleDBDMAAudio *)owner;
    bool result = true;

    if (dmaEngine) {
        result = dmaEngine->filterInterrupt(source->getIntIndex());
    }

    return result;
}

void AppleDBDMAAudio::interruptHandler(OSObject *owner, IOInterruptEventSource *source, int count)
{
    return;
}

IOReturn AppleDBDMAAudio::performAudioEngineStart()
{
	IOPhysicalAddress			commandBufferPhys;
	IOReturn					result;
#if DEBUGLOG
	IOLog(" + AppleDBDMAAudio::performAudioEngineStart()\n");
#endif
	result = kIOReturnError;
    FailIf (!ioBaseDMAOutput || !dmaCommandBufferOut || !status || !interruptEventSource, Exit);

    flush_dcache((vm_offset_t)dmaCommandBufferOut, commandBufferSize, false);
    if(ioBaseDMAInput)
        flush_dcache((vm_offset_t)dmaCommandBufferIn, commandBufferSize, false);

	resetiSubProcessingState();

    if (NULL != iSubEngine) {
		startiSub = TRUE;
		needToSync = TRUE;
    }

    interruptEventSource->enable();

	// add the time stamp take to test
    takeTimeStamp(false);

#if DEBUGLOG
	IOLog ( "getNumSampleFramesPerBuffer %ld\n", getNumSampleFramesPerBuffer() );
#endif
	((AppleOnboardAudio *)audioDevice)->setCurrentSampleFrame (0);
	if ( mPlatformObject ) { mPlatformObject->LogDBDMAChannelRegisters(); }

	// start the input DMA first
    if(ioBaseDMAInput) {
        IOSetDBDMAChannelControl(ioBaseDMAInput, IOClearDBDMAChannelControlBits(kdbdmaS0));
        IOSetDBDMABranchSelect(ioBaseDMAInput, IOSetDBDMAChannelControlBits(kdbdmaS0));
		commandBufferPhys = dmaCommandBufferInMemDescriptor->getPhysicalAddress ();
		FailIf (NULL == commandBufferPhys, Exit);
		IODBDMAStart(ioBaseDMAInput, (IODBDMADescriptor *)commandBufferPhys);
#if DEBUGLOG
		IOLog("AppleDBDMAAudio::performAudioEngineStart - starting input DMA\n");
#endif
   }
    
    IOSetDBDMAChannelControl(ioBaseDMAOutput, IOClearDBDMAChannelControlBits(kdbdmaS0));
    IOSetDBDMABranchSelect(ioBaseDMAOutput, IOSetDBDMAChannelControlBits(kdbdmaS0));
	commandBufferPhys = dmaCommandBufferOutMemDescriptor->getPhysicalAddress ();
	FailIf (NULL == commandBufferPhys, Exit);
	IODBDMAStart(ioBaseDMAOutput, (IODBDMADescriptor *)commandBufferPhys);
	if ( mPlatformObject ) { mPlatformObject->LogDBDMAChannelRegisters(); }

	dmaRunState = TRUE;				//	rbm 7.12.02	added for user client support
	result = kIOReturnSuccess;

#if DEBUGLOG
    IOLog(" - AppleDBDMAAudio::performAudioEngineStart() returns %X\n", result);
#endif

Exit:
    return result;
}

IOReturn AppleDBDMAAudio::performAudioEngineStop()
{
    UInt16 attemptsToStop = 1000;

    debugIOLog("+ AppleDBDMAAudio::performAudioEngineStop()\n");

    if (NULL != iSubEngine) {
        iSubEngine->StopiSub ();
        needToSync = TRUE;
    }

    if (!interruptEventSource) {
        return kIOReturnError;
    }

    interruptEventSource->disable();
        
	// stop the output
	if ( NULL != ioBaseDMAOutput ) {
		IOSetDBDMAChannelControl(ioBaseDMAOutput, IOSetDBDMAChannelControlBits(kdbdmaS0));
		while ((IOGetDBDMAChannelStatus(ioBaseDMAOutput) & kdbdmaActive) && (attemptsToStop--)) {
			eieio();
			IOSleep(1);
		}
	
		IODBDMAStop(ioBaseDMAOutput);
		IODBDMAReset(ioBaseDMAOutput);
	} else {
		debugIOLog ( "AppleDBDMAAudio::performAudioEngineStop has no output\n" );
	}

	// stop the input
    if(ioBaseDMAInput){
        IOSetDBDMAChannelControl(ioBaseDMAInput, IOSetDBDMAChannelControlBits(kdbdmaS0));
        while ((IOGetDBDMAChannelStatus(ioBaseDMAInput) & kdbdmaActive) && (attemptsToStop--)) {
            eieio();
            IOSleep(1);
        }

        IODBDMAStop(ioBaseDMAInput);
        IODBDMAReset(ioBaseDMAInput);
	} else {
		debugIOLog ( "AppleDBDMAAudio::performAudioEngineStop has no input\n" );
    }
    
	dmaRunState = FALSE;				//	rbm 7.12.02	added for user client support
    interruptEventSource->enable();

    DEBUG_IOLOG("- AppleDBDMAAudio::performAudioEngineStop()\n");
    return kIOReturnSuccess;
}

// This gets called when a new audio stream needs to be mixed into an already playing audio stream
void AppleDBDMAAudio::resetClipPosition (IOAudioStream *audioStream, UInt32 clipSampleFrame) {
  if ((NULL != iSubBufferMemory) && (NULL != iSubEngine)) {
				
		resetiSubProcessingState();

		#if DEBUGLOG
        debug4IOLog ("+resetClipPosition: iSubBufferOffset=%ld, previousClippedToFrame=%ld, clipSampleFrame=%ld\n", miSubProcessingParams.iSubBufferOffset, previousClippedToFrame, clipSampleFrame);
		#endif
        if (previousClippedToFrame < clipSampleFrame) {
			// Resetting the clip point backwards around the end of the buffer
			clipAdjustment = (getNumSampleFramesPerBuffer () - clipSampleFrame + previousClippedToFrame) * iSubEngine->GetNumChannels();
        } else {
			clipAdjustment = (previousClippedToFrame - clipSampleFrame) * iSubEngine->GetNumChannels();
        }
		#if DEBUGLOG
        if (clipAdjustment < kMinimumLatency) {
            debug2IOLog ("resetClipPosition: 44.1 clipAdjustment < min, clipAdjustment=%ld\n", clipAdjustment); 
        }                
		#endif
        clipAdjustment = (clipAdjustment * 1000) / ((1000 * getSampleRate()->whole) / iSubEngine->GetSampleRate());  
        miSubProcessingParams.iSubBufferOffset -= clipAdjustment;

		#if DEBUGLOG
        if (clipAdjustment > (iSubBufferMemory->getLength () / 2)) {
            debug2IOLog ("resetClipPosition: clipAdjustment > iSub buffer size, clipAdjustment=%ld\n", clipAdjustment); 
        }                
		#endif

        if (miSubProcessingParams.iSubBufferOffset < 0) {
			miSubProcessingParams.iSubBufferOffset += (iSubBufferMemory->getLength () / 2);	
			miSubProcessingParams.iSubLoopCount--;
        }

        previousClippedToFrame = clipSampleFrame;
        justResetClipPosition = TRUE;

		#if DEBUGLOG
        debug3IOLog ("-resetClipPosition: iSubBufferOffset=%ld, previousClippedToFrame=%ld\n", miSubProcessingParams.iSubBufferOffset, previousClippedToFrame);
		#endif
    }
}

IOReturn AppleDBDMAAudio::restartDMA () {
	IOReturn					result;

	result = kIOReturnError;
    FailIf (!ioBaseDMAOutput || !dmaCommandBufferOut || !interruptEventSource, Exit);

	performAudioEngineStop ();
	performAudioEngineStart ();
	result = kIOReturnSuccess;

Exit:
    return result;
}

void AppleDBDMAAudio::setSampleLatencies (UInt32 outputLatency, UInt32 inputLatency) {
	setOutputSampleLatency (outputLatency);
	setInputSampleLatency (inputLatency);
}

void AppleDBDMAAudio::stop(IOService *provider)
{
    IOWorkLoop *workLoop;
    
    DEBUG3_IOLOG(" + AppleDBDMAAudio[%p]::stop(%p)\n", this, provider);
    
    if (interruptEventSource) {
        workLoop = getWorkLoop();
        if (workLoop) {
            workLoop->removeEventSource(interruptEventSource);
        }
    }
 	
    if (NULL != iSubEngineNotifier) {
        iSubEngineNotifier->remove ();
		iSubEngineNotifier = NULL;
    }
   
    super::stop(provider);
    stopAudioEngine();
    DEBUG3_IOLOG(" - AppleDBDMAAudio[%p]::stop(%p)\n", this, provider);
}


void AppleDBDMAAudio::detach(IOService *provider)
{
	super::detach(provider);
    debug2IOLog("AppleDBDMAAudio::detach(%p)\n", provider);
}

#pragma mark ------------------------ 
#pragma mark еее Conversion Routines
#pragma mark ------------------------ 

// [3094574] aml, pick the correct output conversion routine based on our current state
void AppleDBDMAAudio::chooseOutputClippingRoutinePtr()
{
	if ((NULL != iSubBufferMemory) && (NULL != iSubEngine)) {
		if (32 == mDBDMAFormat.fBitWidth) {
			if (TRUE == fNeedsRightChanMixed) {
				mClipAppleDBDMAToOutputStreamRoutine = &AppleDBDMAAudio::clipAppleDBDMAToOutputStream32iSubMixRightChannel;
			} else if (TRUE == fNeedsRightChanDelay) {
				mClipAppleDBDMAToOutputStreamRoutine = &AppleDBDMAAudio::clipAppleDBDMAToOutputStream32iSubDelayRightChannel;
			} else {
				mClipAppleDBDMAToOutputStreamRoutine = &AppleDBDMAAudio::clipAppleDBDMAToOutputStream32iSub;
			}
		} else if (16 == mDBDMAFormat.fBitWidth) {
			if (TRUE == fNeedsPhaseInversion) {
				mClipAppleDBDMAToOutputStreamRoutine = &AppleDBDMAAudio::clipAppleDBDMAToOutputStream16iSubInvertRightChannel;
			} else if (TRUE == fNeedsRightChanMixed) {
				mClipAppleDBDMAToOutputStreamRoutine = &AppleDBDMAAudio::clipAppleDBDMAToOutputStream16iSubMixRightChannel;
			} else if (TRUE == fNeedsRightChanDelay) {
				mClipAppleDBDMAToOutputStreamRoutine = &AppleDBDMAAudio::clipAppleDBDMAToOutputStream16iSubDelayRightChannel;
			} else {
				mClipAppleDBDMAToOutputStreamRoutine = &AppleDBDMAAudio::clipAppleDBDMAToOutputStream16iSub;
			}
		} else {
			debugIOLog("AppleDBDMAAudio::chooseOutputClippingRoutinePtr - iSub attached, non-supported output bit depth!\n");
		}
	} else {
		if (32 == mDBDMAFormat.fBitWidth) {
			if (TRUE == fNeedsRightChanMixed) {
				mClipAppleDBDMAToOutputStreamRoutine = &AppleDBDMAAudio::clipAppleDBDMAToOutputStream32MixRightChannel;
			} else if (TRUE == fNeedsRightChanDelay) {
				mClipAppleDBDMAToOutputStreamRoutine = &AppleDBDMAAudio::clipAppleDBDMAToOutputStream32DelayRightChannel;
			} else {
				mClipAppleDBDMAToOutputStreamRoutine = &AppleDBDMAAudio::clipAppleDBDMAToOutputStream32;
			}
		} else if (16 == mDBDMAFormat.fBitWidth) {
			if (TRUE == fNeedsPhaseInversion) {
				mClipAppleDBDMAToOutputStreamRoutine = &AppleDBDMAAudio::clipAppleDBDMAToOutputStream16InvertRightChannel;
			} else if (TRUE == fNeedsRightChanMixed) {
				mClipAppleDBDMAToOutputStreamRoutine = &AppleDBDMAAudio::clipAppleDBDMAToOutputStream16MixRightChannel;
			} else if (TRUE == fNeedsRightChanDelay) {
				mClipAppleDBDMAToOutputStreamRoutine = &AppleDBDMAAudio::clipAppleDBDMAToOutputStream16DelayRightChannel;
			} else {
				mClipAppleDBDMAToOutputStreamRoutine = &AppleDBDMAAudio::clipAppleDBDMAToOutputStream16;
			}
		} else {
			debugIOLog("AppleDBDMAAudio::chooseOutputClippingRoutinePtr - Non-supported output bit depth!\n");
		}
	}
}

// [3094574] aml, pick the correct input conversion routine based on our current state
void AppleDBDMAAudio::chooseInputConversionRoutinePtr() 
{
	if (32 == mDBDMAFormat.fBitWidth) {
		if (mUseSoftwareInputGain) {
			mConvertInputStreamToAppleDBDMARoutine = &AppleDBDMAAudio::convertAppleDBDMAFromInputStream32WithGain;
		} else {
			mConvertInputStreamToAppleDBDMARoutine = &AppleDBDMAAudio::convertAppleDBDMAFromInputStream32;
		}
	} else if (16 == mDBDMAFormat.fBitWidth) {
		if (mUseSoftwareInputGain) {
			mConvertInputStreamToAppleDBDMARoutine = &AppleDBDMAAudio::convertAppleDBDMAFromInputStream16WithGain;
		} else {
			if (e_Mode_CopyRightToLeft == mInputDualMonoMode) {
				mConvertInputStreamToAppleDBDMARoutine = &AppleDBDMAAudio::convertAppleDBDMAFromInputStream16CopyR2L;
			} else {
				mConvertInputStreamToAppleDBDMARoutine = &AppleDBDMAAudio::convertAppleDBDMAFromInputStream16;
			}
		}
	} else {
		debugIOLog("AppleDBDMAAudio::chooseInputConversionRoutinePtr - Non-supported input bit depth!\n");
	}
}

IOReturn AppleDBDMAAudio::clipOutputSamples(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat, IOAudioStream *audioStream)
{
	IOReturn result;
 
 	// if the DMA went bad restart it
	if (mNeedToRestartDMA) {
		mNeedToRestartDMA = FALSE;
		restartDMA ();
	}

	startTiming();

	// [3094574] aml, use function pointer instead of if/else block - handles both iSub and non-iSub clipping cases.
	result = (*this.*mClipAppleDBDMAToOutputStreamRoutine)(mixBuf, sampleBuf, firstSampleFrame, numSampleFrames, streamFormat);

	endTiming();

	return result;
}

// [3094574] aml, use function pointer instead of if/else block
IOReturn AppleDBDMAAudio::convertInputSamples(const void *sampleBuf, void *destBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat, IOAudioStream *audioStream)
{
	IOReturn result;

	result = (*this.*mConvertInputStreamToAppleDBDMARoutine)(sampleBuf, destBuf, firstSampleFrame, numSampleFrames, streamFormat);

	return result;
}

#pragma mark ------------------------ 
#pragma mark еее Output Routines
#pragma mark ------------------------ 

// ------------------------------------------------------------------------
// Float32 to Native SInt16
// ------------------------------------------------------------------------
IOReturn AppleDBDMAAudio::clipAppleDBDMAToOutputStream16(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
{
    float	*inFloatBufferPtr;
    SInt16	*outSInt16BufferPtr;
	UInt32	numSamples;

	numSamples = numSampleFrames*streamFormat->fNumChannels;
    inFloatBufferPtr = (float *)mixBuf+firstSampleFrame*streamFormat->fNumChannels;
	outSInt16BufferPtr = (SInt16 *)sampleBuf+firstSampleFrame * streamFormat->fNumChannels;
	
	Float32ToNativeInt16( inFloatBufferPtr, outSInt16BufferPtr, numSamples );

    return kIOReturnSuccess;
}

// ------------------------------------------------------------------------
// Float32 to Native SInt16, delay right channel to correct for TAS 3004
// I2S clocking issue which puts left and right samples out of phase.
// ------------------------------------------------------------------------
IOReturn AppleDBDMAAudio::clipAppleDBDMAToOutputStream16DelayRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
{

    float	*inFloatBufferPtr;
    SInt16	*outSInt16BufferPtr;
	UInt32	numSamples;

	numSamples = numSampleFrames*streamFormat->fNumChannels;
    inFloatBufferPtr = (float *)mixBuf+firstSampleFrame*streamFormat->fNumChannels;
	outSInt16BufferPtr = (SInt16 *)sampleBuf+firstSampleFrame * streamFormat->fNumChannels;
	
	delayRightChannel( inFloatBufferPtr, numSamples );
	
	Float32ToNativeInt16( inFloatBufferPtr, outSInt16BufferPtr, numSamples );

    return kIOReturnSuccess;
}

// ------------------------------------------------------------------------
// Float32 to Native SInt16, invert phase to correct older iMac hardware
// assumes 2 channels.  Note that there is no 32 bit version of this 
// conversion routine, since the older hardware does not support it.
// ------------------------------------------------------------------------
IOReturn AppleDBDMAAudio::clipAppleDBDMAToOutputStream16InvertRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
{
    float	*inFloatBufferPtr;
    SInt16	*outSInt16BufferPtr;
	UInt32	numSamples;
	
	numSamples = numSampleFrames*streamFormat->fNumChannels;
    inFloatBufferPtr = (float *)mixBuf+firstSampleFrame*streamFormat->fNumChannels;
	outSInt16BufferPtr = (SInt16 *)sampleBuf+firstSampleFrame * streamFormat->fNumChannels;

	invertRightChannel( inFloatBufferPtr, numSamples );
 
	Float32ToNativeInt16( inFloatBufferPtr, outSInt16BufferPtr, numSamples );
   
	return kIOReturnSuccess;
}

// ------------------------------------------------------------------------
// Float32 to Native SInt16, mix right and left channels and mute right
// assumes 2 channels
// ------------------------------------------------------------------------
IOReturn AppleDBDMAAudio::clipAppleDBDMAToOutputStream16MixRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
{
    float	*inFloatBufferPtr;
    SInt16	*outSInt16BufferPtr;
	UInt32	numSamples;

	numSamples = numSampleFrames*streamFormat->fNumChannels;
    inFloatBufferPtr = (float *)mixBuf+firstSampleFrame*streamFormat->fNumChannels;
	outSInt16BufferPtr = (SInt16 *)sampleBuf+firstSampleFrame * streamFormat->fNumChannels;
	
	mixAndMuteRightChannel( inFloatBufferPtr, numSamples );
	
	Float32ToNativeInt16( inFloatBufferPtr, outSInt16BufferPtr, numSamples );

    return kIOReturnSuccess;
}

// ------------------------------------------------------------------------
// Float32 to Native SInt32
// ------------------------------------------------------------------------
IOReturn AppleDBDMAAudio::clipAppleDBDMAToOutputStream32(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
{
    float	*inFloatBufferPtr;
	SInt32	*outSInt32BufferPtr;
	UInt32	numSamples;

	numSamples = numSampleFrames*streamFormat->fNumChannels;
    inFloatBufferPtr = (float *)mixBuf+firstSampleFrame*streamFormat->fNumChannels;
	outSInt32BufferPtr = (SInt32 *)sampleBuf + firstSampleFrame * streamFormat->fNumChannels;
	
	Float32ToNativeInt32( inFloatBufferPtr, outSInt32BufferPtr, numSamples );

    return kIOReturnSuccess;
}

// ------------------------------------------------------------------------
// Float32 to Native SInt32, delay right channel to correct for TAS 3004
// I2S clocking issue which puts left and right samples out of phase.
// ------------------------------------------------------------------------
IOReturn AppleDBDMAAudio::clipAppleDBDMAToOutputStream32DelayRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
{

    float	*inFloatBufferPtr;
	SInt32	*outSInt32BufferPtr;
	UInt32	numSamples;

	numSamples = numSampleFrames*streamFormat->fNumChannels;
    inFloatBufferPtr = (float *)mixBuf+firstSampleFrame*streamFormat->fNumChannels;
	outSInt32BufferPtr = (SInt32 *)sampleBuf + firstSampleFrame * streamFormat->fNumChannels;
	
	delayRightChannel( inFloatBufferPtr, numSamples );

	Float32ToNativeInt32( inFloatBufferPtr, outSInt32BufferPtr, numSamples );
	
    return kIOReturnSuccess;
}

// ------------------------------------------------------------------------
// Float32 to Native SInt32, mix right and left channels and mute right
// assumes 2 channels
// ------------------------------------------------------------------------
IOReturn AppleDBDMAAudio::clipAppleDBDMAToOutputStream32MixRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
{
    float	*inFloatBufferPtr;
	SInt32	*outSInt32BufferPtr;
	UInt32	numSamples;

	numSamples = numSampleFrames*streamFormat->fNumChannels;
    inFloatBufferPtr = (float *)mixBuf+firstSampleFrame*streamFormat->fNumChannels;
	outSInt32BufferPtr = (SInt32 *)sampleBuf + firstSampleFrame * streamFormat->fNumChannels;
	
	mixAndMuteRightChannel( inFloatBufferPtr, numSamples );

	Float32ToNativeInt32( inFloatBufferPtr, outSInt32BufferPtr, numSamples );

    return kIOReturnSuccess;
}

#pragma mark ------------------------ 
#pragma mark еее iSub Output Routines
#pragma mark ------------------------ 

// ------------------------------------------------------------------------
// Float32 to Native SInt16 with iSub, assumes 2 channel data
// ------------------------------------------------------------------------
IOReturn AppleDBDMAAudio::clipAppleDBDMAToOutputStream16iSub(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
{
    UInt32 		maxSampleIndex, numSamples;
    float*		floatMixBuf;
    SInt16*	outputBuf16;
	UInt32		sampleIndex;

	iSubSynchronize(firstSampleFrame, numSampleFrames);

	PreviousValues* filterState = &(miSubProcessingParams.filterState);
	PreviousValues* filterState2 = &(miSubProcessingParams.filterState2);
	PreviousValues* phaseCompState = &(miSubProcessingParams.phaseCompState);
	UInt32* loopCount = &(miSubProcessingParams.iSubLoopCount);
	SInt32* iSubBufferOffset = &(miSubProcessingParams.iSubBufferOffset);
	float* srcPhase = &(miSubProcessingParams.srcPhase);
	float* srcState = &(miSubProcessingParams.srcState);

	float* low = miSubProcessingParams.lowFreqSamples;
	float* high = miSubProcessingParams.highFreqSamples;
	UInt32 sampleRate = miSubProcessingParams.sampleRate;
	UInt32 adaptiveSampleRate = miSubProcessingParams.adaptiveSampleRate;
	SInt16* iSubBufferMemory = miSubProcessingParams.iSubBuffer;
	UInt32 iSubBufferLen = miSubProcessingParams.iSubBufferLen;
	UInt32 outputSampleRate = miSubProcessingParams.iSubFormat.outputSampleRate;

    floatMixBuf = (float *)mixBuf;
	numSamples = numSampleFrames * streamFormat->fNumChannels;
    maxSampleIndex = (firstSampleFrame + numSampleFrames) * streamFormat->fNumChannels;

    // Filter audio into low and high buffers using a 24 dB/octave crossover
	StereoFilter4thOrderPhaseComp (&floatMixBuf[firstSampleFrame * streamFormat->fNumChannels], &low[firstSampleFrame * streamFormat->fNumChannels], &high[firstSampleFrame * streamFormat->fNumChannels], numSampleFrames, sampleRate, filterState, filterState2, phaseCompState);

    // high side 
	outputBuf16 = (SInt16 *)sampleBuf+firstSampleFrame * streamFormat->fNumChannels;
	Float32ToNativeInt16( &high[firstSampleFrame * streamFormat->fNumChannels], outputBuf16, numSamples );

    // low side
 	sampleIndex = (firstSampleFrame * streamFormat->fNumChannels);
	iSubDownSampleLinearAndConvert( low, srcPhase, srcState, adaptiveSampleRate, outputSampleRate, sampleIndex, maxSampleIndex, iSubBufferMemory, iSubBufferOffset, iSubBufferLen, loopCount );	

	updateiSubPosition(firstSampleFrame, numSampleFrames);
		
	return kIOReturnSuccess;
}

// ------------------------------------------------------------------------
// Float32 to Native SInt16 with iSub, mix and mute right channel
// ------------------------------------------------------------------------
IOReturn AppleDBDMAAudio::clipAppleDBDMAToOutputStream16iSubDelayRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
{
    UInt32 		sampleIndex, maxSampleIndex, numSamples;
    float *		floatMixBuf;
    SInt16 *	outputBuf16;

	iSubSynchronize(firstSampleFrame, numSampleFrames);

	PreviousValues* filterState = &(miSubProcessingParams.filterState);
	PreviousValues* filterState2 = &(miSubProcessingParams.filterState2);
	PreviousValues* phaseCompState = &(miSubProcessingParams.phaseCompState);
	UInt32* loopCount = &(miSubProcessingParams.iSubLoopCount);
	SInt32* iSubBufferOffset = &(miSubProcessingParams.iSubBufferOffset);
	float* srcPhase = &(miSubProcessingParams.srcPhase);
	float* srcState = &(miSubProcessingParams.srcState);

	float* low = miSubProcessingParams.lowFreqSamples;
	float* high = miSubProcessingParams.highFreqSamples;
	UInt32 sampleRate = miSubProcessingParams.sampleRate;
	UInt32 adaptiveSampleRate = miSubProcessingParams.adaptiveSampleRate;
	SInt16* iSubBufferMemory = miSubProcessingParams.iSubBuffer;
	UInt32 iSubBufferLen = miSubProcessingParams.iSubBufferLen;
	UInt32 outputSampleRate = miSubProcessingParams.iSubFormat.outputSampleRate;

    floatMixBuf = (float *)mixBuf;
	numSamples = numSampleFrames * streamFormat->fNumChannels;
    maxSampleIndex = (firstSampleFrame + numSampleFrames) * streamFormat->fNumChannels;

    // Filter audio into low and high buffers using a 24 dB/octave crossover
	StereoFilter4thOrderPhaseComp (&floatMixBuf[firstSampleFrame * streamFormat->fNumChannels], &low[firstSampleFrame * streamFormat->fNumChannels], &high[firstSampleFrame * streamFormat->fNumChannels], numSampleFrames, sampleRate, filterState, filterState2, phaseCompState);

    // high side 
	outputBuf16 = (SInt16 *)sampleBuf+firstSampleFrame * streamFormat->fNumChannels;
	delayRightChannel(  &high[firstSampleFrame * streamFormat->fNumChannels], numSamples );
	Float32ToNativeInt16( &high[firstSampleFrame * streamFormat->fNumChannels], outputBuf16, numSamples );

    // low side
 	sampleIndex = (firstSampleFrame * streamFormat->fNumChannels);
	iSubDownSampleLinearAndConvert( low, srcPhase, srcState, adaptiveSampleRate, outputSampleRate, sampleIndex, maxSampleIndex, iSubBufferMemory, iSubBufferOffset, iSubBufferLen, loopCount );	
		
	updateiSubPosition(firstSampleFrame, numSampleFrames);
		
	return kIOReturnSuccess;
}

// ------------------------------------------------------------------------
// Float32 to Native SInt16 with iSub, invert right channel - assumes 2 channels 
// ------------------------------------------------------------------------
IOReturn AppleDBDMAAudio::clipAppleDBDMAToOutputStream16iSubInvertRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
{
    UInt32 		sampleIndex, maxSampleIndex, numSamples;
    float *		floatMixBuf;
    SInt16 *	outputBuf16;

	iSubSynchronize(firstSampleFrame, numSampleFrames);

	PreviousValues* filterState = &(miSubProcessingParams.filterState);
	PreviousValues* filterState2 = &(miSubProcessingParams.filterState2);
	PreviousValues* phaseCompState = &(miSubProcessingParams.phaseCompState);
	UInt32* loopCount = &(miSubProcessingParams.iSubLoopCount);
	SInt32* iSubBufferOffset = &(miSubProcessingParams.iSubBufferOffset);
	float* srcPhase = &(miSubProcessingParams.srcPhase);
	float* srcState = &(miSubProcessingParams.srcState);

	float* low = miSubProcessingParams.lowFreqSamples;
	float* high = miSubProcessingParams.highFreqSamples;
	UInt32 sampleRate = miSubProcessingParams.sampleRate;
	UInt32 adaptiveSampleRate = miSubProcessingParams.adaptiveSampleRate;
	SInt16* iSubBufferMemory = miSubProcessingParams.iSubBuffer;
	UInt32 iSubBufferLen = miSubProcessingParams.iSubBufferLen;
	UInt32 outputSampleRate = miSubProcessingParams.iSubFormat.outputSampleRate;

    floatMixBuf = (float *)mixBuf;
	numSamples = numSampleFrames * streamFormat->fNumChannels;
    maxSampleIndex = (firstSampleFrame + numSampleFrames) * streamFormat->fNumChannels;

    // Filter audio into low and high buffers using a 24 dB/octave crossover
	StereoFilter4thOrderPhaseComp (&floatMixBuf[firstSampleFrame * streamFormat->fNumChannels], &low[firstSampleFrame * streamFormat->fNumChannels], &high[firstSampleFrame * streamFormat->fNumChannels], numSampleFrames, sampleRate, filterState, filterState2, phaseCompState);

    // high side 
	outputBuf16 = (SInt16 *)sampleBuf+firstSampleFrame * streamFormat->fNumChannels;
	invertRightChannel( &high[firstSampleFrame * streamFormat->fNumChannels], numSamples );
	Float32ToNativeInt16( &high[firstSampleFrame * streamFormat->fNumChannels], outputBuf16, numSamples );

    // low side
 	sampleIndex = (firstSampleFrame * streamFormat->fNumChannels);
	iSubDownSampleLinearAndConvert( low, srcPhase, srcState, adaptiveSampleRate, outputSampleRate, sampleIndex, maxSampleIndex, iSubBufferMemory, iSubBufferOffset, iSubBufferLen, loopCount );	
		
	updateiSubPosition(firstSampleFrame, numSampleFrames);

    return kIOReturnSuccess;
}

// ------------------------------------------------------------------------
// Float32 to Native SInt16 with iSub, mix and mute right channel
// ------------------------------------------------------------------------
IOReturn AppleDBDMAAudio::clipAppleDBDMAToOutputStream16iSubMixRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
{

    UInt32 		sampleIndex, maxSampleIndex, numSamples;
    float *		floatMixBuf;
    SInt16 *	outputBuf16;

	iSubSynchronize(firstSampleFrame, numSampleFrames);

	PreviousValues* filterState = &(miSubProcessingParams.filterState);
	PreviousValues* filterState2 = &(miSubProcessingParams.filterState2);
	PreviousValues* phaseCompState = &(miSubProcessingParams.phaseCompState);
	UInt32* loopCount = &(miSubProcessingParams.iSubLoopCount);
	SInt32* iSubBufferOffset = &(miSubProcessingParams.iSubBufferOffset);
	float* srcPhase = &(miSubProcessingParams.srcPhase);
	float* srcState = &(miSubProcessingParams.srcState);

	float* low = miSubProcessingParams.lowFreqSamples;
	float* high = miSubProcessingParams.highFreqSamples;
	UInt32 sampleRate = miSubProcessingParams.sampleRate;
	UInt32 adaptiveSampleRate = miSubProcessingParams.adaptiveSampleRate;
	SInt16* iSubBufferMemory = miSubProcessingParams.iSubBuffer;
	UInt32 iSubBufferLen = miSubProcessingParams.iSubBufferLen;
	UInt32 outputSampleRate = miSubProcessingParams.iSubFormat.outputSampleRate;

    floatMixBuf = (float *)mixBuf;
	numSamples = numSampleFrames * streamFormat->fNumChannels;
    maxSampleIndex = (firstSampleFrame + numSampleFrames) * streamFormat->fNumChannels;

    // Filter audio into low and high buffers using a 24 dB/octave crossover
	StereoFilter4thOrderPhaseComp (&floatMixBuf[firstSampleFrame * streamFormat->fNumChannels], &low[firstSampleFrame * streamFormat->fNumChannels], &high[firstSampleFrame * streamFormat->fNumChannels], numSampleFrames, sampleRate, filterState, filterState2, phaseCompState);

    // high side 
	outputBuf16 = (SInt16 *)sampleBuf+firstSampleFrame * streamFormat->fNumChannels;
	mixAndMuteRightChannel(  &high[firstSampleFrame * streamFormat->fNumChannels], numSamples );
	Float32ToNativeInt16( &high[firstSampleFrame * streamFormat->fNumChannels], outputBuf16, numSamples );

    // low side
 	sampleIndex = (firstSampleFrame * streamFormat->fNumChannels);
	iSubDownSampleLinearAndConvert( low, srcPhase, srcState, adaptiveSampleRate, outputSampleRate, sampleIndex, maxSampleIndex, iSubBufferMemory, iSubBufferOffset, iSubBufferLen, loopCount );	
		
	updateiSubPosition(firstSampleFrame, numSampleFrames);
		
	return kIOReturnSuccess;
}

// ------------------------------------------------------------------------
// Float32 to Native SInt32 with iSub, assumes 2 channel data
// ------------------------------------------------------------------------
IOReturn AppleDBDMAAudio::clipAppleDBDMAToOutputStream32iSub(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
{
    UInt32 		sampleIndex, maxSampleIndex, numSamples;
    float *		floatMixBuf;
    SInt32 *	outputBuf32;

	iSubSynchronize(firstSampleFrame, numSampleFrames);

	PreviousValues* filterState = &(miSubProcessingParams.filterState);
	PreviousValues* filterState2 = &(miSubProcessingParams.filterState2);
	PreviousValues* phaseCompState = &(miSubProcessingParams.phaseCompState);
	UInt32* loopCount = &(miSubProcessingParams.iSubLoopCount);
	SInt32* iSubBufferOffset = &(miSubProcessingParams.iSubBufferOffset);
	float* srcPhase = &(miSubProcessingParams.srcPhase);
	float* srcState = &(miSubProcessingParams.srcState);

	float* low = miSubProcessingParams.lowFreqSamples;
	float* high = miSubProcessingParams.highFreqSamples;
	UInt32 sampleRate = miSubProcessingParams.sampleRate;
	UInt32 adaptiveSampleRate = miSubProcessingParams.adaptiveSampleRate;
	SInt16* iSubBufferMemory = miSubProcessingParams.iSubBuffer;
	UInt32 iSubBufferLen = miSubProcessingParams.iSubBufferLen;
	UInt32 outputSampleRate = miSubProcessingParams.iSubFormat.outputSampleRate;

    floatMixBuf = (float *)mixBuf;
	numSamples = numSampleFrames * streamFormat->fNumChannels;
    maxSampleIndex = (firstSampleFrame + numSampleFrames) * streamFormat->fNumChannels;

    // Filter audio into low and high buffers using a 24 dB/octave crossover
	StereoFilter4thOrderPhaseComp (&floatMixBuf[firstSampleFrame * streamFormat->fNumChannels], &low[firstSampleFrame * streamFormat->fNumChannels], &high[firstSampleFrame * streamFormat->fNumChannels], numSampleFrames, sampleRate, filterState, filterState2, phaseCompState);

    // high side 
	outputBuf32 = (SInt32 *)sampleBuf + firstSampleFrame * streamFormat->fNumChannels;
	Float32ToNativeInt32( &high[firstSampleFrame * streamFormat->fNumChannels], outputBuf32, numSamples );

    // low side
  	sampleIndex = (firstSampleFrame * streamFormat->fNumChannels);
	iSubDownSampleLinearAndConvert( low, srcPhase, srcState, adaptiveSampleRate, outputSampleRate, sampleIndex, maxSampleIndex, iSubBufferMemory, iSubBufferOffset, iSubBufferLen, loopCount );	
		
	updateiSubPosition(firstSampleFrame, numSampleFrames);

	return kIOReturnSuccess;
}

// ------------------------------------------------------------------------
// Float32 to Native SInt32 with iSub, delay right channel one sample
// ------------------------------------------------------------------------
IOReturn AppleDBDMAAudio::clipAppleDBDMAToOutputStream32iSubDelayRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
{
    UInt32 		sampleIndex, maxSampleIndex, numSamples;
    float *		floatMixBuf;
    SInt32 *	outputBuf32;

	iSubSynchronize(firstSampleFrame, numSampleFrames);

	PreviousValues* filterState = &(miSubProcessingParams.filterState);
	PreviousValues* filterState2 = &(miSubProcessingParams.filterState2);
	PreviousValues* phaseCompState = &(miSubProcessingParams.phaseCompState);
	UInt32* loopCount = &(miSubProcessingParams.iSubLoopCount);
	SInt32* iSubBufferOffset = &(miSubProcessingParams.iSubBufferOffset);
	float* srcPhase = &(miSubProcessingParams.srcPhase);
	float* srcState = &(miSubProcessingParams.srcState);

	float* low = miSubProcessingParams.lowFreqSamples;
	float* high = miSubProcessingParams.highFreqSamples;
	UInt32 sampleRate = miSubProcessingParams.sampleRate;
	UInt32 adaptiveSampleRate = miSubProcessingParams.adaptiveSampleRate;
	SInt16* iSubBufferMemory = miSubProcessingParams.iSubBuffer;
	UInt32 iSubBufferLen = miSubProcessingParams.iSubBufferLen;
	UInt32 outputSampleRate = miSubProcessingParams.iSubFormat.outputSampleRate;

    floatMixBuf = (float *)mixBuf;
	numSamples = numSampleFrames * streamFormat->fNumChannels;
    maxSampleIndex = (firstSampleFrame + numSampleFrames) * streamFormat->fNumChannels;

    // Filter audio into low and high buffers using a 24 dB/octave crossover
	StereoFilter4thOrderPhaseComp (&floatMixBuf[firstSampleFrame * streamFormat->fNumChannels], &low[firstSampleFrame * streamFormat->fNumChannels], &high[firstSampleFrame * streamFormat->fNumChannels], numSampleFrames, sampleRate, filterState, filterState2, phaseCompState);

    // high side 
	outputBuf32 = (SInt32 *)sampleBuf + firstSampleFrame * streamFormat->fNumChannels;
	delayRightChannel( &high[firstSampleFrame * streamFormat->fNumChannels], numSamples );
	Float32ToNativeInt32( &high[firstSampleFrame * streamFormat->fNumChannels], outputBuf32, numSamples );

    // low side
 	sampleIndex = (firstSampleFrame * streamFormat->fNumChannels);
	iSubDownSampleLinearAndConvert( low, srcPhase, srcState, adaptiveSampleRate, outputSampleRate, sampleIndex, maxSampleIndex, iSubBufferMemory, iSubBufferOffset, iSubBufferLen, loopCount );	

	updateiSubPosition(firstSampleFrame, numSampleFrames);
		
	return kIOReturnSuccess;
}

// ------------------------------------------------------------------------
// Float32 to Native SInt32 with iSub, mix and mute right channel
// ------------------------------------------------------------------------
IOReturn AppleDBDMAAudio::clipAppleDBDMAToOutputStream32iSubMixRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
{
    UInt32 		sampleIndex, maxSampleIndex, numSamples;
    float *		floatMixBuf;
    SInt32 *	outputBuf32;

	iSubSynchronize(firstSampleFrame, numSampleFrames);

	PreviousValues* filterState = &(miSubProcessingParams.filterState);
	PreviousValues* filterState2 = &(miSubProcessingParams.filterState2);
	PreviousValues* phaseCompState = &(miSubProcessingParams.phaseCompState);
	UInt32* loopCount = &(miSubProcessingParams.iSubLoopCount);
	SInt32* iSubBufferOffset = &(miSubProcessingParams.iSubBufferOffset);
	float* srcPhase = &(miSubProcessingParams.srcPhase);
	float* srcState = &(miSubProcessingParams.srcState);

	float* low = miSubProcessingParams.lowFreqSamples;
	float* high = miSubProcessingParams.highFreqSamples;
	UInt32 sampleRate = miSubProcessingParams.sampleRate;
	UInt32 adaptiveSampleRate = miSubProcessingParams.adaptiveSampleRate;
	SInt16* iSubBufferMemory = miSubProcessingParams.iSubBuffer;
	UInt32 iSubBufferLen = miSubProcessingParams.iSubBufferLen;
	UInt32 outputSampleRate = miSubProcessingParams.iSubFormat.outputSampleRate;

    floatMixBuf = (float *)mixBuf;
	numSamples = numSampleFrames * streamFormat->fNumChannels;
    maxSampleIndex = (firstSampleFrame + numSampleFrames) * streamFormat->fNumChannels;

    // Filter audio into low and high buffers using a 24 dB/octave crossover
	StereoFilter4thOrderPhaseComp (&floatMixBuf[firstSampleFrame * streamFormat->fNumChannels], &low[firstSampleFrame * streamFormat->fNumChannels], &high[firstSampleFrame * streamFormat->fNumChannels], numSampleFrames, sampleRate, filterState, filterState2, phaseCompState);

    // high side 
	outputBuf32 = (SInt32 *)sampleBuf + firstSampleFrame * streamFormat->fNumChannels;
	mixAndMuteRightChannel( &high[firstSampleFrame * streamFormat->fNumChannels], numSamples );
	Float32ToNativeInt32( &high[firstSampleFrame * streamFormat->fNumChannels], outputBuf32, numSamples );

    // low side
 	sampleIndex = (firstSampleFrame * streamFormat->fNumChannels);
	iSubDownSampleLinearAndConvert( low, srcPhase, srcState, adaptiveSampleRate, outputSampleRate, sampleIndex, maxSampleIndex, iSubBufferMemory, iSubBufferOffset, iSubBufferLen, loopCount );	
		
	updateiSubPosition(firstSampleFrame, numSampleFrames);
		
	return kIOReturnSuccess;
}

#pragma mark ------------------------ 
#pragma mark еее Input Routines
#pragma mark ------------------------ 

// ------------------------------------------------------------------------
// Native SInt16 to Float32
// ------------------------------------------------------------------------
IOReturn AppleDBDMAAudio::convertAppleDBDMAFromInputStream16(const void *sampleBuf, void *destBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
{
    UInt32 numSamplesLeft;
    float *floatDestBuf;
    SInt16 *inputBuf16;
	
    floatDestBuf = (float *)destBuf;
    numSamplesLeft = numSampleFrames * streamFormat->fNumChannels;
	inputBuf16 = &(((SInt16 *)sampleBuf)[firstSampleFrame * streamFormat->fNumChannels]);

   	NativeInt16ToFloat32(inputBuf16, floatDestBuf, numSamplesLeft, 16);

    return kIOReturnSuccess;
}

// ------------------------------------------------------------------------
// Native SInt16 to Float32, copy the rigth sample to the left channel for
// older machines only.  Note that there is no 32 bit version of this  
// function because older hardware does not support it.
// ------------------------------------------------------------------------
IOReturn AppleDBDMAAudio::convertAppleDBDMAFromInputStream16CopyR2L(const void *sampleBuf, void *destBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
{
    UInt32 numSamplesLeft;
    float *floatDestBuf;
    SInt16 *inputBuf16;
    
    floatDestBuf = (float *)destBuf;    
    numSamplesLeft = numSampleFrames * streamFormat->fNumChannels;
 
	inputBuf16 = &(((SInt16 *)sampleBuf)[firstSampleFrame * streamFormat->fNumChannels]);
   
	NativeInt16ToFloat32CopyRightToLeft(inputBuf16, floatDestBuf, numSamplesLeft, 16);

    return kIOReturnSuccess;
}

// ------------------------------------------------------------------------
// Native SInt16 to Float32, with software input gain
// ------------------------------------------------------------------------
IOReturn AppleDBDMAAudio::convertAppleDBDMAFromInputStream16WithGain(const void *sampleBuf, void *destBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
{
    UInt32 numSamplesLeft;
    float *floatDestBuf;
    SInt16 *inputBuf16;
    
    floatDestBuf = (float *)destBuf;    
    numSamplesLeft = numSampleFrames * streamFormat->fNumChannels;
	inputBuf16 = &(((SInt16 *)sampleBuf)[firstSampleFrame * streamFormat->fNumChannels]);

	NativeInt16ToFloat32Gain(inputBuf16, floatDestBuf, numSamplesLeft, 16, mInputGainLPtr, mInputGainRPtr);

    return kIOReturnSuccess;
}

// ------------------------------------------------------------------------
// Native SInt32 to Float32
// ------------------------------------------------------------------------
IOReturn AppleDBDMAAudio::convertAppleDBDMAFromInputStream32(const void *sampleBuf, void *destBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
{
    UInt32 numSamplesLeft;
    float *floatDestBuf;
    SInt32 *inputBuf32;

    floatDestBuf = (float *)destBuf;
    numSamplesLeft = numSampleFrames * streamFormat->fNumChannels;
	inputBuf32 = &(((SInt32 *)sampleBuf)[firstSampleFrame * streamFormat->fNumChannels]);
   
	NativeInt32ToFloat32(inputBuf32, floatDestBuf, numSamplesLeft, 32);

    return kIOReturnSuccess;
}

// ------------------------------------------------------------------------
// Native SInt32 to Float32, with software input gain
// ------------------------------------------------------------------------
IOReturn AppleDBDMAAudio::convertAppleDBDMAFromInputStream32WithGain(const void *sampleBuf, void *destBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
{
    UInt32 numSamplesLeft;
    float *floatDestBuf;
    SInt32 *inputBuf32;
  
    floatDestBuf = (float *)destBuf;    
    numSamplesLeft = numSampleFrames * streamFormat->fNumChannels;
	inputBuf32 = &(((SInt32 *)sampleBuf)[firstSampleFrame * streamFormat->fNumChannels]);

	NativeInt32ToFloat32Gain(inputBuf32, floatDestBuf, numSamplesLeft, 32, mInputGainLPtr, mInputGainRPtr);

    return kIOReturnSuccess;
}

#pragma mark ------------------------ 
#pragma mark еее State Routines
#pragma mark ------------------------ 

void AppleDBDMAAudio::resetOutputClipOptions() {
	fNeedsPhaseInversion = false;
	fNeedsRightChanDelay = false;
	fNeedsRightChanMixed = false;
}

void AppleDBDMAAudio::resetInputClipOptions() {
	mInputDualMonoMode = e_Mode_Disabled;
}

void AppleDBDMAAudio::setEqualizationFromDictionary (OSDictionary * inDictionary) 
{ 
	OSDictionary *					theFilterDict;
	OSCollectionIterator *			collectionIterator;
	OSNumber *						typeNumber;
	OSSymbol *						theSymbol;
	OSData *						parameterData;
	EQParamStruct 					eqParams;
	UInt32							typeCode;
	UInt32							index;
	
	debug2IOLog ("AppleDBDMAAudio::setEqualizationFromDictionary (%p)\n", inDictionary);

	collectionIterator = OSCollectionIterator::withCollection (inDictionary);
	FailIf (NULL == collectionIterator, Exit);
	
	clearSoftwareEQ ();
	
	eqParams.type = (FilterType)0;
	eqParams.fc = 0;
	eqParams.Q = 0;
	eqParams.gain = 0;

	mEQStruct.bypassAll = true;
	
	index = 0;

	while (theSymbol = OSDynamicCast (OSSymbol, collectionIterator->getNextObject ())) {
		debug2IOLog ("symbol = %s\n", theSymbol->getCStringNoCopy ());
		theFilterDict = OSDynamicCast (OSDictionary, inDictionary->getObject (theSymbol));
		debug2IOLog ("theFilterDict = %p\n", theFilterDict);

		if (NULL != theFilterDict) {
			typeNumber = OSDynamicCast (OSNumber, theFilterDict->getObject (kFilterType));
			debug2IOLog ("typeNumber = %p\n", typeNumber);
			FailIf (NULL == typeNumber, Exit);
			typeCode = typeNumber->unsigned32BitValue ();
			debug2IOLog ("filter type = %4s\n", (char *)&typeCode);
			eqParams.type = (FilterType)typeCode;

			parameterData = OSDynamicCast (OSData, theFilterDict->getObject (kFilterFrequency));
			FailIf (NULL == parameterData, Exit);
			debug2IOLog ("parameterData = %lx\n", *(UInt32 *)(parameterData->getBytesNoCopy ()));
			memcpy (&(eqParams.fc), parameterData->getBytesNoCopy (), 4);

			parameterData = OSDynamicCast (OSData, theFilterDict->getObject (kFilterQ));
			FailIf (NULL == parameterData, Exit);
			debug2IOLog ("parameterData = %lx\n", *(UInt32 *)(parameterData->getBytesNoCopy ()));
			memcpy (&(eqParams.Q), parameterData->getBytesNoCopy (), 4);

			parameterData = OSDynamicCast (OSData, theFilterDict->getObject (kFilterGain));
			if (NULL != parameterData) {
				FailIf (NULL == parameterData, Exit);
				debug2IOLog ("parameterData = %lx\n", *(UInt32 *)(parameterData->getBytesNoCopy ()));
				memcpy (&(eqParams.gain), parameterData->getBytesNoCopy (), 4);
			}
			setEQCoefficients (&eqParams, &mEQStruct, index, sampleRate.whole);

			debug2IOLog ("mEQStruct.b0[0] = 0x%lx\n", *((UInt32 *)&(mEQStruct.b0[index])));
			debug2IOLog ("mEQStruct.b1[0] = 0x%lx\n", *((UInt32 *)&(mEQStruct.b1[index])));
			debug2IOLog ("mEQStruct.b2[0] = 0x%lx\n", *((UInt32 *)&(mEQStruct.b2[index])));
			debug2IOLog ("mEQStruct.a1[0] = 0x%lx\n", *((UInt32 *)&(mEQStruct.a1[index])));
			debug2IOLog ("mEQStruct.a2[0] = 0x%lx\n", *((UInt32 *)&(mEQStruct.a2[index])));
			
			index++;
		}
	}

	mEQStruct.numFilters = index;
	debug2IOLog ("mEQStruct.numFilters = %ld\n", mEQStruct.numFilters);
	
	mEQStruct.bypassAll = false;
	
	collectionIterator->release ();

Exit:
	return;   	
}

void AppleDBDMAAudio::setLimiterFromDictionary (OSDictionary * inDictionary) 
{ 
	OSDictionary *					theLimiterDict;
	OSCollectionIterator *			collectionIterator;
	OSNumber *						typeNumber;
	OSNumber *						bandIndexNumber;
	OSSymbol *						theSymbol;
	OSData *						parameterData;
	LimiterParamStruct 				limiterParams;
	UInt32							typeCode;
	UInt32							bandIndex;
	UInt32							count;
	
	debug2IOLog ("AppleDBDMAAudio::setLimiterFromDictionary (%p)\n", inDictionary);

	collectionIterator = OSCollectionIterator::withCollection (inDictionary);
	FailIf (NULL == collectionIterator, Exit);
	
	clearSoftwareLimiter ();
	
	limiterParams.type = (LimiterType)0;
	limiterParams.threshold = 0;
	limiterParams.ratio = 0;
	limiterParams.attack = 0;
	limiterParams.release = 0;
	limiterParams.lookahead = 0;
	bandIndex = 0;
	
	mLimiterStruct.bypassAll = true;
	
	count = 0;

	while (theSymbol = OSDynamicCast (OSSymbol, collectionIterator->getNextObject ())) {
		debug2IOLog ("symbol = %s\n", theSymbol->getCStringNoCopy ());
		if (theSymbol->isEqualTo (kCrossover))
			continue;
		theLimiterDict = OSDynamicCast (OSDictionary, inDictionary->getObject (theSymbol));
		debug2IOLog ("theLimiterDict = %p\n", theLimiterDict);

		if (NULL != theLimiterDict) {
			typeNumber = OSDynamicCast (OSNumber, theLimiterDict->getObject (kLimiterType));
			debug2IOLog ("typeNumber = %p\n", typeNumber);
			FailIf (NULL == typeNumber, Exit);
			typeCode = typeNumber->unsigned32BitValue ();
			debug2IOLog ("limiter type = %4s\n", (char *)&typeCode);
			limiterParams.type = (LimiterType)typeCode;

			bandIndexNumber = OSDynamicCast (OSNumber, theLimiterDict->getObject (kLimiterBandIndex));
			debug2IOLog ("bandIndexNumber = %p\n", bandIndexNumber);
			FailIf (NULL == bandIndexNumber, Exit);
			bandIndex = bandIndexNumber->unsigned32BitValue ();
			debug2IOLog ("band index = %ld\n", bandIndex);
			if (bandIndex > kMaxNumLimiters)
				break;

			parameterData = OSDynamicCast (OSData, theLimiterDict->getObject (kLimiterAttackTime));
			FailIf (NULL == parameterData, Exit);
			debug2IOLog ("parameterData = %lx\n", *(UInt32 *)(parameterData->getBytesNoCopy ()));
			memcpy (&(limiterParams.attack), parameterData->getBytesNoCopy (), 4);

			parameterData = OSDynamicCast (OSData, theLimiterDict->getObject (kLimiterReleaseTime));
			FailIf (NULL == parameterData, Exit);
			debug2IOLog ("parameterData = %lx\n", *(UInt32 *)(parameterData->getBytesNoCopy ()));
			memcpy (&(limiterParams.release), parameterData->getBytesNoCopy (), 4);

			parameterData = OSDynamicCast (OSData, theLimiterDict->getObject (kLimiterThreshold));
			FailIf (NULL == parameterData, Exit);
			debug2IOLog ("parameterData = %lx\n", *(UInt32 *)(parameterData->getBytesNoCopy ()));
			memcpy (&(limiterParams.threshold), parameterData->getBytesNoCopy (), 4);

			parameterData = OSDynamicCast (OSData, theLimiterDict->getObject (kLimiterRatio));
			FailIf (NULL == parameterData, Exit);
			debug2IOLog ("parameterData = %lx\n", *(UInt32 *)(parameterData->getBytesNoCopy ()));
			memcpy (&(limiterParams.ratio), parameterData->getBytesNoCopy (), 4);

			parameterData = OSDynamicCast (OSData, theLimiterDict->getObject (kLimiterLookahead));
			FailIf (NULL == parameterData, Exit);
			debug2IOLog ("parameterData = %lx\n", *(UInt32 *)(parameterData->getBytesNoCopy ()));
			memcpy (&(limiterParams.lookahead), parameterData->getBytesNoCopy (), 4);

			setLimiterCoefficients (&limiterParams, &mLimiterStruct, bandIndex, sampleRate.whole);

			debug3IOLog ("mLimiterStruct.threshold[%ld] = 0x%lx\n", bandIndex, *((UInt32 *)&(mLimiterStruct.threshold[bandIndex])));
			debug3IOLog ("mLimiterStruct.oneMinusOneOverRatio[%ld] = 0x%lx\n", count, *((UInt32 *)&(mLimiterStruct.oneMinusOneOverRatio[bandIndex])));
			debug3IOLog ("mLimiterStruct.attackTc[%ld] = 0x%lx\n", bandIndex, *((UInt32 *)&(mLimiterStruct.attackTc[bandIndex])));
			debug3IOLog ("mLimiterStruct.releaseTc[%ld] = 0x%lx\n", bandIndex, *((UInt32 *)&(mLimiterStruct.releaseTc[bandIndex])));
			debug3IOLog ("mLimiterStruct.lookahead[%ld] = 0x%lx\n", bandIndex, *((UInt32 *)&(mLimiterStruct.lookahead[bandIndex])));
			debug3IOLog ("mLimiterStruct.type[%ld] = 0x%x\n", bandIndex, mLimiterStruct.type[bandIndex]);
			
			count++;

		}
	}

	mLimiterStruct.numLimiters = count;
	debug2IOLog ("mLimiterStruct.numLimiters = %ld\n", mLimiterStruct.numLimiters);

	mLimiterStruct.bypassAll = false;

	collectionIterator->release ();

Exit:
	return;   	
}

void AppleDBDMAAudio::setCrossoverFromDictionary (OSDictionary * inDictionary) 
{ 
	OSNumber *							numBandsNumber;
	OSData *							parameterData;
	CrossoverParamStruct 				crossoverParams;
	UInt32								index;
	UInt32								numBands;
	char								frequencyCString[32];

	debug2IOLog ("AppleDBDMAAudio::setCrossoverFromDictionary (%p)\n", inDictionary);

	clearSoftwareCrossover ();
	
	crossoverParams.numBands = 0;
	crossoverParams.frequency[0] = 0;
	crossoverParams.frequency[1] = 0;
	
	if (NULL != inDictionary) {
		numBandsNumber = OSDynamicCast (OSNumber, inDictionary->getObject (kCrossoverNumberOfBands));
		debug2IOLog ("numBandsNumber = %p\n", numBandsNumber);
		FailIf (NULL == numBandsNumber, Exit);
		numBands = numBandsNumber->unsigned32BitValue ();
		debug2IOLog ("number of bands = 0x%lx\n", numBands);
		FailIf (kMaxNumCrossoverBands > numBands, Exit);
		crossoverParams.numBands = numBands;

		for (index = 0; index < (crossoverParams.numBands - 1); index++) {
			sprintf (frequencyCString, "%s%ld", kCrossoverFrequency, index); 
			debug2IOLog("frequency string = %s\n", frequencyCString);
			parameterData = OSDynamicCast (OSData, inDictionary->getObject (frequencyCString));
			FailIf (NULL == parameterData, Exit);
			debug2IOLog ("parameterData = %lx\n", *(UInt32 *)(parameterData->getBytesNoCopy ()));
			memcpy (&(crossoverParams.frequency[index]), parameterData->getBytesNoCopy (), 4);
		}

		setCrossoverCoefficients (&crossoverParams, &mCrossoverStruct, sampleRate.whole);

		debug2IOLog ("mCrossoverStruct.numBands = 0x%lx\n", mCrossoverStruct.numBands);
		debug2IOLog ("mCrossoverStruct.c1_1st[0] = 0x%lx\n", *((UInt32 *)&(mCrossoverStruct.c1_1st[0])));
		debug2IOLog ("mCrossoverStruct.c1_2nd[0] = 0x%lx\n", *((UInt32 *)&(mCrossoverStruct.c1_2nd[0])));
		debug2IOLog ("mCrossoverStruct.c2_2nd[0] = 0x%lx\n", *((UInt32 *)&(mCrossoverStruct.c2_2nd[0])));
		debug2IOLog ("mCrossoverStruct.outBufferPtr[0] = %p\n", mCrossoverStruct.outBufferPtr[0]);
		debug2IOLog ("mCrossoverStruct.outBufferPtr[1] = %p\n", mCrossoverStruct.outBufferPtr[1]);
			
	}
	
	if (mLimiterStruct.numLimiters != crossoverParams.numBands) {
		IOLog ("AppleDBDMAAudio: bad info plist - mismatched number of limiters (%ld) for specified crossover (%ld bands).\n", mLimiterStruct.numLimiters, crossoverParams.numBands);
		crossoverParams.numBands = 0;
	}

Exit:
	return;   	
}

void AppleDBDMAAudio::clearSoftwareEQ (void) 
{ 
	debugIOLog ("AppleDBDMAAudio::clearSoftwareEQ\n");

	bzero (&mEQStruct, sizeof (EQStruct));

	return;   	
}

void AppleDBDMAAudio::clearSoftwareLimiter (void) 
{ 
	debugIOLog ("AppleDBDMAAudio::clearSoftwareLimiter\n");

	bzero (&mLimiterStruct, sizeof (mLimiterStruct));

	return;   	
}

void AppleDBDMAAudio::clearSoftwareCrossover (void) 
{ 
	debugIOLog ("AppleDBDMAAudio::clearSoftwareCrossover\n");

	initializeCrossover (&mCrossoverStruct);

	return;   	
}

void AppleDBDMAAudio::disableSoftwareEQ (void) 
{ 
	debugIOLog ("AppleDBDMAAudio::disableSoftwareEQ\n");

	mEQStruct.bypassAll = true;
	
	return;   	
}

void AppleDBDMAAudio::disableSoftwareCrossover() 
{ 
	debugIOLog ("AppleDBDMAAudio::disableSoftwareCrossover\n");

	mCrossoverStruct.numBands = 0;

	return;   	
}

void AppleDBDMAAudio::disableSoftwareLimiter() 
{ 
	debugIOLog ("AppleDBDMAAudio::disableSoftwareLimiter\n");

	mLimiterStruct.bypassAll = true;

	return;   	
}
	
void AppleDBDMAAudio::setDualMonoMode(const DualMonoModeType inDualMonoMode) 
{ 
	mInputDualMonoMode = inDualMonoMode; 
	chooseInputConversionRoutinePtr();

	return;   	
}

void AppleDBDMAAudio::setInputGainL(UInt32 inGainL) 
{ 
    if (mInputGainLPtr == NULL) {        
        mInputGainLPtr = (float *)IOMalloc(sizeof(float));
    }
    inputGainConverter(inGainL, mInputGainLPtr);
	
    return;   
} 

void AppleDBDMAAudio::setInputGainR(UInt32 inGainR) 
{ 
    if (mInputGainRPtr == NULL) {        
        mInputGainRPtr = (float *)IOMalloc(sizeof(float));
    }
    inputGainConverter(inGainR, mInputGainRPtr);

    return;   
} 

// [3094574] aml, updated routines below to set the proper clipping routine

void AppleDBDMAAudio::setPhaseInversion(const bool needsPhaseInversion) 
{
	fNeedsPhaseInversion = needsPhaseInversion; 
	chooseOutputClippingRoutinePtr();
	
	return;   
}

void AppleDBDMAAudio::setRightChanDelay(const bool needsRightChanDelay)  
{
	fNeedsRightChanDelay = needsRightChanDelay;  
	chooseOutputClippingRoutinePtr();
	
	return;   
}

void AppleDBDMAAudio::setRightChanMixed(const bool needsRightChanMixed)  
{
	fNeedsRightChanMixed = needsRightChanMixed;  
	chooseOutputClippingRoutinePtr();
	
	return;   
}

void AppleDBDMAAudio::setUseSoftwareInputGain(const bool inUseSoftwareInputGain) 
{     
	debug2IOLog ("setUseSoftwareInputGain (%s)\n", inUseSoftwareInputGain ? "true" : "false");
	
	mUseSoftwareInputGain = inUseSoftwareInputGain;     	
	chooseInputConversionRoutinePtr();
	
	return;   
}

#pragma mark ------------------------ 
#pragma mark еее Format Routines
#pragma mark ------------------------ 

bool AppleDBDMAAudio::getDmaState (void )
{
	return dmaRunState;
}

IOReturn AppleDBDMAAudio::performFormatChange(IOAudioStream *audioStream, const IOAudioStreamFormat *newFormat, const IOAudioSampleRate *newSampleRate)
{
	IOReturn					result;
	
	result = kIOReturnError;
	
	debug4IOLog ("AppleDBDMAAudio::performFormatChange (%p, %p, %p)\n", audioStream, newFormat, newSampleRate);	
	
	if ( NULL != newFormat ) {	
		mDBDMAFormat.fNumChannels = newFormat->fNumChannels;
		mDBDMAFormat.fSampleFormat = newFormat->fSampleFormat;
		mDBDMAFormat.fNumericRepresentation = newFormat->fNumericRepresentation;
		mDBDMAFormat.fBitDepth = newFormat->fBitDepth;
		mDBDMAFormat.fAlignment = newFormat->fAlignment;
		mDBDMAFormat.fByteOrder = newFormat->fByteOrder;
		mDBDMAFormat.fIsMixable = newFormat->fIsMixable;
		mDBDMAFormat.fDriverTag = newFormat->fDriverTag;

		if (mDBDMAFormat.fBitWidth != newFormat->fBitWidth) {
			blockSize = ( DBDMAAUDIODMAENGINE_ROOT_BLOCK_SIZE * ( newFormat->fBitWidth / 8 ) );
			
			mDBDMAFormat.fBitWidth = newFormat->fBitWidth;
			deallocateDMAMemory ();
			allocateDMABuffers ();

			FailIf (NULL == mOutputSampleBuffer, Exit);
			if (ioBaseDMAInput) 
				FailIf (NULL == mInputSampleBuffer, Exit);

			mOutputStream->setSampleBuffer ((void *)mOutputSampleBuffer, numBlocks * blockSize);
			if (mInputStream) {
				mInputStream->setSampleBuffer ((void *)mInputSampleBuffer, numBlocks * blockSize);
			}
			
			if (audioStream != mInputStream && NULL != mInputStream) {
				mInputStream->hardwareFormatChanged (&mDBDMAFormat);
			} else if (audioStream != mOutputStream && NULL != mOutputStream) {
				mOutputStream->hardwareFormatChanged (&mDBDMAFormat);
			}
			
			FailIf (FALSE == createDMAPrograms (NULL != ioBaseDMAInput), Exit);
		}
	}

// FIX - move to inside ourProvider->formatChangeRequest
/*	if (NULL != newSampleRate) {
		setEQCoefficients (&mEQStruct, newSampleRate->whole);

		IOLog ("mEQStruct.b0[0] = 0x%lx\n", *((UInt32 *)&(mEQStruct.b0[0])));
		IOLog ("mEQStruct.b1[0] = 0x%lx\n", *((UInt32 *)&(mEQStruct.b1[0])));
		IOLog ("mEQStruct.b2[0] = 0x%lx\n", *((UInt32 *)&(mEQStruct.b2[0])));
		IOLog ("mEQStruct.a1[0] = 0x%lx\n", *((UInt32 *)&(mEQStruct.a1[0])));
		IOLog ("mEQStruct.a2[0] = 0x%lx\n", *((UInt32 *)&(mEQStruct.a2[0])));
	}
*/	
	result = ourProvider->formatChangeRequest (newFormat, newSampleRate);

	// in and out have the same format always.
	chooseOutputClippingRoutinePtr();
	chooseInputConversionRoutinePtr();

Exit:
    return result;
}

#pragma mark ------------------------ 
#pragma mark еее iSub Support
#pragma mark ------------------------ 

IOReturn AppleDBDMAAudio::iSubAttachChangeHandler (IOService *target, IOAudioControl *attachControl, SInt32 oldValue, SInt32 newValue) {
    IOReturn						result;
    AppleDBDMAAudio *		audioDMAEngine;
    IOCommandGate *					cg;

	debug5IOLog ("+ AppleDBDMAAudio::iSubAttachChangeHandler (%p, %p, 0x%lx, 0x%lx)\n", target, attachControl, oldValue, newValue);

	result = kIOReturnSuccess;
	if (oldValue != newValue) {
		audioDMAEngine = OSDynamicCast (AppleDBDMAAudio, target);
		FailIf (NULL == audioDMAEngine, Exit);
	
		if (newValue) {
			debugIOLog ("will try to connect to an iSub, installing notifier.\n");
			// Set up notifier to run when iSub shows up
			audioDMAEngine->iSubEngineNotifier = addNotification (gIOPublishNotification, serviceMatching ("AppleiSubEngine"), (IOServiceNotificationHandler)&iSubEnginePublished, audioDMAEngine);
			if (NULL != audioDMAEngine->iSubBufferMemory) {
				// it looks like the notifier could be called before iSubEngineNotifier is set, 
				// so if it was called, then iSubBufferMemory would no longer be NULL and we can remove the notifier
				debugIOLog ("iSub was already attached\n");
				audioDMAEngine->iSubEngineNotifier->remove ();
				audioDMAEngine->iSubEngineNotifier = NULL;
			}
		} else {
			debugIOLog ("do not try to connect to iSub, removing notifier.\n");
			if (NULL != audioDMAEngine->iSubBufferMemory) {
				debugIOLog ("disconnect from iSub\n");
				// We're already attached to an iSub, so detach
				cg = audioDMAEngine->getCommandGate ();
				if (NULL != cg) {
					cg->runAction (iSubCloseAction);
				}
			}
	
			// We're not attached to the iSub, so just remove our notifier
			if (NULL != audioDMAEngine->iSubEngineNotifier) {
				debugIOLog ("remove iSub notifier\n");
				audioDMAEngine->iSubEngineNotifier->remove ();
				audioDMAEngine->iSubEngineNotifier = NULL;
			}
		}
	}
	
Exit:
    debugIOLog ("- AppleDBDMAAudio::iSubAttachChangeHandler\n");
    return result;
}

bool AppleDBDMAAudio::iSubEnginePublished (AppleDBDMAAudio * dbdmaEngineObject, void * refCon, IOService * newService) {
	IOReturn						result;
	bool							resultCode;
    IOCommandGate *					cg;

	debug4IOLog ("+AppleDBDMAAudio::iSubEnginePublished (%p, %p, %p)\n", dbdmaEngineObject, (UInt32*)refCon, newService);

	resultCode = false;

	FailIf (NULL == dbdmaEngineObject, Exit);
	FailIf (NULL == newService, Exit);

	dbdmaEngineObject->iSubEngine = (AppleiSubEngine *)newService;
	FailIf (NULL == dbdmaEngineObject->iSubEngine, Exit);

	// Create the memory for the high/low samples to go into
    dbdmaEngineObject->miSubProcessingParams.lowFreqSamples = (float *)IOMallocAligned (round_page((dbdmaEngineObject->numBlocks * dbdmaEngineObject->blockSize) * sizeof (float)), PAGE_SIZE);
	FailIf (NULL == dbdmaEngineObject->miSubProcessingParams.lowFreqSamples, Exit);
    dbdmaEngineObject->miSubProcessingParams.highFreqSamples = (float *)IOMallocAligned (round_page((dbdmaEngineObject->numBlocks * dbdmaEngineObject->blockSize) * sizeof (float)), PAGE_SIZE);
	FailIf (NULL == dbdmaEngineObject->miSubProcessingParams.highFreqSamples, Exit);

	// Open the iSub which will cause it to create mute and volume controls
	dbdmaEngineObject->attach (dbdmaEngineObject->iSubEngine);
	cg = dbdmaEngineObject->getCommandGate ();
	FailWithAction (NULL == cg, dbdmaEngineObject->detach (dbdmaEngineObject->iSubEngine), Exit);
	dbdmaEngineObject->setSampleOffset(kMinimumLatencyiSub);	// HAL should notice this when iSub adds it's controls and sends out update
	IOSleep (102);
	result = cg->runAction (iSubOpenAction);
	FailWithAction (kIOReturnSuccess != result, dbdmaEngineObject->detach (dbdmaEngineObject->iSubEngine), Exit);
	dbdmaEngineObject->iSubBufferMemory = dbdmaEngineObject->iSubEngine->GetSampleBuffer ();
	debug2IOLog ("iSubBuffer length = %ld\n", dbdmaEngineObject->iSubBufferMemory->getLength ());

	// remove our notifier because we only care about the first iSub
	if (NULL != dbdmaEngineObject->iSubEngineNotifier) {
		dbdmaEngineObject->iSubEngineNotifier->remove ();
		dbdmaEngineObject->iSubEngineNotifier = NULL;
	}

	resultCode = true;
	dbdmaEngineObject->iSubOpen = TRUE;

Exit:
	if (FALSE == resultCode) {
		// We didn't actually open the iSub
		dbdmaEngineObject->iSubBufferMemory = NULL;
		dbdmaEngineObject->iSubEngine = NULL;
		dbdmaEngineObject->iSubOpen = FALSE;
		dbdmaEngineObject->setSampleOffset(kMinimumLatency);

		if (NULL != dbdmaEngineObject->miSubProcessingParams.lowFreqSamples) {
			IOFree (dbdmaEngineObject->miSubProcessingParams.lowFreqSamples, (dbdmaEngineObject->numBlocks * dbdmaEngineObject->blockSize) * sizeof (float));
		}

		if (NULL != dbdmaEngineObject->miSubProcessingParams.highFreqSamples) {
			IOFree (dbdmaEngineObject->miSubProcessingParams.highFreqSamples, (dbdmaEngineObject->numBlocks * dbdmaEngineObject->blockSize) * sizeof (float));
		}

	} else {
		// [3094574] aml - iSub opened sucessfully, update the clipping routine
		dbdmaEngineObject->chooseOutputClippingRoutinePtr();
	}
	
	debug5IOLog ("-AppleDBDMAAudio::iSubEnginePublished (%p, %p, %p), result = %d\n", dbdmaEngineObject, (UInt32 *)refCon, newService, resultCode);
	return resultCode;
}

IOReturn AppleDBDMAAudio::iSubCloseAction (OSObject *owner, void *arg1, void *arg2, void *arg3, void *arg4) {
    if (NULL != owner) {
        AppleDBDMAAudio *		audioEngine;

		debugIOLog ("+AppleDBDMAAudio::iSubCloseAction\n");

		audioEngine = OSDynamicCast (AppleDBDMAAudio, owner);

        if (NULL != audioEngine && NULL != audioEngine->iSubEngine && TRUE == audioEngine->iSubOpen) {
			AppleiSubEngine *				oldiSubEngine;
			
			oldiSubEngine = audioEngine->iSubEngine;
			
			audioEngine->pauseAudioEngine ();
			audioEngine->beginConfigurationChange ();

			audioEngine->iSubEngine->closeiSub (audioEngine);

			audioEngine->iSubEngine = NULL;
			audioEngine->iSubBufferMemory = NULL;

			// [3094574] aml - iSub is gone, update the clipping routine while the engine is paused
			audioEngine->chooseOutputClippingRoutinePtr();

			audioEngine->completeConfigurationChange ();
			audioEngine->resumeAudioEngine ();

			audioEngine->detach (oldiSubEngine); //(audioEngine->iSubEngine);

			//audioEngine->iSubEngine = NULL;
			//audioEngine->iSubBufferMemory = NULL;

			if (NULL != audioEngine->miSubProcessingParams.lowFreqSamples) {
				IOFree (audioEngine->miSubProcessingParams.lowFreqSamples, (audioEngine->numBlocks * audioEngine->blockSize) * sizeof (float));
				audioEngine->miSubProcessingParams.lowFreqSamples = NULL;
			}

			if (NULL != audioEngine->miSubProcessingParams.highFreqSamples) {
				IOFree (audioEngine->miSubProcessingParams.highFreqSamples, (audioEngine->numBlocks * audioEngine->blockSize) * sizeof (float));
				audioEngine->miSubProcessingParams.highFreqSamples = NULL;
			}

#if DEBUGLOG
			IOLog ("iSub connections terminated\n");
#endif
        } else {
#if DEBUGLOG
			IOLog ("didn't terminate the iSub connections because we didn't have an audioEngine\n");
#endif
		}
	} else {
#if DEBUGLOG
		IOLog ("didn't terminate the iSub connections owner = %p, arg1 = %p\n", owner, arg1);
#endif
    }

	debugIOLog ("-AppleDBDMAAudio::iSubCloseAction\n");
	return kIOReturnSuccess;
}

IOReturn AppleDBDMAAudio::iSubOpenAction (OSObject *owner, void *arg1, void *arg2, void *arg3, void *arg4) {
	IOReturn					result;
	bool						resultBool;

	debugIOLog ("+AppleDBDMAAudio::iSubOpenAction\n");

	result = kIOReturnError;
	resultBool = FALSE;

    if (NULL != owner) {
        AppleDBDMAAudio *		audioEngine;

		audioEngine = OSDynamicCast (AppleDBDMAAudio, owner);
		resultBool = audioEngine->iSubEngine->openiSub (audioEngine);
    }

	if (resultBool) {
		result = kIOReturnSuccess;
	}

	debugIOLog ("-AppleDBDMAAudio::iSubOpenAction\n");
	return result;
}

void AppleDBDMAAudio::iSubSynchronize(UInt32 firstSampleFrame, UInt32 numSampleFrames) 
{
	void *						iSubBuffer = NULL;
	SInt32						offsetDelta;
	SInt32						safetyOffset;
	UInt32						iSubBufferLen = 0;
	iSubAudioFormatType			iSubFormat;	
	UInt32						distance;
	static UInt32				oldiSubBufferOffset;

	UInt32						adaptiveSampleRate;
	UInt32						sampleRate;

	// pass in:
	//
	// еее in the iSubProcessingParams structure, need to set before this method
	// iSubBufferLen		iSubBufferMemory->getLength ()
	// iSubBuffer			(void*)iSubBufferMemory->getVirtualSegment (0, &iSubBufferLen)
	// sampleRate 			getSampleRate()->whole
	// iSubFormat			iSubEngine->Get methods
	//
	// еее in values/pointers
	// iSubEngineLoopCount	iSubEngine->GetCurrentLoopCount ()
	// iSubEngineByteCount	iSubEngine->GetCurrentByteCount ()
	// 
	// еее io pointers							$$$
	// &needToSync				member
	// &startiSub				member
	// &justResetClipPosition	member		
	// &initialiSubLead			member
	// &previousClippedToFrame	member
	// iSubEngineNeedToSync		iSubEngine->GetNeedToSync(), iSubEngine->SetNeedToSync()
	
	iSubBufferLen = iSubBufferMemory->getLength ();		
	iSubBuffer = (void*)iSubBufferMemory->getVirtualSegment (0, &iSubBufferLen); 
	// (iSubBufferLen / 2) is because iSubBufferOffset is in UInt16s so convert iSubBufferLen to UInt16 length
	iSubBufferLen = iSubBufferLen / 2;

	sampleRate = getSampleRate()->whole;		
	adaptiveSampleRate = sampleRate;

	iSubFormat.altInterface = iSubEngine->GetAltInterface();	
	iSubFormat.numChannels = iSubEngine->GetNumChannels();		
	iSubFormat.bytesPerSample = iSubEngine->GetBytesPerSample();		
	iSubFormat.outputSampleRate = iSubEngine->GetSampleRate();		

	if (needToSync == FALSE) {
		UInt32			wrote;
		wrote = miSubProcessingParams.iSubBufferOffset - oldiSubBufferOffset;
//			IOLog ("wrote %ld iSub samples\n", wrote);
		if (miSubProcessingParams.iSubLoopCount == iSubEngine->GetCurrentLoopCount () && miSubProcessingParams.iSubBufferOffset > (SInt32)(iSubEngine->GetCurrentByteCount () / 2)) {
			distance = miSubProcessingParams.iSubBufferOffset - (iSubEngine->GetCurrentByteCount () / 2);
		} else if (miSubProcessingParams.iSubLoopCount == (iSubEngine->GetCurrentLoopCount () + 1) && miSubProcessingParams.iSubBufferOffset < (SInt32)(iSubEngine->GetCurrentByteCount () / 2)) {
			distance = iSubBufferLen - (iSubEngine->GetCurrentByteCount () / 2) + miSubProcessingParams.iSubBufferOffset;
		}

		if (distance < (initialiSubLead / 2)) {			
			// Write more samples into the iSub's buffer
//				IOLog ("speed up! %ld, %ld, %ld\n", initialiSubLead, distance, iSubEngine->GetCurrentByteCount () / 2);
			adaptiveSampleRate = sampleRate - (sampleRate >> 4);
		} else if (distance > (initialiSubLead + (initialiSubLead / 2))) {
			// Write fewer samples into the iSub's buffer
//				IOLog ("slow down! %ld, %ld, %ld\n", initialiSubLead, distance, iSubEngine->GetCurrentByteCount () / 2);
			adaptiveSampleRate = sampleRate + (sampleRate >> 4);
		} else {
			// The sample rate is just right
//				IOLog ("just right %ld, %ld, %ld\n", initialiSubLead, distance, iSubEngine->GetCurrentByteCount () / 2);
			adaptiveSampleRate = sampleRate;
		}
	}
	
	// Detect being out of sync with the iSub
	if (needToSync == FALSE && previousClippedToFrame == firstSampleFrame && 0x0 != iSubEngine->GetCurrentLoopCount ()) {
		// aml - make the reader/writer check more strict - this helps get rid of long term crunchy iSub audio
		// the reader is now not allowed within one frame (one millisecond of audio) of the writer
		safetyOffset = miSubProcessingParams.iSubBufferOffset - ((iSubFormat.outputSampleRate) / 1000);		// 6 samples at 6kHz
		if (safetyOffset < 0) {
			safetyOffset += iSubBufferLen;
		}
		if (miSubProcessingParams.iSubLoopCount == iSubEngine->GetCurrentLoopCount () && safetyOffset < (SInt32)(iSubEngine->GetCurrentByteCount () / 2)) {
			#if DEBUGLOG
			distance = miSubProcessingParams.iSubBufferOffset - (iSubEngine->GetCurrentByteCount () / 2);
			IOLog ("****iSub is in front of write head safetyOffset = %ld, iSubEngine->GetCurrentByteCount () / 2 = %ld\n", safetyOffset, iSubEngine->GetCurrentByteCount () / 2);
//				IOLog ("distance = %ld\n", distance);
			#endif
			needToSync = TRUE;
			startiSub = TRUE;
		} else if (miSubProcessingParams.iSubLoopCount > (iSubEngine->GetCurrentLoopCount () + 1)) {
			#if DEBUGLOG
			IOLog ("****looped more than the iSub iSubLoopCount = %ld, iSubEngine->GetCurrentLoopCount () = %ld\n", miSubProcessingParams.iSubLoopCount, iSubEngine->GetCurrentLoopCount ());
			#endif
			needToSync = TRUE;
			startiSub = TRUE;
		} else if (miSubProcessingParams.iSubLoopCount < iSubEngine->GetCurrentLoopCount ()) {
			#if DEBUGLOG
			IOLog ("****iSub is ahead of us iSubLoopCount = %ld, iSubEngine->GetCurrentLoopCount () = %ld\n", miSubProcessingParams.iSubLoopCount, iSubEngine->GetCurrentLoopCount ());
			#endif
			needToSync = TRUE;
			startiSub = TRUE;
		} else if (miSubProcessingParams.iSubLoopCount == iSubEngine->GetCurrentLoopCount () && miSubProcessingParams.iSubBufferOffset > ((SInt32)( (iSubEngine->GetCurrentByteCount() + (((iSubFormat.outputSampleRate)/1000 * NUM_ISUB_FRAME_LISTS_TO_QUEUE * NUM_ISUB_FRAMES_PER_LIST) * iSubFormat.bytesPerSample * iSubFormat.numChannels) ) / 2))) {			// aml 3.27.02, this is the right number here (buffersize was 2x too large).
					
			#if DEBUGLOG
			IOLog ("****iSub is too far behind write head iSubBufferOffset = %ld, (iSubEngine->GetCurrentByteCount () / 2 + max queued data) = %ld\n", miSubProcessingParams.iSubBufferOffset, (iSubEngine->GetCurrentByteCount() / 2 + iSubBufferLen/2));					
			#endif
			needToSync = TRUE;
			startiSub = TRUE;
		}
	}
	if (FALSE == needToSync && previousClippedToFrame != firstSampleFrame && !(previousClippedToFrame == getNumSampleFramesPerBuffer () && firstSampleFrame == 0)) {
		#if DEBUGLOG
		IOLog ("clipOutput: no sync: iSubBufferOffset was %ld\n", miSubProcessingParams.iSubBufferOffset);
		#endif
		if (firstSampleFrame < previousClippedToFrame) {
			#if DEBUGLOG
			IOLog ("clipOutput: no sync: firstSampleFrame < previousClippedToFrame (delta = %ld)\n", previousClippedToFrame-firstSampleFrame);
			#endif
			// We've wrapped around the buffer
			offsetDelta = (getNumSampleFramesPerBuffer () - firstSampleFrame + previousClippedToFrame) * iSubEngine->GetNumChannels();	
		} else {
			#if DEBUGLOG
			IOLog ("clipOutput: no sync: previousClippedToFrame < firstSampleFrame (delta = %ld)\n", firstSampleFrame - previousClippedToFrame);
			#endif
			offsetDelta = (firstSampleFrame - previousClippedToFrame) * iSubEngine->GetNumChannels();
		}
		// aml 3.21.02, adjust for new sample rate
		offsetDelta = (offsetDelta * 1000) / ((sampleRate * 1000) / iSubFormat.outputSampleRate);

		miSubProcessingParams.iSubBufferOffset += offsetDelta;
		#if DEBUGLOG
		IOLog ("clipOutput: no sync: clip to point was %ld, now %ld (delta = %ld)\n", previousClippedToFrame, firstSampleFrame, offsetDelta);
		IOLog ("clipOutput: no sync: iSubBufferOffset is now %ld\n", miSubProcessingParams.iSubBufferOffset);
		#endif
		if (miSubProcessingParams.iSubBufferOffset > (SInt32)iSubBufferLen) {
			#if DEBUGLOG
			IOLog ("clipOutput: no sync: iSubBufferOffset > iSubBufferLen, iSubBufferOffset = %ld\n", miSubProcessingParams.iSubBufferOffset);
			#endif
			// Our calculated spot has actually wrapped around the iSub's buffer.
			miSubProcessingParams.iSubLoopCount += miSubProcessingParams.iSubBufferOffset / iSubBufferLen;
			miSubProcessingParams.iSubBufferOffset = miSubProcessingParams.iSubBufferOffset % iSubBufferLen;

			#if DEBUGLOG
			IOLog ("clipOutput: no sync: iSubBufferOffset > iSubBufferLen, iSubBufferOffset is now %ld\n", miSubProcessingParams.iSubBufferOffset);
			#endif
		} else if (miSubProcessingParams.iSubBufferOffset < 0) {

			miSubProcessingParams.iSubBufferOffset += iSubBufferLen;

			#if DEBUGLOG
			IOLog ("clipOutput: no sync: iSubBufferOffset < 0, iSubBufferOffset is now %ld\n", miSubProcessingParams.iSubBufferOffset);
			#endif
		}
	}

	if (TRUE == justResetClipPosition) {
		justResetClipPosition = FALSE;
		needToSync = FALSE;
		startiSub = FALSE;
	}

	// sync up with iSub only if everything is proceeding normally.
	// aml [3095619] - added check with iSubEngine for sync state.
	if ((TRUE == needToSync) || (iSubEngine->GetNeedToSync())) {		
		UInt32				curSampleFrame;
		
		// aml [3095619] reset iSub sync state if we've handled that case.
		iSubEngine->SetNeedToSync(false);								
		
		needToSync = FALSE;
					
		resetiSubProcessingState();
					
		// aml 4.25.02 wipe out the iSub buffer, changed due to moving zeroing of iSub buffer in AUA write handler when aborting the pipe
		bzero(iSubBuffer, iSubBufferLen);

		curSampleFrame = getCurrentSampleFrame ();

		if (TRUE == restartedDMA) {
			miSubProcessingParams.iSubBufferOffset = initialiSubLead;		
			restartedDMA = FALSE;
		} else {
			if (firstSampleFrame < curSampleFrame) {
				offsetDelta = (getNumSampleFramesPerBuffer () - curSampleFrame + firstSampleFrame) * iSubEngine->GetNumChannels();
			} else {
				offsetDelta = (firstSampleFrame - curSampleFrame) * iSubEngine->GetNumChannels();
			}
			#if DEBUGLOG
			IOLog ("clipOutput: need to sync: 44.1kHz offsetDelta = %ld\n", offsetDelta);

			if (offsetDelta < kMinimumLatency) {
				IOLog ("clipOutput: no sync: 44.1 offsetDelta < min, offsetDelta=%ld\n", offsetDelta); 
			}                
			#endif
			// aml 3.21.02, adjust for new sample rate
			offsetDelta = (offsetDelta * 1000) / ((sampleRate * 1000) / iSubFormat.outputSampleRate);
			#if DEBUGLOG
			IOLog ("clipOutput: need to sync: iSubBufferOffset = %ld, offsetDelta = %ld\n", miSubProcessingParams.iSubBufferOffset, offsetDelta);
			#endif

			miSubProcessingParams.iSubBufferOffset = offsetDelta;
			#if DEBUGLOG
			IOLog ("clipOutput: need to sync: offsetDelta = %ld\n", offsetDelta);
			IOLog ("clipOutput: need to sync: firstSampleFrame = %ld, curSampleFrame = %ld\n", firstSampleFrame, curSampleFrame);
			IOLog ("clipOutput: need to sync: starting iSubBufferOffset = %ld, numSampleFrames = %ld\n", miSubProcessingParams.iSubBufferOffset, numSampleFrames);
			#endif
			if (miSubProcessingParams.iSubBufferOffset > (SInt32)iSubBufferLen) {
		
				needToSync = TRUE;	// aml 4.24.02, requests larger than our buffer size = bad!
				#if DEBUGLOG
				IOLog ("clipOutput: need to sync: SubBufferOffset too big (%ld) RESYNC!\n", miSubProcessingParams.iSubBufferOffset);
				#endif
				
				// Our calculated spot has actually wrapped around the iSub's buffer.

				miSubProcessingParams.iSubLoopCount += miSubProcessingParams.iSubBufferOffset / iSubBufferLen;
				miSubProcessingParams.iSubBufferOffset = miSubProcessingParams.iSubBufferOffset % iSubBufferLen;

				#if DEBUGLOG
				IOLog ("clipOutput: need to sync: iSubBufferOffset > iSubBufferLen (%ld), iSubBufferOffset is now %ld\n", iSubBufferLen, miSubProcessingParams.iSubBufferOffset);
				#endif
			} else if (miSubProcessingParams.iSubBufferOffset < 0) {

				miSubProcessingParams.iSubBufferOffset += iSubBufferLen;

				#if DEBUGLOG
				IOLog ("clipOutput: need to sync: iSubBufferOffset < 0, iSubBufferOffset is now %ld\n", miSubProcessingParams.iSubBufferOffset);
				#endif
			}
			initialiSubLead = miSubProcessingParams.iSubBufferOffset;
		}
	}

	// [3094574] aml - updated iSub state, some of this could probably be done once off line, but it isn't any worse than before
	miSubProcessingParams.iSubBufferLen = iSubBufferLen;
	miSubProcessingParams.iSubFormat.altInterface = iSubEngine->GetAltInterface();
	miSubProcessingParams.iSubFormat.numChannels = iSubEngine->GetNumChannels();
	miSubProcessingParams.iSubFormat.bytesPerSample = iSubEngine->GetBytesPerSample();
	miSubProcessingParams.iSubFormat.outputSampleRate = iSubEngine->GetSampleRate();
	miSubProcessingParams.sampleRate = sampleRate;
	miSubProcessingParams.adaptiveSampleRate = adaptiveSampleRate;
	miSubProcessingParams.iSubBuffer = (SInt16*)iSubBuffer;

	return;
}

void AppleDBDMAAudio::resetiSubProcessingState() 
{ 	
	miSubProcessingParams.srcPhase =  1.0;		
	miSubProcessingParams.srcState =  0.0;		
				
	miSubProcessingParams.filterState.xl_1 = 0.0;
	miSubProcessingParams.filterState.xr_1 = 0.0;
	miSubProcessingParams.filterState.xl_2 = 0.0;
	miSubProcessingParams.filterState.xr_2 = 0.0;
	miSubProcessingParams.filterState.yl_1 = 0.0;
	miSubProcessingParams.filterState.yr_1 = 0.0;
	miSubProcessingParams.filterState.yl_2 = 0.0;
	miSubProcessingParams.filterState.yr_2 = 0.0;

	miSubProcessingParams.filterState2.xl_1 = 0.0;
	miSubProcessingParams.filterState2.xr_1 = 0.0;
	miSubProcessingParams.filterState2.xl_2 = 0.0;
	miSubProcessingParams.filterState2.xr_2 = 0.0;
	miSubProcessingParams.filterState2.yl_1 = 0.0;
	miSubProcessingParams.filterState2.yr_1 = 0.0;
	miSubProcessingParams.filterState2.yl_2 = 0.0;
	miSubProcessingParams.filterState2.yr_2 = 0.0;

	miSubProcessingParams.phaseCompState.xl_1 = 0.0;
	miSubProcessingParams.phaseCompState.xr_1 = 0.0;
	miSubProcessingParams.phaseCompState.xl_2 = 0.0;
	miSubProcessingParams.phaseCompState.xr_2 = 0.0;
	miSubProcessingParams.phaseCompState.yl_1 = 0.0;
	miSubProcessingParams.phaseCompState.yr_1 = 0.0;
	miSubProcessingParams.phaseCompState.yl_2 = 0.0;
	miSubProcessingParams.phaseCompState.yr_2 = 0.0;
	
	return;   	
}

bool AppleDBDMAAudio::willTerminate (IOService * provider, IOOptionBits options) {
    IOCommandGate *					cg;
	Boolean 						result;
	
	debug3IOLog ("+AppleDBDMAAudio[%p]::willTerminate (%p)\n", this, provider);

	if (iSubEngine == (AppleiSubEngine *)provider) {
		debugIOLog ("iSub requesting termination\n");

		cg = getCommandGate ();
		if (NULL != cg) {
			cg->runAction (iSubCloseAction);
		}

		// Set up notifier to run when iSub shows up again
		FailIf(NULL == iSubAttach, Exit);
		
		if (iSubAttach->getIntValue ()) {
			iSubEngineNotifier = addNotification (gIOPublishNotification, serviceMatching ("AppleiSubEngine"), (IOServiceNotificationHandler)&iSubEnginePublished, this);
		}
	}
	
	debug2IOLog ("AppleDBDMAAudio::willTerminate, before audioDevice retain count = %d\n", audioDevice->getRetainCount());

	result = super::willTerminate (provider, options);
	debug3IOLog ("-AppleDBDMAAudio[%p]::willTerminate, super::willTerminate () returned %d\n", this, result);

Exit:
	return result;
}


bool AppleDBDMAAudio::requestTerminate (IOService * provider, IOOptionBits options) {
	Boolean 						result;

	result = super::requestTerminate (provider, options);
	debug3IOLog ("AppleDBDMAAudio[%p]::requestTerminate, super::requestTerminate () returned %d\n", this, result);

	return result;
}

void AppleDBDMAAudio::updateiSubPosition(UInt32 firstSampleFrame, UInt32 numSampleFrames)
{
	if (TRUE == startiSub) {
		iSubEngine->StartiSub ();
		startiSub = FALSE;
		miSubProcessingParams.iSubLoopCount = 0;
 	}

	previousClippedToFrame = firstSampleFrame + numSampleFrames;
}

#pragma mark ------------------------ 
#pragma mark еее Utilities
#pragma mark ------------------------ 

inline void AppleDBDMAAudio::startTiming() {
#ifdef _TIME_CLIP_ROUTINE
	AbsoluteTime				uptime;
	AbsoluteTime				lastuptime;
	AbsoluteTime				tempTime;
	UInt64						nanos;

	mCallCount++;
	clock_get_uptime (&uptime);
	tempTime = uptime;
	if ((mCallCount % kCallFrequency) == 0) {
		SUB_ABSOLUTETIME (&uptime, &mPreviousUptime);
		absolutetime_to_nanoseconds (uptime, &nanos);
		debug3IOLog("clipOutputSamples[%ld]:\t%ld:", mCallCount, uptime.lo);
	}
	mPreviousUptime = tempTime;

	if ((mCallCount % kCallFrequency) == 0) {
		clock_get_uptime (&lastuptime);
	}	
#endif
}

inline void AppleDBDMAAudio::endTiming() {
#ifdef _TIME_CLIP_ROUTINE
	if ((mCallCount % kCallFrequency) == 0) {
		clock_get_uptime (&uptime);
		SUB_ABSOLUTETIME (&uptime, &lastuptime);
		absolutetime_to_nanoseconds (uptime, &nanos);
		debug2IOLog("%ld\n", uptime.lo);
	}
#endif
}
