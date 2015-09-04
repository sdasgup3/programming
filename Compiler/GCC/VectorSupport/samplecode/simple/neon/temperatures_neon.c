/* ************************************************************************* 
* NAME: neon/temperatures_neon.c
*
* DESCRIPTION:
*
* a simple example that uses gcc's neon intrinsics to convert an array
* of temperatures in degrees Celsius to degrees Fahrenheit
*
* this file does the conversion 3 ways:
*
* * fixed point math (neon_fixedpoint)
* * floating point math (neon_float)
* * combined multiply/accumulate (neon_float_multiply_accumulate)
*
* the fixed point math should be faster than the floating point math
* the floating point math should be more accurate than fixed point
*     (and easier to understand)
* the multiply/accumulate is to illustrate using intrinsics that combine
*    the multiply and add operations into one instruction.
*
* PROCESS:
*
* * to compile and run debugging:
* cc -g -o temperatures_neon temperatures_neon.c -mfpu=neon \
     -mfloat-abi=softfp -mcpu=cortex-a8 -std=c99
*
* ./temperatures_neon
*
*  to compile and run optimized:
*
* cc -g -o temperatures_neon temperatures_neon.c -mfpu=neon	\
     -mfloat-abi=softfp -mcpu=cortex-a8 -std=c99
*
* ./temperatures_neon
*
* you can use objdump to see the assembly language that is
*   generated at full opt:
*
* cc -ggdb -O3 -c temperatures_neon.c -mfpu=neon \
     -mfloat-abi=softfp -mcpu=cortex-a8 -std=c99
* objdump -dS temperatures_neon.o > o3temperatures_neon.dump
*
* or at debug:
*
* cc -ggdb -c temperatures_neon.c -mfpu=neon \
     -mfloat-abi=softfp -mcpu=cortex-a8 -std=c99
* objdump -dS temperatures_neon.o > temperatures_neon.dump
*
* GLOBALS: none
*
*
* REFERENCES:
*
* RealView Compilation Tools Compiler Reference Guide Appendix G
* RealView Compilation Tools Assembler Guide (esp chapter 5)
*
* LIMITATIONS:
*
* * test_neon_feature parses /proc/cpuinfo, which can change format
*   between releases.
*
* * be careful about the ranges of temperatures. temperatures and
*   partial results from the formula must fit into an 16-bit integer
*   for neon_fixedpoint.
*
* * be careful to make sure that right shift rounds in the direction
*   that you want for your formula. for this example i'm not worrying
*   about it.
*
* * neon_fixedpoint uses integer math so this will be an approximation
*
*
*
* REVISION HISTORY:
*
*   STR                Description                          Author
*
*   30-Jan-10          initial coding                        gpk
*    6-Nov-10       cleaned up comments                      gpk
*
* TARGET: GNU C version 4 or higher on an ARM processor with neon
*         extensions.
*
* This file is in the public domain. If it breaks, you get to keep both
* pieces.
*
* ************************************************************************* */

#include <stdio.h>
#include <stdlib.h> /* exit  */
#include <string.h> /* strncmp  */
#include <stdint.h> /* int8_t, int16_t, int32_t  */
#include <arm_neon.h> /* neon intrinsics  */


/* SHORT_ARRAYSIZE - the number of elements we'd need in an  */
/* array of int16_t's to hold an int128: should be 8.         */

#define SHORT_ARRAYSIZE (sizeof(int16x8_t) / sizeof(int16_t))

/*  FLOAT_ARRAYSIZE - the number of elements we'd need in an array */
/* of floats to hold a float32x4_t; should be 4                    */

#define FLOAT_ARRAYSIZE (sizeof(float32x4_t)/ sizeof(float))





/* local prototypes  */
void test_neon_feature(void);
void neon_fixedpoint(void);
void neon_float(void);
void neon_float_multiply_accumulate(void);
/* end local prototypes  */




int main(int argc, char * argv[])
{
  /* first check to see if our processor has the neon extensions.  */
  /* exit if we don't.   */
  test_neon_feature();

  /* now look at an integer implementation  */
  
  neon_fixedpoint();


  /* and a floating point implementation  */
  
  neon_float();

  /* and one that uses a combined multiply-add instruction */

  neon_float_multiply_accumulate();

}

/* MAXLINE - the longest line we'll accept from /proc/cpuinfo  */

#define MAXLINE 256





/* ************************************************************************* 


   NAME:  test_neon_feature


   USAGE: 

   test_neon_feature();

   returns: void

   DESCRIPTION:
                 read the contents of /proc/cpuinfo and look for a
		 line that starts with the the word "Features". this
		 will list the features on the CPU.

		 if "neon" appears on that line, the processor has
		   the neon extensions. tell the user

		 if "neon" doesn't appear on that line, the processor
		   doesn't have neon extensions. tell the user and exit

		 

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

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

void test_neon_feature(void)
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
  else
    {
      foundit = 0;
      
      astring = fgets(cpuinfo, MAXLINE, file);

      while ((0 == foundit) && (NULL != astring ) )
	{

	  if (0 == strncmp("Features", astring, strlen("Features")))
	    {

	      if (NULL != strstr(astring, "neon"))
	        {
		  foundit = 1;
	        }
	    }

	 astring = fgets(cpuinfo, MAXLINE, file);
	}

      if (0 == foundit)
	{
	  fprintf(stderr, "Feature 'neon' not found in /proc/cpuinfo.\n");
	  exit(-1);
	}
      else
	{
	  fprintf(stderr, "Feature 'neon' IS found in /proc/cpuinfo.\n");
	}
    }
}






/* ************************************************************************* 


   NAME:  neon_fixedpoint


   USAGE: 

    
   test_neon_feature();
   
   neon_fixedpoint();

   returns: void

   DESCRIPTION:
                this function uses some neon integer vector
		intrinsics to convert celsius to fahrenheit temperatures.

		this example uses vectors of 8, 16-bit integers.
		it applies the formula:
		
		f = (9/5)C + 32 = (9 * C) / 5 + 32

		to elements of a vector of celsius temperatures.
		in this case we take an array of 16-bit celsius values,
		load that into a vector, do the math, then store the
		vector into an array so we can print it out, 
   REFERENCES:

   LIMITATIONS:
   
   * be careful about the ranges of temperatures. temperatures and
     partial results from the formula must fit into an 16-bit integer.

   * be careful to make sure that right shift rounds in the direction
     that you want for your formula. for this example i'm not worrying
     about it.

   * uses integer math so this will be an approximation

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   vdupq_n_s16 - assign a value to all elements of a vector
   vmulq_s16 - multiply two signed 16-bit vectors
   vshrq_n_s16 - right shift each value stored in a vector
   vaddq_s16 - add two vectors
   vst1q_s16 - store a vector into an array
   fprintf
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

      7-Feb-10               initial coding                           gpk

 ************************************************************************* */

void neon_fixedpoint(void)
{
  int16_t celsiusarray[] ={-100, -80, -40, 0, 32,  40,  80, 100};
  int16_t fahrenheitarray[SHORT_ARRAYSIZE];
  int16x8_t celsiusvector, fahrenheitvector, partial;
  int16x8_t coeffvector;
  const int16x8_t thirtytwovector = { 32, 32, 32, 32,  32, 32, 32, 32};
  int i;
  int16_t coefficient;

  /* hmmm. this works at low and high levels of optimization.  */
  celsiusvector = *((int16x8_t *)celsiusarray);

  /* compute the coefficient we're going to use: 9/5 * 64  */
  /* and store it in all elements of  coeffvector */
  
  coefficient = (int16_t)( (9 * 64) / 5);
  coeffvector = vdupq_n_s16(coefficient);
  
  /* multiply all elements of coeffvector by  celsiusvector */
  /* and store in partial.                                  */
  
  partial = vmulq_s16(coeffvector, celsiusvector);

  /* bit shift all values in partial 6 bits right. after this,  */
  /* each element of partial will be about 9/5 * C  */

  partial = vshrq_n_s16(partial, 6);

  /* here's a rounding shift...  */
  /*  partial = vrshrq_n_s16(int16x8_t(partial, 6); */
  
   /* now add the vector of 32's onto partial to get the vector of  */
  /* fahrenheit temperatures.                                      */
  
  fahrenheitvector = vaddq_s16(partial, thirtytwovector);

  /* copy the fahrenheitvector into an array so we can print it out  */
  
  vst1q_s16(fahrenheitarray, fahrenheitvector);

  /*  memcpy(fahrenheitarray, &fahrenheitvector, sizeof(fahrenheitarray)); */

  /* now print out the results.  */
  
  fprintf(stderr, "--- function %s ------\n", __FUNCTION__);
  /* fprintf(stderr, "From neon_fixedpoint:\n"); */
  
  for (i = 0; i < SHORT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%d celsius is %d fahrenheit\n",
	      celsiusarray[i], fahrenheitarray[i]);
    }

}




/* ************************************************************************* 


   NAME:  neon_float


   USAGE: 

   neon_float();

   returns: void

   DESCRIPTION:
                 this shows a floating point implementation for converting
		 celsius to fahrenheit using:

		 f = (9/5)C + 32

		 and the neon vector extensions.

		 in this case we take a two arrays of celsius values,
		 turn them into neon vectors, and then do the math.

		 this example uses vectors of 4 32-bit floats, loads
		 the data into the vectors, does the math, then
		 transfers the data back to arrays to print out the
		 results.

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   vmovq_n_f32 - copy a 32-bit float into all elements of a vector
   vld1q_f32 - load data from an float array into a vector
   vmulq_f32 - multiply two floating point vectors
   vaddq_f32 - add two floating point vectors
   vst1q_f32 - store a vector into an array
   fprintf
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     20-Feb-10               initial coding                           gpk

 ************************************************************************* */

void neon_float(void)
{
  float celsiusarray1[]= {-100, -80, -40, 0};
  float celsiusarray2[]= {32,  40,  80, 100};
  float32_t fahrenheitarray[FLOAT_ARRAYSIZE];
  float32x4_t celsiusvector, fahrenheitvector, partial;
  float32x4_t coeffvector;
  float coefficient;
  const float32x4_t thirtytwovector = {32.0, 32.0, 32.0, 32.0};
  int i;

  /* compute 9/5 and fill coeffvector with that value using  */
  /* the vmovq_n_f32 function.  */
  
  coefficient = (9.0 / 5.0);
  coeffvector = vmovq_n_f32(coefficient);

  /* pull celsiusarray1 into celsiusvector  */

  /* note: this assignment: */
  /* celsiusvector = *((float32x4_t *)celsiusarray1); */
  /* works at "-g", but not "-O3"  */
  /* test carefully when using vectors and intrinsics!  */
  
  celsiusvector = vld1q_f32((float32_t *)celsiusarray1);


  /* multiply all elements of coeffvector by  celsiusvector */
  /* and store in partial.                                  */
  
  partial = vmulq_f32(coeffvector, celsiusvector);

  /* now add the vector of 32's onto partial to get the vector of  */
  /* fahrenheit temperatures.                                      */

  fahrenheitvector = vaddq_f32(partial, thirtytwovector);

  /* copy the fahrenheitvector into an array so we can print it out  */
  
  vst1q_f32(fahrenheitarray, fahrenheitvector);

  /* fprintf(stderr, "From neon_float:\n"); */
  fprintf(stderr, "--- function %s ------\n", __FUNCTION__);
  
  for (i = 0; i < FLOAT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%f celsius is %f fahrenheit\n",
	      celsiusarray1[i], fahrenheitarray[i]);
    }


  /* now do the contents of celsiusarray2  */

 /* celsiusvector = *((float32x4_t *)celsiusarray2); */
  celsiusvector = vld1q_f32((float32_t *)celsiusarray2);


  /* multiply all elements of coeffvector by  celsiusvector */
  /* and store in partial.                                  */
  
  partial = vmulq_f32(coeffvector, celsiusvector);

  /* now add the vector of 32's onto partial to get the vector of  */
  /* fahrenheit temperatures.                                      */

  fahrenheitvector = vaddq_f32(partial, thirtytwovector);

  /* copy the fahrenheitvector into an array so we can print it out  */
  
  vst1q_f32(fahrenheitarray, fahrenheitvector);

  for (i = 0; i < FLOAT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%f celsius is %f fahrenheit\n",
	      celsiusarray2[i], fahrenheitarray[i]);
    }
  

}




/* ************************************************************************* 


   NAME:  neon_float_multiply_accumulate


   USAGE: 

   neon_float_multiply_accumulate();

   returns: void

   DESCRIPTION:
                 this does the same work as neon_float except that it
		 combines the multiplying by 9/5's and the adding of 32
		 by using the neon vmlaq_n_f32 intrinsic.

		 vmlaq_n_f32(float32x4_t a, float32x4_t b, float32_t c);
		 will multiply each element of b by c and add the product
		 to a to form the result. that is:

		 result[i] = a[i] + b[i] * c

		 so make a[i] = 32,
		         c = 9/5;
			 b[i] = temperature in celsius
		 

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   vld1q_f32 - load array into vector
   vmlaq_n_f32 - multiply & add
   vst1q_f32 - store vector into array
   fprintf 
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     20-Feb-10               initial coding                           gpk

 ************************************************************************* */

void neon_float_multiply_accumulate(void)
{

  float celsiusarray1[]= {-100, -80, -40, 0};
  float celsiusarray2[]= {32,  40,  80, 100};
  const float32x4_t thirtytwovector = {32.0, 32.0, 32.0, 32.0};
  float32_t fahrenheitarray[FLOAT_ARRAYSIZE];
  float32x4_t celsiusvector, fahrenheitvector, partial;
  const float coefficient = (9.0 / 5.0);
  int i;

  /* pull celsiusarray1 into celsiusvector with vld1q_f32 */

  /* note: you could use this assignment: */
  /* celsiusvector = *((float32x4_t *)celsiusarray1); */
  /* except that it works at "-g", but not "-O3"  */
  /* so we'll use vld1q_f32  */
  /* test carefully when using vectors and intrinsics!  */
  
  celsiusvector = vld1q_f32((float32_t *)celsiusarray1);


  /* this formula is simple enough we can do the whole thing in  */
  /* one intrinsic: vmlaq_n_f32.  */


  fahrenheitvector = vmlaq_n_f32(thirtytwovector, celsiusvector, coefficient);

 
   /* copy the fahrenheitvector into an array so we can print it out  */
  
  vst1q_f32(fahrenheitarray, fahrenheitvector);
  
  fprintf(stderr, "--- function %s ------\n", __FUNCTION__);
  /*  fprintf(stderr, "From neon_float_multiply_accumulate:\n"); */
  
  for (i = 0; i < FLOAT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%f celsius is %f fahrenheit\n",
	      celsiusarray1[i], fahrenheitarray[i]);
    }


  /* now do the contents of celsiusarray2  */

  celsiusvector = vld1q_f32((float32_t *)celsiusarray2);
  
  fahrenheitvector = vmlaq_n_f32(thirtytwovector, celsiusvector, coefficient);  

   /* copy the fahrenheitvector into an array so we can print it out  */
  
  vst1q_f32(fahrenheitarray, fahrenheitvector);
  
  for (i = 0; i < FLOAT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%f celsius is %f fahrenheit\n",
	      celsiusarray2[i], fahrenheitarray[i]);
    }  
}

#if 0

expected output:

$ ./temperatures_neon
Feature 'neon' IS found in /proc/cpuinfo.
--- function neon_fixedpoint ------
-100 celsius is -148 fahrenheit
-80 celsius is -112 fahrenheit
-40 celsius is -40 fahrenheit
0 celsius is 32 fahrenheit
32 celsius is 89 fahrenheit
40 celsius is 103 fahrenheit
80 celsius is 175 fahrenheit
100 celsius is 211 fahrenheit
--- function neon_float ------
-100.000000 celsius is -148.000000 fahrenheit
-80.000000 celsius is -112.000000 fahrenheit
-40.000000 celsius is -40.000000 fahrenheit
0.000000 celsius is 32.000000 fahrenheit
32.000000 celsius is 89.599998 fahrenheit
40.000000 celsius is 104.000000 fahrenheit
80.000000 celsius is 176.000000 fahrenheit
100.000000 celsius is 212.000000 fahrenheit
--- function neon_float_multiply_accumulate ------
-100.000000 celsius is -148.000000 fahrenheit
-80.000000 celsius is -112.000000 fahrenheit
-40.000000 celsius is -40.000000 fahrenheit
0.000000 celsius is 32.000000 fahrenheit
32.000000 celsius is 89.599998 fahrenheit
40.000000 celsius is 104.000000 fahrenheit
80.000000 celsius is 176.000000 fahrenheit
100.000000 celsius is 212.000000 fahrenheit

#endif /* 0  */
