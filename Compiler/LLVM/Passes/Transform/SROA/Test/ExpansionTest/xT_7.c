#include<stdio.h>

int main()
{ 
  typedef struct fruit {
    int variety;
    int shape; 
  } fruit;

  fruit st;

  for(int i = 0 ; i < 2; ++i ) {
    fruit st;
    printf("%d %d", st.variety, st.shape);
  }


    printf("%d %d", st.variety, st.shape);
  return 0;
}

