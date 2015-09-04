/* ************************************************************************* 
* NAME: altivec_shift.c
*
* DESCRIPTION:
*
* this program demonstrates some of the vector shift operations.
* vec_sr, vec_sra, vec_srl are some of the right shift operations. there
* are vec_sl, vec_sla, vec_sll for corresponding left shift. 
*
* PROCESS:
*
* to compile and run debugging:
* cc -g -o altivec_shift altivec_shift.c -maltivec -mabi=altivec
* ./altivec_shift
*
*  to compile and run optimized:
* cc -O3 -o altivec_shift altivec_shift.c -maltivec -mabi=altivec
* ./altivec_shift
*
* you can use objdump to see the assembly language that is
*   generated at full opt:
*
* cc -ggdb -O3 -c altivec_shift.c -maltivec -mabi=altivec
* objdump -dS altivec_shift.o > o3altivec_shift.dump
*
* or at debug:
*
* cc -ggdb -c altivec_shift.c -maltivec -mabi=altivec
* objdump -dS altivec_shift.o > altivec_shift.dump
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
*    28-Feb-11        initial coding                         kaj
*    16-Apr-11   added to comments in file header            gpk
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
   NAME:  test_shift
 ************************************************************************* */
void test_shift(void)
{
  fprintf(stderr, "Error: can't generate shift instructions without");
  fprintf(stderr, " an altivec-aware compiler. Did you compile with the ");
  fprintf(stderr, "correct flags?\n");

}

#else

/* ************************************************************************* 
 *    NAME:  printVecUShorts
 *
 * ************************************************************************* */
void printVecUShorts(char *label, vector unsigned short outShorts, int arraySize)
{
  unsigned short printshort[arraySize] __attribute__ ((aligned (16)));
  int i;

  vec_st(outShorts, 0, printshort);

  fprintf(stderr,"%s --> unsigned short:{",label);
  for (i = 0; i < arraySize; i++)
  {
     fprintf(stderr,"%d ",printshort[i]);
  }
  fprintf(stderr,"}\n\n");
  
} /* printVecUShorts */


/* ************************************************************************* 
 *    NAME:  printVecShorts
 *
 * ************************************************************************* */
void printVecShorts(char *label, vector signed short outShorts, int arraySize)
{
  signed short printshort[arraySize] __attribute__ ((aligned (16)));
  int i;

  vec_st(outShorts, 0, printshort);

  fprintf(stderr,"%s --> short:{",label);
  for (i = 0; i < arraySize; i++)
  {
     fprintf(stderr,"%d ",printshort[i]);
  }
  fprintf(stderr,"}\n\n");

} /* printVecShorts */



/* ************************************************************************* 

   NAME:  test_shift_unsigned

   USAGE: 

   test_shift_unsigned();

   returns: void

   DESCRIPTION:
                   see how shift works on unsigned vectors

   REFERENCES:

   Ian Ollmann's Altivec Tutorial
   
   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   fprintf
   vec_sr  - shift right
   vec_sra - algebraic shift right
   vec_srl - shift right long

   
   REVISION HISTORY:
        STR                  Description of Revision                 Author
     28-Feb-11               initial coding                           kaj

 ************************************************************************* */
void test_shift_unsigned(void)
{
  vector unsigned short shiftVector1 = 
       {0,1,2,3,4,5,6,7};
  vector unsigned short inputVector1 = 
       {128,128,128,128,128,128,128,128};
  vector unsigned short inputVector2 = 
       {65535,65535,65535,65535,65535,65535,65535,65535};
  vector unsigned short resultVector;
  short printshort[SHORT_ARRAYSIZE] __attribute__ ((aligned (16)));

  fprintf(stderr,"-----------------------------------------------------------\n");

  /* vec_sr, vec_sra, vec_srl */
  resultVector = vec_sr(vec_splat_u16(8),shiftVector1);
  printVecUShorts("vec_sr 8 shifted to right 0..7", resultVector,SHORT_ARRAYSIZE);
  resultVector = vec_sra(vec_splat_u16(8),shiftVector1);
  printVecUShorts("vec_sra 8 shifted to right 0..7", resultVector,SHORT_ARRAYSIZE);
  resultVector = vec_srl(vec_splat_u16(8),shiftVector1);
  printVecUShorts("vec_srl 8 shifted to right 0..7", resultVector,SHORT_ARRAYSIZE);

  fprintf(stderr,"-----------------------------------------------------------\n");

  /* vec_sr, vec_sra, vec_srl */
  resultVector = vec_sr(inputVector1,shiftVector1);
  printVecUShorts("vec_sr 128 shifted to right 0..7", resultVector,SHORT_ARRAYSIZE);
  resultVector = vec_sra(inputVector1,shiftVector1);
  printVecUShorts("vec_sra 128 shifted to right 0..7", resultVector,SHORT_ARRAYSIZE);
  resultVector = vec_srl(inputVector1,shiftVector1);
  printVecUShorts("vec_srl 128 shifted to right 0..7", resultVector,SHORT_ARRAYSIZE);

  fprintf(stderr,"-----------------------------------------------------------\n");

  /* vec_sr, vec_sra, vec_srl */
  resultVector = vec_sr(inputVector2,shiftVector1);
  printVecUShorts("vec_sr 65535 shifted to right 0..7", resultVector,SHORT_ARRAYSIZE);
  resultVector = vec_sra(inputVector2,shiftVector1);
  printVecUShorts("vec_sra 65535 shifted to right 0..7", resultVector,SHORT_ARRAYSIZE);
  resultVector = vec_srl(inputVector2,shiftVector1);
  printVecUShorts("vec_srl 65535 shifted to right 0..7", resultVector,SHORT_ARRAYSIZE);

} /* test_shift_unsigned */



/* ************************************************************************* 

   NAME:  test_shift_signed

   USAGE: 

   test_shift_signed();

   returns: void

   DESCRIPTION:
                   see how shift works on signed vectors

   REFERENCES:

   Ian Ollmann's Altivec Tutorial
   
   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   fprintf
   vec_sr  - shift right
   vec_sra - algebraic shift right
   vec_srl - shift right long

   
   REVISION HISTORY:
        STR                  Description of Revision                 Author
     28-Feb-11               initial coding                           kaj

 ************************************************************************* */
void test_shift_signed(void)
{
  vector unsigned short shiftVector1 = 
       {0,1,2,3,4,5,6,7};
  vector signed short inputVector1 = 
       {-128,-128,-128,-128,-128,-128,-128,-128};
  vector signed short inputVector2 = 
       {128,128,128,128,128,128,128,128};
  vector signed short inputVector3 = 
       {65535,65535,65535,65535,65535,65535,65535,65535};
  vector signed short resultVector;
  short printshort[SHORT_ARRAYSIZE] __attribute__ ((aligned (16)));

  fprintf(stderr,"-----------------------------------------------------------\n");

  /* vec_sr, vec_sra, vec_srl */
  resultVector = vec_sr(vec_splat_s16(-16),shiftVector1);
  printVecShorts("vec_sr -16 shifted to right 0..7", resultVector,SHORT_ARRAYSIZE);
  resultVector = vec_sra(vec_splat_s16(-16),shiftVector1);
  printVecShorts("vec_sra -16 shifted to right 0..7", resultVector,SHORT_ARRAYSIZE);
  resultVector = vec_srl(vec_splat_s16(-16),shiftVector1);
  printVecShorts("vec_srl -16 shifted to right 0..7", resultVector,SHORT_ARRAYSIZE);

  fprintf(stderr,"-----------------------------------------------------------\n");

  /* vec_sr, vec_sra, vec_srl */
  resultVector = vec_sr(inputVector1,shiftVector1);
  printVecShorts("vec_sr -128 shifted to right 0..7", resultVector,SHORT_ARRAYSIZE);
  resultVector = vec_sra(inputVector1,shiftVector1);
  printVecShorts("vec_sra -128 shifted to right 0..7", resultVector,SHORT_ARRAYSIZE);
  resultVector = vec_srl(inputVector1,shiftVector1);
  printVecShorts("vec_srl -128 shifted to right 0..7", resultVector,SHORT_ARRAYSIZE);

  fprintf(stderr,"-----------------------------------------------------------\n");

  /* vec_sr, vec_sra, vec_srl */
  resultVector = vec_sr(inputVector2,shiftVector1);
  printVecShorts("vec_sr 128 shifted to right 0..7", resultVector,SHORT_ARRAYSIZE);
  resultVector = vec_sra(inputVector2,shiftVector1);
  printVecShorts("vec_sra 128 shifted to right 0..7", resultVector,SHORT_ARRAYSIZE);
  resultVector = vec_srl(inputVector2,shiftVector1);
  printVecShorts("vec_srl 128 shifted to right 0..7", resultVector,SHORT_ARRAYSIZE);

  fprintf(stderr,"-----------------------------------------------------------\n");

  /* vec_sr, vec_sra, vec_srl */
  resultVector = vec_sr(inputVector3,shiftVector1);
  printVecShorts("vec_sr 65535 shifted to right 0..7", resultVector,SHORT_ARRAYSIZE);
  resultVector = vec_sra(inputVector3,shiftVector1);
  printVecShorts("vec_sra 65535 shifted to right 0..7", resultVector,SHORT_ARRAYSIZE);
  resultVector = vec_srl(inputVector3,shiftVector1);
  printVecShorts("vec_srl 65535 shifted to right 0..7", resultVector,SHORT_ARRAYSIZE);

} /* test_shift_signed */

#endif /* __VEC__ */


/* ************************************************************************* 
 Name: main
 ************************************************************************* */
int main (int argc, char * argv[])
{
  /* test_altivec_feature will exit if the processor */
  /* can not do altivec */
  test_altivec_feature();

  /* test the shift features */
  test_shift_unsigned();
  test_shift_signed();
  
  return(0);
}

