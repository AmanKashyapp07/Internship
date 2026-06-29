## Complete Solution Set (With Explanations & Variants)

This section provides comprehensive solutions for the 30 target LeetCode SQL problems. Each problem contains multiple dialects/approaches, performance insights, and trap warnings.

### 1. [LeetCode 175: Combine Two Tables](https://leetcode.com/problems/combine-two-tables/)

**Problem:** Report `firstName`, `lastName`, `city`, and `state` for each person. If the address of a `personId` is not present, report `null`.

#### **Variant 1: Standard LEFT JOIN (Recommended)**
```sql
SELECT 
    p.firstName, 
    p.lastName, 
    a.city, 
    a.state
FROM Person p
LEFT JOIN Address a ON p.personId = a.personId;
```

#### **Variant 2: RIGHT JOIN Equivalent**
```sql
SELECT 
    p.firstName, 
    p.lastName, 
    a.city, 
    a.state
FROM Address a
RIGHT JOIN Person p ON a.personId = p.personId;
```

* **Key Difference & Performance:** `LEFT JOIN` preserves all rows in the left table (`Person`) regardless of whether there is a match in the right table (`Address`). Using `INNER JOIN` is a common mistake that filters out any person without an address record.

---

### 2. [LeetCode 176: Second Highest Salary](https://leetcode.com/problems/second-highest-salary/)

**Problem:** Find the second highest salary. Return `null` if it doesn't exist.

#### **Variant 1: Subquery with MAX (Most Portable)**
```sql
SELECT MAX(salary) AS SecondHighestSalary
FROM Employee
WHERE salary < (SELECT MAX(salary) FROM Employee);
```

#### **Variant 2: LIMIT/OFFSET with IFNULL/COALESCE (MySQL Optimized)**
```sql
SELECT COALESCE(
    (SELECT DISTINCT salary 
     FROM Employee 
     ORDER BY salary DESC 
     LIMIT 1 OFFSET 1), 
    NULL
) AS SecondHighestSalary;
```

#### **Variant 3: Window Function DENSE_RANK()**
```sql
WITH RankedSalaries AS (
    SELECT salary, DENSE_RANK() OVER (ORDER BY salary DESC) as rnk
    FROM Employee
)
SELECT MAX(salary) AS SecondHighestSalary
FROM RankedSalaries
WHERE rnk = 2;
```

* **Key Difference & Performance:** Variant 1 is extremely simple and naturally handles cases with fewer than 2 rows by returning `NULL`. Variant 2 is very fast due to index scans but requires wrapping inside a dummy select statement or using `COALESCE` to ensure it returns `NULL` when there is no second row.

---

### 3. [LeetCode 177: Nth Highest Salary](https://leetcode.com/problems/nth-highest-salary/)

**Problem:** Write a SQL function to get the Nth highest salary.

#### **Variant 1: MySQL Stored Function (LIMIT OFFSET)**
```sql
CREATE FUNCTION getNthHighestSalary(N INT) RETURNS INT
BEGIN
  DECLARE M INT;
  SET M = N - 1;
  RETURN (
      SELECT DISTINCT salary 
      FROM Employee 
      ORDER BY salary DESC 
      LIMIT 1 OFFSET M
  );
END
```

#### **Variant 2: Window Function DENSE_RANK()**
```sql
CREATE FUNCTION getNthHighestSalary(N INT) RETURNS INT
BEGIN
  RETURN (
      SELECT MAX(salary)
      FROM (
          SELECT salary, DENSE_RANK() OVER (ORDER BY salary DESC) AS rnk
          FROM Employee
      ) t
      WHERE rnk = N
  );
END
```

* **Key Difference & Performance:** Variant 1 performs an offset scan which is efficient but requires pre-calculating `N-1` because MySQL does not accept mathematical expressions (like `N-1`) directly inside `LIMIT/OFFSET`. Variant 2 handles ties properly using `DENSE_RANK()`.

---

### 4. [LeetCode 178: Rank Scores](https://leetcode.com/problems/rank-scores/)

**Problem:** Rank scores. If there is a tie, both get the same rank. The next rank should be the next consecutive integer.

#### **Variant 1: Standard Window Function DENSE_RANK() (Recommended)**
```sql
SELECT score, DENSE_RANK() OVER (ORDER BY score DESC) AS `rank`
FROM Scores;
```

#### **Variant 2: Correlated Subquery (Traditional SQL compatibility)**
```sql
SELECT s1.score,
       (SELECT COUNT(DISTINCT s2.score) 
        FROM Scores s2 
        WHERE s2.score >= s1.score) AS `rank`
FROM Scores s1
ORDER BY s1.score DESC;
```

* **Key Difference & Performance:** Variant 1 is evaluated in $O(N \log N)$ (sorting) or $O(N)$ (if indexed) and uses the database engine's native window optimizer. Variant 2 has quadratic time complexity $O(N^2)$ because the inner subquery scans the table for each row in the outer table.

---

### 5. [LeetCode 180: Consecutive Numbers](https://leetcode.com/problems/consecutive-numbers/)

**Problem:** Find all numbers that appear at least three times consecutively.

#### **Variant 1: Multi Self-Join (Simple & Readable)**
```sql
SELECT DISTINCT l1.num AS ConsecutiveNums
FROM Logs l1
JOIN Logs l2 ON l1.id = l2.id - 1
JOIN Logs l3 ON l1.id = l3.id - 2
WHERE l1.num = l2.num AND l2.num = l3.num;
```

#### **Variant 2: LEAD & LAG (Modern Window Function)**
```sql
SELECT DISTINCT num AS ConsecutiveNums
FROM (
    SELECT num,
           LAG(num, 1) OVER (ORDER BY id) AS prev,
           LEAD(num, 1) OVER (ORDER BY id) AS next
    FROM Logs
) t
WHERE num = prev AND num = next;
```

#### **Variant 3: Gaps and Islands (Generalizable to N consecutive rows)**
```sql
WITH numbered AS (
    SELECT num, id,
           ROW_NUMBER() OVER (ORDER BY id) AS row_num,
           ROW_NUMBER() OVER (PARTITION BY num ORDER BY id) AS num_row_num
    FROM Logs
)
SELECT DISTINCT num AS ConsecutiveNums
FROM numbered
GROUP BY num, (id - num_row_num)
HAVING COUNT(*) >= 3;
```

* **Key Difference & Performance:** Variant 1 assumes consecutive IDs have no gaps (e.g. `1, 2, 3`). If IDs have gaps (like `1, 3, 4`), Variant 1 fails. Variant 2 and 3 are robust to ID gaps. Variant 3 (Gaps and Islands) is highly generalized and works for any consecutive count threshold (e.g., 5 or 100 days).

---

### 6. [LeetCode 181: Employees Earning More Than Their Managers](https://leetcode.com/problems/employees-earning-more-than-their-managers/)

**Problem:** Find employees who earn more than their managers.

#### **Variant 1: Self-Join (Recommended)**
```sql
SELECT e.name AS Employee
FROM Employee e
JOIN Employee m ON e.managerId = m.id
WHERE e.salary > m.salary;
```

#### **Variant 2: Correlated Subquery**
```sql
SELECT name AS Employee
FROM Employee e
WHERE salary > (
    SELECT salary 
    FROM Employee m 
    WHERE m.id = e.managerId
);
```

* **Key Difference & Performance:** The self-join version allows the optimizer to perform nested loop or hash joins efficiently based on the `managerId` index. The correlated subquery version is executed row-by-row unless the database optimizer rewrites it into a join.

---

### 7. [LeetCode 182: Duplicate Emails](https://leetcode.com/problems/duplicate-emails/)

**Problem:** Find all duplicate emails.

#### **Variant 1: GROUP BY + HAVING (Standard)**
```sql
SELECT email AS Email
FROM Person
GROUP BY email
HAVING COUNT(email) > 1;
```

#### **Variant 2: Self-Join**
```sql
SELECT DISTINCT p1.email AS Email
FROM Person p1
JOIN Person p2 ON p1.email = p2.email AND p1.id <> p2.id;
```

* **Key Difference & Performance:** Variant 1 is far more performant because it leverages hashing/sorting for grouping. Variant 2 performs a self-join which can result in $O(N^2)$ runtime in the worst case.

---

### 8. [LeetCode 184: Department Highest Salary](https://leetcode.com/problems/department-highest-salary/)

**Problem:** Find employees who have the highest salary in each department.

#### **Variant 1: IN Subquery (Clean & Portable)**
```sql
SELECT d.name AS Department, e.name AS Employee, e.salary AS Salary
FROM Employee e
JOIN Department d ON e.departmentId = d.id
WHERE (e.departmentId, e.salary) IN (
    SELECT departmentId, MAX(salary)
    FROM Employee
    GROUP BY departmentId
);
```

#### **Variant 2: Window Function DENSE_RANK()**
```sql
WITH RankedSalaries AS (
    SELECT
        d.name AS Department,
        e.name AS Employee,
        e.salary,
        DENSE_RANK() OVER (
            PARTITION BY e.departmentId
            ORDER BY e.salary DESC
        ) AS rnk
    FROM Employee e
    JOIN Department d
        ON e.departmentId = d.id
)
SELECT Department, Employee, salary
FROM RankedSalaries
WHERE rnk = 1;
```

* **Key Difference & Performance:** Variant 1 is compatible with almost all database engines. Variant 2 is useful when you need details about the rank (e.g. finding the top 2 earners). Both correctly return multiple employees if there is a tie for the top salary.

---

### 9. [LeetCode 185: Department Top 3 Salaries](https://leetcode.com/problems/department-top-three-salaries/)

**Problem:** Find employees who earn a salary in the top three unique salaries for their department.

#### **Variant 1: Window Function DENSE_RANK() (Standard)**
```sql
WITH RankedSalaries AS (
    SELECT
        d.name AS Department,
        e.name AS Employee,
        e.salary,
        DENSE_RANK() OVER (
            PARTITION BY e.departmentId
            ORDER BY e.salary DESC
        ) AS rnk
    FROM Employee e
    JOIN Department d
        ON e.departmentId = d.id
)
SELECT
    Department,
    Employee,
    salary AS Salary
FROM RankedSalaries
WHERE rnk <= 3;
```

#### **Variant 2: Correlated Subquery (Traditional)**
```sql
SELECT d.name AS Department, e1.name AS Employee, e1.salary AS Salary
FROM Employee e1
JOIN Department d ON e1.departmentId = d.id
WHERE 3 > (
    SELECT COUNT(DISTINCT e2.salary)
    FROM Employee e2
    WHERE e2.departmentId = e1.departmentId AND e2.salary > e1.salary
);
```

* **Trap:** Do not use `RANK()` instead of `DENSE_RANK()`. Using `RANK()` will skip ranks (e.g., if there is a tie for 1st place, the ranks are `1, 1, 3`, meaning rank `4` is excluded). The problem specifies the top 3 *unique* salaries, which requires `DENSE_RANK()` (where ranks are `1, 1, 2, 3`).

---

### 10. [LeetCode 196: Delete Duplicate Emails](https://leetcode.com/problems/delete-duplicate-emails/)

**Problem:** Delete all duplicate email entries, keeping only the one with the smallest ID.

#### **Variant 1: Self-Join DELETE (MySQL Specific)**
```sql
DELETE p1 
FROM Person p1
JOIN Person p2 ON p1.email = p2.email AND p1.id > p2.id;
```

#### **Variant 2: Subquery with NOT IN (Standard SQL)**
```sql
DELETE FROM Person
WHERE id NOT IN (
    SELECT min_id FROM (
        SELECT MIN(id) AS min_id
        FROM Person
        GROUP BY email
    ) temp
);
```

#### **Variant 3: CTE with ROW_NUMBER() (Recommended for PostgreSQL / MS SQL)**
```sql
WITH DuplicateEmails AS (
    SELECT
        id,
        email,
        ROW_NUMBER() OVER (
            PARTITION BY email
            ORDER BY id
        ) AS rn
    FROM Person
)
DELETE FROM Person
WHERE id IN (
    SELECT id
    FROM DuplicateEmails
    WHERE rn > 1
);
```

* **MySQL Trap:** You cannot directly modify a table if it is referenced in the subquery of the same statement. For example, `DELETE FROM Person WHERE id NOT IN (SELECT MIN(id) FROM Person GROUP BY email)` will throw error `1093 (ER_UPDATE_TABLE_USED)`. You must wrap it in an intermediate temporary select (`SELECT min_id FROM (...) temp`) to trick the optimizer into materializing the subquery first. Also note that MySQL does not allow referencing CTEs inside a `DELETE` statement in some older versions.

---

### 11. [LeetCode 626: Exchange Seats](https://leetcode.com/problems/exchange-seats/)

**Problem:** Swap the seat IDs of consecutive students. If the number of students is odd, the last student's seat is not swapped.

#### **Variant 1: CASE Statement with MOD/Arithmetic (Fastest)**
```sql
SELECT 
    CASE 
        WHEN id % 2 = 1 AND id = (SELECT COUNT(*) FROM Seat) THEN id
        WHEN id % 2 = 1 THEN id + 1
        ELSE id - 1
    END AS id,
    student
FROM Seat
ORDER BY id;
```

#### **Variant 2: LEAD & LAG (Swapping student values, not IDs)**
```sql
SELECT 
    id,
    COALESCE(
        CASE 
            WHEN id % 2 = 1 THEN LEAD(student) OVER (ORDER BY id)
            ELSE LAG(student) OVER (ORDER BY id)
        END, 
        student
    ) AS student
FROM Seat;
```

* **Key Difference & Performance:** Variant 1 actually modifies the `id` key values during select (sorting them at the end). Variant 2 leaves `id` alone and swaps the corresponding `student` value using `LEAD` and `LAG`. In production, swapping values is safer because changing auto-increment or primary keys (`id`) is an anti-pattern.

---

### 12. [LeetCode 1179: Reformat Department Table](https://leetcode.com/problems/reformat-department-table/)

**Problem:** Pivot table to display department ID and revenue columns for each month.

#### **Variant 1: Conditional Aggregation (Standard Pivot)**
```sql
SELECT id,
    SUM(CASE WHEN month = 'Jan' THEN revenue END) AS Jan_Revenue,
    SUM(CASE WHEN month = 'Feb' THEN revenue END) AS Feb_Revenue,
    SUM(CASE WHEN month = 'Mar' THEN revenue END) AS Mar_Revenue,
    SUM(CASE WHEN month = 'Apr' THEN revenue END) AS Apr_Revenue,
    SUM(CASE WHEN month = 'May' THEN revenue END) AS May_Revenue,
    SUM(CASE WHEN month = 'Jun' THEN revenue END) AS Jun_Revenue,
    SUM(CASE WHEN month = 'Jul' THEN revenue END) AS Jul_Revenue,
    SUM(CASE WHEN month = 'Aug' THEN revenue END) AS Aug_Revenue,
    SUM(CASE WHEN month = 'Sep' THEN revenue END) AS Sep_Revenue,
    SUM(CASE WHEN month = 'Oct' THEN revenue END) AS Oct_Revenue,
    SUM(CASE WHEN month = 'Nov' THEN revenue END) AS Nov_Revenue,
    SUM(CASE WHEN month = 'Dec' THEN revenue END) AS Dec_Revenue
FROM Department
GROUP BY id;
```

* **Why Aggregate?** If you omit the `SUM()` function, SQL will require all unaggregated columns to be present in `GROUP BY`, which would collapse nothing. The `SUM` (or `MAX`) function compresses all rows belonging to the same department `id` into a single record.

---

### 13. [LeetCode 183: Customers Who Never Order](https://leetcode.com/problems/customers-who-never-order/)

**Problem:** Find all customers who never order anything.

#### **Variant 1: LEFT JOIN (Recommended)**
```sql
SELECT c.name AS Customers
FROM Customers c
LEFT JOIN Orders o ON c.id = o.customerId
WHERE o.id IS NULL;
```

#### **Variant 2: NOT IN Subquery (Watch out for NULLs!)**
```sql
SELECT name AS Customers
FROM Customers
WHERE id NOT IN (
    SELECT DISTINCT customerId 
    FROM Orders 
    WHERE customerId IS NOT NULL
);
```

* **Key Difference & Performance:** The `LEFT JOIN` checks for non-existence using index lookups and is highly optimized. If using `NOT IN`, ensure you filter out `NULL` values from the subquery, otherwise the entire outer query will return an empty set.

---

### 14. [LeetCode 197: Rising Temperature](https://leetcode.com/problems/rising-temperature/)

**Problem:** Find all dates' IDs with higher temperatures compared to their previous dates (yesterday).

#### **Variant 1: Self-Join (Recommended)**
```sql
SELECT w1.id
FROM Weather w1
JOIN Weather w2 ON w1.recordDate = DATE_ADD(w2.recordDate, INTERVAL 1 DAY)
WHERE w1.temperature > w2.temperature;
```

#### **Variant 2: Window Function LAG()**
```sql
SELECT id
FROM (
    SELECT id, recordDate, temperature,
           LAG(temperature) OVER (ORDER BY recordDate) AS prev_temp,
           LAG(recordDate) OVER (ORDER BY recordDate) AS prev_date
    FROM Weather
) t
WHERE temperature > prev_temp 
  AND recordDate = DATE_ADD(prev_date, INTERVAL 1 DAY);
```

* **Key Difference & Performance:** Variant 1 is simple but relies on date math. Variant 2 uses window functions which are safer if record dates have gaps because we explicitly check if the lag date matches `yesterday`.

---

### 15. [LeetCode 511: Game Play Analysis I](https://leetcode.com/problems/game-play-analysis-i/)

**Problem:** Find the first login date for each player.

#### **Variant 1: Simple GROUP BY (Recommended)**
```sql
SELECT player_id, MIN(event_date) AS first_login
FROM Activity
GROUP BY player_id;
```

#### **Variant 2: Window Function ROW_NUMBER()**
```sql
SELECT player_id, event_date AS first_login
FROM (
    SELECT player_id, event_date,
           ROW_NUMBER() OVER (PARTITION BY player_id ORDER BY event_date) AS rn
    FROM Activity
) t
WHERE rn = 1;
```

* **Key Difference & Performance:** Variant 1 is optimal and standard for retrieving aggregates alone. Variant 2 is useful if you need to fetch extra columns from the same "first login" record (e.g. first logged device).

---

### 16. [LeetCode 584: Find Customer Referee](https://leetcode.com/problems/find-customer-referee/)

**Problem:** Find the names of customers that are not referred by the customer with ID = 2.

#### **Variant 1: OR with NULL Check (Standard)**
```sql
SELECT name
FROM Customer
WHERE referee_id <> 2 OR referee_id IS NULL;
```

#### **Variant 2: COALESCE / IFNULL (MySQL Specific)**
```sql
SELECT name
FROM Customer
WHERE COALESCE(referee_id, 0) <> 2;
```

* **Trap:** In SQL, three-valued logic applies (`TRUE`, `FALSE`, `UNKNOWN`). Comparisons like `referee_id <> 2` evaluate to `UNKNOWN` when `referee_id` is `NULL`, which filters them out. You must explicitly handle `NULL` cases.

---

### 17. [LeetCode 586: Customer Placing the Largest Number of Orders](https://leetcode.com/problems/customer-placing-the-largest-number-of-orders/)

**Problem:** Find the customer number who has placed the largest number of orders.

#### **Variant 1: LIMIT 1 (MySQL/PostgreSQL)**
```sql
SELECT customer_number
FROM Orders
GROUP BY customer_number
ORDER BY COUNT(*) DESC
LIMIT 1;
```

#### **Variant 2: Subquery with ALL (Handles Ties)**
```sql
SELECT customer_number
FROM Orders
GROUP BY customer_number
HAVING COUNT(*) >= ALL (
    SELECT COUNT(*)
    FROM Orders
    GROUP BY customer_number
);
```

* **Key Difference & Performance:** Variant 1 is highly efficient but only returns one customer even if there is a tie. Variant 2 is standard ANSI SQL and returns all customers tied for the most orders.

---

### 18. [LeetCode 595: Big Countries](https://leetcode.com/problems/big-countries/)

**Problem:** Find big countries by area (>= 3M) or population (>= 25M).

#### **Variant 1: Simple OR**
```sql
SELECT name, population, area
FROM World
WHERE area >= 3000000 OR population >= 25000000;
```

#### **Variant 2: UNION (Optimized for Indexes)**
```sql
SELECT name, population, area FROM World WHERE area >= 3000000
UNION
SELECT name, population, area FROM World WHERE population >= 25000000;
```

* **Performance:** When indexes exist on both `area` and `population`, `UNION` is often faster because database engines can scan both indexes separately and merge results, whereas `OR` may force a full table scan.

---

### 19. [LeetCode 596: Classes More Than 5 Students](https://leetcode.com/problems/classes-more-than-5-students/)

**Problem:** Find all classes that have at least five students.

#### **Variant 1: GROUP BY + HAVING (Recommended)**
```sql
SELECT class
FROM Courses
GROUP BY class
HAVING COUNT(DISTINCT student) >= 5;
```

* **Key Detail:** The problem requires counting distinct students per class to ensure duplicate student course registrations are not counted multiple times.

---

### 20. [LeetCode 601: Human Traffic of Stadium](https://leetcode.com/problems/human-traffic-of-stadium/)

**Problem:** Find rows with consecutive IDs where traffic >= 100 on each of those consecutive days.

#### **Variant 1: Gaps & Islands (General)**
```sql
WITH group_consecutive AS (
    SELECT id, visit_date, people,
           id - ROW_NUMBER() OVER (ORDER BY id) AS grp
    FROM Stadium
    WHERE people >= 100
),
counted_groups AS (
    SELECT id, visit_date, people,
           COUNT(*) OVER (PARTITION BY grp) AS cnt
    FROM group_consecutive
)
SELECT id, visit_date, people
FROM counted_groups
WHERE cnt >= 3
ORDER BY id;
```

#### **Variant 2: Self-Join (Traditional)**
```sql
SELECT DISTINCT s1.id, s1.visit_date, s1.people
FROM Stadium s1, Stadium s2, Stadium s3
WHERE s1.people >= 100 AND s2.people >= 100 AND s3.people >= 100
  AND (
      (s1.id = s2.id - 1 AND s1.id = s3.id - 2) OR -- s1, s2, s3
      (s1.id = s2.id + 1 AND s1.id = s3.id - 1) OR -- s2, s1, s3
      (s1.id = s2.id + 1 AND s1.id = s3.id + 2)    -- s3, s2, s1
  )
ORDER BY s1.id;
```

* **Key Difference & Performance:** Variant 1 (Gaps and Islands) is highly performant and scales to any length of consecutive records (e.g. >= 10 days) without adding more self-joins.

---

### 21. [LeetCode 607: Sales Person](https://leetcode.com/problems/sales-person/)

**Problem:** Find names of salespeople who did not have orders related to the company "RED".

#### **Variant 1: NOT IN (Subquery)**
```sql
SELECT name
FROM SalesPerson
WHERE sales_id NOT IN (
    SELECT o.sales_id
    FROM Orders o
    JOIN Company c ON o.com_id = c.com_id
    WHERE c.name = 'RED'
);
```

#### **Variant 2: LEFT JOIN (Anti-Pattern Join)**
```sql
SELECT s.name
FROM SalesPerson s
LEFT JOIN Orders o ON s.sales_id = o.sales_id
LEFT JOIN Company c ON o.com_id = c.com_id AND c.name = 'RED'
WHERE c.com_id IS NULL;
```

* **Trap:** Joining directly and using `WHERE c.name <> 'RED'` is a classic mistake. That filters out salespeople who sold to other companies but also sold to "RED". You must identify anyone who sold to "RED" first, then exclude them.

---

### 22. [LeetCode 608: Tree Node](https://leetcode.com/problems/tree-node/)

**Problem:** Classify each node as Root, Inner, or Leaf.

#### **Variant 1: CASE WHEN (Recommended)**
```sql
SELECT id,
       CASE 
           WHEN p_id IS NULL THEN 'Root'
           WHEN id IN (SELECT DISTINCT p_id FROM Tree WHERE p_id IS NOT NULL) THEN 'Inner'
           ELSE 'Leaf'
       END AS type
FROM Tree;
```

* **Trap:** When checking `id NOT IN (SELECT p_id FROM Tree)`, if the subquery returns any `NULL` value, the comparison will result in `UNKNOWN` and return no leaf nodes. Always filter `p_id IS NOT NULL` inside the subquery.

---

### 23. [LeetCode 620: Not Boring Movies](https://leetcode.com/problems/not-boring-movies/)

**Problem:** Find movies with an odd-numbered ID and a description that is not 'boring'.

#### **Variant 1: MOD operator**
```sql
SELECT *
FROM Cinema
WHERE id % 2 = 1 AND description <> 'boring'
ORDER BY rating DESC;
```

#### **Variant 2: Bitwise AND**
```sql
SELECT *
FROM Cinema
WHERE (id & 1) = 1 AND description <> 'boring'
ORDER BY rating DESC;
```

---

### 24. [LeetCode 1075: Project Employees I](https://leetcode.com/problems/project-employees-i/)

**Problem:** Find the average experience years of employees for each project (rounded to 2 decimal places).

#### **Variant 1: JOIN + ROUND + AVG**
```sql
SELECT p.project_id, ROUND(AVG(e.experience_years), 2) AS average_years
FROM Project p
JOIN Employee e ON p.employee_id = e.employee_id
GROUP BY p.project_id;
```

---

### 25. [LeetCode 1084: Sales Analysis III](https://leetcode.com/problems/sales-analysis-iii/)

**Problem:** Find products that were only sold in spring 2019 (between 2019-01-01 and 2019-03-31).

#### **Variant 1: HAVING bounds (Recommended)**
```sql
SELECT p.product_id, p.product_name
FROM Product p
JOIN Sales s ON p.product_id = s.product_id
GROUP BY p.product_id, p.product_name
HAVING MIN(s.sale_date) >= '2019-01-01' 
   AND MAX(s.sale_date) <= '2019-03-31';
```

#### **Variant 2: NOT EXISTS**
```sql
SELECT product_id, product_name
FROM Product
WHERE product_id IN (SELECT product_id FROM Sales)
  AND product_id NOT IN (
      SELECT product_id 
      FROM Sales 
      WHERE sale_date < '2019-01-01' OR sale_date > '2019-03-31'
  );
```

---

### 26. [LeetCode 1141: User Activity for the Past 30 Days I](https://leetcode.com/problems/user-activity-for-the-past-30-days-i/)

**Problem:** Find active user count for each day in a 30-day period ending on 2019-07-27.

#### **Variant 1: Date Boundaries**
```sql
SELECT activity_date AS day, COUNT(DISTINCT user_id) AS active_users
FROM Activity
WHERE activity_date BETWEEN '2019-06-28' AND '2019-07-27'
GROUP BY activity_date;
```

---

### 27. [LeetCode 1148: Article Views I](https://leetcode.com/problems/article-views-i/)

**Problem:** Find all authors that viewed at least one of their own articles.

#### **Variant 1: DISTINCT (Recommended)**
```sql
SELECT DISTINCT author_id AS id
FROM Views
WHERE author_id = viewer_id
ORDER BY id ASC;
```

---

### 28. [LeetCode 1158: Market Analysis I](https://leetcode.com/problems/market-analysis-i/)

**Problem:** Find for each user, their join date and the number of orders they made as a buyer in 2019.

#### **Variant 1: LEFT JOIN with conditional ON (Recommended)**
```sql
SELECT u.user_id AS buyer_id, 
       u.join_date, 
       COUNT(o.order_id) AS orders_in_2019
FROM Users u
LEFT JOIN Orders o 
    ON u.user_id = o.buyer_id 
   AND YEAR(o.order_date) = 2019
GROUP BY u.user_id, u.join_date;
```

* **Trap:** Placing `YEAR(order_date) = 2019` in the `WHERE` clause instead of the `ON` clause turns the `LEFT JOIN` into an `INNER JOIN`, filtering out users who made no orders in 2019. Keep it in the `ON` clause to preserve all users.

---

### 29. [LeetCode 1204: Last Person to Fit in the Bus](https://leetcode.com/problems/last-person-to-fit-in-the-bus/)

**Problem:** Find the last person who can board the bus without exceeding the weight limit of 1000 kgs.

#### **Variant 1: Window Function SUM()**
```sql
SELECT person_name
FROM (
    SELECT person_name, 
           SUM(weight) OVER (ORDER BY turn) AS running_weight
    FROM Queue
) t
WHERE running_weight <= 1000
ORDER BY running_weight DESC
LIMIT 1;
```

#### **Variant 2: Non-Equi Self-Join**
```sql
SELECT q1.person_name
FROM Queue q1
JOIN Queue q2 ON q1.turn >= q2.turn
GROUP BY q1.person_id, q1.person_name, q1.turn
HAVING SUM(q2.weight) <= 1000
ORDER BY q1.turn DESC
LIMIT 1;
```

---

### 30. [LeetCode 1321: Restaurant Growth](https://leetcode.com/problems/restaurant-growth/)

**Problem:** Compute the moving 7-day average and total amount paid by customers.

#### **Variant 1: Window Range with CTE**
```sql
WITH DailyAmounts AS (
    SELECT visited_on, SUM(amount) AS amount
    FROM Customer
    GROUP BY visited_on
),
MovingWindow AS (
    SELECT visited_on,
           SUM(amount) OVER (ORDER BY visited_on ROWS BETWEEN 6 PRECEDING AND CURRENT ROW) AS sum_amount,
           ROUND(AVG(amount) OVER (ORDER BY visited_on ROWS BETWEEN 6 PRECEDING AND CURRENT ROW), 2) AS avg_amount,
           ROW_NUMBER() OVER (ORDER BY visited_on) AS rn
    FROM DailyAmounts
)
SELECT visited_on, sum_amount AS amount, avg_amount AS average_amount
FROM MovingWindow
WHERE rn >= 7;
```

* **Key Detail:** The window function computes over `DailyAmounts`, so we group by date first to handle multiple customer payments on the same date. The `rn >= 7` constraint ensures we only output rows that have at least 6 days of preceding data.


