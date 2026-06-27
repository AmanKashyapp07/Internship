/**
 * File: test17.cpp
 * Demonstrates: Static Members (Variables & Functions)
 * Description: Demonstrates static member variables shared across all instances and static member functions called without class instances.
 * Interview Takeaway: Static functions do not have access to the implicit 'this' pointer.
 */

#include <iostream>
using namespace std;

class Student { // Class definition
public: // Accessible from anywhere in the program
    static int count; // Static member: shared across all class instances

    Student() {
        count++;
    }
};

int Student::count = 0; // we cannot initialize static data members inside class because static data members are shared among all instances of the class, and they need to be defined and initialized outside the class definition to allocate memory for them. By defining Student::count outside the class, we ensure that there is only one instance of count that is shared among all Student objects, and we can initialize it to 0 before any Student objects are created. This allows us to keep track of the number of Student objects that have been created by incrementing count in the constructor each time a new Student object is instantiated.
// :: is used to define and initialize the static data member count outside the class definition. Since count is a static data member, it is shared among all instances of the Student class, and we need to define it outside the class to allocate memory for it. By using Student::count, we are specifying that we are referring to the count variable that belongs to the Student class, rather than any instance of the class. This allows us to initialize count to 0 before any Student objects are created, and then increment it in the constructor each time a new Student object is created.
int main() { // Entry point of the program

    Student s1;
    Student s2;
    Student s3;

    cout << Student::count;
}

// static data member is a member of a class that is shared among all objects of that class. It is declared with the static keyword and is not associated with any particular instance of the class. In the code above, we have defined a static data member called count in the Student class, which keeps track of the number of Student objects that have been created. Each time a new Student object is created, the constructor increments the count by 1. Since count is a static data member, it is shared among all instances of the Student class, which means that when we create multiple Student objects, they all share the same count variable. Therefore, when we print Student::count at the end of the main function, it outputs 3, which is the total number of Student objects that were created. Static data members are useful for keeping track of information that is common to all instances of a class, such as counting the number of objects or maintaining a shared resource.
// count is not stored in each object, it is stored separately in memory and shared among all objects of the class. When we create a new Student object, it does not have its own copy of the count variable; instead, it accesses the shared count variable that belongs to the Student class. Therefore, when we increment count in the constructor, we are modifying the shared count variable that is accessed by all Student objects. This is why when we print Student::count at the end of the main function, it outputs 3, which is the total number of Student objects that were created, rather than showing a separate count for each individual object. Static data members are stored separately from the instances of the class and are accessed using the class name rather than through an instance of the class.