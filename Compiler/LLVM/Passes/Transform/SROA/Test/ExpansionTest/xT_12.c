#include <stdio.h>


int main()
{

struct stud_Res
{
	int std[10];
	struct stud_Marks
	{
		int subj_nm[30];
	}marks;
}result;
	printf("\nSubject Code : %d",result.marks.subj_nm[0]);
        return 0;
}
