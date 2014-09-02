struct Worker : public CBase_Worker {
  int prime_count;
  Worker(int worker_index) {
    prime_count = 0;
    for(int i=M*worker_index; i<((worker_index+1)*M); i++){
        if(i%2 != 0)
            prime_count += isPrime(i); 
    }
    mainProxy.addContribution(prime_count);
  }

  int isPrime(int number) {
    if(number<=1) return 0;
    
    for(int i=2; i<number; i++){
      if(0 == number%i) return 0;
    }
    return 1;
  }
};
#include "prime.def.h"
