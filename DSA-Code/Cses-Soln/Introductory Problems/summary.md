# CSES Introductory Problems - Self-Contained Interview Cheat Sheet

This document contains **problem statements, interview-style explanations, intuitive breakdowns, step-by-step algorithms, structured pseudocode, and complexity analysis** for all 5 problems in the **Introductory Problems** topic.

---

## 1. Trailing Zeros

- **Tier**: Tier 1
- **Link**: [Trailing Zeros](https://cses.fi/problemset/task/1618)
- **Problem Statement**: Calculate the number of trailing zeros in $N!$.
- **Interview Pattern**: Legendre's Formula for Prime Factor 5.
- **Intuition**: Trailing zeros are produced by pairs of $2 \times 5$. Count factors of 5 in $N!$: $\lfloor N/5 \rfloor + \lfloor N/25 \rfloor + \lfloor N/125 \rfloor + \dots$
- **Step-by-Step Interview Walkthrough**:
  1. Initialize `zeros = 0`.
  2. Loop $p = 5, 25, 125 \dots \le N$: `zeros += N / p`.
  3. Return `zeros`.
- **Pseudocode**:
```text
zeros = 0
p = 5
while p <= N:
    zeros += N / p
    p *= 5
return zeros
```
- **Complexity**: Time: $\mathcal{O}(\log_5 N)$, Space: $\mathcal{O}(1)$.

---

## 2. Palindrome Reorder

- **Tier**: Tier 1
- **Link**: [Palindrome Reorder](https://cses.fi/problemset/task/1755)
- **Problem Statement**: Reorder characters of a string $S$ to form a palindrome, or output `NO SOLUTION`.
- **Interview Pattern**: Frequency Counting + Symmetric Half Mirroring.
- **Intuition**: At most one character can have an odd frequency. Build left half alphabetically, place odd character in middle, append reversed left half.
- **Step-by-Step Interview Walkthrough**:
  1. Count character frequencies.
  2. If count of odd-frequency characters $> 1$, output `NO SOLUTION`.
  3. Build `half` string by placing $\lfloor \text{freq}[c] / 2 \rfloor$ copies of each character $c$.
  4. Output `half` + `odd_char` + `reverse(half)`.
- **Pseudocode**:
```text
if count(freq[c] % 2 == 1) > 1: return "NO SOLUTION"
half = ""
for c in 'A'..'Z': half += c * (freq[c] / 2)
middle = odd_char if exists else ""
return half + middle + reverse(half)
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 3. Gray Code

- **Tier**: Tier 2
- **Link**: [Gray Code](https://cses.fi/problemset/task/2205)
- **Problem Statement**: Generate Gray code sequence of length $N$ (all $2^N$ binary strings of length $N$ such that adjacent strings differ in exactly 1 bit).
- **Interview Pattern**: Binary-to-Gray Code Formula $G(i) = i \oplus (i \gg 1)$.
- **Intuition**: Formula $i \oplus (i \gg 1)$ converts binary index $i$ to Gray code integer in $\mathcal{O}(1)$.
- **Step-by-Step Interview Walkthrough**:
  1. Loop $i$ from 0 to $2^N - 1$.
  2. Compute $g = i \oplus (i \gg 1)$.
  3. Print $g$ as $N$-bit binary string.
- **Pseudocode**:
```text
for i = 0 to (1 << N) - 1:
    g = i ^ (i >> 1)
    print_binary(g, N)
```
- **Complexity**: Time: $\mathcal{O}(2^N \cdot N)$, Space: $\mathcal{O}(1)$.

---

## 4. Apple Division

- **Tier**: Tier 1
- **Link**: [Apple Division](https://cses.fi/problemset/task/1623)
- **Problem Statement**: Divide $N$ apples into two groups to minimize total weight difference ($N \le 20$).
- **Interview Pattern**: Bitmask Subset Sum Enumeration ($2^N$).
- **Intuition**: Iterate all $2^N$ subset bitmasks. For each mask, compute subset sum $S_1$, and evaluate difference $| \text{totalSum} - 2 S_1 |$.
- **Step-by-Step Interview Walkthrough**:
  1. Compute `total = sum(weights)`.
  2. Initialize `min_diff = total`.
  3. Loop `mask` from 0 to $(1 \ll N) - 1$:
     - Compute subset sum $S_1 = \sum_{i: \text{bit } i \text{ set}} \text{weights}[i]$.
     - Update `min_diff = min(min_diff, abs(total - 2 * S1))`.
  4. Return `min_diff`.
- **Pseudocode**:
```text
function apple_division(weights, N):
    total = sum(weights)
    min_diff = total
    for mask = 0 to (1 << N) - 1:
        s1 = 0
        for i = 0 to N - 1:
            if mask & (1 << i):
                s1 += weights[i]
        min_diff = min(min_diff, abs(total - 2 * s1))
    return min_diff
```
- **Complexity**: Time: $\mathcal{O}(N \cdot 2^N)$, Space: $\mathcal{O}(N)$.

---

## 5. Digit Queries

- **Tier**: Tier 2
- **Link**: [Digit Queries](https://cses.fi/problemset/task/2431)
- **Problem Statement**: Find the digit at 1-based index $K$ in the infinite string `"123456789101112..."`.
- **Interview Pattern**: Bucket Range Decomposition + Arithmetic Offset.
- **Intuition**:
  - 1-digit numbers ($1..9$): $9 \times 1$ digits.
  - 2-digit numbers ($10..99$): $90 \times 2$ digits.
  - Subtract bucket sizes from $K$ until landing in correct digit length range `len`.
  - Number $= \text{start} + (K - 1) / \text{len}$, digit index $= (K - 1) \bmod \text{len}$.
- **Step-by-Step Interview Walkthrough**:
  1. `len = 1, count = 9, start = 1`.
  2. While $K > \text{len} \times \text{count}$:
     - $K \mathrel{-}= \text{len} \times \text{count}$.
     - `len += 1, count *= 10, start *= 10`.
  3. Target number $= \text{start} + (K - 1) / \text{len}$.
  4. Return character of Target number at index $(K - 1) \bmod \text{len}$.
- **Pseudocode**:
```text
len = 1, count = 9, start = 1
while K > len * count:
    K -= len * count
    len += 1, count *= 10, start *= 10

num = start + (K - 1) / len
return str(num)[(K - 1) % len]
```
- **Complexity**: Time: $\mathcal{O}(\log_{10} K)$, Space: $\mathcal{O}(1)$.

---
