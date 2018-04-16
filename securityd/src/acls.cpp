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
// acls - SecurityServer ACL implementation
//
#include "acls.h"
#include "connection.h"
#include "server.h"
#include <security_agent_client/agentclient.h>
#include <security_cdsa_utilities/acl_any.h>
#include <security_cdsa_utilities/acl_password.h>
#include <security_cdsa_utilities/acl_threshold.h>


//
// SecurityServerAcl is virtual
//
SecurityServerAcl::~SecurityServerAcl()
{ }


//
// Each SecurityServerAcl type must provide some indication of a database
// it is associated with. The default, naturally, is "none".
//
const Database *SecurityServerAcl::relatedDatabase() const
{ return NULL; }


//
// Provide environmental information to get/change-ACL calls.
// Also make them virtual so our children can override them.
//
void SecurityServerAcl::cssmChangeAcl(const AclEdit &edit, const AccessCredentials *cred)
{
	SecurityServerEnvironment env(*this);
	ObjectAcl::cssmChangeAcl(edit, cred, &env);
}

void SecurityServerAcl::cssmChangeOwner(const AclOwnerPrototype &newOwner,
	const AccessCredentials *cred)
{
	SecurityServerEnvironment env(*this);
	ObjectAcl::cssmChangeOwner(newOwner, cred, &env);
}


//
// Modified validate() methods to connect all the conduits...
//
void SecurityServerAcl::validate(AclAuthorization auth, const AccessCredentials *cred)
{
    SecurityServerEnvironment env(*this);
	StLock<Mutex> objectSequence(aclSequence);
	StLock<Mutex> processSequence(Server::process().aclSequence);
    ObjectAcl::validate(auth, cred, &env);
}

void SecurityServerAcl::validate(AclAuthorization auth, const Context &context)
{
	validate(auth,
		context.get<AccessCredentials>(CSSM_ATTRIBUTE_ACCESS_CREDENTIALS));
}


//
// Implement our environment object
//
uid_t SecurityServerEnvironment::getuid() const
{
    return Server::process().uid();
}

gid_t SecurityServerEnvironment::getgid() const
{
    return Server::process().gid();
}

pid_t SecurityServerEnvironment::getpid() const
{
    return Server::process().pid();
}

bool SecurityServerEnvironment::verifyCodeSignature(const CodeSigning::Signature *signature,
	const CssmData *comment)
{
	return Server::codeSignatures().verify(Server::process(), signature, comment);
}
