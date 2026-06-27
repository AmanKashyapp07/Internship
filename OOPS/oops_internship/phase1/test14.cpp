/**
 * File: test14.cpp
 * Demonstrates: implicit 'this' Pointer & Method Chaining
 * Description: Demonstrates using the 'this' pointer to access calling objects and enabling method chaining.
 * Interview Takeaway: The 'this' pointer is an implicit parameter to all non-static member functions.
 */

#include <iostream>
using namespace std;

class Student { // Class definition
public: // Accessible from anywhere in the program
    int* marks;

    Student(int value) {
        marks = new int(value);
    }

    ~Student() {
        delete marks;

        cout << "Memory released" << endl;
    }
};

// destructor is called when the object goes out of scope, which happens at the end of the main function. When the main function ends, the destructor for s1 is automatically called, and we see the message "Memory released" printed to the console. This is because when an object goes out of scope, its destructor is called to clean up any resources that the object may have acquired during its lifetime. In this case, since we allocated memory for marks using new in the constructor, we need to release that memory in the destructor to avoid memory leaks. By calling delete marks in the destructor, we ensure that the memory allocated for marks is properly deallocated when the Student object goes out of scope, if we don't write destructor then the memory allocated for marks will not be released when the Student object goes out of scope, which can lead to memory leaks and inefficient use of memory in our program. Therefore, it is important to define a destructor for any class that allocates dynamic memory to ensure that the memory is properly released when it is no longer needed, automatic destructor is provided by the compiler if we don't define one, but it will not release any dynamically allocated memory, so we should always define our own destructor to ensure that all resources are properly released when the object goes out of scope.
// automatic destructor is useless