## Some concepts

##Const Correctness

###Const References
- when we declare a const reference, we're only making the data referred to 
const. References, by their very nature, cannot change what they refer to.
- An lvalue represents an object that occupies some identifiable location in 
memory (i.e. has an address).
- A rvalues are defined by exclusion, by saying that every expression is either 
an lvalue or an rvalue. Therefore, from the above definition of lvalue, an 
rvalue is an expression that does not represent an object occupying some 
identifiable location in memory.
- Consider the code

  ```
  int &r = 10;    //Error: invalid initialization of non-const reference of type 
                  //‘int&’ from an rvalue of type ‘int’
  const &r = 10;  //OK
  ```

  - Non-const lvalue references cannot be assigned rvalues, since that would 
  require an invalid rvalue-to-lvalue conversion. Here instead of lvalue it gets 
  a xvalue (“eXpiring” value refers to an object, usually near the end of its 
      lifetime) i.e int(10).
  - Constant lvalue references can be assigned rvalues/xvalues. Since they're 
  constant, the value can't be modified through the reference and hence there's 
  no problem of modifying an rvalue.

###Const Pointers
  ```
  const int *p_int;
  p_int = &x;
  *p_int = 10;  //Error

  int * const p_int = &x;
  p_int = &y;   //Error
  *p_int = 10;  //OK
  ```

###Const Member functions
  - Does not makes any sense for non member function
  ```
  class A {
    int y;
    public:
      static int x;
      void f() const {
        x = 10;
        y = 90; //ERROR
      }
  };
  int A::x = 100;

  int main() {
    const  A a = A();
    a.f();
  }
  ```
  - If a program calls for the default initialization of an object of a 
  const-qualified type T, T shall be a class type with a user-provided default 
  constructor.
  
  ```
  class POD {
    int i;
  };

  POD p1; //uninitialized - but don't worry we can assign some value later on!
  p1.i = 10; //assign some value later on!

  POD p2 = POD(); //initialized

  const POD p3 = POD(); //initialized 

  const POD p4; //uninitialized  - error - as we cannot change it later on!
  ```
  - Every method of an object receives an implicit this pointer to the object; 
  const methods effectively receive a ```const T* const this``` pointer. The secondconst qualifieris due to the property of this.   

  - Const functions can always be called
  - Non-const functions can only be called by non-const objects
  - When const functions return references or pointers to members of the class, they must also be const.
  - Pointing at y with a const int* does not make y const, it just means that you can't change y using that pointer.
  ```
  int y;
  const int* pConstY = &y;  // legal - but can't use pConstY to modify y
  int* pMutableY = &y;      // legal - can use pMutableY to modify y
  *pMutableY = 42;
  ```
  ```
  class T {
      int y;
      int *p;
    public:
      T() {y = 20; p =&y;}
      void inspect() const {
        cout << y << " " << *p << " " << p << "\n";
      }
      void mutate() {
        *p = 10;
      }
  };


  int main() {
    const  T a;
    T b = a;

    b.mutate();
  }
  ```

###casting away const-ness
  -   Bad practice
  ```
  const int x = 4;           // x is const, it can't be modified
  const int* pX = &x;        // you can't modify x through the pX pointer

  cout << x << endl;         // prints "4"

  int* pX2 = (int *)pX;      // explicitly cast pX as an int*
  *pX2 = 3;                  // result is undefined

  cout << x << endl;        // who knows what it prints? undefined. The compiler 
                            //may still print 4 as x is const. 
  ```
  - use ```int* pX2 = const_cast<int *>pX;```  instead.

##Constructor 
  -  
  ```
  T a();
  a.mem\_function();  //Error: a is connsodered a function declaration with 
                      //return type T 
  ```

###References
  - [Understanding lvalues and rvalues in C and 
  C++](http://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c)

