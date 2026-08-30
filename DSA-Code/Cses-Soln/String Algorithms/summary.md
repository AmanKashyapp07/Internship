# CSES String Algorithms - Self-Contained Interview Cheat Sheet

This document contains **problem statements, interview-style explanations, intuitive breakdowns, step-by-step algorithms, structured pseudocode, and complexity analysis** for all 11 problems in the **String Algorithms** topic.

---

## 1. String Matching

- **Tier**: Tier 1
- **Link**: [String Matching](https://cses.fi/problemset/task/1753)
- **Problem Statement**: Given a string $S$ of length $N$ and a pattern $P$ of length $M$, count the number of occurrences of pattern $P$ as a substring in $S$.
- **Interview Pattern**: KMP (Knuth-Morris-Pratt) / Z-Algorithm / String Hashing.
- **Intuition**:
  - Build KMP prefix function $\pi$ on string $P + \# + S$.
  - Count how many positions $i$ in $S$ have prefix function value $\pi[i] == M$.
- **Step-by-Step Interview Walkthrough**:
  1. Construct combined string $T = P + \# + S$.
  2. Compute KMP $\pi$ array: $\pi[i]$ stores length of longest proper prefix that is also a suffix of $T[0..i]$.
  3. Count indices $i \ge M + 1$ where $\pi[i] == M$.
- **Pseudocode**:
```text
T = P + "#" + S
pi = compute_kmp_pi(T)
ans = count(pi[i] == M for i in M+1..T.length-1)
return ans
```
- **Complexity**: Time: $\mathcal{O}(N + M)$, Space: $\mathcal{O}(N + M)$.

---

## 2. Finding Borders

- **Tier**: Tier 2
- **Link**: [Finding Borders](https://cses.fi/problemset/task/1732)
- **Problem Statement**: Given a string $S$, find all lengths $L > 0$ such that the prefix of length $L$ equals the suffix of length $L$ (excluding the full string length $N$).
- **Interview Pattern**: KMP Prefix Function Iteration / Polynomial Rolling Hash.
- **Intuition**:
  - The longest proper border of $S$ is given by $L_1 = \pi[N-1]$.
  - Next border is $\pi[L_1 - 1]$, and so on, following chain $\pi[\pi[\dots]-1]$ until 0.
- **Step-by-Step Interview Walkthrough**:
  1. Compute KMP $\pi$ array for string $S$.
  2. Jump back using $curr = \pi[N-1]$. While $curr > 0$:
     - Push $curr$ to border list.
     - $curr = \pi[curr - 1]$.
  3. Reverse border list and output.
- **Pseudocode**:
```text
pi = compute_kmp_pi(S)
borders = []
curr = pi[N - 1]
while curr > 0:
    borders.push(curr)
    curr = pi[curr - 1]

reverse(borders)
return borders
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 3. Finding Periods

- **Tier**: Tier 2
- **Link**: [Finding Periods](https://cses.fi/problemset/task/1733)
- **Problem Statement**: A string $S$ has period $L$ if $S[i] = S[i + L]$ for all valid indices. Find all possible period lengths $L > 0$ of string $S$.
- **Interview Pattern**: Z-Algorithm / String Hashing.
- **Intuition**:
  - Using Z-algorithm, $Z[i]$ is the length of the longest common prefix between $S$ and $S[i..N-1]$.
  - Length $L = i$ is a valid period iff $i + Z[i] == N$.
- **Step-by-Step Interview Walkthrough**:
  1. Compute Z-array for string $S$: $Z[i] = \text{LCP}(S, S[i \dots N-1])$.
  2. For candidate period $L$ from 1 to $N$:
     - If $L + Z[L] == N$, output $L$.
- **Pseudocode**:
```text
Z = compute_z_array(S)
for L = 1 to N-1:
    if L + Z[L] == N: print L
print N
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 4. Minimal Rotation

- **Tier**: Tier 2
- **Link**: [Minimal Rotation](https://cses.fi/problemset/task/1110)
- **Problem Statement**: Given a string $S$, find its lexicographically smallest cyclic shift (rotation).
- **Interview Pattern**: Booth's Algorithm ($\mathcal{O}(N)$ Lexicographical Minimum Rotation).
- **Intuition**:
  - Booth's algorithm uses a failure function similar to KMP on duplicated string $S + S$ to find the start index of the lexicographically smallest rotation in linear time.
- **Step-by-Step Interview Walkthrough**:
  1. Concatenate $T = S + S$.
  2. Maintain candidate indices $i = 0, j = 1, k = 0$.
  3. Compare $T[i+k]$ and $T[j+k]$:
     - If equal, $k \mathrel{+}= 1$.
     - If $T[i+k] > T[j+k]$, $i = \max(i + k + 1, j + 1)$, reset $k = 0$.
     - If $T[i+k] < T[j+k]$, $j = \max(j + k + 1, i + 1)$, reset $k = 0$.
  4. Return $S[i \dots i + N - 1]$.
- **Pseudocode**:
```text
T = S + S
i = 0, j = 1, k = 0
while i < N and j < N and k < N:
    if T[i+k] == T[j+k]: k += 1
    else if T[i+k] > T[j+k]: i = max(i + k + 1, j + 1); k = 0
    else: j = max(j + k + 1, i + 1); k = 0

best_start = min(i, j)
return T[best_start .. best_start + N - 1]
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 5. Longest Palindrome

- **Tier**: Tier 2
- **Link**: [Longest Palindrome](https://cses.fi/problemset/task/1111)
- **Problem Statement**: Given a string $S$, find the longest palindromic substring in $S$.
- **Interview Pattern**: Manacher's Algorithm ($\mathcal{O}(N)$ Palindrome Centers).
- **Intuition**:
  - Insert `#` between characters to unify odd and even length palindromes.
  - Manacher's algorithm maintains center $C$ and right boundary $R$ of the rightmost palindrome to expand centers in $\mathcal{O}(1)$ amortized time.
- **Step-by-Step Interview Walkthrough**:
  1. Transform string $S \to T$ (e.g. `^#a#b#c#$`).
  2. Compute radius array $P[i]$ using Manacher's algorithm.
  3. Find center $i$ with maximum radius $P[i]$.
  4. Extract and print corresponding substring from original $S$.
- **Pseudocode**:
```text
T = transform_with_delimiters(S)
P = compute_manacher_radii(T)
best_center = argmax(P)
return extract_original_substring(S, best_center, P[best_center])
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 6. Repeating Substring

- **Tier**: Tier 3
- **Link**: [Repeating Substring](https://cses.fi/problemset/task/2106)
- **Problem Statement**: Given a string $S$, find the longest substring that appears at least twice as a substring in $S$, or output -1 if none.
- **Interview Pattern**: Suffix Array + LCP (Longest Common Prefix) Array.
- **Intuition**:
  - Construct Suffix Array $SA$ and LCP array $LCP$.
  - The longest repeating substring corresponds to the maximum value in the $LCP$ array.
- **Step-by-Step Interview Walkthrough**:
  1. Construct Suffix Array $SA$ for string $S$.
  2. Construct LCP array $LCP$ using Kasai's algorithm.
  3. Find index $i$ that maximizes $LCP[i]$.
  4. Return substring of length $LCP[i]$ starting at $SA[i]$.
- **Pseudocode**:
```text
SA = build_suffix_array(S)
LCP = build_lcp_kasai(S, SA)
best_idx = argmax(LCP)
if LCP[best_idx] == 0: return -1
return S[SA[best_idx] .. SA[best_idx] + LCP[best_idx] - 1]
```
- **Complexity**: Time: $\mathcal{O}(N \log N)$, Space: $\mathcal{O}(N)$.

---

## 7. All Palindromes

- **Tier**: Tier 2
- **Link**: [All Palindromes](https://cses.fi/problemset/task/2110)
- **Problem Statement**: Given a string $S$, count the total number of distinct palindromic substrings.
- **Interview Pattern**: Palindromic Tree (EERTREE) / Suffix Automaton + Manacher.
- **Intuition**:
  - Each character added to a string adds at most 1 new distinct palindromic substring (the longest palindromic suffix).
  - Palindromic Tree (EERTREE) maintains all unique palindromes as nodes in a DAG tree structure.
- **Step-by-Step Interview Walkthrough**:
  1. Initialize EERTREE with 2 imaginary root nodes: length -1 (odd root) and length 0 (even root).
  2. Process each character of $S$, walk suffix links to insert new palindromic nodes if not present.
  3. Total distinct palindromes = `tree.node_count - 2`.
- **Pseudocode**:
```text
eertree.init()
for char c in S:
    eertree.add_char(c)
return eertree.num_nodes - 2
```
- **Complexity**: Time: $\mathcal{O}(N \Sigma)$, Space: $\mathcal{O}(N \Sigma)$.

---

## 8. Word Combinations

- **Tier**: Tier 2
- **Link**: [Word Combinations](https://cses.fi/problemset/task/1731)
- **Problem Statement**: Given a target string $S$ of length $N$ and a dictionary of $K$ words, count the number of ways to construct $S$ by concatenating words from the dictionary modulo $10^9+7$.
- **Interview Pattern**: Trie-Optimized 1D Dynamic Programming.
- **Intuition**:
  - `dp[i]` = ways to construct prefix $S[0..i-1]$.
  - Store all dictionary words in a Trie. When at index $i$, walk the Trie using characters $S[i \dots N-1]$ to match dictionary words in $\mathcal{O}(\text{depth})$ time.
- **Step-by-Step Interview Walkthrough**:
  1. Insert all $K$ dictionary words into a Trie.
  2. Set `dp[0] = 1`.
  3. For $i = 0 \dots N-1$: if `dp[i] > 0`:
     - Walk Trie starting from node root with characters $S[i], S[i+1], \dots$.
     - If current Trie node is a word end at character $S[j]$, update `dp[j+1] = (dp[j+1] + dp[i]) % MOD`.
  4. Return `dp[N]`.
- **Pseudocode**:
```text
trie.insert_all(dictionary_words)
dp[0] = 1

for i = 0 to N-1:
    if dp[i] == 0: continue
    curr_node = trie.root
    for j = i to N-1:
        if S[j] not in curr_node.children: break
        curr_node = curr_node.children[S[j]]
        if curr_node.is_word:
            dp[j + 1] = (dp[j + 1] + dp[i]) % MOD

return dp[N]
```
- **Complexity**: Time: $\mathcal{O}(N \cdot \text{max\_word\_len} + \sum |W_i|)$, Space: $\mathcal{O}(\sum |W_i| + N)$.

---

## 9. String Functions

- **Tier**: Tier 2
- **Link**: [String Functions](https://cses.fi/problemset/task/2107)
- **Problem Statement**: Given a string $S$ of length $N$, compute both the KMP Prefix Function $\pi$ and the Z-Algorithm Array $Z$.
- **Interview Pattern**: KMP $\pi$-Array + Z-Array Algorithms.
- **Intuition**:
  - Compute standard $Z$-array matching LCP of $S[0..N-1]$ and $S[i..N-1]$.
  - Compute standard KMP $\pi$-array matching longest proper prefix-suffix.
- **Step-by-Step Interview Walkthrough**:
  1. Compute Z-array: maintain window $[L, R]$ of furthest match.
  2. Compute $\pi$-array: maintain border candidate length $j = \pi[i-1]$.
  3. Print $Z[0 \dots N-1]$ and $\pi[0 \dots N-1]$.
- **Pseudocode**:
```text
Z = compute_z_array(S)
pi = compute_kmp_pi(S)
print Z
print pi
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 10. Palindrome Queries

- **Tier**: Tier 3
- **Link**: [Palindrome Queries](https://cses.fi/problemset/task/2420)
- **Problem Statement**: Given a string $S$, process $Q$ queries: (1) update character at index $k$ to $c$, (2) check if substring $S[l \dots r]$ is a palindrome.
- **Interview Pattern**: Dual Segment Trees / Fenwick Trees with Polynomial Rolling Hash.
- **Intuition**:
  - Substring $S[l..r]$ is a palindrome iff its forward polynomial hash equals its backward polynomial hash!
  - Point updates and range hash queries are maintained using two Fenwick Trees / Segment Trees (one forward, one backward).
- **Step-by-Step Interview Walkthrough**:
  1. Build forward BIT (weights $B^i$) and backward BIT (weights $B^{N-1-i}$).
  2. Type 1 update $(k, c)$: update forward and backward BITs with diff $(c - S[k]) \times B^\text{power}$.
  3. Type 2 query $[l, r]$: compute forward hash $H_f$ and backward hash $H_b$. Compare $H_f == H_b$ (adjusting base powers).
- **Pseudocode**:
```text
forward_BIT.update(k, diff * B^k)
backward_BIT.update(k, diff * B^(N-1-k))

function is_palindrome(l, r):
    h_f = forward_BIT.query_range(l, r) * B^(N - r)
    h_b = backward_BIT.query_range(l, r) * B^(l + 1)
    return h_f == h_b
```
- **Complexity**: Time: $\mathcal{O}((N + Q) \log N)$, Space: $\mathcal{O}(N)$.

---

## 11. Distinct Subsequences

- **Tier**: Tier 3
- **Link**: [Distinct Subsequences](https://cses.fi/problemset/task/2108)
- **Problem Statement**: Given a string $S$, count the total number of distinct subsequences modulo $10^9+7$.
- **Interview Pattern**: Dynamic Programming with Last Occurrence Tracking.
- **Intuition**:
  - `dp[i]` = number of distinct subsequences using prefix $S[0..i-1]$.
  - Recurrence: $\text{dp}[i] = 2 \times \text{dp}[i-1] - \text{dp}[\text{last}[S[i-1]] - 1]$.
- **Step-by-Step Interview Walkthrough**:
  1. Initialize `dp[0] = 1`, `last[0..25] = -1`.
  2. For $i = 1 \dots N$:
     - `dp[i] = (2 * dp[i-1]) % MOD`.
     - If character $S[i-1]$ was seen at index $p = \text{last}[S[i-1]]$:
       `dp[i] = (dp[i] - dp[p - 1] + MOD) % MOD`.
     - Update `last[S[i-1]] = i`.
  3. Return `(dp[N] - 1 + MOD) % MOD` (excluding empty subsequence).
- **Pseudocode**:
```text
dp[0] = 1
last[a..z] = -1

for i = 1 to N:
    char_code = S[i-1] - 'a'
    dp[i] = 2 * dp[i-1] % MOD
    if last[char_code] != -1:
        dp[i] = (dp[i] - dp[last[char_code] - 1] + MOD) % MOD
    last[char_code] = i

return (dp[N] - 1 + MOD) % MOD
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N + \Sigma)$.
