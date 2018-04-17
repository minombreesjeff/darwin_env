/*
 *  Copyright (c) 2003-2004 Apple Computer, Inc. All Rights Reserved.
 *
 *  @APPLE_LICENSE_HEADER_START@
 *  
 *  Copyright (c) 1999-2003 Apple Computer, Inc.  All Rights Reserved.
 *  
 *  This file contains Original Code and/or Modifications of Original Code
 *  as defined in and that are subject to the Apple Public Source License
 *  Version 2.0 (the 'License'). You may not use this file except in
 *  compliance with the License. Please obtain a copy of the License at
 *  http://www.opensource.apple.com/apsl/ and read it before using this
 *  file.
 *  
 *  The Original Code and all software distributed under the License are
 *  distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 *  EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 *  INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 *  Please see the License for the specific language governing rights and
 *  limitations under the License.
 *  
 *  @APPLE_LICENSE_HEADER_END@
 *
 *  SecKeychainAddIToolsPassword.c
 *  
 *	Based on Keychain item access control example
 *	  -- added "always allow" ACL support
 */

#include <Security/SecKeychain.h>
#include <Security/SecKeychainItem.h>
#include <Security/SecAccess.h>
#include <Security/SecAccessPriv.h>
#include <Security/SecTrustedApplication.h>
#include <Security/SecACL.h>
#include <CoreFoundation/CoreFoundation.h>
#include <sys/param.h>

static CFArrayRef CopyTrustedAppListFromBundle();
static SecAccessRef createAccess(CFStringRef accessLabel,const int allowAny);

OSStatus SecKeychainAddIToolsPassword(SecKeychainRef keychain, UInt32 accountNameLength, const char *accountName,
    UInt32 passwordLength, const void *passwordData, SecKeychainItemRef *itemRef)
{
    OSStatus err;
    SecKeychainItemRef item = nil;
    const char *serviceUTF8 = "iTools";
	CFStringRef itemLabel = CFSTR("iTools");
	const int allowAny = 0;
    
	// create initial access control settings for the item
	SecAccessRef access = createAccess(itemLabel, allowAny);
	
	// below is the lower-layer equivalent to the SecKeychainAddGenericPassword() function;
	// it does the same thing (except specify the access controls)
	
	// set up attribute vector (each attribute consists of {tag, length, pointer})
	SecKeychainAttribute attrs[] =
    {
		{ kSecLabelItemAttr, strlen(serviceUTF8), (char *)serviceUTF8 },	// use the service string as the name of this item for display purposes
		{ kSecAccountItemAttr, accountNameLength, (char *)accountName },
		{ kSecServiceItemAttr, strlen(serviceUTF8), (char *)serviceUTF8 }
	};
	SecKeychainAttributeList attributes = { sizeof(attrs) / sizeof(attrs[0]), attrs };

	err = SecKeychainItemCreateFromContent(kSecGenericPasswordItemClass,
		&attributes,
		passwordLength,
		(const char *)passwordData,
		keychain,
		access,
		&item);
	
	if (access)
        CFRelease(access);
	if (item)	
        CFRelease(item);
    return noErr;
}

SecAccessRef createAccess(CFStringRef accessLabel,const int allowAny)
{
	OSStatus err;
	SecAccessRef access=nil;
	CFMutableArrayRef trustedApplications=nil;
	
	if (!allowAny) // use default access ("confirm access")
	{
		// make an exception list of applications you want to trust,
		// which are allowed to access the item without requiring user confirmation
		SecTrustedApplicationRef myself=NULL, someOther=NULL;
        CFArrayRef trustedAppListFromBundle=NULL;
        
        trustedApplications=CFArrayCreateMutable(kCFAllocatorDefault,0,&kCFTypeArrayCallBacks); 
        err = SecTrustedApplicationCreateFromPath(NULL, &myself);
        if (!err)
            CFArrayAppendValue(trustedApplications,myself); 

        trustedAppListFromBundle=CopyTrustedAppListFromBundle();
        if (trustedAppListFromBundle)
        {
            int ix,top;
            char buffer[MAXPATHLEN];
            top = CFArrayGetCount(trustedAppListFromBundle);
            for (ix=0;ix<top;ix++)
            {
                CFStringRef filename = CFArrayGetValueAtIndex(trustedAppListFromBundle,ix);
                CFIndex stringLength = CFStringGetLength(filename);
                CFIndex usedBufLen; 
            //    CFShow(filename);	// debug

                if (stringLength != CFStringGetBytes(filename,CFRangeMake(0,stringLength),kCFStringEncodingUTF8,0,
                    false,(UInt8 *)&buffer,MAXPATHLEN, &usedBufLen))
                    break;
                buffer[usedBufLen] = 0;
            //    printf("converted filename: [sl=%d, ubl=%d %s\n",stringLength,usedBufLen,buffer);
                err = SecTrustedApplicationCreateFromPath(buffer,&someOther);
                if (!err)
                    CFArrayAppendValue(trustedApplications,someOther); 
            }
            CFRelease(trustedAppListFromBundle);
        }
	}

	err = SecAccessCreate((CFStringRef)accessLabel, (CFArrayRef)trustedApplications, &access);
    if (err)
        return nil;

	if (allowAny) // change access to be wide-open for decryption ("always allow access")
	{
		// get the access control list for decryption operations
		CFArrayRef aclList=nil;
		err = SecAccessCopySelectedACLList(access, CSSM_ACL_AUTHORIZATION_DECRYPT, &aclList);
		
		if (!err)
		{
			// get the first entry in the access control list
			SecACLRef aclRef=(SecACLRef)CFArrayGetValueAtIndex(aclList, 0);
			CFArrayRef appList=nil;
			CFStringRef promptDescription=nil;
			CSSM_ACL_KEYCHAIN_PROMPT_SELECTOR promptSelector;
			err = SecACLCopySimpleContents(aclRef, &appList, &promptDescription, &promptSelector);

			// modify the default ACL to not require the passphrase, and have a nil application list
			promptSelector.flags &= ~CSSM_ACL_KEYCHAIN_PROMPT_REQUIRE_PASSPHRASE;
			err = SecACLSetSimpleContents(aclRef, NULL, promptDescription, &promptSelector);

			if (appList) CFRelease(appList);
			if (promptDescription) CFRelease(promptDescription);
		}
	}

	return access;
}

CFArrayRef CopyTrustedAppListFromBundle()
{
    CFStringRef errorString = nil;
    CFURLRef bundleURL,trustedAppsURL = NULL;
    CFBundleRef secBundle = NULL;
	CFPropertyListRef trustedAppsPlist = NULL;
	CFDataRef xmlDataRef = NULL;
	SInt32 errorCode;
    CFArrayRef trustedAppList = NULL;

    // Make a CFURLRef from the CFString representation of the bundleÕs path.
    bundleURL = CFURLCreateWithFileSystemPath( 
        kCFAllocatorDefault,CFSTR("/System/Library/Frameworks/Security.framework/Resources/"),kCFURLPOSIXPathStyle,true);
    if (!bundleURL)
        goto xit;
        
    // Make a bundle instance using the URLRef.
    secBundle = CFBundleCreate(kCFAllocatorDefault,bundleURL);
    if (!secBundle)
        goto xit;

    // Look for a resource in the bundle by name and type
    trustedAppsURL = CFBundleCopyResourceURL(secBundle,CFSTR("iToolsTrustedApps"),CFSTR("plist"),NULL);
    if (!trustedAppsURL)
        goto xit;

    if (!CFURLCreateDataAndPropertiesFromResource(kCFAllocatorDefault,trustedAppsURL,&xmlDataRef,NULL,NULL,&errorCode))
        goto xit;
        
	trustedAppsPlist = CFPropertyListCreateFromXMLData(kCFAllocatorDefault,xmlDataRef,kCFPropertyListImmutable,&errorString);
    
//    if (!CFPropertyListIsValid(trustedAppsPlist,kCFPropertyListXMLFormat_v1_0))
//        goto xit;

/*
	if (CFGetTypeID(trustedAppsPlist) != CFGetTypeID(trustedAppList))
    {
        CFRelease(trustedAppsPlist);
        goto xit;
    }
*/
    trustedAppList = (CFArrayRef)trustedAppsPlist;
    
xit:
    if (bundleURL)
        CFRelease(bundleURL);	
    if (secBundle)
        CFRelease(secBundle);	
    if (trustedAppsURL)
        CFRelease(trustedAppsURL);
    if (xmlDataRef)
        CFRelease(xmlDataRef);
    if (errorString)
        CFRelease(errorString);

    return trustedAppList;
}

