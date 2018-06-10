/*
 * Copyright (c) 2000-2001 Apple Computer, Inc. All Rights Reserved.
 * 
 * The contents of this file constitute Original Code as defined in and are
 * subject to the Apple Public Source License Version 1.2 (the 'License').
 * You may not use this file except in compliance with the License. Please obtain
 * a copy of the License at http://www.apple.com/publicsource and read it before
 * using this file.
 * 
 * This Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS
 * OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT. Please see the License for the
 * specific language governing rights and limitations under the License.
 */


//
// miscAlgFactory.h - miscellaneous algorithm factory
// Written by Doug Mitchell 3/28/2001
//

#include "miscAlgFactory.h"
#include <aescspi.h>
#include <gladmanContext.h>
#include "desContext.h"
#include "rc2Context.h"
#include "rc4Context.h"
#include "rc5Context.h"
#include "MacContext.h"
#include "DigestContext.h"
#include "SHA1_MD5_Object.h"			/* raw digest */
#include "MD2Object.h"
#include "NullCryptor.h"
#include "bfContext.h"
#include "castContext.h"
#include <Security/cssmapple.h>

/*
 * These #defines are mainly to facilitate measuring the performance of our own
 * implementation vs. the ones in BSafe. This factory gets called first; if
 * we disable e.g. DES here the BSAFE version will be used.
 */
#ifdef	BSAFE_CSP_ENABLE

#define MAF_DES_ENABLE		0
#define MAF_DES3_ENABLE		0
#define MAF_RC2_ENABLE		0
#define MAF_RC4_ENABLE		0
#define MAF_RC5_ENABLE		0
#define MAF_MAC_ENABLE		0

#else	/* !BSAFE_CSP_ENABLE, normal case */

#define MAF_DES_ENABLE		1
#define MAF_DES3_ENABLE		1
#define MAF_RC2_ENABLE		1
#define MAF_RC4_ENABLE		1
#define MAF_RC5_ENABLE		1
#define MAF_MAC_ENABLE		1

#endif	/* BSAFE_CSP_ENABLE */

#if	(!MAF_DES_ENABLE || !MAF_DES3_ENABLE || !MAF_RC2_ENABLE || !MAF_RC4_ENABLE || \
		!MAF_RC5_ENABLE || !MAF_MAC_ENABLE)
#warning	Internal DES/RC2/RC4/RC5/Mac implementation disabled! 
#endif

/*
 * Synthetic secure passphrase generation context. This eventually
 * goes away in favor of the real code in Security server. 
 */
#ifndef NDEBUG
#define SECURE_PASSPHRASE_ENABLE	1
#else
#define SECURE_PASSPHRASE_ENABLE	0
#endif

#if SECURE_PASSPHRASE_ENABLE

/* 
 * HACK! DO not even think of enabling this for a checked-in NDEBUG configuration!
 */
#include <CoreFoundation/CoreFoundation.h>
/* 
 * Safe gets().
 * -- guaranteed no buffer overflow
 * -- guaranteed NULL-terminated string
 * -- handles empty string (i.e., response is just CR) properly
 */
void getString(
	char *buf,
	unsigned bufSize)
{
	unsigned dex;
	char c;
	char *cp = buf;
	*cp = '\0';
	
	for(dex=0; dex<bufSize-1; dex++) {
		c = getchar();
		if (c == EOF) {
			break;
		}
		
		if(!isprint(c)) {
			break;
		}
		switch(c) {
			case '\n':
			case '\r':
				goto done;
			default:
				*cp++ = c;
		}
	}
done:
	*cp = '\0';
}

class SecurePhraseGenerator : public AppleCSPContext {
public:
	SecurePhraseGenerator(
		AppleCSPSession &session) :
			AppleCSPContext(session) { }
	
	void init(const Context &context, bool encoding = true) { }
			
	void generate(
		const Context 	&context, 
		CssmKey 		&symKey, 
		CssmKey 		&dummyKey);
};

static void showStringAttr(
	const Context		&context, 
	const char			*label,
	CSSM_ATTRIBUTE_TYPE attrType)
{
	CssmData *dataAttr = context.get<CssmData>(attrType);
	if(dataAttr == NULL) {
		return;
	}
	CFDataRef cfData = CFDataCreate(NULL, dataAttr->Data, dataAttr->Length);
	CFStringRef cfStr = CFStringCreateFromExternalRepresentation(NULL,
		cfData, kCFStringEncodingUTF8);
	if(cfStr == NULL) {
		printf("***Bad context attr string\n");
		CssmError::throwMe(CSSMERR_CSP_INVALID_ATTR_SEED);
	}
	char buf[1000];
	CFStringGetCString(cfStr, buf, 1000, kCFStringEncodingUTF8);
	printf("%s: %s\n", label, buf);
	CFRelease(cfData);
	CFRelease(cfStr);
}

void SecurePhraseGenerator::generate(
		const Context 	&context, 
		CssmKey 		&cssmKey, 
		CssmKey 		&dummyKey)
{

	/* here's the hack: get a attributes from context and string from the user */
	printf("====== secure passphrase acquisition =====\n");
	showStringAttr(context, "Alert Title", CSSM_ATTRIBUTE_ALERT_TITLE);
	showStringAttr(context, "Alert Prompt", CSSM_ATTRIBUTE_PROMPT);
	try {
		uint32 vfyBit = context.getInt(CSSM_ATTRIBUTE_VERIFY_PASSPHRASE,
			CSSMERR_CSP_INVALID_ATTR_PASSPHRASE);
		printf("Verify Phrase bit: %lu\n", vfyBit);
	}
	catch(...) {
		printf("***No CSSM_ATTRIBUTE_VERIFY_PASSPHRASE attribute!\n");
	}
	char phraseStr[512];
	
	printf("Enter passphrase: ");
	getString(phraseStr, 512);
	if(phraseStr[0] == 0) {
		CssmError::throwMe(CSSMERR_CSP_USER_CANCELED);
	}
	printf("==========================================\n");

	CFStringRef cfStr = CFStringCreateWithCString(NULL, phraseStr, kCFStringEncodingASCII);
	CFDataRef cfData = CFStringCreateExternalRepresentation(NULL,
		cfStr, kCFStringEncodingUTF8, 0);
	unsigned phraseLen = CFDataGetLength(cfData);
	unsigned char *phrase = (unsigned char *)CFDataGetBytePtr(cfData);
	
	/* remainder copied from AppleSymmKeyGenContext::generateSymKey() */
	
	// validate KeyAtrr and KeyUsage already present in header
	cspKeyStorage 	keyStorage;
	CssmKey::Header	&hdr  = cssmKey.header(); 
	
	keyStorage = cspParseKeyAttr(CKT_Session,  hdr.KeyAttr);
	cspValidateKeyUsageBits(CKT_Session, hdr.KeyUsage);
	hdr.KeyAttr  &= ~KEY_ATTR_RETURN_MASK;
	
	hdr.LogicalKeySizeInBits = phraseLen * 8;
	SymmetricBinaryKey *binKey = NULL;
	CssmData *keyData = NULL;
	
	switch(keyStorage) {
		case CKS_None:
			/* no way */
			CssmError::throwMe(CSSMERR_CSP_INVALID_KEYATTR_MASK);
		case CKS_Ref:
			/* cook up a symmetric binary key */
			binKey = new SymmetricBinaryKey(phraseLen * 8);
			keyData = &binKey->mKeyData;
			break;
		case CKS_Data:
			/* key bytes --> caller's cssmKey */
			keyData = &(CssmData::overlay(cssmKey.KeyData));
			setUpCssmData(*keyData, phraseLen, 
				session().normAlloc());
			break;
	}
	
	// in any case, fill key bytes with phrase data
	memmove(keyData->Data, phrase, phraseLen);
	// orig: session.getRandomBytes(keySizeInBytes, keyData->Data);

	if(keyStorage == CKS_Ref) {
		session().addRefKey(*binKey, cssmKey);
	}
	else {
		/* Raw data */
		hdr.BlobType = CSSM_KEYBLOB_RAW;
		hdr.Format = CSSM_KEYBLOB_RAW_FORMAT_OCTET_STRING; 
	}
	CFRelease(cfData);
	CFRelease(cfStr);
}

#endif  /* SECURE_PASSPHRASE_ENABLE */

bool MiscAlgFactory::setup(
	AppleCSPSession &session,
	CSPFullPluginSession::CSPContext * &cspCtx, 
	const Context &context)
{
	CSSM_CONTEXT_TYPE ctype = context.type();
	CSSM_ALGORITHMS alg = context.algorithm();
	
	switch(ctype) {
		case CSSM_ALGCLASS_SYMMETRIC:
			switch(alg) {
				case CSSM_ALGID_AES:
					if(cspCtx == NULL) {
						/* 
						 * Get optional block size to determine correct implementation
						 */
						uint32 blockSize = context.getInt(CSSM_ATTRIBUTE_BLOCK_SIZE);
						if(blockSize == 0) {
							blockSize = GLADMAN_BLOCK_SIZE_BYTES;
						}
						if(GLADMAN_AES_128_ENABLE && 
							(blockSize == GLADMAN_BLOCK_SIZE_BYTES)) {
							cspCtx = new GAESContext(session);
						}
						else {
							cspCtx = new AESContext(session);
						}
					}
					return true;

				#if		MAF_DES_ENABLE
				case CSSM_ALGID_DES:
					if(cspCtx == NULL) {
						cspCtx = new DESContext(session);
					}
					return true;
				#endif	/* MAF_DES_ENABLE */
				
				#if		MAF_DES3_ENABLE
				/*
				 * TripleDES: for some reason, cssmtype.h defines different symbols
				 * for CSSM_ALGID_3DES_3KEY (key gen) and CSSM_ALGID_3DES_3KEY_EDE
				 * (an encrypt alg with mode), but they define to the same value.
				 */
				case CSSM_ALGID_3DES_3KEY_EDE:
					if(cspCtx == NULL) {
						cspCtx = new DES3Context(session);
					}
					return true;
				#endif
				
				#if		MAF_RC2_ENABLE
				case CSSM_ALGID_RC2:
					if(cspCtx == NULL) {
						cspCtx = new RC2Context(session);
					}
					return true;
				#endif
				
				#if		MAF_RC4_ENABLE
				case CSSM_ALGID_RC4:
					if(cspCtx == NULL) {
						cspCtx = new RC4Context(session);
					}
					return true;
				#endif
				
				#if		MAF_RC5_ENABLE
				case CSSM_ALGID_RC5:
					if(cspCtx == NULL) {
						cspCtx = new RC5Context(session);
					}
					return true;
				#endif
				
				case CSSM_ALGID_BLOWFISH:
					if(cspCtx == NULL) {
						cspCtx = new BlowfishContext(session);
					}
					return true;

				case CSSM_ALGID_CAST:
				case CSSM_ALGID_CAST5:			
					if(cspCtx == NULL) {
						cspCtx = new CastContext(session);
					}
					return true;

				#if		NULL_CRYPT_ENABLE
				case CSSM_ALGID_NONE:
					if(cspCtx == NULL) {
						cspCtx = new NullCryptor(session);
					}
					return true;
				#endif	/* NULL_CRYPT_ENABLE */
				
				default:
					break;	// not our symmetric alg
			}				// switch alg for symmetric 
			break;			// from case CSSM_ALGCLASS_SYMMETRIC 
			
		/* digest algorithms always enabled here */
		case CSSM_ALGCLASS_DIGEST:
			switch(alg) {
				case CSSM_ALGID_SHA1:
					if(cspCtx == NULL) {
						/* reuse is OK */
						cspCtx = new DigestContext(session, 
								*(new SHA1Object));
					}
					return true;
				case CSSM_ALGID_MD5:
					if(cspCtx == NULL) {
						/* reuse is OK */
						cspCtx = new DigestContext(session, 
								*(new MD5Object));
					}
					return true;
				case CSSM_ALGID_MD2:
					if(cspCtx == NULL) {
						/* reuse is OK */
						cspCtx = new DigestContext(session, 
								*(new MD2Object));
					}
					return true;
				default:
					break;		// not our digest alg
			}					// switch digest alg
			break;				// from case CSSM_ALGCLASS_DIGEST
			
		case CSSM_ALGCLASS_KEYGEN:
			switch(alg) {
				case CSSM_ALGID_AES:
					if(cspCtx == NULL) {
						cspCtx = new AESKeyGenContext(session);
					}
					return true;

				#if		MAF_DES_ENABLE
				case CSSM_ALGID_DES:
					if(cspCtx == NULL) {
						cspCtx = new AppleSymmKeyGenerator(session,
							DES_KEY_SIZE_BITS_EXTERNAL,
							DES_KEY_SIZE_BITS_EXTERNAL,
							true);				// must be byte size
					}
					return true;
				#endif	/* MAF_DES_ENABLE */
				
				#if		MAF_DES3_ENABLE
				case CSSM_ALGID_3DES_3KEY_EDE:
					if(cspCtx == NULL) {
						cspCtx = new AppleSymmKeyGenerator(session,
							DES3_KEY_SIZE_BYTES * 8,
							DES3_KEY_SIZE_BYTES * 8,
							true);			// must be byte size
					}
					return true;
				#endif
				
				#if		MAF_RC2_ENABLE
				case CSSM_ALGID_RC2:
					if(cspCtx == NULL) {
						cspCtx = new AppleSymmKeyGenerator(session,
							RC2_MIN_KEY_SIZE_BYTES * 8,
							RC2_MAX_KEY_SIZE_BYTES * 8,
							true);				// must be byte size
					}
					return true;
				#endif
				
				#if		MAF_RC4_ENABLE
				case CSSM_ALGID_RC4:
					if(cspCtx == NULL) {
						cspCtx = new AppleSymmKeyGenerator(session,
							RC4_MIN_KEY_SIZE_BYTES * 8,
							RC4_MAX_KEY_SIZE_BYTES * 8,
							true);				// must be byte size
					}
					return true;
				#endif
				
				#if		MAF_RC5_ENABLE
				case CSSM_ALGID_RC5:
					if(cspCtx == NULL) {
						cspCtx = new AppleSymmKeyGenerator(session,
							RC5_MIN_KEY_SIZE_BYTES * 8,
							RC5_MAX_KEY_SIZE_BYTES * 8,
							true);				// must be byte size
					}
					return true;
				#endif
				
				case CSSM_ALGID_BLOWFISH:
					if(cspCtx == NULL) {
						cspCtx = new AppleSymmKeyGenerator(session,
							BF_MIN_KEY_SIZE_BYTES * 8,
							BF_MAX_KEY_SIZE_BYTES * 8,
							true);				// must be byte size
					}
					return true;

				/* Note we require keys to be ALGID_CAST, not ALGID_CAST5 */
				case CSSM_ALGID_CAST:
					if(cspCtx == NULL) {
						cspCtx = new AppleSymmKeyGenerator(session,
							CAST_MIN_KEY_LENGTH * 8,
							CAST_KEY_LENGTH * 8,
							true);				// must be byte size
					}
					return true;

				#if		MAF_MAC_ENABLE
				case CSSM_ALGID_SHA1HMAC:
					if(cspCtx == NULL) {
						cspCtx = new AppleSymmKeyGenerator(session,
							HMAC_SHA_MIN_KEY_SIZE * 8,
							HMAC_MAX_KEY_SIZE * 8,
							true);				// must be byte size
					}
					return true;
				case CSSM_ALGID_MD5HMAC:
					if(cspCtx == NULL) {
						cspCtx = new AppleSymmKeyGenerator(session,
							HMAC_MD5_MIN_KEY_SIZE * 8,
							HMAC_MAX_KEY_SIZE * 8,
							true);				// must be byte size
					}
					return true;
				#endif
				
				#if		NULL_CRYPT_ENABLE
				case CSSM_ALGID_NONE:
					if(cspCtx == NULL) {
						cspCtx = new AppleSymmKeyGenerator(session,
							NULL_CRYPT_BLOCK_SIZE * 8,
							NULL_CRYPT_BLOCK_SIZE * 8,
							true);				// must be byte size
					}
					return true;
				#endif	/* NULL_CRYPT_ENABLE */
				
				#if		SECURE_PASSPHRASE_ENABLE
				case CSSM_ALGID_SECURE_PASSPHRASE:
					if(cspCtx == NULL) {
						cspCtx = new SecurePhraseGenerator(session);
					}
					return true;
				#endif  /* SECURE_PASSPHRASE_ENABLE */
				
				default:
					break;	// not our keygen alg
			}				// switch alg for keygen
			break;			// from case CSSM_ALGCLASS_KEYGEN
			
		case CSSM_ALGCLASS_MAC:
			switch(alg) {
				#if		MAF_MAC_ENABLE
				case CSSM_ALGID_SHA1HMAC:
				case CSSM_ALGID_MD5HMAC:
					if(cspCtx == NULL) {
						cspCtx = new MacContext(session, alg);
					}
					return true;
				#endif
				#if		CRYPTKIT_CSP_ENABLE
				case CSSM_ALGID_SHA1HMAC_LEGACY:
					if(cspCtx == NULL) {
						cspCtx = new MacLegacyContext(session, alg);
					}
					return true;
				#endif
				default:
					/* not our mac alg */
					break;
			}
			break;
			
		default:
			break;			// not our context type
	}						// switch context type
	
	/* not ours */
	return false;
}
