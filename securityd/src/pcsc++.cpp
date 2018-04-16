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
// pcsc++ - PCSC client interface layer in C++
//
#include "pcsc++.h"
#include <security_utilities/debugging.h>


namespace Security {
namespace PCSC {


//
// Internal utilities
//
static void decode(vector<string> &names, const char *buffer, size_t size)
{
	names.clear();
	for (size_t pos = 0; pos < size - 1; ) {
		size_t len = strlen(buffer + pos);
		names.push_back(string(buffer + pos, len));
		pos += len + 1;
	}
}

inline void decode(vector<string> &names, const vector<char> &buffer, size_t size)
{
	decode(names, &buffer[0], size);
}


//
// PCSC domain errors
//
Error::Error(long err) : error(err)
{
	IFDEBUG(debugDiagnose(this));
}


const char *Error::what() const throw ()
{
	return pcsc_stringify_error(error);
}


void Error::throwMe(long err)
{
	throw Error(err);
}


OSStatus Error::osStatus() const
{
	return -1;	//@@@ preliminary
}

int Error::unixError() const
{
	return EINVAL;  //@@@ preliminary
}

#if !defined(NDEBUG)
void Error::debugDiagnose(const void *id) const
{
    secdebug("exception", "%p PCSC::Error %s (%ld) osStatus %ld",
		id, pcsc_stringify_error(error), error, osStatus());
}
#endif //NDEBUG


//
// PodWrappers
//
void ReaderState::set(const char *name, unsigned long known)
{
	clearPod();
	szReader = name;
	pvUserData = NULL;
	dwCurrentState = known;
}


void ReaderState::lastKnown(unsigned long s)
{
	// clear out CHANGED and UNAVAILABLE
	dwCurrentState = s & ~(SCARD_STATE_CHANGED | SCARD_STATE_UNAVAILABLE);
}


#if defined(DEBUGDUMP)

void ReaderState::dump()
{
	Debug::dump("reader(%s) state=0x%lx events=0x%lx",
		szReader ? szReader : "(null)", dwCurrentState, dwEventState);
	Debug::dumpData(" ATR", rgbAtr, cbAtr);
}

#endif //DEBUGDUMP


//
// Session objects
//
Session::Session()
	: mIsOpen(false)
{
	open();
}


Session::~Session()
{
	if (mIsOpen)
		Error::check(SCardReleaseContext(mContext));
}


//
// (Re)establish PCSC context.
// Don't fail on SCARD_F_INTERNAL_ERROR (pcscd not running).
void Session::open()
{
	if (!mIsOpen) {
		try {
			Error::check(::SCardEstablishContext(SCARD_SCOPE_SYSTEM, NULL, NULL, &mContext));
			mIsOpen = true;
			secdebug("pcsc", "context opened");
		} catch (const Error &err) {
			if (err.error == long(SCARD_F_INTERNAL_ERROR)) {
				secdebug("pcsc", "context not opened");
				return;
			}
		}
	}
}


bool Session::check(long rc)
{
	switch (rc) {
	case SCARD_S_SUCCESS:
		return true;	// got reader(s), call succeeded
	case SCARD_E_READER_UNAVAILABLE:
		return false;   // no readers, but don't treat as error
	default:
		Error::throwMe(rc);
		return false;   // placebo
	}
}


void Session::listReaders(vector<string> &readers, const char *groups)
{
	mReaderBuffer.resize(1000); //@@@ preliminary hack
	unsigned long size = mReaderBuffer.size();
	if (check(::SCardListReaders(mContext, groups, &mReaderBuffer[0], &size)))
		decode(readers, mReaderBuffer, size);
	else
		readers.clear();	// treat as success (returning zero readers)
}


//
// Reader status check
//
void Session::statusChange(ReaderState *readers, unsigned int nReaders, long timeout)
{
	if (nReaders == 0)
		return; // no readers, no foul
	check(::SCardGetStatusChange(mContext, timeout, readers, nReaders));
}


//
// PCSC Card objects
//
Card::Card()
	: mIsOpen(false)
{
}

Card::~Card()
{
}

void Card::open(Session &session, const char *reader,
	unsigned long share, unsigned long protocols)
{
	Error::check(::SCardConnect(session.mContext,
		reader, share, protocols, &mHandle, &mProtocol));
	mIsOpen = true;
}

void Card::close(unsigned long disposition)
{
	if (mIsOpen) {
		Error::check(::SCardDisconnect(mHandle, disposition));
		mIsOpen = false;
	}
}


}   // namespace PCSC
}   // namespace Security
