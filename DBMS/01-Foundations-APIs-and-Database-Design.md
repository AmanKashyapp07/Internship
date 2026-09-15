# 01. Foundations, APIs & Database-Centered Design

> **The Core Mantra**: In system design interviews, never jump randomly into components. Follow the golden path:
> **Requirements ──► APIs ──► Data Model ──► Database ──► Scaling ──► Caching & Queues**

---

## PART 1: Foundations (The Communication Layer)

Before data reaches a database, it must travel from the user through the network and application tier.

### 1. Client–Server Architecture & Request Lifecycle

```
┌─────────────────────────────────────────────────────────────┐
│                      Client Device                          │
│               (Web Browser / Mobile App)                    │
└──────────────────────────────┬──────────────────────────────┘
                               │ 1. DNS Lookup (Domain ──► IP)
                               │ 2. TCP Handshake + TLS / SSL
                               │ 3. HTTP Request (Method, Path, Headers, Body)
                               ▼
┌─────────────────────────────────────────────────────────────┐
│                   Web / Application Server                  │
│             (Node.js, Go, Spring Boot, FastAPI)             │
└──────────────────────────────┬──────────────────────────────┘
                               │ 4. Validates Auth Token / Permissions
                               │ 5. Executes Business Logic
                               │ 6. Executes SQL Queries over Connection Pool
                               ▼
┌─────────────────────────────────────────────────────────────┐
│                       Database Tier                         │
│                    (PostgreSQL / MySQL)                     │
└─────────────────────────────────────────────────────────────┘
```

---

### 2. HTTP Basics & REST APIs

REST (Representational State Transfer) is the standard architectural style for web APIs.

#### HTTP Methods & Idempotency:
- **Idempotent**: Making the same request $N$ times leaves the system in the exact same state as making it once.

| Method | CRUD Action | Idempotent? | Safe (Read-Only)? | Typical Use Case |
| :--- | :--- | :---: | :---: | :--- |
| `GET` | Read | **YES** | **YES** | `GET /api/v1/restaurants/42` |
| `POST` | Create | **NO** | **NO** | `POST /api/v1/orders` (creates new order) |
| `PUT` | Full Replace | **YES** | **NO** | `PUT /api/v1/users/42` (replaces entire profile) |
| `PATCH`| Partial Update| **NO / Conditional**| **NO** | `PATCH /api/v1/users/42` (updates bio only) |
| `DELETE`| Delete | **YES** | **NO** | `DELETE /api/v1/orders/101` |

#### Crucial HTTP Status Codes for Interviews:
- `200 OK`: Success with response payload.
- `201 Created`: Resource successfully created (standard for `POST`).
- `400 Bad Request`: Client validation failure (malformed JSON, missing fields).
- `401 Unauthorized`: Missing or invalid authentication token (Who are you?).
- `403 Forbidden`: Authenticated, but lacks permission (You can't delete another user's order).
- `404 Not Found`: Resource does not exist.
- `429 Too Many Requests`: Client exceeded Rate Limits.
- `500 Internal Server Error`: Backend crashed or unhandled exception.
- `502 / 503 Bad Gateway / Service Unavailable`: Upstream server down or overloaded.

---

### 3. Stateless vs Stateful Servers

```
STATEFUL ARCHITECTURE (Anti-Pattern at Scale)
Client A ──► [Server 1 (Stores session in local RAM)]
Client A ──► [Server 2] ──► 💥 Error: "Session not found! Please log in again."
(Requires complex Sticky Sessions / IP Hashing on Load Balancer).

STATELESS ARCHITECTURE (Production Standard)
Client A ──► [Server 1] ──┐
                          ├──► [Centralized Session Store / Redis] OR [Self-Contained JWT]
Client A ──► [Server 2] ──┘
(Any server can handle any request; scaling out is trivial).
```

- **Stateless Rule**: Server memory retains zero client state between requests. Every incoming request must carry all necessary context (e.g., JWT token in `Authorization: Bearer <token>`).

---

### 4. Authentication vs Authorization

| Dimension | Authentication (AuthN) | Authorization (AuthZ) |
| :--- | :--- | :--- |
| **Question** | *"Who are you?"* | *"What are you allowed to do?"* |
| **Mechanism** | Passwords, OTP, OAuth2 (Google Login), Biometrics. | Role-Based Access Control (RBAC), Permissions matrix. |
| **Artifact** | Issues an identity token (e.g., JWT signed with secret). | Checks if `token.role == 'ADMIN'` before executing action. |
| **HTTP Code** | `401 Unauthorized` (if missing/expired token). | `403 Forbidden` (if role has insufficient privilege). |

---

## PART 2: Database-Centered Design

This is the core skill in technical interviews: **turning English business requirements into a robust, normalized database schema**.

### 1. Entities, Keys & Relationships

```
┌─────────────────┐       ┌─────────────────┐       ┌─────────────────┐
│     1 : 1       │       │     1 : N       │       │     M : N       │
│  One-to-One     │       │  One-to-Many    │       │  Many-to-Many   │
└────────┬────────┘       └────────┬────────┘       └────────┬────────┘
         ▼                         ▼                         ▼
  User ──► UserProfile     Restaurant ──► MenuItems    Order ◄──► MenuItem
(Profile has user_id FK) (MenuItem has rest_id FK) (Via OrderItem Junction)
```

1. **Primary Key (PK)**: Uniquely identifies a row.
   - *Auto-increment BIGINT*: Compact (8 bytes), fast sequential B+ tree inserts. Exposes business volume (`id=1052` means 1,052 users).
   - *UUID v4*: Globally unique across distributed databases without a central coordinator, but 16 bytes and causes index fragmentation due to random order.
   - *UUID v7 (Modern)*: Time-ordered UUIDs—distributed, unique, and B+ tree friendly.
2. **Foreign Key (FK)**: Enforces referential integrity between parent and child tables.
3. **Relationships**:
   - **1:1**: Parent table PK is referenced as a `UNIQUE` foreign key in child table.
   - **1:N**: Child table stores a foreign key referencing the parent table's PK.
   - **M:N**: Requires a **Junction Table** (Associative Entity) containing two foreign keys.

---

### 2. Normalization vs Strategic Denormalization

- **Normalization (3NF)**:
  - *Goal*: Eliminate data redundancy and avoid update anomalies.
  - *Rule*: Every non-key attribute must depend on the key, the whole key, and nothing but the key.
- **Denormalization (For Read Performance)**:
  - *Goal*: Pre-calculate or duplicate data to avoid expensive multi-table `JOIN` operations at high scale.
  - *Example*: Storing `total_amount` directly on the `orders` table instead of running `SUM(price * quantity)` across 50 `order_items` on every user page reload.

---

### 3. Read vs Write Patterns & Workload Analysis

Before drafting schemas or choosing indexes, characterize the system's access pattern:

| Workload Type | Read : Write Ratio | Characteristics | Primary Optimizations | Example Systems |
| :--- | :---: | :--- | :--- | :--- |
| **Read-Heavy** | $100:1$ to $1000:1$ | High read traffic, infrequent updates, stale reads often tolerated. | Multi-level Caching (Redis/CDN), Read Replicas, Covering Indexes. | Social feeds, Wikipedia, E-commerce catalogs, URL shortener. |
| **Write-Heavy** | $1:10$ to $1:1$ | High ingestion throughput, append-mostly, real-time inserts. | LSM-Tree engines, Write Queues (Kafka/RabbitMQ), Sharding, Batching. | Telemetry/IoT, Audit logs, Financial ledgers, Uber driver GPS tracking. |
| **OLTP** | Real-time | High concurrency, short ACID transactions, low latency ($<10$ms). | Normalized tables, B+ Tree indexes on PK/FK, Connection pooling. | Banking, Order checkout, Booking reservation. |
| **OLAP** | Analytical | Low concurrency, complex aggregate scans over millions of rows. | Denormalized star schema, Columnar databases (ClickHouse, Snowflake). | Business intelligence dashboards, Year-end analytics reports. |

---

### 4. Concrete Schema Walkthrough: Food Delivery System (Swiggy / DoorDash)

Let's design the complete data model for a food ordering platform:

```
┌───────────────────────┐                    ┌───────────────────────┐
│         users         │                    │      restaurants      │
├───────────────────────┤                    ├───────────────────────┤
│ id (PK)               │                    │ id (PK)               │
│ name, email, phone    │                    │ name, address, rating │
└───────────┬───────────┘                    └───────────┬───────────┘
            │                                            │
            │ 1:N                                        │ 1:N
            ▼                                            ▼
┌───────────────────────┐                    ┌───────────────────────┐
│        orders         │◄───────────────────┤      menu_items       │
├───────────────────────┤   References rest  ├───────────────────────┤
│ id (PK)               │                    │ id (PK)               │
│ user_id (FK)          │                    │ restaurant_id (FK)    │
│ restaurant_id (FK)    │                    │ name, price, is_veg   │
│ status, total_amount  │                    └───────────┬───────────┘
└───────────┬───────────┘                                │
            │                                            │
            │ 1:N                                        │ 1:N
            ▼                                            ▼
┌────────────────────────────────────────────────────────┐
│                      order_items                       │
│           (M:N Junction Table: Order <-> Item)         │
├────────────────────────────────────────────────────────┤
│ id (PK)                                                │
│ order_id (FK)                                          │
│ menu_item_id (FK)                                      │
│ quantity, unit_price                                   │
└────────────────────────────────────────────────────────┘
```

#### Production SQL Schema (DDL):

```sql
-- 1. Users Table
CREATE TABLE users (
    id BIGSERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(255) UNIQUE NOT NULL,
    phone VARCHAR(20) UNIQUE NOT NULL,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

-- 2. Restaurants Table
CREATE TABLE restaurants (
    id BIGSERIAL PRIMARY KEY,
    name VARCHAR(150) NOT NULL,
    address TEXT NOT NULL,
    is_active BOOLEAN DEFAULT TRUE,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

-- 3. Menu Items Table (1:N with Restaurant)
CREATE TABLE menu_items (
    id BIGSERIAL PRIMARY KEY,
    restaurant_id BIGINT NOT NULL REFERENCES restaurants(id) ON DELETE CASCADE,
    name VARCHAR(150) NOT NULL,
    price DECIMAL(10, 2) NOT NULL CHECK (price >= 0),
    is_available BOOLEAN DEFAULT TRUE,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);
CREATE INDEX idx_menu_items_restaurant ON menu_items(restaurant_id);

-- 4. Orders Table (1:N with User, 1:N with Restaurant)
CREATE TABLE orders (
    id BIGSERIAL PRIMARY KEY,
    user_id BIGINT NOT NULL REFERENCES users(id),
    restaurant_id BIGINT NOT NULL REFERENCES restaurants(id),
    status VARCHAR(30) NOT NULL DEFAULT 'PLACED', -- PLACED, CONFIRMED, DELIVERING, COMPLETED, CANCELLED
    total_amount DECIMAL(10, 2) NOT NULL DEFAULT 0.00,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);
CREATE INDEX idx_orders_user ON orders(user_id, created_at DESC);

-- 5. Order Items Table (Junction Table for M:N Order <-> MenuItem)
CREATE TABLE order_items (
    id BIGSERIAL PRIMARY KEY,
    order_id BIGINT NOT NULL REFERENCES orders(id) ON DELETE CASCADE,
    menu_item_id BIGINT NOT NULL REFERENCES menu_items(id),
    quantity INT NOT NULL CHECK (quantity > 0),
    unit_price DECIMAL(10, 2) NOT NULL, -- Snapshot of price at time of order!
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);
CREATE INDEX idx_order_items_order ON order_items(order_id);

-- 6. Payments Table (1:1 with Order)
CREATE TABLE payments (
    id BIGSERIAL PRIMARY KEY,
    order_id BIGINT UNIQUE NOT NULL REFERENCES orders(id),
    payment_method VARCHAR(50) NOT NULL, -- UPI, CREDIT_CARD, CASH
    transaction_ref VARCHAR(100) UNIQUE,
    status VARCHAR(30) NOT NULL DEFAULT 'PENDING', -- PENDING, SUCCESS, FAILED
    amount DECIMAL(10, 2) NOT NULL,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);
```

---

### 5. Transactions & ACID Guarantees

When placing a food order:
1. Deduct user wallet balance.
2. Insert order record.
3. Clear shopping cart.

If step 2 succeeds but step 3 crashes, your system has corrupted state. **Transactions ensure all-or-nothing execution.**

- **A (Atomicity)**: Either all statements in the transaction succeed, or the entire transaction is rolled back.
- **C (Consistency)**: Database moves from one valid state to another, satisfying all foreign keys, checks, and unique constraints.
- **I (Isolation)**: Concurrent transactions do not interfere with or corrupt each other's in-progress state.
- **D (Durability)**: Once a transaction commits, data is guaranteed to survive server power outages or crashes via the Write-Ahead Log (WAL).

---

## 6. Interview Cheat Sheet: Schema Design Rubric

```
1. "Clarify the entities from the user stories (Who is buying? What are they buying? Who is selling?)."
2. "Identify cardinality (Is it 1:1, 1:N, or M:N?)."
3. "Always snapshot transactional prices in order_items (Menu item price might change tomorrow!)."
4. "Index all foreign keys and frequently filtered columns (e.g., user_id + created_at)."
5. "Protect critical state transitions with ACID transactions and unique idempotency constraints."
```
