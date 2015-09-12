/* mod2.c */

#include <stdio.h>
#include <unistd.h>
int test2[100000] = { 1, 2, 3 };

#ifndef VERSION
#define VERSION ""
#endif

void 
x2(void) { 
    printf("Called mod2-x2 " VERSION "\n"); 
    printf("exiting x2\n");
} 
