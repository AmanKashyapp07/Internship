# 02. Object Relationships, Design Principles & Object Design

> **Interview Mindset**: Writing clean code is not about knowing syntax; it is about knowing **how objects relate to each other, who owns what in memory, and how to design systems that are open for extension but closed for modification**.

---

## PART 1: Object Relationships in C++

```
                     OBJECT RELATIONSHIP HIERARCHY
      Weakest Coupling ────────────────────────────────► Strongest Coupling
┌──────────────┐      ┌──────────────┐      ┌──────────────┐      ┌──────────────┐
│  Dependency  │      │ Association  │      │ Aggregation  │      │ Composition  │
│  "Uses-A"    │      │  "Knows-A"   │      │ "Has-A" Weak │      │"Has-A" Strong│
└──────────────┘      └──────────────┘      └──────────────┘      └──────────────┘
```

---

### 1. Dependency ("Uses-A")
A temporary relationship where one class uses another inside a member function (e.g. as a parameter or local variable). Neither class owns the other.

```cpp
class Printer {
public:
    void print(const std::string& text);
};

class Document {
public:
    // Document DEPENDS on Printer only for the duration of this call
    void exportTo(Printer& printer) const {
        printer.print("Document Content");
    }
};
```

---

### 2. Association ("Knows-A")
A persistent relationship where two classes know about each other, but have independent lifecycles.

```cpp
class Doctor;

class Patient {
    std::string name_;
    Doctor* primaryDoctor_{nullptr}; // Association: Doctor outlives Patient
public:
    void assignDoctor(Doctor* doc) { primaryDoctor_ = doc; }
};
```

---

### 3. Aggregation (Weak "Has-A")
Parent holds a reference/pointer to child, but **does not manage its lifetime**. If the parent is destroyed, the child continues to exist.

```
┌──────────────────┐               ┌──────────────────┐
│    Department    │◇─────────────►│     Teacher      │
└──────────────────┘  (Aggregates) └──────────────────┘
Teacher exists independently even if Department is shut down.
```

```cpp
class Teacher {
    std::string name_;
public:
    explicit Teacher(std::string name) : name_(std::move(name)) {}
};

class Department {
    std::vector<std::shared_ptr<Teacher>> faculty_; // Aggregation
public:
    void addTeacher(std::shared_ptr<Teacher> t) {
        faculty_.push_back(std::move(t));
    }
};
```

---

### 4. Composition (Strong "Has-A")
Parent **strictly owns and manages the lifetime of the child**. If the parent dies, the child dies with it.

```
┌──────────────────┐               ┌──────────────────┐
│      House       │◆─────────────►│       Room       │
└──────────────────┘  (Composes)   └──────────────────┘
Room cannot exist independently without the House.
```

```cpp
class Room {
    std::string type_;
public:
    explicit Room(std::string type) : type_(std::move(type)) {}
};

class House {
    std::vector<Room> rooms_; // Direct value member: House destroys all rooms!
    // Or: std::vector<std::unique_ptr<Room>> rooms_;
public:
    void addRoom(const std::string& type) {
        rooms_.emplace_back(type);
    }
};
```

---

### Summary of C++ Ownership Rules:

| Relationship | UML Symbol | Lifetime Dependency | C++ Memory Representation |
| :--- | :---: | :--- | :--- |
| **Dependency** | `───►` (dashed) | None (transient call) | Function parameter (`Foo& foo` or `const Foo*`) |
| **Association**| `───►` | Independent | Raw pointer (`Foo*`) or non-owning reference |
| **Aggregation**| `◇──►` (hollow) | Child outlives Parent | `std::weak_ptr<Foo>` or `std::shared_ptr<Foo>` |
| **Composition**| `◆──►` (filled) | Child dies with Parent| Direct value member `Foo foo;` or `std::unique_ptr<Foo>` |
| **Inheritance**| `───▷` (triangle)| Subclass IS base class | `class Derived : public Base` |

---

## PART 2: The SOLID Principles in Modern C++

```
┌───┐  Single Responsibility  ──► A class should have ONLY ONE reason to change.
│ S │
├───┤  Open / Closed          ──► Open for extension, but CLOSED for modification.
│ O │
├───┤  Liskov Substitution    ──► Derived types must be substitutable for base types.
│ L │
├───┤  Interface Segregation  ──► Clients should not depend on interfaces they don't use.
│ I │
├───┤  Dependency Inversion   ──► Depend on abstractions, NOT on concrete implementations.
│ D │
└───┘
```

---

### 1. Single Responsibility Principle (SRP)

> *"A class should have one, and only one, reason to change."*

```cpp
// ❌ BAD: God Class violating SRP
class UserAccount {
public:
    void updateProfile(const std::string& name);
    void saveToDatabase();                  // Reason to change: DB schema changes
    void sendEmailVerification();           // Reason to change: Email provider changes
    void generateInvoicePDF();              // Reason to change: Accounting format changes
};

// ✅ GOOD: Decomposed by Single Responsibility
class UserProfile { /* Pure user state and domain logic */ };

class UserRepository {
public:
    virtual ~UserRepository() = default;
    virtual void save(const UserProfile& user) = 0;
};

class NotificationService {
public:
    virtual ~NotificationService() = default;
    virtual void sendEmail(const std::string& email, const std::string& body) = 0;
};
```

---

### 2. Open/Closed Principle (OCP)

> *"Software entities should be open for extension, but closed for modification."*

```cpp
// ❌ BAD: Modifying class every time a new shape is added
class AreaCalculator {
public:
    double calculate(const std::string& shapeType, double d1, double d2) {
        if (shapeType == "circle") return 3.14159 * d1 * d1;
        if (shapeType == "rectangle") return d1 * d2;
        // Adding Triangle requires modifying this function! (Violates OCP)
        return 0.0;
    }
};

// ✅ GOOD: Polymorphic Abstraction (Add new shapes without editing calculator)
class IShape {
public:
    virtual ~IShape() = default;
    virtual double area() const = 0;
};

class Circle final : public IShape {
    double radius_;
public:
    explicit Circle(double r) : radius_(r) {}
    double area() const override { return 3.14159 * radius_ * radius_; }
};

class Rectangle final : public IShape {
    double width_, height_;
public:
    Rectangle(double w, double h) : width_(w), height_(h) {}
    double area() const override { return width_ * height_; }
};

// Open for extension: Add Triangle, Pentagon without touching existing classes!
```

---

### 3. Liskov Substitution Principle (LSP)

> *"Derived classes must be substitutable for their base classes without altering the correctness of the program."*

#### The Classic Classic Interview Trap: Square inherits from Rectangle

```cpp
// ❌ BAD: Square inherits from Rectangle (Violates LSP!)
class Rectangle {
protected:
    double width_{0}, height_{0};
public:
    virtual ~Rectangle() = default;
    virtual void setWidth(double w) { width_ = w; }
    virtual void setHeight(double h) { height_ = h; }
    double getArea() const { return width_ * height_; }
};

class Square : public Rectangle {
public:
    void setWidth(double w) override { width_ = height_ = w; }
    void setHeight(double h) override { width_ = height_ = h; }
};

// Caller expects a standard Rectangle behavior:
void resize(Rectangle& r) {
    r.setWidth(5);
    r.setHeight(10);
    // Assertion fails for Square! Area becomes 100 instead of 50!
    assert(r.getArea() == 50); 
}
```

> **The Fix**: A Square is mathematically a rectangle, but **behaviorally** it is not! Make `IShape` the common interface with `area()`, but do NOT inherit `Square` from `Rectangle`.

---

### 4. Interface Segregation Principle (ISP)

> *"No client should be forced to depend on methods it does not use."*

```cpp
// ❌ BAD: Fat Interface
class IMachine {
public:
    virtual ~IMachine() = default;
    virtual void print() = 0;
    virtual void scan() = 0;
    virtual void fax() = 0;
};

// BasicPrinter is forced to implement empty or throwing methods!
class BasicPrinter : public IMachine {
public:
    void print() override { /* Real print */ }
    void scan() override { throw std::runtime_error("Scan not supported!"); } // Violates ISP!
    void fax() override { throw std::runtime_error("Fax not supported!"); }
};

// ✅ GOOD: Segregated Granular Interfaces
class IPrinter {
public:
    virtual ~IPrinter() = default;
    virtual void print() = 0;
};

class IScanner {
public:
    virtual ~IScanner() = default;
    virtual void scan() = 0;
};

// BasicPrinter only implements IPrinter
class SimpleLaserPrinter final : public IPrinter {
public:
    void print() override { /* Real print */ }
};

// Multi-function hub implements both cleanly
class AllInOneOfficeHub final : public IPrinter, public IScanner {
public:
    void print() override { /* Real print */ }
    void scan() override { /* Real scan */ }
};
```

---

### 5. Dependency Inversion Principle (DIP) & Dependency Injection

> *"High-level modules should not depend on low-level modules. Both should depend on abstractions."*

```cpp
// Low-Level Abstraction
class IDatabase {
public:
    virtual ~IDatabase() = default;
    virtual void executeQuery(const std::string& query) = 0;
};

class PostgresDatabase final : public IDatabase {
public:
    void executeQuery(const std::string& query) override {
        // Run SQL on Postgres
    }
};

// High-Level Service depends ONLY on IDatabase abstraction
class OrderService {
private:
    std::shared_ptr<IDatabase> db_; // Injected dependency

public:
    // Constructor Injection
    explicit OrderService(std::shared_ptr<IDatabase> db) 
        : db_(std::move(db)) {}

    void checkout(const std::string& orderId) {
        db_->executeQuery("INSERT INTO orders VALUES ('" + orderId + "')");
    }
};
```

---

## PART 3: Composition Over Inheritance

```
THE COMBINATORIAL EXPLOSION OF INHERITANCE:
Vehicle ──► LandVehicle ──► Car
        ──► WaterVehicle ──► Boat
        ──► FlyingVehicle ──► Plane
        ──► AmphibiousCar ??? (Multiple inheritance nightmare!)
```

Instead of rigid inheritance hierarchies, compose objects of independent capability strategies:

```cpp
// Behaviors as Interfaces
class IMovementBehavior {
public:
    virtual ~IMovementBehavior() = default;
    virtual void move() = 0;
};

class DriveBehavior final : public IMovementBehavior {
public:
    void move() override { std::cout << "Driving on road\n"; }
};

class SwimBehavior final : public IMovementBehavior {
public:
    void move() override { std::cout << "Propelling through water\n"; }
};

// Vehicle is composed of dynamic behaviors
class AmphibiousVehicle {
private:
    std::vector<std::unique_ptr<IMovementBehavior>> behaviors_;

public:
    void addBehavior(std::unique_ptr<IMovementBehavior> b) {
        behaviors_.push_back(std::move(b));
    }

    void operate() {
        for (auto& b : behaviors_) b->move();
    }
};
```

---

## PART 4: Object Design Methodology (Whiteboard Framework)

In an LLD interview, use this 5-step sequence to construct your object model:

```
┌─────────────────────────────────────────────────────────────┐
│                 5-STEP OBJECT DESIGN WORKFLOW               │
└─────────────────────────────────────────────────────────────┘
  │ 1. Clarify Requirements & Identify Nouns (Classes) & Verbs (Methods)
  │ 2. Define Core Interfaces & Domain Entities
  │ 3. Establish Relationships (HAS-A Composition vs IS-A Inheritance)
  │ 4. Apply Appropriate Design Patterns (Strategy, State, Observer)
  │ 5. Implement Clean, Thread-Safe C++ Code with Smart Pointers
```

### ASCII Class Diagram Example:

```
┌─────────────────────────────────────┐
│             ParkingLot              │
├─────────────────────────────────────┤
│ - levels_: vector<ParkingLevel>     │
├─────────────────────────────────────┤
│ + parkVehicle(Vehicle*): Ticket*    │
│ + unparkVehicle(Ticket*): double    │
└──────────────────┬──────────────────┘
                   │ 1:N Composition (◆)
                   ▼
┌─────────────────────────────────────┐
│            ParkingLevel             │
├─────────────────────────────────────┤
│ - floorNumber_: int                 │
│ - spots_: vector<ParkingSpot>       │
├─────────────────────────────────────┤
│ + findAvailableSpot(Type): Spot*    │
└──────────────────┬──────────────────┘
                   │ 1:N Composition (◆)
                   ▼
┌─────────────────────────────────────┐
│             ParkingSpot             │
├─────────────────────────────────────┤
│ - spotNumber_: int                  │
│ - isOccupied_: bool                 │
│ - vehicle_: Vehicle*                │
└─────────────────────────────────────┘
```
