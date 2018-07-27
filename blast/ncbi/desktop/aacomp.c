#include <objres.h>
#include <objseq.h>
#include <seqport.h>
#include <aacomp.h>

/* defines */
#define F7   7
#define F24 24
#define F42 42
#define PI  3.14159

/* functions - internal */

static FloatHiPtr AAC (SeqPortPtr spp, Int4 start, Int4 end,
                       Int4 numval, Int4 window,
                       FloatHiPtr scr, CharPtr res)
{
  Int4        i, n, col;
  Int4        stop;
  Char        aa;
  Boolean     flagMatch;
  FloatHi     temp, fmin = 0.0, fmax = 0.0, frange;
  FloatHiPtr  frealheadptr, fheadptr;
  FloatHiPtr  fptr;

  fptr = (FloatHiPtr) MemNew ((sizeof (FloatHi)) * numval);
  frealheadptr = fptr;
  for (i = start; i < end+1; i++)
    *fptr++ = 0.0;
  fptr = frealheadptr;

  for (i = 0; i < window/2; i++)
    fptr++;
  fheadptr = fptr;
  stop = end + 1 - window + 1;
  for (i = start; i < stop; i++)
  {
    temp = 0;
    SeqPortSeek (spp, i, SEEK_SET);
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
        temp += scr[n];
    }
    temp /= window;
    *fptr = temp;
    fptr++;
  }
  fptr = fheadptr;
  for (i = start; i < stop; i++)
  {
    if (*fptr > fmax)
      fmax = *fptr;
    fptr++;
  }
  fptr = fheadptr;
  for (i = start; i < stop; i++)
  {
    if (*fptr < fmin)
      fmin = *fptr;
    fptr++;
  }
  frange = fmax-fmin;
  fptr = fheadptr;
  for (i = start; i < stop; i++)
  {
    *fptr = (*fptr - fmin)/frange*100;
    fptr++;
  }

  return frealheadptr;
}

static SeqGraphPtr AACGraph (SeqPortPtr spp, Int4 start, Int4 end, Int4 window,
                             FloatHiPtr scr, CharPtr res)
{
  SeqGraphPtr  sgp;
  Int4         gwidth = 500;

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
    AAC (spp, start, end, end+1, window, scr, res);
  }
  return sgp;
}

/*
 this stores a score for each of 24 resides
  20 amino acids
   2 ambiguous amino acids (ASX GLX)
   1 unknown amino acid (the average score of the 20 generally)
   1 stop codon (currently also the average score of the 20)
 the scoring table should have the scores in a column
 in order of the residues stored in static Char c[]

 the caller must pass pointers to arrays that can hold
 the 24 residues (res) and scores (scr)

 a successful, tho not necessarily correct, read will return 24
*/

extern Int4 ReadAAC (CharPtr filename, FloatHiPtr scr, CharPtr res)
{
  Int2     val, count;
  Int4     type;
  long int ltype;
  FloatHi  score;
  Char     buf[PATH_MAX], buff[PATH_MAX];
  CharPtr  bptr;
  FILE     *fio;

  static Char c[] = {'A','B','C','D','E','F','G','H','I','K','L','M',
                     'N','P','Q','R','S','T','V','W','X','Y','Z','*'};

  if (!FindPath ("ncbi", "ncbi", "data", buf, sizeof (buf)))
    return 0;
  FileBuildPath (buf, NULL, filename);
  if ((fio = FileOpen (buf, "r")) == NULL)
  {
    if ((fio = FileOpen (filename, "r")) == NULL)
    {
      return 0;
    }
  }

  val = 0;
  type = -1;
  while ((FileGets (buff, sizeof (buff), fio)) != NULL)
  {
    if (buff[0] == ';')
      continue;
    if (type == -1)
    {
      sscanf (buff, "%ld", &ltype);
      type = (Int4) ltype;
      if (type != 1)
      {
        FileClose (fio);
        return 0;
      }
      FileGets (buff, sizeof (buff), fio);
      continue;
    }

    res[val] = c[val];
    bptr = buff;

    sscanf (bptr, "%lf", &score);
    scr[val] = score;

    val++;
    if (val == F24)
      break;
  }
  count = val;

  FileClose (fio);
  return (Int4) count;
}

/* functions - external */

extern FloatHiPtr AAComposition (SeqPortPtr spp,
                                 Int4 start, Int4 end, Int4 numval,
                                 FloatHiPtr scr, CharPtr res, Int4 window)
{
  FloatHiPtr   fptr;

  fptr = AAC (spp, start, end, numval, window, scr, res);
  return fptr;
}
