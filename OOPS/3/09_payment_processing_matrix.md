# LLD: Payment Processing Matrix (Payment Methods × Gateways)

> **Core Category:** Polymorphic Operations & Multi-Axis Variations  
> **Key Design Pattern:** Bridge Pattern (Payment Methods × Payment Gateways) + Strategy & Factory Pattern  
> **Target Concepts:** Decoupling Client Abstractions from Platform Implementors, Preventing $M \times N$ Class Explosion, Open-Closed Principle (OCP)

---

## 1. The Interview Question

> *"You are designing a global payment processing engine for an e-commerce platform. The system supports multiple payment methods (`CreditCard`, `UPI`, `NetBanking`, `Crypto`) and must process them across multiple payment gateways (`Stripe`, `Razorpay`, `PayPal`, `Adyen`). Each gateway has its own proprietary API protocol, tokenization format, and settlement mechanism. How do you design this architecture so adding a new payment method or gateway does not cause a combinatorial $M \times N$ class explosion?"*

---

## 2. The Naive Diagrams (What NOT to Draw on Paper)

### ❌ Anti-Pattern 1: Cartesian Product Subclassing ($M \times N$ Classes)

```
                                  +-----------------------+
                                  |     PaymentMethod     |
                                  +-----------------------+
                                     /        |        \
                                    /         |         \
                    +----------------+ +---------------+ +----------------+
                    |   CreditCard   | |   UPIPayment  | |   NetBanking   |
                    +----------------+ +---------------+ +----------------+
                       /    |    \        /    |    \       /    |    \
                      /     |     \      /     |     \     /     |     \
                 +-------++-------++-------+ +-------++-------++-------+ +-------+
                 |Stripe ||Razor- ||PayPal | |Stripe ||Razor- ||PayPal | |Stripe | ...
                 |Card   ||payCard||Card   | |UPI    ||payUPI ||UPI    | |NetBank|
                 +-------++-------++-------+ +-------++-------++-------+ +-------+
```

#### Why the interviewer rejects this:
1. **$M \times N$ Combinatorial Explosion:** If you have 4 payment methods and 4 gateways, you must write and maintain **16 separate classes**! Adding 1 new gateway requires writing 4 new classes.
2. **Duplicated Gateway Logic:** Stripe's authentication, network retry, and webhook signature verification logic would be copy-pasted across `StripeCard`, `StripeUPI`, and `StripeNetBanking`.
3. **Rigid Routing:** You cannot dynamically route a transaction from Stripe to Razorpay if Stripe experiences a sudden outage.

---

### ❌ Anti-Pattern 2: The Monolithic Switch-Case Engine

```
+-------------------------------------------------------------------------+
|                              PaymentEngine                              |
+-------------------------------------------------------------------------+
| + process(method: string, gateway: string, payload: json): void         |
|   -> Nested 100-line switch(method) { case Card: switch(gateway) { ... }}
+-------------------------------------------------------------------------+
```

---

## 3. The Winning Whiteboard Architecture

### ✅ Core Insight: *"Apply the GoF Bridge Pattern. Decouple the High-Level Abstraction (`PaymentMethod` - user input validation & tokenization) from the Low-Level Implementor (`IPaymentGateway` - raw API transport & settlement)."*

```
+-----------------------------------------------------------------------------------+
|                        <<abstraction>> PaymentMethod                              |
+-----------------------------------------------------------------------------------+
| # gateway: shared_ptr<IPaymentGateway>                                            |
+-----------------------------------------------------------------------------------+
| + processPayment(amount: double, currency: string)*: PaymentResult                |
| + setGateway(gateway: shared_ptr<IPaymentGateway>): void  <-- Dynamic Failover!    |
+-----------------------------------------------------------------------------------+
           ^                                                      |
           | extends                                              | HAS-A (Bridge Link)
           |                                                      v
+-----------------------+ +-----------------------+    +----------------------------+
|   CreditCardPayment   | |      UPIPayment       |    |       <<interface>>        |
+-----------------------+ +-----------------------+    |      IPaymentGateway       |
| - cardNumber: string  | | - upiId: string       |    +----------------------------+
| - cvv: string         | +-----------------------+    | + charge(req)*: GatewayRes |
| - expiry: string      | | + processPayment() {  |    | + refund(txId, amt)*: bool |
+-----------------------+ |     val UPI;          |    | + getGatewayName()*: string|
| + processPayment() {  | |     gw->charge(UPIReq)|    +----------------------------+
|     validateCard();   | |   }                   |                  ^
|     gw->charge(Req);  | +-----------------------+                  | implements
|   }                   |                              +-------------+-------------+
+-----------------------+                              |                           |
                                              +-----------------+         +-----------------+
                                              |  StripeGateway  |         | RazorpayGateway |
                                              +-----------------+         +-----------------+
                                              | - apiKey: string|         | - keySecret: str|
                                              +-----------------+         +-----------------+
                                              | + charge(req)   |         | + charge(req)   |
                                              |   (Stripe API)  |         |   (Razorpay API)|
                                              +-----------------+         +-----------------+
```

---

## 4. Concise Whiteboard Code (What to Write on Paper)

```cpp
// 1. Common Request & Result Data Objects
struct PaymentRequest {
    double amount;
    std::string currency;
    std::unordered_map<std::string, std::string> metadata;
};

struct PaymentResult {
    bool isSuccess;
    std::string transactionId;
    std::string errorMessage;
};

// 2. Implementor Interface: Payment Gateway (Bridge Dimension B)
class IPaymentGateway {
public:
    virtual ~IPaymentGateway() = default;
    virtual PaymentResult charge(const PaymentRequest& req) = 0;
    virtual std::string getGatewayName() const = 0;
};

// Concrete Gateways
class StripeGateway : public IPaymentGateway {
public:
    PaymentResult charge(const PaymentRequest& req) override {
        // Execute Stripe SDK API call (POST https://api.stripe.com/v1/charges)
        return {true, "ch_stripe_9941", ""};
    }
    std::string getGatewayName() const override { return "Stripe"; }
};

class RazorpayGateway : public IPaymentGateway {
public:
    PaymentResult charge(const PaymentRequest& req) override {
        // Execute Razorpay API call
        return {true, "pay_rzp_8812", ""};
    }
    std::string getGatewayName() const override { return "Razorpay"; }
};

// 3. Abstraction Interface: Payment Method (Bridge Dimension A)
class PaymentMethod {
protected:
    std::shared_ptr<IPaymentGateway> gateway;
public:
    explicit PaymentMethod(std::shared_ptr<IPaymentGateway> gw) : gateway(std::move(gw)) {}
    virtual ~PaymentMethod() = default;
    
    // Dynamic Gateway Failover
    void setGateway(std::shared_ptr<IPaymentGateway> gw) { gateway = std::move(gw); }
    virtual PaymentResult pay(double amount, const std::string& currency) = 0;
};

// Concrete Payment Methods
class CreditCardPayment : public PaymentMethod {
    std::string cardNumber, cvv, expiry;
public:
    CreditCardPayment(std::string num, std::string cvv, std::string exp, std::shared_ptr<IPaymentGateway> gw)
        : PaymentMethod(std::move(gw)), cardNumber(std::move(num)), cvv(std::move(cvv)), expiry(std::move(exp)) {}

    PaymentResult pay(double amount, const std::string& currency) override {
        // Step 1: Client-side Luhn algorithm validation
        // Step 2: Delegate transport to bridge gateway
        PaymentRequest req{amount, currency, {{"type", "card"}, {"card_last4", cardNumber.substr(12)}}};
        return gateway->charge(req);
    }
};

class UPIPayment : public PaymentMethod {
    std::string vpa; // e.g. "user@okhdfcbank"
public:
    UPIPayment(std::string vpa, std::shared_ptr<IPaymentGateway> gw)
        : PaymentMethod(std::move(gw)), vpa(std::move(vpa)) {}

    PaymentResult pay(double amount, const std::string& currency) override {
        PaymentRequest req{amount, currency, {{"type", "upi"}, {"vpa", vpa}}};
        return gateway->charge(req);
    }
};
```

---

## 5. The 3-Minute Verbal Pitch (What to Say to the Interviewer)

> 1. *"If we model payment methods and gateways with naive inheritance (e.g. `StripeCreditCard`, `RazorpayUPI`), we suffer an **$M \times N$ class explosion** ($4 \text{ methods} \times 4 \text{ gateways} = 16 \text{ classes}$). Adding a single new gateway forces us to write $M$ new classes."*
> 2. *"To solve this, I apply the **GoF Bridge Pattern**, which splits the system into two independent axes of variation: **Abstraction (`PaymentMethod`)** and **Implementor (`IPaymentGateway`)**."*
> 3. *"`PaymentMethod` (CreditCard, UPI) handles user-side input validation, sanitization, and packaging. `IPaymentGateway` (Stripe, Razorpay, PayPal) handles the low-level vendor protocol, SSL connection, and bank settlement."*
> 4. *"This gives us $O(M + N)$ scalability: Adding 1 gateway requires writing only 1 class (`PayPalGateway`)."*
> 5. *"Furthermore, this enables **Dynamic Runtime Failover**: If Stripe returns a 503 error, the system calls `paymentMethod->setGateway(razorpayGateway)` and retries the charge seamlessly without prompting the user to re-enter their card details."*

---

## 6. Common Interviewer Follow-Ups & Quick Answers

| Follow-up Question | Recommended Response |
| :--- | :--- |
| **"How do you automatically select the cheapest or highest-uptime gateway?"** | Implement a `SmartRoutingStrategy` (Strategy Pattern) that queries historical success rates and interchange fees before injecting the gateway. |
| **"How do you handle asynchronous Webhooks (e.g. UPI payment confirmation)?"** | Use the **Observer Pattern / Event Bus**: When the gateway posts an async webhook, publish a `PaymentCapturedEvent` to update the Order Status. |
| **"How do you prevent PCI-DSS compliance scope on your servers?"** | Pass opaque gateway tokens (e.g. `tok_123` generated via client-side Stripe.js / Razorpay SDK) into `PaymentMethod` rather than raw CVV/Card numbers. |
