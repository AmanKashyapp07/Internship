# SQL Interview Patterns: The 20 Core Query Patterns

> **Purpose:** Comprehensive, dense, and skimmable revision guide for technical SQL interviews at top-tier tech companies (FAANG, Tier-1 Product, FinTech).  
> **Dialect Default:** Standard ANSI SQL (with explicit callouts for PostgreSQL, MySQL, SQL Server, and BigQuery).

---

# Table of Contents
1. [Aggregation & Grouping](#1-aggregation--grouping)
   - [Pattern 01: GROUP BY + HAVING Filter](#pattern-01-group-by--having-filter)
   - [Pattern 02: Conditional Aggregation (CASE inside SUM/COUNT)](#pattern-02-conditional-aggregation-case-inside-sumcount)
   - [Pattern 03: Multiple Joins with Pre-Aggregated Subqueries / CTEs](#pattern-03-multiple-joins-with-pre-aggregated-subqueries--ctes)
2. [Window Functions](#2-window-functions)
   - [Pattern 04: Ranking Triad (ROW_NUMBER vs. RANK vs. DENSE_RANK)](#pattern-04-ranking-triad-row_number-vs-rank-vs-dense_rank)
   - [Pattern 05: Top-N Records per Group](#pattern-05-top-n-records-per-group)
   - [Pattern 06: Running Totals & Cumulative Sums](#pattern-06-running-totals--cumulative-sums)
   - [Pattern 07: Moving & Rolling Window Averages (ROWS BETWEEN)](#pattern-07-moving--rolling-window-averages-rows-between)
   - [Pattern 08: Period-over-Period Changes (LAG & LEAD)](#pattern-08-period-over-period-changes-lag--lead)
   - [Pattern 09: Percentiles, Quartiles & Medians (NTILE & PERCENT_RANK)](#pattern-09-percentiles-quartiles--medians-ntile--percent_rank)
3. [Joins & Self-Joins](#3-joins--self-joins)
   - [Pattern 10: Self-Joins & Hierarchical Tree Traversal](#pattern-10-self-joins--hierarchical-tree-traversal)
   - [Pattern 11: Anti-Joins (LEFT JOIN + IS NULL / NOT EXISTS)](#pattern-11-anti-joins-left-join--is-null--not-exists)
   - [Pattern 12: Non-Equi Joins & Range Matching](#pattern-12-non-equi-joins--range-matching)
4. [Duplicates & Data Cleaning](#4-duplicates--data-cleaning)
   - [Pattern 13: Deduplication & Finding Duplicate Clusters](#pattern-13-deduplication--finding-duplicate-clusters)
   - [Pattern 14: Second / Nth Highest Value (Without Window Functions)](#pattern-14-second--nth-highest-value-without-window-functions)
5. [Date/Time & Sequential Patterns](#5-datetime--sequential-patterns)
   - [Pattern 15: Gaps and Islands (Consecutive Active Streaks)](#pattern-15-gaps-and-islands-consecutive-active-streaks)
   - [Pattern 16: Date-Based Cohort Retention Analysis](#pattern-16-date-based-cohort-retention-analysis)
   - [Pattern 17: Multi-Step Conversion Funnel Analysis](#pattern-17-multi-step-conversion-funnel-analysis)
6. [Subqueries, CTEs & Set Operations](#6-subqueries-ctes--set-operations)
   - [Pattern 18: Correlated Subqueries & EXISTS](#pattern-18-correlated-subqueries--exists)
   - [Pattern 19: Recursive CTEs (Hierarchies & Number Sequences)](#pattern-19-recursive-ctes-hierarchies--number-sequences)
   - [Pattern 20: Set Operations (UNION vs. INTERSECT vs. EXCEPT)](#pattern-20-set-operations-union-vs-intersect-vs-except)
7. [Last-Day Revision Sheet](#7-last-day-revision-sheet)

---

# 1. Aggregation & Grouping

### Pattern 01: GROUP BY + HAVING Filter
- **When to Use:** When filtering groups based on aggregated metrics (e.g., total count, average spend) rather than row-level attributes.
- **Sample Schema:** `orders(order_id, customer_id, order_date, total_amount)`
- **Common Question:** "Find all customers who placed at least 5 orders and spent more than $1,000 in total."
- **Query:**
```sql
SELECT 
    customer_id,
    COUNT(order_id) AS total_orders,
    SUM(total_amount) AS total_spent
FROM orders
WHERE order_date >= '2023-01-01'  -- Row-level filter evaluated BEFORE grouping
GROUP BY customer_id
HAVING COUNT(order_id) >= 5 
   AND SUM(total_amount) > 1000.00;
```
- **Gotcha / Trap:** Attempting to filter aggregate aliases in `WHERE` (e.g. `WHERE COUNT(order_id) >= 5`) or using column aliases in `HAVING` before they are computed (supported in MySQL, but invalid in PostgreSQL/SQL Server/ANSI).

---

### Pattern 02: Conditional Aggregation (CASE inside SUM/COUNT)
- **When to Use:** Pivoting rows into columns or computing multiple custom aggregations across subsets of data in a single table scan without subqueries.
- **Sample Schema:** `transactions(txn_id, user_id, txn_date, txn_type, amount)` where `txn_type IN ('deposit', 'withdrawal', 'refund')`
- **Common Question:** "For each user, calculate total deposits, total withdrawals, deposit-to-withdrawal ratio, and total transaction count in 2023."
- **Query:**
```sql
SELECT 
    user_id,
    -- SUM with CASE: sums amount if condition matches, otherwise 0
    SUM(CASE WHEN txn_type = 'deposit' THEN amount ELSE 0 END) AS total_deposited,
    SUM(CASE WHEN txn_type = 'withdrawal' THEN amount ELSE 0 END) AS total_withdrawn,
    
    -- COUNT with CASE: COUNT ignores NULLs; omitting ELSE defaults to NULL
    COUNT(CASE WHEN txn_type = 'refund' THEN 1 END) AS refund_count,
    
    -- Safe division protecting against division by zero
    ROUND(
        SUM(CASE WHEN txn_type = 'deposit' THEN amount ELSE 0 END) * 1.0 / 
        NULLIF(SUM(CASE WHEN txn_type = 'withdrawal' THEN amount ELSE 0 END), 0),
        2
    ) AS deposit_withdrawal_ratio
FROM transactions
WHERE txn_date BETWEEN '2023-01-01' AND '2023-12-31'
GROUP BY user_id;
```
- **Dialect Difference:** PostgreSQL supports the `FILTER` clause: `COUNT(*) FILTER (WHERE txn_type = 'refund') AS refund_count`.
- **Gotcha / Trap:** Using `COUNT(CASE WHEN condition THEN 0 ELSE 1 END)` — `COUNT` counts `0` as a valid non-null value! Use `COUNT(CASE WHEN condition THEN 1 END)` or `SUM(CASE WHEN condition THEN 1 ELSE 0 END)`.

---

### Pattern 03: Multiple Joins with Pre-Aggregated Subqueries / CTEs
- **When to Use:** Preventing the "fan-out" Cartesian explosion bug where joining parent to multiple 1-to-many child tables multiplies aggregated sums.
- **Sample Schema:** 
  - `users(user_id, user_name)`
  - `orders(order_id, user_id, order_amount)`
  - `support_tickets(ticket_id, user_id, status)`
- **Common Question:** "Return each user's name alongside their total lifetime order revenue and total support ticket count."
- **Query (The Right Way via Pre-Aggregation CTEs):**
```sql
WITH user_orders AS (
    SELECT 
        user_id, 
        SUM(order_amount) AS total_revenue
    FROM orders
    GROUP BY user_id
),
user_tickets AS (
    SELECT 
        user_id, 
        COUNT(ticket_id) AS total_tickets
    FROM support_tickets
    GROUP BY user_id
)
SELECT 
    u.user_id,
    u.user_name,
    COALESCE(uo.total_revenue, 0) AS total_revenue,
    COALESCE(ut.total_tickets, 0) AS total_tickets
FROM users u
LEFT JOIN user_orders uo ON u.user_id = uo.user_id
LEFT JOIN user_tickets ut ON u.user_id = ut.user_id;
```
- **Gotcha / Trap:** Writing `SELECT u.user_id, SUM(o.order_amount), COUNT(t.ticket_id) FROM users u LEFT JOIN orders o ... LEFT JOIN support_tickets t ... GROUP BY u.user_id`. Every order multiplies by every ticket, artificially blowing up `SUM(order_amount)` by the ticket count!

---

# 2. Window Functions

### Pattern 04: Ranking Triad (ROW_NUMBER vs. RANK vs. DENSE_RANK)
- **When to Use:** When assigning relative position/ranking within a group while controlling behavior for ties.
- **Sample Schema:** `employees(emp_id, emp_name, department_id, salary)`
- **Common Question:** "Rank employees by salary within each department and explain how ties are resolved."
- **Query:**
```sql
SELECT 
    emp_id,
    emp_name,
    department_id,
    salary,
    -- Unique sequential integer (1, 2, 3, 4) - arbitrary tie-breaker if non-deterministic
    ROW_NUMBER() OVER (PARTITION BY department_id ORDER BY salary DESC) AS row_num,
    
    -- Gaps on ties: (1, 2, 2, 4) - next rank reflects total preceding rows
    RANK() OVER (PARTITION BY department_id ORDER BY salary DESC) AS rnk,
    
    -- No gaps on ties: (1, 2, 2, 3) - next rank is strictly +1
    DENSE_RANK() OVER (PARTITION BY department_id ORDER BY salary DESC) AS dense_rnk
FROM employees;
```
- **Gotcha / Trap:** Asking for the "2nd highest salary" and filtering on `RANK = 2`. If two employees tie for 1st (`RANK = 1`), `RANK = 2` **does not exist** (skips to 3). Always use `DENSE_RANK() = 2` for the Nth highest value.

---

### Pattern 05: Top-N Records per Group
- **When to Use:** Filtering for the top/bottom $K$ items within categories (e.g., top 3 products per category, most recent 2 logins per user).
- **Sample Schema:** `products(product_id, category_id, product_name, price)`
- **Common Question:** "Find the top 2 highest-priced products in each category. If there is a tie for 2nd place, return all tied products."
- **Query:**
```sql
WITH ranked_products AS (
    SELECT 
        product_id,
        category_id,
        product_name,
        price,
        DENSE_RANK() OVER (PARTITION BY category_id ORDER BY price DESC) AS price_rank
    FROM products
)
SELECT 
    category_id,
    price_rank,
    product_name,
    price
FROM ranked_products
WHERE price_rank <= 2
ORDER BY category_id, price_rank, product_name;
```
- **Gotcha / Trap:** Attempting to put `WHERE DENSE_RANK() OVER (...) <= 2` in the same query block. Window functions are evaluated *after* `WHERE` and `HAVING`; they require a subquery or CTE to be filtered.

---

### Pattern 06: Running Totals & Cumulative Sums
- **When to Use:** Calculating progress-to-date, bank account running balances, or revenue ramp-up over time.
- **Sample Schema:** `daily_revenue(revenue_date, department_id, daily_amount)`
- **Common Question:** "Compute the day-by-day running total of revenue for each department across 2023."
- **Query:**
```sql
SELECT 
    revenue_date,
    department_id,
    daily_amount,
    -- Explicit framing prevents range-based tie duplication
    SUM(daily_amount) OVER (
        PARTITION BY department_id 
        ORDER BY revenue_date ASC
        ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW
    ) AS running_total
FROM daily_revenue;
```
- **Gotcha / Trap:** Writing `SUM(daily_amount) OVER (ORDER BY revenue_date)`. Without `ROWS BETWEEN`, default framing is `RANGE BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW`, which aggregates identical date ties together instead of calculating step-by-step.

---

### Pattern 07: Moving & Rolling Window Averages (ROWS BETWEEN)
- **When to Use:** Smoothing out volatility in time-series metrics (e.g., 7-day rolling active users, 30-day moving average stock price).
- **Sample Schema:** `stock_prices(ticker, trade_date, close_price)`
- **Common Question:** "Calculate the 7-day moving average of close prices (including the current day and past 6 trading records) for each stock ticker."
- **Query:**
```sql
SELECT 
    ticker,
    trade_date,
    close_price,
    ROUND(
        AVG(close_price) OVER (
            PARTITION BY ticker 
            ORDER BY trade_date ASC 
            ROWS BETWEEN 6 PRECEDING AND CURRENT ROW
        ),
        2
    ) AS moving_avg_7d,
    
    COUNT(close_price) OVER (
        PARTITION BY ticker 
        ORDER BY trade_date ASC 
        ROWS BETWEEN 6 PRECEDING AND CURRENT ROW
    ) AS sample_points_count
FROM stock_prices;
```
- **Gotcha / Trap:** Confusing `ROWS` (physical count of row positions) with `RANGE` (logical calendar day offsets). If trading dates have missing weekends/holidays, `ROWS BETWEEN 6 PRECEDING` calculates the last 6 *available trading days*, not 6 *calendar days*.

---

### Pattern 08: Period-over-Period Changes (LAG & LEAD)
- **When to Use:** Comparing current record values against preceding (`LAG`) or succeeding (`LEAD`) rows within a chronological sequence.
- **Sample Schema:** `monthly_sales(region, sale_year, sale_month, revenue)`
- **Common Question:** "Calculate month-over-month revenue growth rate percentage for each region."
- **Query:**
```sql
WITH sales_with_lag AS (
    SELECT 
        region,
        sale_year,
        sale_month,
        revenue,
        LAG(revenue, 1) OVER (
            PARTITION BY region 
            ORDER BY sale_year ASC, sale_month ASC
        ) AS prev_month_revenue
    FROM monthly_sales
)
SELECT 
    region,
    sale_year,
    sale_month,
    revenue,
    prev_month_revenue,
    ROUND(
        ((revenue - prev_month_revenue) * 100.0) / NULLIF(prev_month_revenue, 0), 
        2
    ) AS mom_growth_pct
FROM sales_with_lag;
```
- **Gotcha / Trap:** Missing the `NULLIF(prev_month_revenue, 0)` guard, causing a `division by zero` crash when the previous period had 0 revenue or during the first baseline period where `LAG` returns `NULL`.

---

### Pattern 09: Percentiles, Quartiles & Medians (NTILE & PERCENT_RANK)
- **When to Use:** Dividing ordered datasets into equal buckets (quartiles, deciles) or computing custom continuous percentiles (p50, p90, p99).
- **Sample Schema:** `user_latencies(request_id, endpoint, latency_ms)`
- **Common Question:** "Classify API request latencies into 4 performance quartiles per endpoint and find the 90th percentile threshold."
- **Query:**
```sql
-- Approach A: Discrete Bucketing with NTILE
SELECT 
    request_id,
    endpoint,
    latency_ms,
    NTILE(4) OVER (PARTITION BY endpoint ORDER BY latency_ms ASC) AS latency_quartile
FROM user_latencies;

-- Approach B: Continuous Percentile (Dialect Specific)
-- PostgreSQL / BigQuery / SQL Server:
SELECT 
    endpoint,
    PERCENTILE_CONT(0.50) WITHIN GROUP (ORDER BY latency_ms) AS median_latency,
    PERCENTILE_CONT(0.90) WITHIN GROUP (ORDER BY latency_ms) AS p90_latency
FROM user_latencies
GROUP BY endpoint;
```
- **Gotcha / Trap:** `NTILE(k)` distributes remainders starting from bucket 1 downwards. If 10 rows are placed into 4 buckets, bucket distribution will be: Bucket 1 (3 rows), Bucket 2 (3 rows), Bucket 3 (2 rows), Bucket 4 (2 rows).

---

# 3. Joins & Self-Joins

### Pattern 10: Self-Joins & Hierarchical Tree Traversal
- **When to Use:** When records in a single table maintain relationships with other rows in the exact same table (e.g. employee-to-manager, parent-category-to-child-category).
- **Sample Schema:** `employees(emp_id, emp_name, manager_id, salary)`
- **Common Question:** "Find all employees who earn more than their direct manager."
- **Query:**
```sql
SELECT 
    e.emp_id,
    e.emp_name AS employee_name,
    e.salary AS employee_salary,
    m.emp_name AS manager_name,
    m.salary AS manager_salary
FROM employees e
INNER JOIN employees m 
    ON e.manager_id = m.emp_id
WHERE e.salary > m.salary;
```
- **Gotcha / Trap:** Using `LEFT JOIN` when comparing values against the manager. If the CEO has `manager_id IS NULL`, `e.salary > m.salary` evaluates to `NULL` (falsy) anyway, but `INNER JOIN` expresses intent cleanly and eliminates root nodes immediately.

---

### Pattern 11: Anti-Joins (LEFT JOIN + IS NULL / NOT EXISTS)
- **When to Use:** Finding records in Table A that have no matching records in Table B (unengaged users, products never sold, abandoned carts).
- **Sample Schema:** 
  - `users(user_id, email, signup_date)`
  - `orders(order_id, user_id, order_date)`
- **Common Question:** "Find all registered users who have never placed an order."
- **Query (Two Standard Implementations):**
```sql
-- Style 1: LEFT JOIN + NULL Check (Canonical Anti-Join)
SELECT 
    u.user_id,
    u.email
FROM users u
LEFT JOIN orders o 
    ON u.user_id = o.user_id
WHERE o.user_id IS NULL;

-- Style 2: NOT EXISTS (Often optimal execution plan in Postgres/Oracle)
SELECT 
    u.user_id,
    u.email
FROM users u
WHERE NOT EXISTS (
    SELECT 1 
    FROM orders o 
    WHERE o.user_id = u.user_id
);
```
- **Gotcha / Trap:** Using `WHERE u.user_id NOT IN (SELECT user_id FROM orders)`. If `orders.user_id` contains even a **single NULL value**, `NOT IN` returns **0 rows** for the entire query due to 3-valued SQL logic (`NULL` comparison yields unknown). Always prefer `LEFT JOIN ... IS NULL` or `NOT EXISTS`.

---

### Pattern 12: Non-Equi Joins & Range Matching
- **When to Use:** Joining tables where match criteria are inequalities (`<`, `>`, `BETWEEN`) rather than exact equality (`=`) (e.g. tax brackets, salary grades, event overlaps).
- **Sample Schema:** 
  - `orders(order_id, order_date, amount)`
  - `promotions(promo_id, promo_code, start_date, end_date, discount_pct)`
- **Common Question:** "Match each order to the active promotional discount running on its purchase date."
- **Query:**
```sql
SELECT 
    o.order_id,
    o.order_date,
    o.amount,
    p.promo_code,
    p.discount_pct,
    ROUND(o.amount * (1.0 - COALESCE(p.discount_pct, 0.0) / 100.0), 2) AS discounted_amount
FROM orders o
LEFT JOIN promotions p 
    ON o.order_date >= p.start_date 
   AND o.order_date <= p.end_date;
```
- **Gotcha / Trap:** Overlapping ranges in Table B causing unintentional cartesian explosion. Always verify if ranges in the lookup table are strictly mutually exclusive.

---

# 4. Duplicates & Data Cleaning

### Pattern 13: Deduplication & Finding Duplicate Clusters
- **When to Use:** Identifying, counting, or deleting redundant duplicate records while retaining exactly one master record (e.g., lowest `id` or latest `created_at`).
- **Sample Schema:** `user_contacts(contact_id, user_id, email, phone_number, created_at)`
- **Common Question:** "Find duplicate email entries and write a query to return only the single oldest record per email."
- **Query:**
```sql
-- Step 1: Identifying duplicates
SELECT 
    email, 
    COUNT(*) AS occurrence_count
FROM user_contacts
GROUP BY email
HAVING COUNT(*) > 1;

-- Step 2: Extracting deduplicated clean dataset
WITH ranked_contacts AS (
    SELECT 
        contact_id,
        user_id,
        email,
        phone_number,
        created_at,
        ROW_NUMBER() OVER (
            PARTITION BY email 
            ORDER BY created_at ASC, contact_id ASC
        ) AS dedup_rank
    FROM user_contacts
)
SELECT 
    contact_id,
    user_id,
    email,
    phone_number,
    created_at
FROM ranked_contacts
WHERE dedup_rank = 1;
```
- **Gotcha / Trap:** Using `RANK()` or `DENSE_RANK()` for deduplication instead of `ROW_NUMBER()`. If two duplicate rows share the exact same `created_at`, `RANK` assigns `1` to both, failing to eliminate the duplicate.

---

### Pattern 14: Second / Nth Highest Value (Without Window Functions)
- **When to Use:** Classical algorithmic SQL test asking for the $N$th extreme value without relying on window functions (`DENSE_RANK`).
- **Sample Schema:** `employees(emp_id, emp_name, salary)`
- **Common Question:** "Find the second highest distinct salary from the employees table. Return NULL if no 2nd highest exists."
- **Query (Two Classic ANSI Approaches):**
```sql
-- Approach A: Subquery MAX with Inequality
SELECT MAX(salary) AS second_highest_salary
FROM employees
WHERE salary < (
    SELECT MAX(salary) 
    FROM employees
);

-- Approach B: Correlated Subquery for N-th Highest (Generic N=2)
SELECT DISTINCT e1.salary AS nth_highest_salary
FROM employees e1
WHERE 2 = (
    SELECT COUNT(DISTINCT e2.salary)
    FROM employees e2
    WHERE e2.salary >= e1.salary
);
```
- **Dialect Difference (LIMIT/OFFSET with Subquery Wrapper for NULL safety):**
```sql
-- MySQL / PostgreSQL:
SELECT (
    SELECT DISTINCT salary 
    FROM employees 
    ORDER BY salary DESC 
    LIMIT 1 OFFSET 1
) AS second_highest_salary;
```
- **Gotcha / Trap:** Writing `SELECT salary FROM employees ORDER BY salary DESC LIMIT 1 OFFSET 1` directly without a wrapper. If there is only 1 employee in the table, raw `LIMIT/OFFSET` returns **0 rows (empty)** instead of `NULL` (which interview test suites like LeetCode 176 expect).

---

# 5. Date/Time & Sequential Patterns

### Pattern 15: Gaps and Islands (Consecutive Active Streaks)
- **When to Use:** Grouping contiguous sequential events (e.g. consecutive days logged in, consecutive price increase runs, room booking blocks).
- **Sample Schema:** `user_logins(user_id, login_date)`
- **Common Question:** "Find the longest streak of consecutive days logged in for each user."
- **Query (The Difference-in-Row-Numbers / Date Subtraction Technique):**
```sql
WITH distinct_logins AS (
    -- Step 1: Remove multiple logins on the same day
    SELECT DISTINCT 
        user_id, 
        login_date 
    FROM user_logins
),
numbered_logins AS (
    -- Step 2: Assign sequential row numbers per user
    SELECT 
        user_id,
        login_date,
        ROW_NUMBER() OVER (PARTITION BY user_id ORDER BY login_date ASC) AS seq
    FROM distinct_logins
),
island_groups AS (
    -- Step 3: (login_date - seq days) generates a constant 'island_group_key' for consecutive dates
    SELECT 
        user_id,
        login_date,
        -- Postgres: login_date - (seq * INTERVAL '1 day')
        -- MySQL: DATE_SUB(login_date, INTERVAL seq DAY)
        -- SQL Server: DATEADD(day, -seq, login_date)
        DATE_SUB(login_date, INTERVAL seq DAY) AS island_key
    FROM numbered_logins
),
streak_lengths AS (
    -- Step 4: Count duration of each island
    SELECT 
        user_id,
        island_key,
        MIN(login_date) AS streak_start_date,
        MAX(login_date) AS streak_end_date,
        COUNT(*) AS streak_days
    FROM island_groups
    GROUP BY user_id, island_key
)
-- Step 5: Extract max streak per user
SELECT 
    user_id,
    MAX(streak_days) AS longest_consecutive_streak
FROM streak_lengths
GROUP BY user_id;
```
- **Gotcha / Trap:** Forgetting Step 1 (`DISTINCT`). If a user logs in 3 times on Monday and once on Tuesday, non-deduplicated row numbers break the `(date - seq)` mathematical alignment.

---

### Pattern 16: Date-Based Cohort Retention Analysis
- **When to Use:** Measuring product stickiness by tracking what percentage of users who signed up in Month 0 returned in Month 1, Month 2, etc.
- **Sample Schema:** 
  - `users(user_id, signup_date)`
  - `user_activity(user_id, activity_date)`
- **Common Question:** "Calculate the monthly retention rate for each user signup cohort across their first 3 active months."
- **Query:**
```sql
WITH user_cohorts AS (
    -- Step 1: Assign cohort month (Month 0)
    -- Postgres: DATE_TRUNC('month', signup_date)
    -- MySQL: DATE_FORMAT(signup_date, '%Y-%m-01')
    SELECT 
        user_id,
        DATE_FORMAT(signup_date, '%Y-%m-01') AS cohort_month
    FROM users
),
monthly_activities AS (
    -- Step 2: Calculate month difference between activity and signup cohort
    SELECT 
        c.cohort_month,
        a.user_id,
        -- Month index delta: (Year Diff * 12 + Month Diff)
        TIMESTAMPDIFF(MONTH, c.cohort_month, DATE_FORMAT(a.activity_date, '%Y-%m-01')) AS month_number
    FROM user_activity a
    JOIN user_cohorts c ON a.user_id = c.user_id
    GROUP BY c.cohort_month, a.user_id, month_number
),
cohort_sizes AS (
    -- Step 3: Base denominator (total users in cohort)
    SELECT 
        cohort_month, 
        COUNT(DISTINCT user_id) AS total_cohort_users
    FROM user_cohorts
    GROUP BY cohort_month
)
-- Step 4: Pivot retention percentages across month intervals
SELECT 
    cs.cohort_month,
    cs.total_cohort_users,
    ROUND(100.0 * COUNT(DISTINCT CASE WHEN ma.month_number = 0 THEN ma.user_id END) / cs.total_cohort_users, 1) AS m0_retention_pct,
    ROUND(100.0 * COUNT(DISTINCT CASE WHEN ma.month_number = 1 THEN ma.user_id END) / cs.total_cohort_users, 1) AS m1_retention_pct,
    ROUND(100.0 * COUNT(DISTINCT CASE WHEN ma.month_number = 2 THEN ma.user_id END) / cs.total_cohort_users, 1) AS m2_retention_pct
FROM cohort_sizes cs
LEFT JOIN monthly_activities ma ON cs.cohort_month = ma.cohort_month
GROUP BY cs.cohort_month, cs.total_cohort_users
ORDER BY cs.cohort_month;
```
- **Gotcha / Trap:** Joining `user_activity` without deduplicating multiple actions by the same user in a single calendar month, which distorts count aggregations.

---

### Pattern 17: Multi-Step Conversion Funnel Analysis
- **When to Use:** Tracking user drop-off across an ordered sequence of product events (e.g. Homepage View $\to$ Product Search $\to$ Add to Cart $\to$ Purchase).
- **Sample Schema:** `event_log(event_id, user_id, event_name, event_timestamp)`
- **Common Question:** "Calculate the conversion rate through the 3-step funnel: 'view_item' -> 'add_to_cart' -> 'checkout' on the same day."
- **Query:**
```sql
WITH user_funnel_timestamps AS (
    SELECT 
        user_id,
        MIN(CASE WHEN event_name = 'view_item' THEN event_timestamp END) AS t_view,
        MIN(CASE WHEN event_name = 'add_to_cart' THEN event_timestamp END) AS t_cart,
        MIN(CASE WHEN event_name = 'checkout' THEN event_timestamp END) AS t_checkout
    FROM event_log
    GROUP BY user_id
),
valid_funnel_stages AS (
    SELECT 
        user_id,
        CASE WHEN t_view IS NOT NULL THEN 1 ELSE 0 END AS step_1_view,
        CASE WHEN t_view IS NOT NULL AND t_cart > t_view THEN 1 ELSE 0 END AS step_2_cart,
        CASE WHEN t_view IS NOT NULL AND t_cart > t_view AND t_checkout > t_cart THEN 1 ELSE 0 END AS step_3_checkout
    FROM user_funnel_timestamps
)
SELECT 
    SUM(step_1_view) AS viewed_count,
    SUM(step_2_cart) AS added_to_cart_count,
    SUM(step_3_checkout) AS completed_checkout_count,
    ROUND(100.0 * SUM(step_2_cart) / NULLIF(SUM(step_1_view), 0), 2) AS view_to_cart_pct,
    ROUND(100.0 * SUM(step_3_checkout) / NULLIF(SUM(step_2_cart), 0), 2) AS cart_to_checkout_pct
FROM valid_funnel_stages;
```
- **Gotcha / Trap:** Ignoring event chronologies (e.g. checking `t_cart IS NOT NULL` without ensuring `t_cart > t_view`), which incorrectly counts users who added a previous item to their cart *before* viewing the current item.

---

# 6. Subqueries, CTEs & Set Operations

### Pattern 18: Correlated Subqueries & EXISTS
- **When to Use:** Evaluating existence or threshold conditions per outer row where the inner query depends dynamically on the current outer row value.
- **Sample Schema:** 
  - `departments(dept_id, dept_name)`
  - `employees(emp_id, dept_id, salary)`
- **Common Question:** "Find all departments that currently employ at least one worker with a salary exceeding $150,000."
- **Query:**
```sql
SELECT 
    d.dept_id,
    d.dept_name
FROM departments d
WHERE EXISTS (
    SELECT 1 
    FROM employees e
    WHERE e.dept_id = d.dept_id
      AND e.salary > 150000
);
```
- **Gotcha / Trap:** Using `IN (SELECT dept_id FROM employees WHERE salary > 150000)`. While equivalent here, `EXISTS` short-circuits evaluation on the first match and handles `NULL` values in subqueries safely.

---

### Pattern 19: Recursive CTEs (Hierarchies & Number Sequences)
- **When to Use:** Traversing arbitrary depth trees (org charts, folder trees, bill of materials) or generating continuous sequence tables (date ranges with zero gaps).
- **Sample Schema:** `org_hierarchy(emp_id, emp_name, manager_id)`
- **Common Question:** "Print the entire management reporting hierarchy under the CEO (emp_id = 1), including the organizational depth level."
- **Query:**
```sql
WITH RECURSIVE org_tree AS (
    -- 1. Base Anchor Member: Find Root CEO (Level 0)
    SELECT 
        emp_id,
        emp_name,
        manager_id,
        0 AS depth_level,
        CAST(emp_name AS VARCHAR(1000)) AS reporting_path
    FROM org_hierarchy
    WHERE manager_id IS NULL -- Root node
    
    UNION ALL
    
    -- 2. Recursive Member: Join inner child to previous CTE parent
    SELECT 
        e.emp_id,
        e.emp_name,
        e.manager_id,
        ot.depth_level + 1,
        CAST(CONCAT(ot.reporting_path, ' -> ', e.emp_name) AS VARCHAR(1000))
    FROM org_hierarchy e
    INNER JOIN org_tree ot 
        ON e.manager_id = ot.emp_id
)
SELECT 
    emp_id,
    emp_name,
    depth_level,
    reporting_path
FROM org_tree
ORDER BY depth_level ASC, emp_id ASC;
```
- **Dialect Difference:** SQL Server omits the `RECURSIVE` keyword (`WITH org_tree AS (...)`). PostgreSQL and MySQL require `WITH RECURSIVE`.
- **Gotcha / Trap:** Circular references in data (Employee A reports to B, B reports to A) will trigger infinite recursion. In production/interview follow-ups, limit recursion depth using `WHERE depth_level < 20`.

---

### Pattern 20: Set Operations (UNION vs. INTERSECT vs. EXCEPT)
- **When to Use:** Combining, intersecting, or computing differences between two query result sets sharing identical column signatures.
- **Sample Schema:** 
  - `web_visitors(user_id, visit_date)`
  - `app_visitors(user_id, visit_date)`
- **Common Question:** "Find users who visited via the web portal but never logged in via the mobile app."
- **Query:**
```sql
-- ANSI Standard EXCEPT (Set Difference)
SELECT user_id FROM web_visitors
EXCEPT
SELECT user_id FROM app_visitors;

-- Summary of Core Set Operations:
-- UNION: Combines distinct rows from both sets (deduplicates with sort overhead)
-- UNION ALL: Combines all rows without deduplication (fast, no sort)
-- INTERSECT: Returns only rows present in BOTH sets
-- EXCEPT / MINUS: Returns rows in Set 1 that do not exist in Set 2
```
- **Dialect Difference:** Oracle uses `MINUS` instead of `EXCEPT`. MySQL (prior to v8.0.31) lacks `EXCEPT`/`INTERSECT` and requires `LEFT JOIN ... IS NULL` or `INNER JOIN`.
- **Gotcha / Trap:** Defaulting to `UNION` when `UNION ALL` is sufficient. `UNION` runs an implicit expensive `DISTINCT` sort across the combined dataset.

---

# 7. Last-Day Revision Sheet

## 1. Logical SQL Query Execution Order
Write queries from top to bottom, but understand that the database engine evaluates clauses in this strict logical sequence:

$$\text{FROM} \to \text{ON} \to \text{JOIN} \to \text{WHERE} \to \text{GROUP BY} \to \text{HAVING} \to \text{WINDOW} \to \text{SELECT} \to \text{DISTINCT} \to \text{ORDER BY} \to \text{LIMIT / OFFSET}$$

| Step | Clause | Purpose | Key Implication for Interviews |
| :--- | :--- | :--- | :--- |
| **1** | `FROM / JOIN` | Identifies tables, computes Cartesian products, evaluates `ON` conditions | Table aliases are declared here and available everywhere else. |
| **2** | `WHERE` | Filters base individual rows before aggregation | Cannot use aggregate functions (`SUM`, `COUNT`) or `SELECT` aliases here. |
| **3** | `GROUP BY` | Collapses rows into summary groups | Non-aggregated columns in `SELECT` must appear in `GROUP BY`. |
| **4** | `HAVING` | Filters aggregated groups | Evaluated after grouping; aggregate conditions go here. |
| **5** | `WINDOW` | Computes window/analytic functions | Evaluated over partitioned result sets after `HAVING`. |
| **6** | `SELECT` | Emits columns, computes expressions, assigns aliases | Column aliases declared in `SELECT` cannot be filtered in `WHERE` or `HAVING`. |
| **7** | `DISTINCT` | Deduplicates emitted output rows | Applied after `SELECT` expressions are evaluated. |
| **8** | `ORDER BY` | Sorts final result rows | Can reference `SELECT` aliases and expressions. |
| **9** | `LIMIT / OFFSET` | Slices output row count | Executed last; dialects vary (`LIMIT`, `TOP`, `FETCH FIRST`). |

---

## 2. Window Function Syntax Cheat-Sheet

$$\text{FUNCTION}() \quad \text{OVER} \; \Big( \text{PARTITION BY } \text{col}_1, \dots \quad \text{ORDER BY } \text{col}_2 \; [\text{ASC}\mid\text{DESC}] \quad [\text{ROWS}\mid\text{RANGE} \; \text{FRAME\_CLAUSE}] \Big)$$

```sql
-- Frame Specification Variations:
ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW   -- Default for running totals with ORDER BY
ROWS BETWEEN 6 PRECEDING AND CURRENT ROW           -- Exact 7-row moving window (current + 6 prior)
ROWS BETWEEN 1 PRECEDING AND 1 FOLLOWING           -- 3-row centered window (1 prior + current + 1 next)
ROWS BETWEEN CURRENT ROW AND UNBOUNDED FOLLOWING   -- Lookahead forward calculation
```

---

## 3. Cross-Dialect Function Compatibility Matrix

| Requirement | PostgreSQL | MySQL (v8.0+) | SQL Server (T-SQL) | BigQuery (GoogleSQL) |
| :--- | :--- | :--- | :--- | :--- |
| **Null Replacement** | `COALESCE(col, val)` | `IFNULL(col, val)` / `COALESCE` | `ISNULL(col, val)` / `COALESCE` | `IFNULL(col, val)` / `COALESCE` |
| **Safe Division** | `a * 1.0 / NULLIF(b, 0)` | `a * 1.0 / NULLIF(b, 0)` | `a * 1.0 / NULLIF(b, 0)` | `IEEE_DIVIDE(a, b)` / `SAFE_DIVIDE(a, b)` |
| **Date Truncation** | `DATE_TRUNC('month', d)` | `DATE_FORMAT(d, '%Y-%m-01')` | `DATETRUNC(month, d)` | `DATE_TRUNC(d, MONTH)` |
| **Date Difference** | `d2 - d1` (days) | `DATEDIFF(d2, d1)` (days) | `DATEDIFF(day, d1, d2)` | `DATE_DIFF(d2, d1, DAY)` |
| **Date Addition** | `d + INTERVAL '7 days'` | `DATE_ADD(d, INTERVAL 7 DAY)` | `DATEADD(day, 7, d)` | `DATE_ADD(d, INTERVAL 7 DAY)` |
| **String Concat** | `col1 \|\| ' ' \|\| col2` | `CONCAT(col1, ' ', col2)` | `col1 + ' ' + col2` / `CONCAT` | `CONCAT(col1, ' ', col2)` |
| **String Slicing** | `SUBSTRING(s FROM 1 FOR 4)` | `SUBSTRING(s, 1, 4)` | `SUBSTRING(s, 1, 4)` | `SUBSTR(s, 1, 4)` |
| **Limiting Rows** | `LIMIT N OFFSET M` | `LIMIT N OFFSET M` | `TOP (N)` / `OFFSET M ROWS FETCH NEXT N` | `LIMIT N OFFSET M` |

---

## 4. The 5-Step Pre-Flight Checklist (Run Before Submitting Any Query)

1. **Clarify the Output Grain:** What does one row in the output represent? (e.g. One row per customer? One row per customer per month?)
2. **Scan for NULL Poisoning:** Are there `NULL` values in join keys, `NOT IN` subqueries, or division denominators? (Always wrap denominators in `NULLIF(den, 0)`).
3. **Verify Ties in Ranking:** Did the interviewer ask for "Top 3"? Clarify: "If the 3rd and 4th candidates tie in score, should both be returned?" (`DENSE_RANK` vs. `ROW_NUMBER`).
4. **Check for Join Fan-Out:** If joining Table A to two separate 1-to-many child tables B and C, are you aggregating after the join? (Pre-aggregate in CTEs first!).
5. **Inspect Date Boundary Inclusivity:** Are ranges inclusive or exclusive? (e.g. `BETWEEN '2023-01-01' AND '2023-01-31'` includes timestamps at midnight, but misses `2023-01-31 15:30:00` if stored as `DATETIME`).

---

## 5. Question Keyword to Query Pattern Cheat Sheet

| When you see this phrase in the prompt... | Immediately use this Pattern... | Core Function / Syntax Trigger |
| :--- | :--- | :--- |
| **"At least $N$ times" / "More than $X$ occurrences"** | `GROUP BY ... HAVING` | `HAVING COUNT(*) >= N` |
| **"Compare to previous period / Month-over-Month"** | Analytic Window Function | `LAG(metric, 1) OVER (ORDER BY date)` |
| **"Consecutive days / active streak"** | Gaps & Islands Technique | `DATE_SUB(date, INTERVAL ROW_NUMBER() OVER (...) DAY)` |
| **"Top $K$ items per category"** | Ranked Window CTE Filter | `DENSE_RANK() OVER (PARTITION BY cat ORDER BY val DESC) <= K` |
| **"Users who did action A but never did B"** | Anti-Join | `LEFT JOIN B ON ... WHERE B.id IS NULL` |
| **"Running balance / cumulative revenue"** | Cumulative Window Sum | `SUM(val) OVER (PARTITION BY id ORDER BY date ROWS UNBOUNDED PRECEDING)` |
| **"Rolling 7-day moving average"** | Sliding Window Frame | `AVG(val) OVER (ORDER BY date ROWS BETWEEN 6 PRECEDING AND CURRENT ROW)` |
| **"Hierarchy / Manager-Employee / Org tree"** | Self-Join or Recursive CTE | `WITH RECURSIVE tree AS (...)` |
| **"Pivoting rows into columns / conditional counts"** | Conditional Aggregation | `SUM(CASE WHEN status = 'success' THEN 1 ELSE 0 END)` |
| **"Retention cohort by signup month"** | Date Cohort Grouping | `TIMESTAMPDIFF(MONTH, signup_month, activity_month)` |
