/* ************************************************************************* 
* NAME: colorconv/cvt2ps.h
*
* DESCRIPTION:
*
* this is an instruction created from the contents of gcc's xmmintrin.h.
* it takes two __m128 vectors (4 single precision floats) and turns them
* into 8 unsigned 8-bit integers. (useful for turning floating point
* RGB values into 0 - 255 chars.)
* the code here does saturation arithmetic (so if elements of __A, or __B
* are outside the range of 0 - 255, they'll be clamped to 0 - 255.
*
* PROCESS:
*
* GLOBALS: none
*
* REFERENCES:
*
* gcc's  xmmintrin.h
*
* LIMITATIONS:
*
* - based on gcc intrinsics: uses __v4hi type and __builtin_ia32_packuswb
*   compiler intrinsic. if you're compiling on windows, substitute the
*   intel __m64 type and _mm_packs_pu16 intrinsic
* - i don't know if windows does attributes (__inline, etc)
*
* REVISION HISTORY:
*
*   STR                Description                          Author
*
*   22-Dec-09          initial coding                        gpk
*
* TARGET: GNU C version 4 or higher
*
*
* ************************************************************************* */


#ifdef __cplusplus
extern "C" {
#endif /* cplusplus */


/* USE_CVT2PS - if this is defined, this macro will be used by  */
/* sse2.c, sse1.c to convert vectors of floats into vectors of  */
/* unsigned chars. (ie floating point RGB values into 8-bit     */
/* values.) this may be faster or slower than alternate methods */
/* depending on your hardware.                                  */
/* note: this uses gnu-specific functions so you'll need an     */
/* alternate for windows.                                       */

/* #define USE_CVT2PS */ /* check the Makefile for this  */

#ifdef USE_CVT2PS
/* Convert the four SPFP values in A and four SPFP values in B */
/* to eight signed 8-bit integers.  */

extern __inline __m64 __attribute__((__gnu_inline__, __always_inline__, __artificial__))
_mm_cvt2ps_pu8(__m128 __A, __m128 __B)
{
  __v4hi __tmp0 = (__v4hi) _mm_cvtps_pi16 (__A);
  __v4hi __tmp1 = (__v4hi) _mm_cvtps_pi16 (__B);

  /* if builtins are out, use __m64 for v4i, _mm_packs_pu16 for */
  /*  __builtin_ia32_packuswb */

  /* see about __builtin_ia32_packuswb128...  */
  return (__m64) __builtin_ia32_packuswb (__tmp0, __tmp1);
}
#endif /* USE_CVT2PS  */


#ifdef __cplusplus
}
#endif /* cplusplus */
