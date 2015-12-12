/* ID for each stmt
  p=malloc ----1
  p=NULL ------2
  p=q----------3
  p=&(q->f)----4
  p=q OP n-----5
  p->f=NULL----6
  p->f=q-------7
  p=q->f-------8
  
  When stmts involve array of pointers or array of fields
  //No kill info
  p[]=NULL-----21
  p[]=q--------31
  p->f[]=NULL--61
  p->f[]=q-----71
    
  //Has kill info
  p=q->f[]-----81
*/



#define HEAP_PTR_CHARS 10
#define STMT_CHARS  10


//related to bool equations
#define EMPTY -1
//this LOCATION is used whether we want to find the shape before(then "in" of the statement is used) 
//or after(then "out" of the statement is used" 
#define LOCATION out
//q.cycle is found....change to dag accordingly
#define SHAPE cycle



#define NUM_PTRS noOfHeapPtrs

#define alloc_mem ggc_alloc_cleared_atomic

#define ggc_alloc_cleared_atomic(S)             \
    (ggc_internal_cleared_alloc_stat ((S) MEM_STAT_INFO))
    

#define total_bbs n_basic_blocks-NUM_FIXED_BLOCKS


/*----------------------------related to the fill_table_plugin.h file---------*/

struct ptr_to_struct_table
{
	int id;
	int isTypedef;   // 0 or 1
	char ptrName[100];
	char scope[100];      //function name!!
	char structName[100];
	int noOfFields;
	int isArray;

};



struct ptr_to_field_of_struct_table
{
int id;
int isTypedef;
char ptrFieldName[100];
char structName[100];
char pointsTo[100];
int idInItsStruct;
int isArray;
//char scope[100];   //not required

};

/*--------------------------------------------------------------------*/



/* --------------------copied from my file dfa_utils.h------------*/


typedef enum{TREE=0,DAG,CYCLE} shape;

typedef struct dataFlowValue
{
int **direction;
int **interference;
shape *attribute;
  
}dataFlowValue;


typedef struct pfbv_dfi
{
   dataFlowValue* gen; //for bb this is what is used remaining neglected 
   dataFlowValue* kill;
   dataFlowValue* in;
   dataFlowValue* out;
  // bool ***boolVariables;

} pfbv_dfi;

/*Global Variable declarations */
extern gimple *local_stmt_list;
extern int s_old_size_local,s_new_size_local;
extern int number_of_nodes;
extern int number_of_stmts;
extern pfbv_dfi **current_pfbv_dfi_stmt;
extern pfbv_dfi **current_pfbv_dfi_bb;

// static dataFlowValue *entry_info = NULL;





/* --------------------copied from my file dfa_utils.h------------*/




//mine-- this indicates this dataflow nformation with this callstring
struct cs_dfa
{
//    int linfo;    /*mine--index into the vector (its VEC_df_list_heap i guess!!!!!!) */

   pfbv_dfi * shape_info;   // this means in,out,kill,gen   ..
   //not all may be reqd at a time..ex:out_of_bb(cbb) retrieves the csdfa_info struct
   //we can say out as default in those cases i.e even for in_of_bb(cbb)
   //out field is only used
   
//    int *shapeArray;
   
   bool new_shape_df;
   int cs_index;   /*mine--index into the vector (callstring..)  */
   struct cs_dfa *next;
};
typedef struct cs_dfa *csdfa_info;



/* The auxillary data structre associated with each basic block. 
   This consists of the cgraph node which this basic block belongs to. The IN and
   OUT pointsto information associated with the basic block, the callstring map
   (if the basic block is Sp), and flags to determine the type of the block 
   (call block, return block, end block, start block) */

struct bb_csipdfa_info_def
{
    int rp_index;
    struct cgraph_node *cnode;
    csdfa_info in_ipdfa, out_ipdfa;
    //constraint_list constraint_start, constraint_end;
    bool call_block, return_block;
    bool start_block, end_block;
    bool has_alias, shape_bypass_block;// lv_bypass_block;
    bool bb_has_deref_stmts;
};

typedef struct bb_csipdfa_info_def *bb_csipdfa_info;

/* The worklist queue. It consists of list of cgraph nodes that need to be serviced. */
struct worklist_queue
{
   struct cgraph_node *called_fn;
   bool process_shape;
   struct worklist_queue *next;
};
typedef struct worklist_queue *worklist;

struct bb_worklist_def
{
   bool process_shape;
   basic_block bb;
};
typedef struct bb_worklist_def *bb_worklist;


/* Callstrings are represented as a sequence of callblocks. */
struct callstring_def
{
   basic_block bb;
   struct callstring_def *next;
};
typedef struct callstring_def *callstring;


/* A callstring map (cs_map_def)      is stored at Sp (start of each procedure p). This contains a 
   representative callstring index at LHS, and the callstring indices of all 
   pointsto values at RHS that have the same must-may info as the representative
   callstring. For example, if callstrings W, X, Y, Z have the same pointsto value
   associated with them, and we choose X to be the representative callstring, the
   lhs of cs_map_def will be the index of W in callstring pool, and the rhs
   will be the csmap_rhs consisting of the linked list of indices of X, Y and Z. */
struct csmap_rhs_def
{
   csdfa_info rhsel;
   bool present_in_out;
   struct csmap_rhs_def *next; 
};
typedef struct csmap_rhs_def *csmap_rhs;

struct cs_map_def
{
   csdfa_info lhs;     //mine-- calstring index is present here!!!
   bool present_in_out;
   csmap_rhs rhs;
   struct cs_map_def *next;
};
typedef struct cs_map_def *cs_map;


/* Auxillary data structure associated with each cgraph node. This data
   structure stores the callstring map and the array of blocks to be
   processed */
struct cgraph_info_def
{
   bb_worklist bb_list;
    cs_map pcsmap;   //, lcsmap;
    bool pcsmap_required;
//    bool lcsmap_required;
};
typedef struct cgraph_info_def *cgraph_info;


//----------------------------------------------------------------------------------------------------------
//global varibles throughout
extern struct cgraph_node *cnode;
extern basic_block cbb;
extern basic_block main_firstbb;
extern csdfa_info cshape_info;
extern bool all_contexts_together;
struct stmt_shape
{
  gimple stmt;
  int *shape;
};

DEF_VEC_P(callstring);           //mine-- creates structs VEC_callstring_none and VEC_callstring_base   
                                 //and many functions like VEC_callstring_base_length,VEC_callstring_base_last,VEC_callstring_base_index etc are created...all vector related functions!!!!!!!!!!
   
DEF_VEC_ALLOC_P(callstring, heap);   //mine-- creates struct   VEC_callstring_heap  and a function for memory allocation VEC_callstring_heap_aloc()
                                     // and many othe functions like   VEC_callstring_heap_free,VEC_callstring_heap_copy, VEC_callstring_heap_reserve, VEC_callstring_heap_safe_push,VEC_callstring_heap_safe_insert 
   
#define GEN_STMT(dfi,stmt)   ((dfi)[find_index_of_local_stmt(stmt)]->gen)
#define KILL_STMT(dfi,stmt)   ((dfi)[find_index_of_local_stmt(stmt)]->kill)
#define IN_STMT(dfi,stmt)   ((dfi)[find_index_of_local_stmt(stmt)]->in)
#define OUT_STMT(dfi,stmt)   ((dfi)[find_index_of_local_stmt(stmt)]->out)

// Globals (extern)
extern FILE                                  *pFile;
extern FILE                                  *debug_file;
extern struct ptr_to_struct_table            *tableOfHeaps;
extern int                                    noOfHeapPtrs;
extern int                                    global;
extern int                                    tableOfHeaps_Size;
extern int                                    noOfFieldPointers;
extern int                                    field_Table_Size;
extern struct ptr_to_field_of_struct_table   *field_Table;
extern worklist                               start, shape_node;
extern struct stmt_shape *                    stmt_shape_map;
extern int                                 ***shapeForStmt;
extern struct timeval                         start1, end1;
extern FILE                                  *shape_bb_dump;
extern FILE                                  *debug_file1;
extern FILE                                  *tempFile;
extern int                                    globTemp;
extern int                                    globalStmtId;
