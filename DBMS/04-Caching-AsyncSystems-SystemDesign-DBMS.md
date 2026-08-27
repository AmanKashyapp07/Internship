# Master Guide 04: Caching, Async Systems & System Design Database Architecture

> **Focus:** Caching Strategies & Catastrophes (Penetration, Avalanche, Stampede), Redis Data Structure Internals (ZipList, SkipList, IntSet), Dual-Write Race Conditions, L4/L7 Load Balancing, Rate Limiting, Kafka vs RabbitMQ (Zero-Copy Architecture), and System Design Database Selection.
> 
> *Designed to crush top-tier FAANG/FinTech database & system design interviews.*

---

# Table of Contents
1. [Stateless vs. Stateful Architecture](#1-stateless-vs-stateful-architecture)
2. [Caching Patterns & The 3 Production Catastrophes](#2-caching-patterns--catastrophes)
3. [The Database Dual-Write Problem & Race Conditions](#3-the-database-dual-write-problem--race-conditions)
4. [Redis vs. Memcached & Redis Internal Data Structures](#4-redis-vs-memcached--redis-internal-data-structures)
5. [Traffic Routing & Rate Limiting Algorithms](#5-traffic-routing--rate-limiting-algorithms)
6. [Asynchronous Messaging: Kafka vs. RabbitMQ](#6-asynchronous-messaging-kafka-vs-rabbitmq)
7. [Why Kafka is Blazing Fast (Zero-Copy Architecture)](#7-why-kafka-is-blazing-fast-zero-copy-architecture)
8. [System Design Database Selection Framework](#8-system-design-database-selection-framework)
9. [Production System Design Case Studies](#9-production-system-design-case-studies)
10. [Interview Quick-Fire Verbal Q&A](#10-interview-quick-fire-verbal-qa)

---

# 1. Stateless vs. Stateful Architecture

```
+-----------------------------------------------------------------------------------------+
| DIMENSION           | STATELESS ARCHITECTURE                 | STATEFUL ARCHITECTURE    |
+-----------------------------------------------------------------------------------------+
| Server Memory       | Holds **ZERO client session state**    | Retains client session in|
|                     | across requests.                       | local server RAM/disk.   |
+-----------------------------------------------------------------------------------------+
| Horizontal Scaling  | **Trivially Simple**: Spin up 50 nodes | **Complex**: Requires    |
|                     | behind a standard Round-Robin LB.      | Sticky Sessions or state |
|                     |                                        | replication across nodes.|
+-----------------------------------------------------------------------------------------+
| Failure Impact      | Zero impact: Any node handles any req. | High: Active sessions    |
|                     | State offloaded to shared Redis store. | lost if node crashes.    |
+-----------------------------------------------------------------------------------------+
```

---

# 2. Caching Patterns & The 3 Production Catastrophes

### A. Caching Design Patterns

```
+-----------------------------------------------------------------------------------------+
| PATTERN             | FLOW / MECHANICS                       | SPOKEN TRADEOFF          |
+-----------------------------------------------------------------------------------------+
| Cache-Aside         | App reads Cache. On miss: App reads DB,| **Pros:** Cache holds    |
| (Lazy Loading)      | writes to Cache, returns data.         | only requested keys.     |
|                     | Writes update DB & invalidate Cache.   | **Cons:** Cache miss lag.|
+-----------------------------------------------------------------------------------------+
| Write-Through       | App writes to Cache; Cache             | **Pros:** Zero stale data|
|                     | synchronously writes to DB before OK.  | **Cons:** High write lag.|
+-----------------------------------------------------------------------------------------+
| Write-Back          | App writes to Cache only. Cache batches| **Pros:** Ultra-fast write|
| (Write-Behind)      | and asynchronously flushes to DB.      | **Cons:** Data loss risk |
|                     |                                        | if cache crashes!        |
+-----------------------------------------------------------------------------------------+
```

### B. The 3 Major Cache Catastrophes & Spoken Mitigations

```
+-----------------------------------------------------------------------------------------+
| CATASTROPHE         | ROOT CAUSE                             | SPOKEN MITIGATION        |
+-----------------------------------------------------------------------------------------+
| Cache Penetration   | Queries for non-existent keys (e.g.    | 1. **Bloom Filter** in   |
|                     | `user_id = -999`) bypass cache and hit | front of cache (O(1)).   |
|                     | the database directly.                 | 2. **Cache Null Values** |
|                     |                                        | with short 60s TTL.      |
+-----------------------------------------------------------------------------------------+
| Cache Avalanche     | Millions of keys expire at the EXACT   | **Add TTL Jitter**:      |
|                     | SAME SECOND, flooding the DB at once.  | `TTL = base + rand(0,300)`|
+-----------------------------------------------------------------------------------------+
| Cache Stampede      | A heavily accessed "Hot Key" expires;  | 1. **Distributed Mutex** |
| (Thundering Herd)   | 10,000 concurrent threads experience a | (`SETNX` in Redis).      |
|                     | miss and execute heavy query at DB.    | 2. **XFetch early refresh|
|                     |                                        | in background.           |
+-----------------------------------------------------------------------------------------+
```

---

# 3. The Database Dual-Write Problem & Race Conditions

### The Dual-Write Race Condition Trap
When updating both a Database and a Cache concurrently:

```text
Thread 1 (Write $100): Writes DB ($100) ------------------------> Invalidates Cache
Thread 2 (Write $200): --------------> Writes DB ($200) -> Invalidates Cache
```
If Thread 1 is delayed, Thread 2's update can be overwritten by Thread 1's stale write, leaving stale data in the cache permanently!

### The Production Fix: Delete/Invalidate Cache AFTER DB Commit
1. Always update the Database first.
2. Invalidate (delete) the Cache key **after the DB transaction successfully commits**.
3. Use **CDC (Debezium + Kafka)** to listen to DB WAL logs and asynchronously invalidate Redis cache keys to achieve eventual consistency.

---

# 4. Redis vs. Memcached & Redis Internal Data Structures

```
+-----------------------------------------------------------------------------------------+
| DIMENSION           | REDIS                                  | MEMCACHED                |
+-----------------------------------------------------------------------------------------+
| Architecture        | Single-Threaded Event Loop (epoll)     | Multi-Threaded Memory    |
|                     | with background I/O threads.           | Allocator (Slab Allocator)|
+-----------------------------------------------------------------------------------------+
| Data Structures     | Strings, Hashes, Lists, Sets, Sorted   | Pure Strings / Raw       |
|                     | Sets (ZSET), Bitmaps, HyperLogLog, GEO.| Binary Blobs only.       |
+-----------------------------------------------------------------------------------------+
| Persistence         | **RDB Snapshots** & **AOF Logs**       | Pure In-Memory (No disk).|
+-----------------------------------------------------------------------------------------+
```

### Redis Low-Level Data Structure Internals
To minimize RAM overhead, Redis uses specialized internal data encodings:

```
+-----------------------------------------------------------------------------------------+
| REDIS TYPE          | SMALL SIZE ENCODING (< 512 items)    | LARGE SIZE ENCODING        |
+-----------------------------------------------------------------------------------------+
| Redis Hash          | **ZipList / ListPack:** Single         | Standard Hash Table      |
|                     | contiguous byte array (high cache loc).| (`dict`).                |
+-----------------------------------------------------------------------------------------+
| Redis Set           | **IntSet:** Sorted compact integer     | Hash Table (`dict` with  |
|                     | array (for integer elements).          | NULL values).            |
+-----------------------------------------------------------------------------------------+
| Redis Sorted Set    | **ZipList**                            | **SkipList + Hash Map:** |
| (`ZSET`)            |                                        | $O(\log N)$ search/rank. |
+-----------------------------------------------------------------------------------------+
```

---

# 5. Traffic Routing & Rate Limiting Algorithms

### L4 vs. L7 Load Balancing
- **Layer 4 (TCP/UDP):** Routes traffic based on IP address and Port without inspecting packet contents (AWS NLB). Ultra-fast packet NAT in kernel space.
- **Layer 7 (HTTP):** Inspects HTTP headers, cookies, URL paths (AWS ALB, NGINX). Enables path routing (`/api` vs `/static`), SSL termination, and WAF inspection.

### Rate Limiting Algorithms Comparison
- **Token Bucket:** Refills tokens at constant rate. Allows traffic bursts up to bucket capacity $B$.
- **Leaky Bucket:** Enforces smooth, strict constant output outflow rate.
- **Fixed Window:** Counts requests per time window (e.g. 100/min). *Boundary Spike Trap:* 2x traffic burst at window boundary!
- **Sliding Window Log (`Redis ZSET`):** Exact precision; removes timestamps older than window.

---

# 6. Asynchronous Messaging: Kafka vs. RabbitMQ

```
+-----------------------------------------------------------------------------------------+
| DIMENSION           | RABBITMQ (Message Queue)               | APACHE KAFKA (Event Log) |
+-----------------------------------------------------------------------------------------+
| Architecture        | **Smart Broker / Dumb Consumer**:      | **Dumb Broker / Smart Consumer**:|
|                     | Exchanges route to queues.             | Append-only Partitioned Log file.|
+-----------------------------------------------------------------------------------------+
| Message Deletion    | **Deleted immediately** on consumer ACK| **Retained on disk** for |
|                     |                                        | days/months by policy.   |
+-----------------------------------------------------------------------------------------+
| Message Replay      | Cannot replay historical messages.     | **Full Replay**: Consumers rewind|
|                     |                                        | offset pointers at will. |
+-----------------------------------------------------------------------------------------+
| Ordering Guarantee  | Per-queue ordering                     | **Strict Total Order within each |
|                     |                                        | individual Partition**.  |
+-----------------------------------------------------------------------------------------+
| Throughput          | ~20k–50k msgs/sec                      | **Millions msgs/sec** (Page Cache|
|                     |                                        | + Zero-Copy `sendfile()`).|
+-----------------------------------------------------------------------------------------+
```

---

# 7. Why Kafka is Blazing Fast (Zero-Copy Architecture)

1. **Sequential Disk I/O:** Appends sequentially to commit log files on disk (~2GB/s on SSDs).
2. **OS Page Cache:** Relies on Linux kernel page cache rather than JVM heap memory, eliminating Java Garbage Collection (GC) pauses.
3. **Linux Zero-Copy (`sendfile()`):** Transfers bytes directly from OS Page Cache into Network Socket buffer without copying data into JVM user space.

---

# 8. System Design Database Selection Framework

```
+-----------------------------------------------------------------------------------------+
| SYSTEM / FEATURE           | PRIMARY DB CHOICE        | SECONDARY / CACHING DB          |
+-----------------------------------------------------------------------------------------+
| Payment / Banking Ledger   | PostgreSQL / Spanner     | Redis (Distributed Locks SETNX) |
| User Sessions & Auth       | Redis (Key-Value)        | PostgreSQL / MySQL (Persistent) |
| Social Network Connections | Neo4j (Graph)            | Redis (Set Caching)             |
| Live Gaming Leaderboard    | Redis (`ZSET`)           | PostgreSQL                      |
| IoT Metric Telemetry       | Cassandra / TimescaleDB  | InfluxDB / Prometheus           |
| E-Commerce Catalog         | MongoDB (Document)       | Elasticsearch (Search Bar)      |
| Analytics Dashboard (BI)   | ClickHouse / Snowflake   | Redis (Query Caching)           |
+-----------------------------------------------------------------------------------------+
```

---

# 9. Production System Design Case Studies

### Case Study A: Uber Driver Location Tracking (High-Velocity Writes)
- **Requirement:** 1,000,000 drivers send GPS coordinates every 4 seconds ($250,000 \text{ writes/sec}$).
- **Database Architecture:** **Apache Cassandra / Redis GEO**.
- **Reasoning:** Relational DBs fail under 250k writes/sec due to random disk page locking. Cassandra's append-only LSM-Tree engine absorbs 250k writes/sec sequentially with zero lock contention.

### Case Study B: E-Commerce Inventory & Order Processing (Stripe / Amazon)
- **Requirement:** Prevent overselling flash-sale items (`inventory = 1`); strong ACID consistency across payments and orders.
- **Database Architecture:** **PostgreSQL + Redis Distributed Lock**.
- **Reasoning:** Requires ACID transactions, row-level locking (`SELECT ... FOR UPDATE`), and Foreign Key constraints to prevent double-spending.

---

# 10. Interview Quick-Fire Verbal Q&A

1. **"How does Redis store Sorted Sets (`ZSET`) internally?"**  
   *Answer:* Redis uses a **SkipList + Hash Map**. The Hash Map maps member $\to$ score in $O(1)$ time. The SkipList maintains scores in sorted order, supporting $O(\log N)$ rank queries and range fetches (`ZRANGEBYSCORE`).
2. **"Why should you delete a cache key rather than updating it during a DB write?"**  
   *Answer:* Updating the cache can result in race conditions where concurrent writes complete out of order, writing stale data to the cache. Invalidation (deletion) guarantees that the next read will pull fresh data from the DB.
