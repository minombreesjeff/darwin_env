/*
 *  AppleDBDMAClipLib.h
 *  AppleOnboardAudio
 *
 * 	Private header for floating point library
 *
 *  Created by Aram Lindahl on Thu Nov 14 2002.
 *  Copyright (c) 2002 Apple Computer. All rights reserved.
 *
 */

#include "iSubTypes.h"	

typedef enum {							
    e_Mode_Disabled = 0,
    e_Mode_CopyLeftToRight,
    e_Mode_CopyRightToLeft
} DualMonoModeType;
