# DBMS Interview Solutions — The Microsoft Pitch Guide

> **Format**: Each answer is structured as:
> 1. **How to Say It Out Loud (The 20-Second Pitch)**: Direct, crisp answer hitting the exact keywords interviewers score on.
> 2. **Technical Backing / Under the Hood**: Architecture, algorithms, disk I/O, or SQL internals.
> 3. **Follow-Up / Preempting the Interviewer**: The next question the interviewer usually probes.

---

### Q1: How does a database engine physically achieve the ACID properties under the hood?
- **How to Say It Out Loud**:
  "Atomicity is guaranteed via Undo logs that rollback incomplete transactions. Durability is guaranteed via Write-Ahead Logging (WAL) and Redo logs flushed to disk at commit time. Isolation is enforced using either Two-Phase Locking (2PL) or Multi-Version Concurrency Control (MVCC). Consistency is maintained by schema integrity constraints combined with Atomicity and Isolation preventing dirty states."
- **Under the Hood**:
  - *WAL*: All changes are written to the append-only transaction log on disk before data pages in memory are written to disk.
  - *Undo Log*: Contains old image of data before modification. Used for transaction rollback and MVCC reads.
  - *Redo Log*: Contains new image of data after modification. Used during crash recovery to replay committed transactions.
- **Follow-Up**: *What happens during database crash recovery?* ARIES recovery: Analysis pass (identifies dirty pages) -> Redo pass (rolls forward all committed changes) -> Undo pass (rolls back uncommitted transactions).

---

### Q2: What is Write-Ahead Logging (WAL), and why can't we write modified data pages directly to table files?
- **How to Say It Out Loud**:
  "Writing modified data pages directly to disk requires random disk I/O across scattered blocks, which is slow and would leave data corrupted if a crash happens mid-write. WAL solves this by appending transaction log records sequentially to disk before touching table files. Sequential disk I/O is 100 times faster than random I/O, ensuring Durability with minimal commit latency."
- **Under the Hood**:
  The buffer pool maintains dirty pages in RAM. When a transaction commits, only the small, sequential WAL log buffer is flushed via `fsync()`. The actual dirty pages are flushed lazily in batches by background threads (checkpointing).
- **Follow-Up**: *What is fsync()?* A system call that flushes OS kernel buffer caches directly to physical persistent storage.

---

### Q3: What is a Checkpoint in database crash recovery, and how does it prevent infinite WAL replay?
- **How to Say It Out Loud**:
  "A checkpoint is a periodic background operation where the DBMS flushes all dirty memory pages in the buffer pool to disk and records a Log Sequence Number (LSN). During crash recovery, the DBMS only needs to replay WAL logs generated after the checkpoint, bounding recovery time from hours to seconds."
- **Under the Hood**:
  Without checkpoints, the WAL would grow indefinitely, and crash recovery would require replaying every transaction since the database was created.
- **Follow-Up**: *What is a Fuzzy Checkpoint?* Modern engines do not pause transactions; they record the minimum LSN of dirty pages currently in the buffer pool and flush them asynchronously.

---

### Q4: What is the structural and storage difference between a Clustered Index and a Non-Clustered Index?
- **How to Say It Out Loud**:
  "A Clustered Index dictates the physical sorting order of table rows on disk; the leaf nodes are the actual table rows. There can only be one clustered index per table. A Non-Clustered (Secondary) Index is a separate B+ tree whose leaf nodes store only the indexed keys plus a row locator pointing to the clustered key. Looking up data via a secondary index requires two traversals: one in the secondary index and a second in the clustered index, known as a Bookmark or Key Lookup."
- **Under the Hood**:
  - Clustered Key: By default the Primary Key. If absent, engines like MySQL InnoDB pick the first Unique Not-Null key or generate a hidden 6-byte row ID.
  - Secondary Index leaf payload: `(Secondary Key Columns, Clustered Primary Key)`.
- **Follow-Up**: *How do you avoid the Bookmark Lookup?* By creating a **Covering Index** that includes all columns requested by the query.

---

### Q5: Why do production databases use B+ Trees instead of B-Trees or Hash Tables for indexing?
- **How to Say It Out Loud**:
  "First, B+ Trees store data records exclusively in leaf nodes, keeping internal nodes slim with only keys and child pointers. This creates massive fan-out (order M ~ 1000), allowing a tree height of just 3 to 4 levels to index billions of rows with 3 to 4 disk I/Os. Second, all leaf nodes are linked in a continuous doubly-linked list, making range queries (`BETWEEN`, `>`, `<`) fast sequential scans. Hash tables cannot do range scans ($O(N)$), and standard B-Trees require expensive in-order tree traversals across random disk blocks."
- **Under the Hood**:
  A 16KB database page holding 8-byte keys and 8-byte pointers can store ~1,000 keys per internal node. At height 3: $1000^3 = 1 \text{ billion rows}$.
- **Follow-Up**: *When is a Hash index preferred?* In-memory key-value stores (Redis) or memory tables with pure equality lookups (`WHERE id = 5`).

---

### Q6: What is a Covering Index, and how does it eliminate "Bookmark / Key Lookups"?
- **How to Say It Out Loud**:
  "A Covering Index is an index that contains all the columns requested by a query (in `SELECT`, `WHERE`, `JOIN`, and `ORDER BY`). Because all required data resides directly inside the leaf nodes of the secondary index, the engine returns the result immediately without performing a secondary lookup into the clustered index."
- **Under the Hood**:
  - Query: `SELECT id, status, created_at FROM orders WHERE status = 'PENDING';`
  - Index: `CREATE INDEX idx_orders_status_created ON orders(status, created_at);`
  - Since `id` is the primary key (automatically present in InnoDB secondary leaves) and `status` + `created_at` are in the index, the query is 100% covered. In `EXPLAIN`, this appears as `Using index`.
- **Follow-Up**: *What is the cost of covering indexes?* Increased index size in RAM buffer pool and slower writes on `INSERT`/`UPDATE`.

---

### Q7: Explain the Leftmost Prefix Rule for composite indexes with a practical query example.
- **How to Say It Out Loud**:
  "A composite index on `(A, B, C)` is physically sorted first by A, then by B within each A, and by C within each B. Therefore, queries can only use the index if they filter starting from the leftmost column without skipping. A query filtering on `A` or `(A, B)` or `(A, B, C)` uses the index. A query filtering on `B` or `C` or `(B, C)` cannot use the index and triggers a full table scan."
- **Under the Hood**:
  Think of a phonebook sorted by `(LastName, FirstName)`. You can find everyone with last name 'Smith' or 'Smith, John'. But you cannot search for everyone named 'John' without reading the entire phonebook.
- **Follow-Up**: *Can `WHERE A = 1 AND C = 3` use the index?* Yes, but only column `A` uses the index for lookup; column `C` is evaluated as an index condition filter (Index Condition Pushdown).

---

### Q8: If an index exists on `(A, B)`, why can't a query with `WHERE A > 10 AND B = 5` utilize the index on column `B`?
- **How to Say It Out Loud**:
  "Because `A` is filtered using a range operator (`>`), multiple distinct values of `A` are returned. Within different values of `A`, the values of `B` are not in global sorted order. The engine uses the index to jump to `A = 11`, but after that point, `B` values are scattered across all qualifying `A` nodes, meaning `B` cannot be used for binary search and must be filtered row-by-row."
- **Under the Hood**:
  Rule: A composite index stops being usable for index lookup after the first range comparison (`<`, `>`, `BETWEEN`, `LIKE 'prefix%'`).
- **Follow-Up**: *How do you fix this?* If `B` has high cardinality and is always equality-checked, define the index as `(B, A)` instead.

---

### Q9: Why shouldn't you index every column in a table? What are the write-time and memory penalties?
- **How to Say It Out Loud**:
  "Every index adds write overhead: every `INSERT`, `UPDATE`, and `DELETE` must synchronously update all corresponding B+ trees, causing page splits, WAL writes, and random I/O. Furthermore, indexes consume significant RAM inside the database buffer pool, evicting cached data pages and lowering overall cache hit ratios."
- **Under the Hood**:
  Page splits occur when an insert hits a full 16KB page, requiring allocating a new page, moving half the keys, and updating parent pointers.
- **Follow-Up**: *What is index selectivity?* Selectivity = `COUNT(DISTINCT col) / COUNT(*)`. Index columns with high selectivity (near 1.0, e.g. emails); avoid low selectivity columns (e.g. `gender` where 50% match).

---

### Q10: What is the difference between a Dirty Read, a Non-Repeatable (Fuzzy) Read, and a Phantom Read?
- **How to Say It Out Loud**:
  "A Dirty Read occurs when a transaction reads uncommitted changes from another transaction that might later rollback. A Non-Repeatable Read occurs when a transaction re-reads the same row and discovers updated values because another transaction modified and committed it. A Phantom Read occurs when a transaction re-executes a range query and discovers new rows added or deleted by another committed transaction."
- **Under the Hood**:
  - Dirty Read: Read uncommitted row.
  - Non-Repeatable Read: Read committed *UPDATE*.
  - Phantom Read: Read committed *INSERT / DELETE* matching a range.
- **Follow-Up**: *Which level prevents each?* Read Committed prevents Dirty Reads. Repeatable Read prevents Non-Repeatable Reads. Serializable prevents Phantom Reads.

---

### Q11: What are the four ANSI SQL isolation levels, and what concurrency anomalies does each prevent?
- **How to Say It Out Loud**:
  "From lowest to highest strictness:
  1. Read Uncommitted: Prevents nothing; allows dirty reads.
  2. Read Committed: Prevents dirty reads using short-lived read locks or snapshot read.
  3. Repeatable Read: Prevents dirty reads and non-repeatable reads by holding read locks until commit or pinning an MVCC snapshot for the whole transaction.
  4. Serializable: Prevents all anomalies including phantom reads, typically using Range Locks or Strict Two-Phase Locking."
- **Under the Hood**:
  | Isolation Level | Dirty Read | Non-Repeatable Read | Phantom Read |
  |---|---|---|---|
  | Read Uncommitted | Allowed | Allowed | Allowed |
  | Read Committed | Prevented | Allowed | Allowed |
  | Repeatable Read | Prevented | Prevented | Allowed (SQL standard) |
  | Serializable | Prevented | Prevented | Prevented |
- **Follow-Up**: *What is the default isolation level in MySQL vs. PostgreSQL?* MySQL InnoDB defaults to Repeatable Read; PostgreSQL defaults to Read Committed.

---

### Q12: How does MySQL InnoDB prevent Phantom Reads in Repeatable Read isolation level?
- **How to Say It Out Loud**:
  "MySQL InnoDB prevents phantom reads through two mechanisms: for non-locking snapshot reads (`SELECT`), it uses **MVCC**, where a transaction reads rows from its initial snapshot timestamp; for locking reads (`SELECT ... FOR UPDATE`), it uses **Next-Key Locking**, which combines a Record Lock on the index row with a Gap Lock on the space between index rows, preventing other transactions from inserting new records into the range."
- **Under the Hood**:
  If index has values 10 and 20, a Next-Key Lock on 20 locks the interval `(10, 20]`. Any attempt by another transaction to insert 15 blocks until the lock is released.
- **Follow-Up**: *What is the downside of Gap Locks?* Increased risk of database deadlocks when concurrent inserts lock overlapping gaps.

---

### Q13: What is Multi-Version Concurrency Control (MVCC), and why is it preferred over 2-Phase Locking for reads?
- **How to Say It Out Loud**:
  "MVCC's core principle is that *readers never block writers, and writers never block readers*. Instead of acquiring shared read locks on data pages, updating a row creates a new version tagged with a transaction ID in the undo log. Queries read the consistent version visible at their transaction start timestamp, eliminating read lock contention and drastically boosting concurrency."
- **Under the Hood**:
  Each row in InnoDB contains hidden columns: `DB_TRX_ID` (transaction ID that created/modified it) and `DB_ROLL_PTR` (pointer to the undo log record containing previous version).
- **Follow-Up**: *When do undo logs get deleted?* During the **Purge** process, once no active transaction's read view requires the old versions.

---

### Q14: What is the difference between Pessimistic Locking (`SELECT ... FOR UPDATE`) and Optimistic Concurrency Control (OCC)?
- **How to Say It Out Loud**:
  "Pessimistic Locking assumes conflicts are frequent and acquires an exclusive lock immediately upon reading (`SELECT ... FOR UPDATE`), blocking others until commit. It is ideal for high-write contention like banking transfers. Optimistic Concurrency Control assumes conflicts are rare, performs reads without locks, and includes a version number column. On update, it executes `UPDATE ... WHERE id = 1 AND version = @oldVersion`. If zero rows are updated, it aborts or retries. OCC is best for read-heavy web applications and distributed systems."
- **Under the Hood**:
  Pessimistic locking risks lock timeouts and deadlocks; OCC avoids locks but incurs retry overhead under high write contention.
- **Follow-Up**: *How do you prevent starvation in OCC?* Exponential backoff with jitter on retries.

---

### Q15: What is Two-Phase Locking (2PL), and how does it fundamentally differ from Two-Phase Commit (2PC)?
- **How to Say It Out Loud**:
  "Two-Phase Locking (2PL) is a concurrency control protocol on a single database node ensuring serializability via a Growing Phase (acquiring locks without releasing) and a Shrinking Phase (releasing locks without acquiring new ones). Two-Phase Commit (2PC) is an atomic commitment consensus protocol across multiple distributed database nodes via a coordinator using a Prepare phase and a Commit phase. 2PL is about *concurrency*; 2PC is about *distributed consensus*."
- **Under the Hood**:
  - *Strict 2PL*: Holds all exclusive locks until commit/rollback to prevent cascading aborts.
  - *2PC*: Phase 1: Coordinator asks nodes 'Can you commit?'. Phase 2: If all say YES, send 'Commit'; if any say NO, send 'Rollback'.
- **Follow-Up**: *What is the flaw in 2PC?* Blocking protocol: if the coordinator crashes during Phase 2, participants remain blocked holding locks.

---

### Q16: What is Write Skew, and why doesn't Snapshot Isolation prevent it?
- **How to Say It Out Loud**:
  "Write Skew is a concurrency anomaly under Snapshot Isolation where two concurrent transactions read overlapping data, satisfy local constraints based on their snapshot, and make disjoint updates that together violate a global constraint. For example, two on-call doctors simultaneously request off because each sees two active doctors, resulting in zero doctors on call. Snapshot Isolation only detects write-write conflicts on the *same* row; because they modified *different* rows, both transactions succeed."
- **Under the Hood**:
  Solution: Elevate to full `Serializable` isolation or manually lock the overlapping rows using `SELECT ... FOR UPDATE`.
- **Follow-Up**: *Is Snapshot Isolation the same as Serializable?* No, Snapshot Isolation permits Write Skew.

---

### Q17: Explain 1NF, 2NF, and 3NF with a concrete table decomposition example.
- **How to Say It Out Loud**:
  "1NF requires atomic values in every column with a defined primary key (no comma-separated lists). 2NF requires 1NF plus no partial dependency: every non-key column must depend on the whole composite primary key. 3NF requires 2NF plus no transitive dependency: non-key columns must depend only on the primary key, never on other non-key columns."
- **Under the Hood**:
  - *Unnormalized*: `(StudentID, CourseID, StudentName, CourseName, InstructorName, InstructorOffice)`
  - *Violates 2NF*: `StudentName` depends only on `StudentID`, not `(StudentID, CourseID)`.
    *Fix*: Split into `Students(StudentID, StudentName)` and `Enrollments(StudentID, CourseID)`.
  - *Violates 3NF*: In `Courses(CourseID, InstructorName, InstructorOffice)`, `InstructorOffice` depends on `InstructorName` (non-key $\to$ non-key).
    *Fix*: Split into `Courses(CourseID, InstructorName)` and `Instructors(InstructorName, InstructorOffice)`.
- **Follow-Up**: *What anomalies does 3NF eliminate?* Insertion, Update, and Deletion anomalies.

---

### Q18: What is Boyce-Codd Normal Form (BCNF), and when does a 3NF table fail to satisfy BCNF?
- **How to Say It Out Loud**:
  "BCNF is a stricter version of 3NF. In 3NF, for any functional dependency $X \to Y$, $X$ must be a superkey OR $Y$ must be a prime attribute (part of a candidate key). BCNF removes the second condition: for every functional dependency $X \to Y$, $X$ MUST be a superkey. A 3NF table fails BCNF when it has multiple overlapping candidate keys."
- **Under the Hood**:
  Example: `(Student, Subject, Professor)` where each professor teaches only one subject ($Professor \to Subject$), but a subject can have multiple professors. Candidate keys: `(Student, Subject)` and `(Student, Professor)`. In $Professor \to Subject$, `Professor` is not a superkey, violating BCNF.
- **Follow-Up**: *Why not always decompose to BCNF?* BCNF decomposition may not preserve functional dependencies, requiring expensive multi-table checks.

---

### Q19: When and why would you deliberately Denormalize a database in a high-throughput production system?
- **How to Say It Out Loud**:
  "You denormalize in high-throughput, read-heavy production services to eliminate expensive multi-table joins and aggregations. By duplicating columns (like embedding `username` directly into an `orders` table), read queries become single-table lookups. The trade-off is higher disk usage and the application burden of updating duplicated data across multiple rows on write."
- **Under the Hood**:
  Used heavily in reporting databases, read-heavy feeds, and microservice read models (CQRS pattern).
- **Follow-Up**: *How do you handle the data inconsistency risk?* Use background event queues (Kafka) or database change data capture (CDC via Debezium) to sync redundant data asynchronously.

---

### Q20: Explain the three physical JOIN algorithms: Nested Loop Join, Hash Join, and Sort-Merge Join.
- **How to Say It Out Loud**:
  "Nested Loop Join iterates through the outer table and probes the inner table row-by-row; it is $O(M \times N)$ naively, but $O(M \log N)$ if the inner table has an index. Hash Join builds an in-memory hash table on the smaller table's join key and scans the larger table probing for matches ($O(M + N)$); it is best for large unsorted equality joins. Sort-Merge Join sorts both tables on the join key (if not already indexed) and merges them linearly ($O(M \log M + N \log N)$); it is optimal when inputs are already sorted or for range comparisons."
- **Under the Hood**:
  - Nested Loop: Best for small datasets or OLTP queries with indexed primary/foreign keys.
  - Hash Join: Cannot do range joins (`<`, `>`).
  - Sort-Merge: Best for large datasets exceeding memory (external disk merge-sort).
- **Follow-Up**: *What is a Block Nested Loop Join?* Loads chunks of outer rows into a memory buffer to scan the inner table once per chunk rather than once per row.

---

### Q21: What is the difference between `DELETE`, `TRUNCATE`, and `DROP` in terms of DDL vs. DML, logging, and rollback?
- **How to Say It Out Loud**:
  "`DELETE` is a DML command that removes specified rows one-by-one, firing triggers and recording each row deletion in the transaction log; it can be rolled back and is slowest. `TRUNCATE` is a DDL command that deallocates entire data pages at the storage level, does not fire row triggers, resets auto-increment counters, and is fast. `DROP` is a DDL command that completely destroys the table definition, data, constraints, and indexes from the database catalog."
- **Under the Hood**:
  - `DELETE`: High undo/redo log generation.
  - `TRUNCATE`: Minimal logging (logs page deallocations, not individual rows).
  - Can you rollback `TRUNCATE`? In SQL Server and PostgreSQL, yes, if executed within a transaction block. In MySQL, `TRUNCATE` causes an implicit commit and cannot be rolled back.
- **Follow-Up**: *Which command reclaims disk space immediately?* `TRUNCATE` and `DROP` deallocate pages back to the OS or tablespace; `DELETE` leaves empty space in pages for reuse.

---

### Q22: What is the difference between a Primary Key and a Unique Key?
- **How to Say It Out Loud**:
  "A Primary Key uniquely identifies a row, cannot contain NULL values, has exactly one instance per table, and automatically generates a clustered index by default. A Unique Key enforces column uniqueness, permits NULL values (one or more depending on database engine), allows multiple unique constraints per table, and generates a non-clustered index by default."
- **Under the Hood**:
  SQL standard allows multiple NULLs in a Unique Key because `NULL != NULL`. In SQL Server, only one NULL is allowed; in PostgreSQL and MySQL, multiple NULLs are permitted.
- **Follow-Up**: *Can a Primary Key be changed?* Yes, but modifying a clustered primary key requires updating every secondary index pointing to it, causing heavy overhead.

---

### Q23: What is a View vs. a Materialized View, and when should you use a Materialized View?
- **How to Say It Out Loud**:
  "A standard View is a virtual table: it stores only the SQL query definition, executing the underlying query in real-time whenever accessed without consuming storage. A Materialized View physically stores the computed query results on disk like a real table. You use Materialized Views in analytical and reporting systems where complex joins and aggregations on millions of rows would be too slow to calculate on every request."
- **Under the Hood**:
  Materialized views must be refreshed: either periodically via cron, on-demand, or incrementally on write using triggers or binlogs.
- **Follow-Up**: *What is the tradeoff of Materialized Views?* Stale data between refreshes and storage/CPU cost during refresh operations.

---

### Q24: What is the difference between Database Sharding and Read Replicas?
- **How to Say It Out Loud**:
  "Read Replicas copy the entire database to secondary servers to scale read throughput asynchronously; all writes still hit the single primary database. Sharding is horizontal partitioning of both reads and writes across multiple independent database servers based on a Shard Key. Read replicas scale read capacity; sharding scales write throughput and overall data storage capacity."
- **Under the Hood**:
  - Read Replicas: Master-replica with async binlog replication.
  - Sharding: Requires an application routing layer (e.g. Vitess, Citus) to hash keys to shards.
- **Follow-Up**: *What is the hardest problem with sharding?* Cross-shard transactions, cross-shard joins, and re-sharding when expanding from $N$ to $2N$ shards.

---

### Q25: What is Replication Lag in a Primary-Replica architecture, and how do you handle it in application design?
- **How to Say It Out Loud**:
  "Replication Lag is the time delay between a write committing on the primary and that modification being replayed on read replicas due to network latency or replica workload. If a user updates their profile and immediately refreshes, they might see old data (stale read). To handle this, use **Read-Your-Own-Writes routing**: route queries for recently updated user data directly to the primary database for 5 seconds, while routing all other reads to replicas."
- **Under the Hood**:
  Track transaction timestamps in a user session cookie. If `cookie.last_write_timestamp > replica_current_timestamp`, redirect query to primary.
- **Follow-Up**: *What is synchronous replication?* Primary waits for replica ACK before committing. Eliminates lag, but increases write latency and decreases availability if replica dies.

---

### Q26: How do you choose a good Shard Key, and what happens if you choose poorly (Shard Hotspotting)?
- **How to Say It Out Loud**:
  "A good shard key has high cardinality, distributes read and write traffic uniformly across all shards, and aligns with your most frequent query patterns so queries target a single shard. A poor choice leads to Shard Hotspotting, where one shard handles 90% of traffic (e.g., sharding by date where all writes hit today's shard, or by tenant where a celebrity tenant overwhelms one shard)."
- **Under the Hood**:
  - Good key: `hash(user_id)`. Uniform distribution.
  - Bad key: `created_at` (monotonically increasing, writes hit single node) or `country` (USA shard overloaded, Iceland shard idle).
- **Follow-Up**: *How do you fix celebrity tenant hotspotting?* Salt the shard key: `concat(celebrity_id, "_", random_int(0, 9))` to spread writes across 10 shards, then aggregate on read.

---

### Q27: When would you choose a NoSQL database over a Relational SQL database?
- **How to Say It Out Loud**:
  "Choose NoSQL when you need:
  1. Massive horizontal write throughput and petabyte-scale storage with automatic sharding.
  2. Highly flexible, dynamic, or hierarchical nested schemas (like JSON documents).
  3. Simple access patterns by primary key where complex multi-table joins are unnecessary.
  Choose SQL when you need strict ACID compliance, complex relational queries, foreign key integrity, and zero data redundancy (financial transactions)."
- **Under the Hood**:
  SQL scales vertically (CPU/RAM); NoSQL scales horizontally (commodity hardware clusters).
- **Follow-Up**: *Can SQL handle JSON?* Yes, modern PostgreSQL and MySQL have native `JSONB` columns with GIN indexing, bridging the gap for hybrid use cases.

---

### Q28: What are the trade-offs between Key-Value, Document, Wide-Column, and Graph databases?
- **How to Say It Out Loud**:
  "Key-Value stores (Redis) are blazing fast in-memory stores for caching and sessions, but lack query flexibility beyond the key. Document stores (MongoDB) store flexible JSON documents, ideal for content management and catalogs. Wide-Column stores (Cassandra) organize data by column families, optimized for massive write throughput and time-series data across clusters. Graph databases (Neo4j) store nodes and relationships, optimized for deep graph traversals like social networks and recommendation engines."
- **Under the Hood**:
  - Key-Value: $O(1)$ read/write.
  - Document: Secondary indexes on JSON fields.
  - Wide-Column: LSM-trees (Log-Structured Merge-trees) appending writes to disk.
  - Graph: Index-free adjacency (nodes point directly to neighbors in memory).
- **Follow-Up**: *What is an LSM-tree?* Write-optimized data structure buffering writes in memory (MemTable) and flushing sequentially to immutable disk files (SSTables).

---

### Q29: What is Eventual Consistency vs. Strong Consistency, and where does the CAP Theorem fit in?
- **How to Say It Out Loud**:
  "Strong Consistency guarantees that any read returns the most recent write immediately across all replicas. Eventual Consistency guarantees that, in the absence of new writes, all replicas will eventually converge to the same value. In CAP Theorem, when a Network Partition occurs (P), a distributed system must choose between Consistency (C - reject writes if replicas cannot sync) or Availability (A - accept writes on all partitions, leading to temporary eventual consistency)."
- **Under the Hood**:
  - CP Systems: MongoDB, ZooKeeper, Google Spanner (with TrueTime).
  - AP Systems: Cassandra, DynamoDB, DNS.
- **Follow-Up**: *What is PACELC theorem?* Extends CAP: If there is a Partition (P), choose Availability (A) or Consistency (C); Else (E), choose Latency (L) or Consistency (C).

---

### Q30: How do you detect and resolve a Deadlock in a relational database?
- **How to Say It Out Loud**:
  "Database engines detect deadlocks by maintaining a Wait-For Graph (where transactions are vertices and lock requests are directed edges). A background thread periodically checks for cycles. When a cycle is detected, the engine breaks the deadlock by aborting the transaction with the lowest rollback cost (the Victim), issuing a deadlock error and allowing the other transaction to complete."
- **Under the Hood**:
  - Wait-For Graph: Cycle = Deadlock ($T_1 \to T_2 \to T_1$).
  - Prevention: Always acquire locks in the exact same predefined order across application services, keep transactions small, and use `SELECT ... FOR UPDATE` with explicit timeouts.
- **Follow-Up**: *What error code does MySQL return on deadlock?* Error 1213 (`ER_LOCK_DEADLOCK`). Application code must catch this error and retry the transaction with jitter.
