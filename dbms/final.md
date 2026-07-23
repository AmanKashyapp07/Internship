# DBMS Master Summary & Interview Revision Guide
> The Ultimate Cheat-Sheet for OA and Technical Interview Prep

This guide serves as a master summary, roadmap, and revision checklist for the entire DBMS interview prep series. Use it to quickly review key concepts, formulas, database designs, and SQL templates before your assessments.

---

## 1. Lecture Directory & Roadmap
Click the links below to access the deep-dive notes for each topic:

1.  **[L1 — Relational Model & Keys](file:///Users/amankashyap/Documents/internship/DBMS/l1.md)**: Relational schemas, domains, super/candidate/primary/foreign keys, referential integrity.
2.  **[L2 — ER Modeling](file:///Users/amankashyap/Documents/internship/DBMS/l2.md)**: Entities, attributes, weak entity sets, cardinalities, ER-to-Relational mapping rules.
3.  **[L3 — SQL: DDL, DML & Queries](file:///Users/amankashyap/Documents/internship/DBMS/l3.md)**: DDL vs DML, SELECT syntax, JOINs (Inner/Outer/Self), Subqueries, `GROUP BY` & `HAVING`, `IN` vs `EXISTS`.
4.  **[L4 — Normalization (1NF → BCNF)](file:///Users/amankashyap/Documents/internship/DBMS/l4.md)**: Functional dependencies, Attribute Closure algorithm, 1NF/2NF/3NF/BCNF requirements, Lossless join & Dependency preservation.
5.  **[L5 — Transactions & ACID](file:///Users/amankashyap/Documents/internship/DBMS/l5.md)**: Atomicity, Consistency, Isolation, Durability, transaction state machine, conflict & view serializability, precedence graphs.
6.  **[L6 — Concurrency Control](file:///Users/amankashyap/Documents/internship/DBMS/l6.md)**: 2PL (Strict/Rigorous), Timestamp Ordering, Deadlock handling (Wait-Die vs Wound-Wait), Concurrency anomalies (Dirty Read, Lost Update, Phantom Read).
7.  **[L7 — Indexing & Query Processing](file:///Users/amankashyap/Documents/internship/DBMS/l7.md)**: Primary vs Secondary indexing, Clustered vs Non-Clustered index, Dense vs Sparse index, B-Trees vs B+ Trees.
8.  **[L8 — Advanced SQL + Window Functions](file:///Users/amankashyap/Documents/internship/DBMS/l8.md)**: Window functions (`ROW_NUMBER`, `RANK`, `DENSE_RANK`, `NTILE`, `LEAD`, `LAG`), CTEs (Common Table Expressions), Nth highest salary.
9.  **[L9 — Recovery & Database Internals](file:///Users/amankashyap/Documents/internship/DBMS/l9.md)**: Storage hierarchies, WAL (Write-Ahead Logging), Checkpoints, UNDO/REDO recovery.
10. **[L10 — SQL Problem Patterns](file:///Users/amankashyap/Documents/internship/DBMS/l10.md)**: High-frequency interview queries, Top-N per group, Cumulative Sum, gaps/consecutive login problems.

---

## 2. Normalization & FDs Cheat-Sheet
When analyzing functional dependencies (FDs) to determine normal forms:

*   **Attribute Closure ($X^+$)**: Use it to find candidate keys. If $X^+$ includes all attributes of relation $R$, then $X$ is a Super Key.
*   **Normal Form Summary Table**:

| Normal Form | Rule / Requirement | Disallowed Condition |
| :--- | :--- | :--- |
| **1NF** | Attribute values must be atomic (no multi-valued or composite values). | Non-atomic cell values. |
| **2NF** | In 1NF + No **Partial Dependencies** (no non-prime attribute depends on a proper subset of a candidate key). | Part-of-Key $\rightarrow$ Non-Prime. |
| **3NF** | In 2NF + No **Transitive Dependencies**. For every non-trivial $X \rightarrow Y$: Either $X$ is a super key, OR $Y$ is a prime attribute. | Non-Prime $\rightarrow$ Non-Prime. |
| **BCNF** | For every non-trivial $X \rightarrow Y$: $X$ must be a super key. | Any dependency where LHS is not a Super Key. |

---

## 3. Transactions & Concurrency Cheat-Sheet

### ACID Implementation
*   **Atomicity & Durability**: Implemented by the **Recovery Manager** using logs (UNDO/REDO logs).
*   **Consistency**: Maintained by application logic and schema constraints (check constraints, foreign keys).
*   **Isolation**: Implemented by the **Concurrency Control Manager** using locking or multi-version concurrency control (MVCC).

### Serializability & Recoverability
*   **Conflict Serializability**: Tested using a **Precedence Graph** (draw an edge $T_i \rightarrow T_j$ if $T_i$ accesses $x$ before $T_j$, one is a write, and they conflict). **Acyclic graph = Conflict Serializable**.
*   **View Serializability**: Weaker than conflict serializability. Testing is NP-Complete (requires checking blind writes).
*   **Recoverable Schedule**: If $T_j$ reads a value written by $T_i$, the commit of $T_i$ must appear before the commit of $T_j$.
*   **Cascadeless Schedule**: If $T_j$ reads a value written by $T_i$, the commit of $T_i$ must appear before the *read* operation of $T_j$.

### Concurrency Anomalies
1.  **Dirty Read (WR Conflict)**: Reading uncommitted data that is later rolled back.
2.  **Unrepeatable Read (RW Conflict)**: Reading the same row twice and getting different values because another transaction modified it in-between.
3.  **Lost Update (WW Conflict)**: Overwriting an update made by another transaction without reading the value first.
4.  **Phantom Read**: A query retrieves a different set of rows twice because another transaction inserted/deleted rows in-between.

---

## 4. Indexing Quick Reference

*   **Clustered (Primary) Index**: Data rows physically sorted on disk to match the index order. Maximum **one** clustered index per table.
*   **Non-Clustered (Secondary) Index**: Data rows scattered; index stores search key + pointer (RID) to actual physical page. Multiple secondary indexes allowed.
*   **B-Tree vs. B+ Tree**:

| Feature | B-Tree | B+ Tree (Preferred in Databases) |
| :--- | :--- | :--- |
| **Data Storage** | Kept in both leaf and internal nodes. | Kept **only** in leaf nodes. Internal nodes store only search keys/pointers. |
| **Leaf Node Linking** | Leaf nodes are not linked. | Leaf nodes are **doubly linked** to allow fast sequential range queries. |
| **Search Cost** | Varies depending on node depth (can be $O(1)$). | Always uniform ($O(\log N)$) as all data is at leaf level. |
| **Page Fill Capacity** | Lower search key capacity due to data records inside internal pages. | Higher fan-out/capacity since internal nodes are lightweight. |

---


## 5. Classic SQL Coding Patterns

### Pattern A: Nth Highest Salary (using `DENSE_RANK()`)
Handles ties correctly (if two employees share the highest salary, they are both ranked #1, and the next is ranked #2).
```sql
WITH RankedEmployees AS (
    SELECT EmployeeID, Salary,
           DENSE_RANK() OVER (ORDER BY Salary DESC) AS rnk
    FROM Employee
)
SELECT Salary 
FROM RankedEmployees 
WHERE rnk = :N;
```

### Pattern B: Find Duplicate Rows
```sql
SELECT Email, COUNT(*) 
FROM Users 
GROUP BY Email 
HAVING COUNT(*) > 1;
```

### Pattern C: Row Pagination (using `ROW_NUMBER()`)
```sql
WITH OrderedRows AS (
    SELECT *,
           ROW_NUMBER() OVER (ORDER BY CreatedAt DESC) AS RowNum
    FROM Transactions
)
SELECT * 
FROM OrderedRows 
WHERE RowNum BETWEEN 11 AND 20; -- Page 2 (10 items per page)
```

---

