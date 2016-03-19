//    If a thread is currently executing inside a CRITICAL region and another
//    thread reaches that CRITICAL region and attempts to execute it, it will
//    block until the first thread exits that CRITICAL region.
//
//    The optional name enables multiple different CRITICAL regions to exist:
//    Names act as global identifiers. Different CRITICAL regions with the same
//    name are treated as the same region.  All CRITICAL sections which are
//    unnamed, are treated as the same section.  It is illegal to branch into or
//    out of a CRITICAL block.
#include <stdio.h> 
#include <omp.h>


int main( ) 
{
    int i;
    
    #pragma omp parallel
   {
    
    #pragma omp critical (A)
    {
	printf("In critical");
    	#pragma omp critical (B)
    	{
	}
   }
    }
}
