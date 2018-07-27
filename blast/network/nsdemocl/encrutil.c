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
* File Name:    encrutil.c
*
* Author:       Epstein
*
* Version Creation Date:        2/14/94
*
* $Revision: 6.0 $
*
* File Description:
*   Encryption utility standalone program, to perform many simple encryption
*   tasks and validate encryption operation on a given machine.
*
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* RCS Modification History:
* $Log: encrutil.c,v $
* Revision 6.0  1997/08/25 18:40:10  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 14:13:55  ostell
* Set to revision 5.0
*
 * Revision 4.0  1995/07/26  13:57:04  ostell
 * force revision to 4.0
 *
 * Revision 1.5  1995/05/17  17:52:58  epstein
 * add RCS log revision history
 *
*/

#include <ncbi.h>
#include <asn.h>
#include <ncbinet.h>

#define RSA_ENCR_TEXT "The quick"
#define DES_ENCR_TEXT "The quick brown fox jumped over the lazy dog"

#define NEEDED_CONFOUND 500

Args myargs[] = {
        {"Bits in public key modulus", "508","508", "1024", TRUE,'b',ARG_INT,0.0,0,NULL},
        {"pUblic key file", "pubkey.enc", NULL, NULL, TRUE, 'u', ARG_STRING,0.0,0,NULL},
        {"pRivate key file", "dispd.privkey", NULL, NULL, TRUE, 'r', ARG_STRING,0.0,0,NULL},
        {"password for des Encryption of private key", NULL, NULL, NULL, TRUE, 'e', ARG_STRING,0.0,0,NULL},
        {"perform Validation", "F", NULL, NULL, TRUE, 'v', ARG_BOOLEAN, 0.0,0,NULL},
        {"Arbitrary file used to confound key cracking", NULL, NULL, NULL, TRUE, 'a', ARG_STRING, 0.0,0,NULL},
        {"additional Help", "F", NULL, NULL, TRUE, 'h', ARG_BOOLEAN, 0.0,0,NULL}};

static void
PrintHelp(void)
{
    printf ("\nThis program generates public-encryption keys for use in the NCBI network\n");
    printf ("services paradigm.  It can also be used to validate the operation of DES\n");
    printf ("and public-key encryption on the computer where this program is run.\n\n");
    printf ("If the '-v' option (validation) is specified, the program randomly generates a\n");
    printf ("DES key and a public-private RSA pair, and tests encryption and decryption under\n");
    printf ("both DES and RSA.  The normal output of keys is not performed in this case.\n\n");
    printf ("If the '-e' (password for DES Encryption of private key) option is used, the\n");
    printf ("specified password is encoded (but not encrypted!) and displayed upon the\n");
    printf ("standard output, while the private RSA key is encrypted using that encoded DES\n");
    printf ("key and written to the private key output file.  This option provides a way to\n");
    printf ("store a private key in a file without fear that compromise of that file\n");
    printf ("will immediately result in compromise of the private key.\n\n");
}

static void
EncodePassword(CharPtr encoding, CharPtr desPassword)
{
   Uchar input[12];
   UcharPtr inp = input;
   UcharPtr out = (UcharPtr) encoding;
   Int2 i;
   Int2 j;
   Uint4 temp;
   Uchar ch;

   StrNCpy((CharPtr) input, desPassword, sizeof(input));
   for (j = 0; j < 3; j++)
   {
      temp = 0;

      for (i = 0; i < 4; i++)
      {
          ch = *inp++;
          if (ch >= 128)
              ch -= 128;
          if (ch >= 32)
              ch -= 32;
          if (ch >= 64)
              ch -= 32;
          /* ch is now normalized between 0 and 63 */
          temp = temp * 64 + ch;
      }
      *out++ = temp / 65536;
      temp /= 256;
      *out++ = temp / 256;
      *out++ = temp & 255;
   }
}


Int2
Main()
{
   int Numarg = sizeof(myargs)/sizeof(Args);
   AsnIoPtr pubAip;
   NI_PubKeyPtr pub1;
   VoidPtr privKey;
   FILE *fp;
   UcharPtr text;
   UcharPtr cipherText = NULL;
   UcharPtr plainText = NULL;
   Int2 cLen, newlen;
   CharPtr privFileName;
   CharPtr pubFileName;
   CharPtr desPassword = NULL;
   int bits;
   Char c;
   Int2 numConfoundChars;
   Int2 numConfoundLongWords;
   Char databuf[9];
   Char strbuf[20];
   Uint4 longWord;

   if (! NI_EncrAvailable())
   {
        printf ("The NCBI toolkit does not support encryption in the manner in which this\n");
        printf ("executable was prepared.  To support encryption, be sure that ni_encr.c appears\n");
        printf ("in the link, rather than ni_encrs.c\n");
        return -1;
   }

   if ( ! GetArgs("Encryption utilities $Revision: 6.0 $", Numarg, myargs))
        return 1;

   if (myargs[6].intvalue)
   {
       PrintHelp();
       return 0;
   }

   bits = myargs[0].intvalue;
   pubFileName = myargs[1].strvalue;
   privFileName = myargs[2].strvalue;
   if (myargs[3].strvalue != NULL && myargs[3].strvalue[0] != '\0')
   { /* encode an existing private key using the specified password */
       Int2 privkeylen;
       Char encoding[9];
       Int4 newLen;
       NI_HandPtr encrHP;
       NI_EncrDataPtr encr;
       CharPtr  encryptedPrivKey;
       Int2 i;

       desPassword = myargs[3].strvalue;
       EncodePassword(encoding, desPassword);
       fp = FileOpen(privFileName, "r");
       privKey = NI_LoadPrivKey(fp, &privkeylen);
       encrHP = MsgMakeHandle(FALSE);
       NI_SetupDESEncryption(encrHP, encoding);
       encr = encrHP->encryption;
       encryptedPrivKey = (CharPtr) MemNew(privkeylen + 100);
       newLen = encr->write_filter(encrHP, privKey, privkeylen, encryptedPrivKey);
       MsgDestroyHandle(encrHP);
       FileClose(fp);
       fp = FileOpen(privFileName, "w");
       FileWrite(encryptedPrivKey, newLen, 1, fp);
       MemFree (encryptedPrivKey);
       FileClose(fp);

       /* print out the environment variable which must be used for decryption */
       printf ("NI_DES_FOR_PRIVKEY=");
       for (i = 0; i < 8; i++)
           printf ("%02X", encoding[i] & 255);
       printf ("\n");
       return 0;
   }


#ifdef NOT_REALPROG
   pubAip = AsnIoOpen("dispd.pubkeytest", "r");
   pub1 = (NI_PubKeyPtr) NI_MakePubKey();
   NI_ReadPubKey(pubAip, NULL, pub1);
   AsnIoClose(pubAip);
   if ((cLen = NI_PubKeyEncrypt(pub1, text, StrLen(text) + 1, &cipherText)) < 0)
   {
       fprintf (stderr, "Encryption failed (%d)\n", cLen);
       exit (1);
   }
   fp = FileOpen("dispd.privkeytest", "r");
   privKey = NI_LoadPrivKey(fp, NULL);
   FileClose(fp);
   if ((newlen = NI_PubKeyDecrypt(privKey, &plainText, cipherText, cLen)) < 0)
   {
       fprintf (stderr, "Decryption failed (%d)\n", newlen);
       exit (1);
   }
   MemFree(privKey);
   fprintf (stderr, "Decrypted text is %s, length is %d\n", plainText, newlen);
   MemFree (plainText);
   MemFree (cipherText);
   NI_DestroyPubKey (pub1);

#else
   if (myargs[5].strvalue != NULL && myargs[5].strvalue[0] != '\0')
   {
       if ((fp = FileOpen(myargs[5].strvalue, "r")) == NULL)
       {
           fprintf (stderr, "Unable to open `confounder' file %s for reading\n",
                    myargs[5].strvalue);
           exit (1);
       }
       numConfoundChars = 0;
       numConfoundLongWords = 0;
       longWord = 0;
       while ((c = getc(fp)) != EOF && numConfoundLongWords < NEEDED_CONFOUND)
       {
           if (c != NULLB)
           {
               longWord = longWord * 256 + c;
               numConfoundChars++;
               if (numConfoundChars == 4)
               {
                   numConfoundChars = 0;
                   sprintf (strbuf, "CONFOUND_%d", numConfoundLongWords++);
                   sprintf (databuf, "%ld", longWord);
                   TransientSetAppParam ("NCBI", "NET_SERV", strbuf, databuf);
               }
           }
       }
       FileClose(fp);
       if (numConfoundLongWords < NEEDED_CONFOUND)
       {
           fprintf (stderr, "Confounder file %s contains too few non-zero data bytes\n",
                    myargs[5].strvalue);
           exit (1);
       }
   }
       
   pubAip = AsnIoOpen(pubFileName, "w");
   fp = FileOpen(privFileName, "w");
   NI_GenAndWritePEMKeys(bits, pubAip, fp);
   AsnIoClose(pubAip);
   FileClose(fp);
   return 0;
#endif
}
