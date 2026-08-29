# Master Guide 07: Master C++ OOP Interview Cheat Sheet

> **Focus:** The Unified C++ OOP Matrix, Modern C++ Idioms (Rule of 0/3/5, RAII, Smart Pointers, C++ Casts), 50 High-Yield Rapid-Fire Verbal Questions with Bold Answers, and the Top 10 Red Flag Junior C++ Mistakes.
> 
> *The 10-Minute Pre-Interview Revision Document for FAANG/Tier-1 Tech Interviews.*

---

# Table of Contents
1. [The Unified C++ OOP Matrix](#1-the-unified-c-oop-matrix)
2. [C++ Modern Idioms & Core Rules Card](#2-c-modern-idioms--core-rules-card)
3. [50 Rapid-Fire C++ OOP Questions & Bold Answers](#3-50-rapid-fire-c-oop-questions--bold-answers)
4. [Top 10 C++ Red Flag Mistakes That Sound Junior](#4-top-10-c-red-flag-mistakes-that-sound-junior)

---

# 1. The Unified C++ OOP Matrix

```
+--------------------------------------------------------------------------------------------------------------------+
| OOP PILLAR     | RELATED SOLID PRINCIPLE | C++ CORE IDIOM / MECHANISM       | RELEVANT LLD DESIGN PATTERN          |
+--------------------------------------------------------------------------------------------------------------------+
| Encapsulation  | Single Responsibility   | `private` fields, RAII,          | Builder, Value Object,               |
|                | Principle (SRP)         | `std::lock_guard`, `std::unique` | Repository Pattern                   |
+--------------------------------------------------------------------------------------------------------------------+
| Abstraction    | Interface Segregation   | Pure Virtual Classes (`= 0`),    | Factory Method, Abstract Factory,    |
|                | (ISP) & DIP             | Header / Source separation       | Adapter Pattern                      |
+--------------------------------------------------------------------------------------------------------------------+
| Inheritance    | Liskov Substitution     | `public` derivation,             | Template Method, Composite Pattern,  |
|                | Principle (LSP)         | `virtual ~Base() = default`      | Decorator Pattern                    |
+--------------------------------------------------------------------------------------------------------------------+
| Polymorphism   | Open / Closed Principle | Dynamic: `vptr`/`vtable`         | Strategy Pattern, Observer Pattern,  |
|                | (OCP)                   | Static: Templates, CRTP          | Command Pattern, State Pattern       |
+--------------------------------------------------------------------------------------------------------------------+
```

---

# 2. C++ Modern Idioms & Core Rules Card

```
+---------------------------------------------------------------------------------------------------+
| 1. THE RULE OF 0 / 3 / 5                                                                          |
|    * Rule of 0: Prefer classes that require NO custom destructor/copy/move (use standard types).  |
|    * Rule of 3 (C++98): If you write 1, you must write all 3:                                    |
|      1. Destructor (~T)                                                                           |
|      2. Copy Constructor (T(const T&))                                                            |
|      3. Copy Assignment Operator (T& operator=(const T&))                                         |
|    * Rule of 5 (Modern C++11/17/20): Add move semantics:                                         |
|      4. Move Constructor (T(T&&) noexcept)                                                        |
|      5. Move Assignment Operator (T& operator=(T&&) noexcept)                                     |
+---------------------------------------------------------------------------------------------------+
| 2. C++ EXPLICIT CASTS DECISION MATRIX                                                             |
|    * static_cast: Compile-time cast for related types (numeric conversions, upcasting pointers).   |
|    * dynamic_cast: Runtime-checked polymorphic downcast (requires >=1 virtual function, uses RTTI)|
|    * const_cast: Adds or strips `const` / `volatile` qualifiers (dangerous if modifying UB).     |
|    * reinterpret_cast: Low-level bit reinterpretation between unrelated pointer types (HFT/HW).   |
+---------------------------------------------------------------------------------------------------+
| 3. SMART POINTER LIFECYCLE MAPPING                                                                |
|    * std::unique_ptr: Exclusive ownership, zero overhead, non-copyable, move-only (Composition).  |
|    * std::shared_ptr: Shared ownership via atomic reference counting (Aggregation).               |
|    * std::weak_ptr: Non-owning observer, prevents cyclic dependency memory leaks.                 |
+---------------------------------------------------------------------------------------------------+
```

---

# 3. 50 Rapid-Fire C++ OOP Questions & Bold Answers

### Topic 1: Encapsulation & Core OOP Basics
1. **What is the difference between `struct` and `class` in C++?**
   - **`struct` members and base inheritance default to `public`; `class` members and base inheritance default to `private`.**
2. **What is RAII in C++?**
   - **Resource Acquisition Is Initialization: tying resource lifetime (memory, file handles, locks) directly to object lifetime via constructors and destructors.**
3. **What is an invariant in class design?**
   - **A condition or business rule that must always remain true throughout the lifetime of an object (e.g. `balance >= 0`).**
4. **What does the `explicit` keyword on a single-argument constructor do?**
   - **Prevents the compiler from performing implicit type conversions and copy-initialization.**
5. **What is the `friend` keyword in C++ and does it violate encapsulation?**
   - **`friend` grants a specific external function or class access to `private` and `protected` members; used judiciously (e.g. `operator<<`), it tightens interfaces without exposing public getters.**
6. **What is the difference between shallow copy and deep copy?**
   - **Shallow copy duplicates pointer addresses (sharing the underlying memory); deep copy allocates a new memory block and copies the actual values.**
7. **What is a copy constructor signature in C++?**
   - **`ClassName(const ClassName& other);` (passed by `const` reference to avoid infinite recursive copying).**
8. **What does the `mutable` keyword do?**
   - **Allows a specific member variable to be modified inside a `const` member function (e.g. internal mutexes or caching counters).**

---

### Topic 2: Inheritance & Memory Layout
9. **What is Object Slicing in C++?**
   - **When a derived class object is assigned or passed by value to a base class variable, slicing off all derived member variables and derived vtable pointers.**
10. **Why should polymorphic objects always be passed by reference or pointer?**
    - **To prevent Object Slicing and enable dynamic virtual method dispatch at runtime.**
11. **What is the difference between `public`, `protected`, and `private` inheritance?**
    - **`public` models "is-a" subtyping; `protected` restricts base public members to derived children; `private` models "has-a" implementation reuse.**
12. **What is the Diamond Problem and how does C++ solve it?**
    - **Multiple inheritance where a class inherits from two classes sharing a common ancestor; solved via Virtual Inheritance (`virtual public Base`).**
13. **How does virtual inheritance affect object memory layout?**
    - **It places a single shared base sub-object at the end of the memory layout and inserts a Virtual Base Pointer (`vbptr`) in intermediate sub-objects.**
14. **In virtual inheritance, who is responsible for calling the virtual base constructor?**
    - **The most derived class (leaf class) is directly responsible.**
15. **What is the size of an empty class in C++?**
    - **1 byte to guarantee unique memory addresses for distinct object instances; 8 bytes on 64-bit systems if it contains virtual functions (`vptr`).**
16. **What is the order of constructor execution in inheritance?**
    - **Virtual base classes first, then non-virtual base classes in declaration order, then member objects in declaration order, then the derived constructor body.**
17. **What is the order of destructor execution?**
    - **Exact reverse of construction: derived destructor body, member destructors, non-virtual base destructors, virtual base destructors.**

---

### Topic 3: Polymorphism, Virtual Functions & Vtables
18. **Why must a base class with virtual functions have a `virtual` destructor?**
    - **To ensure that calling `delete basePtr` invokes the derived class destructor first, preventing resource and memory leaks.**
19. **How does dynamic dispatch work under the hood in C++?**
    - **Each polymorphic class has a static `vtable` of function pointers; each object instance stores a hidden `vptr` pointer pointing to its class vtable.**
20. **Can a constructor be `virtual` in C++?**
    - **No. The concrete type and `vptr` are not established until construction begins.**
21. **Can a virtual function be inlined by the compiler?**
    - **Yes, when invoked directly on an object value (`obj.func()`) or when the compiler can prove the exact concrete type (devirtualization).**
22. **What is a Pure Virtual Function?**
    - **A function declared with `= 0` (`virtual void foo() = 0;`), making the enclosing class an Abstract Class that cannot be instantiated directly.**
23. **What is the difference between function overloading and function overriding?**
    - **Overloading occurs at compile-time within the same scope (different signatures); overriding occurs at runtime across base and derived classes (identical signatures with `virtual`).**
24. **What does the `override` specifier in C++11 do?**
    - **Instructs the compiler to verify that the method overrides an exact matching virtual base method, preventing signature mismatch bugs.**
25. **What does the `final` specifier on a class or virtual method do?**
    - **Prevents a class from being inherited or a virtual method from being overridden, enabling compiler devirtualization optimizations.**
26. **What is CRTP (Curiously Recurring Template Pattern)?**
    - **A static polymorphism technique (`class Derived : public Base<Derived>`) achieving compile-time polymorphic dispatch with zero vtable overhead.**
27. **What is RTTI in C++ and what is its overhead?**
    - **Run-Time Type Information: enables `dynamic_cast` and `typeid` by storing type metadata in the vtable, adding slight memory and execution overhead.**

---

### Topic 4: SOLID Principles & Modern Design
28. **What does the Single Responsibility Principle (SRP) state?**
    - **A class should have only one reason to change, encapsulating a single cohesive responsibility.**
29. **What does the Open/Closed Principle (OCP) state?**
    - **Classes should be open for extension (via polymorphism or templates) but closed for modification.**
30. **What is the Liskov Substitution Principle (LSP) violation in the Square-Rectangle problem?**
    - **Setting `Square`'s width mutates its height, violating the `Rectangle` invariant where width and height vary independently.**
31. **What is the Interface Segregation Principle (ISP)?**
    - **Clients should not be forced to depend on fat interfaces with methods they do not use.**
32. **What is the Dependency Inversion Principle (DIP)?**
    - **High-level modules should depend on abstractions (interfaces) rather than concrete implementations.**
33. **Why should you favor Composition over Inheritance?**
    - **Composition provides loose coupling, dynamic runtime behavior swapping, preserves encapsulation, and avoids the Fragile Base Class problem.**
34. **What is the Fragile Base Class problem?**
    - **When internal implementation changes or vtable modifications to a base class unintentionally break derived subclasses across a codebase.**
35. **What is the difference between Aggregation and Composition?**
    - **Composition represents exclusive ownership ("part-of", child dies with parent); Aggregation represents shared ownership ("has-a", child outlives container).**
36. **Which smart pointer models Composition in C++?**
    - **`std::unique_ptr` (or direct value member embedding).**
37. **Which smart pointer models Aggregation in C++?**
    - **`std::shared_ptr`.**
38. **How do you break cyclic `std::shared_ptr` references?**
    - **By using `std::weak_ptr` for observing back-references.**

---

### Topic 5: Modern C++ Features & Move Semantics
39. **What is an rvalue reference in C++11?**
    - **A reference (`T&&`) that binds to temporary objects (rvalues), allowing their resources to be stolen (moved) instead of copied.**
40. **What does `std::move` actually do?**
    - **It does NOT move anything; it is a static cast converting an lvalue expression into an rvalue reference (`static_cast<T&&>(val)`).**
41. **Why should move constructors and move assignment operators be marked `noexcept`?**
    - **To allow standard containers like `std::vector` to use move operations during dynamic resizing rather than falling back to slow deep copies.**
42. **What is the difference between `std::make_unique` and `new`?**
    - **`std::make_unique` is exception-safe against memory leaks during multiple argument evaluations and avoids raw `new` calls.**
43. **What is the difference between `std::make_shared` and `std::shared_ptr<T>(new T())`?**
    - **`std::make_shared` allocates the object and the reference control block in a single contiguous memory allocation, saving one heap allocation.**
44. **When should you NOT use `std::make_shared`?**
    - **When large objects have long-lived `std::weak_ptr` observers, because the memory block cannot be freed until all weak pointers expire.**
45. **What is the difference between `static_cast` and `dynamic_cast`?**
    - **`static_cast` performs compile-time checks without RTTI; `dynamic_cast` performs runtime polymorphic downcast checks and returns `nullptr` on failure.**
46. **What is the cost of calling a virtual function vs. a regular function?**
    - **One extra memory indirection through the `vtable` pointer (`vptr[offset]`) and inhibition of compiler inlining.**
47. **What is the difference between `const int* p`, `int* const p`, and `const int* const p`?**
    - **`const int* p` is pointer to const int; `int* const p` is const pointer to int; `const int* const p` is const pointer to const int.**
48. **What does `const` at the end of a member function declaration mean?**
    - **It treats the `this` pointer as `const ClassName* const`, preventing modification of any non-`mutable` member variables.**
49. **What is the difference between `delete` and `delete[]` in C++?**
    - **`delete` destructs a single object; `delete[]` reads the array size prefix in the heap header to invoke destructors for every element in the array.**
50. **What is the Copy-and-Swap idiom in C++?**
    - **An assignment operator implementation technique providing strong exception safety by passing by value and swapping internal state with `std::swap`.**

---

# 4. Top 10 C++ Red Flag Mistakes That Sound Junior

```
+---------------------------------------------------------------------------------------------------+
| #  | THE JUNIOR C++ MISTAKE                 | THE SENIOR CORRECTION / INTERVIEW PIVOT             |
+---------------------------------------------------------------------------------------------------+
| 1  | "Omitting `virtual` on base class      | Deleting derived objects through base pointers      |
|    | destructors."                          | causes UB and resource leaks without virtual dtor.  |
+---------------------------------------------------------------------------------------------------+
| 2  | "Passing polymorphic objects by value."| Always pass by `const Base&` or `std::unique_ptr`;  |
|    |                                        | passing by value causes fatal Object Slicing.       |
+---------------------------------------------------------------------------------------------------+
| 3  | "Claiming `std::move` moves data."     | `std::move` is just an unconditional rvalue cast    |
|    |                                        | (`static_cast<T&&>`); move constructor moves data.  |
+---------------------------------------------------------------------------------------------------+
| 4  | "Using raw `new` and `delete` in 2026."| Use modern RAII: `std::make_unique`, smart pointers,|
|    |                                        | or standard containers (`std::vector`).             |
+---------------------------------------------------------------------------------------------------+
| 5  | "Assuming Diamond Problem is method-   | It duplicates member variables in memory, causing   |
|    | only."                                 | state inconsistency without virtual inheritance.    |
+---------------------------------------------------------------------------------------------------+
| 6  | "Overusing `std::shared_ptr` everywhere"| Default to `std::unique_ptr` for exclusive owner;  |
|    |                                        | shared_ptr adds atomic ref-counting cache overhead. |
+---------------------------------------------------------------------------------------------------+
| 7  | "Forgetting `noexcept` on move ctors." | Without `noexcept`, `std::vector` reallocations     |
|    |                                        | fall back to expensive deep copies for safety.      |
+---------------------------------------------------------------------------------------------------+
| 8  | "Using `dynamic_cast` in tight loops." | `dynamic_cast` traverses RTTI structures at runtime;|
|    |                                        | use static polymorphism, CRTP, or visitor pattern.  |
+---------------------------------------------------------------------------------------------------+
| 9  | "Returning references to local stack   | Stack frames are destroyed on return, leaving       |
|    | objects."                              | dangling references and memory corruption.          |
+---------------------------------------------------------------------------------------------------+
| 10 | "Confusing Aggregation & Composition." | Composition owns the child lifecycle; Aggregation  |
|    |                                        | shares the child lifecycle with other containers.   |
+---------------------------------------------------------------------------------------------------+
```
