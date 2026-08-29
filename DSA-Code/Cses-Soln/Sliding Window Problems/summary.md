# CSES Sliding Window Problems - Self-Contained Interview Cheat Sheet

This document contains **problem statements, interview-style explanations, intuitive breakdowns, step-by-step algorithms, structured pseudocode, and complexity analysis** for all 10 problems in the **Sliding Window Problems** topic.

---

## 1. Sliding Window Minimum

- **Link**: [Sliding Window Minimum](https://cses.fi/problemset/task/2181)
- **Problem Statement**: Given an array of $N$ integers and a window size $K$, find the minimum element in every sliding window of size $K$.
- **Interview Pattern**: Monotonic Deque (Sliding Window Min/Max).
- **Intuition**:
  - Maintain a double-ended queue (deque) storing indices of elements in increasing order of their values.
  - Front of deque always holds the index of the minimum element in the current window.
- **Step-by-Step Interview Walkthrough**:
  1. Maintain deque `dq` of indices.
  2. For $i$ from 0 to $N-1$:
     - Remove elements from back of `dq` while $A[\text{dq.back()}] \ge A[i]$.
     - Push $i$ to back of `dq`.
     - Remove front of `dq` if it falls outside window ($\text{dq.front()} \le i - K$).
     - If $i \ge K - 1$, record $A[\text{dq.front()}]$ as minimum for current window.
- **Pseudocode**:
```text
for i = 0 to N-1:
    while not dq.empty() and A[dq.back()] >= A[i]:
        dq.pop_back()
    dq.push_back(i)
    if dq.front() <= i - K:
        dq.pop_front()
    if i >= K - 1:
        print A[dq.front()]
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(K)$.

---

## 2. Sliding Window Sum

- **Link**: [Sliding Window Sum](https://cses.fi/problemset/task/2182)
- **Problem Statement**: Given an array of $N$ integers and a window size $K$, compute the sum of elements in every sliding window of size $K$.
- **Interview Pattern**: Sliding Window Running Total.
- **Intuition**:
  - When sliding the window by 1 position to the right, add the incoming element $A[i]$ and subtract the outgoing element $A[i-K]$.
- **Step-by-Step Interview Walkthrough**:
  1. Compute sum of first $K$ elements `window_sum`. Output `window_sum`.
  2. For $i$ from $K$ to $N-1$:
     - `window_sum += A[i] - A[i - K]`.
     - Output `window_sum`.
- **Pseudocode**:
```text
curr_sum = sum(A[0..K-1])
print curr_sum
for i = K to N-1:
    curr_sum += A[i] - A[i - K]
    print curr_sum
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(1)$ auxiliary.

---

## 3. Sliding Window Distinct Values

- **Link**: [Sliding Window Distinct Values](https://cses.fi/problemset/task/2183)
- **Problem Statement**: Given an array of $N$ integers and a window size $K$, count the number of distinct values in every sliding window of size $K$.
- **Interview Pattern**: Sliding Window Frequency Map / Counter.
- **Intuition**:
  - Maintain a hash map / frequency array of elements in current window.
  - Increment count for incoming element $A[i]$; if count becomes 1, increment `distinct_count`.
  - Decrement count for outgoing element $A[i-K]$; if count drops to 0, decrement `distinct_count`.
- **Step-by-Step Interview Walkthrough**:
  1. Populate frequency map for first $K$ elements. Record initial `distinct_count`.
  2. For $i$ from $K$ to $N-1$:
     - Remove $A[i-K]$: decrement freq; if 0, `distinct_count -= 1`.
     - Add $A[i]$: increment freq; if 1, `distinct_count += 1`.
     - Output `distinct_count`.
- **Pseudocode**:
```text
for i = 0 to K-1: add(A[i])
print freq_map.size()

for i = K to N-1:
    remove(A[i - K])
    add(A[i])
    print freq_map.size()
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(K)$.

---

## 4. Sliding Window Mode

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

## 5. Sliding Window Mex

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

## 6. Sliding Window Median

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

## 7. Sliding Window Cost

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

## 8. Sliding Window Inversions

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

---

## 9. Sliding Window Or

- **Link**: [Sliding Window Or](https://cses.fi/problemset/task/2187)
- **Problem Statement**: Given an array of $N$ integers and a window size $K$, find the bitwise OR sum of elements in every sliding window of size $K$.
- **Interview Pattern**: Sliding Window 32-Bit Count Array.
- **Intuition**:
  - Bitwise OR is non-invertible by direct subtraction, but tracking the count of set bits at each bit position $0 \dots 30$ in the current window allows $\mathcal{O}(1)$ OR updates!
  - Bit $b$ of window OR is 1 iff `bit_count[b] > 0`.
- **Step-by-Step Interview Walkthrough**:
  1. Maintain `bit_count[0..30] = 0`.
  2. Add $x$: for each set bit $b$ of $x$, `bit_count[b] += 1`.
  3. Remove $y$: for each set bit $b$ of $y$, `bit_count[b] -= 1`.
  4. Reconstruct OR sum: `window_or = sum(1 << b for b where bit_count[b] > 0)`.
- **Pseudocode**:
```text
function add(x):
    for b = 0 to 30:
        if x & (1 << b): bit_count[b] += 1

function remove(y):
    for b = 0 to 30:
        if y & (1 << b): bit_count[b] -= 1

function get_or():
    res = 0
    for b = 0 to 30:
        if bit_count[b] > 0: res |= (1 << b)
    return res
```
- **Complexity**: Time: $\mathcal{O}(N \log(\max A))$, Space: $\mathcal{O}(\log(\max A))$.

---

## 10. Sliding Window Xor

- **Link**: [Sliding Window Xor](https://cses.fi/problemset/task/2188)
- **Problem Statement**: Given an array of $N$ integers and a window size $K$, compute the bitwise XOR sum of elements in every sliding window of size $K$.
- **Interview Pattern**: Sliding Window Self-Inverse XOR Update.
- **Intuition**:
  - XOR is its own inverse ($x \oplus x = 0$).
  - Sliding the window requires simply updating `window_xor ^= incoming ^ outgoing`.
- **Step-by-Step Interview Walkthrough**:
  1. Compute XOR sum of first $K$ elements `window_xor`. Output `window_xor`.
  2. For $i$ from $K$ to $N-1$:
     - `window_xor ^= A[i] ^ A[i - K]`.
     - Output `window_xor`.
- **Pseudocode**:
```text
window_xor = 0
for i = 0 to K-1: window_xor ^= A[i]
print window_xor

for i = K to N-1:
    window_xor ^= A[i] ^ A[i - K]
    print window_xor
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(1)$ auxiliary.
