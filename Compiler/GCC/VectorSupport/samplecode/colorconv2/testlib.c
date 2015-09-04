/* ************************************************************************* 
* NAME: colorconv/testlib.c
*
* DESCRIPTION:
*
* this is code used to test the color conversion library:
*
* * rgb2yuv422 given a color in RGB, return it in YUYV format.
* * init_imagedata - make a YUYV image of a given color.
* * test_sse1 - initializes test image and runs timing tests on
*   scalar and sse1 functions
* * test_sse2 - initializes test image and runs timing tests on
*   scalar and sse2 functions
*
* * test_scalar - initializes test image and runs timing tests on
*   scalar functions
* 
* * test_neon - initializes test image and runs timing tests on
*   neon vector and scalar functions
* 
* * test_altivec - initializes test image and runs timing tests on
*   altivec vector and scalar functions
* 
* 
* PROCESS:
*
* make
* ./`uname -i`/testcolorconv
*
* GLOBALS: none
*
* REFERENCES:
*
* LIMITATIONS: assumes that the image size is a multiple of 4 bytes
*
* REVISION HISTORY:
*
*   STR                Description                          Author
*
*   22-Dec-09          initial coding                        gpk
*   23-Oct-10        added neon, altivec                     gpk
*   16-Jan-11  modified test_altivec, range_test             gpk
*   1-May-11   code cleanup and commenting                   gpk
*
* TARGET: GCC version 4 or higher
*
*
* ************************************************************************* */

#include <stdio.h>
#include <string.h> /* memcpy memset */
#include <sys/time.h> /* gettimeofday  */
#include <stdlib.h> /* abs */

#ifdef __SSE2__
#include <emmintrin.h> /* SSE2 __m128i: long long,  __m128d double  */
#elif defined( __SSE__)
#include <xmmintrin.h> /* SSE  __m128  float */
#include <mmintrin.h> /* MMX  __m64 int */
#endif

/* USEC_PER_SEC - number of microseconds in a second (as floating point)  */
#define USEC_PER_SEC 1000000.0  

/* TEST_COUNT - number of times to convert an image. make this high  */
/* enough that the test runs long enough to even out things like     */
/* paging. 100 seems a good number on an 800 MHz pentium             */

#define TEST_COUNT 100

#include "colorconv.h" /* test_cpu_simd_features yuv422rgb_*  */



/* ************************************************************************* 


   NAME:  rgb2yuv422


   USAGE: 

    
   int red0;    -- 0 to 255
   int green0;  -- 0 to 255
   int blue0;   -- 0 to 255
   
   int red1;    -- 0 to 255
   int green1;  -- 0 to 255
   int blue1;   -- 0 to 255
   
   int luma0, chroma_u, luma1, chroma_v;

   rgb2yuv422(red0, green0, blue0, red1, green1, blue1, &luma0,
              &chroma_u, &luma1, &chroma_v);

   returns: void

   DESCRIPTION:
                 given the RGB values for two adjacent pixels, return the
		 YUYV values for the macropixel that represents them in
		 YUV422 form.

		 modifies luma0, chroma_u, luma1, chroma_v

   REFERENCES:

   LIMITATIONS: not all RGB's translate to a unique YUV.

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   fprintf
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     13-Dec-09               initial coding                           gpk

 ************************************************************************* */

void rgb2yuv422(int red0, int green0, int blue0,
		int red1, int green1, int blue1,
		int *luma0, int *chroma_u,
		int *luma1, int *chroma_v)
{
  /* Y = R *  .299000 + G *  .587000 + B *  .114000  */
  *luma0 =(int) (red0 * .299000 + green0 * 0.587 + blue0 * 0.114);
  *luma1 = (int)(red1 * .299000 + green1 * 0.587 + blue1 * 0.114);

  /* U = R * -.168736 + G * -.331264 + B *  .500000 + 128  */
  *chroma_u = (int)( -0.168736 * red0 + -0.331264 * green0 + 0.5 * blue0 + 128);

  /* V = R *  .500000 + G * -.418688 + B * -.081312 + 128  */
  *chroma_v = (int)(0.5 * red0 - 0.418688 * green0 - 0.081312 * blue0  + 128);
#if 1
  fprintf(stderr, "[%d %d %d] -> [%d %d %d %d]\n", red0, green0, blue0,
	  *luma0,  *chroma_u, *luma1, *chroma_v);
#endif /* 0  */
}




/* ************************************************************************* 


   NAME:  init_imagedata


   USAGE: 

    
   char red; -- 0 to 255
   char green; -- 0 to 255
   char blue; -- 0 to 255
   int width, height; --  of image in pixels
   int byte_count; -- number of bytes in YUV image (2 * width * height) 
   unsigned char * yuv_imagep = malloc(byte_count);
  

   init_imagedata(red, green, blue, yuv_imagep, byte_count);

   returns: void

   DESCRIPTION:
                 initialize the image pointed to by yuv_imagep
		 and containing byte_count bytes of data to be
		 the color given by red/green/blue and stored in
		 YUYV format.

   REFERENCES:

   LIMITATIONS: assumes that byte_count is a multiple of 4

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   rgb2yuv422
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     14-Dec-09               initial coding                           gpk

 ************************************************************************* */

void init_imagedata( char red,  char green,  char blue,
		     unsigned char * yuv_imagep, int byte_count)
{
  int y0, u, y1, v;
  unsigned char * image_endp;

  /* get the YUYV translation of the given red/green/blue  */
  rgb2yuv422(red,  green,  blue, red,  green,  blue, &y0, &u, &y1, &v); 
  image_endp = yuv_imagep + byte_count;

  /* note:should check to make sure byte_count is multiple of 4  */
  while (yuv_imagep < image_endp)
    {
      *(yuv_imagep++) = (unsigned char)y0;
      *(yuv_imagep++) = (unsigned char)u;
      *(yuv_imagep++) = (unsigned char)y1;
      *(yuv_imagep++) = (unsigned char)v;
    }
}


/* ************************************************************************* 


   NAME:  test_general_int


   USAGE: 

   int some_int;
   void;

   some_int =  test_general();

   returns: int

   DESCRIPTION:
                 test the general yuv422rgb_scalar_fixed function. since
		 yuv422rgb_scalar_fixed figures out which CPU features are
		 available by itself, we don't need that here.

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   init_imagedata
   yuv422rgb_scalar_fixed
   fprintf
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     27-Dec-09               initial coding                           gpk

 ************************************************************************* */

int test_general_int(void)
{
  /* yuv_pixeldata contains macropixels of YUYV for two pixels */
  unsigned char yuv_pixeldata[640 * 480 * 2] __attribute__ ((aligned (16)));
  unsigned char rgba_pixeldata[640 * 480 * 4] __attribute__ ((aligned (16)));
  int source_byte_count = sizeof(yuv_pixeldata);
  
  init_imagedata (127, 56, 43, yuv_pixeldata, source_byte_count);

  yuv422rgb_scalar_fixed(yuv_pixeldata,  source_byte_count, rgba_pixeldata);
   
  fprintf(stderr, "yuyv general_int %d %d %d %d  RGBA %d %d %d %d\n",
	  yuv_pixeldata[0],  yuv_pixeldata[1],  yuv_pixeldata[2],
	  yuv_pixeldata[3],  rgba_pixeldata[0], rgba_pixeldata[1],
	  rgba_pixeldata[2], rgba_pixeldata[3]);

  return(0);
}



/* ************************************************************************* 


   NAME:  test_general_float


   USAGE: 

   int some_int;

   some_int =  test_general_float();

   returns: int

   DESCRIPTION:
               this calls the general floating-point interface to the
	       colorconv2 library: yuv422rgb_float. it passes in an image
	       initialized to YUV (127, 56, 43), and puts the RGBA image into
	       a second buffer. the starting YUV and translated RGBA
	       are printed and zero returned.

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   init_imagedata
   yuv422rgb_float
   fprintf
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     26-Mar-11               initial coding                           gpk
      1-May-11           added to comment block                       gpk
      
 ************************************************************************* */

int test_general_float(void)
{
  /* yuv_pixeldata contains macropixels of YUYV for two pixels */
  unsigned char yuv_pixeldata[640 * 480 * 2] __attribute__ ((aligned (16)));
  unsigned char rgba_pixeldata[640 * 480 * 4] __attribute__ ((aligned (16)));
  int source_byte_count = sizeof(yuv_pixeldata);
  
  init_imagedata (127, 56, 43, yuv_pixeldata, source_byte_count);

  yuv422rgb_float(yuv_pixeldata,  source_byte_count, rgba_pixeldata);
   
  fprintf(stderr, "yuyv general_float %d %d %d %d  RGBA %d %d %d %d\n",
	  yuv_pixeldata[0],  yuv_pixeldata[1],  yuv_pixeldata[2],
	  yuv_pixeldata[3],  rgba_pixeldata[0], rgba_pixeldata[1],
	  rgba_pixeldata[2], rgba_pixeldata[3]);

  return(0);
}

/* ************************************************************************* 


   NAME:  test_scalar


   USAGE: 

   int some_int;

   some_int =  test_scalar();

   returns: int

   DESCRIPTION:
                 this is basically a test stub for the non-vector
		 (scalar) yuv422rgb_*  functions.

		 first use init_imagedata to fill yuv_pixeldata
		 with a constant YUV color given by the RGB
		 (first three arguments) passed in.

		 then call each of the yuv422rgb_* in a loop (TEST_COUNT times)
		 to translate yuv_pixeldata into an RGBA image stored
		 in rgba_pixeldata. (the TEST_COUNT times make sure we can
		 measure the time without worrying about clock jitter
		 and allows for things like the data paging in and out
		 to happen and be averaged into the test time.
                 

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   init_imagedata
   gettimeofday
   perror
   yuv422rgb_scalar_float
   yuv422rgb_scalar_fixed
   fprintf
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     27-Dec-09               initial coding                           gpk

 ************************************************************************* */

int test_scalar(void)
{
  /* yuv_pixeldata contains macropixels of YUYV for two pixels */
  unsigned char yuv_pixeldata[640 * 480 * 2];
  unsigned char rgba_pixeldata[640 * 480 * 4];
  int source_byte_count = sizeof(yuv_pixeldata);
  struct timeval one, two, three;
  int i, timeerror;

  
  init_imagedata (127, 56, 43, yuv_pixeldata, source_byte_count);

 
  /* test yuv422rgb_scalar_float in the interval between one and two.  */
  
  timeerror= gettimeofday(&one, NULL);
  if (-1 == timeerror)
    {
      perror("Error gettimg time one");
      return(-1);
    }
  
  for (i = 0; i < TEST_COUNT; i++)
    {
      yuv422rgb_scalar_float(yuv_pixeldata,  source_byte_count, rgba_pixeldata);
    }
   
  timeerror= gettimeofday(&two, NULL);
  if (-1 == timeerror)
    {
      perror("Error gettimg time two");
      return(-1);
    }
  /* check yuv422rgb_scalar_fixed between two and three  */
  for (i = 0; i < TEST_COUNT; i++)
    {
      yuv422rgb_scalar_fixed(yuv_pixeldata,  source_byte_count, rgba_pixeldata);
    }
   
  timeerror=  gettimeofday(&three, NULL);
  if (-1 == timeerror)
    {
      perror("Error gettimg time three");
      return(-1);
    }
 
  /* now run them each once to get the answers and print them out for  */
  /* comparison....  */
   
  yuv422rgb_scalar_float(yuv_pixeldata,  source_byte_count, rgba_pixeldata);
   
  fprintf(stderr, "(floating point scalar) %d %d %d %d  RGBA %d %d %d %d\n",
	  yuv_pixeldata[0],  yuv_pixeldata[1],  yuv_pixeldata[2],
	  yuv_pixeldata[3],  rgba_pixeldata[0], rgba_pixeldata[1],
	  rgba_pixeldata[2], rgba_pixeldata[3]);
   
  yuv422rgb_scalar_fixed(yuv_pixeldata,  source_byte_count, rgba_pixeldata); 

  fprintf(stderr, "(fixed point scalar) %d %d %d %d  RGBA %d %d %d %d\n",
	  yuv_pixeldata[0],  yuv_pixeldata[1],  yuv_pixeldata[2],
	  yuv_pixeldata[3],  rgba_pixeldata[0], rgba_pixeldata[1],
	  rgba_pixeldata[2], rgba_pixeldata[3]);

  return(0);
}



/* ************************************************************************* 


   NAME:  test_sse1


   USAGE: 

 

   test_sse1

   returns: int

   DESCRIPTION:
                 this is basically a test stub for the SSE1 yuv422rgb_*
		 functions.

		 first use init_imagedata to fill yuv_pixeldata
		 with a constant YUV color given by the RGB
		 (first three arguments) passed in.

		 then call each of the yuv422rgb_* in a loop (TEST_COUNT times)
		 to translate yuv_pixeldata into an RGBA image stored
		 in rgba_pixeldata. (the TEST_COUNT times make sure we can
		 measure the time without worrying about clock jitter
		 and allows for things like the data paging in and out
		 to happen and be averaged into the test time.

		 if we're not on X86 (__i386__ or __x86_64__ are defined)
		 then this function becomes a stub that returns zero.

   REFERENCES:

   LIMITATIONS:

   assumes image size is a multiple of 4
   
   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   init_imagedata
   gettimeofday
   timersub
   yuv422rgb_sse1
   yuv422rgb_scalar_float
   yuv422rgb_scalar_fixed
   yuv422rgb_mmx1
   fprintf
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     16-Dec-09               initial coding                           gpk

 ************************************************************************* */

#if !( (defined __i386__) || (defined  __x86_64__))
int test_sse1(void)
{
  return(0);
}

#else 
int test_sse1(void)
{
  /* yuv_pixeldata contains macropixels of YUYV for two pixels */
  unsigned char yuv_pixeldata[640 * 480 * 2] __attribute__ ((aligned (16))); 
   /* rgba  for each pixel  */
  unsigned char rgba_pixeldata[640 * 480 * 4] __attribute__ ((aligned (16)));
  /* __attribute__ ((aligned (16))); */
  int source_byte_count = sizeof(yuv_pixeldata);
  struct timeval one, two, three, four, five, delta;
  int i, timeerror;
  float vector_float_sec, vector_int_sec;
  float scalar_float_sec, scalar_int_sec;
  
  init_imagedata (127, 56, 43, yuv_pixeldata, source_byte_count);

  /* test yuv422rgb_sse1 (floating point vector) in the interval */
  /* between one and two.  */
  timeerror= gettimeofday(&one, NULL);

  if (-1 == timeerror)
    {
      perror("Error gettimg time one");
      return(-1);
    }
  for (i = 0; i < TEST_COUNT; i++)
    {
      yuv422rgb_sse1(yuv_pixeldata,  source_byte_count, rgba_pixeldata);
    }
   timeerror= gettimeofday(&two, NULL);
  if (-1 == timeerror)
    {
      perror("Error gettimg time two");
      return(-1);
    }
  /* test yuv422rgb_scalar_float (floating point scalar)  in the interval */
  /* between two and three.  */
  
   for (i = 0; i < TEST_COUNT; i++)
     {
       yuv422rgb_scalar_float(yuv_pixeldata,  source_byte_count,
			      rgba_pixeldata);
     }
   timeerror= gettimeofday(&three, NULL);
  if (-1 == timeerror)
    {
      perror("Error gettimg time three");
      return(-1);
    }
  /* check yuv422rgb_scalar_fixed (fixed point scalar) */
  /* between three and four*/
   for (i = 0; i < TEST_COUNT; i++)
     {
       yuv422rgb_scalar_fixed(yuv_pixeldata, source_byte_count, rgba_pixeldata);
     }
   timeerror=  gettimeofday(&four, NULL);
  if (-1 == timeerror)
    {
      perror("Error gettimg time four");
      return(-1);
    }
   /* test yuv422rgb_mmx (fixed point vector) between four and five.  */
   
   for (i = 0; i < TEST_COUNT; i++)
     {
       yuv422rgb_mmx1(yuv_pixeldata,  source_byte_count, rgba_pixeldata);
     }
   
    timeerror= gettimeofday(&five, NULL);
  if (-1 == timeerror)
    {
      perror("Error gettimg time five");
      return(-1);
    }
   /* now use timersub to find the deltas between times  */
   timersub(&five, &four, &delta);

   vector_int_sec = delta.tv_sec + delta.tv_usec / USEC_PER_SEC;

   fprintf(stderr, "(fixed point vector) mmx took %f seconds\n",
	   vector_int_sec);
   
   timersub(&four, &three, &delta);

   scalar_int_sec = delta.tv_sec + delta.tv_usec / USEC_PER_SEC;

   fprintf(stderr,
	   "(fixed point scalar) yuv422rgb_scalar_fixed took %f seconds\n",
	   scalar_int_sec);
   
   timersub(&three, &two, &delta);
   
   scalar_float_sec = delta.tv_sec + delta.tv_usec / USEC_PER_SEC;

   fprintf(stderr,
	   "(floating point scalar) yuv422rgb_scalar_float took %f seconds\n",
	   scalar_float_sec);

   timersub(&two, &one, &delta);
   
   vector_float_sec = delta.tv_sec + delta.tv_usec / USEC_PER_SEC;
   fprintf(stderr, "(floating point vector) sse1 took %f seconds\n",
	   vector_float_sec);

   fprintf(stderr,
	   "\nfloat vector time was %f percent of float scalar time\n",
	   (100.0 * vector_float_sec) / scalar_float_sec);

   fprintf(stderr,
	   "fixed point vector was %f percent of fixed point scalar time\n\n",
	   (100.0 * vector_int_sec) / scalar_int_sec);
 
   /* now run them each once to get the answers and print them out for  */
   /* comparison....  */
   
   yuv422rgb_scalar_float(yuv_pixeldata,  source_byte_count, rgba_pixeldata);
   
   fprintf(stderr, "yuyv scalar %d %d %d %d  RGBA %d %d %d %d\n",
	   yuv_pixeldata[0],  yuv_pixeldata[1],  yuv_pixeldata[2],
	   yuv_pixeldata[3],  rgba_pixeldata[0], rgba_pixeldata[1],
	   rgba_pixeldata[2], rgba_pixeldata[3]);
   
   yuv422rgb_scalar_fixed(yuv_pixeldata,  source_byte_count, rgba_pixeldata); 

   fprintf(stderr, "yuyv_int %d %d %d %d  RGBA %d %d %d %d\n",
	   yuv_pixeldata[0],  yuv_pixeldata[1],  yuv_pixeldata[2],
	   yuv_pixeldata[3],  rgba_pixeldata[0], rgba_pixeldata[1],
	   rgba_pixeldata[2], rgba_pixeldata[3]);

 
   yuv422rgb_mmx1(yuv_pixeldata,  source_byte_count, rgba_pixeldata);

   fprintf(stderr, "yuyv_mmx %d %d %d %d  RGBA %d %d %d %d\n",
	   yuv_pixeldata[0],  yuv_pixeldata[1],  yuv_pixeldata[2],
	   yuv_pixeldata[3],  rgba_pixeldata[0], rgba_pixeldata[1],
	   rgba_pixeldata[2], rgba_pixeldata[3]);
   
   yuv422rgb_sse1(yuv_pixeldata,  source_byte_count, rgba_pixeldata);

   fprintf(stderr, "yuyv_sse %d %d %d %d  RGBA %d %d %d %d\n",
	   yuv_pixeldata[0],  yuv_pixeldata[1],  yuv_pixeldata[2],
	   yuv_pixeldata[3],  rgba_pixeldata[0], rgba_pixeldata[1],
	   rgba_pixeldata[2], rgba_pixeldata[3]);

   return(0);
}
#endif /* !( (defined __i386__) || (defined  __x86_64__)) */

/* ************************************************************************* 


   NAME:  test_sse2


   USAGE: 

 

   test_sse2

   returns: int

   DESCRIPTION:
                 this is basically a test stub for the SSE2 yuv422rgb_*
		 functions.

		 first use init_imagedata to fill yuv_pixeldata
		 with a constant YUV color given by the RGB
		 (first three arguments) passed in.

		 then call each of the yuv422rgb_* in a loop (TEST_COUNT times)
		 to translate yuv_pixeldata into an RGBA image stored
		 in rgba_pixeldata. (the TEST_COUNT times make sure we can
		 measure the time without worrying about clock jitter
		 and allows for things like the data paging in and out
		 to happen and be averaged into the test time.

		 if we're not on X86 (__i386__ or __x86_64__ are defined)
		 then this function becomes a stub that returns zero.
		 

   REFERENCES:

   LIMITATIONS:

   assumes image size is a multiple of 4
   
   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   init_imagedata
   gettimeofday
   timersub
   yuv422rgb_sse2
   yuv422rgb_scalar_float
   yuv422rgb_scalar_fixed
   yuv422rgb_mmx2
   fprintf
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     16-Dec-09               initial coding                           gpk
      6-Mar-11   add in vector time as percentage of scalar           gpk
 
 ************************************************************************* */

#if  !( (defined __i386__) || (defined  __x86_64__))

int test_sse2(void)
{
  return(0);
}
#else
int test_sse2(void)
{
  /* yuv_pixeldata contains macropixels of YUYV for two pixels */
  unsigned char yuv_pixeldata[640 * 480 * 2] __attribute__ ((aligned (16)));
   /* rgba  for each pixel  */
  unsigned char rgba_pixeldata[640 * 480 * 4] __attribute__ ((aligned (16)));
  int source_byte_count = sizeof(yuv_pixeldata);
  struct timeval one, two, three, four, five, delta;
  int i, timeerror;
  float vector_float_sec, vector_int_sec;
  float scalar_float_sec, scalar_int_sec;

  
  init_imagedata (127, 56, 43, yuv_pixeldata, source_byte_count);

  /* test yuv422rgb_sse2 (floating point vector) in the interval */
  /* between one and two.  */
  timeerror= gettimeofday(&one, NULL);

  if (-1 == timeerror)
    {
      perror("Error gettimg time one");
      return(-1);
    }
  for (i = 0; i < TEST_COUNT; i++)
    {
      yuv422rgb_sse2(yuv_pixeldata,  source_byte_count, rgba_pixeldata);
    }
   timeerror= gettimeofday(&two, NULL);
  if (-1 == timeerror)
    {
      perror("Error gettimg time two");
      return(-1);
    }
  /* test yuv422rgb_scalar_float (floating point scalar) in the interval */
  /* between two and three.  */
  
   for (i = 0; i < TEST_COUNT; i++)
     {
       yuv422rgb_scalar_float(yuv_pixeldata,  source_byte_count,
			      rgba_pixeldata);
     }
   timeerror= gettimeofday(&three, NULL);
  if (-1 == timeerror)
    {
      perror("Error gettimg time three");
      return(-1);
    }
  /* check yuv422rgb_scalar_fixed (fixed point scalar) between */
  /* three and four  */
   for (i = 0; i < TEST_COUNT; i++)
     {
       yuv422rgb_scalar_fixed(yuv_pixeldata,  source_byte_count,
			      rgba_pixeldata);
     }
   timeerror=  gettimeofday(&four, NULL);
  if (-1 == timeerror)
    {
      perror("Error gettimg time four");
      return(-1);
    }
   /* test yuv422rgb_mmx2 (fixed point vector) between four and five.  */
   
   for (i = 0; i < TEST_COUNT; i++)
     {
       yuv422rgb_mmx2(yuv_pixeldata,  source_byte_count, rgba_pixeldata);
     }
   
    timeerror= gettimeofday(&five, NULL);
  if (-1 == timeerror)
    {
      perror("Error gettimg time five");
      return(-1);
    }
   /* now use timersub to find the deltas between times  */
   timersub(&five, &four, &delta);

   vector_int_sec = delta.tv_sec + delta.tv_usec / USEC_PER_SEC;

   fprintf(stderr, "(fixed point vector) mmx took %f seconds\n", 
	   vector_int_sec);
   
   timersub(&four, &three, &delta);

   scalar_int_sec = delta.tv_sec + delta.tv_usec / USEC_PER_SEC;

   fprintf(stderr,
	   "(fixed point scalar) yuv422rgb_scalar_fixed took %f seconds\n",
	   scalar_int_sec);
   
   timersub(&three, &two, &delta);
   
   scalar_float_sec = delta.tv_sec + delta.tv_usec / USEC_PER_SEC;

   fprintf(stderr,
	   "(floating point scalar) yuv422rgb_scalar_float took %f seconds\n",
	   scalar_float_sec);

   timersub(&two, &one, &delta);
   
   vector_float_sec = delta.tv_sec + delta.tv_usec / USEC_PER_SEC;
   fprintf(stderr, "(floating point vector) sse2 took %f seconds\n", 
	   vector_float_sec);


   fprintf(stderr,
	   "\nfloat vector time was %f percent of float scalar time\n",
	   (100.0 * vector_float_sec) / scalar_float_sec);

   fprintf(stderr,
	   "fixed point vector was %f percent of fixed point scalar time\n\n",
	   (100.0 * vector_int_sec) / scalar_int_sec);
 
 
   /* now run them each once to get the answers and print them out for  */
   /* comparison....  */
   
   yuv422rgb_scalar_float(yuv_pixeldata,  source_byte_count, rgba_pixeldata);
   
   fprintf(stderr, "yuyv scalar %d %d %d %d  RGBA %d %d %d %d\n",
	   yuv_pixeldata[0],  yuv_pixeldata[1],  yuv_pixeldata[2],
	   yuv_pixeldata[3],  rgba_pixeldata[0], rgba_pixeldata[1],
	   rgba_pixeldata[2], rgba_pixeldata[3]);
   
   yuv422rgb_scalar_fixed(yuv_pixeldata,  source_byte_count, rgba_pixeldata); 

   fprintf(stderr, "yuyv_int %d %d %d %d  RGBA %d %d %d %d\n",
	   yuv_pixeldata[0],  yuv_pixeldata[1],  yuv_pixeldata[2],
	   yuv_pixeldata[3],  rgba_pixeldata[0], rgba_pixeldata[1],
	   rgba_pixeldata[2], rgba_pixeldata[3]);

 
   yuv422rgb_mmx2(yuv_pixeldata,  source_byte_count, rgba_pixeldata);

   fprintf(stderr, "yuyv_mmx2 %d %d %d %d  RGBA %d %d %d %d\n",
	   yuv_pixeldata[0],  yuv_pixeldata[1],  yuv_pixeldata[2],
	   yuv_pixeldata[3],  rgba_pixeldata[0], rgba_pixeldata[1],
	   rgba_pixeldata[2], rgba_pixeldata[3]);
   
   yuv422rgb_sse2(yuv_pixeldata,  source_byte_count, rgba_pixeldata);

   fprintf(stderr, "yuyv_sse2 %d %d %d %d  RGBA %d %d %d %d\n",
	   yuv_pixeldata[0],  yuv_pixeldata[1],  yuv_pixeldata[2],
	   yuv_pixeldata[3],  rgba_pixeldata[0], rgba_pixeldata[1],
	   rgba_pixeldata[2], rgba_pixeldata[3]);
   return(0);
}
#endif /* !( (defined __i386__) || (defined  __x86_64__))  */




/* ************************************************************************* 


   NAME:  test_neon


   USAGE: 

   int some_int;
  

   some_int =  test_neon();

   returns: int

   DESCRIPTION:
                 
                 this is basically a test stub for the neon yuv422rgb_*
		 vector functions.

		 first use init_imagedata to fill yuv_pixeldata
		 with a constant YUV color given by the RGB
		 (first three arguments) passed in.

		 then call each of the yuv422rgb_* in a loop (TEST_COUNT times)
		 to translate yuv_pixeldata into an RGBA image stored
		 in rgba_pixeldata. (the TEST_COUNT times make sure we can
		 measure the time without worrying about clock jitter
		 and allows for things like the data paging in and out
		 to happen and be averaged into the test time.

		 if we're not on ARM with neon intrinsics (__ARM_NEON__ defined)
		 then this function becomes a stub that returns zero.


   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   init_imagedata
   fprintf
   gettimeofday
   yuv422rgb_neon_int
   perror
   yuv422rgb_scalar_float
   yuv422rgb_scalar_fixed
   yuv422rgb_neon_float
   timersub
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     23-Oct-10               initial coding                           gpk
     12-Mar-11      added time percent computation                    gpk
     
 ************************************************************************* */

#ifndef __ARM_NEON__
int test_neon(void)
{
  fprintf(stderr, "Warning: neon features not supported\n,");
  fprintf(stderr, " Did you use the right compile options?\n,");
  
  return(-1);
}

#else
int test_neon(void)
{
   /* yuv_pixeldata contains macropixels of YUYV for two pixels */
  unsigned char yuv_pixeldata[640 * 480 * 2] __attribute__ ((aligned (16)));
   /* rgba  for each pixel  */
  unsigned char rgba_pixeldata[640 * 480 * 4] __attribute__ ((aligned (16)));
  int source_byte_count = sizeof(yuv_pixeldata);
  struct timeval one, two, three, four, five, delta;
  int i, timeerror;
  float vector_float_sec, vector_int_sec;
  float scalar_float_sec, scalar_int_sec;
 
  init_imagedata (127, 56, 43, yuv_pixeldata, source_byte_count);

  /* test yuv422rgb_neon_int (fixed point vector) in the interval between */
  /* one and two.  */
  timeerror= gettimeofday(&one, NULL);

  if (-1 == timeerror)
    {
      perror("Error gettimg time one");
      return(-1);
    }
  for (i = 0; i < TEST_COUNT; i++)
    {
      yuv422rgb_neon_int(yuv_pixeldata,  source_byte_count, rgba_pixeldata);
    }
   timeerror= gettimeofday(&two, NULL);
   
  if (-1 == timeerror)
    {
      perror("Error gettimg time two");
      return(-1);
    }

  /* test yuv422rgb_scalar_float (floating point scalar) in the interval */
  /* between two and three.  */
  
   for (i = 0; i < TEST_COUNT; i++)
     {
       yuv422rgb_scalar_float(yuv_pixeldata,  source_byte_count,
			      rgba_pixeldata);
     }
   timeerror= gettimeofday(&three, NULL);
  if (-1 == timeerror)
    {
      perror("Error gettimg time three");
      return(-1);
    }
  /* check yuv422rgb_scalar_fixed (fixed point scalar) between */
  /* three and four  */
   for (i = 0; i < TEST_COUNT; i++)
     {
       yuv422rgb_scalar_fixed(yuv_pixeldata,  source_byte_count,
			      rgba_pixeldata);
     }
   timeerror=  gettimeofday(&four, NULL);
  if (-1 == timeerror)
    {
      perror("Error gettimg time four");
      return(-1);
    }
   /* test yuv422rgb_neon_float (floating point vector) between four and five.*/
   
   for (i = 0; i < TEST_COUNT; i++)
     {
       yuv422rgb_neon_float(yuv_pixeldata,  source_byte_count, rgba_pixeldata);
     }
   
    timeerror= gettimeofday(&five, NULL);
  if (-1 == timeerror)
    {
      perror("Error gettimg time five");
      return(-1);
    }
   /* now use timersub to find the deltas between times  */
   timersub(&five, &four, &delta);

   vector_float_sec = delta.tv_sec + delta.tv_usec / USEC_PER_SEC;

   fprintf(stderr, "(floating point vector) yuv422rgb_neon_float %f seconds\n",
	   vector_float_sec);

   timersub(&four, &three, &delta);

   scalar_int_sec = delta.tv_sec + delta.tv_usec / USEC_PER_SEC;

   fprintf(stderr,
	   "(fixed point scalar) yuv422rgb_scalar_fixed took %f seconds\n",
	   scalar_int_sec);

   timersub(&three, &two, &delta);
   
   scalar_float_sec = delta.tv_sec + delta.tv_usec / USEC_PER_SEC;

   fprintf(stderr,
	   "(floating point scalar) yuv422rgb_scalar_float took %f seconds\n",
	   scalar_float_sec);
  
   timersub(&two, &one, &delta);
   
   vector_int_sec = delta.tv_sec + delta.tv_usec / USEC_PER_SEC;
   fprintf(stderr, "(fixed point vector) yuv422rgb_neon_int took %f seconds\n",
	   vector_int_sec);


   fprintf(stderr,
	   "\nfloat vector time was %f percent of float scalar time\n",
	   (100.0 * vector_float_sec) / scalar_float_sec);

   fprintf(stderr,
	   "fixed point vector was %f percent of fixed point scalar time\n\n",
	   (100.0 * vector_int_sec) / scalar_int_sec);
 
 

   /* now run them each once to get the answers and print them out for  */
   /* comparison....  */
   yuv422rgb_scalar_float(yuv_pixeldata,  source_byte_count, rgba_pixeldata);
   
   fprintf(stderr, "yuyv scalar %d %d %d %d  RGBA %d %d %d %d\n",
	   yuv_pixeldata[0],  yuv_pixeldata[1],  yuv_pixeldata[2],
	   yuv_pixeldata[3],  rgba_pixeldata[0], rgba_pixeldata[1],
	   rgba_pixeldata[2], rgba_pixeldata[3]);
   
   yuv422rgb_scalar_fixed(yuv_pixeldata,  source_byte_count, rgba_pixeldata); 

   fprintf(stderr, "yuyv_int %d %d %d %d  RGBA %d %d %d %d\n",
	   yuv_pixeldata[0],  yuv_pixeldata[1],  yuv_pixeldata[2],
	   yuv_pixeldata[3],  rgba_pixeldata[0], rgba_pixeldata[1],
	   rgba_pixeldata[2], rgba_pixeldata[3]);


   yuv422rgb_neon_float(yuv_pixeldata,  source_byte_count, rgba_pixeldata);

   fprintf(stderr, "yuv422rgb_neon_float %d %d %d %d  RGBA %d %d %d %d\n",
	   yuv_pixeldata[0],  yuv_pixeldata[1],  yuv_pixeldata[2],
	   yuv_pixeldata[3],  rgba_pixeldata[0], rgba_pixeldata[1],
	   rgba_pixeldata[2], rgba_pixeldata[3]);

   yuv422rgb_neon_int(yuv_pixeldata,  source_byte_count, rgba_pixeldata);

   fprintf(stderr, "yuv422rgb_neon_int %d %d %d %d  RGBA %d %d %d %d\n",
	   yuv_pixeldata[0],  yuv_pixeldata[1],  yuv_pixeldata[2],
	   yuv_pixeldata[3],  rgba_pixeldata[0], rgba_pixeldata[1],
	   rgba_pixeldata[2], rgba_pixeldata[3]);




   
  return(0);
}
#endif /* __ARM_NEON__  */

/* ************************************************************************* 


   NAME:  test_altivec


   USAGE: 

   int some_int;

   some_int =  test_altivec();

   returns: int

   DESCRIPTION:
                this is basically a test stub for the altivec yuv422rgb_*
		 vector functions.

		 first use init_imagedata to fill yuv_pixeldata
		 with a constant YUV color given by the RGB
		 (first three arguments) passed in.

		 then call each of the yuv422rgb_* in a loop (TEST_COUNT times)
		 to translate yuv_pixeldata into an RGBA image stored
		 in rgba_pixeldata. (the TEST_COUNT times make sure we can
		 measure the time without worrying about clock jitter
		 and allows for things like the data paging in and out
		 to happen and be averaged into the test time.

                 

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   init_imagedata
   gettimeofday
   perror
   yuv422rgb_avc_int
   yuv422rgb_scalar_float
   yuv422rgb_scalar_fixed
   yuv422rgb_avc_float
   timersub
   fprintf
   


   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     23-Oct-10               initial coding                           gpk
     16-Jan-11  print the percentage of time vector code took         gpk
                compared to scalar code
 ************************************************************************* */

#ifndef __VEC__

int test_altivec(void)
{
  fprintf(stderr, "Warning: altivec features not supported\n,");
  fprintf(stderr, " Did you use the right compile options?\n,");
  
  return(-1);
}


#else
int test_altivec(void)
{
   /* yuv_pixeldata contains macropixels of YUYV for two pixels */
  unsigned char yuv_pixeldata[640 * 480 * 2] __attribute__ ((aligned (16)));
   /* rgba  for each pixel  */
  unsigned char rgba_pixeldata[640 * 480 * 4] __attribute__ ((aligned (16)));
  int source_byte_count = sizeof(yuv_pixeldata);
  struct timeval one, two, three, four, five, delta;
  int i, timeerror;
  float float_vector_seconds, int_vector_seconds;
  float float_scalar_seconds, int_scalar_seconds;
  
  init_imagedata (127, 56, 43, yuv_pixeldata, source_byte_count);

  /* test yuv422rgb_avc_int (fixed point vector) in the interval */
  /*  between one and two.  */
  timeerror= gettimeofday(&one, NULL);

  if (-1 == timeerror)
    {
      perror("Error getting time one");
      return(-1);
    }
  for (i = 0; i < TEST_COUNT; i++)
    {
      yuv422rgb_avc_int(yuv_pixeldata,  source_byte_count, rgba_pixeldata);
    }
   timeerror= gettimeofday(&two, NULL);
   
  if (-1 == timeerror)
    {
      perror("Error gettimg time two");
      return(-1);
    }

  /* test yuv422rgb_scalar_float (floating point scalar) in the interval */
  /* between two and three.  */
  
   for (i = 0; i < TEST_COUNT; i++)
     {
       yuv422rgb_scalar_float(yuv_pixeldata,  source_byte_count,
			      rgba_pixeldata);
     }
   timeerror= gettimeofday(&three, NULL);
  if (-1 == timeerror)
    {
      perror("Error gettimg time three");
      return(-1);
    }
  /* check yuv422rgb_scalar_fixed (fixed point scalar) between */
  /* three and four  */
   for (i = 0; i < TEST_COUNT; i++)
     {
       yuv422rgb_scalar_fixed(yuv_pixeldata,  source_byte_count,
			      rgba_pixeldata);
     }
   timeerror=  gettimeofday(&four, NULL);
  if (-1 == timeerror)
    {
      perror("Error gettimg time four");
      return(-1);
    }
  /* test yuv422rgb_avc_float (floating point vector) */
  /* between four and five.  */
   
   for (i = 0; i < TEST_COUNT; i++)
     {
       yuv422rgb_avc_float(yuv_pixeldata,  source_byte_count,
			       rgba_pixeldata);
     }
   
    timeerror= gettimeofday(&five, NULL);
  if (-1 == timeerror)
    {
      perror("Error gettimg time five");
      return(-1);
    }
   /* now use timersub to find the deltas between times  */
   timersub(&five, &four, &delta);

   float_vector_seconds = delta.tv_sec + delta.tv_usec / USEC_PER_SEC;

   fprintf(stderr, "(floating point vector) yuv422rgb_avc_float %f seconds\n",
	   float_vector_seconds);

   timersub(&four, &three, &delta);

   int_scalar_seconds = delta.tv_sec + delta.tv_usec / USEC_PER_SEC;

   fprintf(stderr,
	   "(fixed point scalar) yuv422rgb_scalar_fixed took %f seconds\n",
	   int_scalar_seconds);

   timersub(&three, &two, &delta);
   
   float_scalar_seconds = delta.tv_sec + delta.tv_usec / USEC_PER_SEC;

   fprintf(stderr,
	   "(floating point scalar) yuv422rgb_scalar_float took %f seconds\n",
	   float_scalar_seconds);
  
   timersub(&two, &one, &delta);
   
   int_vector_seconds = delta.tv_sec + delta.tv_usec / USEC_PER_SEC;
   fprintf(stderr, "(fixed point vector) yuv422rgb_avc_int took %f seconds\n",
	   int_vector_seconds);

   fprintf(stderr, 
	   "\nfloating vector time was %f percent of floating scalar time\n", 
	   (100.0 * float_vector_seconds / float_scalar_seconds));

   fprintf(stderr, 
	   "fixed point vector was %f percent of fixed point scalar time\n\n", 
	   (100.0 * int_vector_seconds / int_scalar_seconds));

   /* now run them each once to get the answers and print them out for  */
   /* comparison....  */
   yuv422rgb_scalar_float(yuv_pixeldata,  source_byte_count, rgba_pixeldata);
   
   fprintf(stderr, 
	  "(floating point scalar) yuyv scalar %d %d %d %d  RGBA %d %d %d %d\n",
	   yuv_pixeldata[0],  yuv_pixeldata[1],  yuv_pixeldata[2],
	   yuv_pixeldata[3],  rgba_pixeldata[0], rgba_pixeldata[1],
	   rgba_pixeldata[2], rgba_pixeldata[3]);
   
   yuv422rgb_scalar_fixed(yuv_pixeldata,  source_byte_count, rgba_pixeldata); 

   fprintf(stderr, 
	   "(scalar fixed point) yuyv_int %d %d %d %d  RGBA %d %d %d %d\n",
	   yuv_pixeldata[0],  yuv_pixeldata[1],  yuv_pixeldata[2],
	   yuv_pixeldata[3],  rgba_pixeldata[0], rgba_pixeldata[1],
	   rgba_pixeldata[2], rgba_pixeldata[3]);


   yuv422rgb_avc_float(yuv_pixeldata,  source_byte_count, rgba_pixeldata);

   fprintf(stderr, 
	   "(vector float)yuv422rgb_avc_float %d %d %d %d  RGBA %d %d %d %d\n",
	   yuv_pixeldata[0],  yuv_pixeldata[1],  yuv_pixeldata[2],
	   yuv_pixeldata[3],  rgba_pixeldata[0], rgba_pixeldata[1],
	   rgba_pixeldata[2], rgba_pixeldata[3]);

   yuv422rgb_avc_int(yuv_pixeldata,  source_byte_count, rgba_pixeldata);

   fprintf(stderr, 
	  "(fixed point vector) yuv422rgb_avc_int %d %d %d %d  RGBA %d %d %d %d\n",
	   yuv_pixeldata[0],  yuv_pixeldata[1],  yuv_pixeldata[2],
	   yuv_pixeldata[3],  rgba_pixeldata[0], rgba_pixeldata[1],
	   rgba_pixeldata[2], rgba_pixeldata[3]);

  return(0);
}

#endif /* __VEC__ */

/* ************************************************************************* 


   NAME:  main


   USAGE: 

 

  

   returns: int

   DESCRIPTION:
                 this is basically a test stub for the yuv422rgb_*
		 
		 it calls test_cpu_simd_features to see whether
		 the processor supports MMX, SSE1, SSE2, NEON, altivec.
		 if the processor supports those extensions,
		 main calls the test stub for them.

   REFERENCES:

   LIMITATIONS:
   
   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   test_cpu_simd_features
   fprintf
   test_sse2
   test_sse1
   test_neon
   test_altivec
   test_scalar
   test_general_int
   test_general_float
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     16-Dec-09               initial coding                           gpk
     23-Oct-10            added neon, altivec                         gpk
     
 ************************************************************************* */


int main(int argc, char * argv[])
{
  Cpusimdfeatures_t supported;
  int retval;

  /* see which features we support: store results in supported structure  */
  retval = test_cpu_simd_features(&supported);

  if  (supported.sse2) /* we can do SSE2  */
    {
      fprintf(stderr, "Testing SSE2:\n");
      test_sse2();
    }
  else
    {
      fprintf(stderr, "SSE2 not supported: skipping test\n");
    }
  
  if  (supported.sse1) /* we can do SSE1  */
    {
      fprintf(stderr, "Testing SSE1:\n");
      test_sse1();
    }
  else
    {
      fprintf(stderr, "SSE1 not supported: skipping test\n");
    }

  if (supported.neon)
    {
      fprintf(stderr, "Testing NEON:\n");
      test_neon();
    }
  else
    {
      fprintf(stderr, "NEON not supported: skipping test\n");
    }
  
  if (supported.altivec)
    {
      fprintf(stderr, "Testing ALTIVEC:\n");
      test_altivec();
    }
  else
    {
      fprintf(stderr, "ALTIVEC not supported: skipping test\n");
    }

  
  fprintf(stderr, "Testing scalar functions:\n");
  test_scalar();
    

  /* call the general functions to make sure it works  */
  test_general_int();
  test_general_float();
  
  return(0);
}

