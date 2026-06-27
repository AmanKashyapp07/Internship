/**
 * File: 7.cpp
 * Demonstrates: Inheritance Basics (IS-A Relationship)
 * Description: Demonstrates deriving classes to inherit properties and methods from a base class.
 * Interview Takeaway: Inheritance establishes a class hierarchy, enabling code reuse and polymorphical pointers.
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