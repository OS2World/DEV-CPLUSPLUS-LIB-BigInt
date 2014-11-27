/* Optimized version of bigInt 1.0
 * Optimized for: intel 80386 in Borland C++ 3.1
 * What have been optimized:
 *   memcmprvs()
 */

/* to avoid problems with signed/unsigned char functions in the runtime
 * library we have to switch to 'default char = signed char'
 */
#pragma option -K-
#define I asm
#if defined(__COMPACT__) || defined(__LARGE_) || defined(__HUGE__)
#define LDATA 1
#else
#define LDATA 0
#endif


#include "bigint.hpp"
#include <mem.h>
#include <limits.h>
#include <ctype.h>


template<class A,class B> inline A max(A a,B b) {return a>b?a:b; }
template<class A,class B> inline A min(A a,B b) {return a<b?a:b; }

int BigInt::IsNegative() const {
	return (bits[bigint_unitprecis-1]>>(bigint_unitbits-1))==1;
}

int BigInt::IsZero() const {
	const UnitType *p=bits;
	for(int x=0;x<bigint_unitprecis;x++,p++)
		if(*p!=0) return 0;
	return 1;
}

int BigInt::IsOdd() const {
	return (bits[0]&1)==1;
}

void BigInt::incr() {
	UnitType *p=bits;
	for(int x=0;x<bigint_unitprecis;x++,p++) {
		if(*p==bigint_unitmax)
			*p=0;
		else {
			++*p;
			break;
		}
	}
}

void BigInt::decr() {
	UnitType *p=bits;
	for(int x=0;x<bigint_unitprecis;x++,p++) {
		if(*p==0)
			*p=bigint_unitmax;
		else {
			--*p;
			break;
		}
	}
}


BigInt::BigInt(unsigned char x) {
	memset(bits,0,bigint_bytes);
	bits[0]=UnitType(x);
}

BigInt::BigInt(signed char x) {
	if(x<=0)
		memset(bits,-1,bigint_bytes);
	else
		memset(bits,0,bigint_bytes);
	bits[0]=UnitType(x);
}

BigInt::BigInt(unsigned short x) {
	memset(bits,0,bigint_bytes);
	UnitType *p=bits;
	while(x && (p-bits)<bigint_unitprecis) {
		UnitType c=x&bigint_unitmax;
		*p++=c;
		x>>=bigint_unitbits;
	}
}

BigInt::BigInt(signed short x) {
	if(x<0)
		memset(bits,-1,bigint_bytes);
	else
		memset(bits,0,bigint_bytes);
	UnitType *p=bits;
	unsigned short xt=(unsigned short)(x);
	while(xt) {
		UnitType c=xt&bigint_unitmax;
		*p++=c;
#ifdef __BORLANDC__
	/* Borland C++ DOES generate partly correct code, but it seems that
	 * the 80386+ processor ignores the instruction:
	 *      shl dx,20h or: shl edx,20h   (Note: 20h == 32 !)
	 * strangly the instruction:
	 *      shl dx,10h
	 * behaves correctly even though it too gives total loss of precision
	 * So we have to make a hack to ensure that if we are using 32-bit
	 * units (longs) that the shift is done in two tempi. This has been
	 * additionally hacked: BC++ does not provide a built-in type widers
	 * that 32 bit (long), so we can just as well terminate the loop!
	 * The problem only arises when UnitType::= unsigned long and
	 * generating 80186+ code. Multi-bit shifts were first implemented on
	 * the 80186 processor, so on 8086 a 32-bit shift is done by 32 1-bit
	 * shifts and no the problem should not occur.
	 */
#if bigint_unitbits==32
			break;
#else
			xt>>=bigint_unitbits;
#endif
#else
#error Check that your compiler does this correctly!
		//original code:
		xt>>=bigint_unitbits;
#endif
	}
}


BigInt::BigInt(unsigned int x) {
	memset(bits,0,bigint_bytes);
	UnitType *p=bits;
	while(x) {
		UnitType c=x&bigint_unitmax;
		*p++=c;
		x>>=bigint_unitbits;
	}
}

BigInt::BigInt(signed int x) {
	if(x<0)
		memset(bits,-1,bigint_bytes);
	else
		memset(bits,0,bigint_bytes);
	UnitType *p=bits;
	unsigned int xt=(unsigned int)(x);
	while(xt) {
		UnitType c=UnitType(xt)&UnitType(bigint_unitmax);
		*p++=c;
#ifdef __BORLANDC__
		// see description in BigInt(signed short)
#if bigint_unitbits==32
			break;
#else
			xt>>=bigint_unitbits;
#endif
#else
#error Check that your compiler does this correctly!
		xt>>=bigint_unitbits;
#endif
	}
}

BigInt::BigInt(unsigned long x) {
	memset(bits,0,bigint_bytes);
	UnitType *p=bits;
	while(x) {
		UnitType c=UnitType(x)&UnitType(bigint_unitmax);
		*p++=c;
		x>>=bigint_unitbits;
	}
}

BigInt::BigInt(signed long x) {
	if(x<0)
		memset(bits,-1,bigint_bytes);
	else
		memset(bits,0,bigint_bytes);
	UnitType *p=bits;
	unsigned long xt=(unsigned long)(x);
	while(xt) {
		UnitType c=UnitType(xt)&UnitType(bigint_unitmax);
		*p++=c;
#ifdef __BORLANDC__
		// see description in BigInt(signed short)
#if bigint_unitbits==32
			break;
#else
			xt>>=bigint_unitbits;
#endif
#else
#error Check that your compiler does this correctly!
		xt>>=bigint_unitbits;
#endif
	}
}

void BigInt::Zeroize() {
	memset(bits,0,bigint_bytes);
}

void BigInt::Oneize() {
	memset(bits,0,bigint_bytes);
	bits[0]=1;
}

void BigInt::OnesComplement() {
	UnitType *p=bits;
	for(int x=0;x<bigint_unitprecis;x++,p++)
		*p^=bigint_unitmax;
}

void BigInt::TwosComplement() {
	OnesComplement();
	incr();
}

BigInt::operator int() {
	int result;
	memcpy(&result,bits,min(sizeof(result),bigint_bytes));
	return result;
}

BigInt::operator long() {
	long result;
	memcpy(&result,bits,min(sizeof(result),bigint_bytes));
	return result;
}




BigInt abs(const BigInt& x) {
	BigInt result=x;
	if(result.IsNegative())
		result.TwosComplement();
	return result;
}

BigInt operator+(const BigInt& a,const BigInt& b) {
	BigInt result=a;
	return result+=b;
}

BigInt operator-(const BigInt& a,const BigInt& b) {
	BigInt result=a;
	result-=b;
	return result;
}


/* The following two function can on some cpu's be optimized heavily by using
 * some sort of BitBlitting/bit moving assembler instruction
 */
BigInt operator<<(const BigInt& a,unsigned int b) {
	BigInt result;
	if(b>=bigint_bitprecis)
		//total loss of precision
		result.Zeroize();
	else if(b==0)
		//no shift at all
		result=a;
	else if(b%CHAR_BIT==0) {
		//byte move - fast!
		unsigned byteshift=b%CHAR_BIT;
		result.Zeroize();
		memmove(result.bits+byteshift,a.bits,bigint_bytes-byteshift);
	} else {
		//hard one:
		const BigInt::UnitType	*from=a.bits;
		BigInt::UnitType	*to=result.bits;
		unsigned toi=0;
		//do whole units first
		while(b>=bigint_unitbits) {
			b-=bigint_unitbits;
			*to++=0;
			toi++;
		}

		BigInt::UnitType carrybits=0;
		while(toi<bigint_unitprecis) {
			BigInt::UnitType newcarrybits=(*from)>>(bigint_unitbits-b);
			*to=(*from<<b)|carrybits;
			to++;
			from++;
			toi++;
			carrybits=newcarrybits;
		}
	}
	return result;
}

BigInt operator>>(const BigInt& a,unsigned int b) {
	BigInt result;
	if(b>=bigint_bitprecis)
		//total loss of precision
		result.Zeroize();
	else if(b==0)
		//no shift at all
		result=a;
	else if(b%CHAR_BIT==0) {
		//byte move - fast!
		unsigned byteshift=b%CHAR_BIT;
		result.Zeroize();
		memmove(result.bits,a.bits+byteshift,bigint_bytes-byteshift);
	} else {
		//hard one:
		const BigInt::UnitType	*from=(a.bits)+(bigint_unitprecis-1);
		BigInt::UnitType	*to=(result.bits)+(bigint_unitprecis-1);
		int toi=bigint_unitprecis-1;
		//do whole units first
		while(b>=bigint_unitbits) {
			b-=bigint_unitbits;
			*to--=0;
			toi--;
		}

		BigInt::UnitType carrybits=0;	/* depends wether og not BigInt is signed or not*/
		while(toi>=0) {
			BigInt::UnitType newcarrybits=(*from);
			*to=(*from>>b)|(carrybits<<(bigint_unitbits-b));
			to--;
			from--;
			toi--;
			carrybits=newcarrybits;
		}
	}
	return result;
}


/* the following three functions can one some systems be optimized with a bit
 * block function
 */
BigInt operator|(const BigInt& a,const BigInt& b) {
	BigInt result=a;
	const BigInt::UnitType *ap=b.bits;
	const BigInt::UnitType *bp=b.bits;
	BigInt::UnitType *rp=result.bits;
	for(int x=0;x<bigint_unitprecis; ap++,bp++,rp++,x++)
		*rp=*ap|*bp;

	return result;
}

BigInt operator&(const BigInt& a,const BigInt& b) {
	BigInt result=a;
	const BigInt::UnitType *ap=b.bits;
	const BigInt::UnitType *bp=b.bits;
	BigInt::UnitType *rp=result.bits;
	for(int x=0;x<bigint_unitprecis; ap++,bp++,rp++,x++)
		*rp=*ap&*bp;

	return result;
}

BigInt operator^(const BigInt& a,const BigInt& b) {
	BigInt result=a;
	const BigInt::UnitType *ap=b.bits;
	const BigInt::UnitType *bp=b.bits;
	BigInt::UnitType *rp=result.bits;
	for(int x=0;x<bigint_unitprecis; ap++,bp++,rp++,x++)
		*rp=*ap^*bp;

	return result;
}


/* the following six/seven function can on some systems be heavily optimized by
 * using assembler.
 * eg: the Intel i80x86 family can compare memory block backwards
 * the memory comparsion is isolated in the function memcmprvs()
 */
static int memcmprvs(const void *s1, const void *s2, size_t n)
{
I	push	ds
I	mov	ax, n
I	mov     cx, ax
I	jcxz    mcm_end
I	lds	si, s1
I	les	di, s2
#if !(LDATA)
I	mov     ax, ds
I	mov     es, ax
#endif
I	add	si,n
I	dec	si
I	add	di,n
I	dec	di
I	std
I	rep     cmpsb

I	mov     al, [si+1]
I	xor     ah, ah
I	mov     cl, es:[di+1]
I	xor     ch, ch

mcm_end:
I	pop	ds

	return( _AX - _CX );
}

int operator==(const BigInt& a,const BigInt& b) {
	return memcmprvs(a.bits,b.bits,bigint_bytes)==0;
}
int operator!=(const BigInt& a,const BigInt& b) {
	return memcmprvs(a.bits,b.bits,bigint_bytes)!=0;
}
int operator<=(const BigInt& a,const BigInt& b) {
	return memcmprvs(a.bits,b.bits,bigint_bytes)<=0;
}
int operator>=(const BigInt& a,const BigInt& b) {
	return memcmprvs(a.bits,b.bits,bigint_bytes)>=0;
}
int operator<(const BigInt& a,const BigInt& b) {
	return memcmprvs(a.bits,b.bits,bigint_bytes)<0;
}
int operator>(const BigInt& a,const BigInt& b) {
	return memcmprvs(a.bits,b.bits,bigint_bytes)>0;
}



BigInt& BigInt::operator+=(const BigInt& a) {
	const UnitType *ap=a.bits;
	UnitType *bp=bits;
	UnitType carry=0;

	for(int x=0;x<bigint_unitprecis;x++,ap++,bp++) {
		UnitType result=*ap+*bp+carry;
		if(UnitType(*ap+*bp)<*ap) carry=1;
		else if(UnitType(*ap+*bp+carry)<*ap+*bp) carry=1;
		else carry=0;
		*bp=result;
	}
	return *this;
}

BigInt& BigInt::operator-=(const BigInt& a) {
	const UnitType *ap=a.bits;
	UnitType *tp=bits;
	UnitType carry=0;

	for(int x=0;x<bigint_unitprecis;x++,ap++,tp++) {
		UnitType result=*tp-*ap-carry;
		if(UnitType(*tp-*ap)>*tp) carry=1;
		else if(UnitType(*tp-*ap-carry)>*tp-*ap) carry=1;
		else carry=0;
		*tp=result;
	}
	return *this;
}

BigInt& BigInt::operator<<=(unsigned int a) {
	return (*this)=(*this)<<a;
}

BigInt& BigInt::operator>>=(unsigned int a) {
	return (*this)=(*this)>>a;
}

BigInt& BigInt::operator|=(const BigInt& a) {
	UnitType *tp=bits;
	const UnitType *ap=a.bits;
	for(int x=0;x<bigint_unitprecis;++x,*tp++|=*ap++);
	return *this;
}

BigInt& BigInt::operator&=(const BigInt& a) {
	UnitType *tp=bits;
	const UnitType *ap=a.bits;
	for(int x=0;x<bigint_unitprecis;++x,*tp++&=*ap++);
	return *this;
}

BigInt& BigInt::operator^=(const BigInt& a) {
	UnitType *tp=bits;
	const UnitType *ap=a.bits;
	for(int x=0;x<bigint_unitprecis;++x,*tp++^=*ap++);
	return *this;
}

BigInt BigInt::operator~() {
	BigInt result=*this;
	result.OnesComplement();
	return result;
}

/* inline'd
BigInt& BigInt::operator++() {
	incr();
	return *this;
}
*/
BigInt  BigInt::operator++(int) {
	BigInt result=*this;
	incr();
	return result;
}
/* inline'd too
BigInt& BigInt::operator--() {
	decr();
	return *this;
}
*/
BigInt  BigInt::operator--(int) {
	BigInt result=*this;
	decr();
	return result;
}

BigInt BigInt::operator-() {
	BigInt result=*this;
	result.TwosComplement();
	return result;
}


BigInt operator*(const BigInt& a,const BigInt& b) {
	BigInt result=a;
	result*=b;
	return result;
}


//time complexity: O(log(a div b))
void divmod(const BigInt& A,const BigInt& B,BigInt& q,BigInt& r) {
	if(B==BigInt(0)) return;	// #@%!

	q=0;
	r=A;
	BigInt b=B;
	BigInt k=0;

	while(r>=b) {
		b<<=1;
		++k;
	}
	while(b!=B) {
		q<<=1;
		b>>=1;
		--k;
		if(r>=b) {
			++q;
			r-=b;
		}
	}
}


//pre-condition: a>=0 && b>=0         <-----------------------------!!!!!
//time complexity: O(log B)
BigInt pow(const BigInt& A,const BigInt& B) {
	BigInt result=1,x=A,y=B;
	while(!y.IsZero()) {
		if(y.IsEven()) {
			x*=x;
			y>>=1;
		} else {
			result*=x;
			--y;
		}
	}
	return result;
}


BigInt operator/(const BigInt& a,const BigInt& b) {
	BigInt result=a;
	result/=b;
	return result;
}

BigInt operator%(const BigInt& a,const BigInt& b) {
	BigInt result=a;
	result%=b;
	return result;
}


//time complexity: O(log(a))
//could be optimized to: O(log(a min This))
BigInt& BigInt::operator*=(const BigInt& a) {
	BigInt adder=*this;
	BigInt b=a;
	Zeroize();
	while(!b.IsZero()) {
		if(b.IsOdd())
			(*this)+=adder;
		b>>=1;
		adder<<=1;
	}
	return *this;
}

BigInt& BigInt::operator/=(const BigInt& a) {
	BigInt tmp=*this;
	BigInt dummy;
	divmod(tmp,a,*this,dummy);
	return *this;
}

BigInt& BigInt::operator%=(const BigInt& a) {
	BigInt tmp=*this;
	BigInt dummy;
	divmod(tmp,a,dummy,*this);
	return *this;
}


inline char digit(int x) {
	if(x<10) return '0'+x;
	else return 'a'+x-10;
}

unsigned char *btoa(const BigInt& value,unsigned char *string, int radix) {
	BigInt q=value,r;
	if(!q.IsZero()) {
		BigInt bRadix=radix;	//avoid more than one conversion int->BigInt
		unsigned char *p1=string;
		while(!q.IsZero()) {
			BigInt tmp=q;
			divmod(tmp,bRadix,q,r);
			*p1++=digit(r);
		}
		*p1--='\0';

		//reverse the string
		unsigned char *p2=string;
		while(p1>p2) {
			char tmp=*p1;
			*p1=*p2;
			*p2=tmp;
			--p1;
			++p2;
		}
		return string;
	} else {
		unsigned char *p=string;
		*p++='0';
		*p='\0';
		return string;
	}
}

BigInt atob(const unsigned char *s) {
	BigInt result=0;
	int isNegative=0;
	while(isspace(*s)) ++s;

	if(*s=='-') {
		isNegative=1;
		++s;
	}

	while(isdigit(*s)) {
		result*=10;
		result+=(*s-'0');
		++s;
	}
	if(isNegative) result=-result;

	return result;
}


ostream& operator<<(ostream& os,const BigInt& b) {
	char *cp=new char[bigint_bytes*3];
	btoa(b,cp,10);
	os << cp;
	delete[] cp;
	return os;
}

istream& operator>>(istream& is,BigInt& b) {
	b=BigInt(0);
	char c;
	while(is>>c && c>='0' && c<='9') {
		b*=10;
		b+=c-'0';
	}
	is.putback(c);
	return is;
}


BigInt sqrt(const BigInt& a) {
	BigInt x=0,y=a;
	while(x+BigInt(1)!=y) {
		BigInt h=(x+y)>>1;
		BigInt sqr=h*h;
		if(sqr<=a)
			x=h;
		else
			y=h;
	}
	return x;
}

/*
	BigInt low=0,high=a+1;
	while(low+1<high) {
		BigInt center=(low+high)>>1;
		BigInt centersqr=center*center;
		if(centersqr<a)
			low=center;
		else if(centersqr>a)
}
*/