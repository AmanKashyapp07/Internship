# 01. OOP Core & SOLID Principles in System Design

> **Interview Mindset**: In Low-Level Design (LLD) interviews, nobody will ask you to recite textbook definitions of *"What is Polymorphism?"* 
> Instead, the interviewer evaluates whether you write **clean, decoupled, extensible C++ code** using abstract interfaces, smart pointers (`std::unique_ptr`, `std::shared_ptr`), and explicit contracts that handle new requirements without breaking existing functionality.

---

## 1. The 4 Pillars Applied to Real System Design

Forget generic school examples (e.g., `Dog extends Animal`). Here is how the 4 pillars function inside real production systems:

```
┌──────────────────────────────────────────────────────────────────────────────────────────┐
│                           The 4 Pillars in System Architecture                           │
└──────────────────────────────────────────────────────────────────────────────────────────┘
           │                        │                         │                       │
           ▼                        ▼                         ▼                       ▼
┌───────────────────────┐┌───────────────────────┐┌───────────────────────┐┌───────────────────────┐
│     1. Abstraction    ││   2. Encapsulation    ││    3. Polymorphism    ││  4. Composition / Inh │
│                       ││                       ││                       ││                       │
│  Defines the 'What',  ││  Protects internal    ││  Eliminates branching ││  Flexible Has-A over  │
│    hides the 'How'    ││   state & invariants  ││     if-else chains    ││   rigid Is-A classes  │
└───────────────────────┘└───────────────────────┘└───────────────────────┘└───────────────────────┘
```

### 1. Abstraction (The Public Contract)
- **Concept**: Hide complex internal mechanics behind abstract base classes (`pure virtual` functions). Callers interact strictly with contracts, never concrete implementations.
- **System Example**: A `PaymentProcessor` interface with `virtual void processPayment(double amount) = 0;`. The caller doesn't care whether underneath it performs RSA tokenization, calls Stripe REST APIs, or executes a bank transfer.

### 2. Encapsulation (Protecting Invariants)
- **Concept**: Bundle state and behavior together; keep state `private`. Modifications must pass through validated public methods to preserve system invariants.
- **Anti-Pattern**: Exposing public mutable lists or public fields that allow foreign classes to corrupt internal state (e.g., `order.totalPrice = -50`).
- **Best Practice (C++)**:
  ```cpp
  #include <iostream>
  #include <mutex>
  #include <stdexcept>

  class BankAccount {
  private:
      double balance{0.0};
      std::mutex mtx; // Thread-safe state protection

  public:
      explicit BankAccount(double initialBalance) : balance(initialBalance) {}

      void withdraw(double amount) {
          std::lock_guard<std::mutex> lock(mtx);
          if (amount <= 0.0 || amount > balance) {
              throw std::invalid_argument("Invalid withdrawal amount");
          }
          balance -= amount;
      }

      double getBalance() {
          std::lock_guard<std::mutex> lock(mtx);
          return balance;
      }
  };
  ```

### 3. Polymorphism (The Switch-Killer)
- **Concept**: Treat different specialized objects through a unified base pointer/reference with `virtual` methods.
- **Senior Rule**: If you see a growing `switch (type)` or `if (type == "CREDIT_CARD")` chain in your code, you have violated Polymorphism. Replace it with a polymorphic interface!

```
┌─────────────────────────────────────────┐
│           Notification Service          │
└────────────────────┬────────────────────┘
                     │ Calls notifyUser() via base pointer
                     ▼
┌─────────────────────────────────────────┐
│       << Abstract Base Class >>         │
│          NotificationChannel            │
└────────────────────┬────────────────────┘
                     │
       ┌─────────────┼─────────────┐
       ▼             ▼             ▼
┌─────────────┐┌─────────────┐┌─────────────┐
│    Email    ││     SMS     ││    Push     │
│ Notification││ Notification││ Notification│
└─────────────┘└─────────────┘└─────────────┘
```

### 4. Composition over Inheritance (The Golden Rule)
- **Why Inheritance Fails at Scale**:
  - **Tight Coupling**: Changes in base class break derived classes (Fragile Base Class problem).
  - **Class Explosion**: If you have `Vehicle`, `ElectricVehicle`, `FlyingVehicle`, `ElectricFlyingVehicle`, combining behaviors leads to exponential subclasses.
- **The Solution (Composition)**: Build classes using **Has-A** rather than **Is-A**. A `Car` *has* an `Engine` and *has* a `DriveStrategy`. You can swap strategies at runtime via smart pointers (`std::unique_ptr`) without touching the `Car` class.

---

## 2. SOLID Principles: The Interview Decoded Edition

```
S ──► Single Responsibility Principle (One reason to change)
O ──► Open/Closed Principle (Open for extension, closed for modification)
L ──► Liskov Substitution Principle (Subtypes must be substitutable for base types)
I ──► Interface Segregation Principle (Many small client-specific interfaces)
D ──► Dependency Inversion Principle (Depend on abstractions, not concretions)
```

---

### S: Single Responsibility Principle (SRP)
> *"A class should have one, and only one, reason to change."*

#### ❌ The Anti-Pattern (God Object):
```cpp
class UserService {
public:
    void registerUser(const std::string& email, const std::string& password) {
        // 1. Validate email
        if (email.find('@') == std::string::npos) throw std::invalid_argument("Bad email");
        // 2. Direct database persistence
        dbConnection.execute("INSERT INTO users VALUES (...)");
        // 3. Network SMTP call
        smtpClient.sendEmail(email, "Welcome to our platform!");
    }
private:
    DatabaseConnection dbConnection;
    SMTPClient smtpClient;
};
```
*Why this fails*: `UserService` changes if validation rules change, if database technology changes, OR if email templates change.

#### ✅ The Refactored Design:
```
┌────────────────────────────────────────────────────────┐
│                      UserService                       │
│                 (Coordinates Workflow)                 │
└──────────────┬───────────────────┬─────────────────────┘
               │                   │
         ┌─────┘                   └─────┐
         ▼                               ▼
┌──────────────────┐           ┌──────────────────┐
│  UserValidator   │           │  UserRepository  │
│ (Validates Input)│           │ (DB Persistence) │
└──────────────────┘           └──────────────────┘
               │
               ▼
┌──────────────────┐
│   EmailService   │
│  (SMTP Delivery) │
└──────────────────┘
```

---

### O: Open/Closed Principle (OCP)
> *"Software entities should be open for extension, but closed for modification."*

You should be able to introduce a new feature by writing **new code**, not by modifying working, tested existing code.

#### ❌ The Anti-Pattern:
```cpp
class PaymentGateway {
public:
    void pay(const std::string& type, double amount) {
        if (type == "CREDIT_CARD") { payWithCard(amount); }
        else if (type == "PAYPAL") { payWithPaypal(amount); }
        else if (type == "CRYPTO") { payWithCrypto(amount); } // Modified existing file!
    }
};
```

#### ✅ The Refactored Design (Strategy Pattern):
```
┌──────────────────────────────────────┐
│            PaymentGateway            │
└──────────────────┬───────────────────┘
                   │ Executes Strategy
                   ▼
┌──────────────────────────────────────┐
│       << Interface Contract >>       │
│           PaymentStrategy            │
└──────────────────┬───────────────────┘
                   │
      ┌────────────┼────────────┐───────────────────────┐
      ▼            ▼            ▼                       ▼
┌───────────┐┌───────────┐┌───────────┐       ┌───────────────────┐
│CreditCard ││  PayPal   ││    UPI    │       │   CryptoPayment   │
│  Payment  ││  Payment  ││  Payment  │       │ (Added with ZERO  │
│           ││           ││           │       │edit to Gateway!)  │
└───────────┘└───────────┘└───────────┘       └───────────────────┘
```

---

### L: Liskov Substitution Principle (LSP)
> *"Subtypes must be substitutable for their base types without altering system correctness."*

#### ❌ The Classic Trap (Square extends Rectangle):
```cpp
class Rectangle {
protected:
    int width{0}, height{0};
public:
    virtual void setWidth(int w) { width = w; }
    virtual void setHeight(int h) { height = h; }
    int getArea() const { return width * height; }
    virtual ~Rectangle() = default;
};

class Square : public Rectangle {
public:
    void setWidth(int w) override { width = w; height = w; } // Violates expectations!
    void setHeight(int h) override { width = h; height = h; }
};
```
*Why this breaks*: A function that expects a `Rectangle` and sets `rect->setWidth(5); rect->setHeight(10);` expects area 50. If passed a `Square`, the area becomes 100!

#### ✅ The Fix:
Separate hierarchies into distinct classes implementing a common `Shape` interface with `virtual int getArea() const = 0;`. Never inherit just to reuse a few fields.

---

### I: Interface Segregation Principle (ISP)
> *"Clients should not be forced to depend upon interfaces that they do not use."*

#### ❌ The Anti-Pattern (Fat Interface):
```cpp
class Worker {
public:
    virtual void work() = 0;
    virtual void eat() = 0;
    virtual void sleep() = 0;
    virtual ~Worker() = default;
};

class RobotWorker : public Worker {
public:
    void work() override { /* Working */ }
    void eat() override { throw std::logic_error("Robots do not eat!"); }
    void sleep() override { throw std::logic_error("Robots do not sleep!"); }
};
```

#### ✅ The Refactored Design:
Break into granular, role-focused interfaces:
```
┌─────────────────────┐                   ┌─────────────────────┐
│     HumanWorker     │                   │     RobotWorker     │
└──────────┬──────────┘                   └──────────┬──────────┘
           │                                         │
           ├─────────────────────┐                   │
           ▼                     ▼                   ▼
┌─────────────────────┐┌─────────────────────┐ ┌───────────────────┐
│  << Interface >>    ││  << Interface >>    │ │  (Robots only     │
│      Workable       ││      Eatable        │ │   implement       │
│       work()        ││       eat()         │ │   Workable!)      │
└─────────────────────┘└─────────────────────┘ └───────────────────┘
```

---

### D: Dependency Inversion Principle (DIP)
> *"High-level modules should not depend on low-level modules. Both should depend on abstractions."*

#### ❌ The Anti-Pattern (Hardcoded Dependency):
```cpp
class MySQLDatabase {
public:
    void save(const std::string& data) { /* MySQL driver execution */ }
};

class OrderService {
private:
    MySQLDatabase database; // Tight coupling to MySQL!
};
```

#### ✅ The Refactored Design (Dependency Injection):
```cpp
#include <memory>
#include <string>

// Abstraction
class Database {
public:
    virtual void save(const std::string& data) = 0;
    virtual ~Database() = default;
};

class MySQLDatabase : public Database {
public:
    void save(const std::string& data) override { /* ... */ }
};

class OrderService {
private:
    std::shared_ptr<Database> db; // Depends on abstraction

public:
    // Injected via constructor
    explicit OrderService(std::shared_ptr<Database> database) : db(std::move(database)) {}

    void createOrder(const std::string& orderDetails) {
        db->save(orderDetails);
    }
};
```

```
┌──────────────────────────────────────────────┐
│         High-Level: OrderService             │
└──────────────────────┬───────────────────────┘
                       │ Depends on Abstraction
                       ▼
┌──────────────────────────────────────────────┐
│           << Interface >> Database           │
└──────────────────────┬───────────────────────┘
                       │ Implemented by
          ┌────────────┴────────────┐
          ▼                         ▼
┌────────────────────┐    ┌────────────────────┐
│   MySQLDatabase    │    │  PostgresDatabase  │
└────────────────────┘    └────────────────────┘
```

---

## 3. UML Class Relationships: Visual Quick Guide

In an LLD interview, draw these relationships clearly on the whiteboard or digital canvas:

```
1. Inheritance / Generalization (Is-A)
┌──────────────┐                       ┌──────────────┐
│     Car      ├──────────────────────►│   Vehicle    │
└──────────────┘                       └──────────────┘

2. Realization / Implementation
┌──────────────┐                       ┌──────────────┐
│ MySQLAdapter ├ - - - - - - - - - - -►│   Database   │
└──────────────┘                       └──────────────┘

3. Composition (Contains-A: Strong Lifecycle)
┌──────────────┐                       ┌──────────────┐
│    House     ├──────────────────────►│     Room     │  (If House is destroyed,
└──────────────┘                       └──────────────┘   Rooms are destroyed)

4. Aggregation (Has-A: Weak Lifecycle)
┌──────────────┐                       ┌──────────────┐
│  Department  ├──────────────────────►│  Professor   │  (Professor exists
└──────────────┘                       └──────────────┘   independently)
```

| Relationship | Meaning | C++ Representation | Lifecycle Dependency |
| :--- | :--- | :--- | :--- |
| **Inheritance (Is-A)** | Subclass specializes base class | `class Car : public Vehicle` | Tightest coupling |
| **Realization** | Class implements pure abstract class | `class MySQL : public Database` | Contract bound |
| **Composition (Owns-A)** | Child cannot exist without parent | Object stored by value or `std::unique_ptr` | If parent dies, child dies |
| **Aggregation (Has-A)** | Child can exist independently | Stored via `std::shared_ptr` or raw pointer | Independent lifecycles |
| **Association (Uses-A)** | Temporary usage or method param | `void print(const Printer& p)` | Weakest connection |

---

## 4. Interview Cheat Sheet: The 60-Second Sanity Check

Before writing code in an LLD interview, ask yourself these 4 questions:

```
1. "Am I using if-else or switch for types? Can I replace it with Polymorphism?"
2. "Does my class have more than one reason to change? Should I split it?"
3. "Are base class destructors marked 'virtual' to prevent memory leaks?"
4. "Am I passing dependencies via constructor injection (smart pointers) rather than hardcoding 'new'?"
```
