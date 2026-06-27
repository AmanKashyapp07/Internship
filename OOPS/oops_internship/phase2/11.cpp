/**
 * File: 11.cpp
 * Demonstrates: Runtime Polymorphism (Virtual Functions)
 * Description: Demonstrates dynamic dispatch at runtime using virtual functions and pointers/references.
 * Interview Takeaway: Runtime polymorphism is resolved at runtime using vtable and vptr. Base class must have a virtual destructor.
 */

#include <iostream>

class Base { // Class definition
public: // Accessible from anywhere in the program
    Base() {}
    virtual ~Base() { std::cout << "Base Destructor called\n"; } // Virtual destructor, ensures proper cleanup of derived class resources when deleting through a base pointer

};

class Derived : public Base { // Class definition
private: // Inaccessible from outside the class
    int* largeArray;
public: // Accessible from anywhere in the program
    Derived() { largeArray = new int[1000]; } // Allocates memory
    ~Derived() { 
        std::cout << "Derived Destructor called\n"; 
        delete[] largeArray; // Clean up memory
    }
};

int main() { // Entry point of the program
    Base* ptr = new Derived();
    delete ptr; // This will call the Derived destructor first, then the Base destructor, ensuring proper cleanup of resources. If the Base destructor were not virtual, only the Base destructor would be called, leading to a memory leak from the Derived class.
    return 0;
}