# DBMS — GS OA MCQ Revision Notes

---

## 1. Concurrency Anomalies & Isolation Levels (Most Tested)

When multiple transactions run concurrently, these anomalies can occur:

*   **Dirty Read:** Transaction A reads data modified by Transaction B *before* Transaction B commits. If B rolls back, A has read invalid data.
*   **Non-Repeatable Read:** Transaction A reads a row, Transaction B updates/deletes that row and commits. Transaction A re-reads the row and gets different data.
*   **Phantom Read:** Transaction A runs a query finding rows matching a condition. Transaction B inserts new matching rows and commits. Transaction A re-runs the query and gets a different set of rows.

### Transaction Isolation Levels (ANSI SQL Standard)

| Isolation Level | Dirty Read | Non-Repeatable Read | Phantom Read |
| :--- | :--- | :--- | :--- |
| **Read Uncommitted** | Allowed | Allowed | Allowed |
| **Read Committed** | Prevented | Allowed | Allowed |
| **Repeatable Read** | Prevented | Prevented | Allowed |
| **Serializable** | Prevented | Prevented | Prevented |

---

## 2. Lock Types & Concurrency Control

To prevent anomalies, databases use locks:

### Shared vs. Exclusive Locks
*   **Shared Lock (S-Lock / Read Lock):** Multiple transactions can hold a shared lock on the same data simultaneously. It allows reading but prevents writing.
*   **Exclusive Lock (X-Lock / Write Lock):** Only one transaction can hold an exclusive lock. No other transaction can read or write to this data.

### Two-Phase Locking (2PL) Protocol
Ensures serializability of concurrent transactions. Divided into two phases:
1.  **Growing Phase:** A transaction may acquire locks but cannot release any.
2.  **Shrinking Phase:** A transaction may release locks but cannot acquire new ones.
*   *Note:* 2PL prevents non-serializable runs, but **does not prevent deadlocks**.
*   **Strict 2PL:** A variation where all Exclusive (X) locks held by a transaction must be released only *after* the transaction commits or aborts. This prevents cascading rollbacks.

---

## 3. Indexing Structures: B-Tree vs. B+ Tree

*   **B-Tree Index:**
    *   Stores keys, data pointers, and child pointers in **both** internal nodes and leaf nodes.
    *   *Drawback:* Internal nodes are larger, meaning fewer keys fit in a single memory block, increasing disk I/O.
*   **B+ Tree Index (Default in databases like MySQL InnoDB):**
    *   Stores actual data pointers **only in the leaf nodes**. Internal nodes only store keys and child pointers (acting strictly as a router).
    *   Leaf nodes are linked in a **doubly-linked list**, making range queries (`SELECT WHERE key BETWEEN A AND B`) extremely fast.
*   **Hash Index:** Uses a hash table. Fast $O(1)$ for exact lookups (`=`). **Cannot** perform range queries.

### Clustered vs. Non-Clustered Indexes

*   **Clustered Index:**
    *   Determines the physical order of data rows on the disk.
    *   Only **one** clustered index is allowed per table (usually the Primary Key).
    *   Leaf nodes contain the actual data rows.
*   **Non-Clustered Index:**
    *   Does not alter the physical layout of the table. It maintains a separate structure pointing to the physical data location.
    *   Multiple non-clustered indexes are allowed per table.
    *   Leaf nodes contain pointers (or primary keys) to the data rows.

---

## 4. Normalization Forms

*   **1NF:** All attributes contain atomic (single-valued) values. No repeating groups.
*   **2NF:** Must be in 1NF. No **partial dependency** (no non-key attribute can depend on a subset of a composite candidate key).
*   **3NF:** Must be in 2NF. No **transitive dependency** (non-key attributes cannot depend on other non-key attributes).
*   **BCNF (Boyce-Codd NF):** Stronger than 3NF. For every functional dependency `X -> Y`, `X` must be a super key.

---

## 5. Database Keys

*   **Candidate Key:** Minimal set of attributes that uniquely identifies a row.
*   **Primary Key:** Chosen candidate key. Unique + Not Null. Only one per table.
*   **Super Key:** Any set of attributes that uniquely identifies a row (superset of a candidate key).
*   **Foreign Key:** Field that references a Primary Key in another table, ensuring **referential integrity**.

---

## 6. SQL Join Cheatsheet

*   `INNER JOIN`: Returns rows when there is a match in both tables.
*   `LEFT JOIN`: All rows from the left table + matched rows from the right table (NULL if no match).
*   `RIGHT JOIN`: All rows from the right table + matched rows from the left table (NULL if no match).
*   `SELF JOIN`: Joining a table with itself (useful for tree/hierarchy representation, like matching employees to manager IDs).
