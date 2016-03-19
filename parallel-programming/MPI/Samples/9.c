#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <assert.h>

int main(int argc, char* argv[]) {

   int size, rank;
    double inval, outval=4.0;
    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
            MPI_Send(&outval,1,MPI_DOUBLE,rank+1,rank,MPI_COMM_WORLD);
            MPI_Recv(&inval,1,MPI_DOUBLE,rank+1,rank,MPI_COMM_WORLD,&status);
    }  else if (rank == 1) {
            MPI_Recv(&inval,1,MPI_DOUBLE,rank-1,rank,MPI_COMM_WORLD,&status);
            MPI_Send(&outval,1,MPI_DOUBLE,rank-1,rank,MPI_COMM_WORLD);
    }

    MPI_Finalize();


    return 0;
}
