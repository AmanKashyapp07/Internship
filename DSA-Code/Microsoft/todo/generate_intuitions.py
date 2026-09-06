import json
import re
import os

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
JSON_PATH = os.path.join(BASE_DIR, 'problems.json')

with open(JSON_PATH, 'r', encoding='utf-8') as f:
    problems = json.load(f)

# Comprehensive intuition mapping
SPECIFIC_INTUITIONS = {
    # 1.cpp
    "Next Permutation": [
        "A strictly descending suffix cannot be rearranged into any larger permutation.",
        "Scan from right to find the first decrease nums[i] < nums[i+1] (pivot point).",
        "Swap nums[i] with the smallest element in the suffix that is strictly greater than nums[i].",
        "Reverse the suffix after index i to make it ascending (lexicographically smallest)."
    ],
    "Jump Game II": [
        "Treat jumps as BFS levels where each step explores an entire reachable window [curEnd, nextFarthest].",
        "Greedily track the farthest reachable index for any jump initiated in the current window.",
        "When index reaches curEnd, increment jump count and update curEnd to nextFarthest.",
        "Single pass O(N) avoids costly O(N^2) DP or explicit queue memory."
    ],
    "Merge Intervals": [
        "Sorting intervals by start time ensures all overlapping intervals are contiguous.",
        "Maintain a running interval; for each next interval, check if next.start <= current.end.",
        "If overlapping, merge by extending current.end = max(current.end, next.end).",
        "If disjoint, push current interval to result and update current to the next interval."
    ],
    "Gas Station": [
        "If total gas across all stations is less than total cost, completion is impossible (return -1).",
        "Accumulate tank = gas[i] - cost[i]; if tank drops below 0, no station from current start to i can be valid.",
        "Greedily reset start station to i + 1 and reset current tank to 0.",
        "Single pass O(N) guarantees the first valid candidate after the last reset works globally."
    ],
    "Heaters": [
        "Each house needs heat from the closest available heater to minimize total radius.",
        "Sort heaters array so binary search (lower_bound) can locate adjacent heaters in O(log M).",
        "For each house, find the minimum distance between left and right neighboring heaters.",
        "Global answer is the maximum of all individual house minimum heater distances."
    ],
    "Task Scheduler": [
        "The task with the maximum frequency creates the dominant structural bottleneck.",
        "Arrange max frequency tasks into frames separated by cool-down interval n.",
        "Frame formula: (maxFreq - 1) * (n + 1) + count of tasks sharing max frequency.",
        "Result is max(formula, total tasks) to account for cases with no idle slots."
    ],
    "Partition Labels": [
        "A partition must extend until all occurrences of every character seen so far are contained.",
        "Precompute the last occurrence index of every character in a 26-size lookup array.",
        "Iterate through the string, greedily updating boundary end = max(end, last[s[i]]).",
        "When current index equals end, a valid minimal partition has closed; record size and restart."
    ],
    "Remove Covered Intervals": [
        "Sort intervals by start ascending; on tie, sort by end descending so larger intervals appear first.",
        "Maintain running maximum right boundary seen among previous intervals.",
        "If an interval's end <= maxRight, it is fully covered by a predecessor and discarded.",
        "Otherwise, increment non-covered count and expand maxRight to current interval's end."
    ],
    "Max Number of Events Attended": [
        "Sort events by start day; attend events day by day starting from earliest day.",
        "On day d, add all events starting on day d into a min-heap keyed by end day.",
        "Remove expired events from heap (end day < d).",
        "Greedily pick the event ending earliest from the min-heap to preserve options for future days."
    ],
    "Max Items From Sale": [
        "Compute the net effective cost or savings benefit for each item on sale.",
        "Sort items based on maximum marginal gain per unit of budget spent.",
        "Greedily acquire items with highest return until budget or inventory limit is reached.",
        "Breaks ties by favoring lower base price to maximize total items acquired."
    ],
    "Longest Substring Without Repeating Characters": [
        "Use a sliding window [left, right] alongside a last-seen index map for each character.",
        "When s[right] has been seen inside the current window, jump left to lastSeen[s[right]] + 1.",
        "Update lastSeen[s[right]] = right on every step.",
        "Window size right - left + 1 at each iteration tracks the maximum non-repeating length in O(N)."
    ],
    "Container With Most Water": [
        "Water capacity is constrained by min(height[left], height[right]) * (right - left).",
        "Start with two pointers at the widest boundaries (left = 0, right = n - 1).",
        "Moving the taller line inward can never increase area because width decreases and height is bounded by the shorter line.",
        "Greedily move the shorter line pointer inward to search for a potentially taller boundary."
    ],
    "3Sum": [
        "Sort array in O(N log N) to enable two-pointer scanning and duplicate skipping.",
        "Fix the first element nums[i]; if nums[i] > 0, break early as subsequent sums cannot reach 0.",
        "Use two pointers (left = i + 1, right = n - 1) to find pairs summing to -nums[i].",
        "Skip identical consecutive elements for i, left, and right to guarantee strictly unique triplets."
    ],
    "4Sum": [
        "Sort array and use two nested loops to fix the first two elements nums[i] and nums[j].",
        "Convert remaining target to target - nums[i] - nums[j] using 64-bit int to prevent overflow.",
        "Run two pointers on remaining subarray [j + 1, n - 1] to locate valid complementary pairs.",
        "Prune duplicate values at all four levels (i, j, left, right) to prevent duplicate quadruplets."
    ],
    "Trapping Rain Water": [
        "Water trapped above index i depends strictly on min(maxLeft, maxRight) - height[i].",
        "Use two converging pointers from both ends with running variables leftMax and rightMax.",
        "If leftMax < rightMax, the bottleneck at left is confirmed regardless of unseen middle heights.",
        "Accumulate leftMax - height[left] and advance left; otherwise accumulate from rightMax and retreat right."
    ],
    "Sort Colors": [
        "Dutch National Flag algorithm partitions array into three regions: 0s, 1s, and 2s.",
        "Maintain three pointers: low (next 0 slot), mid (current element), high (next 2 slot).",
        "If nums[mid] == 0: swap with nums[low], advance both low and mid.",
        "If nums[mid] == 2: swap with nums[high], decrement high without advancing mid (to inspect swapped value)."
    ],
    "Minimum Window Substring": [
        "Count frequency of characters required from pattern t; maintain count of matched distinct chars.",
        "Expand sliding window right boundary until all characters and frequencies of t are satisfied.",
        "Once valid, contract left boundary to shed redundant characters and minimize window length.",
        "Record smallest valid window coordinates; overall time is O(N + M) with O(1) alphabet space."
    ],
    "Find All Anagrams in a String": [
        "An anagram is equivalent to identical character frequency counts over a fixed window of size p.length.",
        "Initialize frequency maps for string p and the first window of string s.",
        "Slide the fixed window right by 1: decrement count of departed character and increment count of incoming character.",
        "Record window starting index whenever the sliding window frequency matches target frequency."
    ],
    "Permutation in String": [
        "Determine if s2 contains a substring with identical character frequencies to s1.",
        "Track frequency match count between s1 table and a sliding window of size s1.length in s2.",
        "Shift window one step at a time: update counts for outgoing and incoming characters.",
        "Return true immediately if matched characters reaches 26, avoiding full map comparisons."
    ],
    "Subarrays with K Different Integers": [
        "Counting exact K distinct elements directly in a sliding window is tricky due to multiple shrink boundaries.",
        "Decompose the problem: exact(K) = atMost(K) - atMost(K - 1).",
        "Implement atMost(X): expand right pointer, shrink left pointer when distinct count exceeds X.",
        "At each step, exactly right - left + 1 valid subarrays ending at right have <= X distinct elements."
    ],
    "Frequency of the Most Frequent Element": [
        "Sort array ascending: converting elements to nums[right] costs sum(nums[right] - nums[i]).",
        "Track running sum in sliding window [left, right]; total cost is (right - left + 1) * nums[right] - windowSum.",
        "If cost exceeds budget k, shrink window from left by subtracting nums[left++].",
        "Window size right - left + 1 monotonically expands to find the maximum possible frequency."
    ],
    "Longest Consecutive Sequence": [
        "Insert all numbers into an unordered hash set for O(1) average existence lookups.",
        "Only initiate sequence counting from a streak start: num is a start if num - 1 is NOT in set.",
        "If num is a start, count consecutive elements num + 1, num + 2, ... while present in set.",
        "Each number is visited at most twice (once in set check, once in sequence walk), achieving O(N) time."
    ],
    "Majority Element": [
        "Boyer-Moore Voting Algorithm cancels out pairs of distinct elements.",
        "Maintain candidate element and counter; if counter reaches 0, adopt current element as candidate.",
        "Increment counter if element matches candidate; decrement counter otherwise.",
        "Since majority element appears > N/2 times, it survives all cancellations in O(N) time and O(1) space."
    ],
    "Majority Element II": [
        "At most two elements can have frequency strictly greater than floor(N/3).",
        "Generalize Boyer-Moore Voting using two candidate variables and two respective counters.",
        "First pass: identify top two surviving candidates through 3-way pair cancellation.",
        "Second pass: verify actual frequencies of both candidates to ensure each strictly exceeds N/3."
    ],
    "Set Mismatch": [
        "Array contains numbers 1..N with one duplicate and one missing value.",
        "Use array indices as in-place hash markers by negating the value at index abs(nums[i]) - 1.",
        "If nums[abs(nums[i]) - 1] is already negative, abs(nums[i]) is the duplicate.",
        "Second pass: the index containing a positive value reveals the missing number (index + 1)."
    ],

    # 2.cpp
    "Set Matrix Zeroes": [
        "Use the matrix's 0th row and 0th column as in-place storage to record which rows/cols to zero.",
        "Use two boolean flags to record if row 0 and col 0 initially contain any zeroes.",
        "Scan inner cells (1..N, 1..M); mark matrix[i][0] = 0 and matrix[0][j] = 0 on finding zero.",
        "Zero inner cells based on markers, then finally zero row 0 and col 0 according to flags."
    ],
    "Surrounded Regions": [
        "Any 'O' connected to the matrix boundary can never be completely surrounded by 'X'.",
        "Run DFS or BFS starting from all 'O' cells located on the matrix perimeter (4 borders).",
        "Temporarily mark perimeter-connected 'O's with a safe marker '#'.",
        "Final pass: flip remaining 'O's to 'X' (captured), and restore '#' back to 'O' (escaped)."
    ],
    "Rotting Oranges": [
        "All initially rotten oranges spread rot simultaneously at time t = 0 (multi-source BFS).",
        "Queue all rotten orange coordinates and count total fresh oranges.",
        "Process BFS level by level: infect adjacent fresh oranges (4 directions) and decrement fresh count.",
        "If fresh count reaches 0, return minutes elapsed; otherwise return -1 if isolated fresh oranges remain."
    ],
    "Zigzag Conversion": [
        "Characters bounce up and down across numRows like a ball in vertical pinball.",
        "Maintain an array of strings, one per row, and track current row index and direction (+1 or -1).",
        "Append each character to its current row string, flipping direction at row 0 and row numRows - 1.",
        "Concatenate all row strings at the end to produce the final converted output in O(N) time."
    ],
    "Valid Parentheses": [
        "A closing bracket must match the most recently opened unmatched bracket (LIFO behavior).",
        "Push opening brackets '(', '{', '[' onto a stack as they appear.",
        "When encountering a closing bracket, verify the stack is non-empty and top matches type.",
        "After processing the entire string, stack must be completely empty for parentheses to be valid."
    ],
    "Generate Parentheses": [
        "Use backtracking with remaining count of open '(' and close ')' parentheses.",
        "Can place '(' whenever openCount < n.",
        "Can place ')' only when closeCount < openCount to preserve prefix validity at all times.",
        "When openCount == closeCount == n, valid combination is formed; generated combinations follow Catalan number."
    ],
    "Group Anagrams": [
        "Anagrams share the exact same character multiset when sorted.",
        "For each string, sort its characters to produce a canonical hash map key (e.g., 'eat' -> 'aet').",
        "Append the original string to the hash map bucket keyed by its canonical form.",
        "Return all grouped bucket values; runs in O(N * K log K) time with O(N * K) space."
    ],
    "Isomorphic Strings": [
        "Requires a strict 1-to-1 bijection between characters of string s and string t.",
        "Track two mapping tables: mapS_to_T and mapT_to_S (or last seen positions in both).",
        "For each index, if existing mapping contradicts current character pair, strings are not isomorphic.",
        "If no conflict arises across all indices, the transformation is a valid bijection."
    ],
    "Median of Two Sorted Arrays": [
        "Binary search on the partition cut of the smaller array to ensure O(log(min(M, N))) runtime.",
        "Partition both arrays such that left halves contain (M + N + 1) / 2 total elements.",
        "Validate partition: maxLeftA <= minRightB and maxLeftB <= minRightA.",
        "If valid, median is max(lefts) for odd total, or average of max(lefts) and min(rights) for even."
    ],
    "Search in Rotated Sorted Array": [
        "In a rotated sorted array, at least one half [low, mid] or [mid, high] is always strictly sorted.",
        "Compare nums[low] with nums[mid] to determine which half is sorted.",
        "Check if target falls within the sorted half's boundary range.",
        "If inside, search that half; otherwise discard it and search the opposite rotated half."
    ],
    "Single Element in a Sorted Array": [
        "Before the single element, pairs start at even indices and end at odd indices (even, odd).",
        "After the single element, the pattern shifts: pairs start at odd indices and end at even indices.",
        "Binary search mid; ensure mid is even (if odd, decrement mid).",
        "If nums[mid] == nums[mid + 1], anomaly is on the right; otherwise it is at mid or on the left."
    ],

    # 3.cpp
    "Construct Binary Tree from Preorder and Inorder": [
        "Preorder traversal root is always the first element (preorder[preStart]).",
        "Locate root value in inorder traversal using a precomputed hash map for O(1) lookup.",
        "Elements left of root in inorder belong to left subtree; elements right belong to right subtree.",
        "Recursively construct left and right subtrees by calculating subtree sizes to slice preorder indices."
    ],
    "Balanced Binary Tree": [
        "A tree is height-balanced if left and right subtree heights differ by at most 1 at every node.",
        "Use bottom-up post-order DFS to compute subtree heights.",
        "If any subtree is unbalanced, immediately return -1 to short-circuit further traversal.",
        "Avoids redundant O(N^2) top-down depth recalculations, achieving clean O(N) complexity."
    ],
    "Binary Tree Maximum Path Sum": [
        "A path can turn at any node as the highest ancestor (root of path = node.val + leftGain + rightGain).",
        "Post-order traversal computes the maximum single branch gain extending upward from each node.",
        "Clamp branch gains to 0: max(0, dfs(child)) to ignore negative path contributions.",
        "Update global maximum path sum at each node with node.val + leftGain + rightGain."
    ],
    "Kth Smallest Element in a BST": [
        "In-order traversal (left, root, right) of a Binary Search Tree visits nodes in strictly sorted order.",
        "Simulate in-order traversal iteratively using a stack or recursion.",
        "Decrement counter k on visiting each node (in-order root step).",
        "When k reaches 0, current node's value is the kth smallest; terminate early in O(H + K) time."
    ],
    "Serialize and Deserialize Binary Tree": [
        "Serialization: use preorder traversal or BFS queue; serialize null children explicitly as '#' delimiters.",
        "Join node values with commas into a compact serialized string.",
        "Deserialization: tokenize string into a queue of tokens.",
        "Recursively or queue-wise construct tree: consume next token; if '#', return null; otherwise build node."
    ],
    "Diameter of Binary Tree": [
        "Longest path between any two nodes passes through some node as their lowest common ancestor.",
        "Path length through node = left subtree depth + right subtree depth.",
        "Post-order DFS calculates max depth of each subtree while updating global diameter at every node.",
        "Runs in O(N) time with O(H) recursion stack space."
    ],
    "Two Sum IV - Input is a BST": [
        "Option 1: In-order traversal produces a sorted array; apply two pointers in O(N).",
        "Option 2: Traverse tree (BFS/DFS) with a hash set checking if target - node.val was seen.",
        "Option 3: BST Iterator with two stacks (one forward, one backward) to achieve O(H) space two-pointer.",
        "Hash set approach gives concise O(N) time and O(N) space implementation."
    ],
    "All Nodes Distance K in Binary Tree": [
        "Standard binary tree pointers only go downwards; cannot traverse toward parent nodes.",
        "First pass: BFS/DFS to map each child node to its parent pointer in a hash map.",
        "Second pass: BFS starting from target node moving in 3 directions (left, right, parent).",
        "Maintain visited set to avoid backtracking; stop BFS at level K and collect all frontier nodes."
    ],
    "Vertical Order Traversal of a Binary Tree": [
        "Assign coordinates (row, col) to root (0, 0); left child is (row+1, col-1), right is (row+1, col+1).",
        "Traverse tree storing nodes grouped by col, then sorted by row, then by value on tie.",
        "Use BFS or DFS to collect coordinates into a hash table or sorted map.",
        "Extract columns in increasing column order to produce final vertical column groupings."
    ],
    "Merge BSTs to Create Single BST": [
        "Every valid BST root must match a unique leaf in another tree, except the global root.",
        "Map all tree roots by value; count leaf values across all trees to detect the candidate root.",
        "The single root that is never a leaf must be the global root of the merged tree.",
        "Traverse and graft matching leaves; validate final unified tree satisfies BST range invariants."
    ],
    "Find Eventual Safe States": [
        "A node is safe if every possible directed path from it leads to a terminal node (no cycles).",
        "Nodes in or leading into a cycle are unsafe.",
        "Use 3-color DFS cycle detection: 0 = unvisited, 1 = visiting (active stack), 2 = safe.",
        "If a node's neighbor is currently visiting (1), a cycle exists; mark node unsafe. Otherwise mark safe (2)."
    ],
    "Making A Large Island": [
        "First pass: find all connected components of 1s using DFS/BFS; label each island with unique ID (2, 3, ...).",
        "Record the area of each island ID in a lookup hash map.",
        "Second pass: check every 0 cell in the grid; inspect unique adjacent island IDs in 4 directions.",
        "Potential area = 1 + sum of areas of distinct neighbor islands; global max is the answer."
    ],
    "Reorder Routes to Make All Paths Lead to City Zero": [
        "Build an undirected adjacency list where original directed edges are flagged (e.g. forward = 1, backward = 0).",
        "Start BFS/DFS from city 0, visiting all connected nodes outward.",
        "If traversing an edge in the original forward direction (away from 0), it must be reversed (count++).",
        "Tree structure guarantees exactly N - 1 edges visited with zero cycle checks."
    ],
    "Path Existence Queries in a Graph I": [
        "Determine if paths exist between pairs of nodes under connectivity constraints.",
        "Use Disjoint Set Union (DSU) or BFS/DFS connected component labeling.",
        "Union all connected edges into disjoint sets with path compression and rank.",
        "Answer each query in O(1) by checking if find(u) == find(v)."
    ],
    "Longest Valid Parentheses": [
        "Stack stores indices of unmatched parentheses; initialize stack with -1 as base index boundary.",
        "When encountering '(': push its index onto stack.",
        "When encountering ')': pop top index.",
        "If stack empty: push current index as new boundary. If non-empty: maxLen = max(maxLen, i - stack.top())."
    ],
    "Largest Rectangle in Histogram": [
        "For each bar, maximum rectangle height is bounded by the first smaller bar to its left and right.",
        "Use a monotonic increasing stack storing bar indices.",
        "When current bar is smaller than stack top, pop stack top as the rectangle height.",
        "Width is bounded by current index (right limit) and new stack top (left limit); runs in O(N)."
    ],
    "Maximal Rectangle": [
        "Convert 2D binary matrix into consecutive rows of 1D histogram heights.",
        "For each row, if matrix[r][c] == '1', height[c] += 1; else height[c] = 0.",
        "Apply Largest Rectangle in Histogram (monotonic stack) algorithm on each row's height array.",
        "Tracks global maximum rectangle area in O(M * N) time."
    ],
    "Min Stack": [
        "Design a stack supporting push, pop, top, and getMin in O(1) time.",
        "Maintain a parallel min-stack, or store pairs (val, currentMin) in the main stack.",
        "On push(x): push (x, min(x, currentMin)).",
        "getMin() simply inspects the min value of the top element in O(1) time."
    ],
    "Basic Calculator": [
        "Use a stack to manage signs and parenthetical contexts during left-to-right evaluation.",
        "Maintain running result, current number, and current sign (+1 or -1).",
        "On '(': push current result and sign onto stack, reset result and sign for inner sub-expression.",
        "On ')': complete sub-expression, multiply by popped sign, and add to popped previous result."
    ],
    "Sliding Window Maximum": [
        "Maintain a monotonic decreasing deque storing indices whose corresponding values are strictly decreasing.",
        "Remove elements from back of deque that are smaller than current element (they can never be max).",
        "Remove elements from front of deque that fall outside sliding window boundary (index <= i - k).",
        "Front of deque always holds the maximum element for the current window in amortized O(N) time."
    ],
    "Find K Pairs with Smallest Sums": [
        "Arrays nums1 and nums2 are sorted; pairs (nums1[i], nums2[j]) form a 2D sorted matrix.",
        "Initialize min-heap with (nums1[i] + nums2[0], i, 0) for i from 0 to min(k, len(nums1)).",
        "Pop smallest pair (i, j); push pair to result.",
        "Push next candidate from same row: (nums1[i] + nums2[j + 1], i, j + 1) until k pairs collected."
    ],
    "Next Greater Element I": [
        "Use a monotonic decreasing stack to find the next greater element for all elements in nums2.",
        "Iterate nums2: while stack top < current element, pop stack top and record mapping top -> current.",
        "Push current element onto stack; remaining stack elements have no next greater (-1).",
        "Answer nums1 queries in O(1) each using the precomputed hash map."
    ],

    # 4.cpp
    "Next Greater Element II": [
        "Array is circular: duplicate iteration virtually by traversing indices from 0 to 2*N - 1 using index % N.",
        "Maintain monotonic decreasing stack of indices.",
        "While stack top value < nums[i % N], pop stack top and assign nums[i % N] as its next greater element.",
        "Push index onto stack only during the first pass (i < N) to avoid redundant entries."
    ],
    "Jump Game VI": [
        "DP state: dp[i] = max score to reach index i = nums[i] + max(dp[i-k .. i-1]).",
        "Naive sliding window max search takes O(N * K), which TLEs for large K.",
        "Optimize transition with a monotonic decreasing deque storing indices sorted by dp value.",
        "Pop outdated indices (> i - k) from front, transition in O(1), and pop smaller values from back."
    ],
    "Regular Expression Matching": [
        "2D DP table: dp[i][j] indicates if s[0..i-1] matches pattern p[0..j-1].",
        "If p[j-1] is a char or '.': dp[i][j] = dp[i-1][j-1] && (s[i-1] == p[j-1] || p[j-1] == '.').",
        "If p[j-1] == '*': zero occurrences (dp[i][j-2]) OR one/more occurrences (dp[i-1][j] if char matches).",
        "Base case: dp[0][0] = true; handle leading empty matches with '*' appropriately."
    ],
    "Unique Paths": [
        "Robot moves only right and down: dp[i][j] = dp[i-1][j] + dp[i][j-1].",
        "Top and left edges have exactly 1 unique path to reach each cell.",
        "Optimize space to 1D array of size N: dp[j] += dp[j-1].",
        "Can also be computed in O(min(M, N)) using combinatorics: C(M + N - 2, M - 1)."
    ],
    "Edit Distance": [
        "dp[i][j] = minimum operations to convert word1[0..i-1] to word2[0..j-1].",
        "If word1[i-1] == word2[j-1], characters match: dp[i][j] = dp[i-1][j-1].",
        "Otherwise take 1 + min of insert (dp[i][j-1]), delete (dp[i-1][j]), or replace (dp[i-1][j-1]).",
        "Space can be optimized to O(min(M, N)) using two 1D rows."
    ],
    "Triangle": [
        "Bottom-up DP avoids boundary edge cases and requires zero top-down memoization overhead.",
        "Initialize dp array with the values of the bottom row of the triangle.",
        "Iterate from second-to-bottom row up to root: dp[j] = triangle[i][j] + min(dp[j], dp[j+1]).",
        "When loop completes, dp[0] holds the minimum path sum in O(N^2) time and O(N) space."
    ],
    "Best Time to Buy and Sell Stock": [
        "Max profit on day i requires buying at the minimum stock price observed prior to day i.",
        "Single pass maintains running minimum price: minPrice = min(minPrice, price[i]).",
        "Calculate potential profit: profit = price[i] - minPrice.",
        "Update global max profit; runs in O(N) time and O(1) space."
    ],
    "House Robber": [
        "At house i, robber can either rob house i (nums[i] + rob[i-2]) or skip it (rob[i-1]).",
        "Recurrence: dp[i] = max(dp[i-1], nums[i] + dp[i-2]).",
        "Only the previous two states are needed to compute the current state.",
        "Rolling two variables (prev1, prev2) achieves O(N) time with O(1) auxiliary space."
    ],
    "Longest Increasing Subsequence": [
        "Patience sorting algorithm: maintain array tails where tails[len] is the smallest tail of all IS of length len + 1.",
        "For each x, binary search (std::lower_bound) in tails to find first element >= x.",
        "If x is greater than all tails, extend sequence by appending x.",
        "Otherwise replace the found element with x to keep future tails as small as possible; runs in O(N log N)."
    ],
    "Partition Equal Subset Sum": [
        "Problem reduces to 0/1 Knapsack: find subset summing to exactly totalSum / 2.",
        "If totalSum is odd, equal partition into integers is impossible (return false).",
        "dp[w] represents whether subset sum w is achievable; iterate backwards: dp[w] |= dp[w - num].",
        "Can be highly optimized using std::bitset for blazing fast O(N * S / 64) execution."
    ],
    "Reverse Pairs": [
        "Condition nums[i] > 2 * nums[j] with i < j mirrors inversion counting in Merge Sort.",
        "Divide array into left and right sorted halves.",
        "Before merging, run two pointers: for each left element, count right elements satisfying nums[i] > 2 * nums[j].",
        "Merge two sorted halves standardly; total time complexity is O(N log N)."
    ],
    "Longest Palindromic Subsequence": [
        "Equivalent to finding the Longest Common Subsequence (LCS) between string s and reverse(s).",
        "Interval DP approach: dp[i][j] is LPS of substring s[i..j].",
        "If s[i] == s[j]: dp[i][j] = 2 + dp[i+1][j-1].",
        "If s[i] != s[j]: dp[i][j] = max(dp[i+1][j], dp[i][j-1]); runs in O(N^2) time and O(N) space."
    ],
    "Coin Change II & Coin Change I": [
        "Unbounded knapsack formulation where each coin denomination can be reused infinitely.",
        "For Coin Change I (Min Coins): dp[a] = min(dp[a], 1 + dp[a - coin]), outer loop coins.",
        "For Coin Change II (Total Combos): dp[a] += dp[a - coin], outer loop coins prevents permutations.",
        "Both problems solve cleanly in 1D space O(amount) and O(N * amount) time."
    ],
    "Beautiful Arrangement": [
        "Backtracking with state compression (bitmask) to place numbers 1..N at valid positions.",
        "Number i can be placed at position pos if i % pos == 0 or pos % i == 0.",
        "Track used numbers with an integer bitmask (mask | (1 << i)).",
        "Memoize dp[mask] to avoid solving identical sub-arrangements repeatedly."
    ],
    "K-th Symbol in Grammar": [
        "Observe tree structure: each row doubles in size; parent of index k is (k + 1) / 2 in row n - 1.",
        "If k is odd, k-th symbol equals parent symbol.",
        "If k is even, k-th symbol is the inverted parent symbol (1 - parent).",
        "Recursively trace back to root in O(N) time with O(1) space, equivalent to counting bits in k - 1."
    ],
    "Number of Zigzag Arrays I": [
        "Dynamic programming tracking alternating directional transitions (increasing vs decreasing).",
        "State: dp[i][val][dir] represents valid arrays of length i ending with val moving in direction dir.",
        "Prefix sums accelerate range sum queries over previous values from O(M) to O(1) per transition.",
        "Reduces total runtime from O(N * M^2) down to O(N * M)."
    ],
    "Combination Sum": [
        "Backtracking with reuse: since candidates can be picked multiple times, recursive step does not increment index.",
        "Sort candidates ascending to enable early pruning when candidate > remaining target.",
        "Push candidate to path, recurse with target - candidate, pop candidate on return.",
        "Guarantees all combinations sum to target with zero duplicate permutations."
    ],
    "N-Queens": [
        "Place one queen per row, ensuring no two queens share the same column or diagonal.",
        "Track occupied columns and diagonals using hash sets or boolean bitmasks.",
        "Main diagonal: row - col is constant; Anti-diagonal: row + col is constant.",
        "Pruning illegal attacks immediately cuts backtracking branches, achieving optimal O(N!) search."
    ],
    "Subsets": [
        "Every element has two binary choices: either include it in current subset or exclude it.",
        "Can be solved via DFS backtracking or bit manipulation from 0 to (1 << N) - 1.",
        "In bitmasking, the j-th bit of integer i indicates whether nums[j] is included in subset i.",
        "Generates all 2^N subsets in O(N * 2^N) time."
    ],
    "Subsets II": [
        "Array may contain duplicates; must avoid duplicate subsets in output.",
        "Sort array so identical elements are contiguous.",
        "In backtracking loop, skip element if nums[i] == nums[i - 1] and i > startIndex.",
        "Ensures identical values are only picked in sequential depth order, preventing branching duplicates."
    ],
    "Concatenated Words & Word Break II": [
        "Store all words in a HashSet or Prefix Trie for fast dictionary lookups.",
        "For each word, check if it can be formed by concatenating two or more shorter words.",
        "Use memoized DFS or 1D DP: canForm(word, index) checks if prefix is in set and suffix canForm.",
        "Sort words by length ascending so shorter component words are validated first."
    ],
    "Reverse Integer": [
        "Extract digits from right using digit = x % 10 and shift x = x / 10.",
        "Check for 32-bit signed integer overflow before multiplying current result by 10.",
        "Overflow condition: rev > INT_MAX / 10 or (rev == INT_MAX / 10 and digit > 7).",
        "Underflow condition: rev < INT_MIN / 10 or (rev == INT_MIN / 10 and digit < -8)."
    ],
    "Pow(x, n)": [
        "Binary Exponentiation computes x^n in O(log N) multiplications instead of linear O(N).",
        "If n is even, x^n = (x^(n/2))^2; if n is odd, x^n = x * x^(n-1).",
        "Handle negative powers by setting x = 1/x and using 64-bit int for n to prevent overflow on INT_MIN.",
        "Iterative bitwise approach squares base and multiplies result when current bit of n is 1."
    ],
    "Single Number II": [
        "Every element appears three times except one element which appears once.",
        "Digital logic state machine tracks bit counts modulo 3 using two variables (ones, twos).",
        "ones = (ones ^ x) & ~twos: captures bits appearing 1st and 4th times.",
        "twos = (twos ^ x) & ~ones: captures bits appearing 2nd and 5th times; both clear on 3rd."
    ],
    "Count Primes": [
        "Sieve of Eratosthenes efficiently marks composites up to N.",
        "Initialize boolean vector isPrime of size N with true; mark 0 and 1 as false.",
        "Iterate i from 2 up to sqrt(N); if isPrime[i], mark all multiples i*i, i*i + i, ... as false.",
        "Count surviving true entries; achieves O(N log log N) time with O(N) space."
    ],
    "Rectangle Area": [
        "Total area covered by two rectilinear rectangles = Area(A) + Area(B) - OverlapArea.",
        "Calculate individual areas: width * height for each rectangle.",
        "Calculate overlap width: max(0, min(ax2, bx2) - max(ax1, bx1)).",
        "Calculate overlap height: max(0, min(ay2, by2) - max(ay1, by1)); subtract overlap to avoid double-counting."
    ],
    "H-Index": [
        "A researcher has index H if H papers have at least H citations each, and other papers <= H citations.",
        "Sort citations descending: scan until citations[i] < i + 1; maximum valid i + 1 is H.",
        "Optimal O(N) Bucket Sort: count papers by citation count up to N (cap citations > N at N).",
        "Accumulate paper counts from N down to 0; first index where accumulated papers >= count is H."
    ],
    "Consecutive Numbers Sum": [
        "Represent N as sum of k consecutive integers: N = x + (x+1) + ... + (x+k-1) = k*x + k*(k-1)/2.",
        "Rearrange equation: k*x = N - k*(k-1)/2, requiring (N - k*(k-1)/2) > 0 and divisible by k.",
        "Loop k starting from 1 while k*(k-1)/2 < N.",
        "Increment count whenever (N - k*(k-1)/2) % k == 0; terminates in O(sqrt(N)) iterations."
    ],

    # 5.cpp
    "Reverse Linked List": [
        "Maintain three pointers: prev (null), curr (head), and nextNode.",
        "Save curr->next into nextNode to preserve reference to remainder of the list.",
        "Reverse current pointer: curr->next = prev.",
        "Advance prev = curr and curr = nextNode until curr is null; return prev as new head."
    ],
    "Reverse Linked List II": [
        "Navigate dummy node and advance pointer to node left - 1 (node preceding reversal window).",
        "Use pointer manipulation to repeatedly move curr->next to the front of the sublist.",
        "Preserve connections: pre->next connects to inverted node, curr->next bridges to remainder.",
        "Single pass O(N) modifies pointers in-place without needing list cut/reconnection."
    ],
    "Reverse Nodes in k-Group": [
        "Count ahead k nodes; if fewer than k nodes remain, leave them unchanged.",
        "Reverse the current k nodes standardly using iterative pointer reversal.",
        "Connect previous group's tail to current group's new head.",
        "Recursively or iteratively proceed to next k-group; achieves O(N) time and O(1) space."
    ],
    "Merge Two Sorted Lists": [
        "Create dummy head node to eliminate special edge cases for list head initialization.",
        "Compare head values of list1 and list2; attach smaller node to tail of merged list.",
        "Advance chosen list's pointer and tail pointer.",
        "Attach remaining non-null list suffix directly to tail in O(1) step; runs in O(N + M)."
    ],
    "Merge k Sorted Lists": [
        "Min-heap stores current head node of each of the K sorted lists.",
        "Pop smallest node from heap, attach to merged list tail.",
        "If popped node has a next pointer, push node->next into min-heap.",
        "Heap size stays bounded by K, ensuring O(N log K) time where N is total node count."
    ],
    "Sort List": [
        "Apply Merge Sort on linked list to achieve guaranteed O(N log N) time and O(log N) stack.",
        "Use Slow/Fast pointer technique to locate exact middle node of list.",
        "Sever middle node (mid->next = null) to split into two independent sublists.",
        "Recursively sort both halves and merge using standard Merge Two Sorted Lists routine."
    ],
    "Linked List Cycle": [
        "Floyd's Tortoise and Hare algorithm uses two pointers moving at different speeds.",
        "Slow pointer moves 1 step per iteration; Fast pointer moves 2 steps per iteration.",
        "If list contains a cycle, fast pointer will inevitably lap and meet slow pointer.",
        "If fast pointer hits null, list is acyclic; runs in O(N) time and O(1) space."
    ],
    "Linked List Cycle II": [
        "First phase: run Floyd's Tortoise and Hare until slow and fast pointers collide.",
        "Mathematical proof: distance from head to cycle entrance equals distance from meeting point to cycle entrance.",
        "Second phase: reset slow pointer to head, keep fast pointer at meeting point.",
        "Advance both pointers 1 step at a time; the node where they meet is the exact cycle entrance."
    ],
    "Middle of the Linked List": [
        "Use two pointers (slow and fast) starting at head.",
        "Slow pointer advances 1 node; Fast pointer advances 2 nodes.",
        "When fast reaches null (even length) or fast->next is null (odd length), slow is at middle.",
        "Single pass O(N) avoids two-pass length counting."
    ],
    "Remove Nth Node From End of List": [
        "Use dummy head to cleanly handle edge case of removing the first node.",
        "Advance fast pointer n + 1 steps ahead of slow pointer to establish fixed gap.",
        "Advance both slow and fast pointers together until fast hits null.",
        "slow->next points directly to target node; bypass target with slow->next = slow->next->next."
    ],
    "Reorder List": [
        "Step 1: Find middle of linked list using slow and fast pointers.",
        "Step 2: Split list and reverse the second half completely.",
        "Step 3: Interleave nodes alternately from first half and reversed second half.",
        "In-place O(N) time and O(1) space modification without external arrays."
    ],
    "Palindrome Linked List": [
        "Find middle of linked list using slow and fast pointers.",
        "Reverse second half of list starting from middle node.",
        "Compare values node-by-node between first half and reversed second half.",
        "Optionally restore original list structure; achieves O(N) time and O(1) space."
    ],
    "Odd Even Linked List": [
        "Maintain two separate sublist pointers: odd head/tail and even head/tail.",
        "Iterate list: odd->next = even->next; odd = odd->next; even->next = odd->next; even = even->next.",
        "Splits odd-indexed and even-indexed nodes in-place in a single pass.",
        "Connect odd tail to preserved even head at the end."
    ],
    "Swap Nodes in Pairs": [
        "Use dummy head pointing to head of list.",
        "Maintain prev pointer; identify first = prev->next and second = first->next.",
        "Swap pointers: first->next = second->next; second->next = first; prev->next = second.",
        "Advance prev to first and repeat while prev->next and prev->next->next exist."
    ],
    "Rotate List": [
        "Traverse list to calculate total length and locate tail node.",
        "Connect tail->next back to head to form a temporary circular ring.",
        "Calculate effective rotations k = k % len; new tail is at index len - k - 1 from start.",
        "Advance to new tail, set newHead = newTail->next, and sever newTail->next = null."
    ],
    "Partition List": [
        "Create two dummy head chains: lessHead (nodes < x) and greaterHead (nodes >= x).",
        "Traverse original list; append each node to lessTail or greaterTail based on val.",
        "Sever greaterTail->next = null to prevent cycles.",
        "Connect lessTail->next = greaterHead->next to merge both partitions preserving original relative order."
    ],
    "Remove Duplicates from Sorted List": [
        "List is already sorted, so duplicate values are strictly adjacent.",
        "Traverse with curr pointer; compare curr->val with curr->next->val.",
        "If equal, bypass duplicate: curr->next = curr->next->next without advancing curr.",
        "If different, advance curr = curr->next; runs in O(N) time and O(1) space."
    ],
    "Remove Duplicates from Sorted List II": [
        "Must remove all nodes that have duplicate numbers, leaving only distinct numbers.",
        "Use dummy head pointing to head in case initial nodes are duplicates.",
        "Detect consecutive identical values: while curr->next and curr->next->next have same val, advance.",
        "If duplicates detected, link prev->next = curr->next->next; else advance prev."
    ],
    "Remove Linked List Elements": [
        "Use dummy head pointing to original list head to eliminate special case of removing head.",
        "Traverse list with curr pointer starting at dummy.",
        "If curr->next->val == target, unlink node: curr->next = curr->next->next.",
        "Only advance curr = curr->next when no deletion occurred at the current step."
    ],
    "Add Two Numbers": [
        "Simulate column-by-column addition starting from least significant digit (head).",
        "Maintain running carry = sum / 10 and digit = sum % 10.",
        "Continue loop while list1, list2, or non-zero carry remains.",
        "Attach new digit node to dummy head; handles lists of unequal lengths cleanly."
    ],
    "Add Two Numbers II": [
        "Numbers are stored in most significant digit first order; cannot reverse inputs directly.",
        "Push digits of list1 and list2 onto two separate stacks to access least significant digits first.",
        "Pop from stacks, compute sum with carry, and prepend new nodes to result list.",
        "Prepending new node (node->next = head; head = node) avoids reversing the final output list."
    ],
    "Intersection of Two Linked Lists": [
        "Two pointers ptrA and ptrB start at headA and headB respectively.",
        "Advance each pointer 1 step; when ptrA reaches end, redirect it to headB; similarly ptrB to headA.",
        "Both pointers travel total distance of lenA + lenB, equalizing path length differences.",
        "Pointers will collide at intersection node, or both hit null if disjoint."
    ],
    "Copy List with Random Pointer": [
        "Step 1: Interleave clone nodes immediately after original nodes (curr->next = clone).",
        "Step 2: Assign random pointers for clones: curr->next->random = curr->random->next.",
        "Step 3: Separate original list and cloned list by restoring original next pointers.",
        "Achieves deep copy in O(N) time with O(1) extra auxiliary space."
    ],
    "Flatten a Multilevel Doubly Linked List": [
        "Iterate through list; when a node with child pointer is encountered, flatten child branch.",
        "Save curr->next into a stack or traverse to end of child chain.",
        "Splice child chain directly between curr and curr->next, updating prev and next pointers.",
        "Set child pointer to null; continue traversal along newly flattened chain."
    ],
    "Convert BST to Sorted Doubly Linked List": [
        "In-order traversal (left, root, right) of BST visits nodes in strictly ascending order.",
        "Maintain prev pointer to most recently visited node and first pointer to smallest node.",
        "Link prev->right = curr and curr->left = prev during in-order visit.",
        "At end of traversal, close circular ring by linking first->left = prev and prev->right = first."
    ],
    "Split Linked List in Parts": [
        "Count total list length N to determine part sizes.",
        "Base part size is N / k; the first N % k parts receive 1 extra node for even distribution.",
        "Traverse list, chopping off sublists of computed lengths by severing tail->next = null.",
        "Store each part head in result vector, padding with null pointers if k > N."
    ],
    "Next Greater Node In Linked List": [
        "Convert linked list values into an array for indexed lookups.",
        "Maintain a monotonic decreasing stack storing indices of elements.",
        "For each value, pop indices with smaller values and record current value as their next greater.",
        "Push current index to stack; unpopped indices default to 0."
    ],
    "Insert into a Sorted Circular Linked List": [
        "Traverse circular list looking for insertion slot: prev->val <= insertVal <= curr->val.",
        "Also detect maximum-to-minimum boundary (prev->val > curr->val): insert if val is global min or max.",
        "If entire cycle completes without condition match (all nodes equal), insert anywhere.",
        "Splice new node between prev and curr: prev->next = new Node(insertVal, curr)."
    ],
    "Merge In Between Linked Lists": [
        "Traverse list1 to find node at index a - 1 (preA) and node at index b + 1 (postB).",
        "Connect preA->next to head of list2.",
        "Traverse to the tail of list2.",
        "Connect list2 tail->next to postB; completes in-place splice in O(N + M) time."
    ],
    "Delete Node in a Linked List": [
        "Only given access to target node to be deleted, not the head of the list.",
        "Cannot modify predecessor pointer directly.",
        "Copy value of next node into target node: node->val = node->next->val.",
        "Bypass next node: node->next = node->next->next in O(1) time."
    ],
    "Design Linked List": [
        "Implement custom doubly-linked list with dummy head and tail to eliminate boundary edge cases.",
        "Maintain size counter to validate index bounds in O(1).",
        "get(index): optimize traversal by starting from head if index < size/2, else from tail.",
        "addAtIndex and deleteAtIndex relink prev and next pointers in O(index) time."
    ],
    "LRU Cache": [
        "Requires O(1) get and put operations; combine Hash Map with Doubly-Linked List.",
        "Hash map stores key -> Node pointer; Doubly linked list maintains access recency with dummy head and tail.",
        "On access (get or put overwrite), splice node and move it directly behind head (most recently used).",
        "On capacity overflow, evict node immediately preceding tail (least recently used) and erase from map."
    ],
    "LFU Cache": [
        "Track access frequency and recency: combine key-to-node map with freq-to-doubly-linked-list map.",
        "Maintain minFreq variable representing current minimum frequency across all stored items.",
        "On access, increment item's frequency, move it to new frequency list, and update minFreq if needed.",
        "On eviction, remove least recently used item from minFreq list and clean up in O(1) time."
    ],
    "Find the Duplicate Number": [
        "Array of size N + 1 with values in range 1..N forms a functional graph where i -> nums[i].",
        "Since at least two indices point to same value, a cycle must exist.",
        "Apply Floyd's Tortoise and Hare cycle detection algorithm directly on array indices.",
        "Phase 1 finds collision; Phase 2 resets one pointer to start and steps to find cycle entrance (duplicate)."
    ],
    "Design Browser History": [
        "Doubly-linked list or dynamic array with current pointer efficiently simulates browser history.",
        "visit(url): clear forward history, append new url, and advance current pointer.",
        "back(steps): retreat current pointer by min(steps, stepsAvailableToRoot).",
        "forward(steps): advance current pointer by min(steps, stepsAvailableToTail) in O(steps) or O(1) array."
    ]
}

def get_greedy_intuition(name, lc, pattern, cat):
    n = name.lower()
    p = pattern.lower()

    if "assign cookies" in n:
        return [
            "Sort greed factors of children and sizes of cookies ascending.",
            "Two pointers match smallest cookie that satisfies the current least greedy child.",
            "If cookie satisfies child, both pointers advance; otherwise advance cookie pointer.",
            "Greedy choice maximizes total satisfied children in O(N log N) time."
        ]
    if "buy and sell stock ii" in n:
        return [
            "Any positive price difference between adjacent days represents an exploitable profit.",
            "Whenever prices[i] > prices[i - 1], execute a virtual buy on day i-1 and sell on day i.",
            "Summing all local price ascents equals the maximum possible cumulative profit.",
            "Single pass O(N) time with O(1) space."
        ]
    if "jump game" in n:
        return [
            "Track the maximum index reachable so far with a running variable maxReach.",
            "Iterate through array: if current index i > maxReach, position is unreachable (return false).",
            "Update maxReach = max(maxReach, i + nums[i]).",
            "If maxReach >= n - 1 at any point, the destination is guaranteed reachable."
        ]
    if "candy" in n:
        return [
            "Each child must receive at least 1 candy; higher rated children need more candies than neighbors.",
            "Left-to-right pass: if rating[i] > rating[i - 1], candies[i] = candies[i - 1] + 1.",
            "Right-to-left pass: if rating[i] > rating[i + 1], candies[i] = max(candies[i], candies[i + 1] + 1).",
            "Sum candies across all children in O(N) time and O(N) space."
        ]
    if "lemonade change" in n:
        return [
            "Customer pays with $5, $10, or $20 bills; track available count of $5 and $10 bills.",
            "$10 bill requires giving a $5 bill in change.",
            "$20 bill: greedily give one $10 bill and one $5 bill first, preserving flexible $5 bills.",
            "If cannot make change at any point, immediately return false."
        ]
    if "can place flowers" in n:
        return [
            "Flowers cannot be planted in adjacent plots (need empty left, center, and right).",
            "Scan flowerbed: if current plot is 0 and both neighbors (with virtual boundary 0s) are 0, plant flower.",
            "Mark current plot as 1 and decrement required planting count n.",
            "Early exit if n <= 0; runs in O(N) time and O(1) space."
        ]
    if "maximum units on a truck" in n:
        return [
            "Sort box types descending by units per box to maximize packing density.",
            "Greedily load boxes of highest unit yield until truck size is exhausted.",
            "Take min(truckSize, numberOfBoxes) for each box type and multiply by units.",
            "Runs in O(N log N) sorting time and O(1) extra space."
        ]
    if "meeting rooms" in n:
        return [
            "Sort meetings by start time.",
            "Iterate through meetings: check if next meeting starts before current meeting ends.",
            "If intervals overlap, a person cannot attend both meetings (return false).",
            "If no overlaps exist across entire sorted list, return true in O(N log N) time."
        ]
    if "meeting rooms ii" in n:
        return [
            "Sort meeting intervals by start time.",
            "Use a min-heap storing end times of active meetings in used conference rooms.",
            "If next meeting starts after earliest ending meeting (heap top), reuse that room (pop heap).",
            "Push next meeting end time to heap; maximum heap size represents minimum rooms needed."
        ]
    if "minimum number of arrows" in n:
        return [
            "Sort balloons by end coordinate ascending.",
            "Greedily shoot an arrow at the end coordinate of the first balloon.",
            "This arrow bursts all subsequent balloons whose start coordinate <= arrow position.",
            "When a balloon starts after current arrow position, fire a new arrow at its end coordinate."
        ]
    if "non-overlapping intervals" in n:
        return [
            "Equivalent to maximizing non-overlapping intervals (Interval Scheduling problem).",
            "Sort intervals ascending by end time.",
            "Greedily retain interval that finishes earliest to leave maximum room for future intervals.",
            "Answer is total intervals minus maximum compatible intervals retained."
        ]
    if "car pooling" in n:
        return [
            "Use Difference Array or Sweepline bucket to track net passenger changes at each location.",
            "For each trip [num, from, to], add +num at 'from' and subtract -num at 'to'.",
            "Compute prefix sum along locations to track current passenger load.",
            "If passenger load exceeds capacity at any stop, return false."
        ]
    if "two city scheduling" in n:
        return [
            "Send all 2N people to City A initially.",
            "Compute the refund or additional cost to send person i to City B instead: costB - costA.",
            "Sort people by (costB - costA) ascending.",
            "Greedily send the first N people with the largest savings to City B."
        ]
    if "boats to save people" in n:
        return [
            "Each boat carries at most 2 people and has a weight limit.",
            "Sort people by weight ascending; use two pointers (lightest at left, heaviest at right).",
            "If lightest and heaviest can share a boat (sum <= limit), advance left pointer.",
            "Heaviest person always boards current boat (retreat right pointer); increment boat count."
        ]
    if "remove k digits" in n:
        return [
            "To minimize number magnitude, smaller digits should appear at higher place values (left).",
            "Use a monotonic increasing stack: pop larger previous digits when a smaller digit arrives.",
            "Decrement k with each pop; stop popping when k reaches 0.",
            "Strip leading zeroes and handle edge case where all digits are removed."
        ]
    if "queue reconstruction by height" in n:
        return [
            "Sort people by height descending; on height tie, sort by k ascending.",
            "Taller people are processed first and are invisible to shorter people.",
            "Insert each person at index k of the output list.",
            "Since all previously placed people are >= current person's height, inserting at index k is always valid."
        ]
    if "gas station" in n:
        return SPECIFIC_INTUITIONS["Gas Station"]
    if "hand of straights" in n or "divide array in sets of k" in n:
        return [
            "Count card frequencies in a sorted map or hash map with sorted keys.",
            "Identify smallest available card v; it must form a consecutive group [v, v+1, ..., v+groupSize-1].",
            "Decrement count for all cards in the consecutive group.",
            "If any required consecutive card is missing or exhausted, partitioning is impossible."
        ]
    if "reorganize string" in n:
        return [
            "Most frequent character dictates feasibility: if maxFreq > (N + 1) / 2, impossible.",
            "Max-heap stores character frequencies.",
            "Pop top two most frequent characters, append to result, decrement counts, and push back.",
            "Pairing distinct characters guarantees no two adjacent characters are identical."
        ]
    if "minimum add to make parentheses valid" in n:
        return [
            "Track open bracket balance as string is scanned from left to right.",
            "Increment open count on '('; on ')', decrement open if open > 0, else increment needed additions.",
            "At end of scan, any unmatched open brackets must also be closed.",
            "Total additions needed = additions + remaining open brackets."
        ]
    if "minimum remove to make valid parentheses" in n:
        return [
            "Use a stack storing indices of unmatched '(' brackets.",
            "When encountering ')', pop stack if non-empty; otherwise mark ')' for deletion.",
            "After scan, all indices remaining in stack represent unmatched '(' and are marked for deletion.",
            "Build filtered string omitting all marked invalid indices in O(N) time."
        ]
    if "valid parenthesis string" in n:
        return [
            "Asterisk '*' can act as '(', ')', or empty string.",
            "Maintain a range of possible open bracket counts [cmin, cmax].",
            "'(': cmin++, cmax++; ')': cmin--, cmax--; '*': cmin-- (as ')'), cmax++ (as '(').",
            "Clamp cmin to 0; if cmax < 0 at any point, return false. Valid if cmin == 0 at end."
        ]
    if "monotone increasing digits" in n:
        return [
            "Convert number to string; scan digits from right to left.",
            "If digits[i-1] > digits[i], decrement digits[i-1] and mark position to fill subsequent digits with '9'.",
            "Decrementing digits[i-1] ensures monotonicity with respect to the left neighbor.",
            "Set all digits from the first modified position to the end to '9' to maximize value."
        ]
    if "advantage shuffle" in n:
        return [
            "Sort nums1 and keep track of original indices of sorted nums2 (Tian Ji horse race).",
            "Use two pointers on sorted nums2 (smallest at left, largest at right).",
            "If smallest available in nums1 > smallest in nums2, assign it to beat nums2.",
            "Otherwise, sacrifice this smallest nums1 card against the largest element of nums2."
        ]
    if "minimum cost to hire k workers" in n:
        return [
            "Each worker must be paid proportionally to quality: wage[i] = quality[i] * maxRatio.",
            "Sort workers by wage-to-quality ratio ascending.",
            "Maintain a max-heap of qualities of size K to minimize total quality sum.",
            "For each worker ratio, totalCost = currentRatio * qualitySum; track minimum cost."
        ]
    if "course schedule iii" in n:
        return [
            "Sort courses by lastDay ascending so deadlines are respected in chronological order.",
            "Maintain a max-heap of durations of courses taken so far.",
            "Add course: if total time exceeds current course deadline, drop course with largest duration.",
            "Dropping longest course always preserves maximum courses completed while reducing total time."
        ]
    if "maximum performance of a team" in n:
        return [
            "Performance = sum(speeds) * min(efficiencies).",
            "Sort engineers descending by efficiency so current engineer is always minimum efficiency.",
            "Maintain min-heap of speeds of size up to K.",
            "Track running speed sum; update global performance at each step in O(N log K)."
        ]
    if "furthest building you can reach" in n:
        return [
            "Ladders should greedily cover the largest height climbs; bricks cover smaller climbs.",
            "Maintain a min-heap of size ladders storing the largest climb heights encountered.",
            "When heap size exceeds ladders, pop smallest climb and pay for it using bricks.",
            "If bricks drop below 0, return current building index as the furthest reachable."
        ]
    if "minimum number of refueling stops" in n:
        return [
            "Travel as far as current fuel allows.",
            "When encountering fuel stations along the way, push their fuel amounts to a max-heap.",
            "When fuel runs out before reaching next station or target, greedily refuel from max-heap.",
            "If heap is empty and fuel is exhausted, destination is unreachable."
        ]
    if "russian doll envelopes" in n:
        return [
            "Sort envelopes by width ascending; for identical widths, sort height descending.",
            "Sorting height descending prevents picking multiple envelopes with the exact same width.",
            "Apply Longest Increasing Subsequence (LIS) on heights using binary search patience sorting.",
            "Finds maximum nested envelopes in O(N log N) time."
        ]
    if "activity selection" in n or "maximum meetings in one room" in n:
        return [
            "Sort activities by finishing time ascending.",
            "Greedily select the activity that finishes earliest.",
            "For each subsequent activity, select it if its start time >= end time of last chosen activity.",
            "Optimal greedy choice leaves maximum remaining time for subsequent activities."
        ]
    if "job sequencing problem" in n:
        return [
            "Sort jobs in descending order of profit to prioritize high-yield tasks.",
            "Assign each job to the latest available slot before or at its deadline.",
            "Use Disjoint Set Union (DSU) to locate the next free available slot in near O(1) time.",
            "Maximizes total profit within available deadlines in O(N log N) time."
        ]
    if "connect ropes with minimum cost" in n:
        return [
            "Connecting two ropes of lengths A and B incurs cost A + B; smaller ropes contribute repeatedly.",
            "Insert all rope lengths into a min-heap.",
            "Repeatedly extract the two smallest ropes, sum them, add to total cost, and reinsert sum.",
            "Huffman-style greedy pairing guarantees minimum total combination cost in O(N log N)."
        ]
    if "fractional knapsack" in n:
        return [
            "Calculate value-to-weight ratio (density) for each item.",
            "Sort items descending by density.",
            "Greedily take entire items as long as remaining knapsack capacity allows.",
            "Take fractional slice of next item to fill exact remaining capacity."
        ]
    if "huffman encoding" in n:
        return [
            "Build optimal prefix code tree by minimizing weighted path length.",
            "Insert all character frequencies into a min-heap of tree nodes.",
            "Repeatedly merge two lowest frequency trees into a combined node until one tree remains.",
            "Assign '0' to left branch and '1' to right branch; produces optimal variable-length codes."
        ]
    if "page faults in lru" in n:
        return [
            "Maintain cache memory of fixed capacity using a hash set and doubly-linked list (or deque).",
            "When page is requested: if in cache, hit (move to most recently used).",
            "If page not in cache, page fault: if cache full, evict least recently used page.",
            "Insert new page; count total page faults in O(N * C) or O(N) time."
        ]
    if "minimum number of coins" in n:
        return [
            "Standard canonical coin denominations (e.g. 1, 2, 5, 10, 20, 50, 100, 500, 2000).",
            "Greedily pick the largest denomination that is <= remaining amount.",
            "Subtract chosen coin from amount and repeat until amount reaches 0.",
            "Canonical currency system guarantees greedy choice yields minimal coin count."
        ]
    if "egyptian fraction" in n:
        return [
            "Represent fraction P / Q as sum of distinct unit fractions 1 / n.",
            "Find smallest unit fraction 1 / ceil(Q / P) that is <= P / Q.",
            "Subtract 1 / ceil(Q / P) from P / Q to obtain a new smaller remainder fraction.",
            "Greedy Fibonacci-Sylvester algorithm is guaranteed to terminate with distinct unit fractions."
        ]

    # Pattern-based default fallback (high signal 3-4 points)
    return [
        f"Identify the optimal substructure and greedy choice property for {name}.",
        f"Sort or organize candidates using {pattern} to prioritize highest-yield choices.",
        "Maintain running boundaries or heap state to validate local feasibility without backtracking.",
        f"Achieves optimal runtime in {p if p else 'linear or logarithmic'} time complexity."
    ]

updated_count = 0
for p in problems:
    name = p['name']
    lc = p.get('lcNum', '')
    pat = p.get('pattern', '')
    cat = p.get('category', '')

    if name in SPECIFIC_INTUITIONS:
        p['intuition'] = SPECIFIC_INTUITIONS[name]
    else:
        p['intuition'] = get_greedy_intuition(name, lc, pat, cat)
    updated_count += 1

with open(JSON_PATH, 'w', encoding='utf-8') as f:
    json.dump(problems, f, indent=2)

print(f"Updated {updated_count} problems with 3-4 point intuitions.")
