/**
 * File: test10.cpp
 * Demonstrates: implicit 'this' Pointer & Method Chaining
 * Description: Demonstrates using the 'this' pointer to access calling objects and enabling method chaining.
 * Interview Takeaway: The 'this' pointer is an implicit parameter to all non-static member functions.
 */

#include <iostream>
using namespace std;

class Student { // Class definition
public: // Accessible from anywhere in the program
    int age;

    Student(int age) {
        this->age = age;
    }

    Student(const Student& other) {
        this->age = other.age;
    } // const because of the reference, we don't want to modify the other object, and we pass it by reference to avoid unnecessary copying of the object. The copy constructor takes a reference to another Student object and initializes the age data member of the new object with the age of the existing object. This allows us to create a new Student object as a copy of an existing Student object.
};

int main() { // Entry point of the program
    Student s1(18);

    Student s2(s1);

    cout << s2.age;
}

// copy constructor is a special constructor in C++ that is used to create a new object as a copy of an existing object. It is called when we initialize a new object with an existing object of the same class. In the code above, we have defined a copy constructor for the Student class that takes a reference to another Student object and initializes the age data member of the new object with the age of the existing object. When we create s2 as a copy of s1 using the statement 'Student s2(s1);', the copy constructor is called, and s2's age is initialized to 18, which is the age of s1. Therefore, when we print s2.age, it outputs 18.
//if we change s1 age to 20 after creating s2, s2's age will still be 18 because the copy constructor creates a new object with its own copy of the data members. The copy constructor performs a shallow copy of the data members, which means that it copies the values of the data members from the existing object to the new object. Since age is an integer, it is copied by value, and any changes to s1's age after creating s2 will not affect s2's age. Therefore, if we change s1.age to 20 after creating s2, s2.age will still be 18.
// even if we don't define a copy constructor, the compiler will automatically generate a default copy constructor for us, which performs a shallow copy of the object's data members. However, if we have any dynamically allocated memory or resources in our class, we should define our own copy constructor to ensure that a deep copy is performed and that the resources are properly managed. In this case, since we only have a simple integer data member, the default copy constructor would work fine, but we have defined our own copy constructor for demonstration purposes.
// shallow copy vs deep copy - A shallow copy of an object copies the values of the object's data members, but if any of those data members are pointers to dynamically allocated memory, the shallow copy will only copy the pointer value, not the actual memory it points to. This can lead to issues such as double deletion if both the original and the copied object try to delete the same memory. A deep copy, on the other hand, creates a new copy of any dynamically allocated memory and ensures that each object has its own separate copy of that memory. In C++, if we have a class with dynamically allocated memory, we should define our own copy constructor to perform a deep copy and avoid potential issues with shallow copying.