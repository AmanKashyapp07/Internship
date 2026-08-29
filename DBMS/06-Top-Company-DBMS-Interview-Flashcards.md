# Master Guide 06: Top 40 DBMS Spoken Flashcards & Trap Questions

> **Focus:** 40 High-Yield Spoken Flashcards for Database Engineering interviews at Google, Meta, Amazon, Microsoft, Stripe, and FinTech/HFT firms.
> 
> *The 15-minute complete verbal drill to read one day before any tech interview.*

---

# Table of Contents
1. [ACID, Storage Engines & MVCC Internals (Cards 1–8)](#1-acid-storage-engines--mvcc-internals-cards-18)
2. [Indexing, B+ Trees & Query Optimization (Cards 9–16)](#2-indexing-b-trees--query-optimization-cards-916)
3. [SQL Query Archetypes & Logic Traps (Cards 17–24)](#3-sql-query-archetypes--logic-traps-cards-1724)
4. [Normalization, Locking & Concurrency Control (Cards 25–32)](#4-normalization-locking--concurrency-control-cards-2532)
5. [Distributed Databases, Sharding & NoSQL (Cards 33–40)](#5-distributed-databases-sharding--nosql-cards-3340)

---

# 1. ACID, Storage Engines & MVCC Internals (Cards 1–8)

### Card 1: "What is the fundamental difference between Atomicity and Isolation?"
> **Spoken Answer:** **Atomicity** guarantees that all operations within a single transaction complete or all roll back via Undo logs (dealing with single-transaction failure). **Isolation** guarantees that concurrently executing transactions do not observe each other's in-flight uncommitted state (dealing with concurrency). A system can have 100% Atomicity with 0% Isolation (e.g. Read Uncommitted).

---

### Card 2: "What is Write Skew, and why doesn't Snapshot Isolation prevent it?"
> **Spoken Answer:** **Write Skew** occurs under Snapshot Isolation when two concurrent transactions read overlapping data, validate business constraints independently, and update disjoint rows that together violate a global invariant (e.g. withdrawing from checking and savings simultaneously until total balance $< 0$). Snapshot Isolation only detects concurrent writes to the *exact same row*, so it fails to catch Write Skew. Fix: Use Serializable isolation or explicit row locks (`SELECT ... FOR UPDATE`).

---

### Card 3: "How does MVCC eliminate read-write locking contention?"
> **Spoken Answer:** Instead of updating rows in-place under locks, Multi-Version Concurrency Control creates new timestamped versions of rows on update. Readers read a consistent historical snapshot corresponding to their transaction start time without acquiring locks. Therefore, **readers never block writers, and writers never block readers**.

---

### Card 4: "What is the Golden Rule of Write-Ahead Logging (WAL)?"
> **Spoken Answer:** A dirty data page in the RAM Buffer Pool can **never be flushed to disk** until the corresponding Redo Log (WAL) record describing the modification has been flushed to disk (`fsync`). This ensures that if the system crashes, the database can replay the WAL during ARIES recovery to restore committed state.

---

### Card 5: "What is XID Wraparound in PostgreSQL and how is it prevented?"
> **Spoken Answer:** PostgreSQL uses 32-bit transaction IDs (XIDs), which wrap around after 4.2 billion transactions. If left unchecked, past historical transactions would appear in the future and become invisible. PostgreSQL's background `VACUUM` process prevents this by **freezing old tuple XIDs** (marking them with a special `FrozenXID` bit), treating them as permanently in the past.

---

### Card 6: "How does Postgres SSI (Serializable Snapshot Isolation) achieve serializability without 2PL locks?"
> **Spoken Answer:** SSI tracks non-blocking SIREAD locks in RAM and constructs an in-memory dependency graph of read-write anti-dependencies ($rw$-antidependencies). If the engine detects two consecutive $rw$-antidependency edges in a cycle ($T_1 \xrightarrow{rw} T_2 \xrightarrow{rw} T_3$), it automatically aborts one transaction with a serialization error, eliminating Write Skew with zero lock contention.

---

### Card 7: "What is the Slotted Page storage architecture and why is it used?"
> **Spoken Answer:** Disk pages are partitioned into a header at the top storing an array of slot pointers and actual row payloads growing upward from the bottom. Secondary indexes store pointers to `(PageID, SlotNumber)` rather than direct byte offsets, allowing rows inside a page to be rearranged or compacted without updating external secondary index pointers.

---

### Card 8: "What is the difference between Write-Ahead Logging (Redo) and Undo Logging?"
> **Spoken Answer:** **Redo Logs (WAL)** record the new state to replay committed transactions that were not yet flushed from the RAM Buffer Pool to disk before a crash (**Durability**). **Undo Logs** record the original old state to roll back uncommitted transactions that aborted or crashed halfway through execution (**Atomicity**).

---

# 2. Indexing, B+ Trees & Query Optimization (Cards 9–16)

### Card 9: "Why do databases use B+ Trees instead of Binary Search Trees (BST) or standard B-Trees?"
> **Spoken Answer:** BSTs have small nodes (~24 bytes) that waste 99.7% of an 8KB disk page and yield tall tree heights (~30 disk seeks). B+ Trees store *only routing keys* in internal nodes, yielding massive fan-out ($M > 100$) and keeping height $\le 3$ for 1 billion rows. Additionally, B+ Tree leaves store all data payloads and are doubly linked together for $O(1)$ sequential range scans (`BETWEEN A AND B`).

---

### Card 10: "What is the InnoDB Double Lookup Penalty and how do you eliminate it?"
> **Spoken Answer:** In MySQL InnoDB, secondary indexes store the Primary Key as their pointer payload. Querying via a secondary index performs a first B+ Tree seek to find the Primary Key, followed by a second B+ Tree seek on the Clustered Index to retrieve full row data. This double lookup is eliminated using a **Covering Index** that includes all requested columns in the secondary index leaf node.

---

### Card 11: "What is the Leftmost Prefix Rule in Composite Indexes `(A, B, C)`?"
> **Spoken Answer:** A composite index on `(A, B, C)` sorts data lexicographically starting from `A`. Queries can only utilize the index for binary search tree seeks if their filters match columns sequentially starting from the leftmost column `A` with no gaps. A query on `WHERE B = 2 AND C = 3` cannot utilize the index for seeks and triggers a full table scan.

---

### Card 12: "When should you prefer an LSM-Tree over a B+ Tree?"
> **Spoken Answer:** Prefer **LSM-Trees (RocksDB, Cassandra)** for **write-heavy workloads**. LSM-Trees convert random writes into sequential in-memory appends (MemTable) that flush sequentially to disk (SSTables), minimizing SSD write amplification. Prefer **B+ Trees (PostgreSQL, InnoDB)** for **read-heavy transactional OLTP workloads** requiring fast point lookups and range scans.

---

### Card 13: "Why is a BRIN index ideal for time-series data in PostgreSQL?"
> **Spoken Answer:** Time-series data is naturally inserted in chronological order. A Block Range Index (BRIN) stores only the minimum and maximum values for blocks of 128 disk pages rather than indexing every single row, taking up a few kilobytes of RAM instead of gigabytes.

---

### Card 14: "Why is random UUIDv4 disastrous for B+ Tree Primary Keys?"
> **Spoken Answer:** Random UUIDv4 values scatter inserts uniformly across the entire B+ Tree. This forces frequent **leaf page splits**, reduces page fill factors to ~50%, evicts hot pages from the buffer pool, and causes massive random disk I/O. Use time-ordered **UUIDv7 or `BIGINT AUTO_INCREMENT`** instead.

---

### Card 15: "What is Index Condition Pushdown (ICP)?"
> **Spoken Answer:** ICP is an execution optimization where the database engine evaluates `WHERE` filter conditions directly inside the storage engine's index scanning loop, avoiding the overhead of fetching full table rows into server memory for records that will ultimately be discarded.

---

### Card 16: "Why does `SELECT * FROM Users WHERE email LIKE '%gmail.com'` ignore the index on `email`?"
> **Spoken Answer:** B+ Tree string indexes sort strings from left to right. A leading wildcard (`%gmail.com`) prevents the engine from performing binary search prefix matching, forcing a **full table scan**. To optimize suffix queries, store reversed strings and index `REVERSE(email)` or use Trigram / GIN indexes.

---

# 3. SQL Query Archetypes & Logic Traps (Cards 17–24)

### Card 17: "Explain SQL Logical Execution Order in 15 seconds."
> **Spoken Answer:** **`FROM` and `JOINs`** assemble the dataset $\to$ **`WHERE`** filters candidate rows $\to$ **`GROUP BY`** groups rows $\to$ **`HAVING`** filters aggregated groups $\to$ **`SELECT`** projects columns and aliases $\to$ **`DISTINCT`** deduplicates $\to$ **`ORDER BY`** sorts $\to$ **`LIMIT/OFFSET`** truncates the output.

---

### Card 18: "What is the difference between `ROW_NUMBER()`, `RANK()`, and `DENSE_RANK()`?"
> **Spoken Answer:** For tie values `[100, 100, 80]`:
> - `ROW_NUMBER()` assigns arbitrary unique numbers: `1, 2, 3`.
> - `RANK()` assigns identical ranks with gaps: `1, 1, 3` (skips rank 2).
> - `DENSE_RANK()` assigns identical ranks without gaps: `1, 1, 2`.

---

### Card 19: "Why does `WHERE id NOT IN (SELECT id FROM Orders)` fail if `Orders.id` contains a `NULL`?"
> **Spoken Answer:** In SQL three-valued logic, `x NOT IN (1, 2, NULL)` expands to `x != 1 AND x != 2 AND x != NULL`. Comparing anything to `NULL` yields `UNKNOWN`, making the entire `AND` expression evaluate to `UNKNOWN` and filtering out 100% of candidate rows. Always use **`NOT EXISTS` or `LEFT JOIN ... WHERE id IS NULL`**.

---

### Card 20: "What is the difference between `WHERE` and `HAVING`?"
> **Spoken Answer:** **`WHERE`** filters individual rows *before* grouping occurs and cannot accept aggregate functions (`WHERE salary > 50000`). **`HAVING`** filters grouped summary records *after* the `GROUP BY` clause and operates directly on aggregate values (`HAVING AVG(salary) > 50000`).

---

### Card 21: "How do you find the N-th Highest Salary in SQL handling duplicate ties?"
> **Spoken Answer:** Using a CTE with `DENSE_RANK()`:
> ```sql
> WITH Ranked AS (
>     SELECT salary, DENSE_RANK() OVER (ORDER BY salary DESC) AS rnk FROM Employees
> )
> SELECT salary FROM Ranked WHERE rnk = N LIMIT 1;
> ```

---

### Card 22: "What is the Date-Subtraction Island Trick for finding consecutive active login streaks?"
> **Spoken Answer:** By subtracting `ROW_NUMBER() * INTERVAL '1 day'` from the `login_date` (`login_date - ROW_NUMBER() OVER (PARTITION BY user_id ORDER BY login_date)`), consecutive dates produce an identical constant anchor date. Grouping by this calculated date reveals the streak length via `COUNT(*)`.

---

### Card 23: "Why should you prefer `UNION ALL` over `UNION`?"
> **Spoken Answer:** `UNION` executes an expensive deduplication sort or hash distinct operation in memory/disk across the entire combined dataset. `UNION ALL` concatenates datasets directly without deduplication, executing in fraction of the time.

---

### Card 24: "Why should you use Partial Indexes for Soft-Deleted tables?"
> **Spoken Answer:** In soft-deleted tables (`deleted_at IS NOT NULL`), 95%+ of active queries filter for `WHERE deleted_at IS NULL`. A partial index (`CREATE INDEX idx_active ON Users(email) WHERE deleted_at IS NULL`) indexes only live active rows, keeping the index small, cache-resident, and lightning-fast.

---

# 4. Normalization, Locking & Concurrency Control (Cards 25–32)

### Card 25: "What are the 1NF, 2NF, 3NF, and BCNF normalization rules in 30 seconds?"
> **Spoken Answer:**
> - **1NF:** Atomic scalar values only (no arrays or repeating groups).
> - **2NF:** 1NF + No partial dependencies (non-key columns depend on the entire composite primary key).
> - **3NF:** 2NF + No transitive dependencies (non-key columns depend *only* on candidate keys).
> - **BCNF:** 3NF + For every dependency $X \to Y$, $X$ must be a strict Super Key.

---

### Card 26: "What is the difference between Optimistic and Pessimistic Locking?"
> **Spoken Answer:** **Pessimistic Locking (`SELECT ... FOR UPDATE`)** locks rows upfront on read, blocking other transactions until commit (ideal for high-contention banking/seats). **Optimistic Locking** does not acquire database locks; it reads a `version` column and checks `WHERE version = old_version` on update, rolling back/retrying if another transaction updated the row first.

---

### Card 27: "What is a Dirty Read vs. a Non-Repeatable Read vs. a Phantom Read?"
> **Spoken Answer:**
> - **Dirty Read:** Reading uncommitted changes made by another transaction that later rolls back.
> - **Non-Repeatable Read:** Re-reading the same single row and finding updated/deleted column values.
> - **Phantom Read:** Re-executing a range query (`WHERE age > 20`) and finding brand new rows inserted by another committed transaction.

---

### Card 28: "How does MySQL InnoDB prevent Phantom Reads in Repeatable Read isolation?"
> **Spoken Answer:** Using **Next-Key Locking**, which locks both the physical index record and the "gap" in the index space before and after the record, preventing concurrent transactions from inserting new phantom rows into the scanned range.

---

### Card 29: "What is Two-Phase Locking (2PL) and does it prevent deadlocks?"
> **Spoken Answer:** 2PL is a concurrency protocol with a **Growing Phase** (acquiring locks without releasing any) and a **Shrinking Phase** (releasing locks without acquiring new ones), guaranteeing conflict serializability. **2PL DOES NOT prevent deadlocks**; transactions can still wait circularly for locks, requiring deadlock detection engines.

---

### Card 30: "How do relational databases detect and recover from deadlocks?"
> **Spoken Answer:** The engine constructs an in-memory **Wait-For Graph (WFG)** and periodically runs cycle detection (Tarjan's algorithm / DFS). When a cycle is detected, it selects a victim transaction (the one with the fewest mutations/locks held), aborts it, and rolls back its changes to break the cycle.

---

### Card 31: "Why does an unindexed Foreign Key cause database lock escalation?"
> **Spoken Answer:** When a row in the parent table is deleted or updated, the database must verify referential integrity by checking the child table. Without an index on the child's foreign key, the engine must perform a **full table scan or acquire table-level locks**, stalling all concurrent writes on the child table.

---

### Card 32: "When is Denormalization justified in database system design?"
> **Spoken Answer:** When read throughput and latency requirements outweigh write complexity (e.g. read-heavy OLAP analytics, e-commerce product pages). Storing redundant pre-joined attributes eliminates expensive multi-table joins at the expense of write amplification and potential consistency anomalies.

---

# 5. Distributed Databases, Sharding & NoSQL (Cards 33–40)

### Card 33: "What does the CAP Theorem state?"
> **Spoken Answer:** In the presence of a **Network Partition ($P$)**, which is unavoidable in distributed physical networks, a system must choose between **Consistency ($C$)** (returning errors or waiting to ensure linearizable reads) or **Availability ($A$)** (returning stale data from surviving partitioned nodes).

---

### Card 34: "What is the PACELC Theorem?"
> **Spoken Answer:** An extension to CAP: If there is a Partition ($P$), trade off Availability ($A$) or Consistency ($C$). **Else ($E$)**, under normal execution with no partition, trade off **Latency ($L$)** or **Consistency ($C$)**.

---

### Card 35: "What is the Dynamo Quorum consistency formula?"
> **Spoken Answer:** For $N$ replicas, write quorum $W$, and read quorum $R$, if **$W + R > N$**, the read quorum and write quorum are guaranteed to overlap on at least one node containing the latest timestamped write, ensuring strong read-after-write consistency.

---

### Card 36: "What is Replication Lag and how do you achieve Read-Your-Own-Writes consistency?"
> **Spoken Answer:** **Replication Lag** is the asynchronous delay for committed leader writes to propagate to read replicas. To guarantee a user sees their own update immediately after submitting a form, route their read requests to the **Primary Leader for a few seconds** before switching back to read replicas.

---

### Card 37: "What is Database Sharding and what is the Cross-Shard Join Penalty?"
> **Spoken Answer:** Sharding splits a table horizontally by rows across independent physical database servers using a **Shard Key**. Joining tables across different physical shards requires fetching datasets over the network into application memory to execute distributed joins, degrading performance from milliseconds to seconds.

---

### Card 38: "Why is Consistent Hashing critical for distributed databases and caches?"
> **Spoken Answer:** In standard modulo hashing (`hash(key) % N`), adding or removing a server changes the hash slot for nearly 100% of keys, causing massive data movement and cache stampedes. Consistent Hashing maps keys and servers to a circular ring, ensuring adding or removing a node migrates **only $K/N$ keys on average**.

---

### Card 39: "When should you choose a Document Store vs. Wide-Column vs. Key-Value vs. Graph DB?"
> **Spoken Answer:**
> - **Document (MongoDB):** Polymorphic JSON payloads and rapid schema evolution.
> - **Key-Value (Redis):** Sub-millisecond caching, rate limiters, and session storage.
> - **Wide-Column (Cassandra):** High-throughput append-only time-series telemetry and chat logs.
> - **Graph (Neo4j):** Highly interconnected networks with complex multi-hop relationship traversals (fraud rings, social graphs).

---

### Card 40: "What is the 4-step playbook for scaling a bottlenecked relational database?"
> **Spoken Answer:**
> 1. **Optimize Queries & Indexes:** Analyze slow query logs, add covering indexes, remove unindexed full table scans.
> 2. **Add Read Replicas:** Separate read and write traffic, routing all analytical/read queries to replicas.
> 3. **Introduce a Caching Layer:** Deploy Redis (Cache-Aside pattern) for hot read keys and session data.
> 4. **Shard Horizontally:** Partition database tables across physical nodes based on a clean Shard Key (`tenant_id`/`user_id`).
