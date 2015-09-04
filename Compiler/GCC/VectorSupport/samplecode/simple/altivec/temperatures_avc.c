/* ************************************************************************* 
* NAME: altivec/temperatures_avc.c
*
* DESCRIPTION:
*
* a simple example using gcc's altivec intrinsics to convert an array
* of temperatures in degrees Celsius to degrees Fahrenheit
*
* PROCESS:
*
* to compile and run debugging:
* cc -g -o temperatures_avc temperatures_avc.c -maltivec -mabi=altivec
* ./temperatures_avc
*
*  to compile and run optimized:
* cc -O3 -o temperatures_avc temperatures_avc.c -maltivec -mabi=altivec
* ./temperatures_avc
*
* you can use objdump to see the assembly language that is
*   generated at full opt:
*
* cc -ggdb -O3 -c temperatures_avc.c -maltivec -mabi=altivec
* objdump -dS temperatures_avc.o > o3temperatures_avc.dump
*
* or at debug:
*
* cc -ggdb -c temperatures_avc.c -maltivec -mabi=altivec
* objdump -dS temperatures_avc.o > temperatures_avc.dump
*
* GLOBALS: none
*
* REFERENCES:
*
* http://developer.apple.com/hardwaredrivers/ve/sse.html#Translation
* GCC info page for PowerPC built-ins
* AltiVec Technology Programming Interface Manual
*
*
* LIMITATIONS:
*
* * test_altivec_feature parses /proc/cpuinfo, which can change format
*   between releases.
*
* * be careful about the ranges of temperatures. temperatures and
*   partial results from the formula must fit into an 16-bit integer
*   for altivec_integers.
*
* * be careful to make sure that right shift rounds in the direction
*   that you want for your formula. for this example i'm not worrying
*   about it.
*
* * altivec_fixedpoint uses integer math so this will be an approximation
*
*
* REVISION HISTORY:
*
*   STR                Description                          Author
*
*    2-Oct-10          initial coding                        gpk
*
* TARGET: GNU C version 4 or higher
*
* This file is in the public domain. If it breaks, you get to keep both
* pieces.
* 
*
* ************************************************************************* */

#include <stdio.h>
#include <stdlib.h> /* exit  */
#include <string.h> /* memcpy */

#include <altivec.h>

/*  MAXLINE - the longest line i'll read from /proc/cpuinfo */

#define MAXLINE 255

/* protoytpes  */
void test_altivec_feature(void);
void altivec_float(void);
void altivec_fixedpoint(void);

/* FLOAT_ARRAYSIZE - the number of elements in a float array that  */
/* make it the same size as a float vector                         */

#define FLOAT_ARRAYSIZE (sizeof(vector float) / sizeof(float))


/* SHORT_ARRAYSIZE - the number of elements in a short array that  */
/* make it the same size as a short vector                         */
#define SHORT_ARRAYSIZE (sizeof(vector short) / sizeof(short))


/* ************************************************************************* 
     MAIN
   ************************************************************************* */

int main (int argc, char * argv[])
{

  /* exit if we don't the processor can't do altivec */

  test_altivec_feature();

  /* first do the temperature conversion with floating point math  */
  altivec_float();

  /* now with fixed point math  */
  altivec_fixedpoint();
  
}



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
  else
    {
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
    }
}




/* ************************************************************************* 


   NAME:  build4float


   USAGE: 

   vector float somevector;
   float e0;
   float e1;
   float e2;
   float e3;

   somevector = build4float(e0, e1, e2, e3);

   returns:  vector float 

   DESCRIPTION:
                 return a vector of 4 floats whose elements
		 have the values e0, e1, e2, e3
		 
		 this is a convenience function.
		 inline it to make things run faster.
   REFERENCES:

   LIMITATIONS:

 
   FUNCTIONS CALLED:
  
   vec_ld - load a vector from memory
 
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     15-Oct-10               initial coding                           gpk

 ************************************************************************* */

static vector float build4float(float e0, float e1, float e2, float e3)
{
  float floatarray[FLOAT_ARRAYSIZE]__attribute__ ((aligned (16)));
  vector float retval;

  /* load up floatarray  */
  
  floatarray[0] = e0; floatarray[1] = e1;
  floatarray[2] = e2; floatarray[3] = e3;

  /* load floatarray into retval  */
  retval = vec_ld(0, floatarray);

  return(retval);
  
}


/* ************************************************************************* 


   NAME:  build1float


   USAGE: 

   vector float somevector;
   float e0;
   float e1;
   float e2;
   float e3;

   somevector =  build1float(value)

   returns:  vector float 

   DESCRIPTION:
                 return a vector of 4 floats whose elements
		 all have the given value 
		 
		 this is a convenience function.
		 inline it to make things run faster.
   REFERENCES:

   LIMITATIONS:

   it would be faster to use vec_splat_* for this.

   FUNCTIONS CALLED:
   
   vec_ld - load a vector from memory
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     15-Oct-10               initial coding                           gpk

 ************************************************************************* */

static vector float build1float(float value)
{
  float floatarray[FLOAT_ARRAYSIZE]__attribute__ ((aligned (16)));
  vector float retval;

  /* assign value to all elements of floatarray  */
  floatarray[0] = value; floatarray[1] = value;
  floatarray[2] = value; floatarray[3] = value;

  /* load floatarray into retval  */
  
  retval = vec_ld(0, floatarray);

  return(retval);
  
}







/* ************************************************************************* 


   NAME:  altivec_float


   USAGE: 


   altivec_float();

   returns: void

   DESCRIPTION:
                 this example code uses altivec intrinsics to
		 convert the celsius temperatures in
		 celsiusarray1 & celsiusarray2 to fahrenheit and
		 print them out.

   REFERENCES:

   LIMITATIONS:


   FUNCTIONS CALLED:

   build1float
   vec_ld - load a vector from memory
   vec_madd - combined multiply and add
   vec_st - store a vector into memory
   fprintf
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     15-Oct-10               initial coding                           gpk

 ************************************************************************* */

void altivec_float(void)
{
  vector float celsiusvector, fahrenheitvector, partialvector;
  vector float coeffvector;
  const vector float thirtytwovector __attribute__ ((aligned (16)))
     = {32.0, 32.0, 32.0, 32.0};
  float celsiusarray1[] __attribute__ ((aligned (16))) =
    {-100.0, -80.0, -40.0, 0.0};
  float celsiusarray2[] __attribute__ ((aligned (16))) =
    {  32.0,  40.0,  80.0, 100.0};
  float fahrenheitarray[FLOAT_ARRAYSIZE] __attribute__ ((aligned (16)));
  int i;

  fprintf(stderr, "--- function %s ------\n", __FUNCTION__);
  
  /* set coeffvector to have all elements equal to 9.0/5.0 */
  
  coeffvector = build1float(9.0f / 5.0f);
  
  /* pull celsiusarray1 into celsiusvector  */

  celsiusvector = vec_ld(0, celsiusarray1);

  /* make fahrenheitvector the result of multiplying celsiusvector  */
  /* by coeffvector, then adding thirtytwovector to that.  */
  
  fahrenheitvector = vec_madd(celsiusvector, coeffvector, thirtytwovector);
 
  /* copy fahrenheitvector into fahrenheitarray  */
  
  vec_st(fahrenheitvector, 0, fahrenheitarray);

  /* now print out the results.  */

  fprintf(stderr, "Using floating point vectors...\n");
  
  for (i = 0; i < FLOAT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%f celsius is %f fahrenheit\n",
	      celsiusarray1[i], fahrenheitarray[i]);
    }


  /* pull celsiusarray2 into celsiusvector  */
  /* and repeat the steps above...  */
  
 celsiusvector = vec_ld(0, celsiusarray2);

  /* make fahrenheitvector the result of multiplying celsiusvector  */
  /* by coeffvector, then adding thirtytwovector to that.  */
  
  fahrenheitvector = vec_madd(celsiusvector, coeffvector, thirtytwovector);
 
  /* copy fahrenheitvector into fahrenheitarray  */
  
  vec_st(fahrenheitvector, 0, fahrenheitarray);

  /* now print out the results.  */
  
  for (i = 0; i < FLOAT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%f celsius is %f fahrenheit\n",
	      celsiusarray2[i], fahrenheitarray[i]);
    }


}





/* ************************************************************************* 


   NAME:  build1short


   USAGE: 

   vector short avector
   short value;

   avector =  build1short(value);

   returns: vector short

   DESCRIPTION:
                 set all 8 elements of a short vector to be the
		 given value and return that vector.

   REFERENCES:

   LIMITATIONS:

   it would be faster to use vec_splat_* for this.
 
   FUNCTIONS CALLED:
   
   vec_ld - load a vector from memory
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     15-Oct-10               initial coding                           gpk

 ************************************************************************* */

static vector short build1short(short value)
{
  short shortarray[SHORT_ARRAYSIZE] __attribute__ ((aligned (16)));
  vector short retval;

  shortarray[0] = value;  shortarray[1] = value;
  shortarray[2] = value;  shortarray[3] = value;
  shortarray[4] = value;  shortarray[5] = value;
  shortarray[6] = value;  shortarray[7] = value;

  retval = vec_ld(0, shortarray);

  return(retval);
  
}



/* ************************************************************************* 


   NAME:  altivec_fixedpoint


   USAGE: 

 
   altivec_fixedpoint();

   returns: void

   DESCRIPTION:
                 this example code uses altivec intrinsics to
		 convert the celsius values in celsiusarray
		 into fahrenheit and print them out

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: 

      modified: 

   FUNCTIONS CALLED:
   
   vec_ld - load a vector from memory
   build1short
   vec_mladd - combined multiply & add
   vec_sra - arithmetic shift right
   vec_adds - saturated add 
   vec_st - store vector to memory
   fprintf
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     15-Oct-10               initial coding                           gpk
     06-Mar-11               added comment re truncation on vec_sra   kaj

 ************************************************************************* */

void altivec_fixedpoint(void)
{
  short celsiusarray[] __attribute__ ((aligned (16)))
    = {-100, -80, -40, 0,  32,  40,  80, 100};
  
  short fahrenheitarray[SHORT_ARRAYSIZE] __attribute__ ((aligned (16)));
  vector short  celsiusvector, fahrenheitvector, partial;
  const vector short zerovec = {0, 0, 0, 0, 0, 0, 0, 0};
  const vector unsigned short shiftvec = {6, 6, 6, 6, 6, 6, 6, 6};
  vector short coeffvector;
  vector short  thirtytwovector;
  int i;
  short coefficient;

  fprintf(stderr, "--- function %s ------\n", __FUNCTION__);
    
  /* load the array into  celsiusvector */
  
  celsiusvector = vec_ld(0, celsiusarray);


  /* we're doing scaled arithmetic: instead of multiplying by  */
  /* 9/5, we'll multiply by 64 * 9/5, then shift left 6 places  */
  /* to divide by 64.  */
  /* so compute  64 * 9/5 (as an integer), then fill coeffvector with */
  /* that value.   */
  
  coefficient = (short)( (9 * 64) / 5);
  coeffvector = build1short(coefficient);

  /* give me a vector of 32's to add to the end.  */
  
  thirtytwovector = build1short(32);

  /* make partial the result of multiplying celsiusvector  */
  /* by coeffvector, then adding zerovector to that.  */
  
  partial = vec_mladd(celsiusvector, coeffvector, zerovec);

  /* now shift right 6 places to simulate dividing by 64  */
  /* values are being truncated here which could cause */
  /* final values to be off slightly */
  
  partial = vec_sra(partial, shiftvec);

  /* and add 32 to each element of partial to get fahrenheitvector  */

  fahrenheitvector = vec_adds(partial, thirtytwovector);

  /* store the results back in  fahrenheitvector so we can  */
  /* print out the results.   */
  
  vec_st(fahrenheitvector, 0, fahrenheitarray);

  /* now print out the results.  */
  
  fprintf(stderr, "Using short (fixed point) vectors...\n");
  
  for (i = 0; i < SHORT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%d celsius is %d fahrenheit\n",
	      celsiusarray[i], fahrenheitarray[i]);
    }


}
