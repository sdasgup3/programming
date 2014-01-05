#include<stdio.h>
#include<mpi.h>
#define SEED 100


int main (int argc, char ** argv) 
{
  int size, rank, gap;
  int value;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  srand(SEED * rank);
  value = rand() % 1000;

  printf ("rank %d value %d\n", rank, value);

  int gap = 1;

  for(i = 0 ; i < size - 1 ; i ++) {


  


  MPI_Finalize();
}

