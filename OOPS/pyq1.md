# Object-Oriented Programming (OOP) Master Interview Q&A — Part 1 (Q1–Q50) 🚀

This document compiles the first 50 of the 100 most-asked theoretical and practical Object-Oriented Programming (OOP) and Design interview questions. These questions are tailored to the style of top-tier companies (Google, Meta, Microsoft, Amazon, Uber, Stripe, etc.), focusing on core pillars, memory management, compile-time/run-time polymorphism internals, and SOLID principles. Design patterns and LLD coding problems are excluded.

---

## Section 1: OOP Core Pillars & Concepts (Q1–Q11)

### Q1. What is OOP and how does it differ from Procedural Programming?
* **Asked by:** Google, Amazon, Microsoft
* **Answer:**
  * **Procedural Programming:** Focuses on writing functions or procedures that operate on data. Data and functions are separate, and execution follows a top-down logical flow. Key languages: C, Fortran.
  * **Object-Oriented Programming (OOP):** Focuses on creating **Objects** that combine both **data** (attributes) and **behavior** (methods) into a single unit. It models real-world entities. Key languages: C++, Java, C#, Python.
  * **Differences:**
    * **Data Security:** OOP offers data hiding (via encapsulation), whereas data in procedural programming is often globally accessible and vulnerable.
    * **Approach:** Procedural is top-down; OOP is bottom-up.
    * **Code Reuse:** OOP provides inheritance and polymorphism to prevent code repetition.

---

### Q2. Explain the four main pillars of OOP with real-world analogies.
* **Asked by:** Meta, Microsoft, Salesforce
* **Answer:**
  1. **Encapsulation (Data Binding):** Bundling data and the methods that operate on that data into a single class, restricting direct access.
     * *Analogy:* A medical capsule. The medicine (data) is safe inside, and you can only interact with it by swallowing it (public methods).
  2. **Abstraction (Hiding Complexity):** Showing only essential features and hiding the complex implementation details.
     * *Analogy:* Driving a car. You interact with the steering wheel and pedals (public interface) without needing to understand the combustion engine internals (implementation).
  3. **Inheritance (Reusability):** Creating new classes (child) from existing classes (parent) to inherit their characteristics.
     * *Analogy:* Genetically inheriting traits (eye color, height) from parents while having your own unique personality.
  4. **Polymorphism (Many Forms):** The ability of a message, function, or object to take on multiple forms depending on the context.
     * *Analogy:* The word "Run". A person runs, water runs, and a computer program runs—the action differs depending on the entity performing it.

---

### Q3. What is the difference between a Class and an Object?
* **Asked by:** Amazon, Adobe, Walmart
* **Answer:**
  * **Class:** A user-defined data type that acts as a **blueprint or template** for creating objects. It occupies **no memory space** at runtime. It defines the structure (attributes and methods).
  * **Object:** An **instance of a class**. It is a physical entity that **occupies memory space** at runtime. It holds actual data values.
  * *Code Example (C++):*
    ```cpp
    class Car { public: string model; }; // Class (blueprint, no memory)
    Car myCar; // Object (instance, occupies memory)
    ```

---

### Q4. What is Encapsulation? How does it differ from Data Hiding?
* **Asked by:** Uber, Stripe, Amazon
* **Answer:**
  * **Encapsulation:** The mechanical process of binding data and methods together inside a class. It is the outer protective shell.
  * **Data Hiding:** The security concept of restricting direct access to an object's internal variables (e.g., using `private` access modifiers and exposing access only via `public` getter/setter methods).
  * **Summary:** Encapsulation is the *packaging* of data; Data Hiding is the *restriction* of access to that package. You can have encapsulation without data hiding (e.g., if all class members are public), but you cannot have data hiding without encapsulation.

---

### Q5. What is Abstraction? How is it achieved in C++ and Java?
* **Asked by:** Meta, Google, Apple
* **Answer:**
  * **Abstraction** focus on *what* an object does rather than *how* it does it. It simplifies system design by hiding background implementation details.
  * **Implementation:**
    * **In C++:** Achieved using **Abstract Classes** (classes containing at least one Pure Virtual Function) and header files (`.h` exposing interfaces, `.cpp` hiding implementation).
    * **In Java:** Achieved using **Abstract Classes** and **Interfaces** (which define pure method contracts).

---

### Q6. Explain the difference between Abstraction and Encapsulation.
* **Asked by:** Google, Netflix, Microsoft
* **Answer:**
  * **Abstraction** is a design-level concept focused on **hiding complexity** by exposing only essential contracts. It answers: *"What does the system expose?"*
  * **Encapsulation** is an implementation-level concept focused on **binding data and hiding access** to secure state. It answers: *"How does the system protect its data?"*
  * **Complementary Nature:** You use encapsulation to *implement* abstraction.

---

### Q7. What are Access Modifiers? Compare public, private, and protected scopes.
* **Asked by:** Salesforce, Oracle, Microsoft
* **Answer:**
  Access modifiers define the visibility and accessibility of class members (attributes and methods):
  * **`private`**: Accessible only within the class itself. Derived classes cannot access private members of the parent.
  * **`protected`**: Accessible within the class itself and by **derived child classes**. External classes cannot access them.
  * **`public`**: Accessible from anywhere in the program where the object is visible.

---

### Q8. What is a Constructor? Explain Default, Parameterized, and Copy Constructors.
* **Asked by:** Goldman Sachs, Amazon, Zepto
* **Answer:**
  A **Constructor** is a special member function automatically invoked when an object is instantiated. It shares the same name as the class and has no return type.
  * **Default Constructor:** Takes no arguments; initializes variables to default values.
  * **Parameterized Constructor:** Takes arguments to initialize object attributes with custom values at instantiation.
  * **Copy Constructor:** Instantiates a new object by copying the attributes of an existing object of the same class.

---

### Q9. What is a Destructor? Why does C++ have destructors while Java/Python do not?
* **Asked by:** Google, Amazon, Bloomberg
* **Answer:**
  * A **Destructor** is a special member function called automatically when an object's lifetime ends (goes out of scope or is deleted). It deallocates memory and releases resource handles (files, database connections).
  * **Language Differences:**
    * **C++** uses manual memory management. When an object is deleted, C++ must run the destructor immediately to prevent memory leaks.
    * **Java/Python** use an automatic **Garbage Collector** to manage memory. Objects are not destroyed immediately when they go out of scope; the Garbage Collector deallocates them in the background, making explicit destructors unnecessary (Java uses `finalize()`—now deprecated—or `try-with-resources`).

---

### Q10. Why must the parameter to a Copy Constructor be passed by reference?
* **Asked by:** Google, Microsoft, Adobe (C++ interviews)
* **Answer:**
  * A copy constructor must take its argument by reference (e.g., `Car(const Car &source)`).
  * **The Trap:** If you pass the object by value (e.g., `Car(Car source)`), compiler semantics dictate that passing by value requires creating a temporary copy of the argument.
  * **Infinite Recursion:** To create that copy, the compiler would need to call the copy constructor again. This leads to an **infinite recursive chain of copy constructor calls**, which eventually crashes the program with a **Stack Overflow** error.

---

### Q11. Explain the difference between Shallow Copy and Deep Copy.
* **Asked by:** Uber, Stripe, Netflix, Zepto
* **Answer:**
  When copying an object that contains pointers to dynamically allocated memory:
  * **Shallow Copy:** Copies member values directly. For pointer members, it copies the **memory address (pointer value)**, meaning both the original and copied objects point to the same memory location on the heap.
    * *Danger:* If one object modifies the heap value, it affects the other. If one object is deleted, it deallocates the heap memory, leaving the other object with a **dangling pointer** (double free corruption error).
  * **Deep Copy:** Allocates brand new memory on the heap for the copy, and copies the actual values pointed to. Both objects maintain independent heap allocations.

```cpp
// Shallow Copy
this->ptr = source.ptr;

// Deep Copy
this->ptr = new int(*source.ptr);
```

---

## Section 2: Polymorphism Internals (Q12–Q19)

### Q12. Explain Compile-time (Static) vs. Run-time (Dynamic) Polymorphism.
* **Asked by:** Microsoft, Google, Intel
* **Answer:**
  * **Compile-time (Static) Polymorphism:** The compiler determines which function to execute at compile time based on parameter signatures.
    * *Mechanisms:* Method Overloading, Operator Overloading.
    * *Performance:* Extremely fast; no runtime overhead (Early Binding).
  * **Run-time (Dynamic) Polymorphism:** The exact method to execute is determined at runtime based on the actual type of the object, not the reference pointer.
    * *Mechanisms:* Method Overriding, Virtual Functions.
    * *Performance:* Slightly slower due to lookup overhead (Late Binding / Virtual Table).

---

### Q13. What is Method Overloading? Can we overload by changing only the return type?
* **Asked by:** Amazon, Flipkart, Walmart
* **Answer:**
  * **Method Overloading** allows multiple functions in the same scope to share the same name with different parameter signatures (different parameter counts, types, or order).
  * **Return Type Trap:** **No**, you cannot overload a method by changing only its return type.
  * **Why:** When resolving function calls (e.g., `calc(5, 10)`), the compiler relies on the argument list to determine which function matches. The return type is not part of the function signature used for resolution, making it impossible for the compiler to distinguish them, resulting in a compile-time redeclaration error.

---

### Q14. What is Method Overriding? What are its rules?
* **Asked by:** Meta, Stripe, Netflix
* **Answer:**
  * **Method Overriding** allows a child class to provide a specific implementation of a method that is already defined in its parent class.
  * **Rules:**
    1. The method in the child class must share the exact same name, return type, and parameter list as the parent method.
    2. The parent method must be marked as virtual (in C++) or not be marked final/static (in Java).
    3. The overriding method cannot narrow the access modifier (e.g., a `public` parent method cannot be overridden as `private` in the child).

---

### Q15. What are Virtual Functions? How do they enable run-time polymorphism?
* **Asked by:** Google, Microsoft, Adobe (C++ Core)
* **Answer:**
  * A **Virtual Function** is a member function in a base class declared with the `virtual` keyword, which tells the compiler to defer binding until runtime.
  * **How it works:** When a base class pointer points to a derived class object, calling a virtual function executes the **derived class's version** of the function, rather than the base class's version.

```cpp
Base* ptr = new Derived();
ptr->show(); // If show() is virtual, calls Derived::show(); else calls Base::show()
```

---

### Q16. Explain the internal working of Virtual Functions (VTABLE and VPTR).
* **Asked by:** Google, Microsoft, Bloomberg, Apple (High-frequency C++ internals)
* **Answer:**
  Run-time polymorphism in C++ is implemented using a **Virtual Table (VTABLE)** and a **Virtual Pointer (VPTR)**:
  1. **VTABLE:** For every class containing at least one virtual function, the compiler constructs a static table called the VTABLE. This table stores **function pointers** pointing to the implementations of the virtual functions for that class.
  2. **VPTR:** When an object of a class with virtual functions is instantiated, the compiler inserts a hidden pointer member (the `vptr`) pointing to the class's VTABLE.
  3. **Resolution:** When calling `ptr->virtual_func()`, the program dereferences the object's `vptr` to locate the class VTABLE, looks up the function pointer index, and jumps to the correct implementation. This introduces a slight pointer indirection overhead at runtime.

```
Object Instance          VTABLE (Derived)
+----------+             +-----------------------+
|  _vptr   | ----------> | [0] &Derived::func1() |
+----------+             | [1] &Derived::func2() |
|  data1   |             +-----------------------+
+----------+
```

---

### Q17. What is a Pure Virtual Function and an Abstract Class?
* **Asked by:** Oracle, Salesforce, Amazon
* **Answer:**
  * **Pure Virtual Function:** A virtual function in a base class that has no implementation and is declared with `= 0` (e.g., `virtual void draw() = 0;`).
  * **Abstract Class:** A class containing at least one Pure Virtual Function.
  * **Contracts:** Abstract classes serve as architectural blueprints. Any derived concrete class must override and implement all pure virtual functions of the base abstract class, or it will also be classified as an abstract class.

---

### Q18. Can an Abstract Class have a constructor? Can we instantiate it?
* **Asked by:** Google, Amazon, Microsoft
* **Answer:**
  * **Instantiation:** **No**, you cannot create an instance of an abstract class directly (e.g., `Base b;` is invalid) because it has incomplete implementations.
  * **Constructor:** **Yes**, an abstract class can (and often should) have a constructor.
  * **Why:** Although you cannot instantiate the abstract class directly, it is instantiated as part of the base portion when a derived concrete class object is created (derived constructor calls the base constructor). The abstract class constructor initializes the base class attributes.

---

### Q19. What is an Interface? How does it differ from an Abstract Class?
* **Asked by:** Meta, Stripe, Netflix, Java/C# interviews
* **Answer:**
  An **Interface** is a pure contract containing only method signatures without state or implementation (though modern Java allows default methods).

| Feature | Abstract Class | Interface |
| :--- | :--- | :--- |
| **Multiple Inheritance** | Classes can inherit only one abstract class. | Classes can implement multiple interfaces. |
| **State** | Can contain instance variables (fields). | Cannot contain instance state (fields must be `static final`). |
| **Methods** | Can have a mix of abstract and concrete methods. | Methods are implicitly public and abstract. |
| **Constructors** | Can have constructors. | Cannot have constructors. |

---

## Section 3: Inheritance & Associations (Q20–Q22)

### Q20. Why does C++ support Multiple Inheritance, while Java does not?
* **Asked by:** Microsoft, Oracle, Salesforce
* **Answer:**
  * **C++** supports multiple inheritance (inheriting from more than one class) to offer design flexibility, placing the responsibility of resolving ambiguities on the developer.
  * **Java** chose simplicity and safety. It blocked multiple class inheritance to prevent:
    1. **The Diamond Problem** (ambiguity of duplicate base classes).
    2. **Class-loading complexities** and casting issues.
    * *Java Solution:* Achieved multiple inheritance of interfaces (a class can implement multiple interfaces), which is safe because interfaces do not hold conflicting state.

---

### Q21. Explain the Diamond Problem in Inheritance. How is it resolved in C++?
* **Asked by:** Google, Amazon, Bloomberg
* **Answer:**
  * **The Diamond Problem:** Occurs when class `D` inherits from both `B` and `C`, which in turn both inherit from a single base class `A`.
  * **Ambiguity:** Class `D` receives two distinct physical copies of class `A`'s attributes (one via `B`, one via `C`). Calling a method of `A` from an object of `D` causes a compiler error due to ambiguity (`A::method` is ambiguous).
  * **C++ Resolution (Virtual Inheritance):**
    Declare the intermediate inheritance as `virtual`:
    ```cpp
    class B : virtual public A { ... };
    class C : virtual public A { ... };
    class D : public B, public C { ... };
    ```
    `virtual` tells the compiler to instantiate only **one shared copy** of base class `A` inside derived class `D`, resolving the ambiguity.

---

### Q22. Compare Association, Aggregation, and Composition.
* **Asked by:** Uber, Stripe, Netflix (Object-Oriented Design)
* **Answer:**
  These model relationships between objects, ranging from weak to strong:
  1. **Association:** A general relationship where objects have independent lifecycles (e.g., Doctor and Patient. A doctor has patients, but both can exist independently).
  2. **Aggregation (HAS-A - Weak):** A specialized association modeling a parent-child relationship where the child can exist independently of the parent. (e.g., Department and Teacher. If the department is deleted, the teacher still exists).
  3. **Composition (HAS-A - Strong):** A highly restrictive relationship where the child **cannot exist** without the parent. The parent owns the lifecycle of the child. (e.g., House and Room. If the house is deleted, the rooms are destroyed).

---

## Section 4: SOLID Design Principles (Q23–Q27)

### Q23. Explain the Liskov Substitution Principle (LSP).
* **Asked by:** Meta, Stripe, Netflix, Google
* **Answer:**
  * **LSP Rule:** Subtypes must be substitutable for their base types without altering the correctness of the program.
  * **Classic Violation:** The Square-Rectangle problem.
    * If class `Square` inherits from `Rectangle`, and we override `setWidth()` to update both width and height to keep it square.
    * A function expecting a `Rectangle` sets width to 5 and height to 10, expecting area to be 50. If passed a `Square`, setting width to 5 changes height to 5, resulting in an area of 25. This violates the caller's expectations.
  * **Fix:** Avoid inheritance if behavioral rules differ. Square and Rectangle should implement a shared `Shape` interface instead.

---

### Q24. What is the Single Responsibility Principle (SRP)? Give a counter-example.
* **Asked by:** Stripe, Netflix, Amazon
* **Answer:**
  * **SRP Rule:** A class should have **one, and only one, reason to change**.
  * **Counter-Example (Violation):** An `Employee` class that contains:
    1. Employee properties (e.g., `name`, `id`).
    2. Business logic (`calculatePay()`).
    3. Database logic (`saveToDatabase()`).
    4. Presentation logic (`reportHours()`).
    * *Why it's bad:* If the database schema changes, you must modify `Employee`. If the accounting payment calculation changes, you modify the same class.
  * **Fix:** Split into `Employee` (data), `EmployeeRepository` (DB), and `PayCalculator` (logic).

---

### Q25. Explain the Open-Closed Principle (OCP). How is it achieved?
* **Asked by:** Uber, Stripe, Google
* **Answer:**
  * **OCP Rule:** Software entities (classes, modules, functions) should be **open for extension, but closed for modification**.
  * **How achieved:** Use Interfaces and Polymorphism.
  * **Example:** A `PaymentProcessor` class with if-else blocks checking payment types:
    ```cpp
    if (type == "credit") payCredit();
    else if (type == "paypal") payPaypal();
    ```
    Adding "crypto" requires modifying `PaymentProcessor` (violating OCP).
  * **Fix:** Define a `PaymentMethod` interface. `PaymentProcessor` calls `paymentMethod->pay()`. Adding crypto simply means creating a new class `CryptoPayment` implementing the interface without modifying existing code.

---

### Q26. What is the Interface Segregation Principle (ISP)?
* **Asked by:** Stripe, Netflix
* **Answer:**
  * **ISP Rule:** Clients should not be forced to depend on methods they do not use. It is better to have many small, specific interfaces than one large, general interface.
  * **Violation:** A `MultiFunctionPrinter` interface containing `print()`, `scan()`, and `fax()`. A simple `BasicPrinter` class implementing it is forced to write stub dummy methods for `scan()` and `fax()`.
  * **Fix:** Split into `Printer`, `Scanner`, and `Fax` interfaces.

---

### Q27. What is the Dependency Inversion Principle (DIP)? How does it relate to Dependency Injection?
* **Asked by:** Google, Amazon, Stripe
* **Answer:**
  * **DIP Rule:** High-level modules should not depend on low-level modules. Both should depend on **abstractions**. Abstractions should not depend on details; details should depend on abstractions.
  * **Dependency Injection (DI):** The structural technique used to achieve DIP. Instead of a class instantiating its own dependencies (coupling), the dependencies are passed in (injected) via the constructor or setters.
  * **Example:**
    * *Violating DIP:* `Car` instantiates `V8Engine` inside its constructor.
    * *DIP Compliant:* `Car` constructor takes an `Engine` interface pointer (`Car(Engine* e)`), allowing us to pass in a `V8Engine` or `ElectricEngine` dynamically.

---

## Section 5: Advanced C++ & Object Lifecycle (Q28–Q36)

### Q28. Why is Composition (HAS-A) often preferred over Inheritance (IS-A)?
* **Asked by:** Meta, Stripe, Netflix
* **Answer:**
  "Prefer composition over inheritance" is a core software design guideline.
  * **Problems with Inheritance:**
    * **Tight Coupling:** Child classes are coupled to the parent's implementation ("White-box reuse"). Changes to the parent class can break child behaviors.
    * **Static Binding:** Relationships are fixed at compile time and cannot change at runtime.
  * **Benefits of Composition:**
    * **Loose Coupling:** The host class only interacts with the component class through its public interface ("Black-box reuse").
    * **Dynamic Binding:** Components can be swapped dynamically at runtime (e.g., changing strategy components).

---

### Q29. What is Delegation in OOP?
* **Asked by:** Google, Apple, iOS roles
* **Answer:**
  * **Delegation** is a design pattern where an object handles a request by delegating the actual work to a helper component object.
  * **Example:** Instead of a `Window` class implementing detailed rectangle geometry math, it delegates the calculation to a `Rectangle` component.
  ```cpp
  class Window {
      Rectangle bounds;
  public:
      int getArea() { return bounds.calculateArea(); } // Delegation
  };
  ```

---

### Q30. What is a Virtual Destructor? Why is it crucial in C++ inheritance?
* **Asked by:** Google, Bloomberg, Apple (High-frequency C++ gotcha)
* **Answer:**
  * A **Virtual Destructor** ensures that the destructor of derived classes is called when deleting an object through a base class pointer.
  * **The Trap:** If the base class destructor is **not virtual**, deleting a derived object through a base pointer results in **Undefined Behavior**—typically, the compiler executes only the base class destructor, leaving any dynamically allocated members of the derived class un-deallocated, causing a **memory leak**.
  * **Rule:** If a class has even one virtual function, its destructor must be declared `virtual`.

```cpp
class Base { virtual ~Base(); }; // Correct
```

---

### Q31. Can a Constructor be Virtual in C++?
* **Asked by:** Google, Microsoft (C++ Core)
* **Answer:**
  * **No**, a constructor cannot be virtual in C++.
  * **Why:**
    1. **Initialization Sequence:** To invoke a virtual function, the object must have a `vptr` pointing to a VTABLE. During construction, the `vptr` is not fully initialized. You cannot use the virtual mechanism before the object is created.
    2. **Concept contradiction:** A constructor must know the exact physical type of the class to allocate the correct memory footprint on the stack or heap. Virtual lookup exists to hide the exact type, which contradicts the purpose of construction.

---

### Q32. What is the difference between Early Binding and Late Binding?
* **Asked by:** Microsoft, Intel
* **Answer:**
  * **Early Binding (Static Binding):** The compiler associates the function call with the physical address of the function code at compile time.
    * *Used for:* Normal function calls, overloaded methods.
  * **Late Binding (Dynamic Binding):** The resolution of the function to call is deferred until runtime, using the object's `vptr` lookup.
    * *Used for:* Virtual functions, enabling run-time polymorphism.

---

### Q33. What is a Friend Class and Friend Function in C++?
* **Asked by:** Core C++ engineering roles
* **Answer:**
  * **Friend:** A keyword that grants external functions or classes access to the `private` and `protected` members of the declaring class.
  * **Trade-off:**
    * *Pros:* Simplifies design of closely coupled helper classes (e.g., iterator classes, matrix operations, unit test frameworks).
    * *Cons:* Breaks encapsulation.
  * **Rule:** Friendship is **not mutual** (if A is a friend of B, B is not automatically a friend of A) and **not transitive** (if A is B's friend, and B is C's friend, A is not C's friend).

---

### Q34. What is the `this` pointer? Can you delete `this`?
* **Asked by:** Google, Microsoft, Bloomberg
* **Answer:**
  * **`this` pointer:** A hidden pointer passed implicitly to all non-static member functions, pointing to the object instance invoking the function.
  * **Can you delete `this`?** **Yes**, it is legally valid to run `delete this;`, but it is extremely dangerous and subject to strict conditions:
    1. The object must have been allocated on the heap using `new`.
    2. You must never access any member variables or call member functions of the object after `delete this;` is executed.
    3. The object must not be accessed again anywhere else in the application.

---

### Q35. What is Operator Overloading? Give a practical scenario.
* **Asked by:** Microsoft, Game studios (EA, Rockstar)
* **Answer:**
  * **Operator Overloading** allows compile-time polymorphism that redefines standard operators (`+`, `-`, `*`, `<<`, etc.) to operate on user-defined objects.
  * **Use Case:** Creating intuitive mathematical coordinate types or string concatenators.
  ```cpp
  Vector operator+(const Vector& other) {
      return Vector(this->x + other.x, this->y + other.y);
  }
  // Enables: Vector v3 = v1 + v2;
  ```

---

### Q36. What is a Static Data Member and a Static Member Function?
* **Asked by:** Adobe, Goldman Sachs
* **Answer:**
  * **Static Data Member:** A variable shared by all instances of a class. It is stored once in global static memory, rather than duplicated inside every object.
  * **Static Member Function:** A function that belongs to the class itself rather than any object instance.
    * *Rule:* It can only access static data members or static functions. It **does not have a `this` pointer** because it is not invoked on an object instance.

---

## Section 6: Memory Management & Language Internals (Q37–Q50)

### Q37. What is Object Cloning? How does it work in Java?
* **Asked by:** Java roles
* **Answer:**
  * **Object Cloning** is the process of creating an exact copy of an existing object.
  * **In Java:** Achieved by implementing the `Cloneable` marker interface and overriding the `clone()` method of the `Object` class.
  * **Warning:** The default `super.clone()` performs a **shallow copy**. If your class contains reference attributes, you must write custom logic to deep copy those objects.

---

### Q38. What is RAII (Resource Acquisition Is Initialization)?
* **Asked by:** Google, Apple, Bloomberg (C++ memory safety)
* **Answer:**
  * **RAII** is a C++ programming idiom where the lifecycle of a resource (heap memory, file handles, mutex locks) is bound to the lifecycle of a stack-allocated object.
  * **Mechanism:**
    * **Acquisition:** The resource is acquired inside the object's constructor.
    * **Release:** The resource is automatically released inside the destructor.
  * **Benefit:** When the stack object goes out of scope (even during exceptions), C++ guarantees its destructor runs, preventing resource leaks automatically. This is the foundation of C++ smart pointers (`std::unique_ptr`, `std::shared_ptr`).

---

### Q39. What is a final class in Java / sealed class in C#?
* **Asked by:** Oracle, Microsoft
* **Answer:**
  * A **final** (Java) or **sealed** (C#) class is a class that **cannot be inherited**.
  * **Why use it:**
    * **Security:** Prevents malicious actors from subclassing system classes (like `String` in Java) and overriding behaviors.
    * **Performance:** Allows compiler optimizations (static binding of methods since no child overrides exist).

---

### Q40. Explain the string pool (string interning) in Java. How does it optimize memory at the JVM level?
* **Asked by:** Amazon, Adobe, Oracle
* **Answer:**
  * **String Pool:** A special storage region in the JVM heap memory. When a string literal is created (e.g., `String s = "hello"`), the JVM checks the pool first. If the string already exists, it returns a reference to the pooled instance; otherwise, it creates a new string in the pool.
  * **Memory Optimization:** Prevents duplicate string objects from occupying duplicate heap addresses.
  * **String Interning:** The method `s.intern()` can be called on a dynamically created string (e.g., via `new String("hello")`) to force the JVM to put it in the string pool and return the pool reference.
  * **Immutability requirement:** This memory sharing is only safe because string objects in Java are immutable. If strings were mutable, changing the value of `s` would silently modify all other string variables referencing that pooled address.

---

### Q41. What is the difference between Upcasting and Downcasting in OOP? What are the compile-time and runtime behaviors?
* **Asked by:** Microsoft, Goldman Sachs
* **Answer:**
  * **Upcasting:** Casting a subclass reference/pointer to a superclass type (e.g., `Parent* p = new Child()`).
    * *Behavior:* Always safe and performed **implicitly** by the compiler. It promotes code generalizability (e.g., storing different shapes in a `List<Shape>`).
  * **Downcasting:** Casting a superclass reference/pointer back to a subclass type (e.g., `Child* c = (Child*)p`).
    * *Behavior:* Inherently unsafe because the compiler cannot guarantee at compile-time that the parent pointer actually points to that specific child type. Must be done **explicitly**.
    * *Safety check:* In C++, we use `dynamic_cast<Child*>(p)` (which uses RTTI to check types at runtime and returns `nullptr` if invalid). In Java, we use `instanceof` before casting to avoid `ClassCastException`.

---

### Q42. How does C++ handle member initialization order in a constructor initializer list? Why does the order of declaration in the class matter?
* **Asked by:** Google, Bloomberg, Apple
* **Answer:**
  * **Rule:** In C++, class members are initialized **in the order they are declared in the class definition**, not the order they appear in the constructor's initializer list.
  * **Why it matters:** If member `B` is initialized using member `A` inside the initializer list, but `B` is declared *before* `A` in the class body, the compiler will initialize `B` first with garbage data before `A` is set, causing subtle bugs.
  * **Example:**
    ```cpp
    class Widget {
        int x; // Declared first
        int y; // Declared second
    public:
        // x is initialized first (with 5), then y is initialized (with 5 * 2 = 10). Correct!
        Widget(int val) : y(val * 2), x(val) {} 
    };
    ```

---

### Q43. Explain constructor delegation in C++11 and Java. What is its primary use case?
* **Asked by:** Adobe, Microsoft
* **Answer:**
  * **Constructor Delegation:** Allows a constructor to call another constructor of the same class to reuse initialization code, preventing code duplication.
  * **Syntax:**
    * **Java:** Uses `this(...)` and must be the very first statement in the constructor body.
    * **C++11:** Uses the constructor initializer list: `Widget(int x) : Widget(x, 0) {}`
  * **Rule:** A constructor cannot delegate to another constructor and initialize class members at the same time in the C++ initializer list; delegation must be the sole initializer action.

---

### Q44. What are strong, soft, weak, and phantom references in Java memory management? When is each type used?
* **Asked by:** Google, Uber, Android developer roles
* **Answer:**
  * **Strong Reference (Default):** (e.g., `Object obj = new Object()`). The GC will never collect this object as long as a strong reference exists, even if JVM runs out of memory (throws `OutOfMemoryError`).
  * **Soft Reference:** (`SoftReference<T>`). GC only collects these objects if the JVM actively runs out of memory (useful for building memory-sensitive caches).
  * **Weak Reference:** (`WeakReference<T>`). GC collects these objects on its next garbage collection cycle, regardless of JVM memory levels. Used to prevent memory leaks (e.g., `WeakHashMap`, observer lists).
  * **Phantom Reference:** (`PhantomReference<T>`). Used to track when an object has been finalized and is about to be swept from memory, enabling custom post-mortem cleanups.

---

### Q45. How does Python resolve method inheritance paths in multiple inheritance? Explain Method Resolution Order (MRO) and C3 Linearization.
* **Asked by:** Python backend roles, Django/Flask systems
* **Answer:**
  * **MRO:** The order in which Python searches for a method or attribute in a class hierarchy when multiple inheritance is used.
  * **Algorithm (C3 Linearization):** Python uses the C3 Linearization algorithm to construct a deterministic resolution order. It guarantees:
    1. **Local Precedence Order:** Subclasses are checked before their parent classes.
    2. **Monotonicity:** The relative order of parent classes is preserved across all subclasses.
  * **Usage:** You can check a class's resolution order by calling `ClassName.__mro__` or `ClassName.mro()`.

---

### Q46. What is the difference between Method Hiding and Method Overriding?
* **Asked by:** Java/C# interviews
* **Answer:**
  * **Method Overriding:** A child class replaces a parent virtual/non-static method. Resolution occurs at runtime based on the object type (Late Binding).
  * **Method Hiding:** A child class declares a static method with the same signature as a parent static method. Resolution occurs at compile time based on the reference pointer type (Early Binding).

---

### Q47. Why should C++ destructors never throw exceptions?
* **Asked by:** Google, Bloomberg, Apple (C++ robustness)
* **Answer:**
  * If a destructor throws an exception during **stack unwinding** (the process of cleaning up stack variables because another exception was already thrown), the C++ runtime finds two active exceptions simultaneously.
  * **Result:** C++ cannot resolve multiple active exceptions and terminates the program immediately via `std::terminate()`. Destructors must catch and swallow all exceptions.

---

### Q48. What is the `volatile` keyword in Java vs C++?
* **Asked by:** Multi-threaded backend roles (Uber, Stripe)
* **Answer:**
  * **In Java:** Guarantees visibility of variable updates across threads. Writes to a volatile variable are written directly to main memory, bypassing CPU registers/caches.
  * **In C++:** Tells the compiler not to apply optimizations (such as caching the value in a register) because the variable can be modified by hardware or external threads. **It does not offer thread safety or thread synchronization.**

---

### Q49. Explain the difference between concrete classes, abstract classes, and interfaces.
* **Asked by:** Adobe, Goldman Sachs
* **Answer:**
  * **Concrete Class:** Complete implementation; can be instantiated directly.
  * **Abstract Class:** Incomplete implementation (has abstract/pure virtual methods); cannot be instantiated.
  * **Interface:** No implementation (pure contract/signatures); defines behavior capabilities.

---

### Q50. What is a virtual base class?
* **Asked by:** Core C++ roles
* **Answer:**
  A **Virtual Base Class** is a class used in inheritance hierarchies (marked `virtual`) to prevent duplicate instances of the base class from being loaded in memory during multiple/diamond inheritance paths, resolving structural ambiguity.
