# Distributed Databases, Storage Partitioning & Asynchronous Event Systems

> **Scope:** Distributed Database Replication Models (Single-Leader, Multi-Leader, Leaderless Dynamo Quorums), Replication Lag & Consistency Guarantees, Horizontal Partitioning & Sharding Architectures, Consistent Hashing Rings with Virtual Nodes, Distributed Transaction Processing (Two-Phase Commit vs. Saga Orchestration), and Asynchronous Message Brokering (AMQP vs. Distributed Partitioned Logs).

---

# Table of Contents
1. [Distributed Database Replication Topologies](#1-distributed-database-replication-topologies)
2. [Replication Lag & Client-Side Consistency Guarantees](#2-replication-lag--client-side-consistency-guarantees)
3. [Database Partitioning & Horizontal Sharding Models](#3-database-partitioning--horizontal-sharding-models)
4. [Consistent Hashing & Virtual Node Topology](#4-consistent-hashing--virtual-node-topology)
5. [Distributed Transaction Coordination: 2PC vs. Saga Pattern](#5-distributed-transaction-coordination-2pc-vs-saga-pattern)
6. [Asynchronous Message Queues & Distributed Event Streaming](#6-asynchronous-message-queues--distributed-event-streaming)
7. [Core Architectural Summary Matrix](#7-core-architectural-summary-matrix)

---

# 1. Distributed Database Replication Topologies

Database replication distributes duplicate copies of data across physically isolated compute nodes to enhance read capacity, provide fault tolerance, and reduce geographic latency.

```
Single-Leader Replication Architecture:
[ Ingress Writes ] ---> [ Primary Node (R/W) ]
                                |
                +---------------+---------------+ (Asynchronous WAL Stream)
                |                               |
                v                               v
    [ Read Replica 1 (RO) ]          [ Read Replica 2 (RO) ]
                ^                               ^
                +---- [ Ingress Read Traffic ] -+
```

```
+----------------------------------------------------------------------------------------------------+
| REPLICATION MODEL    | WRITE INGRESS                       | READ INGRESS          | CONCURRENCY TRADEOFF |
+----------------------------------------------------------------------------------------------------+
| Single-Leader        | 1 designated Primary node processes | Distributed across N  | Primary write I/O is |
| (Master-Replica)     | all write mutations sequentially    | read-only replicas    | a scaling ceiling    |
| Multi-Leader         | Multiple Primary nodes across       | Local datacenter reads| Cross-datacenter     |
| (Active-Active)      | geographic regions accept writes    | with minimal latency  | conflict resolution  |
| Leaderless           | Any node accepts writes; evaluated  | Quorum evaluation:    | Eventual consistency;|
| (Dynamo / Cassandra) | via distributed quorum consensus    | Read quorum (R)       | absence of ACID joins|
+----------------------------------------------------------------------------------------------------+
```

### The Leaderless Dynamo Quorum Invariant:
For a replication cluster of $N$ nodes, configuring write quorum $W$ and read quorum $R$:
$$W + R > N$$
- **Theorem:** The pigeonhole principle guarantees that the set of nodes responding to a read operation ($R$) intersects with the set of nodes that acknowledged the latest write ($W$) by at least one overlapping replica node, ensuring strong consistency when paired with version timestamps.

---

# 2. Replication Lag & Client-Side Consistency Guarantees

In asynchronous replication, mutations take non-zero time ($\Delta t$) to propagate from the Primary WAL to Read Replicas, resulting in **Replication Lag**:

```
Replication Lag Inconsistency Cycle:
1. Client mutates entity E on Primary (t = 0)
2. Application redirects immediate read request to Read Replica (t = 10ms)
3. Replica WAL stream lags by 50ms -> Returns stale record!
```

```
+----------------------------------------------------------------------------------------------------+
| CONSISTENCY MODEL    | OPERATIONAL GUARANTEE                | SYSTEM IMPLEMENTATION                |
+----------------------------------------------------------------------------------------------------+
| Read-Your-Own-Writes | A user always observes their own     | Route client reads to Primary for a  |
| Consistency          | latest updates immediately           | fixed duration ($\Delta t$) post-write|
| Monotonic Reads      | Once a client observes version $V_k$,| Deterministically bind user sessions |
|                      | they will never observe $V_{k-1}$    | to a specific replica via IP hash    |
| Consistent Prefix    | Causally related writes are observed | Enforce single-partition ordering or |
| Reads                | in their exact chronological order   | attach Lamport logical timestamps    |
+----------------------------------------------------------------------------------------------------+
```

---

# 3. Database Partitioning & Horizontal Sharding Models

Horizontal partitioning (sharding) divides large relational tables or document collections into independent physical database instances:

```
[ Request: Record ID 4021 ] ---> [ Sharding Router / Coordinator ]
                                                |
                                      Calculates: hash(Key) % N
                                                |
                        +-----------------------+-----------------------+
                        | (Shard 0)             | (Shard 1)             | (Shard 2)
                        v                       v                       v
                 [ DB Node 0 ]           [ DB Node 1 ]           [ DB Node 2 ]
                 (Range: 0-1M)           (Range: 1M-2M)          (Range: 2M-3M)
```

```
+----------------------------------------------------------------------------------------------------+
| SHARDING STRATEGY    | PARTITIONING LOGIC                   | PROS                 | CONS                 |
+----------------------------------------------------------------------------------------------------+
| Range-Based          | Partitions by contiguous key ranges  | Optimized sequential | Write hotspotting on |
|                      | (e.g. `[2026-01-01, 2026-02-01)`)    | range query scans    | current timestamp tip|
| Hash-Based           | Partitions by mathematical hash:     | Uniform data and     | Scatter-gather scans |
|                      | `hash(shard_key) % ShardCount`       | traffic distribution | on all range queries |
| Directory-Based      | Central lookup service maps keys to  | Dynamic partition    | Lookup directory is a|
|                      | physical database shard endpoints    | migration & resizing | bottleneck & SPOF    |
+----------------------------------------------------------------------------------------------------+
```

---

# 4. Consistent Hashing & Virtual Node Topology

Modulo hashing ($h(k) \pmod N$) forces a complete cluster remapping ($100\%$ key migration) upon adding or removing nodes. **Consistent Hashing** maps keys and servers onto a circular integer space $[0, 2^{32}-1]$:

```
Consistent Hashing Ring Topology:
                            [ Node A (Offset: 0) ]
                                 /          \
                  Key 101       /            \     Key 205
                     *         /              \       *
                              /                \
         [ Node C (Offset: 240) ] ------------ [ Node B (Offset: 120) ]
                                     *
                                  Key 309
  (Rule: Map key to ring coordinate; assign to first server clockwise)
```

### Properties & Virtual Nodes:
1. **Minimal Disruption Property:** Scaling a cluster from $N$ to $N+1$ nodes migrates on average only $K / N$ keys, leaving all other assignments intact.
2. **Virtual Nodes (V-Nodes):** Physical servers are assigned $V$ distinct pseudorandom coordinates across the ring (e.g. $V = 128$). This prevents hot spot clustering and guarantees uniform distribution across heterogeneous hardware.

---

# 5. Distributed Transaction Coordination: 2PC vs. Saga Pattern

```
Two-Phase Commit (2PC - Centralized Synchronous):
Coordinator ---> Phase 1: Prepare (Acquires locks on all nodes) ---> (All YES?)
            ---> Phase 2: Commit  (Flushes write on all nodes)

Saga Pattern (Decentralized Asynchronous with Compensations):
[ Step 1: Reserve Stock ] ---> [ Step 2: Authorize Payment (FAIL!) ]
                                               |
                                               v (Trigger Compensating Transaction)
                               [ Compensate 1: Release Stock ]
```

```
+----------------------------------------------------------------------------------------------------+
| DIMENSION            | TWO-PHASE COMMIT (2PC)               | SAGA PATTERN (Orchestration/Choreog) |
+----------------------------------------------------------------------------------------------------+
| Consistency Model    | ACID (Immediate Strong Consistency)  | BASE (Eventual Consistency)          |
| Locking Strategy     | Blocking pessimistic locks held      | Zero cross-service database locks;   |
|                      | across all participant nodes         | local atomic transactions only       |
| Latency Profile      | High latency; scales poorly over WAN | Low latency; high parallel throughput|
| Failure Rollback     | Automated coordinator rollback abort | Explicit compensating transactions   |
| Primary Domain       | Core financial ledger reconciliations| Distributed microservice workflows   |
+----------------------------------------------------------------------------------------------------+
```

---

# 6. Asynchronous Message Queues & Distributed Event Streaming

```
Distributed Event Log Architecture (Apache Kafka):
Topic: [ Order-Events ]
  Partition 0: [ Msg 0 | Msg 1 | Msg 2 | Msg 3 (Offset: 3) ] ---> Consumer Group A (Worker 1)
  Partition 1: [ Msg 0 | Msg 1 | Msg 2 (Offset: 2)         ] ---> Consumer Group A (Worker 2)

Message Broker Queue Architecture (RabbitMQ / AMQP):
[ Producer ] ---> [ Exchange ] ---> [ In-Memory FIFO Queue ] ---> [ Consumer ACK ] -> (Message Purged)
```

```
+----------------------------------------------------------------------------------------------------+
| ATTRIBUTE            | AMQP BROKER (RabbitMQ)               | DISTRIBUTED LOG (Apache Kafka)       |
+----------------------------------------------------------------------------------------------------+
| Model Architecture   | Smart broker / Dumb consumer         | Dumb broker / Smart consumer         |
| Persistence Model    | In-memory queue (Purged post-ACK)    | Sequential append-only disk segments |
| Message Replay       | Non-supported (Ephemeral messages)   | Supported via consumer offset rewind |
| Throughput Scale     | Tens of thousands of msgs/sec        | Millions of events/sec per cluster   |
| Ordering Guarantee   | Per-queue FIFO                       | Strictly ordered per partition       |
| Consumer Model       | Push-based delivery                  | Pull-based polling model             |
+----------------------------------------------------------------------------------------------------+
```

### Operational Resilience Primitives:
- **Dead-Letter Queue (DLQ):** Quarantine buffer for malformed payloads that exceed retry thresholds, preventing head-of-line blocking.
- **Idempotency Deduplication:** Attaching deterministic idempotency tokens (UUIDs) to ensure safe consumer retries under at-least-once delivery guarantees.

---

# 7. Core Architectural Summary Matrix

```
+----------------------------------------------------------------------------------------------------+
| SYSTEM COMPONENT     | STRUCTURAL ROLE                      | PRIMARY GOVERNING INVARIANT          |
+----------------------------------------------------------------------------------------------------+
| Single-Leader DB     | Centralized write sequencing         | Asynchronous replica WAL streaming   |
| Leaderless Quorum    | High-availability decentralized writes| Quorum intersection: W + R > N       |
| Database Sharding    | Horizontal storage partitioning      | High cardinality shard key selection |
| Consistent Hashing   | Topology-resilient key assignment    | Minimal migration: K/N keys moved    |
| Two-Phase Commit     | Distributed ACID consensus           | Blocking coordinator lock evaluation |
| Saga Architecture    | Distributed eventual consistency     | Compensating rollback actions        |
| Partitioned Log      | Ordered asynchronous event streams   | Append-only disk log with offsets    |
+----------------------------------------------------------------------------------------------------+
```
