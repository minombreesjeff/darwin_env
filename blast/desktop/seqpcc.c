#include <objres.h>
#include <objseq.h>
#include <seqport.h>
#include <seqpcc.h>

/* defines */
#define F7   7
#define F24 24
#define F42 42
#define PI  3.14159

/* functions - internal */

static FloatHi PPCC (FloatHi score)
{
  FloatHi   meang = 0.77;
  FloatHi   stdg  = 0.20;
  FloatHi   meanc = 1.63;
  FloatHi   stdc  = 0.24;
  FloatHi   constg, constc;
  FloatHi   evalg, evalc;
  FloatHi   Gg, Gc;
  FloatHi   prob;

  constg = 1 / (stdg * 2 * PI);
  constc = 1 / (stdc * 2 * PI);
  evalg = (((score - meang) / stdg) * ((score - meang) / stdg)) / 2;
  evalc = (((score - meanc) / stdc) * ((score - meanc) / stdc)) / 2;
  evalg = exp (evalg);
  evalc = exp (evalc);
  Gg = constg / evalg;
  Gc = constc / evalc;
  prob = Gc / ((30 * Gg) + Gc);
  return prob;
}

static FloatHi nroot (FloatHi fv, Int4 wv)
{
  FloatHi  fi = 1.0;

  fi = fi / (FloatHi) wv;
  fv = pow (fv, fi);
  return fv;
}

static void PCC (SeqPortPtr spp, Int4 start, Int4 end, Int4 window,
                 FloatHi scr[F24][F42], Char res[F24], FloatHiPtr fptr)
{
  Int4        i, n, col;
  Int4        stop;
  Char        aa;
  Boolean     flagMatch;
  FloatHi     temp;
  FloatHiPtr  fheadptr;

  fheadptr = fptr;
  for (i = start; i < end; i++)
    *fptr++ = 0.0;
  fptr = fheadptr;
  stop = end - window;
  for (i = start; i < stop; i++)
  {
    SeqPortSeek (spp, i, SEEK_SET);
    temp = 1.0;
    for (col = 0; col < window; col++)
    {
      aa = SeqPortGetResidue (spp);
      flagMatch = FALSE;
      n = 0;
      while (res[n] != '\0')
      {
        if (aa == res[n])
        {
          flagMatch = TRUE;
          break;
        }
        n++;
      }
      if (flagMatch)
        temp *= scr[n][col];
    }
    temp = nroot (temp, window);
    temp = PPCC (temp);
    if (temp < 0.0)
      temp = 0.0;
    temp *= 100.0;
    fheadptr = fptr;
    for (n = 0; n < window; n++)
    {
      if (temp > *fptr)
        *fptr = temp;
      fptr++;
    }
    fptr = fheadptr;
    fptr++;
  }
  return;
}

static SeqGraphPtr PCCGraph (SeqPortPtr spp, Int4 start, Int4 end, Int4 window,
                             FloatHi scr[F24][F42], Char res[F24])
{
  SeqGraphPtr  sgp;
  Int4         gwidth = 500;
  FloatHiPtr   fptr;

  if ((sgp = SeqGraphNew ()) != NULL)
  {
/* type and number of values and compression */
    sgp->numval = end + 1;
    sgp->compr = (Int4) (sgp->numval / gwidth);
    if ((sgp->numval%gwidth) != 0)
      sgp->compr += 1;
/* graph type */
    sgp->flags[2] = 1;
    sgp->values = (Pointer) MemNew ((sizeof (FloatHi)) * sgp->numval);
/* min/max */
    sgp->max.realvalue = 100.0;
    sgp->min.realvalue = 0.0;
    sgp->axis.realvalue = 0.0;
/* scaling */
    sgp->flags[1] = 1;
    sgp->a = 2;
    sgp->b = 0;
/* do it */
    fptr = (FloatHiPtr) sgp->values;
    PCC (spp, start, end, window, scr, res, fptr);
  }
  return sgp;
}

static Int4 ReadPCC (CharPtr res, FloatHi scr[F24][F42], Int4 window)
{
  FILE    *fin;
  Int2    i, n, val, count;
  Int2    numcol;
  FloatHi score;
  Char    buf[PATH_MAX], buff[PATH_MAX];
  CharPtr bptr;

  static Char c[] = {'A','B','C','D','E','F','G','H','I','K','L','M',
                     'N','P','Q','R','S','T','V','W','X','Y','Z','*'};

  numcol = F7;

  if (!FindPath ("ncbi", "ncbi", "data", buf, sizeof (buf)))
    return 0;
  FileBuildPath (buf, NULL, "KSpcc.dat");
  if ((fin = FileOpen (buf, "r")) == NULL)
  {
    if ((fin = FileOpen ("KSpcc.dat", "r")) == NULL)
    {
      return 0;
    }
  }

  val = 0;
  while ((FileGets (buff, sizeof (buff), fin)) != NULL)
  {
    if (buff[0] == ';')
      continue;
    res[val] = c[val];
    bptr = buff;
    for (n = 0; n < numcol; n++)
    {
      sscanf (bptr, "%lf", &score);
      scr[val][n] = score;
      bptr += 7;
    }
    val++;
    if (val == F24)
      break;
  }
  count = val;

  for (n = numcol; n < window; n++)
  {
    if (n == F42)
      break;
    val = (Int2) (n % numcol);
    for (i = 0; i < F24; i++)
    {
      scr[i][n] = scr[i][val];
    }
  }

  FileClose (fin);
  return (Int4) count;
}

/* functions - external */

extern SeqGraphPtr PCCProc (BioseqPtr bsp, SeqFeatPtr sfp, Int4 window)
{
  SeqIdPtr     psip;
  BioseqPtr    pbsp;
  Int4         start, end;
  FloatHi      scr[F24][F42];
  Char         res[F24];
  SeqPortPtr   spp;
  SeqGraphPtr  sgp = NULL;

  if (bsp != NULL)
  {
    pbsp = bsp;

    if (!ISA_aa (pbsp->mol))
      return sgp;
  }
  else if (sfp != NULL)
  {
    if (sfp->data.choice != SEQFEAT_CDREGION)
      return sgp;

    psip = SeqLocId (sfp->product);
    pbsp = BioseqFind (psip);
    if (pbsp == NULL)
      return sgp;

    if (!ISA_aa (pbsp->mol))
      return sgp;
  }

  if (ReadPCC (res, scr, window) != F24)
    return sgp;

  start = 0;
  end = pbsp->length-1;
  if ((spp = SeqPortNew (pbsp, start, end, 0, Seq_code_iupacaa)) != NULL)
  {
    end -= window;
    sgp = PCCGraph (spp, start, end, window, scr, res);
    SeqPortFree (spp);
  }

  return sgp;
}
