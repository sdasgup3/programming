#include<stdio.h>

struct abcd {
  struct abcd *g;
  struct abcd *f;  
};

void hello(struct abcd *x,struct abcd *y);
int main()
{

struct abcd *l,*m,*n,*p;

p= (struct abcd *)malloc(sizeof(struct abcd));
l= (struct abcd *)malloc(sizeof(struct abcd));
m=(struct abcd *)malloc(sizeof(struct abcd));
n=(struct abcd *)malloc(sizeof(struct abcd));
/*
l->f=m;
m->f=l;*/

hello(l,n);

hello(p,m);
/* 
p->f=l;*/

}


void hello(struct abcd *x,struct abcd *y)
{
  
x->f=y;
x->g=y;
  
// m->f=n;

// n->f=p;   
}

