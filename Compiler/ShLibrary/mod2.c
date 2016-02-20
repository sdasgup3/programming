/* mod2.c */

#include <stdio.h>
#include <unistd.h>
int test2[100000] = { 1, 2, 3 };

#ifndef VERSION
#define VERSION ""
#endif

void 
x2(void) { 
    printf("Called mod2.c:x2 \n"); 
    printf("exiting mod2.c:x2\n");
} 
