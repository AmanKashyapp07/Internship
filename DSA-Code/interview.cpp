/*
================================================================================
  FAANG & TIER-1 TECH LIVE CODING INTERVIEW REVISION SHEET (C++17)
  Target: Meta, Google, Amazon, Apple, Netflix, Uber, Stripe, Microsoft, Bloomberg
  Style: Clean, Interview-Realistic C++17 (Readable, Standard Library, No CP Macros)
  File: DSA/interview.cpp

  PRIORITY LEGEND:
  [CORE]     - Must-know high-ROI pattern / universally asked live interview problem
  [HIGH]     - Frequently asked interview question / essential pattern variant
  [MEDIUM]   - Solid pattern practice / common secondary interview problem
  [ADVANCED] - Specialized / hard problem (study after mastering CORE & HIGH)
================================================================================
  TABLE OF CONTENTS:

  01. ARRAYS & HASHING
      - [CORE] Two Sum
      - [CORE] Valid Anagram
      - [CORE] Group Anagrams
      - [CORE] Product of Array Except Self
      - [CORE] Majority Element (Boyer-Moore Voting)
      - [CORE] Longest Consecutive Sequence
      - [HIGH] Find All Duplicates in Array
      - [HIGH] Valid Sudoku
      - [HIGH] Rotate Array

  02. TWO POINTERS
      - [CORE] Pair Sum in Sorted Array (Two Sum II)
      - [CORE] Container With Most Water
      - [CORE] 3Sum
      - [HIGH] 4Sum
      - [CORE] Trapping Rain Water
      - [MEDIUM] Shortest Unsorted Continuous Subarray

  03. SLIDING WINDOW
      - [CORE] Maximum Sum Subarray of Size K
      - [CORE] Sliding Window Maximum (Monotonic Deque)
      - [CORE] Longest Substring Without Repeating Characters
      - [HIGH] Longest Substring with At Most K Distinct Characters
      - [CORE] Minimum Window Substring
      - [HIGH] Subarrays with K Distinct Integers

  04. PREFIX SUM
      - [CORE] Subarray Sum Equals K
      - [CORE] Contiguous Array (Equal 0s and 1s)
      - [HIGH] Subarray Sums Divisible by K
      - [HIGH] Difference Array / Range Updates

  05. STACK & MONOTONIC STACK
      - [CORE] Valid Parentheses
      - [CORE] Min Stack
      - [HIGH] Evaluate Reverse Polish Notation
      - [CORE] Decode String
      - [HIGH] Next Greater Element I
      - [HIGH] Next Greater Element II (Circular)
      - [CORE] Daily Temperatures
      - [CORE] Largest Rectangle in Histogram
      - [HIGH] Maximal Rectangle (2D Grid)
      - [HIGH] Basic Calculator II

  06. GREEDY
      - [CORE] Jump Game
      - [CORE] Jump Game II
      - [CORE] Gas Station
      - [HIGH] Partition Labels
      - [HIGH] Non-overlapping Intervals
      - [HIGH] Minimum Number of Arrows to Burst Balloons
      - [CORE] Task Scheduler
      - [HIGH] Hand of Straights
      - [MEDIUM] Merge Triplets to Form Target

  07. BINARY SEARCH
      - [CORE] Binary Search (Standard Template)
      - [CORE] Find First and Last Position of Element
      - [CORE] Search in Rotated Sorted Array
      - [HIGH] Search in Rotated Sorted Array II (Duplicates)
      - [CORE] Find Minimum in Rotated Sorted Array
      - [CORE] Koko Eating Bananas
      - [HIGH] Capacity to Ship Packages Within D Days
      - [HIGH] Split Array Largest Sum
      - [ADVANCED] Median of Two Sorted Arrays
      - [HIGH] Time Based Key-Value Store

  09. TREES & BST
      - [CORE] Tree Traversals: Preorder, Inorder, Postorder (Recursive & Iterative)
      - [CORE] Level Order Traversal
      - [HIGH] Zigzag Level Order Traversal
      - [HIGH] Right Side View & Left Side View
      - [HIGH] Vertical Order Traversal
      - [CORE] Diameter of Binary Tree
      - [CORE] Maximum Path Sum
      - [CORE] Lowest Common Ancestor (BT, BST, Parent Pointer)
      - [CORE] Validate Binary Search Tree
      - [CORE] Balanced Binary Tree
      - [HIGH] Path Sum I & II
      - [HIGH] Binary Tree Paths
      - [HIGH] Kth Smallest Element in BST
      - [HIGH] Binary Search Tree Iterator
      - [CORE] Serialize and Deserialize Binary Tree
      - [CORE] Construct Binary Tree from Preorder & Inorder Traversal
      - [ADVANCED] Morris Inorder Traversal

  10. TRIE
      - [CORE] Implement Trie (Prefix Tree)
      - [HIGH] Design Add and Search Words Data Structure (Wildcard '.')
      - [HIGH] Word Search II (Trie + 2D Backtracking)

  11. HEAPS & PRIORITY QUEUES
      - [CORE] Kth Largest Element in an Array (QuickSelect & Min-Heap)
      - [CORE] Top K Frequent Elements
      - [HIGH] Top K Frequent Words
      - [HIGH] K Closest Points to Origin
      - [CORE] Merge K Sorted Lists (Min-Heap)
      - [CORE] Find Median from Data Stream (Two Heaps)
      - [ADVANCED] Sliding Window Median (Two Balanced Multisets)
      - [HIGH] Reorganize String

  12. INTERVALS
      - [CORE] Merge Intervals
      - [CORE] Insert Interval
      - [HIGH] Non-overlapping Intervals
      - [CORE] Meeting Rooms I
      - [CORE] Meeting Rooms II
      - [HIGH] Employee Free Time
      - [HIGH] Minimum Number of Arrows to Burst Balloons

  13. BACKTRACKING
      - [CORE] Subsets
      - [CORE] Subsets II (With Duplicates)
      - [HIGH] Combinations
      - [CORE] Combination Sum
      - [HIGH] Combination Sum II
      - [CORE] Permutations
      - [HIGH] Permutations II (With Duplicates)
      - [CORE] Next Permutation
      - [HIGH] N-Queens
      - [HIGH] Sudoku Solver
      - [CORE] Word Search (2D Grid)
      - [HIGH] Palindrome Partitioning
      - [HIGH] Restore IP Addresses

  14. GRAPH BFS / DFS
      - [CORE] Number of Islands
      - [CORE] Rotting Oranges (Multi-Source BFS)
      - [HIGH] Shortest Path in Binary Matrix
      - [HIGH] Pacific Atlantic Water Flow
      - [CORE] Clone Graph
      - [HIGH] Graph Valid Tree
      - [HIGH] Number of Connected Components in an Undirected Graph
      - [CORE] Word Ladder
      - [HIGH] Open the Lock

  15. TOPOLOGICAL SORT
      - [CORE] Course Schedule I (Kahn's Indegree BFS & DFS 3-State Cycle Detection)
      - [CORE] Course Schedule II
      - [HIGH] Alien Dictionary

  16. DISJOINT SET UNION (DSU)
      - [CORE] Reusable DSU Class (Path Compression + Union by Rank/Size)
      - [CORE] Number of Provinces
      - [CORE] Redundant Connection
      - [HIGH] Accounts Merge

  17. SHORTEST PATH & MST
      - [CORE] Dijkstra's Algorithm (Template & Network Delay Time)
      - [HIGH] Cheapest Flights Within K Stops
      - [HIGH] 0-1 BFS
      - [MEDIUM] Bellman-Ford Algorithm (Overview)
      - [HIGH] Kruskal's Algorithm (MST)
      - [HIGH] Prim's Algorithm (Min Cost to Connect All Points)

  18. 1D DYNAMIC PROGRAMMING
      - [CORE] Climbing Stairs
      - [CORE] Min Cost Climbing Stairs
      - [CORE] House Robber I
      - [CORE] House Robber II (Circular Array)
      - [CORE] Coin Change (Fewest Coins)
      - [HIGH] Coin Change II (Total Ways)
      - [CORE] Decode Ways
      - [CORE] Word Break
      - [CORE] Maximum Product Subarray
      - [CORE] Maximum Subarray (Kadane's Algorithm)

  19. 2D / STRING DYNAMIC PROGRAMMING
      - [CORE] Unique Paths
      - [CORE] Unique Paths II (With Obstacles)
      - [CORE] Minimum Path Sum
      - [CORE] Longest Common Subsequence (LCS)
      - [CORE] Edit Distance
      - [HIGH] Interleaving String
      - [HIGH] Longest Palindromic Subsequence
      - [ADVANCED] Wildcard Matching
      - [ADVANCED] Regular Expression Matching

  20. ADVANCED DYNAMIC PROGRAMMING
      - [HIGH] Longest Increasing Subsequence (LIS O(N log N))
      - [HIGH] Partition Equal Subset Sum
      - [HIGH] Target Sum
      - [ADVANCED] Burst Balloons (Interval DP)
      - [ADVANCED] Matrix Chain Multiplication (Interval DP)
      - [HIGH] Best Time to Buy and Sell Stock with Cooldown / Fee
      - [ADVANCED] Tree DP: House Robber III
      - [ADVANCED] Dungeon Game
      - [ADVANCED] Cherry Pickup

  21. DESIGN-ORIENTED DATA STRUCTURES
      - [CORE] Min Stack
      - [CORE] Implement Queue using Stacks
      - [CORE] Design HashMap
      - [HIGH] Design HashSet
      - [CORE] LRU Cache
      - [CORE] Insert Delete GetRandom O(1)
      - [HIGH] Time Based Key-Value Store
      - [HIGH] Logger Rate Limiter
      - [ADVANCED] LFU Cache
      - [ADVANCED] All O(1) Data Structure
      - [ADVANCED] Design In-Memory File System
      - [ADVANCED] Design Twitter
      - [ADVANCED] Read N Characters Given Read4 II

  22. INTERVIEW FOLLOW-UP PATTERNS
      - Two Sum Extensions & Big Data
      - Number of Islands Variants
      - LRU Cache Concurrency & TTL
      - BFS / Shortest Path Trade-offs
      - Binary Search on Real Numbers / Predicates
      - Top-K & Streaming Quantiles
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
// COMMON INTERVIEW DATA STRUCTURE DEFINITIONS
// ============================================================================

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x = 0, ListNode* n = nullptr) : val(x), next(n) {}
};

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x = 0, TreeNode* l = nullptr, TreeNode* r = nullptr) : val(x), left(l), right(r) {}
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
    GraphNode(int _val = 0) : val(_val) {}
};

struct NodeWithParent {
    int val;
    NodeWithParent* left;
    NodeWithParent* right;
    NodeWithParent* parent;
    NodeWithParent(int _val = 0) : val(_val), left(nullptr), right(nullptr), parent(nullptr) {}
};


// ============================================================================
// 01. ARRAYS & HASHING
// ============================================================================

// [CORE] Two Sum
// Idea: Use a hash map to store the complement (target - current value) and its index.
// Time: O(n)
// Space: O(n)
vector<int> twoSum(const vector<int>& nums, int target) {
    unordered_map<int, int> numToIdx;
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        int complement = target - nums[i];
        if (numToIdx.count(complement)) {
            return {numToIdx[complement], i};
        }
        numToIdx[nums[i]] = i;
    }
    return {};
}

// [CORE] Valid Anagram
// Idea: Count frequencies of characters using a fixed array of size 26.
// Time: O(n)
// Space: O(1)
bool isAnagram(const string& s, const string& t) {
    if (s.size() != t.size()) return false;
    vector<int> freq(26, 0);
    for (char c : s) freq[c - 'a']++;
    for (char c : t) {
        if (--freq[c - 'a'] < 0) return false;
    }
    return true;
}

// [CORE] Group Anagrams
// Idea: Sort each string to create a canonical key, and group original strings in a hash map.
// Time: O(n * k log k) where n is number of strings, k is max string length
// Space: O(n * k)
vector<vector<string>> groupAnagrams(const vector<string>& strs) {
    unordered_map<string, vector<string>> groups;
    for (const string& s : strs) {
        string key = s;
        sort(key.begin(), key.end());
        groups[key].push_back(s);
    }
    vector<vector<string>> res;
    res.reserve(groups.size());
    for (auto& [_, group] : groups) {
        res.push_back(std::move(group));
    }
    return res;
}

// [CORE] Product of Array Except Self
// Idea: Compute prefix products in the result array, then multiply by suffix products using a running variable.
// Time: O(n)
// Space: O(1) auxiliary (excluding output array)
vector<int> productExceptSelf(const vector<int>& nums) {
    int n = nums.size();
    vector<int> res(n, 1);
    int prefix = 1;
    for (int i = 0; i < n; ++i) {
        res[i] = prefix;
        prefix *= nums[i];
    }
    int suffix = 1;
    for (int i = n - 1; i >= 0; --i) {
        res[i] *= suffix;
        suffix *= nums[i];
    }
    return res;
}

// [CORE] Majority Element (Boyer-Moore Voting Algorithm)
// Idea: Maintain candidate and balance counter. If counter reaches 0, pick current element as new candidate.
// Time: O(n)
// Space: O(1)
int majorityElement(const vector<int>& nums) {
    int candidate = 0, count = 0;
    for (int x : nums) {
        if (count == 0) {
            candidate = x;
        }
        count += (x == candidate) ? 1 : -1;
    }
    return candidate;
}

// [CORE] Longest Consecutive Sequence
// Idea: Insert all numbers into a hash set. Only start counting sequence lengths from sequence starts (x - 1 not present).
// Time: O(n)
// Space: O(n)
int longestConsecutive(const vector<int>& nums) {
    unordered_set<int> numSet(nums.begin(), nums.end());
    int longest = 0;
    for (int x : numSet) {
        if (!numSet.count(x - 1)) { // Only check from start of sequence
            int currentNum = x;
            int currentStreak = 1;
            while (numSet.count(currentNum + 1)) {
                currentNum++;
                currentStreak++;
            }
            longest = max(longest, currentStreak);
        }
    }
    return longest;
}

// [HIGH] Find All Duplicates in Array
// Idea: Since values are in range [1..n], negate nums[abs(x) - 1] to mark visited elements in-place.
// Time: O(n)
// Space: O(1) auxiliary
vector<int> findDuplicates(vector<int>& nums) {
    vector<int> duplicates;
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        int idx = abs(nums[i]) - 1;
        if (nums[idx] < 0) {
            duplicates.push_back(idx + 1);
        } else {
            nums[idx] = -nums[idx];
        }
    }
    return duplicates;
}

// [HIGH] Valid Sudoku
// Idea: Use bitsets / hash arrays to verify uniqueness of digits 1-9 across rows, columns, and 3x3 sub-boxes.
// Time: O(1) (fixed 9x9 board)
// Space: O(1)
bool isValidSudoku(const vector<vector<char>>& board) {
    int rows[9] = {0}, cols[9] = {0}, boxes[9] = {0};
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (board[r][c] == '.') continue;
            int digit = board[r][c] - '1';
            int bit = 1 << digit;
            int boxIdx = (r / 3) * 3 + (c / 3);

            if ((rows[r] & bit) || (cols[c] & bit) || (boxes[boxIdx] & bit)) {
                return false;
            }
            rows[r] |= bit;
            cols[c] |= bit;
            boxes[boxIdx] |= bit;
        }
    }
    return true;
}

// [HIGH] Rotate Array
// Idea: Reverse entire array, then reverse first k elements, then reverse the remaining n - k elements.
// Time: O(n)
// Space: O(1)
void rotateArray(vector<int>& nums, int k) {
    int n = nums.size();
    k %= n;
    reverse(nums.begin(), nums.end());
    reverse(nums.begin(), nums.begin() + k);
    reverse(nums.begin() + k, nums.end());
}


// ============================================================================
// 02. TWO POINTERS
// ============================================================================

// [CORE] Pair Sum in Sorted Array (Two Sum II)
// Idea: Use two pointers at ends; move left rightward if sum < target, and right leftward if sum > target.
// Time: O(n)
// Space: O(1)
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

// [CORE] Container With Most Water
// Idea: Maximize area by moving the pointer pointing to the shorter vertical line inward.
// Time: O(n)
// Space: O(1)
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

// [CORE] 3Sum (Unique Triplets Summing to Zero)
// Idea: Sort array. Fix one anchor element and use two pointers for remaining two elements; skip duplicates.
// Time: O(n^2)
// Space: O(1) auxiliary (excluding output)
vector<vector<int>> threeSum(vector<int>& nums) {
    sort(nums.begin(), nums.end());
    vector<vector<int>> res;
    int n = nums.size();

    for (int i = 0; i < n - 2; ++i) {
        if (i > 0 && nums[i] == nums[i - 1]) continue; // Skip duplicate anchor
        if (nums[i] > 0) break; // Optimization: all remaining numbers positive

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

// [HIGH] 4Sum
// Idea: Sort array, use two nested loops for the first two anchors, and two pointers for the remaining pair.
// Time: O(n^3)
// Space: O(1) auxiliary
vector<vector<int>> fourSum(vector<int>& nums, int target) {
    sort(nums.begin(), nums.end());
    vector<vector<int>> res;
    int n = nums.size();

    for (int i = 0; i < n - 3; ++i) {
        if (i > 0 && nums[i] == nums[i - 1]) continue;
        for (int j = i + 1; j < n - 2; ++j) {
            if (j > i + 1 && nums[j] == nums[j - 1]) continue;

            long long needed = (long long)target - nums[i] - nums[j];
            int left = j + 1, right = n - 1;
            while (left < right) {
                long long sum = nums[left] + nums[right];
                if (sum == needed) {
                    res.push_back({nums[i], nums[j], nums[left], nums[right]});
                    while (left < right && nums[left] == nums[left + 1]) left++;
                    while (left < right && nums[right] == nums[right - 1]) right--;
                    left++; right--;
                } else if (sum < needed) {
                    left++;
                } else {
                    right--;
                }
            }
        }
    }
    return res;
}

// [CORE] Trapping Rain Water
// Idea: Two pointers maintain leftMax and rightMax; the smaller side determines current trapped water height.
// Time: O(n)
// Space: O(1)
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

// [MEDIUM] Shortest Unsorted Continuous Subarray
// Idea: Track max from left to find right boundary violation, and min from right to find left boundary violation.
// Time: O(n)
// Space: O(1)
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
// 03. SLIDING WINDOW
// ============================================================================

// [CORE] Maximum Sum Subarray of Size K
// Idea: Maintain sum of fixed window of size k by adding incoming element and subtracting outgoing element.
// Time: O(n)
// Space: O(1)
int maxSumSubarraySizeK(const vector<int>& nums, int k) {
    int n = nums.size();
    if (n < k) return 0;
    int windowSum = 0;
    for (int i = 0; i < k; ++i) windowSum += nums[i];
    int maxSum = windowSum;
    for (int i = k; i < n; ++i) {
        windowSum += nums[i] - nums[i - k];
        maxSum = max(maxSum, windowSum);
    }
    return maxSum;
}

// [CORE] Sliding Window Maximum
// Idea: Use a monotonic deque storing indices of decreasing elements. Deque front always holds window maximum.
// Time: O(n)
// Space: O(k)
vector<int> maxSlidingWindow(const vector<int>& nums, int k) {
    deque<int> dq; // Stores indices with decreasing values
    vector<int> res;
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        if (!dq.empty() && dq.front() == i - k) dq.pop_front();
        while (!dq.empty() && nums[dq.back()] <= nums[i]) dq.pop_back();
        dq.push_back(i);
        if (i >= k - 1) res.push_back(nums[dq.front()]);
    }
    return res;
}

// [CORE] Longest Substring Without Repeating Characters
// Idea: Expand window rightward and record last seen index of each character to jump left pointer forward.
// Time: O(n)
// Space: O(min(n, alphabet))
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

// [HIGH] Longest Substring with At Most K Distinct Characters
// Idea: Expand right pointer and keep character frequency map; shrink left pointer whenever distinct count exceeds k.
// Time: O(n)
// Space: O(k)
int lengthOfLongestSubstringKDistinct(const string& s, int k) {
    if (k == 0) return 0;
    unordered_map<char, int> freq;
    int left = 0, maxLen = 0;
    for (int right = 0; right < static_cast<int>(s.size()); ++right) {
        freq[s[right]]++;
        while (static_cast<int>(freq.size()) > k) {
            if (--freq[s[left]] == 0) freq.erase(s[left]);
            left++;
        }
        maxLen = max(maxLen, right - left + 1);
    }
    return maxLen;
}

// [CORE] Minimum Window Substring
// Idea: Track required character counts in a map and count matched unique characters. Shrink from left once valid.
// Time: O(n + m) where n = s.length(), m = t.length()
// Space: O(alphabet)
string minWindow(const string& s, const string& t) {
    if (s.empty() || t.empty()) return "";
    unordered_map<char, int> tCount;
    for (char c : t) tCount[c]++;

    unordered_map<char, int> windowCount;
    int have = 0, need = tCount.size();
    int minLen = INT_MAX, minStart = 0;
    int left = 0;

    for (int right = 0; right < static_cast<int>(s.size()); ++right) {
        char c = s[right];
        if (tCount.count(c)) {
            windowCount[c]++;
            if (windowCount[c] == tCount[c]) have++;
        }

        while (have == need) {
            if (right - left + 1 < minLen) {
                minLen = right - left + 1;
                minStart = left;
            }
            char leftChar = s[left];
            if (tCount.count(leftChar)) {
                if (windowCount[leftChar] == tCount[leftChar]) have--;
                windowCount[leftChar]--;
            }
            left++;
        }
    }
    return (minLen == INT_MAX) ? "" : s.substr(minStart, minLen);
}

// [HIGH] Subarrays with K Distinct Integers
// Idea: Exact(k) = AtMost(k) - AtMost(k - 1). Helper counts subarrays with at most k distinct numbers.
// Time: O(n)
// Space: O(k)
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

int subarraysWithKDistinct(const vector<int>& nums, int k) {
    return atMostKDistinct(nums, k) - atMostKDistinct(nums, k - 1);
}


// ============================================================================
// 04. PREFIX SUM
// ============================================================================

// [CORE] Subarray Sum Equals K
// Idea: Prefix sum hash map. Number of subarrays ending at i with sum k is count of prefix sums equal to (currSum - k).
// Time: O(n)
// Space: O(n)
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

// [CORE] Contiguous Array (Equal 0s and 1s)
// Idea: Treat 0 as -1 and 1 as +1. Find the longest subarray with sum 0 by recording first seen index of each prefix sum.
// Time: O(n)
// Space: O(n)
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

// [HIGH] Subarray Sums Divisible by K
// Idea: Map prefix sum modulo k. Normalize negative remainders: ((currSum % k) + k) % k.
// Time: O(n)
// Space: O(k)
int subarraysDivByK(const vector<int>& nums, int k) {
    unordered_map<int, int> modCount;
    modCount[0] = 1;
    int currSum = 0, total = 0;
    for (int x : nums) {
        currSum += x;
        int rem = ((currSum % k) + k) % k;
        if (modCount.count(rem)) total += modCount[rem];
        modCount[rem]++;
    }
    return total;
}

// [HIGH] Difference Array / Range Updates (Range Addition)
// Idea: Apply range updates [start, end, val] in O(1) via diff[start] += val and diff[end + 1] -= val, then prefix sum.
// Time: O(n + updates.size())
// Space: O(n)
vector<int> getModifiedArray(int length, const vector<vector<int>>& updates) {
    vector<int> diff(length + 1, 0);
    for (const auto& u : updates) {
        int start = u[0], end = u[1], val = u[2];
        diff[start] += val;
        if (end + 1 < length) diff[end + 1] -= val;
    }
    vector<int> res(length, 0);
    int curr = 0;
    for (int i = 0; i < length; ++i) {
        curr += diff[i];
        res[i] = curr;
    }
    return res;
}


// ============================================================================
// 05. STACK & MONOTONIC STACK
// ============================================================================

// [CORE] Valid Parentheses
// Idea: Push expected closing bracket onto stack when opening bracket is encountered; match on close.
// Time: O(n)
// Space: O(n)
bool isValidParentheses(const string& s) {
    stack<char> st;
    for (char c : s) {
        if (c == '(') st.push(')');
        else if (c == '{') st.push('}');
        else if (c == '[') st.push(']');
        else {
            if (st.empty() || st.top() != c) return false;
            st.pop();
        }
    }
    return st.empty();
}

// [CORE] Min Stack
// Idea: Maintain primary stack alongside a min-tracker stack that records the minimum value at each depth.
// Time: O(1) for all operations (push, pop, top, getMin)
// Space: O(n)
class MinStack {
    stack<int> valStack;
    stack<int> minStack;
public:
    MinStack() {}
    void push(int val) {
        valStack.push(val);
        if (minStack.empty() || val <= minStack.top()) {
            minStack.push(val);
        }
    }
    void pop() {
        if (valStack.top() == minStack.top()) {
            minStack.pop();
        }
        valStack.pop();
    }
    int top() { return valStack.top(); }
    int getMin() { return minStack.top(); }
};

// [HIGH] Evaluate Reverse Polish Notation
// Idea: Use a stack to evaluate postfix expression: pop top two operands on operator, compute, push back.
// Time: O(n)
// Space: O(n)
int evalRPN(const vector<string>& tokens) {
    stack<int> st;
    for (const string& t : tokens) {
        if (t == "+" || t == "-" || t == "*" || t == "/") {
            int b = st.top(); st.pop();
            int a = st.top(); st.pop();
            if (t == "+") st.push(a + b);
            else if (t == "-") st.push(a - b);
            else if (t == "*") st.push(a * b);
            else if (t == "/") st.push(a / b);
        } else {
            st.push(stoi(t));
        }
    }
    return st.top();
}

// [CORE] Decode String
// Idea: Use two stacks (counts and strings). Push previous string and repeat count upon '[', pop and build on ']'.
// Time: O(output length)
// Space: O(n)
string decodeString(const string& s) {
    stack<int> countStack;
    stack<string> stringStack;
    string currentStr = "";
    int currentK = 0;

    for (char c : s) {
        if (isdigit(c)) {
            currentK = currentK * 10 + (c - '0');
        } else if (c == '[') {
            countStack.push(currentK);
            stringStack.push(currentStr);
            currentStr = "";
            currentK = 0;
        } else if (c == ']') {
            string decoded = stringStack.top(); stringStack.pop();
            int k = countStack.top(); countStack.pop();
            while (k--) decoded += currentStr;
            currentStr = decoded;
        } else {
            currentStr += c;
        }
    }
    return currentStr;
}

// [HIGH] Next Greater Element I
// Idea: Monotonic decreasing stack on nums2 to map each element to its next greater element in a hash map.
// Time: O(n + m)
// Space: O(n)
vector<int> nextGreaterElement(const vector<int>& nums1, const vector<int>& nums2) {
    unordered_map<int, int> ngeMap;
    stack<int> st;
    for (int x : nums2) {
        while (!st.empty() && st.top() < x) {
            ngeMap[st.top()] = x;
            st.pop();
        }
        st.push(x);
    }
    vector<int> res;
    for (int x : nums1) {
        res.push_back(ngeMap.count(x) ? ngeMap[x] : -1);
    }
    return res;
}

// [HIGH] Next Greater Element II (Circular Array)
// Idea: Traverse 2 * n indices virtually using modulo arithmetic to resolve circular next greater elements.
// Time: O(n)
// Space: O(n)
vector<int> nextGreaterElementsCircular(const vector<int>& nums) {
    int n = nums.size();
    vector<int> res(n, -1);
    stack<int> st; // Stores indices

    for (int i = 0; i < 2 * n; ++i) {
        while (!st.empty() && nums[st.top()] < nums[i % n]) {
            res[st.top()] = nums[i % n];
            st.pop();
        }
        if (i < n) st.push(i);
    }
    return res;
}

// [CORE] Daily Temperatures
// Idea: Monotonic decreasing stack storing indices. Pop when current day temperature is warmer to calculate day delta.
// Time: O(n)
// Space: O(n)
vector<int> dailyTemperatures(const vector<int>& temperatures) {
    int n = temperatures.size();
    vector<int> res(n, 0);
    stack<int> st; // Monotonic decreasing stack of indices

    for (int i = 0; i < n; ++i) {
        while (!st.empty() && temperatures[i] > temperatures[st.top()]) {
            int prevIdx = st.top();
            st.pop();
            res[prevIdx] = i - prevIdx;
        }
        st.push(i);
    }
    return res;
}

// [CORE] Largest Rectangle in Histogram
// Idea: Monotonic increasing stack. Pop when bar is smaller, height is popped element, width is between stack top and i.
// Time: O(n)
// Space: O(n)
int largestRectangleArea(vector<int>& heights) {
    heights.push_back(0); // Sentinel to flush remaining stack
    stack<int> st;
    int maxArea = 0;

    for (int i = 0; i < static_cast<int>(heights.size()); ++i) {
        while (!st.empty() && heights[st.top()] >= heights[i]) {
            int h = heights[st.top()];
            st.pop();
            int w = st.empty() ? i : (i - st.top() - 1);
            maxArea = max(maxArea, h * w);
        }
        st.push(i);
    }
    heights.pop_back();
    return maxArea;
}

// [HIGH] Maximal Rectangle (2D Binary Matrix)
// Idea: Treat each row as a histogram base and solve Largest Rectangle in Histogram for each cumulative row.
// Time: O(r * c)
// Space: O(c)
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

// [HIGH] Basic Calculator II
// Idea: Stack or running operand tracker for handling operator precedence (* and / before + and -).
// Time: O(n)
// Space: O(n)
int calculateBasicCalculatorII(const string& s) {
    stack<int> st;
    long long currNum = 0;
    char op = '+';

    for (int i = 0; i < static_cast<int>(s.size()); ++i) {
        char c = s[i];
        if (isdigit(c)) currNum = currNum * 10 + (c - '0');
        if ((!isdigit(c) && !isspace(c)) || i == static_cast<int>(s.size()) - 1) {
            if (op == '+') st.push(currNum);
            else if (op == '-') st.push(-currNum);
            else if (op == '*') {
                int top = st.top(); st.pop();
                st.push(top * currNum);
            } else if (op == '/') {
                int top = st.top(); st.pop();
                st.push(top / currNum);
            }
            op = c;
            currNum = 0;
        }
    }
    int result = 0;
    while (!st.empty()) {
        result += st.top();
        st.pop();
    }
    return result;
}


// ============================================================================
// 06. GREEDY
// ============================================================================

// [CORE] Jump Game
// Idea: Track the maximum reachable index. If current index exceeds max reachable index, return false.
// Time: O(n)
// Space: O(1)
bool canJump(const vector<int>& nums) {
    int maxReach = 0;
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        if (i > maxReach) return false;
        maxReach = max(maxReach, i + nums[i]);
    }
    return true;
}

// [CORE] Jump Game II
// Idea: Greedy BFS range expansion. When loop reaches the end of current jump boundary, increment jump and extend boundary.
// Time: O(n)
// Space: O(1)
int jumpGameII(const vector<int>& nums) {
    int jumps = 0, currentEnd = 0, farthest = 0;
    for (int i = 0; i < static_cast<int>(nums.size()) - 1; ++i) {
        farthest = max(farthest, i + nums[i]);
        if (i == currentEnd) {
            jumps++;
            currentEnd = farthest;
        }
    }
    return jumps;
}

// [CORE] Gas Station
// Idea: If total gas >= total cost, a solution exists. If running tank drops below 0, reset start to next station.
// Time: O(n)
// Space: O(1)
int canCompleteCircuit(const vector<int>& gas, const vector<int>& cost) {
    int totalTank = 0, currTank = 0, startIdx = 0;
    for (int i = 0; i < static_cast<int>(gas.size()); ++i) {
        int diff = gas[i] - cost[i];
        totalTank += diff;
        currTank += diff;
        if (currTank < 0) {
            startIdx = i + 1;
            currTank = 0;
        }
    }
    return (totalTank >= 0) ? startIdx : -1;
}

// [HIGH] Partition Labels
// Idea: Record last occurrence of each character. Extend partition endpoint until reaching max last occurrence in window.
// Time: O(n)
// Space: O(1)
vector<int> partitionLabels(const string& s) {
    vector<int> lastIdx(26, 0);
    for (int i = 0; i < static_cast<int>(s.size()); ++i) {
        lastIdx[s[i] - 'a'] = i;
    }
    vector<int> partitions;
    int start = 0, end = 0;
    for (int i = 0; i < static_cast<int>(s.size()); ++i) {
        end = max(end, lastIdx[s[i] - 'a']);
        if (i == end) {
            partitions.push_back(end - start + 1);
            start = i + 1;
        }
    }
    return partitions;
}

// [HIGH] Non-overlapping Intervals
// Idea: Sort intervals by end time. Greedily pick intervals that end earliest to leave maximum room for future intervals.
// Time: O(n log n)
// Space: O(1)
int eraseOverlapIntervals(vector<vector<int>>& intervals) {
    if (intervals.empty()) return 0;
    sort(intervals.begin(), intervals.end(), [](const auto& a, const auto& b) {
        return a[1] < b[1];
    });
    int count = 0, prevEnd = intervals[0][1];
    for (size_t i = 1; i < intervals.size(); ++i) {
        if (intervals[i][0] < prevEnd) {
            count++; // Overlap detected, remove current interval
        } else {
            prevEnd = intervals[i][1];
        }
    }
    return count;
}

// [HIGH] Minimum Number of Arrows to Burst Balloons
// Idea: Sort by end coordinate. Greedily shoot arrow at current end coordinate, popping all overlapping balloons.
// Time: O(n log n)
// Space: O(1)
int findMinArrowShots(vector<vector<int>>& points) {
    if (points.empty()) return 0;
    sort(points.begin(), points.end(), [](const auto& a, const auto& b) {
        return a[1] < b[1];
    });
    int arrows = 1, arrowPos = points[0][1];
    for (size_t i = 1; i < points.size(); ++i) {
        if (points[i][0] > arrowPos) {
            arrows++;
            arrowPos = points[i][1];
        }
    }
    return arrows;
}

// [CORE] Task Scheduler
// Idea: Calculate idle slots dictated by the most frequent task. Fill idle slots with remaining tasks.
// Time: O(n)
// Space: O(1)
int leastInterval(const vector<char>& tasks, int n) {
    vector<int> freq(26, 0);
    for (char c : tasks) freq[c - 'A']++;
    int maxFreq = *max_element(freq.begin(), freq.end());
    int maxCount = count(freq.begin(), freq.end(), maxFreq);

    int emptySlots = (maxFreq - 1) * (n - (maxCount - 1));
    int availableTasks = tasks.size() - maxFreq * maxCount;
    int idles = max(0, emptySlots - availableTasks);
    return tasks.size() + idles;
}

// [HIGH] Hand of Straights
// Idea: Use an ordered map of frequencies. Greedily start a group of size groupSize from the smallest available card.
// Time: O(n log n)
// Space: O(n)
bool isNStraightHand(const vector<int>& hand, int groupSize) {
    if (hand.size() % groupSize != 0) return false;
    map<int, int> count;
    for (int card : hand) count[card]++;

    for (auto& [card, freq] : count) {
        if (freq > 0) {
            int needed = freq;
            for (int i = 0; i < groupSize; ++i) {
                if (count[card + i] < needed) return false;
                count[card + i] -= needed;
            }
        }
    }
    return true;
}

// [MEDIUM] Merge Triplets to Form Target
// Idea: Filter out any triplet with any element exceeding target. Check if the remaining valid triplets cover all target dimensions.
// Time: O(n)
// Space: O(1)
bool mergeTriplets(const vector<vector<int>>& triplets, const vector<int>& target) {
    vector<bool> matched(3, false);
    for (const auto& t : triplets) {
        if (t[0] > target[0] || t[1] > target[1] || t[2] > target[2]) continue;
        for (int i = 0; i < 3; ++i) {
            if (t[i] == target[i]) matched[i] = true;
        }
    }
    return matched[0] && matched[1] && matched[2];
}


// ============================================================================
// 07. BINARY SEARCH
// ============================================================================

// [CORE] Binary Search (Standard Template)
// Idea: Standard low <= high template preventing overflow with mid = low + (high - low) / 2.
// Time: O(log n)
// Space: O(1)
int binarySearch(const vector<int>& nums, int target) {
    int low = 0, high = static_cast<int>(nums.size()) - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (nums[mid] == target) return mid;
        if (nums[mid] < target) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

// [CORE] Find First and Last Position of Element in Sorted Array
// Idea: Perform two binary searches: one biasing left on equality, one biasing right on equality.
// Time: O(log n)
// Space: O(1)
vector<int> searchRange(const vector<int>& nums, int target) {
    auto findBound = [&](bool isFirst) {
        int low = 0, high = static_cast<int>(nums.size()) - 1, ans = -1;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (nums[mid] == target) {
                ans = mid;
                if (isFirst) high = mid - 1;
                else low = mid + 1;
            } else if (nums[mid] < target) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        return ans;
    };
    return {findBound(true), findBound(false)};
}

// [CORE] Search in Rotated Sorted Array
// Idea: At least one half [low..mid] or [mid..high] is always sorted. Check if target lies within sorted half.
// Time: O(log n)
// Space: O(1)
int searchRotatedSortedArray(const vector<int>& nums, int target) {
    int low = 0, high = static_cast<int>(nums.size()) - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (nums[mid] == target) return mid;

        if (nums[low] <= nums[mid]) { // Left half sorted
            if (nums[low] <= target && target < nums[mid]) high = mid - 1;
            else low = mid + 1;
        } else { // Right half sorted
            if (nums[mid] < target && target <= nums[high]) low = mid + 1;
            else high = mid - 1;
        }
    }
    return -1;
}

// [HIGH] Search in Rotated Sorted Array II (With Duplicates)
// Idea: When nums[low] == nums[mid] == nums[high], shrink bounds to resolve ambiguity.
// Time: Average O(log n), Worst O(n)
// Space: O(1)
bool searchRotatedWithDuplicates(const vector<int>& nums, int target) {
    int low = 0, high = static_cast<int>(nums.size()) - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (nums[mid] == target) return true;

        if (nums[low] == nums[mid] && nums[mid] == nums[high]) {
            low++; high--;
        } else if (nums[low] <= nums[mid]) {
            if (nums[low] <= target && target < nums[mid]) high = mid - 1;
            else low = mid + 1;
        } else {
            if (nums[mid] < target && target <= nums[high]) low = mid + 1;
            else high = mid - 1;
        }
    }
    return false;
}

// [CORE] Find Minimum in Rotated Sorted Array
// Idea: Compare mid with high. If nums[mid] > nums[high], min is in right half; else in left half.
// Time: O(log n)
// Space: O(1)
int findMinRotatedSortedArray(const vector<int>& nums) {
    int low = 0, high = static_cast<int>(nums.size()) - 1;
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (nums[mid] > nums[high]) low = mid + 1;
        else high = mid;
    }
    return nums[low];
}

// [CORE] Koko Eating Bananas
// Idea: Binary search on eating speed k in range [1..max(piles)]. Monotonic predicate checks total hours.
// Time: O(n log(max_pile))
// Space: O(1)
int minEatingSpeed(const vector<int>& piles, int h) {
    int low = 1, high = *max_element(piles.begin(), piles.end());
    int ans = high;

    while (low <= high) {
        int speed = low + (high - low) / 2;
        long long totalHours = 0;
        for (int pile : piles) {
            totalHours += (pile + speed - 1) / speed;
        }
        if (totalHours <= h) {
            ans = speed;
            high = speed - 1; // Try smaller speed
        } else {
            low = speed + 1;
        }
    }
    return ans;
}

// [HIGH] Capacity to Ship Packages Within D Days
// Idea: Binary search on capacity in range [max(weights)..sum(weights)]. Verify day count greedily.
// Time: O(n log(sum_weights))
// Space: O(1)
int shipWithinDays(const vector<int>& weights, int days) {
    int low = *max_element(weights.begin(), weights.end());
    int high = accumulate(weights.begin(), weights.end(), 0);
    int ans = high;

    while (low <= high) {
        int cap = low + (high - low) / 2;
        int neededDays = 1, currentLoad = 0;
        for (int w : weights) {
            if (currentLoad + w > cap) {
                neededDays++;
                currentLoad = 0;
            }
            currentLoad += w;
        }
        if (neededDays <= days) {
            ans = cap;
            high = cap - 1;
        } else {
            low = cap + 1;
        }
    }
    return ans;
}

// [HIGH] Split Array Largest Sum
// Idea: Binary search on maximum subarray sum in range [max(nums)..sum(nums)]. Check split count.
// Time: O(n log(sum_nums))
// Space: O(1)
int splitArrayLargestSum(const vector<int>& nums, int k) {
    long long low = *max_element(nums.begin(), nums.end());
    long long high = accumulate(nums.begin(), nums.end(), 0LL);
    long long ans = high;

    while (low <= high) {
        long long mid = low + (high - low) / 2;
        int splits = 1;
        long long currentSum = 0;
        for (int x : nums) {
            if (currentSum + x > mid) {
                splits++;
                currentSum = 0;
            }
            currentSum += x;
        }
        if (splits <= k) {
            ans = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return static_cast<int>(ans);
}

// [ADVANCED] Median of Two Sorted Arrays
// Idea: Binary search on partition of the smaller array so left partition has (m + n + 1) / 2 elements.
// Time: O(log(min(m, n)))
// Space: O(1)
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

// [HIGH] Time Based Key-Value Store
// Idea: Hash map maps key to vector of {timestamp, value} pairs. Binary search (upper_bound - 1) on timestamp.
// Time: Set O(1), Get O(log n)
// Space: O(n)
class TimeMap {
    unordered_map<string, vector<pair<int, string>>> store;
public:
    TimeMap() {}
    void set(string key, string value, int timestamp) {
        store[key].push_back({timestamp, value});
    }
    string get(string key, int timestamp) {
        if (!store.count(key)) return "";
        const auto& entries = store[key];
        int low = 0, high = static_cast<int>(entries.size()) - 1;
        string ans = "";
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (entries[mid].first <= timestamp) {
                ans = entries[mid].second;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        return ans;
    }
};

// ============================================================================
// 09. TREES & BST
// ============================================================================

// [CORE] Tree Traversals: Preorder, Inorder, Postorder (Iterative)
// Idea: Preorder: root->left->right (Stack). Inorder: left->root->right (Stack going left). Postorder: left->right->root (Two stacks or Reverse Preorder).
// Time: O(n)
// Space: O(h)
vector<int> inorderTraversal(TreeNode* root) {
    vector<int> res;
    stack<TreeNode*> st;
    TreeNode* curr = root;
    while (curr || !st.empty()) {
        while (curr) {
            st.push(curr);
            curr = curr->left;
        }
        curr = st.top(); st.pop();
        res.push_back(curr->val);
        curr = curr->right;
    }
    return res;
}

// [CORE] Level Order Traversal
// Idea: Queue BFS tracking level size to process nodes layer by layer.
// Time: O(n)
// Space: O(w) where w is maximum tree width
vector<vector<int>> levelOrder(TreeNode* root) {
    if (!root) return {};
    vector<vector<int>> res;
    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int levelSize = q.size();
        vector<int> currentLevel;
        for (int i = 0; i < levelSize; ++i) {
            TreeNode* node = q.front(); q.pop();
            currentLevel.push_back(node->val);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        res.push_back(std::move(currentLevel));
    }
    return res;
}

// [HIGH] Zigzag Level Order Traversal
// Idea: Queue BFS with a boolean flag indicating whether current level array should be populated left-to-right or right-to-left.
// Time: O(n)
// Space: O(w)
vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
    if (!root) return {};
    vector<vector<int>> res;
    queue<TreeNode*> q;
    q.push(root);
    bool leftToRight = true;

    while (!q.empty()) {
        int sz = q.size();
        vector<int> level(sz);
        for (int i = 0; i < sz; ++i) {
            TreeNode* node = q.front(); q.pop();
            int idx = leftToRight ? i : (sz - 1 - i);
            level[idx] = node->val;
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        res.push_back(std::move(level));
        leftToRight = !leftToRight;
    }
    return res;
}

// [HIGH] Right Side View & Left Side View
// Idea: Level-order BFS adding last element of each layer (Right View) or first element of each layer (Left View).
// Time: O(n)
// Space: O(w)
vector<int> rightSideView(TreeNode* root) {
    if (!root) return {};
    vector<int> view;
    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int sz = q.size();
        for (int i = 0; i < sz; ++i) {
            TreeNode* node = q.front(); q.pop();
            if (i == sz - 1) view.push_back(node->val);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
    }
    return view;
}

// [HIGH] Vertical Order Traversal
// Idea: Queue BFS carrying column coordinates {node, col}. Group by column in a sorted map.
// Time: O(n log n)
// Space: O(n)
vector<vector<int>> verticalOrder(TreeNode* root) {
    if (!root) return {};
    map<int, vector<int>> colMap;
    queue<pair<TreeNode*, int>> q;
    q.push({root, 0});

    while (!q.empty()) {
        auto [node, col] = q.front(); q.pop();
        colMap[col].push_back(node->val);
        if (node->left) q.push({node->left, col - 1});
        if (node->right) q.push({node->right, col + 1});
    }

    vector<vector<int>> res;
    for (auto& [col, vals] : colMap) res.push_back(vals);
    return res;
}

// [CORE] Diameter of Binary Tree
// Idea: Postorder DFS computes max depth of left and right subtrees. Diameter through current node is leftDepth + rightDepth.
// Time: O(n)
// Space: O(h)
int diameterOfBinaryTree(TreeNode* root) {
    int maxDiameter = 0;
    function<int(TreeNode*)> depth = [&](TreeNode* node) {
        if (!node) return 0;
        int leftH = depth(node->left);
        int rightH = depth(node->right);
        maxDiameter = max(maxDiameter, leftH + rightH);
        return 1 + max(leftH, rightH);
    };
    depth(root);
    return maxDiameter;
}

// [CORE] Maximum Path Sum
// Idea: Postorder DFS returns max single-branch gain (ignoring negative paths). Updates global max with (leftGain + rightGain + node->val).
// Time: O(n)
// Space: O(h)
int maxPathSum(TreeNode* root) {
    int maxSum = INT_MIN;
    function<int(TreeNode*)> maxGain = [&](TreeNode* node) {
        if (!node) return 0;
        int leftGain = max(0, maxGain(node->left));
        int rightGain = max(0, maxGain(node->right));
        maxSum = max(maxSum, node->val + leftGain + rightGain);
        return node->val + max(leftGain, rightGain);
    };
    maxGain(root);
    return maxSum;
}

// [CORE] Lowest Common Ancestor (Binary Tree & BST & Parent Pointer)
// Idea BT: Recurse left and right. If p and q found in separate subtrees, root is LCA.
// Time: O(n)
// Space: O(h)
TreeNode* lowestCommonAncestorBT(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (!root || root == p || root == q) return root;
    TreeNode* left = lowestCommonAncestorBT(root->left, p, q);
    TreeNode* right = lowestCommonAncestorBT(root->right, p, q);
    if (left && right) return root;
    return left ? left : right;
}

// LCA in BST: Exploit BST invariant. If both p and q are smaller, go left; if both larger, go right; else split point is LCA.
TreeNode* lowestCommonAncestorBST(TreeNode* root, TreeNode* p, TreeNode* q) {
    while (root) {
        if (p->val < root->val && q->val < root->val) root = root->left;
        else if (p->val > root->val && q->val > root->val) root = root->right;
        else return root;
    }
    return nullptr;
}

// LCA with Parent Pointer: Intersect paths like linked list intersection.
NodeWithParent* lowestCommonAncestorWithParent(NodeWithParent* p, NodeWithParent* q) {
    NodeWithParent *a = p, *b = q;
    while (a != b) {
        a = a->parent ? a->parent : q;
        b = b->parent ? b->parent : p;
    }
    return a;
}

// [CORE] Validate Binary Search Tree
// Idea: DFS enforcing valid range (minVal < node->val < maxVal) using 64-bit integer limits.
// Time: O(n)
// Space: O(h)
bool isValidBST(TreeNode* root, long long minVal = LLONG_MIN, long long maxVal = LLONG_MAX) {
    if (!root) return true;
    if (root->val <= minVal || root->val >= maxVal) return false;
    return isValidBST(root->left, minVal, root->val) && isValidBST(root->right, root->val, maxVal);
}

// [CORE] Balanced Binary Tree
// Idea: Postorder DFS returning tree height, or -1 as sentinel if any subtree is unbalanced (|leftH - rightH| > 1).
// Time: O(n)
// Space: O(h)
int checkHeight(TreeNode* root) {
    if (!root) return 0;
    int leftH = checkHeight(root->left);
    if (leftH == -1) return -1;
    int rightH = checkHeight(root->right);
    if (rightH == -1) return -1;
    if (abs(leftH - rightH) > 1) return -1;
    return 1 + max(leftH, rightH);
}

bool isBalanced(TreeNode* root) {
    return checkHeight(root) != -1;
}

// [HIGH] Path Sum I & II
// Idea Path Sum II: Backtracking DFS maintaining current path and remaining target sum. Push path upon reaching target at leaf.
// Time: O(n)
// Space: O(h)
void dfsPathSumII(TreeNode* node, int targetSum, vector<int>& currPath, vector<vector<int>>& res) {
    if (!node) return;
    currPath.push_back(node->val);
    if (!node->left && !node->right && targetSum == node->val) {
        res.push_back(currPath);
    } else {
        dfsPathSumII(node->left, targetSum - node->val, currPath, res);
        dfsPathSumII(node->right, targetSum - node->val, currPath, res);
    }
    currPath.pop_back();
}

vector<vector<int>> pathSumII(TreeNode* root, int targetSum) {
    vector<vector<int>> res;
    vector<int> currPath;
    dfsPathSumII(root, targetSum, currPath, res);
    return res;
}

// [HIGH] Binary Tree Paths
// Idea: DFS accumulating root-to-leaf paths as formatted strings "root->node->leaf".
// Time: O(n)
// Space: O(h)
void dfsTreePaths(TreeNode* node, string path, vector<string>& res) {
    if (!node) return;
    path += to_string(node->val);
    if (!node->left && !node->right) {
        res.push_back(path);
        return;
    }
    path += "->";
    if (node->left) dfsTreePaths(node->left, path, res);
    if (node->right) dfsTreePaths(node->right, path, res);
}

vector<string> binaryTreePaths(TreeNode* root) {
    vector<string> res;
    if (root) dfsTreePaths(root, "", res);
    return res;
}

// [HIGH] Kth Smallest Element in BST
// Idea: Inorder traversal visits BST elements in sorted ascending order. Decrement k and return on k == 0.
// Time: O(h + k)
// Space: O(h)
int kthSmallest(TreeNode* root, int k) {
    stack<TreeNode*> st;
    TreeNode* curr = root;
    while (curr || !st.empty()) {
        while (curr) {
            st.push(curr);
            curr = curr->left;
        }
        curr = st.top(); st.pop();
        if (--k == 0) return curr->val;
        curr = curr->right;
    }
    return -1;
}

// [HIGH] Binary Search Tree Iterator
// Idea: Controlled inorder traversal using stack. Push all left descendants to simulate next smallest element on demand.
// Time: next() O(1) amortized, hasNext() O(1)
// Space: O(h)
class BSTIterator {
    stack<TreeNode*> st;
    void pushAllLeft(TreeNode* node) {
        while (node) {
            st.push(node);
            node = node->left;
        }
    }
public:
    BSTIterator(TreeNode* root) { pushAllLeft(root); }
    int next() {
        TreeNode* topNode = st.top(); st.pop();
        pushAllLeft(topNode->right);
        return topNode->val;
    }
    bool hasNext() { return !st.empty(); }
};

// [CORE] Serialize and Deserialize Binary Tree
// Idea: Preorder DFS string serialization with '#' for nulls and commas as delimiters.
// Time: O(n)
// Space: O(n)
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

// [CORE] Construct Binary Tree from Preorder & Inorder Traversal
// Idea: First preorder element is root. Find root in inorder using hash map to determine left/right subtree sizes.
// Time: O(n)
// Space: O(n)
TreeNode* buildTreeHelper(const vector<int>& preorder, int preStart, int preEnd,
                          int inStart, int inEnd, unordered_map<int, int>& inMap) {
    if (preStart > preEnd || inStart > inEnd) return nullptr;
    int rootVal = preorder[preStart];
    TreeNode* root = new TreeNode(rootVal);
    int inRootIdx = inMap[rootVal];
    int leftTreeSize = inRootIdx - inStart;

    root->left = buildTreeHelper(preorder, preStart + 1, preStart + leftTreeSize, inStart, inRootIdx - 1, inMap);
    root->right = buildTreeHelper(preorder, preStart + leftTreeSize + 1, preEnd, inRootIdx + 1, inEnd, inMap);
    return root;
}

TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
    unordered_map<int, int> inMap;
    for (int i = 0; i < static_cast<int>(inorder.size()); ++i) inMap[inorder[i]] = i;
    return buildTreeHelper(preorder, 0, preorder.size() - 1, 0, inorder.size() - 1, inMap);
}

// [ADVANCED] Morris Inorder Traversal (O(1) Auxiliary Space)
// Idea: Threaded binary tree. Use rightmost node of left subtree to point back to current node before visiting left.
// Time: O(n)
// Space: O(1) auxiliary
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
// 10. TRIE
// ============================================================================

// [CORE] Implement Trie (Prefix Tree)
// Idea: N-ary tree where each node has 26 child pointers representing alphabet transitions, and an isEnd flag.
// Time: Insert O(L), Search O(L), StartsWith O(L) where L is word length
// Space: O(Total characters across words)
class Trie {
    struct TrieNode {
        TrieNode* children[26] = {};
        bool isEnd = false;
    };
    TrieNode* root;
public:
    Trie() { root = new TrieNode(); }

    void insert(const string& word) {
        TrieNode* curr = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!curr->children[idx]) curr->children[idx] = new TrieNode();
            curr = curr->children[idx];
        }
        curr->isEnd = true;
    }

    bool search(const string& word) {
        TrieNode* curr = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!curr->children[idx]) return false;
            curr = curr->children[idx];
        }
        return curr->isEnd;
    }

    bool startsWith(const string& prefix) {
        TrieNode* curr = root;
        for (char c : prefix) {
            int idx = c - 'a';
            if (!curr->children[idx]) return false;
            curr = curr->children[idx];
        }
        return true;
    }
};

// [HIGH] Design Add and Search Words Data Structure (Wildcard '.')
// Idea: Trie with recursive DFS search. When '.' wildcard is encountered, branch into all 26 non-null child nodes.
// Time: Add O(L), Search with '.' O(26^dots * L)
// Space: O(Total characters)
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

    void addWord(const string& word) {
        TrieNode* curr = root;
        for (char c : word) {
            int i = c - 'a';
            if (!curr->children[i]) curr->children[i] = new TrieNode();
            curr = curr->children[i];
        }
        curr->isEnd = true;
    }

    bool search(const string& word) {
        return searchInNode(word, 0, root);
    }
};

// [HIGH] Word Search II (Trie + 2D Backtracking)
// Idea: Insert word dictionary into Trie. Run DFS from each cell in grid matching Trie nodes; store found word and clear word flag.
// Time: O(R * C * 4^L) where L is max word length
// Space: O(Total characters in words)
class WordSearchII {
    struct TrieNode {
        TrieNode* children[26] = {};
        string word = "";
    };

    void insertWord(TrieNode* root, const string& word) {
        TrieNode* curr = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!curr->children[idx]) curr->children[idx] = new TrieNode();
            curr = curr->children[idx];
        }
        curr->word = word;
    }

    void dfs(vector<vector<char>>& board, int r, int c, TrieNode* node, vector<string>& res) {
        char ch = board[r][c];
        if (ch == '#' || !node->children[ch - 'a']) return;
        node = node->children[ch - 'a'];

        if (!node->word.empty()) {
            res.push_back(node->word);
            node->word = ""; // Prevent duplicate matches
        }

        board[r][c] = '#'; // Mark visited
        int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1};
        for (int d = 0; d < 4; ++d) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr >= 0 && nr < static_cast<int>(board.size()) &&
                nc >= 0 && nc < static_cast<int>(board[0].size())) {
                dfs(board, nr, nc, node, res);
            }
        }
        board[r][c] = ch; // Backtrack
    }

public:
    vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
        TrieNode* root = new TrieNode();
        for (const string& w : words) insertWord(root, w);
        vector<string> res;
        for (int r = 0; r < static_cast<int>(board.size()); ++r) {
            for (int c = 0; c < static_cast<int>(board[0].size()); ++c) {
                dfs(board, r, c, root, res);
            }
        }
        return res;
    }
};


// ============================================================================
// 11. HEAPS & PRIORITY QUEUES
// ============================================================================

// [CORE] Kth Largest Element in an Array (QuickSelect & Min-Heap)
// Idea QuickSelect: Partition array around pivot. Average O(n) selection of (n - k)-th index.
// Time: Average O(n), Worst O(n^2)
// Space: O(1) auxiliary
int findKthLargestQuickSelect(vector<int>& nums, int k) {
    int targetIdx = nums.size() - k;
    int low = 0, high = nums.size() - 1;

    while (low <= high) {
        int pivot = nums[high], pIdx = low;
        for (int i = low; i < high; ++i) {
            if (nums[i] <= pivot) swap(nums[i], nums[pIdx++]);
        }
        swap(nums[pIdx], nums[high]);

        if (pIdx == targetIdx) return nums[pIdx];
        if (pIdx < targetIdx) low = pIdx + 1;
        else high = pIdx - 1;
    }
    return -1;
}

// [CORE] Top K Frequent Elements
// Idea: Frequency hash map + bucket sort where bucket index represents occurrence frequency.
// Time: O(n)
// Space: O(n)
vector<int> topKFrequent(const vector<int>& nums, int k) {
    unordered_map<int, int> count;
    for (int x : nums) count[x]++;

    vector<vector<int>> buckets(nums.size() + 1);
    for (auto& [val, freq] : count) {
        buckets[freq].push_back(val);
    }

    vector<int> res;
    for (int i = nums.size(); i >= 0 && static_cast<int>(res.size()) < k; --i) {
        for (int x : buckets[i]) {
            res.push_back(x);
            if (static_cast<int>(res.size()) == k) break;
        }
    }
    return res;
}

// [HIGH] Top K Frequent Words
// Idea: Frequency hash map + min-heap of size k with custom comparator (higher freq preferred, smaller lex preferred).
// Time: O(n log k)
// Space: O(n)
vector<string> topKFrequentWords(const vector<string>& words, int k) {
    unordered_map<string, int> freq;
    for (const string& w : words) freq[w]++;

    auto comp = [](const pair<int, string>& a, const pair<int, string>& b) {
        if (a.first != b.first) return a.first > b.first; // Lower freq has higher priority to be popped
        return a.second < b.second; // Larger lex has higher priority to be popped
    };
    priority_queue<pair<int, string>, vector<pair<int, string>>, decltype(comp)> pq(comp);

    for (auto& [w, count] : freq) {
        pq.push({count, w});
        if (static_cast<int>(pq.size()) > k) pq.pop();
    }

    vector<string> res(k);
    for (int i = k - 1; i >= 0; --i) {
        res[i] = pq.top().second;
        pq.pop();
    }
    return res;
}

// [HIGH] K Closest Points to Origin
// Idea: Maintain a max-heap of size k storing {distance, point}. Pop whenever heap size exceeds k.
// Time: O(n log k)
// Space: O(k)
vector<vector<int>> kClosest(const vector<vector<int>>& points, int k) {
    priority_queue<pair<int, pair<int, int>>> maxHeap;
    for (const auto& p : points) {
        int distSq = p[0] * p[0] + p[1] * p[1];
        maxHeap.push({distSq, {p[0], p[1]}});
        if (static_cast<int>(maxHeap.size()) > k) maxHeap.pop();
    }
    vector<vector<int>> res;
    while (!maxHeap.empty()) {
        res.push_back({maxHeap.top().second.first, maxHeap.top().second.second});
        maxHeap.pop();
    }
    return res;
}

// [CORE] Find Median from Data Stream (Two Heaps)
// Idea: Max-heap stores smaller half, min-heap stores larger half. Balance heaps so size diff is at most 1.
// Time: addNum O(log n), findMedian O(1)
// Space: O(n)
class MedianFinder {
    priority_queue<int> small; // Max-heap
    priority_queue<int, vector<int>, greater<int>> large; // Min-heap
public:
    MedianFinder() {}

    void addNum(int num) {
        if (small.empty() || num <= small.top()) small.push(num);
        else large.push(num);

        // Balance
        if (small.size() > large.size() + 1) {
            large.push(small.top()); small.pop();
        } else if (large.size() > small.size()) {
            small.push(large.top()); large.pop();
        }
    }

    double findMedian() {
        if (small.size() > large.size()) return small.top();
        return (small.top() + large.top()) / 2.0;
    }
};

// [ADVANCED] Sliding Window Median (Two Balanced Multisets)
// Idea: Balance two multisets (low and high) to support log(k) insertions and deletions in a rolling window of size k.
// Time: O(n log k)
// Space: O(k)
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

// [HIGH] Reorganize String
// Idea: Max-heap of character frequencies. Greedily pair up the two most frequent characters to avoid adjacent duplicates.
// Time: O(n log(alphabet))
// Space: O(alphabet)
string reorganizeString(const string& s) {
    vector<int> freq(26, 0);
    for (char c : s) freq[c - 'a']++;

    priority_queue<pair<int, char>> pq;
    for (int i = 0; i < 26; ++i) {
        if (freq[i] > 0) {
            if (freq[i] > (static_cast<int>(s.size()) + 1) / 2) return "";
            pq.push({freq[i], 'a' + i});
        }
    }

    string res = "";
    while (pq.size() >= 2) {
        auto [count1, char1] = pq.top(); pq.pop();
        auto [count2, char2] = pq.top(); pq.pop();

        res += char1;
        res += char2;

        if (--count1 > 0) pq.push({count1, char1});
        if (--count2 > 0) pq.push({count2, char2});
    }

    if (!pq.empty()) res += pq.top().second;
    return res;
}


// ============================================================================
// 12. INTERVALS
// ============================================================================

// [CORE] Merge Intervals
// Idea: Sort intervals by start time. Iterate through: merge with previous interval if overlapping, else push new interval.
// Time: O(n log n)
// Space: O(n)
vector<vector<int>> mergeIntervals(vector<vector<int>>& intervals) {
    if (intervals.empty()) return {};
    sort(intervals.begin(), intervals.end());
    vector<vector<int>> merged;

    for (const auto& in : intervals) {
        if (merged.empty() || merged.back()[1] < in[0]) {
            merged.push_back(in);
        } else {
            merged.back()[1] = max(merged.back()[1], in[1]);
        }
    }
    return merged;
}

// [CORE] Insert Interval
// Idea: 3 phases: 1) Add all intervals ending before newInterval starts; 2) Merge all overlapping intervals; 3) Add remaining intervals.
// Time: O(n)
// Space: O(n)
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

// [CORE] Meeting Rooms I
// Idea: Sort meetings by start time. Check if any meeting starts before the previous meeting ends.
// Time: O(n log n)
// Space: O(1)
bool canAttendMeetings(vector<vector<int>>& intervals) {
    sort(intervals.begin(), intervals.end());
    for (size_t i = 1; i < intervals.size(); ++i) {
        if (intervals[i][0] < intervals[i - 1][1]) return false;
    }
    return true;
}

// [CORE] Meeting Rooms II
// Idea: Sort meetings by start time. Min-heap tracks ongoing meeting end times. If earliest ends before next starts, reuse room.
// Time: O(n log n)
// Space: O(n)
int minMeetingRooms(vector<vector<int>>& intervals) {
    if (intervals.empty()) return 0;
    sort(intervals.begin(), intervals.end());
    priority_queue<int, vector<int>, greater<int>> minEndTimes;

    for (const auto& in : intervals) {
        if (!minEndTimes.empty() && minEndTimes.top() <= in[0]) {
            minEndTimes.pop(); // Room reused
        }
        minEndTimes.push(in[1]);
    }
    return minEndTimes.size();
}

// [HIGH] Employee Free Time
// Idea: Flatten all employee intervals, sort by start time, and collect gaps between consecutive merged intervals.
// Time: O(n log n) where n is total intervals
// Space: O(n)
vector<vector<int>> employeeFreeTime(vector<vector<vector<int>>>& schedule) {
    vector<vector<int>> allIntervals;
    for (const auto& emp : schedule) {
        for (const auto& in : emp) allIntervals.push_back(in);
    }
    sort(allIntervals.begin(), allIntervals.end());

    vector<vector<int>> freeTimes;
    int end = allIntervals[0][1];
    for (size_t i = 1; i < allIntervals.size(); ++i) {
        if (allIntervals[i][0] > end) {
            freeTimes.push_back({end, allIntervals[i][0]});
        }
        end = max(end, allIntervals[i][1]);
    }
    return freeTimes;
}


// ============================================================================
// 13. BACKTRACKING
// ============================================================================

// [CORE] Subsets
// Idea: Backtracking choices. At each index, decide whether to include current element or skip.
// Time: O(n * 2^n)
// Space: O(n) recursion stack
void backtrackSubsets(int start, const vector<int>& nums, vector<int>& curr, vector<vector<int>>& res) {
    res.push_back(curr);
    for (int i = start; i < static_cast<int>(nums.size()); ++i) {
        curr.push_back(nums[i]);
        backtrackSubsets(i + 1, nums, curr, res);
        curr.pop_back();
    }
}

vector<vector<int>> subsets(const vector<int>& nums) {
    vector<vector<int>> res;
    vector<int> curr;
    backtrackSubsets(0, nums, curr, res);
    return res;
}

// [CORE] Subsets II (With Duplicates)
// Idea: Sort input. Skip duplicate elements at the same tree depth with (i > start && nums[i] == nums[i - 1]).
// Time: O(n * 2^n)
// Space: O(n)
void backtrackSubsetsWithDup(int start, const vector<int>& nums, vector<int>& curr, vector<vector<int>>& res) {
    res.push_back(curr);
    for (int i = start; i < static_cast<int>(nums.size()); ++i) {
        if (i > start && nums[i] == nums[i - 1]) continue;
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

// [HIGH] Combinations (N choose K)
// Idea: Standard backtracking selecting k numbers from range [1..n].
// Time: O(C(n, k))
// Space: O(k)
void backtrackCombine(int start, int n, int k, vector<int>& curr, vector<vector<int>>& res) {
    if (static_cast<int>(curr.size()) == k) {
        res.push_back(curr);
        return;
    }
    for (int i = start; i <= n - (k - static_cast<int>(curr.size())) + 1; ++i) {
        curr.push_back(i);
        backtrackCombine(i + 1, n, k, curr, res);
        curr.pop_back();
    }
}

vector<vector<int>> combine(int n, int k) {
    vector<vector<int>> res;
    vector<int> curr;
    backtrackCombine(1, n, k, curr, res);
    return res;
}

// [CORE] Combination Sum
// Idea: Elements can be reused infinitely. Recurse with same index `i` until target reaches 0 or below.
// Time: O(2^target)
// Space: O(target)
void backtrackCombSum(int start, const vector<int>& candidates, int target, vector<int>& curr, vector<vector<int>>& res) {
    if (target == 0) {
        res.push_back(curr);
        return;
    }
    for (int i = start; i < static_cast<int>(candidates.size()); ++i) {
        if (candidates[i] > target) break;
        curr.push_back(candidates[i]);
        backtrackCombSum(i, candidates, target - candidates[i], curr, res); // Reuse index i
        curr.pop_back();
    }
}

vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
    sort(candidates.begin(), candidates.end());
    vector<vector<int>> res;
    vector<int> curr;
    backtrackCombSum(0, candidates, target, curr, res);
    return res;
}

// [HIGH] Combination Sum II
// Idea: Sort candidates. Each number may only be used once (recurse with i + 1) and skip duplicates at same level.
// Time: O(2^n)
// Space: O(n)
void backtrackCombSumII(int start, const vector<int>& candidates, int target, vector<int>& curr, vector<vector<int>>& res) {
    if (target == 0) {
        res.push_back(curr);
        return;
    }
    for (int i = start; i < static_cast<int>(candidates.size()); ++i) {
        if (candidates[i] > target) break;
        if (i > start && candidates[i] == candidates[i - 1]) continue;
        curr.push_back(candidates[i]);
        backtrackCombSumII(i + 1, candidates, target - candidates[i], curr, res);
        curr.pop_back();
    }
}

vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
    sort(candidates.begin(), candidates.end());
    vector<vector<int>> res;
    vector<int> curr;
    backtrackCombSumII(0, candidates, target, curr, res);
    return res;
}

// [CORE] Permutations
// Idea: In-place swap backtracking to generate all n! permutations without auxiliary hash set.
// Time: O(n * n!)
// Space: O(n)
void backtrackPermute(int start, vector<int>& nums, vector<vector<int>>& res) {
    if (start == static_cast<int>(nums.size())) {
        res.push_back(nums);
        return;
    }
    for (int i = start; i < static_cast<int>(nums.size()); ++i) {
        swap(nums[start], nums[i]);
        backtrackPermute(start + 1, nums, res);
        swap(nums[start], nums[i]);
    }
}

vector<vector<int>> permute(vector<int>& nums) {
    vector<vector<int>> res;
    backtrackPermute(0, nums, res);
    return res;
}

// [HIGH] Permutations II (With Duplicates)
// Idea: Sort array and track visited elements. Skip duplicates if previous identical element was NOT visited in current branch.
// Time: O(n * n!)
// Space: O(n)
void backtrackPermuteUnique(const vector<int>& nums, vector<bool>& visited, vector<int>& curr, vector<vector<int>>& res) {
    if (curr.size() == nums.size()) {
        res.push_back(curr);
        return;
    }
    for (size_t i = 0; i < nums.size(); ++i) {
        if (visited[i]) continue;
        if (i > 0 && nums[i] == nums[i - 1] && !visited[i - 1]) continue;
        visited[i] = true;
        curr.push_back(nums[i]);
        backtrackPermuteUnique(nums, visited, curr, res);
        curr.pop_back();
        visited[i] = false;
    }
}

vector<vector<int>> permuteUnique(vector<int>& nums) {
    sort(nums.begin(), nums.end());
    vector<vector<int>> res;
    vector<int> curr;
    vector<bool> visited(nums.size(), false);
    backtrackPermuteUnique(nums, visited, curr, res);
    return res;
}

// [CORE] Next Permutation
// Idea: 1) Find largest pivot i where nums[i] < nums[i + 1]; 2) Swap with smallest larger number to its right; 3) Reverse suffix.
// Time: O(n)
// Space: O(1)
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

// [HIGH] N-Queens
// Idea: Place queens row by row. Validate placement using boolean sets for columns and both diagonals (r - c, r + c).
// Time: O(n!)
// Space: O(n)
void backtrackNQueens(int row, int n, vector<string>& board, vector<bool>& cols,
                     vector<bool>& diag1, vector<bool>& diag2, vector<vector<string>>& res) {
    if (row == n) {
        res.push_back(board);
        return;
    }
    for (int col = 0; col < n; ++col) {
        if (cols[col] || diag1[row + col] || diag2[row - col + n - 1]) continue;
        board[row][col] = 'Q';
        cols[col] = diag1[row + col] = diag2[row - col + n - 1] = true;
        backtrackNQueens(row + 1, n, board, cols, diag1, diag2, res);
        board[row][col] = '.';
        cols[col] = diag1[row + col] = diag2[row - col + n - 1] = false;
    }
}

vector<vector<string>> solveNQueens(int n) {
    vector<vector<string>> res;
    vector<string> board(n, string(n, '.'));
    vector<bool> cols(n, false), diag1(2 * n, false), diag2(2 * n, false);
    backtrackNQueens(0, n, board, cols, diag1, diag2, res);
    return res;
}

// [HIGH] Sudoku Solver
// Idea: Search for empty cell '.', try valid digits '1'-'9', backtrack if unfeasible.
// Time: O(9^(empty_cells))
// Space: O(1) recursion stack (at most 81 frames)
bool isSudokuValidPlacement(const vector<vector<char>>& board, int r, int c, char ch) {
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
                    if (isSudokuValidPlacement(board, r, c, ch)) {
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

// [CORE] Word Search (2D Grid)
// Idea: DFS from matching start cell in 4 directions. Temporarily mark current cell '#' to prevent revisiting.
// Time: O(R * C * 3^L) where L is word length
// Space: O(L)
bool dfsWordSearch(vector<vector<char>>& board, int r, int c, const string& word, int idx) {
    if (idx == static_cast<int>(word.size())) return true;
    if (r < 0 || r >= static_cast<int>(board.size()) ||
        c < 0 || c >= static_cast<int>(board[0].size()) || board[r][c] != word[idx]) {
        return false;
    }
    char temp = board[r][c];
    board[r][c] = '#'; // Mark visited
    int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1};
    bool found = false;
    for (int d = 0; d < 4; ++d) {
        if (dfsWordSearch(board, r + dr[d], c + dc[d], word, idx + 1)) {
            found = true;
            break;
        }
    }
    board[r][c] = temp; // Backtrack
    return found;
}

bool existWordSearch(vector<vector<char>>& board, const string& word) {
    for (int r = 0; r < static_cast<int>(board.size()); ++r) {
        for (int c = 0; c < static_cast<int>(board[0].size()); ++c) {
            if (board[r][c] == word[0] && dfsWordSearch(board, r, c, word, 0)) {
                return true;
            }
        }
    }
    return false;
}

// [HIGH] Palindrome Partitioning
// Idea: Backtracking substring partitions. If substring s[start..i] is a palindrome, recurse on remaining suffix.
// Time: O(n * 2^n)
// Space: O(n)
bool isPalindromeStr(const string& s, int l, int r) {
    while (l < r) {
        if (s[l++] != s[r--]) return false;
    }
    return true;
}

void backtrackPalinPartition(int start, const string& s, vector<string>& curr, vector<vector<string>>& res) {
    if (start == static_cast<int>(s.size())) {
        res.push_back(curr);
        return;
    }
    for (int i = start; i < static_cast<int>(s.size()); ++i) {
        if (isPalindromeStr(s, start, i)) {
            curr.push_back(s.substr(start, i - start + 1));
            backtrackPalinPartition(i + 1, s, curr, res);
            curr.pop_back();
        }
    }
}

vector<vector<string>> partitionPalindrome(string s) {
    vector<vector<string>> res;
    vector<string> curr;
    backtrackPalinPartition(0, s, curr, res);
    return res;
}

// [HIGH] Restore IP Addresses
// Idea: Split string into 4 segments of length 1-3. Validate each segment (no leading zeros unless "0", value <= 255).
// Time: O(1) bounded (at most 3^4 checks)
// Space: O(1)
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
// 14. GRAPH BFS / DFS
// ============================================================================

// [CORE] Number of Islands
// Idea: Traverse grid. Upon finding '1', increment island count and sink the island to '0' using DFS/BFS.
// Time: O(r * c)
// Space: O(r * c)
void sinkIsland(vector<vector<char>>& grid, int r, int c) {
    if (r < 0 || r >= static_cast<int>(grid.size()) ||
        c < 0 || c >= static_cast<int>(grid[0].size()) || grid[r][c] != '1') {
        return;
    }
    grid[r][c] = '0'; // Sink
    int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1};
    for (int d = 0; d < 4; ++d) {
        sinkIsland(grid, r + dr[d], c + dc[d]);
    }
}

int numIslands(vector<vector<char>>& grid) {
    int count = 0;
    for (int r = 0; r < static_cast<int>(grid.size()); ++r) {
        for (int c = 0; c < static_cast<int>(grid[0].size()); ++c) {
            if (grid[r][c] == '1') {
                count++;
                sinkIsland(grid, r, c);
            }
        }
    }
    return count;
}

// [CORE] Rotting Oranges (Multi-Source BFS)
// Idea: Queue all initially rotten oranges (multi-source). Process layer by layer (minute by minute) rotting fresh neighbors.
// Time: O(r * c)
// Space: O(r * c)
int orangesRotting(vector<vector<int>>& grid) {
    int R = grid.size(), C = grid[0].size();
    queue<pair<int, int>> q;
    int freshCount = 0;

    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c) {
            if (grid[r][c] == 2) q.push({r, c});
            else if (grid[r][c] == 1) freshCount++;
        }
    }
    if (freshCount == 0) return 0;

    int minutes = -1;
    int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1};
    while (!q.empty()) {
        int sz = q.size();
        minutes++;
        for (int i = 0; i < sz; ++i) {
            auto [r, c] = q.front(); q.pop();
            for (int d = 0; d < 4; ++d) {
                int nr = r + dr[d], nc = c + dc[d];
                if (nr >= 0 && nr < R && nc >= 0 && nc < C && grid[nr][nc] == 1) {
                    grid[nr][nc] = 2;
                    freshCount--;
                    q.push({nr, nc});
                }
            }
        }
    }
    return (freshCount == 0) ? minutes : -1;
}

// [HIGH] Shortest Path in Binary Matrix
// Idea: Standard 8-directional BFS from (0, 0) to (n-1, n-1) on cells containing 0.
// Time: O(n^2)
// Space: O(n^2)
int shortestPathBinaryMatrix(vector<vector<int>>& grid) {
    int n = grid.size();
    if (grid[0][0] != 0 || grid[n - 1][n - 1] != 0) return -1;
    queue<pair<int, int>> q;
    q.push({0, 0});
    grid[0][0] = 1; // Mark distance

    int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        int dist = grid[r][c];
        if (r == n - 1 && c == n - 1) return dist;

        for (int d = 0; d < 8; ++d) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr >= 0 && nr < n && nc >= 0 && nc < n && grid[nr][nc] == 0) {
                grid[nr][nc] = dist + 1;
                q.push({nr, nc});
            }
        }
    }
    return -1;
}

// [HIGH] Pacific Atlantic Water Flow
// Idea: Reverse inflow DFS starting from ocean borders (Pacific: top/left, Atlantic: bottom/right). Intersect reachable cells.
// Time: O(r * c)
// Space: O(r * c)
void dfsWaterFlow(int r, int c, const vector<vector<int>>& heights, vector<vector<bool>>& ocean) {
    ocean[r][c] = true;
    int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1};
    int R = heights.size(), C = heights[0].size();

    for (int d = 0; d < 4; ++d) {
        int nr = r + dr[d], nc = c + dc[d];
        if (nr >= 0 && nr < R && nc >= 0 && nc < C && !ocean[nr][nc] && heights[nr][nc] >= heights[r][c]) {
            dfsWaterFlow(nr, nc, heights, ocean);
        }
    }
}

vector<vector<int>> pacificAtlantic(const vector<vector<int>>& heights) {
    if (heights.empty() || heights[0].empty()) return {};
    int R = heights.size(), C = heights[0].size();
    vector<vector<bool>> pac(R, vector<bool>(C, false)), atl(R, vector<bool>(C, false));

    for (int r = 0; r < R; ++r) { dfsWaterFlow(r, 0, heights, pac); dfsWaterFlow(r, C - 1, heights, atl); }
    for (int c = 0; c < C; ++c) { dfsWaterFlow(0, c, heights, pac); dfsWaterFlow(R - 1, c, heights, atl); }

    vector<vector<int>> res;
    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c) {
            if (pac[r][c] && atl[r][c]) res.push_back({r, c});
        }
    }
    return res;
}

// [CORE] Clone Graph
// Idea: Hash map maps original GraphNode* to cloned GraphNode*. DFS/BFS clones vertices and creates edges.
// Time: O(V + E)
// Space: O(V)
GraphNode* cloneGraphHelper(GraphNode* node, unordered_map<GraphNode*, GraphNode*>& clones) {
    if (!node) return nullptr;
    if (clones.count(node)) return clones[node];
    GraphNode* copy = new GraphNode(node->val);
    clones[node] = copy;
    for (GraphNode* neighbor : node->neighbors) {
        copy->neighbors.push_back(cloneGraphHelper(neighbor, clones));
    }
    return copy;
}

GraphNode* cloneGraph(GraphNode* node) {
    unordered_map<GraphNode*, GraphNode*> clones;
    return cloneGraphHelper(node, clones);
}

// [HIGH] Graph Valid Tree
// Idea: An undirected graph is a valid tree iff edges == n - 1 and all nodes are connected (no cycles).
// Time: O(V + E)
// Space: O(V + E)
bool validTree(int n, vector<vector<int>>& edges) {
    if (static_cast<int>(edges.size()) != n - 1) return false;
    vector<vector<int>> adj(n);
    for (const auto& e : edges) {
        adj[e[0]].push_back(e[1]);
        adj[e[1]].push_back(e[0]);
    }
    vector<bool> visited(n, false);
    queue<int> q;
    q.push(0);
    visited[0] = true;
    int visitedCount = 0;

    while (!q.empty()) {
        int u = q.front(); q.pop();
        visitedCount++;
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }
    return visitedCount == n;
}

// [HIGH] Number of Connected Components in an Undirected Graph
// Idea: DFS traversal to visit all vertices in each component; count total number of unvisited starts.
// Time: O(V + E)
// Space: O(V + E)
void dfsComponent(int u, const vector<vector<int>>& adj, vector<bool>& visited) {
    visited[u] = true;
    for (int v : adj[u]) {
        if (!visited[v]) dfsComponent(v, adj, visited);
    }
}

int countComponents(int n, const vector<vector<int>>& edges) {
    vector<vector<int>> adj(n);
    for (const auto& e : edges) {
        adj[e[0]].push_back(e[1]);
        adj[e[1]].push_back(e[0]);
    }
    vector<bool> visited(n, false);
    int components = 0;
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            components++;
            dfsComponent(i, adj, visited);
        }
    }
    return components;
}

// [CORE] Word Ladder
// Idea: Shortest path in unweighted word transition graph -> BFS transforming one character ('a'-'z') at a time.
// Time: O(M * 26 * N) where M is word length, N is dictionary size
// Space: O(N)
int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
    unordered_set<string> dict(wordList.begin(), wordList.end());
    if (!dict.count(endWord)) return 0;

    queue<string> q;
    q.push(beginWord);
    int level = 1;

    while (!q.empty()) {
        int sz = q.size();
        for (int k = 0; k < sz; ++k) {
            string word = q.front(); q.pop();
            if (word == endWord) return level;

            for (size_t i = 0; i < word.size(); ++i) {
                char original = word[i];
                for (char c = 'a'; c <= 'z'; ++c) {
                    word[i] = c;
                    if (dict.count(word)) {
                        dict.erase(word); // Prevent revisiting
                        q.push(word);
                    }
                }
                word[i] = original;
            }
        }
        level++;
    }
    return 0;
}

// [HIGH] Open the Lock
// Idea: BFS on 10,000 lock combinations. For each of the 4 wheels, rotate +1 and -1 digit transitions.
// Time: O(10^4 * 8)
// Space: O(10^4)
int openLock(vector<string>& deadends, string target) {
    unordered_set<string> visited(deadends.begin(), deadends.end());
    if (visited.count("0000")) return -1;
    if (target == "0000") return 0;

    queue<pair<string, int>> q;
    q.push({"0000", 0});
    visited.insert("0000");

    while (!q.empty()) {
        auto [state, turns] = q.front(); q.pop();
        if (state == target) return turns;

        for (int i = 0; i < 4; ++i) {
            for (int diff : {-1, 1}) {
                string nextState = state;
                nextState[i] = (nextState[i] - '0' + diff + 10) % 10 + '0';
                if (!visited.count(nextState)) {
                    visited.insert(nextState);
                    q.push({nextState, turns + 1});
                }
            }
        }
    }
    return -1;
}


// ============================================================================
// 15. TOPOLOGICAL SORT
// ============================================================================

// [CORE] Course Schedule I (Kahn's BFS Indegree & DFS 3-State Cycle Detection)
// Kahn's BFS Indegree Algorithm:
// Idea: Count indegrees. Queue 0-indegree nodes, decrement neighbor indegrees. If processed == numCourses, no cycle exists.
// Time: O(V + E)
// Space: O(V + E)
bool canFinishKahn(int numCourses, vector<vector<int>>& prerequisites) {
    vector<vector<int>> adj(numCourses);
    vector<int> inDegree(numCourses, 0);
    for (const auto& pre : prerequisites) {
        adj[pre[1]].push_back(pre[0]);
        inDegree[pre[0]]++;
    }
    queue<int> q;
    for (int i = 0; i < numCourses; ++i) {
        if (inDegree[i] == 0) q.push(i);
    }
    int visitedCount = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        visitedCount++;
        for (int v : adj[u]) {
            if (--inDegree[v] == 0) q.push(v);
        }
    }
    return visitedCount == numCourses;
}

// DFS 3-State Cycle Detection (0 = Unvisited, 1 = Visiting / In Current Path, 2 = Visited)
bool hasCycleDFS(int u, const vector<vector<int>>& adj, vector<int>& state) {
    state[u] = 1; // Mark visiting
    for (int v : adj[u]) {
        if (state[v] == 1) return true; // Back edge detected
        if (state[v] == 0 && hasCycleDFS(v, adj, state)) return true;
    }
    state[u] = 2; // Fully processed
    return false;
}

bool canFinishDFS(int numCourses, vector<vector<int>>& prerequisites) {
    vector<vector<int>> adj(numCourses);
    for (const auto& pre : prerequisites) adj[pre[1]].push_back(pre[0]);
    vector<int> state(numCourses, 0);
    for (int i = 0; i < numCourses; ++i) {
        if (state[i] == 0 && hasCycleDFS(i, adj, state)) return false;
    }
    return true;
}

// [CORE] Course Schedule II
// Idea: Return the topological sequence using Kahn's algorithm. If cycle exists, return empty vector.
// Time: O(V + E)
// Space: O(V + E)
vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
    vector<vector<int>> adj(numCourses);
    vector<int> inDegree(numCourses, 0);
    for (const auto& pre : prerequisites) {
        adj[pre[1]].push_back(pre[0]);
        inDegree[pre[0]]++;
    }
    queue<int> q;
    for (int i = 0; i < numCourses; ++i) {
        if (inDegree[i] == 0) q.push(i);
    }
    vector<int> order;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : adj[u]) {
            if (--inDegree[v] == 0) q.push(v);
        }
    }
    return (static_cast<int>(order.size()) == numCourses) ? order : vector<int>{};
}

// [HIGH] Alien Dictionary
// Idea: Build DAG from first differing characters between adjacent sorted words, then perform topological sort.
// Time: O(Total characters in words)
// Space: O(1) (at most 26 vertices)
string alienOrder(vector<string>& words) {
    unordered_map<char, unordered_set<char>> adj;
    unordered_map<char, int> inDegree;
    for (const string& w : words) for (char c : w) inDegree[c] = 0;

    for (size_t i = 0; i < words.size() - 1; ++i) {
        const string& w1 = words[i];
        const string& w2 = words[i + 1];
        if (w1.size() > w2.size() && w1.rfind(w2, 0) == 0) return ""; // Invalid prefix order

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
        char curr = q.front(); q.pop();
        order += curr;
        for (char nxt : adj[curr]) {
            if (--inDegree[nxt] == 0) q.push(nxt);
        }
    }
    return (order.size() == inDegree.size()) ? order : "";
}


// ============================================================================
// 16. DISJOINT SET UNION (DSU)
// ============================================================================

// [CORE] Reusable DSU Structure (Path Compression + Union by Rank/Size)
// Idea: Disjoint Set Union with near O(1) inverse Ackermann complexity per operation.
// Time: O(alpha(n)) per operation
// Space: O(n)
class DSU {
    vector<int> parent;
    vector<int> rank;
    int numComponents;
public:
    DSU(int n) : parent(n), rank(n, 0), numComponents(n) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] == x) return x;
        return parent[x] = find(parent[x]); // Path compression
    }

    bool unite(int x, int y) {
        int rootX = find(x), rootY = find(y);
        if (rootX == rootY) return false; // Already in same set (cycle)

        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
        numComponents--;
        return true;
    }

    bool isConnected(int x, int y) { return find(x) == find(y); }
    int getComponents() const { return numComponents; }
};

// [CORE] Number of Provinces
// Idea: Connect cities with isConnected[i][j] == 1 using DSU; return remaining number of components.
// Time: O(n^2 * alpha(n))
// Space: O(n)
int findCircleNum(vector<vector<int>>& isConnected) {
    int n = isConnected.size();
    DSU dsu(n);
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (isConnected[i][j]) dsu.unite(i, j);
        }
    }
    return dsu.getComponents();
}

// [CORE] Redundant Connection
// Idea: Iterate through edges; the edge that connects two already united vertices forms the cycle.
// Time: O(n * alpha(n))
// Space: O(n)
vector<int> findRedundantConnection(vector<vector<int>>& edges) {
    int n = edges.size();
    DSU dsu(n + 1);
    for (const auto& e : edges) {
        if (!dsu.unite(e[0], e[1])) return e;
    }
    return {};
}

// [HIGH] Accounts Merge
// Idea: Map each email to account ID. Unite accounts sharing common emails using DSU, then group by root parent.
// Time: O(N * alpha(N) + N log N) for sorting emails
// Space: O(N)
vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
    int n = accounts.size();
    DSU dsu(n);
    unordered_map<string, int> emailToId;

    for (int i = 0; i < n; ++i) {
        for (size_t j = 1; j < accounts[i].size(); ++j) {
            const string& email = accounts[i][j];
            if (emailToId.count(email)) {
                dsu.unite(i, emailToId[email]);
            } else {
                emailToId[email] = i;
            }
        }
    }

    unordered_map<int, vector<string>> groups;
    for (auto& [email, id] : emailToId) {
        groups[dsu.find(id)].push_back(email);
    }

    vector<vector<string>> res;
    for (auto& [rootId, emails] : groups) {
        sort(emails.begin(), emails.end());
        emails.insert(emails.begin(), accounts[rootId][0]); // Prepend account name
        res.push_back(std::move(emails));
    }
    return res;
}


// ============================================================================
// 17. SHORTEST PATH & MST
// ============================================================================

// [CORE] Dijkstra's Algorithm (Template & Network Delay Time)
// Idea: Min-heap tracks {distance, vertex}. Greedily process shortest active distance node and relax outgoing edges.
// Time: O((V + E) log V)
// Space: O(V + E)
int networkDelayTime(vector<vector<int>>& times, int n, int k) {
    vector<vector<pair<int, int>>> adj(n + 1);
    for (const auto& t : times) adj[t[0]].push_back({t[1], t[2]}); // u -> {v, weight}

    vector<int> dist(n + 1, INT_MAX);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    dist[k] = 0;
    pq.push({0, k});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;

        for (auto& [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    int maxDelay = 0;
    for (int i = 1; i <= n; ++i) {
        if (dist[i] == INT_MAX) return -1;
        maxDelay = max(maxDelay, dist[i]);
    }
    return maxDelay;
}

// [HIGH] Cheapest Flights Within K Stops
// Idea: Bellman-Ford / BFS relaxing edges up to k + 1 times using a temporary distance copy to prevent cascading.
// Time: O(k * E)
// Space: O(V)
int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
    vector<int> dist(n, 1e9);
    dist[src] = 0;

    for (int step = 0; step <= k; ++step) {
        vector<int> tmp = dist;
        for (const auto& f : flights) {
            int u = f[0], v = f[1], w = f[2];
            if (dist[u] != 1e9 && dist[u] + w < tmp[v]) {
                tmp[v] = dist[u] + w;
            }
        }
        dist = tmp;
    }
    return (dist[dst] >= 1e9) ? -1 : dist[dst];
}

// [HIGH] 0-1 BFS
// Idea: Deque shortest path for graphs with edge weights 0 and 1. Push 0-weight edges to front, 1-weight edges to back.
// Time: O(V + E)
// Space: O(V)
int zeroOneBFS(int n, const vector<vector<pair<int, int>>>& adj, int src, int target) {
    vector<int> dist(n, INT_MAX);
    deque<int> dq;
    dist[src] = 0;
    dq.push_back(src);

    while (!dq.empty()) {
        int u = dq.front(); dq.pop_front();
        if (u == target) return dist[u];

        for (auto& [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                if (w == 0) dq.push_front(v);
                else dq.push_back(v);
            }
        }
    }
    return dist[target] == INT_MAX ? -1 : dist[target];
}

// [MEDIUM] Bellman-Ford Algorithm (Overview & Implementation)
// Idea: Relax all E edges V - 1 times. If a further relaxation occurs on V-th iteration, a negative cycle exists.
// Time: O(V * E)
// Space: O(V)
bool bellmanFord(int n, const vector<vector<int>>& edges, int src, vector<int>& dist) {
    dist.assign(n, 1e9);
    dist[src] = 0;

    for (int i = 0; i < n - 1; ++i) {
        for (const auto& e : edges) {
            int u = e[0], v = e[1], w = e[2];
            if (dist[u] != 1e9 && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
            }
        }
    }
    // Check for negative weight cycle
    for (const auto& e : edges) {
        int u = e[0], v = e[1], w = e[2];
        if (dist[u] != 1e9 && dist[u] + w < dist[v]) return false; // Negative cycle found
    }
    return true;
}

// [HIGH] Kruskal's Algorithm (MST)
// Idea: Sort all edges by weight. Greedily add edges using DSU if they connect disjoint components until V - 1 edges added.
// Time: O(E log E)
// Space: O(V)
int kruskalMST(int n, vector<vector<int>>& edges) {
    sort(edges.begin(), edges.end(), [](const auto& a, const auto& b) {
        return a[2] < b[2];
    });
    DSU dsu(n);
    int totalCost = 0, edgesCount = 0;
    for (const auto& e : edges) {
        if (dsu.unite(e[0], e[1])) {
            totalCost += e[2];
            if (++edgesCount == n - 1) break;
        }
    }
    return (edgesCount == n - 1) ? totalCost : -1;
}

// [HIGH] Prim's Algorithm (Min Cost to Connect All Points)
// Idea: Dense MST. Grow MST by repeatedly picking non-MST node with smallest distance to current MST.
// Time: O(V^2) or O(E log V)
// Space: O(V)
int minCostConnectPointsPrim(vector<vector<int>>& points) {
    int n = points.size(), totalCost = 0;
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
// 18. 1D DYNAMIC PROGRAMMING
// ============================================================================

// [CORE] Climbing Stairs
// Idea: Fibonacci DP: dp[i] = dp[i-1] + dp[i-2]. Optimized to two variables.
// Time: O(n)
// Space: O(1)
int climbStairs(int n) {
    if (n <= 2) return n;
    int prev2 = 1, prev1 = 2;
    for (int i = 3; i <= n; ++i) {
        int curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

// [CORE] Min Cost Climbing Stairs
// Idea: dp[i] = cost[i] + min(dp[i-1], dp[i-2]).
// Time: O(n)
// Space: O(1)
int minCostClimbingStairs(const vector<int>& cost) {
    int prev2 = cost[0], prev1 = cost[1];
    for (size_t i = 2; i < cost.size(); ++i) {
        int curr = cost[i] + min(prev1, prev2);
        prev2 = prev1;
        prev1 = curr;
    }
    return min(prev1, prev2);
}

// [CORE] House Robber I
// Idea: dp[i] = max(dp[i-1], dp[i-2] + nums[i]). Track two variables: rob current vs skip current.
// Time: O(n)
// Space: O(1)
int rob(const vector<int>& nums) {
    int robPrev = 0, robCurr = 0;
    for (int x : nums) {
        int newRob = max(robCurr, robPrev + x);
        robPrev = robCurr;
        robCurr = newRob;
    }
    return robCurr;
}

// [CORE] House Robber II (Circular Array)
// Idea: Solve House Robber I twice: once on subarray [0..n-2] and once on subarray [1..n-1].
// Time: O(n)
// Space: O(1)
int robLinear(const vector<int>& nums, int start, int end) {
    int robPrev = 0, robCurr = 0;
    for (int i = start; i <= end; ++i) {
        int newRob = max(robCurr, robPrev + nums[i]);
        robPrev = robCurr;
        robCurr = newRob;
    }
    return robCurr;
}

int robII(const vector<int>& nums) {
    int n = nums.size();
    if (n == 1) return nums[0];
    return max(robLinear(nums, 0, n - 2), robLinear(nums, 1, n - 1));
}

// [CORE] Coin Change (Fewest Coins)
// Idea: 1D bottom-up DP: dp[i] = min(dp[i], dp[i - coin] + 1) initialized to amount + 1.
// Time: O(amount * coins.size())
// Space: O(amount)
int coinChange(const vector<int>& coins, int amount) {
    vector<int> dp(amount + 1, amount + 1);
    dp[0] = 0;
    for (int i = 1; i <= amount; ++i) {
        for (int c : coins) {
            if (i >= c) dp[i] = min(dp[i], dp[i - c] + 1);
        }
    }
    return dp[amount] > amount ? -1 : dp[amount];
}

// [HIGH] Coin Change II (Total Ways)
// Idea: Unbounded knapsack 1D DP: iterate outer loop over coins to avoid counting permutations.
// Time: O(amount * coins.size())
// Space: O(amount)
int change(int amount, const vector<int>& coins) {
    vector<unsigned long long> dp(amount + 1, 0);
    dp[0] = 1;
    for (int c : coins) {
        for (int i = c; i <= amount; ++i) {
            dp[i] += dp[i - c];
        }
    }
    return dp[amount];
}

// [CORE] Decode Ways
// Idea: dp[i] is ways to decode s[0..i-1]. Check 1-digit valid [1..9] and 2-digit valid [10..26].
// Time: O(n)
// Space: O(1)
int numDecodings(const string& s) {
    if (s.empty() || s[0] == '0') return 0;
    int prev2 = 1, prev1 = 1;
    for (size_t i = 1; i < s.size(); ++i) {
        int curr = 0;
        if (s[i] != '0') curr += prev1;
        int twoDigit = stoi(s.substr(i - 1, 2));
        if (twoDigit >= 10 && twoDigit <= 26) curr += prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

// [CORE] Word Break
// Idea: dp[i] is true if prefix s[0..i-1] can be segmented into dictionary words.
// Time: O(n^2)
// Space: O(n)
bool wordBreak(const string& s, const vector<string>& wordDict) {
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

// [CORE] Maximum Product Subarray
// Idea: Maintain running maxProd and minProd because multiplying by negative flips signs.
// Time: O(n)
// Space: O(1)
int maxProduct(const vector<int>& nums) {
    int maxProd = nums[0], minProd = nums[0], ans = nums[0];
    for (size_t i = 1; i < nums.size(); ++i) {
        int x = nums[i];
        if (x < 0) swap(maxProd, minProd);
        maxProd = max(x, maxProd * x);
        minProd = min(x, minProd * x);
        ans = max(ans, maxProd);
    }
    return ans;
}

// [CORE] Maximum Subarray (Kadane's Algorithm)
// Idea: Running sum maxCurr = max(x, maxCurr + x); update global maxSoFar.
// Time: O(n)
// Space: O(1)
int maxSubArray(const vector<int>& nums) {
    int maxCurr = nums[0], maxSoFar = nums[0];
    for (size_t i = 1; i < nums.size(); ++i) {
        maxCurr = max(nums[i], maxCurr + nums[i]);
        maxSoFar = max(maxSoFar, maxCurr);
    }
    return maxSoFar;
}


// ============================================================================
// 19. 2D / STRING DYNAMIC PROGRAMMING
// ============================================================================

// [CORE] Unique Paths
// Idea: dp[c] = dp[c] + dp[c - 1] across rows using 1D rolling array.
// Time: O(m * n)
// Space: O(n)
int uniquePaths(int m, int n) {
    vector<int> dp(n, 1);
    for (int r = 1; r < m; ++r) {
        for (int c = 1; c < n; ++c) {
            dp[c] += dp[c - 1];
        }
    }
    return dp[n - 1];
}

// [CORE] Unique Paths II (With Obstacles)
// Idea: Grid DP setting dp[c] = 0 whenever obstacleGrid[r][c] == 1.
// Time: O(m * n)
// Space: O(n)
int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
    int m = obstacleGrid.size(), n = obstacleGrid[0].size();
    vector<long long> dp(n, 0);
    dp[0] = (obstacleGrid[0][0] == 0) ? 1 : 0;

    for (int r = 0; r < m; ++r) {
        for (int c = 0; c < n; ++c) {
            if (obstacleGrid[r][c] == 1) {
                dp[c] = 0;
            } else if (c > 0) {
                dp[c] += dp[c - 1];
            }
        }
    }
    return dp[n - 1];
}

// [CORE] Minimum Path Sum
// Idea: dp[r][c] = grid[r][c] + min(dp[r-1][c], dp[r][c-1]).
// Time: O(m * n)
// Space: O(n)
int minPathSum(vector<vector<int>>& grid) {
    int m = grid.size(), n = grid[0].size();
    vector<int> dp(n, 0);
    dp[0] = grid[0][0];
    for (int c = 1; c < n; ++c) dp[c] = dp[c - 1] + grid[0][c];

    for (int r = 1; r < m; ++r) {
        dp[0] += grid[r][0];
        for (int c = 1; c < n; ++c) {
            dp[c] = grid[r][c] + min(dp[c], dp[c - 1]);
        }
    }
    return dp[n - 1];
}

// [CORE] Longest Common Subsequence (LCS)
// Idea: If text1[i] == text2[j], dp[i][j] = 1 + dp[i-1][j-1]; else max(dp[i-1][j], dp[i][j-1]).
// Time: O(m * n)
// Space: O(min(m, n))
int longestCommonSubsequence(const string& text1, const string& text2) {
    int m = text1.size(), n = text2.size();
    vector<int> dp(n + 1, 0);

    for (int i = 1; i <= m; ++i) {
        int prevDiag = 0;
        for (int j = 1; j <= n; ++j) {
            int temp = dp[j];
            if (text1[i - 1] == text2[j - 1]) {
                dp[j] = 1 + prevDiag;
            } else {
                dp[j] = max(dp[j], dp[j - 1]);
            }
            prevDiag = temp;
        }
    }
    return dp[n];
}

// [CORE] Edit Distance
// Idea: Levenshtein distance: if match, dp[i-1][j-1]; else 1 + min(insert dp[i][j-1], delete dp[i-1][j], replace dp[i-1][j-1]).
// Time: O(m * n)
// Space: O(n)
int minDistance(const string& word1, const string& word2) {
    int m = word1.size(), n = word2.size();
    vector<int> dp(n + 1);
    iota(dp.begin(), dp.end(), 0);

    for (int i = 1; i <= m; ++i) {
        int prevDiag = dp[0];
        dp[0] = i;
        for (int j = 1; j <= n; ++j) {
            int temp = dp[j];
            if (word1[i - 1] == word2[j - 1]) {
                dp[j] = prevDiag;
            } else {
                dp[j] = 1 + min({dp[j], dp[j - 1], prevDiag});
            }
            prevDiag = temp;
        }
    }
    return dp[n];
}

// [HIGH] Interleaving String
// Idea: dp[r][c] true if s3[0..r+c-1] is formed by interleaving s1[0..r-1] and s2[0..c-1].
// Time: O(m * n)
// Space: O(n)
bool isInterleave(const string& s1, const string& s2, const string& s3) {
    int m = s1.size(), n = s2.size();
    if (m + n != static_cast<int>(s3.size())) return false;
    vector<bool> dp(n + 1, false);
    dp[0] = true;

    for (int c = 1; c <= n; ++c) dp[c] = dp[c - 1] && (s2[c - 1] == s3[c - 1]);

    for (int r = 1; r <= m; ++r) {
        dp[0] = dp[0] && (s1[r - 1] == s3[r - 1]);
        for (int c = 1; c <= n; ++c) {
            dp[c] = (dp[c] && s1[r - 1] == s3[r + c - 1]) || (dp[c - 1] && s2[c - 1] == s3[r + c - 1]);
        }
    }
    return dp[n];
}

// [HIGH] Longest Palindromic Subsequence
// Idea: LCS between string s and its reverse, or 2D interval DP: if s[i]==s[j] dp[i][j]=2+dp[i+1][j-1].
// Time: O(n^2)
// Space: O(n)
int longestPalindromeSubseq(const string& s) {
    string rev = s;
    reverse(rev.begin(), rev.end());
    return longestCommonSubsequence(s, rev);
}

// [ADVANCED] Wildcard Matching ('?' matches any char, '*' matches any sequence)
// Idea: 2D matching DP where '*' can match 0 characters (dp[i][j-1]) or 1+ characters (dp[i-1][j]).
// Time: O(m * n)
// Space: O(m * n)
bool isMatchWildcard(const string& s, const string& p) {
    int m = s.size(), n = p.size();
    vector<vector<bool>> dp(m + 1, vector<bool>(n + 1, false));
    dp[0][0] = true;

    for (int j = 1; j <= n; ++j) {
        if (p[j - 1] == '*') dp[0][j] = dp[0][j - 1];
    }

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (p[j - 1] == '*') {
                dp[i][j] = dp[i][j - 1] || dp[i - 1][j];
            } else if (p[j - 1] == '?' || s[i - 1] == p[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            }
        }
    }
    return dp[m][n];
}

// [ADVANCED] Regular Expression Matching ('.' matches single char, '*' matches zero or more of preceding element)
// Idea: 2D DP handling '*' matching 0 preceding elements (dp[i][j-2]) or 1+ preceding elements (dp[i-1][j]).
// Time: O(m * n)
// Space: O(m * n)
bool isMatchRegex(const string& s, const string& p) {
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
// 20. ADVANCED DYNAMIC PROGRAMMING
// ============================================================================

// [HIGH] Longest Increasing Subsequence (Patience Sorting O(N log N))
// Idea: Maintain tails array of minimum tail elements of increasing subsequences using lower_bound binary search.
// Time: O(n log n)
// Space: O(n)
int lengthOfLIS(const vector<int>& nums) {
    vector<int> tails;
    for (int x : nums) {
        auto it = lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x);
        else *it = x;
    }
    return tails.size();
}

// [HIGH] Partition Equal Subset Sum
// Idea: Reduce to 0/1 Knapsack: check if subset with sum == total / 2 exists using a 1D DP boolean array.
// Time: O(n * target)
// Space: O(target)
bool canPartition(const vector<int>& nums) {
    int total = accumulate(nums.begin(), nums.end(), 0);
    if (total % 2 != 0) return false;
    int target = total / 2;

    vector<bool> dp(target + 1, false);
    dp[0] = true;
    for (int x : nums) {
        for (int j = target; j >= x; --j) {
            dp[j] = dp[j] || dp[j - x];
        }
    }
    return dp[target];
}

// [HIGH] Target Sum (Subset Sum Reduction)
// Idea: Positive subset P and Negative subset N: P = (total + target) / 2. Reduce to subset sum count DP.
// Time: O(n * subsetSum)
// Space: O(subsetSum)
int findTargetSumWays(const vector<int>& nums, int target) {
    int total = accumulate(nums.begin(), nums.end(), 0);
    if (total < abs(target) || (total + target) % 2 != 0) return 0;
    int subsetSum = (total + target) / 2;

    vector<int> dp(subsetSum + 1, 0);
    dp[0] = 1;
    for (int x : nums) {
        for (int j = subsetSum; j >= x; --j) {
            dp[j] += dp[j - x];
        }
    }
    return dp[subsetSum];
}

// [ADVANCED] Burst Balloons (Interval DP)
// Idea: dp[left][right] represents max coins from bursting balloons in interval (left, right) with balloon k popped LAST.
// Time: O(n^3)
// Space: O(n^2)
int maxCoinsBurstBalloons(vector<int>& nums) {
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

// [ADVANCED] Matrix Chain Multiplication
// Idea: dp[i][j] = min operations to multiply matrices from i to j: min over k of (dp[i][k] + dp[k+1][j] + p[i-1]*p[k]*p[j]).
// Time: O(n^3)
// Space: O(n^2)
int matrixChainOrder(const vector<int>& p) {
    int n = p.size() - 1;
    vector<vector<int>> dp(n + 1, vector<int>(n + 1, 0));

    for (int len = 2; len <= n; ++len) {
        for (int i = 1; i <= n - len + 1; ++i) {
            int j = i + len - 1;
            dp[i][j] = INT_MAX;
            for (int k = i; k < j; ++k) {
                int cost = dp[i][k] + dp[k + 1][j] + p[i - 1] * p[k] * p[j];
                dp[i][j] = min(dp[i][j], cost);
            }
        }
    }
    return dp[1][n];
}

// [HIGH] Best Time to Buy and Sell Stock with Cooldown
// Idea: State machine DP with three states: hold (own stock), sold (just sold), rest (cooldown/can buy).
// Time: O(n)
// Space: O(1)
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

// [ADVANCED] Tree DP: House Robber III
// Idea: Postorder DFS returning pair {robCurrent, skipCurrent}.
// Time: O(n)
// Space: O(h)
pair<int, int> dfsHouseRobberIII(TreeNode* node) {
    if (!node) return {0, 0};
    auto [lRob, lSkip] = dfsHouseRobberIII(node->left);
    auto [rRob, rSkip] = dfsHouseRobberIII(node->right);

    int robCurr = node->val + lSkip + rSkip;
    int skipCurr = max(lRob, lSkip) + max(rRob, rSkip);
    return {robCurr, skipCurr};
}

int robIII(TreeNode* root) {
    auto [robRoot, skipRoot] = dfsHouseRobberIII(root);
    return max(robRoot, skipRoot);
}

// [ADVANCED] Dungeon Game
// Idea: Reverse bottom-right to top-left DP. dp[c] = min HP required before entering cell (r, c).
// Time: O(r * c)
// Space: O(c)
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

// [ADVANCED] Cherry Pickup
// Idea: Synchronous 3D DP of two persons walking from (0, 0) to (n-1, n-1). Step count k = r1 + c1 = r2 + c2.
// Time: O(n^3)
// Space: O(n^2)
int cherryPickup(vector<vector<int>>& grid) {
    int n = grid.size();
    vector<vector<int>> dp(n, vector<int>(n, -1));
    dp[0][0] = grid[0][0];

    for (int step = 1; step <= 2 * n - 2; ++step) {
        vector<vector<int>> nextDP(n, vector<int>(n, -1));
        for (int r1 = max(0, step - (n - 1)); r1 <= min(n - 1, step); ++r1) {
            for (int r2 = max(0, step - (n - 1)); r2 <= min(n - 1, step); ++r2) {
                int c1 = step - r1, c2 = step - r2;
                if (grid[r1][c1] == -1 || grid[r2][c2] == -1) continue;

                int cherries = (r1 == r2) ? grid[r1][c1] : grid[r1][c1] + grid[r2][c2];
                int prevMax = -1;
                for (int dr1 : {0, -1}) {
                    for (int dr2 : {0, -1}) {
                        int pr1 = r1 + dr1, pr2 = r2 + dr2;
                        if (pr1 >= 0 && pr2 >= 0 && dp[pr1][pr2] != -1) {
                            prevMax = max(prevMax, dp[pr1][pr2]);
                        }
                    }
                }
                if (prevMax != -1) nextDP[r1][r2] = prevMax + cherries;
            }
        }
        dp = nextDP;
    }
    return max(0, dp[n - 1][n - 1]);
}


// ============================================================================
// 21. DESIGN-ORIENTED DATA STRUCTURES
// ============================================================================

// [CORE] Implement Queue using Stacks
// Idea: Two stacks: inStack for push, outStack for pop/peek. Shift elements on demand for amortized O(1).
// Time: Push O(1), Pop/Peek O(1) amortized
// Space: O(n)
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
    MyQueue() {}
    void push(int x) { inStack.push(x); }
    int pop() { shift(); int v = outStack.top(); outStack.pop(); return v; }
    int peek() { shift(); return outStack.top(); }
    bool empty() { return inStack.empty() && outStack.empty(); }
};

// [CORE] Design HashMap
// Idea: Separate chaining using an array of buckets (lists of {key, value} pairs) with a hash function.
// Time: O(1) average for put, get, remove
// Space: O(Capacity + Elements)
class MyHashMap {
    static const int BUCKET_COUNT = 10007;
    vector<list<pair<int, int>>> buckets;

    int hash(int key) const { return key % BUCKET_COUNT; }

public:
    MyHashMap() : buckets(BUCKET_COUNT) {}

    void put(int key, int value) {
        auto& chain = buckets[hash(key)];
        for (auto& entry : chain) {
            if (entry.first == key) {
                entry.second = value;
                return;
            }
        }
        chain.push_back({key, value});
    }

    int get(int key) const {
        const auto& chain = buckets[hash(key)];
        for (const auto& entry : chain) {
            if (entry.first == key) return entry.second;
        }
        return -1;
    }

    void remove(int key) {
        auto& chain = buckets[hash(key)];
        for (auto it = chain.begin(); it != chain.end(); ++it) {
            if (it->first == key) {
                chain.erase(it);
                return;
            }
        }
    }
};

// [HIGH] Design HashSet
// Idea: Separate chaining with vector of lists of keys.
// Time: O(1) average for add, remove, contains
// Space: O(Capacity + Elements)
class MyHashSet {
    static const int BUCKET_COUNT = 10007;
    vector<list<int>> buckets;
    int hash(int key) const { return key % BUCKET_COUNT; }
public:
    MyHashSet() : buckets(BUCKET_COUNT) {}
    void add(int key) {
        auto& chain = buckets[hash(key)];
        for (int k : chain) if (k == key) return;
        chain.push_back(key);
    }
    void remove(int key) {
        auto& chain = buckets[hash(key)];
        chain.remove(key);
    }
    bool contains(int key) const {
        const auto& chain = buckets[hash(key)];
        for (int k : chain) if (k == key) return true;
        return false;
    }
};

// [CORE] LRU Cache
// Idea: Hash map storing iterators to a doubly linked list of {key, value} pairs. Move accessed nodes to front.
// Time: O(1) for get and put
// Space: O(capacity)
class LRUCache {
    int capacity;
    list<pair<int, int>> dll; // Most recently used at front
    unordered_map<int, list<pair<int, int>>::iterator> cacheMap;

public:
    LRUCache(int cap) : capacity(cap) {}

    int get(int key) {
        if (!cacheMap.count(key)) return -1;
        dll.splice(dll.begin(), dll, cacheMap[key]); // Move to front
        return cacheMap[key]->second;
    }

    void put(int key, int value) {
        if (cacheMap.count(key)) {
            dll.splice(dll.begin(), dll, cacheMap[key]);
            cacheMap[key]->second = value;
            return;
        }
        if (static_cast<int>(dll.size()) == capacity) {
            int lruKey = dll.back().first;
            dll.pop_back();
            cacheMap.erase(lruKey);
        }
        dll.push_front({key, value});
        cacheMap[key] = dll.begin();
    }
};

// [CORE] Insert Delete GetRandom O(1)
// Idea: Dynamic array storing values + hash map storing value to array index. Swap with back element to delete in O(1).
// Time: O(1) average for insert, remove, getRandom
// Space: O(n)
class RandomizedSet {
    vector<int> nums;
    unordered_map<int, int> valToIdx;

public:
    RandomizedSet() {}

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

// [HIGH] Logger Rate Limiter
// Idea: Hash map storing message to last printed timestamp. Print if timestamp >= lastTimestamp + 10.
// Time: O(1)
// Space: O(number of distinct messages)
class Logger {
    unordered_map<string, int> msgTimestamps;
public:
    Logger() {}
    bool shouldPrintMessage(int timestamp, const string& message) {
        if (msgTimestamps.count(message) && timestamp < msgTimestamps[message] + 10) {
            return false;
        }
        msgTimestamps[message] = timestamp;
        return true;
    }
};

// [ADVANCED] LFU Cache
// Idea: Hash map for keys, hash map for frequency lists, and minFreq tracker to evict least frequently and least recently used.
// Time: O(1) for get and put
// Space: O(capacity)
class LFUCache {
    struct Node {
        int key, val, freq;
    };
    int capacity, minFreq;
    unordered_map<int, list<Node>::iterator> keyToIter;
    unordered_map<int, list<Node>> freqToList;

    void updateFreq(list<Node>::iterator it) {
        int key = it->key, val = it->val, freq = it->freq;
        freqToList[freq].erase(it);
        if (freqToList[freq].empty()) {
            freqToList.erase(freq);
            if (minFreq == freq) minFreq++;
        }
        freqToList[freq + 1].push_front({key, val, freq + 1});
        keyToIter[key] = freqToList[freq + 1].begin();
    }

public:
    LFUCache(int cap) : capacity(cap), minFreq(0) {}

    int get(int key) {
        if (!keyToIter.count(key)) return -1;
        updateFreq(keyToIter[key]);
        return keyToIter[key]->val;
    }

    void put(int key, int value) {
        if (capacity <= 0) return;
        if (keyToIter.count(key)) {
            keyToIter[key]->val = value;
            updateFreq(keyToIter[key]);
            return;
        }
        if (static_cast<int>(keyToIter.size()) == capacity) {
            auto lfuNode = freqToList[minFreq].back();
            keyToIter.erase(lfuNode.key);
            freqToList[minFreq].pop_back();
            if (freqToList[minFreq].empty()) freqToList.erase(minFreq);
        }
        minFreq = 1;
        freqToList[1].push_front({key, value, 1});
        keyToIter[key] = freqToList[1].begin();
    }
};

// [ADVANCED] All O(1) Data Structure
// Idea: Doubly linked list of frequency buckets, where each bucket holds a set of keys. Maps key to bucket iterator.
// Time: O(1) for inc, dec, getMaxKey, getMinKey
// Space: O(number of keys)
class AllOne {
    struct Bucket {
        int count;
        unordered_set<string> keys;
    };
    list<Bucket> buckets; // Sorted frequency list
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

    string getMaxKey() { return buckets.empty() ? "" : *buckets.back().keys.begin(); }
    string getMinKey() { return buckets.empty() ? "" : *buckets.front().keys.begin(); }
};

// [ADVANCED] Design In-Memory File System
// Idea: Trie-like hierarchical directory tree where each node holds subdirectories and concatenated file content.
// Time: ls O(path length + files), mkdir O(path length), addContentToFile O(path length), readContentFromFile O(path length)
// Space: O(Total file system contents)
class FileSystem {
    struct FSNode {
        bool isFile = false;
        string content = "";
        map<string, FSNode*> children;
    };
    FSNode* root;

    vector<string> splitPath(const string& path) {
        vector<string> parts;
        stringstream ss(path);
        string part;
        while (getline(ss, part, '/')) {
            if (!part.empty()) parts.push_back(part);
        }
        return parts;
    }

    FSNode* traverse(const string& path) {
        FSNode* curr = root;
        for (const string& p : splitPath(path)) {
            if (!curr->children.count(p)) curr->children[p] = new FSNode();
            curr = curr->children[p];
        }
        return curr;
    }

public:
    FileSystem() { root = new FSNode(); }

    vector<string> ls(string path) {
        FSNode* node = traverse(path);
        if (node->isFile) {
            auto parts = splitPath(path);
            return {parts.back()};
        }
        vector<string> res;
        for (auto& [name, _] : node->children) res.push_back(name);
        return res;
    }

    void mkdir(string path) { traverse(path); }

    void addContentToFile(string filePath, string content) {
        FSNode* node = traverse(filePath);
        node->isFile = true;
        node->content += content;
    }

    string readContentFromFile(string filePath) {
        return traverse(filePath)->content;
    }
};

// [ADVANCED] Design Twitter
// Idea: Map users to followees set and user tweet lists. Pull top 10 most recent tweets via merge k-sorted lists using min-heap.
// Time: postTweet O(1), follow/unfollow O(1), getNewsFeed O(k log U) where U is followees count
// Space: O(Users + Tweets)
class Twitter {
    int timestamp = 0;
    unordered_map<int, unordered_set<int>> followees;
    unordered_map<int, vector<pair<int, int>>> userTweets; // {time, tweetId}

public:
    Twitter() {}

    void postTweet(int userId, int tweetId) {
        userTweets[userId].push_back({timestamp++, tweetId});
    }

    vector<int> getNewsFeed(int userId) {
        priority_queue<pair<int, int>> pq;
        unordered_set<int> users = followees[userId];
        users.insert(userId); // Self tweets included

        for (int u : users) {
            const auto& tweets = userTweets[u];
            int n = tweets.size();
            for (int i = n - 1; i >= max(0, n - 10); --i) {
                pq.push(tweets[i]);
            }
        }

        vector<int> feed;
        while (!pq.empty() && feed.size() < 10) {
            feed.push_back(pq.top().second);
            pq.pop();
        }
        return feed;
    }

    void follow(int followerId, int followeeId) {
        if (followerId != followeeId) followees[followerId].insert(followeeId);
    }

    void unfollow(int followerId, int followeeId) {
        followees[followerId].erase(followeeId);
    }
};

// [ADVANCED] Read N Characters Given Read4 II (Call Multiple Times)
// Idea: Maintain internal 4-character buffer, buffer pointer, and buffer count across consecutive read calls.
// Time: O(n)
// Space: O(1)
int read4(char *buf4); // API stub provided by interview platform

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


// ============================================================================
// 22. INTERVIEW FOLLOW-UP PATTERNS & PLAYBOOK
// ============================================================================
/*
--------------------------------------------------------------------------------
  1. TWO SUM VARIANTS & SCALE FOLLOW-UPS
--------------------------------------------------------------------------------
  - Sorted Input:
    Use Two Pointers (O(n) time, O(1) space) instead of Hash Map (O(n) space).
  - Return All Unique Pairs:
    Sort first or use hash map with frequency counters; skip duplicate elements upon match.
  - 3Sum / 4Sum Generalization:
    Sort array, reduce K-Sum to (K-1)-Sum recursively or iteratively until 2Sum with Two Pointers.
  - Streaming Input / Data Stream:
    Use balanced BST or Two Pointers if window bounded; hash map if unbounded.
  - Limited Memory / Massive Dataset (Does not fit in RAM):
    External sort chunks of data on disk, then perform two-pointer linear pass across sorted external files,
    or use bucketed hashing (partition data into hash buckets on disk where x and (target - x) go to paired partitions).

--------------------------------------------------------------------------------
  2. NUMBER OF ISLANDS VARIANTS
--------------------------------------------------------------------------------
  - 8-Directional Movement:
    Expand direction arrays `dr[] = {-1,-1,-1,0,0,1,1,1}` and `dc[] = {-1,0,1,-1,1,-1,0,1}`.
  - Largest Island Area:
    DFS returns cumulative cell count `1 + sum(dfs(neighbor))` instead of void.
  - Dynamic Land Additions / Online Queries (Number of Islands II):
    Use Disjoint Set Union (DSU). For each added land cell, initialize component with parent=self,
    check 4 neighbors, and execute `dsu.unite(land, neighbor)` for every adjacent land cell.
  - Hexagonal Grids:
    Adjust adjacency list based on odd/even row parity coordinates.

--------------------------------------------------------------------------------
  3. LRU CACHE EXTENSIONS & CONCURRENCY
--------------------------------------------------------------------------------
  - Thread Safety & Concurrency:
    Use std::shared_mutex (Read-Write lock) for fine-grained concurrent access, or Sharded LRU
    (partition hash keys across N independent LRU segments, e.g. 16 shards, to minimize lock contention).
  - TTL (Time-To-Live / Key Expiration):
    Store expiration timestamp in Node. Evict expired nodes lazily during `get()` / `put()`,
    or maintain an auxiliary Min-Heap ordered by expiration timestamp for background cleanup.
  - Memory-Bounded Eviction (Variable Payload Sizes):
    Track current total memory in bytes instead of element count; evict tail until `currentBytes + newItemBytes <= maxBytes`.

--------------------------------------------------------------------------------
  4. BFS & SHORTEST PATH TRADE-OFFS
--------------------------------------------------------------------------------
  - Unweighted Graph -> Standard BFS (O(V + E) using queue).
  - Binary Weights (0 and 1) -> 0-1 BFS (O(V + E) using std::deque: 0 to front, 1 to back).
  - Non-negative Weights -> Dijkstra's Algorithm (O((V + E) log V) using min-priority queue).
  - General Weights with Negative Edges -> Bellman-Ford (O(V * E)).
  - Large Search Space (e.g. Word Ladder, Rubik's Cube) -> Bidirectional BFS (meets in middle, reduces branching factor from b^d to 2 * b^(d/2)).
  - Multi-Source Propagation (Rotting Oranges, Walls and Gates) -> Push all sources into queue initially at level 0.

--------------------------------------------------------------------------------
  5. BINARY SEARCH ON REAL NUMBERS & PREDICATES
--------------------------------------------------------------------------------
  - Monotonic Feasibility Predicate (BS on Answer):
    If condition `canAchieve(mid)` is monotonic (`TTTTFFFF` or `FFFFTTTT`), binary search over candidate value domain [low..high].
  - Floating Point / Real Number Precision:
    Instead of `while (low <= high)`, iterate for a fixed count (e.g. `for (int iter = 0; iter < 100; ++iter)`)
    or check `while (high - low > 1e-7)` to prevent infinite floating-point loops.
  - Boundary Disambiguation:
    `lower_bound`: first element >= target.
    `upper_bound`: first element > target.

--------------------------------------------------------------------------------
  6. TOP-K & STREAMING QUANTILES
--------------------------------------------------------------------------------
  - In-Memory Static Array -> QuickSelect (O(n) average, O(1) space).
  - Streaming / Continuous Updates -> Min-Heap of size K (O(n log k) time, O(k) space).
  - Bounded Integer Range -> Bucket Sort (O(n) time, O(range) space).
  - Approximate Quantiles in Distributed Stream -> Count-Min Sketch / T-Digest / HyperLogLog.
================================================================================
*/
