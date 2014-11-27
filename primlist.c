/*
 * primlist.c
 * example program for bint.c
 * version: 1.0
 * author: ISJ
 * Date: 1992-06-25
 */

/* THe difference form primes.c and this program is that this program
 * computes primes (max MAX_PRIMES) until a key is hit
 */
#include <stdio.h>     /* prototype for printf()*/
#include <conio.h>

#include "bint.h"

/* this program is slower than a program which uses standard int's, but the
 * program can compute primes up to eg. 124854546546824110115908409845121,
 * this is not possible with int's!
 */


void print_prime(bint *a) {
	char s[1000]; /*should be enough*/
	/*convert to string*/
	bint_to_string(a,s);
	/*print out the prime*/
	printf("%s\n",s);
	cprintf("%s\r",s);
}



#define MAX_PRIMES 20000 /*number of primes to compute*/
bint huge prime[MAX_PRIMES];

int main(void) {
	/*this program computes primes*/
	int primes;

	bint testprime,zero;

	/* we have to enter at least 1 prime */
	primes=1;
	uint_to_bint(2,&testprime);
	print_prime(&testprime);
	prime[primes-1]=testprime;


	/*make zero 0*/
	uint_to_bint(0,&zero);


	/*we start testing at the last prime in table +1*/
	testprime=prime[primes-1];

	printf("Prime generator:\n");
	printf("Primes number 2-%d\n",MAX_PRIMES);
	while(primes<MAX_PRIMES && !kbhit()) {
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
		      bint_cmp((bint *)(&prime[primeptr]),&testsqrt)<=0) {
			bint remainder;

			bint_mod(&testprime,(bint *)(&prime[primeptr]),&remainder);
			if(bint_cmp(&remainder,&zero)==0)
				is_a_prime=0;
			else
				primeptr++;
		}
		if(is_a_prime) {
			/*got a prime*/
			prime[primes++]=testprime;
			print_prime((bint *)(&prime[primes-1]));
		}
	}
	putchar('\n');
	return 0;/*Ok*/
}


