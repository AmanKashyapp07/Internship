# 04. Caching, Message Queues & System Reliability

> **Interview Mindset**: In production, your database is the most fragile and expensive component. Every request that never touches the database—absorbed by a Cache, smoothed by a Queue, or routed around by a Load Balancer—is an architectural victory.

---

## PART 1: Caching Architecture

A cache stores frequently accessed data in fast in-memory RAM (e.g., Redis). While a disk database query takes 10–50ms, a cache lookup takes **under 1ms**.

```
Client ──► App Server ──► [Redis Cache (RAM < 1ms)] ──► [Database (Disk 10-50ms)]
```

### 1. Cache Hit, Cache Miss & TTL
- **Cache Hit**: Data is found in the cache; returned immediately.
- **Cache Miss**: Data is not in cache; server queries database, populates cache, and returns response.
- **TTL (Time-To-Live)**: Expiration timer attached to cached keys (e.g., `SET product:42 ... EX 3600`). Prevents stale data from remaining forever.

### 2. What to Cache vs What NOT to Cache
- **DO Cache**: Static or slow-changing data (user profiles, product details, app configs, top 20% hot articles).
- **DO NOT Cache**: Rapidly mutating data with high consistency requirements (live stock trading prices, real-time inventory count during flash sales, user bank balances).

---

### 3. The 4 Core Caching Topologies

```
1. Cache-Aside (Lazy Loading):
┌────────────┐     1. Read Cache     ┌─────────────┐
│ App Server ├──────────────────────►│ Redis Cache │
└─────┬──────┘                       └──────▲──────┘
      │ 2. On Miss:                         │ 3. Populate
      │    Read Database                    │    Cache
      ▼                                     │
┌────────────┐                              │
│  Database  ├──────────────────────────────┘
└────────────┘

2. Write-Through:
┌────────────┐     1. Write Data     ┌─────────────┐    2. Sync Write    ┌────────────┐
│ App Server ├──────────────────────►│ Redis Cache ├────────────────────►│  Database  │
└────────────┘                       └─────────────┘                     └────────────┘

3. Write-Behind (Write-Back):
┌────────────┐  1. Write & Return    ┌─────────────┐   2. Async Batch    ┌────────────┐
│ App Server ├──────────────────────►│ Redis Cache ├ - - - - - - - - - -►│  Database  │
└────────────┘                       └─────────────┘       Flush         └────────────┘
```

| Pattern | How it Works | Pros | Cons / Risks | Best Used For |
| :--- | :--- | :--- | :--- | :--- |
| **Cache-Aside (Standard)** | App checks cache; on miss, reads DB and populates cache. | Only requested data is cached; resilient to cache node restarts. | Cache miss latency penalty; potential stale data until TTL expires. | General web workloads (user profiles, product details). |
| **Write-Through** | App writes to Cache, which synchronously updates DB before returning. | Cache is never stale; immediate consistency. | Higher write latency (writes to two systems). | Systems where reads must immediately see recent writes. |
| **Write-Behind (Write-Back)** | App writes to Cache and returns immediately. Cache asynchronously flushes in batches to DB. | **Extreme write throughput**; absorbs write bursts. | **Data loss risk** if cache crashes before dirty writes flush to disk. | Real-time view counts, gaming scores, IoT metrics. |
| **Write-Around** | App writes directly to DB, bypassing cache. Cache only filled on read miss. | Avoids polluting cache with data that may never be read again. | First read after write is always a cache miss. | Infrequently read logs or archival records. |

---

### 4. Cache Eviction Policies & The 3 Classic Disasters

#### Eviction Policies:
- **LRU (Least Recently Used)**: Evicts items not accessed for the longest time. *(Default standard choice).*
- **LFU (Least Frequently Used)**: Evicts items with the lowest total access count.
- **FIFO (First-In, First-Out)**: Evicts oldest created items regardless of access.

#### The 3 Classic Cache Disasters (Interview Favorites!):
1. **Cache Stampede (Thundering Herd)**:
   - *Problem*: A super-popular key (e.g., World Cup score) expires. 20,000 concurrent requests miss simultaneously and hit the DB at once.
   - *Fix*: **Distributed Mutex (Lock)** so only 1 thread recalculates the cache, or **Probabilistic Early Expiration (XFetch)** to refresh before expiry.
2. **Cache Penetration**:
   - *Problem*: Malicious user requests non-existent keys (`id = -9999`). Every request bypasses cache and hammers the DB.
   - *Fix*: Store `{ id: -9999, value: NULL }` with a short 60s TTL, or place a **Bloom Filter** in front.
3. **Cache Avalanche**:
   - *Problem*: Thousands of keys are inserted with identical `TTL = 3600s`. Exactly 1 hour later, all keys expire at once, crushing the DB.
   - *Fix*: Add **TTL Jitter** (`TTL = 3600 + rand(-300, 300) seconds`).

---

## PART 2: Message Queues & Asynchronous Processing

```
SYNCHRONOUS (Tight Coupling, Slow):
Client ──► Server ──► External Payment API (Wait 3s) ──► DB ──► Client

ASYNCHRONOUS (Decoupled, Fast):
Client ──► Server ──► [Message Queue] ──► Immediate Response (< 100ms)
                             │
                             ▼
                     [Async Worker Tier] ──► Processes in Background
```

### Why Use Message Queues?
1. **Decoupling**: Producer and Consumer services operate independently.
2. **Backpressure (Shock Absorbing)**: During flash sales (50,000 writes/sec), queues buffer incoming messages safely while workers process at a controlled rate (2,000/sec), preventing DB crashes.
3. **Retry & Fault Tolerance**: If worker crashes, message remains safely in the queue.

### Kafka vs RabbitMQ:

| Feature | RabbitMQ (Message Broker) | Apache Kafka (Distributed Commit Log) |
| :--- | :--- | :--- |
| **Model** | Smart broker, dumb consumer. Deletes message once acknowledged. | Append-only partitioned disk log. Messages persist for days. |
| **Ordering** | Per-queue ordering. | Guaranteed strict ordering **within each partition**. |
| **Replayability** | Cannot replay consumed messages. | **Can replay** messages from any past offset or timestamp. |
| **Throughput** | Moderate (~50k msgs/sec). | **Ultra-high** (millions of msgs/sec with zero-copy I/O). |
| **Best For** | Task queues (Celery), complex routing rules. | Event streaming, real-time analytics, metrics, audit trails. |

---

## PART 3: Reliability, Load Balancing & Redundancy

### The Essential Mental Model for System Design

You don't need to memorize complex architectures. You only need to understand **why each component exists**:

```
                    ┌── Server 1 ──┐
Client ──► LB ──────┼── Server 2 ──┼── Cache (Redis)
                    └── Server 3 ──┘       │
                                           ▼
                                      Primary DB
                                       /       \
                                  Replica 1   Replica 2
```

| Component | Why It Exists | What Fails If It's Missing? |
| :--- | :--- | :--- |
| **Load Balancer (LB)** | Distributes incoming client traffic across multiple servers. | A single server crashes under traffic spikes or hardware failure. |
| **Multiple App Servers** | Provides horizontal scale and eliminates single point of failure (SPOF). | Deploying a code update causes downtime; one crash kills the entire app. |
| **Cache (Redis)** | Serves hot data in sub-millisecond RAM, shielding the database. | The database CPU spikes to 100% on repetitive reads. |
| **Primary Database** | Serves as the single source of truth for all writes and mutations. | Data consistency breaks; simultaneous conflicting writes corrupt state. |
| **Read Replicas** | Offload 90%+ read traffic from the Primary DB. | Read queries starve write transactions of disk I/O and connection slots. |

---

### The Production Distributed System Blueprint

```
┌─────────────────────────────────────────────────────────────┐
│                    Clients (Mobile & Web)                   │
└──────────────┬───────────────────────────────┬──────────────┘
               │                               │ Static Assets
               │ API Requests                  ▼
               │                    ┌─────────────────────┐
               │                    │  CDN / Edge Cache   │
               ▼                    └─────────────────────┘
┌─────────────────────────────┐
│    Global Load Balancer     │
└──────────────┬──────────────┘
               │
               ▼
┌─────────────────────────────┐     Fast Reads / Locks     ┌─────────────────────┐
│   Stateless App Servers     │◄──────────────────────────►│ Redis Cache Cluster │
└──────┬───────────────┬──────┘                            └─────────────────────┘
       │ Writes        │ Reads
       ▼               ▼
┌──────────────┐ ┌──────────────┐ ┌──────────────┐
│  Primary DB  │ │Read Replica 1│ │Read Replica 2│
│(All Mutations│ └──────▲───────┘ └──────▲───────┘
└──────┬───────┘        │ Async Binary   │
       │                │   Replication  │
       └────────────────┴────────────────┘
       │
       │ Async Events / Metrics
       ▼
┌─────────────────────────────┐
│ Message Queue / Apache Kafka│
└──────────────┬──────────────┘
               │
               ▼
┌─────────────────────────────┐
│   Async Consumer Workers    │
└──────────────┬──────────────┘
               │ Batch Ingestion
               ▼
┌─────────────────────────────┐
│ Columnar DB / ClickHouse DW │
└─────────────────────────────┘
```

### 1. Load Balancers & Algorithms
- **Layer 4 (L4 - Transport)**: Routes based on IP and Port. Ultra-fast packet forwarding without inspecting HTTP payload.
- **Layer 7 (L7 - Application)**: Inspects HTTP headers, cookies, and URLs (e.g., `/api/orders` vs `/api/users`).
- **Routing Algorithms**:
  - *Round Robin*: Sequential distribution ($1 \to 2 \to 3 \to 1$).
  - *Least Connections*: Routes to server with fewest active requests.
  - *IP Hash*: Maps client IP to a specific server.

### 2. Eliminating Single Points of Failure (SPOF)
- **Rule of Redundancy**: Any system with $N=1$ component has a SPOF.
- **Load Balancers**: Deploy active-passive load balancers using VRRP / Virtual IP (Keepalived).
- **App Servers**: Run at least 2 stateless instances behind the Load Balancer.
- **Health Checks**: Load balancer pings `GET /health` every 5 seconds. If server fails 3 consecutive checks, it is removed from rotation.

### 3. Basic Rate Limiting
- Protects services from being overwhelmed or abused.
- **Token Bucket Algorithm**: Tokens are added to a bucket at a constant rate. Requests consume a token. If bucket is empty, request receives `429 Too Many Requests`. Allows controlled bursts!
- **Sliding Window Counter**: Tracks request counts in Redis across rolling time windows.

### 4. Basic Monitoring & Metrics
- **Latency Percentiles**: Average latency is misleading. Monitor **P95** (95% of users experience this latency or faster) and **P99** (worst 1% experience).
- **Slow Query Logs**: Automatically log any SQL query executing longer than 200ms.
- **Cache Hit Ratio**: $\frac{\text{Hits}}{\text{Hits} + \text{Misses}}$. Target $> 85\%$.
