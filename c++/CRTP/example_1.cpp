//http://eli.thegreenplace.net/2011/05/17/the-curiously-recurring-template-pattern-in-c

#include<iostream>
using namespace std;

template <typename T>
class Base {
  public:
    void interface() {
      cout << "In Base\n";
      static_cast<T*>(this)->implementation();
    }
};

class Derived1:public Base<Derived1> {
  protected:
    int x;
  public:
  Derived1():x(10) {}
   void implementation() {
    cout << "In Derived 1\n";
    cerr << "Derived 1 implementation" << x << "\n";
  }

};

class Derived2:public Base<Derived2> {
  protected:
    int x;
  public:
  Derived2():x(20) {}
   void implementation() {
    cout << "In Derived 2\n";
    cerr << "Derived 2 implementation " << x << "\n";
  }

};

template <>
class Base<void> : public Base< Base <void> > {
  protected:
    int x;
  public:
  Base():x(30) {}
   void implementation() {
    cout << "In Base void  \n";
    cerr << "Base implementation" << x << "\n";
  }

};

template <typename T>
void perform(Base<T> *X) {
  X->interface();
}

int main() {
  Derived1 D1;
  Derived2 D2;
  Base<void> B;

  perform(&B);
  perform(&D1);
  perform(&D2);

  // Wrongs
  Base<Derived1> D3;
  Base<Derived2> D4;
  perform(&D3);
  perform(&D4);

  Base<Derived1> b;
  static_cast<Derived1*> (&b)->implementation();
  
  return 0 ;
}
