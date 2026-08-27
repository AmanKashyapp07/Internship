# Master Guide 05: Top-Company DBMS Spoken Flashcards & Trap Questions

> **Focus:** Rapid-fire spoken flashcards, verbal script answers, tricky trap questions, and high-ROI technical answers for FAANG, FinTech (Stripe, Plaid), and High-Frequency Trading interviews.
> 
> *Designed to crush top-tier database interviews with zero fluff.*

---

# Table of Contents
1. [ACID & Concurrency Control Spoken Flashcards](#1-acid--concurrency-control-spoken-flashcards)
2. [Indexing & Storage Engine Spoken Flashcards](#2-indexing--storage-engine-spoken-flashcards)
3. [SQL Execution & Normalization Spoken Flashcards](#3-sql-execution--normalization-spoken-flashcards)
4. [Distributed Systems & Replication Spoken Flashcards](#4-distributed-systems--replication-spoken-flashcards)
5. [Caching & System Design Spoken Flashcards](#5-caching--system-design-spoken-flashcards)

---

# 1. ACID & Concurrency Control Spoken Flashcards

> 🎴 **Card 1: "What is the difference between Atomicity and Isolation?"**  
> **Spoken Answer:** Atomicity guarantees that all operations within a single transaction complete or none do (rollback via Undo logs). Isolation guarantees that concurrent executing transactions do not observe each other's intermediate state. A system can have 100% Atomicity with 0% Isolation (e.g. Read Uncommitted execution).

> 🎴 **Card 2: "What is Write Skew, and why doesn't Snapshot Isolation prevent it?"**  
> **Spoken Answer:** Write Skew occurs under Snapshot Isolation when two concurrent transactions read overlapping data states, validate constraints independently, and update disjoint rows that together violate a global invariant (e.g. withdrawing from checking and savings simultaneously until total balance $< 0$). Snapshot Isolation only detects concurrent writes to the *exact same row*, so it fails to catch Write Skew. Fix: Use Serializable isolation or explicit row locks (`SELECT ... FOR UPDATE`).

> 🎴 **Card 3: "How does MVCC eliminate read-write locking contention?"**  
> **Spoken Answer:** Instead of updating rows in-place under locks, MVCC creates new timestamped versions of rows on update. Readers read a consistent historical snapshot corresponding to their transaction start time without acquiring locks. Therefore, **readers never block writers, and writers never block readers**.

> 🎴 **Card 4: "What is the Golden Rule of Write-Ahead Logging (WAL)?"**  
> **Spoken Answer:** A dirty data page in the RAM Buffer Pool can **never be flushed to disk** until the corresponding WAL log record describing the change has been flushed to disk (`fsync`). This ensures the database can always perform Redo/Undo recovery after a power crash.

> 🎴 **Card 5: "What is XID Wraparound in PostgreSQL?"**  
> **Spoken Answer:** Postgres 32-bit transaction IDs wrap around after 4.2 billion transactions. Autovacuum must freeze old tuples before wraparound occurs; otherwise past historical data would appear in the future and become invisible.

> 🎴 **Card 6: "How does Postgres SSI (Serializable Snapshot Isolation) achieve serializability without 2PL locks?"**  
> **Spoken Answer:** SSI tracks non-blocking SIREAD locks in RAM and constructs a dependency graph of $rw$-antidependencies. If the engine detects two consecutive $rw$-antidependency edges ($T_1 \xrightarrow{rw} T_2 \xrightarrow{rw} T_3$), it aborts one transaction with a serialization failure, eliminating Write Skew without locking.

---

# 2. Indexing & Storage Engine Spoken Flashcards

> 🎴 **Card 7: "Why do databases use B+ Trees instead of Binary Search Trees (BST) or standard B-Trees?"**  
> **Spoken Answer:** BSTs have small nodes (~24 bytes) that waste 99.7% of an 8KB disk page and yield tall tree heights (~30 disk seeks). B+ Trees store *only routing keys* in internal nodes, yielding massive fan-out ($M > 100$) and keeping height $\le 3$ for 1 billion rows. Additionally, B+ Tree leaves store all data payloads and are doubly linked together for $O(1)$ sequential range scans (`BETWEEN A AND B`).

> 🎴 **Card 8: "What is the InnoDB Double Lookup Penalty and how do you eliminate it?"**  
> **Spoken Answer:** In MySQL InnoDB, secondary indexes store the Primary Key as their pointer payload. Querying via a secondary index performs a first B+ Tree seek to find the Primary Key, followed by a second B+ Tree seek on the Clustered Index to retrieve full row data. This double lookup is eliminated using a **Covering Index** that includes all requested columns in the secondary index leaf node.

> 🎴 **Card 9: "What is the Leftmost Prefix Rule in Composite Indexes?"**  
> **Spoken Answer:** A composite index on `(A, B, C)` sorts data lexicographically starting from `A`. Queries can only utilize the index if their filters match columns sequentially starting from the leftmost column `A` with no gaps. If column `A` is missing in the query, the index cannot be used for binary search seeks.

> 🎴 **Card 10: "When should you prefer an LSM-Tree over a B+ Tree?"**  
> **Spoken Answer:** Prefer LSM-Trees (RocksDB, Cassandra) for **write-heavy workloads**. LSM-Trees convert random writes into sequential in-memory appends (MemTable) that flush sequentially to disk (SSTables), minimizing SSD write amplification. Prefer B+ Trees (PostgreSQL, InnoDB) for **read-heavy transactional OLTP workloads**.

> 🎴 **Card 11: "Why is a BRIN index ideal for time-series data in PostgreSQL?"**  
> **Spoken Answer:** Time-series data is naturally inserted in chronological order. A BRIN index stores only the min/max timestamps for blocks of 128 disk pages, taking up kilobytes instead of gigabytes of RAM.

> 🎴 **Card 12: "How does the Slotted Page architecture allow row updates without invalidating secondary indexes?"**  
> **Spoken Answer:** Secondary indexes store pointers to `(Page, Slot)`. When rows inside a page are moved or compacted, only the internal page slot offset is updated; the external `(Page, Slot)` address remains identical.

---

# 3. SQL Execution & Normalization Spoken Flashcards

> 🎴 **Card 13: "Explain SQL Logical Execution Order in 15 seconds."**  
> **Spoken Answer:** SQL engines execute clauses in 8 steps: `FROM & JOIN` $\to$ `WHERE` $\to$ `GROUP BY` $\to$ `HAVING` $\to$ `SELECT` $\to$ `DISTINCT` $\to$ `ORDER BY` $\to$ `LIMIT`. `WHERE` executes before `SELECT`, which is why column aliases defined in `SELECT` cannot be filtered inside `WHERE`.

> 🎴 **Card 14: "What is the difference between `ROW_NUMBER()`, `RANK()`, and `DENSE_RANK()`?"**  
> **Spoken Answer:** On duplicate values: `ROW_NUMBER()` assigns unique sequential integers (1, 2, 3, 4); `RANK()` assigns tied ranks and skips subsequent rank numbers (1, 2, 2, 4); `DENSE_RANK()` assigns tied ranks with zero gaps (1, 2, 2, 3). For $N$-th highest queries, always use `DENSE_RANK()`.

> 🎴 **Card 15: "Why is UUIDv4 terrible for B+ Tree primary keys, and why is UUIDv7 preferred?"**  
> **Spoken Answer:** UUIDv4 is completely random. Inserting random keys into a B+ Tree forces random leaf page reads and triggers frequent 50% Page Splits, destroying memory cache locality. UUIDv7 prefixes a 48-bit timestamp, ensuring time-ordered sequential B+ Tree appends with zero page splits.

> 🎴 **Card 16: "Why should you use Partial Indexes for Soft-Deleted tables?"**  
> **Spoken Answer:** Soft-deleted tables store 99% active rows (`is_deleted = FALSE`). A standard index includes soft-deleted rows, wasting RAM. A partial index `WHERE is_deleted = FALSE` indexes only live rows, keeping index size tiny and blazing fast.

---

# 4. Distributed Systems & Replication Spoken Flashcards

> 🎴 **Card 17: "Explain the Quorum Formula $W + R > N$."**  
> **Spoken Answer:** In leaderless replication (Cassandra), $N$ is total replicas, $W$ is write confirmation count, and $R$ is read query count. When $W + R > N$, at least one node in the read set overlaps with the write set, guaranteeing the client reads the latest write.

> 🎴 **Card 18: "How does Google Spanner achieve global serializability using TrueTime?"**  
> **Spoken Answer:** TrueTime uses GPS and atomic clocks to bound time uncertainty $\epsilon$. Spanner enforces the **Commit Wait Rule**: delaying transaction commit until $2\epsilon$ time elapses, guaranteeing causally later transactions receive strictly higher timestamps worldwide.

> 🎴 **Card 19: "What is the difference between Local and Global Secondary Indexes in sharded databases?"**  
> **Spoken Answer:** Local Secondary Indexes are partitioned by the main shard key (fast $O(1)$ writes, but reads require Scatter-Gather queries across all shards). Global Secondary Indexes are partitioned by the secondary key (fast $O(1)$ reads, but writes require distributed transactions across nodes).

---

# 5. Caching & System Design Spoken Flashcards

> 🎴 **Card 20: "What is Cache Penetration vs. Cache Stampede?"**  
> **Spoken Answer:** Cache Penetration is non-existent keys (e.g. `id = -999`) missing cache and flooding the DB. Fix with Bloom Filters or caching nulls. Cache Stampede (Thundering Herd) is a popular existing key expiring, causing 10,000 concurrent threads to hit the DB at once. Fix with a Distributed Lock (`SETNX`).

> 🎴 **Card 21: "How does Redis store Sorted Sets (`ZSET`) internally?"**  
> **Spoken Answer:** Redis uses a **SkipList + Hash Map**. The Hash Map maps member $\to$ score in $O(1)$ time. The SkipList maintains scores in sorted order, supporting $O(\log N)$ rank queries and range fetches (`ZRANGEBYSCORE`).

> 🎴 **Card 22: "Why is Linux Zero-Copy `sendfile()` critical for Kafka's performance?"**  
> **Spoken Answer:** Standard I/O copies data from OS Page Cache $\to$ JVM Memory $\to$ Socket Buffer $\to$ Network, incurring 4 context switches and 3 copies. Zero-copy `sendfile()` transfers bytes directly from OS Page Cache to the Socket Buffer inside kernel space, avoiding JVM overhead.
