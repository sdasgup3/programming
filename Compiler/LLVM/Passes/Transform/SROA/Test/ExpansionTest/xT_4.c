#include<stdio.h>

int main()
{ 
  typedef struct fruit {
    int variety;
    int shape; 
  } FRUIT;

  int count = 0 ;

  FRUIT f[10];
  for(int i=0; i < 10 ; i ++) {
    count += f[i].variety + f[1].shape ;
  }

  printf ("%d ", count);

  return 0;
}

