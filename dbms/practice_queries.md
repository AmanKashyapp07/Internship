# SQL Interview Practice — 100 Questions
> **Database**: `dbms-intern-prep` | **Target**: Amazon, Microsoft, Google, Flipkart, Zepto, etc.

Open `solution.md` for the answers **only after** you have tried each query.

---

## Schema Quick Reference

| Table | Key Columns |
|---|---|
| `Employee` | `id`, `name`, `department`, `salary`, `manager_id`, `hire_date`, `is_active` |
| `Department` | `dept_id`, `name`, `budget`, `location` |
| `Project` | `project_id`, `name`, `budget`, `start_date`, `end_date`, `dept_id` |
| `EmployeeProject` | `employee_id`, `project_id`, `role`, `joined_date` |
| `Customer` | `customer_id`, `name`, `city`, `tier`, `registration_date` |
| `Product` | `product_id`, `name`, `category`, `price`, `stock_qty` |
| `Orders` | `order_id`, `customer_id`, `order_date`, `status` |
| `OrderItem` | `order_item_id`, `order_id`, `product_id`, `quantity`, `unit_price` |
| `Sales` | `sale_id`, `customer_id`, `sale_date`, `amount` |
| `Logins` | `login_id`, `user_id`, `login_date` |
| `StockPrice` | `trade_date`, `price` |

---

## Section I — Basic SELECT & Filtering (Q1–Q10)

1. Retrieve the full name and salary of every active employee.
2. List all unique cities where customers live.
3. Find all products in the `'Electronics'` category with a price greater than 5,000.
4. Count the total number of employees in each department.
5. Find the minimum, maximum, and average salary across all employees.
6. List all customers from `'Delhi'` or `'Mumbai'`.
7. Find all orders that have a status of `'Cancelled'`.
8. List all products whose stock quantity is below 60.
9. Find all employees hired after `'2022-01-01'`.
10. Find the total revenue (quantity × unit_price) across all order items.

---

## Section II — Aggregation & Grouping (Q11–Q20)

11. Find the total budget allocated to each department.
12. List each product category with the average price of products in that category.
13. Find the total number of orders placed by each customer.
14. Find the customer(s) who placed the most orders.
15. List every department along with the number of employees in it, sorted by employee count descending.
16. Find the total sales amount per customer from the `Sales` table.
17. Find the average order value (total OrderItem revenue per order) across all orders.
18. Find the month and year with the highest number of orders placed.
19. List all categories and the total quantity sold across all orders.
20. Find the product that has generated the most revenue (quantity × unit_price summed across all order items).

---

## Section III — Joins (Q21–Q35)

21. List each employee's name alongside their direct manager's name.
22. List all employees, including those without a manager, showing `'CEO'` for employees with no manager.
23. List every employee along with the project name(s) they are assigned to.
24. Find all employees who are NOT assigned to any project.
25. List each project with the number of employees assigned to it.
26. List customer names together with all their order IDs and order dates.
27. Find customers who have never placed an order.
28. For each order, show the order ID, customer name, and the total number of distinct products in that order.
29. Find all products that have never been ordered.
30. List every order item showing the product name, quantity, unit price, and total line value (quantity × unit_price).
31. Find the department name for each employee (join `Employee` to `Department` on the `name` column).
32. List all projects alongside the department name that owns each project.
33. Find all employees who have the same salary as at least one other employee in a different department.
34. For each customer tier (`Gold`, `Silver`, `Bronze`), count the number of customers and the total number of orders they placed.
35. List orders placed in `'2026-01'` along with the customer name and city.

---

## Section IV — Subqueries & CTEs (Q36–Q50)

36. Find all employees earning above the company-wide average salary.
37. Find employees earning more than the average salary of their own department.
38. Find the department that has the highest average salary.
39. Find the product with the highest unit price in each category.
40. Find customers who have placed at least one `'Delivered'` order.
41. Find orders where the total line value (sum of quantity × unit_price) exceeds 50,000.
42. Using a CTE, find the top 3 most expensive products per category.
43. Find all employees who are also a manager of at least one other employee.
44. Find customers who have ordered every product in the `'Furniture'` category.
45. Using a CTE chain, compute each employee's salary as a percentage of their department's total salary budget.
46. Find the second-highest product price without using `LIMIT` or window functions.
47. Find all orders placed by `'Gold'` tier customers only.
48. Using a recursive CTE, list the full management chain from each root manager down to every leaf employee.
49. Find projects whose budget exceeds the average budget of all projects in the same department.
50. Using a CTE, find employees who were hired in the same month and year as at least one other employee.

---

## Section V — Window Functions (Q51–Q65)

51. Rank every employee by salary within their department using `DENSE_RANK()`.
52. Find the employee with the 2nd highest salary in each department.
53. Assign a `ROW_NUMBER()` to each order per customer, ordered by order date.
54. Calculate the running total of sales for each customer over time.
55. Calculate the cumulative count of orders placed per day.
56. Using `LAG()`, compute the day-over-day stock price change.
57. Using `LEAD()`, for each stock trading day, show what the price will be the next trading day.
58. Calculate the 3-day moving average of stock prices.
59. Calculate a 3-day moving average of sales amounts across all customers.
60. For each order item, show the revenue as a percentage of total revenue for that order.
61. Find the first and last order date for each customer using `FIRST_VALUE` and `LAST_VALUE`.
62. Using `NTILE(4)`, group employees into four salary quartiles.
63. For each sale, show the difference from the previous sale for the same customer using `LAG()`.
64. Find the highest single-day stock price gain and the date it occurred.
65. Using `ROW_NUMBER()`, de-duplicate the `Logins` table — keep only the first login record per user per day.

---

## Section VI — Advanced Scenarios & Real Interview Problems (Q66–Q85)

66. Find users who logged in on 3 or more consecutive days (Gaps-and-Islands problem).
67. For each employee, calculate the number of years since they were hired (tenure in years).
68. Label each employee's salary tier: `'Senior'` (≥ 90k), `'Mid'` (60k–89k), `'Junior'` (< 60k).
69. Pivot the total sales amount for customers 1, 2, and 3 into three separate columns.
70. Find the employee(s) with the highest salary in the company using only a subquery (no `LIMIT`, no window function).
71. Find all pairs of employees who share the same department and the same manager.
72. For each department, find the employee who was hired most recently.
73. Find customers whose total order revenue exceeds the average total order revenue of all customers.
74. Find all orders where every item in the order belongs to the `'Electronics'` category.
75. Compute the month-over-month revenue growth from the `Sales` table.
76. Find the top 2 best-selling products by total quantity sold.
77. Find employees who work on more than one project.
78. For each project, find the employee who joined the project most recently.
79. Find all customers who placed orders in two or more different months.
80. Find the stock price percentage change from the very first recorded day to the last recorded day.
81. Find all products that were ordered by more than one customer.
82. Find the average number of days between consecutive orders per customer.
83. List all employees along with the total budget of every project they are assigned to.
84. Find the department(s) where the maximum salary equals the minimum salary (all employees have the same pay).
85. Find orders that contain more than 2 distinct products.

---

## Section VII — NULL Handling, String Functions & Date Arithmetic (Q86–Q100)

86. List all employees who have no manager, replacing NULL with the string `'No Manager'`.
87. Find all employees whose name contains the substring `'son'` (case-insensitive).
88. Display every employee's name in `UPPER` case and their department in `LOWER` case.
89. Concatenate each employee's name and department as `"Name — Department"`.
90. Find the length (character count) of each product name and order by it descending.
91. Find all orders placed in the last 60 days from the most recent order date in the table.
92. For each customer, compute the number of days between their registration and their first order.
93. Extract the day of the week for every login date (e.g., `Monday`, `Tuesday`).
94. Find all stock prices recorded on a `Monday`.
95. Calculate how many full months each employee has been at the company.
96. Find all customers who registered in `2022`.
97. Find products whose name starts with `'W'` or ends with `'e'`.
98. For each order, add a column showing `'Recent'` if placed in the last 90 days from the latest order date, otherwise `'Old'`.
99. Find all `NULL` values across `manager_id` in `Employee` and `end_date` in `Project` — count how many are NULL in each.
100. For each department, list the employee names as a single comma-separated string.
