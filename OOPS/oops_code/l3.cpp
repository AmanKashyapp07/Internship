/**
 * @file l3_inheritance_polymorphism.cpp
 * @brief Lecture 3: Inheritance & Polymorphism (Types of Inheritance, Access specifiers, Compile/Runtime Polymorphism, vtable/vptr, Diamond Problem, Slicing, Function hiding)
 */

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// === 1. Types of Inheritance ===

class Creature {
public:
    string species;
    void breathe() const { cout << species << " is breathing\n"; }
};

// Single Inheritance
class Dog : public Creature {
public:
    Dog() { species = "Dog"; }
    void bark() const { cout << "Woof! Woof!\n"; }
};

// Multilevel Inheritance: Creature -> Dog -> Labrador
class Labrador : public Dog {
public:
    Labrador() { species = "Labrador"; }
};

// Hierarchical Inheritance: Cat and Dog both inherit from Creature
class Cat : public Creature {
public:
    Cat() { species = "Cat"; }
    void meow() const { cout << "Meow!\n"; }
};

// Multiple Inheritance: Inherits from both Dog and Cat (hypothetical mixed breed)
class ToyDog {
public:
    void play() const { cout << "Playing with toy dog\n"; }
};

class CompanionDog : public Dog, public ToyDog {};

// === 2. Access Specifiers in Inheritance ===

class BaseAccess {
public:    int pub = 1;
protected: int prot = 2;
private:   [[maybe_unused]] int priv = 3; // inaccessible to anyone outside BaseAccess
};

// Public Inheritance: public -> public, protected -> protected
class PublicDerived : public BaseAccess {
    void test() {
        pub = 10;   // OK
        prot = 20;  // OK
        // priv = 30; // ERROR: private member of base is inaccessible
    }
};

// Protected Inheritance: public -> protected, protected -> protected
class ProtectedDerived : protected BaseAccess {
    void test() {
        pub = 10;   // OK (is protected in ProtectedDerived)
        prot = 20;  // OK (is protected in ProtectedDerived)
    }
};

// Private Inheritance: public -> private, protected -> private
class PrivateDerived : private BaseAccess {
    void test() {
        pub = 10;   // OK (is private in PrivateDerived)
        prot = 20;  // OK (is private in PrivateDerived)
    }
};

// === 3. Constructor & Destructor Execution Order ===

class Parent {
public:
    Parent()  { cout << "Parent constructor called\n"; }
    virtual ~Parent() { cout << "Parent destructor called\n"; }
};

class Child : public Parent {
public:
    Child()  { cout << "Child constructor called\n"; }
    ~Child() override { cout << "Child destructor called\n"; }
};

// === 4. Compile-Time Polymorphism (Overloading) ===

class Calculator {
public:
    // Function Overloading
    int add(int a, int b) { return a + b; }
    double add(double a, double b) { return a + b; }
    int add(int a, int b, int c) { return a + b + c; }
};

// Operator Overloading
class Vector2D {
public:
    double x, y;
    Vector2D(double x = 0, double y = 0) : x(x), y(y) {}

    // Overload + operator
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    // Overload * operator for scalar multiplication
    Vector2D operator*(double scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    // Overload == operator
    bool operator==(const Vector2D& other) const {
        return (x == other.x) && (y == other.y);
    }

    // Overload << operator as a friend function
    friend ostream& operator<<(ostream& os, const Vector2D& v) {
        os << "(" << v.x << ", " << v.y << ")";
        return os;
    }
};

// === 5. Runtime Polymorphism, vtable/vptr size, override & final ===

class Shape {
public:
    virtual void draw() const { cout << "Drawing generic Shape\n"; }
    virtual ~Shape() = default;
};

// final class: cannot be inherited from
class Circle final : public Shape {
public:
    void draw() const override { cout << "Drawing Circle\n"; }
};

// Sizing demonstration
class ClassNoVirtual {
    [[maybe_unused]] int x;
};

class ClassWithVirtual {
    [[maybe_unused]] int x;
public:
    virtual void func() {}
};

// === 6. Diamond Problem & Virtual Inheritance ===

class Animal {
public:
    int age = 0;
    void breathe() const { cout << "Animal is breathing...\n"; }
};

// Virtual inheritance ensures only a single shared copy of the Animal subobject exists.
class VirtualDog : virtual public Animal {};
class VirtualCat : virtual public Animal {};

class VirtualHybrid : public VirtualDog, public VirtualCat {};

// Non-virtual equivalent causes ambiguity
class NonVirtualDog : public Animal {};
class NonVirtualCat : public Animal {};
class NonVirtualHybrid : public NonVirtualDog, public NonVirtualCat {};

// === 7. Object Slicing ===

class BasePolymorphic {
public:
    virtual void speak() const { cout << "Base speaking\n"; }
};

class DerivedPolymorphic : public BasePolymorphic {
public:
    void speak() const override { cout << "Derived speaking\n"; }
};

// === 8. Function Hiding vs Overriding ===

class Super {
public:
    virtual void foo(int x) { cout << "Super::foo(int)\n"; }
    void bar(int x) { cout << "Super::bar(int)\n"; }
};

class Sub : public Super {
public:
    // Overriding: same name, same signature, virtual in base
    void foo(int x) override { cout << "Sub::foo(int)\n"; }

    // Hiding: same name, but not virtual in base
    void bar(int x) { cout << "Sub::bar(int)\n"; }

    // Hiding: same name, different signature. Hides ALL overloads in Base!
    void foo(double x) { cout << "Sub::foo(double) -> Hides Super::foo(int)\n"; }

    // Bring back hidden Super overloads
    using Super::foo;
};

int main() {
    cout << "=== 1. Constructor and Destructor Order ===\n";
    {
        Child c;
    } // Child destroyed first, then Parent

    cout << "\n=== 2. Compile-Time Polymorphism (Overloading) ===\n";
    Calculator calc;
    cout << "Add 2 ints: " << calc.add(5, 10) << "\n";
    cout << "Add 2 doubles: " << calc.add(5.5, 10.5) << "\n";
    
    Vector2D v1(1.0, 2.0), v2(3.0, 4.0);
    cout << "Vector v1 + v2: " << (v1 + v2) << "\n";
    cout << "Vector v1 * 3.0: " << (v1 * 3.0) << "\n";
    cout << "Vector equality check: " << (v1 == v2 ? "Equal" : "Not Equal") << "\n";

    cout << "\n=== 3. Runtime Polymorphism (Virtual Dispatch) ===\n";
    Shape* shPtr = new Circle();
    shPtr->draw(); // calls Circle::draw() due to dynamic dispatch (vtable lookup)
    delete shPtr;

    cout << "\n=== 4. Under the hood: vtable/vptr sizing ===\n";
    cout << "Size of ClassNoVirtual: " << sizeof(ClassNoVirtual) << " bytes (only int x)\n";
    cout << "Size of ClassWithVirtual: " << sizeof(ClassWithVirtual) 
         << " bytes (int x + hidden vptr pointer)\n";

    cout << "\n=== 5. Diamond Problem solved with Virtual Inheritance ===\n";
    VirtualHybrid vh;
    vh.age = 10;     // Unambiguous: only one shared Animal subobject exists
    vh.breathe();    // Unambiguous

    NonVirtualHybrid nvh;
    // nvh.age = 10; // COMPILE ERROR: Request for member 'age' is ambiguous
    nvh.NonVirtualDog::age = 10; // Disambiguation required
    nvh.NonVirtualCat::age = 15;

    cout << "\n=== 6. Object Slicing Trap ===\n";
    DerivedPolymorphic dp;
    
    // Value assignment (slicing occurs!)
    BasePolymorphic baseVal = dp; // The derived class slice is sliced off!
    baseVal.speak(); // Prints: Base speaking

    // Pointer/Reference assignment (polymorphism preserved)
    BasePolymorphic& baseRef = dp;
    baseRef.speak(); // Prints: Derived speaking

    cout << "\n=== 7. Function Hiding ===\n";
    Sub subObj;
    subObj.foo(3.14); // Calls Sub::foo(double)
    subObj.foo(5);    // Calls Super::foo(int) thanks to "using Super::foo;" bring-back
    
    Super* superPtr = &subObj;
    superPtr->bar(5); // Calls Super::bar(int) because bar is not virtual (static dispatch)
    subObj.bar(5);    // Calls Sub::bar(int) (static dispatch)

    return 0;
}
