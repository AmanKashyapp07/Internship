/**
 * File: test8.cpp
 * Demonstrates: Static Members (Variables & Functions)
 * Description: Demonstrates static member variables shared across all instances and static member functions called without class instances.
 * Interview Takeaway: Static functions do not have access to the implicit 'this' pointer.
 */

#include <iostream>
using namespace std;

class Student { // Class definition
public: // Accessible from anywhere in the program
    int age;

    void setAge(int age) {
        this->age = age;
    }

    void show() {
        cout << age << endl;
    }
};

int main() { // Entry point of the program
    Student s;

    s.setAge(18);

    s.show();
}

// this pointer is a special pointer that is implicitly passed to all non-static member functions of a class. It points to the object that is currently invoking the member function. In the code above, we have defined a setAge function that takes an integer parameter and assigns it to the age data member of the Student class using the this pointer. The this pointer allows us to differentiate between the parameter age and the data member age, which have the same name. When we call s.setAge(18), the this pointer points to the object s, and we are able to set the age data member of s to 18. Similarly, when we call s.show(), the this pointer points to s, and we are able to access the age data member of s and print its value to the console.
// this = &s means that the this pointer is pointing to the memory address of the object s. In C++, when we call a member function on an object, the this pointer is automatically set to point to that object. So, when we call s.setAge(18), the this pointer inside the setAge function points to the memory address of s, allowing us to access and modify the data members of s using the this pointer. This is how member functions can operate on the specific instance of the class that is invoking them.
// non static member function - A non-static member function is a member function of a class that operates on an instance of the class. It has access to the non-static data members of the class and can modify them. Non-static member functions are called on objects of the class, and they have access to the this pointer, which points to the specific instance of the class that is invoking the function. In contrast, static member functions do not operate on an instance of the class and do not have access to the this pointer or non-static data members. They can only access static data members and other static member functions of the class.
//static means that the member belongs to the class rather than to any specific instance of the class. A static member function can be called without creating an object of the class, and it can only access static data members and other static member functions of the class. In contrast, a non-static member function operates on an instance of the class and has access to non-static data members and the this pointer, which points to the specific instance of the class that is invoking the function. Static members are shared among all instances of the class, while non-static members are unique to each instance of the class.