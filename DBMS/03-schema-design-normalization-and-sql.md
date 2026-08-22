# 03: Normalization, Schema Design & SQL Execution

> **Target:** Verbal Technical Interviews at Top Tech Companies (FAANG, Uber, Stripe, Atlassian, Microsoft, Google).  
> **Focus:** Spoken definitions of normal forms, SQL execution order, physical join algorithms, and window function mechanics.

---

## 1. Keys & Functional Dependencies Taxonomy

```
+-----------------------------------------------------------------------------------------+
| KEY TYPE            | 15-SECOND VERBAL DEFINITION                                       |
+-----------------------------------------------------------------------------------------+
| Super Key (SK)      | Any set of attributes that uniquely identifies a row in a table.  |
+-----------------------------------------------------------------------------------------+
| Candidate Key (CK)  | A **Minimal Super Key** (removing any column breaks uniqueness).  |
+-----------------------------------------------------------------------------------------+
| Primary Key (PK)    | The candidate key chosen by the database designer as the primary. |
+-----------------------------------------------------------------------------------------+
| Prime Attribute     | Any column that is part of **AT LEAST ONE Candidate Key**.        |
+-----------------------------------------------------------------------------------------+
| Non-Prime Attribute | A column that does not belong to any Candidate Key.               |
+-----------------------------------------------------------------------------------------+
```

---

## 2. Database Normalization (1NF to BCNF)

### The 30-Second Verbal Script for Normal Forms
> *"1NF eliminates repeating groups and enforces atomic values. 2NF removes partial key dependencies where a non-prime column depends on only part of a composite key. 3NF removes transitive dependencies where a non-prime column depends on another non-prime column. BCNF is a stricter version of 3NF requiring that for every functional dependency $X \to Y$, determinant $X$ must be a Super Key."*

```
+-----------------------------------------------------------------------------------------+
| NORMAL FORM         | RULE / CONDITION                       | VIOLATION EXAMPLE & FIX  |
+-----------------------------------------------------------------------------------------+
| 1NF                 | All column values must be **Atomic**   | `phones: "123, 456"`     |
|                     | (no lists, arrays, or repeating groups)| Fix: Separate rows/table.|
+-----------------------------------------------------------------------------------------+
| 2NF                 | 1NF + **No Partial Key Dependencies**  | `(order_id, item_id)` PK |
|                     | (Non-prime column depends on FULL CK,  | $\to$ `item_name` depends|
|                     | not a subset of a composite key).      | on `item_id` only. Split!|
+-----------------------------------------------------------------------------------------+
| 3NF                 | 2NF + **No Transitive Dependencies**   | `emp_id` $\to$ `dept_id` |
|                     | (In $X \to Y$, $X$ must be a Super Key | $\to$ `dept_name`.       |
|                     | OR $Y$ must be a Prime Attribute).     | Split dept into own table|
+-----------------------------------------------------------------------------------------+
| BCNF                | For EVERY non-trivial $X \to Y$,       | In $X \to Y$, $Y$ is     |
|                     | **$X$ MUST BE A SUPER KEY**            | prime but $X$ is not SK. |
|                     | (Zero exceptions for prime attributes).| Stricter than 3NF.       |
+-----------------------------------------------------------------------------------------+
```

---

## 3. Normalization vs. Denormalization (OLTP vs. OLAP)

```
+-----------------------------------------------------------------------------------------+
| DIMENSION           | NORMALIZED SCHEMA (3NF / BCNF)         | DENORMALIZED SCHEMA (Star Schema)|
+-----------------------------------------------------------------------------------------+
| Workload Focus      | **OLTP (Transactional Apps)**          | **OLAP (Analytics & Reporting)** |
+-----------------------------------------------------------------------------------------+
| Write Performance   | **Fast & Lightweight** (update 1 row;  | **Slow & Complex** (must sync    |
|                     | zero redundant copies to maintain).    | multiple redundant tables).      |
+-----------------------------------------------------------------------------------------+
| Read Performance    | Requires multi-table `JOIN` operations.| **Blazing Fast** (pre-joined flat|
|                     |                                        | tables; zero joins needed).      |
+-----------------------------------------------------------------------------------------+
| Storage Footprint   | Minimal storage (zero redundancy).     | High storage (data duplicated).  |
+-----------------------------------------------------------------------------------------+
```

---

## 4. SQL Logical Execution Order

SQL is declarative: while queries begin with `SELECT`, database engines execute clauses in a strict **8-step logical order**:

```
1. FROM & JOIN     ---> Loads base tables and executes joins
2. WHERE           ---> Filters individual raw rows BEFORE aggregation
3. GROUP BY        ---> Groups matching rows into summary buckets
4. HAVING          ---> Filters aggregated buckets (can use SUM, COUNT, AVG)
5. SELECT          ---> Evaluates expressions, projections, and column aliases
6. DISTINCT        ---> Removes duplicate output rows
7. ORDER BY        ---> Sorts the final result set (Can use SELECT aliases!)
8. LIMIT / OFFSET  ---> Slices output for pagination
```

### Verbal Interview Follow-ups:
- **Why can't `WHERE` use column aliases defined in `SELECT`?**  
  *Spoken Answer:* `WHERE` (Step 2) executes *before* `SELECT` (Step 5). When `WHERE` is evaluated, the column aliases have not yet been projected by the engine. In contrast, `ORDER BY` (Step 7) executes *after* `SELECT`, which is why aliases work in `ORDER BY`.
- **`WHERE` vs. `HAVING`:**  
  `WHERE` filters individual rows *before* grouping (cannot contain aggregate functions like `SUM`). `HAVING` filters summary buckets *after* grouping (`HAVING COUNT(*) > 5`).

---

## 5. Window Functions

A **Window Function** computes values across a set of table rows related to the current row without collapsing rows into a single summary output (unlike `GROUP BY`).

$$\text{FUNCTION}() \ \mathbf{OVER} \ ( \mathbf{PARTITION \ BY} \ \text{category} \ \mathbf{ORDER \ BY} \ \text{metric} )$$

### `ROW_NUMBER()` vs. `RANK()` vs. `DENSE_RANK()`

```
Given Salaries: [ 100k, 90k, 90k, 80k ]

Function      | Assigned Values | Behavior on Duplicate / Tie Values
---------------------------------------------------------------------------------------------------
ROW_NUMBER()  | 1, 2, 3, 4      | Strictly unique sequential integers (arbitrary tie break).
RANK()        | 1, 2, 2, 4      | Ties share rank; **SKIPS subsequent rank numbers** (gap at 3).
DENSE_RANK()  | 1, 2, 2, 3      | Ties share rank; **NO GAPS in rank numbers**.
```

### Top-K per Group Pattern (High-Frequency Interview SQL)
```sql
WITH Ranked AS (
    SELECT 
        emp_id, department_id, salary,
        DENSE_RANK() OVER (PARTITION BY department_id ORDER BY salary DESC) AS rnk
    FROM employees
)
SELECT * FROM Ranked WHERE rnk <= 3;
```

---

## 6. Physical Join Algorithms in Database Engines

```
+-----------------------------------------------------------------------------------------+
| JOIN ALGORITHM      | TIME COMPLEXITY             | WHEN THE OPTIMIZER CHOOSES IT       |
+-----------------------------------------------------------------------------------------+
| Nested Loop Join    | $O(M \cdot N)$ (Unindexed)  | One table is small (outer) and the  |
|                     | $O(M \log N)$ (Indexed)     | other table has an INDEX on join key|
+-----------------------------------------------------------------------------------------+
| Hash Join           | $O(M + N)$                  | Large tables with equality (`=`)    |
|                     |                             | condition and NO indexes on join key|
+-----------------------------------------------------------------------------------------+
| Sort-Merge Join     | $O(M \log M + N \log N)$    | Both tables are already sorted on   |
|                     | $O(M + N)$ (If pre-sorted)  | the join key, or for range joins.   |
+-----------------------------------------------------------------------------------------+
```

---

## 7. High-Yield Verbal Interview Q&A

### Q1: "What is the difference between 3NF and BCNF?"
> **Spoken Answer:** 3NF allows a functional dependency $X \to Y$ if $X$ is a superkey OR if $Y$ is a prime attribute. BCNF eliminates this second exception: $X$ must be a Super Key for every functional dependency without exception. BCNF guarantees zero redundancy, but decomposing into BCNF may sometimes lose functional dependencies.

### Q2: "Why does `WHERE col NOT IN (SELECT x FROM t)` return 0 rows if `t.x` contains a NULL?"
> **Spoken Answer:** SQL uses three-valued logic (`TRUE`, `FALSE`, `UNKNOWN`). `NOT IN (1, 2, NULL)` evaluates to `col != 1 AND col != 2 AND col != NULL`. Comparing any value to `NULL` yields `UNKNOWN`. An `AND` expression with `UNKNOWN` results in `UNKNOWN`, so all rows are filtered out. Always use `NOT EXISTS` instead.

### Q3: "What is the difference between a Correlated and Non-Correlated subquery?"
> **Spoken Answer:** A non-correlated subquery is independent of the outer query and executes once globally. A correlated subquery references columns from the outer query row, forcing the database engine to execute the subquery once for every single outer row ($O(N^2)$), which should be refactored into a `JOIN` or CTE in production.
