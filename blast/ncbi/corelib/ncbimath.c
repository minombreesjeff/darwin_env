/*   ncbimath.c
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
* File Name:  ncbimath.c
*
* Author:  Gish, Kans, Ostell, Schuler
*
* Version Creation Date:   10/23/91
*
* $Revision: 6.3 $
*
* File Description:
*   	portable math functions
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 04-15-93 Schuler     Changed _cdecl to LIBCALL
* 12-22-93 Schuler     Converted ERRPOST((...)) to ErrPostEx(...)
*
* $Log: ncbimath.c,v $
* Revision 6.3  1999/11/24 17:29:16  sicotte
* Added LnFactorial function
*
* Revision 6.2  1997/11/26 21:26:18  vakatov
* Fixed errors and warnings issued by C and C++ (GNU and Sun) compilers
*
* Revision 6.1  1997/10/31 16:22:49  madden
* Limited the loop in Nlm_Log1p to 500 iterations
*
* Revision 6.0  1997/08/25 18:16:35  madden
* Revision changed to 6.0
*
* Revision 5.4  1997/01/31 22:21:40  kans
* had to remove <fp.h> and define HUGE_VAL inline, because of a conflict
* with <math.h> in 68K CodeWarrior 11
*
 * Revision 5.3  1997/01/28  22:57:57  kans
 * include <fp.h> for CodeWarrior to get HUGE_VAL
 *
 * Revision 5.2  1996/12/03  21:48:33  vakatov
 * Adopted for 32-bit MS-Windows DLLs
 *
 * Revision 5.1  1996/06/20  14:08:00  madden
 * Changed int to Int4, double to Nlm_FloatHi
 *
 * Revision 5.0  1996/05/28  13:18:57  ostell
 * Set to revision 5.0
 *
 * Revision 4.1  1996/03/06  19:47:15  epstein
 * fix problem observed by Epstein & fixed by Spouge in log calculation
 *
 * Revision 4.0  1995/07/26  13:46:50  ostell
 * force revision to 4.0
 *
 * Revision 2.11  1995/05/15  18:45:58  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

#define THIS_MODULE g_corelib
#define THIS_FILE _this_file

#include <ncbimath.h>

#ifdef OS_MAC
#ifdef COMP_METRO
/*#include <fp.h>*/
#ifndef HUGE_VAL
#define HUGE_VAL __inf()
double_t __inf ( void );
#endif
#endif
#endif

extern char * g_corelib;
static char * _this_file = __FILE__;

/*
    Nlm_Expm1(x)
    Return values accurate to approx. 16 digits for the quantity exp(x)-1
    for all x.
*/
NLM_EXTERN Nlm_FloatHi LIBCALL Nlm_Expm1(register Nlm_FloatHi	x)
{
  register Nlm_FloatHi	absx;

  if ((absx = ABS(x)) > .33)
    return exp(x) - 1.;

  if (absx < 1.e-16)
    return x;

  return x * (1. + x *
              (0.5 + x * (1./6. + x *
                          (1./24. + x * (1./120. + x *
                                         (1./720. + x * (1./5040. + x *
                                                         (1./40320. + x * (1./362880. + x *
                                                                           (1./3628800. + x * (1./39916800. + x *
                                                                                               (1./479001600. + x/6227020800.)
                                                                                               ))
                                                                           ))
                                                         ))
                                         ))
                          )));
}

/*
    Nlm_Log1p(x)
    Return accurate values for the quantity log(x+1) for all x > -1.
*/
NLM_EXTERN Nlm_FloatHi LIBCALL Nlm_Log1p(register Nlm_FloatHi x)
{
	register Int4	i;
	register Nlm_FloatHi	sum, y;

	if (ABS(x) >= 0.2)
		return log(x+1.);

	/* Limit the loop to 500 terms. */
	for (i=0, sum=0., y=x; i<500 ; ) {
		sum += y/++i;
		if (ABS(y) < DBL_EPSILON)
			break;
		y *= x;
		sum -= y/++i;
		if (y < DBL_EPSILON)
			break;
		y *= x;
	}
	return sum;
}


/*
** Special thanks to Dr. John ``Gammas Galore'' Spouge for deriving the
** method for calculating the gamma coefficients and their use.
** (See the #ifdef-ed program included at the end of this file).
**/

/*
    For discussion of the Gamma function, see "Numerical Recipes in C",
    Press et al. (1988) pages 167-169.
*/

static Nlm_FloatHi NEAR general_lngamma PROTO((Nlm_FloatHi x,Int4 n));

static Nlm_FloatHi _default_gamma_coef [] = {
	 4.694580336184385e+04,
	-1.560605207784446e+05,
	 2.065049568014106e+05,
	-1.388934775095388e+05,
	 5.031796415085709e+04,
	-9.601592329182778e+03,
	 8.785855930895250e+02,
	-3.155153906098611e+01,
	 2.908143421162229e-01,
	-2.319827630494973e-04,
	 1.251639670050933e-10
	 };
static Nlm_FloatHi	PNTR gamma_coef = _default_gamma_coef;
static unsigned	gamma_dim = DIM(_default_gamma_coef);
static Nlm_FloatHi	xgamma_dim = DIM(_default_gamma_coef);

NLM_EXTERN void LIBCALL Nlm_GammaCoeffSet(Nlm_FloatHi PNTR cof, unsigned dim) /* changes gamma coeffs */
{
	if (dim < 3 || dim > 100) /* sanity check */
		return;
	gamma_coef = cof;
	xgamma_dim = gamma_dim = dim;
}


static Nlm_FloatHi NEAR
general_lngamma(Nlm_FloatHi x, Int4 order)	/* nth derivative of ln[gamma(x)] */
	/* x is 10-digit accuracy achieved only for 1 <= x */
	/* order is order of the derivative, 0..POLYGAMMA_ORDER_MAX */
{
	Int4		i;
	Nlm_FloatHi	xx, tx;
	Nlm_FloatHi	y[POLYGAMMA_ORDER_MAX+1];
	register Nlm_FloatHi	tmp, value, PNTR coef;

	xx = x - 1.;  /* normalize from gamma(x + 1) to xx! */
	tx = xx + xgamma_dim;
	for (i = 0; i <= order; ++i) {
		tmp = tx;
		/* sum the least significant terms first */
		coef = &gamma_coef[gamma_dim];
		if (i == 0) {
			value = *--coef / tmp;
			while (coef > gamma_coef)
				value += *--coef / --tmp;
		}
		else {
			value = *--coef / Nlm_Powi(tmp, i + 1);
			while (coef > gamma_coef)
				value += *--coef / Nlm_Powi(--tmp, i + 1);
			tmp = Nlm_Factorial(i);
			value *= (i%2 == 0 ? tmp : -tmp);
		}
		y[i] = value;
	}
	++y[0];
	value = Nlm_LogDerivative(order, y);
	tmp = tx + 0.5;
	switch (order) {
	case 0:
		value += ((NCBIMATH_LNPI+NCBIMATH_LN2) / 2.)
					+ (xx + 0.5) * log(tmp) - tmp;
		break;
	case 1:
		value += log(tmp) - xgamma_dim / tmp;
		break;
	case 2:
		value += (tmp + xgamma_dim) / (tmp * tmp);
		break;
	case 3:
		value -= (1. + 2.*xgamma_dim / tmp) / (tmp * tmp);
		break;
	case 4:
		value += 2. * (1. + 3.*xgamma_dim / tmp) / (tmp * tmp * tmp);
		break;
	default:
		tmp = Nlm_Factorial(order - 2) * Nlm_Powi(tmp, 1 - order)
				* (1. + (order - 1) * xgamma_dim / tmp);
		if (order % 2 == 0)
			value += tmp;
		else
			value -= tmp;
		break;
	}
	return value;
}


NLM_EXTERN Nlm_FloatHi LIBCALL Nlm_PolyGamma(Nlm_FloatHi x, Int4 order) /* ln(ABS[gamma(x)]) - 10 digits of accuracy */
	/* x is and derivatives */
	/* order is order of the derivative */
/* order = 0, 1, 2, ...  ln(gamma), digamma, trigamma, ... */
/* CAUTION:  the value of order is one less than the suggested "di" and
"tri" prefixes of digamma, trigamma, etc.  In other words, the value
of order is truly the order of the derivative.  */
{
	Int4		k;
	register Nlm_FloatHi	value, tmp;
	Nlm_FloatHi	y[POLYGAMMA_ORDER_MAX+1], sx;

	if (order < 0 || order > POLYGAMMA_ORDER_MAX) {
		ErrPostEx(SEV_WARNING,E_Math,ERR_NCBIMATH_DOMAIN,"PolyGamma: unsupported derivative order");
		/**ERRPOST((CTX_NCBIMATH, ERR_NCBIMATH_DOMAIN, "unsupported derivative order"));**/
		return HUGE_VAL;
	}

	if (x >= 1.)
		return general_lngamma(x, order);

	if (x < 0.) {
		value = general_lngamma(1. - x, order);
		value = ((order - 1) % 2 == 0 ? value : -value);
		if (order == 0) {
			sx = sin(NCBIMATH_PI * x);
			sx = ABS(sx);
			if ( (x < -0.1 && (ceil(x) == x || sx < 2.*DBL_EPSILON))
					|| sx == 0.) {
				ErrPostEx(SEV_WARNING,E_Math,ERR_NCBIMATH_DOMAIN,"PolyGamma: log(0)");
				/**ERRPOST((CTX_NCBIMATH, ERR_NCBIMATH_DOMAIN, "log(0)"));**/
				return HUGE_VAL;
			}
			value += NCBIMATH_LNPI - log(sx);
		}
		else {
			y[0] = sin(x *= NCBIMATH_PI);
			tmp = 1.;
			for (k = 1; k <= order; k++) {
				tmp *= NCBIMATH_PI;
				y[k] = tmp * sin(x += (NCBIMATH_PI/2.));
			}
			value -= Nlm_LogDerivative(order, y);
		}
	}
	else {
		value = general_lngamma(1. + x, order);
		if (order == 0) {
			if (x == 0.) {
				ErrPostEx(SEV_WARNING,E_Math,ERR_NCBIMATH_DOMAIN,"PolyGamma: log(0)");
				/**ERRPOST((CTX_NCBIMATH, ERR_NCBIMATH_DOMAIN, "log(0)"));**/
				return HUGE_VAL;
			}
			value -= log(x);
		}
		else {
			tmp = Nlm_Factorial(order - 1) * Nlm_Powi(x,  -order);
			value += (order % 2 == 0 ? tmp : - tmp);
		}
	}
	return value;
}

NLM_EXTERN Nlm_FloatHi LIBCALL Nlm_LogDerivative(Int4 order, Nlm_FloatHi PNTR u) /* nth derivative of ln(u) */
	/* order is order of the derivative */
	/* u is values of u, u', u", etc. */
{
  Int4		i;
  Nlm_FloatHi	y[LOGDERIV_ORDER_MAX+1];
  register Nlm_FloatHi	value, tmp;

  if (order < 0 || order > LOGDERIV_ORDER_MAX) {
InvalidOrder:
    ErrPostEx(SEV_WARNING,E_Math,ERR_NCBIMATH_DOMAIN,"LogDerivative: unsupported derivative order");
    /**ERRPOST((CTX_NCBIMATH, ERR_NCBIMATH_DOMAIN, "unsupported derivative order"));**/
    return HUGE_VAL;
  }

  if (order > 0 && u[0] == 0.) {
    ErrPostEx(SEV_WARNING,E_Math,ERR_NCBIMATH_DOMAIN,"LogDerivative: divide by 0");
    /**ERRPOST((CTX_NCBIMATH, ERR_NCBIMATH_DOMAIN, "divide by 0"));**/
    return HUGE_VAL;
  }
  for (i = 1; i <= order; i++)
    y[i] = u[i] / u[0];

  switch (order) {
  case 0:
    if (u[0] > 0.)
      value = log(u[0]);
    else {
      ErrPostEx(SEV_WARNING,E_Math,ERR_NCBIMATH_DOMAIN,"LogDerivative: log(x<=0)");
      /**ERRPOST((CTX_NCBIMATH, ERR_NCBIMATH_DOMAIN, "log(x<=0)"));**/
      return HUGE_VAL;
    }
    break;
  case 1:
    value = y[1];
    break;
  case 2:
    value = y[2] - y[1] * y[1];
    break;
  case 3:
    value = y[3] - 3. * y[2] * y[1] + 2. * y[1] * y[1] * y[1];
    break;
  case 4:
    value = y[4] - 4. * y[3] * y[1] - 3. * y[2] * y[2]
      + 12. * y[2] * (tmp = y[1] * y[1]);
    value -= 6. * tmp * tmp;
    break;
  default:
    goto InvalidOrder;
  }
  return value;
}


NLM_EXTERN Nlm_FloatHi LIBCALL Nlm_Gamma(Nlm_FloatHi x)		/* ABS[gamma(x)] - 10 digits of accuracy */
{
	return exp(Nlm_PolyGamma(x, 0));
}

NLM_EXTERN Nlm_FloatHi LIBCALL Nlm_LnGamma(Nlm_FloatHi x)		/* ln(ABS[gamma(x)]) - 10 digits of accuracy */
{
	return Nlm_PolyGamma(x, 0);
}

NLM_EXTERN Nlm_FloatHi LIBCALL Nlm_DiGamma(Nlm_FloatHi x)	/* digamma, 1st order derivative of log(gamma) */
{
	return Nlm_PolyGamma(x, 1);
}

NLM_EXTERN Nlm_FloatHi LIBCALL Nlm_TriGamma(Nlm_FloatHi x)	/* trigamma, 2nd order derivative of log(gamma) */
{
	return Nlm_PolyGamma(x, 2);
}


#ifdef foo
/* A program to calculate the gamma coefficients based on a method
   by John Spouge.
   Cut this program out, save it in a separate file, and compile.
   Be sure to link with a math library.
*/
/*
	a[n] = ((gamma+0.5-n)^(n-0.5)) * exp(gamma+0.5-n) *
		((-1)^(n-1) / (n-1)!) * (1/sqrt(2*Pi))
*/
#include <stdio.h>
#include <math.h>

main(ac, av)
	int ac;
	char **av;

{
	int	i, j, cnt;
	double	a, x, y, z, ifact = 1.;

	if (ac != 2 || sscanf(av[1], "%d", &cnt) != 1)
		exit(1);

	for (i=0; i<cnt; ++i) {
		x = cnt - (i + 0.5);
		y = log(x) * (i + 0.5) + x;
		y = exp(y);
		if (i > 1)
			ifact *= i;
		y /= ifact;
		if (i%2 == 1)
			y = -y;
		printf("\t\t\t%.17lg", y);
		if (i < cnt-1)
			putchar(',');
		putchar('\n');
	}
}
#endif /* foo */


#define FACTORIAL_PRECOMPUTED	36

NLM_EXTERN Nlm_FloatHi LIBCALL Nlm_Factorial(Int4 n)
{
	static Nlm_FloatHi	precomputed[FACTORIAL_PRECOMPUTED]
		= { 1., 1., 2., 6., 24., 120., 720., 5040., 40320., 362880., 3628800.};
	static Int4	nlim = 10;
	register Int4	m;
	register Nlm_FloatHi	x;

	if (n >= 0) {
		if (n <= nlim)
			return precomputed[n];
		if (n < DIM(precomputed)) {
			for (x = precomputed[m = nlim]; m < n; ) {
				++m;
				precomputed[m] = (x *= m);
			}
			nlim = m;
			return x;
		}
		return exp(Nlm_LnGamma((Nlm_FloatHi)(n+1)));
	}
	return 0.0; /* Undefined! */
}

/* Nlm_LnGammaInt(n) -- return log(Gamma(n)) for integral n */
NLM_EXTERN Nlm_FloatHi LIBCALL Nlm_LnGammaInt(Int4 n)
{
	static Nlm_FloatHi	precomputed[FACTORIAL_PRECOMPUTED];
	static Int4	nlim = 1; /* first two entries are 0 */
	register Int4 m;

	if (n >= 0) {
		if (n <= nlim)
			return precomputed[n];
		if (n < DIM(precomputed)) {
			for (m = nlim; m < n; ++m) {
				precomputed[m+1] = log(Nlm_Factorial(m));
			}
			return precomputed[nlim = m];
		}
	}
	return Nlm_LnGamma((Nlm_FloatHi)n);
}



/*
	Combined Newton-Raphson and Bisection root-finder

	Original Function Name:  Inv_Xnrbis()
	Author:  Dr. John Spouge
	Location:  NCBI
	Received:  July 16, 1991
*/
#define F(x)  ((*f)(x)-y)
#define DF(x) ((*df)(x))
#define NRBIS_ITMAX 100

NLM_EXTERN Nlm_FloatHi LIBCALL Nlm_NRBis(Nlm_FloatHi y, Nlm_FloatHi (LIBCALL *f )PROTO ((Nlm_FloatHi )), Nlm_FloatHi (LIBCALL *df )PROTO ((Nlm_FloatHi )), Nlm_FloatHi p, Nlm_FloatHi x, Nlm_FloatHi q, Nlm_FloatHi tol)		/* tolerance */

{
	Nlm_FloatHi	temp;	/* for swapping end-points if necessary */
	Nlm_FloatHi	dx;		/* present interval length */
	Nlm_FloatHi	dxold;	/* old interval length */
	Nlm_FloatHi	fx;		/* f(x)-y */
	Nlm_FloatHi	dfx;	/* Df(x) */
	Int4	j;                       /* loop index */
	Nlm_FloatHi	fp, fq;

/* Preliminary checks for improper bracketing and end-point root. */
	if ((fp = F(p)) == 0.)
		return p;
	if ((fq = F(q)) == 0.)
		return q;
	if ((fp > 0. && fq > 0.) || (fp < 0. && fq < 0.)) {
		ErrPostEx(SEV_WARNING,E_Math,ERR_NCBIMATH_INVAL,"NRBis: root not bracketed");
		/**ERRPOST((CTX_NCBIMATH, ERR_NCBIMATH_INVAL, "root not bracketed"));**/
		return HUGE_VAL;
	}
/* Swaps end-points if necessary to make F(p)<0.<F(q) */
	if (fp > 0.) {
		temp = p;
		p = q;
		q = temp;
	}
/* Set up the Bisection & Newton-Raphson iteration. */
	if ((x-p) * (x-q) > 0.)
		x = 0.5*(p+q);
	dxold = dx = p-q;
	for (j = 1; j <= NRBIS_ITMAX; ++j) {
		fx = F(x);
		if (fx == 0.) /* Check for termination. */
			return x;
		if (fx < 0.)
			p = x;
		else
			q = x;
		dfx = DF(x);
/* Check: root out of bounds or bisection faster than Newton-Raphson? */
		if ((dfx*(x-p)-fx)*(dfx*(x-q)-fx) >= 0. || 2.*ABS(fx) > ABS(dfx*dx)) {
			dx = dxold;               /* Bisect */
			dxold = 0.5*(p-q);
			x = 0.5*(p+q);
			if (ABS(dxold) <= tol)
				return x;
		} else {
			dx = dxold;               /* Newton-Raphson */
			dxold = fx/dfx;
			x -= dxold;
			if (ABS(dxold) < tol && F(x-SIGN(dxold)*tol)*fx < 0.)
				return x;
		}
	}
	
	ErrPostEx(SEV_WARNING,E_Math,ERR_NCBIMATH_ITER,"NRBis: iterations > NRBIS_ITMAX");
	/**ERRPOST((CTX_NCBIMATH, ERR_NCBIMATH_ITER, "iterations > NRBIS_ITMAX"));**/
	return HUGE_VAL;
}
#undef F /* clean-up */
#undef DF /* clean-up */



/*
	Romberg numerical integrator

	Author:  Dr. John Spouge, NCBI
	Received:  July 17, 1992
	Reference:
		Francis Scheid (1968)
		Schaum's Outline Series
		Numerical Analysis, p. 115
		McGraw-Hill Book Company, New York
*/
#define F(x)  ((*f)((x), fargs))
#define ROMBERG_ITMAX 20

NLM_EXTERN Nlm_FloatHi LIBCALL Nlm_RombergIntegrate(Nlm_FloatHi (LIBCALL *f) (Nlm_FloatHi,Nlm_VoidPtr), Nlm_VoidPtr fargs, Nlm_FloatHi p, Nlm_FloatHi q, Nlm_FloatHi eps, Int4 epsit, Int4 itmin)

{
	Nlm_FloatHi	romb[ROMBERG_ITMAX];	/* present list of Romberg values */
	Nlm_FloatHi	h;	/* mesh-size */
	Int4		i, j, k, npts;
	long	n;	/* 4^(error order in romb[i]) */
	Int4		epsit_cnt = 0, epsck;
	register Nlm_FloatHi	y;
	register Nlm_FloatHi	x;
	register Nlm_FloatHi	sum;

	/* itmin = min. no. of iterations to perform */
	itmin = MAX(1, itmin);
	itmin = MIN(itmin, ROMBERG_ITMAX-1);

	/* epsit = min. no. of consecutive iterations that must satisfy epsilon */
	epsit = MAX(epsit, 1); /* default = 1 */
	epsit = MIN(epsit, 3); /* if > 3, the problem needs more prior analysis */

	epsck = itmin - epsit;

	npts = 1;
	h = q - p;
	x = F(p);
	if (ABS(x) == HUGE_VAL)
		return x;
	y = F(q);
	if (ABS(y) == HUGE_VAL)
		return y;
	romb[0] = 0.5 * h * (x + y);	/* trapezoidal rule */
	for (i = 1; i < ROMBERG_ITMAX; ++i, npts *= 2, h *= 0.5) {
		sum = 0.;	/* sum of ordinates for */
		/* x = p+0.5*h, p+1.5*h, ..., q-0.5*h */
		for (k = 0, x = p+0.5*h; k < npts; k++, x += h) {
			y = F(x);
			if (ABS(y) == HUGE_VAL)
				return y;
			sum += y;
		}
		romb[i] = 0.5 * (romb[i-1] + h*sum); /* new trapezoidal estimate */

		/* Update Romberg array with new column */
		for (n = 4, j = i-1; j >= 0; n *= 4, --j)
			romb[j] = (n*romb[j+1] - romb[j]) / (n-1);

		if (i > epsck) {
			if (ABS(romb[1] - romb[0]) > eps * ABS(romb[0])) {
				epsit_cnt = 0;
				continue;
			}
			++epsit_cnt;
			if (i >= itmin && epsit_cnt >= epsit)
				return romb[0];
		}
	}
	ErrPostEx(SEV_WARNING,E_Math,ERR_NCBIMATH_ITER,"RombergIntegrate: iterations > ROMBERG_ITMAX");
	/**ERRPOST((CTX_NCBIMATH, ERR_NCBIMATH_ITER, "iterations > ROMBERG_ITMAX"));**/
	return HUGE_VAL;
}

/*
	Nlm_Gcd(a, b)

	Return the greatest common divisor of a and b.

	Adapted 8-15-90 by WRG from code by S. Altschul.
*/
NLM_EXTERN long LIBCALL Nlm_Gcd(register long a, register long b)
{
	register long	c;

	b = ABS(b);
	if (b > a)
		c=a, a=b, b=c;

	while (b != 0) {
		c = a%b;
		a = b;
		b = c;
	}
	return a;
}

/* Round a floating point number to the nearest integer */
NLM_EXTERN long LIBCALL Nlm_Nint(register Nlm_FloatHi x)	/* argument */
{
	x += (x >= 0. ? 0.5 : -0.5);
	return (long)x;
}

/*
integer power function

Original submission by John Spouge, 6/25/90
Added to shared library by WRG
*/
NLM_EXTERN Nlm_FloatHi LIBCALL Nlm_Powi(Nlm_FloatHi x, Int4 n)	/* power */
{
	Nlm_FloatHi	y;

	if (n == 0)
		return 1.;

	if (x == 0.) {
		if (n < 0) {
			ErrPostEx(SEV_WARNING,E_Math,ERR_NCBIMATH_DOMAIN,"Powi: divide by 0");
			/**ERRPOST((CTX_NCBIMATH, ERR_NCBIMATH_DOMAIN, "divide by 0"));**/
			return HUGE_VAL;
		}
		return 0.;
	}

	if (n < 0) {
		x = 1./x;
		n = -n;
	}

	while (n > 1) {
		if (n&1) {
			y = x;
			goto Loop2;
		}
		n /= 2;
		x *= x;
	}
	return x;

Loop2:
	n /= 2;
	x *= x;
	while (n > 1) {
		if (n&1)
			y *= x;
		n /= 2;
		x *= x;
	}
	return y * x;
}

/*
	Additive random number generator

	Modelled after "Algorithm A" in
	Knuth, D. E. (1981). The art of computer programming, volume 2, page 27.

	7/26/90 WRG
*/

static long	state[33] = {
	(long)0xd53f1852,  (long)0xdfc78b83,  (long)0x4f256096,  (long)0xe643df7,
	(long)0x82c359bf,  (long)0xc7794dfa,  (long)0xd5e9ffaa,  (long)0x2c8cb64a,
	(long)0x2f07b334,  (long)0xad5a7eb5,  (long)0x96dc0cde,  (long)0x6fc24589,
	(long)0xa5853646,  (long)0xe71576e2,  (long)0xdae30df,  (long)0xb09ce711,
	(long)0x5e56ef87,  (long)0x4b4b0082,  (long)0x6f4f340e,  (long)0xc5bb17e8,
	(long)0xd788d765,  (long)0x67498087,  (long)0x9d7aba26,  (long)0x261351d4,
	(long)0x411ee7ea,  (long)0x393a263,  (long)0x2c5a5835,  (long)0xc115fcd8,
	(long)0x25e9132c,  (long)0xd0c6e906,  (long)0xc2bc5b2d,  (long)0x6c065c98,
	(long)0x6e37bd55 };

#define r_off	12

static long	*rJ = &state[r_off],
			*rK = &state[DIM(state)-1];

NLM_EXTERN void LIBCALL Nlm_RandomSeed(long x)
{
	register size_t i;

	state[0] = x;
	/* linear congruential initializer */
	for (i=1; i<DIM(state); ++i) {
		state[i] = 1103515245*state[i-1] + 12345;
	}

	rJ = &state[r_off];
	rK = &state[DIM(state)-1];

	for (i=0; i<10*DIM(state); ++i)
		(void) Nlm_RandomNum();
}


/*
	Nlm_RandomNum --  return value in the range 0 <= x <= 2**31 - 1
*/
NLM_EXTERN long LIBCALL Nlm_RandomNum(void)
{
	register long	r;

	r = *rK;
	r += *rJ--;
	*rK-- = r;

	if (rK < state)
		rK = &state[DIM(state)-1];
	else
		if (rJ < state)
			rJ = &state[DIM(state)-1];
	return (r>>1)&0x7fffffff; /* discard the least-random bit */
}

NLM_EXTERN Nlm_FloatHi LIBCALL Nlm_LnFactorial (Nlm_FloatHi x) {
    if(x<0.0) 
        ErrPostEx(SEV_WARNING,0,0,"LogFact: Negative Argument to Factorial function!\n");
    if(x<=0.0)
        return 0.0;
    else
        return Nlm_LnGamma(x+1.0);
        
}
