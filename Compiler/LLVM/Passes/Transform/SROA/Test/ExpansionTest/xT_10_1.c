#include <stdio.h>


int main()
{

struct stud_Res
{
	int rno;
	char std[10];
	struct stud_Marks
	{
		char subj_nm[30];
		int subj_mark;
	}marks;
}result;
	scanf("%d",&result.rno);
	scanf("%s",result.std);
	scanf("%s",result.marks.subj_nm);
	scanf("%d",&result.marks.subj_mark);
	printf("\n\n\t Roll Number : %d",result.rno);
	printf("\n\n\t Standard : %s",result.std);
	printf("\nSubject Code : %s",result.marks.subj_nm);
	printf("\n\n\t Marks : %d",result.marks.subj_mark);
        return 0;
}
