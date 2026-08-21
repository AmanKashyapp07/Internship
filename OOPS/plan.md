# Low-Level Object-Oriented Design & Architecture Master Plan

> **Goal:** Master advanced object-oriented design, design patterns, and architectural trade-offs in modern C++17.
> **Target Folder:** `OOPS/`
> **Approach:** High educational value — every problem illustrates the *anti-pattern* (why naive inheritance fails), the *optimal design pattern*, and clean, production-grade C++17 code using smart pointers, standard abstractions, and clean SOLID principles.

---

## Pedagogical Philosophy for Whiteboard / Paper Interviews

To maximize interview performance, every `.md` document in this repository follows a 6-step whiteboard structure:

1. **The Interview Question & Exact Phrasing**: What interviewers actually ask in live rounds.
2. **The Naive Diagram (What NOT to draw on paper)**: Visualizes the anti-patterns (Diamond Problem, God Object, $2^N$ combinatorial explosion) and explicitly details why interviewers reject them.
3. **The Winning Whiteboard Architecture (UML & Mermaid)**: Clean class diagrams showing interfaces, HAS-A composition, and loose coupling.
4. **Minimal Whiteboard Code Snippets**: The exact 20–30 lines of clean, readable C++ interface and class definitions to write on paper.
5. **The 3-Minute Verbal Pitch**: The exact explanation script to articulate your design choices out loud to the interviewer.
6. **Common Follow-up Questions & Edge Cases**: Rapid-fire answers to deep-dive follow-ups.

---

## Problem Index & Architecture Mapping

```
OOPS/
├── plan.md                                    <- This Roadmap
├── 01_Role_Behavior_Combinations/
│   ├── 00_cricket_player_roles.md             <- Role Object / Capability (Player / Batsman / Bowler)
│   ├── 01_employee_roles.md                   <- Role Object / Composition (IC / Manager / Tech Lead)
│   ├── 02_vehicle_amphibious.md               <- Bridge & Capability (Car / Boat / Amphibious / Flying)
│   ├── 03_smart_device_capabilities.md        <- Interface Segregation (Printer / Scanner / Copier / Fax)
│   └── 04_ecommerce_products.md               <- Orthogonal Decoupling (Physical / Digital / Subscriptions)
├── 02_Dynamic_State_Entity_Evolution/
│   ├── 05_user_roles_permissions.md           <- Dynamic Role Attachment / Policy Pattern
│   ├── 06_game_character_classes.md           <- Entity-Component-System & Skill Composite
│   ├── 07_banking_account_capabilities.md     <- Decorator / Capability Wrappers
│   └── 08_customizable_food_orders.md         <- Decorator & Builder Pattern
├── 03_Polymorphic_Operations_MultiAxis/
│   ├── 09_payment_processing_matrix.md        <- Bridge + Strategy + Factory Pattern
│   ├── 10_notification_system.md              <- Bridge (Formatters × Delivery Channels)
│   └── 11_db_query_and_connection.md          <- Strategy (SQL Dialect) × Bridge (Transport)
└── 04_Hierarchical_Aggregation_Modeling/
    ├── 12_in_memory_file_system.md            <- Composite Pattern + Visitor
    ├── 13_ui_component_tree.md                <- Composite Pattern + Chain of Responsibility
    └── 14_organization_budgeting.md           <- Composite Pattern + Hierarchical Aggregation
```

---

## Detailed Problem Breakdown

### MODULE 1: Role & Behavior Combinations (Multiple Inheritance Traps)

---

#### 01. Employee / Manager / Director / Tech Lead
- **The Problem:** An employee can be an individual contributor (`IC`), a manager of a team, or both simultaneously (a `TechLead` who codes and manages reviews).
- **The Naive Trap:** Creating subclasses for every combination (`Manager`, `Director`, `TechLead`, `ManagerTechLead`, etc.) leads to an $O(2^N)$ class explosion and makes runtime promotion/role changes impossible without reallocating objects.
- **Architectural Solution:** **Role Object Pattern / Composition of Responsibilities**.
  - Separate the stable identity (`Employee` having ID, name, base salary) from dynamic roles (`IRole`: `IndividualContributorRole`, `ManagerRole`, `DirectorRole`).
  - An `Employee` maintains a collection of active `unique_ptr<IRole>` and delegates work dynamically.
- **Key Concepts Taught:** Single Responsibility Principle (SRP), Composition over Inheritance, Dynamic Role Switching.

---

#### 02. Vehicle / Amphibious Car / Flying Car
- **The Problem:** We have land vehicles (`Car`), watercraft (`Boat`), and aircraft (`Plane`). An `AmphibiousCar` drives on roads and floats on water; a `FlyingCar` drives and flies.
- **The Naive Trap:** Using multiple inheritance (`class AmphibiousCar : public Car, public Boat`) creates the classic **Diamond Problem** (duplicated base state, ambiguous method resolution, fragile virtual base classes).
- **Architectural Solution:** **Bridge Pattern / Capability-Based Composition**.
  - Define distinct propulsion/movement strategies (`ILandPropulsion`, `IWaterPropulsion`, `IAirPropulsion`).
  - A `Vehicle` holds optional propulsion capabilities (or a list of capability interfaces).
- **Key Concepts Taught:** Avoiding Diamond Inheritance, Capability Modeling, Bridge Pattern.

---

#### 03. Smart Device Features (Printer / Scanner / Copier / Fax)
- **The Problem:** Physical office devices vary wildly: `BasicPrinter` (print only), `Multifunction` (print + scan + copy), `EnterpriseHub` (print + scan + fax + network storage).
- **The Naive Trap:** A monolithic `SmartDevice` base class with empty/throwing methods (`scan()` throwing `NotSupportedException` in `BasicPrinter`), violating the Liskov Substitution Principle and Interface Segregation Principle.
- **Architectural Solution:** **Interface Segregation Principle (ISP) + Component Strategy**.
  - Granular interfaces: `IPrinter`, `IScanner`, `IFax`, `INetworkSharable`.
  - Devices are composed of modular hardware engines (`PrintEngine`, `ScanEngine`, etc.) that implement these interfaces cleanly.
- **Key Concepts Taught:** Interface Segregation Principle (ISP), Liskov Substitution Principle (LSP), Component Architecture.

---

#### 04. E-Commerce Products (Physical / Digital / Subscription)
- **The Problem:** A book can be physical (requires shipping address, weight calculation, warehouse tracking) or digital (generates instant download links, zero shipping). A magazine can be a recurring subscription of physical or digital deliveries.
- **The Naive Trap:** Creating rigid subclasses like `PhysicalBook`, `DigitalBook`, `PhysicalSubscriptionMagazine`, `DigitalSubscriptionMagazine`.
- **Architectural Solution:** **Type-Object / Bridge Pattern with Delivery & Billing Strategies**.
  - A `Product` entity is decoupled from its `FulfillmentStrategy` (`PhysicalShippingStrategy`, `DigitalDownloadStrategy`) and its `PricingModel` (`OneTimePricing`, `RecurringSubscriptionPricing`).
- **Key Concepts Taught:** Orthogonal Dimension Decoupling, Open-Closed Principle (OCP).

---

### MODULE 2: Dynamic State & Entity Evolution (Runtime Role Changes)

---

#### 05. User Permission & Account Roles (Customer / Seller / Admin)
- **The Problem:** A user registers as a `Buyer`, later opens a shop to become a `Seller`, and can receive temporary `Moderator` or `Admin` privileges.
- **The Naive Trap:** Storing separate objects for `BuyerUser` and `SellerUser` forces data migration, copies user history, and breaks references when a user upgrades their account.
- **Architectural Solution:** **State / Dynamic Role Attachment & Policy Pattern**.
  - `UserAccount` holds a persistent identity and a dynamic `std::unordered_set<std::shared_ptr<IUserRole>>`.
  - Operations check permissions polymorphically via role delegates (`hasPermission(Permission::LIST_ITEM)`).
- **Key Concepts Taught:** Runtime Entity Evolution, Fine-Grained Authorization, Policy-Based Design.

---

#### 06. Game Character Class System (Warrior / Mage / Paladin)
- **The Problem:** Characters can wield weapons, cast spells, or be hybrids (`Paladin`). Characters can respecialize, unlock talent branches, or equip artifacts that bestow new abilities during gameplay.
- **The Naive Trap:** Rigid inheritance trees (`Character -> Warrior -> Paladin`) make runtime respecialization and hybrid skill combinations impossible.
- **Architectural Solution:** **Entity-Component System (ECS) Light & Skill Composite**.
  - `Character` contains component slots (`CombatComponent`, `MagicComponent`, `InventoryComponent`).
  - Abilities are modeled as `IAbility` objects managed in a composite `SpellBook` or `TalentTree`.
- **Key Concepts Taught:** Entity-Component Modeling, Command & Strategy Patterns in Game Architecture.

---

#### 07. Banking Account Types (Savings / Current / Overdraft)
- **The Problem:** An account starts as standard savings, but is later upgraded to include an overdraft facility or foreign-currency balance without altering transaction logs or account IDs.
- **The Naive Trap:** Changing account type by deleting the old object and instantiating an `OverdraftAccount` loses transaction history and invalidates pointers/references.
- **Architectural Solution:** **Decorator / Account Feature Wrapper & State Pattern**.
  - Base `BankAccount` encapsulates core balance and transaction ledger.
  - Features like `OverdraftFacility` and `MultiCurrencyFeature` wrap withdrawal and balance checks via Decorators or Strategy delegates.
- **Key Concepts Taught:** Non-Invasive Feature Augmentation, Decorator Pattern, Invariant Preservation.

---

#### 08. Restaurant Order Items (Customizable Food Items)
- **The Problem:** A burger has a base price and calories. Customers customize it with extra cheese, double patties, gluten-free buns, or sauce removals.
- **The Naive Trap:** Subclassing combinations (`CheeseBurgerWithDoublePattyAndGlutenFreeBun`) creates combinatorial explosion.
- **Architectural Solution:** **Classic Decorator Pattern + Builder Pattern**.
  - Base interface `IFoodItem` with `getPrice()`, `getCalories()`, and `getDescription()`.
  - Base items (`Burger`, `Pizza`) and concrete decorators (`ExtraCheeseDecorator`, `GlutenFreeBunDecorator`).
  - A `FoodItemBuilder` creates custom food stacks cleanly.
- **Key Concepts Taught:** Recursive Composition, Decorator Pattern, Fluent Builder API.

---

### MODULE 3: Polymorphic Operations & Multi-Axis Variations

---

#### 09. Payment Processing (Payment Methods × Gateways)
- **The Problem:** Support multiple payment methods (`CreditCard`, `UPI`, `NetBanking`) across multiple payment gateways (`Stripe`, `Razorpay`, `PayPal`).
- **The Naive Trap:** Creating $M \times N$ classes like `StripeCreditCard`, `RazorpayUPI`, `PayPalNetBanking`. Adding 1 method and 1 gateway requires $M + N + 1$ new classes.
- **Architectural Solution:** **Bridge Pattern + Factory Pattern**.
  - **Abstraction:** `PaymentMethod` (defines payment workflow, input validation, and user authorization).
  - **Implementor:** `IPaymentGateway` (handles gateway-specific API protocols, tokens, and settlement).
  - Adding a method or gateway is strictly $O(1)$ independent work.
- **Key Concepts Taught:** The Bridge Pattern in enterprise systems, Multi-Axis Variation Decoupling.

---

#### 10. Notification System (Channels × Formats)
- **The Problem:** Send notifications across multiple channels (`SMS`, `Email`, `Push`, `Slack`) formatted in multiple styles (`PlainText`, `HTML`, `Markdown`, `JSON`).
- **The Naive Trap:** `EmailHtmlNotification`, `SmsPlainTextNotification`, `PushJsonNotification` ($M \times N$ class explosion).
- **Architectural Solution:** **Bridge Pattern / Formatter Strategy + Channel Adapters**.
  - `IMessageFormatter` converts message payload into the target markup format.
  - `INotificationChannel` transmits the formatted payload to the transport medium.
  - `NotificationService` combines them seamlessly at runtime.
- **Key Concepts Taught:** Decoupling Content Presentation from Delivery Transport, Adapter Pattern.

---

#### 11. Database Connection & Query Builder
- **The Problem:** Support PostgreSQL, MySQL, and SQLite. Each has unique SQL dialect syntax (e.g., `LIMIT/OFFSET` vs `TOP`, string concatenation) and unique connection/transport protocols.
- **The Naive Trap:** Tightly coupling SQL string building with network socket management inside a single `Database` hierarchy.
- **Architectural Solution:** **Strategy (Query Dialect) × Bridge (Connection Transport) + Abstract Factory**.
  - `ISqlDialect`: Generates dialect-specific SQL AST/strings.
  - `IDbConnection`: Manages transactions, connection pooling, and protocol execution.
  - `DatabaseClientFactory`: Pairs matching dialect and connection objects.
- **Key Concepts Taught:** Abstract Factory, Query Builder Strategy, Protocol/Dialect Separation.

---

### MODULE 4: Hierarchical & Aggregation Modeling

---

#### 12. In-Memory File System (File vs. Folder)
- **The Problem:** A file system contains files and directories. Both respond to `getSize()`, `printPath()`, `delete()`, while directories also support `addNode(INode)`.
- **The Naive Trap:** Type-casting using `dynamic_cast<Directory*>` everywhere or adding dummy directory methods into the file class.
- **Architectural Solution:** **Composite Pattern + Visitor Pattern**.
  - Abstract base `FileSystemNode` provides uniform access to common attributes (`getName()`, `getSize()`, `display()`).
  - `File` is the leaf node; `Directory` is the composite holding `vector<unique_ptr<FileSystemNode>>`.
  - A `FileSystemVisitor` enables external operations like virus scanning, disk usage analysis, or JSON export without polluting node classes.
- **Key Concepts Taught:** Composite Pattern, Visitor Pattern, Recursive Tree Traversal.

---

#### 13. UI Component Tree (Button / Container / Window)
- **The Problem:** A GUI tree contains leaf widgets (`Button`, `Label`, `TextBox`) and layout containers (`Panel`, `Window`) that hold children. Events like `render()`, `resize()`, and `handleMouseEvent()` must propagate smoothly down or up the hierarchy.
- **The Naive Trap:** Spaghetti coordinate translation and manual type-switching to dispatch events to nested panels.
- **Architectural Solution:** **Composite Pattern + Chain of Responsibility**.
  - `UIComponent` base class defines uniform rendering and bounding-box geometry.
  - `UIContainer` handles child layout management and recursive rendering.
  - User events bubble up or drill down using the Chain of Responsibility.
- **Key Concepts Taught:** Hierarchical UI Architecture, Chain of Responsibility, Composite Pattern.

---

#### 14. Organization Chart & Department Budgeting
- **The Problem:** An enterprise consists of individual employees and nested departments. Calculating total headcounts, payroll budgets, and operational overhead requires aggregating leaves and sub-trees uniformly.
- **The Naive Trap:** Distinct data structures for employees and departments requiring customized recursive functions for every new reporting metric.
- **Architectural Solution:** **Composite Pattern + Iterator Pattern**.
  - `IOrgUnit` interface (`getBudget()`, `getHeadcount()`, `getStaffDirectory()`).
  - `Employee` acts as leaf; `Department` acts as composite managing sub-departments and staff.
- **Key Concepts Taught:** Financial Aggregation Modeling, Clean Tree Traversal, Composite Pattern.

---

## Implementation Quality Checklist

When implementing each `.cpp` file:
- [ ] Compiles cleanly with `g++ -std=c++17 -Wall -Wextra`
- [ ] No memory leaks (uses RAII and smart pointers)
- [ ] Clear comments detailing:
  - *Problem Statement & Context*
  - *Anti-Pattern / Naive Approach (with explanation of flaws)*
  - *Applied Design Pattern(s)*
  - *SOLID Principles Illustrated*
- [ ] Working `main()` test driver demonstrating realistic usage and edge cases

---

*This plan acts as the single source of truth for the OOPS low-level design repository.*
