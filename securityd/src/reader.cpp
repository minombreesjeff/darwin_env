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
// reader - token reader objects
//
#include "reader.h"


Reader::Reader(const PCSC::ReaderState &state)
	: mToken(NULL)
{
	mName = state.name();
	secdebug("reader", "%p (%s) new reader", this, name().c_str());
	transit(state);
}


Reader::~Reader()
{
	secdebug("reader", "%p (%s) destroyed", this, name().c_str());
}


void Reader::kill()
{
	if (mToken)
		removeToken();
	NodeCore::kill();
}


void Reader::update(const PCSC::ReaderState &state)
{
	transit(state);
}


//
// State transition matrix for a reader, based on PCSC state changes
//
void Reader::transit(const PCSC::ReaderState &state)
{
	if (state.state(SCARD_STATE_UNAVAILABLE)) {
		// reader is unusable (probably being removed)
		secdebug("reader", "%p (%s) unavailable (0x%lx)",
			this, name().c_str(), state.state());
		if (mToken)
			removeToken();
	} else if (state.state(SCARD_STATE_EMPTY)) {
		// reader is empty (no token present)
		secdebug("reader", "%p (%s) empty (0x%lx)",
			this, name().c_str(), state.state());
		if (mToken)
			removeToken();
	} else if (state.state(SCARD_STATE_PRESENT)) {
		// reader has a token inserted
		secdebug("reader", "%p (%s) card present (0x%lx)",
			this, name().c_str(), state.state());
		//@@@ is this hack worth it (with notifications in)??
		if (mToken && CssmData(state) != CssmData(pcscState()))
			removeToken();  // incomplete but better than nothing
		//@@@ or should we call some verify-still-the-same function of tokend?
		if (!mToken)
			insertToken();
	} else {
		secdebug("reader", "%p (%s) unexpected state change (0x%ld to 0x%lx)",
			this, name().c_str(), mState.state(), state.state());
	}
	mState = state;
}


void Reader::insertToken()
{
	RefPointer<Token> token = new Token();
	token->insert(*this);
	mToken = token;
	addReference(*token);
	secdebug("reader", "%p (%s) inserted token %p",
		this, name().c_str(), mToken);
}


void Reader::removeToken()
{
	secdebug("reader", "%p (%s) removing token %p",
		this, name().c_str(), mToken);
	assert(mToken);
	mToken->remove();
	removeReference(*mToken);
	mToken = NULL;
}


//
// Debug dump support
//
#if defined(DEBUGDUMP)

void Reader::dumpNode()
{
	PerGlobal::dumpNode();
	Debug::dump(" [%s] state=0x%lx", name().c_str(), mState.state());
}

#endif //DEBUGDUMP
