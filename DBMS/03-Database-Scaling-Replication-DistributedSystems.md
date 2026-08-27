# Master Guide 03: Database Scaling, Replication & Distributed Systems

> **Focus:** Vertical vs Horizontal Scaling, Replication Topologies & Lag Anomalies, Quorum Math ($W + R > N$), Sharding & Consistent Hashing, Distributed Consensus (Raft vs Paxos), Google Spanner & TrueTime, Distributed Indexing (Local vs Global), CAP/PACELC Theorems, Distributed Transactions (2PC vs Saga), and Database Selection Taxonomies (SQL vs NoSQL vs OLAP vs Vector DBs).
> 
> *Designed to crush top-tier FAANG/FinTech database engineering interviews.*

---

# Table of Contents
1. [Master Database Selection Flowchart](#1-master-database-selection-flowchart)
2. [Database Taxonomies & Engine Mechanics](#2-database-taxonomies--engine-mechanics)
3. [Replication Topologies & Lag Anomalies](#3-replication-topologies--lag-anomalies)
4. [Leaderless Replication & Quorum Math ($W + R > N$)](#4-leaderless-replication--quorum-math)
5. [Sharding Strategies & Consistent Hashing](#5-sharding-strategies--consistent-hashing)
6. [Distributed Consensus: Raft vs. Paxos](#6-distributed-consensus-raft-vs-paxos)
7. [Google Spanner & TrueTime Architecture](#7-google-spanner--truetime-architecture)
8. [Distributed Indexing: Local vs. Global Secondary Indexes](#8-distributed-indexing-local-vs-global-secondary-indexes)
9. [CAP vs. PACELC Theorems](#9-cap-vs-pacelc-theorems)
10. [Distributed Transactions: 2PC vs. Saga & CDC Outbox](#10-distributed-transactions-2pc-vs-saga--cdc-outbox)
11. [Interview Quick-Fire Verbal Q&A](#11-interview-quick-fire-verbal-qa)

---

# 1. Master Database Selection Flowchart

```
                            START: WHAT IS YOUR PRIMARY DATA REQUIREMENT?
                                                  │
         ┌────────────────────────────────────────┼────────────────────────────────────────┐
         │                                        │                                        │
         ▼                                        ▼                                        ▼
[ STRUCTURED & RELATIONAL ]             [ UNSTRUCTURED / NOSQL ]                [ SPECIALIZED ENGINES ]
         │                                        │                                        │
 ├── Need ACID, transactions,             ├── Dynamic JSON documents,             ├── Sub-ms RAM caching,
 │   complex JOINs, SQL?                  │   nested data, fast schema?           │   sessions, counters?
 │   └── PostgreSQL / MySQL               │   └── MongoDB (Document)              │   └── Redis (Key-Value)
 │                                        │                                       │
 └── Need global horizontal scale         ├── Massive write volume (>100k/s),     ├── Millions of rows analytics,
     + strict ACID transactions?          │   time-series, wide sparse data?      │   BI reports, aggregations?
     └── Google Spanner / CockroachDB     │   └── Cassandra (Wide-Column)         │   └── ClickHouse (Columnar)
                                          │                                       │
                                          └── Complex relationships, network      ├── Fuzzy text search, logs,
                                              traversals, fraud graphs?           │   autocomplete?
                                              └── Neo4j (Graph)                   │   └── Elasticsearch (Search)
                                                                                  │
                                                                                  └── GenAI, LLM memory,
                                                                                      semantic embeddings?
                                                                                      └── Pinecone / pgvector
```

---

# 2. Database Taxonomies & Engine Mechanics

```
+---------------------------------------------------------------------------------------------------+
| DATABASE CATEGORY   | ENGINE INTERNALS       | CORE ADVANTAGE         | PRODUCTION USE CASES      |
+---------------------------------------------------------------------------------------------------+
| Relational (RDBMS)  | B+ Trees (8KB Pages)   | ACID, FKs, Join safety | Stripe checkout, Users    |
| Document (MongoDB)  | BSON Trees (WiredTiger)| Dynamic JSON, No Join  | Product catalog, CMS      |
| Key-Value (Redis)   | RAM Hash Table/SkipList| Sub-ms speed, 100k+ ops| Caching, Sessions, RateLim|
| Wide-Column         | LSM-Trees + SSTables   | High write throughput  | IoT telemetry, Logs, Chat |
| Graph (Neo4j)       | Index-Free Adjacency   | $O(k)$ pointer hop     | Social graph, Fraud rings |
| Columnar (ClickHouse)| Column-Major + SIMD  | 100x fast aggregations | BI, OLAP Data Warehouses  |
| Vector (Pinecone)   | HNSW Graphs            | Similarity embeddings  | LLM RAG, Semantic search  |
+---------------------------------------------------------------------------------------------------+
```

---

# 3. Replication Topologies & Lag Anomalies

### Single-Leader (Master-Slave) Replication
- **Leader (Primary):** Accepts 100% of write requests; appends changes to local WAL and streams logs to followers.
- **Followers (Replicas):** Read-only nodes that serve read traffic to scale read throughput horizontally.
- **Synchronous vs. Asynchronous:** Synchronous replication guarantees zero data loss on leader crash but adds write latency and blocks if a replica fails. Asynchronous replication provides fast writes (5ms) but introduces **Replication Lag**.

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

# 4. Leaderless Replication & Quorum Math ($W + R > N$)

In leaderless databases (Cassandra, DynamoDB):
- $N$ = Total replicas per key
- $W$ = Number of replicas confirming a write
- $R$ = Number of replicas queried on a read

$$\mathbf{W + R > N} \implies \text{Guarantees Strong Consistency (At least 1 overlapping node contains the latest write)}$$

- **Read Repair:** When a client reads from $R$ nodes and notices one node has a stale timestamp, it asynchronously sends an update to repair the stale node.
- **Anti-Entropy (Merkle Trees):** Replicas periodically compare cryptographic Merkle Trees of key ranges in the background to sync missing data without scanning entire tables.

---

# 5. Sharding Strategies & Consistent Hashing

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

---

# 6. Distributed Consensus: Raft vs. Paxos

Distributed consensus allows a cluster of nodes to agree on a single sequence of state machine operations even if nodes fail or network partitions occur.

```
+-----------------------------------------------------------------------------------------+
| CONSENSUS MECHANISM | RAFT PROTOCOL                          | PAXOS PROTOCOL           |
+-----------------------------------------------------------------------------------------+
| Design Goal         | Designed for **Understandability**.    | Theoretical foundation.  |
|                     | Used in etcd, CockroachDB, Consul.    | Used in Google Chubby/Spanner|
+-----------------------------------------------------------------------------------------+
| Leader Selection    | Uses **Terms & Randomized Timers**.    | Proposer / Acceptor      |
|                     | Leader heartbeats suppress elections.  | multi-phase consensus.   |
+-----------------------------------------------------------------------------------------+
| Log Replication     | Strong Leader appends log entries to   | Multi-Paxos allows multiple|
|                     | followers via `AppendEntries` RPCs.     | proposers to drive logs. |
+-----------------------------------------------------------------------------------------+
| Safety Invariant    | **Leader Completeness**: A candidate   | Majority quorum ($N/2+1$) |
|                     | MUST contain all committed log entries | guarantees zero log      |
|                     | to be elected leader!                  | conflicts across epochs. |
+-----------------------------------------------------------------------------------------+
```

---

# 7. Google Spanner & TrueTime Architecture

Google Cloud Spanner achieves **Global External Consistency (Strict Serializability / Linearizability)** across worldwide datacenters using **TrueTime**:

### A. What is TrueTime?
- Uses synchronized **GPS receivers** and **Atomic Clocks** in every datacenter.
- TrueTime API exposes time not as a static scalar, but as a bounded time interval $[t_{\text{earliest}}, t_{\text{latest}}]$ where uncertainty $\epsilon = \frac{t_{\text{latest}} - t_{\text{earliest}}}{2} \approx 1\text{ms to } 7\text{ms}$.

### B. The Commit Wait Rule
- To guarantee that Transaction 2 (starting after Transaction 1 commits) receives a strictly larger timestamp ($TS(T_2) > TS(T_1)$):
- The Spanner leader delays committing $T_1$ until $2\epsilon$ time has elapsed (**Commit Wait**).
- This guarantees that when $T_1$ returns success, $TS(T_1)$ is guaranteed to be in the absolute past globally!

---

# 8. Distributed Indexing: Local vs. Global Secondary Indexes

When sharding a table across physical partitions, secondary indexes must be designed as Local or Global:

```
+-----------------------------------------------------------------------------------------+
| INDEX TYPE          | ARCHITECTURE                           | READ / WRITE TRADEOFF    |
+-----------------------------------------------------------------------------------------+
| Local Secondary     | Secondary index is partitioned BY THE  | **Fast Writes**: Local to|
| Index (LSI)         | SAME shard key as the main table.      | 1 shard node ($O(1)$).   |
|                     | Each shard holds its own local index.  | **Scatter-Gather Reads**:|
|                     |                                        | Must query ALL shards!   |
+-----------------------------------------------------------------------------------------+
| Global Secondary    | Secondary index is partitioned by the  | **Fast Reads**: Queries  |
| Index (GSI)         | SECONDARY KEY, independent of table    | target 1 exact index node|
|                     | shard key.                             | **Distributed Writes**:  |
|                     |                                        | Requires 2PC / Saga sync!|
+-----------------------------------------------------------------------------------------+
```

---

# 9. CAP vs. PACELC Theorems

### A. The CAP Theorem Reality
In a distributed network, partitions ($P$) are physical inevitabilities (cables get cut, switches fail). You **CANNOT choose "CA"**. The actual choice is:

$$\mathbf{\text{During a Network Partition (P), choose between Consistency (CP) or Availability (AP)}}.$$

### B. The PACELC Theorem (Normal Operation)
Extends CAP to describe system trade-offs when the network is **healthy (Else)**:

$$\mathbf{\text{If Partition (P)}} \implies \text{Choose } \mathbf{A} \text{ vs. } \mathbf{C}; \quad \mathbf{\text{Else (E)}} \implies \text{Choose } \mathbf{\text{Latency (L)}} \text{ vs. } \mathbf{\text{Consistency (C)}}.$$

---

# 10. Distributed Transactions: 2PC vs. Saga & CDC Outbox

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

# 11. Interview Quick-Fire Verbal Q&A

1. **"How does Google Spanner achieve global serializability using TrueTime?"**  
   *Answer:* TrueTime uses GPS and atomic clocks to bound time uncertainty $\epsilon$. Spanner enforces the **Commit Wait Rule**: delaying transaction commit until $2\epsilon$ time elapses, guaranteeing causally later transactions receive strictly higher timestamps worldwide.
2. **"What is the difference between Local and Global Secondary Indexes in sharded databases?"**  
   *Answer:* Local Secondary Indexes are partitioned by the main shard key (fast $O(1)$ writes, but reads require Scatter-Gather queries across all shards). Global Secondary Indexes are partitioned by the secondary key (fast $O(1)$ reads, but writes require distributed transactions across nodes).
