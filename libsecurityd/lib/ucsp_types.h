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
// ucsp_types - type equivalence declarations for SecurityServer MIG
//
#include "ssclient.h"

// who forgot that one?
extern "C" kern_return_t mig_deallocate(vm_address_t addr, vm_size_t size);

namespace Security {

using namespace SecurityServer;


typedef void *Data;
typedef void *Pointer;
typedef void *BasePointer;

typedef const char *CssmString;

typedef void *ContextAttributes;
typedef Context::Attr *ContextAttributesPointer;

typedef AclEntryPrototype *AclEntryPrototypePtr;
typedef AclEntryInput *AclEntryInputPtr;
typedef AclEntryInfo *AclEntryInfoPtr;
typedef AclOwnerPrototype *AclOwnerPrototypePtr;
typedef AccessCredentials *AccessCredentialsPtr;
typedef void *ParamInputPtr;

typedef CssmDbRecordAttributeData *CssmDbRecordAttributeDataPtr;
typedef CssmNetAddress *CssmNetAddressPtr;
typedef CssmQuery *CssmQueryPtr;
typedef CssmSubserviceUid *CssmSubserviceUidPtr;
typedef CSSM_DBINFO *CSSM_DBINFOPtr;
typedef CSSM_DB_SCHEMA_ATTRIBUTE_INFO *CSSM_DB_SCHEMA_ATTRIBUTE_INFOPtr;
typedef CSSM_DB_SCHEMA_INDEX_INFO *CSSM_DB_SCHEMA_INDEX_INFOPtr;
typedef CSSM_NAME_LIST *CSSM_NAME_LISTPtr;
typedef void *VoidPtr;

typedef DataWalkers::DLDbFlatIdentifier DLDbIdentBlob;
typedef DataWalkers::DLDbFlatIdentifier *DLDbIdentPtr;

typedef AuthorizationItemSet AuthorizationItemSetBlob;
typedef AuthorizationItemSet *AuthorizationItemSetPtr;
typedef void *AuthorizationHandle;

typedef CssmKey::Header CssmKeyHeader;

typedef const char *ExecutablePath;
typedef const char *RelationName;

inline Context &inTrans(CSSM_CONTEXT &arg) { return Context::overlay(arg); }
inline CssmKey &inTrans(CSSM_KEY &arg) { return CssmKey::overlay(arg); }
inline CSSM_KEY &outTrans(CssmKey &key) { return key; }


//
// Customization macros for MIG code
//
#define __AfterSendRpc(id, name) \
	if (msg_result == MACH_MSG_SUCCESS && Out0P->Head.msgh_id == MACH_NOTIFY_DEAD_NAME) \
		return MIG_SERVER_DIED;

#define UseStaticTemplates 0


//
// MIG-used byte swapping macros
//
#define __NDR_convert__int_rep__BasePointer__defined
#define __NDR_convert__int_rep__BasePointer(a, f)	/* do not flip */

} // end namespace Security
