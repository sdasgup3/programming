#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>


void* Hello ( void*  myRank ) 
{	printf ( "Hello from thread %d\n" , *((int *)(myRank)) ) ;
	return NULL ;
}

void main ( int argc , char*  argv [ ] ) 
{	long t; 
	pthread_t* threadHandles ;
         int threads = strtol(argv[1],NULL,10);

printf ("%d\n", threads);

//exit(0);
	threadHandles = (pthread_t *) malloc ( threads * sizeof( pthread_t  ) ) ;

	 for ( t = 0 ; t< threads; t ++ )
 		pthread_create(&threadHandles [ t ] , NULL ,Hello , ( void *) &t );

	printf ( "Hello from the main thread\n" ) ;
	for ( t= 0 ; t < threads; t ++)   pthread_join ( threadHandles [ t ] , NULL ) ;
	 free( threadHandles ) ;
} 
