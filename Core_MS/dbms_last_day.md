# DBMS — Last-Day Revision & Flashcards (Microsoft Prep)

> **Goal**: 15-minute high-yield cram sheet. Review these flashcards out loud, then review the core formulas, tables, and top traps.

---

## Part 1: Top 20 High-Yield DBMS Flashcards

### Card 1: How does a database engine physically achieve Atomicity and Durability?
- **Answer**: Through **Write-Ahead Logging (WAL)**.
  - **Atomicity**: If a transaction aborts or fails, the engine uses **Undo Logs** to reverse in-memory modifications.
  - **Durability**: Before `COMMIT` returns success, transaction modifications are flushed sequentially to **Redo Logs** on disk. On restart after a crash, the engine replays redo logs.

### Card 2: What is a Checkpoint in database recovery?
- **Answer**: A periodic background operation where the DBMS flushes all modified (dirty) buffer pool pages in RAM out to table files on disk and records the Log Sequence Number (LSN). During crash recovery, the DBMS only needs to replay WAL logs recorded **after the last checkpoint**, bounding recovery time.

### Card 3: What is the difference between a Clustered and a Non-Clustered Index?
- **Answer**:
  - **Clustered Index**: Determines the **physical storage order** of rows on disk. Leaf nodes **are** the actual data rows. There can be only **one** per table (usually Primary Key).
  - **Non-Clustered (Secondary) Index**: A separate B+ tree structure. Leaf nodes store the indexed key columns plus a **Row Locator** (a pointer to the Clustered Key or physical RID).

### Card 4: Why do database engines use B+ Trees instead of B-Trees?
- **Answer**:
  1. **Higher Fan-out & Lower Height**: B+ Tree internal nodes store *only keys and child pointers* (no data payloads), fitting hundreds of keys per 16KB disk page. A 3–4 level tree can index billions of rows with just 3–4 disk I/Os.
  2. **Fast Sequential Range Scans**: All leaf nodes are linked in a **doubly-linked list**. Range queries (`BETWEEN`, `>`, `<`) perform one $O(\log N)$ point lookup to the start leaf and then scan sequentially ($O(K)$) without re-traversing the tree.

### Card 5: What is a Covering Index?
- **Answer**: An index that contains all the columns referenced in a query (in `SELECT`, `WHERE`, `JOIN`). The query engine fulfills the query directly from the secondary index leaves, completely avoiding an expensive secondary lookup (**Bookmark / Key Lookup**) into the clustered index.

### Card 6: What is the Leftmost Prefix Rule for composite indexes?
- **Answer**: A composite index on `(A, B, C)` can only be utilized if the query filters from left to right without gaps. Queries with `WHERE A = 1` or `WHERE A = 1 AND B = 2` use the index. Queries filtering on `WHERE B = 2` or `WHERE C = 3` **cannot** use the index.

### Card 7: What is the difference between a Dirty Read, a Non-Repeatable Read, and a Phantom Read?
- **Answer**:
  - **Dirty Read**: Reading uncommitted data written by another transaction (which might be rolled back).
  - **Non-Repeatable (Fuzzy) Read**: Re-reading the *same row* within a transaction returns altered column values because another transaction updated and committed it.
  - **Phantom Read**: Re-running a *range query* returns a different set of rows because another transaction inserted or deleted rows matching the range criteria.

### Card 8: How does MySQL InnoDB prevent Phantom Reads in Repeatable Read?
- **Answer**: By combining **MVCC (Multi-Version Concurrency Control)** for standard snapshot reads with **Next-Key Locking** (which locks both the index record and the "gap" between index records) during locking reads (`SELECT ... FOR UPDATE`), preventing concurrent inserts into the range.

### Card 9: What is the core premise of MVCC?
- **Answer**: *"Readers never block writers, and writers never block readers."* When rows are updated, old versions are preserved in undo logs. Queries read a consistent snapshot corresponding to the database state at their transaction start timestamp without acquiring shared read locks.

### Card 10: Pessimistic Locking vs. Optimistic Concurrency Control (OCC)?
- **Answer**:
  - **Pessimistic**: Locks records on read (`SELECT ... FOR UPDATE`). Best for **high write contention** where collisions are frequent (e.g. banking transfers).
  - **Optimistic**: Does not lock on read. Includes a `version` column and checks `WHERE id = 1 AND version = @oldVersion` during update. If 0 rows updated, caller aborts/retries. Best for **read-heavy / low contention** workloads.

### Card 11: Explain Normalization up to 3NF in 10 seconds.
- **Answer**:
  - **1NF**: Atomic values per column (no lists/repeating groups), unique primary key.
  - **2NF**: In 1NF + **No partial dependencies** (every non-key attribute depends on the *entire* composite primary key).
  - **3NF**: In 2NF + **No transitive dependencies** (non-key attributes depend *only* on the primary key, never on other non-key attributes).

### Card 12: Why and when would you Denormalize a database?
- **Answer**: Normalization eliminates redundancy but forces queries to perform complex, expensive multi-table `JOIN` operations. In high-throughput, **read-heavy production systems**, tables are deliberately denormalized (e.g. embedding user names in the orders table) to achieve single-table fast lookups, trading storage space and update synchronization complexity for read performance.

### Card 13: What are the 3 physical JOIN algorithms used by database query engines?
- **Answer**:
  1. **Nested Loop Join**: For each row in outer table, scan inner table ($O(M \log N)$ if inner is indexed). Best when one table is small and inner table has an index on join key.
  2. **Hash Join**: Builds an in-memory hash table on the smaller table's join keys, then probes it while scanning the larger table ($O(M+N)$). Best for large, unsorted, unindexed equality joins.
  3. **Sort-Merge Join**: Sorts both tables on join key, then steps through both linearly ($O(M \log M + N \log N)$). Best when inputs are already sorted or for range/inequality joins.

### Card 14: What is the difference between a Primary Key and a Unique Key?
- **Answer**:
  - **Primary Key**: Uniquely identifies each row, **cannot contain NULL values**, exactly **one** allowed per table, creates a **clustered index** by default.
  - **Unique Key**: Enforces uniqueness, **allows NULL values** (one or more depending on SQL dialect), **multiple** unique keys allowed per table, creates a non-clustered index by default.

### Card 15: What is the trade-off of adding database indexes?
- **Answer**: Indexes accelerate `SELECT` read queries from $O(N)$ full-table scans to $O(\log N)$ tree lookups. However, they **slow down `INSERT`, `UPDATE`, and `DELETE` operations** (because every index on the table must be updated atomically) and consume significant disk and memory buffer pool cache.

### Card 16: What is the difference between Two-Phase Locking (2PL) and Two-Phase Commit (2PC)?
- **Answer**:
  - **2PL (Concurrency Control)**: A single-database transaction protocol with a **Growing Phase** (acquiring locks) and a **Shrinking Phase** (releasing locks) ensuring serializability.
  - **2PC (Distributed Consensus)**: An atomic commitment protocol ensuring a transaction commits across **multiple distributed database nodes** via a Coordinator (Prepare Phase $\to$ Commit Phase).

### Card 17: What is Database Sharding vs. Read Replicas?
- **Answer**:
  - **Read Replicas**: Master-replica replication where writes hit the primary and reads are offloaded to read-only replicas. Solves read throughput; subject to **replication lag**.
  - **Sharding**: Horizontal partitioning of both reads and writes across separate database instances based on a **Shard Key** (e.g. `user_id % 4`). Solves storage capacity and write bottlenecks; makes cross-shard joins difficult.

### Card 18: What is Write Skew in transaction isolation?
- **Answer**: An anomaly occurring under Snapshot Isolation where two concurrent transactions read overlapping data, satisfy local constraints, and make disjoint updates that together violate a global constraint (e.g., two doctors simultaneously taking on-call duty off because each saw that two doctors were active, leaving zero doctors on duty). Solved only by `Serializable` isolation or explicit locking.

### Card 19: What is the difference between `TRUNCATE`, `DROP`, and `DELETE`?
- **Answer**:
  - **`DELETE`**: DML command. Deletes specified rows one-by-one, logs each deletion in transaction log (can be rolled back), triggers run, slowest.
  - **`TRUNCATE`**: DDL command. Deallocates data pages directly, minimally logged (cannot be easily rolled back in some engines), resets auto-increment counter, much faster.
  - **`DROP`**: DDL command. Completely destroys the table schema, indexes, data, and constraints from the database catalog.

### Card 20: When should you choose NoSQL over SQL?
- **Answer**: Choose **NoSQL** when you need:
  1. Horizontal auto-sharding at massive scale (petabytes/billions of writes).
  2. Flexible, dynamic, or nested schema (JSON documents).
  3. High write throughput with tunable/eventual consistency (BASE over strict ACID).
  Choose **SQL** for relational integrity, multi-table transactions, and complex analytical reporting.

---

## Part 2: 1-Minute Memory Matrix & Key Points

### ANSI SQL Isolation Anomaly Matrix:
| Isolation Level | Dirty Read | Non-Repeatable Read | Phantom Read |
|---|---|---|---|
| **Read Uncommitted** | Yes (Dirty data) | Yes | Yes |
| **Read Committed** | Prevented | Yes (Row values change) | Yes |
| **Repeatable Read** | Prevented | Prevented | Yes (Default SQL)* |
| **Serializable** | Prevented | Prevented | Prevented |

*\*MySQL InnoDB prevents Phantom Reads in Repeatable Read using Next-Key Locks + MVCC.*

### B+ Tree vs B-Tree Quick Comparison:
| Feature | B-Tree | B+ Tree |
|---|---|---|
| Data Pointers | In internal AND leaf nodes | **Leaf nodes ONLY** |
| Fan-Out | Lower (due to large row data in nodes) | **Extremely High** (more keys fit per page) |
| Tree Height | Taller | **Shorter (typically 3–4 levels)** |
| Range Queries | Slow (repeated tree in-order traversals) | **Blazing fast (doubly-linked leaf nodes)** |

---

## Part 3: Top 5 Microsoft Traps in DBMS

1. **Forgetting How Secondary Indexes Store References**:
   - In engines like MySQL InnoDB, a secondary index leaf does **not** store the physical disk address; it stores the **Clustered Index Key**. Hence, secondary lookups trigger a second traversal through the Clustered Index (unless covered).
2. **Assuming Range Queries Use All Columns in a Composite Index**:
   - For an index on `(A, B)`: `WHERE A > 10 AND B = 5` uses the index **only for `A`**. Column `B` cannot be used for index filtering because `A` is a range comparison, destroying sort order for `B`.
3. **Confusing 2PL (Locking) with 2PC (Distributed Commit)**:
   - 2PL handles local concurrency control (locks); 2PC handles distributed transaction agreement across network nodes.
4. **Believing Indexes Speed Up Everything**:
   - Over-indexing destroys write throughput and buffer pool efficiency. Every `INSERT`/`UPDATE` must maintain every associated B+ tree.
5. **Ignoring Replication Lag**:
   - When using primary-replica setups, a user updating their profile and immediately refreshing might see old data if the read replica has not caught up with the primary's binlog. Fix: Read-your-own-writes routing to primary.
