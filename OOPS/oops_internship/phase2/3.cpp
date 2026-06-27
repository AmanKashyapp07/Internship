/**
 * File: 3.cpp
 * Demonstrates: Inheritance Basics (IS-A Relationship)
 * Description: Demonstrates deriving classes to inherit properties and methods from a base class.
 * Interview Takeaway: Inheritance establishes a class hierarchy, enabling code reuse and polymorphical pointers.
 */

#include <iostream>
using namespace std;

class Animal { // Class definition
protected: // Accessible only to this class and derived classes
    int age;

public: // Accessible from anywhere in the program
    void setAge(int a) {
        age = a;
    }
};

class Dog : public Animal { // Class definition
public: // Accessible from anywhere in the program
    void a() {
        cout << "Dog age: " << age << endl;
    }
};

class Puppy : public Dog { // Class definition
public: // Accessible from anywhere in the program
    void a() {
        cout << "Puppy age: " << age << endl;
    }
};

int main() { // Entry point of the program
    Puppy p;

    p.setAge(3);

    p.a(); // puppy a method will override the dog a method because of inheritance and it will print "Puppy age: 3" instead of "Dog age: 3"

    return 0;
}