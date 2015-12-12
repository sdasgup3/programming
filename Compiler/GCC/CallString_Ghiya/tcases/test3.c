#include<stdio.h>

struct abcd {
  struct abcd *g;
  struct abcd *f;  
};

struct abcd *l,*m,*n,*p;

void hello();
void hello1();
int main()
{
p= (struct abcd *)malloc(sizeof(struct abcd));
l= (struct abcd *)malloc(sizeof(struct abcd));
m=(struct abcd *)malloc(sizeof(struct abcd));
n=(struct abcd *)malloc(sizeof(struct abcd));

// struct abcd *s,*s1,*s2,*s3;
// s=l;
// s1=m;
// s2=n;
// s3=p;
 
l->f=m;


 hello();

 hello1();
 
// l->g=n;



p->f=l;



// s3->f=NULL;


}


void hello()
{
  
m->f=n;
n->f=p;  

}


void hello1()
{
  
m->f=NULL;
n->f=p;  

}
