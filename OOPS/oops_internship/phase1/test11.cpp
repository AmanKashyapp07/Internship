/**
 * File: test11.cpp
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
};

int main() { // Entry point of the program
    Student s1(85);

    cout << *s1.marks<<endl;
    Student s2(s1);
    cout << *s2.marks<<endl; // now both s1 and s2 point to the same memory location where the marks are stored, which means that if we change the marks through one of the objects, it will affect the other object as well. This is because the default copy constructor performs a shallow copy of the object's data members, which means that it copies the pointer value but not the actual memory it points to. Therefore, both s1 and s2 have their marks pointer pointing to the same memory location, and any changes made to the marks through either s1 or s2 will affect both objects.
    *s2.marks = 90; // changing the marks through s2 will also change the marks for s1 because both s1 and s2 point to the same memory location where the marks are stored. This is a common issue with shallow copying, where multiple objects end up sharing the same memory for their data members, leading to unintended side effects when one object modifies the shared data. To avoid this issue, we should define our own copy constructor that performs a deep copy of the data members, ensuring that each object has its own separate copy of the memory for the marks.
    cout << *s1.marks<<endl; // now when we print the marks for s1, it will show 90 instead of 85 because both s1 and s2 point to the same memory location where the marks are stored, and we changed the marks through s2.

    delete s1.marks;
    delete s2.marks;
}

// problems with shallow copy - When we use the default copy constructor provided by the compiler, it performs a shallow copy of the object's data members. This means that if the class has any pointer data members that point to dynamically allocated memory, the shallow copy will only copy the pointer value, not the actual memory it points to. As a result, multiple objects may end up sharing the same memory for their data members, which can lead to unintended side effects when one object modifies the shared data. For example, in the code above, both s1 and s2 point to the same memory location for marks, so changing the marks through s2 also changes the marks for s1. This can lead to bugs and unexpected behavior in our program. To avoid this issue, we should define our own copy constructor that performs a deep copy of the data members, ensuring that each object has its own separate copy of the memory for the marks.