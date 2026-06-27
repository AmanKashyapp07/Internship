# OOP + SOLID — Master Revision Cheat-Sheet

> **One file to rule them all.** Quick-reference for top IT company interviews.  
> C++ focused | Interview Q&A | Code templates | Trap catalogue

---

## Table of Contents

| Lecture | Topic |
|---------|-------|
| [L1](#l1--oop-fundamentals) | OOP Fundamentals — Class, Object, Memory, Access, `this`, `static` |
| [L2](#l2--encapsulation--abstraction) | Encapsulation & Abstraction — Getters/Setters, Abstract Class, Interface |
| [L3](#l3--inheritance--polymorphism) | Inheritance & Polymorphism — vtable, Diamond, Slicing |
| [L4](#l4--constructors--destructors) | Constructors & Destructors — Rule of 3/5/0, Virtual Dtor, RAII |
| [L5](#l5--solid-principles) | SOLID Principles — SRP, OCP, LSP, ISP, DIP |
| [L6](#l6--smart-pointers--modern-c) | Smart Pointers — unique_ptr, shared_ptr, weak_ptr, Alignment |

---

## L1 — OOP Fundamentals

```
Class  = Blueprint (no memory consumed)
Object = Instance (memory allocated at runtime)

Stack: fast, auto-managed, LIFO, small (~8 MB default)
Heap:  manual/smart-ptr, large, fragmented

Access Modifiers:
  private   → class only
  protected → class + derived classes
  public    → everywhere

class  → private by default
struct → public by default

this  → implicit pointer to calling object (not in static fns)
static → shared across all instances; no `this`
```

### Key Code Templates
```cpp
// Heap allocation (prefer smart pointer over raw new)
auto obj = make_unique<MyClass>(args);

// Static member
class Counter {
    static int count;
public:
    static int getCount() { return count; }
};
int Counter::count = 0;  // define outside class

// Method chaining via this
MyClass& setX(int v) { x = v; return *this; }
```

### Must-Know Q&A (L1)
| Question | Answer |
|----------|--------|
| Class vs Object | Blueprint vs instance |
| `class` vs `struct` C++ | Default access: private vs public |
| What is `this`? | Implicit pointer to calling object |
| Static fn and `this`? | Static has no `this` — no instance context |
| Memory leak? | Heap object not freed → OS can't reclaim |

---

## L2 — Encapsulation & Abstraction

```
Encapsulation = Bundle data + behaviour; restrict direct access (private + getters/setters)
Abstraction   = Hide HOW; expose WHAT (pure virtual functions)

Encapsulation hides DATA
Abstraction   hides IMPLEMENTATION
```

### Key Code Templates
```cpp
// Encapsulation — validated setter
class BankAccount {
    double balance;
public:
    double getBalance() const { return balance; }   // const getter
    void deposit(double a) {
        if (a <= 0) throw invalid_argument("...");
        balance += a;
    }
};

// Abstraction — abstract class / interface
class Shape {
public:
    virtual double area()      = 0;   // pure virtual
    virtual double perimeter() = 0;
    virtual ~Shape() {}               // ALWAYS virtual dtor in base!
};

// Interface (all pure virtual, no data)
class ISerializable {
public:
    virtual string serialize()              = 0;
    virtual void   deserialize(string data) = 0;
    virtual ~ISerializable() = default;
};
```

### Must-Know Q&A (L2)
| Question | Answer |
|----------|--------|
| Encapsulation vs Abstraction | Encap = hide data; Abstraction = hide impl |
| Pure virtual function syntax | `virtual void f() = 0;` |
| Can abstract class have ctor? | Yes — called by derived |
| Can abstract class be instantiated? | NO — compile error |
| Abstract class vs Interface | Abstract: partial impl + data; Interface: pure contract, no state |

---

## L3 — Inheritance & Polymorphism

```
Inheritance types:
  Single       A → B
  Multilevel   A → B → C
  Multiple     A + B → C
  Hierarchical A → B, A → C
  Hybrid       Combination (can cause Diamond)

Constructor order: Base → Derived (top-down)
Destructor order:  Derived → Base (bottom-up)

Polymorphism:
  Compile-time: function overloading, operator overloading, templates
  Runtime:      virtual functions (vtable/vptr dispatch)
```

### vtable & vptr
```
Each class with virtual fns → has a vtable (array of fn pointers)
Each object → has a hidden vptr pointing to its class's vtable

shape->draw()  compiles to:  shape->vptr[draw_index]()
                              ↑ runtime decision = dynamic dispatch

Cost: 8 bytes vptr per object + one indirection per virtual call
```

### Diamond Problem Fix
```cpp
class Animal { public: int age; };
class Dog : virtual public Animal { };  // virtual inheritance
class Cat : virtual public Animal { };
class DogCat : public Dog, public Cat {
    // Only ONE Animal subobject — no ambiguity
};
```

### Slicing Trap
```cpp
Dog d;
Animal a = d;       // SLICING — Dog data lost, no virtual dispatch
Animal& r = d;      // OK — reference, no slicing
Animal* p = &d;     // OK — pointer, no slicing
// Rule: Polymorphism only works via POINTER or REFERENCE
```

### Must-Know Q&A (L3)
| Question | Answer |
|----------|--------|
| vtable created when? | Compile time (per class); vptr set in ctor |
| Override vs hiding | Override: same sig + virtual; Hiding: non-virtual or diff sig |
| `override` keyword | Compile-time check that you're overriding a virtual fn |
| `final` keyword | Blocks further override (fn) or derivation (class) |
| Diamond problem solution | `virtual public` inheritance → shared base subobject |
| Slicing | Copying derived to base VALUE loses derived part; use ptr/ref |

---

## L4 — Constructors & Destructors

```
Constructor types:
  Default       → MyClass()
  Parameterised → MyClass(int x)
  Copy          → MyClass(const MyClass& other)
  Move          → MyClass(MyClass&& other) noexcept
  Delegating    → MyClass() : MyClass(0, 0) {}

Initialiser list order = DECLARATION order (not list order)

Rule of Three: define dtor → define copy-ctor + copy-assign
Rule of Five:  + move-ctor + move-assign (C++11)
Rule of Zero:  use RAII types; compiler generates all correctly
```

### Key Code Templates
```cpp
// Virtual destructor (MANDATORY in base classes with virtual fns)
class Base {
public:
    virtual ~Base() = default;
};

// Deep copy pattern
MyClass(const MyClass& o) : data(new int[o.size]), size(o.size) {
    copy(o.data, o.data + size, data);
}
MyClass& operator=(const MyClass& o) {
    if (this == &o) return *this;   // self-assignment check
    delete[] data;
    size = o.size;
    data = new int[size];
    copy(o.data, o.data + size, data);
    return *this;
}

// Move constructor
MyClass(MyClass&& o) noexcept : data(o.data), size(o.size) {
    o.data = nullptr; o.size = 0;   // leave moved-from in valid state
}

// explicit: block implicit conversions
explicit MyClass(int x) { ... }
```

### Must-Know Q&A (L4)
| Question | Answer |
|----------|--------|
| Initialiser list order | Declaration order, not list order! |
| Why virtual destructor | Without it: `delete basePtr` → partial destruction (UB/leak) |
| Shallow vs deep copy | Shallow: copies pointer (double-free!); Deep: copies data |
| `explicit` keyword | Prevents implicit single-arg constructor conversions |
| `mutable` keyword | Allows modification in `const` methods (logical const-ness) |
| `friend` function | Non-member with private access; weakens encapsulation |
| RVO | Compiler constructs returned value in-place; no copy/move (mandatory C++17) |
| `std::move` | Cast to rvalue ref; enables move (doesn't move by itself) |

---

## L5 — SOLID Principles

```
S — Single Responsibility   : One class = one reason to change
O — Open/Closed             : Open for extension, closed for modification
L — Liskov Substitution     : Derived must honour base class behavioural contract
I — Interface Segregation   : Small focused interfaces > one fat interface
D — Dependency Inversion    : Depend on abstractions, not concretions; inject deps
```

### SOLID at a Glance

| Principle | Violation Red Flag | Fix |
|-----------|-------------------|-----|
| SRP | God class / huge method doing 5 things | Split into focused classes |
| OCP | `if/else if/switch` on type | Polymorphism / Strategy pattern |
| LSP | Derived throws unexpected exception or ignores method | Redesign hierarchy or split interface |
| ISP | Class implements empty/stub methods | Split fat interface |
| DIP | `new ConcreteClass()` inside high-level service | Inject via constructor/setter |

### Key Code Templates

```cpp
// SRP: separate responsibilities
class OrderValidator { void validate(Order&); };
class OrderRepository { void save(Order&); };
class NotificationService { void notify(Customer&); };

// OCP: extend via new class
class PaymentStrategy { virtual void pay(Payment&) = 0; };
class CreditCard : public PaymentStrategy { void pay(Payment&) override {...} };
class Crypto      : public PaymentStrategy { void pay(Payment&) override {...} };

// LSP: don't override in a way that breaks base contract
// Bad: Square::setWidth breaks Rectangle client assumptions
// Fix: Square and Rectangle both inherit from Shape, not from each other

// ISP: lean interfaces
class IWorkable   { virtual void work() = 0; };
class IEatable    { virtual void eat()  = 0; };
// Robot: only IWorkable; Human: IWorkable + IEatable

// DIP: inject abstraction
class IDatabase { virtual void save(Order&) = 0; };
class OrderService {
    IDatabase& db;
public:
    OrderService(IDatabase& db) : db(db) {}  // constructor injection
};
```

### Must-Know Q&A (L5)
| Question | Answer |
|----------|--------|
| LSP classic violation | Square IS-A Rectangle geometrically but breaks setWidth/setHeight contract |
| DI vs DIP | DIP = principle (depend on abstraction); DI = technique (pass dependency in) |
| Which pattern implements OCP? | Strategy, Observer, Decorator |
| Which pattern implements DIP? | Factory, Strategy (inject via interface) |
| ISP benefit for testing | Mock only the needed interface, not the entire fat interface |

---

## L6 — Smart Pointers & Modern C++

```
unique_ptr:  Exclusive ownership. Move-only. Zero overhead. Use by default.
shared_ptr:  Shared via ref count (atomic). Control block. make_shared = 1 alloc.
weak_ptr:    Non-owning. Doesn't inc ref count. lock() to access. Breaks cycles.

Raw pointer: only for non-owning observation or C API interop.
```

### Key Code Templates
```cpp
// unique_ptr — preferred for single owner
auto obj = make_unique<MyClass>(args);
auto obj2 = move(obj);   // transfer ownership; obj is now nullptr

// shared_ptr — shared ownership
auto sp1 = make_shared<Resource>();
auto sp2 = sp1;                 // ref count = 2
cout << sp1.use_count();        // 2

// weak_ptr — break circular reference
weak_ptr<Node> wp = sp1;
if (auto locked = wp.lock()) {  // safe access
    locked->use();
}
cout << wp.expired();           // true if object destroyed

// Custom deleter
auto f = unique_ptr<FILE, decltype(&fclose)>(fopen("x.txt","r"), fclose);

// enable_shared_from_this — get shared_ptr from inside class
class Foo : public enable_shared_from_this<Foo> {
public:
    shared_ptr<Foo> self() { return shared_from_this(); }
};
```

### Alignment & Sizing
```cpp
// Order members largest → smallest to minimise padding
struct Good { double d; int i; char c; };   // 16 bytes
struct Bad  { char c; int i; double d; };   // 24 bytes

sizeof(EmptyClass) == 1   // guaranteed unique address
// Empty Base Optimisation: empty base class takes 0 bytes when inherited
```

### Must-Know Q&A (L6)
| Question | Answer |
|----------|--------|
| unique_ptr vs shared_ptr | unique: exclusive, zero-overhead; shared: ref-counted, control block |
| Circular shared_ptr leak fix | Use weak_ptr for back-pointers |
| make_shared benefit | Single allocation for object + control block |
| RAII definition | Resource tied to object lifetime; ctor acquires, dtor releases |
| sizeof(EmptyClass) | 1 byte — every object needs unique address |
| Dangling pointer | Pointer to freed memory; accessing = UB |
| shared_ptr overhead | Control block + atomic ref count ops |

---

## Interview Trap Catalogue

```
❌ Forgetting virtual destructor in base class
   → delete basePtr → Derived dtor not called → leak/UB

❌ Initialiser list order ≠ written order (= declaration order)
   → int b; int a; with `: a(x), b(a)` → b init'd before a → garbage

❌ Object slicing (copy derived to base value)
   → Use pointer/reference for polymorphism, never value copy

❌ Circular shared_ptr
   → A holds B, B holds A → neither destroyed → leak
   → Fix: one side uses weak_ptr

❌ Creating shared_ptr from same raw pointer twice
   → Two independent control blocks → double-free crash

❌ Using moved-from object
   → After move(), object is in valid but unspecified state
   → Don't dereference; reassign before reuse

❌ ISP/SRP fat class
   → One class + one interface doing 5 jobs → testing nightmare

❌ DIP violation: `new Concrete()` inside a service
   → Hardcodes dependency; impossible to swap implementation in tests

❌ OCP violation: if/else chain on type
   → Every new type requires modifying core logic → fragile
```

---

## Quick One-Liner Definitions (Interview Flash Cards)

| Term | One Line |
|------|----------|
| Class | Blueprint for objects |
| Object | Runtime instance of a class |
| Encapsulation | Bundle + hide data; control access |
| Abstraction | Expose WHAT, hide HOW |
| Inheritance | Derive new class from existing (IS-A) |
| Polymorphism | One interface, many implementations |
| Virtual function | Runtime dispatch via vtable |
| vtable | Per-class array of fn pointers |
| vptr | Per-object hidden pointer to vtable |
| Diamond problem | Duplicate base subobject in multiple inheritance |
| Virtual inheritance | Shared single base subobject |
| Slicing | Copying derived to base value loses derived data |
| Rule of Zero | Use RAII types; let compiler generate everything |
| RAII | Resource tied to scope; ctor acquires, dtor releases |
| unique_ptr | Exclusive ownership, zero overhead |
| shared_ptr | Shared ownership via atomic ref count |
| weak_ptr | Non-owning observer; breaks cycles |
| SRP | One class = one reason to change |
| OCP | Extend by adding code, not modifying |
| LSP | Derived must honour base behavioural contract |
| ISP | Prefer small focused interfaces |
| DIP | Depend on abstractions; inject concrete via ctor |
| Dependency Injection | Pass dependencies from outside; don't `new` inside |

---

## Revision Order Suggestion

```
Day 1: L1 (Fundamentals) + L2 (Encapsulation/Abstraction)
Day 2: L3 (Inheritance/Polymorphism) — focus on vtable, slicing, diamond
Day 3: L4 (Constructors) — Rule of 3/5/0, virtual dtor, shallow/deep copy
Day 4: L5 (SOLID) — one violation + fix per principle
Day 5: L6 (Smart Ptrs) — unique vs shared vs weak + circular ref + alignment
Day 6: Full mock — answer every Q in this cheat-sheet from memory
```

---

*Lectures: [L1](file:///Users/amankashyap/Documents/cses/oops_solid/l1.md) | [L2](file:///Users/amankashyap/Documents/cses/oops_solid/l2.md) | [L3](file:///Users/amankashyap/Documents/cses/oops_solid/l3.md) | [L4](file:///Users/amankashyap/Documents/cses/oops_solid/l4.md) | [L5](file:///Users/amankashyap/Documents/cses/oops_solid/l5.md) | [L6](file:///Users/amankashyap/Documents/cses/oops_solid/l6.md)*
