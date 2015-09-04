/* ************************************************************************* 
* NAME: altivec_add_subtract.c
*
* DESCRIPTION:
*
* this is a test program to help me figure out altivec instructions.
* this file covers the add and subtract operations
*
* PROCESS:
*
* to compile and run debugging:
* cc -g -o altivec_add_subtract altivec_add_subtract.c -maltivec -mabi=altivec
* ./altivec_add_subtract
*
*  to compile and run optimized:
* cc -O3 -o altivec_add_subtract altivec_add_subtract.c -maltivec -mabi=altivec
* ./altivec_add_subtract
*
* you can use objdump to see the assembly language that is
*   generated at full opt:
*
* cc -ggdb -O3 -c altivec_add_subtract.c -maltivec -mabi=altivec
* objdump -dS altivec_add_subtract.o > o3altivec_add_subtract.dump
*
* or at debug:
*
* cc -ggdb -c altivec_add_subtract.c -maltivec -mabi=altivec
* objdump -dS altivec_add_subtract.o > altivec_add_subtract.dump
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
*    27-Feb-11          initial coding                        kaj
*    06-Mar-11          added test_mladd                      kaj
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
   NAME:  test_add_subtract
 ************************************************************************* */
void test_add_subtract(void)
{
  fprintf(stderr, "Error: can't generate add and subtract instructions without");
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
 *    NAME:  printVecFloats
 *
 * ************************************************************************* */
void printVecFloats(char *label, vector float outFloats, int arraySize)
{
  float printfloat[arraySize] __attribute__ ((aligned (16)));
  int i;

  vec_st(outFloats, 0, printfloat);

  fprintf(stderr,"%s --> float:{",label);
  for (i = 0; i < arraySize; i++)
  {
     fprintf(stderr,"%f ",printfloat[i]);
  }
  fprintf(stderr,"}\n\n");

} /* printVecFloats */


/* ************************************************************************* 

   NAME:  test_add_subtract

   USAGE: 

   test_add_subtract();

   returns: void

   DESCRIPTION:
                   see how add and subtract work on vectors

   REFERENCES:

   Ian Ollmann's Altivec Tutorial
   
   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   fprintf
   vec_add - add two vectors
   vec_adds - add two vectors, saturation
   vec_sub - subtract two vectors
   
   REVISION HISTORY:
        STR                  Description of Revision                 Author
     27-Feb-11               initial coding                           kaj

 ************************************************************************* */
void test_add_subtract(void)
{
  vector unsigned short addVector1 = 
       { 0, 1000, 5000, 10000, 15000, 20000, 50000, 65535};
  vector signed short addSVector1 = 
       { -32768, -10000, -5000, 0, 10, 5000, 10000, 32767};
  vector signed short addSVector2 = 
       { -10, -10, -10, 0, 10, 10, 10, 10};
  vector unsigned short sumVector;
  vector signed short sumSVector;
  short printshort[SHORT_ARRAYSIZE] __attribute__ ((aligned (16)));


  /* vec_add should wrap, vec_adds will chop at max/min */

  fprintf(stderr,"-----------------------------------------------------------\n");
  
  /* add 10 to each element - unsigned short */
  printVecUShorts("vec_add unsigned short input vector", addVector1,SHORT_ARRAYSIZE); 
  sumVector = vec_add(addVector1, vec_splat_u16(10));
  printVecUShorts("vec_add sum vector (Input+10) ", sumVector,SHORT_ARRAYSIZE);

  /* add 10 to each element using saturation add - unsigned short */
  sumVector = vec_adds(addVector1, vec_splat_u16(10));
  printVecUShorts("vec_adds sum vector (Input+10)", sumVector,SHORT_ARRAYSIZE);

  /* subtract 10 from each element - unsigned short */
  sumVector = vec_sub(addVector1, vec_splat_u16(10));
  printVecUShorts("vec_sub sum vector (Input-10) ", sumVector,SHORT_ARRAYSIZE);

  fprintf(stderr,"-----------------------------------------------------------\n\n");

   /* add 10 to each element - signed short */
  printVecShorts("vec_add signed short input vector", addSVector1,SHORT_ARRAYSIZE); 
  sumSVector = vec_add(addSVector1,addSVector2);
  printVecShorts("vec_add sum vector (increment pos & neg by 10) ",
                      sumSVector,SHORT_ARRAYSIZE);

   /* add 10 to each element using saturation add - signed short */
  sumSVector = vec_adds(addSVector1,addSVector2);
  printVecShorts("vec_adds sum vector (increment pos & neg by 10)",
                     sumSVector,SHORT_ARRAYSIZE);

  /* subtract 10 from each element - signed short */
  sumSVector = vec_sub(addSVector1,addSVector2);
  printVecShorts("vec_sub vector (decrement pos & neg by 10) ",
                      sumSVector,SHORT_ARRAYSIZE);


} /* test_add_subtract */


/* ************************************************************************* 

   NAME:  test_add_subtract_float

   USAGE: 

   test_add_subtract_float();

   returns: void

   DESCRIPTION:
                   see how add and subtract work with float vectors

   REFERENCES:

   Ian Ollmann's Altivec Tutorial
   
   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   fprintf
   vec_add - add two vectors
   vec_sub - subtract two vectors
   
   REVISION HISTORY:
        STR                  Description of Revision                 Author
     27-Feb-11               initial coding                           kaj

 ************************************************************************* */
void test_add_subtract_float(void)
{
  vector float addFVector1 =
       { 0.0, 10.0, 50.0, 100.0};
  vector float addFVector2 = 
       { 100.0, 100.0, 100.0, 100.0};
  vector float sumFVector;
  short printfloat[FLOAT_ARRAYSIZE] __attribute__ ((aligned (16)));

  fprintf(stderr,"-----------------------------------------------------------\n\n");

   /* add float vectors */
  sumFVector = vec_add(addFVector1, addFVector2);  
  printVecFloats("vec_add Float vector 1", addFVector1,FLOAT_ARRAYSIZE);
  printVecFloats("vec_add Float sum vector (Input+100.0)", sumFVector,FLOAT_ARRAYSIZE);

   /* subtract float vectors */
  sumFVector = vec_sub(addFVector1, addFVector2);  
  printVecFloats("vec_sub Float vector (Input-100.0)", sumFVector,FLOAT_ARRAYSIZE);

} /* test_add_subtract_float */


/* ************************************************************************* 

   NAME:  test_madd

   USAGE: 

   test_madd();

   returns: void

   DESCRIPTION:
                   see how the combination multiply/add operation works

   REFERENCES:

   Ian Ollmann's Altivec Tutorial
   
   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   fprintf
   vec_madd - multiply two float vectors and add to the sum a float value 
              all in one operation
 
   
   REVISION HISTORY:
        STR                  Description of Revision                 Author
     27-Feb-11               initial coding                           kaj

 ************************************************************************* */
void test_madd(void)
{
  vector float floatVector1 =
       { -17.777777, 0.0, 37.777777, 100.0};
  vector float addVector = 
       { 32.0, 32.0, 32.0, 32.0};
  vector float coeffVector = 
       { 1.8, 1.8, 1.8, 1.8};
  vector float fahrenheitVector;
  short printfloat[FLOAT_ARRAYSIZE] __attribute__ ((aligned (16)));

  /* print vectors performing madd on */
  fprintf(stderr,"-----------------------------------------------------------\n\n");
  printVecFloats("vec_madd input vector 1", floatVector1,FLOAT_ARRAYSIZE);
  printVecFloats("vec_madd input vector to add", addVector,FLOAT_ARRAYSIZE);  
  printVecFloats("vec_madd coeffvector to multiply", coeffVector,FLOAT_ARRAYSIZE); 
  
  /* calculate */
  fahrenheitVector = vec_madd(floatVector1,coeffVector,addVector);
  printVecFloats("vec_madd vector (Input*9/5+32)", fahrenheitVector,FLOAT_ARRAYSIZE);

} /* test_madd */


/* ************************************************************************* 

   NAME:  test_mladd

   USAGE: 

   test_madd();

   returns: void

   DESCRIPTION:
                   see how the combination multiply/add operation works
                   this will work on low order 16-bits 

   REFERENCES:

   Ian Ollmann's Altivec Tutorial
   
   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   fprintf
   vec_madd - multiply two short vectors and add to the sum a short
              all in one operation
 
   
   REVISION HISTORY:
        STR                  Description of Revision                 Author
     06-Mar-11               initial coding                           kaj

 ************************************************************************* */
void test_mladd(void)
{
  vector unsigned short shortVector1 =
       { 0, 2, 4, 8, 16, 32, 64, 128 };
  vector unsigned short addVector;
  vector unsigned short coeffVector;
  vector unsigned short resultVector;
  vector short shortVector2 =
       { -128, -64, -32, -16, 0, 16, 32, 64};
  vector short addVector2 = 
       { -10, -10, -10, -10, 0, 10, 10, 10};
  vector short coeffVector2;
  vector short resultVector2;
  short printshort[SHORT_ARRAYSIZE] __attribute__ ((aligned (16)));

  coeffVector = vec_splat_u16(2);
  addVector = vec_splat_u16(0);

  /* print vectors performing mladd on */
  fprintf(stderr,"-----------------------------------------------------------"
                 "\n\n");
  printVecUShorts("vec_mladd unsigned input vector 1", shortVector1,
                   SHORT_ARRAYSIZE);
  printVecUShorts("vec_mladd unsigned input vector to add", addVector,
                   SHORT_ARRAYSIZE);  
  printVecUShorts("vec_mladd unsigned coeffvector to multiply", coeffVector,
                   SHORT_ARRAYSIZE); 
  
  /* calculate */
  resultVector = vec_mladd(shortVector1,coeffVector,addVector);
  printVecUShorts("vec_mladd vector (Input*2+0)", resultVector,
                   SHORT_ARRAYSIZE);

  /* signed shorts */
  coeffVector2 = vec_splat_s16(2);
    
  /* print signed short vectors performing mladd on */
  fprintf(stderr,"----------------------------------------------------------"
                 "\n\n");
  printVecShorts("vec_mladd signed input vector 1", shortVector2,
                  SHORT_ARRAYSIZE);
  printVecShorts("vec_mladd signed input vector to add", addVector2,
                  SHORT_ARRAYSIZE);  
  printVecShorts("vec_mladd signed coeffvector to multiply", coeffVector2,
                  SHORT_ARRAYSIZE); 
  
  /* calculate */
  resultVector2 = vec_mladd(shortVector2,coeffVector2,addVector2);
  printVecShorts("vec_mladd vector (Input*2 + 10(increment pos & neg by 10)",
                  resultVector2,SHORT_ARRAYSIZE);

} /* test_mladd */



#endif /* __VEC__ */


/* ************************************************************************* 
 Name: main
 ************************************************************************* */
int main (int argc, char * argv[])
{
  /* test_altivec_feature will exit if the processor */
  /* can not do altivec */
  test_altivec_feature();

  /* test the add and subtract features */
  test_add_subtract();
  test_add_subtract_float();
  test_madd();
  test_mladd();
  
  return(0);
}

