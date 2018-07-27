#include <ncbi.h>
#include <ncbimath.h>
#include <objcode.h>
#include <objseq.h>
#include <sequtil.h>
#include <blastkar.h>
#include <blastpri.h>

/*
	impalaKarlinLambdaBis

	Calculate Lambda using the bisection method (slow).
*/
static Nlm_FloatHi
impalaKarlinLambdaBis(BLAST_ScoreFreqPtr sfp, Nlm_FloatHi initialLambda)
{
	Nlm_FloatHi	PNTR sprob;
	Nlm_FloatHi	lambda, up, newval;
	BLAST_Score	i, low, high;
	int		j;
	Nlm_FloatHi	sum, x0, x1;
        Nlm_FloatHi     returnValue;

	if (sfp->score_avg >= 0.) {
		return -1.;
	}
	low = sfp->obs_min;
	high = sfp->obs_max;
	/* if (BlastScoreChk(low, high) != 0)
		return -1.; */

	sprob = sfp->sprob;
	up = initialLambda;
	for (lambda=0.; ; ) {
		up *= 2;
		x0 = exp((Nlm_FloatHi)up);
		x1 = Nlm_Powi((Nlm_FloatHi)x0, low - 1);
		if (x1 > 0.) {
			for (sum=0., i=low; i<=high; ++i)
				sum += sprob[i] * (x1 *= x0);
		}
		else {
			for (sum=0., i=low; i<=high; ++i)
				sum += sprob[i] * exp(up * i);
		}
		if (sum >= 1.0)
			break;
		lambda = up;
	}

	for (j=0; j<BLAST_KARLIN_LAMBDA_ITER_DEFAULT; ++j) {
		newval = (lambda + up) / 2.;
		x0 = exp((Nlm_FloatHi)newval);
		x1 = Nlm_Powi((Nlm_FloatHi)x0, low - 1);
		if (x1 > 0.) {
			for (sum=0., i=low; i<=high; ++i)
				sum += sprob[i] * (x1 *= x0);
		}
		else {
			for (sum=0., i=low; i<=high; ++i)
				sum += sprob[i] * exp(newval * i);
		}
		if (sum > 1.0)
			up = newval;
		else
			lambda = newval;
	}
        returnValue = (lambda + up) /2;
	return (returnValue);
}

/******************* Fast Lambda Calculation Subroutine ************************
	Version 1.0	May 16, 1991
	Program by:	Stephen Altschul

	Uses Newton-Raphson method (fast) to solve for Lambda, given an initial
	guess (lambda0) obtained perhaps by the bisection method.
*******************************************************************************/

Nlm_FloatHi LIBCALL
impalaKarlinLambdaNR(BLAST_ScoreFreqPtr sfp, Nlm_FloatHi initialLambda)
{
	BLAST_Score	low;			/* Lowest score (must be negative)  */
	BLAST_Score	high;			/* Highest score (must be positive) */
	int		j;
	BLAST_Score	i;
	Nlm_FloatHi PNTR	sprob;
	Nlm_FloatHi	lambda0, sum, slope, temp, x0, x1, amt;
        Nlm_FloatHi returnValue;
	Boolean foundPositive;

	low = sfp->obs_min;
	high = sfp->obs_max;
	if (sfp->score_avg >= 0.) {	/* Expected score must be negative */
		return -1.0;
	}
	/* if (BlastScoreChk(low, high) != 0)
		return -1.; */

	lambda0 = initialLambda;

	/* Calculate lambda */

	sprob = sfp->sprob;
	foundPositive = FALSE;
	for(j = 1; j <=high; j++)
	  if (sprob[j] > 0.0) {
            foundPositive = TRUE;
            break;
	  }
        if (!foundPositive)
	  return(-1);
	for (j=0; j<40; ++j) { /* limit of 20 should never be close-approached */
		sum = -1.0;
		slope = 0.0;
		if (lambda0 < 0.01)
			break;
		x0 = exp((Nlm_FloatHi)lambda0);
		x1 = Nlm_Powi((Nlm_FloatHi)x0, low - 1);
		if (x1 == 0.)
			break;
		for (i=low; i<=high; ++i) {
			sum += (temp = sprob[i] * (x1 *= x0));
			slope += temp * i;
		}
		lambda0 -= (amt = sum/slope);
		if (ABS(amt/lambda0) < BLAST_KARLIN_LAMBDA_ACCURACY_DEFAULT) {
			/*
			Does it appear that we may be on the verge of converging
			to the ever-present, zero-valued solution?
			*/
			if (lambda0 > BLAST_KARLIN_LAMBDA_ACCURACY_DEFAULT)
				return lambda0;
			break;
		}
	}
	returnValue= impalaKarlinLambdaBis(sfp, initialLambda);
        return(returnValue);
}

