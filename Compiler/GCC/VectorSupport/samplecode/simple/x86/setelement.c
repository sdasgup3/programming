/* ************************************************************************* 
* NAME: samples/x86/setelement.c
*
* DESCRIPTION:
*
* this tries to implement a something like the altivec "vec_sel" function
* using sse instructions _mm_and_ps, _mm_andnot_ps, _mm_or_ps.
* it succeeds under -g, fails under -O3 (gcc bug 47825)
*
* PROCESS:
*
* cc -g -o setelement setelement.c  -msse -mmmx  -pedantic -Wall \
     -fno-strict-aliasing
*
* ./setelement
*
* cc -O3  -o setelement setelement.c  -msse -mmmx -pedantic -Wall \
     -fno-strict-aliasing
*
* expected output:
*
* $ ./setelement
* SSE1 features ARE available
* -1.100000 0.000000 1.100000 255.000000
*
* GLOBALS: none
*
* REFERENCES:
*
* this is gcc bugzilla number 47825
*
* LIMITATIONS:
*
* REVISION HISTORY:
*
*   STR                Description                          Author
*
*  20-Feb-11          initial coding                        gpk
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

#define INT_ARRAYSIZE (sizeof(__m128)/ sizeof(int))

/* prototypes  */
void test_sse1_feature(void);
void test_setelement(void);
__m128 basic_sel_ps( __m128  a, __m128 b,  __m128 mask );

/* end prototypes  */

int main(int argc, char * argv[])
{


  /* exit if we dn't the processor can't do SSE  */
  
  test_sse1_feature();

  /* now do some vector operations with SSE1 extensions  */
  
  test_setelement();
  
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
  int result, sse1_available;
  
  /* call __get_cpuid: there will be bits set in ecx, edx for  */
  /* the intel-defined SSE1, SSEn features.                     */

  edx = 0;
  
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


   NAME:  basic_sel_ps


   USAGE: 

   __m128 result;
   __m128  a; - a source
   __m128 b; - another source
   __m128 mask; - 0-bits here will pull bits from a; 1-bits here pull from b

   result =  basic_sel_ps(a, b, mask);

   returns: __m128

   DESCRIPTION:
                 select bits from a or b under control of mask.

   REFERENCES:

   LIMITATIONS:

   works with -g, fails -O3
   
   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   _mm_and_ps
   _mm_andnot_ps
   _mm_or_ps
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     20-Feb-11               initial coding                           gpk

 ************************************************************************* */

__m128 basic_sel_ps( __m128  a, __m128 b,  __m128 mask )
{
  __m128 bmask, amask, result;

  /* get the part from b  */
  bmask = _mm_and_ps( b, mask );
  /* get the part from a  */
  amask = _mm_andnot_ps( mask, a );
  /* put the parts together  */
  result = _mm_or_ps( amask, bmask );

  return(result);
}
  
  
/* ************************************************************************* 


   NAME:  test_setelement


   USAGE: 

   test_setelement();

   returns: void

   DESCRIPTION:

   build result vector from asource and bsource under control of
   maskarray.
   
   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   _mm_load_ps 
   basic_sel_ps
   _mm_store_ps
   _mm_empty
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     27-Jan-11               initial coding                           gpk

 ************************************************************************* */
  
void test_setelement(void)
{
  int maskarray[INT_ARRAYSIZE] __attribute__ ((aligned (16))) = {0, 0, 0, ~0 };
  __m128 asource = {-1.1, 0.0, 1.1, 1.5};
  __m128 bsource = {0.0, 0.0, 0.0, 255.0};
  __m128 result;
  /* __m128 selectmask = {0, 0, 0, ~0}; */ /* doesn't work  */ 
  __m128 selectmask;
  float floatmask[FLOAT_ARRAYSIZE] __attribute__ ((aligned (16)));
  float floatarray[FLOAT_ARRAYSIZE] __attribute__ ((aligned (16)));
  int i;

  memcpy(floatmask, maskarray, sizeof(floatmask));
  
  selectmask = _mm_load_ps(floatmask);
  result = basic_sel_ps(asource, bsource, selectmask);
  _mm_store_ps(floatarray, result);
  
/* need the _mm_empty here?  */
 _mm_empty();
 
  for (i = 0; i < FLOAT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%f ", floatarray[i]);
    }
  fprintf(stderr, "\n");

}

