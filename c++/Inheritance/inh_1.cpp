#include<iostream>

class X {
  public:
    virtual void n() {} ;

};

class Y: public X {
  public:
    void n() {
      std::cout << "In Y\n";
    }

};

class Z : public X{
  public:
    void n() {
      std::cout << "In Z\n";
    }
};

class A {
  public:
    X* f;
    A(X *xa) {
      this->f = xa;
    }
};

class B : public A 
{
  public:
    B(X* xa): A(xa) { }
    void m () {
      X* xb = this->f;
      xb->n();
    }
};

class C : public A 
{
  public:
    C(X* xc):A(xc) { }
    void m () {
      X* xc = this->f;
      xc->n();
    }
};

int main ()
{
  Y *y = new Y();
  Z *z = new Z();

  B* b = new B(y);
  C* c = new C(z);

  b->m();
  c->m();

  return 0;

}
