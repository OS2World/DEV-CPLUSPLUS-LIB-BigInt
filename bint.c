/*******************************************/
/*                                         */
/* BINT - Big INTeger                      */
/*                                         */
/* file: bint.c                            */
/*                                         */
/* author: ISJ                             */
/*                                         */
/* date: 1992-05-27                        */
/*                                         */
/*******************************************/

#include "bint.h"
#include <math.h>           /* floor()*/
#include <stddef.h>         /* definition of NULL */

static void bint_set_0(bint *a) {
	/* internal function to set a to zero*/
	int p;
	for(p=0;p<BINT_ELEMENTS;p++) a->e[p]=0;
}


/* converters: */

bint *uint_to_bint(unsigned int a,bint *result) {
	/* convert a integer to a bint */
	int p;
	if(result==NULL) return NULL;
	bint_set_0(result);

	for(p=0;p<BINT_ELEMENTS;p++) {
		result->e[p]=a%BINT_ELEM_RANGE;
		a/=BINT_ELEM_RANGE;
	}
	return result;
}


bint *long_to_bint(long a,bint *result)
{
	/* convert a long integer to a bint */
	int p;
	if(result==NULL) return NULL;
	bint_set_0(result);

	for(p=0;p<BINT_ELEMENTS;p++) {
		result->e[p]=a%BINT_ELEM_RANGE;
		a/=BINT_ELEM_RANGE;
	}
	return result;
}


bint *double_to_bint(double a,bint *result)
{
	/* convert a double to a bint */
	int p;
	if(result==NULL) return NULL;
	bint_set_0(result);

	for(p=0;p<BINT_ELEMENTS;p++) {
		result->e[p]=a-(BINT_ELEM_RANGE*floor(a/BINT_ELEM_RANGE));
		a/=BINT_ELEM_RANGE;
	}
	return result;
}


unsigned int bint_to_uint(bint *a) {
	unsigned int result;
	int p;

	if(a==NULL) return 0;
	result=0;
	for(p=BINT_ELEMENTS-1;p>=0;p--) {
		result=result*BINT_ELEM_RANGE+a->e[p];
	}
	return result;
}


long bint_to_long(bint *a) {
	long result;
	int p;

	if(a==NULL) return 0;
	result=0;
	for(p=BINT_ELEMENTS-1;p>=0;p--) {
		result=result*BINT_ELEM_RANGE+a->e[p];
	}
	return result;
}


double bint_to_double(bint *a) {
	double result;
	int p;

	if(a==NULL) return 0;
	result=0;
	for(p=BINT_ELEMENTS-1;p>=0;p--) {
		result=result*BINT_ELEM_RANGE+a->e[p];
	}
	return result;
}


bint *bint_inc(bint *result) {
	/* increase result by 1*/
	/*pre:none*/
	/*post: result'=result+1*/
	/*note: overflow is not detected*/
	int p;
	p=0;

	if(result==NULL) return NULL;
	do{
		if(result->e[p]==BINT_ELEM_MAX) {
			result->e[p]=0;
			p++;
		} else {
			++(result->e[p]);
			p=BINT_ELEMENTS;/*forced termination - better than goto! */
		}
	}while(p<BINT_ELEMENTS);
	return result;
}

bint *bint_dec(bint *result) {
	/*decrease result by 1*/
	/*pre:none*/
	/*post: result'=result-1*/
	/*note: overflow is not detected*/
	int p;
	p=0;

	if(result==NULL) return NULL;
	do{
		if(result->e[p]==0) {
			result->e[p]=BINT_ELEM_MAX;
			p++;
		} else {
			--(result->e[p]);
			p=BINT_ELEMENTS;/*forced termination - better than goto! */
		}
	}while(p<BINT_ELEMENTS);
	return result;
}


bint *bint_complement1(bint *a,bint *result) {
	/*this is equivalent to the C operator ^ */
	int p;
	if(a==NULL || result==NULL) return NULL;
	for(p=0;p<BINT_ELEMENTS;p++)
		result->e[p]=(a->e[p]^BINT_ELEM_MAX);
	return result;
}


bint *bint_complement2(bint *a,bint *result)
{
	/*2-complement is used to make subtraction almost the same as addition*/
	if(a==NULL) return NULL;
	return bint_inc(bint_complement1(a,result));
}


bint *bint_add(bint *a,bint *b,bint *result)
{
	/* add to bints*/
	int p;
	int carry;

	if(a==NULL || b==NULL || result==NULL) return NULL;
	for(carry=0,p=0;p<BINT_ELEMENTS;p++) {
		BINT_LARGER_TYPE r;
		r=a->e[p]+b->e[p]+carry;
		result->e[p]=(BINT_ELEMTYPE)r;
		carry=(r>BINT_ELEM_MAX);
	}
	return result;
}

bint *bint_sub(bint *a,bint *b,bint *result)
{
	/*subtract to bints - uses the 2-complement method*/
	bint tmp;
	if(a==NULL || b==NULL || result==NULL) return NULL;
	return bint_add(a,
			bint_complement2(b,&tmp),
			result);
}


bint *bint_div2(bint *a,bint *result)
{
	/*divide a bint by 2 - the same as shift right 1*/
	int p;
	int carry;
	if(a==NULL || result==NULL) return NULL;
	/*bit-wise shift is the easiest and the fastest method*/
	for(p=BINT_ELEMENTS-1,carry=0;p>=0;p--) {
		int carry2;
		carry2=a->e[p]&1; /* get the 0th bit*/
		result->e[p]=  a->e[p]>>1;
		result->e[p]|= (carry<<(BINT_ELEM_BITS-1));
		carry=carry2;
	}
	return result;
}


bint *bint_mod2(bint *a,bint *result)
{
	bint tmp;
	if(a==NULL || result==NULL) return NULL;
	tmp=*a;/* a might be the same as result*/
	bint_set_0(result);
	result->e[0]|= (tmp.e[0]&(BINT_ELEM_MAX-1));
	return result;
}


bint *bint_mul2(bint *a,bint *result)
{
	/*multiply a bint by 2 - the is a left shift */
	/*note: overflow is not detected*/
	int p;
	int carry;
	if(a==NULL || result==NULL) return NULL;
	/*bit-wise shift is the easiest and the fastest method*/
	for(p=0,carry=0;p<BINT_ELEMENTS;p++) {
		int carry2;
		carry2=(a->e[p])>>(BINT_ELEM_BITS-1); /* get MSB*/
		result->e[p]= (a->e[p]<<1)|carry;
		carry=carry2;
	}
	return result;
}

int bint_is_zero(bint *a) {
	int p;
	for(p=0;p<BINT_ELEMENTS;p++) {
		if(a->e[p]!=0) return 0;
	}
	return 1;
}

int bint_cmp(bint *a,bint *b)
{
	/* compare two bints - works like strcmp - qsort() compatible*/
	int p;
	BINT_ELEMTYPE *aptr,*bptr;
	if(a==NULL || b==NULL) return 0;
	aptr=&(a->e[BINT_ELEMENTS-1]);
	bptr=&(b->e[BINT_ELEMENTS-1]);
	p=BINT_ELEMENTS-1;
	while(p>=0) {
		if(*aptr>*bptr) return 1;
		if(*aptr<*bptr) return -1;
		p--;aptr--;bptr--;
	}
	return 0;
}


void bint_divmod(bint *A,bint *B,bint *quotient,bint *remainder)
{
	/*divide a bint giving quotient and remainder */
	/*pre: B<>0*/
	bint q,r,b;
	if(A==NULL || B==NULL || quotient==NULL || remainder==NULL) return;

	bint_set_0(&q);
	if(bint_is_zero(B)) return;/*cannot divide by zero*/
	r=*A;
	b=*B;
	while(bint_cmp(&r,&b)>=0) {
		bint_mul2(&b,&b);
	}
	while(bint_cmp(&b,B)!=0) {
		bint_mul2(&q,&q);
		bint_div2(&b,&b);
		if(bint_cmp(&r,&b)>=0) {
			bint_inc(&q);
			bint_sub(&r,&b,&r);
		}
	}
	*quotient=q;
	*remainder=r;
}


bint *bint_div(bint *a,bint *b,bint *result)
{
	/*divide a by b*/
	/*pre: b<>0*/
	bint dummy;
	bint_divmod(a,b,result,&dummy);
	return result;
}


bint *bint_mod(bint *a,bint *b,bint *result)
{
	/*returns the remainder from the division of a by b*/
	/*pre:b<>0*/
	bint dummy;
	bint_divmod(a,b,&dummy,result);
	return result;
}


bint *bint_mul(bint *a,bint *b,bint *result)
{
	bint ta,tb;


	if(a==NULL || b==NULL || result==NULL) return NULL;

	ta=*a;tb=*b;
	bint_set_0(result);
	/* this algoritm is basicly the same method as the one taught in
	 * primary school, it just works on binary numbers
	 */
	while(!bint_is_zero(&tb)) {
		if((tb.e[0]&1)!=0) {
			bint_add(&ta,result,result);
		}
		bint_div2(&tb,&tb);
		bint_mul2(&ta,&ta);
	}
	return result;
}


bint *bint_and(bint *a,bint *b,bint *result)
{
	/* this function ANDs two numbers binary (like the operater &)*/
	int p;
	if(a==NULL || b==NULL || result==NULL) return NULL;
	for(p=0;p<BINT_ELEMENTS;p++)
		result->e[p]=a->e[p]&b->e[p];
	return result;
}

bint *bint_or(bint *a,bint *b,bint *result)
{
	/* this function ORs two numbers binary (like the operater |)*/
	int p;
	if(a==NULL || b==NULL || result==NULL) return NULL;
	for(p=0;p<BINT_ELEMENTS;p++)
		result->e[p]=a->e[p]|b->e[p];
	return result;
}

bint *bint_xor(bint *a,bint *b,bint *result)
{
	/* this function XORs two numbers binary (like the operater ^)*/
	int p;
	if(a==NULL || b==NULL || result==NULL) return NULL;
	for(p=0;p<BINT_ELEMENTS;p++)
		result->e[p]=a->e[p]^b->e[p];
	return result;
}

bint *bint_setbit(bint *a,int bitno)
{
	/*set the bitno'th bit to 1*/
	if(a==NULL) return NULL;
	if(bitno>=0 && bitno<BINT_ELEMENTS*BINT_ELEM_BITS) {
		int elemno,bit;
		elemno=bitno/BINT_ELEM_BITS;
		bit=bitno%BINT_ELEM_BITS;
		a->e[elemno]|=(1<<bit);
	}
	return a;
}

bint *bint_resetbit(bint *a,int bitno)
{
	/*set the bitno'th bit to 0*/
	if(a==NULL) return NULL;
	if(bitno>=0 && bitno<BINT_ELEMENTS*BINT_ELEM_BITS) {
		int elemno,bit;
		elemno=bitno/BINT_ELEM_BITS;
		bit=bitno%BINT_ELEM_BITS;
		a->e[elemno]&=(~(1<<bit));
	}
	return a;
}

int bint_getbit(bint *a,int bitno)
{
	/*gets the the bitno'th bit*/
	if(a==NULL) return 0;
	if(bitno>=0 && bitno<BINT_ELEMENTS*BINT_ELEM_BITS) {
		int elemno,bit;
		elemno=bitno/BINT_ELEM_BITS;
		bit=bitno%BINT_ELEM_BITS;
		return (a->e[elemno]>>bit)&1;
	}
	return 0;
}

char *bint_to_string(bint *a,char *s)
{
	/*this function converts a bint to a string representation (decimal*/
	bint tmp=*a;
	char *sptr=s;
	bint ten;
	uint_to_bint(10,&ten);
	/*note: this loop puts the digits in the string in reverse*/
	while(!bint_is_zero(&tmp)) {
		bint r1,r2;
		unsigned int remainder;
		bint_divmod(&tmp,&ten,&r1,&r2);
		remainder=bint_to_uint(&r2);
		*sptr='0'+remainder;
		sptr++;
		tmp=r1;
	}
	if(sptr==s) {
		/* then it was initially zero*/
		*sptr='0';
		*(sptr+1)='\0';
	} else {
		char *tmpptr=s;

		*sptr='\0';
		sptr--;
		/*reverse the string*/
		while(tmpptr<sptr) {
			char tmpchar=*tmpptr;
			*tmpptr=*sptr;
			*sptr=tmpchar;
			sptr--;
			tmpptr++;
		}
	}
	return s;
}


bint *bint_gcd(bint *a,bint *b,bint *result)
{
	bint x,y;

	/*greatest common divisor*/
	if(a==NULL || b==NULL || result==NULL) return NULL;
	x=*a;y=*b;
	if(bint_is_zero(&x) || bint_is_zero(&y)==0) {
		/* if x or y is zero there is no common divisor*/
		return NULL;
	}
	while(bint_cmp(&x,&y)!=0) {
		if(bint_cmp(&x,&y)<0)
			bint_sub(&y,&x,&y);
		else
			bint_sub(&x,&y,&x);
	}
	*result=x;
	return result;
}

bint *bint_log2(bint *a,bint *result)
{
	/*return the 2-logarithm of a*/
	int p;
	int bitno;

	if(a==NULL || result==NULL) return NULL;
	for(p=BINT_ELEMENTS-1;p>0 && a->e[p]==0;p--);
	for(bitno=BINT_ELEM_BITS-1;bitno>0 && ((a->e[p]>>bitno)&1)==1;bitno--);
	return uint_to_bint(p*BINT_ELEM_BITS+bitno,result);
}

bint *bint_sqrt(bint *a,bint *result)
{
	/*square root of a*/
	bint x,y,yminusone;

	if(a==NULL || result==NULL) return NULL;
	bint_set_0(&x);

	y=*a;
	bint_inc(&y);
	yminusone=*a;
	while(bint_cmp(&x,&yminusone)<0) {
		bint h;
		bint sqrh;
		bint_add(&x,&y,&h);
		bint_div2(&h,&h);
		bint_mul(&h,&h,&sqrh);
		if(bint_cmp(&sqrh,a)<=0)
			x=h;
		else {
			y=h;
			yminusone=y;bint_dec(&yminusone);
		}
	}
	*result=x;
	return result;
}

bint *bint_pow(bint *a,bint *b,bint *result)
{
	/*power - returns a raised to b*/
	bint r,x,y;

	if(a==NULL || b==NULL || result==NULL) return NULL;

	uint_to_bint(1,&r);
	x=*a;
	y=*b;
	while(!bint_is_zero(&y)) {
		bint mod2;
		bint_mod2(&y,&mod2);
		if(bint_is_zero(&mod2)) {
			bint_mul(&r,&x,&r);
			bint_dec(&y);
		} else {
			bint_mul(&x,&x,&x);
			bint_div2(&y,&y);
		}
	}
	*result=r;
	return result;
}

bint *bint_powmod(bint *a,bint *b,bint *c,bint *result)
{
	/*power and modulus*/
	/*pre: c<>0*/
	/*post: result=a^b mod c*/
	/*this function is especially good for RSA-encryption*/
	bint r,x,y;

	if(a==NULL || b==NULL || c==NULL || result==NULL) return NULL;

	uint_to_bint(1,&r);
	x=*a;
	y=*b;
	while(!bint_is_zero(&y)) {
		bint mod2;
		bint_mod2(&y,&mod2);
		if(bint_is_zero(&mod2)) {
			bint_mul(&r,&x,&r);
			bint_dec(&y);
		} else {
			bint_mul(&x,&x,&x);
			bint_div2(&y,&y);
		}
		bint_mod(&r,c,&r);/* it is safe do a modulus here
				     - i dont know why, but it works!*/
	}
	*result=r;
	return result;
}

/*That's all folks!*/
