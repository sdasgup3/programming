#include <stdio.h>


int main()
{

struct stud_Res
{
	int rno;
	int std;
	struct stud_Marks
	{
		int subj_nm;
		int subj_mark;
	}marks;
}result;
	printf("\n\n\t Roll Number : %d",result.rno);
	printf("\n\n\t Standard : %d",result.std);
	printf("\nSubject Code : %d",result.marks.subj_nm);
	printf("\n\n\t Marks : %d",result.marks.subj_mark);
        return 0;
}
