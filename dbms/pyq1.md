# SQL & DBMS Master Interview Q&A — Part 1 (Q1–Q50) 🚀

This document compiles the first 50 of the 100 most-asked theoretical and practical database interview questions. These questions are tailored to the style of top-tier companies (Google, Meta, Amazon, Netflix, Stripe, Uber, etc.), focusing on core architecture, edge cases, transactional internals, and query execution.

---

## Section 1: Relational Model & ER Design (Q1–Q4)

### Q1. What is the difference between a Super Key, Candidate Key, and Primary Key?
* **Asked by:** Google, Amazon, Microsoft
* **Answer:**
  * **Super Key:** A set of one or more attributes that uniquely identifies a tuple (row) in a relation. It can contain extra/redundant attributes.
  * **Candidate Key:** A *minimal* Super Key. No proper subset of a candidate key can uniquely identify a tuple.
  * **Primary Key:** The candidate key chosen by the database designer to uniquely identify rows in a table. It cannot contain `NULL` values.
* **Pro-Tip:** If the candidate key is `{StudentID}`, super keys could be `{StudentID}`, `{StudentID, Name}`, `{StudentID, Email}`. In interviews, emphasize that a candidate key is minimal—removing even one attribute breaks its uniqueness.

---

### Q2. What is Referential Integrity and how is it enforced?
* **Asked by:** Amazon, Meta, Salesforce
* **Answer:**
  Referential Integrity guarantees that a foreign key value in a child table must match an existing primary key value in the parent table, or be `NULL`. It prevents "orphaned rows" and dangling pointers.
  * **Enforcement:** Enforced using `FOREIGN KEY` constraints.
  * **Referential Actions:**
    * `ON DELETE CASCADE`: Deletes child rows when the parent row is deleted.
    * `ON DELETE SET NULL`: Sets child foreign key columns to NULL.
    * `ON DELETE RESTRICT` / `NO ACTION`: Rejects deletion of parent row if child rows exist.

---

### Q3. What is a Weak Entity Set, and how do you map it to a relational table?
* **Asked by:** Oracle, Goldman Sachs
* **Answer:**
  * A **Weak Entity Set** is an entity set that does not possess sufficient attributes to form a primary key. It depends on a **strong entity set** (identifying owner) for its existence.
  * It has a **partial key** (or discriminator), marked with a dashed underline in ER diagrams.
  * **Mapping Rule:** When mapped to a table, the primary key of the weak entity table is a combination of the **owner's primary key** + the **weak entity's partial key**.
  * **Example:** `Dependent` (weak entity) associated with `Employee` (owner). Table schema: `Dependent(EmployeeID, DependentName, Age)`, where the primary key is `(EmployeeID, DependentName)`.

---

### Q4. What is the difference between DDL, DML, DCL, and TCL?
* **Asked by:** Uber, Snowflake, Flipkart
* **Answer:**
  * **DDL (Data Definition Language):** Defines/modifies database structures (schemas). Commands: `CREATE`, `ALTER`, `DROP`, `TRUNCATE`. DDL is auto-committed in most engines.
  * **DML (Data Manipulation Language):** Retrieves, inserts, updates, and deletes data values. Commands: `SELECT`, `INSERT`, `UPDATE`, `DELETE`.
  * **DCL (Data Control Language):** Manages user privileges and access control. Commands: `GRANT`, `REVOKE`.
  * **TCL (Transaction Control Language):** Manages transactional boundaries. Commands: `COMMIT`, `ROLLBACK`, `SAVEPOINT`.

---

## Section 2: SQL Query Execution & Concepts (Q5–Q13)

### Q5. How does a database logically process a SELECT query? (Execution Order)
* **Asked by:** Meta, Stripe, Netflix
* **Answer:**
  SQL is written declaratively, but the query engine processes it in a strict logical order:
  1. `FROM` & `JOIN` (Gathers and matches target tables)
  2. `WHERE` (Filters raw rows)
  3. `GROUP BY` (Collapses rows into aggregate groups)
  4. `HAVING` (Filters aggregated groups)
  5. `SELECT` (Computes projected columns and window functions)
  6. `DISTINCT` (Deduplicates results)
  7. `ORDER BY` (Sorts output)
  8. `LIMIT` / `OFFSET` (Slices result window)
* **Interview Trap:** Because `SELECT` runs *after* `WHERE` and `GROUP BY`, you cannot use column aliases defined in `SELECT` inside `WHERE` or `GROUP BY` clauses.

---

### Q6. Explain the difference between INNER, LEFT, RIGHT, FULL, and CROSS JOINs.
* **Asked by:** Amazon, Google, Zepto
* **Answer:**
  * **`INNER JOIN`**: Returns only rows with matching keys in both tables.
  * **`LEFT JOIN`**: Returns all rows from the left table + matching rows from the right. Unmatched right rows return `NULL`.
  * **`RIGHT JOIN`**: Returns all rows from the right table + matching rows from the left. Unmatched left rows return `NULL`.
  * **`FULL JOIN`**: Returns all rows from both tables, filling with `NULL` where matches are missing.
  * **`CROSS JOIN`**: Returns the Cartesian Product (combines every row of A with every row of B; `M × N` rows). No `ON` clause is used.

---

### Q7. What is a Self-Join? Give a practical interview scenario where you would use it.
* **Asked by:** Uber, Stripe, Apple
* **Answer:**
  A **Self-Join** joins a table to itself. It requires distinct alias names (e.g., `FROM Employee e1 JOIN Employee e2`) to avoid naming collisions.
  * **Scenario:** Finding employee-manager hierarchies, or comparing events in the same log stream.
  * **Example Query:** Find employees who earn more than their direct managers.
    ```sql
    SELECT e.name AS employee_name
    FROM Employee e
    JOIN Employee m ON e.manager_id = m.id
    WHERE e.salary > m.salary;
    ```

---

### Q8. What is the difference between the WHERE and HAVING clauses?
* **Asked by:** Walmart, Flipkart, Adobe
* **Answer:**
  * **`WHERE`**: Filters individual records *before* they are grouped or aggregated. It cannot contain aggregate functions (e.g., `WHERE SUM(salary) > 100` is invalid).
  * **`HAVING`**: Filters groups *after* `GROUP BY` aggregation has occurred. It is used exclusively with aggregate functions (e.g., `HAVING COUNT(*) > 5`).

---

### Q9. Explain the difference between IN and EXISTS. Which is faster?
* **Asked by:** Google, Amazon, Microsoft
* **Answer:**
  * **`IN`**: Evaluates the subquery first, builds an in-memory hash set of the results, and matches the outer query value.
  * **`EXISTS`**: Returns `TRUE` as soon as a single matching row is found in the subquery (short-circuit evaluation).
  * **Performance:**
    * **Inner Query Large:** Use `EXISTS` because it stops scanning early.
    * **Inner Query Small:** Use `IN` because the in-memory lookup is extremely fast.
    * **NULLs:** `IN` handles NULLs poorly (e.g., `NOT IN` returns nothing if the subquery contains a NULL), whereas `EXISTS` is null-safe.

---

### Q10. What are Window Functions? How do they differ from GROUP BY?
* **Asked by:** Meta, Stripe, Netflix
* **Answer:**
  * **`GROUP BY`** collapses individual rows into a single aggregated row, discarding row-level detail.
  * **Window Functions** perform aggregates and ranking calculations over a set of rows (the "window") related to the current row, **retaining the individual identity and detail of each row**.
  * **Syntax:** `SUM(amount) OVER (PARTITION BY user_id ORDER BY transaction_date)`

---

### Q11. What is the difference between ROW_NUMBER(), RANK(), and DENSE_RANK()?
* **Asked by:** Netflix, Stripe, Zepto
* **Answer:**
  * **`ROW_NUMBER()`**: Assigns a unique sequential integer to each row in the partition, starting at 1. In case of ties, ranks are assigned arbitrarily.
  * **`RANK()`**: Assigns ranks with gaps if there are ties. If two items tie for rank 1, the next rank is 3 (e.g., `1, 1, 3, 4`).
  * **`DENSE_RANK()`**: Assigns ranks *without* gaps. If two items tie for rank 1, the next rank is 2 (e.g., `1, 1, 2, 3`).

---

### Q12. What are Common Table Expressions (CTEs)? When are they preferred over Subqueries?
* **Asked by:** Amazon, Uber, Snowflake
* **Answer:**
  A **CTE** is a temporary result set defined using the `WITH` clause that can be referenced within a `SELECT`, `INSERT`, `UPDATE`, or `DELETE` statement.
  * **Advantages over Subqueries:**
    * **Readability:** Follows a logical top-down sequence rather than nested "inside-out" subqueries.
    * **Reusability:** A CTE can be joined multiple times in the same query.
    * **Recursion:** CTEs can reference themselves (`WITH RECURSIVE`) to query hierarchical structures.

---

### Q13. What is a Recursive CTE? Explain its core components.
* **Asked by:** Meta, Stripe, Netflix
* **Answer:**
  A **Recursive CTE** references itself to traverse nested, hierarchical, or graph-structured datasets (e.g., org charts, network routing, folder trees).
  * **Core Components:**
    1. **Anchor Member:** The base query that establishes the starting result set (executed once).
    2. **Recursive Member:** A query joined to the CTE itself using `UNION` or `UNION ALL`, executing iteratively until no new rows are returned.
    3. **Termination Condition:** Evaluated implicitly when the recursive step returns empty.

---

## Section 3: Database Normalization & Design (Q14–Q20)

### Q14. What is database normalization? What anomalies does it prevent?
* **Asked by:** Microsoft, Oracle, Salesforce
* **Answer:**
  **Normalization** is the process of decomposing database relations to minimize data redundancy and maintain data integrity. It prevents three critical anomalies:
  1. **Insertion Anomaly:** Inability to insert new data because some other unrelated data is missing.
  2. **Update Anomaly:** Inconsistencies caused when data values are updated in one place but not in duplicated rows.
  3. **Deletion Anomaly:** Accidental loss of unrelated data caused by deleting a row containing combined details.

---

### Q15. Explain 1NF, 2NF, and 3NF with simple examples.
* **Asked by:** Amazon, Google, Walmart
* **Answer:**
  * **1NF (First Normal Form):** All attribute values must be **atomic** (no multi-valued columns or repeating groups).
    * *Example:* Splitting a comma-separated `phone_numbers` string into separate rows.
  * **2NF (Second Normal Form):** Must be in 1NF + **no partial dependencies** (no non-prime attribute can depend on a proper subset of a composite candidate key).
    * *Example:* If key is `(StudentID, CourseID)`, an attribute like `StudentName` depends only on `StudentID`, violating 2NF. Move it to a `Student` table.
  * **3NF (Third Normal Form):** Must be in 2NF + **no transitive dependencies** (no non-prime attribute can determine another non-prime attribute).
    * *Example:* `StudentID → DeptID → DeptHOD`. `DeptHOD` depends transitively on `StudentID`. Split into `Student(StudentID, DeptID)` and `Department(DeptID, DeptHOD)`.

---

### Q16. What is Boyce-Codd Normal Form (BCNF)? How does it differ from 3NF?
* **Asked by:** Google, Amazon, Microsoft
* **Answer:**
  * **BCNF** is a stricter version of 3NF.
  * **Rule:** For every non-trivial functional dependency $X \rightarrow Y$, **$X$ must be a super key**.
  * **Difference:** 3NF has an escape clause: if $X$ is not a super key, the dependency is still allowed if $Y$ is a **prime attribute** (part of a candidate key). BCNF removes this escape clause.
  * **Example:** `CourseTeacher(StudentID, Course, Teacher)` with keys `{StudentID, Course}` and `{StudentID, Teacher}`. If `Teacher → Course`, this table is in 3NF (Course is prime) but violates BCNF (Teacher is not a super key).

---

### Q17. What is a Functional Dependency (FD) and Attribute Closure ($X^+$)?
* **Asked by:** Oracle, Adobe
* **Answer:**
  * **Functional Dependency ($X \rightarrow Y$):** A constraint indicating that a set of attributes $X$ uniquely determines the values of another set of attributes $Y$.
  * **Attribute Closure ($X^+$):** The set of all attributes that can be functionally determined by $X$, directly or transitively, given a set of FDs.
  * **Use:** Attribute closure is used to find candidate keys. If $X^+$ contains all attributes in the relation $R$, then $X$ is a super key.

---

### Q18. What are Armstrong's Axioms in functional dependency?
* **Asked by:** Academic/Core Engineering tests (e.g., gate/interviews)
* **Answer:**
  Armstrong's Axioms are a set of rules used to infer all functional dependencies that hold in a relational database. They are **sound** (never generate incorrect FDs) and **complete** (can generate all valid FDs).
  * **Primary Rules:**
    1. **Reflexivity:** If $Y \subseteq X$, then $X \rightarrow Y$.
    2. **Augmentation:** If $X \rightarrow Y$, then $XZ \rightarrow YZ$.
    3. **Transitivity:** If $X \rightarrow Y$ and $Y \rightarrow Z$, then $X \rightarrow Z$.
  * **Derived Rules:** Union, Decomposition, Pseudo-transitivity.

---

### Q19. What is a Canonical Cover ($F_c$)? Why is it useful?
* **Asked by:** Salesforce, Oracle
* **Answer:**
  A **Canonical Cover** of a set of functional dependencies $F$ is a simplified, minimal set of FDs equivalent to $F$ that has:
  1. No redundant FDs.
  2. No extraneous attributes on the left-hand or right-hand side of any FD.
  3. Unique left-hand sides (combined into a single FD per determinant).
  * **Usefulness:** It forms the starting point for 3NF synthesis algorithms, ensuring we design schemas with the minimum number of tables possible.

---

### Q20. What is the difference between Lossless-Join and Dependency-Preserving Decomposition?
* **Asked by:** Google, Amazon, Microsoft
* **Answer:**
  When decomposing a table $R$ into smaller tables $R_1, R_2$:
  * **Lossless-Join:** Guarantees that Joining $R_1$ and $R_2$ produces exactly the original table $R$, with **no spurious rows**. Tested by checking if $(R_1 \cap R_2)$ determines $R_1$ or $R_2$ (i.e., common columns must be a key in at least one decomposed table).
  * **Dependency-Preserving:** Guarantees that all original functional dependencies can be checked within individual tables without performing a JOIN.
  * **Trade-off:** 3NF guarantees both lossless-join and dependency preservation. BCNF guarantees lossless-join but may fail to preserve dependencies.

---

## Section 4: Transactions & Concurrency (Q21–Q28)

### Q21. What are the ACID properties in database transactions? Explain each.
* **Asked by:** All Tech Companies (Meta, Google, Uber, Amazon, etc.)
* **Answer:**
  * **Atomicity:** All operations within a transaction succeed, or the entire transaction is rolled back ("all or nothing"). Implemented using recovery logs (UNDO).
  * **Consistency:** The database transitions from one valid state to another, maintaining all schema constraints (keys, checks). Maintained by application logic and constraints.
  * **Isolation:** Uncommitted changes from one transaction are invisible to other transactions running concurrently. Implemented using concurrency control (locks, MVCC).
  * **Durability:** Once committed, the transaction's changes are permanent and survive system crashes. Implemented using WAL (Write-Ahead Logging) and checkpoints.

---

### Q22. What is Conflict Serializability? How is it tested?
* **Asked by:** Uber, Stripe, Netflix
* **Answer:**
  * A schedule is **Conflict Serializable** if it is conflict-equivalent to some serial schedule (transactions executed one after another).
  * **Conflicting Operations:** Two operations conflict if they belong to different transactions, access the same data item, and at least one of them is a **WRITE** (i.e., Read-Write, Write-Read, or Write-Write conflicts).
  * **Testing:** Construct a **Precedence Graph (Serialization Graph)**:
    * Nodes represent transactions ($T_1, T_2$).
    * Draw a directed edge $T_i \rightarrow T_j$ if an operation in $T_i$ conflicts with and occurs before an operation in $T_j$.
    * **Rule:** If the precedence graph is **acyclic (no cycles)**, the schedule is Conflict Serializable.

---

### Q23. What is View Serializability? Why is it NP-Complete to test?
* **Asked by:** Core systems roles (Snowflake, AWS RDS)
* **Answer:**
  * **View Serializability** is a weaker condition than conflict serializability. A schedule is view serializable if its read/write views match some serial schedule.
  * Every conflict serializable schedule is view serializable, but not vice versa (view serializability allows schedules with **blind writes**—writing without reading first).
  * **NP-Completeness:** Testing view serializability is NP-Complete because you must evaluate all possible permutations of serial schedules to find a matching view state, which grows exponentially ($N!$) with the number of transactions.

---

### Q24. Explain the difference between Recoverable and Cascadeless Schedules.
* **Asked by:** Amazon, Stripe, Google
* **Answer:**
  * **Recoverable Schedule:** If transaction $T_2$ reads a data item written by $T_1$, then the commit of $T_1$ must happen *before* the commit of $T_2$. This ensures that if $T_1$ aborts, we can roll back $T_2$.
  * **Cascadeless Schedule:** If $T_2$ reads a data item written by $T_1$, $T_1$ must commit *before* $T_2$ reads the data.
  * **Why it matters:** Cascadeless schedules prevent **Cascading Rollbacks** (where aborting one transaction forces rolling back dozens of dependent active transactions), which wastes CPU and locks database resources. Every cascadeless schedule is recoverable.

---

### Q25. What are the SQL Isolation Levels and the anomalies they prevent?
* **Asked by:** Meta, Stripe, Amazon, Salesforce
* **Answer:**
  SQL standard defines 4 isolation levels based on three read phenomena:

| Isolation Level | Dirty Read | Non-Repeatable Read | Phantom Read |
| :--- | :--- | :--- | :--- |
| **Read Uncommitted** | Allowed | Allowed | Allowed |
| **Read Committed** | Prevented | Allowed | Allowed |
| **Repeatable Read** | Prevented | Prevented | Allowed |
| **Serializable** | Prevented | Prevented | Prevented |

* **Read Anomalies:**
  * **Dirty Read:** Reading uncommitted data that is later rolled back.
  * **Non-Repeatable Read:** Reading the same row twice in one transaction and finding different data because another transaction modified it.
  * **Phantom Read:** Re-executing a range query and finding new "phantom" rows inserted by another transaction.

---

### Q26. What is the "NOT IN" NULL Trap in SQL? How do you fix it?
* **Asked by:** Meta, Amazon, Zepto
* **Answer:**
  If a subquery evaluated by `NOT IN` returns even a single `NULL` value, the entire query returns **zero rows**.
  * **Why:** In SQL, `id NOT IN (1, 2, NULL)` translates to `id != 1 AND id != 2 AND id != NULL`. Any comparison with `NULL` returns `UNKNOWN`. Therefore, the entire condition evaluates to `UNKNOWN`, filtering out all rows.
  * **Fix A:** Filter out NULLs explicitly in the subquery:
    ```sql
    WHERE id NOT IN (SELECT manager_id FROM Employee WHERE manager_id IS NOT NULL)
    ```
  * **Fix B:** Use `NOT EXISTS` instead (which uses 2-valued boolean logic and is null-safe):
    ```sql
    WHERE NOT EXISTS (SELECT 1 FROM Employee m WHERE m.manager_id = e.id)
    ```

---

### Q27. How does a database handle division by zero in SQL?
* **Asked by:** Stripe, Google, Finance companies
* **Answer:**
  * Attempting to divide a number by zero causes SQL queries to crash with a runtime error (e.g., `Division by zero` in Postgres/MySQL).
  * **Solution:** Use the `NULLIF(expression, value)` function. `NULLIF` returns `NULL` if the expression matches the value.
  * **Example:**
    ```sql
    SELECT total_revenue / NULLIF(total_clicks, 0) AS revenue_per_click
    ```
    If `total_clicks` is 0, the denominator becomes `NULL`, and any math division with `NULL` returns `NULL` instead of throwing an error.

---

### Q28. Explain the performance implications of UNION vs UNION ALL.
* **Asked by:** Zepto, Flipkart, Amazon
* **Answer:**
  * **`UNION ALL`**: Appends the results of two queries directly. It is highly efficient because it does not inspect the data for duplicates ($O(1)$ memory/CPU overhead).
  * **`UNION`**: Appends results and **deduplicates** them. It is slower because it must perform an implicit sorting or hashing phase ($O(N \log N)$ cost) to identify and discard duplicate rows.
  * **Pro-Tip:** Always default to `UNION ALL` in interviews unless you explicitly need duplicate removal.

---

## Section 5: Database Indexing & Query Tuning (Q29–Q36)

### Q29. What is the difference between a Clustered and Non-Clustered Index?
* **Asked by:** Uber, Microsoft, Oracle
* **Answer:**
  * **Clustered Index:**
    * The physical rows of the table are sorted and stored on disk in the index order.
    * Only **one** clustered index can exist per table (usually the primary key).
    * Leaf nodes contain the **actual data pages**.
  * **Non-Clustered Index:**
    * The physical storage order of rows is independent of the index.
    * Multiple non-clustered indexes can exist on a table.
    * Leaf nodes contain the **search key + a row identifier pointer** (RID/primary key value) to locate the actual row.

---

### Q30. Why do databases prefer B+ Trees over B-Trees for indexing?
* **Asked by:** Google, Amazon, Snowflake, Oracle
* **Answer:**
  Databases prefer B+ Trees for three primary reasons:
  1. **Larger Fan-Out:** B+ Trees store data records **only in leaf nodes**; internal nodes store only keys and routing pointers. This makes internal nodes smaller, allowing more keys per page (higher fan-out) and lowering tree height, which minimizes disk I/Os.
  2. **Range Query Efficiency:** Leaf nodes in a B+ Tree are **linked sequentially** (usually doubly-linked). Range scans only need to find the first key, then traverse the leaf pointers directly. B-Trees require traversing up and down tree levels.
  3. **Consistent Search Cost:** All lookups in a B+ Tree terminate at leaf nodes, ensuring a uniform time complexity of $O(\log N)$.

---

### Q31. What is a Sparse Index vs. a Dense Index?
* **Asked by:** Core database roles
* **Answer:**
  * **Dense Index:** Has an index record for **every single search-key value** in the data file. Fast lookup, but requires more memory/storage.
  * **Sparse Index:** Has an index record for only **some** of the search-key values (typically one entry per disk block/page). To find a record, locate the largest index entry $\le$ target key, load that block, and scan sequentially. Smaller footprint, but requires data to be physically sorted on disk.

---

### Q32. What is a Hash Index? When would you choose it over a B+ Tree?
* **Asked by:** AWS RDS team, Salesforce, Oracle
* **Answer:**
  * A **Hash Index** uses a hash function to map search keys to specific buckets, providing $O(1)$ constant time complexity for lookups.
  * **When to choose:** Choose it for **pure equality lookups** (e.g., `WHERE id = 100`).
  * **Why B+ Tree is usually preferred:** Hash indexes do not support range queries (e.g., `WHERE age > 21`), inequality filters, or sorting operations. Because the hash values are scattered randomly, range searches require scanning the entire table.

---

### Q33. How does the query optimizer decide between Index Scan vs. Full Table Scan?
* **Asked by:** Stripe, Netflix, Google
* **Answer:**
  The query optimizer uses database statistics (histograms) to estimate the **selectivity** of a query.
  * **High Selectivity (Filters many rows):** If a query returns a small fraction of the table (e.g., $<5\%$), the optimizer performs an **Index Scan** followed by row lookups.
  * **Low Selectivity (Filters few rows):** If the query returns a large fraction of the table (e.g., $>20\%$), fetching rows via index pointers causes random disk I/O, which is slower than sequentially reading the entire table. The optimizer will choose a **Full Table Scan (Sequential Scan)**.

---

### Q34. What is the difference between EXPLAIN and EXPLAIN ANALYZE?
* **Asked by:** Zepto, Uber, Stripe, Netflix
* **Answer:**
  * **`EXPLAIN`**: Shows the **estimated execution plan** generated by the query optimizer without actually running the query. It lists estimated costs, row counts, and join paths.
  * **`EXPLAIN ANALYZE`**: Actually **runs the query** in a sandbox environment and prints the actual execution timings, loop counts, and memory footprint alongside the optimizer's estimates. This is critical for diagnosing stale database statistics.

---

### Q35. Explain the three main Join Algorithms used under the hood.
* **Asked by:** Google, Snowflake, ClickHouse, AWS
* **Answer:**
  * **Nested Loop Join:** For each row in outer table A, scan the inner table B. Best for small tables ($O(M \times N)$).
  * **Sort-Merge Join:** Sort both tables by the join column, then step through them in parallel to merge matches. Highly efficient if tables are already sorted by indexes ($O(M \log M + N \log N)$).
  * **Hash Join:** Read outer table A and build an in-memory hash table on the join column. Read inner table B, hashing each join key to probe the hash table. Best for large, unsorted datasets ($O(M + N)$).

---

### Q36. What is a Covered Index (or Index-Only Scan)?
* **Asked by:** High-scale engineering teams (Stripe, Uber, Meta)
* **Answer:**
  * A **Covered Index** is an index that contains all the columns referenced by a query (both in the `SELECT`, `WHERE`, and `JOIN` clauses).
  * **Why it matters:** The database engine satisfies the entire query using only the index pages. It skips the expensive secondary phase of looking up data blocks on disk (Index-Only Scan), significantly reducing random disk I/O overhead.

---

## Section 6: Recovery, Sharding & Distributed Systems (Q37–Q50)

### Q37. What is Write-Ahead Logging (WAL)? How does it guarantee durability?
* **Asked by:** Uber, Stripe, AWS
* **Answer:**
  * **WAL** is a protocol stating that any modification to a database page must be written to stable non-volatile storage (the transaction log) **before** the modified page is flushed to the actual database files on disk.
  * **How it guarantees Durability:** If the system crashes mid-transaction, memory pages (buffer pool) are lost, but the log on disk is safe. Upon restart, the database recovery manager reads the WAL log sequentially to REDO committed changes and UNDO uncommitted modifications.

---

### Q38. What is the difference between UNDO logging, REDO logging, and UNDO/REDO logging?
* **Asked by:** Core systems engineering teams
* **Answer:**
  * **UNDO Logging:** Stores the *old value* of modified data. Used to roll back transactions that were active during a crash. Requires flushing all modified data blocks to disk *before* committing.
  * **REDO Logging:** Stores the *new value* of modified data. Used to reapply changes of committed transactions. Allows committing without forcing data pages to disk immediately.
  * **UNDO/REDO Logging:** Stores *both* old and new values. Offers maximum flexibility: data pages can be written to disk before or after commit (non-force, steal policy).

---

### Q39. Explain the role of checkpoints in database recovery.
* **Asked by:** Stripe, Oracle, Salesforce
* **Answer:**
  * Without checkpoints, recovery would require scanning the WAL log from the beginning of time, which is slow and wastes disk space.
  * **Checkpoint Process:** Periodically, the database forces all dirty pages in memory to disk and writes a `<CHECKPOINT>` record to the log.
  * **Benefit:** During recovery, the engine only needs to scan the log back to the last checkpoint. Older log portions can be safely truncated, saving disk space.

---

### Q40. What is database sharding? Compare Horizontal vs. Vertical Partitioning.
* **Asked by:** Meta, Google, Uber, Zepto (System Design + DBMS)
* **Answer:**
  Sharding is a method of scaling databases horizontally by distributing data across multiple physical machines.
  * **Horizontal Partitioning (Sharding):** Splitting rows of a single table across multiple databases based on a shard key (e.g., users 1–1M on Shard A, users 1M–2M on Shard B). Table schema remains identical.
  * **Vertical Partitioning:** Splitting columns of a table into separate tables (e.g., putting large text/blob columns like `user_bio` in a separate database table to keep the core `User` table narrow and fast).

---

### Q41. What is Consistent Hashing? Why is it critical for distributed databases?
* **Asked by:** Meta, Netflix, AWS, Cassandra teams
* **Answer:**
  * In classic hashing (`shard = hash(key) % N`), adding or removing a shard machine ($N$) invalidates all mappings, requiring moving nearly $100\%$ of the data.
  * **Consistent Hashing** maps both keys and servers to a circular ring. A key is assigned to the first server it encounters clockwise on the ring.
  * **Benefit:** When a node is added or removed, only a small fraction of keys ($K/N$) must be remapped. This makes distributed databases (Cassandra, DynamoDB) horizontally scale with minimal data movement overhead.

---

### Q42. What are the challenges of distributed transactions? Explain 2-Phase Commit (2PC).
* **Asked by:** Uber, Stripe, Meta
* **Answer:**
  Managing transactions across multiple database servers requires a consensus protocol to ensure atomic commits.
  * **2-Phase Commit (2PC):**
    1. **Prepare Phase:** A coordinator asks all participant shards if they are ready to commit. Shards write changes to WAL and reply with "VOTE_COMMIT" or "VOTE_ABORT".
    2. **Commit Phase:** If all shards voted commit, the coordinator sends a "GLOBAL_COMMIT" signal. Otherwise, it sends a "GLOBAL_ROLLBACK" to abort.
  * **Drawback:** 2PC is a **blocking protocol**. If the coordinator fails mid-phase, shards are left holding locks indefinitely, hurting system throughput.

---

### Q43. What is the CAP Theorem?
* **Asked by:** Meta, Stripe, Netflix (System Design)
* **Answer:**
  In a distributed data store, you can only guarantee at most two out of the three properties:
  1. **Consistency (C):** Every read receives the most recent write or an error.
  2. **Availability (A):** Every non-failing node returns a non-error response.
  3. **Partition Tolerance (P):** The system continues to operate despite network partitions/failures.
  * **Real-world constraint:** Network partitions are inevitable ($P$ is mandatory). Thus, distributed systems must choose between being **CP** (Consistent but Unavailable under partition) or **AP** (Available but Temporarily Inconsistent).

---

### Q44. Compare ACID vs. BASE transaction models.
* **Asked by:** Uber, Amazon, NoSQL projects
* **Answer:**
  * **ACID (SQL/RDBMS):** Focuses on strict consistency. Data is guaranteed to be correct and isolated at all times. Best for financial ledgers and order tables.
  * **BASE (NoSQL/Distributed):** Focuses on availability and scaling.
    * **B**asically **A**vailable (reads/writes are fast and always respond)
    * **S**oft state (data can change without user interaction due to background syncs)
    * **E**ventual consistency (replicas will synchronize and align given time). Best for social feeds, chat logs.

---

### Q45. What is a Materialized View vs. a Standard View?
* **Asked by:** Stripe, Snowflake, Google
* **Answer:**
  * **Standard View:** A virtual table defined by a query. It does not store data on disk; when queried, it acts as a macro, rewriting and running the underlying query dynamically.
  * **Materialized View:** Actually **stores the query results on disk**. It speeds up complex aggregate queries but must be periodically refreshed (`REFRESH MATERIALIZED VIEW`) when base table data changes, introducing write overhead.

---

### Q46. What is denormalization? When is it acceptable?
* **Asked by:** Uber, Flipkart, Zepto
* **Answer:**
  * **Denormalization** is the intentional introduction of data redundancy to optimize read performance by reducing relational joins.
  * **When acceptable:**
    1. Read-to-Write ratio is extremely high (e.g., system reads data millions of times but updates it once a day).
    2. Joining large tables causes unacceptable query latency.
    3. Analytical data warehouses (OLAP platforms) where read throughput is the primary metric.

---

### Q47. What is Dimensional Modeling? Compare Star Schema vs. Snowflake Schema.
* **Asked by:** Amazon, Google, Analytics teams
* **Answer:**
  Dimensional Modeling partitions data into **Fact tables** (numerical metrics) and **Dimension tables** (descriptive contexts) to optimize analytics.
  * **Star Schema:** Dimension tables are completely denormalized. Connects facts to dimensions via a single-level join. Fast queries, but high data redundancy.
  * **Snowflake Schema:** Dimension tables are normalized (split into sub-dimensions). Reduces data redundancy but requires complex multi-level joins, reducing read performance.

---

### Q48. What are Slowly Changing Dimensions (SCD Types 1, 2, and 3)?
* **Asked by:** Data warehousing / ETL pipelines interviews
* **Answer:**
  SCD defines how a data warehouse handles changes in dimension attributes (e.g., a customer changing their address).
  * **SCD Type 1 (Overwrite):** Overwrites old data directly. No history is kept.
  * **SCD Type 2 (Add Row):** Creates a new record row with version trackers (`start_date`, `end_date`, `is_active` flag). Preserves complete history (industry standard).
  * **SCD Type 3 (Add Column):** Adds a column to store the previous value (e.g., `previous_address`). Only tracks the immediate last state.

---

### Q49. What is Database Connection Pooling? Why is it necessary?
* **Asked by:** Uber, Stripe, Backend roles
* **Answer:**
  * Establishing a new TCP connection to a database requires an expensive 3-way handshake and SSL negotiation.
  * **Connection Pooling** maintains a cache of active database connections (the pool) that are reused by incoming application requests.
  * **Why necessary:** Prevents the database from exhausting CPU and memory resources from constantly creating/destroying connection threads, maintaining high-throughput request rates.

---

### Q50. What is a deadlock? Explain Wait-Die vs. Wound-Wait.
* **Asked by:** Google, Amazon, Core systems
* **Answer:**
  * A **Deadlock** is a state where two or more transactions are blocked indefinitely, each waiting for a lock held by the other.
  * **Wait-Die (Non-preemptive):** If older transaction $T_{old}$ requests a lock held by younger $T_{young}$, $T_{old}$ waits. If younger $T_{young}$ requests a lock held by older $T_{old}$, $T_{young}$ dies (aborts and restarts).
  * **Wound-Wait (Preemptive):** If older $T_{old}$ requests a lock held by younger $T_{young}$, $T_{old}$ wounds/preempts $T_{young}$ (forcing $T_{young}$ to abort). If younger $T_{young}$ requests a lock held by older $T_{old}$, $T_{young}$ waits.
  * **Why it works:** Both schemes assign transaction priorities using timestamps, preventing starvation by restarting aborted transactions with their original timestamps.
