# OOP in C++ Interview Solutions — The Microsoft Pitch Guide

> **Format**: Each answer is structured as:
> 1. **How to Say It Out Loud (The 20-Second Pitch)**: Direct, crisp answer hitting the exact keywords interviewers score on.
> 2. **Technical Backing / Under the Hood**: C++ memory layout, vtable mechanics, language standard rules, or code snippets.
> 3. **Follow-Up / Preempting the Interviewer**: The next question the interviewer usually probes.

---

### Q1: What are the four pillars of OOP, and what is the exact difference between Encapsulation and Abstraction?
- **How to Say It Out Loud**:
  "The four pillars are Encapsulation, Abstraction, Inheritance, and Polymorphism. The key difference between Encapsulation and Abstraction is that Encapsulation is about data hiding and bundling—restricting direct access to internal state using access specifiers (`private`, `protected`) to protect class invariants. Abstraction is about complexity hiding—exposing only what an object does via a public interface while concealing how it works internally."
- **Under the Hood**:
  - *Encapsulation*: Implementation-level detail (`private double balance;` accessed via `deposit()`).
  - *Abstraction*: Design-level detail (pure virtual class `IShape` defining `virtual void draw() = 0;`).
- **Follow-Up**: *Can you have encapsulation without abstraction?* Yes, bundling fields and methods in a single class with getters/setters without an abstract interface is encapsulation without abstraction.

---

### Q2: How is dynamic polymorphism implemented under the hood in C++ using `vtable` and `vptr`?
- **How to Say It Out Loud**:
  "For every class containing at least one virtual function, the C++ compiler generates a static lookup array of function pointers called the **`vtable`**. Every object instance of that class contains a hidden 8-byte pointer called the **`vptr`** (typically stored at offset 0) pointing to its class's `vtable`. When calling `ptr->draw()`, the runtime dereferences `ptr` to get `vptr`, indexes into the `vtable` at a compile-time known offset, and jumps to the derived function pointer. This costs one extra pointer indirection and inhibits compiler inlining."
- **Under the Hood**:
  ```cpp
  // Conceptual assembly generated for: basePtr->draw();
  // (*(basePtr->vptr[0]))(basePtr);
  ```
  If a class has multiple virtual functions, they occupy slots 0, 1, 2... in the vtable.
- **Follow-Up**: *Does `vtable` exist per-object or per-class?* `vtable` exists once per class in read-only memory; `vptr` exists once per object instance in heap/stack memory.

---

### Q3: Why MUST a base class destructor be declared `virtual` when deleting an object through a base pointer?
- **How to Say It Out Loud**:
  "If a base class destructor is not virtual, deleting a derived class object through a base class pointer (`Base* ptr = new Derived(); delete ptr;`) results in **Undefined Behavior** according to the C++ standard (§8.3.5). In practice, the compiler performs static binding and calls only `~Base()`. The derived destructor `~Derived()` is never invoked, leaking any heap memory, OS file handles, mutex locks, or sockets owned by the derived class."
- **Under the Hood**:
  ```cpp
  class Base {
  public:
      virtual ~Base() = default; // Essential!
  };
  class Derived : public Base {
      int* buffer = new int[100];
      ~Derived() override { delete[] buffer; } // Never called if Base::~Base() is non-virtual!
  };
  ```
- **Follow-Up**: *When is it safe to have a non-virtual destructor?* When the class is marked `final` or when its destructor is declared `protected` (preventing deletion via base pointer).

---

### Q4: Can a constructor be `virtual` in C++? Can a destructor be pure virtual?
- **How to Say It Out Loud**:
  "A constructor **cannot be virtual** because to construct an object, the compiler must know the exact concrete type and memory size at compile time; furthermore, the object's `vptr` is only initialized inside the constructor code, meaning dynamic dispatch is impossible before construction finishes. A destructor **can be pure virtual** (`virtual ~Base() = 0;`), which makes the class abstract, but **you must provide a function body** (`Base::~Base() {}`) because derived class destructors always invoke base destructors during teardown."
- **Under the Hood**:
  ```cpp
  class AbstractBase {
  public:
      virtual ~AbstractBase() = 0; // Pure virtual destructor
  };
  AbstractBase::~AbstractBase() {} // Definition is MANDATORY, otherwise linker error!
  ```
- **Follow-Up**: *How do you simulate a virtual constructor?* Use the **Virtual Constructor / Prototype pattern**: define a virtual clone function `virtual Base* clone() const = 0;`.

---

### Q5: What is C++ Name Hiding, and how do you bring base class overloads into derived scope?
- **How to Say It Out Loud**:
  "In C++, declaring a member function in a derived class hides **all** overloads of that function name in the base class, even if their parameter types differ completely. The compiler searches derived scope first and stops as soon as it finds the name. To bring the hidden base class overloads into derived scope so they participate in overload resolution, use the `using` declaration: `using Base::funcName;`."
- **Under the Hood**:
  ```cpp
  class Base {
  public:
      void print(int x) {}
      void print(double x) {}
  };
  class Derived : public Base {
  public:
      using Base::print; // Unhides Base::print(int) and Base::print(double)
      void print(const std::string& s) {}
  };
  ```
- **Follow-Up**: *Is this overriding?* No, this is overload resolution across scopes; overriding only applies to identical signatures on virtual functions.

---

### Q6: Can you overload a function based on return type alone? Why or why not?
- **How to Say It Out Loud**:
  "No, C++ does not permit function overloading based solely on return type. Overload resolution relies strictly on function name, parameter types, and const-qualifiers. If two functions differed only in return type, the compiler could not determine which function to call when the caller ignores the return value (e.g. `compute();` without assignment)."
- **Under the Hood**:
  Function name mangling in C++ encodes parameter types into the symbol name (e.g. `_Z3addii` for `add(int, int)`), but does not encode the return type for normal functions.
- **Follow-Up**: *Can template functions be overloaded based on return type?* You can use explicit template arguments (`auto x = func<int>();`), but that is template specialization, not function signature overloading.

---

### Q7: What is a Covariant Return Type in C++, and when is it useful?
- **How to Say It Out Loud**:
  "A Covariant Return Type is an exception to the rule that overriding functions must have identical return types. In C++, if a base class virtual function returns a pointer or reference to `Base`, an overriding derived function is allowed to return a pointer or reference to a more derived type (`Derived`). This is especially useful for the Virtual Copy Constructor (`clone()`) idiom, eliminating the need for client code to explicitly `dynamic_cast`."
- **Under the Hood**:
  ```cpp
  class Base {
  public:
      virtual ~Base() = default;
      virtual Base* clone() const { return new Base(*this); }
  };
  class Derived : public Base {
  public:
      Derived* clone() const override { return new Derived(*this); } // Covariant return!
  };
  ```
- **Follow-Up**: *Does covariance work with smart pointers (`std::unique_ptr<Base>`)?* No! `std::unique_ptr<Base>` and `std::unique_ptr<Derived>` are completely distinct template types with no inheritance relationship.

---

### Q8: What is the Diamond Problem in multiple inheritance, and how does C++ solve it using `virtual` inheritance?
- **How to Say It Out Loud**:
  "The Diamond Problem occurs when class `D` inherits from both `B` and `C`, and both `B` and `C` inherit from class `A`. Because both paths copy `A`, class `D` contains two duplicate `A` subobjects, creating ambiguity when accessing `A` members (`d.val` causes compile error). C++ solves this using **`virtual` inheritance** (`class B : virtual public A`). The compiler inserts a virtual base pointer so only **one shared instance of `A`** exists inside `D`."
- **Under the Hood**:
  ```cpp
  class A { public: int val; };
  class B : virtual public A {};
  class C : virtual public A {};
  class D : public B, public C {
  public:
      // With virtual base classes, the most derived class (D) must initialize A!
      D(int v) : A(), B(), C() { val = v; }
  };
  ```
- **Follow-Up**: *What is the performance overhead of virtual inheritance?* Extra pointer indirection (`vbase_offset`) to locate the shared virtual base subobject, and slightly larger object size.

---

### Q9: How is a pure virtual interface created in C++, and what happens if a derived class does not override all pure virtual functions?
- **How to Say It Out Loud**:
  "In C++, an interface is created as an Abstract Base Class containing only pure virtual functions (`= 0`) and a virtual destructor. If a derived class fails to override even one pure virtual function, that derived class **remains an abstract class** and any attempt to instantiate it directly results in a compile-time error."
- **Under the Hood**:
  ```cpp
  struct IRepository {
      virtual ~IRepository() = default;
      virtual void save() = 0;
      virtual void load() = 0;
  };
  class PartialRepo : public IRepository {
      void save() override {} // Did not override load()
  };
  // PartialRepo p; // Compile Error: cannot declare variable 'p' to be of abstract type!
  ```
- **Follow-Up**: *Can a pure virtual function have an implementation in C++?* Yes! `virtual void foo() = 0;` can have a body `void Base::foo() { ... }`, which derived classes can explicitly invoke via `Base::foo()`.

---

### Q10: What is Object Slicing in C++, and how do you prevent it?
- **How to Say It Out Loud**:
  "Object Slicing occurs when a derived class object is assigned or passed by value to a base class object (`Base b = derivedObj;`). The compiler copies only the `Base` portion of the derived object and slices away all derived member variables and derived `vtable` pointer. You prevent it by always passing polymorphic objects by **reference** (`const Base&`) or by **pointer** (`Base*` or `std::unique_ptr<Base>`)."
- **Under the Hood**:
  ```cpp
  void printShape(Shape s);        // WRONG: Causes object slicing!
  void printShape(const Shape& s); // CORRECT: Polymorphic dynamic dispatch preserved.
  ```
- **Follow-Up**: *How can a base class protect itself against accidental slicing?* Mark the base class copy constructor and copy assignment operator as `protected` or `delete`.

---

### Q11: Explain the Liskov Substitution Principle (LSP) and why inheriting `Square` from `Rectangle` violates it.
- **How to Say It Out Loud**:
  "LSP states that objects of a derived class must be substitutable for objects of their base class without breaking program correctness or altering expected invariants. Inheriting `Square` from `Rectangle` violates LSP: in a `Rectangle`, setting width does not affect height. In a `Square`, setting width forces height to equal width. Client code testing `r.setWidth(5); r.setHeight(10); assert(r.getArea() == 50);` will fail if `r` is a `Square`. Mathematical subtyping does not equal behavioral subtyping in OOP."
- **Under the Hood**:
  ```cpp
  class Rectangle {
  public:
      virtual void setWidth(int w) { width = w; }
      virtual void setHeight(int h) { height = h; }
      int getArea() const { return width * height; }
  protected:
      int width, height;
  };
  class Square : public Rectangle {
  public:
      void setWidth(int w) override { width = height = w; } // Breaks invariant!
  };
  ```
- **Follow-Up**: *How do you fix this hierarchy?* Have both `Rectangle` and `Square` independently implement an abstract `Shape` interface with `getArea()`, without an inheritance relationship between each other.

---

### Q12: What is the Single Responsibility Principle (SRP), and how do you identify a code smell violating it?
- **How to Say It Out Loud**:
  "SRP states that a class should have only one reason to change, meaning it should perform a single cohesive set of responsibilities. A classic code smell violating SRP is a 'God Class'—such as an `Order` class that handles order calculations, database queries (`saveToDatabase()`), and sending email receipts (`sendEmail()`). Any change to database drivers or email templates forces modifications to the `Order` class. The fix is splitting into `Order`, `OrderRepository`, and `EmailNotificationService`."
- **Under the Hood**:
  High Cohesion + Low Coupling = SRP.
- **Follow-Up**: *Does SRP mean a class should only have one method?* No, it means all methods in the class should collaborate toward a single overarching responsibility.

---

### Q13: What is the Open/Closed Principle (OCP), and how do interfaces and polymorphism achieve it?
- **How to Say It Out Loud**:
  "OCP states that software entities should be open for extension, but closed for modification. You achieve this by programming to abstractions rather than concrete implementations. Instead of writing giant `switch` or `if-else` blocks that check object types, define a pure virtual interface. Adding a new feature simply requires creating a new derived class implementing that interface without modifying or recompiling existing, tested client code."
- **Under the Hood**:
  Anti-pattern violating OCP:
  `if (paymentType == "CREDIT") payCredit(); else if (paymentType == "PAYPAL") payPayPal();`
  OCP fix:
  `paymentStrategy->pay(amount);`
- **Follow-Up**: *Which design patterns directly embody OCP?* Strategy, Factory, and Decorator patterns.

---

### Q14: What is the Interface Segregation Principle (ISP) vs. Dependency Inversion Principle (DIP)?
- **How to Say It Out Loud**:
  "ISP states that clients should not be forced to depend on interfaces they do not use; it is better to have many small, cohesive interfaces (`IPrintable`, `IScannable`) than one fat interface (`IMultiFunctionMachine`). DIP states that high-level business logic should not depend on low-level implementation details—both should depend on abstractions. We implement DIP using Constructor Dependency Injection (passing interface pointers into constructors)."
- **Under the Hood**:
  DIP in C++:
  ```cpp
  class Car {
      std::unique_ptr<IEngine> engine; // Depends on IEngine interface, not concrete V8Engine
  public:
      explicit Car(std::unique_ptr<IEngine> eng) : engine(std::move(eng)) {}
  };
  ```
- **Follow-Up**: *What is the relationship between DIP and Inversion of Control (IoC)?* DIP is the architectural principle; IoC / Dependency Injection is the practical implementation pattern.

---

### Q15: How do you implement a thread-safe Singleton in modern C++ (Meyers' Singleton vs. Double-Checked Locking)?
- **How to Say It Out Loud**:
  "In modern C++ (C++11 and later), you implement a Singleton using **Meyers' Singleton**: declare a static local variable inside `getInstance()`. The C++11 standard (§6.7) explicitly guarantees that local static variables are initialized in a thread-safe manner on first execution. Double-Checked Locking (DCL) with manual mutexes is obsolete and error-prone in modern C++."
- **Under the Hood**:
  ```cpp
  class Singleton {
  public:
      Singleton(const Singleton&) = delete;
      Singleton& operator=(const Singleton&) = delete;

      static Singleton& getInstance() {
          static Singleton instance; // Guaranteed thread-safe in C++11
          return instance;
      }
  private:
      Singleton() = default;
  };
  ```
- **Follow-Up**: *Why did pre-C++11 Double-Checked Locking fail?* Compiler and CPU instruction reordering could set the instance pointer before the constructor finished executing, causing another thread to observe a half-constructed object.

---

### Q16: What is the difference between the Factory Method pattern and the Abstract Factory pattern?
- **How to Say It Out Loud**:
  "Factory Method uses inheritance and defines a single method to create one specific type of product (`VehicleFactory::createVehicle()`). Abstract Factory uses composition and provides an interface to create **families of related or dependent objects** without specifying their concrete classes (e.g. `GUIFactory` creates both `WindowsButton` and `WindowsScrollbar`, while `MacFactory` creates `MacButton` and `MacScrollbar`)."
- **Under the Hood**:
  Factory Method = 1 product. Abstract Factory = Factory of factories creating a product suite.
- **Follow-Up**: *What smart pointer should factories return?* `std::unique_ptr<Product>`, signaling clear transfer of ownership to the caller.

---

### Q17: What is the Observer pattern, and how do you prevent memory leaks / dangling pointers using `std::weak_ptr`?
- **How to Say It Out Loud**:
  "The Observer pattern defines a one-to-many dependency where a Subject notifies all registered Observers of state changes. A classic flaw is the **Lapsed Listener problem**: if the subject holds strong references (`std::shared_ptr<IObserver>`), observers are never destroyed, causing memory leaks. In C++, you fix this by having the subject store a vector of **`std::weak_ptr<IObserver>`**. When notifying, the subject attempts to promote each weak pointer to a shared pointer via `wp.lock()`; if expired, it removes the dead observer."
- **Under the Hood**:
  ```cpp
  for (auto it = observers.begin(); it != observers.end();) {
      if (auto obs = it->lock()) {
          obs->onNotify();
          ++it;
      } else {
          it = observers.erase(it); // Safe cleanup of destroyed observers
      }
  }
  ```
- **Follow-Up**: *What is the difference between Observer and Pub-Sub?* Observer is tightly coupled in-memory; Pub-Sub uses an external broker (message bus) where publisher and subscriber have zero awareness of each other.

---

### Q18: What is the Strategy pattern, and when would you use it over a large `switch-case` block?
- **How to Say It Out Loud**:
  "The Strategy pattern encapsulates a family of interchangeable algorithms into distinct classes implementing a common interface (`ISortStrategy`). You use it over a `switch-case` block whenever algorithms need to be swapped dynamically at runtime, or when new algorithms will be added in the future, adhering to the Open/Closed Principle without modifying the context class."
- **Under the Hood**:
  ```cpp
  class Sorter {
      std::unique_ptr<ISortStrategy> strategy;
  public:
      void setStrategy(std::unique_ptr<ISortStrategy> s) { strategy = std::move(s); }
      void sort(std::vector<int>& v) { strategy->execute(v); }
  };
  ```
- **Follow-Up**: *Can you implement Strategy with `std::function` in modern C++?* Yes! Instead of full virtual classes, pass lambdas: `std::function<void(std::vector<int>&)>`.

---

### Q19: What is the Decorator pattern, and how does it avoid subclass explosion?
- **How to Say It Out Loud**:
  "The Decorator pattern attaches additional behaviors to an object dynamically at runtime by wrapping it inside a decorator class that implements the same interface. It prevents **subclass explosion**—where combining $N$ optional features (e.g. `Coffee`, `WithMilk`, `WithCaramel`, `WithWhip`) requires $2^N$ static subclasses. With decorators, features are chained dynamically: `new Milk(new Caramel(new Coffee()))`."
- **Under the Hood**:
  The Decorator contains a pointer to the wrapped component (`std::unique_ptr<Component>`) and delegates operations to it while adding its own logic before or after.
- **Follow-Up**: *Where is Decorator used in standard libraries?* C++ streams (e.g. custom stream buffers wrapping underlying streams) and Java I/O (`BufferedReader(InputStreamReader(...))`).

---

### Q20: What is the difference between Composition and Inheritance ("is-a" vs. "has-a"), and why is composition favored?
- **How to Say It Out Loud**:
  "Inheritance represents an 'is-a' relationship: it is tightly coupled, white-box reuse where changes in the parent class can break derived class invariants (the Fragile Base Class problem). Composition represents a 'has-a' relationship: it is loosely coupled, black-box reuse where an object contains another object as a private member. Composition is favored because behaviors can be swapped dynamically at runtime, deep brittle inheritance hierarchies are avoided, and class encapsulation is preserved."
- **Under the Hood**:
  Rule: Use inheritance only when true polymorphic subtyping is required (LSP holds 100%). For mere code reuse, always prefer composition.
- **Follow-Up**: *What is the relationship between private inheritance and composition in C++?* Private inheritance (`class Car : private Engine`) is essentially composition with access to protected members and virtual function overrides, but standard composition (`Engine engine;` member) is cleaner and preferred.

---

### Q21: What is the difference between Public, Protected, and Private inheritance in C++?
- **How to Say It Out Loud**:
  "Public inheritance represents an 'is-a' relationship: public base members stay public, protected stay protected. Protected inheritance makes public and protected base members protected in the derived class. Private inheritance (the default for `class`) makes all public and protected base members private in the derived class, representing 'implemented-in-terms-of' (composition-like access)."
- **Under the Hood**:
  | Base Specifier | Public Inheritance | Protected Inheritance | Private Inheritance |
  |---|---|---|---|
  | `public` | `public` | `protected` | `private` |
  | `protected` | `protected` | `protected` | `private` |
  | `private` | Inaccessible | Inaccessible | Inaccessible |
- **Follow-Up**: *Can a client cast `Derived*` to `Base*` under private inheritance?* No, the cast is illegal outside the derived class because the base subobject is inaccessible to the public.

---

### Q22: What is the difference between a Shallow Copy and a Deep Copy, and why does a default shallow copy cause double-free crashes?
- **How to Say It Out Loud**:
  "A Shallow Copy performs a bitwise or member-wise copy: if an object contains raw pointers to heap memory, only the memory addresses are copied. Both objects now point to the exact same heap memory block. When the first object goes out of scope, its destructor frees the memory. When the second object goes out of scope, its destructor attempts to free the already-freed pointer, causing a fatal **Double-Free memory corruption crash**. A Deep Copy allocates a distinct new heap block and copies the underlying values."
- **Under the Hood**:
  ```cpp
  // Deep copy constructor:
  Buffer(const Buffer& other) : size(other.size), data(new int[other.size]) {
      std::copy(other.data, other.data + size, data);
  }
  ```
- **Follow-Up**: *How do you eliminate raw pointer copy bugs completely?* Follow the **Rule of 0** by using `std::vector` or `std::unique_ptr`, which handle deep copying or disable illegal copying automatically.

---

### Q23: What is the Rule of 3, the Rule of 5, and the Rule of 0 in modern C++?
- **How to Say It Out Loud**:
  "The **Rule of 3** (C++98) states that if a class manages a raw resource and defines a Destructor, it must also implement a Copy Constructor and Copy Assignment Operator. The **Rule of 5** (modern C++11) adds Move Constructor and Move Assignment Operator to support zero-copy move semantics. The **Rule of 0** states that custom resource management should be avoided entirely by using standard RAII wrappers (`std::unique_ptr`, `std::vector`), eliminating the need to write any of the 5 special member functions manually."
- **Under the Hood**:
  The 5 special member functions:
  1. `~T()`
  2. `T(const T&)`
  3. `T& operator=(const T&)`
  4. `T(T&&) noexcept`
  5. `T& operator=(T&&) noexcept`
- **Follow-Up**: *Why should move constructors be marked `noexcept`?* If not `noexcept`, `std::vector` will fall back to expensive copy constructors during reallocation to preserve the strong exception guarantee.

---

### Q24: What is the Copy-and-Swap idiom in C++, and why does it provide strong exception safety?
- **How to Say It Out Loud**:
  "The Copy-and-Swap idiom implements the copy assignment operator by taking its parameter by value (which creates a temporary copy) and swapping its internal resources with `*this` using `std::swap`. It provides **Strong Exception Safety** because if memory allocation throws an exception, it happens during the pass-by-value copy *before* entering the assignment operator body, leaving `*this` untouched. If it succeeds, the swap is guaranteed `noexcept` and the old resources are automatically cleaned up when the parameter goes out of scope."
- **Under the Hood**:
  ```cpp
  Buffer& operator=(Buffer other) noexcept { // Pass by value creates copy
      swap(*this, other);                  // Noexcept swap
      return *this;                        // Old data destroyed with 'other'
  }
  ```
- **Follow-Up**: *Does Copy-and-Swap also handle self-assignment (`a = a;`)?* Yes, cleanly and safely, without needing an explicit `if (this == &other)` check.

---

### Q25: What is RAII (Resource Acquisition Is Initialization), and how does it guarantee resource cleanup in the presence of exceptions?
- **How to Say It Out Loud**:
  "RAII is the core C++ memory management idiom where acquiring a resource (heap memory, mutex lock, file descriptor) is tied directly to the lifetime of a stack-allocated object. The resource is acquired in the constructor and released in the destructor. Because C++ guarantees **stack unwinding** during an exception, the destructors of all local objects on the stack are guaranteed to execute, preventing resource and memory leaks even if an error is thrown."
- **Under the Hood**:
  Examples of RAII classes in standard C++: `std::unique_ptr`, `std::lock_guard`, `std::fstream`.
- **Follow-Up**: *What happens if an exception is thrown inside a destructor during stack unwinding?* `std::terminate()` is called, immediately crashing the program. Destructors in C++11 are `noexcept` by default.

---

### Q26: What is the difference between `std::unique_ptr`, `std::shared_ptr`, and `std::weak_ptr`?
- **How to Say It Out Loud**:
  "`std::unique_ptr` represents exclusive, non-copyable ownership with zero memory overhead compared to a raw pointer. `std::shared_ptr` represents shared ownership where a dynamically allocated control block tracks a reference counter; memory is freed when the count reaches zero. `std::weak_ptr` is a non-owning observer to an object managed by `shared_ptr`; it references the control block without incrementing the strong reference counter, preventing circular reference memory leaks."
- **Under the Hood**:
  - `unique_ptr`: Size = 8 bytes.
  - `shared_ptr`: Size = 16 bytes (pointer to object + pointer to control block).
  - To access data from `weak_ptr`: Must call `wp.lock()`, which returns a `shared_ptr`.
- **Follow-Up**: *Why prefer `std::make_unique` over `new`?* Exception safety and prevents memory leaks if parameter evaluation throws before constructor assignment.

---

### Q27: How does reference counting work in `std::shared_ptr`, and how do circular references cause memory leaks?
- **How to Say It Out Loud**:
  "A `shared_ptr` points to both the object and a heap-allocated **Control Block** containing a strong reference counter and a weak reference counter. Copying increments the strong count; destruction decrements it. If Object A holds a `shared_ptr` to Object B, and Object B holds a `shared_ptr` to Object A, a **Circular Reference** is created. Even when all external pointers go out of scope, both counters remain at 1, meaning neither destructor ever executes, leaking memory. The solution is breaking the cycle by making one reference a `std::weak_ptr`."
- **Under the Hood**:
  `std::make_shared` allocates the object and the control block in a single contiguous memory allocation, improving cache locality.
- **Follow-Up**: *When is the control block itself freed?* When both the strong count AND the weak count drop to zero.

---

### Q28: What is the difference between `delete` and `delete[]` in C++?
- **How to Say It Out Loud**:
  "`delete ptr;` frees memory allocated with `new` for a single object and executes one destructor call. `delete[] ptr;` frees an array allocated with `new[]`. When allocating an array, the compiler places a hidden header (an array cookie) just before the returned pointer storing the number of elements. `delete[]` reads this cookie, invokes the destructor for **each element** in the array in reverse order, and deallocates the entire memory block. Using `delete` on an array causes undefined behavior and memory leaks."
- **Under the Hood**:
  Memory layout for `new T[N]`:
  `[ Array Size N (8 bytes) | T[0] | T[1] | ... | T[N-1] ]`
- **Follow-Up**: *What happens if you call `delete[]` on a single object?* Undefined behavior; the runtime reads garbage as the array size and calls destructors on invalid memory addresses.

---

### Q29: Can static member functions be `virtual` or access non-static member variables in C++?
- **How to Say It Out Loud**:
  "No. Static member functions belong to the class itself rather than any object instance; they do not receive an implicit `this` pointer. Because dynamic dispatch requires an object instance containing a `vptr` to index into a `vtable`, static functions cannot be `virtual`. For the same reason, static functions cannot access non-static member variables without being explicitly passed an object reference."
- **Under the Hood**:
  Static functions are essentially global functions scoped inside the class namespace.
- **Follow-Up**: *Can static functions be overloaded?* Yes, based on parameter types, like any normal function.

---

### Q30: What is the `friend` keyword in C++, and does it violate encapsulation?
- **How to Say It Out Loud**:
  "The `friend` keyword allows a non-member function or another class full access to `private` and `protected` members of the class granting friendship. While purists argue it violates encapsulation, in practice it **enhances encapsulation** when used correctly: it allows tightly coupled classes (like a `LinkedList` and its `Iterator`, or operator overloading `operator<<`) to collaborate without exposing sensitive internal pointers through public getters/setters."
- **Under the Hood**:
  Friendship is **not symmetric** (if A is a friend of B, B is not automatically a friend of A) and **not transitive** (if A is a friend of B and B is a friend of C, A is not a friend of C).
- **Follow-Up**: *How is `friend` commonly used with operator overloading?* Stream insertion: `friend std::ostream& operator<<(std::ostream& os, const MyClass& obj);`.
