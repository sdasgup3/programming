/* ************************************************************************* 
* NAME: colorconv2/printValues.c
*
* DESCRIPTION:
*
* This contains the print functions that can be used for debugging
* or just for printing output.
*
* Pass in array sizes so that this can be used by altivec or non-
* altivec functions
*
* PROCESS:
*
* GLOBALS: none
*
* REFERENCES:
*
* LIMITATIONS:
*
* REVISION HISTORY:
*
*   STR        Who     Description
*   13Feb2011  kaj     initial coding   
*   23Feb2011  kaj     added printVecShorts   
*
*
* TARGET: GNU C version 4 or higher
*
* 
*
* ************************************************************************* */
#include <stdio.h>

#ifdef __VEC__
 #include <altivec.h>
#endif

/* ************************************************************************* 
 *    NAME:  printChars
 *
 * ************************************************************************* */
void printChars(char *label, unsigned char *outChars, int arraySize)
{
  int i;

  fprintf(stderr,"%s --> Char:{ ",label);
  for (i = 0; i < arraySize; i++)
  {
     fprintf(stderr,"%d ",outChars[i]);
  }
  fprintf(stderr,"}\n\n");

} /* printChars */


/* ************************************************************************* 
 *    NAME:  printShorts
 *
 * ************************************************************************* */
void printShorts(char *label, signed short *outShorts, int arraySize)
{
  int i;

  fprintf(stderr,"%s --> short:{ ",label);
  for (i = 0; i < arraySize; i++)
  {
     fprintf(stderr,"%d ",outShorts[i]);
  }
  fprintf(stderr,"}\n\n");

} /* printShorts */


/* ************************************************************************* 
 *    NAME:  printInts
 *
 * ************************************************************************* */
void printInts(char *label, signed int *outInts, int arraySize)
{
  int i;

  fprintf(stderr,"%s --> Int:{ ",label);
  for (i = 0; i < arraySize; i++)
  {
     fprintf(stderr,"%d ",outInts[i]);
  }
  fprintf(stderr,"}\n\n");

} /* printInts */


/* ************************************************************************* 
 *     NAME:  printFloats
 *
 * ************************************************************************* */
void printFloats( char *label,float *outFloats, int arraySize)
{
  int i;


  fprintf(stderr,"%s --> float:{ ",label);
  for (i = 0; i < arraySize; i++)
  {
     fprintf(stderr,"%f ",outFloats[i]);
  }
  fprintf(stderr,"}\n\n");

} /* printFloats */


#ifdef __VEC__

/* ************************************************************************* 
 *    NAME:  printVecShorts
 *
 * ************************************************************************* */
void printVecShorts(char *label, vector signed short outShorts, int arraySize)
{
  short printshort[arraySize] __attribute__ ((aligned (16)));

  vec_st(outShorts, 0, printshort);
  printShorts(label,printshort,arraySize);
  
} /* printShorts */

#endif /* __VEC__ */

