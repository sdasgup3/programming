#include "stdio.h"
#include "stdlib.h"


int main()
{
  typedef struct {
    int e;
    int r;
  } One;
  One i;

  if(&(i.e) != NULL  || &(i.r) != NULL) {
    printf("%d", i.r);
  }
 
  return 0;
}

