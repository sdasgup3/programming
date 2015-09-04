/* ************************************************************************* 
* NAME: altivec_select.c
*
* DESCRIPTION:
*
* this program demonstrates the vector select (vec_sel) function.
* vec_sel lets you select bits from two vectors to form a third one.
*
* PROCESS:
*
* to compile and run debugging:
* cc -g -o altivec_select altivec_select.c -maltivec -mabi=altivec
* ./altivec_select
*
*  to compile and run optimized:
* cc -O3 -o altivec_select altivec_select.c -maltivec -mabi=altivec
* ./altivec_select
*
* you can use objdump to see the assembly language that is
*   generated at full opt:
*
* cc -ggdb -O3 -c altivec_select.c -maltivec -mabi=altivec
* objdump -dS altivec_select.o > o3altivec_select.dump
*
* or at debug:
*
* cc -ggdb -c altivec_select.c -maltivec -mabi=altivec
* objdump -dS altivec_select.o > altivec_select.dump
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
   NAME:  test_select
 ************************************************************************* */
void test_select(void)
{
  fprintf(stderr, "Error: can't generate unpack instructions without");
  fprintf(stderr, " an altivec-aware compiler. Did you compile with the ");
  fprintf(stderr, "correct flags?\n");
}

#else

/* ************************************************************************* 


   NAME:  test_select


   USAGE: 


   test_select();

   returns: void

   DESCRIPTION:
                 demonstrate vec_sel to create a vector using bits
		 from two other vectors and a selection vector.

		 if the bit from the selection vector is 0, the
		 bit from the first argument is taken. otherwise
		 the bit from the second argument is taken.

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   fprintf
   vec_sel - build a vector with selected bits from others
   vec_st - store vector to memory
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     25-Nov-10               initial coding                           gpk

 ************************************************************************* */

void test_select(void)
{
  vector float firstvector = {1.0, 2.0, 3.0, 4.0};
  vector float secondvector = {100.0, 200.0, 300.0, 400.0};
  vector unsigned int selector = {0, 0, 0, 0xffffffff};
  vector float outputvector;
  float printfloat[FLOAT_ARRAYSIZE];
  int i;

  fprintf(stderr, "--- function %s ------\n", __FUNCTION__);

  /* i'm expecting the output here to be {1.0, 2.0, 3.0, 400.0} */
  /* select selector has zeros for all elements of the first three */
  /* vectors, they should get the bits from the firstvector. since */
  /* selector is all ones for the fourth element, the fourth element */
  /* of outputvector should get the fourth element of secondvector */
 
  outputvector = vec_sel(firstvector, secondvector, selector);

  vec_st(outputvector, 0, printfloat);


  fprintf(stderr, "selected vector:  ");

  for(i = 0; i < FLOAT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%f ", printfloat[i]);
    }
  fprintf(stderr, "\n");

}  /* test_select */

#endif  /* __VEC__ */


/* ************************************************************************* 
 Name: main
 ************************************************************************* */
int main (int argc, char * argv[])
{
  /* test_altivec_feature will exit if the processor */
  /* can not do altivec */
  test_altivec_feature();

  /* test the select feature */
  test_select();

  return(0);
}

