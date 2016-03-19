#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <sys/time.h>
#include <limits.h>
#include <stdbool.h>
#include <mpi.h>
#define SEED 100
#define OUTPUT 0
#define CHECK 1
//Sequential sampleSort.  
//Assume size is a multiple of nbuckets*nbuckets

void* mymalloc(size_t alloc_size) 
{
  void* mem = malloc(alloc_size);
  if(NULL == mem) {
    printf("Cannot allocate memory.. Exiting");
    MPI_Finalize();
    exit(0);
  }
  return mem; 
}

int main(int argc, char *argv[]) {

    int *arr, *net_arr; 
    int size, rank, i;


    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        

    arr = (int*)mymalloc(sizeof(int )*10);
    net_arr = (int*)mymalloc(sizeof(int )*10);

    for(i = 0 ; i < 10 ; i ++) {
      arr[i] = (rank + 1)* (i+1);
    }
    for(i = 0 ; i < 10 ; i ++) {
      printf("%d ", arr[i]);
    }
    printf("\n");
    
    MPI_Reduce(arr, net_arr, 10, MPI_INT, MPI_SUM, 0 , MPI_COMM_WORLD);

    if(0 == rank)
      for(i = 0 ; i < 10 ; i ++) {
       printf("%d ", net_arr[i]);
      }

    printf("\n");


    MPI_Finalize();
    return 0;
}

