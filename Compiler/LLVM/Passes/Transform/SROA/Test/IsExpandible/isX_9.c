#include "stdio.h"
#include "stdlib.h"


int main()
{

  typedef struct {
    int e;;
    int r;;
  } One;

  typedef struct {
    One O;
  } Two;

  typedef struct {
    Two T;
    One O;
    int w;
  } Three;

  Three i ;

  i.O.e = i.T.O.r;
  i.T.O.e = i.O.e * i.O.r;
  printf("%d ", NULL != &(i) );
  printf("%d ", NULL != &(i.w) );
  printf("%d ", NULL != &(i.T.O.e) );

 
  return 0;
}

