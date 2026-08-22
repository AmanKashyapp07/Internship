# 03: Database Normalization, Schema Design & SQL Execution

> **Target:** Top Product & FinTech Technical Interviews (FAANG, Uber, Stripe, Atlassian, Citadel).  
> **Style:** High-ROI revision sheet. Focuses on functional dependencies, 1NF–BCNF normalization proofs, SQL logical execution lifecycle, window functions, and physical join algorithms.

---

# Table of Contents
1. [Functional Dependencies & Keys](#1-functional-dependencies--keys)
2. [Database Normalization Forms (1NF, 2NF, 3NF, BCNF)](#2-database-normalization-forms-1nf-2nf-3nf-bcnf)
3. [Normalization vs. Denormalization (OLTP vs. OLAP)](#3-normalization-vs-denormalization-oltp-vs-olap)
4. [SQL Logical Execution Order (The 9 Phases)](#4-sql-logical-execution-order-the-9-phases)
5. [Window Functions & Analytic SQL](#5-window-functions--analytic-sql)
6. [Complex Joins & Physical Engine Join Algorithms](#6-complex-joins--physical-engine-join-algorithms)
7. [Subqueries vs. CTEs & Recursive Queries](#7-subqueries-vs-ctes--recursive-queries)
8. [High-Yield Interview Questions & Defense](#8-high-yield-interview-questions--defense)
9. [Quick Recap](#9-quick-recap)

---

# 1. Functional Dependencies & Keys

A **Functional Dependency (FD)** $X \to Y$ between two sets of attributes in a relational schema $R$ means that whenever two tuples agree on the value of attribute set $X$, they **must also agree** on the value of attribute set $Y$.

```
Key Taxonomy:
+-----------------------------------------------------------------------------------------+
| KEY TYPE            | DEFINITION                                                        |
+-----------------------------------------------------------------------------------------+
| Super Key (SK)      | A set of attributes that uniquely identifies every tuple in R.    |
| Candidate Key (CK)  | A **Minimal Super Key** (no proper subset is a Super Key).        |
| Primary Key (PK)    | The candidate key chosen by the database architect as primary.    |
| Alternate Key (AK)  | Candidate keys not chosen as the Primary Key.                     |
| Prime Attribute     | An attribute that is a member of **ANY Candidate Key**.          |
| Non-Prime Attribute | An attribute that does NOT belong to any Candidate Key.           |
+-----------------------------------------------------------------------------------------+
```

---

### Armstrong’s Axioms & Attribute Closure ($X^+$)
- **Reflexivity:** If $Y \subseteq X$, then $X \to Y$.
- **Augmentation:** If $X \to Y$, then $XZ \to YZ$.
- **Transitivity:** If $X \to Y$ and $Y \to Z$, then $X \to Z$.
- **Attribute Closure ($X^+$):** The set of all attributes functionally determined by $X$ under a set of FDs $F$. If $X^+ = R$, then $X$ is a **Super Key**!

---

# 2. Database Normalization Forms (1NF, 2NF, 3NF, BCNF)

**Normalization** is the systematic decomposition of tables to **eliminate data redundancy and mutation anomalies** (Insertion, Deletion, and Update anomalies).

```
Normalization Invariants Hierarchy:
[ Unnormalized Data ] ---> [ 1NF: Atomic Values Only ]
                               |
                           [ 2NF: No Partial Dependencies (Full Functional Dependency) ]
                               |
                           [ 3NF: No Transitive Dependencies ]
                               |
                           [ BCNF: Every Determinant MUST be a Super Key ]
```

---

### A. First Normal Form (1NF)
- **Rule:** Every column must contain **Atomic (Indivisible) values**. No repeating groups, arrays, or comma-separated lists.
- **Violation:** `users(id, name, phone_numbers: "123, 456, 789")`.
- **Fix:** Split into atomic rows or a separate child table `user_phones(user_id, phone)`.

---

### B. Second Normal Form (2NF)
- **Rule:** Table must be in **1NF**, and **NO Non-Prime attribute can be Partially Dependent on a proper subset of any Composite Candidate Key**.
- **Formal Definition:** For every non-trivial FD $X \to Y$, if $Y$ is non-prime, $X$ cannot be a proper subset of any Candidate Key.
- **Violation:** `order_items(order_id, item_id, item_name, quantity)` where CK is `(order_id, item_id)`.
  - `item_id -> item_name` is a **Partial Dependency** (depends on part of the key!).
- **Fix:** Decompose into `order_items(order_id, item_id, quantity)` and `items(item_id, item_name)`.

---

### C. Third Normal Form (3NF)
- **Rule:** Table must be in **2NF**, and **NO Non-Prime attribute can be Transitively Dependent on a Candidate Key**.
- **Formal Definition:** For every non-trivial FD $X \to Y$, at least one of the following must hold:
  1. $X$ is a **Super Key**, OR
  2. $Y$ is a **Prime Attribute** (member of some Candidate Key).
- **Violation:** `employees(emp_id, emp_name, dept_id, dept_name)` where CK is `emp_id`.
  - `emp_id -> dept_id` and `dept_id -> dept_name` $\implies$ `emp_id -> dept_name` is a **Transitive Dependency** ($dept\_id$ is not a superkey, and $dept\_name$ is non-prime!).
- **Fix:** Decompose into `employees(emp_id, emp_name, dept_id)` and `departments(dept_id, dept_name)`.

---

### D. Boyce-Codd Normal Form (BCNF)
- **Rule:** A stricter version of 3NF. For EVERY non-trivial functional dependency $X \to Y$, **$X$ MUST BE A SUPER KEY**.
- **Difference from 3NF:** 3NF allows $X \to Y$ if $Y$ is prime even if $X$ is not a superkey; BCNF forbids this.

```
Classic 3NF but NOT BCNF Example:
Table: student_advisor(student_id, subject, advisor_name)
Candidate Keys: (student_id, subject)
Functional Dependencies:
1. (student_id, subject) -> advisor_name (Valid: LHS is Candidate Key)
2. advisor_name -> subject (Violation! advisor_name is NOT a Super Key, but subject is prime -> Allowed in 3NF, FORBIDDEN in BCNF!).
Fix: Decompose into student_advisor(student_id, advisor_name) and advisor_subject(advisor_name, subject).
```

---

# 3. Normalization vs. Denormalization (OLTP vs. OLAP)

```
+-----------------------------------------------------------------------------------------+
| DIMENSION           | NORMALIZED SCHEMA (3NF / BCNF)         | DENORMALIZED SCHEMA (Star/Snowflake) |
+-----------------------------------------------------------------------------------------+
| Primary Objective   | **Minimize Redundancy & Anomaly Safety**| **Maximize Analytical Read Speed**  |
| Target Workload     | **OLTP (Online Transaction Processing)**| **OLAP (Data Warehouses / Analytics)|
| Write Performance   | **Fast & Lightweight** (update 1 row;   | **Slower / Complex** (updates must  |
|                     | zero redundant copies to sync).        | modify multiple redundant tables).  |
| Read Performance    | Requires multi-table `JOIN` operations. | **Fast** (pre-joined flat tables;   |
|                     |                                         | zero joins needed for reporting).   |
| Disk Footprint      | Minimal storage footprint.              | High storage footprint (redundancy).|
+-----------------------------------------------------------------------------------------+
```

---

# 4. SQL Logical Execution Order (The 9 Phases)

While SQL queries are written starting with `SELECT`, the database engine's query processor executes clauses in a **strict 9-step logical sequence**:

```
SQL Logical Execution Pipeline:
1. [ FROM & JOIN ]   ---> Loads base tables and executes Cartesian / Join conditions
2. [ ON ]            ---> Filters rows on join conditions
3. [ WHERE ]         ---> Filters raw row predicates BEFORE grouping
4. [ GROUP BY ]      ---> Collapses matching rows into summary aggregate buckets
5. [ HAVING ]        ---> Filters aggregated summary buckets
6. [ SELECT ]        ---> Evaluates expressions, projections, and scalar functions
7. [ DISTINCT ]      ---> Deduplicates identical projection rows
8. [ ORDER BY ]      ---> Sorts final output rows (Supports SELECT column aliases!)
9. [ LIMIT / OFFSET ]---> Slices final output window for pagination
```

---

### Why `WHERE` Cannot Use `SELECT` Column Aliases
- In the execution pipeline, **`WHERE` (Phase 3)** executes *before* **`SELECT` (Phase 6)**. When the engine evaluates the `WHERE` filter, the column alias defined in `SELECT` has **not yet been projected or evaluated**!
- In contrast, **`ORDER BY` (Phase 8)** executes *after* `SELECT`, which is why column aliases can be used in `ORDER BY`.

---

### `WHERE` vs. `HAVING` (The Classic Interview Distinction)
- **`WHERE`:** Filters individual raw table rows **before aggregation** occurs. Cannot contain aggregate functions (`WHERE SUM(salary) > 50000` is **illegal**).
- **`HAVING`:** Filters grouped summary buckets **after `GROUP BY`** collapses rows (`HAVING COUNT(*) >= 5` is valid).

```sql
-- Correct Execution Order in Practice:
SELECT department_id, AVG(salary) AS avg_sal  -- 4. SELECT projections & alias
FROM employees                                -- 1. FROM base table
WHERE hire_date >= '2024-01-01'               -- 2. WHERE raw row filter
GROUP BY department_id                        -- 3. GROUP BY departments
HAVING AVG(salary) > 80000                    -- 3b. HAVING aggregate filter
ORDER BY avg_sal DESC                         -- 5. ORDER BY alias
LIMIT 5;                                      -- 6. LIMIT output
```

---

# 5. Window Functions & Analytic SQL

**Window Functions** perform calculations across a set of table rows related to the current row without collapsing rows into a single summary output (unlike `GROUP BY`).

$$\text{FUNCTION}() \ \mathbf{OVER} \ ( \mathbf{PARTITION \ BY} \ \text{category} \ \mathbf{ORDER \ BY} \ \text{metric} \ \mathbf{ROWS \ BETWEEN} \dots )$$

---

### A. `ROW_NUMBER()` vs. `RANK()` vs. `DENSE_RANK()`

```
Given Salaries: [ 100k, 90k, 90k, 80k ]

Function      | Values Assigned | Behavior on Duplicate / Tie Values
---------------------------------------------------------------------------------------------------
ROW_NUMBER()  | 1, 2, 3, 4      | Strictly unique sequential integers (arbitrary tie break).
RANK()        | 1, 2, 2, 4      | Same rank for ties; **SKIPS subsequent ranks** (gap at 3).
DENSE_RANK()  | 1, 2, 2, 3      | Same rank for ties; **NO GAPS in rank numbers**.
```

```sql
-- Finding the Top 3 Highest Paid Employees per Department:
WITH RankedEmployees AS (
    SELECT 
        emp_id, name, department_id, salary,
        DENSE_RANK() OVER (
            PARTITION BY department_id 
            ORDER BY salary DESC
        ) AS salary_rank
    FROM employees
)
SELECT * FROM RankedEmployees WHERE salary_rank <= 3;
```

---

### B. Positional & Value Window Functions
- **`LAG(col, offset, default)`:** Accesses data from a previous row at a specified physical offset (e.g. calculate Month-over-Month revenue growth: `revenue - LAG(revenue, 1) OVER (...)`).
- **`LEAD(col, offset, default)`:** Accesses data from a subsequent forward row.
- **`NTILE(k)`:** Divides an ordered partition into $k$ equal buckets (percentiles / quartiles).

---

# 6. Complex Joins & Physical Engine Join Algorithms

---

### A. Relational Join Types
- **`INNER JOIN`:** Returns rows with matching keys in **both** tables.
- **`LEFT (OUTER) JOIN`:** Returns all rows from left table; matched rows from right table (unmatched filled with `NULL`).
- **`RIGHT (OUTER) JOIN`:** Returns all rows from right table.
- **`FULL OUTER JOIN`:** Returns all rows from both tables, filling missing sides with `NULL`.
- **`CROSS JOIN`:** Cartesian product of all rows ($M \times N$).
- **`SELF JOIN`:** Joining a table to itself using aliases (e.g. Employee-Manager hierarchy).

---

### B. Physical Join Execution Algorithms in RDBMS Optimizers

```
+-----------------------------------------------------------------------------------------+
| JOIN ALGORITHM      | TIME COMPLEXITY             | SPACE | IDEAL WORKLOAD SCENARIO     |
+-----------------------------------------------------------------------------------------+
| Nested Loop Join    | O(M * N) (Unindexed)        | O(1)  | Small outer table joined to |
|                     | O(M * log N) (Index Loop)   |       | an inner table with an INDEX|
+-----------------------------------------------------------------------------------------+
| Hash Join           | O(M + N)                    | O(M)  | Large unsorted equi-joins   |
|                     |                             | (RAM) | (Builds hash table on small |
|                     |                             |       | table, probes with larger). |
+-----------------------------------------------------------------------------------------+
| Sort-Merge Join     | O(M log M + N log N)        | O(1)  | Tables already sorted on the|
|                     | O(M + N) (If pre-sorted)    |       | join key, or non-equi joins.|
+-----------------------------------------------------------------------------------------+
```

```
Hash Join Engine Execution:
1. BUILD PHASE: Read smaller table M into an in-memory Hash Table keyed on join column.
2. PROBE PHASE: Scan larger table N row by row, hashing join key to lookup matches in O(1) time!
```

---

# 7. Subqueries vs. CTEs & Recursive Queries

### A. Non-Correlated vs. Correlated Subqueries
- **Non-Correlated Subquery:** Independent inner query that executes **once globally**:
  `WHERE salary > (SELECT AVG(salary) FROM employees);` ($O(N)$ time).
- **Correlated Subquery:** Inner query references columns from outer query row; executes **once per outer row** ($O(N^2)$ time! Avoid in production).

---

### B. Common Table Expressions (CTEs)
A **Common Table Expression (CTE)** defines a temporary named result set using the `WITH` keyword, providing modular, readable SQL queries that the query optimizer can optimize or inline.

---

### C. Recursive CTEs (Graph & Hierarchy Traversals)
Used to traverse tree/graph hierarchies (e.g., finding all direct and indirect report subordinates of a manager):

```sql
WITH RECURSIVE SubordinateTree AS (
    -- 1. ANCHOR MEMBER: Base starting root (e.g. CEO with emp_id = 1)
    SELECT emp_id, name, manager_id, 1 AS level
    FROM employees
    WHERE emp_id = 1
    
    UNION ALL
    
    -- 2. RECURSIVE MEMBER: Joins previous level result with employees table
    SELECT e.emp_id, e.name, e.manager_id, st.level + 1
    FROM employees e
    INNER JOIN SubordinateTree st ON e.manager_id = st.emp_id
)
SELECT * FROM SubordinateTree;
```

---

# 8. High-Yield Interview Questions & Defense

### Q1: What is the difference between 3NF and BCNF?
> **Answer:** 3NF allows a functional dependency $X \to Y$ if $X$ is a superkey OR if $Y$ is a prime attribute (part of any candidate key). BCNF eliminates this second exception: $X$ **MUST be a Super Key for ALL dependencies**. BCNF is strictly anomaly-free, but decomposing into BCNF may sometimes fail to preserve functional dependencies.

### Q2: Why does `WHERE col NOT IN (SELECT x FROM t)` return 0 rows if `t.x` contains a NULL?
> **Answer:** SQL uses three-valued logic (`TRUE`, `FALSE`, `UNKNOWN`). `NOT IN (1, 2, NULL)` expands to `col != 1 AND col != 2 AND col != NULL`. Comparing any value to NULL evaluates to `UNKNOWN`, turning the entire `AND` chain into `UNKNOWN`, filtering out 100% of rows. Always use `NOT EXISTS` instead.

### Q3: When does a Query Optimizer choose a Hash Join over a Nested Loop Join?
> **Answer:** When joining two large tables on an equality condition (`=`) where neither table has an index on the join key. The optimizer builds an in-memory hash table on the smaller relation and probes it with the larger relation in $O(M + N)$ time, vastly outperforming an $O(M \cdot N)$ unindexed Nested Loop Join.

---

# 9. Quick Recap

- **Normalization Invariants:** 1NF is atomic values $\to$ 2NF removes partial key dependencies $\to$ 3NF removes transitive dependencies $\to$ BCNF enforces that every determinant $X$ in $X \to Y$ is a Super Key.
- **SQL Execution Order:** `FROM/JOIN` $\to$ `WHERE` $\to$ `GROUP BY` $\to$ `HAVING` $\to$ `SELECT` $\to$ `DISTINCT` $\to$ `ORDER BY` $\to$ `LIMIT`.
- **`WHERE` vs. `HAVING`:** `WHERE` filters raw table rows before grouping; `HAVING` filters aggregated buckets after grouping.
- **Rank Functions:** `ROW_NUMBER` has unique integers $\to$ `RANK` leaves gaps on ties ($1, 2, 2, 4$) $\to$ `DENSE_RANK` has no gaps ($1, 2, 2, 3$).
- **Physical Joins:** Nested Loop for small/indexed tables; Hash Join for large unsorted equi-joins ($O(M+N)$); Sort-Merge for pre-sorted datasets.
- **Recursive CTEs:** Structured with an Anchor Member `UNION ALL` Recursive Member to traverse hierarchies and graphs cleanly.
