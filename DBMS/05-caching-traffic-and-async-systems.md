# 05: Caching Strategies, Traffic Routing & Asynchronous Systems

> **Target:** Top Product & FinTech Technical Interviews (FAANG, Uber, Stripe, Atlassian, Citadel).  
> **Style:** High-ROI revision sheet. Focuses on caching failure modes (Stampede/Avalanche/Penetration), L4 vs. L7 load balancing, rate limiting algorithms, Kafka vs. RabbitMQ internals, and real-time streaming protocols.

---

# Table of Contents
1. [Scaling Fundamentals (Stateless vs. Stateful Architecture)](#1-scaling-fundamentals-stateless-vs-stateful-architecture)
2. [Caching Strategies & Production Failure Modes](#2-caching-strategies--production-failure-modes)
3. [Distributed Caching: Redis vs. Memcached](#3-distributed-caching-redis-vs-memcached)
4. [Traffic Routing: Load Balancers, Proxies & Rate Limiters](#4-traffic-routing-load-balancers-proxies--rate-limiters)
5. [Asynchronous Systems: Message Queues vs. Event Streams](#5-asynchronous-systems-message-queues-vs-event-streams)
6. [Real-Time Client-Server Protocols](#6-real-time-client-server-protocols)
7. [High-Yield Interview Questions & Defense](#7-high-yield-interview-questions--defense)
8. [Quick Recap](#8-quick-recap)

---

# 1. Scaling Fundamentals (Stateless vs. Stateful Architecture)

```
+-----------------------------------------------------------------------------------------+
| DIMENSION           | STATELESS ARCHITECTURE                 | STATEFUL ARCHITECTURE    |
+-----------------------------------------------------------------------------------------+
| Server Memory       | Holds **ZERO client session state**    | Retains client state in  |
|                     | across requests.                       | local server RAM/disk.   |
| Horizontal Scaling  | **Trivially Simple:** Spin up 50 nodes | **Complex:** Requires    |
|                     | behind a standard Round-Robin LB.      | Sticky Sessions or state |
|                     |                                        | sync across nodes.       |
| Node Failure Impact | Zero impact: Any surviving node can    | High: Active sessions    |
|                     | handle the next user request.          | lost unless replicated.  |
| State Offloading    | Session data pushed to external shared | Local RAM caches /       |
|                     | storage (Redis, DB, JWT Tokens).       | WebSocket open sockets.  |
+-----------------------------------------------------------------------------------------+
```

```
Stateless Web Tier with Distributed Shared State:
[ Client Requests ] ---> [ Load Balancer (Round Robin) ]
                               /         |         \
                              v          v          v
                        [ App Node 1 ] [ App Node 2 ] [ App Node 3 ]
                              \          |          /
                               v         v         v
                      [ Central Redis Shared Session Store ]
```

---

# 2. Caching Strategies & Production Failure Modes

A **Cache** is a high-speed, volatile in-memory data layer (SRAM/DRAM) that stores copies of frequently accessed data to eliminate expensive database queries and disk I/O.

---

### A. Caching Design Patterns

```
+-----------------------------------------------------------------------------------------+
| PATTERN             | FLOW / MECHANICS                       | PROS & CONS              |
+-----------------------------------------------------------------------------------------+
| Cache-Aside         | App reads Cache. On Miss: App reads DB,| **Pros:** Cache holds only|
| (Lazy Loading)      | writes to Cache, and returns data.     | requested data.          |
|                     | Writes update DB and invalidate cache. | **Cons:** Cache miss     |
|                     |                                        | penalty on first read.   |
+-----------------------------------------------------------------------------------------+
| Write-Through       | App writes to Cache; Cache             | **Pros:** Data is never  |
|                     | synchronously writes to DB before      | stale in cache.          |
|                     | returning success.                     | **Cons:** Higher write   |
|                     |                                        | latency (dual write).    |
+-----------------------------------------------------------------------------------------+
| Write-Back          | App writes to Cache only. Cache batches| **Pros:** Ultra-fast     |
| (Write-Behind)      | and asynchronously writes to DB in the | write throughput.        |
|                     | background.                            | **Cons:** Risk of DATA   |
|                     |                                        | LOSS if cache crashes!   |
+-----------------------------------------------------------------------------------------+
| Read-Through        | App interacts ONLY with Cache. Cache   | **Pros:** Simplifies app |
|                     | transparently reads from DB on misses. | application logic.       |
+-----------------------------------------------------------------------------------------+
```

---

### B. The 3 Major Cache Catastrophes & Mitigations

```
1. CACHE PENETRATION:
- Problem: Attackers repeatedly query non-existent keys (e.g. user_id = -999).
  Every request misses the cache and hits the database directly, exhausting DB connection pools!
- Mitigations:
  a. BLOOM FILTER: Place a Bloom Filter in front of the cache. Non-existent keys are rejected in O(1) time without DB/cache queries.
  b. NULL VALUE CACHING: Cache empty/null results with a short TTL (e.g. 60 seconds).

2. CACHE AVALANCHE:
- Problem: Millions of keys are set with the EXACT SAME TTL (e.g. 1 hour).
  At 1:00:00, all keys expire simultaneously. 100% of traffic floods the DB at once, crashing it.
- Mitigation:
  a. RANDOMIZED JITTER: Add a random delta to every TTL: TTL = base_ttl + rand(0, 300) seconds.

3. CACHE STAMPEDE / THUNDERING HERD:
- Problem: A super-popular "Hot Key" (e.g. World Cup Final Score) expires.
  Thousands of concurrent threads experience a Cache Miss simultaneously and all execute the same heavy SQL query on the DB!
- Mitigations:
  a. DISTRIBUTED MUTEX LOCK: Only the first thread that misses acquires a Redis lock (SETNX) to query the DB and update the cache; others wait or retry.
  b. PROBABILISTIC EARLY EXPIRATION (XFetch Algorithm): Refresh cache in the background before it actually expires.
```

---

# 3. Distributed Caching: Redis vs. Memcached

```
+-----------------------------------------------------------------------------------------+
| DIMENSION           | REDIS                                  | MEMCACHED                |
+-----------------------------------------------------------------------------------------+
| Architecture        | Single-Threaded Event Loop (with I/O   | Multi-Threaded Memory    |
|                     | Multiplexing via epoll/kqueue)         | Allocator (Slab Allocator)|
| Data Structures     | Strings, Hashes, Lists, Sets, Sorted   | Pure String / Binary     |
|                     | Sets (ZSET), Bitmaps, HyperLogLog, GEO | Blobs only (Key-Value)   |
| Persistence         | **RDB Snapshots** & **AOF Logs**       | Pure In-Memory (No disk) |
| Replication & HA    | Redis Sentinel (Failover) & Redis      | None native (Client-side |
|                     | Cluster (16,384 Hash Slots)            | sharding required)       |
| Lua Scripting       | Supported (Atomic multi-step scripts)  | Not supported            |
| Ideal Use Case      | Complex state, Leaderboards, Rate      | Pure read-heavy caching  |
|                     | limiters, Pub/Sub, Queues              | with high multi-core CPU |
+-----------------------------------------------------------------------------------------+
```

---

# 4. Traffic Routing: Load Balancers, Proxies & Rate Limiters

---

### A. Layer 4 (L4) vs. Layer 7 (L7) Load Balancers

```
+-----------------------------------------------------------------------------------------+
| DIMENSION           | LAYER 4 (Transport Layer - TCP/UDP)    | LAYER 7 (Application Layer - HTTP)|
+-----------------------------------------------------------------------------------------+
| Routing Basis       | IP Address and TCP/UDP Port number.    | HTTP Headers, Cookies, URL Path, |
|                     | Does NOT inspect packet payload.       | Query Parameters, JSON body.     |
| Performance         | **Ultra-High Throughput & Low Latency**| **Slightly Higher Latency**      |
|                     | (Performs packet NAT in kernel space). | (Must buffer and parse HTTP).    |
| Features            | Simple round-robin, connection hashing.| **Path-Based Routing** (/api vs  |
|                     |                                        | /static), SSL Termination, WAF.  |
| Examples            | AWS NLB, Linux IPVS, HAProxy (TCP mode)| AWS ALB, NGINX, Envoy, Traefik   |
+-----------------------------------------------------------------------------------------+
```

---

### B. Reverse Proxy vs. Forward Proxy
- **Forward Proxy:** Sits in front of **Clients** (e.g. corporate network proxy). Hides client IPs, enforces outbound firewall policies, and caches external web assets.
- **Reverse Proxy (e.g. NGINX, Envoy):** Sits in front of **Web Servers**. Hides internal server topology, provides SSL/TLS termination, compresses responses (Gzip/Brotli), and load-balances backend microservices.

---

### C. Rate Limiting Algorithms

```
+-----------------------------------------------------------------------------------------+
| ALGORITHM           | MECHANICS                              | TRADEOFFS                |
+-----------------------------------------------------------------------------------------+
| Token Bucket        | Tokens added to bucket at rate R.      | **Allows bursts of up to |
|                     | Consumes 1 token per request. Rejects  | bucket capacity B**;     |
|                     | when empty.                            | memory-efficient (2 nums)|
+-----------------------------------------------------------------------------------------+
| Leaky Bucket        | Requests enter FIFO queue; processed at| **Smooths traffic to a   |
|                     | strict constant output rate.           | strictly constant rate**;|
|                     |                                        | bursts are buffered.     |
+-----------------------------------------------------------------------------------------+
| Fixed Window        | Counts requests in fixed time windows  | **Boundary Spike Trap**: |
| Counter             | (e.g. 100 req/min). Resets at window.  | 2x limit can pass at the |
|                     |                                        | window boundary!         |
+-----------------------------------------------------------------------------------------+
| Sliding Window Log  | Stores timestamps of all requests in a | **High Memory**: Storing |
|                     | sorted set (`Redis ZSET`).             | millions of timestamps.  |
+-----------------------------------------------------------------------------------------+
| Sliding Window      | Approximates count by blending current | **Low memory, 99% accuracy|
| Counter             | and previous window weights.           | without boundary spikes. |
+-----------------------------------------------------------------------------------------+
```

```
Sliding Window Log implementation in Redis (ZSET):
1. Remove timestamps older than (now - 60s):
   ZREMRANGEBYSCORE rate_limit:user_42 0 (now - 60000)
2. Count remaining timestamps:
   ZCARD rate_limit:user_42
3. If count < Limit -> ZADD rate_limit:user_42 now now -> ACCEPT
   Else -> REJECT (HTTP 429 Too Many Requests)
```

---

# 5. Asynchronous Systems: Message Queues vs. Event Streams

---

### Apache Kafka vs. RabbitMQ Architecture

```
+-----------------------------------------------------------------------------------------+
| DIMENSION           | RABBITMQ (Traditional Message Queue)   | APACHE KAFKA (Distributed Event Log)|
+-----------------------------------------------------------------------------------------+
| Underlying Model    | **Smart Broker / Dumb Consumer**:      | **Dumb Broker / Smart Consumer**:|
|                     | Routes via Exchanges to Queues.        | Append-only Partitioned Log file.|
| Message Deletion    | Messages are **DELETED immediately**   | Messages are **RETAINED on disk**|
|                     | after consumer acknowledges (ACK).     | for days/months (Retention-based)|
| Message Replay      | Cannot replay historical messages.     | **Full Replayability**: Consumers|
|                     |                                        | rewind offset pointers at will.  |
| Ordering Guarantee  | Per-queue ordering; lost if using      | **Strict Total Order within each |
|                     | multiple consumers on 1 queue.         | individual Partition**.          |
| Consumer Scaling    | Competing Consumers on same queue.     | **Consumer Groups**: Max 1 active|
|                     |                                        | consumer per partition in a group|
| Throughput          | ~10k – 50k msgs/sec                    | **Millions msgs/sec** (via Page  |
|                     |                                        | Cache and Zero-Copy `sendfile`). |
+-----------------------------------------------------------------------------------------+
```

```
Apache Kafka Partitioning & Consumer Group Mechanics:
Topic: 'order_events' (3 Partitions):
[ Partition 0 ] ===> Consumer A (Instance 1)
[ Partition 1 ] ===> Consumer B (Instance 2)  } Consumer Group: "PaymentService"
[ Partition 2 ] ===> Consumer C (Instance 3)

Key Rule: If you add a 4th consumer instance to this group, it remains IDLE because
a single partition can only be consumed by at most ONE consumer in the same group!
```

---

### Why Kafka is Blazing Fast (The Low-Level Hardware Secrets)
1. **Sequential Disk I/O:** Appends to the end of a commit log file sequentially. Sequential disk writes on modern SSDs achieve ~2GB/sec (as fast as RAM random access).
2. **OS Page Cache:** Relies heavily on the Linux kernel Page Cache rather than JVM heap memory, avoiding Java Garbage Collection (GC) pauses.
3. **Linux Zero-Copy (`sendfile` system call):** Transfers data directly from the OS Page Cache into the Network Socket buffer **without copying bytes into JVM user space**, eliminating CPU context switches:
   $$\text{Disk} \to \text{OS Page Cache} \xrightarrow{\mathbf{sendfile()}} \text{NIC Network Buffer}$$

---

# 6. Real-Time Client-Server Protocols

```
+-----------------------------------------------------------------------------------------+
| PROTOCOL            | DIRECTIONALITY  | TRANSPORT / PROTOCOL | IDEAL PRODUCTION USE CASE|
+-----------------------------------------------------------------------------------------+
| Short Polling       | Unidirectional  | Repeated HTTP GET    | Infrequent updates with  |
|                     | (Client -> Srv) | requests on timer    | low developer effort.    |
+-----------------------------------------------------------------------------------------+
| Long Polling        | Unidirectional  | Hanging HTTP request | Push notifications where |
|                     | (Server -> Clt) | held open until data | WebSockets are blocked.  |
+-----------------------------------------------------------------------------------------+
| Server-Sent Events  | **Unidirectional**| Persistent HTTP/2  | Live stock tickers, AI   |
| (SSE)               | (Server -> Clt) | `text/event-stream`  | token streaming (ChatGPT)|
+-----------------------------------------------------------------------------------------+
| WebSockets          | **Full-Duplex** | Single persistent TCP| Multiplayer gaming, live |
|                     | (Bidirectional) | connection (Upgrade) | collaborative docs, chat.|
+-----------------------------------------------------------------------------------------+
```

---

# 7. High-Yield Interview Questions & Defense

### Q1: How does Kafka guarantee in-order message processing?
> **Answer:** Kafka guarantees strict total ordering **only within a single partition**, not across the entire topic. By providing a message `PartitionKey` (e.g. `order_id`), Kafka hashes the key (`hash(key) % partition_count`) to ensure all events for that specific order always land in the exact same partition, processed sequentially by a single consumer thread.

### Q2: When should you choose Server-Sent Events (SSE) over WebSockets?
> **Answer:** Use SSE when communication is strictly **unidirectional from server to client** (e.g. live sports scores, LLM token streaming like ChatGPT). SSE runs over standard HTTP/2, natively supports automatic client reconnection with `Last-Event-ID`, traverses enterprise firewalls with zero special configuration, and requires no custom WebSocket protocol framing.

### Q3: What is the Thundering Herd / Cache Stampede problem and how is it solved?
> **Answer:** It occurs when a heavily accessed cached key expires, and thousands of concurrent requests experience a cache miss at the exact same instant, all hitting the primary database with identical queries. It is solved using a **Distributed Mutex Lock (`SETNX` in Redis)** so only one thread executes the query while others wait, or using **Probabilistic Early Expiration (XFetch)** to refresh the cache in the background prior to expiry.

---

# 8. Quick Recap

- **Stateless Tier:** Session state pushed to shared Redis; allows trivial horizontal auto-scaling behind load balancers.
- **Cache Patterns:** Cache-Aside for general reads; Write-Through for consistent data; Write-Back for ultra-fast batched writes (data loss risk).
- **Cache Failures:** Penetration $\to$ Bloom Filters; Avalanche $\to$ Randomized TTL Jitter; Stampede $\to$ Distributed Mutex Lock (`SETNX`).
- **L4 vs. L7:** L4 routes at TCP/IP level without inspecting packets (maximum speed); L7 parses HTTP headers, cookies, and URLs for smart routing.
- **Rate Limiters:** Token Bucket allows bursts; Leaky Bucket forces constant rate; Sliding Window Log (Redis ZSET) provides exact precision.
- **Kafka vs. RabbitMQ:** RabbitMQ deletes messages on ACK; Kafka stores immutable append-only logs on disk with consumer offset pointers.
- **Kafka Speed:** Sequential disk I/O + OS Page Cache + Linux `sendfile()` Zero-Copy.
- **SSE vs. WebSockets:** SSE is unidirectional HTTP event streaming (ChatGPT); WebSockets are bidirectional full-duplex TCP channels (Chat/Gaming).
