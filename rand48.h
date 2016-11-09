/*
 * Copyright (c) 2016 Phil Ronan
 * 
 * Disclaimer: This software is provided "as is", and without any warranty.
 * You are free to use and modify this software for any purpose, but please
 * include this copyright notice and disclaimer with any copies you make.
 */

#ifndef SO_RAND48
#define SO_RAND48

/* Generate a random unsigned int by iterating the internal buffer */
long lrand48(void);

/* Generate a random unsigned int by iterating an external buffer */
long jrand48(unsigned short xsubi[3]);

/* Generate a random signed int by iterating the internal buffer */
long mrand48(void);

/* Generate a random signed int by iterating an external buffer */
long nrand48(unsigned short xsubi[3]);

/* Generate a random value of type double by iterating the internal buffer */
double drand48(void);

/* Generate a random value of type double by iterating an external buffer */
double erand48(unsigned short xsubi[3]);

/* Change the LCG parameters */
void lcong48(unsigned short param[7]);

/* Change the seed value and reset the other LCG parameters, return old seed */
unsigned short *seed48(unsigned short seed16v[3]);

/* Update seed value from a 32-bit unsigned integer */
void srand48(long seedval);

#endif
