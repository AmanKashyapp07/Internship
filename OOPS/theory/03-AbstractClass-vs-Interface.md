# Abstract Classes & Pure Virtual Interfaces in C++

> **Scope:** Architectural Comparison of Abstract Classes vs. Pure Interfaces, Pure Virtual Functions (`= 0`), The Mandatory Virtual Destructor Rule, Template Method Design Pattern, Multiple Interface Inheritance, Pure Virtual Function Implementations, and Virtual Table Dynamic Dispatch Overhead.

---

# Table of Contents
1. [Architectural Comparison: Abstract Class vs. Pure Interface](#1-architectural-comparison-abstract-class-vs-pure-interface)
2. [C++ Pure Virtual Interface Implementation](#2-c-pure-virtual-interface-implementation)
3. [Abstract Base Classes & The Template Method Pattern](#3-abstract-base-classes--the-template-method-pattern)
4. [The Virtual Destructor Invariant](#4-the-virtual-destructor-invariant)
5. [Pure Virtual Functions with Definitions](#5-pure-virtual-functions-with-definitions)
6. [Multi-Tier Provider Hierarchy Case Study](#6-multi-tier-provider-hierarchy-case-study)
7. [Core Theoretical Summary Principles](#7-core-theoretical-summary-principles)

---

# 1. Architectural Comparison: Abstract Class vs. Pure Interface

In C++, pure interfaces and abstract classes are both constructed using pure virtual member functions (`= 0`), but serve distinct architectural roles:

```
+---------------------------------------------------------------------------------------------------+
| ATTRIBUTE            | PURE VIRTUAL INTERFACE                | ABSTRACT BASE CLASS                |
+---------------------------------------------------------------------------------------------------+
| Definition           | Class containing ONLY pure virtual    | Class containing AT LEAST ONE pure |
|                      | methods (`= 0`) and virtual destructor| virtual method, plus member data   |
+---------------------------------------------------------------------------------------------------+
| State Storage        | ZERO member variables (Stateless)     | Permitted to hold shared state     |
+---------------------------------------------------------------------------------------------------+
| Constructors         | Defaulted or omitted                  | Explicit constructors initialized  |
|                      |                                       | via derived initializer lists      |
+---------------------------------------------------------------------------------------------------+
| Concrete Logic       | No concrete default implementations   | Provides common behavioral logic   |
+---------------------------------------------------------------------------------------------------+
| Multiple Inheritance | Safe (Zero state duplication)         | Prone to diamond ambiguity without |
|                      |                                       | virtual base inheritance           |
+---------------------------------------------------------------------------------------------------+
| Design Role          | Decouples interface contracts (ISP)   | Code reuse via Template Method     |
+---------------------------------------------------------------------------------------------------+
```

---

# 2. C++ Pure Virtual Interface Implementation

A pure interface defines an immutable contract that implementing types must satisfy:

```cpp
// Pure Interface (Stateless Contract)
class ISerializable {
public:
    virtual ~ISerializable() = default; // Mandatory virtual destructor
    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& data) = 0;
};

class IPrintable {
public:
    virtual ~IPrintable() = default;
    virtual void print() const = 0;
};

// Concrete class implementing multiple interfaces safely
class Document : public ISerializable, public IPrintable {
public:
    std::string serialize() const override {
        return "{ \"payload\": \"content\" }";
    }

    void deserialize(const std::string& data) override {
        // Parse payload
    }

    void print() const override {
        std::cout << "Printing document payload...\n";
    }
};
```

---

# 3. Abstract Base Classes & The Template Method Pattern

An Abstract Base Class models common state and invariant algorithmic structure while deferring specialized steps to derived classes:

```cpp
class DatabaseClient {
protected:
    std::string connectionString;
    int timeoutMs;

public:
    DatabaseClient(std::string connStr, int timeout)
        : connectionString(std::move(connStr)), timeoutMs(timeout) {}

    virtual ~DatabaseClient() = default;

    // Template Method: Fixes the invariant transaction pipeline structure
    void executeTransaction(const std::string& query) {
        logAudit("BEGIN TRANSACTION");
        openSocket();
        sendPayload(query);
        closeSocket();
        logAudit("COMMIT TRANSACTION");
    }

private:
    void logAudit(const std::string& msg) {
        std::cout << "[AUDIT] " << msg << "\n";
    }

    // Pure virtual primitive operations deferred to specific driver implementations
    virtual void openSocket() = 0;
    virtual void sendPayload(const std::string& query) = 0;
    virtual void closeSocket() = 0;
};
```

---

# 4. The Virtual Destructor Invariant

When deleting a derived object via a pointer to its base class, dynamic dispatch must resolve the derived destructor first before unwinding base members.

```cpp
class Base {
public:
    // Non-virtual destructor violates polymorphic destruction invariant!
    ~Base() { std::cout << "Base Destructor\n"; }
};

class Derived : public Base {
private:
    int* buffer;
public:
    Derived() : buffer(new int[1024]) {}
    ~Derived() {
        delete[] buffer;
        std::cout << "Derived Destructor Cleaned Buffer\n";
    }
};

void executeCleanup() {
    Base* ptr = new Derived();
    delete ptr; // Undefined Behavior: Executes ONLY ~Base(); ~Derived() is skipped!
}
```

```
Correct Polymorphic Virtual Destruction Sequence:
delete ptr;
   |
   v
Lookup ~Derived() in Derived vtable
   |
   +---> Executes ~Derived() (Releases heap buffer)
   |
   +---> Chains invocation to ~Base()
```

### The Invariant:
Any class providing virtual member functions **must declare a `virtual` destructor**:
```cpp
virtual ~Base() = default;
```

---

# 5. Pure Virtual Functions with Definitions

In C++, a pure virtual function (`= 0`) can optionally provide a body definition. Derived classes must still override the method explicitly, but they can invoke the base implementation to execute common shared logic:

```cpp
class AbstractLogger {
public:
    virtual ~AbstractLogger() = default;
    virtual void log(const std::string& message) = 0;
};

// Pure virtual function body implementation
void AbstractLogger::log(const std::string& message) {
    std::cout << "[BASE TIMESTAMP] " << message << "\n";
}

class FileLogger : public AbstractLogger {
public:
    void log(const std::string& message) override {
        AbstractLogger::log(message); // Invoke base default implementation
        // Specialized file write operations...
    }
};
```

---

# 6. Multi-Tier Provider Hierarchy Case Study

```
                             <<interface>>
                            IPaymentGateway
                      +---------------------------+
                      | + processPayment(): bool  |
                      | + refundPayment(): bool   |
                      +---------------------------+
                                    ^
                                    |
                        AbstractPaymentProcessor
                    (Shared Idempotency & Retry Loop)
                    +------------------------------+
                    | # maxRetries: int            |
                    | # generateIdempotencyKey()   |
                    | + processPayment(): bool     |
                    +------------------------------+
                                    ^
                      +-------------+-------------+
                      |                           |
                StripeGateway               PayPalGateway
            (+ executeStripeAPI)        (+ executePayPalAPI)
```

```cpp
class IPaymentGateway {
public:
    virtual ~IPaymentGateway() = default;
    virtual bool processPayment(const std::string& orderId, double amount) = 0;
};

class AbstractPaymentProcessor : public IPaymentGateway {
protected:
    int maxRetries;

    std::string generateIdempotencyKey(const std::string& orderId) const {
        return "IDEMPOTENCY_" + orderId;
    }

public:
    explicit AbstractPaymentProcessor(int retries) : maxRetries(retries) {}

    bool processPayment(const std::string& orderId, double amount) override {
        std::string key = generateIdempotencyKey(orderId);

        for (int attempt = 1; attempt <= maxRetries; ++attempt) {
            if (sendNetworkRequest(orderId, amount, key)) {
                return true;
            }
        }
        return false;
    }

protected:
    virtual bool sendNetworkRequest(const std::string& orderId, double amount, const std::string& key) = 0;
};

class StripeGateway : public AbstractPaymentProcessor {
public:
    StripeGateway() : AbstractPaymentProcessor(3) {}

protected:
    bool sendNetworkRequest(const std::string& orderId, double amount, const std::string& key) override {
        // Concrete Stripe API payload dispatch
        return true;
    }
};
```

---

# 7. Core Theoretical Summary Principles

1. **Pure Interfaces:** Pure virtual contracts without member variables decouple components without introducing multiple-inheritance state collisions.
2. **Abstract Base Classes:** Combine shared state and invariant template methods with pure virtual extension hooks.
3. **Destructor Virtualization:** Deleting polymorphic objects via base pointers requires virtual destructors to prevent resource leaks and undefined behavior.
4. **Pure Virtual Definitions:** Pure virtual methods can define base logic that derived overrides invoke explicitly.
5. **Dynamic Dispatch Latency:** Interface calls incur one level of pointer indirection through the `vtable`, precluding inlining unless devirtualized by the compiler.
