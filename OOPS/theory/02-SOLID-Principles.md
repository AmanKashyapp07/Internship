# Master Guide 02: SOLID Principles in Modern C++

> **Focus:** The 5 Foundational Principles of Object-Oriented Design in C++17/20, "Bad Code -> Violation -> Fixed C++ Code" Anti-Pattern Refactoring, and The 5-Step Interview Code-Review Diagnostic Checklist.
> 
> *Targeted for Top-Tier C++ Systems, HFT, and Backend Engineering Interviews.*

---

# Table of Contents
1. [The 5-Step SOLID Interview Diagnostic Checklist](#1-the-5-step-solid-interview-diagnostic-checklist)
2. [Single Responsibility Principle (SRP)](#2-single-responsibility-principle-srp)
3. [Open/Closed Principle (OCP)](#3-openclosed-principle-ocp)
4. [Liskov Substitution Principle (LSP)](#4-liskov-substitution-principle-lsp)
5. [Interface Segregation Principle (ISP)](#5-interface-segregation-principle-isp)
6. [Dependency Inversion Principle (DIP)](#6-dependency-inversion-principle-dip)
7. [High-Frequency C++ Interview Drill & Verbal Q&A](#7-high-frequency-c-interview-drill--verbal-qa)

---

# 1. The 5-Step SOLID Interview Diagnostic Checklist

When presented with a class in a low-level design (LLD) interview and asked *"What is wrong with this design?"*, scan in this exact order:

```
+---------------------------------------------------------------------------------------------------+
| STEP | SCAN TARGET                                       | LIKELY SOLID VIOLATION                 |
+---------------------------------------------------------------------------------------------------+
| 1    | Does the class do 3+ unrelated things (e.g. DB,   | Single Responsibility Principle (SRP)  |
|      | JSON parsing, formatting, business logic)?        |                                        |
+---------------------------------------------------------------------------------------------------+
| 2    | Does a function contain a `switch(type)` or long  | Open/Closed Principle (OCP)            |
|      | `if-else if` chain checking object types?         |                                        |
+---------------------------------------------------------------------------------------------------+
| 3    | Does a derived class throw `std::runtime_error`   | Liskov Substitution Principle (LSP)    |
|      | or leave an overridden base method as a no-op?    |                                        |
+---------------------------------------------------------------------------------------------------+
| 4    | Is a derived class forced to implement fat pure   | Interface Segregation Principle (ISP)  |
|      | virtual methods it has no use for?                |                                        |
+---------------------------------------------------------------------------------------------------+
| 5    | Does a high-level class instantiate concrete      | Dependency Inversion Principle (DIP)   |
|      | low-level dependencies using `new Concrete()`?    |                                        |
+---------------------------------------------------------------------------------------------------+
```

---

# 2. Single Responsibility Principle (SRP)

> *"A class should have one, and only one, reason to change."*

### Bad C++ Design (Violates SRP):
```cpp
class UserManager {
public:
    void registerUser(const std::string& name, const std::string& email) {
        // Reason 1 to change: Validation business rules change
        if (email.find('@') == std::string::npos) return;

        // Reason 2 to change: Database schema or SQL driver changes
        std::cout << "Executing: INSERT INTO users VALUES ('" << name << "');\n";

        // Reason 3 to change: Email provider / SMTP template changes
        std::cout << "Sending welcome email via SendGrid...\n";
    }
};
```

### Fixed C++ Design (Adheres to SRP):
```cpp
class UserValidator {
public:
    static bool isValid(const std::string& email) {
        return email.find('@') != std::string::npos;
    }
};

class IUserRepository {
public:
    virtual ~IUserRepository() = default;
    virtual void save(const std::string& name, const std::string& email) = 0;
};

class IEmailService {
public:
    virtual ~IEmailService() = default;
    virtual void sendWelcomeEmail(const std::string& email) = 0;
};

class UserService {
private:
    std::unique_ptr<IUserRepository> userRepo;
    std::unique_ptr<IEmailService> emailService;

public:
    UserService(std::unique_ptr<IUserRepository> repo, std::unique_ptr<IEmailService> email)
        : userRepo(std::move(repo)), emailService(std::move(email)) {}

    void registerUser(const std::string& name, const std::string& email) {
        if (!UserValidator::isValid(email)) return;
        userRepo->save(name, email);
        emailService->sendWelcomeEmail(email);
    }
};
```

---

# 3. Open/Closed Principle (OCP)

> *"Software entities should be open for extension, but closed for modification."*

### Bad C++ Design (Violates OCP):
```cpp
enum class ShapeType { Circle, Rectangle, Triangle };

class AreaCalculator {
public:
    double computeArea(ShapeType type, double a, double b) {
        // Every new shape requires MODIFYING this existing function (breaking OCP)!
        switch (type) {
            case ShapeType::Circle:    return 3.14159 * a * a;
            case ShapeType::Rectangle: return a * b;
            case ShapeType::Triangle:  return 0.5 * a * b;
            default: return 0.0;
        }
    }
};
```

### Fixed C++ Design (Adheres to OCP via Polymorphism):
```cpp
class IShape {
public:
    virtual ~IShape() = default;
    virtual double calculateArea() const = 0; // Open for extension
};

class Circle : public IShape {
private:
    double radius;
public:
    explicit Circle(double r) : radius(r) {}
    double calculateArea() const override { return 3.14159 * radius * radius; }
};

class Rectangle : public IShape {
private:
    double width, height;
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    double calculateArea() const override { return width * height; }
};

// Adding a new Shape requires ZERO modifications to existing AreaCalculator code!
class AreaCalculator {
public:
    double computeTotalArea(const std::vector<std::unique_ptr<IShape>>& shapes) {
        double total = 0.0;
        for (const auto& shape : shapes) total += shape->calculateArea();
        return total;
    }
};
```

---

# 4. Liskov Substitution Principle (LSP)

> *"Functions that use pointers or references to base classes must be able to use objects of derived classes without knowing it and without altering program correctness."*

### Bad C++ Design (The Classic Square-Rectangle LSP Trap):
```cpp
class Rectangle {
protected:
    int width, height;
public:
    virtual void setWidth(int w) { width = w; }
    virtual void setHeight(int h) { height = h; }
    int getArea() const { return width * height; }
};

class Square : public Rectangle {
public:
    void setWidth(int w) override { width = height = w; } // Mutates height unexpectedly!
    void setHeight(int h) override { width = height = h; } // Mutates width unexpectedly!
};

void clientTest(Rectangle& r) {
    r.setWidth(5);
    r.setHeight(4);
    assert(r.getArea() == 20); // FAILS IF PASSED A SQUARE (Area becomes 16)! VIOLATES LSP!
}
```

### Fixed C++ Design (Adheres to LSP):
```cpp
// Square is NOT a mutable Rectangle because their behavioral invariants differ!
class IShape {
public:
    virtual ~IShape() = default;
    virtual int getArea() const = 0;
};

class Rectangle : public IShape {
private:
    int width, height;
public:
    Rectangle(int w, int h) : width(w), height(h) {}
    int getArea() const override { return width * height; }
};

class Square : public IShape {
private:
    int side;
public:
    explicit Square(int s) : side(s) {}
    int getArea() const override { return side * side; }
};
```

- **One-Line Intuition:** If it looks like a duck and quacks like a duck, but needs batteries, you have the wrong abstraction.

---

# 5. Interface Segregation Principle (ISP)

> *"Clients should not be forced to depend upon interfaces that they do not use."*

### Bad C++ Design (Violates ISP):
```cpp
// Fat interface forcing empty implementations
class IMultiFunctionPrinter {
public:
    virtual ~IMultiFunctionPrinter() = default;
    virtual void print(const std::string& doc) = 0;
    virtual void scan(std::string& doc) = 0;
    virtual void fax(const std::string& doc) = 0;
};

class BasicPrinter : public IMultiFunctionPrinter {
public:
    void print(const std::string& doc) override { std::cout << "Printing...\n"; }
    void scan(std::string& doc) override { throw std::logic_error("Not supported!"); } // Bad!
    void fax(const std::string& doc) override { throw std::logic_error("Not supported!"); }
};
```

### Fixed C++ Design (Adheres to ISP):
```cpp
class IPrinter {
public:
    virtual ~IPrinter() = default;
    virtual void print(const std::string& doc) = 0;
};

class IScanner {
public:
    virtual ~IScanner() = default;
    virtual void scan(std::string& doc) = 0;
};

// Basic printer implements only what it supports
class BasicPrinter : public IPrinter {
public:
    void print(const std::string& doc) override { std::cout << "Printing...\n"; }
};

// All-In-One printer composes multiple fine-grained interfaces
class AdvancedPrinter : public IPrinter, public IScanner {
public:
    void print(const std::string& doc) override { std::cout << "Printing...\n"; }
    void scan(std::string& doc) override { std::cout << "Scanning...\n"; }
};
```

---

# 6. Dependency Inversion Principle (DIP)

> *"High-level modules should not depend on low-level modules. Both should depend on abstractions."*

### Bad C++ Design (Violates DIP):
```cpp
class StripePaymentGateway {
public:
    void pay(double amount) { std::cout << "Paid via Stripe\n"; }
};

class CheckoutService {
private:
    StripePaymentGateway stripe; // Hardcoded concrete dependency (Cannot swap with PayPal!)
public:
    void processOrder(double total) {
        stripe.pay(total);
    }
};
```

### Fixed C++ Design (Adheres to DIP via Dependency Injection):
```cpp
// Abstraction owned by domain logic
class IPaymentGateway {
public:
    virtual ~IPaymentGateway() = default;
    virtual bool processPayment(double amount) = 0;
};

// Low-level plugin implementation 1
class StripeGateway : public IPaymentGateway {
public:
    bool processPayment(double amount) override {
        std::cout << "Processing $" << amount << " via Stripe\n";
        return true;
    }
};

// Low-level plugin implementation 2
class PayPalGateway : public IPaymentGateway {
public:
    bool processPayment(double amount) override {
        std::cout << "Processing $" << amount << " via PayPal\n";
        return true;
    }
};

// High-level module depends ONLY on IPaymentGateway interface
class CheckoutService {
private:
    std::unique_ptr<IPaymentGateway> paymentGateway;

public:
    explicit CheckoutService(std::unique_ptr<IPaymentGateway> gateway)
        : paymentGateway(std::move(gateway)) {}

    bool processOrder(double total) {
        return paymentGateway->processPayment(total);
    }
};
```

---

# 7. High-Frequency C++ Interview Drill & Verbal Q&A

### Q1: What is the difference between Dependency Inversion (DIP) and Dependency Injection (DI)?
> **Answer:** **DIP is the architectural principle** stating high-level modules must depend on abstractions rather than concretions. **DI is the design pattern/technique** used to supply dependencies (e.g. passing a `std::unique_ptr<IPaymentGateway>` via constructor) rather than letting the class instantiate it directly.

### Q2: How does Liskov Substitution Principle relate to C++ contracts?
> **Answer:** LSP requires that derived classes **cannot strengthen preconditions** (e.g. demanding stricter argument ranges) and **cannot weaken postconditions** (e.g. failing to guarantee return invariants) of base class virtual methods.

### Q3: Why is OCP critical in large-scale C++ codebases?
> **Answer:** Modifying existing header files forces **recompilation of all dependent translation units (`.cpp` files)**. Designing for extension via pure virtual interfaces allows new derived classes to be added in separate files without triggering full project recompiles.

### Q4: Does applying Single Responsibility Principle mean every class should have only one method?
> **Answer:** **No.** A class should have high **cohesion**, meaning all its member functions work together toward a single unified business responsibility (e.g. a `UserRepository` may have `find()`, `save()`, and `delete()`, but all deal exclusively with user persistence).

### Q5: How do C++ templates achieve OCP at compile-time without virtual functions?
> **Answer:** Via **C++20 Concepts and Policy-Based Design**. High-level algorithms accept template parameters constrained by concepts (e.g. `template <Printable T> void log(T val)`), allowing new types to be plugged in without modifying the template algorithm and with zero vtable overhead.
