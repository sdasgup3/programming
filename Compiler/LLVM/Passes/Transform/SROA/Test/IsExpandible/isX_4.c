#include "stdio.h"
#include "stdlib.h"


int main()
{
  typedef struct {
    int e;
    int r;
  } twoInt;

  twoInt i2,i1 ;

  i1.e = i2.r;
  i1.r = i2.e;

  printf("%d %d %d %d", i1.e, i2.r, i1.r, i2.e );
 
  return 0;
}

