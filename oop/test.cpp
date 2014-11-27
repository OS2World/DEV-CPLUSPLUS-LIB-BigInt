// test of BigInt library. You can use this program to ensure that your
// modifications work correctly

#include <iostream.h>
#include "bigint.hpp"

#define nl << "\n";

#ifdef __BORLANDC__
#pragma argsused
#endif
int main(int ,char **) {
        cout << "BigInt test program" nl;

        {
                cout << "Test 1: constructors & type conversion: ";
                BigInt a=int(1),b=2L;
                if(int(a)!=1 || int(b)!=2L) {
                        cout << "Fail!" nl;
                        return 1;
                } else
                        cout << "Passed." nl;
        }

        {
                cout << "Test 2: complement functions          : ";
                BigInt a=2;
                a.TwosComplement();
                a.TwosComplement();

                BigInt b=27;
                b.TwosComplement();
                b.OnesComplement();
                b.incr();

                if(int(a)!=2 || int(b)!=27) {
                        cout << "Fail!" nl;
                        return 2;
                } else
                        cout << "Passed." nl;
        }

        {
                cout << "Test 3: +, -, ++ and --               : ";
                BigInt a=2,b=1;
                ++a;
                a-=1;
                --b;
                --b;
                b+=2;
                b+=120000L;
                b-=120000L;

                if(int(a)!=2 || int(b)!=1) {
                        cout << "Fail!" nl;
                        return 3;
                } else
                        cout << "Passed." nl;
        }

        {
                cout << "Test 4: bit shifts                    : ";
                BigInt a=1,b=32;
                a<<=18;
                a>>=18;
                b>>=5;
                b<<=5;
                if(int(a)!=1 || int(b)!=32) {
                        cout << "Fail!" nl;
                        return 4;
                } else
                        cout << "Passed." nl;
        }

        {
                cout << "Test 5: multiplication & division     : ";
                BigInt a=1,b=32,c=29;
                a*=5;
                a+=10;
                a/=5;
                a-=2;

                b/=8;
                b+=1;
                b*=8;
                b-=8;

                c%=9;
                if(int(a)!=1 || int(b)!=32 || int(c)!=2) {
                        cout << "Fail!" nl;
                        return 5;
                } else
                        cout << "Passed." nl;
        }

        {
                cout << "Test 6: comparison                    : ";
                if(BigInt(1)!=BigInt(1) ||
                   BigInt(1)>=BigInt(2) ||
                   BigInt(1)<=BigInt(0) ||
                   BigInt(1)==BigInt(2) ||
                   BigInt(1)< BigInt(1) ||
                   BigInt(1)> BigInt(2)) {
                        cout << "Fail!" nl;
                        return 5;
                } else
                        cout << "Passed." nl;
        }

        cout << "All test passed" nl;

        return 0;
}
