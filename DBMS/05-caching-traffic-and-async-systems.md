# 05: Caching Strategies, Traffic Routing & Asynchronous Systems

> **Target:** Verbal Technical Interviews at Top Tech Companies (FAANG, Uber, Stripe, Atlassian, Microsoft, Google).  
> **Focus:** Caching failure modes, Redis vs. Memcached, rate limiting algorithms, Kafka vs. RabbitMQ internals, and real-time protocols.

---

## 1. Stateless vs. Stateful Architecture

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

## 2. Caching Patterns & The 3 Production Catastrophes

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

### B. The 3 Major Cache Catastrophes & Spoken Solutions

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
|                     | miss and execute the heavy query at DB.| 2. **XFetch early refresh|
|                     |                                        | in background.           |
+-----------------------------------------------------------------------------------------+
```

---

## 3. Distributed Caching: Redis vs. Memcached

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
| High Availability   | Redis Sentinel (Automatic Failover)    | None native (Requires    |
|                     | & Redis Cluster (16,384 Hash Slots).   | client-side sharding).   |
+-----------------------------------------------------------------------------------------+
| Best Use Case       | Complex state, Leaderboards (ZSET),    | Simple read-heavy key-val|
|                     | Rate limiters, Pub/Sub, Queues.        | caching on multi-core CPU|
+-----------------------------------------------------------------------------------------+
```

---

## 4. Traffic Routing & Rate Limiting

### L4 vs. L7 Load Balancing
- **Layer 4 (Transport Layer - TCP/UDP):** Routes traffic based on IP address and Port without inspecting packet contents (e.g. AWS NLB). Ultra-fast packet NAT in kernel space.
- **Layer 7 (Application Layer - HTTP):** Inspects HTTP headers, cookies, query params, and URL paths (e.g. AWS ALB, NGINX). Enables path-based routing (`/api` vs `/static`), SSL termination, and WAF inspection.

### Rate Limiting Algorithms Comparison

```
+-----------------------------------------------------------------------------------------+
| ALGORITHM           | HOW IT WORKS                           | SPOKEN TRADEOFF          |
+-----------------------------------------------------------------------------------------+
| Token Bucket        | Tokens refill at constant rate.        | **Allows traffic bursts**|
|                     | 1 token per request. Rejects when empty| up to bucket capacity B. |
+-----------------------------------------------------------------------------------------+
| Leaky Bucket        | Requests enter FIFO queue; processed at| **Enforces smooth, strict|
|                     | a strict constant outflow rate.        | constant output rate**.  |
+-----------------------------------------------------------------------------------------+
| Fixed Window        | Counts requests per fixed time window  | **Boundary Spike Trap**: |
|                     | (e.g. 100/min). Resets at boundary.    | 2x limit at window edge! |
+-----------------------------------------------------------------------------------------+
| Sliding Window Log  | Stores request timestamps in a Sorted  | **Exact precision**; high|
|                     | Set (`Redis ZSET`). Removes old stamps.| memory for high traffic. |
+-----------------------------------------------------------------------------------------+
```

---

## 5. Asynchronous Messaging: Kafka vs. RabbitMQ

```
+-----------------------------------------------------------------------------------------+
| DIMENSION           | RABBITMQ (Message Queue)               | APACHE KAFKA (Distributed Event Log)|
+-----------------------------------------------------------------------------------------+
| Architecture        | **Smart Broker / Dumb Consumer**:      | **Dumb Broker / Smart Consumer**:|
|                     | Exchanges route to queues.             | Append-only Partitioned Log file.|
+-----------------------------------------------------------------------------------------+
| Message Deletion    | **Deleted immediately** on consumer ACK| **Retained on disk** for |
|                     |                                        | days/months by retention policy. |
+-----------------------------------------------------------------------------------------+
| Message Replay      | Cannot replay historical messages.     | **Full Replay**: Consumers rewind|
|                     |                                        | offset pointers at will.         |
+-----------------------------------------------------------------------------------------+
| Ordering Guarantee  | Per-queue ordering (lost if multiple   | **Strict Total Order within each |
|                     | consumers read from same queue).       | individual Partition**.          |
+-----------------------------------------------------------------------------------------+
| Throughput          | ~20k–50k msgs/sec                      | **Millions msgs/sec** (Page Cache|
|                     |                                        | + Zero-Copy `sendfile()`).       |
+-----------------------------------------------------------------------------------------+
```

### Why Kafka is Blazing Fast (3 Low-Level Hardware Reasons)
1. **Sequential Disk I/O:** Appends sequentially to commit log files on disk (~2GB/s on modern SSDs).
2. **OS Page Cache:** Relies on Linux kernel page cache rather than JVM heap memory, eliminating Java Garbage Collection (GC) pauses.
3. **Linux Zero-Copy (`sendfile()`):** Transfers bytes directly from the OS Page Cache into the Network Socket buffer without copying data into JVM user space.

---

## 6. Real-Time Communication Protocols

```
+-----------------------------------------------------------------------------------------+
| PROTOCOL            | DIRECTIONALITY  | TRANSPORT / PROTOCOL | BEST FOR                 |
+-----------------------------------------------------------------------------------------+
| Server-Sent Events  | **Unidirectional**| Persistent HTTP/2  | Live financial tickers,  |
| (SSE)               | (Server $\to$ Clt)| `text/event-stream`  | LLM token stream (ChatGPT)|
+-----------------------------------------------------------------------------------------+
| WebSockets          | **Full-Duplex** | Persistent single TCP| Collaborative editing,   |
|                     | (Bidirectional) | connection (Upgrade) | chat apps, live gaming.  |
+-----------------------------------------------------------------------------------------+
| Long Polling        | Request-Response| Hanging HTTP request | Push fallback when       |
|                     |                 | held open by server  | WebSockets are blocked.  |
+-----------------------------------------------------------------------------------------+
```

---

## 7. High-Yield Verbal Interview Q&A

### Q1: "How does Kafka guarantee in-order message processing?"
> **Spoken Answer:** Kafka guarantees strict ordering **only within a single partition**, not globally across the entire topic. By providing a message `PartitionKey` (e.g. `order_id`), Kafka hashes the key (`hash(key) % partition_count`) to ensure all events for that entity always land in the same partition, where a single assigned consumer thread processes them sequentially.

### Q2: "When should you choose Server-Sent Events (SSE) over WebSockets?"
> **Spoken Answer:** Choose SSE when communication is strictly unidirectional from server to client (e.g. LLM streaming like ChatGPT, live scoreboards). SSE runs over standard HTTP/2, natively supports client auto-reconnection with `Last-Event-ID`, works across firewalls with zero custom setup, and avoids the complexity of bidirectional WebSocket framing.

### Q3: "What is the Cache Stampede (Thundering Herd) problem and how do you solve it?"
> **Spoken Answer:** It occurs when a popular cached key expires, causing thousands of concurrent requests to experience a cache miss simultaneously and flood the primary database with identical heavy queries. It is solved using a **Distributed Mutex Lock (`SETNX` in Redis)** so only the first request queries the DB while others wait, or via **Probabilistic Early Expiration (XFetch)** to refresh the cache in the background before it expires.
