#include "all_includes_defines.h"

VEC(callstring, heap) *callstrings;    //pointer of struct VEC_callstring_heap

/*----------------------------------*/

/* Initialize the bbaux info for each basic block. */
//mine-- struct bb_csipdfa_info_def is present in every basic_block structure.It has the dfa info associated with tthat basic_block along with callString info.

 void
initialize_bbaux_info (struct cgraph_node *node, basic_block bb)
{
   bb->aux = (void *) alloc_mem (sizeof (struct bb_csipdfa_info_def));
   ((bb_csipdfa_info)(bb->aux))->rp_index = -1;
   ((bb_csipdfa_info)(bb->aux))->cnode = node;
   ((bb_csipdfa_info)(bb->aux))->call_block = false;
   ((bb_csipdfa_info)(bb->aux))->return_block = false;
   ((bb_csipdfa_info)(bb->aux))->start_block = false;
   ((bb_csipdfa_info)(bb->aux))->end_block = false;
   ((bb_csipdfa_info)(bb->aux))->has_alias = false;
   //these are added by me
   ((bb_csipdfa_info)(bb->aux))->in_ipdfa=NULL;
   ((bb_csipdfa_info)(bb->aux))->out_ipdfa=NULL;
}

 gimple_stmt_iterator
split_bb_at_stmt (gimple stmt)
{

/* mine-- split_block---->Splits basic block BB after the specified instruction I (but at least after
   the labels).  If I is NULL, splits just after labels.  The newly created edge
   is returned.  The new basic block is created just after the old one.  */
   edge e = split_block (cbb, stmt);
   cbb = e->dest;
   initialize_bbaux_info (cnode, cbb);
   return gsi_start_bb (cbb);
}

/* returns true if the statement is the end block's return statement. */
bool is_gimple_endblock (gimple t)
{
    return (gimple_code (t) == GIMPLE_RETURN);
}

basic_block
start_bb_of_fn (struct cgraph_node *node)          
{
    return ENTRY_BLOCK_PTR_FOR_FUNCTION (DECL_STRUCT_FUNCTION (node->decl))->next_bb;
}


/*--------------------------------------------------------------------
  Returns the called function's decl tree. If it is a direct function 
  call, the TREE_CODE of the returned decl will be FUNCTION_DECL. If 
  it is a call via function pointer, it will be VAR_DECL. 
  -------------------------------------------------------------------*/
 tree
get_called_fn_decl (gimple stmt)
{
    /* If we can resolve it here, its a simple function call. */
    tree decl = gimple_call_fndecl (stmt);
    /* The call is through function pointer. */
    if (!decl)
        decl = gimple_call_fn (stmt);
    return decl;
}

void * ggc_internal_cleared_alloc_stat (size_t size MEM_STAT_DECL)
{
  return xcalloc (size, 1);
}

/* Return the node to which the basic block belongs. */
struct cgraph_node * cnode_of_bb (basic_block bb)
{
   return ((bb_csipdfa_info)(bb->aux))->cnode;
}

/* Initialize the cgraphaux info for each cgraph node. */
 void
initialize_cgraphaux_info (struct cgraph_node *node, basic_block endblock,
                           int *rp, int num_bb)
{
   int i;
   basic_block bb;
   //mine-- bb_worklist is an array of struct bb_worklist_def
   bb_worklist bb_list = XNEWVEC (struct bb_worklist_def, num_bb+1);
   /* mine-- (struct cgraph_info_def) is an  Auxillary data structure associated with each cgraph node. This data
   structure stores the callstring map and the array of blocks to be
   processed */
   node->aux = (void *) alloc_mem (sizeof (struct cgraph_info_def));
   for (i=0; i<num_bb; i++) {
       bb = BASIC_BLOCK (rp[i]);
       set_rp_index (bb, i);
       bb_list[i].bb = bb;
       bb_list[i].process_shape = false;
   }

   /* Make the endblock the last block in reverse postorder. */
   set_rp_index (endblock, i);

   bb_list[i].bb = endblock;
   bb_list[i].process_shape = false;

   ((cgraph_info)(node->aux))->bb_list = bb_list;
   ((cgraph_info)(node->aux))->pcsmap_required = false;
}

 void
set_rp_index (basic_block bb, int i)
{
   ((bb_csipdfa_info)(bb->aux))->rp_index = i;
}

/* Returns true if the basic block is a return block. */
 bool                
return_block (basic_block bb)
{                          
   return ((bb_csipdfa_info)(bb->aux))->return_block;
}

/* Returns true if the basic block is an end block (i.e, has the statement 'return'
   of the cfg). */
 bool        
end_block (basic_block bb)
{              
   return ((bb_csipdfa_info)(bb->aux))->end_block;
} 

/* Returns true if the basic block is a call block. */
 bool                
call_block (basic_block bb) 
{                          
   return ((bb_csipdfa_info)(bb->aux))->call_block;
}

 void  preprocess_Cfg()
{
  bool is_start_block;
  for (cnode=cgraph_nodes; cnode; cnode=cnode->next) {
       int *rp;
       basic_block endblock = NULL;
       /* Nodes without a body, and clone nodes are not interesting. */
       if (!gimple_has_body_p (cnode->decl) || cnode->clone_of)
          continue;
        push_cfun (DECL_STRUCT_FUNCTION (cnode->decl));
	is_start_block = true;
	FOR_EACH_BB (cbb) {
	 
	   gimple_stmt_iterator gsi;
	
	 if (!cbb->aux)
               initialize_bbaux_info (cnode, cbb);
	 
	 if (is_start_block) {
               ((bb_csipdfa_info)(cbb->aux))->start_block = true;
               is_start_block = false;
	   }
   
	   for (gsi = gsi_start_bb (cbb); !gsi_end_p (gsi); gsi_next (&gsi)) {
	       gimple stmt = gsi_stmt (gsi);

               /* Break at boundaries of callbb and returnbb. */
	       if (is_gimple_call (stmt) || is_gimple_endblock (stmt)) {
                   gimple_stmt_iterator origgsi = gsi;
                   tree decl = NULL;

                   /* Need not break in case of library routines. */
                   if (is_gimple_call (stmt)) {
                       decl = get_called_fn_decl (stmt);   //mine-- in this statement a function is called its decl is returned
                       if (TREE_CODE (decl) == FUNCTION_DECL) {
                           if (!DECL_STRUCT_FUNCTION (decl)) {
			       //process_library_call (stmt);    //mine--it also has some liveness generation...not needed for my case!!!! we can ignore or handle only "malloc"  ?????????????????????????
				     continue;
                               //continue;
                           }
                       }
                   }
                   
                    gsi_prev (&gsi);
		   /* Check if the call/return stmt is the first stmt in the bb. */
		   if (gsi_end_p (gsi) || gimple_code (gsi_stmt (gsi)) == GIMPLE_LABEL) {
		    gsi = origgsi;
		   }
		   else
		   {
		     /* Split before the call/return stmt. */
			gsi = split_bb_at_stmt (gsi_stmt (gsi));
                   }
			
                   /* Split the call into a new bb if its not the last stmt. */
                   origgsi = gsi;
                   gsi_next (&gsi);
                   if (!is_gimple_endblock (stmt) && !gsi_end_p (gsi)) {
                       gsi = origgsi;
                       split_block (cbb, gsi_stmt (gsi));
                   }
		   else 
                       gsi = origgsi;
		   
		   if (is_gimple_call (stmt)) {
		     ((bb_csipdfa_info)(cbb->aux))->call_block = true; 
			 gsi = split_bb_at_stmt (gsi_stmt (gsi));
                       ((bb_csipdfa_info)(cbb->aux))->return_block = true;
                       break;
                   }
                   
                   if (is_gimple_endblock (stmt)) {
                       ((bb_csipdfa_info)(cbb->aux))->end_block = true;
                       endblock = cbb;
                   }
               }
	   }
	   }
	    /* If there was no endblock, mark it. */
       if (!endblock)
           endblock = EXIT_BLOCK_PTR_FOR_FUNCTION (DECL_STRUCT_FUNCTION (cnode->decl))->prev_bb;
       rp=(int *)xmalloc(sizeof(int)*total_bbs);
                
       
       //mine-- reverse post order--Numbering in ths is such that the parent is numbered before any of its children   (
       //mine-- preorder is same as smple dfs ordering of a fgraph from root.
       //false:meaning the entry and exit block are not considered
       pre_and_rev_post_order_compute (NULL, rp, false); //mine--This func is implemented in some other file cfganal.c   The preorder and reverse-post order of cfg is calculated

       /* Initialize the cgraph info. */
       //mine--the aux field of cgraph_node is allocated for struct cgraph_info_def   and memory allocated for it
       //mine-- the basicBlock worklist is created and inserted into the aux field!!!!!!!!!!!
       initialize_cgraphaux_info (cnode, endblock, rp, total_bbs);
       free (rp);
       
    pop_cfun ();
   }
}

static
void free_csmap_rhs(csmap_rhs cmr)
{
    if (!cmr)
        return;
    free_csmap_rhs(cmr->next);
    // free(cmr);
}

static
void free_csmap(cs_map cm)
{
    if (!cm)
        return;
    free_csmap(cm->next);
    free_csmap_rhs(cm->rhs);
    //free(cm);
}

static void
free_csdfa_info(csdfa_info* ci) 
{
    if (!*ci)
	return;

    // recursively delete rest of the info.
    free_csdfa_info(&(*ci)->next);
    free_pfbv_dfi(&(*ci)->shape_info);
    // free(*ci);
    *ci = NULL;
}


static void
free_cgraph_info(cgraph_info* cgaux) 
{
    XDELETEVEC((*cgaux)->bb_list);
    free_csmap((*cgaux)->pcsmap);
    //free(*cgaux);
    *cgaux = NULL;
}

static void
free_csipdfa_info(bb_csipdfa_info* bbaux) 
{
    csdfa_info in = (*bbaux)->in_ipdfa;
    csdfa_info out= (*bbaux)->out_ipdfa;
    free_csdfa_info(&in);
    free_csdfa_info(&out);
    // free(*bbaux);
    *bbaux = NULL;
}

void restore_Cfg (void)
{
    for (cnode = cgraph_nodes; cnode; cnode = cnode->next) {
        if (!gimple_has_body_p (cnode->decl) || cnode->clone_of)
            continue;
        current_function_decl = cnode->decl;
        set_cfun (DECL_STRUCT_FUNCTION (cnode->decl));
        FOR_EACH_BB (cbb) {
            if(cbb->aux) {
                bb_csipdfa_info aux = (bb_csipdfa_info)(cbb->aux);
                free_csipdfa_info (&aux);
                cbb->aux = NULL;
            }
        }
        if(cnode->aux) {
            cgraph_info aux = (cgraph_info)(cnode->aux);
            free_cgraph_info (&aux);
            cnode->aux = NULL;
        }
        
        /* Merge bb's if necessary. */
        cleanup_tree_cfg ();
        /* Free the dominator info. */
        free_dominance_info (CDI_DOMINATORS);
        free_dominance_info (CDI_POST_DOMINATORS);
    }
 }

#ifdef DUMP_IN_DUMPFILE
void print_cfg()
{
  
  for (cnode=cgraph_nodes; cnode; cnode=cnode->next) {
  
    if (!gimple_has_body_p (cnode->decl) || cnode->clone_of)
          continue;
    push_cfun (DECL_STRUCT_FUNCTION (cnode->decl));
    fprintf(dump_file,"\t\t\tFunction : %s\n",current_function_name());
    FOR_EACH_BB(cbb)
    {
      gimple_stmt_iterator gsi;
      fprintf(dump_file,"\nbb no: %d\n",cbb->index);
       for (gsi = gsi_start_bb (cbb); !gsi_end_p (gsi); gsi_next (&gsi)) {
	       gimple stmt = gsi_stmt (gsi);
	       print_gimple_stmt(dump_file,stmt,1,1);
          }
    }
    fprintf(dump_file,"---------------------------------------------------------------------------------------\n");
    brief_dump_cfg (dump_file);
    fprintf(dump_file,"***************************************************************************************\n");
    pop_cfun();
  }
      
}
#endif

/* Return a copy of the given pointsto information. */
csdfa_info
copy_csdfa_info (csdfa_info ci)
{
   csdfa_info temp = (csdfa_info) alloc_mem (sizeof (struct cs_dfa));
   initialize_pfbv_dfi(&(temp->shape_info));
   
   assignRightToLeft_shape_info(&(temp->shape_info),ci->shape_info);
   temp->cs_index = ci->cs_index;
   temp->new_shape_df = ci->new_shape_df;
   return temp;
}

void set_bb_to_process_shape (struct cgraph_node *node, basic_block bb)
{
   bb_worklist bb_list = worklist_of_cnode (node);
   int index = get_rp_index (bb);
   bb_list[index].process_shape = true;
}

void set_in_shape_worklist (struct cgraph_node *called_fn)
{
   worklist tmp;
   for (tmp=shape_node; tmp; tmp=tmp->next) {
      if (tmp->called_fn == called_fn) {
          tmp->process_shape = true;
          return;
       }
   }
   /* Create a new worklist node, and add it to the worklist. */
   tmp = new_worklist_node (called_fn);
   tmp->next = shape_node->next;
   shape_node->next = tmp;
}


void assignRightToLeft_shapeArray(int *left,int *right)
{
  int i;
  
  for(i=0;i<noOfHeapPtrs;i++)
    left[i]=right[i];
}

//propagation of shapeArray
void propagate_RightToLeft_shapeArray(int *left,int *right)
{
  int i;
  for(i=0;i<noOfHeapPtrs;i++)
    left[i]=(left[i]>right[i])?left[i]:right[i];
}


/* Return the in_ipdfa of the basic block. */
csdfa_info in_of_bb (basic_block bb)
{
   return ((bb_csipdfa_info)(bb->aux))->in_ipdfa;
}

/* Return the out_ipdfa of the basic block. */
csdfa_info out_of_bb (basic_block bb)
{
   return ((bb_csipdfa_info)(bb->aux))->out_ipdfa;
}

/* Returns true if the basic block is a start block. */
bool start_block (basic_block bb)
{
   return ((bb_csipdfa_info)(bb->aux))->start_block;
}

gimple bb_call_stmt (basic_block bb)
{
   gimple_stmt_iterator gsi;
   gimple stmt;
   for (gsi = gsi_start_bb (bb); !gsi_end_p (gsi); gsi_next (&gsi)) {
      stmt = gsi_stmt (gsi);
      if (is_gimple_call (stmt))
         return stmt;
   }
   return NULL;
}

/* Returns true if the block has no pointer statements, is neither
   call nor end block, and has a single predecessor. Such blocks
   can be bypassed. */
bool shape_bypass_block (basic_block bb)
{          
   return ((bb_csipdfa_info)(bb->aux))->shape_bypass_block;
}  


/* Return the pcsmap of the cgraph node. */
 cs_map pcsmap_of_cnode (struct cgraph_node *node)
{
   return ((cgraph_info)(node->aux))->pcsmap;
}

 void print_pcsmap_at_startblock (basic_block bb,FILE *dump_file)
{
   cs_map ai;
   if (!pcsmap_required (cnode_of_bb (bb)))
       fprintf (dump_file, "\nNo pointsto map required at %s node's %d block \n",
                cgraph_node_name (cnode_of_bb (bb)), bb->index);
   else {
       fprintf (dump_file, "\npointsto mapping performed at %s node's %d block:\n",
                cgraph_node_name (cnode_of_bb (bb)), bb->index);
       for (ai=pcsmap_of_cnode (cnode_of_bb (bb)); ai; ai=ai->next) {
           csmap_rhs bi = ai->rhs;
	   fprintf (dump_file, "(%d)",ai->lhs->new_shape_df);
           print_callstring_sequence (ai->lhs->cs_index,dump_file);
           fprintf (dump_file, " --> ");
           if (bi == NULL)
               fprintf (dump_file, "Empty");
           else {
               for (; bi; bi=bi->next) {
		   fprintf (dump_file, "(%d)",bi->rhsel->new_shape_df);
                   print_callstring_sequence (bi->rhsel->cs_index,dump_file);
                   if (bi->next)
                       fprintf (dump_file, "  ,  ");
               }
           }
           fprintf (dump_file, "\n");
       }
   }
}

 void print_callstring_sequence (int cs,FILE *dump)
{
   if (cs == -1)
       fprintf (dump, "null");
   else {
       callstring ci = VEC_index (callstring, callstrings, cs);
       fprintf (dump, "{");
       for (; ci; ci=ci->next) {
           basic_block bb = ci->bb;
           if (ci->next == NULL)
               fprintf (dump, "%s.%d", cgraph_node_name (cnode_of_bb (bb)), bb->index);
           else
               fprintf (dump, "%s.%d, ", cgraph_node_name (cnode_of_bb (bb)), bb->index);
       }
       fprintf (dump, "}");
   }
   fprintf(dump,"\n");
}

 bool pcsmap_required (struct cgraph_node *node)
{
   return ((cgraph_info)(node->aux))->pcsmap_required;
}

/* Returns the end basic block of the function (The end block, i.e the block
   before EXIT_BLOCK_PTR). */
basic_block end_bb_of_fn (struct cgraph_node *node)
{
    bb_worklist bb_list = worklist_of_cnode (node);
    int i = n_basic_blocks_for_function (DECL_STRUCT_FUNCTION (node->decl)) - NUM_FIXED_BLOCKS;
    return bb_list[i].bb;
}

 bool is_shape_bypass_block (basic_block bb)
{
   return (single_pred_p (bb) && !end_block (bb) && !return_block (bb));
}


 void print_shape_worklist (FILE *dump)
{
   worklist ai;
   bb_worklist list = worklist_of_cnode (cnode);
   int i;
   int num_bb = n_basic_blocks_for_function (DECL_STRUCT_FUNCTION (cnode->decl)) - NUM_FIXED_BLOCKS;
   fprintf (dump, "\nPrinting function worklist:  ");
   for (ai=start; ai->next; ai=ai->next)
       fprintf (dump, "%s,  ", cgraph_node_name (ai->called_fn));
   fprintf (dump, "%s ", cgraph_node_name (ai->called_fn));

   fprintf (dump, "\nPrinting current  bb worklist:  ");
   for (i=0; i<=num_bb; i++) {
       if (list[i].process_shape)
           fprintf (dump, "%s.%d ", cgraph_node_name (cnode), list[i].bb->index);
   }
   fprintf (dump, "\n");
}



int isSubstring(char *sub,char *str)
{
  char *flag;
  flag=strstr(str,sub);
  
  if(flag)
     return 1;
  else
    return 0;
  
}

int canPrintInfo()
{

   char *yesNo=getenv("NEED_DEBUG_INFO");

   if(!yesNo)      //Even when the variable is not defined return 1
     return 1;

   if(strcmp(yesNo,"1")==0)
        return 1;

  return 0;

}

void myPrint_Dump(FILE *f, ...)
{

    if(!canPrintInfo())
      return;

    va_list argp;
    va_start(argp, f);

     char *p;
    int i;
    unsigned u;
    char *s;

    char * mainFrmt=va_arg(argp,char *);

    for(p=mainFrmt; *p!='\0';p++)
    {
      if(*p!='%')
      {
        putchar(*p);continue;
      }
      p++;

      switch(*p)
      {
        case 'c' : i=va_arg(argp,int);fprintf(f,"%d",i);break;
        case 'd' : i=va_arg(argp,int);
        if(i<0){i=-i;putchar('-');}fprintf(f,"%s",myconvert(i,10));break;
        case 'o': i=va_arg(argp,unsigned int); fprintf(f,"%s",myconvert(i,8));break;
        case 's': s=va_arg(argp,char *);fprintf(f,"%s",s); break;
        case 'u': u=va_arg(argp,unsigned int); fprintf(f,"%s",myconvert(u,10));break;
        case 'x': u=va_arg(argp,unsigned int); fprintf(f,"%s",myconvert(u,16));break;
        case '%': fprintf(f,"%c",'%');break;
      }
    }
  va_end(argp);
}


char *myconvert(unsigned int num, int base)
{
static char buf[33];
char *ptr;
ptr=&buf[sizeof(buf)-1];
*ptr='\0';
do
{
  *--ptr="0123456789abcdef"[num%base];
  num/=base;
}while(num!=0);

  return(ptr);
}


