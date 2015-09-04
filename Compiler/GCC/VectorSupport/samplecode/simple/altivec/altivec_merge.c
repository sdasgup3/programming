/* ************************************************************************* 
* NAME: altivec_merge.c
*
* DESCRIPTION:
*
* this program demonstrates the merge operation; showing how to
* widen a char vector into 2 short vectors by merging with a
* vector of zeroes.
*
* PROCESS:
*
* to compile and run debugging:
* cc -g -o altivec_merge altivec_merge.c -maltivec -mabi=altivec
* ./altivec_merge
*
*  to compile and run optimized:
* cc -O3 -o altivec_merge altivec_merge.c -maltivec -mabi=altivec
* ./altivec_merge
*
* you can use objdump to see the assembly language that is
*   generated at full opt:
*
* cc -ggdb -O3 -c altivec_merge.c -maltivec -mabi=altivec
* objdump -dS altivec_merge.o > o3altivec_merge.dump
*
* or at debug:
*
* cc -ggdb -c altivec_merge.c -maltivec -mabi=altivec
* objdump -dS altivec_merge.o > altivec_merge.dump
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
   NAME:  test_merge
 ************************************************************************* */
void test_merge(void)
{
  fprintf(stderr, "Error: can't generate merge instructions without");
  fprintf(stderr, " an altivec-aware compiler. Did you compile with the ");
  fprintf(stderr, "correct flags?\n");

}

#else

/* ************************************************************************* 

   NAME:  test_merge

   USAGE: 

   test_merge();

   returns: void

   DESCRIPTION:
                 merge a vector of unsigned chars with a vector of
		 zeroes to make two vectors of shorts. Ollmann's
		 tutorial suggests this for creating wider vectors
		 as long as everything is unsigned.

		 do something like the code in test_unpack (altivec_widen.c) 
                 when you are working with signed data

   REFERENCES:

   Ian Ollmann's Altivec Tutorial
   
   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   fprintf
   vec_st - store a vector to memory
   vec_mergeh - merge two vectors, extract the high half
   vec_mergel - merge two vectors, extract the low half
   
   REVISION HISTORY:
        STR                  Description of Revision                 Author
     25-Nov-10               initial coding                           gpk

 ************************************************************************* */

void test_merge(void)
{
  vector unsigned char inputvector = 
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

  /* vec_splat_u8 = 16 element, unsigned char */
  const vector unsigned char zero = vec_splat_u8(0); 

  vector unsigned short himerge;
  vector unsigned short lomerge;
  unsigned char printchar[CHAR_ARRAYSIZE] __attribute__ ((aligned (16)));
  unsigned short printshort[SHORT_ARRAYSIZE] __attribute__ ((aligned (16)));
  int i;

  fprintf(stderr, "--- function %s ------\n", __FUNCTION__);

  /* first, print the inputvector  */
  /* vec_st: Make vector accessible for printing */
  vec_st(inputvector, 0, printchar);

  for (i = 0; i < CHAR_ARRAYSIZE; i++)
    {
      fprintf(stderr, "Input vector %d: $%x\n", i, (int)(printchar[i]));
    }

  /* in this usage, vec_mergeh produces a vector of unsigned chars. */
  /* i'll coerce that into a vector of unsigned shorts. coerce      */
  /* works as long as i don't expect the bits to change.            */

  /* vec_mergeh alternates vector 1 high element then vector 2 high 
     element for numElements/2 */
  /* himerge should contain 00 00 to 00 07 */
  himerge = (vector unsigned short)vec_mergeh(zero, inputvector);

  vec_st(himerge, 0, printshort);

  for (i = 0; i < SHORT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "himerge output vector %d: $%x\n", i, printshort[i]);
    }

  /* vec_mergel alternates vector 1 low element (starting from left)
     then vector 2 low element (starting from left) for numElements/2 */
  /* lomerge should contain 00 08 to 00 0F */
  lomerge = (vector unsigned short)vec_mergel(zero, inputvector);

  vec_st(lomerge, 0, printshort);

  for (i = 0; i < SHORT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "lomerge output vector %d: $%x\n", i, printshort[i]);
    }

} /* test_merge */


#endif /* __VEC__ */


/* ************************************************************************* 
 Name: main
 ************************************************************************* */
int main (int argc, char * argv[])
{
  /* test_altivec_feature will exit if the processor */
  /* can not do altivec */
  test_altivec_feature();

  /* test the merge feature */
  test_merge();

  return(0);
}

