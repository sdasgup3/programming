#include<stdio.h>
#include<stdlib.h>
#include"header.h"

/*********** Stack Utilities *****************/
struct stack* createStack()
{
    struct stack* st = malloc(sizeof(struct stack));
    if(NULL == st) {
        fprintf(stderr,"Cannot allocate memory for stack creation....");
    exit(1);
    }
    st->stacktop = -1;
    st->stackMem = NULL;
    return st;
}

void destroyStack(struct stack* st)
{
    int index ;
    struct stackNode* tempNode;
    while(0 == isEmpty(st)) {
        tempNode =  st->stackMem;
        st->stackMem = st->stackMem->next;
        free(tempNode);
    }
    free(st);
}

void push(struct stack* st,int item)
{
    struct stackNode* newNode = malloc(sizeof(struct stackNode));
    newNode->data = item;
    newNode->next = st->stackMem;
    st->stackMem = newNode;
    st->stacktop++;
}

int pop(struct stack* st)
{
    if(NULL == st->stackMem) {
        fprintf(stderr,"Attemp to pop from am empty stack....");
        exit(0);
    }
    struct stackNode* tempNode;
    int item = st->stackMem->data;
    tempNode =  st->stackMem;
    st->stackMem = st->stackMem->next;
    st->stacktop --;
    free(tempNode);
    return item;
}

int isEmpty(struct stack* st)
{
    return (NULL == st->stackMem);
}


/*********** Graph Utilities *****************/
int* findPredecessor(int index, int* length)
{
    int* tempList = (int*) mycalloc(NODES, sizeof(int));
    int j;

    *length = 0;

    for(j = 1; j<= NODES; j++) {
        if(1 == ADJ_MATRIX[j][index]) {
            tempList[*length] = j;
            (*length) ++;
        }
    }
    return tempList;    
}

int* findSucessor(int index, int* length)
{
    int* tempList = (int*) mycalloc(NODES, sizeof(int));
    int j;

    *length = 0;

     for(j = 1; j<= NODES; j++) {
         if(1 == ADJ_MATRIX[index][j]) {
             tempList[*length] = j;
             (*length) ++;
         }
     }
}
int isHeadDominateTail(int index_head, int index_tail)
{
    int index;
    for(index =0; index < NODES; index ++) {
        if(index_head == DOMINATOR_INFO[index_tail][index]) {
            return 1;
        }
    }
    return 0;  
}

void printDominatorTree(int root, int tabcount)
{
    int index;    
    for(index = 1; index <= tabcount; index++ ) {
        fprintf(outputFp,"   ");
    }
    fprintf(outputFp, "%d\n", root);
    for(index = 1; index <= NODES; index ++ ) {
        if(DOMINATOR_TREE[root][index] == 1) {
            printDominatorTree(index, tabcount + 1);
        }
    }
    return;
}

void printNaturalLoops()
{
    fprintf(outputFp,"\t\t-------------------------------------\n");
    fprintf(outputFp,"\t\t Back Edge\t\tNatural Loop\n");
    fprintf(outputFp,"\t\t-------------------------------------\n\n");
    if(NULL == LOOP_INFO_HEADER) {
        fprintf(outputFp,"\t\t         -- nill --\n");
        return;
    }

    struct naturalLoopInfo* tempInfo = LOOP_INFO_HEADER ;
    int index;
    while(NULL != tempInfo) {
        fprintf(outputFp,"\t\t %d --> %d\t\t [ ", tempInfo->backEdges[1],tempInfo->backEdges[0]);
        for(index =0; index < tempInfo->loop[NODES]; index ++) {
            fprintf(outputFp,"%d ", tempInfo->loop[index]);
        }
        fprintf(outputFp,"]\n");
        tempInfo = tempInfo->next;
    }
}

void printfMergeInfo(void) 
{
    fprintf(outputFp,"\n\n\n\t\t'If Any Two BackEdges Have The Same 'Head', We Combine The Coresponding Loops'\n");
    fprintf(outputFp,"\t\t------------------------\n");
    fprintf(outputFp,"\t\t Combined Natural Loops\n");
    fprintf(outputFp,"\t\t------------------------\n\n");
    if(NULL == LOOP_INFO_HEADER) {
        fprintf(outputFp,"\t\t         -- nill --\n");
        return;
    }
    
    struct naturalLoopInfo* tempInfo = LOOP_INFO_HEADER ;
    int index;
    int counter = 1;
    while(NULL != tempInfo) {
        if(tempInfo->numTails > 1) {
            if(1 == counter) {
                fprintf(outputFp,"\t\tThe Following BackEdges can be combined :\n\n");
            }
            fprintf(outputFp,"\t\t %d ) ", counter++); 
            for(index =1; index <= tempInfo->numTails; index ++) {
                fprintf(outputFp,"{%d --> %d} ", tempInfo->backEdges[index], tempInfo->backEdges[0]);
            }
            fprintf(outputFp,"\n\t\t The Corresponding Merged Natural Loop : [");
            for(index =0; index < tempInfo->loop[NODES]; index ++) {
                fprintf(outputFp,"%d ", tempInfo->loop[index]);
            }
            fprintf(outputFp,"]\n");
        }
        tempInfo = tempInfo->next;
    }
    if(1 == counter) {
        fprintf(outputFp,"\t\t         -- nill --\n");
    }
    fprintf(outputFp,"\n");
}

void printfDominatorInfo(void) 
{
    fprintf(outputFp,"\n\n\n\t\t'D(a) = Set Of Nodes Which Dominates a'\n");
    fprintf(outputFp,"\t\t------------------------\n");
    fprintf(outputFp,"\t\t Dominator Information\n");
    fprintf(outputFp,"\t\t------------------------\n\n");

    int index_1, index_2;
    for(index_1 = 1; index_1 <= NODES; index_1 ++ ) {
        fprintf(outputFp,"\t\t D(%d) = [ ", index_1);
        for(index_2 = 0; index_2 < NODES; index_2 ++ ) {
            if(0 == DOMINATOR_INFO[index_1][index_2]) {
                break;
            }
            fprintf(outputFp,"%d ", DOMINATOR_INFO[index_1][index_2]);
        }
        fprintf(outputFp,"]\n");
    }
    fprintf(outputFp,"\n\n");
}

/*********** Set Utilities *****************/
int* initialiseArray(int* a, int n) 
{
    int index;
    for(index =0; index < n; index++) {
        a[index] = 0;
    }
    return a;    
}

int isEqual(int* a, int* b)
{
    int* tempArray = (int*) mycalloc(NODES +1, sizeof(int));
    int index;
    int len_b;    
    int len_a;    

    for(index =0; index <= NODES; index ++) {
        if(0 == b[index]) {
           break;
        }
        tempArray[b[index]] = 1;
    }
    len_b = index;

    for(index =0; index <= NODES; index ++) {
        if(0 == a[index]) { 
            break;
        }
        if(1 != tempArray[a[index]]) {
            free(tempArray);
            return 0;
        }
    }
    len_a = index;
    if(len_b != len_a) {
        free(tempArray);
        return 0;
    }

    free(tempArray);
    return 1;
}

int* addElementInSet(int* a, int element)
{
    int len_a;
    int i;
    for(i= 0; i <NODES; i++) {
        if(a[i] == 0) {
            break;
        }
        if(a[i] == element) {
            return a;
        }
    }
    a[i] = element;
    a[NODES] = i+1; // Keeping the number of elements at the end    
    return a;
}           

int* findUnion(int* a, int *b)
{
    int* tempArray = (int*) mycalloc(NODES +1, sizeof(int));
    int index;
    for(index =0; index < NODES; index ++) {
        if(0 == b[index]) {
            break;
        }
        tempArray[b[index]] = 1; 
    }
    for(index =0; index < NODES; index ++) {
        if(0 == a[index]) {
            break;
        }
        tempArray[a[index]] = 1;
    }

    int i = 0;
    int j ;
    for(index =1; index <= NODES; index ++) {
        if(tempArray[index] == 1) {
            a[i++] = index;
        }
    }
    for(j=i; j< NODES; j++) {
        a[j] = 0;
    }
    a[NODES] = i; // Keeping the number of elements at the end    

    free(tempArray);
    return a;
} 

int* findIntersection(int* a, int *b) 
{
    int* tempArray = (int*) mycalloc(NODES +1, sizeof(int));
    int index;
    for(index =0; index < NODES; index ++) {
        if(0 == b[index]) {
            break;
        }
        tempArray[b[index]] ++;
    }
    for(index =0; index < NODES; index ++) {
        if(0 == a[index]) {
            break;
        }
        tempArray[a[index]] ++;
    }

    int i = 0;
    int j ;
    for(index =1; index <= NODES; index ++) {
        if(tempArray[index] == 2) {
            a[i++] = index;
        }
    }    
    for(j=i; j< NODES; j++) {
         a[j] = 0;
    }
    a[NODES] = i; // Keeping the number of elements at the end    

    free(tempArray);
    return a;
}


/*********** Memory Utilities *****************/
int ** allocate2DMatrix(int ** array)
{
        int i;
        array = (int **)mycalloc((NODES+1),sizeof(int*));
        for(i = 0; i <= NODES; i++) {
                array[i] = (int*)mycalloc((NODES  + 1 ),sizeof(int));
        }
        return array;
}

void free2DMatrix(int ** array)
{
        int i;
        for(i = 0; i <= NODES; i++)
                free(array[i]);
        free(array);
}

void cleanUpRoutine(FILE *fp)
{
        fclose(fp);
        destroyStack(STACK); 

        struct naturalLoopInfo* tempNode;
        while(NULL != LOOP_INFO_HEADER) {
            tempNode = LOOP_INFO_HEADER;
            LOOP_INFO_HEADER = LOOP_INFO_HEADER->next;
            free(tempNode->loop);
            free(tempNode->backEdges);
            free(tempNode);
        }       

        free2DMatrix(ADJ_MATRIX); 
        free2DMatrix(DOMINATOR_TREE); 
        free2DMatrix(DOMINATOR_INFO);
}

void printdAdjMatrx(int** matrix)
{
        int i, j;
        fprintf(outputFp,"\t ");
        for(i = 0; i <= NODES ; i++) {
                fprintf(outputFp,"%d ", i);
        }
        fprintf(outputFp,"\n");

        for(i = 0; i <= NODES ; i++) {
                fprintf(outputFp,"%d\t:", i);
                for(j = 0; j <= NODES ; j++) {
                        fprintf(outputFp,"%d ",matrix[i][j]);
                }
                fprintf(outputFp,"\n");
        }
        fprintf(outputFp,"\n\n\n");
}

void* mycalloc(int num, int size) 
{
    void* mem = calloc(num,  size);
    if(NULL == mem) {
        fprintf(stderr,"Cannot Allocate Memory...");
    exit(1);
    }
    return mem;
}

/************ Debug Utilities *******************/
void debugInfo(void)
{
    fprintf(outputFp,"\t\tThe Control Graph\n\n");
    printdAdjMatrx(ADJ_MATRIX);
    fprintf(outputFp,"\t\tThe Dominator Info\n\n");
    printdAdjMatrx(DOMINATOR_INFO);
    fprintf(outputFp,"\t\tThe Dominator Tree(Adj Matrix)\n\n");
    printdAdjMatrx(DOMINATOR_TREE);
}
