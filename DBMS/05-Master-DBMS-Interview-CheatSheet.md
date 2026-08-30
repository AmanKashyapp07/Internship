# Core DBMS Theorems, Formulas & Architecture Reference

> **Scope:** Unified Database System Architecture Matrix, Mathematical Formulas & Relational Axioms, 50 Foundational Database Engineering Principles, and Engine Failure Modes & Design Pitfalls.

---

# Table of Contents
1. [Unified Database System Architecture Matrix](#1-unified-database-system-architecture-matrix)
2. [Mathematical Formulas & Theoretical Quick-Reference](#2-mathematical-formulas--theoretical-quick-reference)
3. [50 Foundational Database Engineering Principles](#3-50-foundational-database-engineering-principles)
4. [Critical Database Engine Design Pitfalls](#4-critical-database-engine-design-pitfalls)

---

# 1. Unified Database System Architecture Matrix

```
+--------------------------------------------------------------------------------------------------------------------+
| CORE DATABASE TOPIC  | KEY DATABASE PRIMITIVE           | ARCHITECTURAL APPLICATION & IMPACT                |
+--------------------------------------------------------------------------------------------------------------------+
| Schema Normalization | 3NF / BCNF Decomposition         | Minimizing data redundancy, OLTP transactional integrity|
+--------------------------------------------------------------------------------------------------------------------+
| Denormalization      | Redundant embedded attributes    | High-throughput read dashboards, Wide-Column stores|
+--------------------------------------------------------------------------------------------------------------------+
| B+ Tree Indexing     | Clustered Index, Covering Index  | Sub-millisecond point lookups, sequential range scans|
+--------------------------------------------------------------------------------------------------------------------+
| Write-Ahead Log (WAL)| Append-Only Redo Log `fsync`     | Durability, ARIES crash recovery, Change Data Capture|
+--------------------------------------------------------------------------------------------------------------------+
| MVCC & Isolation     | `xmin`/`xmax`, Snapshot Isolation| Non-blocking concurrent reads, optimistic versioning|
+--------------------------------------------------------------------------------------------------------------------+
| Read Replicas        | Single-Leader Async Replication  | Read/Write splitting, analytical workload offload  |
+--------------------------------------------------------------------------------------------------------------------+
| Sharding             | Consistent Hashing Shard Keys    | Horizontal multi-node scaling, cross-shard joins   |
+--------------------------------------------------------------------------------------------------------------------+
```

---

# 2. Mathematical Formulas & Theoretical Quick-Reference

```
+---------------------------------------------------------------------------------------------------+
| 1. SQL LOGICAL EVALUATION SEQUENCE                                                                |
|    FROM -> JOIN -> WHERE -> GROUP BY -> HAVING -> SELECT -> DISTINCT -> ORDER BY -> LIMIT/OFFSET  |
+---------------------------------------------------------------------------------------------------+
| 2. ISOLATION LEVELS VS. ANOMALIES MATRIX                                                          |
|    * Read Uncommitted: Dirty Reads, Non-Repeatable Reads, Phantom Reads, Write Skew allowed       |
|    * Read Committed:   Dirty Reads prevented; Non-Repeatable Reads, Phantoms, Write Skew allowed  |
|    * Repeatable Read:  Dirty & Non-Repeatable Reads prevented; Phantoms prevented in InnoDB; Skew |
|    * Serializable:     All anomalies prevented (Strict Serial Equivalence)                        |
+---------------------------------------------------------------------------------------------------+
| 3. DISTRIBUTED QUORUM CONSISTENCY FORMULA                                                         |
|    W + R > N (N = Total Replicas, W = Write Quorum, R = Read Quorum) -> Linearizable Consistency  |
+---------------------------------------------------------------------------------------------------+
| 4. NORMALIZATION CHECKLIST                                                                        |
|    * 1NF: Atomic values only (No repeating groups or array attributes)                             |
|    * 2NF: 1NF + No partial functional dependencies on composite candidate keys                    |
|    * 3NF: 2NF + No transitive functional dependencies between non-prime attributes                |
|    * BCNF: 3NF + For every non-trivial FD X -> Y, X MUST be a Super Key                           |
+---------------------------------------------------------------------------------------------------+
```

---

# 3. 50 Foundational Database Engineering Principles

### Category A: Relational Schema & Normalization
1. **Candidate Key vs. Primary Key:** A Candidate Key is a minimal super key; the Primary Key is the designated candidate key that defines physical clustered indexing.
2. **Key Randomness Impact:** Non-sequential primary keys (UUIDv4) cause B+ Tree page splits, fragmentation, and reduced page fill factors (~50%). Monotonic identifiers (BIGINT, UUIDv7) preserve append-only leaf layout.
3. **3NF vs. BCNF Boundary:** 3NF permits $X \to Y$ if $Y$ is a prime attribute; BCNF strictly requires $X$ to be a super key for all non-trivial dependencies.
4. **Denormalization Criteria:** Appropriate when read throughput demands pre-computed aggregations or join avoidance, trading off write amplification and update consistency.
5. **M:N Relational Mapping:** Requires a junction table with foreign keys referencing both participating entities, joined under a composite primary key.
6. **Surrogate Keys:** System-generated identifiers independent of mutable business domain logic.
7. **Referential Integrity:** Guarantees foreign key values strictly reference existing primary keys in parent relations.
8. **Weak Entities:** Entities lacking primary keys of their own, identified via partial keys combined with owner entity primary keys.

---

### Category B: Query Engine & Evaluation Semantics
9. **Projection Scoping:** Column aliases defined in `SELECT` cannot be referenced in `WHERE` or `GROUP BY` because projection evaluates after filtering and grouping.
10. **WHERE vs. HAVING:** `WHERE` filters input rows prior to grouping; `HAVING` filters aggregated groups following `GROUP BY`.
11. **RANK() vs. DENSE_RANK():** `RANK()` introduces gaps in rank sequences on ties (1, 1, 3); `DENSE_RANK()` assigns contiguous ranks (1, 1, 2).
12. **Three-Valued Logic in NOT IN:** Conjunction with `NULL` yields `UNKNOWN`, causing `x NOT IN (subquery_with_null)` to return zero rows.
13. **UNION vs. UNION ALL:** `UNION` performs explicit sorting/hashing for deduplication; `UNION ALL` performs direct concatenation.
14. **Anti-Joins:** Expressed via `LEFT JOIN ... WHERE child.id IS NULL` or `NOT EXISTS` to find non-matching records.
15. **Self-Joins:** Unary joins utilized to query recursive hierarchies within a single relation.
16. **LEAD() and LAG():** Window navigation functions retrieving relative offset rows within a partition without self-joining.

---

### Category C: Indexing & Storage Engine Physical Architecture
17. **B+ Tree Physical Advantages:** Aligns with disk page allocation, offers large fan-out ($>100$), keeps tree depth shallow ($\le 3$), and links leaf nodes for sequential range traversals.
18. **Clustered Index Physical Layout:** Leaf pages contain the actual table rows sorted by the clustered index key.
19. **Secondary Index Lookups:** Secondary index leaf nodes store the indexed attribute and a pointer to the primary key, requiring a secondary clustered index traversal for non-indexed attributes.
20. **Covering Index Optimization:** All queried attributes reside within the secondary index leaf nodes, satisfying queries via Index-Only Scans.
21. **Leftmost Prefix Invariant:** Multi-column composite indexes `(A, B, C)` require leading attributes in predicates to perform index seeks.
22. **Index Write Penalty:** Secondary indexes accelerate read lookups but add disk I/O overhead to `INSERT`, `UPDATE`, and `DELETE` operations.
23. **Leading Wildcard Scans:** Pattern matching with leading wildcards (`LIKE '%text'`) prevents index tree seeks, forcing full relation scans.
24. **Index Condition Pushdown (ICP):** Evaluates `WHERE` predicates directly inside the storage engine index traversal loop.

---

### Category D: Transactions, ACID & Concurrency Control
25. **Atomicity Guarantee:** All transactional modifications commit together or roll back completely via Undo log records.
26. **Write-Ahead Logging Invariant:** Redo log records must be flushed to non-volatile storage (`fsync`) before corresponding dirty buffer pool pages are written to disk.
27. **Dirty Read Anomaly:** Reading uncommitted modifications from a concurrent transaction that subsequently aborts.
28. **Non-Repeatable Read Anomaly:** A transaction re-reads a row and observes modifications committed by a concurrent transaction.
29. **Phantom Read Anomaly:** A transaction re-executes a range query and observes newly inserted tuples committed by a concurrent transaction.
30. **Write Skew Anomaly:** Concurrent transactions read overlapping state and update disjoint records, violating a global multi-record constraint.
31. **MVCC Architecture:** Maintains timestamped tuple versions so readers read historical snapshots without acquiring shared locks.
32. **PostgreSQL Tuple Headers:** Stores `xmin` (creating transaction ID) and `xmax` (deleting transaction ID) in row headers.
33. **PostgreSQL Vacuuming:** Purges dead row versions whose lifespans precede all active transaction snapshots.
34. **Strict Two-Phase Locking (S2PL):** Growing phase acquires locks; all locks are held until transaction termination to ensure conflict serializability.
35. **Next-Key Locking:** Combines index record locks with gap locks to eliminate phantom reads in MySQL InnoDB Repeatable Read.
36. **Optimistic vs. Pessimistic Locking:** Pessimistic locks rows upfront (`FOR UPDATE`); Optimistic checks a `version` field on write and retries on conflict.

---

### Category E: Distributed Databases, Replication & Partitioning
37. **CAP Theorem:** During an inevitable network partition ($P$), a distributed data store must choose between Consistency ($C$) and Availability ($A$).
38. **CP vs. AP Posture:** CP systems reject conflicting writes during partitions to preserve correctness; AP systems accept writes, resolving divergence eventually.
39. **BASE Principles:** Basically Available, Soft state, Eventual consistency.
40. **Quorum Overlap Condition:** $W + R > N$ guarantees that read and write quorums intersect on at least one replica holding the latest write.
41. **Replication Lag Handling:** Guarantees Read-Your-Own-Writes by routing immediate post-write reads to the primary leader.
42. **Horizontal Sharding:** Distributes relation tuples across separate physical database instances based on a Shard Key.
43. **Cross-Shard Join Overhead:** Multi-shard joins require network data transfer and application-tier distributed hash joins.
44. **Consistent Hashing Mechanism:** Maps keys and nodes to a circular hash ring, ensuring node membership changes migrate only $K/N$ keys.
45. **Document Store Domain:** Semi-structured data, polymorphic payloads, and single-document ACID operations.
46. **Wide-Column Store Domain:** High-throughput sequential write workloads, time-series metrics, and append-heavy logs.
47. **Graph Store Domain:** Interconnected network traversal with index-free adjacency pointers.
48. **Change Data Capture (CDC):** Tails transaction Write-Ahead Logs to asynchronously propagate committed state to external systems.
49. **Database Scaling Progression:** Query optimization $\to$ Read Replicas $\to$ In-Memory Caching $\to$ Horizontal Sharding.
50. **Split-Brain Prevention:** Uses majority quorum consensus algorithms (Raft, Paxos) requiring $> N/2$ active nodes to elect leaders.

---

# 4. Critical Database Engine Design Pitfalls

```
+---------------------------------------------------------------------------------------------------+
| #  | ENGINE DESIGN PITFALL                  | THEORETICAL ROOT CAUSE & ARCHITECTURAL MITIGATION   |
+---------------------------------------------------------------------------------------------------+
| 1  | Subquery `NOT IN` with Nullable Columns| Three-valued logic yields UNKNOWN; use NOT EXISTS or|
|    |                                        | LEFT JOIN ... WHERE ... IS NULL.                    |
+---------------------------------------------------------------------------------------------------+
| 2  | Non-Sequential Primary Keys (UUIDv4)   | Causes random I/O and B+ Tree leaf page splits; use |
|    |                                        | monotonic BIGINT or timestamp-ordered UUIDv7.       |
+---------------------------------------------------------------------------------------------------+
| 3  | Over-Indexing Tables                   | Adds write amplification on disk across INSERT,     |
|    |                                        | UPDATE, and DELETE operations.                      |
+---------------------------------------------------------------------------------------------------+
| 4  | Unindexed Foreign Keys                 | Forces full relation scans or table locks on parent |
|    |                                        | modifications to enforce referential integrity.     |
+---------------------------------------------------------------------------------------------------+
| 5  | Unchecked MVCC Dead Tuples             | Produces table bloat and XID wraparound; requires   |
|    |                                        | tuned background VACUUM maintenance.                |
+---------------------------------------------------------------------------------------------------+
| 6  | Cross-Shard Relational Joins           | Forces scatter-gather network transfer and memory   |
|    |                                        | joins; co-locate related entities via Shard Keys.   |
+---------------------------------------------------------------------------------------------------+
| 7  | Application Dual-Writing               | Partial failures cause silent cache/search index    |
|    |                                        | drift; use Write-Ahead Log Change Data Capture.     |
+---------------------------------------------------------------------------------------------------+
```
