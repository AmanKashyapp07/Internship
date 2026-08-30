# CSES Sliding Window Problems - Self-Contained Interview Cheat Sheet

This document contains **problem statements, interview-style explanations, intuitive breakdowns, step-by-step algorithms, structured pseudocode, and complexity analysis** for all 5 problems in the **Sliding Window Problems** topic.

---

## 1. Sliding Window Mode

- **Tier**: Tier 2
- **Link**: [Sliding Window Mode](https://cses.fi/problemset/task/2184)
- **Problem Statement**: Given an array of $N$ integers and a window size $K$, find the mode (most frequent element) in every sliding window of size $K$. (If tie, choose smallest value).
- **Interview Pattern**: Multi-Set / Frequency Buckets in Sliding Window.
- **Intuition**:
  - Maintain `freq[val]` map and a `std::set<pair<int, int>>` storing `(-freq[val], val)` to dynamically maintain maximum frequency element at the top.
- **Step-by-Step Interview Walkthrough**:
  1. Maintain `set<pair<int, int>> mode_set` storing `(-freq[val], val)`.
  2. To update element $x$ count by $\Delta$:
     - Remove `(-freq[x], x)` from set.
     - `freq[x] += delta`.
     - Insert `(-freq[x], x)` into set.
  3. Window mode is `mode_set.begin()->second`.
- **Pseudocode**:
```text
function update(x, delta):
    mode_set.erase({-freq[x], x})
    freq[x] += delta
    mode_set.insert({-freq[x], x})
```
- **Complexity**: Time: $\mathcal{O}(N \log K)$, Space: $\mathcal{O}(K)$.

---

---

## 2. Sliding Window Mex

- **Tier**: Tier 2
- **Link**: [Sliding Window Mex](https://cses.fi/problemset/task/2185)
- **Problem Statement**: Given an array of $N$ integers and a window size $K$, find the MEX (minimum excluded non-negative integer) in every sliding window of size $K$.
- **Interview Pattern**: Frequency Map + Active Missing Value Set (`std::set`).
- **Intuition**:
  - Maintain frequency map of elements in current window and a `std::set` of missing values $\le K$.
  - The MEX is always `*missing_set.begin()`.
- **Step-by-Step Interview Walkthrough**:
  1. Insert $0 \dots K$ into `missing_set`.
  2. For incoming element $x$: if $x \le K$, increment `freq[x]`; if `freq[x] == 1`, erase $x$ from `missing_set`.
  3. For outgoing element $y$: if $y \le K$, decrement `freq[y]`; if `freq[y] == 0`, insert $y$ into `missing_set`.
  4. Window MEX is `*missing_set.begin()`.
- **Pseudocode**:
```text
missing_set = {0, 1, 2, ..., K}
for x in first K elements: add(x)
print *missing_set.begin()

for i = K to N-1:
    remove(A[i - K])
    add(A[i])
    print *missing_set.begin()
```
- **Complexity**: Time: $\mathcal{O}(N \log K)$, Space: $\mathcal{O}(K)$.

---

---

## 3. Sliding Window Median

- **Tier**: Tier 2
- **Link**: [Sliding Window Median](https://cses.fi/problemset/task/1076)
- **Problem Statement**: Given an array of $N$ integers and a window size $K$, find the median element in every sliding window of size $K$.
- **Interview Pattern**: Dual Multisets / Two Heaps Balancing (`left_max`, `right_min`).
- **Intuition**:
  - Split window elements into two balanced multisets: `left_set` (lower half of size $\lceil K/2 \rceil$) and `right_set` (upper half of size $\lfloor K/2 \rfloor$).
  - Median is always `*left_set.rbegin()`.
- **Step-by-Step Interview Walkthrough**:
  1. Maintain `left_set` and `right_set`.
  2. Insert element $x$: add to `left_set`, then move `*left_set.rbegin()` to `right_set`. If `left_set.size() < right_set.size()`, move `*right_set.begin()` back to `left_set`.
  3. Erase outgoing element $y$: remove from `left_set` or `right_set`, rebalance sizes.
  4. Median is `*left_set.rbegin()`.
- **Pseudocode**:
```text
function add(x):
    left_set.insert(x)
    right_set.insert(*left_set.rbegin())
    left_set.erase(prev(left_set.end()))
    if left_set.size() < right_set.size():
        left_set.insert(*right_set.begin())
        right_set.erase(right_set.begin())
```
- **Complexity**: Time: $\mathcal{O}(N \log K)$, Space: $\mathcal{O}(K)$.

---

---

## 4. Sliding Window Cost

- **Tier**: Tier 2
- **Link**: [Sliding Window Cost](https://cses.fi/problemset/task/1077)
- **Problem Statement**: Given an array of $N$ integers and a window size $K$, find the minimum cost to make all elements in every sliding window equal. (Cost = sum of absolute differences $|x_i - \text{median}|$).
- **Interview Pattern**: Dual Multisets with Running Sum Tracking.
- **Intuition**:
  - Minimum cost is achieved by setting all elements to the **median** of the window!
  - Cost formula: $(\text{median} \times |L| - \text{sum}_L) + (\text{sum}_R - \text{median} \times |R|)$.
  - Maintain running sums `sum_L` and `sum_R` alongside dual multisets.
- **Step-by-Step Interview Walkthrough**:
  1. Maintain `left_set`, `right_set`, `sum_L`, `sum_R` using Dual Multiset logic from Sliding Window Median.
  2. Window cost = $(\text{median} \cdot |L| - \text{sum}_L) + (\text{sum}_R - \text{median} \cdot |R|)$.
  3. Update running sums `sum_L` and `sum_R` on every insert/erase/rebalance.
- **Pseudocode**:
```text
median = *left_set.rbegin()
cost = (median * left_set.size() - sum_L) + (sum_R - median * right_set.size())
return cost
```
- **Complexity**: Time: $\mathcal{O}(N \log K)$, Space: $\mathcal{O}(K)$.

---

---

## 5. Sliding Window Inversions

- **Tier**: Tier 3
- **Link**: [Sliding Window Inversions](https://cses.fi/problemset/task/2186)
- **Problem Statement**: Given an array of $N$ integers and a window size $K$, count the number of inversion pairs $(i, j)$ with $i < j$ and $A[i] > A[j]$ in every sliding window of size $K$.
- **Interview Pattern**: Fenwick Tree (BIT) / Order Statistic Tree in Sliding Window.
- **Intuition**:
  - Maintain a Fenwick Tree storing frequencies of elements in current window (coordinate compressed).
  - Adding incoming element $x$: adds count of elements currently in window strictly greater than $x$.
  - Removing outgoing element $y$: subtracts count of elements currently in window strictly smaller than $y$.
- **Step-by-Step Interview Walkthrough**:
  1. Coordinate compress array elements.
  2. Maintain Fenwick Tree and running `inversions_count = 0`.
  3. Insert $x$: `inversions_count += BIT.query_range(x + 1, MAX)`, `BIT.add(x, +1)`.
  4. Remove $y$: `BIT.add(y, -1)`, `inversions_count -= BIT.query_range(1, y - 1)`.
  5. Output `inversions_count`.
- **Pseudocode**:
```text
for i = 0 to N-1:
    inversions += BIT.query_greater(A[i])
    BIT.add(A[i], +1)
    if i >= K:
        BIT.add(A[i - K], -1)
        inversions -= BIT.query_smaller(A[i - K])
    if i >= K - 1:
        print inversions
```
- **Complexity**: Time: $\mathcal{O}(N \log K)$, Space: $\mathcal{O}(K)$.
