# OOP in C++ — High ROI Revision (Microsoft Interview Prep)

> **Mindset**: Microsoft C++ interviews emphasize **runtime mechanics** (`vtable`, `vptr`, virtual destructors), **object lifecycle** (Rule of 3/5, RAII, deep vs shallow copy), **name hiding**, **multiple inheritance & diamond problem**, and **thread-safe design patterns** (Meyers' Singleton).

---

## 1. Core Pillars — C++ Internals & Traps

### 1. Encapsulation & Access Specifiers
- **Encapsulation**: Bundles data with functions and enforces boundaries via `private`, `protected`, and `public`.
- **C++ Inheritance Modes (Often tested)**:
  - `class Derived : public Base` ➔ Public stays public, protected stays protected.
  - `class Derived : protected Base` ➔ Public & protected become protected.
  - `class Derived : private Base` (default for `class`) ➔ All inherited members become private (represents "implemented-in-terms-of", similar to composition).
- **`friend` keyword**: Grants a non-member function or another class access to `private` and `protected` members. Breaks pure encapsulation; used judiciously (e.g., operator overloading `operator<<`).

---

### 2. Abstraction via Pure Virtual Interfaces
C++ has no `interface` keyword. An interface is represented as an **Abstract Base Class** containing only **pure virtual functions** (`= 0`) and a virtual destructor:

```cpp
class IShape {
public:
    virtual ~IShape() = default;          // Crucial: Virtual destructor!
    virtual double getArea() const = 0;   // Pure virtual function
    virtual void draw() const = 0;
};
```

---

### 3. Polymorphism Under the Hood (vtable & vptr)

- **Compile-Time (Static)**: Function overloading, operator overloading, templates (`CRTP` / compile-time polymorphism). Zero runtime overhead.
  - *Trap*: Can you overload based on return type alone? **No**. The compiler cannot resolve the target function when the caller does not capture the return value (`foo();`).
- **Runtime (Dynamic)**: Virtual functions (`virtual`) resolved via **Dynamic Dispatch**.

#### How the Compiler Implements Dynamic Dispatch:
1. **`vtable` (Virtual Method Table)**:
   - For every class containing at least one virtual function, the compiler generates a static table of function pointers.
2. **`vptr` (Virtual Table Pointer)**:
   - Every object instance of that class contains a hidden pointer (usually the first 8 bytes on a 64-bit machine) pointing to its class's `vtable`.
3. **Execution Cost**: Calling `ptr->draw()` requires:
   `vptr dereference ➔ offset lookup in vtable ➔ function call jump` (1 extra pointer dereference + prevents inlining).

```
Object in Memory:                vtable in Read-Only Data:
┌──────────────┐                ┌─────────────────────────┐
│ vptr         │───────────────▶│ &Circle::draw()         │
├──────────────┤                ├─────────────────────────┤
│ radius = 5.0 │                │ &Circle::getArea()      │
└──────────────┘                └─────────────────────────┘
```

#### Why Base Class Destructors MUST be `virtual`:
If you delete a derived object through a base pointer (`Base* b = new Derived(); delete b;`), a non-virtual destructor invokes **Undefined Behavior (UB)**. In practice, only `~Base()` executes, leaking any heap memory, file handles, or sockets allocated by `Derived`.
```cpp
class Base {
public:
    virtual ~Base() { cout << "~Base\n"; } // Ensures ~Derived() is called first!
};

class Derived : public Base {
    int* data;
public:
    Derived() : data(new int[100]) {}
    ~Derived() override { delete[] data; cout << "~Derived\n"; }
};
```

---

## 2. Overloading, Overriding & C++ Name Hiding

| Feature | Function Overloading | Function Overriding |
|---|---|---|
| **Binding** | Compile-time (Static) | Runtime (Dynamic) |
| **Scope** | Same scope/class | Derived class overrides Base class |
| **Signature** | Must have **different** parameters | Must have **identical** signature and qualifiers (`const`) |
| **Return Type** | Can be different | Must be identical **or covariant** (`Derived*` overrides `Base*`) |

### The C++ Name Hiding Trap:
If a derived class declares a function with the same name as a function in the base class, **it hides all overloads of that name in the base class**, even if parameter lists differ!
```cpp
class Base {
public:
    void print(int x) { cout << "int: " << x << endl; }
    void print(double x) { cout << "double: " << x << endl; }
};

class Derived : public Base {
public:
    void print(string s) { cout << "string: " << s << endl; }
    // Unhide Base overloads:
    using Base::print; 
};

Derived d;
d.print(5); // ERROR without 'using Base::print;' because print(string) hides Base::print!
```

---

## 3. Multiple Inheritance & The Diamond Problem

When class `D` inherits from both `B` and `C`, and both `B` and `C` inherit from `A`, `D` receives **two duplicate copies** of `A`'s member variables, creating ambiguity (`d.aValue` causes compile error).

```
       [ A ]
      ▲     ▲
     /       \
  [ B ]     [ C ]
     \       /
      ▼     ▼
       [ D ]
```

### Solution: `virtual` Inheritance
Declare base inheritance as `virtual` in intermediate classes:
```cpp
class A {
public:
    int x;
    A(int val = 0) : x(val) {}
};

class B : virtual public A {}; // Virtual inheritance
class C : virtual public A {}; // Virtual inheritance

class D : public B, public C {
public:
    // With virtual inheritance, the most derived class (D) is responsible for initializing A!
    D(int val) : A(val), B(), C() {}
};
```
*How it works internally*: The compiler inserts a virtual base pointer (`vptr` / `vbase_offset`) inside `B` and `C` pointing to a single shared `A` subobject in memory.

---

## 4. SOLID Principles in C++

### 1. Single Responsibility (SRP)
A class should have only one reason to change. Separate business logic from logging/persistence:
```cpp
class Order { /* holds order items, calculates total */ };
class OrderRepository { public: void saveToDatabase(const Order& order); };
class InvoicePrinter  { public: void printInvoice(const Order& order); };
```

### 2. Open/Closed Principle (OCP)
Open for extension, closed for modification. Use interfaces and polymorphism rather than modifying giant `switch-case` statements:
```cpp
class IPaymentMethod {
public:
    virtual ~IPaymentMethod() = default;
    virtual void pay(double amount) = 0;
};
class CreditCardPayment : public IPaymentMethod { /* ... */ };
class UPIPayment        : public IPaymentMethod { /* ... */ };
```

### 3. Liskov Substitution Principle (LSP) — *Classic Interview Trap*
Subtypes must be substitutable for base types without breaking invariants.
- **The Square-Rectangle Trap**:
```cpp
class Rectangle {
protected:
    int width, height;
public:
    virtual void setWidth(int w) { width = w; }
    virtual void setHeight(int h) { height = h; }
    int getArea() const { return width * height; }
};

class Square : public Rectangle {
public:
    void setWidth(int w) override { width = height = w; } // Violates caller expectations!
    void setHeight(int h) override { width = height = h; }
};

void verifyArea(Rectangle& r) {
    r.setWidth(5);
    r.setHeight(10);
    // Caller expects 5 * 10 = 50. If r is a Square, area is 100! LSP is broken.
    assert(r.getArea() == 50);
}
```
*Fix*: Make `Shape` the base class with `getArea()`; do not inherit `Square` from `Rectangle`.

### 4. Interface Segregation (ISP)
Prefer small, cohesive pure virtual interfaces over "fat" interfaces:
```cpp
struct IPrintable { virtual void print() = 0; virtual ~IPrintable() = default; };
struct IScannable { virtual void scan() = 0;  virtual ~IScannable() = default; };
// A basic printer implements only IPrintable, not a bloated IMultiFunctionMachine.
```

### 5. Dependency Inversion (DIP)
Depend on abstractions, not concrete classes. Inject interfaces via smart pointers:
```cpp
class Car {
    std::unique_ptr<IEngine> engine; // Injected abstraction, not concrete V8Engine
public:
    explicit Car(std::unique_ptr<IEngine> eng) : engine(std::move(eng)) {}
};
```

---

## 5. Design Patterns in Modern C++

### 1. Thread-Safe Singleton (Meyers' Singleton)
In C++11 and later (§6.7), initialization of function-local static variables is **guaranteed to be thread-safe** by the language runtime. Naive Double-Checked Locking is obsolete in modern C++:

```cpp
class Singleton {
public:
    // Delete copy and move semantics to enforce single instance
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    static Singleton& getInstance() {
        static Singleton instance; // Initialized exactly once in a thread-safe manner
        return instance;
    }

    void doWork() { /* ... */ }

private:
    Singleton() { /* private constructor */ }
    ~Singleton() = default;
};
```

*(If interviewer insists on manual thread-safe DCL, use `std::call_once` with `std::once_flag` or `std::atomic<Singleton*>` with memory order fences).*

---

### 2. Factory Pattern (using `std::unique_ptr`)
```cpp
enum class VehicleType { Car, Bike };

class VehicleFactory {
public:
    static std::unique_ptr<IVehicle> createVehicle(VehicleType type) {
        if (type == VehicleType::Car) return std::make_unique<Car>();
        if (type == VehicleType::Bike) return std::make_unique<Bike>();
        return nullptr;
    }
};
```

---

### 3. Observer Pattern (Preventing Memory Leaks with `std::weak_ptr`)
Avoid circular references and dangling pointers by storing observers as `std::weak_ptr`:
```cpp
class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void onUpdate(int state) = 0;
};

class Subject {
    std::vector<std::weak_ptr<IObserver>> observers;
public:
    void attach(std::shared_ptr<IObserver> obs) {
        observers.push_back(obs);
    }
    void notify(int state) {
        for (auto it = observers.begin(); it != observers.end();) {
            if (auto sharedObs = it->lock()) { // Convert weak_ptr to shared_ptr
                sharedObs->onUpdate(state);
                ++it;
            } else {
                it = observers.erase(it); // Clean up expired observer!
            }
        }
    }
};
```

---

### 4. Strategy Pattern (Dynamic Algorithm Swapping)
```cpp
class ISortStrategy {
public:
    virtual ~ISortStrategy() = default;
    virtual void sort(std::vector<int>& data) = 0;
};

class QuickSort : public ISortStrategy { /* ... */ };
class MergeSort : public ISortStrategy { /* ... */ };

class Sorter {
    std::unique_ptr<ISortStrategy> strategy;
public:
    void setStrategy(std::unique_ptr<ISortStrategy> strat) { strategy = std::move(strat); }
    void execute(std::vector<int>& data) { if (strategy) strategy->sort(data); }
};
```

---

## 6. Object Lifecycle: Shallow vs. Deep Copy & Rule of 3 / 5 / 0

### Shallow Copy vs. Deep Copy
- **Shallow Copy**: Default member-wise copy. Pointer addresses are copied, resulting in two objects pointing to the **same heap memory**. Causes **Double-Free crashes** when destructors run!
- **Deep Copy**: Allocates new memory and copies the underlying values.

### Rule of 5 (Modern C++ Resource Management)
If a class manages an explicit raw resource (heap memory, OS file descriptor), you must explicitly declare or delete all 5 special member functions:
```cpp
class Buffer {
    int* data;
    size_t size;
public:
    explicit Buffer(size_t s) : size(s), data(new int[s]) {}

    // 1. Destructor
    ~Buffer() { delete[] data; }

    // 2. Copy Constructor (Deep Copy)
    Buffer(const Buffer& other) : size(other.size), data(new int[other.size]) {
        std::copy(other.data, other.data + size, data);
    }

    // 3. Copy Assignment Operator (Copy-and-Swap Idiom)
    Buffer& operator=(Buffer other) { // Takes by value
        swap(*this, other);
        return *this;
    }

    // 4. Move Constructor (Transfers ownership, zero-cost)
    Buffer(Buffer&& other) noexcept : data(nullptr), size(0) {
        swap(*this, other);
    }

    // 5. Move Assignment Operator
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }

    friend void swap(Buffer& a, Buffer& b) noexcept {
        std::swap(a.data, b.data);
        std::swap(a.size, b.size);
    }
};
```
> **Rule of 0**: In modern C++, prefer using smart pointers (`std::unique_ptr`, `std::vector`) so you don't have to write any of the 5 functions manually.

---

## Rapid-Fire C++ Interview Drill (Answer in 30 Seconds)
1. *What happens if a base class destructor is NOT virtual?* ➔ Deleting a derived object through a base pointer causes undefined behavior; the derived destructor is never invoked, leaking resources.
2. *How is dynamic dispatch implemented in C++?* ➔ Using a compiler-generated `vtable` of function pointers and an instance-level `vptr` that indexes into the table at runtime.
3. *How does C++ solve the Diamond Problem?* ➔ Through `virtual` inheritance (`class B : virtual public A`), ensuring only one shared subobject of `A` exists in `D`.
4. *How do you write a thread-safe Singleton in modern C++?* ➔ Using **Meyers' Singleton** with a static local variable inside `getInstance()`, which C++11 guarantees is initialized thread-safely.
5. *What is C++ Name Hiding?* ➔ A function in a derived class hides all functions with the same name in the base class, regardless of parameters; resolved using `using Base::func;`.