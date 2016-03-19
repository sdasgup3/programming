#include <mpi.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#define SEED 100

int main(int argc, char *argv[]) {//Implementation of prefix sum using point-to-point comm
    int size, local_size, rank, p, distance, i, value, x;
    int *elmnts, *local_elmnts;
    int src, dest;
    MPI_Status status;

        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //value = i;

    if(0 == rank) {
      value = 1;
    }
    if(1 == rank) {
      value = 2;
    }
    if(2 == rank) {
      value = 3;
    }
    if(3 == rank) {
      value = 4;
    }
    if(4 == rank) {
      value = 5;
    }


    distance = 1;
    while(distance < size) {
        dest = rank + distance;
        if(dest < size) {
            MPI_Send(&value,1,MPI_DOUBLE, dest,0,MPI_COMM_WORLD);
            printf("%d sending to %d (%d %d)\n",rank, rank+distance, rank, distance);
        }
        src = rank - distance;
        if(src >= 0) {
            MPI_Recv(&x,1,MPI_DOUBLE, src,0,MPI_COMM_WORLD,&status);
            value+=x; 
        }
        distance = distance*2;
    printf("Distance %d \n",distance);
        //exit(0);
    }

    MPI_Finalize();

}
