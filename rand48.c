/*
 * Copyright (c) 2016 Phil Ronan
 * 
 * Disclaimer: This software is provided "as is", and without any warranty.
 * You are free to use and modify this software for any purpose, but please
 * include this copyright notice and disclaimer with any copies you make.
 */

/*

rand48 - a stand-alone replacement for the 48-bit random number functions
available in Linux, OS/X, etc.

Notes:
This code is intended to be used when porting legacy code to platforms that
do not support the rand48 functions. For new projects, there are much better
pseudo-random number generators out there (like arc4random, for example).
Whatever you do, don't use these functions for cryptography or in any other
situations where you need random numbers that are unpredictable

Functions:
lrand48 and nrand48: Generate a random unsigned int (from 0 to 2**31-1)
mrand48 and jrand48: Generate a random signed int (from -2**31 to 2**31-1)
drand48, erand48: Generate a random double-precision float (0 <= r < 1)
lcong48: Apply a different set of LCG parameters (seed, multiplicand and addend)
seed48: Provide a new seed, reset the multiplicand & addend, return the old seed
srand48: Update 32 bits of seed value from a 32-bit unsigned integer

The lrand48, mrand48 and drand48 functions operate on an internal seed value.
The nrand48, jrand48 and erand48 functions operate on a seed value provided as
a parameter.

Detailed documentation may be available here:
https://www.freebsd.org/cgi/man.cgi?query=drand48&sektion=3&manpath=FreeBSD+11.0-RELEASE

*/

#include "rand48.h"

#define R48_MULTIPLICAND 0x5deece66dULL
#define R48_ADDEND       11
#define R48_INITIAL_VAL  0x1234abcd330eULL
#define R48_MASK         0xffffffffffffULL


static unsigned short r48state[3] = { R48_INITIAL_VAL & 0xffff,
                                     (R48_INITIAL_VAL >> 16) & 0xffff,
                                     (R48_INITIAL_VAL >> 32) & 0xffff };

static unsigned short r48multiplicand[3] = { R48_MULTIPLICAND & 0xffff,
                                     (R48_MULTIPLICAND >> 16) & 0xffff,
                                     (R48_MULTIPLICAND >> 32) & 0xffff };

static unsigned short r48addend = R48_ADDEND;


/* Get a 48-bit number as a long integer */

inline static long long r48_getll(unsigned short s[3]) {
    return (long long)s[0] | ((long long)s[1] << 16) | ((long long)s[2] << 32);
}

/* Iterate the LCG state provided as a pointer to 3 16-bit unsigned ints */

static void r48_iterate(unsigned short state[3]) {
    unsigned long long hi, lo, multiplicand, result;
    lo = state[0] | ((unsigned long)state[1] << 16);
    hi = state[2];
    multiplicand = r48_getll(r48multiplicand);
    result = (((lo * multiplicand) & R48_MASK) + 
             (((hi * (multiplicand & 0xffff)) << 32) & R48_MASK) + r48addend) & R48_MASK;
    state[0] = result & 0xffffUL;
    state[1] = (result >> 16) & 0xffffUL;
    state[2] = (result >> 32) & 0xffffUL;
}

/* Generate a random unsigned int by iterating the internal buffer */
long lrand48(void) {
    long r;
    r48_iterate(r48state);
    r = r48state[2];
    r = (r << 15) | (r48state[1] >> 1);
    return r;
}

/* Generate a random unsigned int by iterating an external buffer */
long nrand48(unsigned short xsubi[3]) {
    long r;
    r48_iterate(xsubi);
    r = xsubi[2];
    r = (r << 15) | (xsubi[1] >> 1);
    return r;
}

/* Generate a random signed int by iterating the internal buffer */
long mrand48(void) {
    int r;
    r48_iterate(r48state);
    r = r48state[2];
    r = (r << 16) | r48state[1];
    return (long)r;
}

/* Generate a random signed int by iterating an external buffer */
long jrand48(unsigned short xsubi[3]) {
    int r;
    r48_iterate(xsubi);
    r = xsubi[2];
    r = (r << 16) | xsubi[1];
    return (long)r;
}

/* Generate a random value of type double by iterating the internal buffer */
double drand48(void) {
    r48_iterate(r48state);
    return r48_getll(r48state) * 0x1.0p-48;  /* (0x1.0p-48 = 1.0 / 2**48) */
}

/* Generate a random value of type double by iterating an external buffer */
double erand48(unsigned short xsubi[3]) {
    r48_iterate(xsubi);
    return r48_getll(xsubi) * 0x1.0p-48;  /* (0x1.0p-48 = 1.0 / 2**48) */
}

/* Change the LCG parameters */
void lcong48(unsigned short param[7]) {
    /* First three shorts == seed */
    r48state[0] = param[0];
    r48state[1] = param[1];
    r48state[2] = param[2];
    /* Second three == multiplicand */
    r48multiplicand[0] = param[3];
    r48multiplicand[1] = param[4];
    r48multiplicand[2] = param[5];
    /* Last short == addend */
    r48addend = param[6];
}

/* Change the seed value and reset the other LCG parameters, return old seed */
unsigned short *seed48(unsigned short seed16v[3]) {
    static unsigned short old_seed[3];
    /* Save old seed */
    old_seed[0] = r48state[0];
    old_seed[1] = r48state[1];
    old_seed[2] = r48state[2];
    /* Update new seed value */
    r48state[0] = seed16v[0];
    r48state[1] = seed16v[1];
    r48state[2] = seed16v[2];
    /* Reset multiplicand */
    r48multiplicand[0] = R48_MULTIPLICAND & 0xffff;
    r48multiplicand[1] = (R48_MULTIPLICAND >> 16) & 0xffff;
    r48multiplicand[2] = (R48_MULTIPLICAND >> 32) & 0xffff;
    /* Reset addend */
    r48addend = R48_ADDEND;
    return old_seed;
}

/* Update seed value from a 32-bit unsigned integer */
void srand48(long seedval) {
    r48state[0] = R48_INITIAL_VAL & 0xffff;   /* Arbitrarily set to 0x330e */
    r48state[1] = seedval & 0xffff;
    r48state[2] = (seedval >> 16) & 0xffff;
}

