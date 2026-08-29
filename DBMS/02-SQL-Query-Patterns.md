# Master Guide 02: SQL Query Patterns & Archetypes

> **Focus:** The 6 Classic SQL Interview Archetypes (Nth Highest, Duplicates, Running Sums, Streaks/Gaps, Top-N Per Group, Anti-Joins), Window Functions, WHERE vs. HAVING, and Self-Joins.
> 
> *Targeted for Top-Tier Tech System & Backend Engineering Interviews.*

---

# Table of Contentss
1. [SQL Logical Execution Order](#1-sql-logical-execution-order)
2. [The 6 Core SQL Interview Archetypes](#2-the-6-core-sql-interview-archetypes)
3. [Window Functions: ROW_NUMBER vs. RANK vs. DENSE_RANK](#3-window-functions-row_number-vs-rank-vs-dense_rank)
4. [WHERE vs. HAVING: The Classic Trap](#4-where-vs-having-the-classic-trap)
5. [Self-Joins & Hierarchical Queries](#5-self-joins--hierarchical-queries)
6. [High-Frequency Interview Drill & Verbal Q&A](#6-high-frequency-interview-drill--verbal-qa)

---

# 1. SQL Logical Execution Order

```
Logical Evaluation Sequence:
[ 1. FROM & JOINs ] ---> [ 2. WHERE Filters ] ---> [ 3. GROUP BY ] ---> [ 4. HAVING Filters ]
                                                                               |
[ 8. LIMIT / OFFSET ] <-- [ 7. ORDER BY ] <--- [ 6. DISTINCT ] <--- [ 5. SELECT (Columns/Aliases) ]
```

- **The Interview Trap:** Referencing a column alias declared in the `SELECT` clause inside the `WHERE` or `GROUP BY` clause. Because `WHERE` and `GROUP BY` are evaluated **before `SELECT`**, the alias does not exist yet and causes a SQL syntax error.

---

# 2. The 6 Core SQL Interview Archetypes

### Archetype 1: N-th Highest Salary (e.g. 2nd Highest)
```sql
-- Solution A: Modern SQL with DENSE_RANK (Handles duplicate salary ties safely)
WITH RankedSalaries AS (
    SELECT salary, DENSE_RANK() OVER (ORDER BY salary DESC) AS rank_val
    FROM Employees
)
SELECT salary AS SecondHighestSalary
FROM RankedSalaries
WHERE rank_val = 2
LIMIT 1;

-- Solution B: LIMIT with OFFSET (Simple, but returns empty set instead of NULL if < 2 rows)
SELECT (
    SELECT DISTINCT salary
    FROM Employees
    ORDER BY salary DESC
    LIMIT 1 OFFSET 1
) AS SecondHighestSalary;
```

---

### Archetype 2: Duplicate Record Detection & Deletion
```sql
-- Step A: Find Duplicate Emails
SELECT email, COUNT(*) AS cnt
FROM Users
GROUP BY email
HAVING COUNT(*) > 1;

-- Step B: Delete Duplicates keeping only the MIN(id)
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

### Archetype 3: Running Total / Cumulative Sum
```sql
-- Cumulative revenue per customer over time
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

### Archetype 4: Consecutive Streaks (3+ Consecutive Active Days)
```sql
-- The "Date Subtraction Island" Trick:
-- If dates are consecutive, (order_date - row_number * 1 day) produces a CONSTANT date group!
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

### Archetype 5: Top-N Records Per Category (e.g. Top 3 Earners per Dept)
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

### Archetype 6: Anti-Join (Records in Table A NOT in Table B)
```sql
-- Option A (RECOMMENDED): LEFT JOIN with IS NULL (Fast with indexes)
SELECT c.customer_id, c.name
FROM Customers c
LEFT JOIN Orders o ON c.customer_id = o.customer_id
WHERE o.customer_id IS NULL;

-- Option B: NOT EXISTS (Safe with NULL values)
SELECT c.customer_id, c.name
FROM Customers c
WHERE NOT EXISTS (
    SELECT 1 FROM Orders o WHERE o.customer_id = c.customer_id
);

-- THE TRAP: Never use `WHERE id NOT IN (SELECT id FROM Orders)` if Orders.id contains NULL!
-- If the subquery returns even ONE NULL, `NOT IN` evaluates to UNKNOWN and returns 0 rows!
```

---

# 3. Window Functions: ROW_NUMBER vs. RANK vs. DENSE_RANK

Given Salaries: `[100, 100, 80, 70]`

```
+--------+------------+------+------------+---------------------------------------------------------+
| SALARY | ROW_NUMBER | RANK | DENSE_RANK | BEHAVIORAL DIFFERENCE                                   |
+--------+------------+------+------------+---------------------------------------------------------+
| 100    | 1          | 1    | 1          | All start at 1                                          |
| 100    | 2          | 1    | 1          | ROW_NUMBER breaks ties arbitrarily; RANK/DENSE assign 1 |
| 80     | 3          | 3    | 2          | RANK skips rank 2 (1,1,3); DENSE_RANK preserves (1,1,2) |
| 70     | 4          | 4    | 3          | DENSE_RANK produces continuous sequential rankings      |
+--------+------------+------+------------+---------------------------------------------------------+
```

### LEAD() and LAG() Value Offset Queries:
```sql
-- Compare each day's price to the previous day's price
SELECT 
    stock_date,
    price,
    LAG(price, 1) OVER (ORDER BY stock_date) AS prev_day_price,
    price - LAG(price, 1) OVER (ORDER BY stock_date) AS daily_change
FROM StockPrices;
```

---

# 4. WHERE vs. HAVING: The Classic Trap

```sql
-- INCORRECT QUERY (Compile Error):
SELECT dept_id, AVG(salary) AS avg_sal
FROM Employees
WHERE AVG(salary) > 50000 -- ERROR: Cannot use aggregate functions in WHERE clause!
GROUP BY dept_id;

-- CORRECT QUERY (WHERE filters rows BEFORE grouping; HAVING filters groups AFTER):
SELECT dept_id, AVG(salary) AS avg_sal
FROM Employees
WHERE status = 'Active'   -- Filters individual employee rows first
GROUP BY dept_id
HAVING AVG(salary) > 50000; -- Filters grouped department aggregates
```

---

# 5. Self-Joins & Hierarchical Queries

```sql
-- Find Employees earning strictly MORE than their direct Manager
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

# 6. High-Frequency Interview Drill & Verbal Q&A

### Q1: Why does `NOT IN` return 0 rows when the subquery contains a `NULL`?
> **Answer:** In SQL three-valued logic (`TRUE`, `FALSE`, `UNKNOWN`), `x NOT IN (1, 2, NULL)` expands to `x != 1 AND x != 2 AND x != NULL`. Comparing anything to `NULL` yields `UNKNOWN`, making the entire `AND` condition evaluate to `UNKNOWN` and filtering out all candidate rows.

### Q2: What is the difference between `UNION` and `UNION ALL`?
> **Answer:** **`UNION`** removes duplicate rows by executing an expensive implicit sort or hash distinct operation in memory/disk. **`UNION ALL`** concatenates the result sets directly without deduplication, making it significantly faster.

### Q3: When does an index fail to optimize a `LIKE` query?
> **Answer:** Standard B+ Tree indexes only support prefix lookups (`LIKE 'Alice%'`). Wildcards at the beginning (`LIKE '%Alice'`) or middle (`LIKE '%Alice%'`) **force a full table scan** because the leftmost characters cannot be matched against the index tree.

### Q4: What is the difference between `COUNT(*)` and `COUNT(column_name)`?
> **Answer:** **`COUNT(*)`** counts total rows in the table or group, including rows containing `NULL` values. **`COUNT(column_name)`** counts only rows where `column_name` is **strictly `NOT NULL`**.

### Q5: How do Window Functions differ from `GROUP BY`?
> **Answer:** `GROUP BY` collapses multiple rows into a single summary output row per group. Window Functions calculate aggregate/ranking values across defined partitions while **retaining individual row identities and returning the original row count**.
