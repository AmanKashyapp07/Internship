/**
 * File: test13.cpp
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
        marks = new int(value); // new because we are allocating memory for marks on the heap, which allows us to have more control over the lifetime of the memory and avoid issues with stack memory limitations. By using new, we can allocate memory for marks dynamically at runtime, and we can also ensure that the memory is properly deallocated when it is no longer needed by using delete in the destructor or when we are done with the Student object.
    }

    Student& operator=(const Student& other) {

        if(this != &other) {
            delete marks; // this is because we need to free the existing memory that marks is pointing to before we can assign it a new value. If we don't delete the existing memory, we would have a memory leak because the old memory would still be allocated but we would lose the reference to it when we assign marks to point to the new memory. By deleting the existing memory, we ensure that we are not leaking memory and that we can safely assign marks to point to the new memory allocated for the other object's marks.

            marks = new int(*other.marks);
        } // only when the current object is not the same as the other object, we perform the assignment. This check is important to avoid self-assignment, which can lead to issues such as deleting the marks pointer before copying the value from the other object. By checking if this is not equal to &other, we ensure that we only perform the assignment when we are assigning from a different object, and we can safely delete the existing marks and allocate new memory for the marks without risking deleting memory that we are trying to copy from.

        return *this;
    }

    void show() {
        cout << *marks << endl;
    }
};

int main() { // Entry point of the program
    Student s1(90);

    Student s2(80);

    s2 = s1;

    *s2.marks = 100; // * because marks is a pointer, so we need to dereference it to access the value it points to. By changing *s2.marks to 100, we are modifying the value that s2's marks pointer points to. Since s2 is now a copy of s1 (after the assignment), s2's marks pointer points to the same memory location as s1's marks pointer. Therefore, changing *s2.marks also changes the value that s1's marks pointer points to, which means that both s1 and s2 will show the updated marks value when we call their show() method.

    s1.show();
    s2.show();
}