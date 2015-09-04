/* ************************************************************************* 
* NAME: samples/x86/widen.c
*
* DESCRIPTION:
*
* this demonstrates how to widen a vector of chars into a vector of
* shorts by _mm_unpack*-ing with zero. (you might do this if your input
* data is in packed bytes, but you need to work with it in a wider type.)
*
* PROCESS:
*
* cc -g -o widen widen.c  -msse -mmmx  OR
* cc -O3  -o widen widen.c  -msse -mmmx
*
* ./widen
*
* expected output:
*
* $ ./widen
* SSE1 features ARE available
* hi is 3 2 1 0 
* lo is 255 254 253 252 
* adding 10 to each value to overflow old type int8_t.
* if we get the right answers, we're in int16_t now
* hi + 10 is 13 12 11 10 
* lo + 10 is 265 264 263 262 
*
*
* GLOBALS: none
*
* REFERENCES:
*
* LIMITATIONS:
*
* REVISION HISTORY:
*
*   STR                Description                          Author
*
*   20-Jan-11          initial coding                        gpk
*
* TARGET: GCC version 4 or higher on X86
*
* The contents of this file is in the public domain. If it breaks, you
* get to keep both pieces.
*
* ************************************************************************* */

#include <stdio.h>
#include <stdlib.h> /* exit  */
#include <string.h> /* memcpy */
#include <stdint.h> /* int8_t, int16_t, int32_t  */
#include <cpuid.h> /* __get_cpuid_max, __get_cpuid */
#include <mmintrin.h> /* MMX instrinsics  __m64 integer type  */
#include <xmmintrin.h> /* sse intrinsics */

/* SHORT_ARRAYSIZE - the number of elements we'd need in an  */
/* array of int16_t's to hold an __m64: should be 4.         */

#define SHORT_ARRAYSIZE (sizeof(__m64) / sizeof(int16_t))

/*  FLOAT_ARRAYSIZE - the number of elements we'd need in an array */
/* of floats to hold an __m128; should be 4                        */

#define FLOAT_ARRAYSIZE (sizeof(__m128)/ sizeof(float))


/* prototypes  */
void test_sse1_feature(void);
void test_widen(void);

/* end prototypes  */

int main(int argc, char * argv[])
{


  /* exit if we dn't the processor can't do SSE  */
  
  test_sse1_feature();

  /* now do some vector operations with SSE1 extensions  */
  
  test_widen();
  
  return(0);

}

/* FUNC_FEATURES - return the list of intel-defined features in the  */
/* __get_cpuid function.   */

#define FUNC_FEATURES 0x1


/* ************************************************************************* 


   NAME:  test_sse1_feature


   USAGE:

   test_sse1_feature();

   returns: void

   DESCRIPTION:
                 use the __get_cpuid function found on X86 to see if
		 this processor has the SSE feature. if it doesn't,
		 complain and exit.

		 (The feature is called "SSE", I'm referring to it as
		 SSE1 to avoid confusion with SSE2, SSE3, etc)
		 
   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   __get_cpuid
   fprintf
   exit
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     17-Jan-10               initial coding                           gpk

 ************************************************************************* */


void test_sse1_feature(void)
{
  unsigned int eax, ebx, ecx, edx;
  unsigned int extensions, sig;
  int result, sse1_available;
  
  /* call __get_cpuid: there will be bits set in ecx, edx for  */
  /* the intel-defined SSE1, SSEn features.                     */
  
  result = __get_cpuid (FUNC_FEATURES, &eax, &ebx, &ecx, &edx);

  if (-1 == result)
    {
      fprintf(stderr, "Fatal Error: can't get CPU features\n");
      exit(-1);
    }
  else
    {
      sse1_available =  (bit_SSE & edx);

      if (0 == sse1_available)
	{
	  fprintf(stderr, "Error: SSE1 features not available\n");
	  fprintf(stderr, "Had this been an actual program, we'd fall ");
	  fprintf(stderr, "back to a non-SSE1 implementation\n");
	  exit(-1);
	}
      else
	{
	  fprintf(stderr, "SSE1 features ARE available\n");
	}
    }
}
    



/* ************************************************************************* 


   NAME:  test_widen


   USAGE: 

   test_widen();

   returns: void

   DESCRIPTION:
                 show how to widen an array of chars into an array of
		 unsigned shorts by _mm_unpack*_pi8 with zero.

		 after creating the vectors of shorts, add 10  to each
		 of their elements. if they were still chars, they'd
		 overflow. if shorts, we get the right answer.

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   _mm_set_pi16 - set a vector of int16_t's
   _mm_set_pi8 - set a vector of chars
   _mm_setzero_si6 - set a vector of zero
   _mm_unpackhi_pi8 - merge vectors of chars, return hi half
   _mm_unpacklo_pi8 - merge vectors of chars, return lo half
   memcpy
   fprintf
   _mm_add_pi16 - add two vectors of int16_t's
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     27-Jan-11               initial coding                           gpk

 ************************************************************************* */

void test_widen(void)
{
  __m64 ten =_mm_set_pi16(10, 10, 10, 10);
  __m64 narrow =_mm_set_pi8(0, 1, 2, 3, 252, 253, 254, 255);
  __m64 hi, lo;
  __m64 zero = _mm_setzero_si64();
  int16_t printarray[SHORT_ARRAYSIZE]  __attribute__ ((aligned (16)));
  int i;

  /* hi will contain 3 2 1 0  */
  /* lo will contain 255 254 253 252   */
  hi = _mm_unpackhi_pi8(narrow, zero);
  lo = _mm_unpacklo_pi8(narrow, zero);

  memcpy(printarray, &hi, sizeof(printarray));
  fprintf(stderr, "hi is ");
  for(i = 0; i < SHORT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%hd ", printarray[i]);
    }
  fprintf(stderr, "\n");
  
  memcpy(printarray, &lo, sizeof(printarray));
 fprintf(stderr, "lo is ");
  for(i = 0; i < SHORT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%hd ", printarray[i]);
    }
  fprintf(stderr, "\n");
  fprintf(stderr, "adding 10 to each value to overflow old type int8_t.\n");
  fprintf(stderr, "if we get the right answers, we're in int16_t now\n");

  hi = _mm_add_pi16(hi, ten);
  lo = _mm_add_pi16(lo, ten);
  
  memcpy(printarray, &hi, sizeof(printarray));
  fprintf(stderr, "hi + 10 is ");
  
  for(i = 0; i < SHORT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%hd ", printarray[i]);
    }
  fprintf(stderr, "\n");
  
  memcpy(printarray, &lo, sizeof(printarray));
  fprintf(stderr, "lo + 10 is ");
  for(i = 0; i < SHORT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%hd ", printarray[i]);
    }
  fprintf(stderr, "\n");

 
}

