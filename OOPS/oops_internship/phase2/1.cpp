/**
 * File: 1.cpp
 * Demonstrates: Inheritance Basics (IS-A Relationship)
 * Description: Demonstrates deriving classes to inherit properties and methods from a base class.
 * Interview Takeaway: Inheritance establishes a class hierarchy, enabling code reuse and polymorphical pointers.
 */

#include <iostream>
using namespace std;

class Animal { // Class definition
public: // Accessible from anywhere in the program
    int age;

    void eat() {
        cout << "Eating" << endl;
    }
};

class Dog : public Animal { // Class definition
public: // Accessible from anywhere in the program
    int breed;
};

int main() { // Entry point of the program

    Dog d;

    d.age = 5;
    d.breed = 1;    
    d.eat();
    cout<<d.age<<" "<<d.breed<<endl;

}
