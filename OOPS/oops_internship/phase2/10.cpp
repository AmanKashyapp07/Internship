/**
 * File: 10.cpp
 * Demonstrates: Operator Overloading (Compile-time Polymorphism)
 * Description: Demonstrates overloading C++ operators to perform custom operations on class objects.
 * Interview Takeaway: Operators like +, *, ==, and stream insertion << can be customized for objects.
 */

#include <iostream>
using namespace std;

class Animal { // Class definition
public: // Accessible from anywhere in the program
    void sound() {
        cout << "Animal sound" << endl;
    }
};

class Dog : public Animal { // Class definition
public: // Accessible from anywhere in the program
    void sound() {
        cout << "Dog barking" << endl;
    }
};

int main() { // Entry point of the program

    Dog d;

    d.sound(); // this will print dog barking, for animal sound we need to call the sound function of the animal class using the dog object as follows: d.Animal::sound();
    d.Animal::sound(); // this will print animal sound because we are explicitly calling the sound function of the animal class using the dog object and scope resolution operator (::) to specify which class's function we want to call.
}

// here, dog sound is printing instead of animal sound because of function overriding. Function overriding occurs when a derived class provides a specific implementation of a function that is already defined in its base class. In this case, the 'sound' function in the Dog class overrides the 'sound' function in the Animal class. When we call d.sound(), it calls the overridden version in the Dog class, resulting in "Dog barking" being printed instead of "Animal sound".
// can we call sound func of animal class using dog object? Yes, we can call the sound function of the Animal class using a Dog object by explicitly specifying the scope of the function we want to call. We can do this using the scope resolution operator (::) as follows:
