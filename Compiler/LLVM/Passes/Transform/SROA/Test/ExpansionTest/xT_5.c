#include<stdio.h>

int main()
{ 
  typedef struct fruit {
    int variety;
    int shape; 
  } FRUIT;


  for(int i=0; i < 10 ; i ++) {
    FRUIT f;
    f.variety = 2*f.shape;
  }


  return 0;
}

