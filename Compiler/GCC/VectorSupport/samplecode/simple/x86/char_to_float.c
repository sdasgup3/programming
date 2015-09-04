/* ************************************************************************* 
* NAME: samples/x86/char_to_float.c
*
* DESCRIPTION:
*
* this demonstrates how to convert a vector of chars into a vector of
* floats by _mm_unpack*-ing with zero and using _mm_cvtpu16_ps1. you
* might do this if your input is in chars, but you need to do floating
* point operations on the data.
*
* PROCESS:
*
* cc -g -o char_to_float char_to_float.c -I.  -msse -mmmx
*
* ./char_to_float
*
* cc -O3 -o char_to_float char_to_float.c -I.  -msse -mmmx
* ./char_to_float
*
* expected output:
*
* $  ./char_to_float
* SSE1 features ARE available
* loshorts 255 254 253 252 
* lofloats 255.000000 254.000000 253.000000 252.000000 
* hishorts 3 2 1 0 
* hifloats 3.000000 2.000000 1.000000 0.000000 
*
*
* GLOBALS: none
*
* REFERENCES:
*
* Intel C++ Intrinsics Reference
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

#include <extract.h> /* _mm_cvtpu16_ps1  */

/* SHORT_ARRAYSIZE - the number of elements we'd need in an  */
/* array of int16_t's to hold an __m64: should be 4.         */

#define SHORT_ARRAYSIZE (sizeof(__m64) / sizeof(int16_t))

/*  FLOAT_ARRAYSIZE - the number of elements we'd need in an array */
/* of floats to hold an __m128; should be 4                        */

#define FLOAT_ARRAYSIZE (sizeof(__m128)/ sizeof(float))


/* prototypes  */
void test_sse1_feature(void);
void test_char_to_float(void);
/* end prototypes  */

int main(int argc, char * argv[])
{


  /* exit if we dn't the processor can't do SSE  */
  
  test_sse1_feature();

  /* now do some vector operations with SSE1 extensions  */

  test_char_to_float();
  
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


   NAME:  test_char_to_float


   USAGE: 

   test_char_to_float();

   returns: void

   DESCRIPTION:
                 show how to turn a vector of chars into a vector of floats.

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   _mm_unpacklo_pi8 - interleave two char vectors, extract low half
   _mm_unpackhi_pi8 - interleave two char vectors, extract hi half
   _mm_store_ps - store __m128 to array of floats
   _mm_empty - reset float registers after MMX operation
   memcpy
   fprintf
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     27-Jan-11               initial coding                           gpk
      5-Feb-11 added  _mm_empty() since we used an SSE1 intrinsic     gpk
                      that used an __m64 type (and generates an
		      MMX instruction)
		      
 ************************************************************************* */

void test_char_to_float(void)
{
  __m64 narrow = _mm_set_pi8(0, 1, 2, 3, 252, 253, 254, 255);
  __m64 zero = _mm_setzero_si64();
  __m64 loshorts, hishorts;
  __m128 lofloats, hifloats;
  float lofloatarray[FLOAT_ARRAYSIZE] __attribute__ ((aligned (16)));
  float hifloatarray[FLOAT_ARRAYSIZE] __attribute__ ((aligned (16)));
  int16_t shortarray[SHORT_ARRAYSIZE] __attribute__ ((aligned (16)));
  int i;
  
  /* interleave zero with narrow and return halves: essentially widening  */
  /* elements from unsigned chars to unsigned shorts                      */
  loshorts = _mm_unpacklo_pi8(narrow, zero);
  hishorts = _mm_unpackhi_pi8(narrow, zero);

  /* now turn the 4 shorts in loshorts into floats and store them in lofloats */
  /* likewise hishorts into hifloats.  */
  /* bug in _mm_cvtpi16_ps ? */
  
  lofloats = _mm_cvtpu16_ps1(loshorts);
  hifloats = _mm_cvtpu16_ps1(hishorts);
  
  _mm_store_ps(lofloatarray, lofloats);
  _mm_store_ps(hifloatarray, hifloats);

  /* we used SSE1 instructions that used __m64: add an   _mm_empty */
  _mm_empty(); 

  /* now store loshorts in shortarray and lofloats into lofloatarray  */
  /* and print them.   */
  memcpy(shortarray, &loshorts, sizeof(shortarray));
  
  fprintf(stderr, "loshorts ");
  for(i= 0; i < SHORT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%d ", shortarray[i]);
    }
  fprintf(stderr, "\n");
  
   
  fprintf(stderr, "lofloats ");
  for(i= 0; i < FLOAT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%f ", lofloatarray[i]);
    }
  fprintf(stderr, "\n");

  /* now store hishorts in shortarray and hifloats into lofloatarray  */
  /* and print them.   */
  memcpy(shortarray, &hishorts, sizeof(shortarray));
  
  fprintf(stderr, "hishorts ");
  for(i= 0; i < SHORT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%d ", shortarray[i]);
    }
  fprintf(stderr, "\n");
  
  fprintf(stderr, "hifloats ");
  for(i= 0; i < FLOAT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%f ", hifloatarray[i]);
    }
  fprintf(stderr, "\n");

	  
}
