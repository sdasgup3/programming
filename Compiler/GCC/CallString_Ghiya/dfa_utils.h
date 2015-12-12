//these are included because of in this sys(my lappy) i didnot use the gdfa patch which these belong
int find_index_bb (basic_block bb);
void report_dfa_spec_error (const char *mesg);
//----------------------------------------------------------
#define ENTITY_INDEX(node) (node).common.typed.base.index
#define D(stmt,type,i,j) (((current_pfbv_dfi_stmt[find_index_of_local_stmt(stmt)])->type->direction)[i][j])
#define I(stmt,type,i,j) (((current_pfbv_dfi_stmt[find_index_of_local_stmt(stmt)])->type->interference)[i][j])
#define E(stmt,type,i) ((current_pfbv_dfi_stmt[find_index_of_local_stmt(stmt)])->type->equation)[i]
#define BV(stmt,type,p,q,f)  ((current_pfbv_dfi_stmt[find_index_of_local_stmt(stmt)])->type->boolVariables)[p][q][f]
#define D(stmt,type,i,j) (((current_pfbv_dfi_stmt[find_index_of_local_stmt(stmt)])->type->direction)[i][j])
#define I(stmt,type,i,j) (((current_pfbv_dfi_stmt[find_index_of_local_stmt(stmt)])->type->interference)[i][j])
// #define A(stmt,type,i)  (((current_pfbv_dfi_stmt[find_index_of_local_stmt(stmt)])->type->attribute)[i])
#define E(stmt,type,i) ((current_pfbv_dfi_stmt[find_index_of_local_stmt(stmt)])->type->equation)[i]

int find_index_of_local_stmt(gimple stmt);
 void initialize_some_dataFlowValue(dataFlowValue **temp);
void assign_indices_to_stmt(void);
 int isItInThe8Stmts (gimple stmt);

 int getFieldIdInItsStructGivenName(const char *fieldPtr,char *structName);
 int isStructPtr(tree var);
 char * getDummyName(tree var);
 char * getTheName(tree var);
 bool is_pointer_type (tree type);
 bool is_pointer_var (tree var);
int getFieldIdGivenName(char *fieldPtr,char *structName);
 void assignRightToLeft_dataFlowValue(dataFlowValue *left,dataFlowValue *right);
// void assignRightToLeft_BoolEq(bool_Equations *left,bool_Equations *right,int dagOrCycle);

void initialize_pfbv_dfi(pfbv_dfi **p);
void assignRightToLeft_shape_info(pfbv_dfi **left,pfbv_dfi *right);
void allocate_space (pfbv_dfi*** dfi, int number_of_nodes);
dataFlowValue * transfer_fn(dataFlowValue * gen,dataFlowValue * in,dataFlowValue * kill);
// void makeICellPHI(node_I_MAT_CELL *ICell,int pId,int qId);
// void makeDCellPHI(node_D_MAT_CELL *DCell,int pId);
 void generateGENKILLvalues(int stmtType,gimple stmt,dataFlowValue *inStmt);
// void selectAlphaBetaWhereAlphaIsF(node_I_MAT_CELL *kill,node_I_MAT_CELL *input,int pId,int s,char *fieldName);
// void fieldSpecificData(char *fieldName,int pId,node_D_MAT_CELL * temp,node_D_MAT_CELL *input);
// void cross(node_D_MAT_CELL *leftD,node_I_MAT *rightBASE,node_I_MAT_CELL *Iresult,int p,int q);
 int findNameAndGetId_q(int stmtType , gimple stmt);
 bool is_new_dfvalue (dataFlowValue *prev_info, dataFlowValue * new_info);
// void cleanDirectionCell(node_D_MAT_CELL *D,int i);
// void cleanInterferenceCell(node_I_MAT_CELL *I,int i,int j);
// node_D_MAT_CELL * replaceEpsilonByField(int D_or_I, node_D_MAT_CELL *D1,int pId,int fieldId,char *fieldName);
// void limit_dfvalue_to_K(dataFlowValue *df);
 void free_dataFlowValue(dataFlowValue **temp);
void free_pfbv_dfi(pfbv_dfi **P);
// int isThisEntryEpsilon(node_D_MAT_CELL *D1,int pId);
char *getStructName(tree var);
// node_I_MAT_CELL * cross_2_DMat(node_D_MAT_CELL *D1,node_D_MAT_CELL *D2,int d1,int d2);
 void initialize_some_dataFlowValue_at_entry(dataFlowValue **temp);
 int getId(const char * heapPtrName,struct cgraph_node *cnode);

void printDataFlow(int di,FILE *file,dataFlowValue *value);
 void initialize_some_dataFlowValue_malloc(dataFlowValue **temp);
/*--------------------------------------------------------------------*/
