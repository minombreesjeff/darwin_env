#include <seqscrl.h>

extern SeqScrollDataPtr SeqScrollDataNew (Int4 scrnum)
{
  SeqScrollDataPtr  ssdp;
  Int4              i;

  if ((ssdp = (SeqScrollDataPtr) MemNew (sizeof (SeqScrollData))) != NULL)
  {
    ssdp->br = NULL;
    ssdp->pc = NULL;
    ssdp->curval = 0;
    ssdp->minval = 0;
    ssdp->maxval = 100;
    ssdp->bsp = NULL;
    ssdp->gvp = NULL;
    ssdp->scr = (FloatHiPtr) MemNew (sizeof (FloatHi) * scrnum);
    ssdp->res = (CharPtr) MemNew (sizeof (Char) * scrnum);
    ssdp->filtertype = 0;
    ssdp->next = NULL;
    for (i = 0; i < scrnum; i++)
    {
      ssdp->scr[i] = 0.0;
      ssdp->res[i] = ' ';
    }
  }
  return ssdp;
}

/* all pointers to structures are handled by others */

extern SeqScrollDataPtr SeqScrollDataFree (SeqScrollDataPtr ssdp)
{
  SeqScrollDataPtr snext;

  while (ssdp != NULL)
  {
    snext = ssdp->next;
    ssdp->next = NULL;
    ssdp->br = NULL;
    ssdp->pc = NULL;
    ssdp->bsp = NULL;
    ssdp->gvp = NULL;
    ssdp->scr = (FloatHiPtr) MemFree (ssdp->scr);
    ssdp->res = (CharPtr) MemFree (ssdp->res);
    (SeqScrollDataPtr) MemFree (ssdp);
    ssdp = snext;
  }
  return ssdp;
}

static void ScrollBarScrollProc (BaR b, GraphiC g, Int2 new, Int2 old)
{
  SeqScrollDataPtr  ssdp;
  Int2              curval;
  Char              buf[8];

  ssdp = (SeqScrollDataPtr) GetObjectExtra (b);
  if (ssdp == NULL || ssdp->pc == NULL)
    return;

  curval = (Int2) (((ssdp->maxval-ssdp->minval) - new) + ssdp->minval);
  sprintf (buf, "%3d", curval);
  SetTitle (ssdp->pc, buf);
  ssdp->curval = curval;
  return;
}

static void ScrollTextScrollProc (TexT t)
{
  SeqScrollDataPtr  ssdp;
  Int2              curval;
  Char              buf[8];

  ssdp = (SeqScrollDataPtr) GetObjectExtra (t);
  if (ssdp == NULL || ssdp->pc == NULL || ssdp->br == NULL)
    return;

  GetTitle (ssdp->pc, buf, sizeof (buf));
  StrToInt (buf, &curval);
  if (curval > ssdp->maxval)
    curval = ssdp->maxval;
  if (curval < ssdp->minval)
    curval = ssdp->minval;
/*
  sprintf (buf, "%3d", curval);
  SetTitle (ssdp->pc, buf);
*/
  CorrectBarValue (ssdp->br, (ssdp->maxval-ssdp->minval)
                                  -(curval-ssdp->minval));
  ssdp->curval = curval;
  return;
}

extern void TextScrollWindowNew (SeqScrollDataPtr ssdp, GrouP g)
{
  Char              buf[8];

  ssdp->pc = DialogText (g, "", 2, ScrollTextScrollProc);
  SetObjectExtra (ssdp->pc, ssdp, NULL);
  ssdp->br = ScrollBar (g, 0, 2, ScrollBarScrollProc);
  SetObjectExtra (ssdp->br, ssdp, NULL);
  CorrectBarMax (ssdp->br, ssdp->maxval-ssdp->minval);
  CorrectBarValue (ssdp->br, (ssdp->maxval-ssdp->minval)
                            -(ssdp->curval-ssdp->minval));
  sprintf (buf, "%3d", ssdp->curval);
  SetTitle (ssdp->pc, buf);
  return;
}

extern void AddScrollControl (SeqScrollDataPtr PNTR ssdp, GrouP g,
                              Int2 cur, Int2 min, Int2 max,
                              Int4 score_number)
{
  SeqScrollDataPtr  ssdpcur, ssdpnext;

  if ((ssdpnext = SeqScrollDataNew (score_number)) == NULL)
    return;
  ssdpnext->curval = cur;
  ssdpnext->minval = min;
  ssdpnext->maxval = max;
  TextScrollWindowNew (ssdpnext, g);

  if (*ssdp == NULL)
  {
    *ssdp = ssdpnext;
  }
  else
  {
    ssdpcur = *ssdp;
    while (ssdpcur->next != NULL)
      ssdpcur = ssdpcur->next;
    ssdpcur->next = ssdpnext;
  }
  return;
}
