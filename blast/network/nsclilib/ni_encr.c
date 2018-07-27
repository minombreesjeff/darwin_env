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
* File Name:    ni_encr.c
*
* Author:       Epstein
*
* Version Creation Date:        2/14/94
*
* $Revision: 6.2 $
*
* File Description:
*   Supports RSAREF-based encryption for NCBI Network Services client-server
*   architecture
*
*
* Modifications:
* --------------------------------------------------------------------------
* 02/22/94 Epstein     Fix reading of public keys from config. file, and
*                      initialization of random data structure for RSA
*                      encryption.
* 03/04/94 Epstein     Reduce memory leakage, avoid ErrPost error if unable
*                      to open public-key file.
* 03/09/94 Epstein     Add length parameter to NI_LoadPrivKey(), add code
*                      which allows {Transient}SetAppParam() to provide
*                      some help in providing an unbreakable key.  Also
*                      use high-granularity timing in generating random
*                      key, where available.
* 07/14/94 Epstein     Pad buffer more efficiently in DoDesWrite().
* 02/22/96 Epstein     Correct boundary condition in DoDesRead() to refrain
*                      from trying to decrypt a buffer when we don't have
*                      at least 8 bytes of input to work with.
*
* $Log: ni_encr.c,v $
* Revision 6.2  1998/06/23 19:45:19  vakatov
* [WIN32,MSVC++]  Made some functions be DLL-exportable
*
* Revision 6.1  1997/12/02 16:09:37  epstein
* fix use of sprintf
*
* Revision 4.2  1996/02/22  18:43:03  epstein
* Correct boundary condition in DoDesRead()
*
* Revision 4.1  1995/10/06  12:51:34  epstein
* cleaned-up port to MS Windows
* --------------------------------------------------------------------------
*/

#include <ncbi.h>
#include <ni_types.h>
#include <ni_msg.h>
#include <global.h>
#include <des.h>
#include <rsaref.h>
#ifdef OS_UNIX
#include <sys/time.h>
#endif /* OS_UNIX */

#define ENCR_DES_TYPE                 1

#define ENCR_DES_STATE_IDLE           0
#define ENCR_DES_STATE_GOT1LENBYTE    1
#define ENCR_DES_STATE_INSTREAM       2




/*
 * Purpose:     Encrypts a buffer using DES
 *
 * Parameters:
 *   encr       Encryption data structure, includes Cypher-block-chaining info
 *   buf        Input buffer
 *   len        Length of buf
 *   tmpbuf     Output buffer
 *
 * Returns:
 *                The length of the resulting encrypted data
 *
 * Description:
 *              Encodes the specified input buffer using two bytes which contain
 *              the length of the _plaintext_ data which follows, followed by
 *              the encrypted text.  Thus, for example, an input buffer of
 *              length 9 will result in 0x0, 0x9, followed by 16 bytes of
 *              encrypted data.
 *
 * Note:
 *              For safety, the output buffer must be large enough to accomodate
 *              the two-byte header, plus the length of the input buffer, plus
 *              an additional seven bytes.
 *
 *              DES cipher-block chaining (CBC) is used, and the CBC information
 *              is encoded in the desWriteContext data structure.
 */

static Int4
DoDesWrite(NI_EncrDataPtr encr, CharPtr buf, int len, CharPtr tmpbuf)
{
    int encrLen;
    UcharPtr encrBuf;
    UcharPtr outbuf = (UcharPtr) tmpbuf;

    outbuf[0] = len / 256; /* high order byte */
    outbuf[1] = len % 256; /* low order byte */

    encrLen = ((len + 7) / 8) * 8;
    encrBuf = (UcharPtr) MemNew(encrLen);

    /* pad with zeros */
    MemSet ((CharPtr) &encrBuf[len], '\0', encrLen - len);

    MemCpy ((CharPtr) encrBuf, buf, len);
    DES_CBCUpdate ((DES_CBC_CTX PNTR) encr->desWriteContext, &outbuf[2],
                   encrBuf, encrLen);
    MemFree (encrBuf);
    return (encrLen + 2);
}


/*
 * Purpose:     Encryption write-filter for DES
 *
 * Parameters:
 *   mhvoid     Pointer to message handle data structure
 *   buf        Input buffer
 *   len        Length of buf
 *   tmpbuf     Output buffer
 *
 * Returns:
 *                The length of the resulting encrypted data
 *
 * Description:
 *              Validates the input and then processes the data using
 *              DoDesWrite().
 */

static Int4 LIBCALL
DesWriteFilt(VoidPtr mhvoid, CharPtr buf, int len, CharPtr tmpbuf)
{
    NI_HandPtr mh = (NI_HandPtr) mhvoid;

    if (len <= 0 || tmpbuf == NULL || buf == NULL || mh == NULL ||
        mh->encryption == NULL)
        return 0;
    return DoDesWrite(mh->encryption, buf, len, tmpbuf);
}


/*
 * Purpose:     Decrypts a buffer using DES
 *
 * Parameters:
 *   encr       Encryption data structure, includes Cypher-block-chaining info
 *   buf        Input buffer containing encrypted data, and output plaintext
 *   bytesRead  Length of input data in buf
 *   len        Max # of bytes which will fit in buf (requested read length)
 *
 * Returns:
 *                The length of the resulting plaintext which has been processed
 *
 * Description:
 *              Decodes the specified input buffer, using a protocol which
 *              consists of two bytes of length information (realDataLeft)
 *              followed by bytesToRead bytes of encrypted data, where bytesToRead
 *              is realDataLeft padded out to a multiple of eight bytes.
 *
 *              The algorithm uses a state machine to process as much of the input
 *              buffer as possible, and to store the remainder of the unprocessed
 *              input data in encr->deferredData.
 *              
 *
 * Note:
 *
 *              DES cipher-block chaining (CBC) is used, and the CBC information
 *              is encoded in the desReadContext data structure.  Also note that
 *              the history of the CBC consists of the entire communications
 *              session between client and server up to this point in time.
 */

static Int4
DoDesRead(NI_EncrDataPtr encr, CharPtr buf, int bytesRead, int len)
{
    UcharPtr scratchInbuf;
    UcharPtr ip;               /* ptr to next byte in scratchInbuf */
    Int4 retval;
    Int2 roomInBuffer;
    Int2 avail;
    Int2 bytesToDecrypt;
    Int2 bytesToCopy;
    UcharPtr scratchOutbuf;
    Boolean done;

    if (bytesRead <= 0 || buf == NULL || encr == NULL)
        return 0;

    if (encr->state == ENCR_DES_STATE_IDLE)
    {
        encr->numDeferredBytes = 0;
        encr->realDataLeft = 0;
        encr->bytesToRead = 0;
    }

    if ((scratchInbuf = (UcharPtr) MemNew(bytesRead + encr->numDeferredBytes)) ==
        NULL)
        return 0;
    ip = scratchInbuf;

    MemCpy ((CharPtr) scratchInbuf, (CharPtr) encr->deferredData,
            encr->numDeferredBytes);
    MemCpy ((CharPtr) &scratchInbuf[encr->numDeferredBytes], buf, bytesRead);
    bytesRead += encr->numDeferredBytes;
    encr->numDeferredBytes = 0;
    retval = 0;
    done = FALSE;

    while (bytesRead > 0 && ! done)
    {
        switch (encr->state) {
        case ENCR_DES_STATE_IDLE:
            if (bytesRead == 1)
            {
                encr->realDataLeft = *ip++;
                encr->state = ENCR_DES_STATE_GOT1LENBYTE;
                done = TRUE;
                bytesRead--;
            } else {
                encr->realDataLeft = ip[0] * 256 + ip[1];
                encr->bytesToRead = ((encr->realDataLeft + 7) / 8) * 8;
                encr->numDeferredBytes = 0;
                ip += 2;
                bytesRead -= 2;
                encr->state = ENCR_DES_STATE_INSTREAM;
            }
            break;
        case ENCR_DES_STATE_GOT1LENBYTE:
            encr->realDataLeft = encr->realDataLeft * 256 + *ip++;
            encr->bytesToRead = ((encr->realDataLeft + 7) / 8) * 8;
            encr->numDeferredBytes = 0;
            bytesRead--;
            encr->state = ENCR_DES_STATE_INSTREAM;
            break;
        case ENCR_DES_STATE_INSTREAM:
            avail = MIN(encr->bytesToRead, bytesRead);
            roomInBuffer = (Int2) (len - retval);
            bytesToDecrypt = (MIN(roomInBuffer, avail) / 8) * 8;
            if (bytesToDecrypt <= 0)
            {
                if (roomInBuffer >= MAX(encr->realDataLeft, 0) && avail >= 8)
                { /* we can squeeze this in, and there's enough to process */
                    bytesToDecrypt = 8;
                } else {
                    done = TRUE;
                    break;
                }
            }
            scratchOutbuf = (UcharPtr) MemNew(bytesToDecrypt);
            DES_CBCUpdate ((DES_CBC_CTX PNTR) encr->desReadContext,
                           scratchOutbuf, ip, bytesToDecrypt);
            bytesToCopy = MIN(encr->realDataLeft, bytesToDecrypt);
            MemCpy (buf, (CharPtr) scratchOutbuf, bytesToCopy);
            MemFree (scratchOutbuf);
            buf += bytesToCopy;
            retval += bytesToCopy;
            ip += bytesToDecrypt;
            bytesRead -= bytesToDecrypt;
            encr->bytesToRead -= bytesToDecrypt;
            encr->realDataLeft -= bytesToDecrypt;
            if (encr->bytesToRead <= 0)
            {
                encr->state = ENCR_DES_STATE_IDLE;
                encr->numDeferredBytes = 0;
                encr->realDataLeft = 0;
                encr->bytesToRead = 0;
            }
            break;
        }
    }

    if (bytesRead > 0)
    {
        encr->numDeferredBytes = bytesRead;
        if (bytesRead > sizeof(encr->deferredData))
        {
            ErrPostEx(SEV_ERROR, 0, 0, "Too much deferred decryption data %d bytes", bytesRead);
        } else {
            MemCpy ((CharPtr) encr->deferredData, (CharPtr) ip, bytesRead);
        }
    }

    MemFree (scratchInbuf);

    return retval;
}


/*
 * Purpose:     DES read filter
 *
 * Parameters:
 *   mhvoid     Pointer to message handle data structure
 *   buf        Input buffer containing encrypted data, and output plaintext
 *   bytesRead  Length of input data in buf
 *   len        Max # of bytes which will fit in buf (requested read length)
 *   extra_buf  unused, but required for NI_ReadFilt declaration
 *   extra_buf_len  unused, but required for NI_ReadFilt declaration
 *
 * Returns:
 *                The length of the resulting plaintext which has been processed
 *
 * Description:
 *              Uses DoDesRead() to process input data, and returns to caller
 */

static Int4 LIBCALL
DesReadFilt(VoidPtr mhvoid, CharPtr buf, int bytesRead, int len, CharPtr PNTR extra_buf, Int4Ptr extra_buf_len)
{
    NI_HandPtr mh = (NI_HandPtr) mhvoid;

    if (bytesRead <= 0 || buf == NULL || mh == NULL ||
        mh->encryption == NULL)
        return 0;
    return DoDesRead(mh->encryption, buf, bytesRead, len);
}


/*
 * Purpose:     Setup DES encryption for this message handle
 *
 * Parameters:
 *   mh         Pointer to message handle data structure
 *   desKey     DES key to be used for the life of this session
 *
 * Returns:
 *                TRUE if setup was successful, FALSE otherwise
 *
 * Description:
 *              Allocates an encryption data structure to attach to the message
 *              handle, as well as the RSAREF data structures for both reading
 *              and writing data.  Note that the read and write data structures
 *              are each handling an independent half-duplex channel, i.e,
 *              either client->server or server->client data.
 *
 * Note:
 *              A caller which uses this function must also call
 *              NI_DestroyEncrStruct() when it is time to destroy the message
 *              handle.
 */

NLM_EXTERN Boolean LIBCALL
NI_SetupDESEncryption(NI_HandPtr mh, UcharPtr desKey)
{
    Uchar iv[8];
    NI_EncrDataPtr encr;

    if (mh->encryption != NULL)
        return FALSE;
    if ((encr = MemNew(sizeof(*encr))) == NULL)
        return FALSE;
    mh->encryption = encr;
    encr->encrType = ENCR_DES_TYPE; /* the only possibility, for now */
    encr->state = ENCR_DES_STATE_IDLE;
    encr->write_filter = DesWriteFilt;
    encr->read_filter = DesReadFilt;
    encr->desWriteContext = (DES_CBC_CTX PNTR) MemNew(sizeof(DES_CBC_CTX));
    MemSet((CharPtr) iv, '\0', sizeof(iv));
    DES_CBCInit((DES_CBC_CTX PNTR) encr->desWriteContext, desKey, iv, TRUE);
    encr->desReadContext = (DES_CBC_CTX PNTR) MemNew(sizeof(DES_CBC_CTX));
    MemSet((CharPtr) iv, '\0', sizeof(iv));
    DES_CBCInit((DES_CBC_CTX PNTR) encr->desReadContext, desKey, iv, FALSE);

    return TRUE;
}


/*
 * Purpose:     Seed the random number generator if necessary
 *
 * Description:
 *              If this function has not previously been called, it seeds the NCBI
 *              random number generator with the best pseudo-random data available
 *              on all systems, namely the current time in seconds, and a notion
 *              of the application's process ID.  A high-granularity time is
 *              also included when available.
 *
 * Note:
 *              It would be helpful to have another NCBI function which returns
 *              the highest-granularity time available, e.g., many systems have
 *              microsecond and/or ticks available.  Any other creative data
 *              available on this system (e.g., for UNIX systems, how many inodes
 *              are in use in the root filesystem of this computer) would also be
 *              helpful to help defeat malicious attempts to crack the security
 *              of this encryption subsystem.
 */

static void
SetRandomSeed(void)
{
    static Boolean inited = FALSE;
    Int4 highGranularity = 0;
#ifdef OS_UNIX
    struct timeval tv;

    gettimeofday(&tv, NULL);
    highGranularity = tv.tv_usec;
#endif
#ifdef OS_MAC
    highGranularity = clock();
#endif
#ifdef WIN_MSWIN
    highGranularity = (Int4) GetCurrentTime();
#endif
    
    if (! inited)
    {
        RandomSeed ((long) (GetSecs() | Nlm_GetAppProcessID() | highGranularity));
        inited = TRUE;
    }
}



/*
 * Purpose:     Initialize an RSAREF random data structure used for RSA encryption
 *
 * Parameters:
 *     randomStruct   The data structure to be populated
 *
 * Description:
 *              Seed the random number generator if necessary, create a random
 *              R_RANDOM_STRUCT data structure, and populate it with
 *              pseudo-random data.  When available, data from the NCBI config.
 *              file is exclusive-ORed into each pseudo-random number, so
 *              that an RSA key cannot subsequently be broken by trying all
 *              2^^32 possible values of the random number generator.
 */

static void InitRandomStruct (randomStruct)
R_RANDOM_STRUCT *randomStruct;
{
    unsigned int bytesNeeded;
    long ran;
    Int4 seednum = 0;
    Char buf[22];
    Char moredata[10];

    SetRandomSeed();
    R_RandomInit (randomStruct);

    while (1) {
        R_GetRandomBytesNeeded (&bytesNeeded, randomStruct);
        if (bytesNeeded == 0)
            break;

        ran = RandomNum();
        sprintf (buf, "CONFOUND_%ld", (long) seednum);
        GetAppParam("NCBI", "NET_SERV", buf, "0", moredata, sizeof moredata);
        ran ^= atoi(moredata);
        seednum++;
        R_RandomUpdate (randomStruct, (UcharPtr) &ran, MIN(bytesNeeded, sizeof ran));
    }
}



/*
 * Purpose:     Convert pub encryption key from RSAREF format to internal NCBI fmt
 *
 * Parameters:
 *     publicKey    Key in RSAREF format
 *
 * Returns:
 *                  Key in NCBI format, or NULL if an error occurred
 *
 * Description:
 *              Convert RSAREF public key data structure into a form which is 
 *              suitable for being stored and transmitted in ASN.1
 */

static NI_PubKeyPtr
PubKeyToInternalFormat(R_RSA_PUBLIC_KEY PNTR publicKey)
{
    NI_PubKeyPtr retval = (NI_PubKeyPtr) MemNew(sizeof(*retval));

    if (retval == NULL || publicKey == NULL)
        return NULL;
    retval->bits = publicKey->bits;
    retval->modulus = BSNew(sizeof(publicKey->modulus));
    BSWrite(retval->modulus, (VoidPtr) publicKey->modulus, sizeof(publicKey->modulus));
    retval->exponent = BSNew(sizeof(publicKey->exponent));
    BSWrite(retval->exponent, (VoidPtr) publicKey->exponent, sizeof(publicKey->exponent));
    return retval;
}



/*
 * Purpose:     Convert pub encryption key from internal NCBI format to RSAREF fmt
 *
 * Parameters:
 *     internal     Key in NCBI format
 *
 * Returns:
 *                  Key in RSAREF format, or NULL if an error occurred
 *
 * Description:
 *              Produce RSAREF public key data structure from a form which is 
 *              suitable for being stored and transmitted in ASN.1
 */

static R_RSA_PUBLIC_KEY PNTR
InternalToPubKeyFormat(NI_PubKeyPtr internal)
{
    R_RSA_PUBLIC_KEY PNTR retval;

    if (internal == NULL || internal->modulus == NULL ||
        internal->exponent == NULL)
        return NULL;
    retval = (R_RSA_PUBLIC_KEY PNTR) MemNew(sizeof(*retval));
    retval->bits = internal->bits;
    BSSeek (internal->modulus, 0, SEEK_SET);
    BSSeek (internal->exponent, 0, SEEK_SET);
    BSRead(internal->modulus, retval->modulus, sizeof(retval->modulus));
    BSRead(internal->exponent, retval->exponent, sizeof(retval->exponent));
    return retval;
}


/*
 * Purpose:     Compare two public encryption keys for equality
 *
 * Parameters:
 *     x            Public key #1
 *     y            Public key #2
 *
 * Returns:
 *                  TRUE if keys match, FALSE otherwise
 *
 * Description:
 *              Converts keys into RSAREF format, because these are flat data
 *              structures and hence easy to compare. 
 */

NLM_EXTERN Boolean LIBCALL
NI_PubKeysEqual(NI_PubKeyPtr x, NI_PubKeyPtr y)
{
    R_RSA_PUBLIC_KEY PNTR xRsa = InternalToPubKeyFormat(x);
    R_RSA_PUBLIC_KEY PNTR yRsa = InternalToPubKeyFormat(y);
    Boolean retval;

    if (xRsa == NULL && yRsa== NULL)
        return TRUE;
    if (xRsa == NULL || yRsa== NULL)
    {
        retval = FALSE;
    } else {
        retval = MemCmp((CharPtr) xRsa, (CharPtr) yRsa, sizeof (*xRsa)) == 0;
    }
    MemFree (xRsa);
    MemFree (yRsa);
    return retval;
}



/*
 * Purpose:     Pseudo-randomly generate an 8-byte DES key
 *
 * Parameters:
 *     desKey       Resulting DES key
 *
 * Description:
 *              Generates 8-byte DES key, grabbing 2 bytes from each of 4 pseudo-
 *              randomly generated long integers.
 *
 * Note:
 *              Only 2 bytes are used from the long integer, because it's
 *              conceivable that a machine exists without 4-byte long integers.
 *              In hindsight, sizeof(long) could be used to determine how much to
 *              use, but this is a reasonable implementation.
 */

NLM_EXTERN void
NI_GenerateDESKey(UcharPtr desKey)
{
    Int2 i;
    long ran;

    SetRandomSeed();
    for (i = 0; i < 8;)
    {
        ran = RandomNum();
        desKey[i++] = (Uchar) (ran & 255);
        desKey[i++] = (Uchar) ((ran >> 8) & 255);
    }
}



/*
 * Purpose:     Generate pub encryption public + private keys, and write to files
 *
 * Parameters:
 *     bits        Length of key modulus in bits, within a restricted range
 *     pubAip      AsnIoPtr to where public-key should be stored
 *     privFp      File pointer to where private key should be stored 
 *
 * Returns:
 *              TRUE if operations were successful, FALSE otherwise
 *
 * Description:
 *              Generates public and private keys using RSAREF function, and then
 *              output to AsnIoPtr and file pointer.
 *
 * Note:
 *              An AsnIoPtr is used for the public key, because the public key
 *              must be transmittable in a canonical format.  Since private keys
 *              are never transmitted, a single block of memory is used for
 *              private keys, and private keys are stored in a single chunk on
 *              disk.  This reduces the number of special data structures and
 *              ASN.1 object loaders which needed to be constructed to add
 *              encryption to NCBI Network Services.
 */

NLM_EXTERN Boolean LIBCALL
NI_GenAndWritePEMKeys(Int2 bits, AsnIoPtr pubAip, FILE *privFp)
{
    R_RSA_PUBLIC_KEY publicKey;
    R_RSA_PRIVATE_KEY privateKey;
    R_RSA_PROTO_KEY protoKey;
    R_RANDOM_STRUCT randomStruct;
    int retval;
    AsnModulePtr amp;
    AsnTypePtr pubAtp;
    NI_PubKeyPtr internalPub;
    NI_HandPtr dummyHand;

    if (bits < MIN_RSA_MODULUS_BITS || bits > MAX_RSA_MODULUS_BITS)
        return FALSE;
    if (pubAip == NULL || privFp == NULL)
        return FALSE;
    /* create a dummy message handle to ensure that ASN.1 is loaded */
    dummyHand = (NI_HandPtr) MsgMakeHandle(FALSE);
    MsgDestroyHandle(dummyHand);
    if ((amp = AsnAllModPtr()) == NULL)
        return FALSE;
    if ((pubAtp = AsnTypeFind(amp, "RSA-Pubkey")) == NULL)
        return FALSE;
    InitRandomStruct (&randomStruct);
    protoKey.bits = bits;
    protoKey.useFermat4 = 1;
    retval = R_GeneratePEMKeys(&publicKey, &privateKey, &protoKey, &randomStruct);
    if (retval != 0)
    {
        ErrPostEx(SEV_ERROR, 0, 0, "Error when generating PEM keys %d", retval);
    }
    internalPub = PubKeyToInternalFormat(&publicKey);
    NI_WritePubKey(pubAip, pubAtp, (NIPubKeyPtr) internalPub);

    FileWrite(&privateKey, sizeof(privateKey), 1, privFp);

    return TRUE;
}



/*
 * Purpose:     Write a public key to a standard location on client machines
 *
 * Parameters:
 *
 *   pub           The public-key to be written to standard file
 *
 * Returns:
 *              TRUE if operations were successful, FALSE otherwise
 *
 * Description:
 *              Writes public-key to a file in the DATA directory.
 *              Alternatively, the public-key could be stored anywhere ...
 *              originally it was stored in the NCBI configuration file. 
 */

NLM_EXTERN Boolean LIBCALL
NI_WritePubKeyToConfig (NI_PubKeyPtr pub)
{
    Char fname[PATH_MAX];
    AsnIoPtr aip;
    AsnModulePtr amp;
    AsnTypePtr pubAtp;
    NI_HandPtr dummyHand;

    if (pub == NULL)
    {
        return FALSE;
    }

    /* create a dummy message handle to ensure that ASN.1 is loaded */
    dummyHand = (NI_HandPtr) MsgMakeHandle(FALSE);
    MsgDestroyHandle(dummyHand);
    if ((amp = AsnAllModPtr()) == NULL)
        return FALSE;
    if ((pubAtp = AsnTypeFind(amp, "RSA-Pubkey")) == NULL)
        return FALSE;

    if (! FindPath("ncbi", "ncbi", "data", fname, sizeof (fname)))
    {
        ErrPost(CTX_NCBIOBJ, 1, "FindPath failed");
        return FALSE;
    }

    StringCat(fname, "pubkey.enc");

    if ((aip = AsnIoOpen(fname, "w")) == NULL)
    {
        return FALSE;
    }
    NI_WritePubKey(aip, pubAtp, (NIPubKeyPtr) pub);
    AsnIoClose(aip);
    return TRUE;
}

/*
 * Purpose:     Read a public key from a standard configuration location
 *
 * Returns:
 *              A pointer to the allocated public-key, or NULL if failed
 *
 * Description:
 *              Reads public-key from a file in the DATA directory.
 *              Alternatively, the public-key could be stored anywhere ...
 *              originally it was stored in the NCBI configuration file. 
 */

NLM_EXTERN NI_PubKeyPtr LIBCALL
NI_ReadPubKeyFromConfig (void)
{
    Char fname[PATH_MAX];
    AsnIoPtr aip;
    NI_PubKeyPtr pub;
    NIPubKeyPtr pub2;
    FILE *fp;

    if (! FindPath("ncbi", "ncbi", "data", fname, sizeof (fname)))
    {
        ErrPost(CTX_NCBIOBJ, 1, "FindPath failed");
        return NULL;
    }

    StringCat(fname, "pubkey.enc");
    /* try opening the file first, to suppress AsnIoOpen messages */
    if ((fp = FileOpen(fname, "r")) == NULL)
    {
        return NULL;
    } else {
        FileClose(fp);
    }
    if ((aip = AsnIoOpen(fname, "r")) == NULL)
    {
        FileRemove(fname);
        return NULL;
    }
    /* pub = (NI_PubKeyPtr) NI_MakePubKey(); */
    pub2 = NI_MakePubKey();
    pub = (NI_PubKeyPtr) pub2;
    if (NI_ReadPubKey(aip, NULL, (NIPubKeyPtr) pub) < 0)
    {
        NI_DestroyPubKey((NIPubKeyPtr) pub);
        pub = NULL;
    }
    AsnIoClose(aip);

    return pub;
}

/*
 * Purpose:     Make a copy of a public key
 *
 * Parameters:
 *   orig          The key to be copied
 *
 * Returns:
 *              A pointer to the copy of the public-key, or NULL if failed
 *
 * Description:
 *              Makes a copy of a public key
 */

NLM_EXTERN NI_PubKeyPtr LIBCALL
NI_PubKeyDup (NI_PubKeyPtr orig)
{
    NI_PubKeyPtr dup;

    if (orig == NULL)
        return NULL;
    dup = MemNew(sizeof(*dup));
    dup->bits = orig->bits;
    dup->modulus = BSDup(orig->modulus);
    dup->exponent = BSDup(orig->exponent);
    return dup;
}
    

/*
 * Purpose:     Load a private-key from the specified file pointer
 *
 * Parameters:
 *   fp            File pointer from which to read private key
 *   privKeyLenPtr Pointer to where the length of private key may be stored
 *
 * Returns:
 *              A pointer to the resulting data structure, or NULL if unsuccessful
 *
 * Description:
 *              Reads private key from data file.
 */

NLM_EXTERN VoidPtr LIBCALL
NI_LoadPrivKey(FILE *fp, Int2Ptr privKeyLenPtr)
{
    R_RSA_PRIVATE_KEY PNTR privKey;

    privKey = (R_RSA_PRIVATE_KEY PNTR) MemNew(sizeof(*privKey));

    FileRead(privKey, sizeof(*privKey), 1, fp);

    if (privKeyLenPtr != NULL)
    {
        *privKeyLenPtr = sizeof(*privKey);
    }

    return privKey;
}

/*
 * Purpose:     Perform public-key decryption
 *
 * Parameters:
 *   pKey          Private key
 *   plainText     Pointer to resulting plaintext
 *   cipherText    Ciphertext to be decrypted
 *   cipherTextLen Length of cipherText
 *
 * Returns:
 *              The length of resulting plaintext, or a negative error code
 *
 * Description:
 *              Decrypts the specified ciphertext using the specified private
 *              key.  Subsequently resizes the resulting plaintext to be only
 *              as large as is needed.
 *
 * Note:
 *              The caller must free the pointer to the resulting plaintext.
 */

NLM_EXTERN Int2 LIBCALL
NI_PubKeyDecrypt(VoidPtr pKey, UcharPtr PNTR plainText, UcharPtr cipherText, Int2 cipherTextLen)
{
    R_RSA_PRIVATE_KEY PNTR privKey = (R_RSA_PRIVATE_KEY PNTR) pKey;
    int plainTextLen;
    UcharPtr pText1, pText2;

    if (pKey == NULL || plainText == NULL || cipherText == NULL)
        return -1;

    *plainText = NULL;
    /* plain text is certainly shorter than ciphertext */
    pText1 = (UcharPtr) MemNew(cipherTextLen);

    if (RSAPrivateDecrypt(pText1, &plainTextLen, cipherText, cipherTextLen, privKey) != 0)
    {
        MemFree (pText1);
        return -2;
    }
    pText2 = (UcharPtr) MemDup(pText1, plainTextLen);
    MemFree (pText1);
    *plainText = pText2;
    return ((Int2) plainTextLen);
}


/*
 * Purpose:     Perform public-key encryption
 *
 * Parameters:
 *   pub           Public key
 *   plainText     Plaintext to be encrypted
 *   plainTextLen  Length of plainText
 *   cipherText    Pointer to resulting ciphertext
 *
 * Returns:
 *              The length of resulting ciphertext, or a negative error code
 *
 * Description:
 *              Encrypts the specified plaintext using the specified public
 *              key.  Subsequently resizes the resulting ciphertext to be only
 *              as large as is needed.
 *
 * Note:
 *              The caller must free the pointer to the resulting ciphertext.
 */

NLM_EXTERN Int2 LIBCALL
NI_PubKeyEncrypt(NI_PubKeyPtr pub, UcharPtr plainText, Int2 plainTextLen, UcharPtr PNTR cipherText)
{
    R_RSA_PUBLIC_KEY PNTR pubKeyPtr;
    R_RANDOM_STRUCT randomStruct;
    UcharPtr cipher, cipher2;
    int cipherTextLen = 0;

    if (pub == NULL || plainText == NULL || cipherText == NULL)
    {
        return -1;
    }
    *cipherText = NULL;
    if ((pubKeyPtr = InternalToPubKeyFormat(pub)) == NULL)
    {
        return -2;
    }
    cipher = (UcharPtr) MemNew(plainTextLen * 2 + 64);
    InitRandomStruct (&randomStruct);
    if (RSAPublicEncrypt(cipher, &cipherTextLen, plainText, plainTextLen, pubKeyPtr, &randomStruct) != 0)
    {
        MemFree (cipher);
        MemFree (pubKeyPtr);
        return -3;
    }
    MemFree (pubKeyPtr);
    /* reduce to the proper length */
    cipher2 = (UcharPtr) MemDup(cipher, cipherTextLen);
    MemFree (cipher);
    *cipherText = cipher2;
    return ((Int2) cipherTextLen);
}

/*
 * Purpose:     Free an encryption data structure, and erase secret data
 *
 * Parameters:
 *   encr          Encryption data structure to be destroyed
 *
 *
 * Description:
 *              Frees an encryption data structure, and erases secret data
 *              which could be used by a hostile party to break the encryption.
 *              The caller must free the pointer to the resulting ciphertext.
 */

NLM_EXTERN void
NI_DestroyEncrStruct (NI_EncrDataPtr encr)
{
    if (encr == NULL)
        return;
    if (encr->desReadContext != NULL)
    {
        /* clear this memory for security reasons */
        MemSet(encr->desReadContext, '\0', sizeof(DES_CBC_CTX));
        MemFree(encr->desReadContext);
    }
    if (encr->desWriteContext != NULL)
    {
        /* clear this memory for security reasons */
        MemSet(encr->desWriteContext, '\0', sizeof(DES_CBC_CTX));
        MemFree(encr->desWriteContext);
    }
    MemFree(encr);
}

/*
 * Purpose:     Indicates whether encryption is available
 *
 * Returns:     Always TRUE for this file, always FALSE for stub file
 *
 *
 * Description:
 *              Indicates to the caller whether encryption is available, and
 *              whether or not it is safe to call the other encryption
 *              functions.
 */

NLM_EXTERN Boolean LIBCALL
NI_EncrAvailable(void)
{
    return TRUE;
}
