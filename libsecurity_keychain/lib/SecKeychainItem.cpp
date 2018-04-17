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

#include <Security/SecKeychainItem.h>
#include <Security/SecKeychainItemPriv.h>

#include <security_keychain/Keychains.h>
#include <security_keychain/KeyItem.h>
#include <security_keychain/Item.h>
#include <security_keychain/KCCursor.h> // @@@ Remove this when SecKeychainItemFindFirst moves to SecKeychainSearch

#include "SecBridge.h"
#include "KCExceptions.h"
#include "Access.h"


//
// Given a polymorphic Sec type object, return
// its AclBearer component.
// Note: Login ACLs are not hooked into this layer;
// modules or attachments have no Sec* layer representation.
//
RefPointer<AclBearer> aclBearer(CFTypeRef itemRef)
{
	// well, exactly what kind of something are you?
	CFTypeID id = CFGetTypeID(itemRef);
	if (id == gTypes().ItemImpl.typeID) {
		// keychain item. If it's in a protected group, return the group key
		if (SSGroup group = ItemImpl::required(SecKeychainItemRef(itemRef))->group())
			return &*group;
	} else if (id == gTypes().KeyItem.typeID) {
		// key item, return the key itself.
		if (CssmClient::Key key = KeyItem::required(SecKeyRef(itemRef))->key())
			return &*key;
	} else if (id == gTypes().KeychainImpl.typeID) {
		// keychain (this yields the database ACL)
		//@@@ not hooked up yet
	}
	// Guess not. Bummer
	MacOSError::throwMe(errSecNoAccessForItem);
}


CFTypeID
SecKeychainItemGetTypeID(void)
{
	BEGIN_SECAPI

	secdebug("kcitem", "SecKeychainItemGetTypeID()");
	return gTypes().ItemImpl.typeID;

	END_SECAPI1(_kCFRuntimeNotATypeID)
}


OSStatus
SecKeychainItemCreateFromContent(SecItemClass itemClass, SecKeychainAttributeList *attrList,
		UInt32 length, const void *data, SecKeychainRef keychainRef,
		SecAccessRef initialAccess, SecKeychainItemRef *itemRef)
{
    BEGIN_SECAPI
		secdebug("kcitem", "SecKeychainItemCreateFromContent(%lu, %p, %lu, %p, %p, %p)",
			itemClass, attrList, length, data, keychainRef, initialAccess);
		KCThrowParamErrIf_(length!=0 && data==NULL);
        Item item(itemClass, attrList, length, data);
		if (initialAccess)
			item->setAccess(Access::required(initialAccess));

        Keychain keychain = nil;
        try
        {
            keychain = Keychain::optional(keychainRef);
            if ( !keychain->exists() )
            {
                MacOSError::throwMe(errSecNoSuchKeychain);	// Might be deleted or not available at this time.
            }
        }
        catch(...)
        {
            keychain = globals().storageManager.defaultKeychainUI(item);
        }

        keychain->add(item);
        if (itemRef)
        	*itemRef = item->handle();
	END_SECAPI
}


OSStatus
SecKeychainItemModifyContent(SecKeychainItemRef itemRef, const SecKeychainAttributeList *attrList, UInt32 length, const void *data)
{
    BEGIN_SECAPI
		secdebug("kcitem", "SecKeychainItemModifyContent(%p, %p, %lu, %p)", itemRef, attrList, length, data);
		Item item = ItemImpl::required(itemRef);
		item->modifyContent(attrList, length, data);
	END_SECAPI
}


OSStatus
SecKeychainItemCopyContent(SecKeychainItemRef itemRef, SecItemClass *itemClass, SecKeychainAttributeList *attrList, UInt32 *length, void **outData)
{
	BEGIN_SECAPI
		secdebug("kcitem", "SecKeychainItemCopyContent(%p, %p, %p, %p, %p)",
			itemRef, itemClass, attrList, length, outData);
		Item item = ItemImpl::required(itemRef);
		item->getContent(itemClass, attrList, length, outData);
	END_SECAPI
}


OSStatus
SecKeychainItemFreeContent(SecKeychainAttributeList *attrList, void *data)
{
	BEGIN_SECAPI
		secdebug("kcitem", "SecKeychainItemFreeContent(%p, %p)", attrList, data);
		ItemImpl::freeContent(attrList, data);
	END_SECAPI
}


OSStatus
SecKeychainItemModifyAttributesAndData(SecKeychainItemRef itemRef, const SecKeychainAttributeList *attrList, UInt32 length, const void *data)
{
    BEGIN_SECAPI
		secdebug("kcitem", "SecKeychainItemModifyAttributesAndData(%p, %p, %lu, %p)", itemRef, attrList, length, data);
		Item item = ItemImpl::required(itemRef);
		item->modifyAttributesAndData(attrList, length, data);
	END_SECAPI
}


OSStatus
SecKeychainItemCopyAttributesAndData(SecKeychainItemRef itemRef, SecKeychainAttributeInfo *info, SecItemClass *itemClass, SecKeychainAttributeList **attrList, UInt32 *length, void **outData)
{
	BEGIN_SECAPI
		secdebug("kcitem", "SecKeychainItemCopyAttributesAndData(%p, %p, %p, %p, %p, %p)", itemRef, info, itemClass, attrList, length, outData);
		Item item = ItemImpl::required(itemRef);
		item->getAttributesAndData(info, itemClass, attrList, length, outData);
	END_SECAPI
}


OSStatus
SecKeychainItemFreeAttributesAndData(SecKeychainAttributeList *attrList, void *data)
{
	BEGIN_SECAPI
		secdebug("kcitem", "SecKeychainItemFreeAttributesAndData(%p, %p)", attrList, data);
		ItemImpl::freeAttributesAndData(attrList, data);
	END_SECAPI
}


OSStatus
SecKeychainItemDelete(SecKeychainItemRef itemRef)
{
    BEGIN_SECAPI
		secdebug("kcitem", "SecKeychainItemFreeAttributesAndData(%p)", itemRef);
		Item item = ItemImpl::required( itemRef );
		Keychain keychain = item->keychain();
		KCThrowIf_( !keychain, errSecInvalidItemRef );
		
        keychain->deleteItem( item ); // item must be persistant.
	END_SECAPI
}


OSStatus
SecKeychainItemCopyKeychain(SecKeychainItemRef itemRef, SecKeychainRef* keychainRef)
{
    BEGIN_SECAPI
		secdebug("kcitem", "SecKeychainItemCopyKeychain(%p, %p)", itemRef, keychainRef);
		Required(keychainRef) = ItemImpl::required(itemRef)->keychain()->handle();
	END_SECAPI
}


OSStatus
SecKeychainItemCreateCopy(SecKeychainItemRef itemRef, SecKeychainRef destKeychainRef,
	SecAccessRef initialAccess, SecKeychainItemRef *itemCopy)
{
    BEGIN_SECAPI
		secdebug("kcitem", "SecKeychainItemCreateCopy(%p, %p, %p, %p)",
			itemRef, destKeychainRef, initialAccess, itemCopy);

		Item copy = ItemImpl::required(itemRef)->copyTo(Keychain::optional(destKeychainRef), Access::optional(initialAccess));
		if (itemCopy)
			*itemCopy = copy->handle();
	END_SECAPI
}


OSStatus
SecKeychainItemGetUniqueRecordID(SecKeychainItemRef itemRef, const CSSM_DB_UNIQUE_RECORD **uniqueRecordID)
{
    BEGIN_SECAPI
		secdebug("kcitem", "SecKeychainItemGetUniqueRecordID(%p, %p)", itemRef, uniqueRecordID);
        Required(uniqueRecordID) = ItemImpl::required(itemRef)->dbUniqueRecord();
	END_SECAPI
}


OSStatus
SecKeychainItemGetDLDBHandle(SecKeychainItemRef itemRef, CSSM_DL_DB_HANDLE* dldbHandle)
{
    BEGIN_SECAPI
		secdebug("kcitem", "SecKeychainItemGetDLDBHandle(%p, %p)", itemRef, dldbHandle);
        *dldbHandle = ItemImpl::required(itemRef)->keychain()->database()->handle();
	END_SECAPI
}


OSStatus SecAccessCreateFromObject(CFTypeRef sourceRef,
	SecAccessRef *accessRef)
{
	BEGIN_SECAPI
	secdebug("kcitem", "SecAccessCreateFromObject(%p, %p)", sourceRef, accessRef);
	Required(accessRef);	// preflight
	SecPointer<Access> access = new Access(*aclBearer(sourceRef));
	*accessRef = access->handle();
	END_SECAPI
}


/*!
 */
OSStatus SecAccessModifyObject(SecAccessRef accessRef, CFTypeRef sourceRef)
{
	BEGIN_SECAPI
	secdebug("kcitem", "SecAccessModifyObject(%p, %p)", accessRef, sourceRef);
	Access::required(accessRef)->setAccess(*aclBearer(sourceRef), true);
	END_SECAPI
}

OSStatus
SecKeychainItemCopyAccess(SecKeychainItemRef itemRef, SecAccessRef* accessRef)
{
    BEGIN_SECAPI

	secdebug("kcitem", "SecKeychainItemCopyAccess(%p, %p)", itemRef, accessRef);
	Required(accessRef);	// preflight
	SecPointer<Access> access = new Access(*aclBearer(reinterpret_cast<CFTypeRef>(itemRef)));
	*accessRef = access->handle();

    END_SECAPI
}


OSStatus
SecKeychainItemSetAccess(SecKeychainItemRef itemRef, SecAccessRef accessRef)
{
    BEGIN_SECAPI

	secdebug("kcitem", "SecKeychainItemSetAccess(%p, %p)", itemRef, accessRef);
	Access::required(accessRef)->setAccess(*aclBearer(reinterpret_cast<CFTypeRef>(itemRef)), true);

    END_SECAPI
}

/*  Sets an item's data for legacy "KC" CoreServices APIs.
    Note this version sets the data, but doesn't update the item 
    as the KC behavior dictates.
*/
OSStatus SecKeychainItemSetData(SecKeychainItemRef itemRef, UInt32 length, const void* data)
{
    BEGIN_SECAPI
		ItemImpl::required(itemRef)->setData(length, data);
	END_SECAPI
}

/*  Gets an item's data for legacy "KC" CoreServices APIs.
    Note this version doesn't take a SecItemClass parameter.
*/
OSStatus SecKeychainItemGetData(SecKeychainItemRef itemRef, UInt32 maxLength, void* data, UInt32* actualLength)
{
    BEGIN_SECAPI
		/* The caller either needs to specify data and maxLength or an actualLength, so we return either the data itself or the actual length of the data or both.  */
		if (!((data && maxLength) || actualLength))
			MacOSError::throwMe(paramErr);

        CssmDataContainer aData;
        ItemImpl::required(itemRef)->getData(aData);
        if (actualLength)
            *actualLength = aData.length();
    
		if (data)
		{
			// Make sure the buffer is big enough
			if (aData.length() > maxLength)
				MacOSError::throwMe(errKCBufferTooSmall);
			memcpy(data, aData.data(), aData.length());
		}
	END_SECAPI
}

/*  Update a keychain item for legacy "KC" CoreServices APIs.
    The "KC" API's do a 'set attribute', then an 'update'.
*/
OSStatus SecKeychainItemUpdate(SecKeychainItemRef itemRef)
{
    BEGIN_SECAPI
        ItemImpl::required(itemRef)->update();
	END_SECAPI
}

/* Add a 'floating' keychain item without UI for legacy "KC" CoreServices APIs.
*/
OSStatus SecKeychainItemAddNoUI(SecKeychainRef keychainRef, SecKeychainItemRef itemRef)
{
    BEGIN_SECAPI
        Item item = ItemImpl::required(itemRef);
        Keychain::optional(keychainRef)->add(item);
	END_SECAPI
}

/* Add a 'floating' keychain item to the default keychain with possible UI for legacy "KC" Carbon APIs.
*/
OSStatus SecKeychainItemAdd(SecKeychainItemRef itemRef)
{
    BEGIN_SECAPI
        Item item = ItemImpl::required(itemRef);
        Keychain defaultKeychain = globals().storageManager.defaultKeychainUI(item);
        defaultKeychain->add(item);
	END_SECAPI
}

/* Creates a floating keychain item for legacy "KC" CoreServices APIs
*/
OSStatus SecKeychainItemCreateNew(SecItemClass itemClass, OSType itemCreator, UInt32 length, const void* data, SecKeychainItemRef* itemRef)
{
    BEGIN_SECAPI
        RequiredParam(itemRef) = Item(itemClass, itemCreator, length, data)->handle();
	END_SECAPI
}

/* Gets an individual attribute for legacy "KC" CoreServices APIs
*/
OSStatus SecKeychainItemGetAttribute(SecKeychainItemRef itemRef, SecKeychainAttribute* attribute, UInt32* actualLength)
{
    BEGIN_SECAPI
        ItemImpl::required(itemRef)->getAttribute(RequiredParam(attribute), actualLength);
	END_SECAPI
}

/* Sets an individual attribute for legacy "KC" CoreServices APIs
*/
OSStatus SecKeychainItemSetAttribute(SecKeychainItemRef itemRef, SecKeychainAttribute* attribute)
{
    BEGIN_SECAPI
        ItemImpl::required(itemRef)->setAttribute(RequiredParam(attribute));
	END_SECAPI
}

/*  Finds a keychain item for legacy "KC" CoreServices APIs.
    Note: This version doesn't take a SecItemClass because 
            SecKeychainSearchCreateFromAttributes() requires it.
    @@@ This should move to SecKeychainSearch.cpp
*/
OSStatus SecKeychainItemFindFirst(SecKeychainRef keychainRef, const SecKeychainAttributeList *attrList, SecKeychainSearchRef *searchRef, SecKeychainItemRef *itemRef)
{
    BEGIN_SECAPI
        KCCursor cursor;
        if (keychainRef)
            cursor = KeychainImpl::required(keychainRef)->createCursor(attrList);
        else
            cursor = globals().storageManager.createCursor(attrList);
    
        Item item;
        if (!cursor->next(item))
            return errKCItemNotFound;
    
        *itemRef=item->handle();
        if (searchRef)
            *searchRef=cursor->handle();
	END_SECAPI
}
