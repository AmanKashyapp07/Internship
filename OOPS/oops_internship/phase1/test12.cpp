/**
 * File: test12.cpp
 * Demonstrates: Operator Overloading (Compile-time Polymorphism)
 * Description: Demonstrates overloading C++ operators to perform custom operations on class objects.
 * Interview Takeaway: Operators like +, *, ==, and stream insertion << can be customized for objects.
 */

#include <iostream>
using namespace std;

class Student { // Class definition
public: // Accessible from anywhere in the program
    int* marks; // marks is a pointer to an integer, which means that it points to a memory location where an integer value is stored. In this case, we are using marks to store the marks of a student, and we will allocate memory for it dynamically using the 'new' operator in the constructor of the Student class.

    Student(int value) {
        marks = new int(value);
    }

    Student(const Student& other) {
        marks = new int(*other.marks);
    }

    void show() {
        cout << *marks << endl;
    }
};

int main() { // Entry point of the program
    Student s1(90);

    Student s2(s1);

    *s2.marks = 100;

    s1.show();
    s2.show();
}

// this is deep copying manually implemented in the copy constructor of the Student class. In the copy constructor, we allocate new memory for the marks pointer and copy the value from the other object's marks pointer. This ensures that each Student object has its own separate copy of the marks, and changes to one object's marks will not affect the other object's marks. In this example, when we create s2 as a copy of s1, s2 gets its own copy of the marks with the value 90. When we change s2's marks to 100, it does not affect s1's marks, which remain at 90. Therefore, when we call s1.show() and s2.show(), we see that s1's marks are 90 and s2's marks are 100, demonstrating that deep copying has been successfully implemented.
// problem arises on heap memory when we have pointer data members in our class. If we use the default copy constructor provided by the compiler, it will perform a shallow copy of the object's data members, which means that it will copy the pointer value but not the actual memory it points to. As a result, multiple objects may end up sharing the same memory for their data members, which can lead to unintended side effects when one object modifies the shared data. For example, in the code above, if we had used the default copy constructor instead of defining our own copy constructor that performs a deep copy, both s1 and s2 would point to the same memory location for marks. Therefore, changing s2's marks would also change s1's marks, leading to unexpected behavior in our program. To avoid this issue, we should always define our own copy constructor that performs a deep copy of any pointer data members in our class to ensure that each object has its own separate copy of the memory for those data members.