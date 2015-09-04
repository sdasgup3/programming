/* ************************************************************************* 
* NAME: /home/gpk/papers/samples/neon/char_to_float.c
*
* DESCRIPTION:
*
* make sure i understand how to convert 4 chars into a vector of 4 floats.
*
* PROCESS:
*
*
* to compile and run debugging:
* cc -g -o char_to_float char_to_float.c -mfpu=neon \
     -mfloat-abi=softfp -mcpu=cortex-a8 -std=c99
*
* ./char_to_float
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
*   20-Mar-11          initial coding                        gpk
*
* TARGET:  GNU C v 4+ on ARMv7 with Neon intrinsics
*
* The contents of this file is in the public domain. If it breaks, you
* get to keep both pieces.
*
*
* ************************************************************************* */



#include <stdio.h>
#include <stdlib.h> /* exit  */
#include <string.h> /* strncmp  */
#include <stdint.h> /* int8_t, int16_t, int32_t  */
#include <arm_neon.h> /* neon intrinsics  */




/* MAXLINE - the longest line we'll accept from /proc/cpuinfo  */

#define MAXLINE 256


/*  FLOAT_ARRAYSIZE - the number of elements we'd need in an array */
/* of floats to hold an __m128; should be 4                        */

#define FLOAT_ARRAYSIZE (sizeof(float32x4_t)/ sizeof(float))




/* local prototypes  */
void test_neon_feature(void);
void char_to_float_vectors(const unsigned char * sourcep,
			   float32x4_t *mp0, float32x4_t * mp1);
void test_char_to_float(void);
/* end prototypes  */

int main(int argc, char * argv[])
{
  /* first check to see if our processor has the neon extensions.  */
  /* exit if we don't.   */
  test_neon_feature();
  
  /* now do some vector operations with neon extensions  */

  test_char_to_float();
  
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


   NAME:  char_to_float_vectors


   USAGE: 

    
   const unsigned char * sourcep= {...};
   
   float32x4_t mp0;
   float32x4_t mp1;

   char_to_float_vectors(sourcep, &mp0, &mp1);

   returns: void

   DESCRIPTION:
                 take the first 8 bytes from sourcep and turn them into
		 two vectors of floats: storing in mp0, mp1.

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   vld1_u8 - load vector from memory
   vmovl_u8 - widen elements of vector 
   vreinterpretq_s16_u16 - reminterpret unsigned as signed
   vget_low_s16 - get low half of a vector
   vget_high_s16  - get the high half of a vector
   vmovl_s16 - widen elements of vector 
   vcvtq_f32_s32 - convert from a signed int to a float

   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     20-Mar-11               initial coding                           gpk

 ************************************************************************* */

void char_to_float_vectors(const unsigned char * sourcep,
			   float32x4_t *mp0, float32x4_t * mp1)
{
 uint8x8_t rawpixels; /* source pixels as {[YUYV]0 [YUYV]1}   */
 int16x8_t widerpixels; /*  rawpixels promoted to shorts per component */
 int16x4_t high16, low16;
 int32x4_t high32, low32;

 
 rawpixels = vld1_u8(sourcep);
 widerpixels = vreinterpretq_s16_u16(vmovl_u8(rawpixels));
 low16 = vget_low_s16(widerpixels);
 high16 = vget_high_s16(widerpixels);
 high32 = vmovl_s16(high16);
 low32  = vmovl_s16(low16);

 *mp0 = vcvtq_f32_s32(low32);
 *mp1 = vcvtq_f32_s32(high32);
  
}



/* ************************************************************************* 


   NAME:  test_char_to_float


   USAGE: 

   test_char_to_float();

   returns: void

   DESCRIPTION:
                 build an array of unsigned chars and pass it to
		 char_to_float_vectors (expecting two vectors of
		 floats in return). print those vectors of floats

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: 

      modified: 

   FUNCTIONS CALLED:
   
   char_to_float_vectors
   vst1q_f32 - store a float32x4_t into memory
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     20-Mar-11               initial coding                           gpk

 ************************************************************************* */

void test_char_to_float(void)
{
  unsigned char rawpixels[8] = {0, 1, 2, 3, 252, 253, 254, 255};
  float32x4_t low, high;
  float32_t lofloatarray[FLOAT_ARRAYSIZE] __attribute__ ((aligned (16)));
  float32_t hifloatarray[FLOAT_ARRAYSIZE] __attribute__ ((aligned (16)));
  int i;
  
  char_to_float_vectors(rawpixels, &low, &high);
  vst1q_f32(lofloatarray, low);
  vst1q_f32(hifloatarray, high);

  fprintf(stderr, "low {%f, %f, %f, %f} hi {%f %f, %f, %f}\n",
	  lofloatarray[0], lofloatarray[1], lofloatarray[2], lofloatarray[3],
	  hifloatarray[0], hifloatarray[1], hifloatarray[2], hifloatarray[3]);
}


#if 0

expected output:

./char_to_float
Feature 'neon' IS found in /proc/cpuinfo.
low {0.000000, 1.000000, 2.000000, 3.000000} hi {252.000000 253.000000, 254.000000, 255.000000}

#endif /* 0  */
