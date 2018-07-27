/*
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
* ===========================================================================
*
* File Name:    ni_encrs.c
*
* Author:       Epstein
*
* Version Creation Date:        2/14/94
*
* $Revision: 6.0 $
*
* File Description:
*   Stub replacement for ni_encr.c.  This makes it easy to build NCBI Network
*   Services tools either with or without encryption support.
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 2/15/94  Epstein     Replaced nice macro with explicit definitions, due to
*                      inconsistency involving C pre-processors.
*
*
* RCS Modification History:
* $Log: ni_encrs.c,v $
* Revision 6.0  1997/08/25 18:38:36  madden
* Revision changed to 6.0
*
* Revision 5.1  1997/07/01 19:12:43  vakatov
* [WIN32]  DLL'd "netcli.lib"
*
* Revision 5.0  1996/05/28 14:11:55  ostell
* Set to revision 5.0
*
 * Revision 4.0  1995/07/26  13:56:32  ostell
 * force revision to 4.0
 *
 * Revision 1.5  1995/05/17  17:52:06  epstein
 * add RCS log revision history
 *
*/

#include <ncbi.h>
#include <ni_types.h>


/* this is the only function which may be called legally */
NLM_EXTERN Boolean LIBCALL NI_EncrAvailable (void)
{ return FALSE; }

NLM_EXTERN Boolean LIBCALL NI_SetupDESEncryption (NI_HandPtr mh, UcharPtr desKey)
{ ErrPostEx(SEV_ERROR,0,0,"Invalid call to encryption function NI_SetupDESEncryption"); return FALSE; }

NLM_EXTERN Boolean LIBCALL NI_PubKeysEqual (NI_PubKeyPtr x, NI_PubKeyPtr y)
{ ErrPostEx(SEV_ERROR,0,0,"Invalid call to encryption function NI_PubKeysEqual"); return FALSE; }

NLM_EXTERN void NI_GenerateDESKey (UcharPtr desKey)
{ ErrPostEx(SEV_ERROR,0,0,"Invalid call to encryption function NI_GenerateDESKey"); }

NLM_EXTERN Boolean LIBCALL NI_GenAndWritePEMKeys (Int2 bits, AsnIoPtr pubAip, FILE *privFp)
{ ErrPostEx(SEV_ERROR,0,0,"Invalid call to encryption function NI_GenAndWritePEMKeys"); return FALSE; }

NLM_EXTERN Boolean LIBCALL NI_WritePubKeyToConfig (NI_PubKeyPtr pub)
{ ErrPostEx(SEV_ERROR,0,0,"Invalid call to encryption function NI_WritePubKeyToConfig"); return FALSE; }

NLM_EXTERN NI_PubKeyPtr LIBCALL NI_ReadPubKeyFromConfig (void)
{ ErrPostEx(SEV_ERROR,0,0,"Invalid call to encryption function NI_ReadPubKeyFromConfig"); return NULL; }

NLM_EXTERN NI_PubKeyPtr LIBCALL NI_PubKeyDup (NI_PubKeyPtr orig)
{ ErrPostEx(SEV_ERROR,0,0,"Invalid call to encryption function NI_PubKeyDup"); return NULL; }

NLM_EXTERN VoidPtr LIBCALL NI_LoadPrivKey (FILE *fp, Int2Ptr privKeyLenPtr)
{ ErrPostEx(SEV_ERROR,0,0,"Invalid call to encryption function NI_LoadPrivKey"); return NULL; }

NLM_EXTERN Int2 LIBCALL NI_PubKeyDecrypt (VoidPtr pKey, UcharPtr PNTR plainText, UcharPtr cipherText, Int2 cipherTextLen)
{ ErrPostEx(SEV_ERROR,0,0,"Invalid call to encryption function NI_PubKeyDecrypt"); return -1; }

NLM_EXTERN Int2 LIBCALL NI_PubKeyEncrypt (NI_PubKeyPtr pub, UcharPtr plainText, Int2 plainTextLen, UcharPtr PNTR cipherText)
{ ErrPostEx(SEV_ERROR,0,0,"Invalid call to encryption function NI_PubKeyDecrypt"); return -1; }

NLM_EXTERN void NI_DestroyEncrStruct (NI_EncrDataPtr encr)
{ ErrPostEx(SEV_ERROR,0,0,"Invalid call to encryption function NI_DestroyEncrStruct"); }
