#include<stdio.h>

struct abcd {
  struct abcd *g;
  struct abcd *f;  
};

struct abcd *l,*m;
// struct abcd *n,*p;

void hello();

int main()
{
// p= (struct abcd *)malloc(sizeof(struct abcd));
l= (struct abcd *)malloc(sizeof(struct abcd));
m=(struct abcd *)malloc(sizeof(struct abcd));
// n=(struct abcd *)malloc(sizeof(struct abcd));


l->f=m;
// hello();
l->g=m;


}


void hello()
{
  
}
