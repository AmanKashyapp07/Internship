# 01: Transactions, Concurrency Control & Crash Recovery

> **Target:** Top Product & FinTech Technical Interviews (FAANG, Uber, Stripe, Atlassian, Citadel).  
> **Style:** High-ROI revision sheet. Focuses on low-level database engine mechanics, MVCC, lock hierarchies, isolation anomalies, WAL logging, and crash recovery.

---

# Table of Contents
1. [ACID Properties Deep Dive](#1-acid-properties-deep-dive)
2. [Transaction Isolation Levels & Concurrency Anomalies](#2-transaction-isolation-levels--concurrency-anomalies)
3. [Concurrency Control Architectures (Pessimistic 2PL vs. OCC vs. MVCC)](#3-concurrency-control-architectures)
4. [Locking Mechanisms & Hierarchies](#4-locking-mechanisms--hierarchies)
5. [Deadlock Detection & Prevention Strategies](#5-deadlock-detection--prevention-strategies)
6. [Storage Engines, WAL & Crash Recovery (ARIES)](#6-storage-engines-wal--crash-recovery-aries)
7. [High-Yield Interview Questions & Defense](#7-high-yield-interview-questions--defense)
8. [Quick Recap](#8-quick-recap)

---

# 1. ACID Properties Deep Dive

A **Database Transaction** is a logical unit of work comprising one or more database operations (reads, writes, updates) executed against shared storage.

```
+-----------------------------------------------------------------------------------------+
| PROPERTY        | CORE MEANING                         | ENGINE IMPLEMENTATION PRIMITIVE|
+-----------------------------------------------------------------------------------------+
| Atomicity (A)   | "All or Nothing"                     | Undo Logs (WAL / Rollback Seg) |
| Consistency (C) | Valid state to valid state           | Schema Constraints, Foreign    |
|                 | (preserves business invariants)      | Keys, Application Logic        |
| Isolation (I)   | Concurrent transactions do not       | 2-Phase Locking (2PL), MVCC    |
|                 | interfere with each other            | Snapshot Isolation             |
| Durability (D)  | Committed data survives crashes/power| Redo Logs (WAL fsync), Flash   |
|                 | failures permanently                 | Checkpoints to Non-Volatile Disk|
+-----------------------------------------------------------------------------------------+
```

---

### Atomicity vs. Isolation (The Senior Distraction Trap)
- **Atomicity (Single Transaction Focus):** Guarantees that if a transaction performs 5 updates and crashes on step 4, the database uses its **Undo Log** to roll back steps 1–3, leaving the database as if the transaction never started. It does *not* protect against concurrent observers seeing intermediate state.
- **Isolation (Multi-Transaction Focus):** Guarantees that while Transaction A is executing steps 1–4, concurrent Transaction B cannot observe intermediate or uncommitted values.
- **Key Interview Nuance:** A transaction can have **100% Atomicity while having zero Isolation** (e.g. at Read Uncommitted level).

---

### Consistency: Database-Enforced vs. Application-Enforced
- **Database Consistency:** Enforces schema declarations (e.g., `NOT NULL`, `CHECK (age >= 18)`, Foreign Key integrity).
- **Application Consistency (C in ACID):** Enforces business logic rules (e.g., in a bank transfer between accounts $A$ and $B$, the sum $A + B$ must remain constant before and after the transaction).
- **Note:** The "C" in ACID is fundamentally an application-level property, distinct from the "C" (Linearizability/Single-copy Consistency) in the CAP theorem!

---

### Durability & The `fsync` Disk Guarantee
- Durability is achieved when a transaction's commit record is physically flushed to persistent non-volatile media (SSD/HDD) via the **`fsync()` system call**.
- **The Trade-Off:** Calling `fsync()` on every single commit introduces disk I/O latency (~1–5ms on SSDs). Databases use **Group Commits** to batch multiple concurrent transaction commits into a single sequential `fsync()` disk flush.

---

# 2. Transaction Isolation Levels & Concurrency Anomalies

The ANSI SQL standard defines **4 standard Isolation Levels**, structured around which concurrency anomalies they prevent.

```
ANSI SQL Isolation vs. Anomaly Matrix:
+-----------------------------------------------------------------------------------------+
| ISOLATION LEVEL      | DIRTY READ        | NON-REPEATABLE READ | PHANTOM READ           |
+-----------------------------------------------------------------------------------------+
| Read Uncommitted     | YES (Allowed)     | YES (Allowed)       | YES (Allowed)          |
| Read Committed       | NO (Prevented)    | YES (Allowed)       | YES (Allowed)          |
| Repeatable Read      | NO (Prevented)    | NO (Prevented)      | YES (Allowed in ANSI)* |
| Serializable         | NO (Prevented)    | NO (Prevented)      | NO (Prevented)         |
+-----------------------------------------------------------------------------------------+
*Note: In PostgreSQL and MySQL InnoDB, Repeatable Read ALSO prevents Phantom Reads via MVCC and Next-Key Locks!
```

---

### Deep Dive: Concurrency Anomalies Explained

```
1. DIRTY READ (G-1):
Transaction A updates row X (Balance = $500 -> $1000) WITHOUT committing.
Transaction B reads row X (sees $1000).
Transaction A ROLLS BACK.
--> Transaction B operated on data that NEVER officially existed in the database!

2. NON-REPEATABLE READ / FUZZY READ (G-2a):
Transaction A reads row X (sees Balance = $500).
Transaction B updates row X (Balance = $500 -> $800) AND COMMITS.
Transaction A reads row X again (sees Balance = $800).
--> A single transaction reads TWO DIFFERENT VALUES for the SAME exact row!

3. PHANTOM READ (G-2b):
Transaction A reads a range: SELECT COUNT(*) WHERE age >= 21 (Returns 10 rows).
Transaction B INSERTS a new row (User with age = 25) AND COMMITS.
Transaction A reads the same range again: SELECT COUNT(*) WHERE age >= 21 (Returns 11 rows).
--> New "phantom" rows appear inside a range query that were not there previously!

4. LOST UPDATE (P4):
Transaction A and Transaction B both read Balance = $100.
Transaction A writes Balance = $100 + $50 = $150.
Transaction B writes Balance = $100 + $20 = $120 (overwriting A's write).
--> Transaction A's $50 deposit is completely LOST! (Fixed via SELECT ... FOR UPDATE).

5. WRITE SKEW (Snapshot Isolation Anomaly):
Constraint: Balance(Checking) + Balance(Savings) >= 0. Current: Checking = $100, Savings = $100.
Txn A reads Checking ($100) and Savings ($100), withdraws $150 from Checking (New: -$50 + $100 >= 0 -> Valid!).
Txn B concurrently reads Checking ($100) and Savings ($100), withdraws $150 from Savings (New: $100 + -$50 >= 0 -> Valid!).
Both Txn A and Txn B commit under Snapshot Isolation!
--> Final State: Checking = -$50, Savings = -$50 (Total = -$100 -> Invariant Violated!).
```

---

# 3. Concurrency Control Architectures

Databases manage concurrent access using one of three primary paradigms:

```
+-----------------------------------------------------------------------------------------+
| 1. PESSIMISTIC CONCURRENCY CONTROL (2-Phase Locking - 2PL)                              |
| - Assumes conflicts WILL happen.                                                        |
| - Acquires locks before reading/writing. Blocks concurrent conflicting transactions.     |
| - Trade-off: High contention, latency spikes, deadlock risk.                            |
+-----------------------------------------------------------------------------------------+
| 2. OPTIMISTIC CONCURRENCY CONTROL (OCC)                                                 |
| - Assumes conflicts are RARE.                                                           |
| - Reads and writes execute in private local workspace (3 Phases: Read, Validate, Write).|
| - Validates timestamps at commit time; if conflict occurred, transaction ROLLS BACK.    |
| - Trade-off: Zero lock overhead, but high abort rates under heavy write contention.     |
+-----------------------------------------------------------------------------------------+
| 3. MULTI-VERSION CONCURRENCY CONTROL (MVCC)                                             |
| - Industry standard in modern databases (PostgreSQL, MySQL InnoDB, Oracle).             |
| - Updates/Deletes do not overwrite data in place; they write a NEW ROW VERSION.         |
| - Golden Rule: "READERS NEVER BLOCK WRITERS, AND WRITERS NEVER BLOCK READERS."          |
+-----------------------------------------------------------------------------------------+
```

---

### Deep Dive: MVCC Mechanics in PostgreSQL
Every tuple (row) in PostgreSQL stores hidden header metadata:
- **`xmin`:** The transaction ID (`txid`) that created/inserted this row version.
- **`xmax`:** The transaction ID (`txid`) that deleted or updated this row version (0 if active).

```
PostgreSQL MVCC Row Lifecycle:

1. Txn 100 INSERTS user 'Aman':
   [ Heap Tuple: Aman | xmin = 100 | xmax = 0 ]

2. Txn 105 UPDATES user 'Aman' to 'Aman Kashyap':
   - Sets old tuple's xmax = 105 (marks old version deleted).
   - Appends NEW tuple version:
   [ Old Tuple: Aman         | xmin = 100 | xmax = 105 ] (Dead tuple once Txn 105 commits)
   [ New Tuple: Aman Kashyap | xmin = 105 | xmax = 0   ] (Active version)

3. Visibility Rule for Concurrent Txn 102:
   - Txn 102 started before Txn 105 committed.
   - Txn 102 inspects Old Tuple: xmin (100) < 102 (Visible) and xmax (105) > 102 (Ignore deletion!).
   - Txn 102 reads 'Aman' without blocking Txn 105!
```

- **Table Bloat & `AUTOVACUUM`:** Dead row versions occupy disk space until the background `AUTOVACUUM` daemon sweeps table pages and reclaims space for new inserts.

---

# 4. Locking Mechanisms & Hierarchies

## A. Shared ($S$) vs. Exclusive ($X$) Locks
- **Shared Lock ($S$ / Read Lock):** Multiple transactions can hold $S$ locks on the same resource concurrently (`SELECT ... LOCK IN SHARE MODE`).
- **Exclusive Lock ($X$ / Write Lock):** Only one transaction can hold an $X$ lock. No other transaction can acquire $S$ or $X$ locks on that resource (`SELECT ... FOR UPDATE`, `UPDATE`, `DELETE`).

```
Lock Compatibility Matrix:
                 Requested Lock:
Existing Lock:     Shared (S)       Exclusive (X)
Shared (S)         COMPATIBLE       CONFLICT (Blocked)
Exclusive (X)      CONFLICT (Block) CONFLICT (Blocked)
```

---

## B. Two-Phase Locking (2PL) Protocol
2PL guarantees **Conflict Serializability** by dividing transaction locking into two distinct phases:

```
Lock Count
    ^
    |                   [ GROWING PHASE ]              [ SHRINKING PHASE ]
    |                 Acquires locks only.             Releases locks only.
    |                 Zero locks released.             Zero locks acquired.
    |                        /\                                \
    |                       /  \                                \
    |                      /    \ [ LOCK POINT ]                 \
    |                     /      \ (All locks acquired)           \
    +--------------------+--------+--------------------------------+---> Time
```

1. **Strict 2PL (S2PL):** Holds all **Exclusive ($X$) locks** until transaction `COMMIT` or `ROLLBACK` (prevents Cascading Aborts).
2. **Rigorous 2PL (SS2PL):** Holds **ALL Shared ($S$) and Exclusive ($X$) locks** until `COMMIT`/`ROLLBACK` (guarantees serializable ordering).

---

## C. Intent Locks & Granularity Hierarchy
When locking a row, how does the database prevent another transaction from acquiring an exclusive lock on the entire Table without scanning millions of rows? **Intent Locks** solve this!

```
Lock Granularity Hierarchy:
           [ DATABASE ]
                |
            [ TABLE ]        <--- Intent Locks (IS / IX) placed here!
                |
            [ PAGE ]
                |
            [ ROW / TUPLE ]  <--- Explicit Locks (S / X) placed here!
```

- **Intent Shared ($IS$):** Indicates an intention to set an $S$ lock on a child row.
- **Intent Exclusive ($IX$):** Indicates an intention to set an $X$ lock on a child row.
- **Protocol:** To acquire an $X$ lock on Row $R$, the transaction must first acquire an **$IX$ lock on the parent Table**. A concurrent `LOCK TABLE WRITE` request sees the table's $IX$ lock and blocks immediately in $O(1)$ time without scanning individual rows.

---

## D. Gap Locks & Next-Key Locks (MySQL InnoDB)
In MySQL InnoDB (Repeatable Read level), phantom reads are eliminated using specialized index-record locks:
- **Record Lock:** Locks the exact index record (e.g. lock row where `id = 10`).
- **Gap Lock:** Locks the **empty space between index records** (e.g. locks the open gap $(10, 20)$ so no concurrent transaction can insert a record with `id = 15`).
- **Next-Key Lock:** A Record Lock on the index entry **PLUS a Gap Lock on the gap immediately preceding it**:
  $$\text{Next-Key Lock on } 20 = \text{Gap Lock on } (10, 20) + \text{Record Lock on } 20$$

---

# 5. Deadlock Detection & Prevention Strategies

A **Deadlock** occurs when two or more transactions form a circular dependency, each waiting for a lock held by the other.

```
Circular Wait Deadlock:
[ Transaction 1 ] --- (Holds Lock on Row A, Waiting for Row B) ---> [ Transaction 2 ]
        ^                                                                   |
        +------------- (Holds Lock on Row B, Waiting for Row A) <-----------+
```

---

### A. Deadlock Detection: Wait-For Graph (WFG)
- The database maintains a directed **Wait-For Graph** where vertices are active transactions and directed edges $T_1 \to T_2$ indicate $T_1$ is waiting for a lock held by $T_2$.
- A background daemon periodically runs **Cycle Detection (DFS / Tarjan's)** on the graph.
- If a cycle is detected, the database selects a **Victim Transaction** (the one with the lowest cost/fewest updates) and aborts/rolls it back.

---

### B. Deadlock Prevention: Timestamp Ordering Schemes
Each transaction is assigned a unique monotonic timestamp $TS(T_i)$ when it starts (older transactions have smaller timestamps):

1. **Wait-Die Scheme (Non-Preemptive):**
   - If older transaction requests a lock held by younger transaction $\implies$ **Older WAITS** ($TS(T_{\text{old}}) < TS(T_{\text{young}})$).
   - If younger transaction requests a lock held by older transaction $\implies$ **Younger DIES (Aborts & restarts)**.
2. **Wound-Wait Scheme (Preemptive - Lower abort rate):**
   - If older transaction requests a lock held by younger transaction $\implies$ **Older WOUNDS younger** (younger is preempted and aborted).
   - If younger transaction requests a lock held by older transaction $\implies$ **Younger WAITS**.

---

# 6. Storage Engines, WAL & Crash Recovery (ARIES)

---

### A. Write-Ahead Logging (WAL) & Buffer Pool Policies
Databases maintain an in-memory **Buffer Pool** of 8KB table data pages. To balance durability with high write throughput, database storage engines enforce two fundamental buffer management policies:

```
Buffer Management Matrix:
+-----------------------------------------------------------------------------------------+
| POLICY          | DEFINITION                                                            |
+-----------------------------------------------------------------------------------------+
| NO-FORCE Policy | Committed transaction pages do NOT need to be flushed to table data   |
|                 | files on disk immediately (avoids slow random disk writes).           |
|                 | Requirement: REDO LOG in WAL is mandatory to replay committed pages   |
|                 | after a power failure!                                                |
+-----------------------------------------------------------------------------------------+
| STEAL Policy    | Uncommitted dirty pages CAN overwrite clean pages on disk if the      |
|                 | Buffer Pool runs out of RAM.                                          |
|                 | Requirement: UNDO LOG in WAL is mandatory to roll back uncommitted    |
|                 | changes after a crash!                                                |
+-----------------------------------------------------------------------------------------+
```

- **The Golden WAL Rule:** A dirty data page can **NEVER be written to disk** until its corresponding WAL log record containing the change has been **flushed to disk (`fsync`)**.

---

### B. Checkpointing & Fuzzy Checkpoints
If the database only relied on WAL, recovering after a crash would require replaying years of WAL logs from the beginning of time.
- **Checkpointing:** Periodically flushes all dirty memory buffer pages to disk and writes a `<CHECKPOINT>` record into the WAL stream.
- **Fuzzy Checkpoint:** To avoid freezing active queries during a checkpoint, the engine writes a checkpoint start marker, flushes dirty pages in background threads asynchronously, and writes a checkpoint end marker along with an active transaction table. Crash recovery only needs to scan WAL records starting from the latest checkpoint!

---

### C. The ARIES Recovery Algorithm (3 Phases)
When a database recovers from a sudden crash or power loss, it executes the **ARIES (Algorithm for Recovery and Isolation Exploiting Semantics)** protocol:

```
ARIES Crash Recovery Pipeline:

       [ Crash Point ]
             |
             v
1. [ ANALYSIS PHASE ] (Scans WAL FORWARD from last Checkpoint to Crash Point)
   - Reconstructs internal state: Active Transaction Table + Dirty Page Table.
             |
             v
2. [ REDO PHASE ] (Scans WAL FORWARD from oldest unwritten dirty page)
   - Replays ALL changes (both committed and uncommitted transactions) to bring
     the database to the exact physical state at the instant of the crash ("Repeating History").
             |
             v
3. [ UNDO PHASE ] (Scans WAL BACKWARD from Crash Point)
   - Reverses all changes made by transactions that were ACTIVE (uncommitted) at the crash,
     writing Compensation Log Records (CLRs) to ensure idempotency if another crash occurs.
```

---

# 7. High-Yield Interview Questions & Defense

### Q1: What is the difference between Optimistic Concurrency Control (OCC) and 2-Phase Locking (2PL)?
> **Answer:** 2PL is pessimistic: it acquires locks *before* accessing data and blocks concurrent writers, making it ideal for high-contention write-heavy workloads. OCC is lock-free: transactions execute in private local workspaces, validating read/write timestamps only at commit time. OCC outperforms 2PL on read-heavy or low-contention systems, but suffers high abort rates under heavy write contention.

### Q2: How does PostgreSQL implement MVCC compared to MySQL InnoDB?
> **Answer:** 
> - **PostgreSQL:** Appends new row versions directly into the main table heap file (`xmin`/`xmax`). Requires background `VACUUM` to clean up dead tuples, causing table bloat if long-running transactions hold open snapshots.
> - **MySQL InnoDB:** Updates rows in-place in the Clustered Index and writes older row versions into a separate **Undo Tablespace (Rollback Segment)**. Traversing the undo chain reconstructs older snapshots without bloating the primary table.

### Q3: Why does `SELECT ... FOR UPDATE` solve the Lost Update anomaly?
> **Answer:** A standard `SELECT` acquires no persistent locks under Read Committed / MVCC. Two transactions can read $100 concurrently and write back $150 and $120, overwriting each other. `SELECT ... FOR UPDATE` forces the transaction to acquire an **Exclusive ($X$) row lock**, forcing the second transaction to block until the first commits.

### Q4: What is the Write Skew anomaly and how do you prevent it?
> **Answer:** Write Skew occurs under Snapshot Isolation when two concurrent transactions read overlapping data, evaluate a shared constraint, and update *disjoint* rows (e.g. doctors on call withdrawing simultaneously). Because they modify different rows, row-level locks don't conflict. It is prevented by promoting the isolation level to **Serializable** or using explicit range locks (`SELECT ... FOR UPDATE`).

### Q5: What is a Phantom Read and why does standard row locking fail to prevent it?
> **Answer:** A Phantom Read occurs when a transaction queries a range (e.g. `WHERE age > 21`), and a concurrent transaction inserts a *new* row matching the predicate. Row locks cannot prevent this because the new row did not exist when locks were acquired. It is prevented using **Gap Locks / Next-Key Locks** in InnoDB or Predicate Locks in Serializable isolation.

---

# 8. Quick Recap

- **Atomicity vs. Isolation:** Atomicity is all-or-nothing rollback (Undo Log); Isolation is hiding in-flight work from concurrent observers (Locks/MVCC).
- **MVCC Rule:** Readers never block writers, and writers never block readers.
- **Anomalies Hierarchy:** Dirty Read (uncommitted data) $\to$ Non-Repeatable Read (modified data) $\to$ Phantom Read (new inserted rows) $\to$ Write Skew (disjoint constraint violation).
- **Strict 2PL:** Releases Exclusive ($X$) locks only at `COMMIT`/`ROLLBACK` to eliminate cascading rollbacks.
- **Intent Locks ($IS$/$IX$):** Placed on table nodes to allow $O(1)$ table-level lock conflict checks without row scans.
- **Next-Key Lock:** Record Lock on index entry + Gap Lock on the preceding gap (eliminates Phantom Reads in MySQL InnoDB).
- **WAL & ARIES:** WAL forces log writes before dirty page flushes (`fsync`); ARIES recovery executes in 3 phases: **Analysis $\to$ Redo (Repeating History) $\to$ Undo (Active rollbacks)**.
