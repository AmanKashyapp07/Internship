# 04: Database Scaling, Replication & Sharding

> **Target:** Top Product & FinTech Technical Interviews (FAANG, Uber, Stripe, Atlassian, Citadel).  
> **Style:** High-ROI revision sheet. Focuses on replication lag anomalies, quorum math ($W + R > N$), consistent hashing, PACELC theorem, 2PC vs. Sagas, and the Transactional Outbox Pattern.

---

# Table of Contents
1. [SQL vs. NoSQL Decision Matrix](#1-sql-vs-nosql-decision-matrix)
2. [Database Replication Topologies & Lag Anomalies](#2-database-replication-topologies--lag-anomalies)
3. [Database Sharding & Partitioning Strategies](#3-database-sharding--partitioning-strategies)
4. [CAP vs. PACELC Theorem Deep-Dive](#4-cap-vs-pacelc-theorem-deep-dive)
5. [Distributed Transactions (2PC vs. Sagas & Outbox Pattern)](#5-distributed-transactions-2pc-vs-sagas--outbox-pattern)
6. [High-Yield Interview Questions & Defense](#6-high-yield-interview-questions--defense)
7. [Quick Recap](#7-quick-recap)

---

# 1. SQL vs. NoSQL Decision Matrix

```
+-----------------------------------------------------------------------------------------+
| DATABASE TYPE       | DATA MODEL            | EXAMPLES             | IDEAL PRODUCTION USE CASE|
+-----------------------------------------------------------------------------------------+
| Relational (SQL)    | Tables, Rows, Foreign | PostgreSQL, MySQL,   | Financial ledgers, ACID  |
|                     | Keys, Normalized      | Oracle, SQLite       | integrity, complex JOINs.|
+-----------------------------------------------------------------------------------------+
| Document (NoSQL)    | Hierarchical JSON /   | MongoDB, Couchbase,  | Dynamic catalogs, CMS,   |
|                     | BSON Documents        | DynamoDB             | polymorphic payload trees|
+-----------------------------------------------------------------------------------------+
| Key-Value (NoSQL)   | Key -> Binary Blob /  | Redis, Memcached,    | Session stores, caching, |
|                     | Data Structures       | Amazon DynamoDB      | real-time counters.      |
+-----------------------------------------------------------------------------------------+
| Wide-Column (NoSQL) | Tables with dynamic   | Apache Cassandra,    | High-velocity time-series|
|                     | column families       | ScyllaDB, Bigtable   | metrics, IoT sensor data.|
+-----------------------------------------------------------------------------------------+
| Graph (NoSQL)       | Nodes, Edges, Named   | Neo4j, Amazon        | Social networks, fraud   |
|                     | Relationships         | Neptune, ArangoDB    | rings, recommendation DAG|
+-----------------------------------------------------------------------------------------+
```

---

### ACID vs. BASE Paradigm Comparison
- **ACID (Traditional Relational):** Atomicity, Consistency, Isolation, Durability. Prioritizes strict immediate consistency and financial correctness at the cost of partition availability.
- **BASE (Distributed NoSQL):**
  - **Basically Available:** The system guarantees availability by accepting reads and writes even during network partitions.
  - **Soft State:** Data state may drift or change over time without user interaction due to background replica convergence.
  - **Eventual Consistency:** Given sufficient time without new updates, all replicas are mathematically guaranteed to converge to identical values.

---

# 2. Database Replication Topologies & Lag Anomalies

Replication copies data across multiple physical machines to provide **high availability, disaster recovery, and read throughput scaling**.

```
Single-Leader (Master-Slave) Architecture:

         [ Client Writes ]              [ Client Reads (Load Balanced) ]
                 |                                      |
                 v                                      v
       [ LEADER (PRIMARY) ]                     +-------+-------+
       - Accepts ALL Writes                     |               |
       - Streams WAL Replication Log            v               v
                 |                      [ REPLICA 1 ]    [ REPLICA 2 ]
                 +--------------------> (Follower)       (Follower)
                   (Async Replication)
```

---

### A. Synchronous vs. Asynchronous Replication
- **Synchronous Replication:** Leader waits for replica confirmation before returning success to the client.
  - *Advantage:* Zero data loss on leader crash.
  - *Disadvantage:* High write latency (blocked by slowest replica); write availability collapses if 1 replica drops offline.
- **Asynchronous Replication (Industry Standard):** Leader appends to local WAL, returns success to the client in 5ms, and streams logs to replicas in background.
  - *Advantage:* Ultra-fast write throughput; surviving replicas don't block writes.
  - *Disadvantage:* **Replication Lag** introduces read anomalies and potential data loss if the leader crashes before logs reach replicas (**Failover Data Loss**).

---

### B. Replication Lag Anomalies & Mitigations

```
1. READ-YOUR-WRITES CONSISTENCY (Casual Consistency):
- Anomaly: User updates profile picture (writes to Leader), refreshes page immediately (reads from lagging Replica), and sees OLD profile picture!
- Mitigation: Route reads for the user's OWN profile to the Leader for 10 seconds post-write, or inspect replication LSN (Log Sequence Number) timestamps.

2. MONOTONIC READS (Time Travel Anomaly):
- Anomaly: User reads post comments (hits Replica 1 with 0ms lag, sees 5 comments). User refreshes page (hits Replica 2 with 500ms lag, sees only 2 comments!). Data appears to travel backwards in time!
- Mitigation: Pin user sessions to a specific replica using Sticky Session hashing (`hash(user_id) % replica_count`).

3. CONSISTENT PREFIX READS (Causal Ordering Anomaly):
- Anomaly: Question is asked, then Answer is submitted. In a distributed partition, a user reads the Answer BEFORE the Question because writes replicate out of order.
- Mitigation: Route causally dependent writes to the same physical partition/leader.
```

---

### C. Leaderless Replication & Quorum Math ($W + R > N$)
In Dynamo-style leaderless databases (Apache Cassandra, Amazon DynamoDB):
- $N$ = Replication Factor (Total copies of data across the cluster).
- $W$ = Write Quorum (Number of replicas that must acknowledge a write).
- $R$ = Read Quorum (Number of replicas that must be queried during a read).

$$\mathbf{W + R > N} \implies \text{Guarantees Strong Consistency (Overlap Principle)}$$

```
Quorum Overlap Proof:
Let N = 3 Replicas [ R1, R2, R3 ]. Configure W = 2, R = 2 (W + R = 4 > 3).
- Write key = 'v2' acknowledged by R1 and R2 (Set: {R1, R2}).
- Read key queries R2 and R3 (Set: {R2, R3}).
- Intersection of Write and Read sets = {R2} (R2 contains the latest version!).
- Client compares timestamps and receives the up-to-date value 'v2'!
```

- **Read Repair:** When a client reads from multiple quorum replicas and notices an outdated value on one node, it asynchronously sends an update to the stale node.
- **Anti-Entropy with Merkle Trees:** Replicas periodically compare cryptographic **Merkle Trees** of their stored key ranges to sync missing data in the background without scanning millions of records.

---

# 3. Database Sharding & Partitioning Strategies

When a database's data volume or write throughput exceeds the hardware capacity of a single physical server, **Horizontal Partitioning (Sharding)** splits rows across independent database nodes.

```
Sharding Architecture via Router (e.g. MongoS / Vitess):

                       [ Application Server ]
                                 |
                                 v
                     [ Database Shard Router ]
                     (Evaluates hash(Shard Key))
                     /           |           \
                    v            v            v
             [ SHARD 1 ]    [ SHARD 2 ]   [ SHARD 3 ]
             (Users A-H)    (Users I-P)   (Users Q-Z)
```

---

### Sharding Strategies Comparison

```
+-----------------------------------------------------------------------------------------+
| STRATEGY            | MECHANICS                              | PRODUCTION TRADEOFFS     |
+-----------------------------------------------------------------------------------------+
| Range-Based         | Contiguous ranges:                     | **Hot-Spotting Risk**: If|
| Sharding            | Shard 1: IDs 1–1M; Shard 2: IDs 1M–2M. | writing auto-increment   |
|                     |                                        | IDs, ALL writes hit the  |
|                     |                                        | latest shard!            |
+-----------------------------------------------------------------------------------------+
| Hash-Based          | `shard_id = hash(key) % N`             | **Resharding Nightmare**:|
| Sharding            | Distributes rows uniformly across      | Changing N moves 90% of  |
|                     | N nodes.                               | data across the network! |
+-----------------------------------------------------------------------------------------+
| Consistent Hashing  | Hashes both nodes and keys onto a      | **Minimizes Data Churn**:|
| with Virtual Nodes  | virtual 360-degree hash ring.          | Adding a node moves only |
|                     |                                        | $K/N$ keys.              |
+-----------------------------------------------------------------------------------------+
| Directory / Lookup  | Centralized mapping service tracks     | High flexibility; lookup |
| Sharding            | which shard holds each entity.         | table is a single point  |
|                     |                                        | of failure / bottleneck. |
+-----------------------------------------------------------------------------------------+
```

---

### The Critical Shard Key Selection Criteria
A poor shard key will destroy cluster scalability. A senior engineer evaluates three factors:
1. **High Cardinality:** The key must have millions of distinct values (e.g. `user_id` or `uuid`), not enums like `country` or `gender` (which create massive unbalanced chunks).
2. **Uniform Write Distribution:** Avoid monotonically increasing values (timestamps, auto-increment IDs) as the sole shard key, which route 100% of incoming writes to a single physical machine (**Hot Spotting**).
3. **Query Isolation / Colocation:** Choose a key present in 90% of application queries (e.g. `workspace_id` or `tenant_id`) so the router directs queries to **exactly 1 shard**, completely avoiding expensive **Scatter-Gather cross-shard queries**.

---

### The 3 Major Hardships of Sharded Databases
1. **Cross-Shard Joins:** Relational `JOIN` operations across tables residing on different physical machines require transferring raw tables over the network, which is unacceptably slow.
2. **Distributed Transactions:** Enforcing ACID across multiple shards requires heavy distributed locking protocols (Two-Phase Commit).
3. **Global Unique ID Generation:** Standard database auto-increment sequences fail across distributed shards. We must use distributed ID generators:
   - **Twitter Snowflake:** 64-bit integer composed of `[ 41 bits: Epoch Timestamp | 10 bits: Machine Node ID | 12 bits: Local Sequence Counter ]`.
   - **UUIDv7:** Time-ordered 128-bit UUIDs that maintain B+ tree index insertion locality.

---

# 4. CAP vs. PACELC Theorem Deep-Dive

---

### A. The CAP Theorem (Brewer’s Theorem)
In an asynchronous network where network partitions are inevitable, a distributed data store can guarantee at most **two out of three properties**:

```
+-----------------------------------------------------------------------------------------+
| PROPERTY            | FORMAL MEANING IN DISTRIBUTED SYSTEMS                             |
+-----------------------------------------------------------------------------------------+
| Consistency (C)     | **Linearizability / Single-Copy Consistency**: Every read receives|
|                     | the most recent write or an error.                                |
+-----------------------------------------------------------------------------------------+
| Availability (A)    | Every non-failing node returns a non-error response for every     |
|                     | request (without guaranteeing it contains the latest write).      |
+-----------------------------------------------------------------------------------------+
| Partition           | The system continues to operate despite arbitrary message loss or |
| Tolerance (P)       | network delay between nodes (MANDATORY on real networks!).        |
+-----------------------------------------------------------------------------------------+
```

- **The Senior Reality:** You **CANNOT "choose CA"**. Network cables get cut, routers crash, and cloud switches fail. Partition Tolerance ($P$) is a physical reality of distributed networking. The real choice is:
  $$\mathbf{\text{During a Network Partition (P), choose between Consistency (CP) or Availability (AP)}}.$$

```
Network Partition Dilemma:
Node A (Leader in US) <----- [ NETWORK CABLE CUT ] -----> Node B (Replica in EU)

- CP Choice (Consistency): Node B refuses reads/writes and returns Error (Preserves consistency; sacrifices availability).
- AP Choice (Availability): Node B accepts writes locally (Maintains 100% availability; data diverges into split-brain!).
```

---

### B. The PACELC Theorem (Extending CAP to Normal Operation)
The CAP theorem only describes behavior *during a network failure*. The **PACELC Theorem** (Abadi) addresses the fundamental trade-off during **normal, healthy operation**:

$$\mathbf{\text{If Partition (P)}} \implies \text{Choose } \mathbf{\text{Availability (A)}} \text{ vs. } \mathbf{\text{Consistency (C)}}; \quad \mathbf{\text{Else (E)}} \implies \text{Choose } \mathbf{\text{Latency (L)}} \text{ vs. } \mathbf{\text{Consistency (C)}}.$$

```
Database PACELC Classifications:
- MongoDB:   PC / EC (Under partition: Consistent; Under normal: Consistent, higher read latency).
- Cassandra: PA / EL (Under partition: Available; Under normal: Low Latency, eventual consistency).
- PostgreSQL: PC / EC (Single-node ACID guarantees).
- Google Spanner: PC / EC (TrueTime atomic GPS/atomic clock synchronization achieves CP with low latency).
```

---

# 5. Distributed Transactions (2PC vs. Sagas & Outbox Pattern)

---

### A. Two-Phase Commit (2PC) Protocol
2PC guarantees atomic commits across multiple independent database nodes via a centralized **Transaction Coordinator**:

```
2PC Execution Lifecycle:

1. [ PREPARE PHASE ]
   - Coordinator sends "CAN COMMIT?" to all Cohorts (Nodes).
   - Cohorts acquire local locks, write Undo/Redo logs, and vote "YES" or "NO".
             |
             v
2. [ COMMIT PHASE ]
   - If ALL Cohorts vote "YES" -> Coordinator sends "GLOBAL COMMIT!". Cohorts commit and release locks.
   - If ANY Cohort votes "NO" (or times out) -> Coordinator sends "GLOBAL ABORT!". All rollback.
```

- **The Fatal Flaw of 2PC (Why High-Scale Systems Avoid It):**
  - **Blocking Protocol:** If the Coordinator crashes after Cohorts vote "YES" during Phase 1, **all Cohorts remain frozen indefinitely holding locks on database rows**, causing system-wide cascading outages.

---

### B. The Saga Pattern (Compensating Transactions)
Sagas replace heavy distributed locking by splitting a distributed transaction into a sequence of **independent local database transactions**:

$$T_1 \to T_2 \to T_3 \dots \to T_n$$

- Each local transaction commits and releases locks immediately in its local database.
- **Rollback via Compensating Actions:** If $T_3$ fails (e.g. Payment Rejected), the Saga coordinates executing reverse **Compensating Transactions** in reverse order:
  $$C_2 (\text{Release Reserved Inventory}) \to C_1 (\text{Cancel Order})$$
- **Implementation Styles:**
  1. **Choreography:** Microservices publish events to Kafka/RabbitMQ and react to sibling events autonomously.
  2. **Orchestration:** A dedicated central orchestrator state machine (e.g. Temporal, AWS Step Functions) explicitly commands each microservice step.

---

### C. The Transactional Outbox Pattern + CDC
- **The Dual-Write Problem:** Updating a database and publishing an event to Kafka in the same API call risks inconsistency if the server crashes between the DB write and the Kafka push.
- **The Outbox Solution:** The service writes the business entity AND an event record into an **`outbox` table in the SAME local database ACID transaction**.
- A separate **Change Data Capture (CDC)** engine (like **Debezium**) tails the database WAL log, extracts committed outbox events, and reliably streams them to Kafka with **At-Least-Once delivery guarantees**.

```
Transactional Outbox Flow:
[ User Request ] ---> [ Local DB Transaction: INSERT into orders + INSERT into outbox_table ] (Atomic!)
                                      |
                              [ Database WAL Log ]
                                      |
                     [ Debezium / CDC Engine (Tails WAL) ]
                                      |
                                      v
                        [ Apache Kafka Event Bus ]
```

---

# 6. High-Yield Interview Questions & Defense

### Q1: What is the Split-Brain scenario in database clusters and how is it prevented?
> **Answer:** Split-Brain occurs during a network partition when two isolated subsets of nodes both believe the other has died, and both elect their own primary leader. Clients write conflicting data to both leaders, corrupting data. It is prevented by enforcing **Strict Majority Quorum ($\lfloor N/2 \rfloor + 1$)**: a node can only become leader if it receives votes from a majority of all nodes in the cluster.

### Q2: Why is Two-Phase Locking (2PL) not the same as Two-Phase Commit (2PC)?
> **Answer:** 
> - **2PL (Two-Phase Locking):** A single-node **Concurrency Control** protocol (Growing Phase and Shrinking Phase) that guarantees serializable transaction ordering.
> - **2PC (Two-Phase Commit):** A distributed **Atomic Commit** consensus protocol (Prepare Phase and Commit Phase) ensuring multiple database nodes all commit or all abort a shared transaction.

### Q3: How does Consistent Hashing minimize data migration during node scaling?
> **Answer:** In traditional modulo hashing (`hash(k) % N`), changing $N$ (adding/removing a server) alters the hash bucket for $(N - 1)/N \approx 90\%$ of all keys, forcing massive cluster-wide data transfers. Consistent Hashing maps keys and servers onto a circular 360-degree ring; adding 1 server only reassigns keys belonging to its immediate predecessor, moving only $1/N$ of the total dataset.

---

# 7. Quick Recap

- **ACID vs. BASE:** ACID enforces strict immediate consistency; BASE accepts temporary soft-state drift for high partition availability.
- **Replication Lag Anomalies:** Prevent Read-Your-Writes drift by reading from Leader post-write; prevent Monotonic Read time-travel via sticky sessions.
- **Quorum Overlap:** $\mathbf{W + R > N}$ guarantees reading the latest write.
- **Sharding Rules:** Choose high-cardinality, non-monotonic shard keys to avoid hot-spotting; avoid cross-shard joins.
- **CAP Reality:** You cannot choose "CA" on physical networks; choose CP (Consistency) or AP (Availability) during network partitions.
- **Sagas vs. 2PC:** 2PC is a blocking protocol that freezes row locks on coordinator crashes; Sagas use asynchronous local transactions with Compensating Actions.
- **Transactional Outbox:** Solves dual-write inconsistencies by writing events into an `outbox` table within the primary ACID transaction, streamed via Debezium CDC.
