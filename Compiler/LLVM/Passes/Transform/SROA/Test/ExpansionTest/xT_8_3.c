/* nested structure & array with in structure */
#include<stdio.h>
 
struct date
{
   int dd;
   int mm;
   int yy;
};
struct student
{
   int rl;
   struct date dob;
   int m[3];
   int t;
};
int main()
{
   struct student list;

   int i,j,n;
 
   for(i=0;i<n;i++)
   {
     printf("%d\t%d-%d-%d\t",list.rl, list.dob.dd, list.dob.mm, list.dob.yy);
  }
  return 0;
}
