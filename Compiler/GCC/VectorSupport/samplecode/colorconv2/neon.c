/* ************************************************************************* 
* NAME: colorconv2/neon.c
*
* DESCRIPTION:
*
* this is the functions of the color conversion library that should be
* compiled for neon functionality:
*
* * yuv422rgb_neon_int
* * yuv422rgb_neon_float
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
*    if (0 != supported.neon)
*       you can use yuv422rgb_neon_int, yuv422rgb_neon_float
*    else
*       your processor doesn't support neon: try the contents of scalar.c
*    endif
* else
*    -- there was an error trying to find out processor features
* endif
*
* GLOBALS: none
*
* REFERENCES:
*
* LIMITATIONS:
*
* REVISION HISTORY:
*
*   STR                Description                          Author
*
*   21-Feb-10          initial coding                        gpk
*
* TARGET: GNU C version 4 or higher
*
* 
*
* ************************************************************************* */

#include <string.h> /* memcpy memset */

#include <arm_neon.h> /* neon intrinsics  */

#include "colorconv.h" /* prototypes for yuv422rgb_neon* functions */



/* MP_Y0 - the position of Y0 within a [YUYV] macropixel  */
#define MP_Y0 0x0

/* MP_U - the position of U within a [YUYV] macropixel  */
#define MP_U  0x1

/* MP_Y1 - the position of Y1 within a [YUYV] macropixel  */
#define MP_Y1 0x2

/* MP_V - the position of V within a [YUYV] macropixel  */
#define MP_V  0x3


/* MP0_Y0 - the location of macropixel 0's Y0 in a */
/* macropixel pair of YUYV YUYV */

#define MP0_Y0 MP_Y0

/* MP0_U - the location of macropixel 0's U in a */
/* macropixel pair of YUYV YUYV */

#define MP0_U MP_U


/* MP0_Y1 - the location of macropixel 0's Y1 in a */
/* macropixel pair of YUYV YUYV */
#define MP0_Y1 MP_Y1


/* MP0_V - the location of macropixel 0's V in a */
/* macropixel pair of YUYV YUYV */
#define MP0_V MP_V


/* MP1_Y0 - the location of macropixel 1's Y0 in a */
/* macropixel pair of YUYV YUYV */

#define MP1_Y0 0x4

/* MP1_U - the location of macropixel 1's U in a */
/* macropixel pair of YUYV YUYV */

#define MP1_U 0x5

/* MP1_Y1 - the location of macropixel 1's Y1 in a */
/* macropixel pair of YUYV YUYV */
#define MP1_Y1 0x6

/* MP1_V - the location of macropixel 1's V in a */
/* macropixel pair of YUYV YUYV */
#define MP1_V 0x7

/* ALPHA - the desired alpha level for the output  */
/* 0 - transparent, 255 opaque  */

#define ALPHA 0xff



/* ************************************************************************* 


   NAME:  yuv422rgb_neon_int


   USAGE: 

   int source_byte_count;
   char red; -- 0 to 255
   char green; -- 0 to 255
   char blue; -- 0 to 255
   int width, height; --  of image in pixels
   int byte_count; -- number of bytes in YUV image (2 * width * height) 
   const unsigned char * sourcep = malloc(byte_count); -- source yuv image

   unsigned char * destp = malloc(4 * width * height); -- dest RGBA image 

   init_imagedata(red, green, blue, yuv_imagep, byte_count);

   yuv422rgb_neon_int(sourcep, source_byte_count, destp);

   returns: void 

   DESCRIPTION:
                 
                 translate the YUV image starting at sourcep and having
		 byte_count bytes from YUYV into RGBA and store the results
		 starting at destp.

		 this uses the neon features for integers to solve the
		 following equation:

		 
		 R = clip(Y + 1.402  * (Cr - 128))
		   
		 G = clip(Y - 0.344  * (Cb - 128) - 0.714 *  (Cr - 128))
		   
		 B = clip(Y + 1.772  * (Cb - 128))

		 since this works on integers, approximate by multiplying
		 the equations by 64 (to get integer coefficients that
		 are about the same as the floating point ones), then 
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

		  turn the equations 90 degrees and do the sums vertically:

   1 * Y                     Y          Y           Y           255
   (u_coeff * U) / 64:	  ( 0 * U   -22 * U     113 * U        0 * U ) >> 6 
   (v_coeff * V) / 64:	  (90 * V   -46 * V       0 * V        0 * V ) >> 6 
 + -------------------     ------   -------     ------        ------
    result	             R         G           B           alpha 


    this lets me vectorize:

    * each multiplication is done in parallel with  vmulq_s16
    * each shift (the >> 6) is done in parallel with vshrq_n_s16
    * the vertical summations are done in parallel with vaddq_s16

   since i can work with 8 16-bit numbers at a time, i'll do two RGBA's
   at the same time, in the same int16x8_t vector. so each macro pixel
   (YUYV) in the input will get expanded into Y, U, and V vectors for
   for the inputs, vector for each row of coefficients (u_coeff, v_coeff)
   will repeat them twice, and the output vectors (rgba0, rgba1) will each
   contain two RGBA pixels.
   

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   vld1_u8 - load vector from memory
   vmovl_u8 - widen elements of vector 
   vreinterpretq_s16_u16 - reminterpret unsigned as signed
   vdup_lane_u8 - build a vector with an element from another vector
   vbsl_u8 - build a vector with bits from two others
   vset_lane_u8 - put a given value into a given element of a vector
   vget_low_s16 - get the low half of a vector
   vdupq_lane_s16 - put an element of a vector into all elements of a vector
   vsubq_s16 - subtract vectors of signed 16-bit elements
   vmulq_s16 - multiply vectors of signed 16-bit elements
   vaddq_s16 - add vectors of signed 16-bit elements
   vshrq_n_s16 - right shift signed 16-bit elements of a vector
   vget_high_s16 - get the high half of a vector
   vqmovun_s16 - saturating move and narrow a vector from signed to unsigned
   vcombine_u8 - combine 2 uint8x8_t's into a uint8x16_t
   vst1q_u8 - store vector in memory

   REVISION HISTORY:

        STR                  Description of Revision                 Author

      4-Sep-10               initial coding                           gpk
      4-Jan-11       added functions called to header block           gpk
     12-Mar-11   sum u & v then shift to get the same results as the  gpk
                 scalar code
     20-Mar-11   switch to vbsl_u8 instead of selecting all pixels    gpk
                 components as scalars; save vector operations
		 
 ************************************************************************* */

void  yuv422rgb_neon_int(const unsigned char * sourcep, int source_byte_count,
			 unsigned char * destp)
{
  const unsigned char *source_endp;
  const unsigned char *vector_endp;
  int remainder;
  const int16x8_t u_coeff = {0, -22, 113, 0, 0, -22, 113, 0};
  const int16x8_t v_coeff = {90, -46, 0,  0, 90, -46, 0,  0};
  const uint8x8_t zeroalpha = {0x0, 0x0, 0x0, 0xFF, 0x0, 0x0, 0x0, 0xFF};
  const int16x8_t uvbias = {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80}; 
  int16x8_t mp0_rgba;  /* macropixel 0's resulting RGBA RGBA pixels  */
  int16x8_t mp1_rgba; /* macropixel 1's resulting RGBA RGBA pixels  */
  uint8x8_t rawpixels; /* source pixels as {[YUYV]0 [YUYV]1}   */
  uint8x8_t rgba0, rgba1; /* rgba values as bytes  */
  uint8x16_t bothrgba;
  uint8_t * destinationp; /* pointer into output buffer destp  */
  int16x8_t widerpixels; /*  rawpixels promoted to shorts per component */
  const uint8x8_t yselect = {0xff, 0xff, 0xff, 0xff,
			     0x00, 0x00, 0x00, 0x00};
  
  
  /* we're working with things in 4-byte macropixels  */
  remainder = source_byte_count % 4;

  source_endp = sourcep + source_byte_count;
  vector_endp = source_endp - remainder;
  destinationp = (uint8_t *)destp;

  while (sourcep < vector_endp)
    {
     /* pull YUYV from 2 four byte macropixels starting at sourcep. */
      /* we'll increment sourcep as we go to save the array dereference */
      /* and separate increment instruction at the end of the loop  */

      /* load rawpixels with {[YUYV]0 [YUYV]1 } with byte components */
      rawpixels = vld1_u8(sourcep);
      sourcep += sizeof(rawpixels);

      widerpixels = vreinterpretq_s16_u16(vmovl_u8(rawpixels));
 


      
      /* ---------- process macropixel 0 --------------- */
      /* take macropixel zero ([YUYV]0) from rawpixels and   */
      /* compute the two RGBA pixels that come from it. store  */
      /* those two pixels in mp0_rgba  */
      {
	int16x8_t wider_yalpha;
	int16x8_t u_vec, v_vec, uv_vec;
	uint8x8_t narrow_yalpha;
	uint8x8_t y0_vec, y1_vec;
	int16x4_t yuyv;

	/* narrow_yalpha is drawn from [YUYV]0 and formed into */
	/* {Y0, Y0, Y0, alpha, Y1, Y1, Y1, alpha}   */
	/* this would have been a nice place for vtbx1_u8, but i  */
	/* can't get it to work. so i'll have to use vbsl_u8 instead.  */

	y0_vec = vdup_lane_u8(rawpixels, MP0_Y0);
	y1_vec = vdup_lane_u8(rawpixels, MP0_Y1);
	narrow_yalpha = vbsl_u8(yselect, y0_vec, y1_vec);

	/* store ALPHA in elements 3 and 7 (after the RGB components)  */
	narrow_yalpha =  vset_lane_u8(ALPHA, narrow_yalpha, 3);
	narrow_yalpha =  vset_lane_u8(ALPHA, narrow_yalpha, 7);

	/* use vmovl_u8 to go from being unsigned 8-bit to  */
	/* unsigned 16-bit, the use vreinterpretq_s16_u16 to  */
	/* change interpretation from unsigned 16-bit to signed  */
	/* 16-bit.   */
	wider_yalpha = vreinterpretq_s16_u16(vmovl_u8(narrow_yalpha));

	yuyv = vget_low_s16(widerpixels);
	
	/* form a vector of the U component from MP0  */
	u_vec = vdupq_lane_s16(yuyv, MP0_U);
	
	/* subtract uvbias from u_vec */
	u_vec = vsubq_s16(u_vec, uvbias);

	/* form a vector of the V component from MP0  */
	v_vec = vdupq_lane_s16(yuyv, MP0_V);
	
	/* subtract uvbias from v_vec */
	v_vec = vsubq_s16(v_vec, uvbias);

		
	/* Multiply eight 16-bit values in u_vec by eight 16-bit */
	/* values in u_coeff and store the results in u_vec.  */


	u_vec = vmulq_s16(u_vec, u_coeff);

	/* likewise multiply eight 16-bit values in v_vec by   */
	/* v_coeff and store the results in  v_vec */
	
	v_vec = vmulq_s16(v_vec, v_coeff);

	/* form uv_vec as the sum of u_vec & v_vec, then shift 6 places   */
	/* (dividing by 64)  */
	uv_vec = vaddq_s16(u_vec, v_vec);
	  
	uv_vec = vshrq_n_s16(uv_vec, 6);

	/* now mp0_rgba = y_vec + u_vec + v_vec  */
	mp0_rgba = vaddq_s16(wider_yalpha, uv_vec);

      }

      /* ---------- process macropixel 1 --------------- */
      /* take macropixel one ([YUYV]1) from rawpixels and   */
      /* compute the two RGBA pixels that come from it. store  */
      /* those two pixels in mp1_rgba  */      
      {
	int16x8_t wider_yalpha;
	int16x8_t u_vec, v_vec, uv_vec;
	uint8x8_t narrow_yalpha;
	uint8x8_t y0_vec, y1_vec;
	int16x4_t yuyv;

	/* narrow_yalpha is drawn from [YUYV]1 and formed into */
	/* {Y0, Y0, Y0, alpha, Y1, Y1, Y1, alpha}   */
	/* this would have been a nice place for vtbx1_u8, but i  */
	/* can't get it to work. so i'll have to use vbsl_u8 instead.  */

	y0_vec = vdup_lane_u8(rawpixels, MP1_Y0);
	y1_vec = vdup_lane_u8(rawpixels, MP1_Y1);
	narrow_yalpha = vbsl_u8(yselect, y0_vec, y1_vec);
	  
	narrow_yalpha =  vset_lane_u8(ALPHA, narrow_yalpha, 3);
	narrow_yalpha =  vset_lane_u8(ALPHA, narrow_yalpha, 7);

	/* use vmovl_u8 to go from being unsigned 8-bit to  */
	/* unsigned 16-bit, the use vreinterpretq_s16_u16 to  */


	wider_yalpha = vreinterpretq_s16_u16(vmovl_u8(narrow_yalpha));

	yuyv = vget_high_s16(widerpixels);
	u_vec = vdupq_lane_s16(yuyv, 1);
	u_vec = vsubq_s16(u_vec, uvbias);
	
	v_vec = vdupq_lane_s16(yuyv, 3);
	v_vec = vsubq_s16(v_vec, uvbias);

		
	/* Multiply eight 16-bit values in u_vec by eight 16-bit */
	/* values in u_coeff and store the results in u_vec.  */


	u_vec = vmulq_s16(u_vec, u_coeff);

	/* likewise multiply eight 16-bit values in v_vec by   */
	/* v_coeff and store the results in  v_vec */
	
	v_vec = vmulq_s16(v_vec, v_coeff);
     
	/* form uv_vec as the sum of u_vec & v_vec, then shift 6 places   */
	/* (dividing by 64)  */
	uv_vec  = vaddq_s16(u_vec, v_vec);
	uv_vec = vshrq_n_s16(uv_vec, 6);


	/* now mp1_rgba = y_vec + u_vec + v_vec  */
	mp1_rgba = vaddq_s16(wider_yalpha, uv_vec);
      }
      

      /* turn mp0_rgba from a vector of shorts to a vector of  */
      /* unsigned unsigned chars. this will saturate: clipping  */
      /* the values between 0 and 255.   */
      
      rgba0 = vqmovun_s16(mp0_rgba);
      rgba1 = vqmovun_s16(mp1_rgba);

      /* make it faster to copy these back out of vector registers into  */
      /* memory by combining rgba0 and rgba1 into the larger bothrgba.   */
      /* then store that back into memory at destinationp.               */

      bothrgba = vcombine_u8(rgba0, rgba1);
      
      vst1q_u8(destinationp, bothrgba);
      destinationp += 16;
      
      
    }
}



/* ************************************************************************* 


   NAME:  char_to_float_vectors


   USAGE: 

    
   const unsigned char * sourcep= {...};
   
   float32x4_t mp0;
   float32x4_t mp1;

   char_to_float_vectors(sourcep, &mp0, &mp1);

   returns: void

   DESCRIPTION:
                 take the first 8 bytes of yuyv (two macropixels)
		 from sourcep. subtract 128 from the U & V components.
		 store the results n mp0, mp1 as two vectors of floats

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   vld1_u8 - load vector from memory
   vmovl_u8 - widen elements of vector 
   vreinterpretq_s16_u16 - reminterpret unsigned as signed
   vsubq_s16 - subtract signed 16-bit vectors
   vget_low_s16 - get low half of a vector
   vget_high_s16  - get the high half of a vector
   vmovl_s16 - widen elements of vector 
   vcvtq_f32_s32 - convert from a signed int to a float

   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     20-Mar-11               initial coding                           gpk

 ************************************************************************* */

static inline void char_to_float_vectors(const unsigned char * sourcep,
			   float32x4_t *mp0, float32x4_t * mp1)
{
 uint8x8_t rawpixels; /* source pixels as {[YUYV]0 [YUYV]1}   */
 int16x8_t widerpixels; /*  rawpixels promoted to shorts per component */
 int16x4_t high16, low16;
 int32x4_t high32, low32;
 const  int16x8_t uvbias = {0, 128, 0, 128, 0, 128, 0, 128};
 
 rawpixels = vld1_u8(sourcep);
 widerpixels = vreinterpretq_s16_u16(vmovl_u8(rawpixels));

 /* subtract uvbias from widerpixels  */
 widerpixels = vsubq_s16(widerpixels, uvbias);

 /* now take widerpixels apart into (low16, high16) and   */
 /* then expand those into (low32, high32)    */
 low16 = vget_low_s16(widerpixels);
 high16 = vget_high_s16(widerpixels);
 high32 = vmovl_s16(high16);
 low32  = vmovl_s16(low16);

 /* now convert low32 and high32 into floats and store them in   */
 /*  *mp0,  *mp1 */

 *mp0 = vcvtq_f32_s32(low32);
 *mp1 = vcvtq_f32_s32(high32);
  
}


/* ************************************************************************* 


   NAME:  neon_make_rgb


   USAGE: 

    
   float32x4_t macropixel;
   float32x4_t rgba0;
   
   float32x4_t  rgba1;

   neon_make_rgb(macropixel, &rgba0, &rgba1);

   returns: void

   DESCRIPTION:
                 take the contents of the macropixel and turn it from
		 partly processed YUYV (129 has been subracted from the
		 U and V components) into two rgba vectors. store them
		 in rgba0, rgba1

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   vget_low_f32 - get the low pair of floats from a float32x4_t
   vget_high_f32 - get the high pair of floats from a float32x4_t
   vdupq_lane_f32 - copy a value from a vector, use it to make another vector
   vsetq_lane_f32 - set a particular element of a vector
   vmulq_f32 - multiply two floating point vectors
   vaddq_f32 - add two floating point vectors
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     27-Mar-11               initial coding                           gpk

 ************************************************************************* */

static inline void neon_make_rgb(float32x4_t macropixel, float32x4_t *rgba0p,
				 float32x4_t  *rgba1p)
{
  const float32x4_t  u_coeff = {0.0, -0.34455,  1.7790, 0.0 };
  const float32x4_t  v_coeff = {1.4075, -0.7169, 0.0, 0.0 };
  float32x4_t  y0_vec, y1_vec, u_vec, v_vec,  uv_vec;
  float32x2_t y0_u, y1_v;
  const float32_t alpha = 255.0;

  
  /* macropixel is [Y0, U, Y1, V].   */

  /* since vdupq_lane_f32 will only take two element vectors we */
  /* need to pick macropixel apart to build vectors of the components.  */
  /* so make y0_u be the first half of macropixel [Y0, U] and  */
  /* y1_v be the second half [Y1, V]. */
 
  y0_u =  vget_low_f32(macropixel);
  y1_v =  vget_high_f32(macropixel);

  /* now copy Y0 to all elements of y0_vec, then overwrite element 3  */
  /* with alpha.   */
  y0_vec = vdupq_lane_f32(y0_u, 0);
  y0_vec =  vsetq_lane_f32(alpha, y0_vec, 3);

  /* make u_vec be [U, U, U, U]. we'll do that using  */
  /* vdupq_lane_f32 and selecting U (element 1) from y0_u  */
  
  u_vec  = vdupq_lane_f32(y0_u, 1);

  /* now copy Y1 to all elements of y1_vec, then overwrite element 3  */
  /* with alpha.   */
  
  y1_vec  = vdupq_lane_f32(y1_v, 0);
  y1_vec =  vsetq_lane_f32(alpha, y1_vec, 3);

  /* make v_vec be [V, V, V, V]. we'll do that using  */
  /* vdupq_lane_f32 and selecting V (element 1) from y1_v  */
  
  v_vec = vdupq_lane_f32(y1_v, 1);

  /* now multiply u_vec * u_coeff and v_vec by v_coeff.  */
  u_vec =  vmulq_f32(u_vec, u_coeff);
  v_vec =  vmulq_f32(v_vec, v_coeff);

  /* add u_vec and v_vec to form uv_vec. use that to build  */
  /*  rgba0 and  rgba1 by adding y0_vec, y1_vec*/
  
  uv_vec = vaddq_f32(u_vec, v_vec);
  *rgba0p =  vaddq_f32(y0_vec, uv_vec);
  *rgba1p =  vaddq_f32(y1_vec, uv_vec);
  
  
}



/* ************************************************************************* 


   NAME:  condense_float_rgbas


   USAGE: 

   uint8x16_t rgba_bytes;
   float32x4_t rgba0;
   float32x4_t rgba1;
   float32x4_t rgba2;
   float32x4_t rgba3;

   rgba_bytes =  condense_float_rgbas(rgba0, rgba1, rgba2, rgba3);

   returns: uint8x16_t

   DESCRIPTION:
                 take the four floating point rgbas and condense them
		 down to char components. return the results.

		 this function is about turning floating point vectors
		 into integer vectors, then narrowing to shorts, and
		 finally to unsigned chars.

   REFERENCES:

   LIMITATIONS:

   if i understand the spec right, vcvtq_s32_f32 should round to nearest.
   it rounds towards zero (ie, truncates) instead. since this isn't the
   correct behavior, it might be fixed someday.
   
   GLOBAL VARIABLES:

      accessed:  none

      modified: none

   FUNCTIONS CALLED:
   
   vcvtq_s32_f32 - convert from float to int
   vqmovn_s32 - convert from int to short
   vcombine_s16 - combine two int16x4_t's into an int16x8_t
   vqmovun_s16 - convert int16x8_t to uint8x8_t
   vcombine_u8 - combine two uint8x8_t's into a uint8x16_t
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     27-Mar-11               initial coding                           gpk

 ************************************************************************* */

static inline uint8x16_t condense_float_rgbas(float32x4_t rgba0,
					      float32x4_t rgba1,  
					      float32x4_t rgba2,
					      float32x4_t rgba3)
{
  uint8x16_t retval = {0};  /* 16 bytes as 4 4-byte RGBAs  */
  int32x4_t i32pixels0, i32pixels1, i32pixels2, i32pixels3;
  int16x4_t i16pixels0, i16pixels1, i16pixels2, i16pixels3;
  int16x8_t i16pixels01, i16pixels23;
  uint8x8_t u8pixels0, u8pixels1;
  
  /* the choice of saturating conversions here will turn the elements  */
  /* of the rgbaN vectors into unsigned chars (0 - 255), so no max/min  */
  /* is required here.   */


  /* first float to int  */
  i32pixels0 = vcvtq_s32_f32(rgba0);
  i32pixels1 = vcvtq_s32_f32(rgba1);
  i32pixels2 = vcvtq_s32_f32(rgba2);
  i32pixels3 = vcvtq_s32_f32(rgba3);

  /* then int to short  */
  i16pixels0 = vqmovn_s32(i32pixels0);
  i16pixels1 = vqmovn_s32(i32pixels1);
  i16pixels2 = vqmovn_s32(i32pixels2);
  i16pixels3 = vqmovn_s32(i32pixels3);
  
  i16pixels01 = vcombine_s16(i16pixels0, i16pixels1);
  i16pixels23 = vcombine_s16(i16pixels2, i16pixels3);

  /* now short to unsigned int. saturation takes care of the boundary cases  */
  u8pixels0 = vqmovun_s16(i16pixels01);
  u8pixels1 = vqmovun_s16(i16pixels23);
  
  retval = vcombine_u8(u8pixels0, u8pixels1);

  return(retval);
}



/* ************************************************************************* 


   NAME:  yuv422rgb_neon_float


   USAGE: 


   char red; -- 0 to 255
   char green; -- 0 to 255
   char blue; -- 0 to 255
   int width, height; --  of image in pixels
   int byte_count; -- number of bytes in YUV image (2 * width * height) 
   const unsigned char * sourcep = malloc(byte_count); -- source yuv image
   unsigned char * destp = malloc(4 * width * height); -- dest RGBA image
   
   init_imagedata(red, green, blue, yuv_imagep, byte_count);
   
   yuv422rgb_neon_float(sourcep, byte_count, destp);

   returns: void

   DESCRIPTION:
   
                translate the YUV image starting at sourcep and having
		byte_count bytes from YUYV into RGBA and store the
	        results starting at destp.

		this uses the neon functions to vectorize the function:
		unlike yuv422rgb_neon_int, it uses floating point math.
		the equations we're using are:

		R = Y + 1.4075 * (V - 128)
		G = Y - 0.3455 * (U - 128) - (0.7169 * (V - 128))
		B = Y + 1.7790 * (U - 128)

		so turn the equations 90 degrees and do the sums
		vertically:

		              -----------------
			      
			      
    1 *  Y                    Y              Y            Y       255
    u_coeff * U       0.0   * U   -0.34455 * U  1.7790  * U   0.0 * U
    v_coeff * V      1.4075 * V   -0.7169  * V    0.0   * V   0.0 * V
   + ---------       ----------   ------------  -----------   -------
      result             R           G             B           alpha

      
                             -----------------

    i can use 4 32-bit values in the a vector, so i'll do that here.
    this means i can vectorize:

    * multiplications are done in parallel with vmulq_f32
    * additions are done with in parallel with vaddq_f32
    * the input is a macropixe [Y0 U Y1 V]. it generates two RGBA  pixels.
      the first using [Y0 U V] and the second using [Y1 U V]. note that
      the only difference between the two outputs is the difference
      between the Y0 and Y1 (U, V, and coefficients stay the same). so
      to generate the second RGBA from the the macropixel, i'll just add
      the difference (Y1 - Y0) to the RGB components of the first RGBA.
                 
   note that we have the option of a 16-bit float, but i'll stick with 32
   just for illustration.
   
   REFERENCES:
  
   http://softpixel.com/~cwright/programming/colorspace/yuv/
 
   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   char_to_float_vectors
   vst1q_u8 - store 16 unsigned bytes to memory
   neon_make_rgb
   condense_float_rgbas
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

      5-Sep-10               initial coding                           gpk
      4-Jan-11       added functions called to header block           gpk

      
 ************************************************************************* */
void yuv422rgb_neon_float(const unsigned char * sourcep, int source_byte_count,
			  unsigned char * destp)
{
  uint8x8_t rawpixels; /* source pixels as {[YUYV]0 [YUYV]1}   */
  uint8_t * destinationp; /* pixels  */
  float32x4_t  y0_vec, u_vec, v_vec, rgba0, rgba1, rgba2, rgba3;
  const unsigned char *source_endp;
  const unsigned char *vector_endp;
  int remainder;
  float32_t y0, u, y1, v, delta_y;
  float32_t yarray[4];
  const float32_t alpha = 255.0;
  uint8x8_t bytepixels;
  float32x4_t macropixel0,  macropixel1;
  uint8x16_t fourrgba;
  
  /* remainder = (source_byte_count % VECSIZE); */
  remainder = source_byte_count % 4; /* we're working with things in 4-byte  */
  
  source_endp = sourcep + source_byte_count;
  vector_endp = source_endp - remainder;
  destinationp = (uint8_t *)destp;

  /* with each iteration through the loop we pull one macropixel  */
  /* (YUYV) from sourcep and increment sourcep to point to the    */
  /* next macropixel.  */
  
  while (sourcep < vector_endp)
    {
      char_to_float_vectors(sourcep, &macropixel0,  &macropixel1);
      sourcep += 8; /* forward two macropixels  */
      neon_make_rgb(macropixel0, &rgba0, &rgba1);
      neon_make_rgb(macropixel1, &rgba2, &rgba3);
      fourrgba = condense_float_rgbas(rgba0, rgba1, rgba2, rgba3);
      vst1q_u8(destinationp, fourrgba);
      destinationp += sizeof(uint8x16_t);
    }

  /* if there are pixels between vector_endp and source_endp  */
  /* add something to process them here.  */
}
