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
	printf("\n\n\t Standard : %d",result.std[0]);
        return 0;
}
