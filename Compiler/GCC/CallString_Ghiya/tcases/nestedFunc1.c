#include<stdio.h>

struct abcd {
  struct abcd *g;
  struct abcd *f;  
};

struct abcd *l,*m,*n,*p;

void someFunc();
void hello(struct abcd *x);
int main()
{


p= (struct abcd *)malloc(sizeof(struct abcd));
l= (struct abcd *)malloc(sizeof(struct abcd));
m=(struct abcd *)malloc(sizeof(struct abcd));
n=(struct abcd *)malloc(sizeof(struct abcd));

l->f=m;

hello(n);
 
someFunc();
p->f=l;

}

void someFunc()
{
  hello(n);
}

void hello(struct abcd *x)
{
  
m->f=n;
n->f=m;   
}

