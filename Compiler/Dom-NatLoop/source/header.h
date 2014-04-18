/*Global Variable Declarations*/
int NODES;
int ENTRY_NODE;
FILE* outputFp;

int **ADJ_MATRIX ;
int **DOMINATOR_TREE;
int **DOMINATOR_INFO;

/*User Defined Types : Stack*/
struct stackNode
{
    int data;
    struct stackNode* next;
};

struct stack
{
        int stacktop;
        struct stackNode* stackMem;
};
struct stack* STACK;
struct stack* createStack();
void destroyStack(struct stack* st);
void push(struct stack* st,int item);
int pop(struct stack* st);
int isEmpty(struct stack* st);

/*User Defined Types : Linked List for Storing Back Edge and Loop info*/
struct naturalLoopInfo
{
    int* backEdges; //backEdges[0] contain the head, and backEdges[1.2.3...] contain the tails.
                    //As after loop merging we may have multiple tails.
    int numTails;
    int* loop;
    struct naturalLoopInfo* next;
};
struct naturalLoopInfo* LOOP_INFO_HEADER;



/*Function Prototypes*/
void InitializationRoutine(FILE* fp);
int ** allocate2DMatrix(int ** array);
void free2DMatrix(int ** array);
void cleanUpRoutine(FILE *fp);
void printdAdjMatrx(int **);
void createDominatorInfo(void);
void createDominatorTree(void);
int* makeDoninatorInfo(int index);
int* findPredecessor(int , int*);
int* findSucessor(int index, int* length);
int* findIntersection(int*, int*);
int* findUnion(int*, int*);
int isEqual(int*, int*);
void* mycalloc(int, int);
int* addElementInSet(int* a, int element);
int* initialiseArray(int* a, int n);
void printDominatorTree(int, int);
void findBackEdgesAndNaturalLoops(void);
void findNaturalLoop(int , int);
void insert(int node, int* loop, struct stack* st);
void addToNaturalLoopInfoList(int, int, int*);
void printNaturalLoops(void);
void debugInfo(void);
void mergeLoops(void);
void printfMergeInfo(void);
void printfDominatorInfo(void);
