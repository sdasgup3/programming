// CSc 422
// Matrix multiplication example with pthreads

#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>

double **a, **b, **c;
int numThreads, matrixSize;

double **allocateMatrix() {
  int i;
  double *vals, **temp;

  // allocate values
  vals = (double *) malloc (matrixSize * matrixSize * sizeof(double));

  // allocate vector of pointers
  temp = (double **) malloc (matrixSize * sizeof(double*));

  for(i=0; i < matrixSize; i++)
    temp[i] = &(vals[i * matrixSize]);

  return temp;
}

void printMatrix(double **mat) {
  int i,j;

  printf("The %d * %d matrix is\n", matrixSize, matrixSize);
  for(i=0; i < matrixSize; i++){
    for(j=0; j < matrixSize; j++)
      printf("%lf ",  mat[i][j]);
    printf("\n");
  }
}

void mm(int myId) {
  int i,j,k;
  double sum;

  // compute bounds for this threads---just algebra
  int startrow = myId * matrixSize/numThreads;
  int endrow = (myId+1) * (matrixSize/numThreads) - 1;

  // matrix mult over the strip of rows for this thread
  for (i = startrow; i <= endrow; i++) {
    for (j = 0; j < matrixSize; j++) {
      sum = 0.0;
      for (k = 0; k < matrixSize; k++) {
	sum = sum + a[i][k] * b[k][j];
      }
      c[i][j] = sum;
    }
  }
}

void *worker(void *arg)
{
  int id = *((int *) arg);
  mm(id);
}


int main(int argc, char *argv[]) {
  int i, j;
  int *p;
  pthread_t *threads;

  if (argc != 3) {
    printf("Usage: %s <size> <n>,  where size is dimension of square matrix, and n is number of threads\n", argv[0]);
    exit(1);
  }

  matrixSize = atoi(argv[1]);
  numThreads = atoi(argv[2]);

  a = allocateMatrix();
  b = allocateMatrix();
  c = allocateMatrix();
  
  for (i = 0; i < matrixSize; i++)
    for (j = 0; j < matrixSize; j++) {
      a[i][j] = i + j;
      b[i][j] = i + j;
    }

  if (matrixSize < 10) {  
    printMatrix(a);
    printMatrix(b);
  }

  // Allocate thread handles
  threads = (pthread_t *) malloc(numThreads * sizeof(pthread_t));

  // Create threads 
  for (i = 0; i < numThreads; i++) {
    p = (int *) malloc(sizeof(int));  // yes, memory leak, don't worry for now
    *p = i;
    pthread_create(&threads[i], NULL, worker, (void *)(p));
  }

  for (i = 0; i < numThreads; i++) {
    pthread_join(threads[i], NULL);
  }

  if (matrixSize < 10)   // print matrix if relatively small
    printMatrix(c); 
}

