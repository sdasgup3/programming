#define NULL 0

struct node
{
struct node *next;
int data;
};

void reverse( struct node * ptr ) { 
struct node * a; 
struct node * b = NULL; 

while(ptr != NULL) { 
a = ptr->next; 
ptr->next = b; 
b = ptr; 
ptr = a; 
}
 
// return b; 
}


int main()
{
  reverse(NULL);
/*
struct node *ptr;
struct node * a;
struct node * b = NULL;

while(ptr != NULL) {
a = ptr->next;
ptr->next = b;
b = ptr;
ptr = a;
}*/



}
