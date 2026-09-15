# 05. The 7-Step Framework & 6 Classic System Designs

> **Interview Mindset**: In a 45-minute System Design interview, the interviewer cares more about your **process** than memorized answers. 
> Memorize the 7-step sequence below, and you will navigate every single design problem calmly and systematically.

---

## The Universal 7-Step Interview Framework

```
1. Clarify Requirements (Functional & Non-Functional)
       │
       ▼
2. Define APIs (REST endpoints, parameters, status codes)
       │
       ▼
3. Define Entities & Database Schema (Tables, PK/FK, Relationships)
       │
       ▼
4. Draw Basic Architecture (Client ──► Server ──► Database)
       │
       ▼
5. Identify Bottlenecks (CPU, RAM, Read/Write volume, Disk I/O)
       │
       ▼
6. Scale the System (Indexes ──► Cache ──► Replicas ──► Queues ──► S3)
       │
       ▼
7. Discuss Trade-offs (CAP, Consistency, Failures, Monitoring)
```

### The Framework in Action: "Design Instagram"

```
1. Requirements
   └── Share photos, follow users, generate chronological home feed.
       │
       ▼
2. APIs
   └── POST /api/v1/posts, POST /api/v1/follow, GET /api/v1/feed.
       │
       ▼
3. Entities & Database Schema
   └── users, posts, follows, likes tables with relational foreign keys.
       │
       ▼
4. Basic Architecture
   └── Client ──► App Server ──► Single Database.
       │
       ▼
5. Identify Bottlenecks
   └── Generating feed is sluggish (expensive multi-table JOINs across all followees).
       │
       ▼
6. Add Caching & Async Scaling
   ├── Cache: Redis stores pre-computed home timeline for fast O(1) reads.
   ├── Media: Photos stored in AWS S3 with CDN edge caching (NOT in DB).
   ├── DB Scaling: Primary + Read Replicas, Sharding posts by user_id.
   └── Message Queue: Kafka workers asynchronously fan-out new posts to followers' feeds.
       │
       ▼
7. Discuss Trade-offs
   └── Hybrid Fan-out: Push for normal users, Pull for celebrities with millions of followers.
```

---

## Case Study 1: URL Shortener (TinyURL)

### 1. Requirements:
- **Functional**: Shorten a long URL to a 7-character alias. Redirect short URL to original long URL ($301 / 302$).
- **Non-Functional**: Highly available, sub-10ms redirect latency, read-heavy ($100:1$).

### 2. APIs:
- `POST /api/v1/urls` ──► Body: `{ "long_url": "https://..." }` ──► Returns `201 Created` `{ "short_url": "https://tiny.url/aB3x9k" }`.
- `GET /{short_code}` ──► Returns `302 Found` (Redirect) with `Location: https://...`. *(Use 302 instead of 301 so analytics tracking hits your server).*

### 3. Database Schema:
```sql
CREATE TABLE urls (
    id BIGSERIAL PRIMARY KEY,
    short_code VARCHAR(10) UNIQUE NOT NULL,
    long_url TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
CREATE UNIQUE INDEX idx_urls_short_code ON urls(short_code);
```
- **Base62 Encoding**: Use characters `[0-9][a-z][A-Z]`. With 7 characters: $62^7 \approx 3.5 \text{ trillion}$ unique URLs.

### 4. Architecture & Scaling:
```
Client ──► Load Balancer ──► App Servers ──► Redis Cache (Top 20% URLs)
                                                    │ Cache Miss
                                                    ▼
                                            Read Replicas (PostgreSQL)
```
- **Caching**: 80/20 rule (20% of hot URLs generate 80% of redirect traffic). Store in Redis: `SET short_code -> long_url`.

---

## Case Study 2: Library Management System

### 1. Requirements:
- Track books, multiple physical copies of each book, members, loans, and reservations.
- Concurrency: Prevent two members from borrowing the same physical book copy simultaneously.

### 2. Database Schema (Relational Modeling):
```
┌─────────────────┐       ┌─────────────────┐       ┌─────────────────┐
│      books      │       │   book_copies   │       │     members     │
├─────────────────┤       ├─────────────────┤       ├─────────────────┤
│ id (PK)         │1 ──► N│ id (PK)         │       │ id (PK)         │
│ isbn, title     │       │ book_id (FK)    │       │ name, email     │
│ author, category│       │ status (AVAIL.) │       │ membership_date │
└─────────────────┘       └────────┬────────┘       └────────┬────────┘
                                   │                         │
                                   │ 1:N                     │ 1:N
                                   ▼                         ▼
                          ┌───────────────────────────────────┐
                          │               loans               │
                          ├───────────────────────────────────┤
                          │ id (PK)                           │
                          │ book_copy_id (FK)                 │
                          │ member_id (FK)                    │
                          │ issue_date, due_date, return_date │
                          └───────────────────────────────────┘
```

### 3. Transactional Integrity:
When borrowing a book, run inside a strict transaction with **Pessimistic Locking**:
```sql
BEGIN;
-- 1. Find and lock an available copy
SELECT id FROM book_copies 
WHERE book_id = 42 AND status = 'AVAILABLE' 
LIMIT 1 FOR UPDATE;

-- 2. Mark copy as borrowed
UPDATE book_copies SET status = 'BORROWED' WHERE id = 101;

-- 3. Record loan
INSERT INTO loans (book_copy_id, member_id, issue_date, due_date)
VALUES (101, 5, CURRENT_DATE, CURRENT_DATE + INTERVAL '14 days');
COMMIT;
```

---

## Case Study 3: Food Ordering System (Swiggy / DoorDash)

### 1. Requirements:
- Browse menus, place orders, process payments, and track order state.
- Order state machine: `PLACED` $\to$ `CONFIRMED` $\to$ `PREPARING` $\to$ `DELIVERING` $\to$ `COMPLETED`.

### 2. Architecture & Concurrency:

```
┌──────────────┐     POST /orders     ┌──────────────┐   Validate & Charge   ┌─────────────────┐
│    Client    ├─────────────────────►│  App Server  ├──────────────────────►│ Payment Gateway │
└──────────────┘                      └──────┬───────┘                       └─────────────────┘
                                             │
                                             │ 1. Atomic Order Creation
                                             ▼
                                      ┌──────────────┐
                                      │ PostgreSQL   │ (Tables: orders, order_items, payments)
                                      └──────┬───────┘
                                             │
                                             │ 2. Publishes OrderPlacedEvent
                                             ▼
                                      ┌──────────────┐
                                      │ Apache Kafka ├──► Restaurant Tablet App
                                      └──────┬───────┘
                                             └──► Delivery Rider Allocation Service
```

- **Idempotency**: Client passes `Idempotency-Key: uuid` on order checkout to prevent double charges on mobile network reconnects.

---

## Case Study 4: Real-Time Chat System (WhatsApp / Slack)

### 1. Requirements:
- 1-on-1 and group messaging. Real-time delivery, message persistence, unread counters.

### 2. Communication Protocol:
- **WebSockets**: Full-duplex persistent bidirectional TCP connection between client and chat servers.

### 3. Database Selection (Why Cassandra / Wide-Column?):
- Relational databases choke on millions of concurrent message inserts.
- **Apache Cassandra** is built for append-only sequential writes (LSM-Tree):
```sql
CREATE TABLE messages (
    chat_id UUID,
    message_id TIMEUUID,
    sender_id UUID,
    content TEXT,
    PRIMARY KEY (chat_id, message_id)
) WITH CLUSTERING ORDER BY (message_id DESC);
```
- **Partition Key (`chat_id`)**: Groups all messages of a chat together on the same physical node.
- **Clustering Key (`message_id DESC`)**: Physically sorts messages in reverse chronological order on disk for instant $O(1)$ range retrieval of recent messages.
- **Unread Counters**: Maintained in Redis using atomic hash increments (`HINCRBY unread:user_id chat_id 1`).

---

## Case Study 5: File Storage System (Google Drive / Dropbox)

### 1. Requirements:
- Upload and download large files (PDFs, photos, videos), share files, organize into folders.

### 2. The Golden Rule: Metadata vs Actual Files

```
┌──────────────┐     1. POST /upload-request (Metadata)     ┌──────────────┐
│    Client    ├───────────────────────────────────────────►│  API Server  │
└──────┬───────┘                                            └──────┬───────┘
       │                                                           │ 2. Saves metadata
       │ 3. Direct Multi-Part Upload                               ▼
       │    (Pre-Signed S3 URL)                             ┌──────────────┐
       ▼                                                    │ PostgreSQL   │
┌──────────────┐                                            │ (Files Table)│
│   AWS S3     │                                            └──────────────┘
│ Object Store │
└──────────────┘
```

- **Database (PostgreSQL)** stores only metadata:
  ```sql
  CREATE TABLE files (
      id UUID PRIMARY KEY,
      user_id BIGINT REFERENCES users(id),
      filename VARCHAR(255) NOT NULL,
      s3_key VARCHAR(500) NOT NULL,
      file_size_bytes BIGINT NOT NULL,
      mime_type VARCHAR(100) NOT NULL,
      created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
  );
  ```
- **Object Store (AWS S3)** stores raw file bytes.
- **Optimization (Deduplication)**: Client calculates SHA-256 hash of file before uploading. If hash already exists in DB, increment reference count and return immediately—zero storage or upload bandwidth wasted!

---

## Case Study 6: Social Media Feed (Twitter / Instagram)

### 1. Requirements:
- Users post tweets/photos, follow other users, and view a personalized reverse-chronological timeline of people they follow.
- Workload: **Massively Read-Heavy** ($1,000:1$ reads to writes).

### 2. Fan-out on Write (Push) vs Fan-out on Read (Pull):

| Model | How it Works | Pros | Cons |
| :--- | :--- | :--- | :--- |
| **Fan-out on Read (Pull)** | When user opens app, query DB for all followed users and merge their tweets on the fly. | Fast writes; zero background processing when user is inactive. | **Slow reads**: Generating feed requires joining millions of rows across followed users. |
| **Fan-out on Write (Push)** | When user posts, background worker immediately writes the tweet ID into every follower's timeline in Redis. | **Blazing fast reads**: Feed lookup is $O(1)$ (`LRANGE timeline:user_id 0 20`). | **Celebrity Problem**: A user with 50M followers triggers 50 million Redis writes on a single tweet! |

### 3. The Production Solution (Hybrid Feed Architecture):

```
Regular Users (99%):
User posts ──► Worker fans out tweet ID to followers' Redis Timelines (Push).

Celebrity Users (Justin Bieber / Elon Musk):
Celebrity posts ──► Tweet saved in DB only (NO push fanout).
Follower opens feed ──► Reads cached timeline + merges celebrity tweets dynamically (Pull).
```

---

## 7. "Say This, Not That" in System Design Interviews

| Instead of saying... ❌ | Say this like a senior candidate... ✅ |
| :--- | :--- |
| *"I'll just throw everything into MongoDB."* | *"Given our strict transaction requirements and relational integrity between orders and payments, PostgreSQL is the proper choice for our core ledger."* |
| *"We will shard the database on day one."* | *"Given the read-heavy nature, sharding introduces unnecessary cross-shard join complexity. I would first add read replicas and a Redis cache, and only shard when write volume or storage exceeds a single machine."* |
| *"We can store user images directly in the database."* | *"Storing binary blobs in the database pollutes the buffer pool and slows backups. I will upload images directly to AWS S3 via pre-signed URLs and store only the URL metadata in the database."* |
| *"I'll use HTTP polling for chat."* | *"HTTP polling creates unnecessary overhead and latency. I will use persistent WebSockets for real-time bidirectional message delivery, falling back to push notifications when the user is offline."* |
| *"My distributed database is CA (Consistent & Available)."* | *"Since network partitions are inevitable in real-world distributed systems, we must choose between CP and AP during a partition. For this banking balance, we prioritize Consistency (CP) and reject requests if consensus cannot be reached."* |

---

## 8. What You Can Safely Skip for Internship Interviews

Do not let yourself get overwhelmed by senior/staff-level distributed systems topics. For internship interviews, you do **not** need to dive deep into:

| Topic | What You Need to Know (Internship Level) | What You Can Safely Skip (Senior/Staff Level) |
| :--- | :--- | :--- |
| **Kubernetes & Docker** | Know Docker creates isolated containers and K8s orchestrates them. | Pod networking (CNI), sidecars, K8s control plane, Helm charts. |
| **Microservices & Mesh** | Know microservices split monoliths by business domain. | Service meshes (Istio, Envoy), mutual TLS (mTLS), distributed tracing. |
| **Distributed Consensus** | Know databases use consensus (Quorum) to elect leaders. | Raft or Paxos log replication, leader election state machines. |
| **Distributed Transactions** | Know 2PC and Saga conceptually (compensating actions). | XA transactions, TCC (Try-Confirm-Cancel), distributed locks internals. |
| **Kafka Internals** | Know Kafka is an append-only, partitioned distributed commit log. | In-Sync Replicas (ISR), segment files, log compaction, controller election. |
| **Database Engines** | Know B+ Tree is read-optimized, LSM-Tree is write-optimized. | Buffer pool replacement algorithms (2Q/LRU-K), WAL checkpoint algorithms. |
| **Multi-Region Systems** | Know CDNs cache data at edge locations close to users. | Active-Active multi-region database conflicts, CRDTs, global geo-routing. |
| **Observability** | Know the basics of P95/P99 latency, CPU metrics, and slow query logs. | OpenTelemetry collectors, distributed trace sampling, flame graphs. |

> **The Golden Interview Test**:
> If you can comfortably explain:
> **"What data do we have?" ──► "How do we store it?" ──► "How do we query it?" ──► "What happens when traffic grows?" ──► "What component do we add, and why?"**
> ...you will easily pass any internship system design interview!
