# Distributed Databases, Replication & Partitioning Theory

> **Scope:** Relational vs. Non-Relational Storage Models, CAP & PACELC Theorems, ACID vs. BASE, Replication Topologies, Quorum Consensus ($W + R > N$), Sharding Strategies, Consistent Hashing, and Multi-Tier Database Scaling Architectures.

---

# Table of Contents
1. [Database Storage Models & Architecture Families](#1-database-storage-models--architecture-families)
2. [CAP Theorem & The PACELC Theoretical Framework](#2-cap-theorem--the-pacelc-theoretical-framework)
3. [ACID vs. BASE Consistency Models](#3-acid-vs-base-consistency-models)
4. [Replication Topologies & Quorum Consensus](#4-replication-topologies--quorum-consensus)
5. [Partitioning & Horizontal Sharding Mechanics](#5-partitioning--horizontal-sharding-mechanics)
6. [Multi-Tier Database Scaling Pipeline](#6-multi-tier-database-scaling-pipeline)
7. [Distributed Systems Database Principles](#7-distributed-systems-database-principles)

---

# 1. Database Storage Models & Architecture Families

```
+--------------------------------------------------------------------------------------------------------------------+
| DATABASE FAMILY      | REPRESENTATIVE ENGINES     | DATA MODEL & STORAGE STRUCTURE   | PRIMARY ARCHITECTURAL DOMAIN|
+--------------------------------------------------------------------------------------------------------------------+
| Relational (RDBMS)   | PostgreSQL, MySQL, Oracle  | Structured Tables, Foreign Keys, | Strict ACID transactions,   |
|                      |                            | B+ Tree On-Disk Indexing         | complex multi-table joins   |
+--------------------------------------------------------------------------------------------------------------------+
| Document             | MongoDB, Couchbase, Dynamo | Hierarchical JSON / BSON Trees,  | Semi-structured entities,   |
|                      |                            | Polymorphic Dynamic Schemas      | rapidly evolving schemas    |
+--------------------------------------------------------------------------------------------------------------------+
| Key-Value            | Redis, Memcached, RocksDB  | In-Memory Hash Tables, SkipLists,| High-speed caching, state   |
|                      |                            | Byte Array Values                | sessions, sub-ms lookup     |
+--------------------------------------------------------------------------------------------------------------------+
| Wide-Column          | Apache Cassandra, ScyllaDB | LSM-Trees, SSTables, Sparse      | High-throughput time-series,|
|                      |                            | Multi-dimensional Distributed Map| telemetry, append workloads |
+--------------------------------------------------------------------------------------------------------------------+
| Graph                | Neo4j, Amazon Neptune      | Nodes, Edges, Adjacency Pointers | Complex relationship graphs,|
|                      |                            | Direct Index-Free Graph Traversal| social graphs, fraud rings  |
+--------------------------------------------------------------------------------------------------------------------+
```

---

# 2. CAP Theorem & The PACELC Theoretical Framework

### The CAP Theorem (Brewer's Conjecture / Gilbert & Lynch Proof)
In an asynchronous network subject to partitions, a distributed data store can simultaneously provide at most two of the following three guarantees:
- **Consistency (C):** Every read receives the most recent write or an error (Linearizability).
- **Availability (A):** Every non-failing node returns a non-error response without guarantee that it contains the most recent write.
- **Partition Tolerance (P):** The system continues to operate despite arbitrary message loss or delay across network links.

```
                                  [ CAP THEOREM TRIANGLE ]
                                      Consistency (C)
                                           /   \
                                          /     \
                                         /   P   \  (Partitions are inevitable in physical networks)
                                        /         \
                      Availability (A) ----------- Partition Tolerance (P)
```

- **CP Systems:** Reject writes or block during network partitions to prevent split-brain inconsistencies (e.g. Google Spanner, etcd).
- **AP Systems:** Accept local writes across separated partitions, reconciling divergent versions asynchronously (e.g. Apache Cassandra, DynamoDB).

### The PACELC Theorem (Abadi's Extension)
Extends CAP to model steady-state operation in the absence of partitions:
- **If Partition ($P$):** Trade off Availability ($A$) versus Consistency ($C$).
- **Else ($E$):** Trade off Latency ($L$) versus Consistency ($C$).
  - *PC/EC (e.g. Spanner):* Preserves consistency under both partitioned and normal states at the expense of availability and latency.
  - *PA/EL (e.g. Cassandra):* Optimizes for availability during partitions and minimal latency during normal execution.

---

# 3. ACID vs. BASE Consistency Models

```
+---------------------------------------------------------------------------------------------------+
| ATTRIBUTE            | ACID (Strong Relational Model)        | BASE (Distributed Eventual Model)  |
+---------------------------------------------------------------------------------------------------+
| Core Invariant       | Immediate Linearizable Consistency    | Eventual Convergence across nodes  |
| Meaning of Acronym   | Atomicity, Consistency, Isolation,    | Basically Available, Soft state,   |
|                      | Durability                            | Eventual consistency               |
| Availability Posture | Fails fast or blocks on partitions    | Returns best-effort local response |
| Concurrency Strategy | Pessimistic Locking (2PL), MVCC       | Conflict resolution (LWW, CRDTs)   |
+---------------------------------------------------------------------------------------------------+
```

---

# 4. Replication Topologies & Quorum Consensus

```
Single-Leader (Primary-Replica Architecture):
[ Client Writes ] ---> [ Primary Leader Node (Read/Write) ]
                                |
               +----------------+----------------+ (Asynchronous / Semi-Synchronous Replication)
               |                                 |
               v                                 v
   [ Read Replica 1 (Read-Only) ]    [ Read Replica 2 (Read-Only) ]
```

### Replication Strategies:
1. **Synchronous Replication:** Leader waits for replica write acknowledgment before returning success. Guarantees zero data loss on leader failure, at the cost of write latency.
2. **Asynchronous Replication:** Leader acknowledges write immediately after local commit, propagating changes to replicas in the background. High write throughput, but subject to **Replication Lag** and potential data loss during sudden primary failover.

### Leaderless Quorum Consensus (Dynamo Model):
For a system with $N$ total replicas, write quorum size $W$, and read quorum size $R$:

$$W + R > N \implies \text{Strong Consistency (Overlapping Replicas Guaranteed)}$$

```
Example (N = 3, W = 2, R = 2):
W + R = 4 > 3 -> The Read Quorum is guaranteed to intersect with at least one node in the Write Quorum.
```

---

# 5. Partitioning & Horizontal Sharding Mechanics

```
+---------------------------------------------------------------------------------------------------+
| SHARDING STRATEGY    | MECHANICS                             | ADVANTAGES           | DISADVANTAGES       |
+---------------------------------------------------------------------------------------------------+
| Range-Based          | Shards mapped by contiguous key ranges| Efficient range scans| Write hotspots on   |
|                      | (e.g. `[A-M]`, `[N-Z]`)               | (`BETWEEN A AND B`)  | sequential keys     |
+---------------------------------------------------------------------------------------------------+
| Hash-Based           | `Shard ID = hash(key) % N`            | Uniform key and write| Range queries force |
|                      |                                       | distribution         | scatter-gather scans|
+---------------------------------------------------------------------------------------------------+
| Consistent Hashing   | Keys and nodes mapped to a circular   | Minimizes data re-   | Requires virtual    |
|                      | 360-degree hash ring                  | balancing (K/N keys) | node balancing      |
+---------------------------------------------------------------------------------------------------+
```

### The Cross-Shard Join Problem:
- Relational joins across distinct physical shards require distributed network scatter-gather queries, streaming partial datasets to coordinator nodes to perform distributed hash joins.
- **Mitigation:** Select a **Shard Key** (e.g. `tenant_id`, `customer_id`) that co-locates relational child records with parent records on the same physical shard.

---

# 6. Multi-Tier Database Scaling Pipeline

Database scaling follows a systematic progression from single-node optimization to distributed partitioning:

```
[ Tier 1: Query & Index Optimization ]
(Profile slow queries via EXPLAIN ANALYZE, build covering B-Tree indexes, tune buffer pools)
                 |
                 v
[ Tier 2: Read/Write Splitting ]
(Direct writes to Primary Leader; route analytical/read traffic across asynchronous Read Replicas)
                 |
                 v
[ Tier 3: In-Memory Caching ]
(Deploy Cache-Aside or Write-Through caching with Redis/Memcached for hot read paths)
                 |
                 v
[ Tier 4: Horizontal Sharding ]
(Partition relation rows across independent physical nodes using Consistent Hashing)
```

---

# 7. Distributed Systems Database Principles

1. **Replication Lag & Read-Your-Own-Writes:** Asynchronous replication introduces a temporal lag. Systems enforce *Read-Your-Own-Writes* consistency by routing reads for modified user state directly to the primary leader for a bounded window following an update.
2. **Split-Brain Mitigation:** Network partitions can isolate cluster nodes. Systems prevent dual-leader split-brain scenarios using majority consensus protocols (Paxos, Raft) requiring $> N/2$ node votes for leadership election and log commit.
3. **Change Data Capture (CDC):** Instead of application-level dual writes (which risk silent drift on partial failures), CDC tails the database transaction Write-Ahead Log (WAL) to asynchronously publish committed mutations to search indexes, caches, and stream processors.
