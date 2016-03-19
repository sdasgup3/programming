#include <stdio.h>
#include <math.h>
#include "unifyChareLoad.decl.h" //based on module name
//#define DEBUG 

/*readonly*/ CProxy_Main mainProxy;
/*readonly*/ CProxy_ChareElem chareArray;
/*readonly*/ int chare_array_size;
/*readonly*/ int min;
/*readonly*/ int max;

class Main : public CBase_Main {
  private:
    int checksum_before_balance;

  public:
  Main(CkArgMsg* msg) {

    if(msg->argc < 4) {
      CkPrintf("\n\n\nUsage: <exec> chare_array_size min max\n");
      CkPrintf("\twhere chare_array_size > 0 \n\n\n");
      CkExit();
    }

    chare_array_size  = atoi(msg->argv[1]);
    min               = atoi(msg->argv[2]);
    max               = atoi(msg->argv[3]);

    if(0 == chare_array_size) {
      CkPrintf("\n\n\n\"chare_array_size\" must be greater than 0 \n\n\n");
      CkExit();
    } else if(0 == (max - min)) {
      CkPrintf("\n\n\nUsage: <exec> chare_array_size min max\n");
      CkPrintf("\t (max - min) shoud not be equal to  0\n\n\n");
      CkExit();
    }

    checksum_before_balance = 0;

    mainProxy= thisProxy;

    delete msg;

    chareArray = CProxy_ChareElem::ckNew(chare_array_size);

    CkCallback cb = CkCallback(CkReductionTarget(Main, Qdetect2), thisProxy); 
    CkStartQD(cb);

  }

  void Qdetect2() {
    CkPrintf("Quiescence2 detecetd\n");
    CkExit();
  }

  Main(CkMigrateMessage* msg) {}

  void collectAverage(int total_num_elem) { 

    if(total_num_elem < chare_array_size) {
      CkPrintf("\n\n\n Total Number of Elements (%d) must be greater than number of chares (%d)\n\n\n", total_num_elem, chare_array_size );
      CkExit();
    }

    double avg = ((double) total_num_elem) /  chare_array_size;
    CkPrintf("\nAverage: %f Total %d\n\n", avg, total_num_elem);
    chareArray.unifyLoad(avg);
  }

  void collectInitChecksum(int initCheckSum) { 
    checksum_before_balance = initCheckSum;
  }

  void done(int finalCheckSum) { 
    if(finalCheckSum == checksum_before_balance) {
      CkPrintf("\n\nCorrectness Criteria PASSED (After Balance Checksum: %d Before Balance Checksum: %d)\n", finalCheckSum, checksum_before_balance); 
    } else { 
      CkPrintf("Correctness Criteria FAILED %d %d\n",finalCheckSum, checksum_before_balance); 
    } 
    CkPrintf("Exiting...\n\n");
  }
};

class ChareElem: public CBase_ChareElem {
  private:
    int num_elems;      /*number of elems the ith chare array element owns*/
    int new_num_elems;  /*number of elems the ith chare array element owns after load balancing*/

    int *elems;
    int *new_elems;

    int start_index_after_ldb;
    int end_index_after_ldb;

    double avg;
    int int_avg;

    int exclusiveParPrefix;
    int parPrefix;

    int local_checksum;

    /*The private variables needed to perform parallel prefix*/
    int dist;

  public:
  ChareElem_SDAG_CODE  
  ChareElem() {
    __sdag_init();

    srand(time(NULL) + thisIndex); 
    num_elems = rand()%(max - min) + min;

    elems = (int *) mymalloc(sizeof(int) * num_elems);
    for(int k = 0; k < num_elems ; k++) {
      elems[k] = rand() ;
    }

    new_num_elems = 0;
    new_elems = NULL;
    local_checksum = 0;

    /* Finding the parallel prefix of num_elems over chare array */
    parPrefix = num_elems;
    step();
  }

  void unifyLoad(double average) {

#ifdef DEBUG
    CkPrintf("%d in Unify\n", thisIndex);
#endif
    avg = average;
    int_avg = avg;

    if(num_elems > 0) {
      int nextTarget;
      int end_index;

      int glob_index = exclusiveParPrefix;
      int targetChare = targetResolution(glob_index);

      int start_index  = glob_index;
      int *collectValueToSameDest = (int *) mymalloc(sizeof(int)*(2*int_avg));
      int cnt = 0;
      collectValueToSameDest[cnt++] = elems[0];

      for(int i = 1 ; i < num_elems; i++) {
        glob_index = i + exclusiveParPrefix;
        nextTarget = targetResolution(glob_index);

        if(nextTarget == targetChare) {
          collectValueToSameDest[cnt++]  = elems[i];
        } else {
          end_index = glob_index   - 1 ;

#ifdef DEBUG
          CkPrintf("[%d] sends [%d - %d] to %d\n ", thisIndex, start_index, end_index, targetChare);
#endif 
          chareArray[targetChare].recvPacket(start_index, end_index, collectValueToSameDest, avg); 

          targetChare = nextTarget;
          start_index = glob_index;
          cnt = 0;
          collectValueToSameDest[cnt++] = elems[i];
        }
      }
      end_index = glob_index;
#ifdef DEBUG
      CkPrintf("[%d] sends [%d - %d] to %d\n ", thisIndex, start_index, end_index, targetChare);
#endif 
      chareArray[targetChare].recvPacket(start_index, end_index, collectValueToSameDest, avg); 
      free(collectValueToSameDest);
    }
  }

  int targetResolution(int glob_index) {
    int maybeTarget = glob_index /  int_avg;

    for (int t = maybeTarget; t >= 0 ; t --) {
      int min_index = (int ) ( t * avg) ;
      int max_index = (int ) ( (t + 1 ) * avg - 1 ) ;

      if(glob_index >= min_index && glob_index <= max_index) {
        if(t >= chare_array_size) {
          return chare_array_size -1;
        } else {
          return t;
        }
      } 
    }
    CkAbort("Target not found");
  }

  void recvPacket(int start_index, int end_index, int * collectValueToSameDest, double average) {
#ifdef DEBUG
    CkPrintf("%d in Receive\n", thisIndex);
#endif 
    new_num_elems += (end_index - start_index + 1) ;

    /*Find the start and end index of elements to be owned by thisIndex once the avg is known*/
    if(NULL == new_elems) {
      avg = average;
      start_index_after_ldb = (int ) ( thisIndex * avg) ;
      end_index_after_ldb   = (int ) ( (thisIndex + 1 )* avg - 1 ) ;
      if(thisIndex == chare_array_size - 1) {
        end_index_after_ldb = exclusiveParPrefix + num_elems - 1 ;
      }

      /* Allocate new storage of size average */
      new_elems = (int *) mymalloc(sizeof(int)* (end_index_after_ldb - start_index_after_ldb + 1));
    }

    for(int j = 0, placement = (start_index - start_index_after_ldb) ; j < (end_index - start_index + 1 ); j++) {
      new_elems[placement++] = collectValueToSameDest[j];
    }

#ifdef DEBUG
    CkPrintf("Chare [%d]: [ %d - %d] -> %d (%d) attempting after balance \n", thisIndex, start_index, end_index, new_num_elems,end_index_after_ldb - start_index_after_ldb + 1  );
#endif
    if(new_num_elems == (end_index_after_ldb - start_index_after_ldb + 1 )) {

      CkPrintf("After Balance: Chare [%d]: [ %d - %d] -> %d values \n", thisIndex, start_index_after_ldb, end_index_after_ldb, new_num_elems);
      local_checksum = 0;
      for(int i = 0;   i< new_num_elems  ; i ++) {
#ifdef DEBUG
        CkPrintf("%d ", new_elems[i]);
#endif
        local_checksum ^= new_elems[i]; 
      }
#ifdef DEBUG
      CkPrintf("\n");
#endif

      CkCallback cb_3(CkReductionTarget(Main, done), mainProxy);
      contribute(sizeof(int), &local_checksum, CkReduction::bitvec_xor , cb_3);
    } 
  }


  ChareElem (CkMigrateMessage*) {};

  void* mymalloc(int size) {
    void * mem = malloc(size);
    if(mem == NULL) {
      CkAbort("Insuficient Memory to allocate");
    }
    return mem;
  }
};


#include "unifyChareLoad.def.h" //based on module name
