# 03. Core Design Patterns for Low-Level Design (LLD)

> **Interview Mindset**: Do not recite pattern definitions like a textbook. Explain **the concrete problem (anti-pattern)**, **why naive inheritance fails**, and **how the pattern decouples the architecture** using modern C++ smart pointers.

---

## The 8 Essential Interview Design Patterns

```
┌─────────────────┬─────────────────┬──────────────────────────────────────────┐
│ Pattern         │ Category        │ Core Intent                              │
├─────────────────┼─────────────────┼──────────────────────────────────────────┤
│ 1. Factory      │ Creational      │ Decouple object creation from business   │
│ 2. Builder      │ Creational      │ Construct complex objects step-by-step   │
│ 3. Singleton    │ Creational      │ Ensure exactly one instance + global pt  │
│ 4. Strategy     │ Behavioral      │ Interchangeable algorithms at runtime    │
│ 5. Observer     │ Behavioral      │ 1:N pub-sub event notification system    │
│ 6. Command      │ Behavioral      │ Encapsulate action as object (Undo/Redo) │
│ 7. Adapter      │ Structural      │ Convert incompatible interface to target │
│ 8. Decorator    │ Structural      │ Attach dynamic responsibilities flexibly │
└─────────────────┴─────────────────┴──────────────────────────────────────────┘
```

---

## 1. Factory Pattern (Decoupling Object Instantiation)

- **Problem**: Client code directly calls `new CreditCardPayment()`, `new UPIPayment()`. Adding a new payment type requires modifying every client file.
- **Solution**: Centralize object creation behind a factory interface returning `std::unique_ptr<IType>`.

```
┌──────────────────┐               ┌──────────────────┐
│  PaymentClient   ├──────────────►│  PaymentFactory  │
└──────────────────┘               └────────┬─────────┘
                                            │ Creates
                                            ▼
                                   ┌──────────────────┐
                                   │ IPayment (Pure)  │
                                   └────────▲─────────┘
                                   ┌────────┴─────────┐
                                   ▼                  ▼
                           ┌──────────────┐   ┌──────────────┐
                           │ CreditCard   │   │ UPIPayment   │
                           └──────────────┘   └──────────────┘
```

```cpp
#include <iostream>
#include <memory>
#include <string>

// Abstract Product
class IPayment {
public:
    virtual ~IPayment() = default;
    virtual void pay(double amount) = 0;
};

class CreditCardPayment final : public IPayment {
public:
    void pay(double amount) override { 
        std::cout << "Paid $" << amount << " via Credit Card.\n"; 
    }
};

class UPIPayment final : public IPayment {
public:
    void pay(double amount) override { 
        std::cout << "Paid $" << amount << " via UPI.\n"; 
    }
};

// Factory
enum class PaymentType { CREDIT_CARD, UPI };

class PaymentFactory {
public:
    static std::unique_ptr<IPayment> create(PaymentType type) {
        switch (type) {
            case PaymentType::CREDIT_CARD: 
                return std::make_unique<CreditCardPayment>();
            case PaymentType::UPI: 
                return std::make_unique<UPIPayment>();
        }
        return nullptr;
    }
};
```

---

## 2. Builder Pattern (Constructing Complex Objects)

- **Problem**: **Telescoping Constructor Anti-Pattern** (`Computer(cpu, ram, disk, gpu, wifi, bluetooth, cooler)`). Passing 8 parameters with lots of `nullptr` / `false` is unreadable and error-prone.
- **Solution**: Fluent builder with method chaining returning `*this`.

```cpp
class Computer {
public:
    std::string cpu;
    int ramGB{0};
    int diskGB{0};
    bool hasGPU{false};
    bool hasWifi{false};

    void display() const {
        std::cout << "PC: CPU=" << cpu << ", RAM=" << ramGB << "GB, Disk=" 
                  << diskGB << "GB, GPU=" << (hasGPU ? "Yes" : "No") << "\n";
    }
};

// Fluent Builder
class ComputerBuilder {
private:
    Computer comp_;
public:
    ComputerBuilder& setCPU(std::string cpu) {
        comp_.cpu = std::move(cpu);
        return *this;
    }
    ComputerBuilder& setRAM(int gb) {
        comp_.ramGB = gb;
        return *this;
    }
    ComputerBuilder& setDisk(int gb) {
        comp_.diskGB = gb;
        return *this;
    }
    ComputerBuilder& setGPU(bool hasGPU) {
        comp_.hasGPU = hasGPU;
        return *this;
    }
    Computer build() {
        return std::move(comp_);
    }
};

// Usage:
// Computer pc = ComputerBuilder().setCPU("Intel i9").setRAM(32).setGPU(true).build();
```

---

## 3. Singleton Pattern (Thread-Safe in C++)

- **Problem**: Need exactly one shared instance (e.g., Configuration Manager, Logger, DB Connection Pool).
- **Modern C++ Solution**: **Meyers' Singleton** (uses C++11 static local variable initialization guarantee — thread-safe with zero mutex overhead!).

```cpp
class DatabaseConnectionPool {
private:
    // 1. Private Constructor
    DatabaseConnectionPool() {
        std::cout << "DB Pool Initialized.\n";
    }

public:
    // 2. Delete Copy and Move constructors & assignments
    DatabaseConnectionPool(const DatabaseConnectionPool&) = delete;
    DatabaseConnectionPool& operator=(const DatabaseConnectionPool&) = delete;
    DatabaseConnectionPool(DatabaseConnectionPool&&) = delete;
    DatabaseConnectionPool& operator=(DatabaseConnectionPool&&) = delete;

    // 3. Thread-Safe Meyers' Singleton
    static DatabaseConnectionPool& getInstance() {
        static DatabaseConnectionPool instance; // Thread-safe in C++11+
        return instance;
    }

    void execute(const std::string& query) {
        // Run SQL query
    }
};
```

> **Interview Trap**: If the interviewer asks: *"How would you do Double-Checked Locking manually?"*
> Mention: Must use `std::atomic<Singleton*>` with memory acquire-release fences to prevent CPU instruction reordering. But always recommend Meyers' singleton first!

---

## 4. Strategy Pattern (Interchangeable Algorithms)

- **Problem**: E-commerce checkout needs to calculate shipping: Standard, Express, International. Using `if/else` or `switch` violates the Open/Closed Principle.
- **Solution**: Encapsulate each algorithm inside a strategy class implementing a common interface.

```cpp
// Strategy Interface
class IShippingStrategy {
public:
    virtual ~IShippingStrategy() = default;
    virtual double calculate(double weightKg) const = 0;
};

class StandardShipping final : public IShippingStrategy {
public:
    double calculate(double weightKg) const override { return weightKg * 5.0; }
};

class ExpressShipping final : public IShippingStrategy {
public:
    double calculate(double weightKg) const override { return weightKg * 15.0 + 20.0; }
};

// Context
class ShippingContext {
private:
    std::unique_ptr<IShippingStrategy> strategy_;
public:
    void setStrategy(std::unique_ptr<IShippingStrategy> s) {
        strategy_ = std::move(s);
    }
    double getCost(double weight) const {
        if (!strategy_) return 0.0;
        return strategy_->calculate(weight);
    }
};
```

---

## 5. Observer Pattern (Pub-Sub Event System)

- **Problem**: A `StockTicker` changes price. 10 different UI components, algorithmic traders, and notification engines must react without tight coupling.
- **Solution**: 1:N Subject-Observer model with thread-safe subscription and dispatch.

```
┌──────────────────────────────────────┐
│           StockTicker (Subject)      │
├──────────────────────────────────────┤
│ - observers_: vector<IObserver*>     │
├──────────────────────────────────────┤
│ + attach(IObserver*)                 │
│ + notify(double newPrice)            │
└──────────────────┬───────────────────┘
                   │ Notifies 1:N
                   ▼
┌──────────────────────────────────────┐
│          IObserver (Interface)       │
├──────────────────────────────────────┤
│ + onPriceUpdate(double price) = 0    │
└──────────────────▲───────────────────┘
         ┌─────────┴─────────┐
         ▼                   ▼
┌──────────────────┐ ┌──────────────────┐
│ MobileAppDisplay │ │ AlgoTradingBot   │
└──────────────────┘ └──────────────────┘
```

```cpp
#include <vector>
#include <algorithm>
#include <mutex>

class IStockObserver {
public:
    virtual ~IStockObserver() = default;
    virtual void onUpdate(const std::string& symbol, double price) = 0;
};

class StockTicker {
private:
    std::string symbol_;
    double price_{0.0};
    std::vector<IStockObserver*> observers_;
    mutable std::mutex mtx_;

public:
    explicit StockTicker(std::string symbol) : symbol_(std::move(symbol)) {}

    void attach(IStockObserver* obs) {
        std::lock_guard<std::mutex> lock(mtx_);
        observers_.push_back(obs);
    }

    void setPrice(double newPrice) {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            price_ = newPrice;
        }
        notifyAll();
    }

private:
    void notifyAll() {
        std::lock_guard<std::mutex> lock(mtx_);
        for (auto* obs : observers_) {
            if (obs) obs->onUpdate(symbol_, price_);
        }
    }
};
```

---

## 6. Decorator Pattern (Dynamic Behavior Extension)

- **Problem**: A coffee shop has `Espresso`, `Latte`, `Tea`. Customers add `Milk`, `Caramel`, `WhippedCream`, `Soy`. Subclassing every combination results in $2^N$ classes (`EspressoWithMilkAndCaramel`).
- **Solution**: Wrap the base component in a decorator that implements the same interface and delegates before/after adding its own behavior.

```
┌──────────────────────────────────────┐
│          IBeverage (Interface)       │
├──────────────────────────────────────┤
│ + getCost(): double                  │
│ + getDescription(): string           │
└──────────────────▲───────────────────┘
         ┌─────────┴─────────┐
         ▼                   ▼
┌──────────────────┐ ┌──────────────────────────────────────┐
│   SimpleCoffee   │ │    BeverageDecorator (Wrapper)       │
├──────────────────┤ ├──────────────────────────────────────┤
│ + cost() = $2.0  │ │ - wrapped_: unique_ptr<IBeverage>    │
└──────────────────┘ └──────────────────▲───────────────────┘
                               ┌────────┴────────┐
                               ▼                 ▼
                      ┌────────────────┐ ┌────────────────┐
                      │  MilkDecorator │ │ CaramelDecorator│
                      │  (+ $0.50)     │ │  (+ $0.75)     │
                      └────────────────┘ └────────────────┘
```

```cpp
class IBeverage {
public:
    virtual ~IBeverage() = default;
    virtual double cost() const = 0;
    virtual std::string description() const = 0;
};

class SimpleCoffee final : public IBeverage {
public:
    double cost() const override { return 2.0; }
    std::string description() const override { return "Simple Coffee"; }
};

// Base Decorator
class BeverageDecorator : public IBeverage {
protected:
    std::unique_ptr<IBeverage> wrapped_;
public:
    explicit BeverageDecorator(std::unique_ptr<IBeverage> b) 
        : wrapped_(std::move(b)) {}
};

class MilkDecorator final : public BeverageDecorator {
public:
    using BeverageDecorator::BeverageDecorator;
    double cost() const override { return wrapped_->cost() + 0.50; }
    std::string description() const override { return wrapped_->description() + ", Milk"; }
};

class CaramelDecorator final : public BeverageDecorator {
public:
    using BeverageDecorator::BeverageDecorator;
    double cost() const override { return wrapped_->cost() + 0.75; }
    std::string description() const override { return wrapped_->description() + ", Caramel"; }
};

// Usage:
// auto cup = std::make_unique<CaramelDecorator>(
//                std::make_unique<MilkDecorator>(
//                    std::make_unique<SimpleCoffee>()));
// cup->cost() -> $3.25
```

---

## 7. Adapter Pattern (Incompatible Interface Bridge)

- **Problem**: Your system requires an `ITargetPayment` interface (`pay(double)`), but you must integrate a third-party `LegacyBankSDK` that requires `makeTransaction(int cents, string currency)`.
- **Solution**: Create an adapter class that implements the target interface and translates calls to the adaptee.

```cpp
// Target Interface Expected by Your System
class IPaymentProcessor {
public:
    virtual ~IPaymentProcessor() = default;
    virtual void processPayment(double dollars) = 0;
};

// Incompatible Third-Party Adaptee
class LegacyBankSDK {
public:
    void makeTransaction(int cents, const std::string& currency) {
        std::cout << "Legacy Bank processed " << cents << " " << currency << "\n";
    }
};

// Adapter
class BankSDKAdapter final : public IPaymentProcessor {
private:
    std::shared_ptr<LegacyBankSDK> adaptee_;
public:
    explicit BankSDKAdapter(std::shared_ptr<LegacyBankSDK> sdk) 
        : adaptee_(std::move(sdk)) {}

    void processPayment(double dollars) override {
        int cents = static_cast<int>(dollars * 100);
        adaptee_->makeTransaction(cents, "USD");
    }
};
```

---

## 8. Command Pattern (Encapsulating Invocations & Undo/Redo)

- **Problem**: Need to execute actions, queue tasks for background execution, log operations, and support **Undo / Redo** (e.g., text editor, bank transfer).
- **Solution**: Encapsulate a request as an object with `execute()` and `undo()` methods.

```cpp
#include <stack>

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

// Receiver
class TextDocument {
    std::string text_;
public:
    void append(const std::string& s) { text_ += s; }
    void erase(size_t count) { 
        if (count <= text_.size()) text_.erase(text_.size() - count); 
    }
    const std::string& getText() const { return text_; }
};

// Concrete Command
class AppendCommand final : public ICommand {
private:
    TextDocument& doc_;
    std::string addedText_;
public:
    AppendCommand(TextDocument& doc, std::string text) 
        : doc_(doc), addedText_(std::move(text)) {}

    void execute() override { doc_.append(addedText_); }
    void undo() override { doc_.erase(addedText_.size()); }
};

// Invoker with Undo Stack
class CommandHistory {
private:
    std::stack<std::unique_ptr<ICommand>> history_;
public:
    void executeCommand(std::unique_ptr<ICommand> cmd) {
        cmd->execute();
        history_.push(std::move(cmd));
    }
    void undoLast() {
        if (!history_.empty()) {
            history_.top()->undo();
            history_.pop();
        }
    }
};
```
