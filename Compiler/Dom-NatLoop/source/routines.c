#include<stdio.h>
#include<stdlib.h>
#include"header.h"

void mergeLoops(void)
{
    struct naturalLoopInfo* temp_1 = LOOP_INFO_HEADER;
    struct naturalLoopInfo* temp_2;
    struct naturalLoopInfo* delNode;
    int index;

    while(NULL != temp_1) {
        temp_2 = LOOP_INFO_HEADER;
        while(NULL != temp_2) {
            if(temp_1 != temp_2 && temp_1->backEdges[0] == temp_2->backEdges[0]) {
                temp_1->loop = findUnion(temp_1->loop, temp_2->loop);
                for(index = 1; index <= temp_2->numTails; index ++) {
                    temp_1->backEdges[++(temp_1->numTails)] = temp_2->backEdges[index];
                }
                temp_2->backEdges[0] = 0; //This shows this node has with merged with some other
                                          //with same header.
            }
            temp_2 = temp_2->next;
        }
        temp_1 = temp_1->next;
    }                
}

void addToNaturalLoopInfoList(int tail, int head, int* loop)
{

    struct naturalLoopInfo* newNode = (struct naturalLoopInfo *)malloc(sizeof(struct naturalLoopInfo));

    newNode->backEdges = (int *) mycalloc(NODES, sizeof(int));
    newNode->numTails = 0;
    newNode->backEdges[0] = head;
    newNode->backEdges[++(newNode->numTails)] = tail; 
    newNode->loop = loop;
    newNode->next = NULL;

    if(NULL == LOOP_INFO_HEADER) {
        LOOP_INFO_HEADER = newNode;
    } else {
        newNode->next = LOOP_INFO_HEADER;
        LOOP_INFO_HEADER = newNode;
    }
}

void insert(int node, int* loop, struct stack* st)
{
    int index;
    int loopLength = loop[NODES];
    for(index = 0; index < loopLength; index ++) {
        if(node == loop[index]) {
            return;
        }
    }

    loop = addElementInSet(loop, node);
    push(st, node);
}

void findNaturalLoop(int tail, int head)
{
    int* loop = (int *) mycalloc(NODES + 1, sizeof(int));
    int topStack, index;
    int* predecessorList = NULL;
    int predecessorListLength;

    loop[0]     = head;
    loop[NODES] = 1;
    insert(tail, loop, STACK);

    while(0 == isEmpty(STACK)) {
        topStack = pop(STACK);
        predecessorList = findPredecessor(topStack, &predecessorListLength);
        for(index = 0; index < predecessorListLength; index ++) {
            insert(predecessorList[index], loop, STACK);
        }
        free(predecessorList);
    }

    addToNaturalLoopInfoList(tail,head,loop);
}

void findBackEdgesAndNaturalLoops(void)
{
    LOOP_INFO_HEADER = NULL;
    STACK = createStack();

    int index_tail;
    int index_head;
    for(index_tail = 1; index_tail <= NODES; index_tail++) {
        for(index_head = 1 ; index_head <= NODES; index_head ++) {
            if(1 == ADJ_MATRIX[index_tail][index_head]) {
                if(1 == isHeadDominateTail(index_head, index_tail)) {
                    findNaturalLoop(index_tail, index_head);            
                }
            }
        } 
    }
}

void createDominatorTree(void)
{
    int* leafNodes = (int*) mycalloc(NODES, sizeof(int));
    int* tempNodes = (int*) mycalloc(NODES, sizeof(int));
    int currentLeafNodeCount = 0; 
    int tempNodeCount = 0; 
    int size;
    int dominatorInfoIterator;
    int leafNodesIterator;
    int flag, index;    

    leafNodes[currentLeafNodeCount++] = ENTRY_NODE;

    for(size = 2 ; size <= NODES; size ++) {
    tempNodeCount = 0;
    tempNodes = initialiseArray(tempNodes, NODES);
        for(dominatorInfoIterator = 1 ;dominatorInfoIterator <= NODES; dominatorInfoIterator++) {
            if(DOMINATOR_INFO[dominatorInfoIterator][NODES] == size) {
                for(leafNodesIterator = 0; leafNodesIterator < currentLeafNodeCount; leafNodesIterator++ ) {
                    flag = 0;
                    for(index = 0; index < NODES; index ++ ) {
                        if(leafNodes[leafNodesIterator] == DOMINATOR_INFO[dominatorInfoIterator][index]) {
                DOMINATOR_TREE[leafNodes[leafNodesIterator]][dominatorInfoIterator] =1;
                tempNodes[tempNodeCount++] = dominatorInfoIterator;
                                flag =1;
                break;
                        }
                    } 
                    if(1 == flag) {
                        break;
                    }
                }
            }
        }
    currentLeafNodeCount = tempNodeCount;
        for(index = 0; index < NODES; index++) {
            leafNodes[index] = tempNodes[index] ;
        }       
    }
    free(leafNodes);
    free(tempNodes);        
}

int* makeDoninatorInfo(int index)
{
    int length,i;
    int predecessor;
    int* PredecessorList = findPredecessor(index, &length);

    /*Initialize newDominatorInfo*/
    int* newDominatorInfo = (int*) mycalloc(NODES + 1, sizeof(int));
    predecessor = PredecessorList[0];
    for(i = 0; i<= NODES; i ++) {
        newDominatorInfo[i] = DOMINATOR_INFO[predecessor][i];
    }


    for(i = 0; i < length; i++) {
        predecessor = PredecessorList[i];
        newDominatorInfo = findIntersection(newDominatorInfo, DOMINATOR_INFO[predecessor]);
    }
    newDominatorInfo =   addElementInSet(newDominatorInfo, index);

    free(PredecessorList);
    return newDominatorInfo;
}


void createDominatorInfo()
{
    int isChanged = 1;
    int index_i ;
    int *tempVector = NULL;

    DOMINATOR_INFO[ENTRY_NODE][NODES] = 1;    

    while(1 == isChanged) {
        isChanged = 0;
        for(index_i = 1 ; index_i <= NODES; index_i++) {
            if(ENTRY_NODE == index_i) {
                continue;
            }        
            tempVector = makeDoninatorInfo(index_i);
            if(0 == isEqual(DOMINATOR_INFO[index_i], tempVector)) {
                isChanged = 1;
                free(DOMINATOR_INFO[index_i]);    
                DOMINATOR_INFO[index_i] = tempVector;
            } else {
                free(tempVector);
                tempVector = NULL;
            }            
        }
    }    
}

void InitializationRoutine(FILE* fp)
{
    int index_i;
    int index_j;
    int node_1;
    int node_2;

    fscanf(fp, "%d", &NODES);
    fscanf(fp, "%d", &ENTRY_NODE);

    ADJ_MATRIX         = allocate2DMatrix(ADJ_MATRIX);
    DOMINATOR_TREE     = allocate2DMatrix(DOMINATOR_TREE);
    DOMINATOR_INFO     = allocate2DMatrix(DOMINATOR_INFO);

    while(1) {
        if(EOF == fscanf(fp, "%d", &index_i) || EOF ==  fscanf(fp, "%d", &index_j)) {
            break;
        }
        ADJ_MATRIX[index_i][index_j] =1;
    }

    DOMINATOR_INFO[ENTRY_NODE][0] = ENTRY_NODE;
    for(node_1 =1 ; node_1 <= NODES; node_1 ++ ) {
        if(node_1 == ENTRY_NODE) { 
            continue;
        }
        for(node_2 = 1; node_2 <= NODES ; node_2++) {
            DOMINATOR_INFO[node_1][node_2 - 1] = node_2;
        }
    }
}

