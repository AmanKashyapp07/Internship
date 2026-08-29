#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <climits>
#endif
using namespace std;

// =========================================================
// 59. SORT COLORS (DUTCH NATIONAL FLAG)
// =========================================================

void sortColors(vector<int>& nums) {
    int low = 0, mid = 0, high = (int)nums.size() - 1;
    while (mid <= high) {
        if (nums[mid] == 0) {
            swap(nums[low++], nums[mid++]);
        } else if (nums[mid] == 1) {
            mid++;
        } else {
            swap(nums[mid], nums[high--]);
        }
    }
}
// Interview Explanation:
// - Problem Statement: Sort array of 0s, 1s, and 2s in-place in a single pass.
// - Approach: Dutch National Flag 3-Way Partitioning.
// - Intuition: Maintain low, mid, and high pointers for 0-boundary, unclassified, and 2-boundary.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 60. 3SUM
// =========================================================

vector<vector<int>> threeSum(vector<int>& nums) {
    sort(nums.begin(), nums.end());
    vector<vector<int>> res;
    int n = nums.size();

    for (int i = 0; i < n - 2; ++i) {
        if (i > 0 && nums[i] == nums[i - 1]) continue;
        if (nums[i] > 0) break;

        int l = i + 1, r = n - 1;
        while (l < r) {
            int sum = nums[i] + nums[l] + nums[r];
            if (sum == 0) {
                res.push_back({nums[i], nums[l], nums[r]});
                while (l < r && nums[l] == nums[l + 1]) l++;
                while (l < r && nums[r] == nums[r - 1]) r--;
                l++; r--;
            } else if (sum < 0) l++;
            else r--;
        }
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Find all unique triplets summing to 0.
// - Approach: Sort + Two Pointers with Duplication Skipping.
// - Intuition: Fix first element, then scan rest with two pointers; skip adjacent identical values.
// - Complexity: Time: O(N^2), Space: O(1) auxiliary space.


// =========================================================
// 61. 4SUM
// =========================================================

vector<vector<int>> fourSum(vector<int>& nums, int target) {
    sort(nums.begin(), nums.end());
    vector<vector<int>> res;
    int n = nums.size();

    for (int i = 0; i < n - 3; ++i) {
        if (i > 0 && nums[i] == nums[i - 1]) continue;
        for (int j = i + 1; j < n - 2; ++j) {
            if (j > i + 1 && nums[j] == nums[j - 1]) continue;
            long long rem = (long long)target - nums[i] - nums[j];
            int l = j + 1, r = n - 1;
            while (l < r) {
                long long sum = nums[l] + nums[r];
                if (sum == rem) {
                    res.push_back({nums[i], nums[j], nums[l], nums[r]});
                    while (l < r && nums[l] == nums[l + 1]) l++;
                    while (l < r && nums[r] == nums[r - 1]) r--;
                    l++; r--;
                } else if (sum < rem) l++;
                else r--;
            }
        }
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Find all unique quadruplets summing to target.
// - Approach: Two Nested Loops + Two Pointers.
// - Intuition: Fix first two elements with deduplication, then use Two Pointers for remaining pair.
// - Complexity: Time: O(N^3), Space: O(1).


// =========================================================
// 62. CONTAINER WITH MOST WATER
// =========================================================

int maxArea(vector<int>& height) {
    int l = 0, r = (int)height.size() - 1, maxWater = 0;
    while (l < r) {
        maxWater = max(maxWater, min(height[l], height[r]) * (r - l));
        if (height[l] < height[r]) l++;
        else r--;
    }
    return maxWater;
}
// Interview Explanation:
// - Problem Statement: Find two lines that together with x-axis contain the most water.
// - Approach: Greedy Inward Two Pointers.
// - Intuition: Area is constrained by shorter line; move shorter pointer inward to find potentially taller boundary.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 63. MERGE INTERVALS
// =========================================================

vector<vector<int>> mergeIntervals(vector<vector<int>>& intervals) {
    if (intervals.empty()) return {};
    sort(intervals.begin(), intervals.end());
    vector<vector<int>> merged = {intervals[0]};

    for (size_t i = 1; i < intervals.size(); ++i) {
        if (intervals[i][0] <= merged.back()[1]) {
            merged.back()[1] = max(merged.back()[1], intervals[i][1]);
        } else {
            merged.push_back(intervals[i]);
        }
    }
    return merged;
}
// Interview Explanation:
// - Problem Statement: Merge all overlapping intervals.
// - Approach: Interval Sorting + Greedy Merge.
// - Intuition: If curr.start <= prev.end, merge by updating prev.end = max(prev.end, curr.end).
// - Complexity: Time: O(N log N), Space: O(1) auxiliary space.


// =========================================================
// 64. NON-OVERLAPPING INTERVALS
// =========================================================

int eraseOverlapIntervals(vector<vector<int>>& intervals) {
    if (intervals.empty()) return 0;
    sort(intervals.begin(), intervals.end(), [](const auto& a, const auto& b) {
        return a[1] < b[1];
    });

    int count = 1, prevEnd = intervals[0][1];
    for (size_t i = 1; i < intervals.size(); ++i) {
        if (intervals[i][0] >= prevEnd) {
            count++;
            prevEnd = intervals[i][1];
        }
    }
    return intervals.size() - count;
}
// Interview Explanation:
// - Problem Statement: Minimum intervals to remove to make remainder non-overlapping.
// - Approach: Greedy Interval Scheduling (Sort by End Time).
// - Intuition: Picking intervals that end earliest leaves maximum room for subsequent intervals.
// - Complexity: Time: O(N log N), Space: O(1).


// =========================================================
// 65. TOP K FREQUENT ELEMENTS
// =========================================================

vector<int> topKFrequent(vector<int>& nums, int k) {
    unordered_map<int, int> countMap;
    for (int x : nums) countMap[x]++;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> minHeap;
    for (auto& [val, freq] : countMap) {
        minHeap.push({freq, val});
        if ((int)minHeap.size() > k) minHeap.pop();
    }

    vector<int> res;
    while (!minHeap.empty()) {
        res.push_back(minHeap.top().second);
        minHeap.pop();
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Return the k most frequent elements.
// - Approach: Frequency Map + Min-Heap of Size K.
// - Intuition: Maintaining a heap of size k retains the top k elements in O(N log k).
// - Complexity: Time: O(N log K), Space: O(N).


// =========================================================
// 66. KTH LARGEST ELEMENT IN AN ARRAY
// =========================================================

int findKthLargest(vector<int>& nums, int k) {
    priority_queue<int, vector<int>, greater<int>> minHeap;
    for (int x : nums) {
        minHeap.push(x);
        if ((int)minHeap.size() > k) minHeap.pop();
    }
    return minHeap.top();
}
// Interview Explanation:
// - Problem Statement: Find kth largest element in unsorted array.
// - Approach: Min-Heap of Capacity K (or QuickSelect).
// - Intuition: Min-heap of size k ensures top element is the kth largest seen so far.
// - Complexity: Time: O(N log K), Space: O(K).


// =========================================================
// 67. K CLOSEST POINTS TO ORIGIN
// =========================================================

vector<vector<int>> kClosest(vector<vector<int>>& points, int k) {
    // Max-heap storing {distSq, pointIdx}
    priority_queue<pair<int, int>> maxHeap;
    for (int i = 0; i < (int)points.size(); ++i) {
        int distSq = points[i][0] * points[i][0] + points[i][1] * points[i][1];
        maxHeap.push({distSq, i});
        if ((int)maxHeap.size() > k) maxHeap.pop();
    }
    vector<vector<int>> res;
    while (!maxHeap.empty()) {
        res.push_back(points[maxHeap.top().second]);
        maxHeap.pop();
    }
    return res;
}
// Interview Explanation:
// - Problem Statement: Find k closest points to origin (0, 0).
// - Approach: Max-Heap of Size K.
// - Intuition: Max-heap automatically evicts the furthest point when size exceeds k.
// - Complexity: Time: O(N log K), Space: O(K).


// =========================================================
// 68. FIND MEDIAN FROM DATA STREAM
// =========================================================

class MedianFinder {
    priority_queue<int> maxHeap; // Lower half
    priority_queue<int, vector<int>, greater<int>> minHeap; // Upper half
public:
    MedianFinder() {}
    void addNum(int num) {
        maxHeap.push(num);
        minHeap.push(maxHeap.top()); maxHeap.pop();
        if (maxHeap.size() < minHeap.size()) {
            maxHeap.push(minHeap.top()); minHeap.pop();
        }
    }
    double findMedian() {
        if (maxHeap.size() > minHeap.size()) return maxHeap.top();
        return (maxHeap.top() + minHeap.top()) / 2.0;
    }
};
// Interview Explanation:
// - Problem Statement: Find median dynamically from a stream of integers.
// - Approach: Two Heaps (Balanced Max-Heap & Min-Heap).
// - Intuition: Maintain balance such that maxHeap has half smaller elements and minHeap has half larger elements.
// - Complexity: Time: O(log N) for addNum, O(1) for findMedian, Space: O(N).


// =========================================================
// 69. JUMP GAME
// =========================================================

bool canJump(vector<int>& nums) {
    int maxReach = 0, n = nums.size();
    for (int i = 0; i < n; ++i) {
        if (i > maxReach) return false;
        maxReach = max(maxReach, i + nums[i]);
        if (maxReach >= n - 1) return true;
    }
    return true;
}
// Interview Explanation:
// - Problem Statement: Check if last index is reachable.
// - Approach: Greedy Max Reachable Index Tracker.
// - Intuition: If current index exceeds maxReach, last index is unreachable.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 70. JUMP GAME II
// =========================================================

int jump(vector<int>& nums) {
    int jumps = 0, currEnd = 0, farthest = 0, n = nums.size();
    for (int i = 0; i < n - 1; ++i) {
        farthest = max(farthest, i + nums[i]);
        if (i == currEnd) {
            jumps++;
            currEnd = farthest;
            if (currEnd >= n - 1) break;
        }
    }
    return jumps;
}
// Interview Explanation:
// - Problem Statement: Find minimum number of jumps to reach last index.
// - Approach: Greedy Window BFS.
// - Intuition: In each jump, determine the farthest reachable boundary for the next jump.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 71. GAS STATION
// =========================================================

int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
    int totalTank = 0, currTank = 0, startIdx = 0;
    for (size_t i = 0; i < gas.size(); ++i) {
        totalTank += gas[i] - cost[i];
        currTank += gas[i] - cost[i];
        if (currTank < 0) {
            startIdx = i + 1;
            currTank = 0;
        }
    }
    return totalTank >= 0 ? startIdx : -1;
}
// Interview Explanation:
// - Problem Statement: Find starting gas station index to complete circular tour.
// - Approach: Greedy Net Balance Invariant.
// - Intuition: If totalGas >= totalCost, a unique solution is guaranteed; reset start when tank drops negative.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 72. PARTITION LABELS
// =========================================================

vector<int> partitionLabels(string s) {
    vector<int> lastIdx(26, 0);
    for (int i = 0; i < (int)s.size(); ++i) lastIdx[s[i] - 'a'] = i;

    vector<int> partitions;
    int start = 0, maxReach = 0;
    for (int i = 0; i < (int)s.size(); ++i) {
        maxReach = max(maxReach, lastIdx[s[i] - 'a']);
        if (i == maxReach) {
            partitions.push_back(i - start + 1);
            start = i + 1;
        }
    }
    return partitions;
}
// Interview Explanation:
// - Problem Statement: Partition string so each letter appears in at most one part.
// - Approach: Greedy Last Occurrence Window.
// - Intuition: Extend current partition until reaching the max last occurrence index of all characters inside it.
// - Complexity: Time: O(N), Space: O(1).


// =========================================================
// 73. SUBSETS
// =========================================================

class SolutionSubsets {
    void backtrack(const vector<int>& nums, int start, vector<int>& curr, vector<vector<int>>& res) {
        res.push_back(curr);
        for (int i = start; i < (int)nums.size(); ++i) {
            curr.push_back(nums[i]);
            backtrack(nums, i + 1, curr, res);
            curr.pop_back();
        }
    }
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>> res;
        vector<int> curr;
        backtrack(nums, 0, curr, res);
        return res;
    }
};
// Interview Explanation:
// - Problem Statement: Return power set of distinct integers.
// - Approach: Backtracking Cascading Recursion.
// - Intuition: At each step, record current subset, then branch on subsequent elements.
// - Complexity: Time: O(N * 2^N), Space: O(N).


// =========================================================
// 74. PERMUTATIONS
// =========================================================

class SolutionPermutations {
    void backtrack(vector<int>& nums, int start, vector<vector<int>>& res) {
        if (start == (int)nums.size()) {
            res.push_back(nums);
            return;
        }
        for (int i = start; i < (int)nums.size(); ++i) {
            swap(nums[start], nums[i]);
            backtrack(nums, start + 1, res);
            swap(nums[start], nums[i]); // Backtrack
        }
    }
public:
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> res;
        backtrack(nums, 0, res);
        return res;
    }
};
// Interview Explanation:
// - Problem Statement: Return all permutations of distinct integers.
// - Approach: In-Place Backtracking via Element Swapping.
// - Intuition: Swap elements into the current position, recurse for the suffix, then backtrack.
// - Complexity: Time: O(N * N!), Space: O(N) recursion stack.


// =========================================================
// 75. COMBINATION SUM
// =========================================================

class SolutionCombinationSum {
    void backtrack(const vector<int>& cand, int target, int start, vector<int>& curr, vector<vector<int>>& res) {
        if (target == 0) { res.push_back(curr); return; }
        for (int i = start; i < (int)cand.size(); ++i) {
            if (cand[i] > target) break;
            curr.push_back(cand[i]);
            backtrack(cand, target - cand[i], i, curr, res);
            curr.pop_back();
        }
    }
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        sort(candidates.begin(), candidates.end());
        vector<vector<int>> res;
        vector<int> curr;
        backtrack(candidates, target, 0, curr, res);
        return res;
    }
};
// Interview Explanation:
// - Problem Statement: Find all unique combinations summing to target (candidates can be reused).
// - Approach: Backtracking with Candidate Re-use & Sorted Pruning.
// - Intuition: Passing current index 'i' allows element reuse; sorting enables early loop breaks.
// - Complexity: Time: O(2^T), Space: O(T / min_cand).


// =========================================================
// 76. WORD SEARCH
// =========================================================

class SolutionWordSearch {
    bool dfs(vector<vector<char>>& board, const string& word, int r, int c, int idx) {
        if (idx == (int)word.size()) return true;
        int m = board.size(), n = board[0].size();
        if (r < 0 || r >= m || c < 0 || c >= n || board[r][c] != word[idx]) return false;

        char temp = board[r][c];
        board[r][c] = '#';
        bool found = dfs(board, word, r + 1, c, idx + 1) ||
                     dfs(board, word, r - 1, c, idx + 1) ||
                     dfs(board, word, r, c + 1, idx + 1) ||
                     dfs(board, word, r, c - 1, idx + 1);
        board[r][c] = temp;
        return found;
    }
public:
    bool exist(vector<vector<char>>& board, string word) {
        for (int r = 0; r < (int)board.size(); ++r) {
            for (int c = 0; c < (int)board[0].size(); ++c) {
                if (board[r][c] == word[0] && dfs(board, word, r, c, 0)) return true;
            }
        }
        return false;
    }
};
// Interview Explanation:
// - Problem Statement: Determine if word exists in 2D grid of characters.
// - Approach: 2D Grid Backtracking DFS with In-Place '#' Masking.
// - Intuition: Temporarily mask visited cells to avoid re-entry; unmask on backtrack.
// - Complexity: Time: O(M * N * 3^L), Space: O(L) recursion stack.


// =========================================================
// 77. GENERATE PARENTHESES
// =========================================================

class SolutionGenerateParens {
    void backtrack(int open, int close, int n, string& curr, vector<string>& res) {
        if ((int)curr.size() == 2 * n) { res.push_back(curr); return; }
        if (open < n) {
            curr.push_back('(');
            backtrack(open + 1, close, n, curr, res);
            curr.pop_back();
        }
        if (close < open) {
            curr.push_back(')');
            backtrack(open, close + 1, n, curr, res);
            curr.pop_back();
        }
    }
public:
    vector<string> generateParenthesis(int n) {
        vector<string> res;
        string curr = "";
        backtrack(0, 0, n, curr, res);
        return res;
    }
};
// Interview Explanation:
// - Problem Statement: Generate all combinations of well-formed parentheses.
// - Approach: Backtracking with Open/Close Count Balance.
// - Intuition: Can add '(' if open < n; can add ')' if close < open.
// - Complexity: Time: O(4^N / sqrt(N)) (Catalan Number), Space: O(N).


/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 59 | Sort Colors                                 | Dutch National Flag 3-Way         | O(N)     | O(1)     |
 | 60 | 3Sum                                        | Sort + Two Pointers + Skip DUP    | O(N^2)   | O(1)     |
 | 61 | 4Sum                                        | Two Nested Loops + Two Pointers   | O(N^3)   | O(1)     |
 | 62 | Container With Most Water                   | Greedy Inward Two Pointers        | O(N)     | O(1)     |
 | 63 | Merge Intervals                             | Interval Sorting + Greedy Merge   | O(N logN)| O(1)     |
 | 64 | Non-overlapping Intervals                   | Greedy Interval Scheduling (End)  | O(N logN)| O(1)     |
 | 65 | Top K Frequent Elements                     | Frequency Map + Min-Heap Size K   | O(N logK)| O(N)     |
 | 66 | Kth Largest Element in an Array             | Min-Heap of Capacity K            | O(N logK)| O(K)     |
 | 67 | K Closest Points to Origin                  | Max-Heap of Size K                | O(N logK)| O(K)     |
 | 68 | Find Median From Data Stream                | Two Heaps (Balanced Max/Min)      | O(logN)/1| O(N)     |
 | 69 | Jump Game                                   | Greedy Max Reachable Index        | O(N)     | O(1)     |
 | 70 | Jump Game II                                | Greedy Window BFS (Farthest)      | O(N)     | O(1)     |
 | 71 | Gas Station                                 | Greedy Net Balance Invariant      | O(N)     | O(1)     |
 | 72 | Partition Labels                            | Greedy Last Occurrence Window     | O(N)     | O(1)     |
 | 73 | Subsets                                     | Backtracking Cascading Recursion  | O(N * 2^N)| O(N)    |
 | 74 | Permutations                                | In-Place Backtracking Swapping    | O(N * N!)| O(N)     |
 | 75 | Combination Sum                             | Backtracking Candidate Re-use     | O(2^T)   | O(T)     |
 | 76 | Word Search                                 | 2D Grid Backtracking Masking      | O(MN*3^L)| O(L)     |
 | 77 | Generate Parentheses                        | Backtracking Open/Close Balance   | O(4^N/rtN)| O(N)    |
 ====================================================================================================
*/
