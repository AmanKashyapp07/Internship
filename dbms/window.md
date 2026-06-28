# SQL Window Functions: The Ultimate Guide 🏆

Window functions perform calculations across a set of table rows that are related to the current row, without collapsing them into a single output row. The row-level detail is fully preserved while computing aggregate or ranking metrics.

---

## 1. Syntax & Logical Execution Order ⚙️

### The Window Function Template
```sql
<function> OVER (
    [PARTITION BY <columns>]
    [ORDER BY <columns>]
    [ROWS|RANGE <frame_specification>]
)
```

* **`PARTITION BY`**: Divides the rows into groups (or partitions) that share the same values. If omitted, the entire result set is treated as a single partition.
* **`ORDER BY`**: Defines the sort order of rows within each partition. This is mandatory for ranking and time-series value functions.
* **`ROWS` / `RANGE`**: Specifies the window frame (a subset of rows inside the partition) relative to the current row.

### Where Window Functions Run in Query Evaluation
Logical Query Execution Order:
`FROM` $\rightarrow$ `JOIN` $\rightarrow$ `WHERE` $\rightarrow$ `GROUP BY` $\rightarrow$ `HAVING` $\rightarrow$ **`SELECT` (Window Functions Evaluated Here)** $\rightarrow$ `DISTINCT` $\rightarrow$ `ORDER BY` $\rightarrow$ `LIMIT`

> ⚠️ **Interview Implication:** Because window functions are evaluated in the `SELECT` clause (after filtering), **you cannot place window functions in the `WHERE` or `HAVING` clauses**. To filter on the result of a window function, you must wrap it in a CTE or a subquery.
> 
> * ❌ *Incorrect:* `SELECT name FROM Employee WHERE RANK() OVER (ORDER BY salary DESC) <= 3;`
> *  *Correct:* `WITH CTE AS (SELECT name, RANK() OVER (ORDER BY salary DESC) as rnk FROM Employee) SELECT name FROM CTE WHERE rnk <= 3;`

---

## 2. Window Frame Clauses: ROWS vs RANGE vs GROUPS 🪟

The frame clause specifies which subset of rows within the partition are evaluated for the current row.

```
Window Partition
┌──────────────────────────────────────┐
│ UNBOUNDED PRECEDING                  │
│ ...                                  │
│ 1 PRECEDING                          │
│ CURRENT ROW    ◄─── [Current Row]    │
│ 1 FOLLOWING                          │
│ ...                                  │
│ UNBOUNDED FOLLOWING                  │
└──────────────────────────────────────┘
```

### Frame Boundary Keywords
* `UNBOUNDED PRECEDING`: Start from the first row of the partition.
* `N PRECEDING`: Go back `N` rows before the current row.
* `CURRENT ROW`: The row currently being evaluated.
* `N FOLLOWING`: Go forward `N` rows after the current row.
* `UNBOUNDED FOLLOWING`: Extend to the last row of the partition.

### Frame Types: ROWS vs RANGE vs GROUPS

* **`ROWS`**: Evaluates physical row offsets.
  * `ROWS BETWEEN 2 PRECEDING AND CURRENT ROW` includes exactly the current row and the 2 physical rows immediately before it.
* **`RANGE`**: Evaluates value-based offsets. It groups duplicate values in the `ORDER BY` column together.
  * If the sorted column has values `10, 10, 20`, evaluating the second row (`10`) with `RANGE BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW` will include **both** rows containing `10` because their values match.
* **`GROUPS`**: Groups duplicate keys in the `ORDER BY` column together, but treats each unique group as a single unit when calculating physical offsets.

```sql
-- 3-day moving average (always exactly 3 physical rows)
AVG(amount) OVER (ORDER BY sale_date ROWS BETWEEN 2 PRECEDING AND CURRENT ROW)

-- Running total up to and including all rows with the same date as current (range-based)
SUM(amount) OVER (ORDER BY sale_date RANGE BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW)
```

> ⚠️ **Crucial Default Behavior Gotcha:**
> If you specify `ORDER BY` in your `OVER()` clause but omit the frame clause, the database implicitly applies:
> `RANGE BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW`
> This can cause slow execution (database sorting/ranges) and unexpected output if there are ties. **Always write an explicit `ROWS` frame for running totals and moving averages.**

---

## 3. Window Function Categories 📊

### A. Ranking Functions
Ranking functions assign integer values to rows based on their ordering partition.

* **`ROW_NUMBER()`**: Assigns a unique sequential integer to each row in the partition, starting at 1. Ties are resolved arbitrarily.
* **`RANK()`**: Assigns ranks, but creates gaps if there are ties. If two rows share rank 1, the next row is assigned rank 3 (`1, 1, 3, 4`).
* **`DENSE_RANK()`**: Assigns ranks without gaps. If two rows share rank 1, the next row is assigned rank 2 (`1, 1, 2, 3`).
* **`NTILE(N)`**: Distributes rows in a partition as evenly as possible into `N` buckets. Useful for quartiles (NTILE(4)) or deciles (NTILE(10)).
* **`PERCENT_RANK()`**: Returns the relative rank of a row as a decimal between `0` and `1` ($\frac{\text{Rank} - 1}{\text{Total Rows} - 1}$).
* **`CUME_DIST()`**: Calculates the cumulative distribution of a value ($\frac{\text{Rows with value} \le \text{current value}}{\text{Total Rows}}$).

#### Code Comparison
For a table containing salaries: `10000, 10000, 12000, 15000`

```sql
SELECT 
    salary,
    ROW_NUMBER() OVER (ORDER BY salary DESC) as row_num,  -- 1, 2, 3, 4
    RANK()       OVER (ORDER BY salary DESC) as rnk,      -- 1, 1, 3, 4
    DENSE_RANK() OVER (ORDER BY salary DESC) as dense_rnk -- 1, 1, 2, 3
FROM Employee;
```

---

### B. Value & Analytical Functions
These functions retrieve data values from other rows relative to the current row without utilizing self-joins.

* **`LAG(col, offset, default)`**: Accesses a value from the row that is `offset` positions *before* the current row. If no such row exists, returns `default` (or `NULL` if omitted).
* **`LEAD(col, offset, default)`**: Accesses a value from the row that is `offset` positions *after* the current row.
* **`FIRST_VALUE(col)`**: Returns the value of `col` from the first row in the window frame.
* **`LAST_VALUE(col)`**: Returns the value of `col` from the last row in the window frame.
* **`NTH_VALUE(col, N)`**: Returns the value of `col` from the N-th row in the window frame.

> ⚠️ **The `LAST_VALUE` Trap:**
> Because the default window frame is `RANGE BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW`, calling `LAST_VALUE(salary)` will evaluate only up to the *current row*, returning the current row's salary instead of the true last salary of the partition.
> 
> **Fix:** You must explicitly override the frame to scan the entire partition:
> `LAST_VALUE(col) OVER (ORDER BY col ROWS BETWEEN UNBOUNDED PRECEDING AND UNBOUNDED FOLLOWING)`

**Example: LAG, LEAD, and Extreme values**
```sql
SELECT 
    trade_date, 
    price,
    -- Yesterday's price; default to current price if first record
    LAG(price, 1, price) OVER (ORDER BY trade_date) AS yesterday_price,
    -- Tomorrow's price; default to current price if last record
    LEAD(price, 1, price) OVER (ORDER BY trade_date) AS tomorrow_price,
    -- Absolute highest stock price to date (first value in DESC order)
    FIRST_VALUE(price) OVER (ORDER BY price DESC) AS all_time_high
FROM StockPrice;
```

---

### C. Aggregate Window Functions
Performs standard aggregates (`SUM`, `AVG`, `COUNT`, `MIN`, `MAX`) over the defined window partition.

* **`SUM(col) OVER (...)`**: Calculates a running total or a rolling sum.
* **`AVG(col) OVER (...)`**: Calculates moving averages.
* **`COUNT(col) OVER (...)`**: Computes cumulative or rolling counts.

---

## 4. Advanced Interview Patterns & Recipes 🍳

### Recipe 1: Finding Top N per Group
> **Scenario:** Find the top 3 highest-paid employees in each department. Include ties.

```sql
WITH RankedEmployees AS (
    SELECT 
        department,
        name,
        salary,
        DENSE_RANK() OVER (PARTITION BY department ORDER BY salary DESC) as rnk
    FROM Employee
)
SELECT department, name, salary
FROM RankedEmployees
WHERE rnk <= 3;
```

---

### Recipe 2: Time-Series Growth Rate (MoM / YoY)
> **Scenario:** Compute Month-over-Month (MoM) revenue growth percentage.

```sql
WITH MonthlySales AS (
    SELECT 
        DATE_TRUNC('month', sale_date) as sale_month,
        SUM(amount) as current_month_revenue
    FROM Sales
    GROUP BY DATE_TRUNC('month', sale_date)
),
RevenueLag AS (
    SELECT 
        sale_month,
        current_month_revenue,
        LAG(current_month_revenue) OVER (ORDER BY sale_month) as prev_month_revenue
    FROM MonthlySales
)
SELECT 
    sale_month,
    current_month_revenue,
    COALESCE(prev_month_revenue, 0) as prev_month_revenue,
    ROUND(
        (current_month_revenue - prev_month_revenue) * 100.0 / NULLIF(prev_month_revenue, 0), 
        2
    ) as mom_growth_pct
FROM RevenueLag;
```

---

### Recipe 3: Gaps & Islands (Streaks & Consecutive Actions)
> **Scenario:** Find users with 3 or more consecutive days of active logins.

```sql
WITH UniqueLogins AS (
    SELECT DISTINCT user_id, CAST(login_date AS DATE) as login_date
    FROM Logins
),
RankedLogins AS (
    SELECT 
        user_id, 
        login_date,
        ROW_NUMBER() OVER (PARTITION BY user_id ORDER BY login_date) as rn
    FROM UniqueLogins
),
Islands AS (
    SELECT 
        user_id, 
        login_date,
        -- If dates are consecutive, subtracting rn increments will result in the same grp_date
        (login_date - INTERVAL '1 day' * rn) as grp_date
    FROM RankedLogins
)
SELECT 
    user_id, 
    MIN(login_date) as streak_start, 
    MAX(login_date) as streak_end, 
    COUNT(*) as consecutive_days
FROM Islands
GROUP BY user_id, grp_date
HAVING COUNT(*) >= 3;
```

---

### Recipe 4: Sessionization
> **Scenario:** Group consecutive clickstream events into sessions. A session ends if there are 30 minutes of inactivity.

```sql
WITH TimeDifferences AS (
    SELECT 
        user_id,
        click_timestamp,
        LAG(click_timestamp) OVER (PARTITION BY user_id ORDER BY click_timestamp) as prev_click
    FROM Clicks
),
SessionFlags AS (
    SELECT 
        user_id,
        click_timestamp,
        -- If it's the first click or gap is > 30 minutes, mark as a new session (1)
        CASE 
            WHEN prev_click IS NULL THEN 1
            WHEN click_timestamp - prev_click > INTERVAL '30 minutes' THEN 1
            ELSE 0
        END as is_new_session
    FROM TimeDifferences
),
SessionGroups AS (
    SELECT 
        user_id,
        click_timestamp,
        -- Running sum of flags creates incremental session IDs
        SUM(is_new_session) OVER (
            PARTITION BY user_id 
            ORDER BY click_timestamp 
            ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW
        ) as session_id
    FROM SessionFlags
)
SELECT 
    user_id, 
    session_id, 
    MIN(click_timestamp) as session_start, 
    MAX(click_timestamp) as session_end,
    MAX(click_timestamp) - MIN(click_timestamp) as session_duration
FROM SessionGroups
GROUP BY user_id, session_id;
```
