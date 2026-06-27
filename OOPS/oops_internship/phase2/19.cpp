/**
 * File: 19.cpp
 * Demonstrates: Inheritance Basics (IS-A Relationship)
 * Description: Demonstrates deriving classes to inherit properties and methods from a base class.
 * Interview Takeaway: Inheritance establishes a class hierarchy, enabling code reuse and polymorphical pointers.
 */

#include <iostream>
using namespace std;

class Animal { // Class definition
public: // Accessible from anywhere in the program
    void eat() {
        cout << "Eating" << endl;
    }
};

class Dog : public Animal { // Class definition
public: // Accessible from anywhere in the program
    void bark() {
        cout << "Barking" << endl;
    }
};

class Puppy : public Dog { // Class definition
public: // Accessible from anywhere in the program
    void cry() {
        cout << "Crying" << endl;
    }
};

int main() { // Entry point of the program

    Puppy p;

    p.eat();
    p.bark();
    p.cry();
}