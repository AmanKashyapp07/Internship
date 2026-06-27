/**
 * File: 12.cpp
 * Demonstrates: Runtime Polymorphism (Virtual Functions)
 * Description: Demonstrates dynamic dispatch at runtime using virtual functions and pointers/references.
 * Interview Takeaway: Runtime polymorphism is resolved at runtime using vtable and vptr. Base class must have a virtual destructor.
 */

#include <iostream>
using namespace std;

class Animal { // Class definition
public: // Accessible from anywhere in the program

    virtual void sound() { // Virtual function for runtime override dispatch
        cout << "Animal sound" << endl;
    }
};

class Dog : public Animal { // Class definition
public: // Accessible from anywhere in the program

    void sound() override { // Indicates this function overrides a base virtual function
        cout << "Dog barking" << endl;
    }
};

int main() { // Entry point of the program

    Animal* ptr;

    Dog d;

    ptr = &d;

    ptr->sound();
}

// here, dog sound is printing instead of animal sound because of function overriding. Function overriding occurs when a derived class provides a specific implementation of a function that is already defined in its base class. In this case, the 'sound' function in the Dog class overrides the 'sound' function in the Animal class. When we call ptr->sound(), it calls the overridden version in the Dog class, resulting in "Dog barking" being printed instead of "Animal sound". To achieve this behavior, we need to declare the base class function as virtual, which allows for late binding and ensures that the correct function is called based on the actual object type at runtime.
//virtual ensures that the correct function is called for an object, regardless of the type of reference (or pointer) used for the function call. It enables dynamic dispatch, allowing the program to determine at runtime which function to invoke based on the actual type of the object being pointed to, rather than the type of the pointer or reference. This is essential for achieving polymorphism in C++.