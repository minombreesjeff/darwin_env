/*
 *	by Ian Ollmann
 *
 *	based on fdlibm cosh by Sun Microsystems, with improvements for efficiency and proper flag setting on i386. 
 *
 *	Copyright (C) 2005 by Apple Computer, Inc. All rights reserved.
 */



#include "math.h"
#include "math_private.h"

float coshf(  float x )
{
    static const float overflow = 183.2222702f;      //log(0x1.0p127)
    static const float ln2 = 0.693147180559945309417232121458176568L;                             //ln(2)
    float fabsx = __builtin_fabsf( x );
    float t, w;

	if( x != x )	return x + x;

    if( fabsx < __builtin_inff() )
    {
        if( fabsx < 0.5f * ln2 )
        {
			if( fabsx > 0x1.0p-50 )	//avoid underflow, save time
				fabsx = expm1l( fabsx );
            w = 1.0f + fabsx;
            if( fabsx < 0x1.0p-26 )
                return w;
            return 1.0f + (fabsx*fabsx)/(w+w);
        }
        
        if( fabsx < 22.0f )
        {
            t =  exp( fabsx );
            return 0.5f * t + 0.5f/t;
        }
        
        if( fabsx < overflow )
        {
            w = exp( 0.5f * fabsx );
            t = 0.5f * w;
            return t * w;
        }
		
		return fabsx * 0x1.0p126f;
    }

    //Nan or Inf result
    return fabsx + fabsx;
}

double cosh(  double x )
{
    static const double overflow = 1.477319723e+03L;      //log(0x1.0p1024)
    static const double ln2 = 0.693147180559945309417232121458176568L;                             //ln(2)
    double fabsx = __builtin_fabs( x );
    double t, w;

	if( x != x )	return x + x;

    if( fabsx < __builtin_inf() )
    {
        if( fabsx < 0.5 * ln2 )
        {
			if( fabsx > 0x1.0p-100 )	//avoid underflow, save time
				fabsx = expm1l( fabsx );
            w = 1.0 + fabsx;
            if( fabsx < 0x1.0p-55 )
                return w;
            return 1.0 + (fabsx*fabsx)/(w+w);
        }
        
        if( fabsx < 22 )
        {
            t =  exp( fabsx );
            return 0.5 * t + 0.5/t;
        }
        
        if( fabsx < overflow )
        {
            w = exp( 0.5 * fabsx );
            t = 0.5 * w;
            return t * w;
        }
		
		return fabsx * 0x1.0p1023;
    }

    //Nan or Inf result
    return fabsx + fabsx;
}


long double coshl( long double x )
{
    static const long double overflow = 1.13565234062941445534588410310297337926799095235775e+04L;      //log(0x1.0p16384)
    static const long double ln2 = 0.693147180559945309417232121458176568L;                             //ln(2)
    long double fabsx = __builtin_fabsl( x );
    long double t, w;

	if( x != x )	return x + x;

    if( fabsx < __builtin_infl() )
    {
        if( fabsx < 0.5 * ln2 )
        {
			if( fabsx > 0x1.0p-1000 )	//avoid underflow, save time
				fabsx = expm1l( fabsx );
            w = 1.0L + fabsx;
            if( fabsx < 0x1.0p-67 )
                return w;
            return 1.0L + (fabsx*fabsx)/(w+w);
        }
        
        if( fabsx < 22 )
        {
            t =  expl( fabsx );
            return 0.5L * t + 0.5L/t;
        }
        
        if( fabsx < overflow )
        {
            w = expl( 0.5L * fabsx );
            t = 0.5L * w;
            return t * w;
        }
		
		return fabsx * 0x1.0p16383L;
    }

    //Nan or Inf result
    return fabsx + fabsx;
}
