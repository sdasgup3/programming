/* ************************************************************************* 
* NAME: colorconv/cputest.c
*
* DESCRIPTION:
*
* find out what Single Instruction Multiple Data (SIMD) features are
* supported by the CPU.
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
* else
*    -- there was an error trying to find out processor features
*
*
* use the contents of the supported structure to see what your
* CPU will support.
*
* use print_cpusimdfeatures_t to dump out the contents of the supported
* structure.
*
* to compile for self-test:
*
* cc -g -o cputest -DTEST_MODE cputest.c
* ./cputest
*
* GLOBALS: none
*
* REFERENCES:
*
* * http://gcc.gnu.org/ml/gcc-patches/2008-03/msg00803.html
*
* * experiments on beagleboard, g5 powerpc.
*
* LIMITATIONS:
*
* * the tests for neon and altivec are based on parsing /proc/cpuinfo.
*   the contents of which can change between releases (so this is not
*   future-proof.)
*
* * i don't have non-altivec powerpc to test on.
*
* REVISION HISTORY:
*
*   STR                Description                          Author
*
*   22-Dec-09          initial coding                        gpk
*    9-Oct-10        added altivec tests                     gpk
*
*
* TARGET: Gnu C version 4 or higher, X86, arm, altivec  processor
*
* 
*
* ************************************************************************* */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* memset  */

#if (defined __i386__) || (defined  __x86_64__)
 /* we're on an X86  */
#include <cpuid.h> /* __get_cpuid_max, __get_cpuid */
#endif /*  __i386__ */

#include "colorconv.h" /* Cpusimdfeatures_t, test_cpu_simd_features */

#if  (defined __i386__) || (defined  __x86_64__) /* we're on an X86  */
/* if we're on X86, the __get_cpuid instruction can do lots of */
/* different things. what it does is controlled by the first argument. */
/* we're interested in two of those things: */
/* FUNC_FEATURES, FUNC_EXTENDED_FEATURES  */    

/* FUNC_FEATURES - return the list of intel-defined features   */
#define FUNC_FEATURES 0x1

/* FUNC_EXTENDED_FEATURES - return the list of non-intel defined  */
/* features. used for AMD processors.   */

#define FUNC_EXTENDED_FEATURES 0x80000001

/* if you're not on X86, don't worry about these. */
#endif /*  __i386__ */

/* local prototypes  */
int test_cpu_simd_features_x86(Cpusimdfeatures_t * supported);
int test_cpu_simd_features_neon(Cpusimdfeatures_t * supported);
int test_cpu_simd_features_altivec(Cpusimdfeatures_t * supported);
/* end local prototypes  */

/* ************************************************************************* 


   NAME:  test_cpu_simd_features


   USAGE: 

   int some_int;
   Cpusimdfeatures_t supported;

   some_int =  test_cpu_simd_features(&supported);

   if (0 == some_int)
   -- carry on
   else
   -- handle an error
   
   returns: int

   DESCRIPTION:
                 check for SIMD-friendly features on the processor and
		 modify supported to indicate which are present.

		 modifies the contents of supported

   REFERENCES:

   LIMITATIONS: 

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   test_cpu_simd_features_x86
   test_cpu_simd_features_neon
   test_cpu_simd_features_altivec
   fprintf
   memset
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     22-Dec-09               initial coding                           gpk

 ************************************************************************* */
int test_cpu_simd_features(Cpusimdfeatures_t * supported)
{
  int retval, cpu_recognized;

  cpu_recognized = 0;
 
#if   (defined __i386__) || (defined  __x86_64__)
  /* compiler defines __i386__  if you're on an X86-class processor  */
  cpu_recognized = 1;
  retval = test_cpu_simd_features_x86(supported);
  
#endif /*  __i386__  */

#ifdef  __arm__
  /* compiler defines __ARM_NEON__  if you're on an ARM  processor  */
  /* with neon extensions  */
  cpu_recognized = 1;
  retval = test_cpu_simd_features_neon(supported);
#endif /* __arm__  */

#ifdef __powerpc__
  cpu_recognized = 1;
  retval = test_cpu_simd_features_altivec(supported);
#endif /* __powerpc__  */
  
  if (0 == cpu_recognized)
    {
      fprintf(stderr, "Warning: CPU type not recognized by %s in file %s\n",
	      __FUNCTION__, __FILE__);
      fprintf(stderr, " No SIMD support will be shown.\n");
      fprintf(stderr, " (Update this function for your processor.)\n");
      /* we're not on a recognized processor, the answer  */
      /* is that nothing is supported: clear all flags  */
      memset(supported, 0, sizeof(*supported));
 
      retval = 0;
    }


  return(retval);
}





/* ************************************************************************* 


   NAME:  test_cpu_simd_features_x86


   USAGE: 

   int some_int;
   Cpusimdfeatures_t supported;

   some_int =  test_cpu_simd_features_x86(&supported);

   if (0 == some_int)
   -- carry on
   else
   -- handle an error
   
   returns: int

   DESCRIPTION:
                 check for SIMD-friendly features on the processor and
		 modify supported to indicate which are present.

		 modifies the contents of supported

   REFERENCES:

   LIMITATIONS: only X86 processors

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   memset
   __get_cpuid
   __get_cpuid_max
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     22-Dec-09               initial coding                           gpk
      1-Mar-11             added xop, fma4                            gpk
      17-Mar-11              clean up                                 gpk
      
 ************************************************************************* */
#if  (defined __i386__) || (defined  __x86_64__) /* we're on an X86  */
int test_cpu_simd_features_x86(Cpusimdfeatures_t * supported)
{
  unsigned int eax, ebx, ecx, edx;
  unsigned int extensions, sig, maxinput;
  int result, retval;

  /* clear all flags so on error the answer is nothing is supported  */
  
  memset(supported, 0, sizeof(*supported));

  /* call __get_cpuid: there will be bits set in ecx, edx for  */
  /* the intel-defined MMX, SSEn features.                     */
  
  result = __get_cpuid (FUNC_FEATURES, &eax, &ebx, &ecx, &edx);

  if (1 != result)
    {
      retval = -1; /* error  */
    }
  else
    {
      retval = 0;
      supported->mmx = (bit_MMX & edx);
      supported->sse1 = (bit_SSE  & edx);
      supported->sse2 = (bit_SSE2  & edx);
      supported->sse3 = (bit_SSE3 & ecx);
      supported->sse4_1 = (bit_SSE4_1 & ecx);
      supported->sse4_2 = (bit_SSE4_2 & ecx);
      
      /* see if there are any features in the extended features list  */
      extensions = 0;
      maxinput = __get_cpuid_max(extensions, &sig );

      if (0 != maxinput)
	{
	  result = __get_cpuid (FUNC_EXTENDED_FEATURES, &eax, &ebx, &ecx, &edx);
	  supported->sse4_a =  (bit_SSE4a & ecx);
#ifdef bit_SSE5
	  supported->sse5 = (bit_SSE5 & ecx);
#else
	  supported->sse5 = 0;
#endif /* bit_SSE5  */
	  supported->three_d_now = (bit_3DNOW & edx);
	  supported->three_d_now2 =(bit_3DNOWP & edx);
#ifdef bit_XOP
	  supported->xop =  (bit_XOP & ecx);
#else
	  supported->xop = 0;
#endif /* bit_XOP  */

#ifdef bit_FMA4
	  supported->fma4 =  (bit_FMA4 & ecx);
#else
	  supported->fma4 = 0;
#endif /* bit_FMA4  */

	}
    }

  return(retval);
  
}
#endif /* __i386__   */

/* MAXLINE - the longest line we'll accept from /proc/cpuinfo  */

#define MAXLINE 256





/* ************************************************************************* 


   NAME:  test_cpu_simd_features_neon


   USAGE: 

   int some_int;
   Cpusimdfeatures_t  supported;


   some_int =  test_cpu_simd_features_neon(&supported);

   if (0 == some_int)
   -- carry on
   else
   -- handle an error
   
  
   returns: int

   DESCRIPTION:
                 look for the neon features in /proc/cpuinfo and modify
		 supported to tell if neon is present

		 return -1 if there was an error reading /proc/cpuinfo
		         0 on no error
			 
		 modifies contents of supported
   REFERENCES:

   LIMITATIONS:

   * format of /proc/cpuinfo can change between releases, so this is not
     future-proof.

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:
   fopen
   perror
   fgets
   strncmp
   strstr
   fprintf

   REVISION HISTORY:

        STR                  Description of Revision                 Author

      9-Oct-10               initial coding                           gpk

 ************************************************************************* */

int test_cpu_simd_features_neon(Cpusimdfeatures_t * supported)
{
  int retval;
  FILE * file;
  char cpuinfo[MAXLINE];
  char * astring;
  int foundit;

  /* clear all flags so on error the answer is nothing is supported  */
  
  memset(supported, 0, sizeof(*supported));

  file = fopen("/proc/cpuinfo", "r");

  if (NULL == file)
    {
      perror("Error: can't read /proc/cpuinfo");
      retval = -1; /* error  */
    }
  else
    {
      foundit = 0;

      astring = fgets(cpuinfo, MAXLINE, file);

      while ((0 == foundit) && (NULL != astring ) )
        {

          if (0 == strncmp("Features", astring, strlen("Features")))
            {

              if (NULL != strstr(astring, "neon"))
                {
                  foundit = 1;
                }
            }

         astring = fgets(cpuinfo, MAXLINE, file);
        }

      if (0 == foundit)
        {
          fprintf(stderr, "Feature 'neon' not found in /proc/cpuinfo.\n");
          retval = 0; /* no error  */
	  supported->neon = 0;
        }
      else
        {
          fprintf(stderr, "Feature 'neon' IS found in /proc/cpuinfo.\n");
	  supported->neon = 1;
	  retval = 0; /* no error  */
        }
    }

  
  return(retval);

}

/* ************************************************************************* 


   NAME:  test_cpu_simd_features_altivec


   USAGE: 
   Cpusimdfeatures_t  supported;
   
   test_altivec_feature(&supported);

   returns: void

   DESCRIPTION:
                 read the contents of /proc/cpuinfo and look for a
		 line that starts with the the word "cpu". this
		 will list the features on the CPU.

		 if "altivec supported" appears in that line,
		    the processor has the altivec extensions. 
		    set the altivec flag in supported.

		 return -1 if there was an error reading /proc/cpuinfo
		         0 on no error


		 

   REFERENCES:

   LIMITATIONS:

   * format of /proc/cpuinfo can change between releases, so this is not
     future-proof.

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   fopen
   perror
   fgets
   strncmp
   strstr
   fprintf
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

      7-Feb-10               initial coding                           gpk

 ************************************************************************* */

int test_cpu_simd_features_altivec(Cpusimdfeatures_t * supported)
{

  FILE * file;
  char cpuinfo[MAXLINE];
  char * astring;
  int foundit;
  int retval;
  
 /* clear all flags so on error the answer is nothing is supported  */
  
  memset(supported, 0, sizeof(*supported));

  file = fopen("/proc/cpuinfo", "r");

  if (NULL == file)
    {
      perror("Fatal error: can't read /proc/cpuinfo");
      retval = -1; /* error  */
    }
  else
    {
      foundit = 0;
      retval = 0; /* no error  */
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
	  supported->altivec = 0;
	}
      else
	{
	  fprintf(stderr, "Feature 'altivec' IS found in /proc/cpuinfo.\n");
	  supported->altivec = 1;
	}
    }
  return(retval);
}




/* ************************************************************************* 


   NAME:  print_cpusimdfeatures_t


   USAGE: 

    
   char * prompt = "something useful";
   Cpusimdfeatures_t supported;

   some_int =  test_cpu_simd_features(&supported);

   ...
   print_cpusimdfeatures_t(prompt, supported);

   returns: void

   DESCRIPTION:
                 print the contents of prompt followed by the contents
		 of supported in human-readable form for debugging.

   REFERENCES:

   LIMITATIONS:

   GLOBAL VARIABLES:

      accessed: none

      modified: none

   FUNCTIONS CALLED:

   fprintf
   
   REVISION HISTORY:

        STR                  Description of Revision                 Author

     22-Dec-09               initial coding                           gpk

 ************************************************************************* */

void print_cpusimdfeatures_t(char * prompt, Cpusimdfeatures_t supported)
{
  fprintf(stderr, "%s\n", prompt);
  if (supported.mmx)
    {
      fprintf(stderr, "MMX is supported\n");
    }
  else
    {
      fprintf(stderr, "MMX is NOT supported\n");
    }

  if (supported.sse1)
    {
      fprintf(stderr, "SSE is supported\n");
    }
  else
    {
      fprintf(stderr, "SSE is NOT supported\n");
    }
   
  if  (supported.sse2)
    {
      fprintf(stderr, "SSE2 is supported\n");
    }
  else
    {
      fprintf(stderr, "SSE2 is NOT supported\n");
    }
   
  if  (supported.sse3)
    {
      fprintf(stderr, "SSE3 is supported\n");
    }
  else
    {
      fprintf(stderr, "SSE3 is NOT supported\n");
    }

  if  (supported.sse4_1)
    {
      fprintf(stderr, "SSE4.1 is supported\n");
    }
  else
    {
      fprintf(stderr, "SSE4.1 is NOT supported\n");
    }

  if (supported.sse4_2) 
    {
      fprintf(stderr, "SSE4.2 is supported\n");
    }
  else
    {
      fprintf(stderr, "SSE4.2 is NOT supported\n");
    }
  if  (supported.sse4_a)
    {
      fprintf(stderr, "SSE4a is supported\n");
    }
  else
    {
      fprintf(stderr, "SSE4a is NOT supported\n");
    }

  if  (supported.sse5)
    {
      fprintf(stderr, "SSE5 is supported\n");
    }
  else
    {
      fprintf(stderr, "SSE5 is NOT supported\n");
    }

 
  if  (supported.three_d_now)
    {
      fprintf(stderr, "3DNOW is supported\n");
    }
  else
    {
      fprintf(stderr, "3DNOW is NOT supported\n");
    }

  if  (supported.three_d_now2)
    {
      fprintf(stderr, "3DNOW2 is supported\n");
    }
  else
    {
      fprintf(stderr, "3DNOW2 is NOT supported\n");
    }

  if (supported.neon)
    {
      fprintf(stderr, "Neon feature set is supported\n");
    }
  else
    {
      fprintf(stderr, "Neon feature set is NOT supported\n");
    }

  if (supported.altivec)
    {
      fprintf(stderr, "Altivec feature set is supported\n");
    }
  else
    {
      fprintf(stderr, "Altivec feature set is NOT supported\n");
    }

  if (supported.xop)
     {
      fprintf(stderr, "XOP feature set is supported\n");
    }
  else
    {
      fprintf(stderr, "XOP feature set is NOT supported\n");
    }
  if (supported.fma4)
     {
      fprintf(stderr, "FMA4 feature set is supported\n");
    }
  else
    {
      fprintf(stderr, "FMA4 feature set is NOT supported\n");
    }
}



/* ================== T E S T  S T U B ==========================  */
/* ================== T E S T  S T U B ==========================  */
/* ================== T E S T  S T U B ==========================  */

#ifdef TEST_MODE

int main(int argc, char * argv[])
{
  Cpusimdfeatures_t supported;
  int retval;

  retval = test_cpu_simd_features(&supported);

  if (0 == retval)
    {
      print_cpusimdfeatures_t("supported features:", supported);
    }
  else
    {
      fprintf(stderr, "error looking for supported SIMD features\n");
      fprintf(stderr, "is the CPUID instruction supported on this processor?\n"
	      );
      fprintf(stderr, "is /proc/cpuinfo readable?\n");
    }

  return(0);
}

#endif /* TEST_MODE  */

/* ================== T E S T  S T U B ==========================  */
/* ================== T E S T  S T U B ==========================  */
/* ================== T E S T  S T U B ==========================  */


