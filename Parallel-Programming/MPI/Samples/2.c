#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <assert.h>

int main(int argc, char* argv[]) {

    int npes, myrank;  
    int a[10],b[10];

    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&npes);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    
    if(myrank%2==1) {
      printf("%d is sending to %d\n",myrank, (myrank+1)%npes);                       
      MPI_Send(a, 10, MPI_INT, (myrank+1)%npes, 1, MPI_COMM_WORLD);

      printf("%d is receiving from %d\n",myrank, (myrank-1+npes)%npes);                       
      MPI_Recv(b, 10, MPI_INT, (myrank-1+npes)%npes, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
      printf("%d is receiving from %d\n", myrank, (myrank-1+npes)%npes);                       
      MPI_Recv(b, 10, MPI_INT, (myrank-1+npes)%npes, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      printf("%d is sending to %d\n", myrank, (myrank+1)%npes);                       
      MPI_Send(a, 10, MPI_INT, (myrank+1)%npes, 1, MPI_COMM_WORLD);
    }



                        


    MPI_Finalize();

    return 0;
}
