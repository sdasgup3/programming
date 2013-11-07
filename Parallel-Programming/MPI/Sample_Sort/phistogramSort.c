#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <sys/time.h>
#include <limits.h>
#include <mpi.h>
#include <stdbool.h>
#define SEED 100
#define OUTPUT 0
#define CHECK 1
//Sequential histogramSort.  
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
     long long* n1 = ( long long*)num1;
     long long* n2 = ( long long*)num2;
    return (*n1 > *n2) - (*n1 < *n2);
}

int main(int argc, char *argv[]) {
    int i,j,k,size,nbuckets, numprocs, count, local_size, myrank;
    double t1,t2;
     long long *splitters,*elmnts,*buckets,*nsplitters,
                  *rearranged_bucket,*local_elmnts, *local_buckets, *output_buffer, *first_last_elem_of_bkt_buffer ;
     long long tol, error, maxval, minval, check, local_check, global_check;
    int *hist,*cumulative,*ideal, *recvcounts;
    bool checkMax;
    int repeat,local_repeat;

    if (argc != 2) {
        fprintf(stderr,
            "Wrong number of arguments.\nUsage: %s N \n",argv[0]);
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
      
      elmnts = ( long long*)mymalloc(sizeof( long long)*size);

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
    tol = .3*(( long long)size)/nbuckets;

    local_elmnts = ( long long*)mymalloc(sizeof( long long)*local_size);

    /* Distribute size/numprocs elements to each process */
    MPI_Scatter(elmnts, local_size, MPI_LONG_LONG, local_elmnts, 
        local_size, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    t1 = get_clock();

    qsort(local_elmnts,local_size,sizeof( long long), compare);

    /*Colllect all the indivdal sorted arrays at Rank 0*/
    MPI_Gather(local_elmnts, local_size , MPI_LONG_LONG,
        elmnts, local_size, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    splitters = ( long long*)mymalloc(sizeof( long long)*nbuckets);
    nsplitters = ( long long*)mymalloc(sizeof( long long)*nbuckets);
    recvcounts = (int *) mymalloc(sizeof(int)*numprocs);
    for (i=0; i<nbuckets; i++)
        recvcounts[i] = 1;

    /* rank 0 will select the initial splitters*/
    if(0 == myrank) {
      for(i=0;i<nbuckets-1;i++) {  
          splitters[i] = elmnts[size/nbuckets/nbuckets*(i+1)];
      }
      maxval = elmnts[0];
      minval = elmnts[0];
      for(i=0;i<size;i++) {
          if(elmnts[i] > maxval) {
              maxval = elmnts[i];
          }
          if(elmnts[i] < minval) {
              minval = elmnts[i];
          }
      }
      maxval+=1;
      splitters[nbuckets-1] = maxval;
    }

    ideal = (int*)mymalloc(sizeof(int)*nbuckets);
    for(i=0;i<nbuckets;i++) {
        ideal[i] = local_size*(i+1);
    }

    /*Rank 0 broadcast the minval  */
    MPI_Bcast (&minval, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    /*Rank 0 broadcast the initial splitters */
    MPI_Bcast (splitters, nbuckets, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    /* Create local histogram*/
    hist = (int*)mymalloc(sizeof(int)*nbuckets);
    cumulative = (int*)mymalloc(sizeof(int)*nbuckets);
    repeat = true;
    local_repeat = true;

    while(repeat) {
        local_repeat = false;
        for(i=0;i<nbuckets;i++) {
            hist[i] = 0;
        }

        k = 0;
        for(j = 0;j < local_size ; j++) {
            if(local_elmnts[j] < splitters[k]) {
                hist[k]++;
            }
            else {
                while(local_elmnts[j] > splitters[k]) {
                    k++;
                }
                hist[k]++;
            }
        }

        /*Applying reduce scatter, such that hist[0] entries from all processes
          are added up to rank 0, and soon.
        */
        int hist_buffer;
        MPI_Reduce_scatter (hist, &hist_buffer, recvcounts, 
            MPI_INT,MPI_SUM,MPI_COMM_WORLD);

        int cumulative_buffer;
        MPI_Scan ( &hist_buffer, &cumulative_buffer, 1, MPI_INT , MPI_SUM , MPI_COMM_WORLD);
        MPI_Allgather ( &cumulative_buffer, 1, MPI_INT , cumulative, 1, MPI_INT ,
                       MPI_COMM_WORLD );

        //Check the global histogram for goodness of split
        if(myrank == nbuckets-1) {
            nsplitters[nbuckets-1] = splitters[nbuckets-1];
        } else {
            nsplitters[myrank] = splitters[myrank];
            error = cumulative[myrank]-ideal[myrank]; 

            if(abs(error) > tol) {
                local_repeat = true;
                //update probe by scaled linear interpolation
                if(error > tol) {
                    k = myrank-1;
                    while(k > -1 && cumulative[k] > ideal[myrank]) {
                        k--;
                    }
                } else {
                    k = myrank+1;
                    while(cumulative[k] < ideal[myrank]) {
                        k++;
                    }
                }

                if(k > -1) {
                    nsplitters[myrank] += (splitters[k]-splitters[myrank])*
                        (double)abs(ideal[myrank]-cumulative[myrank])/
                        (double)abs(cumulative[k]-cumulative[myrank]);
                } else {
                    nsplitters[myrank] += (minval - splitters[myrank])*
                        (double)ideal[myrank]/(double)cumulative[myrank];
                }
            }
        }

        MPI_Allgather ( &nsplitters[myrank], 1, MPI_LONG_LONG , 
            splitters, 1, MPI_LONG_LONG , MPI_COMM_WORLD );

        MPI_Allreduce ( &local_repeat, &repeat, 1,
                          MPI_INT , MPI_LOR, MPI_COMM_WORLD  );
    }

    /* Creating local buckets and  put into buckets based on splitters */
    buckets = ( long long*)mymalloc(
        sizeof( long long)*(size + numprocs));

    //put into buckets based on splitters
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

    rearranged_bucket = ( long long*)mymalloc(
        sizeof( long long)*(size + numprocs));
    MPI_Alltoall (buckets, local_size + 1, MPI_LONG_LONG, 
        rearranged_bucket, local_size + 1, MPI_LONG_LONG, MPI_COMM_WORLD);

    /*Removing the metadata like element count from the rearranged_bucket*/
    //the size of each bucket is guaranteed to be less than
    //2*size/nbuckets becuase of the way we choose the sample
    local_buckets = ( long long*)mymalloc(
        sizeof( long long)*2*local_size);

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
        sizeof( long long), compare);


    t2 = get_clock();

    if(0 == myrank) {
        printf("Time: %lf\n",(t2-t1));
        first_last_elem_of_bkt_buffer =  ( long long*) mymalloc ( sizeof( long long) * 2 * numprocs);
    }
        
    #if CHECK
    local_check =  0 ;
    for(j = 1 ; j <= local_buckets[0]; j ++) {
      local_check ^= local_buckets[j];
    }
    MPI_Reduce ( &local_check, &global_check, 1, MPI_LONG_LONG , MPI_BXOR, 0, MPI_COMM_WORLD);

    if(0 == myrank) {
      check ^= global_check;
      printf("The bitwise xor is %llu\n",check);
    }

     long long first_last_elem_of_bkt[2];
    first_last_elem_of_bkt[0]           =  local_buckets[1];
    first_last_elem_of_bkt[1]           =  local_buckets[local_buckets[0]];  

    MPI_Gather(first_last_elem_of_bkt, 2, MPI_LONG_LONG, first_last_elem_of_bkt_buffer, 2, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

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
        output_buffer = ( long long*) mymalloc (
            sizeof( long long) * 2 * size);
    }

    MPI_Gather (local_buckets, 2*local_size, MPI_LONG_LONG, 
          output_buffer, 2*local_size, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

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
    free(nsplitters);
    free(recvcounts);
    free(hist);
    free(cumulative);
    free(buckets);
    free(ideal);
    
    /**** Finalize ****/
    MPI_Finalize();
    return 0;
}
