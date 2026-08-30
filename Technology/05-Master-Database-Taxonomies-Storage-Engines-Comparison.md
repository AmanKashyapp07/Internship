# Database Taxonomies, Storage Engine Architectures & Physical Storage Models

> **Scope:** Deep Comparative Architecture of Enterprise Database Systems: Relational B+ Tree Engines (SQL / OLTP), In-Memory Hash/SkipList Structures, Hierarchical Document Stores, Wide-Column Distributed LSM Trees, Index-Free Adjacency Graph Databases, Columnar Analytical Warehouses (OLAP), Time-Series Rollup Partitioning, High-Dimensional Vector Embeddings (HNSW), and Globally Distributed Consensus NewSQL (Raft/Paxos & TrueTime).

---

# Table of Contents
1. [Relational Database Management Systems (SQL / OLTP)](#1-relational-database-management-systems-sql--oltp)
2. [In-Memory Key-Value Stores](#2-in-memory-key-value-stores)
3. [Document-Oriented Databases](#3-document-oriented-databases)
4. [Wide-Column Distributed Storage Engines](#4-wide-column-distributed-storage-engines)
5. [Graph Databases & Index-Free Adjacency](#5-graph-databases--index-free-adjacency)
6. [Columnar Storage Engines & Analytical Warehouses (OLAP)](#6-columnar-storage-engines--analytical-warehouses-olap)
7. [Time-Series Database Engines](#7-time-series-database-engines)
8. [Vector Databases & High-Dimensional Embedding Indexes](#8-vector-databases--high-dimensional-embedding-indexes)
9. [Distributed SQL & Consensus Engines (NewSQL)](#9-distributed-sql--consensus-engines-newsql)
10. [Master Database Architectural Decision Matrix](#10-master-database-architectural-decision-matrix)

---

# 1. Relational Database Management Systems (SQL / OLTP)

Relational databases organize structured data into normalized tables with strict mathematical schemas, providing ACID transactional guarantees:

```
Relational Storage Architecture (Page-Oriented B+ Tree):
[ Master Table Heap File ]
  |-- Page 0 (8KB Buffer): [ Tuple 1 | Tuple 2 | Tuple 3 ... ]
  |-- Page 1 (8KB Buffer): [ Tuple 4 | Tuple 5 ... ]

[ Primary Index: Balanced B+ Tree ]
       [ Root Node: Keys 100, 200 ]
             /            \
  [ Leaf Page: 1..99 ] <---> [ Leaf Page: 100..199 ] (Linked Leaf Block Chain)
```

```
+----------------------------------------------------------------------------------------------------+
| ARCHITECTURAL CHARACTERISTIC                                                                       |
+----------------------------------------------------------------------------------------------------+
| - Primary Storage Engine: Page-oriented B+ Trees, Write-Ahead Logs (WAL), Buffer Pools.            |
| - Concurrency Control: Multi-Version Concurrency Control (MVCC) or Two-Phase Locking (2PL).         |
| - Query Language: Structured Query Language (SQL) compiled via Cost-Based Optimizers.              |
| - Inherent Strengths: Strong ACID correctness, mature relational integrity, expressive joins.      |
| - Inherent Weaknesses: Horizontal scaling (sharding) introduces operational complexity; rigid schema.|
| - Exemplar Implementations: PostgreSQL, MySQL (InnoDB), Microsoft SQL Server, Oracle Database.    |
+----------------------------------------------------------------------------------------------------+
```

---

# 2. In-Memory Key-Value Stores

Key-Value stores map arbitrary scalar or binary keys directly to typed values, operating predominantly in high-speed volatile RAM:

```
In-Memory Storage Topology:
Key (String) ---> [ In-Memory Universal Hash Table ] ---> Direct Memory Pointer
                                                                 |
                                +--------------------------------+--------------------------------+
                                | (String / Integer Payload)                                      | (Sorted Set: SkipList)
                                v                                                                 v
                        [ Scalar Value ]                                           [ Multi-Level SkipList Pointer ]
```

```
+----------------------------------------------------------------------------------------------------+
| ARCHITECTURAL CHARACTERISTIC                                                                       |
+----------------------------------------------------------------------------------------------------+
| - Primary Storage Engine: Single-threaded reactor event loops, Hash Tables, SkipLists.             |
| - Persistence Options: Asynchronous point-in-time snapshots (RDB) and Append-Only Files (AOF).      |
| - Inherent Strengths: Sub-millisecond read/write latencies; atomic mathematical operations.        |
| - Inherent Weaknesses: Memory cost constraints; absence of secondary indices and arbitrary joins.  |
| - Exemplar Implementations: Redis, KeyDB, Memcached, Amazon DynamoDB (Key-Value mode).            |
+----------------------------------------------------------------------------------------------------+
```

---

# 3. Document-Oriented Databases

Document databases store semi-structured records as hierarchical, schema-flexible BSON or JSON payloads:

```
Hierarchical BSON Document Structure:
{
  "_id": ObjectId("64a1f..."),
  "user_id": 1042,
  "profile": { "name": "Alpha", "verified": true },
  "tags": ["systems", "distributed", "storage"]
}
-> Stored on-disk in WiredTiger B-Trees with document-level locking concurrency.
```

```
+----------------------------------------------------------------------------------------------------+
| ARCHITECTURAL CHARACTERISTIC                                                                       |
+----------------------------------------------------------------------------------------------------+
| - Primary Storage Engine: On-disk B-Trees with document-level locking (WiredTiger).                |
| - Schema Model: Dynamic / polymorphic JSON/BSON documents with embedded structures.                |
| - Inherent Strengths: High developer agility, natural object mapping, native auto-sharding.       |
| - Inherent Weaknesses: Denormalization risks data duplication; cross-document joins are expensive. |
| - Exemplar Implementations: MongoDB, Couchbase, Google Cloud Firestore, CouchDB.                   |
+----------------------------------------------------------------------------------------------------+
```

---

# 4. Wide-Column Distributed Storage Engines

Wide-Column stores organize data into sparse, multi-dimensional sorted maps indexed by partition keys and clustering keys, utilizing Log-Structured Merge-Trees (LSM-Trees):

```
LSM-Tree Write Pipeline:
[ Ingress Write ] ---> [ Append to WAL (Disk) ] + [ Insert into MemTable (RAM SkipList) ]
                                                            |
                                        (When MemTable saturates buffer)
                                                            v
                                            [ Flush to Immutable L0 SSTable (Disk) ]
                                                            |
                                            (Background Merge Compaction)
                                                            v
                                            [ Dense Sorted L1 SSTable (Disk) ]
```

```
+----------------------------------------------------------------------------------------------------+
| ARCHITECTURAL CHARACTERISTIC                                                                       |
+----------------------------------------------------------------------------------------------------+
| - Primary Storage Engine: Log-Structured Merge-Trees (MemTables in RAM + SSTables on Disk).        |
| - Consistency Architecture: Decentralized peer-to-peer leaderless topology; tunable quorums.       |
| - Inherent Strengths: Extremely high sequential write throughput; zero single points of failure.   |
| - Inherent Weaknesses: Query-first data modeling required; eventual consistency without joins.     |
| - Exemplar Implementations: Apache Cassandra, ScyllaDB, Google Cloud Bigtable, Apache HBase.       |
+----------------------------------------------------------------------------------------------------+
```

---

# 5. Graph Databases & Index-Free Adjacency

Graph databases model entities as **Nodes** and relationships as **Edges**, utilizing **Index-Free Adjacency** where each node maintains direct physical memory pointers to its adjacent neighbors:

```
Index-Free Adjacency Memory Layout:
[ Node: User A ] ---> Pointer Array: [ &Edge1 (FriendOf) -> &Node: User B, &Edge2 -> &Node: User C ]
-> Traversing relationship graph executes in deterministic O(1) pointer-dereference time per hop!
```

```
+----------------------------------------------------------------------------------------------------+
| ARCHITECTURAL CHARACTERISTIC                                                                       |
+----------------------------------------------------------------------------------------------------+
| - Primary Storage Engine: Native graph stores with bidirectional pointer records (Index-Free Adj). |
| - Query Language: Graph traversal languages (Cypher, Gremlin, openCypher).                         |
| - Inherent Strengths: $O(1)$ pointer traversal per hop, avoiding exponential join table explosions.|
| - Inherent Weaknesses: Difficult horizontal partitioning across cluster nodes; niche domain fit.   |
| - Exemplar Implementations: Neo4j, Amazon Neptune, Memgraph, TigerGraph.                           |
+----------------------------------------------------------------------------------------------------+
```

---

# 6. Columnar Storage Engines & Analytical Warehouses (OLAP)

Columnar storage engines organize physical data on-disk by column vectors rather than by horizontal row tuples, maximizing analytical throughput:

```
Row-Oriented vs. Column-Oriented Storage Layout:
Row-Oriented:    [ Row 1: ID, Age, Zip, Status ] | [ Row 2: ID, Age, Zip, Status ]
Column-Oriented: File 1 (ID):     [ 1 | 2 | 3 | 4 ... ]
                 File 2 (Age):    [ 24 | 38 | 29 | 45 ... ] ---> Compressed & Vectorized SIMD Scan
                 File 3 (Status): [ "ACTIVE" | "PENDING" ... ]
```

```
+----------------------------------------------------------------------------------------------------+
| ARCHITECTURAL CHARACTERISTIC                                                                       |
+----------------------------------------------------------------------------------------------------+
| - Primary Storage Engine: Columnar data blocks, vectorized SIMD execution engines, MergeTree runs. |
| - Compression Codecs: Run-Length Encoding (RLE), Delta Encoding, Double-Delta, Gorilla, Dictionary.|
| - Inherent Strengths: High analytical aggregation speeds over billions of rows; massive compression|
| - Inherent Weaknesses: Poor transactional single-row update/point-lookup performance.              |
| - Exemplar Implementations: ClickHouse, Snowflake, DuckDB, Amazon Redshift, Google BigQuery.       |
+----------------------------------------------------------------------------------------------------+
```

---

# 7. Time-Series Database Engines

Time-Series databases are specialized storage engines optimized for timestamp-ordered sequences of metrics and telemetry events:

- **Chunked Time Partitioning:** Automatically creates physical partitions based on contiguous time windows (e.g. 2-hour, 24-hour chunks), enabling instantaneous dropping of aged data via drop-table commands rather than expensive row deletes.
- **Automated Rollup Downsampling:** Downsamples high-resolution metrics (e.g. 1-second raw telemetry) into compressed long-term aggregates (1-hour averages).
- **Exemplars:** TimescaleDB, InfluxDB, Prometheus, VictoriaMetrics.

---

# 8. Vector Databases & High-Dimensional Embedding Indexes

Vector databases store high-dimensional floating-point embeddings generated by machine learning models and execute **Approximate Nearest Neighbor (ANN)** similarity search:

```
Hierarchical Navigable Small World (HNSW) Graph Index:
Layer 2: [ Node A ] ---------------------------------------------> [ Node Z ] (Long-range skips)
Layer 1: [ Node A ] ---------------------> [ Node M ] -----------> [ Node Z ]
Layer 0: [ Node A ] -> [ Node B ] -> [ Node C ] -> [ Node M ] -> ... [ Node Z ] (Dense vector graph)
```

- **Distance Metrics:** Evaluates Cosine Similarity, Euclidean Distance ($L_2$), or Dot Product.
- **Index Topologies:** Hierarchical Navigable Small World (HNSW) graphs and Inverted File with Product Quantization (IVF-PQ) provide sub-10ms similarity queries over millions of dense vectors.
- **Exemplars:** Pinecone, Milvus, Qdrant, Weaviate, `pgvector`.

---

# 9. Distributed SQL & Consensus Engines (NewSQL)

Distributed SQL systems provide full relational ACID semantics and SQL support across multi-region geographic clusters through distributed consensus:

- **Consensus Protocols:** Employs Multi-Paxos or Raft consensus groups per shard/range partition.
- **Hardware Time Synchronization:** Google Spanner leverages **TrueTime** (GPS receivers and atomic clocks) to bound clock uncertainty ($\epsilon \le 7\text{ms}$), enabling globally linearizable multi-node transactions without centralized lock managers.
- **Exemplars:** Google Cloud Spanner, CockroachDB, YugabyteDB, TiDB.

---

# 10. Master Database Architectural Decision Matrix

```
+----------------------------------------------------------------------------------------------------+
| DATABASE CATEGORY   | STORAGE ENGINE CORE          | CONCURRENCY / ACID     | OPTIMAL DATA WORKLOAD|
+----------------------------------------------------------------------------------------------------+
| Relational (SQL)    | B+ Trees, Heap Pages, WAL    | Full ACID Transactions | Financial ledgers,   |
|                     |                              | (Strict Serializability) transactional systems |
+----------------------------------------------------------------------------------------------------+
| In-Memory KV        | RAM Hash Tables & SkipLists  | Single-Threaded Atomic | Caching, session auth|
|                     |                              | Operations             | real-time counters   |
+----------------------------------------------------------------------------------------------------+
| Document NoSQL      | WiredTiger B-Trees (BSON)    | Single-Document ACID   | Dynamic catalogs,    |
|                     |                              | (Multi-doc available)  | content management   |
+----------------------------------------------------------------------------------------------------+
| Wide-Column NoSQL   | Log-Structured Merge (LSM)   | Tunable Quorum         | High-throughput IoT, |
|                     |                              | (Eventual Consistency) | time-series telemetry|
+----------------------------------------------------------------------------------------------------+
| Graph Database      | Index-Free Adjacency Pointers| Graph-Level ACID       | Fraud ring detection,|
|                     |                              |                        | social relationship  |
+----------------------------------------------------------------------------------------------------+
| Columnar OLAP       | Columnar Blocks + SIMD Merge | Append-Only Bulk Batches| Large-scale analytics|
|                     |                              |                        | aggregation queries  |
+----------------------------------------------------------------------------------------------------+
| Vector Database     | HNSW Graphs & IVF-PQ Indexes | Approximate ANN Search | AI Semantic Search,  |
|                     |                              |                        | RAG retrieval systems|
+----------------------------------------------------------------------------------------------------+
| Distributed NewSQL  | Raft/Paxos + TrueTime Clocks | Distributed Multi-Node | Global multi-region  |
|                     |                              | ACID Relational Joins  | banking & commerce   |
+----------------------------------------------------------------------------------------------------+
```