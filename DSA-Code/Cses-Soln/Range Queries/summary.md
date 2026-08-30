# CSES Range Queries - Self-Contained Interview Cheat Sheet

This document contains **problem statements, interview-style explanations, intuitive breakdowns, step-by-step algorithms, structured pseudocode, and complexity analysis** for all 14 problems in the **Range Queries** topic.

---

## 1. Static Range Sum Queries

- **Tier**: Tier 1
- **Link**: [Static Range Sum Queries](https://cses.fi/problemset/task/1646)
- **Problem Statement**: Given an array of $N$ integers, process $Q$ static range sum queries asking for the sum of values in range $[l, r]$.
- **Interview Pattern**: 1D Prefix Sum Array.
- **Intuition**:
  - Precompute cumulative prefix sums `pref[i] = pref[i-1] + A[i]`.
  - Any range sum $[l, r]$ can be computed in $\mathcal{O}(1)$ time as $\text{pref}[r] - \text{pref}[l-1]$.
- **Step-by-Step Interview Walkthrough**:
  1. Build prefix sum array `pref[0..N]` where `pref[i] = pref[i-1] + A[i]`.
  2. For each query $[l, r]$ (1-indexed):
     - Output `pref[r] - pref[l-1]`.
- **Pseudocode**:
```text
for i = 1 to N:
    pref[i] = pref[i-1] + A[i]

for query (l, r):
    print pref[r] - pref[l-1]
```
- **Complexity**: Time: $\mathcal{O}(N + Q)$, Space: $\mathcal{O}(N)$.

---

## 2. Static Range Minimum Queries

- **Tier**: Tier 2
- **Link**: [Static Range Minimum Queries](https://cses.fi/problemset/task/1647)
- **Problem Statement**: Given an array of $N$ integers, process $Q$ static range minimum queries asking for the minimum value in range $[l, r]$.
- **Interview Pattern**: Sparse Table ($\mathcal{O}(1)$ Overlapping Range Query).
- **Intuition**:
  - Minimum is an idempotent operator ($\min(a, a) = a$).
  - Precompute `st[j][i]` = min in window of length $2^j$ starting at index $i$.
  - Range $[l, r]$ of length $L = r - l + 1$ can be covered by two overlapping intervals of length $2^{\lfloor \log_2 L \rfloor}$.
- **Step-by-Step Interview Walkthrough**:
  1. Build Sparse Table: `st[0][i] = A[i]`.
  2. For $j = 1 \dots \log N$: `st[j][i] = min(st[j-1][i], st[j-1][i + 2^(j-1)])`.
  3. For query $[l, r]$: compute $k = \lfloor \log_2(r - l + 1) \rfloor$.
  4. Return $\min(\text{st}[k][l], \text{st}[k][r - 2^k + 1])$.
- **Pseudocode**:
```text
for i = 0 to N-1: st[0][i] = A[i]
for j = 1 to LOG-1:
    for i = 0 to N - (1<<j):
        st[j][i] = min(st[j-1][i], st[j-1][i + (1<<(j-1))])

function query(l, r):
    k = log2(r - l + 1)
    return min(st[k][l], st[k][r - (1<<k) + 1])
```
- **Complexity**: Build: $\mathcal{O}(N \log N)$, Query: $\mathcal{O}(1)$, Space: $\mathcal{O}(N \log N)$.

---

## 3. Dynamic Range Sum Queries

- **Tier**: Tier 2
- **Link**: [Dynamic Range Sum Queries](https://cses.fi/problemset/task/1648)
- **Problem Statement**: Given an array of $N$ integers, process $Q$ queries: (1) update element at index $k$ to value $u$, (2) calculate range sum $[l, r]$.
- **Interview Pattern**: Fenwick Tree (BIT) / Segment Tree.
- **Intuition**:
  - Point update and prefix sum queries can be maintained efficiently in $\mathcal{O}(\log N)$ using a Binary Indexed Tree (BIT).
- **Step-by-Step Interview Walkthrough**:
  1. Build BIT of size $N$ using initial array values.
  2. Type 1 update $(k, u)$: compute diff $= u - A[k]$, update $A[k] = u$, and `BIT.add(k, diff)`.
  3. Type 2 query $[l, r]$: output `BIT.query(r) - BIT.query(l-1)`.
- **Pseudocode**:
```text
for i = 1 to N: BIT.add(i, A[i])

for query:
    if type == 1:
        diff = val - A[k]
        A[k] = val
        BIT.add(k, diff)
    else:
        print BIT.query(r) - BIT.query(l - 1)
```
- **Complexity**: Build: $\mathcal{O}(N \log N)$, Query/Update: $\mathcal{O}(\log N)$, Space: $\mathcal{O}(N)$.

---

## 4. Dynamic Range Minimum Queries

- **Tier**: Tier 2
- **Link**: [Dynamic Range Minimum Queries](https://cses.fi/problemset/task/1649)
- **Problem Statement**: Given an array of $N$ integers, process $Q$ queries: (1) update element at index $k$ to value $u$, (2) find minimum value in range $[l, r]$.
- **Interview Pattern**: Segment Tree (Point Update, Range Min Query).
- **Intuition**:
  - Segment Tree node $v$ stores the minimum value of its covered interval. Point update updates $\mathcal{O}(\log N)$ nodes bottom-up; range query merges $\mathcal{O}(\log N)$ node minimums top-down.
- **Step-by-Step Interview Walkthrough**:
  1. Build Segment Tree recursively over array $A$.
  2. Type 1 update $(k, u)$: recurse down to leaf $k$, update value to $u$, update parent nodes `tree[v] = min(tree[2v], tree[2v+1])`.
  3. Type 2 query $[l, r]$: return minimum of subtrees completely covered by $[l, r]$.
- **Pseudocode**:
```text
function update(v, l, r, idx, val):
    if l == r: tree[v] = val; return
    m = (l + r) / 2
    if idx <= m: update(2v, l, m, idx, val)
    else: update(2v+1, m+1, r, idx, val)
    tree[v] = min(tree[2v], tree[2v+1])

function query(v, l, r, ql, qr):
    if qr < l or ql > r: return INF
    if ql <= l and r <= qr: return tree[v]
    m = (l + r) / 2
    return min(query(2v, l, m, ql, qr), query(2v+1, m+1, r, ql, qr))
```
- **Complexity**: Time: $\mathcal{O}((N + Q) \log N)$, Space: $\mathcal{O}(N)$.

---

## 5. Range Update Queries

- **Tier**: Tier 2
- **Link**: [Range Update Queries](https://cses.fi/problemset/task/1651)
- **Problem Statement**: Given an array of $N$ integers, process $Q$ queries: (1) add value $u$ to all elements in range $[l, r]$, (2) query value at single index $k$.
- **Interview Pattern**: Fenwick Tree on Difference Array.
- **Intuition**:
  - Let $D[i] = A[i] - A[i-1]$ be the difference array. Then value $A[k] = \sum_{j=1}^{k} D[j]$.
  - Adding $u$ to range $[l, r]$ corresponds to 2 point updates on $D$: $D[l] \mathrel{+}= u$ and $D[r+1] \mathrel{-}= u$.
- **Step-by-Step Interview Walkthrough**:
  1. Build Fenwick Tree over initial difference array $D[i] = A[i] - A[i-1]$.
  2. Type 1 range update $[l, r] \mathrel{+}= u$: `BIT.add(l, +u)`, `BIT.add(r+1, -u)`.
  3. Type 2 point query $k$: return `BIT.prefix_sum(k)`.
- **Pseudocode**:
```text
// Range update [l, r] += u
BIT.add(l, +u)
BIT.add(r + 1, -u)

// Point query at k
return BIT.prefix_sum(k)
```
- **Complexity**: Time: $\mathcal{O}((N + Q) \log N)$, Space: $\mathcal{O}(N)$.

---

## 6. Range Xor Queries

- **Tier**: Tier 1
- **Link**: [Range Xor Queries](https://cses.fi/problemset/task/1650)
- **Problem Statement**: Given an array of $N$ integers, process $Q$ static queries asking for the XOR sum of elements in range $[l, r]$.
- **Interview Pattern**: 1D Prefix XOR Array.
- **Intuition**:
  - XOR is its own inverse ($x \oplus x = 0$).
  - Prefix XOR `pref[i] = pref[i-1] ^ A[i]`.
  - Range XOR $[l, r]$ is $\text{pref}[r] \oplus \text{pref}[l-1]$.
- **Step-by-Step Interview Walkthrough**:
  1. Compute `pref[i] = pref[i-1] ^ A[i]` for $i = 1 \dots N$.
  2. For query $[l, r]$: output `pref[r] ^ pref[l-1]`.
- **Pseudocode**:
```text
for i = 1 to N:
    pref[i] = pref[i-1] ^ A[i]

for query (l, r):
    print pref[r] ^ pref[l-1]
```
- **Complexity**: Time: $\mathcal{O}(N + Q)$, Space: $\mathcal{O}(N)$.

---

## 7. Forest Queries

- **Tier**: Tier 2
- **Link**: [Forest Queries](https://cses.fi/problemset/task/1652)
- **Problem Statement**: Given an $N \times N$ grid of trees (`*`) and empty spaces (`.`), process $Q$ queries asking for total trees inside subgrid $(y_1, x_1)$ to $(y_2, x_2)$.
- **Interview Pattern**: 2D Prefix Sum Array (Inclusion-Exclusion).
- **Intuition**:
  - `pref[r][c]` = total trees in subgrid from $(1, 1)$ to $(r, c)$.
  - `pref[r][c] = tree + pref[r-1][c] + pref[r][c-1] - pref[r-1][c-1]`.
  - Subgrid sum $(y_1, x_1) \dots (y_2, x_2) = \text{pref}[y_2][x_2] - \text{pref}[y_1-1][x_2] - \text{pref}[y_2][x_1-1] + \text{pref}[y_1-1][x_1-1]$.
- **Step-by-Step Interview Walkthrough**:
  1. Build 2D prefix array `pref[r][c]`.
  2. For each query $(y_1, x_1, y_2, x_2)$: compute inclusion-exclusion formula in $\mathcal{O}(1)$.
- **Pseudocode**:
```text
for r = 1 to N:
    for c = 1 to N:
        pref[r][c] = is_tree(r, c) + pref[r-1][c] + pref[r][c-1] - pref[r-1][c-1]

for query (y1, x1, y2, x2):
    ans = pref[y2][x2] - pref[y1-1][x2] - pref[y2][x1-1] + pref[y1-1][x1-1]
    print ans
```
- **Complexity**: Time: $\mathcal{O}(N^2 + Q)$, Space: $\mathcal{O}(N^2)$.

---

## 8. Hotel Queries

- **Tier**: Tier 2
- **Link**: [Hotel Queries](https://cses.fi/problemset/task/1143)
- **Problem Statement**: Given $N$ hotels with available room counts $A_i$ and $M$ groups of tourists with required room counts $X_j$, assign each group to the first hotel with $\ge X_j$ rooms and decrease its rooms by $X_j$. Output assigned hotel index or 0 if none available.
- **Interview Pattern**: Segment Tree Range Max + Binary Walk on Tree.
- **Intuition**:
  - Maintain a Range Max Segment Tree.
  - To find the **first** hotel with $\ge X$ rooms, walk down the tree: if left child max $\ge X$, go left; else go right.
- **Step-by-Step Interview Walkthrough**:
  1. Build Range Max Segment Tree over hotel room array $A$.
  2. For tourist requirement $X$:
     - If `root.max < X`: output 0.
     - Else walk down tree: if `left_child.max >= X`, move to left child, else right child.
     - Upon reaching leaf index $i$: output $i+1$, decrease $A[i]$ by $X$, update Segment Tree point value.
- **Pseudocode**:
```text
function first_ge(v, l, r, X):
    if tree[v] < X: return -1
    if l == r: return l
    m = (l + r) / 2
    if tree[2v] >= X: return first_ge(2v, l, m, X)
    else: return first_ge(2v+1, m+1, r, X)
```
- **Complexity**: Time: $\mathcal{O}((N + M) \log N)$, Space: $\mathcal{O}(N)$.

---

## 9. List Removals

- **Tier**: Tier 2
- **Link**: [List Removals](https://cses.fi/problemset/task/1749)
- **Problem Statement**: Given an initial list of $N$ integers, process $N$ queries where each query specifies 1-based index $k$ of element to be removed from the **current** list. Print removed values.
- **Interview Pattern**: Fenwick Tree Binary Lifting (Find $k$-th Active Element).
- **Intuition**:
  - Maintain a BIT initialized with 1 for all active positions.
  - Query $k$ asks for index $i$ where prefix sum equals $k$.
  - Binary lifting on BIT finds index $i$ in $\mathcal{O}(\log N)$ time.
- **Step-by-Step Interview Walkthrough**:
  1. Initialize BIT with 1 at all indices $0 \dots N-1$.
  2. For each query $k$:
     - Find index $i$ with prefix sum $k$ using power-of-2 binary lifting on BIT.
     - Print $A[i]$.
     - Update BIT at index $i$ by -1 (mark removed).
- **Pseudocode**:
```text
BIT.init_all_ones()
for query k:
    idx = 0
    for pw = 1 << LOG down to 1:
        if idx + pw <= N and BIT[idx + pw] < k:
            idx += pw
            k -= BIT[idx]
    // idx is 0-indexed position
    print A[idx]
    BIT.add(idx, -1)
```
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(N)$.

---

## 10. Salary Queries

- **Tier**: Tier 2
- **Link**: [Salary Queries](https://cses.fi/problemset/task/1144)
- **Problem Statement**: Given salaries of $N$ employees, process $Q$ queries: (1) change salary of employee $k$ to $x$, (2) count employees with salary in range $[a, b]$.
- **Interview Pattern**: Coordinate Compression + Fenwick Tree.
- **Intuition**:
  - Collect all initial salaries and query salary values, compress to rank range $[0, K]$.
  - Maintain frequency of each compressed salary in a Fenwick Tree.
- **Step-by-Step Interview Walkthrough**:
  1. Collect all salary values from input and queries; sort and unique to build coordinate compressor.
  2. Populate Fenwick Tree with initial employee salaries.
  3. Type 1 update $(k, x)$: decrement count of old salary `sal[k]` in BIT, update `sal[k] = x`, increment count of new salary `x` in BIT.
  4. Type 2 query $[a, b]$: output `BIT.query(compress(b)) - BIT.query(compress(a) - 1)`.
- **Pseudocode**:
```text
compress_all_salaries()
for s in initial_salaries: BIT.add(compress(s), +1)

for query:
    if type == '!':
        BIT.add(compress(sal[k]), -1)
        sal[k] = new_sal
        BIT.add(compress(new_sal), +1)
    else:
        print BIT.query(compress(b)) - BIT.query(compress(a) - 1)
```
- **Complexity**: Time: $\mathcal{O}((N + Q) \log(N + Q))$, Space: $\mathcal{O}(N + Q)$.

---

## 11. Prefix Sum Queries

- **Tier**: Tier 2
- **Link**: [Prefix Sum Queries](https://cses.fi/problemset/task/2166)
- **Problem Statement**: Given an array of $N$ integers, process $Q$ queries: (1) update element at index $k$ to $u$, (2) find maximum prefix sum in subarray $[l, r]$.
- **Interview Pattern**: Segment Tree with Augmented Node `(sum, max_prefix)`.
- **Intuition**:
  - Segment Tree node stores `{sum, max_pref}`.
  - Merging left child $L$ and right child $R$:
    - `res.sum = L.sum + R.sum`
    - `res.max_pref = max(L.max_pref, L.sum + R.max_pref)`
- **Step-by-Step Interview Walkthrough**:
  1. Build Segment Tree using node struct `{sum, max_pref}`.
  2. Point update $(k, u)$: update leaf to `{u, max(0, u)}`, recompute parents using merge formula.
  3. Range query $[l, r]$: merge covered subtrees top-down and return `.max_pref`.
- **Pseudocode**:
```text
function merge(L, R):
    return {L.sum + R.sum, max(L.max_pref, L.sum + R.max_pref)}

function update(k, u):
    leaf[k] = {u, max(0, u)}
    update_parents()

function query(l, r):
    return query_segment_tree(l, r).max_pref
```
- **Complexity**: Time: $\mathcal{O}((N + Q) \log N)$, Space: $\mathcal{O}(N)$.

---

## 12. Subarray Sum Queries

- **Tier**: Tier 3
- **Link**: [Subarray Sum Queries](https://cses.fi/problemset/task/1190)
- **Problem Statement**: Given an array of $N$ integers, after each of $Q$ point updates, print the maximum subarray sum in the **entire array**.
- **Interview Pattern**: Segment Tree Kadane's Node `(sum, pref, suff, max_ans)`.
- **Intuition**:
  - Each node stores 4 values: `sum`, `pref` (max prefix), `suff` (max suffix), `ans` (max subarray sum).
  - Merging $L$ and $R$:
    - `ans = max({L.ans, R.ans, L.suff + R.pref})`.
- **Step-by-Step Interview Walkthrough**:
  1. Build Segment Tree with Kadane node struct.
  2. For each query $(k, u)$: update leaf $k$, update parent nodes bottom-up.
  3. Output root node `tree[1].ans`.
- **Pseudocode**:
```text
function merge(L, R):
    res.sum = L.sum + R.sum
    res.pref = max(L.pref, L.sum + R.pref)
    res.suff = max(R.suff, R.sum + L.suff)
    res.ans = max({L.ans, R.ans, L.suff + R.pref})
    return res
```
- **Complexity**: Time: $\mathcal{O}((N + Q) \log N)$, Space: $\mathcal{O}(N)$.

---

## 13. Subarray Sum Queries II

- **Tier**: Tier 3
- **Link**: [Subarray Sum Queries II](https://cses.fi/problemset/task/3226)
- **Problem Statement**: Given a static array of $N$ integers, process $Q$ queries asking for maximum subarray sum in arbitrary subsegment $[l, r]$.
- **Interview Pattern**: Segment Tree Kadane's Range Query.
- **Intuition**:
  - Use the exact same Kadane node struct `{sum, pref, suff, ans}` as Subarray Sum Queries, but evaluate over arbitrary range $[l, r]$.
- **Step-by-Step Interview Walkthrough**:
  1. Build Segment Tree with Kadane node struct.
  2. Range query $[l, r]$: combine covered node structs using `merge(L, R)`.
  3. Return `result.ans`.
- **Pseudocode**:
```text
function query(node, start, end, l, r):
    if l <= start and end <= r:
        return tree[node]
    mid = (start + end) / 2
    if r <= mid:
        return query(2 * node, start, mid, l, r)
    if l > mid:
        return query(2 * node + 1, mid + 1, end, l, r)
    left_node = query(2 * node, start, mid, l, r)
    right_node = query(2 * node + 1, mid + 1, end, l, r)
    return merge(left_node, right_node)
```
- **Complexity**: Time: $\mathcal{O}((N + Q) \log N)$, Space: $\mathcal{O}(N)$.

---

## 14. Distinct Values Queries

- **Tier**: Tier 3
- **Link**: [Distinct Values Queries](https://cses.fi/problemset/task/1734)
- **Problem Statement**: Given an array of $N$ integers, process $Q$ static range queries asking for the number of distinct values in subarray $[l, r]$.
- **Interview Pattern**: Offline Queries + Fenwick Tree + Last Occurrence Sweep.
- **Intuition**:
  - Group queries by their right endpoint $r$.
  - Sweep index $i$ from 0 to $N-1$:
    - If $A[i]$ was seen before at index $p$, remove $p$ from BIT (`BIT.add(p, -1)`).
    - Add current index $i$ to BIT (`BIT.add(i, +1)`).
    - Update `last[A[i]] = i`.
    - Answer queries ending at $r = i$: `ans = BIT.query(r) - BIT.query(l-1)`.
- **Step-by-Step Interview Walkthrough**:
  1. Sort/Group queries by right endpoint $r$.
  2. Sweep $i$ from 0 to $N-1$:
     - If `last.count(A[i])`: `BIT.add(last[A[i]], -1)`.
     - `BIT.add(i, +1)`, `last[A[i]] = i`.
     - For all queries ending at $i$: `ans[id] = BIT.query_range(l, r)`.
  3. Print recorded answers in original order.
- **Pseudocode**:
```text
for i = 0 to N-1:
    if A[i] in last_seen:
        BIT.add(last_seen[A[i]], -1)
    BIT.add(i, +1)
    last_seen[A[i]] = i
    for query (l, r, id) ending at i:
        ans[id] = BIT.query_range(l, r)
```
- **Complexity**: Time: $\mathcal{O}((N + Q) \log N)$, Space: $\mathcal{O}(N + Q)$.
