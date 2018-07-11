/*
 *	PlatformInterface.h
 *
 *	Defines an interface for IO controllers (eg. Key Largo)
 *
 *  Created by Aram Lindahl on Mon Mar 10 2003.
 *  Copyright (c) 2003 AppleComputer. All rights reserved.
 */
#pragma once

#include <IOKit/IOService.h>
#include <IOKit/IOInterruptEventSource.h>
#include "AudioHardwareConstants.h"
#include <IOKit/ppc/IODBDMA.h>

class AppleOnboardAudio;

typedef enum {
	kI2C_StandardMode 			= 0,
	kI2C_StandardSubMode		= 1,
	kI2C_CombinedMode			= 2
} BusMode;

typedef enum {
	kI2S_18MHz 					= 0,
	kI2S_45MHz					= 1,
	kI2S_49MHz					= 2
} I2SClockFrequency;

typedef enum {
	kGPIO_Disconnected			= 0,
	kGPIO_Connected,
	kGPIO_Unknown,
	kGPIO_Muted,
	kGPIO_Unmuted,
	kGPIO_Reset,
	kGPIO_Run,
	kGPIO_MuxSelectDefault,
	kGPIO_MuxSelectAlternate,
	kGPIO_CodecInterruptActive,
	kGPIO_CodecInterruptInactive,
	kGPIO_CodecIRQEnable,
	kGPIO_CodecIRQDisable,
	kGPIO_TypeIsAnalog,
	kGPIO_TypeIsDigital
} GpioAttributes;

typedef enum {
	kCODEC_RESET_Analog			= 0,
	kCODEC_RESET_Digital
} CODEC_RESET;

typedef enum {
	kUnknownInterrupt			= 0,
	kHeadphoneDetectInterrupt,
	kSpeakerDetectInterrupt,
	kCodecInterrupt,
	kCodecErrorDetectInterrupt,
	kLineInputDetectInterrupt,
	kLineOutputDetectInterrupt,
	kDigitalInDetectInterrupt,
	kDigitalOutDetectInterrupt
} PlatformInterruptSource;

class PlatformInterface : public OSObject {

    OSDeclareDefaultStructors(PlatformInterface);

public:	

	virtual bool			init(IOService* device, AppleOnboardAudio* provider, UInt32 inDBDMADeviceIndex);
	virtual void			free (void);
	virtual bool			registerInterrupts ( IOService * device );
	
	virtual	void			setWorkLoop(IOWorkLoop* inWorkLoop) {return;}					
	//
	// Codec Methods
	//
	virtual bool			readCodecRegister(UInt8 address, UInt8 subAddress, UInt8 *data, UInt16 len, BusMode mode) {return false;}
	virtual bool			writeCodecRegister(UInt8 address, UInt8 subAddress, UInt8 *data, UInt16 len, BusMode mode) {return false;}

	virtual IOReturn		setCodecReset ( CODEC_RESET target, GpioAttributes reset ) { return kIOReturnError; }
	virtual GpioAttributes	getCodecReset ( CODEC_RESET target ) { return kGPIO_Unknown; }
	//
	// I2S Methods: FCR3
	//
	virtual IOReturn		requestI2SClockSource(I2SClockFrequency inFrequency) {return kIOReturnError;}
	virtual IOReturn		releaseI2SClockSource(I2SClockFrequency inFrequency) {return kIOReturnError;}
	//
	// I2S Methods: FCR1
	//
	virtual IOReturn		setI2SEnable(bool enable) {return kIOReturnError;}
	virtual bool			getI2SEnable() {return false;}

	virtual IOReturn		setI2SClockEnable(bool enable) {return kIOReturnError;}
	virtual bool			getI2SClockEnable() {return false;}

	virtual IOReturn		setI2SCellEnable(bool enable) {return kIOReturnError;}
	virtual bool			getI2SCellEnable() {return false;}
	
	virtual IOReturn		setI2SSWReset(bool enable) {return kIOReturnError;}
	virtual bool			getI2SSWReset() {return false;}
	//
	// I2S Methods: IOM Control
	//
	virtual IOReturn		setSerialFormatRegister(UInt32 serialFormat) {return kIOReturnError;}
	virtual UInt32			getSerialFormatRegister() {return 0;}

	virtual IOReturn		setDataWordSizes(UInt32 dataWordSizes) {return kIOReturnError;}
	virtual UInt32			getDataWordSizes() {return 0;}
	
	virtual IOReturn		setFrameCount(UInt32 value) {return kIOReturnError;}
	virtual UInt32			getFrameCount() {return 0;}

	virtual IOReturn		setI2SIOMIntControl(UInt32 intCntrl) {return kIOReturnError;}
	virtual UInt32			getI2SIOMIntControl() {return 0;}
	//
	// GPIO Methods
	//
	virtual GpioAttributes	getHeadphoneConnected() {return kGPIO_Unknown;}
	virtual GpioAttributes	getSpeakerConnected() {return kGPIO_Unknown;}
	virtual	GpioAttributes	getLineOutConnected() {return kGPIO_Unknown;}
	virtual	GpioAttributes	getLineInConnected() {return kGPIO_Unknown;}
	virtual	GpioAttributes	getDigitalOutConnected() {return kGPIO_Unknown;}
	virtual	GpioAttributes	getDigitalOutTypeConnected() {return kGPIO_Unknown;}		//	for combo digital/analog connector
	virtual	GpioAttributes	getDigitalInConnected() {return kGPIO_Unknown;}
	virtual	GpioAttributes	getDigitalInTypeConnected() {return kGPIO_Unknown;}			//	for combo digital/analog connector
	virtual GpioAttributes	getCodecInterrupt() {return kGPIO_Unknown;}
	virtual GpioAttributes	getCodecErrorInterrupt() {return kGPIO_Unknown;}

	virtual IOReturn 		setHeadphoneMuteState( GpioAttributes muteState ) {return kIOReturnError;}
	virtual GpioAttributes 	getHeadphoneMuteState() {return kGPIO_Unknown;}
	
	virtual IOReturn 		setLineOutMuteState( GpioAttributes muteState ) {return kIOReturnError;}
	virtual GpioAttributes 	getLineOutMuteState() {return kGPIO_Unknown;}
	
	virtual IOReturn 		setSpeakerMuteState( GpioAttributes muteState ) {return kIOReturnError;}
	virtual GpioAttributes 	getSpeakerMuteState() {return kGPIO_Unknown;}
	
	virtual IOReturn		setClockMux(GpioAttributes muxState) { return kIOReturnError; }
	virtual GpioAttributes	getClockMux() { return kGPIO_Unknown; }

	virtual IOReturn		setInputDataMux(GpioAttributes muxState) { return kIOReturnError; }
	virtual GpioAttributes	getInputDataMux() { return kGPIO_Unknown; }

	virtual bool	 		getInternalSpeakerID() {return false;}

	static void				headphoneDetectInterruptHandler ( OSObject *owner, IOInterruptEventSource *source, UInt32 count, void * arg4 );
	static void				speakerDetectInterruptHandler ( OSObject *owner, IOInterruptEventSource *source, UInt32 count, void * arg4 );
	static void				lineInDetectInterruptHandler ( OSObject *owner, IOInterruptEventSource *source, UInt32 count, void * arg4 );
	static void				lineOutDetectInterruptHandler ( OSObject *owner, IOInterruptEventSource *source, UInt32 count, void * arg4 );
	static void				digitalInDetectInterruptHandler ( OSObject *owner, IOInterruptEventSource *source, UInt32 count, void * arg4 );
	static void				digitalOutDetectInterruptHandler ( OSObject *owner, IOInterruptEventSource *source, UInt32 count, void * arg4 );
	static void				codecInterruptHandler ( OSObject *owner, IOInterruptEventSource *source, UInt32 count, void * arg4 );
	static void				codecErrorInterruptHandler ( OSObject *owner, IOInterruptEventSource *source, UInt32 count, void * arg4 );

	virtual  void 			logFCR1() {return;}
	virtual  void 			logFCR3() {return;}

	//
	// Set Interrupt Handler Methods
	//
	virtual  IOReturn		registerInterruptHandler (IOService * theDevice, void * interruptHandler, PlatformInterruptSource source ) { return kIOReturnError; }
	virtual  IOReturn		unregisterInterruptHandler (IOService * theDevice, void * interruptHandler, PlatformInterruptSource source ) { return kIOReturnError; }

	//
	// DBDMA Memory Address Acquisition Methods
	//
	virtual	IODBDMAChannelRegisters *	GetInputChannelRegistersVirtualAddress ( IOService * dbdmaProvider ) { return NULL; };
	virtual	IODBDMAChannelRegisters *	GetOutputChannelRegistersVirtualAddress ( IOService * dbdmaProvider ) { return NULL; };
	virtual void						LogDBDMAChannelRegisters ( void ) { return; }

protected:

	AppleOnboardAudio *		mProvider;

private:
	bool					mIsComboInJack;
	bool					mIsComboOutJack;
	GpioAttributes			mComboInJackState;
	GpioAttributes			mComboOutJackState;
};

