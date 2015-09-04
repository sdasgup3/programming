/* ************************************************************************* 
* NAME: altivec_splat.c
*
* DESCRIPTION:
*
* this program gives examples of the vec_splat family of functions.
* vec_splat generates vectors whose elements all have the same value.
* it's frequently faster than loading a constant vector from memory.
*
* PROCESS:
*
* to compile and run debugging:
* cc -g -o altivec_splat altivec_splat.c -maltivec -mabi=altivec
* ./altivec_splat
*
*  to compile and run optimized:
* cc -O3 -o altivec_splat altivec_splat.c -maltivec -mabi=altivec
* ./altivec_splat
*
* you can use objdump to see the assembly language that is
*   generated at full opt:
*
* cc -ggdb -O3 -c altivec_splat.c -maltivec -mabi=altivec
* objdump -dS altivec_splat.o > o3altivec_splat.dump
*
* or at debug:
*
* cc -ggdb -c altivec_splat.c -maltivec -mabi=altivec
* objdump -dS altivec_splat.o > altivec_splat.dump
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
*  05-Feb-2011        initial coding                         kaj
*  16-Apr-11     added to description in test_splat          gpk
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
   NAME:  test_splat
 ************************************************************************* */
void test_splat()
{
  fprintf(stderr, "Error: can't generate splat instructions without");
  fprintf(stderr, " an altivec-aware compiler. Did you compile with the ");
  fprintf(stderr, "correct flags?\n");
}

#else

/* ************************************************************************* 


   NAME:  test_splat


   USAGE: 


   test_splat();

   returns: void

   DESCRIPTION:
                 vec_splat() and its variants generate vectors whose elements
		 are all a single value.

		 vec_splat takes a vector and an index. the element of that
		 vector specified by index is used as the value for the newly
		 created vector.

		 the vec_splat_<type> variants take a single value and use
		 that as the value for each element of the returned vector.
		 the type of the returned vector is dictated by the _<type>
		 part of the function name.
		 
		 
		 sometimes it's faster to generate a vector with one of
		 the vec_splat* functions  than to copy a constant vector
		 from memory.
		 

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   fprintf
   vec_splat -  create vector composed entirely of one component of another vector
   vec_splat_s8 - create signed char vector with all elements the given value
   vec_splat_s16 - create signed short vector with all elements the given value
   vec_splat_s32 - create signed int vector with all elements the given value
   vec_splat_u8 - create unsigned char vector with all elements the given value
   vec_splat_u16 - create unsigned short vector with all elements the given value
   vec_splat_u32 - create unsigned int vector with all elements the given value
   vec_st - store vector to memory


   REVISION HISTORY:
        STR                  Description of Revision                 Author
     05-Feb-2011              initial coding                           kaj
     16-Apr-11            added to description                         gpk
     
 ************************************************************************* */
void test_splat(void)
{
  vector signed char  sCharVec;
  vector signed short sShortVec;
  vector signed int   sIntVec;
  vector unsigned char  uCharVec;
  vector unsigned short uShortVec;
  vector unsigned int   uIntVec;  
  vector signed char  sCharSplatVec = {-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7};
  signed char outArray_sChar[CHAR_ARRAYSIZE];
  signed short outArray_sShort[SHORT_ARRAYSIZE];
  signed int outArray_sInt[INT_ARRAYSIZE];
  unsigned char outArray_uChar[CHAR_ARRAYSIZE];
  unsigned short outArray_uShort[SHORT_ARRAYSIZE];
  unsigned int outArray_uInt[INT_ARRAYSIZE];
  int i;

  fprintf(stderr, "--- function %s ------\n", __FUNCTION__);

  /* vec_splat */  
  /* print test vector */
  vec_st(sCharSplatVec,0,outArray_sChar);
  fprintf(stderr,"\ntest vector for vec_splat:\n");
  for (i=0; i<CHAR_ARRAYSIZE; i++)
  {
    fprintf(stderr,"%d ",(int) outArray_sChar[i]); 
  }

  /* randomly test a couple of positions */
  sCharVec = vec_splat(sCharSplatVec,11);
  vec_st(sCharVec,0,outArray_sChar);
  fprintf(stderr,"\nvec_splat(testvector,11)\n");
  for (i=0; i<CHAR_ARRAYSIZE; i++)
  {
    fprintf(stderr,"%d ",(int) outArray_sChar[i]); 
  }
  sCharVec = vec_splat(sCharSplatVec,2);
  vec_st(sCharVec,0,outArray_sChar);
  fprintf(stderr,"\nvec_splat(testvector,2)\n");
  for (i=0; i<CHAR_ARRAYSIZE; i++)
  {
    fprintf(stderr,"%d ",(int) outArray_sChar[i]); 
  }

  /* signed splats */
  /* Note: for the signed splats, valid values are a 5-bit literal
           from -16 to +15. Values beyond that range will generate
           an error when compiling 
  */

  sCharVec = vec_splat_s8(2);
  vec_st(sCharVec,0,outArray_sChar);
  fprintf(stderr,"\n\nvec_splat_s8(2):\n");
  for (i=0; i<CHAR_ARRAYSIZE; i++)
  {
    fprintf(stderr,"%d ",(int) outArray_sChar[i]); 
  }
  sCharVec = vec_splat_s8(-2);
  vec_st(sCharVec,0,outArray_sChar);
  fprintf(stderr,"\nvec_splat_s8(-2):\n");
  for (i=0; i<CHAR_ARRAYSIZE; i++)
  {
    fprintf(stderr,"%d ",(int) outArray_sChar[i]); 
  }
 
  sShortVec = vec_splat_s16(2);
  vec_st(sShortVec,0,outArray_sShort);
  fprintf(stderr,"\n\nvec_splat_s16(2):\n");
  for (i=0; i<SHORT_ARRAYSIZE; i++)
  {
    fprintf(stderr,"%hd ",outArray_sShort[i]); 
  }
  sShortVec = vec_splat_s16(-2);
  vec_st(sShortVec,0,outArray_sShort);
  fprintf(stderr,"\nvec_splat_s16(-2):\n");
  for (i=0; i<SHORT_ARRAYSIZE; i++)
  {
    fprintf(stderr,"%hd ",outArray_sShort[i]); 
  }

  sIntVec = vec_splat_s32(2);
  vec_st(sIntVec,0,outArray_sInt);
  fprintf(stderr,"\n\nvec_splat_s32(2):\n");
  for (i=0; i<INT_ARRAYSIZE; i++)
  {
    fprintf(stderr,"%d ",outArray_sInt[i]); 
  }
  sIntVec = vec_splat_s32(-2);
  vec_st(sIntVec,0,outArray_sInt);
  fprintf(stderr,"\nvec_splat_s32(-2):\n");
  for (i=0; i<INT_ARRAYSIZE; i++)
  {
    fprintf(stderr,"%d ",outArray_sInt[i]); 
  }

  /* unsigned splats */
  /* Note: for the unsigned splats, valid values are a 5-bit literal
           from -16 to +15. Values beyond that range will generate
           an error when compiling 
  */
  uCharVec = vec_splat_u8(15);
  vec_st(uCharVec,0,outArray_uChar);
  fprintf(stderr,"\n\n\nvec_splat_u8(15):\n");
  for (i=0; i<CHAR_ARRAYSIZE; i++)
  {
    fprintf(stderr,"%d ",(int) outArray_uChar[i]); 
  }
  uCharVec = vec_splat_u8(-15);
  vec_st(uCharVec,0,outArray_uChar);
  fprintf(stderr,"\nvec_splat_u8(-15):\n");
  for (i=0; i<CHAR_ARRAYSIZE; i++)
  {
    fprintf(stderr,"%d ",(int) outArray_uChar[i]); 
  }

  uShortVec = vec_splat_u16(15);
  vec_st(uShortVec,0,outArray_uShort);
  fprintf(stderr,"\n\nvec_splat_u16(15):\n");
  for (i=0; i<SHORT_ARRAYSIZE; i++)
  {
    fprintf(stderr,"%hu ",outArray_uShort[i]); 
  }
  uShortVec = vec_splat_u16(-15);
  vec_st(uShortVec,0,outArray_uShort);
  fprintf(stderr,"\nvec_splat_u16(-15):\n");
  for (i=0; i<SHORT_ARRAYSIZE; i++)
  {
    fprintf(stderr,"%hu ",outArray_uShort[i]); 
  }

  uIntVec = vec_splat_u32(15);
  vec_st(uIntVec,0,outArray_uInt);
  fprintf(stderr,"\n\nvec_splat_u32(15):\n");
  for (i=0; i<INT_ARRAYSIZE; i++)
  {
    fprintf(stderr,"%u ",outArray_uInt[i]); 
  }
  uIntVec = vec_splat_u32(-15);
  vec_st(uIntVec,0,outArray_uInt);
  fprintf(stderr,"\nvec_splat_u32(-15):\n");
  for (i=0; i<INT_ARRAYSIZE; i++)
  {
    fprintf(stderr,"%u ",outArray_uInt[i]); 
  }

  fprintf(stderr,"\n\n");


} /* test_splat */


#endif /* __VEC__ */


/* ************************************************************************* 
 Name: main
 ************************************************************************* */
int main (int argc, char * argv[])
{
  /* test_altivec_feature will exit if the processor */
  /* can not do altivec */
  test_altivec_feature();

  /* test the splat feature */
  test_splat();

  return(0);
}

