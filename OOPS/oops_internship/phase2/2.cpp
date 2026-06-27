/**
 * File: 2.cpp
 * Demonstrates: Encapsulation & Access Modifiers
 * Description: Demonstrates restricting access to internal variables (private) and exposing them through validated setters and getters (public).
 * Interview Takeaway: Encapsulation protects class invariants and prevents unauthorized state corruption.
 */


#include <iostream>
using namespace std;

class Animal { // Class definition
private: // Inaccessible from outside the class
    int x;

public: // Accessible from anywhere in the program
    int y;
};

class Dog : public Animal { // Class definition
public: // Accessible from anywhere in the program
    void print() {
       // cout << x << endl; // we cannot use x here because it is private in the base class 'Animal'
        cout << y << endl; // we can use y here because it is public in the
    }
};

int main() { // Entry point of the program

    Dog d;

    //d.x = 5; // ERROR! 'x' is private in 'Animal'
    d.y = 10; // Allowed: 'y' is public in 'Animal'
    d.print(); // Calls the print function and prints the value of y

    return 0;
}

// point to remember // 1. Private members of a base class are NOT accessible in the derived class.
// 2. Public members of a base class are accessible in the derived class.
// 3. Protected members of a base class are accessible in the derived class but not outside of it.
