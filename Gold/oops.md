# OOP — GS OA MCQ Revision Notes

---

## 1. Core Pillars

*   **Encapsulation:** Grouping data + functions. Access control is managed using access modifiers (`private`, `protected`, `public`).
*   **Abstraction:** Hiding complexity. Achieved via **abstract classes** (can have member variables and concrete functions) and **interfaces** (pure contracts).
*   **Inheritance:** Reusing code. Constructors are **NOT** inherited, but the parent constructor is always executed before the child constructor.
*   **Polymorphism:**
    *   **Static (Compile-time):** Method overloading (same name, different signatures).
    *   **Dynamic (Runtime):** Method overriding (child class redefines a parent method with the exact same signature).

---

## 2. Virtual Functions & Destructors (GS C++ Favorites)

*   **Virtual Function:** Declared in a base class using the `virtual` keyword. Allows dynamic binding (runtime polymorphism) via a **VTABLE (Virtual Table)** pointer.
*   **Pure Virtual Function:** A function declared as `virtual void func() = 0;`. Any class containing at least one pure virtual function is an **Abstract Class** and cannot be instantiated.
*   **Virtual Destructor:** **CRITICAL FOR OA.** If a base class pointer deletes a derived class object, the destructor MUST be virtual:
    ```cpp
    class Base {
        virtual ~Base() {} // Must be virtual to prevent memory leaks!
    };
    ```
    *   *Without `virtual`, only the Base destructor is called, causing derived class allocations to leak.*

### Under the Hood: VTABLE and VPTR
*   If a class contains a `virtual` function, the compiler inserts a hidden virtual pointer (`_vptr`) pointing to the VTABLE (table of virtual function pointers).
*   **Size Impact:** The size of an object of a class with virtual functions increases by the size of a pointer (`sizeof(void*)` = 8 bytes on 64-bit systems).
    ```cpp
    class Empty { }; // sizeof(Empty) = 1 (minimum size to guarantee unique address)
    class WithVirtual { virtual void f(); }; // sizeof(WithVirtual) = 8 (for VPTR)
    ```

---

## 3. Multiple Inheritance & The Diamond Problem

*   **Diamond Problem:** Occurs when class `D` inherits from both `B` and `C`, which both inherit from `A`. This creates duplicate copies of `A`'s members inside `D`, causing ambiguity.
*   **GS Solution:** Use **Virtual Inheritance**:
    ```cpp
    class B : virtual public A {};
    class C : virtual public A {};
    class D : public B, public C {}; // Only one copy of A is created inside D
    ```

---

## 4. Copy Semantics: Shallow vs. Deep Copy

*   **Copy Constructor:** Initializing an object using another object of the same class: `Base b2 = b1;` or `Base b2(b1);`.
*   **Copy Assignment Operator:** Assigning values of an existing object to another existing object: `b2 = b1;`.
*   **Shallow Copy (Default):** Copies member values directly. If members are pointers, both objects point to the same memory. Deleting one object will cause a dangling pointer / double-free runtime error in the other.
*   **Deep Copy (User-defined):** Allocates new memory for pointer members and copies the actual values, ensuring independent objects.

---

## 5. Object Slicing
*   Occurs when a derived class object is assigned **by value** to a base class object.
*   The extra variables/methods of the derived class are "sliced away" because the base object only has enough memory for base members.
    ```cpp
    Derived d;
    Base b = d; // b's vptr is reset to Base, and Derived members are sliced away.
    ```
*   **Prevention:** Use references or pointers (`Base& b = d;` or `Base* b = &d;`) to preserve polymorphism and derived data.

---

## 6. Access Control: `friend` Keyword
*   **Friend Function/Class:** A function or class marked with the `friend` keyword inside another class.
*   It has access to the **private and protected** members of the class that declared it a friend.
*   *Rules:* 
    *   Friendship is not mutual (If A is a friend of B, B is not automatically a friend of A).
    *   Friendship is not inherited.

---

## 7. Static Members & Methods
*   **Static Variable:** Declared inside the class, but shared by all instances. In C++, it **must** be defined outside the class in global scope (except `constexpr static`).
*   **Static Method:** Belongs to the class, not an object.
    *   Can **only** access other static variables and static methods.
    *   Does **not** have access to the `this` pointer.

---

## 8. Key Quick-Check Rules for MCQs

| Situation / Trap | Correct Behavior / Rule |
| :--- | :--- |
| **Constructor Overriding?** | Constructors **cannot** be virtual, static, or overridden. |
| **Destructor Overriding?** | Destructors **can** (and often should) be virtual. |
| **Copy Constructor call?** | Called when passing objects by value, or returning objects by value from functions. |
| **Abstract Class instantiation?** | Compilation error. Abstract classes can only be accessed via pointers/references. |
| **Method Overloading signature?** | Must differ by parameter count, type, or order. **Changing return type alone is NOT enough.** |
