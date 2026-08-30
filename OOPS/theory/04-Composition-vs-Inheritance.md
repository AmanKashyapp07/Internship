# Composition vs. Inheritance in Object-Oriented Architecture

> **Scope:** Architectural Comparison of Composition ("Has-a") vs. Inheritance ("Is-a"), White-Box vs. Black-Box Code Reuse, The Fragile Base Class Problem, Dynamic Strategy Pattern via Smart Pointers, Binary ABI & `vtable` Coupling, Private Inheritance vs. Object Composition, and Memory Locality Analysis.

---

# Table of Contents
1. [Architectural Principles: Composition ("Has-a") vs. Inheritance ("Is-a")](#1-architectural-principles-composition-has-a-vs-inheritance-is-a)
2. [Hierarchical Inheritance Failure Modes: The Bird-Penguin Problem](#2-hierarchical-inheritance-failure-modes-the-bird-penguin-problem)
3. [Dynamic Strategy Pattern via Smart Pointers](#3-dynamic-strategy-pattern-via-smart-pointers)
4. [The Fragile Base Class Problem & ABI Stability](#4-the-fragile-base-class-problem--abi-stability)
5. [Private Inheritance vs. Object Composition in C++](#5-private-inheritance-vs-object-composition-in-c)
6. [Memory Layout & Performance Characteristics](#6-memory-layout--performance-characteristics)
7. [Core Theoretical Summary Principles](#7-core-theoretical-summary-principles)

---

# 1. Architectural Principles: Composition ("Has-a") vs. Inheritance ("Is-a")

```
+---------------------------------------------------------------------------------------------------+
| ATTRIBUTE            | INHERITANCE ("Is-a")                  | COMPOSITION ("Has-a")              |
+---------------------------------------------------------------------------------------------------+
| Coupling Paradigm    | White-Box Reuse (Derived class relies | Black-Box Reuse (Interacts strictly|
|                      | on internal base implementation details)| through public method interfaces)|
+---------------------------------------------------------------------------------------------------+
| Binding Lifetime     | Static (Fixed at compile time; cannot | Dynamic (Behaviors can be swapped  |
|                      | alter base class at runtime)          | at runtime via smart pointers)     |
+---------------------------------------------------------------------------------------------------+
| Encapsulation Impact | Weakens encapsulation (Base mutations | Preserves encapsulation (Contained |
|                      | propagate down derived hierarchy)     | objects isolate internal state)    |
+---------------------------------------------------------------------------------------------------+
| Polymorphic Contract | Enforces strict Liskov substitutability| Assembles orthogonal capabilities  |
+---------------------------------------------------------------------------------------------------+
```

---

# 2. Hierarchical Inheritance Failure Modes: The Bird-Penguin Problem

Rigid inheritance hierarchies often encode domain assumptions that break when specialized subtypes violate base behavioral contracts:

```cpp
// Anti-Pattern: Flawed Inheritance Contract
class Bird {
public:
    virtual ~Bird() = default;
    virtual void eat() { std::cout << "Consuming food...\n"; }
    virtual void fly() { std::cout << "Flying through air...\n"; }
};

class Sparrow : public Bird {
    // Satisfies Bird contract
};

class Penguin : public Bird {
public:
    void fly() override {
        // Violates Liskov Substitution Principle: Cannot fulfill base contract
        throw std::logic_error("Penguins cannot fly");
    }
};

void triggerFlight(Bird& b) {
    b.fly(); // Raises runtime exception if passed an instance of Penguin
}
```

---

# 3. Dynamic Strategy Pattern via Smart Pointers

Composition resolves behavioral divergence by encapsulating dynamic behaviors behind orthogonal interfaces:

```cpp
// 1. Orthogonal Behavior Interface
class IFlyBehavior {
public:
    virtual ~IFlyBehavior() = default;
    virtual void fly() const = 0;
};

// 2. Concrete Strategy Implementations
class FlyWithWings : public IFlyBehavior {
public:
    void fly() const override {
        std::cout << "Flying with wings\n";
    }
};

class NonFlying : public IFlyBehavior {
public:
    void fly() const override {
        std::cout << "Incapable of flight\n";
    }
};

// 3. Composed Domain Entity
class Bird {
private:
    std::unique_ptr<IFlyBehavior> flyBehavior; // Composition

public:
    explicit Bird(std::unique_ptr<IFlyBehavior> behavior)
        : flyBehavior(std::move(behavior)) {}

    void setFlyBehavior(std::unique_ptr<IFlyBehavior> behavior) {
        flyBehavior = std::move(behavior); // Dynamic runtime reconfiguration
    }

    void performFly() const {
        flyBehavior->fly(); // Delegated dispatch
    }
};
```

---

# 4. The Fragile Base Class Problem & ABI Stability

### Behavioral Coupling Breakdown:
A modification to the internal implementation of a base class can silently corrupt derived class invariants:

```
Version 1 Base Implementation:         Version 2 Base Optimization:
class Set {                            class Set {
public:                                public:
    virtual void add(int x) { ... }        virtual void add(int x) { ... }
    virtual void addAll(vector<int> v) {   virtual void addAll(vector<int> v) {
        for (int x : v) add(x);                // Directly inserts without calling add()
    }                                      }
};                                     };

Derived Subclass:
class CountingSet : public Set {
    int count = 0;
public:
    void add(int x) override { count++; Set::add(x); }
    void addAll(vector<int> v) override {
        count += v.size(); // Version 1: count is doubled! Version 2: count is accurate!
        Set::addAll(v);    // Derived correctness broke without derived code changes!
    }
};
```

### Application Binary Interface (ABI) Invalidation:
In C++, inserting or reordering virtual functions in a base class modifies the `vtable` slot indices of all subsequent virtual methods. Any dynamically loaded shared library compiled against the prior class header will invoke incorrect virtual memory offsets, resulting in memory corruption or segmentation faults.

---

# 5. Private Inheritance vs. Object Composition in C++

Both private inheritance and composition represent "has-a" relationships, but possess distinct access capabilities:

```
+---------------------------------------------------------------------------------------------------+
| CAPABILITY                           | PRIVATE INHERITANCE           | OBJECT COMPOSITION         |
+---------------------------------------------------------------------------------------------------+
| Access to Base Protected Members     | Permitted                     | Denied (Public API only)   |
| Override Base Virtual Functions      | Permitted                     | Denied                     |
| Empty Base Optimization (EBO)        | Supported (0-byte base size)  | Unsupported (Takes >= 1 B) |
| Runtime Polymorphic Swapping         | Unsupported (Fixed statically)| Supported (Smart pointers) |
+---------------------------------------------------------------------------------------------------+
```

### The Invariant:
Composition should be preferred unless the derived entity must override a protected virtual template method or leverage the Empty Base Optimization (EBO).

---

# 6. Memory Layout & Performance Characteristics

```
Flat Memory Layout (Concrete Value Composition):
+-------------------------------------------------------------------+
| Host Object: [ Field A ] [ Composed Object B: (x, y) ] [ Field C ] |
+-------------------------------------------------------------------+
-> Single contiguous memory allocation, zero pointer chasing, optimal hardware cache locality.

Heap Indirection (Interface Pointer Composition):
+-----------------------+              +------------------------------------+
| Host Object           |              | Dynamic Component Instance         |
| [ ptr ] ------------->| -----------> | [ vptr ] [ Component State ]       |
+-----------------------+              +------------------------------------+
-> Requires heap allocation, incurs one pointer indirection + dynamic virtual dispatch.
```

---

# 7. Core Theoretical Summary Principles

1. **Principle of Least Coupling:** Composition maintains strict encapsulation by exposing only public member interfaces, whereas inheritance exposes protected implementation details.
2. **Dynamic Behavioral Mutation:** Composition with interface pointers allows runtime behavior reassignment via strategy patterns.
3. **Fragile Base Class Mitigation:** Composed structures decouple component logic from parent execution pathways, eliminating side-effect cascading.
4. **Binary Compatibility (ABI):** Composing through opaque pointers or stable interfaces prevents `vtable` index shifts from breaking dependent compilation units.
5. **Private Inheritance Boundary:** Private inheritance should only be deployed when overriding internal virtual hooks or leveraging Empty Base Optimization.
