#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <assert.h>

int main(int argc, char* argv[]) {

    double val[6]  ;
    double result[6]  ;
    int numprocessors, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocessors);

    if(rank==0)  {
        val[0]=1.0; val[1]=2.0; val[2]=3.0; val[3]=4.0; val[4]=5.0; val[5]=6.0;
    }
    if(rank==1)  {
        val[0]=11.0; val[1]=12.0; val[2]=13.0; val[3]=14.0; val[4]=15.0; val[5]=16.0;
    }
    if(rank==2)  {
        val[0]=21.0; val[1]=22.0; val[2]=23.0; val[3]=24.0; val[4]=25.0; val[5]=26.0;
    }
    if(rank==3)  {
        val[0]=31.0; val[1]=32.0; val[2]=33.0; val[3]=34.0; val[4]=35.0; val[5]=36.0;
    }
    if(rank==4)  {
        val[0]=41.0; val[1]=42.0; val[2]=43.0; val[3]=44.0; val[4]=45.0; val[5]=46.0;
    }
    if(rank==5)  {
        val[0]=51.0; val[1]=52.0; val[2]=53.0; val[3]=54.0; val[4]=55.0; val[5]=56.0;
    }


    printf("%lf %lf %lf %lf %lf %lf\n", val[0], val[1], val[2], val[3], val[4], val[5]);
    
    MPI_Allgather(&val,1,MPI_DOUBLE,&result,1,MPI_DOUBLE,MPI_COMM_WORLD);
    printf("\n===============================\n", val[0], val[1], val[2], val[3], val[4], val[5]);

    sleep(1);
    printf("%lf %lf %lf %lf %lf %lf\n", result[0], result[1], result[2], result[3], result[4], result[5]);

    MPI_Finalize();

    return 0;
}
