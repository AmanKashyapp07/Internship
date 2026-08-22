# Section 01: Complexity & Foundations

> **Focus:** Theoretical foundations of algorithmic complexity, asymptotic analysis, recurrence relations, Master Theorem, and amortized efficiency.

---

# Table of Contents
1. [Asymptotic Notations (Big-O, Omega, Theta)](#1-asymptotic-notations)
2. [Best, Average, and Worst Case Analysis](#2-best-average-and-worst-case-analysis)
3. [Recursion & Recurrence Relations](#3-recursion--recurrence-relations)
4. [The Master Theorem](#4-the-master-theorem)
5. [Amortized Analysis](#5-amortized-analysis)
6. [Quick Recap](#6-quick-recap)

---

# 1. Asymptotic Notations

Asymptotic notation is a mathematical framework used to describe the **limiting behavior** of an algorithm's execution time or memory consumption as the input size $n$ approaches infinity ($n \to \infty$). It allows us to compare algorithms independently of hardware specs, CPU clock cycles, compiler optimizations, or programming languages.

```
Execution Time / Operations
    ^
    |                                   f(n) <= c * g(n)  [Big-O: Upper Bound]
    |                          /
    |                       .-'  c * g(n)
    |                     .'
    |                   .'   f(n) (Actual Runtime)
    |                 .'
    |               .'       c' * g(n)  [Big-Omega: Lower Bound]
    |             .'
    |           .'
    +----------+---------------------------------------> Input Size (n)
               n_0 (Threshold)
```

---

### A. Big-O Notation ($O$) — Asymptotic Upper Bound
- **Definition:** $f(n) = O(g(n))$ if there exist positive constants $c > 0$ and $n_0 \ge 1$ such that:
  $$0 \le f(n) \le c \cdot g(n) \quad \text{for all } n \ge n_0$$
- **Simple Meaning:** $g(n)$ provides an **upper ceiling** on the growth rate of $f(n)$. The algorithm will grow *at most* as fast as $g(n)$ for sufficiently large inputs.
- **Example:** If an algorithm takes $f(n) = 3n^2 + 5n + 100$ operations, we say $f(n) = O(n^2)$ because for $c = 4$ and $n_0 = 10$, $3n^2 + 5n + 100 \le 4n^2$.

### B. Big-Omega Notation ($\Omega$) — Asymptotic Lower Bound
- **Definition:** $f(n) = \Omega(g(n))$ if there exist positive constants $c > 0$ and $n_0 \ge 1$ such that:
  $$0 \le c \cdot g(n) \le f(n) \quad \text{for all } n \ge n_0$$
- **Simple Meaning:** $g(n)$ provides a **lower floor**. The algorithm will take *at least* $c \cdot g(n)$ operations in the long run.
- **Example:** Comparison-based sorting algorithms require $\Omega(n \log n)$ comparisons in the worst case (proven via decision trees).

### C. Big-Theta Notation ($\Theta$) — Asymptotic Tight Bound
- **Definition:** $f(n) = \Theta(g(n))$ if and only if $f(n) = O(g(n))$ and $f(n) = \Omega(g(n))$. Formally, there exist constants $c_1, c_2 > 0$ and $n_0 \ge 1$ such that:
  $$c_1 \cdot g(n) \le f(n) \le c_2 \cdot g(n) \quad \text{for all } n \ge n_0$$
- **Simple Meaning:** $g(n)$ is a **tight bound** that sandwiches $f(n)$ from both above and below.
- **Example:** Merge Sort is $\Theta(n \log n)$ because its runtime grows proportional to $n \log n$ across best, average, and worst-case inputs.

---

### D. Little-o ($o$) and Little-omega ($\omega$) Notations
- **Little-o ($o$):** Strict upper bound. $f(n) = o(g(n))$ means $f(n)$ grows **strictly slower** than $g(n)$:
  $$\lim_{n \to \infty} \frac{f(n)}{g(n)} = 0$$
  *Example:* $2n = o(n^2)$, but $2n^2 \ne o(n^2)$.
- **Little-omega ($\omega$):** Strict lower bound. $f(n) = \omega(g(n))$ means $f(n)$ grows **strictly faster** than $g(n)$:
  $$\lim_{n \to \infty} \frac{f(n)}{g(n)} = \infty$$

---

### Common Time Complexity Hierarchy (Fastest to Slowest)
$$O(1) < O(\log \log n) < O(\log n) < O(\sqrt{n}) < O(n) < O(n \log n) < O(n^2) < O(n^3) < O(2^n) < O(n!)$$

```cpp
// Complexity Hierarchy in Practice:
// O(1): Hash table lookup (average), Array indexing
int val = arr[5];

// O(log n): Binary Search
int l = 0, r = n - 1;
while (l <= r) {
    int mid = l + (r - l) / 2;
    if (arr[mid] == target) return mid;
    else if (arr[mid] < target) l = mid + 1;
    else r = mid - 1;
}

// O(n): Linear Scan
for (int i = 0; i < n; i++) { sum += arr[i]; }

// O(n log n): Merge Sort / Heap Sort
// O(n^2): Nested loops (Selection Sort / Bubble Sort)
for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) { matrix[i][j] = 0; }
}
```

---

### Interview Follow-Up Questions
1. **"Is $O(n)$ equal to $\Theta(n)$?"**
   - *Answer:* No. $O(n)$ is merely an upper bound (an algorithm that is $O(1)$ is also technically $O(n)$). $\Theta(n)$ means the algorithm is both upper-bounded and lower-bounded by $c \cdot n$ (tight bound).
2. **"Why do we drop low-order terms and constant coefficients in asymptotic analysis?"**
   - *Answer:* Because as $n \to \infty$, the highest-order term dominates the growth rate. For $f(n) = 100n^2 + 10^6n$, when $n = 10^7$, $100n^2 = 10^{16}$ while $10^6n = 10^{13}$ (a 1,000x difference).
3. **"Does $O(1)$ mean a function runs in exactly 1 CPU cycle?"**
   - *Answer:* No. $O(1)$ means the execution time is constant and independent of the input size $n$. It could take 5 nanoseconds or 5 minutes, as long as it does not grow when $n$ scales.

---

# 2. Best, Average, and Worst Case Analysis

An algorithm's runtime can vary significantly depending on the **specific arrangement of input data**, not just the input size $n$.

```
+-----------------------------------------------------------------------------------+
| CASE TYPE       | DEFINITION                             | EXAMPLE: Insertion Sort|
+-----------------------------------------------------------------------------------+
| Best Case       | Minimum operations over all inputs     | Already sorted: O(n)   |
|                 | of size n.                             | (1 comparison per item)|
+-----------------------------------------------------------------------------------+
| Average Case    | Expected operations over a uniform     | Random permutations:   |
|                 | probability distribution of inputs.    | O(n^2)                 |
+-----------------------------------------------------------------------------------+
| Worst Case      | Maximum operations over all possible   | Reverse sorted: O(n^2) |
|                 | inputs of size n (Guaranteed ceiling). | (Max shifts needed)    |
+-----------------------------------------------------------------------------------+
```

---

### Deep Dive: QuickSort Case Analysis
- **Best Case — $\Theta(n \log n)$:** The chosen pivot always partitions the array into two equal halves ($n/2$ and $n/2$). The recursion tree depth is $\log_2 n$, doing $O(n)$ work per level:
  $$T(n) = 2T(n/2) + O(n) \implies \Theta(n \log n)$$
- **Average Case — $\Theta(n \log n)$:** Even an unbalanced split like $90\% / 10\%$ yields a recursion tree depth of $\log_{10/9} n = O(\log n)$, keeping total average runtime $\Theta(n \log n)$.
- **Worst Case — $\Theta(n^2)$:** The chosen pivot is always the absolute minimum or maximum element (e.g. array is already sorted and pivot is the last element). The partitions are of size $0$ and $n-1$:
  $$T(n) = T(n-1) + O(n) \implies \Theta(n^2)$$

```cpp
// Insertion Sort demonstrating Best vs Worst case:
void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        // Best case (sorted): inner loop condition fails immediately (O(1) per element -> O(n) total)
        // Worst case (reverse): inner loop runs j times (O(n) per element -> O(n^2) total)
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}
```

---

### Interview Follow-Up Questions
1. **"Is Worst-Case complexity the same as Big-O notation?"**
   - *Answer:* No! This is the single most common interview confusion. Big-O is a mathematical upper bound that can be applied to *any* case. For example, QuickSort has a *Best-Case Time Complexity* of $O(n \log n)$ and a *Worst-Case Time Complexity* of $O(n^2)$.
2. **"How do you eliminate the $O(n^2)$ worst case in QuickSort in production?"**
   - *Answer:*
     1. **Randomized Pivot Selection:** Pick a random index as pivot ($O(n^2)$ probability becomes mathematically negligible: $1/n!$).
     2. **Median-of-Three Partitioning:** Choose the median of `arr[low]`, `arr[mid]`, and `arr[high]`.
     3. **Introsort:** Start with QuickSort, switch to HeapSort if recursion depth exceeds $2 \log n$ (guaranteeing worst-case $O(n \log n)$ like `std::sort` in C++).

---

# 3. Recursion & Recurrence Relations

A recurrence relation is an equation or inequality that defines a function in terms of its value on smaller inputs.

```
Recursion Tree for T(n) = 2T(n/2) + c*n (Merge Sort):

Level 0:                      c*n                        = c*n
                            /     \
Level 1:               c*(n/2)    c*(n/2)                = c*n
                       /    \     /    \
Level 2:            c*(n/4) ... c*(n/4) ...              = c*n
                      |                   |
Level log2(n):       T(1)   T(1)   ...   T(1)            = c*n
                     [ 2^(log2 n) = n leaves ]

Total Cost = (Cost per Level) * (Number of Levels) = (c*n) * (log2 n + 1) = O(n log n)
```

---

### Methods for Solving Recurrences

#### Method 1: Recursion Tree Method
1. Draw the tree where each node represents the non-recursive cost $f(n)$ at that step.
2. Calculate the work done at each individual depth level.
3. Determine the total depth of the tree (until reaching base case $T(1)$).
4. Sum the work done across all levels:
   $$\text{Total Work} = \sum_{\text{level}=0}^{\text{depth}} (\text{Work at level})$$

#### Method 2: Substitution Method (Mathematical Induction)
1. **Guess** the form of the solution (e.g. guess $T(n) \le c n \log n$).
2. **Induction Step:** Substitute the inductive hypothesis into the recurrence and prove it holds for $n$.
3. **Base Case:** Prove the bound holds for small initial constants $n_0$.

*Example:* Solve $T(n) = 2T(n/2) + n$.
- Guess: $T(n) \le c n \log_2 n$.
- Assume true for $n/2$: $T(n/2) \le c (n/2) \log_2 (n/2)$.
- Substitute:
  $$T(n) = 2 \left[ c \frac{n}{2} \log_2 \left(\frac{n}{2}\right) \right] + n = c n (\log_2 n - 1) + n = c n \log_2 n - c n + n$$
- For this to be $\le c n \log_2 n$, we need $-c n + n \le 0 \implies c \ge 1$.
- Thus, $T(n) = O(n \log n)$.

---

### Space Complexity of Recursion: The Call Stack
Every recursive function call pushes a stack frame containing local variables and return addresses to the **OS Call Stack**.
- **Auxiliary Space of Recursion:** $\text{Max Depth of Recursion Tree} \times \text{Memory per Stack Frame}$.
- **Tail Call Optimization (TCO):** If the recursive call is the final statement in the function, modern compilers (C++, Scheme) optimize it into an iterative loop, reducing stack space from $O(n)$ to $O(1)$.

```cpp
// Non-Tail-Recursive (Call Stack Depth = O(n))
int fact(int n) {
    if (n <= 1) return 1;
    return n * fact(n - 1); // Must wait for result before multiplying
}

// Tail-Recursive (Eligible for TCO -> O(1) Stack Space)
int factTail(int n, int accumulator = 1) {
    if (n <= 1) return accumulator;
    return factTail(n - 1, n * accumulator); // Nothing left to compute after call
}
```

---

### Interview Follow-Up Questions
1. **"What causes a Stack Overflow error?"**
   - *Answer:* When the recursion depth exceeds the OS-allocated stack limit (typically 1MB–8MB). For instance, calling a non-tail recursive function with $n = 10^6$ requires $10^6$ stack frames, exhausting stack memory.
2. **"How do you convert any recursive algorithm to an iterative one?"**
   - *Answer:* Use an explicit heap-allocated `std::stack` to simulate the system call stack, which bypasses the limited OS stack frame size limit.

---

# 4. The Master Theorem

The Master Theorem provides a cookbook method for solving divide-and-conquer recurrences of the standard form:

$$T(n) = a \cdot T\left(\frac{n}{b}\right) + f(n)$$

Where:
- $a \ge 1$: Number of subproblems created at each step.
- $b > 1$: Factor by which the input size is divided.
- $f(n)$: Cost of dividing the problem and combining the subproblem results outside the recursive calls ($O(n^d)$).
- **Critical Exponent:** $c_{\text{crit}} = \log_b a$. It represents the work done at the leaf nodes of the recursion tree.

---

### The 3 Core Cases of Master Theorem

Compare the non-recursive work $f(n) = \Theta(n^k)$ with the leaf node work $n^{\log_b a}$:

```
+-----------------------------------------------------------------------------------------+
| CASE 1: Leaves Dominate (a > b^k)                                                       |
| If f(n) = O(n^(log_b a - eps)) for some eps > 0                                         |
| -> T(n) = Theta(n^(log_b a))                                                            |
| Example: T(n) = 8T(n/2) + 1000n^2                                                       |
|   a = 8, b = 2 -> log_2 8 = 3. Since f(n) = n^2 < n^3 -> T(n) = Theta(n^3)             |
+-----------------------------------------------------------------------------------------+
| CASE 2: Work is Balanced Across All Levels (a = b^k)                                    |
| If f(n) = Theta(n^(log_b a) * log^p n) where p >= 0                                     |
| -> T(n) = Theta(n^(log_b a) * log^(p+1) n)                                              |
| Example: Merge Sort T(n) = 2T(n/2) + Theta(n)                                           |
|   a = 2, b = 2 -> log_2 2 = 1. f(n) = n^1 (p=0) -> T(n) = Theta(n log n)               |
+-----------------------------------------------------------------------------------------+
| CASE 3: Root / Division Step Dominates (a < b^k)                                        |
| If f(n) = Omega(n^(log_b a + eps)) AND satisfies Regularity Condition:                 |
|   a * f(n/b) <= c * f(n) for some constant c < 1                                        |
| -> T(n) = Theta(f(n))                                                                   |
| Example: T(n) = 2T(n/2) + n^2                                                           |
|   a = 2, b = 2 -> log_2 2 = 1. f(n) = n^2 > n^1 -> T(n) = Theta(n^2)                   |
+-----------------------------------------------------------------------------------------+
```

---

### When Does the Master Theorem FAIL?
The standard Master Theorem cannot be applied if:
1. $a < 1$ (e.g. $T(n) = 0.5 T(n/2) + n$ — subproblems cannot be fractional).
2. $b \le 1$ (e.g. $T(n) = T(n - 1) + n$ — subtraction, not division; solve via arithmetic progression $\Theta(n^2)$).
3. $f(n)$ is not polynomial (e.g. $T(n) = 2T(n/2) + n \log n$ — falls in the gap between Case 2 and Case 3. Solved using Extended Master Theorem: $\Theta(n \log^2 n)$).
4. Regularity condition in Case 3 fails.

---

### Master Theorem Cheat Sheet for Classic Algorithms

| Algorithm | Recurrence Relation | $a$ | $b$ | $\log_b a$ | $f(n)$ | Master Case | Final Time Complexity |
| :--- | :--- | :---: | :---: | :---: | :--- | :---: | :--- |
| **Binary Search** | $T(n) = T(n/2) + O(1)$ | 1 | 2 | 0 | $O(1) = n^0$ | Case 2 ($p=0$) | $\Theta(\log n)$ |
| **Merge Sort** | $T(n) = 2T(n/2) + O(n)$ | 2 | 2 | 1 | $O(n) = n^1$ | Case 2 ($p=0$) | $\Theta(n \log n)$ |
| **Binary Tree Traversal**| $T(n) = 2T(n/2) + O(1)$ | 2 | 2 | 1 | $O(1) = n^0$ | Case 1 | $\Theta(n)$ |
| **Karatsuba Multiplication**| $T(n) = 3T(n/2) + O(n)$ | 3 | 2 | 1.585 | $O(n) = n^1$ | Case 1 | $\Theta(n^{\log_2 3}) \approx \Theta(n^{1.585})$ |
| **Strassen's Matrix Mult** | $T(n) = 7T(n/2) + O(n^2)$ | 7 | 2 | 2.807 | $O(n^2)$ | Case 1 | $\Theta(n^{\log_2 7}) \approx \Theta(n^{2.807})$ |

---

### Interview Follow-Up Questions
1. **"What is the complexity of $T(n) = 2T(n/2) + n \log n$?"**
   - *Answer:* Standard Master Theorem does not apply directly because $f(n) = n \log n$ is asymptotically larger than $n^{\log_2 2} = n^1$, but not by a polynomial factor $n^\epsilon$. Using the Extended Master Theorem (Case 2 with $p=1$), the solution is $\Theta(n \log^2 n)$.
2. **"How do you solve $T(n) = T(n-1) + O(1)$ vs $T(n) = T(n-1) + O(n)$?"**
   - *Answer:* Unroll the recurrence:
     - $T(n) = T(n-1) + 1 = 1 + 1 + \dots + 1 = O(n)$ (e.g. linear scan / counting).
     - $T(n) = T(n-1) + n = n + (n-1) + (n-2) + \dots + 1 = \frac{n(n+1)}{2} = O(n^2)$ (e.g. Selection Sort).

---

# 5. Amortized Analysis

Amortized analysis guarantees the **average performance of each operation in the worst-case sequence of operations**.

> **Crucial Distinction:** Amortized analysis is **NOT** Average-Case analysis.
> - **Average-case analysis** assumes a probability distribution of random inputs.
> - **Amortized analysis** makes **no probabilistic assumptions**; it guarantees the average time per operation even if an adversary designs the worst possible sequence of operations.

---

### The 3 Formal Methods of Amortized Analysis

```
+-----------------------------------------------------------------------------------------+
| 1. AGGREGATE METHOD                                                                     |
| Calculate total cost of a sequence of n operations T(n) in the worst case.              |
| Amortized Cost per operation = T(n) / n.                                                |
+-----------------------------------------------------------------------------------------+
| 2. ACCOUNTING (BANKER'S) METHOD                                                         |
| Charge each operation an amortized fee ($).                                             |
| - If Actual Cost < Fee -> Save excess as "Prepaid Credit" on specific data tokens.      |
| - If Actual Cost > Fee -> Pay expensive cost using saved accumulated credit.            |
| Requirement: Accumulated Credit must NEVER drop below zero!                             |
+-----------------------------------------------------------------------------------------+
| 3. POTENTIAL (PHYSICIST'S) METHOD                                                       |
| Define a Potential Function Phi(D) mapping data structure state D to a real number.     |
|   Amortized Cost a_i = Actual Cost c_i + [ Phi(D_i) - Phi(D_{i-1}) ]                   |
| - Cheap ops increase potential energy (Phi rises).                                      |
| - Expensive ops release potential energy (Phi drops), paying for actual cost.           |
+-----------------------------------------------------------------------------------------+
```

---

### Classic Case Study: Dynamic Array Resizing (`std::vector` / ArrayList)

A dynamic array doubles its capacity ($N \to 2N$) when full.

```
Operation | Action           | Actual Cost | Array State (Size / Capacity)
--------------------------------------------------------------------------
push(1)   | Insert 1         | 1           | [1]       (1 / 1)
push(2)   | Double + Copy 1  | 1 + 1 = 2   | [1, 2]    (2 / 2)
push(3)   | Double + Copy 2  | 1 + 2 = 3   | [1, 2, 3, _] (3 / 4)
push(4)   | Insert 4         | 1           | [1, 2, 3, 4] (4 / 4)
push(5)   | Double + Copy 4  | 1 + 4 = 5   | [1, 2, 3, 4, 5, _, _, _] (5 / 8)
```

#### 1. Aggregate Method Proof:
For $N$ push operations, array doublings occur at sizes $1, 2, 4, 8, \dots, 2^k \le N$.
$$\text{Total Copying Cost} = 1 + 2 + 4 + 8 + \dots + 2^k = \sum_{i=0}^{\log_2 N} 2^i = 2^{\log_2 N + 1} - 1 = 2N - 1 < 2N$$
$$\text{Total Insertions Cost} = N \times 1 = N$$
$$\text{Total Cost for } N \text{ pushes} = 2N + N = 3N$$
$$\text{Amortized Cost per push} = \frac{\text{Total Cost}}{N} = \frac{3N}{N} = O(1)$$

#### 2. Accounting Method Proof:
- Charge an amortized fee of **$3 per `push()`**:
  - **$1** pays for the actual current insertion.
  - **$1** is stored as credit on the newly inserted element.
  - **$1** is stored as credit on an older element that already moved.
- When the array reaches capacity $N$ and must double, exactly $N/2$ elements were added since the last resizing. Each of those $N/2$ elements holds **$2 in credit**, providing exactly $N/2 \times \$2 = \$N$ in accumulated credit—paying for copying all $N$ elements with zero deficit.

---

### Why Does Adding Fixed Capacity (e.g. $+100$) Fail Amortized $O(1)$?
If an array expands by a fixed increment $+K$ (e.g., $+100$ slots) rather than multiplying ($2 \times$):
- Resizing happens after every $K$ insertions.
- Total copy cost for $N$ elements:
  $$K + 2K + 3K + \dots + \frac{N}{K} K = K \sum_{i=1}^{N/K} i \approx K \cdot \frac{(N/K)^2}{2} = O\left(\frac{N^2}{K}\right)$$
- Amortized cost per operation: $\frac{O(N^2)}{N} = \mathbf{O(N)}$ (**Linear time per push — Disastrous!**).

---

### Interview Follow-Up Questions
1. **"What is the amortized cost of incrementing an $n$-bit binary counter?"**
   - *Answer:* $O(1)$ amortized per increment. Even though incrementing from `0111` to `1000` flips 4 bits (worst-case $O(k)$), bit 0 flips every time ($n$ times), bit 1 flips $n/2$ times, bit 2 flips $n/4$ times. Total bit flips for $n$ increments $= n + n/2 + n/4 + \dots < 2n$. Average flips per increment $= 2n/n = O(1)$.
2. **"What is the amortized time complexity of a Queue implemented using 2 Stacks?"**
   - *Answer:* $O(1)$ amortized for both `push` and `pop`. While transferring $N$ elements from Stack 1 to Stack 2 during `pop()` takes $O(N)$ actual time, each element is pushed to Stack 1 once, popped from Stack 1 once, pushed to Stack 2 once, and popped from Stack 2 once—exactly 4 operations per element over its entire lifetime.

---

# 6. Quick Recap

- **Asymptotic Triad:** Big-O ($O$) is the upper ceiling, Big-Omega ($\Omega$) is the lower floor, and Big-Theta ($\Theta$) is the tight bound.
- **Worst-Case vs. Big-O:** Big-O is a mathematical limit, not a synonym for worst case. Worst case describes the worst input configuration; Big-O describes growth rate.
- **Master Theorem Formula:** For $T(n) = aT(n/b) + \Theta(n^k)$, compare $k$ with $\log_b a$:
  - $k < \log_b a \implies \Theta(n^{\log_b a})$ (Leaves dominate)
  - $k = \log_b a \implies \Theta(n^k \log n)$ (Balanced)
  - $k > \log_b a \implies \Theta(n^k)$ (Root dominates)
- **Dynamic Array Sizing:** Geometric expansion (multiplying by $2\times$ or $1.5\times$) guarantees amortized $O(1)$ insertions; linear expansion ($+K$) degrades amortized time to $O(n)$.
- **Amortized Analysis:** Guarantees average cost per operation over the absolute worst-case sequence of operations without probabilistic assumptions.
