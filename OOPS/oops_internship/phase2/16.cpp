/**
 * File: 16.cpp
 * Demonstrates: Runtime Polymorphism (Virtual Functions)
 * Description: Demonstrates dynamic dispatch at runtime using virtual functions and pointers/references.
 * Interview Takeaway: Runtime polymorphism is resolved at runtime using vtable and vptr. Base class must have a virtual destructor.
 */

#include <iostream>
using namespace std;

class Grandparent { // Class definition
public: // Accessible from anywhere in the program
    Grandparent(int x) {
        cout << "Grandparent constructor called with value: " << x << endl;
    }
    ~Grandparent() {
        cout << "Grandparent Destructor called\n";
    }
};

// Notice the 'virtual' keyword
class ParentA : virtual public Grandparent { // Class definition
public: // Accessible from anywhere in the program
    ParentA(int x) : Grandparent(x) {
        cout << "ParentA constructor called with value: " << x << endl;
    }
};

class ParentB : virtual public Grandparent { // Class definition
public: // Accessible from anywhere in the program
    ParentB(int x) : Grandparent(x) {
        cout << "ParentB constructor called with value: " << x << endl;
    }
};
class ParentC : virtual public Grandparent { // Class definition
public:    ParentC(int x) : Grandparent(x) {
        cout << "ParentC constructor called with value: " << x << endl;
    }
};
class Grandchild : public ParentA, public ParentB, public ParentC { // Class definition
public: // Accessible from anywhere in the program
    // In VIRTUAL inheritance, this is not only allowed, it is REQUIRED!
    Grandchild(int x) : Grandparent(2*x), ParentA(3*x), ParentB(4*x), ParentC(5*x) {
        cout << "Grandchild constructor called with value: " << x << endl;
    }
};

int main() { // Entry point of the program
    Grandchild gc(5);
    return 0;
}