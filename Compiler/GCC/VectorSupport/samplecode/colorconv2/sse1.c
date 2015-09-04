/* ************************************************************************* 
* NAME: colorconv/sse1.c
*
* DESCRIPTION:
*
* this is the functions of the color conversion library that should be
* compiled for SSE1 functionality:
*
* * yuv422rgb_mmx1
* * yuv422rgb_sse1
*
*
* PROCESS:
*
* #include "colorconv.h"
* 
* Cpusimdfeatures_t supported;
*
* some_int =  test_cpu_simd_features(&supported);
*
* if (0 == some_int)
*    -- supported structure now contains data on which features are supported
*    if (0 != supported.sse1)
*       -- you can use yuv422rgb_mmx1, yuv422rgb_sse1
*    else
*       -- your processor doesn't support SSE1: try the contents of scalar.c
*    endif
* else
*    -- there was an error trying to find out processor features
* endif
*
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
*    9-Jan-11  filled in intrinsics summaries in headers     gpk
*
*
* TARGET: GNU C version 4 or higher
*
* The contents of this file are in the public domain. If it breaks,
* you get to keep both pieces.
*
* ************************************************************************* */

#include <stdio.h> 
#include <string.h> /* memcpy memset */
#include <xmmintrin.h> /* SSE  __m128  float */
#include <mmintrin.h> /* MMX  __m64 int */


#include "colorconv.h" /* prototypes for yuv422rgb_mmx2, yuv422rgb_sse2  */

#include "cvt2ps.h" /*  _mm_cvt2ps_pu8 */

#include "extract.h" /* _mm_cvtpu16_ps1  */

/* ************************************************************************* 


   NAME:   yuv422rgb_mmx1


   USAGE: 

   char red; -- 0 to 255
   char green; -- 0 to 255
   char blue; -- 0 to 255
   int width, height; --  of image in pixels
   int byte_count; -- number of bytes in YUV image (2 * width * height) 
   const unsigned char * sourcep = malloc(byte_count); -- source yuv image

   unsigned char * destp = malloc(4 * width * height); -- dest RGBA image 

   init_imagedata(red, green, blue, yuv_imagep, byte_count);
   
   yuv422rgb_mmx1(sourcep, byte_count, destp);

   returns: void

   DESCRIPTION:
                 translate the YUV image starting at sourcep and having
		 byte_count bytes from YUYV into RGBA and store the results
		 starting at destp.

		 this uses the X86 MMX functions to vectorize the process.
		 because the MMX functions are limited to integer
		 arithmetic only, the equation solved are:

		 
		 R = clip(Y + 1.402  * (Cr - 128))
		   
		 G = clip(Y - 0.344  * (Cb - 128) - 0.714 *  (Cr - 128))
		   
		 B = clip(Y + 1.772  * (Cb - 128))
		   
                                                                     
		 since i can't do floats, approximate by multiplying the
		 equations by 64 (to get integer coefficients that are
		 about the same as the floating point ones), then 
		 shift right by 6 to scale back to the correct range.
		 so:
		   
		 Let: Cr = Cr - 128; Cb = Cb - 128;  

		 64R = (64 * Y) + 64 * 1.402 * Cr 
		 64R = (64 * Y) + 90 * Cr 
		 R = Y + (90 * Cr) >> 6  

		 64G = 64 * Y - 64 * (0.344 * Cb) - 64 * (0.714 * Cr)  
		 64G = 64 * Y - 22 * Cb - 46 * Cr  
		 G = Y - (22 * Cb) >> 6 - (46 * Cr) >> 6  

		 64B = 64 * Y + 64 *  1.772 * Cb  
		 64B = 64 * Y + 113 * Cb  
		 B = Y + (113 * Cb) >> 6  
	 
		 so we're working with:

		 
		  R = Y + (90 * Cr) >> 6
		  G = Y - (22 * Cb) >> 6 - (46 * Cr) >> 6
		  B = Y + (113 * Cb) >> 6  

		  at the level of SSE, MMX gives me registers that will
		  hold 4 16-bit integers (shorts). so turn the equations
		  90 degrees and do the sums vertically:

   1 * Y                     Y          Y           Y           255
   (u_coeff * U) / 64:	  ( 0 * U   -22 * U     113 * U        0 * U ) >> 6 
   (v_coeff * V) / 64:	  (90 * V   -46 * V       0 * V        0 * V ) >> 6 
 + -------------------     ------   -------     ------        ------
    result	             R         G           B           alpha 


    this lets me vectorize:

    * each multiplication is done in parallel with  _mm_mullo_pi16
    * each shift (the >> 6) is done in parallel (_mm_sra_pi16)
    * the vertical summations are done in parallel (_mm_add_pi16)

    so i have the results stored in a vector rgb0. do the same for rgba1.

    
    finally use _mm_packs_pu16 to combine rgba0, rgba1 into
    8 unsigned chars. the conversion will saturate at 0 and 255 so
    bounds checking isn't needed earlier on.
    
    
   REFERENCES:
   
   http://www.fourcc.org/fccyvrgb.php
   http://en.wikipedia.org/wiki/YUV#Converting_between_Y.27UV_and_RGB
   
   LIMITATIONS:

   assumes that the image width is a multiple of 4.

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   _mm_set_pi16 - set a vector's values
   _mm_mullo_pi16 - multiply vectors, extract low bytes of each element
   _mm_srai_pi16 - arithmetic shift right
   _mm_add_pi16 - add vectors
   _mm_packs_pu16 - saturating pack of 16-bit vectors into 8-bit vector
   _mm_empty - empty MMX registers, making it safe for floating point scalar
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     15-Dec-09               initial coding                           gpk
     16-Jan-11  compute rgba1 directly, not using delta_y; fix        gpk
                reference URL
     20-Feb-11  sum u_vec & v_vec before shifting to match order      gpk
                of operations in scalar.c code
		
 ************************************************************************* */

void  yuv422rgb_mmx1(const unsigned char *  __restrict__ sourcep,
		     int source_byte_count,
		     unsigned char * __restrict__ destp)
{
  const unsigned char *source_endp;
  const unsigned char *vector_endp;
  int remainder;
  const __m64 u_coeff = _mm_setr_pi16(0, -22, 113, 0);
  const __m64 v_coeff = _mm_setr_pi16(90, -46, 0,  0);
  __m64 y0_vec, y1_vec, u_vec, v_vec, uv_vec, rgba0, rgba1;
  short  y0, u, y1, v;
  const unsigned char alpha = 255;
  
  
  /* we're working with things in 4-byte macropixels  */
  remainder = source_byte_count % 4;

  source_endp = sourcep + source_byte_count;
  vector_endp = source_endp - remainder;

  while (sourcep < vector_endp)
    {
     /* pull YUYV from the four byte macropixel starting at sourcep. */
      /* we'll increment sourcep as we go to save the array dereference */
      /* and separate increment instruction at the end of the loop  */
      y0 = *(sourcep++); /* Y  */
      u =  *(sourcep++) - 128; /* U  */
      y1 = *(sourcep++); /* Y  */
      v =  *(sourcep++) - 128; /* V  */


      /* make y0_vec be a 4 element vector containing y0   */
      /* (the first luma we pulled from the macro pixel.) we'll */
      /* make three elements y0 (for r, g, b) and one be 255 for alpha.  */
      /* likewise y1_vec, u_vec, and v_vec will hold U, V  */
      /* values.   */
      y0_vec = _mm_set_pi16(alpha, y0, y0, y0);
      y1_vec = _mm_set_pi16(alpha, y1, y1, y1);
      
      u_vec = _mm_set_pi16(u, u, u, u);
      v_vec = _mm_set_pi16(v, v, v, v);

      /* multiply u_vec by u_coeff  */

      /* Multiply four 16-bit values in u_vec by four 16-bit */
      /* values in u_coeff and produce the low 16 bits of the results.  */
      /* since the values in u_vec are between -128 and +127 and  */
      /* the largest coefficient is 113, the results should fit in   */
      /* 16 bits.   */
      /* _mm_mullo_pi16  */
      u_vec =  _mm_mullo_pi16(u_vec, u_coeff);

      v_vec =  _mm_mullo_pi16(v_vec, v_coeff);
      
      /* now shift the contents of u_vec and v_vec right 6 places  */
      /* (effectively dividing by 64)   */
      /* u_vec = _mm_srai_pi16(u_vec, 6); */  /* _mm_srai_pi16  */
      /*  v_vec = _mm_srai_pi16(v_vec, 6); */  /* _mm_srai_pi16  */
      uv_vec = _mm_add_pi16(u_vec, v_vec);
      uv_vec  = _mm_srai_pi16(uv_vec, 6);

      /* rgba0 = y0_vec + u_vec + v_vec; doesn't generate the */
      /* mmx instruction directly: i'll use the intrinsic */
     
      rgba0 = _mm_add_pi16(y0_vec, uv_vec);

      /* likewise rgba1...  */
      rgba1 = _mm_add_pi16(y1_vec, uv_vec);

      
      /* now store the contents of rgba0, rgba0 back in memory...  */
      /* _mm_packs_pu16 combines and turns rgba0, rgba1  */
      /* into unsigned chars element-wise. values in rgba0, rgba1   */
      /* that fall outside the range of 0 to 255 will be clamped to  */
      /* that range. _mm_packs_pu16 */

      *((__m64 *)destp) = _mm_packs_pu16(rgba0, rgba1);
      destp += 8;
	
    }
  
  /* do this after MMX, before floating point operations  */
  /* this builtin doesn't appear in the info pages...     */
  _mm_empty ();
}


/* ************************************************************************* 


   NAME:  char_to_float_vectors


   USAGE: 

    
   const unsigned char * sourcep; -- pointer to YUYV data
   __m128 mp0; -- destination macropixel
   __m128 mp1; -- destination macropixel

   sourcep = ...;
   
   char_to_float_vectors(sourcep, &mp0, &mp1);

   returns: void

   DESCRIPTION:
                 this takes the YUYV data from sourcep, subtracts the U & V
		 biases, turns the results into floating point vectors and
		 store the results in mp0, mp1.

		 modifies mp0, mp1

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   memcpy
   _mm_unpacklo_pi8 - zip 2 char vectors into a short vector, return low half
   _mm_unpackhi_pi8 - zip 2 char vectors into a short vector, return high half
   _mm_sub_pi16 - subtract two vectors if signed shorts
   _mm_cvtp16_ps - convert floats to signed shorts 
   _mm_empty
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     30-Jan-11               initial coding                           gpk
      2-Feb-11  uvbias in wrong order: use _mm_setr_pi16              gpk
      
 ************************************************************************* */

static void char_to_float_vectors(const unsigned char * sourcep, __m128 *mp0,
			   __m128 * mp1)
{
  __m64 inputvector; /* 8 unsigned chars is 2 YUYV macropixels  */
  const __m64 zero = _mm_setzero_si64();
  const __m64 uvbias = _mm_setr_pi16(0, 128, 0, 128);
  __m64 loshorts, hishorts; /* 8 shorts each  */

  memcpy(&inputvector, sourcep, sizeof(inputvector));

  /* interleave zero with narrow and return halves: essentially widening  */
  /* elements from chars to unsigned shorts                      */
  loshorts = _mm_unpacklo_pi8(inputvector, zero);
  hishorts = _mm_unpackhi_pi8(inputvector, zero);

  /* now subtract 128 from the U & V components by subtracting uvbias  */
  /* from the YUYV data in loshorts and hishorts.   */
  
  loshorts = _mm_sub_pi16(loshorts, uvbias);
  hishorts = _mm_sub_pi16(hishorts, uvbias);

  /* turn loshorts from a vector of 4 shorts into a vector of 4 floats  */
  /* and store it into *mp0. likewise convert hishorts and store into   */
  /* *mp1.   */

  /* note: if bug #? is present, use _ps1.   */
  *mp0 = _mm_cvtpi16_ps1(loshorts);
  *mp1 = _mm_cvtpi16_ps1(hishorts);
  _mm_empty();
  
}

/* Y0_INDEX - the shuffle index of Y0 in YUYV  */
#define Y0_INDEX 0

/* U_INDEX - the shuffle index of U in YUYV  */
#define U_INDEX  1

/* Y1_INDEX - the shuffle index of Y1 in YUYV  */

#define Y1_INDEX 2

/* V_INDEX - the shuffle index of V in YUYV  */
#define V_INDEX 3





/* ************************************************************************* 


   NAME:  sse_make_rgb


   USAGE: 

    
   __m128 macropixel; -- floating point YUYV macropixel
   __m128 rgba0; -- the first float RGBA generated from macropixel
   __m128 rgba1; -- the second float RGBA generated from macropixel

   sse_make_rgb(macropixel, &rgba0, &rgba1);

   returns: void

   DESCRIPTION:
                 take the YUYV macropixel and generate two RGBA
		 pixels from that. store them in rgba0, rgba1.

		 this uses the SSE (1) functions to vectorize the
		 function. this does the same sort of "vertical math" as the
		 yuv422rgb_mmx does: only with floating point numbers
		 instead of integer values. we're using:

		 R = Y + 1.4075 * (V - 128)
		 G = Y - 0.3455 * (U - 128) - (0.7169 * (V - 128))
		 B = Y + 1.7790 * (U - 128)

		 SSE (1) gives me a vector that can hold 4 single
		 precision floats. (within precision that i need here.)

		 so turn the equations 90 degrees and do the sums
		 vertically:
 
    1 *  Y                    Y              Y            Y       255
    u_coeff * U       0.0   * U   -0.34455 * U  1.7790  * U   0.0 * U
    v_coeff * V      1.4075 * V   -0.7169  * V    0.0   * V   0.0 * V
   + ---------       ----------   ------------  -----------   -------
      result             R           G             B           alpha


   this lets me vectorize:

   * each multiplication is done in parallel with (gcc emits the
     SSE MULPS instruction all by itself on X86)
     
   * the vertical summations are done in parallel with _mm_add_ps

   then i have to bounds check and clip the results to range between
    0 and 255 inclusive. i can vectorize that comparison by:

 
    first looking for the element-wise maximum between
          {R, G, B, A} and {0, 0, 0, 255}.
	  (using the _mm_max_ps intrinsic)

    then looking for the element-wise minimum between
          {R, G, B, A} and {255, 255, 255, 255}.
	  (using the _mm_min_ps intrinsic)
		 modifies  rgba0, rgba1.

   note that since the macropixel has two Y values, we'll get two
   RGBA's from it. since the only difference is which Y we use (Y0 or
   Y1), i'll store the U & V stuff in uv_vec, then add Y0 for rgba0, and
   Y1 for rgba1.
   
   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   _mm_shuffle_ps - rearrange a vector's components into a new vector
   _mm_mul_ps - multiply floating point vectors
   _mm_add_ps - add floating point vectors
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     27-Feb-11               initial coding                           gpk

 ************************************************************************* */

static void sse_make_rgb(__m128 macropixel, __m128 *rgba0p, __m128 *rgba1p)
{
  const __m128 u_coeff = {0.0, -0.34455,  1.7790, 0.0 };
  const __m128 v_coeff = {1.4075, -0.7169, 0.0, 0.0 };
  __m128 y0_vec, y1_vec, u_vec, v_vec,  uv_vec;

  /* macropixel is [Y0, U, Y1, V].   */
  
  /* make y0_vec [Y0,Y0,Y0, Y0]. */
 
  y0_vec = _mm_shuffle_ps(macropixel, macropixel,
			  _MM_SHUFFLE(Y0_INDEX, Y0_INDEX,
				      Y0_INDEX, Y0_INDEX));

  /* make y1_vec [Y1, Y1, Y1, Y1]  */
  y1_vec = _mm_shuffle_ps(macropixel, macropixel,
			  _MM_SHUFFLE(Y1_INDEX, Y1_INDEX,
				      Y1_INDEX, Y1_INDEX));
  /* likewise, make u_vec only U  */
  u_vec =  _mm_shuffle_ps(macropixel, macropixel,
			  _MM_SHUFFLE(U_INDEX,  U_INDEX,
				      U_INDEX,  U_INDEX));
  
  /* and make v_vec only V  */
  v_vec =  _mm_shuffle_ps(macropixel, macropixel,
			  _MM_SHUFFLE(V_INDEX,  V_INDEX,
				      V_INDEX,  V_INDEX));

  /* multiply U by U coefficients and V by V coefficients  */
  u_vec = _mm_mul_ps(u_vec, u_coeff);
  v_vec = _mm_mul_ps(v_vec,  v_coeff);

  /* add products into uv_vec  */
  uv_vec = _mm_add_ps(u_vec, v_vec);
  
  /*  rgba0 = y0_vec + u_vec + v_vec; */
  *rgba0p = _mm_add_ps(y0_vec, uv_vec);

  /*  rgba1 = y1_vec + u_vec + v_vec; */
  *rgba1p = _mm_add_ps(y1_vec, uv_vec);

  /* note: alpha is not set  */

}


/* ************************************************************************* 


   NAME:  condense_float_rgbas


   USAGE: 

   __m128 fourrgbas; -- byte components
   __m128 rgba0; -- float components
   __m128 rgba1; -- float components
   __m128 rgba2; -- float components   
   __m128 rgba3; -- float components

   fourrgbas = condense_float_rgbas(rgba0, rgba1, rgba2, rgba3);

   returns:  __m128

   DESCRIPTION:
                 condense 4 vectors of RGBA (with float components) into
		 a vector of 4 RGBAs with byte components

		 to do this we'll first trim each rgbaN to be
		 between the maxvector and minvector. (since we
		 saturate when we store using _mm_packs_pu16,
		 the only thing this step does is to force the
		 alpha component to 255.)

		 next, round the rgbaN's to shorts vectors (that is,
		 fixed point 16 bit) using _mm_cvtps_pi16

		 next, turn the 4 short vectors into 2 64-bit char vectors
		 
		 finally, take append the two 64-bit char vectors into a
		 128-bit char vector and return that.

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   _mm_max_ps - build component-wise max of two vectors
   _mm_min_ps - build component-wise min of two vectors
   _mm_cvtps_pi16 - convert 4-float-vector  to 4-short-vector
   _mm_packs_pu16 - pack 2 short vectors into halves of a char vector
   _mm_loadl_pi - set lower half of an __m128
   _mm_loadh_pi - set upper half of an __m128
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     27-Feb-11               initial coding                           gpk

 ************************************************************************* */

static __m128 condense_float_rgbas(__m128 rgba0, __m128 rgba1, __m128 rgba2,
				   __m128 rgba3)
{
  __m128 retval = {0};  /* 16 bytes as 4 4-byte RGBAs  */
  const __m128 minvector= {0.0, 0.0, 0.0, 255.0}; 
  const __m128 maxvector= {255.0, 255.0, 255.0, 255.0};
  __m64 shortvec0, shortvec1, shortvec2, shortvec3;
  __m64 charvec0, charvec1;

 

  /* store rgbaN into shortvecN  */


#if 1
  rgba0 = _mm_max_ps(rgba0, minvector); /* trim to min boundary  */
  rgba0 = _mm_min_ps(rgba0, maxvector); /* trim to max boundary  */

  rgba1 = _mm_max_ps(rgba1, minvector); /* trim to min boundary  */
  rgba1 = _mm_min_ps(rgba1, maxvector); /* trim to max boundary  */

  rgba2 = _mm_max_ps(rgba2, minvector); /* trim to min boundary  */
  rgba2 = _mm_min_ps(rgba2, maxvector); /* trim to max boundary  */

  rgba3 = _mm_max_ps(rgba3, minvector); /* trim to min boundary  */
  rgba3 = _mm_min_ps(rgba3, maxvector); /* trim to max boundary  */

  
#else
  {
    const __m128 zeroalpha = {0.0, 0.0, 0.0, 255.0}; 
  /*  condense_float_rgbas will fit the results into 0 - 255. i */
  /* only need to force the alpha element to be 255.  */
  /* OR doesn't do that. need a better way.  */
  rgba0 = _mm_or_ps (rgba0, zeroalpha); 
  rgba1 = _mm_or_ps (rgba1, zeroalpha); 
  rgba2 = _mm_or_ps (rgba2, zeroalpha); 
  rgba3 = _mm_or_ps (rgba3, zeroalpha);
  }

#endif /* 0  */

  /* uses rounding mode set in yuv422rgb_sse1  */
  shortvec0 = _mm_cvtps_pi16(rgba0);
  shortvec1 = _mm_cvtps_pi16(rgba1);
  shortvec2 = _mm_cvtps_pi16(rgba2);
  shortvec3 = _mm_cvtps_pi16(rgba3);

  
  charvec0 = _mm_packs_pu16(shortvec0, shortvec1);
  charvec1 = _mm_packs_pu16(shortvec2, shortvec3);

  retval =  _mm_loadl_pi(retval, &charvec0);
  retval =  _mm_loadh_pi(retval, &charvec1);


  
  return(retval);
}

/* ************************************************************************* 


   NAME:  yuv422rgb_sse1


   USAGE: 

  
   char red; -- 0 to 255
   char green; -- 0 to 255
   char blue; -- 0 to 255
   int width, height; --  of image in pixels
   int byte_count; -- number of bytes in YUV image (2 * width * height) 
   const unsigned char * sourcep = malloc(byte_count); -- source yuv image

   unsigned char * destp = malloc(4 * width * height); -- dest RGBA image 

   init_imagedata(red, green, blue, yuv_imagep, byte_count);
  
   yuv422rgb_sse1(sourcep, source_byte_count, destp);

   returns: void

   DESCRIPTION:
                 translate the YUV image starting at sourcep and having
		 byte_count bytes from YUYV into RGBA and store the
		 results starting at destp.




   
 
    
   that takes care of the first RGBA pixel of the macropixel. for the
    second one, observe that the second RGBA is the first RGBA plus
    the difference between the second luminance and the first. so
    add delta_y onto the first RGBA to get the second. then bounds
    check again to make sure nothing was pushed out of bounds.

    finally, unpack the two RGBA vectors into an array of 8 floats.
    then copy the values from the array into destp.
      
   REFERENCES:

   
   http://softpixel.com/~cwright/programming/colorspace/yuv/
   
   LIMITATIONS:

   the values of u_coeff and v_coeff are from a different source than
   the other functions, so the results will be slightly different.
   
   assumes that the width of the image is a multiple of 4
   
   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   _MM_GET_ROUNDING_MODE
   _MM_SET_ROUNDING_MODE
   char_to_float_vectors
   sse_make_rgb
   condense_float_rgbas
   memcpy
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     15-Dec-09               initial coding                           gpk
     16-Jan-11  compute rgba1 directly, not using delta_y; fix        gpk
                reference URL
     29-Jan-11  use aligned stores to eightfloats                     gpk
     13-Feb-11  code cleanup                                          gpk
     
 ************************************************************************* */


void yuv422rgb_sse1(const unsigned char * __restrict__ sourcep,
		    int source_byte_count,
		    unsigned char * __restrict__ destp)
{
  __m128 rgba0, rgba1, rgba2, rgba3;
  __m128 fourrgba; /* 4x 4-byte RGBAs  */
  const unsigned char *source_endp;
  const unsigned char *vector_endp;
  int remainder;
  __m64 * destinationp; /* __m128i  */
  __m128 mp0, mp1;
  unsigned int initial_rounding_mode;
  
  /* remainder = (source_byte_count % VECSIZE); */
  remainder = source_byte_count % 4; /* we're working with things in 4-byte  */
  
  source_endp = sourcep + source_byte_count;
  vector_endp = source_endp - remainder;
  destinationp = (__m64 *)destp;
  
  /* with each iteration through the loop we pull one macropixel  */
  /* (YUYV) from sourcep and increment sourcep to point to the    */
  /* next macropixel.  */
  initial_rounding_mode = _MM_GET_ROUNDING_MODE();

  /* because the scalar code uses a cast to convert from float to int,  */
  /* i have to set the rounding mode to _MM_ROUND_TOWARD_ZERO so i get  */
  /* the same behavior when condense_float_rgbas calls _mm_cvtps_pi16.  */
  /* i'm doing setting rounding mode here (outside the loop) to save    */
  /* operations (ie don't set and reset the rounding mode for each pair */
  /* of macropixels: only do it once.   */
  
  _MM_SET_ROUNDING_MODE(_MM_ROUND_TOWARD_ZERO); 
  while (sourcep < vector_endp)
    {

      char_to_float_vectors(sourcep, &mp0, &mp1);
      sourcep += 8;
      
      sse_make_rgb(mp0, &rgba0, &rgba1);
      sse_make_rgb(mp1, &rgba2, &rgba3);
      fourrgba = condense_float_rgbas(rgba0, rgba1, rgba2, rgba3);
      memcpy(destinationp, &fourrgba, sizeof(fourrgba));
      destinationp += 2; /* two sets of 8 chars  */
    } /* end while  */
  _MM_SET_ROUNDING_MODE(initial_rounding_mode);
  
  /* start here: add something to process the pixels between   */
  /* vector_endp and source_endp  */
 
}

