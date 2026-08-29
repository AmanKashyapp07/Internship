# Master Guide 01: The Four Pillars of OOP in C++

> **Focus:** Encapsulation & RAII, Abstraction via Pure Virtual Interfaces, Inheritance Modes & The Object Slicing Trap, Compile-Time vs Runtime Polymorphism (Templates vs vptr/vtable Dynamic Dispatch), and Memory Layout Internals.
> 
> *Targeted for Top-Tier C++ Systems, HFT, and Backend Engineering Interviews.*

---

# Table of Contents
1. [Encapsulation & Invariant Protection](#1-encapsulation--invariant-protection)
2. [Abstraction & Pure Virtual Interfaces](#2-abstraction--pure-virtual-interfaces)
3. [Inheritance Modes & The Object Slicing Trap](#3-inheritance-modes--the-object-slicing-trap)
4. [Polymorphism: Compile-Time vs. Runtime (vtable Internals)](#4-polymorphism-compile-time-vs-runtime-vtable-internals)
5. [High-Frequency C++ Interview Drill & Verbal Q&A](#5-high-frequency-c-interview-drill--verbal-qa)

---

# 1. Encapsulation & Invariant Protection

```cpp
class BankAccount {
private:
    double balance; // Hidden internal state (cannot be tampered with directly)

public:
    explicit BankAccount(double initial) : balance(initial >= 0 ? initial : 0) {}

    void deposit(double amount) {
        if (amount > 0) balance += amount; // Validates and enforces class invariants
    }

    bool withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            return true;
        }
        return false;
    }

    double getBalance() const { return balance; } // Read-only inspector
};
```

- **One-Line Intuition:** Encapsulation is packaging raw data with the validation rules that govern it, preventing external code from corrupting internal invariants.
- **The Interview Trap:** Saying encapsulation is merely "making fields private and adding getter/setter methods." Blindly exposing public getters and setters for all private fields violates encapsulation by leaking mutable internal state.
- **30-Second Verbal Answer:** **"Encapsulation binds data members and member functions into a single cohesive unit while restricting direct external access via `private` access specifiers. It ensures that class invariants can only be mutated through validated public interfaces and forms the foundation of RAII in C++."**
- **C++ Low-Level Angle:** `struct` and `class` are identical in C++ except for default visibility: `struct` defaults to `public` inheritance and members, while `class` defaults to `private`.
- **Follow-up:** *What is RAII and how does it relate to encapsulation?* **Resource Acquisition Is Initialization (RAII) encapsulates resource management inside an object's lifecycle: acquire in constructor, release in destructor (`std::unique_ptr`, `std::lock_guard`).**

---

# 2. Abstraction & Pure Virtual Interfaces

```cpp
// Pure Abstract Interface (Contract)
class IDatabaseConnection {
public:
    virtual ~IDatabaseConnection() = default; // Mandatory virtual destructor
    virtual bool connect(const std::string& connStr) = 0; // Pure virtual function
    virtual void executeQuery(const std::string& query) = 0;
};

// Concrete Implementation
class PostgresConnection : public IDatabaseConnection {
public:
    bool connect(const std::string& connStr) override {
        // Socket connection, TLS handshake, auth
        return true;
    }
    void executeQuery(const std::string& query) override {
        // Wire protocol serialization, network write
    }
};
```

- **One-Line Intuition:** Abstraction is the dashboard of a car: you press the accelerator pedal to speed up without knowing whether you are driving an EV or a V8 combustion engine.
- **The Interview Trap:** Confusing Abstraction with Encapsulation. **Encapsulation** hides internal data (data hiding); **Abstraction** hides implementation complexity behind a simplified contract (design decoupling).
- **30-Second Verbal Answer:** **"Abstraction separates the 'what' from the 'how' by defining high-level contracts using pure virtual functions (`= 0`). Callers depend solely on the abstract interface, allowing underlying implementations to change without breaking client code."**
- **LLD / System Design Angle:** Abstraction is the cornerstone of the **Dependency Inversion Principle (DIP)** and the **Factory Pattern**, allowing seamless swapping of mock database drivers during unit testing.

---

# 3. Inheritance Modes & The Object Slicing Trap

```
+---------------------------------------------------------------------------------------------------+
| BASE SPECIFIER       | PUBLIC INHERITANCE (is-a)     | PROTECTED INHERITANCE (impl)  | PRIVATE (has-a)    |
+---------------------------------------------------------------------------------------------------+
| public members       | public in Derived             | protected in Derived          | private in Derived |
| protected members    | protected in Derived          | protected in Derived          | private in Derived |
| private members      | Inaccessible in Derived       | Inaccessible in Derived       | Inaccessible       |
+---------------------------------------------------------------------------------------------------+
```

### The Object Slicing Trap (Pass-by-Value):
```cpp
class Base { public: int b = 1; virtual void print() { std::cout << "Base"; } };
class Derived : public Base { public: int d = 2; void print() override { std::cout << "Derived"; } };

void badFunction(Base obj) { // PASSED BY VALUE -> OBJECT SLICING OCCURS!
    obj.print(); // Prints "Base" because derived parts (d and Derived vtable) were sliced off!
}

void goodFunction(const Base& obj) { // PASSED BY REFERENCE -> POLYMORPHIC DISPATCH PRESERVED!
    obj.print(); // Prints "Derived"
}
```

```
Memory Layout during Slicing:
Derived Object: [ Base sub-object (b, vptr) | Derived sub-object (d) ]
                         | (Copied by value into Base)
                         v
Base Parameter: [ Base sub-object (b, Base vptr) ]  <--- 'd' is chopped off!
```

- **One-Line Intuition:** Passing a derived object by value to a base parameter is like forcing a smartphone into a regular telephone mold by chopping off the screen.
- **The Interview Trap:** Passing polymorphic types by value. It causes **Object Slicing**, stripping the derived fields and rewriting the `vptr` to the base class `vtable`.
- **30-Second Verbal Answer:** **"Inheritance models an 'is-a' relationship via `public` derivation. However, passing a derived class object by value to a base class parameter slices off all derived member variables and resets the `vptr` to the base vtable. Polymorphic types must always be passed by reference (`const Base&`) or pointer (`std::unique_ptr<Base>`)."**

---

# 4. Polymorphism: Compile-Time vs. Runtime (vtable Internals)

```
+---------------------------------------------------------------------------------------------------+
| ATTRIBUTE            | COMPILE-TIME (Static / Early Binding) | RUNTIME (Dynamic / Late Binding)   |
+---------------------------------------------------------------------------------------------------+
| Mechanism            | Function/Operator Overloading,        | Virtual Functions, `vptr` / `vtable`|
|                      | Templates, CRTP                       | dynamic dispatch                   |
| Resolution Time      | Compile-time (Zero runtime overhead)  | Runtime (1 memory indirection)     |
| Binary Footprint     | Code bloat per template instantiation | Small (1 vtable per class + 1 vptr)|
| Optimization         | Easily inlined by compiler            | Hard to inline (requires devirtual)|
+---------------------------------------------------------------------------------------------------+
```

### C++ Runtime Dynamic Dispatch (`vptr` & `vtable`):
```cpp
class Animal {
public:
    virtual void speak() { std::cout << "Animal sound\n"; }
    virtual ~Animal() = default;
};

class Dog : public Animal {
public:
    void speak() override { std::cout << "Woof\n"; }
};
```

```
Object in RAM:                         Virtual Table (vtable in Read-Only Data):
+-----------------------+              +------------------------------------+
| Dog Instance          |              | Animal vtable:                     |
| [ vptr ] ------------>| -----------> |   [0] -> &Animal::speak            |
| [ Dog fields... ]     |              |   [1] -> &Animal::~Animal          |
+-----------------------+              +------------------------------------+
                                       | Dog vtable:                        |
                                       |   [0] -> &Dog::speak               |
                                       |   [1] -> &Dog::~Dog                |
                                       +------------------------------------+
```

### How `dogPtr->speak()` Executes:
1. Fetch `vptr` from first 8 bytes of object instance: `vptr = dogPtr->__vptr`.
2. Index offset in vtable corresponding to `speak()` (e.g. index 0): `func_ptr = vptr[0]`.
3. Jump and execute through function pointer: `(*func_ptr)(dogPtr)`.

- **One-Line Intuition:** Compile-time polymorphism is a phone with physical speed-dial buttons (hardcoded at build time); runtime polymorphism is looking up the phone number in a live contacts book before calling.
- **The Interview Trap:** Claiming dynamic dispatch has "massive overhead." A virtual call is just one extra memory lookup (`vptr[offset]`) and typically costs under 2-3 nanoseconds unless it causes an instruction cache miss.
- **30-Second Verbal Answer:** **"Polymorphism allows objects to take multiple forms. Compile-time polymorphism resolves function signatures during compilation via overloading and templates with zero runtime cost. Runtime polymorphism uses virtual functions where each class has a static `vtable` of function pointers and each object instance holds a hidden `vptr` pointing to its class vtable."**

---

# 5. High-Frequency C++ Interview Drill & Verbal Q&A

### Q1: Why must a Base class with virtual functions always have a `virtual` destructor?
> **Answer:** If a derived class object is deleted through a base class pointer (`Base* p = new Derived(); delete p;`) with a non-virtual destructor, **only the Base destructor executes**, failing to clean up Derived resources and triggering **Undefined Behavior**.

### Q2: What is the difference between `override` and `final` in C++11?
> **Answer:** `override` tells the compiler to verify that the method is overriding an exact matching virtual method in the base class (catching signature typos at compile time). `final` prevents further overriding of a virtual method in derived classes or prevents inheritance of a class entirely, enabling **compiler devirtualization optimizations**.

### Q3: What is the size of an empty class in C++?
> **Answer:** **1 byte** (e.g. `sizeof(Empty) == 1`). The C++ standard mandates that every distinct object instance must have a unique, non-zero memory address. If the class has virtual functions, its size becomes **8 bytes** on 64-bit systems to accommodate the hidden `vptr`.

### Q4: What is CRTP (Curiously Recurring Template Pattern) and why is it used?
> **Answer:** A static polymorphism technique where a derived class inherits from a templated base class parameterized by the derived class itself (`class Derived : public Base<Derived>`). It achieves **polymorphic method dispatch at compile time with zero vtable/vptr overhead**, frequently used in High-Frequency Trading (HFT).

### Q5: Can a constructor or destructor be `virtual` in C++?
> **Answer:** **Destructors can and often must be virtual.** **Constructors CANNOT be virtual** because to create an object, the compiler must know its exact concrete type at compile-time to allocate memory and initialize the `vptr`.
