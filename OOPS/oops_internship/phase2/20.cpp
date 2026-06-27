/**
 * File: 20.cpp
 * Demonstrates: Operator Overloading (Compile-time Polymorphism)
 * Description: Demonstrates overloading C++ operators to perform custom operations on class objects.
 * Interview Takeaway: Operators like +, *, ==, and stream insertion << can be customized for objects.
 */

#include <iostream>
using namespace std;

class A { // Class definition
public: // Accessible from anywhere in the program
    void show() {
        cout << "A" << endl;
    }
};

class B { // Class definition
public: // Accessible from anywhere in the program
    void show() {
        cout << "B" << endl;
    }
};

class C : public A, public B { // Class definition

};

int main() { // Entry point of the program

    C obj;

   // obj.show(); this is ambiguous because both A and B have a show function, so the compiler does not know which one to call. To resolve this ambiguity, we need to specify which base class's show function we want to call using the scope resolution operator (::) as follows:
    obj.A::show(); // This will call the show function of class A and print "A"
    obj.B::show(); // This will call the show function of class B and print "B"
}