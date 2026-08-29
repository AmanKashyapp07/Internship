# Master Guide 03: Indexing, Transactions & Concurrency Control

> **Focus:** B+ Tree vs. B-Tree Storage Internals, Clustered vs. Secondary Indexes, ACID Engine Primitives (WAL & Undo Logs), Master Isolation Levels vs. Anomalies Matrix, MVCC Mechanics, and The Double-Booking Reservation Scenario.
> 
> *Targeted for Top-Tier Tech System & Backend Engineering Interviews.*

---

# Table of Contents
1. [Part A: Read Speed — Indexing Internals](#1-part-a-read-speed--indexing-internals)
2. [Clustered vs. Secondary Indexes & Covering Indexes](#2-clustered-vs-secondary-indexes--covering-indexes)
3. [Part B: Correctness — ACID Properties & Engine Mechanics](#3-part-b-correctness--acid-properties--engine-mechanics)
4. [The Master Isolation Levels vs. Anomalies Matrix](#4-the-master-isolation-levels-vs-anomalies-matrix)
5. [Multi-Version Concurrency Control (MVCC) Internals](#5-multi-version-concurrency-control-mvcc-internals)
6. [System Design Scenario: Solving the Double-Booking Problem](#6-system-design-scenario-solving-the-double-booking-problem)
7. [High-Frequency Interview Drill & Verbal Q&A](#7-high-frequency-interview-drill--verbal-qa)

---

# 1. Part A: Read Speed — Indexing Internals

```
B+ Tree Architecture (Height <= 3 for 1 Billion Rows):
                         [ Root Node: 100 | 500 ] (Routing Keys Only in RAM)
                                /       |       \
            [ Internal Node ]    [ Internal Node ]    [ Internal Node ]
               /        \           /        \           /        \
  [ Leaf: (1..99) ] <---> [ Leaf: (100..499) ] <---> [ Leaf: (500..999) ] (Doubly-Linked List in Disk Pages)
  (Stores Data Rows)       (Stores Data Rows)         (Stores Data Rows)
```

```
+---------------------------------------------------------------------------------------------------+
| ATTRIBUTE            | BINARY SEARCH TREE (BST)    | B-TREE                      | B+ TREE (INDUSTRY STANDARD)  |
+---------------------------------------------------------------------------------------------------+
| Node Sizing          | Tiny nodes (~24 bytes)      | Page-sized (8KB / 16KB)     | Page-sized (8KB / 16KB)      |
| Tree Height          | Tall (~30 disk seeks)       | Moderate (~5 disk seeks)    | Extremely Shallow (3-4 seeks)|
| Data Payload Location| Stored in every node        | Stored in internal + leaves | Stored EXCLUSIVELY in leaves |
| Internal Node Fan-out| 2 (Binary branching)        | Moderate (~20-50 keys)      | Massive (> 100-200 keys)     |
| Range Scan Support   | Slow in-order tree traversal| Requires tree traversals    | O(1) Doubly-linked leaf scan |
+---------------------------------------------------------------------------------------------------+
```

- **One-Line Intuition:** B+ Trees maximize fan-out by keeping internal routing nodes skinny (keys only), pushing all data to a doubly-linked leaf floor for lightning-fast range queries (`BETWEEN A AND B`).
- **The Write-Cost Tradeoff:** Every new secondary index speeds up `SELECT` reads but **penalizes `INSERT`, `UPDATE`, and `DELETE` writes**, which must update multiple B+ Tree indexes on disk.

---

# 2. Clustered vs. Secondary Indexes & Covering Indexes

```
MySQL InnoDB Index Lookup Mechanics:
Secondary Index on `email`:
[ 'alice@work.com' | PK: 101 ] ---> Secondary B+ Tree Search (Seek #1)
                                                |
                                                v
Clustered Index on `id` (PK):
[ id: 101 | 'Alice' | 'Engineering' | $120k ] ---> Primary B+ Tree Search (Seek #2: Double Lookup!)
```

### 1. Clustered Index (Primary Key)
- The table **IS** the index. Leaf pages store the complete, actual physical row data sorted by the primary key.
- A table can have **only ONE clustered index**.

### 2. Secondary Index & The Double Lookup Penalty
- Secondary index leaf nodes store the search key and a pointer to the **Primary Key value**.
- Finding non-indexed columns requires a second traversal through the Clustered Index (the "Double Lookup").

### 3. The Covering Index Fix
- A **Covering Index** includes all columns requested by the `SELECT` query (e.g. `CREATE INDEX idx_user ON Users(email, name)`).
- The query engine satisfies the request directly from the secondary index leaf node with **zero clustered index lookups**.

### 4. Leftmost Prefix Rule for Composite Indexes `(A, B, C)`
```
Query Filters:
WHERE A = 1 AND B = 2 AND C = 3  ---> FULL INDEX SEEK (Matches A, B, C)
WHERE A = 1 AND B = 2            ---> INDEX SEEK (Matches A, B)
WHERE A = 1                      ---> INDEX SEEK (Matches A)
WHERE B = 2 AND C = 3            ---> FULL TABLE SCAN! (Violates Leftmost Prefix: Missing A)
```

---

# 3. Part B: Correctness — ACID Properties & Engine Mechanics

```
+---------------------------------------------------------------------------------------------------+
| ACID PROPERTY        | 30-SECOND VERBAL SCRIPT              | UNDERLYING ENGINE PRIMITIVE         |
+---------------------------------------------------------------------------------------------------+
| Atomicity (A)        | All-or-nothing execution. Partial    | Undo Logs (Rollback segments in RAM |
|                      | failures roll back automatically.    | and disk)                           |
+---------------------------------------------------------------------------------------------------+
| Consistency (C)      | Preserves all database invariants and| Application schema, Foreign Keys,   |
|                      | constraints across transactions.     | Unique / Check constraints          |
+---------------------------------------------------------------------------------------------------+
| Isolation (I)        | Concurrent transactions execute      | 2-Phase Locking (2PL), MVCC         |
|                      | without seeing uncommitted changes.  | (Snapshot Isolation)                |
+---------------------------------------------------------------------------------------------------+
| Durability (D)       | Committed data survives power crashes| Redo Logs (Write-Ahead Logging /    |
|                      | and hardware reboots.                | WAL `fsync` to disk)                |
+---------------------------------------------------------------------------------------------------+
```

- **The Write-Ahead Logging (WAL) Rule:** A dirty data page in the Buffer Pool is **never written to disk until the corresponding Redo Log record has been flushed (`fsync`) to disk**.

---

# 4. The Master Isolation Levels vs. Anomalies Matrix

```
+---------------------------------------------------------------------------------------------------+
| ISOLATION LEVEL      | DIRTY READS | NON-REPEATABLE READS | PHANTOM READS | SERIALIZATION ANOMALIES|
+---------------------------------------------------------------------------------------------------+
| 1. Read Uncommitted  | YES (Allowed)| YES (Allowed)       | YES (Allowed) | YES (Allowed)          |
+---------------------------------------------------------------------------------------------------+
| 2. Read Committed    | NO          | YES (Allowed)        | YES (Allowed) | YES (Allowed)          |
|    (Postgres Default)|             |                      |               |                        |
+---------------------------------------------------------------------------------------------------+
| 3. Repeatable Read   | NO          | NO                   | NO (in InnoDB)| YES (Write Skew allowed|
|    (MySQL Default)   |             |                      | via Next-Key) | in Snapshot Isolation) |
+---------------------------------------------------------------------------------------------------+
| 4. Serializable      | NO          | NO                   | NO            | NO (True Serial Order) |
+---------------------------------------------------------------------------------------------------+
```

### Anomaly Definitions in 1 Line Each:
- **Dirty Read:** Transaction $B$ reads uncommitted modifications made by Transaction $A$ (which later rolls back).
- **Non-Repeatable Read:** Transaction $A$ reads row $X$, Transaction $B$ **updates row $X$ and commits**, Transaction $A$ re-reads row $X$ and sees different values.
- **Phantom Read:** Transaction $A$ executes a range query (`WHERE age > 30`), Transaction $B$ **inserts a brand new row**, Transaction $A$ re-executes the range query and sees new "phantom" rows.
- **Write Skew:** Concurrent transactions read overlapping data, check constraints independently, and write to disjoint rows that together violate a global invariant (e.g. withdrawing from checking and savings simultaneously).

---

# 5. Multi-Version Concurrency Control (MVCC) Internals

```
Row Evolution under MVCC:
Tuple v1: [ data: 'Alice' | xmin: 100 | xmax: 105 ]
Tuple v2: [ data: 'Alicia'| xmin: 105 | xmax: 0   ] (Created by Transaction 105)

Reader at Timestamp 102: Reads Tuple v1 (xmin <= 102 and xmax > 102) -> Zero Lock Contention!
```

- **The Core Rule of MVCC:** **Readers never block writers, and writers never block readers.**
- **Postgres Mechanics:** Each row stores `xmin` (creating transaction ID) and `xmax` (deleting/updating transaction ID). Updates insert a new row version rather than overwriting in-place.
- **Postgres Vacuuming:** Dead row versions (where `xmax < oldest_active_xid`) must be periodically purged by `VACUUM` to prevent table bloat and XID wraparound.

---

# 6. System Design Scenario: Solving the Double-Booking Problem

### The Product Scenario:
Two users simultaneously attempt to book the last available seat (`seat_id = 42`) on a flight.

### Solution 1: Pessimistic Locking (`SELECT ... FOR UPDATE`)
```sql
BEGIN TRANSACTION;
-- Acquires an exclusive row lock on seat 42; second transaction blocks until commit
SELECT status FROM Seats WHERE seat_id = 42 FOR UPDATE;

UPDATE Seats SET status = 'BOOKED', user_id = 101 WHERE seat_id = 42;
COMMIT;
```
- **Pros:** Guarantees zero conflicts; simple to implement.
- **Cons:** Holds database row locks, reducing concurrency under heavy traffic.

### Solution 2: Optimistic Locking with Version Number
```sql
-- Step 1: Read without locks
SELECT seat_id, status, version FROM Seats WHERE seat_id = 42; -- Returns version = 5

-- Step 2: Atomic update checking version
UPDATE Seats 
SET status = 'BOOKED', user_id = 101, version = version + 1
WHERE seat_id = 42 AND version = 5;

-- If rows_affected == 0, another user booked first -> Rollback and return retry error to user!
```
- **Pros:** Zero database locks, maximum throughput for low-contention workloads.

---

# 7. High-Frequency Interview Drill & Verbal Q&A

### Q1: What is the difference between Optimistic Concurrency Control (OCC) and Two-Phase Locking (2PL)?
> **Answer:** **2PL (Pessimistic)** locks resources upfront, blocking concurrent transactions to prevent conflicts at the cost of throughput. **OCC (Optimistic)** allows transactions to execute without locking, validating for conflicts only at commit time and aborting/retrying if a collision is detected.

### Q2: How does MySQL InnoDB prevent Phantom Reads under Repeatable Read?
> **Answer:** Using **Next-Key Locking**, which combines an index-record lock on the existing rows with a **Gap Lock** on the empty space between index records, preventing concurrent transactions from inserting new rows into the scanned range.

### Q3: What is the difference between Write-Ahead Logging (WAL) and Undo Logging?
> **Answer:** **WAL (Redo Log)** records changes to ensure committed transactions survive crashes (**Durability**). **Undo Logs** record inverse operations to roll back in-flight transactions on error or crash (**Atomicity** and MVCC snapshots).

### Q4: Why is an unindexed Foreign Key dangerous during concurrent transactions?
> **Answer:** Updating or deleting rows in the parent table forces the database engine to acquire **table-level locks or execute full scans** on the child table to verify referential integrity, causing massive concurrency bottlenecks.

### Q5: What is Index Condition Pushdown (ICP)?
> **Answer:** An optimization where the database engine evaluates `WHERE` filter conditions directly inside the storage engine's index scanning loop, avoiding the overhead of fetching full table rows into server memory for records that will ultimately be discarded.
