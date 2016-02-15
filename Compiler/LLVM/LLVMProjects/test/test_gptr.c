#include<stdlib.h>


struct S {
  int x, y; 
};

struct S data[][2] =
  {
    {
      { 1,2 },
      { 3,4 }
    },
    {
      { 5,6 },
      { 7,8 }
    },
  };

int main(int argc, char** argv) {


  int i, x, S;
  x= 0 ;
  struct S* z;


  for(i = 0 ; i < 1 ; i ++) {
    z   = &data[x][x];
    S = z->x ; 
  }

  //int arr[100];
  //int y  = arr[x];
  int *arr  = (int *) malloc(100*sizeof(int));
  int y  = arr[x];

  return 0;
}
