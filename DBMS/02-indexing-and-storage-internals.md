# 02: Indexing & Storage Internals

> **Target:** Top Product & FinTech Technical Interviews (FAANG, Uber, Stripe, Atlassian, Citadel).  
> **Style:** High-ROI revision sheet. Focuses on low-level disk block layouts, B+ Tree physics, composite key ordering, covering indexes, and B+ Trees vs. LSM-Trees.

---

# Table of Contents
1. [Clustered vs. Non-Clustered Indexes](#1-clustered-vs-non-clustered-indexes)
2. [B-Trees vs. B+ Trees (Storage Engine Architecture)](#2-b-trees-vs-b-trees-storage-engine-architecture)
3. [Composite Indexes & The Leftmost Prefix Rule](#3-composite-indexes--the-leftmost-prefix-rule)
4. [Covering Indexes & Index-Only Scans](#4-covering-indexes--index-only-scans)
5. [Specialized Index Types (GIN, GiST, Partial, Expression)](#5-specialized-index-types)
6. [LSM-Trees (Log-Structured Merge-Trees) vs. B+ Trees](#6-lsm-trees-vs-b-trees)
7. [High-Yield Interview Questions & Defense](#7-high-yield-interview-questions--defense)
8. [Quick Recap](#8-quick-recap)

---

# 1. Clustered vs. Non-Clustered Indexes

An **Index** is an auxiliary data structure (typically a B+ Tree) that enables the database engine to locate specific rows in $O(\log N)$ disk reads without performing an expensive $O(N)$ Sequential Table Scan.

```
Clustered Index (InnoDB - Table IS the Index):     Non-Clustered Index (Secondary Index):
                [ Root Node ]                                    [ Root Node ]
               /             \                                  /             \
       [ Leaf 1 ]           [ Leaf 2 ]                  [ Leaf 1 ]           [ Leaf 2 ]
       (Contains FULL       (Contains FULL              (Contains Search     (Contains Search
        ROW DATA on Disk)    ROW DATA on Disk)           Key + Primary Key)   Key + Primary Key)
```

---

### Comparison Matrix

```
+-----------------------------------------------------------------------------------------+
| DIMENSION           | CLUSTERED INDEX (Index-Organized Table) | NON-CLUSTERED (SECONDARY) INDEX  |
+-----------------------------------------------------------------------------------------+
| Physical Row Order  | Dictates the PHYSICAL on-disk storage   | Stored in a separate B+ tree;   |
|                     | order of table rows.                    | physical data rows are un-moved.|
| Maximum per Table   | Exactly 1 per table.                    | Multiple (typically 5–10 max).  |
| Leaf Node Content   | Contains the **ENTIRE ACTUAL ROW DATA** | Contains **Search Key + Pointer |
|                     | (all columns).                          | to Primary Key / Tuple ID**.    |
| MySQL InnoDB Engine | Automatically built on **PRIMARY KEY**. | Stores Search Key + Primary Key.|
| PostgreSQL Engine   | Uses Heap Files for raw rows; all       | Points to physical Heap Tuple   |
|                     | indexes (including PK) are secondary!   | ID: `ctid (page_no, offset)`.   |
+-----------------------------------------------------------------------------------------+
```

---

### The InnoDB "Double Lookup" (Bookmark Lookup) Penalty
In MySQL InnoDB:
1. When you query by a secondary index: `SELECT name, age, email FROM users WHERE email = 'aman@...';`
2. **Step 1:** The engine searches the secondary index B+ tree on `email` to find the matching entry. The leaf node returns the user's **Primary Key (`id = 42`)**.
3. **Step 2:** Because `name` and `age` are not stored in the secondary index, the engine must perform a second search on the **Clustered Index B+ tree** using `id = 42` to fetch the actual row data (**Double Lookup / Bookmark Lookup**).

```
Double Lookup Pipeline:
[ Query: WHERE email = '...' ] ---> [ Secondary Index on Email ] ---> Returns PK: id = 42
                                                                             |
                                    [ Clustered Index on PK ] <---------------+
                                             |
                                             v
                                    Returns FULL ROW DATA: { id: 42, name: 'Aman', age: 22 }
```

---

# 2. B-Trees vs. B+ Trees (Storage Engine Architecture)

Relational database storage engines (PostgreSQL, MySQL InnoDB, SQLite) store indexes exclusively using **B+ Trees** rather than Binary Search Trees (BSTs) or standard B-Trees.

```
Binary Search Tree (RAM Optimized):           B+ Tree Node (8KB Disk Page Block Optimized):
           [ 50 ]                             +--------------------------------------------+
          /      \                            | Key 10 | Key 50 | Key 100 | Key 500 | ...  | (Order M = 100+)
       [ 20 ]  [ 80 ]                         +--------------------------------------------+
                                              /        |        |         |         \
 (Height H = 30 -> 30 Random Disk Seeks!)    [Page 1] [Page 2] [Page 3]  [Page 4]   [Page 5]
                                             (Height H = 3 -> Exactly 3 Disk Page Reads!)
```

---

### Why Databases Use B+ Trees Over Binary Search Trees
1. **Disk Block Alignment:** Disks read and write in fixed **4KB or 8KB Page Blocks**. A standard binary tree node contains only 1 key and 2 child pointers (~24 bytes), wasting **99.7% of an 8KB disk block read**. A B+ Tree node has a massive branching factor $M$ ($M \ge 100$), packing hundreds of keys into a single 8KB disk page.
2. **Shallow Tree Height:** For a table with 1 billion rows ($N = 10^9$):
   - In a balanced BST: Height $H = \log_2(10^9) \approx 30$ levels $\implies$ **30 sequential disk seeks** (~300ms on rotational disk / ~3ms on SSD).
   - In a B+ Tree with $M = 500$: Height $H = \log_{500}(10^9) \approx 3$ levels $\implies$ **Only 3 disk reads** (<0.5ms). The root and internal nodes are permanently pinned in the RAM Buffer Pool, so searches require **only 1 physical disk read**!

---

### Standard B-Tree vs. Database B+ Tree

```
+-----------------------------------------------------------------------------------------+
| DIMENSION           | STANDARD B-TREE                         | DATABASE B+ TREE                |
+-----------------------------------------------------------------------------------------+
| Data Record Storage | Key AND full row pointer stored inside  | Internal nodes store **ONLY     |
|                     | **ALL internal and leaf nodes**.        | ROUTING KEYS & CHILD POINTERS**.|
|                     |                                         | Data stored **ONLY in Leaves**. |
| Fan-Out & Height    | Lower fan-out (payload takes up page    | **Massive fan-out** (internal   |
|                     | space) -> Taller tree height.           | pages hold maximum routing keys)|
| Range Query Speed   | Slow. Must perform recursive in-order   | **Blazing Fast $O(1)$ Range     |
|                     | tree traversals across levels.          | Scans**: All leaf nodes form a  |
|                     |                                         | **Doubly Linked List**!         |
+-----------------------------------------------------------------------------------------+
```

```
B+ Tree Doubly Linked Leaf Range Scan:
Query: SELECT * FROM users WHERE age BETWEEN 21 AND 25;

1. Traverse tree once to locate first leaf containing age = 21 (3 disk reads).
2. Sequentially walk the leaf-level linked list pointers directly on disk until age > 25:
   [ Leaf Page 1: 18, 19, 21 ] <---> [ Leaf Page 2: 22, 23, 24 ] <---> [ Leaf Page 3: 25, 26, 30 ]
                                     |----------------- Sequential Disk Read -----------------|
```

---

# 3. Composite Indexes & The Leftmost Prefix Rule

A **Composite Index (Multi-Column Index)** is an index constructed on two or more columns: `CREATE INDEX idx_user ON users (department_id, last_name, first_name);`

```
Composite Key Sorting Hierarchy:
The B+ tree sorts tuples strictly lexicographically:
First by department_id ASC.
  -> If department_id matches, sort by last_name ASC.
       -> If last_name matches, sort by first_name ASC.
```

---

### The Leftmost Prefix Rule
A query can utilize a composite index **only if the query filters match columns starting from the leftmost column sequentially with NO GAPS**:

```
Given Index on (A, B, C):

Query Filter                       | Index Used? | Mechanical Reason
---------------------------------------------------------------------------------------------------
WHERE A = 5                        | YES (Full)  | Leftmost prefix column A is present.
WHERE A = 5 AND B = 10             | YES (Full)  | Leftmost prefix (A, B) is present.
WHERE A = 5 AND B = 10 AND C = 20  | YES (Full)  | Complete composite key (A, B, C) is present.
WHERE B = 10 AND C = 20            | NO (Scan)   | FAILS Leftmost rule! A is missing. Tree cannot be searched.
WHERE C = 20                       | NO (Scan)   | FAILS Leftmost rule! A and B are missing.
WHERE A = 5 AND C = 20             | PARTIAL     | Index used for A only; C filtered via row scan (Gap at B).
```

---

### The Range Column Trap in Composite Indexes
When a composite index encounters a **Range Comparison (`<`, `>`, `BETWEEN`, `LIKE 'abc%'`)**, the index can be used for that range column, but **CANNOT be used to filter subsequent columns**:

```sql
-- Index on: (status, created_at, user_id)
SELECT * FROM orders 
WHERE status = 'PAID'          -- 1. Exact match: Index utilized!
  AND created_at >= '2026-01-01'-- 2. Range match: Index utilized!
  AND user_id = 1042;          -- 3. TRAP: Index CANNOT be used for user_id! 
                               -- (Rows with created_at >= '2026-01-01' are not sorted by user_id).
```

- **Rule of Thumb for Composite Index Column Ordering:**
  $$\mathbf{[ \text{Equality Columns } (=) ] \implies [ \text{Sorting Columns } (\text{ORDER BY}) ] \implies [ \text{Range Columns } (<, >) ]}$$

---

# 4. Covering Indexes & Index-Only Scans

A **Covering Index** is a secondary index that contains **all the columns requested by a query** (in both `SELECT`, `WHERE`, `JOIN`, and `ORDER BY` clauses).

```
Standard Query vs. Index-Only Scan:

Standard Secondary Index Lookup:
[ Query: SELECT name FROM users WHERE email = 'aman@...'; ]
  1. Search Secondary Index on (email) ---> Finds PK: 42
  2. Table Heap Fetch / Clustered Index ---> Reads Disk Page to get 'name' (Extra I/O!)

Covering Index on (email, name):
[ Query: SELECT name FROM users WHERE email = 'aman@...'; ]
  1. Search Covering Index on (email, name) ---> Returns 'name' directly from B+ Tree Leaf!
  --> Table Heap Fetch is COMPLETELY ELIMINATED! (100% Index-Only Scan in EXPLAIN ANALYZE).
```

---

### PostgreSQL `INCLUDE` Clause (Index-Only Scans without Key Overhead)
- Adding extra columns to an index key increases B+ tree internal routing size.
- PostgreSQL provides the **`INCLUDE` clause**:
  `CREATE INDEX idx_users_email ON users (email) INCLUDE (name, age);`
- **Mechanics:** `email` is stored in internal routing pages as the search key; `name` and `age` are stored **only in the leaf nodes** as unindexed non-key payloads, enabling Index-Only Scans with minimal B+ tree branch overhead.

---

# 5. Specialized Index Types

```
+-----------------------------------------------------------------------------------------+
| INDEX TYPE          | IDEAL USE CASE                         | LIMITATIONS / MECHANICS  |
+-----------------------------------------------------------------------------------------+
| B+ Tree Index       | Range queries, Equality, Sorting       | Default index in all RDBMS|
| Hash Index          | Pure exact equality (`WHERE id = 5`)   | No range scans, no ORDER BY|
| GIN (Generalized    | Composite items: JSONB, Arrays,        | Maps internal tokens to  |
| Inverted Index)     | Full-Text Search documents             | row lists (Posting Lists)|
| GiST (Generalized   | Multi-dimensional geometric & spatial  | R-Tree bounding boxes     |
| Search Tree)        | data (PostGIS coordinates, polygons)   | (find nearest neighbors) |
| Partial / Filtered  | Skewed data distributions              | Tiny memory footprint;   |
| Index               | (`WHERE is_processed = false`)         | indexes subset of rows   |
| Expression /        | Querying computed function outputs     | Evaluates function at    |
| Functional Index    | (`WHERE LOWER(email) = 'aman@...'`)    | INSERT/UPDATE time       |
+-----------------------------------------------------------------------------------------+
```

```sql
-- Partial Index Example (Indexes only active unpaid invoices, ignoring 99% historical rows):
CREATE INDEX idx_unpaid_invoices ON invoices (customer_id) 
WHERE status = 'UNPAID';

-- Expression Index Example:
CREATE INDEX idx_lower_email ON users (LOWER(email));
```

---

# 6. LSM-Trees vs. B+ Trees

In modern big-data architectures, write throughput bottlenecks on traditional B+ trees led to the adoption of **Log-Structured Merge-Trees (LSM-Trees)** in storage engines like **RocksDB, Cassandra, LevelDB, and ClickHouse**.

```
LSM-Tree Storage Pipeline:

[ CLIENT WRITE ]
       |
       +---> 1. Append to Sequential WAL on Disk (Crash Durability)
       +---> 2. Insert into in-memory MemTable (SkipList / Red-Black Tree)
                    | (When MemTable reaches 64MB capacity)
                    v
             [ Flush to Immutable SSTable on Disk (Level 0) ]
                    |
                    v (Background Compaction Process)
             [ Level 1 SSTables ] ---> [ Level 2 SSTables ]
```

---

### Architectural Trade-Offs: B+ Tree vs. LSM-Tree

```
+-----------------------------------------------------------------------------------------+
| DIMENSION           | B+ TREE (PostgreSQL, MySQL, SQLite)    | LSM-TREE (RocksDB, Cassandra)   |
+-----------------------------------------------------------------------------------------+
| Primary Workload    | **Read-Heavy OLTP Queries**            | **Ultra-High Write Throughput** |
| Write Mechanism     | **In-Place Updates:** Overwrites 8KB   | **Append-Only Writes:** Appends |
|                     | disk pages (Random disk writes + WAL). | to memory MemTable & flushes.   |
| Write Amplification | **High** (modifying 1 row rewrites an  | **Low** (sequential batch disk  |
|                     | entire 8KB/16KB disk page).            | appends maximize SSD write life)|
| Read Amplification  | **Low & Deterministic** (at most 3–4   | **Higher** (must search MemTable|
|                     | disk page reads).                      | & multiple SSTables on disk).   |
| Read Optimization   | RAM Buffer Pool caching.               | **Bloom Filters** on SSTables to|
|                     |                                        | bypass nonexistent disk reads.  |
+-----------------------------------------------------------------------------------------+
```

---

# 7. High-Yield Interview Questions & Defense

### Q1: Why does adding an index speed up reads but slow down writes?
> **Answer:** Every `INSERT`, `UPDATE`, or `DELETE` must not only modify the table heap/clustered index, but must also traverse and update every secondary B+ tree index on the table, potentially triggering node splits ($O(\log N)$) and page rebalancing.

### Q2: What causes a B+ Tree node split and what is its time complexity?
> **Answer:** A node split occurs when an insertion attempts to write to a B+ tree page block that has exceeded its maximum capacity (e.g. 8KB). The engine allocates a new page, moves half the keys to the new page, and inserts a routing key into the parent node. It executes in $O(\log N)$ time and propagates up to the root if parent nodes are also full.

### Q3: Why is `WHERE status != 'ACTIVE'` unable to utilize a B+ tree index efficiently?
> **Answer:** B+ trees are structured for ordered equality and range seeks. Negative operators (`!=`, `NOT IN`, `NOT LIKE`) match the vast majority of the table; scanning a B+ tree for non-matching keys requires traversing almost all leaf pages, prompting the query optimizer to choose a faster Sequential Table Scan.

### Q4: What is the purpose of Bloom Filters in LSM-Tree databases like Cassandra or RocksDB?
> **Answer:** Because an LSM-tree distributes keys across multiple immutable SSTable files on disk, reading a missing key could force reading every SSTable file. A Bloom Filter resides in RAM for each SSTable; if the Bloom Filter returns `false`, the database knows with 100% mathematical certainty that the key is not in that SSTable, completely bypassing disk I/O.

### Q5: What is an Index Skip Scan?
> **Answer:** When a composite index exists on `(A, B)` and a query filters only on `WHERE B = 10` (missing leftmost column `A`), an Index Skip Scan allows the engine to skip through distinct values of `A` and search `B = 10` within each `A` subtree, avoiding a full table scan when column `A` has low cardinality (e.g. gender or status).

---

# 8. Quick Recap

- **Clustered vs. Secondary:** Clustered index stores full row data in leaf pages (1 per table); Secondary indexes store search keys pointing to Primary Keys (InnoDB) or Tuple IDs (Postgres).
- **B+ Tree Superiority:** Massive branching factor ($M > 100$) matches 8KB disk page reads, keeping tree height $H \le 3$ for $10^9$ rows; doubly linked leaf pages allow $O(1)$ sequential range scans.
- **Leftmost Prefix Rule:** Composite index `(A, B, C)` requires columns starting from `A` with no gaps; range comparisons on column $B$ prevent index utilization on subsequent column $C$.
- **Covering Index:** Satisfies query directly from B+ tree leaf nodes without performing table heap lookups (Index-Only Scan).
- **LSM-Tree vs. B+ Tree:** B+ Trees excel at low-latency reads via in-place updates; LSM-Trees excel at massive write throughput via append-only MemTables and SSTables.
- **Bloom Filters:** Space-efficient bitsets in LSM-trees that eliminate disk reads for non-existent keys with zero false negatives.
