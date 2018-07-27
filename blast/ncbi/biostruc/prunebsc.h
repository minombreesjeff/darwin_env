/*****************************************************************************
*
*   prunebiostruc.h
*
*****************************************************************************/

#ifndef _PRUNEBIOSTRUC_
#define _PRUNEBIOSTRUC_

#include <ncbi.h>
#include <asn.h>
#include <mmdbapi.h>

#include "strimprt.h"
#define MAXNUM 50

#ifdef __cplusplus
extern "C" {
#endif

BiostrucPtr LIBCALL PruneBiostruc(BiostrucPtr bsp, CharPtr chain);

#ifdef __cplusplus
}
#endif

#endif
