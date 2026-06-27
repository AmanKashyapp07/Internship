/**
 * File: 22.cpp
 * Demonstrates: Operator Overloading (Compile-time Polymorphism)
 * Description: Demonstrates overloading C++ operators to perform custom operations on class objects.
 * Interview Takeaway: Operators like +, *, ==, and stream insertion << can be customized for objects.
 */

class A { // Class definition
public: // Accessible from anywhere in the program
    void show(int) {}
};

class B : public A { // Class definition
public: // Accessible from anywhere in the program
    void show() {}
};

int main() { // Entry point of the program
    B b;
    b.show(); // This will call B's show() and not A's show(int) because of name hiding. In C++, when a derived class defines a function with the same name as a function in the base class, it hides all overloads of that function in the base class. To call A's show(int) from an object of class B, we need to use the scope resolution operator to specify that we want to call the base class version of the function, like this: b.A::show(5); where 5 is an example argument for the show(int) function.
    // this is not oevrriding because the function signatures are different, A's show takes an int parameter while B's show takes no parameters, so they are considered different functions and B's show does not override A's show, it simply hides it. To achieve overriding, the function signatures must be the same in both the base and derived classes.
}