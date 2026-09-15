# 03. SQL vs NoSQL, Storage Engines & Concurrency

> **Interview Mindset**: Choosing a database is not about brand names; it is about matching **query access patterns, schema flexibility, read/write ratios, and consistency requirements** to the engine's internal physics.

---

## 1. SQL vs NoSQL: The 5-Step Decision Framework

Whenever an interviewer asks: *"Which database would you pick for this service and why?"*, walk through this checklist:

```
┌────────────────────────────────────────────────────────────────────────┐
│                   The 5-Step DB Selection Framework                    │
└────────────────────────────────────────────────────────────────────────┘
  │ 1. Schema Stability: Rigid relational entities OR Polymorphic JSON?
  │ 2. Query Patterns: Complex multi-table JOINs OR simple Key Lookups?
  │ 3. Read vs Write Ratio: Read-heavy with indexes OR Append-only writes?
  │ 4. ACID Integrity: Strict financial consistency OR Eventual consistency?
  │ 5. Scale Trajectory: Standard vertical scale OR Petabyte horizontal clusters?
```

| Dimension | Relational (SQL) | Non-Relational (NoSQL) |
| :--- | :--- | :--- |
| **Data Structure** | Structured tables with strict schemas and foreign keys | JSON documents, Key-Value pairs, or Column families |
| **Query Engine** | Powerful joins, aggregations, flexible ad-hoc filtering | Simple key lookups, pre-computed query paths |
| **ACID Guarantees** | Full ACID transactions out of the box | Usually BASE (Basically Available, Soft-state, Eventual) |
| **Scaling** | Vertical scaling + Read Replicas (covers 95% of apps) | Native horizontal auto-sharding across massive clusters |
| **When to Choose** | E-commerce orders, banking, user accounts, SaaS ERP | Social feeds, IoT metrics, chat history, caching, catalogs |

---

## 2. The 4 Major NoSQL Families

```
┌─────────────────────────┐     1. Document Store (MongoDB, Couchbase): Dynamic JSON, product catalogs.
│                         ├─────►2. Key-Value Store (Redis, Memcached): In-memory, sub-ms caching & tokens.
│     The 4 NoSQL         ├─────►3. Wide-Column / Columnar (Cassandra, ClickHouse): High-write throughput & OLAP.
│       Families          └─────►4. Graph Database (Neo4j): Highly interconnected nodes, social graphs, fraud rings.
└─────────────────────────┘
```

1. **Document Store (MongoDB)**:
   - *Data Model*: BSON/JSON nested documents.
   - *Best For*: Product catalogs with variable attributes (laptops have RAM/CPU, shoes have size/color), content management.
2. **Key-Value Store (Redis, DynamoDB)**:
   - *Data Model*: Simple `key -> value` lookup in memory (RAM).
   - *Best For*: Caching, session tokens, distributed locks, rate-limiting counters.
3. **Wide-Column (Cassandra, ScyllaDB) & Columnar (ClickHouse)**:
   - *Cassandra*: High write throughput, distributed time-series data, chat messages.
   - *ClickHouse / Snowflake*: Columnar storage for analytics (scanning specific columns instead of full rows).
4. **Graph Database (Neo4j)**:
   - *Data Model*: Nodes, Edges, and Properties.
   - *Best For*: Finding 2nd and 3rd-degree connections, social graphs, fraud detection networks.

---

## 3. Storage Engine Internals: B+ Tree vs LSM-Tree

```
       B+ TREE (PostgreSQL, MySQL InnoDB)                LSM-TREE (Cassandra, RocksDB)
               [Root Node]                                   Client Write
                 /    \                                           │
             [Internal Nodes]                             ┌───────▼───────┐
              /     |     \                               │ MemTable(RAM) │
          [Leaf]─[Leaf]─[Leaf] (Pages on Disk)            └───────┬───────┘
          Reads: O(log N) - In-place updates                      │ Flush (Sequential)
                                                              ┌───▼───────────┐
                                                              │ SSTables(Disk)│
                                                              └───┬───────────┘
                                                           Compaction (Merge)
```

| Feature | B+ Tree (Read-Optimized) | LSM-Tree (Write-Optimized) |
| :--- | :--- | :--- |
| **Typical DBs** | MySQL (InnoDB), PostgreSQL, SQLite, Oracle | Apache Cassandra, RocksDB, ScyllaDB |
| **Write Flow** | **In-place updates**: Directly modifies fixed-size 8KB/16KB disk pages. May trigger random disk I/O. | **Append-only**: Writes straight to memory `MemTable` + sequential Write-Ahead Log. Flushes to disk as `SSTables`. |
| **Write Speed** | Slower (random disk I/O, page splits) | **Blazing fast** (pure sequential disk writes) |
| **Read Speed** | **Fastest** ($O(\log N)$ tree traversal directly to target page) | Slower (may check multiple SSTables on disk) |
| **Read Helper** | Buffer Pool caching hot disk pages | **Bloom Filters** (probabilistic check to avoid reading SSTables that don't have the key) |

---

## 4. Indexing at Scale: Rules & Traps

### 1. Clustered vs Secondary Indexes
- **Clustered Index (Primary Key)**: Dictates the physical disk order of the rows. A table can only have **one** clustered index. The leaf node *is* the actual row data.
- **Secondary Index**: An independent B+ tree. The leaf node stores the indexed value + the clustered index pointer.

### 2. The Leftmost Prefix Rule (Composite Indexes)
If you create an index on `(country, state, city)`:
```sql
CREATE INDEX idx_user_geo ON users (country, state, city);
```
- `WHERE country = 'IN'` ──► **USES INDEX** (Matches leftmost column).
- `WHERE country = 'IN' AND state = 'KA'` ──► **USES INDEX** (Matches leftmost prefix).
- `WHERE state = 'KA'` ──► **FULL TABLE SCAN** (Skips leftmost column; index is unusable!).

### 3. Covering Indexes (Index-Only Scan)
If your query selects **only** columns that already exist inside the index:
```sql
SELECT state FROM users WHERE country = 'IN';
```
The database never visits the disk table at all; it resolves the query 100% inside the index memory.

### 4. The Write Penalty
Every index on a table must be updated on every `INSERT`, `UPDATE`, and `DELETE`.
> *"Indexes make reads fast, but too many indexes slow down writes and waste memory."*

---

## 5. Transactions, Isolation Levels & MVCC

### The 4 SQL Isolation Levels & Concurrency Anomalies

```
       CONCURRENCY ANOMALIES                       ISOLATION LEVEL THAT PREVENTS IT
1. Dirty Read            (Read uncommitted data)       ──► Read Committed
2. Non-Repeatable Read  (Row value changes mid-tx)    ──► Repeatable Read
3. Phantom Read         (New rows appear mid-tx)       ──► Serializable (or InnoDB Next-Key Lock)
4. Write Skew           (Concurrent disjoint checks)   ──► Serializable
```

| Isolation Level | Dirty Read | Non-Repeatable Read | Phantom Read | Write Skew | Default In |
| :--- | :---: | :---: | :---: | :---: | :--- |
| **Read Uncommitted** | ❌ Allowed | ❌ Allowed | ❌ Allowed | ❌ Allowed | Rarely used |
| **Read Committed** | ✅ **Prevented** | ❌ Allowed | ❌ Allowed | ❌ Allowed | PostgreSQL, Oracle |
| **Repeatable Read** | ✅ **Prevented** | ✅ **Prevented** | ⚠️ Partial | ❌ Allowed | MySQL (InnoDB) |
| **Serializable** | ✅ **Prevented** | ✅ **Prevented** | ✅ **Prevented** | ✅ **Prevented** | Explicit |

---

### MVCC (Multi-Version Concurrency Control)
> *"Readers never block writers, and writers never block readers."*

- In PostgreSQL and MySQL InnoDB, updating a row does **not** overwrite the old row in place.
- It inserts a **new version** tagged with transaction IDs (`xmin`/`xmax`).
- A `SELECT` query reads a consistent **Snapshot** of data committed before the query started, ignoring concurrent in-flight writes without acquiring any read locks!

---

### Optimistic vs Pessimistic Locking
- **Pessimistic Locking**: Locks the row upfront (`SELECT ... FOR UPDATE`). Safe, but blocks concurrent transactions. Best for high-contention inventory.
- **Optimistic Locking**: Reads without locks, checks a `version` column before updating:
  ```sql
  UPDATE accounts 
  SET balance = balance - 50, version = version + 1 
  WHERE id = 101 AND version = 3;
  ```
  If affected rows = 0, another transaction updated the row first; retry!

---

### Distributed Transactions: 2PC vs Saga Pattern & Idempotency

```
┌─────────────────────────────────────────────────────────────┐
│                   Transaction Coordinator                   │
└──────────────┬───────────────────────────────┬──────────────┘
               │ Phase 1: Prepare to commit?   │ Phase 1: Prepare to commit?
               ▼                               ▼
┌─────────────────────────────┐ ┌─────────────────────────────┐
│         DB Shard 1          │ │         DB Shard 2          │
└──────────────┬──────────────┘ └──────────────┬──────────────┘
               │ Phase 2: Commit! / Abort!     │ Phase 2: Commit! / Abort!
               ▼                               ▼
┌─────────────────────────────────────────────────────────────┐
│           Both Shards Commit Simultaneously                 │
└─────────────────────────────────────────────────────────────┘
```

- **Saga Pattern**: Replaces synchronous 2PC in distributed microservices. Executes a sequence of local transactions. If a later step fails, it executes **Compensating Transactions** (e.g., refund payment) to roll back.
- **Idempotency**: Clients attach a unique **Idempotency Key** (UUID) to API requests. If a network timeout occurs, the retry checks the key and returns the original result without charging or inserting twice.
