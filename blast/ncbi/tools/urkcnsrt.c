static char const rcsid[] = "$Id: urkcnsrt.c,v 6.21 2003/05/30 17:25:38 coulouri Exp $";

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
* File Name: urkcnsrt.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.21 $
*
* File Description: consort
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: urkcnsrt.c,v $
* Revision 6.21  2003/05/30 17:25:38  coulouri
* add rcsid
*
* Revision 6.20  2000/02/09 20:29:04  kans
* changed FindNuc to avoid linker conflicts on newer alpha compiler
*
* Revision 6.19  1999/04/02 20:13:17  kans
* cast added
*
* Revision 6.18  1999/03/12 14:08:05  kuzio
* cast
*
* Revision 6.17  1998/10/20 16:07:40  kuzio
* one pass: hash-sort-count for Markov
*
* Revision 6.16  1998/10/20 14:33:37  kuzio
* var name readability
*
* Revision 6.15  1998/10/19 18:23:25  kuzio
* SeqPortRead in chunks
*
* Revision 6.14  1998/10/13 17:16:36  kuzio
* additional Markov setup
*
* Revision 6.13  1998/10/05 13:35:15  kuzio
* start markov chains and codon bias
*
* Revision 6.12  1998/09/28 16:36:10  kuzio
* no met orf check
*
* Revision 6.11  1998/09/16 18:03:32  kuzio
* cvs logging
*
*
* ==========================================================================
*/

#include <urkcnsrt.h>
#include <gather.h>
#include <seqport.h>

/* CONDONE */

static FloatHi findtot (Int4Ptr fq, Int4 index)
{
  Int4 score;

  switch (index)
  {
   default:
   case 0:         /* lys K */
   case 1:
    score = fq[0] + fq[1];
    break;
   case 2:         /* asn N */
   case 3:
    score = fq[2] + fq[3];
    break;
   case 4:         /* arg R */
   case 5:
   case 36:
   case 37:
   case 38:
   case 39:
    score = fq[4] + fq[5] + fq[36] + fq[37] + fq[38] + fq[39];
    break;
   case 6:         /* ser S */
   case 7:
   case 56:
   case 57:
   case 58:
   case 59:
    score = fq[6] + fq[7] + fq[56] + fq[57] + fq[58] + fq[59];
    break;
   case 8:         /* thr T */
   case 9:
   case 10:
   case 11:
    score = fq[8] + fq[9] + fq[10] + fq[11];
    break;
   case 12:        /* ile I */
   case 14:
   case 15:
    score = fq[12] + fq[14] + fq[15];
    break;
   case 13:        /* met M */
    score = fq[13];
    break;
   case 16:        /* glu E */
   case 17:
    score = fq[16] + fq[17];
    break;
   case 18:        /* asp D */
   case 19:
    score = fq[18] + fq[19];
    break;
   case 20:        /* gly G */
   case 21:
   case 22:
   case 23:
    score = fq[20] + fq[21] + fq[22] + fq[23];
    break;
   case 24:        /* ala A */
   case 25:
   case 26:
   case 27:
    score = fq[24] + fq[25] + fq[26] + fq[27];
    break;
   case 28:        /* val V */
   case 29:
   case 30:
   case 31:
    score = fq[28] + fq[29] + fq[30] + fq[31];
    break;
   case 32:        /* gln N */
   case 33:
    score = fq[32] + fq[33];
    break;
   case 34:        /* his H */
   case 35:
    score = fq[34] + fq[35];
    break;
   case 40:        /* pro P */
   case 41:
   case 42:
   case 43:
    score = fq[40] + fq[41] + fq[42] + fq[43];
    break;
   case 44:        /* leu L */
   case 45:
   case 46:
   case 47:
   case 60:
   case 61:
    score = fq[44] + fq[45] + fq[46] + fq[47] + fq[60] + fq[61];
    break;
   case 48:        /* ochre UAA */
   case 49:        /* amber UAG */
   case 52:        /* umber UGA */
    score = fq[48] + fq[49] + fq[52];
    break;
   case 50:        /* tyr Y */
   case 51:
    score = fq[50] + fq[51];
    break;
   case 53:        /* trp W */
    score = fq[53];
    break;
   case 54:        /* cys C */
   case 55:
    score = fq[54] + fq[55];
    break;
   case 62:        /* phe F */
   case 63:
    score = fq[62] + fq[63];
    break;
  }
  return (FloatHi) score;
}

static Int4Ptr Condone (SeqPortPtr spp, Int4Ptr codontable, Uint1 strand,
                        Boolean flagAdd)
{
  Boolean flagNextCodon = TRUE;
  Uint1   na;
  Int4    nn, nx;

/*
 note:
 use A G C T in switch statement rather than A C G T (alphabetical)
     because the order comes out nicer in findtot switch statement
     it's biological
*/
  SeqPortSeek (spp, 0, SEEK_SET);
  while (flagNextCodon)
  {
    nn = 0;
    na = '\0';
    while (na < 'A' || na > 'Z')
    {
      if ((na = SeqPortGetResidue (spp)) == SEQPORT_EOF)
        flagNextCodon = FALSE;
      if (na < 'A' || na > 'Z')
        flagNextCodon = FALSE;
      if (!flagNextCodon)
        break;
    }
    if (!flagNextCodon)
      continue;
    switch (na)
    {
     default:
     case 'A':
      nx = 0;
      break;
     case 'G':
      nx = 1;
      break;
     case 'C':
      nx = 2;
      break;
     case 'T':
      nx = 3;
      break;
    }
    if (strand == Seq_strand_minus)
      nn += (3 - nx);
    else
      nn += (nx * 16);
    na = '\0';
    while (na < 'A' || na > 'Z')
    {
      if ((na = SeqPortGetResidue (spp)) == SEQPORT_EOF)
        flagNextCodon = FALSE;
      if (na < 'A' || na > 'Z')
        flagNextCodon = FALSE;
      if (!flagNextCodon)
        break;
    }
    if (!flagNextCodon)
      continue;
    switch (na)
    {
     default:
     case 'A':
      nx = 0;
      break;
     case 'G':
      nx = 1;
      break;
     case 'C':
      nx = 2;
      break;
     case 'T':
      nx = 3;
      break;
    }
    if (strand == Seq_strand_minus)
      nn += ((3 - nx) * 4);
    else
      nn += (nx * 4);
    na = '\0';
    while (na < 'A' || na > 'Z')
    {
      if ((na = SeqPortGetResidue (spp)) == SEQPORT_EOF)
        flagNextCodon = FALSE;
      if (na < 'A' || na > 'Z')
        flagNextCodon = FALSE;
      if (!flagNextCodon)
        break;
    }
    if (!flagNextCodon)
      continue;
    switch (na)
    {
     default:
     case 'A':
      nx = 0;
      break;
     case 'G':
      nx = 1;
      break;
     case 'C':
      nx = 2;
      break;
     case 'T':
      nx = 3;
      break;
    }
    if (strand == Seq_strand_minus)
      nn += ((3 - nx) * 16);
    else
      nn += nx;
    if (flagAdd)
      codontable[nn] += 1;
    else
      codontable[nn] -= 1;
  }
  return codontable;
}

/* CONCUR */

static FloatHi Concur (Int4Ptr freq1, Int4Ptr freq2, Int4Ptr freqgbl)
{
  Int4    i;
  Int4    freqlcl[64];
  FloatHi st1, st2, fr1, fr2, frq, score;

  if (freq1 == NULL || freq2 == NULL)
    return 0.0;

  if (freqgbl == NULL)
  {
    for (i = 0; i < 64; i++)
    {
      freqlcl[i] = (freq1[i] + freq2[i]) / 2;
      if (freqlcl[i] == 0)
        freqlcl[i] = 1;
    }
    freqgbl = freqlcl;
  }
  score = 0.0;
  for (i = 0; i < 64; i++)
  {
    st1 = findtot (freq1, i);
    if (st1 == 0)
      st1 = 1;
    st2 = findtot (freq2, i);
    if (st2 == 0)
      st2 = 1;
    fr1 = freq1[i]/st1;
    fr2 = freq2[i]/st2;
    frq = fr1 - fr2;
    score += (((FloatHi) frq * (FloatHi) frq) / (FloatHi) freqgbl[i]);
  }
  return score;
}

/* CONSORT */

static TreeNodePtr Consort (FloatHiPtr PNTR conmat, Int4 matsize,
                            CharPtr PNTR coname)
{
  ValNodePtr vnphead = NULL, vnp = NULL;
  TreeNodePtr ptrNode;
  Int4       i, n, min1, min2;
  FloatHi    ftemp, minscore;
  TreeNodePtr ptrFirst, ptrLast;

  for (i = 0; i < matsize; i++)
  {
    vnp = ValNodeNew (vnp);
    if (vnphead == NULL)
      vnphead = vnp;
    ptrNode = TreeNodeNew ();
    vnp->data.ptrvalue = ptrNode;
    ptrNode->name = StringSave (coname[i]);
  }

  while (matsize > 2)
  {
/* find lowest non-trivial score */
    min1 = 0;
    min2 = 1;
    minscore = conmat[min1][min2];
    for (n = 0; n < matsize-1; n++)
    {
      for (i = n+1; i < matsize; i++)
      {
        if (minscore > conmat[n][i])
        {
          minscore = conmat[n][i];
          min1 = n;
          min2 = i;
        }
      }
    }
    if (min2 < min1)
    {
      i = min2;
      min2 = min1;
      min1 = i;
    }

/* average the low scoring columns */
    for (i = 0; i < matsize; i++)
    {
      ftemp = (conmat[i][min1] + conmat[i][min2]) / 2;
      conmat[i][min1] = ftemp;
      conmat[i][min2] = ftemp;
    }
/* average the low scoring rows */
    for (i = 0; i < matsize; i++)
    {
      ftemp = (conmat[min1][i] + conmat[min2][i]) / 2;
      conmat[min1][i] = ftemp;
      conmat[min2][i] = ftemp;
    }
/* exchange 2nd low column with last column */
    for (i = 0; i < matsize; i++)
    {
      ftemp = conmat[i][min2];
      conmat[i][min2] = conmat[i][matsize-1];
      conmat[i][matsize-1] = ftemp;
    }
/* exchange 1st low column with 2nd last column */
    for (i = 0; i < matsize; i++)
    {
      ftemp = conmat[i][min1];
      conmat[i][min1] = conmat[i][matsize-2];
      conmat[i][matsize-2] = ftemp;
    }
/* exchange 2nd low row with last row */
    for (i = 0; i < matsize; i++)
    {
      ftemp = conmat[min2][i];
      conmat[min2][i] = conmat[matsize-1][i];
      conmat[matsize-1][i] = ftemp;
    }
/* exchange 1st low row with 2nd last row */
    for (i = 0; i < matsize; i++)
    {
      ftemp = conmat[min1][i];
      conmat[min1][i] = conmat[matsize-2][i];
      conmat[matsize-2][i] = ftemp;
    }

/* exchange 2nd low node with last node */
    vnp = vnphead;
    for (i = 0; i < min2; i++)
      vnp = vnp->next;
    ptrFirst = (TreeNodePtr) vnp->data.ptrvalue;
    for (i = min2+1; i < matsize; i++)
      vnp = vnp->next;
    ptrLast = (TreeNodePtr) vnp->data.ptrvalue;
    vnp = vnphead;
    for (i = 0; i < min2; i++)
      vnp = vnp->next;
    vnp->data.ptrvalue = ptrLast;
    for (i = min2+1; i < matsize; i++)
      vnp = vnp->next;
    vnp->data.ptrvalue = ptrFirst;

/* exchange 1st low node with 2nd last node */
    vnp = vnphead;
    for (i = 0; i < min1; i++)
      vnp = vnp->next;
    ptrFirst = (TreeNodePtr) vnp->data.ptrvalue;
    for (i = min1+1; i < matsize-1; i++)
      vnp = vnp->next;
    ptrLast = (TreeNodePtr) vnp->data.ptrvalue;
    vnp = vnphead;
    for (i = 0; i < min1; i++)
      vnp = vnp->next;
    vnp->data.ptrvalue = ptrLast;
    for (i = min1+1; i < matsize-1; i++)
      vnp = vnp->next;
    vnp->data.ptrvalue = ptrFirst;

/*
 fuse 2nd last and last nodes
 new node will be last node after matrix shrinks
*/
    ptrNode = TreeNodeNew ();
    ptrNode->name = StringSave ("Internal Node");
    ptrNode->score = minscore;
    ptrNode->ptrChildLeft = (TreeNodePtr) vnp->data.ptrvalue;
    ptrNode->ptrChildRight = (TreeNodePtr) vnp->next->data.ptrvalue;
    ptrNode->ptrChildLeft->ptrUp = ptrNode;
    ptrNode->ptrChildRight->ptrUp = ptrNode;
    vnp->data.ptrvalue = ptrNode;

/* shrink matrix */
    matsize--;
  }

/* fuse the remaining two nodes at the root node */
  vnp = vnphead;
  ptrNode = TreeNodeNew ();
  ptrNode->name = StringSave ("Root Node");
  ptrNode->score = conmat[0][1];
  ptrNode->ptrChildLeft = (TreeNodePtr) vnp->data.ptrvalue;
  ptrNode->ptrChildRight = (TreeNodePtr) vnp->next->data.ptrvalue;
  ptrNode->ptrChildLeft->ptrUp = ptrNode;
  ptrNode->ptrChildRight->ptrUp = ptrNode;

/* just free the vnp store */
  ValNodeFree (vnphead);

  return ptrNode;
}

/* CONFORM */

static Int4 whattotal (Int4Ptr freq)
{
  Int4  i, codons = 0;

  for (i = 0; i < 64; i++)
  {
    codons += *freq;
    freq++;
  }
  return codons;
}

static FloatHi whatthou (Int4Ptr freq, Int4 count, Int4 codons)
{
  return ((FloatHi) freq[count] / (FloatHi) codons * 1000.0);
}

/* main outside connections */

typedef struct gather_Cds
{
  SeqEntryPtr     sep;
  BioseqPtr       bsp;
  Int4            globaltable[64];
  Int4            gene1table[64];
  Int4            gene2table[64];
  Int4            genecount, genecurrent, genelocal;
  CharPtr PNTR    genename;
  FloatHiPtr PNTR conmat;
  TreeNodePtr     ptrNode;
} GatherCDS, PNTR GatherCDSPtr;

static Boolean getgene1cdn (GatherContextPtr gcp)
{
  GatherCDSPtr gcdsp;
  SeqAnnotPtr  sap;
  SeqFeatPtr   sfp;
  Int4         start, stop;
  SeqPortPtr   spp;
  Uint1        strand;

  GBQualPtr    gbqp;
  ValNodePtr   vnp;
  DbtagPtr     dtp;
  ObjectIdPtr  oip;
  Char         buf[16];

  if (gcp == NULL)
    return FALSE;
  if ((gcdsp = (GatherCDSPtr) gcp->userdata) == NULL)
    return FALSE;
  if (gcp->thistype != OBJ_SEQANNOT)
    return TRUE;
  if ((sap = (SeqAnnotPtr) (gcp->thisitem)) == NULL)
    return TRUE;

  while (sap != NULL)
  {
    sfp = (SeqFeatPtr) sap->data;
    while (sfp != NULL && gcdsp->genelocal < CONMATSIZE)
    {
      if (sfp->data.choice == SEQFEAT_CDREGION)
      {
        if (sfp->location->choice == SEQLOC_INT)
        {
          start = SeqLocStart (sfp->location);
          stop = SeqLocStop (sfp->location);
          strand = SeqLocStrand (sfp->location);
          if ((spp = SeqPortNew (gcdsp->bsp, start, stop, 0,
                                 Seq_code_iupacna)) != NULL)
          {
            if (gcdsp->genelocal == gcdsp->genecurrent)
            {
              MemSet ((Pointer) gcdsp->gene1table, 0,
                (size_t) (64 * sizeof (Int4)));
              Condone (spp, gcdsp->gene1table, strand, TRUE);
              buf[0] = '\0';
              if (sfp->qual != NULL)
              {
                gbqp = sfp->qual;
                if (gbqp->val != NULL)
                {
                  StrNCpy (buf, gbqp->val, 15);
                  buf[15] = '\0';
                }
              }
              else if (sfp->comment != NULL)
              {
                StrNCpy (buf, sfp->comment, 15);
                buf[15] = '\0';
              }
              else if (sfp->dbxref != NULL)
              {
                vnp = sfp->dbxref;
                if (vnp != NULL)
                {
                  dtp = vnp->data.ptrvalue;
                  if (dtp != NULL)
                  {
                    oip = dtp->tag;
                    if (oip != NULL)
                    {
                      StrNCpy (buf, oip->str, 15);
                      buf[15] = '\0';
                    }
                  }
                }
              }
              else
              {
                sprintf (buf, "ORF_%ld", (long) (gcdsp->genecurrent+1));
              }
              if (buf[0] == '\0')
              {
                sprintf (gcdsp->genename[gcdsp->genecurrent],
                         "ORF_%ld", (long) (gcdsp->genecurrent+1));
              }
              else
              {
                StrCpy (gcdsp->genename[gcdsp->genecurrent], buf);
              }
              gcdsp->genelocal += 1;
              SeqPortFree (spp);
              return TRUE;
            }
            gcdsp->genelocal += 1;
            SeqPortFree (spp);
          }
        }
      }
      sfp = sfp->next;
    }
    sap = sap->next;
  }
  return TRUE;
}

static Boolean getgene2cdn (GatherContextPtr gcp)
{
  GatherCDSPtr gcdsp;
  SeqAnnotPtr  sap;
  SeqFeatPtr   sfp;
  Int4         start, stop;
  SeqPortPtr   spp;
  Uint1        strand;

  if (gcp == NULL)
    return FALSE;
  if ((gcdsp = (GatherCDSPtr) gcp->userdata) == NULL)
    return FALSE;
  if (gcp->thistype != OBJ_SEQANNOT)
    return TRUE;
  if ((sap = (SeqAnnotPtr) (gcp->thisitem)) == NULL)
    return TRUE;

  while (sap != NULL)
  {
    sfp = (SeqFeatPtr) sap->data;
    while (sfp != NULL && gcdsp->genelocal < CONMATSIZE)
    {
      if (sfp->data.choice == SEQFEAT_CDREGION)
      {
        if (sfp->location->choice == SEQLOC_INT)
        {
          start = SeqLocStart (sfp->location);
          stop = SeqLocStop (sfp->location);
          strand = SeqLocStrand (sfp->location);
          if ((spp = SeqPortNew (gcdsp->bsp, start, stop, 0,
                                 Seq_code_iupacna)) != NULL)
          {
            MemSet ((Pointer) gcdsp->gene2table, 0,
              (size_t) (64 * sizeof (Int4)));
            Condone (spp, gcdsp->gene2table, strand, TRUE);
            SeqPortFree (spp);
            gcdsp->conmat[gcdsp->genecurrent][gcdsp->genelocal] =
              Concur (gcdsp->gene1table, gcdsp->gene2table,
                      gcdsp->globaltable);
            gcdsp->genelocal += 1;
          }
        }
      }
      sfp = sfp->next;
    }
    sap = sap->next;
  }
  return TRUE;
}

static Boolean getgblcdn (GatherContextPtr gcp)
{
  GatherCDSPtr gcdsp;
  SeqAnnotPtr  sap;
  SeqFeatPtr   sfp;
  Int4         start, stop;
  SeqPortPtr   spp;
  Uint1        strand;

  if (gcp == NULL)
    return FALSE;
  if ((gcdsp = (GatherCDSPtr) gcp->userdata) == NULL)
    return FALSE;
  if (gcp->thistype != OBJ_SEQANNOT)
    return TRUE;
  if ((sap = (SeqAnnotPtr) (gcp->thisitem)) == NULL)
    return TRUE;

  while (sap != NULL)
  {
    sfp = (SeqFeatPtr) sap->data;
    while (sfp != NULL && gcdsp->genecount < CONMATSIZE)
    {
      if (sfp->data.choice == SEQFEAT_CDREGION)
      {
        if (sfp->location->choice == SEQLOC_INT)
        {
          start = SeqLocStart (sfp->location);
          stop = SeqLocStop (sfp->location);
          strand = SeqLocStrand (sfp->location);
          if ((spp = SeqPortNew (gcdsp->bsp, start, stop, 0,
                                 Seq_code_iupacna)) != NULL)
          {
            Condone (spp, gcdsp->globaltable, strand, TRUE);
            gcdsp->genecount += 1;
            SeqPortFree (spp);
          }
        }
      }
      sfp = sfp->next;
    }
    sap = sap->next;
  }
  return TRUE;
}

static void UrkcnsrtFindNuc (SeqEntryPtr sep, Pointer data, Int4 index,
                     Int2 indent)
{
  BioseqPtr PNTR bspp;
  BioseqPtr bsp;

  bspp = (BioseqPtr PNTR) data;

  if (*bspp != NULL)
    return;

  if (IS_Bioseq (sep))
  {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (ISA_na (bsp->mol))
      *bspp = bsp;
  }
  return;
}

extern TreeNodePtr ConsortSeqEntry (SeqEntryPtr sep)
{
  BioseqPtr           bsp = NULL;
  Int4                i, genecount;

  static GatherScope  gs;
  GatherScopePtr      gsp;

  static GatherCDS    gcds;
  GatherCDSPtr        gcdsp;

  SeqEntryExplore (sep, &bsp, UrkcnsrtFindNuc);
  if (bsp == NULL)
    return NULL;

  gsp = &gs;
  gcdsp = &gcds;

  MemSet ((Pointer) gsp, 0, sizeof (GatherScope));
  MemSet ((Pointer) gsp->ignore, (int) (TRUE),
          (size_t) (OBJ_MAX * sizeof (Boolean)));
  gsp->ignore[OBJ_SEQANNOT] = FALSE;

  gcdsp->genename = MemNew ((size_t) (CONMATSIZE * sizeof (Pointer)));
  for (i = 0; i < CONMATSIZE; i++)
    gcdsp->genename[i] = MemNew (64 * sizeof (Char));
  gcdsp->conmat = MemNew (CONMATSIZE * sizeof (Pointer));
  for (i = 0; i < CONMATSIZE; i++)
    gcdsp->conmat[i] = MemNew ((size_t) (CONMATSIZE * sizeof (FloatHi)));

  MemSet ((Pointer) gcdsp->globaltable, 0,
          (size_t) (64 * sizeof (Int4)));

  gcdsp->sep       = sep;
  gcdsp->bsp       = bsp;
  gcdsp->ptrNode   = NULL;
  gcdsp->genecount = 0;

  GatherSeqEntry (sep, (Pointer) gcdsp, getgblcdn, (Pointer) gsp);
  for (i = 0; i < 64; i++)
  {
    if (gcdsp->globaltable[i] == 0)
      gcdsp->globaltable[i] = 1;
  }

  for (genecount = 0; genecount < gcdsp->genecount; genecount++)
  {
    gcdsp->genecurrent = genecount;
    gcdsp->genelocal = 0;

    MemSet ((Pointer) gsp, 0, sizeof (GatherScope));
    MemSet ((Pointer) gsp->ignore, (int) (TRUE),
            (size_t) (OBJ_MAX * sizeof (Boolean)));
    gsp->ignore[OBJ_SEQANNOT] = FALSE;

    GatherSeqEntry (sep, (Pointer) gcdsp, getgene1cdn, (Pointer) gsp);

    gcdsp->genelocal = 0;

    MemSet ((Pointer) gsp, 0, sizeof (GatherScope));
    MemSet ((Pointer) gsp->ignore, (int) (TRUE),
            (size_t) (OBJ_MAX * sizeof (Boolean)));
    gsp->ignore[OBJ_SEQANNOT] = FALSE;

    GatherSeqEntry (sep, (Pointer) gcdsp, getgene2cdn, (Pointer) gsp);
  }

  gcdsp->ptrNode = Consort (gcdsp->conmat, gcdsp->genecount,
                            gcdsp->genename);

  for (i = 0; i < CONMATSIZE; i++)
    MemFree (gcdsp->genename[i]);
  MemFree (gcdsp->genename);
  for (i = 0; i < CONMATSIZE; i++)
    MemFree (gcdsp->conmat[i]);
  MemFree (gcdsp->conmat);

  SetLevelNode (gcdsp->ptrNode, 0);
  UnSetPivotNodes (gcdsp->ptrNode);
  SetAllPivotNodes (gcdsp->ptrNode);

  return gcdsp->ptrNode;
}

extern Int4Ptr ConformSeqEntry (SeqEntryPtr sep)
{
  BioseqPtr           bsp;
  Int4Ptr             freq;
  Int4                i;

  static GatherScope  gs;
  GatherScopePtr      gsp;

  static GatherCDS    gcds;
  GatherCDSPtr        gcdsp;

  bsp = NULL;
  SeqEntryExplore (sep, &bsp, UrkcnsrtFindNuc);
  if (bsp == NULL)
    return NULL;

  gsp = &gs;
  gcdsp = &gcds;

  MemSet ((Pointer) gsp, 0, sizeof (GatherScope));
  MemSet ((Pointer) gsp->ignore, (int) (TRUE),
          (size_t) (OBJ_MAX * sizeof (Boolean)));
  gsp->ignore[OBJ_SEQANNOT] = FALSE;

  MemSet ((Pointer) gcdsp->globaltable, 0,
          (size_t) (64 * sizeof (Int4)));

  gcdsp->sep       = sep;
  gcdsp->bsp       = bsp;
  gcdsp->ptrNode   = NULL;
  gcdsp->genecount = 0;

  GatherSeqEntry (sep, (Pointer) gcdsp, getgblcdn, (Pointer) gsp);
  freq = (Int4Ptr) MemNew (sizeof (Int4) * 64);
  for (i = 0; i < 64; i++)
  {
    if (gcdsp->globaltable[i] == 0)
      gcdsp->globaltable[i] = 1;
    freq[i] = gcdsp->globaltable[i];
  }
  return freq;
}

/* exploration */

extern Int4Ptr NewCodonTable (void)
{
  return (Int4Ptr) MemNew ((size_t) (64 * sizeof (Int4)));
}

extern Int4Ptr FreeCodonTable (Int4Ptr cutp)
{
  return (Int4Ptr) MemFree (cutp);
}

extern Int4Ptr CodonTableFromSeqLoc (BioseqPtr bsp, SeqLocPtr slp)
{
  Int4Ptr    cutp;
  Int4       start, stop;
  SeqPortPtr spp;
  Uint1      strand;
  Boolean    flagHaveCDS = FALSE;

  if (bsp == NULL || slp == NULL)
    return NULL;

  if (!ISA_na (bsp->mol))
    return NULL;

  if ((cutp = NewCodonTable ()) == NULL)
    return NULL;

  while (slp != NULL)
  {
    start = SeqLocStart (slp);
    stop = SeqLocStop (slp);
    strand = SeqLocStrand (slp);
    if ((spp = SeqPortNew (bsp, start, stop, 0, Seq_code_iupacna)) != NULL)
    {
      Condone (spp, cutp, strand, TRUE);
      SeqPortFree (spp);
      flagHaveCDS = TRUE;
    }
    slp = slp->next;
  }
  if (!flagHaveCDS)
    cutp = FreeCodonTable (cutp);
  return cutp;
}

extern void AddSeqLocToCodonTable (Int4Ptr cutp, BioseqPtr bsp,
                                   SeqLocPtr slp, Boolean flagAdd)
{
  Int4       start, stop;
  SeqPortPtr spp;
  Uint1      strand;

  if (cutp == NULL || bsp == NULL || slp == NULL)
    return;

  if (!ISA_na (bsp->mol))
    return;

  while (slp != NULL)
  {
    start = SeqLocStart (slp);
    stop = SeqLocStop (slp);
    strand = SeqLocStrand (slp);
    if ((spp = SeqPortNew (bsp, start, stop, 0, Seq_code_iupacna)) != NULL)
    {
      Condone (spp, cutp, strand, flagAdd);
      SeqPortFree (spp);
    }
    slp = slp->next;
  }
  return;
}

extern Int4Ptr MergeCodonTables (Int4Ptr cutp1, Int4Ptr cutp2)
{
  Int4Ptr    cutp;
  Int4       i;

  if (cutp1 == NULL || cutp2 == NULL)
    return NULL;

  if ((cutp = NewCodonTable ()) == NULL)
    return NULL;

  for (i = 0; i < 64; i++)
    cutp[i] = cutp1[i] + cutp2[i];

  return cutp;
}

extern FloatHi Confide (Int4Ptr cutgene, Int4Ptr cutgbl)
{
  Int4    i;
  FloatHi st1, st2, fr1, fr2, frq, score;
  Boolean flagCheck;

  if (cutgene == NULL || cutgbl == NULL)
    return 0.0;

  flagCheck = FALSE;
  score = 0.0;
  for (i = 0; i < 64; i++)
  {
    st1 = findtot (cutgene, i);
    st2 = findtot (cutgbl, i);
    if (st1 != 0 || (st1 == 0 && flagCheck))
    {
      if (st1 != 0)
        fr1 = cutgene[i]/st1;
      else
        fr1 = 1;
      if (st2 != 0)
        fr2 = cutgbl[i]/st2;
      else
        fr2 = 1;
      frq = fr1 - fr2;
      score += ((FloatHi) frq * (FloatHi) frq);
    }
  }

  return score;
}

extern void Conform (Int4Ptr freq, FILE *fn)
{
  Int4     i, codons;
  FloatHi  perthou, percent;
  CharPtr  trips[] = {"AAA", "AAG", "AAC", "AAT",
                      "AGA", "AGG", "AGC", "AGT",
                      "ACA", "ACG", "ACC", "ACT",
                      "ATA", "ATG", "ATC", "ATT",
                      "GAA", "GAG", "GAC", "GAT",
                      "GGA", "GGG", "GGC", "GGT",
                      "GCA", "GCG", "GCC", "GCT",
                      "GTA", "GTG", "GTC", "GTT",
                      "CAA", "CAG", "CAC", "CAT",
                      "CGA", "CGG", "CGC", "CGT",
                      "CCA", "CCG", "CCC", "CCT",
                      "CTA", "CTG", "CTC", "CTT",
                      "TAA", "TAG", "TAC", "TAT",
                      "TGA", "TGG", "TGC", "TGT",
                      "TCA", "TCG", "TCC", "TCT",
                      "TTA", "TTG", "TTC", "TTT"};
  CharPtr  amino[] = {"Lys", "Lys", "Asn", "Asn",
                      "Arg", "Arg", "Ser", "Ser",
                      "Thr", "Thr", "Thr", "Thr",
                      "Ile", "Met", "Ile", "Ile",
                      "Glu", "Glu", "Asp", "Asp",
                      "Gly", "Gly", "Gly", "Gly",
                      "Ala", "Ala", "Ala", "Ala",
                      "Val", "Val", "Val", "Val",
                      "Gln", "Gln", "His", "His",
                      "Arg", "Arg", "Arg", "Arg",
                      "Pro", "Pro", "Pro", "Pro",
                      "Leu", "Leu", "Leu", "Leu",
                      "Och", "Amb", "Tyr", "Tyr",
                      "Umb", "Trp", "Cys", "Cys",
                      "Ser", "Ser", "Ser", "Ser",
                      "Leu", "Leu", "Phe", "Phe"};

  codons  = whattotal (freq);
  fprintf (fn, "Total nodenumber amino acids %3d\n", codons);
  fprintf (fn, "AA    Codon  Number Fraction\n");
  for (i = 0; i < 64; i++)
  {
    perthou = whatthou (freq, i, codons);
    percent = perthou / 10.0;
    fprintf (fn, "%3.3s    %3.3s    %5d   %6.2f\n",
             amino[i], trips[i], (int) (freq[i]), percent);
  }
  fprintf (fn, "\n");

  return;
}

extern ValNodePtr ClearNonMetOrfs (ValNodePtr orflist)
{
  ValNodePtr orfhead = NULL;
  ValNodePtr orfold = NULL;
  ValNodePtr orfnext;
  SeqLocPtr  slp, slpn;

  while (orflist != NULL)
  {
    slp = (SeqLocPtr) orflist->data.ptrvalue;
    if (slp->choice == 0)
    {
      if (orfold != NULL)
        orfold ->next = orflist->next;
      orfnext = orflist->next;
      orflist->next = NULL;
      ValNodeFree (orflist);
      orflist = orfnext;
      while (slp != NULL)
      {
        slpn = slp->next;
        SeqLocFree (slp);
        slp = slpn;
      }
      continue;
    }
    if (orfhead == NULL)
      orfhead = orflist;
    orfold = orflist;
    orflist = orflist->next;
  }
  return orfhead;
}

/* Markovianisms */

extern FreqPtr FreqNew (void)
{
  FreqPtr frqp;
  Int4    number;

  if ((frqp = (FreqPtr) MemNew (sizeof (Freq))) == NULL)
    return NULL;

/* no mem alloc checks ! */
  number = (Int4) pow (4, 9);
  frqp->frame0exp = (FloatHiPtr)
                     MemNew ((size_t) (sizeof (FloatHi) * number));
  frqp->frame1exp = (FloatHiPtr)
                     MemNew ((size_t) (sizeof (FloatHi) * number));
  frqp->frame2exp = (FloatHiPtr)
                     MemNew ((size_t) (sizeof (FloatHi) * number));
  frqp->frame0std = (FloatHiPtr)
                     MemNew ((size_t) (sizeof (FloatHi) * number));
  frqp->frame1std = (FloatHiPtr)
                     MemNew ((size_t) (sizeof (FloatHi) * number));
  frqp->frame2std = (FloatHiPtr)
                     MemNew ((size_t) (sizeof (FloatHi) * number));
  frqp->frame0trip = (Int4Ptr)
                      MemNew ((size_t) (sizeof (Int4) * number));
  frqp->frame1trip = (Int4Ptr)
                      MemNew ((size_t) (sizeof (Int4) * number));
  frqp->frame2trip = (Int4Ptr)
                      MemNew ((size_t) (sizeof (Int4) * number));
  number = (Int4) pow (4, 3);
  frqp->frame0cdnobs = (Int4Ptr)
                      MemNew ((size_t) (sizeof (Int4) * number));
  frqp->frame1cdnobs = (Int4Ptr)
                      MemNew ((size_t) (sizeof (Int4) * number));
  frqp->frame2cdnobs = (Int4Ptr)
                      MemNew ((size_t) (sizeof (Int4) * number));
  frqp->frame0cdn = (Int4Ptr)
                     MemNew ((size_t) (sizeof (Int4) * number));
  frqp->frame1cdn = (Int4Ptr)
                     MemNew ((size_t) (sizeof (Int4) * number));
  frqp->frame2cdn = (Int4Ptr)
                     MemNew ((size_t) (sizeof (Int4) * number));
  frqp->globalcdnobs = (Int4Ptr)
                        MemNew ((size_t) (sizeof (Int4) * number));
  number = (Int4) pow (4, 6);
  frqp->frame0leftdi = (Int4Ptr)
                        MemNew ((size_t) (sizeof (Int4) * number));
  frqp->frame1leftdi = (Int4Ptr)
                        MemNew ((size_t) (sizeof (Int4) * number));
  frqp->frame2leftdi = (Int4Ptr)
                        MemNew ((size_t) (sizeof (Int4) * number));
  frqp->frame0rightdi = (Int4Ptr)
                         MemNew ((size_t) (sizeof (Int4) * number));
  frqp->frame1rightdi = (Int4Ptr)
                         MemNew ((size_t) (sizeof (Int4) * number));
  frqp->frame2rightdi = (Int4Ptr)
                         MemNew ((size_t) (sizeof (Int4) * number));
  return frqp;
}

extern FreqPtr FreqFree (FreqPtr frqp)
{
  if (frqp == NULL)
    return frqp;

  frqp->frame0exp = (FloatHiPtr) MemFree (frqp->frame0exp);
  frqp->frame1exp = (FloatHiPtr) MemFree (frqp->frame1exp);
  frqp->frame2exp = (FloatHiPtr) MemFree (frqp->frame2exp);
  frqp->frame0std = (FloatHiPtr) MemFree (frqp->frame0std);
  frqp->frame1std = (FloatHiPtr) MemFree (frqp->frame1std);
  frqp->frame2std = (FloatHiPtr) MemFree (frqp->frame2std);
  frqp->frame0trip = (Int4Ptr) MemFree (frqp->frame0trip);
  frqp->frame1trip = (Int4Ptr) MemFree (frqp->frame1trip);
  frqp->frame2trip = (Int4Ptr) MemFree (frqp->frame2trip);
  frqp->frame0cdnobs = (Int4Ptr) MemFree (frqp->frame0cdnobs);
  frqp->frame1cdnobs = (Int4Ptr) MemFree (frqp->frame1cdnobs);
  frqp->frame2cdnobs = (Int4Ptr) MemFree (frqp->frame2cdnobs);
  frqp->frame0cdn = (Int4Ptr) MemFree (frqp->frame0cdn);
  frqp->frame1cdn = (Int4Ptr) MemFree (frqp->frame1cdn);
  frqp->frame2cdn = (Int4Ptr) MemFree (frqp->frame2cdn);
  frqp->globalcdnobs = (Int4Ptr) MemFree (frqp->globalcdnobs);
  frqp->frame0leftdi = (Int4Ptr) MemFree (frqp->frame0leftdi);
  frqp->frame1leftdi = (Int4Ptr) MemFree (frqp->frame1leftdi);
  frqp->frame2leftdi = (Int4Ptr) MemFree (frqp->frame2leftdi);
  frqp->frame0rightdi = (Int4Ptr) MemFree (frqp->frame0rightdi);
  frqp->frame1rightdi = (Int4Ptr) MemFree (frqp->frame1rightdi);
  frqp->frame2rightdi = (Int4Ptr) MemFree (frqp->frame2rightdi);

  return (FreqPtr) MemFree (frqp);
}

static void ScanFrame (BioseqPtr bsp, SeqLocPtr slp, Int4 frame,
                       Int4Ptr count, Int4 residues)
{
  SeqLocPtr   slpThis;
  SeqPortPtr  spp;
  Int4        i, n, start, stop, length, chunk;
  Uint1Ptr    seq, sq, sqq;
  Int4        number, numb, iframe;

  if (bsp == NULL || slp == NULL || count == NULL)
    return;

  while (slp != NULL)
  {
    if (slp->choice == SEQLOC_MIX)
      slpThis = (SeqLocPtr) slp->data.ptrvalue;
    else
      slpThis = slp;
    seq = (Uint1Ptr) MemNew (sizeof (Uint1));
    while (slpThis != NULL)
    {
      start = SeqLocStart (slpThis);
      stop = SeqLocStop (slpThis);
      length = stop - start + 1;
      sqq = sq = (Uint1Ptr) MemNew ((size_t) (sizeof (Uint1) * (length+1)));
      if ((spp = SeqPortNew (bsp, start, stop, SeqLocStrand (slpThis),
                             Seq_code_iupacna)) != NULL)
      {
        i = start;
        chunk = 4096;
        while (i <= stop)
        {
          SeqPortRead (spp, sq, (Int2) chunk);
          sq += chunk;
          i += chunk;
        }
        SeqPortFree (spp);
      }
      sqq[length] = 0;
      seq = Realloc (seq, ((size_t) (sizeof (Uint1) * (length+1))));
      StrCat ((CharPtr) seq, (CharPtr) sqq);
      MemFree (sqq);
      if (slp->choice == SEQLOC_MIX)
        slpThis = slpThis->next;
      else
        slpThis = NULL;
    }
    numb = (Int4) pow (4, residues-1);
    number = 0;
    for (n = 0; n < residues-1; n++)
    {
      switch ((Char) seq[n])
      {
       case 'A':
        number += 0;
        break;
/* standard code no contribution to start stop */
       default:
       case 'C':
        number += 1;
        break;
       case 'G':
        number += 2;
        break;
       case 'T':
        number += 3;
        break;
      }
      number *= 4;
    }
    iframe = residues;
    iframe %= 3;
    for (i = n; i < length; i++)
    {
      switch ((Char) seq[i])
      {
       case 'A':
        number += 0;
        break;
/* standard code no contribution to start stop */
       default:
       case 'C':
        number += 1;
        break;
       case 'G':
        number += 2;
        break;
       case 'T':
        number += 3;
        break;
      }
      if (iframe == frame)
        count[number]++;
      number %= numb;
      number *= 4;
      iframe++;
      iframe %= 3;
    }
    MemFree (seq);
    slp = slp->next;
  }
  return;
}

static void Scan3Frames (BioseqPtr bsp, SeqLocPtr slp, Int4Ptr count0,
                         Int4Ptr count1, Int4Ptr count2, Int4 residues)
{
  SeqLocPtr   slpThis;
  SeqPortPtr  spp;
  Int4        i, n, start, stop, length, chunk;
  Uint1Ptr    seq, sq, sqq;
  Int4        number, numb, iframe;

  if (bsp == NULL || slp == NULL ||
      count0 == NULL || count1 == NULL || count2 == NULL)
    return;

  while (slp != NULL)
  {
    if (slp->choice == SEQLOC_MIX)
      slpThis = (SeqLocPtr) slp->data.ptrvalue;
    else
      slpThis = slp;
    seq = (Uint1Ptr) MemNew (sizeof (Uint1));
    while (slpThis != NULL)
    {
      start = SeqLocStart (slpThis);
      stop = SeqLocStop (slpThis);
/* try to include start and stop codons in freq count */
      if (start-3 >= 0)
        start -= 3;
      if (stop+3 < bsp->length)
        stop += 3;
      length = stop - start + 1;
      sqq = sq = (Uint1Ptr) MemNew ((size_t) (sizeof (Uint1) * (length+1)));
      if ((spp = SeqPortNew (bsp, start, stop, SeqLocStrand (slpThis),
                             Seq_code_iupacna)) != NULL)
      {
        i = start;
        chunk = 4096;
        while (i <= stop)
        {
          SeqPortRead (spp, sq, (Int2) chunk);
          sq += chunk;
          i += chunk;
        }
        SeqPortFree (spp);
      }
      sqq[length] = 0;
      seq = Realloc (seq, ((size_t) (sizeof (Uint1) * (length+1))));
      StrCat ((CharPtr) seq, (CharPtr) sqq);
      MemFree (sqq);
      if (slp->choice == SEQLOC_MIX)
        slpThis = slpThis->next;
      else
        slpThis = NULL;
    }
    numb = (Int4) pow (4, residues-1);
    number = 0;
    for (n = 0; n < residues-1; n++)
    {
      switch ((Char) seq[n])
      {
       case 'A':
        number += 0;
        break;
/* standard code no contribution to start stop */
       default:
       case 'C':
        number += 1;
        break;
       case 'G':
        number += 2;
        break;
       case 'T':
        number += 3;
        break;
      }
      number *= 4;
    }
    iframe = residues;
    iframe %= 3;
    for (i = n; i < length; i++)
    {
      switch ((Char) seq[i])
      {
       case 'A':
        number += 0;
        break;
/* standard code no contribution to start stop */
       default:
       case 'C':
        number += 1;
        break;
       case 'G':
        number += 2;
        break;
       case 'T':
        number += 3;
        break;
      }
      switch (iframe)
      {
       case 0:
        count0[number]++;
        break;
       case 1:
        count1[number]++;
        break;
       case 2:
        count2[number]++;
        break;
      }
      number %= numb;
      number *= 4;
      iframe++;
      iframe %= 3;
    }
    MemFree (seq);
    slp = slp->next;
  }
  return;
}

extern FreqPtr ConKovCDSNtFreqs (BioseqPtr bsp, SeqLocPtr slp)
{
  FreqPtr frqp;
  Int4    i, number;

  if (bsp == NULL || slp == NULL)
    return NULL;
  if ((frqp = FreqNew ()) == NULL)
    return NULL;

  Scan3Frames (bsp, slp, frqp->frame0trip, frqp->frame1trip,
               frqp->frame2trip, 9);

  number = (Int4) pow (4, 9);
  for (i = 0; i < number; i++)
  {
    if (frqp->frame0trip[i] != 0)
    {
      frqp->frame0leftdi[i/64] += frqp->frame0trip[i];
      frqp->frame0rightdi[i%4096] += frqp->frame0trip[i];
      frqp->frame0cdnobs[(i/64)%64] += frqp->frame0trip[i];
    }
    if (frqp->frame1trip[i] != 0)
    {
      frqp->frame1leftdi[i/64] += frqp->frame1trip[i];
      frqp->frame1rightdi[i%4096] += frqp->frame1trip[i];
      frqp->frame1cdnobs[(i/64)%64] += frqp->frame1trip[i];
    }
    if (frqp->frame2trip[i] != 0)
    {
      frqp->frame2leftdi[i/64] += frqp->frame2trip[i];
      frqp->frame2rightdi[i%4096] += frqp->frame2trip[i];
      frqp->frame2cdnobs[(i/64)%64] += frqp->frame2trip[i];
    }
  }

  number = (Int4) pow (4, 6);
  for (i = 0; i < number; i++)
  {
    if (frqp->frame0leftdi[i] == 0)
      frqp->frame0leftdi[i] = 1;
    if (frqp->frame1leftdi[i] == 0)
      frqp->frame1leftdi[i] = 1;
    if (frqp->frame2leftdi[i] == 0)
      frqp->frame2leftdi[i] = 1;
    if (frqp->frame0rightdi[i] == 0)
      frqp->frame0rightdi[i] = 1;
    if (frqp->frame1rightdi[i] == 0)
      frqp->frame1rightdi[i] = 1;
    if (frqp->frame2rightdi[i] == 0)
      frqp->frame2rightdi[i] = 1;
  }
  number = (Int4) pow (4, 3);
  for (i = 0; i < number; i++)
  {
    frqp->frame0cdn[i] = frqp->frame0cdnobs[i];
    frqp->frame1cdn[i] = frqp->frame1cdnobs[i];
    frqp->frame2cdn[i] = frqp->frame2cdnobs[i];
    if (frqp->frame0cdn[i] == 0)
      frqp->frame0cdn[i] = 1;
    if (frqp->frame1cdn[i] == 0)
      frqp->frame1cdn[i] = 1;
    if (frqp->frame2cdn[i] == 0)
      frqp->frame2cdn[i] = 1;
  }

  return frqp;
}

extern Boolean ConKovCDSGlobalNtFreqs (BioseqPtr bsp, FreqPtr frqp)
{
  SeqLocPtr slp1, slp2;
  Int4      i, number;

  if (bsp == NULL || frqp == NULL)
    return FALSE;

  slp1 = SeqLocIntNew (0, bsp->length-1, Seq_strand_plus, bsp->id);
  slp2 = SeqLocIntNew (0, bsp->length-1, Seq_strand_minus, bsp->id);
  if (slp1 == NULL || slp2 == NULL)
  {
    SeqLocFree (slp1);
    SeqLocFree (slp2);
    return FALSE;
  }

  Scan3Frames (bsp, slp1, frqp->globalcdnobs, frqp->globalcdnobs,
               frqp->globalcdnobs, 3);
  Scan3Frames (bsp, slp2, frqp->globalcdnobs, frqp->globalcdnobs,
               frqp->globalcdnobs, 3);
  SeqLocFree (slp1);
  SeqLocFree (slp2);

  number = (Int4) pow (4, 3);
  for (i = 0; i < number; i++)
  {
    if (frqp->globalcdnobs[i] == 0)
      frqp->globalcdnobs[i] = 1;
  }

  return TRUE;
}

extern FreqPtr ConKovTrainCDS (BioseqPtr bsp, SeqLocPtr slp)
{
  FreqPtr frqp;
  Int4    residues, number, i, y, loop;

  if (bsp == NULL || slp == NULL)
    return NULL;

  if ((frqp = ConKovCDSNtFreqs (bsp, slp)) != NULL)
  {
    residues = 6;
    number = (Int4) pow (4, residues);
    loop = 0;
    for (i = 0; i < number; i++)
    {
      for (y = 0; y < number; y++)
      {
        if (i%64 == y/64)
        {
          frqp->frame0exp[loop] = (((FloatHi) frqp->frame0leftdi[i] *
                                    (FloatHi) frqp->frame0rightdi[y]) /
                                    (FloatHi) frqp->frame0cdn[i%64]);
          frqp->frame1exp[loop] = (((FloatHi) frqp->frame1leftdi[i] *
                                    (FloatHi) frqp->frame1rightdi[y]) /
                                    (FloatHi) frqp->frame1cdn[i%64]);
          frqp->frame2exp[loop] = (((FloatHi) frqp->frame2leftdi[i] *
                                    (FloatHi) frqp->frame2rightdi[y]) /
                                    (FloatHi) frqp->frame2cdn[i%64]);
          loop++;
        }
      }
    }
    residues = 9;
    number = (Int4) pow (4, residues);
    for (i = 0; i < number; i++)
    {
      frqp->frame0std[i] = ((FloatHi) frqp->frame0trip[i] -
                                      frqp->frame0exp[i]) /
                            (FloatHi) sqrt (frqp->frame0exp[i]);
      frqp->frame1std[i] = ((FloatHi) frqp->frame1trip[i] -
                                      frqp->frame1exp[i]) /
                            (FloatHi) sqrt (frqp->frame1exp[i]);
      frqp->frame2std[i] = ((FloatHi) frqp->frame2trip[i] -
                                      frqp->frame2exp[i]) /
                            (FloatHi) sqrt (frqp->frame2exp[i]);
    }
  }
  return frqp;
}
