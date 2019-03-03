#include <stdio.h>
#include <stdlib.h>
typedef union {
  int i;
  char c[4];

} intchar;

main() {
  char *c = calloc(4, sizeof(char));
  c[0] = 0;
  c[1] = 1;
  c[2] = 2;
  c[3] = 3;
  printf("%d\n", *(int *)c); // 2^8 + 2^17 + 2^24 + 2^25 = 50462976

  // Little Indian
  intchar C;
  C.c[0] = 0; // 00000000
  C.c[1] = 0; // 00000000
  C.c[2] = 0; // 00000000
  C.c[3] = 3; // 00000011

  printf("%d\n",
         C.i); // 50331648 = (00000011) + (00000000) + (00000000) +  (00000000)

  intchar C1;
  C1.c[0] = 10; // 00000000
  C1.c[1] = 0;  // 00000000
  C1.c[2] = 0;  // 00000000
  C1.c[3] = 0;  // 00000011

  printf("%d\n",
         C1.i); // 10 = (00000000) + (00000000) + (00000000) +  (00001010)

  char *p = malloc(4 * sizeof(char));
  *(int *)p = 10; // hihger address-->  ff|ff|ff|fA   <-- lower address
  printf("%d [%c]", *(int *)p, *p); // c[0] == fA .. \n charcter

  return 0;
}
