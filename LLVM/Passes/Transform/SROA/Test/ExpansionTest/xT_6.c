#include<stdio.h>

int main()
{ 
  struct fruit {
    int variety;
    int shape; 
  } st;

  printf("%d %d", st.variety, st.shape);


  return 0;
}

