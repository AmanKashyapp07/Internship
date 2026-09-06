#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <climits>
#include <cassert>
#include <utility>
#include <sstream>
#include <bitset>
#include <functional>
#endif
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;
using vl = vector<ll>;
using vvi = vector<vector<int>>;
using vvl = vector<vector<ll>>;

[[maybe_unused]] const ll MOD = 1e9 + 7;

/*
 ====================================================================================================
                                      PROBLEM SUMMARY & COMPLEXITY TABLE
 ====================================================================================================
 | #  | Problem Name                                | Pattern / Technique               | Time     | Space    |
 |----|---------------------------------------------|-----------------------------------|----------|----------|
 | 01 | Next Permutation                            | Suffix Inversion Pivot Swap       | O(N)     | O(1)     |
 | 02 | Jump Game II                                | Greedy Farthest Range Window      | O(N)     | O(1)     |
 | 03 | Merge Intervals                             | Sorting by Start + Overlap Merge  | O(N logN)| O(N)     |
 | 04 | Gas Station                                 | Greedy Net Tank Reset             | O(N)     | O(1)     |
 | 05 | Heaters                                     | Sorting + Binary Search Radius    | O(M logM)| O(1)     |
 | 06 | Task Scheduler                              | Max Frequency Frame Formula       | O(N)     | O(1)     |
 | 07 | Partition Labels                            | Greedy Last-Occurrence Window     | O(N)     | O(1)     |
 | 08 | Remove Covered Intervals                    | Sort (Start Asc, End Desc)        | O(N logN)| O(1)     |
 | 09 | Max Number of Events Attended               | Greedy Min-Heap by End Day        | O(N logN)| O(N)     |
 | 10 | Max Items From Sale                         | Greedy Effective Cost Sorting     | O(N logN)| O(N)     |
 | 11 | Longest Substring Without Repeating Chars   | Sliding Window + Last-Seen Map    | O(N)     | O(1)     |
 | 12 | Container With Most Water                   | Inward Converging Two Pointers    | O(N)     | O(1)     |
 | 13 | 3Sum                                        | Sorting + Two Pointers + Skip Dups| O(N^2)   | O(1)     |
 | 14 | 4Sum                                        | Generalized 2-Pointers + Skip     | O(N^3)   | O(1)     |
 | 15 | Trapping Rain Water                         | Two Pointers Running Max Enclosure| O(N)     | O(1)     |
 | 16 | Sort Colors                                 | Dutch National Flag 3-Way Part    | O(N)     | O(1)     |
 | 17 | Minimum Window Substring                    | Dynamic Window + Freq Matches     | O(N + M) | O(1)     |
 | 19 | Find All Anagrams in a String               | Fixed-Size Sliding Window Table   | O(N)     | O(1)     |
 | 20 | Permutation in String                       | Fixed-Size Window Freq Equality   | O(N)     | O(1)     |
 | 21 | Subarrays with K Different Integers         | Exact(K) = AtMost(K) - AtMost(K-1)| O(N)     | O(K)     |
 | 22 | Frequency of Most Frequent Element          | Sort + Sliding Window Budget      | O(N logN)| O(1)     |
 | 24 | Longest Consecutive Sequence                | Hash Set Boundary Expansion       | O(N)     | O(N)     |
 | 25 | Majority Element                            | Boyer-Moore Voting Algorithm      | O(N)     | O(1)     |
 | 26 | Majority Element II                         | Extended 2-Candidate Boyer-Moore  | O(N)     | O(1)     |
 | 31 | Set Mismatch                                | In-Place Sign Negation Marking    | O(N)     | O(1)     |
 ====================================================================================================
*/


// =========================================================
// 1. NEXT PERMUTATION (LC 31)
// =========================================================

class Solution1 {
public:
    void nextPermutation(vector<int>& nums) {
        int n = nums.size();
        int i = n - 2;
        // Step 1: Find first decreasing element from right
        while (i >= 0 && nums[i] >= nums[i + 1]) {
            i--;
        }
        // now i is the index of the first element that is smaller than its next element from the right
        // Step 2: Swap with the smallest element in suffix larger than nums[i]
        if (i >= 0) {
            int j = n - 1;
            while (nums[j] <= nums[i]) { // find the first element from the right that is greater than nums[i]
                j--;
            }
            swap(nums[i], nums[j]); // swap the found elements to get the next permutation
        }
        // Step 3: Reverse suffix to get lexicographically smallest order
        reverse(nums.begin() + i + 1, nums.end()); // reveser subarray [i+1, n-1] to get the next permutation
    }
};
// Interview Explanation:
// - Problem Statement: Find next lexicographically greater permutation in-place. If sorted descending, reverse to ascending.
// - Approach: Single-pass scan for pivot drop from the right, swap with next larger element, reverse suffix.
// - Intuition:
//   * A strictly descending suffix cannot be rearranged into a larger permutation.
//   * Find the first element from right that drops: nums[i] < nums[i + 1] (pivot at index i).
//   * Find smallest element in suffix larger than nums[i] to minimize the increase, then swap.
//   * Suffix remains descending; reversing subarray [i + 1, n - 1] makes it ascending (lexicographically minimal).
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 2. JUMP GAME II (LC 45)
// =========================================================

class Solution2 {
public:
    int jump(vector<int>& nums) {
        int n = nums.size();
        if (n <= 1) return 0;
        int jumps = 0;
        int currEnd = 0;
        int farthest = 0;
        // Greedy: Extend max reach within current window
        for (int i = 0; i < n - 1; ++i) {
            farthest = max(farthest, i + nums[i]);
            // Jump to next window when reaching end of current reach
            if (i == currEnd) {
                jumps++;
                currEnd = farthest;
                if (currEnd >= n - 1) break; // Early exit if we can reach or exceed the last index
            }
        }
        return jumps;
    }
};
// Interview Explanation:
// - Problem Statement: Return minimum number of jumps to reach the last index starting from index 0.
// - Approach: Greedy BFS / Window Reach Expansion.
// - Intuition:
//   * Maintain the current jump range [currStart, currEnd] and track farthest reach possible from within it.
//   * When iteration reaches currEnd, commit a jump (jumps++) and shift the boundary to currEnd = farthest.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 3. MERGE INTERVALS (LC 56)
// =========================================================

class Solution3 {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        if (intervals.empty()) return {};
        // Sort intervals by start time
        sort(intervals.begin(), intervals.end());
        vector<vector<int>> merged;
        for (const auto& interval : intervals) {
            // Append if no overlap; otherwise extend the current interval's end
            if (merged.empty() || merged.back()[1] < interval[0]) {
                merged.push_back(interval);
            } else {
                merged.back()[1] = max(merged.back()[1], interval[1]);
            }
        }
        return merged;
    }
};
// Interview Explanation:
// - Problem Statement: Given a collection of intervals, merge all overlapping intervals.
// - Approach: Sort intervals by start time + linear scan overlap merge.
// - Intuition:
//   * Sorting ensures all intervals that could possibly overlap are contiguous.
//   * If current interval starts after previous interval ends, append it as a new disjoint interval.
//   * Otherwise, overlap exists: update merged.back()[1] = max(merged.back()[1], interval[1]).
// - Complexity: Time: O(N log N), Space: O(N) to store merged output.


// =========================================================
// 4. GAS STATION (LC 134)
// =========================================================

class Solution4 {
public:
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
        int totalTank = 0;
        int currTank = 0;
        int start = 0;
        for (int i = 0; i < (int)gas.size(); ++i) {
            int diff = gas[i] - cost[i];
            totalTank += diff;
            currTank += diff;
            // If tank drops below zero, reset start to next station
            if (currTank < 0) {
                start = i + 1;
                currTank = 0;
            }
        }
        // If overall gas >= cost, the circuit can be completed
        return (totalTank >= 0) ? start : -1;
    }
};
// Interview Explanation:
// - Problem Statement: Find starting gas station index to travel clockwise around a circuit once, or -1 if impossible.
// - Approach: Greedy Net Gas Accumulation with Reset.
// - Intuition:
//   * If sum(gas) < sum(cost), totalTank < 0 and completing a full circuit is impossible.
//   * If currTank drops below 0 when traveling from start to i, no station between start and i can reach i + 1. Reset start = i + 1 and currTank = 0.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 5. HEATERS (LC 475)
// =========================================================

class Solution5 {
public:
    int findRadius(vector<int>& houses, vector<int>& heaters) {
        sort(heaters.begin(), heaters.end());
        int maxRadius = 0;
        for (int house : houses) {
            // Binary search nearest heaters to the right and left
            auto it = lower_bound(heaters.begin(), heaters.end(), house);
            int dist = INT_MAX;
            if (it != heaters.end()) {
                dist = min(dist, *it - house);
            }
            if (it != heaters.begin()) {
                dist = min(dist, house - *prev(it));
            }
            // Max radius needed across all houses
            maxRadius = max(maxRadius, dist);
        }
        return maxRadius;
    }
};
// Interview Explanation:
// - Problem Statement: Find minimum radius of heaters required to cover all houses.
// - Approach: Sort heaters + Binary Search (lower_bound) nearest heater per house.
// - Intuition:
//   * Each house needs to be covered by either the closest heater to its left or its right.
//   * Sort heaters. For each house, lower_bound finds the first heater with position >= house.
//   * Min distance for a house is min(rightHeater - house, house - leftHeater). Global answer is max over all houses.
// - Complexity: Time: O(M log M + N log M) where M = heaters.size(), N = houses.size(), Space: O(1) auxiliary space.


// =========================================================
// 6. TASK SCHEDULER (LC 621)
// =========================================================

class Solution6 {
public:
    int leastInterval(vector<char>& tasks, int n) {
        vector<int> freq(26, 0);
        int maxFreq = 0;
        // Count frequencies of tasks
        for (char c : tasks) {
            freq[c - 'A']++;
            maxFreq = max(maxFreq, freq[c - 'A']);
        }
        // Count how many tasks share the maximum frequency
        int maxCount = 0;
        for (int f : freq) {
            if (f == maxFreq) maxCount++;
        }
        // Frame formula: maxFreq blocks with n idle slots each
        int emptySlots = (maxFreq - 1) * (n + 1) + maxCount;
        return max((int)tasks.size(), emptySlots);
    }
};
// Interview Explanation:
// - Problem Statement: Find minimum intervals needed to execute all tasks with cooling interval n between identical tasks.
// - Approach: Max-Frequency Slot Framework Formula.
// - Intuition:
//   * The task with highest frequency maxFreq dictates the timeline skeleton.
//   * It forms (maxFreq - 1) frames, each requiring 1 task slot + n cooling slots = (n + 1) slots.
//   * The final frame needs exactly maxCount tasks (all tasks having frequency == maxFreq).
//   * If task variety is large, empty slots fill up with remaining tasks, taking tasks.size() total units.
// - Complexity: Time: O(N), Space: O(1) (fixed 26-element array).


// =========================================================
// 7. PARTITION LABELS (LC 763)
// =========================================================

class Solution7 {
public:
    vector<int> partitionLabels(string s) {
        vector<int> lastIndex(26, 0);
        // Record last occurrence index of each character
        for (int i = 0; i < (int)s.size(); ++i) {
            lastIndex[s[i] - 'a'] = i;
        }
        vector<int> result;
        int start = 0;
        int currEnd = 0;
        // Greedily expand current partition boundary
        for (int i = 0; i < (int)s.size(); ++i) {
            currEnd = max(currEnd, lastIndex[s[i] - 'a']);
            if (i == currEnd) {
                result.push_back(currEnd - start + 1);
                start = i + 1;
            }
        }
        return result;
    }
};
// Interview Explanation:
// - Problem Statement: Partition string into maximum number of parts so each letter appears in at most one part.
// - Approach: Greedy Last Occurrence Index + Window Boundary Expansion.
// - Intuition:
//   * Precompute the last seen index for each character in string s.
//   * As we iterate through s, extend the current partition end to max(currEnd, lastIndex[c]).
//   * When index i reaches currEnd, all characters seen in this window have concluded; cut the partition here.
// - Complexity: Time: O(N), Space: O(1) auxiliary space (26 characters).


// =========================================================
// 8. REMOVE COVERED INTERVALS (LC 1288)
// =========================================================

class Solution8 {
public:
    // Sort start ascending; break ties with end descending
    static bool cmp(const vector<int>& a, const vector<int>& b) {
        return a[0] != b[0] ? a[0] < b[0] : a[1] > b[1];
    }
    int removeCoveredIntervals(vector<vector<int>>& intervals) {
        sort(intervals.begin(), intervals.end(), cmp);
        int count = 0, maxEnd = 0;
        // Retain intervals that extend past the maximum seen end point
        for (const auto& interval : intervals) {
            if (interval[1] > maxEnd) {
                count++;
                maxEnd = interval[1];
            }
        }
        return count;
    }
};
// Interview Explanation:
// - Problem Statement: Count remaining intervals after removing all intervals covered by another interval in the set.
// - Approach: Sort (start asc, end desc) + Running Max End Comparison.
// - Intuition:
//   * Sorting start ascending guarantees earlier intervals can cover later ones.
//   * For matching starts, sorting end descending places the covering interval first.
//   * An interval is covered if its end <= maxEnd seen so far. If interval[1] > maxEnd, it survives.
// - Complexity: Time: O(N log N), Space: O(1) auxiliary space.


// =========================================================
// 9. MAXIMUM NUMBER OF EVENTS THAT CAN BE ATTENDED (LC 1353)
// =========================================================

class Solution9 {
public:
    int maxEvents(vector<vector<int>>& events) {
        sort(events.begin(), events.end());
        priority_queue<int, vector<int>, greater<int>> minHeap;
        int day = 1;
        int i = 0;
        int n = events.size();
        int attended = 0;
        while (i < n || !minHeap.empty()) {
            // Advance day if no active events in queue
            if (minHeap.empty()) {
                day = events[i][0];
            }
            // Add all events that start by today
            while (i < n && events[i][0] <= day) {
                minHeap.push(events[i][1]);
                i++;
            }
            // Discard expired events
            while (!minHeap.empty() && minHeap.top() < day) {
                minHeap.pop();
            }
            // Greedily attend the event ending earliest
            if (!minHeap.empty()) {
                minHeap.pop();
                attended++;
                day++;
            }
        }
        return attended;
    }
};
// Interview Explanation:
// - Problem Statement: Attend maximum number of non-overlapping 1-day events from a list of intervals [start, end].
// - Approach: Sorting + Greedy Min-Heap of Active Event End Days.
// - Intuition:
//   * On any day, from all available active events, attending the one that ends earliest leaves maximum flexibility for future events.
//   * Sort events by start day. Push end days of available events into a min-heap.
//   * Pop expired events (end < day). Attend the earliest expiring event, increment day.
// - Complexity: Time: O(N log N + D log N) where D is max days, Space: O(N) for min-heap.


// =========================================================
// 10. MAX ITEMS FROM SALE (MICROSOFT OA)
// =========================================================

class Solution10 {
public:
    int maxItemsFromSale(vector<int>& basePrices, vector<int>& discounts, int budget) {
        int n = basePrices.size();
        vector<int> effectiveCosts(n);
        // Calculate discounted prices
        for (int i = 0; i < n; ++i) {
            effectiveCosts[i] = max(0, basePrices[i] - discounts[i]);
        }
        // Greedily purchase cheapest items first
        sort(effectiveCosts.begin(), effectiveCosts.end());
        int count = 0;
        for (int cost : effectiveCosts) {
            if (budget >= cost) {
                budget -= cost;
                count++;
            } else {
                break;
            }
        }
        return count;
    }
};
// Interview Explanation:
// - Problem Statement: Find maximum items purchasable given base prices, discounts, and total budget.
// - Approach: Greedy Effective Cost Array Sorting.
// - Intuition:
//   * Each item i has effective price max(0, basePrices[i] - discounts[i]).
//   * To maximize the count of items acquired within a fixed budget, greedily purchase the cheapest items first.
//   * Sort effective prices ascending and deduct from budget until exhausted.
// - Complexity: Time: O(N log N), Space: O(N) to store effective costs.


// =========================================================
// 11. LONGEST SUBSTRING WITHOUT REPEATING CHARACTERS (LC 3)
// =========================================================

class Solution11 {
public:
    int lengthOfLongestSubstring(string s) {
        vector<int> lastIndex(256, -1);
        int left = 0;
        int maxLen = 0;
        // Sliding window: move left pointer past previous occurrence of s[right]
        for (int right = 0; right < (int)s.size(); ++right) {
            unsigned char c = s[right];
            if (lastIndex[c] >= left) {
                left = lastIndex[c] + 1;
            }
            lastIndex[c] = right;
            maxLen = max(maxLen, right - left + 1);
        }
        return maxLen;
    }
};
// Interview Explanation:
// - Problem Statement: Find length of longest substring without duplicate characters.
// - Approach: Variable-size Sliding Window with Direct Address Last-Seen Table.
// - Intuition:
//   * Expand window rightwards. Maintain lastIndex[c] storing the most recent index of character c.
//   * If character s[right] was seen inside current window (lastIndex[c] >= left), jump left pointer to lastIndex[c] + 1.
//   * Window [left..right] remains strictly duplicate-free at all times.
// - Complexity: Time: O(N), Space: O(1) (256 ASCII table).


// =========================================================
// 12. CONTAINER WITH MOST WATER (LC 11)
// =========================================================

class Solution12 {
public:
    int maxArea(vector<int>& height) {
        int left = 0;
        int right = (int)height.size() - 1;
        int maxWater = 0;
        // Two pointers: move inward from the shorter wall
        while (left < right) {
            int h = min(height[left], height[right]);
            int w = right - left;
            maxWater = max(maxWater, h * w);
            if (height[left] < height[right]) {
                left++;
            } else {
                right--;
            }
        }
        return maxWater;
    }
};
// Interview Explanation:
// - Problem Statement: Find two lines forming a container with the x-axis that stores the maximum water volume.
// - Approach: Two Pointers Inward Convergence.
// - Intuition:
//   * Area is bounded by min(height[left], height[right]) * (right - left).
//   * Inward movement reduces width by 1. To possibly achieve a larger area, height must increase.
//   * Moving the taller wall inward cannot increase area because height is capped by shorter wall. Thus, always advance the shorter wall inward.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 13. 3SUM (LC 15)
// =========================================================

class Solution13 {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        vector<vector<int>> result;
        int n = nums.size();
        for (int i = 0; i < n - 2; ++i) {
            // Skip duplicates for the first element
            if (i > 0 && nums[i] == nums[i - 1]) continue;
            if (nums[i] > 0) break; // Cannot sum to 0 if smallest is positive
            int left = i + 1;
            int right = n - 1;
            // Two pointers for two-sum target = -nums[i]
            while (left < right) {
                int sum = nums[i] + nums[left] + nums[right];
                if (sum == 0) {
                    result.push_back({nums[i], nums[left], nums[right]});
                    while (left < right && nums[left] == nums[left + 1]) left++;
                    while (left < right && nums[right] == nums[right - 1]) right--;
                    left++;
                    right--;
                } else if (sum < 0) {
                    left++;
                } else {
                    right--;
                }
            }
        }
        return result;
    }
};
// Interview Explanation:
// - Problem Statement: Find all unique triplets [nums[i], nums[j], nums[k]] summing to 0.
// - Approach: Sorting + Fixed Outer Loop + Two-Pointer Convergence with Duplicate Pruning.
// - Intuition:
//   * Sort array. Fix nums[i] as the first element.
//   * Use two pointers left = i + 1 and right = n - 1 to find pairs summing to -nums[i].
//   * Skip duplicate values for i, left, and right to guarantee unique triplets.
//   * If nums[i] > 0, sum cannot equal 0; break early.
// - Complexity: Time: O(N^2), Space: O(1) auxiliary space (excluding result).


// =========================================================
// 14. 4SUM (LC 18)
// =========================================================

class Solution14 {
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        sort(nums.begin(), nums.end());
        vector<vector<int>> result;
        int n = nums.size();
        for (int i = 0; i < n - 3; ++i) {
            if (i > 0 && nums[i] == nums[i - 1]) continue; // Skip duplicates for 1st num
            for (int j = i + 1; j < n - 2; ++j) {
                if (j > i + 1 && nums[j] == nums[j - 1]) continue; // Skip duplicates for 2nd num
                int left = j + 1;
                int right = n - 1;
                // Two pointers for remaining two numbers
                while (left < right) {
                    long long sum = (long long)nums[i] + nums[j] + nums[left] + nums[right];
                    if (sum == target) {
                        result.push_back({nums[i], nums[j], nums[left], nums[right]});
                        while (left < right && nums[left] == nums[left + 1]) left++;
                        while (left < right && nums[right] == nums[right - 1]) right--;
                        left++;
                        right--;
                    } else if (sum < target) {
                        left++;
                    } else {
                        right--;
                    }
                }
            }
        }
        return result;
    }
};
// Interview Explanation:
// - Problem Statement: Find all unique quadruplets summing to target.
// - Approach: Sorting + Two Nested Fixed Loops + Two-Pointer Convergence with Duplicate Pruning.
// - Intuition:
//   * Sort array. Fix outer two elements i and j, reducing problem to 2Sum for left and right.
//   * Use 64-bit integer arithmetic (long long) to prevent integer overflow when adding 4 numbers.
//   * Skip duplicates at every pointer level (i, j, left, right) to prevent duplicate quadruplets.
// - Complexity: Time: O(N^3), Space: O(1) auxiliary space (excluding result).

class Solution15 {
public:
    int trap(vector<int>& h) {
        stack<int> st;
        int water = 0;

        for (int i = 0; i < h.size(); i++) {
            while (!st.empty() && h[i] > h[st.top()]) {
                int mid = st.top();
                st.pop();

                if (st.empty()) break;

                int left = st.top();
                int width = i - left - 1;
                int boundedHeight = min(h[left], h[i]) - h[mid];

                water += width * boundedHeight;
            }
            st.push(i);
        }

        return water;
    }
};
// Interview Explanation:
// - Problem Statement: Calculate total trapped rain water on elevation map.
// - Approach: Two Pointers Inward Convergence with Running Maximums.
// - Intuition:
//   * Water trapped at index i is min(leftMax, rightMax) - height[i].
//   * If height[left] < height[right], trapped water at left is strictly bounded by leftMax because height[right] ensures a taller wall exists on the right.
//   * Thus, process left when height[left] < height[right]; otherwise process right.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 16. SORT COLORS (LC 75)
// =========================================================

class Solution16 {
public:
    void sortColors(vector<int>& nums) {
        int low = 0;
        int mid = 0;
        int high = (int)nums.size() - 1;
        // Dutch National Flag: 3-way partition (0s at low, 2s at high)
        while (mid <= high) {
            if (nums[mid] == 0) {
                swap(nums[low], nums[mid]);
                low++;
                mid++;
            } else if (nums[mid] == 1) {
                mid++;
            } else {
                swap(nums[mid], nums[high]);
                high--;
            }
        }
    }
};
// Interview Explanation:
// - Problem Statement: Sort an array with elements 0, 1, and 2 in-place in a single pass.
// - Approach: Dutch National Flag 3-Way Partitioning (Dijkstra).
// - Intuition:
//   * Invariant: [0..low-1] is 0s, [low..mid-1] is 1s, [high+1..n-1] is 2s, [mid..high] is unprocessed.
//   * When nums[mid] == 0: swap with nums[low], advance both low and mid.
//   * When nums[mid] == 1: advance mid.
//   * When nums[mid] == 2: swap with nums[high], decrement high without advancing mid (swapped value must be examined).
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 17. MINIMUM WINDOW SUBSTRING (LC 76)
// =========================================================

class Solution17 {
public:
    string minWindow(string s, string t) {
        vector<int> need(128, 0);
        int required = 0;
        // Count frequency of required characters
        for (char c : t) {
            if (need[(unsigned char)c] == 0) required++;
            need[(unsigned char)c]++;
        }
        vector<int> window(128, 0);
        int have = 0;
        int bestLen = INT_MAX;
        int bestStart = 0;
        int left = 0;
        // Expand window to the right
        for (int right = 0; right < (int)s.size(); ++right) {
            unsigned char c = s[right];
            window[c]++;
            if (need[c] > 0 && window[c] == need[c]) {
                have++;
            }
            // Contract window from left while condition is satisfied
            while (have == required) {
                if (right - left + 1 < bestLen) {
                    bestLen = right - left + 1;
                    bestStart = left;
                }
                unsigned char leftChar = s[left];
                window[leftChar]--;
                if (need[leftChar] > 0 && window[leftChar] < need[leftChar]) {
                    have--;
                }
                left++;
            }
        }
        return (bestLen == INT_MAX) ? "" : s.substr(bestStart, bestLen);
    }
};
// Interview Explanation:
// - Problem Statement: Find shortest substring in s that contains all characters of t (including duplicates).
// - Approach: Dynamic Sliding Window + Character Frequency Match Counter.
// - Intuition:
//   * need counts target characters; required is the number of distinct characters needed.
//   * Expand right: add s[right] to window. When window[c] == need[c], have++.
//   * When have == required, window is valid: record minimum length, then contract left to find minimal valid boundary.
// - Complexity: Time: O(|s| + |t|), Space: O(1) (fixed 128-element ASCII array).


// =========================================================
// 19. FIND ALL ANAGRAMS IN A STRING (LC 438)
// =========================================================

class Solution19 {
public:
    vector<int> findAnagrams(string s, string p) {
        int ns = s.size(), np = p.size();
        if (ns < np) return {};
        vector<int> pCount(26, 0), sCount(26, 0);
        for (char c : p) pCount[c - 'a']++;
        vector<int> result;
        // Fixed-size sliding window of length |p|
        for (int i = 0; i < ns; ++i) {
            sCount[s[i] - 'a']++;
            if (i >= np) {
                sCount[s[i - np] - 'a']--;
            }
            if (sCount == pCount) {
                result.push_back(i - np + 1);
            }
        }
        return result;
    }
};
// Interview Explanation:
// - Problem Statement: Find all start indices of p's anagrams in s.
// - Approach: Fixed-Size Sliding Window of Length |p| with Frequency Table.
// - Intuition:
//   * An anagram must have the identical length np and identical character frequencies as p.
//   * Slide a window of length np over s: add s[i] count, subtract s[i - np] count.
//   * Direct comparison sCount == pCount runs in O(26) = O(1) time. Record valid start index i - np + 1.
// - Complexity: Time: O(N), Space: O(1) auxiliary space (26-element vectors).


// =========================================================
// 20. PERMUTATION IN STRING (LC 567)
// =========================================================

class Solution20 {
public:
    bool checkInclusion(string s1, string s2) {
        int n1 = s1.size(), n2 = s2.size();
        if (n1 > n2) return false;
        vector<int> c1(26, 0), c2(26, 0);
        for (char c : s1) c1[c - 'a']++;
        // Sliding window of length |s1| over s2
        for (int i = 0; i < n2; ++i) {
            c2[s2[i] - 'a']++;
            if (i >= n1) {
                c2[s2[i - n1] - 'a']--;
            }
            if (c1 == c2) return true;
        }
        return false;
    }
};
// Interview Explanation:
// - Problem Statement: Determine whether s2 contains a permutation of s1 as a substring.
// - Approach: Fixed-Size Sliding Window of Length |s1| with Frequency Vector Comparison.
// - Intuition:
//   * A permutation of s1 is an exact anagram of length n1.
//   * Maintain rolling character counts across a window of length n1 in s2.
//   * Return true immediately if c1 == c2 at any step.
// - Complexity: Time: O(N2), Space: O(1) auxiliary space (26-element arrays).


// =========================================================
// 21. SUBARRAYS WITH K DIFFERENT INTEGERS (LC 992)
// =========================================================

class Solution21 {
    // Helper: count subarrays with at most k distinct elements
    int atMost(vector<int>& nums, int k) {
        unordered_map<int, int> freq;
        int left = 0, ans = 0;
        for (int right = 0; right < nums.size(); right++) {
            freq[nums[right]]++;
            while (freq.size() > k) {
                if (--freq[nums[left]] == 0)
                    freq.erase(nums[left]);
                left++;
            }
            ans += right - left + 1;
        }
        return ans;
    }
public:
    // Exactly(k) = atMost(k) - atMost(k - 1)
    int subarraysWithKDistinct(vector<int>& nums, int k) {
        return atMost(nums, k) - atMost(nums, k - 1);
    }
};
// Interview Explanation:
// - Problem Statement: Return number of subarrays containing exactly k distinct integers.
// - Approach: Sliding Window Difference: exactly(k) = atMost(k) - atMost(k - 1).
// - Intuition:
//   * Directly counting subarrays with exactly k distinct elements is non-monotonic and tricky with sliding windows.
//   * In contrast, counting subarrays with at most k distinct elements is monotonic: for each right pointer, valid left boundary can only move right.
//   * For any valid window [left..right] with <= k distinct elements, exactly (right - left + 1) subarrays end at right.
// - Complexity: Time: O(N), Space: O(K) for distinct element frequency map.


// =========================================================
// 22. FREQUENCY OF THE MOST FREQUENT ELEMENT (LC 1838)
// =========================================================

class Solution22 {
public:
    int maxFrequency(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        long long sum = 0;
        int left = 0, ans = 0;
        // Sliding window: operations needed to make [left..right] equal to nums[right]
        for (int right = 0; right < nums.size(); right++) {
            sum += nums[right];
            while (1LL * nums[right] * (right - left + 1) - sum > k)
                sum -= nums[left++];
            ans = max(ans, right - left + 1);
        }
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Find maximum frequency of any element after at most k increments of elements.
// - Approach: Sorting + Sliding Window Budget Check.
// - Intuition:
//   * Sort nums ascending. Target value in any window [left..right] should be nums[right].
//   * Total operations to elevate all elements in window to nums[right] is nums[right] * (right - left + 1) - sum.
//   * If required operations exceed budget k, shrink window by incrementing left.
//   * Window length right - left + 1 represents achievable frequency.
// - Complexity: Time: O(N log N), Space: O(1) auxiliary space.


// =========================================================
// 24. LONGEST CONSECUTIVE SEQUENCE (LC 128)
// =========================================================

class Solution24 {
public:
    int longestConsecutive(vector<int>& nums) {
        unordered_set<int> st(nums.begin(), nums.end());
        int ans = 0;
        for (int x : st) {
            // Only start count from the beginning of a sequence
            if (st.count(x - 1)) continue;
            int len = 1;
            while (st.count(x + len))
                len++;
            ans = max(ans, len);
        }
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Find length of longest consecutive elements sequence in unsorted array in O(N) time.
// - Approach: Hash Set Boundary Identification + Forward Streak Traversal.
// - Intuition:
//   * Insert all elements into an unordered_set.
//   * An element x can only initiate a consecutive streak if x - 1 is absent from the set.
//   * If x - 1 exists, skip x to avoid redundant O(N) streak traversals.
//   * This ensures each element is visited at most twice, achieving strictly O(N) linear time.
// - Complexity: Time: O(N), Space: O(N) for hash set storage.


// =========================================================
// 25. MAJORITY ELEMENT (LC 169)
// =========================================================

class Solution25 {
public:
    int majorityElement(vector<int>& nums) {
        int ans = 0, count = 0;
        // Boyer-Moore Voting: find candidate element appearing > n/2 times
        for (int x : nums) {
            if (count == 0) ans = x;
            count += (x == ans) ? 1 : -1;
        }
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Find majority element that appears strictly more than n / 2 times.
// - Approach: Boyer-Moore Voting Algorithm.
// - Intuition:
//   * Maintain a candidate ans and a balance counter count.
//   * When count == 0, adopt current element x as candidate.
//   * Pairwise cancellation: every instance of a non-majority element cancels out one instance of the majority element.
//   * Since majority element occurs > n / 2 times, its count remains > 0 at the end.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 26. MAJORITY ELEMENT II (LC 229)
// =========================================================

class Solution26 {
public:
    vector<int> majorityElementII(vector<int>& nums) {
        int a = 0, b = 1, ca = 0, cb = 0;
        // Boyer-Moore Voting: track up to 2 candidates appearing > n/3 times
        for (int x : nums) {
            if (x == a) ca++; // Increment count for candidate a
            else if (x == b) cb++; // Increment count for candidate b
            else if (ca == 0) a = x, ca = 1; // Adopt new candidate a
            else if (cb == 0) b = x, cb = 1; // Adopt new candidate b
            else ca--, cb--;
        }
        // Verify actual counts for both candidates
        ca = cb = 0;
        for (int x : nums) {
            if (x == a) ca++;
            else if (x == b) cb++;
        }
        vector<int> ans;
        if (ca > nums.size() / 3) ans.push_back(a);
        if (cb > nums.size() / 3) ans.push_back(b);
        return ans;
    }
};
// Interview Explanation:
// - Problem Statement: Find all elements appearing strictly more than n / 3 times.
// - Approach: Extended Boyer-Moore Voting Algorithm (2 Candidates).
// - Intuition:
//   * At most two numbers can have frequency strictly exceeding n / 3.
//   * Maintain two candidates (a, b) and counters (ca, cb). In pass 1, 3 distinct elements cancel out simultaneously.
//   * In pass 2, verify true occurrence counts for candidates a and b, collecting those with count > n / 3.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.


// =========================================================
// 31. SET MISMATCH (LC 645)
// =========================================================

class Solution31 {
public:
    vector<int> findErrorNums(vector<int>& nums) {
        int dup = -1, missing = -1;
        // In-place sign negation to find duplicate value
        for (int x : nums) {
            int i = abs(x) - 1; 
            if (nums[i] < 0) dup = abs(x);
            else nums[i] = -nums[i];
        }
        // Positive index + 1 identifies the missing value
        for (int i = 0; i < nums.size(); i++) {
            if (nums[i] > 0) {
                missing = i + 1;
                break;
            }
        }
        return {dup, missing};
    }
};
// Interview Explanation:
// - Problem Statement: Find the duplicate and missing numbers from an array containing numbers from 1 to n.
// - Approach: In-Place Sign Negation Index Marking.
// - Intuition:
//   * Map each value x to index abs(x) - 1. Negate the value at nums[abs(x) - 1] to mark it visited.
//   * If nums[abs(x) - 1] is already negative, abs(x) is the duplicate number.
//   * In second pass, whichever index i remains positive was never visited, indicating (i + 1) is the missing number.
// - Complexity: Time: O(N), Space: O(1) auxiliary space.
