# CSES Additional Problems I - Self-Contained Interview Cheat Sheet

This document contains **problem statements, interview-style explanations, intuitive breakdowns, step-by-step algorithms, structured pseudocode, and complexity analysis** for all 16 problems in the **Additional Problems I** topic.

---

## 1. Shortest Subsequence

- **Link**: [Shortest Subsequence](https://cses.fi/problemset/task/1087)
- **Problem Statement**: Given a DNA sequence of characters ('A', 'C', 'G', 'T'), find the shortest DNA sequence that is **not** a subsequence of the given string.
- **Interview Pattern**: Greedy Character-Set Coverage (Alphabet Phase Completion).
- **Intuition**:
  - Divide the string into consecutive blocks such that each block contains all 4 DNA characters ('A', 'C', 'G', 'T').
  - The minimum length of a non-subsequence must be (number of full 4-character blocks + 1). We greedily pick the character that completes each block.
- **Step-by-Step Interview Walkthrough**:
  1. Maintain a set of seen characters in the current block.
  2. Iterate through the string character by character. Add each character to the set.
  3. When `set.size() == 4`, append the current character to `result`, clear the set, and start a new block.
  4. After finishing the loop, pick any character missing from the last incomplete set and append it to `result`.
- **Pseudocode**:
```text
set seen = {}
result = ""
for char c in s:
    seen.insert(c)
    if seen.size() == 4:
        result += c
        seen.clear()

for char c in ['A', 'C', 'G', 'T']:
    if c not in seen:
        result += c
        break
return result
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(1)$ auxiliary.

---

## 2. Advertisement

- **Link**: [Advertisement](https://cses.fi/problemset/task/1142)
- **Problem Statement**: Given $N$ histogram bars of width 1 and specified heights, find the area of the largest rectangle that can be formed within the histogram.
- **Interview Pattern**: Monotonic Stack (Largest Rectangle in Histogram).
- **Intuition**:
  - For each bar $i$ with height $h[i]$, the rectangle with height $h[i]$ can extend left to the first smaller bar on the left and right to the first smaller bar on the right.
  - A monotonic increasing stack maintains candidate bar indices. Popping a taller bar when a shorter one is encountered determines its exact maximum width span.
- **Step-by-Step Interview Walkthrough**:
  1. Append a sentinel bar of height 0 at index $N$.
  2. Iterate $i$ from 0 to $N$.
  3. While stack is not empty and $h[\text{stack.top()}] \ge h[i]$:
     - Pop height $H = h[\text{stack.top()}]$.
     - Calculate width $W = i$ if stack is empty, else $i - \text{stack.top()} - 1$.
     - Update $\text{max\_area} = \max(\text{max\_area}, H \times W)$.
  4. Push $i$ to stack.
- **Pseudocode**:
```text
h.push_back(0)
stack st = {}
max_area = 0
for i = 0 to N:
    while not st.empty() and h[st.top()] >= h[i]:
        height = h[st.top()]
        st.pop()
        width = i if st.empty() else (i - st.top() - 1)
        max_area = max(max_area, height * width)
    st.push(i)
return max_area
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 3. Bubble Sort Rounds I

- **Link**: [Bubble Sort Rounds I](https://cses.fi/problemset/task/1147)
- **Problem Statement**: Given an array of $N$ numbers, calculate how many rounds of Bubble Sort pass are required until the array is fully sorted.
- **Interview Pattern**: Maximum Displacement of Inversions.
- **Intuition**:
  - In each round of Bubble Sort, every element moves at most 1 position to the left if there are smaller elements to its right.
  - Thus, the total number of Bubble Sort rounds required equals $\max_{i} (\text{initial\_index}[i] - \text{sorted\_index}[i], 0) + 1$.
- **Step-by-Step Interview Walkthrough**:
  1. Pair each element with its initial index $(A[i], i)$ and sort the pairs.
  2. For each element at sorted position $j$ coming from initial index $i$:
     - Compute leftward displacement $i - j$.
  3. The answer is $\max(0, \max_{j} (i - j)) + 1$ (if already sorted, 0 rounds).
- **Pseudocode**:
```text
pairs = [(A[i], i) for i in 0..N-1]
sort(pairs)

max_left_shift = 0
for j = 0 to N-1:
    orig_i = pairs[j].second
    max_left_shift = max(max_left_shift, orig_i - j)

return max_left_shift
```
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(N)$.

---

## 4. Multiplication Table

- **Link**: [Multiplication Table](https://cses.fi/problemset/task/2422)
- **Problem Statement**: Given an $N \times N$ multiplication table where cell $(i, j)$ contains $i \times j$, find the median element (the element at position $\lfloor N^2 / 2 \rfloor + 1$ in sorted order).
- **Interview Pattern**: Binary Search on Answer / Counting Multiples.
- **Intuition**:
  - The values in the table range from $1$ to $N^2$.
  - For a candidate value $X$, the number of elements in the table $\le X$ is $\sum_{i=1}^{N} \min(N, \lfloor X / i \rfloor)$.
  - Binary search for the smallest $X$ such that at least $\lfloor N^2 / 2 \rfloor + 1$ elements are $\le X$.
- **Step-by-Step Interview Walkthrough**:
  1. Set binary search bounds $\text{low} = 1, \text{high} = N^2$, $\text{target} = (N^2 + 1) / 2$.
  2. For mid point $M = (\text{low} + \text{high}) / 2$:
     - Count elements $\le M$: `cnt = sum(min(N, M / i) for i in 1..N)`.
     - If `cnt >= target`: record answer and try smaller values (`high = M - 1`).
     - Else: try larger values (`low = M + 1`).
- **Pseudocode**:
```text
low = 1, high = N * N, target = (N * N + 1) / 2
ans = high
while low <= high:
    mid = (low + high) / 2
    cnt = 0
    for i = 1 to N:
        cnt += min(N, mid / i)
    if cnt >= target:
        ans = mid
        high = mid - 1
    else:
        low = mid + 1
return ans
```
- **Complexity**: Time: $\mathcal{O}(N \log(N^2))$, Space: $\mathcal{O}(1)$.

---

## 5. Bit Inversions

- **Link**: [Bit Inversions](https://cses.fi/problemset/task/1188)
- **Problem Statement**: Given a bit string of length $N$ and $M$ queries (each flipping a single bit), output the length of the longest contiguous substring of identical bits after each flip.
- **Interview Pattern**: Segment Tree with Subsegment Merging / `std::set` Boundary Maintenance.
- **Intuition**:
  - Maintain the lengths of contiguous blocks of same bits using a Segment Tree or a set of boundary indices.
  - In a Segment Tree, each node stores: `pref` (longest prefix of same bit), `suff` (longest suffix of same bit), `max_len` (longest continuous block), and total length.
- **Step-by-Step Interview Walkthrough**:
  1. Build a Segment Tree over the bit string.
  2. Each node merges left and right children:
     - `max_len = max(left.max_len, right.max_len)`
     - If left child is uniform and matches right child's prefix, combine across boundary: `max_len = max(max_len, left.suff + right.pref)`.
  3. For each flip query at index $i$: flip bit at $i$, update Segment Tree in $\mathcal{O}(\log N)$, and print `root.max_len`.
- **Pseudocode**:
```text
function merge(L, R):
    res.max_len = max(L.max_len, R.max_len)
    if s[L.right_idx] == s[R.left_idx]:
        res.max_len = max(res.max_len, L.suff + R.pref)
    // update pref and suff accordingly
    return res
```
- **Complexity**: Time: $\mathcal{O}((N + M) \log N)$, Space: $\mathcal{O}(N)$.

---

## 6. Distinct Values Sum

- **Link**: [Distinct Values Sum](https://cses.fi/problemset/task/2084)
- **Problem Statement**: Given an array of $N$ numbers, calculate the sum of the number of distinct values in all possible subarrays $[l, r]$.
- **Interview Pattern**: Contribution Technique / Last Occurrence Indexing.
- **Intuition**:
  - Instead of computing distinct count per subarray, compute the **contribution** of each element $A[i]$ across all subarrays where it is the *first* occurrence of that value.
  - Element $A[i]$ is the first occurrence of its value in any subarray starting at $l$ and ending at $r$ iff $\text{last\_seen}[A[i]] < l \le i$ and $i \le r \le N-1$.
- **Step-by-Step Interview Walkthrough**:
  1. Maintain map `last_seen[val]` storing 0-based index of previous occurrence of `val` (default -1).
  2. For index $i$ from 0 to $N-1$:
     - Number of valid left endpoints: $i - \text{last\_seen}[A[i]]$.
     - Number of valid right endpoints: $N - i$.
     - Contribution to total sum = $(i - \text{last\_seen}[A[i]]) \times (N - i)$.
  3. Accumulate total sum over all $i$.
- **Pseudocode**:
```text
total_sum = 0
last_seen = map() // val -> index

for i = 0 to N-1:
    prev = last_seen.get(A[i], -1)
    left_choices = i - prev
    right_choices = N - i
    total_sum += left_choices * right_choices
    last_seen[A[i]] = i

return total_sum
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 7. Swap Game

- **Link**: [Swap Game](https://cses.fi/problemset/task/1670)
- **Problem Statement**: Given a $3 \times 3$ grid containing numbers 1 to 9, find the minimum number of adjacent cell swaps required to reach the target grid (row-major order 1 to 9).
- **Interview Pattern**: BFS State-Space Search / Permutation Hash.
- **Intuition**:
  - Total grid states = $9! = 362,880$, which is very small!
  - Unweighted shortest path from initial state to target state via 12 possible adjacent swaps is solved via BFS.
- **Step-by-Step Interview Walkthrough**:
  1. Encode the $3 \times 3$ grid as a 9-character string or integer hash.
  2. Precompute the 12 allowable adjacent swap pairs in a $3 \times 3$ grid.
  3. Start BFS from the initial state string with distance 0.
  4. For popped state: try all 12 swaps to generate neighbor states.
  5. When target `"123456789"` is reached, return distance.
- **Pseudocode**:
```text
queue.push(initial_state)
dist[initial_state] = 0

while queue not empty:
    state = queue.pop()
    if state == "123456789": return dist[state]
    for (i, j) in valid_swap_pairs:
        next_state = swap_chars(state, i, j)
        if next_state not in dist:
            dist[next_state] = dist[state] + 1
            queue.push(next_state)
```
- **Complexity**: Time: $\mathcal{O}(9!)$ states, Space: $\mathcal{O}(9!)$.

---

## 8. Maximum Building I

- **Link**: [Maximum Building I](https://cses.fi/problemset/task/1147)
- **Problem Statement**: Given an $N \times M$ grid of forest trees (`*`) and empty spaces (`.`), find the maximum area of a rectangular building that can be built on empty spaces.
- **Interview Pattern**: 2D Grid Monotonic Stack (Histogram Expansion per Row).
- **Intuition**:
  - Convert 2D grid into $N$ 1D histogram problems.
  - Maintain `height[c]` = consecutive empty cells above row $r$ in column $c$.
  - Run 1D Largest Rectangle in Histogram algorithm on `height` for each row.
- **Step-by-Step Interview Walkthrough**:
  1. Initialize `height[0..M-1] = 0`.
  2. For each row $r$ from 0 to $N-1$:
     - For each column $c$: if `grid[r][c] == '.'`, `height[c] += 1`; else `height[c] = 0`.
     - Run 1D histogram max rectangle algorithm on `height` array using monotonic stack.
     - Update global `max_area`.
- **Pseudocode**:
```text
max_area = 0
heights = array of size M with 0s

for r = 0 to N-1:
    for c = 0 to M-1:
        if grid[r][c] == '.': heights[c] += 1
        else: heights[c] = 0
    max_area = max(max_area, max_rectangle_histogram(heights))
return max_area
```
- **Complexity**: Time: $\mathcal{O}(N \times M)$, Space: $\mathcal{O}(M)$.

---

## 9. Sorting Methods

- **Link**: [Sorting Methods](https://cses.fi/problemset/task/1162)
- **Problem Statement**: Calculate the minimum number of operations required to sort a permutation of $1 \dots N$ using 4 different operation types: (1) Swap any adjacent, (2) Swap any pair, (3) Move element to end, (4) Move element to front.
- **Interview Pattern**: Permutation Inversions, Cycle Decomposition, LIS, LNDS.
- **Intuition**:
  - Operation 1 (adjacent swaps) = Number of Inversions $\mathcal{O}(N \log N)$ via BIT.
  - Operation 2 (any pair swap) = $N - \text{count\_of\_permutation\_cycles}$.
  - Operation 3 (move to end) = $N - \text{length\_of\_Longest\_Increasing\_Subsequence\_starting\_with\_1}$.
  - Operation 4 (move to front) = $N - \text{length\_of\_suffix\_sequence\_(N, N-1, ...)}$.
- **Step-by-Step Interview Walkthrough**:
  1. Op 1: Count inversions using Fenwick Tree / Merge Sort.
  2. Op 2: Find cycle components in permutation graph; result = $N - C$.
  3. Op 3: Find longest subsegment of values $1, 2, 3, \dots$ appearing in relative order; result = $N - \text{length}$.
  4. Op 4: Find longest subsegment of values $\dots, N-1, N$ appearing in relative order; result = $N - \text{length}$.
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(N)$.

---

## 10. Cyclic Array

- **Link**: [Cyclic Array](https://cses.fi/problemset/task/1191)
- **Problem Statement**: Given a cyclic array of $N$ integers and a sum limit $K$, partition the cyclic array into minimum number of contiguous subarrays such that each subarray sum $\le K$.
- **Interview Pattern**: Binary Lifting on Functional Jump Graph (Doubling).
- **Intuition**:
  - Duplicate array to length $2N$ to handle cyclicity.
  - For each starting index $i$, find the maximal next starting index `next_idx[i]` using Two Pointers such that subarray sum $\le K$.
  - Precompute binary lifting table `up[i][j]` = index reached after $2^j$ subarray jumps.
- **Step-by-Step Interview Walkthrough**:
  1. Use Two Pointers on duplicated array $[1..2N]$ to find `next_idx[i]` for all $i \in [1, N]$.
  2. Precompute binary lifting `up[i][j]`.
  3. For each starting position $i \in [1, N]$:
     - Binary lift jumps until total distance covered $\ge N$.
     - Count minimum jumps needed.
  4. Return minimum jumps over all valid start positions $i$.
- **Pseudocode**:
```text
compute next_idx[i] via Two Pointers on 2N array
up[i][0] = next_idx[i]
build up[i][j] table for binary lifting

min_subarrays = INF
for start = 1 to N:
    curr = start, jumps = 0
    for j = LOG-1 down to 0:
        if up[curr][j] - start < N:
            curr = up[curr][j]
            jumps += (1 << j)
    min_subarrays = min(min_subarrays, jumps + 1)
return min_subarrays
```
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(N \log N)$.

---

## 11. Special Substrings

- **Link**: [Special Substrings](https://cses.fi/problemset/task/2186)
- **Problem Statement**: Given a string $S$, count the number of non-empty substrings in which all distinct characters present in $S$ appear with equal frequency.
- **Interview Pattern**: Difference Relative Frequency Map / Prefix Hash.
- **Intuition**:
  - Let $C$ be the number of distinct characters in $S$.
  - A substring $S[l..r]$ has equal counts of all $C$ characters iff the difference between frequencies of consecutive characters is identical at prefix $l-1$ and prefix $r$.
  - State vector at index $i$: $(cnt(c_2) - cnt(c_1), cnt(c_3) - cnt(c_1), \dots, cnt(c_C) - cnt(c_1))$.
- **Step-by-Step Interview Walkthrough**:
  1. Identify all distinct characters in $S$.
  2. Maintain running frequency array for each character.
  3. At each index $i$, construct a tuple representing $(cnt[j] - cnt[0])$ for all distinct characters $j > 0$.
  4. Use a hash map to store frequencies of these difference tuples.
  5. Add `map[tuple]` to total answer and increment `map[tuple]`.
- **Pseudocode**:
```text
tuple_count = { (0, 0, ...): 1 }
ans = 0
for char in S:
    freq[char] += 1
    tuple_state = (freq[c_1] - freq[c_0], freq[c_2] - freq[c_0], ...)
    ans += tuple_count[tuple_state]
    tuple_count[tuple_state] += 1
return ans
```
- **Complexity**: Time: $\mathcal{O}(N \cdot \Sigma)$, Space: $\mathcal{O}(N \cdot \Sigma)$ where $\Sigma$ is alphabet size.

---

## 12. Writing Numbers

- **Link**: [Writing Numbers](https://cses.fi/problemset/task/1086)
- **Problem Statement**: You have $N$ ones available. You write numbers $1, 2, 3, \dots$ in sequence. Find the maximum integer $X$ you can write before running out of ones.
- **Interview Pattern**: Binary Search on Answer + Digit DP / Digit Counting.
- **Intuition**:
  - The total number of ones needed to write all integers from 1 to $X$ is a monotonically increasing function $F(X)$.
  - Binary search for maximum $X$ such that $F(X) \le N$.
- **Step-by-Step Interview Walkthrough**:
  1. Define function $F(X)$ counting total digit '1's in range $[1, X]$.
     - $F(X) = \sum_{p=10^0}^{10^{18}} \left( \lfloor X / (10p) \rfloor \times p + \min(p, \max(0LL, X \bmod (10p) - p + 1)) \right)$.
  2. Binary search $X \in [1, 10^{18}]$.
  3. If $F(\text{mid}) \le N$: record answer and try larger $X$ (`low = mid + 1`). Else try smaller $X$ (`high = mid - 1`).
- **Pseudocode**:
```text
function count_ones(X):
    cnt = 0
    p = 1
    while p <= X:
        cnt += (X / (p * 10)) * p + min(p, max(0, X % (p * 10) - p + 1))
        p *= 10
    return cnt

low = 1, high = 1e18, ans = 0
while low <= high:
    mid = (low + high) / 2
    if count_ones(mid) <= N:
        ans = mid
        low = mid + 1
    else:
        high = mid - 1
return ans
```
- **Complexity**: Time: $\mathcal{O}(\log(10^{18}) \cdot \log_{10}(10^{18}))$, Space: $\mathcal{O}(1)$.

---

## 13. Permutation Subsequence

- **Link**: [Permutation Subsequence](https://cses.fi/problemset/task/2215)
- **Problem Statement**: Given a permutation of $1 \dots N$, find the length of the longest subsequence that can be formed such that elements are in strictly increasing order and values differ by 1.
- **Interview Pattern**: Index-Map Dynamic Programming.
- **Intuition**:
  - Store the 0-based position `pos[val]` of each value $1 \dots N$ in the input permutation.
  - A valid sequence $x, x+1, x+2, \dots$ requires `pos[x] < pos[x+1] < pos[x+2]`.
- **Step-by-Step Interview Walkthrough**:
  1. Record `pos[A[i]] = i` for all $i$.
  2. Iterate $x$ from 1 to $N-1$: if `pos[x] < pos[x+1]`, increment current sequence length. Else reset length to 1.
  3. Track maximum sequence length.
- **Pseudocode**:
```text
for i = 0 to N-1: pos[A[i]] = i
max_len = 1, curr_len = 1
for x = 1 to N-1:
    if pos[x] < pos[x+1]:
        curr_len += 1
    else:
        curr_len = 1
    max_len = max(max_len, curr_len)
return max_len
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 14. Subarray Sum Constraints

- **Link**: [Subarray Sum Constraints](https://cses.fi/problemset/task/2414)
- **Problem Statement**: Given subarray sum constraints, determine if there exists a non-negative array satisfying all constraints.
- **Interview Pattern**: Prefix Sum Graph / Constraint Graph 2-Coloring.
- **Intuition**:
  - Subarray sum $A[l..r] = S$ translates to prefix sum constraint $P[r] - P[l-1] = S$.
  - Represents system of linear equations on graph nodes $P[0 \dots N]$.
- **Step-by-Step Interview Walkthrough**:
  1. Model prefix sums $P[i]$ as graph nodes.
  2. Add weighted edge between $l-1$ and $r$ with value $S$.
  3. Check bipartite/component consistency via DFS: `P[v] = P[u] + weight`.
- **Complexity**: Time: $\mathcal{O}(N + M)$, Space: $\mathcal{O}(N + M)$.

---

## 15. Subsets with Fixed Average

- **Link**: [Subsets with Fixed Average](https://cses.fi/problemset/task/2425)
- **Problem Statement**: Count non-empty subsets of an array whose average equals $A$.
- **Interview Pattern**: Target Shifted Subset Sum DP.
- **Intuition**:
  - Subset average equals $A$ iff $\sum (x_i - A) = 0$.
  - Transform each array element $x_i \to x_i - A$ and count non-empty subsets summing to 0.
- **Step-by-Step Interview Walkthrough**:
  1. Shift all elements $B[i] = A[i] - K$.
  2. Run 0/1 Knapsack DP to count ways to achieve sum 0 using elements of $B$.
  3. Subtract 1 to exclude empty set.
- **Complexity**: Time: $\mathcal{O}(N \cdot \text{MaxSum})$, Space: $\mathcal{O}(\text{MaxSum})$.

---

## 16. Two Array Average

- **Link**: [Two Array Average](https://cses.fi/problemset/task/2426)
- **Problem Statement**: Given two arrays $A$ and $B$, select $K$ indices to maximize the ratio $\sum A_i / \sum B_i$.
- **Interview Pattern**: Binary Search on Ratio (Dinkelbach's Algorithm).
- **Intuition**:
  - Ratio $\ge \lambda$ iff $\sum (A_i - \lambda B_i) \ge 0$.
  - Binary search $\lambda$, compute transformed weights $W_i = A_i - \lambda B_i$, select top $K$ largest $W_i$.
- **Step-by-Step Interview Walkthrough**:
  1. Binary search ratio $\lambda \in [0, \max(A_i)]$.
  2. Evaluate $\text{mid}$: compute $W_i = A_i - \text{mid} \times B_i$.
  3. Sort $W_i$ descending and sum top $K$.
  4. If sum $\ge 0$, set $\text{low} = \text{mid}$; else $\text{high} = \text{mid}$.
- **Complexity**: Time: $\mathcal{O}(N \log N \cdot \log(\text{precision}))$, Space: $\mathcal{O}(N)$.
