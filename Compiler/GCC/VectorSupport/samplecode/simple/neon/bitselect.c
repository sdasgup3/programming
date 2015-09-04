/* ************************************************************************* 
* NAME: neon/bitselect.c
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
* cc -g -o bitselect bitselect.c -mfpu=neon \
     -mfloat-abi=softfp -mcpu=cortex-a8 -std=c99
*
* ./bitselect
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
void test_bit_selection(void);
void test_yalpha(void);
/* end local prototypes  */


int main(int argc, char * argv[])
{

  /* first check to see if our processor has the neon extensions.  */
  /* exit if we don't.   */
  test_neon_feature();
  
  test_bit_selection();
  test_yalpha();

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

  result = vbsl_u8(combine, a, b);
  vst1_u8(outputarray, result);

  fprintf(stderr, "combining \n");

  for (i = 0; i < 8; i++)
    {
      fprintf(stderr, "%d %d \n", i, outputarray[i]);
    }

}

#define MP0_Y0 0
#define MP0_Y1 2
void test_yalpha(void)
{

  uint8x8_t yuyvyuyv = {50, 20, 51, 30, 52, 21, 53, 33};
  uint8x8_t yselect = {0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00};
  uint8x8_t zeroalpha = {0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff};
  uint8x8_t y0alphay1alpha;
  uint8x8_t y0vec, y1vec;
  uint8_t outputarray[8]__attribute__ ((aligned (16)));
  int i;
  
  y0vec = vdup_lane_u8(yuyvyuyv, MP0_Y0);
  y1vec = vdup_lane_u8(yuyvyuyv, MP0_Y1);

  
  y0alphay1alpha =  vbsl_u8(yselect, y0vec, y1vec);
  y0alphay1alpha = vset_lane_u8(0xff, y0alphay1alpha, 3);
  y0alphay1alpha = vset_lane_u8(0xff, y0alphay1alpha, 7);

  vst1_u8(outputarray, y0alphay1alpha);

  fprintf(stderr, "yalpha should be {50, 50, 50, 255, 51, 51, 51, 255} \n");
  fprintf(stderr, "yalpha IS { ");
  for (i = 0; i < 8; i++)
    {
      fprintf(stderr, "%d ", outputarray[i]);
    }
  fprintf(stderr, "} \n");
  
}


#if 0


  expected output:
./bitselect
Feature 'neon' IS found in /proc/cpuinfo.
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
yalpha should be {50, 50, 50, 255, 51, 51, 51, 255}
yalpha IS { 50 50 50 255 51 51 51 255 }


#endif /* 0 */


