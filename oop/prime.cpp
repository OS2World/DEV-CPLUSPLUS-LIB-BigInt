// Example program for BigInt class : Prime generator
#include <iostream.h>
#include "bigint.hpp"
#include <conio.h>

struct ListElem {
	BigInt value;
	struct ListElem *next;
};

#ifdef __BORLANDC__
#pragma argsused
#endif
int main(int argc,char *argv[]) {
	ListElem *list;
	BigInt current;

	list=new ListElem;
	list->value=2;
	list->next=NULL;
	current=3;


	while(!kbhit()) {
		ListElem *p=list;
		int IsPrime=1;
		BigInt sqrtCurrent=sqrt(current);
		while(IsPrime && p && p->value<=sqrtCurrent) {
			BigInt r=current%p->value;
			if(r.IsZero())
				IsPrime=0;
			else
				p=p->next;
		}
		if(IsPrime) {
			ListElem *p1=list,*pp=NULL;
			while(p1) {
				pp=p1;
				p1=p1->next;
			}
			ListElem *n=new ListElem;
			n->value=current;
			n->next=NULL;
			pp->next=n;

			cout << current << "\n";
		}
		++current;
	}
	return 0;
}
