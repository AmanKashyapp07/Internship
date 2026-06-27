/**
 * File: test7.cpp
 * Demonstrates: Inheritance Basics (IS-A Relationship)
 * Description: Demonstrates deriving classes to inherit properties and methods from a base class.
 * Interview Takeaway: Inheritance establishes a class hierarchy, enabling code reuse and polymorphical pointers.
 */

#include <iostream>
using namespace std;

class Student { // Class definition
public: // Accessible from anywhere in the program
    int age;

    Student() {
        cout << "Constructor called" << endl;
        age = 18;
    }
};

int main() { // Entry point of the program
    Student s;

    cout << s.age;
}

// constructor is a special member function of a class that is executed whenever we create new objects of that class. The purpose of a constructor is to initialize the object's data members and perform any setup tasks that are necessary for the object to function properly. In C++, a constructor has the same name as the class and does not have a return type. In the code above, we have defined a constructor for the Student class that initializes the age data member to 18 and prints a message to indicate that the constructor has been called. When we create an object of the Student class in the main function, the constructor is automatically called, and we see the message "Constructor called" printed to the console, followed by the value of age which is 18.