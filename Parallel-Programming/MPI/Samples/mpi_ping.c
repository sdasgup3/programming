#include<mpi.h>
#include<stdio.h>


int main(int argc, char* argv[])
{
    int rc,  numsize, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    char out_mssg , in_mssg = 'i';
    int dest, tag, src;
    int tag1, tag2;
    MPI_Status stat;

    rc  = MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &numsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(hostname, &len);


    if(0 == rank) {
        dest = 1;
        src = 1;
        tag1 = 0;
        tag2 = 1;
        out_mssg = '0';
        MPI_Send(&out_mssg, 1, MPI_CHAR, dest, tag1, MPI_COMM_WORLD);
        MPI_Recv(&in_mssg, 1, MPI_CHAR, src, tag2, MPI_COMM_WORLD, &stat);
    }

    if(1 == rank) {
        dest = 0;
        src = 0;
        tag1 = 0;
        tag2 = 1;
        out_mssg = '1';
        MPI_Send(&out_mssg, 1, MPI_CHAR, dest, tag2, MPI_COMM_WORLD);
        MPI_Recv(&in_mssg, 1, MPI_CHAR, src, tag1, MPI_COMM_WORLD,&stat);
    }

    int count;
    rc = MPI_Get_count(&stat, MPI_CHAR, &count);
    printf("Task %d: Received %d char(s) [%c] from task %d with tag %d \n", rank, count,in_mssg, stat.MPI_SOURCE, stat.MPI_TAG);
   
    MPI_Finalize();
}
