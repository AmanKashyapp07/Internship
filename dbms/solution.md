# SQL Solutions — 100 Interview Queries
> **Database**: `dbms-intern-prep`
> Each solution includes the query + a concise explanation of *why* this approach is used.

---

## Section I — Basic SELECT & Filtering

### Q1 — Active employees
```sql
SELECT name, salary FROM Employee WHERE is_active = TRUE;
```
> Simple boolean filter. `is_active` lets you soft-delete employees; always filter on it first in real schemas.

---

### Q2 — Distinct customer cities
```sql
SELECT DISTINCT city FROM Customer;
```
> `DISTINCT` removes duplicate city names so each city appears once.

---

### Q3 — Electronics products above ₹5,000
```sql
SELECT * FROM Product
WHERE category = 'Electronics' AND price > 5000;
```
> Compound `AND` filter — both conditions must be true simultaneously.

---

### Q4 — Employee count per department
```sql
SELECT department, COUNT(*) AS emp_count
FROM Employee
GROUP BY department;
```
> `GROUP BY` collapses rows with the same department; `COUNT(*)` counts rows in each group.

---

### Q5 — Min / Max / Avg salary
```sql
SELECT MIN(salary) AS min_sal,
       MAX(salary) AS max_sal,
       ROUND(AVG(salary), 2) AS avg_sal
FROM Employee;
```
> Three aggregate functions in one scan. `ROUND` keeps the output clean.

---

### Q6 — Customers in Delhi or Mumbai
```sql
SELECT * FROM Customer WHERE city IN ('Delhi', 'Mumbai');
```
> `IN (list)` is cleaner than chaining multiple `OR` conditions.

---

### Q7 — Cancelled orders
```sql
SELECT * FROM Orders WHERE status = 'Cancelled';
```
> Direct equality filter on an enum-style column.

---

### Q8 — Low-stock products
```sql
SELECT * FROM Product WHERE stock_qty < 60;
```
> Inventory alert pattern — useful for reorder triggers.

---

### Q9 — Employees hired after 2022-01-01
```sql
SELECT * FROM Employee WHERE hire_date > '2022-01-01';
```
> Date literals in ISO format (`YYYY-MM-DD`) work in all major SQL dialects.

---

### Q10 — Total revenue across all order items
```sql
SELECT SUM(quantity * unit_price) AS total_revenue FROM OrderItem;
```
> Revenue is computed on the fly — `quantity * unit_price` is a derived column.

---

## Section II — Aggregation & Grouping

### Q11 — Total budget per department
```sql
SELECT name, budget FROM Department ORDER BY budget DESC;
```
> Budget is stored directly on `Department`; no aggregation needed.

---

### Q12 — Average product price by category
```sql
SELECT category, ROUND(AVG(price), 2) AS avg_price
FROM Product
GROUP BY category;
```
> Classic group-then-aggregate. `ROUND` prevents overly long decimals.

---

### Q13 — Orders per customer
```sql
SELECT customer_id, COUNT(*) AS order_count
FROM Orders
GROUP BY customer_id;
```
> `COUNT(*)` over a grouped `customer_id` gives the order frequency.

---

### Q14 — Customer(s) who placed the most orders
```sql
SELECT customer_id, COUNT(*) AS order_count
FROM Orders
GROUP BY customer_id
ORDER BY order_count DESC
LIMIT 1;
```
> Sort descending and take the first row. Use a subquery with `MAX` if you need ties.

---

### Q15 — Department employee counts, sorted
```sql
SELECT department, COUNT(*) AS emp_count
FROM Employee
GROUP BY department
ORDER BY emp_count DESC;
```
> Adding `ORDER BY` after `GROUP BY` sorts the aggregated results.

---

### Q16 — Total sales per customer (Sales table)
```sql
SELECT customer_id, SUM(amount) AS total_sales
FROM Sales
GROUP BY customer_id;
```
> Sum each customer's transaction amounts to get their lifetime spend.

---

### Q17 — Average order value
```sql
SELECT order_id, ROUND(SUM(quantity * unit_price), 2) AS order_total
FROM OrderItem
GROUP BY order_id;
```
> First compute line totals, then sum per order. Use as a subquery if you need the overall average.

---

### Q18 — Month/year with the most orders
```sql
SELECT TO_CHAR(order_date, 'YYYY-MM') AS month,
       COUNT(*) AS order_count
FROM Orders
GROUP BY month
ORDER BY order_count DESC
LIMIT 1;
```
> `TO_CHAR` formats a date into a string bucket like `'2026-01'` for grouping.

---

### Q19 — Total quantity sold per category
```sql
SELECT p.category, SUM(oi.quantity) AS total_qty_sold
FROM OrderItem oi
JOIN Product p ON oi.product_id = p.product_id
GROUP BY p.category;
```
> Requires a join to get the category from `Product` before grouping.

---

### Q20 — Top-revenue product
```sql
SELECT p.name, SUM(oi.quantity * oi.unit_price) AS revenue
FROM OrderItem oi
JOIN Product p ON oi.product_id = p.product_id
GROUP BY p.name
ORDER BY revenue DESC
LIMIT 1;
```
> Revenue per product = sum of all line values across every order it appears in.

---

## Section III — Joins

### Q21 — Employee with their manager's name
```sql
SELECT e.name AS employee, m.name AS manager
FROM Employee e
JOIN Employee m ON e.manager_id = m.id;
```
> A **self-join** — the same table is aliased twice. The inner join excludes employees with no manager.

---

### Q22 — All employees including those without a manager
```sql
SELECT e.name AS employee,
       COALESCE(m.name, 'CEO') AS manager
FROM Employee e
LEFT JOIN Employee m ON e.manager_id = m.id;
```
> `LEFT JOIN` keeps employees whose `manager_id` is NULL. `COALESCE` replaces NULL with a default string.

---

### Q23 — Employees and their project names
```sql
SELECT e.name AS employee, p.name AS project
FROM Employee e
JOIN EmployeeProject ep ON e.id = ep.employee_id
JOIN Project p ON ep.project_id = p.project_id;
```
> A three-way join through the junction table `EmployeeProject`.

---

### Q24 — Employees not assigned to any project
```sql
SELECT e.name
FROM Employee e
LEFT JOIN EmployeeProject ep ON e.id = ep.employee_id
WHERE ep.employee_id IS NULL;
```
> `LEFT JOIN` + `WHERE ... IS NULL` is the canonical "not in" join pattern — faster than `NOT IN` on large tables.

---

### Q25 — Project with employee count
```sql
SELECT p.name, COUNT(ep.employee_id) AS team_size
FROM Project p
LEFT JOIN EmployeeProject ep ON p.project_id = ep.project_id
GROUP BY p.name;
```
> `LEFT JOIN` ensures projects with no employees still appear with a count of 0.

---

### Q26 — Customer names with orders
```sql
SELECT c.name, o.order_id, o.order_date
FROM Customer c
JOIN Orders o ON c.customer_id = o.customer_id;
```
> Standard `INNER JOIN` — only customers who have placed at least one order.

---

### Q27 — Customers who never ordered
```sql
SELECT c.name
FROM Customer c
LEFT JOIN Orders o ON c.customer_id = o.customer_id
WHERE o.order_id IS NULL;
```
> Classic **anti-join** pattern using `LEFT JOIN + NULL` check.

---

### Q28 — Order, customer name, distinct product count
```sql
SELECT o.order_id, c.name, COUNT(DISTINCT oi.product_id) AS product_count
FROM Orders o
JOIN Customer c   ON o.customer_id = c.customer_id
JOIN OrderItem oi ON o.order_id    = oi.order_id
GROUP BY o.order_id, c.name;
```
> `COUNT(DISTINCT ...)` avoids double-counting if a product appears twice in the same order.

---

### Q29 — Products never ordered
```sql
SELECT p.name
FROM Product p
LEFT JOIN OrderItem oi ON p.product_id = oi.product_id
WHERE oi.product_id IS NULL;
```
> Anti-join on `OrderItem` to surface unsold products.

---

### Q30 — Order items with line values
```sql
SELECT p.name, oi.quantity, oi.unit_price,
       (oi.quantity * oi.unit_price) AS line_total
FROM OrderItem oi
JOIN Product p ON oi.product_id = p.product_id;
```
> Derived column computed in-query; no need to store it in the table.

---

### Q31 — Employee with department name (join on name)
```sql
SELECT e.name, d.name AS dept_name, d.location
FROM Employee e
JOIN Department d ON e.department = d.name;
```
> Joining on `VARCHAR` name columns — works here but a `dept_id` FK would be more robust.

---

### Q32 — Projects with department names
```sql
SELECT p.name AS project, d.name AS department
FROM Project p
JOIN Department d ON p.dept_id = d.dept_id;
```
> Standard FK join between `Project` and `Department`.

---

### Q33 — Employees with same salary in different departments
```sql
SELECT a.name AS emp1, b.name AS emp2, a.salary
FROM Employee a
JOIN Employee b
  ON a.salary = b.salary
 AND a.department <> b.department
 AND a.id < b.id;
```
> Self-join on `salary`, filtered so departments differ. `a.id < b.id` prevents duplicated pairs.

---

### Q34 — Orders per tier
```sql
SELECT c.tier,
       COUNT(DISTINCT c.customer_id) AS customers,
       COUNT(o.order_id)             AS total_orders
FROM Customer c
LEFT JOIN Orders o ON c.customer_id = o.customer_id
GROUP BY c.tier;
```
> `COUNT(DISTINCT c.customer_id)` counts unique customers; `COUNT(o.order_id)` counts their total orders.

---

### Q35 — January 2026 orders with customer details
```sql
SELECT o.order_id, c.name, c.city, o.order_date
FROM Orders o
JOIN Customer c ON o.customer_id = c.customer_id
WHERE o.order_date BETWEEN '2026-01-01' AND '2026-01-31';
```
> `BETWEEN ... AND` is inclusive on both ends; equivalent to `>= AND <=`.

---

## Section IV — Subqueries & CTEs

### Q36 — Employees above company-wide average salary
```sql
SELECT * FROM Employee
WHERE salary > (SELECT AVG(salary) FROM Employee);
```
> The subquery runs once and returns a scalar, which the outer query compares against.

---

### Q37 — Employees above their department's average
```sql
SELECT e.name, e.department, e.salary
FROM Employee e
WHERE salary > (
    SELECT AVG(salary)
    FROM Employee
    WHERE department = e.department
);
```
> This is a **correlated subquery** — it references the outer query's row (`e.department`) and re-runs per row.

---

### Q38 — Department with highest average salary
```sql
SELECT department, ROUND(AVG(salary), 2) AS avg_sal
FROM Employee
GROUP BY department
ORDER BY avg_sal DESC
LIMIT 1;
```
> Sort the grouped result and take the first row.

---

### Q39 — Most expensive product per category
```sql
SELECT category, name, price
FROM (
    SELECT category, name, price,
           RANK() OVER (PARTITION BY category ORDER BY price DESC) AS rnk
    FROM Product
) t
WHERE rnk = 1;
```
> `RANK()` partitioned by category — handles ties (two products with the same max price both appear).

---

### Q40 — Customers with at least one Delivered order
```sql
SELECT DISTINCT c.name
FROM Customer c
WHERE EXISTS (
    SELECT 1 FROM Orders o
    WHERE o.customer_id = c.customer_id
      AND o.status = 'Delivered'
);
```
> `EXISTS` stops at the first matching row — more efficient than `IN` when the subquery is large.

---

### Q41 — Orders with total line value > 50,000
```sql
SELECT order_id, SUM(quantity * unit_price) AS order_total
FROM OrderItem
GROUP BY order_id
HAVING SUM(quantity * unit_price) > 50000;
```
> `HAVING` filters on aggregated values; you cannot use `WHERE` for this.

---

### Q42 — Top 3 most expensive products per category (CTE)
```sql
WITH ranked AS (
    SELECT category, name, price,
           DENSE_RANK() OVER (PARTITION BY category ORDER BY price DESC) AS rnk
    FROM Product
)
SELECT category, name, price
FROM ranked
WHERE rnk <= 3;
```
> CTE makes the windowed ranking readable; `DENSE_RANK` ensures ties don't skip ranks.

---

### Q43 — Employees who are also a manager
```sql
SELECT DISTINCT e.name
FROM Employee e
WHERE EXISTS (
    SELECT 1 FROM Employee m WHERE m.manager_id = e.id
);
```
> Correlated `EXISTS` subquery checks if any other row has `manager_id` pointing to this employee.

---

### Q44 — Customers who ordered every Furniture product
```sql
SELECT c.customer_id, c.name
FROM Customer c
WHERE NOT EXISTS (
    SELECT p.product_id
    FROM Product p
    WHERE p.category = 'Furniture'
      AND NOT EXISTS (
          SELECT 1
          FROM Orders o
          JOIN OrderItem oi ON o.order_id = oi.order_id
          WHERE o.customer_id = c.customer_id
            AND oi.product_id = p.product_id
      )
);
```
> **Relational Division** — the double `NOT EXISTS` idiom: "there is no Furniture product that this customer has NOT ordered."

---

### Q45 — Salary as % of department's total salary (CTE chain)
```sql
WITH dept_totals AS (
    SELECT department, SUM(salary) AS dept_salary
    FROM Employee
    GROUP BY department
)
SELECT e.name, e.department, e.salary,
       ROUND(100.0 * e.salary / dt.dept_salary, 2) AS salary_pct
FROM Employee e
JOIN dept_totals dt ON e.department = dt.department;
```
> A CTE pre-computes the department sums so the join is clean and the division is straightforward.

---

### Q46 — Second-highest product price (no LIMIT, no window)
```sql
SELECT MAX(price) AS second_highest
FROM Product
WHERE price < (SELECT MAX(price) FROM Product);
```
> Nested `MAX` subquery: exclude the global maximum, then take the `MAX` of what remains.

---

### Q47 — Orders by Gold-tier customers
```sql
SELECT o.*
FROM Orders o
WHERE o.customer_id IN (
    SELECT customer_id FROM Customer WHERE tier = 'Gold'
);
```
> Subquery with `IN` is clean when the list can be large. `EXISTS` would also work.

---

### Q48 — Recursive CTE: full management chain
```sql
WITH RECURSIVE chain AS (
    SELECT id, name, manager_id, 1 AS depth
    FROM Employee
    WHERE manager_id IS NULL          -- start at root managers
    UNION ALL
    SELECT e.id, e.name, e.manager_id, c.depth + 1
    FROM Employee e
    JOIN chain c ON e.manager_id = c.id
)
SELECT id, name, depth FROM chain ORDER BY depth, id;
```
> The anchor clause selects roots; the recursive clause joins children onto the growing result set.

---

### Q49 — Projects exceeding dept's average budget
```sql
SELECT p.name, p.budget, d.name AS department
FROM Project p
JOIN Department d ON p.dept_id = d.dept_id
WHERE p.budget > (
    SELECT AVG(p2.budget)
    FROM Project p2
    WHERE p2.dept_id = p.dept_id
);
```
> Correlated subquery computes the department average fresh for each project row.

---

### Q50 — Employees hired in the same month/year as another
```sql
WITH hire_groups AS (
    SELECT id, name, TO_CHAR(hire_date, 'YYYY-MM') AS hire_month
    FROM Employee
)
SELECT h1.name, h1.hire_month
FROM hire_groups h1
WHERE EXISTS (
    SELECT 1 FROM hire_groups h2
    WHERE h2.hire_month = h1.hire_month AND h2.id <> h1.id
);
```
> CTE extracts the `YYYY-MM` bucket; the self-referencing `EXISTS` finds cohort matches.

---

## Section V — Window Functions

### Q51 — Salary rank within department
```sql
SELECT name, department, salary,
       DENSE_RANK() OVER (PARTITION BY department ORDER BY salary DESC) AS dept_rank
FROM Employee;
```
> `DENSE_RANK` assigns the same rank to ties and doesn't skip ranks — correct for "2nd highest" problems.

---

### Q52 — 2nd highest salary per department
```sql
SELECT department, name, salary
FROM (
    SELECT department, name, salary,
           DENSE_RANK() OVER (PARTITION BY department ORDER BY salary DESC) AS rnk
    FROM Employee
) t
WHERE rnk = 2;
```
> Filter the windowed result to `rnk = 2`. Using `RANK` would also work but might return more rows on ties.

---

### Q53 — Order row number per customer
```sql
SELECT customer_id, order_id, order_date,
       ROW_NUMBER() OVER (PARTITION BY customer_id ORDER BY order_date) AS order_seq
FROM Orders;
```
> `ROW_NUMBER` generates unique sequential IDs — useful for "Nth order" queries.

---

### Q54 — Running total of sales per customer
```sql
SELECT customer_id, sale_date, amount,
       SUM(amount) OVER (PARTITION BY customer_id ORDER BY sale_date) AS running_total
FROM Sales;
```
> Window `SUM` with `ORDER BY` accumulates the total from the first row up to the current row.

---

### Q55 — Cumulative order count per day
```sql
SELECT order_date, COUNT(*) AS daily_orders,
       SUM(COUNT(*)) OVER (ORDER BY order_date) AS cumulative_orders
FROM Orders
GROUP BY order_date;
```
> `SUM` of the aggregate `COUNT(*)` — you can nest aggregate functions inside window functions.

---

### Q56 — Day-over-day stock price change (LAG)
```sql
SELECT trade_date, price,
       LAG(price) OVER (ORDER BY trade_date) AS prev_price,
       price - LAG(price) OVER (ORDER BY trade_date) AS daily_change
FROM StockPrice;
```
> `LAG(col)` accesses the value from the previous row; the first row gets NULL (no predecessor).

---

### Q57 — Next day's stock price (LEAD)
```sql
SELECT trade_date, price,
       LEAD(price) OVER (ORDER BY trade_date) AS next_price
FROM StockPrice;
```
> `LEAD(col)` accesses the value from the following row; the last row gets NULL.

---

### Q58 — 3-day moving average of stock prices
```sql
SELECT trade_date, price,
       ROUND(AVG(price) OVER (
           ORDER BY trade_date
           ROWS BETWEEN 2 PRECEDING AND CURRENT ROW
       ), 2) AS moving_avg_3d
FROM StockPrice;
```
> `ROWS BETWEEN 2 PRECEDING AND CURRENT ROW` anchors the frame to exactly 3 rows.

---

### Q59 — 3-day moving average of sales
```sql
SELECT sale_date, amount,
       ROUND(AVG(amount) OVER (
           ORDER BY sale_date
           ROWS BETWEEN 2 PRECEDING AND CURRENT ROW
       ), 2) AS moving_avg_3d
FROM Sales;
```
> Same frame clause applied to the `Sales` table.

---

### Q60 — Revenue % of order total per item
```sql
SELECT oi.order_id, p.name,
       (oi.quantity * oi.unit_price) AS line_total,
       ROUND(100.0 * (oi.quantity * oi.unit_price)
             / SUM(oi.quantity * oi.unit_price) OVER (PARTITION BY oi.order_id), 2) AS pct_of_order
FROM OrderItem oi
JOIN Product p ON oi.product_id = p.product_id;
```
> Window `SUM` partitioned by `order_id` gives the order total for each row to divide against.

---

### Q61 — First and last order date per customer
```sql
SELECT DISTINCT customer_id,
       FIRST_VALUE(order_date) OVER (PARTITION BY customer_id ORDER BY order_date
           ROWS BETWEEN UNBOUNDED PRECEDING AND UNBOUNDED FOLLOWING) AS first_order,
       LAST_VALUE(order_date)  OVER (PARTITION BY customer_id ORDER BY order_date
           ROWS BETWEEN UNBOUNDED PRECEDING AND UNBOUNDED FOLLOWING) AS last_order
FROM Orders;
```
> `LAST_VALUE` requires an explicit unbounded frame to see all rows; otherwise it defaults to the current row.

---

### Q62 — Salary quartiles with NTILE
```sql
SELECT name, salary,
       NTILE(4) OVER (ORDER BY salary) AS quartile
FROM Employee;
```
> `NTILE(n)` divides rows into `n` equal-sized buckets — useful for percentile-based segmentation.

---

### Q63 — Per-customer sale difference from previous
```sql
SELECT customer_id, sale_date, amount,
       amount - LAG(amount) OVER (PARTITION BY customer_id ORDER BY sale_date) AS diff_from_prev
FROM Sales;
```
> `PARTITION BY customer_id` ensures the lag looks at the same customer's previous row.

---

### Q64 — Highest single-day stock price gain
```sql
WITH daily AS (
    SELECT trade_date,
           price - LAG(price) OVER (ORDER BY trade_date) AS gain
    FROM StockPrice
)
SELECT trade_date, gain
FROM daily
ORDER BY gain DESC NULLS LAST
LIMIT 1;
```
> `NULLS LAST` ensures the first row (which has no predecessor) doesn't sort to the top.

---

### Q65 — De-duplicate Logins (keep first login per user per day)
```sql
WITH ranked AS (
    SELECT *,
           ROW_NUMBER() OVER (PARTITION BY user_id, login_date ORDER BY login_id) AS rn
    FROM Logins
)
SELECT user_id, login_date FROM ranked WHERE rn = 1;
```
> `ROW_NUMBER` partitioned by `(user_id, login_date)` numbers duplicates; keeping `rn = 1` retains only the earliest.

---

## Section VI — Advanced Scenarios

### Q66 — Consecutive login streaks ≥ 3 days
```sql
WITH deduped AS (
    SELECT DISTINCT user_id, login_date FROM Logins
),
numbered AS (
    SELECT user_id, login_date,
           ROW_NUMBER() OVER (PARTITION BY user_id ORDER BY login_date) AS rn
    FROM deduped
),
groups AS (
    SELECT user_id, login_date,
           login_date - CAST(rn AS INT) AS grp
    FROM numbered
)
SELECT user_id, MIN(login_date) AS streak_start,
                MAX(login_date) AS streak_end,
                COUNT(*)        AS streak_len
FROM groups
GROUP BY user_id, grp
HAVING COUNT(*) >= 3;
```
> **Gaps-and-Islands**: Subtracting `ROW_NUMBER` from a date creates an invariant group key for each consecutive run.

---

### Q67 — Employee tenure in years
```sql
SELECT name, hire_date,
       DATE_PART('year', AGE(CURRENT_DATE, hire_date)) AS tenure_years
FROM Employee;
```
> `AGE()` returns an interval; `DATE_PART('year', ...)` extracts the integer year component.

---

### Q68 — Salary tier with CASE
```sql
SELECT name, salary,
       CASE
           WHEN salary >= 90000 THEN 'Senior'
           WHEN salary >= 60000 THEN 'Mid'
           ELSE 'Junior'
       END AS tier
FROM Employee;
```
> `CASE WHEN` is evaluated top-down — the first matching branch wins.

---

### Q69 — Pivot: total sales per customer (Q1/Q2/Q3)
```sql
SELECT
    SUM(CASE WHEN customer_id = 1 THEN amount ELSE 0 END) AS cust_1,
    SUM(CASE WHEN customer_id = 2 THEN amount ELSE 0 END) AS cust_2,
    SUM(CASE WHEN customer_id = 3 THEN amount ELSE 0 END) AS cust_3
FROM Sales;
```
> **Conditional aggregation** — a portable manual pivot that works in every SQL dialect.

---

### Q70 — Highest salary via subquery only
```sql
SELECT name, salary FROM Employee
WHERE salary = (SELECT MAX(salary) FROM Employee);
```
> Returns all employees tied for the maximum — handles ties correctly without window functions.

---

### Q71 — Pairs sharing department and manager
```sql
SELECT a.name AS emp1, b.name AS emp2, a.department, a.manager_id
FROM Employee a
JOIN Employee b
  ON a.department = a.department
 AND a.manager_id = b.manager_id
 AND a.id < b.id
WHERE a.manager_id IS NOT NULL;
```
> `a.id < b.id` ensures each pair appears once rather than twice.

---

### Q72 — Most recently hired per department
```sql
SELECT department, name, hire_date
FROM (
    SELECT department, name, hire_date,
           RANK() OVER (PARTITION BY department ORDER BY hire_date DESC) AS rnk
    FROM Employee
) t
WHERE rnk = 1;
```
> `RANK()` within each department, ordering newest first.

---

### Q73 — Customers whose order revenue exceeds the average
```sql
WITH cust_revenue AS (
    SELECT o.customer_id, SUM(oi.quantity * oi.unit_price) AS total_rev
    FROM Orders o
    JOIN OrderItem oi ON o.order_id = oi.order_id
    GROUP BY o.customer_id
)
SELECT c.name, cr.total_rev
FROM cust_revenue cr
JOIN Customer c ON cr.customer_id = c.customer_id
WHERE cr.total_rev > (SELECT AVG(total_rev) FROM cust_revenue);
```
> Two-stage: compute per-customer totals in a CTE, then compare against the average of those totals.

---

### Q74 — Orders where every item is Electronics
```sql
SELECT order_id
FROM OrderItem oi
JOIN Product p ON oi.product_id = p.product_id
GROUP BY order_id
HAVING COUNT(*) = COUNT(CASE WHEN p.category = 'Electronics' THEN 1 END);
```
> If all items match, the conditional count equals the total count — a compact way to express universal quantification.

---

### Q75 — Month-over-month revenue growth
```sql
WITH monthly AS (
    SELECT TO_CHAR(sale_date, 'YYYY-MM') AS month,
           SUM(amount) AS revenue
    FROM Sales
    GROUP BY month
)
SELECT month, revenue,
       LAG(revenue) OVER (ORDER BY month) AS prev_revenue,
       ROUND(100.0 * (revenue - LAG(revenue) OVER (ORDER BY month))
             / NULLIF(LAG(revenue) OVER (ORDER BY month), 0), 2) AS growth_pct
FROM monthly;
```
> `NULLIF(..., 0)` prevents division-by-zero when previous revenue is 0.

---

### Q76 — Top 2 products by total quantity sold
```sql
SELECT p.name, SUM(oi.quantity) AS qty_sold
FROM OrderItem oi
JOIN Product p ON oi.product_id = p.product_id
GROUP BY p.name
ORDER BY qty_sold DESC
LIMIT 2;
```
> Simple aggregation + sort. Use `DENSE_RANK` if you need to handle ties properly.

---

### Q77 — Employees on more than one project
```sql
SELECT employee_id, COUNT(project_id) AS project_count
FROM EmployeeProject
GROUP BY employee_id
HAVING COUNT(project_id) > 1;
```
> `HAVING COUNT > 1` on the junction table identifies multi-project employees.

---

### Q78 — Most recently joined employee per project
```sql
SELECT project_id, employee_id, joined_date
FROM (
    SELECT project_id, employee_id, joined_date,
           RANK() OVER (PARTITION BY project_id ORDER BY joined_date DESC) AS rnk
    FROM EmployeeProject
) t
WHERE rnk = 1;
```
> Same `RANK() + filter` pattern used for "most recent" within each group.

---

### Q79 — Customers who ordered in 2+ distinct months
```sql
SELECT customer_id
FROM Orders
GROUP BY customer_id
HAVING COUNT(DISTINCT TO_CHAR(order_date, 'YYYY-MM')) >= 2;
```
> `COUNT(DISTINCT ...)` on the formatted month string counts unique calendar months.

---

### Q80 — Stock price % change first to last day
```sql
SELECT
    ROUND(100.0 * (last_price - first_price) / first_price, 2) AS pct_change
FROM (
    SELECT MIN(price) FILTER (WHERE trade_date = (SELECT MIN(trade_date) FROM StockPrice)) AS first_price,
           MIN(price) FILTER (WHERE trade_date = (SELECT MAX(trade_date) FROM StockPrice)) AS last_price
    FROM StockPrice
) t;
```
> `FILTER (WHERE ...)` is a PostgreSQL aggregate filter that selects a specific row's value without a self-join.

---

### Q81 — Products ordered by more than one customer
```sql
SELECT oi.product_id, p.name, COUNT(DISTINCT o.customer_id) AS customer_count
FROM OrderItem oi
JOIN Orders o  ON oi.order_id   = o.order_id
JOIN Product p ON oi.product_id = p.product_id
GROUP BY oi.product_id, p.name
HAVING COUNT(DISTINCT o.customer_id) > 1;
```
> `COUNT(DISTINCT customer_id)` counts unique buyers; `HAVING > 1` filters for multi-customer products.

---

### Q82 — Average days between consecutive orders per customer
```sql
WITH ordered AS (
    SELECT customer_id, order_date,
           LAG(order_date) OVER (PARTITION BY customer_id ORDER BY order_date) AS prev_date
    FROM Orders
),
gaps AS (
    SELECT customer_id,
           order_date - prev_date AS gap_days
    FROM ordered
    WHERE prev_date IS NOT NULL
)
SELECT customer_id, ROUND(AVG(gap_days), 1) AS avg_gap_days
FROM gaps
GROUP BY customer_id;
```
> `LAG` computes adjacent differences; averaging them gives the inter-order cadence.

---

### Q83 — Employee with total project budget they're assigned to
```sql
SELECT e.name,
       SUM(p.budget) AS total_project_budget
FROM Employee e
JOIN EmployeeProject ep ON e.id = ep.employee_id
JOIN Project p          ON ep.project_id = p.project_id
GROUP BY e.name;
```
> Three-table join aggregated by employee name.

---

### Q84 — Departments where every employee earns the same
```sql
SELECT department
FROM Employee
GROUP BY department
HAVING MAX(salary) = MIN(salary);
```
> If `MAX = MIN`, all values are identical — a single comparison check replaces a complex self-join.

---

### Q85 — Orders with more than 2 distinct products
```sql
SELECT order_id, COUNT(DISTINCT product_id) AS product_count
FROM OrderItem
GROUP BY order_id
HAVING COUNT(DISTINCT product_id) > 2;
```
> `COUNT(DISTINCT product_id)` per order; filter on the aggregate with `HAVING`.

---

## Section VII — NULL Handling, String & Date Functions

### Q86 — Replace NULL manager_id with 'No Manager'
```sql
SELECT name,
       COALESCE(CAST(manager_id AS VARCHAR), 'No Manager') AS manager
FROM Employee;
```
> `COALESCE` returns the first non-NULL value; the integer must be cast to VARCHAR to match types.

---

### Q87 — Employees whose name contains 'son' (case-insensitive)
```sql
SELECT * FROM Employee WHERE name ILIKE '%son%';
```
> `ILIKE` is PostgreSQL's case-insensitive `LIKE`. In standard SQL, use `LOWER(name) LIKE '%son%'`.

---

### Q88 — UPPER name / LOWER department
```sql
SELECT UPPER(name) AS emp_name, LOWER(department) AS dept
FROM Employee;
```
> Standard `UPPER` and `LOWER` string functions.

---

### Q89 — Concatenate name and department
```sql
SELECT name || ' — ' || department AS label FROM Employee;
```
> `||` is the SQL string concatenation operator. In MySQL use `CONCAT()`.

---

### Q90 — Product name length, sorted descending
```sql
SELECT name, LENGTH(name) AS name_length
FROM Product
ORDER BY name_length DESC;
```
> `LENGTH()` returns character count. Sorted longest-to-shortest.

---

### Q91 — Orders in the last 60 days
```sql
SELECT * FROM Orders
WHERE order_date >= (SELECT MAX(order_date) FROM Orders) - INTERVAL '60 days';
```
> Using `MAX(order_date)` as the reference point makes the query reproducible on any snapshot of data.

---

### Q92 — Days from registration to first order per customer
```sql
WITH first_orders AS (
    SELECT customer_id, MIN(order_date) AS first_order
    FROM Orders GROUP BY customer_id
)
SELECT c.name,
       fo.first_order - c.registration_date AS days_to_first_order
FROM Customer c
JOIN first_orders fo ON c.customer_id = fo.customer_id;
```
> Date subtraction in PostgreSQL returns an integer number of days.

---

### Q93 — Day of week for each login
```sql
SELECT user_id, login_date,
       TO_CHAR(login_date, 'Day') AS day_of_week
FROM Logins;
```
> `TO_CHAR(date, 'Day')` returns the full weekday name (e.g. `'Monday   '`). Trim with `TRIM()` if needed.

---

### Q94 — Stock prices on a Monday
```sql
SELECT * FROM StockPrice
WHERE EXTRACT(DOW FROM trade_date) = 1;
```
> `EXTRACT(DOW ...)` returns 0=Sunday, 1=Monday … 6=Saturday in PostgreSQL.

---

### Q95 — Full months of tenure per employee
```sql
SELECT name,
       EXTRACT(YEAR  FROM AGE(CURRENT_DATE, hire_date)) * 12
       + EXTRACT(MONTH FROM AGE(CURRENT_DATE, hire_date)) AS months_tenure
FROM Employee;
```
> Multiply years by 12 and add residual months for exact month count.

---

### Q96 — Customers registered in 2022
```sql
SELECT * FROM Customer
WHERE EXTRACT(YEAR FROM registration_date) = 2022;
```
> `EXTRACT(YEAR ...)` is cleaner than `BETWEEN` when you only care about the year.

---

### Q97 — Products starting with 'W' or ending with 'e'
```sql
SELECT * FROM Product
WHERE name LIKE 'W%' OR name LIKE '%e';
```
> Two `LIKE` patterns combined with `OR`.

---

### Q98 — Tag orders as Recent or Old
```sql
SELECT order_id, order_date,
       CASE
           WHEN order_date >= (SELECT MAX(order_date) FROM Orders) - INTERVAL '90 days'
           THEN 'Recent'
           ELSE 'Old'
       END AS recency
FROM Orders;
```
> Combines a subquery scalar with a `CASE` expression for dynamic categorization.

---

### Q99 — Count NULLs in manager_id and end_date
```sql
SELECT
    (SELECT COUNT(*) FROM Employee WHERE manager_id IS NULL) AS null_managers,
    (SELECT COUNT(*) FROM Project  WHERE end_date   IS NULL) AS null_end_dates;
```
> Two scalar subqueries in the same `SELECT` — each independently counts NULLs in a different table.

---

### Q100 — Comma-separated employee names per department
```sql
SELECT department,
       STRING_AGG(name, ', ' ORDER BY name) AS employees
FROM Employee
GROUP BY department;
```
> `STRING_AGG(col, sep ORDER BY ...)` is PostgreSQL's group concatenation function (equivalent to `GROUP_CONCAT` in MySQL).
