/*#* prog.c */

#include <stdio.h> 
#include <stdlib.h> 

void x1(void);
void x2(void);

int
main(int argc, char *argv[])
{
    printf("Caller prog.c:main\n"); 
    printf("Calling x1\n"); 
    x1();

    //printf("Caller prog.c:main\n"); 
    //printf("Calling x2\n"); 
    //x2();
    printf("exiting main.c:main\n");
    exit(EXIT_SUCCESS);
} /* main */
