#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

struct test_struct
{
    int val;
    struct test_struct *next;
};

struct test_struct head ;
struct test_struct curr ;

int create_list(int val)
{
    struct test_struct ptr;
    ptr.val = val;
    ptr.next = NULL;

    return 0;
}

