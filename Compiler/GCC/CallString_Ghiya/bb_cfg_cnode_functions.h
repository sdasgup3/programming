 #ifndef _BB_CFG_CNODE_FUN_H_
#define _BB_CFG_CNODE_FUN_H_

bool is_gimple_endblock (gimple t);
gimple_stmt_iterator split_bb_at_stmt (gimple stmt);
basic_block start_bb_of_fn (struct cgraph_node *node);          
void initialize_bbaux_info (struct cgraph_node *node, basic_block bb);
tree get_called_fn_decl (gimple stmt);
void * ggc_internal_cleared_alloc_stat (size_t size MEM_STAT_DECL);
void  preprocess_Cfg();
void restore_Cfg (void);

void print_cfg();
void initialize_cgraphaux_info (struct cgraph_node *node, basic_block endblock,
                                int *rp, int num_bb);
void set_rp_index (basic_block bb, int i);
bool return_block (basic_block bb);
bool end_block (basic_block bb);
bool call_block (basic_block bb) ;
csdfa_info copy_csdfa_info (csdfa_info ci);

void set_in_shape_worklist (struct cgraph_node *called_fn);
void set_bb_to_process_shape (struct cgraph_node *node, basic_block bb);
csdfa_info in_of_bb (basic_block bb);
csdfa_info out_of_bb (basic_block bb);
bool start_block (basic_block bb);
gimple bb_call_stmt (basic_block bb);
bool shape_bypass_block (basic_block bb);
/*  headers for functions in file otherFunctions.h    */
void initialize_fn_worklist();
 worklist new_worklist_node (struct cgraph_node *node);
 void set_bb_to_process_shape(struct cgraph_node *node, basic_block bb);
 int get_rp_index (basic_block bb);
 bb_worklist worklist_of_cnode (struct cgraph_node *node);

void assignRightToLeft_shape_info(pfbv_dfi **left,pfbv_dfi *right);
cs_map pcsmap_of_cnode (struct cgraph_node *node);
void print_pcsmap_at_startblock (basic_block bb,FILE *dump);
bool pcsmap_required (struct cgraph_node *node);
void print_callstring_sequence (int cs,FILE *dump);
basic_block end_bb_of_fn (struct cgraph_node *node);
bool is_shape_bypass_block (basic_block bb);
void print_shape_worklist (FILE *dump);
void initialize_pfbv_dfi(pfbv_dfi **p);
int isItInThe8Stmts (gimple stmt,gimple_stmt_iterator *gsiPtr);
int isStructPtr(tree var);
void propagate_RightToLeft_shapeArray(int *left,int *right);
void assignRightToLeft_shapeArray(int *left,int *right);
struct cgraph_node * cnode_of_bb (basic_block bb);
int isSubstring(char *sub,char *str);

void myPrint_Dump(FILE *f, ...);
char *myconvert(unsigned int num, int base);
int canPrintInfo();

#endif /* _BB_CFG_CNODE_FUN_H_ */
