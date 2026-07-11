# GS OA Mathematics Revision — Quick Reference

Only what is directly tested in Goldman Sachs OA MCQs. No fluff.

---

## 1. Core Probability Rules

*   **Complement:** `P(not A) = 1 - P(A)`
*   **Addition Rule (any events):** `P(A or B) = P(A) + P(B) - P(A and B)`
*   **Mutually Exclusive (cannot both happen):** `P(A and B) = 0`, so `P(A or B) = P(A) + P(B)`
*   **Independent events:** `P(A and B) = P(A) * P(B)`
*   **Conditional Probability:** `P(A | B) = P(A and B) / P(B)`
*   **Multiplication Rule:** `P(A and B) = P(A | B) * P(B)`

---

## 2. Bayes' Theorem

Used when you know `P(B | A)` but need `P(A | B)` — the reversal of a conditional probability.

```
P(A | B) = [P(B | A) * P(A)] / P(B)

where P(B) = P(B | A) * P(A) + P(B | not A) * P(not A)   ← Law of Total Probability
```

#### Worked Example (Most Common GS Template)
*   A disease affects 1 in 1000 people. A test is 99% accurate (1% false positive rate). If a person tests positive, what is the probability they actually have the disease?
    *   `P(Pos) = (0.99 * 0.001) + (0.01 * 0.999) = 0.01098`
    *   `P(Disease | Pos) = (0.99 * 0.001) / 0.01098 ≈ 9%`
    *   **Key insight:** Even with 99% accuracy, a rare disease still means most positives are false positives.

---

## 3. Expected Value

*   **Definition:** `E[X] = sum of [x * P(X = x)]` for all outcomes.
*   **Linearity (GS Favorite):** `E[X + Y] = E[X] + E[Y]` — true even if X and Y are dependent.
    *   *Classic use:* Expected sum of 10 dice = `10 * E[1 die] = 10 * 3.5 = 35`.
*   **Expected tosses to get first success** (geometric): `E = 1/p`. For a fair coin, `E = 1/0.5 = 2`.

#### Classic GS Puzzle: Optimal Die Roll
*   You roll a fair die. You may keep the result or pay $1 to re-roll (one re-roll allowed). What is the optimal strategy?
    *   **Expected value of re-rolling** = `3.5 (average) - 1.0 (cost) = 2.5`
    *   **Strategy:** Keep your roll if it is ≥ 3 (i.e., > 2.5). Re-roll if you get 1 or 2.
    *   **Expected payoff** = `(4/6) * (3+4+5+6)/4 + (2/6) * 3.5 = (4/6) * 4.5 + (2/6) * 3.5 = 3.0 + 1.17 = 4.17`

---

## 4. Combinatorics

*   **Permutations (order matters):** `P(n, k) = n! / (n - k)!`
*   **Combinations (order does not matter):** `C(n, k) = n! / (k! * (n - k)!)`

#### At Least One Trick
*   **"At least one" = 1 - P(none)**
    *   *Example:* Probability of getting at least one Head in 3 flips = `1 - (0.5)^3 = 7/8`.

#### Inclusion-Exclusion Principle
Used to count elements belonging to at least one of several sets without double-counting.
*   **Two sets:** `|A or B| = |A| + |B| - |A and B|`
*   **Three sets:** `|A or B or C| = |A| + |B| + |C| - |A and B| - |B and C| - |A and C| + |A and B and C|`
*   *Example (GS style):* How many integers from 1 to 100 are divisible by 2 or 3?
    *   `|div by 2| = 50`, `|div by 3| = 33`, `|div by 6| = 16`
    *   Answer = `50 + 33 - 16 = 67`

#### Pigeonhole Principle
If `n` items are placed into `k` containers and `n > k`, then at least one container must hold more than one item.
*   *Example (GS style):* In a group of 13 people, at least 2 share the same birth month (since only 12 months exist).
*   **Generalized:** If `n` items go into `k` containers, at least one container has at least `ceil(n/k)` items.

#### Birthday Problem (Classic MCQ)
*   How many people are needed for the probability of a shared birthday to exceed 50%?
    *   **Answer: 23 people.** (GS tests this to check probabilistic intuition.)

---

## 5. Distributions (Quick Reference)

### Binomial
*   **When:** Exactly `k` successes in `n` independent trials with probability `p` each.
*   **Formula:** `P(X = k) = C(n, k) * p^k * (1-p)^(n-k)`
*   **Mean:** `n * p` | **Variance:** `n * p * (1 - p)`

### Geometric
*   **When:** First success occurs on the `k`-th trial.
*   **Formula:** `P(X = k) = (1-p)^(k-1) * p`
*   **Mean:** `1 / p`

---

## 6. Linear Algebra (GS MCQ Topics)

### Matrix Operations
*   **Matrix Multiplication:** `(A * B)[i][j] = sum of A[i][k] * B[k][j]` for all k.
    *   Only defined when columns of A = rows of B.
    *   Result: if A is `(m x n)` and B is `(n x p)`, result is `(m x p)`.
*   **Matrix Multiplication is NOT commutative:** `A * B ≠ B * A` in general.
*   **Identity Matrix (I):** `A * I = I * A = A`.

### Determinant
*   **2x2:** `det([[a, b], [c, d]]) = ad - bc`
*   **Singular Matrix:** A matrix whose determinant is 0. It has no inverse.
*   **Invertible Matrix:** `det(A) ≠ 0`. Its inverse satisfies `A * A_inv = I`.

### Rank
*   The rank of a matrix = the number of linearly independent rows (or columns).
*   For an `(m x n)` matrix: `rank(A) <= min(m, n)`.
*   **Full rank:** All rows/columns are independent. The system has a unique solution.

### Eigenvalues
*   `Av = Lv` where `v` is the eigenvector and `L` (lambda) is the eigenvalue.
*   **Key property:** The sum of all eigenvalues = trace of the matrix (sum of diagonal elements).
*   **Key property:** The product of all eigenvalues = determinant of the matrix.
*   A matrix with a zero eigenvalue is singular (non-invertible).

---

## 7. Key Traps GS Tests

| Trap | What they test | Remember |
| :--- | :--- | :--- |
| P(A or B) with overlap | Adding without subtracting intersection | Subtract `P(A and B)` |
| Rare events + accurate tests | Counter-intuitive Bayes result | Most positives are false positives |
| Birthday problem | Probabilistic intuition | Answer is 23, not 183 |
| Independent vs. mutually exclusive | Common confusion | Independent: `P(A and B) = P(A)*P(B)`. Mutually exclusive: `P(A and B) = 0`. NOT the same. |
| E[X + Y] with dependency | Whether linearity holds | E is always additive regardless of independence |
| Matrix multiplication order | Commutativity | `A*B ≠ B*A` in general |
| Zero eigenvalue | What it implies | Matrix is singular (non-invertible) |
