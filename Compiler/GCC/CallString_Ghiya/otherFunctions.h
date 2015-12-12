#ifndef _OTHER_FUNCTIONS_H_
#define _OTHER_FUNCTIONS_H_

int get_rp_index (basic_block bb);

bb_worklist worklist_of_cnode (struct cgraph_node *node);
worklist new_worklist_node (struct cgraph_node *node);
void initialize_fn_worklist();
csdfa_info initialize_csdfa_info();
void perform_shapedfa_on_callblock();
void perform_shapedfa_on_returnblock();
void perform_shapedfa_on_endblock();
void perform_shapedfa_on_intrablock();
 struct cgraph_node * prev_node_from_shape_worklist (void);
 struct cgraph_node * next_node_from_shape_worklist (worklist node);
 bool shape_callee_nodes_processed (void);
 basic_block get_bb_for_shape_processing (bb_worklist list, bool *new_cnode);
 bool shape_callee_nodes_processed (void);
struct cgraph_node *first_node_from_shape_worklist();
void copy_in_to_out (void);
void copy_in_to_out_1 (void);
// bool propagate_pointsto_to_calledblock (struct cgraph_node *node, basic_block bb);
int append_bb_to_callstring_1 (basic_block bb, csdfa_info *ai,
                               basic_block succ_bb, bool in_ipdfa);
int check_callstring_existance (callstring cs, basic_block bb, bool in_ipdfa);
bool callstrings_equal (callstring a, callstring b);
int callstring_length (callstring ai);
csdfa_info get_representative_csdfa (basic_block bb, callstring *cs);
int callstring_is_substring (callstring *cs, int index);
bool propagate_shape_to_calledblock (struct cgraph_node *node, basic_block bb);

pfbv_dfi * compute_in_confluence_pfbv_dfi(pfbv_dfi * p1,pfbv_dfi *p2);
int strip_bb_from_callstring (int cs, basic_block bb, bool in_ipdfa);
int get_caller_bb (basic_block *bb, int cs_index);
basic_block get_bb_from_callstring (int cs);
csdfa_info compute_shape_info(basic_block bb,csdfa_info ai);
void compute_out_from_in_intra();
void compute_out_from_csmap_intra();
 void
modify_pcsmap_at_startblock (struct cgraph_node *node, csdfa_info ai, bool is_new);
 csmap_rhs create_csmap_rhs (csdfa_info ai);                     
 cs_map create_new_csmap_cell (csdfa_info ai);
 void exchange_csmap_entries (cs_map *ai, csmap_rhs *bi);
 void
modify_pcsmap_1 (struct cgraph_node *node, cs_map bi, csdfa_info ai, cs_map prev_lhs);
 void
modify_pcsmap_2 (struct cgraph_node *node, cs_map bi, csdfa_info ai, cs_map prev_lhs);
 void regenerate_shape_at_out (basic_block bb, int rep_cs, csdfa_info ai);
 void
propagate_shape_to_returnblock_out (basic_block retbb);
 void
propagate_shape_to_returnblock_in (basic_block retbb);
 void
perform_regeneration_at_endblock (basic_block sp_bb);
void change_ai_wrt_call_args(csdfa_info ai,basic_block called_bb);
dataFlowValue * process_return_2(dataFlowValue *GEN,dataFlowValue *KILL,dataFlowValue *IN,int pId);
void process_return(csdfa_info ai,basic_block caller_bb);
dataFlowValue * process_return_1(dataFlowValue *GEN,dataFlowValue *KILL,dataFlowValue *IN,int pId,int qId);
dataFlowValue * process_return_2(dataFlowValue *GEN,dataFlowValue *KILL,dataFlowValue *IN,int pId);
void printAttribute(FILE *pFile,shape *attribute);
int compare_pfbv_dfi (pfbv_dfi *a, pfbv_dfi *b);

#endif /* _OTHER_FUNCTIONS_H_ */
