/**
 * File: 18.cpp
 * Demonstrates: Runtime Polymorphism (Virtual Functions)
 * Description: Demonstrates dynamic dispatch at runtime using virtual functions and pointers/references.
 * Interview Takeaway: Runtime polymorphism is resolved at runtime using vtable and vptr. Base class must have a virtual destructor.
 */

#include <iostream>
using namespace std;

class Base { // Class definition
public: // Accessible from anywhere in the program
    int x = 10;
    // Made 'show' virtual so it can support polymorphism if needed later, in simple terms, virtual means that the function can be overridden in derived classes and allows for dynamic dispatch, which means that the correct function will be called based on the actual type of the object at runtime, rather than the type of the pointer or reference used to call the function.
    virtual void show() { // Virtual function for runtime override dispatch
        cout << "Base x = " << x << "\n";
    }
};

class Derived : public Base { // Class definition
public: // Accessible from anywhere in the program
    int y = 20;
    void show() override { // Indicates this function overrides a base virtual function
        cout << "Derived x = " << x << ", y = " << y << "\n";
    }
};

int main() { // Entry point of the program
    Derived d;
    
    // SOLUTION: Use a reference instead of a value copy
   
    // if Base b = d; then we would have object slicing, and b.show() would call Base's show() instead of Derived's show(), resulting in only "Base x = 10" being printed. By using a reference (Base&), we avoid slicing and allow polymorphism to work correctly, so b.show() will call Derived's show() and print "Derived x = 10, y = 20".
    Base b1 = d; // This will cause object slicing, and b1 will only have access to the Base part of the Derived object, losing the Derived-specific data (y) and behavior (Derived's show() implementation).
    b1.show(); // Outputs: Base x = 10 (Slicing occurred, Derived part is lost)

    Base *b3 = &d; // Using a pointer to avoid slicing
    b3->show(); // Outputs: Derived x = 10, y = 20 (No slicing occurred!), even if we didn't make show() virtual, because we are calling it through a pointer to the base class, it will call the derived class's show() function due to dynamic dispatch.

    Base& b = d; 
    b.show(); // Outputs: Derived x = 10, y = 20 (No slicing occurred!)

    // Base *b3 = &d and Base &b =d are same in terms of avoiding slicing and allowing polymorphism to work correctly, but they are different in terms of syntax and usage. Base *b3 = &d; creates a pointer to the base class and assigns it the address of the derived class object, while Base& b = d; creates a reference to the base class and binds it to the derived class object. Both allow us to call the show() function and get the correct output without slicing, but they do so using different C++ features (pointers vs references).
    return 0;
}