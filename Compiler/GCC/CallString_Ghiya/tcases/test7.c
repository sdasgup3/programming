#include<stdio.h>

struct abcd {
  struct abcd *g;
  struct abcd *f;  
};

struct efg
{
  struct abcd *g;
};


 void hello(struct abcd *x,struct abcd *y);
int main()
{

struct abcd *l,*m,*n,*p;
struct efg *o;

p= (struct abcd *)malloc(sizeof(struct abcd));
l= (struct abcd *)malloc(sizeof(struct abcd));
m=(struct abcd *)malloc(sizeof(struct abcd));
n=(struct abcd *)malloc(sizeof(struct abcd));

o=(struct efg *)malloc(sizeof(struct efg));

hello(l,m);

m=NULL;

}


void hello(struct abcd *l,struct abcd *m)
{
  
l->f=m;
l->g=m;
  
// m->f=n;

// n->f=p;   
}

