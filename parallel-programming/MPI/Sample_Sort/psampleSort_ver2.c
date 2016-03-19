#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <float.h>
#include <limits.h>
#include <sys/time.h>
#define SEED 100
#define CHECK 1
#define OUTPUT 0
//MPI sampleSort.  Taken from Grama et. al.
//Assume n is divisible by npes
double get_clock() {
   struct timeval tv; int ok;
   ok = gettimeofday(&tv, (void *) 0);
   if (ok<0) { printf("gettimeofday error");  }
   return (tv.tv_sec * 1.0 + tv.tv_usec * 1.0E-6); 
}

int compare(const void *num1, const void *num2) {
	unsigned long long* n1 = (unsigned long long*)num1;
	unsigned long long* n2 = (unsigned long long*)num2;
	return (*n1 > *n2) - (*n1 < *n2);
}

int main(int argc, char *argv[]) {
	int i,j,n,nlocal,nsorted,npes,myrank;
	double t1,t2;
	unsigned long long *all_data,*elmnts,*sorted_elmnts,*splitters,*sample,*firsts,*lasts;
	unsigned long long check, global_check;
	int *scounts, *sdispls, *rcounts, *rdispls;
	bool checkMax;
	MPI_Comm comm;

	MPI_Init(&argc,&argv);
	comm = MPI_COMM_WORLD;
	MPI_Comm_size(comm, &npes);
	MPI_Comm_rank(comm, &myrank);

	if (argc < 2) {
		fprintf(stderr,
			"Wrong number of arguments.\nUsage: %s N\n",
			argv[0]);
		return -1;
	}
	n = atoi(argv[1]);
	nlocal = n/npes;

	all_data = NULL;
	if (myrank == 0) {
		all_data = (unsigned long long*)malloc(sizeof(unsigned long long)*n);
		srand(SEED);
		for(i=0;i<n;i++) {
			all_data[i] = rand()%100;
		}
	}

	elmnts = (unsigned long long*)malloc(sizeof(unsigned long long)*nlocal);
	MPI_Scatter(all_data,nlocal,MPI_UNSIGNED_LONG_LONG,elmnts,nlocal,MPI_UNSIGNED_LONG_LONG,0,comm);

	#if CHECK
	check = 0;
	for(i=0;i<nlocal;i++) {
	  check ^= elmnts[i];
	}
	firsts = (unsigned long long*)malloc(sizeof(unsigned long long)*npes);
	lasts = (unsigned long long*)malloc(sizeof(unsigned long long)*npes);
	#endif

	splitters = (unsigned long long *)malloc(npes*sizeof(unsigned long long));
	sample = (unsigned long long *)malloc(npes*(npes-1)*sizeof(unsigned long long));
	scounts = (int *)malloc(npes*sizeof(int));
	rcounts = (int *)malloc(npes*sizeof(int));
	sdispls = (int *)malloc(npes*sizeof(int));
	rdispls = (int *)malloc(npes*sizeof(int));

	t1 = get_clock();

	//Sort local array
	qsort(elmnts, nlocal, sizeof(unsigned long long), compare);

	//Select local equally spaced elements
	for(i=1;i<npes;i++)
		splitters[i-1] = elmnts[i*nlocal/npes];

	MPI_Allgather(splitters, npes-1, MPI_UNSIGNED_LONG_LONG, sample, npes-1,
		MPI_UNSIGNED_LONG_LONG, comm);

	//Sort the gathered samples and get the splitters
	qsort(sample, npes*(npes-1), sizeof(unsigned long long), compare);
	for (i=1;i<npes;i++)
		splitters[i-1] = sample[i*(npes-1)];
	splitters[npes-1] = ULLONG_MAX;

	//Compute the number of elements that belong to each bucket
	for(i=0;i<npes;i++)
		scounts[i] = 0;

	j=0;
	for(i=0;i<nlocal;i++) {
		while(elmnts[i] >= splitters[j]) {
			j++;
		}
		scounts[j]++;
	}
	
	//Determine the starting location of each bucket's elements in the array
	sdispls[0] = 0;
	for (i=1;i<npes;i++)
		sdispls[i] = sdispls[i-1]+scounts[i-1];

	//Tell the other processes how many elements to receive
	MPI_Alltoall(scounts, 1, MPI_INT, rcounts, 1, MPI_INT, comm);

	rdispls[0] = 0;
	for (i=1;i<npes;i++)
		rdispls[i] = rdispls[i-1]+rcounts[i-1];

	nsorted = rdispls[npes-1]+rcounts[npes-1];
	sorted_elmnts = (unsigned long long*)malloc(nsorted*sizeof(unsigned long long));

	MPI_Alltoallv(elmnts, scounts, sdispls, MPI_UNSIGNED_LONG_LONG, sorted_elmnts,
		rcounts, rdispls, MPI_UNSIGNED_LONG_LONG, comm);

	//final local sort
	qsort(sorted_elmnts, nsorted, sizeof(unsigned long long), compare);

	t2 = get_clock();
	if (myrank==0) {
		printf("Time: %lf\n",(t2-t1));
	}

	#if CHECK
	for(i=0;i<nsorted;i++) {
		check ^= sorted_elmnts[i];
	}
	MPI_Reduce(&check,&global_check,1,MPI_UNSIGNED_LONG_LONG, MPI_BXOR, 0, comm);
	if(myrank == 0) {
		printf("The bitwise xor is %llu\n",global_check);
	}
	MPI_Gather(sorted_elmnts,1,MPI_UNSIGNED_LONG_LONG,firsts,1,MPI_UNSIGNED_LONG_LONG,0,comm);
	MPI_Gather(&sorted_elmnts[nsorted-1],1,MPI_UNSIGNED_LONG_LONG,lasts,1,MPI_UNSIGNED_LONG_LONG,0,comm);
	if(myrank == 0) {
		checkMax = true;
		for(i=0;i<npes-1;i++) {
			if(firsts[i+1] < lasts[i]) {
				checkMax = false;
			}
		}
		printf("The max of each bucket is not greater than the min of the next: %s\n", checkMax ? "true" : "false");
	}
	#endif

	#if OUTPUT
	MPI_Gather(&nsorted,1,MPI_INT,rcounts,1,MPI_INT,0,comm);
	if(myrank==0) {
		rdispls[0] = 0;
		for (i=1;i<npes;i++)
			rdispls[i] = rdispls[i-1]+rcounts[i-1];
	}
	MPI_Gatherv(sorted_elmnts,nsorted,MPI_UNSIGNED_LONG_LONG,all_data,rcounts,rdispls,MPI_UNSIGNED_LONG_LONG,0,comm);
	if(myrank==0) {
		for(i=0;i<n;i++) {
			printf("%6.2lf\n",all_data[i]);
		}
	}
	#endif
	MPI_Finalize();

	free(splitters); free(sample); free(scounts); free(sdispls);
	free(rcounts); free(rdispls); free(elmnts); free(sorted_elmnts);
	if(myrank==0) {
		free(all_data);
	}

	return 0;
}
