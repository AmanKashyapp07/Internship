# LLD: E-Commerce Products (Physical, Digital, Subscription)

> **Core Category:** Role & Behavior Combinations / Multi-Dimensional Variations  
> **Key Design Pattern:** Bridge Pattern (Fulfillment Strategy) + Decorator/Strategy (Pricing & Billing Model)  
> **Target Concepts:** Orthogonal Dimension Decoupling, Open-Closed Principle (OCP), Avoiding Class Explosion ($M \times N$ Subclasses)

---

## 1. The Interview Question

> *"In an e-commerce platform, a product can be a physical item (requires physical shipping address, weight calculation, warehouse allocation) or digital (generates secure download links/license keys, zero shipping). A product can also be sold as a one-time purchase or as a recurring subscription (e.g. monthly magazine, quarterly subscription box, SaaS software). How do you model `Product` attributes and behaviors without creating bloated, combinatorial subclasses like `PhysicalSubscriptionBook`?"*

---

## 2. The Naive Diagrams (What NOT to Draw on Paper)

### ❌ Anti-Pattern 1: The Combinatorial Inheritance Explosion ($M \times N$ Subclasses)

```
                               +-----------------------+
                               |        Product        |
                               +-----------------------+
                                   /               \
                                  /                 \
                     +-----------------+       +-----------------+
                     | PhysicalProduct |       | DigitalProduct  |
                     +-----------------+       +-----------------+
                         /         \               /         \
                        /           \             /           \
                 +------------+ +------------+ +------------+ +------------+
                 |OneTimePhys | |SubscriptPhys| |OneTimeDigit| |SubscriptDig|
                 +------------+ +------------+ +------------+ +------------+
```

#### Why the interviewer rejects this:
1. **$M \times N$ Class Explosion:** If you have 3 fulfillment types (Physical, Digital, Service) and 4 billing models (One-Time, Weekly Subscription, Monthly Subscription, Free Trial), you must create $3 \times 4 = 12$ concrete subclasses!
2. **Duplicated Logic:** Subscription renewal logic is duplicated across `SubscriptPhys` and `SubscriptDig`.
3. **Rigid Nature:** What if a physical book is bundled with a digital e-book version? Static inheritance completely fails for product bundles.

---

### ❌ Anti-Pattern 2: The God Product Class (LSP & SRP Violation)

```
+-------------------------------------------------------------------------+
|                                 Product                                 |
+-------------------------------------------------------------------------+
| - isDigital: bool                                                       |
| - isSubscription: bool                                                  |
| - weightKg: double                                                      |
| - downloadUrl: string                                                   |
| - subscriptionIntervalDays: int                                         |
| + calculateShipping(): double -> if (isDigital) throw!                  |
| + generateDownloadLink(): string -> if (!isDigital) throw!              |
+-------------------------------------------------------------------------+
```

---

## 3. The Winning Whiteboard Architecture

### ✅ Core Insight: *"A Product has two independent, orthogonal dimensions: (1) How it is Fulfilled (`IFulfillmentStrategy`), and (2) How it is Priced & Billed (`IBillingModel`)."*

```
+-----------------------------------------------------------------------------------+
|                                      Product                                      |
+-----------------------------------------------------------------------------------+
| - sku: string                                                                     |
| - title: string                                                                   |
| - basePrice: double                                                               |
| - fulfillment: unique_ptr<IFulfillmentStrategy>                                   |
| - billingModel: unique_ptr<IBillingModel>                                         |
+-----------------------------------------------------------------------------------+
| + getFinalPrice(): double                                                         |
| + checkout(user: User): OrderItem                                                 |
| + fulfill(user: User): void                                                       |
| + requiresShipping(): bool                                                        |
+-----------------------------------------------------------------------------------+
             |                                              |
             | HAS-A (Bridge)                               | HAS-A (Strategy)
             v                                              v
+---------------------------------------+      +------------------------------------+
|             <<interface>>             |      |           <<interface>>            |
|         IFulfillmentStrategy          |      |           IBillingModel            |
+---------------------------------------+      +------------------------------------+
| + fulfillOrder(user: User)*: void     |      | + calculatePrice(basePrice)*: double
| + calculateShippingCost()*: double    |      | + isRecurring()*: bool             |
| + requiresShippingAddress()*: bool    |      | + getBillingCycleDescription()*    |
+---------------------------------------+      +------------------------------------+
                    ^                                             ^
                    | implements                                  | implements
         +----------+----------+                       +----------+----------+
         |                     |                       |                     |
+-----------------+   +-----------------+     +-----------------+   +-----------------+
|PhysicalShipping |   | DigitalDownload |     | OneTimeBilling  |   |SubscriptionBill |
+-----------------+   +-----------------+     +-----------------+   +-----------------+
| - weightKg      |   | - downloadExpiry|     | + calculatePrice|   | - intervalMonths|
| - dimensions    |   | - licenseKey    |     |   (return base) |   | - discountRate  |
+-----------------+   +-----------------+     +-----------------+   +-----------------+
| + fulfillOrder  |   | + fulfillOrder  |                           | + calculatePrice|
|   (Ship Parcel) |   |   (Email Link)  |                           |   (Apply Discnt)|
+-----------------+   +-----------------+                           +-----------------+
```

---

## 4. Concise Whiteboard Code (What to Write on Paper)

```cpp
// 1. Orthogonal Dimension A: Fulfillment Strategy (Bridge)
class IFulfillmentStrategy {
public:
    virtual ~IFulfillmentStrategy() = default;
    virtual void fulfill(const std::string& userEmail) = 0;
    virtual double getShippingCost() const = 0;
    virtual bool requiresPhysicalShipping() const = 0;
};

class PhysicalShippingStrategy : public IFulfillmentStrategy {
    double weightKg;
public:
    explicit PhysicalShippingStrategy(double w) : weightKg(w) {}
    void fulfill(const std::string& email) override { /* Generate warehouse pick-list & tracking # */ }
    double getShippingCost() const override { return weightKg * 5.0; }
    bool requiresPhysicalShipping() const override { return true; }
};

class DigitalDownloadStrategy : public IFulfillmentStrategy {
public:
    void fulfill(const std::string& email) override { /* Generate secure S3 download URL & email */ }
    double getShippingCost() const override { return 0.0; }
    bool requiresPhysicalShipping() const override { return false; }
};

// 2. Orthogonal Dimension B: Billing & Pricing Strategy
class IBillingModel {
public:
    virtual ~IBillingModel() = default;
    virtual double calculateFinalPrice(double basePrice) const = 0;
    virtual bool isSubscription() const = 0;
};

class OneTimePurchase : public IBillingModel {
public:
    double calculateFinalPrice(double basePrice) const override { return basePrice; }
    bool isSubscription() const override { return false; }
};

class MonthlySubscription : public IBillingModel {
    double recurringDiscountRate;
public:
    explicit MonthlySubscription(double discount = 0.10) : recurringDiscountRate(discount) {}
    double calculateFinalPrice(double basePrice) const override { return basePrice * (1.0 - recurringDiscountRate); }
    bool isSubscription() const override { return true; }
};

// 3. The Decoupled Product Entity (Composition Root)
class Product {
private:
    std::string sku;
    std::string title;
    double basePrice;
    std::unique_ptr<IFulfillmentStrategy> fulfillment;
    std::unique_ptr<IBillingModel> billing;

public:
    Product(std::string sku, std::string title, double price,
            std::unique_ptr<IFulfillmentStrategy> f,
            std::unique_ptr<IBillingModel> b)
        : sku(std::move(sku)), title(std::move(title)), basePrice(price),
          fulfillment(std::move(f)), billing(std::move(b)) {}

    double calculateTotalCheckoutPrice() const {
        return billing->calculateFinalPrice(basePrice) + fulfillment->getShippingCost();
    }

    void processOrder(const std::string& customerEmail) {
        fulfillment->fulfill(customerEmail);
    }
};
```

---

## 5. The 3-Minute Verbal Pitch (What to Say to the Interviewer)

> 1. *"If we model products via inheritance (e.g. `PhysicalSubscriptionBook`), we suffer an **$M \times N$ class explosion** because every fulfillment method multiplied by every billing model requires a new subclass."*
> 2. *"Putting all fields in a single `Product` class violates the **Single Responsibility Principle** and **Interface Segregation Principle**, as digital items would have meaningless weight and shipping attributes."*
> 3. *"Instead, I apply the **Bridge Pattern** by identifying the two orthogonal axes of variation: **Fulfillment (`IFulfillmentStrategy`)** and **Billing (`IBillingModel`)**."*
> 4. *"A `Product` holds a reference to both strategies. A physical magazine subscription is simply a `Product` composed of `PhysicalShippingStrategy` and `MonthlySubscription`."*
> 5. *"This design satisfies the **Open-Closed Principle (OCP)**: We can add an `InstantGiftCardDelivery` strategy or an `AnnualInstallmentBilling` model without touching existing product classes."*

---

## 6. Common Interviewer Follow-Ups & Quick Answers

| Follow-up Question | Recommended Response |
| :--- | :--- |
| **"How do you handle product bundles (e.g. Physical Book + E-Book)?"** | Use the **Composite Pattern**: Create a `CompositeFulfillmentStrategy` holding a `vector<unique_ptr<IFulfillmentStrategy>>` that triggers both warehouse shipping and download link generation. |
| **"What if a customer pauses or cancels a subscription?"** | Use the **State Pattern** on `SubscriptionBilling` (`ActiveState`, `PausedState`, `CancelledState`) without altering the product entity. |
| **"How do you support dynamic discounts (Black Friday promo codes)?"** | Use the **Decorator Pattern** (`DiscountDecorator` wrapping `IBillingModel`) to chain price modifications transparently. |
