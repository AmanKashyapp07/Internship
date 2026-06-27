/**
 * File: 9.cpp
 * Demonstrates: Operator Overloading (Compile-time Polymorphism)
 * Description: Demonstrates overloading C++ operators to perform custom operations on class objects.
 * Interview Takeaway: Operators like +, *, ==, and stream insertion << can be customized for objects.
 */

#include <iostream>
using namespace std;
// this code demonstrates operator overloading in C++. We have a class 'Complex' that represents complex numbers with real and imaginary parts. The '+' operator is overloaded to allow adding two Complex objects together, resulting in a new Complex object that contains the sum of the real and imaginary parts. The 'show' function is used to display the complex number in a readable format. In the main function, we create three Complex objects, add them together, and display the result.
class Complex { // Class definition
public: // Accessible from anywhere in the program

    int real;
    int imag;

    Complex(int r, int i) {
        real = r;
        imag = i;
    }

    Complex operator+(const Complex& other) {

        Complex temp(
            real + other.real,
            imag + other.imag
        );

        return temp;
    } // this returns a new Complex object that is the sum of the current object and the 'other' Complex object passed as a parameter, which can be added further to another Complex object or used in any other way as needed.
    Complex operator*(const Complex& other) {

        Complex temp(
            real * other.real - imag * other.imag,
            real * other.imag + imag * other.real
        );

        return temp;
    } // this returns a new Complex object that is the product of the current object and the 'other' Complex object passed as a parameter, which can be multiplied further to another Complex object or used in any other way as needed.
    void show() {
        cout << real << " + " << imag << "i" << endl;
    }
};

int main() { // Entry point of the program

    Complex c1(1, 2);
    Complex c2(3, 4);
    Complex c3(4,5);
    Complex c4 = c1 + c2+c3;
    Complex c5 = c1 * c2; // this will multiply c1 and c2 using the overloaded '*' operator and store the result in c5
   // c3.show();
    c4.show();
    c5.show();
}

// operator overloading is powerful feature in C++ that allows us to define custom behavior for operators when they are used with user-defined types (like classes). In this example, we have overloaded the '+' operator to enable adding two Complex objects together, and the '*' operator to enable multiplying two Complex objects together. This makes our code more intuitive and easier to read when working with complex numbers, otherwise we would have to call member functions like c1.add(c2) or c1.multiply(c2) which is less natural than using the '+' and '*' operators directly.

// c1+c2 internally becomes c1.operator+(c2) which calls the overloaded '+' operator function defined in the Complex class, allowing us to add two Complex objects together in a natural way. Similarly, c1*c2 becomes c1.operator*(c2) which calls the overloaded '*' operator function to multiply two Complex objects together.

// operators which cannot be overloaded in C++ are: 1. Scope resolution operator (::) 2. Member access operator (.) 3. Ternary conditional operator (?:) 4. sizeof operator 5. typeid operator 6. static_cast, dynamic_cast, const_cast, and reinterpret_cast operators.