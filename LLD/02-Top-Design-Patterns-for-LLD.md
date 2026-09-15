# 02. The Essential 8 Design Patterns for System Design

> **Interview Mindset**: Out of the 23 Gang of Four (GoF) design patterns, **only 8 patterns appear in 95% of Low-Level Design (LLD) interviews**. 
> An interviewer tests whether you can spot the design smell and immediately select the right pattern using clean, modern C++ (with smart pointers and RAII) rather than writing sprawling `if-else` logic.

---

## 1. Design Patterns Quick-Reference Matrix

```
                        ┌── 1. Singleton (Meyers Singleton & Double-Checked Locking)
         Creational ────┼── 2. Factory / Abstract Factory (Object Creation Decoupling)
                        └── 3. Builder (Complex Object Construction)

                        ┌── 4. Strategy (Swappable Algorithms via std::unique_ptr)
         Behavioral ────┼── 5. Observer (Pub/Sub Event Notification)
                        └── 6. State (Finite State Machine Transitions)

                        ┌── 7. Decorator (Dynamic Behavior Extension)
         Structural ────┴── 8. Adapter (Incompatible Interface Translation)
```

---

## 2. Creational Patterns

### 1. Singleton Pattern
- **Intent**: Ensure a class has only one instance and provide a global point of access to it.
- **Common Uses**: Database connection pool manager, Logger, Configuration manager.

#### The Modern C++ Standard (Meyers' Singleton - Thread-Safe in C++11+):
```cpp
class DatabaseConnection {
public:
    // C++11 guarantees thread-safe initialization of static local variables!
    static DatabaseConnection& getInstance() {
        static DatabaseConnection instance;
        return instance;
    }

    // Delete copy constructor and assignment operator
    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;

    void executeQuery(const std::string& query) {
        // execute SQL query
    }

private:
    DatabaseConnection() = default; // Private constructor
    ~DatabaseConnection() = default;
};
```

#### The Classic Double-Checked Locking (For Interviewers who ask for explicit locks):
```cpp
#include <mutex>
#include <atomic>

class DatabaseConnectionDCL {
private:
    static std::atomic<DatabaseConnectionDCL*> instance;
    static std::mutex mtx;
    DatabaseConnectionDCL() = default;

public:
    static DatabaseConnectionDCL* getInstance() {
        DatabaseConnectionDCL* ptr = instance.load(std::memory_order_acquire);
        if (!ptr) {
            std::lock_guard<std::mutex> lock(mtx);
            ptr = instance.load(std::memory_order_relaxed);
            if (!ptr) {
                ptr = new DatabaseConnectionDCL();
                instance.store(ptr, std::memory_order_release);
            }
        }
        return ptr;
    }
};
```

---

### 2. Factory Method Pattern
- **Intent**: Define an interface for creating an object, but let subclasses decide which class to instantiate.
- **When to Use**: When a system needs to create different variants of an entity without coupling the caller to concrete classes.

```
┌────────────────────────────────────────────────────────┐
│                  Notification Service                  │
└───────────────────────────┬────────────────────────────┘
                            │ Calls createNotification("EMAIL")
                            ▼
┌────────────────────────────────────────────────────────┐
│                  NotificationFactory                   │
└───────────────────────────┬────────────────────────────┘
                            │ Instantiates Concrete Variant
              ┌─────────────┼─────────────┐
              ▼             ▼             ▼
        ┌───────────┐ ┌───────────┐ ┌───────────┐
        │   Email   │ │    SMS    │ │   Push    │
        │Notificat. │ │Notificat. │ │Notificat. │
        └───────────┘ └───────────┘ └───────────┘
```

```cpp
#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>

// Product Interface
class Notification {
public:
    virtual void notifyUser(const std::string& message) = 0;
    virtual ~Notification() = default;
};

// Concrete Products
class EmailNotification : public Notification {
public:
    void notifyUser(const std::string& message) override {
        std::cout << "[EMAIL] " << message << "\n";
    }
};

class SMSNotification : public Notification {
public:
    void notifyUser(const std::string& message) override {
        std::cout << "[SMS] " << message << "\n";
    }
};

// Factory
class NotificationFactory {
public:
    static std::unique_ptr<Notification> createNotification(const std::string& channel) {
        if (channel == "EMAIL") return std::make_unique<EmailNotification>();
        if (channel == "SMS")   return std::make_unique<SMSNotification>();
        throw std::invalid_argument("Unknown notification channel: " + channel);
    }
};
```

---

### 3. Builder Pattern
- **Intent**: Construct complex objects step-by-step.
- **Solves**: The **Telescoping Constructor Anti-Pattern** (constructors with 10 parameters where half are `nullptr` or empty).

```
┌─────────────┐   Step-by-step chained setters    ┌──────────────────────┐    build()    ┌───────────────────────┐
│ Client Code ├──────────────────────────────────►│  HttpRequestBuilder   ├──────────────►│ Immutable HttpRequest │
└─────────────┘                                   └──────────────────────┘               └───────────────────────┘
```

```cpp
#include <string>
#include <unordered_map>
#include <stdexcept>

class HttpRequest {
private:
    std::string url;
    std::string method;
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    // Private constructor: Only Builder can instantiate
    HttpRequest() = default;

public:
    class Builder {
    private:
        HttpRequest request;
    public:
        Builder() {
            request.method = "GET"; // Default method
        }

        Builder& setUrl(const std::string& url) {
            request.url = url;
            return *this;
        }

        Builder& setMethod(const std::string& method) {
            request.method = method;
            return *this;
        }

        Builder& addHeader(const std::string& key, const std::string& value) {
            request.headers[key] = value;
            return *this;
        }

        Builder& setBody(const std::string& body) {
            request.body = body;
            return *this;
        }

        HttpRequest build() {
            if (request.url.empty()) {
                throw std::logic_error("URL is required to build HttpRequest");
            }
            return std::move(request);
        }
    };

    const std::string& getUrl() const { return url; }
};

// Usage in client:
// HttpRequest req = HttpRequest::Builder()
//                      .setUrl("https://api.example.com/data")
//                      .setMethod("POST")
//                      .setBody("{\"key\":\"value\"}")
//                      .build();
```

---

## 3. Behavioral Patterns

### 4. Strategy Pattern
- **Intent**: Define a family of algorithms, encapsulate each one, and make them interchangeable at runtime.
- **Interview Favorite**: Payment methods, Dynamic pricing (Uber Surge vs Normal), Navigation routing (Car vs Walk).

```
┌──────────────────────────────────────────────┐
│                Order Context                 │
└──────────────────────┬───────────────────────┘
                       │ Has-A PaymentStrategy pointer
                       ▼
┌──────────────────────────────────────────────┐
│           << Abstract Interface >>           │
│               PaymentStrategy                │
└──────────────────────┬───────────────────────┘
                       │
          ┌────────────┼────────────┐
          ▼            ▼            ▼
    ┌───────────┐┌───────────┐┌───────────┐
    │CreditCard ││    UPI    ││  PayPal   │
    │ Strategy  ││ Strategy  ││ Strategy  │
    └───────────┘└───────────┘└───────────┘
```

```cpp
#include <iostream>
#include <memory>

class PaymentStrategy {
public:
    virtual void pay(double amount) = 0;
    virtual ~PaymentStrategy() = default;
};

class UPIStrategy : public PaymentStrategy {
public:
    void pay(double amount) override {
        std::cout << "Paid $" << amount << " using UPI.\n";
    }
};

class CreditCardStrategy : public PaymentStrategy {
public:
    void pay(double amount) override {
        std::cout << "Paid $" << amount << " using Credit Card.\n";
    }
};

class PaymentContext {
private:
    std::unique_ptr<PaymentStrategy> strategy;

public:
    void setStrategy(std::unique_ptr<PaymentStrategy> newStrategy) {
        this->strategy = std::move(newStrategy); // Swappable at runtime!
    }

    void executePayment(double amount) {
        if (!strategy) throw std::runtime_error("Payment strategy not set!");
        strategy->pay(amount);
    }
};
```

---

### 5. Observer Pattern
- **Intent**: Define a one-to-many dependency so that when one object changes state, all its dependents are notified automatically.
- **Use Cases**: YouTube subscriber notifications, Stock market ticker feeds, Event buses.

```
┌──────────────────────────────────────────────┐
│            StockMarket (Subject)             │
└──────────────────────┬───────────────────────┘
                       │ notifyObservers() on price change
          ┌────────────┼────────────┐
          ▼            ▼            ▼
    ┌───────────┐┌───────────┐┌───────────┐
    │  Mobile   ││    Web    ││  Trading  │
    │  Display  ││ Dashboard ││    Bot    │
    └───────────┘└───────────┘└───────────┘
```

```cpp
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

// Observer Interface
class Observer {
public:
    virtual void update(double price) = 0;
    virtual ~Observer() = default;
};

// Concrete Observer
class MobileDisplay : public Observer {
public:
    void update(double price) override {
        std::cout << "Mobile Display updated: New Price = $" << price << "\n";
    }
};

// Subject / Observable
class StockMarket {
private:
    std::vector<std::shared_ptr<Observer>> observers;
    double stockPrice{0.0};

public:
    void addObserver(std::shared_ptr<Observer> obs) {
        observers.push_back(obs);
    }

    void setPrice(double newPrice) {
        this->stockPrice = newPrice;
        notifyObservers();
    }

private:
    void notifyObservers() {
        for (const auto& obs : observers) {
            obs->update(stockPrice);
        }
    }
};
```

---

### 6. State Pattern
- **Intent**: Allow an object to alter its behavior when its internal state changes. The object will appear to change its class.
- **Solves**: Massive nested `switch-case` and boolean flag checks (`if (status == ORDER_PLACED) ...`).

```
┌───────────────────┐     Process      ┌───────────────────┐     Deliver      ┌─────────────────────┐
│ OrderPlacedState  ├─────────────────►│ OrderShippedState ├─────────────────►│ OrderDeliveredState │
└───────────────────┘                  └───────────────────┘                  └─────────────────────┘
```

```cpp
#include <iostream>
#include <memory>

class OrderContext; // Forward declaration

class OrderState {
public:
    virtual void next(OrderContext& context) = 0;
    virtual void printStatus() = 0;
    virtual ~OrderState() = default;
};

class OrderContext {
private:
    std::unique_ptr<OrderState> state;

public:
    explicit OrderContext(std::unique_ptr<OrderState> initialState) 
        : state(std::move(initialState)) {}

    void setState(std::unique_ptr<OrderState> newState) {
        state = std::move(newState);
    }

    void nextState() {
        state->next(*this);
    }

    void showStatus() {
        state->printStatus();
    }
};

class DeliveredState : public OrderState {
public:
    void next(OrderContext& context) override {
        std::cout << "Order already delivered. Final state.\n";
    }
    void printStatus() override {
        std::cout << "Package delivered to customer.\n";
    }
};

class PlacedState : public OrderState {
public:
    void next(OrderContext& context) override {
        context.setState(std::make_unique<DeliveredState>());
    }
    void printStatus() override {
        std::cout << "Order placed. Ready for shipping.\n";
    }
};
```

---

## 4. Structural Patterns

### 7. Decorator Pattern
- **Intent**: Attach additional responsibilities to an object dynamically without modifying the underlying class or causing class explosion.
- **Use Cases**: Coffee shop beverage customization (Milk, Sugar, Whip), Pizza toppings, Java/C++ Stream wrappers.

```
┌──────────────────────────────────────────────┐
│            Plain Espresso: $3.00             │
└──────────────────────┬───────────────────────┘
                       │ Wraps with MilkDecorator (+$0.50)
                       ▼
┌──────────────────────────────────────────────┐
│           Espresso + Milk: $3.50             │
└──────────────────────┬───────────────────────┘
                       │ Wraps with CaramelDecorator (+$0.75)
                       ▼
┌──────────────────────────────────────────────┐
│             Total Coffee: $4.25              │
└──────────────────────────────────────────────┘
```

```cpp
#include <iostream>
#include <memory>
#include <string>

// Component Interface
class Coffee {
public:
    virtual double getCost() const = 0;
    virtual std::string getDescription() const = 0;
    virtual ~Coffee() = default;
};

// Concrete Component
class SimpleCoffee : public Coffee {
public:
    double getCost() const override { return 3.00; }
    std::string getDescription() const override { return "Simple Coffee"; }
};

// Base Decorator
class CoffeeDecorator : public Coffee {
protected:
    std::unique_ptr<Coffee> wrappedCoffee;
public:
    explicit CoffeeDecorator(std::unique_ptr<Coffee> coffee) 
        : wrappedCoffee(std::move(coffee)) {}
};

// Concrete Decorator
class MilkDecorator : public CoffeeDecorator {
public:
    explicit MilkDecorator(std::unique_ptr<Coffee> coffee) 
        : CoffeeDecorator(std::move(coffee)) {}

    double getCost() const override {
        return wrappedCoffee->getCost() + 0.50;
    }
    std::string getDescription() const override {
        return wrappedCoffee->getDescription() + ", Milk";
    }
};

// Client usage:
// auto coffee = std::make_unique<MilkDecorator>(std::make_unique<SimpleCoffee>());
// std::cout << coffee->getDescription() << " : $" << coffee->getCost();
```

---

### 8. Adapter Pattern
- **Intent**: Convert the interface of a class into another interface clients expect. Lets incompatible classes work together.
- **Use Cases**: Integrating a 3rd-party Payment Gateway (e.g. Stripe SDK) into your internal `PaymentProcessor` interface.

```
┌─────────────────────┐   Calls pay(10.50)    ┌─────────────────────┐   makeCharge(1050)    ┌─────────────────────┐
│   Internal System   ├──────────────────────►│    StripeAdapter    ├──────────────────────►│ 3rd-Party StripeSDK  │
└─────────────────────┘                       └─────────────────────┘                       └─────────────────────┘
```

```cpp
#include <iostream>
#include <memory>

// Expected Target Interface
class PaymentProcessor {
public:
    virtual void pay(double dollars) = 0;
    virtual ~PaymentProcessor() = default;
};

// 3rd Party Incompatible SDK (Adaptee)
class StripeSDK {
public:
    void makeCharge(long cents) {
        std::cout << "Charged " << cents << " cents via Stripe API.\n";
    }
};

// Adapter
class StripeAdapter : public PaymentProcessor {
private:
    std::shared_ptr<StripeSDK> stripe;

public:
    explicit StripeAdapter(std::shared_ptr<StripeSDK> stripeSDK) : stripe(std::move(stripeSDK)) {}

    void pay(double dollars) override {
        long cents = static_cast<long>(dollars * 100);
        stripe->makeCharge(cents); // Translates dollars to cents and calls Stripe API
    }
};
```

---

## 5. Pattern Comparison Matrix (Interview Traps)

| Pattern A | Pattern B | How to Distinguish in an Interview |
| :--- | :--- | :--- |
| **Strategy** | **State** | In **Strategy**, the client explicitly chooses and injects the algorithm. In **State**, transitions happen automatically from within the states themselves based on business rules. |
| **Factory** | **Builder** | Use **Factory** when object creation is a single-step decision based on a type. Use **Builder** when an object has many optional attributes and requires step-by-step assembly. |
| **Decorator** | **Adapter** | **Decorator** enhances an object's behavior without changing its interface. **Adapter** changes an incompatible interface to match the client's expectations. |
| **Observer** | **Pub/Sub** | In pure **Observer**, the Subject directly holds pointers to observers. In **Pub/Sub**, publishers and subscribers never know each other; they communicate through an external Event Channel / Broker. |
