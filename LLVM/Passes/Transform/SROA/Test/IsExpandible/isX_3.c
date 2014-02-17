#include<stdio.h>



int main()
{
  typedef enum {APPLE, BANANA, ORANGE} VARIETY;
  typedef enum {LONG, ROUND} SHAPE;
  typedef struct fruit {
    VARIETY variety;
    SHAPE shape;
  } FRUIT;


  FRUIT snack;
  snack.variety = ORANGE;
  snack.shape = ROUND;

  switch(snack.variety) {
    case APPLE: 
                printf("%d\n", 0);
                break;
    case BANANA: return 1;
                printf("%d\n", 1);
                break;
    case ORANGE: 
                printf("%d\n", 2);
                break;
  }

  return 0;
}
