/**
 * File: 5.cpp
 * Demonstrates: Inheritance Basics (IS-A Relationship)
 * Description: Demonstrates deriving classes to inherit properties and methods from a base class.
 * Interview Takeaway: Inheritance establishes a class hierarchy, enabling code reuse and polymorphical pointers.
 */

#include <iostream>
using namespace std;

class Animal { // Class definition
public: // Accessible from anywhere in the program
    ~Animal() {
        cout << "Animal destructor" << endl;
    }
};

class Dog : public Animal { // Class definition
public: // Accessible from anywhere in the program
    ~Dog() {
        cout << "Dog destructor" << endl;
    }
};

int main() { // Entry point of the program
    Dog d;
}