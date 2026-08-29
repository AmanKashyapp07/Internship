# Master Guide 03: Abstract Classes vs. Pure Interfaces in C++

> **Focus:** Pure Virtual Interfaces vs Abstract Base Classes, The Mandatory Virtual Destructor Rule, Virtual Table Dispatch Mechanics, override & final Compiler Specifiers, and The Payment Gateway Multi-Provider System Design Drill.
> 
> *Targeted for Top-Tier C++ Systems, HFT, and Backend Engineering Interviews.*

---

# Table of Contents
1. [The Master Decision Matrix: Abstract Class vs. Pure Interface](#1-the-master-decision-matrix-abstract-class-vs-pure-interface)
2. [C++ Pure Interface Architecture](#2-c-pure-interface-architecture)
3. [C++ Abstract Base Class with Template Method Pattern](#3-c-abstract-base-class-with-template-method-pattern)
4. [The Mandatory Virtual Destructor Rule](#4-the-mandatory-virtual-destructor-rule)
5. [System Design Drill: Multi-Provider Payment Gateway](#5-system-design-drill-multi-provider-payment-gateway)
6. [High-Frequency C++ Interview Drill & Verbal Q&A](#6-high-frequency-c-interview-drill--verbal-qa)

---

# 1. The Master Decision Matrix: Abstract Class vs. Pure Interface

In C++, there is no dedicated `interface` keyword like in Java or C#. Both are implemented using classes with pure virtual functions (`= 0`).

```
+---------------------------------------------------------------------------------------------------+
| ATTRIBUTE            | PURE INTERFACE CLASS (C++)            | ABSTRACT BASE CLASS (C++)          |
+---------------------------------------------------------------------------------------------------+
| Definition           | Class with ONLY pure virtual functions| Class with AT LEAST ONE pure       |
|                      | (`= 0`) and a virtual destructor      | virtual function, but may have data|
+---------------------------------------------------------------------------------------------------+
| Member Variables     | ZERO state (No instance data fields)  | YES (Contains shared state/fields) |
+---------------------------------------------------------------------------------------------------+
| Constructors         | Defaulted / None                      | YES (Initializes shared fields)    |
+---------------------------------------------------------------------------------------------------+
| Method Implementation| NO concrete methods (Pure contract)   | YES (Provides default/shared logic)|
+---------------------------------------------------------------------------------------------------+
| Multiple Inheritance | Safe & Encouraged (Mimics Java/C#     | Prone to Diamond Problem & state   |
|                      | multiple interface implementation)    | duplication without virtual inher. |
+---------------------------------------------------------------------------------------------------+
| Primary Design Role  | Decoupling contracts (ISP / DIP)      | Code reuse via Template Method     |
+---------------------------------------------------------------------------------------------------+
```

- **One-Line Intuition:** A Pure Interface is a contract stating *what* must be done; an Abstract Class is a half-built house providing shared plumbing while letting you design the rooms.
- **The Interview Trap:** Thinking an Abstract Class in C++ cannot have constructors. Abstract classes **can and should have constructors** called by derived class initialization lists to set up shared base member variables.

---

# 2. C++ Pure Interface Architecture

```cpp
// Pure Interface (Prefix with 'I')
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

// Safe Multiple Interface Implementation
class UserDocument : public ISerializable, public IPrintable {
public:
    std::string serialize() const override { return "{ \"doc\": \"data\" }"; }
    void deserialize(const std::string& data) override { /* parse */ }
    void print() const override { std::cout << "Printing document...\n"; }
};
```

---

# 3. C++ Abstract Base Class with Template Method Pattern

```cpp
// Abstract Base Class with Shared State & Reusable Algorithm Skeleton
class DatabaseClient {
protected:
    std::string connectionString;
    int timeoutMs;

public:
    DatabaseClient(std::string connStr, int timeout)
        : connectionString(std::move(connStr)), timeoutMs(timeout) {}

    virtual ~DatabaseClient() = default;

    // Template Method: Skeleton algorithm defining invariant execution order
    void executeTransaction(const std::string& sql) {
        logAudit("BEGIN TRANSACTION");
        openSocket();
        sendPayload(sql);
        closeSocket();
        logAudit("COMMIT TRANSACTION");
    }

private:
    void logAudit(const std::string& msg) {
        std::cout << "[AUDIT LOG] " << msg << "\n";
    }

    // Pure virtual hooks deferred to specific database drivers
    virtual void openSocket() = 0;
    virtual void sendPayload(const std::string& sql) = 0;
    virtual void closeSocket() = 0;
};
```

---

# 4. The Mandatory Virtual Destructor Rule

```cpp
class Base {
public:
    // Non-virtual destructor is a FATAL C++ BUG!
    ~Base() { std::cout << "Base Destructor\n"; }
};

class Derived : public Base {
private:
    int* rawBuffer;
public:
    Derived() : rawBuffer(new int[1000]) {}
    ~Derived() {
        delete[] rawBuffer;
        std::cout << "Derived Destructor Cleaned Memory\n";
    }
};

void leakMemory() {
    Base* ptr = new Derived();
    delete ptr; // ONLY ~Base() RUNS! Derived destructor is skipped! Massive memory leak!
}
```

```
Correct C++ Virtual Destructor Memory Cleanup:
delete ptr;
   |
   v
Looks up ~Derived() in Derived vtable
   |
   +---> Executes ~Derived() (frees rawBuffer)
   |
   +---> Automatically chains to ~Base()
```

- **The Rule of Thumb:** If a class has **even one virtual function**, its destructor **must be declared `virtual`**:
  ```cpp
  virtual ~Base() = default;
  ```

---

# 5. System Design Drill: Multi-Provider Payment Gateway

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
                   (Shared Idempotency, Retry, Log)
                   +------------------------------+
                   | - retryCount: int            |
                   | # generateIdempotencyKey()   |
                   | + processPayment(): bool     |
                   +------------------------------+
                                   ^
                     +-------------+-------------+
                     |                           |
               StripeGateway               PayPalGateway
           (+ executeStripeAPI)        (+ executePayPalAPI)
```

### C++ System Design Implementation:
```cpp
// 1. Pure Interface (The Contract)
class IPaymentGateway {
public:
    virtual ~IPaymentGateway() = default;
    virtual bool processPayment(const std::string& orderId, double amount) = 0;
};

// 2. Abstract Base Class (Shared Logging, Retries, Idempotency)
class AbstractPaymentProcessor : public IPaymentGateway {
protected:
    int maxRetries;

    std::string generateIdempotencyKey(const std::string& orderId) {
        return "IDEMPOTENT_KEY_" + orderId;
    }

public:
    explicit AbstractPaymentProcessor(int retries) : maxRetries(retries) {}

    bool processPayment(const std::string& orderId, double amount) override {
        std::string key = generateIdempotencyKey(orderId);
        std::cout << "[METRICS] Processing order " << orderId << " with key " << key << "\n";

        for (int attempt = 1; attempt <= maxRetries; attempt++) {
            if (sendNetworkPayment(orderId, amount, key)) {
                return true;
            }
            std::cout << "[RETRY] Attempt " << attempt << " failed. Retrying...\n";
        }
        return false;
    }

protected:
    virtual bool sendNetworkPayment(const std::string& orderId, double amount, const std::string& key) = 0;
};

// 3. Concrete Provider Adapters
class StripeGateway : public AbstractPaymentProcessor {
public:
    explicit StripeGateway() : AbstractPaymentProcessor(3) {}

protected:
    bool sendNetworkPayment(const std::string& orderId, double amount, const std::string& key) override {
        std::cout << "Executing Stripe API call for $" << amount << "\n";
        return true;
    }
};
```

---

# 6. High-Frequency C++ Interview Drill & Verbal Q&A

### Q1: Can an Abstract Class in C++ have pure virtual function implementations?
> **Answer:** **Yes.** A pure virtual function (`virtual void foo() = 0;`) can have a definition in C++ (`void Base::foo() { ... }`). Derived classes must still explicitly override it, but they can invoke the base implementation via `Base::foo()`.

### Q2: What happens if a derived class does not override all pure virtual functions of its base class?
> **Answer:** The derived class **remains an abstract class itself** and cannot be directly instantiated. Attempting to instantiate it triggers a compile-time error (`cannot allocate an object of abstract type`).

### Q3: Why does C++ not have an `interface` keyword?
> **Answer:** C++'s support for **multiple inheritance** and **pure virtual functions (`= 0`)** allows ordinary abstract classes with zero state and pure virtual methods to fulfill the exact role of interfaces without requiring a separate language keyword.

### Q4: Can a pure virtual function be declared `inline` or `constexpr` in C++?
> **Answer:** It can be `inline`, but **cannot be `constexpr`** before C++20 because dynamic virtual table dispatch requires runtime evaluation, conflicting with compile-time constant expressions.

### Q5: What is the cost difference between calling a method through a pure interface vs. an ordinary member function?
> **Answer:** An ordinary member function is a direct static assembly call (`call address`), which can be aggressively inlined. An interface call requires **one level of pointer indirection through the `vtable`** (`call *%rax`), preventing inlining unless the compiler can prove the exact concrete type (devirtualization).
