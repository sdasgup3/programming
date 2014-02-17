#include<stdio.h>



int main()
{

  int x = 9;
  int* y = &x;
  
  int z = x + *y;
  printf("%d", z);
}

