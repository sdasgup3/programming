#include<stdio.h>
#include<stdlib.h>
#include<values.h>
#include<mpi.h>
#define N 50
#define SEED 100

int compare(const void *num1, const void *num2) {
    int* n1 = (int*)num1;
    int* n2 = (int*)num2;
    return (*n1 > *n2) - (*n1 < *n2);
}


int main(int argc, char** argv )
{
  int i, j, nlocal, npes, myrank;
  int *sorted_elmnts, *splitters, *allpicks, *elmnts, *master_elmnts;
  int *scounts, *sdispls, *rcounts, *rdispls;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &npes);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  if(10 != npes) {
    exit(0);
    MPI_Finalize();
  }

  nlocal = N/npes;

  if(0 == myrank) {
    master_elmnts = (int*)malloc(sizeof(int)*N);
    srand(SEED);
    for(i=0;i<N;i++) {
      master_elmnts[i] = rand()%100;
    }
  }

  /* Allocate memory for the arrays that will store the splitters */
  elmnts = (int *)malloc(nlocal*sizeof(int));
  splitters = (int *)malloc(npes*sizeof(int));
  allpicks = (int *)malloc(npes*(npes-1)*sizeof(int));

  MPI_Scatter(master_elmnts, nlocal, MPI_INT, elmnts, 10 , MPI_INT, 0, MPI_COMM_WORLD);



  /* Sort local array */
  qsort(elmnts, nlocal, sizeof(int), compare);


  for(i=0; i < nlocal; i++) {
    printf ("%d ", elmnts[i]);
  }
  printf ("\n");
  /* Select local npes-1 equally spaced elements */
  for (i=1; i<npes; i++)
    splitters[i-1] = elmnts[i*nlocal/npes];
  
  /* Gather the samples in the processors */
  MPI_Allgather(splitters, npes -1 , MPI_INT, allpicks, npes -1 , MPI_INT, MPI_COMM_WORLD);

  /* sort these samples */
  qsort(allpicks, npes*(npes-1), sizeof(int), compare);

  /* Select splitters */
  for (i=1; i<npes; i++)
    splitters[i-1] = allpicks[i*npes];
  splitters[npes-1] = MAXINT;

  printf ("Splitter\n");
  for (i=0; i<npes; i++) {
    printf ("%d ", splitters[i] );
  }
  printf ("\n");

  
  /*
scounts = (int *)malloc(npes*sizeof(int));
for (i=0; i<npes; i++)
scounts[i] = 0;

41
for (j=i=0; i<nlocal; i++) {
42
if (elmnts[i] < splitters[j])
43
scounts[j]++;
44
else
45
scounts[++j]++;
46
}
47
48
49
sdispls = (int *)malloc(npes*sizeof(int));
50
sdispls[0] = 0;
51
for (i=1; i<npes; i++)
52
sdispls[i] = sdispls[i-1]+scounts[i-1];
53
54
55
56
rcounts = (int *)malloc(npes*sizeof(int));
57
MPI_Alltoall(scounts, 1, MPI_INT, rcounts, 1, MPI_INT, comm);
rdispls = (int *)malloc(npes*sizeof(int));
rdispls[0] = 0;
for (i=1; i<npes; i++)
rdispls[i] = rdispls[i-1]+rcounts[i-1];
*nsorted = rdispls[npes-1]+rcounts[i-1];
sorted_elmnts = (int *)malloc((*nsorted)*sizeof(int));
MPI_Alltoallv(elmnts, scounts, sdispls, MPI_INT, sorted_elmnts,
rcounts, rdispls, MPI_INT, comm);
qsort(sorted_elmnts, *nsorted, sizeof(int), IncOrder);
free(splitters); free(allpicks); free(scounts); free(sdispls);
free(rcounts); free(rdispls);
*/


  MPI_Finalize();
  return 0;
}

