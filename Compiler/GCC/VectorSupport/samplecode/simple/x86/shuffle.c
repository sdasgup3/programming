/* ************************************************************************* 
* NAME: /home/gpk/papers/samples/x86/shuffle.c
*
* DESCRIPTION:
*
* this demonstrates how to use the shuffle intrinsic produce a vector
* by rearranging words in a source vector. you might do this as part of
* dealing with non-uniform vectors: load your source data from memory
* into a vector register, then rearrange the data and spread it from
* into other vector registers.
*
* PROCESS:
*
* cc -g -o shuffle shuffle.c  -msse
*
* ./shuffle
*
* expected output:
*
*$ ./shuffle
* SSE1 features ARE available
* source 4 3 2 1 
* shuffled 1 2 3 4 
* test_shuffle_float
* 4.400000 3.300000 20.200001 10.100000
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
void test_shuffle(void);
void test_shuffle_float(void);
/* end prototypes  */

int main(int argc, char * argv[])
{


  /* exit if the processor can't do SSE  */
  
  test_sse1_feature();

  /* now do some vector operations with SSE1 extensions  */
  
  test_shuffle();

  test_shuffle_float();
  
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


   NAME:  test_shuffle


   USAGE: 

   test_shuffle();

   returns: void

   DESCRIPTION:
                 demonstrate the _mm_shuffle_pi16 instruction: make
		 a vector by rearranging elements of a second vector,
		 as indicated by a list of compile-time constants
		 

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   memcpy
   fprintf
   _mm_shuffle_pi16 - rearrange elements of an __m64
   
   
   REVISION HISTORY

        STR                  Description of Revision                 Author

      2-Apr-11               initial coding                           gpk

 ************************************************************************* */

void test_shuffle(void)
{
  __m64 source =_mm_set_pi16( 1, 2, 3, 4); /* 4 16-bit integers  */
  __m64 dest;
  int16_t shortarray[SHORT_ARRAYSIZE] __attribute__ ((aligned (16))); 
  int i;
  
  memcpy(shortarray, &source, sizeof(shortarray));
  
  fprintf(stderr, "source ");
   for (i = 0; i < SHORT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%d ", shortarray[i]);
    }
   
  fprintf(stderr, "\nshuffled ");

  /* _MM_SHUFFLE is a macro that produces an integer that tells  */
  /* how to rearrange the words in the vector given to _mm_shuffle.  */
  /* the arguments given to _MM_SHUFFLE are the "indices" of the  */
  /* words within the source vector. note that the second argument to  */
  /* _mm_shuffle must be a compile-time constant. you can't use a   */
  /* variable here.   */
  /* shuffling source vector with 0, 1, 2, 3  will reverse it  */
  dest = _mm_shuffle_pi16(source, _MM_SHUFFLE(0, 1, 2, 3));
  
  memcpy(shortarray, &dest, sizeof(shortarray));
  
  for (i = 0; i < SHORT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%d ", shortarray[i]);
    }
  fprintf(stderr, "\n");
     
}



/* ************************************************************************* 


   NAME:  test_shuffle_float


   USAGE: 

    
   void;

   test_shuffle_float();

   returns: void

   DESCRIPTION:
                 show how to use the _mm_shuffle_ps intrinsic to
		 rearrange elements of a floating point vector
		 to build another floating point vector.
		 

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   fprintf
   _mm_shuffle_ps - rearrange a float vector to make another vector
   _mm_store_ps - store a float vector into memory
   _mm_empty - restore floating MMX registers
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

      2-Apr-11               initial coding                           gpk

 ************************************************************************* */

void test_shuffle_float(void)
{
  __m128 source0 = {1.1, 2.2, 3.3, 4.4};
  __m128 source1 = {10.1, 20.2, 30.3, 40.4};
  __m128 dest;
  float printarray[FLOAT_ARRAYSIZE] __attribute__ ((aligned (16)));
  int i;
  
  fprintf(stderr, " %s\n", __FUNCTION__);

  /* in _mm_shuffle_ps you're trying to place 4 floats into an __m128  */
  /* the first two floats come from the first argument, the second two  */
  /* come from the second argument.   */

  /* indices start at zero. the first argument to _MM_SHUFFLE selects   */
  /* the last element of the output.   */
  
  /* the first two indices to _MM_SHUFFLE control which elements are   */
  /* selected from the first argument.   */
  
  dest = _mm_shuffle_ps(source0, source1, _MM_SHUFFLE(0, 1, 2, 3));
 
  _mm_store_ps(printarray, dest);

  /* need the _mm_empty here.  */
  _mm_empty();

   for (i = 0; i < FLOAT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%f ", printarray[i]);
    }
  fprintf(stderr, "\n");
}


#if 0

_MM_SHUFFLE(0, 1, 2, 3) --> 4.400000 3.300000 20.200001 10.100000
_MM_SHUFFLE(0, 0, 0, 0) --> 1.100000 1.100000 10.100000 10.100000
_MM_SHUFFLE(1, 1, 1, 1) --> 2.200000 2.200000 20.200001 20.200001
_MM_SHUFFLE(2, 3, 3, 2) --> 3.300000 4.400000 40.400002 30.299999

  Note that the first argument to _MM_SHUFFLE selects the last element
of the vector and so on.

#endif /* 0  */

