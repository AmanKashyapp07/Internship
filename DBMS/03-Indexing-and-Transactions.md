# Database Storage, Indexing & Transaction Concurrency Control

> **Scope:** B+ Tree vs. B-Tree Physical Page Internals, Clustered vs. Secondary Indexes, Covering Indexes, ACID Implementation Mechanics (WAL, Undo Logs, ARIES Recovery), ANSI Isolation Levels & Concurrency Anomalies, Multi-Version Concurrency Control (MVCC), and Two-Phase Locking (2PL).

---

# Table of Contents
1. [Physical Storage & Indexing Data Structures](#1-physical-storage--indexing-data-structures)
2. [Clustered vs. Secondary Indexes & Covering Index Optimization](#2-clustered-vs-secondary-indexes--covering-index-optimization)
3. [ACID Engine Implementation Primitives](#3-acid-engine-implementation-primitives)
4. [Transaction Isolation Levels & Concurrency Anomalies](#4-transaction-isolation-levels--concurrency-anomalies)
5. [Multi-Version Concurrency Control (MVCC) Mechanics](#5-multi-version-concurrency-control-mvcc-mechanics)
6. [Concurrency Control: Pessimistic (2PL) vs. Optimistic (OCC)](#6-concurrency-control-pessimistic-2pl-vs-optimistic-occ)
7. [Core Theoretical Summary Principles](#7-core-theoretical-summary-principles)

---

# 1. Physical Storage & Indexing Data Structures

Database systems organize on-disk storage into fixed-size **Pages** (typically 4KB, 8KB, or 16KB in InnoDB/PostgreSQL). To minimize disk I/O seek latency, database engines rely on tree-structured index hierarchies.

```
B+ Tree Architecture (Fan-out > 100, Height <= 3 for Millions of Tuples):
                         [ Root Node: 100 | 500 ] (Routing Keys Only in Memory)
                                /       |       \
            [ Internal Node ]    [ Internal Node ]    [ Internal Node ]
               /        \           /        \           /        \
  [ Leaf: (1..99) ] <---> [ Leaf: (100..499) ] <---> [ Leaf: (500..999) ] (Doubly-Linked List in Disk Pages)
  (Stores Row Pointers)    (Stores Row Pointers)      (Stores Row Pointers)
```

### Storage Hierarchy Comparison:

```
+---------------------------------------------------------------------------------------------------+
| ATTRIBUTE            | BINARY SEARCH TREE (BST)    | B-TREE                      | B+ TREE (STANDARD)           |
+---------------------------------------------------------------------------------------------------+
| Node Sizing          | Tiny nodes (~24 bytes)      | Page-sized (8KB / 16KB)     | Page-sized (8KB / 16KB)      |
| Tree Height          | Deep (O(log2 N), ~30 seeks) | Moderate (O(logB N), ~5)    | Shallow (O(logB N), 3-4 seeks) |
| Data Payload Location| Stored in every node        | Stored in internal + leaves | Stored EXCLUSIVELY in leaves |
| Internal Fan-out     | 2 (Binary branching)        | Moderate (~20-50 keys)      | Massive (> 100-200 keys)     |
| Range Scan Support   | O(N) In-order traversal     | O(N log N) Tree traversals  | O(K) Sequential leaf scan    |
+---------------------------------------------------------------------------------------------------+
```

### Physical Mechanics:
- **High Fan-Out ($B$):** Internal routing nodes store only search keys and child page pointers without tuple payloads, maximizing the number of keys fitting in a single disk page.
- **Sequential Range Traversal:** Leaf pages form a doubly-linked list on disk, allowing range predicates (`WHERE age BETWEEN 20 AND 30`) to execute via a single tree search followed by sequential horizontal leaf page scans.

---

# 2. Clustered vs. Secondary Indexes & Covering Index Optimization

```
Storage Index Lookup Architecture:
Secondary Index on `email`:
[ 'alice@work.com' | PK: 101 ] ---> Secondary B+ Tree Search (Seek #1)
                                                |
                                                v
Clustered Index on `id` (PK):
[ id: 101 | 'Alice' | 'Engineering' | $120k ] ---> Primary B+ Tree Search (Seek #2: Clustered Lookup)
```

### 1. Clustered Index (Primary Table Storage)
- The table rows are physically ordered and stored on disk inside the leaf pages of the Clustered Index B+ Tree.
- A relation can have **exactly one clustered index** (usually the Primary Key).

### 2. Secondary (Non-Clustered) Index
- Built on non-primary key columns. Leaf nodes store the indexed key along with a pointer to the tuple's Primary Key value.
- **Double Lookup (Heap/Clustered Fetch):** Retrieving columns not present in the secondary index requires traversing the secondary index tree to find the Primary Key, then traversing the Clustered Index tree to fetch the full row.

### 3. Covering Index Optimization
- An index that contains all columns requested by a query (e.g. `CREATE INDEX idx_user ON Users(email, name)` or PostgreSQL `CREATE INDEX ... INCLUDE (name)`).
- The query planner satisfies the query entirely from the secondary index leaf pages, bypassing clustered index page fetches (**Index-Only Scan**).

### 4. Leftmost Prefix Rule for Composite Indexes `(A, B, C)`
```
Query Predicates:
WHERE A = 1 AND B = 2 AND C = 3  ---> FULL INDEX SEEK (Matches A, B, C)
WHERE A = 1 AND B = 2            ---> INDEX SEEK (Matches A, B)
WHERE A = 1                      ---> INDEX SEEK (Matches A)
WHERE B = 2 AND C = 3            ---> FULL RELATION SCAN (Violates Leftmost Prefix: Missing leading column A)
```

---

# 3. ACID Engine Implementation Primitives

```
+---------------------------------------------------------------------------------------------------+
| ACID PROPERTY        | FORMAL DEFINITION                   | UNDERLYING ENGINE IMPLEMENTATION     |
+---------------------------------------------------------------------------------------------------+
| Atomicity (A)        | All-or-nothing execution. All writes| Undo Logs (Rollback segments stored  |
|                      | commit or all roll back on error.   | on disk and memory buffer pools)     |
+---------------------------------------------------------------------------------------------------+
| Consistency (C)      | State transitions preserve schema   | Relational DDL invariants, Foreign   |
|                      | constraints and relational rules.   | Keys, Unique & Check constraints     |
+---------------------------------------------------------------------------------------------------+
| Isolation (I)        | Concurrent transactions execute as  | Two-Phase Locking (2PL), MVCC        |
|                      | if running serially without leaks.  | (Snapshot Isolation, SSI)            |
+---------------------------------------------------------------------------------------------------+
| Durability (D)       | Committed state survives crashes,   | Write-Ahead Logging (Redo Log WAL    |
|                      | power failures, and reboot events.  | flushed via `fsync` to disk)         |
+---------------------------------------------------------------------------------------------------+
```

### Write-Ahead Logging (WAL) & ARIES Recovery:
- **The WAL Invariant:** A dirty memory buffer page can **never be written to non-volatile disk storage until the corresponding Redo Log record has been flushed (`fsync`) to disk**.
- **ARIES Recovery Phases:**
  1. **Analysis Phase:** Scans the WAL forward from the last checkpoint to identify active transactions and dirty buffer pages at crash time.
  2. **Redo Phase:** Replays all log records forward from the earliest unwritten page modification to restore exact pre-crash state (Repeating History).
  3. **Undo Phase:** Reverses the operations of all transactions that were active (uncommitted) at the time of crash using Undo log entries.

---

# 4. Transaction Isolation Levels & Concurrency Anomalies

```
+---------------------------------------------------------------------------------------------------+
| ISOLATION LEVEL      | DIRTY READS | NON-REPEATABLE READS | PHANTOM READS | SERIALIZATION ANOMALIES|
+---------------------------------------------------------------------------------------------------+
| 1. Read Uncommitted  | ALLOWED     | ALLOWED              | ALLOWED       | ALLOWED                |
+---------------------------------------------------------------------------------------------------+
| 2. Read Committed    | PREVENTED   | ALLOWED              | ALLOWED       | ALLOWED                |
|    (Postgres Default)|             |                      |               |                        |
+---------------------------------------------------------------------------------------------------+
| 3. Repeatable Read   | PREVENTED   | PREVENTED            | PREVENTED*    | ALLOWED (Write Skew    |
|    (MySQL Default)   |             |                      | (*in InnoDB)  | in Snapshot Isolation) |
+---------------------------------------------------------------------------------------------------+
| 4. Serializable      | PREVENTED   | PREVENTED            | PREVENTED     | PREVENTED (True Serial)|
+---------------------------------------------------------------------------------------------------+
```

### Formal Anomaly Definitions:
1. **Dirty Read ($G_1$):** Transaction $T_2$ reads data modified by active transaction $T_1$, which subsequently aborts and rolls back.
2. **Non-Repeatable Read ($G_{2a}$ / Fuzzy Read):** Transaction $T_1$ reads a row. Transaction $T_2$ modifies that row and commits. $T_1$ re-reads the row and observes the modified values.
3. **Phantom Read ($A_3$):** Transaction $T_1$ reads a range of tuples matching predicate $P$. Transaction $T_2$ inserts new tuples satisfying $P$ and commits. $T_1$ re-executes the predicate query and observes a different set of rows.
4. **Write Skew:** Two concurrent transactions under Snapshot Isolation read overlapping datasets, independently verify an invariant, and make disjoint writes that collectively violate the global database invariant.

---

# 5. Multi-Version Concurrency Control (MVCC) Mechanics

MVCC eliminates read-write lock contention by creating timestamped tuple versions on write rather than modifying data in-place under exclusive locks.

```
Row Evolution under MVCC:
Tuple v1: [ data: 'Alice' | xmin: 100 | xmax: 105 ]
Tuple v2: [ data: 'Alicia'| xmin: 105 | xmax: 0   ] (Created by Transaction 105)

Reader with Snapshot Timestamp 102: Reads Tuple v1 (xmin <= 102 and xmax > 102) -> Zero Lock Contention!
```

### Engine Mechanics:
- **Concurrency Rule:** **Readers do not block writers, and writers do not block readers.**
- **PostgreSQL Tuple Headers:**
  - `xmin`: The transaction identifier that inserted the row version.
  - `xmax`: The transaction identifier that deleted or updated the row version (0 if active).
- **Snapshot Visibility:** A reader evaluates each tuple version against its transaction snapshot descriptor (active transactions list and snapshot watermark), determining visibility without acquiring shared read locks.
- **Garbage Collection (Vacuuming):** Outdated tuple versions whose `xmax` precedes all active transaction snapshots must be reclaimed (`VACUUM`) to prevent disk bloat and 32-bit transaction ID (`XID`) wraparound.

---

# 6. Concurrency Control: Pessimistic (2PL) vs. Optimistic (OCC)

### 1. Pessimistic Concurrency Control (Strict Two-Phase Locking - S2PL)
- Transactions acquire shared locks ($S$) for reads and exclusive locks ($X$) for writes.
- **Growing Phase:** Locks are acquired as needed; no locks can be released.
- **Shrinking Phase:** All locks are held until transaction commit or abort (Strict 2PL), guaranteeing conflict serializability and avoiding cascading aborts.

```sql
-- Explicit row locking under Pessimistic model
BEGIN TRANSACTION;
SELECT status FROM Inventory WHERE item_id = 42 FOR UPDATE; -- Acquires exclusive row lock
UPDATE Inventory SET status = 'RESERVED' WHERE item_id = 42;
COMMIT; -- Releases row lock
```

### 2. Optimistic Concurrency Control (OCC)
- Assumes transaction collisions are rare. Transactions execute without acquiring locks, buffering writes locally.
- **Phases:** Read Phase $\to$ Validation Phase (verifies read-set invariants against concurrent commits) $\to$ Write/Commit Phase.
- If validation fails, the transaction aborts and retries.

```sql
-- Optimistic locking via state versioning
SELECT item_id, status, version FROM Inventory WHERE item_id = 42; -- Returns version = 5

UPDATE Inventory 
SET status = 'RESERVED', version = version + 1
WHERE item_id = 42 AND version = 5;

-- If rows_affected == 0, abort and retry
```

---

# 7. Core Theoretical Summary Principles

1. **B+ Tree vs. B-Tree:** B+ Trees store all data exclusively in leaf pages, maximizing internal node fan-out and enabling $O(K)$ sequential range queries via horizontal doubly-linked leaf pointers.
2. **Clustered vs. Secondary Indexes:** A clustered index determines the physical on-disk tuple ordering. Secondary indexes reference the primary key and require a secondary traversal unless satisfied by a Covering Index.
3. **Write-Ahead Logging:** Ensures Durability by forcing Redo log records to non-volatile storage (`fsync`) before corresponding dirty buffer pool pages are written to disk.
4. **MVCC Isolation:** Solves read-write contention by maintaining versioned tuples tagged with transaction lifespans (`xmin`/`xmax`), providing snapshot isolation without read locks.
5. **Next-Key Locking:** Combines record locks with gap locks in index B-Trees to prevent Phantom Reads in Repeatable Read isolation.
