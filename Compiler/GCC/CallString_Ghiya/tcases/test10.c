
#define NULL 0
struct abcd {
  
  struct abcd *f;

  struct abcd *g;	
};

  struct abcd *a,*b;
int main()
{

//   struct abcd *a1,*b1;
//   struct abcd *a2,*b2;
  a=(struct abcd *)malloc(sizeof(struct abcd));
  b=(struct abcd *)malloc(sizeof(struct abcd));
  
 a->g=b;
  
 b->f=a;

 a=NULL;
} 