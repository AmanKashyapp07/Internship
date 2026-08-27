# CSES Mathematics - Self-Contained Interview Cheat Sheet

This document contains **problem statements, interview-style explanations, intuitive breakdowns, step-by-step algorithms, structured pseudocode, and complexity analysis** for all 21 problems in the **Mathematics** topic.

---

## 1. Exponentiation

- **Link**: [Exponentiation](https://cses.fi/problemset/task/1095)
- **Problem Statement**: Calculate $a^b \pmod{10^9+7}$ for $N$ test cases.
- **Interview Pattern**: Binary Exponentiation (Square-and-Multiply).
- **Intuition**:
  - $a^b = (a^{b/2})^2$ if $b$ is even, and $a \cdot a^{b-1}$ if $b$ is odd.
  - Halve the exponent each step using bitwise right-shift `b >>= 1` in $\mathcal{O}(\log b)$ operations.
- **Step-by-Step Interview Walkthrough**:
  1. Initialize `result = 1`, `base = a % MOD`.
  2. While $b > 0$:
     - If $b \ \& \ 1$: `result = (result * base) % MOD`.
     - `base = (base * base) % MOD`.
     - `b >>= 1`.
  3. Return `result`.
- **Pseudocode**:
```text
function power(a, b):
    res = 1, base = a % MOD
    while b > 0:
        if b & 1: res = (res * base) % MOD
        base = (base * base) % MOD
        b >>= 1
    return res
```
- **Complexity**: Time: $\mathcal{O}(\log B)$ per query, Space: $\mathcal{O}(1)$.

---

## 2. Exponentiation II

- **Link**: [Exponentiation II](https://cses.fi/problemset/task/1712)
- **Problem Statement**: Calculate $a^{(b^c)} \pmod{10^9+7}$ for $N$ test cases.
- **Interview Pattern**: Double Modular Exponentiation via Fermat's Little Theorem.
- **Intuition**:
  - By Fermat's Little Theorem, $a^{p-1} \equiv 1 \pmod p$ for prime $p = 10^9+7$.
  - Therefore, $a^x \equiv a^{x \pmod{p-1}} \pmod p$.
  - First compute $E = b^c \pmod{10^9+6}$, then compute $a^E \pmod{10^9+7}$.
- **Step-by-Step Interview Walkthrough**:
  1. Compute exponent $E = \text{power}(b, c, 10^9 + 6)$.
  2. Compute final answer $= \text{power}(a, E, 10^9 + 7)$.
  3. Print answer.
- **Pseudocode**:
```text
exp = power(b, c, MOD - 1)
ans = power(a, exp, MOD)
return ans
```
- **Complexity**: Time: $\mathcal{O}(\log C + \log B)$ per query, Space: $\mathcal{O}(1)$.

---

## 3. Counting Divisors

- **Link**: [Counting Divisors](https://cses.fi/problemset/task/1713)
- **Problem Statement**: Given $N$ queries, find the total number of divisors of integer $x_i$ ($x_i \le 10^6$).
- **Interview Pattern**: Sieve-Style Precomputation / Harmonic Series Sweep.
- **Intuition**:
  - Instead of factorizing each $x_i$ independently in $\mathcal{O}(\sqrt{x_i})$, precompute a divisor-count table `div_count` for all $1 \dots 10^6$ using a Sieve in $\mathcal{O}(MAX \log MAX)$ total time.
- **Step-by-Step Interview Walkthrough**:
  1. Allocate `div_count[1..10^6] = 0`.
  2. For $d = 1 \dots 10^6$: for multiple $m = d, 2d, 3d \dots \le 10^6$: `div_count[m] += 1`.
  3. Answer each query $x$ in $\mathcal{O}(1)$ time by returning `div_count[x]`.
- **Pseudocode**:
```text
for d = 1 to 10^6:
    for m = d to 10^6 step d:
        div_count[m] += 1

for query x:
    print div_count[x]
```
- **Complexity**: Precomputation: $\mathcal{O}(MAX \log MAX)$, Query: $\mathcal{O}(1)$, Space: $\mathcal{O}(MAX)$.

---

## 4. Common Divisors

- **Link**: [Common Divisors](https://cses.fi/problemset/task/1081)
- **Problem Statement**: Given an array of $N$ positive integers, find the maximum Greatest Common Divisor (GCD) shared by at least two numbers in the array.
- **Interview Pattern**: Frequency Array + Divisor Sieve Sweep.
- **Intuition**:
  - For each candidate GCD $d$ from $10^6$ down to 1, count how many elements in the input array are multiples of $d$.
  - The first candidate $d$ that has $\ge 2$ multiples in the array is the maximum GCD!
- **Step-by-Step Interview Walkthrough**:
  1. Build frequency array `freq[x]` of input elements.
  2. For $d = 10^6$ down to 1:
     - Count total multiples: `cnt = sum(freq[m] for m in d, 2d, 3d...)`.
     - If `cnt >= 2`: return $d$.
- **Pseudocode**:
```text
for x in A: freq[x] += 1

for d = 10^6 down to 1:
    multiples = 0
    for m = d to 10^6 step d:
        multiples += freq[m]
    if multiples >= 2:
        return d
```
- **Complexity**: Time: $\mathcal{O}(MAX \log MAX + N)$, Space: $\mathcal{O}(MAX)$.

---

## 5. Next Prime

- **Link**: [Next Prime](https://cses.fi/problemset/task/3396)
- **Problem Statement**: Given $N$ queries $n_i$, find the smallest prime number strictly greater than $n_i$.
- **Interview Pattern**: Trial Division Primality Test + Linear Odd-Step Search.
- **Intuition**:
  - By Bertrand's Postulate, a prime always exists between $n$ and $2n$.
  - Start checking at $x = n + 1$ (make odd if $> 2$), test primality via trial division up to $\sqrt{x}$ in steps of 2.
- **Step-by-Step Interview Walkthrough**:
  1. For query $n$: set $x = n + 1$.
  2. If $x \le 2$, return 2. If $x$ is even, $x = x + 1$.
  3. While `!isPrime(x)`: $x = x + 2$.
  4. Return $x$.
- **Pseudocode**:
```text
function isPrime(n):
    if n < 2: return false
    if n == 2: return true
    if n % 2 == 0: return false
    for d = 3 to sqrt(n) step 2:
        if n % d == 0: return false
    return true

x = n + 1
if x <= 2: return 2
if x % 2 == 0: x += 1
while not isPrime(x): x += 2
return x
```
- **Complexity**: Time: $\mathcal{O}(\sqrt{P})$ per query where $P \approx n$, Space: $\mathcal{O}(1)$.

---

## 6. Binomial Coefficients

- **Link**: [Binomial Coefficients](https://cses.fi/problemset/task/1079)
- **Problem Statement**: Calculate $\binom{a}{b} = \frac{a!}{b!(a-b)!} \pmod{10^9+7}$ for $N$ queries ($a, b \le 10^6$).
- **Interview Pattern**: Precomputed Factorials and Inverse Factorials.
- **Intuition**:
  - Precompute `fact[i]` and `invFact[i]` up to $10^6$ using Fermat's Little Theorem: $\text{invFact}[N] = (N!)^{\text{MOD}-2} \pmod{\text{MOD}}$.
  - Each query evaluates in $\mathcal{O}(1)$ time as $\text{fact}[a] \times \text{invFact}[b] \times \text{invFact}[a-b] \pmod{\text{MOD}}$.
- **Step-by-Step Interview Walkthrough**:
  1. Precompute `fact[0..10^6]`.
  2. Compute `invFact[10^6] = power(fact[10^6], MOD - 2)`.
  3. Compute `invFact[i-1] = invFact[i] * i % MOD` backwards.
  4. Query $(a, b)$: return `fact[a] * invFact[b] % MOD * invFact[a-b] % MOD`.
- **Pseudocode**:
```text
fact[0] = 1
for i = 1 to MAX: fact[i] = fact[i-1] * i % MOD
invFact[MAX] = power(fact[MAX], MOD - 2)
for i = MAX down to 1: invFact[i-1] = invFact[i] * i % MOD

function nCr(a, b):
    return fact[a] * invFact[b] % MOD * invFact[a - b] % MOD
```
- **Complexity**: Precomputation: $\mathcal{O}(MAX)$, Query: $\mathcal{O}(1)$, Space: $\mathcal{O}(MAX)$.

---

## 7. Bracket Sequences I

- **Link**: [Bracket Sequences I](https://cses.fi/problemset/task/2064)
- **Problem Statement**: Count the number of valid bracket sequences of length $N$ modulo $10^9+7$.
- **Interview Pattern**: Catalan Numbers $C_k = \frac{1}{k+1} \binom{2k}{k}$.
- **Intuition**:
  - If $N$ is odd, no valid bracket sequence can be formed $\to 0$.
  - If $N$ is even ($N = 2k$), the number of valid bracket sequences equals the $k$-th Catalan number $C_k = \frac{1}{k+1} \binom{2k}{k} \pmod{10^9+7}$.
- **Step-by-Step Interview Walkthrough**:
  1. If $N$ is odd, return 0.
  2. Set $k = N / 2$.
  3. Compute Catalan number $C_k = \binom{2k}{k} \times (k + 1)^{-1} \pmod{10^9+7}$.
- **Pseudocode**:
```text
if N % 2 != 0: return 0
k = N / 2
nCr = fact[2*k] * invFact[k] % MOD * invFact[k] % MOD
ans = nCr * power(k + 1, MOD - 2) % MOD
return ans
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 8. Christmas Party

- **Link**: [Christmas Party](https://cses.fi/problemset/task/1717)
- **Problem Statement**: $N$ children exchange gifts such that no child receives their own gift. Count the total valid gift assignments (derangements $D_N$) modulo $10^9+7$.
- **Interview Pattern**: Derangement Recurrence $D_n = (n-1)(D_{n-1} + D_{n-2})$.
- **Intuition**:
  - Base cases: $D_1 = 0, D_2 = 1$.
  - Child 1 can trade with any of $(N-1)$ children. If they swap gifts mutually, remaining subproblem is $D_{N-2}$. If not mutual, remaining is $D_{N-1}$.
  - Recurrence: $D_n = (n-1)(D_{n-1} + D_{n-2}) \pmod{10^9+7}$.
- **Step-by-Step Interview Walkthrough**:
  1. If $N == 1$, return 0. If $N == 2$, return 1.
  2. Maintain `prev2 = 0`, `prev1 = 1`.
  3. Loop $i$ from 3 to $N$:
     - `curr = (i - 1) * (prev1 + prev2) % MOD`.
     - `prev2 = prev1`, `prev1 = curr`.
  4. Return `curr`.
- **Pseudocode**:
```text
if N == 1: return 0
if N == 2: return 1
d2 = 0, d1 = 1
for i = 3 to N:
    curr = (i - 1) * (d1 + d2) % MOD
    d2 = d1, d1 = curr
return d1
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(1)$.

---

## 9. Creating Strings II

- **Link**: [Creating Strings II](https://cses.fi/problemset/task/1715)
- **Problem Statement**: Given a string $S$ of length $N$ containing repeating characters, count the number of distinct permutations of the string modulo $10^9+7$.
- **Interview Pattern**: Multinomial Coefficient $\frac{N!}{\prod (f_c!)}$.
- **Intuition**:
  - Distinct permutations of a multiset of size $N$ with character frequencies $f_a, f_b, \dots, f_z$ equals $\frac{N!}{f_a! \cdot f_b! \cdot \dots \cdot f_z!} \pmod{10^9+7}$.
- **Step-by-Step Interview Walkthrough**:
  1. Count frequency $f_c$ of each character in $S$.
  2. Compute `ans = fact[N]`.
  3. For each non-zero frequency $f_c$: `ans = ans * invFact[f_c] % MOD`.
  4. Output `ans`.
- **Pseudocode**:
```text
ans = fact[N]
for count in freq:
    ans = ans * invFact[count] % MOD
return ans
```
- **Complexity**: Time: $\mathcal{O}(N)$, Space: $\mathcal{O}(N)$.

---

## 10. Distributing Apples

- **Link**: [Distributing Apples](https://cses.fi/problemset/task/1716)
- **Problem Statement**: Count the number of ways to distribute $M$ identical apples to $N$ children modulo $10^9+7$.
- **Interview Pattern**: Stars and Bars Theorem $\binom{N + M - 1}{M}$.
- **Intuition**:
  - Distributing $M$ identical objects into $N$ distinct bins is equivalent to arranging $M$ stars and $N-1$ bars in a line: $\binom{N + M - 1}{M} \pmod{10^9+7}$.
- **Step-by-Step Interview Walkthrough**:
  1. Total items $K = N + M - 1$.
  2. Compute $\binom{K}{M} = \text{fact}[K] \times \text{invFact}[M] \times \text{invFact}[N-1] \pmod{10^9+7}$.
- **Pseudocode**:
```text
K = N + M - 1
return fact[K] * invFact[M] % MOD * invFact[N - 1] % MOD
```
- **Complexity**: Time: $\mathcal{O}(N + M)$, Space: $\mathcal{O}(N + M)$.

---

## 11. Divisor Analysis

- **Link**: [Divisor Analysis](https://cses.fi/problemset/task/2182)
- **Problem Statement**: Given prime factorization of $N = \prod p_i^{k_i}$, compute: (1) total number of divisors, (2) sum of divisors, (3) product of divisors modulo $10^9+7$.
- **Interview Pattern**: Multiplicative Number Theory Formulas + Fermat's Exponent Reduction.
- **Intuition**:
  - Number of divisors: $d(N) = \prod (k_i + 1) \pmod{\text{MOD}}$.
  - Sum of divisors: $\sigma(N) = \prod \frac{p_i^{k_i+1} - 1}{p_i - 1} \pmod{\text{MOD}}$.
  - Product of divisors: $P(N) = N^{d(N)/2} \pmod{\text{MOD}}$. Compute exponent using Fermat's theorem modulo $(\text{MOD}-1)$.
- **Step-by-Step Interview Walkthrough**:
  1. Compute $d(N)$ modulo $\text{MOD}$.
  2. Compute $\sigma(N)$ using geometric series formula for each prime factor.
  3. Compute product of divisors maintaining running exponent modulo $(\text{MOD}-1)$.
- **Pseudocode**:
```text
num = 1, sum_div = 1, prod_div = 1, num_mod_phi = 1
for (p, k) in prime_factors:
    num = num * (k + 1) % MOD
    geom = (power(p, k + 1) - 1) * power(p - 1, MOD - 2) % MOD
    sum_div = sum_div * geom % MOD
    
    tri = k * (k + 1) / 2 % (MOD - 1)
    exp = num_mod_phi * tri % (MOD - 1)
    prod_div = power(prod_div, k + 1) * power(p, exp) % MOD
    num_mod_phi = num_mod_phi * (k + 1) % (MOD - 1)
```
- **Complexity**: Time: $\mathcal{O}(N \log \text{MOD})$, Space: $\mathcal{O}(1)$.

---

## 12. Fibonacci Numbers

- **Link**: [Fibonacci Numbers](https://cses.fi/problemset/task/1722)
- **Problem Statement**: Compute the $N$-th Fibonacci number $F_N$ modulo $10^9+7$ for $N \le 10^{18}$.
- **Interview Pattern**: Fast Doubling Fibonacci Method.
- **Intuition**:
  - Fast doubling identities:
    - $F_{2k} = F_k (2 F_{k+1} - F_k)$
    - $F_{2k+1} = F_k^2 + F_{k+1}^2$
  - Recursive function returning pair $\{F_k, F_{k+1}\}$ computes $F_N$ in $\mathcal{O}(\log N)$ time.
- **Step-by-Step Interview Walkthrough**:
  1. Recursive base case: `fib(0) = {0, 1}`.
  2. For `fib(n / 2)` returning $\{a, b\} = \{F_k, F_{k+1}\}$:
     - Compute $c = F_{2k} = a (2b - a) \pmod{\text{MOD}}$.
     - Compute $d = F_{2k+1} = (a^2 + b^2) \pmod{\text{MOD}}$.
  3. If $N$ is even, return $\{c, d\}$. If odd, return $\{d, c + d\}$.
- **Pseudocode**:
```text
function fib(n):
    if n == 0: return {0, 1}
    (a, b) = fib(n / 2)
    c = a * ((2 * b - a + MOD) % MOD) % MOD
    d = (a * a + b * b) % MOD
    if n % 2 == 0: return {c, d}
    else: return {d, (c + d) % MOD}
```
- **Complexity**: Time: $\mathcal{O}(\log N)$, Space: $\mathcal{O}(\log N)$.

---

## 13. Graph Paths I

- **Link**: [Graph Paths I](https://cses.fi/problemset/task/1723)
- **Problem Statement**: Given a directed graph with $N$ nodes and $M$ edges, count the number of paths of length $K$ from node 1 to node $N$ modulo $10^9+7$.
- **Interview Pattern**: Adjacency Matrix Exponentiation.
- **Intuition**:
  - The cell $(A^K)_{i, j}$ of the adjacency matrix $A$ raised to power $K$ gives the number of directed paths of length $K$ from node $i$ to node $j$.
- **Step-by-Step Interview Walkthrough**:
  1. Construct $N \times N$ adjacency matrix $A$.
  2. Compute matrix power $P = A^K \pmod{10^9+7}$ using binary matrix exponentiation.
  3. Output $P[0][N-1]$.
- **Pseudocode**:
```text
A[u][v] = count_of_directed_edges(u -> v)
P = matrix_power(A, K)
return P[0][N-1]
```
- **Complexity**: Time: $\mathcal{O}(N^3 \log K)$, Space: $\mathcal{O}(N^2)$.

---

## 14. Permutation Order

- **Link**: [Permutation Order](https://cses.fi/problemset/task/3397)
- **Problem Statement**: Process two query types: (1) Find the $K$-th lexicographical permutation of $1 \dots N$, (2) Find the 1-based lexicographical rank of a given permutation.
- **Interview Pattern**: Factorial Number System (Lehmer Code).
- **Intuition**:
  - Each element at position $i$ fixes a block of $(N - i - 1)!$ permutations.
  - Index in unused elements = $K / (N - i - 1)!$.
- **Step-by-Step Interview Walkthrough**:
  1. **Rank to Permutation**: For each position, compute index = $K / (N - i - 1)!$, pick the `index`-th unused number, and update $K = K \bmod (N - i - 1)!$.
  2. **Permutation to Rank**: For each element $x_i$, count how many unused elements are smaller than $x_i$ (say $c$), add $c \times (N - i - 1)!$ to rank.
- **Pseudocode**:
```text
function kth_permutation(n, k):
    k -= 1, unused = [1..n]
    for rem = n down to 1:
        block = fact[rem - 1]
        idx = k / block
        ans.push(unused[idx])
        unused.erase(idx)
        k %= block
    return ans
```
- **Complexity**: Time: $\mathcal{O}(N^2)$, Space: $\mathcal{O}(N)$.

---

## 15. Permutation Rounds

- **Link**: [Permutation Rounds](https://cses.fi/problemset/task/3398)
- **Problem Statement**: Given a permutation $A$ of $1 \dots N$, find the minimum number of rounds of applying $A$ until all elements return to their initial positions modulo $10^9+7$.
- **Interview Pattern**: Disjoint Cycle Decomposition + LCM over Prime Factorization.
- **Intuition**:
  - Decompose permutation into disjoint cyclic orbits $C_1, C_2, \dots, C_k$.
  - The return period is $\text{LCM}(|C_1|, |C_2|, \dots, |C_k|)$.
  - Compute LCM modulo $10^9+7$ by taking the maximum power of each prime factor across all cycle lengths.
- **Step-by-Step Interview Walkthrough**:
  1. Decompose permutation into disjoint cycles and record cycle lengths.
  2. Factorize each cycle length into prime powers `prime^p`.
  3. Maintain `maxPower[prime] = max(maxPower[prime], p)`.
  4. Compute $\text{LCM} = \prod (\text{prime}^{\text{maxPower}[\text{prime}]}) \pmod{10^9+7}$.
- **Pseudocode**:
```text
for i = 1 to N:
    if not vis[i]:
        len = get_cycle_length(i)
        update_prime_powers(len)

lcm = 1
for (prime, power) in maxPower:
    lcm = lcm * power(prime, power) % MOD
return lcm
```
- **Complexity**: Time: $\mathcal{O}(N \sqrt{N})$, Space: $\mathcal{O}(N)$.

---

## 16. Prime Multiples

- **Link**: [Prime Multiples](https://cses.fi/problemset/task/2185)
- **Problem Statement**: Given an integer $N$ and $K$ prime numbers, count how many integers in $1 \dots N$ are divisible by at least one of the given primes ($K \le 20$).
- **Interview Pattern**: Inclusion-Exclusion Principle over Bitmask Subsets.
- **Intuition**:
  - By Inclusion-Exclusion, total multiples $= \sum_{\emptyset \ne S \subseteq P} (-1)^{|S|+1} \lfloor N / \prod_{p \in S} p \rfloor$.
  - Avoid overflow when multiplying primes by checking `prod > N / p`.
- **Step-by-Step Interview Walkthrough**:
  1. Iterate bitmask `mask` from 1 to $2^K - 1$.
  2. Calculate product of selected primes in `mask`. If product exceeds $N$, skip.
  3. If bit count $|S|$ is odd, add $\lfloor N / \text{prod} \rfloor$ to answer; if even, subtract.
- **Pseudocode**:
```text
ans = 0
for mask = 1 to (1<<K) - 1:
    prod = 1, ok = true
    for i = 0 to K-1:
        if mask & (1 << i):
            if prod > N / P[i]: ok = false; break
            prod *= P[i]
    if ok:
        if popcount(mask) % 2 == 1: ans += N / prod
        else: ans -= N / prod
return ans
```
- **Complexity**: Time: $\mathcal{O}(K \cdot 2^K)$, Space: $\mathcal{O}(K)$.

---

## 17. Sum of Divisors

- **Link**: [Sum of Divisors](https://cses.fi/problemset/task/1082)
- **Problem Statement**: Calculate $\sum_{i=1}^{N} \sigma(i) \pmod{10^9+7}$, where $\sigma(i)$ is the sum of all divisors of $i$ ($N \le 10^{12}$).
- **Interview Pattern**: Quotient Range Decomposition (Square Root Decomposition).
- **Intuition**:
  - Rewriting sum: $\sum_{i=1}^{N} \sigma(i) = \sum_{d=1}^{N} d \cdot \lfloor N / d \rfloor$.
  - The value of $q = \lfloor N / d \rfloor$ remains constant over continuous ranges $[l, r]$.
  - There are at most $2\sqrt{N}$ distinct quotient values!
- **Step-by-Step Interview Walkthrough**:
  1. Initialize $l = 1$, `total_sum = 0`.
  2. While $l \le N$:
     - Quotient $q = \lfloor N / l \rfloor$.
     - Range end $r = \lfloor N / q \rfloor$.
     - Sum of $d$ in range $[l, r]$ is arithmetic progression sum $\frac{(l + r)(r - l + 1)}{2} \pmod{\text{MOD}}$.
     - `total_sum += q * sum_range % MOD`.
     - Jump $l = r + 1$.
- **Pseudocode**:
```text
l = 1, ans = 0
while l <= N:
    q = N / l
    r = N / q
    count = (r - l + 1) % MOD
    sum_range = (l + r) % MOD * count % MOD * INV2 % MOD
    ans = (ans + (q % MOD) * sum_range) % MOD
    l = r + 1
return ans
```
- **Complexity**: Time: $\mathcal{O}(\sqrt{N})$, Space: $\mathcal{O}(1)$.

---

## 18. Sum of Four Squares

- **Link**: [Sum of Four Squares](https://cses.fi/problemset/task/3355)
- **Problem Statement**: Given $N$, express $N$ as the sum of 4 squares $N = a^2 + b^2 + c^2 + d^2$ (Lagrange's Four-Square Theorem).
- **Interview Pattern**: Precomputed 2-Square Lookup Table + Meet-in-the-Middle Splitting.
- **Intuition**:
  - By Lagrange's Theorem, every non-negative integer can be written as the sum of 4 squares.
  - Precompute 2-square representation table `twoSq[s] = {a, b}` for all $s \le 10^7$.
  - Split $N = s + (N - s)$ and find $s$ such that both $s$ and $N - s$ can be represented as sums of 2 squares.
- **Step-by-Step Interview Walkthrough**:
  1. Precompute `twoSq[a^2 + b^2] = {a, b}` for all $a \le b \le \sqrt{MAX}$.
  2. For query $N$: iterate $s$ from 0 to $N$:
     - If both `twoSq[s]` and `twoSq[N - s]` exist:
       - Retrieve $\{a, b\} = \text{twoSq}[s]$ and $\{c, d\} = \text{twoSq}[N - s]$.
       - Return $\{a, b, c, d\}$.
- **Pseudocode**:
```text
// Precomputation
for a = 0 to sqrt(MAX):
    for b = a to sqrt(MAX):
        s = a*a + b*b
        if s <= MAX and twoSq[s] empty: twoSq[s] = {a, b}

// Query
for s = 0 to N:
    if twoSq[s] and twoSq[N - s]:
        return {twoSq[s].a, twoSq[s].b, twoSq[N-s].a, twoSq[N-s].b}
```
- **Complexity**: Time: $\mathcal{O}(MAX + T \cdot N)$, Space: $\mathcal{O}(MAX)$.

---

## 19. Throwing Dice

- **Link**: [Throwing Dice](https://cses.fi/problemset/task/1096)
- **Problem Statement**: Count ways to reach sum $N$ by throwing a 6-sided die modulo $10^9+7$ ($N \le 10^{18}$).
- **Interview Pattern**: $6 \times 6$ Matrix Exponentiation on Linear Recurrence.
- **Intuition**:
  - DP recurrence: $f(n) = f(n-1) + f(n-2) + f(n-3) + f(n-4) + f(n-5) + f(n-6)$.
  - Express state transition as a $6 \times 6$ matrix $T$ and evaluate $T^{N-5} \times \text{BaseState}$ using binary matrix exponentiation.
- **Step-by-Step Interview Walkthrough**:
  1. Compute base cases $dp[0 \dots 5]$.
  2. Construct $6 \times 6$ transition matrix $T$ with first row all 1s and sub-diagonal 1s.
  3. Compute $P = T^{N-5} \pmod{10^9+7}$.
  4. Answer is $\sum_{j=0}^{5} P[0][j] \times dp[5-j] \pmod{10^9+7}$.
- **Pseudocode**:
```text
T = [[1,1,1,1,1,1],
     [1,0,0,0,0,0],
     [0,1,0,0,0,0],
     [0,0,1,0,0,0],
     [0,0,0,1,0,0],
     [0,0,0,0,1,0]]

P = matrix_power(T, N - 5)
ans = sum(P[0][j] * dp[5 - j] for j in 0..5) % MOD
return ans
```
- **Complexity**: Time: $\mathcal{O}(6^3 \log N)$, Space: $\mathcal{O}(6^2)$.

---

## 20. Triangle Number Sums

- **Link**: [Triangle Number Sums](https://cses.fi/problemset/task/3406)
- **Problem Statement**: Find the minimum number of triangular numbers ($T_k = k(k+1)/2$) needed to sum to $N$.
- **Interview Pattern**: Gauss's Triangular Number Theorem + Two Pointers.
- **Intuition**:
  - Gauss's Eureka theorem states every integer is the sum of at most 3 triangular numbers!
  - Answer is always 1, 2, or 3.
  - Test 1: binary search in precomputed triangular numbers.
  - Test 2: two pointers on precomputed triangular numbers array.
  - Otherwise: answer is 3.
- **Step-by-Step Interview Walkthrough**:
  1. Precompute all triangular numbers $T_k \le 10^{12}$.
  2. If $N \in T$, return 1.
  3. Run Two Pointers (`left = 0`, `right = max_index`): if $T_{\text{left}} + T_{\text{right}} == N$, return 2.
  4. Otherwise, return 3.
- **Pseudocode**:
```text
if N in triangular_list: return 1

left = 0, right = upper_bound(N) - 1
while left <= right:
    s = T[left] + T[right]
    if s == N: return 2
    else if s < N: left += 1
    else: right -= 1

return 3
```
- **Complexity**: Time: $\mathcal{O}(\sqrt{N} + T \cdot \sqrt{N})$, Space: $\mathcal{O}(\sqrt{N})$.

---

## 21. Counting Coprime Pairs

- **Link**: [Counting Coprime Pairs](https://cses.fi/problemset/task/2417)
- **Problem Statement**: Given an array of $N$ integers, count the number of pairs $(A[i], A[j])$ with $i < j$ such that $\gcd(A[i], A[j]) = 1$.
- **Interview Pattern**: Inclusion-Exclusion on Exact GCDs (Backward Frequency Inversion).
- **Intuition**:
  - For candidate GCD $d$, count naive pairs $P(d) = \binom{c_d}{2}$ where $c_d$ is total multiples of $d$.
  - Exact GCD count $E[d] = P(d) - \sum_{m = 2d, 3d, \dots} E[m]$.
  - Desired coprime pair count is $E[1]$.
- **Step-by-Step Interview Walkthrough**:
  1. Compute frequency array of input numbers.
  2. For $d = \text{MAX}$ down to 1:
     - Count total multiples $c_d$.
     - Set $E[d] = c_d \times (c_d - 1) / 2$.
     - Subtract exact counts of strict multiples: $E[d] = E[d] - \sum_{m=2d, 3d, \dots} E[m]$.
  3. Output $E[1]$.
- **Pseudocode**:
```text
for d = MAX down to 1:
    multiples = sum(freq[m] for m in d, 2d, 3d...)
    E[d] = multiples * (multiples - 1) / 2
    for m in 2d, 3d, 4d...:
        E[d] -= E[m]

return E[1]
```
- **Complexity**: Time: $\mathcal{O}(MAX \log MAX + N)$, Space: $\mathcal{O}(MAX)$.
