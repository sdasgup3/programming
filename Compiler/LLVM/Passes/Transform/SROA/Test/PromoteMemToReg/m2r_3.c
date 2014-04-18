#include<stdio.h>
#include<stdlib.h>


int main()
{

  int *x = (int *)malloc(sizeof(int));
  int y = 10;
  
  *x = y;
  printf("%d", *x);
}

