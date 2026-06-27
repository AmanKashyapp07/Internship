/**
 * File: 15.cpp
 * Demonstrates: Runtime Polymorphism (Virtual Functions)
 * Description: Demonstrates dynamic dispatch at runtime using virtual functions and pointers/references.
 * Interview Takeaway: Runtime polymorphism is resolved at runtime using vtable and vptr. Base class must have a virtual destructor.
 */

#include <iostream>
#include <string>
using namespace std;

class Grandparent { // Class definition
protected: // Accessible only to this class and derived classes
    string legacyName;
public: // Accessible from anywhere in the program
    Grandparent(string name) {
        legacyName = name;
        cout << "Grandparent Constructor initialized with: " << legacyName << "\n";
    }
    virtual ~Grandparent() { // Virtual function for runtime override dispatch
        cout << "Grandparent Destructor called\n";
    }
};

class Parent : public Grandparent { // Class definition
protected: // Accessible only to this class and derived classes
    int parentAge;
public: // Accessible from anywhere in the program
    // Parent receives two values, keeps one, and forwards 'name' to Grandparent
    Parent(string name, int age) : Grandparent(name) {
        parentAge = age;
        cout << "Parent Constructor initialized.\n";
    }
    ~Parent() {
        cout << "Parent Destructor called\n";
    }
};

class Grandchild : public Parent { // Class definition
private: // Inaccessible from outside the class
    string hobby;
public: // Accessible from anywhere in the program
    // Grandchild receives everything and forwards 'name' and 'age' to Parent
    Grandchild(string name, int age, string myHobby) : Parent(name, age) {
        hobby = myHobby;
        cout << "Grandchild Constructor initialized.\n";
    }
    void print(){
        cout<<"Grandchild's legacy name: "<<legacyName<<"\n"; // Accessing inherited property
        cout<<"Grandchild's hobby: "<<hobby<<"\n"; // Accessing own property
        cout<<"Grandchild's parent age: "<<parentAge<<"\n"; // Accessing inherited property
    }
    ~Grandchild() {
        cout << "Grandchild Destructor called\n";}
};
// grandchild cannot directly access grandparent constructor, it can only access the parent constructor which in turn can access the grandparent constructor, this is because of the inheritance hierarchy and the way constructors are called in C++. When we create an object of the Grandchild class, it first calls the Parent constructor, which then calls the Grandparent constructor to initialize the inherited properties. The Grandchild constructor can only directly call the Parent constructor, and it relies on the Parent constructor to call the Grandparent constructor to ensure that all inherited properties are properly initialized. This is a fundamental aspect of how constructors work in an inheritance hierarchy in C++.

int main() { // Entry point of the program
    Grandchild* gc = new Grandchild("Smith Family Legacy", 50, "Painting");
    gc->print();
    delete gc; // delete means we are done using the dynamically allocated object and we want to free up the memory it was using, when we call delete on a pointer to an object, it will call the destructor of that object to perform any necessary cleanup before deallocating the memory. In this case, when we delete gc, it will call the destructor of the Grandchild class first, then the Parent class destructor, and finally the Grandparent class destructor, in that order, ensuring that all resources are properly released and any necessary cleanup is performed for each class in the inheritance hierarchy. This is important to prevent memory leaks and ensure that any resources allocated by the object are properly released when we are done using it.
    return 0;
}