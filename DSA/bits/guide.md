# Bit Manipulation
## Master Interview & OA Guide (C++)

---

## Why Bit Manipulation

- Replaces expensive operations with single CPU instructions
- Solves subset / state problems in O(1) space per state
- Often the "clever" O(n) solution to what looks like an O(n^2) problem
- Shows up in bitmask DP, XOR tricks, and "single number" problems

---

# PART 1: Fundamentals

## Bit Operators in C++

```cpp
a & b    // AND  — bit is 1 only if BOTH bits are 1
a | b    // OR   — bit is 1 if AT LEAST ONE bit is 1
a ^ b    // XOR  — bit is 1 if bits are DIFFERENT
~a       // NOT  — flip all bits
a << k   // left shift  — multiply by 2^k
a >> k   // right shift — divide by 2^k (arithmetic for signed ints)
```

## Truth Table

```
a  b  |  a&b  a|b  a^b
0  0  |   0    0    0
0  1  |   0    1    1
1  0  |   0    1    1
1  1  |   1    1    0
```

## Reading a Number in Binary

```cpp
// n = 13 = 1101 in binary
//   bit 3 = 1
//   bit 2 = 1
//   bit 1 = 0
//   bit 0 = 1

// Check bit i (0-indexed from right)
bool bit_i_set = (n >> i) & 1;

// Example: check all bits of n
for (int i = 31; i >= 0; i--)
    cout << ((n >> i) & 1);
```

## Powers of Two

```
2^0  = 1
2^1  = 2
2^2  = 4
2^3  = 8
2^4  = 16
2^10 = 1024
2^20 = 1,048,576  (~1 million)
2^30 = 1,073,741,824  (~1 billion)
2^31 - 1 = INT_MAX = 2,147,483,647
```

---

# PART 2: Bit Tricks — The Core Toolkit

Every bit manipulation problem uses one or more of these 15 tricks.

## Trick 1: Check if bit i is set

```cpp
bool is_set = (n >> i) & 1;
// Shift n right by i positions, then AND with 1
// If bit i was 1, result is 1. Otherwise 0.
```

## Trick 2: Set bit i (turn it ON)

```cpp
n = n | (1 << i);
// OR with a mask that has only bit i set
```

## Trick 3: Clear bit i (turn it OFF)

```cpp
n = n & ~(1 << i);
// AND with a mask that has all bits 1 EXCEPT bit i
```

## Trick 4: Toggle bit i (flip it)

```cpp
n = n ^ (1 << i);
// XOR with 1 flips the bit: 0 becomes 1, 1 becomes 0
```

## Trick 5: Check if n is a power of two

```cpp
bool isPow2 = (n > 0) && (n & (n - 1)) == 0;
// Powers of 2: 0001, 0010, 0100, 1000
// n - 1:       0000, 0001, 0011, 0111
// n & (n-1) is always 0 for powers of 2
// MUST check n > 0 separately (0 & -1 = 0 but 0 is not a power of 2)
```

## Trick 6: Remove the lowest set bit

```cpp
n = n & (n - 1);
// n     = ...1010 0  (some bit pattern ending with rightmost 1)
// n - 1 = ...1001 1  (borrows from the rightmost 1, flips it and bits below)
// n & (n-1) clears the rightmost 1

// Use case: count set bits
int count = 0;
while (n) {
    n &= (n - 1); // remove lowest set bit
    count++;
}
```

## Trick 7: Isolate (extract) the lowest set bit

```cpp
int lowest = n & (-n);
// -n in two's complement = ~n + 1
// n     = ...0110 1000
// -n    = ...1001 1000
// n&(-n)= ...0000 1000  (only the lowest set bit survives)

// Use case: Fenwick Tree (BIT) index traversal
// Also: iterate over all set bits
while (n) {
    int bit = n & (-n);  // isolate lowest bit
    // process bit
    n ^= bit;            // remove it (same as n &= n-1)
}
```

## Trick 8: Count set bits (popcount)

```cpp
// Method 1: built-in (fastest)
int cnt = __builtin_popcount(n);    // for int
int cnt = __builtin_popcountll(n);  // for long long

// Method 2: Brian Kernighan
int cnt = 0;
while (n) { n &= n - 1; cnt++; }

// Method 3: loop all bits
int cnt = 0;
for (int i = 0; i < 32; i++) cnt += (n >> i) & 1;
```

## Trick 9: Position of lowest set bit

```cpp
int pos = __builtin_ctz(n);    // count trailing zeros
// __builtin_ctz(12) = __builtin_ctz(1100) = 2

int pos = __builtin_ctzll(n);  // for long long
```

## Trick 10: Position of highest set bit

```cpp
int pos = 31 - __builtin_clz(n);   // 31 - count leading zeros
// __builtin_clz(12) = __builtin_clz(0...01100) = 28
// 31 - 28 = 3 (bit 3 is highest set bit of 12)

// floor(log2(n)) = 31 - __builtin_clz(n)  for n > 0
```

## Trick 11: XOR properties — the most useful rules

```cpp
a ^ a = 0       // XOR with itself = 0
a ^ 0 = a       // XOR with 0 = unchanged
a ^ b ^ a = b   // XOR is commutative and associative — a's cancel
a ^ b ^ b = a   // same: b's cancel

// Consequence: XOR all elements in a pair-cancel problem
// If every number appears twice except one, XOR all -> the lone number
```

## Trick 12: Swap without temp variable

```cpp
a ^= b;
b ^= a;
a ^= b;
// Works but is slower than using a temp on modern CPUs.
// Know it exists; don't use in production.
```

## Trick 13: Check if two integers have opposite signs

```cpp
bool opposite = (a ^ b) < 0;
// The sign bit (bit 31) is 1 for negative, 0 for positive.
// XOR of sign bits: 1 only if they differ.
```

## Trick 14: Multiply and divide by powers of 2

```cpp
n << k   // n * 2^k   (left shift)
n >> k   // n / 2^k   (right shift, rounds toward -inf for signed)
```

## Trick 15: Get / set a range of bits (bitmask)

```cpp
// Create a mask of k ones: (1 << k) - 1
int mask = (1 << k) - 1;
// k=3: (1000) - 1 = 0111

// Extract bits [lo, hi] (inclusive) from n
int extracted = (n >> lo) & ((1 << (hi - lo + 1)) - 1);
```

---

# PART 3: Built-in Functions Reference

```cpp
__builtin_popcount(n)     // count of set bits (int)
__builtin_popcountll(n)   // count of set bits (long long)
__builtin_ctz(n)          // count trailing zeros (position of lowest set bit)
__builtin_clz(n)          // count leading zeros
__builtin_parity(n)       // parity: 1 if odd number of set bits, 0 if even

// All undefined for n = 0 except popcount.
// Always guard: if (n == 0) handle separately.
```

---

# PART 4: Patterns and Problems

## Pattern A: XOR to Find Unique Elements

XOR cancels pairs. Anything appearing an even number of times vanishes.

```cpp
// LC 136. Single Number
// Every number appears twice except one. Find it.
int singleNumber(vector<int>& nums) {
    int result = 0;
    for (int x : nums) result ^= x;
    return result;
    // All pairs XOR to 0. The lone number remains.
}
```

```cpp
// LC 137. Single Number II
// Every number appears three times except one.
// Track bits that have appeared 1 time (ones) and 2 times (twos).
int singleNumber(vector<int>& nums) {
    int ones = 0, twos = 0;
    for (int x : nums) {
        ones = (ones ^ x) & ~twos;
        twos = (twos ^ x) & ~ones;
    }
    return ones;
    // ones holds bits that have appeared 1 mod 3 times.
}
```

```cpp
// LC 260. Single Number III
// Two numbers appear once; all others appear twice. Find both.
vector<int> singleNumber(vector<int>& nums) {
    int xorAll = 0;
    for (int x : nums) xorAll ^= x; // xorAll = a ^ b

    // Find any bit where a and b differ (a set bit in xorAll)
    int diffBit = xorAll & (-xorAll); // lowest set bit

    int a = 0;
    for (int x : nums)
        if (x & diffBit) a ^= x; // XOR numbers sharing this bit -> one of them

    return {a, xorAll ^ a};
}
```

---

## Pattern B: Bit Counting Problems

```cpp
// LC 191. Number of 1 Bits (Hamming Weight)
int hammingWeight(uint32_t n) {
    return __builtin_popcount(n);
    // Or: int c = 0; while(n) { n &= n-1; c++; } return c;
}
```

```cpp
// LC 338. Counting Bits
// For every number 0 to n, count set bits.
// DP: popcount(i) = popcount(i >> 1) + (i & 1)
vector<int> countBits(int n) {
    vector<int> dp(n + 1, 0);
    for (int i = 1; i <= n; i++)
        dp[i] = dp[i >> 1] + (i & 1);
    return dp;
    // i >> 1 drops the lowest bit; (i & 1) adds it back if it was set.
}
```

```cpp
// LC 461. Hamming Distance
// Number of positions where bits differ between x and y.
int hammingDistance(int x, int y) {
    return __builtin_popcount(x ^ y);
    // XOR gives 1 wherever bits differ. Count those bits.
}
```

```cpp
// LC 477. Total Hamming Distance
// Sum of hamming distances for every pair in the array.
// O(n^2) naive; O(32n) with bit-column approach.
int totalHammingDistance(vector<int>& nums) {
    int total = 0, n = nums.size();
    for (int i = 0; i < 32; i++) {
        int ones = 0;
        for (int x : nums) ones += (x >> i) & 1;
        total += ones * (n - ones); // ones paired with zeros
    }
    return total;
}
```

---

## Pattern C: Bit Manipulation on Integers

```cpp
// LC 7. Reverse Bits
uint32_t reverseBits(uint32_t n) {
    uint32_t result = 0;
    for (int i = 0; i < 32; i++) {
        result = (result << 1) | (n & 1);
        n >>= 1;
    }
    return result;
}
```

```cpp
// LC 268. Missing Number
// Array has n numbers from 0 to n with one missing. Find it.
int missingNumber(vector<int>& nums) {
    int result = nums.size();
    for (int i = 0; i < nums.size(); i++)
        result ^= i ^ nums[i];
    return result;
    // XOR all indices 0..n with all values. Present numbers cancel.
    // Alternative: result = n*(n+1)/2 - sum(nums)
}
```

```cpp
// LC 371. Sum of Two Integers (without + or -)
int getSum(int a, int b) {
    while (b != 0) {
        unsigned carry = (unsigned)(a & b) << 1; // carry bits, shift left
        a = a ^ b;                                // sum without carry
        b = carry;
    }
    return a;
    // Repeat until no carry remains.
}
```

```cpp
// LC 201. Bitwise AND of Numbers Range
// AND of all numbers in [left, right]
// Key insight: find the common prefix of left and right in binary
int rangeBitwiseAnd(int left, int right) {
    int shift = 0;
    while (left != right) {
        left >>= 1;
        right >>= 1;
        shift++;
    }
    return left << shift;
    // Bits that differ between left and right will be AND'd to 0.
    // Only the common prefix survives.
}
```

---

## Pattern D: Bitmask for Subsets

With n items, there are 2^n subsets. Each subset is represented by an
integer where bit i being set means item i is included.

```cpp
// Enumerate all subsets of nums
int n = nums.size();
for (int mask = 0; mask < (1 << n); mask++) {
    vector<int> subset;
    for (int i = 0; i < n; i++)
        if (mask >> i & 1)
            subset.push_back(nums[i]);
    // process subset
}
```

```cpp
// Enumerate all subsets of a MASK (sub-masks)
// Used in bitmask DP over subsets
for (int sub = mask; sub > 0; sub = (sub - 1) & mask) {
    // process sub as a subset of mask
}
// Total iterations over all masks: 3^n (each bit can be in mask not in sub,
// in mask and in sub, or not in mask at all)
```

```cpp
// LC 78. Subsets using bitmask
vector<vector<int>> subsets(vector<int>& nums) {
    int n = nums.size();
    vector<vector<int>> result;
    for (int mask = 0; mask < (1 << n); mask++) {
        vector<int> subset;
        for (int i = 0; i < n; i++)
            if (mask >> i & 1)
                subset.push_back(nums[i]);
        result.push_back(subset);
    }
    return result;
}
```

```cpp
// LC 1986 / general: check if a set covers all required items
// Represent required as a bitmask, check if covered mask == required
int required = (1 << m) - 1; // all m bits set
if ((covered & required) == required) {
    // all requirements met
}
```

---

## Pattern E: XOR for Prefix / Range Queries

XOR has the property: `prefix[l..r] = prefix[0..r] ^ prefix[0..l-1]`
Same as prefix sum but for XOR.

```cpp
// Build XOR prefix array
vector<int> prefix(n + 1, 0);
for (int i = 0; i < n; i++)
    prefix[i + 1] = prefix[i] ^ nums[i];

// XOR of range [l, r] (0-indexed)
int xorRange = prefix[r + 1] ^ prefix[l];
```

```cpp
// LC 1310. XOR Queries of a Subarray
vector<int> xorQueries(vector<int>& arr, vector<vector<int>>& queries) {
    int n = arr.size();
    vector<int> prefix(n + 1, 0);
    for (int i = 0; i < n; i++) prefix[i+1] = prefix[i] ^ arr[i];

    vector<int> result;
    for (auto& q : queries)
        result.push_back(prefix[q[1]+1] ^ prefix[q[0]]);
    return result;
}
```

---

## Pattern F: Bitmask DP

Track which items have been used in a compact integer state.
Only feasible for n <= 20 (2^20 = ~1 million states).

```cpp
// LC 847. Shortest Path Visiting All Nodes
// BFS where state = (current_node, visited_mask)
int shortestPathLength(vector<vector<int>>& graph) {
    int n = graph.size();
    int full = (1 << n) - 1; // all nodes visited

    queue<tuple<int,int,int>> q; // (node, mask, dist)
    vector<vector<bool>> visited(n, vector<bool>(1 << n, false));

    for (int i = 0; i < n; i++) {
        q.push({i, 1 << i, 0});
        visited[i][1 << i] = true;
    }

    while (!q.empty()) {
        auto [node, mask, dist] = q.front(); q.pop();
        if (mask == full) return dist;
        for (int next : graph[node]) {
            int newMask = mask | (1 << next);
            if (!visited[next][newMask]) {
                visited[next][newMask] = true;
                q.push({next, newMask, dist + 1});
            }
        }
    }
    return -1;
}
```

```cpp
// LC 1125. Smallest Sufficient Team
// Find smallest team that covers all required skills
// dp[skill_mask] = smallest team achieving this skill coverage
vector<int> smallestSufficientTeam(vector<string>& req_skills,
                                    vector<vector<string>>& people) {
    int m = req_skills.size();
    unordered_map<string, int> skillIdx;
    for (int i = 0; i < m; i++) skillIdx[req_skills[i]] = i;

    int full = (1 << m) - 1;
    vector<vector<int>> dp(1 << m); // dp[mask] = team for this skill mask
    dp[0] = {};
    for (int mask = 1; mask <= full; mask++)
        dp[mask].resize(INT_MAX); // placeholder for "not found"

    for (int p = 0; p < people.size(); p++) {
        int skill = 0;
        for (auto& s : people[p])
            if (skillIdx.count(s)) skill |= (1 << skillIdx[s]);

        for (int mask = full; mask >= 0; mask--) {
            if (dp[mask].empty() && mask != 0) continue;
            int newMask = mask | skill;
            if (dp[newMask].size() > dp[mask].size() + 1) {
                dp[newMask] = dp[mask];
                dp[newMask].push_back(p);
            }
        }
    }
    return dp[full];
}
```

---

## Pattern G: XOR Tricks for Arrays

```cpp
// LC 421. Maximum XOR of Two Numbers in an Array
// Build a trie of binary representations; for each number, greedily pick opposite bit
int findMaximumXOR(vector<int>& nums) {
    int max_xor = 0, mask = 0;
    for (int i = 31; i >= 0; i--) {
        mask |= (1 << i);
        unordered_set<int> prefixes;
        for (int x : nums) prefixes.insert(x & mask);

        int candidate = max_xor | (1 << i);
        for (int prefix : prefixes) {
            if (prefixes.count(candidate ^ prefix)) {
                max_xor = candidate;
                break;
            }
        }
    }
    return max_xor;
}
```

```cpp
// LC 1442. Count Triplets That Can Form Two Equal XOR Arrays
// XOR of range [i+1, k] == 0 means a ^ b == 0, so arr[i] == arr[k]
// For each pair (i, k) where prefix[i] == prefix[k+1]:
//   all j in (i, k) are valid -> contributes (k - i) triplets
int countTriplets(vector<int>& arr) {
    int n = arr.size(), count = 0;
    vector<int> prefix(n + 1, 0);
    for (int i = 0; i < n; i++) prefix[i+1] = prefix[i] ^ arr[i];

    for (int i = 0; i < n; i++)
        for (int k = i + 1; k < n; k++)
            if (prefix[i] == prefix[k+1])
                count += k - i;
    return count;
}
```

---

# PART 5: Number Theory Bit Tricks

## Two's Complement (how negative numbers work)

```
Positive n:  stored normally
Negative n:  flip all bits of |n|, then add 1

Example: -5 in 8-bit
|5|  = 0000 0101
flip = 1111 1010
+1   = 1111 1011  <- this is -5

Key facts:
  -n = ~n + 1
  n + (~n) = -1 (all ones)
  n & (-n) = lowest set bit  (because -n = ~n + 1 flips lower bits and sets the lowest 1)
```

## Integer Overflow Traps

```cpp
1 << 31         // undefined behavior for signed int (overflow)
1u << 31        // correct: use unsigned
1LL << 31       // correct: use long long
(1 << 31) - 1   // INT_MAX: 2,147,483,647
(1 << 30)       // 1,073,741,824 — this is fine (bit 30, not 31)

// Safe check for n <= 20 bitmask DP:
1 << 20         // = 1,048,576 — fine as int
1 << 25         // = 33,554,432 — fine as int
1 << 31         // OVERFLOW for signed int — use 1u << 31 or 1LL << 31
```

## XOR-Swap Order Matters

```cpp
// WRONG when a and b point to the same memory:
a ^= b; b ^= a; a ^= b; // if a == b (same address), becomes 0

// Always use a temp variable for swapping in practice.
```

---

# PART 6: Quick Reference Table

## Bit Operations Summary

| Goal | Code | Notes |
|---|---|---|
| Check bit i | `(n >> i) & 1` | Returns 0 or 1 |
| Set bit i | `n \| (1 << i)` | Turn on |
| Clear bit i | `n & ~(1 << i)` | Turn off |
| Toggle bit i | `n ^ (1 << i)` | Flip |
| Is power of 2? | `n > 0 && (n & n-1) == 0` | Must check n > 0 |
| Remove lowest set bit | `n & (n - 1)` | Also: check if power of 2 |
| Isolate lowest set bit | `n & (-n)` | Used in Fenwick tree |
| Count set bits | `__builtin_popcount(n)` | O(1) |
| Position of lowest set bit | `__builtin_ctz(n)` | 0-indexed |
| Flip all bits | `~n` | Gives -(n+1) for signed int |
| Multiply by 2^k | `n << k` | |
| Divide by 2^k | `n >> k` | |
| Mask of k ones | `(1 << k) - 1` | k bits all set |
| All subsets of mask | `for(sub=mask; sub; sub=(sub-1)&mask)` | |
| XOR range [l, r] | `prefix[r+1] ^ prefix[l]` | |

## XOR Laws

```
x ^ 0  = x           identity
x ^ x  = 0           self-inverse
x ^ y  = y ^ x       commutative
(x^y)^z = x^(y^z)   associative
x ^ y ^ x = y        pair cancellation
```

---

# PART 7: Problem List

### XOR / Single Number
136, 137, 260, 268, 389, 1720

### Bit Counting
191, 338, 461, 477, 693

### Integer Bit Tricks
7 (reverse bits), 29 (divide without *//), 201, 371, 405

### Subsets via Bitmask
78, 90 (also backtracking), 1986

### Bitmask DP
847, 1125, 526, 698, 980, 1349

### XOR Prefix / Range
1310, 1442, 2588

### Maximum XOR
421, 1707 (XOR + trie)

### Miscellaneous
231 (power of 2), 342 (power of 4), 1009 (complement), 476 (complement)

---

# PART 8: Notes to Remember

**1. Always use `1LL << k` when k >= 31.**
`1 << 31` is undefined behavior for signed int. Write `1LL << k` or `1u << k` defensively.

**2. `n & (n-1)` is your most used trick.**
Removes the lowest set bit. Appears in: power-of-two check, popcount loop, many problems.

**3. XOR cancels pairs.**
If every element appears an even number of times except one, XOR all elements.
If every element appears 3 times except one, use the ones/twos trick.

**4. Bitmask DP limit: n <= 20.**
2^20 = ~1M states. 2^25 = ~33M (borderline). 2^30 = ~1B (too slow).

**5. `__builtin_popcount` is O(1).**
Use it freely. No need to write a loop unless the interviewer forbids built-ins.

**6. Sub-mask enumeration is O(3^n) total, not O(4^n).**
Iterating all sub-masks of all masks: `for mask: for sub of mask` is 3^n total
because each bit is either: not in mask (1 way), in mask but not in sub (1 way),
in both mask and sub (1 way) = 3 choices per bit = 3^n.

**7. `n & (-n)` isolates the lowest set bit.**
This is the Fenwick tree (BIT) index trick. Also useful for iterating over set bits.

**8. XOR prefix sums work exactly like regular prefix sums.**
`XOR(l, r) = prefix[r+1] ^ prefix[l]`. Build once, answer all queries in O(1).

**9. For bitmask DP, represent state as an integer.**
"Set of visited nodes" = bitmask. "Set of covered skills" = bitmask.
Check coverage: `(state & required) == required`.
Add item i: `state | (1 << i)`.
Remove item i: `state & ~(1 << i)`.

**10. Complement of n in its bit-length = `(1 << bit_length) - 1 ^ n`.**
Number of bits: `32 - __builtin_clz(n)`.
Complement: `((1 << (32 - __builtin_clz(n))) - 1) ^ n`.