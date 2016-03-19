#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <sys/time.h>
#include <limits.h>
#include <stdbool.h>
#include <mpi.h>
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

void* mymalloc(size_t alloc_size) 
{
  void* mem = malloc(alloc_size);
  if(NULL == mem) {
    printf("Cannot allocate memory.. Exiting");
    MPI_Finalize();
    exit(0);
  }
  return mem; 
}

int compare(const void *num1, const void *num2) {
    unsigned long long* n1 = (unsigned long long*)num1;
    unsigned long long* n2 = (unsigned long long*)num2;
    return (*n1 > *n2) - (*n1 < *n2);
}

int main(int argc, char *argv[]) {
    int i, j, k, size, nbuckets, numprocs, count, local_size, myrank;
    double t1,t2;
    unsigned long long *splitters, *elmnts,*sample,*buckets, *rearranged_bucket,  
                  *local_elmnts, *local_buckets, *output_buffer, *first_last_elem_of_bkt_buffer;
    unsigned long long check, local_check, global_check;
    bool checkMax;

    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments.\nUsage: %s N\n", argv[0]);
        return -1;
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
        
    nbuckets = numprocs;

    /* Rank 0 will read the input and broadcast it*/ 
    if(0 == myrank) {
        size = atoi(argv[1]);

        if(( size % numprocs) != 0){
	    printf("Number of Elements are not divisible by Numprocs \n");
            MPI_Finalize();
	    exit(0);
        }

        elmnts = (unsigned long long*)mymalloc(sizeof(unsigned long long)*size);
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
    }
    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    local_size = size/numprocs;
    local_elmnts = (unsigned long long*)mymalloc(sizeof(unsigned long long)*local_size);

    /* Distribute size/numprocs elements to each process */
    MPI_Scatter(elmnts, local_size, MPI_UNSIGNED_LONG_LONG, local_elmnts, 
        local_size, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);


    MPI_Barrier(MPI_COMM_WORLD);
    t1 = get_clock();

    /* Do local sorting */
    qsort(local_elmnts,local_size,sizeof(unsigned long long), compare);

    /* Select local splitters */
    splitters = (unsigned long long*)mymalloc(sizeof(unsigned long long)*numprocs);
    for(i=0;i<nbuckets-1;i++) {
        splitters[i] = local_elmnts[local_size/numprocs*(i+1)];
    }

    /* Collect all the local splitters at the root to form the sample*/
    sample = (unsigned long long*)mymalloc(
        sizeof(unsigned long long)*numprocs*(numprocs -1));
    MPI_Gather(splitters, numprocs -1 , MPI_UNSIGNED_LONG_LONG,
        sample, numprocs-1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);

    /* Sorting the sample at rank 0 and choosing global splitters */
    if (0 == myrank){
      qsort (sample, numprocs*(numprocs-1), sizeof(unsigned long long), compare);

      for(i=1;i<numprocs;i++) {
          splitters[i-1] = sample[i*(nbuckets-1)];
      }
      splitters[nbuckets-1] = ULLONG_MAX;
    }

    /* Broadcasting the global splitters */
    MPI_Bcast (splitters, numprocs-1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);

    /* Creating local buckets and  put into buckets based on splitters */

    buckets = (unsigned long long*)mymalloc(
        sizeof(unsigned long long)*(size + numprocs));
  
    j = 0;
    k = 1;
    for (i=0; i<local_size; i++){
        if(j < (numprocs-1)){
            if (local_elmnts[i] < splitters[j]) {
	        buckets[((local_size + 1) * j) + k++] = local_elmnts[i]; 
            } else {
	        buckets[(local_size + 1) * j] = k-1;
	        k=1;
		j++;
		i--;
            }
        } else {
            buckets[((local_size + 1) * j) + k++] = local_elmnts[i];
        }
    }
    buckets[(local_size + 1) * j] = k - 1;

    /* Each of the processes have Buckets as B0, B1, .. B(p-1), where B0 is
       less than splitters[0], B1 is less than splitters[1], and soon.
       Now we are going to send buckets to respective processes such that
       rank 0 has contents of all the buckets whose elements are less 
       than splitters[0], and soon.
    */

    rearranged_bucket = (unsigned long long*)mymalloc(
        sizeof(unsigned long long)*(size + numprocs));
    MPI_Alltoall (buckets, local_size + 1, MPI_UNSIGNED_LONG_LONG, 
        rearranged_bucket, local_size + 1, MPI_UNSIGNED_LONG_LONG, MPI_COMM_WORLD);

    /*Removing the metadata like element count from the rearranged_bucket*/
    //the size of each bucket is guaranteed to be less than
    //2*size/nbuckets becuase of the way we choose the sample
    local_buckets = (unsigned long long*)mymalloc(
        sizeof(unsigned long long)*2*local_size);

    count = 1;

    for (j=0; j<numprocs; j++) {
      k = 1;
      for (i=0; i<rearranged_bucket[(local_size + 1) * j]; i++)  {
        local_buckets[count++] = rearranged_bucket[(local_size + 1) * j + k++];
      }
    }
    local_buckets[0] = count-1;
    

    //sort the local buckets
    qsort (&local_buckets[1], local_buckets[0], 
        sizeof(unsigned long long), compare);


    t2 = get_clock();

    if(0 == myrank) {
        printf("Time: %lf\n",(t2-t1));
        first_last_elem_of_bkt_buffer =  (unsigned long long*) mymalloc ( sizeof(unsigned long long) * 2 * numprocs);
    }
        
    #if CHECK
    local_check =  0 ;
    for(j = 1 ; j <= local_buckets[0]; j ++) {
      local_check ^= local_buckets[j];
    }
    MPI_Reduce ( &local_check, &global_check, 1, MPI_UNSIGNED_LONG_LONG , MPI_BXOR, 0, MPI_COMM_WORLD);

    if(0 == myrank) {
      check ^= global_check;
      printf("The bitwise xor is %llu\n",check);
    }

    unsigned long long first_last_elem_of_bkt[2];
    first_last_elem_of_bkt[0]           =  local_buckets[1];
    first_last_elem_of_bkt[1]           =  local_buckets[local_buckets[0]];  

    MPI_Gather(first_last_elem_of_bkt, 2, MPI_UNSIGNED_LONG_LONG, first_last_elem_of_bkt_buffer, 2, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);

    if(0 == myrank) {
        checkMax = true;
        for(i=1;i < nbuckets - 1 ; i++) {
          if(first_last_elem_of_bkt_buffer[2*i - 1 ] > first_last_elem_of_bkt_buffer[2*i]) {
              checkMax = false;
          }
      }
      printf("The max of each bucket is not greater than the min of the next:    %s\n", checkMax ? "true" : "false");
    }
    #endif

    /* Collecting the sorted buckets at rank 0 */
    if(0 == myrank) {
        output_buffer = (unsigned long long*) mymalloc (
            sizeof(unsigned long long) * 2 * size);
    }

    MPI_Gather (local_buckets, 2*local_size, MPI_UNSIGNED_LONG_LONG, 
          output_buffer, 2*local_size, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);

    if(0 == myrank) {
      #if OUTPUT
      /* Createing the ultimate output */
      count = 0;
      for(i=0; i<numprocs; i++){
          k = 1;
          int offset = (2 * local_size) * i;
          for(j=0; j<output_buffer[offset]; j++) 
              elmnts[count++] = output_buffer[offset + k++];
      }

      for(i=0;i<size;i++) {
          printf("%llu\n",elmnts[i]);
      }
      #endif
      free(output_buffer);
      free(elmnts);
      free(first_last_elem_of_bkt_buffer);
    }

    free(splitters);
    free(local_elmnts);
    free(sample);
    free(buckets);
    free(local_buckets);
    free(rearranged_bucket);
    
    /**** Finalize ****/
    MPI_Finalize();
    return 0;
}

