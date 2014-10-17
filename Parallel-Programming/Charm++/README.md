General
========
1. readonly  CProxy_Master name; // This name should not be mainchare
2. All the entry methods should be public
3. Add Worker(CkMigrateMessage*) {}
4. Given a CProxy_worker arra; you cannot do array[index].memberVar. The only way to get  a member var is to pass it using a entry method.
5. The place where we do mainProxy.entryM(); the control does not go to the mainchare, but instead it will call in asuynchronously and go ahead, so better make sure that the following code has a cleaner exit in case you do  not want to do anything else after calling mainchare entry method.

SDAG
=====
1.  structure dagger entry method in ci file should end with semicolon
2.  handy to use wrap (x) ((x + n) %n) 
3.  for reduction with logical_and use int as the contributing elements
4.  Dont forget to the following
    ``` C++
    class Worker: public CBase_Worker {
      Worker_SDAG_CODE
      public:
    
        Worker() {
          __sdag_init();
        }

        void pup(PUP::er &p) {
          CBase_Worker::pup(p);
          __sdag_pup(p);
        }
    }
  ```
  








To Dos
========
1. Quicense detection
2. 
