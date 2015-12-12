#include "all_includes_defines.h"

static void process_arg2(dataFlowValue *OUT, dataFlowValue *IN, int pId);
static void process_arg1(dataFlowValue *OUT, dataFlowValue *IN, int pId, int qId);

extern VEC(callstring, heap) *callstrings; //pointer of struct VEC_callstring_heap

void initialize_fn_worklist()
{
	//mine-- start is a worklist created with cnode being in that worklist  (cnode denoting that corresponding function in worklist)
	start = new_worklist_node (cnode);
	set_bb_to_process_shape (cnode, start_bb_of_fn (cnode));     //mine--start_bb_of_fn since its a forward analysis
	/* Initialize the global worklist pointers. */
	shape_node = start;
}

/* Return a new worklist node. */
	worklist
new_worklist_node (struct cgraph_node *node)
{
	/*mine--  The worklist queue. It consists of list of cgraph nodes that need to be serviced. */
	worklist temp = (worklist) alloc_mem (sizeof (struct worklist_queue));
	temp->called_fn = node;
	temp->process_shape = true;
	return temp;
}
/* Get the reverse postorder index of the basic block. */
	int
get_rp_index (basic_block bb)
{
	return ((bb_csipdfa_info)(bb->aux))->rp_index;
}

bb_worklist worklist_of_cnode (struct cgraph_node *node)
{
	return ((cgraph_info)(node->aux))->bb_list;
}
/*--------------------------------------------------------------------------------
  Copy the in pointsto information for a basic block to the out information.
  After copying, reset the 'new_shape_df' flag for each points-to information.
  ------------------------------------------------------------------------------*/
void copy_in_to_out (void)
{
	csdfa_info ai, bi=NULL;
	for (ai=in_of_bb (cbb); ai; ai=ai->next) {
		if (ai->new_shape_df) {
			csdfa_info prev = NULL;
			for (bi=out_of_bb (cbb); bi; bi=bi->next) {
				if (ai->cs_index >= bi->cs_index)
					break;
				prev = bi;
			}
			if (bi && (bi->cs_index == ai->cs_index)) {
				assignRightToLeft_shape_info(&(bi->shape_info),ai->shape_info);
				bi->new_shape_df = true;
			}
			else {
				csdfa_info temp = copy_csdfa_info (ai);
				if (prev == NULL) {
					temp->next = out_of_bb (cbb);
					((bb_csipdfa_info)(cbb->aux))->out_ipdfa = temp;
				}
				else {
					temp->next = prev->next;
					prev->next = temp;
				}
			}
			ai->new_shape_df = false;
		}
	}
}


void copy_in_to_out_1 (void)
{
#ifdef MYDEBUG
	printf("\n In copy_in_to_out_1     --1");
#endif

	csdfa_info ai, bi=NULL;
	for (ai=in_of_bb (cbb); ai; ai=ai->next) {
		if (ai->new_shape_df) {
			csdfa_info prev = NULL;
			for (bi=out_of_bb (cbb); bi; bi=bi->next) {
				if (ai->cs_index >= bi->cs_index)
					break;
				prev = bi;
			}
			if (bi && (bi->cs_index == ai->cs_index)) {
				pfbv_dfi *old_info;

				initialize_pfbv_dfi(&old_info);
				assignRightToLeft_shape_info(&old_info,bi->shape_info);	       
				pfbv_dfi *temp=compute_in_confluence_pfbv_dfi(bi->shape_info,ai->shape_info);

				free_pfbv_dfi(&(bi->shape_info));
				bi->shape_info = temp;
				if(compare_pfbv_dfi(bi->shape_info,old_info))
					bi->new_shape_df=true;
			}
			else {
				csdfa_info temp = copy_csdfa_info (ai);
				if (prev == NULL) {
					temp->next = out_of_bb (cbb);
					((bb_csipdfa_info)(cbb->aux))->out_ipdfa = temp;
				}
				else {
					temp->next = prev->next;
					prev->next = temp;
				}
			}
			ai->new_shape_df = false;
		}
	}

#ifdef MYDEBUG
	printf("\n In copy_in_to_out_1     --4");
#endif
}


/* -------------------------------------------------------------------------
   Function to perform callstring mapping at the start of a function.
   We prefer choosing a representative which has already been processed, i.e
   whose 'new_ptadf' flag is set to false.
   ------------------------------------------------------------------------ */
	void
compute_mapped_out_at_startblock (void)
{
#ifdef MYDEBUG
	printf("\n In compute_mapped_out_at_startblock1");
#endif
	csdfa_info ai, di,result;
	cs_map bi;
	/* copy the mapped points-to info to the outcsipdfa. */
	//mine-- the lhs part of the pcsmap is copied to the out of that bb
	//if there is any rhs for that pcsmap entry then at the and of this bb we need to regenerate it!!!!  as done by the function perform_regeneration_at_endblock
	for (bi=pcsmap_of_cnode (cnode); bi; bi=bi->next) {
		ai = bi->lhs;
		if (ai->new_shape_df) {

#ifdef MYDEBUG
			printf("\n In compute_mapped_out_at_startblock2 ");
#endif
			result=compute_shape_info(cbb,ai);

			csdfa_info prev = NULL;
			bi->present_in_out = true;

			for (di=out_of_bb (cbb); di; di=di->next) {
				if (ai->cs_index >= di->cs_index)
					break;
				prev = di;
			}
			if (di && (di->cs_index == ai->cs_index)) {
				assignRightToLeft_shape_info(&(di->shape_info),result->shape_info);
				di->new_shape_df = true;
#ifdef MYDEBUG
				printf("\n In compute_mapped_out_at_startblock3 ");
#endif
			}
			else {
#ifdef MYDEBUG
				printf("\n In compute_mapped_out_at_startblock4");
#endif
				csdfa_info ci = copy_csdfa_info (result);
				if (prev == NULL) {
					ci->next = out_of_bb (cbb);
					((bb_csipdfa_info)(cbb->aux))->out_ipdfa = ci;
				}
				else {
					ci->next = prev->next;
					prev->next = ci;
				}
			}
			if (!pcsmap_required (cnode))
				ai->new_shape_df = false;
		}
	}
	if (pcsmap_required (cnode)) {    //this would be true if there is any rhs present in that case regeneration is reqd for which endblock should be added to the worklist
		basic_block end_bb = end_bb_of_fn (cnode);
		if (end_block (end_bb))
			set_bb_to_process_shape (cnode, end_bb);
	}
	/* Dump the map in the dump_file */
	//   if (dump_file && !ipacs_time)   //code changed
	//        print_pcsmap_at_startblock (cbb,dump_file);

#ifdef MYDEBUG
	printf("\n In compute_mapped_out_at_startblock5");
#endif
}


void process_call_stmt (gimple stmt)
{
	tree decl = get_called_fn_decl (stmt);
	if (TREE_CODE (decl) == FUNCTION_DECL) {
		basic_block called_bb;
		csdfa_info ai;
		bool add_to_worklist = false;
		struct cgraph_node *node;
		struct function *called_function = DECL_STRUCT_FUNCTION (decl);
		gcc_assert (called_function);
		called_bb = ENTRY_BLOCK_PTR_FOR_FUNCTION (called_function)->next_bb;
		//mine-- node is cnode of called function
		node = cnode_of_bb (called_bb);

		for (ai=out_of_bb (cbb); ai; ai=ai->next) {
			if (ai->new_shape_df) {
				cshape_info = copy_csdfa_info (ai);
				add_to_worklist |= propagate_shape_to_calledblock (node, called_bb);
			}
		}
		if (add_to_worklist) {

#ifdef MYDEBUG
			printf("\n function %s : added to wlist\n",cgraph_node_name(node));
#endif           
			set_bb_to_process_shape (node, called_bb);  //mine-- add the basic block called_bb to node's worklist
			set_in_shape_worklist (node);
		}

#ifdef DUMP_IN_DUMPFILE
		fprintf(debug_file,"pcsmap of func %s \n",cgraph_node_name(node));
		print_pcsmap_at_startblock(start_bb_of_fn(node),debug_file);
#endif		
	}
	/* The call is through function pointers. */
	//    else
	//        process_fptr_call_pointsto (stmt, decl);
}

/* ------------------------------------------------------------------------------
   Function to propagate a single pointsto info cell to the given basic block.
   The pointsto info to be propagated is pointed to by the cshape_info.
   -----------------------------------------------------------------------------*/
bool propagate_shape_to_calledblock (struct cgraph_node *node, basic_block bb)
{

	bool add_to_worklist = false;
	csdfa_info ai=cshape_info, bi, prev=NULL;
#ifdef MYDEBUG   
	print_callstring_sequence(ai->cs_index,stdout);
#endif
	ai->cs_index = append_bb_to_callstring_1(cbb, &ai, bb, true);   //mine-- if appended the new cs_index is returned   //ai->cs_index is also updated

#ifdef MYDEBUG
	printf("\n In propagate_shape_to_calledblock1");
#endif

#ifdef MYDEBUG
	print_callstring_sequence(ai->cs_index,stdout);
#endif

	if (ai->cs_index == -1)
		return false;

	for (bi=in_of_bb (bb); bi; bi=bi->next) {
		if (ai->cs_index >= bi->cs_index)
			break;
		prev = bi;
	}
	if (bi && (bi->cs_index == ai->cs_index)) {
		pfbv_dfi *old_info;
		// this stmt old_info = bi->shape_info; is replaced
		initialize_pfbv_dfi(&old_info);
		assignRightToLeft_shape_info(&old_info,bi->shape_info);

		//when considering about arguments ,instead of using ai,we used modified ai in which the
		//parameters use is reflected
		change_ai_wrt_call_args(ai,bb);
		pfbv_dfi *temp=compute_in_confluence_pfbv_dfi(bi->shape_info,ai->shape_info);
		free_pfbv_dfi(&(bi->shape_info));
		bi->shape_info = temp;
#ifdef MYDEBUG
		printf("\n In propagate_shape_to_calledblock2 ");
#endif
		// if (old_info != bi->shape_info)
		if(compare_pfbv_dfi(bi->shape_info,old_info))  ////comparing the data flow values here!!!!  (IMPORTANT)
		{
			bi->new_shape_df = true;
			add_to_worklist = true;
#ifdef MYDEBUG
			printf("\n In propagate_shape_to_calledblock2.5 ");
#endif
#ifdef DUMP_IN_DUMPFILE
			fprintf(debug_file,"\n Its added to wl because newdf!=olddf)"); 
#endif
			modify_pcsmap_at_startblock (node, bi, false);
		}
		else
#ifdef DUMP_IN_DUMPFILE
			fprintf(debug_file,"\n Its NOT added to wl because newdf==olddf)"); 
#endif

		free_pfbv_dfi(&old_info);
		free_pfbv_dfi(&(ai->shape_info));
		// free(ai);
	}
	else {
		change_ai_wrt_call_args(ai,bb);
		//implement this
		modify_pcsmap_at_startblock (node, ai, true);
#ifdef MYDEBUG
		printf("\n In propagate_shape_to_calledblock3");
#endif
		if (prev == NULL) {
			ai->next = in_of_bb (bb);
			((bb_csipdfa_info)(bb->aux))->in_ipdfa = ai;
		}
		else {
			ai->next = prev->next;
			prev->next = ai;
		}
		add_to_worklist = true;
	}

#ifdef MYDEBUG
	printf("\n In propagate_shape_to_calledblock4 ");
#endif

#ifdef DUMP_IN_DUMPFILE
	fprintf(debug_file,"pcsmap of func %s  just after its called from  %s \n",cgraph_node_name(node),cgraph_node_name(cnode));
	print_pcsmap_at_startblock(start_bb_of_fn(node),debug_file);
#endif

	return add_to_worklist;
}

// change the dataFlowValue values wrt call arguments
void change_ai_wrt_call_args(csdfa_info ai,basic_block called_bb)
{
	gimple call_stmt=bb_call_stmt(cbb);
	tree decl=get_called_fn_decl(call_stmt);
	dataFlowValue *IN=ai->shape_info->gen;
	dataFlowValue *OUT=NULL;
	initialize_some_dataFlowValue(&OUT);
	assignRightToLeft_dataFlowValue(OUT,IN);
	tree formal_arg;
	tree actual_arg;
	int formal_Id;
	int actual_Id = -1;
	int i=0,assignType;   //assignType=1 =>p=q    ,=2  =>p=NULL;
	for(formal_arg=DECL_ARGUMENTS(decl);formal_arg;formal_arg=TREE_CHAIN(formal_arg))
	{
		assignType=-1;
		actual_arg=gimple_call_arg(call_stmt,i);
		i++;
		if(isStructPtr(formal_arg))
		{
			if(TREE_CODE(actual_arg)==SSA_NAME)
				actual_arg=SSA_NAME_VAR(actual_arg);

			if(TREE_CODE(actual_arg)==VAR_DECL || TREE_CODE(actual_arg)==PARM_DECL)
			{
				actual_Id=getId(getTheName(actual_arg),cnode);
				assignType=1;
			}
			if(TREE_CODE(actual_arg)==INTEGER_CST)
			{
				assignType=2;
				//its like p=NULL;
			}

			formal_Id=getId(getTheName(formal_arg),cnode_of_bb(called_bb));
			if(assignType==1)//p=q
			{
				process_arg1(OUT, IN, formal_Id, actual_Id);
				IN=OUT;
				continue;
			}
			if(assignType==2)  //p=NULL
			{
				process_arg2(OUT, IN,formal_Id);
				IN=OUT;
				continue;
			}

		}

	}

	ai->shape_info->gen=OUT;

}

//if the arg mapping is similar to p=NULL
void process_arg2(dataFlowValue *OUT, dataFlowValue *IN,int pId)
{
	int i;
	for(i=0;i<noOfHeapPtrs;i++)
	{
		// kill relations are same as of p=malloc()
		if( (IN->direction)[pId][i])
			(OUT->direction)[pId][i]=0;
		if( (IN->direction)[i][pId])
			(OUT->direction)[i][pId]=0;;

		if( (IN->interference)[pId][i] )
			(OUT->interference)[i][pId]=(OUT->interference)[pId][i]=0;
	}

	(OUT->attribute)[pId]=TREE;

}

//NOT using gen,kill (evaluating OUT directly)
//if the arg mapping is similar to p=q;
void process_arg1(dataFlowValue *OUT,dataFlowValue *IN,int pId,int qId)
{
	int i;
	//copy in to out, change the values related to pId manually
	for(i=0;i<noOfHeapPtrs;i++)
	{

		if(i!=pId && (IN->direction)[i][qId])
			(OUT->direction)[i][pId]=1;

		if(i!=pId && (IN->direction)[qId][i])
			(OUT->direction)[pId][i]=1;

		if(i!=pId && (IN->interference)[qId][i])
			(OUT->interference)[pId][i]=(OUT->interference)[i][pId]=1;

	}

	if((IN->direction)[qId][qId])
		(OUT->direction)[pId][pId]=1;
	if((IN->interference)[qId][qId])
		(OUT->interference)[pId][pId]=1;
	//April 11  
	(OUT->attribute)[pId]=(IN->attribute)[qId];

}
	void
modify_pcsmap_at_startblock (struct cgraph_node *node, csdfa_info ai, bool is_new)
{
#ifdef DUMP_IN_DUMPFILE
	fprintf(debug_file,"\n Is_new is %d\n",is_new);
#endif	

	cs_map bi, prev_lhs=NULL;
	/* If ai is in lhs/rhs of the csmap, modify it. */
	if (!is_new) {
		for (bi=pcsmap_of_cnode (node); bi; bi=bi->next) {
			/* If the changed in info was lhs, we need to choose 
			   a new representative from its rhs values. */
			if (bi->lhs->cs_index == ai->cs_index) {
				if (bi->lhs->new_shape_df == true) {
					modify_pcsmap_1 (node, bi, ai, prev_lhs);
					return;
				}
				else {
					modify_pcsmap_2 (node, bi, ai, prev_lhs);
					break;
				}
			}
			else {
				/* Iterate over all the RHS and see if they match ai. */
				csmap_rhs di, prev_rhs=NULL;
				for (di=bi->rhs; di; di=di->next) {
					if (di->rhsel->cs_index == ai->cs_index) {
						if (!prev_rhs)
							bi->rhs = di->next;
						else
							prev_rhs->next = di->next;

						// 		       ggc_free (di);  DOWN CODE
						// 				      {
						// 					free_pfbv_dfi(&(di->rhsel->shape_info));
						// 					free(di->rhsel);
						// 					free(di);
						// 				      }
						break;
					}
					prev_rhs = di;
				}
			}
			prev_lhs = bi;
		}
	}
	/* Add the dataflow value to the callstring map. */
	for (bi=pcsmap_of_cnode (node); bi; bi=bi->next) {
		//        if (bi->lhs->pinfo == ai->pinfo)
		if(compare_pfbv_dfi(bi->lhs->shape_info,ai->shape_info)==0)
			break;
	}
	if (bi == NULL) {
		cs_map temp = create_new_csmap_cell (ai);
		temp->next = pcsmap_of_cnode (node);
		((cgraph_info)(node->aux))->pcsmap = temp;
	}
	else {
		csmap_rhs temp = create_csmap_rhs (ai);
		temp->next = bi->rhs;
		bi->rhs = temp;
		if (bi->lhs->new_shape_df)
		{
#ifdef DUMP_IN_DUMPFILE		
			fprintf(debug_file,"\n Exchange has happenend\n");
#endif			
			exchange_csmap_entries (&bi, &(bi->rhs));
		}
		((cgraph_info)(node->aux))->pcsmap_required = true;
	}
}

int compare_pfbv_dfi(pfbv_dfi *a,pfbv_dfi *b)
{
	return is_new_dfvalue(a->gen,b->gen);   //returns T if not equal
}

/* Modify both callstrings and liveness maps. */
int append_bb_to_callstring_1 (basic_block bb, csdfa_info *ai,
                               basic_block succ_bb, bool in_ipdfa)
{
	int index = -1;
	callstring oldcs, newcs;
	if ((*ai)->cs_index == -1)
		oldcs = NULL;
	else
		oldcs = VEC_index (callstring, callstrings, (*ai)->cs_index);
	newcs = (callstring) alloc_mem (sizeof (struct callstring_def));
	newcs->bb = bb;
	newcs->next = oldcs;
	index = check_callstring_existance (newcs, succ_bb, in_ipdfa);
	if (index == -1) {
		index = VEC_length (callstring, callstrings);
		VEC_safe_push (callstring, heap, callstrings, newcs);
		(*ai)->cs_index = index;
	}
	return index;
}

pfbv_dfi * compute_in_confluence_pfbv_dfi(pfbv_dfi * p1,pfbv_dfi *p2)
{
	pfbv_dfi *temp;
	int i,j;
	initialize_pfbv_dfi(&temp);

	for(i=0;i<noOfHeapPtrs;i++)
	{
		for(j=0;j<noOfHeapPtrs;j++)
		{
			(temp->gen->direction)[i][j] = (p1->gen->direction)[i][j] ||  (p2->gen->direction)[i][j];
			(temp->gen->interference)[i][j] = (p1->gen->interference)[i][j] ||  (p2->gen->interference)[i][j]; 
		} 
		(temp->gen->attribute)[i]= ( (p1->gen->attribute)[i]>(p2->gen->attribute)[i] ? (p1->gen->attribute)[i]:(p2->gen->attribute)[i]  );
	}

	return temp;
}


	void
modify_pcsmap_1 (struct cgraph_node *node, cs_map bi, csdfa_info ai, cs_map prev_lhs)
{
	cs_map ci;
	/* Can the new bi, with its rhs, be represented by another 
	   csinfo in the map? */
	for (ci=pcsmap_of_cnode (node); ci; ci=ci->next) {
		if(compare_pfbv_dfi(ci->lhs->shape_info,ai->shape_info)==0 && (ci!=bi))
			break;
	}
	/* If not, change bi. */
	if (!ci) {

		assignRightToLeft_shape_info(&(bi->lhs->shape_info),ai->shape_info);
		bi->lhs->new_shape_df = true;
		//mine-- as the rhs is not empty pcsmap is required and so set
		if (bi->rhs)
			((cgraph_info)(node->aux))->pcsmap_required = true;
	}
	/* If yes, merge the two rhs'es. */
	else {
		csmap_rhs temp;
		if (bi->rhs) {
			csmap_rhs rhs = NULL;
			/* Append RHS of bi to RHS of ci. */
			if (ci->lhs->new_shape_df == false) {
				csmap_rhs di;
				for (di=bi->rhs; di; di=di->next) {
					di->rhsel->new_shape_df = true;
					rhs = di;
				}
			}
			else
				for (rhs=bi->rhs; rhs->next; rhs=rhs->next);
			rhs->next = ci->rhs;
			ci->rhs = bi->rhs;
		}
		temp = create_csmap_rhs (ai);
		temp->next = ci->rhs;
		ci->rhs = temp;
		if (ci->lhs->new_shape_df)
			exchange_csmap_entries (&ci, &(ci->rhs));
		/* Remove the row of bi. */
		if (!prev_lhs)
			((cgraph_info)(node->aux))->pcsmap = bi->next;
		else
			prev_lhs->next = bi->next;
		//ggc_free (bi);


		((cgraph_info)(node->aux))->pcsmap_required = true;
	}
}


void
modify_pcsmap_2 (struct cgraph_node *node, cs_map bi, csdfa_info ai, cs_map prev_lhs)
{
	csmap_rhs di = bi->rhs;

	/* If RHS is NULL, remove this map entry. */
	if (!di) {
		if (prev_lhs)
			prev_lhs->next = bi->next;
		else
			((cgraph_info)(node->aux))->pcsmap = bi->next;
		//ggc_free (bi);
		return;
	}
	else {
		// bool rep_found = false;
		/* Initially, replace lhs with the first rhs of the map. */
		bi->lhs = di->rhsel;
		bi->rhs = di->next;
		bi->present_in_out = di->present_in_out;
		//        ggc_free (di);   //free di i.e csmap_rhs (WRONGGGGGGGGG)  because rhsel is 
		//assigned to someone
		//        {
		// 	 free_pfbv_dfi(&(di->rhsel->shape_info));
		// 	 free(di->rhsel);
		// 	 free(di);
		//        }

		/* If new lhs is to be processed, try finding another
		   value from rhs which is already processed. */
		if (bi->lhs->new_shape_df == true) {
			/* Find a better replacement for lhs from rhs:
			   an info with new_ptadf as false. */
			for (di=bi->rhs; di; di=di->next) {
				if (di->rhsel->new_shape_df == false) {
					exchange_csmap_entries (&bi, &di);
					break;

				}
				/* Otherwise if the RHS is already present in out, choose it as LHS. */
				//                if (!rep_found) {     //mine--not required for me
				//                    if (di->present_in_out) {
				//                        exchange_csmap_entries (&bi, &di);
				//                        rep_found = true;
				// 		    }
				//                }
			}
		}
		/* The new replacement of bi's lhs must be at out. */

		//the new replacement of bi's lhs is attached to the
		//out of endblock but with the dfvalue changed to that of
		//another dfvlue present in outOfEndblock whose cs_index is same as ai->cs_index(i.e the one whose dfvalue was changed before entering
		//the function and because of which modify_pcsmap_at_statblock is called
		regenerate_shape_at_out (end_bb_of_fn (node), ai->cs_index, bi->lhs);
	}
}
void regenerate_shape_at_out (basic_block bb, int rep_cs, csdfa_info ai)
{
	csdfa_info di, ei, prev_out = NULL;

	for (di=out_of_bb (bb); di; di=di->next) {
		if (rep_cs >= di->cs_index)
			break;
	}
	if (!di || di->cs_index != rep_cs)
		return;

	for (ei=out_of_bb (bb); ei; ei=ei->next) {
		if (ai->cs_index >= ei->cs_index)
			break;
		prev_out = ei;
	}

	//mine-- (if this func is called from modify_pcsmap_2)
	//ei->cs_index is the same cs as that of one which was newly replaced as representative(new lhs) of the pcsmap
	//then the dfvalue of ei is same as that of di,where di is in out_of_bb(endblock) whose cs is same as that of one which
	//is modified at the start of this cnode(because of which modify_pcsmap_at_startblock is called
	//
	if (ei && (ei->cs_index == ai->cs_index))
	{  
		//     ei->pinfo = di->pinfo;
		assignRightToLeft_shape_info(&(ei->shape_info),di->shape_info);
	}

	else {
		csdfa_info temp = copy_csdfa_info (ai);

		//        temp->pinfo = di->pinfo;
		assignRightToLeft_shape_info(&(temp->shape_info),di->shape_info);
		//        temp->linfo = -2;
		temp->new_shape_df = false; 
		if (prev_out == NULL) {
			temp->next = out_of_bb (cbb);
			((bb_csipdfa_info)(cbb->aux))->out_ipdfa = temp;
		}
		else {
			temp->next = prev_out->next;
			prev_out->next = temp;
		}
	}
}

int get_caller_bb (basic_block *bb, int cs_index)
{
	*bb = get_bb_from_callstring (cs_index);
	return strip_bb_from_callstring (cs_index, *bb, false);
}

basic_block get_bb_from_callstring (int cs)
{
	callstring ai = VEC_index (callstring, callstrings, cs);
	return ai->bb;
}

int strip_bb_from_callstring (int cs, basic_block bb, bool in_ipdfa)
{
	int index = -1;
	callstring oldcs = VEC_index (callstring, callstrings, cs);
	callstring newcs = oldcs->next;
	if (!newcs)
		return -1;
	else {
		index = check_callstring_existance (newcs, bb, in_ipdfa);
		return index;
	}
}

/*------------------------------------------------------------------------------                                                                     
  If the current basic block is the end block, we need to propagate the pointsto  
  information to the return block of the respective caller of the function to     
  which the endblock belongs. To do so, we find out the caller call block by          
  analyzing the callstrings. The pointsto information is then propagated to the    
  return block corresponding to that call block. If the in value has changed, 
  the return block is added to the worklist.
  -----------------------------------------------------------------------------*/

void
propagate_shape_to_returnblock_in (basic_block retbb)                  
{
	csdfa_info ai, prev=NULL;                                              

	for (ai=in_of_bb (retbb); ai; ai=ai->next) {
		if (cshape_info->cs_index >= ai->cs_index)                              
			break;                                                         
		prev = ai;
	}                                                                      
	//    cshape_info->shape_info = calculate_actual_out (cshape_info, ai);                     
	if (ai && (ai->cs_index == cshape_info->cs_index)) {
		pfbv_dfi *temp=compute_in_confluence_pfbv_dfi(ai->shape_info,cshape_info->shape_info);
		free_pfbv_dfi(&(ai->shape_info));
		ai->shape_info = temp;
		free_pfbv_dfi(&(cshape_info->shape_info));
		//free(cshape_info);
	}    
	else {                                                                 
		if (prev == NULL) {
			cshape_info->next = in_of_bb (retbb);
			((bb_csipdfa_info)(retbb->aux))->in_ipdfa = cshape_info;            
		}
		else {
			cshape_info->next = prev->next;
			prev->next = cshape_info;
		}
	}
}

void
propagate_shape_to_returnblock_out (basic_block retbb)
{
	csdfa_info ai, prev=NULL;

	for (ai=out_of_bb (retbb); ai; ai=ai->next) {
		if (cshape_info->cs_index >= ai->cs_index)
			break;
		prev = ai;
	}
	//    cshape_info->pinfo = calculate_actual_out_1 (cshape_info, ai);

	if (ai && (ai->cs_index == cshape_info->cs_index)) {
		pfbv_dfi *temp=compute_in_confluence_pfbv_dfi(ai->shape_info,cshape_info->shape_info);
		free_pfbv_dfi(&(ai->shape_info));
		ai->shape_info = temp;
		free_pfbv_dfi(&(cshape_info->shape_info));
		//free(cshape_info);
	}
	else {
		if (prev == NULL) {
			cshape_info->next = out_of_bb (retbb);
			((bb_csipdfa_info)(retbb->aux))->out_ipdfa = cshape_info;
		}
		else {
			cshape_info->next = prev->next;
			prev->next = cshape_info;
		}
	}
}

void removeLocalVarData(csdfa_info ai)
{
  char *funcName=(char *)cgraph_node_name(cnode);
  
  int i,k;
  for(i=0;i<noOfHeapPtrs;i++)
  {
#ifdef CPP
    if(isSubstring(tableOfHeaps[i].scope,funcName))
#else      
    if(strcmp(tableOfHeaps[i].scope,funcName)==0)
#endif      
    {      
      for(k=0;k<noOfHeapPtrs;k++)
      {	
          (ai->shape_info->gen->direction)[i][k]=0;
          (ai->shape_info->gen->direction)[k][i]=0;
          (ai->shape_info->gen->interference)[i][k]=0;
          (ai->shape_info->gen->interference)[k][i]=0;	
      }
      (ai->shape_info->gen->attribute)[i]=TREE;
    }    
  }      
}


void propagate_shape_to_returnblock (void)
{
	csdfa_info ai;
	for (ai=out_of_bb (cbb); ai; ai=ai->next) {
		if (ai->new_shape_df) {
			basic_block callerbb = NULL;
			int cs_index = get_caller_bb (&callerbb, ai->cs_index);

			/* Propagate a set to in of returnblock. */
			cshape_info = copy_csdfa_info (ai);
			
			removeLocalVarData(cshape_info);
			
			cshape_info->cs_index = cs_index;
			propagate_shape_to_returnblock_in (single_succ (callerbb));
			process_return(ai,callerbb);
			/* Propagate a set to out of returnblock. */
			cshape_info = copy_csdfa_info (ai);
			
			removeLocalVarData(cshape_info);
			
			cshape_info->cs_index = cs_index;
			propagate_shape_to_returnblock_out (single_succ (callerbb));
			ai->new_shape_df = false;
		}
	}
}

//assumption there is only one stmt in the callblock and i.e the callstmt
//process the return dtatement
void process_return(csdfa_info ai,basic_block caller_bb)
{
	gimple call_stmt=bb_call_stmt(caller_bb);
	tree lhsop=gimple_call_lhs(call_stmt);
	if(!lhsop)
		return;
	if(TREE_CODE(lhsop)==SSA_NAME)
		lhsop=SSA_NAME_VAR(lhsop);

	if(!isStructPtr(lhsop))
		return;
	int caller_retId,called_retId=-1;
	dataFlowValue *IN,*OUT = NULL,*GEN,*KILL;
	IN=ai->shape_info->gen;
	caller_retId=getId(getTheName(lhsop),cnode_of_bb(caller_bb));
	gimple_stmt_iterator ret_gsi;
	/* Is there a struct pointer value returned by the return statement? */
	for (ret_gsi = gsi_start_bb (cbb); !gsi_end_p (ret_gsi); gsi_next (&ret_gsi))
	{
		gimple ret_stmt = gsi_stmt (ret_gsi);
		if (gimple_code (ret_stmt) == GIMPLE_RETURN)
		{
			tree returnValue=gimple_return_retval (ret_stmt);
			if(!returnValue)
				return;
			//for the case of p=NULL;
			if(TREE_CODE(returnValue)==INTEGER_CST)
			{
				initialize_some_dataFlowValue(&GEN);
				initialize_some_dataFlowValue(&KILL);
				OUT=process_return_2(GEN,KILL,IN,caller_retId);
				free_dataFlowValue(&GEN);
				free_dataFlowValue(&KILL);
				free_dataFlowValue(&IN);
			}
			if(TREE_CODE(returnValue)==SSA_NAME)
				returnValue=SSA_NAME_VAR(returnValue);
			//similar to caller_retId=called_retId
			if(TREE_CODE(returnValue)==VAR_DECL || TREE_CODE(returnValue)==PARM_DECL)
			{
				called_retId=getId(getTheName(returnValue),cnode);
				initialize_some_dataFlowValue(&GEN);
				initialize_some_dataFlowValue(&KILL); 
				OUT=process_return_1(GEN,KILL,IN,caller_retId,called_retId);
				free_dataFlowValue(&GEN);
				free_dataFlowValue(&KILL);
				free_dataFlowValue(&IN);
			}
		}
	}
	ai->shape_info->gen=OUT;
}
/* ------------------------------------------------------------------------
   Function to propagate all the shape information cells (whose new_shape_df
   is set) of a block to all its successors.    
   -----------------------------------------------------------------------*/
//if the arg mapping is similar to p=q;
dataFlowValue * process_return_1(dataFlowValue *GEN,dataFlowValue *KILL,dataFlowValue *IN,int pId,int qId)
{
	dataFlowValue *OUT;
	int i;
	for(i=0;i<noOfHeapPtrs;i++)
	{
		// kill relations are same as of p=malloc()
		if((IN->direction)[pId][i])
			(KILL->direction)[pId][i]=1;
		if( (IN->direction)[i][pId])
			(KILL->direction)[i][pId]=1;
		if((IN->interference)[pId][i])
			(KILL->interference)[i][pId]=(KILL->interference)[pId][i]=1;
		if(i!=pId && (IN->direction)[i][qId])
			(GEN->direction)[i][pId]=1;

		if(i!=pId && (IN->direction)[qId][i])
			(GEN->direction)[pId][i]=1;

		if(i!=pId && (IN->interference)[qId][i])
			(GEN->interference)[i][pId]=(GEN->interference)[pId][i]=1;
	}

	if((IN->direction)[qId][qId])
		(GEN->direction)[pId][pId]=1;
	if((IN->interference)[qId][qId])
		(GEN->interference)[pId][pId]=1;

	OUT=transfer_fn(GEN,IN,KILL);
	//April 7
	//Transfer_fn doesnot take attr into consideration 
	for(i=0;i<noOfHeapPtrs;i++)
	{
		(OUT->attribute)[i]=(IN->attribute)[i];
	}
	(OUT->attribute)[pId]=(IN->attribute)[qId];
	return OUT;
}
//if the arg mapping is similar to p=NULL
dataFlowValue * process_return_2(dataFlowValue *GEN,dataFlowValue *KILL,dataFlowValue *IN,int pId)
{

	dataFlowValue *OUT;
	int i;
	for(i=0;i<noOfHeapPtrs;i++)
	{
		// kill relations are same as of p=malloc()
		if((IN->direction)[pId][i])
			(KILL->direction)[pId][i]=1;
		if( (IN->direction)[i][pId])
			(KILL->direction)[i][pId]=1;
		if( (IN->interference)[pId][i] )
			(KILL->interference)[i][pId]=(KILL->interference)[pId][i]=1;
	}

	OUT=transfer_fn(GEN,IN,KILL);
	//Transfer_fn doesnot take attr into consideration 
	for(i=0;i<noOfHeapPtrs;i++)
	{
		(OUT->attribute)[i]=(IN->attribute)[i];
	}
	(OUT->attribute)[pId]=TREE;
	return OUT;
}

/* ------------------------------------------------------------------------
   Function to propagate all the pointsto information cells (whose new_shape_df
   is set) of a block to all its successors.    
   -----------------------------------------------------------------------*/


void
propagate_shape_to_all_succs (basic_block bb)
{
#ifdef MYDEBUG
	printf("\nIn propagate_pointsto_to_all_succs of bb%d of %s",bb->index,cgraph_node_name(cnode)); 
#endif
	edge e;
	edge_iterator ei;
	csdfa_info ai, bi;
	FOR_EACH_EDGE (e, ei, bb->succs) {
		basic_block succ_bb = e->dest;
#ifdef MYDEBUG
		printf("\nIn propagate_pointsto_to_all_succs 1"); 
#endif

		bool set_succ_for_processing = false;
		for (ai=out_of_bb (cbb); ai; ai=ai->next) {
			if (ai->new_shape_df) {

				pfbv_dfi *new_info;
#ifdef MYDEBUG
				printf("\nIn propagate_pointsto_to_all_succs 2"); 
#endif		   
				csdfa_info prev = NULL;
				for (bi=in_of_bb (succ_bb); bi; bi=bi->next) {
					if (ai->cs_index >= bi->cs_index)
						break;
					prev = bi;
				}
				new_info=ai->shape_info;
				if (bi && (bi->cs_index == ai->cs_index)) {
					/* calculate confluence of mustout and mayout. */
					pfbv_dfi *old_shape_info;
					//mine--- replace this stmt   old_shape_info = bi->shape_info; by
#ifdef MYDEBUG
					printf("\nIn propagate_pointsto_to_all_succs 3"); 
#endif		
					initialize_pfbv_dfi(&old_shape_info);
					assignRightToLeft_shape_info(&old_shape_info,bi->shape_info);
					pfbv_dfi *temp=compute_in_confluence_pfbv_dfi(bi->shape_info, new_info);
					free_pfbv_dfi(&(bi->shape_info));
					bi->shape_info = temp;
					//if (old_shape_info != bi->shape_info) {
					if(compare_pfbv_dfi(bi->shape_info,old_shape_info))
					{
						bi->new_shape_df = true;
						set_succ_for_processing = true;
					}

					free_pfbv_dfi(&old_shape_info);
				}
					else {
						csdfa_info temp = copy_csdfa_info (ai);
#ifdef MYDEBUG
						printf("\nIn propagate_pointsto_to_all_succs 4"); 
#endif		
						if (prev == NULL) {
							temp->next = in_of_bb (succ_bb);
							((bb_csipdfa_info)(succ_bb->aux))->in_ipdfa = temp;
						}
						else {
							temp->next = prev->next;
							prev->next = temp;
						}
						set_succ_for_processing = true;
					}
				}
			}
			if (set_succ_for_processing) {
#ifdef MYDEBUG
				printf("\nIn propagate_pointsto_to_all_succs 5--- bb%d added to wklist ",succ_bb->index); 
#endif
				set_bb_to_process_shape (cnode, succ_bb);
			}
		}

#ifdef MYDEBUG
	printf("\nIn propagate_pointsto_to_all_succs 6"); 
#endif
}
void perform_shapedfa_on_callblock()
{
#ifdef DUMP_IN_DUMPFILE
	fprintf(debug_file,"\t\tcallblock\n");
#endif
	csdfa_info ai;
	/* If the call block is the start block of main (), or not a start
	   block, copy in-info to out. */
	if (!start_block (cbb) || cbb == main_firstbb)
		copy_in_to_out ();
	else
		compute_mapped_out_at_startblock ();

	all_contexts_together = false;
	process_call_stmt (bb_call_stmt (cbb));
	all_contexts_together = true;
	propagate_shape_to_all_succs (cbb);
	/* Reset the new_shapedf flag for all out csdfa_info of bb. */
	for (ai=out_of_bb (cbb); ai; ai=ai->next)
		ai->new_shape_df = false;
	//    if (dump_file && !ipacs_time)
	//        print_bb_shape_info (cbb);
	//   
}
void perform_shapedfa_on_returnblock()
{
#ifdef DUMP_IN_DUMPFILE
	fprintf(debug_file,"\t\treturnblock %d\n",cbb->index);
#endif	
	csdfa_info ai;
	copy_in_to_out_1 ();
	propagate_shape_to_all_succs (cbb);
	/* Reset the new_ptadf flag for all out csdfa_info of bb. */
	for (ai=out_of_bb (cbb); ai; ai=ai->next)
		ai->new_shape_df = false;
	//    if (dump_file && !ipacs_time)
	//        print_bb_pinfo (cbb);

}

void perform_shapedfa_on_endblock()
{
#ifdef DUMP_IN_DUMPFILE
	fprintf(debug_file,"\t\tendbblock\n");
#endif	
	basic_block sp_block = start_bb_of_fn (cnode);

	copy_in_to_out ();
	if (sp_block != main_firstbb) {
		perform_regeneration_at_endblock (sp_block);  
		all_contexts_together = false;
		propagate_shape_to_returnblock ();
		all_contexts_together = true;
	}
	//    if (dump_file && !ipacs_time)
	//        print_bb_pinfo (cbb);
}

/* -----------------------------------------------------------------------
We had performed callstring mapping at the start block of a function. 
Now for rest of the intraprocedural basic blocks of that function, 
only the pointsto values associated with the representative callstrings 
will be processed. Therefore, at the end block, we need to regenerate 
the whole pointsto information using the pcsmap at the startblock. For 
eg, let the callstring map be {(ci -> di, ei), (fi->gi)}. The pointsto 
value associated with ci be val1, and with fi be val2. Thus the value 
at in of end block will be {(ci, val1), (fi, val2). Then the final 
regenerated value at the out of the end block will be {(ci, val1), 
(di, val1), (ei, val1), (fi, val2), (gi, val2)}. 
----------------------------------------------------------------------*/
void perform_regeneration_at_endblock (basic_block sp_bb)
{
csdfa_info ai, rep;
cs_map bi;
csmap_rhs rhs;
for (bi=pcsmap_of_cnode (cnode); bi; bi=bi->next) {
	/* Get the representative cs from out of endbb. */
	for (rep=out_of_bb (cbb); rep; rep=rep->next) {
		if (bi->lhs->cs_index > rep->cs_index)
			break;
		if (bi->lhs->cs_index == rep->cs_index) {
			for (rhs=bi->rhs; rhs; rhs=rhs->next) {
				csdfa_info regen = rhs->rhsel;
				if (rep->new_shape_df || regen->new_shape_df) {
					csdfa_info prev = NULL;

					/* Change the pointsto info of regen at out. */
					for (ai=out_of_bb (cbb); ai; ai=ai->next) {
						if (regen->cs_index >= ai->cs_index)
							break;
						prev = ai;
					}
					if (ai && (regen->cs_index == ai->cs_index)) {
						if (ai->new_shape_df == false) {
							//                                if (ai->pinfo != rep->pinfo) {
							if(compare_pfbv_dfi(ai->shape_info,rep->shape_info)){
								assignRightToLeft_shape_info(&(ai->shape_info),rep->shape_info);
								// 				   ai->pinfo = rep->pinfo;
								ai->new_shape_df = true;
							}
						}
						}

						else {
							//mine-- here the df value is copied from rep,and the cs_index of regen (i.e the one in the rhs is attached)
							csdfa_info temp = copy_csdfa_info (rep);
							temp->cs_index = regen->cs_index;
							temp->new_shape_df = true;
							if (prev == NULL) {
								temp->next = out_of_bb (cbb);
								((bb_csipdfa_info)(cbb->aux))->out_ipdfa = temp;
							}
							else {
								temp->next = prev->next;
								prev->next = temp;
							}
						}
					}
				}
			}
		}
	}
	/* Reset the in of sp_bb. */
	for (ai=in_of_bb (sp_bb); ai; ai=ai->next)
		ai->new_shape_df = false;
	//    if (dump_file && !ipacs_time)
	//        print_pcsmap_at_endblock (cbb);
}



static void printTheShape(gimple stmt,int *shapeArray
#ifdef DUMP_IN_DUMPFILE
                    , FILE *pFile
#endif
                    )
{
	int i;
#ifdef DUMP_IN_DUMPFILE
	print_gimple_stmt(pFile,stmt,1,1);
	fprintf(pFile,"\t\t\tBefore Stmt:\n");
#endif
	for(i=0;i<noOfHeapPtrs;i++)
	{
		int n=IN_STMT(current_pfbv_dfi_stmt,stmt)->attribute[i];
		int stmt_index=find_index_of_local_stmt(stmt);
		stmt_shape_map[stmt_index*2].stmt=stmt;
		if(n==2)
		{
			stmt_shape_map[stmt_index*2].shape[i]=2;
			shapeForStmt[1][stmt_index][i]=2;
		}
		else
		{
			if(n==1)
			{
				stmt_shape_map[stmt_index*2].shape[i]=1;
				shapeForStmt[1][stmt_index][i]=1;
			}
			else
			{
				stmt_shape_map[stmt_index*2].shape[i]=0;
				shapeForStmt[1][stmt_index][i]=0;
			}
		}

#ifdef DUMP_IN_DUMPFILE   
		switch(IN_STMT(current_pfbv_dfi_stmt,stmt)->attribute[i])
		{
			case 0:
				fprintf(pFile,"%s\t\tTREE\n",tableOfHeaps[i].ptrName);
				break;
			case 1:
				fprintf(pFile,"%s\t\tDAG\n",tableOfHeaps[i].ptrName);
				break;
			case 2:
				fprintf(pFile,"%s\t\tCYCLE\n",tableOfHeaps[i].ptrName);
				break;
		}
#endif
	}

#ifdef DUMP_IN_DUMPFILE
	fprintf(pFile,"\n\t\t\tAfter Stmt:\n");
#endif    
	for(i=0;i<noOfHeapPtrs;i++)
	{
		int n=OUT_STMT(current_pfbv_dfi_stmt,stmt)->attribute[i];
		int stmt_index=find_index_of_local_stmt(stmt);
		stmt_shape_map[stmt_index*2+1].stmt=stmt;
		if(n==2)
		{
			stmt_shape_map[stmt_index*2+1].shape[i]=2;
			shapeForStmt[1][stmt_index][i]=2;
		}
		else
		{
			if(n==1)
			{
				stmt_shape_map[stmt_index*2+1].shape[i]=1;
				shapeForStmt[1][stmt_index][i]=1;
			}
			else
			{
				stmt_shape_map[stmt_index*2+1].shape[i]=0;
				shapeForStmt[1][stmt_index][i]=0;
			}
		}
		shapeArray[i]=shapeForStmt[1][stmt_index][i];
#ifdef DUMP_IN_DUMPFILE    
		switch(OUT_STMT(current_pfbv_dfi_stmt,stmt)->attribute[i])
		{
			case 0:
				fprintf(pFile,"%s\t\tTREE\n",tableOfHeaps[i].ptrName);

				break;
			case 1:
				fprintf(pFile,"%s\t\tDAG\n",tableOfHeaps[i].ptrName);

				break;
			case 2:
				fprintf(pFile,"%s\t\tCYCLE\n",tableOfHeaps[i].ptrName);
				break;
		}
#endif    
	}

#ifdef DUMP_IN_DUMPFILE
	fprintf(pFile,"\n\n");
#endif  
}

static 
void computeAttributeArray(dataFlowValue **out,dataFlowValue *in,gimple stmt,int stmtType)
{
	int i,pId,qId;

	tree lhsop=NULL,rhsop1=NULL;
	if (is_gimple_assign (stmt))
	{
		lhsop = gimple_assign_lhs (stmt);
		rhsop1 = gimple_assign_rhs1 (stmt);

	}
    if (stmtType==1)
        lhsop = gimple_assign_lhs (stmt);

	//copy all attribute array of into out
	for(i=0;i<noOfHeapPtrs;i++)
	{
		((*out)->attribute)[i]=(in->attribute)[i];
	}

	if(stmtType==1 || stmtType==2)
	{
		pId=getId(getTheName(lhsop),cnode);
		((*out)->attribute)[pId]=TREE;
	}
	if((stmtType>=3 && stmtType<=5) || stmtType==31 )	
	{
		if(stmtType == 31)
		{
		  	pId=getId(getTheName(lhsop),cnode);
			qId=getId(getTheName(rhsop1),cnode);		
			

			
		}
		else
		{	
			pId=getId(getTheName(lhsop),cnode);
			qId=findNameAndGetId_q(stmtType,stmt);
		}

		((*out)->attribute)[pId]=(in->attribute)[qId];
	}
	if(stmtType==6)
	{
		//doesnot change
		pId=getId(getTheName(TREE_OPERAND(TREE_OPERAND(lhsop,0),0)),cnode);
		((*out)->attribute)[pId]=(in->attribute)[pId];
	}
	//p->f=q or p->f[]=q
	if(stmtType==7 || stmtType==71)
	{

		if(stmtType==7)
		{
		    pId=getId(getTheName(TREE_OPERAND(TREE_OPERAND(lhsop,0),0)),cnode);
		    qId=getId(getTheName(rhsop1),cnode);
		}
		else
		{		 			  
		  pId=getId(getTheName(TREE_OPERAND(TREE_OPERAND(TREE_OPERAND(lhsop,0),0),0)),cnode);
		  qId=getId(getTheName(rhsop1),cnode);		  
		}
		
		for(i=0;i<noOfHeapPtrs;i++)
		{
			if( (in->direction)[i][qId] || (in->direction)[i][pId])
				if((in->direction)[qId][pId])
					((*out)->attribute)[i]=CYCLE;
			if( (in->interference)[i][qId] && (in->direction)[i][pId] )
				if( !(in->direction)[qId][pId] && ( (in->attribute)[qId]==TREE))
					((*out)->attribute)[i]=( (in->attribute)[i]>DAG ? (in->attribute)[i]:DAG  );
			if((in->direction)[i][pId])
				if( !(in->direction)[qId][pId] && ( (in->attribute)[qId]!=TREE) )
					((*out)->attribute)[i]=( (in->attribute)[i]>(in->attribute)[qId] ? (in->attribute)[i]:(in->attribute)[qId]  );
		}  
	}

	      //p=q->f and p=q->f[]
	if(stmtType==8 || stmtType==81)	  
	{
		pId=getId(getTheName(lhsop),cnode);
		qId=findNameAndGetId_q(stmtType,stmt);
		((*out)->attribute)[pId]=(in->attribute)[qId];
	}
}



csdfa_info compute_shape_info(basic_block bb,csdfa_info ai)
{
	gimple_stmt_iterator gsi;
	gimple stmt;
	int statementType;
	dataFlowValue *new_in_dfvalue,*new_out_dfvalue;
	initialize_some_dataFlowValue(&new_in_dfvalue);
#ifdef MYDEBUG_1
	printf("\n In compute_shape_info---1");
#endif
	int *shapeArray_temp=(int*)xmalloc(noOfHeapPtrs*sizeof(int));
	shapeArray_temp = shapeArray_temp;
	//   assignRightToLeft_shapeArray(shapeArray_temp,ai->shapeArray);
	assignRightToLeft_dataFlowValue(new_in_dfvalue,ai->shape_info->gen);
#ifdef DUMP_IN_DUMPFILE	
	pFile = fopen ("shape.txt","a");
	fprintf(pFile,"\n----------------------------------------------------------------------------------\n\n");
	fprintf(pFile,"\n----BB %d of %s,callstring=",bb->index,cgraph_node_name(cnode));
	print_callstring_sequence(ai->cs_index,pFile);
	fclose(pFile);

	fprintf(dump_file,"\n --------------bb %d of %s,callstring=",bb->index,cgraph_node_name(cnode));
	print_callstring_sequence(ai->cs_index,dump_file);
#endif

#ifdef MYDEBUG_1
	printf("\n In compute_shape_info---2");
#endif  

	for (gsi = gsi_start_bb (bb); !gsi_end_p (gsi); gsi_next (&gsi)) {
		stmt = gsi_stmt (gsi);
        
		if((statementType=isItInThe8Stmts(stmt)))
		{
#ifdef MYDEBUG_1
			print_gimple_stmt(stdout,stmt,1,1);  
#endif		  
#ifdef DUMP_IN_DUMPFILE
			fprintf(dump_file,"\n\n\n");
			print_gimple_stmt(dump_file,stmt,1,1);
#endif		  
			initialize_some_dataFlowValue(&(IN_STMT(current_pfbv_dfi_stmt,stmt)));
			initialize_some_dataFlowValue(&(OUT_STMT(current_pfbv_dfi_stmt,stmt)));
			initialize_some_dataFlowValue(&(GEN_STMT(current_pfbv_dfi_stmt,stmt)));
			initialize_some_dataFlowValue(&(KILL_STMT(current_pfbv_dfi_stmt,stmt)));
			initialize_some_dataFlowValue(&new_out_dfvalue);
			assignRightToLeft_dataFlowValue(IN_STMT(current_pfbv_dfi_stmt,stmt),new_in_dfvalue);
			generateGENKILLvalues(statementType,stmt,IN_STMT(current_pfbv_dfi_stmt,stmt));

			assignRightToLeft_dataFlowValue(new_out_dfvalue,transfer_fn(GEN_STMT(current_pfbv_dfi_stmt,stmt), new_in_dfvalue, KILL_STMT(current_pfbv_dfi_stmt,stmt)));
			computeAttributeArray(&new_out_dfvalue,new_in_dfvalue,stmt,statementType); //  calculate the attribute array(i.e shape) values.

			assignRightToLeft_dataFlowValue(OUT_STMT(current_pfbv_dfi_stmt,stmt),new_out_dfvalue);
			assignRightToLeft_dataFlowValue(new_in_dfvalue,OUT_STMT(current_pfbv_dfi_stmt,stmt));

#ifdef DUMP_IN_DUMPFILE
			fprintf(dump_file,"\nIN    %d\n",statementType);
			printDataFlow(0,dump_file,IN_STMT(current_pfbv_dfi_stmt,stmt));
			printDataFlow(1,dump_file,IN_STMT(current_pfbv_dfi_stmt,stmt));
			printAttribute(dump_file,IN_STMT(current_pfbv_dfi_stmt,stmt)->attribute);
			fprintf(dump_file,"\nOUT    %d\n",statementType);
			printDataFlow(0,dump_file,OUT_STMT(current_pfbv_dfi_stmt,stmt));
			printDataFlow(1,dump_file,OUT_STMT(current_pfbv_dfi_stmt,stmt));
			printAttribute(dump_file,OUT_STMT(current_pfbv_dfi_stmt,stmt)->attribute);
			pFile=fopen("shape.txt","a");
			fprintf(pFile,"\n");
#endif			
			printTheShape(stmt, shapeArray_temp
#ifdef DUMP_IN_DUMPFILE
                          , pFile
#endif			
                          ); 
#ifdef DUMP_IN_DUMPFILE
			fclose(pFile);
			pFile = fopen ("shape.txt","a");
			fprintf(pFile,"\n----------------------------------------------------------------------------------\n\n");
			fclose(pFile);
			fprintf(dump_file,"\n--------------------------------------------------------------\n"); 
#endif
			free_dataFlowValue(&GEN_STMT(current_pfbv_dfi_stmt,stmt));
			free_dataFlowValue(&IN_STMT(current_pfbv_dfi_stmt,stmt));
			free_dataFlowValue(&OUT_STMT(current_pfbv_dfi_stmt,stmt));
			free_dataFlowValue(&new_out_dfvalue);
		}
	}
#ifdef DUMP_IN_DUMPFILE	
	pFile = fopen ("shape.txt","a");
	fprintf(pFile,"\n----------------------------------------------------------------------------------\n\n");
	fclose(pFile);
#endif	

#ifdef MYDEBUG_1
	printf("\n In compute_shape_info---2");
#endif

	//csdfa_info with only gen //memory only for it is allocated	
	csdfa_info temp = (csdfa_info) alloc_mem (sizeof (struct cs_dfa));
	temp->shape_info=(pfbv_dfi *) xmalloc(sizeof(pfbv_dfi));
	initialize_some_dataFlowValue(&(temp->shape_info->gen));
	assignRightToLeft_dataFlowValue(temp->shape_info->gen,new_in_dfvalue);

	// 	 temp->shapeArray=shapeArray_temp;
	temp->new_shape_df =ai->new_shape_df;
	temp->cs_index=ai->cs_index;
	temp->next=NULL;
	free_dataFlowValue(&new_in_dfvalue);
	return temp; 
}


void compute_out_from_csmap_intra()
{

	csdfa_info ai, di,result;
	cs_map bi;
	/* copy the mapped points-to info and process its dfvalue inside the intrablock 
	    and write it to the outcsipdfa. */
	//mine-- the lhs part of the pcsmap is copied to the out of that bb
	//if there is any rhs for that pcsmap entry then at the and of this bb we need to regenerate it!!!!  as done by the function perform_regeneration_at_endblock
	for (bi=pcsmap_of_cnode (cnode); bi; bi=bi->next) {
		ai = bi->lhs;
		if (ai->new_shape_df) {
			result=compute_shape_info(cbb,ai);  //dfvalue after processing intrablock
			csdfa_info prev = NULL;
			bi->present_in_out = true;
			for (di=out_of_bb (cbb); di; di=di->next) {
				if (ai->cs_index >= di->cs_index)
					break;
				prev = di;
			}
			if (di && (di->cs_index == ai->cs_index)) {
				//di->shape_info=result->shape_info
				assignRightToLeft_shape_info(&(di->shape_info),result->shape_info);
				di->new_shape_df = true;
			}
			else {

				csdfa_info ci = copy_csdfa_info (result);

				if (prev == NULL) {
					ci->next = out_of_bb (cbb);
					((bb_csipdfa_info)(cbb->aux))->out_ipdfa = ci;
				}
				else {
					ci->next = prev->next;
					prev->next = ci;
				}
			}
			if (!pcsmap_required (cnode))
				ai->new_shape_df = false;
		}
	}
	if (pcsmap_required (cnode)) {    //this would be true if there is any rhs present in that case regeneration is reqd for which endblock should be added to the worklist
		basic_block end_bb = end_bb_of_fn (cnode);
		if (end_block (end_bb))
			set_bb_to_process_shape (cnode, end_bb);
	}
#ifdef DUMP_IN_DUMPFILE	
	/* Dump the map in the dump_file */
	//   if (dump_file && !ipacs_time)   //code changed
	print_pcsmap_at_startblock (cbb,dump_file);
#endif	

}
void compute_out_from_in_intra()
{
	csdfa_info ai, bi=NULL,result;
	for (ai=in_of_bb (cbb); ai; ai=ai->next) {
		if (ai->new_shape_df) {
			result=compute_shape_info(cbb,ai);  //dfvalue after processing intrablock
			csdfa_info prev = NULL;
			for (bi=out_of_bb (cbb); bi; bi=bi->next) {
				if (ai->cs_index >= bi->cs_index)
					break;
				prev = bi;
			}
			if (bi && (bi->cs_index == ai->cs_index)) {

#ifdef MYDEBUG
				printf("\n in the if of compute_out_from_in_intra ");
#endif
				//bi->shape_info=result->shape_info
				assignRightToLeft_shape_info(&(bi->shape_info),result->shape_info);
				bi->new_shape_df = true;
			}
			else {
#ifdef MYDEBUG
				printf("\n in the else of compute_out_from_in_intra ---%d",cbb->index);
#endif
				csdfa_info temp = copy_csdfa_info (result);
				if (prev == NULL) {
					temp->next = out_of_bb (cbb);
					((bb_csipdfa_info)(cbb->aux))->out_ipdfa = temp;
				}
				else {
					temp->next = prev->next;
					prev->next = temp;		   
				}
			}
			ai->new_shape_df = false;
		}
	}
}


void perform_shapedfa_on_intrablock()
{
#ifdef DUMP_IN_DUMPFILE
	fprintf(debug_file,"\t\tintrablock\n");
#endif	
#ifdef MYDEBUG
	printf("\n In perform_shapedfa_on_intrablock  -- bb%d",cbb->index);
#endif
	csdfa_info ai;
	/* If it is not a start block, or a start block of main, copy in-info to out. */
	if (!start_block (cbb)) {
#ifdef MYDEBUG
		printf("\n in perform_shapedfa_on_intrablock %d-------1\n",cbb->index);
#endif
		compute_out_from_in_intra();
	}
	else {
		if (cbb == main_firstbb)
		{    
#ifdef MYDEBUG
			printf("\n in perform_shapedfa_on_intrablock ------2");
#endif
			compute_out_from_in_intra();
		}   
		else {
#ifdef MYDEBUG
			printf("\n in perform_shapedfa_on_intrablock -------3");
#endif
			/* Compute mapped out at start block. */
			compute_mapped_out_at_startblock ();  //may have to replace this by compute_out_from_csmap_intra();
#ifdef DUMP_IN_DUMPFILE
			print_pcsmap_at_startblock(cbb,debug_file);
#endif
		}
	}
	propagate_shape_to_all_succs (cbb);
	for (ai=out_of_bb (cbb); ai; ai=ai->next)
		ai->new_shape_df = false;
	//    if (dump_file && !ipacs_time)
	//        print_bb_pinfo (cbb);
}

void printAttribute(FILE *pFile,shape *attribute)
{
	int i;
	fprintf(pFile,"\nShape Info\n");
	for(i=0;i<noOfHeapPtrs;i++)
	{
		switch(attribute[i])
		{
			case 0:
				fprintf(pFile,"%s\t\tTREE\n",tableOfHeaps[i].ptrName);

				break;
			case 1:
				fprintf(pFile,"%s\t\tDAG\n",tableOfHeaps[i].ptrName);

				break;
			case 2:
				fprintf(pFile,"%s\t\tCYCLE\n",tableOfHeaps[i].ptrName);

				break;
		}

	}
}

/*-------------------------------------------------------------------
  Get the basic block of the cnode that needs to be processed. 
  If there is no node to be processed, or the node is a stall node,
  return NULL.
  ------------------------------------------------------------------*/
basic_block
get_bb_for_shape_processing (bb_worklist list, bool *new_cnode)
{
	bool any_bit_set = false;
	int i;
	int num_bb = n_basic_blocks_for_function (DECL_STRUCT_FUNCTION (cnode->decl)) - NUM_FIXED_BLOCKS;   //mine--NUM_FIXED_BLOCKS=2
	for (i=0; i<=num_bb; i++) {
		if (list[i].process_shape == true) {
			if (shape_callee_nodes_processed ()) {   //mine-- if all nodes in workist shape_node are processed
				list[i].process_shape = false;
				return list[i].bb;
			}
			else {
				if (single_pred_p (list[i].bb) && !return_block (list[i].bb)        //mine--- reason for this?????????????
						&& !end_block (list[i].bb)) {
					list[i].process_shape = false;
					return list[i].bb;
				}
			}
			any_bit_set = true;
		}
	}
	// any_bit_set ==FALSE if all the nodes in the bb_worklist(i.e  list) are processed
	if (!any_bit_set) {
		cnode = prev_node_from_shape_worklist ();
		*new_cnode = true;
	}
	return NULL;
}


bool shape_callee_nodes_processed (void)
{
	worklist tmp;
	for (tmp=shape_node->next; tmp; tmp=tmp->next) {
		if (tmp->process_shape)
			return false;
	}
	return true;
}


/* Return a node from shape worklist whose successors have been processed. */
struct cgraph_node * prev_node_from_shape_worklist (void)
{
	worklist ai, bi = NULL;

	/* Reset the process_shape flag of shape_node. */
	shape_node->process_shape = false;
	for (ai=start; ai!=shape_node; ai=ai->next) {
		if (ai->process_shape)
			bi = ai;
	}
	shape_node = bi;
	if (shape_node)
		return shape_node->called_fn;
	return NULL;
}

/* Get a node from shape worklist. */
struct cgraph_node *next_node_from_shape_worklist (worklist node)
{
	worklist tmp;
	for (tmp=node->next; tmp; tmp=tmp->next) {
		if (tmp->process_shape) {
			shape_node = tmp;
			//shouldn't it be tmp->process_shape=FALSE
			return tmp->called_fn;
		}
	}
	return NULL;
}


struct cgraph_node * first_node_from_shape_worklist()
{
	// we could directly extract shape_node->called_fn   , as initially start=shape_node
	worklist tmp;
	for (tmp=start; tmp; tmp=tmp->next) {
		if (tmp->process_shape) {
			shape_node = tmp;
			return tmp->called_fn;
		}
	}
	return NULL;
}


csdfa_info initialize_csdfa_info()
{
	csdfa_info temp = (csdfa_info) alloc_mem (sizeof (struct cs_dfa));
	temp->shape_info=(pfbv_dfi *) xmalloc(sizeof(pfbv_dfi));
	initialize_some_dataFlowValue_malloc(&(temp->shape_info->gen));	    
	temp->next=NULL;	    
	return temp;
}

/*----------------------------------------------------------------------------
  Check if the given callstring already exists in the callstrings at in of bb.
  If it does, return the index from the pool, else return -1.
  ---------------------------------------------------------------------------*/
int check_callstring_existance (callstring cs, basic_block bb, bool in_ipdfa)
{
	csdfa_info ai = (in_ipdfa) ? in_of_bb (bb) : out_of_bb (bb);
	for (; ai; ai=ai->next) {
		callstring bi = VEC_index (callstring, callstrings, ai->cs_index);
		if (callstrings_equal (bi, cs))
			return ai->cs_index;
	}
	return -1;
}

/*---------------------------------------------
  Check if the given two callstrings are equal.
  --------------------------------------------*/
bool callstrings_equal (callstring a, callstring b)
{
	while (a) {
		if (b == NULL)
			return false;
		if (a->bb != b->bb)
			return false;
		a = a->next;
		b = b->next;
	}
	if (b)
		return false;
	return true;
}

csdfa_info get_representative_csdfa (basic_block bb, callstring *cs)
{
	csdfa_info bi, rep_csdfa = NULL;
	int max_len = 0;

	for (bi=in_of_bb (bb); bi; bi=bi->next) {
		int len = callstring_is_substring (cs, bi->cs_index);
#ifdef MYDEBUG
		printf("\n In get_representative_csdfa,  len= %d",len);      
#endif       
		if (len > max_len) {
			rep_csdfa = bi;   
			max_len = len;
		}
	}
	return rep_csdfa;
}

/* Find if callstring indexed at index is a substring of cs. */
int callstring_is_substring (callstring *cs, int index)
{
	callstring bi = (index == -1) ? NULL : VEC_index (callstring, callstrings, index);
	int len_ai = callstring_length (*cs);
	int len_bi = callstring_length (bi);

#ifdef MYDEBUG
	printf("\n In callstring_is_substring --length of cs=%d ,in->cs= %d",len_ai,len_bi);
	print_callstring_sequence(index,stdout);
#endif

	if (len_ai > len_bi) {
		int diff = len_ai - len_bi;
		callstring ci;
		for (ci=*cs; ci && diff>0; ci=ci->next)
			diff--;
		while (ci) {
			if (!bi)
				break;
			if (ci->bb != bi->bb)
				return 0;
			ci = ci->next;
			bi = bi->next;
		}
		if (ci || bi)
			return 0;
		return len_bi;
	}
	return 0;
}

int callstring_length (callstring ai)
{
	int len = 0;
	for (; ai; ai=ai->next)
		len++;
	return len;
}

/* Create a new callstring map cell. */                                   
cs_map                                                             
	create_new_csmap_cell (csdfa_info ai)                
	{                                                                         
		cs_map temp = (cs_map) alloc_mem (sizeof (struct cs_map_def));         
		temp->lhs = ai;                                                        
		//    temp->present_in_out = present_in_out;                                 
		return temp;                                                           
	}                                                                         

csmap_rhs                                                          
	create_csmap_rhs (csdfa_info ai)                     
	{                                                                         
		csmap_rhs newrhs = (csmap_rhs) alloc_mem (sizeof (struct csmap_rhs_def));      
		newrhs->rhsel = ai;                                                    
		//    newrhs->present_in_out = present_in_out;                               
		return newrhs;
	} 
void exchange_csmap_entries (cs_map *ai, csmap_rhs *bi)
{
	if((*bi)->rhsel->new_shape_df==1)
		return;

	csdfa_info temp = (*ai)->lhs;
	//    bool tempbool = (*ai)->present_in_out;

	(*ai)->lhs = (*bi)->rhsel;
	//    (*ai)->present_in_out = (*bi)->present_in_out;

	(*bi)->rhsel = temp;
	//    (*bi)->present_in_out = tempbool;
}

