/*
 * Copyright (c) 2000-2004 Apple Computer, Inc. All Rights Reserved.
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
// sstransit - SecurityServer client library transition code.
//
// These are the functions that implement CssmClient methods in terms of
// MIG IPC client calls, plus their supporting machinery.
//
// WARNING! HERE BE DRAGONS!
// This code involves moderately arcane magic including (but not limited to)
// dancing macros paired off with self-maintaining stack objects. Don't take
// anything for granted! Be very afraid of ALL-CAPS names. Your best bet is
// probably to stick with the existing patterns.
//
#ifndef _H_SSTRANSIT
#define _H_SSTRANSIT

#include <securityd_client/ssclient.h>
#include <security_utilities/mach++.h>
#include <security_cdsa_utilities/cssmwalkers.h>
#include <security_cdsa_utilities/AuthorizationWalkers.h>
#include <securityd_client/ucsp.h>

namespace Security
{

// stock leading argument profile used by all calls
#define UCSP_ARGS	mGlobal().serverPort, mGlobal().thread().replyPort, &rcode

// IPC/IPCN wrap the actual Mach IPC call. IPC also activates the connection first
#define IPCN(statement) \
	{ CSSM_RETURN rcode; check(statement); if (rcode != CSSM_OK) CssmError::throwMe(rcode); }
#define IPC(statement)	{ activate(); IPCN(statement); }
#define IPCKEY(statement, key, tag) \
{ \
	activate(); \
	CSSM_RETURN rcode; \
	for (bool retried = false;; retried = true) \
	{ \
		check(statement); \
		if (retried || rcode != CSSMERR_CSP_APPLE_ADD_APPLICATION_ACL_SUBJECT) \
			break; \
		addApplicationAclSubject(key, tag); \
	} \
	if (rcode != CSSM_OK) \
		CssmError::throwMe(rcode); \
}

// pass mandatory or optional CssmData arguments into an IPC call
#define DATA(arg)			arg.data(), arg.length()
#define OPTIONALDATA(arg)	(arg ? arg->data() : NULL), (arg ? arg->length() : 0)

// pass structured arguments in/out of IPC calls. See "data walkers" for details
#define COPY(copy)			copy, copy.length(), copy
#define COPY_OUT(copy)		&copy, &copy##Length, &copy##Base
#define COPY_OUT_DECL(type,name) type *name, *name##Base; mach_msg_type_number_t name##Length


//
// DataOutput manages an output CssmData argument.
//
class DataOutput {
public:
	DataOutput(CssmData &arg, Allocator &alloc)
		: argument(arg), allocator(alloc) { mData = NULL; mLength = 0; }
	~DataOutput();
	
	void **data() { return &mData; }
	mach_msg_type_number_t *length() { return &mLength; }
	
	CssmData &argument;
	Allocator &allocator;

private:
	void *mData;
	mach_msg_type_number_t mLength;
};


//
// Bundle up an AccessCredentials meant for a database, parsing it for
// "special" samples that need extra evidence to be passed along.
//
class DatabaseAccessCredentials : public Copier<AccessCredentials> {
public:
	DatabaseAccessCredentials(const AccessCredentials *creds, Allocator &alloc);

private:
	void mapKeySample(CSSM_CSP_HANDLE &cspHandle, CssmKey &key);
};


//
// Bundle up a Context for IPC transmission
//
class SendContext {
public:
	SendContext(const Context &ctx);
	~SendContext() { Allocator::standard().free(attributes); }
	
	const Context &context;
	CSSM_CONTEXT_ATTRIBUTE *attributes;
	size_t attributeSize;
};

#define CONTEXT(ctx)	ctx.context, ctx.attributes, ctx.attributes, ctx.attributeSize

} // end namespace Security

#endif //_H_SSTRANSIT
