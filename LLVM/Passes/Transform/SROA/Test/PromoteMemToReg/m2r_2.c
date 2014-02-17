#include<stdio.h>


int fun(int x1, int y1)
{
  int p = x1*y1;
  return p;
}

int main()
{

  int x = 9;
  int y = 10;
  
  int z = fun(x,y);
  printf("%d", z);
}

