static char const rcsid[] = "$Id: urkptpf.c,v 6.33 2003/05/30 17:25:38 coulouri Exp $";

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
* File Name: urkptpf.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.33 $
*
* File Description: patterns and profiles
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: urkptpf.c,v $
* Revision 6.33  2003/05/30 17:25:38  coulouri
* add rcsid
*
* Revision 6.32  1999/07/13 14:08:46  sicotte
* prefix functions SeqAlignSort* with URK to resolve toolkit conflicts
*
* Revision 6.31  1998/12/23 21:34:03  kuzio
* SeqPortStuff
*
* Revision 6.30  1998/11/16 14:29:53  kuzio
* flagBoundaryCondition
*
* Revision 6.29  1998/09/21 14:38:24  kuzio
* check for cr in comments
*
* Revision 6.28  1998/09/16 18:03:36  kuzio
* cvs logging
*
*
* ==========================================================================
*/

#include <objres.h>
#include <objseq.h>
#include <sequtil.h>
#include <seqport.h>
#include <urkptpf.h>

/* Patterns and Profiles */

static Uint1Ptr GetSeqChunk (SeqPortPtr spp, Int4 start, Int4 len,
                             Int4 chunk)
{
  Uint1Ptr seqhead, sequence;
  Int4     i, size;

  if (start >= len)
    return NULL;

  if ((start + chunk) > len)
    size = len - start;
  else
    size = chunk;

  seqhead = sequence = MemNew (sizeof (Uint1) * (size_t) (size+1));
  if (seqhead == NULL)
    return seqhead;

  SeqPortSeek (spp, start, SEEK_SET);
  for (i = 0; i < size; i++)
    *sequence++ = (Uint1) SeqPortGetResidue (spp);
  *sequence = 0;

  return seqhead;
}

static SeqAlignPtr PatSeqAlign (Int4 start, Int4 length, Uint1 strand,
                                Int4 mismatch, SeqIdPtr sip,
                                Int2 topology)
{
  SeqAlignPtr sap;
  SeqLocPtr   slp;
  StdSegPtr   ssp;
  Boolean     flagDone;

  flagDone = FALSE;
  if ((sap = SeqAlignNew ()) != NULL)
  {
    sap->type = 255;      /* other */
    sap->segtype = 3;     /* std */
    sap->dim = 1;         /* for embedded info; is real; is not dummy */
    start -= (length-1);
    if ((slp = SeqLocIntNew (start, start+length-1, strand, sip)) != NULL)
    {
      if ((sap->score = ScoreNew ()) != NULL)
      {
        sap->score->choice = 1;
        sap->score->value.intvalue = mismatch;
      }
      if ((ssp = StdSegNew ()) != NULL)
      {
        ssp->dim = topology;
        ssp->ids = SeqIdDup (sip);
        ssp->loc = slp;
        sap->segs = ssp;
        flagDone = TRUE;
      }
    }
    if (!flagDone)
      sap = SeqAlignFree (sap);
  }
  return sap;
}

static SeqAlignPtr ProfSeqAlign (Int4 start, Int4 length, Uint1 strand,
                                 FloatHi score, SeqIdPtr sip,
                                 Int2 topology)
{
  SeqAlignPtr sap;
  SeqLocPtr   slp;
  StdSegPtr   ssp;
  Boolean     flagDone;

  flagDone = FALSE;
  if ((sap = SeqAlignNew ()) != NULL)
  {
    sap->type = 255;      /* other */
    sap->segtype = 3;     /* std */
    sap->dim = 1;         /* for embedded info; is real; is not dummy */
    start -= (length-1);
    if ((slp = SeqLocIntNew (start, start+length-1, strand, sip)) != NULL)
    {
      if ((sap->score = ScoreNew ()) != NULL)
      {
        sap->score->choice = 2;
        sap->score->value.realvalue = score;
      }
      if ((ssp = StdSegNew ()) != NULL)
      {
        ssp->dim = topology;
        ssp->ids = SeqIdDup (sip);
        ssp->loc = slp;
        sap->segs = ssp;
        flagDone = TRUE;
      }
    }
    if (!flagDone)
      sap = SeqAlignFree (sap);
  }
  return sap;
}

static void SeqAlignLink (SeqAlignPtr PNTR saphead, SeqAlignPtr sap)
{
  SeqAlignPtr sapnext;

  if (*saphead == NULL)
  {
    *saphead = sap;
    return;
  }
  sapnext = *saphead;
  while (sapnext->next != NULL)
    sapnext = sapnext->next;
  sapnext->next = sap;
  return;
}

extern ValNodePtr ReadPatternNames (CharPtr filename)
{
  FILE        *fiop;
  ValNodePtr  namenew, namelist = NULL;
  Char        buf[PATH_MAX];
  CharPtr     cptr;

  if (filename != NULL && StrLen (filename) > 0)
  {
    if (FindPath ("ncbi", "ncbi", "data", buf, sizeof (buf)))
      FileBuildPath (buf, NULL, filename);
    else
      StrCpy (buf, filename);

    if ((fiop = FileOpen (buf, "r")) == NULL)
    {
      if ((fiop = FileOpen (filename, "r")) == NULL)
      {
        return NULL;
      }
    }
    while ((FileGets (buf, sizeof (buf), fiop)) != NULL)
    {
      if (buf[0] == ';' || buf[0] == '>')
        continue;
      if ((cptr = StrChr (buf, '\n')) != NULL)
        *cptr = '\0';
      if ((namenew = ValNodeNew (NULL)) != NULL)
      {
        ValNodeLink (&namelist, namenew);
        namenew->data.ptrvalue = (Pointer) StringSave (buf);
      }
    }
    FileClose (fiop);
  }
  return namelist;
}

/* RENSites */

extern ComPatPtr ReadRENPattern (CharPtr filename, Boolean flagNeedCutPos,
                                 ValNodePtr namelist)
{
  FILE       *fin;
  Char       bufin[PATH_MAX];
  Int4       buflen = PATH_MAX;
  CharPtr    ichar = "ID   ";
  CharPtr    pchar = "RS   ";
  CharPtr    echar = "//";
  Boolean    flagHaveCutPos, flagNamed;
  ValNodePtr namehead;

  CharPtr bufname, bufpat, cptr;

  ComPatPtr cpph, cppn, cpp;

  if (filename == NULL || !(StrLen (filename) > 0))
    return NULL;

  if (FindPath ("ncbi", "ncbi", "data", bufin, sizeof (bufin)))
    FileBuildPath (bufin, NULL, filename);
  else
    StrCpy (bufin, filename);

  if ((fin = FileOpen (bufin, "r")) == NULL)
  {
    if ((fin = FileOpen (filename, "r")) == NULL)
    {
      return NULL;
    }
  }

  bufname = MemNew ((size_t) (sizeof (Char) * buflen));
  bufpat = MemNew ((size_t) (sizeof (Char) * buflen));
  if (bufname == NULL || bufpat == NULL)
  {
    MemFree (bufname);
    MemFree (bufpat);
    FileClose (fin);
    return NULL;
  }

  namehead = namelist;
  cpph = NULL;
  while ((FileGets (bufin, buflen, fin)) != NULL)
  {
    if (StrStr (bufin, ichar))
    {
      if ((cptr = StrChr (bufin, '\n')) != NULL)
        *cptr = '\0';
      cptr = bufin + StrLen (ichar);
      StrCpy (bufname, cptr);
      namelist = namehead;
      if (namelist != NULL)
      {
        flagNamed = FALSE;
        while (namelist != NULL)
        {
          if (StringCmp (bufname, (CharPtr) namelist->data.ptrvalue) == 0)
          {
            flagNamed = TRUE;
            break;
          }
          namelist = namelist->next;
        }
      }
      else
      {
        flagNamed = TRUE;
      }
      bufpat[0] = '\0';
      while ((FileGets (bufin, buflen, fin)) != NULL)
      {
        if (StrStr (bufin, pchar))
        {
/* info only: could just search for 1st comma to remove palidromes */
          if ((cptr = StrChr (bufin, ';')) != NULL)
            *cptr = '\0';
          if (StrStr (bufin, ", ?"))
            flagHaveCutPos = FALSE;
          else
            flagHaveCutPos = TRUE;
/* ignore cut site for now */
          if ((cptr = StrChr (bufin, ',')) != NULL)
            *cptr = '\0';
/* bail if no recognition site */
          if ((cptr = StrChr (bufin, '?')) != NULL)
            *cptr = '\0';
          cptr = bufin + StrLen (pchar);
/* bail if need cut position but have none */
          if (flagNeedCutPos && !flagHaveCutPos)
            *cptr = '\0';
/* bail if not named */
          if (!flagNamed)
            *cptr = '\0';
          StrCpy (bufpat, cptr);
        }
        if (StrStr (bufin, echar))
          break;
      }
      if ((cpp = CompilePattern (bufpat, 0)) != NULL)
      {
        PalindromeCheck (cpp);
        StrCpy (cpp->name, bufname);
        if (cpph == NULL)
        {
          cpph = cpp;
        }
        else
        {
          cppn = cpph;
          while (cppn->nextpattern != NULL)
            cppn = cppn->nextpattern;
          cppn->nextpattern = cpp;
        }
      }
    }
  }
  MemFree (bufname);
  MemFree (bufpat);
  FileClose (fin);
  return cpph;
}

/* Prosite Patterns */

extern ComPatPtr ReadPrositePattern (CharPtr filename, Boolean flagStringent,
                                     Int4 cutoff, CharPtr taxon,
                                     ValNodePtr namelist)
{
  FILE    *fin;
  Char    buf[PATH_MAX];
  CharPtr inbuff, ptbuff, psbuff, gbbuff, nabuff, cptr;
  CharPtr ichar = "ID   ";
  CharPtr rchar = "RS   ";
  CharPtr pchar = "PA   ";
  CharPtr echar = "//";
  CharPtr schar = "/SKIP-FLAG=TRUE;";
  CharPtr tchar = "/TAXO-RANGE=";
  Char    buffer[512];
  Int4    i, cut, blen = 128;
  Boolean flagCutoff, flagSkipIt, flagNamed, flagTaxon, flagTaxonHit;

  ValNodePtr namehead;

  ComPatPtr cpph, cppn, cpp;

  if (cutoff < 0)
    cutoff = INT4_MAX;

  if (filename == NULL || !(StrLen (filename) > 0))
    return NULL;

  if (FindPath ("ncbi", "ncbi", "data", buf, sizeof (buf)))
    FileBuildPath (buf, NULL, filename);
  else
    StrCpy (buf, filename);

  if ((fin = FileOpen (buf, "r")) == NULL)
  {
    if ((fin = FileOpen (filename, "r")) == NULL)
    {
      return NULL;
    }
  }

  inbuff = MemNew ((size_t) (sizeof (Char) * blen));
  nabuff = MemNew ((size_t) (sizeof (Char) * blen * 2));
  psbuff = MemNew ((size_t) (sizeof (Char) * blen * 4));
  if (inbuff == NULL || nabuff == NULL || psbuff == NULL)
  {
    MemFree (inbuff);
    MemFree (nabuff);
    MemFree (psbuff);
    FileClose (fin);
    return NULL;
  }

  namehead = namelist;
  cpph = NULL;
  while ((FileGets (inbuff, blen, fin)) != NULL)
  {
    flagCutoff = FALSE;
    flagSkipIt = FALSE;
    flagTaxon = FALSE;
    if (StrStr (inbuff, ichar))
    {
      namelist = namehead;
      if (namelist != NULL)
      {
        flagNamed = FALSE;
        while (namelist != NULL)
        {
          if (StrStr (inbuff, (CharPtr) namelist->data.ptrvalue))
          {
            flagNamed = TRUE;
            break;
          }
          namelist = namelist->next;
        }
      }
      else
      {
        flagNamed = TRUE;
      }
      if ((cptr = StrChr (inbuff, '\n')) != NULL)
        *cptr = '\0';
      cptr = inbuff + StrLen (ichar);
      StrCpy (nabuff, cptr);
      psbuff[0] = '\0';
      while ((FileGets (inbuff, blen, fin)) != NULL)
      {
        if (StrStr (inbuff, pchar))
        {
          ptbuff = inbuff + StrLen (pchar);
          StrCat (psbuff, ptbuff);
        }
        if (StrStr (inbuff, rchar))
        {
          cptr = inbuff + StrLen (rchar);
          cut = atol (cptr);
          if (cut > cutoff)
            flagCutoff = TRUE;
        }
        if (StrStr (inbuff, schar))
          flagSkipIt = TRUE;
        if (StrStr (inbuff, tchar))
        {
          StrCpy (buffer, inbuff);
          flagTaxon = TRUE;
        }
        if (StrStr (inbuff, echar))
          break;
      }
      if ((flagSkipIt && flagStringent) ||
          !flagNamed || flagCutoff)
        continue;
      flagTaxonHit = TRUE;
      if (flagTaxon && taxon != NULL)
      {
        flagTaxonHit = FALSE;
        cptr = StrStr (buffer, tchar);
        cptr += StrLen (tchar);
        for (i = 0; i < 5; i++)
        {
          if (taxon[i] == cptr[i])
            flagTaxonHit = TRUE;
        }
      }
      if (!flagTaxonHit)
        continue;
      CleanPattern (psbuff);
      if ((gbbuff = PrositeToGBPattern (psbuff)) != NULL)
      {
        if ((cpp = CompilePattern (gbbuff, 1)) != NULL)
        {
          StrCpy (cpp->name, nabuff);
          if (cpph == NULL)
          {
            cpph = cpp;
          }
          else
          {
            cppn = cpph;
            while (cppn->nextpattern != NULL)
              cppn = cppn->nextpattern;
            cppn->nextpattern = cpp;
          }
        }
        MemFree (gbbuff);
      }
    }
  }
  MemFree (inbuff);
  MemFree (nabuff);
  MemFree (psbuff);
  FileClose (fin);
  return cpph;
}

/* prosite patterns must always end with a period */

extern Boolean IsProsite (CharPtr inbuff)
{
  Int4 i;

  if (inbuff == NULL)
    return FALSE;

  i = StrLen (inbuff);
  i--;
  while (i >= 0)
  {
    if (inbuff[i] == '.')
      return TRUE;
    if (!IS_WHITESP (inbuff[i]))
      return FALSE;
    i--;
  }
  return FALSE;
}

extern CharPtr PrositeToGBPattern (CharPtr pspat)
{
  CharPtr   gbpat, gbpathead;
  CharPtr   phead, ptail, pelem;
  Boolean   flagAddResidue, flagHaveElement;
  Char      thisres;

  if (pspat == NULL)
    return NULL;
  if (StrLen (pspat) == 0)
    return NULL;
  if (!IsProsite (pspat))
    return NULL;

  if ((gbpathead = gbpat = (CharPtr) MemNew (((size_t) (sizeof (Char) *
                                             StrLen (pspat) * 32)))) != NULL)
  {
    flagHaveElement = FALSE;
    while (*pspat != '\0')
    {
      if (!flagHaveElement)
      {
        pelem = pspat;
        while (*pelem != '\0' && *pelem != '-' && *pelem != '.')
          pelem++;
        if (*pelem != '\0')
        {
          phead = pspat;
          while (*phead != '(' && phead != pelem)
            phead++;
          if (*phead == '(')
          {
            ptail = phead;
            while (ptail != pelem)
            {
              if (*ptail == ')')
                break;
              ptail++;
            }
            if (ptail == pelem)
            {
              return (CharPtr) MemFree (gbpathead);
            }
            for (pelem = phead; pelem <= ptail; pelem++)
            {
              if (*pelem == ',')
                *gbpat = ':';
              else
                *gbpat = *pelem;
              gbpat++;
            }
          }
        }
        flagHaveElement = TRUE;
      }

      switch (*pspat)
      {
/* it's over */
       case '.':
        return gbpathead;
/* is N term */
       case '<':
        *gbpat = *pspat;
        gbpat++;
        break;
/* is C term */
       case '>':
        *gbpat = *pspat;
        gbpat++;
        break;
/* is collection of keeps */
       case '[':
        phead = ptail = pspat;
        while (*ptail != '\0')
        {
          if (*ptail == ']')
            break;
          ptail++;
        }
        if (*ptail == '\0')
        {
          return (CharPtr) MemFree (gbpathead);
        }
        for (pspat = phead; pspat < ptail; pspat++)
        {
          *gbpat = *pspat;
          gbpat++;
        }
        *gbpat = *ptail;
        gbpat++;
        pspat = ptail;
        break;
/* shouldn't happen */
       case ']':
        break;
/* is collection of discards */
       case '{':
        phead = ptail = pspat;
        while (*ptail != '\0')
        {
          if (*ptail == '}')
            break;
          ptail++;
        }
        if (*ptail == '\0')
        {
          return (CharPtr) MemFree (gbpathead);
        }
        *gbpat = '[';
        gbpat++;
        phead++;
        for (thisres = 'A'; thisres <= 'Z'; thisres++)
        {
          flagAddResidue = TRUE;
          for (pspat = phead; pspat < ptail; pspat++)
          {
            if (thisres == *pspat)
              flagAddResidue = FALSE;
          }
          if (flagAddResidue)
          {
            *gbpat = thisres;
            gbpat++;
          }
        }
        *gbpat = ']';
        gbpat++;
        pspat = ptail;
        break;
/* shouldn't happen */
       case '}':
        break;
/* range of residues - handled outside of switch */
       case '(':
        while (*pspat != '\0' && *pspat != ')')
          pspat++;
        if (*pspat == '\0')
        {
          return (CharPtr) MemFree (gbpathead);
        }
        break;
/* shouldn't happen */
       case ')':
        break;
/* pattern separator */
       case '-':
        flagHaveElement = FALSE;
        break;
/* stop codon */
       case '*':
        break;
/* default better be a residue */
       default:
        if (*pspat < 'A' || *pspat > 'Z')
        {
          return (CharPtr) MemFree (gbpathead);
        }
        *gbpat = *pspat;
        gbpat++;
        break;
      }
      pspat++;
    }
  }
  return gbpathead;
}

/* GenBank Patterns */

extern CharPtr ReadPattern (CharPtr filename)
{
  FILE    *fin;
  CharPtr buff1, buff2;
  CharPtr inbuff, outbuff;
  CharPtr bptr, tptr;
  Int4    blen = 128;

  if (filename == NULL || !(StrLen (filename) > 0))
    return NULL;

  if ((fin = FileOpen (filename, "r")) == NULL)
    return NULL;

  tptr = MemNew (sizeof (Char) * MAXPATSIZE);
  buff1 = MemNew (sizeof (Char) * blen);
  buff2 = MemNew (sizeof (Char) * blen);
  if (tptr == NULL || buff1 == NULL || buff2 == NULL)
  {
    MemFree (tptr);
    MemFree (buff1);
    MemFree (buff2);
    FileClose (fin);
    return NULL;
  }

  while ((FileGets (buff1, blen, fin)) != NULL)
  {
    inbuff = buff1;
    outbuff = buff2;
    while (*inbuff != '\0')
    {
      if (*inbuff == '#' || *inbuff == ';')
      {
        while (*inbuff != '\n')
          inbuff++;
      }
      if (!IS_WHITESP (*inbuff))
      {
        *outbuff = *inbuff;
        outbuff++;
      }
      inbuff++;
    }
    *outbuff = *inbuff;
    if ((StrLen (buff2) + StrLen (tptr)) > MAXPATSIZE)
    {
      MemFree (tptr);
      MemFree (buff1);
      MemFree (buff2);
      FileClose (fin);
      return NULL;
    }
    StrCat (tptr, buff2);
  }

  bptr = MemNew (StrLen (tptr) + 1);
  StrCpy (bptr, tptr);
  StrUpper (bptr);
  MemFree (tptr);
  MemFree (buff1);
  MemFree (buff2);
  FileClose (fin);
  return bptr;
}

extern void CleanPattern (CharPtr inbuff)
{
  CharPtr headptr, outbuff;

  if (inbuff == NULL)
    return;
  headptr = outbuff = inbuff;
  while (*inbuff != '\0')
  {
    if (*inbuff == '#' || *inbuff == ';')
    {
      while (*inbuff != '\n' || *inbuff != '\r')
        inbuff++;
    }
    if (!IS_WHITESP (*inbuff))
    {
      *outbuff = *inbuff;
      outbuff++;
    }
    inbuff++;
  }
  *outbuff = *inbuff;
  StrUpper (headptr);
  return;
}

extern ComPatPtr ComPatNew (ComPatPtr curcpp)
{
  ComPatPtr   cpp;

  cpp = (ComPatPtr) MemNew (sizeof (ComPat));
  cpp->prev = NULL;
  cpp->next = NULL;
  cpp->prevpattern = NULL;
  cpp->nextpattern = NULL;
  cpp->min = 0;
  cpp->max = 0;
  cpp->res[0] = '\0';
  cpp->flagPalindrome = FALSE;
  cpp->flagNTerm = FALSE;
  cpp->flagCTerm = FALSE;

  if (curcpp == NULL)
    return cpp;

  while (curcpp->next != NULL)
    curcpp = curcpp->next;
  curcpp->next = cpp;
  cpp->prev = curcpp;

  return cpp;
}

extern ComPatPtr ComPatFree (ComPatPtr headcpp)
{
  ComPatPtr   cpp, cppp;

  while (headcpp != NULL)
  {
    cppp = headcpp->nextpattern;
    while (headcpp != NULL)
    {
      cpp = headcpp->next;
      headcpp->next = NULL;
      MemFree (headcpp);
      headcpp = cpp;
    }
    headcpp = cppp;
  }

  return headcpp;
}

extern ComPatPtr ComPatDup (ComPatPtr cppold)
{
  ComPatPtr   cpp, cppt, cppnew, cpphead, cppheadpat = NULL;

  while (cppold != NULL)
  {
    cpp = cppold;
    cpphead = NULL;
    cppnew = NULL;
    while (cpp != NULL)
    {
      cppnew = ComPatNew (cppnew);
      if (cpphead == NULL)
        cpphead = cppnew;
      StrCpy (cppnew->name, cpp->name);
      cppnew->min = cpp->min;
      cppnew->max = cpp->max;
      StrCpy (cppnew->res, cpp->res);
      cppnew->flagPalindrome = cpp->flagPalindrome;
      cppnew->flagNTerm = cpp->flagNTerm;
      cppnew->flagCTerm = cpp->flagCTerm;
      cpp = cpp->next;
    }
    if (cppheadpat == NULL)
    {
      cppheadpat = cpphead;
    }
    else
    {
      cppt = cppheadpat;
      while (cppt->nextpattern != NULL)
        cppt = cppt->nextpattern;
      cppt->nextpattern = cpphead;
      cpphead->prevpattern = cppt;
    }
    cppold = cppold->nextpattern;
  }

  return cppheadpat;
}

extern ComPatPtr InvertPattern (ComPatPtr cppold)
{
  ComPatPtr cpp, cpphead, cppnew = NULL, cppnewhead = NULL;
  CharPtr   pat;
  Int4      i, n, maxpat;

  cpp = cpphead = ComPatDup (cppold);
  while (cpp != NULL)
  {
    pat = cpp->res;
    while (*pat != '\0')
    {
      switch (*pat)
      {
       case 'A':
        *pat = 'T';
        break;
       case 'B':
        *pat = 'V';
        break;
       case 'C':
        *pat = 'G';
        break;
       case 'D':
        *pat = 'H';
        break;
       case 'G':
        *pat = 'C';
        break;
       case 'H':
        *pat = 'D';
        break;
       case 'K':
        *pat = 'M';
        break;
       case 'M':
        *pat = 'K';
        break;
       case 'R':
        *pat = 'Y';
        break;
       case 'T':
        *pat = 'A';
        break;
       case 'V':
        *pat = 'B';
        break;
       case 'Y':
        *pat = 'R';
        break;
       default:
        break;
      }
      pat++;
    }
    cpp = cpp->next;
  }

  cpp = cpphead;
  maxpat = 0;
  while (cpp != NULL)
  {
    maxpat++;
    cpp = cpp->next;
  }

  for (i = 0; i < maxpat; i++)
  {
    cpp = cpphead;
    for (n = i+1; n < maxpat; n++)
    {
      cpp = cpp->next;
    }
    cppnew = ComPatNew (cppnew);
    if (cppnew != NULL)
    {
      if (cppnewhead == NULL)
        cppnewhead = cppnew;
      cppnew->min = cpp->min;
      cppnew->max = cpp->max;
      for (n = 0; n < 5; n++)
        cppnew->res[n] = cpp->res[n];
    }
  }
  ComPatFree (cpphead);
  return cppnewhead;
}

extern void PalindromeCheck (ComPatPtr cpp)
{
  ComPatPtr cppt, cppi, cpphi;
  CharPtr   cptr;

  while (cpp != NULL)
  {
    cpp->flagPalindrome = TRUE;
    cppi = cpphi = InvertPattern (cpp);
    cppt = cpp;
    while (cppt != NULL)
    {
      cptr = cppt->res;
      while (*cptr != '\0')
      {
        if (!StrChr (cppi->res, *cptr))
        {
          cpp->flagPalindrome = FALSE;
          break;
        }
        cptr++;
      }
      if (!cpp->flagPalindrome)
        break;
      cppi = cppi->next;
      cppt = cppt->next;
    }
    ComPatFree (cpphi);
    cpp = cpp->nextpattern;
  }
  return;
}

static void AddResidues (CharPtr res, Char pat, Int2 moltype)
{
  Int4 i, n;
  CharPtr reshead;

  static CharPtr ambiresNA[] =
  {
    "AA",
    "BCGT",
    "CC",
    "DAGT",
    "GG",
    "HACT",
    "KGT",
    "MAC",
    "NACGT",
    "RAG",
    "SCG",
    "TT",
    "VACG",
    "WAT",
    "YCT",
    ""
  };

  static CharPtr ambiresAA[] =
  {
    "AA",
    "BDN",
    "CC",
    "DD",
    "EE",
    "FF",
    "GG",
    "HH",
    "II",
    "KK",
    "LL",
    "MM",
    "NN",
    "PP",
    "QQ",
    "RR",
    "SS",
    "TT",
    "UU", /* selenocysteine */
    "VV",
    "WW",
    "XABCDEFGHIKLMNPQRSTUVWXYZ", /* any residue not including stop codon */
    "YY",
    "ZEQ",
    "**", /* stop codon */
    ""
  };

  if (moltype != 0 && moltype != 1)
    return;

  reshead = res;
  i = 0;
  if (moltype == 0)
  {
    while (ambiresNA[i] != NULL && ambiresNA[i][0] != '\0')
    {
      if (ambiresNA[i][0] == pat)
      {
        n = 1;
        while (ambiresNA[i][n] != '\0')
        {
          res = reshead;
          while (*res != '\0')
          {
            if (*res == ambiresNA[i][n])
              break;
            res++;
          }
          if (*res == '\0')
          {
            *res++ = ambiresNA[i][n];
            *res = '\0';
          }
          n++;
        }
        return;
      }
      i++;
    }
  }
  else
  {
    while (ambiresAA[i] != NULL && ambiresAA[i][0] != '\0')
    {
      if (ambiresAA[i][0] == pat)
      {
        n = 1;
        while (ambiresAA[i][n] != '\0')
        {
          res = reshead;
          while (*res != '\0')
          {
            if (*res == ambiresAA[i][n])
              break;
            res++;
          }
          if (*res == '\0')
          {
            *res++ = ambiresAA[i][n];
            *res = '\0';
          }
          n++;
        }
        return;
      }
      i++;
    }
  }
  return;
}

extern ComPatPtr CompilePattern (CharPtr pattern, Int2 moltype)
{
  ComPatPtr    cpp = NULL, headcpp = NULL;
  CharPtr      head;
  Char         temp;
  Boolean      flagCount, flagFirst, flagNTerm, flagCTerm;

  if (pattern == NULL)
    return headcpp;
  if (*pattern == '\0')
    return headcpp;

  CleanPattern (pattern);
  flagCount = FALSE;
  flagFirst = TRUE;
  flagNTerm = FALSE;
  flagCTerm = FALSE;

  if (*pattern != '\0')
  {
    if (*pattern == '<')
      flagNTerm = TRUE;
    if (StrLen (pattern) >= 2)
      if (pattern[StrLen (pattern) - 2] == '>')
        flagCTerm = TRUE;
  }

  while (*pattern != '\0')
  {
    switch (*pattern)
    {
     case '<':         /* just a check for spurious N signals */
      if (!flagFirst)
      {
        headcpp = ComPatFree (headcpp);
        return headcpp;
      }
      break;
     case '>':         /* just a check for spurious C signals */
      if (*(pattern+1) != '\0')
      {
        headcpp = ComPatFree (headcpp);
        return headcpp;
      }
      break;
     case '(':
      cpp = ComPatNew (cpp);
      if (headcpp == NULL)
        headcpp = cpp;
      cpp->flagNTerm = flagNTerm;
      cpp->flagCTerm = flagCTerm;
      pattern++;
      head = pattern;
      while (*pattern != '\0' && *pattern != ')')
      {
        while (*pattern != '\0' && *pattern != ':' && *pattern != ')')
/*
 save comma for sequence of gaps
               *pattern != ',' && *pattern != ')')
*/
        {
          pattern++;
        }
        if (*pattern != '\0')
        {
          temp = *pattern;
          *pattern = '\0';
          if (cpp->min == 0)
            cpp->min = atoi (head);
          else
            cpp->max = atoi (head);
          *pattern = temp;
          if (*pattern == ')')
            continue;
          pattern++;
          head = pattern;
        }
      }
      if (*pattern == '\0')
      {
        headcpp = ComPatFree (headcpp);
        return headcpp;
      }
      if (cpp->min < 1)
        cpp->min = 1;
      if (cpp->max < cpp->min)
        cpp->max = cpp->min;
      flagCount = TRUE;
      break;
     case '[':
      if (!flagCount)
      {
        cpp = ComPatNew (cpp);
        cpp->min = 1;
        cpp->max = 1;
        if (headcpp == NULL)
          headcpp = cpp;
        cpp->flagNTerm = flagNTerm;
        cpp->flagCTerm = flagCTerm;
      }
      flagCount = FALSE;
      pattern++;
      while (*pattern != '\0' && *pattern != ']')
      {
        if (*pattern != '\0')
        {
          AddResidues (cpp->res, *pattern, moltype);
          pattern++;
        }
      }
      if (*pattern == '\0')
      {
        headcpp = ComPatFree (headcpp);
        return headcpp;
      }
      break;
     case ')':
     case ']':
      headcpp = ComPatFree (headcpp);
      return headcpp;
     default:
      if (!flagCount)
      {
        cpp = ComPatNew (cpp);
        cpp->min = 1;
        cpp->max = 1;
        if (headcpp == NULL)
          headcpp = cpp;
        cpp->flagNTerm = flagNTerm;
        cpp->flagCTerm = flagCTerm;
      }
      flagCount = FALSE;
      AddResidues (cpp->res, *pattern, moltype);
      break;
    }
    flagFirst = FALSE;
    pattern++;
  }

  cpp = headcpp;
  while (cpp != NULL)
  {
    if (*(cpp->res) == '\0')
    {
      headcpp = ComPatFree (headcpp);
      return headcpp;
    }
    cpp = cpp->next;
  }
  return headcpp;
}

static Int4 ComPatLenMax (ComPatPtr cpp)
{
  Int4 i = 0;

  while (cpp != NULL)
  {
    i += cpp->max;
    cpp = cpp->next;
  }
  return i;
}

static void WrapSeqAlign (SeqAlignPtr sap, Int4 length)
{
  StdSegPtr ssp;
  SeqLocPtr slp, slpt;
  SeqIntPtr sint;

  while (sap != NULL)
  {
    ssp = sap->segs;
    slp = ssp->loc;
    sint = slp->data.ptrvalue;
    if (sint->from >= length && sint->to >= length)
    {
      sint->from -= length;
      sint->to -= length;
    }
    if (sint->to >= length)
    {
      slpt = SeqLocIntNew (0, sint->to - length, SeqLocStrand (slp),
                           SeqLocId (slp));
      sint->to = length - 1;
      ValNodeLink (&slp, slpt);
      slpt = (SeqLocPtr) ValNodeNew (NULL);
      slpt->choice = 8;  /* SeqLocMix */
      slpt->data.ptrvalue = slp;
      ssp->loc = slpt;
    }
    sap = sap->next;
  }
  return;
}

/*
 flagFirstInvocation *MUST* be TRUE on callers' invocation
                     this sets patlen to 0
 flagFindAllMatches  if FALSE on intial invocation then
                     just find the first match
*/
static SeqAlignPtr PatternMatchX (Uint1Ptr seq, Int4 seqpos, Uint1 strand,
                                  SeqIdPtr sip, ComPatPtr cpp, Int4 patlen,
                                  Int4 gapoffset, Int4 mismatch,
                                  Int4Ptr misses, Int2 topology,
                                  Boolean flagFirstInvocation,
                                  Boolean flagFindAllMatches)
{
  SeqAlignPtr saphead = NULL, sap;
  CharPtr     res;
  Int4        rescount, maxval;

  if (cpp == NULL || *seq == '\0')
    return NULL;

  if (flagFirstInvocation)
  {
    patlen = 0;
    *misses = 0;
  }

/*
  rescount = cpp->min + gapoffset;
  seq += (rescount - 1);
  seqpos += (rescount - 1);
*/

/* way slower but prevents pattern overruns; should pass a length in */
  maxval = cpp->min + gapoffset;
  for (rescount = 1; rescount < maxval; rescount++)
  {
    seq++;
    seqpos++;
    if (*seq == '\0')
      break;
  }

  while (*seq != '\0')
  {
    res = cpp->res;
    while (*res != '\0')
    {
      if (*seq == *res)
      {
        patlen += rescount;
        if (cpp->next != NULL)
        {
          seq++;
          seqpos++;
          sap = PatternMatchX (seq, seqpos, strand, sip, cpp->next,
                               patlen, 0, mismatch, misses, topology,
                               FALSE, FALSE);
          if (sap == NULL)
          {
            if ((rescount+1) <= cpp->max)
            {
              seq -= rescount;
              seqpos -= rescount;
              patlen -= rescount;
              gapoffset++;
              sap = PatternMatchX (seq, seqpos, strand, sip, cpp,
                                   patlen, gapoffset, mismatch,
                                   misses, topology,
                                   FALSE, FALSE);
            }
          }
        }
        else
        {
          sap = PatSeqAlign (seqpos, patlen, strand, *misses, sip,
                             topology);
          seq++;
          seqpos++;   /* needed for single residue patterns */
        }
        if (flagFirstInvocation)
        {
          if (sap == NULL || flagFindAllMatches)
          {
            SeqAlignLink (&saphead, sap);
            seq--;
            seqpos--;
            break;
          }
          else
          {
            return sap;
          }
        }
        else
        {
          return sap;
        }
      }
      res++;
    }
    if (*res == '\0')
    {
      (*misses)++;
      if (*misses <= mismatch)
      {
        patlen += rescount;
        if (cpp->next != NULL)
        {
          seq++;
          seqpos++;
          sap = PatternMatchX (seq, seqpos, strand, sip, cpp->next,
                               patlen, 0, mismatch, misses, topology,
                               FALSE, FALSE);
          if (sap == NULL)
          {
            if ((rescount+1) <= cpp->max)
            {
              seq -= rescount;
              seqpos -= rescount;
              patlen -= rescount;
              gapoffset++;
              sap = PatternMatchX (seq, seqpos, strand, sip, cpp,
                                   patlen, gapoffset, mismatch,
                                   misses, topology,
                                   FALSE, FALSE);
            }
          }
          seq--;
          seqpos--;
        }
        else
        {
          sap = PatSeqAlign (seqpos, patlen, strand, *misses, sip,
                             topology);
        }
        if (!flagFirstInvocation)
        {
          return sap;
        }
        else
        {
          SeqAlignLink (&saphead, sap);
          if (!flagFindAllMatches)
            return saphead;
        }
      }
      else
      {
        if (!flagFirstInvocation)
          return NULL;
      }
    }
    seq++;
    seqpos++;
    patlen = 0;
    *misses = 0;
  }
  return saphead;
}

extern SeqAlignPtr PatternMatch (Uint1Ptr seq, Int4 seqpos, Uint1 strand,
                                 SeqIdPtr sip, ComPatPtr cpp,
                                 Int4 mismatch, Int2 topology,
                                 Boolean flagFindAllMatches)
{
  Int4   misses = 0;
  Int4   length;

  length = ComPatLenMax (cpp);
  if (length / 4 < mismatch)
    mismatch = length / 4;
  if (mismatch < 0)
    mismatch = 0;

  return PatternMatchX (seq, seqpos, strand, sip, cpp, 0, 0,
                        mismatch, &misses, topology,
                        TRUE, flagFindAllMatches);
}

extern SeqAlignPtr PatternMatchBioseq (BioseqPtr bsp, ComPatPtr cpp,
                                       Int4 mismatch)
{
  ComPatPtr   cppi;
  SeqAlignPtr saphead = NULL, sap;
  SeqPortPtr  spp;
  Uint1Ptr    seq, seql, seqr;
  Int4        patlenmax, seqchunk, start, misses;

  if (ISA_na (bsp->mol))
  {
    spp = SeqPortNew (bsp, 0, bsp->length-1, 0, Seq_code_iupacna);
  }
  else
  {
    spp = SeqPortNew (bsp, 0, bsp->length-1, 0, Seq_code_iupacaa);
  }

  patlenmax = ComPatLenMax (cpp);
  seqchunk = MAXCHUNKSIZE;
  start = 0;
  while ((seq = GetSeqChunk (spp, start, bsp->length,
                             seqchunk)) != NULL)
  {
    misses = 0;
    sap = PatternMatchX (seq, start, Seq_strand_plus, bsp->id, cpp, 0, 0,
                         mismatch, &misses, bsp->topology, TRUE, TRUE);
    SeqAlignLink (&saphead, sap);
    if (ISA_na (bsp->mol) && !cpp->flagPalindrome)
    {
      cppi = InvertPattern (cpp);
      misses = 0;
      sap = PatternMatchX (seq, start, Seq_strand_minus, bsp->id, cppi, 0, 0,
                           mismatch, &misses, bsp->topology, TRUE, TRUE);
      SeqAlignLink (&saphead, sap);
      URK_SeqAlignSortByStart (&saphead);
      ComPatFree (cppi);
    }
    MemFree (seq);
    start += (seqchunk - patlenmax + 1);
  }
  if (ISA_na (bsp->mol) && bsp->topology == 2)    /* circular NA */
  {
    start = bsp->length - patlenmax + 1;
    if ((seql = GetSeqChunk (spp, start, bsp->length,
                             seqchunk)) != NULL)
    {
      if ((seqr = GetSeqChunk (spp, 0, patlenmax-1,
                               seqchunk)) != NULL)
      {
        if ((seq = (Uint1Ptr) MemNew ((size_t)
            ((StrLen ((CharPtr) seql)) + (StrLen ((CharPtr) seqr)) + 1)))
            != NULL)
        {
          StrCpy ((CharPtr) seq, (CharPtr) seql);
          StrCat ((CharPtr) seq, (CharPtr) seqr);
          misses = 0;
          sap = PatternMatchX (seq, start, Seq_strand_plus, bsp->id, cpp,
                               0, 0, mismatch, &misses, bsp->topology,
                               TRUE, TRUE);
          WrapSeqAlign (sap, bsp->length);
          SeqAlignLink (&saphead, sap);
          if (ISA_na (bsp->mol) && !cpp->flagPalindrome)
          {
            cppi = InvertPattern (cpp);
            misses = 0;
            sap = PatternMatchX (seq, start, Seq_strand_minus, bsp->id, cppi,
                                 0, 0, mismatch, &misses, bsp->topology,
                                 TRUE, TRUE);
            WrapSeqAlign (sap, bsp->length);
            SeqAlignLink (&saphead, sap);
            ComPatFree (cppi);
          }
          URK_SeqAlignSortByStart (&saphead);
          MemFree (seq);
        }
        MemFree (seqr);
      }
      MemFree (seql);
    }
  }
  SeqPortFree (spp);
  return saphead;
}

/* Profiles */

static Boolean IS_COMMENT (Char c)
{
  if (c == '#' || c == ';')
    return TRUE;
  else
    return FALSE;
}

/* format
;
# comment lines are ; or #
;

A 1.5   2
G 1      2.5
T 0.000   3
C -15.3    3.5

*/

extern ComProfPtr ReadProfile (CharPtr filename)
{
  FILE       *fin;
  Char       bufin[PATH_MAX];
  CharPtr    inbuff;
  CharPtr    hptr;
  FloatHiPtr weight;
  Int4       i;
  Int4       blen = 2048; /* sort've determines max size of profile */
  Int4       size = 0;
  Char       residue;
  ComProfPtr hppp = NULL, ppp;

  if (filename == NULL || !(StrLen (filename) > 0))
    return NULL;

  if (FindPath ("ncbi", "ncbi", "data", bufin, sizeof (bufin)))
    FileBuildPath (bufin, NULL, filename);
  else
    StrCpy (bufin, filename);

  if ((fin = FileOpen (bufin, "r")) == NULL)
  {
    if ((fin = FileOpen (filename, "r")) == NULL)
    {
      return NULL;
    }
  }

  inbuff = MemNew (sizeof (Char) * blen);
  if (inbuff == NULL)
  {
    FileClose (fin);
    return NULL;
  }

  while ((FileGets (inbuff, blen, fin)) != NULL)
  {
    hptr = inbuff;
    while (IS_WHITESP (*hptr))
      hptr++;
    if (IS_COMMENT (*hptr))
      continue;
    if (*hptr == '\0')
      continue;
    if (!IS_ALPHA (*hptr) && *hptr != '*')
      continue;
    hptr++;
/* first real line determine the matrix width (or size) */
    while (!IS_COMMENT (*hptr) && *hptr != '\0')
    {
      while (IS_WHITESP (*hptr))
        hptr++;
      if (IS_COMMENT (*hptr) || *hptr == '\0')
        continue;
      while (!IS_WHITESP (*hptr) && !IS_COMMENT (*hptr) && *hptr != '\0')
        hptr++;
      size++;
    }
    if (size > 1)
    {
      hppp = ComProfNew (NULL);
      for (i = 1; i < size; i++)
        ComProfNew (hppp);
    }
    else
    {
      MemFree (inbuff);
      FileClose (fin);
      return NULL;
    }
    break;
  }

  rewind (fin);
  weight = MemNew (sizeof (FloatHi) * size);
  while ((FileGets (inbuff, blen, fin)) != NULL)
  {
    hptr = inbuff;
    while (IS_WHITESP (*hptr))
      hptr++;
    if (IS_COMMENT (*hptr))
      continue;
    if (*hptr == '\0')
      continue;
    if (!IS_ALPHA (*hptr) && *hptr != '*')
      continue;
    residue = TO_UPPER (*hptr);
    hptr++;
    i = 0;
    while (i < size)
    {
      while (IS_WHITESP (*hptr))
        hptr++;
      if (IS_COMMENT (*hptr) || *hptr == '\0')
        continue;
      weight[i] = atof (hptr);
      i++;
      while (!IS_WHITESP (*hptr) && !IS_COMMENT (*hptr) && *hptr != '\0')
        hptr++;
    }
    if (i != size)
    {
      MemFree (inbuff);
      MemFree (weight);
      ComProfFree (hppp);
      FileClose (fin);
      return NULL;
    }
    ppp = hppp;
    for (i = 0; i < size; i++)
    {
      ppp->score[residue-' '] = weight[i];
      ppp = ppp->next;
    }
  }
  MemFree (inbuff);
  MemFree (weight);
  FileClose (fin);
  return hppp;
}

extern ComProfPtr ComProfNew (ComProfPtr curppp)
{
  ComProfPtr   ppp;
  Int4         i;

  ppp = (ComProfPtr) MemNew (sizeof (ComProf));
  ppp->prev = NULL;
  ppp->next = NULL;
  ppp->prevprofile = NULL;
  ppp->nextprofile = NULL;
  for (i = 0; i < 64; i++)
    ppp->score[i] = 0.0;

  if (curppp == NULL)
    return ppp;

  while (curppp->next != NULL)
    curppp = curppp->next;
  curppp->next = ppp;
  ppp->prev = curppp;

  return ppp;
}

extern ComProfPtr ComProfFree (ComProfPtr headppp)
{
  ComProfPtr   ppp;

  while (headppp != NULL)
  {
    ppp = headppp->next;
    headppp->next = NULL;
    MemFree (headppp);
    headppp = ppp;
  }
  return headppp;
}

extern ComProfPtr ComProfDup (ComProfPtr pppold)
{
  ComProfPtr   ppp, pppt, pppnew, ppphead, pppheadprof = NULL;
  Int4         i;

  while (pppold != NULL)
  {
    ppp = pppold;
    ppphead = NULL;
    pppnew = NULL;
    while (ppp != NULL)
    {
      pppnew = ComProfNew (pppnew);
      if (ppphead == NULL)
        ppphead = pppnew;
      StrCpy (pppnew->name, ppp->name);
      for (i = 0; i < 64; i++)
        pppnew->score[i] = ppp->score[i];
      ppp = ppp->next;
    }
    if (pppheadprof == NULL)
    {
      pppheadprof = ppphead;
    }
    else
    {
      pppt = pppheadprof;
      while (pppt->nextprofile != NULL)
        pppt = pppt->nextprofile;
      pppt->nextprofile = ppphead;
      ppphead->prevprofile = pppt;
    }
    pppold = pppold->nextprofile;
  }

  return pppheadprof;
}

extern ComProfPtr ComProfDupThis (ComProfPtr pppthis)
{
  ComProfPtr   pppdup = NULL;
  Int4         i;

  if (pppthis != NULL)
  {
    pppdup = ComProfNew (NULL);
    StrCpy (pppdup->name, pppthis->name);
    for (i = 0; i < 64; i++)
      pppdup->score[i] = pppthis->score[i];
  }
  return pppdup;
}

extern void ComProfLink (ComProfPtr PNTR ppphead, ComProfPtr pppdup)
{
  ComProfPtr   ppp;

  ppp = *ppphead;
  if (ppp == NULL)
  {
    *ppphead = pppdup;
    return;
  }
  else
  {
    while (ppp->next != NULL)
      ppp = ppp->next;
    ppp->next = pppdup;
    pppdup->prev = ppp;
    return;
  }
}

extern void IntegerProfile (ComProfPtr ppp)
{
  Int4 i;

  while (ppp != NULL)
  {
    for (i = 0; i < 64; i++)
    {
      ppp->iscore[i] = (Int4) ppp->score[i];
    }
    ppp = ppp->next;
  }
  return;
}

extern ComProfPtr InvertProfile (ComProfPtr pppold)
{
  ComProfPtr ppphead, ppp, ppptemp;
  FloatHi    score;

  ppphead = ppp = ComProfDup (pppold);
  while (ppp != NULL)
  {
/* A T */
    score = ppp->score['A'-' '];
    ppp->score['A'-' '] = ppp->score['T'-' '];
    ppp->score['T'-' '] = score;
/* G C */
    score = ppp->score['G'-' '];
    ppp->score['G'-' '] = ppp->score['C'-' '];
    ppp->score['C'-' '] = score;
/* purine pyrimidine */
    score = ppp->score['R'-' '];
    ppp->score['R'-' '] = ppp->score['Y'-' '];
    ppp->score['Y'-' '] = score;
/* AC GT */
    score = ppp->score['K'-' '];
    ppp->score['K'-' '] = ppp->score['M'-' '];
    ppp->score['M'-' '] = score;
/* GCA CGT */
    score = ppp->score['V'-' '];
    ppp->score['V'-' '] = ppp->score['B'-' '];
    ppp->score['B'-' '] = score;
/* ATG TAC */
    score = ppp->score['D'-' '];
    ppp->score['D'-' '] = ppp->score['H'-' '];
    ppp->score['H'-' '] = score;
    ppp = ppp->next;
  }

  ppp = ppphead;
  while (ppp->next != NULL)
    ppp = ppp->next;
  ppphead = ppp;
  ppptemp = ppp->next;
  while (ppp != NULL)
  {
    ppp->next = ppp->prev;
    ppp->prev = ppptemp;
    ppptemp = ppp;
    ppp = ppp->next;
  }
  ppphead->prev = NULL;
  return ppphead;
}

extern FloatHi ProfScoreMax (ComProfPtr ppp)
{
  Int4    i;
  FloatHi score, scoremax = 0.0;

  while (ppp != NULL)
  {
    score = 0.0;
    for (i = 0; i < 64; i++)
    {
      if (ppp->score[i] > score)
        score = ppp->score[i];
    }
    scoremax += score;
    ppp = ppp->next;
  }
  return scoremax;
}

extern Int4 ProfLenMax (ComProfPtr ppp)
{
  Int4 i = 0;

/* profiles are ungapped */
  while (ppp != NULL)
  {
    i++;
    ppp = ppp->next;
  }
  return i;
}

extern Int4 CatenateProfile (ComProfPtr ppphead, Int4 proflen, Int4 icount)
{
  Int4       i, countthis, lenthis;
  ComProfPtr pppthis, pppdup;

  lenthis = proflen;
  if (ppphead == NULL)
    return lenthis;

  pppthis = ppphead;
  countthis = 0;
  for (i = 0; i < icount; i++)
  {
    pppdup = ComProfDupThis (pppthis);
    ComProfLink (&ppphead, pppdup);
    lenthis++;
    countthis++;
    if (countthis == proflen)
    {
      pppthis = ppphead;
      countthis = 0;
    }
    else
    {
      pppthis = pppthis->next;
    }
  }
  return lenthis;
}

/*
 flagFirstInvocation   *MUST* be TRUE on callers' invocation
                       this sets prlnmx and scoremax
 flagFindAllMatches    if FALSE on intial invocation then
                       just find the first hit
 flagBoundaryCondition just finds the fuzzy entries and ignores
                       the clear Yays and Nays
*/
static SeqAlignPtr ProfileMatchX (Uint1Ptr seq, Int4 seqpos, FloatHi cutoff,
                                  Uint1 strand, SeqIdPtr sip, ComProfPtr ppp,
                                  FloatHi scoremax, FloatHi score,
                                  Int4 prlnmx, Int2 topology,
                                  Boolean flagFirstInvocation,
                                  Boolean flagFindAllMatches,
                                  Boolean flagBoundaryCondition)
{
  SeqAlignPtr  saphead = NULL, sap;
  FloatHi      lowcut, highcut;

  if (ppp == NULL || *seq == 0)
    return saphead;

  if (flagFirstInvocation)
  {
    score = 0.0;
    prlnmx = ProfLenMax (ppp);
    scoremax = ProfScoreMax (ppp);
  }

  while (*seq != 0)
  {
    if (*seq >= (Uint1) 'A' && *seq <= (Uint1) 'Z')
      score += (ppp->score[*seq-' ']);
    if (ppp->next != NULL)
    {
      seq++;
      seqpos++;
      sap = ProfileMatchX (seq, seqpos, cutoff, strand, sip, ppp->next,
                           scoremax, score, prlnmx, topology, FALSE, FALSE,
                           flagBoundaryCondition);
    }
    else
    {
      if (flagBoundaryCondition)
      {
        lowcut = cutoff * 0.90;
        highcut = cutoff + (cutoff - (cutoff * 0.90));
        if (score >= lowcut && score <= highcut)
          sap = ProfSeqAlign (seqpos, prlnmx, strand, score, sip, topology);
        else
          sap = NULL;
      }
      else
      {
        if (score >= cutoff)
          sap = ProfSeqAlign (seqpos, prlnmx, strand, score, sip, topology);
        else
          sap = NULL;
      }
    }
    if (flagFirstInvocation)
    {
      score = 0.0;
      if (flagFindAllMatches)
      {
        SeqAlignLink (&saphead, sap);
        seq--;
        seqpos--;
      }
      else
      {
        return sap;
      }
    }
    else
    {
      return sap;
    }
    seq++;
    seqpos++;
  }
  return saphead;
}

static SeqAlignPtr ProfileMatchXInt (Uint1Ptr seq, Int4 seqpos,
                                     Int4 cutoff, Uint1 strand,
                                     SeqIdPtr sip, ComProfPtr ppp,
                                     Int4 scoremax, Int4 score,
                                     Int4 prlnmx, Int2 topology,
                                     Boolean flagFirstInvocation,
                                     Boolean flagFindAllMatches,
                                     Boolean flagBoundaryCondition)
{
  SeqAlignPtr  saphead = NULL, sap;
  FloatHi      lowcut, highcut;

  if (ppp == NULL || *seq == 0)
    return saphead;

  if (flagFirstInvocation)
  {
    score = 0;
    prlnmx = ProfLenMax (ppp);
    scoremax = (Int4) ProfScoreMax (ppp);
  }

  while (*seq != 0)
  {
    score += (ppp->iscore[*seq]);
    if (ppp->next != NULL)
    {
      seq++;
      seqpos++;
      sap = ProfileMatchXInt (seq, seqpos, cutoff, strand, sip, ppp->next,
                              scoremax, score, prlnmx, topology,
                              FALSE, FALSE, flagBoundaryCondition);
    }
    else
    {
      if (flagBoundaryCondition)
      {
        lowcut = cutoff * 0.8;
        highcut = cutoff + (cutoff - (cutoff * 0.8));
        if (score >= lowcut && score <= highcut)
          sap = ProfSeqAlign (seqpos, prlnmx, strand, (FloatHi) score, sip,
                              topology);
        else
          sap = NULL;
      }
      else
      {
        if (score >= cutoff)
          sap = ProfSeqAlign (seqpos, prlnmx, strand, (FloatHi) score, sip,
                              topology);
        else
          sap = NULL;
      }
    }
    if (flagFirstInvocation)
    {
      score = 0;
      if (flagFindAllMatches)
      {
        SeqAlignLink (&saphead, sap);
        seq--;
        seqpos--;
      }
      else
      {
        return sap;
      }
    }
    else
    {
      return sap;
    }
    seq++;
    seqpos++;
  }
  return saphead;
}

extern SeqAlignPtr ProfileMatch (Uint1Ptr seq, Int4 seqpos, FloatHi cutoff,
                                 Uint1 strand, SeqIdPtr sip, ComProfPtr ppp,
                                 Int2 topology, Boolean flagFindAllMatches,
                                 Boolean flagBoundaryCondition)
{
  return ProfileMatchX (seq, seqpos, cutoff, strand, sip, ppp,
                        0.0, 0.0, 0, topology, TRUE, flagFindAllMatches,
                        flagBoundaryCondition);
}

extern SeqAlignPtr ProfileMatchBioseq (BioseqPtr bsp, ComProfPtr ppp,
                                       ComProfPtr ippp, FloatHi cutoff,
                                       Boolean flagBoundaryCondition)
{
  SeqAlignPtr saphead = NULL, sap;
  SeqPortPtr  spp;
  Uint1Ptr    seq;
  Int4        proflenmax;
  Int4        seqchunk, start;

  if (ISA_na (bsp->mol))
  {
    spp = SeqPortNew (bsp, 0, bsp->length-1, 0, Seq_code_iupacna);
  }
  else
  {
    spp = SeqPortNew (bsp, 0, bsp->length-1, 0, Seq_code_iupacaa);
  }

  proflenmax = ProfLenMax (ppp);
  seqchunk = MAXCHUNKSIZE;
  start = 0;
  while ((seq = GetSeqChunk (spp, start, bsp->length,
                             seqchunk)) != NULL)
  {
    sap = ProfileMatchX (seq, start, cutoff, Seq_strand_plus, bsp->id,
                         ppp, 0.0, 0.0, 0, bsp->topology, TRUE, TRUE,
                         flagBoundaryCondition);
    SeqAlignLink (&saphead, sap);
    if (ippp != NULL)
    {
      sap = ProfileMatchX (seq, start, cutoff, Seq_strand_minus, bsp->id,
                           ippp, 0.0, 0.0, 0, bsp->topology, TRUE, TRUE,
                           flagBoundaryCondition);
      SeqAlignLink (&saphead, sap);
    }
    MemFree (seq);
    start += (seqchunk - proflenmax + 1);
  }
  SeqPortFree (spp);
  return saphead;
}

extern SeqAlignPtr IntProfileMatchBioseq (BioseqPtr bsp, ComProfPtr ppp,
                                          ComProfPtr ippp, Int4 cutoff,
                                          Boolean flagBoundaryCondition)
{
  SeqAlignPtr saphead = NULL, sap;
  SeqPortPtr  spp;
  Uint1Ptr    seq, seqtmp;
  Int4        proflenmax;
  Int4        seqchunk, start;

  if (ISA_na (bsp->mol))
  {
    spp = SeqPortNew (bsp, 0, bsp->length-1, 0, Seq_code_iupacna);
  }
  else
  {
    spp = SeqPortNew (bsp, 0, bsp->length-1, 0, Seq_code_iupacaa);
  }

  proflenmax = ProfLenMax (ppp);
  seqchunk = MAXCHUNKSIZE;
  start = 0;
  while ((seq = GetSeqChunk (spp, start, bsp->length,
                             seqchunk)) != NULL)
  {
    seqtmp = seq;
    while (*seqtmp != 0)
    {
      *seqtmp -= ' ';    /* OKAY -- shouldn't been blanks in seq */
      seqtmp++;
    }
    sap = ProfileMatchXInt (seq, start, cutoff, Seq_strand_plus, bsp->id,
                            ppp, 0, 0, 0, bsp->topology, TRUE, TRUE,
                            flagBoundaryCondition);
    SeqAlignLink (&saphead, sap);
    if (ippp != NULL)
    {
      sap = ProfileMatchXInt (seq, start, cutoff, Seq_strand_minus, bsp->id,
                              ippp, 0, 0, 0, bsp->topology, TRUE, TRUE,
                              flagBoundaryCondition);
      SeqAlignLink (&saphead, sap);
    }
    MemFree (seq);
    start += (seqchunk - proflenmax + 1);
  }
  SeqPortFree (spp);
  return saphead;
}

/* Molecular Weight */

static FloatHi CalcMolWt (BioseqPtr bsp, Int4 start, Int4 stop)
{
  SeqPortPtr spp;
  Uint1      residue;
  Int4       aacount;
  FloatHi    mw;
  FloatHi    mwlist[32] = {
    89.09,   /* A */
   132.61,   /* B */
   121.15,   /* C */
   133.10,   /* D */
   147.13,   /* E */
   165.19,   /* F */
    75.07,   /* G */
   155.16,   /* H */
   131.17,   /* I */
     0.00,   /* J */
   146.19,   /* K */
   131.17,   /* L */
   149.21,   /* M */
   132.12,   /* N */
     0.00,   /* O */
   115.13,   /* P */
   146.15,   /* Q */
   174.20,   /* R */
   105.09,   /* S */
   119.12,   /* T */
     0.00,   /* U */  /* mol wt of selenocysteine ? */
   117.15,   /* V */
   204.23,   /* W */
   136.90,   /* X */
   181.19,   /* Y */
   146.64};  /* Z */

  if (bsp == NULL)
    return 0.0;

  aacount = 0;
  mw = 0.0;
  spp = SeqPortNew (bsp, start, stop, 0, Seq_code_iupacaa);
  SeqPortSeek (spp, 0, SEEK_SET);
  while (start <= stop && (residue = SeqPortGetResidue (spp))
                           != SEQPORT_EOF)
  {
    if (residue != SEQPORT_EOS)
    {
      if (residue == '*' || residue == SEQPORT_VIRT)
        break;
    }
    residue = TO_UPPER (residue);
    mw += (mwlist[residue-'A']);
    aacount++;
    start++;
  }
  aacount--;
  mw -= (aacount * 18.0152);
  if (mw < 0.0)
    mw = 0.0;
  SeqPortFree (spp);
  return mw;
}

/* SeqAligns SeqLocs */

extern SeqLocPtr MatchSa2Sl (SeqAlignPtr PNTR sapp)
{
  SeqAlignPtr sap, sapn;
  StdSegPtr   ssp;
  SeqLocPtr   slp, slph = NULL;

  if (sapp == NULL)
    return NULL;

  sap = *sapp;
  while (sap != NULL)
  {
    if (sap->segtype == 3)
    {
      if ((ssp = sap->segs) != NULL)
      {
        if ((slp = ssp->loc) != NULL)
        {
          ValNodeLink (&slph, slp);
          ssp->loc = NULL;
        }
      }
    }
    sapn = sap->next;
    SeqAlignFree (sap);
    sap = sapn;
  }
  *sapp = sap;
  return slph;
}

extern void EmbedFragLengthInfo (SeqAlignPtr saph, Int4 length)
{
  SeqAlignPtr sap;
  StdSegPtr   ssp, sspt;
  SeqLocPtr   slp;
  SeqIntPtr   sint;
  SeqIdPtr    sip;
  Int4        start, startest;
  Int2        topology;
  Boolean     flagDone;

  if (saph == NULL)
    return;

/* get embedded topology */
  ssp = (StdSegPtr) saph->segs;
  topology = ssp->dim;
/* get first start site; therefor, should be ordered by start input */
  slp = ssp->loc;
  if (slp->choice == SEQLOC_MIX)
    slp = (SeqLocPtr) slp->data.ptrvalue;
  sint = (SeqIntPtr) slp->data.ptrvalue;
  startest = sint->from;
/* get seqid; not dup'd here */
  sip = SeqLocId (slp);

  sap = saph;
  while (sap != NULL)
  {
    ssp = (StdSegPtr) sap->segs;
    slp = ssp->loc;
    if (slp->choice == SEQLOC_MIX)
      slp = (SeqLocPtr) slp->data.ptrvalue;
    sint = (SeqIntPtr) slp->data.ptrvalue;
    start = sint->from;
    ScoreSetFree (ssp->scores);
    if ((ssp->scores = ScoreNew ()) != NULL)
    {
      if (sap->next != NULL)
      {
        sspt = (StdSegPtr) sap->next->segs;
        slp = sspt->loc;
        if (slp->choice == SEQLOC_MIX)
          slp = (SeqLocPtr) slp->data.ptrvalue;
        sint = (SeqIntPtr) slp->data.ptrvalue;
        ssp->scores->choice = 1;
        ssp->scores->value.intvalue = sint->from - start;
      }
      else
      {
        ssp->scores->value.intvalue = length - start;
      }
    }
    sap = sap->next;
  }

  if (topology == 2)     /* fuse circle */
  {
    sap = saph;
    while (sap->next != NULL)
      sap = sap->next;
    ssp = (StdSegPtr) sap->segs;
    ssp->scores->value.intvalue += startest;
  }
  else                   /* dummy 5' or Nterm */
  {
    if (startest != 0)
    {
      sap = saph;
      while (sap != NULL)
      {
        if (sap->dim == 0)
          break;
        sap = sap->next;
      }
      if (sap == NULL)
      {
        flagDone = FALSE;
        if ((sap = SeqAlignNew ()) != NULL)
        {
          sap->type = 255;    /* other */
          sap->segtype = 3;   /* std */
          sap->dim = 0;       /* dummy */
          if ((slp = SeqLocIntNew (0, startest, Seq_strand_plus, sip))
              != NULL)
          {
            if ((ssp = StdSegNew ()) != NULL)
            {
              ssp->dim = 1;
              ssp->ids = SeqIdDup (sip);
              ssp->loc = slp;
              sap->segs = ssp;
              if ((ssp->scores = ScoreNew ()) != NULL)
              {
                ssp->scores->choice = 1;
                ssp->scores->value.intvalue = startest + 1;
                flagDone = TRUE;
              }
            }
          }
          if (!flagDone)
            SeqAlignFree (sap);
          else
            SeqAlignLink (&saph, sap);
        }
      }
      else
      {
        ssp = (StdSegPtr) sap->segs;
        ScoreSetFree (ssp->scores);
        if ((ssp->scores = ScoreNew ()) != NULL)
        {
          ssp->scores->choice = 1;
          ssp->scores->value.intvalue = startest + 1;
        }
      }
    }
  }
  return;
}

extern void EmbedMolecularWeightInfo (SeqAlignPtr saph, BioseqPtr bsp)
{
  SeqAlignPtr sap;
  StdSegPtr   ssp;
  SeqLocPtr   slp;
  SeqIntPtr   sint;
  Int4        start, stop;
  Boolean     flagDone;

  if (saph == NULL || bsp == NULL)
    return;
  if (!ISA_aa (bsp->mol))
    return;

  start = 0;
  stop = 0;
  sap = saph;
  while (sap != NULL)
  {
    ssp = (StdSegPtr) sap->segs;
    slp = ssp->loc;
    sint = (SeqIntPtr) slp->data.ptrvalue;
    sint->from = start;
    stop = SeqLocStop (slp);    /* circular protein prb ? */
    ScoreSetFree (ssp->scores);
    if ((ssp->scores = ScoreNew ()) != NULL)
    {
      ssp->scores->choice = 2;
      ssp->scores->value.realvalue = CalcMolWt (bsp, start, stop);
    }
    start = stop + 1;
    sap = sap->next;
  }

  if (stop != bsp->length - 1)
  {
    start = stop + 1;
    stop = bsp->length - 1;
    flagDone = FALSE;
    if ((sap = SeqAlignNew ()) != NULL)
    {
      sap->type = 255;    /* other */
      sap->segtype = 3;   /* std */
      sap->dim = 0;       /* dummy */
      if ((slp = SeqLocIntNew (start, stop, Seq_strand_plus, bsp->id))
          != NULL)
      {
        if ((ssp = StdSegNew ()) != NULL)
        {
          ssp->dim = 1;
          ssp->ids = SeqIdDup (bsp->id);
          ssp->loc = slp;
          sap->segs = ssp;
          if ((ssp->scores = ScoreNew ()) != NULL)
          {
            ssp->scores->choice = 2;
            ssp->scores->value.realvalue = CalcMolWt (bsp, start, stop);
            flagDone = TRUE;
          }
        }
      }
      if (!flagDone)
        SeqAlignFree (sap);
      else
        SeqAlignLink (&saph, sap);
    }
  }
  return;
}

static int LIBCALLBACK SeqAlignStartCompProc (VoidPtr ptr1, VoidPtr ptr2)
{
  SeqAlignPtr sap1, sap2;
  StdSegPtr   ssp1, ssp2;
  SeqLocPtr   slp1, slp2, slpt;
  Int4        start1, start2;

  if (ptr1 != NULL && ptr2 != NULL)
  {
    sap1 = *((SeqAlignPtr PNTR) ptr1);
    sap2 = *((SeqAlignPtr PNTR) ptr2);
    if (sap1 != NULL && sap2 != NULL)
    {
      if (sap1->segtype == 3 && sap2->segtype == 3)
      {
        ssp1 = sap1->segs;
        ssp2 = sap2->segs;
        if (ssp1 != NULL && ssp2 != NULL)
        {
          slp1 = ssp1->loc;
          slp2 = ssp2->loc;
          if (slp1 != NULL && slp2 != NULL)
          {
            if (slp1->choice != SEQLOC_MIX)
            {
              start1 = SeqLocStart (slp1);
            }
            else
            {
              slpt = slp1->data.ptrvalue;
              start1 = SeqLocStart (slpt);
            }
            if (slp2->choice != SEQLOC_MIX)
            {
              start2 = SeqLocStart (slp2);
            }
            else
            {
              slpt = slp2->data.ptrvalue;
              start2 = SeqLocStart (slpt);
            }
            if (start1 > start2)
              return 1;
            else if (start1 < start2)
              return -1;
          }
        }
      }
    }
  }
  return 0;
}

extern void URK_SeqAlignSortByStart (SeqAlignPtr PNTR sap)
{
  SeqAlignPtr sapf, sapt, PNTR saph;
  Int4 num, i;

  num = 0;
  sapt = *sap;
  while (sapt != NULL)
  {
    if (sapt->dim != 0)
      num++;
    sapt = sapt->next;
  }

  if (num > 1)
  {
    saph = MemNew ((size_t) ((num+1) * sizeof (SeqAlignPtr)));
    i = 0;
    sapt = *sap;
    while (sapt != NULL && i < num)
    {
      if (sapt->dim != 0)
      {
        saph[i] = sapt;
        sapt = sapt->next;
        i++;
      }
      else
      {
        sapf = sapt;
        sapt = sapt->next;
        SeqAlignFree (sapf);
      }
    }
    HeapSort (saph, num, sizeof (SeqAlignPtr), SeqAlignStartCompProc);
    for (i = 0; i < num; ++i)
    {
      sapt = saph[i];
      sapt->next = saph[i+1];
    }
    *sap = saph[0];
    MemFree (saph);
  }
  return;
}

static int LIBCALLBACK SeqAlignLenCompProc (VoidPtr ptr1, VoidPtr ptr2)
{
  SeqAlignPtr sap1, sap2;
  StdSegPtr   ssp1, ssp2;

  if (ptr1 != NULL && ptr2 != NULL)
  {
    sap1 = *((SeqAlignPtr PNTR) ptr1);
    sap2 = *((SeqAlignPtr PNTR) ptr2);
    if (sap1 != NULL && sap2 != NULL)
    {
      if (sap1->segtype == 3 && sap2->segtype == 3)
      {
        ssp1 = sap1->segs;
        ssp2 = sap2->segs;
        if (ssp1 != NULL && ssp2 != NULL)
        {
          if (ssp1->scores->value.intvalue > ssp2->scores->value.intvalue)
            return 1;
          else if (ssp1->scores->value.intvalue < ssp2->scores->value.intvalue)
            return -1;
        }
      }
    }
  }
  return 0;
}

extern void URK_SeqAlignSortByLength (SeqAlignPtr PNTR sap)
{
  SeqAlignPtr sapt, PNTR saph;
  Int4 num, i;

  num = 0;
  sapt = *sap;
  while (sapt != NULL)
  {
    num++;
    sapt = sapt->next;
  }

  if (num > 1)
  {
    saph = MemNew ((size_t) ((num+1) * sizeof (SeqAlignPtr)));
    i = 0;
    sapt = *sap;
    while (sapt != NULL && i < num)
    {
      saph[i] = sapt;
      sapt = sapt->next;
      i++;
    }
    HeapSort (saph, num, sizeof (SeqAlignPtr), SeqAlignLenCompProc);
    for (i = 0; i < num; ++i)
    {
      sapt = saph[i];
      sapt->next = saph[i+1];
    }
    *sap = saph[0];
    MemFree (saph);
  }
  return;
}

static int LIBCALLBACK SeqAlignMWCompProc (VoidPtr ptr1, VoidPtr ptr2)
{
  SeqAlignPtr sap1, sap2;
  StdSegPtr   ssp1, ssp2;

  if (ptr1 != NULL && ptr2 != NULL)
  {
    sap1 = *((SeqAlignPtr PNTR) ptr1);
    sap2 = *((SeqAlignPtr PNTR) ptr2);
    if (sap1 != NULL && sap2 != NULL)
    {
      if (sap1->segtype == 3 && sap2->segtype == 3)
      {
        ssp1 = sap1->segs;
        ssp2 = sap2->segs;
        if (ssp1 != NULL && ssp2 != NULL)
        {
          if (ssp1->scores->value.realvalue >
              ssp2->scores->value.realvalue)
            return 1;
          else if (ssp1->scores->value.realvalue <
                   ssp2->scores->value.realvalue)
            return -1;
        }
      }
    }
  }
  return 0;
}

extern void URK_SeqAlignSortByMolWt (SeqAlignPtr PNTR sap)
{
  SeqAlignPtr sapt, PNTR saph;
  Int4 num, i;

  num = 0;
  sapt = *sap;
  while (sapt != NULL)
  {
    num++;
    sapt = sapt->next;
  }

  if (num > 1)
  {
    saph = MemNew ((size_t) ((num+1) * sizeof (SeqAlignPtr)));
    i = 0;
    sapt = *sap;
    while (sapt != NULL && i < num)
    {
      saph[i] = sapt;
      sapt = sapt->next;
      i++;
    }
    HeapSort (saph, num, sizeof (SeqAlignPtr), SeqAlignMWCompProc);
    for (i = 0; i < num; ++i)
    {
      sapt = saph[i];
      sapt->next = saph[i+1];
    }
    *sap = saph[0];
    MemFree (saph);
  }
  return;
}

static int LIBCALLBACK SeqAlignScoreCompProc (VoidPtr ptr1, VoidPtr ptr2)
{
  SeqAlignPtr sap1, sap2;

  if (ptr1 != NULL && ptr2 != NULL)
  {
    sap1 = *((SeqAlignPtr PNTR) ptr1);
    sap2 = *((SeqAlignPtr PNTR) ptr2);
    if (sap1 != NULL && sap2 != NULL)
    {
      if (sap1->score->value.realvalue >
          sap2->score->value.realvalue)
        return 1;
      else if (sap1->score->value.realvalue <
               sap2->score->value.realvalue)
        return -1;
    }
  }
  return 0;
}

extern void URK_SeqAlignSortByScore (SeqAlignPtr PNTR sap)
{
  SeqAlignPtr sapt, PNTR saph;
  Int4 num, i;

  num = 0;
  sapt = *sap;
  while (sapt != NULL)
  {
    num++;
    sapt = sapt->next;
  }

  if (num > 1)
  {
    saph = MemNew ((size_t) ((num+1) * sizeof (SeqAlignPtr)));
    i = 0;
    sapt = *sap;
    while (sapt != NULL && i < num)
    {
      saph[i] = sapt;
      sapt = sapt->next;
      i++;
    }
    HeapSort (saph, num, sizeof (SeqAlignPtr), SeqAlignScoreCompProc);
    for (i = 0; i < num; ++i)
    {
      sapt = saph[i];
      sapt->next = saph[i+1];
    }
    *sap = saph[0];
    MemFree (saph);
  }
  return;
}
