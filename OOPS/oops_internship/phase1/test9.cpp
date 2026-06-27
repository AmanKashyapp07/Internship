/**
 * File: test9.cpp
 * Demonstrates: Static Members (Variables & Functions)
 * Description: Demonstrates static member variables shared across all instances and static member functions called without class instances.
 * Interview Takeaway: Static functions do not have access to the implicit 'this' pointer.
 */

#include <iostream>
using namespace std;

class Student { // Class definition
public: // Accessible from anywhere in the program
    int age=0;

    Student& setAge(int age) {
        this->age += age;
        return *this;
    }
};

int main() { // Entry point of the program
    Student s;

    s.setAge(18).setAge(20);

    cout << s.age;
}

// method chaining is a programming technique where multiple method calls are chained together in a single statement. In the code above, we have defined a setAge function that takes an integer parameter and adds it to the age data member of the Student class using the this pointer. The setAge function returns a reference to the current object (*this), which allows us to chain multiple calls to setAge together. When we call s.setAge(18).setAge(20), the first call to setAge adds 18 to the age data member, and then returns a reference to the same object s. The second call to setAge then adds 20 to the age data member of s, resulting in a final age of 38. Method chaining can make code more concise and easier to read by allowing multiple operations to be performed on an object in a single statement.
// it is not available in static functions because static functions do not operate on an instance of the class and do not have access to the this pointer. Since method chaining relies on returning a reference to the current object using the this pointer, it cannot be used in static functions. Static functions can only access static data members and other static member functions of the class, and they cannot modify non-static data members or return a reference to an instance of the class. Therefore, method chaining is not applicable in static functions.