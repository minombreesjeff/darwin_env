#include <objseq.h>
#include <seqport.h>
#include <gather.h>
#include <picture.h>
#include <viewer.h>
#include <ncbibs.h>

#include <seqanal.h>
#include <seqgraph.h>

extern ButRegPtr ButRegNew (void)
{
  ButRegPtr burp;
  Int4      i;

  if ((burp = (ButRegPtr) MemNew (sizeof (ButReg))) == NULL)
    return burp;

  burp->b = NULL;
  burp->g = NULL;
  burp->c = NULL;

  burp->b = (ButtoN PNTR) MemNew (sizeof (ButtoN) * BCOUNT);
  burp->g = (GrouP PNTR) MemNew (sizeof (GrouP) * BCOUNT);
  burp->c = (Int4 PNTR) MemNew (sizeof (Int4) * BCOUNT);

  if (burp->b == NULL || burp->g == NULL || burp->c == NULL)
  {
    MemFree (burp->b);
    MemFree (burp->b);
    MemFree (burp->b);
    return (ButRegPtr) MemFree (burp);
  }

  i = 0;
  while (i < BCOUNT)
  {
    burp->b[i] = NULL;
    burp->g[i] = NULL;
    burp->c[i] = 0;
    i++;
  }
  return burp;
}

extern ButRegPtr ButRegFree (ButRegPtr burp)
{
  burp->b = (ButtoN PNTR) MemFree (burp->b);
  burp->g = (GrouP PNTR) MemFree (burp->g);
  burp->c = (Int4 PNTR) MemFree (burp->c);
  return (ButRegPtr) MemFree (burp);
}

extern void ButRegAdd (ButRegPtr burp, ButtoN b, GrouP g, Int4 current)
{
  Int4      i;

  if (g == NULL)
    return;

  i = 0;
  while (i < BCOUNT)
  {
    if (burp->b[i] == NULL)
    {
      burp->b[i] = b;
      burp->g[i] = g;
      burp->c[i] = current;
      return;
    }
    i++;
  }
  return;
}

extern void ButRegSelectProc (ButtoN b)
{
  ButRegPtr   burp;
  Int4        i;

  if (b != NULL)
  {
    if ((burp = (ButRegPtr) GetObjectExtra (b)) != NULL)
    {
      i = 0;
      while (i < BCOUNT)
      {
        if (burp->b[i] == b && burp->c[i] == 1)
        {
          return;
        }
        i++;
      }
      i = 0;
      while (i < BCOUNT)
      {
        if (burp->c[i] == 1)
        {
          Disable (burp->g[i]);
          burp->c[i] = 0;
        }
        i++;
      }
      i = 0;
      while (i < BCOUNT)
      {
        if (burp->b[i] == b)
        {
          Enable (burp->g[i]);
          burp->c[i] = 1;
          return;
        }
        i++;
      }
    }
  }
  return;
}

static void AnalyseSeqAnalProc (ButtoN b)
{
  SeqAnalFormPtr   sqap;
  ButRegPtr        burp;
  Int4             i, j;

  if (b != NULL)
  {
    if ((sqap = (SeqAnalFormPtr) GetObjectExtra (b)) != NULL)
    {
      burp = sqap->burp;
      i = 0;
      while (i < BCOUNT)
      {
        if (burp->c[i] == 1)
          break;
        i++;
      }
      if (i == BCOUNT)
        return;
      if ((j = (Int4) GetValue (burp->g[i])) == 0)
        return;
    }
  }
  return;
}

extern SeqAnalFormPtr SeqAnalFormNew (void)
{
  SeqAnalFormPtr  sqap;

  sqap = (SeqAnalFormPtr) MemNew (sizeof (SeqAnalForm));
  if (sqap != NULL)
  {
    sqap->burp = NULL;
  }
  return sqap;
}

extern SeqAnalFormPtr SeqAnalFormFree (SeqAnalFormPtr sqap)
{
  sqap = MemFree (sqap);
  return sqap;
}

static void CleanupSeqAnalForm (GraphiC g, VoidPtr data)
{
  SeqAnalFormPtr sqap;

  if ((sqap = (SeqAnalFormPtr) data) != NULL)
  {
    WatchCursor ();
    ButRegFree (sqap->burp);
    ArrowCursor ();
  }
  StdCleanupFormProc (g, data);
  return;
}

static void CloseSeqAnalProc (ButtoN b)
{
  WindoW           w;
  SeqAnalFormPtr   sqap;

  if (b != NULL)
  {
    if ((sqap = (SeqAnalFormPtr) GetObjectExtra (b)) != NULL)
    {
      w = (WindoW) sqap->form;
      Remove (w);
    }
  }
  return;
}

extern void BioseqPtrToSeqAnalForm (ForM f, Pointer data)
{
  SeqAnalFormPtr   sqap;
  BioseqPtr        bsp;
  GrouP            g,       g1, g2;
  GrouP            h,       h1, h2;
  ButtoN           bc, ba,  b1, b2;
  ButRegPtr        burp;

  WatchCursor ();
  sqap = (SeqAnalFormPtr) GetObjectExtra (f);
  bsp = (BioseqPtr) data;
  if (sqap != NULL && bsp != NULL)
  {
    if ((burp = ButRegNew ()) != NULL)
    {
      sqap->burp = burp;
      g = HiddenGroup ((WindoW) sqap->form, -1, 0, NULL);
/*
      g1 = NormalGroup (g, 2, 0, "Patterns", NULL, NULL);
      b1 = PushButton (g1, "  ", ButRegSelectProc);
      SetObjectExtra (b1, burp, NULL);
      h1 = HiddenGroup (g1, 6, 0, NULL);
      RadioButton (h1, "Canned");
      RadioButton (h1, "Select");
      SetValue (h1, 1);
      Enable (h1);
      ButRegAdd (burp, b1, h1, 1);
*/
      g2 = NormalGroup (g, 2, 0, "Profiles", NULL, NULL);
      b2 = PushButton (g2, "  ", ButRegSelectProc);
      SetObjectExtra (b2, burp, NULL);
      h2 = HiddenGroup (g2, 6, 0, NULL);
      RadioButton (h2, "Canned");
      RadioButton (h2, "Select");
      SetValue (h2, 1);
      Disable (h2);
      ButRegAdd (burp, b2, h2, 0);

      h = HiddenGroup (g, 2, 0, NULL);
      ba = PushButton (h, "Analyse", AnalyseSeqAnalProc);
      SetObjectExtra (ba, sqap, NULL);
      bc = PushButton (h, "Close", CloseSeqAnalProc);
      SetObjectExtra (bc, sqap, NULL);
    }
  }
  ArrowCursor ();
  return;
}

extern ForM CreateSeqAnalForm (Int2 left, Int2 top, CharPtr title,
                                 BioseqPtr bsp)
{
  WindoW           w;
  SeqAnalFormPtr   sqap;

  w = NULL;
  if (bsp == NULL)
    return (ForM) w;
  sqap = SeqAnalFormNew ();
  if (sqap != NULL)
  {
    w = DocumentWindow (left, top, -10, -10, title,
                        NULL, NULL);
    SetObjectExtra (w, sqap, CleanupSeqAnalForm);
    sqap->form = (ForM) w;
    sqap->actproc = NULL;
    sqap->toform = BioseqPtrToSeqAnalForm;
    sqap->bsp = bsp;
    sqap->entityID = 0;
    sqap->itemID = 0;
  }
  return (ForM) w;
}
