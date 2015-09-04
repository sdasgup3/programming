/* ************************************************************************* 
* NAME: x86/temperatures_sse.c
*
* DESCRIPTION:
* this is an example program using SSE to convert an array of
* temperatures in degrees Celsius into degrees Fahrenheit.
*
* this is much like the  temperatures.c program that used the gcc
* vector extensions, except that it uses the SSE extensions. (if you
* have SSE extensions, you should also have MMX as well.)
*
* SSE(1) adds with 128-bit float vectors.
*
* this example uses 4 32-bit floats
*
* this applies the formula:
*
* f = (9/5)C + 32 = 1.8 * C + 32
*
* to elements of a vector of celsius temperatures {c0, c1, c2, c 3}
*
* the easiest way to visualize this is "vertical math". the elements
* of the vectors are operated on simultaneously. so if you had a vector
* of celsius temperatures {c0, c1, c2, c3}:
*
*    { c0    c1     c2     c3 }       --- celsius temperature vector
*  * { 1.8   1.8    1.8    1.8}       --- coeff vector
*  ----------------------------
*    { p0    p1     p2     p3 }        --- partial result vector
*  + { 32    32     32     32 }        -- add thirtytwovector (all 32's)
*  ----------------------------
*    { f0    f1     f2     f3 }       -- fahrenheit temperature vector 
*
* execution goes top down, with operations on all elements in a row happening
* at the same time. eg all c0 to c3 get multiplied by 1.8 at the same time.
*
* that's the "vertical math" part.
*
* unlike MMX, SSE does floating point numbers.
*
*
* PROCESS:
*
* cc -g -o temperatures_sse temperatures_sse.c -msse
* ./temperatures_sse
*
* also test at -O3
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
*   23-Jan-10          initial coding                        gpk
*    8-Jan-11 added header block to test_sse1                gpk
*
* TARGET: GNU C version 4 or higher
*
* This file is in the public domain. If it breaks, you get to keep both
* pieces.
*
* ************************************************************************* */

#include <stdio.h>
#include <stdlib.h> /* exit  */
#include <string.h> /* memcpy */
#include <cpuid.h> /* __get_cpuid_max, __get_cpuid */
#include <mmintrin.h> /* MMX instrinsics  __m64 integer type  */
#include <xmmintrin.h> /* SSE  __m128  float */



void test_sse1_feature(void);
void test_sse1(void);

/*  FLOAT_ARRAYSIZE - the number of elements we'd need in an array */
/* of floats to hold an __m128; should be 4                        */

#define FLOAT_ARRAYSIZE (sizeof(__m128)/ sizeof(float))


int main (int argc, char * argv[])
{


  /* exit if we dn't the processor can't do SSE  */
  
  test_sse1_feature();

  /* now do some vector operations with SSE1 extensions  */
  
  test_sse1();
  
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


   NAME:  test_sse1


   USAGE: 


   test_sse1();

   returns: void

   DESCRIPTION:
                 this implements the celsius to fahrenheit conversion
		 described in the file header.

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   _mm_set1_ps - set all elements of a floating point vector to given value
   _mm_load_ps - load vector from memory
   _mm_mul_ps - return product of two floating point vectors
   _mm_add_ps - return sum of two floating point vectors
   _mm_store_ps - store vector to memory
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

      8-Jan-11               initial coding                           gpk

 ************************************************************************* */

void test_sse1(void)
{
  __m128 celsiusvector, fahrenheitvector, partialvector;
  __m128 coeffvector;
  const __m128 thirtytwovector = {32.0, 32.0, 32.0, 32.0};
  float celsiusarray1[]  __attribute__ ((aligned (16)))
    ={-100.0, -80.0, -40.0, 0.0};
  float celsiusarray2[]  __attribute__ ((aligned (16)))
    ={  32.0,  40.0,  80.0, 100.0};
  float fahrenheitarray[FLOAT_ARRAYSIZE] __attribute__ ((aligned (16)));
  int i;

  /* set coeffvector to have all elements equal to 9.0/5.0 */
  
  coeffvector = _mm_set1_ps(9.0 / 5.0); 

  /* pull celsiusarray1 into celsiusvector  */

  /* check alignment issues  */
  /*  celsiusvector = *((__m128 *)celsiusarray1); */
  celsiusvector = _mm_load_ps(celsiusarray1);

  /* set partialvector as the product of 9/5 * C  */
  
  partialvector = _mm_mul_ps(celsiusvector, coeffvector);

  /* add 32 to all elements of partialvector to get fahrenheitvector  */
  
  fahrenheitvector =  _mm_add_ps(partialvector, thirtytwovector);

  /* copy fahrenheitvector into fahrenheitarray  */
  
  /* memcpy(fahrenheitarray, &fahrenheitvector, sizeof(fahrenheitarray)); */
  _mm_store_ps(fahrenheitarray, fahrenheitvector);

  /* now print out the results.  */
  
  for (i = 0; i < FLOAT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%f celsius is %f fahrenheit\n",
	      celsiusarray1[i], fahrenheitarray[i]);
    }


  /* pull celsiusarray2 into celsiusvector  */
  /* and repeat the steps above...  */
  
  /*  celsiusvector = *((__m128 *)celsiusarray2); */
  celsiusvector = _mm_load_ps(celsiusarray2);
  
  partialvector = _mm_mul_ps(celsiusvector, coeffvector);
  fahrenheitvector =  _mm_add_ps(partialvector, thirtytwovector);

  /*  memcpy(fahrenheitarray, &fahrenheitvector, sizeof(fahrenheitarray)); */
  _mm_store_ps(fahrenheitarray, fahrenheitvector);
  
  /* now print out the results.  */
  
  for (i = 0; i < FLOAT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%f celsius is %f fahrenheit\n",
	      celsiusarray2[i], fahrenheitarray[i]);
    }

}

#if 0

expected output:
---------------

$ ./temperatures_sse
SSE1 features ARE available
-100.000000 celsius is -148.000000 fahrenheit
-80.000000 celsius is -112.000000 fahrenheit
-40.000000 celsius is -40.000000 fahrenheit
0.000000 celsius is 32.000000 fahrenheit
32.000000 celsius is 89.599998 fahrenheit
40.000000 celsius is 104.000000 fahrenheit
80.000000 celsius is 176.000000 fahrenheit
100.000000 celsius is 212.000000 fahrenheit

#endif /* 0  */
