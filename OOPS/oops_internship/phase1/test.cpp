/**
 * File: test.cpp
 * Demonstrates: Inheritance Basics (IS-A Relationship)
 * Description: Demonstrates deriving classes to inherit properties and methods from a base class.
 * Interview Takeaway: Inheritance establishes a class hierarchy, enabling code reuse and polymorphical pointers.
 */

#include <iostream>
using namespace std;

class Student { // Class definition
public: // Accessible from anywhere in the program
    int age;
    int roll;

    void print() {
        cout << age << " " << roll << endl;
    }
};

int main() { // Entry point of the program
    Student s1;

    s1.age = 18;
    s1.roll = 101;

    s1.print();
}