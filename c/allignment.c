#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void main() {

  typedef struct {
    char a;  // 1 byte
    int b;   // 4 bytes
    long c;  // 8 bytes
    short d; // 2 bytes
    char e;  // 1 byte
  } MyStruct;

  char C;
  long L;
  char C2;

  long *LP = &L;
  char *CP = &C;
  char *C2P = &C2;

  printf("char a   (%ld) offset: %ld\n", sizeof(char), offsetof(MyStruct, a));
  printf("int b    (%ld) offset: %ld\n", sizeof(int), offsetof(MyStruct, b));
  printf("long c   (%ld) offset: %ld\n", sizeof(long), offsetof(MyStruct, c));
  printf("shart d  (%ld) offset: %ld\n", sizeof(short), offsetof(MyStruct, d));
  printf("char e   (%ld) offset: %ld\n", sizeof(char), offsetof(MyStruct, e));
  printf("main C    (%ld) offset:  %x\n", sizeof(char), (unsigned *)CP);
  printf("main L    (%ld) offset:  %x\n", sizeof(long), (unsigned *)LP);
  printf("main C2    (%ld) offset: %x\n", sizeof(char), (unsigned *)C2P);
}
