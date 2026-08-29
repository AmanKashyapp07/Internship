# Microsoft Interview High-Frequency Delta Problems - Algorithmic Report

This report covers all **55 unique problems** from Section 1 (Last 30 Days), Section 2 (Last 3 Months), and Section 3 (Curated Tricky Easy Problems) of the Microsoft extension dataset.

Each entry contains the concise problem statement, core algorithmic approach, and time/space complexity.

---

## Table of Contents

### Section 1 & 2: Recent 30-Day and 3-Month Delta Problems
1. [509. Fibonacci Number (Easy)](#problem-509)
2. [451. Sort Characters By Frequency (Medium)](#problem-451)
3. [136. Single Number (Easy)](#problem-136)
4. [205. Isomorphic Strings (Easy)](#problem-205)
5. [645. Set Mismatch (Easy)](#problem-645)
6. [779. K-th Symbol in Grammar (Medium)](#problem-779)
7. [3532. Path Existence Queries in a Graph I (Medium)](#problem-3532)
8. [1288. Remove Covered Intervals (Medium)](#problem-1288)
9. [9. Palindrome Number (Easy)](#problem-9)
10. [7. Reverse Integer (Medium)](#problem-7)
11. [2858. Minimum Edge Reversals So Every Node Is Reachable (Hard)](#problem-2858)
12. [863. All Nodes Distance K in Binary Tree (Medium)](#problem-863)
13. [6. Zigzag Conversion (Medium)](#problem-6)
14. [1353. Maximum Number of Events That Can Be Attended (Medium)](#problem-1353)
15. [85. Maximal Rectangle (Hard)](#problem-85)
16. [17. Letter Combinations of a Phone Number (Medium)](#problem-17)
17. [475. Heaters (Medium)](#problem-475)
18. [204. Count Primes (Medium)](#problem-204)
19. [1838. Frequency of the Most Frequent Element (Medium)](#problem-1838)
20. [36. Valid Sudoku (Medium)](#problem-36)
21. [137. Single Number II (Medium)](#problem-137)
22. [274. H-Index (Medium)](#problem-274)
23. [992. Subarrays with K Different Integers (Hard)](#problem-992)
24. [130. Surrounded Regions (Medium)](#problem-130)
25. [2161. Partition Array According to Given Pivot (Medium)](#problem-2161)
26. [30. Substring with Concatenation of All Words (Hard)](#problem-30)
27. [1283. Find the Smallest Divisor Given a Threshold (Medium)](#problem-1283)
28. [526. Beautiful Arrangement (Medium)](#problem-526)
29. [3761. Minimum Absolute Distance Between Mirror Pairs (Medium)](#problem-3761)
30. [827. Making A Large Island (Hard)](#problem-827)
31. [802. Find Eventual Safe States (Medium)](#problem-802)
32. [684. Redundant Connection (Medium)](#problem-684)
33. [525. Contiguous Array (Medium)](#problem-525)
34. [373. Find K Pairs with Smallest Sums (Medium)](#problem-373)
35. [223. Rectangle Area (Medium)](#problem-223)
36. [1932. Merge BSTs to Create Single BST (Hard)](#problem-1932)
37. [3699. Number of ZigZag Arrays I (Hard)](#problem-3699)
38. [3946. Maximum Number of Items From Sale I (Medium)](#problem-3946)

### Section 3: High-Yield Tricky Easy Problems
39. [202. Happy Number (Easy)](#problem-202)
40. [387. First Unique Character in a String (Easy)](#problem-387)
41. [168. Excel Sheet Column Title (Easy)](#problem-168)
42. [171. Excel Sheet Column Number (Easy)](#problem-171)
43. [1071. Greatest Common Divisor of Strings (Easy)](#problem-1071)
44. [405. Convert a Number to Hexadecimal (Easy)](#problem-405)
45. [844. Backspace String Compare (Easy)](#problem-844)
46. [977. Squares of a Sorted Array (Easy)](#problem-977)
47. [1047. Remove All Adjacent Duplicates In String (Easy)](#problem-1047)
48. [724. Find Pivot Index (Easy)](#problem-724)
49. [290. Word Pattern (Easy)](#problem-290)
50. [190. Reverse Bits (Easy)](#problem-190)
51. [191. Number of 1 Bits (Easy)](#problem-191)
52. [268. Missing Number (Easy)](#problem-268)
53. [338. Counting Bits (Easy)](#problem-338)
54. [680. Valid Palindrome II (Easy)](#problem-680)
55. [953. Verifying an Alien Dictionary (Easy)](#problem-953)

---

### <a id="problem-509"></a>1. Fibonacci Number (LeetCode 509)

- **Difficulty**: Easy
- **Practice Link**: [Fibonacci Number](https://leetcode.com/problems/fibonacci-number)

#### Problem Statement
Given integer n, calculate F(n) where F(0)=0, F(1)=1, and F(n) = F(n-1) + F(n-2) for n >= 2.

#### Algorithmic Solution
1. Base cases: If n <= 1, return n.
2. Initialize two variables `prev2 = 0` (F0) and `prev1 = 1` (F1).
3. Loop from i = 2 up to n:
   a. Compute `curr = prev1 + prev2`.
   b. Shift states: `prev2 = prev1`, `prev1 = curr`.
4. Return `prev1`.
(Alternative: For huge n > 10^9, use 2x2 Matrix Exponentiation [[1,1],[1,0]]^(n-1) in O(log n)).

- **Time Complexity**: O(N) (or O(log N) with Matrix Exp)
- **Space Complexity**: O(1)

---

### <a id="problem-451"></a>2. Sort Characters By Frequency (LeetCode 451)

- **Difficulty**: Medium
- **Practice Link**: [Sort Characters By Frequency](https://leetcode.com/problems/sort-characters-by-frequency)

#### Problem Statement
Given a string s, sort it in decreasing order based on the frequency of the characters. Return the sorted string.

#### Algorithmic Solution
1. Count character frequencies using an unordered_map<char, int> or int count[128].
2. Create an array of buckets `vector<string> bucket(s.size() + 1)` where bucket[f] stores all characters appearing f times.
3. Populate buckets: For each (char, freq) in the map, append `string(freq, char)` to bucket[freq].
4. Build result: Iterate through buckets backwards from freq = s.size() down to 1, concatenating each non-empty string.
5. Return the resulting string.

- **Time Complexity**: O(N) (Bucket Sort avoids O(N log N))
- **Space Complexity**: O(N)

---

### <a id="problem-136"></a>3. Single Number (LeetCode 136)

- **Difficulty**: Easy
- **Practice Link**: [Single Number](https://leetcode.com/problems/single-number)

#### Problem Statement
Given a non-empty array of integers nums, every element appears twice except for one. Find that single one in O(N) time and O(1) space.

#### Algorithmic Solution
1. Utilize the properties of bitwise XOR:
   a. A ^ A = 0 (self-cancellation).
   b. A ^ 0 = A (identity).
   c. XOR is commutative and associative.
2. Initialize `result = 0`.
3. Iterate through every number x in nums: `result ^= x`.
4. All pairs cancel out to 0, leaving only the unique element.
5. Return `result`.

- **Time Complexity**: O(N)
- **Space Complexity**: O(1)

---

### <a id="problem-205"></a>4. Isomorphic Strings (LeetCode 205)

- **Difficulty**: Easy
- **Practice Link**: [Isomorphic Strings](https://leetcode.com/problems/isomorphic-strings)

#### Problem Statement
Given two strings s and t, determine if they are isomorphic (characters in s can be replaced to get t while preserving order and adhering to 1-to-1 bijection).

#### Algorithmic Solution
1. Maintain two 256-element arrays `mapST` and `mapTS` initialized to 0 (representing the last seen 1-based index).
2. Iterate through index i from 0 to s.size() - 1:
   a. Compare `mapST[s[i]]` with `mapTS[t[i]]`.
   b. If they differ, the mapping is inconsistent -> return false.
   c. Update both tracking arrays with current 1-based position: `mapST[s[i]] = i + 1`, `mapTS[t[i]] = i + 1`.
3. If loop completes without mismatch, return true.

- **Time Complexity**: O(N)
- **Space Complexity**: O(1) (fixed 256 ASCII size)

---

### <a id="problem-645"></a>5. Set Mismatch (LeetCode 645)

- **Difficulty**: Easy
- **Practice Link**: [Set Mismatch](https://leetcode.com/problems/set-mismatch)

#### Problem Statement
You have a set of integers [1, n]. Due to an error, one number duplicated and one was lost. Find the duplicate and missing numbers.

#### Algorithmic Solution
1. Cyclic Sort / In-place Marking:
   a. Iterate through nums. For each x = abs(nums[i]):
      - If nums[x - 1] < 0, then x is the duplicate number!
      - Else, mark visited by negating: nums[x - 1] = -nums[x - 1].
2. In a second pass, the index i where nums[i] > 0 corresponds to the missing number `i + 1`.
3. Return {duplicate, missing}.

- **Time Complexity**: O(N)
- **Space Complexity**: O(1)

---

### <a id="problem-779"></a>6. K-th Symbol in Grammar (LeetCode 779)

- **Difficulty**: Medium
- **Practice Link**: [K-th Symbol in Grammar](https://leetcode.com/problems/k-th-symbol-in-grammar)

#### Problem Statement
Row 1 is 0. Each row replaces 0 with 01 and 1 with 10. Given n and k (1-indexed), return the kth symbol in row n.

#### Algorithmic Solution
1. Mathematical Insight: Row n consists of Row n-1 followed by the inverted Row n-1.
2. The value at index k is determined by the number of 1-bits in (k - 1).
3. If the binary representation of (k - 1) has an even number of set bits (parity 0), return 0.
4. If (k - 1) has an odd number of set bits (parity 1), return 1.
5. In C++: `return __builtin_popcount(k - 1) & 1;`.

- **Time Complexity**: O(1)
- **Space Complexity**: O(1)

---

### <a id="problem-3532"></a>7. Path Existence Queries in a Graph I (LeetCode 3532)

- **Difficulty**: Medium
- **Practice Link**: [Path Existence Queries in a Graph I](https://leetcode.com/problems/path-existence-queries-in-a-graph-i)

#### Problem Statement
Given an undirected graph with queries [u, v] determining whether a valid path exists between u and v under edge threshold/weight constraints.

#### Algorithmic Solution
1. Disjoint Set Union (DSU / Union-Find) approach:
   a. Initialize parent array of size N where parent[i] = i, rank[i] = 0.
   b. Union all valid edges matching the graph threshold criteria.
2. For each query [u, v]:
   a. Check if find(u) == find(v).
   b. If root(u) == root(v), path exists -> push true; else push false.
3. Return query results vector.

- **Time Complexity**: O(E * alpha(V) + Q * alpha(V))
- **Space Complexity**: O(V)

---

### <a id="problem-1288"></a>8. Remove Covered Intervals (LeetCode 1288)

- **Difficulty**: Medium
- **Practice Link**: [Remove Covered Intervals](https://leetcode.com/problems/remove-covered-intervals)

#### Problem Statement
Given a list of intervals, remove all intervals that are covered by another interval in the list. Return the count of remaining intervals.

#### Algorithmic Solution
1. Sort intervals by `start` ASCENDING, and for ties by `end` DESCENDING.
   - Example: [1,4] comes before [1,2]. This ensures larger covering intervals are processed first.
2. Initialize `count = 0` and `maxRight = 0`.
3. Iterate through sorted intervals [l, r]:
   a. If `r > maxRight`: This interval extends further and is NOT covered. Increment `count++` and update `maxRight = r`.
   b. Else (`r <= maxRight`): This interval is completely covered by an earlier interval. Skip it.
4. Return `count`.

- **Time Complexity**: O(N log N)
- **Space Complexity**: O(1)

---

### <a id="problem-9"></a>9. Palindrome Number (LeetCode 9)

- **Difficulty**: Easy
- **Practice Link**: [Palindrome Number](https://leetcode.com/problems/palindrome-number)

#### Problem Statement
Given integer x, return true if x is a palindrome without converting the integer to a string.

#### Algorithmic Solution
1. Negative numbers are never palindromes (e.g. -121 -> 121-).
2. Numbers ending in 0 (except 0 itself) are not palindromes (e.g. 10).
3. Reverse only the second half of the number to prevent 32-bit integer overflow:
   a. Initialize `revertedNumber = 0`.
   b. While `x > revertedNumber`:
      - `revertedNumber = revertedNumber * 10 + x % 10`
      - `x /= 10`
4. When loop ends, check if `x == revertedNumber` (even length) or `x == revertedNumber / 10` (odd length).

- **Time Complexity**: O(log10(N))
- **Space Complexity**: O(1)

---

### <a id="problem-7"></a>10. Reverse Integer (LeetCode 7)

- **Difficulty**: Medium
- **Practice Link**: [Reverse Integer](https://leetcode.com/problems/reverse-integer)

#### Problem Statement
Given a signed 32-bit integer x, return x with its digits reversed. If reversing causes the value to go outside [-2^31, 2^31 - 1], return 0.

#### Algorithmic Solution
1. Initialize `rev = 0`.
2. While x != 0:
   a. Pop last digit: `pop = x % 10`, `x /= 10`.
   b. Check for 32-bit signed overflow BEFORE multiplying by 10:
      - If `rev > INT_MAX / 10` or (`rev == INT_MAX / 10` and `pop > 7`), return 0.
      - If `rev < INT_MIN / 10` or (`rev == INT_MIN / 10` and `pop < -8`), return 0.
   c. Push digit: `rev = rev * 10 + pop`.
3. Return `rev`.

- **Time Complexity**: O(log10(N))
- **Space Complexity**: O(1)

---

### <a id="problem-2858"></a>11. Minimum Edge Reversals So Every Node Is Reachable (LeetCode 2858)

- **Difficulty**: Hard
- **Practice Link**: [Minimum Edge Reversals So Every Node Is Reachable](https://leetcode.com/problems/minimum-edge-reversals-so-every-node-is-reachable)

#### Problem Statement
Given a directed tree with n nodes and n-1 edges. Return an array ans of size n where ans[i] is the minimum edge reversals required to reach all nodes from root i.

#### Algorithmic Solution
1. Re-rooting Tree Dynamic Programming (2-pass DFS):
2. Build bidirectional adjacency list storing pair {neighbor, cost}:
   - Directed edge u -> v: add {v, 0} to adj[u] and {u, 1} to adj[v] (reversing u->v costs 1).
3. Pass 1 (DFS from root 0):
   - Compute total reversals needed if node 0 is the root: sum of all traversal costs.
4. Pass 2 (Re-rooting DFS from parent to child):
   - When transitioning root from `curr` to `child`:
     - If original edge was curr -> child (cost 0), making child the root requires reversing child -> curr (reversal cost becomes +1).
     - If original edge was child -> curr (cost 1), making child the root frees this edge (reversal cost becomes -1).
     - State transition: `ans[child] = ans[curr] + (weight == 0 ? 1 : -1)`.
5. Return computed ans array.

- **Time Complexity**: O(V + E) = O(N)
- **Space Complexity**: O(N)

---

### <a id="problem-863"></a>12. All Nodes Distance K in Binary Tree (LeetCode 863)

- **Difficulty**: Medium
- **Practice Link**: [All Nodes Distance K in Binary Tree](https://leetcode.com/problems/all-nodes-distance-k-in-binary-tree)

#### Problem Statement
Given root of binary tree, target node, and integer k. Return all node values that have distance k from target node.

#### Algorithmic Solution
1. Tree to Graph Conversion (Parent Mapping):
   a. Run DFS from root to build an `unordered_map<TreeNode*, TreeNode*> parentTrack`.
2. BFS Expansion from Target:
   a. Queue `q.push(target)`, `unordered_set<TreeNode*> visited; visited.insert(target)`.
   b. Level by level BFS while `dist < k`:
      - For each node popped: examine 3 directions: `node->left`, `node->right`, `parentTrack[node]`.
      - If unvisited, mark visited and push to queue.
      - Increment `dist++`.
3. When `dist == k`, all nodes remaining in queue are at distance k.
4. Extract values from queue into vector and return.

- **Time Complexity**: O(N)
- **Space Complexity**: O(N)

---

### <a id="problem-6"></a>13. Zigzag Conversion (LeetCode 6)

- **Difficulty**: Medium
- **Practice Link**: [Zigzag Conversion](https://leetcode.com/problems/zigzag-conversion)

#### Problem Statement
Write string s across numRows in a zigzag pattern and read line by line.

#### Algorithmic Solution
1. Base case: If `numRows == 1` or `numRows >= s.size()`, return s.
2. Initialize `vector<string> rows(numRows)`.
3. Maintain `curRow = 0` and direction flag `goingDown = false`.
4. Iterate through each char c in s:
   a. `rows[curRow] += c`.
   b. If `curRow == 0` or `curRow == numRows - 1`: flip direction `goingDown = !goingDown`.
   c. Update row: `curRow += goingDown ? 1 : -1`.
5. Join all rows from 0 to numRows - 1 into a single string and return.

- **Time Complexity**: O(N)
- **Space Complexity**: O(N)

---

### <a id="problem-1353"></a>14. Maximum Number of Events That Can Be Attended (LeetCode 1353)

- **Difficulty**: Medium
- **Practice Link**: [Maximum Number of Events That Can Be Attended](https://leetcode.com/problems/maximum-number-of-events-that-can-be-attended)

#### Problem Statement
Given events [startDay, endDay]. You can attend at most one event on any given day. Return maximum number of events you can attend.

#### Algorithmic Solution
1. Sort events by startDay ASC.
2. Maintain a min-heap `pq` storing endDays of currently available events.
3. Iterate day `d` from 1 to max possible day (e.g. 100000):
   a. Add all events starting on day `d` to `pq`.
   b. Remove all events from `pq` whose endDay < d (already expired).
   c. If `pq` is not empty, greedily attend the event that ends earliest (`pq.top()`), pop it, and increment `attendedCount++`.
   d. If all events processed and `pq` is empty, break early.
4. Return `attendedCount`.

- **Time Complexity**: O(N log N)
- **Space Complexity**: O(N)

---

### <a id="problem-85"></a>15. Maximal Rectangle (LeetCode 85)

- **Difficulty**: Hard
- **Practice Link**: [Maximal Rectangle](https://leetcode.com/problems/maximal-rectangle)

#### Problem Statement
Given a rows x cols binary matrix filled with '0's and '1's, find the largest rectangle containing only 1's and return its area.

#### Algorithmic Solution
1. Reduce 2D matrix to repeated 1D 'Largest Rectangle in Histogram' subproblems:
2. Maintain `vector<int> heights(cols, 0)`.
3. For each row `r` from 0 to rows - 1:
   a. For each col `c`: if matrix[r][c] == '1' heights[c] += 1; else heights[c] = 0 (reset height on 0).
   b. Run Monotonic Increasing Stack on `heights` to compute max area in O(cols):
      - For i = 0 to cols: while stack not empty and heights[i] < heights[st.top()]:
        pop h, width = st.empty() ? i : i - st.top() - 1, maxArea = max(maxArea, h * width).
4. Return the global maxArea.

- **Time Complexity**: O(Rows * Cols)
- **Space Complexity**: O(Cols)

---

### <a id="problem-17"></a>16. Letter Combinations of a Phone Number (LeetCode 17)

- **Difficulty**: Medium
- **Practice Link**: [Letter Combinations of a Phone Number](https://leetcode.com/problems/letter-combinations-of-a-phone-number)

#### Problem Statement
Given string of digits 2-9, return all possible letter combinations that the number could represent based on telephone keypad mapping.

#### Algorithmic Solution
1. Create lookup map: 2:"abc", 3:"def", 4:"ghi", 5:"jkl", 6:"mno", 7:"pqrs", 8:"tuv", 9:"wxyz".
2. If digits is empty, return {}.
3. Backtracking DFS function `solve(index, currentCombination)`:
   a. If index == digits.size(): add currentCombination to result; return.
   b. For each letter in map[digits[index]]:
      - append letter to currentCombination.
      - recurse `solve(index + 1, currentCombination)`.
      - pop_back letter (backtrack).
4. Return result vector.

- **Time Complexity**: O(4^N * N)
- **Space Complexity**: O(N) recursion depth

---

### <a id="problem-475"></a>17. Heaters (LeetCode 475)

- **Difficulty**: Medium
- **Practice Link**: [Heaters](https://leetcode.com/problems/heaters)

#### Problem Statement
Given positions of houses and heaters on a 1D line, find the minimum radius required for heaters to cover all houses.

#### Algorithmic Solution
1. Sort the `heaters` array.
2. For each house at position `h`:
   a. Use `std::lower_bound` on heaters to find the closest heater on the right: `it`.
   b. Candidate 1 (right heater): dist1 = it != heaters.end() ? abs(*it - h) : INT_MAX.
   c. Candidate 2 (left heater): dist2 = it != heaters.begin() ? abs(*(it - 1) - h) : INT_MAX.
   d. The minimum radius to cover this house is `min(dist1, dist2)`.
3. Global radius is `max(minDistHouse)` over all houses.
4. Return max radius.

- **Time Complexity**: O((H + N) log N) where N = heaters count
- **Space Complexity**: O(1)

---

### <a id="problem-204"></a>18. Count Primes (LeetCode 204)

- **Difficulty**: Medium
- **Practice Link**: [Count Primes](https://leetcode.com/problems/count-primes)

#### Problem Statement
Given an integer n, return the number of prime numbers strictly less than n.

#### Algorithmic Solution
1. Sieve of Eratosthenes:
2. If n <= 2, return 0.
3. Initialize `vector<bool> isPrime(n, true)`, set isPrime[0] = isPrime[1] = false.
4. For i = 2 up to sqrt(n):
   a. If isPrime[i] is true:
      - Mark all multiples starting at i * i up to n as false: `for j = i*i; j < n; j += i: isPrime[j] = false`.
5. Count `true` values in isPrime array and return count.

- **Time Complexity**: O(N log log N)
- **Space Complexity**: O(N)

---

### <a id="problem-1838"></a>19. Frequency of the Most Frequent Element (LeetCode 1838)

- **Difficulty**: Medium
- **Practice Link**: [Frequency of the Most Frequent Element](https://leetcode.com/problems/frequency-of-the-most-frequent-element)

#### Problem Statement
You can increment elements at most k times. Return the maximum possible frequency of any element.

#### Algorithmic Solution
1. Sort `nums` in ascending order.
2. Sliding Window Invariant:
   - For window [left..right], if all elements are raised to `nums[right]`, cost required is:
     `cost = (right - left + 1) * nums[right] - windowSum`.
3. While `cost > k`:
   - Subtract `nums[left]` from `windowSum`, increment `left++`.
4. Update `maxFreq = max(maxFreq, right - left + 1)`.
5. Return `maxFreq`.

- **Time Complexity**: O(N log N)
- **Space Complexity**: O(1)

---

### <a id="problem-36"></a>20. Valid Sudoku (LeetCode 36)

- **Difficulty**: Medium
- **Practice Link**: [Valid Sudoku](https://leetcode.com/problems/valid-sudoku)

#### Problem Statement
Determine if a 9x9 Sudoku board is valid (no duplicate digits 1-9 in any row, column, or 3x3 box).

#### Algorithmic Solution
1. Use bitmasks or boolean arrays: `rowSeen[9][9]`, `colSeen[9][9]`, `boxSeen[9][9]`.
2. Iterate r from 0 to 8, c from 0 to 8:
   a. If board[r][c] == '.', continue.
   b. Digit `val = board[r][c] - '1'`.
   c. 3x3 Box index: `boxIdx = (r / 3) * 3 + (c / 3)`.
   d. If `rowSeen[r][val]` or `colSeen[c][val]` or `boxSeen[boxIdx][val]` is true:
      - Return false (duplicate detected).
   e. Set `rowSeen[r][val] = colSeen[c][val] = boxSeen[boxIdx][val] = true`.
3. Return true if no conflicts.

- **Time Complexity**: O(1) (fixed 81 cells)
- **Space Complexity**: O(1)

---

### <a id="problem-137"></a>21. Single Number II (LeetCode 137)

- **Difficulty**: Medium
- **Practice Link**: [Single Number II](https://leetcode.com/problems/single-number-ii)

#### Problem Statement
Every element appears three times except for one which appears exactly once. Find that single element in O(N) time and O(1) space.

#### Algorithmic Solution
1. Digital Logic Finite State Machine with two bitmask variables `ones` and `twos`:
   - `ones` tracks bits that appeared 1 or 4 or 7 times (1 mod 3).
   - `twos` tracks bits that appeared 2 or 5 times (2 mod 3).
2. For each number `x` in nums:
   a. `ones = (ones ^ x) & ~twos;`
   b. `twos = (twos ^ x) & ~ones;`
3. When an element appears 3 times, both bits reset to 0.
4. Return `ones`.

- **Time Complexity**: O(N)
- **Space Complexity**: O(1)

---

### <a id="problem-274"></a>22. H-Index (LeetCode 274)

- **Difficulty**: Medium
- **Practice Link**: [H-Index](https://leetcode.com/problems/h-index)

#### Problem Statement
Given array of citations, find scientist's h-index (at least h papers have >= h citations).

#### Algorithmic Solution
1. Counting / Bucket Sort in O(N):
2. Let n = citations.size(). Create `vector<int> buckets(n + 1, 0)`.
3. For each c in citations:
   - If `c >= n`, `buckets[n]++`; else `buckets[c]++`.
4. Accumulate paper count from k = n down to 0:
   - `totalPapers += buckets[k]`.
   - If `totalPapers >= k`, return `k` (this is the maximum valid h-index).
5. Return 0.

- **Time Complexity**: O(N)
- **Space Complexity**: O(N)

---

### <a id="problem-992"></a>23. Subarrays with K Different Integers (LeetCode 992)

- **Difficulty**: Hard
- **Practice Link**: [Subarrays with K Different Integers](https://leetcode.com/problems/subarrays-with-k-different-integers)

#### Problem Statement
Given array nums and integer k, return number of good subarrays having exactly k distinct integers.

#### Algorithmic Solution
1. Exact-K Reduction Pattern:
   `exact(K) = atMost(K) - atMost(K - 1)`.
2. Helper `atMost(K)` using Sliding Window:
   a. Maintain `unordered_map<int, int> count`.
   b. For right = 0 to n - 1:
      - count[nums[right]]++.
      - while `count.size() > K`:
        decrement count[nums[left]], if count[nums[left]] == 0 erase it, left++.
      - Add `(right - left + 1)` to total subarrays ending at right.
3. Return `atMost(k) - atMost(k - 1)`.

- **Time Complexity**: O(N)
- **Space Complexity**: O(K)

---

### <a id="problem-130"></a>24. Surrounded Regions (LeetCode 130)

- **Difficulty**: Medium
- **Practice Link**: [Surrounded Regions](https://leetcode.com/problems/surrounded-regions)

#### Problem Statement
Given m x n matrix board of 'X' and 'O', capture all regions surrounded by 'X' by flipping them to 'X'. Regions connected to boundaries cannot be flipped.

#### Algorithmic Solution
1. Boundary-First Reverse DFS:
   - Any 'O' connected to the 4 boundary borders CANNOT be captured.
2. Traverse all border cells:
   - For all 'O' on boundary (row 0, row m-1, col 0, col n-1), run DFS/BFS to temporarily mark connected 'O' cells as '#'.
3. Traverse entire grid:
   - If cell is 'O' (unconnected to boundary): flip to 'X' (captured).
   - If cell is '#' (connected to boundary): restore back to 'O'.
4. Return modified board.

- **Time Complexity**: O(M * N)
- **Space Complexity**: O(M * N) recursion stack

---

### <a id="problem-2161"></a>25. Partition Array According to Given Pivot (LeetCode 2161)

- **Difficulty**: Medium
- **Practice Link**: [Partition Array According to Given Pivot](https://leetcode.com/problems/partition-array-according-to-given-pivot)

#### Problem Statement
Rearrange nums such that elements < pivot appear first, elements == pivot appear in middle, and elements > pivot appear last, preserving original relative order.

#### Algorithmic Solution
1. Stable 3-Partitioning in O(N) time and O(N) auxiliary space:
2. Count occurrences: count number of elements < pivot, and count elements == pivot.
3. Pointers for output array `res(n)`:
   - `lessIdx = 0`
   - `equalIdx = countLessThan`
   - `greaterIdx = countLessThan + countEqual`
4. Second linear pass through nums:
   - If `x < pivot`: `res[lessIdx++] = x`.
   - Else if `x == pivot`: `res[equalIdx++] = x`.
   - Else (`x > pivot`): `res[greaterIdx++] = x`.
5. Return `res`.

- **Time Complexity**: O(N)
- **Space Complexity**: O(N)

---

### <a id="problem-30"></a>26. Substring with Concatenation of All Words (LeetCode 30)

- **Difficulty**: Hard
- **Practice Link**: [Substring with Concatenation of All Words](https://leetcode.com/problems/substring-with-concatenation-of-all-words)

#### Problem Statement
Find all starting indices in s that are a concatenation of each word in words exactly once and without intervening characters.

#### Algorithmic Solution
1. Let wordLen = words[0].size(), totalWords = words.size(), subLen = wordLen * totalWords.
2. Build frequency map of words `wordCount`.
3. Shift starting offset `offset` from 0 to wordLen - 1:
   a. Maintain sliding window of word tokens with `seenMap` and `matchedWordsCount`.
   b. Jump right by wordLen: extract word = s.substr(right, wordLen).
   c. If word exists in wordCount:
      - Add to seenMap. If seenMap[word] > wordCount[word], shrink left by wordLen until frequency valid.
      - If matchedWordsCount == totalWords, record `left` as valid start index.
   d. Else (invalid word): reset window, left = right + wordLen, clear seenMap.
4. Return indices list.

- **Time Complexity**: O(N * wordLen)
- **Space Complexity**: O(M * wordLen)

---

### <a id="problem-1283"></a>27. Find the Smallest Divisor Given a Threshold (LeetCode 1283)

- **Difficulty**: Medium
- **Practice Link**: [Find the Smallest Divisor Given a Threshold](https://leetcode.com/problems/find-the-smallest-divisor-given-a-threshold)

#### Problem Statement
Given array nums and integer threshold, choose a positive divisor, divide all elements, sum up division results (ceil), and find smallest divisor such that sum <= threshold.

#### Algorithmic Solution
1. Binary Search on Monotonic Answer:
   - Search range: `low = 1`, `high = max_element(nums)`.
2. Predicate `isValid(divisor)`:
   - `sum = 0`
   - For each x in nums: `sum += (x + divisor - 1) / divisor` (integer ceiling division).
   - Return `sum <= threshold`.
3. While `low <= high`:
   - `mid = low + (high - low) / 2`.
   - If `isValid(mid)`: `ans = mid`, `high = mid - 1` (try smaller divisor).
   - Else: `low = mid + 1`.
4. Return `ans`.

- **Time Complexity**: O(N log(max_val))
- **Space Complexity**: O(1)

---

### <a id="problem-526"></a>28. Beautiful Arrangement (LeetCode 526)

- **Difficulty**: Medium
- **Practice Link**: [Beautiful Arrangement](https://leetcode.com/problems/beautiful-arrangement)

#### Problem Statement
Numbers 1..n. An arrangement is beautiful if for every position i (1-indexed), nums[i] % i == 0 or i % nums[i] == 0. Return count of beautiful arrangements.

#### Algorithmic Solution
1. Backtracking with Bitmask State Memoization:
2. Function `countArrangements(pos, mask)`:
   a. If `pos > n`, return 1 (valid full permutation found).
   b. For val = 1 to n:
      - If bit `val` is not used in mask (`!(mask & (1 << val))`):
        - Check condition: if `val % pos == 0` or `pos % val == 0`:
          recurse with `countArrangements(pos + 1, mask | (1 << val))`.
3. Return total count.

- **Time Complexity**: O(N * 2^N)
- **Space Complexity**: O(2^N)

---

### <a id="problem-3761"></a>29. Minimum Absolute Distance Between Mirror Pairs (LeetCode 3761)

- **Difficulty**: Medium
- **Practice Link**: [Minimum Absolute Distance Between Mirror Pairs](https://leetcode.com/problems/minimum-absolute-distance-between-mirror-pairs)

#### Problem Statement
Find minimum index distance |i - j| between any pair (nums[i], nums[j]) such that string/number representation of nums[i] is reverse mirror of nums[j].

#### Algorithmic Solution
1. Maintain a hash map `unordered_map<int, int> lastSeen` storing value -> most recent index.
2. Helper `reverseNum(x)`: reverse digits of integer x.
3. Initialize `minDist = INT_MAX`.
4. Iterate i from 0 to n - 1:
   a. Target mirror needed is `target = reverseNum(nums[i])`.
   b. If `lastSeen.count(target)`:
      - `minDist = min(minDist, i - lastSeen[target])`.
   c. Update `lastSeen[nums[i]] = i`.
5. Return minDist == INT_MAX ? -1 : minDist.

- **Time Complexity**: O(N * log10(max_val))
- **Space Complexity**: O(N)

---

### <a id="problem-827"></a>30. Making A Large Island (LeetCode 827)

- **Difficulty**: Hard
- **Practice Link**: [Making A Large Island](https://leetcode.com/problems/making-a-large-island)

#### Problem Statement
Given n x n binary grid. You are allowed to change at most one 0 to 1. Return the size of the largest island possible.

#### Algorithmic Solution
1. Pass 1 (Island ID Coloring & Area Mapping):
   a. Assign each connected island a unique ID starting from 2, 3, 4...
   b. Run DFS on each unvisited 1, painting cells with `islandId` and counting its area.
   c. Store `areaMap[islandId] = area`.
   d. Track `maxArea = max(existing island areas)`.
2. Pass 2 (Test 0-to-1 flips):
   a. For each cell (r, c) where grid[r][c] == 0:
      - Collect unique island IDs from its 4 neighbors using an `unordered_set<int> neighborIds`.
      - Combined area = 1 (the flipped cell) + sum of areaMap[id] for all unique neighbors.
      - `maxArea = max(maxArea, combinedArea)`.
3. Return `maxArea`.

- **Time Complexity**: O(N^2)
- **Space Complexity**: O(N^2)

---

### <a id="problem-802"></a>31. Find Eventual Safe States (LeetCode 802)

- **Difficulty**: Medium
- **Practice Link**: [Find Eventual Safe States](https://leetcode.com/problems/find-eventual-safe-states)

#### Problem Statement
Directed graph of n nodes. A node is a terminal node if it has no outgoing edges. A node is eventually safe if every possible path starting from it leads to a terminal node. Return all safe nodes sorted.

#### Algorithmic Solution
1. 3-Color Cycle Detection DFS:
   - Color 0: Unvisited.
   - Color 1: Currently in recursion stack (visiting).
   - Color 2: Safe node (all paths lead to terminal).
2. DFS(node):
   a. If color[node] > 0: return color[node] == 2.
   b. Mark color[node] = 1.
   c. For each neighbor in graph[node]:
      - If !DFS(neighbor), cycle exists -> return false.
   d. Mark color[node] = 2 (safe). Return true.
3. Collect all nodes i where color[i] == 2 and return.

- **Time Complexity**: O(V + E)
- **Space Complexity**: O(V)

---

### <a id="problem-684"></a>32. Redundant Connection (LeetCode 684)

- **Difficulty**: Medium
- **Practice Link**: [Redundant Connection](https://leetcode.com/problems/redundant-connection)

#### Problem Statement
Given a tree with n nodes (1 to n) and one extra edge added creating a cycle. Find and return the extra edge that can be removed so the graph is a tree.

#### Algorithmic Solution
1. Disjoint Set Union (DSU / Union-Find) with path compression:
2. Initialize parent array: `parent[i] = i`.
3. For each edge [u, v] in edges:
   a. `rootU = find(u)`, `rootV = find(v)`.
   b. If `rootU == rootV`:
      - u and v are ALREADY connected in the same component! Adding edge [u, v] creates a cycle.
      - This edge [u, v] is the redundant connection -> return [u, v].
   c. Else, `parent[rootU] = rootV`.
4. Return {}.

- **Time Complexity**: O(N * alpha(N))
- **Space Complexity**: O(N)

---

### <a id="problem-525"></a>33. Contiguous Array (LeetCode 525)

- **Difficulty**: Medium
- **Practice Link**: [Contiguous Array](https://leetcode.com/problems/contiguous-array)

#### Problem Statement
Given binary array nums, return maximum length of a contiguous subarray with equal number of 0 and 1.

#### Algorithmic Solution
1. Prefix Sum Transformation:
   - Treat '0' as -1 and '1' as +1.
   - Equal 0s and 1s means subarray sum equals 0!
2. Maintain `unordered_map<int, int> firstSeen` storing prefixSum -> first index where it appeared.
3. Initialize `firstSeen[0] = -1`, `sum = 0`, `maxLen = 0`.
4. For i = 0 to n - 1:
   a. `sum += (nums[i] == 1 ? 1 : -1)`.
   b. If `firstSeen.count(sum)`:
      - `maxLen = max(maxLen, i - firstSeen[sum])`.
   c. Else:
      - `firstSeen[sum] = i` (record first occurrence only to maximize length).
5. Return `maxLen`.

- **Time Complexity**: O(N)
- **Space Complexity**: O(N)

---

### <a id="problem-373"></a>34. Find K Pairs with Smallest Sums (LeetCode 373)

- **Difficulty**: Medium
- **Practice Link**: [Find K Pairs with Smallest Sums](https://leetcode.com/problems/find-k-pairs-with-smallest-sums)

#### Problem Statement
Given two sorted arrays nums1 and nums2, return k pairs (u, v) with the smallest sum.

#### Algorithmic Solution
1. Min-Heap Priority Queue of Tuples {sum, i, j}:
2. Push first elements: For i = 0 up to min(k, (int)nums1.size()) - 1:
   - `pq.push({nums1[i] + nums2[0], i, 0})`.
3. While `pq` is not empty and result.size() < k:
   a. Pop `{sum, i, j}` from pq.
   b. Add `{nums1[i], nums2[j]}` to result.
   c. If `j + 1 < nums2.size()`:
      - `pq.push({nums1[i] + nums2[j + 1], i, j + 1})`.
4. Return result.

- **Time Complexity**: O(K log(min(N, K)))
- **Space Complexity**: O(min(N, K))

---

### <a id="problem-223"></a>35. Rectangle Area (LeetCode 223)

- **Difficulty**: Medium
- **Practice Link**: [Rectangle Area](https://leetcode.com/problems/rectangle-area)

#### Problem Statement
Given coordinates of two rectilinear rectangles in 2D plane, return total area covered by the two rectangles.

#### Algorithmic Solution
1. Compute individual areas:
   - `area1 = (ax2 - ax1) * (ay2 - ay1)`
   - `area2 = (bx2 - bx1) * (by2 - by1)`
2. Calculate overlapping dimensions:
   - `overlapWidth = max(0, min(ax2, bx2) - max(ax1, bx1))`
   - `overlapHeight = max(0, min(ay2, by2) - max(ay1, by1))`
   - `overlapArea = overlapWidth * overlapHeight`
3. Total area by Principle of Inclusion-Exclusion:
   `total = area1 + area2 - overlapArea`.
4. Return total.

- **Time Complexity**: O(1)
- **Space Complexity**: O(1)

---

### <a id="problem-1932"></a>36. Merge BSTs to Create Single BST (LeetCode 1932)

- **Difficulty**: Hard
- **Practice Link**: [Merge BSTs to Create Single BST](https://leetcode.com/problems/merge-bsts-to-create-single-bst)

#### Problem Statement
Given n BST roots with 3 nodes each. Merge all BSTs into a single valid BST by replacing leaf nodes with matching root values.

#### Algorithmic Solution
1. Hash Map of Root Values:
   - Map each `root->val -> TreeNode*`.
2. Count in-degrees of all root values by traversing all children across trees.
3. Identify unique candidate root:
   - The global BST root MUST have in-degree 0. If multiple or zero candidates exist, return nullptr.
4. Tree Merging & Validation DFS:
   - Traverse starting from global root, passing valid BST bounds `(minVal, maxVal)`.
   - When encountering a leaf node whose value is in root map: splice in the child tree, erase from root map, and continue validating.
5. If valid BST and all n trees merged (map empty), return root; else return nullptr.

- **Time Complexity**: O(N)
- **Space Complexity**: O(N)

---

### <a id="problem-3699"></a>37. Number of ZigZag Arrays I (LeetCode 3699)

- **Difficulty**: Hard
- **Practice Link**: [Number of ZigZag Arrays I](https://leetcode.com/problems/number-of-zigzag-arrays-i)

#### Problem Statement
Count number of arrays of length n with elements in [1, m] that alternate directions (a[0] < a[1] > a[2] < a[3]...).

#### Algorithmic Solution
1. Dynamic Programming State Tracking:
   - `dpInc[i][v]`: number of zigzag arrays of length i ending with value v on an increasing step (a[i-1] < a[i]).
   - `dpDec[i][v]`: number of zigzag arrays of length i ending with value v on a decreasing step (a[i-1] > a[i]).
2. Transitions with Prefix Sum Optimization:
   - `dpInc[i][v] = sum_{u < v} dpDec[i-1][u] = prefixSumDec[i-1][v-1]`
   - `dpDec[i][v] = sum_{u > v} dpInc[i-1][u] = suffixSumInc[i-1][v+1]`
3. Sum results across all values v in [1, m] for length n modulo 10^9+7.

- **Time Complexity**: O(N * M)
- **Space Complexity**: O(M)

---

### <a id="problem-3946"></a>38. Maximum Number of Items From Sale I (LeetCode 3946)

- **Difficulty**: Medium
- **Practice Link**: [Maximum Number of Items From Sale I](https://leetcode.com/problems/maximum-number-of-items-from-sale-i)

#### Problem Statement
Given items with costs and discounts, optimize budget allocation to maximize total items acquired.

#### Algorithmic Solution
1. Sort items by discounted effective cost.
2. Greedy purchase simulation:
   - Buy lowest-cost available items first within total budget.
3. If discount triggers depend on prior purchases, model as 0/1 Knapsack DP `dp[b]` = max items with budget b.
4. Return maximum count.

- **Time Complexity**: O(N log N) / O(N * Budget)
- **Space Complexity**: O(Budget)

---

### <a id="problem-202"></a>39. Happy Number (LeetCode 202)

- **Difficulty**: Easy
- **Practice Link**: [Happy Number](https://leetcode.com/problems/happy-number)

#### Problem Statement
Starting with positive integer n, replace it with the sum of the squares of its digits repeatedly until n equals 1 (happy) or loops endlessly in a cycle.

#### Algorithmic Solution
1. Floyd's Tortoise and Hare Cycle Detection:
   - Treat digit square sum transitions as a directed linked list: `n -> getNext(n)`.
2. Helper `getNext(n)`:
   - While `n > 0`: `d = n % 10`, `sum += d * d`, `n /= 10`. Return `sum`.
3. Initialize `slow = n`, `fast = getNext(n)`.
4. While `fast != 1 && slow != fast`:
   - `slow = getNext(slow)` (1 step).
   - `fast = getNext(getNext(fast))` (2 steps).
5. Return `fast == 1` (if cycle met at 1, number is happy; else false).

- **Time Complexity**: O(log N)
- **Space Complexity**: O(1)

---

### <a id="problem-387"></a>40. First Unique Character in a String (LeetCode 387)

- **Difficulty**: Easy
- **Practice Link**: [First Unique Character in a String](https://leetcode.com/problems/first-unique-character-in-a-string)

#### Problem Statement
Given string s, find the first non-repeating character and return its index. If it does not exist, return -1.

#### Algorithmic Solution
1. First pass: Count character frequencies in a fixed 26-element integer array `count[26]`.
   - For each char c in s: `count[c - 'a']++`.
2. Second pass: Iterate through string s with index i from 0 to s.size() - 1:
   - If `count[s[i] - 'a'] == 1`, return index `i`.
3. If loop finishes without returning, return -1.

- **Time Complexity**: O(N)
- **Space Complexity**: O(1) (fixed 26-element array)

---

### <a id="problem-168"></a>41. Excel Sheet Column Title (LeetCode 168)

- **Difficulty**: Easy
- **Practice Link**: [Excel Sheet Column Title](https://leetcode.com/problems/excel-sheet-column-title)

#### Problem Statement
Given an integer columnNumber, return its corresponding Excel column title (e.g. 1 -> A, 28 -> AB, 701 -> ZY).

#### Algorithmic Solution
1. Base-26 conversion with 1-based indexing gotcha:
   - Unlike standard base-26 [0..25], Excel columns are 1-based [1..26] ('A'..'Z').
2. While `columnNumber > 0`:
   a. CRITICAL STEP: Decrement `columnNumber--` to convert 1-based to 0-based.
   b. Extract remainder: `rem = columnNumber % 26`.
   c. Character: `char c = 'A' + rem`.
   d. Prepend char to result string (or append and reverse at the end).
   e. `columnNumber /= 26`.
3. Reverse result string and return.

- **Time Complexity**: O(log26(N))
- **Space Complexity**: O(1)

---

### <a id="problem-171"></a>42. Excel Sheet Column Number (LeetCode 171)

- **Difficulty**: Easy
- **Practice Link**: [Excel Sheet Column Number](https://leetcode.com/problems/excel-sheet-column-number)

#### Problem Statement
Given a string columnTitle, return its corresponding Excel column number (e.g. 'A' -> 1, 'AB' -> 28, 'ZY' -> 701).

#### Algorithmic Solution
1. Base-26 numerical accumulation (analogous to base-10 string-to-integer conversion):
2. Initialize `ans = 0`.
3. For each char c in columnTitle:
   a. Value of current digit: `val = c - 'A' + 1`.
   b. Shift base: `ans = ans * 26 + val`.
4. Return `ans`.

- **Time Complexity**: O(N)
- **Space Complexity**: O(1)

---

### <a id="problem-1071"></a>43. Greatest Common Divisor of Strings (LeetCode 1071)

- **Difficulty**: Easy
- **Practice Link**: [Greatest Common Divisor of Strings](https://leetcode.com/problems/greatest-common-divisor-of-strings)

#### Problem Statement
For two strings s and t, t divides s if s = t + t + ... + t. Return largest string x such that x divides both str1 and str2.

#### Algorithmic Solution
1. Fundamental Property: str1 and str2 share a common divisor string if and only if `str1 + str2 == str2 + str1`.
2. Check: If `str1 + str2 != str2 + str1`, return "" (no common divisor exists).
3. If equal, the length of the GCD string is exactly `gcd(str1.size(), str2.size())`.
4. In C++: `return str1.substr(0, std::gcd(str1.size(), str2.size()));`.

- **Time Complexity**: O(N + M)
- **Space Complexity**: O(N + M)

---

### <a id="problem-405"></a>44. Convert a Number to Hexadecimal (LeetCode 405)

- **Difficulty**: Easy
- **Practice Link**: [Convert a Number to Hexadecimal](https://leetcode.com/problems/convert-a-number-to-hexadecimal)

#### Problem Statement
Given a 32-bit integer num, return a string representing its hexadecimal representation. Two's complement must be handled for negative numbers.

#### Algorithmic Solution
1. Base case: If `num == 0`, return "0".
2. Cast integer to `unsigned int n = num` (handles two's complement automatically in C++).
3. Lookup table: `const string hex = "0123456789abcdef";`.
4. While `n > 0`:
   a. Extract lowest 4 bits: `int digit = n & 0xf;`.
   b. Append `hex[digit]` to result.
   c. Logical right shift by 4 bits: `n >>= 4;`.
5. Reverse the string and return.

- **Time Complexity**: O(1) (at most 8 iterations for 32-bit integer)
- **Space Complexity**: O(1)

---

### <a id="problem-844"></a>45. Backspace String Compare (LeetCode 844)

- **Difficulty**: Easy
- **Practice Link**: [Backspace String Compare](https://leetcode.com/problems/backspace-string-compare)

#### Problem Statement
Given two strings s and t with '#' representing backspace, return true if they are equal after applying backspaces. Solve in O(1) space.

#### Algorithmic Solution
1. Backward Two Pointers (O(1) Space):
   - Traverse both strings from right to left (index i = s.size()-1, j = t.size()-1).
2. For each string, track backspace skip counters `skipS = 0`, `skipT = 0`:
   a. While i >= 0: if s[i] == '#', skipS++, i--; else if skipS > 0, skipS--, i--; else break.
   b. While j >= 0: if t[j] == '#', skipT++, j--; else if skipT > 0, skipT--, j--; else break.
3. Compare characters:
   a. If both valid and `s[i] != t[j]`, return false.
   b. If one string exhausted and the other is not (`(i >= 0) != (j >= 0)`), return false.
   c. `i--`, `j--`.
4. If loop completes, return true.

- **Time Complexity**: O(N + M)
- **Space Complexity**: O(1)

---

### <a id="problem-977"></a>46. Squares of a Sorted Array (LeetCode 977)

- **Difficulty**: Easy
- **Practice Link**: [Squares of a Sorted Array](https://leetcode.com/problems/squares-of-a-sorted-array)

#### Problem Statement
Given sorted array nums (including negatives), return array of squares sorted in non-decreasing order in O(N) time.

#### Algorithmic Solution
1. Two Pointers Inward Scan (Left & Right Ends):
   - The largest squares can ONLY come from the leftmost negative elements or rightmost positive elements.
2. Initialize `left = 0`, `right = nums.size() - 1`, `k = nums.size() - 1`, `vector<int> res(nums.size())`.
3. While `left <= right`:
   a. `sqLeft = nums[left] * nums[left]`.
   b. `sqRight = nums[right] * nums[right]`.
   c. If `sqLeft > sqRight`: `res[k--] = sqLeft`, `left++`.
   d. Else: `res[k--] = sqRight`, `right--`.
4. Return `res`.

- **Time Complexity**: O(N)
- **Space Complexity**: O(1) auxiliary space

---

### <a id="problem-1047"></a>47. Remove All Adjacent Duplicates In String (LeetCode 1047)

- **Difficulty**: Easy
- **Practice Link**: [Remove All Adjacent Duplicates In String](https://leetcode.com/problems/remove-all-adjacent-duplicates-in-string)

#### Problem Statement
Repeatedly remove duplicate adjacent character pairs from string until no duplicates remain.

#### Algorithmic Solution
1. In-Place String Stack:
   - Use output string `res` directly as a stack to avoid extra stack object allocation.
2. For each char c in s:
   a. If `!res.empty() && res.back() == c`:
      - Pop duplicate: `res.pop_back();`.
   b. Else:
      - Push char: `res.push_back(c);`.
3. Return `res`.

- **Time Complexity**: O(N)
- **Space Complexity**: O(1) auxiliary space

---

### <a id="problem-724"></a>48. Find Pivot Index (LeetCode 724)

- **Difficulty**: Easy
- **Practice Link**: [Find Pivot Index](https://leetcode.com/problems/find-pivot-index)

#### Problem Statement
Find the leftmost pivot index where the sum of numbers strictly to the left equals the sum of numbers strictly to the right.

#### Algorithmic Solution
1. Compute `totalSum = accumulate(nums.begin(), nums.end(), 0)`.
2. Initialize `leftSum = 0`.
3. For index i from 0 to nums.size() - 1:
   a. Right sum is implicitly: `rightSum = totalSum - leftSum - nums[i]`.
   b. If `leftSum == rightSum`, return `i`.
   c. `leftSum += nums[i]`.
4. If no pivot found, return -1.

- **Time Complexity**: O(N)
- **Space Complexity**: O(1)

---

### <a id="problem-290"></a>49. Word Pattern (LeetCode 290)

- **Difficulty**: Easy
- **Practice Link**: [Word Pattern](https://leetcode.com/problems/word-pattern)

#### Problem Statement
Given pattern and string s, determine if s follows the same pattern (full 1-to-1 bijection between a letter in pattern and a non-empty word in s).

#### Algorithmic Solution
1. Split string s into tokens/words using `istringstream`.
2. If words.size() != pattern.size(), return false.
3. Maintain two hash maps: `unordered_map<char, string> charToWord`, `unordered_map<string, char> wordToChar`.
4. For index i from 0 to pattern.size() - 1:
   a. `c = pattern[i]`, `w = words[i]`.
   b. If `charToWord.count(c)` and `charToWord[c] != w`: return false.
   c. If `wordToChar.count(w)` and `wordToChar[w] != c`: return false.
   d. Insert mappings: `charToWord[c] = w`, `wordToChar[w] = c`.
5. Return true.

- **Time Complexity**: O(N)
- **Space Complexity**: O(N)

---

### <a id="problem-190"></a>50. Reverse Bits (LeetCode 190)

- **Difficulty**: Easy
- **Practice Link**: [Reverse Bits](https://leetcode.com/problems/reverse-bits)

#### Problem Statement
Reverse bits of a given 32-bit unsigned integer.

#### Algorithmic Solution
1. Initialize `ans = 0`.
2. Loop 32 times (i = 0 to 31):
   a. Shift ans left by 1 to make room: `ans = (ans << 1) | (n & 1)`.
   b. Shift n right by 1 to process next bit: `n >>= 1`.
3. Return `ans`.

- **Time Complexity**: O(1) (fixed 32 steps)
- **Space Complexity**: O(1)

---

### <a id="problem-191"></a>51. Number of 1 Bits (LeetCode 191)

- **Difficulty**: Easy
- **Practice Link**: [Number of 1 Bits](https://leetcode.com/problems/number-of-1-bits)

#### Problem Statement
Write a function that takes an unsigned integer and returns the number of '1' bits (Hamming weight).

#### Algorithmic Solution
1. Brian Kernighan's Algorithm:
   - Operation `n & (n - 1)` clears the lowest set bit in binary representation of n.
2. Initialize `count = 0`.
3. While `n != 0`:
   a. `n = n & (n - 1)`.
   b. `count++`.
4. In C++ built-in: `return __builtin_popcount(n);`.
5. Return `count`.

- **Time Complexity**: O(Number of set bits) <= 32
- **Space Complexity**: O(1)

---

### <a id="problem-268"></a>52. Missing Number (LeetCode 268)

- **Difficulty**: Easy
- **Practice Link**: [Missing Number](https://leetcode.com/problems/missing-number)

#### Problem Statement
Given array nums containing n distinct numbers in range [0, n], return the only number in range that is missing.

#### Algorithmic Solution
1. Bitwise XOR Approach:
   - Index range has numbers 0..n. Array has n numbers.
   - XOR all indices 0..n and XOR all array values: all present numbers appear twice and cancel to 0; missing number remains.
2. `int ans = n; for (int i = 0; i < n; ++i) ans ^= i ^ nums[i]; return ans;`
3. (Alternative Gauss Sum: `sumExpected = n * (n + 1) / 2`, `return sumExpected - sumActual`).

- **Time Complexity**: O(N)
- **Space Complexity**: O(1)

---

### <a id="problem-338"></a>53. Counting Bits (LeetCode 338)

- **Difficulty**: Easy
- **Practice Link**: [Counting Bits](https://leetcode.com/problems/counting-bits)

#### Problem Statement
Given integer n, return an array ans of length n + 1 where ans[i] is the number of 1's in the binary representation of i.

#### Algorithmic Solution
1. 1D Dynamic Programming with Bitwise Shift:
   - The number of 1-bits in `i` equals the 1-bits in `i / 2` (`i >> 1`) plus 1 if `i` is odd (`i & 1`).
2. Initialize `vector<int> dp(n + 1, 0)`.
3. For i = 1 to n:
   - `dp[i] = dp[i >> 1] + (i & 1);`.
4. Return `dp`.

- **Time Complexity**: O(N)
- **Space Complexity**: O(1) auxiliary space

---

### <a id="problem-680"></a>54. Valid Palindrome II (LeetCode 680)

- **Difficulty**: Easy
- **Practice Link**: [Valid Palindrome II](https://leetcode.com/problems/valid-palindrome-ii)

#### Problem Statement
Given string s, return true if the string can be a palindrome after deleting at most one character.

#### Algorithmic Solution
1. Two pointers inwards scan: `left = 0`, `right = s.size() - 1`.
2. Helper `isPalindromeRange(s, l, r)`: returns true if s[l..r] is palindrome.
3. While `left < right`:
   a. If `s[left] == s[right]`: `left++`, `right--`.
   b. If mismatch `s[left] != s[right]`:
      - We can delete `s[left]` (test range `[left + 1, right]`) OR delete `s[right]` (test range `[left, right - 1]`).
      - Return `isPalindromeRange(s, left + 1, right) || isPalindromeRange(s, left, right - 1)`.
4. If loop completes, return true.

- **Time Complexity**: O(N)
- **Space Complexity**: O(1)

---

### <a id="problem-953"></a>55. Verifying an Alien Dictionary (LeetCode 953)

- **Difficulty**: Easy
- **Practice Link**: [Verifying an Alien Dictionary](https://leetcode.com/problems/verifying-an-alien-dictionary)

#### Problem Statement
Given a sequence of words written in an alien language and the order of the alphabet, return true if words are sorted lexicographically in this alien language.

#### Algorithmic Solution
1. Map alien character rank: `int rank[26]; for (int i = 0; i < 26; ++i) rank[order[i] - 'a'] = i;`.
2. For each adjacent pair (words[i], words[i+1]):
   a. Compare characters at index k:
      - If `words[i][k] != words[i+1][k]`:
        - If `rank[words[i][k] - 'a'] > rank[words[i+1][k] - 'a']`, return false.
        - Else break (this pair is correctly ordered).
   b. If prefix matched completely, ensure `words[i].size() <= words[i+1].size()` (e.g. "apple" cannot come before "app").
3. If all pairs valid, return true.

- **Time Complexity**: O(Total Characters)
- **Space Complexity**: O(1)

---

