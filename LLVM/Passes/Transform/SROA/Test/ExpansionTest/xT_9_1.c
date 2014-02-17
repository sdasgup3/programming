#include <stdio.h>
#include <string.h>
 
struct student_college_detail
{
    int college_id;
    char college_name[50];
};
 
struct student_detail
{
    int id;
    char name[20];
    float percentage;
    // structure within structure
    struct student_college_detail clg_data;
}stu_data, *stu_data_ptr;
 
int main()
{
  struct student_detail stu_data = {1, "Raju", 90.5, 71145,
                                    "Anna University"};
    stu_data_ptr = &stu_data;
 
    printf(" Id is: %d \n", stu_data_ptr->id);
 
    return 0;
}
