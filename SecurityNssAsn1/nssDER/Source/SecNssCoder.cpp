/*
 * Copyright (c) 2003 Apple Computer, Inc. All Rights Reserved.
 * 
 * The contents of this file constitute Original Code as defined in and are
 * subject to the Apple Public Source License Version 1.2 (the 'License').
 * You may not use this file except in compliance with the License. Please 
 * obtain a copy of the License at http://www.apple.com/publicsource and 
 * read it before using this file.
 * 
 * This Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER 
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES, 
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT. 
 * Please see the License for the specific language governing rights and 
 * limitations under the License.
 */
/*
 * SecNssCoder.cpp: simple C++ wrapper for PLArenaPool and the
 * high-level ANS1 encode/decode routines.
 */
 
#include "SecNssCoder.h"
#include <Security/cssmerr.h>
#include <Security/utilities.h>
#include <SecurityNssAsn1/secasn1.h>
#include <string.h>
#include <assert.h>

#ifdef	NDEBUG
#define THROW_ENABLE	1
#else
/* disable link against Security framework when true */
#define THROW_ENABLE	0
#endif

#if		THROW_ENABLE
#define THROW_ERROR	Security::CssmError::throwMe(CSSMERR_CSSM_MEMORY_ERROR)
#else
#define THROW_ERROR
#endif

SecNssCoder::SecNssCoder(PRUint32 chunkSize /* = SNC_CHUNKSIZE_DEF */)
	: mPool(NULL)
{
	mPool = PORT_NewArena(chunkSize);
	if(mPool == NULL) {
		THROW_ERROR;
	}
}

SecNssCoder::~SecNssCoder()
{
	if(mPool != NULL) {
		/*
		 * Note: we're asking for a memory zero here, but 
		 * PORT_FreeArena doesn't do that (yet).
		 */
		PORT_FreeArena(mPool, PR_TRUE);
		mPool = NULL;
	}
}

PRErrorCode	SecNssCoder::decode(
	const void				*src,		// BER-encoded source
	unsigned long			len,
	const SEC_ASN1Template 	*templ,	
	void					*dest)
{
	SECStatus prtn;
	
	assert(mPool != NULL);
	prtn = SEC_ASN1Decode(mPool, dest, templ, (const char *)src, len);
	if(prtn) {
		return PR_GetError();
	}
	else {
		return 0;
	}
}

PRErrorCode SecNssCoder::encodeItem(
	const void				*src,
	const SEC_ASN1Template 	*templ,	
	SECItem					&dest)
{
	assert(mPool != NULL);

	dest.Data = NULL;
	dest.Length = 0;
	
	SECItem *rtnItem = SEC_ASN1EncodeItem(mPool, &dest, src, templ);
	if(rtnItem == NULL) {
		return PR_GetError();
	}
	else {
		return 0;
	}
}

void *SecNssCoder::malloc(size_t len)
{
	assert(mPool != NULL);
	void *rtn = PORT_ArenaAlloc(mPool, len);
	if(rtn == NULL) {
		THROW_ERROR;
	}
	return rtn;
}

/* malloc item.Data, set item.Length */
void SecNssCoder::allocItem(
	SECItem					&item,
	size_t					len)
{
	item.Data = (uint8 *)malloc(len);
	item.Length = len;
}

/* malloc and copy */
void SecNssCoder::allocCopyItem(
	const void				*src,
	size_t					len,
	SECItem					&dest)
{
	allocItem(dest, len);
	memmove(dest.Data, src, len);
}

/*
 * This is pretty much a copy of SEC_ASN1EncodeItem, with a CssmAllocator
 * malloc replacing the sec_asn1e_allocate_item to alloc the output data.
 */
PRErrorCode SecNssEncodeItem(
	const void				*src,
	const SEC_ASN1Template 	*templ,	
	Security::CssmAllocator	&alloc,
	SECItem					&dest)
{
    unsigned long encoding_length = 0;
    SECStatus rv;

	dest.Data = NULL;
	dest.Length = 0;

    rv = SEC_ASN1Encode (src, templ,
			 sec_asn1e_encode_item_count, &encoding_length);
    if (rv != SECSuccess) {
		return PR_GetError();
	}

	/* replace this...
    dest = sec_asn1e_allocate_item (poolp, dest, encoding_length);
    if (dest == NULL)
	return NULL;
	... with this: */
	dest.Data = (uint8 *)alloc.malloc(encoding_length);
	dest.Length = 0;
	/* end replacement */
	
    rv = SEC_ASN1Encode (src, templ, sec_asn1e_encode_item_store, &dest);
    if (rv != SECSuccess) {
		return PR_GetError();
	}

    assert(encoding_length == dest.Length);
    return 0;
}

PRErrorCode SecNssEncodeItemOdata(
	const void				*src,
	const SEC_ASN1Template 	*templ,	
	CssmOwnedData			&odata)
{
	CssmAllocator &alloc = odata.allocator;
	SECItem sitem;
	PRErrorCode prtn;
	
	prtn = SecNssEncodeItem(src, templ, alloc, sitem);
	if(prtn) {
		return prtn;
	}
	odata.set(sitem.Data, sitem.Length);
	return 0;
}


