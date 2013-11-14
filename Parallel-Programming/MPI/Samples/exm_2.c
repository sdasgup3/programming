#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#define SEED 100

int main(int argc, char** argv) 
{
  int p, rank, received, myvalue, i;
  MPI_Status status;
  int *elmnts, *final_elmnts;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  elmnts = (int *) malloc(sizeof(int) * p);
  final_elmnts = (int *)malloc(sizeof(int) * p);
  if(p-1 == rank) {

    srand(SEED);
    for(i = 0 ; i < p ; i ++ ) {
      elmnts[i] = rand()%p;
      printf ("%d ", elmnts[i]);
    }
    printf ("\n\n");
  }

  MPI_Scatter(elmnts, 1 , MPI_INT, &received, 1, MPI_INT,  p-1, MPI_COMM_WORLD);

  if(received > rank) {
    myvalue = 1;
  } else {
    myvalue = 0;
  }
  MPI_Gather(&myvalue, 1, MPI_INT, final_elmnts, 1, MPI_INT, 0, MPI_COMM_WORLD );

  if(0 == rank) {
    for(i = 0 ; i < p ; i ++ ) {
      printf ("%d ", final_elmnts[i]);
    }
    printf ("\n\n");
  }

    free(elmnts);
    free(final_elmnts);

  MPI_Finalize();
}
