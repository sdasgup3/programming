/* mod1.c */

#include <stdio.h>

extern void x2();
void 
x1(void) { 
    printf("Callee mod1.c:x1 \n"); 
    printf("Caller mod1.c:x1\n"); 
    printf("Calling x2\n"); 
    x2();
    printf("exiting mod1.c:x1\n");
} 
