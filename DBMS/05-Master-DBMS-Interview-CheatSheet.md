# Master Guide 05: Master DBMS Interview Cheat Sheet

> **Focus:** The Unified DBMS Matrix, Formula & Syntax Quick-Reference, 50 High-Yield Rapid-Fire Verbal Questions with Bold Answers, and the Top 10 Red Flag DBMS Junior Mistakes.
> 
> *The 10-Minute Pre-Interview Revision Document for FAANG/Tier-1 Tech Interviews.*

---

# Table of Contents
1. [The Unified DBMS System Design Matrix](#1-the-unified-dbms-system-design-matrix)
2. [Formula & Syntax Quick-Reference Card](#2-formula--syntax-quick-reference-card)
3. [50 Rapid-Fire DBMS Questions & Bold Answers](#3-50-rapid-fire-dbms-questions--bold-answers)
4. [Top 10 DBMS Red Flag Mistakes That Sound Junior](#4-top-10-dbms-red-flag-mistakes-that-sound-junior)

---

# 1. The Unified DBMS System Design Matrix

```
+--------------------------------------------------------------------------------------------------------------------+
| CORE DATABASE TOPIC  | KEY DATABASE PRIMITIVE           | WHERE IT REAPPEARS IN LARGE-SCALE SYSTEM DESIGN   |
+--------------------------------------------------------------------------------------------------------------------+
| Schema Normalization | 3NF / BCNF Decomposition         | Microservice domain boundaries, Write-heavy OLTP   |
+--------------------------------------------------------------------------------------------------------------------+
| Denormalization      | Redundant embedded columns       | Read-heavy dashboards, Wide-Column NoSQL, Caching  |
+--------------------------------------------------------------------------------------------------------------------+
| B+ Tree Indexing     | Clustered Index, Covering Index  | Sub-millisecond latency tuning, eliminating scans  |
+--------------------------------------------------------------------------------------------------------------------+
| Write-Ahead Log (WAL)| Append-Only Redo Log `fsync`     | Kafka event streaming, Change Data Capture (CDC)   |
+--------------------------------------------------------------------------------------------------------------------+
| MVCC & Isolation     | `xmin`/`xmax`, Snapshot Isolation| Preventing Double-Booking, Optimistic Lock Version |
+--------------------------------------------------------------------------------------------------------------------+
| Read Replicas        | Single-Leader Async Replication  | Read/Write splitting, CQRS, Caching architecture   |
+--------------------------------------------------------------------------------------------------------------------+
| Sharding             | Consistent Hashing Shard Keys    | Multi-tenant scaling, avoiding Cross-Shard joins   |
+--------------------------------------------------------------------------------------------------------------------+
```

---

# 2. Formula & Syntax Quick-Reference Card

```
+---------------------------------------------------------------------------------------------------+
| 1. SQL LOGICAL EXECUTION ORDER                                                                    |
|    FROM -> JOIN -> WHERE -> GROUP BY -> HAVING -> SELECT -> DISTINCT -> ORDER BY -> LIMIT/OFFSET  |
+---------------------------------------------------------------------------------------------------+
| 2. ISOLATION LEVELS VS. ANOMALIES                                                                 |
|    * Read Uncommitted: Allows Dirty Reads, Non-Repeatable Reads, Phantom Reads, Write Skew         |
|    * Read Committed:   NO Dirty Reads; Allows Non-Repeatable Reads, Phantoms, Write Skew (Postgres)|
|    * Repeatable Read:  NO Dirty/Non-Repeatable Reads; NO Phantoms in InnoDB (Next-Key); Allows Skew|
|    * Serializable:     NO Anomalies (Strict Serial Schedule)                                       |
+---------------------------------------------------------------------------------------------------+
| 3. DISTRIBUTED QUORUM CONSISTENCY FORMULA                                                         |
|    W + R > N (Where N = Replicas, W = Write Quorum, R = Read Quorum) -> Strong Consistency        |
+---------------------------------------------------------------------------------------------------+
| 4. NORMALIZATION CHECKLIST                                                                        |
|    * 1NF: Atomic values only (No repeating groups / arrays)                                       |
|    * 2NF: 1NF + No Partial Dependencies on Composite Primary Keys                                 |
|    * 3NF: 2NF + No Transitive Dependencies between non-key columns                                |
|    * BCNF: 3NF + Every determinant X in X -> Y MUST be a Super Key                                |
+---------------------------------------------------------------------------------------------------+
```

---

# 3. 50 Rapid-Fire DBMS Questions & Bold Answers

### Topic 1: Schema Design & Keys
1. **What is the difference between a Candidate Key and a Primary Key?**
   - **A Candidate Key is any minimal super key that uniquely identifies a row; the Primary Key is the single candidate key chosen to enforce uniqueness and build the clustered index.**
2. **Why is random UUIDv4 terrible for B+ Tree Primary Keys?**
   - **Random inserts cause frequent B+ Tree page splits, fragmentation, and reduce leaf page fill factor to ~50%; use time-ordered UUIDv7 or Auto-Increment instead.**
3. **What is the difference between 3NF and BCNF?**
   - **3NF allows functional dependency $X \to Y$ if $Y$ is a prime attribute; BCNF strictly requires $X$ to be a Super Key for every dependency.**
4. **When should you denormalize a database table?**
   - **For read-heavy workloads (OLAP/Dashboards) where joining normalized tables introduces unacceptable query latency.**
5. **How do you model a Many-to-Many relationship in SQL?**
   - **By using a Junction/Bridge table containing Foreign Keys referencing both parent tables with a composite primary key.**
6. **What is a Surrogate Key?**
   - **An artificial system-generated unique identifier with no real-world business meaning (e.g. `BIGINT AUTO_INCREMENT`).**
7. **What is Referential Integrity?**
   - **A relational constraint ensuring that a Foreign Key value in a child table must always match an existing Primary Key in the parent table.**
8. **What is a Weak Entity in ER modeling?**
   - **An entity that cannot be uniquely identified by its own attributes alone and depends on an identifying relationship with an owner entity.**

---

### Topic 2: SQL Query Execution & Window Functions
9. **Why can't you use a column alias defined in SELECT inside the WHERE clause?**
   - **Because the SQL engine executes the `WHERE` clause before evaluating the `SELECT` projection clause.**
10. **What is the difference between `WHERE` and `HAVING`?**
    - **`WHERE` filters individual rows before grouping; `HAVING` filters aggregated groups after the `GROUP BY` clause.**
11. **What is the difference between `RANK()` and `DENSE_RANK()`?**
    - **`RANK()` skips rank numbers on ties (e.g. 1, 1, 3); `DENSE_RANK()` assigns continuous sequential ranks without skipping (e.g. 1, 1, 2).**
12. **Why does `x NOT IN (1, 2, NULL)` return 0 rows?**
    - **Because comparing anything to `NULL` yields `UNKNOWN`, causing the entire `AND` condition to evaluate to `UNKNOWN` and filter out all candidate rows.**
13. **What is the difference between `UNION` and `UNION ALL`?**
    - **`UNION` executes an expensive deduplication sort operation; `UNION ALL` concatenates datasets directly without deduplication.**
14. **What is an Anti-Join and how is it best implemented?**
    - **A query returning rows in Table A with no match in Table B, best implemented using `LEFT JOIN ... WHERE B.id IS NULL` or `NOT EXISTS`.**
15. **What is a Self-Join?**
    - **Joining a table to itself, commonly used to query hierarchical structures such as Employee-Manager relationships.**
16. **What is the purpose of `LEAD()` and `LAG()` window functions?**
    - **To access data from subsequent (`LEAD`) or preceding (`LAG`) rows without performing an explicit self-join.**

---

### Topic 3: Indexing & Storage Engine Internals
17. **Why do databases use B+ Trees instead of Binary Search Trees (BST)?**
    - **B+ Trees match disk page sizes, have massive fan-out ($>100$), keep tree height $\le 3$, and link leaf nodes for sequential $O(1)$ range scans.**
18. **What is a Clustered Index?**
    - **An index where the physical order of rows on disk matches the index order (the table leaf pages store the actual row data).**
19. **What is a Secondary Index in MySQL InnoDB?**
    - **An index whose leaf nodes store the indexed key and the Primary Key pointer, requiring a secondary double lookup to fetch full row data.**
20. **What is a Covering Index?**
    - **An index containing all columns requested by a query, allowing the database to satisfy the query entirely from the index leaf without touching table pages.**
21. **What is the Leftmost Prefix Rule in Composite Indexes `(A, B, C)`?**
    - **Queries must filter columns sequentially from left to right starting with `A`; a query on `(B, C)` cannot utilize the index for tree seeks.**
22. **What is the write penalty of adding multiple indexes?**
    - **Every `INSERT`, `UPDATE`, and `DELETE` must synchronously update all corresponding B+ Tree index structures on disk, increasing write latency.**
23. **Why does `LIKE '%Alice'` cause a full table scan?**
    - **Because leading wildcards prevent the B+ Tree from performing binary search prefix seeks on the indexed string.**
24. **What is Index Condition Pushdown (ICP)?**
    - **Evaluating `WHERE` filter clauses directly inside the storage engine index scan loop to avoid fetching non-matching rows into server memory.**

---

### Topic 4: Transactions, ACID & MVCC
25. **What does Atomicity mean in ACID?**
    - **All operations in a transaction complete or none do, rolling back partial changes via Undo Logs.**
26. **What is Write-Ahead Logging (WAL)?**
    - **A durability invariant requiring that Redo Log records be flushed to disk (`fsync`) before corresponding dirty buffer pages can be written to disk.**
27. **What is a Dirty Read?**
    - **Reading uncommitted modifications made by another concurrent transaction that is later rolled back.**
28. **What is a Non-Repeatable Read?**
    - **A transaction reads a row, another transaction updates and commits that row, and the first transaction re-reads the row seeing altered data.**
29. **What is a Phantom Read?**
    - **A transaction queries a range, another transaction inserts a new row into that range and commits, and re-executing the range query returns new rows.**
30. **What is Write Skew under Snapshot Isolation?**
    - **Concurrent transactions read overlapping state, check constraints, and update disjoint rows that together violate a global invariant.**
31. **What is Multi-Version Concurrency Control (MVCC)?**
    - **A concurrency engine that maintains timestamped row versions so readers read historical snapshots without acquiring locks (readers never block writers).**
32. **How does PostgreSQL implement MVCC at the tuple level?**
    - **By storing `xmin` (creating transaction ID) and `xmax` (deleting transaction ID) in every physical row header.**
33. **What is the purpose of PostgreSQL `VACUUM`?**
    - **To reclaim storage occupied by dead tuple versions (`xmax < oldest_active_xid`) and prevent XID wraparound.**
34. **What is Two-Phase Locking (2PL)?**
    - **A pessimistic locking protocol with a Growing Phase (acquiring locks) and a Shrinking Phase (releasing locks), guaranteeing serializability.**
35. **How does MySQL InnoDB eliminate Phantom Reads in Repeatable Read?**
    - **Using Next-Key Locking, which locks both index records and the gaps between index records to prevent phantom inserts.**
36. **What is the difference between Optimistic and Pessimistic Locking?**
    - **Pessimistic locks rows upfront (`SELECT ... FOR UPDATE`); Optimistic checks a `version` column on update and retries on conflict.**

---

### Topic 5: Database Scaling & Distributed Systems
37. **What does the CAP Theorem state?**
    - **In the presence of a Network Partition ($P$), a distributed system must choose between Consistency ($C$) or Availability ($A$).**
38. **What is the difference between CP and AP systems?**
    - **CP systems (Banking/Spanner) reject writes during partitions to preserve correctness; AP systems (Cassandra/DNS) accept writes with eventual consistency.**
39. **What does BASE stand for in NoSQL?**
    - **Basically Available, Soft state, Eventual consistency.**
40. **What is the Dynamo Quorum consistency formula?**
    - **$W + R > N$, guaranteeing that the write quorum ($W$) and read quorum ($R$) overlap on at least one node with the latest data.**
41. **What is Replication Lag and how do you handle it?**
    - **The delay in async replication; solve Read-Your-Own-Writes by routing a user's reads to the Primary Leader for a few seconds after they write.**
42. **What is Database Sharding?**
    - **Horizontal partitioning that distributes rows across multiple independent physical database instances based on a Shard Key.**
43. **What is the Cross-Shard Join problem?**
    - **Joining data located on different physical shards requires network transfer and in-memory application processing, causing severe latency.**
44. **What is Consistent Hashing and why is it used?**
    - **A ring hashing topology where adding or removing a shard node moves only $K/N$ keys, avoiding full cluster rebalancing.**
45. **What is a Document Database (MongoDB) best used for?**
    - **Semi-structured hierarchical payloads, rapid schema evolution, and single-document transactional access.**
46. **What is a Wide-Column Store (Cassandra) best used for?**
    - **Massive write throughput on time-series telemetry, chat histories, and append-heavy workloads.**
47. **What is a Graph Database (Neo4j) best used for?**
    - **Highly interconnected networks (social graphs, fraud detection) requiring multi-hop relationship traversals without expensive SQL joins.**
48. **What is Change Data Capture (CDC)?**
    - **Streaming committed database mutations directly from the Write-Ahead Log (WAL) to downstream search indexes and caches.**
49. **What is the 4-step database scaling playbook?**
    - **1. Query/Index tuning $\to$ 2. Read Replicas $\to$ 3. Redis Caching Layer $\to$ 4. Horizontal Sharding.**
50. **What is the Split-Brain problem and how is it prevented?**
    - **Two nodes believing they are both Primary leaders during a network split; prevented using majority quorum consensus (Raft/Paxos).**

---

# 4. Top 10 DBMS Red Flag Mistakes That Sound Junior

```
+---------------------------------------------------------------------------------------------------+
| #  | THE JUNIOR DBMS MISTAKE                | THE SENIOR CORRECTION / INTERVIEW PIVOT             |
+---------------------------------------------------------------------------------------------------+
| 1  | "Using NOT IN with subqueries that     | NOT IN fails completely if the subquery contains    |
|    | return NULL values."                   | NULLs; always use NOT EXISTS or LEFT JOIN IS NULL.  |
+---------------------------------------------------------------------------------------------------+
| 2  | "Claiming NoSQL is faster than SQL."   | SQL primary key lookups run in sub-milliseconds;    |
|    |                                        | NoSQL trades relational ACID joins for scale.       |
+---------------------------------------------------------------------------------------------------+
| 3  | "Index every column to make it fast."  | Every index adds write amplification on disk and    |
|    |                                        | slows down INSERT, UPDATE, and DELETE operations.   |
+---------------------------------------------------------------------------------------------------+
| 4  | "Using UUIDv4 as Primary Keys in       | Random UUIDv4 causes B+ Tree leaf page splits and   |
|    | InnoDB."                               | 50% fragmentation; use time-ordered UUIDv7.         |
+---------------------------------------------------------------------------------------------------+
| 5  | "Confusing WHERE and HAVING."          | WHERE filters rows before aggregation; HAVING       |
|    |                                        | filters groups after aggregation.                   |
+---------------------------------------------------------------------------------------------------+
| 6  | "Denormalizing without write-cost      | Denormalization speeds up reads but causes massive  |
|    | justification."                        | update anomalies and write amplification.           |
+---------------------------------------------------------------------------------------------------+
| 7  | "Assuming Repeatable Read eliminates   | Standard SQL Repeatable Read allows Phantoms;       |
|    | Phantoms in all DBs."                  | InnoDB eliminates them only via Next-Key Locking.   |
+---------------------------------------------------------------------------------------------------+
| 8  | "Believing MVCC requires no cleanup."  | MVCC creates dead tuple versions that cause table   |
|    |                                        | bloat unless reclaimed by PostgreSQL VACUUM.        |
+---------------------------------------------------------------------------------------------------+
| 9  | "Using Sharding as the first scaling   | Always optimize indexes, add read replicas, and     |
|    | step."                                 | add Redis caching before introducing sharding.      |
+---------------------------------------------------------------------------------------------------+
| 10 | "Unindexed Foreign Keys."              | Unindexed FKs cause full table scans and lock       |
|    |                                        | contention on parent table updates/deletes.         |
+---------------------------------------------------------------------------------------------------+
```
