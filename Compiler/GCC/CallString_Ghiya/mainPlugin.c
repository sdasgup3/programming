#include "all_includes_defines.h"

//-------------------------
extern "C" 
{
    unsigned int inter_shape_analysis();
}
// Globals
FILE                                  *pFile;
FILE                                  *debug_file;
struct ptr_to_struct_table            *tableOfHeaps;
struct ptr_to_field_of_struct_table   *field_Table;
worklist                               start, shape_node;
FILE                                  *debug_file1;
FILE                                  *tempFile;


void init_info (void);
void fillTable();
void init_shapeAnalysis();
csdfa_info initialize_in();
void assign_indices_to_stmt();
void compute_shape();
int plugin_is_GPL_compatible;
void print_stack(FILE *f);

void countShape();
long long timeval_diff(struct timeval *difference,
                       struct timeval *end_time, struct timeval *start_time);
void print_shapeArray(basic_block bb,int in);
void createShapeFile();
char *itoa(int n, char *s);

struct simple_ipa_opt_pass pass_inter_gimple_manipulation =
{
    {
        SIMPLE_IPA_PASS, /* optimization pass type */
        "interShapeAlys", /* name of the pass*/
        NULL,   //gate_gimple_manipulation, /* gate. */
        inter_shape_analysis, /* execute (driver function) */
        NULL, /* sub passes to be run */
        NULL, /* next pass to run */
        0, /* static pass number */
        (timevar_id_t) 0, /* timevar_id */
        0, /* properties required */
        0, /* properties provided */
        0, /* properties destroyed */
        0, /* todo_flags start */
        0 /* todo_flags end */
    }
};


struct register_pass_info pass_info = {
  &(pass_inter_gimple_manipulation.pass),            /* Address of new pass, here, the 'struct
                                     opt_pass' field of 'gimple_opt_pass'
                                     defined above */
  "pta",                          /* Name of the reference pass for hooking up
                                     the new pass. */
  0,                              /* Insert the pass at the specified instance
                                     number of the reference pass. Do it for
                                     every instance if it is 0. */
  PASS_POS_INSERT_AFTER           /* how to insert the new pass: before,
                                     after, or replace. Here we are inserting
                                     a pass names 'plug' after the pass named
                                     'cfg' */
};

/*-----------------------------------------------------------------------------
 *  plugin_init is the first function to be called after the plugin is loaded
 *-----------------------------------------------------------------------------*/
int
plugin_init(struct plugin_name_args *plugin_info,
    struct plugin_gcc_version *version)
{


// 	printf("%s\n",plugin_info->base_name);
  /*-----------------------------------------------------------------------------
   * Plugins are activiated using this callback 
   *-----------------------------------------------------------------------------*/
  register_callback (
      plugin_info->base_name,     /* char *name: Plugin name, could be any
                                     name. plugin_info->base_name gives this
                                     filename */
      PLUGIN_PASS_MANAGER_SETUP,  /* int event: The event code. Here, setting
                                     up a new pass */
      NULL,                       /* The function that handles event */
      &pass_info);                /* plugin specific data */

  // use version some how...
  (void)version;
  return 0;
}



 unsigned int
inter_shape_analysis (void)
{
  
 #ifdef DUMP_IN_DUMPFILE  
  shape_bb_dump =fopen("shape_bb_dump.txt","w");
  fprintf(shape_bb_dump,"Shape Dump for each basic block\n");
  fclose(shape_bb_dump);
    
  
    debug_file=fopen("DebugInfo.txt","w");
    fclose(debug_file);
   
  
  pFile=fopen("shape.txt","w");
  fprintf(pFile,"----------------SHAPE INFO---------------\n\n");
  fclose(pFile);
#endif  
  
struct timeval interval;
struct rusage start_usage, end_usage;
  //gettimeofday(&start1, NULL);
  getrusage(RUSAGE_SELF, &start_usage);
  start1 = start_usage.ru_utime;
  
  // this contains info about all pointer variables..
  fillTable();
  
 
  init_info();   // not sure of its req
  
  struct function *old_cfun = cfun;
  tree old_cfundecl = current_function_decl;
  preprocess_Cfg();
  
  
#ifdef DUMP_IN_DUMPFILE  
      print_cfg();
#endif
  //assign index for each stmt!!
   
#ifdef MYDEBUG_1
   printf("\n In mainPlugin after print_cfg\n");
#endif
  
   


   assign_indices_to_stmt(); 
   
 #ifdef MYDEBUG_1
   printf("\n In mainPlugin after  assign_indices_to_stmt  \n");
#endif
   allocate_space(&current_pfbv_dfi_stmt, number_of_stmts);  //allocate space for each stmt in each basic block!!!!!!!!11
  
  #ifdef MYDEBUG_1
   printf("\n In mainPlugin after  allocate_space  \n");
#endif
  
#ifdef DUMP_IN_DUMPFILE   
   debug_file=fopen("DebugInfo.txt","a");
#endif   
  //mine--initialize the entry block In info,entry block entry stmt in_info
  init_shapeAnalysis();
#ifdef DUMP_IN_DUMPFILE   
   fclose(debug_file);
#endif   
  
  #ifdef MYDEBUG_1
   printf("\n In mainPlugin after  init_shapeAnalysis  \n");
#endif
  

//   printf("no of stmts =  %d\n",number_of_stmts);
//   global_dfa();
  
  compute_shape();
  
  #ifdef MYDEBUG_1
   printf("\n In mainPlugin after compute_shape\n");
  #endif
  
  countShape(); 

	//gettimeofday(&end1, NULL);
	getrusage(RUSAGE_SELF, &end_usage);
	end1 = end_usage.ru_utime;

	timeval_diff(&interval,&end1,&start1);
  #ifdef MYDEBUG_1
  printf("time for func:%s   is %ld seconds, %ld microseconds\n",current_function_name(), interval.tv_sec,interval.tv_usec);
  #endif
	
     createShapeFile();	
     
    //log is of form (t,d,c)_(sec S,msec MS)
    char secStr[100],msStr[100];
    char timeStr[200];
    sprintf(secStr,"%ld",interval.tv_sec);
    sprintf(msStr,"%.4f",((float)interval.tv_usec)/1000);

    strcpy(timeStr,"_(");
    strcat(timeStr,secStr);	strcat(timeStr,"S,");
    strcat(timeStr,msStr);	strcat(timeStr,"MS)\n");
    
    FILE *tempoFile=fopen("shape_time_count.txt","a");
    fprintf(tempoFile,timeStr);
    fprintf(tempoFile,"\n");
    fclose(tempoFile);
	
   restore_Cfg();
   current_function_decl = old_cfundecl;
   set_cfun (old_cfun);
 return 0;
}  



void createShapeFile()
{
  
    int treeCount=0;
    int dagCount=0;
    int cycleCount=0;
  
    FILE *f= fopen("shapeSummary.txt","w");
    int i,j;

    for(i=0;i<number_of_stmts*2;i++)
    {
      
        gimple stmt=stmt_shape_map[i].stmt;
      
        if(stmt==NULL)
            continue;
  
        if(i%2==0)
        {
	
	
	
            print_gimple_stmt(f,stmt_shape_map[i].stmt,1,1);
	  
            fprintf(f,"\nBefore\n");
	    
            for(j=0;j<noOfHeapPtrs;j++)
            {
	      
                switch(stmt_shape_map[i].shape[j])
                {
                    case 0:
                        fprintf(f,"%s\tTREE\n",tableOfHeaps[j].ptrName);
                        break;
                    case 1:
                        fprintf(f,"%s\tDAG\n",tableOfHeaps[j].ptrName);
                        break;
                    case 2:
                        fprintf(f,"%s\tCYCLE\n",tableOfHeaps[j].ptrName);
                        break;
                }
            }
        }
      
        if(i%2==1)   //After
        {
            fprintf(f,"After\n");
            for(j=0;j<noOfHeapPtrs;j++)
            {
                switch(stmt_shape_map[i].shape[j])
                {
                    case 0:
                        fprintf(f,"%s\tTREE\n",tableOfHeaps[j].ptrName);
                        treeCount++;
                        break;
                    case 1:
                        fprintf(f,"%s\tDAG\n",tableOfHeaps[j].ptrName);
                        dagCount++;
                        break;
                    case 2:
                        fprintf(f,"%s\tCYCLE\n",tableOfHeaps[j].ptrName);
                        cycleCount++;
                        break;
                }
            }
        }
        fprintf(f,"***************************\n");
        fprintf(f,"\n");
    }
    
    
    
	char treeStr[100],dagStr[100],cycleStr[100];
	char resultStr[200];
	itoa(treeCount,treeStr);
	itoa(dagCount,dagStr);
	itoa(cycleCount,cycleStr);
	strcpy(resultStr,"(");
	strcat(resultStr,treeStr);	strcat(resultStr,",");
	strcat(resultStr,dagStr);	strcat(resultStr,",");
	strcat(resultStr,cycleStr); strcat(resultStr,")");
	FILE *tempoFile=fopen("shape_time_count.txt","w");
	fprintf(tempoFile,resultStr);
	//     fprintf(tempoFile,"\n");
	fclose(tempoFile);
	
    fprintf(stderr,"\nTree:%d, DAG: %d, Cycle: %d\n", treeCount, dagCount, cycleCount);    
    fclose(f);

}
  
 
 
void countShape()
{
  int i,j;
  int treeCount=0,dagCount=0,cycleCount=0;
  
  for(i=0;i<number_of_stmts;i++)
  {
    for(j=0;j<noOfHeapPtrs;j++)
    {
	switch(shapeForStmt[1][i][j])
	{
	  case 0:
	    treeCount++;
	    break;
	  case 1:
	    dagCount++;
	    break;
	  case 2:
	    cycleCount++;
	    break;
	}
    }
  }

#ifdef MYDEBUG_1
printf("\n\ntree=%d,dag=%d,cycle=%d\n\n",treeCount,dagCount,cycleCount);  
#endif  
}


void compute_shape()
{

	cnode=first_node_from_shape_worklist();
	while (cnode) {
#ifdef MYDEBUG    
		debug_file=fopen("DebugInfo.txt","a");
		fprintf(debug_file,"\n $$$$$$$$$$$$\t\t\t%s\t\t\t$$$$$$$$$$$$$\n\n",cgraph_node_name(cnode));
		fclose(debug_file);
#endif

		bb_worklist bb_list = worklist_of_cnode (cnode);
		bool process_bb = true;
		while (process_bb) {
#ifdef DUMP_IN_DUMPFILE	 
			debug_file=fopen("DebugInfo.txt","a");       
#endif	   
			bool new_cnode = false;
#ifdef DUMP_IN_DUMPFILE	 
			print_shape_worklist(debug_file);
#endif	   
			cbb = get_bb_for_shape_processing (bb_list, &new_cnode);    //mine--if the current cnode's evaluation is complete we need to get a new not processed cnode..if so new_code=TRUE
			if (!new_cnode) {
				if (cbb) {
#ifdef MYDEBUG
					fprintf(debug_file,"\n ------------------------start processing of bb%d-----------------------\n",cbb->index);
#endif
					if (call_block (cbb))
						perform_shapedfa_on_callblock ();   //mine-- yet to do ???????
					else if (return_block (cbb))
						perform_shapedfa_on_returnblock ();
					else if (end_block (cbb)) {
						perform_shapedfa_on_endblock ();    //mine-- also the information is passed to respective callblock	
						if (shape_callee_nodes_processed ()) {   //mine-- it is true if all successors of the current node in the worklist  lv_node are processed
							cnode = prev_node_from_shape_worklist ();
#ifdef MYDEBUG
							if(cnode)
								fprintf(debug_file,"\n The cnode added is %s \n",cgraph_node_name(cnode));	   
#endif	
							process_bb = false;
						}
					}
					else
						perform_shapedfa_on_intrablock ();

#ifdef MYDEBUG
					fprintf(debug_file,"\n----------------- end processing of bb%d---------------------\n",cbb->index);
#endif
#ifdef DUMP_IN_DUMPFILE
					if(in_of_bb(cbb))
					{
						fprintf(dump_file,"\nIN of bb%d\n",cbb->index);
						printDataFlow(0,dump_file,in_of_bb(cbb)->shape_info->gen);
						printDataFlow(1,dump_file,in_of_bb(cbb)->shape_info->gen);
						printAttribute(dump_file,in_of_bb(cbb)->shape_info->gen->attribute);
					}
					if(out_of_bb(cbb))
					{   
						fprintf(dump_file,"\nOUT of bb%d\n",cbb->index);
						printDataFlow(0,dump_file,out_of_bb(cbb)->shape_info->gen);
						printDataFlow(1,dump_file,out_of_bb(cbb)->shape_info->gen);
						printAttribute(dump_file,out_of_bb(cbb)->shape_info->gen->attribute);
					}
#endif		
				}
				else {
					cnode = next_node_from_shape_worklist (shape_node);
#ifdef MYDEBUG
					if(cnode)
						fprintf(debug_file,"\n the cnode added is %s \n",cgraph_node_name(cnode));	   
#endif	

					process_bb = false;
				}
			}
			else
				process_bb = false;
#ifdef DUMP_IN_DUMPFILE	   
			fclose(debug_file);
#endif		
		}
	}
}





 void init_shapeAnalysis()
{
  
  if (main_firstbb) {   //mine-- this is the first basic block of main function
       cnode = cnode_of_bb (main_firstbb);
       
       initialize_fn_worklist();  //mine-- global worklist
                                  //start,shape_node are
                                  //initialised!!!!!!  also in them
                                  //the startblock of main are added
                                  //to the worklist by saying
                                  //process_shape=TRUE for that
                                  //block!!!!!!
       
        ((bb_csipdfa_info)(start_bb_of_fn (cnode)->aux))->in_ipdfa =
            initialize_in();   //mine--Initialize the dataflow values        
   }
  
}

csdfa_info initialize_in()
{
  
  csdfa_info temp = initialize_csdfa_info();
  temp->new_shape_df=TRUE;
  temp->cs_index=-1;
  return temp;
}


 void init_info (void)
{
  //add anything required later
  
  
//    init_alias_heapvars ();
//    cs_init_alias_vars ();

   for (cnode = cgraph_nodes; cnode; cnode = cnode->next) {
//        struct cgraph_node *alias;
//        csvarinfo_t vi;

       // Nodes without a body, and clone nodes are not interesting. 
       if (!gimple_has_body_p (cnode->decl) || cnode->clone_of)
           continue;

       //locating main function. 
#ifdef CPP	   
       if (strcmp (cgraph_node_name (cnode), "int main(int, char**)") == 0)
#else
       if (strcmp (cgraph_node_name (cnode), "main") == 0)
#endif	 
           main_firstbb = start_bb_of_fn (cnode);

      //mine---in this func a variable is created for the function and one for each argument
     //after that the pair(corresponding tree,variable) is inserted in the map vi_for_tree
//        vi = cs_create_func_info_for (cnode->decl, cgraph_node_name (cnode));                    //mine-- not required!!!!

       // Associate the varinfo node with all aliases.  
     //mine--same_body= For normal nodes pointer to the list of alias and thunk nodes,in alias,thunk nodes pointer to the normal node.
    //according to struct cgraph_node..
//        for (alias = cnode->same_body; alias; alias = alias->next)
//            cs_insert_vi_for_tree (alias->decl, vi);
   }
}


long long timeval_diff(struct timeval *difference,struct timeval *end_time,struct timeval *start_time)
{
  struct timeval temp_diff;

  if(difference==NULL)
  {
    difference=&temp_diff;
  }

  difference->tv_sec =end_time->tv_sec -start_time->tv_sec ;
  difference->tv_usec=end_time->tv_usec-start_time->tv_usec;

  /* Using while instead of if below makes the code slightly more robust. */

  while(difference->tv_usec<0)
  {
    difference->tv_usec+=1000000;
    difference->tv_sec -=1;
  }

  return 1000000LL*difference->tv_sec+
                   difference->tv_usec;

} /* timeval_diff() */

char *strrev(char *str) {
    char *p1, *p2;

    if (!str || !*str)
        return str;

    for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2) {
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
    }

    return str;
}

char *itoa(int n, char *s) 
{
    int b=10;  //base =10;
    static char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    int i=0, sign;
    
    if ((sign = n) < 0)
        n = -n; 

    do {
        s[i++] = digits[n % b]; 
    } while ((n /= b) > 0); 

    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';

    return strrev(s);
}

