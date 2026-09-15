# 01. OOP Fundamentals & C++ Deep Dive

> **Interview Mindset**: In C++ OOP rounds, interviewers don't just test theoretical definitions. They want to see if you understand **object memory layout, lifetime management, performance trade-offs, and dynamic dispatch mechanics (`vptr`/`vtable`)**.

---

## PART 1: The 4 Core OOP Fundamentals

```
                     THE 4 PILLARS OF OBJECT-ORIENTED DESIGN
┌──────────────────┐  ┌──────────────────┐  ┌──────────────────┐  ┌──────────────────┐
│  Encapsulation   │  │   Abstraction    │  │   Inheritance    │  │   Polymorphism   │
├──────────────────┤  ├──────────────────┤  ├──────────────────┤  ├──────────────────┤
│ Bundling data +  │  │ Exposing ONLY    │  │ Modeling "IS-A"  │  │ Many forms:      │
│ methods, hiding  │  │ essential public │  │ relationships &  │  │ Static (compile) │
│ internal state   │  │ contracts        │  │ code reuse       │  │ Dynamic (vtable) │
└──────────────────┘  └──────────────────┘  └──────────────────┘  └──────────────────┘
```

---

### 1. Class vs Object & Memory Layout

- **Class**: A user-defined blueprint/type defining fields and member functions. Resides in the code/text segment of binary memory.
- **Object**: A concrete instance of a class occupying physical memory during runtime.

```
OBJECT MEMORY LAYOUT (On Stack or Heap)
┌────────────────────────────────────────┐
│  Hidden vptr (8 bytes on 64-bit OS)    │ ──► Points to Class vtable in memory
├────────────────────────────────────────┤
│  Member Variable 1 (e.g., int id)      │ (4 bytes)
├────────────────────────────────────────┤
│  Padding / Alignment (4 bytes)         │ (Ensures 8-byte CPU boundary)
├────────────────────────────────────────┤
│  Member Variable 2 (double balance)    │ (8 bytes)
└────────────────────────────────────────┘
Total Size = sizeof(vptr) + 4 + 4(pad) + 8 = 24 bytes
```

---

### 2. Encapsulation & The "Tell, Don't Ask" Principle

Encapsulation bundles data with the operations that manipulate it, restricting direct access to prevent invalid object states.

```cpp
// ❌ BAD: Anemic Model violating Encapsulation (Caller micro-manages state)
class BankAccount {
public:
    double balance; // Public state: Any caller can make balance negative!
};

// Caller:
if (account.balance >= amount) {
    account.balance -= amount; // Logic leaked into caller
}

// ✅ GOOD: Proper Encapsulation with Invariant Protection
class BankAccount {
private:
    double balance_{0.0}; // Hidden internal representation

public:
    explicit BankAccount(double initialBalance) {
        if (initialBalance >= 0.0) balance_ = initialBalance;
    }

    bool withdraw(double amount) {
        if (amount <= 0.0 || amount > balance_) {
            return false; // Rejects illegal state transition
        }
        balance_ -= amount;
        return true;
    }

    [[nodiscard]] double getBalance() const noexcept { return balance_; }
};
```

> **Interview Golden Rule ("Tell, Don't Ask")**: Tell the object what action to perform (`account.withdraw(50)`), instead of asking for its internal state (`account.balance`) to make decisions on its behalf.

---

### 3. Abstraction

Abstraction separates **what** an entity does from **how** it is implemented.
- **Interface/Abstract Class**: Declares public behavior contracts.
- **Implementation**: Hides algorithms, network calls, and database operations.

```cpp
// Public Abstraction Contract
class IPaymentGateway {
public:
    virtual ~IPaymentGateway() = default;
    virtual bool processPayment(const std::string& orderId, double amount) = 0;
};

// Hidden Concrete Implementations
class StripeGateway final : public IPaymentGateway {
public:
    bool processPayment(const std::string& orderId, double amount) override {
        // Complex HTTP REST call, signature verification, token handling
        return true;
    }
};
```

---

### 4. Inheritance: IS-A vs HAS-A

- **Inheritance (IS-A)**: A `SavingsAccount` **is a** `BankAccount`.
- **Composition (HAS-A)**: A `Car` **has an** `Engine`.

```
        IS-A (Inheritance)                     HAS-A (Composition)
       ┌──────────────────┐                   ┌──────────────────┐
       │   BankAccount    │                   │       Car        │
       └────────▲─────────┘                   ├──────────────────┤
                │                             │ - Engine engine_ │
       ┌────────┴─────────┐                   └────────┬─────────┘
       │  SavingsAccount  │                            │ Owns
       └──────────────────┘                            ▼
                                              ┌──────────────────┐
                                              │      Engine      │
                                              └──────────────────┘
```

> [!WARNING]
> **The Fragile Base Class Trap**: Changes to a base class unintentionally break all derived classes. Favor **Composition over Inheritance** whenever dynamic flexibility or loose coupling is needed.

---

### 5. Polymorphism: Compile-Time vs Run-Time

| Feature | Compile-Time (Static) Polymorphism | Run-Time (Dynamic) Polymorphism |
| :--- | :--- | :--- |
| **Mechanisms** | Function Overloading, Operator Overloading, Templates, CRTP | Virtual Functions, Abstract Interfaces, Inheritance |
| **Binding Time** | Resolved early at compile time (Static Binding) | Resolved late at runtime via `vptr`/`vtable` (Dynamic Binding) |
| **Performance** | **Zero runtime overhead**. Functions can be fully inlined by CPU. | Small pointer indirection overhead (~1–2ns) + blocks CPU inlining. |
| **Flexibility** | Types must be known at compile time. | Concrete types can be selected dynamically at runtime (e.g. user plugin). |

```cpp
// 1. Compile-Time Polymorphism (Templates / Overloading)
template <typename T>
T add(T a, T b) { return a + b; } // Instantiated at compile time

// 2. Run-Time Polymorphism (Virtual Dispatch)
class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0; // Dynamic dispatch
};

class Circle : public Shape {
    double radius_;
public:
    explicit Circle(double r) : radius_(r) {}
    double area() const override { return 3.14159 * radius_ * radius_; }
};
```

---

## PART 2: C++ OOP Mechanics & Memory Internals

---

### 1. Constructors & Lifetime Management

C++ provides strict deterministic object construction and destruction (RAII — Resource Acquisition Is Initialization):

```cpp
class UserSession {
private:
    std::string userId_;
    int timeoutSeconds_;

public:
    // 1. Default Constructor
    UserSession() : userId_("guest"), timeoutSeconds_(300) {}

    // 2. Parameterized Constructor with Member Initializer List
    UserSession(std::string id, int timeout) 
        : userId_(std::move(id)), timeoutSeconds_(timeout) {}

    // 3. Explicit Constructor (Prevents accidental implicit type casting)
    explicit UserSession(std::string id) : UserSession(std::move(id), 300) {} 
    // ^ Delegating constructor

    // 4. Destructor (Executes deterministically when leaving scope)
    ~UserSession() = default;
};
```

> **Why Member Initializer Lists?**: Initializing members in constructor body causes fields to be **default-constructed first and then copy-assigned**, doubling work for heavy objects like `std::string` or `std::vector`.

---

### 2. Copy & Move Semantics: The Rule of 0, 3, and 5

When an object manages raw resources (memory buffers, file descriptors, network sockets), you must explicitly manage copying and moving:

```
┌─────────────────────────────────────────────────────────────────────────┐
│                      THE RULE OF 5 IN MODERN C++                        │
├────────────────────────────────┬────────────────────────────────────────┤
│ 1. Destructor                  │ Releases owned resource                │
│ 2. Copy Constructor            │ Performs DEEP copy of resource         │
│ 3. Copy Assignment Operator    │ Cleans up old resource + deep copies   │
│ 4. Move Constructor            │ Steals resource pointer, sets other=0  │
│ 5. Move Assignment Operator    │ Cleans old, steals other, sets other=0 │
└────────────────────────────────┴────────────────────────────────────────┘
```

#### Production C++ Rule of 5 Example:

```cpp
#include <iostream>
#include <utility>

class DynamicBuffer {
private:
    size_t size_{0};
    int* data_{nullptr};

public:
    // Standard Constructor
    explicit DynamicBuffer(size_t size) 
        : size_(size), data_(new int[size]()) {}

    // 1. Destructor
    ~DynamicBuffer() {
        delete[] data_;
    }

    // 2. Copy Constructor (Deep Copy)
    DynamicBuffer(const DynamicBuffer& other) 
        : size_(other.size_), data_(new int[other.size_]) {
        std::copy(other.data_, other.data_ + size_, data_);
    }

    // 3. Copy Assignment (Copy-and-Swap Idiom)
    DynamicBuffer& operator=(const DynamicBuffer& other) {
        if (this != &other) {
            int* newData = new int[other.size_];
            std::copy(other.data_, other.data_ + other.size_, newData);
            delete[] data_;
            data_ = newData;
            size_ = other.size_;
        }
        return *this;
    }

    // 4. Move Constructor (Resource Pilfering - No new allocation!)
    DynamicBuffer(DynamicBuffer&& other) noexcept 
        : size_(other.size_), data_(other.data_) {
        other.size_ = 0;
        other.data_ = nullptr; // Neutralize donor
    }

    // 5. Move Assignment
    DynamicBuffer& operator=(DynamicBuffer&& other) noexcept {
        if (this != &other) {
            delete[] data_;     // Free existing resource
            size_ = other.size_;
            data_ = other.data_; // Steal resource
            other.size_ = 0;
            other.data_ = nullptr;
        }
        return *this;
    }
};
```

> **The Rule of Zero**: If your class uses modern smart pointers (`std::unique_ptr`, `std::vector`, `std::string`), **do not write any of the 5 custom operators**. The compiler will synthesize optimal copy/move operations automatically!

---

### 3. Virtual Functions, `vptr` & `vtable` Internals

How does C++ achieve dynamic polymorphism under the hood?

```
OBJECT INSTANCE (In RAM)                 VIRTUAL METHOD TABLE (vtable in Read-Only Data)
┌────────────────────────┐              ┌────────────────────────────────────────────┐
│      vptr (8 bytes)    ├─────────────►│ &Dog::speak()     ──► Points to Dog code   │
├────────────────────────┤              ├────────────────────────────────────────────┤
│      name_ (string)    │              │ &Dog::destructor()──► Points to Dog dtor   │
└────────────────────────┘              └────────────────────────────────────────────┘
```

#### How Dynamic Dispatch Works:
1. When a class declares or inherits a `virtual` function, the compiler creates a static table of function pointers for that class called the **`vtable`**.
2. Every object instance of that class is invisibly augmented with a **virtual table pointer (`vptr`)** pointing to its class's `vtable`.
3. When calling `animalPtr->speak()`:
   - CPU fetches object's `vptr`.
   - Offsets into the `vtable` to slot index for `speak()`.
   - Jumps to the resolved function address.
4. **The Cost**:
   - +8 bytes memory per object (`vptr`).
   - One extra pointer dereference per virtual call.
   - Prevents CPU branch prediction and compiler function inlining.

---

### 4. The Virtual Destructor Rule (Critical Interview Question!)

```cpp
class Base {
public:
    Base() = default;
    ~Base() { std::cout << "Base destroyed\n"; } // ❌ NON-VIRTUAL!
};

class Derived : public Base {
    int* buffer_{new int[1000]};
public:
    ~Derived() { 
        delete[] buffer_;
        std::cout << "Derived buffer freed\n"; 
    }
};

int main() {
    Base* ptr = new Derived();
    delete ptr; 
    // 💥 DISASTER! Calls ~Base() only. ~Derived() is NEVER invoked!
    // buffer_ leaks 4,000 bytes. Undefined Behavior.
}
```

```
NON-VIRTUAL DESTRUCTION LEAK:
Base* ptr ──► [Base slice | Derived slice with heap buffer]
delete ptr ──► Executes ~Base() only ──► Derived slice leaked!

FIX: Add virtual to Base destructor:
class Base {
public:
    virtual ~Base() = default; // ✅ Virtual dispatch ensures ~Derived() executes first!
};
```

> **The Rule**: Always declare the destructor `virtual` in any base class that has at least one virtual function or is intended for polymorphic deletion.

---

### 5. Abstract Classes & Pure Virtual Interfaces

In C++, an **Abstract Class** has at least one **pure virtual function** (`= 0`). It cannot be directly instantiated.

```cpp
// Pure Interface (All methods pure virtual + virtual destructor)
class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(const std::string& message) = 0; // Pure virtual
};

class ConsoleLogger final : public ILogger {
public:
    void log(const std::string& message) override {
        std::cout << "[LOG]: " << message << "\n";
    }
};
```

---

### 6. Multiple Inheritance & The Diamond Problem

When a derived class inherits from two classes that share a common base:

```
               THE DIAMOND PROBLEM
                 ┌─────────────┐
                 │  Device     │ (has int id)
                 └──────┬──────┘
                        │
             ┌──────────┴──────────┐
             ▼                     ▼
      ┌─────────────┐       ┌─────────────┐
      │  Printer    │       │  Scanner    │
      └──────┬──────┘       └──────┬──────┘
             │                     │
             └──────────┬──────────┘
                        ▼
                 ┌─────────────┐
                 │ Copier      │ 💥 Contains TWO copies of Device!
                 └─────────────┘    (Which 'id' do you mean?)
```

#### The Solution: Virtual Inheritance (`virtual public`)

```cpp
#include <iostream>

class Device {
public:
    int id{0};
    virtual ~Device() = default;
};

// Use virtual inheritance so only ONE shared Device subobject exists in Copier
class Printer : virtual public Device {
public:
    void print() { std::cout << "Printing on device " << id << "\n"; }
};

class Scanner : virtual public Device {
public:
    void scan() { std::cout << "Scanning on device " << id << "\n"; }
};

class Copier : public Printer, public Scanner {
public:
    // Copier constructor is responsible for directly initializing the virtual base!
    Copier(int deviceId) {
        id = deviceId; // Unambiguous! Only one instance of Device::id exists.
    }
};
```

```
RESOLVED WITH VIRTUAL INHERITANCE:
Copier Memory Layout:
┌────────────────────────┐
│  vbptr (to Device)     │
│  Printer fields        │
├────────────────────────┤
│  vbptr (to Device)     │
│  Scanner fields        │
├────────────────────────┤
│  Single Shared Device  │ ──► id resolved unambiguously!
└────────────────────────┘
```

---

## 7. Interview Rapid-Fire Defense

| Question | Winning Answer |
| :--- | :--- |
| *"Why do we need a virtual destructor?"* | *"Without a virtual destructor, deleting a derived object via a base pointer invokes only the base destructor via static binding, skipping the derived destructor and leaking its member resources."* |
| *"What is the memory cost of adding `virtual` to a class?"* | *"Adds an 8-byte `vptr` per object instance and creates one static `vtable` array of function pointers per class in read-only memory."* |
| *"What is the difference between `override` and `virtual`?"* | *"`virtual` declares that a function can be overridden in subclasses. `override` is a compile-time check ensuring the signature matches an existing base virtual function, preventing subtle naming or parameter bugs."* |
| *"Can a constructor be virtual in C++?"* | *"No. To construct an object, the compiler must know its exact concrete type and allocate its exact memory layout upfront. However, the Factory Method and Prototype patterns act as virtual constructor idioms."* |
