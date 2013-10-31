#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <assert.h>

void *func(void *number, MPI_Datatype datatype, MPI_Comm comm) {   

  int rank, comm_size;   

  MPI_Comm_rank(comm, &rank);   
  MPI_Comm_size(comm, &comm_size);  

  int datatype_size;   
  MPI_Type_size(datatype, &datatype_size);   

  void *numbers = NULL;   

  if (rank == 0)
     numbers = malloc(datatype_size * comm_size);

  MPI_Gather(number, 1, datatype, numbers, 1, datatype, 0, comm);
  return numbers; 

}

int main(int argc, char* argv[]) {

  double val ;
  int numprocessors, rank;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&numprocessors);

  if(rank==0) val=4.0;
  if(rank==1) val=6.0;
  if(rank==2) val=10.0;
  if(rank==3) val=12.0;

  double *numbers = func(&val, MPI_DOUBLE, MPI_COMM_WORLD);
  if(0 == rank) {
     printf("numbers[0] = %lf\n", numbers[0]);
     printf("numbers[1] = %lf\n", numbers[1]);
     printf("numbers[2] = %lf\n", numbers[2]);
     printf("numbers[3] = %lf\n", numbers[3]);
  }

  MPI_Finalize();

  return 0;
}
