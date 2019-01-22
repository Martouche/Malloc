#include "include/malloc.h"

int main()
{
	void *p;
	p = my_malloc(5);
    printf("\n p : 0x%p \n",p);
}