/*
 * math.h : mathematical function defintions
 * Copyright (c) 1985, 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)math.h	8.1 (Berkeley) 6/2/93
 */

#ifndef __MATH_H
#define __MATH_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef HUGE_VAL
/* Positive Infinity in a double */
#if 0
#define HUGE_VAL	(__extension__ 0x1.0p2047)
#else
#define HUGE_VAL (__DBL_MAX__ * 2.0)
#endif
#endif

#if !defined(_ANSI_SOURCE) && !defined(_POSIX_SOURCE)
#ifndef HUGE
#define	HUGE	HUGE_VAL
#endif
#endif

#define	M_E		2.7182818284590452354	/* e */
#define	M_LOG2E		1.4426950408889634074	/* log 2e */
#define	M_LOG10E	0.43429448190325182765	/* log 10e */
#define	M_LN2		0.69314718055994530942	/* log e2 */
#define	M_LN10		2.30258509299404568402	/* log e10 */
#define	M_PI		3.14159265358979323846	/* pi */
#define	M_PI_2		1.57079632679489661923	/* pi/2 */
#define	M_PI_4		0.78539816339744830962	/* pi/4 */
#define	M_1_PI		0.31830988618379067154	/* 1/pi */
#define	M_2_PI		0.63661977236758134308	/* 2/pi */
#define	M_2_SQRTPI	1.12837916709551257390	/* 2/sqrt(pi) */
#define	M_SQRT2		1.41421356237309504880	/* sqrt(2) */
#define	M_SQRT1_2	0.70710678118654752440	/* 1/sqrt(2) */

#if defined(__GNUC__)
#define __attrconst __attribute__((__const))
#else
#define __attrconst
#endif

/*
 *   prototypes for the standard C math functions.
 */
#if (__DBL_MANT_DIG != __FLT_MANT_DIG__)
double	acos (double);
double	asin (double);
double	atan (double);
double	atan2 (double, double);
double	sqrt (double);
double	ceil (double) __attrconst;
double	cos (double);
double	cosh (double);
double	exp (double);
double	fabs (double) __attrconst;
double	floor (double) __attrconst;
double	fmod (double, double) __attrconst;
double	frexp (double, int *);
double	ldexp (double, int);
double	log (double);
double	log10 (double);
double	modf (double, double *);
double	pow (double, double);
double	sin (double);
double	sinh (double);
double	tan (double);
double	tanh (double);
#endif /* (__DBL_MANT_DIG != __FLT_MANT_DIG__) */

/* 
 *  For PIC32, a long double is always 64 bits regardless of the setting of the
 *  -fno-short-double and -fshort-double options.
 */
#if !defined(_ANSI_SOURCE) && !defined(_POSIX_SOURCE)
long double	acosh (long double);
long double	asinh (long double);
long double	atanh (long double);
long double	cbrt (long double);
long double	copysign (long double, long double) __attrconst;
long double	drem (long double, long double);
long double	erf (long double);
long double	erfc (long double);
long double	expm1 (long double);
int	finite (long double) __attrconst;
long double	hypot (long double, long double);
#if defined(vax) || defined(tahoe)
long double	infnan (int);
#endif
int	isinf (long double) __attrconst;
int	isnan (long double) __attrconst;
long double	lgamma (long double);
long double	log1p (long double);
long double	logb (long double);
long double	rint (long double);
#endif  /* !defined(_ANSI_SOURCE) && !defined(_POSIX_SOURCE) */

/*
 *   prototypes for the float math functions.
 */
float acosf(float);
float asinf(float);
float atanf(float);
float atan2f(float, float);
float sqrtf (float);
float ceilf(float) __attrconst;
float cosf(float);
float coshf(float);
float expf(float);
float fabsf(float) __attrconst;
float floorf(float) __attrconst;
float fmodf(float, float) __attrconst;
float frexpf(float, int *);
float ldexpf(float, int);
float logf(float);
float log10f(float);

/*
 *   Without -fno-short-double, float * and double * are compatible, but a
 *   warning can still be produced.
 */
#if (__DBL_MANT_DIG__ == __FLT_MANT_DIG__)
float modff(float, void *);
#else
float modff(float, float *);
#endif

float powf(float, float);
float sinf(float);
float sinhf(float);
float sqrtf(float);
float tanf(float);
float tanhf(float);

#if (__DBL_MANT_DIG__ == __FLT_MANT_DIG__)
#define __MPROTO(x)  x ## f
#else
#define __MPROTO(x) x
#endif

#define	acos(p) __MPROTO(acos)(p)
#define	asin(p) __MPROTO(asin)(p)
#define	atan(p) __MPROTO(atan)(p)
#define	atan2(p1,p2) __MPROTO(atan2)(p1,p2)
#define	sqrt(p) __MPROTO(sqrt)(p)
#define	ceil(p) __MPROTO(ceil)(p)
#define	cos(p) __MPROTO(cos)(p)
#define	cosh(p) __MPROTO(cosh)(p)
#define	exp(p) __MPROTO(exp)(p)
#define	fabs(p) __MPROTO(fabs)(p)
#define	floor(p) __MPROTO(floor)(p)
#define	fmod(p1,p2) __MPROTO(fmod)(p1,p2)
#define	frexp(p1,p2) __MPROTO(frexp)(p1,p2)
#define	ldexp(p1,p2) __MPROTO(ldexp)(p1,p2)
#define	log(p) __MPROTO(log)(p)
#define	log10(p) __MPROTO(log10)(p)
#define	modf(p1,p2) __MPROTO(modf)(p1,p2)
#define	pow(p1,p2) __MPROTO(pow)(p1,p2)
#define	sin(p) __MPROTO(sin)(p)
#define	sinh(p) __MPROTO(sinh)(p)
#define	tan(p) __MPROTO(tan)(p)
#define	tanh(p) __MPROTO(tanh)(p)

#undef __attrconst

/* Global control over mchplibm error handling.  */

#define MATH_ERRNO     (1 << 0)
#define MATH_ERREXCEPT (1 << 1)

extern unsigned int __attribute__((section(".data.math_errhandling_flag"))) math_errhandling;


#ifdef __cplusplus
}
#endif


#endif

