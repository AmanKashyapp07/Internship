/**
 * File: test5.cpp
 * Demonstrates: Operator Overloading (Compile-time Polymorphism)
 * Description: Demonstrates overloading C++ operators to perform custom operations on class objects.
 * Interview Takeaway: Operators like +, *, ==, and stream insertion << can be customized for objects.
 */

#include <iostream>
using namespace std;

class Student { // Class definition
public: // Accessible from anywhere in the program
    int age;

    Student() {
        cout << "Constructor called" << endl;
    }

    ~Student() {
        cout << "Destructor called" << endl;
    }
};

int main() { // Entry point of the program
    Student s1;

    cout << "Inside main" << endl;
}

// destructor is called when the object goes out of scope, which happens at the end of the main function. When the main function ends, the destructor for s1 is automatically called, and we see the message "Destructor called" printed to the console.
// what exactly is destructor - A destructor is a special member function of a class that is executed when an object of that class goes out of scope or is explicitly deleted. The purpose of a destructor is to perform any necessary cleanup tasks, such as releasing resources (like memory or file handles) that the object may have acquired during its lifetime. In C++, a destructor has the same name as the class but is preceded by a tilde (~) symbol. It does not take any parameters and does not return any value.
// when scope ends - object is destroyed automatically, and the destructor is called to clean up any resources that the object may have acquired during its lifetime. This is an important aspect of C++'s memory management, as it helps prevent memory leaks and ensures that resources are properly released when they are no longer needed.
// heap memory - Heap memory is a region of memory that is used for dynamic memory allocation. In C++, you can allocate memory on the heap using the 'new' operator and deallocate it using the 'delete' operator. When you allocate memory on the heap, it remains allocated until you explicitly deallocate it, which means that you have more control over the lifetime of the memory. However, it also means that you need to be careful to avoid memory leaks by ensuring that all dynamically allocated memory is properly deallocated when it is no longer needed.