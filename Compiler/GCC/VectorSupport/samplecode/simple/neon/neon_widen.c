/* ************************************************************************* 
* NAME: neon/neon_widen.c
*
* DESCRIPTION:
*
* test out more of the ARM neon constructs i'll need for color space
* conversion. this file uses vreinterpretq_s16_u16, vmulq_n_s16,
* vst1q_s16, vget_high_s16, vget_low_s16, vdupq_lane_s16, vmovl_u8,
* vtbx1_u8
* 
*
* PROCESS:
*
*
* to compile and run debugging:
* cc -g -o neon_widen neon_widen.c -mfpu=neon \
     -mfloat-abi=softfp -mcpu=cortex-a8 -std=c99
*
* ./neon_widen
*
* GLOBALS: none
*
* REFERENCES:
*
* RealView Compilation Tools Compiler Reference Guide Appendix G
* RealView Compilation Tools Assemebler Guide (esp chapter 5)
*
*
* LIMITATIONS:
*
* REVISION HISTORY:
*
*   STR                Description                          Author
*
*   13-Nov-10          initial coding                        gpk
*
* TARGET: GNU C v 4+ on ARMv7 with Neon intrinsics
*
* This file is in the public domain. If it breaks, you get to keep
* both pieces.
*
* ************************************************************************* */


#include <stdio.h>
#include <stdlib.h> /* exit  */
#include <string.h> /* strncmp  */
#include <stdint.h> /* int8_t, int16_t, int32_t  */
#include <arm_neon.h> /* neon intrinsics  */



/* MAXLINE - the longest line we'll accept from /proc/cpuinfo  */

#define MAXLINE 256

/* SHORTARRAY_SIZE - the size an array of int16_t has to be   */
/* to hold an int16x8_t vector (should be 8).                 */

#define SHORTARRAY_SIZE (sizeof(int16x8_t) / sizeof(int16_t))

/* local prototypes  */
void test_neon_feature(void);
void test_byte_ops(void);
void test_widen(void);
void test_bit_selection(void);
void test_split(void);
/* end local prototypes  */


int main(int argc, char * argv[])
{

  /* first check to see if our processor has the neon extensions.  */
  /* exit if we don't.   */
  test_neon_feature();

  /* test some of things we'll do in colorconv2  */
  test_byte_ops();

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


   NAME:  test_byte_ops


   USAGE: 

 

   test_byte_ops();

   returns: void

   DESCRIPTION:
                 test things we're going to use in colorconv2

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed:  none

      modified:  none

   FUNCTIONS CALLED:

   REVISION HISTORY:

        STR                  Description of Revision                 Author

      1-Apr-11               initial coding                           gpk

 ************************************************************************* */

void test_byte_ops(void)
{
  test_widen(); /* unsigned chars to signed shorts  */
  test_split(); /* take apart a vector  */
  test_bit_selection(); /* shuffle bytes around to form a new vector  */

}

 


/* ************************************************************************* 


   NAME:  test_widen


   USAGE: 

   test_widen();

   returns: void

   DESCRIPTION:
                 see how to widen a vector of unsigned chars to a
		 vector of signed 16-bit integers.

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   vmovl_u8 - widen an uint8x8_t into a uint16x8_t
   vreinterpretq_s16_u16 - reinterpret bits to add sign to unsigned 16-bit int
   vmulq_n_s16 - multiply vector by scalar
   vst1q_s16 - store vector to memory
   vget_high_s16 - get high 64-bits of 128 bit vector
   vget_low_s16 - get low 64-bits of 128 bit vector
   vdupq_lane_s16 - copy one element of a vector into all elements of a vector
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

      8-Jan-11               initial coding                           gpk

 ************************************************************************* */

void test_widen(void)
{
  uint8x8_t narrow = {10, 20, 30, 40, 50, 60, 70, 80};
  int16x8_t wider;
  int16x4_t high, low;
  int16x8_t selected;
  int16_t outputarray[SHORTARRAY_SIZE];
  int i;

  /* turn unsigned char vector into vector of shorts  */
  
  wider = vreinterpretq_s16_u16(vmovl_u8(narrow));

  /* multiply those values by 100 to make sure we can  */
  
  wider = vmulq_n_s16(wider, 100);

  /* store and print  */
  vst1q_s16 (outputarray, wider);

  fprintf(stderr, "---in %s---- \n", __FUNCTION__);
  fprintf(stderr, "wider vector is:\n");
  
  for (i = 0; i < SHORTARRAY_SIZE; i++)
    {
      fprintf(stderr, "%d %d \n", i, outputarray[i]);
    }


  /* separate wider into high and low halves  */
  
  high = vget_high_s16(wider);
  low = vget_low_s16(wider);

  fprintf(stderr, "selecting index 0 from high gives:\n");

  /* make selected a vector of the zeroth element of high  */
  selected = vdupq_lane_s16(high, 0);

  vst1q_s16 (outputarray, selected);

  for (i = 0; i < SHORTARRAY_SIZE; i++)
    {
      fprintf(stderr, "%d %d \n", i, outputarray[i]);
    }
  fprintf(stderr, "selecting index 3 from low gives:\n");

  /* make selected a vector of element 3 from low  */
  selected = vdupq_lane_s16(low, 3);

  vst1q_s16 (outputarray, selected);

  for (i = 0; i < SHORTARRAY_SIZE; i++)
    {
      fprintf(stderr, "%d %d \n", i, outputarray[i]);
    }

}







/* ************************************************************************* 


   NAME:  test_bit_selection


   USAGE: 

 
   test_bit_selection();

   returns: void

   DESCRIPTION:
                 use the vbsl_u8 instruction to shuffle elements of two
		 vectors to form a third.
		 

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   vbsl_u8 - shuffle two vectors to build a third
   vst1_u8 - store vector to memory
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

      1-Apr-11               initial coding                           gpk

 ************************************************************************* */

void test_bit_selection(void)
{
  uint8x8_t firstvector = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
  uint8x8_t secondvector = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; 
  uint8x8_t combine ={0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00};
  uint8x8_t a = {10, 20, 30, 40, 50, 60, 70, 80};
  uint8x8_t b = {90, 100, 110, 120, 130, 140, 150, 160};
  uint8x8_t result;
  uint8_t outputarray[8]__attribute__ ((aligned (16)));
  int i;

  fprintf(stderr, "---in %s---- \n", __FUNCTION__);


  /* select the elements from a & b dictated by firstvector and store them  */
  /* in result. (firstvector will select the elements of a in order)  */
  result = vbsl_u8(firstvector, a, b);
  vst1_u8(outputarray, result);

  fprintf(stderr, "selecting from firstvector \n");

  for (i = 0; i < 8; i++)
    {
      fprintf(stderr, "%d %d \n", i, outputarray[i]);
    }
  /* select the elements from a & b dictated by secondvector and store them  */
  /* in result. (secondvector will select the elements of b in order)  */
  
  result = vbsl_u8(secondvector, a, b);
  vst1_u8(outputarray, result);

  fprintf(stderr, "selecting from secondvector \n");

  for (i = 0; i < 8; i++)
    {
      fprintf(stderr, "%d  %d \n", i,  outputarray[i]);
    }

  /* now interleave the elements of a & b using the combine vector  */
  /* and store the results out result. (combine will zip a & b   */
  /* together.)  */
  
  result = vbsl_u8(combine, a, b);
  vst1_u8(outputarray, result);

  fprintf(stderr, "combining \n");

  for (i = 0; i < 8; i++)
    {
      fprintf(stderr, "%d %d \n", i, outputarray[i]);
    }

}



/* ************************************************************************* 


   NAME:  test_split


   USAGE: 

    
   void;

   test_split();

   returns: void

   DESCRIPTION:
                 see how to split a vector with vget_low_s16 &
		 vget_high_s16; print the results

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   vget_low_s16 - get the low half of a vector
   vget_high_s16 - get the high half of a vector
   vst1_s16 - store vector to memory
   fprintf
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

      1-Apr-11               initial coding                           gpk

 ************************************************************************* */

void test_split(void)
{
  int16x8_t avector = {0, 1, 2, 3, 10, 20, 30, 40};
  int16x4_t low, high;
  int16_t outputarray[4];
  int i;
  
  fprintf(stderr, "---in %s---- \n", __FUNCTION__);

  /* low should be {0, 1, 2, 3}  */
  /* high should be {10, 20, 30, 40}  */
  
  low = vget_low_s16(avector);
  high = vget_high_s16(avector);

   /* store and print  */
  vst1_s16 (outputarray, low);

  fprintf(stderr, "low elements {%d, %d, %d, %d}\n",
	  outputarray[0], outputarray[1], outputarray[2], outputarray[3]);

   /* store and print  */
  vst1_s16 (outputarray, high);

  fprintf(stderr, "high elements {%d, %d, %d, %d}\n",
	  outputarray[0], outputarray[1], outputarray[2], outputarray[3]);

 

  
}


#if 0

$  ./neon_widen                                   
Feature 'neon' IS found in /proc/cpuinfo.                                       
---in test_widen----                                                            
wider vector is:                                                                
0 1000                                                                          
1 2000
2 3000
3 4000
4 5000
5 6000
6 7000
7 8000
selecting index 0 from high gives:
0 5000
1 5000
2 5000
3 5000
4 5000
5 5000
6 5000
7 5000
selecting index 3 from low gives:
0 4000
1 4000
2 4000
3 4000
4 4000
5 4000
6 4000
7 4000
---in test_split----
low elements {0, 1, 2, 3}
high elements {10, 20, 30, 40}
---in test_bit_selection----
selecting from firstvector
0 10
1 20
2 30
3 40
4 50
5 60
6 70
7 80
selecting from secondvector
0  90
1  100
2  110
3  120
4  130
5  140
6  150
7  160
combining
0 10
1 100
2 30
3 120
4 50
5 140
6 70
7 160


#endif /* 0 */


