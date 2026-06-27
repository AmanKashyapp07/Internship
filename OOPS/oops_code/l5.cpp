/**
 * @file l5_solid.cpp
 * @brief Lecture 5: SOLID Principles (SRP, OCP, LSP, ISP, DIP with violations and clean designs)
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <cassert>

using namespace std;

// Standard types for our examples
struct Item { string name; double price; };
struct Order { vector<Item> items; double totalAmount; string customerEmail; };

// ==========================================
// 1. Single Responsibility Principle (SRP)
// ==========================================

// --- VIOLATION ---
// OrderManager handles validation, DB persistence, and email notifications.
// It has 3 reasons to change (validator logic, db changes, notification rules).
class BadOrderManager {
public:
    void placeOrder(Order& o) {
        if (o.items.empty()) throw runtime_error("Order is empty");
        cout << "Saving order to Database...\n";
        cout << "Sending order confirmation email to " << o.customerEmail << "\n";
    }
};

// --- RESOLUTION ---
// Split into separate classes, each with a single responsibility.
class OrderValidator {
public:
    void validate(const Order& o) const {
        if (o.items.empty()) throw runtime_error("Order is empty");
        if (o.totalAmount < 0) throw runtime_error("Invalid amount");
    }
};

class OrderRepository {
public:
    void save(const Order& o) const {
        cout << "OrderRepository: Saving order to database...\n";
    }
};

class EmailNotificationService {
public:
    void sendConfirmation(const Order& o) const {
        cout << "EmailNotificationService: Sending confirmation to " << o.customerEmail << "\n";
    }
};

// Orchestrator: delegates responsibilities
class GoodOrderService {
private:
    OrderValidator validator;
    OrderRepository repo;
    EmailNotificationService notifier;

public:
    void placeOrder(Order& o) {
        validator.validate(o);
        repo.save(o);
        notifier.sendConfirmation(o);
    }
};

// ==========================================
// 2. Open/Closed Principle (OCP)
// ==========================================

struct Payment { double amount; };

// --- VIOLATION ---
// Every new payment type requires modifying BadPaymentProcessor (violates OCP).
class BadPaymentProcessor {
public:
    void process(const Payment& p, const string& type) {
        if (type == "credit_card") {
            cout << "Processing CC payment of $" << p.amount << "\n";
        } else if (type == "paypal") {
            cout << "Processing PayPal payment of $" << p.amount << "\n";
        }
        // If we want to add "crypto", we MUST modify this class!
    }
};

// --- RESOLUTION ---
// Close processor for modification, open it for extension by introducing an abstraction.
class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    virtual void process(const Payment& p) const = 0;
};

class CreditCardPayment : public PaymentStrategy {
public:
    void process(const Payment& p) const override {
        cout << "Processing CC payment of $" << p.amount << "\n";
    }
};

class PayPalPayment : public PaymentStrategy {
public:
    void process(const Payment& p) const override {
        cout << "Processing PayPal payment of $" << p.amount << "\n";
    }
};

class CryptoPayment : public PaymentStrategy { // added cleanly without modifying existing code
public:
    void process(const Payment& p) const override {
        cout << "Processing Crypto payment of $" << p.amount << "\n";
    }
};

class GoodPaymentProcessor {
private:
    const PaymentStrategy& strategy;

public:
    GoodPaymentProcessor(const PaymentStrategy& s) : strategy(s) {}
    void process(const Payment& p) const {
        strategy.process(p);
    }
};

// ==========================================
// 3. Liskov Substitution Principle (LSP)
// ==========================================

// --- VIOLATION ---
// Square inherits from Rectangle, but overriding setWidth/setHeight to keep them equal
// violates the behavioral expectations (postconditions) of the base class contract.
class BadRectangle {
protected:
    int width, height;

public:
    BadRectangle(int w, int h) : width(w), height(h) {}
    virtual void setWidth(int w)  { width = w; }
    virtual void setHeight(int h) { height = h; }
    int getArea() const { return width * height; }
};

class BadSquare : public BadRectangle {
public:
    BadSquare(int s) : BadRectangle(s, s) {}
    void setWidth(int w) override { width = height = w; } // Violates LSP behavior contract
    void setHeight(int h) override { width = height = h; }
};

void testLSPRectangle(BadRectangle& r) {
    r.setWidth(5);
    r.setHeight(10);
    // For any rectangle, setWidth(5) and setHeight(10) must yield an area of 50.
    // However, if we pass a BadSquare, the assertion fails (Area becomes 100).
    cout << "Expected area: 50. Actual area: " << r.getArea() << "\n";
}

// --- RESOLUTION ---
// Separate the classes. Square is NOT behaviorally substitutable for Rectangle.
// Make them both inherit from a common Shape abstraction.
class Shape2D {
public:
    virtual ~Shape2D() = default;
    virtual int getArea() const = 0;
};

class GoodRectangle : public Shape2D {
private:
    int width, height;

public:
    GoodRectangle(int w, int h) : width(w), height(h) {}
    void setWidth(int w)  { width = w; }
    void setHeight(int h) { height = h; }
    int getArea() const override { return width * height; }
};

class GoodSquare : public Shape2D {
private:
    int side;

public:
    GoodSquare(int s) : side(s) {}
    void setSide(int s) { side = s; }
    int getArea() const override { return side * side; }
};

// ==========================================
// 4. Interface Segregation Principle (ISP)
// ==========================================

// --- VIOLATION ---
// A "fat" interface containing methods that not all clients need or use.
class IWorker {
public:
    virtual ~IWorker() = default;
    virtual void work() = 0;
    virtual void eat() = 0;
    virtual void sleep() = 0;
};

class HumanWorker : public IWorker {
public:
    void work() override { cout << "Human working...\n"; }
    void eat() override { cout << "Human eating...\n"; }
    void sleep() override { cout << "Human sleeping...\n"; }
};

class RobotWorker : public IWorker {
public:
    void work() override { cout << "Robot working...\n"; }
    void eat() override { /* Violation: forced to implement eat! */ }
    void sleep() override { /* Violation: forced to implement sleep! */ }
};

// --- RESOLUTION ---
// Segregate the fat interface into smaller, focused interfaces.
class IWorkable {
public:
    virtual ~IWorkable() = default;
    virtual void work() = 0;
};

class IEatable {
public:
    virtual ~IEatable() = default;
    virtual void eat() = 0;
};

// Robot now only depends on what it actually needs / does.
class GoodRobot : public IWorkable {
public:
    void work() override { cout << "GoodRobot working efficiently...\n"; }
};

class GoodHuman : public IWorkable, public IEatable {
public:
    void work() override { cout << "GoodHuman working...\n"; }
    void eat() override { cout << "GoodHuman eating...\n"; }
};

// ==========================================
// 5. Dependency Inversion Principle (DIP)
// ==========================================

// --- VIOLATION ---
// High-level service directly instantiates and depends on a low-level MySQL database.
class MySQLDatabase {
public:
    void save(string data) { cout << "Saved data to MySQL Database\n"; }
};

class BadOrderService {
private:
    MySQLDatabase db; // Tight coupling! Cannot easily switch database types or mock for testing.

public:
    void process(string data) {
        db.save(data);
    }
};

// --- RESOLUTION ---
// High-level and low-level modules should both depend on an interface abstraction.
class IDatabase {
public:
    virtual ~IDatabase() = default;
    virtual void save(string data) = 0;
};

class MySQLDatabaseGood : public IDatabase {
public:
    void save(string data) override { cout << "MySQLDatabaseGood: Saved '" << data << "'\n"; }
};

class PostgreSQLDatabaseGood : public IDatabase {
public:
    void save(string data) override { cout << "PostgreSQLDatabaseGood: Saved '" << data << "'\n"; }
};

// High-level Service depends on the IDatabase abstraction.
// We support 3 forms of Dependency Injection:
class GoodService {
private:
    IDatabase* db; // Pointer to allow setters/methods

public:
    // 1. Constructor Injection (Preferred)
    GoodService(IDatabase* database) : db(database) {}

    // 2. Setter Injection
    void setDatabase(IDatabase* database) {
        db = database;
    }

    // 3. Method Injection
    void processAndSave(string data, IDatabase& temporaryDb) {
        temporaryDb.save(data);
    }

    void process(string data) {
        if (db) db->save(data);
    }
};

int main() {
    cout << "=== 1. SRP Example ===\n";
    Order myOrder;
    myOrder.items = { {"Laptop", 1000.0} };
    myOrder.totalAmount = 1000.0;
    myOrder.customerEmail = "alice@example.com";
    
    GoodOrderService orderService;
    orderService.placeOrder(myOrder);

    cout << "\n=== 2. OCP Example ===\n";
    Payment p{250.0};
    CryptoPayment crypto;
    GoodPaymentProcessor processor(crypto); // Injecting CryptoPayment strategy
    processor.process(p);

    cout << "\n=== 3. LSP Violation Demo ===\n";
    BadSquare sq(5);
    testLSPRectangle(sq); // Fails assertion under normal testing, but we just print area here.

    cout << "\n=== 4. ISP Example ===\n";
    GoodRobot robot;
    robot.work();

    cout << "\n=== 5. DIP & Dependency Injection ===\n";
    MySQLDatabaseGood mysqlDb;
    PostgreSQLDatabaseGood pgDb;

    // Constructor injection
    GoodService svc(&mysqlDb);
    svc.process("Order #123");

    // Setter injection
    svc.setDatabase(&pgDb);
    svc.process("Order #123");

    return 0;
}
