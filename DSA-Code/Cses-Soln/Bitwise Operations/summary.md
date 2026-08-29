# CSES Bitwise Operations - Self-Contained Interview Cheat Sheet

This document contains **problem statements, interview-style explanations, intuitive breakdowns, step-by-step algorithms, structured pseudocode, and complexity analysis** for all 5 problems in the **Bitwise Operations** topic.

---

## 1. Counting Bits

- **Link**: [Counting Bits](https://cses.fi/problemset/task/1146)
- **Problem Statement**: Calculate the total number of 1-bits in the binary representations of all integers from 1 to $N$. ($N \le 10^{15}$).
- **Interview Pattern**: Bit Position Contribution Analysis / Digit DP.
- **Intuition**:
  - Analyze the contribution of bit position $k$ ($2^k$) across numbers $1 \dots N$.
  - Bit $k$ alternates between 0 and 1 in blocks of $2^k$ size.
  - Number of full pairs of $(0, 1)$ blocks = $\lfloor (N + 1) / 2^{k+1} \rfloor \times 2^k$.
  - Remaining partial block = $\max(0LL, (N + 1) \bmod 2^{k+1} - 2^k)$.
- **Step-by-Step Interview Walkthrough**:
  1. Initialize `total_bits = 0`.
  2. For bit position $k$ from 0 up to 60:
     - Full blocks contribution: `(N + 1) / (1 << (k + 1)) * (1 << k)`.
     - Partial block contribution: `max(0LL, (N + 1) % (1 << (k + 1)) - (1 << k))`.
     - Add both to `total_bits`.
  3. Return `total_bits`.
- **Pseudocode**:
```text
total_bits = 0
for k = 0 to 60:
    blockSize = 1 << (k + 1)
    fullBlocks = (N + 1) / blockSize
    total_bits += fullBlocks * (1 << k)
    remainder = (N + 1) % blockSize
    total_bits += max(0, remainder - (1 << k))
return total_bits
```
- **Complexity**: Time: $\mathcal{O}(\log N)$, Space: $\mathcal{O}(1)$.

---

## 2. Maximum Xor Subarray

- **Link**: [Maximum Xor Subarray](https://cses.fi/problemset/task/1655)
- **Problem Statement**: Given an array of $N$ integers, find the maximum XOR sum of any contiguous subarray.
- **Interview Pattern**: Trie (Prefix Tree) for Maximum XOR Pair.
- **Intuition**:
  - Subarray XOR $A[l..r] = P[r] \oplus P[l-1]$ where $P$ is prefix XOR array.
  - Problem reduces to: for each prefix XOR $P[r]$, find previously seen prefix XOR $P[l-1]$ that maximizes $P[r] \oplus P[l-1]$.
  - Insert binary representations of prefix XORs into a Binary Trie. To maximize XOR, query Trie by greedily choosing opposite bit at each step.
- **Step-by-Step Interview Walkthrough**:
  1. Insert prefix XOR 0 into Binary Trie.
  2. Maintain `curr_xor = 0` and `max_xor = 0`.
  3. For each element $x \in A$:
     - `curr_xor ^= x`.
     - Query Trie with `curr_xor` to find best match: at bit position $b$, prefer branch `1 - bit`.
     - `max_xor = max(max_xor, curr_xor ^ best_match)`.
     - Insert `curr_xor` into Trie.
  4. Return `max_xor`.
- **Pseudocode**:
```text
trie.insert(0)
curr_xor = 0, max_xor = 0

for x in A:
    curr_xor ^= x
    best_match = trie.get_max_xor_match(curr_xor)
    max_xor = max(max_xor, curr_xor ^ best_match)
    trie.insert(curr_xor)

return max_xor
```
- **Complexity**: Time: $\mathcal{O}(N \log(\max A))$, Space: $\mathcal{O}(N \log(\max A))$.

---

## 3. Maximum Xor Subset

- **Link**: [Maximum Xor Subset](https://cses.fi/problemset/task/2134)
- **Problem Statement**: Given an array of $N$ positive integers, find the maximum possible XOR sum of any subset.
- **Interview Pattern**: Linear Basis / Gaussian Elimination over GF(2).
- **Intuition**:
  - Any subset XOR sum belongs to the vector space spanned by the elements over GF(2).
  - Construct a Basis of linear independent vectors of size $\le 30$.
  - Greedily form the maximum XOR sum by iterating basis vectors from most significant bit to least significant bit.
- **Step-by-Step Interview Walkthrough**:
  1. Maintain basis array `basis[0..29] = 0`.
  2. For each element $x \in A$:
     - Insert $x$ into basis: iterate bit $b$ from 29 down to 0. If bit $b$ of $x$ is set:
       - If `basis[b] == 0`: set `basis[b] = x` and break.
       - Else: `x ^= basis[b]`.
  3. Form max XOR: `max_val = 0`. For bit $b$ from 29 down to 0: if `(max_val ^ basis[b]) > max_val`, update `max_val ^= basis[b]`.
  4. Return `max_val`.
- **Pseudocode**:
```text
function insert(x):
    for b = 29 down to 0:
        if (x & (1 << b)):
            if basis[b] == 0: basis[b] = x; return
            x ^= basis[b]

for x in A: insert(x)

max_xor = 0
for b = 29 down to 0:
    if (max_xor ^ basis[b]) > max_xor:
        max_xor ^= basis[b]
return max_xor
```
- **Complexity**: Time: $\mathcal{O}(N \log(\max A))$, Space: $\mathcal{O}(\log(\max A))$.

---

## 4. Number of Subset Xors

- **Link**: [Number of Subset Xors](https://cses.fi/problemset/task/2135)
- **Problem Statement**: Given an array of $N$ integers, count the number of distinct values that can be formed as the XOR sum of any subset.
- **Interview Pattern**: Vector Space Dimension via Linear Basis over GF(2).
- **Intuition**:
  - The set of all achievable subset XOR values forms a vector space over GF(2).
  - If the dimension of the Linear Basis is $K$ (number of non-zero basis elements), then the total number of distinct XOR sums is $2^K$.
- **Step-by-Step Interview Walkthrough**:
  1. Insert all elements into a Linear Basis of bit size 30.
  2. Count non-zero basis elements $K$.
  3. Output $2^K$.
- **Pseudocode**:
```text
basis = [0] * 30
for x in A: insert_into_basis(x)
K = count_non_zero_elements(basis)
return 1 << K
```
- **Complexity**: Time: $\mathcal{O}(N \log(\max A))$, Space: $\mathcal{O}(\log(\max A))$.

---

## 5. SOS Bit Problem

- **Link**: [SOS Bit Problem](https://cses.fi/problemset/task/2136)
- **Problem Statement**: Given an array of $N$ integers, for each element $A[i]$, calculate: (1) count of elements $x$ such that $x \mid A[i] = A[i]$ (subsets), (2) count of elements $x$ such that $x \ \& \ A[i] = A[i]$ (supersets), (3) count of elements $x$ such that $x \ \& \ A[i] \ne 0$.
- **Interview Pattern**: Sum Over Subsets (SOS) Dynamic Programming / Yates' Algorithm.
- **Intuition**:
  - SOS DP computes $F[\text{mask}] = \sum_{\text{sub} \subset \text{mask}} A[\text{sub}]$ in $\mathcal{O}(K \cdot 2^K)$ time instead of $\mathcal{O}(3^K)$.
  - Subsets: standard SOS DP.
  - Supersets: reversed SOS DP (populating superset bitmask).
  - Non-disjoint ($x \ \& \ A[i] \neq 0$): $N - (\text{count of elements } x \text{ disjoint from } A[i]) = N - \text{subsets}(\sim A[i])$.
- **Step-by-Step Interview Walkthrough**:
  1. Precompute `F_sub[mask]` = frequency count of exact bitmasks.
  2. SOS DP for Subsets:
     - For bit $i = 0 \dots 19$: for mask $= 0 \dots 2^{20}-1$: if bit $i$ of mask is set, `F_sub[mask] += F_sub[mask ^ (1<<i)]`.
  3. SOS DP for Supersets:
     - For bit $i = 0 \dots 19$: for mask $= 2^{20}-1$ down to 0: if bit $i$ of mask is NOT set, `F_super[mask] += F_super[mask | (1<<i)]`.
  4. For each element $A[k]$:
     - Subsets count = `F_sub[A[k]]`.
     - Supersets count = `F_super[A[k]]`.
     - Non-disjoint count = $N - \text{F\_sub}[(\sim A[k]) \ \& \ (2^{20}-1)]$.
- **Pseudocode**:
```text
// SOS Subset DP
for i = 0 to 19:
    for mask = 0 to (1<<20)-1:
        if (mask & (1 << i)): F_sub[mask] += F_sub[mask ^ (1 << i)]

// SOS Superset DP
for i = 0 to 19:
    for mask = (1<<20)-1 down to 0:
        if not (mask & (1 << i)): F_super[mask] += F_super[mask | (1 << i)]

for x in A:
    c1 = F_sub[x]
    c2 = F_super[x]
    c3 = N - F_sub[(~x) & MASK_MAX]
    print c1, c2, c3
```
- **Complexity**: Time: $\mathcal{O}(K \cdot 2^K + N)$ where $K=20$, Space: $\mathcal{O}(2^K)$.
