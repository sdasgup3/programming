/* ************************************************************************* 
* NAME: colorconv2/printValues.h
*
* DESCRIPTION:
*
* This contains the prototypes for the print functions that can
* be used for debugging or just for printing output
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
*   13Feb2011  kaj      initial coding
*  18-Feb-11   gpk     changed c++ comment to C comment
*   23Feb2011  kaj     added printVecShorts
*
*
* TARGET: GNU C version 4 or higher
*
* 
*
* ************************************************************************* */
#ifndef __PRINTVALS__H__
#define __PRINTVALS__H__


/* ************************************************************************* 
 *   print functions
 *
 * ************************************************************************* */
void printChars(char *label, unsigned char *outChars, int arraySize);
void printShorts(char *label, signed short *outShorts, int arraySize);
void printInts(char *label, signed int *outInts, int arraySize);
void printFloats( char *label,float *outFloats, int arraySize);

#ifdef __VEC__
 void printVecShorts(char *label, vector signed short outShorts, int arraySize);
#endif


#endif /* __PRINTVALS__H__ */
