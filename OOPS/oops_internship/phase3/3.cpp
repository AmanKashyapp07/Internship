/**
 * File: 3.cpp
 * Demonstrates: Abstract Classes & Pure Virtual Functions
 * Description: Demonstrates defining interfaces and abstract classes that cannot be instantiated directly.
 * Interview Takeaway: A class with at least one pure virtual function is abstract. Derived classes must implement all pure virtuals to be instantiated.
 */

#include <iostream>
using namespace std;

class PaymentMethod { // Class definition
public: // Accessible from anywhere in the program

    virtual void pay() = 0; // Pure virtual function (makes class abstract)

    virtual ~PaymentMethod() {} // Virtual function for runtime override dispatch
};

class UPI : public PaymentMethod { // Class definition
public: // Accessible from anywhere in the program

    void pay() override { // Indicates this function overrides a base virtual function
        cout << "UPI payment" << endl;
    }
};

class Card : public PaymentMethod { // Class definition
public: // Accessible from anywhere in the program

    void pay() override { // Indicates this function overrides a base virtual function
        cout << "Card payment" << endl;
    }
};

class PayPal : public PaymentMethod { // Class definition
public: // Accessible from anywhere in the program

    void pay() override { // Indicates this function overrides a base virtual function
        cout << "PayPal payment" << endl;
    }
};

class Cash : public PaymentMethod { // Class definition
public: // Accessible from anywhere in the program

    void pay() override { // Indicates this function overrides a base virtual function
        cout << "Cash payment" << endl;
    }
};

int main() { // Entry point of the program

    PaymentMethod* method1 = new UPI();
    PaymentMethod* method2 = new Card();
    PaymentMethod* method3 = new PayPal();
    PaymentMethod* method4 = new Cash();

    method1->pay(); // Outputs: UPI payment
    method2->pay(); // Outputs: Card payment
    method3->pay(); // Outputs: PayPal payment
    method4->pay(); // Outputs: Cash payment

    delete method1;
    delete method2;
    delete method3; 
    delete method4;

}

// OCP (Open/Closed Principle) states that software entities (classes, modules, functions, etc.) should be open for extension but closed for modification. In this example, we have a base class 'PaymentMethod' with a pure virtual function 'pay()', which allows us to define different payment methods (UPI, Card, PayPal) as derived classes that implement the 'pay()' function. This design allows us to add new payment methods in the future without modifying the existing code of the base class or other derived classes, thus adhering to the Open/Closed Principle. By using polymorphism, we can call the 'pay()' function on pointers to the base class, and the correct implementation will be executed based on the actual type of the object, allowing for easy extension of functionality without changing existing code. we can easily add new payment methods by simply creating new classes that inherit from 'PaymentMethod' and implement the 'pay()' function, without needing to modify any existing code, which is the essence of the Open/Closed Principle.
// OCP is closely related to polymorphism, which allows us to write code that can work with objects of different types through a common interface (like the 'PaymentMethod' base class), enabling us to extend functionality without modifying existing code, thus adhering to the Open/Closed Principle.