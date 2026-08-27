# Types of System Design Patterns — Interview Notes

A guide to speak about distributed architecture patterns, scaling strategies,
resilience mechanisms, and data consistency models with the confidence of an engineer
who has designed, partitioned, and operated high-availability production systems.

---

## What They Are & Why They Are Needed

### 1. What Are System Design Patterns?
System design patterns are battle-tested architectural blueprints for solving
fundamental distributed systems challenges: handling massive traffic scale, ensuring
fault tolerance, managing concurrent data consistency, and decoupling complex software systems.

```
+---------------------------------------------------------------------------------------------------+
| DISTRIBUTED SYSTEM GOALS            | CORE ARCHITECTURAL PATTERNS APPLIED                         |
+---------------------------------------------------------------------------------------------------+
| 1. High Availability (99.999% SLA)  | Database Replication, Anycast DNS, Active-Active Failover   |
| 2. Horizontal Scalability           | Load Balancing, Consistent Hashing Sharding, Microservices  |
| 3. Sub-50ms Latency                 | Multi-Tier Caching (Cache-Aside), Edge CDNs, Read Replicas  |
| 4. Fault Isolation & Self-Healing   | Circuit Breakers, Bulkheads, Dead Letter Queues (DLQ)       |
| 5. Distributed Data Correctness     | Saga Pattern (Choreography/Orchestration), Outbox + CDC     |
+---------------------------------------------------------------------------------------------------+
```

### 2. Why Are They Needed?
1. **The Physical Limits of Single Machines:** Vertical scaling (buying bigger CPU/RAM) hits hard financial and hardware ceilings; horizontal scaling across clusters of machines is mandatory for modern web scale.
2. **Network Partitions & Hardware Failures:** In distributed systems, networks will drop packets, hard drives will crash, and cloud VM instances will terminate without warning. Systems must be engineered to survive failure by design.
3. **Traffic Spikes & Hotspots:** Viral events (flash sales, breaking news) can multiply read/write traffic by 100x in seconds; systems require load distribution, caching, and sharding to prevent complete cascading collapses.

---

## 1. Monolith vs. Microservices vs. Modular Monolith

**What it is:** The foundational structural decision of how software codebases,
deployment boundaries, and team domains are organized.

**Strengths**
- **Monolith:** Single codebase, zero network serialization latency between modules,
  simple ACID database transactions, and effortless local debugging and deployments.
- **Microservices:** Independent deployment units, independent horizontal auto-scaling
  per service, decoupled tech stacks, and team autonomy aligned with domain boundaries (Conway's Law).
- **Modular Monolith:** Clean domain boundaries enforced in code (separate modules/packages)
  while keeping a single deployable binary and single shared database.

**Weaknesses**
- Monolith: Blended deployment risks (one bad commit breaks everything), long CI/CD build times,
  and difficulty scaling individual resource-intensive components independently.
- Microservices: Distributed latency overhead ($T_{total} = T_1 + T_2 + T_3$), network failure
  modes, complex observability (distributed tracing), and eventual consistency data sync challenges.

**When to use:** Monolith / Modular Monolith for startups, MVPs, and small teams (<20 engineers);
Microservices when organizational scale (multiple independent feature teams) demands independent deployments.

**Interview-ready line:** "Microservices are an organizational scaling solution, not a performance optimization —
I default to a Modular Monolith until team size or divergent scaling requirements justify the distributed complexity."

---

## 2. Load Balancing (Layer 4 vs. Layer 7 & Algorithms)

**What it is:** The practice of distributing incoming network traffic across a pool
of backend application servers to prevent any single node from becoming a bottleneck.

**Strengths**
- **Layer 4 (Transport Layer - TCP/UDP):** Extremely fast packet routing (millions of pkts/sec)
  based on IP address and port without inspecting HTTP payloads (e.g. AWS NLB, IPVS, HAProxy L4).
- **Layer 7 (Application Layer - HTTP/HTTPS):** Intelligent routing based on URL paths (`/api/users`
  vs `/api/billing`), HTTP headers, cookies, and TLS termination (e.g. Nginx, AWS ALB, Envoy).
- **Key Algorithms:**
  - *Round Robin / Weighted:* Distributes requests sequentially; best for uniform stateless requests.
  - *Least Connections:* Routes to the server with the fewest active in-flight requests; best for long-lived WebSocket sessions or heavy tasks.
  - *Consistent Hashing / IP Hash:* Hashes client IP or session key to the same server for session stickiness.

**Weaknesses**
- Single point of failure if the load balancer itself is not configured in High Availability
  pairs (active-passive VRRP/Keepalived or DNS Anycast).
- Layer 7 introduces slight CPU parsing latency compared to raw Layer 4 packet forwarding.

**When to use:** In front of every stateless web/API server fleet, microservice mesh,
and database read replica pool.

**Interview-ready line:** "I use Layer 4 load balancing at the outer edge for raw TCP throughput
and DDoS mitigation, and Layer 7 at the ingress for intelligent path-based microservice routing and SSL termination."

---

## 3. Database Replication (Primary-Replica vs. Multi-Leader vs. Leaderless)

**What it is:** Copying and synchronizing data across multiple physical database
nodes to ensure fault tolerance, disaster recovery, and high read throughput.

**Strengths**
- **Single-Leader (Primary-Replica):** All writes go to 1 Primary node and replicate
  asynchronously or semi-synchronously to multiple Read Replicas (PostgreSQL, MySQL). Simple consistency model.
- **Multi-Leader (Active-Active):** Multiple primary nodes accept writes (e.g. across multiple geographic datacenters);
  low-latency local writes worldwide.
- **Leaderless (Dynamo Model):** Any node accepts reads and writes using **Tunable Quorums ($W + R > N$)**
  and background anti-entropy repairs (Cassandra, Amazon DynamoDB).

**Weaknesses**
- Single-Leader: **Replication Lag** — reading from a replica immediately after writing to the
  primary can return stale data (*Read-Your-Own-Writes inconsistency*).
- Multi-Leader: **Write Conflicts** — concurrent writes to the same row in different datacenters
  require complex conflict resolution (Last-Write-Wins timestamps or CRDTs).
- Leaderless: Eventual consistency requires application-level conflict handling and reads during partitions.

**When to use:** Single-Leader for standard read-heavy transactional apps; Multi-Leader for multi-region
disaster tolerance; Leaderless for massive continuous write streams across global clusters.

**Interview-ready line:** "For standard web workloads, Single-Leader replication with async read replicas
is ideal — to prevent stale reads right after a user action, we route reads to the Primary for 5 seconds after a mutation."

---

## 4. Database Partitioning & Sharding (Consistent Hashing)

**What it is:** Breaking a massive database table into smaller, independent subsets
(shards) distributed across multiple database servers to scale past single-node RAM and storage limits.

**Strengths**
- Scales write throughput and storage linearly across dozens of physical database instances.
- **Sharding Strategies:**
  - *Range-Based:* Partitions by ranges (e.g. User IDs 1–1M $\to$ Shard 1); simple but causes **Hotspotting** on recent IDs.
  - *Hash / Modulo-Based:* Shard ID = `hash(key) % N`; evenly distributes writes, but adding a new node requires resharding 100% of data.
  - *Consistent Hashing with Virtual Nodes:* Maps keys and nodes onto a $2^{32}$ circular ring. Adding/removing a node migrates only $K/N$ keys on average, while **Virtual Nodes** prevent uneven load distribution.

**Weaknesses**
- **Cross-Shard Queries (Scatter-Gather):** Queries that do not filter by the Shard Key must
  query all shards in parallel and merge results, which destroys latency.
- Cross-shard transactions and joins are extremely expensive or unsupported.

**When to use:** When a single database instance exceeds 1TB+ storage, maxes out IOPS,
or write throughput exceeds the limits of single-node hardware.

**Interview-ready line:** "Consistent Hashing with Virtual Nodes is the gold standard for sharding —
it prevents write hotspots and ensures that scaling up the cluster moves only a tiny fraction of data rather than re-indexing everything."

---

## 5. Caching Strategies & Cache Invalidation

**What it is:** Storing expensive-to-compute or frequently accessed data in ultra-fast
in-memory storage (Redis / Memcached) to reduce database load and cut latencies to sub-milliseconds.

**Strengths**
- **Cache-Aside (Lazy Loading):** Application queries cache first; on cache miss, reads database,
  writes to cache, and returns. Extremely resilient (cache failure doesn't kill the DB).
- **Write-Through:** Application writes to cache, and cache synchronously writes to database before returning.
  Data is always fresh.
- **Write-Back (Write-Behind):** Application writes to cache, which acknowledges immediately and
  asynchronously batches writes to the database. Delivers massive write performance.

**Weaknesses**
- **Cache Invalidation:** *"There are only two hard things in Computer Science: cache invalidation and naming things."*
- **Cache Stampede (Thundering Herd):** When a popular key expires, thousands of concurrent requests
  miss the cache simultaneously and hammer the database.
- **Cache Penetration:** Requests for non-existent keys bypass the cache and hit the database repeatedly.

**When to use:** Read-heavy workloads (>80% reads), user sessions, live leaderboards,
and pre-computed dashboard aggregations.

**Interview-ready line:** "I use Cache-Aside as the default pattern, defend against Thundering Herds
using Mutex locks or TTL jitter, and use Bloom Filters to prevent Cache Penetration for non-existent keys."

---

## 6. The CAP & PACELC Theorems

**What it is:** The governing mathematical laws of distributed data stores.

**Strengths**
- **CAP Theorem:** In a network partition (**P**), you must choose between **Consistency (CP)**
  (returning an error or waiting for sync) OR **Availability (AP)** (returning stale data immediately).
- **PACELC Theorem:** Extends CAP by defining normal (non-partition) operation:
  *"If there is a Partition (**P**), trade off Availability (**A**) vs. Consistency (**C**); **E**lse, trade off Latency (**L**) vs. Consistency (**C**)."*
  - *PC/EC (e.g. CockroachDB, Spanner):* Favors consistency under partitions AND normal operation.
  - *PA/EL (e.g. Cassandra, DynamoDB):* Favors availability under partitions AND low latency under normal operation.

**Weaknesses**
- Misunderstanding CAP as a static choice: modern distributed stores let you tune consistency
  per query (e.g. Cassandra's `LOCAL_QUORUM` vs `ONE`).

**When to use:** Evaluating database selections during system design and architecting cross-region replication.

**Interview-ready line:** "PACELC gives a far more realistic picture than CAP — it forces you
to acknowledge that even during normal operation with zero network failures, you are constantly trading off Latency for Strong Consistency."

---

## 7. Resiliency Patterns: Circuit Breakers, Bulkheads & Rate Limiters

**What it is:** Defensive patterns that prevent local component failures from
cascading into catastrophic cluster-wide outages.

**Strengths**
- **Circuit Breaker (Martin Fowler State Machine):**
  - *Closed:* Normal operation, requests pass through.
  - *Open:* Error rate exceeds threshold (e.g. 50% failures); requests fail immediately with a fallback response without calling the downstream service.
  - *Half-Open:* After a cooldown period, lets a few trial requests through; closes if successful, re-opens if failed.
- **Bulkhead Pattern:** Isolates thread pools and connection pools per downstream service so a slow service cannot exhaust all available server threads.
- **Rate Limiting:** Prevents API abuse using Token Bucket, Leaky Bucket, or Sliding Window algorithms.

**Weaknesses**
- Requires thoughtful fallback design (returning cached data or graceful degradation).
- Misconfigured thresholds can cause circuit breakers to trip prematurely during temporary network blips.

**When to use:** Any microservice making synchronous HTTP/gRPC calls to downstream services
or third-party external APIs (Stripe, Twilio, OpenAI).

**Interview-ready line:** "Circuit breakers stop cascading failures dead in their tracks —
when a downstream service starts failing, the circuit opens immediately to allow the dependency to recover while returning a graceful fallback."

---

## 8. Distributed Consistency: Saga Pattern vs. Two-Phase Commit (2PC)

**What it is:** Architectures for maintaining transactional data consistency across
multiple independent databases and microservices without traditional monolithic ACID locks.

**Strengths**
- **Two-Phase Commit (2PC):** A central coordinator runs a *Prepare Phase* followed by a *Commit Phase*.
  Guarantees strict ACID consistency across distributed databases.
- **Saga Pattern (Eventual Consistency):** Breaks a distributed transaction into a sequence of
  local transactions. If any step fails, the Saga executes **Compensating Transactions** in reverse order to undo changes.
  - *Choreography:* Services publish and listen to events (decentralized, best for simple workflows).
  - *Orchestration:* A central Orchestrator state machine instructs services step-by-step (best for complex checkout flows).
- **Transactional Outbox Pattern:** Writes domain data AND an event record atomically in the same
  local database transaction, letting a CDC tool (Debezium) publish to Kafka with zero dual-write race conditions.

**Weaknesses**
- 2PC is a blocking protocol: if the coordinator crashes during the prepare phase, resources remain locked indefinitely, destroying throughput.
- Sagas are eventually consistent: intermediate states are visible to other users, requiring careful business logic design.

**When to use:** Sagas for microservice e-commerce checkout flows and order lifecycles; Outbox Pattern for reliable database-to-queue publishing.

**Interview-ready line:** "In microservices, 2PC is an anti-pattern because synchronous distributed locking kills availability — I use Orchestrated Sagas with compensating transactions paired with the Transactional Outbox pattern."

---

## Side-by-Side Comparison

| Pattern | Primary Goal | Core Trade-off | Key Tech / Mechanism | Primary Failure Risk |
|---|---|---|---|---|
| **Microservices** | Team autonomy & independent scale | Network latency & data consistency | Docker, K8s, gRPC, API Gateway | Cascading outages, distributed debugging |
| **Consistent Hashing** | Even sharding with minimal rebalancing | Implementation complexity | Virtual Nodes, Hash Ring ($2^{32}$) | Hotspotting without virtual nodes |
| **Primary-Replica Sync**| Read scaling & disaster recovery | Replication lag (stale reads) | Postgres WAL, MySQL Binlog | Split-brain during failover |
| **Cache-Aside** | Sub-ms read latency | Stale cache & invalidation | Redis, Memcached, TTLs | Cache Stampede / Thundering Herd |
| **Circuit Breaker** | Cascading failure prevention | Fallback design overhead | Closed $\to$ Open $\to$ Half-Open | Premature tripping on temporary blips |
| **Orchestrated Saga** | Distributed transactions | Eventual consistency (dirty reads) | State Machine + Compensating Txns | Missing compensations on edge crashes |
| **Transactional Outbox**| Reliable event emission | Secondary polling / CDC pipeline | Debezium, Kafka Connect, Postgres | CDC pipeline lag |

---

## Decision Framework (say this out loud in interviews)

1. **System is under 20 engineers or launching an MVP** → Start with a Modular Monolith on PostgreSQL.
2. **Read traffic is 10x write traffic and queries are repetitive** → Add Redis with Cache-Aside + TTL Jitter.
3. **Database single-node storage or write throughput maxed out** → Shard horizontally using Consistent Hashing with Virtual Nodes.
4. **Third-party dependency or downstream service is failing intermittently** → Wrap downstream calls in a Circuit Breaker with fallback responses.
5. **Executing a financial transaction across 3 independent microservices** → Orchestrated Saga with Compensating Transactions.
6. **Need to update the database AND publish an event to Kafka with zero data loss** → Transactional Outbox Pattern with Debezium CDC.
7. **Designing a global multi-region system** → PACELC evaluation: choose PA/EL (Cassandra) for write scale, or PC/EC (Spanner) for financial ledgers.

---

## What separates a senior answer from a junior one

### 1. Microservices as an Organizational Tool (Conway's Law)
- **Junior:** "Microservices are always better because they are modern and scalable."
- **Senior:** *"Microservices are a tool to solve **organizational scaling, not code performance**. Following Conway's Law, when an engineering org exceeds 50+ engineers, a monolith creates deployment bottlenecks and merge conflicts. Splitting into microservices buys you team autonomy and independent deployment velocity, but at the cost of distributed tracing complexity, network latency, and eventual consistency challenges. A senior defaults to a **Modular Monolith** until team boundaries demand the split."*

### 2. Consistent Hashing Physics & Virtual Nodes
- **Junior:** "We shard the database by taking `user_id % num_servers`."
- **Senior:** *"Modulo sharding is catastrophic at scale because adding a single new server changes the denominator, forcing **$O(N)$ data migration of almost 100% of keys**. A senior uses **Consistent Hashing on a $2^{32}$ circular ring**, where adding a node moves only $K/N$ keys on average. Furthermore, a senior always implements **Virtual Nodes (vnodes)**: by assigning multiple virtual points on the ring to each physical machine, you ensure uniform data distribution and prevent non-uniform write hotspots."*

### 3. The Dual-Write Race Condition & Transactional Outbox
- **Junior:** "When an order is created, our API saves the order to PostgreSQL and then immediately calls `kafkaProducer.send()`."
- **Senior:** *"This is a classic **Dual-Write Anti-Pattern** that guarantees data corruption:
  - If the database commit succeeds but the network drops before Kafka acknowledges, the event is permanently lost.
  - If Kafka succeeds but the database transaction rolls back, you emitted a phantom ghost event.
  - A senior solves this using the **Transactional Outbox Pattern**: the order and an `outbox_event` record are written in the **exact same ACID local database transaction**. An asynchronous Change Data Capture (CDC) tool like Debezium streams outbox commits to Kafka reliably with zero dual-write vulnerabilities."*

### 4. Cache Stampedes, Thundering Herds & Penetration Defenses
- **Junior:** "We set a 1-hour expiration time on all cache keys."
- **Senior:** *"Setting identical TTLs creates a **Cache Stampede (Thundering Herd)**: when the key expires, thousands of concurrent requests miss the cache simultaneously and overwhelm the database. A senior defends against this systematically:
  1. **TTL Jitter:** Add a random delta to expirations (`TTL = 3600 + rand(0, 300)`) so keys expire staggered.
  2. **Mutex Locking:** The first request acquiring a distributed Redis lock queries the DB and repopulates the cache, while subsequent requests wait.
  3. **Bloom Filters:** Place a Bloom Filter in front of the cache to immediately reject requests for IDs that do not exist, eliminating **Cache Penetration**."*

### 5. PACELC in Real-World Global Architectures
- **Junior:** "Our system is CAP-Available, so it's always fast and never goes down."
- **Senior:** *"A senior evaluates the **PACELC Theorem**: even when there are zero network partitions, you are constantly trading off **Latency (L) vs. Consistency (C)**. In a multi-region deployment:
  - If you want immediate global consistency, every write must wait for cross-oceanic synchronous consensus handshakes ($T_{latency} > 250\text{ms}$).
  - If you want sub-20ms write latency, you must accept local async replication and eventual consistency.
  - A senior maps business requirements: financial ledgers use strong consistency (Spanner / PC/EC), while user notifications and activity feeds use eventual consistency (Cassandra / PA/EL)."*
