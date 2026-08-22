# 01: Transactions, Concurrency Control & Crash Recovery

> **Target:** Verbal Technical Interviews at Top Tech Companies (FAANG, Uber, Stripe, Atlassian, Microsoft, Google).  
> **Focus:** Spoken definitions, core engine mechanics, trade-offs, and tricky follow-up questions. Zero fluff.

---

## 1. ACID Properties (Spoken Deep Dive)

```
+-----------------------------------------------------------------------------------------+
| PROPERTY        | 30-SECOND VERBAL EXPLANATION         | ENGINE IMPLEMENTATION PRIMITIVE|
+-----------------------------------------------------------------------------------------+
| Atomicity (A)   | "All or nothing" execution. If a     | Undo Logs (WAL / Rollback Seg) |
|                 | transaction crashes halfway, all its |                                |
|                 | partial changes are rolled back.     |                                |
+-----------------------------------------------------------------------------------------+
| Consistency (C) | Preserves database invariants and    | Application Logic, Foreign     |
|                 | business rules from one valid state  | Keys, Unique/Check Constraints |
|                 | to another (e.g. total balance >= 0).|                                |
+-----------------------------------------------------------------------------------------+
| Isolation (I)   | Concurrent transactions execute      | 2-Phase Locking (2PL), MVCC    |
|                 | without seeing each other's          | (Snapshot Isolation)           |
|                 | intermediate, in-flight state.       |                                |
+-----------------------------------------------------------------------------------------+
| Durability (D)  | Once a commit is acknowledged, data  | Redo Logs (WAL `fsync` to disk)|
|                 | survives power outages and crashes.  | and Checkpointing              |
+-----------------------------------------------------------------------------------------+
```

### Verbal Interview Traps & Follow-ups:
- **Atomicity vs. Isolation:**
  - *Atomicity* deals with a **single transaction's** failure (rollback via Undo log).
  - *Isolation* deals with **concurrency between multiple transactions** (hiding in-flight changes).
  - *Trap:* A system can have 100% Atomicity with 0% Isolation (e.g., Read Uncommitted).
- **"C" in ACID vs. "C" in CAP:**
  - *ACID Consistency:* Application-level invariants (e.g., Money transferred between A and B sums to constant).
  - *CAP Consistency:* Distributed Linearizability (every read returns the latest global write).
- **How Durability works without killing performance:**
  - Flushing table pages to disk on every commit is too slow (random I/O). The database instead flushes an append-only sequential **WAL (Write-Ahead Log)** record using `fsync()` and batches multiple commits via **Group Commit**.

---

## 2. Transaction Isolation Levels & Concurrency Anomalies

### The 4 ANSI Isolation Levels & Anomalies Matrix

```
+-----------------------------------------------------------------------------------------+
| ISOLATION LEVEL      | DIRTY READ        | NON-REPEATABLE READ | PHANTOM READ           |
+-----------------------------------------------------------------------------------------+
| Read Uncommitted     | YES (Allowed)     | YES (Allowed)       | YES (Allowed)          |
| Read Committed       | NO (Prevented)    | YES (Allowed)       | YES (Allowed)          |
| Repeatable Read      | NO (Prevented)    | NO (Prevented)      | YES (Allowed in ANSI)* |
| Serializable         | NO (Prevented)    | NO (Prevented)      | NO (Prevented)         |
+-----------------------------------------------------------------------------------------+
* Note: Modern engines (PostgreSQL MVCC & MySQL InnoDB Next-Key Locks) also prevent Phantom Reads in Repeatable Read!
```

### 5 Concurrency Anomalies (Spoken Examples)

1. **Dirty Read:** Txn A updates row X without committing. Txn B reads row X. Txn A rolls back. Txn B operated on data that never officially existed.
2. **Non-Repeatable Read (Fuzzy Read):** Txn A reads row X ($50). Txn B updates row X to $80 and commits. Txn A reads row X again and sees $80 (two different values for the same row in one transaction).
3. **Phantom Read:** Txn A queries a range (`SELECT COUNT(*) WHERE age >= 21` $\to$ 10 rows). Txn B inserts a *new* row with `age = 25` and commits. Txn A re-runs the range query and gets 11 rows.
4. **Lost Update:** Txn A and Txn B both read Balance = $100. Txn A writes $150. Txn B writes $120, overwriting Txn A's deposit. *Fix: `SELECT ... FOR UPDATE` or atomic `UPDATE balance = balance + 50`.*
5. **Write Skew (Snapshot Isolation Anomaly):** Invariant: `Checking + Savings >= 0`. Current: $100 in each. Txn A reads both, withdraws $150 from Checking (New sum: $50 $\ge$ 0 $\to$ OK). Txn B concurrently reads both, withdraws $150 from Savings (New sum: $50 $\ge$ 0 $\to$ OK). Both commit under Snapshot Isolation because they modified disjoint rows, but total balance is now -$100 (Invariant violated!). *Fix: Serializable isolation or explicit row locks.*

---

## 3. Concurrency Control: 2PL vs. OCC vs. MVCC

```
+-----------------------------------------------------------------------------------------+
| PARADIGM            | CORE PHILOSOPHY                        | PROS & CONS              |
+-----------------------------------------------------------------------------------------+
| Pessimistic 2PL     | Assume conflicts are frequent. Acquire | Pros: No aborts/rollbacks|
|                     | locks before reading/writing.          | Cons: High lock overhead,|
|                     |                                        | deadlocks, high latency. |
+-----------------------------------------------------------------------------------------+
| Optimistic (OCC)    | Assume conflicts are rare. Execute in  | Pros: Zero lock overhead.|
|                     | private memory; validate at commit.    | Cons: High abort rate on |
|                     | Rollback if conflict detected.         | write-heavy contention.  |
+-----------------------------------------------------------------------------------------+
| MVCC                | Industry Standard. Updates create new  | Pros: Readers NEVER      |
|                     | row versions instead of in-place.      | block writers; writers   |
|                     | Older transactions see older snapshot. | NEVER block readers.     |
+-----------------------------------------------------------------------------------------+
```

### MVCC Engine Comparison: PostgreSQL vs. MySQL InnoDB

- **PostgreSQL (Append-Only in Table Heap):**
  - Inserts new row versions directly into the main table with `xmin` (creator txid) and `xmax` (deleter/updater txid).
  - *Trade-off:* Requires background `VACUUM` to clean up dead tuples; long transactions cause table bloat.
- **MySQL InnoDB (In-Place + Undo Log / Rollback Segment):**
  - Updates the clustered index row in-place and writes previous versions into a separate **Undo Log chain**.
  - *Trade-off:* No table bloat, but long transactions create huge Undo Tablespaces and slow historical snapshot traversals.

---

## 4. Locking Mechanics & Hierarchies

### A. Lock Types
- **Shared Lock ($S$):** Read lock. Multiple transactions can hold $S$ locks simultaneously. Blocks $X$ locks.
- **Exclusive Lock ($X$):** Write lock (`FOR UPDATE`, `UPDATE`, `DELETE`). Only one transaction can hold it; blocks all other $S$ and $X$ requests.

### B. Two-Phase Locking (2PL)
- **Growing Phase:** Locks are acquired; zero locks released.
- **Shrinking Phase:** Locks are released; zero locks acquired.
- **Strict 2PL (S2PL):** Holds all **Exclusive ($X$) locks** until `COMMIT`/`ROLLBACK` (prevents cascading rollbacks).
- **Rigorous 2PL (SS2PL):** Holds **all $S$ and $X$ locks** until `COMMIT`/`ROLLBACK` (guarantees serializability).

### C. Intent Locks ($IS$ / $IX$)
- **Purpose:** Placed on parent levels (Table/Page) before acquiring fine-grained row locks.
- **Why it matters:** When a query requests `LOCK TABLE orders WRITE`, the engine checks the table's Intent Exclusive ($IX$) flag in $O(1)$ time without scanning millions of individual rows.

### D. Gap Locks & Next-Key Locks (MySQL InnoDB)
- **Record Lock:** Locks the exact index row (e.g., `id = 10`).
- **Gap Lock:** Locks the empty space *between* index records (e.g., open interval $(10, 20)$) so concurrent transactions cannot insert `id = 15`.
- **Next-Key Lock:** Record Lock + Gap Lock on preceding gap. **Eliminates Phantom Reads in InnoDB Repeatable Read**.

---

## 5. Deadlocks: Detection vs. Prevention

A **Deadlock** is a circular wait where Txn 1 holds resource A waiting for B, and Txn 2 holds resource B waiting for A.

### Detection: Wait-For Graph (WFG)
- The engine builds a directed graph where nodes are transactions and edges $T_1 \to T_2$ mean $T_1$ is waiting for $T_2$.
- A background thread runs **Cycle Detection (DFS)** periodically.
- On cycle detection, the engine selects a **Victim Transaction** (fewest updates / lowest cost) and rolls it back.

### Prevention: Timestamp Ordering Schemes
Transactions are assigned unique timestamps $TS(T)$ when they start ($T_{\text{old}} < T_{\text{young}}$):
1. **Wait-Die (Non-preemptive):** Older waits for younger; younger dies (aborts) if requesting older's lock.
2. **Wound-Wait (Preemptive - Lower latency):** Older wounds (preempts/aborts) younger; younger waits for older.

---

## 6. Storage Engines, WAL & Crash Recovery (ARIES)

### Buffer Pool Policies & WAL
- **NO-FORCE Policy:** The DB does *not* flush committed dirty pages to disk immediately (avoids random I/O). $\to$ Requires a **Redo Log** to replay committed changes after a crash.
- **STEAL Policy:** The DB *can* write uncommitted dirty pages to disk if RAM is full. $\to$ Requires an **Undo Log** to roll back uncommitted changes after a crash.
- **The Golden WAL Rule:** A dirty data page can **NEVER be written to disk** until its corresponding WAL log record has been flushed to disk (`fsync`).

### ARIES Recovery Protocol (3 Phases)
1. **Analysis Phase:** Scans WAL *forward* from the latest checkpoint to find all active transactions and dirty pages at the time of the crash.
2. **Redo Phase ("Repeating History"):** Scans WAL *forward* from the oldest unwritten dirty page, replaying ALL changes (committed and uncommitted) to restore the exact crash-moment physical state.
3. **Undo Phase:** Scans WAL *backward* from the crash point, rolling back all transactions that were active (uncommitted) at the crash, writing Compensation Log Records (CLRs) for idempotency.

---

## 7. High-Yield Verbal Interview Q&A

### Q1: "How does MVCC allow non-blocking reads?"
> **Spoken Answer:** In MVCC, updates and deletes do not overwrite data in-place; they create a new timestamped version of the row. When a transaction reads, it receives a consistent snapshot corresponding to its start timestamp. Readers see older committed versions without acquiring locks, so readers never block writers and writers never block readers.

### Q2: "Why is 2-Phase Locking (2PL) different from 2-Phase Commit (2PC)?"
> **Spoken Answer:** 2PL is a single-node **concurrency control** protocol with a growing and shrinking lock phase to ensure serializability. 2PC is a distributed **consensus/atomic commit** protocol (Prepare and Commit phases) that ensures multiple separate database nodes all commit or abort together.

### Q3: "What is Write Skew and why does Repeatable Read not catch it?"
> **Spoken Answer:** Write Skew occurs when two transactions concurrently read overlapping data, evaluate a shared business constraint, but update *disjoint* rows. Because they modify different rows, row-level locks and snapshot isolation detect no conflict, but the global invariant is broken. It requires Serializable isolation or explicit locking (`SELECT ... FOR UPDATE`).

### Q4: "What is the difference between Write-Ahead Logging (WAL) and Checkpointing?"
> **Spoken Answer:** WAL provides durability by appending transaction modifications sequentially to disk before data pages are modified in table files. Checkpointing periodically flushes all dirty memory buffer pages to disk and writes a checkpoint marker into the WAL, bounding crash recovery time so the engine doesn't have to replay the entire WAL from the beginning of time.
