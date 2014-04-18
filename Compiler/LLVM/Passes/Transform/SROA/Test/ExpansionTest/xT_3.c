#include<stdio.h>

int main()
{ 
  typedef struct fruit {
    int variety;
    int shape; 
  } FRUIT;

  FRUIT f;
  for(int i=0; i < 10 ; i ++) {
    f.variety += 2*i;
    f.shape += 5*i;
  }

  printf ("%d %d", f.variety, f.shape);

  return 0;
}

