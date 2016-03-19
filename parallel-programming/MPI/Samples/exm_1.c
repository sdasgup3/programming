#include<mpi.h>
#include<stdio.h>

int main(int argc, char** argv) 
{
  int p, rank, received, myvalue, step;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  myvalue  = rank;
  
  for(step = 1 ; step < p ; step *= 2) {
    if(rank % step == 0) {
      if(0 == ((rank/ step) % 2) ) {
        MPI_Recv(&received, 1, MPI_INT, rank + step, 0, MPI_COMM_WORLD, &status);
        printf("%d is receiving from %d+%d\n", rank, rank,step);
        myvalue += received;
      } else  {
        MPI_Send(&myvalue, 1, MPI_INT, rank - step, 0, MPI_COMM_WORLD);
        printf("%d is sending to %d-%d\n", rank, rank,step);
      }
    } else {
      break;
    }
  }
  if(0 == rank) {
    printf("Rank 0 value : %d\n\n", myvalue);
  }
  MPI_Finalize();
}
