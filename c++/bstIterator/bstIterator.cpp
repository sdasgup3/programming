#include<iostream>
#include<vector>
#include<stack>

using namespace std;


template <class T>
class bstnode {
  public:
    T data;
    bstnode<T> *left, *right;
  public:
    bstnode(T d, bstnode<T>* l, bstnode<T>* r): data(d) {
      left  =  l;
      right = r;
    }
    T   &getdata() {return data;}
    bstnode<T>*  &getleft() {return left;}
    bstnode<T>*  &getright() {return right;}
};

bstnode<int> *null_node = NULL;
int mydefault = 0;

template <class T> 
class bstiterator;

template <class T> 
class bst {
  private:
    bstnode<T> *root;
  public:
    bst(): root(NULL) {}
    friend class bstiterator<T>; // does not matter if the friend decl is in private or public
                                 // This is for bstiterator class to use the private members of bst 
    typedef bstiterator<T> iterator;
    iterator begin() {
      return bstiterator<T> (root);
    }
    iterator end() {
      return bstiterator<T> (null_node);
    }

    void addrecursive(bstnode<T> **R, T val) {
      if(*R == NULL) {
        *R = new bstnode<T>(val, NULL, NULL);
        return;
      }
      if(val <= (*R)->getdata()) {
        addrecursive(&((*R)->getleft()), val);
      } else {
        addrecursive(&((*R)->getright()), val);
      }
    }
    void insert(T val) {
      addrecursive(&root, val);
    }
};


template <class T> 
class bstiterator {
  private:
    // If we dont use ref here, then on assigning bst::root = NULL, will not change this value;
    // where the expectation is that this iterator is tied to any changes of the iterator'ee
    bstnode<T>* &root;  
    stack< bstnode<T>* > S;
  public:
    bstiterator(bstnode<T>* &r): root(r) { // Here we need the reference of the root, not its copy
      fill(root);
    }

    void fill(bstnode<T> *root) {
      while(root) {
        S.push(root);
        root = root->left;
      }
    }

    bstiterator<T> operator++() {  //prefix
      bstnode<T> *tmp = S.top();
      S.pop();
      fill(tmp->right);
      if(S.empty()) {
        root  = NULL;
      }
      return *this;
    }

    bstiterator<T> operator++(int notused) {  //postfix;  the return type is NOT ref because we dont want to return ref to a temp
      bstiterator<T> clone(*this);
      bstnode<T> *tmp = S.top();
      S.pop();
      fill(tmp->right);
      if(S.empty()) {
        root  = NULL;
      }
      return clone;
    }

    bool operator==(bstiterator<T> rhs) {
      return (root == rhs.root);
    }

    bool operator!=(bstiterator<T> rhs) {
      return !(*this == rhs);
    }

    const T& operator*() {
      if(!S.empty()) {
        bstnode<T> *node = S.top();
        return node->data;
      } else {
        return mydefault;
      }
    }

};

int main() {
  bst<int> bInt;
  bInt.insert(5);
  bInt.insert(4);
  bInt.insert(6);
  bInt.insert(2);

  bst<int>::iterator i = bInt.begin(); 
  while(i != bInt.end()) {
    std::cout << *i << " " ; 
    i++;
  }
  std::cout << "\n" ; 

  return 0;
}
