/*
 * Copyright (c) 1998-2000 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 *
 * The contents of this file constitute Original Code as defined in and
 * are subject to the Apple Public Source License Version 1.1 (the
 * "License").  You may not use this file except in compliance with the
 * License.  Please obtain a copy of the License at
 * http://www.apple.com/publicsource and read it before using this file.
 *
 * This Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 *
 * @APPLE_LICENSE_HEADER_END@
 */
#include "IOFireWireIP.h"
#include "IOFireWireIPDiagnostics.h"

OSDefineMetaClassAndStructors(IOFireWireIPDiagnostics, OSObject)

OSObject * IOFireWireIPDiagnostics::createDiagnostics( IOFireWireIP* obj )
{
	IOFireWireIPDiagnostics *	diagnostics;
	
	diagnostics = new IOFireWireIPDiagnostics;
	if( diagnostics && !diagnostics->init() )
	{
		diagnostics->release();
		diagnostics = NULL;
	}
	
	diagnostics->fIPObj		= obj;
	
	obj->fTxBcast			= 0;
	obj->fRxBcast			= 0;
	obj->fTxUni				= 0;
	obj->fRxUni				= 0;
	obj->fRxFragmentPkts	= 0;
	obj->fTxFragmentPkts	= 0;
	obj->fActiveBcastCmds	= 0;
	obj->fInActiveBcastCmds	= 0;
	obj->fActiveCmds		= 0;
	obj->fInActiveCmds		= 0;
	obj->fNoCommands		= 0;
	obj->fNoBCastCommands	= 0;
	obj->fDoubleCompletes	= 0;
	obj->fSubmitErrs		= 0;
	obj->fCallErrs			= 0;
	obj->fNoResources		= 0;
	obj->fMaxQueueSize		= 0;
	obj->fServiceInOutput	= 0;
	obj->fServiceInCallback	= 0;
	obj->fLastStarted		= 0;
	obj->fMaxPacketSize		= 0;
	obj->inActiveMbufs		= 0;
	obj->activeMbufs		= 0;	
	obj->fNoMbufs			= 0;	
	obj->fGaspTagError		= 0;
	obj->fGaspHeaderError	= 0;
	obj->fNonRFC2734Gasp	= 0;
	obj->fRemoteGaspError	= 0;
	obj->fBusyAcks			= 0;
	obj->fFastRetryBusyAcks	= 0;
	
	obj->fEncapsulationHeaderError	= 0;	
	
	return diagnostics;
}

bool IOFireWireIPDiagnostics::serialize( OSSerialize * s ) const
{
	OSDictionary *	dictionary;
	bool			ok;
	
	dictionary = OSDictionary::withCapacity( 4 );
	if( !dictionary )
		return false;
		
	/////////
	updateNumberEntry( dictionary, fIPObj->fTxBcast, "TxB");
	updateNumberEntry( dictionary, fIPObj->fRxBcast, "RxB");
	updateNumberEntry( dictionary, fIPObj->fTxUni, "TxU");
	updateNumberEntry( dictionary, fIPObj->fRxUni, "RxU");
	updateNumberEntry( dictionary, fIPObj->fRxFragmentPkts, "RxF");
	updateNumberEntry( dictionary, fIPObj->fTxFragmentPkts, "TxF");

    updateNumberEntry( dictionary, fIPObj->transmitQueue->getState(), "tqState");
    updateNumberEntry( dictionary, fIPObj->transmitQueue->getStallCount(), "tqStall");
    updateNumberEntry( dictionary, fIPObj->transmitQueue->getRetryCount(), "tqRetries");
    updateNumberEntry( dictionary, fIPObj->transmitQueue->getSize(), "tqSize");

	updateNumberEntry( dictionary, fIPObj->fActiveBcastCmds, "fwActiveBCastCmds" );
	updateNumberEntry( dictionary, fIPObj->fInActiveBcastCmds, "fwInActiveBCastCmds" );

	updateNumberEntry( dictionary, fIPObj->fActiveCmds, "fwActiveCmds" );
	updateNumberEntry( dictionary, fIPObj->fNoCommands, "fwNoCommands" );
	updateNumberEntry( dictionary, fIPObj->fNoBCastCommands, "fwNoBcastCommands" );
	updateNumberEntry( dictionary, fIPObj->fInActiveCmds, "fwInActiveCmds" );
	updateNumberEntry( dictionary, fIPObj->fDoubleCompletes, "fwAttemptedDC" );
	updateNumberEntry( dictionary, fIPObj->fSubmitErrs, "fwSubmitErrs" );
	updateNumberEntry( dictionary, fIPObj->fCallErrs, "fwCompletionErrs" );
	updateNumberEntry( dictionary, fIPObj->fNoResources, "fwIPNoResources");
	updateNumberEntry( dictionary, fIPObj->fMaxQueueSize, "fwMaxQueueSize");
	updateNumberEntry( dictionary, fIPObj->fServiceInOutput, "fwServiceInOP");
	updateNumberEntry( dictionary, fIPObj->fServiceInCallback, "fwServiceInCB");
	updateNumberEntry( dictionary, fIPObj->fLastStarted, "fwLastStarted");
	updateNumberEntry( dictionary, fIPObj->fMaxPacketSize, "fwMaxPacketSize");
	
	updateNumberEntry( dictionary, fIPObj->fGaspTagError, "fwGASPTagError");
	updateNumberEntry( dictionary, fIPObj->fGaspHeaderError, "fwGASPHeaderError");
	updateNumberEntry( dictionary, fIPObj->fNonRFC2734Gasp, "fwNonRFC2734Error");
	updateNumberEntry( dictionary, fIPObj->fRemoteGaspError, "fwRemoteGaspError");
	updateNumberEntry( dictionary, fIPObj->fEncapsulationHeaderError, "fwRxBHeaderError");	

	updateNumberEntry( dictionary, fIPObj->inActiveMbufs, "fwInActiveMbufs");
	updateNumberEntry( dictionary, fIPObj->activeMbufs, "fwActiveMbufs");	
	updateNumberEntry( dictionary, fIPObj->fNoMbufs, "fwNoMbufs");	
	updateNumberEntry( dictionary, fIPObj->fBusyAcks, "fwBusyAcks");	
	updateNumberEntry( dictionary, fIPObj->fFastRetryBusyAcks, "fwFastRetryBusyAcks");	
	updateNumberEntry( dictionary, fIPObj->fDoFastRetry, "fwFastRetryOn");	
	
	
#ifdef IPFIREWIRE_DIAGNOSTICS
	fIPObj->fDumpLog = true;
   	updateNumberEntry( dictionary, fIPObj->fMaxInputCount, "MaxInputCount");
	updateNumberEntry( dictionary, fIPObj->fMaxPktSize, "MaxPktSize");
	updateNumberEntry( dictionary, fIPObj->fLcb->maxBroadcastPayload, "maxBroadcastPayload");
	updateNumberEntry( dictionary, fIPObj->fLcb->maxBroadcastSpeed, "currBroadcastSpeed");
	updateNumberEntry( dictionary, fIPObj->fPrevBroadcastSpeed, "prevBroadcastSpeed");
#endif	
	/////////
	
	ok = dictionary->serialize(s);
	dictionary->release();
	
	return ok;
}
	
void IOFireWireIPDiagnostics::updateNumberEntry( OSDictionary * dictionary, UInt32 value, const char * name )
{
	OSNumber *	number;
	
	number = OSNumber::withNumber( value, 32 );
	if( !number )
		return;
		
	dictionary->setObject( name, number );
	number->release();
}

///////////////////////////

void IOFireWireIPDiagnostics::incrementExecutedORBCount( void )
{
	fExecutedORBCount++;
}


