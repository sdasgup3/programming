/* ************************************************************************* 
* NAME: samples/x86/47617.c
*
* DESCRIPTION:
*
* sometimes seeing what doesn't work is as useful as seeing what does.
* there is a bug in this version of GCC (4.3.2 20081105) where the compiler
* opimizes by only evaluating a repeated expression once. in this example,
* the expression is _mm_cvtps_pi16(source). the code evaluates this after
* changing the rounding mode. however, the optimizer doesn't tie the
* change in rounding mode to the need to re-evaluate the expression--
* so the optimization goes awry. this file includes code that demonstrates
* the bug and two work-arounds that keep the optimization from happening.
*
* this demonstrates intel intrinsics for controlling rounding mode.
* you can tell the processor to round in couple different directions:
*
* _MM_ROUND_NEAREST -- nearest integer up or down
* _MM_ROUND_DOWN -- towards negative infinity; floor()
* _MM_ROUND_UP -- towards positive infinity; ceil()
* _MM_ROUND_TOWARD_ZERO -- truncate
*
*
* use _MM_GET_ROUNDING_MODE() to see what the current mode is
* use _MM_SET_ROUNDING_MODE(newmode) to change it
*
* PROCESS:
*
* cc -g -o round47617 47617.c  -msse -mmmx
*
* ./round47617
*
*  cc -O3 -o round47617 47617.c  -msse -mmmx #  -mfpmath=sse -frounding-math
*
* ./round47617
*
* 
* GLOBALS: none
*
* REFERENCES:
*
* * Intel C++ Intrinsics Reference
* * GCC Bugzilla bug 47617
*
* LIMITATIONS:
* 
* gives expected answers at -g, but not at -O3 because of an compiler bug.
*
* REVISION HISTORY:
*
*   STR                Description                          Author
*
*    5-Feb-11          initial coding                        gpk
*
* TARGET: GNU C version 4 or higher
*
* the contents of this file is in the public domain. if it breaks,
* you get to keep both pieces.
*
* ************************************************************************* */


#include <stdio.h>
#include <stdlib.h> /* exit  */
#include <string.h> /* memcpy */
#include <stdint.h> /* int8_t, int16_t, int32_t  */
#include <cpuid.h> /* __get_cpuid_max, __get_cpuid */
#include <mmintrin.h> /* MMX instrinsics  __m64 integer type  */
#include <xmmintrin.h> /* sse intrinsics */

/* SHORT_ARRAYSIZE - the number of elements we'd need in an  */
/* array of int16_t's to hold an __m64: should be 4.         */

#define SHORT_ARRAYSIZE (sizeof(__m64) / sizeof(int16_t))

/*  FLOAT_ARRAYSIZE - the number of elements we'd need in an array */
/* of floats to hold an __m128; should be 4                        */

#define FLOAT_ARRAYSIZE (sizeof(__m128)/ sizeof(float))

/* prototypes  */
void test_sse1_feature(void);
void test_rounding_buggy(void);
void test_rounding_volatile(void);
void test_rounding_asm(void); 
void print_rounding_mode(char * label, unsigned int mode);
void print_round_results(char * label,  __m128 source, __m64 dest);
/* end prototypes  */



int main(int argc, char * argv[])
{


  /* exit if the processor can't do SSE  */
  
  test_sse1_feature();

  /* show how rounding modes work  */
  test_rounding_buggy();
  test_rounding_volatile();
  test_rounding_asm(); 

  return(0);
}



/* FUNC_FEATURES - return the list of intel-defined features in the  */
/* __get_cpuid function.   */

#define FUNC_FEATURES 0x1


/* ************************************************************************* 


   NAME:  test_sse1_feature


   USAGE:

   test_sse1_feature();

   returns: void

   DESCRIPTION:
                 use the __get_cpuid function found on X86 to see if
		 this processor has the SSE feature. if it doesn't,
		 complain and exit.

		 (The feature is called "SSE", I'm referring to it as
		 SSE1 to avoid confusion with SSE2, SSE3, etc)
		 
   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   __get_cpuid
   fprintf
   exit
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     17-Jan-10               initial coding                           gpk

 ************************************************************************* */


void test_sse1_feature(void)
{
  unsigned int eax, ebx, ecx, edx;
  unsigned int extensions, sig;
  int result, sse1_available;
  
  /* call __get_cpuid: there will be bits set in ecx, edx for  */
  /* the intel-defined SSE1, SSEn features.                     */
  
  result = __get_cpuid (FUNC_FEATURES, &eax, &ebx, &ecx, &edx);

  if (-1 == result)
    {
      fprintf(stderr, "Fatal Error: can't get CPU features\n");
      exit(-1);
    }
  else
    {
      sse1_available =  (bit_SSE & edx);

      if (0 == sse1_available)
	{
	  fprintf(stderr, "Error: SSE1 features not available\n");
	  fprintf(stderr, "Had this been an actual program, we'd fall ");
	  fprintf(stderr, "back to a non-SSE1 implementation\n");
	  exit(-1);
	}
      else
	{
	  fprintf(stderr, "SSE1 features ARE available\n");
	}
    }
}
    


/* ************************************************************************* 


   NAME:  test_rounding_buggy


   USAGE: 

   test_rounding();

   returns: void

   DESCRIPTION:
                 show how rounding modes work by starting with a
		 vector of floats an using _mm_cvtps_pi16 to round
		 them into a vector of shorts. since _mm_cvtps_pi16
		 is affected by the current rounding mode, set that
		 to a different value each time and show the results.
		 finally restore the initial rounding mode.
		 

   REFERENCES:

   LIMITATIONS:

                 this doesn't work at -O3 (see the file header).
		 
   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   _MM_GET_ROUNDING_MODE - get current rounding mode
   _MM_SET_ROUNDING_MODE - set rounding mode
   _mm_cvtps_pi16 - turn vector of floats to vector of shorts 
   print_round_results
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

      5-Feb-11               initial coding                           gpk

 ************************************************************************* */

void test_rounding_buggy(void)
{
 __m128 source = {-1.1, 0.0, 1.1, 1.5};
 __m64 dest;
 unsigned int initial_mode;

 fprintf(stderr, " %s\n", __FUNCTION__);

 initial_mode = _MM_GET_ROUNDING_MODE();
 print_rounding_mode("initial rounding mode", initial_mode);
 
 /* now set the rounding mode to each value to see the result  */
 
 _MM_SET_ROUNDING_MODE(_MM_ROUND_NEAREST);

 dest = _mm_cvtps_pi16(source);
 _mm_empty();
 print_round_results("with _MM_ROUND_NEAREST ", source, dest);

 _MM_SET_ROUNDING_MODE(_MM_ROUND_DOWN);

 dest = _mm_cvtps_pi16(source);
 _mm_empty();
 print_round_results("with _MM_ROUND_DOWN ", source, dest);
 
 _MM_SET_ROUNDING_MODE(_MM_ROUND_UP);

 dest = _mm_cvtps_pi16(source);
 _mm_empty();
 print_round_results("with _MM_ROUND_UP ", source, dest);

 _MM_SET_ROUNDING_MODE(_MM_ROUND_TOWARD_ZERO);

 dest = _mm_cvtps_pi16(source);
 _mm_empty();
 print_round_results("with _MM_ROUND_TOWARD_ZERO ", source, dest);

 /* restore initial rounding mode  */
  _MM_SET_ROUNDING_MODE(initial_mode);
  
}




/* ************************************************************************* 


   NAME:  test_rounding_volatile


   USAGE: 

   test_rounding_volatile();

   returns: void

   DESCRIPTION:
                 show how rounding modes work by starting with a
		 vector of floats an using _mm_cvtps_pi16 to round
		 them into a vector of shorts. since _mm_cvtps_pi16
		 is affected by the current rounding mode, set that
		 to a different value each time and show the results.
		 finally restore the initial rounding mode.

                 this is one work around for the problem in
		 test_rounding_buggy. the optimization that causes the
		 problem there is turned off by declaring the variable
		 "source" to be volatile.

		 this is not the best way to work around this problem.
		 a better solution is to structure your code so that
		 this doesn't come up. (see round.c)
		 

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   _MM_GET_ROUNDING_MODE - get current rounding mode
   _MM_SET_ROUNDING_MODE - set rounding mode
   _mm_cvtps_pi16 - turn vector of floats to vector of shorts 
   print_round_results
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     10-Feb-11               initial coding                           gpk

 ************************************************************************* */

void test_rounding_volatile(void)
{
 volatile __m128 source = {-1.1, 0.0, 1.1, 1.5};
 __m64 dest;
 unsigned int initial_mode;
 
 fprintf(stderr, " %s\n", __FUNCTION__);

 initial_mode = _MM_GET_ROUNDING_MODE();
 print_rounding_mode("initial rounding mode", initial_mode);
 
 /* now set the rounding mode to each value to see the result  */
 
 _MM_SET_ROUNDING_MODE(_MM_ROUND_NEAREST);

 dest = _mm_cvtps_pi16(source);
 _mm_empty();
 print_round_results("with _MM_ROUND_NEAREST ", source, dest);

 _MM_SET_ROUNDING_MODE(_MM_ROUND_DOWN);

 dest = _mm_cvtps_pi16(source);
 _mm_empty();
 print_round_results("with _MM_ROUND_DOWN ", source, dest);
 
 _MM_SET_ROUNDING_MODE(_MM_ROUND_UP);

 dest = _mm_cvtps_pi16(source);
 _mm_empty();
 print_round_results("with _MM_ROUND_UP ", source, dest);

 _MM_SET_ROUNDING_MODE(_MM_ROUND_TOWARD_ZERO);

 dest = _mm_cvtps_pi16(source);
 _mm_empty();
 print_round_results("with _MM_ROUND_TOWARD_ZERO ", source, dest);

 /* restore initial rounding mode  */
  _MM_SET_ROUNDING_MODE(initial_mode);
  
}

#if 1


void test_rounding_asm(void)
{
}


#else

#define reload(avar) asm volatile ("" : "=m"(avar) );

void test_rounding_asm(void)
{
 __m128 source = {-1.1, 0.0, 1.1, 1.5};
 __m64 dest;
 unsigned int initial_mode;
 
 fprintf(stderr, " %s\n", __FUNCTION__);

 initial_mode = _MM_GET_ROUNDING_MODE();
 print_rounding_mode("initial rounding mode", initial_mode);
 
 /* now set the rounding mode to each value to see the result  */
 /* asm volatile ("" : "=m"(source)); */
 /* asm("" : "+X"(source)); */  // force source to be different but the same

 reload(source);
 
 _MM_SET_ROUNDING_MODE(_MM_ROUND_NEAREST);

 dest = _mm_cvtps_pi16(source);
 _mm_empty();
 print_round_results("with _MM_ROUND_NEAREST ", source, dest);

 
 asm("":"+X"(source)); // force source to be different but the same
 _MM_SET_ROUNDING_MODE(_MM_ROUND_DOWN);

 dest = _mm_cvtps_pi16(source);
 _mm_empty();
 print_round_results("with _MM_ROUND_DOWN ", source, dest);

 asm("":"+X"(source)); // force source to be different but the same
 _MM_SET_ROUNDING_MODE(_MM_ROUND_UP);

 dest = _mm_cvtps_pi16(source);
 _mm_empty();
 print_round_results("with _MM_ROUND_UP ", source, dest);
 
 asm("":"+X"(source)); // force source to be different but the same
 _MM_SET_ROUNDING_MODE(_MM_ROUND_TOWARD_ZERO);

 dest = _mm_cvtps_pi16(source);
 _mm_empty();
 print_round_results("with _MM_ROUND_TOWARD_ZERO ", source, dest);

 /* restore initial rounding mode  */
  _MM_SET_ROUNDING_MODE(initial_mode);
  
}
#endif /* 0  */


/* ************************************************************************* 


   NAME:  print_rounding_mode


   USAGE: 

   char * label = "something useful";
   unsigned int mode; -- one of the rounding modes

   print_rounding_mode(label, mode);

   returns: void

   DESCRIPTION:
                 print label followed by the symbolic name for the rounding
		 mode in mode. if mode is not a known mode, say so.

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   fprintf
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

      5-Feb-11               initial coding                           gpk

 ************************************************************************* */

void print_rounding_mode(char * label, unsigned int mode)
{
  char * modestring;

  switch(mode) {
  case _MM_ROUND_NEAREST:
    modestring = "_MM_ROUND_NEAREST ";
    break;

  case _MM_ROUND_DOWN:
    modestring = "_MM_ROUND_DOWN ";
    break;

  case _MM_ROUND_UP:
    modestring = "_MM_ROUND_UP ";
    break;

  case _MM_ROUND_TOWARD_ZERO:
    modestring = "_MM_ROUND_TOWARD_ZERO ";
    break;

  default:
    modestring = "unknown rounding mode";
    break;
  }
  fprintf(stderr, "%s %s (%d)\n", label, modestring, mode );

}





/* ************************************************************************* 


   NAME:  print_round_results


   USAGE: 

    
   char * label= "someething useful";
   __m128 source; -- floating point vector
    __m64 dest; -- source, converted to integer

   print_round_results(label, source, dest);

   returns: void

   DESCRIPTION:
                 print label followed by the contents of source and the
		 contents of dest.

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   _mm_store_ps -- store a vector of floats to an array
   memcpy
   _mm_empty -- empty out multimedia state
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

      5-Feb-11               initial coding                           gpk

 ************************************************************************* */

void print_round_results(char * label,  __m128 source, __m64 dest)
{
  float floatarray[FLOAT_ARRAYSIZE] __attribute__ ((aligned (16)));
  short shortarray[SHORT_ARRAYSIZE] __attribute__ ((aligned (16)));
  int i;

 _mm_store_ps(floatarray, source);
 memcpy(shortarray, &dest, sizeof(shortarray));
 
 /* need the _mm_empty here.  */
 _mm_empty();
 
  fprintf(stderr, "%s\n", label);
  fprintf(stderr, "source ");
  for (i = 0; i < FLOAT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%f ", floatarray[i]);
    }
   fprintf(stderr, " rounds to ");
   for (i = 0; i < SHORT_ARRAYSIZE; i++)
    {
      fprintf(stderr, "%d ", shortarray[i]);
    }
   fprintf(stderr, "\n");
}
