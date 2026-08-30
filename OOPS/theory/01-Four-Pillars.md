# The Four Pillars of Object-Oriented Programming in C++

> **Scope:** Encapsulation & Invariant Enforcement, RAII Resource Management, Abstraction via Pure Virtual Interfaces, Inheritance Access Specifiers, The Object Slicing Phenomenon, Compile-Time vs. Runtime Polymorphism, Dynamic Dispatch Mechanics (`vtable` & `vptr`), Virtual Destructors, and Object Memory Layouts.

---

# Table of Contents
1. [Encapsulation & Class Invariant Enforcement](#1-encapsulation--class-invariant-enforcement)
2. [Abstraction & Pure Virtual Interface Contracts](#2-abstraction--pure-virtual-interface-contracts)
3. [Inheritance Access Modes & Object Slicing Mechanics](#3-inheritance-access-modes--object-slicing-mechanics)
4. [Polymorphism: Static vs. Dynamic Dispatch](#4-polymorphism-static-vs-dynamic-dispatch)
5. [Virtual Table (`vtable`) & Virtual Pointer (`vptr`) Memory Architecture](#5-virtual-table-vtable--virtual-pointer-vptr-memory-architecture)
6. [Core Theoretical Summary Principles](#6-core-theoretical-summary-principles)

---

# 1. Encapsulation & Class Invariant Enforcement

**Encapsulation** binds data members and member functions into a unified structure while restricting direct external access to internal state via access specifiers (`private`, `protected`, `public`). It guarantees that class state transitions satisfy internal validation invariants:

```cpp
class BankAccount {
private:
    double balance; // Internal state shielded from direct external mutation

public:
    explicit BankAccount(double initial) : balance(initial >= 0.0 ? initial : 0.0) {}

    void deposit(double amount) {
        if (amount > 0.0) {
            balance += amount; // Validates state transition invariant
        }
    }

    bool withdraw(double amount) {
        if (amount > 0.0 && amount <= balance) {
            balance -= amount;
            return true;
        }
        return false;
    }

    double getBalance() const { return balance; } // Read-only inspector
};
```

### Encapsulation and RAII:
In C++, **Resource Acquisition Is Initialization (RAII)** couples resource management directly to object lifecycles through encapsulation:
- **Constructor:** Acquires resources (heap memory, mutex locks, file descriptors).
- **Destructor:** Automatically releases acquired resources upon object destruction (e.g. `std::unique_ptr`, `std::lock_guard`).

---

# 2. Abstraction & Pure Virtual Interface Contracts

**Abstraction** separates high-level contractual behavior from underlying implementation details. It defines *what* operations an object supports while encapsulating *how* those operations are executed:

```cpp
// Pure Abstract Interface Contract
class IDatabaseConnection {
public:
    virtual ~IDatabaseConnection() = default; // Mandatory virtual destructor for polymorphic base classes
    virtual bool connect(const std::string& connStr) = 0; // Pure virtual function (= 0)
    virtual void executeQuery(const std::string& query) = 0;
};

// Concrete Relational Implementation
class PostgresConnection : public IDatabaseConnection {
public:
    bool connect(const std::string& connStr) override {
        // Socket connection, TLS handshake, authentication logic
        return true;
    }
    void executeQuery(const std::string& query) override {
        // Wire protocol serialization, network socket I/O
    }
};
```

### Theoretical Architectural Impact:
- **Dependency Inversion:** High-level modules depend on abstractions (`IDatabaseConnection`), not low-level concrete implementations (`PostgresConnection`).
- **Substitutability:** Alternative implementations (e.g. `MockDatabaseConnection`) can be substituted without modifying client invocation logic.

---

# 3. Inheritance Access Modes & Object Slicing Mechanics

Inheritance models subtyping relationships and enables code reuse across hierarchical structures.

```
+---------------------------------------------------------------------------------------------------+
| BASE ACCESS LEVEL    | PUBLIC INHERITANCE (is-a)     | PROTECTED INHERITANCE (impl)  | PRIVATE (has-a)    |
+---------------------------------------------------------------------------------------------------+
| public members       | public in Derived             | protected in Derived          | private in Derived |
| protected members    | protected in Derived          | protected in Derived          | private in Derived |
| private members      | Inaccessible in Derived       | Inaccessible in Derived       | Inaccessible       |
+---------------------------------------------------------------------------------------------------+
```

### The Object Slicing Phenomenon:
Object Slicing occurs when a derived class instance is assigned or passed by value to a base class variable. The derived-specific data members and derived `vtable` pointer are stripped away:

```cpp
class Base {
public:
    int b = 1;
    virtual void print() const { std::cout << "Base\n"; }
    virtual ~Base() = default;
};

class Derived : public Base {
public:
    int d = 2;
    void print() const override { std::cout << "Derived\n"; }
};

void evaluateByValue(Base obj) { // Passed by value -> Object Slicing occurs
    obj.print(); // Invokes Base::print() because derived attributes were sliced
}

void evaluateByReference(const Base& obj) { // Passed by reference -> Polymorphic dispatch preserved
    obj.print(); // Invokes Derived::print() dynamically
}
```

```
Memory Layout during Slicing:
Derived Object: [ Base sub-object (b, vptr) | Derived sub-object (d) ]
                         | (Copied by value into Base)
                         v
Base Parameter: [ Base sub-object (b, Base vptr) ]  (Field 'd' is sliced off)
```

---

# 4. Polymorphism: Static vs. Dynamic Dispatch

```
+---------------------------------------------------------------------------------------------------+
| ATTRIBUTE            | COMPILE-TIME (Static / Early Binding) | RUNTIME (Dynamic / Late Binding)   |
+---------------------------------------------------------------------------------------------------+
| Primary Mechanism    | Function Overloading, Templates, CRTP | Virtual Functions, `vptr` / `vtable`|
| Resolution Timestamp | Compile-time (Zero execution overhead)| Runtime (One pointer indirection)  |
| Code Generation      | Inlines easily; code expansion per type| Single code block; vtable per class|
| Execution Cost       | Zero nanosecond dispatch cost         | Small (~1-3ns) memory dereference  |
+---------------------------------------------------------------------------------------------------+
```

### Static Polymorphism via Curiously Recurring Template Pattern (CRTP):
CRTP resolves polymorphic calls at compile time without dynamic dispatch overhead:
```cpp
template <typename Derived>
class BaseProcessor {
public:
    void process() {
        static_cast<Derived*>(this)->processImpl(); // Compile-time static binding
    }
};

class FastProcessor : public BaseProcessor<FastProcessor> {
public:
    void processImpl() {
        // High-throughput specialized processing
    }
};
```

---

# 5. Virtual Table (`vtable`) & Virtual Pointer (`vptr`) Memory Architecture

Dynamic polymorphism in C++ is implemented via compiler-generated dispatch tables:

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
Object Instance in RAM:                Virtual Table (vtable in Read-Only Data):
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

### Dynamic Invocation Sequence (`animalPtr->speak()`):
1. **Fetch `vptr`:** Dereference the hidden `__vptr` located at offset 0 of the object instance.
2. **Index Function Pointer:** Read function address at slot index corresponding to `speak()`: `func_ptr = vptr[0]`.
3. **Branch Execution:** Jump to address: `(*func_ptr)(animalPtr)`.

### Virtual Destructor Invariant:
If a class contains any virtual functions, its destructor **must be declared `virtual`**. Deleting a derived object through a base class pointer (`Base* ptr = new Derived(); delete ptr;`) when the base destructor is non-virtual invokes undefined behavior, executing only the base destructor and leaking derived resources.

---

# 6. Core Theoretical Summary Principles

1. **Encapsulation:** Protects internal class invariants by restricting state manipulation to explicit public methods.
2. **Abstraction Contracts:** Decouples interface definitions from implementation details via pure virtual member functions (`= 0`).
3. **Object Slicing Invariant:** Polymorphic types must be passed by pointer (`Base*`) or reference (`const Base&`) to prevent slicing of derived fields and `vptr` overwrite.
4. **Dynamic Dispatch Mechanics:** Runtime polymorphism relies on class-level `vtable` function pointer arrays and instance-level `vptr` hidden pointers.
5. **Base Destructor Requirement:** Deleting polymorphic objects requires virtual destructors to ensure derived cleanup sequences execute completely.
