/* ************************************************************************* 
* NAME: neon/neon_table.c
*
* DESCRIPTION:
*
* this is a sample program to help me understand the ARM neon table
* construct.
*
* table lets you start with a vector (the table), and use a second
* vector to select elements from the table. the result is used to
* build a third vector.
*
* PROCESS:
* * to compile and run debugging:
* cc -g -o neon_table neon_table.c -mfpu=neon \
     -mfloat-abi=softfp -mcpu=cortex-a8 -std=c99
* ./neon_table
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
*   24-Oct-10          initial coding                        gpk
*
* TARGET: GNU C version 4 or higher
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





/* MAXLINE - the longest line we'll accept from /proc/cpuinfo  */

#define MAXLINE 256


/* local prototypes  */
void test_neon_feature(void);

/* end local prototypes  */


int main(int argc, char * argv[])
{
  const uint8x8_t table = {10, 20, 30, 40, 50, 60, 70, 80};
  const uint8x8_t indexvector = {2, 4, 6, 1, 1, 1, 3, 5};
  uint8x8_t destination;
  uint8_t outputarray[8];
  int i;
  
  /* first check to see if our processor has the neon extensions.  */
  /* exit if we don't.   */
  test_neon_feature();

  /* look up values from table using indexvector. the result is  */
  /* a vector built like this:  */
  /* destination[i] = table[indexvector[i]]  */
  
  destination = vtbl1_u8(table, indexvector);

  vst1_u8 (outputarray, destination);

  for (i = 0; i < 8; i++)
    {
      fprintf(stderr, "%d %d \n", i, outputarray[i]);
    }
  

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




#if 0

expected output:  

Feature 'neon' IS found in /proc/cpuinfo.
0 30
1 50
2 70
3 20
4 20
5 20
6 40
7 60

#endif /* 0  */
