# DBMS — High ROI Revision (Microsoft Interview Prep)

> **Mindset**: You already know basic definitions. Microsoft interviews probe **internals**: how engines guarantee ACID via WAL, why engines use B+ Trees over B-Trees, Clustered vs Secondary indexes, and exact Isolation anomalies (Dirty Read vs Fuzzy Read vs Phantom Read vs Write Skew).

---

## 1. ACID Properties — Real Database Implementation Mechanics

Interviewers don't just ask what ACID stands for; they ask: **"How does the database engine actually guarantee each property?"**

| Property | Meaning | How the DBMS Implements It |
|---|---|---|
| **Atomicity** | All-or-nothing execution | **Undo Logs & WAL (Write-Ahead Logging)**: If a transaction fails mid-flight, the engine reads undo logs to revert all modified pages to their pre-transaction state. |
| **Consistency** | Database moves from one valid state to another, preserving invariants | **Schema Constraints** (Foreign Keys, Unique, Check constraints) + **Atomicity & Isolation** preventing dirty partial states. |
| **Isolation** | Concurrent transactions execute without mutual interference | **Concurrency Control**: Either **2PL (Two-Phase Locking)** or modern **MVCC (Multi-Version Concurrency Control)**. |
| **Durability** | Committed transactions survive system crashes & power loss | **Redo Logs & WAL (Write-Ahead Logging)**: Changes are flushed sequentially to the append-only transaction log on disk *before* dirty pages are written to table files. On crash recovery, the engine replays redo logs. |

### The Golden Concept: Write-Ahead Logging (WAL)
- Writing modified data pages randomly to disk is slow (random disk I/O).
- Instead, the DBMS appends transaction changes to a sequential **WAL log buffer** and flushes it to disk at `COMMIT` time (sequential I/O is 100x faster).
- **Checkpoints**: Periodically flushes dirty in-memory data pages to disk and truncates older WAL logs, preventing indefinite log growth and speeding up crash recovery.

---

## 2. Indexing Internals — B-Tree vs. B+ Tree & Clustered Indexes

### Clustered Index vs. Non-Clustered (Secondary) Index

| Dimension | Clustered Index | Non-Clustered (Secondary) Index |
|---|---|---|
| **Storage Structure** | **The table is the index**. Rows are physically sorted by the clustered key. | Separate B+ tree structure completely disconnected from row physical order. |
| **Count Per Table** | **Exactly 1** (by default, the Primary Key). | Multiple allowed (typically 5–10 max). |
| **Leaf Node Content** | **The actual table row data** (all columns). | The indexed key columns + a **Row Locator** (pointer to the Clustered Key or physical RID). |
| **Lookup Cost** | Direct lookup: 1 tree traversal ($O(\log N)$). | 2 lookups: Traverses secondary index ➔ extracts clustered key ➔ traverses clustered index (**Bookmark / Key Lookup**). |

> **What is a Covering Index?**
> If a query selects only columns that exist inside the secondary index (e.g. `SELECT id, email FROM users WHERE email = 'x'`), the engine retrieves data directly from the secondary index leaves without performing an expensive clustered key lookup.

---

### Why do Databases use B+ Trees instead of B-Trees or Binary Trees?
1. **High Fan-Out & Low Tree Height**:
   - In a **B-Tree**, every node stores both the search key and the actual data row/pointer.
   - In a **B+ Tree**, internal nodes store **only keys and child pointers**; all actual data pointers are pushed to **leaf nodes**.
   - *Result*: A 16KB database page can pack hundreds of keys per node. Fan-out is huge ($M \approx 1000$). A 3-to-4 level B+ tree can index **billions of rows in just 3–4 disk I/Os**.
2. **Linked Leaf Nodes (Blazing Fast Range Scans)**:
   - In a B+ Tree, all leaf nodes form a **doubly-linked list**.
   - To execute `SELECT * FROM users WHERE age BETWEEN 20 AND 30`:
     Traverse the tree once to find `age = 20` ($O(\log N)$), then simply walk the linked list sequentially until `30` ($O(K)$). A standard B-Tree would require repeated in-order tree traversals across disk blocks.

---

### Composite Indexes & The Leftmost Prefix Rule
If you create an index on `(A, B, C)`:
- Uses index: `WHERE A = 1`, `WHERE A = 1 AND B = 2`, `WHERE A = 1 AND B = 2 AND C = 3`, `WHERE A = 1 ORDER BY B`
- **Cannot** use index: `WHERE B = 2`, `WHERE C = 3`, `WHERE B = 2 AND C = 3` (skips leading column `A`).

---

## 3. Transactions, Isolation Levels & Anomaly Matrix

### The 4 Standard ANSI Isolation Levels:

| Isolation Level | Dirty Read | Non-Repeatable (Fuzzy) Read | Phantom Read | Mechanism Under the Hood |
|---|---|---|---|---|
| **Read Uncommitted** | Allowed | Allowed | Allowed | No read locks; reads latest memory state. |
| **Read Committed** | **Prevented** | Allowed | Allowed | Reads committed data snapshot at the start of *each query*. |
| **Repeatable Read** | **Prevented** | **Prevented** | Allowed* | Reads committed snapshot pinned at start of *entire transaction*. |
| **Serializable** | **Prevented** | **Prevented** | **Prevented** | Strict 2-Phase Locking (2PL) or Range Locks. Lowest concurrency. |

*\*Note on MySQL InnoDB: Repeatable Read in InnoDB uses **Next-Key Locking** (record locks + gap locks) and MVCC, which prevents Phantom Reads in practice!*

### The 3 Classic Anomalies Defined:
1. **Dirty Read**: Transaction A reads data modified by Transaction B that has **not yet committed**. If Transaction B rolls back, Transaction A operated on garbage data.
2. **Non-Repeatable (Fuzzy) Read**: Transaction A reads a row. Transaction B **updates** that same row and commits. Transaction A re-reads the row and observes altered values.
3. **Phantom Read**: Transaction A executes a range query (`WHERE age > 25`, returns 5 rows). Transaction B **inserts or deletes** a row matching the range and commits. Transaction A re-runs query and observes a different number of rows (a "phantom" row appeared).

---

## 4. Concurrency Control: 2PL vs. MVCC vs. Optimistic Locking

### MVCC (Multi-Version Concurrency Control)
- *"Readers never block writers, and writers never block readers."*
- When a row is updated, the engine does not overwrite it in place; it writes a new version with a transaction ID (`xmin` / `xmax`).
- Queries see a consistent snapshot of the database at their transaction's start time without holding read locks. Used by PostgreSQL, MySQL (InnoDB), and Oracle.

### Pessimistic vs. Optimistic Locking
- **Pessimistic Locking**:
  - Assumes collisions will happen. Locks records immediately upon reading (`SELECT ... FOR UPDATE`).
  - *Best for*: High write contention, banking transactions where rollbacks are costly.
- **Optimistic Concurrency Control (OCC)**:
  - Assumes collisions are rare. No locks on read. Adds a `version` column.
  - On update: `UPDATE accounts SET balance = 100, version = version + 1 WHERE id = 1 AND version = 5;`
  - If rows affected == 0, another transaction updated it first; caller aborts/retries.
  - *Best for*: Read-heavy workloads, web APIs, distributed systems.

---

## 5. Normalization vs. Denormalization (Production Perspective)

### Normal Forms Summary:
- **1NF**: Atomic values per column (no comma-separated lists), unique primary key.
- **2NF**: In 1NF + **No partial dependencies** (every non-key column depends on the *whole* composite primary key).
- **3NF**: In 2NF + **No transitive dependencies** (non-key columns must not depend on other non-key columns).
- **BCNF**: Stricter 3NF: For every functional dependency $X \to Y$, $X$ must be a super key.

### Why Denormalize in Real-World Systems?
Normalization eliminates redundancy and write anomalies, but introduces deep, costly multi-table `JOIN` operations.
- **When to Denormalize**: Read-heavy high-traffic services (e.g., storing `user_name` directly in `orders` table to avoid joining `users` on every order list query). Tradeoff: Higher storage and requires syncing redundant copies on update.

---

## 6. Physical Join Algorithms (How Engines Execute JOINs)

When you write `INNER JOIN`, the query optimizer chooses one of three physical algorithms:

1. **Nested Loop Join**:
   - For each outer row, scans inner table. $O(M \times N)$ naive, but $O(M \log N)$ if inner table has an index on the join key.
   - *Best when*: One table is very small, and the other table is indexed.
2. **Hash Join**:
   - Builds an in-memory hash table on the smaller table's join key, then scans the larger table probing the hash table. $O(M + N)$.
   - *Best when*: Joining large, unsorted, unindexed tables for equality (`=`).
3. **Sort-Merge Join**:
   - Sorts both tables on the join key (if not already indexed), then scans both in linear parallel. $O(M \log M + N \log N)$.
   - *Best when*: Both inputs are already sorted or indexed, or when performing inequality joins (`<`, `>`).

---

## 7. SQL vs. NoSQL & Horizontal Scaling

| Dimension | Relational (SQL) | NoSQL (Mongo, Cassandra, Redis) |
|---|---|---|
| **Data Model** | Structured tables, strict schema, foreign keys | Key-Value, Document (JSON), Wide-Column, Graph |
| **Transactions** | Strong ACID out of the box | Eventual Consistency (BASE) / Tunable consistency |
| **Scaling** | Primarily **Vertical** (bigger machines); read replicas for reads | Primarily **Horizontal** (native data sharding across clusters) |
| **Complex Queries**| Rich declarative SQL, multi-table joins | Optimized for single-key lookups; joins avoided/denormalized |

### Database Scaling Strategies:
- **Read Replicas (Primary-Replica)**: Primary receives all writes; changes replicate asynchronously to replicas which handle read traffic. Caveat: **Replication lag** can lead to stale reads.
- **Sharding (Horizontal Partitioning)**: Splitting table rows across multiple distinct database servers based on a **Shard Key** (e.g. `hash(user_id) % num_shards`).
  - *Risk*: Shard hotspots (one tenant generating 90% of traffic) and cross-shard joins are extremely complex/slow.

---

## Rapid-Fire Interview Drill (Answer in 30 Seconds)
1. *Why does B+ Tree perform better than B-Tree for range queries?* ➔ All leaf nodes are linked in a continuous doubly-linked list, enabling sequential disk scans without re-traversing tree branches.
2. *What is the difference between a Non-Repeatable Read and a Phantom Read?* ➔ Non-repeatable read occurs when an existing row is *modified/updated*; phantom read occurs when new rows matching a range are *inserted or deleted*.
3. *What is a Covering Index?* ➔ An index that contains all the columns requested by a query, allowing the engine to return results without a secondary lookup into the clustered index.
4. *How does WAL guarantee Durability?* ➔ Changes are written sequentially to the append-only disk log before transactions commit; after a crash, the engine replays the log to restore committed transactions.
5. *Why shouldn't you index every column in a table?* ➔ Every index adds write overhead (every `INSERT`/`UPDATE`/`DELETE` must update all indexes) and consumes significant disk/RAM buffer pool space.