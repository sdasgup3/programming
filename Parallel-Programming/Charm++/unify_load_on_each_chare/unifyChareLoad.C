#include <stdio.h>
#include <math.h>
#include "unifyChareLoad.decl.h" //based on module name

/*readonly*/ CProxy_Main mainProxy;
/*readonly*/ CProxy_ChareElem chareArray;
/*readonly*/ int chare_array_size;
/*readonly*/ int min;
/*readonly*/ int max;

typedef struct  {
  int *values;
  int start;
  int end;
} Packet;

class Main : public CBase_Main {
  public:
  Main(CkArgMsg* msg) {

    if(msg->argc < 3) {
      CkPrintf("Usage: <exec> chare_array_size min max\n");
      CkExit();
    }

    chare_array_size  = atoi(msg->argv[1]);
    min               = atoi(msg->argv[2]);
    max               = atoi(msg->argv[3]);
    mainProxy= thisProxy;

    delete msg;
              
    chareArray = CProxy_ChareElem::ckNew(chare_array_size);
  }

  Main(CkMigrateMessage* msg) {}
  void collectAverage(int total_num_elem) { 
    double avg = ((double) total_num_elem) /  chare_array_size;
    chareArray.unifyLoad(avg);
  }
};

class ChareElem: public CBase_ChareElem {
  private:
    int num_elems; //number of elems the ith chare array element owns
    int *elems;
    int *new_alloc;
    int new_alloc_size;
    double avg;
    int exclusiveParPrefix;
    int parPrefix;

    /*The private variables needed to perform parallel prefix*/
    int* valueBuf, *flagBuf, stage, numStages;

  public:
  ChareElem() {
    srand(time(NULL) + thisIndex); //To ASK
    num_elems = rand()%(max - min) + min;
    elems = (int *) malloc(sizeof(int) * num_elems);
    for(int k = 0; k < num_elems ; k++) {
      elems[k] = rand();
    }

    /* Finding the parallel prefix of num_elems over chare array */
    parPrefix = num_elems;
    stage =0;
    numStages = log2(chare_array_size);
    valueBuf  = (int *) malloc(sizeof(int)*numStages);
    flagBuf   = (int *) malloc(sizeof(int)*numStages);
    for(int i = 0 ; i < numStages; i ++ ) {
      valueBuf[i] = 0;
      flagBuf[i] = 0;
    }
    step(parPrefix);
  }

  void unifyLoad(double average) {
    avg = average;

    /* Test if all chares get equal share or total number of integers is  evenly divisible by chare_array_size*/
    int int_avg_1 = avg;
    int int_avg_2 = (int )(avg + 0.5);
    if(int_avg_1 == int_avg_2) {
      new_alloc_size = int_avg_1;
    } else {
      new_alloc_size = int_avg_2;
    }

    new_alloc = (int *) malloc(sizeof(int)* new_alloc_size);
  
    int nextTarget;
    int glob_index, end_index;

    int targetChare = exclusiveParPrefix / avg ;
    int start_index  = exclusiveParPrefix;
    int *collectValueToSameDest = (int *) malloc(sizeof(int)*new_alloc_size);
    int cnt = 0;
    collectValueToSameDest[cnt++] = elems[0];

    for(int i = 1 ; i < num_elems; i++) {
      glob_index = i + exclusiveParPrefix;
      nextTarget = glob_index / avg;
      if(nextTarget == targetChare) {
        collectValueToSameDest[cnt++]  = elems[i];
      } else {
        end_index = i   - 1 + exclusiveParPrefix;
        //To ASK: how to make malloc memory sharable
        Packet *p = (Packet *) malloc(sizeof(Packet));
        p.values  = collectValueToSameDest;
        p.start = start_index;
        p.end = end_index;
        if(thisIndex == targetChare) {
          for(int j = 0, placement = (start_index - thisIndex*new_alloc_size) ; j < (end_index - start_index); j++) {
            new_alloc[placement++] = collectValueToSameDest[j];
          }
        } else {
          chareArray[targetChare].recvPacket(p);
        }
        targetChare = nextTarget;
        start_index = glob_index;
        cnt = 0;
        collectValueToSameDest[cnt++] = elems[i];
      }
    }
  }

  void recvPacket(Packet* p) {
    for(int j = 0, placement = (p->start_index - thisIndex*new_alloc_size) ; j < (p->end_index - p->start_index); j++) {
      new_alloc[placement++] = p->collectValueToSameDest[j];
    }
  }

  void step(int value) {
    if(stage >= numStages) {
      exclusiveParPrefix = parPrefix - num_elems;
      CkPrintf("\n[%d].(num_elems: %d  parallel Prefix: %d ExcPar Prefix: %d) \n", thisIndex, num_elems, parPrefix, exclusiveParPrefix);
      CkCallback cb(CkReductionTarget(Main, collectAverage), mainProxy);// To ASK: the last chare is holding the total sum
      contribute(sizeof(int), &num_elems, CkReduction::sum_int, cb);
    } else {
      int sendIndex = thisIndex + (1 << stage);
      if(sendIndex < chare_array_size) {
        thisProxy[sendIndex].passValue(stage, parPrefix);
      }
      //if you no longer receive, but need to continue sending
      if(thisIndex - (1<<stage) < 0) {
        stage ++;
        step(parPrefix);
      }
    } 
  }

  void passValue(int incoming_stage, int incoming_value) {
    valueBuf[incoming_stage]  = incoming_value;
    flagBuf[incoming_stage]   = 1;

    for(int i = stage; i < numStages; i++) {
      if(flagBuf[i] == 0) break;

      parPrefix += valueBuf[i];
      stage ++;
      step(parPrefix);
    }
  }
  ChareElem (CkMigrateMessage*) {};
};



#include "unifyChareLoad.def.h" //based on module name
