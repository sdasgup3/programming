#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <sys/time.h>
#include <limits.h>
#include <stdbool.h>
#define SEED 100
#define OUTPUT 0
#define CHECK 1
//Sequential sampleSort.  
//Assume size is a multiple of nbuckets*nbuckets

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
	int i,j,size,nbuckets,count;
	double t1,t2;
	unsigned long long *splitters,*elmnts,*sample,**buckets;
	unsigned long long check;
	int *bucket_sizes;
	bool checkMax;

	if (argc != 2) {
		fprintf(stderr,
			"Wrong number of arguments.\nUsage: %s N\n",
			argv[0]);
		return -1;
	}

	nbuckets = 12;
	size = atoi(argv[1]);
	splitters = (unsigned long long*)malloc(sizeof(unsigned long long)*nbuckets);
	elmnts = (unsigned long long*)malloc(sizeof(unsigned long long)*size);
	sample = (unsigned long long*)malloc(sizeof(unsigned long long)*size);
	buckets = (unsigned long long**)malloc(sizeof(unsigned long long*)*nbuckets);
	//the size of each bucket is guaranteed to be less than
	//2*size/nbuckets becuase of the way we choose the sample
	for(i=0;i<nbuckets;i++) {
		buckets[i] = (unsigned long long*)malloc(sizeof(unsigned long long)*2*size/nbuckets);
	}
	bucket_sizes = (int*)malloc(sizeof(int)*nbuckets);
	for(i=0;i<nbuckets;i++) {
		bucket_sizes[i] = 0;
	}

	//Fill elmnts with random numbers
	srand(SEED);
	for(i=0;i<size;i++) {
		elmnts[i] = rand()%100;
	}

	#if CHECK
	check = 0;
	for(i=0;i<size;i++) {
		check ^= elmnts[i];
	}
	#endif

	t1 = get_clock();

	//select the sample
	for(i=0;i<nbuckets;i++) {
		qsort(&elmnts[i*size/nbuckets],size/nbuckets,sizeof(unsigned long long),
			compare);
		for(j=0;j<nbuckets-1;j++) {
			sample[i*(nbuckets-1)+j] = elmnts[i*size/nbuckets+
				size/nbuckets/nbuckets*(j+1)];
		}
	}


	//select the splitters
	qsort(sample,nbuckets*(nbuckets-1),sizeof(unsigned long long),compare);
	for(i=1;i<nbuckets;i++) {
		splitters[i-1] = sample[i*(nbuckets-1)];
	}
	splitters[nbuckets-1] = ULLONG_MAX;

	//put into buckets based on splitters
	for(i=0;i<size;i++) {
		j = 0;
		while(j < nbuckets) {
			if(elmnts[i]<splitters[j]) {
				buckets[j][bucket_sizes[j]] = elmnts[i];
				bucket_sizes[j]++;
				j = nbuckets;
			}
			j++;
		}
	}

	//sort the buckets
	for(i=0;i<nbuckets;i++) {
		qsort(buckets[i],bucket_sizes[i],sizeof(unsigned long long),compare);
	}

	t2 = get_clock();
	printf("Time: %lf\n",(t2-t1));
	#if CHECK
	count = 0;
	for(i=0;i<nbuckets;i++) {
		for(j=0;j<bucket_sizes[i];j++) {
			check ^= buckets[i][j];
		}
		count += bucket_sizes[i];
	}
	printf("The bitwise xor is %llu\n",check);
	checkMax = true;
	for(i=0;i<nbuckets-1;i++) {
		if(buckets[i][bucket_sizes[i]-1] > buckets[i+1][0]) {
			checkMax = false;
		}
	}
	printf("The max of each bucket is not greater than the min of the next:	%s\n",
		checkMax ? "true" : "false");
	#endif
	#if OUTPUT
	count = 0;
	for(i=0;i<nbuckets;i++) {
		for(j=0;j<bucket_sizes[i];j++) {
			elmnts[count+j] = buckets[i][j];
		}
		count += bucket_sizes[i];
	}
	for(i=0;i<size;i++) {
		printf("%llu\n",elmnts[i]);
	}
	#endif

	free(splitters);
	free(elmnts);
	free(sample);
	free(bucket_sizes);
	for(i=0;i<nbuckets;i++) {
		free(buckets[i]);
	}
	free(buckets);
	
	return 0;
}
