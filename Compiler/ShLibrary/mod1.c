/* mod1.c */

#include <stdio.h>
#include <unistd.h>
int test1[250000] = { 1, 2, 3 };

#ifndef VERSION
#define VERSION "x1 . 1"
#endif

void 
x1(void) { 
    printf("Called mod1.c:x1 \n"); 
    printf("exiting mod1.c:x1\n");
} 
