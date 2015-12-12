#include<stdio.h>

struct Node
{
	struct Node *f;
	struct Node *g;
};

int main()
{
struct Node *p,*q;

p=malloc(sizeof(struct Node));
q=malloc(sizeof(struct Node));



p->f=q;
q->f=p;

//r->g=q;
// q->f=p;
p=NULL;


// p=q->f;
// q->g=m;
// p->g=a;
// q->g=NULL; 

}
