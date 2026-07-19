# SQL & DBMS Master Interview Q&A — Part 2 (Q51–Q100) 🚀

This document compiles the remaining 50 of the 100 most-asked theoretical and practical database interview questions. These questions focus on advanced database engines, distributed system design, transactions under MVCC, query optimization internals, and real-world system architecture.

---

## Section 7: Advanced Engine Internals & Concurrency (Q51–Q58)

### Q51. What is Multi-Version Concurrency Control (MVCC)? How does it eliminate read-write conflicts?
* **Asked by:** Uber, Stripe, Meta, Amazon
* **Answer:**
  * **MVCC** is a concurrency control method where the database maintains multiple physical versions of the same data row instead of overwriting it in place.
  * **How it works:** Each row is tagged with transaction IDs indicating when it was created (`xmin`) and deleted/expired (`xmax`). When a transaction reads, it receives a consistent snapshot of the data matching its starting timestamp.
  * **Eliminating conflicts:** Reads do not block writes, and writes do not block reads. Writers create a new version of the row, while readers continue scanning older committed versions.
  * **Garbage Collection:** Expired row versions (called dead tuples) are eventually cleaned up by background processes (e.g., `VACUUM` in PostgreSQL).

---

### Q52. Explain the difference between Pessimistic Locking and Optimistic Concurrency Control (OCC).
* **Asked by:** Stripe, Netflix, Google
* **Answer:**
  * **Pessimistic Locking:** Assumes conflicts are highly likely. Blocks resources immediately using locks (e.g., `SELECT ... FOR UPDATE`).
    * *Pros:* Safe; prevents conflicts before they occur.
    * *Cons:* Reduced concurrency; risks deadlocks; high locking overhead.
  * **Optimistic Concurrency Control (OCC):** Assumes conflicts are rare. Transactions execute without locking. At commit time, the database checks if another transaction modified the data in-between. If yes, the transaction aborts and restarts.
    * *OCC Phases:* Read/Execute $\rightarrow$ Validate $\rightarrow$ Commit.
    * *Pros:* Extremely high throughput in read-heavy or low-contention workloads.
    * *Cons:* High abort rates in write-heavy, highly contested workloads.

---

### Q53. What is the ARIES Recovery Algorithm? Explain its three phases.
* **Asked by:** Core systems roles (Snowflake, AWS RDS, Oracle)
* **Answer:**
  **ARIES** (Algorithm for Recovery and Isolation Exploiting Semantics) is a transaction recovery system based on the "Write-Ahead Logging" (WAL) protocol. It has three phases executed upon database startup after a crash:
  1. **Analysis Phase:** Scans the WAL log forward from the last checkpoint to identify active transactions (loser list) and dirty pages in memory at the time of the crash.
  2. **Redo Phase:** Repeats history by scanning forward, reapplying all logged operations (both committed and uncommitted) to restore the database to the exact state it was in at the moment of failure.
  3. **Undo Phase:** Scans backward to roll back all changes made by active transactions that never committed before the crash (the loser transactions), leaving only committed data.

---

### Q54. What is the difference between Write-Ahead Logging (WAL) and Shadow Paging?
* **Asked by:** Apple, AWS, Core engines
* **Answer:**
  * **Write-Ahead Logging (WAL):** Modifications are written sequentially to a log file on disk before being applied to the actual data pages. The database can overwrite data pages in place.
    * *Pros:* Highly optimized sequential writes; support for concurrent transactions.
  * **Shadow Paging:** The database maintains two page tables: a current page table and a shadow page table. Writing to a page involves duplicating it, writing to the new duplicate page on disk, and updating pointers. Commit is done by atomic swap of the page tables.
    * *Pros:* No log parsing during recovery (very fast startup after crash).
    * *Cons:* Destroys physical page locality (fragmentation); hard to implement concurrent transaction locking.

---

### Q55. What is the difference between Shared Locks (S), Exclusive Locks (X), and Intent Locks (IS, IX)?
* **Asked by:** Oracle, Microsoft, Stripe
* **Answer:**
  * **Shared Lock (S):** Acquired for read operations. Multiple transactions can hold a Shared lock on the same row concurrently, but no transaction can write.
  * **Exclusive Lock (X):** Acquired for write operations (`UPDATE`, `DELETE`). Only one transaction can hold an Exclusive lock on a resource. No other read or write locks are allowed.
  * **Intent Locks (IS / IX):** Enforced at higher levels of the table hierarchy (e.g., table level) to indicate that a transaction holds or intends to acquire shared/exclusive locks on individual rows.
    * **Intent Shared (IS):** Indicates a transaction plans to acquire S locks on rows in the table.
    * **Intent Exclusive (IX):** Indicates a transaction plans to acquire X locks on rows in the table.
    * **Why needed:** Without intent locks, if a transaction wants to lock the *entire table*, it would have to scan every row to check if there are individual row locks. With intent locks, the engine checks the table-level intent lock in $O(1)$ time.

---

### Q56. What is 2-Phase Locking (2PL)? How does it guarantee Serializability, and how does it differ from Strict 2PL?
* **Asked by:** Amazon, Google, Core engines
* **Answer:**
  **2-Phase Locking (2PL)** is a concurrency control protocol that guarantees conflict serializability of transaction schedules. It operates in two distinct phases:
  1. **Growing Phase:** The transaction can acquire locks but cannot release any lock.
  2. **Shrinking Phase:** The transaction can release locks but cannot acquire any new lock.
  * **Strict 2PL (S-2PL):** Modifies 2PL by requiring that all **Exclusive (write) locks** held by a transaction must be kept until the transaction completes (Commit or Abort).
  * **Rigorous 2PL:** Requires *all* locks (both S and X) to be held until Commit/Abort.
  * **Key Difference:** Standard 2PL can suffer from **cascading aborts** (if transaction A releases a lock early and aborts, transaction B which read that data must also abort). Strict 2PL prevents cascading aborts by holding write locks until final validation.

---

### Q57. What is the Phantom Read anomaly, and how does Next-Key Locking prevent it in InnoDB?
* **Asked by:** Uber, Stripe, MySQL specialists
* **Answer:**
  * **Phantom Read:** Occurs when transaction A runs a query to read a range of rows (e.g., `WHERE age > 30`), transaction B inserts a *new* row in that range (e.g., `age = 35`) and commits, and transaction A runs the range query again, seeing the "phantom" row.
  * **Next-Key Locking:** InnoDB prevents phantoms under Repeatable Read using Next-Key Locks. A Next-Key lock is a combination of:
    1. An **Index-Record lock** on the matching index records.
    2. A **Gap lock** on the empty spaces (gaps) in the index structure between, before, and after those records.
  * **Mechanism:** By locking the gaps, other transactions are blocked from inserting any values into the gap, preventing phantoms from being created.

---

### Q58. Explain the anatomy of a database page/disk block.
* **Asked by:** Snowflake, Oracle, Apple
* **Answer:**
  A database page (typically 8KB in Postgres, 16KB in InnoDB) is the unit of disk I/O. Its structure contains:
  1. **Page Header:** Metadata about the page (LSN log identifier, free space pointers, page flags).
  2. **Line Pointer Array (Slot Directory):** An array of offsets pointing to the start of each actual row on the page. Sorted from left-to-right.
  3. **Free Space:** Unallocated bytes. The directory grows forward, and rows are written backward from the end of the page to meet in the middle.
  4. **Data Tuples (Row Data):** Actual row values, headers, and column bytes written from the bottom of the page upward.
  * **Benefits:** Slot directories let the engine move rows around on a page (defragmentation) without changing row pointers in indexes; only the directory offset needs update.

---

## Section 8: Storage Engine Internals & Indexing (Q59–Q65)

### Q59. What is Row Overflow? How does a database store columns larger than a single page?
* **Asked by:** Oracle, PostgreSQL core teams
* **Answer:**
  Since databases read and write data in fixed-size pages (e.g., 8KB), a row containing large text or blob values (like `user_bio` or `pdf_file`) cannot fit in a standard page.
  * **Row Overflow / TOAST (The Oversized-Attribute Storage Technique):**
    * If a row exceeds a page limit, the database splits the large column out of the row page.
    * The main row stores a small 24-byte **pointer (descriptor)** referencing the large attribute.
    * The actual data is compressed, split into chunks, and stored in a separate, dedicated overflow storage table (e.g., TOAST table in Postgres) across multiple pages.
  * **Benefit:** Keeps the primary table pages narrow and clean, ensuring fast scans when queries don't select the large column.

---

### Q60. Why do MVCC database engines need Vacuuming / Compaction?
* **Asked by:** PostgreSQL, MySQL DBAs, Heroku
* **Answer:**
  Under Multi-Version Concurrency Control (MVCC), when a row is `UPDATED`, the database does not overwrite it; it writes a new version of the row and marks the old version as dead. When a row is `DELETED`, it is simply flagged as deleted.
  * **The Problem (Bloat):** Dead rows (tuples) continue to occupy physical space on disk and in memory pages, slowing down table scans.
  * **Vacuuming / Compaction:**
    * A background system process (e.g., `VACUUM` in PostgreSQL) scans pages, identifies dead tuples that are no longer visible to any active transaction, and marks their space as free for reuse by new inserts.
    * **VACUUM FULL:** Rewrites the entire table to a new physical file on disk to shrink the database size, but locks the table exclusively.

---

### Q61. What is the difference between a Lock and a Latch in database internals?
* **Asked by:** Google, Core engines, Systems roles
* **Answer:**
  Both are synchronization mechanisms, but they operate at different layers:
  * **Locks:** High-level logical constructs used to manage concurrency and transaction isolation (ACID). They protect database *user data* (tables, rows).
    * *Scope:* Visible to users; held for the duration of a transaction (long-lived).
    * *Modes:* Shared, Exclusive, Intent.
  * **Latches:** Low-level physical locks used to protect internal *memory structures* (e.g., buffer pool frames, B+ Tree page pointers) from concurrent thread access.
    * *Scope:* Internal (invisible to users); held only for microseconds while modifying pointers (short-lived).
    * *Modes:* Read, Write.

---

### Q62. Explain the difference between a Clustered Index and a Primary Key Constraint.
* **Asked by:** Microsoft, Oracle, SQL Server developers
* **Answer:**
  * **Primary Key (Logical):** A constraint that guarantees uniqueness and nullability rules for a column. It is a logical model definition.
  * **Clustered Index (Physical):** Dictates how the actual data rows of the table are physically sorted and stored on disk.
  * **Differences:**
    * You can define a primary key, and the database will automatically back it with an index, but that index doesn't *have* to be clustered.
    * A table can have only **one clustered index** (since data can only be physically sorted in one way).
    * You can create a clustered index on a non-primary key column if range queries on that column are very common.

---

### Q63. How does a Cost-Based Optimizer (CBO) work? What is the role of Database Statistics?
* **Asked by:** Google, Snowflake, AWS Aurora
* **Answer:**
  * The **CBO** evaluates multiple physical execution plans for a parsed SQL query and assigns an estimated "cost" (CPU cycles and disk page I/Os) to each. It selects the plan with the lowest cost.
  * **Role of Statistics:** To estimate cost, the optimizer needs to know the distribution of data. It stores:
    * Row count of tables.
    * **Data density / selectivity** of indexes.
    * **Histograms** showing the distribution of values in columns.
  * **The Gotcha:** If statistics are stale (e.g., table recently grew from 1,000 to 1,000,000 rows without `ANALYZE` updating statistics), the optimizer may choose a full table scan instead of an index seek, causing a massive performance collapse.

---

### Q64. Compare Index Scan, Index Seek, and Table Scan.
* **Asked by:** Microsoft, Walmart, Adobe
* **Answer:**
  * **Table Scan:** The engine scans the entire heap or clustered table page-by-page. It reads every row. Used when no index fits, or when the query retrieves a large fraction of the table ($>20\%$).
  * **Index Scan:** The engine scans the entire index tree (usually leaf nodes) from start to finish. Useful when a query only requests indexed columns (index-only scan) but needs to inspect all rows.
  * **Index Seek:** The engine utilizes the index key to traverse the B+ Tree from root to leaf, navigating directly to the specific starting row matching the query filter (`O(log N)` complexity). This is the most optimal execution operation.

---

### Q65. Row-oriented vs. Columnar storage: How do they differ at the storage level?
* **Asked by:** ClickHouse, Snowflake, Google BigQuery
* **Answer:**
  * **Row-oriented (OLTP, e.g., Postgres):** Stores entire row data blocks contiguously on disk.
    * *Pros:* Fast single-row reads and writes (whole row loaded in one I/O).
    * *Cons:* Scanning a single column requires loading the entire table into memory.
  * **Columnar storage (OLAP, e.g., Parquet, Snowflake):** Stores all values of a single column contiguously on disk.
    * *Pros:* Analytical aggregates (e.g., `AVG(salary)`) only load the `salary` column. High compression ratios because columns contain similar data types.
    * *Cons:* Writing a single row requires updating multiple distinct files on disk (slow writes).

---

## Section 9: Advanced Indexing & Query Optimization (Q66–Q73)

### Q66. What is Index Selectivity? Why is low selectivity bad for B+ Trees?
* **Asked by:** Netflix, Stripe, Google
* **Answer:**
  * **Selectivity** is the ratio of unique values in a column to the total number of records.
    $$\text{Selectivity} = \frac{\text{Count of Unique Values}}{\text{Total Rows}}$$
  * **High Selectivity:** Values are highly unique (e.g., `email`, `employee_id`).
  * **Low Selectivity:** Values have high duplication (e.g., `gender`, `is_active` boolean).
  * **Why low selectivity is bad:** If a query filters on a column with low selectivity (e.g., `WHERE gender = 'F'`), it matches a massive fraction of rows. The engine would spend more time switching between index page lookups and data page lookups than it would performing a single, sequential full table scan.

---

### Q67. What is a Composite Index? Explain the Left-Prefix Rule.
* **Asked by:** Meta, Stripe, Zepto
* **Answer:**
  * A **Composite Index** is an index created on multiple columns (e.g., `INDEX (col_A, col_B, col_C)`).
  * **Left-Prefix Rule:** The query optimizer can only utilize a composite index if the query filters on the columns in order from left to right.
    * A query filtering on `col_A` or `col_A AND col_B` **can** use the index.
    * A query filtering on `col_B` or `col_C` **cannot** use the index because the index tree is sorted primarily by `col_A`, and only sorted by `col_B` within matching values of `col_A`.

---

### Q68. What is a Partial (or Filtered) Index? When is it useful?
* **Asked by:** Uber, Stripe, Netflix
* **Answer:**
  * A **Partial Index** is an index built over a subset of rows defined by a conditional filter.
  * **Syntax:** `CREATE INDEX idx_active_users ON users(id) WHERE status = 'active';`
  * **When useful:**
    * High-volume skew (e.g., $99\%$ of rows are archived, $1\%$ are active). Building an index on only active rows keeps the index small, fits it in RAM, and speeds up queries.
    * Indexing only non-null values to save space.

---

### Q69. What is a Function-Based (Expression) Index?
* **Asked by:** Amazon, Oracle
* **Answer:**
  * Standard indexes evaluate raw column values. A **Function-Based Index** indexes the calculated result of a function or expression applied to a column.
  * **Use Case:** Preventing index-busting functions in queries.
  * **Example:**
    ```sql
    CREATE INDEX idx_lower_email ON Users(LOWER(email));
    ```
    Now, a query using `WHERE LOWER(email) = 'user@test.com'` will use this index. Without it, using `LOWER()` on a standard email index forces a full table scan because the engine must compute `LOWER()` for every row to match.

---

### Q70. What is Index Fragmentation and Page Splits? How do you resolve them?
* **Asked by:** Microsoft, AWS RDS, Core platforms
* **Answer:**
  * **Page Splits:** When inserting a row into a sorted B+ Tree page that is full ($100\%$ capacity), the engine splits the page into two, moving half the keys to a new page and updating parent pointers.
  * **Index Fragmentation:** Page splits leave pages partially empty (e.g., $50\%$ fill factor) and physically scattered across disk pages rather than sequential. This increases index size and degrades scan performance.
  * **Resolution:** Rebuild the index using maintenance commands (e.g., `ALTER INDEX ALL REBUILD` in SQL Server, or `REINDEX` in PostgreSQL).

---

### Q71. What is the N+1 Query Problem in ORMs? How do you prevent it?
* **Asked by:** Backend Engineers (Django, Rails, Spring Boot roles)
* **Answer:**
  The **N+1 Query Problem** occurs when an ORM framework executes 1 initial query to fetch a parent list, followed by $N$ separate queries to fetch related child details for each parent.
  * **Example:** Loading 50 books and their authors:
    * Query 1: `SELECT * FROM books;` (returns 50 books).
    * Queries 2–51: `SELECT * FROM authors WHERE id = ?;` (executed 50 times).
  * **Prevention:** Use **Eager Loading** (e.g., `select_related` or `prefetch_related` in Django, or `JOIN FETCH` in Hibernate) to combine parent and child data into a single SQL join query.

---

### Q72. What are Database Triggers? Explain the trade-offs of using them in production.
* **Asked by:** Uber, Stripe, FinTechs
* **Answer:**
  A **Trigger** is a database object that automatically executes a SQL procedure in response to specific events (`INSERT`, `UPDATE`, `DELETE`) on a table.
  * **Pros:** Guarantees audit trails; enforces business rules at the storage level regardless of application code changes.
  * **Cons:**
    * **Hidden Logic:** Difficult to debug because application code does not show why a database write failed.
    * **Performance Penalty:** Triggers run synchronously inside the write transaction block, increasing transaction lock times.
    * **Maintenance:** Hinders database migration and bulk loading pipelines.

---

### Q73. What is a Stored Procedure? Compare it with Ad-hoc Queries.
* **Asked by:** Microsoft, Financial Institutions
* **Answer:**
  A **Stored Procedure** is a set of precompiled SQL statements stored directly in the database engine.
  * **Stored Procedure vs. Ad-hoc:**
    * **Performance:** Stored procedures are parsed and compiled once by the query planner, saving parsing cycles. They reduce network latency because multiple commands run inside the engine without round-trips.
    * **Security:** Prevents SQL injection by enforcing parameterized execution; allows administrators to grant execute permissions on the procedure without exposing direct table access.
    * **Maintainability:** Harder to version control and scale compared to application code.

---

## Section 10: Transactions, Locking & Scaling Edge Cases (Q74–Q85)

### Q74. What is the Write Skew anomaly? In which isolation level does it occur?
* **Asked by:** Stripe, Meta, Uber
* **Answer:**
  * **Write Skew** is a serialization anomaly that occurs when two concurrent transactions read the same data, make independent decisions based on that data, and write conflicting updates that violate a business rule.
  * **Scenario:** Doctors On Call. Rule: at least one doctor must be active. Doctor A and Doctor B are both on call. Both try to check out concurrently.
    * T1 reads: "Active count = 2". T1 checks out.
    * T2 reads: "Active count = 2". T2 checks out.
    * Both commit. Active count becomes 0, violating the rule.
  * **Isolation Level:** Occurs under **Snapshot Isolation** (often implemented as Repeatable Read).
  * **Prevention:** Upgrade to **Serializable** isolation, or use explicit row locking (`SELECT ... FOR UPDATE`) to block concurrent reads of the active count.

---

### Q75. What is the difference between a Hard Parse and a Soft Parse?
* **Asked by:** Oracle, Salesforce
* **Answer:**
  * **Hard Parse:** The database engine receives a query, validates syntax, checks user permissions, runs optimization algorithms, and generates a new execution plan. Extremely CPU-heavy.
  * **Soft Parse:** The database checks its shared pool cache (Query Plan Cache) and finds an identical compiled query plan, skipping optimization.
  * **How to ensure Soft Parses:** Use **Parameterized Queries / Prepared Statements** (`SELECT * FROM users WHERE id = ?`) rather than dynamically appending string variables, allowing the database to reuse the cached query execution plan.

---

### Q76. Explain the difference between Shared, Exclusive, and Intent Locks.
* **Asked by:** Google, Amazon, Snowflake
* **Answer:**
  * **Shared Lock (S):** Placed on a resource for read operations. Multiple transactions can hold S locks concurrently.
  * **Exclusive Lock (X):** Placed on a resource for write operations. Only one transaction can hold an X lock, blocking all other reads/writes.
  * **Intent Locks (IS / IX):** Placed at the table level to indicate that a transaction holds or intends to acquire S or X locks on individual rows.
    * *Why Intent Locks exist:* Before acquiring a table lock, the database engine checks the table's intent locks to see if any row-level locks are active, avoiding scanning millions of rows to verify lock compatibility.

---

### Q77. What is Lock Escalation? Why does a database perform it?
* **Asked by:** Core DBMS/Platform roles
* **Answer:**
  * **Lock Escalation** is the process of converting many fine-grained locks (e.g., thousands of row locks) into a single coarse-grained lock (e.g., a table lock).
  * **Why:** Every lock consumed uses a portion of the database server's RAM. If a transaction updates millions of rows, maintaining millions of individual lock structures can exhaust the database memory pool. Escalating to a table lock saves memory, though it reduces concurrency.

---

### Q78. Explain "Parameter Sniffing".
* **Asked by:** Microsoft, SQL Server teams
* **Answer:**
  * **Parameter Sniffing** is when the query optimizer compiles an execution plan using the specific parameter values passed during the query's *first* execution.
  * **Why it can go wrong:** If the first execution used a highly atypical parameter (e.g., searching for a rare keyword returning 1 row), the optimizer generates a fast index-seek plan. If subsequent executions pass a common parameter returning $90\%$ of the table, using that cached seek plan causes terrible performance.
  * **Mitigation:** Query hints to recompile the query, or ignoring parameter values when caching plans.

---

### Q79. What is Write Amplification? How does it affect SSDs?
* **Asked by:** Core infrastructure roles (RocksDB, Cassandra, SSD engineering)
* **Answer:**
  * **Write Amplification** is the phenomenon where the actual volume of physical data written to disk is a multiple of the logical data written by the application.
  * **Causes:** In-place updates causing page splits, WAL logs, and LSM tree compaction cycles.
  * **SSD Impact:** SSDs can only write to erased blocks. High write amplification exhausts SSD write cycles (TBW - Terabytes Written) and degrades disk I/O performance due to garbage collection cycles.

---

### Q80. Compare Basic 2PL, Strict 2PL, and Rigorous 2PL.
* **Asked by:** Uber, Stripe, Core systems
* **Answer:**
  **2-Phase Locking (2PL)** requires transactions to lock resources in two phases: Growing (acquiring locks) and Shrinking (releasing locks).
  * **Basic 2PL:** Allows releasing locks at any time during the shrinking phase.
    * *Cons:* Can cause cascading rollbacks.
  * **Strict 2PL:** Holds all **Exclusive (write) locks** until the end of the transaction (Commit/Abort).
    * *Pros:* Guarantees cascadeless schedules (prevents dirty reads).
  * **Rigorous 2PL:** Holds **both Shared (read) and Exclusive (write) locks** until the end of the transaction.
    * *Pros:* Strongest; guarantees strict serializability.

---

### Q81. What is the Thundering Herd / Cache Stampede problem in database caching?
* **Asked by:** High-Scale System Design (Meta, Netflix, Amazon)
* **Answer:**
  * **Cache Stampede** occurs when a highly popular cached key expires, and thousands of concurrent requests find a "cache miss" at the exact same moment.
  * **The Problem:** All these requests hit the primary database in parallel to fetch and rebuild the cache key, overwhelming the database CPU/connections and potentially crashing the system.
  * **Mitigation:**
    * **Mutex Locking:** Only the first request acquires a lock to query the DB and update the cache; other requests wait or receive stale data temporarily.
    * **Probabilistic Early Expiry:** Recompute and refresh the cache key in the background *before* it physically expires.

---

### Q82. What are 4NF and 5NF in normalization?
* **Asked by:** High-end database architects
* **Answer:**
  * **4NF (Fourth Normal Form):** Must be in BCNF + **no multi-valued dependencies (MVD)**. An MVD ($A \twoheadrightarrow B$) occurs when the presence of one attribute value determines a set of independent values in another column.
    * *Example:* If an employee has multiple phone numbers and multiple skills, storing them in a single table causes Cartesian duplicates. 4NF splits them into `EmployeePhone` and `EmployeeSkill` tables.
  * **5NF (Project-Join Normal Form):** Handles join dependencies. A relation is in 5NF if it cannot be decomposed into smaller tables without losing information when re-joined.

### Q83. What is the difference between a Natural Join and an Inner Join?
* **Asked by:** Oracle, IBM, Goldman Sachs
* **Answer:**
  * **Inner Join:** Joins tables based on an explicit join predicate specified in the `ON` or `USING` clause. You can join columns even if they have different names (e.g., `JOIN Orders o ON c.customer_id = o.buyer_id`).
  * **Natural Join:** An implicit inner join that automatically joins tables based on **all columns that share the same name** in both tables.
  * **Why it's avoided in production:**
    * If someone adds a column to one of the tables in the future (e.g., adding `created_at` to both `Customer` and `Orders`), the natural join will implicitly begin joining on `created_at` too, breaking the application's query output.
    * It is not self-documenting; developers cannot see the join condition without inspecting both table schemas.

---

### Q84. How do databases handle NULL values in conditional expressions, sorting, and aggregate functions (Three-Valued Logic)?
* **Asked by:** Google, Amazon, Stripe
* **Answer:**
  * **Three-Valued Logic (3VL):** Boolean logic in SQL can evaluate to `TRUE`, `FALSE`, or `UNKNOWN`. Any comparison involving `NULL` (e.g., `age = NULL`, `age != NULL`, or `NULL = NULL`) evaluates to `UNKNOWN`.
  * **Sorting:** In SQL standard, `NULL` values are grouped together. PostgreSQL sorts `NULL` as highest (placed at the end in `ASC` sorting), whereas MySQL/SQL Server sort `NULL` as lowest.
  * **Aggregate Functions:** Functions like `SUM`, `AVG`, `MIN`, `MAX`, and `COUNT(column)` **ignore NULL values**. Only `COUNT(*)` counts rows regardless of nullability.
  * **Conditional Checks:** To check for NULL, you must use `IS NULL` or `IS NOT NULL`. E.g., `WHERE age IS NULL`.

---

### Q85. What is the difference between a Clustered and a Non-Clustered index under the hood in terms of physical traversal?
* **Asked by:** Microsoft, Oracle, Amazon
* **Answer:**
  * **Clustered Index Traversal:**
    * The leaf nodes of the clustered index B+ Tree contain the **actual physical row data pages**.
    * Once the engine traverses the tree and finds the matching key, it reads the row immediately from the leaf page.
  * **Non-Clustered Index Traversal:**
    * The leaf nodes of a non-clustered index contain **pointers** to the physical rows (either physical Row IDs (RIDs) in a heap table, or the primary key values in an index-organized table).
    * Traversal requires a two-step process: (1) traverse the non-clustered B+ Tree to find the pointer, (2) use that pointer to fetch the row from the data table (often called a RID Lookup or Key Lookup).

---

## Section 11: Core Schema Design, Joins & Execution Internals (Q86–Q100)

### Q86. What is Database Migration? How do you perform zero-downtime migrations?
* **Asked by:** Stripe, Uber, Backend roles
* **Answer:**
  Database migration is the process of modifying the database schema (adding columns, dropping tables) over time.
  * **Zero-Downtime Migration Pattern (Three-Phase Deploy):**
    1. **Add Column:** Add the new column to the database (nullable or with a default). Deploy application code that writes to *both* the old and new columns, but reads from the old one.
    2. **Backfill:** Run a background job to copy historical data from the old column to the new column in batches (preventing lock exhaustion).
    3. **Switch & Drop:** Update application code to read from the new column. Once verified, deploy code to stop writing to the old column and safely drop the old column from the DB.

---

### Q87. Explain the three physical join algorithms: Nested Loop Join, Hash Join, and Sort-Merge Join. Compare their execution time and memory complexity.
* **Asked by:** Google, Snowflake, AWS Aurora
* **Answer:**
  The database engine compiles logical Joins into one of three physical join algorithms:
  1. **Nested Loop Join:** For each row in the outer table, scan the inner table for a match.
    * *Complexity:* $O(M \times N)$ time, $O(1)$ memory. Optimal if one table is very small and the join column on the other table has a B+ Tree index (making it $O(M \log N)$).
  2. **Hash Join:** Builds an in-memory hash table on the join key of the smaller table (build input), then scans the larger table (probe input) to find matches.
    * *Complexity:* $O(M + N)$ time, $O(M)$ memory. Optimal for large, unsorted tables without indexes.
  3. **Sort-Merge Join:** Sorts both tables by the join key, then merges them in a single parallel scan.
    * *Complexity:* $O(M \log M + N \log N)$ time, $O(1)$ memory (or $O(M+N)$ if sorting in memory). Optimal if tables are already sorted (e.g., via indexes) or for inequality joins (`>`, `<`).

---

### Q88. What is a Sargable query? How do you rewrite a query to make it sargable?
* **Asked by:** Microsoft, Walmart, Adobe
* **Answer:**
  * **Sargable (Search Argument Able):** A query is sargable if the database engine can utilize an index to speed up execution (Index Seek).
  * **Non-Sargable:** Occurs when you wrap index columns in functions or operations, forcing the engine to evaluate the function for every single row (Table/Index Scan).
  * **Rewriting Examples:**
    * ❌ *Non-Sargable:* `WHERE YEAR(order_date) = 2026`
    *  *Sargable:* `WHERE order_date >= '2026-01-01' AND order_date < '2027-01-01'`
    * ❌ *Non-Sargable:* `WHERE SUBSTRING(name, 1, 3) = 'Rob'`
    *  *Sargable:* `WHERE name LIKE 'Rob%'`

---

### Q89. Compare the performance and execution plan impact of UNION vs. UNION ALL.
* **Asked by:** Oracle, Amazon, Stripe
* **Answer:**
  * **UNION ALL:** Simply concatenates the result sets of two queries.
    * *Performance:* Extremely fast ($O(N)$). It requires no memory sorting or hashing because it returns duplicate rows.
  * **UNION:** Concatenates result sets, then removes duplicate rows.
    * *Performance:* Slow ($O(N \log N)$ or $O(N)$ with hash). Under the hood, the engine must perform a duplicate-elimination step using sorting or hashing.
  * **Interview Rule:** Always use `UNION ALL` unless you explicitly require duplicate rows to be filtered out.

---

### Q90. What is a database transaction Savepoint? How is it used?
* **Asked by:** Stripe, Oracle, Finance dev teams
* **Answer:**
  * **Savepoint** is a transaction control language (TCL) primitive that allows a transaction to be rolled back partially without aborting the entire transaction.
  * **Mechanism:**
    1. Start a transaction (`BEGIN`).
    2. Execute some statements.
    3. Declare a savepoint: `SAVEPOINT my_savepoint;`
    4. Execute more statements.
    5. If an error occurs, roll back only to that savepoint: `ROLLBACK TO my_savepoint;`
    6. Continue other work and `COMMIT`.
  * **Use Case:** Nested transaction blocks in application servers where a minor failure (e.g., logging statement failure) shouldn't abort the core payment transaction.

---

### Q91. What is the role of the Database Buffer Pool? Explain LRU scan pollution and the Clock sweep replacement algorithm.
* **Asked by:** Google, Amazon, Snowflake
* **Answer:**
  * The **Buffer Pool** is a portion of database RAM used to cache table data pages and index pages read from disk.
  * **LRU vs. Clock replacement:**
    * **LRU (Least Recently Used):** Evicts the page that has not been accessed for the longest time.
      * *Problem:* A full table scan loads millions of pages once, evicting the entire active cache (LRU Scan pollution).
    * **Clock (Second Chance):** Pages are arranged in a circular buffer with a usage bit. A clock hand sweeps; if the usage bit is 1, it resets to 0. If it finds a 0, it evicts that page. Highly efficient; avoids page lock overhead of LRU list pointer updates.

---

### Q92. What is the difference between Row-level locking and Page-level locking?
* **Asked by:** Microsoft, Oracle
* **Answer:**
  * **Row-Level Locking:** Locks individual records. High concurrency (other transactions can update graphacent rows), but higher memory overhead due to tracking thousands of individual locks.
  * **Page-Level Locking:** Locks the entire disk block (page) containing many rows. Low concurrency (updates to one row block updates to all rows in that page), but extremely low memory footprint.

---

### Q93. What is Referential Integrity? Explain CASCADE, SET NULL, RESTRICT, and NO ACTION referential actions.
* **Asked by:** Oracle, SQL Server, MySQL Developers
* **Answer:**
  * **Referential Integrity:** A database design rule requiring that foreign key columns must always reference a valid, existing primary key in the parent table.
  * **Referential Actions (On Delete/On Update):** Defines what happens to the child table when a parent row is deleted or updated:
    * `CASCADE`: Automatically deletes or updates matching rows in the child table.
    * `SET NULL`: Sets the child table's foreign key columns to `NULL`.
    * `RESTRICT`: Rejects the delete or update on the parent table immediately (fails the transaction).
    * `NO ACTION`: Similar to RESTRICT, but the check is deferred until the end of the statement or transaction.

---

### Q94. What is Write Amplification Factor (WAF)? How is it calculated?
* **Asked by:** SSD & Storage engine developers
* **Answer:**
  * **WAF** is the ratio of physical writes written to disk to the logical writes requested by the database host.
    $$\text{WAF} = \frac{\text{Physical Bytes Written to Disk}}{\text{Logical Bytes Written by Host}}$$
  * A WAF of 1 is ideal. A WAF of 5 means writing 1GB of data from the app results in 5GB of actual write wear on the disk drives.

---

### Q95. What are Functional Dependencies and Armstrong's Axioms in database theory?
* **Asked by:** Academic/DBMS internals interviews
* **Answer:**
  * **Functional Dependency (FD):** A constraint between two sets of attributes. $A \rightarrow B$ means the value of attribute set $A$ uniquely determines the value of attribute set $B$.
  * **Armstrong's Axioms:** A set of rules used to find all functional dependencies of a relation:
    1. **Reflexivity:** If $B \subseteq A$, then $A \rightarrow B$.
    2. **Augmentation:** If $A \rightarrow B$, then $AC \rightarrow BC$ for any attribute set $C$.
    3. **Transitivity:** If $A \rightarrow B$ and $B \rightarrow C$, then $A \rightarrow C$.
  * **Secondary rules derived from axioms:** Union (if $A \rightarrow B$ and $A \rightarrow C$, then $A \rightarrow BC$) and Decomposition (if $A \rightarrow BC$, then $A \rightarrow B$ and $A \rightarrow C$).

---

### Q96. What is Lossless-Join Decomposition and Dependency Preservation in database normalization?
* **Asked by:** High-end database architects, DB Designers
* **Answer:**
  When decomposing a table into smaller tables during normalization, two properties must be preserved:
  1. **Lossless-Join Decomposition:** Guarantees that when the split tables are joined back together, the result is identical to the original table without introducing "spurious" or fake rows. (This holds true if the common column is a key of at least one of the decomposed tables).
  2. **Dependency Preservation:** Enforces that all functional dependencies of the original table can be checked using only individual tables in the decomposition, without executing expensive join operations.

---

### Q97. What is Lock Escalation? How does the database engine decide when to escalate locks?
* **Asked by:** Microsoft SQL Server, DB2 Specialists
* **Answer:**
  * **Lock Escalation:** The process of converting multiple fine-grained locks (e.g., locking 10,000 individual rows) into a single, coarse-grained lock (e.g., locking the entire table) to free up database lock manager memory.
  * **How the engine decides:**
    * Locks consume memory pages. If a single transaction exceeds a lock threshold (e.g., holds $>5,000$ row locks on a single table) or if the database-wide lock memory consumption exceeds a safety threshold ($10-20\%$ of memory), the engine escalates locks.
  * **Drawback:** Drastically reduces concurrency, as other transactions are now blocked from accessing unrelated rows in that table.

---

### Q98. What is the role of the Database Transaction Log (redo log)?
* **Asked by:** AWS Aurora, Snowflake, Oracle
* **Answer:**
  * **Redo Log** records all changes made to the database. Its primary role is to support the durability of transactions.
  * It allows the database to cache dirty pages in memory and delay writing them to disk. If a crash occurs, the redo log is parsed to re-apply committed modifications that were not yet flushed to disk.

---

### Q99. What is a Database Cursor? Explain its overhead and compare Forward-Only vs. Scrollable cursors.
* **Asked by:** SQL Developer roles, Oracle/SQL Server interviews
* **Answer:**
  * **Cursor** is a database object used to retrieve and process query result rows one-by-one sequentially, rather than in bulk (set-based processing).
  * **Overhead:** Cursors are notoriously slow because they violate SQL's set-based design, forcing high row-by-row CPU context switching and maintaining locks on database resources for long periods.
  * **Types:**
    * **Forward-Only (Default):** Rows can only be read sequentially from the first row to the last row. Highly efficient compared to scrollable.
    * **Scrollable:** Allows the developer to navigate backward, skip rows, or jump directly to specific row offsets. Requires keeping the result set in tempdb memory, creating high resource overhead.

---

### Q100. What is the difference between a B+ Tree index and a Hash index? When is a Hash index preferred?
* **Asked by:** Meta, Google, Postgres internals
* **Answer:**
  * **B+ Tree Index:**
    * Stores keys in a balanced tree structure where leaf nodes are sorted and linked.
    * Supports range scans (`WHERE age BETWEEN 20 AND 30`), inequality searches (`<`, `>`), and prefix matches (`LIKE 'Rob%'`).
    * *Complexity:* $O(\log N)$ lookup.
  * **Hash Index:**
    * Maps keys to data page locations using a hash function.
    * Supports only exact equality checks (`WHERE id = 101`). Cannot perform range scans or sorting because hashing destroys sort order.
    * *Complexity:* $O(1)$ constant time lookup.
  * **When Preferred:** When the query workload is strictly exact equality checks and the database requires the fastest possible single-record retrieval (e.g., key-value lookups in caching engines or memory tables).


