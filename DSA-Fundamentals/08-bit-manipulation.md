# Section 08: Bit Manipulation

> **Focus:** Low-level bitwise operators, hardware Two's Complement representation, essential bit manipulation hacks, XOR mathematical properties, and Bitmasking for combinatorial Dynamic Programming.

---

# Table of Contents
1. [Bitwise Operators & Two's Complement](#1-bitwise-operators--twos-complement)
2. [Essential Bit Manipulation Hacks](#2-essential-bit-manipulation-hacks)
3. [XOR Properties & Interview Patterns](#3-xor-properties--interview-patterns)
4. [Bitmasking for Combinatorics & DP](#4-bitmasking-for-combinatorics--dp)
5. [Quick Recap](#5-quick-recap)

---

# 1. Bitwise Operators & Two's Complement

Bitwise operations manipulate individual binary bits of integer data types directly inside CPU registers, executing in **1 single clock cycle ($O(1)$ time)**.

```
+-----------------------------------------------------------------------------------------+
| OPERATOR        | SYNTAX  | OPERATION                                                   |
+-----------------------------------------------------------------------------------------+
| Bitwise AND     | a & b   | 1 only if BOTH bits are 1.                                  |
| Bitwise OR      | a | b   | 1 if AT LEAST ONE bit is 1.                                 |
| Bitwise XOR     | a ^ b   | 1 if bits are DIFFERENT (Exclusive OR).                     |
| Bitwise NOT     | ~a      | Inverts all bits (0 -> 1, 1 -> 0).                          |
| Left Shift      | a << k  | Shifts bits left by k positions (Multiplies by 2^k).        |
| Right Shift     | a >> k  | Shifts bits right by k positions (Divides by 2^k).          |
+-----------------------------------------------------------------------------------------+
```

---

### Two's Complement Representation of Negative Integers
Modern computer architectures represent negative integers using **Two's Complement**:

$$-X = \sim X + 1 \quad (\text{Invert all bits and add } 1)$$

```
Example for 8-bit Integer representation:
+5 in binary:     0000 0101
Invert bits (~5): 1111 1010
Add 1:            1111 1011  ==> This is -5 in binary!

Notice: Adding (+5) + (-5):
   0000 0101 (+5)
 + 1111 1011 (-5)
 = 0000 0000 (with 1 carried out and discarded) = 0!
```

---

### Arithmetic Shift vs. Logical Shift
- **Arithmetic Right Shift (`>>` on signed types in C++):** Preserves the sign bit (copies the MSB sign bit into vacant high-order positions).
- **Logical Right Shift (`>>>` in Java / `>>` on `unsigned int` in C++):** Always injects zeroes into high-order positions regardless of sign.

---

# 2. Essential Bit Manipulation Hacks

```
+-----------------------------------------------------------------------------------------+
| OPERATION                   | BITWISE EXPRESSION       | MECHANICAL EXPLANATION         |
+-----------------------------------------------------------------------------------------+
| Check if k-th bit is set    | (n >> k) & 1             | Shifts bit k to index 0.       |
| Set k-th bit                | n | (1 << k)             | Forces bit k to 1.             |
| Clear k-th bit              | n & ~(1 << k)            | Masks bit k to 0 via NOT.      |
| Toggle k-th bit             | n ^ (1 << k)             | Flips bit k (0->1, 1->0).      |
| Isolate lowest set bit      | n & (-n)                 | Exploits Two's Complement.     |
| Clear lowest set bit        | n & (n - 1)              | Flips lowest 1 and trailing 0s.|
| Power of 2 Check            | n > 0 && !(n & (n - 1))  | Powers of 2 have exactly one 1.|
+-----------------------------------------------------------------------------------------+
```

---

### Deep Dive: Isolate Lowest Set Bit (`n & (-n)`)
- Let $n = 12$ (`0000 1100`).
- $-n = \sim n + 1 = 1111 0011 + 1 = 1111 0100$.
- $n \ \& \ (-n) = 0000 1100 \ \& \ 1111 0100 = \mathbf{0000\ 0100}$ (Isolates bit at position 2!).

---

### Deep Dive: Brian Kernighan's Algorithm (Counting Set Bits)
Subtracting 1 from a number flips its lowest set bit and all trailing zeroes (`1100 - 1 = 1011`). Performing `n & (n - 1)` clears that lowest set bit on every iteration.
- **Time Complexity:** strictly **$O(\text{Number of Set Bits})$**, running faster than scanning all 32 bits.

```cpp
// Count Set Bits (Hamming Weight) in O(Set Bits)
int countSetBits(int n) {
    int count = 0;
    while (n > 0) {
        n &= (n - 1); // Clears the lowest set bit!
        count++;
    }
    return count;
    // Alternative GCC Built-in (Single CPU Instruction POPCNT): __builtin_popcount(n);
}
```

---

# 3. XOR Properties & Interview Patterns

The **Exclusive OR ($\oplus$)** operator possesses unique mathematical identities:

```
Fundamental XOR Axioms:
1. Identity:      x ^ 0 = x
2. Self-Inverse: x ^ x = 0  (Any number XORed with itself cancels to 0!)
3. Commutative:   a ^ b = b ^ a
4. Associative:   (a ^ b) ^ c = a ^ (b ^ c)
```

---

### Pattern 1: Single Number (All appear twice, one appears once)
XORing all numbers together cancels out all duplicate pairs ($x \oplus x = 0$), leaving only the unique number in **$O(N)$ Time and $O(1)$ Space**:

```cpp
int singleNumber(const vector<int>& nums) {
    int unique = 0;
    for (int x : nums) unique ^= x;
    return unique;
}
```

---

### Pattern 2: Single Number III (Two unique numbers, all others twice)
Given an array where exactly two numbers $X$ and $Y$ appear once, and all other numbers appear twice:
1. XOR all numbers: $\text{xorAll} = X \oplus Y$. Because $X \ne Y$, $\text{xorAll} \ne 0$.
2. Find any set bit in $\text{xorAll}$ (e.g. Lowest Set Bit: `diffBit = xorAll & (-xorAll)`). This bit must be `1` in $X$ and `0` in $Y$ (or vice versa).
3. Partition the array into two groups based on `diffBit`:
   - Group 1: Numbers with `diffBit` set.
   - Group 2: Numbers with `diffBit` unset.
4. XORing within each group isolates $X$ and $Y$ in **$O(N)$ Time and $O(1)$ Space**!

```cpp
// Single Number III: O(N) Time, O(1) Space
pair<int, int> singleNumber2(const vector<int>& nums) {
    long long xorAll = 0;
    for (int x : nums) xorAll ^= x;
    
    // Isolate lowest set bit
    long long lowestBit = xorAll & (-xorAll);
    int x = 0, y = 0;
    
    for (int num : nums) {
        if (num & lowestBit) x ^= num; // Group 1
        else y ^= num;                 // Group 2
    }
    return {x, y};
}
```

---

# 4. Bitmasking for Combinatorics & DP

An integer's binary representation can model the presence or absence of elements in a finite set of size $N \le 30$.

$$\text{Mask } = 01011_2 \implies \text{Set contains elements at indices } \{0, 1, 3\}$$

```
Bitmask Operations Cheat Sheet:
- Empty Set:                mask = 0
- Universe Set of size N:   mask = (1 << N) - 1
- Add element i:            mask |= (1 << i)
- Remove element i:         mask &= ~(1 << i)
- Check membership of i:    (mask >> i) & 1
- Size of subset:           __builtin_popcount(mask)
```

---

### A. Generating All $2^N$ Subsets via Bitmasking

```cpp
// Generate all subsets of an array of size N <= 20 (O(N * 2^N) Time)
vector<vector<int>> getSubsets(const vector<int>& nums) {
    int n = nums.size();
    int totalSubsets = 1 << n; // 2^N
    vector<vector<int>> subsets;
    
    for (int mask = 0; mask < totalSubsets; mask++) {
        vector<int> current;
        for (int i = 0; i < n; i++) {
            if ((mask >> i) & 1) {
                current.push_back(nums[i]);
            }
        }
        subsets.push_back(current);
    }
    return subsets;
}
```

---

### B. Iterating Over All Submasks of a Mask
To iterate through all sub-subsets of a given bitmask without scanning all numbers:

```cpp
// Iterates through all submasks of 'mask' in O(2^(popcount(mask))) time
for (int submask = mask; submask > 0; submask = (submask - 1) & mask) {
    // Process submask
}
// Sum of submasks over all masks of size N = (1 + 2)^N = 3^N (Binomial Theorem!)
```

---

### C. Bitmask DP: Traveling Salesperson Problem (TSP)
Given $N$ cities and distances between them, find the minimum cost tour visiting every city once and returning to start.
- **Brute Force:** $O(N!)$ ($N = 20 \implies 20! \approx 2.4 \times 10^{18}$ — Impossible!).
- **Bitmask DP:** $\text{dp}[\text{mask}][u]$ = Min cost having visited the set of cities in `mask`, currently standing at city $u$.
- **Complexity:** **$O(N^2 \cdot 2^N)$ Time and $O(N \cdot 2^N)$ Space** (Solves $N = 20$ in $\approx 20^2 \times 10^6 \approx 4 \times 10^8$ operations).

```cpp
// TSP with Bitmask DP: O(N^2 * 2^N)
int tsp(int mask, int u, int n, const vector<vector<int>>& dist, vector<vector<int>>& memo) {
    if (mask == (1 << n) - 1) return dist[u][0]; // Return to start city
    if (memo[mask][u] != -1) return memo[mask][u];
    
    int minCost = 1e9;
    for (int v = 0; v < n; v++) {
        if (!((mask >> v) & 1)) { // If city v has not been visited
            int cost = dist[u][v] + tsp(mask | (1 << v), v, n, dist, memo);
            minCost = min(minCost, cost);
        }
    }
    return memo[mask][u] = minCost;
}
```

---

# 5. Quick Recap

- **Two's Complement:** $-X = \sim X + 1$.
- **Lowest Set Bit:** `n & (-n)` isolates lowest 1; `n & (n - 1)` clears lowest 1.
- **Power of 2 Check:** `n > 0 && !(n & (n - 1))`.
- **XOR Identity:** $x \oplus x = 0$ and $x \oplus 0 = x$ (enables finding single unique numbers in $O(N)$ time and $O(1)$ space).
- **Submask Iteration Trick:** `s = (s - 1) & mask` iterates all submasks in $O(3^N)$ total operations.
- **Bitmask DP Power:** Reduces factorial combinatorial searches ($O(N!)$) to exponential state-space DP ($O(N^2 2^N)$).
