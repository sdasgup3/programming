/*
 *Non-virtual base classes are always initialised by the class which is immediately derived from them. That is, if the scenario were as follows:

class Final : public MakeFinal
{
public:
  Final() {}
};

class Derived : public Final
{};
then the ctor of Derived only initialises Final, which is fine (Final has a public ctor). Final's ctor then initialises MakeFinal, which is also possible, since Final is a friend.

However, for virtual base classes, the rule is different. All virtual base
classes are initialised by the ctor of the most-derived object. That is, when
creating an instance of Final, it's Final's ctor which initialises MakeFinal (possible because Final is a friend og MakwFinal).
However, when trying to create an instance of Derived, it must be Derived's
ctor which initialises MakeFinal (most derived object). And that is impossible, due to MakeFinal's
private ctor.
 *
 */


#include<iostream>
using namespace std;
 
class Final;  // The class to be made final
 
class MakeFinal // used to make the Final class final
{
private:
    MakeFinal() { cout << "MakFinal constructor" << endl; }
friend class Final;
};
 
//class Final : virtual MakeFinal
class Final : public MakeFinal
{
public:
    Final() { cout << "Final constructor" << endl; }
};

class Derived : public Final // Compiler error
{
public:
    Derived() { cout << "Derived constructor" << endl; }
};

int main() {

  Derived f;
  return 0;
}
