#include<iostream>
using namespace std;

class base {
  public:
    virtual void func() {cout << "Base\n";};
};

class derived : public base{
  public:
    void func() {cout << "derived\n";};
    void other() {cout << "derived::other\n";};
};

int main () {
  base *b = new derived();
  b->func();
  //b->other();//compile error
  return 0;
}
