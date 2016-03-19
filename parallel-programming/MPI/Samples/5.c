#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <assert.h>



void func(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm comm)
{  

        int rank, size, i;  

        MPI_Comm_rank(comm, &rank);  
        MPI_Comm_size(comm, &size);

        printf("Rankk %d\n", rank);
    
        if (rank == root) {    
          for (i = 0; i < size; i++) {      
             if (i != rank)       
               MPI_Send(data, count, datatype, i, 0, comm);      
          }  
        } else {    
               MPI_Recv(data, count, datatype, root, 0, comm,              MPI_STATUS_IGNORE);  
        }
}

int main(int argc, char* argv[]) {

    double a[16000];
    double val ;
    int numprocessors, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocessors);

    if(rank==0) val=4.0;
    if(rank==1) val=6.0;
    if(rank==2) val=10.0;
    if(rank==3) val=12.0;

    func(&val, 1, MPI_DOUBLE, 2, MPI_COMM_WORLD);

    printf("rank = %d val = %lf\n", rank, val);

    MPI_Finalize();

    return 0;
}
