#include <vibfltr.h>

extern SeqGraphPtr FilterGraph (Int4 start, Int4 end, FloatHiPtr score,
                                FloatHi cutoff)
{
  SeqGraphPtr  sgp;
  Int4         gwidth = 500;
  Int4         i;
  Boolean      flagCC = FALSE;

  if (score == NULL)
    return NULL;

  if ((sgp = SeqGraphNew ()) != NULL)
  {
/* type and number of values and compression */
    sgp->numval = end - start + 1;
    sgp->compr = (Int4) (sgp->numval / gwidth);
    if ((sgp->numval%gwidth) != 0)
      sgp->compr += 1;
/* graph type */
    sgp->flags[2] = 1;
    sgp->values = (Pointer) score;
/* min/max */
    sgp->max.realvalue = 20.0;
    sgp->min.realvalue =   0.0;
    sgp->axis.realvalue = 0.0;
    score = (FloatHiPtr) sgp->values;
    for (i = 0; i < sgp->numval; i++)
    {
/*      *score *= 100.0; */
      if (*score > 100.0)
        *score = 100.0;
      if (*score < 0.0)
        *score = 0.0;
      if (*score > cutoff)
        flagCC = TRUE;
      score++;
    }
    score = (FloatHiPtr) sgp->values;
    for (i = 0; i < sgp->numval; i++)
    {
      *score = (*score / 100.0 * sgp->max.realvalue);
      score++;
    }
/* scaling */
    sgp->flags[1] = 1;
    sgp->a = 2;
    sgp->b = 0;
/* do it */
    if (!flagCC)
      sgp = SeqGraphFree (sgp);
  }
  return sgp;
}
