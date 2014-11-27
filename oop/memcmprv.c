// BC++ 3.1 intel 386 speedup for BigInt
#include <stddef.h>

#define I asm

#if defined(__COMPACT__) || defined(__LARGE_) || defined(__HUGE__)
#define LDATA 1
#else
#define LDATA 0
#endif

int memcmprvs(const void *s1, const void *s2, size_t n)
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


void main(void) {
	int i=memcmprvs("3111","1111",4);
	if(i) i++;
}