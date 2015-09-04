/* ************************************************************************* 
* NAME: colorconv/scalar.c
*
* DESCRIPTION:
*
* these are non-vector versions of the yuv422rgb_* functions.
* since they don't use vector processing at all, they should run
* on non-MMX/SSE/whatever machines.
*
* they have the same return type and arguments as the vectorized
* versions.
*
* yuv422rgb_scalar_float - uses floating point math to do the conversion from
*                   YUYV to RGBA. (this should be the most accurate)
* yuv422rgb_scalar_fixed - uses integer math to do the conversion (should be faster
*                   than yuv422rgb_scalar_float).
*
* 
* PROCESS:
*
* int width, height; --  of image in pixels
* int byte_count; -- number of bytes in YUV image (2 * width * height) 
* const unsigned char * sourcep = malloc(byte_count); -- source yuv image
* unsigned char * destp = malloc(4 * width * height); -- dest RGBA image 
*
* -- build your YUYV image in sourcep
* -- use yuv422rgb_scalar_float or yuv422rgb_scalar_fixed to convert from YUYV to RGBA
* 
* GLOBALS:
*
* REFERENCES:
*
* LIMITATIONS:
*
* REVISION HISTORY:
*
*   STR                Description                          Author
*
*   22-Dec-09          initial coding                        gpk
*   16-Jan-11   modified yuv422rgb_scalar_float, yuv422rgb_scalar_fixed     gpk
*               added public domain statement
*    5-Feb-11   include stdio.h for fprintf                  kaj
*
* TARGET: GNU C version 4 or higher
*
* The contents of this file is in the public domain. If it breaks,
* you get to keep both pieces.
*
* ************************************************************************* */
#include <stdio.h>  /* fprintf */
#include <string.h> /* memcpy memset */
#include "colorconv.h" /* prototypes of yuv422rgb_scalar_float, yuv422rgb_scalar_fixed */

/* ************************************************************************* 


   NAME:  yuv422rgb_scalar_float


   USAGE: 

    
 
   char red; -- 0 to 255
   char green; -- 0 to 255
   char blue; -- 0 to 255
   int width, height; --  of image in pixels
   int byte_count; -- number of bytes in YUV image (2 * width * height) 
   const unsigned char * sourcep = malloc(byte_count); -- source yuv image

   unsigned char * destp = malloc(4 * width * height); -- dest RGBA image 

   init_imagedata(red, green, blue, yuv_imagep, byte_count);

   yuv422rgb_scalar_float(sourcep, source_byte_count, destp);

   returns: void

   DESCRIPTION:
                 this is a scalar (non-vector) function that converts
		 YUV to RGBA the same as the other functions. this
		 uses floating point math and is here as the baseline
		 for timing the other functions.
		 
		 modifies the memory pointed to by destp.
   REFERENCES:
  
   http://softpixel.com/~cwright/programming/colorspace/yuv/

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   REVISION HISTORY:

        STR                  Description of Revision                 Author

     15-Dec-09               initial coding                           gpk
     16-Jan-11  delta-Y code incorrect with clipping. compute         gpk
                both RGBA's explicitly.
		
 ************************************************************************* */

void yuv422rgb_scalar_float(const unsigned char * sourcep, int source_byte_count,
		    unsigned char * destp)
{
  unsigned char red0, green0, blue0, alpha;
  unsigned char red1, green1, blue1;
  int y0, y1, u, v;
  const unsigned char *source_endp;
  const unsigned char *vector_endp;
  int remainder;
  float red_partial, green_partial, blue_partial;
  
  alpha = 255;
  /* we're working with things in 4-byte macropixels  */
  remainder = source_byte_count % 4; 
  
  source_endp = sourcep + source_byte_count;
  vector_endp = source_endp - remainder;

  /* with each iteration through the loop we pull one macropixel  */
  /* (YUYV) from sourcep and increment sourcep to point to the    */
  /* next macropixel.  */
  
  while (sourcep < vector_endp)
    {
      y0 = *(sourcep++); /* Y  */
      u =  *(sourcep++) - 128; /* U  */
      y1 = *(sourcep++); /* Y  */
      v =  *(sourcep++) - 128; /* V  */

      red_partial =  1.4075 * (float)v;
      green_partial = -0.34455 * (float)u - 0.7169 * (float)v;
      blue_partial = 1.7790 * (float)u;
      {
	int redsum, greensum, bluesum;
	/* note: i'm truncating here instead of rounding so  */
	/* that will cause some error. if that's a problem,  */
	/* either call round before assigning the expression  */
	/* (and don't forget to link -lm) or add 0.5 and then */
	/* truncate. right now i'm more interested in speed   */
	/* so i'll accept the rounding error  */
	
	redsum =   (int)(y0 + red_partial);
	greensum = (int)(y0 + green_partial);
	bluesum =  (int)(y0 + blue_partial);

	if (255 < redsum)
	  {
	    redsum = 255;
	  }
	else if (0 > redsum)
	  {
	    redsum = 0;
	  }

	if (255 < greensum)
	  {
	    greensum = 255;
	  }
	else if (0 > greensum)
	  {
	    greensum = 0;
	  }

	if (255 < bluesum)
	  {
	    bluesum = 255;
	  }
	else if (0 > bluesum)
	  {
	    bluesum = 0;
	  }

	red0 =   (unsigned char)redsum;
	green0 = (unsigned char)greensum;
	blue0  = (unsigned char)bluesum;
      }
      

      {
	int redsum, greensum, bluesum;
	
	redsum = (int)(y1 + red_partial);
	greensum = (int)(y1 + green_partial);
	bluesum = (int)(y1 + blue_partial);

	if (255 < redsum)
	  {
	    redsum = 255;
	  }
	else if (0 > redsum)
	  {
	    redsum = 0;
	  }

	if (255 < greensum)
	  {
	    greensum = 255;
	  }
	else if (0 > greensum)
	  {
	    greensum = 0;
	  }

	if (255 < bluesum)
	  {
	    bluesum = 255;
	  }
	else if (0 > bluesum)
	  {
	    bluesum = 0;
	  }
	
	red1 = (unsigned char)redsum;
	green1 = (unsigned char)greensum;
	blue1 = (unsigned char)bluesum;

      }
      
      /* now store then where destp points  */
      *(destp++) = red0;
      *(destp++) = green0;
      *(destp++) = blue0;
      *(destp++) = alpha;
      *(destp++) = red1;
      *(destp++) = green1;
      *(destp++) = blue1;
      *(destp++) = alpha;
      
    }
  
}


/* #define SHIFT_AND_ADD */

/* ************************************************************************* 


   NAME:   yuv422rgb_scalar_fixed


   USAGE: 

   char red; -- 0 to 255
   char green; -- 0 to 255
   char blue; -- 0 to 255
   int width, height; --  of image in pixels
   int byte_count; -- number of bytes in YUV image (2 * width * height) 
   const unsigned char * sourcep = malloc(byte_count); -- source yuv image

   unsigned char * destp = malloc(4 * width * height); -- dest RGBA image 

   init_imagedata(red, green, blue, yuv_imagep, byte_count);
   
   yuv422rgb_scalar_fixed(sourcep, byte_count, destp);

   returns: void

   DESCRIPTION:
                 translate the YUV image of bute_count size
		 pointed to by sourcep into an  RGBA stored starting
		 at destp.

		 this is an integer-only function for comparison
		 to floating point scalar and vector functions.
		 (floating point is slower than integer math. how
		 much? how does this compare to the vector versions?)

		 modifies memory pointed to by destp.
   REFERENCES:

   this implements the integer-only formula found at:

    http://en.wikipedia.org/wiki/YUV#Converting_between_Y.27UV_and_RGB

    Integer operation of ITU-R standard for YCbCr(8 bits per channel):
    
    Cr = Cr - 128;  Cb = Cb - 128;
    
    R = Y + Cr +( Cr >> 2) + (Cr >> 3) + (Cr >> 5)
    
    G = Y - (Cb >> 2 + Cb >> 4 + Cb >> 5) 
          - (Cr >> 1 + Cr >> 3 + Cr >> 4 + Cr >> 5)
	  
    B = Y + Cb + (Cb >> 1) + (Cb >> 2) + (Cb >> 6)

    clamp RGB to  0 - 255.

   LIMITATIONS:

   * this is a different formula from the others; expect slightly 
     different results.

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   REVISION HISTORY:

        STR                  Description of Revision                 Author

     15-Dec-09               initial coding                           gpk
     16-Jan-11  delta-Y code incorrect with clipping. compute         gpk
                both RGBA's explicitly.
     28-Jan-11  save chrominance partial values and reuse for Y1      gpk
     20-Feb-11  change signs on green coefficients so we can add      gpk
                green partial.
		
 ************************************************************************* */

void  yuv422rgb_scalar_fixed(const unsigned char * sourcep, int source_byte_count,
		  unsigned char * destp)
{
  int y0, y1, u, v;
  const unsigned char *source_endp;
  const unsigned char *vector_endp;
  int remainder;
  int redpartial, greenpartial, bluepartial;
  int redsum, greensum, bluesum;
  unsigned char red0, green0, blue0;
  unsigned char red1, green1, blue1;
  const unsigned char alpha = 255;
  
  
  /* we're working with things in 4-byte macropixels  */
  remainder = source_byte_count % 4; 
  
  source_endp = sourcep + source_byte_count;
  vector_endp = source_endp - remainder;

  /* the while loop processes one macropixel (YUYV value) for each       */
  /* iteration. it produces two RGB values per iteration.                */
  
  /* i'm being a little bit crafty here in observing from the equations  */
  /* that second pixel's RGB value differs from the first only in the    */
  /* difference between the Y values (y0 and y1) for the two pixels.     */
  /* so instead of computing red/green/blue for the second pixel the     */
  /* same way as the first, compute the first, then add the delta Y's    */
  /* to its RGB values to get the second's RGB values.                   */

  /* i'm also trying to make it easier for the compiler to do any loop   */
  /* vectorization that it can   */
  while (sourcep < vector_endp)
    {
      y0 = *(sourcep++); /* Y  */
      u =  *(sourcep++) - 128; /* U Cb */
      y1 = *(sourcep++); /* Y  */
      v =  *(sourcep++) - 128; /* V Cr */

      /* compute the RGB of the first pixel from the given equations  */

#ifdef SHIFT_AND_ADD
      redpartial =  v + (v >> 2) + (v >> 3) + (v >> 5);
      greenpartial = ( (u >> 2) + (u >> 4) + (u >> 5))
	+ ((v >> 1) + (v >> 3) + (v >> 4) + (v >> 5));
      bluepartial =  u + (u >> 1) + (u >> 2) + (u >> 6);
#else
      redpartial =  ((90 * v) >> 6);
      greenpartial = (((-22 * u) +(-46 * v)) >> 6);
      bluepartial = ((113 * u) >> 6);
#endif /* SHIFT_AND_ADD  */
      
      redsum = y0 + redpartial;
      greensum = y0 + greenpartial;
      bluesum =  y0 + bluepartial;
      /* and clip them to the range of 0 to 255  */
      if (255 < redsum)
	{
	  redsum = 255;
	}
      else if (0 > redsum)
	{
	  redsum = 0;
	}

      if (255 < greensum)
	{
	  greensum = 255;
	}
      else if (0 > greensum)
	{
	  greensum = 0;
	}

      if (255 < bluesum)
	{
	  bluesum = 255;
	}
      else if (0 > bluesum)
	{
	  bluesum = 0;
	}

      /* now take the integer sums and turn them back into   */
      /* unsigned chars. the range of values is already checked  */
      /* so this should just be repacking the data  */
      
      red0 =   (unsigned char)redsum;
      green0 = (unsigned char)greensum;
      blue0  = (unsigned char)bluesum;
      

      /* now compute the RGB of the second pixel the same way    */
      redsum = y1 + redpartial;
      greensum = y1 + greenpartial;
      bluesum =  y1 + bluepartial;
	
       /* and clip them to the range of 0 to 255  */
      
      if (255 < redsum)
	{
	  redsum = 255;
	}
      else if (0 > redsum)
	{
	  redsum = 0;
	}

      if (255 < greensum)
	{
	  greensum = 255;
	}
      else if (0 > greensum)
	{
	  greensum = 0;
	}

      if (255 < bluesum)
	{
	  bluesum = 255;
	}
      else if (0 > bluesum)
	{
	  bluesum = 0;
	}

      /* now take the integer sums and turn them back into   */
      /* unsigned chars. the range of values is already checked  */
      /* so this should just be repacking the data  */
  
	
      red1 = (unsigned char)redsum;
      green1 = (unsigned char)greensum;
      blue1 = (unsigned char)bluesum;
     
      /* now store then where destp points  */
      *(destp++) = red0;
      *(destp++) = green0;
      *(destp++) = blue0;
      *(destp++) = alpha;
      *(destp++) = red1;
      *(destp++) = green1;
      *(destp++) = blue1;
      *(destp++) = alpha;
    } /* end while  */

}
