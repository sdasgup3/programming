/* ************************************************************************* 
* NAME: altivec_permute.c
*
* DESCRIPTION:
*
* this file covers the permute (vec_perm) operation. if you need to
* unscramble input data from memory and put it into different vector
* registers, this is a good way to do it.
*
* PROCESS:
*
* to compile and run debugging:
* cc -g -o altivec_permute altivec_permute.c -maltivec -mabi=altivec
* ./altivec_permute
*
*  to compile and run optimized:
* cc -O3 -o altivec_permute altivec_permute.c -maltivec -mabi=altivec 
* ./altivec_permute
*
* you can use objdump to see the assembly language that is
*   generated at full opt:
*
* cc -ggdb -O3 -c altivec_permute.c -maltivec -mabi=altivec 
* objdump -dS altivec_permute.o > o3altivec_permute.dump
*
* or at debug:
*
* cc -ggdb -c altivec_permute.c -maltivec -mabi=altivec
* objdump -dS altivec_permute.o > altivec_permute.dump
*
* GLOBALS: none
*
* REFERENCES:
*
* http://developer.apple.com/hardwaredrivers/ve/sse.html#Translation
* GCC info page for PowerPC built-ins
* AltiVec Technology Programming Interface Manual
* Ian Ollmann's Altivec Tutorial
*
* LIMITATIONS:
*
* * test_altivec_feature parses /proc/cpuinfo, which can change format
*   between releases of linux.
*
* REVISION HISTORY:
*   STR                Description                          Author
*  29-Oct-10          initial coding                        gpk
*  16-Apr-11  removed -flax-vector-conversions argument     gpk
*
* TARGET: GNU C version 4 or higher
*
* ************************************************************************ */

/* ===== INCLUDES ===== */

#include <stdio.h>
#include <stdlib.h> /* exit  */
#include <string.h> /* memcpy */

#include <altivec.h>

/* ===== DEFINES ===== */

/*  MAXLINE - the longest line i'll read from /proc/cpuinfo */

#define MAXLINE 255

/* CHAR_ARRAYSIZE - the number of chars in an array it would take */
/* to make it the same size as a char vector                      */

#define CHAR_ARRAYSIZE (sizeof (vector char) / sizeof (char))

/* SHORT_ARRAYSIZE - the number of elements in a short array that  */
/* make it the same size as a short vector                         */
#define SHORT_ARRAYSIZE (sizeof(vector short) / sizeof(short))

/* INT_ARRAYSIZE - the number of elements needed in an int array  */
/* to hold the contents of an int vector   */

#define INT_ARRAYSIZE (sizeof(vector int) / sizeof(int))

/* FLOAT_ARRAYSIZE - the number of elements needed in a float array  */
/* to hold the contents of a float vector   */

#define FLOAT_ARRAYSIZE (sizeof(vector float) / sizeof(float))

/* ===== FUNCTIONS ===== */

/* ************************************************************************* 

   NAME:  test_altivec_feature

   USAGE: 

   test_altivec_feature();

   returns: void

   DESCRIPTION:
                 read the contents of /proc/cpuinfo and look for a
		 line that starts with the the word "cpu". this
		 will list the features on the CPU.

		 if "altivec supported" appears on that line, the processor has
		   the altivec extensions. tell the user

		 if "altivec supported" doesn't appear on that line, 
                   the processor
		   doesn't have altivec extensions. tell the user and exit

   REFERENCES:

   LIMITATIONS:

   * format of /proc/cpuinfo can change between releases, so this is not
     future-proof.
  
   FUNCTIONS CALLED:

   fopen
   perror
   fgets
   strncmp
   strstr
   fprintf
   exit
   
   REVISION HISTORY:
        STR                  Description of Revision                 Author
      7-Feb-10               initial coding                           gpk

 ************************************************************************* */

void test_altivec_feature(void)
{
  FILE * file;
  char cpuinfo[MAXLINE];
  char * astring;
  int foundit;
  
  file = fopen("/proc/cpuinfo", "r");

  if (NULL == file)
    {
      perror("Fatal error: can't read /proc/cpuinfo");
      exit(-1);
    }

  foundit = 0;
      
  astring = fgets(cpuinfo, MAXLINE, file);

  while ((0 == foundit) && (NULL != astring ) )
    {
       if (0 == strncmp("cpu", astring, strlen("cpu")))
	 {
	   /* is this correct ??  */
	   if (NULL != strstr(astring, "altivec supported"))
	     {
	       foundit = 1;
	     }
	 }

       astring = fgets(cpuinfo, MAXLINE, file);
    }

    if (0 == foundit)
      {
	fprintf(stderr, "Feature 'altivec' not found in /proc/cpuinfo.\n");
	exit(-1);
      }
    else
      {
	fprintf(stderr, "Feature 'altivec' IS found in /proc/cpuinfo.\n");
      }
   
} /* test_altivec_feature */

#ifndef __VEC__ /* no altivec instructions allowed */
#error This file uses altivec instructions: are you compiling with
#error  the right flags? ("-maltivec -mabi=altivec")

/* ************************************************************************* 
   NAME:  test_permute
 ************************************************************************* */
void test_permute(void)
{
  fprintf(stderr, "Error: can't generate permute instructions without");
  fprintf(stderr, " an altivec-aware compiler. Did you compile with the ");
  fprintf(stderr, "correct flags?\n");

}

#else

/* ************************************************************************* 

   NAME:  test_permute

   USAGE:
   
   test_permute();

   returns: void

   DESCRIPTION:
                 use the vec_perm() intrinsic to see how it works.

                 take inputvector and zero and permute them using a couple
                 of other vectors:

                 * permuting inputvector and zero with the reverser vector
                   will produce an output that has the components of
                   inputvector in the opposite order

                 * permuting inputvector and zero with the expandlow
                   will produce a new vector with the first 8 elements
                   of input vector interleaved with zero. (expanding
                   an unsigned char into an unsigned short.)

                 * permuting inputvector and zero with the expandhi
                   will produce a new vector with the second 8 elements
                   of input vector interleaved with zero. (expanding
                   an unsigned char into an unsigned short.)

   REFERENCES:

   LIMITATIONS:

   * in real code, use vec_mergeh and vec_mergel instead of vec_perm
     to expand from char to short.
      
   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   vec_st - store vector to array
   fprintf
   vec_perm - build a vector with bytes from 2 sources, controlled by
              a third vector
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author
     30-Oct-10               initial coding                           gpk

 ************************************************************************* */

void test_permute(void)
{
  vector unsigned char inputvector =
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

  /* vec_splat_u8 = 16 element, unsigned char */
  const vector unsigned char zero = vec_splat_u8(0);

  const vector unsigned char reverser =
    { 0xF, 0xE, 0xD, 0xC, 0xB, 0xA, 0x9, 0x8, 0x7, 0x6,
      0x5, 0x4, 0x3, 0x2, 0x1, 0x0};
  const vector unsigned char expandlow =
    {0x10, 0x00, 0x11, 0x01, 0x12, 0x02, 0x13, 0x03, 0x14, 0x04,
     0x15, 0x05, 0x16, 0x06, 0x17, 0x07};
  const vector unsigned char expandhi =
    {0x18, 0x08, 0x19, 0x09, 0x1A, 0x0A, 0x1B, 0x0B, 0x1C, 0x0C,
     0x1D, 0x0D, 0x1E, 0x0E, 0x1F, 0x0f};

  vector unsigned char outputvector;
  vector unsigned short shortoutsvector;
  unsigned char printchar[CHAR_ARRAYSIZE] __attribute__ ((aligned (16)));
  unsigned short printshort[SHORT_ARRAYSIZE] __attribute__ ((aligned (16)));
  int i;

  /* first, print the inputvector  */
  /* vec_st: Make vector accessible for printing */
  vec_st(inputvector, 0, printchar);

  for (i = 0; i < CHAR_ARRAYSIZE; i++)
    {
      fprintf(stderr, "Input vector %d: $%x\n", i, (int)(printchar[i]));
    }

  /* now print the permutation by reverser vector  */
  /* (input vector printed in reverse) */ 
   
  fprintf(stderr,"For permutation by reverser\n");
  outputvector = vec_perm(inputvector, zero, reverser);
  vec_st(outputvector, 0, printchar);

  for (i = 0; i < CHAR_ARRAYSIZE; i++)
    {
      fprintf(stderr, "Output vector %d: $%x\n", i, (int)(printchar[i]));
    }

  /* now print the permutation by expandlo vector */
  /* (low end of input vector alternating with 0s) */ 

  fprintf(stderr,"For permutation by expandlow\n");

  outputvector = vec_perm(inputvector, zero, expandlow);

  /* copy from expanded vector of unsigned chars to vector of */
  /* shorts. make sure we have the byte order correct. */

  /* note: type coercion works as long as we don't change the  */
  /* bits in the values.                                */

  /* needs to be unsigned short: 1/11/11 gpk */
  shortoutsvector = (vector unsigned short)outputvector;

  vec_st(outputvector, 0, printchar);
  vec_st(shortoutsvector, 0, printshort);

  for (i = 0; i < CHAR_ARRAYSIZE; i++)
    {
      fprintf(stderr, "Char output vector %d: $%x\n", i, (int)(printchar[i]));
    }
  for (i = 0; i < SHORT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "short output vector %d: $%x\n", i, printshort[i]);
    }

  /* now the same thing with expandhi  */
  /* (high end of input vector alternating with 0s) */ 

  fprintf(stderr,"For permutation by expandhi\n");

  outputvector = vec_perm(inputvector, zero, expandhi);

  /* copy from expanded vector of unsigned chars to vector of */
  /* shorts. make sure we have the byte order correct. */

  /* needs to be unsigned short - gpk 1/11/11 */
  shortoutsvector = (vector unsigned short)outputvector;

  vec_st(outputvector, 0, printchar);
  vec_st(shortoutsvector, 0, printshort);

  for (i = 0; i < CHAR_ARRAYSIZE; i++)
    {
      fprintf(stderr, "Output vector %d: $%x\n", i, (int)(printchar[i]));
    }
  for (i = 0; i < SHORT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "short output vector %d: $%x\n", i, printshort[i]);
    }

} /* test_permute */
 

#endif /* __VEC__ */


/* ************************************************************************* 
 Name: main
 ************************************************************************* */
int main (int argc, char * argv[])
{
  /* test_altivec_feature will exit if the processor */
  /* can not do altivec */
  test_altivec_feature();

  /* test the permute feature */
  test_permute();

  return(0);
}

