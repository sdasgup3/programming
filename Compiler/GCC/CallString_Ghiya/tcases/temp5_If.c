#include<stdio.h>

struct Node
{
	struct Node *f;
	struct Node *g;
};

int main()
{
struct Node *p,*q,*r;

p=malloc(sizeof(struct Node));
q=malloc(sizeof(struct Node));

if(p==NULL)
	p->f=q;
else
	q->f=p;




p=NULL;
}
