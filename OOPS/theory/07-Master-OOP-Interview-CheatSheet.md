# Core Object-Oriented Programming Theorems, Idioms & Architecture Reference

> **Scope:** Unified OOP Architecture Matrix, Modern C++ Resource Management Idioms (Rule of 0/3/5, Explicit Casts, Smart Pointer Models, Copy-and-Swap), 50 Foundational OOP & Systems Design Principles, and Memory Management & Concurrency Failure Modes.

---

# Table of Contents
1. [Unified OOP Design Matrix](#1-unified-oop-design-matrix)
2. [Modern C++ Memory Management & Resource Idioms](#2-modern-c-memory-management--resource-idioms)
3. [50 Foundational Object-Oriented & C++ Systems Principles](#3-50-foundational-object-oriented--c-systems-principles)
4. [Critical Object-Oriented Anti-Patterns & Failure Modes](#4-critical-object-oriented-anti-patterns--failure-modes)

---

# 1. Unified OOP Design Matrix

```
+--------------------------------------------------------------------------------------------------------------------+
| OOP PILLAR     | RELATED SOLID PRINCIPLE | C++ CORE IDIOM / MECHANISM       | STRUCTURAL DESIGN PATTERN            |
+--------------------------------------------------------------------------------------------------------------------+
| Encapsulation  | Single Responsibility   | `private` visibility, RAII,      | Builder, Value Object,               |
|                | Principle (SRP)         | `std::lock_guard`, `std::unique` | Repository Pattern                   |
+--------------------------------------------------------------------------------------------------------------------+
| Abstraction    | Interface Segregation   | Pure Virtual Interfaces (`= 0`), | Factory Method, Abstract Factory,    |
|                | (ISP) & DIP             | Header/Source implementation sep | Adapter Pattern                      |
+--------------------------------------------------------------------------------------------------------------------+
| Inheritance    | Liskov Substitution     | `public` subtyping,              | Template Method, Composite Pattern,  |
|                | Principle (LSP)         | `virtual ~Base() = default`      | Decorator Pattern                    |
+--------------------------------------------------------------------------------------------------------------------+
| Polymorphism   | Open / Closed Principle | Dynamic: `vptr`/`vtable`         | Strategy Pattern, Observer Pattern,  |
|                | (OCP)                   | Static: Templates, CRTP          | Command Pattern, State Pattern       |
+--------------------------------------------------------------------------------------------------------------------+
```

---

# 2. Modern C++ Memory Management & Resource Idioms

```
+---------------------------------------------------------------------------------------------------+
| 1. THE RULE OF 0 / 3 / 5                                                                          |
|    * Rule of 0: Design classes requiring zero custom destructors, copy, or move operations       |
|      by leveraging RAII member types (std::unique_ptr, std::vector, std::string).                 |
|    * Rule of 3: If managing raw pointers manually, implement Destructor (~T),                     |
|      Copy Constructor (T(const T&)), and Copy Assignment (T& operator=(const T&)).                |
|    * Rule of 5: In modern C++, complement Rule of 3 with Move Constructor                         |
|      (T(T&&) noexcept) and Move Assignment Operator (T& operator=(T&&) noexcept).                 |
+---------------------------------------------------------------------------------------------------+
| 2. C++ EXPLICIT CASTS SPECIFICATION                                                               |
|    * static_cast: Compile-time conversion for related types (numeric types, explicit upcasts).    |
|    * dynamic_cast: Runtime-checked polymorphic downcasting (requires virtual base class; uses RTTI)|
|    * const_cast: Adds or removes const/volatile qualifiers from references and pointers.          |
|    * reinterpret_cast: Low-level bit pattern reinterpretation between incompatible pointer types. |
+---------------------------------------------------------------------------------------------------+
| 3. SMART POINTER LIFECYCLE MAPPING                                                                |
|    * std::unique_ptr: Exclusive non-copyable ownership; zero memory overhead (Composition).       |
|    * std::shared_ptr: Shared ownership with thread-safe atomic reference count control block.    |
|    * std::weak_ptr: Non-owning observer preventing cyclic shared_ptr memory leaks.                |
+---------------------------------------------------------------------------------------------------+
```

---

# 3. 50 Foundational Object-Oriented & C++ Systems Principles

### Category A: Encapsulation & Class Design
1. **`struct` vs. `class` Access Defaults:** `struct` defaults to `public` member and inheritance visibility; `class` defaults to `private`.
2. **Resource Acquisition Is Initialization (RAII):** Binds resource acquisition to object construction and resource release to object destruction.
3. **Class Invariant Inviolability:** Class methods must guarantee valid internal state throughout the object's entire lifecycle.
4. **`explicit` Constructor Specifier:** Prevents implicit single-argument constructor conversions and copy-initialization.
5. **`friend` Declarations:** Permits fine-grained access to private members for closely coupled utilities without exposing public getters.
6. **Shallow vs. Deep Copy:** Shallow copy duplicates pointer addresses; deep copy allocates independent memory buffers and copies underlying payloads.
7. **Copy Constructor Signature:** `ClassName(const ClassName& other)` prevents infinite recursive copy instantiation.
8. **`mutable` Specifier:** Permits specific member modification within `const` qualified member functions.

---

### Category B: Inheritance & Memory Layout
9. **Object Slicing:** Occurs when passing a derived instance by value to a base parameter, stripping derived members and rewriting the `vtable` pointer.
10. **Pass-by-Reference for Polymorphic Types:** Polymorphic objects must be passed via pointer (`Base*`) or reference (`const Base&`) to preserve dynamic dispatch.
11. **Inheritance Visibility Modes:** `public` models subtyping; `protected` restricts base public members to derived scopes; `private` implements internal reuse.
12. **Diamond Problem Solution:** Virtual inheritance (`virtual public Base`) collapses multiple base sub-objects into a single shared sub-object.
13. **Virtual Base Pointer (`vbptr`):** Intermediate classes store a `vbptr` referencing a `vbtable` to compute the dynamic offset of shared virtual base members.
14. **Most-Derived Construction Rule:** In virtual inheritance, the leaf derived class is directly responsible for invoking the virtual base constructor.
15. **Empty Class Sizing:** Allocates 1 byte to guarantee unique object memory addresses; expands to 8 bytes on 64-bit systems if virtual methods exist (`vptr`).
16. **Construction Order:** Virtual base classes $\to$ non-virtual base classes $\to$ member fields $\to$ derived constructor body.
17. **Destruction Order:** Exact reverse order of construction.

---

### Category C: Polymorphism & Dynamic Dispatch
18. **Virtual Destructor Invariant:** Any class defining virtual member functions must define a virtual destructor to prevent undefined deletion behavior.
19. **Dynamic Dispatch Mechanics:** Classes with virtual functions maintain a static `vtable` of function pointers; instances store a hidden `vptr` pointer.
20. **Virtual Constructor Infeasibility:** Constructors cannot be virtual because object type resolution and `vptr` assignment occur during construction.
21. **Virtual Method Inlining:** Compilers can inline virtual functions when called directly on concrete objects or when concrete types are proven (devirtualization).
22. **Pure Virtual Method (`= 0`):** Enforces derived class overrides and renders the enclosing class abstract and non-instantiable.
23. **Overloading vs. Overriding:** Overloading resolves at compile-time within the same scope; overriding resolves dynamically across class hierarchies.
24. **`override` Specifier:** Validates that a member function overrides an exact matching virtual base signature at compile time.
25. **`final` Specifier:** Prevents class inheritance or virtual function overriding, enabling compiler devirtualization optimizations.
26. **Curiously Recurring Template Pattern (CRTP):** Implements static polymorphism at compile time with zero dynamic dispatch overhead.
27. **Run-Time Type Information (RTTI):** Embeds type metadata in `vtable` structures to support `dynamic_cast` and `typeid`.

---

### Category D: SOLID Principles & Structural Coupling
28. **Single Responsibility Principle (SRP):** Classes must exhibit high cohesion and contain only a single axis of change.
29. **Open/Closed Principle (OCP):** Software modules must be open for behavioral extension via polymorphism without modifying source code.
30. **Liskov Substitution Principle (LSP):** Derived classes must preserve the preconditions, postconditions, and invariants of their base types.
31. **Interface Segregation Principle (ISP):** Broad interfaces must be partitioned into focused, client-specific role contracts.
32. **Dependency Inversion Principle (DIP):** High-level policy must depend on abstract contracts rather than concrete low-level implementations.
33. **Composition over Inheritance:** Composition provides loose coupling, dynamic runtime behavior swapping, and eliminates Fragile Base Class bugs.
34. **Fragile Base Class Problem:** Unintended base class implementation mutations or vtable index shifts break derived subclasses.
35. **Aggregation vs. Composition:** Composition denotes exclusive lifecycle ownership; Aggregation denotes independent shared lifecycles.
36. **Composition Memory Primitive:** Modeled in C++ via `std::unique_ptr` or direct inline member value embedding.
37. **Aggregation Memory Primitive:** Modeled in C++ via `std::shared_ptr`.
38. **Cyclic Reference Mitigation:** Modeled using non-owning `std::weak_ptr` observers.

---

### Category E: Modern C++ Features & Semantics
39. **Rvalue References (`T&&`):** Bind to temporary objects to transfer resource ownership without copying.
40. **`std::move` Mechanics:** Performs an unconditional cast of an expression to an rvalue reference (`static_cast<T&&>(val)`).
41. **`noexcept` on Move Constructors:** Required for standard containers (`std::vector`) to utilize move semantics during dynamic buffer reallocation.
42. **`std::make_unique` vs. `new`:** Provides exception safety during multi-argument evaluation and enforces exclusive ownership.
43. **`std::make_shared` Optimization:** Allocates the user object and reference count control block within a single contiguous heap allocation.
44. **`std::make_shared` Lifecycle Limitation:** The single heap block cannot be reclaimed until both strong and weak reference counts reach zero.
45. **`static_cast` vs. `dynamic_cast`:** `static_cast` computes compile-time offsets; `dynamic_cast` evaluates runtime RTTI metadata and returns `nullptr` on mismatch.
46. **Virtual Function Indirection Latency:** Incurs an extra memory read through the `vtable` pointer and inhibits compiler inlining optimizations.
47. **Pointer Constness:** `const int*` (pointer to constant), `int* const` (constant pointer), `const int* const` (constant pointer to constant).
48. **Trailing `const` Member Function:** Treats the implicit `this` pointer as `const ClassName* const`, prohibiting non-mutable state mutation.
49. **`delete` vs. `delete[]`:** `delete` destructs a scalar object; `delete[]` reads heap allocation headers to destruct all array elements.
50. **Copy-and-Swap Idiom:** Guarantees strong exception safety by passing parameters by value and executing `std::swap` on internal state.

---

# 4. Critical Object-Oriented Anti-Patterns & Failure Modes

```
+---------------------------------------------------------------------------------------------------+
| #  | ARCHITECTURAL PITFALL                  | MECHANISM & RESOLUTION                               |
+---------------------------------------------------------------------------------------------------+
| 1  | Non-Virtual Base Destructors           | Causes undefined behavior when deleting polymorphic  |
|    |                                        | objects through base pointers; declare virtual ~T(). |
+---------------------------------------------------------------------------------------------------+
| 2  | Object Slicing on Pass-by-Value        | Truncates derived attributes; pass by const ref/ptr. |
+---------------------------------------------------------------------------------------------------+
| 3  | Circular `std::shared_ptr` Cycles      | Prevents ref-count reclamation; break with weak_ptr. |
+---------------------------------------------------------------------------------------------------+
| 4  | LSP Invariant Violation                | Subtype alters base contract; decouple hierarchies.  |
+---------------------------------------------------------------------------------------------------+
| 5  | Fragile Base Class Coupling            | Base mutation breaks derived logic; favor composition|
+---------------------------------------------------------------------------------------------------+
| 6  | Fat Interface Dependencies             | Forces unused method overrides; apply ISP contracts. |
+---------------------------------------------------------------------------------------------------+
| 7  | Omitting `noexcept` on Move Operations | Forces containers to fallback to deep copies.        |
+---------------------------------------------------------------------------------------------------+
```
