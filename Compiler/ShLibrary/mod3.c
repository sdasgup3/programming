/* mod3.c */

#include <stdio.h>
#include <unistd.h>
int test3[10000] = { 1, 2, 3 };

#ifndef VERSION
#define VERSION "x1 . 2"
#endif

void 
x1(void) { 
    printf("Called mod3.c:x1 \n"); 
    printf("exiting mod3.c:x1\n");
} 
