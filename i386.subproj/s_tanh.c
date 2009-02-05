/*
 * by Ian Ollmann
 * Copyright � 2005, Apple Computer Inc. All rights reserved.
 *
 *	based on MathLib v3
 */
 
#include <math.h>

float tanhf( float x )
{
    static const float overflow = 183.2222702f/2.0;      //log(0x1.0p127)
    float fabsx = __builtin_fabsf( x );
    
	if( x != x )	return x + x;
	
	if( fabsx > 0x1.0p-12f )		//sqrt( negative epsilon )
	{
		if( fabsx < overflow )
		{
			fabsx = expm1f( -2.0L * fabsx );
			fabsx = -fabsx / (2.0L + fabsx );
		}
		else
			fabsx = 1.0f;
	}
	else
	{
		if( x == 0.0 )
			return x;
	
		fabsx *= 0x1.0p25;
		fabsx -= 0x1.0p-126f;
		fabsx *= 0x1.0p-25;
	}

	if( x < 0 )
		fabsx = -fabsx;
	
	return fabsx;
}

double tanh( double x )
{
    static const double overflow = 1.477319723e+03L/2.0;      //log(0x1.0p1024)
    double fabsx = __builtin_fabs( x );
    
	if( x != x )	return x + x;
	
	if( fabsx > 0x1.0p-27 )		//sqrt( negative epsilon )
	{
		if( fabsx < overflow )
		{
			fabsx = expm1( -2.0 * fabsx );
			fabsx = -fabsx / (2.0 + fabsx );
		}
		else
			fabsx = 1.0;
	}
	else
	{
		if( x == 0.0 )
			return x;
	
		fabsx *= 0x1.0p55;
		fabsx -= 0x1.0p-1022;
		fabsx *= 0x1.0p-55;
	}

	if( x < 0 )
		fabsx = -fabsx;
	
	return fabsx;
}

long double tanhl( long double x )
{
    static const long double overflow = 1.13565234062941445534588410310297337926799095235775e+04L / 2.0L;      //log(0x1.0p16384)
    long double fabsx = __builtin_fabsl( x );
    
	if( x != x )	return x + x;
	
	if( fabsx > 0x1.0p-32 )		//sqrt( negative epsilon )
	{
		if( fabsx < overflow )
		{
			fabsx = expm1l( -2.0L * fabsx );
			fabsx = -fabsx / (2.0L + fabsx );
		}
		else
			fabsx = 1.0L;
	}
	else
	{
		if( x == 0.0 )
			return x;
	
		fabsx *= 0x1.0p65;
		fabsx -= 0x1.0p-16382L;
		fabsx *= 0x1.0p-65;
	}

	if( x < 0 )
		fabsx = -fabsx;
	
	return fabsx;
}
