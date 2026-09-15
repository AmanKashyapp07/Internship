# 04. Connecting OOP to System Design

> **The Big Picture**: An interview does not stop at writing a C++ class. The senior interviewer will ask:
> *"How does this `Order` class get saved to PostgreSQL?"*
> *"How does this method handle 10,000 requests per second across 5 servers?"*
> *"Where does Redis fit into this object model?"*
> This guide bridges **Low-Level Object Design (LLD)** to **High-Level Distributed Systems (HLD)**.

---

## 1. The Full-Stack Architecture Mapping

```
HIGH-LEVEL DISTRIBUTED TIER                     LOW-LEVEL OBJECT-ORIENTED (C++) TIER
┌─────────────────────────────┐
│  Client (Browser / Mobile)  │
└──────────────┬──────────────┘
               │ HTTP POST /api/v1/orders { DTO payload }
               ▼
┌─────────────────────────────┐                 ┌─────────────────────────────────┐
│     Global Load Balancer    │                 │ Controller Layer                │
└──────────────┬──────────────┘                 │ class OrderController {         │
               │ Routes to any stateless server │   Response postOrder(OrderDTO); │
               ▼                                │ };                              │
┌─────────────────────────────┐                 └────────────────┬────────────────┘
│   Stateless App Server      │                                  │ Invokes
│      (C++ / Go / Java)      │                                  ▼
├─────────────────────────────┤                 ┌─────────────────────────────────┐
│       Service Layer         │                 │ Domain Service Layer            │
│  (Orchestrates transaction) │                 │ class OrderService {            │
└──────┬───────────────┬──────┘                 │   void placeOrder(...);         │
       │               │                        │ };                              │
       │               │                        └──────┬───────────────────┬──────┘
       │ 1. Read Cache │ 2. On Miss:                   │                   │
       ▼               ▼    Query DB                   │ 3. Dispatches     │ 4. Publishes
┌──────────────┐ ┌──────────────┐                      ▼                    ▼
│ Redis Cache  │ │  PostgreSQL  │             ┌─────────────────┐  ┌──────────────────┐
│ (In-Memory)  │ │ (Disk Store) │             │ Domain Entities │  │ Domain Events    │
└──────────────┘ └──────────────┘             │ class Order;    │  │ OrderPlacedEvent │
                                              │ class Item;     │  │ (To Kafka Queue) │
                                              └─────────────────┘  └──────────────────┘
```

---

## 2. Layered Architecture: From DTO to Domain Entity

In clean production code, never expose your internal domain entity directly to the network. Use a 4-layer separation:

```
┌────────────────────────────────────────────────────────┐
│ 1. Presentation Layer (HTTP Controllers, REST DTOs)   │
└───────────────────────────┬────────────────────────────┘
                            │ Validates JSON & maps to Domain
                            ▼
┌────────────────────────────────────────────────────────┐
│ 2. Application Service Layer (Transaction Boundaries)  │
└───────────────────────────┬────────────────────────────┘
                            │ Executes business workflows
                            ▼
┌────────────────────────────────────────────────────────┐
│ 3. Domain Layer (Pure Entities & Business Rules)       │
└───────────────────────────┬────────────────────────────┘
                            │ Persists through interfaces
                            ▼
┌────────────────────────────────────────────────────────┐
│ 4. Infrastructure Layer (DB Repositories, Redis, Kafka)│
└────────────────────────────────────────────────────────┘
```

### Modern C++ Implementation:

```cpp
#include <iostream>
#include <memory>
#include <string>

// 1. DTO (Data Transfer Object for API request/response)
struct CreateOrderRequestDTO {
    std::string userId;
    std::string productId;
    int quantity;
};

// 2. Pure Domain Entity (Contains business logic & invariants)
class Order {
private:
    std::string orderId_;
    std::string userId_;
    double totalAmount_{0.0};
    bool isPaid_{false};

public:
    Order(std::string orderId, std::string userId, double amount)
        : orderId_(std::move(orderId)), userId_(std::move(userId)), totalAmount_(amount) {}

    void markPaid() {
        if (isPaid_) throw std::runtime_error("Order already paid!");
        isPaid_ = true;
    }

    [[nodiscard]] const std::string& getId() const { return orderId_; }
    [[nodiscard]] double getAmount() const { return totalAmount_; }
    [[nodiscard]] bool isPaid() const { return isPaid_; }
};
```

---

## 3. Database Integration: The Repository Pattern

The **Repository Pattern** decouples domain logic from SQL syntax, ORMs, or concrete database drivers.

```cpp
// Abstract Repository Interface (Defined in Domain Layer)
class IOrderRepository {
public:
    virtual ~IOrderRepository() = default;
    virtual void save(const Order& order) = 0;
    virtual std::unique_ptr<Order> findById(const std::string& orderId) = 0;
};

// Concrete Implementation (Infrastructure Layer - PostgreSQL)
class PostgresOrderRepository final : public IOrderRepository {
public:
    void save(const Order& order) override {
        // Run SQL: "INSERT INTO orders (id, amount, is_paid) VALUES (...)"
        std::cout << "[PostgreSQL]: Saved order " << order.getId() << " to disk.\n";
    }

    std::unique_ptr<Order> findById(const std::string& orderId) override {
        // Run SQL: "SELECT * FROM orders WHERE id = ..."
        std::cout << "[PostgreSQL]: Querying order " << orderId << " from disk.\n";
        return std::make_unique<Order>(orderId, "user_123", 99.50);
    }
};
```

---

## 4. Cache Integration: The Decorator Pattern for Cache-Aside

How do you add Redis caching without modifying your business service? **Use the Decorator Pattern to wrap the Repository!**

```
┌────────────────────────────────────────────────────────┐
│                   OrderService                         │
└───────────────────────────┬────────────────────────────┘
                            │ Calls findById()
                            ▼
┌────────────────────────────────────────────────────────┐
│           CachedOrderRepository (Decorator)            │
│  1. Check Redis RAM                                    │
│     ├── Hit: Return cached object immediately (<1ms)   │
│     └── Miss: Delegate to PostgresOrderRepository      │
└───────────────────────────┬────────────────────────────┘
                            │ On Cache Miss
                            ▼
┌────────────────────────────────────────────────────────┐
│             PostgresOrderRepository (Disk)             │
└────────────────────────────────────────────────────────┘
```

```cpp
class CachedOrderRepository final : public IOrderRepository {
private:
    std::shared_ptr<IOrderRepository> dbRepo_; // Wrapped DB Repository

public:
    explicit CachedOrderRepository(std::shared_ptr<IOrderRepository> dbRepo)
        : dbRepo_(std::move(dbRepo)) {}

    void save(const Order& order) override {
        dbRepo_->save(order);
        // Cache Invalidation / Update in Redis:
        std::cout << "[Redis]: Invalidated cache key order:" << order.getId() << "\n";
    }

    std::unique_ptr<Order> findById(const std::string& orderId) override {
        // 1. Check Redis Cache
        std::cout << "[Redis]: Checking cache for order:" << orderId << "... ";
        bool cacheHit = false; // Simulated miss
        
        if (cacheHit) {
            std::cout << "HIT!\n";
            return std::make_unique<Order>(orderId, "cached_user", 99.50);
        }

        std::cout << "MISS!\n";
        // 2. Fetch from Database
        auto order = dbRepo_->findById(orderId);
        
        // 3. Populate Redis Cache
        if (order) {
            std::cout << "[Redis]: Populated cache key order:" << orderId << " (TTL 3600s)\n";
        }
        return order;
    }
};
```

---

## 5. Message Queues & Asynchronous Decoupling

When placing an order:
1. Deduct inventory & charge payment (Synchronous - Critical).
2. Send push notification & email invoice (Asynchronous - Non-Critical).

Do **not** block the client while sending emails! Publish a **Domain Event** to a message queue (Kafka / RabbitMQ):

```cpp
// Domain Event
struct OrderPlacedEvent {
    std::string orderId;
    std::string userEmail;
    double amount;
};

// Event Publisher Interface
class IEventPublisher {
public:
    virtual ~IEventPublisher() = default;
    virtual void publish(const std::string& topic, const OrderPlacedEvent& event) = 0;
};

class KafkaEventPublisher final : public IEventPublisher {
public:
    void publish(const std::string& topic, const OrderPlacedEvent& event) override {
        // Serializes event to JSON/Protobuf and appends to Kafka topic
        std::cout << "[Kafka]: Emitted OrderPlacedEvent for " << event.orderId 
                  << " to topic '" << topic << "'\n";
    }
};
```

---

## 6. The Complete Service Layer Orchestrating Everything

```cpp
class OrderService {
private:
    std::shared_ptr<IOrderRepository> orderRepo_;
    std::shared_ptr<IEventPublisher> eventPublisher_;

public:
    OrderService(std::shared_ptr<IOrderRepository> repo,
                 std::shared_ptr<IEventPublisher> publisher)
        : orderRepo_(std::move(repo)), eventPublisher_(std::move(publisher)) {}

    void placeOrder(const CreateOrderRequestDTO& dto) {
        // 1. Create domain entity & enforce invariants
        std::string orderId = "ord_999";
        Order order(orderId, dto.userId, 150.0);
        order.markPaid();

        // 2. Persist through repository (Cache-Aside + PostgreSQL)
        orderRepo_->save(order);

        // 3. Emit asynchronous event to Kafka for workers
        OrderPlacedEvent event{order.getId(), "user@example.com", order.getAmount()};
        eventPublisher_->publish("orders.placed", event);
        
        std::cout << "Order " << orderId << " successfully completed!\n";
    }
};
```

---

## 7. Stateless Services & Horizontal Scaling

Why do we design domain services like `OrderService` without member state?

```
STATEFUL (Anti-Pattern - Breaks Load Balancer):
class StatefulOrderService {
    Order* currentOrder_{nullptr}; // 💥 Bound to single server RAM!
};
If request 1 goes to Server A, and request 2 goes to Server B, Server B has no currentOrder_!

STATELESS (Production Standard - Trivially Scales):
class OrderService {
    // Member variables are ONLY stateless dependencies:
    std::shared_ptr<IOrderRepository> repo_;
    std::shared_ptr<IEventPublisher> publisher_;
};
Every method receives all necessary state through parameters (DTO / tokens).
Any server instance behind the Load Balancer can serve any user at any millisecond!
```

---

## 8. Summary Table: Concept Translation Matrix

| Low-Level OOP Concept (LLD) | High-Level System Design Equivalent (HLD) |
| :--- | :--- |
| **Object Member Fields** | Relational Database Columns / Redis Key Attributes |
| **Class Member Methods** | REST / gRPC API Endpoints (`POST`, `PUT`, `GET`) |
| **Method Exceptions** | HTTP Status Codes (`400 Bad Request`, `404 Not Found`) |
| **Decorator Pattern** | Cache-Aside Proxy (Redis) / API Gateway Middleware |
| **Observer Pattern** | Distributed Pub-Sub (Apache Kafka / RabbitMQ) |
| **Strategy Pattern** | Feature Flags / Dynamic Routing / Load Balancer Algorithms |
| **Stateless Classes** | Horizontally Scalable Web Server Instances |
