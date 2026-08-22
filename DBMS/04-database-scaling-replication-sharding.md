# 04: Database Scaling, Replication & Sharding

> **Target:** Verbal Technical Interviews at Top Tech Companies (FAANG, Uber, Stripe, Atlassian, Microsoft, Google).  
> **Focus:** Spoken trade-offs, replication lag anomalies, sharding strategies, CAP/PACELC theorems, and distributed transaction patterns.

---

## 1. SQL vs. NoSQL & ACID vs. BASE

```
+-----------------------------------------------------------------------------------------+
| DATABASE TYPE       | DATA MODEL            | BEST FOR PRODUCTION WORKLOADS             |
+-----------------------------------------------------------------------------------------+
| Relational (SQL)    | Tables, Rows, Foreign | Financial ledgers, ACID guarantees,       |
| (Postgres, MySQL)   | Keys, 3NF Normalization| complex relational multi-table JOINs.     |
+-----------------------------------------------------------------------------------------+
| Document (NoSQL)    | Hierarchical JSON /   | Dynamic catalogs, CMS, polymorphic schemas|
| (MongoDB)           | BSON Documents        | with nested entity hierarchies.           |
+-----------------------------------------------------------------------------------------+
| Key-Value (NoSQL)   | Key $\to$ Data Blob   | High-throughput low-latency caching,      |
| (Redis, DynamoDB)   | or Data Structures    | session stores, real-time counters.       |
+-----------------------------------------------------------------------------------------+
| Wide-Column (NoSQL) | Tables with dynamic   | High-velocity append-heavy time-series    |
| (Cassandra, Scylla) | column families       | metrics, IoT data, write-heavy logging.   |
+-----------------------------------------------------------------------------------------+
```

### ACID vs. BASE (The Spoken Contrast)
- **ACID (Relational):** Guarantees immediate consistency, transactional atomicity, and strict correctness at the cost of partition availability.
- **BASE (Distributed NoSQL):**
  - **Basically Available:** System guarantees availability even during node failures.
  - **Soft State:** Data values may drift over time without user interaction during background replica sync.
  - **Eventual Consistency:** Given enough time without new updates, all replicas converge to the exact same value.

---

## 2. Replication Topologies & Lag Anomalies

### Single-Leader (Master-Slave) Replication
- **Leader (Primary):** Accepts 100% of write requests; appends changes to its local WAL and streams logs to followers.
- **Followers (Replicas):** Read-only nodes that serve read traffic to scale read throughput horizontally.
- **Synchronous vs. Asynchronous:** Synchronous replication guarantees zero data loss on leader crash but adds write latency and blocks if a replica goes down. Asynchronous replication provides fast writes (5ms) but introduces **Replication Lag**.

### 3 Major Replication Lag Anomalies & Spoken Fixes

```
+-----------------------------------------------------------------------------------------+
| ANOMALY             | SCENARIO                               | SPOKEN MITIGATION        |
+-----------------------------------------------------------------------------------------+
| Read-Your-Writes    | User updates avatar (writes to Leader),| Route reads for user's   |
| (Causal Drift)      | refreshes page, reads from a lagging   | OWN profile to Leader for|
|                     | replica and sees OLD avatar!           | 10s post-write.          |
+-----------------------------------------------------------------------------------------+
| Monotonic Reads     | User refreshes: hits Replica 1 (0ms    | Pin user session to a    |
| (Time-Travel)       | lag, 5 comments), then hits Replica 2  | specific replica using   |
|                     | (500ms lag, 2 comments). Data regresses| Sticky Sessions.         |
+-----------------------------------------------------------------------------------------+
| Consistent Prefix   | Question asked, then Answer submitted. | Route causally related   |
| (Out-of-Order)      | In distributed partitions, user reads  | writes to the exact same |
|                     | the Answer BEFORE the Question!        | physical partition.      |
+-----------------------------------------------------------------------------------------+
```

---

## 3. Leaderless Replication & Quorum Math ($W + R > N$)

In leaderless databases (Cassandra, DynamoDB):
- $N$ = Total replicas per key
- $W$ = Number of replicas confirming a write
- $R$ = Number of replicas queried on a read

$$\mathbf{W + R > N} \implies \text{Guarantees Strong Consistency (At least 1 overlapping node contains the latest write)}$$

- **Read Repair:** When a client reads from $R$ nodes and notices one node has a stale timestamp, it asynchronously sends an update to repair the stale node.
- **Anti-Entropy (Merkle Trees):** Replicas periodically compare cryptographic Merkle Trees of key ranges in the background to sync missing data without scanning entire tables.

---

## 4. Database Sharding & Shard Key Selection

**Sharding (Horizontal Partitioning)** splits rows across separate physical database servers when data volume or write IOPS exceed a single machine's capacity.

```
+-----------------------------------------------------------------------------------------+
| SHARDING STRATEGY   | MECHANICS                              | SPOKEN TRADEOFF          |
+-----------------------------------------------------------------------------------------+
| Range-Based         | Contiguous ranges (IDs 1–1M, 1M–2M).   | **Hotspotting Risk**: Auto|
|                     |                                        | increment IDs hit 1 node.|
+-----------------------------------------------------------------------------------------+
| Hash-Based          | `shard_id = hash(key) % N`             | **Resharding Churn**:    |
|                     |                                        | Changing N moves 90% data|
+-----------------------------------------------------------------------------------------+
| Consistent Hashing  | Hashes keys & nodes onto a 360-degree  | **Optimal**: Adding node |
| (with Virtual Nodes)| circular ring.                         | moves only $1/N$ keys.   |
+-----------------------------------------------------------------------------------------+
```

### 3 Pillars of Shard Key Selection:
1. **High Cardinality:** Key must have millions of distinct values (e.g. `user_id`, `uuid`), not low-cardinality enums like `gender` or `country`.
2. **Uniform Write Distribution:** Avoid monotonic sequential IDs/timestamps as shard keys to prevent write hotspots.
3. **Query Colocation:** Choose a key present in 90% of queries (e.g. `workspace_id` in SaaS) so the router routes queries to **exactly 1 shard**, avoiding expensive **Scatter-Gather** queries.

---

## 5. CAP vs. PACELC Theorems

### A. The CAP Theorem Reality
In a distributed network, partitions ($P$) are physical inevitabilities (cables get cut, switches fail). You **CANNOT choose "CA"**. The actual choice is:

$$\mathbf{\text{During a Network Partition (P), choose between Consistency (CP) or Availability (AP)}}.$$

- **CP (Consistency):** Replicas reject writes and return errors to prevent divergent data (e.g. HBase, MongoDB).
- **AP (Availability):** Replicas accept writes locally, maintaining 100% uptime with eventual consistency (e.g. Cassandra, DynamoDB).

### B. The PACELC Theorem (Normal Operation)
Extends CAP to describe system trade-offs when the network is **healthy (Else)**:

$$\mathbf{\text{If Partition (P)}} \implies \text{Choose } \mathbf{A} \text{ vs. } \mathbf{C}; \quad \mathbf{\text{Else (E)}} \implies \text{Choose } \mathbf{\text{Latency (L)}} \text{ vs. } \mathbf{\text{Consistency (C)}}.$$

---

## 6. Distributed Transactions: 2PC vs. Sagas & Outbox

```
+-----------------------------------------------------------------------------------------+
| PATTERN             | MECHANICS                              | WHY IT IS USED / FLOCK   |
+-----------------------------------------------------------------------------------------+
| Two-Phase Commit    | 1. Prepare Phase (Vote Yes/No).        | **Blocking Flaw:** If the|
| (2PC)               | 2. Commit Phase (Global Commit/Abort). | coordinator crashes, all |
|                     |                                        | nodes freeze with locks! |
+-----------------------------------------------------------------------------------------+
| Saga Pattern        | Series of local DB transactions        | **Non-blocking:** Highly |
|                     | ($T_1 \to T_2 \to T_3$). On failure,   | scalable. Implemented via|
|                     | triggers **Compensating Actions** ($C_2 \to C_1$)| Choreography or Orchestration.|
+-----------------------------------------------------------------------------------------+
| Transactional Outbox| Service writes entity + event into     | **Solves Dual-Write:**   |
| + CDC (Debezium)    | local `outbox` table in **SAME ACID    | DB and message queue are |
|                     | transaction**. CDC streams to Kafka.   | 100% consistent.         |
+-----------------------------------------------------------------------------------------+
```

---

## 7. High-Yield Verbal Interview Q&A

### Q1: "What is Split-Brain in a database cluster and how is it prevented?"
> **Spoken Answer:** Split-Brain occurs during a network partition when two isolated subsets of nodes both assume the other has died, and both elect their own primary leader. Clients write conflicting data to both leaders, corrupting the database. It is prevented by enforcing **Strict Majority Quorum ($\lfloor N/2 \rfloor + 1$)**: a node can only be elected leader if it receives votes from a majority of all nodes in the cluster.

### Q2: "Why is 'CA' not a valid option in the CAP Theorem?"
> **Spoken Answer:** In real distributed systems, network partitions (hardware faults, dropped packets, fiber cuts) are unavoidable physical realities. A system cannot choose to ignore partitions ($P$). Therefore, the true architectural trade-off is choosing between Consistency ($CP$) or Availability ($AP$) when a partition occurs.

### Q3: "What is the Dual-Write problem and how does the Transactional Outbox pattern solve it?"
> **Spoken Answer:** The Dual-Write problem occurs when an application updates a database and publishes an event to Kafka in the same request; if the server crashes between the two calls, the system becomes permanently inconsistent. The Transactional Outbox pattern writes both the business update and the event record into the local database in a single atomic ACID transaction. A CDC tool (like Debezium) tails the database WAL and publishes the event to Kafka reliably.
