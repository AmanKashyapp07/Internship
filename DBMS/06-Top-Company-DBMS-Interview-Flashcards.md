# Advanced DBMS Concepts & Systems Deep-Dives

> **Scope:** Detailed Theoretical Deep-Dives across Storage Engine Page Layouts, MVCC Mechanics, Indexing Physical Access Paths, Relational Query Processing, Two-Phase Locking & Deadlocks, and Distributed Consensus & Sharding.

---

# Table of Contents
1. [ACID Implementation, Storage Engine Page Layouts & MVCC Internals](#1-acid-implementation-storage-engine-page-layouts--mvcc-internals)
2. [Indexing Data Structures, Physical Access Paths & Query Optimization](#2-indexing-data-structures-physical-access-paths--query-optimization)
3. [Relational Query Processing, Window Functions & Kleene Logic](#3-relational-query-processing-window-functions--kleene-logic)
4. [Normalization Theory, Two-Phase Locking & Deadlock Resolution](#4-normalization-theory-two-phase-locking--deadlock-resolution)
5. [Distributed Consistency Models, Quorum Protocols & Horizontal Sharding](#5-distributed-consistency-models-quorum-protocols--horizontal-sharding)

---

# 1. ACID Implementation, Storage Engine Page Layouts & MVCC Internals

### 1. Atomicity vs. Isolation
**Atomicity** guarantees all-or-nothing execution for an individual transaction, restoring pre-transaction state via Undo logs if execution fails or aborts. **Isolation** governs the visibility of concurrent transactions executing simultaneously, preventing transactions from observing intermediate uncommitted states of others. A system can provide full atomicity while executing under relaxed isolation (e.g. Read Uncommitted).

---

### 2. Write Skew under Snapshot Isolation
**Write Skew** occurs when concurrent transactions read overlapping datasets, validate invariant constraints independently, and subsequently modify disjoint tuples that collectively violate a global multi-tuple constraint (e.g. withdrawing from checking and savings balances simultaneously such that the total sum drops below zero). Because Snapshot Isolation only detects concurrent write-write conflicts on identical rows, Write Skew is permitted. Mitigation requires Serializable isolation or explicit row locks (`SELECT ... FOR UPDATE`).

---

### 3. Non-Blocking Reads under MVCC
Multi-Version Concurrency Control maintains timestamped tuple versions on update rather than modifying data in-place under locks. Readers read consistent historical snapshots corresponding to their transaction start watermark without acquiring shared read locks. Consequently, **readers never block writers, and writers never block readers**.

---

### 4. Write-Ahead Logging (WAL) Invariant
A dirty data page in the memory Buffer Pool can **never be written to persistent non-volatile storage until the corresponding Redo Log (WAL) record describing the modification has been flushed to disk (`fsync`)**. This ensures the system can replay committed modifications during ARIES crash recovery.

---

### 5. PostgreSQL Transaction ID (XID) Wraparound
PostgreSQL utilizes 32-bit transaction identifiers (`XID`), which wrap around after $2^{32} \approx 4.29$ billion transactions. Left unchecked, historical transactions would appear to exist in the future, rendering tuples invisible. PostgreSQL's background `VACUUM` process prevents this by freezing old tuple versions (setting a special `FrozenXID` bit in the tuple header), marking them as permanently committed in the past.

---

### 6. Serializable Snapshot Isolation (SSI) & Dependency Graphs
PostgreSQL SSI provides serializable isolation without traditional two-phase locking by tracking non-blocking `SIREAD` lock tags in memory and maintaining a dependency graph of read-write anti-dependencies ($rw$-antidependencies). If the engine detects a cycle of two consecutive $rw$-antidependency edges ($T_1 \xrightarrow{rw} T_2 \xrightarrow{rw} T_3$), it aborts one participating transaction with a serialization failure.

---

### 7. Slotted Page Storage Architecture
Disk pages are organized into a header containing an array of slot pointers at the top of the page, with row byte payloads growing upward from the bottom. Secondary indexes reference tuple identifiers `(PageID, SlotNumber)` rather than direct byte offsets, allowing tuples to be defragmented or compacted within a page without updating external secondary index pointers.

---

### 8. Write-Ahead Logging (Redo) vs. Undo Logging
**Redo Logs (WAL)** record newly modified states to replay committed transactions whose dirty buffer pool pages were not yet persisted before a crash (**Durability**). **Undo Logs** record inverse operations to roll back in-flight, uncommitted modifications when a transaction aborts or fails (**Atomicity**).

---

# 2. Indexing Data Structures, Physical Access Paths & Query Optimization

### 9. B+ Tree Physical Layout Advantages
Binary Search Trees allocate small nodes (~24 bytes) that waste disk page space and produce deep tree hierarchies ($O(\log_2 N)$ seeks). B+ Trees store only search keys and child page pointers in internal nodes, yielding massive fan-out ($M > 100$) and shallow tree heights ($\le 3$ for millions of tuples). Leaf pages store all data records and are doubly linked for $O(1)$ sequential range scans.

---

### 10. Secondary Index Clustered Traversal
In clustered engines (e.g. MySQL InnoDB), secondary indexes store the Primary Key value as their tuple reference. Accessing non-indexed attributes requires traversing the secondary index tree to find the Primary Key, followed by a secondary traversal of the Clustered Index tree. This is bypassed using **Covering Indexes** that include all queried attributes in the secondary index leaf node (**Index-Only Scan**).

---

### 11. Leftmost Prefix Rule for Composite Indexes
A composite index on attributes `(A, B, C)` sorts tuples lexicographically by $A$, then $B$, then $C$. The query engine can only execute binary search index seeks if predicates specify attributes sequentially starting with the leftmost column $A$. Predicates on `(B, C)` omit the leading sorting key and force full relation scans.

---

### 12. Log-Structured Merge-Trees (LSM-Trees) vs. B+ Trees
**LSM-Trees (RocksDB, Cassandra)** optimize for **write-heavy workloads** by buffering incoming mutations sequentially in an in-memory MemTable and flushing sorted immutable runs (SSTables) to disk, eliminating random disk I/O. **B+ Trees** optimize for **read-heavy transactional OLTP workloads** requiring fast point lookups and range scans.

---

### 13. Block Range Indexing (BRIN)
For relations with naturally sorted or time-series data, a Block Range Index (BRIN) stores only the minimum and maximum attribute values for physical ranges of disk pages (e.g. 128 pages per block range) rather than indexing individual tuples, consuming orders of magnitude less memory and storage than B-Trees.

---

### 14. Non-Sequential Key Fragmentation in B+ Trees
Inserting uniformly distributed random keys (e.g. UUIDv4) forces random writes across leaf pages, triggering frequent **leaf page splits**, reducing leaf page fill factors to ~50%, and causing excessive random I/O. Timestamp-ordered keys (UUIDv7, BIGINT) ensure sequential append-only page inserts.

---

### 15. Index Condition Pushdown (ICP)
ICP is an access path optimization where the storage engine evaluates applicable `WHERE` filter predicates directly during index leaf traversal, discarding non-matching index entries before fetching complete heap/clustered table rows into server memory.

---

### 16. Leading Wildcard Predicates & Index Scans
B+ Tree string indexes order characters lexicographically from left to right. A filter with a leading wildcard (`LIKE '%suffix'`) prevents binary search prefix seeks, forcing a **full relation scan**. Resolving suffix lookups requires reverse indexing (`REVERSE(col)`) or Trigram/GIN indexing.

---

# 3. Relational Query Processing, Window Functions & Kleene Logic

### 17. SQL Logical Evaluation Sequence
1. `FROM` & `JOIN`: Construct and join Cartesian source datasets.
2. `WHERE`: Filters base tuples.
3. `GROUP BY`: Groups tuples by distinct attribute sets.
4. `HAVING`: Filters aggregated groups.
5. `SELECT`: Evaluates projections, expressions, and window functions.
6. `DISTINCT`: Deduplicates output tuples.
7. `ORDER BY`: Sorts the result set.
8. `LIMIT` / `OFFSET`: Truncates output window.

---

### 18. Window Ranking Functions (`ROW_NUMBER`, `RANK`, `DENSE_RANK`)
For tied attribute values `[100, 100, 80]`:
- `ROW_NUMBER()`: Assigns arbitrary sequential integers: `1, 2, 3`.
- `RANK()`: Assigns identical ranks with gaps: `1, 1, 3` (rank 2 is skipped).
- `DENSE_RANK()`: Assigns identical ranks without gaps: `1, 1, 2`.

---

### 19. Three-Valued Logic in `NOT IN` Subqueries
Under SQL three-valued logic (`TRUE`, `FALSE`, `UNKNOWN`), `x NOT IN (1, 2, NULL)` evaluates to `x != 1 AND x != 2 AND x != NULL`. Because equality with `NULL` yields `UNKNOWN`, the entire logical conjunction evaluates to `UNKNOWN` or `FALSE`, causing the query to return zero rows. Safe set differences require `NOT EXISTS` or `LEFT JOIN ... WHERE ... IS NULL`.

---

### 20. WHERE vs. HAVING Filtering Phases
`WHERE` predicates filter candidate tuples *prior* to group formation and cannot evaluate aggregate functions. `HAVING` predicates filter summary records *after* group aggregation and evaluate directly on aggregate expressions.

---

### 21. N-th Highest Ranking Formulation
Calculated deterministically using window CTEs:
```sql
WITH Ranked AS (
    SELECT salary, DENSE_RANK() OVER (ORDER BY salary DESC) AS rnk FROM Employees
)
SELECT salary FROM Ranked WHERE rnk = N LIMIT 1;
```

---

### 22. Island Grouping via Date Arithmetic
Consecutive temporal events are identified by computing `(event_date - ROW_NUMBER() * 1 day)`. For continuous consecutive dates, this subtraction produces an invariant anchor date, enabling streak aggregation via `GROUP BY`.

---

### 23. `UNION` vs. `UNION ALL` Execution Mechanics
`UNION` performs set union with duplicate elimination via memory/disk sorting or hash distinct operations. `UNION ALL` performs direct relational concatenation without deduplication overhead.

---

### 24. Partial Indexes for Filtered Domains
Partial indexes (`CREATE INDEX idx_active ON Users(email) WHERE deleted_at IS NULL`) index exclusively the subset of tuples matching a predicate, reducing index footprint and memory buffer churn for skewed access patterns.

---

# 4. Normalization Theory, Two-Phase Locking & Deadlock Resolution

### 25. The Normalization Progression
- **1NF:** Atomic scalar attribute domains without repeating groups.
- **2NF:** 1NF with zero partial functional dependencies on composite candidate keys.
- **3NF:** 2NF with zero transitive functional dependencies between non-prime attributes.
- **BCNF:** 3NF where every determinant $X$ in non-trivial dependency $X \to Y$ is a super key.

---

### 26. Optimistic vs. Pessimistic Locking
**Pessimistic Locking (`SELECT ... FOR UPDATE`)** acquires exclusive row locks upfront, blocking concurrent access until transaction termination. **Optimistic Locking** avoids database-level locks, verifying an attribute version (`WHERE version = old_version`) at update time and aborting on concurrent modification.

---

### 27. Concurrency Anomaly Definitions
- **Dirty Read ($G_1$):** Reading uncommitted data from a concurrent transaction that subsequently rolls back.
- **Non-Repeatable Read ($G_{2a}$):** Re-reading a row and observing committed column modifications made by another transaction.
- **Phantom Read ($A_3$):** Re-executing a range query and observing newly inserted rows committed by another transaction.

---

### 28. Next-Key Locking in MySQL InnoDB
InnoDB prevents Phantom Reads under Repeatable Read by combining record locks with **Gap Locks** on the open intervals before, between, and after index records, blocking concurrent inserts into the scanned index range.

---

### 29. Two-Phase Locking (2PL) Protocol
2PL divides lock management into a **Growing Phase** (locks are acquired, none released) and a **Shrinking Phase** (locks are released, none acquired). Strict 2PL holds all exclusive locks until transaction commit/abort, guaranteeing conflict serializability. 2PL does not prevent deadlocks.

---

### 30. Deadlock Detection via Wait-For Graphs
The database engine maintains an in-memory directed **Wait-For Graph (WFG)** where nodes represent transactions and edges represent lock wait dependencies. Periodic cycle detection (DFS / Tarjan's algorithm) identifies deadlocks, aborting the minimum-cost victim transaction to break the cycle.

---

### 31. Lock Escalation on Unindexed Foreign Keys
Deleting or updating parent relation rows forces the engine to verify referential integrity on child relations. If the child foreign key column is unindexed, the engine must perform full child relation scans or acquire table-level locks, causing concurrency contention.

---

### 32. Denormalization Performance Rationale
Denormalization intentionally introduces redundant attributes to eliminate multi-table join overhead in high-throughput read workloads, accepting write amplification and requiring application-level consistency controls.

---

# 5. Distributed Consistency Models, Quorum Protocols & Horizontal Sharding

### 33. CAP Theorem Formulation
During an inevitable network partition ($P$), a distributed data store must choose between **Consistency ($C$)** (returning linearizable data or an error) versus **Availability ($A$)** (returning non-error responses from accessible partition nodes).

---

### 34. PACELC Theorem
If partitioned ($P$), trade off Availability ($A$) vs. Consistency ($C$). **Else ($E$)**, under normal non-partitioned operation, trade off **Latency ($L$)** vs. **Consistency ($C$)**.

---

### 35. Dynamo Quorum Consensus Equation
For $N$ total replicas, write quorum size $W$, and read quorum size $R$:

$$W + R > N \implies \text{Overlapping Quorum with Guaranteed Linearizable Read}$$

---

### 36. Read-Your-Own-Writes Consistency
In asynchronous single-leader replication, replication lag can cause clients to observe stale state immediately following a write. Systems achieve Read-Your-Own-Writes consistency by pinning a user's read requests to the primary leader for a bounded temporal duration following an update.

---

### 37. Horizontal Sharding & Cross-Shard Joins
Sharding partitions relation rows across independent physical nodes using a Shard Key. Cross-shard joins require distributed scatter-gather queries and coordinator-node hash joins over the network.

---

### 38. Consistent Hashing Topology
Maps keys and database nodes to a circular 360-degree hash ring. Adding or removing nodes migrates only $K/N$ keys on average, avoiding cluster-wide data re-shuffling.

---

### 39. Non-Relational Storage Model Selection
- **Document (MongoDB):** Polymorphic entities, nested hierarchical schemas, single-document ACID transactions.
- **Key-Value (Redis):** Low-latency caching, ephemeral sessions, in-memory state primitives.
- **Wide-Column (Cassandra):** High-throughput append workloads, time-series metrics, multi-datacenter replication.
- **Graph (Neo4j):** Index-free adjacency graph traversals across highly interconnected entities.

---

### 40. Multi-Tier Scaling Architecture Progression
1. **Query & Index Tuning:** Profiling slow query logs, adding covering indexes, and eliminating unindexed table scans.
2. **Read Replication:** Routing read workloads across asynchronous read replicas, reserving the primary leader for writes.
3. **In-Memory Caching:** Inserting cache-aside / write-through Redis caching layers for high-frequency read paths.
4. **Horizontal Sharding:** Partitioning data across physical nodes via a balanced Shard Key.
