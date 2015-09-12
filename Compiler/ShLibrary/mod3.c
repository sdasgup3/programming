/* mod3.c */

#include <stdio.h>
#include <unistd.h>
int test3[10000] = { 1, 2, 3 };

#ifndef VERSION
#define VERSION ""
#endif

void 
x3(void) { 
    printf("Called mod3-x3 " VERSION "\n"); 
    printf("exiting x3\n");
} 
