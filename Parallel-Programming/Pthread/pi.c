#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

int inTheCircle = 0;
pthread_mutex_t         mutex = PTHREAD_MUTEX_INITIALIZER;


void* Hello ( void*  myRank ) 
{	
	int local =  *((int *)(myRank))  ;

        double x, y ;
	int i;;
      for (i=0; i<local; i++)
      {   x = rand();
          y = rand();
printf("%f\n", x*x + y*y);
if((x*x + y*y) < 100.0) {
               pthread_mutex_lock(&mutex);
	          inTheCircle++;
	         pthread_mutex_unlock(&mutex);
           }
	}
	return NULL ;
}

void main ( int argc , char*  argv [ ] ) 
{	long t; 
	pthread_t* threadHandles ;
         int threads = strtol(argv[1],NULL,10);

printf ("%d\n", threads);

	threadHandles = (pthread_t *) malloc ( threads * sizeof( pthread_t  ) ) ;

        int numtrials=1000000;

		int local = numtrials/threads;
	 for ( t = 0 ; t< threads; t ++ ) {
 		pthread_create(&threadHandles [ t ] , NULL ,Hello , ( void *) &local );
      }
	printf ( "%d %d pi = %d \n",inTheCircle, numtrials, 4*inTheCircle/numtrials ) ;
	for ( t= 0 ; t < threads; t ++)   pthread_join ( threadHandles [ t ] , NULL ) ;
	 free( threadHandles ) ;
} 
