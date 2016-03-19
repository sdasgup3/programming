#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <float.h>
#include <sys/time.h>
#define SEED 100
#define OUTPUT 0
#define CHECK 1
//MPI histogramSort.
//Assume n is divisible by npes
double get_clock() {
   struct timeval tv; int ok;
   ok = gettimeofday(&tv, (void *) 0);
   if (ok<0) { printf("gettimeofday error");  }
   return (tv.tv_sec * 1.0 + tv.tv_usec * 1.0E-6); 
}

int compare(const void *num1, const void *num2) {
	long long* n1 = (long long*)num1;
	long long* n2 = (long long*)num2;
	return (*n1 > *n2) - (*n1 < *n2);
}

int main(int argc, char *argv[]) {
	int i,j,k,size,nlocal,nsorted,npes,rank,repeat;
	double t1,t2;
	long long tol,minval,maxval,error,check,global_check;
	long long *all_data,*elmnts,*sorted_elmnts,*splitters,*nsplitters,*firsts,*lasts;
	int *scounts, *sdispls, *rcounts, *rdispls; 
	int *ideal, *cumulative, *hist; 
	bool checkMax;
	MPI_Comm comm;

	MPI_Init(&argc,&argv);
	comm = MPI_COMM_WORLD;
	MPI_Comm_size(comm, &npes);
	MPI_Comm_rank(comm, &rank);

	if (argc != 2) {
		fprintf(stderr,"Wrong number of arguments.\nUsage: %s N\n",
			argv[0]);
		return -1;
	}
	size = atoi(argv[1]);
	nlocal = size/npes;

	tol = .3*nlocal;

	all_data = NULL;
	if (rank == 0) {
		all_data = (long long*)malloc(sizeof(long long)*size);
		srand(SEED);
		for(i=0;i<size;i++) {
			all_data[i] = rand()%100;
		}
	}
	elmnts = (long long*)malloc(sizeof(long long)*nlocal);
	splitters = (long long *)malloc(npes*sizeof(long long));
	nsplitters = (long long *)malloc(npes*sizeof(long long));
	hist = (int *)malloc(npes*sizeof(int));
	ideal = (int *)malloc(npes*sizeof(int));
	cumulative = (int *)malloc(npes*sizeof(int));
	scounts = (int *)malloc(npes*sizeof(int));
	sdispls = (int *)malloc(npes*sizeof(int));
	rcounts = (int *)malloc(npes*sizeof(int));
	rdispls = (int *)malloc(npes*sizeof(int));

	MPI_Scatter(all_data,nlocal,MPI_LONG_LONG_INT,elmnts,nlocal,MPI_LONG_LONG_INT,0,comm);

	#if CHECK
	check = 0;
	for(i=0;i<nlocal;i++) {
	  check ^= elmnts[i];
	}
	firsts = (long long*)malloc(sizeof(long long)*npes);
	lasts = (long long*)malloc(sizeof(long long)*npes);
	#endif

	t1 = get_clock();


	//Sort local array
	qsort(elmnts, nlocal, sizeof(long long), compare);

	//Select and broadcast splitters
	MPI_Allreduce(&elmnts[0], &minval, 1, MPI_LONG_LONG_INT, MPI_MIN, comm); 
	MPI_Allreduce(&elmnts[nlocal-1], &maxval, 1, MPI_LONG_LONG_INT, MPI_MAX, comm);
	maxval += 1;
	if(rank == 0) {
		for(i=0;i<npes-1;i++) {  
			splitters[i] = elmnts[nlocal/npes*(i+1)];
		}
		splitters[npes-1] = maxval;
	}
	MPI_Bcast(splitters,npes,MPI_LONG_LONG_INT,0,comm);

	for(i=0;i<npes;i++) {
		ideal[i] = nlocal*(i+1);
	}

	//Create histogram
	repeat = 1;
	while(repeat) {
		repeat = 0;
		for(i=0;i<npes;i++) {
			hist[i] = 0;
		}

		//local histogram
		j = 0;
		for(i=0;i<nlocal;i++) {
			if(elmnts[i] < splitters[j]) {
				hist[j]++;
			}
			else {
				while(elmnts[i] > splitters[j]) {
					j++;
				}
				hist[j]++;
			}
		}

		//cumulative histogram
		MPI_Reduce(hist, cumulative, npes, MPI_INT, MPI_SUM, 0, comm);
		for (i=1;i<npes;i++) {
			cumulative[i] += cumulative[i-1];
		}

		if(rank == 0) {
			//Check the global histogram for goodness of split
			for(i=0;i<npes-1;i++) {
				nsplitters[i] = splitters[i];
				error = cumulative[i] - ideal[i];
				if(abs(error) > tol) {
					repeat = 1;
					if(error > tol) {
						k = i-1;
						while(k > -1 && cumulative[k] > ideal[i]) {
							k--;
						}
					} else {
						k = i+1;
						while(cumulative[k] < ideal[i]) {
							k++;
						}
					}

					if(k > -1) {
						nsplitters[i] += (splitters[k]-splitters[i])*
							(double)abs(ideal[i]-cumulative[i])/
							(double)abs(cumulative[k]-cumulative[i]);
					} else {
						nsplitters[i] += (minval-splitters[i])*
							(double)ideal[i]/(double)cumulative[i];
					}
				}
			}
			nsplitters[npes-1] = splitters[npes-1];

			for(i=0;i<npes;i++) {
				splitters[i] = nsplitters[i];
			}
		}

		MPI_Bcast(splitters,npes,MPI_LONG_LONG_INT,0,comm);
		MPI_Bcast(&repeat,1,MPI_INT,0,comm);
	}

	//Compute the number of elements sent to each process
	for(i=0;i<npes;i++)
		scounts[i] = hist[i];

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
	sorted_elmnts = (long long*)malloc(nsorted*sizeof(long long));

	MPI_Alltoallv(elmnts, scounts, sdispls, MPI_LONG_LONG_INT, sorted_elmnts,
		rcounts, rdispls, MPI_LONG_LONG_INT, comm);

	//final local sort
	qsort(sorted_elmnts, nsorted, sizeof(long long), compare);

	MPI_Barrier(comm);
	t2 = get_clock();
	if (rank==0) {
		printf("Time: %lf\n",(t2-t1));
	}

	#if CHECK
	for(i=0;i<nsorted;i++) {
		check ^= sorted_elmnts[i];
	}
	MPI_Reduce(&check,&global_check,1,MPI_LONG_LONG, MPI_BXOR, 0, comm);
	if(rank == 0) {
		printf("The bitwise xor is %llu\n",global_check);
	}
	MPI_Gather(sorted_elmnts,1,MPI_LONG_LONG_INT,firsts,1,MPI_LONG_LONG_INT,0,comm);
	MPI_Gather(&sorted_elmnts[nsorted-1],1,MPI_LONG_LONG_INT,lasts,1,MPI_LONG_LONG_INT,0,comm);
	if(rank == 0) {
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
	if(rank==0) {
		rdispls[0] = 0;
		for (i=1;i<npes;i++)
			rdispls[i] = rdispls[i-1]+rcounts[i-1];
	}
	MPI_Gatherv(sorted_elmnts,nsorted,MPI_LONG_LONG_INT,all_data,rcounts,rdispls,MPI_LONG_LONG_INT,0,comm);
	if(rank==0) {
		for(i=0;i<size;i++) {
			printf("%6.2lld\n",all_data[i]);
		}
	}
	#endif

	MPI_Finalize();

	free(splitters); free(scounts); free(sdispls);
	free(rcounts); free(rdispls); free(elmnts); free(sorted_elmnts);
	if(rank==0) {
		free(all_data);
	}

	return 0;
}
