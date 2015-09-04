/* ************************************************************************* 
* NAME: samples/x86/float_to_char.c
*
* DESCRIPTION:
*
* this demonstrates how to convert a vector of floats into a vector of
* chars using _mm_cvtps_pi16 to turn floats to shorts, followed by
* _mm_packs_pu16 to pack the shorts into chars. you
* might do something like this if you had floating point data for your
* vector work, but needed the output to be in bytes.
*
* PROCESS:
*
* cc -g -o float_to_char float_to_char.c   -msse -mmmx
*
* ./float_to_char
*
* cc -O3 -o float_to_char float_to_char.c   -msse -mmmx
* ./float_to_char
*
* expected output:
*
* $  ./float_to_char
* 
* SSE1 features ARE available
* stored 16 bytes: 1 2 3 4 10 20 30 40 21 22 23 24 31 32 33 34
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




/* prototypes  */
void test_sse1_feature(void);
void test_float_to_char(void);
/* end prototypes  */

int main(int argc, char * argv[])
{


  /* exit if we dn't the processor can't do SSE  */
  
  test_sse1_feature();

  /* now do some vector operations with SSE1 extensions  */

  test_float_to_char();
  
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


   NAME:  test_float_to_char


   USAGE: 

   test_float_to_char();

   returns: void

   DESCRIPTION:
                 show how to turn a vector of floats into a vector of chars.
		 (useful when the results of floating point operations
		 have to be stored back as chars.)
   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   _mm_cvtps_pi16 - convert floats in __m128 to shorts in __m64
   _mm_packs_pu16 - pack shorts into chars with saturation
   _mm_loadl_pi - load an __m64 into lower half of an __m128
   _mm_loadh_pi - load an __m64 into upper half of an __m128
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     27-Jan-11               initial coding                           gpk

 ************************************************************************* */

void test_float_to_char(void)
{
  __m128 float0 = {1.1, 2.2, 3.3, 4.4};
  __m128 float1 = {10.1, 20.2, 30.3, 40.4};
  __m128 float2 = {21.1, 22.2, 23.3, 24.4};
  __m128 float3 = {31.1, 32.2, 33.3, 34.4};
  __m64 shortvec0, shortvec1, shortvec2, shortvec3;
  __m64 fullvec0, fullvec1;
  __m128 resultvec;
  unsigned char output[sizeof(__m128)];
  unsigned char * destp;
  int i;
  
  /* store floatN into  shortvecN  */
  
  shortvec0 = _mm_cvtps_pi16(float0);
  shortvec1 = _mm_cvtps_pi16(float1);
  shortvec2 = _mm_cvtps_pi16(float2);
  shortvec3 = _mm_cvtps_pi16(float3);

  /* pack shortvec0 into the lower four 8-bit values of fullvec0   */
  /* with unsigned saturation. pack shortvec1 into the shortvec1   */
  /* into the upper  four 8-bit values of fullvec0 with unsigned   */
  /* saturation. likewise put shortvec2, shortvec3 into fullvec1   */
  
  fullvec0 = _mm_packs_pu16(shortvec0, shortvec1);
  fullvec1 = _mm_packs_pu16(shortvec2, shortvec3);

  destp = output;


  /* now build resultvec by putting fullvec0 into the lower half  */
  /* and fullvec1 into the upper half. documentation describes    */
  /* _mm_loadl_pi in terms of floats, but prototypes don't make   */
  /* the distinction between floating point values in the vectors */
  /* or fixed point values...  */
  
  resultvec = _mm_loadl_pi(resultvec, &fullvec0);
  resultvec = _mm_loadh_pi(resultvec, &fullvec1);

  memcpy(destp, &resultvec, sizeof(resultvec));

  
  /* if fprintf did any floating point we'd include an  _mm_empty  */
  /* _mm_empty(); */

  fprintf(stderr,"stored %d bytes: ", sizeof(__m128));
  for(i = 0; i < sizeof(__m128); i++)
    {
      fprintf(stderr, "%d ", output[i]);
    }
  fprintf(stderr,"\n");
	  
}
