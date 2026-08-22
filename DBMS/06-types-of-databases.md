# 06: Types of Databases — Quick Decision Guide

> **Focus:** High-ROI summary of database categories, core strengths, ideal use cases, and a comprehensive ASCII decision flowchart for system design interviews.

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
                                                                                  ├── Time-stamped server/IoT
                                                                                  │   monitoring metrics?
                                                                                  │   └── InfluxDB / Prometheus
                                                                                  │
                                                                                  └── GenAI, LLM memory,
                                                                                      semantic embeddings?
                                                                                      └── Pinecone / pgvector
```

---

# 2. Database Categories Breakdown

### 1. Relational Databases (RDBMS / SQL)
```
[ Row 1: ID, Name, Age, Salary ]
[ Row 2: ID, Name, Age, Salary ]
```
- **Examples:** PostgreSQL, MySQL, SQLite, Oracle.
- **Engine:** B+ Trees (Row-oriented 8KB pages).
- **Core Strengths:** Strict ACID compliance, foreign keys, complex multi-table `JOIN`s, schema safety.
- **When to Use:** Financial ledgers, payment processing (Stripe), e-commerce checkout, user auth.
- **Limitation:** Horizontal sharding is difficult; vertical scaling ceiling.

---

### 2. Document Databases
```
{ "_id": 1, "name": "Aman", "addresses": [{ "city": "Bengaluru" }] }
```
- **Examples:** MongoDB, Couchbase, Amazon DocumentDB.
- **Engine:** BSON/JSON hierarchical trees (WiredTiger).
- **Core Strengths:** Schema-on-read flexibility; embeds child data to fetch entities in 1 single disk read.
- **When to Use:** Product catalogs, mobile backends, user profiles with evolving fields.
- **Limitation:** No multi-document ACID joins; document size limits (MongoDB 16MB).

---

### 3. Key-Value Stores
```
Key ("session:1001") ──[ O(1) ]──> Value ({ user_id: 42, role: "admin" })
```
- **Examples:** Redis, Memcached, Aerospike.
- **Engine:** In-memory Hash Tables & SkipLists.
- **Core Strengths:** Sub-millisecond latency, 100k+ ops/sec, rich atomic data structures (`ZSET`, HyperLogLog).
- **When to Use:** Session caching, API rate limiting, live gaming leaderboards, distributed locks (`SETNX`).
- **Limitation:** High RAM cost; cannot query or filter by value attributes without exact keys.

---

### 4. Wide-Column / Column-Family Stores
```
Partition Key ("user_42") ──> [ Col 1: timestamp | Col 2: event_type | Col 3: ip_addr ]
```
- **Examples:** Apache Cassandra, ScyllaDB, HBase.
- **Engine:** LSM-Trees (MemTable + SSTables on disk) with Masterless replication.
- **Core Strengths:** Massive write throughput (append-only), linear horizontal scale, zero single point of failure.
- **When to Use:** High-velocity IoT telemetry, user clickstream logs, chat message history.
- **Limitation:** Query patterns must be designed *before* table creation; zero `JOIN` support.

---

### 5. Graph Databases
```
(Alice:User) ──[:FRIEND_OF]──> (Bob:User) ──[:WORKS_AT]──> (Google:Company)
```
- **Examples:** Neo4j, Amazon Neptune, ArangoDB.
- **Engine:** **Index-Free Adjacency** (nodes store direct memory pointers to adjacent nodes).
- **Core Strengths:** Multi-hop relationship traversals in $O(k)$ pointer steps regardless of total graph size.
- **When to Use:** Social network connections, fraud detection rings, recommendation DAGs.
- **Limitation:** Hard to shard across multiple machines without cutting edges (Graph Cut problem).

---

### 6. Time-Series Databases (TSDB)
```
Timestamp: 10:00:00 ──> CPU: 45.2% | Memory: 62.1%
Timestamp: 10:00:01 ──> CPU: 46.0% | Memory: 62.1%
```
- **Examples:** InfluxDB, TimescaleDB, Prometheus.
- **Engine:** Gorilla compression (Delta-of-delta timestamps + XOR float compression).
- **Core Strengths:** 90%+ data compression, native time bucketing, automatic retention rollups.
- **When to Use:** Server & DevOps monitoring (Grafana), IoT sensor streams, stock market tick data.
- **Limitation:** Highly inefficient for random updates/deletes to historical data.

---

### 7. Columnar / OLAP Databases (Data Warehouses)
```
[ Region Column Block: 'US', 'US', 'EU', 'AP' ] ──> Compressed via Dictionary / RLE
[ Sales Column Block:   100,  250,  400,  150  ] ──> SIMD Vectorized Processing
```
- **Examples:** ClickHouse, Snowflake, Google BigQuery, DuckDB.
- **Engine:** Column-Major blocks with vectorized SIMD hardware execution.
- **Core Strengths:** Analytical aggregations (`SUM`, `AVG`) read only relevant columns, skipping 90%+ disk I/O.
- **When to Use:** Business intelligence (BI), analytics dashboards, clickstream reporting over billions of rows.
- **Limitation:** Terribly slow for single-row transactional writes (`INSERT`/`UPDATE`).

---

### 8. Vector Databases
```
Query Embedding: [ 0.14, -0.89, 0.41, ... ] ──[ HNSW Graph ]──> Top-K Similar Items
```
- **Examples:** Pinecone, Milvus, Qdrant, `pgvector`.
- **Engine:** Approximate Nearest Neighbor (ANN) graphs (HNSW) and quantization (IVF-PQ).
- **Core Strengths:** Fast mathematical similarity search (Cosine, Euclidean distance) in high-dimensional space.
- **When to Use:** LLM Retrieval-Augmented Generation (RAG), semantic document search, image/audio matching.
- **Limitation:** High RAM consumption; approximate search rather than exact.

---

### 9. Search Engines (Inverted Index Stores)
```
"distributed" ──> [ Doc 1, Doc 4, Doc 9 ]
"databases"   ──> [ Doc 1, Doc 2, Doc 4 ]
```
- **Examples:** Elasticsearch, OpenSearch, Apache Solr.
- **Engine:** Apache Lucene Inverted Index (Token $\to$ Posting Lists) + BM25 scoring.
- **Core Strengths:** Sub-second full-text fuzzy search, typo tolerance, log analytics (ELK stack).
- **When to Use:** E-commerce search bars, centralized log aggregation, autocomplete.
- **Limitation:** High memory overhead; not ACID compliant (1s segment flush delay).

---

### 10. NewSQL Databases
- **Examples:** Google Spanner, CockroachDB, TiDB.
- **Engine:** Multi-Raft consensus + atomic hardware clocks (TrueTime) / Hybrid Logical Clocks.
- **Core Strengths:** Horizontal sharding of NoSQL combined with strict ACID relational SQL.
- **When to Use:** Global multi-region financial platforms needing horizontal scale without losing ACID.
- **Limitation:** Cross-region write latency (~50–150ms roundtrip); operational complexity.

---

# 3. Quick Decision Summary

- **Strict Transactions / Financials?** $\implies$ **PostgreSQL / MySQL (RDBMS)**
- **Sub-Millisecond Cache / Sessions?** $\implies$ **Redis (Key-Value)**
- **Dynamic JSON / Fast Schema Iteration?** $\implies$ **MongoDB (Document)**
- **Ultra-High Write Volume (>100k/s)?** $\implies$ **Cassandra (Wide-Column)**
- **Social Graph / Fraud Rings?** $\implies$ **Neo4j (Graph)**
- **Analytics / Aggregations over Billions of Rows?** $\implies$ **ClickHouse (Columnar)**
- **Fuzzy Full-Text Search / Logs?** $\implies$ **Elasticsearch (Search Engine)**
- **DevOps Metrics / IoT Telemetry?** $\implies$ **InfluxDB / Prometheus (TSDB)**
- **AI / LLM Semantic Search (RAG)?** $\implies$ **Pinecone / pgvector (Vector DB)**
- **Global Multi-Region ACID Scale?** $\implies$ **Google Spanner / CockroachDB (NewSQL)**
