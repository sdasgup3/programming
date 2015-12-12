#include<stdio.h>

struct abcd {
  struct abcd *g;
  struct abcd *f;  
};

struct abcd * hello(struct abcd *x,struct abcd *y);
int main()
{

struct abcd *l,*m,*n,*p;

p= (struct abcd *)malloc(sizeof(struct abcd));
l= (struct abcd *)malloc(sizeof(struct abcd));
m=(struct abcd *)malloc(sizeof(struct abcd));
n=(struct abcd *)malloc(sizeof(struct abcd));

m=hello(p,l);
// 
 m=NULL;
}


struct abcd * hello(struct abcd *x,struct abcd *y)
{
  
x->f=y;

x->g=y;

return x;
  
}

