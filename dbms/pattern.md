# SQL Query Design Patterns for Top-Tier Tech Interviews 🚀

In interviews for data and backend engineering roles at top-tier tech companies (like Meta, Amazon, Google, Uber, Stripe, Netflix, and Zepto), the focus shifts away from basic `SELECT` and `JOIN` statements. Interviewers want to see how you handle real-world business analytics, complex edge cases, and massive datasets.

This document compiles the **8 core architectural patterns** that cover almost all top-tier SQL questions, complete with explanations, interview scenarios, code templates, and high-value performance tips.

---

## 1. Top N within a Category (Window Functions) 🥇

### The Scenario
You need to find the top performers, highest earners, or most popular products grouped by a specific category.
> **Common Question (Amazon/Google):** *Find the top 3 highest-paid employees in each department. If there is a tie, they should share the same rank.*

### Why they ask it
To test your understanding of partition grouping (`PARTITION BY`) and the subtle differences between ranking functions:
* **`ROW_NUMBER()`**: Unique sequential integers (`1, 2, 3, 4`). Gaps are never created; ties are resolved arbitrarily.
* **`RANK()`**: Assigns ranks with gaps for ties (`1, 1, 3, 4`).
* **`DENSE_RANK()`**: Assigns ranks without gaps for ties (`1, 1, 2, 3`). **This is almost always what interviewers want for salary/ranking questions.**

### Code Implementation
```sql
WITH RankedEmployees AS (
    SELECT 
        d.Name AS Department,
        e.Name AS Employee,
        e.Salary,
        -- DENSE_RANK ensures ties get the same rank and the next number is not skipped
        DENSE_RANK() OVER(PARTITION BY e.DepartmentId ORDER BY e.Salary DESC) as RankNum
    FROM Employee e
    JOIN Department d ON e.DepartmentId = d.Id
)
SELECT Department, Employee, Salary
FROM RankedEmployees
WHERE RankNum <= 3;
```

---

## 2. Consecutive Events (The "Gaps & Islands" Problem) 🏝️

### The Scenario
Tracking user streaks, system downtime, sensor activity, or repeated behaviors.
> **Common Question (Meta/Uber/LinkedIn):** *Write a query to find all users who logged into the app for 3 or more consecutive days.*

### Why they ask it
This is a classic test of analytical logic. You solve it using a mathematical trick: **if you subtract a sequence of row numbers from a sequence of dates, consecutive dates will yield the exact same baseline date.**

```
Logins:   2026-06-01, 2026-06-02, 2026-06-03, 2026-06-05, 2026-06-06
Row Num:  1,          2,          3,          4,          5
Diff:     2026-05-31, 2026-05-31, 2026-05-31, 2026-06-01, 2026-06-01
          \_________ Island 1 _________/     \_____ Island 2 _____/
```

### Code Implementation
```sql
WITH DistinctLogins AS (
    -- Step 1: Remove multiple logins on the same day by the same user
    SELECT DISTINCT user_id, CAST(login_date AS DATE) as login_date 
    FROM user_logins
),
GroupedDates AS (
    -- Step 2: Subtract a sequential row number from the date. 
    -- Consecutive dates will result in the same 'streak_group' date.
    SELECT 
        user_id, 
        login_date,
        -- PostgreSQL/standard SQL date arithmetic:
        login_date - INTERVAL '1 day' * ROW_NUMBER() OVER(PARTITION BY user_id ORDER BY login_date) AS streak_group
        -- Dialect variation (MySQL): 
        -- DATE_SUB(login_date, INTERVAL ROW_NUMBER() OVER(PARTITION BY user_id ORDER BY login_date) DAY) AS streak_group
    FROM DistinctLogins
)
-- Step 3: Count how many times a user appears in the same streak group
SELECT 
    user_id,
    MIN(login_date) as streak_start,
    MAX(login_date) as streak_end,
    COUNT(login_date) as streak_length
FROM GroupedDates
GROUP BY user_id, streak_group
HAVING COUNT(login_date) >= 3;
```

---

## 3. User Retention & Next Event Analysis (Time Deltas) 📈

### The Scenario
Calculating how fast users churn, customer lifetime patterns, or how quickly they take a second action.
> **Common Question (Amazon/Netflix):** *Find the IDs of users who made a second purchase within 7 days of their first purchase.*

### Why they ask it
To see if you know how to compare a row to previous/future rows using `LAG()`, `LEAD()`, or Self-Joins.

### Code Implementation
```sql
WITH RankedPurchases AS (
    SELECT 
        user_id, 
        purchase_date,
        ROW_NUMBER() OVER(PARTITION BY user_id ORDER BY purchase_date ASC) as purchase_order
    FROM purchases
)
-- Join the 1st purchase with the 2nd purchase for the same user
SELECT p1.user_id
FROM RankedPurchases p1
JOIN RankedPurchases p2 
  ON p1.user_id = p2.user_id AND p2.purchase_order = 2
WHERE p1.purchase_order = 1 
  -- Handle date differences (syntax depends on dialect; DATEDIFF works in MySQL/SQL Server)
  -- PostgreSQL: p2.purchase_date - p1.purchase_date <= 7
  AND DATEDIFF(p2.purchase_date, p1.purchase_date) <= 7;
```

> [!TIP]
> **Alternative (LAG/LEAD Method):** If the interviewer asks to find *any* consecutive purchases within 7 days (not just the 1st and 2nd), use `LAG()`:
> ```sql
> WITH SalesDelta AS (
>     SELECT user_id, purchase_date,
>            LAG(purchase_date) OVER (PARTITION BY user_id ORDER BY purchase_date) as prev_purchase
>     FROM purchases
> )
> SELECT DISTINCT user_id 
> FROM SalesDelta 
> WHERE DATEDIFF(purchase_date, prev_purchase) <= 7;
> ```

---

## 4. Conditional Aggregation (Funnel Metrics) 🎯

### The Scenario
Calculating business performance metrics like Click-Through Rate (CTR), Conversion Rates, or approval percentages across distinct partitions.
> **Common Question (Google/TikTok):** *Given an `Events` table, calculate the Click-Through Rate (clicks / impressions) for each ad campaign in 2026. Round to 2 decimal places.*

### Why they ask it
To check if you can transpose row-based events into columnar computations using `CASE WHEN` inside aggregates, and crucially, **if you remember to handle division-by-zero errors**.

### Code Implementation
```sql
SELECT 
    campaign_id,
    ROUND(
        -- Cast to float/decimal to prevent integer division dropping decimals
        SUM(CASE WHEN event_type = 'click' THEN 1.0 ELSE 0.0 END) / 
        -- NULLIF prevents a "divide by zero" error by returning NULL instead of crashing
        NULLIF(SUM(CASE WHEN event_type = 'impression' THEN 1.0 ELSE 0.0 END), 0) 
        * 100, 
    2) AS ctr_percentage
FROM AdEvents
WHERE EXTRACT(YEAR FROM event_timestamp) = 2026
GROUP BY campaign_id;
```

---

## 5. Rolling Metrics & Cumulative Sums 🔄

### The Scenario
Smoothing out volatile time-series data for dashboard reports or calculating running totals.
> **Common Question (Stripe/Square):** *Calculate the 3-month rolling average of total revenue.*

### Why they ask it
To test your command of advanced Window Function framing clauses (`ROWS BETWEEN`) and pre-aggregation.

### Code Implementation
```sql
WITH MonthlyRevenue AS (
    -- First, aggregate the transactional data by month
    SELECT 
        DATE_TRUNC('month', transaction_date) AS transaction_month, 
        SUM(amount) AS total_revenue
    FROM Transactions
    GROUP BY DATE_TRUNC('month', transaction_date)
)
-- Second, apply the rolling window
SELECT 
    transaction_month,
    total_revenue,
    AVG(total_revenue) OVER(
        ORDER BY transaction_month 
        ROWS BETWEEN 2 PRECEDING AND CURRENT ROW -- Current month + 2 previous months
    ) AS rolling_3m_avg_revenue
FROM MonthlyRevenue;
```

> [!IMPORTANT]
> Be sure to distinguish between `ROWS` (evaluates exactly physical rows offset) and `RANGE` (evaluates values/dates offset). In interviews, aggregating to the time unit first (like the CTE above) makes `ROWS` safe and highly performant.

---

## 6. Sessionization (User Journey Clickstreams) 🖱️

### The Scenario
Segmenting a continuous stream of events into distinct sessions based on inactivity thresholds (e.g., a 30-minute window of inactivity starts a new session).
> **Common Question (Uber/Netflix):** *Define a search session as a series of events by the same user where the gap between consecutive events is less than 30 minutes. Find the total number of sessions per user.*

### Why they ask it
This tests advanced sequencing logic. You solve it using a **Cumulative Flag Sum**: flag rows that start a new session, then compute a running sum of that flag to generate session IDs.

### Code Implementation
```sql
WITH TimeDifferences AS (
    SELECT 
        user_id,
        login_date,
        -- Get timestamp of previous login
        LAG(login_date) OVER (PARTITION BY user_id ORDER BY login_date) as prev_login
    FROM Logins
),
SessionFlags AS (
    SELECT 
        user_id,
        login_date,
        -- If it's the first login OR the gap is > 30 minutes, mark as a new session (1)
        CASE 
            WHEN prev_login IS NULL THEN 1
            WHEN login_date - prev_login > INTERVAL '30 minutes' THEN 1
            ELSE 0
        END as is_new_session
    FROM TimeDifferences
),
SessionGroups AS (
    SELECT 
        user_id,
        -- Cumulative sum of flags generates sequential session IDs per user
        SUM(is_new_session) OVER (
            PARTITION BY user_id 
            ORDER BY login_date 
            ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW
        ) as session_id
    FROM SessionFlags
)
-- Count unique sessions per user
SELECT 
    user_id, 
    COUNT(DISTINCT session_id) as total_sessions
FROM SessionGroups
GROUP BY user_id;
```

---

## 7. Relational Division ("Matching All") 🔀

### The Scenario
Identifying entities that satisfy a complete set of conditions or match all components of a target list.
> **Common Question (Amazon):** *Find all customers who have purchased every single product belonging to the 'Electronics' category.*

### Why they ask it
To check if you know how to match sets dynamically rather than hardcoding.

### Code Implementation
```sql
SELECT customer_id
FROM Orders o
JOIN OrderItem oi ON o.order_id = oi.order_id
JOIN Product p ON oi.product_id = p.product_id
WHERE p.category = 'Electronics'
GROUP BY customer_id
-- Validate that the count of unique products matched equals the total count of products in that category
HAVING COUNT(DISTINCT oi.product_id) = (
    SELECT COUNT(*) 
    FROM Product 
    WHERE category = 'Electronics'
);
```

---

## 8. Pivot / Unpivot (Matrix Representation) 📊

### The Scenario
Transposing columns into rows or vice-versa to restructure reports.
> **Common Question (Finance/Fintech):** *Pivot the sales table to display total quarterly sales columns for each customer for the year 2026.*

### Why they ask it
To test if you can perform complex aggregation groupings without relying on database-specific pivot utilities.

### Code Implementation
```sql
SELECT 
    customer_id,
    SUM(CASE WHEN EXTRACT(QUARTER FROM sale_date) = 1 THEN amount ELSE 0 END) AS Q1_Sales,
    SUM(CASE WHEN EXTRACT(QUARTER FROM sale_date) = 2 THEN amount ELSE 0 END) AS Q2_Sales,
    SUM(CASE WHEN EXTRACT(QUARTER FROM sale_date) = 3 THEN amount ELSE 0 END) AS Q3_Sales,
    SUM(CASE WHEN EXTRACT(QUARTER FROM sale_date) = 4 THEN amount ELSE 0 END) AS Q4_Sales,
    SUM(amount) as Total_Annual_Sales
FROM Sales
WHERE EXTRACT(YEAR FROM sale_date) = 2026
GROUP BY customer_id
ORDER BY Total_Annual_Sales DESC;
```

---

## Pro-Tips for Top-Tier Interviews 💡

### 1. Structure Code with CTEs (`WITH` clauses)
Interviewers heavily prefer CTEs over nested subqueries. CTEs read from top to bottom, making them significantly easier to trace and debug during the screen.

### 2. Proactively Ask About Duplicate Data
Always ask: *"Can a user log in/make a purchase twice at the exact same timestamp?"*
* If **yes**, you must handle duplicate events (usually with a deduplicating `DISTINCT` or `ROW_NUMBER()` CTE) before running window functions. This shows real-world data engineering maturity.

### 3. State Edge Case Handling Upfront
Explicitly call out how you will handle:
* **NULLs:** Window functions ignoring NULLs, `NOT IN` failing on NULLs.
* **Ties:** Using `DENSE_RANK()` vs `ROW_NUMBER()`.
* **Zero Denominators:** Using `NULLIF(col, 0)`.
Doing this *before* you write the query immediately elevates your standing.

### 4. Optimize the Query Under the Hood
If asked how to optimize, suggest:
* **Sargable Filters:** Write filters that use index schemas (`WHERE date_col >= '2026-01-01'` instead of `WHERE YEAR(date_col) = 2026`).
* **Avoid Self-Joins:** Prefer Window Functions (`LAG`, `LEAD`) over joins where possible to decrease operational scanning complexity from `O(N^2)` to `O(N log N)`.
* **Indexes:** Ensure columns used in `ON` and `GROUP BY` are indexed.
