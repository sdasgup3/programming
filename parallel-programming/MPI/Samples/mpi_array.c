#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define  ARRAYSIZE 20
#define  MASTER	0

float  data[ARRAYSIZE];

int main (int argc, char *argv[])
{
  int   numtasks, taskid, rc, dest, offset, i, j, tag1,
      tag2, source, chunksize; 
  float mysum, sum;
  float update(int myoffset, int chunk, int myid);
  MPI_Status status;

  /***** Initializations *****/
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  if (numtasks != 4) {
   printf("Quitting. Number of MPI tasks must be  4.\n");
   MPI_Abort(MPI_COMM_WORLD, rc);
   exit(0);
  }

  MPI_Comm_rank(MPI_COMM_WORLD,&taskid);

  chunksize = (ARRAYSIZE / numtasks);
  offset = chunksize;
  tag2 = 1;
  tag1 = 2;

  if(MASTER == taskid) {
    for(i=0; i<ARRAYSIZE; i++) {
      data[i] =  i ;
    }
  
    for(dest = 1 ; dest <= 3; dest++) {
       MPI_Send(&offset      ,         1,   MPI_INT, dest, tag1, MPI_COMM_WORLD);
       MPI_Send(&data[offset], chunksize, MPI_FLOAT, dest, tag2, MPI_COMM_WORLD);
      offset += chunksize;
    }

    offset = 0;
    update(offset, chunksize, taskid);

    offset = chunksize;
    for(source = 1 ; source <= 3; source++) {
      MPI_Recv(&data[offset], chunksize, MPI_FLOAT, source, tag2, MPI_COMM_WORLD, &status);
       offset += chunksize;
    }

for(i=0;i<ARRAYSIZE; i++)
    printf("%f ", data[i]);
printf("\n ");

  } 

  if (taskid > MASTER) {

    source = MASTER;
    MPI_Recv(&offset,               1,   MPI_INT, source, tag1, MPI_COMM_WORLD, &status);
    MPI_Recv(&data[offset], chunksize, MPI_FLOAT, source, tag2, MPI_COMM_WORLD, &status);

    update(offset, chunksize, taskid);

    dest = MASTER;
    MPI_Send(&data[offset], chunksize, MPI_FLOAT, MASTER, tag2, MPI_COMM_WORLD);
  } /* end of non-master */


MPI_Finalize();

}   /* end of main */


float update(int myoffset, int chunk, int myid) {
  int i; 
  for(i=myoffset; i < myoffset + chunk; i++) {
    data[i] = 2*data[i];
    }
}
