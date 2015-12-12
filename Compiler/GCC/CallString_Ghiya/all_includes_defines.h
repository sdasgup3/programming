#ifndef _ALL_INC_DEFS_H_
#define _ALL_INC_DEFS_H_
#include "gcc-plugin.h"
#include "gimple-pretty-print.h"
#include "input.h"
#include "config.h"
#include <stdio.h>
#include <sys/resource.h>
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tm_p.h"
#include "diagnostic.h"
#include "tree-flow.h"
#include "tree-pass.h"
#include "toplev.h"
#include "ggc.h"
#include "cgraph.h"
#include "function.h"


#include "timevar.h"

#include "params.h"
#include "ggc.h"
#include "string.h"

/**********************************************/
/*The following macros are for debug purposes. Uncommenting DUMP_IN_DUMPFILE require adding -fdump-ipa-all switch*/
//#define DUMP_IN_DUMPFILE
//#define MYDEBUG       //for debugInfo on debug_file
//#define MYDEBUG_1     //this is for stdout debugInfo
/**********************************************/
#include "globalVarAndStructs.h"
#include "bb_cfg_cnode_functions.h"    //contains all bb,cfg,cnode related functions

// #include "matrix_functions.h"
// #include "equations_functions.h"
#include "dfa_utils.h"
#include "otherFunctions.h"
#endif /* _ALL_INC_DEFS_H_ */
