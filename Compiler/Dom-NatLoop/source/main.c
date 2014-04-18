#include<stdio.h>
#include<stdlib.h>
#include"header.h"

/*INPUT      : The Graph, Entry Node and Number Of Vertices Of Graph are read from user supplied file.
**OUTPUT     : The Dominator Tree, and the Natuaral Loop for all Back Edges are wriiten in output file 
**             "output.Info" 
**ASSUMPTION : If Number Of Vertices Of Graph = N, then the vertices need to be labelled from 1 - N , 
**             in any order. 
*/

int main(int argc, char** argv)

{
    if(argc < 2) {
      fprintf(stderr, "Usage ./a.out  <Inputfile>\n");
      exit(0);
    }
    FILE* inputFp = fopen(argv[1], "r");
    outputFp = fopen("Output.Info", "w");
    if(NULL == inputFp || NULL == outputFp) {
        fprintf(stderr, "Error Opening File...\n");
        exit(0);
    }

    /*** Initialization of Data Structures implementing Adjescency matrix and Dominator info ***/
    InitializationRoutine(inputFp);


    /*** Determining Dominator For Each Node ***/
    createDominatorInfo();
 
    printfDominatorInfo();
    

    /*** Creating  Dominator Tree ***/
    createDominatorTree();

    fprintf(outputFp,"\t\t\t--------------------\n");
    fprintf(outputFp,"\t\t\t The Dominator Tree\n");
    fprintf(outputFp,"\t\t\t--------------------\n\n");
    printDominatorTree(ENTRY_NODE,1);	
    fprintf(outputFp,"\n\n");
	debugInfo();


    /*** Finding Backedges And The Natural Loop For Each BackEdge ***/
    findBackEdgesAndNaturalLoops();

    printNaturalLoops();


    /*** Check For The Possibility Of Loop Merging ***/
    mergeLoops();

    printfMergeInfo();


    fprintf(stdout,"Please Open File \"Output.Info\" to get the Output\n\n");
    

    /*** Deallocating Memory Of The Data Structures Used ***/ 
    cleanUpRoutine(inputFp);
    return 0;
}
