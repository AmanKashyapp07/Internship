# 02: Indexing & Storage Internals

> **Target:** Verbal Technical Interviews at Top Tech Companies (FAANG, Uber, Stripe, Atlassian, Microsoft, Google).  
> **Focus:** Spoken explanations, B+ Tree physics, composite key ordering, storage engine mechanics, and high-frequency trade-off questions.

---

## 1. Clustered vs. Non-Clustered (Secondary) Indexes

```
+-----------------------------------------------------------------------------------------+
| DIMENSION           | CLUSTERED INDEX (Index-Organized)       | SECONDARY (NON-CLUSTERED) INDEX |
+-----------------------------------------------------------------------------------------+
| Physical Row Order  | Dictates physical on-disk storage order.| Separate B+ Tree; physical data |
|                     | Exactly **1 per table**.                | rows remain unchanged. Multiple.|
+-----------------------------------------------------------------------------------------+
| Leaf Node Content   | Stores the **FULL ROW DATA** (all cols).| Stores **Search Key + Pointer** |
|                     |                                         | (PK in InnoDB; Tuple ID in PG). |
+-----------------------------------------------------------------------------------------+
| Primary Key Behavior| In MySQL InnoDB, PK IS the Clustered    | All non-PK indexes are secondary|
|                     | Index. In Postgres, all indexes are     | indexes.                        |
|                     | secondary over Heap Files.              |                                 |
+-----------------------------------------------------------------------------------------+
```

### The InnoDB "Double Lookup" (Bookmark Lookup) Penalty
1. When querying via a secondary index: `SELECT name, age FROM users WHERE email = 'aman@...';`
2. **Step 1:** The engine searches the secondary index B+ Tree on `email` to find the matching leaf node, which returns the Primary Key (`id = 42`).
3. **Step 2:** Because `name` and `age` are not stored in the secondary index, the engine must perform a second search on the **Clustered Index B+ Tree** using `id = 42` to retrieve the actual row data (**Bookmark Lookup**).
4. *How to eliminate it:* Create a **Covering Index** on `(email, name, age)`.

---

## 2. Why Databases Use B+ Trees

```
+-----------------------------------------------------------------------------------------+
| DATA STRUCTURE      | WHY IT FAILS / SUCCEEDS AS A DISK STORAGE ENGINE INDEX            |
+-----------------------------------------------------------------------------------------+
| Binary Search Tree  | **Fails on Disk:** Tiny node size (~24 bytes) wastes 99.7% of an  |
| (BST / Red-Black)   | 8KB disk page. Height is ~30 for 1B rows = 30 random disk seeks!  |
+-----------------------------------------------------------------------------------------+
| Standard B-Tree     | **Suboptimal for Range Scans:** Stores data payloads in internal  |
|                     | routing nodes, reducing fan-out. Range queries require slow,      |
|                     | multi-level in-order recursive tree traversals.                   |
+-----------------------------------------------------------------------------------------+
| Database B+ Tree    | **Optimal:** Internal nodes store ONLY routing keys (huge fan-out |
|                     | $M > 100$, tree height $\le 3$). Leaf nodes store data and form a |
|                     | **doubly linked list** for $O(1)$ sequential range scans.         |
+-----------------------------------------------------------------------------------------+
| Hash Index          | **Fails on Ranges:** $O(1)$ exact lookup (`WHERE id = 5`), but    |
|                     | CANNOT do range scans (`<, >`), prefix searches, or `ORDER BY`.   |
+-----------------------------------------------------------------------------------------+
```

### 30-Second Verbal Pitch: "Why B+ Tree over B-Tree?"
> *"B+ Trees separate routing keys from actual data payloads. Internal nodes store only routing keys, which maximizes the branching factor per 8KB disk page and keeps the tree shallow (height 3 for 1 billion rows). Additionally, all leaf nodes are connected via a doubly linked list, so range queries like `BETWEEN 20 AND 50` only require finding the first key and sequentially walking leaf pages without re-traversing tree branches."*

---

## 3. Composite Indexes & The Leftmost Prefix Rule

A **Composite Index** is an index on multiple columns: `CREATE INDEX idx_user ON users (dept_id, last_name, first_name);`

### The Leftmost Prefix Rule
The B+ Tree sorts keys lexicographically from left to right. A query can only use the index if it filters on prefixes starting from the leftmost column **with NO gaps**:

```
Given Index on (A, B, C):

Query Filter                       | Index Used? | Spoken Reason
---------------------------------------------------------------------------------------------------
WHERE A = 5                        | YES (Full)  | Leftmost column A is matched.
WHERE A = 5 AND B = 10             | YES (Full)  | Leftmost prefix (A, B) is matched.
WHERE A = 5 AND B = 10 AND C = 20  | YES (Full)  | Complete composite key matched.
WHERE B = 10 AND C = 20            | NO (Scan)   | Fails Leftmost rule! A is missing.
WHERE A = 5 AND C = 20             | PARTIAL     | Uses index for A only; filters C via row scan.
```

### The Range Column Trap & Ideal Column Ordering
When a query contains a **Range Filter (`<, >, BETWEEN, LIKE 'abc%'`)**, the index can be used for that column, but **CANNOT be used to seek subsequent columns**:

```sql
-- Index on: (status, created_at, user_id)
SELECT * FROM orders 
WHERE status = 'PAID'          -- 1. Exact match: Index utilized!
  AND created_at >= '2026-01-01'-- 2. Range match: Index utilized!
  AND user_id = 1042;          -- 3. TRAP: Cannot seek user_id (rows >= date are not sorted by user_id).
```

- **Golden Rule for Composite Index Column Ordering:**
  $$\mathbf{[ \text{Equality Columns } (=) ] \implies [ \text{Sorting Columns } (\text{ORDER BY}) ] \implies [ \text{Range Columns } (<, >) ]}$$

---

## 4. Covering Indexes & Specialized Index Types

### Covering Index (Index-Only Scan)
- A secondary index that includes **all columns requested by the query** (`SELECT`, `WHERE`, `ORDER BY`).
- **Benefit:** The engine answers the query directly from the B+ Tree leaf nodes, completely eliminating the expensive Clustered Index / Table Heap lookup.
- **Postgres `INCLUDE` Clause:** `CREATE INDEX idx ON users (email) INCLUDE (name, age);`  
  Stores `email` in internal routing nodes and appends `name, age` only as unindexed payloads in leaf nodes.

### Specialized Index Types (Quick Reference)
- **Partial / Filtered Index (`WHERE status = 'UNPAID'`):** Indexes only a subset of rows. Ideal for skewed data where 99% of rows are inactive/processed.
- **Expression / Functional Index (`LOWER(email)`):** Precomputes and indexes function outputs.
- **GIN (Generalized Inverted Index):** For composite/unstructured items: JSONB, Arrays, Full-Text Search.
- **GiST (Generalized Search Tree):** For multi-dimensional geometric/spatial coordinates (PostGIS).

---

## 5. LSM-Trees vs. B+ Trees (Storage Engine Architecture)

```
+-----------------------------------------------------------------------------------------+
| DIMENSION           | B+ TREE (PostgreSQL, MySQL, SQLite)    | LSM-TREE (RocksDB, Cassandra)   |
+-----------------------------------------------------------------------------------------+
| Primary Workload    | **Read-Heavy OLTP Queries**            | **High-Throughput Write Streams**|
+-----------------------------------------------------------------------------------------+
| Write Mechanism     | **In-Place Updates:** Random disk I/O  | **Append-Only:** Writes to RAM  |
|                     | overwriting 8KB pages (+ WAL).         | MemTable, flushes to SSTables.  |
+-----------------------------------------------------------------------------------------+
| Write Amplification | **High** (updating 1 row rewrites an   | **Low** (sequential batch writes|
|                     | entire 8KB/16KB disk page).            | maximize SSD write endurance).  |
+-----------------------------------------------------------------------------------------+
| Read Amplification  | **Low & Deterministic** (at most 3–4   | **Higher** (searches MemTable & |
|                     | disk page reads).                      | multiple SSTable files on disk).|
+-----------------------------------------------------------------------------------------+
| Read Optimization   | Buffer Pool RAM caching.               | **Bloom Filters** in RAM to     |
|                     |                                        | bypass nonexistent disk reads.  |
+-----------------------------------------------------------------------------------------+
```

---

## 6. High-Yield Verbal Interview Q&A

### Q1: "Why does adding an index speed up reads but slow down writes?"
> **Spoken Answer:** An index is an auxiliary B+ Tree. While reads locate rows in $O(\log N)$ time, every `INSERT`, `UPDATE`, or `DELETE` must modify both the table data and all associated secondary B+ Trees. This adds write overhead from tree node traversals, page allocations, and potential node splits.

### Q2: "Why can't `WHERE status != 'ACTIVE'` use an index efficiently?"
> **Spoken Answer:** B+ Trees are ordered data structures optimized for narrow ranges and exact matches. Negative predicates (`!=`, `NOT IN`) typically match the vast majority of table rows. Scanning the B+ Tree for 90% of the table is slower than a sequential disk scan due to repeated random leaf lookups, so the optimizer defaults to a Full Table Scan.

### Q3: "What is the role of a Bloom Filter in an LSM-Tree database like Cassandra or RocksDB?"
> **Spoken Answer:** In an LSM-Tree, data is spread across multiple immutable on-disk SSTables. A read for a non-existent key could force reading every SSTable from disk. A Bloom Filter sits in RAM for each SSTable; if it returns `false`, the engine knows with 100% certainty that the key is absent, completely avoiding unnecessary disk I/O.

### Q4: "What is an Index Skip Scan?"
> **Spoken Answer:** When a composite index exists on `(A, B)` and a query filters only on `WHERE B = 10` (missing leftmost prefix `A`), an Index Skip Scan iterates through each distinct value of `A` and performs a B+ Tree seek for `B = 10` within each subtree. It avoids a full table scan when column `A` has low cardinality (e.g., gender or status).
