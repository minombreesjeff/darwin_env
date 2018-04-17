/*
 * Copyright (c) 2003-2004 Apple Computer, Inc. All Rights Reserved.
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

#ifndef _SECURITY_SECKEYCHAINITEMPRIV_H_
#define _SECURITY_SECKEYCHAINITEMPRIV_H_

#include <Security/SecBase.h>
#include <Security/SecKeychainItem.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* Private keychain item attributes */
enum 
{
	kSecClassItemAttr            = 'clas',                       /* Item class (KCItemClass) */
	kSecAliasItemAttr            = 'alis',                       /* Alias attribute (required by CDSA). */
	kSecProtectedDataItemAttr    = 'prot',                       /* Item's data is protected (encrypted) (Boolean) */
};

/* Temporary: CRL attributes */
enum 
{
	kSecCrlTypeItemAttr				 = 'crtp',
	kSecCrlEncodingItemAttr			 = 'cren',
	kSecThisUpdateItemAttr			 = 'crtu',
	kSecNextUpdateItemAttr			 = 'crnu',
	kSecUriItemAttr					 = 'curi',	// URI from which it came	
	kSecCrlNumberItemAttr			 = 'crnm',
	kSecDeltaCrlNumberItemAttr		 = 'dlcr'
};


OSStatus SecKeychainItemCreateNew(SecItemClass itemClass, OSType itemCreator, UInt32 length, const void* data, SecKeychainItemRef* itemRef);

OSStatus SecKeychainItemGetData(SecKeychainItemRef itemRef, UInt32 maxLength, void* data, UInt32* actualLength);

OSStatus SecKeychainItemGetAttribute(SecKeychainItemRef itemRef, SecKeychainAttribute* attribute, UInt32* actualLength);

OSStatus SecKeychainItemSetAttribute(SecKeychainItemRef itemRef, SecKeychainAttribute* attribute);

OSStatus SecKeychainItemAdd(SecKeychainItemRef itemRef);

OSStatus SecKeychainItemAddNoUI(SecKeychainRef keychainRef, SecKeychainItemRef itemRef);

OSStatus SecKeychainItemUpdate(SecKeychainItemRef itemRef);

OSStatus SecKeychainItemSetData(SecKeychainItemRef itemRef, UInt32 length, const void* data);

OSStatus SecKeychainItemFindFirst(SecKeychainRef keychainRef, const SecKeychainAttributeList *attrList, SecKeychainSearchRef *searchRef, SecKeychainItemRef *itemRef);

#if defined(__cplusplus)
}
#endif

#endif /* !_SECURITY_SECKEYCHAINITEMPRIV_H_ */
