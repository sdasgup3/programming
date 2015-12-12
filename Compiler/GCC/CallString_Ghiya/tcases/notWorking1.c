#include<stdio.h>

struct abcd {
  struct abcd *g;
  struct abcd *f;  
};

struct abcd *l,*p,*q;

void hello();
int main()
{
p= (struct abcd *)malloc(sizeof(struct abcd));
l= (struct abcd *)malloc(sizeof(struct abcd));
 
p->f=l;

hello(); 

// l->f=p;
p->g=l;

}


void hello()
{
  p->f=NULL;
//   q=NULL;
}

