// #include<stdio.h>

struct abcd 
{
struct abcd *f,*g;
};

struct abcd *p,*q;
void hello();
int main()
{
p=(struct abcd *)malloc(sizeof(struct abcd));  
q=(struct abcd *)malloc(sizeof(struct abcd));  

p->f=q;
q->f=p;

hello();

}

void hello()
{
q->g=p;
}