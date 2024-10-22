#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG 0


typedef double v2df __attribute__ ((vector_size(16))); /* vector of two doubles */
typedef int v4si __attribute__ ((vector_size(16))); /* vector of four ints */


const v2df zero = { 0.0, 0.0 };
const v2df four = { 4.0, 4.0 };

/*
 * Constant throughout the program, value depends on N
 */
int bytes_per_row;
double inverse_w;
double inverse_h;

/*
 * Program argument: height and width of the image
 */
int N;

/*
 * Lookup table for initial real-axis value
 */
v2df *Crvs;

/*
 * Mandelbrot bitmap
 */
uint8_t *bitmap;

typedef union 
{
  v2df    v;
  double  f[2];
} f2vec;

void printv2df(v2df v) {
  f2vec vec;
  vec.v = v;

  printf("%f %f\n", vec.f[0], vec.f[1]);

}

static void calc_row(int y) {
    uint8_t *row_bitmap = bitmap + (bytes_per_row * y);
    int x;
    const v2df Civ_init = { y*inverse_h-1.0, y*inverse_h-1.0 };

    for (x=0; x<N; x+=2)
    {
        v2df Crv = Crvs[x >> 1];
        v2df Civ = Civ_init;
        v2df Zrv = zero;
        v2df Ziv = zero;
        v2df Trv = zero;
        v2df Tiv = zero;
        int i = 50;
        int two_pixels;
        v2df is_still_bounded;

        do {
            Ziv = (Zrv*Ziv) + (Zrv*Ziv) + Civ;
            Zrv = Trv - Tiv + Crv;
            Trv = Zrv * Zrv;
            Tiv = Ziv * Ziv;

            /*
             * All bits will be set to 1 if 'Trv + Tiv' is less than 4
             * and all bits will be set to 0 otherwise. Two elements
             * are calculated in parallel here.
             */
            is_still_bounded = __builtin_ia32_cmplepd(Trv + Tiv, four);
#if DEBUG 
            printv2df(is_still_bounded);
#endif

            /*
             * Move the sign-bit of the low element to bit 0, move the
             * sign-bit of the high element to bit 1. The result is
             * that the pixel will be set if the calculation was
             * bounded.
             */
            two_pixels = __builtin_ia32_movmskpd(is_still_bounded);
#if DEBUG 
            printf("%d\n\n",two_pixels);
#endif
        } while (--i > 0 && two_pixels);

        /*
         * The pixel bits must be in the most and second most
         * significant position
         */
        two_pixels <<= 6;

        /*
         * Add the two pixels to the bitmap, all bits are
         * initially zero since the area was allocated with
         * calloc()
         */
        row_bitmap[x >> 3] |= (uint8_t) (two_pixels >> (x & 7));
    }
}

int main (int argc, char **argv)
{
    ///gcc -pipe -Wall -O3 -fomit-frame-pointer -march=native -std=c99 -D_GNU_SOURCE -mfpmath=sse -msse2 -fopenmp
    int i;

    N = atoi(argv[1]);
    bytes_per_row = (N + 7) >> 3;

    inverse_w = 2.0 / (bytes_per_row << 3);
    inverse_h = 2.0 / N;

    /*
     * Crvs must be 16-bytes aligned on some CPU:s.
     */
    if (posix_memalign((void**)&Crvs, sizeof(v2df), sizeof(v2df) * N / 2))
        return EXIT_FAILURE;

    #pragma omp parallel for
    for (i = 0; i < N; i+=2) {
        v2df Crv = { (i+1.0)*inverse_w-1.5, (i)*inverse_w-1.5 };
        Crvs[i >> 1] = Crv;
    }

#if DEBUG 
    for (i = 0; i < N/2; i+=1) {
        printv2df(Crvs[i]);
    }
#endif


    bitmap = calloc(bytes_per_row, N);

    if (bitmap == NULL)
        return EXIT_FAILURE;

    #pragma omp parallel for schedule(static,1)
    for (i = 0; i < N; i++)
        calc_row(i);

#if DEBUG 
    for(int i = 0; i < bytes_per_row*N; i++) {
      uint8_t* next = bitmap + i;
      fprintf(stderr, "%d ", *next);
    }
    fprintf(stderr, "\n ");
#endif


    printf("P4\n%d %d\n", N, N);
    fwrite(bitmap, bytes_per_row, N, stdout);

    free(bitmap);
    free(Crvs);

    return EXIT_SUCCESS;
}
