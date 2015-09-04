/* ************************************************************************* 
* NAME: colorconv2/rgbcalc.c
*
* DESCRIPTION:
*
* this is a test program to spot-check YUYV to RGBA conversions.
* feed it decimal YUYV values and it returns RGBA. this is a test program
* so there's no error checking on the input.
*
* PROCESS:
*
* make
*
* `uname -i`/rgbcalc Y U Y V -- feed in YUYV as decimal values
*
* eg
*
* $ `uname -i`/rgbcalc 255 220 0 134
*
* by floating point scalar computation...
* source [255 220 0 134] --> [255 219 255 255], [8 0 163 255]
* by floating point vector computation...
* source [255 220 0 134] --> [255 219 255 255], [8 0 163 255]
*
* by fixed point scalar computation...
* source [255 220 0 134] --> [255 219 255 255], [8 0 162 255]
* by fixed point vector computation...
* source [255 220 0 134] --> [255 219 255 255], [8 0 162 255]
*
*
*
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
*   21-Jan-11          initial coding                        gpk
*   13-Feb-11  added fprintf in #ifdef ___VEC__ to make      kaj
*              certain it was getting called
*   25-Feb-11  added CHECK_FLOAT to set whether we are       gpk
*              checking floating point functions or
*              integer functions
*   1-Apr-11   remove CHECK_FLOAT: test both fixed and       gpk
*              floating point functions; use interfaces
*              to move ifdef's into library
*   02-Apr-11  Replaced calls for the cpu vector features    kaj
*               (sse, neon, altivec) with call to generic
*               library functions (yuv422rgb_[float|fixed] 
*               which will determine supported feature
*   9-Apr-11   use vector-only function so we never call     gpk
*              scalar function and call it vector output
*
* TARGET: GCC version 4 or higher
*
* The contents of this file is in the public domain. if it breaks, you
* get to keep both pieces
*
* ************************************************************************* */

#include <stdio.h>
#include <stdlib.h> /* atoi */
#include <string.h> /* memset, memcpy  */

#include "colorconv.h" /*  yuv422rgb_*  */



int main(int argc, char * argv[])
{
  int y0, u, y1, v, have_vector;
  unsigned char float_macropixel[2 * 4]  __attribute__ ((aligned (16)));
  unsigned char fixed_macropixel[2 * 4]  __attribute__ ((aligned (16)));
  unsigned char float_rgba[8 * 2]  __attribute__ ((aligned (16)));
  unsigned char fixed_rgba[8 * 2]  __attribute__ ((aligned (16)));

  if (5 != argc)
    {
      fprintf(stderr, "Usage: %s Y U Y V\n", argv[0]);
      exit(-1);
    }
  y0 = atoi(argv[1]);
  u = atoi(argv[2]);
  y1 = atoi(argv[3]);
  v= atoi(argv[4]);


  /* some of the vector code assumes there will be at least two  */
  /* float_macropixels. so build the second as a copy of the first  */
  
  float_macropixel[0] = (unsigned char)y0;
  float_macropixel[1] = (unsigned char)u;
  float_macropixel[2] = (unsigned char)y1;
  float_macropixel[3] = (unsigned char)v;
  
  float_macropixel[4] =   float_macropixel[0];
  float_macropixel[5] =   float_macropixel[1];
  float_macropixel[6] =   float_macropixel[2];
  float_macropixel[7] =   float_macropixel[3];

  memcpy(fixed_macropixel, float_macropixel, sizeof(fixed_macropixel));
  
  
  yuv422rgb_scalar_float(float_macropixel, sizeof(float_macropixel),
			 float_rgba);

  
  fprintf(stderr, "by floating point scalar computation...\n");
  fprintf(stderr, "source [%d %d %d %d] --> [%d %d %d %d], [%d %d %d %d]\n",
	  (int)float_macropixel[0], (int)float_macropixel[1],
	  (int)float_macropixel[2], (int)float_macropixel[3],
	  (int)float_rgba[0], (int)float_rgba[1],(int)float_rgba[2],
	  (int)float_rgba[3], (int)float_rgba[4], (int)float_rgba[5],
	  (int)float_rgba[6], (int)float_rgba[7]);

 
  /* zero out float_rgba so if we don't call a vector function we won't */
  /* show the result from a scalar function.  */

  memset(float_rgba, 0, sizeof(float_rgba));

  fprintf(stderr, "by floating point vector computation...\n");

  
  have_vector = yuv422rgb_float_vector(float_macropixel,
				       sizeof(float_macropixel), float_rgba);
  if (0 == have_vector)
    {
      fprintf(stderr,
	      "Warning: no floating point vector support on this processor "
	      );
      fprintf(stderr,
	      "that this code knows how to use. Did /proc/cpuinfo change?\n "
	      );
    }
  else
    {
   
      fprintf(stderr, "source [%d %d %d %d] --> [%d %d %d %d], [%d %d %d %d]\n",
	      (int)float_macropixel[0], (int)float_macropixel[1],
	      (int)float_macropixel[2], (int)float_macropixel[3],
	      (int)float_rgba[0], (int)float_rgba[1],(int)float_rgba[2],
	      (int)float_rgba[3], (int)float_rgba[4], (int)float_rgba[5],
	      (int)float_rgba[6], (int)float_rgba[7]);
    }
  

  yuv422rgb_scalar_fixed(fixed_macropixel, sizeof(fixed_macropixel),
			 fixed_rgba);

  fprintf(stderr, "\nby fixed point scalar computation...\n");
  fprintf(stderr, "source [%d %d %d %d] --> [%d %d %d %d], [%d %d %d %d]\n",
	  (int)fixed_macropixel[0], (int)fixed_macropixel[1],
	  (int)fixed_macropixel[2], (int)fixed_macropixel[3],
	  (int)fixed_rgba[0], (int)fixed_rgba[1],(int)fixed_rgba[2],
	  (int)fixed_rgba[3], (int)fixed_rgba[4], (int)fixed_rgba[5],
	  (int)fixed_rgba[6], (int)fixed_rgba[7]);

  /* zero out float_rgba so if we don't call a vector function we won't */
  /* show the result from a scalar function.  */

  memset(fixed_rgba, 0, sizeof(fixed_rgba));

  fprintf(stderr, "by fixed point vector computation...\n");

  
  have_vector = yuv422rgb_fixed_vector(fixed_macropixel,
				       sizeof(fixed_macropixel), fixed_rgba);
  if (0 == have_vector)
    {
      fprintf(stderr,
	      "Warning: no fixeding point vector support on this processor "
	      );
      fprintf(stderr,
	      "that this code knows how to use. Did /proc/cpuinfo change?\n "
	      );
    }
  else
    {
   
      fprintf(stderr, "source [%d %d %d %d] --> [%d %d %d %d], [%d %d %d %d]\n",
	      (int)fixed_macropixel[0], (int)fixed_macropixel[1],
	      (int)fixed_macropixel[2], (int)fixed_macropixel[3],
	      (int)fixed_rgba[0], (int)fixed_rgba[1],(int)fixed_rgba[2],
	      (int)fixed_rgba[3], (int)fixed_rgba[4], (int)fixed_rgba[5],
	      (int)fixed_rgba[6], (int)fixed_rgba[7]);
    }
  

   


  return(0);
}
