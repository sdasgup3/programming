/* ************************************************************************* 
* NAME: altivec_char2float.c
*
* DESCRIPTION:
*
* this program shows how to turn an array of chars into vectors of floats.
* you might do this if your input was bytes, but you wanted to to floating
* point operations on it.
*
* PROCESS:
*
* to compile and run debugging:
* cc -g -o altivec_char2float altivec_char2float.c -maltivec -mabi=altivec
* ./altivec_char2float
*
*  to compile and run optimized:
* cc -O3 -o altivec_char2float altivec_char2float.c -maltivec -mabi=altivec
* ./altivec_char2float
*
* you can use objdump to see the assembly language that is
*   generated at full opt:
*
* cc -ggdb -O3 -c altivec_char2float.c -maltivec -mabi=altivec
* objdump -dS altivec_char2float.o > o3altivec_char2float.dump
*
* or at debug:
*
* cc -ggdb -c altivec_char2float.c -maltivec -mabi=altivec
* objdump -dS altivec_char2float.o > altivec_char2float.dump
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
*    14-Jan-11        added test_widen                        gpk
*    16-Apr-11  added comments to char_to_float, test_widen   gpk
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



/* ************************************************************************* 


   NAME:  char_to_float


   USAGE: 

    
   const unsigned char * sourcep = malloc(...);
   vector float mp0;
   vector float mp1;
   vector float mp2;
   vector float mp3;

   char_to_float(sourcep, &mp0, &mp1, &mp2, &mp3);

   returns: void

   DESCRIPTION:
                 figure out how to turn an unsigned char vector into a
		 float vector by merging it with another unsigned char
		 vector to form a short vector, then merging with another
		 short vector to form an int vector , then using vec_ctf to
		 convert the int vector into a float vector.


                  modifies mp0, mp1, mp2, mp3 

		 this is sort of the inverse of condense_float_rgbas
   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   vec_ld - load a vector from memory
   vec_splat_u8 - build unsigned char vector with given value
   vec_unpackh - extract the high end of a vector into a wider vector
   vec_unpackl - extract the low end of a vector into a wider vector
   vec_ctf - convert int vector into float vector

   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     25-Nov-10               initial coding                           gpk
     16-Apr-11            added more comments                         gpk
     
 ************************************************************************* */

void char_to_float(const unsigned char * sourcep, vector float *mp0,
		     vector float * mp1, vector float * mp2,
		     vector float * mp3)
{
  vector unsigned char inputvector;
  vector signed short hiunpack, lounpack;
  vector signed int intvec0, intvec1, intvec2, intvec3; 

  inputvector = vec_ld(0, sourcep);

  
  /* merge the high halvess of the inputvector (unsigned chars) and a  */
  /* vector of zeroes (created with vec_splat_u8) using vec_mergeh */
  /* this creates a vector of shorts.  */
  
  hiunpack = (vector signed short)vec_mergeh(vec_splat_u8(0), inputvector);

  /* now use  vec_mergel to do the same thing with the lower halves. */
  
  lounpack = (vector signed short)vec_mergel(vec_splat_u8(0), inputvector);

  /* now call vec_unpackh and vec_unpackl to dissect hiunpack and lounpack  */
  /* into 4 integer vectors.   */
  
  intvec0 = vec_unpackh(hiunpack);
  intvec1 = vec_unpackl(hiunpack);

  intvec2 = vec_unpackh(lounpack);
  intvec3 = vec_unpackl(lounpack);

  /* finally use vec_ctf on those integer vectors to turn them to float  */
  /* vectors and store them where mp0 - mp3 point.  */
  *mp0 = vec_ctf(intvec0, 0);
  *mp1 = vec_ctf(intvec1, 0);
  *mp2 = vec_ctf(intvec2, 0);
  *mp3 = vec_ctf(intvec3, 0);
}


/* ************************************************************************* 

   NAME:  test_widen

   USAGE: 

   test_widen();

   returns: void

   DESCRIPTION:
                  show how to turn an array of chars into vectors of floats.
		  set up environment around char_to_float with arrays
		  of chars to read from and arrays of floats to store the
		  results into and print

   REFERENCES:

   Ian Ollmann's Altivec Tutorial
   
   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   char_to_float
   vec_st - store vector in memory

   REVISION HISTORY:
        STR                  Description of Revision                 Author
     14-Jan-11               initial coding                           gpk
     16-Apr-11            added more comments                         gpk
   
************************************************************************* */

void test_widen()
{
  unsigned char input[CHAR_ARRAYSIZE] __attribute__ ((aligned (16)))
    = {0, 1, 2, 3, 4, 5, 6, 7, 248, 249, 250, 251, 252,  253, 254, 255};
  signed char printchar[CHAR_ARRAYSIZE] __attribute__ ((aligned (16)));
  float printfloat[FLOAT_ARRAYSIZE] __attribute__ ((aligned (16)));
  vector float output0, output1, output2, output3;
  int i;
  char *outputname;

  fprintf(stderr, "--- function %s ------\n", __FUNCTION__);

  char_to_float(input, &output0, &output1, &output2, &output3);
  /* first, print the input  */
  for (i = 0; i < CHAR_ARRAYSIZE; i++)
    {
      fprintf(stderr, "Input vector %d: %d\n", i, (int)(input[i]));
    }
  
  vec_st(output0, 0, printfloat);

  fprintf(stderr, "\n");
  outputname = "output0";
  for (i = 0; i < FLOAT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%s %d: %f\n", outputname, i, (printfloat[i]));
    } 
 
  vec_st(output1, 0, printfloat);
  fprintf(stderr, "\n");
  outputname = "output1";
  for (i = 0; i < FLOAT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%s %d: %f\n", outputname, i, (printfloat[i]));
    } 

  fprintf(stderr, "\n");
  vec_st(output2, 0, printfloat);

  outputname = "output2";
  for (i = 0; i < FLOAT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%s %d: %f\n", outputname, i, (printfloat[i]));
    }   

  vec_st(output3, 0, printfloat);
  fprintf(stderr, "\n");
  outputname = "output3";
  for (i = 0; i < FLOAT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%s %d: %f\n", outputname, i, (printfloat[i]));
    }   


}  /* test_widen */



/* ************************************************************************* 
 Name: main
 ************************************************************************* */
int main (int argc, char * argv[])
{
  /* test_altivec_feature will exit if the processor */
  /* can not do altivec */
  test_altivec_feature();

  /* test the unpack feature */
  test_widen();

  return(0);
}

