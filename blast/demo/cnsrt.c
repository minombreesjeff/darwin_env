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
* File Name: cnsrt.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.4 $
*
* File Description: consort - codon usage
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: cnsrt.c,v $
* Revision 6.4  1998/12/18 16:24:53  kuzio
* big GIs
*
* Revision 6.3  1998/09/16 18:19:27  kuzio
* cvs logging
*
* ==========================================================================
*/

#include <ncbi.h>
#include <accentr.h>
#include <gather.h>
#include <urkcnsrt.h>

#define TOP_ERROR 1
static char _this_module[] = "consort";
#undef  THIS_MODULE
#define THIS_MODULE _this_module
static char _this_file[] = __FILE__;
#undef  THIS_FILE
#define THIS_FILE _this_file

Args myargs[] =
{
  { "nucleotide GI", "0", "0", "9000000", TRUE,
    'g', ARG_INT, 0.0, 0, NULL},
  { "ASN.1 SeqEntry", NULL, NULL, NULL, TRUE,
    'f', ARG_STRING, 0.0, 0, NULL}
};

Int2 Main (void)
{
  Int2        argcount;
  Boolean     flagHaveNet, flagTakeNext;
  Int4        gi;
  CharPtr     filename;
  AsnIoPtr    aiop;
  SeqEntryPtr sep;
  TreeNodePtr ptrNode, ptrNodeFind;
  Char        nameL[256], nameR[256];
  Int4Ptr     freq;

  argcount = sizeof (myargs) / sizeof (Args);
  if (!GetArgs ("consort", argcount, myargs))
    return 1;

  gi = myargs[0].intvalue;
  filename = myargs[1].strvalue;

  if (gi > 0)
  {
    if (!EntrezInit ("consort", FALSE, &flagHaveNet))
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 101,
                 "Entrez init failed");
      ErrShow ();
      exit (1);
    }
    sep = EntrezSeqEntryGet (myargs[0].intvalue, SEQENTRY_READ_BIOSEQ);
    EntrezFini ();
  }
  else if (filename != NULL)
  {
    if ((aiop = AsnIoOpen (filename, "r")) == NULL)
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 102,
                 "Failed to open: %s", filename);
      ErrShow ();
    }
    sep = SeqEntryAsnRead (aiop, NULL);
    AsnIoClose (aiop);
  }
  else
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 103,
               "No gi or ASN.1 file given :: for help :   cnsrt -");
    ErrShow ();
    exit (1);
  }

  if (sep == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 104,
               "No seqentry found");
    ErrShow ();
    exit (1);
  }

  if ((freq = ConformSeqEntry (sep)) != NULL)
    Conform (freq, stdout);
  ptrNode = ConsortSeqEntry (sep);
  SeqEntryFree (sep);

  SetAllNumberNodes (ptrNode);
  printf ("%4s %24s %24s %16s\n", "Node", "Left", "Right", "Score");

  ptrNodeFind = NULL;
  flagTakeNext = FALSE;
  while ((ptrNodeFind = ExploreTree (ptrNode, ptrNodeFind,
         &flagTakeNext)) != NULL)
  {
    if (ptrNodeFind->ptrChildLeft != NULL &&
        ptrNodeFind->ptrChildRight != NULL)
    {
      StrCpy (nameL, ptrNodeFind->ptrChildLeft->name);
      if (StrLen (nameL) > 24)
        nameL[24] = '\0';
      StrCpy (nameR, ptrNodeFind->ptrChildRight->name);
      if (StrLen (nameR) > 24)
        nameR[24] = '\0';
      printf ("%4ld %24s %24s %16.5f\n", ptrNodeFind->nodenumber,
              nameL, nameR, ptrNodeFind->score);
    }
    flagTakeNext = FALSE;
  }

  return 0;
}
