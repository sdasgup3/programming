#include "stdio.h"
#include "stdlib.h"


int main()
{
  typedef struct {
    int f;
  } Zero;
  typedef struct {
    int i;
    Zero z;
  } One;

  One i ;
  int a  = i.i * i.z.f;

  printf("%d ", a);
  printf("%d ", NULL == &(i.z.f));
  printf("%d ", NULL == &(i.i));
 
  return 0;
}

