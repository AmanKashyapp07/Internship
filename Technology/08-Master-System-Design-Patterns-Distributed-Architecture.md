# Distributed System Design Patterns & Enterprise Architecture Reference

> **Scope:** Deep Architectural Analysis of Distributed System Patterns: Service Topologies (Modular Monoliths, Microservices), Load Balancing Mechanics (L4 vs. L7), Distributed Database Replication & Sharding (Consistent Hashing Rings), In-Memory Caching Strategies & Failure Modes, Distributed Transactions (Two-Phase Commit, Saga Orchestration, Transactional Outbox with Change Data Capture), Fault-Tolerance Patterns (Circuit Breakers, Bulkheads, Exponential Backoff with Jitter), and Rate Limiting Algorithms.

---

# Table of Contents
1. [Service Architecture Topologies: Monolith vs. Microservices vs. Modular Monolith](#1-service-architecture-topologies-monolith-vs-microservices-vs-modular-monolith)
2. [Traffic Routing & Load Balancing Architectures (L4 vs. L7)](#2-traffic-routing--load-balancing-architectures-l4-vs-l7)
3. [Distributed Database Replication Topologies](#3-distributed-database-replication-topologies)
4. [Database Partitioning & Consistent Hashing Rings](#4-database-partitioning--consistent-hashing-rings)
5. [In-Memory Caching Invariants & Failure Modes](#5-in-memory-caching-invariants--failure-modes)
6. [Distributed Transactions & Eventual Consistency Patterns](#6-distributed-transactions--eventual-consistency-patterns)
7. [Distributed Fault-Tolerance & Resilience Patterns](#7-distributed-fault-tolerance--resilience-patterns)
8. [Rate Limiting & Traffic Shaping Algorithms](#8-rate-limiting--traffic-shaping-algorithms)
9. [Master Distributed Systems Patterns Reference Matrix](#9-master-distributed-systems-patterns-reference-matrix)

---

# 1. Service Architecture Topologies: Monolith vs. Microservices vs. Modular Monolith

```
+----------------------------------------------------------------------------------------------------+
| TOPOLOGY             | STRUCTURAL MODEL                     | SCALING & OPERATIONAL TRADEOFF       |
+----------------------------------------------------------------------------------------------------+
| Monolith             | Single deployable binary and single  | Zero network serialization overhead; |
|                      | shared relational database instance  | deployment blast radius is total     |
| Modular Monolith     | Single deployable binary with strict | High cohesion; enables clean future  |
|                      | module boundaries enforced in code   | extraction without microservice ops  |
| Microservices        | Loosely coupled services interacting | Independent horizontal auto-scaling; |
|                      | via RPC / REST / Event Streams       | distributed tracing and latency hops |
+----------------------------------------------------------------------------------------------------+
```

---

# 2. Traffic Routing & Load Balancing Architectures (L4 vs. L7)

```
Layer 4 vs. Layer 7 Load Balancing Topologies:
L4 Transport Balancing: [ Ingress TCP Flow ] ---> [ IPVS / AWS NLB ] ---> Direct TCP Proxy (IP:Port)
L7 Application Balancing: [ Ingress HTTP ] ---> [ NGINX / Envoy ] ---> Path Routing & TLS Offload
```

- **Scheduling Algorithms:**
  - **Round Robin:** Uniform sequential distribution for homogeneous worker pools.
  - **Least Connections:** Routes to the node with the fewest active TCP connections.
  - **Power of Two Random Choices:** Picks two nodes at random and selects the one with the lighter load, avoiding thundering herd herd-synchronization.

---

# 3. Distributed Database Replication Topologies

```
+----------------------------------------------------------------------------------------------------+
| REPLICATION MODEL    | WRITE INGRESS                       | READ INGRESS          | CONCURRENCY MODEL    |
+----------------------------------------------------------------------------------------------------+
| Single-Leader        | 1 Primary node processes all writes | Distributed across N  | Read replication lag;|
| (Master-Replica)     | sequentially                        | read-only replicas    | read-your-own-writes |
| Multi-Leader         | Multiple primary nodes in distinct  | Local region reads    | Cross-region conflict|
| (Active-Active)      | regions accept writes concurrently  | with low latency      | resolution (LWW/CRDT)|
| Leaderless           | Writes accepted by any replica node | Quorum read/write:    | Eventual consistency;|
| (Dynamo / Cassandra) | via distributed quorum coordination | $W + R > N$           | no cross-row joins   |
+----------------------------------------------------------------------------------------------------+
```

---

# 4. Database Partitioning & Consistent Hashing Rings

Consistent Hashing maps database partitions and keys onto a continuous $2^{32}-1$ circular integer ring:

```
Consistent Hashing Ring with Virtual Nodes:
                        [ Node A_1 (Offset: 12) ]
                                /        \
            Key 1042           /          \         Key 8091
                *             /            \           *
                             /              \
    [ Node C_1 (Offset: 240) ] -------------- [ Node B_1 (Offset: 128) ]
```

- **Minimal Migration Property:** Scaling from $N$ to $N+1$ physical nodes requires migrating on average only $K / N$ keys.
- **Virtual Nodes (V-Nodes):** Allocates 100-200 pseudorandom ring positions per physical host to ensure balanced partition distribution.

---

# 5. In-Memory Caching Invariants & Failure Modes

```
+----------------------------------------------------------------------------------------------------+
| CACHING PATTERN      | WRITE BEHAVIOR                       | READ BEHAVIOR         | SYSTEM RISK          |
+----------------------------------------------------------------------------------------------------+
| Cache-Aside (Lazy)   | Updates database; invalidates/deletes| Queries cache; on miss| Temporary stale read |
|                      | corresponding cache key              | queries DB & sets key | race during updates  |
| Write-Through        | Synchronously writes to Cache, which | Reads directly from   | Higher write latency |
|                      | writes synchronously to Database     | cache                 | (Blocking DB update) |
| Write-Back (Behind)  | Writes to Cache only (Returns ACK);  | Reads directly from   | Potential data loss  |
|                      | flushes async batch updates to DB    | cache                 | on cache node crash  |
+----------------------------------------------------------------------------------------------------+
```

### Cache Failure Mitigations:
1. **Cache Stampede (Thundering Herd):** Hot key expiration triggers simultaneous database queries; mitigated via Distributed Mutex Locks (`SET lock:key NX EX 5`).
2. **Cache Penetration:** Queries for non-existent keys bypass cache to saturate the database; mitigated using in-memory **Bloom Filters** or caching null records with short TTLs.
3. **Cache Avalanche:** Mass key expiration at identical timestamps; mitigated by adding pseudorandom TTL Jitter ($TTL = \text{base} \pm \Delta t$).

---

# 6. Distributed Transactions & Eventual Consistency Patterns

### 1. Two-Phase Commit (2PC) vs. Saga Pattern
- **Two-Phase Commit (2PC):** Centralized coordinator issues `PREPARE` and `COMMIT` phases across participant databases, holding blocking locks. Poor performance across wide-area networks.
- **Saga Pattern:** Decomposes a distributed transaction into a sequence of local transactions across microservices, coordinating via **Orchestration** (central workflow state machine) or **Choreography** (event-driven pub/sub). Failures execute explicit **Compensating Transactions** to roll back partial state.

### 2. Transactional Outbox Pattern with Change Data Capture (CDC)
Guarantees reliable atomic message publishing alongside database updates without distributed locks:

```
Transactional Outbox Architecture:
[ Business Service ] ---> 1. Atomic DB Transaction:
                              - INSERT INTO orders (...)
                              - INSERT INTO outbox_table (event_payload)
                                  |
                                  v (Tail Transaction Commit Log via CDC)
                     [ Debezium CDC Connector ] ---> 2. Stream to Apache Kafka
```

---

# 7. Distributed Fault-Tolerance & Resilience Patterns

```
Circuit Breaker State Transitions:
[ CLOSED (Normal Traffic) ] ---(Failure Rate >= Threshold)---> [ OPEN (Fail Fast / Fallback) ]
             ^                                                              |
             |                                                    (Sleep Window Expired)
             |                                                              |
             |                                                              v
             +-----(Success Rate >= Threshold)----- [ HALF-OPEN (Canary Trial Probe) ]
```

- **Bulkhead Pattern:** Isolates thread pools and connection pools by critical domain (e.g. separate thread pools for Payment vs. Search) so a failure in a non-critical subsystem cannot exhaust global application memory or worker threads.
- **Exponential Backoff with Decorrelated Jitter:**
  $$t_{\text{retry}} = \min(t_{\text{max}}, \, 2^{\text{attempt}} \times t_{\text{base}}) + \text{rand}(0, \, J)$$

---

# 8. Rate Limiting & Traffic Shaping Algorithms

```
+----------------------------------------------------------------------------------------------------+
| ALGORITHM            | BEHAVIORAL MECHANISM                 | STORAGE COMPLEXITY   | BURST BEHAVIOR       |
+----------------------------------------------------------------------------------------------------+
| Token Bucket         | Refills tokens at constant rate $r$; | O(1) space per key   | Supports controlled  |
|                      | request consumes 1 token from bucket |                      | burst traffic        |
| Leaky Bucket         | Requests enter FIFO queue; egresses  | O(QueueSize) space   | Zero burst; smooth   |
|                      | at constant smooth processing speed  | per consumer         | output rate          |
| Sliding Window       | Computes weighted sum of previous    | Low O(1) memory      | Smooth boundary      |
| Counter (Approx)     | and current window request counters  | footprint            | approximation        |
+----------------------------------------------------------------------------------------------------+
```

---

# 9. Master Distributed Systems Patterns Reference Matrix

```
+----------------------------------------------------------------------------------------------------+
| PATTERN NAME         | PRIMARY PROBLEM SOLVED               | CORE GOVERNING INVARIANT             |
+----------------------------------------------------------------------------------------------------+
| Consistent Hashing   | Horizontal cluster sharding          | Minimal key migration: K/N keys moved|
| Transactional Outbox | Dual-write inconsistency to DB/Kafka | Atomic DB transaction + CDC streaming|
| Saga Orchestration   | Distributed multi-service transactions| Compensating rollback actions        |
| Circuit Breaker      | Cascading microservice failures      | Closed -> Open -> Half-Open FSM      |
| Token Bucket Limiter | API abuse & DDoS mitigation          | Constant refill rate with burst cap  |
| Bloom Filter Shield  | Cache penetration database overload  | Zero false negatives set membership  |
| Read-Your-Own-Writes | Replication lag read inconsistency   | Primary routing window post-write    |
+----------------------------------------------------------------------------------------------------+
```
