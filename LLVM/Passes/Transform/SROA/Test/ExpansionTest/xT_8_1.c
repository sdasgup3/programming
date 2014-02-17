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
 
   /*
   for(i=0;i<n;i++)
   {
     printf("enter rollno,date of birth\n");
     scanf("%d%d%d%d", &list[i].rl,&list[i].dob.dd, &list[i].dob.mm,&list[i].dob.yy);
 
     list[i].t=0;
     printf("enter three subject marks\n");
     for(j=0;j<3;j++)
     {
scanf("%d",&list[i].m[j]);
list[i].t+=list[i].m[j];
     }
   }
   */
   for(i=0;i<n;i++)
   {
     printf("%d\t%d-%d-%d\t",list.rl, list.dob.dd, list.dob.mm, list.dob.yy);
     for(j=0;j<3;j++)
      printf("%d\t",list.m[j]);
     printf("%d\n",list.t);
}
  return 0;
}
