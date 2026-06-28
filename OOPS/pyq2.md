# Object-Oriented Programming (OOP) Master Interview Q&A — Part 2 (Q51–Q100) 🚀

This document compiles the remaining 50 of the 100 most-asked Object-Oriented Programming (OOP) and Software Design interview questions. It focuses heavily on deep-dive **SOLID Principles**, advanced compiler memory layouts (multiple inheritance offsets, VTABLE adjustments), resource management (smart pointers, RAII), concurrency models inside OOP (monitors, class locks), and compile-time optimizations (RVO, move semantics).

---

## Section 7: Deep Dives on SOLID Design Principles (Q51–Q65)

### Q51. What is the Open-Closed Principle (OCP)? How do we measure if a class violates OCP?
* **Asked by:** Meta, Google, Stripe, Uber
* **Answer:**
  * **OCP Rule:** Software components should be **open for extension, but closed for modification**. You should be able to introduce new features without altering existing, tested code.
  * **Measuring Violations (Code Smells):**
    1. **Type Checking conditional blocks:** The presence of `switch` statements or `if-else` chains checking class types (e.g., `if (type == "A") ... else if (type == "B")`). Adding a new type forces modifying the check code, directly violating OCP.
    2. **Cascading Code Modifications:** Adding a single feature forces you to open and edit multiple distinct classes.
  * **The Fix:** Introduce an abstraction layer (interfaces or abstract base classes). The calling code interacts with the interface, and new features are added by writing new classes implementing that interface.

---

### Q52. Explain the Liskov Substitution Principle (LSP) in detail. Why are covariance and contravariance relevant to LSP?
* **Asked by:** Meta, Stripe, Netflix
* **Answer:**
  * **LSP Rule:** Subtypes must be substitutable for their base types without altering the correctness or behavior of the program.
  * **Detailed Constraints:**
    * **Preconditions cannot be strengthened:** A subclass cannot require more strict input conditions than the parent.
    * **Postconditions cannot be weakened:** A subclass must guarantee at least the same output contracts as the parent.
    * **Invariants must be preserved:** Subclasses must maintain the internal state safety rules of the parent.
  * **Covariance & Contravariance:**
    * **Return Type Covariance:** An overriding subclass method can return a subtype of the return type declared in the parent method (weakening postconditions is not allowed, but specializing output is).
    * **Argument Contravariance:** An overriding method should theoretically be able to accept supertypes of the parent's arguments (accepting broader inputs), though most compiler languages do not support contravariance on overrides (it is treated as overloading instead).

---

### Q53. How does the Interface Segregation Principle (ISP) reduce binary recompilation dependencies in C++?
* **Asked by:** Google, Apple, Bloomberg (High-scale C++)
* **Answer:**
  * **ISP Rule:** Clients should not be forced to depend on interfaces containing methods they do not use.
  * **Binary Recompilation Problem:** In C++, if class `ClientA` depends on a large "fat" header interface `IMultiService.h` that contains dozens of methods (used by `ClientB` and `ClientC`), any modification to `IMultiService.h` (even changing a method signature only used by `ClientB`) triggers a compiler re-evaluation. The compiler must **recompile the translation units** of `ClientA` because the header file has changed.
  * **The ISP Fix:** Split the fat interface into smaller headers: `IServiceA.h`, `IServiceB.h`. `ClientA` only includes `IServiceA.h`. Modifying `IServiceB.h` does not affect `ClientA`'s compilation dependencies, reducing build times.

---

### Q54. Explain the Dependency Inversion Principle (DIP). Compare DIP, DI, and IoC.
* **Asked by:** Stripe, Netflix, Google
* **Answer:**
  * **DIP (Design Principle):** High-level policy classes should not depend on low-level detail classes. Both should depend on abstractions (interfaces).
  * **DI (Implementation Technique):** Dependency Injection is the mechanism of passing dependent resources into an object (via constructor, setter, or interface parameters) rather than letting the object instantiate them internally.
  * **IoC (Architectural Pattern):** Inversion of Control is the broader pattern of delegating control of execution flow and object creation lifecycle to a framework (e.g., Spring Container, NestJS) rather than hardcoding it in the application logic.

---

### Q55. How does the Single Responsibility Principle (SRP) relate to cohesion and coupling?
* **Asked by:** Amazon, Microsoft
* **Answer:**
  SRP is the architectural path to achieving **High Cohesion** and **Low Coupling**:
  * **Cohesion:** The degree to which elements inside a class belong together. A class with a single responsibility has **high cohesion** because all its variables and methods are tightly aligned toward achieving that single goal.
  * **Coupling:** The degree of dependency between distinct classes. A class that tries to do everything (low cohesion) connects itself to multiple databases, network sockets, and rendering layers, leading to **high coupling**. By breaking it down via SRP, each class is small, independent, and communicates through interfaces, creating a loosely coupled system.

---

### Q56. Explain the "Composition Over Inheritance" principle in relation to SOLID.
* **Asked by:** Uber, Stripe, Netflix
* **Answer:**
  * **OCP and LSP Violations:** Inheritance exposes a subclass to the parent's internal implementation details ("white-box reuse"), violating encapsulation. If the parent class changes, the child class can break (violating OCP). If a child overrides behavior in a way that breaks base invariants (e.g., Square inheriting from Rectangle), it violates LSP.
  * **Composition Advantage:** Composition binds classes through their public interface contracts ("black-box reuse"). The host class only knows *what* the component does, not *how* it does it, maintaining encapsulation and making the code OCP/LSP compliant.

---

### Q57. What is the relationship between the Law of Demeter and SOLID?
* **Asked by:** Netflix, Stripe, Google
* **Answer:**
  * **Law of Demeter (Principle of Least Knowledge):** A method of an object should only call methods of:
    1. The object itself.
    2. Arguments passed into the method.
    3. Objects created within the method.
    4. Direct component objects of the host class.
  * **LoD Violation:** `order.getCustomer().getAddress().getZipCode().validate()` (train wreck chain).
  * **SOLID Alignment:** Violating LoD causes tight coupling across multiple classes. If `ZipCode` changes, this code breaks. It violates SRP (the caller class is now responsible for navigating the entire object graph).

---

### Q58. How does the Interface Segregation Principle (ISP) affect unit testing and mocking?
* **Asked by:** Stripe, Backend roles
* **Answer:**
  * If a class depends on a "fat" interface containing 30 methods, writing a unit test requires you to construct a **mock object** that implements/stubs all 30 methods, even if the class under test only invokes a single method.
  * **With ISP:** The class depends on a small interface with 2 methods. Your test mock only needs to implement those 2 methods, making unit tests cleaner, simpler, and less fragile.

---

### Q59. Compare Interface Inheritance vs. Implementation Inheritance.
* **Asked by:** Meta, Google, Microsoft
* **Answer:**
  * **Interface Inheritance (Subtyping / IS-A-CAN-DO):** A class inherits only method signatures (interfaces). It inherits the *contract* but no code or state.
    * *SOLID Stance:* Strongly preferred. It decouples interface from implementation, promoting polymorphism and OCP.
  * **Implementation Inheritance (Subclassing / IS-A-REUSE):** A class inherits state and concrete methods from a parent class.
    * *SOLID Stance:* Cautious. It introduces tight coupling. If parent internals change, child behaviors can break.

---

### Q60. What are common code smells indicating a violation of SOLID principles?
* **Asked by:** Stripe, Netflix
* **Answer:**
  * **Rigidity:** A single change forces cascading modifications across dozens of classes (DIP/SRP violation).
  * **Fragility:** Modifying code in one place breaks behavior in an unrelated part of the system (SRP/LSP violation).
  * **Immobility (Low Reuse):** Code cannot be reused in another module because it is tightly bound to its dependencies (DIP violation).
  * **Tangled Conditional logic:** Extensive `instanceof` checks or `switch` statements checking subclass types (OCP violation).

---

### Q61. How does the Dependency Inversion Principle (DIP) facilitate unit testing?
* **Asked by:** Salesforce, Bloomberg
* **Answer:**
  * If a high-level `PaymentService` class directly instantiates a concrete `StripeApi` client, testing the service is impossible without hitting the real Stripe API network endpoints.
  * **DIP Application:** `PaymentService` depends on an abstract `IPaymentGateway` interface.
  * **Testing Benefit:** In production, we inject `StripeApi`. In unit tests, we inject a lightweight `MockPaymentGateway` that returns stubbed success/failure responses instantly without network dependencies.

---

### Q62. Explain the Hollywood Principle ("Don't call us, we'll call you") in OOP.
* **Asked by:** Microsoft, Oracle
* **Answer:**
  * The **Hollywood Principle** is another term for **Inversion of Control (IoC)**.
  * In traditional code, your application class controls execution flow: it decides when to instantiate database connections and when to call library functions.
  * Under the Hollywood Principle, low-level components register themselves with a high-level framework (e.g., event loops, Web APIs). The framework calls the low-level components when specific events occur (e.g., HTTP request arrives). The control flow is inverted.

---

### Q63. Can applying SOLID principles lead to over-engineering? Give a scenario.
* **Asked by:** Meta, Stripe (Pragmatic design questions)
* **Answer:**
  * **Yes.** Over-application of SOLID can lead to excessive abstraction, making code hard to trace and navigate.
  * **Scenario:** A simple utility class that writes logs to a local file.
    * *Over-engineered SOLID path:* Creating `ILogger` interface, `FileWriter` interface, `FileStreamManager` interface, injecting concrete classes via an IoC container, and creating factory handlers.
    * *Pragmatic path:* If the project is small and logs will only ever go to a local file, a single, straightforward `Logger` class is simpler, more readable, and faster to ship. Only abstract when variability is a concrete requirement.

---

### Q64. Compare DRY (Don't Repeat Yourself) vs. SOLID. Do they ever conflict?
* **Asked by:** Google, Amazon
* **Answer:**
  * **DRY** focuses on eliminating duplication of knowledge/representation in a system.
  * **SOLID** focuses on structural design, maintainability, and loose coupling.
  * **Conflict (Accidental Duplication):** Sometimes, two separate domains have identical code structures by coincidence (e.g., user validation logic and product validation logic look similar).
    * A dogmatic adherence to **DRY** would force you to abstract this into a shared validation base class.
    * However, **SOLID (SRP)** dictates that these classes serve different business actors. Merging them creates coupling; if user rules change, the shared validation base modifies both user and product contexts, violating SRP. Here, duplication is preferred to preserve decoupling.

---

### Q65. What is the Interface Segregation Principle (ISP) vs. Single Responsibility Principle (SRP)?
* **Asked by:** Google, Meta
* **Answer:**
  * **SRP** is focused on the **class implementation boundaries** and actors. A class should have one reason to change.
  * **ISP** is focused on the **client-facing interface boundaries**. It ensures clients are not exposed to interface contracts they do not care about.
  * **Relationship:** You can violate ISP while maintaining SRP. A class can have a single responsibility (e.g., database connection manager), but if its interface exposes raw low-level connection tuning methods to simple reader clients, it violates ISP.

---

## Section 8: Advanced OOP Architecture & Memory Models (Q66–Q85)

### Q66. What is Runtime Type Identification (RTTI) in C++? How does dynamic_cast work?
* **Asked by:** Microsoft, Bloomberg, Apple (High-frequency internals)
* **Answer:**
  * **RTTI** is a compiler mechanism that exposes information about an object's data type at runtime. It is only generated for classes containing virtual functions.
  * **How dynamic_cast works:**
    * When converting a base pointer to a derived pointer (`dynamic_cast<Derived*>(base_ptr)`), the compiler checks the RTTI metadata associated with the object.
    * It traverses the virtual table (VTABLE) structures to verify if the physical object is indeed a `Derived` class instance (or its subtype).
    * If valid, it returns the adjusted pointer. If invalid, it returns `nullptr` (for pointers) or throws `std::bad_cast` (for references). This requires runtime overhead compared to `static_cast` (which does no runtime checks).

---

### Q67. What is Object Slicing in C++? How do you prevent it?
* **Asked by:** Google, Bloomberg, EA (C++ specific)
* **Answer:**
  * **Object Slicing** occurs when a derived class object is assigned **by value** to a base class object.
  * **Why:** The compiler allocates a fixed memory footprint for the base class. When copying the derived object, it "slices off" all the derived-specific member variables and VTABLE associations, keeping only the base portion.
  * **Example:**
    ```cpp
    Base b = Derived(); // Object Slicing occurs! b behaves like Base.
    ```
  * **Prevention:** Pass objects by **reference** or **pointer** (`Base &b = derived_obj;`), which preserves polymorphic behavior using pointer redirections.

---

### Q68. What are Smart Pointers in C++? Explain unique_ptr, shared_ptr, and weak_ptr.
* **Asked by:** Google, Amazon, Microsoft, Apple
* **Answer:**
  Smart pointers are wrapper classes implementing the **RAII** pattern to manage heap memory lifecycles automatically:
  * **`std::unique_ptr`:** Enforces **exclusive ownership** of a heap resource. It cannot be copied, only moved. Deallocates memory when it goes out of scope.
  * **`std::shared_ptr`:** Enforces **shared ownership**. It maintains a thread-safe **reference counter** block. Copying increments the counter; going out of scope decrements it. When the counter reaches 0, the resource is deleted.
  * **`std::weak_ptr`:** Holds a non-owning reference to an object managed by `shared_ptr`. It does not increment the reference counter, preventing cyclic references. It must be converted to a `shared_ptr` (via `.lock()`) to access the resource.

---

### Q69. How does std::weak_ptr resolve cyclic dependency memory leaks?
* **Asked by:** Uber, Stripe, Netflix
* **Answer:**
  * **Cyclic Dependency:** If Object A holds a `shared_ptr` to Object B, and Object B holds a `shared_ptr` to Object A, their reference counts will always be at least 1. Even if all external pointers are deleted, they will never be destroyed, causing a permanent memory leak.
  * **Resolution:** Break the cycle by changing one pointer direction to a `std::weak_ptr` (e.g., Parent holds `shared_ptr` to Child, but Child holds `weak_ptr` to Parent). This keeps the Parent's reference count clean, allowing proper cleanup cascade.

---

### Q70. How does a compiler layout memory for Multiple Inheritance?
* **Asked by:** Core C++ roles, Game engines, Compiler devs
* **Answer:**
  * Under multiple inheritance, a derived class object must contain the layouts of all its parent classes physically aligned.
  * **Pointer Adjustment:**
    If `Class C` inherits from `Class A` and `Class B`:
    ```cpp
    C* c_ptr = new C();
    A* a_ptr = c_ptr; // Points to start of C object (matching A layout)
    B* b_ptr = c_ptr; // Adjusted forward by sizeof(A) to align with B layout!
    ```
    The compiler automatically inserts code to add an **offset adjustment** to the pointer address during assignment to match the memory layout of parent `B`.

---

### Q71. What are Return Value Optimization (RVO) and Copy Elision?
* **Asked by:** Google, High-performance C++ roles
* **Answer:**
  * **Copy Elision** is a compiler optimization technique where the compiler avoids calling copy and move constructors, constructing the return value directly in the memory block allocated for the caller.
  * **RVO (Return Value Optimization):** A specific type of copy elision. When a function returns a local object by value, the compiler constructs the object directly in the destination memory frame, bypassing temporary object creation entirely. In modern C++ (C++17 onwards), copy elision is **guaranteed** by standard specification.

---

### Q72. Explain Move Semantics and rvalue references (&&) in C++.
* **Asked by:** Google, Bloomberg, Microsoft
* **Answer:**
  * **rvalue reference (`&&`):** A reference type that binds to temporary objects (rvalues) that are about to be destroyed.
  * **Move Semantics:** Instead of performing a deep copy of a temporary object's resources (which is slow), move semantics allow an object to **steal/take ownership of resources** (pointers, file descriptors) from the temporary object.
  * **Performance:** Reduces copy overhead to $O(1)$ pointer swaps.
  * *Example:*
    ```cpp
    // Move Constructor
    Vector(Vector&& source) noexcept {
        this->data = source.data; // steal pointer
        source.data = nullptr;    // clear source pointer
    }
    ```

---

### Q73. What is the role of std::move and std::forward?
* **Asked by:** Core C++ engineering roles
* **Answer:**
  * **`std::move`:** A static cast that converts an lvalue (named variable) into an rvalue reference (`&&`), indicating to the compiler that the variable's resources can be safely stolen. It does not physically move anything at runtime.
  * **`std::forward`:** Used in template designs to implement **Perfect Forwarding**. It preserves the original value category (lvalue or rvalue) of passed arguments, routing them correctly to overloaded move or copy parameters.

---

### Q74. How does the Java Garbage Collector handle circular references?
* **Asked by:** Java developers
* **Answer:**
  * Early garbage collectors used **Reference Counting** (which fails on circular references because counts never hit 0).
  * Modern JVMs use **Reachability Analysis**:
    * The GC starts from a set of root objects called **GC Roots** (e.g., active thread local variables, static references, JNI references).
    * It traverses the object reference graph. If an object cannot be reached by any path starting from the GC Roots, it is classified as unreachable and swept, even if circular references exist among the isolated nodes.

---

### Q75. What is a Memory Leak in a Garbage-Collected language like Java?
* **Asked by:** Uber, Stripe, Netflix
* **Answer:**
  * In garbage-collected languages, a memory leak occurs when an application maintains references to objects that are **no longer needed by the business logic**, preventing the GC from reclaiming them.
  * **Classic Cause (Lapsed Listener):** Registering an observer or listener to a static subject, but forgetting to deregister it. The static object keeps a reference to the observer, leaking the observer and all its child references.

---

### Q76. Compare final (Java), const (C++), and readonly (C#).
* **Asked by:** Multi-language system roles
* **Answer:**
  * **`final` (Java):** Variables cannot be reassigned after initialization. For reference objects, the *pointer address* is locked, but the object's internal properties can still be modified.
  * **`const` (C++):** Enforces deep, compile-time immutability. If an object is const, you cannot modify its fields or call non-const member methods.
  * **`readonly` (C#):** Fields can only be assigned during instantiation (within constructors). Similar to Java's final.

---

### Q77. If Virtual Constructors don't exist in C++, how do we implement their behavior?
* **Asked by:** Google, Microsoft
* **Answer:**
  We achieve this behavior using the **Virtual Clone Pattern** (or Prototype pattern).
  * **How:** Define a virtual method `clone()` in the base class, and implement it in derived classes to return a new heap instance copy of the specific derived object.
  ```cpp
  virtual Base* clone() const { return new Derived(*this); }
  ```

---

### Q78. Compare Static Binding and Dynamic Binding at the Assembly level.
* **Asked by:** Core Systems, Apple, Bloomberg
* **Answer:**
  * **Static Binding:** Compiled to a direct call instruction to a hardcoded memory address:
    `call 0x00401080` (fast execution, instruction cache pre-fetchable).
  * **Dynamic Binding:** Compiled to indirect lookup:
    1. Read object address.
    2. Dereference to locate `vptr`.
    3. Read VTABLE offset address.
    4. Call register: `call eax` (slower, introduces pipeline stall risks).

---

### Q79. Why should virtual functions not be called inside C++ Constructors/Destructors?
* **Asked by:** Google, Microsoft, Bloomberg
* **Answer:**
  * In C++, during the constructor execution of a `Base` portion, the `Derived` portion has not yet been built.
  * If the base constructor calls a virtual function `show()`, the compiler bypasses dynamic binding and calls the **Base class version** of `show()`, not the Derived version.
  * During destructors, the Derived portion is destroyed *first*. Calling a virtual function inside the base destructor would attempt to access deleted derived resources, leading to crashes.

---

### Q80. How do virtual functions behave inside constructors in Java vs. C++?
* **Asked by:** Microsoft, Amazon (Java comparison)
* **Answer:**
  * **In Java, virtual calls inside constructors resolve polymorphically** to the subclass implementation.
  * **The Danger:** If a parent constructor calls overridden method `init()`, and the subclass `init()` initializes subclass-specific fields, it executes *before* the subclass constructor body has run. Subclass variables will be uninitialized (e.g., null or 0), leading to dangerous `NullPointerException` bugs.

---

### Q81. Explain Covariant Return Types in method overriding.
* **Asked by:** Google, Oracle
* **Answer:**
  * **Covariant Return Type:** Allows an overriding method in a subclass to return a more specialized subtype than the return type declared in the parent class method.
  * **Example:**
    ```cpp
    class BaseFactory { virtual Base* create(); };
    class DerivedFactory : public BaseFactory { Derived* create() override; };
    ```
    This is valid because `Derived*` is castable to `Base*`, maintaining the contract while avoiding casting at the call site.

---

### Q82. Compare Static and Non-Static Inner Classes in Java.
* **Asked by:** Java roles
* **Answer:**
  * **Non-Static Inner Class:** Maintains an implicit reference to the outer class instance that created it. It can access all outer fields (even private ones).
    * *Cons:* Can cause memory leaks (outer class cannot be garbage collected if the inner class object survives).
  * **Static Inner Class:** Does not have an implicit reference to an outer class instance. It behaves like a top-level class nested only for packaging/namespace organization.

---

### Q83. What is Object Slicing and how does it relate to Value Semantics?
* **Asked by:** Core systems roles
* **Answer:**
  Object slicing is a direct consequence of C++'s **Value Semantics** (objects are stored directly as block values). Java and Python use **Reference Semantics** (variables store pointers to heap structures), which makes object slicing impossible in those languages, though reference management overhead is higher.

---

### Q84. What is a Friend Class in C++? Does it violate encapsulation?
* **Asked by:** C++ roles
* **Answer:**
  * A class declared with the `friend` keyword gets access to another class's private members.
  * **Encapsulation impact:** It does not violate encapsulation if used correctly. Since friendship must be explicitly granted *by the class itself*, it is a deliberate control choice. It is useful for implementing patterns like Iterator without exposing public accessors.

---

### Q85. Explain how alignment and padding affect object size in memory.
* **Asked by:** Google, Game developers, Embedded systems
* **Answer:**
  To optimize memory bus reads, compilers align member variables to boundaries matching their size (e.g., a 4-byte `int` must start at an address divisible by 4). The compiler inserts unused bytes called **Padding** to align fields.
  * **Example:**
    ```cpp
    struct A { char a; int b; char c; }; // size = 12 bytes (padding after a and c)
    struct B { int b; char a; char c; }; // size = 8 bytes (reordered to minimize padding)
    ```

---

## Section 9: Threading & Concurrency in OOP Models (Q86–Q100)

### Q86. Compare Object-Level Locking vs. Class-Level Locking in Java.
* **Asked by:** Uber, Stripe, Java backend roles
* **Answer:**
  * **Object-Level Locking:** Uses synchronized non-static methods or blocks (`synchronized(this)`). It locks the specific instance object, blocking other threads from accessing other synchronized methods of the *same instance*.
  * **Class-Level Locking:** Uses synchronized static methods or blocks (`synchronized(MyClass.class)`). It locks the `Class` object metadata, blocking all threads from accessing synchronized static methods across *all instances* of that class.

---

### Q87. What is a Monitor in OOP concurrency?
* **Asked by:** Google, Microsoft, Oracle
* **Answer:**
  * A **Monitor** is an object-oriented synchronization construct that encapsulating data and mutexes.
  * **Internal structure:** Contains private variables, public synchronized methods, a lock, and **condition variables** (wait sets).
  * **Rules:** Only one thread can be active inside the monitor at any given time. If a thread must wait for a condition, it releases the lock and waits on a condition queue (`wait()`), allowing other threads to enter until they signal (`notify()`).

---

### Q88. How does encapsulation assist in writing thread-safe classes?
* **Asked by:** Stripe, Netflix
* **Answer:**
  * Thread safety requires managing access to mutable shared state.
  * Encapsulation allows you to hide class variables behind private scopes. Because external callers cannot modify fields directly, you can wrap all state transitions within public synchronized methods (e.g., protecting a queue with locks), guaranteeing invariant state safety.

---

### Q89. How do you design an Immutable Object? Why is it thread-safe?
* **Asked by:** Uber, Stripe, Amazon
* **Answer:**
  * **Design Rules:**
    1. Declare all fields as `private` and `final` (or `const`).
    2. Do not provide setter methods.
    3. Ensure the class cannot be inherited (use `final` or `sealed`).
    4. If fields contain references to mutable objects, use deep copying during constructor assignment and getter returns (defensive copying).
  * **Why thread-safe:** Since the object's state can never change after creation, multiple threads can read it concurrently without locks, eliminating data race conditions.

---

### Q90. What is ThreadLocal storage? How does it relate to object state?
* **Asked by:** Java/C# backend roles
* **Answer:**
  * **ThreadLocal** provides thread-local variables. Each thread accessing a ThreadLocal variable has its own independent, initialized copy.
  * **Relation to state:** It bypasses standard object state sharing. Instead of locking a shared database connection instance, each thread stores its own connection object in ThreadLocal, eliminating locking overhead.

---

### Q91. What is the Active Object design pattern?
* **Asked by:** Multi-threaded systems roles
* **Answer:**
  * The **Active Object** pattern decouples method execution from method invocation.
  * **How:** The client calls a method on a proxy. The proxy packages the call into a request object and places it in an activation queue. A scheduler thread reads from the queue and executes the task asynchronously. This prevents the client thread from blocking.

---

### Q92. What is a Reentrant Lock? Why are standard mutexes made reentrant?
* **Asked by:** Uber, Stripe, Backend roles
* **Answer:**
  * A **Reentrant Lock** allows the thread currently holding the lock to acquire it again without blocking itself. It maintains an acquisition counter.
  * **Why:** To prevent self-deadlocks. If method `foo()` acquires a lock and calls `bar()` (which also tries to acquire the same lock), a standard non-reentrant mutex would block the thread indefinitely. A reentrant lock increments the count and proceeds.

---

### Q93. How does exception handling in C++ affect object destruction (Stack Unwinding)?
* **Asked by:** Bloomberg, Google, Apple
* **Answer:**
  * When an exception is thrown, the runtime performs **Stack Unwinding**: it exits the active function scopes sequentially to locate a catch block.
  * **Object cleanup:** During this exit, C++ guarantees that all stack-allocated objects in the unwound frames have their destructors called. This guarantees no leaks if resource management uses RAII.

---

### Q94. Explain the Exception Safety Guarantees.
* **Asked by:** Google, Bloomberg, Adobe
* **Answer:**
  1. **Basic Guarantee:** If an exception occurs, no memory is leaked and state remains valid (though it may contain unexpected values).
  2. **Strong Guarantee:** Transactional semantics: if an exception occurs, the state is rolled back to the exact state it was in before the call (commit or rollback).
  3. **No-throw (Nothrow) Guarantee:** The function is guaranteed to succeed and will never throw an exception (marked `noexcept` in C++).

---

### Q95. Explain the Copy-and-Swap Idiom in C++.
* **Asked by:** High-end C++ roles
* **Answer:**
  * The **Copy-and-Swap Idiom** is used to implement the Assignment Operator (`operator=`) providing a **Strong Exception Safety** guarantee.
  * **How it works:**
    1. Pass the source object by value (making a copy; if this copy throws, the original state is unaffected).
    2. Swap the internal pointer members of the copy with the `this` object.
    3. As the copy goes out of scope, its destructor cleans up the old resources.
  ```cpp
  Widget& operator=(Widget copy) { // pass by value (copy)
      swap(*this, copy); // swap internals
      return *this;
  }
  ```

---

### Q96. What is the Object Pool Pattern? When is it used?
* **Asked by:** Game systems (Rockstar, EA), High-Scale engines
* **Answer:**
  * **Object Pool:** Maintains a set of initialized objects in memory. When needed, the client "borrows" an object, uses it, and returns it to the pool instead of destroying it.
  * **When used:** When object instantiation overhead is extremely high (e.g., DB connections, threads, graphics particles) and garbage collection overhead from constant creation/destruction causes latency spikes.

---

### Q97. Explain Double Dispatch (Visitor Pattern context).
* **Asked by:** Google, Microsoft
* **Answer:**
  * Standard OOP languages support **Single Dispatch**: the method executed is determined by the runtime type of a single object (the caller).
  * **Double Dispatch** resolves method calls based on the runtime types of **two objects** (the caller and the argument). It is implemented by using a double virtual call hook (e.g., caller calls `accept(visitor)`, which in turn calls `visitor->visit(this)`), ensuring both types are resolved dynamically.

---

### Q98. Why do some gaming engines disable RTTI?
* **Asked by:** Game studios (Epic Games, EA, Unity roles)
* **Answer:**
  * RTTI requires compilers to store type metadata in the binary, increasing build sizes.
  * More importantly, type checks (`dynamic_cast`) require traversing type hierarchies at runtime, which is slow and violates the strict frame budget constraints (e.g., 60 FPS = 16.6ms per frame). Game developers prefer static casts or custom enum-based type checking tags.

---

### Q99. What is the Lapsed Listener problem?
* **Asked by:** Java / C# roles
* **Answer:**
  * A specific type of memory leak where a publisher/subject holds a strong reference to an observer/listener.
  * If the observer goes out of use in the application but remains registered to the publisher, the GC cannot collect it because the publisher is still reachable.
  * **Fix:** Use **Weak References** (`WeakReference` in C# / Java) for the listener list so the GC can clean them up.

---

### Q100. How does a compiler structure virtual base class offsets?
* **Asked by:** Core C++ compiler roles
* **Answer:**
  * To resolve the diamond problem via virtual inheritance, the compiler cannot align the virtual base portion at a fixed physical offset inside the derived class layout.
  * **VBPTR:** The compiler inserts a **Virtual Base Pointer (vbptr)** into the object.
  * **Resolution:** At runtime, the code accesses the `vbptr` to read the offset index of the virtual base portion dynamically. This introduces a slight pointer dereferencing penalty for accessing base class variables.
