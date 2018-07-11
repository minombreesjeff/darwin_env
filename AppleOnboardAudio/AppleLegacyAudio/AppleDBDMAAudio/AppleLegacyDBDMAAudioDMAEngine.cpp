#define DEBUGTIMESTAMPS		FALSE

#include "AppleLegacyDBDMAAudioDMAEngine.h"
#include "AppleLegacyAudio.h"

#include <IOKit/IOMemoryDescriptor.h>
#include <IOKit/audio/IOAudioDebug.h>

#include <IOKit/IOFilterInterruptEventSource.h>
#include <IOKit/IOWorkLoop.h>

#include "AudioHardwareUtilities.h"
#include "AppleiSubEngine.h"

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

OSDefineMetaClassAndStructors(AppleLegacyDBDMAAudioDMAEngine, super)

const int AppleLegacyDBDMAAudioDMAEngine::kDBDMADeviceIndex	= 0;
const int AppleLegacyDBDMAAudioDMAEngine::kDBDMAOutputIndex	= 1;
const int AppleLegacyDBDMAAudioDMAEngine::kDBDMAInputIndex	= 2;

#pragma mark ------------------------ 
#pragma mark еее IOAudioEngine Methods
#pragma mark ------------------------ 

bool AppleLegacyDBDMAAudioDMAEngine::filterInterrupt(int index)
{
	// check to see if this interupt is because the DMA went bad
    UInt32 result = IOGetDBDMAChannelStatus(ioBaseDMAOutput);
    UInt32 cmd = IOGetDBDMACommandPtr(ioBaseDMAOutput);

    if (!(result & kdbdmaActive)) {
        fBadResult = result;
        fBadCmd = cmd;
    }		

    if (status) 
    {
        // test the takeTimeStamp :it will increment the fCurrentLoopCount and time stamp it with the time now
        takeTimeStamp();
    }

    return false;
}

void AppleLegacyDBDMAAudioDMAEngine::free()
{
    if (interruptEventSource) {
        interruptEventSource->release();
        interruptEventSource = 0;
    }
    
	if (NULL != dmaCommandBufferInMemDescriptor) {
		dmaCommandBufferInMemDescriptor->release ();
		dmaCommandBufferInMemDescriptor = NULL;
	}
	if (NULL != dmaCommandBufferOutMemDescriptor) {
		dmaCommandBufferOutMemDescriptor->release ();
		dmaCommandBufferOutMemDescriptor = NULL;
	}
	if (NULL != sampleBufferInMemDescriptor) {
		sampleBufferInMemDescriptor->release ();
		sampleBufferInMemDescriptor = NULL;
	}
	if (NULL != sampleBufferOutMemDescriptor) {
		sampleBufferOutMemDescriptor->release ();
		sampleBufferOutMemDescriptor = NULL;
	}
	if (NULL != stopCommandMemDescriptor) {
		stopCommandMemDescriptor->release ();
		stopCommandMemDescriptor = NULL;
	}

    if (dmaCommandBufferOut && (commandBufferSize > 0)) {
        IOFreeAligned(dmaCommandBufferOut, commandBufferSize);
        dmaCommandBufferOut = 0;
    }
    
    if (dmaCommandBufferIn && (commandBufferSize > 0)) {
        IOFreeAligned(dmaCommandBufferIn, commandBufferSize);
        dmaCommandBufferOut = 0;
    }

    if (NULL != iSubEngineNotifier) {
        iSubEngineNotifier->remove ();
		iSubEngineNotifier = NULL;
    }

	if (NULL != iSubAttach) {
		iSubAttach->release ();
		iSubAttach = NULL;
	}

    if (NULL != miSubProcessingParams.lowFreqSamples) {
        IOFree (miSubProcessingParams.lowFreqSamples, (numBlocks * blockSize) * sizeof (float));
    }

    if (NULL != miSubProcessingParams.highFreqSamples) {
        IOFree (miSubProcessingParams.highFreqSamples, (numBlocks * blockSize) * sizeof (float));
    }

    super::free();
}

UInt32 AppleLegacyDBDMAAudioDMAEngine::getCurrentSampleFrame()
{
  
    UInt32 currentBlock = 0;

    if (ioBaseDMAOutput) {
        vm_offset_t currentDMACommandPhys, currentDMACommand;

        currentDMACommandPhys = (vm_offset_t)IOGetDBDMAChannelRegister(ioBaseDMAOutput, commandPtrLo);
        currentDMACommand = phystokv(currentDMACommandPhys);

        if ((UInt32)currentDMACommand > (UInt32)dmaCommandBufferOut) {
            currentBlock = ((UInt32)currentDMACommand - (UInt32)dmaCommandBufferOut) / sizeof(IODBDMADescriptor);
        }
    }

    return currentBlock * blockSize / 4;	// 4 bytes per frame - 2 per sample * 2 channels - BIG HACK
}

bool AppleLegacyDBDMAAudioDMAEngine::init(OSDictionary	*properties,
                                 IOService 			*theDeviceProvider,
                                 bool				hasInput,
                                 UInt32				nBlocks,
                                 UInt32				bSize,
                                 UInt32				rate,
                                 UInt16				bitDepth,
                                 UInt16				numChannels)
{
	IOAudioSampleRate initialSampleRate;
	IOMemoryMap *map;
	Boolean					result;

	CLOG("+ AppleLegacyDBDMAAudioDMAEngine::init\n");
	result = FALSE;

	// Ususal check
	FailIf (FALSE == super::init (properties), Exit);
	FailIf (NULL == theDeviceProvider, Exit);

	// create the memory places for DMA
	map = theDeviceProvider->mapDeviceMemoryWithIndex(AppleLegacyDBDMAAudioDMAEngine::kDBDMAOutputIndex);
	FailIf (NULL == map, Exit);
	ioBaseDMAOutput = (IODBDMAChannelRegisters *) map->getVirtualAddress();

	if(hasInput) {
		map = theDeviceProvider->mapDeviceMemoryWithIndex(AppleLegacyDBDMAAudioDMAEngine::kDBDMAInputIndex);
		FailIf (NULL == map, Exit);
		ioBaseDMAInput = (IODBDMAChannelRegisters *) map->getVirtualAddress();
	} else {
		ioBaseDMAInput = 0;
	}

	dmaCommandBufferIn = 0;
	dmaCommandBufferOut = 0;
	commandBufferSize = 0;
	interruptEventSource = 0;

	numBlocks = nBlocks;
	blockSize = bSize;
	setSampleOffset(kMinimumLatency);
	setNumSampleFramesPerBuffer(numBlocks * blockSize / sizeof (float));

	initialSampleRate.whole = rate;
	initialSampleRate.fraction = 0;

	setSampleRate(&initialSampleRate);
 
	mInputDualMonoMode = e_Mode_Disabled;		   
		   
	resetiSubProcessingState();
	
	mUseSoftwareInputGain = false;	
	mInputGainLPtr = NULL;	
	mInputGainRPtr = NULL;	

#ifdef _TIME_CLIP_ROUTINE
	mCallCount = 0;
	mPreviousUptime.hi = 0;
	mPreviousUptime.lo = 0;
#endif

	result = TRUE;

Exit:
	CLOG("- AppleLegacyDBDMAAudioDMAEngine::init\n");    
	return result;
}

bool AppleLegacyDBDMAAudioDMAEngine::initHardware(IOService *provider)
{
	vm_offset_t					offset;
	vm_offset_t					sampleBufOut;
	vm_offset_t					sampleBufIn;
	IOPhysicalAddress			commandBufferPhys;
	IOPhysicalAddress			sampleBufferPhys;
	IOPhysicalAddress			stopCommandPhys;
    UInt32						blockNum;
	UInt32						dmaCommand = 0;
    Boolean						doInterrupt = false;
    UInt32						interruptIndex;
    IOWorkLoop *				workLoop;
    IOAudioStream *				stream;
	Boolean						result;

	result = FALSE;
	sampleBufIn = NULL;
    IOAudioStreamFormat format = {
            2,
            kIOAudioStreamSampleFormatLinearPCM,
            kIOAudioStreamNumericRepresentationSignedInt,
            16,
            16,
            kIOAudioStreamAlignmentHighByte,
            kIOAudioStreamByteOrderBigEndian,
            true
    };
	
	//	rbm 7.15.2002 keep a copy for user client
	dbdmaFormat.fNumChannels = format.fNumChannels;
	dbdmaFormat.fSampleFormat = format.fSampleFormat;
	dbdmaFormat.fNumericRepresentation = format.fNumericRepresentation;
	dbdmaFormat.fBitDepth = format.fBitDepth;
	dbdmaFormat.fBitWidth = format.fBitWidth;
	dbdmaFormat.fAlignment = format.fAlignment;
	dbdmaFormat.fIsMixable = format.fIsMixable;
	dbdmaFormat.fDriverTag = format.fDriverTag;
        
    DEBUG_IOLOG("+ AppleLegacyDBDMAAudioDMAEngine::initHardware()\n");
    
    ourProvider = provider;
    fBadCmd = 0;
    fBadResult = 0;

    FailIf (!super::initHardware(provider), Exit);
        
	// allocate the memory for the buffer
    sampleBufOut = (vm_offset_t)IOMallocAligned(numBlocks * blockSize, PAGE_SIZE);
    if(ioBaseDMAInput)
        sampleBufIn = (vm_offset_t)IOMallocAligned(numBlocks * blockSize, PAGE_SIZE);
    
	// create the streams
    stream = new IOAudioStream;
    if (stream) {
        const IOAudioSampleRate *rate;        
        rate = getSampleRate();
        
        stream->initWithAudioEngine(this, kIOAudioStreamDirectionOutput, 1, 0, 0);
        stream->setSampleBuffer((void *)sampleBufOut, numBlocks * blockSize);
        stream->addAvailableFormat(&format, rate, rate);
        stream->setFormat(&format);

        addAudioStream(stream);
        stream->release();
    }
    
    if(ioBaseDMAInput) {
        stream = new IOAudioStream;
        if (stream) {
            const IOAudioSampleRate *rate;        
            rate = getSampleRate();
        
            stream->initWithAudioEngine(this, kIOAudioStreamDirectionInput, 1, 0, 0);
            stream->setSampleBuffer((void *)sampleBufIn, numBlocks * blockSize);
            stream->addAvailableFormat(&format, rate, rate);
            stream->setFormat(&format);

            addAudioStream(stream);
            stream->release();
        }
    }

    FailIf (!status || !sampleBufOut, Exit);
    if(ioBaseDMAInput) 
        FailIf (!sampleBufIn, Exit);

	// create the DMA output part
    commandBufferSize = (numBlocks + 1) * sizeof(IODBDMADescriptor);
    dmaCommandBufferOut = (IODBDMADescriptor *)IOMallocAligned(commandBufferSize, 32); 
                                                            // needs to be more than 4 byte aligned
    FailIf (!dmaCommandBufferOut, Exit);

	dmaCommandBufferOutMemDescriptor = IOMemoryDescriptor::withAddress (dmaCommandBufferOut, commandBufferSize, kIODirectionOut);
	FailIf (NULL == dmaCommandBufferOutMemDescriptor, Exit);
	sampleBufferOutMemDescriptor = IOMemoryDescriptor::withAddress ((void *)sampleBufOut, numBlocks * blockSize, kIODirectionOut);
	FailIf (NULL == sampleBufferOutMemDescriptor, Exit);
	stopCommandMemDescriptor = IOMemoryDescriptor::withAddress (&dmaCommandBufferOut[numBlocks], sizeof (IODBDMADescriptor *), kIODirectionOut);
	FailIf (NULL == stopCommandMemDescriptor, Exit);

	commandBufferPhys = dmaCommandBufferOutMemDescriptor->getPhysicalAddress ();
	FailIf (NULL == commandBufferPhys, Exit);
	sampleBufferPhys = sampleBufferOutMemDescriptor->getPhysicalAddress ();
	FailIf (NULL == sampleBufferPhys, Exit);
	stopCommandPhys = stopCommandMemDescriptor->getPhysicalAddress ();
	FailIf (NULL == stopCommandPhys, Exit);

    offset = 0;
    dmaCommand = kdbdmaOutputMore;
    interruptIndex = kDBDMAOutputIndex;

	// install an interrupt handler only on the Ouput size of it
    workLoop = getWorkLoop();
    FailIf (!workLoop, Exit);
    
    interruptEventSource = IOFilterInterruptEventSource::filterInterruptEventSource(this,
                                                                               AppleLegacyDBDMAAudioDMAEngine::interruptHandler,
                                                                               AppleLegacyDBDMAAudioDMAEngine::interruptFilter,
                                                                               audioDevice->getProvider(),
                                                                               interruptIndex);
    FailIf (!interruptEventSource, Exit);
    workLoop->addEventSource(interruptEventSource);

	// create the DMA program
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
        dmaCommandBufferIn = (IODBDMADescriptor *)IOMallocAligned(commandBufferSize, 32); 
                                                            // needs to be more than 4 byte aligned
        FailIf (!dmaCommandBufferIn, Exit);

		dmaCommandBufferInMemDescriptor = IOMemoryDescriptor::withAddress (dmaCommandBufferIn, commandBufferSize, kIODirectionOut);
		FailIf (NULL == dmaCommandBufferInMemDescriptor, Exit);
		sampleBufferInMemDescriptor = IOMemoryDescriptor::withAddress ((void *)sampleBufIn, numBlocks * blockSize, kIODirectionIn);
		FailIf (NULL == sampleBufferInMemDescriptor, Exit);
		stopCommandMemDescriptor = IOMemoryDescriptor::withAddress (&dmaCommandBufferIn[numBlocks], sizeof (IODBDMADescriptor *), kIODirectionOut);
		FailIf (NULL == stopCommandMemDescriptor, Exit);

		commandBufferPhys = dmaCommandBufferInMemDescriptor->getPhysicalAddress ();
		FailIf (NULL == commandBufferPhys, Exit);
		sampleBufferPhys = sampleBufferInMemDescriptor->getPhysicalAddress ();
		FailIf (NULL == sampleBufferPhys, Exit);
		stopCommandPhys = stopCommandMemDescriptor->getPhysicalAddress ();
		FailIf (NULL == stopCommandPhys, Exit);

        doInterrupt = false;
        offset = 0;
        dmaCommand = kdbdmaInputMore;    
        
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

    iSubBufferMemory = NULL;
	iSubEngine = NULL;

	// Set up a control that sound prefs can set to tell us if we should install our notifier or not
	iSubAttach = IOAudioToggleControl::create (FALSE,
										kIOAudioControlChannelIDAll,
										kIOAudioControlChannelNameAll,
										0,
										kIOAudioToggleControlSubTypeiSubAttach,
										kIOAudioControlUsageOutput);

	if (NULL != iSubAttach) {
		addDefaultAudioControl (iSubAttach);
		iSubAttach->setValueChangeHandler ((IOAudioControl::IntValueChangeHandler)iSubAttachChangeHandler, this);
		iSubAttach->release ();
	}

	chooseOutputClippingRoutinePtr();
	chooseInputConversionRoutinePtr();
	
	result = TRUE;

Exit:
    DEBUG_IOLOG("- AppleLegacyDBDMAAudioDMAEngine::initHardware()\n");
    return result;
}


bool AppleLegacyDBDMAAudioDMAEngine::interruptFilter(OSObject *owner, IOFilterInterruptEventSource *source)
{
    register AppleLegacyDBDMAAudioDMAEngine *dmaEngine = (AppleLegacyDBDMAAudioDMAEngine *)owner;
    bool result = true;

    if (dmaEngine) {
        result = dmaEngine->filterInterrupt(source->getIntIndex());
    }

    return result;
}

void AppleLegacyDBDMAAudioDMAEngine::interruptHandler(OSObject *owner, IOInterruptEventSource *source, int count)
{
    return;
}

IOReturn AppleLegacyDBDMAAudioDMAEngine::performAudioEngineStart()
{
	IOPhysicalAddress			commandBufferPhys;
	IOReturn					result;

    debugIOLog(" + AppleLegacyDBDMAAudioDMAEngine::performAudioEngineStart()\n");

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

	// start the input DMA first
    if(ioBaseDMAInput) {
        IOSetDBDMAChannelControl(ioBaseDMAInput, IOClearDBDMAChannelControlBits(kdbdmaS0));
        IOSetDBDMABranchSelect(ioBaseDMAInput, IOSetDBDMAChannelControlBits(kdbdmaS0));
		commandBufferPhys = dmaCommandBufferInMemDescriptor->getPhysicalAddress ();
		FailIf (NULL == commandBufferPhys, Exit);
		IODBDMAStart(ioBaseDMAInput, (IODBDMADescriptor *)commandBufferPhys);
    }
    
    IOSetDBDMAChannelControl(ioBaseDMAOutput, IOClearDBDMAChannelControlBits(kdbdmaS0));
    IOSetDBDMABranchSelect(ioBaseDMAOutput, IOSetDBDMAChannelControlBits(kdbdmaS0));
	commandBufferPhys = dmaCommandBufferOutMemDescriptor->getPhysicalAddress ();
	FailIf (NULL == commandBufferPhys, Exit);
	((AppleLegacyAudio *)audioDevice)->sndHWSetCurrentSampleFrame (0);
	IODBDMAStart(ioBaseDMAOutput, (IODBDMADescriptor *)commandBufferPhys);

	dmaRunState = TRUE;				//	rbm 7.12.02	added for user client support
	result = kIOReturnSuccess;

    debugIOLog(" - AppleLegacyDBDMAAudioDMAEngine::performAudioEngineStart()\n");

Exit:
    return result;
}

IOReturn AppleLegacyDBDMAAudioDMAEngine::performAudioEngineStop()
{
    UInt16 attemptsToStop = 1000;

    debugIOLog("+ AppleLegacyDBDMAAudioDMAEngine::performAudioEngineStop()\n");

    if (NULL != iSubEngine) {
        iSubEngine->StopiSub ();
        needToSync = TRUE;
    }

    if (!interruptEventSource) {
        return kIOReturnError;
    }

    interruptEventSource->disable();
        
	// stop the output
    IOSetDBDMAChannelControl(ioBaseDMAOutput, IOSetDBDMAChannelControlBits(kdbdmaS0));
    while ((IOGetDBDMAChannelStatus(ioBaseDMAOutput) & kdbdmaActive) && (attemptsToStop--)) {
        eieio();
        IOSleep(1);
    }

    IODBDMAStop(ioBaseDMAOutput);
    IODBDMAReset(ioBaseDMAOutput);

	// stop the input
    if(ioBaseDMAInput){
        IOSetDBDMAChannelControl(ioBaseDMAInput, IOSetDBDMAChannelControlBits(kdbdmaS0));
        while ((IOGetDBDMAChannelStatus(ioBaseDMAInput) & kdbdmaActive) && (attemptsToStop--)) {
            eieio();
            IOSleep(1);
        }

        IODBDMAStop(ioBaseDMAInput);
        IODBDMAReset(ioBaseDMAInput);
    }
    
	dmaRunState = FALSE;				//	rbm 7.12.02	added for user client support
    interruptEventSource->enable();

    DEBUG_IOLOG("- AppleLegacyDBDMAAudioDMAEngine::performAudioEngineStop()\n");
    return kIOReturnSuccess;
}

// This gets called when a new audio stream needs to be mixed into an already playing audio stream
void AppleLegacyDBDMAAudioDMAEngine::resetClipPosition (IOAudioStream *audioStream, UInt32 clipSampleFrame) {
  if ((NULL != iSubBufferMemory) && (NULL != iSubEngine)) {
				
		resetiSubProcessingState();

		#if DEBUGLOG
        IOLog ("+resetClipPosition: iSubBufferOffset=%ld, previousClippedToFrame=%ld, clipSampleFrame=%ld\n", miSubProcessingParams.iSubBufferOffset, previousClippedToFrame, clipSampleFrame);
		#endif
        if (previousClippedToFrame < clipSampleFrame) {
			// Resetting the clip point backwards around the end of the buffer
			clipAdjustment = (getNumSampleFramesPerBuffer () - clipSampleFrame + previousClippedToFrame) * iSubEngine->GetNumChannels();
        } else {
			clipAdjustment = (previousClippedToFrame - clipSampleFrame) * iSubEngine->GetNumChannels();
        }
		#if DEBUGLOG
        if (clipAdjustment < kMinimumLatency) {
            IOLog ("resetClipPosition: 44.1 clipAdjustment < min, clipAdjustment=%ld\n", clipAdjustment); 
        }                
		#endif
        clipAdjustment = (clipAdjustment * 1000) / ((1000 * getSampleRate()->whole) / iSubEngine->GetSampleRate());  
        miSubProcessingParams.iSubBufferOffset -= clipAdjustment;

		#if DEBUGLOG
        if (clipAdjustment > (iSubBufferMemory->getLength () / 2)) {
            IOLog ("resetClipPosition: clipAdjustment > iSub buffer size, clipAdjustment=%ld\n", clipAdjustment); 
        }                
		#endif

        if (miSubProcessingParams.iSubBufferOffset < 0) {
			miSubProcessingParams.iSubBufferOffset += (iSubBufferMemory->getLength () / 2);	
			miSubProcessingParams.iSubLoopCount--;
        }

        previousClippedToFrame = clipSampleFrame;
        justResetClipPosition = TRUE;

		#if DEBUGLOG
        IOLog ("-resetClipPosition: iSubBufferOffset=%ld, previousClippedToFrame=%ld\n", miSubProcessingParams.iSubBufferOffset, previousClippedToFrame);
		#endif
    }
}

IOReturn AppleLegacyDBDMAAudioDMAEngine::restartOutputIfFailure(){
	IOPhysicalAddress			commandBufferPhys;
	IOReturn					result;

	result = kIOReturnError;
    FailIf (!ioBaseDMAOutput || !dmaCommandBufferOut || !status || !interruptEventSource, Exit);

#if DEBUGLOG
	IOLog ("Restarting DMA\n");
#endif
    flush_dcache((vm_offset_t)dmaCommandBufferOut, commandBufferSize, false);

	if (NULL != iSubEngine) {
		needToSync = TRUE;
		startiSub = TRUE;
		restartedDMA = TRUE;
	}

    interruptEventSource->enable();

	// add the time stamp take to test
    takeTimeStamp(false);
    
    IOSetDBDMAChannelControl(ioBaseDMAOutput, IOClearDBDMAChannelControlBits(kdbdmaS0));
    IOSetDBDMABranchSelect(ioBaseDMAOutput, IOSetDBDMAChannelControlBits(kdbdmaS0));
	commandBufferPhys = dmaCommandBufferOutMemDescriptor->getPhysicalAddress ();
	FailIf (NULL == commandBufferPhys, Exit);
	IODBDMAStart(ioBaseDMAOutput, (IODBDMADescriptor *)commandBufferPhys);

	result = kIOReturnSuccess;

Exit:
    return result;
}

void AppleLegacyDBDMAAudioDMAEngine::setSampleLatencies (UInt32 outputLatency, UInt32 inputLatency) {
	setOutputSampleLatency (outputLatency);
	setInputSampleLatency (inputLatency);
}

void AppleLegacyDBDMAAudioDMAEngine::stop(IOService *provider)
{
    IOWorkLoop *workLoop;
    
    DEBUG3_IOLOG(" + AppleLegacyDBDMAAudioDMAEngine[%p]::stop(%p)\n", this, provider);
    
    if (interruptEventSource) {
        workLoop = getWorkLoop();
        if (workLoop) {
            workLoop->removeEventSource(interruptEventSource);
        }
    }
    
    super::stop(provider);
    stopAudioEngine();
    DEBUG3_IOLOG(" - AppleLegacyDBDMAAudioDMAEngine[%p]::stop(%p)\n", this, provider);
}

#pragma mark ------------------------ 
#pragma mark еее Conversion Routines
#pragma mark ------------------------ 

// [3094574] aml, pick the correct output conversion routine based on our current state
void AppleLegacyDBDMAAudioDMAEngine::chooseOutputClippingRoutinePtr()
{
	if ((NULL != iSubBufferMemory) && (NULL != iSubEngine)) {
		if (32 == dbdmaFormat.fBitWidth) {
			if (TRUE == fNeedsRightChanMixed) {
				mClipAppleLegacyDBDMAToOutputStreamRoutine = &AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream32iSubMixRightChannel;
			} else if (TRUE == fNeedsRightChanDelay) {
				mClipAppleLegacyDBDMAToOutputStreamRoutine = &AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream32iSubDelayRightChannel;
			} else {
				mClipAppleLegacyDBDMAToOutputStreamRoutine = &AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream32iSub;
			}
		} else if (16 == dbdmaFormat.fBitWidth) {
			if (TRUE == fNeedsPhaseInversion) {
				mClipAppleLegacyDBDMAToOutputStreamRoutine = &AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream16iSubInvertRightChannel;
			} else if (TRUE == fNeedsRightChanMixed) {
				mClipAppleLegacyDBDMAToOutputStreamRoutine = &AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream16iSubMixRightChannel;
			} else if (TRUE == fNeedsRightChanDelay) {
				mClipAppleLegacyDBDMAToOutputStreamRoutine = &AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream16iSubDelayRightChannel;
			} else {
				mClipAppleLegacyDBDMAToOutputStreamRoutine = &AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream16iSub;
			}
		} else {
			IOLog("AppleLegacyDBDMAAudioDMAEngine::chooseOutputClippingRoutinePtr - iSub attached, non-supported output bit depth!\n");
		}
	} else {
		if (32 == dbdmaFormat.fBitWidth) {
			if (TRUE == fNeedsRightChanMixed) {
				mClipAppleLegacyDBDMAToOutputStreamRoutine = &AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream32MixRightChannel;
			} else if (TRUE == fNeedsRightChanDelay) {
				mClipAppleLegacyDBDMAToOutputStreamRoutine = &AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream32DelayRightChannel;
			} else {
				mClipAppleLegacyDBDMAToOutputStreamRoutine = &AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream32;
			}
		} else if (16 == dbdmaFormat.fBitWidth) {
			if (TRUE == fNeedsPhaseInversion) {
				mClipAppleLegacyDBDMAToOutputStreamRoutine = &AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream16InvertRightChannel;
			} else if (TRUE == fNeedsRightChanMixed) {
				mClipAppleLegacyDBDMAToOutputStreamRoutine = &AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream16MixRightChannel;
			} else if (TRUE == fNeedsRightChanDelay) {
				mClipAppleLegacyDBDMAToOutputStreamRoutine = &AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream16DelayRightChannel;
			} else {
				mClipAppleLegacyDBDMAToOutputStreamRoutine = &AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream16;
			}
		} else {
			IOLog("AppleLegacyDBDMAAudioDMAEngine::chooseOutputClippingRoutinePtr - Non-supported output bit depth!\n");
		}
	}
}

// [3094574] aml, pick the correct input conversion routine based on our current state
void AppleLegacyDBDMAAudioDMAEngine::chooseInputConversionRoutinePtr() 
{
	if (32 == dbdmaFormat.fBitWidth) {
		if (mUseSoftwareInputGain) {
			mConvertInputStreamToAppleLegacyDBDMARoutine = &AppleLegacyDBDMAAudioDMAEngine::convertAppleLegacyDBDMAFromInputStream32WithGain;
		} else {
			mConvertInputStreamToAppleLegacyDBDMARoutine = &AppleLegacyDBDMAAudioDMAEngine::convertAppleLegacyDBDMAFromInputStream32;
		}
	} else if (16 == dbdmaFormat.fBitWidth) {
		if (mUseSoftwareInputGain) {
			mConvertInputStreamToAppleLegacyDBDMARoutine = &AppleLegacyDBDMAAudioDMAEngine::convertAppleLegacyDBDMAFromInputStream16WithGain;
		} else {
			if (e_Mode_CopyRightToLeft == mInputDualMonoMode) {
				mConvertInputStreamToAppleLegacyDBDMARoutine = &AppleLegacyDBDMAAudioDMAEngine::convertAppleLegacyDBDMAFromInputStream16CopyR2L;
			} else {
				mConvertInputStreamToAppleLegacyDBDMARoutine = &AppleLegacyDBDMAAudioDMAEngine::convertAppleLegacyDBDMAFromInputStream16;
			}
		}
	} else {
		IOLog("AppleLegacyDBDMAAudioDMAEngine::chooseInputConversionRoutinePtr - Non-supported input bit depth!\n");
	}
}

IOReturn AppleLegacyDBDMAAudioDMAEngine::clipOutputSamples(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat, IOAudioStream *audioStream)
{
	IOReturn result;
 
 	// if the DMA went bad restart it
	if (fBadCmd && fBadResult)
	{
		fBadCmd = 0;
		fBadResult = 0;
		restartOutputIfFailure();
	}

	startTiming();

	// [3094574] aml, use function pointer instead of if/else block - handles both iSub and non-iSub clipping cases.
	result = (*this.*mClipAppleLegacyDBDMAToOutputStreamRoutine)(mixBuf, sampleBuf, firstSampleFrame, numSampleFrames, streamFormat);

	endTiming();

	return result;
}

// [3094574] aml, use function pointer instead of if/else block
IOReturn AppleLegacyDBDMAAudioDMAEngine::convertInputSamples(const void *sampleBuf, void *destBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat, IOAudioStream *audioStream)
{
	IOReturn result;

	result = (*this.*mConvertInputStreamToAppleLegacyDBDMARoutine)(sampleBuf, destBuf, firstSampleFrame, numSampleFrames, streamFormat);

	return result;
}

#pragma mark ------------------------ 
#pragma mark еее Output Routines
#pragma mark ------------------------ 

// ------------------------------------------------------------------------
// Float32 to Native SInt16
// ------------------------------------------------------------------------
IOReturn AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream16(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
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
IOReturn AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream16DelayRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
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
IOReturn AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream16InvertRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
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
IOReturn AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream16MixRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
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
IOReturn AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream32(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
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
IOReturn AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream32DelayRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
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
IOReturn AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream32MixRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
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
IOReturn AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream16iSub(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
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
IOReturn AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream16iSubDelayRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
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
IOReturn AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream16iSubInvertRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
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
IOReturn AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream16iSubMixRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
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
IOReturn AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream32iSub(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
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
IOReturn AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream32iSubDelayRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
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
IOReturn AppleLegacyDBDMAAudioDMAEngine::clipAppleLegacyDBDMAToOutputStream32iSubMixRightChannel(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
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
IOReturn AppleLegacyDBDMAAudioDMAEngine::convertAppleLegacyDBDMAFromInputStream16(const void *sampleBuf, void *destBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
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
IOReturn AppleLegacyDBDMAAudioDMAEngine::convertAppleLegacyDBDMAFromInputStream16CopyR2L(const void *sampleBuf, void *destBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
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
IOReturn AppleLegacyDBDMAAudioDMAEngine::convertAppleLegacyDBDMAFromInputStream16WithGain(const void *sampleBuf, void *destBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
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
IOReturn AppleLegacyDBDMAAudioDMAEngine::convertAppleLegacyDBDMAFromInputStream32(const void *sampleBuf, void *destBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
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
IOReturn AppleLegacyDBDMAAudioDMAEngine::convertAppleLegacyDBDMAFromInputStream32WithGain(const void *sampleBuf, void *destBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat)
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

void AppleLegacyDBDMAAudioDMAEngine::setDualMonoMode(const DualMonoModeType inDualMonoMode) 
{ 
	mInputDualMonoMode = inDualMonoMode; 
	chooseInputConversionRoutinePtr();

	return;   	
}

void AppleLegacyDBDMAAudioDMAEngine::setInputGainL(UInt32 inGainL) 
{ 
    if (mInputGainLPtr == NULL) {        
        mInputGainLPtr = (float *)IOMalloc(sizeof(float));
    }
    inputGainConverter(inGainL, mInputGainLPtr);
	
    return;   
} 

void AppleLegacyDBDMAAudioDMAEngine::setInputGainR(UInt32 inGainR) 
{ 
    if (mInputGainRPtr == NULL) {        
        mInputGainRPtr = (float *)IOMalloc(sizeof(float));
    }
    inputGainConverter(inGainR, mInputGainRPtr);

    return;   
} 

// [3094574] aml, updated routines below to set the proper clipping routine

void AppleLegacyDBDMAAudioDMAEngine::setPhaseInversion(const bool needsPhaseInversion) 
{
	fNeedsPhaseInversion = needsPhaseInversion; 
	chooseOutputClippingRoutinePtr();
	
	return;   
}

void AppleLegacyDBDMAAudioDMAEngine::setRightChanDelay(const bool needsRightChanDelay)  
{
	fNeedsRightChanDelay = needsRightChanDelay;  
	chooseOutputClippingRoutinePtr();
	
	return;   
}

void AppleLegacyDBDMAAudioDMAEngine::setRightChanMixed(const bool needsRightChanMixed)  
{
	fNeedsRightChanMixed = needsRightChanMixed;  
	chooseOutputClippingRoutinePtr();
	
	return;   
}

void AppleLegacyDBDMAAudioDMAEngine::setUseSoftwareInputGain(const bool inUseSoftwareInputGain) 
{     
	mUseSoftwareInputGain = inUseSoftwareInputGain;     	
	chooseInputConversionRoutinePtr();
	
	return;   
}

#pragma mark ------------------------ 
#pragma mark еее Format Routines
#pragma mark ------------------------ 

IOReturn AppleLegacyDBDMAAudioDMAEngine::getAudioStreamFormat( IOAudioStreamFormat * streamFormatPtr )
{
	if ( NULL != streamFormatPtr ) {
		streamFormatPtr->fNumChannels = dbdmaFormat.fNumChannels;
		streamFormatPtr->fSampleFormat = dbdmaFormat.fSampleFormat;
		streamFormatPtr->fNumericRepresentation = dbdmaFormat.fNumericRepresentation;
		streamFormatPtr->fBitDepth = dbdmaFormat.fBitDepth;
		streamFormatPtr->fBitWidth = dbdmaFormat.fBitWidth;
		streamFormatPtr->fAlignment = dbdmaFormat.fAlignment;
		streamFormatPtr->fByteOrder = dbdmaFormat.fByteOrder;
		streamFormatPtr->fIsMixable = dbdmaFormat.fIsMixable;
		streamFormatPtr->fDriverTag = dbdmaFormat.fDriverTag;
	}
	return kIOReturnSuccess;
}

bool AppleLegacyDBDMAAudioDMAEngine::getDmaState (void )
{
	return dmaRunState;
}

IOReturn AppleLegacyDBDMAAudioDMAEngine::performFormatChange(IOAudioStream *audioStream, const IOAudioStreamFormat *newFormat, const IOAudioSampleRate *newSampleRate)
{
	if ( NULL != newFormat ) {									//	rbm 7.15.2002 keep a copy for user client
		dbdmaFormat.fNumChannels = newFormat->fNumChannels;
		dbdmaFormat.fSampleFormat = newFormat->fSampleFormat;
		dbdmaFormat.fNumericRepresentation = newFormat->fNumericRepresentation;
		dbdmaFormat.fBitDepth = newFormat->fBitDepth;
		dbdmaFormat.fBitWidth = newFormat->fBitWidth;
		dbdmaFormat.fAlignment = newFormat->fAlignment;
		dbdmaFormat.fByteOrder = newFormat->fByteOrder;
		dbdmaFormat.fIsMixable = newFormat->fIsMixable;
		dbdmaFormat.fDriverTag = newFormat->fDriverTag;
	}

	// [3094574] aml, set the proper clipping routine
	chooseOutputClippingRoutinePtr();
	chooseInputConversionRoutinePtr();

    return kIOReturnSuccess;
}

#pragma mark ------------------------ 
#pragma mark еее iSub Support
#pragma mark ------------------------ 

IOReturn AppleLegacyDBDMAAudioDMAEngine::iSubAttachChangeHandler (IOService *target, IOAudioControl *attachControl, SInt32 oldValue, SInt32 newValue) {
    IOReturn						result;
    AppleLegacyDBDMAAudioDMAEngine *		audioDMAEngine;
    IOCommandGate *					cg;

	debug5IOLog ("+ AppleLegacyDBDMAAudioDMAEngine::iSubAttachChangeHandler (%p, %p, 0x%lx, 0x%lx)\n", target, attachControl, oldValue, newValue);

	result = kIOReturnSuccess;
	FailIf (oldValue == newValue, Exit);
    audioDMAEngine = OSDynamicCast (AppleLegacyDBDMAAudioDMAEngine, target);
	FailIf (NULL == audioDMAEngine, Exit);

	if (newValue) {
		debugIOLog ("try to connect to an iSub\n");
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
		debugIOLog ("do not try to connect to iSub\n");
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

Exit:
    debugIOLog ("- AppleLegacyDBDMAAudioDMAEngine::iSubAttachChangeHandler\n");
    return result;
}

bool AppleLegacyDBDMAAudioDMAEngine::iSubEnginePublished (AppleLegacyDBDMAAudioDMAEngine * dbdmaEngineObject, void * refCon, IOService * newService) {
	IOReturn						result;
	bool							resultCode;
    IOCommandGate *					cg;

	debug4IOLog ("+AppleLegacyDBDMAAudioDMAEngine::iSubEnginePublished (%p, %p, %p)\n", dbdmaEngineObject, (UInt32*)refCon, newService);

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
	
	debug5IOLog ("-AppleLegacyDBDMAAudioDMAEngine::iSubEnginePublished (%p, %p, %p), result = %d\n", dbdmaEngineObject, (UInt32 *)refCon, newService, resultCode);
	return resultCode;
}

IOReturn AppleLegacyDBDMAAudioDMAEngine::iSubCloseAction (OSObject *owner, void *arg1, void *arg2, void *arg3, void *arg4) {
    if (NULL != owner) {
        AppleLegacyDBDMAAudioDMAEngine *		audioEngine;

		debugIOLog ("+AppleLegacyDBDMAAudioDMAEngine::iSubCloseAction\n");

		audioEngine = OSDynamicCast (AppleLegacyDBDMAAudioDMAEngine, owner);

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

	debugIOLog ("-AppleLegacyDBDMAAudioDMAEngine::iSubCloseAction\n");
	return kIOReturnSuccess;
}

IOReturn AppleLegacyDBDMAAudioDMAEngine::iSubOpenAction (OSObject *owner, void *arg1, void *arg2, void *arg3, void *arg4) {
	IOReturn					result;
	bool						resultBool;

	debugIOLog ("+AppleLegacyDBDMAAudioDMAEngine::iSubOpenAction\n");

	result = kIOReturnError;
	resultBool = FALSE;

    if (NULL != owner) {
        AppleLegacyDBDMAAudioDMAEngine *		audioEngine;

		audioEngine = OSDynamicCast (AppleLegacyDBDMAAudioDMAEngine, owner);
		resultBool = audioEngine->iSubEngine->openiSub (audioEngine);
    }

	if (resultBool) {
		result = kIOReturnSuccess;
	}

	debugIOLog ("-AppleLegacyDBDMAAudioDMAEngine::iSubOpenAction\n");
	return result;
}

void AppleLegacyDBDMAAudioDMAEngine::iSubSynchronize(UInt32 firstSampleFrame, UInt32 numSampleFrames) 
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

void AppleLegacyDBDMAAudioDMAEngine::resetiSubProcessingState() 
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

bool AppleLegacyDBDMAAudioDMAEngine::willTerminate (IOService * provider, IOOptionBits options) {
    IOCommandGate *					cg;

	debug3IOLog ("+AppleLegacyDBDMAAudioDMAEngine[%p]::willTerminate (%p)\n", this, provider);

	if (iSubEngine == (AppleiSubEngine *)provider) {
		debugIOLog ("iSub requesting termination\n");

		cg = getCommandGate ();
		if (NULL != cg) {
			cg->runAction (iSubCloseAction);
		}

		// Set up notifier to run when iSub shows up again
		if (iSubAttach->getIntValue ()) {
			iSubEngineNotifier = addNotification (gIOPublishNotification, serviceMatching ("AppleiSubEngine"), (IOServiceNotificationHandler)&iSubEnginePublished, this);
		}
	}

	debug2IOLog ("-AppleLegacyDBDMAAudioDMAEngine[%p]::willTerminate - about to call super::willTerminate ()\n", this);

	return super::willTerminate (provider, options);
}

void AppleLegacyDBDMAAudioDMAEngine::updateiSubPosition(UInt32 firstSampleFrame, UInt32 numSampleFrames)
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

inline void AppleLegacyDBDMAAudioDMAEngine::startTiming() {
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
		IOLog("clipOutputSamples[%ld]:\t%ld:", mCallCount, uptime.lo);
	}
	mPreviousUptime = tempTime;

	if ((mCallCount % kCallFrequency) == 0) {
		clock_get_uptime (&lastuptime);
	}	
#endif
}

inline void AppleLegacyDBDMAAudioDMAEngine::endTiming() {
#ifdef _TIME_CLIP_ROUTINE
	if ((mCallCount % kCallFrequency) == 0) {
		clock_get_uptime (&uptime);
		SUB_ABSOLUTETIME (&uptime, &lastuptime);
		absolutetime_to_nanoseconds (uptime, &nanos);
		IOLog("%ld\n", uptime.lo);
	}
#endif
}
