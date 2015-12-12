#include "all_includes_defines.h"

void table_the_global_ptr_variables(void);
void fillTable();
int searchStructName(char *s,int Typedef);
int searchPtrName(const char *s);
void table_ptr_details(tree var);
void print_the_table(void);
void print_the_field_Table(void);
void table_the_local_ptr_variables();
char *getTheName(tree var);

void fillTable()
{
	tableOfHeaps=(struct ptr_to_struct_table *)xmalloc(field_Table_Size *sizeof(struct ptr_to_struct_table));
	field_Table=(struct ptr_to_field_of_struct_table *)xmalloc(tableOfHeaps_Size *sizeof(struct ptr_to_field_of_struct_table));
	table_the_global_ptr_variables();
	for (cnode=cgraph_nodes; cnode; cnode=cnode->next) {
		if (!gimple_has_body_p (cnode->decl) || cnode->clone_of)
			continue;
		push_cfun (DECL_STRUCT_FUNCTION (cnode->decl));
		table_the_local_ptr_variables();
#ifdef DUMP_IN_DUMPFILE    
		fprintf(dump_file,"\t\t%s\n\n",current_function_name());
		print_the_table();
		fprintf(dump_file,"\n -------------------------------------\n");
		print_the_field_Table();
#endif  
		pop_cfun();
	}
}

void table_the_global_ptr_variables(void)
{
	struct varpool_node *node;
	global=1;
	for (node=varpool_nodes; node; node=node->next) {
		tree var = node->decl;
		if (!DECL_ARTIFICIAL (var))
		{
#ifdef DUMP_IN_DUMPFILE
			if (dump_file)
				fprintf (dump_file, "Global variable : %s\n", get_name (var));
#endif
			table_ptr_details(var);
		}	
	}
	global=0;
#ifdef DUMP_IN_DUMPFILE
	if (dump_file)
		fprintf (dump_file, "\n");
#endif   
}

void table_the_local_ptr_variables()
{
	tree t;
    unsigned i;
    FOR_EACH_LOCAL_DECL(cfun, i, t)
	{
		table_ptr_details(t);
	}
	tree arg;
	for (arg = DECL_ARGUMENTS (cfun->decl); arg; arg = TREE_CHAIN (arg))
	{
		table_ptr_details(arg);
	}

}

void table_ptr_details(tree var)
{
	int idInItsStruct;
	if(global)   //as table_the_global_ptr_variables function is called once for each function we need to eliminate repetition
	{
		if(searchPtrName(get_name(var)))
			return;
	}	
	if(TREE_CODE(TREE_TYPE(var))==ARRAY_TYPE)
	{
        if(TREE_CODE(TREE_TYPE(TREE_TYPE(var)))==POINTER_TYPE)
        {	   
            if(TREE_CODE(TREE_TYPE(TREE_TYPE(TREE_TYPE(var)))) == RECORD_TYPE
	              || !get_name(var)) {
                tableOfHeaps[noOfHeapPtrs].isArray=1;
                tree z=TYPE_NAME(TREE_TYPE(TREE_TYPE(TREE_TYPE(var))));
                if(TREE_CODE(z)==IDENTIFIER_NODE)
                {
                    strcpy(tableOfHeaps[noOfHeapPtrs].structName,IDENTIFIER_POINTER(z));
                    tableOfHeaps[noOfHeapPtrs].isTypedef=0;
                }
                else
                {
                    const char *s=IDENTIFIER_POINTER(DECL_NAME(z));
                    strcpy(tableOfHeaps[noOfHeapPtrs].structName,s);
                    tableOfHeaps[noOfHeapPtrs].isTypedef=1;
                }			

                if(get_name(var))    
                    strcpy(tableOfHeaps[noOfHeapPtrs].ptrName,get_name(var));
                else         					//for dummy variables
                { 
                    strcpy(tableOfHeaps[noOfHeapPtrs].ptrName,getTheName(var));
                }

                if(!global)
                {
                    tree z1=DECL_CONTEXT(var);
                    tree scope=DECL_NAME(z1);
                    if(scope)
                    {
                        const char *s= IDENTIFIER_POINTER(scope);
                        strcpy(tableOfHeaps[noOfHeapPtrs].scope,s);
                    }
                    else
                    {
                        strcpy(tableOfHeaps[noOfHeapPtrs].scope,"0");
                    }

                }
                else
                {
                    strcpy(tableOfHeaps[noOfHeapPtrs].scope,"0");
                }
                tableOfHeaps[noOfHeapPtrs].id=noOfHeapPtrs;	
                tree field=TYPE_FIELDS(TREE_TYPE(TREE_TYPE(TREE_TYPE(var))));
                int anotherCount=0;
                while (field)
                {
                    if (TREE_CODE (field) == FIELD_DECL)
                    {		
                        if(   (TREE_CODE(TREE_TYPE(field))==POINTER_TYPE) &&   (TREE_CODE(TREE_TYPE(TREE_TYPE(field)))==RECORD_TYPE)  )
                        {
                            anotherCount++; //count no of fields
                        }
					
                        if((TREE_CODE(TREE_TYPE(field))==ARRAY_TYPE) && (TREE_CODE(TREE_TYPE(TREE_TYPE(field)))==POINTER_TYPE)
                           && (TREE_CODE(TREE_TYPE(TREE_TYPE(TREE_TYPE(field))))==RECORD_TYPE) )
                            anotherCount++;
						
                    }
                    field= TREE_CHAIN (field);
                }
                tableOfHeaps[noOfHeapPtrs].noOfFields=anotherCount;
                noOfHeapPtrs++;
                if(noOfHeapPtrs==tableOfHeaps_Size-1)
                {
                    tableOfHeaps_Size=tableOfHeaps_Size+100;
                    tableOfHeaps=(struct ptr_to_struct_table *)xrealloc(tableOfHeaps,tableOfHeaps_Size *sizeof(struct ptr_to_struct_table));
                }
                z=TYPE_NAME((TREE_TYPE(TREE_TYPE(TREE_TYPE(var)))));
                char *s;
                int isTypedef;
                if(TREE_CODE(z)==IDENTIFIER_NODE)
                {

                    s=(char *)IDENTIFIER_POINTER(z);
                    isTypedef=0;
                }
                else
                {
                    s=(char *)IDENTIFIER_POINTER(DECL_NAME(z));
                    isTypedef=1;
                }
                if(searchStructName(s,isTypedef))
                {

                    return;
                }
                else
                {
                    field=TYPE_FIELDS(TREE_TYPE(TREE_TYPE(TREE_TYPE(var))));
                    idInItsStruct=0;
                    while (field)
                    {
                        if (TREE_CODE (field) == FIELD_DECL)
                        {
                            int isArray=0;
                            int tempFlag=0;
                            if((TREE_CODE(TREE_TYPE(field))==POINTER_TYPE) &&   (TREE_CODE(TREE_TYPE(TREE_TYPE(field)))==RECORD_TYPE)  )
                            {
                                isArray=0;
                                tempFlag=1;
						  
                            }
                            else if( (TREE_CODE(TREE_TYPE(field))==ARRAY_TYPE) &&  (TREE_CODE(TREE_TYPE(TREE_TYPE(field)))==POINTER_TYPE) &&  
                                     (TREE_CODE(TREE_TYPE(TREE_TYPE(TREE_TYPE(field))))==RECORD_TYPE)  )
                            {
                                isArray=1;
                                tempFlag=1;
                            }
					        
					        if(tempFlag==1)
                            {
						  
                                char *s;	
                                strcpy(field_Table[noOfFieldPointers].ptrFieldName,get_name(field));   
                                tree temp;
						  
                                temp=isArray?TYPE_NAME(TREE_TYPE(TREE_TYPE(TREE_TYPE(field)))):TYPE_NAME(TREE_TYPE(TREE_TYPE(field)));
                                if(TREE_CODE(temp)==IDENTIFIER_NODE)
                                {

                                    strcpy(field_Table[noOfFieldPointers].pointsTo,IDENTIFIER_POINTER(temp));
                                }
                                else
                                {				
                                    const char *s=IDENTIFIER_POINTER(DECL_NAME(temp));
                                    strcpy(field_Table[noOfFieldPointers].pointsTo,s);
                                    tableOfHeaps[noOfHeapPtrs].isTypedef=1;	
                                }
						  
                                tree z=TYPE_NAME(TREE_TYPE(TREE_TYPE((TREE_TYPE(var)))));
    
                                if(TREE_CODE(z)==IDENTIFIER_NODE)
                                {	
                                    s=(char *)IDENTIFIER_POINTER(z);
                                    field_Table[noOfFieldPointers].idInItsStruct=idInItsStruct;
                                    strcpy(field_Table[noOfFieldPointers].structName,s);                     //structName
                                    field_Table[noOfFieldPointers].isTypedef=0;                 
                                }
                                else   //typedef declaration
                                {
                                    s=(char *)IDENTIFIER_POINTER(DECL_NAME(z));
                                    field_Table[noOfFieldPointers].idInItsStruct=idInItsStruct;
                                    strcpy(field_Table[noOfFieldPointers].structName,s);						
                                    field_Table[noOfFieldPointers].isTypedef=1;   
                                }
							
                                if(isArray)
                                    field_Table[noOfFieldPointers].isArray=1;
                                else
                                    field_Table[noOfFieldPointers].isArray=0;
							
                                field_Table[noOfFieldPointers].id=noOfFieldPointers;                              //id
                                idInItsStruct++;
                                noOfFieldPointers++;			
						  
                            }
					        					  						
                        }
                        field= TREE_CHAIN (field);
                    }
                } //else end
	      
            }	    
        }	  
	}
	
	if(TREE_CODE(TREE_TYPE(var))==RECORD_TYPE)              // for filling fieldTable;
	{
		tree z=TYPE_NAME(TREE_TYPE(var));
		char *s;
		int isTypedef;
		if(TREE_CODE(z)==IDENTIFIER_NODE)
		{
			s=(char *)IDENTIFIER_POINTER(z);
			isTypedef=0;
		}
		else
		{
			s=(char *)IDENTIFIER_POINTER(DECL_NAME(z));
			isTypedef=1;
		}
		if(searchStructName(s,isTypedef))
		{
			return;
		}
		else
		{

			tree field=TYPE_FIELDS(TREE_TYPE(var));
			idInItsStruct=0;
			while (field)
			{
				if (TREE_CODE (field) == FIELD_DECL)
				{
					if(   (TREE_CODE(TREE_TYPE(field))==POINTER_TYPE) &&   (TREE_CODE(TREE_TYPE(TREE_TYPE(field)))==RECORD_TYPE)  )
					{
						char *s;	
						strcpy(field_Table[noOfFieldPointers].ptrFieldName,get_name(field));            //ptrFieldName
						tree temp=TYPE_NAME(TREE_TYPE(TREE_TYPE(field)));                             //pointsTo 
						if(TREE_CODE(temp)==IDENTIFIER_NODE)
						{

							strcpy(field_Table[noOfFieldPointers].pointsTo,IDENTIFIER_POINTER(temp));
						}
						else
						{				
							const char *s=IDENTIFIER_POINTER(DECL_NAME(temp));
							strcpy(field_Table[noOfFieldPointers].pointsTo,s);
							tableOfHeaps[noOfHeapPtrs].isTypedef=1;	
						}			
						tree z=TYPE_NAME(TREE_TYPE(var));
						if(TREE_CODE(z)==IDENTIFIER_NODE)
						{	
							s=(char *)IDENTIFIER_POINTER(z);
							field_Table[noOfFieldPointers].idInItsStruct=idInItsStruct;  
							strcpy(field_Table[noOfFieldPointers].structName,s);                     //structName
							field_Table[noOfFieldPointers].isTypedef=0;                 
						}
						else   //typedef declaration
						{
							s=(char *)IDENTIFIER_POINTER(DECL_NAME(z));
							field_Table[noOfFieldPointers].idInItsStruct=idInItsStruct;  
							strcpy(field_Table[noOfFieldPointers].structName,s);						
							field_Table[noOfFieldPointers].isTypedef=1;   
						}
						field_Table[noOfFieldPointers].id=noOfFieldPointers; //id
						idInItsStruct++;
						noOfFieldPointers++;			
					}			       
				}
				field= TREE_CHAIN (field);
			}
		}
	}

    //for filling table  and also field_Table
	if(TREE_CODE(TREE_TYPE(var))==POINTER_TYPE) {
		if(TREE_CODE(TREE_TYPE(TREE_TYPE(var))) == RECORD_TYPE
           || TREE_CODE(TREE_TYPE(TREE_TYPE(var))) == POINTER_TYPE
           || !get_name(var)) {
            tableOfHeaps[noOfHeapPtrs].isArray=0;
            tree z=TYPE_NAME(TREE_TYPE(TREE_TYPE(var)));
            if (z) {
                if(TREE_CODE(z)==IDENTIFIER_NODE) {
                    strcpy(tableOfHeaps[noOfHeapPtrs].structName,
                           IDENTIFIER_POINTER(z));
                    tableOfHeaps[noOfHeapPtrs].isTypedef=0;
                }
                else {
                    const char *s=IDENTIFIER_POINTER(DECL_NAME(z));
                    strcpy(tableOfHeaps[noOfHeapPtrs].structName,s);
                    tableOfHeaps[noOfHeapPtrs].isTypedef=1;
                }
            }
            
			if(get_name(var))    
				strcpy(tableOfHeaps[noOfHeapPtrs].ptrName,get_name(var));
			else { //for dummy variables
				strcpy(tableOfHeaps[noOfHeapPtrs].ptrName,getTheName(var));
			}
            
			if(!global) {
				tree z1=DECL_CONTEXT(var);
				tree scope=DECL_NAME(z1);
				if(scope) {
					const char *s= IDENTIFIER_POINTER(scope);
					strcpy(tableOfHeaps[noOfHeapPtrs].scope,s);
				}
				else {
					strcpy(tableOfHeaps[noOfHeapPtrs].scope,"0");
				}
			}
			else {
				strcpy(tableOfHeaps[noOfHeapPtrs].scope,"0");
			}
			tableOfHeaps[noOfHeapPtrs].id=noOfHeapPtrs;	
			tree field=TYPE_FIELDS(TREE_TYPE(TREE_TYPE(var)));
			int anotherCount=0;
			while (field)
			{
				if (TREE_CODE (field) == FIELD_DECL)
				{		
					if((TREE_CODE(TREE_TYPE(field))==POINTER_TYPE) &&
                       (TREE_CODE(TREE_TYPE(TREE_TYPE(field)))==RECORD_TYPE)) {
						anotherCount++; //count no of fields
					}
					
					if((TREE_CODE(TREE_TYPE(field))==ARRAY_TYPE)
                       && (TREE_CODE(TREE_TYPE(TREE_TYPE(field)))==POINTER_TYPE)
                       && (TREE_CODE(TREE_TYPE(TREE_TYPE(TREE_TYPE(field))))==RECORD_TYPE))
						anotherCount++;
				}
				field= TREE_CHAIN (field);
			}
			tableOfHeaps[noOfHeapPtrs].noOfFields=anotherCount;
			noOfHeapPtrs++;
			if(noOfHeapPtrs==tableOfHeaps_Size-1)
			{
				tableOfHeaps_Size=tableOfHeaps_Size+100;
				tableOfHeaps=(struct ptr_to_struct_table *)
                    xrealloc( tableOfHeaps,
                              tableOfHeaps_Size*sizeof(
                                  struct ptr_to_struct_table));
			}
			z=TYPE_NAME(TREE_TYPE(TREE_TYPE(var)));
            if (!z)
                return;
            
			char *s;
			int isTypedef;
			if(TREE_CODE(z)==IDENTIFIER_NODE)
			{
				s=(char *)IDENTIFIER_POINTER(z);
				isTypedef=0;
			}
			else
			{
				s=(char *)IDENTIFIER_POINTER(DECL_NAME(z));
				isTypedef=1;
			}
			if(searchStructName(s,isTypedef))
			{
				return;
			}
			else
			{
				field=TYPE_FIELDS(TREE_TYPE(TREE_TYPE(var)));
				idInItsStruct=0;
				while (field)
				{
					if (TREE_CODE (field) == FIELD_DECL)
					{
                        int isArray=0;
                        int tempFlag=0;
						if((TREE_CODE(TREE_TYPE(field))==POINTER_TYPE)
                           && (TREE_CODE(TREE_TYPE(TREE_TYPE(field)))==RECORD_TYPE))
						{
                            isArray=0;
                            tempFlag=1;
						}
						else if( (TREE_CODE(TREE_TYPE(field))==ARRAY_TYPE)
                                 && (TREE_CODE(TREE_TYPE(TREE_TYPE(field)))==POINTER_TYPE)
                                 && (TREE_CODE(TREE_TYPE(TREE_TYPE(TREE_TYPE(field))))==RECORD_TYPE))
						{
                            isArray=1;
                            tempFlag=1;
						}
                        
                        if(tempFlag==1)
						{
                            
                            char *s;	
                            strcpy(field_Table[noOfFieldPointers].ptrFieldName,get_name(field));   
                            tree temp;
                            
                            temp=isArray?TYPE_NAME(TREE_TYPE(TREE_TYPE(TREE_TYPE(field)))):TYPE_NAME(TREE_TYPE(TREE_TYPE(field)));
						  	if(TREE_CODE(temp)==IDENTIFIER_NODE)
							{       
								strcpy(field_Table[noOfFieldPointers].pointsTo,IDENTIFIER_POINTER(temp));
							}
							else
							{				
								const char *s=IDENTIFIER_POINTER(DECL_NAME(temp));
								strcpy(field_Table[noOfFieldPointers].pointsTo,s);
								tableOfHeaps[noOfHeapPtrs].isTypedef=1;	
							}
                            
                            tree z=TYPE_NAME(TREE_TYPE(TREE_TYPE(var)));
                            
							if(TREE_CODE(z)==IDENTIFIER_NODE)
							{	
								s=(char *)IDENTIFIER_POINTER(z);
								field_Table[noOfFieldPointers].idInItsStruct=idInItsStruct;
								strcpy(field_Table[noOfFieldPointers].structName,s);                     //structName
								field_Table[noOfFieldPointers].isTypedef=0;                 
							}
							else   //typedef declaration
							{
								s=(char *)IDENTIFIER_POINTER(DECL_NAME(z));
								field_Table[noOfFieldPointers].idInItsStruct=idInItsStruct;
								strcpy(field_Table[noOfFieldPointers].structName,s);						
								field_Table[noOfFieldPointers].isTypedef=1;   
							}
							if(isArray)
                                field_Table[noOfFieldPointers].isArray=1;
							else
                                field_Table[noOfFieldPointers].isArray=0;
							
							field_Table[noOfFieldPointers].id=noOfFieldPointers;                              //id
							idInItsStruct++;
							noOfFieldPointers++;   
						}
					}
					field= TREE_CHAIN (field);
				}
			}
		}
	}		
}

int searchPtrName(const char *s)
{
    
	int i;
	for(i=0;i<noOfHeapPtrs;i++)
	{
		if( (strcmp(tableOfHeaps[i].ptrName,s)==0) && (strcmp(tableOfHeaps[i].scope,"0")==0) )
			return 1;
	}
	return 0;
}

int searchStructName(char *s,int Typedef)
{
	int i;
	for(i=0;i<noOfFieldPointers;i++)
	{
		if((strcmp(field_Table[i].structName,s)==0) && (field_Table[i].isTypedef==Typedef))
			return 1;
	}
	return 0;
}
void print_the_table(void)
{
	int x;
	for(x=0;x<noOfHeapPtrs;x++)
	{
		fprintf(dump_file,"x=%d  id=%d   isTypeDef=%d   ptrName=%s   scope=%s    structName=%s noOfFields=%d isArray=%d \n",
                x,tableOfHeaps[x].id,tableOfHeaps[x].isTypedef,tableOfHeaps[x].ptrName,tableOfHeaps[x].scope,tableOfHeaps[x].structName,tableOfHeaps[x].noOfFields,tableOfHeaps[x].isArray);
	}
}
void print_the_field_Table(void)
{
	int x;
	for(x=0;x<noOfFieldPointers;x++)
	{
		fprintf(dump_file,"x=%d  id=%d   bit=%d   ptrName=%s   pointsTo= %s  structName=%s idInItsStruct=%d, isArray=%d\n",x,
                field_Table[x].id,field_Table[x].isTypedef,field_Table[x].ptrFieldName,field_Table[x].pointsTo,field_Table[x].structName,field_Table[x].idInItsStruct,field_Table[x].isArray);
	}
}


