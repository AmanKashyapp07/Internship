/*
================================================================================
  FAANG & TIER-1 TECH INTERVIEW REVISION SHEET (C++17)
  Target: Meta, Google, Amazon, Apple, Netflix, Uber, Stripe, Microsoft, etc.
  Style: Clean, Interview-Realistic C++17 (Standard Library, No CP Macros)
  File: DSA/interview.cpp
================================================================================
  CONTENTS / INDEX:

  CORE DATA STRUCTURE PATTERNS
  01. Two Pointers
      - Pair Sum (Sorted)
      - Container With Most Water
      - Trapping Rain Water
      - 3Sum & 4Sum
      - Shortest Unsorted Continuous Subarray
  02. Sliding Window
      - Fixed Size: Maximum Sum Subarray of Size K
      - Fixed Size: Sliding Window Maximum (Monotonic Deque)
      - Variable Size: Longest Substring Without Repeating Characters
      - Variable Size: Subarrays with K Distinct Integers
  03. Fast & Slow Pointers (Floyd's Cycle Finding)
      - Linked List Cycle I & II (Detect & Find Cycle Start)
      - Middle of the Linked List
      - Happy Number
      - Find the Duplicate Number (Floyd's on Array)
  04. Prefix Sum & Difference Array
      - Subarray Sum Equals K (Hash Map)
      - Contiguous Array (Equal 0s and 1s)
      - Subarray Sums Divisible by K
      - Range Update Operations (Difference Array / Line Sweep)
  05. Monotonic Stack & Queue
      - Next Greater Element I & II (Circular)
      - Daily Temperatures
      - Largest Rectangle in Histogram
      - Maximal Rectangle (2D Grid)
  06. Kadane's Algorithm & Extensions
      - Maximum Subarray Sum
      - Maximum Product Subarray
      - Maximum Sum Circular Subarray

  LINKED LISTS
  07. List Reversal Variants
      - Iterative & Recursive Standard Reversal
      - Reverse Linked List II (Between Left and Right)
      - Reverse Nodes in k-Group
  08. Sorting, Merging & Splitting
      - Merge Two Sorted Lists
      - Merge K Sorted Lists (Min-Heap / Divide & Conquer)
      - Sort List (Merge Sort on Linked List)
      - Reorder List
  09. Pointer Manipulations
      - Remove Nth Node From End of List
      - Intersection of Two Linked Lists
      - Palindrome Linked List
  10. Advanced Cache & O(1) Design
      - LRU Cache (List + Hash Map)
      - LFU Cache (Frequency Map + Lists)
      - All O(1) Data Structure

  TREES & TRIE
  11. Tree DFS Traversals
      - Preorder, Inorder, Postorder (Recursive & Iterative)
      - Morris Inorder Traversal (O(1) Auxiliary Space)
  12. BFS & Level Order Patterns
      - Standard Level Order & Zigzag Traversal
      - Binary Tree Right / Left Side View
      - Vertical Order Traversal & Top/Bottom View
  13. Tree Metrics & Ancestors
      - Diameter of Binary Tree
      - Maximum Path Sum (Any Node to Any Node)
      - Lowest Common Ancestor (BT, BST, & with Parent Pointer)
  14. Tree Validation & Reconstruction
      - Validate Binary Search Tree
      - Serialize and Deserialize Binary Tree (N-ary & BT)
      - Construct Binary Tree from Preorder and Inorder Traversal
  15. Trie (Prefix Tree) Variants
      - Standard Trie (Insert, Search, StartsWith)
      - Design Add and Search Words Data Structure (Wildcard Search)
      - Word Search II (Trie + Grid Backtracking)

  BACKTRACKING & COMBINATORICS
  16. Subsets & Combinations
      - Subsets I & II (Handling Duplicates)
      - Combinations & Combination Sum I, II, III
  17. Permutations
      - Permutations I & II (With Duplicates)
      - Next Permutation
  18. Grid & Constraint Backtracking
      - N-Queens
      - Sudoku Solver
      - Word Search (Grid)
  19. String Partitioning
      - Palindrome Partitioning
      - Restore IP Addresses

  GRAPHS & ADVANCED TOPOLOGIES
  20. Grid Traversal (BFS / DFS)
      - Number of Islands
      - Rotting Oranges (Multi-source BFS)
      - Shortest Path in Binary Matrix
      - Pacific Atlantic Water Flow
  21. Topological Sort
      - Course Schedule I & II (Kahn's Indegree BFS & DFS Cycle Detection)
      - Alien Dictionary
  22. Disjoint Set Union (DSU)
      - Union-Find with Path Compression & Rank
      - Number of Provinces
      - Redundant Connection
      - Accounts Merge
  23. Shortest Path & Specialized Graph Algorithms
      - Dijkstra's Algorithm (Network Delay Time, Swim in Rising Water)
      - 0-1 BFS / Bellman-Ford Overview
      - Minimum Spanning Tree (Kruskal's & Prim's)
  24. Graph Reconstruction & Deep Copies
      - Clone Graph (DFS/BFS with Hash Map)
      - Reconstruct Itinerary (Eulerian Path / Hierholzer's Algorithm)

  DYNAMIC PROGRAMMING
  25. 1D & Unbounded DP
      - Climbing Stairs & Min Cost Climbing Stairs
      - House Robber I & II (Circular Array)
      - Coin Change I (Min Coins) & Coin Change II (Total Ways)
      - Word Break I & II
  26. 2D Grid DP
      - Unique Paths I & II (With Obstacles)
      - Minimum Path Sum
      - Dungeon Game / Cherry Pickup
  27. Strings, Sequences & Matching
      - Longest Common Subsequence (LCS)
      - Edit Distance
      - Interleaving String
      - Wildcard & Regular Expression Matching
  28. Intervals & Subsequences
      - Longest Increasing Subsequence (O(N log N) Patience Sorting)
      - Partition Equal Subset Sum & Target Sum (Knapsack Variants)
  29. Advanced DP Paradigms
      - Interval DP: Matrix Chain Multiplication / Burst Balloons
      - State Machine DP: Best Time to Buy/Sell Stock (I, II, III, IV, Cooldown, Fee)
      - Tree DP: House Robber III / Binary Tree Maximum Path Sum

  HEAPS, PRIORITY QUEUES & INTERVALS
  30. Top-K Patterns
      - Kth Largest Element in an Array (QuickSelect vs Min-Heap)
      - Top K Frequent Elements / Words
      - Reorganize String / Task Scheduler
  31. Multi-Heap Patterns
      - Find Median from Data Stream (Two Heaps)
      - Sliding Window Median
  32. Interval Scheduling Patterns
      - Merge Intervals
      - Insert Interval
      - Non-overlapping Intervals
      - Meeting Rooms I & II (Sweep Line / Min-Heap)

  BINARY SEARCH & ADVANCED RANGE QUERIES
  33. Rotated & Boundary Search
      - Search in Rotated Sorted Array I & II (Duplicates)
      - Find Minimum in Rotated Sorted Array
      - Find First and Last Position of Element in Sorted Array
  34. Binary Search on Answer
      - Koko Eating Bananas
      - Capacity To Ship Packages Within D Days
      - Split Array Largest Sum
      - Median of Two Sorted Arrays
  35. Range Query Data Structures
      - Segment Tree (Point Update, Range Query - Mutable)
      - Fenwick Tree / Binary Indexed Tree (Count of Smaller Numbers After Self)

  SYSTEM & DATA STRUCTURE DESIGN
  36. Custom Stack & Queue Variants
      - Min Stack / Max Stack (O(1) Space/Time Aux Tracker)
      - Implement Queue using Stacks
  37. Custom Collections & In-Memory Systems
      - Design HashMap / HashSet (Separate Chaining)
      - Insert Delete GetRandom O(1) (With & Without Duplicates)
      - Design Twitter / Newsfeed System
  38. Stream & File System Design
      - Read N Characters Given Read4 I & II (Call Multiple Times)
      - Design In-Memory File System
      - Design Search Autocomplete System

  INTERVIEW CHEAT SHEETS (FAANG PLAYBOOK)
  - 45-Minute Live Coding Interview Flow
  - Pattern Recognition Quick Cheat Table
  - Common Follow-up Questions & Scalability Considerations
================================================================================
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <deque>
#include <list>
#include <cmath>
#include <climits>
#include <functional>
#include <sstream>
#include <memory>
#include <numeric>
#include <random>

using namespace std;

// ============================================================================
// COMMON CORE DATA STRUCTURES
// ============================================================================

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* n) : val(x), next(n) {}
};

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* l, TreeNode* r) : val(x), left(l), right(r) {}
};

struct NodeWithRandom {
    int val;
    NodeWithRandom* next;
    NodeWithRandom* random;
    NodeWithRandom(int _val) : val(_val), next(nullptr), random(nullptr) {}
};

struct GraphNode {
    int val;
    vector<GraphNode*> neighbors;
    GraphNode(int _val) : val(_val) {}
};

// ============================================================================
// 01. TWO POINTERS
// Solves: LC 167 (Two Sum II), LC 11 (Container Most Water), LC 42 (Trapping Rain Water),
//         LC 15 (3Sum), LC 18 (4Sum), LC 581 (Shortest Unsorted Subarray)
// Companies: Meta, Amazon, Apple, Microsoft, Google
// Complexity: Time: O(N) or O(N^2) | Space: O(1)
// ============================================================================

// 1. Pair Sum (Sorted) - Time: O(N) | Space: O(1)
pair<int, int> twoSumSorted(const vector<int>& numbers, int target) {
    int left = 0, right = static_cast<int>(numbers.size()) - 1;
    while (left < right) {
        int sum = numbers[left] + numbers[right];
        if (sum == target) return {left + 1, right + 1}; // 1-indexed
        if (sum < target) left++;
        else right--;
    }
    return {-1, -1};
}

// 2. Container With Most Water - Time: O(N) | Space: O(1)
int maxAreaWater(const vector<int>& height) {
    int left = 0, right = static_cast<int>(height.size()) - 1;
    int maxWater = 0;
    while (left < right) {
        int width = right - left;
        int h = min(height[left], height[right]);
        maxWater = max(maxWater, width * h);
        if (height[left] < height[right]) left++;
        else right--;
    }
    return maxWater;
}

// 3. Trapping Rain Water (Two Pointers) - Time: O(N) | Space: O(1)
int trapRainWater(const vector<int>& height) {
    if (height.empty()) return 0;
    int left = 0, right = static_cast<int>(height.size()) - 1;
    int leftMax = 0, rightMax = 0, totalWater = 0;

    while (left < right) {
        if (height[left] <= height[right]) {
            if (height[left] >= leftMax) leftMax = height[left];
            else totalWater += leftMax - height[left];
            left++;
        } else {
            if (height[right] >= rightMax) rightMax = height[right];
            else totalWater += rightMax - height[right];
            right--;
        }
    }
    return totalWater;
}

// 4. 3Sum (Unique Triplets Summing to 0) - Time: O(N^2) | Space: O(1) auxiliary
vector<vector<int>> threeSum(vector<int>& nums) {
    sort(nums.begin(), nums.end());
    vector<vector<int>> res;
    int n = nums.size();

    for (int i = 0; i < n - 2; ++i) {
        if (i > 0 && nums[i] == nums[i - 1]) continue; // Skip duplicate anchor
        if (nums[i] > 0) break; // Optimization

        int left = i + 1, right = n - 1;
        while (left < right) {
            int sum = nums[i] + nums[left] + nums[right];
            if (sum == 0) {
                res.push_back({nums[i], nums[left], nums[right]});
                while (left < right && nums[left] == nums[left + 1]) left++;
                while (left < right && nums[right] == nums[right - 1]) right--;
                left++; right--;
            } else if (sum < 0) {
                left++;
            } else {
                right--;
            }
        }
    }
    return res;
}

// 5. Shortest Unsorted Continuous Subarray - Time: O(N) | Space: O(1)
int findUnsortedSubarray(const vector<int>& nums) {
    int n = nums.size();
    int rightBound = -1, leftBound = -1;
    int maxSeen = INT_MIN, minSeen = INT_MAX;

    for (int i = 0; i < n; ++i) {
        maxSeen = max(maxSeen, nums[i]);
        if (nums[i] < maxSeen) rightBound = i;
    }
    for (int i = n - 1; i >= 0; --i) {
        minSeen = min(minSeen, nums[i]);
        if (nums[i] > minSeen) leftBound = i;
    }
    return (rightBound == -1) ? 0 : rightBound - leftBound + 1;
}

// ============================================================================
// 02. SLIDING WINDOW
// Solves: LC 643, LC 239 (Sliding Window Max), LC 3 (Longest Substr No Repeat),
//         LC 992 (Subarrays with K Distinct Integers)
// Companies: Google, Amazon, Meta, Microsoft, Uber
// Complexity: Time: O(N) | Space: O(K)
// ============================================================================

// 1. Sliding Window Maximum (Monotonic Deque) - Time: O(N) | Space: O(K)
vector<int> maxSlidingWindow(const vector<int>& nums, int k) {
    deque<int> dq; // Stores indices of decreasing values
    vector<int> res;

    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        if (!dq.empty() && dq.front() == i - k) dq.pop_front();
        while (!dq.empty() && nums[dq.back()] <= nums[i]) dq.pop_back();
        dq.push_back(i);
        if (i >= k - 1) res.push_back(nums[dq.front()]);
    }
    return res;
}

// 2. Longest Substring Without Repeating Characters - Time: O(N) | Space: O(min(N, Alphabet))
int lengthOfLongestSubstring(const string& s) {
    vector<int> lastIdx(128, -1);
    int left = 0, maxLen = 0;

    for (int right = 0; right < static_cast<int>(s.size()); ++right) {
        char c = s[right];
        if (lastIdx[c] >= left) left = lastIdx[c] + 1;
        lastIdx[c] = right;
        maxLen = max(maxLen, right - left + 1);
    }
    return maxLen;
}

// 3. Subarrays with At Most K Distinct Integers (Helper for Exactly K) - Time: O(N) | Space: O(K)
int atMostKDistinct(const vector<int>& nums, int k) {
    unordered_map<int, int> count;
    int left = 0, total = 0;

    for (int right = 0; right < static_cast<int>(nums.size()); ++right) {
        if (count[nums[right]]++ == 0) k--;
        while (k < 0) {
            if (--count[nums[left]] == 0) k++;
            left++;
        }
        total += right - left + 1;
    }
    return total;
}

// Exactly K Distinct = AtMost(K) - AtMost(K - 1)
int subarraysWithKDistinct(const vector<int>& nums, int k) {
    return atMostKDistinct(nums, k) - atMostKDistinct(nums, k - 1);
}

// ============================================================================
// 03. FAST & SLOW POINTERS (FLOYD'S CYCLE FINDING)
// Solves: LC 141 (Cycle Detect), LC 142 (Cycle Start), LC 876 (Middle),
//         LC 202 (Happy Number), LC 287 (Find the Duplicate Number)
// Companies: Microsoft, Amazon, Meta, Apple
// Complexity: Time: O(N) | Space: O(1)
// ============================================================================

// 1. Detect & Find Cycle Start
ListNode* detectCycleStart(ListNode* head) {
    ListNode *slow = head, *fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            ListNode* entry = head;
            while (entry != slow) {
                entry = entry->next;
                slow = slow->next;
            }
            return entry;
        }
    }
    return nullptr;
}

// 2. Find Duplicate Number in Array of [1..N] - Time: O(N) | Space: O(1)
int findDuplicate(const vector<int>& nums) {
    int slow = nums[0], fast = nums[0];
    do {
        slow = nums[slow];
        fast = nums[nums[fast]];
    } while (slow != fast);

    slow = nums[0];
    while (slow != fast) {
        slow = nums[slow];
        fast = nums[fast];
    }
    return slow;
}

// ============================================================================
// 04. PREFIX SUM & DIFFERENCE ARRAY
// Solves: LC 560 (Subarray Sum K), LC 525 (Contiguous Array 0s & 1s),
//         LC 974 (Subarray Sums Divisible by K), LC 370 (Range Addition)
// Companies: Meta, Google, Amazon, Bloomberg
// Complexity: Time: O(N) | Space: O(N)
// ============================================================================

// 1. Subarray Sum Equals K (Negative Numbers Supported) - Time: O(N) | Space: O(N)
int subarraySumEqualsK(const vector<int>& nums, int k) {
    unordered_map<int, int> prefixFreq;
    prefixFreq[0] = 1;
    int currSum = 0, count = 0;

    for (int x : nums) {
        currSum += x;
        if (prefixFreq.count(currSum - k)) count += prefixFreq[currSum - k];
        prefixFreq[currSum]++;
    }
    return count;
}

// 2. Subarray Sums Divisible by K - Time: O(N) | Space: O(K)
int subarraysDivByK(const vector<int>& nums, int k) {
    unordered_map<int, int> modCount;
    modCount[0] = 1;
    int currSum = 0, total = 0;

    for (int x : nums) {
        currSum += x;
        int rem = ((currSum % k) + k) % k; // Normalized modulo for negative numbers
        if (modCount.count(rem)) total += modCount[rem];
        modCount[rem]++;
    }
    return total;
}

// 3. Contiguous Array (Equal 0s and 1s) - Time: O(N) | Space: O(N)
int findMaxLength(const vector<int>& nums) {
    unordered_map<int, int> firstSeen;
    firstSeen[0] = -1;
    int balance = 0, maxLen = 0;

    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        balance += (nums[i] == 1 ? 1 : -1);
        if (firstSeen.count(balance)) {
            maxLen = max(maxLen, i - firstSeen[balance]);
        } else {
            firstSeen[balance] = i;
        }
    }
    return maxLen;
}

// ============================================================================
// 05. MONOTONIC STACK & QUEUE
// Solves: LC 496, LC 503 (Circular NGE), LC 739 (Daily Temperatures),
//         LC 84 (Largest Rectangle in Histogram), LC 85 (Maximal Rectangle 2D)
// Companies: Amazon, Meta, Google, Microsoft, Bloomberg
// Complexity: Time: O(N) or O(R * C) | Space: O(N)
// ============================================================================

// 1. Next Greater Element II (Circular Array) - Time: O(N) | Space: O(N)
vector<int> nextGreaterElementsCircular(const vector<int>& nums) {
    int n = nums.size();
    vector<int> res(n, -1);
    stack<int> st;

    for (int i = 0; i < 2 * n; ++i) {
        while (!st.empty() && nums[st.top()] < nums[i % n]) {
            res[st.top()] = nums[i % n];
            st.pop();
        }
        if (i < n) st.push(i);
    }
    return res;
}

// 2. Largest Rectangle in Histogram - Time: O(N) | Space: O(N)
int largestRectangleArea(vector<int>& heights) {
    heights.push_back(0); // Dummy sentinel
    stack<int> st;
    int maxArea = 0;

    for (int i = 0; i < static_cast<int>(heights.size()); ++i) {
        while (!st.empty() && heights[st.top()] >= heights[i]) {
            int h = heights[st.top()];
            st.pop();
            int w = st.empty() ? i : i - st.top() - 1;
            maxArea = max(maxArea, h * w);
        }
        st.push(i);
    }
    heights.pop_back();
    return maxArea;
}

// 3. Maximal Rectangle in 2D Binary Matrix - Time: O(R * C) | Space: O(C)
int maximalRectangle(const vector<vector<char>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) return 0;
    int cols = matrix[0].size();
    vector<int> heights(cols, 0);
    int maxRect = 0;

    for (const auto& row : matrix) {
        for (int c = 0; c < cols; ++c) {
            heights[c] = (row[c] == '1') ? heights[c] + 1 : 0;
        }
        maxRect = max(maxRect, largestRectangleArea(heights));
    }
    return maxRect;
}

// ============================================================================
// 06. KADANE'S ALGORITHM & EXTENSIONS
// Solves: LC 53 (Max Subarray), LC 152 (Max Product Subarray), LC 918 (Circular Max Subarray)
// Companies: Microsoft, Amazon, Apple, LinkedIn, Meta
// Complexity: Time: O(N) | Space: O(1)
// ============================================================

// Maximum Sum Circular Subarray - Time: O(N) | Space: O(1)
int maxSubarraySumCircular(const vector<int>& nums) {
    int total = 0;
    int maxSoFar = nums[0], maxCurr = 0;
    int minSoFar = nums[0], minCurr = 0;

    for (int x : nums) {
        maxCurr = max(x, maxCurr + x);
        maxSoFar = max(maxSoFar, maxCurr);
        minCurr = min(x, minCurr + x);
        minSoFar = min(minSoFar, minCurr);
        total += x;
    }
    // If all numbers are negative, total - minSoFar will be 0, return maxSoFar
    return (maxSoFar > 0) ? max(maxSoFar, total - minSoFar) : maxSoFar;
}

// ============================================================================
// 07. LINKED LIST REVERSAL VARIANTS
// Solves: LC 206, LC 92 (Reverse Between Left and Right), LC 25 (Reverse K-Group)
// Companies: Meta, Amazon, Microsoft, Apple, Google
// Complexity: Time: O(N) | Space: O(1)
// ============================================================================

// Reverse Linked List II (Between Left and Right) - Time: O(N) | Space: O(1)
ListNode* reverseBetween(ListNode* head, int left, int right) {
    if (!head || left == right) return head;
    ListNode dummy(0, head);
    ListNode* prev = &dummy;

    for (int i = 1; i < left; ++i) prev = prev->next;
    ListNode* curr = prev->next;

    for (int i = 0; i < right - left; ++i) {
        ListNode* nextNode = curr->next;
        curr->next = nextNode->next;
        nextNode->next = prev->next;
        prev->next = nextNode;
    }
    return dummy.next;
}

// ============================================================================
// 08. SORTING, MERGING & SPLITTING LINKED LISTS
// Solves: LC 21, LC 23 (Merge K Lists), LC 148 (Sort List), LC 143 (Reorder List)
// Companies: Amazon, Meta, Microsoft, Apple, Uber
// Complexity: Sort List: O(N log N) time, O(1) or O(log N) space
// ============================================================================

// 1. Sort List using Merge Sort - Time: O(N log N) | Space: O(log N)
ListNode* mergeTwoSortedLists(ListNode* l1, ListNode* l2) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    while (l1 && l2) {
        if (l1->val <= l2->val) { tail->next = l1; l1 = l1->next; }
        else { tail->next = l2; l2 = l2->next; }
        tail = tail->next;
    }
    tail->next = l1 ? l1 : l2;
    return dummy.next;
}

ListNode* sortList(ListNode* head) {
    if (!head || !head->next) return head;
    ListNode *slow = head, *fast = head, *prev = nullptr;

    while (fast && fast->next) {
        prev = slow;
        slow = slow->next;
        fast = fast->next->next;
    }
    prev->next = nullptr; // Cut list in half

    return mergeTwoSortedLists(sortList(head), sortList(slow));
}

// 2. Reorder List (L0 -> Ln -> L1 -> Ln-1 ...) - Time: O(N) | Space: O(1)
void reorderList(ListNode* head) {
    if (!head || !head->next) return;

    // Find middle
    ListNode *slow = head, *fast = head;
    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // Reverse second half
    ListNode* prev = nullptr;
    ListNode* curr = slow->next;
    slow->next = nullptr;
    while (curr) {
        ListNode* nxt = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nxt;
    }

    // Interleave two halves
    ListNode *first = head, *second = prev;
    while (second) {
        ListNode *t1 = first->next, *t2 = second->next;
        first->next = second;
        second->next = t1;
        first = t1;
        second = t2;
    }
}

// ============================================================================
// 09. POINTER MANIPULATIONS
// Solves: LC 19 (Remove Nth From End), LC 160 (Intersection), LC 234 (Palindrome)
// Companies: Microsoft, Amazon, Meta, Bloomberg
// Complexity: Time: O(N) | Space: O(1)
// ============================================================================

// 1. Remove Nth Node From End of List - Time: O(N) | Space: O(1)
ListNode* removeNthFromEnd(ListNode* head, int n) {
    ListNode dummy(0, head);
    ListNode *fast = &dummy, *slow = &dummy;

    for (int i = 0; i <= n; ++i) fast = fast->next;
    while (fast) {
        slow = slow->next;
        fast = fast->next;
    }
    ListNode* toDelete = slow->next;
    slow->next = slow->next->next;
    delete toDelete;
    return dummy.next;
}

// 2. Intersection of Two Linked Lists - Time: O(N + M) | Space: O(1)
ListNode* getIntersectionNode(ListNode* headA, ListNode* headB) {
    ListNode *ptrA = headA, *ptrB = headB;
    while (ptrA != ptrB) {
        ptrA = ptrA ? ptrA->next : headB;
        ptrB = ptrB ? ptrB->next : headA;
    }
    return ptrA;
}

// ============================================================================
// 10. ADVANCED CACHE & O(1) DESIGN
// Solves: LC 146 (LRU), LC 460 (LFU), LC 432 (All O(1) Data Structure)
// Companies: Amazon, Microsoft, Meta, Google, Apple, Bloomberg, Stripe
// Complexity: All operations O(1) time | O(Capacity) space
// ============================================================================

// All O(1) Data Structure (Inc, Dec, GetMax, GetMin in O(1))
class AllOne {
    struct Bucket {
        int count;
        unordered_set<string> keys;
    };
    list<Bucket> buckets; // Doubly linked list of sorted count buckets
    unordered_map<string, list<Bucket>::iterator> keyMap;

public:
    AllOne() {}

    void inc(string key) {
        if (!keyMap.count(key)) {
            if (buckets.empty() || buckets.front().count != 1) {
                buckets.push_front({1, {key}});
            } else {
                buckets.front().keys.insert(key);
            }
            keyMap[key] = buckets.begin();
        } else {
            auto cur = keyMap[key];
            auto nxt = next(cur);
            if (nxt == buckets.end() || nxt->count != cur->count + 1) {
                nxt = buckets.insert(nxt, {cur->count + 1, {}});
            }
            nxt->keys.insert(key);
            keyMap[key] = nxt;
            cur->keys.erase(key);
            if (cur->keys.empty()) buckets.erase(cur);
        }
    }

    void dec(string key) {
        auto cur = keyMap[key];
        if (cur->count == 1) {
            keyMap.erase(key);
        } else {
            auto prv = prev(cur);
            if (cur == buckets.begin() || prv->count != cur->count - 1) {
                prv = buckets.insert(cur, {cur->count - 1, {}});
            }
            prv->keys.insert(key);
            keyMap[key] = prv;
        }
        cur->keys.erase(key);
        if (cur->keys.empty()) buckets.erase(cur);
    }

    string getMaxKey() {
        return buckets.empty() ? "" : *buckets.back().keys.begin();
    }

    string getMinKey() {
        return buckets.empty() ? "" : *buckets.front().keys.begin();
    }
};

// ============================================================================
// 11. TREE DFS TRAVERSALS & MORRIS TRAVERSAL
// Solves: LC 94, 144, 145, Morris Inorder (O(1) Auxiliary Space)
// Companies: Google, Microsoft, Amazon, Meta
// Complexity: Time: O(N) | Space: Morris O(1), Iterative O(H)
// ============================================================================

// Morris Inorder Traversal - Time: O(N) | Space: O(1) auxiliary
vector<int> morrisInorder(TreeNode* root) {
    vector<int> res;
    TreeNode* curr = root;

    while (curr) {
        if (!curr->left) {
            res.push_back(curr->val);
            curr = curr->right;
        } else {
            TreeNode* pred = curr->left;
            while (pred->right && pred->right != curr) pred = pred->right;

            if (!pred->right) {
                pred->right = curr; // Establish thread
                curr = curr->left;
            } else {
                pred->right = nullptr; // Break thread
                res.push_back(curr->val);
                curr = curr->right;
            }
        }
    }
    return res;
}

// ============================================================================
// 12. BFS & LEVEL ORDER PATTERNS
// Solves: LC 102 (Level Order), LC 103 (Zigzag), LC 199 (Right View), LC 314 (Vertical Order)
// Companies: Meta, Amazon, Microsoft, Bloomberg
// Complexity: Time: O(N) | Space: O(W)
// ============================================================================

// Vertical Order Traversal of Binary Tree - Time: O(N log N) | Space: O(N)
vector<vector<int>> verticalOrder(TreeNode* root) {
    if (!root) return {};
    map<int, vector<int>> colMap; // Column index -> Node values
    queue<pair<TreeNode*, int>> q;
    q.push({root, 0});

    while (!q.empty()) {
        auto [node, col] = q.front();
        q.pop();
        colMap[col].push_back(node->val);

        if (node->left) q.push({node->left, col - 1});
        if (node->right) q.push({node->right, col + 1});
    }

    vector<vector<int>> res;
    for (auto& [col, values] : colMap) res.push_back(values);
    return res;
}

// ============================================================================
// 13. TREE METRICS & ANCESTORS
// Solves: LC 543 (Diameter), LC 124 (Max Path Sum), LC 236 / 235 (LCA)
// Companies: Meta, Amazon, Microsoft, Google
// Complexity: Time: O(N) | Space: O(H)
// ============================================================================

// LCA with Parent Pointer (Intersection of paths) - Time: O(H) | Space: O(1)
struct NodeWithParent {
    int val;
    NodeWithParent *left, *right, *parent;
};

NodeWithParent* lowestCommonAncestorWithParent(NodeWithParent* p, NodeWithParent* q) {
    NodeWithParent *a = p, *b = q;
    while (a != b) {
        a = a->parent ? a->parent : q;
        b = b->parent ? b->parent : p;
    }
    return a;
}

// ============================================================================
// 14. TREE VALIDATION & RECONSTRUCTION
// Solves: LC 98 (Validate BST), LC 297 (Serialize/Deserialize), LC 105 (Pre+In Tree)
// Companies: Meta, Amazon, Microsoft, Google, Uber
// Complexity: Time: O(N) | Space: O(N)
// ============================================================================

// Serialize & Deserialize Binary Tree (Concise BFS Preorder)
class CodecTree {
public:
    string serialize(TreeNode* root) {
        if (!root) return "#";
        return to_string(root->val) + "," + serialize(root->left) + "," + serialize(root->right);
    }

    TreeNode* deserializeHelper(stringstream& ss) {
        string val;
        if (!getline(ss, val, ',') || val == "#") return nullptr;
        TreeNode* node = new TreeNode(stoi(val));
        node->left = deserializeHelper(ss);
        node->right = deserializeHelper(ss);
        return node;
    }

    TreeNode* deserialize(string data) {
        stringstream ss(data);
        return deserializeHelper(ss);
    }
};

// ============================================================================
// 15. TRIE (PREFIX TREE) VARIANTS
// Solves: LC 208 (Trie), LC 211 (Add & Search Word with '.'), LC 212 (Word Search II)
// Companies: Amazon, Microsoft, Google, Meta, Twitter
// Complexity: Add/Search: O(L) | Word Search II: O(R * C * 4^L)
// ============================================================================

class WordDictionary {
    struct TrieNode {
        TrieNode* children[26] = {};
        bool isEnd = false;
    };
    TrieNode* root;

    bool searchInNode(const string& word, int idx, TrieNode* node) {
        if (!node) return false;
        if (idx == static_cast<int>(word.size())) return node->isEnd;

        char c = word[idx];
        if (c == '.') {
            for (int i = 0; i < 26; ++i) {
                if (node->children[i] && searchInNode(word, idx + 1, node->children[i])) return true;
            }
            return false;
        }
        return searchInNode(word, idx + 1, node->children[c - 'a']);
    }

public:
    WordDictionary() { root = new TrieNode(); }

    void addWord(string word) {
        TrieNode* curr = root;
        for (char c : word) {
            int i = c - 'a';
            if (!curr->children[i]) curr->children[i] = new TrieNode();
            curr = curr->children[i];
        }
        curr->isEnd = true;
    }

    bool search(string word) {
        return searchInNode(word, 0, root);
    }
};

// ============================================================================
// 16. SUBSETS & COMBINATIONS (WITH DUPLICATES)
// Solves: LC 78 (Subsets), LC 90 (Subsets II), LC 39, 40 (Comb Sum II), LC 216 (Comb Sum III)
// Companies: Amazon, Meta, Microsoft, Google
// Complexity: Time: O(2^N) | Space: O(N)
// ============================================================================

// Subsets II (Input contains duplicates) - Time: O(2^N) | Space: O(N)
void backtrackSubsetsWithDup(int start, vector<int>& nums, vector<int>& curr, vector<vector<int>>& res) {
    res.push_back(curr);
    for (int i = start; i < static_cast<int>(nums.size()); ++i) {
        if (i > start && nums[i] == nums[i - 1]) continue; // Skip duplicates at same depth
        curr.push_back(nums[i]);
        backtrackSubsetsWithDup(i + 1, nums, curr, res);
        curr.pop_back();
    }
}

vector<vector<int>> subsetsWithDup(vector<int>& nums) {
    sort(nums.begin(), nums.end());
    vector<vector<int>> res;
    vector<int> curr;
    backtrackSubsetsWithDup(0, nums, curr, res);
    return res;
}

// ============================================================================
// 17. PERMUTATIONS & NEXT PERMUTATION
// Solves: LC 46, LC 47 (Permutations II with Dups), LC 31 (Next Permutation)
// Companies: Google, Amazon, Meta, Microsoft
// Complexity: Time: O(N!) or O(N) for nextPermutation | Space: O(N)
// ============================================================================

// Next Permutation in Place - Time: O(N) | Space: O(1)
void nextPermutation(vector<int>& nums) {
    int n = nums.size(), i = n - 2;
    while (i >= 0 && nums[i] >= nums[i + 1]) i--;

    if (i >= 0) {
        int j = n - 1;
        while (nums[j] <= nums[i]) j--;
        swap(nums[i], nums[j]);
    }
    reverse(nums.begin() + i + 1, nums.end());
}

// ============================================================================
// 18. GRID & CONSTRAINT BACKTRACKING
// Solves: LC 51 (N-Queens), LC 37 (Sudoku Solver), LC 79 (Word Search)
// Companies: Google, Amazon, Microsoft, Uber
// Complexity: Sudoku: O(9^(Empty_Cells)) | N-Queens: O(N!)
// ============================================================================

bool isSudokuValid(const vector<vector<char>>& board, int r, int c, char ch) {
    for (int i = 0; i < 9; ++i) {
        if (board[r][i] == ch || board[i][c] == ch) return false;
        if (board[3 * (r / 3) + i / 3][3 * (c / 3) + i % 3] == ch) return false;
    }
    return true;
}

bool solveSudoku(vector<vector<char>>& board) {
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (board[r][c] == '.') {
                for (char ch = '1'; ch <= '9'; ++ch) {
                    if (isSudokuValid(board, r, c, ch)) {
                        board[r][c] = ch;
                        if (solveSudoku(board)) return true;
                        board[r][c] = '.'; // Backtrack
                    }
                }
                return false;
            }
        }
    }
    return true;
}

// ============================================================================
// 19. STRING PARTITIONING
// Solves: LC 131 (Palindrome Partitioning), LC 93 (Restore IP Addresses)
// Companies: Amazon, Google, Meta, Microsoft
// Complexity: Restore IP: O(1) bound | Palindrome: O(N * 2^N)
// ============================================================================

// Restore IP Addresses - Time: O(1) bound | Space: O(1)
void backtrackIP(const string& s, int idx, int segment, string curr, vector<string>& res) {
    if (segment == 4 && idx == static_cast<int>(s.size())) {
        curr.pop_back(); // Remove trailing dot
        res.push_back(curr);
        return;
    }
    if (segment == 4 || idx == static_cast<int>(s.size())) return;

    for (int len = 1; len <= 3 && idx + len <= static_cast<int>(s.size()); ++len) {
        string part = s.substr(idx, len);
        if ((part[0] == '0' && len > 1) || stoi(part) > 255) continue;
        backtrackIP(s, idx + len, segment + 1, curr + part + ".", res);
    }
}

vector<string> restoreIpAddresses(string s) {
    vector<string> res;
    backtrackIP(s, 0, 0, "", res);
    return res;
}

// ============================================================================
// 20. GRID TRAVERSAL (BFS / DFS)
// Solves: LC 200 (Islands), LC 994 (Rotting Oranges), LC 1091 (Binary Matrix Shortest Path),
//         LC 417 (Pacific Atlantic Water Flow)
// Companies: Amazon, Meta, Google, Microsoft, Bloomberg, Apple
// Complexity: Time: O(R * C) | Space: O(R * C)
// ============================================================================

// Pacific Atlantic Water Flow - Time: O(R * C) | Space: O(R * C)
void dfsWater(int r, int c, const vector<vector<int>>& heights, vector<vector<bool>>& ocean) {
    ocean[r][c] = true;
    int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1};
    int R = heights.size(), C = heights[0].size();

    for (int d = 0; d < 4; ++d) {
        int nr = r + dr[d], nc = c + dc[d];
        if (nr >= 0 && nr < R && nc >= 0 && nc < C && !ocean[nr][nc] && heights[nr][nc] >= heights[r][c]) {
            dfsWater(nr, nc, heights, ocean);
        }
    }
}

vector<vector<int>> pacificAtlantic(const vector<vector<int>>& heights) {
    if (heights.empty() || heights[0].empty()) return {};
    int R = heights.size(), C = heights[0].size();
    vector<vector<bool>> pac(R, vector<bool>(C, false)), atl(R, vector<bool>(C, false));

    for (int r = 0; r < R; ++r) { dfsWater(r, 0, heights, pac); dfsWater(r, C - 1, heights, atl); }
    for (int c = 0; c < C; ++c) { dfsWater(0, c, heights, pac); dfsWater(R - 1, c, heights, atl); }

    vector<vector<int>> res;
    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c) {
            if (pac[r][c] && atl[r][c]) res.push_back({r, c});
        }
    }
    return res;
}

// ============================================================================
// 21. TOPOLOGICAL SORT (COURSE SCHEDULE & ALIEN DICTIONARY)
// Solves: LC 207, LC 210, LC 269 (Alien Dictionary)
// Companies: Amazon, Meta, Microsoft, Google, Uber
// Complexity: Time: O(V + E) | Space: O(V + E)
// ============================================================================

string alienOrder(vector<string>& words) {
    unordered_map<char, unordered_set<char>> adj;
    unordered_map<char, int> inDegree;
    for (const string& w : words) for (char c : w) inDegree[c] = 0;

    for (size_t i = 0; i < words.size() - 1; ++i) {
        string w1 = words[i], w2 = words[i + 1];
        if (w1.size() > w2.size() && w1.rfind(w2, 0) == 0) return ""; // Invalid prefix rule

        for (size_t j = 0; j < min(w1.size(), w2.size()); ++j) {
            if (w1[j] != w2[j]) {
                if (!adj[w1[j]].count(w2[j])) {
                    adj[w1[j]].insert(w2[j]);
                    inDegree[w2[j]]++;
                }
                break;
            }
        }
    }

    queue<char> q;
    for (auto& [c, deg] : inDegree) if (deg == 0) q.push(c);

    string order = "";
    while (!q.empty()) {
        char curr = q.front();
        q.pop();
        order += curr;

        for (char nxt : adj[curr]) {
            if (--inDegree[nxt] == 0) q.push(nxt);
        }
    }
    return (order.size() == inDegree.size()) ? order : "";
}

// ============================================================================
// 22. DISJOINT SET UNION (DSU / ACCOUNTS MERGE)
// Solves: LC 547 (Provinces), LC 684 (Redundant Connection), LC 721 (Accounts Merge)
// Companies: Meta, Amazon, Microsoft, Google, Apple
// Complexity: Time: O(N * alpha(N)) | Space: O(N)
// ============================================================================

vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
    unordered_map<string, string> parent;
    unordered_map<string, string> emailToName;

    function<string(string)> find = [&](string s) {
        if (parent[s] == s) return s;
        return parent[s] = find(parent[s]);
    };

    for (const auto& acc : accounts) {
        string name = acc[0];
        for (size_t i = 1; i < acc.size(); ++i) {
            emailToName[acc[i]] = name;
            if (!parent.count(acc[i])) parent[acc[i]] = acc[i];
            string root1 = find(acc[1]);
            string root2 = find(acc[i]);
            parent[root2] = root1;
        }
    }

    unordered_map<string, vector<string>> groups;
    for (auto& [email, _] : emailToName) groups[find(email)].push_back(email);

    vector<vector<string>> res;
    for (auto& [rootEmail, emailList] : groups) {
        sort(emailList.begin(), emailList.end());
        emailList.insert(emailList.begin(), emailToName[rootEmail]);
        res.push_back(emailList);
    }
    return res;
}

// ============================================================================
// 23. SHORTEST PATH & SPECIALIZED GRAPH ALGORITHMS
// Solves: LC 743 (Dijkstra), LC 1584 (Min Cost Connect Points - Prim/Kruskal)
// Companies: Google, Amazon, Microsoft, Uber
// Complexity: Dijkstra: O((V + E) log V) | MST: O(E log V)
// ============================================================================

// Prim's Minimum Spanning Tree - Time: O(V^2) or O(E log V)
int minCostConnectPoints(vector<vector<int>>& points) {
    int n = points.size(), connected = 0, totalCost = 0;
    vector<bool> inMST(n, false);
    vector<int> minDist(n, INT_MAX);
    minDist[0] = 0;

    for (int step = 0; step < n; ++step) {
        int u = -1;
        for (int i = 0; i < n; ++i) {
            if (!inMST[i] && (u == -1 || minDist[i] < minDist[u])) u = i;
        }

        inMST[u] = true;
        totalCost += minDist[u];
        connected++;

        for (int v = 0; v < n; ++v) {
            if (!inMST[v]) {
                int dist = abs(points[u][0] - points[v][0]) + abs(points[u][1] - points[v][1]);
                minDist[v] = min(minDist[v], dist);
            }
        }
    }
    return totalCost;
}

// ============================================================================
// 24. GRAPH RECONSTRUCTION & EULERIAN PATH
// Solves: LC 133 (Clone Graph), LC 332 (Reconstruct Itinerary - Hierholzer)
// Companies: Meta, Google, Amazon, Microsoft
// Complexity: Time: O(E log E) | Space: O(V + E)
// ============================================================

vector<string> findItinerary(vector<vector<string>>& tickets) {
    unordered_map<string, priority_queue<string, vector<string>, greater<string>>> graph;
    for (const auto& t : tickets) graph[t[0]].push(t[1]);

    vector<string> itinerary;
    function<void(string)> dfs = [&](string airport) {
        auto& pq = graph[airport];
        while (!pq.empty()) {
            string nextAirport = pq.top();
            pq.pop();
            dfs(nextAirport);
        }
        itinerary.push_back(airport);
    };

    dfs("JFK");
    reverse(itinerary.begin(), itinerary.end());
    return itinerary;
}

// ============================================================================
// 25. 1D & UNBOUNDED DP
// Solves: LC 70, 746, LC 198, 213 (House Robber), LC 322, 518 (Coin Change), LC 139, 140 (Word Break)
// Companies: Amazon, Microsoft, Meta, Google, Apple
// Complexity: Word Break: O(N^2) | Coin Change: O(Amount * Coins)
// ============================================================================

// Word Break I - Time: O(N^2) | Space: O(N)
bool wordBreak(string s, vector<string>& wordDict) {
    unordered_set<string> dict(wordDict.begin(), wordDict.end());
    int n = s.size();
    vector<bool> dp(n + 1, false);
    dp[0] = true;

    for (int i = 1; i <= n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (dp[j] && dict.count(s.substr(j, i - j))) {
                dp[i] = true;
                break;
            }
        }
    }
    return dp[n];
}

// ============================================================================
// 26. 2D GRID DP
// Solves: LC 62, 63 (Unique Paths I & II), LC 64 (Min Path Sum), LC 174 (Dungeon Game)
// Companies: Google, Amazon, Microsoft, Meta
// Complexity: Time: O(R * C) | Space: O(C)
// ============================================================================

// Dungeon Game (Bottom-Right to Top-Left DP) - Time: O(R * C) | Space: O(C)
int calculateMinimumHP(vector<vector<int>>& dungeon) {
    int R = dungeon.size(), C = dungeon[0].size();
    vector<int> dp(C + 1, INT_MAX);
    dp[C - 1] = 1;

    for (int r = R - 1; r >= 0; --r) {
        for (int c = C - 1; c >= 0; --c) {
            int need = min(dp[c], dp[c + 1]) - dungeon[r][c];
            dp[c] = (need <= 0) ? 1 : need;
        }
        dp[C] = INT_MAX;
    }
    return dp[0];
}

// ============================================================================
// 27. STRINGS, SEQUENCES & MATCHING
// Solves: LC 1143 (LCS), LC 72 (Edit Distance), LC 97 (Interleaving),
//         LC 44 (Wildcard), LC 10 (Regex Matching)
// Companies: Google, Amazon, Meta, Microsoft, Apple
// Complexity: Regex: O(M * N) | Edit Distance: O(M * N)
// ============================================================================

// Regular Expression Matching ('.' and '*') - Time: O(M * N) | Space: O(M * N)
bool isMatchRegex(string s, string p) {
    int m = s.size(), n = p.size();
    vector<vector<bool>> dp(m + 1, vector<bool>(n + 1, false));
    dp[0][0] = true;

    for (int j = 2; j <= n; j += 2) {
        if (p[j - 1] == '*') dp[0][j] = dp[0][j - 2];
    }

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (p[j - 1] == '*') {
                dp[i][j] = dp[i][j - 2]; // Match 0 occurrences
                if (p[j - 2] == '.' || p[j - 2] == s[i - 1]) {
                    dp[i][j] = dp[i][j] || dp[i - 1][j]; // Match 1+ occurrences
                }
            } else if (p[j - 1] == '.' || p[j - 1] == s[i - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            }
        }
    }
    return dp[m][n];
}

// ============================================================================
// 28. INTERVALS & SUBSEQUENCES
// Solves: LC 300 (LIS O(N log N)), LC 416 (Partition Equal Subset), LC 494 (Target Sum)
// Companies: Google, Microsoft, Meta, Amazon
// Complexity: LIS: O(N log N) | Partition: O(N * Sum)
// ============================================================================

// Target Sum via Subset Sum Transformation - Time: O(N * Target) | Space: O(Target)
int findTargetSumWays(vector<int>& nums, int target) {
    int total = accumulate(nums.begin(), nums.end(), 0);
    if (total < abs(target) || (total + target) % 2 != 0) return 0;
    int subsetSum = (total + target) / 2;

    vector<int> dp(subsetSum + 1, 0);
    dp[0] = 1;
    for (int x : nums) {
        for (int j = subsetSum; j >= x; --j) dp[j] += dp[j - x];
    }
    return dp[subsetSum];
}

// ============================================================================
// 29. ADVANCED DP PARADIGMS (STATE MACHINE & BURST BALLOONS)
// Solves: LC 312 (Burst Balloons), LC 121..123, 188, 309, 714 (Stock Series), LC 337 (House Robber III)
// Companies: Google, Amazon, Meta, Microsoft, Citadel
// Complexity: Burst Balloons: O(N^3) | Stock Series: O(N)
// ============================================================================

// 1. Burst Balloons (Interval DP) - Time: O(N^3) | Space: O(N^2)
int maxCoins(vector<int>& nums) {
    int n = nums.size();
    vector<int> arr(n + 2, 1);
    for (int i = 0; i < n; ++i) arr[i + 1] = nums[i];

    vector<vector<int>> dp(n + 2, vector<int>(n + 2, 0));
    for (int len = 1; len <= n; ++len) {
        for (int left = 1; left <= n - len + 1; ++left) {
            int right = left + len - 1;
            for (int k = left; k <= right; ++k) {
                int gain = arr[left - 1] * arr[k] * arr[right + 1];
                dp[left][right] = max(dp[left][right], gain + dp[left][k - 1] + dp[k + 1][right]);
            }
        }
    }
    return dp[1][n];
}

// 2. Best Time to Buy and Sell Stock with Cooldown - Time: O(N) | Space: O(1)
int maxProfitCooldown(const vector<int>& prices) {
    int hold = INT_MIN, sold = 0, rest = 0;
    for (int p : prices) {
        int prevSold = sold;
        sold = hold + p;
        hold = max(hold, rest - p);
        rest = max(rest, prevSold);
    }
    return max(sold, rest);
}

// ============================================================================
// 30. TOP-K PATTERNS & TASK SCHEDULER
// Solves: LC 215 (QuickSelect), LC 347 (Top K Frequent), LC 621 (Task Scheduler), LC 767 (Reorganize String)
// Companies: Meta, Amazon, Microsoft, Google, Apple
// Complexity: QuickSelect: Avg O(N) | Task Scheduler: O(N)
// ============================================================================

// Task Scheduler (Max Frequency Idle Calculation) - Time: O(N) | Space: O(1)
int leastInterval(vector<char>& tasks, int n) {
    vector<int> freq(26, 0);
    for (char c : tasks) freq[c - 'A']++;
    int maxFreq = *max_element(freq.begin(), freq.end());
    int maxCount = count(freq.begin(), freq.end(), maxFreq);

    int emptySlots = (maxFreq - 1) * (n - (maxCount - 1));
    int availableTasks = tasks.size() - maxFreq * maxCount;
    int idles = max(0, emptySlots - availableTasks);

    return tasks.size() + idles;
}

// ============================================================================
// 31. MULTI-HEAP PATTERNS (MEDIAN FINDERS)
// Solves: LC 295 (Find Median Data Stream), LC 480 (Sliding Window Median)
// Companies: Amazon, Google, Microsoft, Meta, Citadel
// Complexity: O(log N) per add | O(1) median
// ============================================================================

class SlidingWindowMedian {
    multiset<long long> low, high;

    void balance() {
        while (low.size() > high.size() + 1) {
            high.insert(*low.rbegin());
            low.erase(prev(low.end()));
        }
        while (low.size() < high.size()) {
            low.insert(*high.begin());
            high.erase(high.begin());
        }
    }

public:
    void add(long long val) {
        if (low.empty() || val <= *low.rbegin()) low.insert(val);
        else high.insert(val);
        balance();
    }

    void remove(long long val) {
        auto it = low.find(val);
        if (it != low.end()) low.erase(it);
        else high.erase(high.find(val));
        balance();
    }

    double getMedian(int k) {
        return (k % 2 == 1) ? *low.rbegin() : (*low.rbegin() + *high.begin()) / 2.0;
    }
};

// ============================================================================
// 32. INTERVAL SCHEDULING PATTERNS
// Solves: LC 56 (Merge Intervals), LC 57 (Insert Interval), LC 435 (Non-overlapping),
//         LC 252, 253 (Meeting Rooms I & II)
// Companies: Meta, Amazon, Google, Microsoft, Bloomberg
// Complexity: Time: O(N log N) | Space: O(N)
// ============================================================================

// Meeting Rooms II (Min Conference Rooms Needed) - Time: O(N log N) | Space: O(N)
int minMeetingRooms(vector<vector<int>>& intervals) {
    if (intervals.empty()) return 0;
    sort(intervals.begin(), intervals.end());
    priority_queue<int, vector<int>, greater<int>> minEndTimes;

    for (const auto& in : intervals) {
        if (!minEndTimes.empty() && minEndTimes.top() <= in[0]) {
            minEndTimes.pop(); // Room freed
        }
        minEndTimes.push(in[1]);
    }
    return minEndTimes.size();
}

// Insert Interval - Time: O(N) | Space: O(N)
vector<vector<int>> insertInterval(vector<vector<int>>& intervals, vector<int>& newInterval) {
    vector<vector<int>> res;
    int i = 0, n = intervals.size();

    while (i < n && intervals[i][1] < newInterval[0]) res.push_back(intervals[i++]);
    while (i < n && intervals[i][0] <= newInterval[1]) {
        newInterval[0] = min(newInterval[0], intervals[i][0]);
        newInterval[1] = max(newInterval[1], intervals[i][1]);
        i++;
    }
    res.push_back(newInterval);
    while (i < n) res.push_back(intervals[i++]);
    return res;
}

// ============================================================================
// 33. ROTATED & BOUNDARY BINARY SEARCH
// Solves: LC 33, LC 81 (Rotated Array with Dups), LC 153 (Find Min), LC 34 (First & Last Position)
// Companies: Meta, Amazon, Microsoft, Apple, Google
// Complexity: Time: O(log N) / Worst O(N) with duplicate plateau
// ============================================================================

// Search in Rotated Sorted Array II (With Duplicates) - Time: Avg O(log N), Worst O(N)
bool searchRotatedWithDuplicates(const vector<int>& nums, int target) {
    int l = 0, r = nums.size() - 1;
    while (l <= r) {
        int mid = l + (r - l) / 2;
        if (nums[mid] == target) return true;

        if (nums[l] == nums[mid] && nums[mid] == nums[r]) {
            l++; r--; // Skip ambiguous duplicates
        } else if (nums[l] <= nums[mid]) {
            if (nums[l] <= target && target < nums[mid]) r = mid - 1;
            else l = mid + 1;
        } else {
            if (nums[mid] < target && target <= nums[r]) l = mid + 1;
            else r = mid - 1;
        }
    }
    return false;
}

// ============================================================================
// 34. BINARY SEARCH ON ANSWER & MEDIAN OF TWO SORTED ARRAYS
// Solves: LC 875, LC 1011, LC 410 (Split Array Largest Sum), LC 4 (Median 2 Sorted Arrays)
// Companies: Google, Amazon, Meta, Microsoft, Apple
// Complexity: Median: O(log(min(M, N))) | Predicate BS: O(N log Range)
// ============================================================================

// Median of Two Sorted Arrays - Time: O(log(min(M, N))) | Space: O(1)
double findMedianSortedArrays(const vector<int>& nums1, const vector<int>& nums2) {
    if (nums1.size() > nums2.size()) return findMedianSortedArrays(nums2, nums1);
    int m = nums1.size(), n = nums2.size();
    int low = 0, high = m;

    while (low <= high) {
        int cut1 = low + (high - low) / 2;
        int cut2 = (m + n + 1) / 2 - cut1;

        int l1 = (cut1 == 0) ? INT_MIN : nums1[cut1 - 1];
        int l2 = (cut2 == 0) ? INT_MIN : nums2[cut2 - 1];
        int r1 = (cut1 == m) ? INT_MAX : nums1[cut1];
        int r2 = (cut2 == n) ? INT_MAX : nums2[cut2];

        if (l1 <= r2 && l2 <= r1) {
            if ((m + n) % 2 == 1) return max(l1, l2);
            return (max(l1, l2) + min(r1, r2)) / 2.0;
        } else if (l1 > r2) {
            high = cut1 - 1;
        } else {
            low = cut1 + 1;
        }
    }
    return 0.0;
}

// ============================================================================
// 35. RANGE QUERY DATA STRUCTURES (SEGMENT & FENWICK TREE)
// Solves: LC 307 (Range Sum Query Mutable), LC 315 (Smaller Numbers After Self)
// Companies: Google, Meta, Amazon
// Complexity: Query/Update: O(log N) | Build: O(N)
// ============================================================================

class FenwickTree {
    vector<int> tree;
    int n;
public:
    FenwickTree(int n) : n(n), tree(n + 1, 0) {}
    void add(int i, int delta) { for (; i <= n; i += i & -i) tree[i] += delta; }
    int query(int i) { int sum = 0; for (; i > 0; i -= i & -i) sum += tree[i]; return sum; }
};

// ============================================================================
// 36. CUSTOM STACK & QUEUE VARIANTS
// Solves: LC 155 (Min Stack), LC 232 (Queue using Stacks)
// Companies: Amazon, Microsoft, Bloomberg, Meta
// Complexity: All operations O(1) amortized
// ============================================================================

class MyQueue {
    stack<int> inStack, outStack;
    void shift() {
        if (outStack.empty()) {
            while (!inStack.empty()) {
                outStack.push(inStack.top());
                inStack.pop();
            }
        }
    }
public:
    void push(int x) { inStack.push(x); }
    int pop() { shift(); int v = outStack.top(); outStack.pop(); return v; }
    int peek() { shift(); return outStack.top(); }
    bool empty() { return inStack.empty() && outStack.empty(); }
};

// ============================================================================
// 37. CUSTOM COLLECTIONS & IN-MEMORY SYSTEMS
// Solves: LC 380 (Insert Delete GetRandom O(1)), LC 355 (Design Twitter)
// Companies: Meta, Amazon, Google, Microsoft, Apple, Twitter
// Complexity: All operations O(1) average
// ============================================================================

// Insert Delete GetRandom O(1) - Time: O(1) | Space: O(N)
class RandomizedSet {
    vector<int> nums;
    unordered_map<int, int> valToIdx;

public:
    bool insert(int val) {
        if (valToIdx.count(val)) return false;
        valToIdx[val] = nums.size();
        nums.push_back(val);
        return true;
    }

    bool remove(int val) {
        if (!valToIdx.count(val)) return false;
        int idx = valToIdx[val];
        int lastVal = nums.back();

        nums[idx] = lastVal;
        valToIdx[lastVal] = idx;

        nums.pop_back();
        valToIdx.erase(val);
        return true;
    }

    int getRandom() {
        return nums[rand() % nums.size()];
    }
};

// ============================================================================
// 38. STREAM & FILE SYSTEM DESIGN
// Solves: LC 158 (Read4 II Multiple Times), LC 588 (Design In-Memory File System)
// Companies: Google, Amazon, Meta, Microsoft
// Complexity: FS Search: O(Path_Length)
// ============================================================================

// Read N Characters Given Read4 II (Call Multiple Times)
int read4(char *buf4); // API stub

class SolutionRead4 {
    char buf4[4];
    int bufPtr = 0, bufCount = 0;
public:
    int read(char *buf, int n) {
        int totalRead = 0;
        while (totalRead < n) {
            if (bufPtr == bufCount) {
                bufCount = read4(buf4);
                bufPtr = 0;
                if (bufCount == 0) break; // EOF
            }
            buf[totalRead++] = buf4[bufPtr++];
        }
        return totalRead;
    }
};

/*
================================================================================
  INTERVIEW CHEAT SHEETS (FAANG PLAYBOOK)
================================================================================

  45-MINUTE LIVE CODING INTERVIEW FLOW
  ------------------------------------------------------------------------------
  00:00 - 05:00 | Clarification & Constraints
                - Ask about edge cases (null inputs, duplicates, negative numbers, empty arrays).
                - Clarify scale (N <= 10^3 vs N <= 10^5 determines acceptable time complexity).
  05:00 - 12:00 | Conceptualize & Dry Run Algorithm
                - State brute force solution explicitly (establishes baseline).
                - Verbally explain optimal approach + data structure choice.
                - Trace dry run on a sample test case before typing a single line of code.
  12:00 - 32:00 | Clean Implementation
                - Write self-documenting code with clear variable names.
                - Keep function modular (helper functions for sub-tasks).
  32:00 - 40:00 | Testing & Edge Case Verification
                - Walk through code manually line-by-line using dry run example.
                - Test empty/null, single-element, duplicate, extreme value inputs.
  40:00 - 45:00 | Complexity Analysis & Follow-ups
                - State Time and Auxiliary Space complexity using Big-O notation.
                - Proactively address trade-offs and potential concurrency/scalability issues.


  PATTERN RECOGNITION QUICK CHEAT TABLE
  ------------------------------------------------------------------------------
  Problem Keyword / Indicator                ->  Primary Pattern / Data Structure
  ------------------------------------------------------------------------------
  - "Top K / Most Frequent K elements"       -> Min-Heap / QuickSelect
  - "Continuous Subarray / Substring"         -> Sliding Window / Prefix Sum
  - "Search in Sorted Matrix / Array"        -> Binary Search
  - "Shortest Path in Unweighted Graph/Grid" -> BFS
  - "Shortest Path in Weighted Graph"        -> Dijkstra's Algorithm
  - "Find Connected Components / Redundancy" -> Disjoint Set Union (DSU) / DFS
  - "Dependencies / Scheduling Order"         -> Topological Sort (Kahn's Indegree)
  - "Matching / Prefix Lookups / Words"      -> Trie (Prefix Tree)
  - "Max / Min Subarray Sum / Product"       -> Kadane's DP / Monotonic Queue
  - "Next Greater / Smaller Element"         -> Monotonic Stack
  - "Combinations / Permutations / Paths"    -> Backtracking / DFS
  - "Optimal Choice with Subproblems"       -> Dynamic Programming
  - "Data Stream Median / Dynamic Quantiles" -> Two Heaps (Max-Heap + Min-Heap)
  - "Range Updates / Overlapping Intervals"  -> Sweep Line / Difference Array
================================================================================
*/
