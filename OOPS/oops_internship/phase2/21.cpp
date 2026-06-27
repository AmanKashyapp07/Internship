/**
 * File: 21.cpp
 * Demonstrates: Runtime Polymorphism (Virtual Functions)
 * Description: Demonstrates dynamic dispatch at runtime using virtual functions and pointers/references.
 * Interview Takeaway: Runtime polymorphism is resolved at runtime using vtable and vptr. Base class must have a virtual destructor.
 */

#include <iostream>
using namespace std;

class Animal { // Class definition
public: // Accessible from anywhere in the program
    int age=5;
};

class Dog : virtual public Animal { // Class definition

};

class Cat : virtual public Animal { // Class definition

};

class Hybrid : public Dog, public Cat { // Class definition

};

int main() { // Entry point of the program

    Hybrid h;

    cout << "Hybrid age: " << h.age << endl; // This will print 5 without ambiguity because of virtual inheritance, which ensures that there is only one instance of the 'age' member variable shared between Dog and Cat, allowing Hybrid to access it without ambiguity.
}
