## Some concepts

###Const Correctness
- Consider the code
  ```
  int &r = 10;   //Error: invalid initialization of non-const reference of type ‘int&’ from an rvalue of type ‘int’
  const &r = 10; //OK
  ```
  - references are also called lvalue referenc; it needs a lvalue (a value with an address)  
