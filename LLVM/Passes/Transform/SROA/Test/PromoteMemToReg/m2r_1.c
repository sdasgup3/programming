#include "stdio.h"
#include "stdlib.h"


int main()
{
  int a;
  int b;

  typedef struct {
    int e;
    int f;
    int r;
  } twoInt;

  twoInt i2;
  i2.e=a;
  i2.f=b;
  i2.r=a+b;

  printf("%d %d\n", i2.e, i2.f);
  return 0;
}

