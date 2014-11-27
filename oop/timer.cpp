#include "bigint.hpp"
#include <time.h>
#include <iomanip.h>

template<class T> void addtest(T limit) {
        T r=0;
        for(T x=0; x<limit; ++x, r+=x);
}

template<class T> void multest(T limit) {
        T r=0;
        for(T x=0; x<limit; ++x, r*=x);
}

template<class T> void divtest(T limit) {
        T r=0;
        for(T x=1; x<limit; ++x, r=(limit-x)/x);
}

clock_t starttime;
void starttimer(void) {
        starttime=clock();
}

void stoptimer(const char *name) {
        clock_t totaltime=clock()-starttime;
        cout << "time for " << name << ":"
             << setprecision(2) << setw(5) << totaltime/CLK_TCK << endl;
}

void main(void) {
        const limit=100000;
        starttimer();
        addtest(long(limit));
        stoptimer("adding/long");

        starttimer();
        addtest(BigInt(limit));
        stoptimer("adding/bigint");

        starttimer();
        multest(long(limit));
        stoptimer("multiplying/long");

        starttimer();
        multest(BigInt(limit));
        stoptimer("multiplying/bigint");

        starttimer();
        divtest(long(limit));
        stoptimer("dividing/long");

        starttimer();
        divtest(BigInt(limit));
        stoptimer("dividing/bigint");
}
