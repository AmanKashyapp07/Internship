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
 | 1  | Non-overlapping Intervals (LC 435)          | Earliest End-Time Sorting         | O(N logN)| O(1)     |
 | 2  | Minimum Arrows to Burst Balloons (LC 452)   | End-Coordinate Interval Shooting  | O(N logN)| O(1)     |
 | 3  | Merge Intervals (LC 56)                     | Start-Time Sort + Linear Merge    | O(N logN)| O(N)     |
 | 4  | Insert Interval (LC 57)                     | 3-Phase Linear Partition & Merge  | O(N)     | O(N)     |
 | 5  | Meeting Rooms II (LC 253)                   | Two-Pointer Chronological Scan    | O(N logN)| O(N)     |
 | 6  | N Meetings in One Room (GFG)                | Greedy Earliest Finish Time       | O(N logN)| O(N)     |
 | 7  | Minimum Platforms for Railway (GFG)         | Event Chronological Sort (2 Ptr)  | O(N logN)| O(1)     |
 | 8  | Jump Game I (LC 55)                         | Max Reachable Index Tracker       | O(N)     | O(1)     |
 | 9  | Jump Game II (LC 45)                        | BFS Range Expansion [curEnd, fwd] | O(N)     | O(1)     |
 | 10 | Video Stitching (LC 1024)                   | Furthest Reachable Extension Jump | O(N logN)| O(1)     |
 | 11 | Min Taps to Water Garden (LC 1326)          | Interval Max Reach Jump Game II   | O(N)     | O(N)     |
 | 12 | Candy (LC 135)                              | 2-Pass Greedy Rating Neighbor Sync| O(N)     | O(N)     |
 | 13 | Gas Station / Circular Tour (LC 134)        | Net Surplus Sum + Tank Reset      | O(N)     | O(1)     |
 | 14 | Valid Parenthesis String '*' (LC 678)       | Min-Max Open Bracket Range Track  | O(N)     | O(1)     |
 | 15 | Min Deletions for Balanced String (LC 1653) | Running 'b' Count vs Deletion DP  | O(N)     | O(1)     |
 | 16 | Break a Palindrome (LC 1328)                | First Non-'a' Prefix Mutation     | O(N)     | O(1)     |
 | 17 | Partition Labels (LC 763)                   | Last-Seen Index Boundary Expansion| O(N)     | O(1)     |
 | 18 | Queue Reconstruction by Height (LC 406)     | Descending Height + K-Index Insert| O(N^2)   | O(N)     |
 | 19 | Assign Cookies (LC 455)                     | Greedy Two-Pointer Size Matching  | O(N logN)| O(1)     |
 | 20 | Boats to Save People (LC 881)               | Inward Converging Two Pointers    | O(N logN)| O(1)     |
 | 21 | Two City Scheduling (LC 1029)               | Cost Delta (costA - costB) Sorting| O(N logN)| O(1)     |
 | 22 | Bag of Tokens (LC 948)                      | Two Pointers: Buy Low, Sell High  | O(N logN)| O(1)     |
 | 23 | Task Scheduler (LC 621)                     | Max Frequency Frame Geometry      | O(N)     | O(1)     |
 | 24 | Reorganize String (LC 767)                  | Max-Heap Frequency Interleaving   | O(N logA)| O(A)     |
 | 25 | Largest Number (LC 179)                     | Custom String Concatenation Sort  | O(N logN)| O(N)     |
 | 26 | Min Deletions for Unique Freqs (LC 1647)    | Hash Set Seen Frequency Decrement | O(N)     | O(1)     |
 | 27 | Min Swaps to Make Strings Equal (LC 1247)   | XY and YX Mismatch Pair Counting  | O(N)     | O(1)     |
 | 28 | Shortest Unsorted Subarray (LC 581)         | Running Prefix Max & Suffix Min   | O(N)     | O(1)     |
 | 29 | Fractional Knapsack (GFG)                   | Value-to-Weight Ratio Sorting     | O(N logN)| O(1)     |
 | 30 | Job Sequencing with Deadlines (GFG)         | Profit Sorting + Slot Allocation  | O(N^2)   | O(MaxD)  |
 | 31 | Lemonade Change (LC 860)                    | Greedy Bill Register Tracking     | O(N)     | O(1)     |
 | 32 | Maximum Units on a Truck (LC 1710)          | Units-Per-Box Descending Greedy   | O(N logN)| O(1)     |
 | 33 | Wiggle Subsequence (LC 376)                 | Local Peak & Valley Count Tracker | O(N)     | O(1)     |
 | 34 | Max Bags With Full Rocks (LC 2279)          | Capacity Deficit Sorting          | O(N logN)| O(N)     |
 | 35 | Course Schedule III (LC 630)                | Deadline Sort + Max-Heap Regret   | O(N logN)| O(N)     |
 | 36 | IPO - Maximize Capital (LC 502)             | Min-Heap Capital + Max-Heap Profit| O(N logN)| O(N)     |
 | 37 | Connect Sticks / Huffman (LC 1167)          | Min-Heap Pairwise Merge Reduction | O(N logN)| O(N)     |
 | 38 | Advantage Shuffle (LC 870)                  | Tian Ji Horse Racing Two Pointers | O(N logN)| O(N)     |
 | 39 | Min Increments to Form Target (LC 1526)     | Positive Delta Level Accumulation | O(N)     | O(1)     |
 ====================================================================================================
*/


class Solution {
public:

    // =========================================================
    // 1. INTERVAL SCHEDULING & OVERLAPS
    // =========================================================

    // Non-overlapping Intervals (LeetCode 435)
    int eraseOverlapIntervals(vector<vector<int>>& intervals) {
        if (intervals.empty()) return 0;
        // Sort by end time: Earliest finishing interval leaves maximum room for future intervals
        sort(intervals.begin(), intervals.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[1] < b[1];
        });

        int nonOverlapCount = 1;
        int lastEnd = intervals[0][1];

        for (int i = 1; i < (int)intervals.size(); i++) {
            if (intervals[i][0] >= lastEnd) {
                nonOverlapCount++;
                lastEnd = intervals[i][1];
            }
        }
        return (int)intervals.size() - nonOverlapCount;
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum number of intervals to remove to make the rest non-overlapping (LeetCode 435).
    // - Approach: Classic Activity Selection / Greedy sort by end time.
    // - Intuition: Picking the interval that finishes earliest leaves the maximal time available for remaining intervals.
    // - Complexity: Time: O(N log N) sorting, Space: O(1) auxiliary.

    // Minimum Number of Arrows to Burst Balloons (LeetCode 452)
    int findMinArrowShots(vector<vector<int>>& points) {
        if (points.empty()) return 0;
        sort(points.begin(), points.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[1] < b[1];
        });

        int arrows = 1;
        int arrowPos = points[0][1];

        for (int i = 1; i < (int)points.size(); i++) {
            if (points[i][0] > arrowPos) {
                arrows++;
                arrowPos = points[i][1];
            }
        }
        return arrows;
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum number of vertical arrows to burst all balloons given [x_start, x_end] intervals (LeetCode 452).
    // - Approach: Greedy sort by end coordinate.
    // - Intuition: Shoot arrow at the earliest balloon end coordinate; all overlapping balloons covering this position are burst simultaneously.
    // - Complexity: Time: O(N log N), Space: O(1).

    // Merge Intervals (LeetCode 56)
    vector<vector<int>> mergeIntervals(vector<vector<int>>& intervals) {
        if (intervals.empty()) return {};
        sort(intervals.begin(), intervals.end());

        vector<vector<int>> merged;
        merged.push_back(intervals[0]);

        for (int i = 1; i < (int)intervals.size(); i++) {
            if (intervals[i][0] <= merged.back()[1]) {
                merged.back()[1] = max(merged.back()[1], intervals[i][1]);
            } else {
                merged.push_back(intervals[i]);
            }
        }
        return merged;
    }
    // Interview Explanation:
    // - Problem Statement: Merge all overlapping intervals into non-overlapping contiguous intervals (LeetCode 56).
    // - Approach: Sort by start time + Linear merged extension.
    // - Intuition: Once sorted by start time, interval i overlaps with current merged interval if `start[i] <= end[merged]`.
    // - Complexity: Time: O(N log N), Space: O(N) output storage.

    // Insert Interval (LeetCode 57)
    vector<vector<int>> insertInterval(vector<vector<int>>& intervals, vector<int>& newInterval) {
        vector<vector<int>> result;
        int i = 0, n = intervals.size();

        // 1. Add all intervals ending before newInterval starts
        while (i < n && intervals[i][1] < newInterval[0]) {
            result.push_back(intervals[i++]);
        }

        // 2. Merge all overlapping intervals with newInterval
        while (i < n && intervals[i][0] <= newInterval[1]) {
            newInterval[0] = min(newInterval[0], intervals[i][0]);
            newInterval[1] = max(newInterval[1], intervals[i][1]);
            i++;
        }
        result.push_back(newInterval);

        // 3. Add all intervals starting after newInterval ends
        while (i < n) {
            result.push_back(intervals[i++]);
        }
        return result;
    }
    // Interview Explanation:
    // - Problem Statement: Insert newInterval into sorted non-overlapping intervals and merge if necessary (LeetCode 57).
    // - Approach: 3-Stage Linear Scan (Left non-overlapping, Middle overlapping merge, Right non-overlapping).
    // - Intuition: Since input is already sorted, process strictly in O(N) linear time without sorting.
    // - Complexity: Time: O(N), Space: O(N) output.

    // Meeting Rooms II / Min Conference Rooms (LeetCode 253 / GFG)
    int minMeetingRooms(vector<vector<int>>& intervals) {
        if (intervals.empty()) return 0;
        int n = intervals.size();
        vector<int> startTimes(n), endTimes(n);
        for (int i = 0; i < n; i++) {
            startTimes[i] = intervals[i][0];
            endTimes[i] = intervals[i][1];
        }
        sort(startTimes.begin(), startTimes.end());
        sort(endTimes.begin(), endTimes.end());

        int rooms = 0, endPtr = 0;
        for (int i = 0; i < n; i++) {
            if (startTimes[i] < endTimes[endPtr]) {
                rooms++; // Need new room
            } else {
                endPtr++; // Reuse freed room
            }
        }
        return rooms;
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum number of conference rooms required for all meetings (LeetCode 253).
    // - Approach: Two-Pointer Chronological Sweep (or Min-Heap for end times).
    // - Intuition: If a meeting starts before the earliest ending meeting finishes (`start[i] < end[endPtr]`), allocate a new room; otherwise reuse the freed room by advancing `endPtr`.
    // - Complexity: Time: O(N log N) sorting, Space: O(N) time arrays.

    // N Meetings in One Room (GFG / Striver SDE #43)
    int maxMeetings(vector<int>& start, vector<int>& end, int n) {
        vector<pair<int, int>> meetings(n);
        for (int i = 0; i < n; i++) meetings[i] = {end[i], start[i]};
        sort(meetings.begin(), meetings.end()); // sort by end time

        int count = 1, lastEnd = meetings[0].first;
        for (int i = 1; i < n; i++) {
            if (meetings[i].second > lastEnd) {
                count++;
                lastEnd = meetings[i].first;
            }
        }
        return count;
    }
    // Interview Explanation:
    // - Problem Statement: Find maximum meetings that can be accommodated in a single room (GFG / Striver SDE).
    // - Approach: Sort by end time and greedily choose non-overlapping meetings.
    // - Complexity: Time: O(N log N), Space: O(N).

    // Minimum Platforms for Railway Station (GFG / Striver SDE #44)
    int findPlatform(vector<int>& arr, vector<int>& dep, int n) {
        sort(arr.begin(), arr.end());
        sort(dep.begin(), dep.end());

        int platforms = 1, maxPlatforms = 1;
        int i = 1, j = 0;

        while (i < n && j < n) {
            if (arr[i] <= dep[j]) {
                platforms++;
                i++;
            } else {
                platforms--;
                j++;
            }
            maxPlatforms = max(maxPlatforms, platforms);
        }
        return maxPlatforms;
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum platforms needed at a railway station so no train waits (GFG / Striver SDE).
    // - Approach: Two-Pointer Chronological Sweep over sorted arrival and departure times.
    // - Intuition: When a train arrives before previous departs (`arr[i] <= dep[j]`), we need +1 platform; when a train departs, -1 platform.
    // - Complexity: Time: O(N log N), Space: O(1).


    // =========================================================
    // 2. JUMP GAME & REACHABILITY GREEDY
    // =========================================================

    // Jump Game I (LeetCode 55)
    bool canJump(vector<int>& nums) {
        int maxReach = 0, n = nums.size();
        for (int i = 0; i < n; i++) {
            if (i > maxReach) return false;
            maxReach = max(maxReach, i + nums[i]);
            if (maxReach >= n - 1) return true;
        }
        return true;
    }
    // Interview Explanation:
    // - Problem Statement: Determine if you can reach the last index starting at index 0 (LeetCode 55).
    // - Approach: Greedy Max Reachable Index tracking.
    // - Intuition: At each step, update `maxReach = max(maxReach, i + nums[i])`. If current index exceeds `maxReach`, remaining positions are unreachable.
    // - Complexity: Time: O(N) single pass, Space: O(1).

    // Jump Game II (LeetCode 45)
    int jump(vector<int>& nums) {
        int n = nums.size();
        if (n <= 1) return 0;
        int jumps = 0, currEnd = 0, farthest = 0;
        for (int i = 0; i < n - 1; i++) {
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
    // - Problem Statement: Find minimum number of jumps to reach the last index (LeetCode 45).
    // - Approach: Greedy BFS Window Exploration.
    // - Intuition: Maintain `currEnd` as current jump range. When `i` reaches `currEnd`, we must increment jumps and update `currEnd = farthest`.
    // - Complexity: Time: O(N), Space: O(1).

    // Video Stitching (LeetCode 1024)
    int videoStitching(vector<vector<int>>& clips, int time) {
        vector<int> maxReach(time + 1, 0);
        for (auto& clip : clips) {
            if (clip[0] <= time) {
                maxReach[clip[0]] = max(maxReach[clip[0]], clip[1]);
            }
        }

        int jumps = 0, currEnd = 0, farthest = 0;
        for (int i = 0; i < time; i++) {
            farthest = max(farthest, maxReach[i]);
            if (i == currEnd) {
                jumps++;
                currEnd = farthest;
                if (currEnd <= i) return -1; // Cannot advance
            }
        }
        return jumps;
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum clips needed to cover interval [0, time] (LeetCode 1024).
    // - Approach: Reduction to Jump Game II.
    // - Intuition: Precompute maximum right reach for each start second; run greedy BFS boundary expansion.
    // - Complexity: Time: O(N + time), Space: O(time).

    // Minimum Number of Taps to Open to Water a Garden (LeetCode 1326)
    int minTaps(int n, vector<int>& ranges) {
        vector<int> maxReach(n + 1, 0);
        for (int i = 0; i <= n; i++) {
            int left = max(0, i - ranges[i]);
            int right = min(n, i + ranges[i]);
            maxReach[left] = max(maxReach[left], right);
        }

        int taps = 0, currEnd = 0, farthest = 0;
        for (int i = 0; i < n; i++) {
            farthest = max(farthest, maxReach[i]);
            if (i == currEnd) {
                taps++;
                currEnd = farthest;
                if (currEnd <= i) return -1; // Stuck
            }
        }
        return taps;
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum taps to water garden from 0 to n (LeetCode 1326).
    // - Approach: Interval Mapping to Jump Game II BFS window expansion.
    // - Intuition: Map each tap to interval `[max(0, i-ranges[i]), min(n, i+ranges[i])]`; update `maxReach[left] = max(maxReach[left], right)` and jump greedily.
    // - Complexity: Time: O(N), Space: O(N).


    // =========================================================
    // 3. TWO-PASS & BALANCE GREEDY
    // =========================================================

    // Candy (LeetCode 135)
    int candy(vector<int>& ratings) {
        int n = ratings.size();
        if (n == 0) return 0;
        vector<int> candies(n, 1);

        // Pass 1: Left to Right (higher rating than left neighbor gets more)
        for (int i = 1; i < n; i++) {
            if (ratings[i] > ratings[i - 1]) {
                candies[i] = candies[i - 1] + 1;
            }
        }

        // Pass 2: Right to Left (higher rating than right neighbor gets max)
        for (int i = n - 2; i >= 0; i--) {
            if (ratings[i] > ratings[i + 1]) {
                candies[i] = max(candies[i], candies[i + 1] + 1);
            }
        }

        return accumulate(candies.begin(), candies.end(), 0);
    }
    // Interview Explanation:
    // - Problem Statement: Distribute minimum candies such that each child has >= 1 and higher rated neighbor gets more (LeetCode 135).
    // - Approach: Two-Pass Greedy Scan.
    // - Intuition: Left-to-right pass satisfies left constraints; right-to-left pass satisfies right constraints while preserving left constraints via `max()`.
    // - Complexity: Time: O(N), Space: O(N).

    // Gas Station / Circular Tour (LeetCode 134)
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
        int totalTank = 0, currTank = 0, startIdx = 0;
        for (int i = 0; i < (int)gas.size(); i++) {
            int net = gas[i] - cost[i];
            totalTank += net;
            currTank += net;
            if (currTank < 0) {
                // Cannot start from any station in [startIdx..i]
                startIdx = i + 1;
                currTank = 0;
            }
        }
        return totalTank >= 0 ? startIdx : -1;
    }
    // Interview Explanation:
    // - Problem Statement: Find starting gas station to complete circular circuit once clockwise (LeetCode 134).
    // - Approach: Single Pass Tank Balance Greedy.
    // - Intuition: If `totalGas < totalCost`, impossible. If starting at A fails at B, NO station between A and B can reach B (all accumulated positive gas is spent); reset `startIdx = B + 1`.
    // - Complexity: Time: O(N), Space: O(1).

    // Valid Parenthesis String with '*' (LeetCode 678)
    bool checkValidString(string s) {
        int minOpen = 0, maxOpen = 0;
        for (char c : s) {
            if (c == '(') {
                minOpen++;
                maxOpen++;
            } else if (c == ')') {
                minOpen = max(0, minOpen - 1);
                maxOpen--;
            } else { // '*' can be '(', ')', or ''
                minOpen = max(0, minOpen - 1); // treat as ')'
                maxOpen++;                     // treat as '('
            }
            if (maxOpen < 0) return false; // Too many ')'
        }
        return minOpen == 0;
    }
    // Interview Explanation:
    // - Problem Statement: Check if parenthesis string is valid where '*' can be '(', ')', or empty (LeetCode 678).
    // - Approach: Greedy Range Tracking `[minOpen, maxOpen]` of possible open brackets.
    // - Intuition: Maintain minimum and maximum possible unclosed '(' brackets. String is valid if range includes 0 at end (`minOpen == 0`).
    // - Complexity: Time: O(N), Space: O(1).

    // Minimum Deletions to Make String Balanced (LeetCode 1653)
    int minimumDeletions(string s) {
        int bCount = 0, deletions = 0;
        for (char c : s) {
            if (c == 'b') {
                bCount++;
            } else {
                // When seeing 'a', either delete this 'a' (+1 deletion) or keep it and delete all previous 'b's
                deletions = min(deletions + 1, bCount);
            }
        }
        return deletions;
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum deletions to make string balanced with no 'b' before 'a' (LeetCode 1653).
    // - Approach: One-Pass Greedy / DP Counter.
    // - Intuition: At each 'a', choose minimum between deleting current 'a' or deleting all preceding 'b's.
    // - Complexity: Time: O(N), Space: O(1).

    // Break a Palindrome (LeetCode 1328)
    string breakPalindrome(string palindrome) {
        int n = palindrome.size();
        if (n <= 1) return "";
        for (int i = 0; i < n / 2; i++) {
            if (palindrome[i] != 'a') {
                palindrome[i] = 'a';
                return palindrome;
            }
        }
        palindrome[n - 1] = 'b'; // All are 'a's, replace last character with 'b'
        return palindrome;
    }
    // Interview Explanation:
    // - Problem Statement: Replace 1 character in palindrome to form lexicographically smallest non-palindrome (LeetCode 1328).
    // - Approach: Greedy Left-to-Right Scan.
    // - Intuition: Replace first non-'a' character in the first half with 'a'. If all first-half characters are 'a', replace the very last character with 'b'.
    // - Complexity: Time: O(N), Space: O(1).


    // =========================================================
    // 4. GREEDY MATCHING, SORTING & TWO POINTERS
    // =========================================================

    // Partition Labels (LeetCode 763)
    vector<int> partitionLabels(string s) {
        vector<int> lastIndex(26, 0);
        for (int i = 0; i < (int)s.size(); i++) lastIndex[s[i] - 'a'] = i;

        vector<int> partitions;
        int start = 0, end = 0;
        for (int i = 0; i < (int)s.size(); i++) {
            end = max(end, lastIndex[s[i] - 'a']);
            if (i == end) {
                partitions.push_back(end - start + 1);
                start = i + 1;
            }
        }
        return partitions;
    }
    // Interview Explanation:
    // - Problem Statement: Partition string into maximum parts such that each letter appears in at most one part (LeetCode 763).
    // - Approach: Last-occurrence Index Map + Greedy Boundary Expansion.
    // - Intuition: For current partition, extend boundary `end = max(end, last[c])`. When `i == end`, partition is complete.
    // - Complexity: Time: O(N), Space: O(1) 26-element array.

    // Queue Reconstruction by Height (LeetCode 406)
    vector<vector<int>> reconstructQueue(vector<vector<int>>& people) {
        // Sort descending by height h; if equal, sort ascending by k
        sort(people.begin(), people.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[0] == b[0] ? a[1] < b[1] : a[0] > b[0];
        });

        vector<vector<int>> queue;
        for (auto& p : people) {
            queue.insert(queue.begin() + p[1], p); // Insert at index k
        }
        return queue;
    }
    // Interview Explanation:
    // - Problem Statement: Reconstruct queue given [h, k] where k is count of taller/equal people in front (LeetCode 406).
    // - Approach: Greedy Height-Descending Sort + Direct Index Insertion.
    // - Intuition: Tallest people placed first establish framework; shorter people inserted at index k do not disrupt taller peoples' k-counts.
    // - Complexity: Time: O(N^2) insertions, Space: O(N).

    // Assign Cookies (LeetCode 455)
    int findContentChildren(vector<int>& g, vector<int>& s) {
        sort(g.begin(), g.end());
        sort(s.begin(), s.end());
        int child = 0, cookie = 0;
        while (child < (int)g.size() && cookie < (int)s.size()) {
            if (s[cookie] >= g[child]) {
                child++; // Satisfied child
            }
            cookie++;
        }
        return child;
    }
    // Interview Explanation:
    // - Problem Statement: Maximize number of children satisfied with cookie sizes s[j] >= greed factor g[i] (LeetCode 455).
    // - Approach: Two Pointers on Sorted Arrays.
    // - Intuition: Greedily satisfy child with smallest greed factor using smallest viable cookie.
    // - Complexity: Time: O(N log N + M log M), Space: O(1).

    // Boats to Save People (LeetCode 881)
    int numRescueBoats(vector<int>& people, int limit) {
        sort(people.begin(), people.end());
        int left = 0, right = (int)people.size() - 1, boats = 0;
        while (left <= right) {
            if (people[left] + people[right] <= limit) {
                left++; // Lightest person shares boat with heaviest
            }
            right--; // Heaviest person always takes boat
            boats++;
        }
        return boats;
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum boats to carry people (at most 2 people per boat, max weight `limit`) (LeetCode 881).
    // - Approach: Inward Two-Pointer Greedy Matching.
    // - Intuition: Heaviest person must always ride; pair with lightest person if sum <= limit.
    // - Complexity: Time: O(N log N), Space: O(1).

    // Two City Scheduling (LeetCode 1029)
    int twoCitySchedCost(vector<vector<int>>& costs) {
        // Sort by refund/benefit of flying to A vs B (costs[i][0] - costs[i][1])
        sort(costs.begin(), costs.end(), [](const vector<int>& a, const vector<int>& b) {
            return (a[0] - a[1]) < (b[0] - b[1]);
        });

        int totalCost = 0, n = costs.size() / 2;
        for (int i = 0; i < n; i++) totalCost += costs[i][0];       // First N to City A
        for (int i = n; i < 2 * n; i++) totalCost += costs[i][1];   // Remaining N to City B
        return totalCost;
    }
    // Interview Explanation:
    // - Problem Statement: Fly 2N people to City A or City B (N to each) minimizing total cost (LeetCode 1029).
    // - Approach: Greedy Sort by Price Differential `costA - costB`.
    // - Intuition: Smallest `costA - costB` gives the largest cost savings for choosing City A over City B.
    // - Complexity: Time: O(N log N), Space: O(1).

    // Bag of Tokens (LeetCode 948)
    int bagOfTokensScore(vector<int>& tokens, int power) {
        sort(tokens.begin(), tokens.end());
        int left = 0, right = (int)tokens.size() - 1, score = 0, maxScore = 0;

        while (left <= right) {
            if (power >= tokens[left]) {
                power -= tokens[left++];
                score++;
                maxScore = max(maxScore, score);
            } else if (score > 0) {
                power += tokens[right--];
                score--;
            } else {
                break;
            }
        }
        return maxScore;
    }
    // Interview Explanation:
    // - Problem Statement: Maximize token score given initial power (play face-up for +1 score, face-down for power) (LeetCode 948).
    // - Approach: Two-Pointer Buy-Low Sell-High Greedy.
    // - Intuition: Greedily buy score with smallest token; trade 1 score to gain largest possible token power.
    // - Complexity: Time: O(N log N), Space: O(1).

    // Task Scheduler (LeetCode 621)
    int leastInterval(vector<char>& tasks, int n) {
        vector<int> freq(26, 0);
        for (char t : tasks) freq[t - 'A']++;
        sort(freq.rbegin(), freq.rend());

        int maxFreq = freq[0];
        int maxCount = 0;
        for (int f : freq) if (f == maxFreq) maxCount++;

        int calculatedTime = (maxFreq - 1) * (n + 1) + maxCount;
        return max((int)tasks.size(), calculatedTime);
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum CPU intervals to execute tasks with cooldown n between identical tasks (LeetCode 621).
    // - Approach: Mathematical Max Frequency Frame Greedy.
    // - Intuition: The task with maximum frequency dictates the minimum frame size `(maxFreq - 1) * (n + 1) + maxCount`. If total tasks exceed frame, answer is `tasks.size()`.
    // - Complexity: Time: O(N) single pass, Space: O(1) 26-element array.

    // Reorganize String (LeetCode 767)
    string reorganizeString(string s) {
        vector<int> freq(26, 0);
        for (char c : s) freq[c - 'a']++;

        int maxFreq = 0, letter = 0, n = s.size();
        for (int i = 0; i < 26; i++) {
            if (freq[i] > maxFreq) {
                maxFreq = freq[i];
                letter = i;
            }
        }
        if (maxFreq > (n + 1) / 2) return ""; // Impossible to separate

        string res(n, ' ');
        int idx = 0;

        while (freq[letter] > 0) {
            res[idx] = (char)('a' + letter);
            idx += 2;
            freq[letter]--;
        }

        for (int i = 0; i < 26; i++) {
            while (freq[i] > 0) {
                if (idx >= n) idx = 1; // wrap to odd indices
                res[idx] = (char)('a' + i);
                idx += 2;
                freq[i]--;
            }
        }
        return res;
    }
    // Interview Explanation:
    // - Problem Statement: Rearrange string so that no two adjacent characters are identical (LeetCode 767).
    // - Approach: Greedy Even-Odd Index Interleaving.
    // - Intuition: Fill the most frequent character at even indices (0, 2, 4...) first; filling remaining letters guarantees no identical adjacent characters.
    // - Complexity: Time: O(N), Space: O(1) 26-element array.

    // Largest Number (LeetCode 179)
    string largestNumber(vector<int>& nums) {
        vector<string> sNums;
        for (int x : nums) sNums.push_back(to_string(x));

        sort(sNums.begin(), sNums.end(), [](const string& a, const string& b) {
            return a + b > b + a;
        });

        if (sNums[0] == "0") return "0";

        string result = "";
        for (const string& s : sNums) result += s;
        return result;
    }
    // Interview Explanation:
    // - Problem Statement: Arrange list of non-negative integers to form the largest possible number (LeetCode 179).
    // - Approach: Custom Concatenation Comparator Sort (`a + b > b + a`).
    // - Intuition: Transitivity of concatenation ordering guarantees global optimal placement.
    // - Complexity: Time: O(N log N * L) where L is max string length, Space: O(N).

    // Minimum Deletions to Make Character Frequencies Unique (LeetCode 1647)
    int minDeletions(string s) {
        vector<int> freq(26, 0);
        for (char c : s) freq[c - 'a']++;

        unordered_set<int> seen;
        int deletions = 0;

        for (int f : freq) {
            while (f > 0 && seen.count(f)) {
                f--;
                deletions++;
            }
            if (f > 0) seen.insert(f);
        }
        return deletions;
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum character deletions so no two characters have equal positive frequencies (LeetCode 1647).
    // - Approach: Greedy Frequency Decrement with Hash Set.
    // - Intuition: If frequency already exists in `seen`, decrement until an unused frequency (or 0) is found.
    // - Complexity: Time: O(N + 26^2), Space: O(1).

    // Minimum Swaps to Make Strings Equal (LeetCode 1247)
    int minimumSwap(string s1, string s2) {
        int xy = 0, yx = 0;
        for (int i = 0; i < (int)s1.size(); i++) {
            if (s1[i] == 'x' && s2[i] == 'y') xy++;
            else if (s1[i] == 'y' && s2[i] == 'x') yx++;
        }
        if ((xy + yx) % 2 != 0) return -1; // Odd mismatches cannot be resolved
        // 2 xy pairs resolved in 1 swap; 2 yx pairs resolved in 1 swap; remaining 1 xy + 1 yx resolved in 2 swaps
        return xy / 2 + yx / 2 + (xy % 2) * 2;
    }
    // Interview Explanation:
    // - Problem Statement: Minimum swaps between s1 and s2 to make them equal (only 'x' and 'y') (LeetCode 1247).
    // - Approach: Greedy Mismatch Pair Resolution.
    // - Intuition: Pair matching mismatches `(xy, xy) -> 1 swap`, `(yx, yx) -> 1 swap`, `(xy, yx) -> 2 swaps`.
    // - Complexity: Time: O(N), Space: O(1).


    // =========================================================
    // 5. GREEDY SUBARRAY, FRACTIONAL KNAPSACK & DEADLINES
    // =========================================================

    // Maximum Subarray (Kadane's Algorithm - LeetCode 53)
    int maxSubArray(vector<int>& nums) {
        int currentSum = 0, maxSum = INT_MIN;
        for (int x : nums) {
            currentSum += x;
            maxSum = max(maxSum, currentSum);
            if (currentSum < 0) currentSum = 0;
        }
        return maxSum;
    }
    // Interview Explanation:
    // - Problem Statement: Find contiguous subarray with the largest sum (LeetCode 53).
    // - Approach: Kadane's Greedy Prefix Discard Algorithm.
    // - Intuition: A prefix with negative sum can never contribute positively to any subsequent subarray sum; discard it immediately.
    // - Complexity: Time: O(N), Space: O(1).

    // Shortest Unsorted Continuous Subarray (LeetCode 581)
    int findUnsortedSubarray(vector<int>& nums) {
        int n = nums.size();
        int maxSeen = INT_MIN, minSeen = INT_MAX;
        int right = -1, left = -1;

        // Left-to-right pass: find rightmost element smaller than maxSeen
        for (int i = 0; i < n; i++) {
            maxSeen = max(maxSeen, nums[i]);
            if (nums[i] < maxSeen) right = i;
        }

        // Right-to-left pass: find leftmost element greater than minSeen
        for (int i = n - 1; i >= 0; i--) {
            minSeen = min(minSeen, nums[i]);
            if (nums[i] > minSeen) left = i;
        }

        return right == -1 ? 0 : (right - left + 1);
    }
    // Interview Explanation:
    // - Problem Statement: Find shortest subarray that when sorted makes whole array sorted (LeetCode 581).
    // - Approach: Two-Pass Greedy Boundary Identification.
    // - Intuition: Any element < `maxSeenSoFar` is out of order (gives right boundary); any element > `minSeenSoFar` from right gives left boundary.
    // - Complexity: Time: O(N), Space: O(1).

    // Fractional Knapsack (GFG / Striver SDE #45)
    struct Item { int value, weight; };
    double fractionalKnapsack(int W, Item arr[], int n) {
        sort(arr, arr + n, [](const Item& a, const Item& b) {
            double r1 = (double)a.value / a.weight;
            double r2 = (double)b.value / b.weight;
            return r1 > r2;
        });

        double totalValue = 0.0;
        int currentWeight = 0;

        for (int i = 0; i < n; i++) {
            if (currentWeight + arr[i].weight <= W) {
                currentWeight += arr[i].weight;
                totalValue += arr[i].value;
            } else {
                int remain = W - currentWeight;
                totalValue += ((double)arr[i].value / arr[i].weight) * remain;
                break;
            }
        }
        return totalValue;
    }
    // Interview Explanation:
    // - Problem Statement: Fractional Knapsack — maximize value where items can be broken down.
    // - Approach: Greedy Value-to-Weight density sort.
    // - Intuition: Greedily pick items with highest value per unit weight.
    // - Complexity: Time: O(N log N), Space: O(1).

    // Job Sequencing Problem with Deadlines (GFG / Striver SDE #46)
    struct Job { int id, dead, profit; };
    vector<int> JobScheduling(Job arr[], int n) {
        sort(arr, arr + n, [](const Job& a, const Job& b) {
            return a.profit > b.profit;
        });

        int maxDeadline = 0;
        for (int i = 0; i < n; i++) maxDeadline = max(maxDeadline, arr[i].dead);

        vector<int> slot(maxDeadline + 1, -1);
        int countJobs = 0, jobProfit = 0;

        for (int i = 0; i < n; i++) {
            for (int j = arr[i].dead; j > 0; j--) {
                if (slot[j] == -1) {
                    slot[j] = arr[i].id;
                    countJobs++;
                    jobProfit += arr[i].profit;
                    break;
                }
            }
        }
        return {countJobs, jobProfit};
    }
    // Interview Explanation:
    // - Problem Statement: Schedule jobs with unit time duration and deadlines to maximize profit (GFG / Striver SDE).
    // - Approach: Greedy Profit Sort + Latest Available Slot Scheduling.
    // - Intuition: Highest profit jobs should be scheduled as late as possible before deadline to free up earlier slots for other jobs.
    // - Complexity: Time: O(N log N + N * maxDeadline), Space: O(maxDeadline).

    // Lemonade Change (LeetCode 860)
    bool lemonadeChange(vector<int>& bills) {
        int five = 0, ten = 0;
        for (int b : bills) {
            if (b == 5) {
                five++;
            } else if (b == 10) {
                if (five == 0) return false;
                five--;
                ten++;
            } else { // b == 20
                if (ten > 0 && five > 0) {
                    ten--;
                    five--;
                } else if (five >= 3) {
                    five -= 3;
                } else {
                    return false;
                }
            }
        }
        return true;
    }
    // Interview Explanation:
    // - Problem Statement: Determine if you can provide correct change to every customer given $5, $10, $20 bills (LeetCode 860).
    // - Approach: Greedy Change Priority.
    // - Intuition: $5 bills are more versatile than $10 bills. Prioritize giving $10 + $5 over three $5s.
    // - Complexity: Time: O(N), Space: O(1).

    // Maximum Units on a Truck (LeetCode 1710)
    int maximumUnits(vector<vector<int>>& boxTypes, int truckSize) {
        sort(boxTypes.begin(), boxTypes.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[1] > b[1];
        });

        int totalUnits = 0;
        for (auto& box : boxTypes) {
            int boxesToTake = min(truckSize, box[0]);
            totalUnits += boxesToTake * box[1];
            truckSize -= boxesToTake;
            if (truckSize == 0) break;
        }
        return totalUnits;
    }
    // Interview Explanation:
    // - Problem Statement: Maximize units loaded on truck of capacity truckSize (LeetCode 1710).
    // - Approach: Fractional Knapsack Greedy sort by unit density.
    // - Complexity: Time: O(N log N), Space: O(1).

    // Wiggle Subsequence (LeetCode 376)
    int wiggleMaxLength(vector<int>& nums) {
        int n = nums.size();
        if (n < 2) return n;
        int prevDiff = 0, count = 1;
        for (int i = 1; i < n; i++) {
            int diff = nums[i] - nums[i - 1];
            if ((diff > 0 && prevDiff <= 0) || (diff < 0 && prevDiff >= 0)) {
                count++;
                prevDiff = diff;
            }
        }
        return count;
    }
    // Interview Explanation:
    // - Problem Statement: Find length of longest wiggle subsequence (alternating differences positive/negative) (LeetCode 376).
    // - Approach: Greedy Peak and Valley Counter.
    // - Complexity: Time: O(N), Space: O(1).

    // Maximum Bags With Full Capacity of Rocks (LeetCode 2279)
    int maximumBags(vector<int>& capacity, vector<int>& rocks, int additionalRocks) {
        int n = capacity.size();
        vector<int> diff(n);
        for (int i = 0; i < n; i++) diff[i] = capacity[i] - rocks[i];
        sort(diff.begin(), diff.end());

        int fullBags = 0;
        for (int i = 0; i < n; i++) {
            if (diff[i] == 0) {
                fullBags++;
            } else if (additionalRocks >= diff[i]) {
                additionalRocks -= diff[i];
                fullBags++;
            } else {
                break;
            }
        }
        return fullBags;
    }
    // Interview Explanation:
    // - Problem Statement: Maximize bags with full capacity placing additionalRocks (LeetCode 2279).
    // - Approach: Greedy Smallest-Gap Filling.
    // - Intuition: Sort required rock capacity `diff[i]` ascending; filling smallest gaps first maximizes total full bags.
    // - Complexity: Time: O(N log N), Space: O(N).


    // =========================================================
    // 6. ADVANCED GREEDY & PRIORITY QUEUE ENGINES
    // =========================================================

    // Course Schedule III (LeetCode 630)
    int scheduleCourse(vector<vector<int>>& courses) {
        // Sort by lastDay ascending
        sort(courses.begin(), courses.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[1] < b[1];
        });

        priority_queue<int> maxHeap; // stores duration of accepted courses
        int totalTime = 0;

        for (auto& c : courses) {
            int duration = c[0], lastDay = c[1];
            if (totalTime + duration <= lastDay) {
                totalTime += duration;
                maxHeap.push(duration);
            } else if (!maxHeap.empty() && maxHeap.top() > duration) {
                // Greedily swap with longest past course to free up time
                totalTime -= maxHeap.top() - duration;
                maxHeap.pop();
                maxHeap.push(duration);
            }
        }
        return maxHeap.size();
    }
    // Interview Explanation:
    // - Problem Statement: Maximize courses taken where course i takes duration[i] days and must finish by lastDay[i] (LeetCode 630).
    // - Approach: Sort by Deadline + Max-Heap Duration Replacement.
    // - Intuition: If a course exceeds deadline, replacing the longest previously taken course with current shorter course reduces `totalTime` without violating earlier deadlines.
    // - Complexity: Time: O(N log N), Space: O(N).

    // IPO (LeetCode 502)
    int findMaximizedCapital(int k, int w, vector<int>& profits, vector<int>& capital) {
        int n = profits.size();
        vector<pair<int, int>> projects(n);
        for (int i = 0; i < n; i++) projects[i] = {capital[i], profits[i]};
        sort(projects.begin(), projects.end()); // Min-heap / sorted by required capital

        priority_queue<int> maxProfitHeap;
        int projIdx = 0;

        for (int i = 0; i < k; i++) {
            while (projIdx < n && projects[projIdx].first <= w) {
                maxProfitHeap.push(projects[projIdx].second);
                projIdx++;
            }
            if (maxProfitHeap.empty()) break;
            w += maxProfitHeap.top();
            maxProfitHeap.pop();
        }
        return w;
    }
    // Interview Explanation:
    // - Problem Statement: Maximize total capital starting with w choosing up to k distinct projects (LeetCode 502).
    // - Approach: Two-Heap / Sorted Array + Max-Heap Profit Priority.
    // - Intuition: At each step, push all affordable projects (`capital <= w`) into Max-Heap; greedily select project with maximum profit.
    // - Complexity: Time: O(N log N + K log N), Space: O(N).

    // Minimum Cost to Connect Sticks / Huffman Coding (LeetCode 1167 / GFG)
    int connectSticks(vector<int>& sticks) {
        priority_queue<int, vector<int>, greater<int>> minHeap(sticks.begin(), sticks.end());
        int totalCost = 0;
        while (minHeap.size() > 1) {
            int first = minHeap.top(); minHeap.pop();
            int second = minHeap.top(); minHeap.pop();
            int combined = first + second;
            totalCost += combined;
            minHeap.push(combined);
        }
        return totalCost;
    }
    // Interview Explanation:
    // - Problem Statement: Connect all sticks into one with cost equal to sum of lengths (LeetCode 1167 / Huffman merge).
    // - Approach: Greedy Min-Heap pairwise merge.
    // - Intuition: Merging the two shortest sticks at each step ensures shorter sticks participate in more addition operations than longer ones.
    // - Complexity: Time: O(N log N), Space: O(N).

    // Advantage Shuffle / Tian Ji Horse Racing (LeetCode 870)
    vector<int> advantageCount(vector<int>& nums1, vector<int>& nums2) {
        int n = nums1.size();
        sort(nums1.begin(), nums1.end());

        priority_queue<pair<int, int>> pq;
        for (int i = 0; i < n; i++) pq.push({nums2[i], i});

        int left = 0, right = n - 1;
        vector<int> ans(n);

        while (!pq.empty()) {
            auto [val, idx] = pq.top(); pq.pop();
            if (nums1[right] > val) {
                ans[idx] = nums1[right--]; // Beat opponent's strongest horse
            } else {
                ans[idx] = nums1[left++];  // Sacrifice weakest horse
            }
        }
        return ans;
    }
    // Interview Explanation:
    // - Problem Statement: Permute nums1 to maximize indices where `nums1[i] > nums2[i]` (LeetCode 870).
    // - Approach: Tian Ji Horse Racing Strategy with Sorting + Two Pointers.
    // - Intuition: Against opponent's largest element, if our largest element beats it, use it; otherwise sacrifice our smallest element.
    // - Complexity: Time: O(N log N), Space: O(N).

    // Minimum Number of Increments on Subarrays to Form a Target Array (LeetCode 1526)
    int minNumberOperations(vector<int>& target) {
        int operations = target[0];
        for (int i = 1; i < (int)target.size(); i++) {
            if (target[i] > target[i - 1]) {
                operations += target[i] - target[i - 1];
            }
        }
        return operations;
    }
    // Interview Explanation:
    // - Problem Statement: Find minimum operations to form target array from all zeroes by incrementing subarrays (LeetCode 1526).
    // - Approach: Greedy Delta Accumulation.
    // - Intuition: Operations are only needed when ascending from `target[i-1]` to a higher level `target[i]`; descending steps are naturally covered by previous operations.
    // - Complexity: Time: O(N), Space: O(1).
};

