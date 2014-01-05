// ompingle.cpp
// compile with: /openmp 
#include <stdio.h>
#include <omp.h>

int main() {
#pragma omp parallel num_threads(2)
  {
#pragma omp single
    {
      // Only a single thread can read the input.
      printf("read input\n");
    }

    // Multiple threads in the team compute the results.
    printf("compute results\n");

#pragma omp barrier

#pragma omp single
    {
      // Only a single thread can write the output.
      printf("write output\n");
    }
  }
}
