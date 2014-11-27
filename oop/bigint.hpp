#ifndef __BIGINT_HPP
#define __BIGINT_HPP

#include <iostream.h>

const unsigned bigint_unitbits=        32;        //bits per unit
const unsigned long bigint_unitmax= 4294967295UL;
const unsigned bigint_unitprecis=2;        //units per BigInt
const unsigned bigint_bitprecis=
        bigint_unitbits*bigint_unitprecis;        // total bits in BigInt
const unsigned bigint_bytes=bigint_unitbits/sizeof(unsigned long);

class BigInt {
        typedef unsigned long UnitType;
        UnitType        bits[bigint_unitprecis];
public:
        int IsNegative() const;
        int IsZero() const;
        int IsOdd() const;
        int IsEven() const { return !IsOdd(); }
        void incr();
        void decr();

        BigInt() {}
        BigInt(unsigned char x);
        BigInt(signed char x);
        BigInt(unsigned short x);
        BigInt(signed short x);
        BigInt(unsigned int x);
        BigInt(signed int x);
        BigInt(unsigned long x);
        BigInt(signed long x);
        BigInt(const char* x);

        void Zeroize();
        void Oneize();
        void OnesComplement();
        void TwosComplement();

        operator int();
        operator long();

        friend BigInt abs(const BigInt &x);
        friend void divmod(const BigInt &A, const BigInt &B, BigInt &q, BigInt &r);
        friend BigInt pow(const BigInt &A,const BigInt &b);
        friend BigInt operator+(const BigInt &a, const BigInt &b);
        friend BigInt operator-(const BigInt &a, const BigInt &b);
        friend BigInt operator*(const BigInt &a, const BigInt &b);
        friend BigInt operator/(const BigInt &a, const BigInt &b);
        friend BigInt operator%(const BigInt &a, const BigInt &b);
        friend BigInt operator<<(const BigInt &a, unsigned int b);
        friend BigInt operator>>(const BigInt &a, unsigned int b);
        friend BigInt operator|(const BigInt &a, const BigInt &b);
        friend BigInt operator&(const BigInt &a, const BigInt &b);
        friend BigInt operator^(const BigInt &a, const BigInt &b);
        friend int operator==(const BigInt &a, const BigInt &b);
        friend int operator!=(const BigInt &a, const BigInt &b);
        friend int operator<=(const BigInt &a, const BigInt &b);
        friend int operator>=(const BigInt &a, const BigInt &b);
        friend int operator<(const BigInt &a, const BigInt &b);
        friend int operator>(const BigInt &a, const BigInt &b);
        BigInt& operator+=(const BigInt &a);
        BigInt& operator-=(const BigInt &a);
        BigInt& operator*=(const BigInt &a);
        BigInt& operator/=(const BigInt &a);
        BigInt& operator%=(const BigInt &a);
        BigInt& operator<<=(unsigned int a);
        BigInt& operator>>=(unsigned int a);
        BigInt& operator|=(const BigInt &a);
        BigInt& operator&=(const BigInt &a);
        BigInt& operator^=(const BigInt &a);
        BigInt  operator~();
        BigInt& operator++() { incr(); return *this; }
        BigInt  operator++(int);
        BigInt& operator--() { decr(); return *this; };
        BigInt  operator--(int);
        BigInt  operator+() { return *this; };
        BigInt  operator-();
};

//string stuff
unsigned char *btoa(const BigInt &value, unsigned char *string, int radix);
inline signed char *btoa(const BigInt &value, signed char *string, int radix)
{
        return (signed char*)btoa(value,(unsigned char*)string,radix);
}

BigInt atob(const unsigned char *s);
inline BigInt atob(const signed char *s) {
        return atob((const unsigned char*)s);
}

//streaming
ostream& operator<<(ostream&, const BigInt&);
istream& operator>>(istream&, BigInt&);

//other stuff
BigInt sqrt(const BigInt& a);
#endif
