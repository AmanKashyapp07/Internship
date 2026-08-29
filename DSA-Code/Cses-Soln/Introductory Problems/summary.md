# CSES Introductory Problems - Self-Contained Interview Cheat Sheet

This document contains **problem statements, interview-style explanations, intuitive breakdowns, step-by-step algorithms, structured pseudocode, and complexity analysis** for all 17 problems in the **Introductory Problems** topic.

---

## 1. Weird Algorithm

- **Link**: [Weird Algorithm](https://cses.fi/problemset/task/1068)
- **Problem Statement**: Given a positive integer $N$, simulate the Collatz conjecture process: if $N$ is even divide by 2, if $N$ is odd multiply by 3 and add 1, until $N$ becomes 1. Print all values.
- **Interview Pattern**: Direct Simulation.
- **Intuition**: Use a 64-bit integer (`long long`) to prevent overflow during odd calculation steps ($3N + 1$).
- **Step-by-Step Interview Walkthrough**:
  1. Print $N$.
  2. While $N \ne 1$:
     - If $N \pmod 2 == 0$, $N = N / 2$.
     - Else $N = 3N + 1$.
     - Print $N$.
- **Pseudocode**:
```text
while N != 1:
    print N
    if N % 2 == 0: N /= 2
    else: N = 3 * N + 1
print 1
```
- **Complexity**: Time: $\mathcal{O}(\text{Steps})$, Space: $\mathcal{O}(1)$.

---

## 2. Missing Number

- **Link**: [Missing Number](https://cses.fi/problemset/task/1083)
- **Problem Statement**: Given $N-1$ numbers from $1 \dots N$, find the single missing number.
- **Interview Pattern**: Mathematical Sum Difference.
- **Intuition**: Expected total sum is $\frac{N(N+1)}{2}$. Subtracting each given number from total sum leaves the missing number.
- **Step-by-Step Interview Walkthrough**:
  1. Calculate `total_sum = N * (N + 1) / 2`.
  2. Subtract each input number $x$ from `total_sum`.
  3. Output remaining `total_sum`.
- **Pseudocode**:
```text
total_sum = N * (N + 1) / 2
for x in input_elements:
    total_sum -= x
return total_sum
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(1)$.

---

## 3. Repetitions

- **Link**: [Repetitions](https://cses.fi/problemset/task/1069)
- **Problem Statement**: Given a DNA string $S$, find the length of the longest contiguous substring containing identical characters.
- **Interview Pattern**: Single-Pass Running Counter.
- **Intuition**: Increment counter if current character equals previous character; reset to 1 otherwise. Update maximum length at each step.
- **Step-by-Step Interview Walkthrough**:
  1. Maintain `max_len = 1`, `curr_len = 1`.
  2. For $i = 1 \dots |S|-1$:
     - If $S[i] == S[i-1]$, `curr_len += 1`.
     - Else `curr_len = 1`.
     - `max_len = max(max_len, curr_len)`.
  3. Output `max_len`.
- **Pseudocode**:
```text
max_len = 1, curr_len = 1
for i = 1 to len(S) - 1:
    if S[i] == S[i-1]: curr_len += 1
    else: curr_len = 1
    max_len = max(max_len, curr_len)
return max_len
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(1)$ auxiliary.

---

## 4. Increasing Array

- **Link**: [Increasing Array](https://cses.fi/problemset/task/1094)
- **Problem Statement**: Given an array of $N$ positive integers, find the minimum moves required to make the array non-decreasing (a move increments an element by 1).
- **Interview Pattern**: Greedy Running Prefix Maximum.
- **Intuition**: If $A[i] < A[i-1]$, we must increment $A[i]$ to match $A[i-1]$. Add $(A[i-1] - A[i])$ to total moves and set $A[i] = A[i-1]$.
- **Step-by-Step Interview Walkthrough**:
  1. Maintain `moves = 0`, `prev = A[0]`.
  2. For $i = 1 \dots N-1$:
     - If $A[i] < \text{prev}$, `moves += prev - A[i]`.
     - Else `prev = A[i]`.
  3. Output `moves`.
- **Pseudocode**:
```text
moves = 0, prev = A[0]
for i = 1 to N-1:
    if A[i] < prev: moves += (prev - A[i])
    else: prev = A[i]
return moves
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(1)$.

---

## 5. Permutations

- **Link**: [Permutations](https://cses.fi/problemset/task/1070)
- **Problem Statement**: Construct a permutation of $1 \dots N$ such that no two adjacent elements differ by 1, or output `NO SOLUTION` if impossible.
- **Interview Pattern**: Evens-First, Odds-Second Interleaving.
- **Intuition**:
  - For $N = 2$ or $3$, impossible.
  - Print all even numbers in increasing order ($2, 4, 6, \dots$), then all odd numbers ($1, 3, 5, \dots$).
  - Adjacent difference within evens or odds is 2; difference between last even and 1 is $\ge 2$ for $N \ge 4$.
- **Step-by-Step Interview Walkthrough**:
  1. If $N == 2$ or $N == 3$, print `NO SOLUTION`.
  2. Print $2, 4, 6, \dots \le N$.
  3. Print $1, 3, 5, \dots \le N$.
- **Pseudocode**:
```text
if N == 2 or N == 3: return "NO SOLUTION"
for i = 2 to N step 2: print i
for i = 1 to N step 2: print i
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(1)$.

---

## 6. Number Spiral

- **Link**: [Number Spiral](https://cses.fi/problemset/task/1071)
- **Problem Statement**: Find the value at row $Y$, column $X$ in an infinite numerical spiral grid.
- **Interview Pattern**: Layer Parity $\mathcal{O}(1)$ Mathematical Formula.
- **Intuition**:
  - The layer index $Z = \max(Y, X)$.
  - Layer $Z$ contains values between $(Z-1)^2 + 1$ and $Z^2$.
  - Parity of $Z$ determines direction of numbers along the boundary.
- **Step-by-Step Interview Walkthrough**:
  1. Set $Z = \max(Y, X)$.
  2. If $Z$ is even:
     - If $Y == Z$, answer $= Z^2 - X + 1$.
     - Else answer $= (Z - 1)^2 + Y$.
  3. If $Z$ is odd:
     - If $X == Z$, answer $= Z^2 - Y + 1$.
     - Else answer $= (Z - 1)^2 + X$.
- **Pseudocode**:
```text
Z = max(Y, X)
if Z % 2 == 0:
    if Y == Z: ans = Z * Z - X + 1
    else: ans = (Z - 1) * (Z - 1) + Y
else:
    if X == Z: ans = Z * Z - Y + 1
    else: ans = (Z - 1) * (Z - 1) + X
return ans
```
- **Complexity**: Time: $\mathcal{O}(1)$, Space: $\mathcal{O}(1)$.

---

## 7. Two Knights

- **Link**: [Two Knights](https://cses.fi/problemset/task/1072)
- **Problem Statement**: For each $K = 1 \dots N$, count the number of ways to place two knights on a $K \times K$ chessboard such that they do not attack each other.
- **Interview Pattern**: Combinatorial Complement Counting.
- **Intuition**:
  - Total ways to place 2 knights on $K^2$ squares is $\binom{K^2}{2} = \frac{K^2(K^2-1)}{2}$.
  - Knights attack iff placed on opposite corners of a $2 \times 3$ or $3 \times 2$ block. A $K \times K$ board has $4(K-1)(K-2)$ such attacking pairs.
- **Step-by-Step Interview Walkthrough**:
  1. Total pairs $= (K^2) \times (K^2 - 1) / 2$.
  2. Attacking pairs $= 4 \times (K - 1) \times (K - 2)$.
  3. Valid positions $= \text{total\_pairs} - \text{attacking\_pairs}$.
- **Pseudocode**:
```text
for K = 1 to N:
    total = (K * K) * (K * K - 1) / 2
    attacking = 4 * (K - 1) * (K - 2)
    print total - attacking
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(1)$.

---

## 8. Two Sets

- **Link**: [Two Sets](https://cses.fi/problemset/task/1092)
- **Problem Statement**: Divide numbers $1 \dots N$ into two sets of equal sum if possible, or output `NO`.
- **Interview Pattern**: Reverse Greedy Selection.
- **Intuition**:
  - Total sum $S = N(N+1)/2$. If $S$ is odd, return `NO`.
  - Greedy choice: target $= S / 2$. Iterate $i$ from $N$ down to 1: if $i \le \text{target}$, add $i$ to Set 1 and subtract $i$ from target; else add $i$ to Set 2.
- **Step-by-Step Interview Walkthrough**:
  1. If $N(N+1)/2$ is odd, print `NO`.
  2. Target $= N(N+1)/4$.
  3. For $i = N$ down to 1:
     - If $i \le \text{target}$: add $i$ to $S_1$, `target -= i`.
     - Else: add $i$ to $S_2$.
  4. Print `YES` and both sets.
- **Pseudocode**:
```text
if (N * (N + 1) / 2) % 2 != 0: return NO
target = N * (N + 1) / 4
for i = N down to 1:
    if i <= target: S1.add(i); target -= i
    else: S2.add(i)
return YES, S1, S2
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 9. Bit Strings

- **Link**: [Bit Strings](https://cses.fi/problemset/task/1617)
- **Problem Statement**: Calculate the number of bit strings of length $N$ ($2^N \pmod{10^9+7}$).
- **Interview Pattern**: Binary Exponentiation.
- **Intuition**: $2^N \pmod{10^9+7}$ using fast power in $\mathcal{O}(\log N)$.
- **Complexity**: Time: $\mathcal{O}(\log N)$, Space: $\mathcal{O}(1)$.

---

## 10. Trailing Zeros

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

## 11. Coin Piles

- **Link**: [Coin Piles](https://cses.fi/problemset/task/1754)
- **Problem Statement**: Given two coin piles of size $A$ and $B$, check if you can empty both piles by repeatedly taking 1 coin from one pile and 2 from the other.
- **Interview Pattern**: Invariant Mathematical Validation.
- **Intuition**: Each move removes 3 total coins. Valid iff $(A+B) \equiv 0 \pmod 3$ and neither pile is larger than double the other ($A \le 2B$ and $B \le 2A$).
- **Step-by-Step Interview Walkthrough**:
  1. Check `(a + b) % 3 == 0`.
  2. Check `a <= 2 * b` and `b <= 2 * a`.
  3. If all pass, output `YES`, else `NO`.
- **Pseudocode**:
```text
if (a + b) % 3 == 0 and a <= 2 * b and b <= 2 * a:
    return YES
else:
    return NO
```
- **Complexity**: Time: $\mathcal{O}(1)$, Space: $\mathcal{O}(1)$.

---

## 12. Palindrome Reorder

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

## 13. Creating Strings

- **Link**: [Creating Strings](https://cses.fi/problemset/task/1622)
- **Problem Statement**: Generate all unique permutations of a string $S$ in lexicographical order.
- **Interview Pattern**: Sorting + `std::next_permutation`.
- **Intuition**: Sort string first, then repeatedly generate next permutation until exhaustion.
- **Complexity**: Time: $\mathcal{O}(K! \cdot K)$ ($K \le 8$), Space: $\mathcal{O}(K!)$.

---

## 14. Gray Code

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

## 15. Apple Division

- **Link**: [Apple Division](https://cses.fi/problemset/task/1623)
- **Problem Statement**: Divide $N$ apples into two groups to minimize total weight difference ($N \le 20$).
- **Interview Pattern**: Bitmask Subset Sum Enumeration ($2^N$).
- **Intuition**: Iterate all $2^N$ subset bitmasks. For mask, compute subset sum $S_1$, difference $| \text{totalSum} - 2 S_1 |$.
- **Complexity**: Time: $\mathcal{O}(N \cdot 2^N)$, Space: $\mathcal{O}(N)$.

---

## 16. Digit Queries

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

## 17. Mex Grid Construction

- **Link**: [Mex Grid Construction](https://cses.fi/problemset/task/3305)
- **Problem Statement**: Construct an $N \times N$ grid such that cell $(i, j)$ contains the MEX of row $i$ and column $j$ prefixes.
- **Interview Pattern**: Bitwise XOR Grid Construction ($i \oplus j$).
- **Intuition**: Setting $\text{grid}[i][j] = i \oplus j$ constructs a valid Nim-addition table satisfying prefix MEX properties across all subgrids.
- **Step-by-Step Interview Walkthrough**:
  1. For $i = 0 \dots N-1$:
  2. For $j = 0 \dots N-1$:
     - `grid[i][j] = i ^ j`.
  3. Print grid.
- **Pseudocode**:
```text
for i = 0 to N-1:
    for j = 0 to N-1:
        grid[i][j] = i ^ j
return grid
```
- **Complexity**: Time: $\mathcal{O}(N^2)$, Space: $\mathcal{O}(N^2)$.
