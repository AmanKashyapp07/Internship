# Master Guide 04: Composition vs. Inheritance in C++

> **Focus:** "Has-a vs Is-a", The Fragile Base Class Problem, Object Slicing Elimination, C++ Strategy Pattern via Smart Pointers, When Inheritance Fails, and When Inheritance is Actually Appropriate.
> 
> *Targeted for Top-Tier C++ Systems, HFT, and Backend Engineering Interviews.*

---

# Table of Contents
1. [The Golden Rule: Favor Composition Over Inheritance](#1-the-golden-rule-favor-composition-over-inheritance)
2. [The Classic Trap: The Bird-Penguin Inheritance Collapse](#2-the-classic-trap-the-bird-penguin-inheritance-collapse)
3. [The C++ Strategy Pattern Solution (Composition in Action)](#3-the-c-strategy-pattern-solution-composition-in-action)
4. [The Fragile Base Class Problem & Vtable Couplings](#4-the-fragile-base-class-problem--vtable-couplings)
5. [Decision Matrix: When to Pick Which](#5-decision-matrix-when-to-pick-which)
6. [High-Frequency C++ Interview Drill & Verbal Q&A](#6-high-frequency-c-interview-drill--verbal-qa)

---

# 1. The Golden Rule: Favor Composition Over Inheritance

```
+---------------------------------------------------------------------------------------------------+
| ATTRIBUTE            | INHERITANCE ("Is-a")                  | COMPOSITION ("Has-a")              |
+---------------------------------------------------------------------------------------------------+
| Coupling Level       | Tight / White-Box (Derived class sees | Loose / Black-Box (Interacts only  |
|                      | internal protected details of Base)   | via public exposed interfaces)     |
+---------------------------------------------------------------------------------------------------+
| Flexibility          | Static (Fixed at compile time, cannot | Dynamic (Can swap behavior plugins |
|                      | change parent class at runtime)       | at runtime via smart pointers)     |
+---------------------------------------------------------------------------------------------------+
| Encapsulation Impact | Weakens encapsulation (Base changes   | Preserves encapsulation (Objects   |
|                      | cascade and break derived invariants) | manage own isolated state)         |
+---------------------------------------------------------------------------------------------------+
| Code Reusability     | Monolithic, rigid taxonomy trees      | Granular, modular building blocks  |
+---------------------------------------------------------------------------------------------------+
```

- **One-Line Intuition:** Inheritance is being born with wings; composition is wearing a jetpack that you can equip, upgrade, or swap out at any time.
- **The Interview Trap:** Reaching for inheritance whenever two classes share code. Inheritance should only be used for **true polymorphic substitutability (LSP)**, never purely for convenient code reuse.
- **30-Second Verbal Answer:** **"Inheritance creates a tight compile-time coupling where subclasses depend on the internal implementation details of the base class, leading to the Fragile Base Class problem. Composition builds complex behavior by combining small, interchangeable objects behind interfaces, allowing dynamic runtime behavior changes and preserving encapsulation."**

---

# 2. The Classic Trap: The Bird-Penguin Inheritance Collapse

```cpp
// BAD DESIGN: Rigid Inheritance Hierarchy
class Bird {
public:
    virtual ~Bird() = default;
    virtual void eat() { std::cout << "Eating seeds...\n"; }
    virtual void fly() { std::cout << "Flapping wings in the sky...\n"; }
};

class Sparrow : public Bird {
    // Works fine: Sparrows eat and fly
};

class Penguin : public Bird {
public:
    void fly() override {
        // FATAL DESIGN FLAW: Penguins cannot fly!
        throw std::logic_error("Penguins cannot fly!"); // VIOLATES LISKOV SUBSTITUTION PRINCIPLE!
    }
};

void makeBirdFly(Bird& b) {
    b.fly(); // Crashes at runtime if passed a Penguin instance!
}
```

- **The Problem:** The real-world assumption that *"all birds can fly"* was encoded into the base class, forcing derived non-flying birds to break the base class contract.

---

# 3. The C++ Strategy Pattern Solution (Composition in Action)

```cpp
#include <iostream>
#include <memory>

// 1. Behavior Interface (Encapsulated Algorithm)
class IFlyBehavior {
public:
    virtual ~IFlyBehavior() = default;
    virtual void fly() const = 0;
};

// 2. Concrete Flying Strategies
class FlyWithWings : public IFlyBehavior {
public:
    void fly() const override { std::cout << "Soaring through the clouds!\n"; }
};

class NoFly : public IFlyBehavior {
public:
    void fly() const override { std::cout << "<< Cannot fly >>\n"; }
};

// 3. Client Class Using Composition
class Bird {
private:
    std::unique_ptr<IFlyBehavior> flyBehavior; // Composition: "Has-a" Flying Behavior

public:
    explicit Bird(std::unique_ptr<IFlyBehavior> fb) : flyBehavior(std::move(fb)) {}

    void setFlyBehavior(std::unique_ptr<IFlyBehavior> fb) {
        flyBehavior = std::move(fb); // Can dynamically swap behavior at runtime!
    }

    void performFly() const {
        flyBehavior->fly(); // Delegates execution to strategy component
    }
};

// Usage:
void testBirds() {
    Bird sparrow(std::make_unique<FlyWithWings>());
    sparrow.performFly(); // Prints: "Soaring through the clouds!"

    Bird penguin(std::make_unique<NoFly>());
    penguin.performFly(); // Prints: "<< Cannot fly >>" (Zero exceptions, 100% LSP compliant!)
}
```

---

# 4. The Fragile Base Class Problem & Vtable Couplings

```
Version 1 Base Class:                  Version 2 Base Class (Developer modifies Base):
class Set {                            class Set {
public:                                public:
    virtual void add(int x) { ... }        virtual void add(int x) { ... }
    virtual void addAll(vector<int> v) {   virtual void addAll(vector<int> v) {
        for (int x : v) add(x); // Calls add()  // Optimized: Inserts directly without calling add()
    }                                      }
};                                     };

Derived Class (CountingSet):
class CountingSet : public Set {
    int count = 0;
public:
    void add(int x) override { count++; Set::add(x); }
    void addAll(vector<int> v) override {
        count += v.size(); // In Version 1: count becomes 2x (double counted)!
        Set::addAll(v);    // In Version 2: count becomes 1x!
    }                      // Derived behavior broke without changing a single line of derived code!
};
```

### C++ Vtable Binary Incompatibility:
- In C++, adding a new virtual method to a base class shifts the **vtable offset indices** of all subsequent virtual methods.
- Any dynamically linked library (`.so` / `.dll`) or client code compiled against the old header will execute the **wrong function pointer**, resulting in silent memory corruption or crashes (ABI breakdown).

---

# 5. Decision Matrix: When to Pick Which

```
+---------------------------------------------------------------------------------------------------+
| USE COMPOSITION WHEN:                                 | USE INHERITANCE ONLY WHEN:                |
+---------------------------------------------------------------------------------------------------+
| * Behavior needs to change dynamically at runtime     | * Relationship is a strict, unchanging    |
|   (Strategy / State pattern).                         |   "is-a" taxonomy throughout the lifecycle|
+---------------------------------------------------------------------------------------------------+
| * You want to reuse functionality without exposing or | * The derived class can be substituted for|
|   inheriting all base class public methods.           |   the base class anywhere (LSP holds 100%)|
+---------------------------------------------------------------------------------------------------+
| * Combining multiple independent behaviors (e.g.      | * Implementing pure virtual interfaces    |
|   `Flyable`, `Quackable`, `Swimable`).                |   (`class Derived : public IInterface`).  |
+---------------------------------------------------------------------------------------------------+
| * Building cross-cutting concerns (Logging, Metrics,  | * Designing closed framework hierarchies  |
|   Caching decorators).                                |   (e.g. `std::exception` subclasses).     |
+---------------------------------------------------------------------------------------------------+
```

---

# 6. High-Frequency C++ Interview Drill & Verbal Q&A

### Q1: What is the Fragile Base Class problem in C++?
> **Answer:** An architectural flaw where seemingly safe modifications to a base class (such as altering internal method delegations or reordering virtual functions) **unintentionally break the correctness or binary ABI layout of derived subclasses**.

### Q2: How does composition eliminate Object Slicing in C++?
> **Answer:** Because composed objects are stored as **member variables (or smart pointers to interfaces)** inside the container class, they are copied and moved as complete, discrete entities rather than being truncated through base class pass-by-value assignments.

### Q3: What is the difference between Private Inheritance and Composition in C++?
> **Answer:** Both represent a "has-a" relationship, but **Private Inheritance grants the derived class access to the base's `protected` members and allows overriding its virtual methods**, whereas Composition interacts strictly through the contained object's public interface. Composition should always be preferred unless overriding a virtual hook is strictly required.

### Q4: How does Composition improve Unit Testing and Mocking?
> **Answer:** By composing dependencies via interfaces (`std::unique_ptr<IDependency>`), unit tests can effortlessly **inject mock implementations (stubs/fakes)** into the class constructor, isolating the class under test without relying on complex inheritance trees.

### Q5: What is the performance difference between Composition and Inheritance in C++?
> **Answer:** Composition of concrete value objects (`Room room;`) has **zero overhead** and optimal cache locality (flat memory layout). Composition via interface pointers (`std::unique_ptr<IBehavior>`) incurs **one pointer indirection and a virtual dispatch**, identical to the runtime cost of virtual inheritance.
