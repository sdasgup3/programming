#include <stdio.h>
#define VECTOR_SIZE 4

//gcc test_1.c

typedef int v4si __attribute__ ((vector_size (sizeof(int)*VECTOR_SIZE)));
     
typedef union i4vector
{
  v4si    v;
  int     i[VECTOR_SIZE];
} i4vector;


void printvector(i4vector vec) {
  printf("%d %d %d %d\n", vec.i[0], vec.i[1], vec.i[2], vec.i[3]);
}

int main()
{

  i4vector a,b,c,r;
  a.v = (v4si){1,2,3,4};
  b.v = (v4si){3,2,1,4};
  c.v = (v4si){5,6,7,8};
  r.v = (a.v * b.v) + c.v;

  printvector(a);
  printvector(b);
  printvector(c);
  printvector(r);


  return  0;
}
