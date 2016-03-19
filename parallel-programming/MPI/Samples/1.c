#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <assert.h>


void produce_data(double a[]) {
   a[0] = 1;
}
void consume_data(double a[]) {
   printf("%lf ", a[0]);
}

int main(int argc, char* argv[]) {

  double a[16000];
  int i = 1;
  int numprocessors, rank;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&numprocessors);


  if(0  == rank) {
    for (i=0; i<1000; i++) {
      produce_data(a);
      MPI_Send (&a,16000,MPI_DOUBLE,1,0,MPI_COMM_WORLD);
    }
  }

  if(1  == rank) {
    for (i=0; i<1000; i++) {
      MPI_Recv(&a,16000,MPI_DOUBLE,0,0, MPI_COMM_WORLD, &status);  
      consume_data(a);
    }
  }

  MPI_Finalize();

  return 0;
}
