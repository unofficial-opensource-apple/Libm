/*
 *  xmm_nextafter.c
 *  xmmLibm
 *
 *  Created by Ian Ollmann on 8/19/05.
 *  Copyright 2005 Apple Computer. All rights reserved.
 *
 */

#include "fenv.h"
#include "xmmLibm_prefix.h"

static inline double _nextafter( double x, double y ) ALWAYS_INLINE;
static inline double _nextafter( double x, double y )
{
    static const double smallest = 0x0.0000000000001p-1022;

    //must be a x or y is NaN
    if( EXPECT_FALSE( x != x ) )
        return x + x;
    
    if( EXPECT_TRUE( x < y ) )
    {
		if( EXPECT_FALSE( x == - __builtin_inf() ) )
			return -0x1.fffffffffffffp1023;
		if( EXPECT_FALSE( x == - smallest ) )		// Added so that nextafter(-smallest, 0) = -0
			return -0.0;													// This is not required under C99, but we
																						// believe that this is the way 754 is going.

        int oldmxcsr = _mm_getcsr();
        int newmxcsr = (oldmxcsr & ~ROUND_MASK ) | ROUND_TO_INFINITY;
        _mm_setcsr( newmxcsr );
         
        x += smallest;
    
        oldmxcsr |= _mm_getcsr() & ALL_FLAGS;
        _mm_setcsr( oldmxcsr );
        return x;
    }

    if( EXPECT_TRUE( x > y ) )
    {
		if( EXPECT_FALSE( x == __builtin_inf() ) )
			return 0x1.fffffffffffffp1023;

        int oldmxcsr = _mm_getcsr();
        int newmxcsr = (oldmxcsr & ~ROUND_MASK ) | ROUND_TO_NEG_INFINITY;
        _mm_setcsr( newmxcsr );
         
        x -= smallest;
    
        oldmxcsr |= _mm_getcsr() & ALL_FLAGS;
        _mm_setcsr( oldmxcsr );
        return x;
    }

    if( EXPECT_TRUE( x == y ) )
        return y;
        
    return y + y;
}

#if defined( BUILDING_FOR_CARBONCORE_LEGACY )


float nextafterf( float x, float y )
{
    static const float smallest = 0x0.000002p-126f;

    //must be a x or y is NaN
    if( EXPECT_FALSE( x != x ) )
        return x + x;

    if( EXPECT_TRUE( x < y ) )
    {
		if( EXPECT_FALSE( x == - __builtin_inff() ) )
			return -0x1.fffffep127f;
		if( EXPECT_FALSE( x == - smallest ) )		// Added so that nextafter(-smallest, 0) = -0
			return -0.0f;												  // This is not required under C99, but we
																						// believe that this is the way 754 is going.

        int oldmxcsr = _mm_getcsr();
        int newmxcsr = (oldmxcsr & ~ROUND_MASK ) | ROUND_TO_INFINITY;
        _mm_setcsr( newmxcsr );
         
        x += smallest;

        oldmxcsr |= _mm_getcsr() & ALL_FLAGS;
        _mm_setcsr( oldmxcsr );
        return x;
    }
    
    if( EXPECT_TRUE( x > y ) )
    {
		if( EXPECT_FALSE( x == __builtin_inff() ) )
			return 0x1.fffffep127f;

        int oldmxcsr = _mm_getcsr();
        int newmxcsr = (oldmxcsr & ~ROUND_MASK ) | ROUND_TO_NEG_INFINITY;
        _mm_setcsr( newmxcsr );
         
        x -= smallest;

        oldmxcsr |= _mm_getcsr() & ALL_FLAGS;
        _mm_setcsr( oldmxcsr );
        return x;
    }

    if( EXPECT_TRUE( x == y ) )
        return y;
        
    return y + y;
}

double nextafterd( double x, double y )
{
	return _nextafter( x, y );
}

#else

double nextafter( double x, double y )
{
	return _nextafter( x, y );
}

/*
long double nextafterl( long double x, long double y )
{
    static const long double smallest = 0x0.0000000000000002p-16382L;
    static const long double tiny = 0x1.0000000000000000p-16382L;
    struct
	{
			uint32_t	control;
			uint32_t	status;
			uint32_t	tag;
			uint32_t	ip;
			uint32_t	stuff;
			uint32_t	op;
			uint32_t	stuff2;
	}env;

    //must be a x or y is NaN
    if( EXPECT_FALSE( x != x ) )
        return x + x;

    if( EXPECT_TRUE( x < y ) )
    {
		if( EXPECT_FALSE( x == - __builtin_infl() ) )
			return -0x1.fffffffffffffffep16383L;
	
        asm volatile( "fnstenv %0" : "=m" (*&env ));
		uint16_t newcontrol = (env.control & 0xf3ff) | 0x0832;			//Round to inf, mask out  precision, underflow and denormal exceptions
        asm volatile( "fldcw %0" : : "m" (*&newcontrol ));
         
        long double result = x + smallest;

		//restore rounding mode and masks
        asm volatile( "fldcw %0" : : "m" (*&env.control ));

        return result;
    }

    if( EXPECT_TRUE( x > y ) )
    {
		if( EXPECT_FALSE( x == __builtin_infl() ) )
			return 0x1.fffffffffffffffep16383L;

        asm volatile( "fnstenv %0" : "=m" (*&env ));
		uint16_t newcontrol = (env.control & 0xf3ff) | 0x0432;			//Round to -inf, mask out  precision, underflow and denormal exceptions
        asm volatile( "fldcw %0" : : "m" (*&newcontrol ));
         
        long double result = x - smallest;
    
		//restore rounding mode and masks
        asm volatile( "fldcw %0" : : "m" (*&env.control ));

        return result;
    }

    if( EXPECT_TRUE( x == y ) )
        return y;
        
    return y + y;
}
*/

float nexttowardf( float x, long double y )
{
    static const float smallest = 0x0.000002p-126f;

    //must be a x or y is NaN
    if( EXPECT_FALSE( x != x ) )
        return x + x;

    if( EXPECT_TRUE( x < y ) )
    {
		if( EXPECT_FALSE( x == - __builtin_inff() ) )
			return -0x1.fffffep127f;

        int oldmxcsr = _mm_getcsr();
        int newmxcsr = (oldmxcsr & ~ROUND_MASK ) | ROUND_TO_INFINITY;
        _mm_setcsr( newmxcsr );
         
        x += smallest;

        oldmxcsr |= _mm_getcsr() & ALL_FLAGS;
        _mm_setcsr( oldmxcsr );
        return x;
    }
    
    if( EXPECT_TRUE( x > y ) )
    {
		if( EXPECT_FALSE( x == __builtin_inff() ) )
			return 0x1.fffffep127f;

        int oldmxcsr = _mm_getcsr();
        int newmxcsr = (oldmxcsr & ~ROUND_MASK ) | ROUND_TO_NEG_INFINITY;
        _mm_setcsr( newmxcsr );
         
        x -= smallest;

        oldmxcsr |= _mm_getcsr() & ALL_FLAGS;
        _mm_setcsr( oldmxcsr );
        return x;
    }

    if( EXPECT_TRUE( x == y ) )
        return y;
        
    return y + y;
}

double nexttoward( double x, long double y )
{
    static const double smallest = 0x0.0000000000001p-1022;

    //must be a x or y is NaN
    if( EXPECT_FALSE( x != x ) )
        return x + x;
    
    if( EXPECT_TRUE( x < y ) )
    {
		if( EXPECT_FALSE( x == - __builtin_inf() ) )
			return -0x1.fffffffffffffp1023;

        int oldmxcsr = _mm_getcsr();
        int newmxcsr = (oldmxcsr & ~ROUND_MASK ) | ROUND_TO_INFINITY;
        _mm_setcsr( newmxcsr );
         
        x += smallest;
    
        oldmxcsr |= _mm_getcsr() & ALL_FLAGS;
        _mm_setcsr( oldmxcsr );
        return x;
    }

    if( EXPECT_TRUE( x > y ) )
    {
		if( EXPECT_FALSE( x == __builtin_inf() ) )
			return 0x1.fffffffffffffp1023;

        int oldmxcsr = _mm_getcsr();
        int newmxcsr = (oldmxcsr & ~ROUND_MASK ) | ROUND_TO_NEG_INFINITY;
        _mm_setcsr( newmxcsr );
         
        x -= smallest;
    
        oldmxcsr |= _mm_getcsr() & ALL_FLAGS;
        _mm_setcsr( oldmxcsr );
        return x;
    }

    if( EXPECT_TRUE( x == y ) )
        return y;
        
    return y + y;
}

/*
long double nexttowardl( long double x, long double y )
{
    return nextafterl( x, y );
}
*/

#endif

