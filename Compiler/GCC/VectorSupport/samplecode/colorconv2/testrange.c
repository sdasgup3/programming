/* ************************************************************************* 
* NAME: colorconv/testrange.c
*
* DESCRIPTION:
*
* this is code used to test the color conversion library. it calls the
* vector parts of the library with all the YUV combinations and compares
* the vector outputs with the scalar outputs from the same inputs.
* functions are:
*
* * init_imagedata2 - make a YUYV image of all possible YUV inputs
*
* * init_imagedata - put YUV combinations into a buffer starting at
*   a given value
*
* 
* * print_pixel
*
* * find_mismatches - locate and describe the differences between
*   two images
*
* * compare_rgba - compare images, print summary of results
*
* * range_test - compare images on machines with large memory
*
* * small_memory_range_test - compare images on machines with
*   small memory
*
* 
* 
* PROCESS:
*
* make
* ./`uname -i`/testrange
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
*   28-Jan-11  remove unneeded parts from speed test         gpk
*    1-Apr-11  added comment blocks                          gpk
*
* TARGET: GCC version 4 or higher
*
*
* ************************************************************************* */

#include <stdio.h>
#include <string.h> /* memcpy memset */
#include <stdlib.h> /* abs */


#include "colorconv.h" /* test_cpu_simd_features yuv422rgb_*  */



/*  INPUTRANGESIZE - size of YUYV buffer needed to get all combinations */
/* of YUV from {0, 0, 0} to {255, 255, 255} at 4 bytes per pixel   */

#define INPUTRANGESIZE 0x4000000

/* OUTPUTRANGESIZE - size of buffer needed to hold an image of */
/* INPUTRANGESIZE: basically 2 RGBA's out for each YUYV in */

#define OUTPUTRANGESIZE (2 * INPUTRANGESIZE) /* bytes  */


/* INPUTIMAGESIZE - the allowed size for an input image */
/* (640x480 pixels expressed as YUYV)                   */

#define INPUTIMAGESIZE (640 * 480 * 2)


/* OUTPUTIMAGESIZE - size of output image: 640x480 pixels expressed as RGBA */

#define OUTPUTIMAGESIZE (2 * INPUTIMAGESIZE)



/* Y0_INDEX - where to get Y0 from  Ibytes_t's bytes field */
/* U_INDEX - where to get U from  Ibytes_t's bytes field  */
/* V_INDEX - where to get V from   Ibytes_t's bytes field  */

/* note: __VEC__ auto-defined by altivec. it's  shorthand */
/* for big-endian here.  */

#ifndef  __VEC__
#define Y0_INDEX 0
#define U_INDEX  1
#define V_INDEX  2
#else
#define Y0_INDEX 3
#define U_INDEX  2
#define V_INDEX  1
#endif /* __VEC__ */


/* SMALL_MEMORY - if this is defined, the range test will  */
/* be done over a series of small images instead of 1 large one  */
/* NOTE: this can also be defined in the architecture-specific Makefile   */
/* #define SMALL_MEMORY */

/* Ibytes_t - a union for generating and comparing combinations of   */
/* YUYV either as ints or as bytes.   */

typedef union ibytes_u {
  unsigned char bytes[sizeof(int)];
  int value;
} Ibytes_t;






/* ************************************************************************* 


   NAME:  init_imagedata


   USAGE: 

   int some_int;
   int byte_count; -- size of YUV422 image in bytes   
   unsigned char * yuv_imagep = malloc(byte_count);

   int startvalue; -- where you want the pixel values to start
   
   some_int =  init_imagedata(yuv_imagep, byte_count, startvalue);

   returns: int

   DESCRIPTION:
                 build a YUYV image starting with value in startvalue
		 and increasing.

		 Y0, U, V come from the counter value
		 Y1 is 255 minus counter value

		 that makes sure we cover all the YUV combinations
   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   REVISION HISTORY:

        STR                  Description of Revision                 Author

     27-Mar-11               initial coding                           gpk

 ************************************************************************* */

int init_imagedata(unsigned char * yuv_imagep, int byte_count,
		    int startvalue)
{
  Ibytes_t i;
  unsigned char y0, u, y1, v;
  unsigned char * image_endp;
  
  image_endp = yuv_imagep + byte_count;
  i.value = startvalue;
  
  while (yuv_imagep < image_endp)
    {
      y0 = i.bytes[Y0_INDEX];
      u = i.bytes[U_INDEX];
      y1 = 255 - y0;
      v = i.bytes[V_INDEX];
      *(yuv_imagep++) = (unsigned char)y0;
      *(yuv_imagep++) = (unsigned char)u;
      *(yuv_imagep++) = (unsigned char)y1;
      *(yuv_imagep++) = (unsigned char)v;
      
      i.value = i.value + 1;
      
    }

  return(i.value);
}


/* ************************************************************************* 


   NAME:  init_imagedata2


   USAGE: 

    
   unsigned char * yuv_imagep;
    int byte_count;

   init_imagedata2(yuv_imagep, byte_count);

   returns: void

   DESCRIPTION:
                 fill byte_count bytes pointed to by yuv_imagep with
		 a pixel test pattern.

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   REVISION HISTORY:

        STR                  Description of Revision                 Author

     28-Jan-11               initial coding                           gpk

 ************************************************************************* */

void init_imagedata2(unsigned char * yuv_imagep, int byte_count)
{
  init_imagedata(yuv_imagep, byte_count, 0);

}


/* ************************************************************************* 


   NAME:  print_pixel


   USAGE: 

    
   char * label;
   unsigned char * image;
   long bytecount;

   print_pixel(label, image, bytecount);

   returns: void

   DESCRIPTION:
                 print label, followed by bytecount bytes of data
		 from image in groups of 4 (appropriate for RGBA
		 or YUYV images).

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   REVISION HISTORY:

        STR                  Description of Revision                 Author

     28-Jan-11               initial coding                           gpk

 ************************************************************************* */

void print_pixel(char * label, unsigned char * image, long bytecount)
{
  int i;
  
  fprintf(stderr, "%s", label);

  for(i = 0; i < bytecount; i +=4)
    {
      fprintf(stderr, "%d %d %d %d\n", (int)image[i], (int)image[i+1],
	      (int)image[i+2], (int)image[i+3]); 
    }

}






/* ************************************************************************* 


   NAME:  find_mismatches


   USAGE: 

   int mismatch_count;
   unsigned char * image1;
   unsigned char * image2;
   
   unsigned long imagesize;
   unsigned char * sourceyuyv;

   mismatch_count =  find_mismatches(image1, image2, imagesize, sourceyuyv);

   returns: int

   DESCRIPTION:
                compare the contents of image1 and image2 for
		imagesize bytes. if they don't match,
		print out the values, where they differ, and the
		content of sourceyuyv that they came from.

		return the number of mismatching pixels.
		
   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   REVISION HISTORY:

        STR                  Description of Revision                 Author

     27-Mar-11               initial coding                           gpk

 ************************************************************************* */

int find_mismatches(unsigned char * image1, unsigned char * image2,
		    unsigned long imagesize, unsigned char * sourceyuyv)
{
  Ibytes_t * pixel1p;
  Ibytes_t * pixel2p;
  Ibytes_t * sourcep;
  unsigned long i, mismatches, pixels;
  int difference, rdiff, gdiff, bdiff;

  pixels = imagesize / 4; /* 4 bytes per pixel (RGBA)  */
  mismatches = 0;
  pixel1p = (Ibytes_t *)image1;
  pixel2p = (Ibytes_t *)image2;
  sourcep = (Ibytes_t *)sourceyuyv;

  /* we're testing 4 bytes at a time so we only need to go to   */
  /* pixels instead of imagesize  */
  for (i = 0; i < pixels; i++)
    {
      if (pixel1p->value != pixel2p->value)
	{
	  difference = abs(pixel1p->bytes[0] - pixel2p->bytes[0]) +
	    abs(pixel1p->bytes[1] - pixel2p->bytes[1]) +
	    abs(pixel1p->bytes[2] - pixel2p->bytes[2]) +
	    abs(pixel1p->bytes[3] - pixel2p->bytes[3]);
	  rdiff = (pixel1p->bytes[0] != pixel2p->bytes[0]);
	  gdiff = (pixel1p->bytes[1] != pixel2p->bytes[1]);
	  bdiff = (pixel1p->bytes[2] != pixel2p->bytes[2]);
	  fprintf(stderr, "YUYV [%d %d %d %d] ", 
		  (int)(sourcep->bytes[0]), (int)(sourcep->bytes[1]), 
		  (int)(sourcep->bytes[2]), (int)(sourcep->bytes[3]));
	  fprintf(stderr,
		  "pixel %ld [%d %d %d %d] !=  [%d %d %d %d] difference %d ",
		  i, (int)(pixel1p->bytes[0]), (int)(pixel1p->bytes[1]),
		  (int)(pixel1p->bytes[2]), (int)(pixel1p->bytes[3]),
		  (int)(pixel2p->bytes[0]), (int)(pixel2p->bytes[1]),
		  (int)(pixel2p->bytes[2]), (int)(pixel2p->bytes[3]),
		  difference);

	  /* now point out where the differences are   */
	  if (pixel1p->bytes[0] != pixel2p->bytes[0])
	    {
	      fprintf(stderr, "R");
	    }
	  if (pixel1p->bytes[1] != pixel2p->bytes[1])
	    {
	      fprintf(stderr, "G");
	    }

	  if (pixel1p->bytes[2] != pixel2p->bytes[2])
	    {
	      fprintf(stderr, "B");
	    }
	  if (pixel1p->bytes[3] != pixel2p->bytes[3])
	    {
	      fprintf(stderr, "A");
	    }
	  
	  fprintf(stderr,"\n");
	  
	  mismatches++;



	} /* if */

      pixel1p++;
      pixel2p++;
      
      /* increment sourcep every other pixel since one macropixel */
      /* generates 2 RGBA's */

       if (1 == i%2) 
	 { 
	   sourcep++;
	 }

    } /* for */

  return(mismatches);
  
}




/* ************************************************************************* 


   NAME:  compare_rgba


   USAGE: 

    
   char * label = "something useful";
   unsigned long imagesize; -- number of bytes in   image1, image2 
   unsigned char image1 = malloc(imagesize);
   unsigned char image2 = malloc(imagesize);
   unsigned char  sourceyuyv  = malloc(imagesize/2);

   -- put something into sourceyuyv
   -- put something into image1, image2 based on sourceyuyv
   
   compare_rgba(label, image1, image2, imagesize, sourceyuyv);

   returns: void

   DESCRIPTION:
                 print label, then compare the contents of image1, image2.

		 print a summary of how many mismatched pixels there are.

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   REVISION HISTORY:

        STR                  Description of Revision                 Author

     28-Jan-11               initial coding                           gpk

 ************************************************************************* */

void compare_rgba(char * label, unsigned char * image1, unsigned char * image2,
		  unsigned long imagesize, unsigned char * sourceyuyv)
{
  unsigned long mismatches, pixels;
  
  fprintf(stderr, "%s\n", label);

  mismatches = find_mismatches(image1, image2, imagesize, sourceyuyv);
  
  pixels = imagesize / 4; /* 4 bytes per pixel (RGBA)  */
  
  fprintf(stderr, "there were %ld mismatches of %ld pixels\n\n", mismatches,
	  pixels);
  

 
  
}



/* ************************************************************************* 


   NAME:  range_test


   USAGE: 

    
   range_test();

   returns: void

   DESCRIPTION:
                 when SMALL_MEMORY is not defined, this runs the range
		 test by creating the yuyvimage containing all the
		 combinations of YUV, then generating an RGBA image from
		 the scalar code and one from the vector code. then it
		 compares the two RGBA images and complains when there
		 are differences.

   REFERENCES:

   LIMITATIONS:

   because yuyvimage, scalarimg, and vectorimg are large, they must be
   declared static. on beagleboard, these are too big to allow the program
   to load (even if this code is not called). so this is not compiled if
   SMALL_MEMORY is defined. (right now that's in the beagleboard's
   unknown/Makefile.

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   REVISION HISTORY:

        STR                  Description of Revision                 Author

     28-Jan-11               initial coding                           gpk
      3-Feb-11     align the arrays on 16-byte boundaries             gpk
     25-Feb-11  test both fixed and floating point ranges             gpk
      1-Apr-11  use the interfaces that don't need the ifdef's        gpk
      
 ************************************************************************* */

#ifndef SMALL_MEMORY

void range_test(void)
{
  static unsigned char yuyvimage[INPUTRANGESIZE] __attribute__ ((aligned (16)));
  static unsigned char scalarimg[OUTPUTRANGESIZE] __attribute__((aligned (16)));
  static unsigned char vectorimg[OUTPUTRANGESIZE] __attribute__((aligned (16)));
  
  long int inputbytecount;
  unsigned char * startp;

  fprintf(stderr, "starting %s (this could take a while)\n", __FUNCTION__);

  memset(scalarimg, 0, OUTPUTRANGESIZE);
  memset(vectorimg, 0, OUTPUTRANGESIZE);
  
  startp = yuyvimage;
  inputbytecount =  INPUTRANGESIZE; 

  init_imagedata2(yuyvimage, inputbytecount);

  /* print_pixel("test range", yuyvimage, inputbytecount); */ 

  /* first test floating point functions...  */
  
  yuv422rgb_scalar_float(yuyvimage, inputbytecount, scalarimg);
  /*  print_pixel("floating point scalar image ", scalarimg, 100); */

        
  yuv422rgb_float(yuyvimage, inputbytecount, vectorimg);
  

  /*  print_pixel("floating point vector image ", vectorimg, 100); */
  compare_rgba("comparing full range floating point images", scalarimg,
	       vectorimg, OUTPUTRANGESIZE, yuyvimage);

  /* now test fixed point functions ... */
  yuv422rgb_scalar_fixed(yuyvimage, inputbytecount, scalarimg);
  
  /*  print_pixel("fixed point scalar image ", scalarimg, 50); */

  yuv422rgb_fixed(yuyvimage, inputbytecount, vectorimg);
  
  /*   print_pixel("fixed point vector image ", vectorimg, 50); */
  compare_rgba("comparing full range fixed point images", scalarimg,
	       vectorimg, OUTPUTRANGESIZE, yuyvimage);

} /* range_test */

#else




/* ************************************************************************* 


   NAME:  small_memory_range_test


   USAGE: 

   small_memory_range_test();

   returns: void

   DESCRIPTION:
                 for machines with smaller memory (like beagleboard),
		 this code conducts the range test. instead of
		 building large images with all the combinations of
		 YUV (like range_test does), this does the test using
		 a series of 640x480 images (reusing memory) that
		 span the input range.

		 along the way, it prints out mismatches. at the end,
		 it prints a summary of how many mismatches there were.
		 

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   REVISION HISTORY:

        STR                  Description of Revision                 Author

      1-Apr-11               initial coding                           gpk

 ************************************************************************* */

void small_memory_range_test(void)
{
  static unsigned char yuyvimage[INPUTIMAGESIZE] __attribute__ ((aligned (16)));
  static unsigned char scalarimg[OUTPUTIMAGESIZE] __attribute__((aligned (16)));
  static unsigned char vectorimg[OUTPUTIMAGESIZE] __attribute__((aligned (16)));
  int startvalue;
  long int inputbytecount;
  unsigned long summarypixelcount, summarymismatches, mismatches;
  unsigned char * startp;

  fprintf(stderr, "starting %s (this could take a while)\n", __FUNCTION__);

  memset(scalarimg, 0, OUTPUTIMAGESIZE);
  memset(vectorimg, 0, OUTPUTIMAGESIZE);
  
  startp = yuyvimage;
  inputbytecount =  INPUTIMAGESIZE; 
  summarypixelcount = 0;
  summarymismatches = 0;
  startvalue = 0;

  while (startvalue < INPUTRANGESIZE)
    {
      /* generate an image starting at startvalue, incrementing for  */
      /* imagebytecount. starting value for next image  */
      
      startvalue = init_imagedata(yuyvimage, inputbytecount, startvalue);
      
 
      /* generate colorspace translation of yuyvimage from scalar code  */
      /* store it in scalarimg.  */
      
      yuv422rgb_scalar_float(yuyvimage, inputbytecount, scalarimg);
      /* print_pixel("floating point scalar image ", scalarimg, 20); */
      
      yuv422rgb_float(yuyvimage, inputbytecount, vectorimg);
   
      /*  print_pixel("floating point vector image ", vectorimg, 20); */
      mismatches = find_mismatches(scalarimg, vectorimg, OUTPUTIMAGESIZE,
				   yuyvimage);
      
      summarypixelcount += OUTPUTIMAGESIZE;
      summarymismatches += mismatches;
      
      if (mismatches > 0)
	{
	  fprintf(stderr, "there were %ld mismatches of %ld pixels\n\n",
		  summarymismatches, summarypixelcount);
  
	}
      
    }

  fprintf(stderr, "Floating point had %ld mismatches of %ld pixels\n", 
	  summarymismatches, summarypixelcount);

  /* now test fixed point functions ... */
  
  summarymismatches = 0;
  summarypixelcount = 0;
  startvalue = 0;
  while (startvalue < INPUTRANGESIZE)
    {
      /* generate an image starting at startvalue, incrementing for  */
      /* imagebytecount. starting value for next image  */
      
      startvalue = init_imagedata(yuyvimage, inputbytecount, startvalue);
      
 
      /* generate colorspace translation of yuyvimage from scalar code  */
      /* store it in scalarimg.  */
      
      yuv422rgb_scalar_fixed(yuyvimage, inputbytecount, scalarimg);
      
      /* print_pixel("floating point scalar image ", scalarimg, 50); */ 

      yuv422rgb_fixed(yuyvimage, inputbytecount, vectorimg);
      
      /*  print_pixel("floating point vector image ", vectorimg, 50); */

      mismatches = find_mismatches(scalarimg, vectorimg, OUTPUTIMAGESIZE,
				   yuyvimage);
      
      summarypixelcount += OUTPUTIMAGESIZE;
      summarymismatches += mismatches;
      
      if (mismatches > 0)
	{
	  fprintf(stderr, "there were %ld mismatches of %ld pixels\n\n",
		  summarymismatches, summarypixelcount);
  
	}
      
    }
   fprintf(stderr, "Fixed point had %ld mismatches of %ld pixels\n", 
	   summarymismatches, summarypixelcount);


} /* small_memory_range_test */
#endif /* SMALL_MEMORY  */

/* ************************************************************************* 


   NAME:  main


   USAGE: 

 
   testrange
  

   returns: int

   DESCRIPTION:
                   the code there tests the vector functions of the library
		   against the scalar functions and complains about
		   differences. 

   REFERENCES:

   LIMITATIONS:
   
   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
     range_test
     small_memory_range_test
     
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     16-Dec-09               initial coding                           gpk
     23-Oct-10            added neon, altivec                         gpk
     23-Jan-2011          removed all but range test                  kaj
      1-Apr-11            added small_memory_range_test               gpk
      
 ************************************************************************* */


int main(int argc, char * argv[])
{
  
#ifdef SMALL_MEMORY
  small_memory_range_test();
#else
  range_test();   
#endif /* SMALL_MEMORY  */
  
  return(0);
}

