# Master Guide 08: Top 40 C++ OOP Spoken Flashcards & Trap Questions

> **Focus:** 40 High-Yield Spoken Flashcards for C++ Object-Oriented Programming and Low-Level Design interviews at Google, Meta, Amazon, Microsoft, Uber, and High-Frequency Trading (HFT) firms.
> 
> *The 15-minute complete verbal drill to read one day before any tech interview.*

---

# Table of Contents
1. [Core OOP Pillars & C++ Encapsulation (Cards 1–8)](#1-core-oop-pillars--c-encapsulation-cards-18)
2. [Inheritance, Memory Layout & Slicing (Cards 9–16)](#2-inheritance-memory-layout--slicing-cards-916)
3. [Polymorphism, Virtual Tables & RTTI (Cards 17–24)](#3-polymorphism-virtual-tables--rtti-cards-1724)
4. [SOLID Principles & Architecture (Cards 25–30)](#4-solid-principles--architecture-cards-2530)
5. [Composition, Smart Pointers & Modern C++ Idioms (Cards 31–40)](#5-composition-smart-pointers--modern-c-idioms-cards-3140)

---

# 1. Core OOP Pillars & C++ Encapsulation (Cards 1–8)

### Card 1: "What is the core difference between Encapsulation and Abstraction?"
> **Spoken Answer:** **Encapsulation** is data hiding and boundary enforcement: packaging data members and member functions into an isolated unit and restricting direct external mutation via access specifiers (`private`). **Abstraction** is complexity hiding: defining high-level contracts (via pure virtual interfaces) so callers interact with *what* the module does rather than *how* it does it.

---

### Card 2: "What is the difference between `struct` and `class` in C++?"
> **Spoken Answer:** In C++, `struct` and `class` are completely identical with only two default visibility differences:
> 1. In a `struct`, members and base class inheritance default to **`public`**.
> 2. In a `class`, members and base class inheritance default to **`private`**.
> In modern C++, idiomatic style uses `struct` for passive data holders with no invariant logic (Plain Old Data / POD) and `class` for objects that encapsulate invariant-enforcing behavior.

---

### Card 3: "What is an invariant in class design and how is it maintained?"
> **Spoken Answer:** An **invariant** is a condition or business rule that must always hold true throughout the lifetime of a valid object (e.g. `balance >= 0` or `capacity >= size`). It is established in the constructor, validated inside every mutating public method, and protected by keeping underlying member fields strictly `private`.

---

### Card 4: "What does the `explicit` keyword on a constructor do?"
> **Spoken Answer:** `explicit` prevents the compiler from performing **implicit type conversions** or copy-initialization when passing arguments. For example, without `explicit`, `Complex c = 5;` silently calls `Complex(int)`, which can cause subtle runtime bugs; marking it `explicit Complex(int r)` forces explicit construction `Complex c(5);`.

---

### Card 5: "What is the `friend` keyword in C++ and does it violate encapsulation?"
> **Spoken Answer:** `friend` grants a specific external function or class access to `private` and `protected` members. When used judiciously (such as for `operator<<` stream formatting or tight factory-product pairings), it **enhances encapsulation** by granting selective access without forcing the class to expose public getter/setter methods to the entire global codebase.

---

### Card 6: "What is the difference between Shallow Copy and Deep Copy?"
> **Spoken Answer:** **Shallow Copy** duplicates only the top-level member fields (copying raw pointer addresses so both objects point to the same memory block, causing double-free crashes on destruction). **Deep Copy** allocates a separate, distinct memory buffer on the heap and duplicates the actual pointed-to values.

---

### Card 7: "Why is the Copy Constructor parameter always passed by `const ClassName&`?"
> **Spoken Answer:** If it were passed by value (`ClassName(ClassName other)`), invoking the copy constructor would require making a copy of `other`, which would invoke the copy constructor again, resulting in an **infinite recursive compilation error**. Passing by `const` reference avoids copying and protects the source object from accidental mutation.

---

### Card 8: "What does the `mutable` keyword do in C++?"
> **Spoken Answer:** `mutable` allows a specific member variable to be modified even inside `const` member functions. It is used for variables that represent internal implementation details rather than observable logical state, such as **caching variables, hit counters, and `std::mutex` instances**.

---

# 2. Inheritance, Memory Layout & Slicing (Cards 9–16)

### Card 9: "What is Object Slicing in C++ and how do you prevent it?"
> **Spoken Answer:** **Object Slicing** occurs when a derived class object is assigned or passed by value to a base class parameter (`void func(Base b)`). The compiler allocates space only for the `Base` sub-object, physically chopping off all derived member variables and resetting the `vptr` to the base class vtable. It is prevented by **always passing polymorphic objects by reference (`const Base&`) or by pointer (`std::unique_ptr<Base>`)**.

---

### Card 10: "What is the difference between `public`, `protected`, and `private` inheritance?"
> **Spoken Answer:**
> - **`public` inheritance:** Models strict "is-a" subtyping; base public/protected members retain their visibility in Derived.
> - **`protected` inheritance:** Base public/protected members become `protected` in Derived (hidden from clients, visible to children).
> - **`private` inheritance:** Models "has-a" implementation reuse; base public/protected members become `private` in Derived.

---

### Card 11: "What is the Diamond Problem in C++ and how does Virtual Inheritance solve it?"
> **Spoken Answer:** The Diamond Problem occurs in multiple inheritance when class $D$ inherits from $B$ and $C$, which both inherit from base $A$, causing duplicate copies of $A$'s member variables in memory and compiler ambiguity on member access. C++ solves it via **Virtual Inheritance (`class B : virtual public A`)**, which stores a single shared $A$ sub-object at the end of the object layout and inserts a Virtual Base Pointer (`vbptr`) in $B$ and $C$ to compute offsets at runtime.

---

### Card 12: "How does Virtual Inheritance alter object memory layout?"
> **Spoken Answer:** Without virtual inheritance, class $D$ contains two complete $A$ sub-objects in its memory footprint. With virtual inheritance, the compiler collapses $A$ into a **single shared sub-object at the very end of $D$**, inserting a **`vbptr` (Virtual Base Pointer)** into each intermediate sub-object ($B$ and $C$) that indexes a `vbtable` offset at runtime to locate $A$.

---

### Card 13: "In Virtual Inheritance, who is responsible for invoking the virtual base constructor?"
> **Spoken Answer:** The **most derived class ($D$) is directly responsible** for initializing the virtual base class ($A$) in its constructor initialization list. Constructor calls to $A$ from intermediate base classes ($B$ and $C$) are completely ignored by the compiler.

---

### Card 14: "What is the size of an empty class in C++?"
> **Spoken Answer:** **1 byte** (e.g. `sizeof(Empty) == 1`). The C++ standard mandates that every distinct object instance must have a unique, non-zero memory address. If the class has virtual functions, its size becomes **8 bytes** on 64-bit systems to accommodate the hidden `vptr`.

---

### Card 15: "What is the exact order of construction and destruction in C++ inheritance?"
> **Spoken Answer:**
> - **Construction:** Virtual base classes first $\to$ Non-virtual base classes in declaration order $\to$ Non-static member fields in declaration order $\to$ Derived constructor body.
> - **Destruction:** Exact reverse of construction: Derived destructor body $\to$ Member destructors $\to$ Non-virtual base destructors $\to$ Virtual base destructors.

---

### Card 16: "Why MUST a base class with virtual functions always have a `virtual` destructor?"
> **Spoken Answer:** If a derived class object allocated on the heap is deleted through a base class pointer (`Base* p = new Derived(); delete p;`) and the base destructor is non-virtual, the compiler performs static binding and **invokes only `~Base()`**. The derived class destructor `~Derived()` is completely bypassed, leaking memory and dynamically allocated handles, resulting in **Undefined Behavior**. Declaring `virtual ~Base() = default;` ensures the derived destructor executes first before chaining to the base destructor.

---

# 3. Polymorphism, Virtual Tables & RTTI (Cards 17–24)

### Card 17: "How does C++ dynamic virtual dispatch work under the hood (`vptr` & `vtable`)?"
> **Spoken Answer:** For every class containing virtual functions, the compiler generates a static **Virtual Method Table (`vtable`)** in read-only memory containing function pointers to its virtual methods. Each object instance of that class contains a hidden 8-byte pointer called **`vptr` (Virtual Table Pointer)** inserted in its memory layout. When calling `ptr->virtualMethod()`, the CPU reads `ptr->__vptr`, indexes the method's fixed offset in the vtable, and jumps to that function pointer.

---

### Card 18: "Can a Constructor be `virtual` in C++?"
> **Spoken Answer:** **No.** A constructor cannot be virtual because to create an object, the compiler must know its exact concrete type at compile time to allocate the appropriate memory buffer and initialize the `vptr`. Virtual dispatch cannot occur until the `vptr` is set up inside the constructor.

---

### Card 19: "Can a Virtual Function be inlined by the C++ compiler?"
> **Spoken Answer:** **Yes, under two conditions:**
> 1. When invoked directly on an object value rather than a pointer/reference (`Dog d; d.speak();`).
> 2. When the compiler's optimizer can perform **devirtualization**, proving at compile time that a pointer or reference points to an exact concrete class (e.g. on `final` methods/classes).

---

### Card 20: "What is a Pure Virtual Function and what is an Abstract Class?"
> **Spoken Answer:** A **Pure Virtual Function** is a virtual method declared with `= 0` (`virtual void execute() = 0;`), indicating it has no default implementation (though one can be defined). A class with at least one pure virtual function becomes an **Abstract Class**, which cannot be directly instantiated.

---

### Card 21: "What is the difference between Function Overloading and Function Overriding?"
> **Spoken Answer:** **Overloading** occurs at compile time within the *same scope*, where multiple functions share the same name but have different parameter types/counts. **Overriding** occurs at runtime across *base and derived scopes*, where a derived class provides a new implementation for a base class `virtual` method with an identical signature.

---

### Card 22: "What do the `override` and `final` specifiers do in C++11?"
> **Spoken Answer:** **`override`** forces the compiler to verify that a method is overriding a base virtual method with an identical signature, preventing silent bugs from mismatched parameter types. **`final`** specifies that a virtual method cannot be overridden in derived classes, or that a class cannot be inherited, allowing the compiler to perform **devirtualization optimizations** (converting virtual calls to direct static calls).

---

### Card 23: "What is CRTP (Curiously Recurring Template Pattern) and why is it used?"
> **Spoken Answer:** CRTP is a static polymorphism idiom where a class inherits from a templated base class parameterized by the derived class itself (`class Derived : public Base<Derived>`). It achieves **polymorphic method dispatch at compile-time with zero vtable/vptr overhead and full compiler inlining**, heavily used in High-Frequency Trading (HFT).

---

### Card 24: "What is RTTI in C++ and what is its performance overhead?"
> **Spoken Answer:** **Run-Time Type Information (RTTI)** is the mechanism that powers `dynamic_cast` and `typeid`. It stores type descriptor metadata alongside the class `vtable`. `dynamic_cast` traverses the class inheritance tree at runtime, incurring non-negligible CPU overhead in performance-critical inner loops.

---

# 4. SOLID Principles & Architecture (Cards 25–30)

### Card 25: "What is the Single Responsibility Principle (SRP) and how do you spot its violation?"
> **Spoken Answer:** SRP states that a class should have **one, and only one, reason to change**. You spot a violation when a single class combines multiple disparate concerns, such as parsing user input, executing SQL database persistence, and formatting outbound notification emails. The fix is to decouple these concerns into cohesive services and repositories (`UserService`, `UserRepository`, `EmailService`).

---

### Card 26: "How does the Open/Closed Principle (OCP) eliminate `switch(type)` statements?"
> **Spoken Answer:** OCP states that software entities should be **open for extension but closed for modification**. When you see a `switch(shapeType)` statement, adding a new shape requires modifying existing battle-tested code. OCP replaces the switch statement with a polymorphic base class (`IShape::calculateArea()`), allowing new shapes to be introduced in separate translation units without touching existing calculator logic.

---

### Card 27: "Why does the Square-Rectangle inheritance hierarchy violate the Liskov Substitution Principle (LSP)?"
> **Spoken Answer:** Mathematically, a square is a rectangle. However, in object-oriented contracts, a `Rectangle` guarantees the invariant that setting width and height vary independently. In a `Square`, setting `setWidth()` must also alter `height`. If a client passes a `Square` to a function expecting a `Rectangle` and sets width to 5 and height to 4, expecting an area of 20, the square produces 16, **breaking client assumptions and violating LSP**.

---

### Card 28: "What is the Interface Segregation Principle (ISP)?"
> **Spoken Answer:** ISP states that **clients should not be forced to depend on interfaces they do not use**. Instead of creating one monolithic "fat" interface containing methods for printing, scanning, and faxing, ISP decomposes it into small, cohesive interfaces (`IPrinter`, `IScanner`). Classes implement only the interfaces relevant to their capabilities, avoiding dummy no-op or exception-throwing overrides.

---

### Card 29: "What is the Dependency Inversion Principle (DIP) and how does it relate to Dependency Injection?"
> **Spoken Answer:** **DIP is the architectural principle** stating that high-level modules should depend on abstractions (interfaces) rather than concrete low-level implementations. **Dependency Injection (DI)** is the software pattern used to fulfill DIP by supplying concrete instances (e.g. passing `std::unique_ptr<IPaymentGateway>` into a class constructor) rather than letting the class instantiate dependencies with raw `new` calls.

---

### Card 30: "What is the difference between a Pure Interface and an Abstract Base Class in C++?"
> **Spoken Answer:** A **Pure Interface** in C++ is a class that contains **only pure virtual functions (`= 0`) and a virtual destructor**, with zero member variables and zero concrete implementations. An **Abstract Base Class** contains at least one pure virtual function, but may also store member variables (state) and provide shared concrete helper methods (such as in the Template Method pattern).

---

# 5. Composition, Smart Pointers & Modern C++ Idioms (Cards 31–40)

### Card 31: "Why should you Favor Object Composition over Class Inheritance?"
> **Spoken Answer:** Inheritance creates tight, rigid compile-time coupling where derived classes depend on the internal implementation details of the base class (white-box reuse), exposing them to the **Fragile Base Class problem**. Composition builds systems by combining loosely coupled objects behind interfaces (black-box reuse), allowing **dynamic runtime behavior swapping (Strategy Pattern)**, preserving encapsulation, and eliminating Object Slicing.

---

### Card 32: "What is the Fragile Base Class problem?"
> **Spoken Answer:** A fundamental architectural flaw where seemingly innocuous modifications to a base class (such as altering internal method delegation or adding new virtual methods that shift vtable index offsets) **unintentionally break the runtime invariants or binary ABI compatibility of derived subclasses across large codebases**.

---

### Card 33: "What is the difference between Association, Aggregation, and Composition?"
> **Spoken Answer:**
> - **Association ("Uses-a"):** Weakest relationship where independent objects interact (Driver uses a Car).
> - **Aggregation ("Has-a"):** Shared ownership with independent lifecycles; child outlives the container (Department has Employees).
> - **Composition ("Part-of"):** Exclusive, strong ownership with coupled lifecycles; child is destroyed when the parent dies (House has Rooms).

---

### Card 34: "How do C++ Smart Pointers map to OOP relationships?"
> **Spoken Answer:**
> - **Composition:** Expressed via **`std::unique_ptr<Child>`** (or direct value embedding `Child child;`) representing exclusive ownership.
> - **Aggregation:** Expressed via **`std::shared_ptr<Child>`** representing shared co-ownership.
> - **Association:** Expressed via non-owning raw pointers (`Target*`), references (`const Target&`), or **`std::weak_ptr<Target>`** to break cyclic dependencies.

---

### Card 35: "How does `std::weak_ptr` break Cyclic Reference memory leaks?"
> **Spoken Answer:** If two objects hold strong `std::shared_ptr` references to each other, their reference counts can never drop to 0, resulting in a permanent memory leak. `std::weak_ptr` holds a non-owning observing reference that does not increment the strong reference count, allowing the parent object to be destroyed normally when out of scope.

---

### Card 36: "What is the Rule of 0 / 3 / 5 in modern C++?"
> **Spoken Answer:**
> - **Rule of 0:** Prefer classes that rely on standard resource-managing types (`std::vector`, `std::unique_ptr`), requiring **no custom destructor, copy, or move operations**.
> - **Rule of 3 (C++98):** If a class manages raw resources and defines a Destructor, it must also define a Copy Constructor and Copy Assignment Operator.
> - **Rule of 5 (Modern C++):** If custom resource management is needed, define all 5 special member functions: Destructor, Copy Constructor, Copy Assignment, **Move Constructor (`noexcept`)**, and **Move Assignment (`noexcept`)**.

---

### Card 37: "What does `std::move` actually do under the hood?"
> **Spoken Answer:** `std::move` **does NOT move anything**. It is an unconditional compile-time cast that converts an lvalue expression into an rvalue reference (`static_cast<T&&>(var)`). The actual transfer of resource ownership is performed by the invoked **Move Constructor or Move Assignment Operator**.

---

### Card 38: "What is the difference between `std::make_unique` and `std::make_shared`?"
> **Spoken Answer:** `std::make_unique` allocates the managed object on the heap and wraps it in an exclusive pointer. `std::make_shared` allocates the managed object **AND the reference-counting control block together in a single contiguous memory buffer**, saving one memory allocation and improving CPU cache locality.

---

### Card 39: "What is the difference between `static_cast`, `dynamic_cast`, and `reinterpret_cast`?"
> **Spoken Answer:**
> - **`static_cast`:** Performs compile-time type conversions between related types (numeric types, upcasting base pointers) with **zero runtime overhead**.
> - **`dynamic_cast`:** Safely downcasts base pointers to derived types at runtime by checking RTTI metadata in the vtable, returning `nullptr` (or throwing for references) on failure.
> - **`reinterpret_cast`:** Reinterprets the raw binary bit-pattern of a pointer to an unrelated type with no safety checks, used primarily in low-level systems programming.

---

### Card 40: "What is the Copy-and-Swap idiom in C++?"
> **Spoken Answer:** It is an idiomatic implementation of the assignment operator (`T& operator=(T other)`) that takes its argument by value (reusing the copy constructor) and swaps internal state with `std::swap`. It provides **strong exception safety** and completely eliminates duplicate code between copy constructors and assignment operators.
