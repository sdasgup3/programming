Grain Size
==========
* Tiny grain size over head on scheduling
* Chunkier grain size not enough parallelism
* tiny objects --> very good load balancing, but the load balancing strategy will take more time.
* Over decomposition gives benefit of better cache utilization: analogy of tiling
 
General
========
1. readonly  CProxy_Master name; // This name should not be mainchare
2. All the entry methods should be public
3. Add Worker(CkMigrateMessage*) {}
4. Given a CProxy_worker arra; you cannot do array[index].memberVar. The only way to get  a member var is to pass it using a entry method.
5. The place where we do mainProxy.entryM(); the control does not go to the mainchare, but instead it will call in asuynchronously and go ahead, so better make sure that the following code has a cleaner exit in case you do  not want to do anything else after calling mainchare entry method.
6. array [1D] Worker NOT chare [1D] ...
7. Worker(CkMigrateMessage* msg) {} //In C file only
8. In ci, Main constructor is also an entry method 
9. To enable generation of the serialization code, the entry method signatures must be declared in the interface file.
10. Since Charm++ runs on distributed memory machines, we cannot pass an array via a pointer in the usual C++ way. The array length expression is evaluated exactly once per invocation, on the sending side only. Thus executing the doGeneral method above will invoke the (user-defined) product function exactly once on the sending processor.
```C++
    entry void doLine(float data[n],int n);
    entry void doPlane(float data[n*n],int n);
    entry void doSpace(int n,int m,int o,float data[n*m*o]);
    entry void doGeneral(int nd,int dims[nd],float data[product(dims,nd)]);
```
11. For creating chare arrays
```C++
    CProxy_array A = CProxy_array::ckNew(p1, size); // size must be the last arg
    
    array::array(int p1) {}
```
12. MPI: a node have cores and which in turn has hardware threads. In MPI, if we have a 100 nodes each with 10 cores we run a 1000 rank mpi prog with each process on each core. Even if the cores share the same node, they still do message passing.
13. Adding more cores does not help in parallelism as the memory contemtion occurs as all the core will be shareing the memry bus and if there is lot of memory request then there might be contention.
14. setcpuaffinity: to say operating system that if a thread is cintext swicthed due to some reason then again host that in the same core. Hosting in other cores lead to loosing chached data.

SDAG
=====
* Why SDAG Imp :If there are dependencies between these entry methods within a single chare, they are specified implicitly in the code. Moreover, since Charm ++ does not guarantee any order on the delivery of messages, these dependencies must be specified at the target of the invocations. These dependencies are usually enforced through variables that track the state of the target chare, causing messages to be buffered until they are ready to be processed by it. 
* structure dagger entry method in ci file should end with semicolon
* handy to use wrap (x) ((x + n) %n) 
* for reduction with logical_and use int as the contributing elements
* Dont forget to the following
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
* 
```C++ 
when method1(paramenters) serial {
}
when method2(paramenters) serial { 
//The parameters of method1 will not be available to method2; Memory usage lower
}
//SO better do
when method2(p1), method (p2) serial {

}

```  

Reduction
=========
1. After the data is reduced, it is passed to you via a callback object. The message passed to the callback is of type CkReductionMsg . Unlike typed reductions, here we discuss callbacks that take CkReductionMsg* argument. The important members of CkReductionMsg are getSize() , which returns the number of bytes of reduction data; and getData() , which returns a ``void *'' to the actual reduced data. You may pass the client callback as an additional parameter to contribute .
```C++
    entry void myReductionEntry(CkReductionMsg* msg);    
    //In C
    double forces[2]=get_my_forces();
    CkCallback cb(CkIndex_myArrayType::myReductionEntry(NULL), thisProxy);
    contribute(2*sizeof(double), forces,CkReduction::sum_double, cb);
```   
```C++
    //For synchronisation purpose
    entry void myReductionEntry(CkReductionMsg* msg);    
    //In C
    CkCallback cb(CkIndex_myArrayType::myReductionEntry(NULL), thisProxy);
    contribute(cb);
```  
2. If no member passes a callback to contribute , the reduction will use the default callback. Programmers can set the default callback for an array or group using the **ckSetReductionClient(CkCallback*)** proxy call on **processor zero**, or by passing the callback to CkArrayOptions::setReductionClient() before creating the array. Again, a CkReductionMsg message will be passed to this callback, *which must delete the message when done*.
```C++
    //In ci
    entry void done();
    entry vod Resulr(CkReductionMsg*)
    //In C
    CkCallback *cb = new CkCallback(CkIndex_Main::Result(NULL),  mainProxy); //Or
    CkCallback *cb = new CkCallback(CkReductionTarget(Main,done), thisProxy);// If Typed reduction
    cellProxy.ckSetReductionClient(cb);
```
3. As above, the client entry method of a reduction takes a single argument of type CkReductionMsg. However, by giving an entry method the reductiontarget attribute in the .ci file, you can instead use entry methods that take arguments of the same type as specified by the contribute call. When creating a callback to the reduction target, the entry method index is generated by CkReductionTarget(ChareClass, method_name) instead of CkIndex_ChareClass::method_name(...). 
```C++
    //In .ci
    entry [reductiontarget] void done(CkReductionMsg*);
    entry vod Result(CkReductionMsg*)
    //In .C
    CkCallback cb(CkReductionTarget(mainChareClassName, done), mainProxy);  
    contribute(cb); // or
    CkCallback cb(CkIndex::Result(NULL), mainProxy);
    contribute(sizeof(int),&myInt,CkReduction::sum_int, cb)
```
4. Sync reductions:
```C++
//In .ci
entry void done(CkReductionMsg* );
//In C
contribute(CkCallback(CkIndex_Main::done(NULL), mainproxy));
            OR
//In .ci
entry [reductiontarget] void barrierH();
//In C
contribute (CkCallback(CkReductionTarget(Worker,  barrierH), workerarray)); 
            OR
//In .ci
entry void resumeIter();
//In .C            
CkCallback cb(CkIndex_Main::resumeIter(), mainProxy);
contribute(0, NULL, CkReduction::sum_int, cb);
            OR
//In .ci            
entry void doStep() 
//In C            
contribute(0, 0, CkReduction::concat, CkCallback(CkIndex_Stencil::doStep(), thisProxy));
```
5. Custom Reduction
 ```C++
 //C or .ci
 CkCallback cb = CkCallback(Ck_Index_Main::Result(NULL), mainProxy);
 contribute(3*sizeof(int), arr, customRType, cb);
 //In .ci
 initproc void reister(void);
 //In C, global space
 void register(void) {
  customType = CkReduction::addReducer(F);
 }
 CkReductionMsg* F(int n, CkReductionMsg** msg) {
  int reduced[3];
  for(int i =0; i< n; i++) {
   CkAssert(sizeof(msg[i]) == 3 * sizeof(int));
   ... = (int *)msg[i]->getData(); // returns an arr of three ints
  
  }
  return CkReductionMsg::buildNew(3*sizeof(int), reduced);
 }
 ```
 
Quiescence Detection
==========================
```C++
//In .ci
    entry void Qdetect(CkReductionMsg*);
    entry [reductiontarget]void Qdetect2();
//In .C
    Main::Main(CkArgMsg* msg) {    
        //CkStartQD(CkIndex_Main::Qdetect(NULL), &thishandle); or
        //CkCallback cb = CkCallback(CkIndex_Main::Qdetect(NULL), thisProxy); or
        CkCallback cb = CkCallback(CkReductionTarget(Main, Qdetect2), thisProxy); 
        CkStartQD(cb);
    }
    void Qdetect(CkReductionMsg* msg) {
        CkPrintf("Quiescence detecetd\n");
        CkExit();
    }
    void Qdetect2() {
        CkPrintf("Quiescence2 detecetd\n");
        CkExit();
    }
```

Threaded Entry Methods
===========================
1. This is a way to have thread sync. Note that The place where the chares elements are going after the reduction is their own respective thisIndex.barrierH(), where they have they owned CthThread t to awaken. After calling contribute the threads suspend them self till all the thread call contribute when the reduction happen and barrierH is called and all are awakened.
```C++
class Worker: public CBase_Worker  {

  public:
   
    CthThread t;
   
    void barrier() {
      contribute (CkCallback(CkReductionTarget(Worker,  barrierH), workerarray));
      t = CthSelf();
      CthSuspend();
    } 

    void barrierH() {
      CthAwaken(t);
    } 
```
2. Sync methods should always   **return message**
3. If respond is a sync method, then the following recurcive calls will serialize them as the second sync call cannot proceed before the first returns. 
    ```C++
    myMsg* n1 = w1.respond(n-1);
    myMsg* n2 = w1.respond(n-2);
    ```
4. Any method that calls a sync method must be able to suspend : threaded method of a chare C Can suspend, without blocking the processor, Other chares can then be executed, Even other methods of chare C can be executed
5. You can suspend a thread and “awaken” it
    * CthThread CthSelf(): Returns the ID of the (calling) thread 
    * CthSuspend: wrap it up, and give control to the scheduler. This is what happens underneath a “blocking invocation” of a sync method
    * CthAwaken(threadID):Put this thread in the list of ready threads (and other method invocations). Scheduler will run it when it comes to the head of the queue
6. Threads are still a cooperative rather than pre-emptive multi-threading, and it is still true that only a single method is actually running at a time on a give chare; but it is now possible for multiple method invocations to be “active” with suspended threads.
7. Threaded methods are used in (relatively rare) situations when the blocking wait happens deep from nested function calls, since structured dagger notation requires such waiting to be lifted to the top level of the control flow. 
8. Structured dagger based methods have a slightly smaller overhead than threaded methods, don’t need allocation of a separate stack, and are typically perceived as clearer to understand by Charm++ programmers.
9. Futures need to be created from a threaded entry method.
10. Syntax
```C++
myMsg* m = (myMsg*)CkWaitFuture(f); // conversion from void*
```
 
Messages
==========
1. Messages passed to Charm belong to Charm – Deleted or reused by Charm after sending
2. Message delivered by Charm belongs to user – Must be reused or deleted; If you don’t delete or reuse, memory leaks happen
3. Priority

```C++
    MyVarsizeMsg *msg = new (10,7, 8*sizeof(int))  MyVarsizeMsg(<constructor args>);
    *(int*)CkPriorityPtr(msg) = prio; //set priority OR
    void CkSetQueueing(MsgType message, int queueingtype)
    int * prioMsg = CkPriorityPtr(msg); //get priority
    queueingType: CK_QUEUEING_XIFO, CK_QUEUEING_IXIFO, CK_QUEUEING_BXIFO where X = F or L 
    
    OR
    CkEntryOptions opts1, opts2;
    opts1.setQueueing(CK_QUEUEING_FIFO);
    opts1.setPriority(prio_t integerPrio);
    opts2.setQueueing(CK_QUEUEING_LIFO);
    opts2.setPriority(int prioBits,const prio_t *prioPtr);
    chare.entry_name(arg1, arg2, **opts1**);
    chare.entry_name(arg1, arg2, **opts2**);  
```
4. When a message is sent, i.e. passed to an asynchronous entry method invocation, the programmer relinquishes control of it; the space allocated for the message is freed by the runtime system.
5. Once a parameter-marshalled entry method finishes, the runtime system discards the memory buffer underlying the marshalled parameters. Therefore, in order to access received parameters in a different entry method invocation, they must be saved via copying. In contrast, the runtime system does not discard the memory buffer passed into an entry method that is invoked with a message. It is up to the programmer to manage the memory associated with the input message at the receiving end. This fact can be used to improve performance in parallel programs.
6. It is worth noting that one of the common sources of overheads in programs (especially, fine-grained computations) is the overhead of memeory allocation. This overhead is reduced here because we reuse the message.
7. 3 scenarios whree message is better
 * Recall that marshalled parameters are available to your code only in the scope of the entry method in which they are received. Therefore, if a chare needs to access marshalled parameters after the entry method has finished, they must be copied into heap-allocatedbuffers. This often happens in programs in which a chare receiving data may not be ready to process the data immediately upon receipt. In this case, data received via a message can be saved for later use by simply recording the pointer to the input message received by theentry method. This is often more efficient than copying received marshalled parameters ontothe heap, especially if the entry method receives arrays and large, composite data structures.Of course, you must remember to delete received messages yourself:
 * You should also use a message instead of marshalled parameters if your entry method exhibits the following pattern: it receives data of a certain size, possibly altering it in the body of the entry method, and sends it to another chare, such that the size of sent data is the same as that of the received data. In such a case, the
contents of an incoming message could be altered, and the message reused in the invocation of an entry method on some other chare.
  * Can be custom packed

Tags
====
1. nokeep: User code should not free messages; 
    * Common usage: avoiding a copy for each chare on a PE during a broadcast
    * Also note: you cannot modify contents of nokeep messages
2. reductiontarget: Target of reductions, despite not taking CkReductionMsg as an argument. 
3. local: 
    * These entry methods are equivalent to normal function calls.
    * The entry method is always executed immediately. This feature is available only for Group objects and Chare Array objects. The user has to guarantee that the recipient chare element reside on the same PE. Otherwise, the application will abort on a failure. If the local entry method uses parameter marshalling, instead of marshalling input parameters into a message, it will pass them direcly to the callee. This implies that the callee can modify the caller data if method parameters are passed by pointer or reference. Furthermore, input parameters do not require to be PUPable. Considering that these entry methods always execute immediately, they are allowed to have a non-void return value. Nevertheless, the return type of the method must be a
pointer. 
4. inline: entry methods will be immediately invoked if the message recipient happens to be on the same PE. These entry methods need to be re-entrant as they could be called multiple times recursively. If the recipient resides on a non-local PE, a regular message is sent, and inline has no effect.
5. exclusive: entry methods should only exist on NodeGroup objects. One such entry method will not execute while some other exclusive entry methods belonging to the same NodeGroup object are executing on the same node. In other words, if one exclusive method of a NodeGroup object is executing on node N, and another one is scheduled to run on the same node, the second exclusive method will wait to execute until the first one finishes.



Group and NodeGroup
========================
1. Note that there can be several instances of each group type. In such a case, each instance has a unique group identifier, and its own set of branches.
2. This call returns a regular C++ pointer to the actual object (not a proxy) referred to by the proxy groupProxy. Once a proxy to the local branch of a group is obtained, that branch can be accessed as a regular C++ object. Its public methods can return values, and its public data is readily accessible.

```C++
    GroupType *g=groupProxy.ckLocalBranch();
    //Or
    groupProkxy[CkMyPe()].F();
```
3. If the mainchare wants to broadcast an entry method on a chare array and after they finishes they must all return back to a specific fun F.

```C++
    ckCallback cb = ckCallback(ckReductionTarget(Main, comaeBackAfterRegistration), mainProxy);
    workerarray.registerToNodeGroup(cb);
    
    Worker::registerToNodeGroup(ckCallback &cb) {
        groupProxy.ckLocalBranch()->registerMe();
        contribute(cb);
    }
    
    myGroup::registerMe {
        countOfCharesOnThisPE++;
    }

    myGroup::submitToMain() {
        if(membersContactedMe == countOfCharesOnThisPE) {
            contribute(..To Main .. );
        }
    }
```
4. When an entry method is invoked on a particular branch of a nodegroup, it may be executed by any PE in that logical node. Thus two invocations of a single entry method on a particular branch of a NodeGroup may be executed concurrently by two different PEs in the logical node.
5. If a method M of a nodegroup NG is marked exclusive, it means that while an instance of M is being executed by a PE within a logical node, no other PE within that logical node will execute any other exclusive methods. However, PEs in the logical node may still execute non-exclusive entry method invocations.
6. 

Migration
===========
```C++
    void pup(PUP::er &p){
      p|x;
      p|y;
      p|size;
      int isNull = (!arr);
      p|isNull;
      if(p.isUnpacking()) {
        if(isNull) {
          arr = NULL;
        } else {
          arr = new int[size]; // NOT int arr ... 
        }
      }
      p(arr, size); //Must be outside packing or unpacking
    }
    
    void pup(PUP::er &p) {
        CBase MyChare::pup(p);
        p | heapArraySize;
        if (p.isUnpacking()) {
            heapArray = new float[heapArraySize];
        }
        p(heapArray, heapArraySize);
        bool isNull = !pointer;
        p | isNull;
        if (!isNull) {
            if (p.isUnpacking()) {
                pointer = new   MyClass();
            }
            p | *pointer;
        }

```
Load Balance
============
1. In **centralized approaches**, the entire machine’s load and communication structure are accumulated to a single point, typically processor 0, followed by a decision making process to determine the new distribution of Charm++ objects. Centralized load balancing requires synchronization which may incur an overhead and delay. However, due to the fact that the decision process has a high degree of the knowledge about the entire platform, it tends to be more
 * Object load data are sent to processor 0
 * Integrate to a complete object graph
 * Migration decision is broadcasted from processor 0
 * Global barrier
 
2. In **distributed approaches**, load data is only exchanged among neighboring processors. There is no global synchronization. However, they will not, in general, provide an immediate restoration for load balance - the process is iterated until the load balance can be achieved.
 * Load balancing among neighboring processors
 * Build partial object graph
 * Migration decision is sent to its neighbors
 * No global barrier
 * Adv: Number of processors is large and/or load variation is rapid

3. Limitations of centralized strategies:
 * Centralized load balancing strategies dont scale on extremely large machines
 * Central node: memory/communication bottleneck
 * Decision-making algorithms tend to be very slow

4. Limitations of distributed strategies:
 * Difficult to achieve well-informed load balancing decisions

3. In greaady the quality of LB is better than Refne but the desition making time time is more
4. Greedy 
* Sort objects by decreasing load; Maintain Minheap of processirs (by assigned load)
* It is worth noting that a simple greedy strategy is adequate for a problem if balancing computation were the sole   criterion. **In a standard greedy strategy, all the migratable objects are sorted in order of decreasing load. The   processors are organied in a heap (i.e. a prioritized queue), so that the least loaded processor is at the top of   the heap. Then, in each pass, the heaviest unassigned migratable object is assigned to the least loaded processor   and the heap is reordered to account for the affected processor's load.** However, such a greedy strategy totally   ignores communication costs.  In general, since more than one patch resides on each processor, message-combining    and multicast mechanisms can further reduce the number of messages per patch if locality is considered. Since the   communication costs (including not just the cost of sending and receiving messages, but also the cost of managing   various data structures related to proxies) constitute a significant fraction of the overall execution time in      each timestep, it is essential that the load balancing algorithm also considers these costs.
* Disadvantage: We can ass8ign a lot of tiny objects to a single processor and if the objects are not commuticating   between them (which might be the case as we are not taking commn into acount), then the communication load on the   processir will increase. To combat that we may use GreedyComm, once we assign a object to a processor, together  
  with the load of the object we also add the communication cost of the object to the assigned proc.
* When communication cost is significant: Still use greedy strategy, but:
  ** At each assignment step, choose between assigning O to least loaded processor and the processor that already has objects that communicate most with O.
  ** Based on the degree of difference in the two metrics
  ** Two-stage assignments:
        *** In early stages, consider communication costs as long as the processors are in the same (broad) load class,
        ***In later stages, decide based on load
* After all the migratable objects are tentatively assigned, the algorithm uses a refinement procedure to reduce the remaining load imbalance. During this step, all the overloaded processors (whose computed load exceeds the average by a certain amount) are arranged in a heap, as are all the under-loaded processors. The algorithm repeatedly picks a migratable object from the highest loaded processor, and assigns it to a suitable under-loaded processor.

Good To know
==================
1. Since objects are scheduled based on availability of messages, no single object can hold the processor while it waits for some remote data. Instead, objects that have asynchronous method invocations (messages) waiting for them in the scheduler’s queue are allowed to execute. This leads to a natural overlap of communication and computation, without extra work from the programmer.
E.g., a chare may send a message to a remote chare, and wait for another message from it before
continuing. The ensuing communication time, which would otherwise be an idle period, is naturally and automatically filled in (i.e., overlapped) by the scheduler with useful computaion, i.e., processing of another message from the scheduler’s queue for another chare.


Array Sections / Multicast
SMP Mode/CkLoop  
