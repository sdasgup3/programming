#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <sys/time.h>
#include <limits.h>
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

int compare(const void *num1, const void *num2) {
    long long* n1 = (long long*)num1;
    long long* n2 = (long long*)num2;
    return (*n1 > *n2) - (*n1 < *n2);
}

int main(int argc, char *argv[]) {
    int i,j,k,size,nbuckets,count;
    double t1,t2;
    long long *splitters,*elmnts,*sample,**buckets,*nsplitters;
    long long tol, error, maxval, minval, check;
    int *bucket_sizes,*hist,*cumulative,*ideal;
    bool repeat,checkMax;

    if (argc != 2) {
        fprintf(stderr,
            "Wrong number of arguments.\nUsage: %s N \n",argv[0]);
        return -1;
    }

    size = atoi(argv[1]);
    nbuckets = 12;
    tol = .3*((long long)size)/nbuckets;

    splitters = (long long*)malloc(sizeof(long long)*nbuckets);
    nsplitters = (long long*)malloc(sizeof(long long)*nbuckets);
    elmnts = (long long*)malloc(sizeof(long long)*size);
    sample = (long long*)malloc(sizeof(long long)*size);
    buckets = (long long**)malloc(sizeof(long long*)*nbuckets);
    for(i=0;i<nbuckets;i++) {
        buckets[i] = (long long*)malloc(sizeof(long long)*size);
    }
    bucket_sizes = (int*)malloc(sizeof(int)*nbuckets);
    hist = (int*)malloc(sizeof(int)*nbuckets);
    cumulative = (int*)malloc(sizeof(int)*nbuckets);
    ideal = (int*)malloc(sizeof(int)*nbuckets);

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

    //sort each bucket individually
    for(i=0;i<nbuckets;i++) {
        qsort(&elmnts[i*size/nbuckets],size/nbuckets,sizeof(long long),
            compare);
    }

    //select the initial splitters
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

    for(i=0;i<nbuckets;i++) {
        ideal[i] = size/nbuckets*(i+1);
    }

    //Create histogram
    repeat = true;
    while(repeat) {
        repeat = false;
        for(i=0;i<nbuckets;i++) {
            hist[i] = 0;
        }
        for(i=0;i<nbuckets;i++) { 
            k = 0;
            for(j=i*size/nbuckets;j<(i+1)*size/nbuckets;j++) {
                if(elmnts[j] < splitters[k]) {
                    hist[k]++;
                }
                else {
                    while(elmnts[j] > splitters[k]) {
                        k++;
                    }
                    hist[k]++;
                }
            }
        }
        
        cumulative[0] = hist[0];
        for(i=1;i<nbuckets;i++) {
            cumulative[i] = cumulative[i-1]+hist[i];
        }

        //Check the global histogram for goodness of split
        for(i=0;i<nbuckets-1;i++) {
            nsplitters[i] = splitters[i];
            error = cumulative[i]-ideal[i]; 
            if(abs(error) > tol) {
                repeat = true;
                //update probe by scaled linear interpolation
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
        nsplitters[nbuckets-1] = splitters[nbuckets-1];

        for(i=0;i<nbuckets;i++) {
            splitters[i] = nsplitters[i];
        }
    }

    //put into buckets based on splitters
    for(i=0;i<nbuckets;i++) {
        bucket_sizes[i] = 0;
    }
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
        qsort(buckets[i],bucket_sizes[i],sizeof(long long),compare);
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
    printf("The max of each bucket is not greater than the min of the next:    %s\n",
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
    free(hist);
    for(i=0;i<nbuckets;i++) {
        free(buckets[i]);
    }
    free(buckets);
    
    return 0;
}
