# Master Guide 02: Schema Design, Normalization & SQL Interview Mastery

> **Focus:** Relational Keys Taxonomy, Normalization (1NF to BCNF), Primary Key ID Generation Strategies (Auto-Increment vs UUIDv4 vs UUIDv7 vs Snowflake), Soft Delete Index Traps, Database Statistics & Cost-Based Optimizer (`EXPLAIN ANALYZE`), Top 20 SQL Query Patterns, Window Functions, CTEs, Gaps & Islands, and Physical Join Algorithms.
> 
> *Designed to crush top-tier FAANG/FinTech database engineering interviews.*

---

# Table of Contents
1. [Keys & Functional Dependencies Taxonomy](#1-keys--functional-dependencies-taxonomy)
2. [Database Normalization (1NF to BCNF)](#2-database-normalization-1nf-to-bcnf)
3. [Normalization vs. Denormalization (OLTP vs. OLAP)](#3-normalization-vs-denormalization-oltp-vs-olap)
4. [Primary Key & ID Generation Strategies (UUIDv4 vs UUIDv7 vs Snowflake)](#4-primary-key--id-generation-strategies)
5. [Soft Deletes Anti-Pattern & Partial Index Solutions](#5-soft-deletes-anti-pattern--partial-index-solutions)
6. [Database Statistics & Cost-Based Optimizer (CBO)](#6-database-statistics--cost-based-optimizer-cbo)
7. [JSON / JSONB Hybrid Schema Design & GIN Indexing](#7-json--jsonb-hybrid-schema-design)
8. [SQL Logical Execution Order](#8-sql-logical-execution-order)
9. [Top 20 Core SQL Interview Patterns](#9-top-20-core-sql-interview-patterns)
10. [Physical Join Algorithms in Database Engines](#10-physical-join-algorithms)
11. [Interview Quick-Fire Verbal Q&A](#11-interview-quick-fire-verbal-qa)

---

# 1. Keys & Functional Dependencies Taxonomy

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

# 2. Database Normalization (1NF to BCNF)

### 30-Second Verbal Script for Normal Forms
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

# 3. Normalization vs. Denormalization (OLTP vs. OLAP)

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

# 4. Primary Key & ID Generation Strategies

Choosing a Primary Key strategy has massive performance implications on B+ Tree page fragmentation and write throughput:

```
+-----------------------------------------------------------------------------------------+
| STRATEGY            | SIZE & FORMAT             | B+ TREE IMPACT & INTERVIEW TRADEOFF   |
+-----------------------------------------------------------------------------------------+
| Auto-Increment      | 8-byte `BIGINT`           | **Sequential Appends:** Zero Page Splits.|
| (BIGINT Serial)     |                           | **Flaw:** High write hotspot on 1 node; |
|                     |                           | reveals total row count to attackers. |
+-----------------------------------------------------------------------------------------+
| UUIDv4              | 16-byte random `UUID`     | **CATASTROPHIC FOR B+ TREES:** Random    |
| (Random GUID)       |                           | insertions cause 50% Page Splits across |
|                     |                           | random leaf pages, destroying cache!   |
+-----------------------------------------------------------------------------------------+
| UUIDv7              | 16-byte time-ordered      | **THE GOLD STANDARD:** Unix timestamp   |
| (Time-Ordered)      | `UUID` (48-bit time prefix)| prefix ensures sequential B+ Tree appends |
|                     |                           | with global uniqueness! Zero splits.    |
+-----------------------------------------------------------------------------------------+
| Snowflake ID        | 8-byte (64-bit) integer   | **Optimal for Distributed Systems:**    |
| (Twitter Snowflake) | (41-bit time, 10-bit node,| Time-ordered, compact 64-bit integer,   |
|                     | 12-bit sequence)          | scaleable across 1,024 worker nodes!    |
+-----------------------------------------------------------------------------------------+
```

---

# 5. Soft Deletes Anti-Pattern & Partial Index Solutions

Setting `is_deleted = TRUE` instead of deleting rows causes severe database degradation over time:

```sql
-- The Anti-Pattern: Millions of active queries scanning non-deleted rows
SELECT * FROM users WHERE status = 'ACTIVE' AND is_deleted = FALSE;
```

### Why Soft Deletes Degrade Indexes:
1. Low Cardinality: `is_deleted` contains only `TRUE` or `FALSE` (99% of rows are `FALSE`).
2. B+ Tree Bloat: Every index on `users` includes dead/soft-deleted rows, bloating index size in RAM.

### The Production Fix: Partial Indexes
```sql
-- Partial Index in PostgreSQL: Excludes soft-deleted rows from the index entirely!
CREATE INDEX idx_active_users_email ON users (email) WHERE is_deleted = FALSE;
```

---

# 6. Database Statistics & Cost-Based Optimizer (CBO)

Database engines use a **Cost-Based Optimizer (CBO)** to determine query execution plans (choosing between Index Scan vs Full Table Scan, or Hash Join vs Nested Loop).

### How the CBO Calculates Query Cost:
- **Selectivity Estimation:** Predicts fraction of rows matching a filter based on table statistics:
  $$\text{Expected Rows} = N \times \text{Selectivity}$$
- **Histograms:** Stores column data distributions (Equi-depth histograms) to handle data skew.
- **Stale Statistics Danger:** If a table receives millions of writes without `ANALYZE`, statistics become stale. The CBO may miscalculate cost and choose a catastrophic Full Table Scan instead of an Index Scan!

### How to Read `EXPLAIN ANALYZE` Output:
```text
EXPLAIN ANALYZE SELECT * FROM orders WHERE total_amount > 500;

-> Index Scan using idx_orders_amount on orders  (cost=0.42..8.44 rows=10 width=32) (actual time=0.031..0.045 rows=12 loops=1)
```
- `cost=0.42..8.44`: `0.42` = startup cost to fetch first row; `8.44` = total estimated cost.
- `rows=10` vs `actual rows=12`: Compare estimated rows vs actual rows. Large discrepancies ($> 10\times$) indicate stale database statistics requiring `ANALYZE`!

---

# 7. JSON / JSONB Hybrid Schema Design

Modern relational engines (PostgreSQL, MySQL) support storing unstructured JSON natively alongside relational columns.

```sql
-- Hybrid Schema: Structured metadata + Flexible JSONB details
CREATE TABLE products (
    id BIGSERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    price NUMERIC(10, 2) NOT NULL,
    attributes JSONB NOT NULL -- E.g. {"color": "red", "specs": {"ram": "16GB"}}
);

-- Indexing JSONB via GIN Index (Generalized Inverted Index)
CREATE INDEX idx_products_attributes ON products USING GIN (attributes);

-- Fast JSONB Query using containment operator (@>)
SELECT * FROM products WHERE attributes @> '{"color": "red"}';
```

---

# 8. SQL Logical Execution Order

SQL is declarative: while queries start with `SELECT`, engines process clauses in an **8-step logical order**:

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

---

# 9. Top 20 Core SQL Interview Patterns

### Pattern 01: GROUP BY + HAVING Filtering
```sql
SELECT customer_id, COUNT(order_id) AS total_orders, SUM(total_amount) AS total_spent
FROM orders
WHERE order_date >= '2023-01-01'
GROUP BY customer_id
HAVING COUNT(order_id) >= 5 AND SUM(total_amount) > 1000.00;
```

### Pattern 02: Conditional Aggregation
```sql
SELECT 
    user_id,
    SUM(CASE WHEN txn_type = 'deposit' THEN amount ELSE 0 END) AS total_deposited,
    SUM(CASE WHEN txn_type = 'withdrawal' THEN amount ELSE 0 END) AS total_withdrawn,
    ROUND(
        SUM(CASE WHEN txn_type = 'deposit' THEN amount ELSE 0 END) * 1.0 / 
        NULLIF(SUM(CASE WHEN txn_type = 'withdrawal' THEN amount ELSE 0 END), 0), 2
    ) AS deposit_withdrawal_ratio
FROM transactions
GROUP BY user_id;
```

### Pattern 03: Pre-Aggregated CTE Joins
Prevents the "fan-out" Cartesian explosion bug when joining multiple 1-to-many child tables:
```sql
WITH user_orders AS (
    SELECT user_id, SUM(order_amount) AS total_revenue FROM orders GROUP BY user_id
),
user_tickets AS (
    SELECT user_id, COUNT(ticket_id) AS total_tickets FROM support_tickets GROUP BY user_id
)
SELECT u.user_id, u.user_name, COALESCE(uo.total_revenue, 0), COALESCE(ut.total_tickets, 0)
FROM users u
LEFT JOIN user_orders uo ON u.user_id = uo.user_id
LEFT JOIN user_tickets ut ON u.user_id = ut.user_id;
```

### Pattern 04: The Ranking Triad
```sql
SELECT emp_id, department_id, salary,
    ROW_NUMBER() OVER (PARTITION BY department_id ORDER BY salary DESC) AS seq_num, -- 1, 2, 3, 4
    RANK() OVER (PARTITION BY department_id ORDER BY salary DESC) AS rnk,           -- 1, 2, 2, 4 (gaps)
    DENSE_RANK() OVER (PARTITION BY department_id ORDER BY salary DESC) AS dense_rnk -- 1, 2, 2, 3 (no gaps)
FROM employees;
```

### Pattern 05: Top-N Records per Group
```sql
WITH Ranked AS (
    SELECT emp_id, department_id, salary,
        DENSE_RANK() OVER (PARTITION BY department_id ORDER BY salary DESC) AS rnk
    FROM employees
)
SELECT * FROM Ranked WHERE rnk <= 3;
```

### Pattern 06: Running Totals & Cumulative Sums
```sql
SELECT txn_date, amount,
    SUM(amount) OVER (ORDER BY txn_date ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW) AS running_total
FROM transactions;
```

### Pattern 07: Moving Averages (`ROWS BETWEEN`)
```sql
SELECT daily_date, revenue,
    AVG(revenue) OVER (ORDER BY daily_date ROWS BETWEEN 6 PRECEDING AND CURRENT ROW) AS 7_day_moving_avg
FROM daily_sales;
```

### Pattern 08: Period-over-Period Changes (`LAG` & `LEAD`)
```sql
SELECT monthly_date, revenue,
    LAG(revenue, 1) OVER (ORDER BY monthly_date) AS prev_month_revenue,
    ROUND((revenue - LAG(revenue, 1) OVER (ORDER BY monthly_date)) * 100.0 / LAG(revenue, 1) OVER (ORDER BY monthly_date), 2) AS mom_growth_pct
FROM monthly_sales;
```

### Pattern 09: Percentiles & Medians
```sql
SELECT user_id, score,
    NTILE(4) OVER (ORDER BY score DESC) AS quartile,
    PERCENT_RANK() OVER (ORDER BY score) AS percentile_rank
FROM user_scores;
```

### Pattern 10: Self-Joins & Hierarchical Tree Traversal
```sql
SELECT e.emp_name AS employee, m.emp_name AS manager
FROM employees e
LEFT JOIN employees m ON e.manager_id = m.emp_id;
```

### Pattern 11: Anti-Joins (`LEFT JOIN + IS NULL` vs `NOT EXISTS`)
```sql
SELECT u.user_id, u.user_name
FROM users u
WHERE NOT EXISTS (
    SELECT 1 FROM orders o WHERE o.user_id = u.user_id
);
```

### Pattern 12: Non-Equi Joins & Range Overlaps
```sql
SELECT e.emp_name, e.salary, s.grade
FROM employees e
JOIN salary_grades s ON e.salary BETWEEN s.min_salary AND s.max_salary;
```

### Pattern 13: Deduplication Techniques
```sql
WITH Duplicates AS (
    SELECT id, email,
        ROW_NUMBER() OVER (PARTITION BY email ORDER BY id ASC) AS row_num
    FROM users
)
DELETE FROM users WHERE id IN (SELECT id FROM Duplicates WHERE row_num > 1);
```

### Pattern 14: Nth Highest Value Without Window Functions
```sql
SELECT DISTINCT salary FROM employees e1
WHERE N - 1 = (
    SELECT COUNT(DISTINCT e2.salary) FROM employees e2 WHERE e2.salary > e1.salary
);
```

### Pattern 15: Gaps and Islands (Consecutive Active Streaks)
```sql
WITH GroupedStreaks AS (
    SELECT user_id, active_date,
        active_date - INTERVAL '1 day' * ROW_NUMBER() OVER (PARTITION BY user_id ORDER BY active_date) AS streak_group
    FROM user_activity
)
SELECT user_id, MIN(active_date) AS streak_start, MAX(active_date) AS streak_end, COUNT(*) AS streak_length
FROM GroupedStreaks
GROUP BY user_id, streak_group
HAVING COUNT(*) >= 3;
```

### Pattern 16: Date-Based Cohort Retention Analysis
```sql
WITH FirstLogin AS (
    SELECT user_id, DATE_TRUNC('month', MIN(login_date)) AS cohort_month
    FROM user_logins GROUP BY user_id
)
SELECT f.cohort_month,
    DATE_TRUNC('month', l.login_date) AS activity_month,
    COUNT(DISTINCT l.user_id) AS active_users
FROM FirstLogin f
JOIN user_logins l ON f.user_id = l.user_id
GROUP BY f.cohort_month, DATE_TRUNC('month', l.login_date);
```

### Pattern 17: Multi-Step Conversion Funnels
```sql
SELECT 
    COUNT(DISTINCT v.user_id) AS viewed,
    COUNT(DISTINCT c.user_id) AS added_to_cart,
    COUNT(DISTINCT p.user_id) AS purchased
FROM page_views v
LEFT JOIN cart_adds c ON v.user_id = c.user_id AND c.created_at >= v.created_at
LEFT JOIN purchases p ON c.user_id = p.user_id AND p.created_at >= c.created_at;
```

### Pattern 18: Correlated Subqueries & `EXISTS`
```sql
SELECT c.customer_name FROM customers c
WHERE EXISTS (
    SELECT 1 FROM orders o WHERE o.customer_id = c.customer_id AND o.amount > 500
);
```

### Pattern 19: Recursive CTEs (Org Chart Hierarchy)
```sql
WITH RECURSIVE OrgChart AS (
    SELECT emp_id, emp_name, manager_id, 1 AS level
    FROM employees WHERE manager_id IS NULL
    UNION ALL
    SELECT e.emp_id, e.emp_name, e.manager_id, o.level + 1
    FROM employees e
    JOIN OrgChart o ON e.manager_id = o.emp_id
)
SELECT * FROM OrgChart ORDER BY level, emp_id;
```

### Pattern 20: Set Operations (`UNION` vs `UNION ALL`)
- `UNION` forces a duplicate removal sort/hash ($O(N \log N)$ cost).
- `UNION ALL` streams rows directly without sorting ($O(1)$ fast). Always use `UNION ALL` unless deduplication is required.

---

# 10. Physical Join Algorithms in Database Engines

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

# 11. Interview Quick-Fire Verbal Q&A

1. **"Why is UUIDv4 terrible for B+ Tree primary keys, and why is UUIDv7 preferred?"**  
   *Answer:* UUIDv4 is completely random. Inserting random keys into a B+ Tree forces random leaf page reads and triggers frequent 50% Page Splits, destroying memory cache locality. UUIDv7 prefixes a 48-bit timestamp, ensuring time-ordered sequential B+ Tree appends with zero page splits.
2. **"Why should you use Partial Indexes for Soft-Deleted tables?"**  
   *Answer:* Soft-deleted tables store 99% active rows (`is_deleted = FALSE`). A standard index includes soft-deleted rows, wasting RAM. A partial index `WHERE is_deleted = FALSE` indexes only live rows, keeping index size tiny and blazing fast.
