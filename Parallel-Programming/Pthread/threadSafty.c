#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>


struct arg_struct {
  int id;
  int arg;
};


void* Hello (void*  arguments ) 
{	
  struct arg_struct *args = (struct arg_struct *)arguments;


  if(args->arg == 0) {
    printf ( "Last Hello from thread [%d] : %d\n" , args->id, args->arg) ;
    return NULL;
  } else {
    printf ( "Hello from thread [%d] : %d\n" , args->id, args->arg) ;
    args->arg -= 1;
    Hello((void* )args);
  }
  return NULL;
}


void main ( int argc , char*  argv [ ] ) 
{	
  long t; 
  pthread_t* threadHandles ;
  int threads = strtol(argv[1],NULL,10);

  printf ("%d\n", threads);

  threadHandles = (pthread_t *) malloc ( threads * sizeof( pthread_t  ) ) ;


  for ( t = 0 ; t< threads; t ++ ) {
    struct arg_struct *args = (struct arg_struct *) malloc (sizeof(struct arg_struct ));
    args->arg = 10;
    args->id = t;
    pthread_create(&threadHandles [ t ] , NULL ,Hello , (void *)(args));
  }


  for ( t= 0 ; t < threads; t ++)   pthread_join ( threadHandles [ t ] , NULL ) ;

  free( threadHandles ) ;
} 
