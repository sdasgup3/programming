#include<mpi.h>
#include<stdio.h>


int main(int argc, char* argv[])
{
    int rc, numsize, id, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    rc  = MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &numsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Get_processor_name(hostname, &len);


    printf("Hello WOrld from task %d on %s\n", id,hostname);	
    if(id ==0) {
        printf("Master: Number of MPI Task is %d ", numsize);
    }

    MPI_Finalize();

}
