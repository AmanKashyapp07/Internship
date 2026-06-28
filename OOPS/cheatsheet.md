# OOP Interview Cheatsheet 🚀

A comprehensive, production-grade reference for Object-Oriented Programming concepts tested in top-tier tech company interviews (Google, Meta, Amazon, Microsoft, Uber, Stripe). Covers core pillars, polymorphism internals, memory management, inheritance mechanics, and SOLID principles. Design Patterns and LLD coding problems are intentionally excluded.

---

## 1. The Four Pillars — Quick Reference ⚙️

| Pillar | One-Line Definition | Mechanism | Real Analogy |
| :--- | :--- | :--- | :--- |
| **Encapsulation** | Bundle data + methods; restrict direct access | `private` fields + `public` getters/setters | Medical capsule — medicine is inside; you can't touch it directly |
| **Abstraction** | Expose *what*, hide *how* | Abstract classes, Interfaces | Car dashboard — you use pedals, not pistons |
| **Inheritance** | Child class acquires properties of parent | `extends` / `:` keyword | Child inherits traits from parents |
| **Polymorphism** | Same name, multiple behaviors | Overloading (compile-time) + Overriding (run-time) | "Run" means different things for a person, water, and a program |

### ⚠️ Key Interview Distinction: Abstraction vs Encapsulation
* **Encapsulation** = *packaging* + *protecting* data (implementation technique)
* **Abstraction** = *hiding complexity*, showing only the essential interface (design concept)
* Encapsulation *implements* abstraction. You can encapsulate without abstracting (all-public class), but you use encapsulation to achieve abstraction.

---

## 2. Access Modifiers 🔐

| Modifier | Same Class | Subclass (same pkg) | Subclass (diff pkg) | World |
| :--- | :---: | :---: | :---: | :---: |
| `private` | ✅ | ❌ | ❌ | ❌ |
| `protected` | ✅ | ✅ | ✅ | ❌ |
| `public` | ✅ | ✅ | ✅ | ✅ |
| (default/package) | ✅ | ✅ | ❌ | ❌ |

**C++ inheritance access matrix** — when inheriting `class B : <access> A`:

| Member in A | `public` inherit | `protected` inherit | `private` inherit |
| :--- | :--- | :--- | :--- |
| `public` member | → `public` in B | → `protected` in B | → `private` in B |
| `protected` member | → `protected` in B | → `protected` in B | → `private` in B |
| `private` member | ❌ inaccessible | ❌ inaccessible | ❌ inaccessible |

---

## 3. Constructors & Destructors 🏗️

### Constructor Types

| Type | Signature | Purpose |
| :--- | :--- | :--- |
| **Default** | `Car()` | Initializes fields to defaults |
| **Parameterized** | `Car(string model, int year)` | Initializes fields with given values |
| **Copy** | `Car(const Car& src)` | Constructs by copying another object |
| **Move** (C++) | `Car(Car&& src) noexcept` | Steals resources from a temporary |

### ⚠️ Copy Constructor — Must Pass by Reference
Passing by value would require a copy, which calls the copy constructor again → **infinite recursion → stack overflow**.
```cpp
Car(const Car& src);   // ✅ Correct — pass by const reference
Car(Car src);          // ❌ Wrong  — infinite recursion
```

### Shallow Copy vs Deep Copy

```
Object A (original)          Object B (shallow copy)
+----------+                 +----------+
|  _data   | ──┐             |  _data   | ──┐
+----------+   │             +----------+   │
               ▼                           │
         [Heap Memory]  ◄──────────────────┘
          "Hello"
  ← Both point to the SAME heap block! Dangerous.
```

```cpp
// Shallow Copy (compiler default — dangerous if pointers exist)
this->data = src.data;          // copies the pointer address

// Deep Copy (explicit — safe, allocates new memory)
this->data = new char[strlen(src.data) + 1];
strcpy(this->data, src.data);
```

| Scenario | Use Shallow | Use Deep |
| :--- | :--- | :--- |
| Class has no pointer/resource members | ✅ Safe | Unnecessary |
| Class owns heap-allocated data | ❌ Dangling pointer risk | ✅ Required |
| Class manages file handles, sockets | ❌ Double-close risk | ✅ Required |

### Virtual Destructor (C++) — Critical Gotcha
```cpp
class Base {
public:
    virtual ~Base() { }  // ✅ MUST be virtual
};
class Derived : public Base { ... };

Base* ptr = new Derived();
delete ptr;  // Without virtual ~Base(), only Base::~Base() runs → MEMORY LEAK
```
> **Rule:** If a class has any virtual function, its destructor must also be `virtual`.

---

## 4. Polymorphism — Compile-Time vs Run-Time 🎭

### Compile-Time (Static) Polymorphism

**Method Overloading** — same name, different parameter signatures.
```cpp
int   add(int a, int b);          // signature 1
float add(float a, float b);      // signature 2
int   add(int a, int b, int c);   // signature 3
```
> ❌ You **cannot** overload by return type alone — the compiler resolves calls based on argument lists, not return types.

**Operator Overloading** — redefine operators for user-defined types.
```cpp
Vector operator+(const Vector& other) {
    return Vector(x + other.x, y + other.y);
}
// Enables: Vector v3 = v1 + v2;
```

### Run-Time (Dynamic) Polymorphism

**Method Overriding** — child class replaces parent's virtual function.

| Rule | Detail |
| :--- | :--- |
| Same name, return type, parameters | Must be identical signature |
| Parent method must be `virtual` | Otherwise child method **hides** it (not overrides) |
| Access cannot be narrowed | `public` parent → cannot be `private` in child |
| Use `override` keyword (C++11) | Compiler error if no matching virtual found — catches typos |

```cpp
class Animal {
public:
    virtual void speak() { cout << "..."; }
};
class Dog : public Animal {
public:
    void speak() override { cout << "Woof"; }  // ✅ override keyword
};

Animal* a = new Dog();
a->speak();  // Prints "Woof" — runtime dispatch via VTABLE
```

### Overloading vs Overriding — Quick Comparison

| Feature | Overloading | Overriding |
| :--- | :--- | :--- |
| **Binding time** | Compile-time | Run-time |
| **Scope** | Same class (or same scope) | Parent → Child class |
| **Signature** | Must differ | Must be identical |
| **Inheritance needed?** | ❌ No | ✅ Yes |
| **`virtual` keyword needed?** | ❌ No | ✅ Yes (in parent) |

---

## 5. Virtual Functions — VTABLE & VPTR Internals 🔬

### How the Compiler Implements Runtime Polymorphism

1. **VTABLE (Virtual Function Table):** For every class with at least one virtual function, the compiler generates a static table of function pointers — one per virtual method.
2. **VPTR (Virtual Pointer):** When an object is instantiated, the compiler silently inserts a hidden pointer member (`__vptr`) pointing to its class's VTABLE.

```
Object Memory Layout (Derived)
┌─────────────┐
│  __vptr     │──────► VTABLE (Derived)
├─────────────┤         ┌──────────────────────┐
│  data1      │         │ [0] &Derived::speak() │
│  data2      │         │ [1] &Derived::draw()  │
└─────────────┘         └──────────────────────┘
```

**Resolution flow:**
```
ptr->speak()
  1. Dereference ptr → get object address
  2. Read __vptr → get VTABLE address
  3. Look up index [0] → get function pointer &Derived::speak()
  4. Call that address
```

### Pure Virtual Functions & Abstract Classes

```cpp
class Shape {
public:
    virtual double area() = 0;   // Pure virtual — no implementation
    virtual ~Shape() {}
};
// Shape s; ❌ Cannot instantiate — abstract class

class Circle : public Shape {
public:
    double area() override { return 3.14 * r * r; }
};
```

| Concept | Rule |
| :--- | :--- |
| **Abstract class** | Has ≥ 1 pure virtual function; cannot be instantiated |
| **Concrete class** | Implements ALL inherited pure virtual functions |
| **Abstract class constructor** | ✅ Can exist — called when child is constructed |
| **Virtual constructor** | ❌ Impossible — VPTR not set up yet during construction |

### ⚠️ Never Call Virtual Functions in Constructors/Destructors (C++)

During `Base::Base()`, the VPTR points to `Base`'s VTABLE — not the derived class. Virtual dispatch is bypassed; it always calls the Base version.

```cpp
class Base {
public:
    Base() { init(); }           // Called during construction
    virtual void init() { cout << "Base::init"; }
};
class Derived : public Base {
public:
    void init() override { cout << "Derived::init"; }
};

Derived d;  // Prints "Base::init" — NOT "Derived::init"! 
```

---

## 6. Abstract Class vs Interface — Comparison 🏛️

| Feature | Abstract Class | Interface |
| :--- | :--- | :--- |
| **Instantiation** | ❌ Cannot instantiate | ❌ Cannot instantiate |
| **Multiple inheritance** | ❌ Single (most languages) | ✅ Multiple interfaces allowed |
| **State (fields)** | ✅ Can have instance variables | ❌ No instance state (Java: `static final` only) |
| **Constructors** | ✅ Can have constructors | ❌ Cannot have constructors |
| **Concrete methods** | ✅ Can have complete implementations | ⚠️ Java 8+ allows `default` methods |
| **Primary purpose** | Partial implementation + IS-A contract | Pure behavioral contract (CAN-DO) |

**When to use which:**
* Use **Abstract Class** when subclasses share code and represent a true IS-A relationship.
* Use **Interface** when unrelated classes should share a behavioral contract (e.g., `Serializable`, `Comparable`).

---

## 7. Inheritance Types & The Diamond Problem 💎

### Inheritance Modes (C++)

```
Single:      A ← B
Multi-level: A ← B ← C
Multiple:    A, B ← C
Hierarchical: A ← B, A ← C
Hybrid:      combination of above
```

### The Diamond Problem

```
        A
       / \
      B   C
       \ /
        D      ← D has TWO copies of A's members: one via B, one via C
```

```cpp
class A { public: int x = 10; };
class B : public A { };
class C : public A { };
class D : public B, public C { };

D d;
d.x;          // ❌ Ambiguous: B::A::x or C::A::x?
d.B::x;       // ✅ Disambiguate manually
```

**Resolution — Virtual Inheritance:**
```cpp
class B : virtual public A { };   // Only ONE shared copy of A in D
class C : virtual public A { };
class D : public B, public C { }; // Single A via VBPTR offset

D d;
d.x;  // ✅ No ambiguity — single shared A
```

### Association vs Aggregation vs Composition

```
Association  : A uses B (loose, both independent)
Aggregation  : A HAS-A B (B can exist without A)  — weak ownership
Composition  : A HAS-A B (B CANNOT exist without A) — strong ownership, A controls B's lifecycle
```

| Relationship | Example | B survives without A? |
| :--- | :--- | :---: |
| Association | Doctor & Patient | ✅ Yes |
| Aggregation | Department & Teacher | ✅ Yes |
| Composition | House & Room | ❌ No |

---

## 8. Memory Management & Smart Pointers (C++) 🧠

### Stack vs Heap Allocation

| Property | Stack | Heap |
| :--- | :--- | :--- |
| **Allocation** | Automatic (LIFO) | Manual (`new` / `malloc`) |
| **Deallocation** | Automatic on scope exit | Manual (`delete` / `free`) — or GC |
| **Speed** | Very fast | Slower (fragmentation, allocation logic) |
| **Size limit** | Small (1–8 MB typical) | Limited by RAM |
| **Risk** | Stack overflow on deep recursion | Memory leaks, dangling pointers |

### RAII (Resource Acquisition Is Initialization)
Bind resource lifecycle to a stack object's constructor/destructor. When the object goes out of scope, the destructor automatically releases the resource — even during exceptions.

```cpp
class FileHandle {
    FILE* f;
public:
    FileHandle(const char* path) { f = fopen(path, "r"); }
    ~FileHandle() { if (f) fclose(f); }  // Automatic cleanup — no leaks
};
```

### Smart Pointers (C++11+)

| Smart Pointer | Ownership | Copy | Move | Use When |
| :--- | :--- | :--- | :--- | :--- |
| `unique_ptr<T>` | Exclusive (one owner) | ❌ | ✅ | Single clear owner of a heap resource |
| `shared_ptr<T>` | Shared (ref-counted) | ✅ (increments count) | ✅ | Multiple owners; resource freed when count → 0 |
| `weak_ptr<T>` | Non-owning observer | ✅ (no count change) | ✅ | Break cyclic `shared_ptr` references |

**Cyclic Reference Memory Leak — fixed with `weak_ptr`:**
```cpp
// ❌ Leak: A holds shared_ptr to B, B holds shared_ptr to A → counts never hit 0
struct A { shared_ptr<B> b_ref; };
struct B { shared_ptr<A> a_ref; };

// ✅ Fix: break the cycle
struct B { weak_ptr<A> a_ref; };  // weak_ptr doesn't hold ownership
```

---

## 9. SOLID Principles — Deep Reference 📐

### S — Single Responsibility Principle
> A class should have **one, and only one, reason to change**.

**Violation (code smell):** A class that handles business logic, database queries, AND UI rendering simultaneously.

```
❌ Employee class with:  calculatePay() + saveToDb() + renderHtml()
✅ Split into:           PayCalculator | EmployeeRepo | EmployeeView
```

**Related metrics:** SRP achieved → **high cohesion** (all members aligned to one goal) + **low coupling** (minimal interdependencies).

---

### O — Open-Closed Principle
> Software entities should be **open for extension, but closed for modification**.

**Code smell that signals violation:** `if/else` or `switch` chains checking object types — adding a new type forces editing existing code.

```cpp
// ❌ Violation
if (shape == "circle") drawCircle();
else if (shape == "square") drawSquare();
// Adding "triangle" requires editing this block.

// ✅ OCP compliant
class Shape { virtual void draw() = 0; };
class Circle  : public Shape { void draw() override { ... } };
class Triangle: public Shape { void draw() override { ... } };
// Adding Triangle never touches existing code.
```

---

### L — Liskov Substitution Principle
> Subtypes must be **substitutable for their base types** without altering program correctness.

**Classic Violation — Square inheriting Rectangle:**
```cpp
Rectangle r;
r.setWidth(5); r.setHeight(10);
assert(r.area() == 50);  // ✅ passes

// Square overrides setWidth to also update height (to keep it square)
Square s;
s.setWidth(5); s.setHeight(10);
assert(s.area() == 50);  // ❌ fails — area is 100 (10×10 because setHeight updated width too)
```

**LSP Constraints:**
| Constraint | Rule |
| :--- | :--- |
| **Preconditions** | Subclass cannot *strengthen* input requirements |
| **Postconditions** | Subclass cannot *weaken* output guarantees |
| **Invariants** | Subclass must preserve parent's internal state rules |
| **Return types** | Subclass may return a *more specific subtype* (Covariant return) |

---

### I — Interface Segregation Principle
> Clients should **not be forced to depend on methods they do not use**.

```
❌ Fat interface:
IMultiDevice { print(), scan(), fax(), staple() }
SimplePrinter must implement scan(), fax(), staple() as stubs — it doesn't even have those features!

✅ Segregated interfaces:
IPrinter { print() }
IScanner  { scan()  }
IFax      { fax()   }
SimplePrinter implements only IPrinter.
```

**Hidden benefit:** Splitting interfaces in C++ reduces header dependencies → fewer recompilation cascades when one interface changes.

---

### D — Dependency Inversion Principle
> High-level modules should **not depend on low-level modules**. Both should depend on abstractions.

```
❌ Tightly coupled (DIP violation):
PaymentService directly instantiates StripeAPI inside its constructor.
  → Cannot unit-test without hitting real Stripe.
  → Swapping to Razorpay requires rewriting PaymentService.

✅ DIP compliant (via Dependency Injection):
PaymentService(IPaymentGateway* gateway)  // depends on abstraction
  → Inject StripeGateway in production
  → Inject MockGateway in unit tests
```

**DIP vs DI vs IoC:**

| Term | What it is |
| :--- | :--- |
| **DIP** | Design Principle — depend on abstractions |
| **DI (Dependency Injection)** | Implementation technique — inject via constructor/setter |
| **IoC (Inversion of Control)** | Architectural pattern — framework controls lifecycle (e.g., Spring) |

---

### SOLID Violations — Code Smell Quick Reference

| Code Smell | Violated Principle |
| :--- | :--- |
| Class has methods from 3 different business domains | SRP |
| Adding a new feature requires editing existing tested classes | OCP |
| Subclass throws `UnsupportedOperationException` on an inherited method | LSP |
| Mock object must stub 20 methods when only 1 is used | ISP |
| Class instantiates its own DB/network dependencies inside constructors | DIP |
| High test-to-mock ratio (hard to unit-test a class in isolation) | DIP |
| Long `if/else instanceof` chains | OCP + LSP |

---

## 10. Critical OOP Gotchas & Traps ⚠️

### 1. Object Slicing (C++)
Assigning a derived object to a base **by value** strips away derived members and the VTABLE linkage.
```cpp
Derived d;
Base b = d;       // ❌ Sliced — b has no Derived data, no virtual dispatch
Base& ref = d;    // ✅ Safe — reference preserves virtual dispatch
Base* ptr = &d;   // ✅ Safe — pointer preserves virtual dispatch
```

### 2. Method Hiding vs Method Overriding (C++)
If the parent method is **not virtual**, the child method **hides** it (compile-time resolution), not overrides it (runtime resolution).
```cpp
class Base  { public: void show() { cout << "Base"; }   };
class Derived: public Base { public: void show() { cout << "Derived"; } };

Base* ptr = new Derived();
ptr->show();  // Prints "Base" — NOT Derived! show() is not virtual → hiding, not overriding
```

### 3. Diamond Problem with Non-Virtual Inheritance
Without `virtual` keyword in intermediate classes, derived class gets duplicate copies of the base — causing ambiguous member access.

### 4. Calling `delete this` — Conditions
`delete this` is legal only if:
* Object was heap-allocated via `new`.
* No members are accessed after the call.
* Object will not be accessed anywhere else.

### 5. `static` Members Don't Have `this` Pointer
Static member functions cannot access non-static members because they have no object instance (`this` is absent).

### 6. Java Virtual Calls in Constructors Are Dangerous
In Java, virtual calls in parent constructors resolve to the **child's override** — but the child fields haven't been initialized yet (child constructor body hasn't run) → potential `NullPointerException`.

### 7. Aggregations Ignore NULLs Parallel — `COUNT(col)` vs `COUNT(*)`
*(Applies to Java streams too)* — Null elements in collections can cause `NullPointerException` in operations unless filtered explicitly.

---

## 11. Key OOP Comparisons Summary Table 📋

| Question | Answer |
| :--- | :--- |
| Abstract class vs Interface | Abstract: partial impl + state; Interface: pure contract, no state |
| Overloading vs Overriding | Overload = same class, diff params, compile-time; Override = child replaces parent, runtime |
| Composition vs Inheritance | Composition = HAS-A, loose coupling; Inheritance = IS-A, tight coupling |
| Early vs Late Binding | Early = resolved at compile time (static); Late = resolved at runtime (virtual) |
| `this` vs `super` | `this` = current instance; `super` = parent class reference |
| Shallow vs Deep Copy | Shallow = copies pointer; Deep = copies pointed data |
| `final` (Java) vs `const` (C++) | `final` = cannot reassign reference; `const` = deep immutability, no non-const methods |
| IS-A vs HAS-A | IS-A = Inheritance (Dog IS-A Animal); HAS-A = Composition (Car HAS-A Engine) |
| Virtual vs Pure Virtual | Virtual has a body (optional override); Pure Virtual has no body (mandatory override) |

---

## 12. Object Lifecycle — Construction & Destruction Order 🔄

### Construction Order (always Base → Derived)
```
new Derived() call sequence:
  1. Allocate memory for full Derived object
  2. Call Base::Base()      ← base portion initialized first
  3. Set VPTR to Base's VTABLE
  4. Execute Base constructor body
  5. Set VPTR to Derived's VTABLE  ← VPTR updated
  6. Execute Derived constructor body
```

### Destruction Order (always Derived → Base — reverse of construction)
```
delete ptr call sequence:
  1. Execute Derived destructor body
  2. Set VPTR to Base's VTABLE     ← VPTR reverts (why you can't virtual-call in dtor)
  3. Execute Base destructor body
  4. Deallocate memory
```

> ⚠️ **Why this matters:** Any virtual function called during Base's destructor will route to Base's version (not Derived) because the VPTR has already reverted.

---

## 13. Concurrency & OOP — Interview Quick Reference 🔒

| Concept | One-liner |
| :--- | :--- |
| **Object-level lock** | `synchronized(this)` — locks specific instance |
| **Class-level lock** | `synchronized(MyClass.class)` — locks across all instances |
| **Immutable object** | All fields `private final`, no setters, no subclassing — inherently thread-safe |
| **ThreadLocal** | Each thread gets its own copy of the variable — no sharing, no locking |
| **Monitor** | OOP synchronization construct: lock + condition variable + protected data as one unit |
| **Reentrant lock** | Same thread can re-acquire the lock it holds (prevents self-deadlock) |

### Designing an Immutable Class Checklist
1. Declare class `final` (prevent subclassing + behavioral override)
2. All fields `private` and `final`
3. No setter methods
4. Deep copy mutable reference fields in constructor and getters (defensive copy)
