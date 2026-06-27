/**
 * File: test1.cpp
 * Demonstrates: Encapsulation & Access Modifiers
 * Description: Demonstrates restricting access to internal variables (private) and exposing them through validated setters and getters (public).
 * Interview Takeaway: Encapsulation protects class invariants and prevents unauthorized state corruption.
 */

#include <iostream>
using namespace std;

class BankAccount { // Class definition
private: // Inaccessible from outside the class
    int balance = 0;

public: // Accessible from anywhere in the program
    void deposit(int amount) {
        if(amount > 0)
            balance += amount;
    }

    void withdraw(int amount) {
        if(amount <= balance)
            balance -= amount;
    }

    void setBalance(int amount) {
        balance = amount;
    }

    int getBalance() {
        return balance;
    }
};

int main() { // Entry point of the program
    BankAccount acc;
    acc.deposit(5000);
    acc.withdraw(2000);

    cout << acc.getBalance();
}