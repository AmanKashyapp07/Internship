# Distributed Systems Tradeoffs, Fault Tolerance & Reliability Engineering

> **Scope:** Horizontal vs. Vertical Elastic Scaling, CAP Theorem Formal Proofs & PACELC Taxonomy, Distributed Consistency Spectrum, Latency vs. Throughput & Microservice Tail Latency Amplification ($p99$), High Availability SLA Mathematical Models, Rate Limiting Algorithms & Distributed Lua Evaluators, Fault-Tolerance State Machines (Circuit Breakers, Exponential Backoff with Jitter, Bulkheads), Database Taxonomy (SQL, NoSQL, NewSQL Distributed Spanner Consensus), and Service Topology Tradeoffs.

---

# Table of Contents
1. [Horizontal vs. Vertical Elastic Scaling Models](#1-horizontal-vs-vertical-elastic-scaling-models)
2. [CAP Theorem Formalism & The PACELC Framework](#2-cap-theorem-formalism--the-pacelc-framework)
3. [The Distributed Consistency Spectrum](#3-the-distributed-consistency-spectrum)
4. [Latency, Throughput & Tail Latency Amplification ($p99$)](#4-latency-throughput--tail-latency-amplification-p99)
5. [High Availability SLAs & Uptime Mathematics](#5-high-availability-slas--uptime-mathematics)
6. [Rate Limiting Algorithms & Distributed Token Buckets](#6-rate-limiting-algorithms--distributed-token-buckets)
7. [Distributed Fault-Tolerance Patterns & State Machines](#7-distributed-fault-tolerance-patterns--state-machines)
8. [Database Engine Taxonomy: SQL vs. NoSQL vs. NewSQL](#8-database-engine-taxonomy-sql-vs-nosql-vs-newsql)
9. [Service Architecture Topologies: Monolith vs. Microservices vs. Serverless](#9-service-architecture-topologies-monolith-vs-microservices-vs-serverless)
10. [Core Architectural Summary Matrix](#10-core-architectural-summary-matrix)

---

# 1. Horizontal vs. Vertical Elastic Scaling Models

```
Vertical Scaling (Scale-Up):
[ Host: 4 Cores, 16GB RAM ] ---> [ Hardware Upgrade ] ---> [ Host: 128 Cores, 2TB RAM ]
                                                            (Bounded by motherboard limits)

Horizontal Scaling (Scale-Out):
                                     +---> [ Compute Node 1 ]
[ Ingress Traffic ] ---> [ L4/L7 LB ] ---> [ Compute Node 2 ] ---> [ Dynamic Elastic Expansion ]
                                     +---> [ Compute Node 3 ]
```

```
+----------------------------------------------------------------------------------------------------+
| ATTRIBUTE            | VERTICAL SCALING (Scale-Up)          | HORIZONTAL SCALING (Scale-Out)       |
+----------------------------------------------------------------------------------------------------+
| Scaling Vector       | Expand CPU, RAM, and NVMe on 1 node  | Add commodity compute nodes to pool  |
| Hardware Limit       | Rigid physical motherboard ceiling   | Practically unbounded linear scaling |
| Downtime Requirement | Hardware replacement reboot cycles   | Zero downtime via rolling deployments|
| Architectural Impact | Trivial (Retains single-node state)  | Requires stateless service instances |
| Cost Trajectory      | Superlinear (Exponential at top spec)| Linear predictable commodity scaling |
+----------------------------------------------------------------------------------------------------+
```

---

# 2. CAP Theorem Formalism & The PACELC Framework

The **CAP Theorem** dictates that in the presence of an asynchronous network partition ($P$), a distributed system must trade off between Consistency ($C$) and Availability ($A$).

```
Distributed Partition Split Scenario:
[ Node Group East ] <======== Network Split (Partition P) ========> [ Node Group West ]
- CP Option: Reject writes on minority partition to guarantee global data consistency.
- AP Option: Accept writes on all nodes; resolve data diverges asynchronously post-heal.
```

### The PACELC Extension (Abadi's Formulation):
The CAP theorem evaluates system behavior strictly during abnormal network partitions. The **PACELC Theorem** models trade-offs across all operating conditions:
- **If Partition ($P$):** Choose between **Availability ($A$)** or **Consistency ($C$)**;
- **Else ($E$ - Normal State):** Choose between **Latency ($L$)** or **Consistency ($C$)**.

```
+----------------------------------------------------------------------------------------------------+
| PACELC CLASS         | SYSTEM BEHAVIOR                      | EXEMPLAR ENGINES                     |
+----------------------------------------------------------------------------------------------------+
| PC / EC              | Guarantees consistency during splits | Google Spanner, CockroachDB,         |
|                      | and waits for replica sync normally  | PostgreSQL Synchronous Multi-Node    |
| PA / EL              | Yields availability during splits and| Apache Cassandra, Amazon DynamoDB,   |
|                      | optimizes for latency in normal state| ScyllaDB, Couchbase                  |
| PC / EL              | Consistent during partitions, but    | MongoDB (Configured with primary     |
|                      | optimizes for read latency normally  | write concern and secondary reads)   |
+----------------------------------------------------------------------------------------------------+
```

---

# 3. The Distributed Consistency Spectrum

```
+----------------------------------------------------------------------------------------------------+
| CONSISTENCY MODEL    | FORMAL BEHAVIORAL GUARANTEE          | DISTRIBUTED PRIMITIVE                |
+----------------------------------------------------------------------------------------------------+
| Linearizable         | Every operation appears to execute   | Paxos / Raft State Machine           |
| (Strict / Strong)    | instantaneously at a global timestamp| Replication with synchronous barriers|
| Sequential           | Operations take effect in an order   | Lamport Logical Timestamps           |
|                      | consistent across all observer nodes |                                      |
| Causal               | Causally related operations appear in| Vector Clocks and Version Vectors    |
|                      | identical order to all participants  |                                      |
| Read-Your-Own-Writes | A client process will always observe | Primary routing windows or           |
|                      | the effects of its own prior writes  | client-tracked mutation timestamps   |
| Monotonic Reads      | Repeated reads by a client observe   | Deterministic session-to-replica     |
|                      | monotonically non-decreasing states  | affinity routing                     |
| Eventual             | Given zero new mutations, all replica| Anti-Entropy Gossip protocols &      |
|                      | states converge to identical values  | Last-Write-Wins (LWW) resolution     |
+----------------------------------------------------------------------------------------------------+
```

---

# 4. Latency, Throughput & Tail Latency Amplification ($p99$)

### Latency vs. Throughput:
- **Latency:** Time interval required to execute a single atomic computation or network roundtrip ($ms$ or $\mu s$).
- **Throughput:** Cumulative units of computation, transactions, or bytes processed per second ($\text{RPS} / \text{QPS}$).

```
Tail Latency Probability Distribution:
Request
Volume
   ^
   |        [ p50 Median: 4ms ]
   |             /\
   |            /  \      [ p95: 18ms ]
   |           /    \         /\
   |          /      \_______/  \                    [ p99 Tail Spike: 650ms ]
   |         /                   \___________________________/\___
   +----------------------------------------------------------------> Latency (ms)
```

### Microservice Tail Latency Amplification Law:
In a distributed fan-out architecture where a single root request executes $M$ parallel downstream RPC calls, each having a tail latency spike probability $p = 1 - 0.99 = 0.01$ ($p99$):
$$P(\text{At least 1 slow downstream call}) = 1 - (1 - p)^M$$
- For $M = 100$ parallel microservice RPCs:
  $$P(\text{Root request encounters } p99) = 1 - (0.99)^{100} = 1 - 0.366 = \mathbf{63.4\%}$$
- For $M = 150$ parallel microservice RPCs:
  $$P(\text{Root request encounters } p99) = 1 - (0.99)^{150} = \mathbf{77.9\%}$$

### Mitigations:
1. **Hedged Requests:** Issue duplicate read requests to secondary replicas if the primary does not respond within the $p95$ threshold, utilizing whichever returns first.
2. **Tied Requests & Speculative Cancellation:** Enqueue request simultaneously in multiple worker queues; when execution begins on one node, broadcast cancellations to the rest.

---

# 5. High Availability SLAs & Uptime Mathematics

$$\text{Availability } A = \frac{\text{MTBF}}{\text{MTBF} + \text{MTTR}} \times 100\%$$
- **MTBF:** Mean Time Between Failures.
- **MTTR:** Mean Time To Recovery.

```
+----------------------------------------------------------------------------------------------------+
| AVAILABILITY LEVEL   | PERCENTAGE UPTIME | MAXIMUM PERMISSIBLE DOWNTIME PER CALENDAR YEAR          |
+----------------------------------------------------------------------------------------------------+
| 2 Nines              | 99.0%             | 3 days, 15 hours, 39 minutes                            |
| 3 Nines (Standard)   | 99.9%             | 8 hours, 45 minutes, 57 seconds                         |
| 4 Nines (High Avail) | 99.99%            | 52 minutes, 35 seconds                                  |
| 5 Nines (Fault-Tol)  | 99.999%           | 5 minutes, 15 seconds                                   |
+----------------------------------------------------------------------------------------------------+
```

---

# 6. Rate Limiting Algorithms & Distributed Token Buckets

```
+----------------------------------------------------------------------------------------------------+
| ALGORITHM            | BEHAVIORAL MECHANICS                 | MEMORY & COMPLEXITY  | BURST CAPACITY|
+----------------------------------------------------------------------------------------------------+
| Token Bucket         | Tokens refill at constant rate $r$;  | O(1) space per key   | Configurable  |
|                      | request consumes 1 token from bucket | (Tokens + LastUpdate)| burst capacity|
| Leaky Bucket         | Requests enter FIFO buffer; leak out | O(QueueSize) space   | Zero burst;   |
|                      | at constant smooth egress rate       | per consumer         | strict outflow|
| Fixed Window         | Increments counter for time block;   | Minimal O(1) space   | Boundary spike|
|                      | resets at start of every window      | per window interval  | allows 2x QPS |
| Sliding Window Log   | Retains exact timestamps in sorted   | High O(N) memory     | Perfect edge  |
|                      | set; counts entries in past interval | proportional to reqs | precision     |
| Sliding Window       | Computes weighted sum of previous    | Low O(1) memory      | Smooth burst  |
| Counter (Approx)     | and current window counters          | footprint per key    | mitigation    |
+----------------------------------------------------------------------------------------------------+
```

```
Distributed Redis Rate Limiting (Atomic Sliding Log Lua Script):
1. Key = "ratelimit:" .. client_ip
2. redis.call('ZREMRANGEBYSCORE', Key, 0, current_time - window_size)
3. local current_requests = redis.call('ZCARD', Key)
4. if current_requests < max_limit then
       redis.call('ZADD', Key, current_time, current_time)
       redis.call('EXPIRE', Key, window_size)
       return 1 -- Request Approved (HTTP 200)
   else
       return 0 -- Rate Limit Exceeded (HTTP 429)
   end
```

---

# 7. Distributed Fault-Tolerance Patterns & State Machines

### 1. The Circuit Breaker Finite State Machine (Martin Fowler Model)

```
        +----------------------------------------------------------------+
        |                                                                |
        v                                                                |
[ CLOSED (Normal) ] ---(Failure Rate >= Threshold)---> [ OPEN (Fail Fast) ]
        ^                                                        |
        |                                                (Sleep Window Expired)
        |                                                        |
        |                                                        v
        +-----(Success >= Threshold)----- [ HALF-OPEN (Canary Trial) ] <+
                                                 |
                                         (Single Failure)
                                                 |
                                                 +-----------------------+
```

### 2. Exponential Backoff with Decorrelated Jitter
Retrying failed network requests requires randomized exponential spacing to avoid synchronization stampedes:
$$t_{\text{backoff}} = \min(t_{\text{max}}, \, 2^{\text{attempt}} \times t_{\text{base}}) + \text{rand}(0, \, J)$$

---

# 8. Database Engine Taxonomy: SQL vs. NoSQL vs. NewSQL

```
+----------------------------------------------------------------------------------------------------+
| DATABASE FAMILY      | STORAGE ENGINE & INDEXING            | SCHEMA & CONSTRAINTS | CONSENSUS / ACID     |
+----------------------------------------------------------------------------------------------------+
| Relational SQL       | B+ Trees, WAL, Heap Files            | Strict relational    | Single-node ACID /   |
| (Postgres, MySQL)    | (Page-oriented on-disk blocks)       | schema with FK joins | 2PC distributed joins|
| Document NoSQL       | B-Trees / WiredTiger                 | Dynamic hierarchical | Single-document ACID;|
| (MongoDB)            | (Compressed JSON/BSON records)       | JSON schema          | multi-node Raft elect|
| Key-Value NoSQL      | In-memory hash tables, SkipLists     | Schemaless binary or | In-memory atomic ops;|
| (Redis, DynamoDB)    | (Volatile RAM or SSD LSM log)        | string value payloads| quorum replication   |
| Wide-Column NoSQL    | LSM-Trees with SSTables & MemTables  | Multi-dimensional map| Tunable quorum; no   |
| (Cassandra, Scylla)  | (Append-only write optimization)     | (Partition + Cluster)| cross-row ACID joins |
| Graph Database       | Direct memory pointer arrays         | Graph topology       | ACID transactional   |
| (Neo4j)              | (Index-free pointer adjacency)       | (Nodes, Edges, Props)| graph traversals     |
| Distributed NewSQL   | Multi-Paxos / Raft + TrueTime atomic | Strict relational    | Globally distributed |
| (Google Spanner)     | clock synchronization engines        | schema with SQL joins| ACID transactions    |
+----------------------------------------------------------------------------------------------------+
```

---

# 9. Service Architecture Topologies: Monolith vs. Microservices vs. Serverless

```
+----------------------------------------------------------------------------------------------------+
| PARADIGM             | STRUCTURAL MODEL                     | ADVANTAGES           | SYSTEM OVERHEAD      |
+----------------------------------------------------------------------------------------------------+
| Modular Monolith     | Single process, unified memory       | Simple deployments,  | Scaling bottleneck   |
|                      | space, shared relational database    | zero network latency | across single DB     |
| Microservices        | Loosely coupled services interacting | Independent scaling, | Network latency,     |
|                      | via RPC / REST / Message Queues      | decoupled deploy     | distributed tracing  |
| Serverless / FaaS    | Ephemeral event-triggered worker     | Automatic scaling,   | Cold start latencies,|
| (AWS Lambda)         | containers spun up on-demand         | zero idle compute fee| stateless constraints|
+----------------------------------------------------------------------------------------------------+
```

---

# 10. Core Architectural Summary Matrix

```
+----------------------------------------------------------------------------------------------------+
| ARCHITECTURAL CONCEPT| CORE MATHEMATICAL / SYSTEM PROPERTY  | PRIMARY APPLICATION DOMAIN           |
+----------------------------------------------------------------------------------------------------+
| PACELC Formalism     | Models partition trade-offs vs normal| Distributed database engine selection|
| Tail Amplification   | P(Slow) = 1 - (1-p)^M                | Microservice fan-out optimization    |
| Availability Nines   | SLA = MTBF / (MTBF + MTTR)           | Fault tolerance & HA engineering     |
| Token Bucket Limiter | Uniform rate fill with burst capacity| Ingress DDoS & API quota enforcement |
| Circuit Breaker      | Closed -> Open -> Half-Open FSM      | Cascading service failure mitigation |
| Distributed NewSQL   | Raft/Paxos + Hardware TrueTime clocks| Global multi-region ACID consistency |
+----------------------------------------------------------------------------------------------------+
```
