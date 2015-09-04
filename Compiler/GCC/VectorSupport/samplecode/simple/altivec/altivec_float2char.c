/* ************************************************************************* 
* NAME: altivec_float2char.c
*
* DESCRIPTION:
*
* this is a test program to help me figure out altivec instructions.
* this file covers converting floats to ints to chars
*
* PROCESS:
*
* to compile and run debugging:
* cc -g -o altivec_float2char altivec_float2char.c -maltivec -mabi=altivec
* ./altivec_float2char
*
*  to compile and run optimized:
* cc -O3 -o altivec_float2char altivec_float2char.c -maltivec -mabi=altivec
* ./altivec_float2char
*
* you can use objdump to see the assembly language that is
*   generated at full opt:
*
* cc -ggdb -O3 -c altivec_float2char.c -maltivec -mabi=altivec
* objdump -dS altivec_float2char.o > o3altivec_float2char.dump
*
* or at debug:
*
* cc -ggdb -c altivec_float2char.c -maltivec -mabi=altivec
* objdump -dS altivec_float2char.o > altivec_float2char.dump
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
*    07-Feb-2011        initial coding                       kaj 
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
   NAME:  test_float2char
 ************************************************************************* */
void test_float2char(void)
{
  fprintf(stderr, "Error: can't generate float conversion instructions without");
  fprintf(stderr, " an altivec-aware compiler. Did you compile with the ");
  fprintf(stderr, "correct flags?\n");

}

#else

/* ************************************************************************* 
 *     NAME:  printFloats
 *
 * ************************************************************************* */
void printFloats( float *outFloats)
{
  int i;

  fprintf(stderr,"float:{ ");
  for (i = 0; i < FLOAT_ARRAYSIZE; i++)
  {
     fprintf(stderr,"%f ",outFloats[i]);
  }
  fprintf(stderr,"}\n\n");
} /* printFloats */


/* ************************************************************************* 
 *    NAME:  printShorts
 *
 * ************************************************************************* */
void printShorts(signed short *outShorts)
{
  int i;

  fprintf(stderr,"short:{ ");
  for (i = 0; i < SHORT_ARRAYSIZE; i++)
  {
     fprintf(stderr,"%d ",outShorts[i]);
  }
  fprintf(stderr,"}\n\n");
} /* printShorts */


/* ************************************************************************* 
 *    NAME:  printUShorts
 *
 * ************************************************************************* */
void printUShorts(unsigned short *outShorts)
{
  int i;

  fprintf(stderr,"short:{ ");
  for (i = 0; i < SHORT_ARRAYSIZE; i++)
  {
     fprintf(stderr,"%d ",outShorts[i]);
  }
  fprintf(stderr,"}\n\n");
} /* printUShorts */



/* ************************************************************************* 
 *    NAME:  printInts
 *
 * ************************************************************************* */
void printInts(signed int *outInts)
{
  int i;

  fprintf(stderr,"Int:{ ");
  for (i = 0; i < INT_ARRAYSIZE; i++)
  {
     fprintf(stderr,"%d ",outInts[i]);
  }
  fprintf(stderr,"}\n\n");
} /* printInts */


/* ************************************************************************* 
 *    NAME:  printUInts
 *
 * ************************************************************************* */
void printUInts(unsigned int *outInts)
{
  int i;

  fprintf(stderr,"Int:{ ");
  for (i = 0; i < INT_ARRAYSIZE; i++)
  {
     fprintf(stderr,"%d ",outInts[i]);
  }
  fprintf(stderr,"}\n\n");
} /* printUInts */


/* ************************************************************************* 
 *    NAME:  printChars
 *
 * ************************************************************************* */
void printChars(unsigned char *outChars)
{
  int i;

  fprintf(stderr,"Char:{ ");
  for (i = 0; i < CHAR_ARRAYSIZE; i++)
  {
     fprintf(stderr,"%d ",outChars[i]);
  }
  fprintf(stderr,"}\n\n");
} /* printChars */


/* ************************************************************************* 

   NAME:  test_float2char

   USAGE: 

   test_float2char();

   returns: void

   DESCRIPTION:
                 convert floats to ints to shorts to chars to test
                 rounding/truncating

   REFERENCES:

   Ian Ollmann's Altivec Tutorial
   
   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   fprintf
   vec_st - store a vector to memory
   vec_cts - convert to fixed point
   vec_pack - narrow 2 vectors and pack them into one vector
   vec_packsu - saturating pack, unsigned result

   
   REVISION HISTORY:
        STR        Description of Revision                   Author
     07-Feb-2011    Based off of gpk's condense_float_rgbas   kaj

 ************************************************************************* */
void test_float2char(void)
{
  vector float floatVec1 = {0.0, -50.14455,  21.7790, 100.0 };
  vector float floatVec2 = {-100.4567, 250.14455,  -210.7790, 170.0 };
  vector float floatVec3 = {0.09876, -150.55,  1.7790, -120.0 };
  vector float floatVec4 = {-90.234, -30.9455,  -205.90, 199.9 };
  vector signed int intVec1, intVec2, intVec3, intVec4;
  vector signed short shortVec1, shortVec2;
  vector unsigned char charVec;
  unsigned char printchar[CHAR_ARRAYSIZE] __attribute__ ((aligned (16)));
  float printfloat[FLOAT_ARRAYSIZE] __attribute__ ((aligned (16)));
  signed short printshort[SHORT_ARRAYSIZE] __attribute__ ((aligned (16)));
  signed int printint[INT_ARRAYSIZE] __attribute__ ((aligned (16)));
  int i;

  fprintf(stderr, "--- function %s ------\n", __FUNCTION__);

  /* print out floats */
  vec_st(floatVec1,0,printfloat);
  printFloats(printfloat);

  vec_st(floatVec2,0,printfloat);
  printFloats(printfloat);

  vec_st(floatVec3,0,printfloat);
  printFloats(printfloat);

  vec_st(floatVec4,0,printfloat);
  printFloats(printfloat);

  //floatVec1 = vec_trunc(floatVec1);
  vec_trunc(floatVec1);

  /* convert from float to signed int and print */
  intVec1 = vec_cts(floatVec1,0);
  vec_st(intVec1,0,printint);
  printInts(printint);

  intVec2 = vec_cts(floatVec2,0);
  vec_st(intVec2,0,printint);
  printInts(printint);

  intVec3 = vec_cts(floatVec3,0);
  vec_st(intVec3,0,printint);
  printInts(printint);

  intVec4 = vec_cts(floatVec4,0);
  vec_st(intVec4,0,printint);
  printInts(printint);

  /* convert from signed int to signed short and print */
  shortVec1 = vec_pack(intVec1,intVec2);
  vec_st(shortVec1,0,printshort);
  printShorts(printshort);

  shortVec2 = vec_pack(intVec3,intVec4);
  vec_st(shortVec2,0,printshort);
  printShorts(printshort);

  /* convert from signed short to unsigned char and print */
  charVec  = vec_packsu(shortVec1, shortVec2);
  vec_st(charVec,0,printchar);
  printChars(printchar);

} /* test_float2char */


/* ************************************************************************* 

   NAME:  test2_float2char

   USAGE: 

   test2_float2char();

   returns: void

   DESCRIPTION:
                 convert floats to ints to shorts to chars to test
                 rounding/truncating

   REFERENCES:

   Ian Ollmann's Altivec Tutorial
   
   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   fprintf
   vec_st - store a vector to memory
   vec_ctu - convert to fixed point
   vec_pack - narrow 2 vectors and pack them into one vector
   
   REVISION HISTORY:
        STR        Description of Revision                   Author
     08-Feb-2011   Based off of test_float2char but          kaj
                    changing set of vec functions called  

 ************************************************************************* */
void test2_float2char(void)
{
  vector float floatVec1 = {0.0, -50.14455,  21.7790, 100.0 };
  vector float floatVec2 = {-100.4567, 250.14455,  -210.7790, 170.0 };
  vector float floatVec3 = {0.09876, -150.55,  1.7790, -120.0 };
  vector float floatVec4 = {-90.234, -30.9455,  -205.90, 199.9 };
  vector unsigned int intVec1, intVec2, intVec3, intVec4;
  vector unsigned short shortVec1, shortVec2;
  vector unsigned char charVec;
  unsigned char printchar[CHAR_ARRAYSIZE] __attribute__ ((aligned (16)));
  float printfloat[FLOAT_ARRAYSIZE] __attribute__ ((aligned (16)));
  unsigned short printshort[SHORT_ARRAYSIZE] __attribute__ ((aligned (16)));
  unsigned int printint[INT_ARRAYSIZE] __attribute__ ((aligned (16)));
  int i;

  fprintf(stderr, "--- function %s ------\n", __FUNCTION__);

  /* print out floats */
  vec_st(floatVec1,0,printfloat);
  printFloats(printfloat);

  vec_st(floatVec2,0,printfloat);
  printFloats(printfloat);

  vec_st(floatVec3,0,printfloat);
  printFloats(printfloat);

  vec_st(floatVec4,0,printfloat);
  printFloats(printfloat);

  /* convert from float to unsigned int and print */
  intVec1 = vec_ctu(floatVec1,0);
  vec_st(intVec1,0,printint);
  printUInts(printint);

  intVec2 = vec_ctu(floatVec2,0);
  vec_st(intVec2,0,printint);
  printUInts(printint);

  intVec3 = vec_ctu(floatVec3,0);
  vec_st(intVec3,0,printint);
  printUInts(printint);

  intVec4 = vec_ctu(floatVec4,0);
  vec_st(intVec4,0,printint);
  printInts(printint);

  /* convert from unsigned int to unsigned short and print */
  shortVec1 = vec_pack(intVec1,intVec2);
  vec_st(shortVec1,0,printshort);
  printUShorts(printshort);

  shortVec2 = vec_pack(intVec3,intVec4);
  vec_st(shortVec2,0,printshort);
  printUShorts(printshort);

  /* convert from unsigned short to unsigned char and print */
  charVec  = vec_pack(shortVec1, shortVec2);
  vec_st(charVec,0,printchar);
  printChars(printchar);

} /* test2_float2char */

#endif /* __VEC__ */



/* ************************************************************************* 

   NAME:  test_float2char_scalar

   USAGE: 

   test_float2char_scalar();

   returns: void

   DESCRIPTION:
                 convert floats to ints to shorts to chars to test
                 rounding/truncating

   REFERENCES:

   Ian Ollmann's Altivec Tutorial
   
   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   fprintf

   
   REVISION HISTORY:
        STR        Description of Revision                   Author
     08-Feb-2011    Based off of gpk's yuv422rgb_scalar       kaj

 ************************************************************************* */
void test_float2char_scalar(void)
{
  float floatVec1[4] = {0.0, -50.14455,  21.7790, 100.0 };
  //float floatVec2[4] = {-100.4567, 250.14455,  -210.7790, 170.0 };
  float floatVec2[4] = {136.0,110.0,119.0,0.0};
  float floatVec3[4] = {0.09876, -150.55,  1.7790, -120.0 };
  float floatVec4[4] = {-90.234, -30.9455,  -205.90, 199.9 };
  signed int intVec1, intVec2, intVec3, intVec4;
  signed short shortVec1, shortVec2;
  unsigned char charVec;
  unsigned char printchar[CHAR_ARRAYSIZE] __attribute__ ((aligned (16)));
  float printfloat[FLOAT_ARRAYSIZE] __attribute__ ((aligned (16)));
  signed short printshort[SHORT_ARRAYSIZE] __attribute__ ((aligned (16)));
  signed int printint[INT_ARRAYSIZE] __attribute__ ((aligned (16)));
  int i;
  unsigned char red0, green0, blue0, alpha;
  unsigned char red1, green1, blue1;
  int y0, y1, u, v;
  const unsigned char *source_endp;
  const unsigned char *vector_endp;
  int remainder;
  float red_partial, green_partial, blue_partial;

  fprintf(stderr, "--- function %s ------\n", __FUNCTION__);

  /* print out floats */
  printFloats(floatVec2);

  i = 0;
  y0 = floatVec2[i++]; /* Y  */
  u =  floatVec2[i++] - 128; /* U  */
  y1 = floatVec2[i++]; /* Y  */
  v =  floatVec2[i++] - 128; /* V  */

  red_partial =  1.4075 * (float)v;
  green_partial = -0.34455 * (float)u - 0.7169 * (float)v;
  blue_partial = 1.7790 * (float)u;

{
  int redsum, greensum, bluesum;

  redsum =   (int)(y0 + red_partial);
  greensum = (int)(y0 + green_partial);
  bluesum =  (int)(y0 + blue_partial);

  red0 =   (unsigned char)redsum;
  green0 = (unsigned char)greensum;
  blue0  = (unsigned char)bluesum;

  fprintf(stderr,"Redsum0: (%d %d %d) RGB0: (%d %d %d) \n",
   redsum,greensum,bluesum,red0,green0,blue0);
}

{
  int redsum, greensum, bluesum;

  redsum = (int)(y1 + red_partial);
  greensum = (int)(y1 + green_partial);
  bluesum = (int)(y1 + blue_partial);

  red1 =   (unsigned char)redsum;
  green1 = (unsigned char)greensum;
  blue1  = (unsigned char)bluesum;

  fprintf(stderr,"Redsum1: (%d %d %d) RGB1: (%d %d %d) \n",
   redsum,greensum,bluesum,red1,green1,blue1);
}


} /* test_float2char_scalar */



/* ************************************************************************* 
 Name: main
 ************************************************************************* */
int main (int argc, char * argv[])
{
  /* test_altivec_feature will exit if the processor */
  /* can not do altivec */
  test_altivec_feature();

  /* test the conversion features */
  test_float2char();

  /* test the conversion features */
  test2_float2char();

  /* test the conversion features */
  test_float2char_scalar();

  return(0);
}

