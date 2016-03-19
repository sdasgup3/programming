#include <stdio.h>
#include <stdlib.h>
#define	ARRAYSIZE	20

int main(int argc, char *argv[])
{
int     i; 			/* loop variable */
float	data[ARRAYSIZE]; 	/* the intial array */

/* Initialize the array */
for(i=0; i<ARRAYSIZE; i++) 
  data[i] =  i * 1.0;

/* Do a simple value assignment to each of the array elements */
for(i=0; i < ARRAYSIZE; i++)
   data[i] = 2*data[i] ;

/* Print a few sample results */
for(i=0;i<ARRAYSIZE; i++)
    printf("%f ", data[i]);
printf("\n ");
}
