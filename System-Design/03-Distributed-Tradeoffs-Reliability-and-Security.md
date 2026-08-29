# Master Guide 03: Distributed Tradeoffs, Reliability & Security

> **Focus:** Horizontal vs. Vertical Scaling, CAP & PACELC Theorems, Consistency Models, Latency vs. Throughput (p99 Tail Latency), High Availability SLAs, Rate Limiting Algorithms, Circuit Breakers, and SQL vs. NoSQL vs. NewSQL.
> 
> *Targeted for Top-Tier Tech Engineering & Internship Interviews.*

---

# Table of Contents
1. [Horizontal vs. Vertical Scaling](#1-horizontal-vs-vertical-scaling)
2. [CAP Theorem & The PACELC Extension](#2-cap-theorem--the-pacelc-extension)
3. [Consistency Models: Strong vs. Eventual](#3-consistency-models-strong-vs-eventual)
4. [Latency vs. Throughput & Tail Latency (p99)](#4-latency-vs-throughput--tail-latency-p99)
5. [High Availability & The "Nines" of Uptime](#5-high-availability--the-nines-of-uptime)
6. [Rate Limiting Algorithms & Redis Implementation](#6-rate-limiting-algorithms--redis-implementation)
7. [Resiliency Patterns: Circuit Breakers & Backoff](#7-resiliency-patterns-circuit-breakers--backoff)
8. [SQL vs. NoSQL vs. NewSQL Master Decision Matrix](#8-sql-vs-nosql-vs-newsql-master-decision-matrix)
9. [Monolith vs. Microservices vs. Serverless](#9-monolith-vs-microservices-vs-serverless)
10. [Glanceable Summary Matrix](#10-glanceable-summary-matrix)

---

# 1. Horizontal vs. Vertical Scaling

### 1. The "Aha!" Intuition
- **Vertical Scaling (Scale Up):** Buying a faster Ferrari to carry packages. Fast and simple, but eventually you hit the laws of physics and Ferrari maxes out.
- **Horizontal Scaling (Scale Out):** Hiring a fleet of 50 delivery vans. If package volume doubles, you simply hire 50 more vans.

### 2. ASCII Architecture Flowchart
```
VERTICAL SCALING (Scale Up):
[ Server: 4 Core, 16GB RAM ] ---> (Upgrade!) ---> [ Monster Server: 128 Core, 1TB RAM ]
                                                   (Hits Hardware Limit & Exponential Cost!)

HORIZONTAL SCALING (Scale Out):
                                  +---> [ Node 1 (8 Core) ]
[ Traffic Spikes 10x ] ---> [ LB ] ---> [ Node 2 (8 Core) ] ---> [ Auto-Scale to 50 Nodes ]
                                  +---> [ Node 3 (8 Core) ]
```

### 3. Comparison Matrix
```
+---------------------------------------------------------------------------------------------------+
| ATTRIBUTE            | VERTICAL SCALING (SCALE UP)           | HORIZONTAL SCALING (SCALE OUT)     |
+---------------------------------------------------------------------------------------------------+
| Mechanics            | Upgrade CPU/RAM of a single machine   | Add more commodity machines to pool|
| Downtime Required    | Usually requires hardware reboot      | Zero downtime (Add nodes live)     |
| Complexity           | Zero code change (Monolith friendly)  | Requires stateless architecture    |
| Hard Scaling Limit   | Physical hardware & motherboard ceiling| Infinite linear scaling in theory  |
| Cost Curve           | Exponentially expensive at high specs | Linear, predictable cloud cost     |
| Best Use Case        | Early stage MVPs, Relational DBs      | Web application tiers, NoSQL nodes |
+---------------------------------------------------------------------------------------------------+
```

---

# 2. CAP Theorem & The PACELC Extension

### 1. The "Aha!" Intuition
Imagine two bank branches in New York and London. A storm cuts the transatlantic undersea cable between them (**Network Partition**). A customer walks into the London branch to withdraw money. The London branch has two choices:
1. **Refuse the withdrawal (Consistency / CP):** "The network is down; I cannot verify your balance in NY. Transaction blocked!"
2. **Hand over the cash (Availability / AP):** "Here is your money! We will reconcile with NY when the storm clears (even if you overdraw)."

### 2. ASCII Architecture Flowchart: The CAP Triangle
```
                                  [ Consistency (C) ]
                                      /         \
                                     /           \
                                    /      P      \  (Partitions are INEVITABLE on real networks!)
                                   /               \
                 [ Availability (A) ] ----------- [ Partition Tolerance (P) ]

CHOICE DURING A NETWORK SPLIT:
* CP (Consistency): Picked by Stripe, Banking Ledgers, Google Spanner (Errors returned to preserve state).
* AP (Availability): Picked by Cassandra, TikTok Likes, DNS, Social Feeds (Always returns data, syncs later).
```

### 3. The PACELC Theorem (Beyond Network Partitions)
- **If Partition ($P$):** Choose between **Availability ($A$)** or **Consistency ($C$)**.
- **Else ($E$ - Normal operation):** Choose between **Latency ($L$)** or **Consistency ($C$)**.
  - *Example (MongoDB / DynamoDB):* In normal state, waiting for all replicas to acknowledge a write adds **Latency** to achieve **Consistency** ($PA/EL$ vs $PC/EC$).

---

# 3. Consistency Models: Strong vs. Eventual

```
+---------------------------------------------------------------------------------------------------+
| CONSISTENCY MODEL    | HOW IT BEHAVES                       | REAL-WORLD PRODUCT EXAMPLE          |
+---------------------------------------------------------------------------------------------------+
| Strong Consistency   | Every read is guaranteed to return   | Bank account balance transfer,      |
| (Linearizable)       | the most recent write immediately    | Airline seat reservation checkout   |
+---------------------------------------------------------------------------------------------------+
| Eventual Consistency | Reads may return stale data briefly; | YouTube video view count,           |
|                      | all replicas converge in seconds     | Twitter follower count, Reddit upvotes
+---------------------------------------------------------------------------------------------------+
| Read-Your-Own-Writes | A user is guaranteed to see their own| Posting a tweet or editing your own |
| Consistency          | updates immediately after submitting | profile biography                   |
+---------------------------------------------------------------------------------------------------+
| Monotonic Reads      | Once a user reads version V2, they   | Reading an online chat transcript   |
|                      | will never observe an older version V1| without messages disappearing       |
+---------------------------------------------------------------------------------------------------+
```

---

# 4. Latency vs. Throughput & Tail Latency (p99)

### 1. The "Aha!" Intuition
- **Latency:** How long it takes a single bullet train to travel from Tokyo to Osaka ($2\text{ hours}$).
- **Throughput:** How many total passengers the rail system delivers per hour ($50,000\text{ passengers/hr}$).
- **Tail Latency (p99):** The 1 passenger out of 100 whose ticket gets jammed at the turnstile, causing a 5-minute delay.

### 2. ASCII Architecture Flowchart: The Tail Latency Curve
```
Number of
Requests
   ^
   |        [ 50% of users: 5ms (p50) ]
   |            /\
   |           /  \      [ 90% of users: 20ms (p90) ]
   |          /    \          /\
   |         /      \        /  \            [ 1% of users: 800ms (p99 Tail Latency!) ]
   |        /        \______/    \_____________________/\___
   +----------------------------------------------------------> Latency (ms)
```

### 3. Why p99 Latency Dictates Large System Performance
In a microservices architecture (like Amazon.com), rendering one homepage makes **150 parallel backend API calls**. If each individual API call has a $1\%$ chance of an 800ms tail latency spike, the probability that your page load hits a slow call is:
$$1 - (0.99)^{150} \approx 78\%$$
**78% of all homepage visits will feel the slow p99 latency unless tail latency is aggressively optimized!**

---

# 5. High Availability & The "Nines" of Uptime

```
+---------------------------------------------------------------------------------------------------+
| AVAILABILITY LEVEL   | PERCENTAGE UPTIME | ALLOWABLE DOWNTIME PER YEAR  | WHERE IT IS USED        |
+---------------------------------------------------------------------------------------------------+
| 2 Nines              | 99.0%             | 3.65 Days                    | Internal dev tools      |
+---------------------------------------------------------------------------------------------------+
| 3 Nines (Industry Std)| 99.9%            | 8.76 Hours                   | Standard SaaS web apps  |
+---------------------------------------------------------------------------------------------------+
| 4 Nines (High Avail) | 99.99%            | 52.6 Minutes                 | E-Commerce checkout, AWS|
+---------------------------------------------------------------------------------------------------+
| 5 Nines (Mission-Crit)| 99.999%          | 5.26 Minutes                 | Telecoms, Stock Exch.   |
+---------------------------------------------------------------------------------------------------+
```

### Eliminating Single Points of Failure (SPOF):
- **Active-Passive (Warm Standby):** Secondary backup server sits idle; heartbeats monitor primary; takes 30 seconds to fail over via DNS/IP swap.
- **Active-Active:** Both servers actively process traffic simultaneously; if one dies, the load balancer shifts 100% traffic to the survivor with zero downtime.

---

# 6. Rate Limiting Algorithms & Redis Implementation

### 1. The "Aha!" Intuition
Think of a nightclub with a velvet rope. The bouncer has a bucket filled with 10 entry tokens. Every 6 seconds, the club owner drops 1 new token into the bucket. When a guest arrives, they hand 1 token to the bouncer. If the bucket is empty, guests must wait outside.

### 2. Algorithm Comparison Matrix
```
+---------------------------------------------------------------------------------------------------+
| ALGORITHM            | HOW IT WORKS                         | PROS                 | CONS                |
+---------------------------------------------------------------------------------------------------+
| Token Bucket         | Tokens refill at constant rate; each | Handles short bursty | Requires lock/sync  |
|                      | request consumes 1 token from bucket | traffic smoothly     | in distributed state|
+---------------------------------------------------------------------------------------------------+
| Leaky Bucket         | Requests enter a queue and leak out  | Smooths output rate  | Drops bursts if the |
|                      | at a strict constant processing speed| to a constant flow   | bucket queue fills  |
+---------------------------------------------------------------------------------------------------+
| Fixed Window Counter | Resets request counter every fixed   | Low memory overhead  | Boundary burst: 2x  |
|                      | minute (e.g. max 100 req per minute) |                      | limit at edge of min|
+---------------------------------------------------------------------------------------------------+
| Sliding Window       | Stores every request timestamp in a  | 100% accurate edge   | High memory: stores |
| Log                  | sorted set; counts timestamps in 60s | boundary enforcement | every timestamp     |
+---------------------------------------------------------------------------------------------------+
| Sliding Window       | Interpolates previous minute count + | Low memory + smooth  | Small probabilistic |
| Counter (BEST!)      | current minute count mathematically  | boundary enforcement | approximation (99%) |
+---------------------------------------------------------------------------------------------------+
```

### 3. Distributed Redis Rate Limiter via Atomic Lua Scripts
```
[ Client Request: IP = 1.2.3.4 ] ---> [ API Gateway ]
                                             |
                                             v
                      [ Execute Redis Lua Script (Atomic Execution!) ]
                      1. Remove timestamps older than 60 seconds (ZREMRANGEBYSCORE)
                      2. Count remaining elements in sorted set (ZCARD)
                      3. If count < Limit (100) -> ZADD current_timestamp -> Allow (HTTP 200)
                      4. Else -> Reject with HTTP 429 Too Many Requests
```

---

# 7. Resiliency Patterns: Circuit Breakers & Backoff

### 1. The "Aha!" Intuition
Your home electrical panel has a **circuit breaker**. If a faulty microwave draws too much current, the breaker trips open, shutting off power to that single room before the entire house catches on fire.

### 2. ASCII Architecture Flowchart: The Circuit Breaker State Machine
```
              +-------------------------------------------------------+
              |                                                       |
              v                                                       |
      [ CLOSED (Normal) ] ---(50% Requests Fail)----> [ OPEN (Broken) ]
              ^                                               |
              |                                     (Wait 30s Timeout)
              |                                               |
              |                                               v
              +-----(Test Pass!)--- [ HALF-OPEN (Testing) ] <-+
                                         |
                                   (Test Fail?)
                                         |
                                         +--------------------> [ Back to OPEN ]
```

### 3. Exponential Backoff with Jitter
When retrying a failed network request, never retry immediately every 1 second (which creates a retry stampede that kills the recovering server).
$$\text{Retry Delay} = 2^{\text{attempt}} + \text{Random Jitter}(0\text{--}500\text{ms})$$
- **Attempt 1:** Wait $2\text{s} + 120\text{ms}$
- **Attempt 2:** Wait $4\text{s} + 350\text{ms}$
- **Attempt 3:** Wait $8\text{s} + 50\text{ms}$

---

# 8. SQL vs. NoSQL vs. NewSQL Master Decision Matrix

```
+--------------------------------------------------------------------------------------------------------------------+
| DATABASE FAMILY      | POPULAR ENGINES            | PRIMARY DATA MODEL           | BEST INTERVIEW USE CASE         |
+--------------------------------------------------------------------------------------------------------------------+
| Relational (SQL)     | PostgreSQL, MySQL          | Structured Tables, ACID joins| Financial ledgers, E-commerce   |
|                      |                            | B+ Tree On-Disk Indexing     | orders, strict schemas          |
+--------------------------------------------------------------------------------------------------------------------+
| Document (NoSQL)     | MongoDB, Couchbase         | JSON / BSON Hierarchical     | Catalogs, Content Management,   |
|                      |                            | Semi-structured payloads     | User profiles with dynamic keys |
+--------------------------------------------------------------------------------------------------------------------+
| Key-Value (NoSQL)    | Redis, Memcached, DynamoDB | In-Memory Hash Maps, SkipList| Fast session stores, Caching,   |
|                      |                            | Key -> String / Binary Blob  | Real-time leaderboards          |
+--------------------------------------------------------------------------------------------------------------------+
| Wide-Column (NoSQL)  | Cassandra, ScyllaDB        | LSM-Trees, SSTables, Sparse  | High-throughput time-series,    |
|                      |                            | Multi-dimensional Tables     | Chat logs, IoT sensor telemetry |
+--------------------------------------------------------------------------------------------------------------------+
| Graph (NoSQL)        | Neo4j, Amazon Neptune      | Nodes, Edges, Adjacency lists| Social networks, Fraud rings,   |
|                      |                            | Zero-Index pointer traversal | Recommendation engines          |
+---------------------------------------------------------------------------------------------------+----------------+
| Distributed SQL      | Google Spanner, CockroachDB| Raft Consensus + TrueTime    | Global banking requiring ACID   |
| (NewSQL)             |                            | Relational tables over Paxos | across multi-region datacenters |
+--------------------------------------------------------------------------------------------------------------------+
```

---

# 9. Monolith vs. Microservices vs. Serverless

```
+---------------------------------------------------------------------------------------------------+
| ARCHITECTURE         | CORE MECHANICS                       | PROS                 | CONS                |
+---------------------------------------------------------------------------------------------------+
| Monolith             | Single unified codebase and single   | Easy testing, simple | Slow deployments,   |
|                      | shared relational database           | deployments, fast dev| scaling bottleneck  |
+---------------------------------------------------------------------------------------------------+
| Microservices        | Distributed independent services     | Autonomous teams,    | Network latency,    |
|                      | communicating over REST/gRPC         | independent scaling  | distributed tracing |
+---------------------------------------------------------------------------------------------------+
| Serverless           | Event-driven functions (AWS Lambda)  | Zero server managemnt| Cold start latency, |
|                      | running only on demand per request   | pay-per-execution    | 15-min timeout limit|
+---------------------------------------------------------------------------------------------------+
```

---

# 10. Glanceable Summary Matrix

```
+--------------------------------------------------------------------------------------------------------------------+
| CONCEPT / TRADEOFF   | THE PLAIN-ENGLISH MEANING        | WHEN TO PICK IN SYSTEM DESIGN INTERVIEW                  |
+--------------------------------------------------------------------------------------------------------------------+
| Horizontal Scaling   | Fleet of delivery vans           | Scaling stateless compute tiers to infinite traffic      |
+--------------------------------------------------------------------------------------------------------------------+
| CP (Consistency)     | Strict banking verification      | Financial ledgers, stock transactions, double-booking    |
+--------------------------------------------------------------------------------------------------------------------+
| AP (Availability)    | Show content now, reconcile later| Social media feeds, tweet likes, video view counts       |
+--------------------------------------------------------------------------------------------------------------------+
| p99 Tail Latency     | Slowest 1% request bottleneck    | Microservices where 1 slow API call delays full page     |
+--------------------------------------------------------------------------------------------------------------------+
| Token Bucket         | Nightclub token bucket           | Distributed rate limiting allowing small traffic bursts  |
+--------------------------------------------------------------------------------------------------------------------+
| Circuit Breaker      | Home electrical fuse             | Stopping cascading microservice crashes when downstream  |
+--------------------------------------------------------------------------------------------------------------------+
| Wide-Column (NoSQL)  | Write-optimized append log       | High-throughput IoT telemetry and Discord chat histories |
+--------------------------------------------------------------------------------------------------------------------+
```
