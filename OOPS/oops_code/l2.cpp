/**
 * @file l2_encapsulation_abstraction.cpp
 * @brief Lecture 2: Encapsulation & Abstraction (Const-correctness, Abstract Classes, Interfaces, Builder & Strategy Patterns, mutable)
 */

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <memory>

using namespace std;

// 1. Encapsulation & Getter/Setter Design with validation
class BankAccount {
private:
    string owner;
    double balance;
    int pin;

public:
    BankAccount(string ownerName, int securityPin, double initialBalance = 0.0) 
        : owner(ownerName), balance(initialBalance), pin(securityPin) {}

    // Read-only getter (marked const for const-correctness)
    double getBalance() const {
        return balance;
    }

    // Read-only getter
    string getOwner() const {
        return owner;
    }

    // Validated setter (deposit)
    void deposit(double amount) {
        if (amount <= 0) {
            throw invalid_argument("Deposit amount must be positive");
        }
        balance += amount;
    }

    // Validated action (withdraw)
    bool withdraw(double amount, int enteredPin) {
        if (enteredPin != pin) {
            cout << "[Error] Incorrect PIN for account of " << owner << "\n";
            return false;
        }
        if (amount > balance) {
            cout << "[Error] Insufficient funds for account of " << owner << "\n";
            return false;
        }
        balance -= amount;
        return true;
    }
};

// 2. Const-correctness & Logical Const-ness using 'mutable'
class Point {
private:
    int x, y;
    // 'mutable' allows these variables to be modified even in a const member function.
    // Useful for caching, instrumentation, or mutex locking (logical constness).
    mutable int accessCount = 0;

public:
    Point(int px, int py) : x(px), y(py) {}

    // Const member function: cannot modify x or y, but can modify accessCount
    int getX() const {
        accessCount++; // allowed because accessCount is mutable
        return x;
    }

    int getY() const {
        accessCount++;
        return y;
    }

    // Non-const member function: modifies state
    void setX(int px) { x = px; }

    int getAccessCount() const {
        return accessCount;
    }
};

// 3. Abstraction: Abstract Class
// A class with at least one pure virtual function is abstract and cannot be instantiated.
class Shape {
protected:
    string colorName;

public:
    Shape(string col) : colorName(col) {}

    // Virtual destructor is MANDATORY for base classes to ensure derived classes are cleaned up correctly
    virtual ~Shape() {
        cout << "Shape destructor called\n";
    }

    // Pure virtual functions: forces derived classes to implement them
    virtual double area() const = 0;
    virtual double perimeter() const = 0;

    // Concrete virtual function with default implementation
    virtual void draw() const {
        cout << "Drawing a " << colorName << " shape\n";
    }
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(string col, double r) : Shape(col), radius(r) {}
    ~Circle() override {
        cout << "Circle destructor called\n";
    }

    double area() const override {
        return 3.14159 * radius * radius;
    }

    double perimeter() const override {
        return 2 * 3.14159 * radius;
    }

    void draw() const override {
        cout << "Drawing a " << colorName << " Circle of radius " << radius << "\n";
    }
};

// 4. Interfaces in C++
// C++ doesn't have an 'interface' keyword, so we simulate it using an abstract class
// with ONLY pure virtual functions and NO member variables (except a virtual destructor).
class IPrintable {
public:
    virtual ~IPrintable() = default;
    virtual void print() const = 0; // Pure virtual
};

class ISerializable {
public:
    virtual ~ISerializable() = default;
    virtual string serialize() const = 0;
};

// A class implementing multiple interfaces
class Document : public IPrintable, public ISerializable {
private:
    string content;

public:
    Document(string text) : content(text) {}
    
    void print() const override {
        cout << "Document Content: " << content << "\n";
    }

    string serialize() const override {
        return "<doc>" + content + "</doc>";
    }
};

// 5. Builder Pattern sketch
class Pizza {
private:
    string size;
    bool extraCheese = false;
    bool pepperoni = false;

    // Constructor: only called via Builder::build()
    Pizza(string sz, bool cheese, bool pep) : size(sz), extraCheese(cheese), pepperoni(pep) {}

public:
    void describe() const {
        cout << size << " pizza with" 
             << (extraCheese ? " extra cheese" : "") 
             << (pepperoni ? " pepperoni" : "") << "\n";
    }

    class Builder;
};

class Pizza::Builder {
private:
    string size;
    bool extraCheese = false;
    bool pepperoni = false;

public:
    Builder(string sz) : size(sz) {}

    Builder& addExtraCheese() {
        extraCheese = true;
        return *this;
    }

    Builder& addPepperoni() {
        pepperoni = true;
        return *this;
    }

    Pizza build() {
        return Pizza(size, extraCheese, pepperoni);
    }
};

// 6. Strategy Pattern sketch
class SortStrategy {
public:
    virtual ~SortStrategy() = default;
    virtual void sort(vector<int>& arr) const = 0;
};

class BubbleSort : public SortStrategy {
public:
    void sort(vector<int>& arr) const override {
        cout << "Sorting vector using Bubble Sort strategy...\n";
        // Bubble sort implementation sketch
    }
};

class QuickSort : public SortStrategy {
public:
    void sort(vector<int>& arr) const override {
        cout << "Sorting vector using Quick Sort strategy...\n";
        // Quick sort implementation sketch
    }
};

class Sorter {
private:
    unique_ptr<SortStrategy> strategy;

public:
    Sorter(unique_ptr<SortStrategy> s) : strategy(std::move(s)) {}

    void setStrategy(unique_ptr<SortStrategy> s) {
        strategy = std::move(s);
    }

    void performSort(vector<int>& arr) const {
        if (strategy) {
            strategy->sort(arr);
        }
    }
};

int main() {
    cout << "=== 1. Encapsulation & Validation ===\n";
    BankAccount acc("Alice", 1234, 1000.0);
    acc.deposit(500.0);
    cout << "Balance after deposit: $" << acc.getBalance() << "\n";
    
    if (acc.withdraw(300.0, 1234)) {
        cout << "Withdrawal successful! New balance: $" << acc.getBalance() << "\n";
    }
    
    // Testing validation
    try {
        acc.deposit(-100);
    } catch (const invalid_argument& e) {
        cout << "Caught validation error: " << e.what() << "\n";
    }

    cout << "\n=== 2. Const-Correctness & mutable ===\n";
    const Point pt(5, 10);
    // Can call getX() on a const object because getX() is a const member function
    cout << "Point X: " << pt.getX() << "\n";
    cout << "Point Y: " << pt.getY() << "\n";
    cout << "Access count: " << pt.getAccessCount() << "\n";

    cout << "\n=== 3. Abstraction (Abstract Classes & Pointers) ===\n";
    // Shape* s = new Shape("Red"); // Error: Cannot instantiate abstract class Shape
    Shape* shape = new Circle("Blue", 5.0);
    shape->draw();
    cout << "Area of Shape: " << shape->area() << "\n";
    delete shape; // Calls Circle destructor then Shape destructor due to virtual destructor

    cout << "\n=== 4. Interfaces ===\n";
    Document doc("OOP Lecture Notes");
    doc.print();
    cout << "Serialized: " << doc.serialize() << "\n";

    cout << "\n=== 5. Builder Pattern ===\n";
    Pizza myPizza = Pizza::Builder("Large")
                        .addExtraCheese()
                        .addPepperoni()
                        .build();
    myPizza.describe();

    cout << "\n=== 6. Strategy Pattern ===\n";
    vector<int> data = {5, 2, 8, 1, 9};
    Sorter sorter(make_unique<BubbleSort>());
    sorter.performSort(data);

    sorter.setStrategy(make_unique<QuickSort>());
    sorter.performSort(data);

    return 0;
}
