
//giving dag at n1 at the end(!!!)
#include<stdio.h>

struct abcd {

  struct abcd *g;
  struct abcd *f;
 
};
struct abcd h;
int main()
{

struct abcd *p,*n1,*n2;

p= (struct abcd *)malloc(sizeof(struct abcd));
n1= (struct abcd *)malloc(sizeof(struct abcd));
n2= malloc(sizeof(struct abcd));

//  p->f=n1;

 if(h.f==NULL)
 {
  p->f=n1;
  n1->f=n2; 
 }
 else
 {
  p->f=n2;
  n2->f=p;
 }
 
 p->f=NULL;
 
}

