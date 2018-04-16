/*
 * Copyright (c) 2004 Apple Computer, Inc. All Rights Reserved.
 * 
 * @APPLE_LICENSE_HEADER_START@
 * 
 * Copyright (c) 1999-2003 Apple Computer, Inc.  All Rights Reserved.
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */


//
// pcscmonitor - use PCSC to monitor smartcard reader/card state for securityd
//
// PCSCMonitor is the "glue" between PCSC and the securityd objects representing
// smartcard-related things. Its job is to translate real-world events into
// securityd-speak.
// Inputs are primarily security notifications sent from pcscd. We can also use
// MachServer-based timers to do things periodically or after some delay, though
// periodic actions are discouraged for the obvious reasons.
//
#include "pcscmonitor.h"


//
// Construct a PCSCMonitor.
// We strongly assume there's only one of us around here.
//
// Note that by construction, we *are* a notification Listener.
// The (unique index) port we use is zero (the null port), since we're
// not actually delivering the event anywhere (else).
//
PCSCMonitor::PCSCMonitor(Server &srv)
	: Listener(kNotificationDomainPCSC, SecurityServer::kNotificationAllEvents),
	  server(srv)
{
	// initial reader poll
	if (mSession.isOpen()) {
		pollReaders();
//		server.setTimer(this, Time::Interval(5));

		// setup complete
		secdebug("sc", "pcsc monitor initialized");
	}
}


PCSCMonitor::~PCSCMonitor()
{
}


//
// (Re)poll PCSC for smartcard status
//
void PCSCMonitor::pollReaders()
{
	// enumerate readers
	vector<string> names;  // will hold reader name C strings throughout
	mSession.listReaders(names);
	size_t count = names.size();
	secdebug("sc", "%ld reader(s) in system", count);
	
	// inquire into reader state
	vector<PCSC::ReaderState> states(count); // reader status array (PCSC style)
	for (unsigned int n = 0; n < count; n++) {
		PCSC::ReaderState &state = states[n];
		ReaderMap::iterator it = mReaders.find(names[n]);
		if (it == mReaders.end()) { // new reader
			state.clearPod();
			state.name(names[n].c_str());
			// lastKnown(PCSC_STATE_UNKNOWN)
			// userData<Reader>() = NULL
		} else {
			state = it->second->pcscState();
			state.name(names[n].c_str());  // OUR pointer
			state.lastKnown(state.state());
			state.userData<Reader>() = it->second;
		}
	}
	mSession.statusChange(states);
#if DEBUGDUMP
	if (Debug::dumping("sc"))
		for (unsigned int n = 0; n < count; n++)
			states[n].dump();
#endif

	// make set of previously known reader objects (to catch those who disappeared)
	set<Reader *> current;
	copy_second(mReaders.begin(), mReaders.end(), inserter(current, current.end()));
		
	// match state array against them
	for (unsigned int n = 0; n < count; n++) {
		PCSC::ReaderState &state = states[n];
		if (Reader *reader = state.userData<Reader>()) {
			// if PCSC flags a change, notify the Reader
			if (state.changed())
				reader->update(state);
			// accounted for this reader
			current.erase(reader);
		} else {
			RefPointer<Reader> newReader = new Reader(state);
			mReaders.insert(make_pair(state.name(), newReader));
		}
	}
	
	// now deal with dead readers
	for (set<Reader *>::iterator it = current.begin(); it != current.end(); it++) {
		secdebug("sc", "killing reader %s", (*it)->name().c_str());
		(*it)->kill();						// prepare to die
		mReaders.erase((*it)->name());		// remove from reader map
	}
}


//
// Event notifier.
// These events are sent by pcscd for our (sole) benefit.
//
void PCSCMonitor::notifyMe(SecurityServer::NotificationDomain domain,
		SecurityServer::NotificationEvent event, const CssmData &data)
{
	//@@@ need some kind of locking, of course...
	pollReaders();
	//server.setTimer(this, Time::Interval(0.5));
	//secdebug("adhoc", "pcsc change event");
}


//
// Timer action.
//
void PCSCMonitor::action()
{
	// not used at the moment; timer is not scheduled
}
