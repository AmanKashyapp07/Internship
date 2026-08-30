# SQL Query Engine & Relational Execution Semantics

> **Scope:** SQL Logical Evaluation Order, Window Functions, Ranking Algorithms, Relational Set Operations, Anti-Joins, Three-Valued Logic, and Hierarchical Self-Joins.

---

# Table of Contents
1. [SQL Logical Query Processing Pipeline](#1-sql-logical-query-processing-pipeline)
2. [Window Functions & Ranking Mechanics](#2-window-functions--ranking-mechanics)
3. [Foundational Relational Query Patterns](#3-foundational-relational-query-patterns)
4. [WHERE vs. HAVING: Filtering Semantics](#4-where-vs-having-filtering-semantics)
5. [Three-Valued Logic & NULL Handling in Set Operations](#5-three-valued-logic--null-handling-in-set-operations)
6. [Hierarchical Queries & Self-Joins](#6-hierarchical-queries--self-joins)
7. [Query Engine Execution Principles](#7-query-engine-execution-principles)

---

# 1. SQL Logical Query Processing Pipeline

SQL is a declarative language where queries specify *what* data to retrieve, while the database query optimizer builds the physical execution plan. Logically, clauses are evaluated in a strict eight-step sequence:

```
Logical Evaluation Sequence:
[ 1. FROM & JOINs ] ---> [ 2. WHERE Filters ] ---> [ 3. GROUP BY ] ---> [ 4. HAVING Filters ]
                                                                               |
[ 8. LIMIT / OFFSET ] <-- [ 7. ORDER BY ] <--- [ 6. DISTINCT ] <--- [ 5. SELECT (Columns/Expressions) ]
```

### Logical Processing Implications:
1. **Column Aliasing Visibility:** Expressions defined in the `SELECT` clause (Step 5) cannot be referenced in `WHERE` (Step 2) or `GROUP BY` (Step 3) because those stages execute before column projection occurs.
2. **Aggregation Scoping:** Aggregation functions (`SUM`, `AVG`, `COUNT`) operate on groups formed during Step 3, meaning they are accessible in `HAVING` (Step 4) and `SELECT` (Step 5), but cannot be evaluated in `WHERE` (Step 2).

---

# 2. Window Functions & Ranking Mechanics

Unlike `GROUP BY`, which collapses input rows into a single aggregated row per group, **Window Functions** compute values across a partition of rows while preserving the identity and cardinality of every individual input row.

### Ranking Function Comparison:
Given a partitioned set of salary values: `[100, 100, 80, 70]`

```
+--------+------------+------+------------+---------------------------------------------------------+
| SALARY | ROW_NUMBER | RANK | DENSE_RANK | MATHEMATICAL BEHAVIOR                                   |
+--------+------------+------+------------+---------------------------------------------------------+
| 100    | 1          | 1    | 1          | All start at rank 1                                     |
| 100    | 2          | 1    | 1          | ROW_NUMBER breaks ties arbitrarily; RANK/DENSE tie at 1 |
| 80     | 3          | 3    | 2          | RANK skips rank 2 (1,1,3); DENSE_RANK produces (1,1,2)  |
| 70     | 4          | 4    | 3          | DENSE_RANK maintains gapless sequential rankings        |
+--------+------------+------+------------+---------------------------------------------------------+
```

### Value Offset Navigation (`LEAD` & `LAG`):
```sql
-- Computes day-over-day price changes within a partition
SELECT 
    stock_date,
    price,
    LAG(price, 1) OVER (ORDER BY stock_date) AS prev_day_price,
    price - LAG(price, 1) OVER (ORDER BY stock_date) AS daily_change
FROM StockPrices;
```

---

# 3. Foundational Relational Query Patterns

### Pattern 1: N-th Highest Value (Handling Duplicate Ties)
```sql
WITH RankedSalaries AS (
    SELECT salary, DENSE_RANK() OVER (ORDER BY salary DESC) AS rank_val
    FROM Employees
)
SELECT salary AS NthHighestSalary
    FROM RankedSalaries
WHERE rank_val = 2
LIMIT 1;
```

---

### Pattern 2: Duplicate Record Identification & Retention
```sql
-- Step A: Identify duplicate groups
SELECT email, COUNT(*) AS count_val
FROM Users
GROUP BY email
HAVING COUNT(*) > 1;

-- Step B: Retain minimum identifier per duplicate group
DELETE FROM Users
WHERE id NOT IN (
    SELECT min_id FROM (
        SELECT MIN(id) AS min_id
        FROM Users
        GROUP BY email
    ) AS temp
);
```

---

### Pattern 3: Cumulative Running Aggregations
```sql
SELECT 
    customer_id,
    order_date,
    amount,
    SUM(amount) OVER (
        PARTITION BY customer_id 
        ORDER BY order_date
        ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW
    ) AS running_total
FROM Orders;
```

---

### Pattern 4: Island & Gap Detection (Consecutive Streak Analysis)
```sql
-- Date Subtraction Island Algorithm:
-- For consecutive dates, (activity_date - ROW_NUMBER() * 1 day) generates an invariant group key.
WITH GroupedActivity AS (
    SELECT 
        user_id,
        activity_date,
        activity_date - (ROW_NUMBER() OVER (PARTITION BY user_id ORDER BY activity_date)) * INTERVAL '1 day' AS grp
    FROM UserLogins
)
SELECT user_id, MIN(activity_date) AS streak_start, COUNT(*) AS streak_length
FROM GroupedActivity
GROUP BY user_id, grp
HAVING COUNT(*) >= 3;
```

---

### Pattern 5: Top-N Elements per Partition
```sql
WITH RankedEmployees AS (
    SELECT 
        e.name,
        e.salary,
        d.dept_name,
        DENSE_RANK() OVER (PARTITION BY e.dept_id ORDER BY e.salary DESC) AS rnk
    FROM Employees e
    JOIN Departments d ON e.dept_id = d.id
)
SELECT dept_name, name, salary
FROM RankedEmployees
WHERE rnk <= 3;
```

---

### Pattern 6: Relational Anti-Join (Set Difference $A \setminus B$)
```sql
-- Approach A: LEFT JOIN with IS NULL check (Optimized via index lookups)
SELECT c.customer_id, c.name
FROM Customers c
LEFT JOIN Orders o ON c.customer_id = o.customer_id
WHERE o.customer_id IS NULL;

-- Approach B: NOT EXISTS Subquery (Correlated subquery evaluation)
SELECT c.customer_id, c.name
FROM Customers c
WHERE NOT EXISTS (
    SELECT 1 FROM Orders o WHERE o.customer_id = c.customer_id
);
```

---

# 4. WHERE vs. HAVING: Filtering Semantics

```sql
-- WHERE filters individual tuples BEFORE grouping.
-- HAVING filters aggregated groups AFTER group formation.
SELECT dept_id, AVG(salary) AS avg_salary
FROM Employees
WHERE status = 'Active'         -- Predicate applied to base tuples
GROUP BY dept_id
HAVING AVG(salary) > 50000;     -- Predicate applied to grouped aggregate result
```

---

# 5. Three-Valued Logic & NULL Handling in Set Operations

SQL implements Kleene's Three-Valued Logic (`TRUE`, `FALSE`, `UNKNOWN`).

### The `NOT IN` with `NULL` Semantics:
```sql
-- Given: Orders table contains a row where customer_id IS NULL
SELECT * FROM Customers 
WHERE customer_id NOT IN (SELECT customer_id FROM Orders);
```
- In three-valued logic, `x NOT IN (1, 2, NULL)` expands to:
  `x != 1 AND x != 2 AND x != NULL`
- Since `x != NULL` evaluates to `UNKNOWN`, the entire conjunction evaluates to `UNKNOWN` or `FALSE`.
- **Result:** The query returns 0 rows. Use `NOT EXISTS` or `LEFT JOIN ... WHERE ... IS NULL` to safely handle nullable foreign keys.

### `UNION` vs. `UNION ALL`:
- **`UNION`:** Performs set union with deduplication via implicit sort or hash aggregation.
- **`UNION ALL`:** Concatenates relations without deduplication, avoiding sorting and hashing CPU overhead.

### Aggregate Counting Behavior:
- **`COUNT(*)`:** Computes the total number of tuples in a relation or group, including rows containing `NULL` values.
- **`COUNT(column_name)`:** Computes only tuples where `column_name` is explicitly `NOT NULL`.

---

# 6. Hierarchical Queries & Self-Joins

A self-join joins a relation with itself, modeling unary relationships and recursive hierarchical data structures (e.g. employee-manager hierarchies, graph adjacency lists).

```sql
-- Query: Find all employees earning more than their immediate managers
SELECT 
    e.name AS employee_name,
    e.salary AS employee_salary,
    m.name AS manager_name,
    m.salary AS manager_salary
FROM Employees e
JOIN Employees m ON e.manager_id = m.id
WHERE e.salary > m.salary;
```

---

# 7. Query Engine Execution Principles

1. **Window Function Execution:** Window operations run during Step 5 (`SELECT`) after `WHERE`, `GROUP BY`, and `HAVING` filters are complete.
2. **Index-Aware Predicates:** B+ Tree indexes accelerate equality (`=`), range (`BETWEEN`, `<`, `>`), and prefix pattern matching (`LIKE 'prefix%'`). Leading wildcards (`LIKE '%suffix'`) negate index tree traversals, forcing full relation scans.
3. **Set Operations:** Prefer `UNION ALL` over `UNION` whenever duplicates are impossible or acceptable, eliminating intermediate sort/hash phases in the query execution engine.
