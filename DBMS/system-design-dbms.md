# System Design & DBMS: The Core Interview Master Sheet

> **Target Audience:** Engineering Internship & Junior/New-Grad Technical Interviews (FAANG, Tier-1 Tech, FinTech, High-Growth Startups).  
> **Focus:** The high-yield intersection of **Database Management Systems (DBMS)** and **Distributed System Design** (Scaling, Caching, Sharding, Replication, Consistency, Indexing, and Async Queues).

---

# Table of Contents
1. [Data Partitioning & Sharding](#1-data-partitioning--sharding)
   - [Horizontal vs. Vertical Sharding](#11-horizontal-vs-vertical-sharding)
   - [Sharding Strategies & Consistent Hashing](#12-sharding-strategies--consistent-hashing)
   - [Distributed Sharding Challenges (Cross-Shard Joins, Hotspots & Resharding)](#13-distributed-sharding-challenges)
2. [Replication, Consistency & High Availability](#2-replication-consistency--high-availability)
   - [Replication Topologies (Primary-Replica vs. Multi-Leader vs. Leaderless)](#21-replication-topologies)
   - [Replication Lag & Read-Your-Own-Writes Consistency](#22-replication-lag--consistency-guarantees)
   - [CAP & PACELC Theorems in Practice](#23-cap--pacelc-theorems-in-practice)
3. [Caching Architecture & CDNs](#3-caching-architecture--cdns)
   - [Cache Strategies (Cache-Aside, Write-Through, Write-Behind, Write-Around)](#31-caching-patterns)
   - [Cache Eviction & Classic Invalidation Pitfalls](#32-cache-invalidation-pitfalls--mitigations)
   - [CDNs & Edge Caching](#33-content-delivery-networks-cdns)
4. [Storage Engines, Indexes & Data Structures](#4-storage-engines-indexes--data-structures)
   - [B+ Tree (OLTP / Read-Heavy) vs. LSM-Tree / SSTables (Write-Heavy)](#41-b-tree-vs-lsm-tree--sstable)
   - [Index Design: Clustered, Secondary, Covering & Composite Indexes](#42-index-design--the-leftmost-prefix-rule)
5. [Transactions, Concurrency & Distributed Consistency](#5-transactions-concurrency--distributed-consistency)
   - [Isolation Levels & Concurrency Anomalies](#51-isolation-levels--concurrency-anomalies)
   - [Pessimistic vs. Optimistic Concurrency Control (OCC)](#52-pessimistic-vs-optimistic-concurrency-control)
   - [Distributed Transactions: 2-Phase Commit (2PC) vs. Sagas](#53-distributed-transactions-2pc-vs-sagas)
6. [SQL vs. NoSQL Selection Matrix](#6-sql-vs-nosql-selection-matrix)
7. [Asynchronous Decoupling & Dual-Write Mitigation](#7-asynchronous-decoupling--dual-write-mitigation)
   - [Transactional Outbox Pattern & CDC](#71-transactional-outbox-pattern--change-data-capture)
   - [Idempotency & Message Delivery Guarantees](#72-idempotency--message-delivery-semantics)
8. [Last-Day Revision Sheet](#8-last-day-revision-sheet)

---

# 1. Data Partitioning & Sharding

### 1.1 Horizontal vs. Vertical Sharding

```
+-----------------------------------------------------------------------------------------+
|                                 VERTICAL PARTITIONING                                   |
| (Split by Domain / Tables / Columns)                                                    |
|                                                                                         |
|       +---------------------------+             +---------------------------+           |
|       |     Users DB (Host A)     |             |    Orders DB (Host B)     |           |
|       |  user_id, email, password |             |  order_id, user_id, total |           |
|       +---------------------------+             +---------------------------+           |
+-----------------------------------------------------------------------------------------+
|                                HORIZONTAL SHARDING                                      |
| (Split Rows across identical schema nodes)                                              |
|                                                                                         |
|   +--------------------------+  +--------------------------+  +---------------------+   |
|   |     Shard 1 (Host A)     |  |     Shard 2 (Host B)     |  |  Shard 3 (Host C)   |   |
|   | Users: IDs 1 -> 1,000,000|  | Users: IDs 1M -> 2,000,000|  | Users: IDs 2M -> 3M |   |
|   +--------------------------+  +--------------------------+  +---------------------+   |
+-----------------------------------------------------------------------------------------+
```

| Dimension | Vertical Partitioning | Horizontal Partitioning (Sharding) |
| :--- | :--- | :--- |
| **How it works** | Divides distinct tables or wide column subsets to separate physical DB instances. | Divides rows of the *same* table across multiple database servers using a **Shard Key**. |
| **When to use** | Early scaling phase; separating high-traffic tables (e.g. `UserAuth` vs `BillingLogs`). | When a single table outgrows a single machine's RAM/Disk (> 1TB or > 50k writes/sec). |
| **Limitation** | Reaches a hard ceiling once a single vertical table outgrows server capacity. | Complex application routing, cross-shard queries, distributed transactions. |

---

### 1.2 Sharding Strategies & Consistent Hashing

#### Sharding Routing Mechanisms
1. **Range-Based Sharding:** Shard by key ranges (e.g., `user_id` 1–1M $\to$ Shard 1, 1M–2M $\to$ Shard 2; or by date `2023-01` $\to$ Shard 1).
   - *Pros:* Simple; efficient range scans (`BETWEEN X AND Y`).
   - *Cons:* Severe write hotspots on the latest range (e.g. current date shard takes 100% of writes).
2. **Hash-Based Sharding:** $\text{Shard ID} = \text{hash}(\text{key}) \pmod N$.
   - *Pros:* Uniformly distributes reads and writes across nodes.
   - *Cons:* Range scans require querying every single shard (**scatter-gather**); adding/removing a node requires rehashing almost 100% of data.
3. **Directory-Based / Lookup Sharding:** Central mapping service (e.g. ZooKeeper/Redis) tracks which key maps to which shard.
   - *Pros:* Total flexibility to rebalance specific keys.
   - *Cons:* Lookup service becomes a single point of failure (SPOF) and read latency bottleneck.

---

#### Consistent Hashing (The Industry Standard)

```
                            [Node A (Pos: 20)]
                                  /     \
                                 /       \
                 (Key 1: 15) -> O         O <- (Key 2: 85)
                               /           \
                 [Node D (Pos: 280)]      [Node B (Pos: 110)]
                               \           /
                 (Key 4: 220) -> O        O <- (Key 3: 160)
                                 \       /
                            [Node C (Pos: 200)]

   * Request Key hashes to integer in range [0, 2^32 - 1].
   * Traverses clockwise until it encounters the first Node on the ring.
   * Virtual Nodes (e.g. A_1, A_2, A_3) prevent data skew across heterogeneous hardware.
```

- **How It Solves Rebalancing:** When a node is added or removed, only $K/N$ keys need migration (where $K = \text{total keys}$, $N = \text{number of nodes}$), rather than all keys.
- **Virtual Nodes:** Each physical node is assigned 100–300 positions across the ring to prevent uneven data clustering and hot spots.

---

### 1.3 Distributed Sharding Challenges

1. **Cross-Shard Joins:** Querying `Users` on Shard 1 and `Orders` on Shard 2 is computationally prohibitive over the network.
   - *Mitigation:* Denormalize data, duplicate small lookup tables to every shard (**Broadcast Tables**), or co-locate related records using a shared partition key (e.g., sharding both `Users` and `Orders` by `user_id`).
2. **The Celebrity / Hotspot Problem:** A viral creator (e.g. Elon Musk with 100M followers) creates massive write/read traffic on a single partition key.
   - *Mitigation:* Append random salt to the key (`user_id + "_" + rand(0, 10)`) to distribute writes across 10 shards, then scatter-gather reads.
3. **Distributed ID Generation:** Auto-incrementing primary keys collide across independent shard nodes.
   - *Mitigation:* **Twitter Snowflake ID** (64-bit: 41 bits timestamp + 10 bits machine/datacenter ID + 12 bits sequence number) or UUIDv7 (time-ordered UUIDs).

---

# 2. Replication, Consistency & High Availability

### 2.1 Replication Topologies

```
+-----------------------------------------------------------------------------------------+
| PRIMARY-REPLICA (Single Leader)       MULTI-LEADER (Active-Active)      LEADERLESS      |
|                                                                                         |
|       [Primary (Writes)]              [Master A] <--> [Master B]        Client Writes/  |
|         /            \                (Writes)         (Writes)         Reads directly  |
|        v              v                  |                |             to all N nodes  |
|  [Replica 1]    [Replica 2]              v                v             via Quorum:     |
|   (Reads)         (Reads)            [Replica]        [Replica]          W + R > N      |
+-----------------------------------------------------------------------------------------+
```

| Topology | Write Path | Read Path | Pros | Cons / Gotchas |
| :--- | :--- | :--- | :--- | :--- |
| **Primary-Replica (Leader-Follower)** | All writes go to single Primary node. | Reads served by multiple Replicas. | Simple; no write conflicts; scales read throughput. | Replication lag causes stale reads; Primary is write bottleneck/SPOF during failover. |
| **Multi-Leader (Multi-Master)** | Writes accepted at any regional Leader. | Reads served locally per datacenter. | Survives datacenter outages; low write latency globally. | **Write Conflict Resolution** required (Last-Write-Wins clock skew, CRDTs). |
| **Leaderless (Dynamo-Style / Cassandra)** | Client writes to $W$ nodes directly. | Client reads from $R$ nodes directly. | Ultra-high write availability; no failover needed. | Weak consistency unless Quorum $W + R > N$ is strictly maintained. |

---

### 2.2 Replication Lag & Consistency Guarantees

When reading from asynchronous read replicas, **replication lag** causes data anomalies:

```
User updates profile ("Name: Alice -> Bob")
      |
      v (Write)
 [Primary DB] ---> (Async Replication Lag: 800ms) ---> [Read Replica]
      |                                                      ^
      +----------------- User refreshes page ----------------+
                         (Reads from Replica -> Sees "Alice"!)
```

#### Essential Consistency Models
1. **Read-Your-Own-Writes Consistency (Causal Consistency):** Guarantees that if a user updates data, their subsequent reads will reflect that update.
   - *Implementation:* Route reads for user $X$ to the Primary DB for 5–10 seconds following any write, while routing other users' reads to replicas.
2. **Monotonic Reads:** Guarantees that if a user sees a value at time $t_1$, they will never see an older state at time $t_2$.
   - *Implementation:* Pin each user session to the same specific replica instance via sticky hash routing.
3. **Quorum Consistency Equation:**
   $$\mathbf{W + R > N}$$
   Where $N = \text{Replication Factor}$, $W = \text{Write Ack Count}$, $R = \text{Read Query Count}$.
   - If $W + R > N$, at least one node in the read set is guaranteed to contain the latest write.
   - Example ($N=3, W=2, R=2$): Strict consistency.
   - Example ($N=3, W=1, R=1$): Eventual consistency, low latency, high availability.

---

### 2.3 CAP & PACELC Theorems in Practice

#### CAP Theorem
In any asynchronous distributed network subject to network partitions (**P**):
- **Choose Consistency (CP):** Return an error or timeout if nodes cannot reach quorum consensus (e.g. Google Spanner, HBase, ZooKeeper, Redis Sentinel).
- **Choose Availability (AP):** Return local (potentially stale) data immediately (e.g. Cassandra, DynamoDB, CouchDB).

#### PACELC Theorem (The Complete Real-World Model)
$$\text{If } \mathbf{P} \text{ (Partition): Choose } \mathbf{A} \lor \mathbf{C} \quad \Big| \quad \mathbf{E} \text{ (Else / Normal Operation): Choose } \mathbf{L} \text{ (Latency)} \lor \mathbf{C} \text{ (Consistency)}$$

- **PC/EC (e.g. Spanner, RDBMS with 2PC):** Prioritizes consistency at all times; pays higher latency cost during normal operations.
- **PA/EL (e.g. Cassandra, DynamoDB):** Sacrifices consistency during partitions for uptime; optimizes for lowest latency during normal operations.

---

# 3. Caching Architecture & CDNs

### 3.1 Caching Patterns

```
1. CACHE-ASIDE (Lazy Loading)           2. WRITE-THROUGH
   App reads Cache -> Miss -> DB           App writes to Cache ->
   App writes to Cache                     Cache synchronously writes to DB

3. WRITE-BEHIND (Write-Back)            4. WRITE-AROUND
   App writes to Cache -> Ack              App writes directly to DB ->
   Async worker batches DB writes          Cache is populated only on Read Miss
```

| Pattern | Write Flow | Read Flow | Pros | Cons / Gotchas |
| :--- | :--- | :--- | :--- | :--- |
| **Cache-Aside (Lazy Loading)** | App writes to DB, then **invalidates (evicts)** key in Cache. | App checks Cache; on miss, queries DB and populates Cache. | Resilient (cache crash doesn't halt writes); only requested data is cached. | Read penalty on cache miss; potential stale data if eviction fails. |
| **Write-Through** | App writes to Cache; Cache synchronously writes to DB before acking. | App reads from Cache. | Cache is never stale; simplified client code. | High write latency (waits for both cache + disk DB write). |
| **Write-Behind (Write-Back)** | App writes to Cache; Cache acks immediately; async daemon flushes to DB. | App reads from Cache. | Ultra-low write latency; absorbs massive write spikes. | Risk of data loss if cache node crashes before flushing dirty writes to DB. |
| **Write-Around** | App writes directly to DB, bypassing cache completely. | App reads from Cache; on miss, loads from DB. | Prevents cache pollution from write-heavy data that is rarely read again. | First read after write is guaranteed cache miss (higher initial latency). |

> **Golden Rule for Cache Updates:** In Cache-Aside, **always delete the cache entry on write**, do not update it (`cache.delete(key)` vs. `cache.set(key, val)`). Updating the cache introduces race conditions between concurrent writes.

---

### 3.2 Cache Invalidation Pitfalls & Mitigations

```
+-----------------------------------------------------------------------------------------+
| PITFALL 1: CACHE AVALANCHE            PITFALL 2: CACHE BREAKDOWN (STAMPEDE)             |
|                                                                                         |
|  Many keys expire simultaneously        Single hot key expires -> Thousands of         |
|  at midnight -> DB overwhelmed!         concurrent requests hit DB simultaneously!      |
|                                                                                         |
|  [Fix: Add Random Jitter to TTL]        [Fix: Mutex Lock / Singleflight / Pre-Warm]     |
|   TTL = Base_TTL + rand(0, 300s)         acquire_lock() -> query DB -> update cache     |
+-----------------------------------------------------------------------------------------+
| PITFALL 3: CACHE PENETRATION                                                            |
|                                                                                         |
|  Attacker queries non-existent ID (id: -9999) -> Bypasses Cache -> DB queried every time!|
|                                                                                         |
|  [Fix: Bloom Filter at Cache Layer OR Cache NULL with short 60-second TTL]              |
+-----------------------------------------------------------------------------------------+
```

---

### 3.3 Content Delivery Networks (CDNs)

- **Push CDN:** Content uploaded directly from server to CDN edge servers (best for small, infrequent release builds, static assets, software patches).
- **Pull CDN (Origin Pull):** Edge server fetches asset from origin server on first client request, then caches it (standard for media streaming, e-commerce product photos).
- **Crucial HTTP Headers:**
  - `Cache-Control: public, max-age=31536000, immutable` (for static hashed assets `app.8f92a.js`).
  - `Cache-Control: s-maxage=3600, stale-while-revalidate=60` (tells CDN to serve stale asset while fetching updated version asynchronously in the background).

---

# 4. Storage Engines, Indexes & Data Structures

### 4.1 B+ Tree vs. LSM-Tree / SSTable

```
B+ TREE (In-Place Updates)                     LSM-TREE (Append-Only Log + SSTables)
--------------------------                     -------------------------------------
            [ Root Page ]                               Incoming Writes
               /     \                                         |
        [ Page A ]  [ Page B ]                                 v
          /    \      /    \                          +------------------+
       [Leaf] [Leaf][Leaf] [Leaf] <-> [Leaf]          | MemTable (RAM)   | -> (Flushes to disk)
       (Doubly linked for fast range scans)           +------------------+
                                                               |
Random I/O: Must modify pages in-place.               +------------------+
Ideal for: Read-heavy OLTP (PostgreSQL, MySQL).       | SSTable Level 0  | (Immutable sorted files)
                                                      | SSTable Level 1  | (Merged via Compaction)
                                                      +------------------+
                                                      Sequential I/O: Append-only write.
                                                      Ideal for: Write-heavy (Cassandra, RocksDB).
```

| Dimension | B+ Tree | LSM-Tree (Log-Structured Merge-Tree) |
| :--- | :--- | :--- |
| **Primary Storage Medium** | In-place mutable disk pages (e.g. 8KB/16KB). | In-memory `MemTable` + immutable on-disk `SSTables`. |
| **Write Performance** | Slower (Random I/O; page splits, dirty buffer pool flushing). | **Extremely Fast** (Sequential append to WAL + MemTable). |
| **Read Performance** | **Fast ($O(\log N)$)** via direct pointer lookups. | Slower (must check MemTable $\to$ Bloom filters $\to$ multiple SSTable levels). |
| **Compaction Overhead** | None (fragmentation managed via page fill factors). | High (background Compaction consumes CPU/disk I/O). |
| **Real-World Examples** | PostgreSQL, MySQL (InnoDB), SQLite, Oracle. | Apache Cassandra, RocksDB, ScyllaDB, LevelDB, InfluxDB. |

---

### 4.2 Index Design & The Leftmost Prefix Rule

#### Index Types
1. **Clustered Index:** Defines the physical on-disk storage order of table rows. Exactly **one** clustered index per table (typically Primary Key in InnoDB).
2. **Secondary Index:** Auxiliary B+ tree where leaf nodes store the **Primary Key** (not physical row pointers) to avoid pointer corruption during page splits.
3. **Covering Index:** An index that contains all columns requested by a `SELECT` query, eliminating the need to perform an expensive **Index Lookup / Bookmark Lookup** to the base table.

#### Composite Index: The Leftmost Prefix Rule
Given an index on `(department_id, salary, created_at)`:
- `WHERE department_id = 5 AND salary > 50000` $\to$ **USES INDEX (Optimal)**
- `WHERE department_id = 5` $\to$ **USES INDEX**
- `WHERE salary > 50000 AND created_at > '2023-01-01'` $\to$ **FULL TABLE SCAN (Violates Leftmost Rule)**
- `WHERE department_id = 5 AND created_at > '2023-01-01'` $\to$ **PARTIAL INDEX USE (Filters on `department_id`, cannot use index for `created_at`)**

> **Rule of Thumb:** Place equality filter columns first in composite index declarations, followed by range filter (`<, >, BETWEEN`) columns last.

---

# 5. Transactions, Concurrency & Distributed Consistency

### 5.1 Isolation Levels & Concurrency Anomalies

$$\text{Read Uncommitted} \longrightarrow \text{Read Committed} \longrightarrow \text{Repeatable Read} \longrightarrow \text{Serializable}$$

| Isolation Level | Dirty Read | Non-Repeatable Read | Phantom Read | Mechanism / Real-World Default |
| :--- | :---: | :---: | :---: | :--- |
| **Read Uncommitted** | ❌ Yes | ❌ Yes | ❌ Yes | Reads raw dirty memory buffers; almost never used in production. |
| **Read Committed** | ✅ Prevented | ❌ Yes | ❌ Yes | Reads latest committed snapshot per statement (**PostgreSQL default**). |
| **Repeatable Read** | ✅ Prevented | ✅ Prevented | ❌ Yes* | Snapshot established at start of transaction (**MySQL InnoDB default** via MVCC + Next-Key Locks). |
| **Serializable** | ✅ Prevented | ✅ Prevented | ✅ Prevented | Strict ordering via Two-Phase Locking (2PL) or Serializable Snapshot Isolation (SSI). |

#### Concurrency Anomalies Defined
- **Dirty Read:** Txn B reads uncommitted data written by Txn A. If Txn A rolls back, Txn B holds garbage data.
- **Non-Repeatable Read:** Txn A reads a row value. Txn B modifies and commits that row. Txn A re-reads the row and sees a *different value*.
- **Phantom Read:** Txn A reads a range of rows (`WHERE age > 30` returns 5 rows). Txn B inserts a new row with `age = 35` and commits. Txn A re-reads the range and sees 6 rows.
- **Write Skew:** Two transactions concurrently read overlapping state, make non-conflicting individual writes, but violate a global business invariant (e.g. two doctors concurrently take on-call leave, leaving 0 doctors on duty). Prevented only by **Serializable** or explicit locking (`SELECT FOR UPDATE`).

---

### 5.2 Pessimistic vs. Optimistic Concurrency Control

```sql
-- 1. PESSIMISTIC LOCKING (Lock row on read; blocks concurrent transactions)
-- Use when: High write contention, frequent conflicts (e.g. Flash Sales, Ticket Booking)
BEGIN;
SELECT available_seats 
FROM flight_seats 
WHERE seat_id = 'A12' 
FOR UPDATE; -- Acquires exclusive row lock

UPDATE flight_seats SET is_booked = TRUE WHERE seat_id = 'A12';
COMMIT;

-- 2. OPTIMISTIC CONCURRENCY CONTROL (OCC via Versioning / CAS)
-- Use when: Low write contention, read-heavy workloads (e.g. Wikipedia editing)
SELECT balance, version FROM accounts WHERE account_id = 42;
-- (Application calculates: new_balance = balance - 100)

UPDATE accounts 
SET balance = 900, version = version + 1
WHERE account_id = 42 AND version = 5;

-- If rows_affected == 0: Another transaction committed first! Abort & Retry in application.
```

---

### 5.3 Distributed Transactions: 2PC vs. Sagas

```
TWO-PHASE COMMIT (2PC)                 SAGA PATTERN (Compensating Transactions)
----------------------                 ----------------------------------------
Coordinator -> Prepare -> All Nodes    Step 1: Order Service -> Create Order (Pending)
Nodes reply -> "Prepared"              Step 2: Payment Service -> Charge Card (Failed!)
Coordinator -> Commit -> All Nodes     Step 3: (Compensating) Order Service -> Cancel Order

* Synchronous blocking locks.          * Asynchronous eventual consistency.
* Single point of failure.             * No cross-service database locks.
* High latency / poor cloud scale.     * Standard for Microservices / FinTech.
```

- **Saga Orchestration:** Central orchestrator service sends commands to participants and triggers rollback compensating events on failure.
- **Saga Choreography:** Services publish and listen to domain events via message brokers (Kafka/RabbitMQ) without a centralized coordinator.

---

# 6. SQL vs. NoSQL Selection Matrix

```
                          [ DATA STORAGE SELECTION ]
                                      |
                +---------------------+---------------------+
                |                                           |
         Relational Data?                           Unstructured /
     ACID & Complex Joins?                          Massive Volume?
                |                                           |
               YES                                         YES
                v                                           v
       [ Relational SQL ]                          [ NoSQL Families ]
    (PostgreSQL, MySQL, Spanner)                            |
                                    +-----------+-----------+-----------+
                                    |           |           |           |
                                    v           v           v           v
                              Key-Value      Document   Wide-Column   Graph
                              (Redis)       (MongoDB)   (Cassandra)   (Neo4j)
```

| Database Family | Leading Technologies | Best Used For | Avoid When |
| :--- | :--- | :--- | :--- |
| **Relational (RDBMS)** | PostgreSQL, MySQL, CockroachDB | Financial ledgers, ACID constraints, complex foreign key relations. | Unstructured data, extreme horizontal write scale (> 100k writes/sec single table). |
| **Key-Value** | Redis, AWS DynamoDB, Memcached | Session storage, rate limiting, leaderboards, distributed locks. | Complex multi-attribute querying, ad-hoc aggregations. |
| **Document Store** | MongoDB, Couchbase | Product catalogs, dynamic schema forms, hierarchical JSON payloads. | Highly normalized relational schemas requiring multi-table joins. |
| **Wide-Column** | Apache Cassandra, ScyllaDB, HBase | Time-series telemetry, IoT sensor logs, massive write ingestion. | Complex range queries on non-partition keys, ad-hoc relational joins. |
| **Graph DB** | Neo4j, Amazon Neptune | Social networks (followers/friends), fraud ring detection, knowledge graphs. | Tabular reporting, high-throughput bulk transactional logging. |

---

# 7. Asynchronous Decoupling & Dual-Write Mitigation

### 7.1 Transactional Outbox Pattern & Change Data Capture (CDC)

#### The Dual-Write Bug
Writing to the Database and publishing to Kafka/Cache in a single HTTP request handler inevitably fails:
```
1. db.insert(order)  -> SUCCESS
2. kafka.send(event) -> NETWORK CRASH!
Result: Order exists in DB, but downstream Payment & Inventory services never receive the event!
```

#### The Transactional Outbox Solution

```
App Service                                  DB Instance (Single Local ACID Transaction)
    |                                            |
    +--- (1) BEGIN TRANSACTION ----------------->|
    |    (2) INSERT INTO orders (...) ---------->|
    |    (3) INSERT INTO outbox_table (...) ---->| (Payload: JSON event)
    +--- (4) COMMIT ---------------------------->|
                                                 |
                                         [ outbox_table ]
                                                 |
                   +-----------------------------+
                   v (Read via CDC Engine / Polling Daemon)
         [ Debezium / Kafka Connect ] 
                   |
                   v (Guaranteed At-Least-Once Delivery)
         [ Kafka Event Topic ] ---> [ Downstream Consumers ]
```

- **Why It Works:** Writing the business entity (`orders`) and the event (`outbox_table`) occurs within a **single local ACID transaction**, eliminating distributed inconsistency without 2PC locks.
- **Change Data Capture (CDC):** Tools like Debezium tail the database's binary transaction log (e.g. Postgres WAL / MySQL binlog) and stream outbox events directly to Kafka.

---

### 7.2 Idempotency & Message Delivery Semantics

- **At-Most-Once:** Message sent once; never retried. Risk of message loss.
- **At-Least-Once (Standard):** Message retried until acknowledged. Risk of duplicate delivery.
- **Idempotent Consumer Pattern:** Because message brokers deliver duplicate messages during network blips, consumers must enforce idempotency:

```sql
-- Client generates unique idempotency_key (UUIDv4) sent in API request header
INSERT INTO processed_requests (idempotency_key, response_payload, created_at)
VALUES ('idem_9941_a8f2', '{"status": "charged"}', NOW())
ON CONFLICT (idempotency_key) DO NOTHING;

-- If rows_affected == 0: Return previously cached response_payload without re-charging card!
```

---

# 8. Last-Day Revision Sheet

## 1. System Design Numbers Every Engineer Must Know

| Operation | Latency Benchmark | Intuition |
| :--- | :--- | :--- |
| **L1 / L2 CPU Cache** | 0.5 – 7 ns | Instantaneous |
| **Main Memory (RAM) Reference** | ~100 ns | Extremely fast; baseline for Redis/Memcached |
| **Read 1 MB sequentially from RAM** | ~3,000 ns (3 µs) | In-memory stream processing |
| **Read 1 MB sequentially from NVMe SSD** | ~50,000 ns (50 µs) | Fast sequential disk I/O (LSM Compaction) |
| **Read 1 MB sequentially from 1Gbps Network** | ~10,000,000 ns (10 ms) | Network is 3,000x slower than RAM |
| **Round Trip inside same Datacenter** | ~0.5 ms | Intra-datacenter RPC |
| **Cross-Continent Round Trip (US $\to$ EU)** | ~150 ms | Global network latency; justifies Edge CDNs |

---

## 2. System Design + DBMS Architecture Decision Tree

```
1. Write Throughput is > 50,000 writes/sec?
   ├── YES ──> Use LSM-Tree Store (Cassandra, RocksDB) + Kafka buffer.
   └── NO  ──> Use Relational RDBMS (Postgres, MySQL) with connection pooling.

2. Read Traffic is > 10x Write Traffic?
   ├── YES ──> Implement Cache-Aside (Redis) + Read Replicas + CDN for static assets.
   └── NO  ──> Focus on Write-Through / Write-Behind or Wide-Column sharding.

3. Need Strict ACID on multi-table transactions?
   ├── Single Shard / Moderate Scale ──> PostgreSQL / MySQL with Read Committed / Repeatable Read.
   ├── Global Distributed Scale     ──> Google Cloud Spanner / CockroachDB (TrueTime / Raft consensus).
   └── Microservices Boundary       ──> Sagas Pattern with Idempotent Outbox Consumers.

4. Data has dynamic, unpredictable schema?
   ├── YES ──> MongoDB / PostgreSQL with JSONB indexing.
   └── NO  ──> Normalized Relational Schema with B+ Tree Indexes.
```

---

## 3. High-Frequency Interview Problem $\to$ Pattern Cheat Sheet

| Interview Design Problem | Core DBMS & Scaling Patterns Applied |
| :--- | :--- |
| **Design a URL Shortener (TinyURL / Bitly)** | Base62 encoding on Snowflake 64-bit ID, NoSQL Key-Value store, Cache-Aside for hot URLs (LRU). |
| **Design a Rate Limiter** | Redis `INCR` + `EXPIRE` (Token Bucket / Sliding Window Log in Redis Sorted Set `ZADD`). |
| **Design Instagram / Twitter Newsfeed** | **Fan-out on Write** for standard users (push to Redis timeline lists); **Fan-out on Read** for celebrities (merge timeline at query time). |
| **Design Ticketmaster / Flash Sale** | Pessimistic locking (`SELECT FOR UPDATE`) or Redis Distributed Lock (Redlock) + Kafka queue to serialize booking requests. |
| **Design E-Commerce Checkout** | Transactional Outbox Pattern + CDC to Kafka, Idempotency Keys on Payment Gateway, Sagas for order-inventory-payment choreography. |
| **Design Distributed Web Crawler** | Bloom Filters for URL deduplication, Consistent Hashing to allocate URL queues across worker nodes, DNS caching. |
| **Design Real-Time Chat (WhatsApp / Slack)** | WebSocket connections, Cassandra for message history (partition by `conversation_id`, cluster by `timestamp`), Redis Pub/Sub for presence. |
