#include "prime.decl.h"

/*readonly*/ CProxy_Master mainProxy;
/*readonly*/ int M;
/*readonly*/ int k;

struct Master : public CBase_Master {
  int count;
  int total_num_primes;
  Master(CkArgMsg* msg) {
    mainProxy= thisProxy;
    count = 0;
    total_num_primes = 0;
    M = atoi(msg->argv[1]);
    k = atoi(msg->argv[2]);
    for (int i = 0; i < k; i++)
      CProxy_Worker::ckNew(i);
  }
  void addContribution(int numPrimes){
    total_num_primes += numPrimes;
    count++;
    if(count == k){
        CkPrintf("Total number of primes within the range [0 - %d] is %d.\n", M*k, total_num_primes);
        CkExit();
    }
  }

};

struct Worker : public CBase_Worker {
  int prime_count;
  Worker(int worker_index) {
    prime_count = 0;
    for(int i=M*worker_index; i<((worker_index+1)*M); i++){
        if(i%2 != 0){
            prime_count += isPrime(i); 
        }
    }
    mainProxy.addContribution(prime_count);
  }

  int isPrime(int number) {
    if(number<=1)
      return 0;
    
    for(int i=2; i<number; i++){
   
      if(0 == number%i)
          return 0;
    }
    return 1;
  }

};

#include "prime.def.h"
