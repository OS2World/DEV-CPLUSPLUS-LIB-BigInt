/*
 * prime.c
 * example program for bint.c
 * version: 1.0
 * author: ISJ
 * Date: 1992-05-27
 */

#include <stdio.h>     /* prototype for printf()*/
#include "bint.h"

/* this program is slower than a program which uses standard int's, but the
 * program can compute primes up to eg. 124854546546824110115908409845121,
 * this is not possible with int's!
 */

#define MAX_PRIMES 200 /*number of primes to compute*/

int main(void) {
	/*this program computes primes*/
	bint prime[MAX_PRIMES];
	int primes;

	bint testprime,zero;

	/* we have to enter at least 1 prime */
	primes=1;
	uint_to_bint(2,&prime[primes-1]);

	/*make zero 0*/
	uint_to_bint(0,&zero);


	/*we start testing at the last prime in table +1*/
	testprime=prime[primes-1];

	printf("Prime generator:\n");
	printf("Primes number 2-%d\n",MAX_PRIMES);
	while(primes<MAX_PRIMES) {
		bint testsqrt;
		int primeptr;
		int is_a_prime=1;

		bint_inc(&testprime);              /* increase*/
		bint_sqrt(&testprime,&testsqrt);   /* compute square root of
						      test - this speed up the
						      algorithm due to the fact
						      we dont have to test
						      division more than to the
						      square root of the number
						    */
		bint_inc(&testsqrt);
		primeptr=0;

		while(primeptr<primes &&
		      is_a_prime &&
		      bint_cmp(&prime[primeptr],&testsqrt)<=0) {
			bint remainder;

			bint_mod(&testprime,&prime[primeptr],&remainder);
			if(bint_cmp(&remainder,&zero)==0)
				is_a_prime=0;
			else
				primeptr++;
		}
		if(is_a_prime) {
			/*got a prime*/
			char s[100];
			prime[primes++]=testprime;
			/*convert to string*/
			bint_to_string(&testprime,s);
			/*print out the prime*/
			printf("%4.4d: %12.12s  ",primes,s);
		}
	}
	putchar('\n');
	return 0;/*Ok*/
}


