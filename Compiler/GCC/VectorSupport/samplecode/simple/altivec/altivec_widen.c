/* ************************************************************************* 
* NAME: altivec_widen.c
*
* DESCRIPTION:
*
* this is a test program to help me figure out altivec instructions.
* this file covers the unpack operation.
*
* PROCESS:
*
* to compile and run debugging:
* cc -g -o altivec_widen altivec_widen.c -maltivec -mabi=altivec
* ./altivec_widen
*
*  to compile and run optimized:
* cc -O3 -o altivec_widen altivec_widen.c -maltivec -mabi=altivec
* ./altivec_widen
*
* you can use objdump to see the assembly language that is
*   generated at full opt:
*
* cc -ggdb -O3 -c altivec_widen.c -maltivec -mabi=altivec
* objdump -dS altivec_widen.o > o3altivec_widen.dump
*
* or at debug:
*
* cc -ggdb -c altivec_widen.c -maltivec -mabi=altivec
* objdump -dS altivec_widen.o > altivec_widen.dump
*
* GLOBALS: none
*
* REFERENCES:
*
* http://developer.apple.com/hardwaredrivers/ve/sse.html#Translation
* GCC info page for PowerPC built-ins
* AltiVec Technology Programming Interface Manual
* Ian Ollmann's Altivec Tutorial
*
* LIMITATIONS:
*
* * test_altivec_feature parses /proc/cpuinfo, which can change format
*   between releases of linux.
*
* REVISION HISTORY:
*   STR                Description                          Author
*    29-Oct-10          initial coding                        gpk
*
* TARGET: GNU C version 4 or higher
*
* ************************************************************************ */

/* ===== INCLUDES ===== */

#include <stdio.h>
#include <stdlib.h> /* exit  */
#include <string.h> /* memcpy */

#include <altivec.h>

/* ===== DEFINES ===== */

/*  MAXLINE - the longest line i'll read from /proc/cpuinfo */

#define MAXLINE 255

/* CHAR_ARRAYSIZE - the number of chars in an array it would take */
/* to make it the same size as a char vector                      */

#define CHAR_ARRAYSIZE (sizeof (vector char) / sizeof (char))

/* SHORT_ARRAYSIZE - the number of elements in a short array that  */
/* make it the same size as a short vector                         */
#define SHORT_ARRAYSIZE (sizeof(vector short) / sizeof(short))

/* INT_ARRAYSIZE - the number of elements needed in an int array  */
/* to hold the contents of an int vector   */

#define INT_ARRAYSIZE (sizeof(vector int) / sizeof(int))

/* FLOAT_ARRAYSIZE - the number of elements needed in a float array  */
/* to hold the contents of a float vector   */

#define FLOAT_ARRAYSIZE (sizeof(vector float) / sizeof(float))

/* ===== FUNCTIONS ===== */

/* ************************************************************************* 

   NAME:  test_altivec_feature

   USAGE: 

   test_altivec_feature();

   returns: void

   DESCRIPTION:
                 read the contents of /proc/cpuinfo and look for a
		 line that starts with the the word "cpu". this
		 will list the features on the CPU.

		 if "altivec supported" appears on that line, the processor has
		   the altivec extensions. tell the user

		 if "altivec supported" doesn't appear on that line, 
                   the processor
		   doesn't have altivec extensions. tell the user and exit

   REFERENCES:

   LIMITATIONS:

   * format of /proc/cpuinfo can change between releases, so this is not
     future-proof.
  
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

void test_altivec_feature(void)
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

  foundit = 0;
      
  astring = fgets(cpuinfo, MAXLINE, file);

  while ((0 == foundit) && (NULL != astring ) )
    {
       if (0 == strncmp("cpu", astring, strlen("cpu")))
	 {
	   /* is this correct ??  */
	   if (NULL != strstr(astring, "altivec supported"))
	     {
	       foundit = 1;
	     }
	 }

       astring = fgets(cpuinfo, MAXLINE, file);
    }

    if (0 == foundit)
      {
	fprintf(stderr, "Feature 'altivec' not found in /proc/cpuinfo.\n");
	exit(-1);
      }
    else
      {
	fprintf(stderr, "Feature 'altivec' IS found in /proc/cpuinfo.\n");
      }
   
} /* test_altivec_feature */

#ifndef __VEC__ /* no altivec instructions allowed */
#error This file uses altivec instructions: are you compiling with
#error  the right flags? ("-maltivec -mabi=altivec")

/* ************************************************************************* 
   NAME:  test_unpack
 ************************************************************************* */
void test_unpack()
{
  fprintf(stderr, "Error: can't generate unpack instructions without");
  fprintf(stderr, " an altivec-aware compiler. Did you compile with the ");
  fprintf(stderr, "correct flags?\n");
}

#else

/* ************************************************************************* 

   NAME:  test_unpack

   USAGE: 

   test_unpack();

   returns: void

   DESCRIPTION:
                 test using vec_unpackh/vec_unpackl to widen a vector.
                 vec_unpackh will unpack the high elements of a
                 vector into a vector and widen the element. eg
                 unpack a char vector into 2 short vectors, a short
                 vector into 2 int vectors, etc. use this when the
                 vectors you're working on are signed types.
                 use something like what's in test_merge (altivec_merge.c)
                 when you're working with unsigned types

   REFERENCES:

   Ian Ollmann's Altivec Tutorial
   
   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   fprintf
   vec_st - store a vector into memory
   vec_unpackh - extract the high end of a vector into a wider vector
   vec_unpackl - extract the low end of a vector into a wider vector

   REVISION HISTORY:
        STR                  Description of Revision                 Author
     25-Nov-10               initial coding                           gpk
     16-Apr-11          added functions called                        gpk
     
 ************************************************************************* */

void test_unpack()
{
  vector signed char inputvector =
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  vector signed short hiunpack;
  vector signed short lounpack;
  signed char printchar[CHAR_ARRAYSIZE] __attribute__ ((aligned (16)));
  short printshort[SHORT_ARRAYSIZE] __attribute__ ((aligned (16)));
  int i;

  fprintf(stderr, "--- function %s ------\n", __FUNCTION__);

  /* first, print the inputvector  */
  /* vec_st: Make vector accessible for printing */
  vec_st(inputvector, 0, printchar);

  for (i = 0; i < CHAR_ARRAYSIZE; i++)
    {
      fprintf(stderr, "Input vector %d: $%x\n", i, (int)(printchar[i]));
    }

  hiunpack = vec_unpackh(inputvector);
  vec_st(hiunpack, 0, printshort);

  /* hiunpack contains 00 00 to 00 07 */
  for (i = 0; i < SHORT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "hiunpack output vector %d: $%x\n", i, printshort[i]);
    }

  lounpack = vec_unpackl(inputvector);
  vec_st(lounpack, 0, printshort);
  /* lounpack contains 00 08 to 00 0F */

  for (i = 0; i < SHORT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "lounpack output vector %d: $%x\n", i, printshort[i]);
    }

}  /* test_unpack */


#endif /* __VEC__ */


/* ************************************************************************* 
 Name: main
 ************************************************************************* */
int main (int argc, char * argv[])
{
  /* test_altivec_feature will exit if the processor */
  /* can not do altivec */
  test_altivec_feature();

  /* test the unpack feature */
  test_unpack();

  return(0);
}

