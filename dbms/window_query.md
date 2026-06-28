# SQL Window Functions Cheat Sheet (Interview Edition)

## 1. Generic Syntax

```sql
FUNCTION(expression) OVER (
    PARTITION BY column1, column2
    ORDER BY column3
    ROWS BETWEEN ...
)
```

Think of it as

```
Window Function
        ↓
FUNCTION(...) OVER(
    Partition
    Ordering
    Frame
)
```

Not every query needs all three parts.

---

# 2. OVER()

Without OVER(), aggregate functions return one row.

Example

```sql
SELECT SUM(amount)
FROM Sales;
```

Output

```
2700
```

With OVER()

```sql
SELECT
    amount,
    SUM(amount) OVER()
FROM Sales;
```

Output

```
500 2700
300 2700
700 2700
...
```

The aggregate is computed without collapsing rows.

---

# 3. PARTITION BY

Think of it as

```
GROUP BY
BUT
Don't collapse rows.
```

Example

```sql
SUM(amount) OVER(
    PARTITION BY customer_id
)
```

Each customer gets their own window.

```
Customer 101

500
700
200

Total = 1400

Customer 102

300
600

Total = 900
```

---

# 4. ORDER BY

Defines sequence inside each partition.

```sql
SUM(amount)
OVER(
    PARTITION BY customer_id
    ORDER BY sale_date
)
```

Without ORDER BY

```
1400
1400
1400
```

With ORDER BY

```
500
1200
1400
```

Now it becomes a running total.

---

# 5. Window Frame

Default

```
UNBOUNDED PRECEDING
TO
CURRENT ROW
```

Meaning

```
Beginning
↓

500
500+700
500+700+200
```

Custom frames

Current + Previous 2

```sql
ROWS BETWEEN 2 PRECEDING
AND CURRENT ROW
```

Entire partition

```sql
ROWS BETWEEN
UNBOUNDED PRECEDING
AND UNBOUNDED FOLLOWING
```

Current row only

```sql
ROWS BETWEEN
CURRENT ROW
AND CURRENT ROW
```

Next 2 rows

```sql
ROWS BETWEEN
CURRENT ROW
AND 2 FOLLOWING
```

---

# 6. Ranking Functions

## ROW_NUMBER()

Unique numbering.

```sql
ROW_NUMBER() OVER(
ORDER BY amount DESC
)
```

Output

```
700 → 1
600 → 2
500 → 3
500 → 4
```

Duplicates still get different numbers.

---

## RANK()

Duplicates share rank.

```sql
RANK() OVER(...)
```

```
700 → 1
600 → 2
500 → 3
500 → 3
400 → 5
```

Notice the gap.

---

## DENSE_RANK()

No gaps.

```
700 → 1
600 → 2
500 → 3
500 → 3
400 → 4
```

---

# 7. Aggregate Window Functions

Running Total

```sql
SUM(amount)
OVER(
ORDER BY sale_date
)
```

Moving Average

```sql
AVG(amount)
OVER(
ORDER BY sale_date
ROWS BETWEEN 2 PRECEDING
AND CURRENT ROW
)
```

Running Count

```sql
COUNT(*)
OVER(
PARTITION BY customer_id
ORDER BY sale_date
)
```

Maximum

```sql
MAX(amount)
OVER(
PARTITION BY category
)
```

Minimum

```sql
MIN(amount)
OVER(
PARTITION BY category
)
```

---

# 8. Navigation Functions

Previous row

```sql
LAG(amount)
OVER(
ORDER BY sale_date
)
```

Next row

```sql
LEAD(amount)
OVER(
ORDER BY sale_date
)
```

Difference from previous

```sql
amount -
LAG(amount)
OVER(...)
```

Growth %

```sql
(amount -
LAG(amount) OVER(...))
/
LAG(amount) OVER(...)
```

---

# 9. First / Last Value

First purchase

```sql
FIRST_VALUE(amount)
OVER(
PARTITION BY customer_id
ORDER BY sale_date
)
```

Last purchase

```sql
LAST_VALUE(amount)
OVER(
PARTITION BY customer_id
ORDER BY sale_date
ROWS BETWEEN
UNBOUNDED PRECEDING
AND UNBOUNDED FOLLOWING
)
```

Always remember the frame for LAST_VALUE.

---

# 10. Distribution Functions

Quartiles

```sql
NTILE(4)
OVER(
ORDER BY amount
)
```

Percent Rank

```sql
PERCENT_RANK()
OVER(
ORDER BY amount
)
```

Cumulative Distribution

```sql
CUME_DIST()
OVER(
ORDER BY amount
)
```

---

# 11. Top K Per Group

```sql
WITH ranked AS
(
SELECT *,
ROW_NUMBER() OVER(
PARTITION BY category
ORDER BY amount DESC
) rn
FROM Sales
)

SELECT *
FROM ranked
WHERE rn <= 3;
```

Most common interview problem.

---

# 12. Percentage Contribution

```sql
SELECT
customer_id,
amount,
amount*100.0/
SUM(amount)
OVER(PARTITION BY customer_id)
FROM Sales;
```

---

# 13. Running Difference

```sql
SELECT
sale_date,
amount,
amount -
LAG(amount)
OVER(ORDER BY sale_date)
AS diff
FROM Sales;
```

---

# 14. Common Interview Patterns

## Running Total

```sql
SUM(col)
OVER(ORDER BY date)
```

---

## Running Average

```sql
AVG(col)
OVER(
ORDER BY date
ROWS BETWEEN
2 PRECEDING
AND CURRENT ROW
)
```

---

## Previous Value

```sql
LAG(col)
OVER(...)
```

---

## Next Value

```sql
LEAD(col)
OVER(...)
```

---

## Rank

```sql
RANK()
OVER(ORDER BY score DESC)
```

---

## Dense Rank

```sql
DENSE_RANK()
OVER(ORDER BY score DESC)
```

---

## Row Number

```sql
ROW_NUMBER()
OVER(ORDER BY ...)
```

---

## Top K Per Group

```sql
ROW_NUMBER()
OVER(
PARTITION BY group
ORDER BY score DESC
)
```

---

## First Value

```sql
FIRST_VALUE(col)
OVER(...)
```

---

## Last Value

```sql
LAST_VALUE(col)
OVER(...)
```

Remember the frame.

---

## Percentage

```sql
col*100.0/
SUM(col)
OVER(PARTITION BY ...)
```

---

# 15. Interview Decision Tree

Question asks...

• Running total
→ SUM() OVER(ORDER BY ...)

• Running average
→ AVG() OVER(...)

• Previous row
→ LAG()

• Next row
→ LEAD()

• Ranking
→ RANK()

• Ranking without gaps
→ DENSE_RANK()

• Unique numbering
→ ROW_NUMBER()

• First record
→ FIRST_VALUE()

• Last record
→ LAST_VALUE()

• Top K per group
→ ROW_NUMBER() + CTE

• Percent contribution
→ value / SUM() OVER(PARTITION BY ...)

• Quartiles
→ NTILE()

• Relative rank
→ PERCENT_RANK()

• Cumulative percentage
→ CUME_DIST()

---

# 16. One-Line Memory Trick

```
OVER()
    ↓
PARTITION BY  → divide data
ORDER BY      → define order
ROWS          → define frame

Function
    ↓
Ranking
Aggregate
Navigation
Distribution
```

If you can identify:
1. **What is the window?**
2. **How should rows be ordered?**
3. **Which window function fits the requirement?**

...you can write almost any interview window function query.