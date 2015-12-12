//converging for this testcase

#define NULL 0
struct abc
{
  struct abc *f;
  struct abc *g;
  int data;
};

int main()
{
  
  int a=10;
  
  struct abc x;
  struct abc *ptr1,*ptr2,*ptr3,*ptr4;
  
  ptr1=&x;
  
  ptr3=malloc(sizeof(struct abc));
  ptr2=malloc(sizeof(struct abc));
  
  ptr1=malloc(sizeof(struct abc));
  
  ptr1->f=ptr2;
  ptr2->f=ptr3;
  
  if(x.f==NULL)
    ptr3->f=ptr1;

  
//    ptr2->f=NULL;
    
  while(a<11)
  { 
//     ptr3=ptr2;
      ptr1->f=ptr3;
    a++;
  }
  
  ptr2=NULL;
 
}


