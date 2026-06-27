/**
 * File: test6.cpp
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
    Student* s1 = new Student;

    s1->age = 18;

    cout << s1->age << endl;

    delete s1;
}

// we are using heap memory to create the object s1, which means that we need to manually deallocate the memory using the 'delete' operator. When we call 'delete s1', the destructor for the Student class is called, and we see the message "Destructor called" printed to the console. This is because when we delete an object that was allocated on the heap, the destructor is automatically called to clean up any resources that the object may have acquired during its lifetime. In this case, since we are not allocating any additional resources in the Student class, the destructor simply prints a message to indicate that it has been called.
// diff between stack and heap memory - Stack memory is a region of memory that is used for storing local variables and function call information. When a function is called, a new block of memory is allocated on the stack to store the function's local variables and other information. When the function returns, the block of memory is automatically deallocated. Heap memory, on the other hand, is a region of memory that is used for dynamic memory allocation. When you allocate memory on the heap using the 'new' operator, it remains allocated until you explicitly deallocate it using the 'delete' operator. This means that you have more control over the lifetime of the memory when using heap memory, but it also means that you need to be careful to avoid memory leaks by ensuring that all dynamically allocated memory is properly deallocated when it is no longer needed.

// dangling pointer - A dangling pointer is a pointer that points to a memory location that has been deallocated or released. This can lead to undefined behavior if the program tries to access or modify the memory through the dangling pointer. In C++, when you delete an object that was allocated on the heap, any pointers that were pointing to that object become dangling pointers. It is important to set any pointers that point to deallocated memory to nullptr after deleting the object to avoid accidentally accessing the dangling pointer and causing undefined behavior.
// eg of dangling pointer - In the code above, after we call 'delete s1', the pointer s1 becomes a dangling pointer because it still points to the memory location that was allocated for the Student object, but that memory has been deallocated. If we were to try to access s1->age after deleting s1, it would lead to undefined behavior because we would be trying to access memory that has already been released. To avoid this, we should set s1 to nullptr after deleting it, like this: 'delete s1; s1 = nullptr;'. This way, if we accidentally try to access s1 after it has been deleted, we will get a null pointer exception instead of undefined behavior.