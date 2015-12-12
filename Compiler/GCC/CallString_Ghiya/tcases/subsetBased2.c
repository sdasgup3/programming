// #include<stdio.h>
#define NULL 0
struct abcd 
{
struct abcd *f,*g;
};


void hello();
int main()
{
  struct abcd *p,*q,*r;
p=(struct abcd *)malloc(sizeof(struct abcd));  
q=(struct abcd *)malloc(sizeof(struct abcd));  
r=(struct abcd *)malloc(sizeof(struct abcd));  


p=q->g;
r=p->g;
r->g=q;
p=NULL;


// hello();



}
/*
void hello()
{

  q->f=NULL;
  
}*/