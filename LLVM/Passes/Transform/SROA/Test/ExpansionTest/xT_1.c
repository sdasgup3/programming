#include<stdio.h>

typedef enum { APPLE, BANANA, ORANGE } VARIETY;
typedef enum { LONG, ROUND } SHAPE;

int   Red = 0;
int   Yellow = 1;
int   Orange = 2;


int main()
{ 
  typedef struct fruit {
    VARIETY variety;
    SHAPE shape; 
  } FRUIT;

  FRUIT snack;
  snack.variety = ORANGE;
  snack.shape = ROUND;

  switch   (snack.variety) {
    case APPLE:   return Red;
                  break;          
    case BANANA:  return Yellow;
                  break;         
    case ORANGE:  return Orange;
                  break;  
  } 
  return 0;
}

