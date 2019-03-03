#include <stdio.h>
#include <stdlib.h>

extern void hexdump(FILE *stream, void const *data, unsigned int len) {
  unsigned int i;
  unsigned int r, c;

  if (!stream)
    return;
  if (!data)
    return;

  for (r = 0, i = 0; r < (len / 16 + (len % 16 != 0)); r++, i += 16) {
    fprintf(stream, "%04X:   ", i); /* location of first byte in line */

    for (c = i; c < i + 8; c++) /* left half of hex dump */
      if (c < len)
        fprintf(stream, "%02X ", ((unsigned char const *)data)[c]);
      else
        fprintf(stream, "   "); /* pad if short line */

    fprintf(stream, "  ");

    for (c = i + 8; c < i + 16; c++) /* right half of hex dump */
      if (c < len)
        fprintf(stream, "%02X ", ((unsigned char const *)data)[c]);
      else
        fprintf(stream, "   "); /* pad if short line */

    fprintf(stream, "   ");

    for (c = i; c < i + 16; c++) /* ASCII dump */
      if (c < len)
        if (((unsigned char const *)data)[c] >= 32 &&
            ((unsigned char const *)data)[c] < 127)
          fprintf(stream, "%c", ((char const *)data)[c]);
        else
          fprintf(stream, "."); /* put this for non-printables */
      else
        fprintf(stream, " "); /* pad if short line */

    fprintf(stream, "\n");
  }

  fflush(stream);
}

int main() {
  char *d = malloc(10 * sizeof(*d));
  d[0] = 'A';
  d[1] = 'B';
  d[2] = 'C';
  d[4] = 'D';
  d[5] = 'E';
  d[6] = 'F';
  d[7] = 'G';
  d[8] = 'H';
  d[9] = 'X';

  hexdump(stdout, d, ((char *)(d + 9) - (char *)d) + 1);
}
