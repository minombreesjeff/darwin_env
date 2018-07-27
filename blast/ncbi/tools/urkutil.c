static char const rcsid[] = "$Id: urkutil.c,v 6.5 2003/05/30 17:25:38 coulouri Exp $";

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
* File Name: urkutil.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.5 $
*
* File Description: urk utilities
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: urkutil.c,v $
* Revision 6.5  2003/05/30 17:25:38  coulouri
* add rcsid
*
* Revision 6.4  1999/10/18 18:03:40  madden
* Remove Entrez calls from tools lib function
*
* Revision 6.3  1999/01/27 15:08:42  kuzio
* ErrorDescString
*
* Revision 6.2  1998/09/16 14:20:10  kuzio
* testing cvs logging on boilerplate (?)
*
*
* ==========================================================================
*/

#include <accentr.h>
#include <tofasta.h>
#include <urkutil.h>

extern CharPtr FastaTitle (BioseqPtr bsp, CharPtr pretext, CharPtr posttext)
{
  CharPtr   title = NULL;
  CharPtr   prep, postp, predum = "", postdum = "";
  Char      idbuf[L_ID], defbuf[L_DEF];
  Int4      prelen = 0, postlen = 0;
  SeqIdPtr  sip;

  if (bsp != NULL)
  {
    if ((sip = bsp->id) != NULL)
    {
      if (pretext != NULL)
      {
        prelen = StrLen (pretext);
        prep = pretext;
      }
      else
      {
        prep = predum;
      }
      if (posttext != NULL)
      {
        postlen = StrLen (posttext);
        postp = posttext;
      }
      else
      {
        postp = postdum;
      }
      if ((title = (CharPtr) MemNew (sizeof (Char) * (L_T+prelen+postlen)))
          != NULL)
      {
        switch (sip->choice)
        {
         case SEQID_LOCAL:
          idbuf[0] = '\0';
          if (sip->data.ptrvalue != NULL)
            StrNCpy (idbuf, ((ObjectIdPtr) sip->data.ptrvalue)->str, L_ID-1);
          idbuf[L_ID-1] = '\0';
          break;
         default:
          FastaId (bsp, idbuf, L_ID);
          break;
        }
        FastaDefLine (bsp, defbuf, L_DEF, NULL, NULL, 0);
        sprintf (title, "%s%s %s%s", prep, idbuf, defbuf, postp);
      }
    }
  }
  return title;
}

extern CharPtr ErrorDescString (SeqIdPtr sip)
{
  SeqIdPtr    bestid;
  CharPtr     errbuf;

  bestid = SeqIdFindBest(sip, SEQID_GI);

  errbuf = (CharPtr) MemNew ((size_t) (sizeof (Char) * 32));
  SeqIdWrite (bestid, errbuf, PRINTID_FASTA_LONG, 32-1);

  return errbuf;
}
