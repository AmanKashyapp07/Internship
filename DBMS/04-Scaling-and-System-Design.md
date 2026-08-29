# Master Guide 04: Database Scaling & Distributed System Design

> **Focus:** SQL vs. NoSQL Decision Matrix, CAP & PACELC Theorems, ACID vs. BASE, Replication Topologies (Master-Slave & Quorums), Sharding & Consistent Hashing, and The 4-Step Database Scaling Playbook.
> 
> *Targeted for Top-Tier Tech System & Backend Engineering Interviews.*

---

# Table of Contents
1. [SQL vs. NoSQL: The Master Decision Matrix](#1-sql-vs-nosql-the-master-decision-matrix)
2. [CAP Theorem & The PACELC Extension](#2-cap-theorem--the-pacelc-extension)
3. [ACID vs. BASE: The Consistency Compromise](#3-acid-vs-base-the-consistency-compromise)
4. [Replication Topologies: Single-Leader vs. Leaderless Quorums](#4-replication-topologies-single-leader-vs-leaderless-quorums)
5. [Partitioning & Sharding Strategies](#5-partitioning--sharding-strategies)
6. [The 4-Step Database Bottleneck Scaling Playbook](#6-the-4-step-database-bottleneck-scaling-playbook)
7. [High-Frequency Interview Drill & Verbal Q&A](#7-high-frequency-interview-drill--verbal-qa)

---

# 1. SQL vs. NoSQL: The Master Decision Matrix

```
+--------------------------------------------------------------------------------------------------------------------+
| DATABASE FAMILY      | REPRESENTATIVE ENGINES     | DATA MODEL & STORAGE STRUCTURE   | BEST INTERVIEW USE CASE     |
+--------------------------------------------------------------------------------------------------------------------+
| Relational (SQL)     | PostgreSQL, MySQL, Oracle  | Structured Tables, Foreign Keys, | Financial transactions,     |
|                      |                            | B+ Tree On-Disk Indexing         | E-Commerce checkout, ACID   |
+--------------------------------------------------------------------------------------------------------------------+
| Document (NoSQL)     | MongoDB, Couchbase, Dynamo | Hierarchical JSON / BSON Trees,  | Catalogs, Content Management|
|                      |                            | Dynamic Polymorphic Schemas      | Rapidly evolving payloads   |
+--------------------------------------------------------------------------------------------------------------------+
| Key-Value (NoSQL)    | Redis, Memcached, DynamoDB | In-Memory Hash Tables, SkipLists,| Session caching, Rate       |
|                      |                            | Strings, Sets, Sorted Sets       | limiters, Real-time leaderbd|
+--------------------------------------------------------------------------------------------------------------------+
| Wide-Column (NoSQL)  | Apache Cassandra, ScyllaDB | LSM-Trees, SSTables, Sparse      | High-throughput time-series,|
|                      |                            | Multi-dimensional Distributed Map| IoT telemetry, Chat history |
+--------------------------------------------------------------------------------------------------------------------+
| Graph (NoSQL)        | Neo4j, Amazon Neptune      | Nodes, Edges, Adjacency Pointers | Social networks, Fraud rings|
|                      |                            | with Zero-Index Graph Traversal  | Recommendation engines      |
+--------------------------------------------------------------------------------------------------------------------+
```

- **One-Line Intuition:** SQL is an Excel workbook with strict mathematical cross-sheet formulas; NoSQL is a filing cabinet of independent folders tailored for specific query patterns.
- **The Interview Trap:** Saying "NoSQL is inherently faster than SQL." SQL on indexed primary keys runs in sub-millisecond time. NoSQL trades away ACID joins and multi-table transactions to achieve horizontal scaling across thousands of nodes.
- **30-Second Verbal Answer:** **"Choose SQL when you need complex relational joins, strict schema integrity, and strong ACID guarantees (e.g. banking ledgers). Choose NoSQL when you have massive write throughput exceeding single-node limits, non-relational unstructured payloads, or require horizontal partition scalability without distributed join overhead."**

---

# 2. CAP Theorem & The PACELC Extension

```
                                  [ CAP THEOREM TRIANGLE ]
                                      Consistency (C)
                                           /   \
                                          /     \
                                         /   P   \  (Network Partition is INEVITABLE on real networks!)
                                        /         \
                      Availability (A) ----------- Partition Tolerance (P)

Choice during a Network Partition:
* CP (Consistency over Availability): Reject writes to prevent split-brain dirty state (Google Spanner, Banking).
* AP (Availability over Consistency): Accept local writes; synchronize asynchronously later (Cassandra, DNS, Feeds).
```

### The PACELC Theorem (Beyond Network Partitions):
- **If Partition ($P$):** Trade off Availability ($A$) or Consistency ($C$).
- **Else ($E$):** Trade off Latency ($L$) or Consistency ($C$).
  - *Example (MongoDB):* Under normal operation, waiting for majority write acknowledgement increases **Latency** to guarantee **Consistency** ($PA/EC$ or $PC/EC$).

---

# 3. ACID vs. BASE: The Consistency Compromise

```
+---------------------------------------------------------------------------------------------------+
| ATTRIBUTE            | ACID (Relational / Strong Consistency)| BASE (Distributed / Eventual Consistency)  |
+---------------------------------------------------------------------------------------------------+
| Core Philosophy      | Pessimistic: Never allow dirty state  | Optimistic: Accept stale reads for speed   |
| Meaning of Acronym   | Atomicity, Consistency, Isolation,    | Basically Available, Soft state,           |
|                      | Durability                            | Eventual consistency                       |
| Consistency Model    | Immediate Strong Linearizability      | Eventual Convergence across replicas       |
| Availability Focus   | May block or abort during network split| Always returns responses (best effort)     |
+---------------------------------------------------------------------------------------------------+
```

---

# 4. Replication Topologies: Single-Leader vs. Leaderless Quorums

```
Single-Leader (Master-Slave with Read Replicas):
[ Client Writes ] ---> [ Primary Leader Node (Read/Write) ]
                                |
               +----------------+----------------+ (Asynchronous / Semi-Sync Replication)
               |                                 |
               v                                 v
   [ Read Replica 1 (Read-Only) ]    [ Read Replica 2 (Read-Only) ]
```

### The Leaderless Dynamo Quorum Formula:
For a cluster with $N$ total replicas, write quorum $W$, and read quorum $R$:
$$W + R > N \implies \text{Guaranteed Strong Consistency (Overlapping Replicas)}$$
- **Example:** $N = 3, W = 2, R = 2 \implies 2 + 2 = 4 > 3$. At least one node in the read quorum contains the latest write from the write quorum.

---

# 5. Partitioning & Sharding Strategies

```
+---------------------------------------------------------------------------------------------------+
| SHARDING STRATEGY    | MECHANICS                             | PROS                 | CONS                |
+---------------------------------------------------------------------------------------------------+
| Range-Based          | Shard by key range (e.g. A-M, N-Z     | Efficient range scans| Hotspotting on      |
|                      | or timestamp ranges)                  | (`BETWEEN A AND B`)  | monotonically rising|
+---------------------------------------------------------------------------------------------------+
| Hash-Based           | Shard ID = `hash(shard_key) % N`      | Uniform write        | Range queries force |
|                      |                                       | distribution         | scatter-gather scans|
+---------------------------------------------------------------------------------------------------+
| Consistent Hashing   | Virtual nodes mapped to a 360-degree  | Minimal data movement| Requires virtual node|
|                      | circular ring (e.g. DynamoDB, K8s)    | on adding/removing nodes| tuning for balance|
+---------------------------------------------------------------------------------------------------+
```

- **The Cross-Shard Join Penalty:** Joining data across two different physical database shards requires fetching datasets over the network into application server memory to perform distributed hash joins, degrading latency from milliseconds to seconds. Always choose a **Shard Key** (e.g. `tenant_id` or `user_id`) that co-locates frequently queried entities on the same physical shard.

---

# 6. The 4-Step Database Bottleneck Scaling Playbook

When an interviewer asks: *"Our relational database is crashing under 100x traffic growth. How do you scale it?"*, execute this exact 4-step progression:

```
[ Step 1: Optimize & Index ] ---> (Analyze Slow Query Logs, EXPLAIN ANALYZE, Add Covering Indexes)
             |
             v
[ Step 2: Read Replicas ]    ---> (Route SELECT queries to Read Replicas; keep Primary for Writes)
             |
             v
[ Step 3: Caching Layer ]    ---> (Insert Redis Cache-Aside layer for read-heavy hot keys & sessions)
             |
             v
[ Step 4: Shard Horizontally]---> (Partition database across multiple nodes by Shard Key)
```

---

# 7. High-Frequency Interview Drill & Verbal Q&A

### Q1: What is Replication Lag and how do you prevent stale reads?
> **Answer:** **Replication Lag** is the delay between a write committing on the Primary leader and propagating to asynchronous Read Replicas. To prevent a user from seeing stale data immediately after updating their profile (**Read-Your-Own-Writes Consistency**), route read requests for recently updated user data directly to the **Primary Leader** for $X$ seconds before falling back to read replicas.

### Q2: What is the difference between Vertical Partitioning and Horizontal Partitioning?
> **Answer:** **Vertical Partitioning** splits a wide table into smaller tables by columns (e.g. moving rarely accessed `user_bio_blob` and `profile_image` to a separate table). **Horizontal Partitioning (Sharding)** splits a table into smaller tables by rows (e.g. storing users 1-1M on Shard A and users 1M-2M on Shard B).

### Q3: What is the Split-Brain problem in distributed database clusters?
> **Answer:** When a network partition isolates nodes, two separate nodes may both believe they are the legitimate Primary Leader and accept conflicting concurrent writes. It is prevented using **Quorum Consensus algorithms (Raft, Paxos)** requiring a strict majority ($> N/2$ nodes) to elect a leader.

### Q4: Why is Consistent Hashing critical for caching and distributed databases?
> **Answer:** In simple modulo hashing (`hash(key) % N`), adding or removing a node changes the modulo base for nearly 100% of all keys, causing massive cache stampedes. Consistent Hashing maps keys and nodes to a ring, ensuring adding or removing a node moves **only $K/N$ keys on average**.

### Q5: What is the difference between Dual-Writing and Change Data Capture (CDC)?
> **Answer:** **Dual-Writing** has the application write to both the database and cache/search engine simultaneously, risking desynchronization on partial failures. **CDC (via Debezium/Kafka)** reads directly from the database's Write-Ahead Log (WAL), streaming committed database mutations asynchronously to downstream search indexes and caches with guaranteed ordering.
