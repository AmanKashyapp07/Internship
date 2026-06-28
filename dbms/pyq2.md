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

### Q55. Explain single-leader, multi-leader, and leaderless database replication.
* **Asked by:** Meta, Uber, Netflix (System Design)
* **Answer:**
  * **Single-Leader (Master-Slave):** All write requests are sent to a single leader node. The leader applies the write and propagates data updates to followers (slaves) as a replication stream. Reads can go to any node.
    * *Pros:* Simple to design; prevents write conflicts.
    * *Cons:* Single point of failure for writes; scaling writes is difficult.
  * **Multi-Leader:** Multiple nodes act as leaders, accepting write requests. They sync modifications asynchronously.
    * *Pros:* Better write throughput; fault tolerance across geographical regions.
    * *Cons:* Requires complex conflict resolution rules (e.g., Last-Write-Wins, CRDTs).
  * **Leaderless (Dynamo-style):** Clients write to and read from multiple nodes in parallel. Relies on **quorum writes and reads** ($W + R > N$) to ensure consistency. Used by Cassandra and DynamoDB.

---

### Q56. What is Replication Lag? Name two anomalies it causes and how to prevent them.
* **Asked by:** Netflix, Meta, Uber
* **Answer:**
  **Replication Lag** is the delay between a write committing on the leader node and it being applied to replica follower nodes in asynchronous replication.
  * **Anomalies Caused:**
    1. **Stale Reads (Read-Your-Own-Writes violation):** A user submits a post (write goes to leader), refreshes the page, and the read hits a lagging replica. The user's post appears to have vanished.
    2. **Monotonic Reads Violation:** A user reads from replica A (which is up-to-date), then refreshes and hits lagging replica B. The user sees data go back in time.
  * **Mitigation:**
    * Route reads of a user's own profile page strictly to the leader.
    * Track client write timestamps and reject reads on replicas that have not reached that timestamp yet.

---

### Q57. Explain Synchronous vs. Asynchronous Replication.
* **Asked by:** Stripe, AWS RDS, Goldman Sachs
* **Answer:**
  * **Synchronous Replication:** The leader writes to disk and waits for confirmations from replicas before returning a "success" response to the client.
    * *Pros:* Zero data loss if the leader dies (High Consistency).
    * *Cons:* High write latency (client waits for network round-trips); writes block if a replica node crashes.
  * **Asynchronous Replication:** The leader writes to disk, immediately returns success to the client, and propagates the update to replicas in the background.
    * *Pros:* Low write latency; writes succeed even if all replicas crash.
    * *Cons:* Data loss occurs if the leader dies before replicating commits to followers.

---

### Q58. What is the "Split-Brain" problem? How is it resolved using Quorums?
* **Asked by:** Google, Meta, Distributed systems teams
* **Answer:**
  * **Split-Brain** occurs when a network partition cuts off leader election nodes from one another. Two separate sub-networks might both assume the active leader is dead, elect their own leaders, and begin accepting concurrent writes, leading to irreconcilable data conflicts.
  * **Resolution (Quorum Consensus):** Replicas use consensus algorithms (Raft, Paxos) requiring a strict **majority quorum** (e.g., $\lfloor N/2 \rfloor + 1$ nodes) to elect a leader or commit writes. A partitioned minority sub-network cannot form a quorum, preventing it from electing a rogue leader or accepting updates.

---

## Section 8: NoSQL & Distributed Storage Internals (Q59–Q65)

### Q59. How does MongoDB store data? Explain BSON.
* **Asked by:** Meta, Walmart, Flipkart
* **Answer:**
  * MongoDB is a **Document Store** NoSQL database. It organizes data into collections containing flexible, schema-less documents.
  * **BSON (Binary JSON):** MongoDB stores data internally and transmits it as BSON.
  * **JSON vs BSON:** JSON is text-based (slow parsing, limited types). BSON is a binary representation of JSON that adds data types (e.g., `Date`, `Int32`, `Double`, binary raw data) and prefix-length descriptors, allowing the engine to skip fields during scans without parsing the entire document.

---

### Q60. What is Redis and how does it achieve high performance? Explain its persistence.
* **Asked by:** Uber, Stripe, Zepto
* **Answer:**
  * **Redis** is an in-memory, key-value data structure store used as a database, cache, and message broker.
  * **Performance:** Achieves sub-millisecond latency because it stores data in RAM (avoiding disk I/O) and operates on a highly optimized **single-threaded event loop** utilizing multiplexed I/O (epoll/kqueue), which eliminates locking and thread-switching overhead.
  * **Persistence Mechanisms:**
    1. **RDB (Redis Database File):** Performs point-in-time snapshots of the dataset at specified intervals. Fast startup, but potential data loss between snapshots.
    2. **AOF (Append Only File):** Logs every write operation received by the server. Highly durable, but generates larger files and slower startup times.

---

### Q61. Compare SQL databases with NoSQL databases. When would you choose NoSQL?
* **Asked by:** Meta, Google, Uber
* **Answer:**

| Feature | SQL (RDBMS) | NoSQL (Non-Relational) |
| :--- | :--- | :--- |
| **Data Model** | Tabular, structured (rows/columns). | Document, Key-Value, Column-family, Graph. |
| **Schema** | Rigid, predefined. | Dynamic, schema-less. |
| **Scaling** | Vertical (scale up CPU/RAM). | Horizontal (scale out across machines). |
| **Transactions** | Strong ACID compliance. | Focus on BASE (eventual consistency). |

* **When to choose NoSQL:**
  * Data has low/no relational structure (e.g., unstructured document logs).
  * High-throughput scaling requirement beyond a single machine's write capability.
  * Rapidly evolving schemas where schema migrations are costly.

---

### Q62. What is a Column-Family Store (e.g., Cassandra)? How does it store data?
* **Asked by:** Uber, Netflix, Cassandra Teams
* **Answer:**
  * Instead of storing table rows together on disk, a **Column-Family Store** groups columns of a row together, storing values key-by-key on disk.
  * **Cassandra Storage:** Uses a partitioning key to route a row to a node, then stores columns sorted by a clustering key inside SSTables (Sorted String Tables).
  * **Why it's used:** Extremely fast writes, horizontal write scalability, and efficient sparse columns (missing column values use zero disk space).

---

### Q63. Explain the LSM Tree (Log-Structured Merge-Tree) storage engine.
* **Asked by:** Uber, Snowflake, Meta, RocksDB teams
* **Answer:**
  An **LSM Tree** is a write-optimized storage engine used in databases like Cassandra and RocksDB instead of traditional B+ Trees.
  * **How it works:**
    1. Writes are appended to an in-memory sorted structure called a **MemTable** and a sequential WAL log.
    2. When the MemTable is full, it is flushed to disk as an immutable **SSTable** (Sorted String Table).
    3. A background process runs **Compaction** to merge SSTables, discard updates, and remove deleted rows (tombstones).
  * **Why it's preferred for writes:** Turns random write operations into sequential disk writes, eliminating random I/O write penalties of in-place B+ Tree updates.

---

### Q64. What is a Graph Database? When is it preferred over a relational database?
* **Asked by:** LinkedIn, Meta, Neo4j
* **Answer:**
  * A **Graph Database** (e.g., Neo4j) stores data in terms of **Nodes** (entities), **Edges** (relationships), and **Properties**.
  * **When preferred:** When relationships are dense, deeply nested, or dynamic (e.g., social networks, recommendation engines, fraud detection, dependency graphs).
  * **Why:** To find a "friend-of-a-friend" up to 5 degrees of separation in a relational database, you need 5 expensive joins ($O(N^K)$ complexity). Graph databases use index-free adjacency (direct memory pointers to adjacent nodes), traversing connections in $O(1)$ constant time per step.

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

---

### Q83. What is a Distributed Hash Table (DHT)?
* **Asked by:** Cassandra / Peer-to-Peer systems roles
* **Answer:**
  * A **DHT** is a decentralized, distributed system that provides a lookup service similar to a hash table: `(key, value)` pairs are stored in the DHT, and any participating node can retrieve the value associated with a key.
  * **Architecture:** Uses consistent hashing to partition the keyspace across nodes on a ring. Nodes maintain routing tables (e.g., Chord finger tables) to route queries to the correct target node in $O(\log N)$ network hops.

---

### Q84. What is a Vector Database? How does it differ from a relational database?
* **Asked by:** AI/ML Engineering roles (Pinecone, Milvus, Qdrant)
* **Answer:**
  * **Vector Databases** are designed to store, index, and query high-dimensional vector embeddings (mathematical representations of unstructured data like text, images, or audio generated by AI models).
  * **Difference:** Relational databases index scalar values (strings, integers) and perform exact matches. Vector databases index multi-dimensional floats and perform **Approximate Nearest Neighbor (ANN)** searches using distance metrics (Cosine Similarity, Euclidean Distance) over specialized index structures (e.g., HNSW - Hierarchical Navigable Small World graphs).

---

### Q85. How do you scale a database for a celebrity "hotspot" write pattern?
* **Asked by:** Meta, Twitter/X, ByteDance
* **Answer:**
  When a celebrity with millions of followers posts (high write fan-out), updating the timeline feeds of all followers instantly (Push model) overwhelms the database with millions of concurrent writes.
  * **Hybrid Architecture Solution:**
    * **Standard Users (Pull model):** Write posts to a home timeline cache. Followers fetch the feed when logging in.
    * **Celebrity Users (Push-Pull hybrid):** Do not fan-out writes for celebrity posts. Instead, store the celebrity post once. When a follower logs in, merge their personal timeline feed with the celebrity's recent post stream at query time (read-side fan-out).

---

## Section 11: Real-World Architecture & Distributed Trade-Offs (Q86–Q100)

### Q86. What is Database Migration? How do you perform zero-downtime migrations?
* **Asked by:** Stripe, Uber, Backend roles
* **Answer:**
  Database migration is the process of modifying the database schema (adding columns, dropping tables) over time.
  * **Zero-Downtime Migration Pattern (Three-Phase Deploy):**
    1. **Add Column:** Add the new column to the database (nullable or with a default). Deploy application code that writes to *both* the old and new columns, but reads from the old one.
    2. **Backfill:** Run a background job to copy historical data from the old column to the new column in batches (preventing lock exhaustion).
    3. **Switch & Drop:** Update application code to read from the new column. Once verified, deploy code to stop writing to the old column and safely drop the old column from the DB.

---

### Q87. What is Change Data Capture (CDC)?
* **Asked by:** Stripe, Netflix, Data Platform roles
* **Answer:**
  * **CDC** is a design pattern that detects changes made to a database (Inserts, Updates, Deletes) and streams those changes as events to other systems (e.g., Elasticsearch, caches, or data lakes) in real time.
  * **Implementation (Log-based):** Read changes directly from the database's transactional commit logs (e.g., PostgreSQL WAL, MySQL binlog) using tools like Debezium or Kafka Connect.
  * **Benefit:** Zero impact on database query performance (unlike SQL triggers) because it reads logs asynchronously.

---

### Q88. Compare Database Federation vs. Data Warehousing.
* **Asked by:** Stripe, AWS
* **Answer:**
  * **Database Federation (Query Virtualization):** A query engine queries data across multiple distinct databases dynamically at runtime, combining results without moving the data physically.
    * *Pros:* Real-time data access; no storage overhead.
    * *Cons:* Poor performance on large aggregates; queries are restricted by the slowest database network link.
  * **Data Warehousing (ETL):** Data is periodically extracted, transformed, and loaded (ETL) from operational databases into a centralized warehouse (OLAP).
    * *Pros:* Excellent analytical performance; consolidated history.
    * *Cons:* Data is stale (delayed by the ETL pipeline frequency).

---

### Q89. What is a Distributed Transaction? Why is it avoided in microservices?
* **Asked by:** Uber, Stripe, Netflix
* **Answer:**
  * A transaction that updates data across multiple physical databases or distinct microservices.
  * **Why avoided:**
    * Requires blocking protocols like **2-Phase Commit (2PC)**, which increase write latency.
    * If one microservice is down, the entire transaction blocks, violating availability (violates CAP).
    * Binds services together, breaking the fundamental rule of independent microservice deployment.

---

### Q90. What is the Saga Pattern?
* **Asked by:** Uber, Stripe, Backend architects
* **Answer:**
  The **Saga Pattern** is an alternative to distributed transactions in microservices.
  * **How it works:** A saga is a sequence of local transactions. Each service performs its local database transaction and publishes an event. The next service receives the event and executes its local transaction.
  * **Handling Failures (Compensating Transactions):** If a step fails, the saga orchestrator triggers a series of **compensating transactions** backward to undo the changes made by the previous steps (e.g., if a flight booking fails after payment succeeded, trigger a refund transaction).

---

### Q91. What is the role of the Database Buffer Pool?
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
  * **Row-Level Locking:** Locks individual records. High concurrency (other transactions can update adjacent rows), but higher memory overhead due to tracking thousands of individual locks.
  * **Page-Level Locking:** Locks the entire disk block (page) containing many rows. Low concurrency (updates to one row block updates to all rows in that page), but extremely low memory footprint.

---

### Q93. What is Database Federation vs. Data Lake?
* **Asked by:** Data Platform engineers
* **Answer:**
  * **Database Federation:** A query layer that federates queries to operational databases on the fly.
  * **Data Lake:** A centralized repository designed to store vast amounts of raw, unstructured, semi-structured, and structured data (e.g., AWS S3 files, Parquet) at very low cost, which is analyzed using engines like Apache Spark or Presto.

---

### Q94. What is Write Amplification Factor (WAF)? How is it calculated?
* **Asked by:** SSD & Storage engine developers
* **Answer:**
  * **WAF** is the ratio of physical writes written to disk to the logical writes requested by the database host.
    $$\text{WAF} = \frac{\text{Physical Bytes Written to Disk}}{\text{Logical Bytes Written by Host}}$$
  * A WAF of 1 is ideal. A WAF of 5 means writing 1GB of data from the app results in 5GB of actual write wear on the disk drives.

---

### Q95. How does a database resolve conflict updates under multi-leader replication?
* **Asked by:** AWS DynamoDB, Cassandra, System Design
* **Answer:**
  1. **Last-Write-Wins (LWW):** Uses physical wall-clock timestamps. The write with the latest timestamp overwrites others. (Cons: clock skew can cause data loss).
  2. **Conflict-Free Replicated Data Types (CRDTs):** Data structures (like grow-only counters or sets) that can be merged concurrently on different replicas without conflict resolution logic.
  3. **Operational Transformation (OT):** Used in collaborative editing (like Google Docs).
  4. **Custom Conflict Resolution:** Triggering application code to prompt the user to resolve the conflict (e.g., Git merge conflicts).

---

### Q96. What is a "Hotspot" in partition keys? How do you prevent it?
* **Asked by:** AWS DynamoDB, Cassandra, Google Spanner
* **Answer:**
  * A **Hotspot** occurs when a partition key routes a disproportionate volume of write requests to a single database node (e.g., sharding by `country` where $90\%$ of customers are in `US`).
  * **Prevention:**
    * **Salting the key:** Append a random suffix (e.g., `US_1`, `US_2`, `US_3`) to the partition key, distributing the data evenly across nodes.
    * **Composite Keys:** Combine the partition key with another column (e.g., `country + registration_date`).

---

### Q97. Explain Consistent Hashing vs. Rendezvous Hashing.
* **Asked by:** Advanced distributed systems roles
* **Answer:**
  * **Consistent Hashing:** Maps nodes and keys to a ring structure. remaps $K/N$ keys on node addition/removal.
  * **Rendezvous Hashing (HRW):** The client computes a hash of the key combined with each active node ID: $weight = hash(key + node\_id)$. The key is assigned to the node that yields the highest weight.
  * **Benefit of Rendezvous:** Does not require maintaining a ring structure; highly effective for distributed caching routing (e.g., proxy caches).

---

### Q98. What is the role of the Database Transaction Log (redo log)?
* **Asked by:** AWS Aurora, Snowflake, Oracle
* **Answer:**
  * The **Redo Log** records all changes made to the database. Its primary role is to support the durability of transactions.
  * It allows the database to cache dirty pages in memory and delay writing them to disk. If a crash occurs, the redo log is parsed to re-apply committed modifications that were not yet flushed to disk.

---

### Q99. What are Vector Search Indexing algorithms? (HNSW vs. IVF)
* **Asked by:** AI database roles
* **Answer:**
  * **HNSW (Hierarchical Navigable Small World):** Builds a multi-layer graph where the top layers have sparse connections (for fast routing across the graph) and the bottom layers have dense connections (for precise nearest-neighbor search). Extremely fast, but high memory footprint.
  * **IVF (Inverted File Index):** Uses clustering (K-Means) to partition the vector space into voronoi cells. The query only searches vectors in the closest centroids, saving memory at the expense of slight search accuracy.

---

### Q100. What is replication lag, and what is its business impact?
* **Asked by:** Netflix, Uber, Zepto
* **Answer:**
  * **Replication Lag** is the delay in propagating writes from the leader to follower nodes.
  * **Business Impact:**
    * A customer pays for an item, but the checkout screen redirects to a lagging read replica, showing the cart is still full, causing duplicate orders.
    * A user changes their privacy settings, but a lagging replica exposes their private data to the public feed for several seconds.
    * **Mitigation:** Enforce strong consistency reads (route checkout transactions strictly to the leader node).
