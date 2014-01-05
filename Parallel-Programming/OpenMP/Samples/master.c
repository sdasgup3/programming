//The MASTER directive specifies a region that is to be executed only by the
//master thread of the team. All other threads on the team skip this section of
//code There is no implied barrier associated with this directive 
//It is illegal to branch into or out of MASTER block. 

#include <omp.h>
#include <stdio.h>

int main( ) 
{
    int a[5], i;

    #pragma omp parallel
    {
        // Perform some computation.
        #pragma omp for
        for (i = 0; i < 5; i++)
            a[i] = i * i;

        // Print intermediate results.
        #pragma omp master
            for (i = 0; i < 5; i++)
                printf("a[%d] = %d\n", i, a[i]);
      
        // Wait.
        #pragma omp barrier

        // Continue with the computation.
        #pragma omp for
        for (i = 0; i < 5; i++)
            a[i] += i;
    }
}
