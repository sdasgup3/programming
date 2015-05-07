
This article looks in some detail at a Java Virtual Machine (JVM) feature
called Escape Analysis and how the JVM can use it to improve an application’s
performance.  As you’ll see below, understanding what the JVM can do with
escape analysis can help explain some otherwise non-intuitive performance
results.

A Performance Puzzle

Let’s assume you have a utility class that generates values by keeping some internal state of the last value generated and using it to arrive at the next value.  Since the class maintains some internal state, and since this is a utility class that you might use in multi-threaded environments, you decide to synchronize thegetNext() method.   You might code it as:

public class Generator {

  private int lastgen;  // saved state

  Generator(int value) {
    // initialize lastgen using supplied value
  }
  public synchronized int getNext() {
    int nextgen = somefunction(lastgen);
    lastgen = nextgen;
    return nextgen;
  }
}

Assume you use this Generator class in a hot method of your application as follows:

  public class HotClass1 {
    final private static int ASIZ = 10000;
    int a[] = new int[ASIZ];
    public int hotMethod() {
      Generator mygen = new Generator(System.nanoTime());

      // fill in array with generated values
      for (i=0; i<ASIZ; i++) {
        a[i] = mygen.getNext();
      }
      // use array of values to compute something
      // and return it
      for (i=0; i<ASIZ-1; i++) {
        computedValue += someFunc(a[i], a[i+1])
      }
      return computedValue;
    }
  }

As part of a code review, you decide that you don’t really need to create a new Generator object every time you call hotMethod().  Instead, you can just have a single Generator as part of the HotClass.  This should be more efficient because you don’t need to allocate and initialize a new mygen object on every call.   So you move the declaration of mygen out to class level:

  public class HotClass2 {
    private Generator mygen = new Generator(System.nanoTime());

    public int hotMethod() {
      // rest of hotMethod is unchanged
    }
  }

To your surprise, you find that your application now runs more slowly.  What’s going on?

Looking at Profiles

You decide to use a profiler (for example, AMD’s CodeAnalyst), which reveals that the amount of time you spend in hotMethod() has now gone up.  Drilling down to a more detailed level, you see that the time spent computing computedValue has not changed; the increased wait time is all in the beginning part ofhotMethod() where the array is filled.  You also notice that no timer samples show up in theGenerator.getNext() method, which is surprising because that’s the only method called in that loop.

When a hot method calls a target method and no timer samples are seen in the target method, the usual explanation is that just-in-time (JIT) compiler has inlined the target method.  This optimization expands the target method in the calling method (as if it were written inline) and is particularly useful in hot methods.  Inlining eliminates the overhead of a call and return instruction, and often eliminates some register saving and shuffling.  In addition, all the normal optimizations like constant folding that can be done within a method can now be applied across the inlined method boundary just by the fact that the target is inlined.

One downside of inlining for profiling purposes is that the JVM often hides from the profiling tool the information about the inlined method.  The calling method gets more samples, but those samples all get attributed to the source line where the target method is invoked, rather than to the individual lines within the target method.  AMD is working with JVM vendors to help make this inlining information available to profiling tools.  Note that some JVMs have ways to disable inlining but, in that case, you’re not profiling the actual code that will be generated.

Let’s use AMD CodeAnalyst to look at the generated code.     The source line

a[i] = mygen.getNext();

seems to have generated some extra code in the HotClass2 version.  In particular, a lot of timer samples show up right after an instruction that looks something like

lock cmpxchg   [esi+4], ecx

and this instruction doesn’t exist in the HotClass1 version.

Sidebar: the cmpxchg instruction and Java synchronization

The cmpxchg instruction is the x86 instruction for Compare and Set (CAS), a common instruction on many processor architectures for conditionally setting a memory location to a new value if its current value matches some expected value.  For details on the x86 flavor, see the description of cmpxchg in the AMD64 Architecture Programmer’s Manual Volume 3.  Cmpxchg is an ideal instruction for acquiring a “lock” on a Java object, which is necessary for synchronized methods and blocks.  In a single instruction, you are able to check whether the object is unlocked and, if so, set its lock word to your threadId, thus preventing other threads from acquiring the lock, assuming they also use cmpxchg.

The lock prefix guarantees that no other processor core will get access to that same memory location until the read and write are both complete.  Note that the lock prefix is only needed on a multi-core system.  If you run this experiment on a single-core system, you will see that the JVM generates a cmpxchg instruction without the lock prefix.

Synchronization Locks in our Performance Puzzle

So it looks like the HotClass2.getNext() method is spending time acquiring locks that HotClass1 did not acquire.  Note: if your JVM has a monitoring tool that lets you record how many locks are acquired, you could confirm this lock acquire count difference by using such a tool.

Let’s try to understand why the JVM needs to generate synchronization locks for HotClass2, but not for HotClass1.  After all, the getNext() method itself didn’t change.  It is synchronized in both cases.

First, remember that the getNext() method was inlined into hotMethod() in both cases.   Once it is inlined, the JIT compiler is free to use optimizations specific to this invocation.    In particular, in HotClass1, the JIT compiler can see that mygen’s scope is limited to hotMethod() and a reference to mygen does not “escape”, possibly to be accessed by some other thread. Thus, no other thread can possibly use this object.  If no other thread can possibly use this object, the semantics of the getNext() method’s synchronized keyword are guaranteed without the need to acquire locks.  Note that this optimization would not be legal in a generic non-inlined getNext().

In HotClass2, on the other hand, mygen exists at class scope, which makes it accessible by any thread that accesses that same HotClass2 object.  Declaring mygen as private makes no difference, since mygen is still accessible to any method in the class, like getNext().

It’s easy for the JVM to determine that the scope of an object is local, but not always so easy to determine whether the object “escapes.”  The JVM must detect whether a reference to the object is copied to a class field, or if the reference is passed to some other method and that other method allows the reference to escape.  This phase of analysis is called Escape Analysis, and JVMs are constantly trying to improve their Escape Analysis to detect more non-escaping cases.   In fact, if you run this experiment on different JVMs, you may not see the performance discrepancy on a certain JVM because either it did not do escape analysis or its analysis did not detect that mygen does not escape in this case.  Note that, by the language semantics, if the JVM cannot prove that a reference to an object does not escape, it must be pessimistic and assume that it does escape, thus limiting optimizations like the one we saw in HotClass1.

In this particular test application, even in the HotClass2 case you were only accessing HotClass2 from one thread (in fact, the whole application only had one thread).  Shouldn’t the JVM have been able to detect that and eliminate the unneeded lock acquisitions?  Ideally, yes; however, it turns out it is much harder for the JVM to prove that a particular HotClass2 object and its associated mygen object is not accessed by some other thread.  And, even when it can detect this, it would have to be able to handle the case where a new thread is created later in time and that new thread tries to access the object.   The JVM would have to recompile hotMethod() with the locking code back in.

Escape Analysis and Heap Allocations

Eliminating unneeded synchronization locking is a clear benefit of escape analysis, but escape analysis can lead to other optimizations.  For example, if an object is at local scope and does not escape, it does not have to be allocated on the heap at all; it can simply be allocated on the thread’s local stack.  Everything allocated on the local stack is effectively deallocated when the stack is popped at method exit.  If the object were allocated on the heap, it would have to be collected later by the garbage collector.   In thehotMethod() example above, assuming your JVM detects that mygen does not escape, you should not see any heap usage changes during the execution of  hotMethod().

Summary

We’ve shown how a JVM can use escape analysis to enable some optimizations like eliminating unnecessary synchronization and allocating objects on the stack rather than on the heap.  In your own Java code, this is something to be aware of when you are deciding whether to declare a new object at method scope or at class scope.   If the object doesn’t have state that must be preserved outside the method and if the object’s constructor is not too large, you should create the object at method scope.  This would allow the JVM to detect that the object does not escape and enable optimizations like those mentioned in this article.




#include<iostream>
using namespace std;



class A {
  public:
    int x;
    A(int z): x(z){}
};

class B {
  public:
    A* a;
    void doSomething( A* a ) 
    {
      this->a = a;
    }
};

B f()
{
  A *aObj = new A(10);
  B bObj = B();
  bObj.doSomething(aObj);
  return bObj;
}

int main ()
{
  B bObj = f();
  std::cout<< bObj.a->x << endl;
  return 0;
}

