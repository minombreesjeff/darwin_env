/*
 *  BLIsEFIRecoveryAccessibleDevice.c
 *  bless
 *
 *  Created by Shantonu Sen on 5/21/07.
 *  Copyright 2007 Apple Inc. All rights reserved.
 *
 */

#include "bless.h"
#include "bless_private.h"

extern bool isPreferredSystemPartition(BLContextPtr context, CFStringRef bsdName);

bool BLIsEFIRecoveryAccessibleDevice(BLContextPtr context, CFStringRef bsdName)
{	
	return isPreferredSystemPartition(context, bsdName);
}
