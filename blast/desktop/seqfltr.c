#include <objres.h>
#include <objseq.h>
#include <seqport.h>
#include <seqfltr.h>
#include <dust.h>
#include <aacomp.h>

/* defines */
#define FILTS            32

/* functions - internal */

static Boolean ReadFilter (CharPtr res, FloatHiPtr scr,
                           CharPtr buf, CharPtr filename)
{
  FILE    *fin;
  Int2    i, val;
  Int4    type;
  long    ltype;
  Char    buff[256];

  static Char c[] =
          {'A','B','C','D','G','H','K','M','N','R','S','T','V','W','Y'};

  for (i = 0; i < FILTS; i++)
    res[i] = '\0';
  for (i = 0; i < FILTS; i++)
    scr[i] = 0.0;

  if ((fin = FileOpen (buf, "r")) == NULL)
  {
    if ((fin = FileOpen (filename, "r")) == NULL)
    {
      return FALSE;
    }
  }

  val = 0;
  type = -1;
  while ((FileGets (buff, sizeof (buff), fin)) != NULL)
  {
    if (buff[0] == ';')
      continue;
    if (type == -1)
    {
      sscanf (buff, "%ld", &ltype);
      type = (Int4) ltype;
      if (type != 1)
      {
        FileClose (fin);
        return FALSE;
      }
      FileGets (buff, sizeof (buff), fin);
      continue;
    }
    res[val] = c[val];
    sscanf (buff, "%ld", &ltype);
    scr[val] = (FloatHi) ltype;
    scr[val] /= 100;
    val++;
  }
  FileClose (fin);
  return TRUE;
}

static FloatHiPtr NTDust (SeqPortPtr spp, Int4 length,
                          Int4 level, Int4 window,
                          Int4 minwin, Int4 linker)
{
  FloatHiPtr fptr;

  fptr = DustGraph (spp, length, (Int2) level, (Int2) window,
                    (Int2) minwin, (Int2) linker);
  return fptr;
}

static FloatHiPtr NTComposition (SeqPortPtr spp, Int4 length,
                                 Int4 window, Uint1 filtertype)
{
  Int4         i, n, iring;
  Boolean      flagMatch;
  FloatHi      NTcount;
  FloatHiPtr   ringptr;
  Char         res[FILTS], chres;
  FloatHi      scr[FILTS];
  Boolean      flagFilter;
  FloatHiPtr   fhead, fptr;
  Char         buf[PATH_MAX], filename[32];

  if (length < window)
    return NULL;

  if (!FindPath ("ncbi", "ncbi", "data", buf, sizeof (buf)))
    return NULL;

  fptr = (FloatHiPtr) MemNew ((sizeof (FloatHi)) * length);
  fhead = fptr;
  for (i = 0; i < length; i++)
    *fptr++ = 0.0;

  fptr = fhead;
  switch (filtertype)
  {
   case NA_FILTER_GC:
    StringCpy (filename, "KSgc.flt");
    FileBuildPath (buf, NULL, "KSgc.flt");
    break;
   case NA_FILTER_AT:
    StringCpy (filename, "KSat.flt");
    FileBuildPath (buf, NULL, "KSat.flt");
    break;
   case NA_FILTER_PUR:
    StringCpy (filename, "KSpur.flt");
    FileBuildPath (buf, NULL, "KSpur.flt");
    break;
   case NA_FILTER_PYR:
    StringCpy (filename, "KSpyr.flt");
    FileBuildPath (buf, NULL, "KSpyr.flt");
    break;
   default:
    MemFree (fptr);
    return NULL;
  }
  flagFilter = ReadFilter (res, scr, buf, filename);
  if (!flagFilter)
  {
    MemFree (fptr);
    return NULL;
  }

/* set up ring buffer */
  ringptr = (FloatHiPtr) MemNew (sizeof (FloatHi) * window);
  iring = 0;
  NTcount = 0;
  for (i = 0; i < window; i++)
  {
    chres = SeqPortGetResidue (spp);
    flagMatch = 0;
    n = 0;
    while (res[n] != '\0')
    {
      if (chres == res[n])
      {
        flagMatch = 1;
        break;
      }
      n++;
    }
    switch (flagMatch)
    {
      case 1:
        NTcount += scr[n];
        ringptr[iring] = scr[n];
        break;
      default:
        ringptr[iring] = 0;
        break;
    }
    iring++;
    if (iring == window)
      iring = 0;
  }

  for (i = 0; i < window/2; i++)
    fptr++;
  *fptr = NTcount/window*100;
  fptr++;

/* calculate average */
  for (i = window; i < length; i++)
  {
    NTcount = NTcount - ringptr[iring];
    chres = SeqPortGetResidue (spp);
    flagMatch = FALSE;
    n = 0;
    while (res[n] != '\0')
    {
      if (chres == res[n])
      {
        flagMatch = TRUE;
        break;
      }
      n++;
    }
    switch (flagMatch)
    {
      case 1:
        NTcount += scr[n];
        ringptr[iring] = scr[n];
        break;
      default:
        ringptr[iring] = 0;
        break;
    }
    iring++;
    if (iring == window)
      iring = 0;
    *fptr = NTcount/window*100;
    fptr++;
  }
  MemFree (ringptr);
  return fhead;
}

/* functions - external */

extern SeqGraphPtr FilterSeq (SeqPortPtr spp, Int4 start, Int4 end,
                              FloatHiPtr scr, CharPtr res, Int4Ptr exval,
                              Uint1 filtertype)
{
  SeqGraphPtr   sgp = NULL;
  Int4          gwidth = 500;
  FloatHiPtr    fptr;

  if (spp != NULL)
  {
    if ((sgp = SeqGraphNew ()) != NULL)
    {
/* type and number of values and compression */
      sgp->numval = end - start + 1;
      sgp->compr = (Int4) (sgp->numval / gwidth);
      if ((sgp->numval%gwidth) != 0)
        sgp->compr += 1;
/* graph type */
      sgp->flags[2] = 1;
/* scaling */
      sgp->flags[1] = 1;
      sgp->a = 2;
      sgp->b = 0;
/* do it */
      switch (filtertype)
      {
       case NA_FILTER_GC:
       case NA_FILTER_AT:
       case NA_FILTER_PUR:
       case NA_FILTER_PYR:
        fptr = NTComposition (spp, sgp->numval, exval[0], filtertype);
        break;
       case NA_FILTER_DUST:
        fptr = NTDust (spp, sgp->numval,
                       exval[0], exval[1], exval[2], exval[3]);
        break;
       case AA_FILTER_COMP:
       case AA_FILTER_COMP_HOPP:
       case AA_FILTER_COMP_KYTE:
        fptr = AAComposition (spp, start, end, sgp->numval,
                              scr, res, exval[0]);
        break;
       default:
        sgp = SeqGraphFree (sgp);
        break;
      }
      if (fptr == NULL)
      {
        sgp = SeqGraphFree (sgp);
      }
      else
      {
        sgp->values = (Pointer) fptr;
/* min/max */
        sgp->max.realvalue = 100.0;
        sgp->min.realvalue = 0.0;
        sgp->axis.realvalue = 0.0;
      }
    }
  }
  return sgp;
}
