/**
 * File: test3.cpp
 * Demonstrates: Operator Overloading (Compile-time Polymorphism)
 * Description: Demonstrates overloading C++ operators to perform custom operations on class objects.
 * Interview Takeaway: Operators like +, *, ==, and stream insertion << can be customized for objects.
 */

#include <iostream>
using namespace std;

class BankAccount;

// External function declaration
void printAccountInfo(BankAccount* acc);

class AccountManager { // Class definition
public: // Accessible from anywhere in the program
    void showBalance(BankAccount* acc);
};

class BankAccount { // Class definition
private: // Inaccessible from outside the class
    int balance = 0;

public: // Accessible from anywhere in the program
    void deposit(int amount) {
        if (amount > 0)
            balance += amount;
    }

    void withdraw(int amount) {
        if (amount <= balance)
            balance -= amount;
    }

    int getBalance() {
        return balance;
    }

    // Object passing itself to an external function
    void displayInfo() {
        printAccountInfo(this);
    }

    // Object passing itself to another class
    void sendToManager(AccountManager& manager) {
        manager.showBalance(this);
    }
};

// Definition after full BankAccount class is known
void AccountManager::showBalance(BankAccount* acc) {
    cout << "Balance from AccountManager: "
         << acc->getBalance() << endl;
} // this is needed because we need to know the definition of BankAccount class before we can define the showBalance function which takes a pointer to BankAccount as a parameter. By defining the showBalance function after the BankAccount class, we ensure that the compiler has all the necessary information about the BankAccount class when it compiles the showBalance function. This allows us to access the getBalance() method of the BankAccount class without any issues.

// External function definition
void printAccountInfo(BankAccount* acc) {
    cout << "Balance from external function: "
         << acc->getBalance() << endl;
}

int main() { // Entry point of the program
    BankAccount acc;

    acc.deposit(5000);
    acc.withdraw(2000);

    // Object passes itself to external function
    acc.displayInfo();

    // Object passes itself to another class
    AccountManager manager;
    acc.sendToManager(manager);

    return 0;
}

// :: means scope resolution operator, it is used to access members of a class or namespace. In the context of the code above, it is used to define the member functions of the BankAccount class outside of the class definition. For example, void BankAccount::deposit(int amount) defines the deposit function as a member of the BankAccount class. This allows us to separate the declaration of the class from the implementation of its member functions, which can improve code organization and readability.

// for interview - In C++, the 'this' pointer is a special pointer that points to the current object instance of a class. It is used within member functions to refer to the object that is invoking the function. The 'this' pointer allows you to access the members of the current object and can be used to resolve naming conflicts between member variables and parameters. In the code above, we use the 'this' pointer to pass the current object instance to an external function and another class, demonstrating how it can be used for object communication and interaction.

