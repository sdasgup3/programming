/* ************************************************************************* 
* NAME: x86/temperatures_mmx.c
*
* DESCRIPTION:
*
* this is an example program using MMX to convert an array of
* temperatures in degrees Celsius into degrees Fahrenheit.
*
* this is much like the  temperatures.c program that used the gcc
* vector extensions, except that it uses the MMX extensions.
*
* MMX only works with integers, but uses the floating point registers to
* do it. (so don't interleave MMX and floating point operations)
*
* MMX (by itself) lets you work with 64-bit integer quantities. you have
* the choice of how to divide up the 64 bits:
*
* * 8 8-bit integers
* * 4 16-bit integers
* * 2 32-bit integers
* * 1 64 bit integer
*
* (when you get to SSE2 extensions you get 128-bit integers).
*
* this example uses 4 16 bit integers.
*
* this applies the formula:
*
* f = (9/5)C + 32 = (9 * C) / 5 + 32
*
* to elements of a vector of celsius temperatures {c0, c1, c2, c 3}
*
* the easiest way to visualize this is "vertical math". the elements
* of the vectors are operated on simultaneously. so if you had a vector
* of celsius temperatures {c0, c1, c2, c3}:
*
*    { c0    c1     c2     c3 }       --- celsius temperature vector
*  * {  9     9      9      9 }       --- multiply by vector1 (all 9's)
*  --------------------------       
*    { p0    p1     p2     p3 }        --- partial result vector
*  / {  5      5     5      5 }        -- divide by vector2 (all 5's)
* --------------------------
*    { p0    p1     p2     p3 }       -- partial result
* +  { 32    32     32     32 }        -- add vector3 (all 32's)
* --------------------------
*    { f0    f1     f2     f3 }       -- fahrenheit temperature vector 
*
* execution goes top down, with operations on all elements in a row happening
* at the same time. eg all c0 to c3 get multiplied by 9 at the same time.
*
* that's the "vertical math" part.
*
* the other catch is that MMX by itself doesn't have a division operator.
* so no floats, no division. if we want to divide by a power of two, we can
* use a shift operation instead. so:
* 
* f = (9/5)C + 32 = 1.8 * C + 32
*
* so we'll approximate 1.8 * C as:
*
*  (1.8 * C * 64)     (1.8 * 64) * C      115.2 * C           115 * C
*  -------------  =   --------------  =  ----------  is about -------
*        64                64               64                   64
*
* so we'll use a six-bit right shift for the division by 64.
* so the implementation is:
*
* f = ((115 * C) >> 6) + 32
*
*    { c0    c1     c2     c3 }       --- celsius temperature vector
*  * { 115   115    115    115 }       --- multiply by vector1 (all 115s)
*  --------------------------       
*    { p0    p1     p2     p3 }        --- partial result vector
* >>    6     6      6      6          -- right shift all values by 6 bits
* --------------------------
*    { p0    p1     p2     p3 }       -- partial result
* +  { 32    32     32     32 }        -- add vector2 (all 32's)
* --------------------------
*    { f0    f1     f2     f3 }       -- fahrenheit temperature vector 
*
*
* PROCESS:
*
* cc -g -o temperatures_mmx temperatures_mmx.c -mmmx
* ./temperatures_mmx
*
* also test at -O3
*
* GLOBALS: none
*
* LIMITATIONS:
*
* * be careful about the ranges of temperatures. temperatures and
*   partial results from the formula must fit into an 16-bit integer.
*
* * be careful to make sure that right shift rounds in the direction
*   that you want for your formula. for this example i'm not worrying
*   about it.
*
* * uses integer math so this will be an approximation
*
* REVISION HISTORY:
*
*   STR                Description                          Author
*
*   17-Jan-10          initial coding                        gpk
*
* TARGET: GNU C v 4 or higher
*
* This file is in the public domain. If it breaks, you get to keep both
* pieces.
* 
*
* ************************************************************************* */

#include <stdio.h>
#include <stdlib.h> /* exit  */
#include <string.h> /* memcpy */
#include <stdint.h> /* int8_t, int16_t, int32_t  */
#include <cpuid.h> /* __get_cpuid_max, __get_cpuid */
#include <mmintrin.h> /* MMX instrinsics  __m64 integer type  */


void test_mmx_feature(void);
void test_mmx(void);

/* SHORT_ARRAYSIZE - the number of elements we'd need in an  */
/* array of int16_t's to hold an __m64: should be 4.         */

#define SHORT_ARRAYSIZE (sizeof(__m64) / sizeof(int16_t))

int main (int argc, char * argv[])
{


  /* make sure our CPU has MMX features: exit if we don't  */
  
  test_mmx_feature();

  /* now do some vector stuff  */
  
  test_mmx();

  return(0);
}



/* FUNC_FEATURES - return the list of intel-defined features in the  */
/* __get_cpuid function.   */

#define FUNC_FEATURES 0x1





/* ************************************************************************* 


   NAME:  test_mmx_feature


   USAGE:

   test_mmx_feature();

   returns: void

   DESCRIPTION:
                 use the __get_cpuid function found on X86 to see if
		 this processor has the MMX feature. if it doesn't,
		 complain and exit.

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

void test_mmx_feature(void)
{
  unsigned int eax, ebx, ecx, edx;
  unsigned int extensions, sig;
  int result, mmx_available;

  /* call __get_cpuid: there will be bits set in ecx, edx for  */
  /* the intel-defined MMX, SSEn features.                     */
  
  result = __get_cpuid (FUNC_FEATURES, &eax, &ebx, &ecx, &edx);

  if (-1 == result)
    {
      fprintf(stderr, "Fatal Error: can't get CPU features\n");
      exit(-1);
    }
  else
    {
      mmx_available =  (bit_MMX & edx);

      if (0 == mmx_available)
	{
	  fprintf(stderr, "Error: MMX features not available\n");
	  fprintf(stderr, "Had this been an actual program, we'd fall ");
	  fprintf(stderr, "back to a non-MMX implementation\n");
	  exit(-1);
	}
      else
	{
	  fprintf(stderr, "MMX features ARE available\n");
	}
    }
}






/* ************************************************************************* 


   NAME:  test_mmx


   USAGE:
   
   test_mmx();

   returns: void

   DESCRIPTION:
                 

   REFERENCES:

   LIMITATIONS:

   * this is fixed point math so it will be an approximation
   
   * this is math in 16-bit vector elements. this means that
     all operations have to fit in 16-bits.
     
   FUNCTIONS CALLED:
   
   _mm_set_pi16 - set elements of a vector to given values
   _mm_set1_pi16 - set all elements of vector to single given value
   _mm_mullo_pi16 - multiply two vectors of shorts, store low-order bytes
   _mm_srai_pi16 - arithmetic shift right for vector of shorts
   _mm_add_pi16 - add two vectors of shorts
   memcpy
   _mm_empty - reset registers so you can do floating point again
   fprintf
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

      6-Nov-10               initial coding                           gpk

 ************************************************************************* */

void test_mmx(void)
{
  int16_t celsiusarray1[]  __attribute__ ((aligned (16))) ={-100, -80, -40, 0};
  int16_t celsiusarray2[]  __attribute__ ((aligned (16))) ={32,  40,  80, 100};
  int16_t fahrenheitarray[SHORT_ARRAYSIZE] __attribute__ ((aligned (16)));
  __m64 celsiusvector, fahrenheitvector, partial;
  __m64 coeffvector;
  const __m64 thirtytwovector = _mm_set_pi16( 32, 32, 32, 32);
  int i, coefficient;


  /* load the values from  celsiusarray1 into celsiusvector */
  
  celsiusvector = *((__m64 *)celsiusarray1);

  /* compute the coefficient we're going to use: 9/5 * 64  */
  /* and store it in all elements of  coeffvector */
  
  coefficient = (int)( (9 * 64) / 5);
  coeffvector = _mm_set1_pi16(coefficient );

  /* multiply all elements of coeffvector by  celsiusvector */
  /* and store in partial.                                  */
  
  partial = _mm_mullo_pi16(coeffvector, celsiusvector);

  /* bit shift all values in partial 6 bits right. after this,  */
  /* each element of partial will be about 9/5 * C  */
  
  partial = _mm_srai_pi16(partial, 6);

  /* now add the vector of 32's onto partial to get the vector of  */
  /* fahrenheit temperatures.                                      */
  
  fahrenheitvector = _mm_add_pi16(partial, thirtytwovector);

  /* copy the fahrenheitvector into an array so we can print it out  */
  
  memcpy(fahrenheitarray, &fahrenheitvector, sizeof(fahrenheitarray));

  /* execute _mm_empty() between MMX operations and any floating point  */
  /* operations.   */
  _mm_empty(); 

  /* now print out the results.  */
  
  for (i = 0; i < SHORT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%d celsius is %d fahrenheit\n",
	      celsiusarray1[i], fahrenheitarray[i]);
    }

  /* now the same steps for other array...  */
  
  celsiusvector = *((__m64 *)celsiusarray2);
  
  partial = _mm_mullo_pi16(coeffvector, celsiusvector);

  partial = _mm_srai_pi16(partial, 6);

  fahrenheitvector = _mm_add_pi16(partial, thirtytwovector);

  memcpy(fahrenheitarray, &fahrenheitvector, sizeof(fahrenheitarray));

  _mm_empty(); /* needed if any floating point operations follow  */
  
  for (i = 0; i < SHORT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%d celsius is %d fahrenheit\n",
	      celsiusarray2[i], fahrenheitarray[i]);
    }

}

#if 0

expected output:
---------------

 ./temperatures_mmx
MMX features ARE available
-100 celsius is -148 fahrenheit
-80 celsius is -112 fahrenheit
-40 celsius is -40 fahrenheit
0 celsius is 32 fahrenheit
32 celsius is 89 fahrenheit
40 celsius is 103 fahrenheit
80 celsius is 175 fahrenheit
100 celsius is 211 fahrenheit

#endif /* 0  */
