/**
 * @file l1_basics.cpp
 * @brief Lecture 1: OOP Fundamentals (Class, Object, Stack vs Heap, Access Modifiers, this, static, scope)
 */

#include <iostream>
#include <string>
#include <memory>

using namespace std;

// Global variable to demonstrate Scope Resolution Operator (::)
int value = 100;

// 1. Class definition (Blueprint)
class Car {
private:
    // private member: accessible only within the class
    int speed;

protected:
    // protected member: accessible within the class and its derived classes
    string model;

public:
    // public member: accessible from outside the class
    string brand;

    // Parameterized constructor
    Car(string b, string m, int s) : speed(s), model(m), brand(b) {}

    // Method to accelerate
    void accelerate(int increase) {
        speed += increase;
        cout << brand << " " << model << " accelerated to " << speed << " km/h\n";
    }

    // Method showing read-only access to private member 'speed'
    int getSpeed() const {
        return speed;
    }

    // Method to demonstrate 'this' pointer & method chaining
    // By returning a reference to the current object (*this), we can chain method calls.
    Car& setBrand(const string& b) {
        this->brand = b; // 'this->' disambiguates member from parameter
        return *this;
    }

    Car& setModel(const string& m) {
        this->model = m;
        return *this;
    }

    Car& setSpeed(int s) {
        this->speed = s;
        return *this;
    }
};

// 2. Struct vs Class default access
// In C++, the ONLY difference between class and struct is the default access modifier.
class DefaultClass {
    [[maybe_unused]] int x; // private by default
};

struct DefaultStruct {
    int x; // public by default
};

// 3. Static Members
class Counter {
private:
    // Static member variable declaration (shared among all instances)
    static int totalCount;

public:
    Counter() {
        totalCount++; // Increment count whenever an object is created
    }

    ~Counter() {
        totalCount--; // Decrement count when object is destroyed
    }

    // Static member function: can be called without any instance.
    // It can ONLY access static variables/functions and has NO 'this' pointer.
    static int getTotalCount() {
        return totalCount;
    }
};

// Definition and initialization of static member variable (must be done outside class definition)
int Counter::totalCount = 0;

// 4. Scope Resolution Operator (::) Demonstration
class ScopeDemo {
public:
    int value = 20;

    void show(int value) {
        cout << "Local parameter value: " << value << "\n";
        cout << "Class member value (using this->): " << this->value << "\n";
        cout << "Global value (using ::): " << ::value << "\n";
    }
};

int main() {
    cout << "=== 1. Class vs Object & Stack vs Heap ===\n";
    
    // Stack Allocation: automatically managed scope-based lifetime
    Car stackCar("Tesla", "Model S", 0);
    stackCar.accelerate(50);
    
    // Heap Allocation: manual memory management (risks memory leaks if not deleted)
    Car* heapCar = new Car("BMW", "M5", 10);
    heapCar->accelerate(80);
    delete heapCar; // MUST free memory explicitly to avoid leak
    
    // Modern C++ heap allocation using smart pointer (auto deallocation, exception safe)
    std::unique_ptr<Car> smartCar = std::make_unique<Car>("Audi", "RS6", 20);
    smartCar->accelerate(100);
    // smartCar is automatically deleted here when it goes out of scope

    cout << "\n=== 2. Struct vs Class Default Access ===\n";
    DefaultStruct s;
    s.x = 42; // OK: public by default
    cout << "Struct default access works: s.x = " << s.x << "\n";
    // DefaultClass c;
    // c.x = 42; // COMPILE ERROR: x is private by default in class

    cout << "\n=== 3. implicit 'this' & Method Chaining ===\n";
    Car customCar("Toyota", "Corolla", 0);
    customCar.setBrand("Honda").setModel("Civic").setSpeed(60);
    cout << "Chained Car details: " << customCar.brand << ", Speed: " << customCar.getSpeed() << "\n";

    cout << "\n=== 4. Static Members ===\n";
    cout << "Initial count: " << Counter::getTotalCount() << "\n";
    {
        Counter c1;
        Counter c2;
        cout << "Count with 2 active instances: " << Counter::getTotalCount() << "\n";
    } // c1 and c2 are destroyed here
    cout << "Count after instances destroyed: " << Counter::getTotalCount() << "\n";

    cout << "\n=== 5. Scope Resolution Operator (::) ===\n";
    ScopeDemo sd;
    sd.show(5);

    return 0;
}
