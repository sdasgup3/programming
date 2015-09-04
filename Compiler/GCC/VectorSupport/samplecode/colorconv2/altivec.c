/* ************************************************************************* 
* NAME: colorconv2/altivec.c
*
* DESCRIPTION:
*
* these are the functions of the color conversion library that should be
* compiled for altivec functionality:
*
* * yuv422rgb_avc_int (fixed point math)
* * yuv422rgb_avc_float (floating point math)
*
* we presume the integer implementation will be faster than the floating
* point, but floating point will be more precise and easier to understand,
* so here's both so you can see the differences in speed and
* precision.
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
*    if (0 != supported.altivec)
*       you can use yuv422rgb_avc_int, yuv422rgb_avc_float
*    else
*       your processor doesn't support altivec: try the contents of scalar.c
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
*    2-Oct-10          initial coding                        gpk
*   12-Nov-10  sped up yuv422rgb_avc_int by taking raw data  gpk
*              int vector and use vec_perm, vec_splat to
*              build other vectors from it.
*  24-Nov-10   pull the loop body out of yuv422rgb_avc_float gpk
*              to functions i can inline and vectorize
*              better. 
*  14-Jan-11   changed char_to_vectors to faster version     gpk
*  13-Feb-11   modified avc_make_rgb so vec_madd would round kaj
*              properly to be truncated later by vec_cts;
*              moved uvbias subtraction from char_to_vectors
*              to avc_make_rgb 
*  23-Feb-11   modified yuv422rgb_avc_int perm vectors       kaj
*
* TARGET: GNU C version 4 or higher
*
* 
*
* ************************************************************************* */

#include <stdio.h>
#include <string.h> /* memcpy memset */

#include <altivec.h>

#include "printValues.h" /* for print function protytpes */
#include "colorconv.h"  /* yuv422rgb_avc_int, yuv422rgb_avc_float */


/* FLOAT_ARRAYSIZE - the number of elements in a float array that  */
/* make it the same size as a float vector                         */
#define FLOAT_ARRAYSIZE (sizeof(vector float) / sizeof(float))


/* SHORT_ARRAYSIZE - the number of elements in a short array that  */
/* make it the same size as a short vector                         */
#define SHORT_ARRAYSIZE (sizeof(vector short) / sizeof(short))


/* INT_ARRAYSIZE - the number of elements needed in an int array  */
/* to hold the contents of an int vector   */
#define INT_ARRAYSIZE (sizeof(vector int) / sizeof(int))


/* CHAR_ARRAYSIZE - the number of chars in an array it would take */
/* to make it the same size as a char vector                      */
#define CHAR_ARRAYSIZE (sizeof (vector char) / sizeof (char))


/* ************************************************************************* 


   NAME:  build8short


   USAGE: 

   vector short avector;
   short val0;
   short val1;
   short val2;
   short val3;
   short val4;
   short val5;
   short val6;
   short val7;

   avector =  build8short(val0, val1, val2, val3, val4, val5, val6, val7);

   returns: vector short

   DESCRIPTION:
                 build a vector with the values of 8 shorts
		 
		 inline this for best performance
   REFERENCES:

   LIMITATIONS:

   unused in this file
   this will be slower than other methods

   FUNCTIONS CALLED:
   
   vec_ld - load a vector from memory
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     17-Oct-10               initial coding                           gpk

 ************************************************************************* */

inline static vector short build8short(short val0, short val1, short val2,
				       short val3, short val4, short val5, 
				       short val6, short val7)
{
  short shortarray[SHORT_ARRAYSIZE] __attribute__ ((aligned (16)));
  vector short retval;

  shortarray[0] = val0;  shortarray[1] = val1;
  shortarray[2] = val2;  shortarray[3] = val3;
  shortarray[4] = val4;  shortarray[5] = val5;
  shortarray[6] = val6;  shortarray[7] = val7;

  retval = vec_ld(0, shortarray);

  return(retval);
}




/* ************************************************************************* 


   NAME:  build1short


   USAGE: 

   static vector short avector
   short value;

   avector =  build1short(value);

   returns: vector short

   DESCRIPTION:
                 build a vector of 8 shorts, all having the
		 given value.

		 inline this for best performance
   REFERENCES:

   LIMITATIONS:
   
   unused in this file
   this will be slower than other methods
   
   FUNCTIONS CALLED:
   
   vec_ld - load a vector from memory
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     17-Oct-10               initial coding                           gpk

 ************************************************************************* */

inline static vector short build1short(short value)
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

/* Permuter constants */
/* in  yuv422rgb_avc_int() we take YUV data from memory as bytes arranged */
/* as {[YUYV]0 [YUYV]1 [YUYV]2 [YUYV]3}. it's faster to load that raw */
/* data into the vectors once, then distribute it to other vectors than */
/* to load from memory repeatedly. we start with the raw data as    */
/* {[YUYV]0 [YUYV]1 [YUYV]2 [YUYV]3} (components as bytes) then expand  */
/* the components into shorts and store the result in two vectors  */
/* arranged as  {[YUYV]0 [YUYV]1}  {[YUYV]2 [YUYV]3}. these are used */
/* to build the other vectors needed for the conversion. so here are */
/* symbolic constants that we can use for the vector permute intrinsic. */
/* MP0_* constants describe the placement of bytes for the first macro  */
/* pixel ([YUYV]0) in {[YUYV]0 [YUYV]1}.   MP1_* constants describe the  */
/* bytes within the second macro pixel ([YUYV]1) in {[YUYV]0 [YUYV]1}.  */
/* MP0_Y00 & MP0_Y01 index to the two bytes that make up the first Y  */
/* MP0_U0 & MP0_U1 index to the two bytes that make up the first U  */
/* MP0_Y10 & MP0_Y11 index to the two bytes that make up the second Y  */
/* MP0_V0 & MP0_V1 index to the two bytes that make up the first V  */

#define MP0_Y00 0x0
#define MP0_Y01 0x1
#define MP0_U0 0x2
#define MP0_U1 0x3
#define MP0_Y10 0x4
#define MP0_Y11 0x5
#define MP0_V0 0x6
#define MP0_V1 0x7

#define MP1_Y00 0x8
#define MP1_Y01 0x9
#define MP1_U0 0xA
#define MP1_U1 0xB
#define MP1_Y10 0xC
#define MP1_Y11 0xD
#define MP1_V0 0xE
#define MP1_V1 0xF

/* ZALPHA_ALPHA0, ZALPHA_ALPHA1 index two bytes that store alpha in zeroalpha */
#define ZALPHA_ALPHA0 0x18
#define ZALPHA_ALPHA1 0x19

/* MP0_U_ELT is the element of hitwopixels that stores the first U  */
/* in [YUYV]n */

#define MP0_U_ELT 0x1

/* MP0_V_ELT is the element of hitwopixels that stores the first V  */
/* in [YUYV]n */

#define MP0_V_ELT 0x3

/* MP1_U_ELT is the element of hitwopixels that stores the second U  */
/* in [YUYV]n */

#define MP1_U_ELT 0x5

/* MP1_V_ELT is the element of hitwopixels that stores the second V  */
/* in [YUYV]n */

#define MP1_V_ELT 0x7

/* ************************************************************************* 


   NAME:  yuv422rgb_avc_int


   USAGE: 


   char red; -- 0 to 255
   char green; -- 0 to 255
   char blue; -- 0 to 255
   int width, height; --  of image in pixels
   int byte_count; -- number of bytes in YUV image (2 * width * height) 
   const unsigned char * sourcep = malloc(byte_count); -- source yuv image

   unsigned char * destp = malloc(4 * width * height); -- dest RGBA image
   
   init_imagedata(red, green, blue, yuv_imagep, byte_count);

   yuv422rgb_avc_int(sourcep, byte_count, destp);

   returns: void

   DESCRIPTION:
                 
                 translate the YUV image starting at sourcep and having
		 byte_count bytes from YUYV into RGBA and store the results
		 starting at destp.

		 this uses the altivec features for integers to solve the
		 following equation:

		 
		 R = clip(Y + 1.402  * (Cr - 128))
		   
		 G = clip(Y - 0.344  * (Cb - 128) - 0.714 *  (Cr - 128))
		   
		 B = clip(Y + 1.772  * (Cb - 128))

		 since this function works on integers, approximate by
		 multiplying the equations by 64 (to get integer coefficients
		 that are about the same as the floating point ones), then 
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

    * each multiplication is done in parallel with  vec_mladd
    * each shift (the >> 6) is done in parallel with vec_sra
    * the vertical summations are done in parallel with vec_adds

    since i can work with 8 16-bit numbers at a time, i'll do two RGBA's
   at the same time, in the same vector short. so each macro pixel
   (YUYV) in the input will get expanded into Y, U, and V vectors for
   for the inputs, vector for each row of coefficients (u_coeff, v_coeff)
   will repeat them twice, and the output vectors (rgba0, rgba1) will each
   contain two RGBA pixels.
               

   Note: since it's faster to load data into the vector processor from
   memory once and distrbute the data between the vector registers than
   loading from memory several times, this function uses vec_perm to
   load four pixels of data _once_ per loop, than parsels the data out
   from that single initial load.


   REFERENCES:

   LIMITATIONS:
 
   assumes that the width of the image is a multiple of 4
 
   FUNCTIONS CALLED:
   
   vec_ld - load a vector from memory
   vec_mergeh - interleave two vectors together, return the upper half
   vec_mergel - interleave two vectors together, return the lower half
   vec_perm - form a new vector by selecting elements of other vectors
   vec_sub - subtract two vectors and return the result
   vec_splat - build a vector with all elements same value
   vec_mladd - combined multiply & add
   vec_splat_s16 - form a short vector with all elements the given value
   vec_sra - arithmetic shift right
   vec_adds - saturated add 
   vec_packsu - pack two vectors of wider type into narrower type, saturating
   vec_st - store vector to memory
  

   REVISION HISTORY:

        STR                  Description of Revision                 Author

     17-Oct-10               initial coding                           gpk
      2-Nov-10  fix additions to build mp0_rgba, mp1_rgba             kaj
     12-Nov-10  switch from forming arrays in memory to pulling in    gpk
                the raw data and building the vectors with
		vec_mergeh, vec_mergel, vec_perm, and vec_splat.
		replace a constant vector of zero's with calls to
		vec_splat_*(0); which Ollmann's tutorial says is
		faster.
     16-Nov-10  added comments                                        gpk
     23-Feb-11  modified perm vectors, fix algebra error in matching  kaj
                scalar code: add u_vec and v_vec, then shift the sum
		
 ************************************************************************* */

void yuv422rgb_avc_int(const unsigned char * sourcep, int source_byte_count,
			 unsigned char * destp)
{
  const unsigned char *source_endp;
  const unsigned char *vector_endp;
  int remainder; 
  const vector short u_coeff = {0, -22, 113, 0, 0, -22, 113, 0};
  const vector short v_coeff = {90, -46, 0,  0, 90, -46, 0,  0};
  const vector unsigned short shiftvec = {6, 6, 6, 6, 6, 6, 6, 6}; 
  const vector unsigned short zeroalpha = {0, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF};
  const vector unsigned char y0perm = {MP0_Y00, MP0_Y01, MP0_Y00, MP0_Y01,
				       MP0_Y00, MP0_Y01, ZALPHA_ALPHA0,
				       ZALPHA_ALPHA1, MP0_Y10, MP0_Y11, MP0_Y10,
				       MP0_Y11, MP0_Y10, MP0_Y11,
				       ZALPHA_ALPHA0, ZALPHA_ALPHA1};

  const vector unsigned char y1perm = {MP1_Y00, MP1_Y01, MP1_Y00, MP1_Y01,
				       MP1_Y00, MP1_Y01, ZALPHA_ALPHA0,
				       ZALPHA_ALPHA1, MP1_Y10, MP1_Y11, MP1_Y10,
				       MP1_Y11, MP1_Y10, MP1_Y11,
				       ZALPHA_ALPHA0, ZALPHA_ALPHA1};


  const vector signed short uvbias = {0x80, 0x80, 0x80, 0x80,
				      0x80, 0x80, 0x80, 0x80};
  
  /* start here: make sure alpha correct */

  /* const vector unsigned short shiftvec = vec_splat_u16(6); */
  vector unsigned char fourpixelvec; 
  vector unsigned short hitwopixels;
  vector unsigned short lotwopixels; 
  vector short mp0_y_vec, mp0_u_vec, mp0_v_vec, mp0_rgba;  /* macropixel 0  */
  vector short mp1_y_vec, mp1_u_vec, mp1_v_vec, mp1_rgba; /* macropixel 1  */
  vector short mp2_y_vec, mp2_u_vec, mp2_v_vec, mp2_rgba;  /* macropixel 2  */
  vector short mp3_y_vec, mp3_u_vec, mp3_v_vec, mp3_rgba; /* macropixel 3  */
  vector short mp0_uv_vec,  mp1_uv_vec,  mp2_uv_vec,  mp3_uv_vec;
  vector unsigned char destinationvec0; /* pointer into output buffer destp  */
  vector unsigned char destinationvec1; /* pointer into output buffer destp  */
  
  unsigned char * destinationp;
  
 /* we're working with things in 4-byte macropixels  */
  remainder = source_byte_count % 4;

  source_endp = sourcep + source_byte_count;
  vector_endp = source_endp - remainder;
  destinationp = destp;
  while (sourcep < vector_endp)
    {

      /* fourpixelvec contains 16 bytes: 4 macropixels of 4 bytes each.  */
      /* fourpixelvec will be {[YUYV]0 [YUYV]1 [YUYV]2 [YUYV]3}. the   */
      /* components are all bytes, but we want to work with them as shorts.  */
      /* to do that, merge with a vector of zeros and store the results  */
      /* in hitwopixels and lotwopixels.  */
      /* hitwopixels should contain {[YUYV]0 [YUYV]1} and  */
      /* lotwopixels should contain {[YUYV]2 [YUYV]3}.  */
      

      /* load raw data and increment the source pointer for the next  */
      /* time through the loop.   */
      
      fourpixelvec = vec_ld(0, sourcep);
      sourcep += sizeof(fourpixelvec);

      /* now expand the components from char to short and break the */
      /* vector into hitwopixels and lotwopixels */
      /* hitwopixels will contain {YUYV YUYV} from the first half of */
      /* fourpixelvec. */

      hitwopixels =  (vector unsigned short)vec_mergeh(vec_splat_u8(0),
						       fourpixelvec);

      /* lotwopixels will contain {YUYV YUYV} from the second half of */
      /* fourpixelvec. */ 

      lotwopixels =  (vector unsigned short)vec_mergel(vec_splat_u8(0),
						       fourpixelvec);
      
      
       
      
      /* ---------- process macropixel 0 --------------- */
      {
	
	/* build mp0_y_vec as */
	/* {mp0_y0, mp0_y0, mp0_y0, alpha, mp0_y1, mp0_y1, mp0_y1, alpha} */
	/* we do this using the vec_perm intrinsic. hitwopixels is the    */
	/* first source, that will supply mp0_y0. zeroalpha is the second */
	/* argument and it will supply alpha. y0perm tells how to take    */
	/* bytes from hitwopixels and  zeroalpha to form mp0_y_vec  */
	
	mp0_y_vec = (vector short)vec_perm(hitwopixels, zeroalpha, 
						  y0perm);
	
	/* make mp0_u_vec consist only of macropixel 0's u component   */
	/* this is done by pulling the first U element from hitwopixels  */
	/* and loading that into all elements of a new vector. the   */
	/* vec_splat intrinsic does this.  */
	
	mp0_u_vec = (vector short)vec_splat(hitwopixels, MP0_U_ELT);
	
	/* and before we can use these U values we need to subtract   */
	/* 128 from them. do this with the vec_sub intrinsic.         */
	
	mp0_u_vec = vec_sub(mp0_u_vec, uvbias);
	
	/* make mp0_v_vec  consist only of macropixel 0's v component */
	/* this uses vec_splat to build the vector (like we did for   */
	/* mp0_u_vec) and vec_sub to subtract 128 from all it's elements  */
	
	mp0_v_vec = (vector short)vec_splat(hitwopixels, MP0_V_ELT);
	mp0_v_vec = vec_sub(mp0_v_vec, uvbias);
	
	/* now that we've formed all the vectors, let's do the math...  */
	
	/* multiply u_vec by u_coeff  */
	
	/* Multiply eight 16-bit values in u_vec by eight 16-bit */
	/* values in u_coeff and produce the low 16 bits of the results.  */
	/* since the values in u_vec are between -128 and +127 and  */
	/* the largest coefficient is 113, the results should fit in   */
	/* 16 bits.   */
	
	mp0_u_vec = vec_mladd(mp0_u_vec, u_coeff, vec_splat_s16(0));
	
	/* likewise  v_vec by v_coeff  */
	
	mp0_v_vec =  vec_mladd(mp0_v_vec, v_coeff, vec_splat_s16(0));
	
        /* now add u and v then shift the contents of the sum right */
        /* 6 places (effectively dividing by 64)   */
	/* tested replacing const vector shiftvec with vec_splat_s16(6) */
        /* and the timing did not seem to speed up any, so left */
        /* shiftvec as a const vector */ 
	
	/* now mp0_rgba = y_vec + ((u_vec + v_vec) >> 6)  */
	mp0_uv_vec = vec_adds(mp0_v_vec, mp0_u_vec);
	mp0_uv_vec = vec_sra(mp0_uv_vec, shiftvec);
	
	mp0_rgba = vec_adds( mp0_y_vec, mp0_uv_vec);
	
      } /* end macropixel0  */

      
      /* ---------- process macropixel 1 --------------- */
      {

	/* this is just like macropixel 0, except that we're harvesting  */
	/* the  [YUYV]1 part of the {[YUYV]0 [YUYV]1} in hitwopixels   */
	/* and storing the result in mp1_rgba  */
	
	mp1_y_vec  = (vector short)vec_perm(hitwopixels, zeroalpha,
						   y1perm);

	
	/* make mp1_u_vec consist only of macropixel 1's u component   */
	/* then subtract uvbias  */
	mp1_u_vec = (vector short)vec_splat(hitwopixels, MP1_U_ELT);
	mp1_u_vec = vec_sub(mp1_u_vec, uvbias);
	
	/* make mp1_v_vec consist only of macropixel 1's v component */
	/* then subtract uvbias  */
	mp1_v_vec = (vector  short)vec_splat(hitwopixels, MP1_V_ELT);
	mp1_v_vec = vec_sub(mp1_v_vec, uvbias);
	
        /* multiply u & v vectors by their coefficients */
	mp1_u_vec = vec_mladd(mp1_u_vec, u_coeff, vec_splat_s16(0));

	mp1_v_vec = vec_mladd(mp1_v_vec, v_coeff, vec_splat_s16(0));

        /* now mp1_rgba = y_vec + ((u_vec + v_vec) >> 6)  */
	mp1_uv_vec = vec_adds(mp1_v_vec, mp1_u_vec);
	mp1_uv_vec = vec_sra(mp1_uv_vec, shiftvec);
	
	mp1_rgba = vec_adds( mp1_y_vec, mp1_uv_vec);

      } /* end macropixel1  */


      /* ---------- process macropixel 2 --------------- */

      /* macropixel 2 should look like macropixel 0, except it comes */
      /* from lotwopixels and is assigned to mp2_* variables           */
      {
	mp2_y_vec = (vector signed short)vec_perm(lotwopixels, zeroalpha, 
						  y0perm);

	mp2_u_vec = (vector signed short)vec_splat(lotwopixels, MP0_U_ELT);
	mp2_u_vec = vec_sub(mp2_u_vec, uvbias);

	mp2_v_vec = (vector signed short)vec_splat(lotwopixels, MP0_V_ELT);
	mp2_v_vec = vec_sub(mp2_v_vec, uvbias);

	mp2_u_vec = vec_mladd(mp2_u_vec, u_coeff, vec_splat_s16(0));
	mp2_v_vec = vec_mladd(mp2_v_vec, v_coeff, vec_splat_s16(0));

	mp2_uv_vec = vec_adds(mp2_v_vec, mp2_u_vec);
	mp2_uv_vec = vec_sra(mp2_uv_vec, shiftvec);
	
	mp2_rgba = vec_adds( mp2_y_vec, mp2_uv_vec);

      }

      /* ---------- process macropixel 3 --------------- */

      {

	/* macropixel 3 is like macropixel 1, only from lotwopixels  */
	/* and the result is stored in mp3_rgba.  */
	
	mp3_y_vec  = (vector signed short)vec_perm(lotwopixels, zeroalpha,
						   y1perm);

	mp3_u_vec = (vector signed short)vec_splat(lotwopixels, MP1_U_ELT);
	mp3_u_vec = vec_sub(mp3_u_vec, uvbias);

	mp3_v_vec = (vector signed short)vec_splat(lotwopixels, MP1_V_ELT);
	mp3_v_vec = vec_sub(mp3_v_vec, uvbias);

	mp3_u_vec = vec_mladd(mp3_u_vec, u_coeff, vec_splat_s16(0));
	mp3_v_vec = vec_mladd(mp3_v_vec, v_coeff, vec_splat_s16(0));

	mp3_uv_vec = vec_adds(mp3_v_vec, mp3_u_vec);
	mp3_uv_vec = vec_sra(mp3_uv_vec, shiftvec);
	
	mp3_rgba = vec_adds( mp3_y_vec, mp3_uv_vec);


      }
      /* now mp0_rgba contains RGBA for macropixel0 and      */
      /* mp1_rgba  contains RGBA for macropixel1. we'd like  */
      /* to pack that into a single vector and turn the results */
      /* from shorts into bytes. things outside the range of   */
      /* 0 to 255 should be clamped. saturation math does this  */
      /* for me with the vec_packsu intrinsic.    */
      
      destinationvec0 = vec_packsu(mp0_rgba, mp1_rgba);

      /* same goes for macropixels 2 and three  */
      destinationvec1 = vec_packsu(mp2_rgba, mp3_rgba);
      
      /* now store destinationvec0 & destinationvec1 back into   */
      /* memory pointed to by destinationp...  */

      vec_st(destinationvec0, 0, destinationp);
      destinationp += sizeof(destinationvec0);
      vec_st(destinationvec1, 0, destinationp);
      destinationp += sizeof(destinationvec1);
      
    } /* end while  */

  /* start here: add something to process the pixels between   */
  /* vector_endp and source_endp  */
  /* code assumes image width multiple of 2 or 4, adjust for that */
  /* here if not the case */ 
}







/* ************************************************************************* 


   NAME:  build4float


   USAGE: 

   vector float avector;
   float e0;
   float e1;
   float e2;
   float e3;
   
   avector =  build4float(e0, e1, e2, e3);

   returns: vector float

   DESCRIPTION:
                 build a vector of 4 floats containing the given elements

		 inline this for best performance
		 
   REFERENCES:

   LIMITATIONS:

   * other methods will probably be faster than this.
   * unused in this file

   FUNCTIONS CALLED:
   
   vec_ld  - load a vector from memory
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     17-Oct-10               initial coding                           gpk

 ************************************************************************* */

inline static vector float build4float(float e0, float e1, float e2, float e3)
{
  float floatarray[FLOAT_ARRAYSIZE]__attribute__ ((aligned (16)));
  vector float retval;

  floatarray[0] = e0; floatarray[1] = e1;
  floatarray[2] = e2; floatarray[3] = e3;

  retval = vec_ld(0, floatarray);

  return(retval);
  
}



/* ************************************************************************* 


   NAME:  build1float


   USAGE: 

   vector float avector;
   float value;
  
   
   avector =  build1float(value);

   returns: vector float

   DESCRIPTION:
                 build a vector if 4 floats, all having the given value

		 inline this for best performance
		 
   REFERENCES:

   LIMITATIONS:

   * other methods (vec_splat) will probably be faster than this.
   * unused in this file

   FUNCTIONS CALLED:
   
   vec_ld  - load a vector from memory

   REVISION HISTORY:

        STR                  Description of Revision                 Author

     17-Oct-10               initial coding                           gpk

 ************************************************************************* */

inline static vector float build1float(float value)
{
 float floatarray[FLOAT_ARRAYSIZE]__attribute__ ((aligned (16)));
  vector float retval;

  floatarray[0] = value; floatarray[1] = value;
  floatarray[2] = value; floatarray[3] = value;

  retval = vec_ld(0, floatarray);

  return(retval);
  
}


/* ************************************************************************* 


   NAME:  char_to_vectors


   USAGE: 

    
   const unsigned char * sourcep = malloc(...);
   vector float mp0;
   vector float mp1;
   vector float mp2;
   vector float mp3;

   char_to_vectors(sourcep, &mp0, &mp1, &mp2, &mp3);

   returns: void

   DESCRIPTION:
                 take the four macropixels (YUYV with byte-wide components)
		 from memory pointed to by sourcep and turn them into
		 4 float vectors. 

		 modifies mp0, mp1, mp2, mp3 

		 this is sort of the inverse of condense_float_rgbas
   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   vec_ld - load a vector from memory
   vec_mergeh - merge two vectors, extract the high half
   vec_mergel - merge two vectors, extract the low half
   vec_unpackh - expand element size of a vector and extract high half
   vec_unpackl - expand element size of a vector and extract low half
   vec_ctf - convert an integer vector to a float vector

   REVISION HISTORY:

        STR                  Description of Revision                 Author

     25-Nov-10               initial coding                           gpk
     14-Jan-11  rewrote to use vector operations instead of scalar    gpk
     21-Jan-11  added missing initializers for uvbias                 gpk
     29-Jan-11  fix comment block                                     gpk
      5-Feb-11  fix comment block                                     kaj
     13-Feb-11  moved subtracting uvbias from here to avc_make_rgb    kaj
                 to move it closer to where scalar routines perform
                 their subtraction
      
 ************************************************************************* */

inline void char_to_vectors(const unsigned char * sourcep, vector float *mp0,
			    vector float * mp1, vector float * mp2,
			    vector float * mp3)
{
  vector unsigned char inputvector;
  vector signed short hiunpack, lounpack;
  vector signed int intvec0, intvec1, intvec2, intvec3; 

  inputvector = vec_ld(0, sourcep);

  /* we want to keep the range 0 - 255 and put it into a short. */
  /* after that we want signed arithmetic, so merge with zero */
  /* vector, then coerce to from unsigned short to signed short */
  /* coerce works because i'm not changing the bits, just */
  /* interpreting them differently. */

  hiunpack = (vector signed short)vec_mergeh(vec_splat_u8(0), inputvector);
  lounpack = (vector signed short)vec_mergel(vec_splat_u8(0), inputvector);

  /* now expand hiunpack and lounpack from shorts to ints and store the */
  /* results in intvec0 ... intvec3 */

  /* note: i want sign extension here so i'm using vec_unpack. above */
  /* where i didn't want sign extension, i used vec_merge with zero */

  intvec0 = vec_unpackh(hiunpack);
  intvec1 = vec_unpackl(hiunpack);

  intvec2 = vec_unpackh(lounpack);
  intvec3 = vec_unpackl(lounpack);

  /* finally convert intvec0 to intvec3 into floating point vectors */
  /* and store them in mp0 to mp3 */
  *mp0 = vec_ctf(intvec0, 0);
  *mp1 = vec_ctf(intvec1, 0);
  *mp2 = vec_ctf(intvec2, 0);
  *mp3 = vec_ctf(intvec3, 0);
}




/* YUYV_*_ELT - in a float vector of YUYV, these are which elements  */
/* are where.   */

#define YUYV_Y0_ELT 0x0 /* first Y component  */
#define YUYV_U_ELT  0x1 /* U component  */
#define YUYV_Y1_ELT 0x2 /* second Y component  */
#define YUYV_V_ELT  0x3 /* V component  */





/* ************************************************************************* 


   NAME:  avc_make_rgb


   USAGE: 

    
   vector float macropixel; -- YUYV as a float vector
   vector float rgba0; -- first pixel from macropixel
   vector float rgba1; -- second pixel from macropixel

   avc_make_rgb(macropixel, &rgba0, &rgba1);

   returns: void

   DESCRIPTION:
                 translate the YUYV macropixel into 2 rgba pixels.

		 the YUYV in macropixel generates two RGB pixels:
		 * rgba0 from YUV using the first Y
		 * rgba1 from YUV using the second Y
		 
 		 this uses the altivec functions to vectorize the 
 		 function. this does the same sort of "vertical math" as the 
 		 yuv422rgb_avc_int does: only with floating point numbers 
 		 instead of integer values. we're using: 

 		 R = Y + 1.4075  * (V - 128) 
 		 G = Y - 0.34455 * (U - 128) - (0.7169 * (V - 128)) 
 		 B = Y + 1.7790  * (U - 128) 

 		 altivec gives me a vector that can hold 4 single 
 		 precision floats. (within precision that i need here.) 

 		 so turn the equations 90 degrees and do the sums 
 		 vertically: 
 
     1 *  Y                    Y              Y            Y       255 
     u_coeff * U       0.0   * U   -0.34455 * U  1.7790  * U   0.0 * U 
     v_coeff * V      1.4075 * V   -0.7169  * V    0.0   * V   0.0 * V 
    + ---------       ----------   ------------  -----------   ------- 
       result             R           G             B           alpha 


    this lets me vectorize: 

    * the multiplications and summations are done in parallel with the 
      vec_madd intrinsic


   bounds checking (making sure that the values are 0 - 255)
   is done in the packing routine (condense_float_rgbas)
   
   REFERENCES:
   
   http://softpixel.com/~cwright/programming/colorspace/yuv/
  
   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   vec_splat - make a vector with value from another vector
   vec_sub - subtract two vectors
   vec_perm - permute parts of two vectors to build a third

   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     26-Nov-10               initial coding                           gpk
     21-Jan-10  switch from incorrect delta_y to computing Y1 same    gpk
                as Y0
     13-Feb-11  Added subtraction of uvbias (u-128, v-128); Modified  kaj
                 vec_madd by adding a rounding factor; removed
                 vec_sel to set alpha because vec_perm appears to be
                 working - want to reduce complexity
    17-Feb-11  Updated vec_madd comments regarding incrementing       kaj
                 roundVec values
 ************************************************************************* */

inline void avc_make_rgb(const vector float macropixel, vector float* rgba0,
			 vector float* rgba1)
{
  const vector float  u_coeff = {0.0, -0.34455,  1.7790, 0.0 };
  const vector float  v_coeff = {1.4075, -0.7169, 0.0, 0.0 };
  const vector float uvbias = {-128,-128,-128,-128};
  const vector float zeroalpha= {0.0, 0.0, 0.0, 255.0}; 
  vector float roundVec = {0.000005,0.000005,0.000005,0.000005};
  const vector unsigned char y0perm = /* Y0, Y0, Y0, alpha */
    {0x0, 0x1, 0x2, 0x3, 0x0, 0x1, 0x2, 0x3, 
     0x0, 0x1, 0x2, 0x3, 0x1C, 0x1D, 0x1E, 0x1F};
  const vector unsigned char y1perm = /* Y1, Y1, Y1, alpha */
    {0x8, 0x9, 0xA, 0xB, 0x8, 0x9, 0xA, 0xB, 
     0x8, 0x9, 0xA, 0xB, 0x1C, 0x1D, 0x1E, 0x1F};
  vector float y0_vec, u_vec, v_vec, y1_vec, u1_vec, v1_vec;
  vector float uComponent,vComponent,yuComponent,yu1Component;
  vector float lrgba0, lrgba1;

  /* make y0_vec as {y0, y0, y0, alpha} */

  /* i'm drawing from {YUYV} in macropixel. each component's 4 bytes. */
  /* vec_perm works at the byte level, so it will take 4 values in */
  /* y0perm to transfer one component of macropixel */
  /* y0 is bytes {0x0, 0x1, 0x2, 0x3}; */
  /* alpha is the last four bytes of zeroalpha, so that's */
  /* going to be {0x1C, 0x1D, 0x1E, 0x1F} */

  y0_vec = vec_perm(macropixel, zeroalpha, y0perm);
      
  /* likewise y1 will be bytes  {0x0, 0x1, 0x2, 0x3}; */
  /* alpha stays the same at  {0x1C, 0x1D, 0x1E, 0x1F} */

  y1_vec = vec_perm(macropixel, zeroalpha, y1perm);

  /* make u_vec be copies of u, v_vec be copies of v */
  /* also perform (U-128), (V-128) */
  /* (U-128) */
  u1_vec = vec_splat(macropixel, YUYV_U_ELT);
  u_vec = vec_add(uvbias,u1_vec);
  
  /* (V-128) */
  v1_vec = vec_splat(macropixel, YUYV_V_ELT);
  v_vec = vec_add(uvbias,v1_vec);

  /* i want to end up with: */
      
  /* lrgba0 = y0_vec + (u_vec * u_coef) + (v_vec * v_coef) and */
  /* lrgba1 = y1_vec + (u_vec * u_coef) + (v_vec * v_coef) */

  /* altivec has a combination multiply/add operation vec_madd:  */
  /* so i'll do lrgba0 in stages:  */
  /*    lrgba0 =  (u_vec * u_coef) + y0_vec; */
  /*    lrgba0 =  (v_vec * v_coef) + lrgba0  */
  /*                                       */
  /*  expressed as: */
  /*           lrgba0 = vec_madd(u_vec, u_coeff, y0_vec);  */
  /*           lrgba0 = vec_madd(v_vec, v_coeff, lrgba0);   */

  /* 
     Using vec_madd as described above was causing a rounding error.
     Values that had a precision of .9999xx would not round up to the
     nearest floating point and would then be truncated in the vec_cts
     function to convert from float to int. This caused some of the 
     rgb values to be off by 1. Another issue was denormalized values. 
     Values ending in .000000 would be truncated by an entire 
     integer, ex: 8.000000 would become 7.000000 in the vec_cts call.
     By adding in a small rounding factor here, these two issues went
     away

     Incremented roundVec from 0.05 to 0.0000000005 by tenths in separate 
     runs. Only 0.000005 gave results that matched scalar
   */

  /* lrgba0 = y0_vec + (u_vec * u_coef+rounding) + (v_vec * v_coef+rounding) */
  uComponent = vec_madd(u_vec, u_coeff, roundVec);
  vComponent = vec_madd(v_vec, v_coeff, roundVec);

  yuComponent = vec_add(y0_vec,uComponent);
  lrgba0 = vec_add(yuComponent,vComponent);

  /* same for lrgba1 */
  /* lrgba1 = y1_vec + (u_vec * u_coef+rounding) + (v_vec * v_coef+rounding) */
  yu1Component = vec_add(y1_vec,uComponent);
  lrgba1 = vec_add(yu1Component,vComponent);


  /* we'll do the bounds checking in the pack routines */

  *rgba0 = lrgba0;
  *rgba1 = lrgba1; 

 } 




/* ************************************************************************* 


   NAME:  condense_float_rgbas


   USAGE: 

   vector unsigned char fourrgba;
   vector float rgba0;
   vector float rgba1;
   vector float rgba2;
   vector float rgba3;

   fourrgba = condense_float_rgbas(rgba0, rgba1, rgba2, rgba3);

   returns: vector unsigned char

   DESCRIPTION:
                 take the floating point RGBA values from rgba0, rgba1,
		 rgba2, rgba3 and condense the values to 1 byte per
		 pixel. return that as an unsigned char vector.

		 note that we have to take the floating point values that are
		 below zero and return them as zero. the ones over 255
		 must be clamped at 255. vec_packsu has that behavior.

		 to do that, we first take the float vectors and turn them
		 to signed ints, then the signed ints to signed shorts.
		 then the signed shorts to unsigned chars.
		 

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   
   vec_cts - convert to fixed point
   vec_pack - narrow 2 vectors and pack them into one vector
   vec_packsu - saturating pack, unsigned result
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     27-Nov-10               initial coding                           gpk
      5-Feb-11            fixed comment block                         kaj
      
 ************************************************************************* */

inline vector unsigned char condense_float_rgbas(vector float rgba0, 
						 vector float rgba1, 
						 vector float rgba2, 
						 vector float rgba3)
{
  vector signed int intvec0, intvec1, intvec2, intvec3;
  vector signed short shortvec01, shortvec23;
  vector unsigned char output; /* rgbas with byte components */

  /* convert the floats to 32-bit ints. vec_cts also allows a */
  /* division by 2^n along the way. i'm not trying to divide by */
  /* anything, so i'll put in 0 as the second arg (2^0 = 1) */

  /* Note: vec_cts truncates */
  intvec0 = vec_cts(rgba0, 0);
  intvec1 = vec_cts(rgba1, 0);
  intvec2 = vec_cts(rgba2, 0);
  intvec3 = vec_cts(rgba3, 0);

  /* pack ints into shorts */
  shortvec01 = vec_pack(intvec0, intvec1);
  shortvec23 = vec_pack(intvec2, intvec3);

  /* bounds clamping at 0 and 255 happens here. */
  output  = vec_packsu(shortvec01, shortvec23);

  return(output);
} 


 /* *************************************************************************  


    NAME:  yuv422rgb_avc_float 


    USAGE:  

    char red; -- 0 to 255 
    char green; -- 0 to 255 
    char blue; -- 0 to 255 
    int width, height; --  of image in pixels 
    int byte_count; -- number of bytes in YUV image (2 * width * height)  
    const unsigned char * sourcep = malloc(byte_count); -- source yuv image 

    unsigned char * destp = malloc(4 * width * height); -- dest RGBA image 
   
    init_imagedata(red, green, blue, yuv_imagep, byte_count); 
  
    yuv422rgb_avc_float(sourcep, byte_count, destp); 

    returns: void 

    DESCRIPTION: 
                  translate the YUV image starting at sourcep and having 
 		 byte_count bytes from YUYV into RGBA and store the 
 		 results starting at destp. 

    REFERENCES: 


    LIMITATIONS: 
   
    assumes that the width of the image is a multiple of 4 
  
    GLOBAL VARIABLES: none 

    FUNCTIONS CALLED: 
 
    vec_st - store vector in memory 
   
    REVISION HISTORY: 

         STR                  Description of Revision                 Author 

      17-Oct-10               initial coding                           gpk 
       2-Nov-10              correct comment                           kaj 
      12-Nov-10 build an array of raw data converted to float, then    gpk 
                loaded into an vector. use vec_perm, vec_splat to 
 	        build the other vectors from that one, decreasing the 
 	        number of times we load from memory. 
      
  ************************************************************************* */ 

 void yuv422rgb_avc_float(const unsigned char * sourcep, int source_byte_count, 
 		   unsigned char * destp) 
 { 

   unsigned char * destinationp; /* pixels  */ 
   vector float mp0, mp1, mp2, mp3; 
   vector float  rgba0, rgba1; 
   vector float  rgba2, rgba3; 
   vector float  rgba4, rgba5; 
   vector float  rgba6, rgba7; 
   vector unsigned char fourrgba;
   
   const unsigned char *source_endp; 
   const unsigned char *vector_endp; 
   int remainder; 
 
   remainder = source_byte_count % 4;  
  
   source_endp = sourcep + source_byte_count; 
   vector_endp = source_endp - remainder; 
   destinationp = (unsigned char *)destp; 

   /* with each iteration through the loop we pull one macropixel  */ 
   /* (YUYV) from sourcep and increment sourcep to point to the    */ 
   /* next macropixel.  */ 
     while (sourcep < vector_endp) 
     { 
       char_to_vectors(sourcep, &mp0, &mp1, &mp2, &mp3);

       sourcep = sourcep + 16; 

       avc_make_rgb(mp0, &rgba0, &rgba1);     
       avc_make_rgb(mp1, &rgba2, &rgba3); 
       avc_make_rgb(mp2, &rgba4, &rgba5); 
       avc_make_rgb(mp3, &rgba6, &rgba7); 
 
       fourrgba = condense_float_rgbas(rgba0, rgba1, rgba2, rgba3);
       vec_st(fourrgba, 0, destp);
       destp = destp + sizeof(fourrgba);
       fourrgba = condense_float_rgbas(rgba4, rgba5, rgba6, rgba7);
       vec_st(fourrgba, 0, destp);
       destp = destp + sizeof(fourrgba);

    } /* end while  */

    /* start here: add something to process the pixels between   */
    /* vector_endp and source_endp  */
    /* code assumes image width multiple of 2 or 4, adjust for that */
    /* here if not the case */ 
      

}
