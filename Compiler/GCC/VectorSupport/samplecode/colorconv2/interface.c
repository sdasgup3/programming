/* ************************************************************************* 
* NAME: colorconv/interface.c
*
* DESCRIPTION:
*
* this code take a call to yuv422rgb_fixed or yuv422rgb_float
* and uses the test_cpu_simd_features
* function to figure out which vector or scalar function to call.
*
* PROCESS:
*
* GLOBALS: none
*
* cpu_tested - non-zero if we've tested the CPU for SIMD features
* supported - what features are supported by the CPU
*
* REFERENCES:
*
* LIMITATIONS:
*
* REVISION HISTORY:
*
*   STR                Description                          Author
*
*   27-Dec-09          initial coding                        gpk
*   27-Mar-11 split into fixed point and floating point      gpk
*             functions.
*   02-Apr-11 Added fprintfs to identify which cpu vector    kaj
*              feature is being used
*
* TARGET: Gnu C version 4 or higher
*
*
* ************************************************************************* */

#include <stdio.h>

#include "colorconv.h" /* test_cpu_simd_features yuv422rgb_*  */


/* IDENTIFY_FUNCTION - if this is defined then the general library interfaces */
/* yuv422rgb_fixed_vector and yuv422rgb_float_vector will print out which   */
/* architecture-specific function they're using.                            */

/* #define IDENTIFY_FUNCTION */



/* ************************************************************************* 


   NAME:  yuv422rgb_fixed


   USAGE: 

    
   char red; -- 0 to 255
   char green; -- 0 to 255
   char blue; -- 0 to 255
   int width, height; --  of image in pixels
   int byte_count; -- number of bytes in YUV image (2 * width * height) 
   const unsigned char * sourcep = malloc(byte_count); -- source yuv image

   unsigned char * destp = malloc(4 * width * height); -- dest RGBA image 

   init_imagedata(red, green, blue, yuv_imagep, byte_count);

   yuv422rgb_fixed(sourcep, source_byte_count, destp);

   returns: void 

   DESCRIPTION:
                 convert the YUV422 image in composed of source_byte_count
		 bytes and pointed to by source into an RGBA image in the
		 buffer pointed to by destp.

		 the first time this is called it checks to see what
		 features are supported by the CPU. then it calls the
		 fixed point vector function that best uses that support.

		 if there are no vector processing features (SSEn, neon, etc)
		 available, it falls back to a scalar (non-vector)
		 implementation. 

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   REVISION HISTORY:

        STR                  Description of Revision                 Author

    27-Mar-11                 original coding                         gpk
    
 ************************************************************************* */

void  yuv422rgb_fixed(const unsigned char * sourcep, int source_byte_count,
		    unsigned char * destp)
{
  int have_vector;
  static int warned = 0;
  
  have_vector = yuv422rgb_fixed_vector(sourcep, source_byte_count, destp);
  if (0 ==  have_vector)
    {
      /* fall back to scalar  */
      if (0 == warned)
	{
	  fprintf(stderr,
	       "Warning: no fixed point vector support: calling scalar code\n"
		  );
	  warned = 1;
	}
      yuv422rgb_scalar_fixed(sourcep, source_byte_count, destp);
    }
}


/* ************************************************************************* 


   NAME:  yuv422rgb_fixed_vector


   USAGE: 

    
   char red; -- 0 to 255
   char green; -- 0 to 255
   char blue; -- 0 to 255
   int width, height; --  of image in pixels
   int byte_count; -- number of bytes in YUV image (2 * width * height) 
   const unsigned char * sourcep = malloc(byte_count); -- source yuv image
   int something_done;
   unsigned char * destp = malloc(4 * width * height); -- dest RGBA image 

   init_imagedata(red, green, blue, yuv_imagep, byte_count);
   something_done =  yuv422rgb_fixed_vector(sourcep, source_byte_count, destp);

   if (0 == something_done)
   -- no vector support on this hardware; no values processed
 

   returns: void 

   DESCRIPTION:
                 convert the YUV422 image in composed of source_byte_count
		 bytes and pointed to by source into an RGBA image in the
		 buffer pointed to by destp.

		 the first time this is called it checks to see what
		 features are supported by the CPU. then it calls the
		 fixed point vector function that best uses that support.

		 if there are no vector processing features (SSEn, neon, etc)
		 available, return 0
		 if there is vector support, return 1
	  

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   REVISION HISTORY:

        STR                  Description of Revision                 Author

     28-Dec-09               initial coding                           gpk
     21-Feb-10                added neon                              gpk
     15-Oct-10                added altivec                           gpk
     2-Mar-11                add x86_64 test                          gpk
     2-Apr-11          Added fprintfs to identify which cpu vector    kaj
                        feature is being used
     9-Apr-11  added static vars so we only print that out once and   gpk
               and only if IDENTIFY_FUNCTION is defined
     
 ************************************************************************* */

int yuv422rgb_fixed_vector (const unsigned char * sourcep,
			    int source_byte_count,  unsigned char * destp)
{
  int retval;
  static int cpu_tested = 0; /* have we tested the CPU for features?  */
  static Cpusimdfeatures_t supported; /* features supported by CPU  */
  int have_vector_function;
#ifdef IDENTIFY_FUNCTION
  static int printed = 0;
#endif /* IDENTIFY_FUNCTION  */
  
  have_vector_function = 0;
  
  if (0 == cpu_tested) /* not tested yet  */
    {
      retval = test_cpu_simd_features(&supported);
      cpu_tested = 1;
    }

  if (supported.sse2) /* we can do SSE2  */
    {
#if  !( (defined __i386__) || (defined  __x86_64__))
      fprintf(stderr,
	      "Error:  __i386__ is not defined but .sse2 is supported: ");
      fprintf(stderr,
	      "check logic in test_cpu_simd_features\n");
#else
      have_vector_function = 1;
#ifdef IDENTIFY_FUNCTION
      if (0 == printed)
	{
	  fprintf(stderr,"  --using yuv422rgb_mmx2--\n");
	  printed = 1;
	}
#endif /* IDENTIFY_FUNCTION  */
      yuv422rgb_mmx2(sourcep, source_byte_count, destp);
#endif /*  __i386__  */
    }
  else if (supported.sse1) /* we can do SSE1  */
    {
#if !  ((defined __i386__) || (defined  __x86_64__))
      fprintf(stderr,
	      "Error:  __i386__ is not defined but .sse1 is supported: ");
      fprintf(stderr,
	      "check logic in test_cpu_simd_features\n");
#else
      have_vector_function = 1;
#ifdef IDENTIFY_FUNCTION
      if (0 == printed)
	{      
	  fprintf(stderr,"  --using yuv422rgb_mmx1--\n");
	  printed = 1;
	}
#endif /* IDENTIFY_FUNCTION  */
	  
      yuv422rgb_mmx1(sourcep, source_byte_count, destp);
#endif /*  __i386__  */
    }
  else if (supported.neon) /* we can do neon  */
    {
#ifndef __arm__
     fprintf(stderr,
	      "Error: __arm__ is not defined but neon is supported: ");
     fprintf(stderr,
	     "check logic in test_cpu_simd_features\n");
#else
     have_vector_function = 1;
#ifdef IDENTIFY_FUNCTION
     if (0 == printed)
       {  
	 fprintf(stderr,"  --using yuv422rgb_neon_int--\n");
	  printed = 1;
       }
#endif /* IDENTIFY_FUNCTION  */

     yuv422rgb_neon_int(sourcep, source_byte_count, destp);
#endif /* __arm__  */
    }
  else if (supported.altivec) /* we can do altivec  */
    {
#ifndef __powerpc__
     fprintf(stderr,
	      "Error: __powerpc__ is not defined but altivec is supported: ");
     fprintf(stderr,
	     "check logic in test_cpu_simd_features\n");
#else 
     have_vector_function = 1;
#ifdef IDENTIFY_FUNCTION
     if (0 == printed)
       {  
	 fprintf(stderr,"  --using yuv422rgb_avc_int--\n");
	  printed = 1;
       }
#endif /* IDENTIFY_FUNCTION  */
     yuv422rgb_avc_int(sourcep, source_byte_count, destp);
#endif /* __powerpc__ */

    }
  else
    {
      /* no supported vector functions on this processor  */
      have_vector_function = 0;
    }

  return(have_vector_function);
}



/* ************************************************************************* 


   NAME:  yuv422rgb_float


   USAGE: 

    
   char red; -- 0 to 255
   char green; -- 0 to 255
   char blue; -- 0 to 255
   int width, height; --  of image in pixels
   int byte_count; -- number of bytes in YUV image (2 * width * height) 
   const unsigned char * sourcep = malloc(byte_count); -- source yuv image

   unsigned char * destp = malloc(4 * width * height); -- dest RGBA image 

   init_imagedata(red, green, blue, yuv_imagep, byte_count);

   yuv422rgb_float(sourcep, source_byte_count, destp);

   returns: void 

   DESCRIPTION:
                 convert the YUV422 image in composed of source_byte_count
		 bytes and pointed to by source into an RGBA image in the
		 buffer pointed to by destp.

		 the first time this is called it checks to see what
		 features are supported by the CPU. then it calls the
		 fixed point vector function that best uses that support.

		 if there are no vector processing features (SSEn, neon, etc)
		 available, it falls back to a scalar (non-vector)
		 implementation. 

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   REVISION HISTORY:

        STR                  Description of Revision                 Author

    27-Mar-11                 original coding                         gpk
    
 ************************************************************************* */

void  yuv422rgb_float(const unsigned char * sourcep, int source_byte_count,
		    unsigned char * destp)
{
  int have_vector;
  static int warned = 0;
  
  have_vector = yuv422rgb_float_vector(sourcep, source_byte_count, destp);
  if (0 ==  have_vector)
    {
      /* fall back to scalar  */
      if (0 == warned)
	{
	  fprintf(stderr,
	      "Warning: no floating point vector support: using scalar code\n");
	  warned = 1;
	}
      yuv422rgb_scalar_float(sourcep, source_byte_count, destp);

    }
}


/* ************************************************************************* 


   NAME:  yuv422rgb_float_vector


   USAGE: 

    
   const unsigned char * sourcep;
   int source_byte_count;
   int something_done;
   unsigned char * destp;

   something_done =  yuv422rgb_float_vector(sourcep, source_byte_count, destp);

   if (0 == something_done)
   -- no vector support on this hardware; no values processed
   
   returns: void

   DESCRIPTION:
                convert the YUV422 image in composed of source_byte_count
		 bytes and pointed to by source into an RGBA image in the
		 buffer pointed to by destp.

		 the first time this is called it checks to see what
		 features are supported by the CPU. then it calls the
		 floating point vector function that best uses that support.

		 if there are no vector processing features (SSEn, neon, etc)
		 available, return 0
		 if there is vector support, return 1
                 

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   REVISION HISTORY:

        STR                  Description of Revision                 Author

     27-Mar-11  separated from yuv422rgb_fixed                        gpk
     02-Apr-11  Added fprintfs to identify which cpu vector feature   kaj
                 is being used
    1-May-11  added static vars so we only print that out once and   gpk
               and only if IDENTIFY_FUNCTION is defined
 

 ************************************************************************* */

int yuv422rgb_float_vector(const unsigned char * sourcep,
			    int source_byte_count, unsigned char * destp)
{
  int retval;
  static int cpu_tested = 0; /* have we tested the CPU for features?  */
  static Cpusimdfeatures_t supported; /* features supported by CPU  */
  int have_vector_function;
  
#ifdef IDENTIFY_FUNCTION
  static int printed = 0;
#endif /* IDENTIFY_FUNCTION  */
  
  have_vector_function = 0;
  if (0 == cpu_tested) /* not tested yet  */
    {
      retval = test_cpu_simd_features(&supported);
      cpu_tested = 1;
    }

  if (supported.sse2) /* we can do SSE2  */
    {
#if  !( (defined __i386__) || (defined  __x86_64__))
      fprintf(stderr,
	      "Error:  __i386__ is not defined but .sse2 is supported: ");
      fprintf(stderr,
	      "check logic in test_cpu_simd_features\n");
#else
      have_vector_function = 1;
#ifdef IDENTIFY_FUNCTION
      if (0 == printed)
	{
	  fprintf(stderr,"  --using yuv422rgb_sse2--\n");
	  printed = 1;
	}
#endif /* IDENTIFY_FUNCTION  */

      yuv422rgb_sse2(sourcep, source_byte_count, destp);
#endif /*  __i386__  */
    }
  else if (supported.sse1) /* we can do SSE1  */
    {
#if !  ((defined __i386__) || (defined  __x86_64__))
      fprintf(stderr,
	      "Error:  __i386__ is not defined but .sse1 is supported: ");
      fprintf(stderr,
	      "check logic in test_cpu_simd_features\n");
#else
      have_vector_function = 1;
#ifdef IDENTIFY_FUNCTION
      if (0 == printed)
	{
	  fprintf(stderr,"  --using yuv422rgb_sse1--\n");
	  printed = 1;
	}
#endif /* IDENTIFY_FUNCTION  */
      yuv422rgb_sse1(sourcep, source_byte_count, destp);
#endif /*  __i386__  */
    }
  else if (supported.neon) /* we can do neon  */
    {
#ifndef __arm__
     fprintf(stderr,
	      "Error: __arm__ is not defined but neon is supported: ");
     fprintf(stderr,
	     "check logic in test_cpu_simd_features\n");
#else
      have_vector_function = 1;
#ifdef IDENTIFY_FUNCTION
      if (0 == printed)
	{
	  fprintf(stderr,"  --using yuv422rgb_neon_float--\n");
	  printed = 1;
	}
#endif /* IDENTIFY_FUNCTION  */
      yuv422rgb_neon_float(sourcep, source_byte_count, destp);
#endif /* __arm__  */
    }
  else if (supported.altivec) /* we can do altivec  */
    {
#ifndef __powerpc__
     fprintf(stderr,
	      "Error: __powerpc__ is not defined but altivec is supported: ");
     fprintf(stderr,
	     "check logic in test_cpu_simd_features\n");
#else      
      have_vector_function = 1;
#ifdef IDENTIFY_FUNCTION
      if (0 == printed)
	{
	  fprintf(stderr,"  --using yuv422rgb_avc_float--\n");
	  printed = 1;
	}
#endif /* IDENTIFY_FUNCTION  */
      yuv422rgb_avc_float(sourcep, source_byte_count, destp);
#endif /* __powerpc__ */

    }
  else /* fall back to scalar  */
    {
      /* no supported vector functions on this processor  */
      have_vector_function = 0;
    }

  return(have_vector_function);
}


