#include "stdio.h"
#include "stdlib.h"


int main()
{
  typedef struct {
    int e;
    int r;
  } twoInt;

  twoInt i2 ;

  int a = i2.e + i2.r;  

  printf("%d", a);
 
  return 0;
}

