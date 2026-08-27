# Master Guide 01: Foundations, Linear Data Structures & Hashing

> **Focus:** Theoretical & Mathematical Foundations, Asymptotic Analysis, Memory Layouts, Linear Data Structures (Arrays, Strings, Linked Lists, Stacks, Queues), and Deep Dive Hash Table Mechanics.
> 
> *Designed to crush top-tier FAANG/HFT technical interviews without missing a single bit of detail.*

---

# Table of Contents
1. [Asymptotic Analysis & Mathematical Foundations](#1-asymptotic-analysis--mathematical-foundations)
2. [Best, Average & Worst Case Analysis](#2-best-average--worst-case-analysis)
3. [Recurrence Relations & Master Theorem](#3-recurrence-relations--master-theorem)
4. [Amortized Analysis (The Banker's & Physicist's View)](#4-amortized-analysis)
5. [Arrays & Memory Layout (Spatial Locality & Row-Major)](#5-arrays--memory-layout)
6. [Strings & Memory Models (SSO & Immutaiblity)](#6-strings--memory-models)
7. [Linked Lists & Pointer Acrobatics (Floyd's Cycle Proof)](#7-linked-lists--pointer-acrobatics)
8. [Stacks (LIFO & Monotonic Patterns)](#8-stacks-lifo--monotonic-patterns)
9. [Queues (FIFO, Circular & Monotonic Deques)](#9-queues-fifo-circular--monotonic-deques)
10. [Hashing Architecture & Collision Resolution](#10-hashing-architecture--collision-resolution)
11. [Load Factor Dynamics & Dynamic Rehashing](#11-load-factor-dynamics--dynamic-rehashing)
12. [HashMap vs. TreeMap (`unordered_map` vs `map`)](#12-hashmap-vs-treemap)
13. [Interview Quick-Fire Recap](#13-interview-quick-fire-recap)

---

# 1. Asymptotic Analysis & Mathematical Foundations

Asymptotic notation is a mathematical framework used to describe the **limiting behavior** of an algorithm's execution time or memory consumption as the input size $n$ approaches infinity ($n \to \infty$). It frees us from benchmark noise—CPU clock speeds, OS context switches, compiler flags, or language runtimes.

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

### A. Big-O Notation ($O$) — Asymptotic Upper Bound
- **Formal Definition:** $f(n) = O(g(n))$ if there exist positive constants $c > 0$ and $n_0 \ge 1$ such that:
  $$0 \le f(n) \le c \cdot g(n) \quad \text{for all } n \ge n_0$$
- **Intuition:** $g(n)$ provides an **asymptotic ceiling**. The algorithm will grow *no faster* than $g(n)$ for large inputs.
- **Example:** If $f(n) = 3n^2 + 5n + 100$, then $f(n) = O(n^2)$ because for $c = 4$ and $n_0 = 10$, $3n^2 + 5n + 100 \le 4n^2$.

### B. Big-Omega Notation ($\Omega$) — Asymptotic Lower Bound
- **Formal Definition:** $f(n) = \Omega(g(n))$ if there exist positive constants $c > 0$ and $n_0 \ge 1$ such that:
  $$0 \le c \cdot g(n) \le f(n) \quad \text{for all } n \ge n_0$$
- **Intuition:** $g(n)$ provides a **floor**. The algorithm takes *at least* $c \cdot g(n)$ operations.
- **Example:** Any comparison-based sorting algorithm requires $\Omega(n \log n)$ comparisons in the worst case (proven via decision trees).

### C. Big-Theta Notation ($\Theta$) — Asymptotic Tight Bound
- **Formal Definition:** $f(n) = \Theta(g(n))$ if and only if $f(n) = O(g(n))$ and $f(n) = \Omega(g(n))$. Formally:
  $$c_1 \cdot g(n) \le f(n) \le c_2 \cdot g(n) \quad \text{for all } n \ge n_0$$
- **Intuition:** $g(n)$ sandwiches $f(n)$ tightly from above and below.
- **Example:** Merge Sort is $\Theta(n \log n)$ across best, average, and worst cases.

### D. Little-o ($o$) and Little-omega ($\omega$) Notations
- **Little-o ($o$):** Strict upper bound. $f(n)$ grows **strictly slower** than $g(n)$:
  $$\lim_{n \to \infty} \frac{f(n)}{g(n)} = 0 \quad (\text{e.g., } 2n = o(n^2), \text{ but } 2n^2 \ne o(n^2))$$
- **Little-omega ($\omega$):** Strict lower bound. $f(n)$ grows **strictly faster** than $g(n)$:
  $$\lim_{n \to \infty} \frac{f(n)}{g(n)} = \infty$$

### The Asymptotic Hierarchy (Fastest to Slowest)
$$O(1) < O(\log \log n) < O(\log n) < O(\sqrt{n}) < O(n) < O(n \log n) < O(n^2) < O(n^3) < O(2^n) < O(n!)$$

```cpp
// Complexity Hierarchy Examples in C++:

// O(1): Array indexing & Hash map lookup
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
for (int i = 0; i < n; i++) sum += arr[i];

// O(n^2): Matrix Traversal / Selection Sort
for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++) matrix[i][j] = 0;
```

---

### 🔥 FAANG Interview Grill Q&A: Asymptotics

> **Interviewer:** *"Is $O(n)$ equal to $\Theta(n)$?"*  
> **Candidate:** *"No. $O(n)$ is merely an upper bound—a constant-time $O(1)$ algorithm is technically also $O(n)$. $\Theta(n)$ means the algorithm is tightly bounded both above and below by $c \cdot n$."*
>
> **Interviewer:** *"Why do we ignore constant factors like $100n$ vs $n$?"*  
> **Candidate:** *"Because as $n \to \infty$, higher-order growth rates completely dwarf constant factors. At $n = 10^6$, an $O(n^2)$ algorithm takes $10^{12}$ operations even with $c=1$, while a $100n \log n$ algorithm takes only $2 \times 10^7$ operations—over 50,000x faster!"*

---

# 2. Best, Average & Worst Case Analysis

An algorithm's performance depends heavily on **input data arrangements**, not just input size $n$.

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

### Deep-Dive Case Study: QuickSort
- **Best Case ($\Theta(n \log n)$):** Pivot splits array into two equal halves ($n/2$). $T(n) = 2T(n/2) + O(n) \implies \Theta(n \log n)$.
- **Average Case ($\Theta(n \log n)$):** Even a skewed 90%/10% split yields tree depth $\log_{10/9} n = O(\log n)$, keeping total work $\Theta(n \log n)$.
- **Worst Case ($\Theta(n^2)$):** Pivot is always min/max (e.g. sorted array with last element pivot). Partitions are size $0$ and $n-1$: $T(n) = T(n-1) + O(n) \implies \Theta(n^2)$.

```cpp
void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i], j = i - 1;
        // Best case (sorted): inner loop terminates immediately -> O(n)
        // Worst case (reverse): inner loop runs j times -> O(n^2)
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}
```

---

# 3. Recurrence Relations & Master Theorem

A recurrence relation defines a function in terms of its value on smaller inputs.

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

### The Master Theorem (Cookbook Method)
For divide-and-conquer recurrences of the form:
$$T(n) = a \cdot T\left(\frac{n}{b}\right) + f(n) \quad (a \ge 1, b > 1)$$

Compare non-recursive work $f(n) = \Theta(n^k)$ against leaf work $n^{\log_b a}$:

```
+-----------------------------------------------------------------------------------------+
| CASE 1: Leaves Dominate (a > b^k)                                                       |
| If f(n) = O(n^(log_b a - eps)) -> T(n) = Theta(n^(log_b a))                             |
| Example: T(n) = 8T(n/2) + 1000n^2 -> a=8, b=2 -> log_2 8 = 3 > 2 -> T(n) = Theta(n^3)   |
+-----------------------------------------------------------------------------------------+
| CASE 2: Balanced Work Across Levels (a = b^k)                                           |
| If f(n) = Theta(n^(log_b a) * log^p n) -> T(n) = Theta(n^(log_b a) * log^(p+1) n)       |
| Example: Merge Sort T(n) = 2T(n/2) + Theta(n) -> a=2, b=2 -> T(n) = Theta(n log n)       |
+-----------------------------------------------------------------------------------------+
| CASE 3: Root Dominates (a < b^k)                                                        |
| If f(n) = Omega(n^(log_b a + eps)) AND a*f(n/b) <= c*f(n) for c < 1                      |
| -> T(n) = Theta(f(n))                                                                   |
| Example: T(n) = 2T(n/2) + n^2 -> a=2, b=2 -> log_2 2 = 1 < 2 -> T(n) = Theta(n^2)       |
+-----------------------------------------------------------------------------------------+
```

### Master Theorem Summary Table
| Algorithm | Recurrence | $a$ | $b$ | $\log_b a$ | $f(n)$ | Master Case | Runtime |
| :--- | :--- | :---: | :---: | :---: | :--- | :---: | :--- |
| **Binary Search** | $T(n) = T(n/2) + O(1)$ | 1 | 2 | 0 | $O(1) = n^0$ | Case 2 ($p=0$) | $\Theta(\log n)$ |
| **Merge Sort** | $T(n) = 2T(n/2) + O(n)$ | 2 | 2 | 1 | $O(n) = n^1$ | Case 2 ($p=0$) | $\Theta(n \log n)$ |
| **Tree Traversal**| $T(n) = 2T(n/2) + O(1)$ | 2 | 2 | 1 | $O(1) = n^0$ | Case 1 | $\Theta(n)$ |
| **Karatsuba Mult**| $T(n) = 3T(n/2) + O(n)$ | 3 | 2 | 1.585 | $O(n)$ | Case 1 | $\Theta(n^{1.585})$ |
| **Strassen Matrix**| $T(n) = 7T(n/2) + O(n^2)$ | 7 | 2 | 2.807 | $O(n^2)$ | Case 1 | $\Theta(n^{2.807})$ |

---

# 4. Amortized Analysis

Amortized analysis guarantees the **average time per operation over the worst-case sequence of operations**.

> 💡 **Crucial Distinction:** Amortized analysis makes **NO probabilistic assumptions**. Average-case relies on input randomness; Amortized analysis guarantees performance even if an adversary designs the worst input sequence.

```
+-----------------------------------------------------------------------------------------+
| 1. AGGREGATE METHOD: Total Cost T(n) / n.                                               |
| 2. ACCOUNTING (BANKER'S) METHOD: Charge $3 per push. $1 pays insertion, $2 stored as   |
|    prepaid credit to pay for future doubling copy costs.                                |
| 3. POTENTIAL (PHYSICIST'S) METHOD: Amortized Cost a_i = c_i + [ Phi(D_i) - Phi(D_{i-1}) ] |
+-----------------------------------------------------------------------------------------+
```

### Case Study: Dynamic Array Resizing (`std::vector`)
- Expanding $N \to 2N$ doubling capacity:
  $$\text{Total Copy Cost} = 1 + 2 + 4 + \dots + N = 2N - 1 < 2N$$
  $$\text{Total Cost for } N \text{ pushes} = N + 2N = 3N \implies \mathbf{O(1) \text{ Amortized per Push!}}$$
- **Why Linear Expansion ($+100$) Fails:** If an array expands by $+K$ slots:
  $$\text{Total Copy Cost} = K + 2K + \dots + \frac{N}{K}K \approx O(N^2) \implies \mathbf{O(N) \text{ Amortized Time (Disastrous!)}}$$

---

# 5. Arrays & Memory Layout

An **Array** is a contiguous block of homogeneous memory elements stored sequentially in RAM.

```
Memory Address:  0x1000   0x1004   0x1008   0x100C   0x1010
Array Index:      [ 0 ]    [ 1 ]    [ 2 ]    [ 3 ]    [ 4 ]
Value:             10       25       40       55       70

Address Calculation Formula:
Address(arr[i]) = Base_Address + (i * sizeof(DataType))
Address(arr[3]) = 0x1000 + (3 * 4) = 0x100C  --> Instant O(1) pointer jump!
```

### Hardware Reality: CPU Cache Lines & Row-Major Order
- CPUs fetch **64-byte Cache Lines** into L1 cache. Loading `arr[0]` automatically prefetches `arr[1]` to `arr[15]`.
- **Row-Major Order (C/C++/Java/Python):** Entire rows stored contiguously. Row-by-row iteration is up to **10x faster** than column-by-column iteration due to 100% CPU cache line hits vs constant cache misses!

```cpp
// Cache-Friendly (Row-Major):
for (int r = 0; r < rows; r++)
    for (int c = 0; c < cols; c++) sum += matrix[r][c]; // Fast!

// Cache-Hostile (Column-Major):
for (int c = 0; c < cols; c++)
    for (int r = 0; r < rows; r++) sum += matrix[r][c]; // 10x Slower!
```

---

# 6. Strings & Memory Models

A **String** is an array of characters (`char`), terminated by `'\0'` in C/C++ or tracked with an explicit length field.

### Small String Optimization (SSO) in C++
- If a string has **15 or fewer characters** (22 bytes in libc++), `std::string` allocates **ZERO heap memory**!
- It stores the string directly inside the stack object buffer, eliminating expensive `malloc`/`free` calls.

```cpp
std::string shortStr = "Hello"; // Stack storage via SSO (0 Heap allocations)
std::string longStr = "This string exceeds 15 characters limit"; // Allocates on Heap
```

### String Mutability Across Languages
- **C++ (`std::string`):** Mutable in-place ($O(1)$ mutation).
- **Java / Python / JS:** Immutable. `s += "a"` in a loop creates a new object each time, leading to **$O(N^2)$ total time**. Use `StringBuilder` / `''.join()` for $O(N)$ total time!

---

# 7. Linked Lists & Pointer Acrobatics

Linked Lists store nodes non-contiguously in Heap memory connected via pointers.

```
1. Singly Linked List:   [ Head: 10 | next ] ---> [ 20 | next ] ---> NULL
2. Doubly Linked List:   NULL <--- [ prev | 10 | next ] <---> [ prev | 20 | next ] ---> NULL
```

### Dummy Sentinel Nodes & Floyd's Cycle Proof
- **Dummy Nodes (`ListNode dummy(0)`):** Eliminates 90% of pointer edge-case bugs (no conditional logic for inserting/deleting at head).

```
Floyd's Cycle Entry Proof:
Let L = distance from Head to Cycle Start.
Let C = Cycle Length, d = distance from Cycle Start to Meeting Point.
Distance Slow = L + d
Distance Fast = L + d + k*C
Since Fast is 2x speed: 2(L + d) = L + d + k*C ==> L + d = k*C ==> L = k*C - d!
Conclusion: Resetting one pointer to Head and advancing both at 1 step/sec guarantees meeting at Cycle Entry after L steps!
```

```cpp
ListNode* detectCycle(ListNode* head) {
    ListNode *slow = head, *fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            ListNode* entry = head;
            while (entry != slow) {
                entry = entry->next;
                slow = slow->next;
            }
            return entry; // Cycle Start
        }
    }
    return nullptr;
}
```

---

# 8. Stacks (LIFO & Monotonic Patterns)

### Monotonic Stack Pattern
Maintains elements in strictly increasing or decreasing order.
- **Applications:** Next Greater Element, Stock Span, Largest Rectangle in Histogram.
- **Time Complexity:** Every element is pushed once and popped at most once $\implies \mathbf{O(N) \text{ Total Time}}$.

```cpp
// Next Greater Element to Right in O(N) Time
vector<int> nextGreaterElement(const vector<int>& nums) {
    int n = nums.size();
    vector<int> result(n, -1);
    stack<int> st;
    for (int i = 0; i < n; i++) {
        while (!st.empty() && nums[i] > nums[st.top()]) {
            result[st.top()] = nums[i];
            st.pop();
        }
        st.push(i);
    }
    return result;
}
```

---

# 9. Queues (FIFO, Circular & Monotonic Deques)

### Monotonic Deque: Sliding Window Maximum in $O(N)$
Maintains indices in decreasing order of value (`dq.front()` is window max).

```cpp
vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    deque<int> dq;
    vector<int> ans;
    for (int i = 0; i < nums.size(); i++) {
        if (!dq.empty() && dq.front() <= i - k) dq.pop_front();
        while (!dq.empty() && nums[dq.back()] <= nums[i]) dq.pop_back();
        dq.push_back(i);
        if (i >= k - 1) ans.push_back(nums[dq.front()]);
    }
    return ans;
}
```

---

# 10. Hashing Architecture & Collision Resolution

Hash tables compute index $h(k) \pmod M$ to achieve average $O(1)$ lookups.

```
Collision Resolution Taxonomy:
1. Separate Chaining: Linked lists / BSTs at each bucket index.
2. Open Addressing: Probing directly inside contiguous array (Linear, Quadratic, Double Hashing).
```

### Separate Chaining vs Open Addressing
- **Java 8+ Treeification:** When a bucket linked list reaches $\ge 8$ elements, Java transforms it into a **Red-Black Tree** (upgrading worst-case search from $O(N)$ to $O(\log N)$).
- **Open Addressing & Tombstones:** Deletions **cannot** write `NULL` (breaks probe chains). They write `TOMBSTONE` markers.
- **Double Hashing Formula:** $h(k, i) = (h_1(k) + i \cdot h_2(k)) \pmod M$. Completely eliminates primary and secondary clustering.

---

# 11. Load Factor Dynamics & Dynamic Rehashing

$$\text{Load Factor } (\alpha) = \frac{N}{M} = \frac{\text{Total Stored Elements}}{\text{Total Capacity}}$$

- **Rehash Trigger:** Capped at $\alpha = 0.75$ (`unordered_map` / Java `HashMap`).
- **Resizing Procedure:** Allocates double capacity ($2M$, prime). Recomputes $\text{new\_index} = \text{hashCode} \pmod{2M}$ for **all** elements. Amortized cost is $O(1)$ per insert.

---

# 12. HashMap vs. TreeMap

| Dimension | `std::unordered_map` (HashMap) | `std::map` (TreeMap) |
| :--- | :--- | :--- |
| **Engine** | Hash Table (Separate Chaining) | Red-Black Tree (Self-Balancing BST) |
| **Average Time** | $O(1)$ Search, Insert, Delete | $O(\log N)$ Search, Insert, Delete |
| **Worst Time** | $O(N)$ (Severe collisions) | $O(\log N)$ (Guaranteed) |
| **Ordering** | Arbitrary / Unordered | Strictly Sorted by Key |
| **Range Queries**| Not supported ($O(N)$ scan) | Fast $O(\log N)$ (`lower_bound` / `upper_bound`) |

---

# 13. Interview Quick-Fire Recap

1. **Big-O vs Worst Case:** Big-O is an upper bound function limit; worst case is a data configuration scenario.
2. **Dynamic Array Resizing:** Doubling capacity gives amortized $O(1)$ time; linear $+K$ expansion degrades to $O(N)$ amortized time.
3. **Cache Line Advantage:** Arrays match 64-byte CPU cache lines; linked lists cause cache misses across heap memory.
4. **Floyd's Cycle Entry:** Meeting point to cycle entry distance equals head to cycle entry distance ($L = kC - d$).
5. **Monotonic Stack/Deque:** Guarantees $O(N)$ time by pushing and popping each index at most once.
6. **Open Addressing Tombstones:** Required so probe chains are not prematurely terminated by deletions.
7. **Java 8 Treeification:** Converts bucket linked lists with $\ge 8$ items to Red-Black Trees for $O(\log N)$ worst-case safety.
