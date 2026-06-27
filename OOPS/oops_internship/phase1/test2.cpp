/**
 * File: test2.cpp
 * Demonstrates: implicit 'this' Pointer & Method Chaining
 * Description: Demonstrates using the 'this' pointer to access calling objects and enabling method chaining.
 * Interview Takeaway: The 'this' pointer is an implicit parameter to all non-static member functions.
 */

// method chaining in c++
#include <iostream>
using namespace std;    

class BankAccount { // Class definition
private: // Inaccessible from outside the class
    int balance = 0;

public: // Accessible from anywhere in the program
    BankAccount& deposit(int amount) {
        if(amount > 0)
            balance += amount;
        return *this;
    }

    BankAccount& withdraw(int amount) {
        if(amount <= balance)
            balance -= amount;
        return *this;
    }

    int getBalance() {
        return balance;
    }
};  

int main() { // Entry point of the program
    BankAccount acc;
    acc.deposit(5000).withdraw(2000).deposit(3000);

    cout << acc.getBalance();
}


// for interview - method chaining is a common technique used in C++ to improve code readability and maintainability. It allows you to call multiple methods on the same object in a single statement, which can make your code more concise and easier to read. In the example above, we have a BankAccount class with deposit and withdraw methods that return a reference to the object itself, allowing us to chain these method calls together. This technique is often used in fluent interfaces and builder patterns, we use This pointer to return the current object from the method, enabling the chaining of method calls. This pointer means the current object, and by returning *this, we allow the next method to be called on the same object.