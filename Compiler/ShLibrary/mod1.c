/* mod1.c */

#include <stdio.h>
#include <unistd.h>
int test1[250000] = { 1, 2, 3 };

#ifndef VERSION
#define VERSION "x1 . 1"
#endif

void 
x1(void) { 
    printf("Callee mod1.c:x1 \n"); 
    printf("Caller mod1.c:x1\n"); 
    printf("Calling x2\n"); 
    x2();
    printf("exiting mod1.c:x1\n");
} 
