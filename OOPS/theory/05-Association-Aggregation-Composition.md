# Master Guide 05: Association, Aggregation & Composition in C++

> **Focus:** The Ownership Strength Gradient, C++ Smart Pointer Memory Mappings (std::unique_ptr vs std::shared_ptr vs std::weak_ptr), UML Class Diagram Vocabulary, Cyclic Reference Memory Leaks, and Low-Level Design (LLD) Relationship Mapping.
> 
> *Targeted for Top-Tier C++ Systems, HFT, and Backend Engineering Interviews.*

---

# Table of Contents
1. [The Ownership Strength Gradient](#1-the-ownership-strength-gradient)
2. [C++ Smart Pointer & Memory Mapping Matrix](#2-c-smart-pointer--memory-mapping-matrix)
3. [1. Association ("Uses-a" / Weakest Connection)](#3-1-association-uses-a--weakest-connection)
4. [2. Aggregation ("Has-a" / Shared Independent Lifecycle)](#4-2-aggregation-has-a--shared-independent-lifecycle)
5. [3. Composition ("Part-of" / Strong Dependent Lifecycle)](#5-3-composition-part-of--strong-dependent-lifecycle)
6. [The Cyclic Reference Trap & std::weak_ptr](#6-the-cyclic-reference-trap--stdweak_ptr)
7. [UML Class Diagram Cheat Sheet for LLD Interviews](#7-uml-class-diagram-cheat-sheet-for-lld-interviews)
8. [High-Frequency C++ Interview Drill & Verbal Q&A](#8-high-frequency-c-interview-drill--verbal-qa)

---

# 1. The Ownership Strength Gradient

```
Weakest Coupling / Zero Ownership                         Strongest Coupling / Exclusive Ownership
+-------------------------------------------------------------------------------------------------+
|   ASSOCIATION ("Uses-a")   --->   AGGREGATION ("Has-a")   --->   COMPOSITION ("Part-of")        |
|   (Driver & Car)                  (Department & Employee)        (House & Room)                 |
|   Independent Lifecycles          Shared Lifecycles              Coupled Lifecycles             |
+-------------------------------------------------------------------------------------------------+
```

- **One-Line Intuition:**
  - **Association:** A passenger hailing a taxi (interact temporarily, both go their own way).
  - **Aggregation:** A football club and its players (club folds, players join another team).
  - **Composition:** A human and their heart (human dies, the heart dies with them).
- **The Interview Trap:** Saying Aggregation and Composition are identical. In **Composition**, the child object cannot exist without the parent. In **Aggregation**, the child object exists independently of the parent container.

---

# 2. C++ Smart Pointer & Memory Mapping Matrix

```
+---------------------------------------------------------------------------------------------------+
| RELATIONSHIP TYPE    | LIFECYCLE DEPENDENCY          | IDIOMATIC C++ MEMORY PRIMITIVE             |
+---------------------------------------------------------------------------------------------------+
| Association          | Independent: Caller and callee| Non-owning raw pointer (`Target*`),        |
| ("Uses-a")           | lifecycles are unrelated      | `const Target&`, or `std::weak_ptr<Target>`|
+---------------------------------------------------------------------------------------------------+
| Aggregation          | Shared: Child outlives parent | `std::shared_ptr<Child>` or collection     |
| ("Has-a")            | container                     | of `std::shared_ptr<Child>`                |
+---------------------------------------------------------------------------------------------------+
| Composition          | Dependent: Child dies with    | Direct value member (`Child child;`) or    |
| ("Part-of")          | parent container              | exclusive owner `std::unique_ptr<Child>`   |
+---------------------------------------------------------------------------------------------------+
```

---

# 3. 1. Association ("Uses-a" / Weakest Connection)

```cpp
class Car {
public:
    void drive() { std::cout << "Car driving forward\n"; }
};

class Driver {
public:
    // Association: Driver uses Car as a parameter; Driver does NOT own the Car
    void operateCar(Car* car) {
        if (car) car->drive();
    }
};
```
- **Lifecycle:** Neither `Driver` nor `Car` creates or deletes the other.

---

# 4. 2. Aggregation ("Has-a" / Shared Independent Lifecycle)

```cpp
class Employee {
private:
    std::string name;
public:
    explicit Employee(std::string n) : name(std::move(n)) {}
    std::string getName() const { return name; }
};

class Department {
private:
    std::string deptName;
    std::vector<std::shared_ptr<Employee>> employees; // Aggregation: Shared ownership

public:
    explicit Department(std::string name) : deptName(std::move(name)) {}

    void addEmployee(std::shared_ptr<Employee> emp) {
        employees.push_back(emp);
    }
};

void testAggregation() {
    auto emp1 = std::make_shared<Employee>("Alice");
    {
        Department engineering("Engineering");
        engineering.addEmployee(emp1);
    } // engineering goes out of scope and is destroyed here...

    // emp1 ("Alice") is STILL ALIVE and valid because ownership was shared, not exclusive!
    std::cout << emp1->getName() << " still exists!\n";
}
```

---

# 5. 3. Composition ("Part-of" / Strong Dependent Lifecycle)

```cpp
class Room {
private:
    std::string roomType;
public:
    explicit Room(std::string type) : roomType(std::move(type)) {}
};

class House {
private:
    Room livingRoom;                     // Composition via direct value embedding
    std::unique_ptr<Room> masterBedroom; // Composition via exclusive smart pointer

public:
    House() : livingRoom("Living Room"), masterBedroom(std::make_unique<Room>("Master Bedroom")) {}

    // When House destructor executes, both livingRoom and masterBedroom are AUTOMATICALLY destroyed!
};
```
- **Low-Level Memory Layout:** Embedding `Room livingRoom;` directly allocates memory inline within the `House` object's memory buffer, maximizing CPU cache line spatial locality.

---

# 6. The Cyclic Reference Trap & std::weak_ptr

```cpp
// BAD DESIGN: Circular std::shared_ptr causes a PERMANENT MEMORY LEAK!
struct Node {
    std::shared_ptr<Node> next;
    std::shared_ptr<Node> prev; // Cyclic reference keeps ref count >= 1 forever!
    ~Node() { std::cout << "Destroyed\n"; }
};

// FIXED C++ DESIGN: Break cycle using std::weak_ptr for back-references
struct SafeNode {
    std::shared_ptr<SafeNode> next; // Forward strong ownership
    std::weak_ptr<SafeNode> prev;   // Backward non-owning observing reference
    ~SafeNode() { std::cout << "Safely Destroyed\n"; }
};
```

```
Memory Lifecycle of std::weak_ptr:
[ Parent Node ] ===== (std::shared_ptr: Strong Ref Count = 1) =====> [ Child Node ]
      ^                                                                     |
      | - - - - - - - (std::weak_ptr: Weak Ref Count = 1) - - - - - - - - - +
      (Does NOT prevent Parent Node from being destroyed when out of scope!)
```

---

# 7. UML Class Diagram Cheat Sheet for LLD Interviews

When drawing UML class diagrams in low-level design interviews, use these standard notations:

```
+---------------------------------------------------------------------------------------------------+
| RELATIONSHIP         | UML NOTATION          | SYMBOL RENDERING (ASCII)                           |
+---------------------------------------------------------------------------------------------------+
| Inheritance          | Solid line + Triangle | Derived ------------------|> Base                  |
+---------------------------------------------------------------------------------------------------+
| Realization (Interf.)| Dashed line + Triangle| Concrete - - - - - - - - -|> Interface            |
+---------------------------------------------------------------------------------------------------+
| Composition          | Solid line + Solid <> | House <*================== Room                    |
|                      | (Filled Black Diamond)|                                                    |
+---------------------------------------------------------------------------------------------------+
| Aggregation          | Solid line + Open <>  | Department <>------------- Employee                |
|                      | (Hollow Diamond)      |                                                    |
+---------------------------------------------------------------------------------------------------+
| Association          | Solid line + Arrow    | Driver -------------------> Car                    |
+---------------------------------------------------------------------------------------------------+
| Dependency           | Dashed line + Arrow   | OrderProcessor - - - - - -> PaymentGateway         |
+---------------------------------------------------------------------------------------------------+
```

---

# 8. High-Frequency C++ Interview Drill & Verbal Q&A

### Q1: How do you choose between `std::unique_ptr` and `std::shared_ptr` for class relationships?
> **Answer:** Default to **`std::unique_ptr` (Composition)** because single, exclusive ownership is simpler, zero-overhead, and expresses clear lifecycle semantics. Use **`std::shared_ptr` (Aggregation)** only when multiple independent entities truly share co-ownership of a resource's lifetime.

### Q2: Why is direct value embedding (`Room room;`) superior to `std::unique_ptr<Room>` for composition?
> **Answer:** Direct value embedding **eliminates dynamic heap allocation overhead (`malloc`/`free`)** and places `Room` data contiguously inside the `House` object's memory footprint, maximizing **CPU L1/L2 cache locality**.

### Q3: What is the purpose of `std::weak_ptr` in C++ object relationships?
> **Answer:** It acts as a **non-owning observer** to a resource managed by `std::shared_ptr`. It allows observing or temporarily locking the resource via `.lock()` without incrementing the strong reference count, **breaking cyclic dependency memory leaks**.

### Q4: In an eCommerce LLD, what relationship exists between `Order` and `OrderItem`?
> **Answer:** **Composition.** An `OrderItem` (e.g. quantity, captured price snapshot) has no independent business meaning or lifecycle outside of its parent `Order`. If the `Order` is deleted, all its `OrderItem` instances are deleted with it.

### Q5: What is the difference between Association and Dependency in UML?
> **Answer:** **Association** represents a structural, long-term relationship where one class holds a reference/pointer to another as a member variable. **Dependency** represents a transient, short-term usage where one class uses another only as a local variable or method parameter (`void func(Service& s)`).
