#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>


void * hello(void* arg) 
{
   printf ("Hello %d\n", *((int *)arg));

}

int main(int argc, char *argv[]) {
  int i, j;
  int *p;


  int numThreads = strtol(argv[1],NULL , 10);
printf ("%d", numThreads);

  pthread_t *threads = (pthread_t *) malloc(numThreads*sizeof(pthread_t));  

  // Create threads 
  for (i = 0; i < numThreads; i++) {
     pthread_create(&threads[i], NULL, hello, (void *)&i);	
  }

  for (i = 0; i < numThreads; i++) {
    pthread_join(threads[i], NULL);
  }

}

