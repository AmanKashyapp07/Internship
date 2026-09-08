# OOP in C++ — Last-Day Revision & Flashcards (Microsoft Prep)

> **Goal**: 15-minute high-yield cram sheet. Read the Flashcards first (cover the answer, say it out loud), then scan the Key Points & Trap Matrix.

---

## Part 1: Top 20 High-Yield C++ OOP Flashcards

### Card 1: What happens if a base class destructor is NOT virtual?
- **Answer**: If a derived object is deleted through a base pointer (`Base* b = new Derived(); delete b;`), it causes **Undefined Behavior (UB)**. In practice, only `~Base()` is called; `~Derived()` is skipped, causing resource/memory leaks.

### Card 2: How does dynamic dispatch work under the hood in C++?
- **Answer**: The compiler generates a **`vtable`** (array of virtual function pointers) for every class with virtual functions. Each object instance contains a hidden **`vptr`** (8 bytes) pointing to its class's `vtable`. Calling a virtual function resolves via: `vptr ➔ offset in vtable ➔ function pointer jump` (1 pointer indirection cost).

### Card 3: Can a constructor be virtual in C++?
- **Answer**: **No**. A virtual call requires a `vptr`, but the `vptr` is only initialized *during* constructor execution. Furthermore, to construct an object, the exact concrete type must be known at compile time. *(Workaround: Virtual Copy Constructor / Clone idiom).*

### Card 4: Can a destructor be pure virtual?
- **Answer**: **Yes** (`virtual ~Base() = 0;`). It makes the class abstract. However, **you must provide a function body** (`Base::~Base() {}`) because derived class destructors always invoke the base destructor during teardown.

### Card 5: What is the Diamond Problem and how does C++ solve it?
- **Answer**: When class `D` inherits from both `B` and `C`, and both inherit from `A`, `D` gets two duplicate copies of `A`'s subobject (ambiguous member access).
- **Fix**: Use **`virtual` inheritance** (`class B : virtual public A`). C++ uses a virtual base pointer so only one shared `A` subobject exists in `D`.

### Card 6: What is C++ Name Hiding?
- **Answer**: If a derived class defines a function with the same name as a function in the base class, **it hides all overloads of that name in the base class**, regardless of parameter lists.
- **Fix**: Bring base overloads into derived scope via `using Base::funcName;`.

### Card 7: Why does `Square` extending `Rectangle` violate the Liskov Substitution Principle (LSP)?
- **Answer**: In `Rectangle`, setting width does not affect height. In `Square`, setting width forces height to change. Client code expecting a `Rectangle` will have its invariants broken. Mathematical subtyping does not equal behavioral subtyping in OOP.

### Card 8: How do you write a thread-safe Singleton in modern C++?
- **Answer**: Use **Meyers' Singleton**:
  ```cpp
  static Singleton& getInstance() {
      static Singleton instance; // Thread-safe by C++11 standard (§6.7)
      return instance;
  }
  ```
  Delete copy/move constructors and assignment operators.

### Card 9: What is the Rule of 5 in C++?
- **Answer**: If a class manages a raw resource and implements a custom **Destructor**, it must also implement or explicitly delete the other 4 special member functions:
  1. Copy Constructor
  2. Copy Assignment Operator
  3. Move Constructor
  4. Move Assignment Operator

### Card 10: What is the difference between Shallow Copy and Deep Copy?
- **Answer**: Shallow copy copies pointer values (addresses) as-is; both objects point to the same heap memory, leading to **double-free crashes** on deletion. Deep copy allocates fresh memory and recursively duplicates the values.

### Card 11: What is the difference between `delete` and `delete[]`?
- **Answer**: `delete ptr;` calls the destructor for a single object and frees memory. `delete[] ptr;` reads the array size stored in the memory header (cookie) and calls the destructor for **each element** in the array before freeing.

### Card 12: Can static member functions be virtual in C++?
- **Answer**: **No**. Static functions are class-level; they do not have a `this` pointer and cannot access an object's `vptr`.

### Card 13: Can you overload a function based on return type alone?
- **Answer**: **No**. The compiler cannot resolve which function to call at the call site if the caller does not assign the return value (e.g., `calculate();`).

### Card 14: What is an Abstract Base Class in C++?
- **Answer**: A class with at least one **pure virtual function** (`virtual void foo() = 0;`). It cannot be instantiated directly and serves as an interface/contract.

### Card 15: What is a Covariant Return Type in C++?
- **Answer**: An overriding virtual function in a derived class can return a pointer/reference to a **derived type** instead of the base type returned by the base function (e.g., `Derived* clone() override` vs `Base* clone()`).

### Card 16: Public vs Protected vs Private inheritance?
- **Answer**:
  - `public`: "Is-A" relationship. Public remains public.
  - `protected`: Public & protected become protected.
  - `private`: "Implemented-in-terms-of" (composition-like). All inherited members become private in derived class.

### Card 17: How do you prevent memory leaks in the Observer Pattern?
- **Answer**: Store observers as **`std::weak_ptr<IObserver>`** inside the subject. This avoids circular reference counting and allows observers to be safely garbage-collected when destroyed.

### Card 18: What is the difference between Strategy and State patterns?
- **Answer**: In **Strategy**, the client explicitly configures an interchangeable algorithm. In **State**, the context object automatically changes its behavior internally as its internal state transitions.

### Card 19: What is Dependency Inversion Principle (DIP)?
- **Answer**: High-level modules and low-level modules should both depend on abstractions (interfaces). Implement by passing `std::unique_ptr<IInterface>` into constructors (**Dependency Injection**).

### Card 20: What is RAII (Resource Acquisition Is Initialization)?
- **Answer**: A core C++ idiom where resource allocation (memory, file handle, lock) is tied to object lifetime. The resource is acquired in the constructor and guaranteed to be released in the destructor when the object goes out of scope (even on exceptions).

---

## Part 2: 1-Minute Memory Matrix & Key Points

### Access Specifiers in Inheritance:
| Base Access | Public Inheritance | Protected Inheritance | Private Inheritance |
|---|---|---|---|
| **`public`** | `public` | `protected` | `private` |
| **`protected`** | `protected` | `protected` | `private` |
| **`private`** | Hidden (Inaccessible) | Hidden (Inaccessible) | Hidden (Inaccessible) |

### SOLID in 5 Seconds:
- **S**: One reason to change.
- **O**: Open to extend, closed to modify (polymorphism).
- **L**: Subtype must preserve base invariants (`Square` $\neq$ `Rectangle`).
- **I**: Small, client-specific pure virtual interfaces.
- **D**: Depend on interfaces, not concrete implementations.

### Design Pattern Trigger Words:
- *"Single global instance, thread-safe"* ➔ **Meyers' Singleton**
- *"Object creation without exposing concrete classes"* ➔ **Factory Method**
- *"Families of related objects"* ➔ **Abstract Factory**
- *"Pub-sub, event listeners without dangling references"* ➔ **Observer (`std::weak_ptr`)**
- *"Interchangeable algorithms at runtime"* ➔ **Strategy**
- *"Attach behavior dynamically without subclass explosion"* ➔ **Decorator**

---

## Part 3: Top 5 Microsoft Traps in C++ OOP

1. **Object Slicing**:
   - Passing a derived object by value to a function taking `Base` slices away all derived member variables and `vptr`. **Always pass polymorphic objects by reference (`const Base&`) or pointer (`Base*` / `std::unique_ptr<Base>`)**.
2. **Calling Virtual Functions inside Constructors/Destructors**:
   - During base construction, the derived object does not exist yet. The `vptr` points to the `Base` vtable! Dynamic dispatch resolves to the **Base class implementation**, not the derived one.
3. **Missing `override` Specifier**:
   - If derived function signature differs even slightly (e.g. missing `const`), the compiler treats it as a new function rather than an override without raising an error. Always use `override`.
4. **Default Copy with Raw Pointers**:
   - Relying on compiler-generated copy constructor for classes managing raw pointers causes double-free errors. Use smart pointers or follow the **Rule of 5**.
5. **Private Inheritance as Default for `class`**:
   - In C++, `class Derived : Base` defaults to **private** inheritance. `struct Derived : Base` defaults to **public** inheritance.
