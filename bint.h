/*********************************************/
/*                                           */
/* BINT - Big INTeger                        */
/*                                           */
/* File: bint.h                              */
/*                                           */
/* Author: ISJ                               */
/*                                           */
/* Language: Borland C++ - but wery portable */
/*                                           */
/*********************************************/

#ifndef BINT_H
#define BINT_H

/*Here you can configure for your environment to get optimum perfomance
 *eg: change the element type to unsigned int
 *Note: 1. remember to change the other defines too
 *      2. the type must be a unsigned type
 */

/*the type used to make the bint:*/
#define BINT_ELEMTYPE	unsigned char
/*number of bits in the type:*/
#define BINT_ELEM_BITS	8
/*number of elements in a bint:*/
#define BINT_ELEMENTS	16
/*number values int the type (including zero):*/
#define BINT_ELEM_RANGE	256
/*maximum value of the type (BINT_ELEM_RANGE-1):*/
#define BINT_ELEM_MAX	255
/*a type which is larger than BINT_ELEMTYPE */
#define BINT_LARGER_TYPE int

typedef struct tagBINT {
	BINT_ELEMTYPE	e[BINT_ELEMENTS];
} bint;

/*converters - something to bint */
bint *uint_to_bint(unsigned int a,bint *result);
bint *long_to_bint(long a,bint *result);
bint *double_to_bint(double a,bint *result);

/*converters - bint to something */
unsigned int bint_to_uint(bint *a);
long bint_to_long(bint *a);
double bint_to_double(bint *a);

/* basic operations */
bint *bint_inc(bint *result);
bint *bint_dec(bint *result);
bint *bint_complement1(bint *a,bint *result);
bint *bint_complement2(bint *a,bint *result);
bint *bint_add(bint *a,bint *b,bint *result);
bint *bint_sub(bint *a,bint *b,bint *result);

/* simple operations */
bint *bint_div2(bint *a,bint *result);
bint *bint_mod2(bint *a,bint *result);
bint *bint_mul2(bint *a,bint *result);
int bint_is_zero(bint *a);
int bint_cmp(bint *a,bint *b);
void bint_divmod(bint *A,bint *B,bint *result1,bint *result2);
bint *bint_div(bint *a,bint *b,bint *result);
bint *bint_mod(bint *a,bint *b,bint *result);
bint *bint_mul(bint *a,bint *b,bint *result);

/* bit-oriented operations */
bint *bint_and(bint *a,bint *b,bint *result);
bint *bint_or(bint *a,bint *b,bint *result);
bint *bint_xor(bint *a,bint *b,bint *result);
bint *bint_setbit(bint *a,int bitno);
bint *bint_resetbit(bint *a,int bitno);
int bint_getbit(bint *a,int bitno);

/*output operations*/
char *bint_to_string(bint *a,char *s);

/* complex arithmics*/
bint *bint_gcd(bint *a,bint *b,bint *result);/*greatest common divisor*/
bint *bint_log2(bint *a,bint *result);
bint *bint_sqrt(bint *a,bint *result);/*square root*/
bint *bint_pow(bint *a,bint *b,bint *result);/*power*/
bint *bint_powmod(bint *a,bint *b,bint *c,bint *result);/*power and modulus*/

#endif