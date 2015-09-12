/* mod1.c */

#include <stdio.h>
#include <unistd.h>
int test1[250000] = { 1, 2, 3 };

#ifndef VERSION
#define VERSION ""
#endif

void 
x1(void) { 
    printf("Called mod1-x1 " VERSION "\n"); 
    printf("exiting x1\n");
} 
