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

   printf("%d\t",list.m[0]);
  return 0;
}
