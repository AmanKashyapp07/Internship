/**
 * File: 8.cpp
 * Demonstrates: Runtime Polymorphism (Virtual Functions)
 * Description: Demonstrates dynamic dispatch at runtime using virtual functions and pointers/references.
 * Interview Takeaway: Runtime polymorphism is resolved at runtime using vtable and vptr. Base class must have a virtual destructor.
 */

#include <iostream>
using namespace std;

class Dog { // Class definition
public: // Accessible from anywhere in the program
    void sound() {
        cout << "Dog sound" << endl;
    }
};

class Puppy : public Dog { // Class definition
public: // Accessible from anywhere in the program
    void sound() {
        cout << "Puppy sound" << endl;
    }
};

int main() { // Entry point of the program

    Dog* p = new Puppy;

    p->sound();

    return 0;
}

// here, dog sound is printing instead of puppy sound because of early binding. Early binding means that the function to be called is determined at compile time based on the type of the pointer (Dog* in this case) rather than the actual object it points to (Puppy). To achieve late binding and call the correct function based on the actual object type, we need to declare the base class function as virtual.