# SOLID Principles 🏗️

The SOLID principles are five design guidelines in Object-Oriented Programming (OOP) that make software designs more understandable, flexible, and maintainable.

---

## 1. Single Responsibility Principle (SRP)
> **"A class should have one, and only one, reason to change."**

Every class or module should take responsibility for exactly one piece of functionality.

### ❌ Violation
The `User` class below handles core user data, database persistence, and communication alerts simultaneously. If email formatting or database drivers change, the `User` class is forced to modify.

```cpp
#include <iostream>
#include <string>

class User {
private:
    std::string email;
    std::string username;

public:
    User(std::string email, std::string username) : email(email), username(username) {}

    // Reason to change 1: core properties logic
    std::string getEmail() const { return email; }

    // Reason to change 2: database persistence logic
    void saveToDatabase() {
        std::cout << "Saving user to DB...\n";
    }

    // Reason to change 3: email formatting/transmission logic
    void sendWelcomeEmail() {
        std::cout << "Sending email to " << email << "\n";
    }
};
```

###  Solution
Decompose the fat class into single-purpose components:

```cpp
#include <iostream>
#include <string>

// Core Data Model
class User {
private:
    std::string email;
    std::string username;

public:
    User(std::string email, std::string username) : email(email), username(username) {}
    std::string getEmail() const { return email; }
};

// Database Persistence Layer
class UserRepository {
public:
    void save(const User& user) {
        std::cout << "Saving user to DB...\n";
    }
};

// Email Transmission Service
class EmailService {
public:
    void sendWelcomeEmail(const User& user) {
        std::cout << "Sending email to " << user.getEmail() << "\n";
    }
};
```

---

## 2. Open/Closed Principle (OCP)
> **"Software entities should be open for extension, but closed for modification."**

You should be able to add new features or behaviors without altering existing code.

### ❌ Violation
To add a new discount type (e.g., `EmployeeDiscount`), we must modify the `DiscountCalculator` class directly, which risks breaking the tested calculations for existing types.

```cpp
#include <string>

class DiscountCalculator {
public:
    double calculateDiscount(const std::string& customerType, double amount) {
        if (customerType == "VIP") {
            return amount * 0.20;
        } else if (customerType == "Student") {
            return amount * 0.10;
        }
        return 0.0;
    }
};
```

###  Solution
Define a polymorphic abstraction and write implementations for each type. Now, adding a discount is as simple as creating a new subclass—no changes to existing calculator code are needed.

```cpp
class DiscountStrategy {
public:
    virtual ~DiscountStrategy() = default;
    virtual double calculate(double amount) const = 0;
};

class VipDiscount : public DiscountStrategy {
public:
    double calculate(double amount) const override { return amount * 0.20; }
};

class StudentDiscount : public DiscountStrategy {
public:
    double calculate(double amount) const override { return amount * 0.10; }
};

// Open for extension: can add EmployeeDiscount here without modifying this class
class DiscountCalculator {
public:
    double calculateDiscount(const DiscountStrategy& strategy, double amount) {
        return strategy.calculate(amount);
    }
};
```

---

## 3. Liskov Substitution Principle (LSP)
> **"Objects in a program should be replaceable with instances of their subtypes without altering the correctness of that program."**

A subclass should override methods from a parent class in a way that does not break the expected behavior or contract of the parent class.

### ❌ Violation
A client using `Bird` expects all subclasses to fly. Since an `Ostrich` cannot fly, substituting it breaks the program's correctness by raising unexpected errors.

```cpp
#include <iostream>
#include <stdexcept>

class Bird {
public:
    virtual ~Bird() = default;
    virtual void fly() {
        std::cout << "Flying in the air...\n";
    }
};

class Ostrich : public Bird {
public:
    void fly() override {
        // Violates LSP: throws exception where parent client expects successful fly
        throw std::logic_error("Ostriches cannot fly!");
    }
};
```

###  Solution
Remove capabilities that are not shared by all subclasses from the parent class. Segregate the flying capability into a separate interface:

```cpp
#include <iostream>

class Bird {
public:
    virtual ~Bird() = default;
    virtual void eat() {
        std::cout << "Eating food...\n";
    }
};

class FlyableBird {
public:
    virtual ~FlyableBird() = default;
    virtual void fly() = 0;
};

class Sparrow : public Bird, public FlyableBird {
public:
    void fly() override {
        std::cout << "Sparrow flying...\n";
    }
};

class Ostrich : public Bird {
    // Only inherits eat(), preventing any unexpected fly errors
};
```

---

## 4. Interface Segregation Principle (ISP)
> **"Many client-specific interfaces are better than one general-purpose interface."**

Clients should never be forced to depend on methods they do not use.

### ❌ Violation
`SimplePrinter` is forced to implement scanning and faxing operations that it does not physically support, polluting the class interface.

```cpp
#include <iostream>
#include <stdexcept>

class IMachine {
public:
    virtual ~IMachine() = default;
    virtual void print() = 0;
    virtual void scan() = 0;
    virtual void fax() = 0;
};

class SimplePrinter : public IMachine {
public:
    void print() override {
        std::cout << "Printing document...\n";
    }

    void scan() override {
        // Forced stub implementation
        throw std::logic_error("Scan not supported.");
    }

    void fax() override {
        // Forced stub implementation
        throw std::logic_error("Fax not supported.");
    }
};
```

###  Solution
Split the interface into highly focused, single-purpose contracts:

```cpp
#include <iostream>

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

class IFax {
public:
    virtual ~IFax() = default;
    virtual void fax() = 0;
};

// Simple printer only implements what it needs
class SimplePrinter : public IPrinter {
public:
    void print() override {
        std::cout << "Printing document...\n";
    }
};

// Smart multi-function hardware implements all interfaces
class MultiFunctionPrinter : public IPrinter, public IScanner, public IFax {
public:
    void print() override { std::cout << "Printing...\n"; }
    void scan() override { std::cout << "Scanning...\n"; }
    void fax() override { std::cout << "Faxing...\n"; }
};
```

---

## 5. Dependency Inversion Principle (DIP)
> **"Depend upon abstractions, not concretions."**
> 1. High-level modules should not depend on low-level modules. Both should depend on abstractions.
> 2. Abstractions should not depend on details. Details should depend on abstractions.

### ❌ Violation
`PasswordReminder` (high-level) directly depends on the concrete class `MySQLConnection` (low-level). Switching databases requires modifying the `PasswordReminder` class.

```cpp
#include <iostream>

class MySQLConnection {
public:
    void insert() { std::cout << "Inserting record into MySQL...\n"; }
};

class PasswordReminder {
private:
    MySQLConnection dbConnection;

public:
    PasswordReminder() {
        // Direct instantiation binds the high-level class to a specific implementation
    }
    void remind() {
        dbConnection.insert();
    }
};
```

###  Solution
Introduce an abstraction layer (an interface) between the classes. Decouple them by using dependency injection (constructor injection):

```cpp
#include <iostream>
#include <memory>

class IDBConnection {
public:
    virtual ~IDBConnection() = default;
    virtual void insert() = 0;
};

class MySQLConnection : public IDBConnection {
public:
    void insert() override { std::cout << "Inserting record into MySQL...\n"; }
};

class MongoDBConnection : public IDBConnection {
public:
    void insert() override { std::cout << "Inserting record into MongoDB...\n"; }
};

class PasswordReminder {
private:
    std::shared_ptr<IDBConnection> dbConnection;

public:
    // High-level class depends on interface, not the concrete implementations
    PasswordReminder(std::shared_ptr<IDBConnection> db) : dbConnection(db) {}
    
    void remind() {
        dbConnection->insert();
    }
};
```
