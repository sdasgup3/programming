//cycle at p,n1,n2

#include<stdio.h>

struct fgi
{
  struct fgi *ff;
};
struct abcd {

  struct abcd *g;
  struct abcd *f;
  struct fgi *t;
  
};


struct abcd *llll;
struct abcd h;

void hello();
int main()
{

struct abcd *p,*n1,*n2;

p= (struct abcd *)malloc(sizeof(struct abcd));
n1= (struct abcd *)malloc(sizeof(struct abcd));
n2= malloc(sizeof(struct abcd));
// t= malloc(sizeof(struct abcd));
 
 p->f=n1;
//  n1->f=n2;
//  hello();
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
//  n1->f=n2;
//  n2->f=t;


//  n2->f=n1;   //cycle here at (p,n1,n2)
// n1->f=t;    //no cycle but still shown as cycle at (p,n1,n2)
// p->f=n2;    //no cycle but still shown as cycle at (p,n1,n2)


}


void hello()
{

  struct abcd *k,*m;
   k= (struct abcd *)malloc(sizeof(struct abcd));
   m= (struct abcd *)malloc(sizeof(struct abcd));
   k->f=m;
   m->f=k;
   hello();
   k->f=llll;
   llll->f=k;
   
   
}
