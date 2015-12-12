#include "all_includes_defines.h"
#include<assert.h>

/*-----------------------------------------------------------------------------*/
/* Global vars */
int noOfFieldPointers =   0;
int field_Table_Size  = 100;
int noOfHeapPtrs      =   0;
int global            =   0;
int tableOfHeaps_Size = 100;

int s_old_size_local = 1000;
int s_new_size_local =  100;
int number_of_nodes  =    0;
int number_of_stmts  =    0;


gimple *local_stmt_list                 = NULL;
pfbv_dfi **current_pfbv_dfi_stmt        = NULL;
pfbv_dfi **current_pfbv_dfi_bb          = NULL;

struct cgraph_node *cnode  = NULL;
basic_block cbb            = NULL;
basic_block main_firstbb   = NULL;
csdfa_info cshape_info     = NULL;
bool all_contexts_together = TRUE;
// work_stack  work_stack_Top = NULL;

struct timeval start1, end1;
FILE *shape_bb_dump;
int ***shapeForStmt;

bool newCnode_pushed_to_stack=FALSE;
struct stmt_shape * stmt_shape_map;
int globTemp=0;
int globalStmtId;


/*--------------------------------------------------------------------*/
//these are included because of in this sys(my lappy) i didnot use the gdfa patch which these belong
void report_dfa_spec_error (const char *mesg)
{
	fprintf (stderr, "DFA initialization error: %s\n", mesg);
	exit (1);
}

int find_index_bb (basic_block bb)
{
	int nid = -1;
	if (bb->index >= 0 && bb->index < n_basic_blocks)
		nid = bb->index;
	else
		report_dfa_spec_error
			("Wrong index of basic block (Function find_index_bb)");
	return nid;
}

//----------------------------------------------------------------------------------------------
void initialize_pfbv_dfi(pfbv_dfi **p)
{
    *p=(pfbv_dfi *) xcalloc(1, sizeof(pfbv_dfi));
	initialize_some_dataFlowValue(&(*p)->gen);
}
void free_pfbv_dfi(pfbv_dfi **temp)
{
	free_dataFlowValue(&((*temp)->gen));
	//free(*temp);
}
//allocate space and initialize for a dataFlowValue pointer variable
 void initialize_some_dataFlowValue(dataFlowValue **temp)
{
	int i,j;
	(*temp) = ( dataFlowValue *)xmalloc(sizeof(dataFlowValue));
	(*temp)->direction=(int **)xmalloc(noOfHeapPtrs*sizeof(int *));
	(*temp)->interference=(int **)xmalloc(noOfHeapPtrs*sizeof(int *));
	(*temp)->attribute=(shape *)xmalloc(noOfHeapPtrs*sizeof(shape));

	for(i=0;i<noOfHeapPtrs;i++)
	{
		((*temp)->direction)[i]=(int *)xmalloc(noOfHeapPtrs*sizeof(int));
		((*temp)->interference)[i]=(int *)xmalloc(noOfHeapPtrs*sizeof(int));

	}
	for(i=0;i<noOfHeapPtrs;i++)
	{  for(j=0;j<noOfHeapPtrs;j++)
		{
			((*temp)->direction)[i][j]=0;
			((*temp)->interference)[i][j]=0;
		}
		((*temp)->attribute)[i]=TREE;
	}  
}
 void initialize_some_dataFlowValue_malloc(dataFlowValue **temp)
{
// 	int i;
	initialize_some_dataFlowValue(temp);
// 	for(i=0;i<noOfHeapPtrs;i++)
// 	{
// 		((*temp)->direction)[i][i]=1;
// 		((*temp)->interference)[i][i]=1;
// 	}

}

 void free_dataFlowValue(dataFlowValue **temp)
{
	int i;

	for(i=0;i<noOfHeapPtrs;i++)
	{
		free( ((*temp)->direction)[i]);
		free( ((*temp)->interference)[i]);
		((*temp)->direction)[i]=NULL;
		((*temp)->interference)[i]=NULL;
	}
	free((*temp)->direction);
	free((*temp)->interference);
	free((*temp)->attribute);
	(*temp)->direction=NULL;
	(*temp)->interference=NULL;
	(*temp)->attribute=NULL;
	free(*temp);
	(*temp)=NULL;
}
 void assignRightToLeft_pfbv_dfi(pfbv_dfi *left,pfbv_dfi *right)
{
	assignRightToLeft_dataFlowValue(left->gen,right->gen);
	assignRightToLeft_dataFlowValue(left->kill,right->kill);
	assignRightToLeft_dataFlowValue(left->in,right->in);
	assignRightToLeft_dataFlowValue(left->out,right->out);
}
//assignment operation between dataFlowValue variables so that data get copied.
 void assignRightToLeft_dataFlowValue(dataFlowValue *left,dataFlowValue *right)
{
	int i,j;
	for(i=0;i<noOfHeapPtrs;i++)
	{  for(j=0;j<noOfHeapPtrs;j++)
		{ 
			(left->direction)[i][j]=(right->direction)[i][j];
			(left->interference)[i][j]=(right->interference)[i][j];
		}
		(left->attribute)[i]=(right->attribute)[i];
	}   
}

void assignRightToLeft_shape_info(pfbv_dfi **left,pfbv_dfi *right)
{
	assignRightToLeft_dataFlowValue((*left)->gen,right->gen);
}

int find_index_of_local_stmt(gimple stmt)
{
	int index = -1;
	if(stmt) {
		index = ENTITY_INDEX(*(tree)stmt);
	}
	return index;
}
void allocate_space (pfbv_dfi*** dfi, int number_of_nodes)
{
	int iter;

	(*dfi) = (pfbv_dfi **) xcalloc (number_of_nodes, sizeof (pfbv_dfi *));

	for (iter = 0; iter < number_of_nodes; iter++)
	{
		(*dfi)[iter] = (pfbv_dfi *) xcalloc (1, sizeof (pfbv_dfi));
	}

}
void
assign_indices_to_stmt(void)
{
	basic_block bb;
	gimple_stmt_iterator gsi;
	gimple stmt;
	shapeForStmt = (int ***)xmalloc(2*sizeof(int **));
	for (cnode=cgraph_nodes; cnode; cnode=cnode->next) {
		if (!gimple_has_body_p (cnode->decl) || cnode->clone_of)
			continue;
		push_cfun (DECL_STRUCT_FUNCTION (cnode->decl));

		//data structure to store all statements TO DO : free local_stmt_list
		local_stmt_list = (gimple *)xcalloc(s_old_size_local,sizeof(gimple ));
		FOR_EACH_BB_FN (bb, cfun) {
			for (gsi = gsi_start_bb (bb); !gsi_end_p (gsi); gsi_next (&gsi)) {
				if(number_of_stmts == s_old_size_local) {
					s_old_size_local += s_new_size_local;
					local_stmt_list =(gimple *) xrealloc(local_stmt_list,sizeof(tree )*s_old_size_local);
				}
				stmt = gsi_stmt (gsi);
				if(isItInThe8Stmts(stmt))
				{
					ENTITY_INDEX((*(tree)stmt)) = number_of_stmts;
					local_stmt_list[number_of_stmts] = stmt;
					number_of_stmts++;
				}
			}
		}

		pop_cfun();
	}
	//For counting the number of tree,dag,cycle
	shapeForStmt[0]=(int **)xmalloc(number_of_stmts*sizeof(int *));
	shapeForStmt[1]=(int **)xmalloc(number_of_stmts*sizeof(int *));
	int i;      
	for(i=0;i<number_of_stmts;i++)
	{
		shapeForStmt[0][i]=(int *)xmalloc(noOfHeapPtrs*sizeof(int));
		shapeForStmt[1][i]=(int *)xmalloc(noOfHeapPtrs*sizeof(int));
	}
	int j;
	for(i=0;i<number_of_stmts;i++)
	{
		for(j=0;j<noOfHeapPtrs;j++)
		{
			shapeForStmt[0][i][j]=0;
			shapeForStmt[1][i][j]=0;
		}
	}
	//This is used to write into file shapeSummary.txt      
	stmt_shape_map=(struct stmt_shape *)xmalloc(number_of_stmts*2*sizeof(struct stmt_shape));

	for(i=0;i<number_of_stmts*2;i++)  //even ---before stmt,  odd----after stmt
	{
		stmt_shape_map[i].shape=(int *)xmalloc(noOfHeapPtrs*sizeof(int));
		stmt_shape_map[i].stmt=NULL;
	}

	for(i=0;i<number_of_stmts*2;i++)
	{
		for(j=0;j<noOfHeapPtrs;j++)
		{
			stmt_shape_map[i].shape[j]=0;
		}
	}
}

 bool is_new_dfvalue (dataFlowValue *prev_info, dataFlowValue * new_info)
{

	bool temp=TRUE;
	int i,j;
	for(i=0;i<noOfHeapPtrs;i++)
	{
		for(j=0;j<noOfHeapPtrs;j++)
		{ 
			temp=temp && ((prev_info->direction)[i][j] == (new_info->direction)[i][j]);
			temp=temp && ((prev_info->interference)[i][j] == (new_info->interference)[i][j]);
		}
		temp=temp && ((prev_info->attribute)[i] == (new_info->attribute)[i]);
	}
	return (!temp);
}
 void generateGENKILLvalues(int stmtType,gimple stmt,dataFlowValue *inStmt)
{
	int i,j,pId,qId;
	tree lhsop=NULL,rhsop1=NULL;
	if (is_gimple_assign (stmt))
	{
		lhsop = gimple_assign_lhs (stmt);
		rhsop1 = gimple_assign_rhs1 (stmt);
	} 
	if(stmtType==1)
	{
		lhsop = gimple_assign_lhs (stmt);
		pId=getId(getTheName(lhsop),cnode);
		for(i=0;i<noOfHeapPtrs;i++)
		{
			if( (inStmt->direction)[pId][i])
				D(stmt,kill,pId,i)=1;
			if( (inStmt->direction)[i][pId])
				D(stmt,kill,i,pId)=1;
			if( (inStmt->interference)[pId][i] )
				I(stmt,kill,pId,i)=1,I(stmt,kill,i,pId)=1;
		}
		D(stmt,gen,pId,pId)=1;
		I(stmt,gen,pId,pId)=1;
		return;
	}
	if(stmtType==2)
	{
		pId=getId(getTheName(lhsop),cnode);
		for(i=0;i<noOfHeapPtrs;i++)
		{
			if( (inStmt->direction)[pId][i])
				D(stmt,kill,pId,i)=1;
			if( (inStmt->direction)[i][pId])
				D(stmt,kill,i,pId)=1;
			if( (inStmt->interference)[pId][i] )
				I(stmt,kill,pId,i)=1,I(stmt,kill,i,pId)=1;
		}
		return;
	}

	//31---p[]=q   no kill info
	if((stmtType>=3 && stmtType <=5 )|| stmtType==31)
	{
        if(stmtType == 31)
		{
		  	pId=getId(getTheName(lhsop),cnode);
			qId=findNameAndGetId_q(stmtType,stmt);	
		}
		else
		{
			pId=getId(getTheName(lhsop),cnode);
			qId=findNameAndGetId_q(stmtType,stmt);
		}
		if(pId==qId && stmtType!=31)
			return;
		
		
		for(i=0;i<noOfHeapPtrs;i++)
		{
			if(stmtType!=31)
			{
			    // kill relations are same as of p=malloc()
			    if( (inStmt->direction)[pId][i])
				    D(stmt,kill,pId,i)=1;
			    if( (inStmt->direction)[i][pId])
				    D(stmt,kill,i,pId)=1;
			    if( (inStmt->interference)[pId][i] )
				    I(stmt,kill,pId,i)=1,I(stmt,kill,i,pId)=1;			    
			}
			
			if(i!=pId && (inStmt->direction)[i][qId])
				D(stmt,gen,i,pId)=1;
			if(i!=pId && (inStmt->direction)[qId][i])
				D(stmt,gen,pId,i)=1;
			if(i!=pId && (inStmt->interference)[qId][i])
				I(stmt,gen,pId,i)=1,I(stmt,gen,i,pId)=1;
		}
		if((inStmt->direction)[qId][qId])
			D(stmt,gen,pId,pId)=1;
		if((inStmt->interference)[qId][qId])
			I(stmt,gen,pId,pId)=1;

		return;
	}
	
	//p->f=q or p->f[]=q
	if(stmtType==7 || stmtType==71)
	{
		//p->f=q;
		// doesnot kill anything
		
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
			for(j=0;j<noOfHeapPtrs;j++)
			{
				if((inStmt->direction)[i][pId] && (inStmt->direction)[qId][j])
				{
					D(stmt,gen,i,j)=1;
				}
				if((inStmt->direction)[i][pId] && (inStmt->interference)[qId][j])
				{

					I(stmt,gen,i,j)=1;
					I(stmt,gen,j,i)=1;
				}
			}
		}
		return;
	}

	      //p=q->f and p=q->f[]
	if(stmtType==8 || stmtType==81)
	{
		pId=getId(getTheName(lhsop),cnode);
		qId=findNameAndGetId_q(stmtType,stmt);
		
		
		if(stmtType !=81)
		{		      
		    for(i=0;i<noOfHeapPtrs;i++)
		    {
			    if( (inStmt->direction)[pId][i])
				    D(stmt,kill,pId,i)=1;
			    if( (inStmt->direction)[i][pId])
				    D(stmt,kill,i,pId)=1;
			    if( (inStmt->interference)[pId][i] )
				    I(stmt,kill,pId,i)=1,I(stmt,kill,i,pId)=1;
		    }
				    
		}
		    
		    
		for(i=0;i<noOfHeapPtrs;i++)
		{
			if(i!=pId && (inStmt->interference)[i][qId])
				D(stmt,gen,i,pId)=1;

			if(i!=pId && i!=qId && (inStmt->direction)[qId][i])
				D(stmt,gen,pId,i)=1;

			if( i!=pId && (inStmt->interference)[qId][i])
				I(stmt,gen,pId,i)=1,I(stmt,gen,i,pId)=1;

		}

		if( (inStmt->attribute)[qId]==CYCLE)
			D(stmt,gen,pId,qId)=1;
		if((inStmt->direction)[qId][qId])
			D(stmt,gen,pId,pId)=1;
		if((inStmt->interference)[qId][qId])
			I(stmt,gen,pId,pId)=1;
	}
}
void printDataFlow(int di,FILE *file,dataFlowValue *value)
{
	int i,j;
	if(di==0)
	{
		fprintf(file,"D\t\t");
		for(i=0;i<noOfHeapPtrs;i++)
		{
			fprintf(file,"%s\t",tableOfHeaps[i].ptrName );
		}
		fprintf(file,"\n");
		for(i=0;i<noOfHeapPtrs;i++)
		{

			fprintf(file,"%s\t\t",tableOfHeaps[i].ptrName );
			for(j=0;j<noOfHeapPtrs;j++)
			{
				fprintf(file,"%d\t",(value->direction)[i][j]);

			}
			fprintf(file,"\n");
		}
		fprintf(file,"\n*************************#############################*********************\n");
	}
	else   //print Interference matrices
	{
		if(di==1)
		{
			fprintf(file,"I\t\t");
			for(i=0;i<noOfHeapPtrs;i++)
			{
				fprintf(file,"%s\t",tableOfHeaps[i].ptrName );
			}
			fprintf(file,"\n");
			for(i=0;i<noOfHeapPtrs;i++)
			{

				fprintf(file,"%s\t\t",tableOfHeaps[i].ptrName );
				for(j=0;j<noOfHeapPtrs;j++)
				{
					fprintf(file,"%d\t",(value->interference )[i][j]);

				}  
				fprintf(file,"\n");
			}
		}
		fprintf(file,"\n*************************#############################*********************\n");
	}

}
dataFlowValue * combine(dataFlowValue * value1, dataFlowValue *value2)
{
	dataFlowValue *temp;
	initialize_some_dataFlowValue(&temp);
	int i,j;
	for(i=0;i<noOfHeapPtrs;i++)
	{  for(j=0;j<noOfHeapPtrs;j++)
		{ 
			(temp->direction)[i][j] = (value1->direction)[i][j] || (value2->direction)[i][j];
			(temp->interference)[i][j]=(value1->interference)[i][j] || (value2->interference)[i][j];
		}
		(temp->attribute)[i]= ( (value1->attribute)[i]>(value2->attribute)[i] ? (value1->attribute)[i]:(value2->attribute)[i]  );
	}   
	return temp;
}
//this function doesnot deal with Attribute array A[].It is dealt by function computeAttributeArray
dataFlowValue * transfer_fn(dataFlowValue * gen,dataFlowValue * in,dataFlowValue * kill)
{
	int i,j;
	dataFlowValue *temp;
	initialize_some_dataFlowValue(&temp);
	for(i=0;i<noOfHeapPtrs;i++)
	{
		for(j=0;j<noOfHeapPtrs;j++)
		{
			(temp->direction)[i][j] = (gen->direction)[i][j] || ( (in->direction)[i][j] && !((kill->direction)[i][j]));   
			(temp->interference)[i][j] = (gen->interference)[i][j] || ( (in->interference)[i][j] && !((kill->interference)[i][j]));   
		}
	}
	//gen union (in -kill)
	return temp;
}
int findNameAndGetId_q(int stmtType , gimple stmt)
{
// 	tree lhsop = gimple_assign_lhs (stmt);
	tree rhsop1 = gimple_assign_rhs1 (stmt);
	switch(stmtType)
	{
		case 3:
		case 31:  
			return getId(getTheName(rhsop1),cnode);
		case 4:
			return getId(getTheName(TREE_OPERAND(TREE_OPERAND(TREE_OPERAND(rhsop1,0),0),0)),cnode);
		case 5:
			return getId(getTheName(rhsop1),cnode);
		case 8:
			return getId(getTheName(TREE_OPERAND(TREE_OPERAND(rhsop1,0),0)),cnode);
		case 81:
			return getId(getTheName(TREE_OPERAND(TREE_OPERAND(TREE_OPERAND(rhsop1,0),0),0)),cnode);
	}
	printf("\n Error in findNameAndGetId_q \n");
	return -1;
}


static bool isMemRef(tree t) 
{
    return INDIRECT_REF_P(t) ||
        (TREE_CODE (t) == MEM_REF);
}

//Is the given statemtns among the 8 stmts we are considering
int isItInThe8Stmts (gimple stmt)
{
    if(is_gimple_call (stmt))
    {
        // 1 if malloc/new of a RECORD (structure)
        // 0 if any other call
		tree call=gimple_call_fndecl(stmt);
        if (!call)
            return 0;
        if (!strcmp(get_name(call),"malloc") || !strcmp(get_name(call),"calloc") || !strcmp(get_name(call),"realloc")    // *alloc
            || !strcmp(get_name(call),"operator new")) { // new
            tree var = gimple_assign_lhs (stmt);
            return (TREE_CODE(TREE_TYPE(TREE_TYPE(var))) == RECORD_TYPE);
        }
        return 0;
    }
    
	if (!is_gimple_assign (stmt))
        return 0;

    tree lhsop = gimple_assign_lhs (stmt);
    tree rhsop1 = gimple_assign_rhs1 (stmt);
    tree rhsop2 = gimple_assign_rhs2 (stmt);
    int lhsCode=TREE_CODE(lhsop);
    int rhsCode1;
    
    rhsCode1=TREE_CODE(rhsop1);
    
    if(lhsCode==SSA_NAME) //sometimes it is necessary due
        //to the ssa representation
    { 
        lhsop=SSA_NAME_VAR(lhsop);
        lhsCode=TREE_CODE(lhsop);
    }
    if(rhsCode1==SSA_NAME)
    {
        rhsop1=SSA_NAME_VAR(rhsop1);
        rhsCode1=TREE_CODE(rhsop1);
    }
    
    if(lhsCode==ARRAY_REF)
    {
        int lhsOp1Code=TREE_CODE(TREE_OPERAND(lhsop,0));
        // p->f[]=
        if(lhsOp1Code == COMPONENT_REF
           && isMemRef(TREE_OPERAND(TREE_OPERAND(lhsop,0),0))
           && isStructPtr(TREE_TYPE(TREE_OPERAND(TREE_OPERAND(lhsop,0),1))))
        {		    
            // p->f[]=NULL		      		    
            if( rhsCode1==INTEGER_CST && TREE_INT_CST_LOW(rhsop1)==0 )
                return 61;
		    
		    //p->f[]=q
		    if(rhsCode1!=INTEGER_CST )
                return 71;
		    
		    //p->f[]=q[]  not possible		      		    
		    //p->f=q[]  not possible		      		    
        }
        
        //p[]=
        else if((lhsOp1Code==VAR_DECL || lhsOp1Code==PARM_DECL)
                && isStructPtr(lhsop))
        {		    
            //p[]=NULL;
            if(rhsCode1==INTEGER_CST && TREE_INT_CST_LOW(rhsop1)==0 )		    
                return 21;
            
		    //p[]=q
		    if((rhsCode1==VAR_DECL||rhsCode1==PARM_DECL)
               && isStructPtr(rhsop1) && rhsop2==NULL)		    
                return 31;	
		    
		    //p[]=q->f not possible
		    //p[]=q[]->f not possible
		    //p[]=q[]  not possible 
        }
    }
		
    // to identify statement p=NULL;
    if ((lhsCode==VAR_DECL || lhsCode==PARM_DECL)
        && isStructPtr(lhsop)
        && rhsCode1==INTEGER_CST
        && TREE_INT_CST_LOW(rhsop1)==0) {
        return 2;
    }
    
    //to identify statement p=q OP n;
    if (rhsop2 &&
        (lhsCode==VAR_DECL||lhsCode==PARM_DECL)
        && (rhsCode1==VAR_DECL||rhsCode1==PARM_DECL)
        && isStructPtr(lhsop)
        && isStructPtr(rhsop1)
        && TREE_CODE(rhsop2)==INTEGER_CST) {
        return 5;
    }
    
    //to identify statement p=q
    if (!rhsop2
        && (lhsCode==VAR_DECL||lhsCode==PARM_DECL)
        && (rhsCode1==VAR_DECL||rhsCode1==PARM_DECL)
        && isStructPtr(lhsop)
        && isStructPtr(rhsop1)) {
        return 3;
    }
	
    //to identify statement p=q[]		
    if(rhsCode1==ARRAY_REF) {
        int rhsOp1code=TREE_CODE(TREE_OPERAND(rhsop1,0));		
        if( (lhsCode==VAR_DECL||lhsCode==PARM_DECL)
            && (rhsOp1code==VAR_DECL||rhsOp1code==PARM_DECL)
            && isStructPtr(lhsop)
            && isStructPtr(rhsop1)
            && rhsop2==NULL)
            return 3;		  		  
    }
    
    //to identify p=&(q->f)
    if( (lhsCode==VAR_DECL||lhsCode==PARM_DECL)
        && rhsCode1==ADDR_EXPR
        && TREE_CODE(TREE_OPERAND(rhsop1,0))==COMPONENT_REF
        && isMemRef(TREE_OPERAND(TREE_OPERAND(rhsop1,0),0)) ) {
        return 4;
    }
    
    //to identify p->f=NULL
    if(lhsCode==COMPONENT_REF
       && isMemRef(TREE_OPERAND(lhsop,0))
       && isStructPtr( TREE_OPERAND(lhsop,1))
       && rhsCode1==INTEGER_CST
       && TREE_INT_CST_LOW(rhsop1)==0 ) {
        return 6;
    }
    
    //to identify p->f=q
    if(lhsCode==COMPONENT_REF
       && isMemRef(TREE_OPERAND(lhsop,0))
       && isStructPtr(TREE_OPERAND(lhsop,1))
       && rhsCode1!=INTEGER_CST ) {
        return 7;
    }
	
    //p=q[]->f not possible
    //to identify p=q->f
    if( (lhsCode==VAR_DECL||lhsCode==PARM_DECL)
        && isStructPtr(lhsop)
        && rhsCode1==COMPONENT_REF
        && isMemRef(TREE_OPERAND(rhsop1,0))
        && isStructPtr(TREE_OPERAND(rhsop1,1)) ) {
        return 8;
    }
	
    //p=q->f[]
    if ((lhsCode==VAR_DECL||lhsCode==PARM_DECL)
        && isStructPtr(lhsop)
        && rhsCode1==ARRAY_REF
        && TREE_CODE(TREE_OPERAND(rhsop1,0))==COMPONENT_REF
        && isMemRef(TREE_OPERAND(TREE_OPERAND(rhsop1,0),0))
        && isStructPtr(TREE_TYPE(TREE_OPERAND(TREE_OPERAND(rhsop1,0),1)))) {
        return 81;
    }
    
    return 0;
}

 bool is_pointer_var (tree var)
{
	return is_pointer_type (TREE_TYPE (var));
}

 bool is_pointer_type (tree type)
{
	if (POINTER_TYPE_P (type))
		return true;
	if (TREE_CODE (type) == ARRAY_TYPE)
		return is_pointer_var (TREE_TYPE (type));
	/* Return true if it is an aggregate type. */
	return AGGREGATE_TYPE_P (type);
}

 int isStructPtr(tree var)
{
	if(POINTER_TYPE_P(TREE_TYPE(var)) && (TREE_CODE(TREE_TYPE(TREE_TYPE(var))) == RECORD_TYPE))
		return 1;
	else
		return 0;
}
 char * getDummyName(tree var)
{
	char *s=(char*)xmalloc(10*sizeof(char));
	s[0]='D';
	s[1]='.';
	s[2]='\0';

	int i=DECL_UID(var);
	char *uid=(char*)xmalloc(5*sizeof(char));
	sprintf(uid,"%d",i);
	return strcat(s,uid);
}

char * getTheName(tree var)           //returns Dummy name if it has
{
	if(TREE_CODE(var)==SSA_NAME)
	{
		if(get_name(SSA_NAME_VAR(var))==NULL)
			return getDummyName(SSA_NAME_VAR(var));
		else
			return (char *)get_name(SSA_NAME_VAR(var));
	}
	else
	{
		if(get_name(var)==NULL)
		{
			if(TREE_CODE(var)==ARRAY_REF)
			  return (char *)get_name(TREE_OPERAND(var,0));
			else 
			return getDummyName(var);
		}
		else
			return (char *)get_name(var);
	}
}




int getId(const char * heapPtrName,struct cgraph_node *cnode)
{
	int i;
	char scope[50];
	strcpy(scope,(char *)cgraph_node_name(cnode));
	//c++ change
	for(i=0;i<noOfHeapPtrs;i++)
	{
#ifdef CPP
		if((strcmp(heapPtrName,tableOfHeaps[i].ptrName)==0) && (isSubstring(tableOfHeaps[i].scope,scope)==1))
		{
			return tableOfHeaps[i].id;      
		}
#else
		if((strcmp(heapPtrName,tableOfHeaps[i].ptrName)==0)&& (strcmp(scope,tableOfHeaps[i].scope)==0))
		{
			return tableOfHeaps[i].id;
		}
#endif
	}
	for(i=0;i<noOfHeapPtrs;i++)
	{
		if((strcmp(heapPtrName,tableOfHeaps[i].ptrName)==0)  && (strcmp("0",tableOfHeaps[i].scope)==0 ))
		{
			return tableOfHeaps[i].id;
		}

	}
    // FAIL if we reach here!
	assert(0 && "Bad name for GetId");
	exit(1);
}
int max(int a,int b)
{
	return (a>b)?a:b;
}
//the var is the struct pointer to that field  
//say struct abcd  *p;   here var refers to p
char *getStructName(tree var)
{
	tree z=TYPE_NAME(TREE_TYPE(TREE_TYPE(var)));
	char *s;
	if(TREE_CODE(z)==IDENTIFIER_NODE)
	{	
		s=(char *)IDENTIFIER_POINTER(z);
		return s;
	}
	else   //typedef declaration
	{
		s=(char *)IDENTIFIER_POINTER(DECL_NAME(z));
		return s;
	}
}
