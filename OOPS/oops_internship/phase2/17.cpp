/**
 * File: 17.cpp
 * Demonstrates: implicit 'this' Pointer & Method Chaining
 * Description: Demonstrates using the 'this' pointer to access calling objects and enabling method chaining.
 * Interview Takeaway: The 'this' pointer is an implicit parameter to all non-static member functions.
 */

#include <iostream>
using namespace std;

class Base { // Class definition
public: // Accessible from anywhere in the program
    int x = 10;

    void show() {
        cout << "Base x = " << x << "\n";
    }
};

class Derived : public Base { // Class definition
public: // Accessible from anywhere in the program
    int y = 20;

    void print() {
        cout << "Derived y = " << y << "\n";
    }
};

int main() { // Entry point of the program

    Derived d;
    Base b = d;
    b.show();
    
}

// here the problem is that when we assign the derived class object 'd' to the base class object 'b', we are slicing off the derived part of the object, which means that only the base class portion of 'd' is copied into 'b'. As a result, when we call b.show(), it will only have access to the base class members and will not be able to access any members of the derived class. This is known as object slicing, and it occurs because we are trying to assign a derived class object to a base class object, which leads to loss of information about the derived class. To avoid this issue, we should use pointers or references to the base class instead of creating a new base class object, which allows us to maintain access to the derived class members through polymorphism.