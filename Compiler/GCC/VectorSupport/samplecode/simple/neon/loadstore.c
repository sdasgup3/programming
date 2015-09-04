/* ************************************************************************* 
* NAME: neon/loadstore.c
*
* DESCRIPTION:
*
* this is example code that uses the arm processor neon intrinsics
* to load data into a vector and store it from the vector into
* an array.
*
* uses:
*
* * vst1q_s16 to store an int16x8_t vector to an 8 element array of int16_t
* * vld1q_s16 to load from an 8 element array of int16_t to an int16x8_t vector
*
* PROCESS:
*
*
* compile debug:
*
* cc -g -o loadstore loadstore.c -mfpu=neon \
     -mfloat-abi=softfp -mcpu=cortex-a8 -std=c99
*
* ./loadstore
*
*
* compile optimized:
*
* cc -O3 -o loadstore loadstore.c -mfpu=neon		\
     -mfloat-abi=softfp -mcpu=cortex-a8 -std=c99
*
* ./loadstore
*
* expected output:
*
* storing from constant vector
* 0: 10
* 1: 20
* 2: 30
* 3: 40
* 4: 50
* 5: 60
* 6: 70
* 7: 80
*
* loading from array
* 0: 1
* 1: 2
* 2: 3
* 3: 4
* 4: 5
* 5: 6
* 6: 7
* 7: 8
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
*    3-Sep-10          initial coding                        gpk
*
* TARGET:  C on an ARM processor with neon intrinsics
*
*
* ************************************************************************* */

#include <stdio.h>
#include <string.h> /* memcpy memset */
#include <stdint.h> /* int8_t, int16_t, int32_t  */
#include <arm_neon.h> /* neon intrinsics  */

int main(int argc, char * argv[])
{
  int16_t array[] ={1, 2, 3, 4, 5, 6, 7, 8};
  int16_t printarray[8];
  int16x8_t avector;
  const int16x8_t constvector = {10, 20, 30, 40, 50, 60, 70, 80};
  int i;

  fprintf(stderr, "storing from constant vector\n");
  
  /* store constvector into printarray  */
  
  vst1q_s16(printarray, constvector);
  for(i = 0; i < 8; i++)
    {
      fprintf(stderr, "%d: %d \n", i, printarray[i]);
    }
  fprintf(stderr, "\n");

  fprintf(stderr, "loading from array\n");

  /* load array into avector  */
  avector = vld1q_s16(array);
  
  /* store avector into printarray  */
  vst1q_s16(printarray, avector);

  for(i = 0; i < 8; i++)
    {
      fprintf(stderr, "%d: %d \n", i, printarray[i]);
    }
  fprintf(stderr, "\n");
  return(0);
}
