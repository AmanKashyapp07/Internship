# Section 05: Hashing

> **Focus:** Hash table architecture, collision resolution strategies (Chaining vs. Open Addressing), Load Factor dynamics, dynamic rehashing, standard library implementations (`unordered_map` vs. `map`), and core algorithmic hashing patterns.

---

# Table of Contents
1. [Hash Tables & Hash Functions](#1-hash-tables--hash-functions)
2. [Collision Resolution Strategies](#2-collision-resolution-strategies)
3. [Load Factor ($\alpha$) & Dynamic Rehashing](#3-load-factor-alpha--dynamic-rehashing)
4. [HashMap vs. TreeMap (`std::unordered_map` vs. `std::map`)](#4-hashmap-vs-treemap)
5. [Core Algorithmic Hashing Patterns](#5-core-algorithmic-hashing-patterns)
6. [Security Vulnerabilities & Engineering Gotchas](#6-security-vulnerabilities--engineering-gotchas)
7. [Quick Recap](#7-quick-recap)

---

# 1. Hash Tables & Hash Functions

A **Hash Table** is a data structure that implements an associative array abstract data type, mapping **Keys to Values**. By using a mathematical **Hash Function**, it computes an integer array index from an arbitrary key, enabling **average $O(1)$ constant-time** insertions, lookups, and deletions.

```
Key ("user_104") ---> [ HASH FUNCTION: hash(key) ] ---> Hash Code (0x8F3A2C10)
                                                                 |
                      [ COMPRESSION: index = code % Capacity ] <--+
                                 |
                                 v
                             Index: 4 ---> Bucket[4]: { "user_104", "Aman", Balance: 500 }
```

---

### Anatomy of a Hash Function
A cryptographic hash (SHA-256) is designed to be one-way and uninvertible. In contrast, a **Data Structure Hash Function** is designed for **maximum throughput and uniform bucket distribution**:
1. **Deterministic:** The exact same key must *always* produce the exact same integer hash code within a program run.
2. **Uniform Distribution:** Keys must spread evenly across all $M$ slots to minimize clustering and collisions.
3. **Fast Computation:** Must execute in $O(1)$ time relative to overall algorithmic operations ($O(L)$ for key length $L$).

#### Compression Function (Mapping Hash Code $\to$ Array Index)
- **Division Method:** $\text{index} = \text{hashCode} \pmod M$ (where $M$ is the prime capacity of the hash table).
- **Bitwise Masking (Power of 2):** When $M = 2^k$, $\text{index} = \text{hashCode} \ \& \ (M - 1)$ (used by Java `HashMap` because bitwise AND is significantly faster than CPU modulo arithmetic).

---

### Interview Follow-Up Questions
1. **"Why should hash table bucket capacities ideally be prime numbers when using modulo division?"**
   - *Answer:* Prime table sizes reduce clustering if the input key distribution shares common arithmetic factors or strides with the table size, ensuring keys utilize all hash buckets uniformly.

---

# 2. Collision Resolution Strategies

Because the domain of possible keys (infinite) is vastly larger than the number of table buckets $M$, the **Pigeonhole Principle** dictates that two distinct keys *will eventually* hash to the same bucket index ($h(k_1) = h(k_2)$). This is a **Hash Collision**.

```
Collision Resolution Taxonomy:
1. Separate Chaining (Open Hashing): Elements outside table in linked nodes / trees.
2. Open Addressing (Closed Hashing):  All elements stored directly inside the fixed array.
```

---

### A. Separate Chaining (Open Hashing)
Each bucket in the table array points to a **Linked List** (or balanced BST) of key-value entries that share the same hash index.

```
Index | Buckets (Linked Lists)
---------------------------------------------
[ 0 ] | ---> NULL
[ 1 ] | ---> [ "apple": 5 ] ---> [ "cat": 2 ] ---> NULL (Collision Chained!)
[ 2 ] | ---> [ "banana": 8 ] ---> NULL
[ 3 ] | ---> NULL
```

#### Modern Java 8+ HashMap Optimization (Treeify Threshold):
- If many keys collide in the same bucket and the bucket linked list length reaches **$\ge 8$ elements**, Java 8 automatically transforms that individual linked list into a **Red-Black Tree**!
- This upgrades worst-case bucket lookup time from **$O(N)$ linear scan to guaranteed $O(\log N)$**.
- If deletions reduce the bucket size to $\le 6$, it converts back to a lightweight linked list ("Untreeify").

---

### B. Open Addressing (Closed Hashing)
In Open Addressing, all key-value pairs are stored **directly inside the table array itself** (no external heap pointer nodes). When a collision occurs at index $h(k)$, the algorithm systematically probes subsequent array slots using a **Probe Sequence** $h(k, i)$ until an empty slot is found.

```
+-----------------------------------------------------------------------------------------+
| 1. LINEAR PROBING                                                                       |
| Formula: h(k, i) = (h'(k) + i) % M                                                      |
| Checks: h'(k), h'(k) + 1, h'(k) + 2, h'(k) + 3...                                       |
| Flaw: PRIMARY CLUSTERING (Long contiguous blocks of occupied cells form, causing        |
| lookup times to degrade drastically to O(N)).                                           |
+-----------------------------------------------------------------------------------------+
| 2. QUADRATIC PROBING                                                                    |
| Formula: h(k, i) = (h'(k) + c1*i + c2*i^2) % M                                          |
| Flaw: SECONDARY CLUSTERING (Keys sharing the same initial hash follow identical probes).|
+-----------------------------------------------------------------------------------------+
| 3. DOUBLE HASHING                                                                       |
| Formula: h(k, i) = (h1(k) + i * h2(k)) % M                                              |
| Mechanics: Uses a second independent hash function h2(k) as the probe stride step!      |
| Eliminates all clustering. Requirement: h2(k) must be relatively prime to M.            |
+-----------------------------------------------------------------------------------------+
```

```
Double Hashing Probe Example:
Let M = 11, h1(k) = k % 11, h2(k) = 7 - (k % 7).
Insert Key 48:
h1(48) = 48 % 11 = 4 (Collision at slot 4!)
Step stride h2(48) = 7 - (48 % 7) = 7 - 6 = 1.
Probe 1: (4 + 1*1) % 11 = 5 (If empty -> Insert at slot 5!).
```

---

### The Critical Open Addressing Requirement: "Tombstones" (`DELETED` Markers)
In Open Addressing, you **cannot simply erase a slot to `NULL` on deletion**:
- If keys $A$ and $B$ collide and $B$ is probed and placed into slot 5 because $A$ occupies slot 4:
- Deleting $A$ by setting slot 4 to `NULL` breaks the probe search chain!
- Subsequent searches for $B$ will inspect slot 4, see `NULL`, and falsely conclude $B$ does not exist.
- **Solution:** Deletions write a special **`TOMBSTONE` (`DELETED`)** sentinel flag. Searches continue past `TOMBSTONE` markers, while new insertions can overwrite `TOMBSTONE` slots.

---

### Separate Chaining vs. Open Addressing Comparison

| Feature | Separate Chaining | Open Addressing |
| :--- | :--- | :--- |
| **Memory Storage** | Nodes allocated on Heap via pointers. | Entirely contiguous array in memory. |
| **CPU Cache Performance** | Lower (pointer chasing across heap). | **High** (linear probing maximizes CPU cache line hits). |
| **Max Load Factor ($\alpha$)** | Can exceed $1.0$ ($\alpha > 1$ simply lengthens lists). | **Must stay $< 1.0$** (Typically capped at $\alpha \le 0.7$). |
| **Deletion Complexity** | Simple node unlinking ($O(1)$). | Requires `TOMBSTONE` markers and cleanup. |

---

### Interview Follow-Up Questions
1. **"What is Robin Hood Hashing?"**
   - *Answer:* An open addressing variant where an incoming key can steal a slot from an already-inserted key if the incoming key has traveled farther from its initial hash index ("take from the rich, give to the poor"). It dramatically reduces search variance and maximum probe length.
2. **"What is Cuckoo Hashing?"**
   - *Answer:* Uses 2 distinct hash functions and 2 separate tables. A key always resides at either `table1[h1(k)]` or `table2[h2(k)]`, guaranteeing **strictly $O(1)$ worst-case lookup time**. Insertions displace existing keys like a cuckoo bird, kicking them to their alternative table.

---

# 3. Load Factor ($\alpha$) & Dynamic Rehashing

The **Load Factor ($\alpha$)** is the fundamental metric measuring how densely populated a hash table is:

$$\alpha = \frac{N}{M} = \frac{\text{Total Stored Elements}}{\text{Total Bucket Capacity}}$$

```
Load Factor Threshold Dynamics:
Low Load Factor (alpha < 0.2):  Wastes RAM memory (mostly empty buckets).
High Load Factor (alpha > 0.8): Severe collision frequency -> Operations degrade from O(1) to O(N)!

Optimal Trade-off Thresholds:
- Separate Chaining: Capped at alpha = 0.75 (Default in Java HashMap / C++ unordered_map).
- Open Addressing:   Capped at alpha = 0.5 to 0.65 (to avoid long probe chains).
```

---

### Dynamic Rehashing Mechanics
When the number of stored elements reaches $N = \alpha_{\text{threshold}} \times M$:
1. Allocate a brand-new bucket array of **double the capacity** ($2 \times M$, ideally next prime).
2. Iterate through all $N$ elements in the old table.
3. Recompute `new_index = hashCode % (2*M)` for every single element and insert into the new array.
4. Deallocate the old bucket array memory.

```cpp
// Amortized Complexity Analysis of Rehashing:
// Individual Resize Operation Cost: O(N) Time
// Frequency: Happens after 1, 2, 4, 8, 16 ... N insertions.
// Sum of Rehashing Work: 1 + 2 + 4 + ... + N = 2N = O(N) Total Cost.
// Amortized Cost per Insertion: O(N) / N = O(1) Amortized Time!
```

---

### Interview Follow-Up Questions
1. **"Why can't we simply copy the old bucket array directly into the first half of the new $2M$ array during resizing?"**
   - *Answer:* Because the array index calculation depends directly on table capacity: $\text{index} = \text{hashCode} \pmod M$. When capacity changes to $2M$, an element with $\text{hashCode} = 15$ in a table of size 10 was at index $15 \pmod{10} = 5$; in a table of size 20, its new index becomes $15 \pmod{20} = 15$. Every element must be rehashed.

---

# 4. HashMap vs. TreeMap

```
+---------------------------------------------------------------------------------------------------+
| DIMENSION               | std::unordered_map (HashMap)       | std::map (TreeMap)                 |
+---------------------------------------------------------------------------------------------------+
| Underlying Engine       | Hash Table (Separate Chaining)     | Red-Black Tree (Self-Balancing BST)|
| Average Time Complexity | O(1) (Insert, Delete, Search)      | O(log N) (Insert, Delete, Search)  |
| Worst Time Complexity   | O(N) (Severe hash collisions)      | O(log N) (Guaranteed strict bound) |
| Element Ordering        | Arbitrary / Unordered              | Strictly Sorted by Key (In-Order)  |
| Key Requirements        | Must implement hash function       | Must implement operator< (Strict   |
|                         | and equality operator==            | Weak Ordering comparator)          |
| Range Query Capability  | NONE (Requires O(N) full scan)     | Fast O(log N) via lower_bound() /  |
|                         |                                    | upper_bound()                      |
+---------------------------------------------------------------------------------------------------+
```

---

### Interview Follow-Up Questions
1. **"When should you choose `std::map` (TreeMap) over `std::unordered_map` (HashMap)?"**
   - *Answer:*
     1. When you need data sorted in ascending/descending order at all times.
     2. When performing range searches (e.g. find all timestamps between $T_1$ and $T_2$ via `lower_bound`).
     3. When deterministic $O(\log N)$ latency is required (avoiding hash collision latency spikes).

---

# 5. Core Algorithmic Hashing Patterns

### Pattern 1: Prefix Sum + Hash Map (Subarray Sum Equals K)
- **Problem:** Count total continuous subarrays whose sum equals $K$ in $O(N)$ time.
- **Mechanics:** Running sum $S_i = \sum_{j=0}^i \text{arr}[j]$. If there exists a previous prefix sum $S_j$ such that $S_i - S_j = K \implies S_j = S_i - K$, then the subarray between $j+1$ and $i$ sums to $K$.

```cpp
// Subarray Sum Equals K: O(N) Time, O(N) Space
int subarraySum(vector<int>& nums, int k) {
    unordered_map<int, int> prefixFreq;
    prefixFreq[0] = 1; // Base case: prefix sum of 0 occurs once
    int runningSum = 0, totalCount = 0;
    
    for (int num : nums) {
        runningSum += num;
        if (prefixFreq.count(runningSum - k)) {
            totalCount += prefixFreq[runningSum - k];
        }
        prefixFreq[runningSum]++;
    }
    return totalCount;
}
```

---

### Pattern 2: Longest Consecutive Sequence in $O(N)$ Time
- **Problem:** Find the length of the longest consecutive elements sequence (`[100, 4, 200, 1, 3, 2]` $\implies$ `[1, 2, 3, 4]` length 4) without sorting!
- **Mechanics:** Insert all numbers into an `unordered_set`. Iterate through the set; only start counting a sequence if `num - 1` is **NOT** in the set (guaranteeing `num` is the true start of a streak).

```cpp
// Longest Consecutive Sequence: Strictly O(N) Time
int longestConsecutive(vector<int>& nums) {
    unordered_set<int> numSet(nums.begin(), nums.end());
    int maxStreak = 0;
    
    for (int num : numSet) {
        // Only start sequence if num is the start of a streak!
        if (!numSet.count(num - 1)) {
            int currentNum = num;
            int currentStreak = 1;
            
            while (numSet.count(currentNum + 1)) {
                currentNum++;
                currentStreak++;
            }
            maxStreak = max(maxStreak, currentStreak);
        }
    }
    return maxStreak; // Every number visited at most twice -> O(N) total!
}
```

---

# 6. Security Vulnerabilities & Engineering Gotchas

### A. Hash Collision Denial of Service (HashDoS Attack)
- **The Vulnerability:** If a web server parses user-submitted JSON headers or POST body parameters into a standard hash table using a deterministic hash function:
- An attacker crafts thousands of specific keys that intentionally hash to the **exact same bucket index**.
- The server's hash table degrades from $O(1)$ to **$O(N^2)$ worst-case linear chaining**, driving CPU usage to 100% and taking down the server.
- **The Production Fix:** Use randomized, cryptographically robust hash functions with random per-process salt seeds (e.g. **SipHash** in Python, Rust, and modern Linux kernels).

---

### B. The Mutable Object Key Gotcha
- **The Bug:** Inserting a mutable object into a `HashMap` or `HashSet` and subsequently modifying the object's properties:
- Modifying the object alters its `hashCode()`.
- When you later attempt to call `map.get(object)` or `set.contains(object)`, the table calculates the *new* hash code, looks in the wrong bucket slot, and **fails to find the element**—creating invisible memory leaks and zombie entries!
- **The Rule:** In all languages, **Keys in Hash Tables MUST be strictly immutable** (primitives, strings, frozen tuples).

---

# 7. Quick Recap

- **Hash Function Goal:** Deterministic, uniform distribution across $M$ buckets, fast $O(1)$ evaluation.
- **Separate Chaining vs. Open Addressing:** Chaining uses linked lists (converting to Red-Black trees in Java at $\ge 8$ items); Open Addressing probes internal array slots.
- **Open Addressing Deletions:** Mandatory to use `TOMBSTONE` sentinel markers to prevent breaking linear/quadratic probe search chains.
- **Load Factor Threshold:** $\alpha = N / M$. When $\alpha \ge 0.75$, dynamic rehashing allocates $2M$ space and recomputes all element indices in amortized $O(1)$ time.
- **HashMap vs. TreeMap:** `unordered_map` is $O(1)$ average and unordered; `map` is $O(\log N)$ guaranteed and strictly sorted via Red-Black Tree.
- **HashDoS Mitigation:** Production systems require keyed, randomized hash algorithms like **SipHash** to prevent worst-case $O(N^2)$ collision attacks.
