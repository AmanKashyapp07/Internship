# SOLID Principles & Object-Oriented Design Theory

> **Scope:** Formal Definitions of the 5 SOLID Architectural Principles in Modern C++, Violation Anti-Patterns, Refactored Design Patterns, Behavioral Subtyping & Contract Invariants (Preconditions / Postconditions in LSP), Interface Decoupling, and Inversion of Control (IoC / Dependency Injection).

---

# Table of Contents
1. [Architectural Overview of SOLID Principles](#1-architectural-overview-of-solid-principles)
2. [Single Responsibility Principle (SRP)](#2-single-responsibility-principle-srp)
3. [Open/Closed Principle (OCP)](#3-openclosed-principle-ocp)
4. [Liskov Substitution Principle (LSP)](#4-liskov-substitution-principle-lsp)
5. [Interface Segregation Principle (ISP)](#5-interface-segregation-principle-isp)
6. [Dependency Inversion Principle (DIP)](#6-dependency-inversion-principle-dip)
7. [Core Theoretical Summary Principles](#7-core-theoretical-summary-principles)

---

# 1. Architectural Overview of SOLID Principles

```
+---------------------------------------------------------------------------------------------------+
| PRINCIPLE                            | FORMAL OBJECT-ORIENTED DEFINITION                          |
+---------------------------------------------------------------------------------------------------+
| Single Responsibility Principle (SRP)| A class should possess one, and only one, reason to change.|
| Open/Closed Principle (OCP)          | Software entities should be open for extension, but closed |
|                                      | for modification.                                          |
| Liskov Substitution Principle (LSP)  | Subtypes must be substitutable for their base types without|
|                                      | altering the correctness of the program.                   |
| Interface Segregation Principle (ISP)| Clients should not be forced to depend on methods they do  |
|                                      | not consume.                                               |
| Dependency Inversion Principle (DIP) | High-level modules should not depend upon low-level        |
|                                      | modules; both must depend upon abstractions.               |
+---------------------------------------------------------------------------------------------------+
```

---

# 2. Single Responsibility Principle (SRP)

> *"A class should have high cohesion, encapsulating a single well-defined responsibility."*

### Anti-Pattern: Mixed Responsibilities
A single class manages user validation, database persistence, and notification dispatching:
```cpp
class UserManager {
public:
    void registerUser(const std::string& name, const std::string& email) {
        // Validation logic
        if (email.find('@') == std::string::npos) return;

        // Persistence logic
        std::cout << "Executing SQL: INSERT INTO users VALUES ('" << name << "');\n";

        // Notification logic
        std::cout << "Dispatching SMTP welcome email...\n";
    }
};
```

### Refactored Architecture: Cohesive Specialized Classes
Separates distinct concerns into orthogonal abstractions:
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

> *"Classes should allow new behavior to be added via extension (inheritance/polymorphism) without altering existing source code."*

### Anti-Pattern: Conditional Type Inspection
Introducing new shapes requires modifying existing conditional evaluation logic:
```cpp
enum class ShapeType { Circle, Rectangle, Triangle };

class AreaCalculator {
public:
    double computeArea(ShapeType type, double a, double b) {
        switch (type) {
            case ShapeType::Circle:    return 3.14159 * a * a;
            case ShapeType::Rectangle: return a * b;
            case ShapeType::Triangle:  return 0.5 * a * b;
            default: return 0.0;
        }
    }
};
```

### Refactored Architecture: Dynamic Polymorphism
```cpp
class IShape {
public:
    virtual ~IShape() = default;
    virtual double calculateArea() const = 0; // Pure virtual extension point
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

class AreaCalculator {
public:
    double computeTotalArea(const std::vector<std::unique_ptr<IShape>>& shapes) const {
        double total = 0.0;
        for (const auto& shape : shapes) {
            total += shape->calculateArea(); // Open to new shapes without modification
        }
        return total;
    }
};
```

---

# 4. Liskov Substitution Principle (LSP)

> *"Let $\phi(x)$ be a property provable about objects $x$ of type $T$. Then $\phi(y)$ should be true for objects $y$ of type $S$ where $S$ is a subtype of $T$."*

### Behavioral Contract Rules for Subtyping:
1. **Preconditions cannot be strengthened** in a subtype.
2. **Postconditions cannot be weakened** in a subtype.
3. **Class Invariants must be preserved** in a subtype.

### Anti-Pattern: The Classic Square-Rectangle LSP Violation
```cpp
class Rectangle {
protected:
    int width = 0;
    int height = 0;
public:
    virtual ~Rectangle() = default;
    virtual void setWidth(int w) { width = w; }
    virtual void setHeight(int h) { height = h; }
    int getArea() const { return width * height; }
};

class Square : public Rectangle {
public:
    void setWidth(int w) override { width = height = w; } // Mutates height; breaks Rectangle invariant!
    void setHeight(int h) override { width = height = h; }
};

void clientFunction(Rectangle& r) {
    r.setWidth(5);
    r.setHeight(4);
    assert(r.getArea() == 20); // FAILS if passed an instance of Square (getArea() returns 16)
}
```

### Refactored Architecture: Distinct Invariant Abstractions
```cpp
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

---

# 5. Interface Segregation Principle (ISP)

> *"Clients should not be forced to depend upon interface methods they do not utilize."*

### Anti-Pattern: Monolithic (Fat) Interface
```cpp
class IMultiFunctionPrinter {
public:
    virtual ~IMultiFunctionPrinter() = default;
    virtual void print(const std::string& doc) = 0;
    virtual void scan(std::string& doc) = 0;
    virtual void fax(const std::string& doc) = 0;
};

class BasicPrinter : public IMultiFunctionPrinter {
public:
    void print(const std::string& doc) override { /* Prints */ }
    void scan(std::string& doc) override { throw std::logic_error("Unsupported Operation"); }
    void fax(const std::string& doc) override { throw std::logic_error("Unsupported Operation"); }
};
```

### Refactored Architecture: Segregated Fine-Grained Interfaces
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

class BasicPrinter : public IPrinter {
public:
    void print(const std::string& doc) override {
        std::cout << "Printing...\n";
    }
};

class AllInOnePrinter : public IPrinter, public IScanner {
public:
    void print(const std::string& doc) override { std::cout << "Printing...\n"; }
    void scan(std::string& doc) override { std::cout << "Scanning...\n"; }
};
```

---

# 6. Dependency Inversion Principle (DIP)

> *"High-level policy modules should not depend on low-level detail modules. Both should depend on abstract contracts."*

### Anti-Pattern: Tight Coupling to Concrete Classes
```cpp
class StripePaymentGateway {
public:
    bool processTransaction(double amount) { return true; }
};

class CheckoutService {
private:
    StripePaymentGateway stripeGateway; // Direct hardcoded dependency
public:
    bool executeOrder(double total) {
        return stripeGateway.processTransaction(total);
    }
};
```

### Refactored Architecture: Dependency Injection via Abstract Interface
```cpp
class IPaymentGateway {
public:
    virtual ~IPaymentGateway() = default;
    virtual bool processTransaction(double amount) = 0;
};

class StripeGateway : public IPaymentGateway {
public:
    bool processTransaction(double amount) override {
        // Stripe API integration
        return true;
    }
};

class PayPalGateway : public IPaymentGateway {
public:
    bool processTransaction(double amount) override {
        // PayPal API integration
        return true;
    }
};

class CheckoutService {
private:
    std::unique_ptr<IPaymentGateway> paymentGateway;

public:
    explicit CheckoutService(std::unique_ptr<IPaymentGateway> gateway)
        : paymentGateway(std::move(gateway)) {}

    bool executeOrder(double total) {
        return paymentGateway->processTransaction(total);
    }
};
```

---

# 7. Core Theoretical Summary Principles

1. **Single Responsibility (SRP):** Encourages high cohesion and minimizes coupling by ensuring a class encapsulates a single bounded context.
2. **Open/Closed (OCP):** Promotes software stability by enabling behavioral expansion through polymorphism without modifying existing tested implementations.
3. **Liskov Substitution (LSP):** Enforces behavioral subtyping, ensuring derived classes honor the preconditions, postconditions, and invariants established by base contracts.
4. **Interface Segregation (ISP):** Avoids polluting clients with irrelevant method contracts through fine-grained role-based interfaces.
5. **Dependency Inversion (DIP):** Decouples domain policy from infrastructure dependencies by channeling all cross-boundary interactions through abstract interfaces.
