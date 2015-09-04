/* ************************************************************************* 
* NAME: samples/neon/float_to_char.c
*
* DESCRIPTION:
*
* this shows how to turn a vector of floats into a vector of
* unsigned chars.
*
* PROCESS:
*
* to compile and run debugging:
* cc -g -o float_to_char float_to_char.c -mfpu=neon \
     -mfloat-abi=softfp -mcpu=cortex-a8 -std=c99
*
* ./float_to_char
*
*
*
* GLOBALS:
*
* REFERENCES:
*
* RealView Compilation Tools Compiler Reference Guide Appendix G
* RealView Compilation Tools Assemebler Guide (esp chapter 5)
*
* LIMITATIONS:
*
* REVISION HISTORY:
*
*   STR                Description                          Author
*
*   24-Mar-11          initial coding                        gpk
*
* TARGET:  GNU C v 4+ on ARMv7 with Neon intrinsics
*
* The contents of this file is in the public domain. If it breaks, you
* get to keep both pieces.
*
* ************************************************************************* */



#include <stdio.h>
#include <stdlib.h> /* exit  */
#include <string.h> /* strncmp  */
#include <stdint.h> /* int8_t, int16_t, int32_t  */
#include <arm_neon.h> /* neon intrinsics  */




/* MAXLINE - the longest line we'll accept from /proc/cpuinfo  */

#define MAXLINE 256

/* local prototypes  */
void test_neon_feature(void);
uint8x16_t condense_float_vectors(float32x4_t vector0, float32x4_t vector1,  
				  float32x4_t vector2, float32x4_t vector3);
void test_float_to_char(void);
/* end prototypes  */


int main(int argc, char * argv[])
{
  /* first check to see if our processor has the neon extensions.  */
  /* exit if we don't.   */
  test_neon_feature();
  
  /* now do some vector operations with neon extensions  */

  test_float_to_char();
  
  return(0);

}


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


   NAME:  condense_float_vectors


   USAGE: 

   uint8x16_t result;
   float32x4_t vector0;
   float32x4_t vector1;
   float32x4_t vector2;
   float32x4_t vector3;

   result =  condense_float_vectors(vector0, vector1, vector2, vector3);

   returns: uint8x16_t

   DESCRIPTION:
                 take the floating point values in vector0 - vector3 and
		 store them in result. the conversion saturates. that is,
		 if a value in one of the vectors gous outside the range
		 of 0 - 255, it will be clamped at the near end of the
		 range.

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   vcvtq_s32_f32 - convert float vector to int vector
   vqmovn_s32 - convert int vector to short vector
   vcombine_s16 - combine two short vectors into a longer short vector
   vqmovun_s16 - convert from short to unsigned char
   vcombine_u8 - concatenate two unsigned char vectors
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

      1-Apr-11               initial coding                           gpk

 ************************************************************************* */

uint8x16_t condense_float_vectors(float32x4_t vector0, float32x4_t vector1,  
				  float32x4_t vector2, float32x4_t vector3)
{
  uint8x16_t retval = {0};  /* 16 bytes as 4 4-byte VECTORs  */
  int32x4_t i32pixels0, i32pixels1, i32pixels2, i32pixels3;
  int16x4_t i16pixels0, i16pixels1, i16pixels2, i16pixels3;
  int16x8_t i16pixels01, i16pixels23;
  uint8x8_t u8pixels0, u8pixels1;
  
  /* bounds check the vectorN components to 0 - 255, condense into a single  */
  /* vector of unsigned chars (uint8x16_t)  */
  i32pixels0 = vcvtq_s32_f32(vector0);
  i32pixels1 = vcvtq_s32_f32(vector1);
  i32pixels2 = vcvtq_s32_f32(vector2);
  i32pixels3 = vcvtq_s32_f32(vector3);

  i16pixels0 = vqmovn_s32(i32pixels0);
  i16pixels1 = vqmovn_s32(i32pixels1);
  i16pixels2 = vqmovn_s32(i32pixels2);
  i16pixels3 = vqmovn_s32(i32pixels3);
  
  i16pixels01 = vcombine_s16(i16pixels0, i16pixels1);
  i16pixels23 = vcombine_s16(i16pixels2, i16pixels3);

  /* where is rounding mode set??  */
  /* documentation implies these should round to nearest, but they  */
  /* truncate instead.    */
  
  u8pixels0 = vqmovun_s16(i16pixels01);
  u8pixels1 = vqmovun_s16(i16pixels23);
  
  retval = vcombine_u8(u8pixels0, u8pixels1);

  return(retval);
}



/* ************************************************************************* 


   NAME:  test_float_to_char


   USAGE: 

 
   test_float_to_char();

   returns: void

   DESCRIPTION:
                 set up some vectors, call condense_float_vectors to turn
		 them into an unsigned char vector, store it and print it.
		 

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   REVISION HISTORY:

        STR                  Description of Revision                 Author

      1-Apr-11               initial coding                           gpk

 ************************************************************************* */

void test_float_to_char(void)
{
  uint8x16_t result;
  float32x4_t vector0 = {-10.0, 0.0, 255.0, 256.0};
  float32x4_t vector1 = { -10.5, 0.5, 255.5, 256.5};
  float32x4_t vector2 = { -1.9, 0.0, 1.5, 1.9};
  float32x4_t vector3 = { 10.9, 20.9, 30.9, 40.9};
  uint8_t printarray[16];
  int i;
  
  result = condense_float_vectors(vector0, vector1, vector2, vector3);

  /* store result to printarray  */
  vst1q_u8(printarray, result);

  for(i=0; i < 16; i+=4)
    {
      fprintf(stderr, " %d  %d  %d  %d \n",
	      printarray[i], printarray[i+1], printarray[i+2], printarray[i+3]
	      );
    }
  
}

#if 0

expected output:

$  ./float_to_char
Feature 'neon' IS found in /proc/cpuinfo.
 0  0  255  255
 0  0  255  255
 0  0  1  1
 10  20  30  40

#endif /* 0  */
